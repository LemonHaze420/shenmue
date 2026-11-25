#include "shenmue.h"
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include "s2/sound_program.hpp"
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/conversation_dc.hpp"
#include "platform/dc/aica.hpp"
#include "s2/camera_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/footstep_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/sound_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "d3t/archive_handles.hpp"
#include "s2/flag.hpp"
#include "s2/player.hpp"

#include "s2/archive_data.hpp"
#include "s2/audio_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include "s2/date_time.hpp"
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/map_info.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scene_state_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include "s2/story_state.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/weather.hpp"

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include <cstring>
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/file_loader_dc.hpp"
#include "s2/directory_cache.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include "s2/hlib.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <span>
#include <string_view>
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
extern "C" {
std::int32_t syCfgInit(void *buffer);
std::int32_t syCfgExit();
std::int32_t syCfgGetSoundMode(std::int32_t *mode);
std::int32_t syCfgSetSoundMode(std::int32_t mode);
}
#include "s2/math.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_data.hpp"
#include "platform/dc/simm.hpp"
#include "s2/character_table_dc.hpp"
#include <cstdio>
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
const std::uint16_t *soundEnvironmentKeyRows[8][6] = {{soundEnvironmentKeys + 0, soundEnvironmentKeys + 0, soundEnvironmentKeys + 0, soundEnvironmentKeys + 0,
                                                          soundEnvironmentKeys + 12, soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 0, soundEnvironmentKeys + 0, soundEnvironmentKeys + 4, soundEnvironmentKeys + 8, soundEnvironmentKeys + 32,
        soundEnvironmentKeys + 36},
    {soundEnvironmentKeys + 20, soundEnvironmentKeys + 20, soundEnvironmentKeys + 24, soundEnvironmentKeys + 28, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 40, soundEnvironmentKeys + 40, soundEnvironmentKeys + 44, soundEnvironmentKeys + 48, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 52, soundEnvironmentKeys + 52, soundEnvironmentKeys + 54, soundEnvironmentKeys + 56, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 58, soundEnvironmentKeys + 58, soundEnvironmentKeys + 60, soundEnvironmentKeys + 62, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 64, soundEnvironmentKeys + 64, soundEnvironmentKeys + 68, soundEnvironmentKeys + 70, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16},
    {soundEnvironmentKeys + 72, soundEnvironmentKeys + 72, soundEnvironmentKeys + 68, soundEnvironmentKeys + 70, soundEnvironmentKeys + 12,
        soundEnvironmentKeys + 16}};

struct ConversationBuffer;

bool FindMaterialSoundCommand(std::uint32_t material, MaterialSoundCommand *output);
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
bool LoadWholeSoundBank(const char *directory, const char *filename, std::int32_t bank);
bool LoadStreamedSoundBank(const char *directory, const char *filename, std::int32_t bank, std::uint32_t bufferBytes);
const std::uint32_t *LoadSoundBankMetadata(const std::uint32_t *data, std::int32_t bank);
static const std::byte *CopyMaterialSoundRecords(const SoundBankMetadataHeader *header, std::int32_t bank);
static const std::byte *CopySampleProgramRecords(const std::uint32_t *data, std::int32_t bank, SampleProgramState *program);
static std::int32_t IsSoundBankSlotNamed(const char *name, std::uint32_t bank);
void SetSoundBankAllocation(std::int32_t bank, std::uint32_t bytes);
std::int32_t LoadAicaDriver();
void ResetSampleProgram(SampleProgramState *program);
void sndClearSceneState();
std::int32_t DispatchSoundCommand(SoundCommandMode mode, const SoundDriverCommand *record);
SoundDriverCommand *SetSoundDriverCommand(SoundDriverCommand *record, std::uint32_t command, std::int32_t value);
void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
std::uint32_t GetSoundMaterialKey(std::uint32_t flags);
std::int32_t GetSoundEnvironment();
std::uint32_t GetSoundEnvironmentFromProperties(std::uint32_t properties);
std::int32_t GetSoundBankPlaybackState(std::int32_t bank);
char *GetCurrentBgm();
void AdvanceSelectedSoundProgram(bool fadeCommandIssued);
void UpdateSelectedSoundProgram(bool fadeCommandIssued);
void UpdateSampleProgramDucking(SampleProgramState *program, bool fadeCommandIssued);
std::int32_t SoundPan(std::int32_t *pan, std::int32_t yaw, const Vector3 *source, const Vector3 *camera);
float SoundDistanceGain(const Vector3 *listener, const Vector3 *source, float minimumDistance, float fadeDistance);
void UpdateSampleProgram(SampleProgramState *program);
std::int32_t SetSampleProgramEnabled(std::int32_t sample, std::int32_t enabled, SampleProgramState *program);
SampleProgramState *SelectSampleProgram(SampleProgramState *first);
void CopySoundProgramKey(const char *name, char *key);
const SoundDataHeader *FindSoundRecordInTable(const SoundDataHeader *table, shen_name tag, std::uint32_t key0, std::uint32_t key1);
const SoundDataHeader *FindSoundRecord(shen_name tag, const char *name);
const SoundSequenceHeader *FindSbt(const char *name);
void ResetBankVolumeFades();
void ConfigureSoundMap(const SoundMapRecord *record, std::int32_t act);

void CopySoundProgramKey(const char *name, char *key);
const SoundDataHeader *FindSoundRecordInTable(const SoundDataHeader *table, shen_name tag, std::uint32_t key0, std::uint32_t key1);
const SoundDataHeader *FindSoundRecord(shen_name tag, const char *name);
const SoundSequenceHeader *FindSbt(const char *name);
void ResetBankVolumeFades();
void ConfigureSoundMap(const SoundMapRecord *record, std::int32_t act);
const char *g_soundDefinitionProgramName = 0;
std::int32_t g_soundDefinitionSceneIndex = -1;
std::int32_t cvsHasValidConverseData(const ConversationBuffer *buffer);
char g_soundBankNames[8][16] = {};
std::uint32_t g_soundBankAllocations[8] = {};
std::int8_t g_soundBankLoadStates[8] = {};
SampleProgramState g_samplePrograms[2] = {};
SampleProgramState g_secondarySamplePrograms[2] = {};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::array<void *, 2> g_aicaMemoryArena{};
std::int32_t g_audioPlaybackEnabled = 1;
std::int32_t g_audioPlaybackSuppressed{};
std::int32_t g_forceDreamcastSoundData{};
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
MaterialSoundPair materialSounds[369] = {};
SampleProgramRecord sampleRecords[85] = {};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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

// Native SGTS pools: first free pool that fits, not bank-indexed.
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
    {1467, 1468, 2466, 2469},
    {1467, 1468, 2466, 2469},
    {1467, 1468, 2466, 2469},
    {1467, 1468, 2466, 2469},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {1467, 1468, 2466, 2469},
    {1467, 1468, 2466, 2469},
    {2141, 2142, 2467, 2470},
    {2153, 2154, 2468, 2471},
    {2863, 2862, 2858, 2859},
    {2856, 2857, 2861, 2860},
    {1469, 1470, 2472, 2475},
    {1469, 1470, 2472, 2475},
    {2143, 2144, 2473, 2476},
    {2155, 2156, 2474, 2477},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {1471, 1472, 2478, 2481},
    {1471, 1472, 2478, 2481},
    {2145, 2146, 2479, 2482},
    {2157, 2158, 2480, 2483},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {1473, 1474, 2147, 2148},
    {1473, 1474, 2147, 2148},
    {2147, 2148, 2159, 2160},
    {2159, 2160, 1475, 1476},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {1475, 1476, 2149, 2150},
    {1475, 1476, 2149, 2150},
    {2149, 2150, 2161, 2162},
    {2161, 2162, 1477, 1478},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {1477, 1478, 2854, 2855},
    {1477, 1478, 2854, 2855},
    {2151, 2152, 2163, 2164},
    {2163, 2164, 2848, 2849},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
    {2848, 2849, 0, 0},
    {2848, 2849, 0, 0},
    {2151, 2152, 2163, 2164},
    {2163, 2164, 2848, 2849},
    {2661, 2662, 2663, 2664},
    {2850, 2851, 2852, 2853},
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
    // Arithmetic shift, without native's undefined signed left shift in C++.
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

