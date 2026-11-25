#include "s2/motion_registry_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/model_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/hand_dc.hpp"
#include <cstring>

#define SHENMUE_MOTION_BANK_ARRAY(type, name, count) type name[count]
#define SHENMUE_MOTION_BANK_INITIALIZER =
#define SHENMUE_MOTION_BANK_NULL 0
#define SHENMUE_MOTION_BANK_DATA(name) name
#define SHENMUE_MOTION_BANK_COUNT(name) (sizeof(name) / sizeof(name[0]))
#define SHENMUE_MOTION_BANK_CONST(type, value) static_cast<type *>(value)
namespace shenmue {
namespace s2 {

std::uint32_t *AllocateStaticMotionFileData(std::uint32_t size);

SHENMUE_MOTION_BANK_ARRAY(MotionRegistryEntry, g_motionRegistry, 32) SHENMUE_MOTION_BANK_INITIALIZER{};
MotionRegistryEntry *g_motionData = SHENMUE_MOTION_BANK_NULL;
MotionRegistryEntry *g_battleMotionData = SHENMUE_MOTION_BANK_NULL;
SHENMUE_MOTION_BANK_ARRAY(MotionPropBank, g_motionPropBanks, kMotionPropBankCount) SHENMUE_MOTION_BANK_INITIALIZER{};
std::int8_t g_motionBankCount = 0;
std::uint32_t g_registeredMotionBytes = 0;
std::byte *g_fallbackMotionTable = SHENMUE_MOTION_BANK_NULL;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t *g_fallbackMotionSection3 = SHENMUE_MOTION_BANK_NULL, *g_fallbackMotionSection1 = SHENMUE_MOTION_BANK_NULL;
std::int32_t *g_fallbackMotionSection2 = SHENMUE_MOTION_BANK_NULL, *g_fallbackMotionSection0 = SHENMUE_MOTION_BANK_NULL;
#else
SHENMUE_MOTION_BANK_ARRAY(std::int32_t *, g_fallbackMotionSections, 4) SHENMUE_MOTION_BANK_INITIALIZER{};
#endif
MotionSnapshot *g_motionSnapshots = SHENMUE_MOTION_BANK_NULL;
std::uint32_t g_motionSnapshotState = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t *AllocateMotionFileData(std::uint32_t size) {
    std::uint32_t *const data = static_cast<std::uint32_t *>(HLib::AllocateTaskData(size, MOTION_SNAPSHOT_STORAGE));
    std::uint32_t *word = data;
    for (std::uint32_t index = 0; index < (size >> 2U); ++index)
        *word++ = 0;
    return data;
}

// @verified - d
std::int32_t MOTN_GetFootstepAttributes(std::uint16_t animationId, std::uint8_t *attributes) {
    const std::byte *record = MOTN_FindAnimationExtraRecord(animationId, 20);
    if (!record)
        return -1;
    ++record;
    attributes[0] = static_cast<std::uint8_t>(*record++);
    attributes[1] = static_cast<std::uint8_t>(*record++);
    attributes[2] = static_cast<std::uint8_t>(*record);
    return 0;
}

// @verified - d
const std::byte *MOTN_FindFrameEvent(std::int8_t eventType, std::uint16_t animationId, float firstFrame, float lastFrame, std::int32_t occurrence) {
    std::int32_t matched = 0;
    const std::byte *record = MOTN_GetMarkerData(animationId);
    if (!record)
        return SHENMUE_MOTION_BANK_NULL;
    record += 12;
    while (static_cast<std::uint8_t>(*record) != 8U) {
        if (!static_cast<std::uint8_t>(*record))
            return SHENMUE_MOTION_BANK_NULL;
        record += motionCommandWidths[static_cast<std::uint8_t>(*record)];
    }
    record += 4;
    while (static_cast<std::uint8_t>(record[2])) {
        if (static_cast<std::uint8_t>(record[2]) == eventType) {
            const float frame = static_cast<float>(static_cast<std::uint8_t>(record[0])) + static_cast<float>(static_cast<std::uint8_t>(record[1])) * 256.0f;
            if (!(firstFrame > frame) && lastFrame > frame && ++matched == occurrence)
                return record;
        }
        record += motionFrameCommandWidths[static_cast<std::uint8_t>(record[2])];
    }
    return SHENMUE_MOTION_BANK_NULL;
}
#endif

// @verified - d
MotionWork *scenFindMotionwork(shen_name name) {
    const MapControlData *const control = SHENMUE_MOTION_BANK_CONST(MapControlData, HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character)
        return static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    return SHENMUE_MOTION_BANK_NULL;
}

// @verified - d
const std::byte *MOTN_FindAnimationExtraRecord(std::uint16_t animationId, std::uint32_t tag) {
    const std::byte *record = MOTN_GetMarkerData(animationId);
    if (!record)
        return SHENMUE_MOTION_BANK_NULL;
    record += 12;
    while (static_cast<std::uint8_t>(*record) != tag) {
        const std::uint8_t type = static_cast<std::uint8_t>(*record);
        if (!type || type == 8)
            return SHENMUE_MOTION_BANK_NULL;
        record += motionCommandWidths[static_cast<std::uint8_t>(*record)];
    }
    return record;
}

// @verified - d
std::uint32_t MOTN_GetLeadFootFlag(std::uint16_t animationId) {
    if (MOTN_FindAnimationExtraRecord(animationId, 19))
        return 0x20U;
    return 0U;
}

// @verified - d
std::uint16_t MOTN_GetLoopFrame(std::uint16_t animation) {
    const std::byte *record = MOTN_FindAnimationExtraRecord(animation, 1);
    if (!record)
        return 0;
    record += 2;
    return *reinterpret_cast<const std::uint16_t *>(record);
}

// @verified - d
std::int32_t GetMotionFrameCount(std::uint16_t animation) {
    if (!animation)
        return 0;
    const MotnAnimationHeader *const data = MOTN_GetDataForAnimId(animation);
    if (!data)
        return 0;
    return data->frameCount;
}

SHENMUE_MOTION_BANK_ARRAY(RetiredMotionRange, g_retiredMotionRanges, 32) SHENMUE_MOTION_BANK_INITIALIZER{};
MotionRegistryEntry *FindFreeMotionBankEntry();

// @verified - d
std::uint32_t RegisterMotionBank(void *data) {
    const MotionBankHeader *const header = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, data);
    if (!header->enabled)
        return UINT32_MAX;
    MotionRegistryEntry *tail = &g_motionRegistry[static_cast<std::uint8_t>(g_motionBankCount) - 1];
    while (tail->next)
        tail = tail->next;
    MotionRegistryEntry *const entry = FindFreeMotionBankEntry();
    tail->next = entry;
    entry->handle = tail->handle + 1;
    entry->previous = tail;
    entry->next = SHENMUE_MOTION_BANK_NULL;
    entry->data = data;
    entry->firstAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, entry->data)->firstAnimation;
    entry->endAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, entry->data)->registeredEndAnimation;
    entry->dataSize = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, entry->data)->fileSize;
    g_registeredMotionBytes += entry->dataSize;
    return entry->handle;
}
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetMotionBankEntry(MotionRegistryEntry *entry) {
    std::size_t index = static_cast<std::uint8_t>(g_motionBankCount);
    while (index < SHENMUE_MOTION_BANK_COUNT(g_motionRegistry) && entry != &g_motionRegistry[index])
        ++index;
    if (index != SHENMUE_MOTION_BANK_COUNT(g_motionRegistry)) {
        MotionRegistryEntry &bank = g_motionRegistry[index];
        bank.handle = UINT32_MAX;
        bank.data = SHENMUE_MOTION_BANK_NULL;
        bank.firstAnimation = 0;
        bank.endAnimation = 0;
        bank.previous = SHENMUE_MOTION_BANK_NULL;
        bank.next = SHENMUE_MOTION_BANK_NULL;
        bank.dataSize = 0;
    }
}

