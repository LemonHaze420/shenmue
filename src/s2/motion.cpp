#include "s2/motion.hpp"

#include <shendk/files/animation/motn_reader.h>

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "d3t/object_store.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <cstdio>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace shenmue::s2 {

std::array<MotionRegistryEntry, 32> g_motionRegistry{};
MotionRegistryEntry *g_motionData = nullptr;
MotionRegistryEntry *g_battleMotionData = nullptr;
std::byte *g_fallbackMotionTable = nullptr;
std::array<std::int32_t *, 4> g_fallbackMotionSections{};
std::int32_t g_motionBankCount = 0;
std::uint32_t g_totalMotionCount = 0;
std::byte *g_armMotionMemory = nullptr;
std::byte *g_unfoSpace = nullptr;
float DecodeMotnFloat(std::uint16_t raw) {
#if 0
    if (!raw)
        return 0.0f;
    const std::uint32_t bits = (static_cast<std::uint32_t>(raw & 0x8000U) << 16U) | (static_cast<std::uint32_t>((raw & 0x7fffU) + 0x1c000U) << 13U);
    return std::bit_cast<float>(bits);
#else
    return shendk::DecodeMotnFloat(raw);
#endif
}

namespace {

std::uint32_t unfoState = 0;
std::array<MotnFile, 32> decodedMotionBanks;
std::span<const MotnFile> authorMotionBanks;
std::array<std::uint64_t, 32> motionBankSizes{};
struct RetiredMotionRange {
    std::uint16_t end;
    std::uint16_t begin;
};
std::array<RetiredMotionRange, 32> retiredMotionRanges{};
HLib::Task *motionRetirementTask{};

struct MotionPropBank {
    std::int32_t handle{-1};
    std::uint32_t reserved{};
    std::byte *data{};
    MotionPropBank *previous{};
    MotionPropBank *next{};
};
static_assert(sizeof(MotionPropBank) == 0x20);
std::array<MotionPropBank, 6> motionPropBanks{{{0}}};
std::array<std::vector<MT7Model>, 6> motionPropModels;

#pragma pack(push, 2)
struct MotionPropDescriptor {
    std::uint16_t prop;
    std::uint16_t bone;
    std::byte reserved4[4];
    std::uint16_t position[3];
    std::int16_t rotation[3];
    std::uint16_t windowCount;
};
#pragma pack(pop)
static_assert(sizeof(MotionPropDescriptor) == 0x16);

void RetireMotionRange(std::uint16_t begin, std::uint16_t end) {
    if (!end || end < begin)
        return;
    for (RetiredMotionRange &range : retiredMotionRanges) {
        if (!range.end) {
            range = {end, begin};
            return;
        }
    }
    throw std::runtime_error("MOTF native retirement capacity exceeded");
}

// MotFileManage: every MOTF frame clears all 32 retired-range words, so a range retired by
// DeleteMotionInternal only blocks its motion IDs until the next MOTF execution.
// @verified - p
void MotFileManage() {
    retiredMotionRanges = {};
}

// MotFileManageTaskEntry_Callback: the MOTF task callback is a thunk to MotFileManage.
// @verified - p
void MotFileManageTaskEntry_Callback(void *) {
    MotFileManage();
}
constexpr std::uint32_t kUnfoSlotCount = 16;
constexpr std::uint32_t kUnfoSlotSize = 0xec;
constexpr std::uint32_t kUnfoSize = kUnfoSlotCount * kUnfoSlotSize;

#pragma pack(push, 4)
struct MotionSnapshot {
    std::int32_t slot;
    shen_name character;
    std::uint8_t requestedRootMode;
    std::byte reserved09[3];
    std::uint16_t motionIds[5];
    std::uint16_t defaultStepFrames[5];
    std::uint16_t regionFlags[5];
    std::byte reserved2a[2];
    std::uint32_t motionFlags;
    std::uint32_t sequenceFlags;
    std::uint32_t environmentMotion;
    Vector3i entityRotation;
    Vector3i worldRotation;
    Vector3 rootPosition;
    Vector3 worldPosition;
    float splitFrames[5];
    float ticksTime[5];
    float maxTime[5];
    float playbackSpeed[5];
    float constraintStepLimit[9];
    float projectionCotangents[3];
    std::uint32_t callbackFlags;
};
#pragma pack(pop)
static_assert(sizeof(MotionSnapshot) == kUnfoSlotSize);
static_assert(offsetof(MotionSnapshot, motionFlags) == 0x2c);
static_assert(offsetof(MotionSnapshot, rootPosition) == 0x50);
static_assert(offsetof(MotionSnapshot, playbackSpeed) == 0xa4);
static_assert(offsetof(MotionSnapshot, callbackFlags) == 0xe8);

MotionSnapshot *GetMotionSnapshot(std::uint32_t slot) {
    return g_unfoSpace && slot < kUnfoSlotCount ? reinterpret_cast<MotionSnapshot *>(g_unfoSpace + slot * kUnfoSlotSize) : nullptr;
}

Character *FindSnapshotCharacter(shen_name name) {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? FindScenarioCharacter(control->scenarioTask, name) : nullptr;
}

std::array<MotionRigClock *, 5> SnapshotClocks(MotionWork &work) {
    return {&work.rig.lowerBody.clock, &work.rig.chest.clock, &work.rig.head.clock, &work.rig.rightArm.clock, &work.rig.leftArm.clock};
}

void AllocateAndSetUNFOFlag() {
    g_unfoSpace = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(kUnfoSize, SHEN_NAME(UNFO)));
    if (g_unfoSpace) {
        std::memset(g_unfoSpace, 0, kUnfoSize);
    }
    HLib::SetTaskDataMotionFlag(g_unfoSpace);
}

std::uint32_t InitializeUNFO() {
    const std::uint32_t previousState = unfoState;
    unfoState = 0;
    if (!g_unfoSpace) {
        return previousState;
    }

    std::memset(g_unfoSpace, 0, kUnfoSize);
    for (std::uint32_t index = 0; index < kUnfoSlotCount; ++index) {
        std::byte *const slot = g_unfoSpace + index * kUnfoSlotSize;
        const std::int32_t unused = -1;
        const std::uint32_t emptyName = SHEN_NAME(____);
        std::memcpy(slot, &unused, sizeof(unused));
        std::memcpy(slot + 4, &emptyName, sizeof(emptyName));
    }
    return previousState;
}

template <typename Value> bool ReadValue(const std::byte *data, std::uint64_t size, std::uint64_t offset, Value &value) {
    if (!data || offset > size || sizeof(Value) > size - offset) {
        return false;
    }
    std::memcpy(&value, data + offset, sizeof(Value));
    return true;
}

std::byte *ReadMotionFile(const char *directory, const char *name, shen_name allocationName, std::uint64_t &size) {
    if (!directory || !name)
        return nullptr;
    char path[256]{};
    const int length = std::snprintf(path, sizeof(path), "%s%s/%s", *directory == '/' ? "" : "/", directory, name);
    if (length <= 0 || static_cast<std::size_t>(length) >= sizeof(path)) {
        return nullptr;
    }
    size = ArchiveData_GetFileSize(path);
    if (!size || size > std::numeric_limits<std::size_t>::max()) {
        return nullptr;
    }
    auto *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, allocationName));
    if (!data || !ArchiveData_Read(path, 0, data, size)) {
        if (data) {
            HLib::FreeTaskData(data);
        }
        return nullptr;
    }
    return data;
}



std::byte *ReadMiscFile(const char *name, shen_name allocationName, std::uint64_t &size) {
    char path[256]{};
    const int length = std::snprintf(path, sizeof(path), "/misc/%s", name);
    if (length <= 0 || static_cast<std::size_t>(length) >= sizeof(path))
        return nullptr;
    size = ArchiveData_GetFileSize(path);
    if (!size || size > std::numeric_limits<std::size_t>::max())
        return nullptr;
    auto *const data = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(size, allocationName));
    if (!data)
        return nullptr;
    if (!ArchiveData_Read(path, 0, data, size)) {
        HLib::FreeTaskData(data);
        return nullptr;
    }
    HLib::SetTaskDataMotionFlag(data);
    return data;
}

bool BindMotionHeader(MotionRegistryEntry &bank, std::uint64_t size) {
    return ReadValue(bank.data, size, 0x14, bank.firstMotion) && ReadValue(bank.data, size, 0x0c, bank.lastMotion) &&
        ReadValue(bank.data, size, 0x10, bank.motionCount);
}

bool BindFallbackMotionTable(std::uint64_t size) {
    if (!g_fallbackMotionTable || size < 4 * sizeof(std::int32_t)) {
        return false;
    }
    auto *const offsets = reinterpret_cast<std::int32_t *>(g_fallbackMotionTable);
    for (std::size_t index = 0; index < g_fallbackMotionSections.size(); ++index) {
        const std::int32_t offset = offsets[index];
        if (offset < 0 || static_cast<std::uint64_t>(offset) >= size) {
            return false;
        }
        g_fallbackMotionSections[index] = reinterpret_cast<std::int32_t *>(g_fallbackMotionTable + offset);
    }
    return true;
}