// @verified - p
bool LoadSoundBankChunk(const char *directory, const char *filename, std::uint32_t bank, std::uint64_t size) noexcept {
    if (cvsHasValidConverseData(nullptr))
        return false;
    char path[256];
    std::sprintf(path, "%s\\%s", directory, filename);
    for (char *cursor = path; *cursor; ++cursor)
        if (*cursor == '/')
            *cursor = '\\';
    d3t::LargeFileHandle *const file = d3t::LargeFile_OpenPathHandle(path);
    if (!file)
        return false;
    std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, SOUND_BANK_CHUNK_DATA));
    (void)d3t::GetFileFromCachedBuffer(file, size >> 11U, data, 0);
    std::int32_t state = d3t::CheckFileInfo(file);
    if (state != 3) {
        while (state != 4) {
            state = d3t::CheckFileInfo(file);
            if (state == 3)
                break;
        }
        if (state == 4)
            nullsub(static_cast<void *>(nullptr));
    }
    (void)LoadBankSoundTable({data, static_cast<std::size_t>(size)}, bank);
    if (static_cast<std::int32_t>(ReturnZero()) >= 0) {
        while (ReturnZero() != 0) {
        }
        if (ReturnZero() != 0) {
            do {
                (void)ReturnZero();
                (void)d3t::GetFileFromCachedBuffer(file, size >> 11U, data, 0);
                state = d3t::CheckFileInfo(file);
                if (state != 3) {
                    while (state != 4) {
                        state = d3t::CheckFileInfo(file);
                        if (state == 3)
                            break;
                    }
                    if (state == 4)
                        nullsub(static_cast<void *>(nullptr));
                }
            } while (ReturnZero() != 0);
        }
        while (ReturnZero() != 0) {
        }
        d3t::ReadCachedFile(file);
        HLib::FreeTaskData(data);
        return true;
    }
    HLib::FreeTaskData(data);
    d3t::ReadCachedFile(file);
    return false;
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
    std::uint64_t totalFree, largestBlock, remainingFree;
    (void)HLib::AnalyzeFreeHeap(&totalFree, &largestBlock, &remainingFree);
    std::array<char, 96> path{};
    const int originalWritten = std::snprintf(path.data(), path.size(), "%s/%s", directory, original);
    if (originalWritten <= 0 || static_cast<std::size_t>(originalWritten) >= path.size())
        return false;
    const std::uint32_t originalSize = static_cast<std::uint32_t>(ArchiveData_GetFileSize(path.data()));
    std::int32_t result = -1;
    if (originalSize > largestBlock) {
        result = LoadSoundBankChunk(directory, original, static_cast<std::uint32_t>(bank), largestBlock & 0xfffff800ULL) ? 0 : -1;
    } else {
        char selected[64];
        SelectSoundBankFilename(selected, directory, original, static_cast<std::uint32_t>(bank));
        const int written = std::snprintf(path.data(), path.size(), "%s/%s", directory, selected);
        if (written <= 0 || static_cast<std::size_t>(written) >= path.size())
            return false;
        const std::uint64_t size = ArchiveData_GetFileSize(path.data());
        if (!size || size > static_cast<std::uint64_t>(SIZE_MAX))
            return false;
        std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, SOUND_BANK_DATA));
        if (!data)
            return false;
        if (ArchiveData_Read(path.data(), 0, data, size)) {
            const std::span<const std::byte> payload = LoadBankSoundTable({data, static_cast<std::size_t>(size)}, static_cast<std::uint32_t>(bank));
            if (!payload.empty()) {
                std::size_t transferSize = payload.size();
                if (size > 0x20) {
                    const HLib::TaskData *const allocation = reinterpret_cast<const HLib::TaskData *>(data - sizeof(HLib::TaskData));
                    if (allocation->next) {
                        // Native bank upload can consume the adjacent arena header, not just the file's bytes.
                        transferSize = HLib::GetTaskDataSize(data) + sizeof(HLib::TaskData) - static_cast<std::size_t>(payload.data() - data);
                    }
                }
                result = static_cast<std::int32_t>(LoadAudioIntoAicaMemory({payload.data(), transferSize}, static_cast<std::uint32_t>(bank)));
            }
        }
        HLib::FreeTaskData(data);
    }
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

// Native SNDD program-name pointer (op 4) and scene override (op 5) published by SNDD_Reader.
const char *snddProgramName{};
std::int32_t snddSceneIndex{-1};

// Native SNDD token 23: the scene count byte of the first .sbt bank named by the current op-4 program's SMAP, or of
// AR02_000.sbt. The port looks up an absent program name as a missing SMAP rather than dereferencing null.
// @verified - p
std::int32_t snddProgramSceneCount() noexcept {
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
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace

MaterialSoundBank g_materialSoundBanks[8] = {
    {SHEN_FIELD(pairs) materialSounds, SHEN_FIELD(capacity) 1, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 1, SHEN_FIELD(capacity) 1, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 2, SHEN_FIELD(capacity) 1, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 3, SHEN_FIELD(capacity) 24, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 27, SHEN_FIELD(capacity) 48, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 75, SHEN_FIELD(capacity) 64, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 139, SHEN_FIELD(capacity) 110, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
    {SHEN_FIELD(pairs) materialSounds + 249, SHEN_FIELD(capacity) 120, SHEN_FIELD(bank) - 1, SHEN_FIELD(count) 0},
};

SampleProgramPool g_sampleProgramStorage[4] = {
    {SHEN_FIELD(records) sampleRecords, SHEN_FIELD(capacity) 5, SHEN_FIELD(bank) - 1},
    {SHEN_FIELD(records) sampleRecords + 5, SHEN_FIELD(capacity) 10, SHEN_FIELD(bank) - 1},
    {SHEN_FIELD(records) sampleRecords + 15, SHEN_FIELD(capacity) 10, SHEN_FIELD(bank) - 1},
    {SHEN_FIELD(records) sampleRecords + 25, SHEN_FIELD(capacity) 60, SHEN_FIELD(bank) - 1},
};

std::int32_t g_pendingSoundSceneIndex = -1;
std::int32_t g_soundMusicCommandEnabled = 1;
float GetWeatherPressure();
float GetWeatherWindVector();
std::int32_t g_soundDebugMode = 0;
std::int32_t g_soundDebugSelection = 0;
std::int32_t g_soundDebugSecondarySelection = 0;
std::uint8_t blurModelFlag = 0, g_savedFightBlurFlag = 0;
std::int32_t accumulatedFightBlur;

std::int32_t SoundProgramNullsub() {
    return 0;
}


// @verified - d
char *shenGetCurrentSbt() {
    return g_currentSoundSequenceName;
}


// @verified - d
std::int32_t GetCurrentSoundSceneIndex() {
    return g_currentSoundSceneIndex;
}


// @verified - d
const char *FindSoundMapSequenceName(const char *name) {
    const SoundMapRecord *const record = reinterpret_cast<const SoundMapRecord *>(FindSoundRecord(SOUND_MAP_RECORD, name));
    if (!record)
        return 0;
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        if (std::strstr(record->banks[bank].filename, ".sbt"))
            return record->banks[bank].filename;
    }
    return 0;
}


// @verified - d
void InitializeMaterialSoundEffects() {
    ExecuteAicaCommand(0x400a0, 0, 0);
    for (std::int32_t index = 0; index < 16; ++index)
        g_materialSoundHandles[index].active = 0;
}


// @verified - d
void RestoreMaterialSoundHandles(std::int32_t mode) {
    for (std::int32_t index = 16; index >= 0; --index) {
        if (g_materialSoundHandles[index].active) {
            if (mode) {
                std::int32_t active;
                do {
                    active = g_materialSoundHandles[index].active = 1;
                } while (0);
                if (mode > active) {
                    ExecuteAicaCommand(0xa5, index, active);
                    ExecuteAicaCommand(0x10a5, index, g_materialSoundHandles[index].volume);
                    ExecuteAicaCommand(0x20a5, index, g_materialSoundHandles[index].control);
                }
                ExecuteAicaCommand(0xa6, index, g_materialSoundHandles[index].gain);
            } else {
                ExecuteAicaCommand(0xa6, index, 0);
                g_materialSoundHandles[index].active = 2;
            }
        }
    }
}


// @verified - d
std::int32_t CompareSoundProgramKeys(const char *first, const char *second) {
    std::uint32_t firstKey[2], secondKey[2];
    CopySoundProgramKey(first, reinterpret_cast<char *>(firstKey));
    CopySoundProgramKey(second, reinterpret_cast<char *>(secondKey));
    if (firstKey[0] == secondKey[0] && firstKey[1] == secondKey[1])
        return 0;
    return 1;
}


// @verified - d
SampleProgramRecord *GetLoudestSampleProgramRecord(const SampleProgramState *program) {
    if (!program || !program->executing)
        return 0;
    SampleProgramRecord *selected = program->records;
    SampleProgramRecord *record = selected + 1;
    for (std::int32_t index = 1; index < program->count; ++index, ++record) {
        if (static_cast<std::int8_t>(selected->volume) < static_cast<std::int8_t>(record->volume))
            selected = record;
    }
    return selected;
}


// @verified - d
bool LoadSoundProgramAnyScene(const char *name, std::uint32_t sceneIndex) {
    return LoadSoundProgram(name, sceneIndex, -1);
}


// @verified - d
std::int32_t GetSoundSequenceSceneCount(const char *name) {
    if (!name) {
        name = shenGetCurrentSbt();
        if (!name)
            return 0;
    }
    const SoundSequenceHeader *const sequence = FindSbt(name);
    if (!sequence)
        return 0;
    return sequence->count;
}


// @verified - d
void SNDD_Reader(const MapInfoChunk *chunk, std::int32_t, std::int32_t entry) {
    std::int32_t comparison = 0;
    std::int32_t mode = 0;
    if (!chunk)
        return;
    const std::int32_t *cursor = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(chunk) + sizeof(MapInfoChunk));
    DateTime date;
    shenGetCurrentDateTime(date);
    std::int32_t values[2];
    std::int32_t operation;
    goto next;
    do {
        if (operation < 0) {
            if (operation == -1) {
                std::int32_t count = 0;
                for (std::int32_t token = *cursor++; token >= 0; token = *cursor++) {
                    if (token == 16)
                        values[count++] = *cursor++;
                    else if (token <= 5)
                        comparison = token;
                    else if (token <= 15)
                        mode = token;
                    else if (token == 17)
                        values[count++] = GetInternalFlag(*cursor++);
                    else if (token == 18)
                        values[count++] = entry;
                    else if (token == 19)
                        values[count++] = date.month;
                    else if (token == 20)
                        values[count++] = date.day;
                    else if (token == 21)
                        values[count++] = date.hour;
                    else if (token == 22)
                        values[count++] = date.minute;
                    else if (token == 23)
                        values[count++] = GetSoundSequenceSceneCount(FindSoundMapSequenceName(g_soundDefinitionProgramName));
                    else if (token == 24)
                        values[count++] = g_storyStep;
                    else if (token == 25)
                        values[count++] = EV_GetShenmueFlag(*cursor++);
                }
                bool hit = false;
                if (count < 2) {
                    values[1] = 0;
                    comparison = 5;
                }
                switch (comparison) {
                case 0:
                    if (values[0] == values[1])
                        hit = true;
                    break;
                case 1:
                    if (values[0] <= values[1])
                        hit = true;
                    break;
                case 2:
                    if (values[0] >= values[1])
                        hit = true;
                    break;
                case 3:
                    if (values[0] < values[1])
                        hit = true;
                    break;
                case 4:
                    if (values[0] > values[1])
                        hit = true;
                    break;
                case 5:
                    if (values[0] != values[1])
                        hit = true;
                    break;
                }
                if (mode == 6)
                    hit = !hit;
                if (hit)
                    ++cursor;
                else
                    cursor = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(cursor) + *cursor);
            } else if (operation == -2) {
                cursor = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(cursor) + *cursor);
            }
        } else if (operation == 4) {
            const char *const name = reinterpret_cast<const char *>(cursor);
            cursor += 4;
            g_soundDefinitionProgramName = name;
        } else if (operation == 5) {
            operation = *cursor++;
            g_soundDefinitionSceneIndex = operation;
        }
    next:
        operation = *cursor++;
    } while (operation != 0);
}