// @verified - d
std::intptr_t FreeMotionFileData(void *data) {
    return HLib::FreeTaskData(data);
}
#endif

// @verified - d
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t ReleaseMotionBank(std::uint32_t handle) {
#else
void ReleaseMotionBank(std::uint32_t handle) {
#endif
    if (static_cast<std::uint8_t>(g_motionBankCount) > static_cast<std::int32_t>(handle))
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        return 0;
#else
        return;
#endif
    MotionRegistryEntry *entry = &g_motionRegistry[static_cast<std::uint8_t>(g_motionBankCount) - 1];
    while (entry->handle != handle) {
        if (!entry->next)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            return 0;
#else
            return;
#endif
        entry = entry->next;
    }
    std::memset(entry->data, 0, entry->dataSize);
    if (entry->next)
        entry->next->previous = entry->previous;
    if (entry->previous)
        entry->previous->next = entry->next;
    g_registeredMotionBytes -= entry->dataSize;
    RetiredMotionRange *range = SHENMUE_MOTION_BANK_DATA(g_retiredMotionRanges);
    while (range->endAnimation)
        ++range;
    range->endAnimation = entry->endAnimation;
    range->firstAnimation = entry->firstAnimation;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    ResetMotionBankEntry(entry);
    return 1;
#else
    std::size_t index = static_cast<std::uint8_t>(g_motionBankCount);
    while (index < SHENMUE_MOTION_BANK_COUNT(g_motionRegistry) && entry != &g_motionRegistry[index])
        ++index;
    if (index != SHENMUE_MOTION_BANK_COUNT(g_motionRegistry)) {
        entry->handle = UINT32_MAX;
        entry->data = SHENMUE_MOTION_BANK_NULL;
        entry->firstAnimation = 0;
        entry->endAnimation = 0;
        entry->previous = SHENMUE_MOTION_BANK_NULL;
        entry->next = SHENMUE_MOTION_BANK_NULL;
        entry->dataSize = 0;
    }
#endif
}
// @verified - d
bool IsMotionAnimationValid(std::uint16_t animation) {
    const RetiredMotionRange *range = SHENMUE_MOTION_BANK_DATA(g_retiredMotionRanges);
    while (range->endAnimation) {
        if (animation <= range->endAnimation && animation >= range->firstAnimation)
            return false;
        ++range;
    }
    return true;
}
// @verified - d
void ClearRetiredMotionRanges() {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(SHENMUE_MOTION_BANK_COUNT(g_retiredMotionRanges)); ++index) {
        g_retiredMotionRanges[index].endAnimation = 0;
        g_retiredMotionRanges[index].firstAnimation = 0;
    }
}