template <typename Value> bool ReadSpanValue(std::span<const std::byte> bytes, std::size_t offset, Value &value) {
    if (offset > bytes.size() || sizeof(Value) > bytes.size() - offset) {
        return false;
    }
    std::memcpy(&value, bytes.data() + offset, sizeof(Value));
    return true;
}

std::uint16_t ReadMotionU16(const std::byte *source) {
    std::uint16_t value;
    std::memcpy(&value, source, sizeof(value));
    return value;
}

template <class Visitor> void VisitMotionProps(const void *track, std::uint32_t region, Visitor &&visitor) {
    if (!track)
        return;
    const auto *const bytes = static_cast<const std::byte *>(track);
    const std::uint16_t count = ReadMotionU16(bytes);
    for (std::uint32_t index = 0; index < count; ++index) {
        const auto *descriptor = reinterpret_cast<const MotionPropDescriptor *>(bytes + ReadMotionU16(bytes + 2U + index * 2U));
        if (static_cast<std::uint8_t>(descriptor->bone) != 44 && (descriptor->bone >> 8) == region)
            visitor(*descriptor);
    }
}

MT7Model *FindMotionProp(std::uint16_t id, std::int32_t handle = -1) {
    if (!motionPropBanks[0].next)
        return nullptr;
    const auto searchBank = [id](MotionPropBank &bank) -> MT7Model * {
        const std::size_t slot = static_cast<std::size_t>(&bank - motionPropBanks.data());
        auto &models = motionPropModels[slot];
        for (std::size_t index = 0; index < models.size(); ++index) {
            std::uint32_t prop{};
            std::memcpy(&prop, bank.data + 8 + index * 8, sizeof(prop));
            if (prop == id)
                return models[index].nodes.empty() ? nullptr : &models[index];
        }
        return nullptr;
    };
    if (handle != -1) {
        for (MotionPropBank *bank = &motionPropBanks[0]; bank; bank = bank->next)
            if (bank->data && bank->handle == handle)
                if (MT7Model *const model = searchBank(*bank))
                    return model;
    }
    for (MotionPropBank *bank = &motionPropBanks[0]; bank; bank = bank->next)
        if (bank->data)
            if (MT7Model *const model = searchBank(*bank))
                return model;
    return nullptr;
}

void RetireMotionBankTracks(const std::byte *data, std::uint64_t size) {
    struct Allocation {
        std::uintptr_t begin;
        std::uint64_t size;
    } allocation{reinterpret_cast<std::uintptr_t>(data), size};
    ForEachLiveCharacterInMapOrder(
        [](Character *character, void *opaque) {
            const auto &allocation = *static_cast<const Allocation *>(opaque);
            MotionWork *const work = FindMotionWork(character);
            if (!work)
                return;
            auto owns = [&](const void *pointer) { return pointer && reinterpret_cast<std::uintptr_t>(pointer) - allocation.begin < allocation.size; };
            for (std::uint32_t region = 0; region < 5; ++region) {
                for (void **pointer : {&work->runtime.regionTrack[region], &work->runtime.previousRegionTrack[region]}) {
                    if (!owns(*pointer))
                        continue;
                    RetireMotionPropTrack(*pointer, region);
                    *pointer = nullptr;
                }
            }
            if (owns(work->runtime.rightArmTrack))
                work->runtime.rightArmTrack = nullptr;
            if (owns(work->runtime.leftArmTrack))
                work->runtime.leftArmTrack = nullptr;
        },
        &allocation);
    while (d3t::ProcessObjectStore(true, -1)) {
    }
}


void SampleTimedNodes(TimedNodeMotion &motion, std::int32_t frame, float fraction) {
    const std::byte *const data = motion.data;
    const std::size_t count = std::min<std::size_t>(ReadMotionU16(data + 8), motion.model->nodes.size());
    for (std::size_t index = 0; index < count; ++index) {
        MT7Node &node = motion.model->nodes[index];
        const std::byte *const record = data + index * 0x20;
        const std::uint16_t scaleOffset = ReadMotionU16(record + 12);
        auto scale = [&](std::size_t axis) {
            const std::byte *const source = scaleOffset ? data + scaleOffset + frame * 6 + axis * 2 : record + 14 + axis * 2;
            float value = DecodeMotnFloat(ReadMotionU16(source));
            if (scaleOffset && fraction != 0.0f)
                value += (DecodeMotnFloat(ReadMotionU16(source + 6)) - value) * fraction;
            return value;
        };
        auto rotation = [&](std::size_t axis) {
            const std::uint16_t offset = ReadMotionU16(record + 20 + axis * 4);
            const std::byte *const source = offset ? data + offset + frame * 2 : record + 22 + axis * 4;
            const std::int16_t value = static_cast<std::int16_t>(ReadMotionU16(source));
            std::int32_t result = value;
            if (offset && fraction != 0.0f) {
                const std::int16_t delta = static_cast<std::int16_t>(ReadMotionU16(source + 2) - ReadMotionU16(source));
                result += static_cast<std::int16_t>(static_cast<std::int32_t>(delta * fraction));
            }
            return result;
        };
        auto position = [&](std::size_t axis) {
            const std::uint16_t offset = ReadMotionU16(record + 32 + axis * 4);
            const std::byte *const source = offset ? data + offset + frame * 2 : record + 34 + axis * 4;
            float value = DecodeMotnFloat(ReadMotionU16(source));
            if (offset && fraction != 0.0f)
                value += (DecodeMotnFloat(ReadMotionU16(source + 2)) - value) * fraction;
            return value;
        };
        node.position = {position(0), position(1), position(2)};
        node.rotation = {rotation(0), rotation(1), rotation(2)};
        node.scale = {scale(0), scale(1), scale(2)};
    }
}

bool ReadMotnTuple(std::span<const std::byte> bytes, std::size_t &cursor, float &value, float &leftSlope, float &rightSlope) {
#if 0
    std::uint16_t rawValue{}, rawLeft{}, rawRight{};
    if (!ReadSpanValue(bytes, cursor, rawValue) || !ReadSpanValue(bytes, cursor + 2, rawLeft)) {
        return false;
    }
    cursor += 4;
    rawRight = rawLeft;
    if (rawValue & 1U) {
        if (!ReadSpanValue(bytes, cursor, rawRight))
            return false;
        cursor += 2;
    }
    value = DecodeMotnFloat(rawValue);
    leftSlope = DecodeMotnFloat(rawLeft);
    rightSlope = DecodeMotnFloat(rawRight);
    return true;
#else
    if (cursor > bytes.size())
        return false;
    shendk::Bytes source = bytes.subspan(cursor);
    shendk::S2MotnTuple tuple{};
    if (!shendk::NextS2MotnTuple(source, tuple)) {
        if (bytes.size() - cursor >= 4)
            cursor += 4;
        return false;
    }
    cursor = bytes.size() - source.size();
    value = DecodeMotnFloat(tuple.value);
    leftSlope = DecodeMotnFloat(tuple.leftSlope);
    rightSlope = DecodeMotnFloat(tuple.rightSlope);
    return true;
#endif
}