// @verified - d
bool SNDP_SoundProgramReader(const MapInfoChunk *chunk, std::int32_t act, shen_name area) {
    std::uint32_t key[3];
    g_currentSoundMap = 0;
    if (!chunk)
        return false;
    ++chunk;
    g_pendingSoundSceneIndex = -1;
    key[0] = 0;
    const MapInfoChunk *soundMap = FindNativeChunk("SNDM", const_cast<MapInfoChunk *>(chunk));
    if (!soundMap)
        return false;
    ++soundMap;
    g_currentSoundMap = reinterpret_cast<const SoundDataHeader *>(soundMap);
    if (const MapInfoChunk *const assignment = FindNativeChunk("SNDA", const_cast<MapInfoChunk *>(chunk))) {
        const char *const name = reinterpret_cast<const char *>(assignment + 1);
        if (std::strcmp(name, "default"))
            CopySoundProgramKey(name, reinterpret_cast<char *>(key));
    }
    if (const MapInfoChunk *const scene = FindNativeChunk("SNDR", const_cast<MapInfoChunk *>(chunk)))
        g_pendingSoundSceneIndex = *reinterpret_cast<const std::int32_t *>(scene + 1);
    if (const MapInfoChunk *const definition = FindNativeChunk("SNDD", const_cast<MapInfoChunk *>(chunk))) {
        g_soundDefinitionProgramName = 0;
        g_soundDefinitionSceneIndex = -1;
        SNDD_Reader(definition, act, g_entryPoint);
        if (g_soundDefinitionProgramName)
            CopySoundProgramKey(g_soundDefinitionProgramName, reinterpret_cast<char *>(key));
        if (g_soundDefinitionSceneIndex >= 0)
            g_pendingSoundSceneIndex = g_soundDefinitionSceneIndex;
    }
    if (!key[0]) {
        key[0] = area;
        key[1] = 0;
    }
    const SoundDataHeader *const record = FindSoundRecordInTable(g_currentSoundMap, SOUND_MAP_RECORD, key[0], key[1]);
    if (record) {
        key[2] = 0;
        std::strcpy(g_activeSoundMapName, reinterpret_cast<const char *>(key));
        ConfigureSoundMap(reinterpret_cast<const SoundMapRecord *>(record), act);
        UpdateSelectedSoundProgram(false);
        ++g_selectedSoundProgramFrame;
        return true;
    }
    return false;
}


// @verified - d
std::int32_t Loop3DSoundEffect(std::uint32_t flags) {
    std::int32_t environment = static_cast<std::int32_t>((flags & 0x780000U) >> 19U);
    std::uint32_t key = flags & 0x87ffffU;
    if (key & 0x40000U) {
        key = GetSoundMaterialKey(key);
        if (!environment)
            environment = GetSoundEnvironment();
    }
    SoundDriverCommand record;
    if (FindMaterialSoundCommand(key, &record.material)) {
        if (environment > 0)
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, environment));
        DispatchSoundCommand(SoundCommandMode_Raw, &record);
        if (environment > 0)
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, 0));
        return 1;
    }
    if (static_cast<std::uint32_t>((flags & 0xffU) - 0xa8U) > 7U)
        return 0;
    ExecuteAicaCommand(flags, 0, 0);
    return 1;
}


// @verified - d
std::int32_t GetSoundBankFadeVolume(std::int32_t bank) {
    if (g_soundBankFades[bank].step != 0.0f) {
        const std::int32_t volume = static_cast<std::int32_t>(g_soundBankFades[bank].volume);
        if (volume <= 127)
            return volume < 0 ? 0 : volume;
    }
    return -1;
}


// @verified - d
bool FindMaterialSoundCommand(std::uint32_t material, MaterialSoundCommand *output) {
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        if (g_materialSoundBanks[bank].bank < 0)
            continue;
        const std::int32_t count = static_cast<std::int32_t>(g_materialSoundBanks[bank].count);
        const MaterialSoundPair *pair = g_materialSoundBanks[bank].pairs;
        for (std::int32_t index = 0; index < count; ++index, ++pair) {
            if (pair->material != material)
                continue;
            const std::int32_t rawCommand = static_cast<std::int32_t>(pair->command);
            const std::uint32_t command = rawCommand & 0x00ffffff;
            output->command = command;
            const std::uint32_t variants = static_cast<std::uint8_t>(rawCommand >> 24) + 1U;
            output->variantCount = variants;
            if (variants > 128U)
                output->extendedCommand = command;
            output->bank = g_materialSoundBanks[bank].bank;
            return true;
        }
    }
    return false;
}


// @verified - d
std::uint32_t FindMaterialSoundBank(std::uint32_t material) {
    SoundDriverCommand command;
    return FindMaterialSoundCommand(material, &command.material) ? static_cast<std::uint32_t>(command.material.bank) : 0xffffffffU;
}


// @verified - d
std::int32_t ExecuteAicaBankCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) {
    SoundDriverCommand record;
    record.material.extendedCommand = command;
    record.material.bank = static_cast<std::int32_t>(bank);
    record.value = value;
    return DispatchSoundCommand(SoundCommandMode_Bank, &record);
}


// @verified - d
void EV_spmBeginSampleExecute(std::int32_t) {
    const SoundMapExtendedRecord *const extended = reinterpret_cast<const SoundMapExtendedRecord *>(FindSoundRecord(SOUND_MAP_RECORD, GetCurrentBgm()));
    const std::uint8_t *durations;
    if (extended && extended->name == SOUND_MAP_EXTENSION)
        durations = extended->fadeIn;
    else
        durations = kDefaultSoundBankFadeIn;
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        if (durations[bank] && GetSoundBankPlaybackState(bank)) {
            g_soundBankFades[bank].volume = 0.0f;
            g_soundBankFades[bank].step = (127.0f - g_soundBankFades[bank].volume) / static_cast<float>(durations[bank]);
        } else {
            g_soundBankFades[bank].step = 0.0f;
        }
    }
}


// @verified - d
void spmFadeOut(std::int32_t) {
    const SoundMapExtendedRecord *const extended = reinterpret_cast<const SoundMapExtendedRecord *>(FindSoundRecord(SOUND_MAP_RECORD, GetCurrentBgm()));
    const std::uint8_t *durations;
    if (extended && extended->name == SOUND_MAP_EXTENSION)
        durations = extended->fadeOut;
    else
        durations = kDefaultSoundBankFadeOut;
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        if (durations[bank] && GetSoundBankPlaybackState(bank)) {
            g_soundBankFades[bank].volume = static_cast<float>(SOND_QueryBankLevel(static_cast<std::uint32_t>(bank), 0x4a0U));
            g_soundBankFades[bank].step = -(g_soundBankFades[bank].volume / static_cast<float>(durations[bank]));
            if (bank == g_soundMusic.bank) {
                g_soundMusic.activeProgramCommand = -1;
            } else if (bank == g_samplePrograms[0].bank) {
                g_samplePrograms[0].executing = 0;
            } else if (bank == g_samplePrograms[1].bank) {
                g_samplePrograms[1].executing = 0;
            } else if (bank == g_secondarySamplePrograms[0].bank) {
                g_secondarySamplePrograms[0].executing = 0;
            } else if (bank == g_secondarySamplePrograms[1].bank) {
                g_secondarySamplePrograms[1].executing = 0;
            }
        } else {
            g_soundBankFades[bank].step = 0.0f;
        }
    }
    if (ConversationBuffer *const voice = FindConversationByPriority(1))
        scenCvsCtrlFree(voice, 60);
}


// @verified - d
std::int32_t GetSystemAudioOutputMode() {
    void *const configuration = HLib::AllocateTaskData(0x4000, SOUND_SYSTEM_CONFIG_STORAGE);
    syCfgInit(configuration);
    std::int32_t mode;
    syCfgGetSoundMode(&mode);
    syCfgExit();
    HLib::FreeTaskData(configuration);
    return mode;
}