// @verified - d
std::uint32_t RegisterMotionPropBank(void *data) {
    if (!data)
        return UINT32_MAX;
    MotionPropBank *tail = &g_motionPropBanks[0];
    while (tail->next)
        tail = tail->next;
    std::int32_t index = 1;
    if (g_motionPropBanks[1].handle != -1) {
        do
            ++index;
        while (index < static_cast<std::int32_t>(SHENMUE_MOTION_BANK_COUNT(g_motionPropBanks)) && g_motionPropBanks[index].handle != -1);
    }
    if (index == 0x20)
        return UINT32_MAX;
    tail->next = &g_motionPropBanks[index];
    MotionPropBank *const bank = &g_motionPropBanks[index];
    g_motionPropBanks[index].handle = tail->handle + 1;
    bank->previous = tail;
    bank->next = SHENMUE_MOTION_BANK_NULL;
    bank->data = static_cast<std::byte *>(data);
    const std::int32_t count = SHENMUE_MOTION_BANK_CONST(MotionPropHeader, data)->count;
    MotionPropEntry *const entries = reinterpret_cast<MotionPropEntry *>(static_cast<std::byte *>(data) + sizeof(MotionPropHeader));
    for (index = 0; index < count; ++index) {
        MT7Model *const model = InitializeMT7Model(static_cast<std::byte *>(data) + entries[index].model);
        entries[index].model = reinterpret_cast<std::uintptr_t>(model->root);
    }
    return static_cast<std::uint32_t>(bank->handle);
}

// @verified - d
MT7Node *FindMotionProp(std::uint16_t id, std::int32_t handle) {
    const MotionPropBank *bank;
    std::int32_t count, index;
    const MotionPropEntry *entries;
    if (!g_motionPropBanks[0].next)
        return 0;
    if (handle != -1) {
        for (bank = &g_motionPropBanks[0]; bank; bank = bank->next) {
            if (bank->data && bank->handle == handle) {
                count = reinterpret_cast<const MotionPropHeader *>(bank->data)->count;
                entries = reinterpret_cast<const MotionPropEntry *>(reinterpret_cast<const MotionPropHeader *>(bank->data) + 1);
                for (index = 0; index < count; ++index) {
                    if (entries[index].id == id)
                        return reinterpret_cast<MT7Node *>(entries[index].model);
                }
                break;
            }
        }
    }
    for (bank = &g_motionPropBanks[0]; bank; bank = bank->next) {
        if (bank->data) {
            count = reinterpret_cast<const MotionPropHeader *>(bank->data)->count;
            entries = reinterpret_cast<const MotionPropEntry *>(reinterpret_cast<const MotionPropHeader *>(bank->data) + 1);
            for (index = 0; index < count; ++index) {
                if (entries[index].id == id)
                    return reinterpret_cast<MT7Node *>(entries[index].model);
            }
        }
    }
    return 0;
}