bool IsMotnNameCharacter(std::uint8_t value) {
    return value == '.' || value == '_' || (value >= '0' && value <= '9') || (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
}



constexpr std::uint8_t motnCommandWidths[] = {12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 4, 8, 8, 8, 0, 4, 4,
    16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 4, 8, 8, 8, 16, 12, 28, 4, 8, 8, 4, 8, 8, 8, 8, 8, 12, 8, 12, 8, 8, 4, 8, 8, 4, 4, 4, 4, 16, 8, 12,
    8, 4, 8, 4, 4, 0};

bool ReadMotnTimedCommands(std::span<const std::byte> bytes, std::size_t cursor, MotnSequence &sequence) {
    for (;;) {
        if (cursor > bytes.size() || bytes.size() - cursor < 3)
            return false;
        std::uint16_t frame{};
        if (!ReadSpanValue(bytes, cursor, frame))
            return false;
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(bytes[cursor + 2]);
        if (!opcode)
            return true;
        if (opcode >= sizeof(motnTimedCommandWidths) || !motnTimedCommandWidths[opcode]) {
            return false;
        }
        const std::size_t width = motnTimedCommandWidths[opcode];
        if (width > bytes.size() - cursor)
            return false;

        if (opcode == 4U) {
            MotnFootstepCueCommand command{};
            command.frame = frame;
            if (!ReadSpanValue(bytes, cursor + 3, command.material) || !ReadSpanValue(bytes, cursor + 4, command.cue)) {
                return false;
            }
            sequence.footstepCues.push_back(command);
        } else if (opcode == 49U) {
            MotnRandomFootstepCueCommand command{};
            command.frame = frame;
            if (!ReadSpanValue(bytes, cursor + 3, command.material)) {
                return false;
            }
            for (std::size_t index = 0; index < command.cues.size(); ++index) {
                if (!ReadSpanValue(bytes, cursor + 4 + index * 4, command.cues[index])) {
                    return false;
                }
            }
            sequence.randomFootstepCues.push_back(command);
        } else if (opcode == 52U) {
            MotnPlayerMotionFlagCommand command{};
            command.frame = frame;
            if (!ReadSpanValue(bytes, cursor + 4, command.flags)) {
                return false;
            }
            sequence.playerMotionFlagCommands.push_back(command);
        }
        cursor += width;
    }
}

bool ReadMotnPlaybackCommands(std::span<const std::byte> bytes, std::int32_t metadataOffset, MotnSequence &sequence) {
    if (!metadataOffset)
        return true;
    if (metadataOffset < 0)
        return false;

    std::size_t cursor = static_cast<std::size_t>(metadataOffset);
    if (cursor > bytes.size() || bytes.size() - cursor < 0x0d)
        return false;
    cursor += 0x0c;
    for (;;) {
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(bytes[cursor]);
        if (opcode == 1U) {
            std::uint16_t parameter{};
            if (!ReadSpanValue(bytes, cursor + 2, parameter))
                return false;
            if (parameter == 0xffffU)
                sequence.allowsFrameOverflow = true;
        } else if (opcode == 19U) {
            sequence.hasLeadFootCommand = true;
        } else if (opcode == 24U) {
            sequence.hasLoopCommand = true;
        } else if (opcode == 8U) {
            return ReadMotnTimedCommands(bytes, cursor + 4, sequence);
        } else if (!opcode) {
            return true;
        }
        if (opcode >= sizeof(motnCommandWidths) || !motnCommandWidths[opcode]) {
            return false;
        }
        const std::size_t width = motnCommandWidths[opcode];
        if (width > bytes.size() - cursor)
            return false;
        cursor += width;
        if (cursor >= bytes.size())
            return false;
    }
}

std::span<const std::byte> FindMotionBankPayload(std::uint32_t runtimeId, bool metadata) {
    for (const MotionRegistryEntry *bank = g_motionData; bank; bank = bank->next) {
        if (runtimeId <= bank->firstMotion || runtimeId >= bank->lastMotion)
            continue;
        const std::size_t index = static_cast<std::size_t>(bank - g_motionRegistry.data());
        const std::uint64_t size = motionBankSizes[index];
        std::uint32_t table{}, data{};
        const std::uint64_t local = runtimeId - bank->firstMotion - 1U;
        if (!ReadValue(bank->data, size, 0, table) || (!metadata && !ReadValue(bank->data, size, 8, data)))
            return {};
#if 0
        std::uint32_t offset{};
        if (!ReadValue(bank->data, size, table + local * 8U + (metadata ? 4U : 0U), offset))
            return {};
        const bool emptyMetadata = metadata && offset == 0;
#else
        const shendk::MotnBankHeader header{.sequenceTableOffset = table, .sequenceDataOffset = data};
        shendk::Bytes payload;
        if (!shendk::ReadMotnPayload({bank->data, static_cast<std::size_t>(size)}, header, static_cast<std::uint32_t>(local), metadata, payload))
            return {};
        const bool emptyMetadata = metadata && payload.empty();
#endif
        if (emptyMetadata) {


            static constexpr std::byte emptyMotionMetadata[0x28]{};
            return emptyMotionMetadata;
        }
#if 0
        const std::uint64_t start = static_cast<std::uint64_t>(data) + offset;
        if (start >= size)
            return {};
        return {bank->data + start, static_cast<std::size_t>(size - start)};
#else
        return payload;
#endif
    }
    return {};
}
} // namespace




// @verified - p
std::int32_t MotionPropFBX(std::span<const std::byte> motp) {
    if (motp.empty())
        return -1;
    MotionPropBank *tail = &motionPropBanks[0];
    for (MotionPropBank *bank = motionPropBanks[0].next; bank; bank = bank->next)
        tail = bank;
    std::size_t slot = 1;
    while (slot < motionPropBanks.size() && motionPropBanks[slot].handle != -1)
        ++slot;
    if (slot == motionPropBanks.size())
        return -1;
    const std::int32_t handle = tail->handle + 1;
    MotionPropBank &bank = motionPropBanks[slot];
    tail->next = &bank;
    bank.handle = handle;
    bank.previous = tail;
    bank.next = nullptr;
    bank.data = const_cast<std::byte *>(motp.data());
    std::int32_t count{};
    ReadSpanValue(motp, 0, count);
    auto &models = motionPropModels[slot];
    models.clear();
    if (count > 0)
        models.resize(static_cast<std::size_t>(count));
    for (std::int32_t index = 0; index < count; ++index) {
        std::uint32_t id{};
        std::int32_t offset{};
        ReadSpanValue(motp, 8ULL + index * 8ULL, id);
        ReadSpanValue(motp, 12ULL + index * 8ULL, offset);
        if (offset < 0 || static_cast<std::size_t>(offset) >= motp.size())
            continue;
        char label[260];
        std::snprintf(label, sizeof(label), "/scene/misc/MotProp%03u/MotProp%03u.fbx", id, id);
        ReadModelFormat7(motp.subspan(static_cast<std::size_t>(offset)), label, models[static_cast<std::size_t>(index)], 0x22);
    }
    return bank.handle;
}

std::int32_t LoadMotionProps(const char *directory, const char *filename, shen_name allocationName) {
    std::uint64_t size{};
    std::byte *const data = ReadMotionFile(directory, filename, allocationName, size);
    if (!data)
        return -1;
    const std::int32_t handle = MotionPropFBX({data, static_cast<std::size_t>(size)});
    if (handle == -1)
        HLib::FreeTaskData(data);
    return handle;
}

bool DeleteMotionProps(std::int32_t handle) {
    if (handle < 1)
        return false;
    MotionPropBank *bank = &motionPropBanks[0];
    while (bank && bank->handle != handle)
        bank = bank->next;
    if (!bank)
        return false;
    const std::size_t slot = static_cast<std::size_t>(bank - motionPropBanks.data());
    for (MT7Model &model : motionPropModels[slot])
        ReleaseModelFormat7(model);
    motionPropModels[slot].clear();
    HLib::FreeTaskData(bank->data);
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    *bank = {};
    return true;
}


// @verified - p
bool MOTPParser(std::int32_t handle) {
    if (handle < 1)
        return false;
    MotionPropBank *bank = &motionPropBanks[0];
    while (bank && bank->handle != handle)
        bank = bank->next;
    if (!bank)
        return false;
    const std::size_t slot = static_cast<std::size_t>(bank - motionPropBanks.data());
    for (MT7Model &model : motionPropModels[slot])
        ReleaseModelFormat7(model);
    motionPropModels[slot].clear();
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    bank->handle = -1;
    bank->data = nullptr;
    bank->previous = nullptr;
    bank->next = nullptr;
    return true;
}

// @verified - p
std::int32_t ReadMotionPropsFromDirectory(std::int32_t act, shen_name map, const char *filename) {
    char directory[64];
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", act, map.characters[0], map.characters[1], map.characters[2], map.characters[3]);
    return LoadMotionProps(directory, filename, SHEN_NAME(MOTI));
}

void RegisterMotionPropTrack(const void *track, std::uint32_t region) {
    VisitMotionProps(track, region, [](const MotionPropDescriptor &descriptor) {
        if (MT7Model *const model = FindMotionProp(descriptor.prop))
            d3t::RegisterObjectPlacement(GetMT7ReaderData(model), &descriptor, 0);
    });
}

void RetireMotionPropTrack(const void *track, std::uint32_t region) {
    VisitMotionProps(track, region, [](const MotionPropDescriptor &descriptor) { d3t::RetireObjectPlacement(&descriptor); });
}

void ReleaseMotionPropTracks(const MotionWork &work) {
    for (std::uint32_t region = 0; region < 5; ++region) {
        RetireMotionPropTrack(work.runtime.regionTrack[region], region);
        RetireMotionPropTrack(work.runtime.previousRegionTrack[region], region);
    }
}