// @verified - d
void SetSystemAudioOutputMode(std::int32_t mode) {
    void *const configuration = HLib::AllocateTaskData(0x4000, SOUND_SYSTEM_CONFIG_STORAGE);
    syCfgInit(configuration);
    syCfgSetSoundMode(mode);
    syCfgExit();
    HLib::FreeTaskData(configuration);
    std::uint32_t command, volume;
    if (mode == 1) {
        volume = 14;
        command = 0x900a0;
    } else {
        command = 0x800a0;
        volume = 15;
    }
    ExecuteAicaCommand(command, 0, 0);
    SOND_ExecuteRawAicaCommand(0x1ba0 + (volume << 16));
}


// @verified - d
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) {
    if ((command & 0xf0U) == 0xb0U) {
        const std::uint32_t channel = command & 15U;
        nlsndExecuteAicaCommand2Ch(bank, channel ? channel : 16, command & 0xfffff0U, value & 127);
    } else if ((command & 0xffU) >= 0xa8U) {
        SOND_ExecuteRawAicaCommand(command);
    } else if ((command & 0xffU) < 0xa5U) {
        nlsndExecuteAicaCommand(bank, command, value & 127);
    } else if (bank <= 16) {
        nlsndExecuteEnvironmentAicaCommand(bank, command, value);
    }
}


// @verified - d
void LoadSoundBankData(const char *directory, const char *filename, std::uint32_t bank) {
    const std::int8_t previousState = g_soundBankLoadStates[bank];
    g_soundBankLoadStates[bank] = 0;
    if (!IsSoundBankSlotNamed(filename, bank) || previousState == -2) {
        ExecuteAicaCommand(0x1100a0, bank, 0);
        std::size_t totalFree;
        std::size_t largestBlock;
        std::size_t otherFree;
        HLib::AnalyzeFreeHeap(&totalFree, &largestBlock, &otherFree);
        const std::uint32_t bytes = shenOpenFile(directory, filename);
        if (bytes <= largestBlock) {
            if (!LoadWholeSoundBank(directory, filename, static_cast<std::int32_t>(bank)))
                g_soundBankLoadStates[bank] = -2;
        } else if (!LoadStreamedSoundBank(directory, filename, static_cast<std::int32_t>(bank), largestBlock & 0xfffff800U)) {
            g_soundBankLoadStates[bank] = -2;
        }
        std::strcpy(g_soundBankNames[bank], filename);
    }
}


// @verified - d
bool LoadWholeSoundBank(const char *directory, const char *filename, std::int32_t bank) {
    void *const data = shenLoadFile(directory, filename, SOUND_FILE_STORAGE);
    const std::uint32_t *const bankData = LoadSoundBankMetadata(reinterpret_cast<const std::uint32_t *>(data), bank);
    const std::int32_t result = SOND_ReplaceBank(bankData, bank);
    HLib::FreeTaskData(data);
    if (result < 0)
        return false;
    return true;
}


// @verified - d
bool LoadStreamedSoundBank(const char *directory, const char *filename, std::int32_t bank, std::uint32_t bufferBytes) {
    if (cvsHasValidConverseData(0))
        return false;
    const std::uint32_t sectorBytes = static_cast<std::int32_t>(bufferBytes) < 0 ? bufferBytes + 0x7ffU : bufferBytes;
    const std::int32_t sectors = static_cast<std::int32_t>(sectorBytes) >> 11;
    const std::uint32_t words = bufferBytes >> 2;
    ADXF const file = ADXF_Open(const_cast<char *>(filename), GetCachedDiscDirectory(directory));
    if (!file)
        return false;
    std::uint32_t *const buffer = static_cast<std::uint32_t *>(HLib::AllocateTaskData(bufferBytes, SOUND_STREAM_STORAGE));
    ADXF_ReadNw32(file, sectors, buffer);
    while (ADXF_GetStat(file) != 3) {
    }
    const std::uint32_t *data = LoadSoundBankMetadata(buffer, bank);
    const std::uint32_t metadataBytes = reinterpret_cast<const std::byte *>(data) - reinterpret_cast<const std::byte *>(buffer);
    if (SOND_BeginStreamReplacement(data, bank, (bufferBytes - metadataBytes) >> 2) < 0) {
        HLib::FreeTaskData(buffer);
        ADXF_Close(file);
        return false;
    }
    while (SOND_PollBankUpload() != 0) {
    }
    while (SOND_WriteStreamChunk(data) != 0) {
        SOND_SetUploadWords(words);
        ADXF_ReadNw32(file, sectors, buffer);
        while (ADXF_GetStat(file) != 3) {
        }
        data = buffer;
    }
    while (SOND_PollBankUpload() != 0) {
    }
    ADXF_Close(file);
    HLib::FreeTaskData(buffer);
    return true;
}


// @verified - d
static const std::byte *CopyMaterialSoundRecords(const SoundBankMetadataHeader *header, std::int32_t bank) {
    std::int32_t bytes = header->recordSize * header->count;
    const std::byte *records = reinterpret_cast<const std::byte *>(header) + header->dataOffset;
    for (std::int32_t index = 0; index < 8; ++index) {
        if (g_materialSoundBanks[index].bank < 0 && header->count <= static_cast<std::int32_t>(g_materialSoundBanks[index].capacity)) {
            std::memcpy(g_materialSoundBanks[index].pairs, records, bytes);
            g_materialSoundBanks[index].bank = bank;
            g_materialSoundBanks[index].count = header->count;
            break;
        }
    }
    return records + bytes;
}


// @verified - d
void ReleaseMaterialSoundBank(std::int32_t bank) {
    for (std::int32_t index = 0; index < 8; ++index) {
        if (g_materialSoundBanks[index].bank == bank) {
            g_materialSoundBanks[index].bank = -1;
            break;
        }
    }
}


// @verified - d
static const std::byte *CopySampleProgramRecords(const std::uint32_t *data, std::int32_t bank, SampleProgramState *program) {
    const SoundBankMetadataHeader *const header = reinterpret_cast<const SoundBankMetadataHeader *>(data);
    std::int32_t bytes = header->recordSize * header->count;
    const std::byte *records = reinterpret_cast<const std::byte *>(header) + header->dataOffset;
    std::int32_t index = 0;
    for (; index < 2; ++index, ++program) {
        if (!program->records)
            break;
    }
    if (index < 2) {
        program->executing = 0;
        for (index = 0; index < 4; ++index) {
            if (g_sampleProgramStorage[index].bank < 0 && header->count <= static_cast<std::int32_t>(g_sampleProgramStorage[index].capacity)) {
                std::memcpy(g_sampleProgramStorage[index].records, records, bytes);
                program->command = header->command;
                program->count = header->count;
                program->bank = bank;
                program->records = g_sampleProgramStorage[index].records;
                g_sampleProgramStorage[index].bank = bank;
                break;
            }
        }
    }
    return records + bytes;
}


// @verified - d
void ReleaseSampleProgramBank(std::int32_t bank) {
    SampleProgramState *selected = 0;
    std::int32_t index;
    for (index = 0; index < 2; ++index) {
        if (g_samplePrograms[index].bank == bank) {
            selected = g_samplePrograms + index;
            break;
        }
        if (g_secondarySamplePrograms[index].bank == bank) {
            selected = g_secondarySamplePrograms + index;
            break;
        }
    }
    if (selected) {
        selected->records = 0;
        selected->executing = 0;
    }
    for (index = 0; index < 4; ++index) {
        if (g_sampleProgramStorage[index].bank == bank)
            g_sampleProgramStorage[index].bank = -1;
    }
}


// @verified - d
static std::int32_t IsSoundBankSlotNamed(const char *name, std::uint32_t bank) {
    if (CompareCharacterTableTokens(name, g_soundBankNames[bank]))
        return 0;
    return 1;
}


// @verified - d
const std::uint32_t *LoadSoundBankMetadata(const std::uint32_t *data, std::int32_t bank) {
    ReleaseMaterialSoundBank(bank);
    ReleaseSampleProgramBank(bank);
    const SoundBankMetadataHeader *const header = reinterpret_cast<const SoundBankMetadataHeader *>(data);
    const std::uint32_t name = header->name;
    if (name == SOUND_MATERIAL_TABLE) {
        data = reinterpret_cast<const std::uint32_t *>(CopyMaterialSoundRecords(header, bank));
    } else {
        SampleProgramState *program;
        if (name == SOUND_AMBIENT_TABLE)
            program = g_samplePrograms;
        else if (name == SOUND_SECONDARY_SAMPLE_TABLE)
            program = g_secondarySamplePrograms;
        else
            return data;
        data = reinterpret_cast<const std::uint32_t *>(CopySampleProgramRecords(data, bank, program));
    }
    return data;
}


// @verified - d
void InitAica() {
    std::int32_t index;
    LoadAicaDriver();
    for (index = 7; index >= 0; --index)
        g_soundBankNames[index][0] = 0;
    g_soundPauseDepth = 0;
    g_soundListenerUsesCamera = 0;
    g_soundMusic.activeProgramCommand = -1;
    g_pendingSoundSceneIndex = -1;
    ResetSampleProgram(g_samplePrograms);
    ResetSampleProgram(g_samplePrograms + 1);
    ResetSampleProgram(g_secondarySamplePrograms);
    ResetSampleProgram(g_secondarySamplePrograms + 1);
    for (index = 0; index < 8; ++index)
        g_materialSoundBanks[index].bank = -1;
    for (index = 0; index < 8; ++index)
        SetSoundBankAllocation(index, kInitialAicaBankAllocations[index]);
    sndClearSceneState();
    LoadSoundBankData("MISC", "system1.snd", 0);
    ExecuteAicaCommand(0xa8, 0, 0);
    SetSystemAudioOutputMode(GetSystemAudioOutputMode());
}