// @verified - d
std::int32_t ReleaseMotionPropBank(std::uint32_t handle) {
    if (static_cast<std::int32_t>(handle) < 1)
        return 0;
    MotionPropBank *bank = &g_motionPropBanks[0];
    while (static_cast<std::uint32_t>(bank->handle) != handle) {
        if (!bank->next)
            return 0;
        bank = bank->next;
    }
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    bank->handle = -1;
    bank->data = SHENMUE_MOTION_BANK_NULL;
    bank->previous = SHENMUE_MOTION_BANK_NULL;
    bank->next = SHENMUE_MOTION_BANK_NULL;
    return 1;
}

// @verified - d
void AllocateAndSetUNFOFlag() {
    g_motionSnapshots = reinterpret_cast<MotionSnapshot *>(AllocateStaticMotionFileData(kMotionSnapshotBytes));
    HLib::SetTaskDataMotionFlag(g_motionSnapshots);
}

// @verified - d
std::uint32_t InitializeUNFO() {
    const std::uint32_t previous = g_motionSnapshotState;
    g_motionSnapshotState = 0;
    std::memset(g_motionSnapshots, 0, kMotionSnapshotBytes);
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kMotionSnapshotSlots); ++index) {
        g_motionSnapshots[index].slot = -1;
        g_motionSnapshots[index].character = EMPTY_MOTION_SNAPSHOT_CHARACTER;
    }
    return previous;
}

shen_name GetMotionSnapshotCharacter(std::uint32_t index) {
    if (index > 16)
        return SHEN_NAME4('!', '?', '?', '?');
    return g_motionSnapshots[index].character;
}

// @verified - d
void AllocateMotionBanks(std::int32_t, std::uint32_t) {
    InitializeUNFO();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t index;
    for (index = static_cast<std::uint8_t>(g_motionBankCount); index < 32; ++index) {
        g_motionRegistry[index].handle = UINT32_MAX;
        g_motionRegistry[index].data = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].firstAnimation = 0;
        g_motionRegistry[index].endAnimation = 0;
        g_motionRegistry[index].previous = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].next = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].dataSize = 0;
    }
    for (index = 1; index < 4; ++index) {
        g_motionPropBanks[index].handle = -1;
        g_motionPropBanks[index].data = SHENMUE_MOTION_BANK_NULL;
        g_motionPropBanks[index].previous = SHENMUE_MOTION_BANK_NULL;
        g_motionPropBanks[index].next = SHENMUE_MOTION_BANK_NULL;
    }
    for (index = 0; index < 32; ++index) {
        g_retiredMotionRanges[index].endAnimation = 0;
        g_retiredMotionRanges[index].firstAnimation = 0;
    }
    g_motionRegistry[static_cast<std::uint8_t>(g_motionBankCount) - 1].next = SHENMUE_MOTION_BANK_NULL;
#else
    for (std::size_t index = static_cast<std::uint8_t>(g_motionBankCount); index < SHENMUE_MOTION_BANK_COUNT(g_motionRegistry); ++index) {
        MotionRegistryEntry &bank = g_motionRegistry[index];
        bank.handle = UINT32_MAX;
        bank.data = SHENMUE_MOTION_BANK_NULL;
        bank.firstAnimation = 0;
        bank.endAnimation = 0;
        bank.previous = SHENMUE_MOTION_BANK_NULL;
        bank.next = SHENMUE_MOTION_BANK_NULL;
        bank.dataSize = 0;
    }
    for (std::size_t index = 1; index < SHENMUE_MOTION_BANK_COUNT(g_motionPropBanks); ++index) {
        MotionPropBank &bank = g_motionPropBanks[index];
        bank.handle = -1;
        bank.data = SHENMUE_MOTION_BANK_NULL;
        bank.previous = SHENMUE_MOTION_BANK_NULL;
        bank.next = SHENMUE_MOTION_BANK_NULL;
    }
    ClearRetiredMotionRanges();
    g_motionRegistry[g_motionBankCount - 1].next = SHENMUE_MOTION_BANK_NULL;