void DrawMotionParts(Character &character, MotionWork &work, bool forceDisable) {
    const bool previousBusy = SetLegacyBusyPreparingDraw(true);
    const shen_name name = GetCharacterScenarioName(&character);
    const bool blend = g_currentMap.name != SHEN_NAME(AR01) || (name.name != SHEN_NAME(SZY_) && name.name != SHEN_NAME(SDN_));
    const auto drawTrack = [&](std::uint32_t region, bool previous) {
        const MotionRuntimeState &state = work.runtime;
        VisitMotionProps(previous ? state.previousRegionTrack[region] : state.regionTrack[region], region, [&](const MotionPropDescriptor &descriptor) {
            const auto *const windows = reinterpret_cast<const std::byte *>(&descriptor) + sizeof(descriptor);
            for (std::int32_t window = 0; window < static_cast<std::int32_t>(descriptor.windowCount) - 1; ++window) {
                const std::uint16_t begin = ReadMotionU16(windows + window * 2);
                if (!(begin & 0x8000U))
                    continue;
                const float frame = GetMotionFrame(&work, static_cast<MotionRegion>(region));
                const bool transitioning = (state.regionFlags[region] & MRF_Transition) != 0;
                const float time =
                    previous ? state.regionPreviousTicksTime[region] : (transitioning && blend ? std::fmax(0.0f, state.layerSampleFrame[region]) : frame);
                const float alpha = character.alpha *
                    (blend && transitioning ? (previous ? 1.0f - frame / state.blendTotalFrames[region] : frame / state.blendTotalFrames[region]) : 1.0f);
                if (window && static_cast<float>(begin & 0x7fffU) > time)
                    break;
                if (static_cast<float>(ReadMotionU16(windows + window * 2 + 2) & 0x7fffU) < time)
                    continue;
                MT7Model *const model = FindMotionProp(descriptor.prop);
                const auto boneId = static_cast<std::uint8_t>(descriptor.bone);
                const Matrix4x4 *bone = charaGetBoneTransform(&work, boneId);


                Matrix4x4 modelSpace;
                if (forceDisable && !previous && ResolveAuthoredMotionBoneTransform(&character, boneId, modelSpace))
                    bone = &modelSpace;
                if (!model || !bone)
                    continue;
                MT7Node &root = model->nodes[0];
                root.position = {DecodeMotnFloat(descriptor.position[0]), DecodeMotnFloat(descriptor.position[1]), DecodeMotnFloat(descriptor.position[2])};
                root.rotation = {descriptor.rotation[0], descriptor.rotation[1], descriptor.rotation[2]};
                Matrix4x4_Push();
                if (forceDisable && !previous)
                    Matrix4x4_PushActiveTransform();
                Matrix4x4::MultiplyWithPrePassMatrix(bone);
                if (forceDisable && !previous) {
                    ApplyMT7NodeTransform(root);
                    d3t::HashModelWithAlpha(&descriptor, alpha, GetLegacyPerspectiveProjection());
                    Matrix4x4::Pop(1);
                } else {
                    charaPrepareNodesForDraw(model, &root, alpha);
                }
                Matrix4x4::Pop(1);
            }
        });
    };
    try {
        for (std::uint32_t region = 0; region < 5; ++region) {
            if (work.runtime.regionFlags[region] & MRF_Mirrored)
                continue;
            drawTrack(region, false);
            if (work.runtime.regionFlags[region] & MRF_Transition)
                drawTrack(region, true);
        }
    } catch (...) {
        SetLegacyBusyPreparingDraw(previousBusy);
        throw;
    }
    SetLegacyBusyPreparingDraw(previousBusy);
}
void TMNT_Callback(Character *character, void *payload) {
    auto *const motion = static_cast<TimedNodeMotion *>(payload);
    if (character && motion && motion->mode != 0)
        TickTimedNodeMotion(*motion);
}

bool AllocateTMNM(Character *character) {
    auto *const motion = static_cast<TimedNodeMotion *>(AllocateModelComponent(character, TASK_TMNM, sizeof(TimedNodeMotion), TMNT_Callback));
    if (!motion)
        return false;
    motion->mode = 0;
    motion->data = nullptr;
    imgmAssignUpdateCallback(character, charaApplyScale);
    charaAssignPrepareForDrawCallback(character, charaPrepareNodesForDraw);
    return true;
}

void InitializeTimedNodeMotion(TimedNodeMotion &motion, MT7Model *model, std::uint16_t id) {
    const std::span<const std::byte> data = FindLoadedMotionData(id);
    if (data.empty())
        return;
    motion.mode = 1;
    motion.data = data.data();
    motion.model = model;
    motion.frame = 0.0f;
    motion.begin = 0.0f;
    motion.end = static_cast<float>(static_cast<std::int16_t>(ReadMotionU16(motion.data)));
    motion.speed = DecodeMotnFloat(ReadMotionU16(motion.data + 4));
    motion.flags = 0;
}
// @verified - p
std::uintptr_t ConfigureScenarioNodeMotion(shen_name name, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return 0;
    TimedNodeMotion *const motion = static_cast<TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
    if (!motion)
        return 0;
    if (motionId != 0)
        InitializeTimedNodeMotion(*motion, static_cast<MT7Model *>(character->model), static_cast<std::uint16_t>(motionId));
    if (begin >= 0.0f)
        motion->begin = begin;
    if (end >= 0.0f)
        motion->end = end;
    if (frame >= 0.0f)
        motion->frame = frame;
    if (speed < 32767.0f)
        motion->speed = speed;
    if (flags >= 0) {
        if (flags != 0)
            motion->flags |= static_cast<std::uint16_t>(flags);
        else
            motion->flags = 0;
    }
    return static_cast<std::uint32_t>(flags);
}

// @verified - p
bool IsScenarioNodeMotionFinished(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return false;
    const TimedNodeMotion *const motion = static_cast<const TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
    return motion->mode == 2;
}

// @verified - p
TimedNodeMotion *StopScenarioNodeMotion(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return nullptr;
    TimedNodeMotion *const motion = static_cast<TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
    motion->mode = 0;
    return motion;
}

// @verified - p
void CopyScenarioNodeMotionTimes(shen_name name, float *begin, float *end, float *frame, float *speed) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return;
    const TimedNodeMotion *const motion = static_cast<const TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
    if (begin)
        *begin = motion->begin;
    if (end)
        *end = motion->end;
    if (frame)
        *frame = motion->frame;
    if (speed)
        *speed = motion->speed;
}

void ApplyNodeMotionFrame(MT7Model &model, std::span<const std::byte> data, float frame) {
    if (data.size() < 4)
        return;
    if (ReadMotionU16(data.data() + 2) != 0xffffU)
        data = data.subspan(4);
    if (data.size() < 10)
        return;
    const auto index = static_cast<std::int32_t>(frame);
    if (index > static_cast<std::int16_t>(ReadMotionU16(data.data())))
        return;
    TimedNodeMotion sample{};
    sample.data = data.data();
    sample.model = &model;
    SampleTimedNodes(sample, index, frame - static_cast<float>(index));
}

// @verified - p
void ClampTimedNodeMotionFrame(TimedNodeMotion &motion, const std::byte *data) {
    const float duration = static_cast<float>(static_cast<std::int16_t>(ReadMotionU16(data)));
    const float end = std::fmin(duration, std::fmax(0.0f, motion.end));
    const float begin = std::fmin(duration, std::fmax(0.0f, motion.begin));
    if (begin > motion.frame) {
        if (motion.flags & 4U) {
            motion.begin = begin;
            motion.end = end;
            motion.frame = std::fmax(begin, (motion.frame - begin) + end);
            return;
        } else {
            motion.frame = begin;
            motion.mode = 2;
        }
    } else if (motion.frame > end) {
        if (motion.flags & 4U) {
            motion.begin = begin;
            motion.end = end;
            motion.frame = std::fmin(end, (begin + motion.frame) - end);
            return;
        } else {
            motion.frame = end;
            motion.mode = 2;
        }
    }
    motion.begin = begin;
    motion.end = end;
}

// TickTimedNodeMotion: clamp or wrap before sampling, then advance.
bool TickTimedNodeMotion(TimedNodeMotion &motion) {
    if (!motion.mode)
        return false;
    const std::byte *const data = motion.data;
    if (ReadMotionU16(data + 2) != 0xffffU || (ReadMotionU16(data + 6) & 0xffdbU) || static_cast<std::uint16_t>(ReadMotionU16(data + 8) - 1) > 126U)
        return false;
    ClampTimedNodeMotionFrame(motion, data);
    const float duration = static_cast<float>(static_cast<std::int16_t>(ReadMotionU16(data)));
    const std::int32_t frame = static_cast<std::int32_t>(motion.frame);
    SampleTimedNodes(motion, frame, duration > frame ? motion.frame - frame : 0.0f);
    const float delta = HLib::GetScaledDeltaTick() * motion.speed;
    motion.frame += (motion.flags & 0x20U) ? -delta : delta;
    return motion.mode == 2;
}