// @verified - d
void FinishAica() {
    SOND_FinishAicaDriver();
}


// @verified - d
void SetSoundBankAllocation(std::int32_t bank, std::uint32_t bytes) {
    if (bytes) {
        SOND_UploadBank(kEmptyAicaBank, bank, bytes);
        std::memcpy(g_soundBankNames[bank], ".", 2);
    }
    g_soundBankAllocations[bank] = bytes;
}


// @verified - d
std::int32_t LoadAicaDriver() {
    void *const driver = shenLoadFile("MISC", "AICADRV.BIN", AICA_DRIVER_STORAGE);
    const std::int16_t result = static_cast<std::int16_t>(SOND_InitializeAicaDriver(reinterpret_cast<const std::uint32_t *>(driver), 0));
    HLib::FreeTaskData(driver);
    return result;
}


// @verified - d
void ResetSampleProgram(SampleProgramState *program) {
    program->executing = 0;
    program->records = 0;
}


// @verified - d
void sndClearSceneState() {
    g_savedSoundMapName[0] = 0;
    g_savedSoundSequenceName[0] = 0;
    g_savedSoundSceneIndex = 0;
}


// @verified - d
std::int32_t DispatchSoundCommand(SoundCommandMode mode, const SoundDriverCommand *record) {
    std::int32_t result;
    switch (mode) {
    case SoundCommandMode_Raw:
        result = SOND_ExecuteRawAicaCommand(record->material.command);
        break;
    case SoundCommandMode_Channel:
        result = nlsndExecuteAicaCommand2Ch(record->material.bank, record->material.variantCount, record->material.extendedCommand, record->value);
        break;
    case SoundCommandMode_Bank:
        result = nlsndExecuteAicaCommand(static_cast<std::uint16_t>(record->material.bank), record->material.extendedCommand, record->value);
        break;
    case SoundCommandMode_Environment:
        result = nlsndExecuteEnvironmentAicaCommand(record->material.bank, record->material.extendedCommand, record->value);
        break;
    }
    return result;
}


// @verified - d
SoundDriverCommand *SetSoundDriverCommand(SoundDriverCommand *record, std::uint32_t command, std::int32_t value) {
    record->material.extendedCommand = command;
    record->value = value;
    return record;
}


// @verified - d
void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) {
    SoundDriverCommand record;
    record.material.extendedCommand = command;
    record.material.bank = static_cast<std::int32_t>(bank);
    record.value = value;
    if ((command & 0xf0U) == 0xb0U) {
        std::uint32_t channel = command & 0xfU;
        if (!channel)
            channel = 16;
        record.material.extendedCommand = command & 0xfffff0U;
        record.value = value & 0x7f;
        record.material.variantCount = channel;
        DispatchSoundCommand(SoundCommandMode_Channel, &record);
    } else if ((command & 0xffU) <= 0xa7U) {
        if ((command & 0xffU) <= 0xa4U) {
            record.value = value & 0x7f;
            DispatchSoundCommand(SoundCommandMode_Bank, &record);
        } else if (bank <= 16U) {
            DispatchSoundCommand(SoundCommandMode_Environment, &record);
        }
    } else {
        record.material.command = command;
        DispatchSoundCommand(SoundCommandMode_Raw, &record);
    }
}


// @verified - d
std::int32_t DispatchChannelSoundCommand(std::int32_t bank, std::uint32_t channel, std::uint32_t command, std::int32_t value) {
    SoundDriverCommand record;
    record.material.extendedCommand = command;
    record.material.bank = bank;
    record.material.variantCount = channel;
    record.value = value;
    return DispatchSoundCommand(SoundCommandMode_Channel, &record);
}


// @verified - d
std::uint32_t GetSoundMaterialKey(std::uint32_t flags) {
    const std::uint32_t group = (flags & 0x70U) >> 4U;
    const std::uint32_t variant = (flags >> 8U) & 3U;
    const std::uint32_t key = flags & 7U;
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(soundEnvironmentKeyRows[group][key][variant])));
}


void SoundProgramObjectNullsub() {
}


// @verified - d
std::int32_t GetSoundEnvironment() {
    Vector3 position;
    camCtrlGetNextCamPos(&position);
    std::int32_t environment = static_cast<std::int32_t>(GetFieldSoundProperties(&position) >> 16U) - 4;
    if (environment < 0)
        environment = 0;
    return environment;
}


// @verified - d
std::int32_t GetSoundBankPlaybackState(std::int32_t bank) {
    return SOND_QueryBankPlaybackState(bank);
}


std::int32_t ConsumeSoundDriverEvent() {
    return SOND_ConsumeDriverEvent();
}


// @verified - d
void spmFadeInBank(std::int32_t bank, std::int32_t frames) {
    g_soundBankFades[bank].volume = 0.0f;
    g_soundBankFades[bank].step = (127.0f - g_soundBankFades[bank].volume) / static_cast<float>(frames);
}


// @verified - d
void spmFadeOutBank(std::int32_t bank, std::int32_t frames) {
    const std::int32_t current = SOND_QueryBankLevel(static_cast<std::uint32_t>(bank), 0x4a0U);
    g_soundBankFades[bank].volume = static_cast<float>(current);
    g_soundBankFades[bank].step = -(g_soundBankFades[bank].volume / static_cast<float>(frames));
}


// @verified - d
SoundEffectHandle *AllocateMaterialSoundHandle(std::int32_t key) {
    SoundDriverCommand command;
    SoundEffectHandle *handle = g_materialSoundHandles;
    std::int32_t slot;
    for (slot = 0; slot < 16 && handle->active; ++slot, ++handle)
        ;
    if (slot > 15)
        return 0;
    if (!FindMaterialSoundCommand(static_cast<std::uint32_t>(key), &command.material))
        return 0;
    handle->active = 1;
    handle->command = command.material.extendedCommand;
    handle->slot = slot;
    handle->gain = 0;
    handle->volume = 0;
    handle->control = 0;
    ExecuteAicaCommand(0xa5U, slot, command.material.extendedCommand);
    return handle;
}


// @verified - d
std::int32_t ExecuteMaterialSoundHandleCommand(SoundEffectHandle *handle, std::uint32_t command, std::int32_t value) {
    std::int32_t *cached = 0;
    switch (command) {
    case 0xa6U:
        cached = &handle->gain;
        break;
    case 0x10a5U:
        cached = &handle->volume;
        break;
    case 0x20a5U:
        cached = &handle->control;
        break;
    }
    if (cached) {
        if (*cached == value)
            return 1;
        *cached = value;
    }
    if (handle->active == 1)
        ExecuteAicaCommand(command, handle->slot, value);
    return 1;
}


// @verified - d
std::int32_t FreeMaterialSoundHandle(SoundEffectHandle *handle) {
    ExecuteAicaCommand(0xa6U, handle->slot, 0);
    handle->active = 0;
    return 1;
}


// @verified - d
void SetSoundListenerUsesCamera(std::int32_t enabled) {
    g_soundListenerUsesCamera = enabled;
}


// @verified - d
std::int32_t Publish3DSoundCommand(
    SoundDriverCommand *record, const Vector3 *position, float minimumDistance, float fadeDistance, std::int32_t volume, std::uint32_t persistent) {
    const std::int32_t yaw = camCtrlGetNextRotY();
    Vector3 listener;
    if (g_soundListenerUsesCamera) {
        camCtrlGetNextCamPos(&listener);
    } else {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterWorldPosition(control->ryo, &listener);
    }
    float gain = SoundDistanceGain(&listener, position, minimumDistance, fadeDistance);
    if (!volume)
        volume = 127;
    if (volume < 127)
        gain *= static_cast<float>(volume) / 127.0f;
    const std::int32_t bankVolume = GetSoundBankFadeVolume(record->material.bank);
    if (bankVolume >= 0)
        gain *= static_cast<float>(bankVolume) / 127.0f;
    volume = static_cast<std::int32_t>(gain);
    if (!volume)
        return 0;
    if (!g_soundListenerUsesCamera)
        camCtrlGetNextCamPos(&listener);
    std::int32_t pan;
    SoundPan(&pan, yaw, position, &listener);
    if (!persistent) {
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x27b0U, volume));
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x2ab0U, pan));
        DispatchSoundCommand(SoundCommandMode_Raw, record);
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x2ab0U, 0));
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x27b0U, 127));
    } else {
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x07b0U, volume));
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(record, 0x0ab0U, pan));
        DispatchSoundCommand(SoundCommandMode_Raw, record);
    }
    return 1;
}


