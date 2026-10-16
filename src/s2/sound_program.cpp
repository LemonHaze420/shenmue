#include "s2/sound_program.hpp"

#include "s2/archive_data.hpp"
#include "s2/audio_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/door.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/model_component.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/pak.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <span>
#include <string_view>

namespace shenmue::s2 {
std::int32_t g_audioPlaybackEnabled = 1;
std::int32_t g_audioPlaybackSuppressed{};
std::int32_t g_forceDreamcastSoundData{};

namespace {

constexpr shen_name soundTaskToken{SHEN_NAME(SOND)};
constexpr shen_name eventTaskToken{SHEN_NAME(EVNT)};
constexpr std::uint32_t smapTag = SHEN_NAME(SMAP);
constexpr std::uint32_t smpxTag = SHEN_NAME(SmpX);
constexpr std::uint32_t sbtsTag = SHEN_NAME(SBTS);

SoundProgramRuntimeState runtime{};
HLib::Task *soundTask = nullptr;
std::span<const std::byte> activeSmap{};
std::span<const std::byte> currentSoundMap{};
std::array<char, 8> lastCutsceneLookupName{};
std::array<char, 16> savedProgramName{};
std::array<char, 32> savedSoundDataName{};
std::int64_t savedSceneIndex{};
std::int32_t pendingSoundSceneIndex{-1};
std::int32_t samplePlaybackSuppressed{};
std::int32_t aicaEmulationEnabled{};
std::array<SampleProgramRecord, 85> sampleRecords{};
std::array<SampleProgramPool, 4> samplePools{{
    {sampleRecords.data(), 5, -1},
    {sampleRecords.data() + 5, 10, -1},
    {sampleRecords.data() + 15, 10, -1},
    {sampleRecords.data() + 25, 60, -1},
}};

void RefreshSampleExecutionState() noexcept {
    runtime.sampleExecutionActive =
        std::any_of(runtime.samplePrograms.begin(), runtime.samplePrograms.end(), [](const SampleProgramState &program) { return program.executing != 0; });
}

SampleProgramState *SelectSampleProgram(std::size_t first) noexcept {
    for (std::size_t index = first; index < first + 2; ++index) {
        SampleProgramState &program = runtime.samplePrograms[index];
        if (program.records && runtime.banks[static_cast<std::size_t>(program.bank)].loaded)
            return &program;
    }
    return nullptr;
}


std::array<MaterialSoundPair, 369> materialSounds{};
std::array<MaterialSoundBank, 8> materialBanks{{
    {materialSounds.data(), 1, -1, 0},
    {materialSounds.data() + 1, 1, -1, 0},
    {materialSounds.data() + 2, 1, -1, 0},
    {materialSounds.data() + 3, 24, -1, 0},
    {materialSounds.data() + 27, 48, -1, 0},
    {materialSounds.data() + 75, 64, -1, 0},
    {materialSounds.data() + 139, 110, -1, 0},
    {materialSounds.data() + 249, 120, -1, 0},
}};
std::array<SoundEffectHandle, MATERIAL_SOUND_HANDLE_COUNT> materialSoundHandles{};
std::int32_t soundListenerUsesCamera{};

// StripDryMask uses dryMaterialKeys from g_stepEnvIds[26..73].
// The remaining pointer at 74 is not a material row (it points at a string).
constexpr std::uint16_t dryMaterialKeys[48][4] = {
    {1467,1468,2466,2469}, {1467,1468,2466,2469},
    {1467,1468,2466,2469}, {1467,1468,2466,2469},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {1467,1468,2466,2469}, {1467,1468,2466,2469},
    {2141,2142,2467,2470}, {2153,2154,2468,2471},
    {2863,2862,2858,2859}, {2856,2857,2861,2860},
    {1469,1470,2472,2475}, {1469,1470,2472,2475},
    {2143,2144,2473,2476}, {2155,2156,2474,2477},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {1471,1472,2478,2481}, {1471,1472,2478,2481},
    {2145,2146,2479,2482}, {2157,2158,2480,2483},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {1473,1474,2147,2148}, {1473,1474,2147,2148},
    {2147,2148,2159,2160}, {2159,2160,1475,1476},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {1475,1476,2149,2150}, {1475,1476,2149,2150},
    {2149,2150,2161,2162}, {2161,2162,1477,1478},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {1477,1478,2854,2855}, {1477,1478,2854,2855},
    {2151,2152,2163,2164}, {2163,2164,2848,2849},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
    {2848,2849,0,0}, {2848,2849,0,0},
    {2151,2152,2163,2164}, {2163,2164,2848,2849},
    {2661,2662,2663,2664}, {2850,2851,2852,2853},
};
std::uint32_t soundProgramPhaseFlags[3]{};

template <typename T> T Read(const std::byte *source) noexcept {
    T value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

bool Contains(std::span<const std::byte> bytes, std::size_t offset, std::size_t size) noexcept {
    return offset <= bytes.size() && size <= bytes.size() - offset;
}

std::span<const std::byte> ChunkBytes(const MapInfoChunk *chunk) noexcept {
    if (!chunk || chunk->nextByteOffset < sizeof(MapInfoChunk))
        return {};
    return {reinterpret_cast<const std::byte *>(chunk), chunk->nextByteOffset};
}

const MapInfoChunk *FindChildChunk(const MapInfoChunk *parent, std::uint32_t name) noexcept {
    const std::span<const std::byte> bytes = ChunkBytes(parent);
    std::size_t offset = sizeof(MapInfoChunk);
    while (Contains(bytes, offset, sizeof(MapInfoChunk))) {
        const auto *child = reinterpret_cast<const MapInfoChunk *>(bytes.data() + offset);
        if (child->name.name == name)
            return child;
        if (child->nextByteOffset < sizeof(MapInfoChunk) || child->nextByteOffset > bytes.size() - offset) {
            return nullptr;
        }
        offset += child->nextByteOffset;
    }
    return nullptr;
}

bool SameName(std::string_view left, std::string_view right) noexcept {
    if (left.size() != right.size())
        return false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        const unsigned char a = static_cast<unsigned char>(left[i]);
        const unsigned char b = static_cast<unsigned char>(right[i]);
        const unsigned char lowerA = a >= 'A' && a <= 'Z' ? static_cast<unsigned char>(a + 32) : a;
        const unsigned char lowerB = b >= 'A' && b <= 'Z' ? static_cast<unsigned char>(b + 32) : b;
        if (lowerA != lowerB)
            return false;
    }
    return true;
}

std::string_view FixedName(const char *name, std::size_t capacity) noexcept {
    std::size_t length = 0;
    while (length < capacity && name[length])
        ++length;
    return {name, length};
}

void CopyFixedName(std::span<char> destination, std::string_view source) noexcept {
    const std::size_t count = std::min(source.size(), destination.size() - 1);
    if (count)
        std::memmove(destination.data(), source.data(), count);
    std::fill(destination.begin() + count, destination.end(), '\0');
}

std::string_view ProgramStem(std::string_view name) noexcept {
    const std::size_t dot = name.find('.');
    return dot == std::string_view::npos ? name : name.substr(0, dot);
}

std::span<const std::byte> FindSoundRecordInTable(
    std::span<const std::byte> table, std::uint32_t tag, std::string_view name, std::size_t offset = sizeof(MapInfoChunk)) noexcept {
    std::array<char, 8> key{};
    name = ProgramStem(name).substr(0, key.size());
    std::memcpy(key.data(), name.data(), name.size());
    while (Contains(table, offset, 16)) {
        const std::byte *const record = table.data() + offset;
        const std::uint32_t recordTag = Read<std::uint32_t>(record);
        if (!recordTag)
            return {};
        const std::uint32_t next = Read<std::uint32_t>(record + 12);
        if (next < 16 || next > table.size() - offset)
            return {};
        if (recordTag == tag && std::memcmp(record + 4, key.data(), key.size()) == 0) {
            return table.subspan(offset, next);
        }
        offset += next;
    }
    return {};
}

std::span<const std::byte> FindSoundRecord(std::uint32_t tag, std::string_view name) noexcept {
    // FindSoundRecord checks ready PACK tables before the borrowed map SNDM.
    const LoadedMapPackage *package = nullptr;
    while ((package = GetNextReadySoundPackage(package)) != nullptr) {
        const auto record = FindSoundRecordInTable(GetPackageSoundTable(*package), tag, name);
        if (!record.empty())
            return record;
    }
    return FindSoundRecordInTable(currentSoundMap, tag, name, 0);
}

std::span<const std::byte> FindSbt(std::string_view filename) noexcept {
    return FindSoundRecord(sbtsTag, filename);
}

std::int32_t Dispatch(AicaBackendOperation operation, std::uint32_t command = 0, std::uint32_t bank = 0, std::int32_t channel = 0, std::int32_t value = 0,
    float scalar = 0.0f, std::string_view path = {}, std::span<const std::byte> data = {}) noexcept {
    return SubmitAicaBackendRequest(AicaBackendRequest{operation, command, bank, channel, value, scalar, path, data.data(), data.size()});
}

bool MainSoundCommand(std::uint32_t command) noexcept {
    if (Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a) < 0)
        return false;
    return Dispatch(AicaBackendOperation::MainCommand, command) >= 0;
}

bool ChannelSoundCommand(std::uint32_t command, std::uint32_t bank, std::int32_t channel, std::int32_t value) noexcept {
    if (Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a) < 0)
        return false;
    return Dispatch(AicaBackendOperation::TwoChannelCommand, command, bank, channel, value) >= 0;
}