bool ReadMotnFile(std::span<const std::byte> bytes, MotnFile &file) {
    file = {};
    if (bytes.size() < 0x1c)
        return false;
    std::int32_t sequenceTableOffset{}, sequenceNameOffset{}, sequenceDataOffset{};
#if 0
    if (!ReadSpanValue(bytes, 0, sequenceTableOffset) || !ReadSpanValue(bytes, 4, sequenceNameOffset) || !ReadSpanValue(bytes, 8, sequenceDataOffset) ||
        !ReadSpanValue(bytes, 0x14, file.animationIdBase) || !ReadSpanValue(bytes, 0x18, file.animationIdEnd) || sequenceTableOffset < 0 ||
        sequenceNameOffset < sequenceTableOffset || sequenceDataOffset < 0) {
        return false;
    }
#else
    shendk::MotnBankHeader header{};
    if (!ReadSpanValue(bytes, 0x14, file.animationIdBase) || !ReadSpanValue(bytes, 0x18, file.animationIdEnd) ||
        !shendk::ReadMotnBankHeader(bytes, header)) {
        return false;
    }
    sequenceTableOffset = std::bit_cast<std::int32_t>(header.sequenceTableOffset);
    sequenceNameOffset = std::bit_cast<std::int32_t>(header.sequenceNameOffset);
    sequenceDataOffset = std::bit_cast<std::int32_t>(header.sequenceDataOffset);
    if (sequenceTableOffset < 0 || sequenceNameOffset < sequenceTableOffset || sequenceDataOffset < 0) {
        return false;
    }
#endif
    const std::size_t table = static_cast<std::size_t>(sequenceTableOffset);
    const std::size_t names = static_cast<std::size_t>(sequenceNameOffset);
    const std::size_t data = static_cast<std::size_t>(sequenceDataOffset);
    if (table > bytes.size() || names > bytes.size() || data > bytes.size()) {
        return false;
    }
    const std::size_t capacity = (names - table) / 8;
    std::size_t sequenceCount = 0;
    for (; sequenceCount < capacity; ++sequenceCount) {
        std::int32_t nameOffset{};
        if (!ReadSpanValue(bytes, names + sequenceCount * 4, nameOffset) || nameOffset < 0 || static_cast<std::size_t>(nameOffset) >= bytes.size()) {
            break;
        }
        std::size_t length = 0;
        while (static_cast<std::size_t>(nameOffset) + length < bytes.size() && bytes[static_cast<std::size_t>(nameOffset) + length] != std::byte{0}) {
            const std::uint8_t value = std::to_integer<std::uint8_t>(bytes[static_cast<std::size_t>(nameOffset) + length]);
            if (++length > 95 || !IsMotnNameCharacter(value))
                break;
        }
        if (!length || length > 95 || static_cast<std::size_t>(nameOffset) + length >= bytes.size()) {
            break;
        }
    }

    const auto parseSequence = [&](std::size_t index, MotnSequence &sequence) -> bool {
        std::int32_t relativeOffset{}, metadataOffset{}, nameOffset{};
        if (!ReadSpanValue(bytes, table + index * 8, relativeOffset) || !ReadSpanValue(bytes, table + index * 8 + 4, metadataOffset) ||
            !ReadSpanValue(bytes, names + index * 4, nameOffset) || relativeOffset < 0 || metadataOffset < 0 || nameOffset < 0) {
            return false;
        }
        const std::uint64_t sequenceOffset64 = static_cast<std::uint64_t>(data) + static_cast<std::uint32_t>(relativeOffset);
        if (sequenceOffset64 > bytes.size() || bytes.size() - sequenceOffset64 < 8) {
            return false;
        }
        const std::size_t sequenceOffset = static_cast<std::size_t>(sequenceOffset64);
        std::uint16_t rawVersion{}, animationFlags{}, frameCount{}, keyDataOffset{};
        if (!ReadSpanValue(bytes, sequenceOffset, rawVersion) || !ReadSpanValue(bytes, sequenceOffset + 2, animationFlags) ||
            !ReadSpanValue(bytes, sequenceOffset + 4, frameCount) || !ReadSpanValue(bytes, sequenceOffset + 6, keyDataOffset)) {
            return false;
        }
        const float version = DecodeMotnFloat(rawVersion);
        if (!(version > 1.18f && version < 1.22f) || !frameCount || keyDataOffset < 8) {
            return false;
        }
        const bool overrideLayout = static_cast<std::uint8_t>(animationFlags) == 16;
        const std::size_t scalarCount = overrideLayout ? 96U : 69U;
        const bool wideCounts = (animationFlags & 0x100U) != 0;
        const std::size_t countWidth = wideCounts ? 2U : 1U;
        if ((keyDataOffset - 8U) / countWidth < scalarCount)
            return false;

        sequence.index = static_cast<std::uint32_t>(index);
        sequence.animationFlags = animationFlags;
        sequence.frameCount = frameCount;
        const char *const name = reinterpret_cast<const char *>(bytes.data() + static_cast<std::size_t>(nameOffset));
        sequence.name = name;
        if (!ReadMotnPlaybackCommands(bytes, metadataOffset, sequence)) {
            return false;
        }
        sequence.scalars.resize(scalarCount);
        std::size_t cursor = sequenceOffset + keyDataOffset;
        for (std::size_t scalar = 0; scalar < scalarCount; ++scalar) {
            std::uint16_t explicitCount{};
            if (wideCounts) {
                if (!ReadSpanValue(bytes, sequenceOffset + 8 + scalar * 2, explicitCount)) {
                    return false;
                }
            } else {
                explicitCount = std::to_integer<std::uint8_t>(bytes[sequenceOffset + 8 + scalar]);
            }
            MotnScalarChannel &channel = sequence.scalars[scalar];
            channel.keys.reserve(static_cast<std::size_t>(explicitCount) + 2);
            MotnKey first{};
            if (!ReadMotnTuple(bytes, cursor, first.value, first.leftSlope, first.rightSlope)) {
                return false;
            }
            channel.keys.push_back(first);
            MotnKey pending{};
            if (!ReadMotnTuple(bytes, cursor, pending.value, pending.leftSlope, pending.rightSlope)) {
                return false;
            }
            for (std::uint16_t key = 0; key < explicitCount; ++key) {
                if (!ReadSpanValue(bytes, cursor, pending.frame) || pending.frame < 0 || pending.frame >= frameCount) {
                    return false;
                }
                cursor += 2;
                if (!channel.keys.empty() && pending.frame < channel.keys.back().frame) {
                    return false;
                }
                channel.keys.push_back(pending);
                pending = {};
                if (!ReadMotnTuple(bytes, cursor, pending.value, pending.leftSlope, pending.rightSlope)) {
                    return false;
                }
            }
            pending.frame = static_cast<std::int16_t>(frameCount - 1);
            if (!channel.keys.empty() && pending.frame < channel.keys.back().frame) {
                return false;
            }
            channel.keys.push_back(pending);
        }
        // ReadMotnFile advances the fifth scalar group before publishing the
        // auxiliary pointers. The loop above has already consumed that group.
        if ((animationFlags & 0xc000U) != 0) {
            std::uint16_t firstCount{};
            if (!ReadSpanValue(bytes, cursor, firstCount))
                return false;
            if ((animationFlags & 0x4000U) != 0) {
                sequence.armTrack = bytes.data() + cursor;
                if ((animationFlags & 0x8000U) != 0) {
                    std::uint16_t secondCount{};
                    if (!ReadSpanValue(bytes, cursor + 2U + 4U * firstCount, secondCount))
                        return false;
                    cursor += 4U * (static_cast<std::size_t>(firstCount) + secondCount) + 4U;
                    if (cursor >= bytes.size())
                        return false;
                    sequence.regionTrack = bytes.data() + cursor;
                }
            } else {
                sequence.regionTrack = bytes.data() + cursor;
            }
        }
        if (sequence.regionTrack) {
            const std::size_t track = static_cast<std::size_t>(sequence.regionTrack - bytes.data());
            std::uint16_t count{};
            if (!ReadSpanValue(bytes, track, count) || 2ULL + count * 2ULL > bytes.size() - track)
                return false;
            for (std::uint32_t index = 0; index < count; ++index) {
                std::uint16_t offset{};
                ReadSpanValue(bytes, track + 2U + index * 2U, offset);
                const std::size_t descriptor = track + static_cast<std::size_t>(offset) * 2;
                std::uint16_t windows{};
                if (!ReadSpanValue(bytes, descriptor + 20U, windows) ||
                    sizeof(MotionPropDescriptor) + static_cast<std::uint64_t>(windows) * 2 > bytes.size() - descriptor)
                    return false;
            }
        }
        return true;
    };



    for (std::size_t index = 0; index < sequenceCount; ++index) {
        MotnSequence sequence{};
        if (parseSequence(index, sequence)) {
            file.sequences.push_back(std::move(sequence));
        }
    }
    return !file.sequences.empty();
}