// @verified - d
std::int32_t PlayFootstepForNode(std::uint32_t flags, Character *character) {
    const std::uint32_t volume = flags >> 24U;
    std::int32_t environment = static_cast<std::int32_t>((flags & 0x780000U) >> 19U);
    const std::uint32_t persistent = flags & 0x20000U;
    flags &= 0x85ffffU;
    if (flags > 0x7fffffU) {
        const std::int32_t token = ResolveFootstepGroundToken(character);
        if (token < 0)
            return 0;
        environment = GetSoundEnvironmentFromProperties(static_cast<std::uint32_t>(token));
        flags = ResolveFootstepMaterialKey(character, static_cast<std::uint32_t>(token), flags);
    }
    if (flags & 0x40000U) {
        flags = GetSoundMaterialKey(flags);
        if (!environment)
            environment = GetSoundEnvironment();
    }
    SoundDriverCommand record;
    if (FindMaterialSoundCommand(flags, &record.material)) {
        if (environment > 0)
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, environment));
        if (character) {
            Vector3 position;
            GetCharacterWorldPosition(character, &position);
            Publish3DSoundCommand(&record, &position, 2.0f, 30.0f, static_cast<std::int32_t>(volume), persistent);
        } else if (volume == 127U) {
            DispatchSoundCommand(SoundCommandMode_Raw, &record);
        } else {
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x27b0U, static_cast<std::int32_t>(volume)));
            DispatchSoundCommand(SoundCommandMode_Raw, &record);
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x27b0U, 127));
        }
        if (environment > 0)
            DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, 0));
        return 1;
    }
    return 0;
}


// @verified - d
std::int32_t Execute3DSoundCommand(std::uint32_t flags, const Vector3 *position, float minimumDistance, float fadeDistance) {
    std::uint32_t priority = flags >> 24U;
    std::int32_t environment = static_cast<std::int32_t>((flags & 0x780000U) >> 19U);
    std::uint32_t loop = flags & 0x20000U;
    flags &= 0x85ffffU;
    if (flags & 0x40000U) {
        flags = GetSoundMaterialKey(flags);
        if (!environment)
            environment = GetSoundEnvironment();
    }
    SoundDriverCommand record;
    if (!FindMaterialSoundCommand(flags, &record.material))
        return 0;
    if (environment > 0)
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, environment));
    Publish3DSoundCommand(&record, position, minimumDistance, fadeDistance, priority, loop);
    if (environment > 0)
        DispatchSoundCommand(SoundCommandMode_Channel, SetSoundDriverCommand(&record, 0x2db0U, 0));
    return 1;
}


// @verified - d
std::int32_t UpdateMaterialSoundEffectPosition(
    SoundEffectHandle *handle, const Vector3 *position, float minimumDistance, float fadeDistance, std::int32_t controlValue) {
    const std::int32_t yaw = camCtrlGetNextRotY();
    Vector3 listener;
    if (g_soundListenerUsesCamera) {
        camCtrlGetNextCamPos(&listener);
    } else {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterWorldPosition(control->ryo, &listener);
    }
    const std::int32_t volume = static_cast<std::int32_t>(SoundDistanceGain(&listener, position, minimumDistance, fadeDistance));
    if (!g_soundListenerUsesCamera)
        camCtrlGetNextCamPos(&listener);
    std::int32_t pan;
    SoundPan(&pan, yaw, position, &listener);
    ExecuteMaterialSoundHandleCommand(handle, 0x10a5U, volume);
    if (volume) {
        ExecuteMaterialSoundHandleCommand(handle, 0x20a5U, pan);
        ExecuteMaterialSoundHandleCommand(handle, 0xa6U, controlValue);
    }
    return 1;
}


// @verified - d
char *GetCurrentBgm() {
    return g_activeSoundMapName;
}


// @verified - d
void ResetSelectedSoundProgram() {
    g_selectedSoundProgramFrame = 0;
    g_selectedSoundProgram = 0;
}


// @verified - d
void InitializeAmbientSoundProgram() {
    ResetSelectedSoundProgram();
}


// @verified - d
bool UpdateVolumeFades() {
    bool changed = false;
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        if (g_soundBankFades[bank].step != 0.0f) {
            std::int32_t volume = static_cast<std::int32_t>(g_soundBankFades[bank].volume);
            if (g_soundBankFades[bank].step > 0.0f) {
                if (volume > 126) {
                    volume = 127;
                    g_soundBankFades[bank].step = 0.0f;
                }
            } else if (volume <= 0) {
                volume = 0;
                g_soundBankFades[bank].step = 0.0f;
            }
            g_soundBankFades[bank].volume += g_soundBankFades[bank].step;
            ExecuteAicaCommand(0x4a0, bank, volume);
            changed = true;
        }
    }
    return changed;
}


// @verified - d
void SoundDebugDisplayNullsub(SampleProgramState *, const char *, std::int32_t *) {
}