bool SoundListenerPosition(Vector3 &position) noexcept {
    if (!g_cameraControl)
        return false;
    if (soundListenerUsesCamera) {
        camCtrlGetNextCamPos(&position);
        return true;
    }
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control || !control->ryo)
        return false;
    Matrix4x4 world{};
    if (!ResolveCharacterWorldTransform(static_cast<const Character *>(control->ryo), world))
        return false;
    position = {world.rows[3].x, world.rows[3].y, world.rows[3].z};
    return true;
}

float SoundDistanceGain(const Vector3 &listener, const Vector3 &position, float minimumDistance, float fadeDistance) noexcept {
    const float dx = position.x - listener.x;
    const float dz = position.z - listener.z;
    const float dy = position.y == 0.0f ? 0.0f : position.y - listener.y;
    const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (distance < minimumDistance)
        return 127.0f;
    if (fadeDistance == 0.0f || distance > minimumDistance + fadeDistance)
        return 0.0f;
    return (fadeDistance - (distance - minimumDistance)) * (127.0f / fadeDistance);
}

std::int32_t SoundPan(const Vector3 &position, bool &behind) noexcept {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const auto angle = static_cast<std::int16_t>(0x4000 - g_cameraControl->nextRotation.yaw + FixedAtan2(camera.x - position.x, camera.z - position.z));
    behind = angle < 0;
    const auto folded = behind ? static_cast<std::int16_t>(~angle) : angle;
    const auto relative = static_cast<std::int16_t>(folded - 0x4000);

    return std::clamp(-((static_cast<std::int32_t>(relative) * 64) >> 14), -40, 40);
}

bool ResolveMaterialSound(
    std::uint32_t flags, std::uint32_t keyMask, float context, MaterialSoundPair &sound, std::uint32_t &bank, std::int32_t &environment) noexcept {
    std::uint32_t key = flags & keyMask;
    environment = static_cast<std::int32_t>((flags >> 19) & 15U);
    if ((flags & 0x40000U) != 0) {
        const std::size_t row = 6 * ((key >> 4) & 7U) + (key & 7U);
        if (row >= std::size(dryMaterialKeys))
            return false;
        key = dryMaterialKeys[row][(key >> 8) & 3U];
        if (environment == 0) {
            if (!g_cameraControl)
                return false;
            Vector3 cameraPosition{};
            camCtrlGetNextCamPos(&cameraPosition);
            environment = std::max(static_cast<std::int32_t>(QueryGroundMaterialToken(cameraPosition, context) >> 16) - 4, 0);
        }
    }
    for (const MaterialSoundBank &entry : materialBanks) {
        if (entry.bank < 0)
            continue;
        for (std::uint32_t index = 0; index < entry.count; ++index) {
            if (entry.pairs[index].material != key)
                continue;
            bank = static_cast<std::uint32_t>(entry.bank);
            sound = entry.pairs[index];
            return true;
        }
    }
    return false;
}

std::span<const std::byte> LoadMaterialSoundTable(std::span<const std::byte> bytes, std::uint32_t bank) noexcept {
    if (!Contains(bytes, 0, 4))
        return {};
    if (Read<std::uint32_t>(bytes.data()) != SHEN_NAME(SGTS))
        return bytes;
    if (!Contains(bytes, 0, 16))
        return {};
    const std::size_t offset = std::to_integer<std::uint8_t>(bytes[12]);
    const std::size_t stride = std::to_integer<std::uint8_t>(bytes[13]);
    const std::uint16_t count = Read<std::uint16_t>(bytes.data() + 14);
    if (stride != sizeof(MaterialSoundPair) || offset < 16 || !Contains(bytes, offset, stride * count))
        return {};
    for (MaterialSoundBank &entry : materialBanks) {
        if (entry.bank >= 0 || count > entry.capacity)
            continue;
        std::memcpy(entry.pairs, bytes.data() + offset, stride * count);
        entry.bank = static_cast<std::int32_t>(bank);
        entry.count = count;
        break;
    }
    return bytes.subspan(offset + stride * count);
}
std::span<const std::byte> LoadBankSoundTable(std::span<const std::byte> bytes, std::uint32_t bank) noexcept {
    if (!Contains(bytes, 0, 4))
        return {};
    const std::uint32_t tag = Read<std::uint32_t>(bytes.data());
    if (tag == SHEN_NAME(SGTS))
        return LoadMaterialSoundTable(bytes, bank);
    if (tag != SHEN_NAME(AMBS) && tag != SHEN_NAME(AMSS))
        return bytes;
    if (!Contains(bytes, 0, 32))
        return {};
    const std::size_t offset = std::to_integer<std::uint8_t>(bytes[12]);
    const std::size_t stride = std::to_integer<std::uint8_t>(bytes[13]);
    const std::uint16_t count = Read<std::uint16_t>(bytes.data() + 14);
    if (stride != sizeof(SampleProgramRecord) || !Contains(bytes, offset, stride * count))
        return {};
    const std::size_t first = tag == SHEN_NAME(AMBS) ? 0 : 2;
    for (std::size_t index = first; index < first + 2; ++index) {
        SampleProgramState &program = runtime.samplePrograms[index];
        if (program.records)
            continue;
        program.executing = 0;
        for (SampleProgramPool &pool : samplePools) {
            if (pool.bank >= 0 || count > pool.capacity)
                continue;
            std::memcpy(pool.records, bytes.data() + offset, stride * count);
            program.command = Read<std::uint32_t>(bytes.data() + 28);
            program.count = count;
            program.bank = static_cast<std::int32_t>(bank);
            program.records = pool.records;
            pool.bank = static_cast<std::int32_t>(bank);
            break;
        }
        break;
    }
    RefreshSampleExecutionState();
    return bytes.subspan(offset + stride * count);
}

void UnloadBank(std::size_t bank) noexcept {
    SoundBankState &state = runtime.banks[bank];
    for (MaterialSoundBank &entry : materialBanks) {
        if (entry.bank != static_cast<std::int32_t>(bank))
            continue;
        entry.bank = -1;
        break;
    }
    if (state.loaded) {
        (void)ExecuteAicaCommand(0x1100a0U, static_cast<std::uint32_t>(bank), 0);
        (void)Dispatch(AicaBackendOperation::UnloadBank, 0, static_cast<std::uint32_t>(bank));
    }
    for (std::size_t slot = 0; slot < 2; ++slot) {
        SampleProgramState *program = nullptr;
        if (runtime.samplePrograms[slot].bank == static_cast<std::int32_t>(bank))
            program = &runtime.samplePrograms[slot];
        else if (runtime.samplePrograms[slot + 2].bank == static_cast<std::int32_t>(bank))
            program = &runtime.samplePrograms[slot + 2];
        if (!program)
            continue;
        program->records = nullptr;
        program->executing = 0;
        break;
    }
    for (SampleProgramPool &pool : samplePools)
        if (pool.bank == static_cast<std::int32_t>(bank))
            pool.bank = -1;
    RefreshSampleExecutionState();
    const std::int32_t allocation = state.allocation;
    state = {};
    state.allocation = allocation;
    runtime.bankFades[bank] = {.volume = 127.0f, .step = 0.0f};
    if (runtime.activeProgramCommand == static_cast<std::int32_t>(bank))
        runtime.activeProgramCommand = -1;
}

bool SelectDreamcastSoundFilename(char *selected, const char *directory, const char *filename) noexcept {
    std::strcpy(selected, filename);
    std::strcpy(std::strstr(selected, ".snd"), "_dc.snd");
    char path[256];
    std::snprintf(path, sizeof(path), "%s/%s", directory, selected);
    return ArchiveData_GetFileSize(path) != 0;
}

void SelectSoundBankFilename(char *selected, const char *directory, const char *filename, std::uint32_t bank) noexcept {
    std::strcpy(selected, filename);
    if (bank == 5) {
        for (const char *name : DREAMCAST_MUSIC_BANK_NAMES) {
            if (std::strstr(filename, name))
                (void)SelectDreamcastSoundFilename(selected, directory, filename);
        }
    } else if (bank == 4) {
        if (GetConversationAudioLanguage() == 1)
            (void)SelectDreamcastSoundFilename(selected, directory, filename);
    } else {
        if (GetConversationAudioLanguage() == 1) {
            for (const char *name : JAPANESE_DREAMCAST_SOUND_BANK_NAMES) {
                if (std::strstr(filename, name))
                    (void)SelectDreamcastSoundFilename(selected, directory, filename);
            }
        }
        if (std::strstr(filename, DREAMCAST_AMBIENT_BANK_NAME))
            (void)SelectDreamcastSoundFilename(selected, directory, filename);
    }
    if (g_forceDreamcastSoundData && !SelectDreamcastSoundFilename(selected, directory, filename)) {
        std::strcpy(selected, filename);
    }
}