const MotnSequence *FindMotnSequence(const MotnFile &file, std::uint32_t runtimeId) {
    for (const MotnSequence &sequence : file.sequences) {
        const std::uint32_t localId = sequence.index + 1;
        if (runtimeId == localId || (file.animationIdEnd > file.animationIdBase + 1 && runtimeId == file.animationIdBase + localId)) {
            return &sequence;
        }
    }
    return nullptr;
}

const MotnSequence *FindLoadedMotionSequence(std::uint32_t runtimeId) {
    for (const MotnFile &bank : authorMotionBanks) {
        if (const MotnSequence *sequence = FindMotnSequence(bank, runtimeId)) {
            return sequence;
        }
    }
    for (const MotionRegistryEntry *bank = g_motionData; bank; bank = bank->next) {
        if (runtimeId <= bank->firstMotion || runtimeId >= bank->lastMotion)
            continue;
        const std::size_t index = static_cast<std::size_t>(bank - g_motionRegistry.data());
        return FindMotnSequence(decodedMotionBanks[index], runtimeId);
    }
    return nullptr;
}

std::span<const std::byte> FindLoadedMotionData(std::uint32_t runtimeId) {
    return FindMotionBankPayload(runtimeId, false);
}

std::span<const std::byte> FindLoadedMotionMetadata(std::uint32_t runtimeId) {
    return FindMotionBankPayload(runtimeId, true);
}

std::span<const std::byte> FindLoadedMotionSecondaryEntry(std::uint32_t runtimeId, std::uint8_t requestedOpcode) {
    const auto metadata = FindLoadedMotionMetadata(runtimeId);
    std::size_t cursor = 0x0c;
    while (cursor < metadata.size()) {
        const auto opcode = std::to_integer<std::uint8_t>(metadata[cursor]);
        if (opcode >= sizeof(motnCommandWidths))
            return {};
        const auto width = motnCommandWidths[opcode];
        if (opcode == requestedOpcode) {
            const auto extent = opcode == 8 ? 4U : static_cast<unsigned>(width);
            if (extent > metadata.size() - cursor)
                return {};
            return metadata.subspan(cursor, extent);
        }
        if (!(opcode & 0xf7) || !width || width > metadata.size() - cursor)
            return {};
        cursor += width;
    }
    return {};
}

std::span<const std::byte> FindLoadedMotionTimedCommand(
    std::uint32_t runtimeId, std::uint8_t requestedOpcode, float beginFrame, float endFrame, std::uint32_t match) {
    const std::span<const std::byte> metadata = FindLoadedMotionMetadata(runtimeId);
    // motnGetAnimOfId returns metadata; command opcodes begin +0xc.
    std::size_t cursor = 0x0c;
    while (cursor < metadata.size()) {
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(metadata[cursor]);
        if (!opcode || opcode >= sizeof(motnCommandWidths))
            return {};
        if (opcode == 8) {
            if (metadata.size() - cursor < 4)
                return {};
            cursor += 4;
            std::uint32_t found{};
            while (metadata.size() - cursor >= 3) {
                std::uint16_t frame{};
                std::memcpy(&frame, metadata.data() + cursor, sizeof(frame));
                const std::uint8_t timedOpcode = std::to_integer<std::uint8_t>(metadata[cursor + 2]);
                if (!timedOpcode || timedOpcode >= sizeof(motnTimedCommandWidths))
                    return {};
                const std::size_t width = motnTimedCommandWidths[timedOpcode];
                if (!width || width > metadata.size() - cursor)
                    return {};
                if (timedOpcode == requestedOpcode && frame >= beginFrame && endFrame > frame && ++found == match)
                    return metadata.subspan(cursor, width);
                cursor += width;
            }
            return {};
        }
        const std::size_t width = motnCommandWidths[opcode];
        if (!width || width > metadata.size() - cursor)
            return {};
        cursor += width;
    }
    return {};
}

std::int32_t CaptureMotionSnapshot(shen_name name) {
    if (unfoState == kUnfoSlotCount)
        return -1;
    Character *const character = FindSnapshotCharacter(name);
    MotionWork *const work = FindMotionWork(character);
    if (!work)
        return -1;
    MotionSnapshot *snapshot{};
    for (std::uint32_t slot = 0; slot < kUnfoSlotCount; ++slot) {
        MotionSnapshot *const candidate = GetMotionSnapshot(slot);
        if (candidate && candidate->slot == -1) {
            snapshot = candidate;
            snapshot->slot = static_cast<std::int32_t>(slot);
            break;
        }
    }
    if (!snapshot)
        return -1;
    ++unfoState;
    snapshot->character = name;
    const MotionRuntimeState &state = work->runtime;
    snapshot->rootPosition = state.rootPosition;
    snapshot->entityRotation = state.entityRotation;
    snapshot->worldPosition = GetCharacterWorldPosition(character);
    snapshot->worldRotation = GetCharacterWorldRotation(character);
    snapshot->requestedRootMode = state.requestedRootMode;
    snapshot->motionFlags = state.motnFlags;
    snapshot->sequenceFlags = state.seqFlags;
    snapshot->environmentMotion = state.enableEnvironmentMotion;
    snapshot->projectionCotangents[0] = state.projectionCotangentNear;
    snapshot->projectionCotangents[1] = state.projectionCotangentMiddle;
    snapshot->projectionCotangents[2] = state.projectionCotangentFar;
    snapshot->callbackFlags = state.callbackFlags;
    const auto clocks = SnapshotClocks(*work);
    for (std::size_t region = 0; region < clocks.size(); ++region) {
        snapshot->motionIds[region] = state.currentMotionId[region];
        snapshot->defaultStepFrames[region] = state.defaultRegionStepFrames[region];
        snapshot->regionFlags[region] = state.regionFlags[region];
        snapshot->playbackSpeed[region] = clocks[region]->playbackSpeed;
        if ((state.regionFlags[region] & MRF_Transition) == 0 || state.currentRootMode == 2) {
            snapshot->splitFrames[region] = clocks[region]->current.splitTimeFrames;
            snapshot->ticksTime[region] = clocks[region]->current.ticksTime;
            snapshot->maxTime[region] = clocks[region]->current.maxTime;
        } else {
            snapshot->splitFrames[region] = state.layerLoopStartTime[region];
            snapshot->ticksTime[region] = state.pendingLayerStartFrame[region];
            snapshot->maxTime[region] = state.layerEndFrame[region];
        }
    }
    std::copy_n(state.constraintStepLimit, 9, snapshot->constraintStepLimit);
    return snapshot->slot;
}

namespace {

// @verified - p
std::uint32_t RestoreMotionWorkConfiguration(MotionWork &work, const MotionSnapshot &snapshot) {
    std::array<MotionRigClock *, 5> clocks{};
    mtwkCopyRigFlags(&work, &clocks[0], &clocks[1], &clocks[2], &clocks[3], &clocks[4]);
    MotionRuntimeState &state = work.runtime;
    state.requestedRootMode = snapshot.requestedRootMode;
    state.enableEnvironmentMotion = snapshot.environmentMotion;
    state.projectionCotangentNear = snapshot.projectionCotangents[0];
    state.projectionCotangentMiddle = snapshot.projectionCotangents[1];
    state.projectionCotangentFar = snapshot.projectionCotangents[2];
    state.callbackFlags = snapshot.callbackFlags;
    constexpr std::uint32_t motionMask = 0x18249;
    constexpr std::uint32_t sequenceMask = MSF_LimitConstraintStep | MSF_LinearInterpolation;
    state.motnFlags = (state.motnFlags & ~motionMask) | (snapshot.motionFlags & motionMask);
    state.seqFlags = (state.seqFlags & ~sequenceMask) | (snapshot.sequenceFlags & sequenceMask);
    for (std::size_t region = 0; region < clocks.size(); ++region) {
        state.defaultRegionStepFrames[region] = snapshot.defaultStepFrames[region];
        clocks[region]->playbackSpeed = snapshot.playbackSpeed[region];
    }
    for (std::size_t index = 0; index < 9; ++index)
        state.constraintStepLimit[index] = snapshot.constraintStepLimit[index];
    return std::bit_cast<std::uint32_t>(snapshot.constraintStepLimit[8]);
}

} // namespace

bool RestoreMotionSnapshot(shen_name name, std::uint32_t slot) {
    const MotionSnapshot *const snapshot = GetMotionSnapshot(slot);
    if (!snapshot || snapshot->slot == -1)
        return false;
    MotionWork *const work = FindMotionWork(FindSnapshotCharacter(name));
    if (!work)
        return false;
    RestoreMotionWorkConfiguration(*work, *snapshot);
    return true;
}