#endif
    g_motionUseEnvironment = 1;
    g_registeredMotionBytes = 0;
    for (const MotionRegistryEntry *bank = g_motionData; bank; bank = bank->next)
        g_registeredMotionBytes += bank->dataSize;
    ResetSearchHandState();
#if defined(SHENMUE_TARGET_S2_DC_JP_1001)
    mtwkSetOverridesEnabled(1);
#endif
}

// @verified - d
void BindFallbackMotionTable(std::byte *data) {
    g_fallbackMotionTable = data;
    const std::int32_t *const offsets = reinterpret_cast<const std::int32_t *>(data);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    g_fallbackMotionSection0 = reinterpret_cast<std::int32_t *>(data + offsets[0]);
    g_fallbackMotionSection1 = reinterpret_cast<std::int32_t *>(data + offsets[1]);
    g_fallbackMotionSection2 = reinterpret_cast<std::int32_t *>(data + offsets[2]);
    g_fallbackMotionSection3 = reinterpret_cast<std::int32_t *>(data + offsets[3]);
#else
    g_fallbackMotionSections[0] = reinterpret_cast<std::int32_t *>(data + offsets[0]);
    g_fallbackMotionSections[1] = reinterpret_cast<std::int32_t *>(data + offsets[1]);
    g_fallbackMotionSections[2] = reinterpret_cast<std::int32_t *>(data + offsets[2]);
    g_fallbackMotionSections[3] = reinterpret_cast<std::int32_t *>(data + offsets[3]);
#endif
}

std::byte *GetFallbackMotionTable() {
    return g_fallbackMotionTable;
}

// @verified - d
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateAndLoadMOTFiles() {
#else
std::byte *AllocateAndLoadMOTFiles() {
#endif
    AllocateAndSetUNFOFlag();
    InitializeUNFO();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t index;
    for (index = 0; index < 32; ++index) {
        g_motionRegistry[index].handle = UINT32_MAX;
        g_motionRegistry[index].data = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].firstAnimation = 0;
        g_motionRegistry[index].endAnimation = 0;
        g_motionRegistry[index].previous = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].next = SHENMUE_MOTION_BANK_NULL;
        g_motionRegistry[index].dataSize = 0;
    }
    for (index = 0; index < 4; ++index) {
        g_motionPropBanks[index].handle = -1;
        g_motionPropBanks[index].data = SHENMUE_MOTION_BANK_NULL;
        g_motionPropBanks[index].previous = SHENMUE_MOTION_BANK_NULL;
        g_motionPropBanks[index].next = SHENMUE_MOTION_BANK_NULL;
    }
    g_motionData = &g_motionRegistry[0];
    g_motionData->handle = 0;
    g_motionData->previous = SHENMUE_MOTION_BANK_NULL;
    g_motionData->next = SHENMUE_MOTION_BANK_NULL;
    g_motionPropBanks[0].handle = 0;
    g_motionPropBanks[0].previous = SHENMUE_MOTION_BANK_NULL;
    g_motionPropBanks[0].next = SHENMUE_MOTION_BANK_NULL;
    g_motionData->data = ReadFile("misc", "MOTION.MOT", MOTION_FILE_STORAGE);
    HLib::SetTaskDataMotionFlag(g_motionData->data);
    g_motionData->firstAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_motionData->data)->firstAnimation;
    g_motionData->endAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_motionData->data)->endAnimation;
    g_registeredMotionBytes = g_motionData->dataSize = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_motionData->data)->fileSize;
    g_battleMotionData = &g_motionRegistry[1];
    g_battleMotionData->data = SHENMUE_MOTION_BANK_NULL;
    g_battleMotionData->data = ReadFile("misc", "BATTLE.MOT", MOTION_FILE_STORAGE);
    HLib::SetTaskDataMotionFlag(g_battleMotionData->data);
    std::byte *const fallback = static_cast<std::byte *>(ReadFile("misc", "FBMOTTBL.BIN", FALLBACK_MOTION_STORAGE));
    HLib::SetTaskDataMotionFlag(fallback);
    BindFallbackMotionTable(fallback);
    if (g_battleMotionData->data) {
        g_motionData->next = g_battleMotionData;
        g_battleMotionData->handle = g_motionData->handle + 1;
        g_battleMotionData->previous = g_motionData;
        g_battleMotionData->next = SHENMUE_MOTION_BANK_NULL;
        g_battleMotionData->firstAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_battleMotionData->data)->firstAnimation;
        g_battleMotionData->endAnimation = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_battleMotionData->data)->endAnimation;
        g_battleMotionData->dataSize = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_battleMotionData->data)->fileSize;
        g_registeredMotionBytes += g_battleMotionData->dataSize;
        g_motionBankCount = 2;
    } else {
        g_motionBankCount = 1;
    }