bool LoadBank(std::int32_t scene, std::size_t bank, std::string_view filename, std::int32_t allocation) noexcept {
    if (bank >= runtime.banks.size() || filename.empty() || filename.size() >= runtime.banks[bank].filename.size()) {
        return false;
    }
    SoundBankState &state = runtime.banks[bank];
    if (SameName(FixedName(state.filename.data(), state.filename.size()), filename) && state.loaded && (allocation < 0 || state.allocation == allocation)) {
        return true;
    }

    const std::int32_t retainedAllocation = state.allocation;
    UnloadBank(bank);
    char directory[40];
    if (scene >= 0)
        std::snprintf(directory, sizeof(directory), "SCENE/%02d/SOUND", scene);
    else
        std::strcpy(directory, "MISC");
    char original[17];
    std::memcpy(original, filename.data(), filename.size());
    original[filename.size()] = 0;
    if (std::strcmp(original, "yuan.snd") == 0 && GetConversationAudioLanguage() == 1)
        std::strcpy(original, "yuan_dc.snd");
    char selected[64];
    SelectSoundBankFilename(selected, directory, original, static_cast<std::uint32_t>(bank));
    std::array<char, 96> path{};
    const int written = std::snprintf(path.data(), path.size(), "%s/%s", directory, selected);
    if (written <= 0 || static_cast<std::size_t>(written) >= path.size())
        return false;

    const std::uint64_t size = ArchiveData_GetFileSize(path.data());
    if (!size || size > static_cast<std::uint64_t>(SIZE_MAX))
        return false;
    std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, SOUND_BANK_DATA));
    if (!data)
        return false;
    std::int32_t result = -1;
    if (ArchiveData_Read(path.data(), 0, data, size)) {
        const std::span<const std::byte> payload = LoadBankSoundTable({data, static_cast<std::size_t>(size)}, static_cast<std::uint32_t>(bank));
        if (!payload.empty()) {
            std::size_t transferSize = payload.size();
            if (size > 0x20) {
                const HLib::TaskData *const allocation = reinterpret_cast<const HLib::TaskData *>(data - sizeof(HLib::TaskData));
                if (allocation->next) {

                    transferSize = HLib::GetTaskDataSize(data) + sizeof(HLib::TaskData) - static_cast<std::size_t>(payload.data() - data);
                }
            }
            result = static_cast<std::int32_t>(LoadAudioIntoAicaMemory({payload.data(), transferSize}, static_cast<std::uint32_t>(bank)));
        }
    }
    HLib::FreeTaskData(data);
    if (result < 0)
        return false;

    CopyFixedName(state.filename, filename);
    state.allocation = allocation >= 0 ? allocation : retainedAllocation;
    state.currentVolume = 127;
    state.loaded = true;
    runtime.bankFades[bank] = {.volume = 127.0f, .step = 0.0f};
    return true;
}

bool LoadSbtBanks(std::string_view sbtName, std::int32_t &pendingSceneIndex, std::int32_t actNumber) noexcept {
    const std::span<const std::byte> stbs = FindSbt(sbtName);
    if (!Contains(stbs, 0x20, 8 * 8))
        return false;
    bool loaded = false;
    for (std::size_t bank = 0; bank < 8; ++bank) {
        const char *raw = reinterpret_cast<const char *>(stbs.data() + 0x20 + bank * 8);
        const std::string_view stem = FixedName(raw, 8);
        if (stem.empty())
            continue;
        std::array<char, 13> filename{};
        CopyFixedName(filename, stem);
        std::memcpy(filename.data() + stem.size(), ".snd", 5);
        loaded |= LoadBank(actNumber, bank, FixedName(filename.data(), filename.size()), -1);
    }
    CopyFixedName(runtime.soundDataName, sbtName);
    if (pendingSceneIndex >= 0) {
        (void)LoadSoundProgram(runtime.soundDataName.data(), pendingSceneIndex, static_cast<std::uint32_t>(g_actNumber));
        pendingSceneIndex = -1;
    }
    return loaded;
}

void ApplySmapBanks(std::span<const std::byte> smap, std::int32_t actNumber) noexcept {
    if (!Contains(smap, 16, 8 * 20))
        return;

    std::size_t firstChanged = runtime.banks.size();
    for (std::size_t bank = 0; bank < runtime.banks.size(); ++bank) {
        const std::int32_t allocation = Read<std::int32_t>(smap.data() + 16 + bank * 20);
        if (allocation != -1 && allocation != runtime.banks[bank].allocation) {
            firstChanged = bank;
            break;
        }
    }
    for (std::size_t offset = runtime.banks.size(); offset > firstChanged; --offset) {
        const std::size_t bank = offset - 1;
        UnloadBank(bank);
        const std::int32_t allocation = Read<std::int32_t>(smap.data() + 16 + bank * 20);
        if (allocation == -1)
            continue;
        runtime.banks[bank].allocation = allocation;
        (void)Dispatch(AicaBackendOperation::SetBankAllocation, 0, static_cast<std::uint32_t>(bank), 0, allocation * 8);
    }

    for (std::size_t bank = 0; bank < runtime.banks.size(); ++bank) {
        const std::size_t recordOffset = 16 + bank * 20;
        const char *raw = reinterpret_cast<const char *>(smap.data() + recordOffset + 4);
        const std::string_view filename = FixedName(raw, 16);
        if (filename.empty()) {
            (void)ExecuteAicaCommand(0x1100a0U, static_cast<std::uint32_t>(bank), 0);
            if (runtime.selectedProgramBank == static_cast<std::int32_t>(bank))
                runtime.activeProgramCommand = -1;
            runtime.banks[bank].loaded = false;
            for (SampleProgramState &program : runtime.samplePrograms)
                if (program.bank == static_cast<std::int32_t>(bank))
                    program.executing = 0;
            continue;
        } else if (filename.front() == '@') {
            continue;
        } else if (filename.find(".sbt") != std::string_view::npos) {
            (void)LoadSbtBanks(filename, pendingSoundSceneIndex, actNumber);
        } else {
            (void)LoadBank(actNumber, bank, filename, -1);
        }
    }
    for (SoundBankFade &fade : runtime.bankFades)
        fade.step = 0.0f;
    (void)BeginPrimarySampleExecution();
    (void)BeginSecondarySampleExecution();
}

bool CompareCondition(std::int32_t comparison, std::int32_t left, std::int32_t right) noexcept {
    switch (comparison) {
    case 0:
        return left == right;
    case 1:
        return left <= right;
    case 2:
        return left >= right;
    case 3:
        return left < right;
    case 4:
        return left > right;
    case 5:
        return left != right;
    default:
        return false;
    }
}


const char *snddProgramName{};
std::int32_t snddSceneIndex{-1};



// @verified - p
std::int32_t SnddProgramSceneCount() noexcept {
    std::string_view sbtName{};
    const std::span<const std::byte> smap = FindSoundRecord(smapTag, snddProgramName ? FixedName(snddProgramName, 16) : std::string_view{});
    for (std::size_t bank = 0; bank < 8 && !smap.empty(); ++bank) {
        const std::size_t offset = 20 + bank * 20;
        if (!Contains(smap, offset, 1))
            break;
        const char *const name = reinterpret_cast<const char *>(smap.data() + offset);
        if (std::strstr(name, ".sbt")) {
            sbtName = name;
            break;
        }
    }
    const std::span<const std::byte> stbs = FindSbt(sbtName.empty() ? std::string_view{"AR02_000.sbt"} : sbtName);
    if (!Contains(stbs, 22, 1))
        return 0;
    return static_cast<std::int8_t>(std::to_integer<std::uint8_t>(stbs[22]));
}

} // namespace




// @verified - p
void SNDD_Reader(const MapInfoChunk *sndd, std::int32_t, std::int32_t entryId) noexcept {
    if (!sndd)
        return;
    const std::byte *cursor = reinterpret_cast<const std::byte *>(sndd) + sizeof(MapInfoChunk);
    std::int32_t comparison = 0;
    std::int32_t mode = 0;
    std::array<std::int32_t, 2> values{};
    DateTime now{};
    GetCurrentDateTime(now);
    std::int32_t op = Read<std::int32_t>(cursor);
    if (!op)
        return;
    std::int32_t right = values[1];
    std::int32_t left = values[0];
    do {
        cursor += 4;
        if (op >= 0) {
            if (op == 4) {
                snddProgramName = reinterpret_cast<const char *>(cursor);
                cursor += 16;
            } else if (op == 5) {
                const std::int32_t scene = Read<std::int32_t>(cursor);
                cursor += 4;
                if (g_doorQueuedAuthor.name == 0)
                    snddSceneIndex = scene;
            }
        } else if (op == -2) {
            cursor += Read<std::int32_t>(cursor);
        } else if (op == -1) {
            std::int32_t token = Read<std::int32_t>(cursor);
            std::size_t count = 0;
            if (token >= 0) {
                do {
                    cursor += 4;
                    bool stored = true;
                    std::int32_t value = 0;
                    if (token == 16) {
                        value = Read<std::int32_t>(cursor);
                        cursor += 4;
                    } else if (token < 6) {
                        comparison = token;
                        stored = false;
                    } else if (token < 16) {
                        mode = token;
                        stored = false;
                    } else {
                        switch (token) {
                        case 17:
                            value = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(Read<std::int32_t>(cursor))) != 0;
                            cursor += 4;
                            break;
                        case 18:
                            value = entryId;
                            break;
                        case 19:
                            value = now.month;
                            break;
                        case 20:
                            value = now.day;
                            break;
                        case 21:
                            value = now.hour;
                            break;
                        case 22:
                            value = now.minute;
                            break;
                        case 23:
                            value = SnddProgramSceneCount();
                            break;
                        case 24:
                            value = static_cast<std::int32_t>(GetCurrentStep());
                            break;
                        case 25:
                            value = EV_GetShenmueFlag(Read<std::int32_t>(cursor));
                            cursor += 4;
                            break;
                        default:
                            stored = false;
                            break;
                        }
                    }
                    if (stored) {
                        if (count < values.size())
                            values[count] = value;
                        ++count;
                    }
                    token = Read<std::int32_t>(cursor);
                } while (token >= 0);
                right = values[1];
                left = values[0];
            }
            const std::byte *const skip = cursor + 4;
            if (count < 2) {
                right = 0;
                comparison = 5;
                values[1] = 0;
            }
            bool hit = CompareCondition(comparison, left, right);
            if (mode == 6)
                hit = !hit;
            cursor = skip + (hit ? 4 : Read<std::int32_t>(skip));
        }
        op = Read<std::int32_t>(cursor);
    } while (op != 0);
}