// @verified - p
bool RestoreMotionSnapshotPlacement(shen_name name, std::uint32_t slot) {
    const MotionSnapshot *const snapshot = GetMotionSnapshot(slot);
    if (!snapshot || snapshot->slot == -1)
        return false;
    Character *const character = FindSnapshotCharacter(name);
    if (!character)
        return false;
    MotionWork *const work = FindMotionWork(character);
    if (!work)
        return false;
    RestoreMotionWorkConfiguration(*work, *snapshot);
    shenApplyTranslationToAll(&character->entity.node, (snapshot->motionFlags & 0x200U) ? snapshot->worldPosition : snapshot->rootPosition);
    SetCharacterWorldRotation(character, snapshot->worldRotation);
    MotionRuntimeState &state = work->runtime;
    state.motnFlags = (state.motnFlags & 0xfffd9fffU) | 0x2000U;
    state.regionFlags[0] = (state.regionFlags[0] & ~(MRF_Reinitialize | MRF_FollowLowerBody | MRF_ReturnToLowerBody)) | 0x80U;
    state.queuedMotionId[0] = snapshot->motionIds[0];
    state.layerLoopStartTime[0] = snapshot->splitFrames[0];
    state.pendingLayerStartFrame[0] = snapshot->ticksTime[0];
    state.layerEndFrame[0] = snapshot->maxTime[0];
    if (snapshot->regionFlags[0] & MRF_Loop)
        state.regionFlags[0] |= MRF_Unknown1000;
    if (SetMotionRegionFlag(work, 70, 1, MotionRegion::LowerBody))
        state.regionFlags[0] |= MRF_LeadFoot;
    for (std::uint32_t region = 1; region < 5; ++region) {
        if (!(snapshot->regionFlags[region] & MRF_Independent))
            continue;
        state.regionFlags[region] = (state.regionFlags[region] & ~(MRF_Reinitialize | MRF_FollowLowerBody | MRF_ReturnToLowerBody)) | 0x80U;
        state.queuedMotionId[region] = snapshot->motionIds[region];
        state.layerLoopStartTime[region] = snapshot->splitFrames[region];
        state.pendingLayerStartFrame[region] = snapshot->ticksTime[region];
        state.layerEndFrame[region] = snapshot->maxTime[region];
        if (snapshot->regionFlags[region] & MRF_Loop)
            state.regionFlags[region] |= MRF_Unknown1000;
        if (SetMotionRegionFlag(work, 70, 1, static_cast<MotionRegion>(region)))
            state.regionFlags[region] |= MRF_LeadFoot;
    }
    return true;
}

// @verified - p
bool ReleaseMotionSnapshot(std::uint32_t slot) {
    if (slot > kUnfoSlotCount || !g_unfoSpace)
        return false;

    std::byte *const bytes = g_unfoSpace + slot * kUnfoSlotSize;
    std::int32_t state;
    std::memcpy(&state, bytes, sizeof(state));
    if (state != -1) {
        std::memset(bytes, 0, kUnfoSlotSize);
        --unfoState;
        state = -1;
        std::memcpy(bytes, &state, sizeof(state));
        const shen_name blank = MOTION_SNAPSHOT_EMPTY_CHARACTER;
        std::memcpy(bytes + 4, &blank, sizeof(blank));
    }
    return true;
}


std::int32_t LoadMoti(std::span<const std::byte> bytes) {
    if (g_motionBankCount <= 0)
        return -1;
    MotionRegistryEntry *tail = &g_motionRegistry[g_motionBankCount - 1];
    while (tail->next)
        tail = tail->next;
    std::size_t index = static_cast<std::size_t>(g_motionBankCount);
    while (index < g_motionRegistry.size() && g_motionRegistry[index].motionId != -1)
        ++index;
    if (index == g_motionRegistry.size())
        return -1;
    auto *const data = const_cast<std::byte *>(bytes.data());
    const std::uint64_t size = bytes.size();
    std::uint32_t enabled{}, count{};
    std::uint16_t first{}, last{};
    if (!ReadValue(data, size, 0x1c, enabled) || !enabled || !ReadValue(data, size, 0x10, count) || !ReadValue(data, size, 0x14, first) ||
        !ReadValue(data, size, 0x18, last)) {
        return -1;
    }


    if (!ReadMotnFile({data, static_cast<std::size_t>(size)}, decodedMotionBanks[index])) {
        decodedMotionBanks[index] = {};
    }
    MotionRegistryEntry &bank = g_motionRegistry[index];
    bank = {};
    bank.motionId = tail->motionId + 1;
    bank.data = data;
    bank.firstMotion = first;
    bank.lastMotion = last;
    bank.previous = tail;
    bank.motionCount = count;
    tail->next = &bank;
    motionBankSizes[index] = size;
    g_totalMotionCount += count;
    return bank.motionId;
}

std::int32_t LoadMOTNPCTable(const char *directory, const char *filename) {
    std::uint64_t size{};
    std::byte *const data = ReadMotionFile(directory, filename, SHEN_NAME(MOTI), size);
    if (!data)
        return -1;
    const std::int32_t handle = LoadMoti({data, static_cast<std::size_t>(size)});
    if (handle < 0)
        HLib::FreeTaskData(data);
    return handle;
}

std::int32_t ReadMotiFromDirectory(std::int32_t act, shen_name map, const char *filename) {
    char directory[64]{};
    const int length =
        std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", act, map.characters[0], map.characters[1], map.characters[2], map.characters[3]);
    if (length < 0 || static_cast<std::size_t>(length) >= sizeof(directory))
        return -1;
    return LoadMOTNPCTable(directory, filename);
}

// @verified - p
std::uint32_t GetMotionFrameCount(std::uint16_t animation) {
    if (animation == 0)
        return 0;
    const std::span<const std::byte> data = FindLoadedMotionData(animation);
    return data.empty() ? 0U : ReadMotionU16(data.data() + 4);
}

namespace {
bool DeleteMotionInternal(std::int32_t handle, bool releaseStorage) {
    if (g_motionBankCount <= 0 || handle < g_motionBankCount)
        return false;
    MotionRegistryEntry *bank = &g_motionRegistry[g_motionBankCount - 1];
    while (bank && bank->motionId != handle)
        bank = bank->next;
    if (!bank)
        return false;
    const std::size_t index = static_cast<std::size_t>(bank - g_motionRegistry.data());
    RetireMotionBankTracks(bank->data, motionBankSizes[index]);
    std::memset(bank->data, 0, bank->motionCount);
    if (releaseStorage)
        HLib::FreeTaskData(bank->data);
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    g_totalMotionCount -= bank->motionCount;
    RetireMotionRange(bank->firstMotion, bank->lastMotion);
    decodedMotionBanks[index] = {};
    motionBankSizes[index] = 0;
    *bank = {};
    bank->motionId = -1;
    return true;
}
} // namespace

bool DeleteMotion(std::int32_t handle) {
    return DeleteMotionInternal(handle, true);
}

bool DeleteBorrowedMotion(std::int32_t handle) {
    return DeleteMotionInternal(handle, false);
}

void RegisterAuthorMotionBanks(std::span<const MotnFile> banks) {
    for (const MotnFile &previous : authorMotionBanks) {
        bool retained = false;
        for (const MotnFile &current : banks) {
            if (&previous == &current) {
                retained = true;
                break;
            }
        }
        if (retained || previous.sequences.empty())
            continue;
        std::uint32_t last = 0;
        for (const MotnSequence &sequence : previous.sequences) {
            last = std::max(last, sequence.index + 1);
            ForEachLiveCharacterInMapOrder(
                [](Character *character, void *opaque) {
                    const auto &sequence = *static_cast<const MotnSequence *>(opaque);
                    MotionWork *const work = FindMotionWork(character);
                    if (!work)
                        return;
                    for (std::uint32_t region = 0; region < 5; ++region) {
                        for (void **track : {&work->runtime.regionTrack[region], &work->runtime.previousRegionTrack[region]}) {
                            if (!*track || *track != sequence.regionTrack)
                                continue;
                            RetireMotionPropTrack(*track, region);
                            *track = nullptr;
                        }
                    }
                    if (work->runtime.rightArmTrack == sequence.armTrack)
                        work->runtime.rightArmTrack = nullptr;
                    if (work->runtime.leftArmTrack == sequence.armTrack)
                        work->runtime.leftArmTrack = nullptr;
                },
                const_cast<MotnSequence *>(&sequence));
        }
        const std::uint32_t base = previous.animationIdEnd > previous.animationIdBase + 1 ? previous.animationIdBase : 0;
        RetireMotionRange(static_cast<std::uint16_t>(base + 1), static_cast<std::uint16_t>(base + last));
    }
    while (d3t::ProcessObjectStore(true, -1)) {
    }
    authorMotionBanks = banks;
}

bool IsRetiredMotionId(std::uint16_t motionId) {
    for (const RetiredMotionRange &range : retiredMotionRanges) {
        if (!range.end)
            break;
        if (motionId >= range.begin && motionId <= range.end)
            return true;
    }
    return false;
}