// @verified - d
void AdvanceSelectedSoundProgram(bool fadeCommandIssued) {
    UpdateSelectedSoundProgram(fadeCommandIssued);
    ++g_selectedSoundProgramFrame;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

// Native SNDD_Reader: interprets the conditional SNDD stream. Comparison and negation tokens and both value slots persist
// across conditions within one call; a condition with fewer than two values compares the first against zero with "not
// equal", which also becomes the persisting comparison. Values past the second are dropped.
// @verified - p
void SNDD_Reader(const MapInfoChunk *sndd, std::int32_t, std::int32_t entryId) noexcept {
    if (!sndd)
        return;
    const std::byte *cursor = reinterpret_cast<const std::byte *>(sndd) + sizeof(MapInfoChunk);
    std::int32_t comparison = 0;
    std::int32_t mode = 0;
    std::array<std::int32_t, 2> values{};
    DateTime now{};
    shenGetCurrentDateTime(now);
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
                            value = snddProgramSceneCount();
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
#endif

namespace {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
    shenGetCurrentDateTime(date);
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
                Character *const character = ctrlGetScenarioChara(shen_name{record.characterName});
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
#endif

} // namespace

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

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

void ZeroAicaMemoryArena() noexcept {
    for (void *&arena : g_aicaMemoryArena)
        if (!arena)
            arena = std::malloc(AICA_MEMORY_ARENA_SIZE);
}

void aicaFree() noexcept {
    for (void *&arena : g_aicaMemoryArena) {
        if (arena) {
            std::free(arena);
            arena = nullptr;
        }
    }
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

// Native Shenmue::aicaSetMainCommand: queues one sound-driver command word (the high bit of its low byte set).
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

// @verified - p
void QueueSoundProgramControl() noexcept {
    soundTask = HLib::EnqueueTaskWithoutParameter(SoundProgramControl_Callback, 0x0e, 3, static_cast<std::int32_t>(soundTaskToken.name));
    runtime.queued = soundTask != nullptr;
    (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
    (void)Dispatch(AicaBackendOperation::BankCommand, 0x400a0U);
    for (SoundEffectHandle &handle : materialSoundHandles)
        handle.active = 0;
    runtime.frame = 0;
    runtime.selectedRecord = nullptr;
    for (SoundBankFade &fade : runtime.bankFades)
        fade.step = 0.0f;
    soundListenerUsesCamera = 0;
    while (runtime.pauseDepth > 0)
        PrimeAicaEmulationForPause(0);
}

// Copies a sound name up to '.', NUL or eight characters into an eight-byte key, zero-padded as native.
void CopySoundProgramKey(std::array<char, 12> &key, const char *name) noexcept {
    std::size_t index = 0;
    for (; index < 8 && name[index] != '.' && name[index] != 0; ++index)
        key[index] = name[index];
    for (; index < 8; ++index)
        key[index] = 0;
}

// Native SNDP_SoundProgramReader: selects the map's SMAP program (SNDA name unless "default", SNDD op-4 override, else
// the map name) and scene index (SNDR, SNDD op-5 override), honours the one-shot QDKJ suppression, then applies the
// SMAP banks. Returns 0 when no SNDP, SNDM or SMAP is found.
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

bool sbtLoadProgram() noexcept {
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

// @verified - p
void PrimeAicaEmulationForPause(std::int32_t enabled) noexcept {
    if (enabled) {
        if (runtime.pauseDepth < 1) {
            (void)ExecuteAicaCommand(0x1100a0U, 0, 0);
            (void)ExecuteAicaCommand(0x1300a0U, 0, 0);
            for (std::size_t offset = runtime.banks.size(); offset > 1; --offset) {
                const std::size_t bank = offset - 1;
                (void)ExecuteAicaCommand(0x2a0U, static_cast<std::uint32_t>(bank), 0);
                (void)ExecuteAicaCommand(0x1300a0U, static_cast<std::uint32_t>(bank), 0);
            }
            for (std::size_t offset = materialSoundHandles.size(); offset > 0; --offset) {
                const std::uint32_t slot = static_cast<std::uint32_t>(offset - 1);
                SoundEffectHandle &handle = materialSoundHandles[slot];
                if (handle.active == 0)
                    continue;
                (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
                (void)Dispatch(AicaBackendOperation::WideBankCommand, MATERIAL_SOUND_GAIN_COMMAND, slot, 0, 0);
                handle.active = 2;
            }
            (void)Dispatch(AicaBackendOperation::PauseStreams);
            PauseConversationCategories(true);
        } else {
            SetConversationVolume(nullptr, 254);
        }
        ++runtime.pauseDepth;
        return;
    }
    if (runtime.pauseDepth <= 0)
        return;
    --runtime.pauseDepth;
    if (runtime.pauseDepth == 1) {
        SetConversationVolume(nullptr, 255);
        return;
    }
    if (runtime.pauseDepth != 0)
        return;

    for (std::uint32_t bank = 1; bank < runtime.banks.size(); ++bank) {
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x1300a0U, bank, 0, 0);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x5a0U, bank, 0, runtime.banks[bank].currentVolume);
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::BankCommand, 0x3a0U, bank, 0, 0);
    }
    for (std::size_t offset = materialSoundHandles.size(); offset > 0; --offset) {
        const std::uint32_t slot = static_cast<std::uint32_t>(offset - 1);
        SoundEffectHandle &handle = materialSoundHandles[slot];
        if (handle.active == 0)
            continue;
        handle.active = 1;
        (void)Dispatch(AicaBackendOperation::SetBankProperty, 0, 1, 0, 0x5a);
        (void)Dispatch(AicaBackendOperation::WideBankCommand, MATERIAL_SOUND_GAIN_COMMAND, slot, 0, handle.gain);
    }
    (void)Dispatch(AicaBackendOperation::ResumeStreams);
    PauseConversationCategories(false);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void SoundProgramControl_Callback(void *) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    g_soundCallbackParity ^= 1U;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    runtime.callbackParity ^= 1U;
#endif
    const bool fadeCommandIssued = UpdateVolumeFades();

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (g_soundPauseDepth == 0)
        AdvanceSelectedSoundProgram(fadeCommandIssued);
    UpdateSampleProgram(SelectSampleProgram(g_samplePrograms));
    UpdateSampleProgram(SelectSampleProgram(g_secondarySamplePrograms));
    if (g_soundDebugMode > 0) {
        std::int32_t direction = 0;
        if (g_inputBuffer[3].repeatedPresses & 0x40U)
            direction = -1;
        else if (g_inputBuffer[3].repeatedPresses & 0x80U)
            direction = 1;
        if (g_soundDebugMode == 1) {
            g_soundDebugSelection += direction;
            if (g_soundDebugSelection < 0)
                g_soundDebugSelection = 0;
            SoundDebugDisplayNullsub(SelectSampleProgram(g_samplePrograms), "BGM AMB", &g_soundDebugSelection);
        } else if (g_soundDebugMode == 2) {
            g_soundDebugSecondarySelection += direction;
            if (g_soundDebugSecondarySelection < 0)
                g_soundDebugSecondarySelection = 0;
            SoundDebugDisplayNullsub(SelectSampleProgram(g_secondarySamplePrograms), "SE AMB", &g_soundDebugSecondarySelection);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (runtime.pauseDepth == 0) {
        UpdateSelectedSoundProgram(fadeCommandIssued);
        ++runtime.frame;
#endif
        }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    }
    UpdateSampleProgramDucking(SelectSampleProgram(g_samplePrograms), fadeCommandIssued);
    UpdateSampleProgramDucking(SelectSampleProgram(g_secondarySamplePrograms), fadeCommandIssued);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    UpdateSampleProgram(SelectSampleProgram(0));
    UpdateSampleProgram(SelectSampleProgram(2));
    UpdateSampleProgramDucking(SelectSampleProgram(0), fadeCommandIssued);
    UpdateSampleProgramDucking(SelectSampleProgram(2), fadeCommandIssued);
    RefreshSampleExecutionState();
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
void QueueSoundProgramControl() {
    HLib::EnqueueTaskWithoutParameter(SoundProgramControl_Callback, 14, 3, SHEN_NAME4('S', 'O', 'N', 'D'));
    InitializeMaterialSoundEffects();
    InitializeAmbientSoundProgram();
    ResetBankVolumeFades();
    g_soundListenerUsesCamera = 0;
    while (g_soundPauseDepth > 0)
        SetSoundPaused(0);
}


// @verified - d
void UpdateSelectedSoundProgram(bool fadeCommandIssued) {
    std::uint32_t requested = 0xffffffffU;
    std::int32_t duck = -1;
    std::uint32_t command;
    if (!g_selectedSoundProgram)
        return;
    const SelectedSoundProgramRecord *const record = g_selectedSoundProgram;
    if (record->startFrame == g_selectedSoundProgramFrame) {
        g_soundMusic.volume = static_cast<float>(record->volume);
        g_soundMusic.fadeFrames = -1;
        command = (static_cast<std::uint32_t>(record->sequence) << 8U) | 0xa8U;
        if (record->restart || command != static_cast<std::uint32_t>(g_soundMusic.activeProgramCommand) ||
            CompareSoundProgramKeys(g_soundMusic.bankName, record->bankName) || g_soundMusic.bank != record->bank) {
            ExecuteAicaCommand(0x4a0, record->bank, record->volume);
            if (g_soundMusicCommandEnabled)
                ExecuteAicaCommand(command, 0, 0);
            g_soundMusic.activeProgramCommand = command;
            std::strncpy(g_soundMusic.bankName, record->bankName, 8);
            g_soundMusic.bank = record->bank;
            g_soundMusic.publishedVolume = record->volume;
            g_soundMusic.duckVolume = 0;
        }
    }
    if (record->panFrame && record->panFrame == g_selectedSoundProgramFrame)
        ExecuteAicaCommand(0xaa0, record->bank, record->pan);
    const SoundProgramFadeRecord *fade = record->fades;
    for (std::int32_t index = 0; index < 16 && fade->frame != 0xffffU; ++index, ++fade) {
        if (fade->frame == g_selectedSoundProgramFrame) {
            g_soundMusic.fadeFrames = fade->duration + 1;
            const float target = fade->volume;
            const float frames = g_soundMusic.fadeFrames;
            g_soundMusic.volumeStep = (target - g_soundMusic.volume) / frames;
        }
    }
    if (g_soundMusic.fadeFrames > 0) {
        g_soundMusic.volume += g_soundMusic.volumeStep;
        if (g_soundMusic.volume > 127.0f)
            g_soundMusic.volume = 127.0f;
        if (g_soundMusic.volume < 0.0f)
            g_soundMusic.volume = 0.0f;
        requested = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_soundMusic.volume));
        --g_soundMusic.fadeFrames;
    }
    SampleProgramRecord *const loudest = GetLoudestSampleProgramRecord(SelectSampleProgram(g_samplePrograms));
    if (loudest) {
        const float volume = static_cast<float>(static_cast<std::int8_t>(loudest->volume));
        float minimum = static_cast<float>(static_cast<std::int8_t>(loudest->minimumMusicVolume));
        if (64.0f > minimum)
            minimum = 64.0f;
        if (volume > 64.0f) {
            requested = static_cast<std::uint32_t>(static_cast<std::int32_t>((1.0f - (volume - 64.0f) / 63.0f) * (g_soundMusic.volume - minimum) + minimum));
            if (static_cast<std::int32_t>(static_cast<std::uint32_t>(g_soundMusic.publishedVolume) - requested) > 2)
                requested = static_cast<std::uint32_t>(g_soundMusic.publishedVolume) - 2U;
        }
    }
    if (FindConversationByPriority(6)) {
        if (g_soundMusic.duckVolume == 0 && g_soundMusic.volume > 95.0f) {
            g_soundMusic.duckVolume = static_cast<std::int32_t>(g_soundMusic.volume);
            duck = g_soundMusic.duckVolume;
            g_soundMusic.duckDelay = 2;
        } else if (g_soundMusic.duckVolume != 0) {
            if (g_soundMusic.duckVolume > 95)
                --g_soundMusic.duckVolume;
            duck = g_soundMusic.duckVolume;
        }
    } else if (g_soundMusic.duckVolume != 0) {
        if (g_soundMusic.duckVolume < static_cast<std::int32_t>(g_soundMusic.volume)) {
            if (g_soundMusic.duckDelay == 0) {
                ++g_soundMusic.duckVolume;
                g_soundMusic.duckDelay = 2;
            } else {
                --g_soundMusic.duckDelay;
            }
            duck = g_soundMusic.duckVolume;
        } else {
            g_soundMusic.duckVolume = 0;
        }
    }
    if (duck > 0 && static_cast<std::uint32_t>(duck) < requested)
        requested = duck;
    if (requested < 65U)
        requested = 0;
    if (static_cast<std::int32_t>(requested) >= 0 && g_soundMusic.publishedVolume != static_cast<std::int32_t>(requested)) {
        if (static_cast<std::int32_t>(requested) > 127)
            requested = 127;
        if (g_soundMusic.activeProgramCommand >= 0 && !fadeCommandIssued) {
            ExecuteAicaCommand(0x4a0, record->bank, requested);
            g_soundMusic.publishedVolume = requested;
        }
    }
}


// @verified - d
void UpdateSampleProgramDucking(SampleProgramState *program, bool fadeCommandIssued) {
    if (!program || !program->executing)
        return;
    std::int32_t volume = -1;
    if (FindConversationByPriority(6) && program->duckVolume == 0) {
        program->duckVolume = 127;
        volume = program->duckVolume;
        program->duckDelay = 2;
    } else if (program->duckVolume != 0) {
        if (FindConversationByPriority(6)) {
            if (program->duckVolume > 95) {
                program->duckVolume -= 2;
                if (program->duckVolume < 95)
                    program->duckVolume = 95;
                volume = program->duckVolume;
            }
        } else if (program->duckVolume < 127) {
            if (program->duckDelay == 0) {
                program->duckVolume = program->duckVolume + 1;
                volume = program->duckVolume;
                program->duckDelay = 2;
            } else {
                --program->duckDelay;
            }
        } else {
            program->duckVolume = 0;
        }
    }
    if (volume >= 0 && !fadeCommandIssued)
        ExecuteAicaCommand(0x4a0, program->bank, volume);
}


// @verified - d
void SampleChannelCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value, const std::uint8_t *channels) {
    SoundDriverCommand record;
    record.material.bank = static_cast<std::int32_t>(bank);
    for (std::int32_t index = 0; index < 8 && *channels; ++index, ++channels) {
        record.material.variantCount = *channels;
        record.material.extendedCommand = command;
        record.value = value;
        DispatchSoundCommand(SoundCommandMode_Channel, &record);
    }
}


// @verified - d
void SampleChannelBankCommand(std::uint32_t command, std::uint32_t bank, const std::uint8_t *channels) {
    SoundDriverCommand record;
    record.material.bank = static_cast<std::int32_t>(bank);
    for (std::int32_t index = 0; index < 8 && *channels; ++index, ++channels) {
        record.material.extendedCommand = command;
        record.value = *channels;
        DispatchSoundCommand(SoundCommandMode_Bank, &record);
    }
}


// @verified - d
std::int32_t SoundPan(std::int32_t *pan, std::int32_t yaw, const Vector3 *source, const Vector3 *camera) {
    std::int32_t behind = 0;
    const std::int16_t direction = InverseFixedAtan2(source->x - camera->x, source->z - camera->z);
    std::int16_t angle = static_cast<std::int16_t>(direction - yaw);
    angle = static_cast<std::int16_t>(angle + 0x4000);
    if (angle < 0) {
        angle = static_cast<std::int16_t>(~angle);
        behind = 1;
    }
    const std::int16_t relative = static_cast<std::int16_t>(static_cast<std::uint16_t>(angle - 0x4000));
    std::int32_t value = -((static_cast<std::int32_t>(relative) * 64) >> 14);
    if (value > 40)
        value = 40;
    else if (value < -40)
        value = -40;
    *pan = value;
    return behind;
}


// @verified - d
float SoundDistanceGain(const Vector3 *listener, const Vector3 *source, float minimumDistance, float fadeDistance) {
    const float distance = source->y == 0.0f ? CalculatePlanarDistanceBetweenPoints(listener, source) : GetEuclideanDistance(listener, source);
    float gain;
    if (minimumDistance > distance)
        gain = 127.0f;
    else if (distance > fadeDistance + minimumDistance)
        gain = 0.0f;
    else
        gain = 127.0f / fadeDistance * (fadeDistance - (distance - minimumDistance));
    return gain;
}


// @verified - d
void UpdateSampleProgram(SampleProgramState *program) {
    Vector3 listener;
    Vector3 source;
    Vector3 camera;
    DateTime date;
    std::int32_t pan;
    std::int32_t index;
    if (!program || !program->executing)
        return;
    shenGetCurrentDateTime(date);
    std::int32_t quarter = date.hour * 4;
    float minute;
    if (date.minute >= 45) {
        quarter += 3;
        minute = static_cast<float>(date.minute - 45);
    } else if (date.minute >= 30) {
        quarter += 2;
        minute = static_cast<float>(date.minute - 30);
    } else if (date.minute >= 15) {
        ++quarter;
        minute = static_cast<float>(date.minute - 15);
    } else {
        minute = static_cast<float>(date.minute);
    }
    std::int32_t nextQuarter = quarter + 1;
    if (nextQuarter > 95)
        nextQuarter = 0;
    const std::int32_t yaw = camCtrlGetNextRotY();
    camCtrlGetNextCamPos(&camera);
    if (g_soundListenerUsesCamera) {
        listener = camera;
    } else {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterWorldPosition(control->ryo, &listener);
    }
    SampleProgramRecord *record = program->records;
    for (index = 0; index < program->count; ++index, ++record) {
        if (!record->disabled && ((record->weekdays >> static_cast<std::int8_t>(date.week)) & 1)) {
            float gain;
            switch (record->spatialMode) {
            case 1:
                if (record->characterName) {
                    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                    const Character *const character = scenFindChara(control->scenarioTask, shen_name(record->characterName));
                    if (!character)
                        continue;
                    GetCharacterWorldPosition(character, &source);
                } else {
                    source.x = record->position.x;
                    source.y = record->position.y;
                    source.z = record->position.z;
                }
                gain = SoundDistanceGain(&listener, &source, record->minimumDistance, record->fadeDistance);
                break;
            case 2:
                gain = 127.0f;
                break;
            case 3:
                gain = 127.0f;
                break;
            default:
                gain = 127.0f;
                break;
            }
            if (gain > 0.0f && record->timeMode) {
                float timeGain = static_cast<float>(record->quarterHourGains[quarter]) / 100.0f;
                if (record->timeMode == 1) {
                    const float difference = (static_cast<float>(record->quarterHourGains[nextQuarter]) / 100.0f - timeGain) * 0.066660002f;
                    timeGain += difference * minute;
                }
                gain *= timeGain;
            }
            if (gain > 0.0f) {
                switch (record->weatherMode) {
                case 1:
                    gain -= gain * GetWeatherWindVector();
                    break;
                case 2: {
                    float pressure = GetWeatherPressure();
                    if (pressure > 0.01f)
                        pressure += 1.0f;
                    else
                        pressure = GetWeatherWindVector();
                    gain -= 0.44999999f * pressure * gain;
                    break;
                }
                case 3:
                    gain = 32.0f + GetWeatherPressure() * gain;
                    if (gain > 127.0f)
                        gain = 127.0f;
                    break;
                }
            }
            std::int32_t volume = static_cast<std::int32_t>(gain);
            if (volume > 127)
                volume = 127;
            if (volume < 64)
                volume = 0;
            std::int32_t behind = 0;
            if (volume > 0 && record->panMode) {
                behind = SoundPan(&pan, yaw, &source, &camera);
                if (pan != record->pan) {
                    SampleChannelCommand(0xab0, program->bank, pan, record->channels);
                    record->pan = static_cast<std::int8_t>(pan);
                }
            }
            if (volume > 0 && record->panMode && record->frontGainPercent < 100) {
                std::int32_t attenuation;
                if (behind) {
                    float scale = static_cast<float>(pan) * 0.015625f;
                    if (0.0f > scale)
                        scale = -scale;
                    attenuation = static_cast<std::int32_t>((1.0f - static_cast<float>(record->frontGainPercent) / 100.0f) * 127.0f * (1.0f - scale));
                } else {
                    attenuation = 0;
                }
                if (attenuation != static_cast<std::int8_t>(record->attenuation)) {
                    SampleChannelCommand(0x2b0, program->bank, attenuation, record->channels);
                    record->attenuation = static_cast<std::uint8_t>(attenuation);
                }
            }
            if (volume != static_cast<std::int8_t>(record->volume)) {
                if (volume == 0)
                    SampleChannelBankCommand(0x28a0, program->bank, record->channels);
                else if (record->volume == 0)
                    SampleChannelBankCommand(0x29a0, program->bank, record->channels);
                if (volume == 0 || static_cast<std::uint32_t>(volume & 1) != g_soundCallbackParity) {
                    SampleChannelCommand(0x7b0, program->bank, volume, record->channels);
                    record->volume = static_cast<std::uint8_t>(volume);
                }
            }
        }
    }
}


// @verified - d
std::int32_t spmSetSampleEnabled(std::int32_t sample, std::int32_t enabled) {
    return SetSampleProgramEnabled(sample, enabled, SelectSampleProgram(g_samplePrograms));
}


// @verified - d
std::int32_t spmSetSecondarySampleEnabled(std::int32_t sample, std::int32_t enabled) {
    return SetSampleProgramEnabled(sample, enabled, SelectSampleProgram(g_secondarySamplePrograms));
}


// @verified - d
std::int32_t SetSampleProgramEnabled(std::int32_t sample, std::int32_t enabled, SampleProgramState *program) {
    if (!sample)
        return 0;
    --sample;
    SampleProgramRecord *record = program->records;
    record += sample;
    if (enabled) {
        record->disabled = 0;
    } else {
        record->disabled = 1;
        record->volume = 0;
        record->pan = 0;
        record->attenuation = 0;
        SampleChannelBankCommand(0x28a0, program->bank, record->channels);
        SampleChannelCommand(0x7b0, program->bank, 0, record->channels);
    }
    return 1;
}


// @verified - d
SampleProgramState *SelectSampleProgram(SampleProgramState *first) {
    SampleProgramState *program = first;
    for (std::int32_t index = 0; index < 2; ++index, ++program) {
        if (program->records && g_soundBankLoadStates[program->bank] > -1)
            return program;
    }
    return 0;
}


// @verified - d
void blurInit() {
    g_savedFightBlurFlag = 0;
    blurModelPreviousFlag = 0;
    blurModelFlag = 0;
    accumulatedFightBlur = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

} // namespace s2

} // namespace shenmue

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace shenmue

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_PlayFootstepForNode(std::uint32_t flags, shenmue::s2::Character *character) {
    return shenmue::s2::PlayFootstepForNode(flags, character);
}


extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_Shenmue_Execute3DSoundCommand(
    std::uint32_t flags, const shenmue::Vector3 *position, float minimumDistance, float fadeDistance) {
    return shenmue::s2::Execute3DSoundCommand(flags, position, minimumDistance, fadeDistance);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_Loop3DSoundEffect(std::uint32_t flags) {
    return shenmue::s2::Loop3DSoundEffect(flags);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_ExecuteAicaCommand_Wrapper(
    shenmue::s2::SoundEffectHandle *handle, std::uint32_t command, std::int32_t value) {
    return shenmue::s2::ExecuteMaterialSoundHandleCommand(handle, command, value);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_StopMaterialSoundEffect(shenmue::s2::SoundEffectHandle *handle) {
    return shenmue::s2::FreeMaterialSoundHandle(handle);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_UpdateMaterialSoundEffectPosition(
    shenmue::s2::SoundEffectHandle *handle, const shenmue::Vector3 *position, float minimumDistance, float fadeDistance, std::int32_t controlValue) {
    return shenmue::s2::UpdateMaterialSoundEffectPosition(handle, position, minimumDistance, fadeDistance, controlValue);
}
#endif