namespace {

std::uint32_t FadeDuration(std::span<const std::byte> smap, std::size_t bank, bool fadeOut) noexcept {
    if (!Contains(smap, 176, 32) || Read<std::uint32_t>(smap.data() + 176) != smpxTag)
        return fadeOut ? defaultSampleFadeOut[bank] : defaultSampleFadeIn[bank];
    return std::to_integer<std::uint8_t>(smap[(fadeOut ? 200 : 192) + bank]);
}

// @verified - p
bool UpdateVolumeFades() noexcept {
    bool issued = false;
    for (std::size_t bank = 0; bank < runtime.banks.size(); ++bank) {
        SoundBankFade &fade = runtime.bankFades[bank];
        const float step = fade.step;
        if (step == 0.0f)
            continue;
        const float current = fade.volume;
        std::int32_t output = TruncateToInt32(current);
        if (!(step > 0.0f) && output <= 0) {
            output = 0;
            fade.step = 0.0f;
        } else if (step > 0.0f && output >= 127) {
            output = 127;
            fade.step = 0.0f;
        }
        fade.volume = current + fade.step;
        runtime.banks[bank].currentVolume = output;
        (void)ExecuteAicaCommand(0x4a0U, static_cast<std::uint32_t>(bank), output & 0x7f);
        issued = true;
    }
    return issued;
}

void SelectFadeIn(std::span<const std::byte> smap) noexcept {
    for (std::size_t bank = 0; bank < runtime.banks.size(); ++bank) {
        const std::uint32_t duration = FadeDuration(smap, bank, false);
        if (duration != 0 && Dispatch(AicaBackendOperation::QueryBankSequenceState, 0, static_cast<std::uint32_t>(bank)) > 0) {
            runtime.bankFades[bank].volume = 0.0f;
            runtime.bankFades[bank].step = 127.0f / static_cast<float>(duration);
        } else {
            runtime.bankFades[bank].step = 0.0f;
        }
    }
}
void SampleChannelBankCommand(const SampleProgramState &program, const SampleProgramRecord &record, std::uint32_t command) noexcept {
    for (std::uint8_t channel : record.channels) {
        if (channel == 0)
            break;
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::BankCommand, command, static_cast<std::uint8_t>(program.bank), 0, channel);
    }
}

void SampleChannelCommand(const SampleProgramState &program, const SampleProgramRecord &record, std::uint32_t command, std::int16_t value) noexcept {
    for (std::uint8_t channel : record.channels) {
        if (channel == 0)
            break;
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::TwoChannelCommand, command, static_cast<std::uint8_t>(program.bank), static_cast<std::int8_t>(channel), value);
    }
}

void UpdateSampleProgram(SampleProgramState *program) noexcept {
    if (!program || program->executing == 0)
        return;
    DateTime date{};
    GetCurrentDateTime(date);
    const std::int32_t quarter = 4 * date.hour + date.minute / 15;
    const std::int32_t nextQuarter = quarter + 1 <= 95 ? quarter + 1 : 0;
    const float minute = static_cast<float>(date.minute % 15);
    const std::int16_t yaw = static_cast<std::int16_t>(camCtrlGetNextRotY());
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Vector3 listener = camera;
    if (!soundListenerUsesCamera) {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        listener = GetCharacterWorldPosition(static_cast<const Character *>(control->ryo));
    }
    Vector3 source{};
    for (std::int32_t index = 0; index < program->count; ++index) {
        SampleProgramRecord &record = program->records[index];
        if (record.disabled != 0 || ((1U << date.week) & record.weekdays) == 0)
            continue;
        float gain = 127.0f;
        if (record.spatialMode == 1) {
            if (record.characterName != 0) {
                Character *const character = FindCharacterByScenarioName(shen_name{record.characterName});
                if (!character)
                    continue;
                source = GetCharacterWorldPosition(character);
            } else {
                source = record.position;
            }
            gain = SoundDistanceGain(listener, source, record.minimumDistance, record.fadeDistance);
        }
        if (gain > 0.0f) {
            if (record.timeMode != 0) {
                float timeGain = static_cast<float>(record.quarterHourGains[static_cast<std::size_t>(quarter)]) / 100.0f;
                if (record.timeMode == 1)
                    timeGain +=
                        ((static_cast<float>(record.quarterHourGains[static_cast<std::size_t>(nextQuarter)]) / 100.0f) - timeGain) * 0.066660002f * minute;
                gain *= timeGain;
            }
            if (gain > 0.0f) {
                switch (record.weatherMode) {
                case 1:
                    gain -= GetWeatherWindVector() * gain;
                    break;
                case 2: {
                    const float pressure = GetWeatherPressure();
                    const float factor = pressure <= 0.01f ? GetWeatherWindVector() * 0.44999999f : (pressure + 1.0f) * 0.44999999f;
                    gain -= factor * gain;
                    break;
                }
                case 3:
                    gain = std::fmin(127.0f, GetWeatherPressure() * gain + 32.0f);
                    break;
                default:
                    break;
                }
            }
        }
        std::int32_t volume = static_cast<std::int32_t>(gain);
        if (volume > 127)
            volume = 127;
        else if (volume < 64)
            volume = 0;
        if (volume > 0 && record.panMode != 0) {
            const std::int16_t angle = static_cast<std::int16_t>(0x4000 - yaw + FixedAtan2(camera.x - source.x, camera.z - source.z));
            const bool behind = angle < 0;
            const std::int16_t folded = behind ? static_cast<std::int16_t>(~angle) : angle;
            const std::int16_t relative = static_cast<std::int16_t>(folded - 0x4000);
            const std::int32_t pan = std::clamp(-((static_cast<std::int32_t>(relative) * 64) >> 14), -40, 40);
            if (pan != record.pan) {
                SampleChannelCommand(*program, record, 0xab0U, static_cast<std::int16_t>(pan));
                record.pan = static_cast<std::int8_t>(pan);
            }
            if (record.frontGainPercent < 100) {
                const std::int32_t attenuation = behind ? static_cast<std::int32_t>((1.0f - static_cast<float>(record.frontGainPercent) / 100.0f) * 127.0f *
                                                              (1.0f - std::fabs(static_cast<float>(pan) * 0.015625f))) :
                                                          0;
                if (attenuation != static_cast<std::int8_t>(record.attenuation)) {
                    SampleChannelCommand(*program, record, 0x2b0U, static_cast<std::int16_t>(attenuation));
                    record.attenuation = static_cast<std::uint8_t>(attenuation);
                }
            }
        }
        if (samplePlaybackSuppressed != 0) {
            SampleChannelBankCommand(*program, record, 0x28a0U);
            SampleChannelCommand(*program, record, 0x7b0U, 0);
        } else if (volume != static_cast<std::int8_t>(record.volume)) {
            if (volume != 0) {
                if (record.volume == 0)
                    SampleChannelBankCommand(*program, record, 0x29a0U);
                if (runtime.callbackParity == static_cast<std::uint32_t>(volume & 1))
                    continue;
            } else {
                SampleChannelBankCommand(*program, record, 0x28a0U);
            }
            SampleChannelCommand(*program, record, 0x7b0U, static_cast<std::int16_t>(volume));
            record.volume = static_cast<std::uint8_t>(volume);
        }
    }
}

void UpdateSampleProgramDucking(SampleProgramState *program, bool fadeCommandIssued) noexcept {
    if (!program || program->executing == 0)
        return;
    if (FindConversationByPriority(6)) {
        if (program->duckVolume == 0) {
            program->duckDelay = 2;
            program->duckVolume = 127;
        } else if (program->duckVolume > 95) {
            program->duckVolume = std::max(program->duckVolume - 2, 95);
        } else {
            return;
        }
    } else {
        if (program->duckVolume == 0)
            return;
        if (program->duckVolume >= 127) {
            program->duckVolume = 0;
            return;
        }
        if (program->duckDelay != 0) {
            --program->duckDelay;
            return;
        }
        ++program->duckVolume;
        program->duckDelay = 2;
    }
    if (program->duckVolume >= 0 && !fadeCommandIssued)
        (void)ExecuteAicaCommand(0x4a0U, static_cast<std::uint8_t>(program->bank), program->duckVolume & 0x7f);
}