#else
    for (std::size_t index = 0; index < SHENMUE_MOTION_BANK_COUNT(g_motionRegistry); ++index) {
        MotionRegistryEntry &entry = g_motionRegistry[index];
        entry.handle = UINT32_MAX;
        entry.data = SHENMUE_MOTION_BANK_NULL;
        entry.firstAnimation = 0;
        entry.endAnimation = 0;
        entry.previous = SHENMUE_MOTION_BANK_NULL;
        entry.next = SHENMUE_MOTION_BANK_NULL;
        entry.dataSize = 0;
    }
    for (std::size_t index = 0; index < SHENMUE_MOTION_BANK_COUNT(g_motionPropBanks); ++index) {
        MotionPropBank &entry = g_motionPropBanks[index];
        entry.handle = -1;
        entry.data = SHENMUE_MOTION_BANK_NULL;
        entry.previous = SHENMUE_MOTION_BANK_NULL;
        entry.next = SHENMUE_MOTION_BANK_NULL;
    }
    g_motionData = &g_motionRegistry[0];
    g_motionData->handle = 0;
    g_motionData->previous = SHENMUE_MOTION_BANK_NULL;
    g_motionData->next = SHENMUE_MOTION_BANK_NULL;
    g_motionPropBanks[0].handle = 0;
    g_motionPropBanks[0].previous = SHENMUE_MOTION_BANK_NULL;
    g_motionPropBanks[0].next = SHENMUE_MOTION_BANK_NULL;
    g_motionData->data = ReadFile("misc", "MOTION.MOT", MOTION_FILE_STORAGE);
    HLib::SetTaskDataMotionFlag(g_motionData->data);
    const MotionBankHeader *header = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_motionData->data);
    g_motionData->firstAnimation = header->firstAnimation;
    g_motionData->endAnimation = header->endAnimation;
    g_registeredMotionBytes = header->fileSize;
    g_motionData->dataSize = g_registeredMotionBytes;
    g_battleMotionData = &g_motionRegistry[1];
    g_battleMotionData->data = SHENMUE_MOTION_BANK_NULL;
    g_battleMotionData->data = ReadFile("misc", "BATTLE.MOT", MOTION_FILE_STORAGE);
    HLib::SetTaskDataMotionFlag(g_battleMotionData->data);
    std::byte *const fallback = static_cast<std::byte *>(ReadFile("misc", "FBMOTTBL.BIN", FALLBACK_MOTION_STORAGE));
    HLib::SetTaskDataMotionFlag(fallback);
    BindFallbackMotionTable(fallback);
    if (!g_battleMotionData->data) {
        g_motionBankCount = 1;
    } else {
        g_motionData->next = g_battleMotionData;
        g_battleMotionData->handle = g_motionData->handle + 1;
        g_battleMotionData->previous = g_motionData;
        g_battleMotionData->next = SHENMUE_MOTION_BANK_NULL;
        header = SHENMUE_MOTION_BANK_CONST(MotionBankHeader, g_battleMotionData->data);
        g_battleMotionData->firstAnimation = header->firstAnimation;
        g_battleMotionData->endAnimation = header->endAnimation;
        g_battleMotionData->dataSize = header->fileSize;
        g_registeredMotionBytes += g_battleMotionData->dataSize;
        g_motionBankCount = 2;
    }
    return fallback;
#endif
}

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MOTION_BANK_ARRAY
#undef SHENMUE_MOTION_BANK_INITIALIZER
#undef SHENMUE_MOTION_BANK_NULL
#undef SHENMUE_MOTION_BANK_DATA
#undef SHENMUE_MOTION_BANK_COUNT
#undef SHENMUE_MOTION_BANK_CONST

#endif
