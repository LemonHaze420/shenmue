#include "s2/npc_data_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/pak_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include <cstring>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/date_time.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/qron_dc.hpp"
#include "platform/dc/sdk.hpp"
#endif

#define SHENMUE_NPC_NULL 0
namespace shenmue {
namespace s2 {

void NpcRegistryTask_Callback(void *);

bool InitializeNpcAreaRouteStore();

void ResetNpcScheduleSeeds();

void ClearNpcScheduleSeeds();

std::byte *npcHumanIndex = SHENMUE_NPC_NULL;

NpcHumanPakSlot *npcHumanSlots = SHENMUE_NPC_NULL;

std::byte *npcBinaryStorage = SHENMUE_NPC_NULL;

std::uint32_t npcBinaryStorageSize = 0;

std::byte *npcEntryStorage = SHENMUE_NPC_NULL;

std::byte *npcAreaRouteStore = SHENMUE_NPC_NULL;

std::uint32_t npcNewLoadState = 0;

shen_name npcScheduleSeedNames[2];

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
char npcScheduleFilename[32] = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const Matrix4x4 *npcMotionClipFrames[8] = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t npcMotionClipBoneCounts[8] = {}, npcMotionClipFrameCounts[8] = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t npcMotionClipFallbackAnimations[8] = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteSpline *npcRouteNodes = SHENMUE_NPC_NULL;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 npcRouteOrigin = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcSavedDateTimeFlag = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
HLib::Task *npcRegistryTask = SHENMUE_NPC_NULL;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcAuthoredEntryCount = 0, npcRouteEntryCount = 0, npcRouteTableReady = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t npcFrameCount;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcScheduleRecordCount = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::byte **npcScheduleRecords = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteSpline *npcScriptRouteTable;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t npcRouteMovementMask;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t npcUsesMainSchedule;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int8_t npcRouteMovementCursor;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteStateContextPrefix *npcActiveRouteContext;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
HLib::Task *npcPendingRegistryTask = 0, *npcRegistryRetainedTask = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcScheduleAct;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
shen_name npcScheduleArea;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t npcScheduleVersion = 0, npcScheduleFlags = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::byte *npcScheduleOriginData = 0, *npcScheduleEntryData = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcScheduleAreaPointCount = 0, npcScheduleRouteCount = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteLiveEntry *npcCrowdHead = 0, *npcCrowdBookmark = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcAreaRestriction npcAreaRestrictions[8];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcAreaRestrictionCount, npcScheduleFloor = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcActiveStreamCost = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t npcPoolTarget = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t npcSlotFlags[0x1c0];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteResetGroupHeader *npcRouteResetGroup = SHENMUE_NPC_NULL;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
shen_name npcLoadedArea;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcTransitionRuntimeSlot npcTransitionRuntimeSlots[4];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareNpcAreaRoutes();
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CoreNPCTask();
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawNpcTransitions();
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RegisterAuthoredNpcEntries(std::byte **records, NpcRouteSpline *routes);
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RebuildNpcRuntimeAfterReload();
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void NpcRegistryTask_Nullsub(void *) {
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
HLib::Task *npc_Callback_Enqueue(std::int8_t index, std::int32_t act, shen_name area) {
    npcLoadedArea = area;
    npcRegistryTask = 0;
    npcHumansReady = 0;
    EV_NPC_NewLoadSw(0);
    npcPendingRegistryTask = HLib::EnqueueTaskWithoutParameter(NpcRegistryTask_Callback, index, 2, SHEN_NAME4('N', 'P', 'C', 'T'));
    npcScheduleAct = act;
    npcScheduleArea = area;
    npcSetupHumans();
    if (area != SHEN_NAME4('0', '0', '1', '5') && area != SHEN_NAME4('0', '1', '3', '8')) {
        switch (area & 0xff) {
        case 'A':
        case 'K':
        case 'Q':
        case 'W':
            break;
        default:
            HLib::AssignCallbackForTask(npcPendingRegistryTask, NpcRegistryTask_Nullsub);
            npcPendingRegistryTask = 0;
        }
    }
    return npcPendingRegistryTask;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetNpcRouteOrigin() {
    npcRouteOrigin.x = npcRouteOrigin.z = 0.0f;
    const char *const data = reinterpret_cast<const char *>(npcScheduleOriginData);
    const char area = reinterpret_cast<const char *>(&npcScheduleArea)[0];
    for (std::int32_t index = 0; index < 8; ++index) {
        if (data[index] == area) {
            const float *const points = reinterpret_cast<const float *>(data + 8);
            npcRouteOrigin.x = points[index * 2];
            npcRouteOrigin.z = points[index * 2 + 1];
            return;
        }
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void LoadCurrentNpcSchedule(const char *filename, std::int32_t loadResources) {
    if (!loadResources)
        EV_ResetFreeTalkFlags();
    npcScheduleFloor = static_cast<std::int32_t>(GetQronNextStream());
    if (!ReadNPCArea(npcScheduleArea, loadResources, filename)) {
        ResetNpcRouteOrigin();
        if (!(npcScheduleFlags & 1U)) {
            ProjectNpcRouteHeights(npcRouteNodes);
            RebuildNpcRouteLanes(npcRouteNodes, 1, 8);
        }
        RebuildNpcAreaPointGround(npcAreaPoints);
        RebuildNpcRuntimeAfterReload();
    } else {
        const std::uint32_t *const header = reinterpret_cast<const std::uint32_t *>(npcBinaryStorage);
        npcScheduleVersion = header[0];
        npcScheduleFlags = header[1];
        if (npcScheduleVersion == 4) {
            npcScheduleRecordCount = header[2];
            npcScheduleRecords = reinterpret_cast<std::byte **>(npcBinaryStorage + header[3]);
            npcScheduleAreaPointCount = header[4];
            npcAreaPoints = reinterpret_cast<NpcAreaPoint *>(npcBinaryStorage + header[5]);
            npcScheduleRouteCount = header[6];
            npcRouteNodes = reinterpret_cast<NpcRouteSpline *>(npcBinaryStorage + header[7]);
            npcScheduleOriginData = npcBinaryStorage + 0x20;
            npcRouteResetGroup = reinterpret_cast<NpcRouteResetGroupHeader *>(npcBinaryStorage + header[0x1a]);
            npcScheduleEntryData = npcBinaryStorage + header[0x1b];
        }
        ResetNpcRouteOrigin();
        InitializeNpcRouteTable(npcRouteNodes, npcScheduleFlags & 1U);
        InitializeNpcAreaPointTable(npcAreaPoints);
        RegisterAuthoredNpcEntries(npcScheduleRecords, npcRouteNodes);
    }
    InitializeNpcRouteResetGroups(npcRouteResetGroup);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void NPC_Route_Control() {
    if (!npcPendingRegistryTask)
        return;
    npcRegistryTask = npcPendingRegistryTask;
    for (std::int32_t index = 0; index < 8; ++index)
        npcMotionClipFrames[index] = 0;
    LoadCurrentNpcSchedule(0, 1);
    EV_NPC_NewLoadSw(1);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void EV_LoadNPCTable(const char *filename) {
    if (!npcPendingRegistryTask)
        return;
    EV_ResetFreeTalkFlags();
    LoadCurrentNpcSchedule(filename, 0);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeNpcRouteContext(NpcRouteLiveEntry *entry) {
    NpcRouteStateContextPrefix &context = entry->stateContext;
    context.flags = 0;
    context.routeFlags = 0;
    context.stateTable = npcScriptRouteTable;
    context.phase = 0;
    context.otherName = 0;
    context.moveFlags = 0;
    context.owner = entry;
    context.motionWork = 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeNpcInterpreterState(NpcRouteLiveEntry *entry, std::int32_t slot) {
    static std::byte npcFallbackScriptRecord[0x38] = {'M', 'O', 'V', 'E', 0, 20, 1, 1};
    std::byte *const record = entry->scriptRecord;
    std::memset(entry, 0, sizeof(*entry));
    entry->scheduleProgress = 0;
    entry->liveState = 0;
    if (slot >= 0) {
        if (!slot)
            entry->previous = 0;
        else
            entry->previous = entry + -1;
        if (slot == npcRouteEntryCount - 1)
            entry->next = 0;
        else
            entry->next = entry + 1;
        entry->poolIndex = slot;
    }
    entry->motionSpeed = 1.0f;
    InitializeNpcRouteContext(entry);
    entry->streamCost = 2;
    if (slot >= npcAuthoredEntryCount) {
        entry->scriptRecord = npcFallbackScriptRecord;
        entry->name = shen_name(SHEN_NAME4('M', 'O', 'V', 'E'));
        entry->humanPackageIndex = static_cast<std::int16_t>(slot);
        entry->flags |= 8U;
        entry->streamSortKey = 1000.0f;
        entry->liveState = 1;
    } else {
        entry->scriptRecord = slot < 0 ? record : npcScheduleRecords[slot];
        entry->name = *reinterpret_cast<const shen_name *>(entry->scriptRecord);
        if (entry->name == SHEN_NAME4('R', 'E', 'N', '_') || entry->name == SHEN_NAME4('S', 'I', 'N', '_') || entry->name == SHEN_NAME4('W', 'O', 'N', '_') ||
            entry->name == SHEN_NAME4('S', 'Y', 'E', '_') || entry->name == SHEN_NAME4('Y', 'U', 'A', '_'))
            entry->flags |= 0x4000000U;
        entry->humanPackageIndex = static_cast<std::int16_t>(npcLoadFromPak(entry->name));
        entry->currentAreaPoint = *reinterpret_cast<const std::int16_t *>(entry->scriptRecord + 0x0c);
        entry->currentCommand = entry->scriptRecord + 0x38;
    }
    entry->branchCommandStart = entry->currentCommand;
    for (std::int32_t index = 0; index < 4; ++index)
        entry->localBranches[index].packedCondition = 0xffffffffU;
    entry->loadHumanName = entry->name;
    *reinterpret_cast<std::int8_t *>(&entry->retained31c) = -1;
    entry->routeMotionState = 23;
    entry->primarySlotIndex = -1;
    entry->ownerRowIndex = -1;
    entry->nextOwnerRowIndex = -1;
    entry->controlRowIndex = -1;
    entry->nextControlRowIndex = -1;
    entry->loadHumanResult = 8;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
NpcRouteLiveEntry *GetNpcRouteEntryByIndex(std::int32_t index);

// @verified - d
void SetNpcRouteMovementCount(std::int32_t count) {
    if (count > 10)
        count = 10;
    else if (count < 1)
        count = 1;
    std::int32_t remaining = count;
    npcRouteMovementMask = npcRouteMovementCursor = 0;
    std::int32_t index;
    for (index = 0; index < 10 && remaining; ++index) {
        if (dc::sdk::RandomFloat() * 10.0f < static_cast<float>(count) || 10 - index == remaining) {
            npcRouteMovementMask |= static_cast<std::uint16_t>(1U << index);
            --remaining;
        }
    }
    NpcRouteLiveEntry *entry = GetNpcRouteEntryByIndex(0);
    for (index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
        entry->flags &= 0xfcffffffU;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t SelectNpcRunningMotion(const Character *character);
bool mtwkCopyTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::int32_t operation, std::int32_t copyType);
std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

void SetNpcRouteTransitionMotion(NpcRouteLiveEntry *entry, const Vector3 *position, std::int32_t yaw) {
    if (!entry->node)
        return;
    mtwkCopyTransition(entry->motionWork, position, yaw, 6, 3);
    NpcRouteStateContextPrefix *const context = &entry->stateContext;
    std::uint16_t motion;
    if (context->flags & 0x10U)
        motion = SelectNpcRunningMotion(entry->node);
    else
        motion = SelectNpcWalkingMotion(entry->node);
    mtwkSetTimeRelated(entry->motionWork, 1, 5, motion);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t SelectNpcRouteMovementSlot() {
    const std::int32_t mask = npcRouteMovementMask;
    const std::int32_t selected = (mask >> npcRouteMovementCursor) & 1;
    npcRouteMovementCursor = static_cast<std::int8_t>(npcRouteMovementCursor + 1);
    if (npcRouteMovementCursor > 10)
        npcRouteMovementCursor = 1;
    else if (npcRouteMovementCursor < 1)
        npcRouteMovementCursor = 10;
    return selected;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void LoadNPCFiles(NpcRouteSpline *routes) {
    npcScriptRouteTable = routes;
    npcAreaRestrictionCount = 0;
    for (std::int32_t index = 0; index < 8; ++index)
        npcAreaRestrictions[index].name = 0;
    npcPreviewRouteName = npcPreviewRouteArea = 0;
    npcSceneRouteOwner = 0;
    if (!std::strcmp(GetNpcScheduleFilename(), "NPC.BIN") || !std::strcmp(GetNpcScheduleFilename(), "NPC_Q100.BIN"))
        npcUsesMainSchedule = 1;
    else
        npcUsesMainSchedule = 0;
    SetNpcRouteMovementCount(10);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t MatchNpcAreaSelector(shen_name selector, shen_name area) {
    const char *selectorChars = reinterpret_cast<const char *>(&selector);
    const char *areaChars = reinterpret_cast<const char *>(&area);
    for (char index = 0; index < 4; ++index, ++selectorChars, ++areaChars)
        if (*selectorChars != '*' && *selectorChars != *areaChars)
            return 0;
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void LoadNpcMotionClipFile(const char *filename, std::int32_t slot, std::uint16_t fallbackMotion) {
    char directory[64];
    std::sprintf(directory, "scene/%02d/npc", static_cast<int>(g_loadedActNumber));
    std::uint32_t *cursor = static_cast<std::uint32_t *>(ReadFile(directory, filename, shen_name(NPC_MOTION_CLIP_MEMORY)));
    if (!cursor)
        return;
    npcMotionClipBoneCounts[slot] = *cursor++;
    npcMotionClipFrameCounts[slot] = *cursor++;
    cursor += sizeof(NpcMotionClipHeader) / sizeof(std::uint32_t) - 2;
    npcMotionClipFrames[slot] = reinterpret_cast<const Matrix4x4 *>(cursor);
    npcMotionClipFallbackAnimations[slot] = fallbackMotion;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t ReadNPCArea(shen_name area, std::int32_t loadResources, const char *filename) {
    char directory[64];
    std::int32_t changed;
    NpcAreaProfile *profile = 0;
    std::int32_t selected = 0;
    std::sprintf(directory, "scene/%02d/npc", static_cast<int>(g_loadedActNumber));
    std::byte *const storage = static_cast<std::byte *>(shenLoadFile("misc", "npcarea.bin", shen_name(NPC_AREA_PROFILE_MEMORY)));
    const std::int32_t version = *reinterpret_cast<const std::int32_t *>(storage);
    if (version == 4 && (profile = reinterpret_cast<NpcAreaProfile *>(storage + 4))->selectorsOffset) {
        do {
            profile->selectorsOffset += reinterpret_cast<std::uintptr_t>(storage);
            for (; *profile->selectors; ++profile->selectors) {
                if (MatchNpcAreaSelector(*profile->selectors, area)) {
                    if (filename) {
                        profile->filenameOffset += reinterpret_cast<std::uintptr_t>(storage);
                        if (!std::strcmp(filename, profile->filename))
                            selected = 1;
                    } else {
                        const std::int16_t condition = profile->condition;
                        if (condition >= 0) {
                            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, condition))
                                selected = 1;
                        } else {
                            selected = 1;
                        }
                    }
                    break;
                }
            }
            if (!selected) {
                ++profile;
            } else {
                if (!filename)
                    profile->filenameOffset += reinterpret_cast<std::uintptr_t>(storage);
                if (profile->motionFilesOffset) {
                    profile->motionFilesOffset += reinterpret_cast<std::uintptr_t>(storage);
                    for (const char **file = profile->motionFiles; *file; ++file)
                        *file = reinterpret_cast<const char *>(reinterpret_cast<std::uintptr_t>(storage) + reinterpret_cast<std::uintptr_t>(*file));
                }
                if (profile->clipFilesOffset) {
                    profile->clipFilesOffset += reinterpret_cast<std::uintptr_t>(storage);
                    for (const char **file = profile->clipFiles; *file; ++file)
                        *file = reinterpret_cast<const char *>(reinterpret_cast<std::uintptr_t>(storage) + reinterpret_cast<std::uintptr_t>(*file));
                }
                break;
            }
        } while (profile->selectorsOffset);
    }
    if (loadResources) {
        if (selected != 1 || !profile->motionMode)
            ReadMoti("misc", "NPC.MOT");
        ReadMoti("misc", "NPC_TBL.MOT");
        RegisterMotionPropBank(ReadFile("misc", "NPC.MPT", shen_name(NPC_BASE_MOTION_PROP_MEMORY)));
    }
    changed = 0;
    if (!selected) {
        if (std::strcmp(npcScheduleFilename, kNpcBinaryFilenames[g_loadedActNumber])) {
            shenLoadFile(directory, kNpcBinaryFilenames[g_loadedActNumber], npcBinaryStorage, npcBinaryStorageSize);
            std::strcpy(npcScheduleFilename, kNpcBinaryFilenames[g_loadedActNumber]);
            changed = 1;
        }
        npcPoolTarget = 32;
    } else if (version == 4) {
        if (std::strcmp(npcScheduleFilename, profile->filename)) {
            shenLoadFile(directory, profile->filename, npcBinaryStorage, npcBinaryStorageSize);
            std::strcpy(npcScheduleFilename, profile->filename);
            changed = 1;
        }
        npcPoolTarget = profile->maximumLiveCount;
        if (loadResources) {
            std::int32_t index;
            if (profile->clipFiles)
                for (index = 0; profile->clipFiles[index]; ++index)
                    if (std::strcmp(profile->clipFiles[index], "NONE"))
                        LoadNpcMotionClipFile(profile->clipFiles[index], index, 0);
            if (profile->motionFiles) {
                for (index = 0; profile->motionFiles[index]; ++index) {
                    if (std::strstr(profile->motionFiles[index], ".MOT"))
                        ReadMoti(directory, profile->motionFiles[index]);
                    else
                        RegisterMotionPropBank(ReadFile(directory, profile->motionFiles[index], shen_name(NPC_PROFILE_MOTION_PROP_MEMORY)));
                }
            }
        }
    }
    HLib::FreeTaskData(storage);
    return changed;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetNpcSavedDateTimeFlag() {
    return npcSavedDateTimeFlag;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetNpcSavedDateTimeFlag() {
    npcSavedDateTimeFlag = 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t ClearNpcSavedDateTimeFlag() {
    npcSavedDateTimeFlag = 0;
    return 0;
}
#endif

// @verified - d
void ClearNpcHumanSlots() {
    std::int32_t count = *reinterpret_cast<const std::int32_t *>(npcHumanIndex);
    NpcHumanPakSlot *slot = npcHumanSlots;
    while (--count != -1) {
        slot->state = NpcHumanPakState::Empty;
        slot->pak = SHENMUE_NPC_NULL;
        ++slot;
    }
}

// @verified - d
void AllocateNPCH() {
    char directory[32];
    std::sprintf(directory, "scene/%02d/npc", g_loadedActNumber);
    npcHumanIndex = static_cast<std::byte *>(ReadFile(directory, "HUMANS.idx", NPC_HUMAN_INDEX_MEMORY));
    HLib::SetTaskDataMotionFlag(npcHumanIndex);
    npcHumanSlots = static_cast<NpcHumanPakSlot *>(
        HLib::AllocateStaticOrDynamicBlock(*reinterpret_cast<const std::uint32_t *>(npcHumanIndex) * sizeof(NpcHumanPakSlot), NPC_HUMAN_SLOT_MEMORY));
    HLib::SetTaskDataMotionFlag(npcHumanSlots);
    ClearNpcHumanSlots();
}

// @verified - d
void InitializeNpcEntryStorage() {
    npcEntryStorage = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(kNpcEntryMemoryBytes, NPC_ENTRY_MEMORY));
    HLib::SetTaskDataMotionFlag(npcEntryStorage);
}

// @verified - d
bool npcInitDataInternal() {
    char directory[64];
    std::sprintf(directory, "scene/%02d/npc", g_loadedActNumber);
    npcBinaryStorageSize = shenOpenFile(directory, kNpcBinaryFilenames[g_loadedActNumber]);
    npcBinaryStorage = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(npcBinaryStorageSize, NPC_BINARY_MEMORY));
    HLib::SetTaskDataMotionFlag(npcBinaryStorage);
    InitializeNpcEntryStorage();
    return InitializeNpcAreaRouteStore();
}

std::int32_t npcCancelHumanPak(shen_name name) {
    const std::int32_t index = npcLoadFromPak(name);
    if (index >= 0) {
        switch (npcHumanSlots[index].state) {
        case NpcHumanPakState::Empty:
        case NpcHumanPakState::Requested:
            break;
        case NpcHumanPakState::Ready:
            CancelPakSlot(npcHumanSlots[index].pak);
            npcHumanSlots[index].pak = SHENMUE_NPC_NULL;
            npcHumanSlots[index].state = NpcHumanPakState::Empty;
            return 1;
        default:
            break;
        }
    }
    return 0;
}

// @verified - d
void ResetNpcLoadState() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    npcScheduleFilename[0] = 0;
#else
    npcNewLoadState = 0;
#endif
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
char *GetNpcScheduleFilename() {
    return npcScheduleFilename;
}

std::uint32_t GetNpcScheduleVersion() {
    return npcScheduleVersion;
}

std::byte *GetNpcScheduleEntryData() {
    return npcScheduleEntryData;
}
#endif

// @verified - d
void npcInitData() {
    AllocateNPCH();
    npcInitDataInternal();
    ResetNpcLoadState();
    ClearNpcScheduleSeeds();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ResetNpcScheduleState() {
    ResetNpcLoadState();
    ClearNpcScheduleSeeds();
}
#endif

} // namespace s2
} // namespace shenmue
#undef SHENMUE_NPC_NULL

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_npcResetDateTimeIfNeeded() {
    shenmue::s2::SaveNpcDateTime();
}
extern "C" void __attribute__((renesas)) shenmue_s2_event_LoadNPCRoute(const char *filename) {
    shenmue::s2::EV_LoadNPCTable(filename);
}
#endif

#endif