// @verified - p
void UpdateSelectedSoundProgram(bool fadeCommandIssued) noexcept {
    const SelectedSoundProgramRecord *const record = runtime.selectedRecord;
    if (!record)
        return;
    std::uint32_t requested = 0xffffffffU;
    if (record->startFrame == runtime.frame) {
        runtime.musicFadeFrames = -1;
        runtime.musicVolume = static_cast<float>(record->volume);
        const std::uint32_t command = (static_cast<std::uint32_t>(record->sequence) << 8U) + 0xa8U;
        std::array<char, 8> name{};
        for (std::size_t index = 0; index < name.size() && record->bankName[index] && record->bankName[index] != '.'; ++index)
            name[index] = record->bankName[index];
        std::array<char, 8> activeName{};
        for (std::size_t index = 0; index < activeName.size() && runtime.activeMusicName[index] && runtime.activeMusicName[index] != '.'; ++index)
            activeName[index] = runtime.activeMusicName[index];
        if (record->restart || command != static_cast<std::uint32_t>(runtime.activeProgramCommand) ||
            std::memcmp(name.data(), activeName.data(), name.size()) != 0 || record->bank != runtime.selectedProgramBank) {
            (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
            (void)Dispatch(AicaBackendOperation::BankCommand, 0x4a0U, record->bank, 0, record->volume & 0x7fU);
            if (g_audioPlaybackEnabled != 0 && g_audioPlaybackSuppressed == 0)
                (void)ExecuteAicaCommand(command, 0, 0);
            runtime.activeProgramCommand = static_cast<std::int32_t>(command);
            std::strncpy(runtime.activeMusicName.data(), record->bankName, runtime.activeMusicName.size());
            runtime.selectedProgramBank = record->bank;
            runtime.publishedMusicVolume = record->volume;
            runtime.musicDuckVolume = 0;
        }
    }
    if (record->panFrame != 0 && record->panFrame == runtime.frame) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0xaa0U, record->bank, 0, record->pan & 0x7fU);
    }
    for (const SoundProgramFadeRecord &fade : record->fades) {
        if (fade.frame == 0xffffU)
            break;
        if (fade.frame == runtime.frame) {
            runtime.musicFadeFrames = static_cast<std::int32_t>(fade.duration) + 1;
            runtime.musicVolumeStep = (static_cast<float>(fade.volume) - runtime.musicVolume) / static_cast<float>(runtime.musicFadeFrames);
        }
    }
    if (runtime.musicFadeFrames > 0) {
        runtime.musicVolume += runtime.musicVolumeStep;
        if (runtime.musicVolume > 127.0f)
            runtime.musicVolume = 127.0f;
        else if (runtime.musicVolume < 0.0f)
            runtime.musicVolume = 0.0f;
        --runtime.musicFadeFrames;
        requested = static_cast<std::uint32_t>(static_cast<std::int32_t>(runtime.musicVolume));
    }
    SampleProgramState *const samples = SelectSampleProgram(0);
    if (samples && samples->executing != 0) {
        SampleProgramRecord *loudest = samples->records;
        for (std::int32_t index = 1; index < samples->count; ++index)
            if (static_cast<std::int8_t>(loudest->volume) < static_cast<std::int8_t>(samples->records[index].volume))
                loudest = samples->records + index;
        if (loudest) {
            const float volume = static_cast<float>(static_cast<std::int8_t>(loudest->volume));
            const float minimum = std::fmax(64.0f, static_cast<float>(static_cast<std::int8_t>(loudest->minimumMusicVolume)));
            if (volume > 64.0f) {
                requested =
                    static_cast<std::uint32_t>(static_cast<std::int32_t>((1.0f - (volume - 64.0f) / 63.0f) * (runtime.musicVolume - minimum) + minimum));
                if (static_cast<std::int32_t>(static_cast<std::uint32_t>(runtime.publishedMusicVolume) - requested) > 2)
                    requested = static_cast<std::uint32_t>(runtime.publishedMusicVolume - 2);
            }
        }
    }
    std::int32_t duck = runtime.musicDuckVolume;
    if (FindConversationByPriority(6)) {
        if (duck != 0) {
            if (duck > 95)
                runtime.musicDuckVolume = --duck;
        } else if (runtime.musicVolume > 95.0f) {
            duck = static_cast<std::int32_t>(runtime.musicVolume);
            runtime.musicDuckVolume = duck;
            runtime.musicDuckDelay = 2;
        }
    } else if (duck != 0) {
        if (duck < static_cast<std::int32_t>(runtime.musicVolume)) {
            if (runtime.musicDuckDelay != 0)
                --runtime.musicDuckDelay;
            else {
                runtime.musicDuckVolume = ++duck;
                runtime.musicDuckDelay = 2;
            }
        } else
            runtime.musicDuckVolume = 0;
    }
    if (runtime.musicDuckVolume > 0 && static_cast<std::uint32_t>(runtime.musicDuckVolume) < requested)
        requested = static_cast<std::uint32_t>(runtime.musicDuckVolume);
    std::int32_t volume = requested >= 65U ? static_cast<std::int32_t>(requested) : 0;
    if (volume >= 0 && runtime.publishedMusicVolume != volume) {
        if (volume > 127)
            volume = 127;
        if (runtime.activeProgramCommand >= 0 && !fadeCommandIssued) {
            (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
            (void)Dispatch(AicaBackendOperation::BankCommand, 0x4a0U, record->bank, 0, volume & 0x7f);
            runtime.publishedMusicVolume = volume;
        }
    }
}

} // namespace

void InitAica() noexcept {
    if (aicaEmulationEnabled)
        return;
    (void)InitializeS2AicaMiddleware();
    for (std::size_t bank = runtime.banks.size(); bank != 0; --bank)
        runtime.banks[bank - 1].filename[0] = 0;
    runtime.activeProgramCommand = -1;
    runtime.pauseDepth = 0;
    soundListenerUsesCamera = 0;
    for (SampleProgramState &program : runtime.samplePrograms) {
        program.executing = 0;
        program.records = nullptr;
    }
    pendingSoundSceneIndex = -1;
    for (MaterialSoundBank &bank : materialBanks)
        bank.bank = -1;
    for (std::size_t bank = 0; bank < INITIAL_SOUND_BANK_ALLOCATIONS.size(); ++bank) {
        if (bank < runtime.banks.size())
            runtime.banks[bank].allocation = INITIAL_SOUND_BANK_ALLOCATIONS[bank] / 8;
        (void)Dispatch(AicaBackendOperation::SetBankAllocation, 0, static_cast<std::uint32_t>(bank), 0, INITIAL_SOUND_BANK_ALLOCATIONS[bank]);
    }
    std::memset(savedProgramName.data(), 0, sizeof(std::uint32_t));
    savedSoundDataName[0] = 0;
    savedSceneIndex = 0;
    (void)LoadBank(-1, 0, "system1.snd", -1);
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 90);
    (void)Dispatch(AicaBackendOperation::MainCommand, 0xa8U);
    for (std::size_t bank = 0; bank < INITIAL_SOUND_BANK_VOLUMES.size(); ++bank) {
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x11a0U, static_cast<std::uint32_t>(bank), 0, INITIAL_SOUND_BANK_VOLUMES[bank]);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x5a0U, static_cast<std::uint32_t>(bank), 0, INITIAL_SOUND_BANK_VOLUMES[bank]);
    }
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 90);
    aicaEmulationEnabled = 1;
    lastCutsceneLookupName.fill(0);
    samplePlaybackSuppressed = 0;
    g_s2CurrentSfxVolume = GetS2AudioSettings().sfxVolume;
    g_s2CurrentMusicVolume = GetS2AudioSettings().musicVolume;
}

// @verified - p
void DisableAicaEmulation() noexcept {
    aicaEmulationEnabled = 0;
}

void DisableAicaIfEnabled() noexcept {
    if (aicaEmulationEnabled) {
        ReleaseS2AicaMiddleware();
        aicaEmulationEnabled = 0;
    }
}

const SoundProgramRuntimeState &GetSoundProgramRuntimeState() noexcept {
    return runtime;
}
void SetSamplePlaybackSuppressed(std::int32_t suppressed) noexcept {
    samplePlaybackSuppressed = suppressed;
}

std::uint64_t GetSamplePlaybackSuppressed() noexcept {
    return static_cast<std::uint32_t>(samplePlaybackSuppressed);
}

// @verified - p
std::uint64_t FindMaterialSoundBank(std::uint32_t material) noexcept {
    for (const MaterialSoundBank &entry : materialBanks) {
        if (entry.bank < 0)
            continue;
        for (std::uint32_t index = 0; index < entry.count; ++index)
            if (entry.pairs[index].material == material)
                return static_cast<std::uint32_t>(entry.bank);
    }
    return 0xffffffffULL;
}

// @verified - p
std::int64_t ExecuteMaterialSoundHandleCommand(SoundEffectHandle *handle, std::uint32_t command, std::int32_t value) noexcept {
    std::int32_t *cachedValue = nullptr;
    switch (command) {
    case MATERIAL_SOUND_GAIN_COMMAND:
        cachedValue = &handle->gain;
        break;
    case MATERIAL_SOUND_VOLUME_COMMAND:
        cachedValue = &handle->volume;
        break;
    case MATERIAL_SOUND_CONTROL_COMMAND:
        cachedValue = &handle->control;
        break;
    default:
        break;
    }
    if (cachedValue) {
        if (*cachedValue == value)
            return 1;
        *cachedValue = value;
    }
    if (handle->active == 1)
        (void)ExecuteAicaCommand(command, handle->slot, value);
    return 1;
}

// @verified - p
std::int64_t FreeMaterialSoundHandle(SoundEffectHandle *handle) noexcept {
    if (handle->slot <= MATERIAL_SOUND_HANDLE_COUNT) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::WideBankCommand, MATERIAL_SOUND_GAIN_COMMAND, handle->slot, 0, 0);
    }
    handle->active = 0;
    return 1;
}


std::int32_t aicaSetMainCommand(std::uint32_t command) noexcept {
    return Dispatch(AicaBackendOperation::MainCommand, command);
}