// Queue_mot_Callback_MotFileManageTaskEntry.
// @verified - p
void QueueMotionRetirementTask(std::uint8_t nextFunctionIndex) {
    motionRetirementTask = HLib::EnqueueTaskWithParameter(MotFileManageTaskEntry_Callback, nextFunctionIndex, 2, 4, SHEN_NAME(MOTF));
}

void FreeMotionRetirementTask() {
    retiredMotionRanges = {};
    if (!motionRetirementTask)
        return;
    HLib::DestroyTask(motionRetirementTask);
    motionRetirementTask = nullptr;
}

float SampleMotnScalar(const MotnScalarChannel &channel, float time) {
    if (channel.keys.empty())
        return 0.0f;
    const float frame = time * 30.0f;
    if (frame <= channel.keys.front().frame)
        return channel.keys.front().value;
    const MotnKey *left = &channel.keys.front();
    for (std::size_t index = 1; index < channel.keys.size(); ++index) {
        const MotnKey &right = channel.keys[index];
        if (frame >= right.frame) {
            left = &right;
            continue;
        }
        const float spanFrames = static_cast<float>(right.frame - left->frame);
        if (std::fabs(spanFrames) <= 0.000001f)
            return right.value;
        const float elapsedSeconds = (frame - left->frame) / 30.0f;
        const float spanSeconds = spanFrames / 30.0f;
        const float amount = elapsedSeconds / spanSeconds;
        const float square = amount * amount;
        const float cube = square * amount;
        return (2.0f * cube - 3.0f * square + 1.0f) * left->value + (-2.0f * cube + 3.0f * square) * right.value +
            (cube - 2.0f * square + amount) * spanSeconds * left->rightSlope + (cube - square) * spanSeconds * right.leftSlope;
    }
    return channel.keys.back().value;
}

void SampleMotnScalarPVA(const MotnScalarChannel &channel, float time, float &value, float &incomingHalfTangent, float &outgoingHalfTangent, bool linear) {
    value = 0.0f;
    incomingHalfTangent = 0.0f;
    outgoingHalfTangent = 0.0f;
    if (channel.keys.empty())
        return;
    const MotnKey *left = &channel.keys.front();
    value = left->value;
    float leftTime = static_cast<float>(left->frame) * 0.033333335f;
    if (time < leftTime)
        return;
    for (std::size_t index = 1; index < channel.keys.size(); ++index) {
        const MotnKey &right = channel.keys[index];
        const float rightTime = static_cast<float>(right.frame) * 0.033333335f;
        if (time == rightTime) {
            value = right.value;
            incomingHalfTangent = right.leftSlope * 0.5f;
            outgoingHalfTangent = right.rightSlope * 0.5f;
            return;
        }
        if (time > rightTime) {
            left = &right;
            leftTime = rightTime;
            value = right.value;
            continue;
        }

        const float inverseDuration = 1.0f / (rightTime - leftTime);
        const float elapsed = time - leftTime;
        float tangent{};
        if (linear) {
            const float slope = (right.value - left->value) * inverseDuration;
            value = elapsed * slope + left->value;
            tangent = slope * 0.5f;
        } else {
            const float inverseSquare = inverseDuration * inverseDuration;
            const float square = elapsed * elapsed;
            const float cubeTerm = (square * elapsed) * inverseSquare;
            const float tripleSquare = (square * 3.0f) * inverseSquare;
            const float sixSquare = ((square * 6.0f) * inverseSquare) * inverseDuration;
            const float sixElapsed = (elapsed * 6.0f) * inverseSquare;
            const float twiceElapsed = (elapsed + elapsed) * inverseDuration;
            tangent = (((sixElapsed - sixSquare) * right.value + (sixSquare - sixElapsed) * left->value) +
                          ((tripleSquare - twiceElapsed - twiceElapsed) + 1.0f) * left->rightSlope + (tripleSquare - twiceElapsed) * right.leftSlope) *
                0.5f;
            const float twiceCube = (cubeTerm + cubeTerm) * inverseDuration;
            const float squareTerm = square * inverseDuration;
            value = ((twiceCube - tripleSquare + 1.0f) * left->value + (tripleSquare - twiceCube) * right.value) +
                ((cubeTerm - squareTerm - squareTerm) + elapsed) * left->rightSlope + (cubeTerm - squareTerm) * right.leftSlope;
        }
        incomingHalfTangent = tangent;
        outgoingHalfTangent = tangent;
        return;
    }
}

// AllocateMotionBanks: CTRL's map entry drops every bank above the resident MOTION/BATTLE
// banks and the five loadable prop banks, clears the retired ranges, re-enables the
// environment gate, recounts the resident motions and resets the search-hand owner.
// @verified - p
std::int64_t AllocateMotionBanks() {
    InitializeUNFO();
    const std::int32_t resident = g_motionBankCount;
    for (std::size_t index = static_cast<std::size_t>(resident); index < g_motionRegistry.size(); ++index) {
        MotionRegistryEntry &bank = g_motionRegistry[index];
        bank.motionId = -1;
        bank.data = nullptr;
        bank.firstMotion = 0;
        bank.lastMotion = 0;
        bank.previous = nullptr;
        bank.next = nullptr;
        bank.motionCount = 0;
        decodedMotionBanks[index] = {};
        motionBankSizes[index] = 0;
    }
    for (std::size_t slot = 1; slot < motionPropBanks.size(); ++slot) {
        MotionPropBank &bank = motionPropBanks[slot];
        bank.handle = -1;
        bank.data = nullptr;
        bank.previous = nullptr;
        bank.next = nullptr;
        for (MT7Model &model : motionPropModels[slot])
            ReleaseModelFormat7(model);
        motionPropModels[slot].clear();
    }
    retiredMotionRanges = {};
    g_usingEnv = 1;
    g_totalMotionCount = 0;
    g_motionRegistry[static_cast<std::size_t>(resident - 1)].next = nullptr;
    for (const MotionRegistryEntry *bank = g_motionData; bank; bank = bank->next)
        g_totalMotionCount += bank->motionCount;
    ResetSearchHandState();
    return mtwkSetOverridesEnabled(1);
}

void AllocateARMM() {
    g_armMotionMemory = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(0x8800, SHEN_NAME(ARMM)));
    HLib::SetTaskDataMotionFlag(g_armMotionMemory);
}

std::byte *AllocateAndLoadMOTFiles() {
    while (motionPropBanks[0].next)
        DeleteMotionProps(motionPropBanks[0].next->handle);
    AllocateAndSetUNFOFlag();
    InitializeUNFO();

    for (MotionRegistryEntry &entry : g_motionRegistry) {
        entry = {};
        entry.motionId = -1;
    }
    motionBankSizes = {};
    decodedMotionBanks = {};
    g_motionData = &g_motionRegistry[0];
    g_motionData->motionId = 0;
    g_battleMotionData = &g_motionRegistry[1];
    g_fallbackMotionTable = nullptr;
    g_fallbackMotionSections = {};
    g_motionBankCount = 0;
    g_totalMotionCount = 0;
    authorMotionBanks = {};
    retiredMotionRanges = {};

    std::uint64_t mainSize = 0;
    g_motionData->data = ReadMiscFile("MOTION.MOT", SHEN_NAME(MOTI), mainSize);
    motionBankSizes[0] = mainSize;
    if (!g_motionData->data || !BindMotionHeader(*g_motionData, mainSize)) {
        return nullptr;
    }
    if (!ReadMotnFile(std::span<const std::byte>{g_motionData->data, static_cast<std::size_t>(mainSize)}, decodedMotionBanks[0])) {
        return nullptr;
    }
    g_totalMotionCount = g_motionData->motionCount;

    std::uint64_t battleSize = 0;
    g_battleMotionData->data = ReadMiscFile("BATTLE.MOT", SHEN_NAME(MOTI), battleSize);
    motionBankSizes[1] = battleSize;

    std::uint64_t fallbackSize = 0;
    g_fallbackMotionTable = ReadMiscFile("FBMOTTBL.BIN", SHEN_NAME(FBMT), fallbackSize);
    if (!g_fallbackMotionTable || !BindFallbackMotionTable(fallbackSize)) {
        return nullptr;
    }

    if (!g_battleMotionData->data || !BindMotionHeader(*g_battleMotionData, battleSize)) {
        g_motionBankCount = 1;
        return g_fallbackMotionTable;
    }

    g_motionBankCount = 2;
    g_motionData->next = g_battleMotionData;
    g_battleMotionData->motionId = g_motionData->motionId + 1;
    g_battleMotionData->previous = g_motionData;
    if (!ReadMotnFile({g_battleMotionData->data, static_cast<std::size_t>(battleSize)}, decodedMotionBanks[1])) {
        decodedMotionBanks[1] = {};
    }
    g_totalMotionCount += g_battleMotionData->motionCount;
    return g_fallbackMotionTable;
}

} // namespace shenmue::s2