std::int32_t ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) noexcept {
    if ((command & 0xf0U) == 0xb0U) {
        std::int32_t channel = static_cast<std::int32_t>(command & 0x0fU);
        if (channel == 0)
            channel = 16;
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        return Dispatch(AicaBackendOperation::TwoChannelCommand, command & 0xfffff0U, bank & 0xffU, channel, value & 0x7f);
    }

    const std::uint32_t operation = command & 0xffU;
    if (operation >= 168U) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        return Dispatch(AicaBackendOperation::MainCommand, command);
    }
    if (operation >= 165U) {
        if (bank > 16U)
            return static_cast<std::int32_t>(operation);
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        return Dispatch(AicaBackendOperation::WideBankCommand, command, bank, 0, value);
    }
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
    return Dispatch(AicaBackendOperation::BankCommand, command, bank, 0, value & 0x7f);
}

// @verified - p
std::uint64_t ExecuteAicaBankCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) noexcept {
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
    return static_cast<std::uint32_t>(Dispatch(AicaBackendOperation::BankCommand, command, static_cast<std::uint16_t>(bank), 0, value));
}
// @verified - p
std::uint64_t LoadAudioIntoAicaMemory(std::span<const std::byte> data, std::uint32_t bank) noexcept {
    (void)Dispatch(AicaBackendOperation::BankCommand, 0x1100a0U, bank & 0x1fU);
    (void)Dispatch(AicaBackendOperation::UnloadBank, 0, bank);
    (void)Dispatch(AicaBackendOperation::LoadBank, 0, bank, 0, -1, 0.0f, {}, data);
    return 0;
}

// @verified - p
std::int64_t ProcessAicaCommand(std::uint32_t command, std::int32_t value) noexcept {
    if (runtime.activeProgramCommand == -1)
        return 0;
    (void)ExecuteAicaCommand(command, static_cast<std::uint32_t>(runtime.selectedProgramBank), value);
    runtime.activeProgramCommand = -1;
    return 1;
}


SoundEffectHandle *AllocateMaterialSoundHandle(std::int32_t key) noexcept {
    std::uint32_t slot = 0;
    for (; slot < materialSoundHandles.size(); ++slot) {
        if (materialSoundHandles[slot].active == 0)
            break;
    }
    if (slot == materialSoundHandles.size())
        return nullptr;
    for (MaterialSoundBank &bank : materialBanks) {
        if (bank.bank < 0)
            continue;
        for (std::int32_t index = 0; index < static_cast<std::int32_t>(bank.count); ++index) {
            const MaterialSoundPair &sound = bank.pairs[index];
            if (sound.material != static_cast<std::uint32_t>(key))
                continue;
            const std::uint32_t command = (sound.command >> 24) >= 0x80U ? sound.command & 0x00ffffffU : 0;
            SoundEffectHandle &handle = materialSoundHandles[slot];
            handle.active = 1;
            handle.command = static_cast<std::int32_t>(command);
            handle.slot = slot;
            handle.gain = 0;
            handle.volume = 0;
            handle.control = 0;
            (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
            (void)Dispatch(AicaBackendOperation::WideBankCommand, MATERIAL_SOUND_SETUP_COMMAND, slot, 0, handle.command);
            return &handle;
        }
    }
    return nullptr;
}

std::int64_t Loop3DSoundEffect(std::uint32_t flags, float context) noexcept {
    MaterialSoundPair sound{};
    std::uint32_t bank{};
    std::int32_t environment{};
    if (!ResolveMaterialSound(flags, 0x87ffffU, context, sound, bank, environment)) {
        if ((static_cast<std::uint8_t>(flags) - 168U) > 7U)
            return 0;
        (void)ExecuteAicaCommand(flags, 0, 0);
        return 1;
    }
    const std::int32_t channel = static_cast<std::uint8_t>(sound.command >> 24) + 1;
    if (environment > 0) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 90);
        (void)Dispatch(AicaBackendOperation::TwoChannelCommand, 0x2db0U, bank, channel, std::min(environment * 9, 127));
    }
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 90);
    (void)Dispatch(AicaBackendOperation::MainCommand, sound.command & 0xffffffU);
    if (environment > 0) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 90);
        (void)Dispatch(AicaBackendOperation::TwoChannelCommand, 0x2db0U, bank, channel, 0);
    }
    return 1;
}

static bool ExecuteResolved3DSoundCommand(std::uint32_t flags, const Vector3 &position, float minimumDistance, float fadeDistance,
    const MaterialSoundPair &sound, std::uint32_t bank, std::int32_t environment) noexcept {
    Vector3 listener{};
    if (!SoundListenerPosition(listener))
        return false;
    float gain = SoundDistanceGain(listener, position, minimumDistance, fadeDistance);
    const std::uint32_t baseVolume = flags >> 24U;
    if (baseVolume != 0 && baseVolume < 127)
        gain *= static_cast<float>(baseVolume) / 127.0f;
    if (runtime.bankFades[bank].step != 0.0f) {
        const std::int32_t volume = TruncateToInt32(runtime.bankFades[bank].volume);
        if (volume <= 127)
            gain *= static_cast<float>(std::max(volume, 0)) / 127.0f;
    }
    if (!std::isfinite(gain) || gain < 1.0f)
        return false;
    bool behind{};
    const std::int32_t pan = SoundPan(position, behind);
    const std::int32_t channel = static_cast<std::uint8_t>(sound.command >> 24U) + 1;
    const bool persistentMix = (flags & 0x20000U) != 0;
    const std::uint32_t volumeCommand = persistentMix ? 0x7b0U : 0x27b0U;
    const std::uint32_t panCommand = persistentMix ? 0xab0U : 0x2ab0U;
    bool success = true;
    if (environment > 0)
        success = ChannelSoundCommand(0x2db0U, bank, channel, std::min(environment * 9, 127));
    if (success)
        success = ChannelSoundCommand(volumeCommand, bank, channel, static_cast<std::int32_t>(gain));
    if (success)
        success = ChannelSoundCommand(panCommand, bank, channel, pan);
    if (success)
        success = MainSoundCommand(sound.command & 0xffffffU);
    if (!persistentMix) {
        success = ChannelSoundCommand(panCommand, bank, channel, 0) && success;
        success = ChannelSoundCommand(volumeCommand, bank, channel, 127) && success;
    }
    if (environment > 0)
        success = ChannelSoundCommand(0x2db0U, bank, channel, 0) && success;
    return success;
}

bool Execute3DSoundCommand(std::uint32_t flags, const Vector3 &position, float minimumDistance, float fadeDistance, float context) noexcept {
    MaterialSoundPair sound{};
    std::uint32_t bank{};
    std::int32_t environment{};
    if (!ResolveMaterialSound(flags, 0x85ffffU, context, sound, bank, environment))
        return false;
    return ExecuteResolved3DSoundCommand(flags, position, minimumDistance, fadeDistance, sound, bank, environment);
}

bool PlayFootstepForNode(Character *character, std::uint32_t flags, bool debugPulse) noexcept {
    if (SubmitFootstepAudioRequest({character, flags}))
        return true;

    std::uint32_t material = flags & 0x85ffffU;
    if (material >= 0x800000U) {
        const std::int32_t step = ResolveFootstepGroundToken(character);
        if (step < 0)
            return false;
        const std::int32_t environment = std::min(step >> 16, 11);
        material = ResolveFootstepMaterialKey(character, static_cast<std::uint16_t>(step), material);
        flags = (flags & ~0x00f5ffffU) | material | (static_cast<std::uint32_t>(environment) << 19U);
    }

    const Vector3 position = GetCharacterWorldPosition(character);
    MaterialSoundPair sound{};
    std::uint32_t bank{};
    std::int32_t environment{};
    if (!ResolveMaterialSound(flags, 0x85ffffU, position.y, sound, bank, environment))
        return false;
    const std::int32_t channel = static_cast<std::uint8_t>(sound.command >> 24U) + 1;
    bool success = true;
    if (debugPulse)
        success = ChannelSoundCommand(0x1b0U, bank, channel, 1);

    if (character) {
        success = ExecuteResolved3DSoundCommand(flags, position, 2.0f, 30.0f, sound, bank, environment) && success;
    } else {
        if (environment > 0)
            success = ChannelSoundCommand(0x2db0U, bank, channel, std::min(environment * 9, 127)) && success;
        const std::uint32_t baseVolume = flags >> 24U;
        if (baseVolume == 127U) {
            success = MainSoundCommand(sound.command & 0xffffffU) && success;
        } else {
            success = ChannelSoundCommand(0x27b0U, bank, channel, static_cast<std::int32_t>(baseVolume)) && success;
            success = MainSoundCommand(sound.command & 0xffffffU) && success;
            success = ChannelSoundCommand(0x27b0U, bank, channel, 127) && success;
        }
        if (environment > 0)
            success = ChannelSoundCommand(0x2db0U, bank, channel, 0) && success;
    }
    if (debugPulse)
        success = ChannelSoundCommand(0x1b0U, bank, channel, 0) && success;
    return success;
}

// @verified - p
void SetSoundListenerUsesCamera(std::int32_t enabled) noexcept {
    soundListenerUsesCamera = enabled;
}

void QueueSoundProgramControl() noexcept {
    soundTask = HLib::EnqueueTaskWithoutParameter(SoundProgramControl_Callback, 0x0e, 3, static_cast<std::int32_t>(soundTaskToken.name));
    if (!soundTask)
        return;
    runtime.queued = true;
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
    (void)Dispatch(AicaBackendOperation::BankCommand, 0x400a0U);
    for (SoundEffectHandle &handle : materialSoundHandles)
        handle.active = 0;
    runtime.frame = 0;
    runtime.selectedRecord = nullptr;
    for (SoundBankFade &fade : runtime.bankFades)
        fade.step = 0.0f;
    soundListenerUsesCamera = 0;
    while (runtime.pauseDepth != 0)
        PrimeAicaEmulationForPause(0);
}


void CopySoundProgramKey(std::array<char, 12> &key, const char *name) noexcept {
    std::size_t index = 0;
    for (; index < 8 && name[index] != '.' && name[index] != 0; ++index)
        key[index] = name[index];
    for (; index < 8; ++index)
        key[index] = 0;
}




// @verified - p
std::int64_t SNDP_SoundProgramReader(const MapInfoChunk *sndp, std::int32_t sceneId, shen_name map) noexcept {
    currentSoundMap = {};
    if (!sndp)
        return 0;
    pendingSoundSceneIndex = -1;
    std::array<char, 12> key{};
    const MapInfoChunk *const sndm = FindChildChunk(sndp, SHEN_NAME(SNDM));
    if (!sndm)
        return 0;
    const std::span<const std::byte> soundRecords = ChunkBytes(sndm);
    currentSoundMap = soundRecords.empty() ? soundRecords : soundRecords.subspan(sizeof(MapInfoChunk));
    if (const MapInfoChunk *const snda = FindChildChunk(sndp, SHEN_NAME(SNDA))) {
        const char *const name = reinterpret_cast<const char *>(snda) + sizeof(MapInfoChunk);
        if (std::memcmp(name, "default", 8) != 0)
            CopySoundProgramKey(key, name);
    }
    if (const MapInfoChunk *const sndr = FindChildChunk(sndp, SHEN_NAME(SNDR)))
        pendingSoundSceneIndex = Read<std::int32_t>(reinterpret_cast<const std::byte *>(sndr) + sizeof(MapInfoChunk));
    if (const MapInfoChunk *const sndd = FindChildChunk(sndp, SHEN_NAME(SNDD))) {
        snddProgramName = nullptr;
        snddSceneIndex = -1;
        SNDD_Reader(sndd, sceneId, g_entryPoint);
        if (snddProgramName)
            CopySoundProgramKey(key, snddProgramName);
        if (snddSceneIndex >= 0)
            pendingSoundSceneIndex = snddSceneIndex;
    }
    if (key[0] == 0) {
        std::memcpy(key.data(), &map.name, 4);
        std::memset(key.data() + 4, 0, 4);
    }
    if (soundProgramPhaseFlags[2] != 0 && std::strncmp(key.data(), "QDKJ", 4) == 0) {
        soundProgramPhaseFlags[2] = 0;
        return 0;
    }
    activeSmap = FindSoundRecordInTable(currentSoundMap, smapTag, FixedName(key.data(), 8), 0);
    if (activeSmap.empty())
        return 0;
    CopyFixedName(runtime.programName, FixedName(key.data(), 8));
    ApplySmapBanks(activeSmap, sceneId);
    UpdateSelectedSoundProgram(false);
    ++runtime.frame;
    return 1;
}

// @verified - p
std::uint64_t GetSoundProgramPhaseFlag(std::int32_t selector) noexcept {
    switch (selector) {
    case 0:
        return soundProgramPhaseFlags[0];
    case 1:
        return soundProgramPhaseFlags[1];
    case 2:
        return soundProgramPhaseFlags[2];
    default:
        return 0;
    }
}

void ResetSoundProgramPhaseFlags(std::int32_t selector) noexcept {
    switch (selector) {
    case 0:
        soundProgramPhaseFlags[0] = 0;
        break;
    case 1:
        soundProgramPhaseFlags[1] = 0;
        break;
    case 2:
        soundProgramPhaseFlags[2] = 0;
        break;
    case 3:
        soundProgramPhaseFlags[1] = 0;
        soundProgramPhaseFlags[2] = 0;
        soundProgramPhaseFlags[0] = 0;
        break;
    default:
        break;
    }
}

// @verified - p
void SetSoundProgramPhaseFlags(std::int32_t selector) noexcept {
    switch (selector) {
    case 0:
        soundProgramPhaseFlags[0] = 1;
        break;
    case 1:
        soundProgramPhaseFlags[1] = 1;
        break;
    case 2:
        soundProgramPhaseFlags[2] = 1;
        break;
    case 3:
        soundProgramPhaseFlags[1] = 1;
        soundProgramPhaseFlags[2] = 1;
        soundProgramPhaseFlags[0] = 1;
        break;
    default:
        break;
    }
}

std::int64_t SaveCurrentSoundProgramContext() noexcept {
    // SaveCurrentSoundProgramContext snapshots names/index; it does not load.
    CopyFixedName(savedProgramName, FixedName(runtime.programName.data(), runtime.programName.size()));
    CopyFixedName(savedSoundDataName, FixedName(runtime.soundDataName.data(), runtime.soundDataName.size()));
    savedSceneIndex = runtime.selectedSceneIndex;
    return savedSceneIndex;
}

// @verified - p
void shenAssignSoundProgramByPtr(std::span<const std::byte> table) noexcept {
    currentSoundMap = table;
}

// @verified - p
std::uint64_t shenAssignActiveSmp(const char *name) noexcept {
    return LoadCutsceneSoundProgram(name, -1);
}

std::uint64_t LoadCutsceneSoundProgram(const char *name, std::int32_t actNumber) noexcept {
    std::strncpy(lastCutsceneLookupName.data(), name, lastCutsceneLookupName.size());
    const std::span<const std::byte> record = FindSoundRecord(smapTag, name);
    if (record.empty())
        return 0;
    activeSmap = record;
    CopyFixedName(runtime.programName, ProgramStem(name).substr(0, 8));
    ApplySmapBanks(record, actNumber);
    return 1;
}

// @verified - p
std::uint64_t EV_LoadCutsceneFromCurrentScene(const char *name) noexcept {
    return LoadCutsceneSoundProgram(name, g_actNumber);
}

bool LoadCutsceneAndSoundProgramForArea() noexcept {
    // LoadCutsceneAndSoundProgramForArea preserves the second load when the first fails.
    if (!savedProgramName[0])
        return false;
    const bool loaded = LoadCutsceneSoundProgram(savedProgramName.data(), g_actNumber) != 0;
    if (savedSoundDataName[0]) {
        return LoadSoundProgram(savedSoundDataName.data(), savedSceneIndex, static_cast<std::uint32_t>(g_actNumber)) != 0;
    }
    return loaded;
}

std::int64_t spmBeginSampleExecute(std::uint32_t flags, SampleProgramState *program) noexcept {
    if (!program || !program->records || program->bank >= 8 || !runtime.banks[static_cast<std::size_t>(program->bank)].loaded || program->executing != 0)
        return 0;
    program->executing = 1;
    program->duckVolume = 0;
    (void)ExecuteAicaCommand(program->command, 0, 0);
    for (std::int32_t index = 0; index < program->count; ++index) {
        SampleProgramRecord &record = program->records[index];
        record.volume = 0;
        record.pan = 0;
        record.attenuation = 0;
        record.disabled = static_cast<std::uint8_t>(flags & 1U);
        flags >>= 1;
    }
    RefreshSampleExecutionState();
    return 1;
}

std::int64_t BeginPrimarySampleExecution(std::uint32_t flags) noexcept {
    return spmBeginSampleExecute(flags, SelectSampleProgram(0));
}

std::int64_t BeginSecondarySampleExecution(std::uint32_t flags) noexcept {
    return spmBeginSampleExecute(flags, SelectSampleProgram(2));
}

std::int64_t EV_spmBeginSampleExecute() noexcept {
    const std::span<const std::byte> smap = FindSoundRecord(smapTag, GetCurrentBgm());
    SelectFadeIn(smap);
    (void)BeginPrimarySampleExecution(0);
    return BeginSecondarySampleExecution(0);
}

std::int64_t spmSetSampleEnabled(std::int32_t sample, std::int32_t enabled) noexcept {
    if (sample == 0)
        return 0;
    SampleProgramState &program = *SelectSampleProgram(0);
    SampleProgramRecord &record = program.records[static_cast<std::uint32_t>(sample) - 1U];
    if (enabled != 0) {
        record.disabled = 0;
    } else {
        record.disabled = 1;
        record.volume = 0;
        record.pan = 0;
        record.attenuation = 0;
        SampleChannelBankCommand(program, record, 0x28a0U);
        SampleChannelCommand(program, record, 0x7b0U, 0);
    }
    return 1;
}

void spmFadeOut() noexcept {
    const std::span<const std::byte> smap = FindSoundRecord(smapTag, GetCurrentBgm());
    for (std::size_t bank = 0; bank < runtime.banks.size(); ++bank) {
        const std::uint32_t duration = FadeDuration(smap, bank, true);
        if (duration == 0 || Dispatch(AicaBackendOperation::QueryBankSequenceState, 0, static_cast<std::uint32_t>(bank)) <= 0) {
            runtime.bankFades[bank].step = 0.0f;
            continue;
        }
        const std::int32_t current = Dispatch(AicaBackendOperation::QueryBankVolume, 0x4a0U, static_cast<std::uint32_t>(bank));
        runtime.bankFades[bank].volume = static_cast<float>(current);
        runtime.bankFades[bank].step = -(static_cast<float>(current) / static_cast<float>(duration));
        if (runtime.selectedProgramBank == static_cast<std::int32_t>(bank)) {
            runtime.activeProgramCommand = -1;
        } else {
            for (SampleProgramState &program : runtime.samplePrograms) {
                if (program.bank != static_cast<std::int32_t>(bank))
                    continue;
                program.executing = 0;
                break;
            }
        }
    }
    RefreshSampleExecutionState();
    if (ConversationBuffer *const voice = FindConversationByPriority(1))
        hsFadeVoice(voice, 60);
}

// @verified - p
std::int64_t spmFadeInBank(std::int32_t bank, std::int32_t frames) noexcept {
    SoundBankFade &fade = runtime.bankFades.data()[bank];
    fade.volume = 0.0f;
    fade.step = 127.0f / static_cast<float>(frames);
    return bank;
}

// @verified - p
std::int64_t spmFadeOutBank(std::int32_t bank, std::int32_t frames) noexcept {
    const std::int32_t current = Dispatch(AicaBackendOperation::QueryBankVolume, 0x4a0U, static_cast<std::uint32_t>(bank));
    SoundBankFade &fade = runtime.bankFades.data()[bank];
    fade.volume = static_cast<float>(current);
    fade.step = -(fade.volume / static_cast<float>(frames));
    return static_cast<std::uint32_t>(current);
}

// @verified - p
bool AreSoundBankFadesComplete() noexcept {
    for (const SoundBankFade &fade : runtime.bankFades) {
        if (fade.step == 0.0f)
            continue;
        std::int32_t volume = TruncateToInt32(fade.volume);
        if (volume <= 127) {
            if (volume < 0)
                volume = 0;
        } else
            volume = -1;
        if (volume >= 0)
            return false;
    }
    return true;
}

std::int64_t LoadSoundProgram(const char *name, std::int64_t sceneIndex, std::uint32_t currentScene) noexcept {
    if (!name || !*name)
        return 0;
    if (sceneIndex == SOUND_PROGRAM_STOP_INDEX) {
        if (runtime.activeProgramCommand == -1)
            return 0;
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x1200a0U, static_cast<std::uint8_t>(runtime.selectedProgramBank), 0, 0);
        runtime.activeProgramCommand = -1;
        return 1;
    }
    const std::span<const std::byte> stbs = FindSbt(name);
    if (!Contains(stbs, 0, 23))
        return 0;
    const std::int32_t count = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(stbs[22]));
    const std::int32_t entries = 16 + static_cast<std::int8_t>(std::to_integer<std::uint8_t>(stbs[20]));
    if (count <= 0 || entries < 0 || !Contains(stbs, static_cast<std::size_t>(entries), static_cast<std::size_t>(count) * 96U))
        return 0;
    for (std::int32_t index = 0; index < count; ++index) {
        const std::byte *const entry = stbs.data() + entries + static_cast<std::size_t>(index) * 96U;
        if (std::to_integer<std::uint8_t>(entry[17]) != sceneIndex)
            continue;
        runtime.selectedSceneIndex = sceneIndex;
        CopyFixedName(runtime.soundDataName, std::string_view(name));
        std::array<char, 13> filename{};
        std::memcpy(filename.data(), entry + 4, 8);
        std::size_t length = 0;
        while (length < 8 && filename[length])
            ++length;
        std::memcpy(filename.data() + length, ".snd", 5);
        const std::size_t bank = std::to_integer<std::uint8_t>(entry[14]);
        (void)LoadBank(static_cast<std::int32_t>(currentScene), bank, FixedName(filename.data(), filename.size()), -1);
        if (bank < runtime.banks.size() && runtime.banks[bank].loaded) {
            runtime.frame = 0;
            runtime.selectedRecord = reinterpret_cast<const SelectedSoundProgramRecord *>(entry);
            return 1;
        }
        runtime.selectedRecord = nullptr;
        return 0;
    }
    return 0;
}

// @verified - p
std::int64_t GetSelectedSoundSceneIndex() noexcept {
    return runtime.selectedSceneIndex;
}

// @verified - p
char *shenGetCurrentSbt() noexcept {
    return runtime.soundDataName.data();
}

// @verified - p
char *GetCurrentBgm() noexcept {
    return runtime.programName.data();
}

// @verified - p
std::uint64_t GetSoundProgramSceneCount(const char *name) noexcept {
    const std::span<const std::byte> record = FindSbt(name ? name : shenGetCurrentSbt());
    if (!Contains(record, 22, 1))
        return 0;
    return static_cast<std::uint32_t>(static_cast<std::int8_t>(std::to_integer<std::uint8_t>(record[22])));
}

// @verified - p
std::int64_t LoadSoundProgramWithoutSelectedRecord(const char *name, std::uint32_t sceneIndex) noexcept {
    const std::int64_t result = LoadSoundProgram(name, sceneIndex, static_cast<std::uint32_t>(g_actNumber));
    runtime.frame = 0;
    runtime.selectedRecord = nullptr;
    return result;
}
const char *FindSoundProgramNameForMap(const char *name) noexcept {
    const std::span<const std::byte> record = FindSoundRecord(smapTag, name);
    if (!Contains(record, 16, 8 * 20))
        return nullptr;
    for (std::size_t bank = 0; bank < 8; ++bank) {
        const char *const candidate = reinterpret_cast<const char *>(record.data() + 20 + bank * 20);
        if (FixedName(candidate, 16).find(".sbt") != std::string_view::npos)
            return candidate;
    }
    return nullptr;
}

void StopSoundProgram() noexcept {
    if (runtime.activeProgramCommand == -1)
        return;
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
    (void)Dispatch(AicaBackendOperation::BankCommand, 0x1200a0U, static_cast<std::uint8_t>(runtime.selectedProgramBank), 0, 0);
    runtime.activeProgramCommand = -1;
}

void PrimeAicaEmulationForPause(std::int32_t enabled) noexcept {
    if (enabled) {
        if (runtime.pauseDepth == 0) {
            (void)ExecuteAicaCommand(0x1100a0U, 0, 0);
            (void)ExecuteAicaCommand(0x1300a0U, 0, 0);
            for (std::size_t offset = runtime.banks.size(); offset > 1; --offset) {
                const std::size_t bank = offset - 1;
                (void)ExecuteAicaCommand(0x2a0U, static_cast<std::uint32_t>(bank), 0);
                (void)ExecuteAicaCommand(0x1300a0U, static_cast<std::uint32_t>(bank), 0);
            }
            (void)Dispatch(AicaBackendOperation::PauseStreams);
        } else {
            (void)Dispatch(AicaBackendOperation::PauseStreams, 0, 0, 0, 254);
        }
        ++runtime.pauseDepth;
        return;
    }
    if (runtime.pauseDepth == 0)
        return;
    --runtime.pauseDepth;
    if (runtime.pauseDepth == 1) {
        (void)Dispatch(AicaBackendOperation::ResumeStreams, 0, 0, 0, 255);
        return;
    }
    if (runtime.pauseDepth != 0)
        return;

    for (std::size_t bank = 1; bank < runtime.banks.size(); ++bank) {
        (void)ExecuteAicaCommand(0x1300a0U, static_cast<std::uint32_t>(bank), 0);
        (void)ExecuteAicaCommand(0x5a0U, static_cast<std::uint32_t>(bank), runtime.banks[bank].currentVolume);
        (void)ExecuteAicaCommand(0x3a0U, static_cast<std::uint32_t>(bank), 0);
    }
    (void)Dispatch(AicaBackendOperation::ResumeStreams);
}

void SoundProgramControl_Callback(void *) {
    runtime.callbackParity ^= 1U;
    const bool fadeCommandIssued = UpdateVolumeFades();
    if (runtime.pauseDepth == 0) {
        UpdateSelectedSoundProgram(fadeCommandIssued);
        ++runtime.frame;
    }
    UpdateSampleProgram(SelectSampleProgram(0));
    UpdateSampleProgram(SelectSampleProgram(2));
    UpdateSampleProgramDucking(SelectSampleProgram(0), fadeCommandIssued);
    UpdateSampleProgramDucking(SelectSampleProgram(2), fadeCommandIssued);
    RefreshSampleExecutionState();
}

void ShutdownSoundProgramControl() noexcept {
    if (soundTask) {
        if (soundTask == HLib::GetCurrentTask())
            (void)HLib::DestroyCurrentTask();
        else
            (void)HLib::DestroyTask(soundTask);
    }
    soundTask = nullptr;
    activeSmap = {};
    currentSoundMap = {};
    runtime.queued = false;
}

void EVT_AT_FadeBGM_Callback(void *taskData) {
    auto *data = static_cast<EventFadeBgmTaskData *>(taskData);
    if (!data) {
        (void)UnregisterEventTask(HLib::GetCurrentTask());
        (void)HLib::DestroyCurrentTask();
        return;
    }
    if (data->time <= 0.0f) {
        if (data->mode != 0)
            spmFadeOut();
        else
            (void)EV_spmBeginSampleExecute();
        if (data->cleanupTask) {
            (void)UnregisterEventTask(HLib::GetCurrentTask());
            (void)HLib::DestroyCurrentTask();
        }
        return;
    }
    data->time -= HLib::GetScaledDeltaTick();
}

void EVT_AT_FadeBGM(std::int32_t frames, std::int32_t mode, std::int32_t delay) {
    if (delay == 0) {
        if (mode != 0)
            spmFadeOut();
        else
            (void)EV_spmBeginSampleExecute();
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_FadeBGM_Callback, 6, 0x0b, sizeof(EventFadeBgmTaskData), eventTaskToken);
    auto *const data = task ? static_cast<EventFadeBgmTaskData *>(HLib::GetTaskParameter(task)) : nullptr;
    if (!data)
        return;
    data->cleanupTask = 1;
    data->frames = frames;
    data->mode = mode;
    data->time = static_cast<float>(delay);
    (void)RegisterEventTask(task);
}

} // namespace shenmue::s2
