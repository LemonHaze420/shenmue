#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {

using namespace CycleManager;

CycleManager::MobjEntry *CycleManager::SimSetCursor = nullptr;

CycleManager::RuntimeState g_cycleManagerRuntime{};

std::int32_t g_MOBJ_num_entries = 0;


CycleManager::MobjEntry *g_MOBJ_pData = nullptr;

CycleManager::MobjCycle *g_MOBJ_Cycles = nullptr;


std::int32_t g_mobjCycleCapacity = 0;

std::int32_t g_bDebugSkipCyclesForCharacters = 0;

std::int32_t g_bProcessCharaCycles = 0;

std::int32_t gCycleManager = 0;

std::int32_t g_cm_unk_flag = 0;

std::int32_t g_dbg_flags = 0;

std::int32_t MaxNearNPCs = 0;


std::int32_t IsDBYO = 0;

std::int32_t g_dormantProgramVisitCount = 0;

std::int32_t g_runningProgramCount = 0;


Vector3 g_tmp_cycle_v3{};

std::int32_t numCharactersSimulated = 0;

std::int32_t g_midQueueCycleCount = 0;

std::int32_t g_farQueueCycleCount = 0;

std::int32_t g_nearQueueCycleCount = 0;

std::int32_t g_promoteListCycleCount = 0;

std::int32_t g_perFrameNpcWorkCnt = 0;

std::int8_t g_lastProximityGroup = 0;

const std::byte *g_pCycleManagerTimeFlag = nullptr;

std::uint64_t g_cycleShopExitPositionXY = 0;

std::int32_t g_cycleShopExitPositionZ = 0;

std::int32_t g_cycleShopExitYaw = 0;

std::int32_t g_cycleShopExitPublished = 0;

std::uint16_t g_steeringIntersectionVolumeCount = 0;

float g_default_turning_box_half_depth_z = 0.0f;

float g_angYoffset = 0.0f;

PackOwner *HUMANS_AFS_Ptr = nullptr;


HumansEntry *HumansIndexTable = nullptr;


PackSlot *g_pszTempLoadedHumanSched = nullptr;

std::int32_t g_bHasLoadedHuman = 0;

// The character whose human pack the script asked for (CycleManager_SetCyclePtrByCharaID); CycleManager_GetNextTaskPointer hands it over.
std::uint32_t g_cycleScriptHumanChara = 0;


char g_cycle_file_str[16]{};

std::uint8_t g_tmp_new_day = 0;

std::uint32_t g_prevCycleSeconds = 0;



char g_tmp_cm_path[32]{};


std::int32_t g_MOBJ_needs_new_update = 0;



CycleManager::MobjEntry *g_DormantPrograms = nullptr;

CycleManager::MobjEntry *g_NearQueue = nullptr;

CycleManager::MobjEntry *g_SimSetHead = nullptr;

CycleManager::MobjEntry *g_PromoteList = nullptr;

CycleManager::MobjEntry *g_MidQueue = nullptr;

CycleManager::MobjEntry *g_LoadQueue = nullptr;

CycleManager::MobjEntry *g_FarQueue = nullptr;

CycleManager::MobjEntry *g_RunningPrograms = nullptr;




uint32_t g_CycleTable[20]{};

uint32_t g_TotalActiveCycles = 0;
namespace CycleManager {


CycleManager::MobjCycle DefaultCycle() {
    CycleManager::MobjCycle cycle{};
    cycle.humansIdx = -1;
    cycle.pos = {1000.0f, 1000.0f, 1000.0f};
    cycle.moveTargetPos = {1000.0f, 1000.0f, 1000.0f};
    cycle.currentWaypointPos = {1000.0f, 1000.0f, 1000.0f};
    cycle.xmptResumePos = {1000.0f, 1000.0f, 1000.0f};
    cycle.primarySoundBankId = -1;
    cycle.secondarySoundBankId = -1;
    cycle.dist = 3.0f;
    cycle.range = 1.5f;
    cycle.pathMoveSpeed = 1.0f;
    cycle.animMoveSpeedScale = 0.041666668f;
    cycle.baseMoveSpeed = 1.0f;
    cycle.steeringState = CycleSteerState_Normal;
    cycle.steeringCommand = CycleSteerCmd_None;
    cycle.actionTimerOrDoorHandle = -1;
    cycle.modeValue = 2;
    cycle.steeringTarget = {1000.0f, 1000.0f, 1000.0f};
    return cycle;
}

CycleManager::MobjEntry *&ProgramListHead(CycleProgramList list) {
    switch (list) {
    case CycleProgramList_Near:
        return g_NearQueue;
    case CycleProgramList_SimSet:
        return g_SimSetHead;
    case CycleProgramList_Promote:
        return g_PromoteList;
    case CycleProgramList_Mid:
        return g_MidQueue;
    case CycleProgramList_Load:
        return g_LoadQueue;
    case CycleProgramList_Far:
        return g_FarQueue;
    case CycleProgramList_Running:
        return g_RunningPrograms;
    default:
        return g_DormantPrograms;
    }
}


void ClearProgramLists() {
    g_DormantPrograms = nullptr;
    g_NearQueue = nullptr;
    g_SimSetHead = nullptr;
    g_PromoteList = nullptr;
    g_MidQueue = nullptr;
    g_LoadQueue = nullptr;
    g_FarQueue = nullptr;
    g_RunningPrograms = nullptr;
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void MoveProgramToList(CycleManager::NpcRecord &npc, CycleProgramList destination, CycleListInsert insert);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

extern std::int32_t g_actNumber;

std::uint16_t g_dynamicTurnCollisionVolumeCount = 0;

std::int32_t g_cycleConversationHoldActive = 0;

std::int32_t g_scntReferencedCycleHeld = 0;

std::int32_t g_cyclTalkCycleActive = 0;

std::int32_t g_cycleTalkSuppressed = 0;

PackSlot *gRainModelPtr = nullptr;



float g_cycle_distance_check = 0.0f;


CycleManager::TaggedPayload *g_CycleManager_Current = nullptr;


CycleManager::TaggedPayload *g_CM_obj_list = nullptr;

std::int32_t nRainState = 0;

std::array<void *, 4> g_rainStateModels{};

float g_fGlobalTurningRadius = 0.40000001f;

CycleManager::ReadFileFn g_cycleManagerReadFile = nullptr;

CycleManager::FileExistsFn g_cycleManagerFileExists = nullptr;

CycleManager::ObjectNodeRotationFn g_cycleManagerObjectNodeRotation = nullptr;

CycleManager::FindCharacterFn g_cycleManagerFindCharacter = nullptr;

CycleManager::EnqueueTaskFn g_cycleManagerEnqueueTask = nullptr;

CycleManager::UmbrellaAnimIdFn g_cycleManagerUmbrellaAnimId = nullptr;

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

#if SHENMUE_IS_PC
IntersectionEntry *g_cycleSteeringVolumes = nullptr;
IntersectionEntry *g_pIntersectionPtr = nullptr;

// @verified - p
bool InitMemory() {
    g_cycleSteeringVolumes = static_cast<IntersectionEntry *>(
        HLib_AllocHeapMemBlock((kDynamicTurnVolumeCapacity + kIntersectionVolumeCapacity) * sizeof(IntersectionEntry), TASK_MOBJ.name));
    g_pIntersectionPtr = nullptr;
    if (g_cycleSteeringVolumes) {
        (reinterpret_cast<HLib::TaskData *>(g_cycleSteeringVolumes) - 1)->alignment |= 1;
        g_pIntersectionPtr = g_cycleSteeringVolumes + kDynamicTurnVolumeCapacity;
    }
    g_cycleManagerRuntime.dynamicTurnCollisionVolumes = {g_cycleSteeringVolumes, g_cycleSteeringVolumes ? kDynamicTurnVolumeCapacity : 0};
    g_cycleManagerRuntime.intersectionEntries = {g_pIntersectionPtr, g_pIntersectionPtr ? kIntersectionVolumeCapacity : 0};
    return g_cycleSteeringVolumes != nullptr;
}
#else
alignas(std::max_align_t) std::array<std::byte, kMobjCoreAllocationSize> g_mobjCoreAllocation{};

alignas(std::max_align_t) std::array<std::byte, kMobjHeapAllocationSize> g_mobjHeapAllocation{};

alignas(std::max_align_t) std::array<std::byte, kMobjDynamicAllocationSize> g_mobjDynamicAllocation{};

#endif
void AttachEntityAndInitCycle(Character *character, bool deactivate);

bool AreaCapacityExceeded(std::uint32_t areaId);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




std::uint32_t GetRelativePositionToCycles(Character *point) {
    Vector3 pointPosition{};
    std::int16_t pointYaw = 0;
    if (point) {
        pointPosition = GetPositionForEntity(point->entity.node);
        for (const CharacterNode *node = &point->entity.node; node; node = node->child) {
            pointYaw = static_cast<std::int16_t>(pointYaw + node->rotation.y);
        }
    }

    std::uint32_t relativePosition = 0;
    const auto count = std::min<std::size_t>(g_cycleManagerRuntime.npcRecords.size(), static_cast<std::size_t>(std::max(g_MOBJ_num_entries, 0)));
    for (std::size_t index = 0; index < count; ++index) {
        const CycleManager::NpcRecord &cycle = g_cycleManagerRuntime.npcRecords[index];
        const Vector3 cyclePosition = cycle.cycle->pos;
        const float distanceX = pointPosition.x - cyclePosition.x;
        const float distanceZ = pointPosition.z - cyclePosition.z;
        if (!(5.0f > std::sqrt(distanceX * distanceX + distanceZ * distanceZ)) || !cycle.cycle->obj)
            continue;
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-pointYaw));
        Vector3 local{};
        Matrix4x4::TransformPoint({cyclePosition.x - pointPosition.x, cyclePosition.y, cyclePosition.z - pointPosition.z}, local);
        Matrix4x4::Pop(1);
        if (1.4 > std::fabs(static_cast<double>(local.x)))
            relativePosition |= 0.0f > local.z ? 1U : 2U;
    }
    return relativePosition;
}
} // namespace CycleManager


Mt5RuntimeNode *FindRuntimeNodeByKind(Mt5RuntimeNode *node, std::byte *modelBase, uint16_t nodeKind) {
    while (node) {
        if (node->nodeKind == nodeKind)
            return node;
        if (node->childOffset) {
            if (Mt5RuntimeNode *const found = FindRuntimeNodeByKind(reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->childOffset), modelBase, nodeKind)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->siblingOffset) : nullptr;
    }
    return nullptr;
}

HLib::Task *g_mobjCoreTask = nullptr;

HLib::Task *g_mobjMotionTask = nullptr;

std::byte *g_pMOBJ = nullptr;

std::byte *g_pMOBJ_HeapHead = nullptr;

std::byte *g_tmp_MOBJ = nullptr;

CycleMoveObjectRuntime *g_visibleMoveObjects = nullptr;

CycleMoveObjectRuntime *g_parkedMoveObjects = nullptr;

CycleMoveObjectRuntime *g_freeMoveObjects = nullptr;
namespace CycleManager {



Character *MapRyo() {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

Character *DefaultFindCharacter(uint32_t token) {
    if (!token || !g_pSCNC || !g_pSCNC->header.scenarioTask)
        return nullptr;
    return CHID_Find(g_pSCNC->header.scenarioTask, token);
}
} // namespace CycleManager


using namespace CycleManager;
namespace CycleManager {


const BikeMoveObjectToken *GetBikeMoveObjectToken(uint32_t ownerToken, uint32_t vehicleToken);

float ResolveBikeSpeedParam(const CycleMoveObjectRuntime *runtime, float rawSpeed);

CycleMoveObjectRuntime *AttachVehicleRuntime(CycleManager::NpcRecord &npc, uint32_t vehicleToken, uint32_t moveObjectKind);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

constexpr std::array<std::int32_t, 8> kDobuitaBikeSounds{0, 1, 2, 4, -1, -1, 5, -1};

constexpr std::array<std::int32_t, 8> kSakuragaokaBikeSounds{-1, 1, -1, -1, -1, -1, 2, -1};

constexpr std::array<std::int32_t, 8> kJu00BikeSounds{-1, -1, -1, -1, -1, -1, 0, -1};

constexpr std::array<std::int32_t, 8> kAmihamaBikeSounds{-1, 5, 2, 4, 0, 1, -1, 6};

constexpr std::array<BikeAnimCurve, 8> kYkhiBikeAnimCurves{{
    {96, 0, 106, 1784},
    {41, 1784, 61, 0},
    {16, 0, 25, 1784},
    {9, 1784, 13, 0},
    {139, 0, 143, -910},
    {28, -910, 32, 0},
    {138, 0, 143, 16384},
    {28, 16384, 33, 0},
}};

constexpr std::array<BikeAnimCurve, 8> kSobaBikeAnimCurves{{
    {51, 0, 86, -364},
    {30, -364, 70, 0},
    {0, 182, 30, -364},
    {1, -364, 30, 0},
    {141, 0, 151, -910},
    {39, -910, 50, 0},
    {141, 0, 151, 16384},
    {37, 16384, 41, 0},
}};

constexpr std::array<BikeAnimCurve, 2> kHalBikeAnimCurves{{
    {87, 5461, 133, 0},
    {113, 0, 180, 5461},
}};

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


int16_t AngleDegreesToFixed16(float degrees) {
    if (!std::isfinite(degrees))
        degrees = 0.0f;
    return static_cast<int16_t>(static_cast<int32_t>((degrees * 8192.0f) / 45.0f) & 0xFFFF);
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

CycleManager::NpcRecord *FindMutableNpcRecordInternal(uint32_t token) {
    for (CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords)
        if (npc.cycle->moveConfig->obj_id.name == token)
            return &npc;
    return nullptr;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


CycleManager::NpcRecord *FindMutableNpcRecord(std::uint32_t token) {
    return FindMutableNpcRecordInternal(token);
}

bool DoesCycleEntryHaveFlagsSet(std::uint32_t token) {
    CycleManager::NpcRecord *const npc = FindMutableNpcRecordInternal(token);
    return npc && npc->cycle->taggedPayloadList;
}

bool GetCycleFlagForSpecificChara(std::uint32_t token) {
    CycleManager::NpcRecord *const npc = FindMutableNpcRecordInternal(token);
    return npc && npc->cycle->doorAnimActive != 0;
}


char *FilePointer(uint32_t offset) {
    return offset ? reinterpret_cast<char *>(g_cycleManagerRuntime.bytes.data()) + offset : nullptr;
}


uint32_t FileOffset(const void *pointer) {
    return pointer ? static_cast<uint32_t>(static_cast<const uint8_t *>(pointer) - g_cycleManagerRuntime.bytes.data()) : 0;
}


CycleManager::NpcRecord *EntryRecord(CycleManager::MobjEntry *entry) {
    return entry ? &g_cycleManagerRuntime.npcRecords[static_cast<std::size_t>(entry - g_MOBJ_pData)] : nullptr;
}

CycleManager::NpcRecord *NextProgramInList(CycleProgramList list, const CycleManager::NpcRecord *after) {
    return CycleManager::EntryRecord(after ? after->entry->nextDormantProgram : CycleManager::ProgramListHead(list));
}

void ResetPublishedSceneState() {
    g_cycleShopExitPublished = 0;
    g_visibleMoveObjects = nullptr;
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void UpdatePathSegmentCache(CycleMoveObjectRuntime &runtime, const Vector2 &from, const Vector2 &to);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


void InitBikeMoveObjectPathContext(CycleManager::NpcRecord &npc);
} // namespace CycleManager


bool npcActivateIfNearInArea(CycleManager::NpcRecord &npc, float radius, const Vector3 *target, int16_t targetYaw, bool commit, uint16_t animId);

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

// The largest free block of the task heap, as CycleManager_UpdateCharaCycles and Weather_SetRainModelsFromHumans scan it inline; the
// scan stops at a misaligned block and before the last.
uint64_t LargestFreeTaskHeapBlock() {
    uint64_t largest = 0;
    for (HLib::TaskData *block = g_pFirstTaskData; block; block = block->next_ptr) {
        if ((reinterpret_cast<uintptr_t>(block + 1) & 0x1Fu) != 0)
            break;
        if (static_cast<uint32_t>(block->status) != make_shen_name('U', 'S', 'E', ' ') || !block->taskToken) {
            const uint32_t size = static_cast<uint32_t>(block->content_size) - 0x40u;
            if (largest < size)
                largest = size;
        }
        if (block->next_ptr == g_pLastTaskData)
            break;
    }
    return largest;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


bool ReadCommandPositionYaw(uint32_t recordOffset, Vector3 &pos, int16_t &yaw) {
    uint32_t rawYaw = 0;
    if (!CycleManager::ReadCommandValue(recordOffset, 4, pos.x) || !CycleManager::ReadCommandValue(recordOffset, 8, pos.y) ||
        !CycleManager::ReadCommandValue(recordOffset, 12, pos.z) || !CycleManager::ReadCommandValue(recordOffset, 16, rawYaw) ||
        !CycleManager::IsReasonablePosition(pos)) {
        return false;
    }
    yaw = static_cast<int16_t>(rawYaw & 0xFFFFu);
    return true;
}

uint32_t CurrentTimeSeconds();

bool InitTimedPathMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);

bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive);

bool InitStairPath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset);

bool InitVehiclePath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset, uint32_t opcode);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

uint32_t MCIR_FindChairGroupById(uint32_t chairGroupId);

Vector3 MoveObjPresetChairPosition(const CycleManager::NpcRecord &npc, uint32_t chairPos);

uint16_t FindMoveObjIdleMotion(const CycleManager::NpcRecord &npc);

bool HasMoveObjChairPreset(const CycleManager::NpcRecord &npc);

bool EndMoveObjForHistory(CycleManager::NpcRecord &npc);

void ClearAllMoveObjChairOccupancy();

float DistanceXZWithEpsilon(const Vector3 &lhs, const Vector3 &rhs);

const BikeAnimTokenSet kBikeAnimsHAL0 = {
    {0x0059, 0x0059, static_cast<int16_t>(0x80DA), static_cast<int16_t>(0x8317), static_cast<int16_t>(0x8319), static_cast<int16_t>(0x8318)}};

const BikeAnimTokenSet kBikeAnimsYKHI = {{static_cast<int16_t>(0x8316), static_cast<int16_t>(0x8314), static_cast<int16_t>(0x8081),
    static_cast<int16_t>(0x8313), static_cast<int16_t>(0x8312), static_cast<int16_t>(0x8315)}};

const BikeAnimTokenSet kBikeAnimsK013 = {
    {0, 0, static_cast<int16_t>(0x815B), static_cast<int16_t>(0x8325), static_cast<int16_t>(0x8327), static_cast<int16_t>(0x8326)}};

const BikeAnimTokenSet kBikeAnimsHAL = {{static_cast<int16_t>(0x8323), static_cast<int16_t>(0x8322), static_cast<int16_t>(0x8123), static_cast<int16_t>(0x831C),
    static_cast<int16_t>(0x831A), static_cast<int16_t>(0x831B)}};

const BikeAnimTokenSet kBikeAnimsSOBA = {{static_cast<int16_t>(0x8321), static_cast<int16_t>(0x831F), static_cast<int16_t>(0x80F6),
    static_cast<int16_t>(0x831E), static_cast<int16_t>(0x831D), static_cast<int16_t>(0x8320)}};

const BikeAnimTokenSet kBikeAnimsHAL0Alt = {{static_cast<int16_t>(0x8004), static_cast<int16_t>(0x8310), static_cast<int16_t>(0x8002),
    static_cast<int16_t>(0x8329), static_cast<int16_t>(0x832B), static_cast<int16_t>(0x832A)}};

const BikeAnimTokenSet kBikeAnimsHALAlt = {
    {0, 0, static_cast<int16_t>(0x8268), static_cast<int16_t>(0x832C), static_cast<int16_t>(0x832E), static_cast<int16_t>(0x832D)}};

const BikeMoveObjectToken kBikeTokenHAL0 =
    CycleManager::MakeBikeToken(&kBikeAnimsHAL0, {0.0f, 0.0f, -0.4f}, 1274, 6917, {-0.391409993f, 0.976859987f, -0.370499998f},
        {0.410589993f, 0.968159974f, -0.370499998f}, {0.0f, 0.904999971f, -0.741999984f}, -1.1f, -0.2f, static_cast<int16_t>(0xC000), 0, 1.2f, 0.49f, 2);

const BikeMoveObjectToken kBikeTokenYKHI = CycleManager::MakeBikeToken(&kBikeAnimsYKHI, {-0.00535f, 0.0f, -0.28985f}, 1820, 1820,
    {-0.306760013f, 1.120569944f, -0.394529998f}, {0.301340014f, 1.143049955f, -0.417129993f}, {0.0f, 1.149000049f, -0.779999971f}, -0.53566003f, -0.029920001f,
    static_cast<int16_t>(0xC000), 1820, 0.9f, 0.32f, 0);

const BikeMoveObjectToken kBikeTokenK013 =
    CycleManager::MakeBikeToken(&kBikeAnimsK013, {0.0f, 0.0f, -0.21842f}, 1820, 1820, {-0.270999998f, 0.949000001f, -0.384000003f},
        {0.264999986f, 0.941999972f, -0.368999988f}, {0.0f, 0.924870014f, -0.349519998f}, -1.0f, 0.0f, 0, 0, 0.96f, 0.32f, 3);

const BikeMoveObjectToken kBikeTokenHAL =
    CycleManager::MakeBikeToken(&kBikeAnimsHAL, {0.0f, 0.0f, -0.20378f}, 3276, 2730, {-0.394279987f, 1.080250025f, -0.266130000f},
        {0.324449986f, 1.061139941f, -0.285629988f}, {0.0f, 0.922999978f, -0.542999983f}, -0.64161998f, 0.21901f, 0, 0, 1.1f, 0.46f, 1);

const BikeMoveObjectToken kBikeTokenSOBA = CycleManager::MakeBikeToken(&kBikeAnimsSOBA, {0.02627f, 0.0f, -0.32223f}, 2184, 1820,
    {-0.291159987f, 1.180629969f, -0.506309986f}, {0.291159987f, 1.180629969f, -0.506309986f}, {0.0f, 0.779269993f, -0.781000018f}, 0.53772002f, -0.32223001f,
    0x4000, static_cast<int16_t>(0xFE94), 0.9f, 0.32f, 0);

const BikeMoveObjectToken kBikeTokenHAL0Alt =
    CycleManager::MakeBikeToken(&kBikeAnimsHAL0Alt, {0.0f, 0.0f, -0.4f}, 1274, 6917, {-0.391629994f, 0.976859987f, -0.377090007f},
        {0.410369992f, 0.966859996f, -0.369089991f}, {0.0f, 0.904999971f, -0.741999984f}, -1.1f, -0.2f, static_cast<int16_t>(0xC000), 0, 1.2f, 0.49f, 2);

const BikeMoveObjectToken kBikeTokenHALAlt =
    CycleManager::MakeBikeToken(&kBikeAnimsHALAlt, {0.0f, 0.0f, -0.20378f}, 3276, 2730, {-0.364279985f, 1.050250053f, -0.276129991f},
        {0.364450008f, 1.051139951f, -0.275629997f}, {0.0f, 0.922999978f, -0.625000000f}, -0.64161998f, 0.21901f, 0, 0, 1.1f, 0.46f, 1);

const BikeSpeedProfile kBikeSpeedDefault = {0.55555558f, 0.55555558f, 0.55555558f, 0.55555558f, 0.55555558f, 0u};

const BikeSpeedProfile kBikeSpeedBike = {0.13888890f, 0.23148148f, 0.23148148f, 0.23148148f, 0.23148148f, 0u};

const BikeSpeedProfile kBikeSpeedHAL = {0.13888890f, 0.32407409f, 0.37037039f, 0.46296296f, 0.55555558f, 0u};

const BikeSpeedProfile kBikeSpeedFallback = {0.18518519f, 0.18518519f, 0.18518519f, 0.18518519f, 0.18518519f, 0u};

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


int16_t FoldUnsignedYaw(uint16_t yaw);
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

float SnapshotMoveObjTransform(CycleManager::NpcRecord &npc, bool live);

void DoBikeAngles(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime, float speed);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {

NaturalCubicSpline3DCache g_spline3d{};

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void RotateForBIKE(CycleMoveObjectRuntime &runtime, int32_t slope, int32_t yaw, int32_t lean);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {



CycleManager::NpcRecord *CycleRecord(CycleManager::MobjCycle *ctx) {
    return &g_cycleManagerRuntime.npcRecords[static_cast<std::size_t>(ctx - g_MOBJ_Cycles)];
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

bool ReadMNRBVector(uint32_t tableOffset, uint32_t index, Vector3 &out);

void PruneMNRBReservations(CycleManager::MNRBEntry &entry);

void CheckBoundaryTriggerAndPlaySounds(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry &entry, uint32_t recordOffset);

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {




uint32_t AppendCycleEntry(CycleManager::MobjCycle *ctx, char *record, uint32_t live, int32_t time) {
    if (!ctx || !record) {
        ctx->morsCommandStatus = 0;
        ctx->currentOpcode = CycleOpcode_AdvanceScript;
        return 0;
    }
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    if (!entry) {
        ctx->morsCommandStatus = 0;
        ctx->morsDeadlineTime = 0;
        ctx->currentOpcode = CycleOpcode_AdvanceScript;
        return 40;
    }
    ctx->morsEntryOffset = entry->lineDataOffset;
    CycleManager::PruneMNRBReservations(*entry);
    if (!entry->lineDataOffset) {
        ctx->currentOpcode = CycleOpcode_AdvanceScript;
        return 40;
    }
    ctx->morsCommandStatus = entry->entryKind << 16;
    ctx->morsDeadlineTime = *reinterpret_cast<const uint32_t *>(record + 8);
    ctx->morsQueueNodeOffset = 0;
    if (entry->entryKind == 0) {
        CycleManager::CheckBoundaryTriggerAndPlaySounds(*CycleManager::CycleRecord(ctx), *entry, CycleManager::FileOffset(record));
    } else if (entry->entryKind == 1) {
        ctx->morsCommandOffset = CycleManager::FileOffset(record);
        ctx->morsCommandStatus = 0x10000;
    } else if (entry->entryKind == 3) {
        CycleManager::HandleTimedMorsApproach(ctx, record, live, time);
    }
    return 40;
}
namespace CycleManager {






uint32_t CheckPointer(int32_t time, CycleManager::MobjCycle *ctx, char **outData, uint32_t *outResult) {
    char *const command = *outData;
    const int32_t *const word = reinterpret_cast<const int32_t *>(command);
    if (word[0] == CycleOpcode_UseMnrBEntry) {
        *outResult = 1;
        *outData = command + 56;
        return 1;
    }
    if (word[0] == CycleOpcode_ScheduleMnrBEntry) {
        if (!ctx || !command) {
            *outResult = 1;
        } else if (CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(static_cast<uint32_t>(word[1]))) {
            if (entry->entryKind == 1) {
                *outResult = 1;
                const int32_t deadline = word[2] <= 0 ? time - word[2] : word[2];
                if ((time >= static_cast<int32_t>(entry->activeTimeStart) && time < static_cast<int32_t>(entry->activeTimeEnd)) || time < deadline) {
                    *outResult = 0;
                    AppendCycleEntry(ctx, command, 0, time);
                }
            } else if (entry->entryKind == 3) {
                *outResult = CycleManager::HandleTimedMorsApproach(ctx, command, 0, time);
            }
        }
        *outData = command + 40;
        return 1;
    }
    if (word[0] != CycleOpcode_PlayEntryStateAnimPoolUntilTime)
        return 0;

    const int32_t trigger = word[1] < 0 ? time - word[1] : word[1];
    ctx->scheduledTriggerTime = static_cast<uint32_t>(trigger);
    ctx->entryStateCommandOffset = static_cast<int32_t>(CycleManager::FileOffset(command));
    int32_t rows = 0;
    if (time < trigger && word[2]) {
        for (const int32_t *row = reinterpret_cast<const int32_t *>(CycleManager::FilePointer(static_cast<uint32_t>(word[2]))); *row != -1; row += 4)
            ++rows;
    }
    ctx->morsCommandStatus = static_cast<uint32_t>(rows);
    ctx->morsAuxOffsetOrState = -1;
    *outData = command + 12;
    *outResult = rows == 0;
    return 1;
}
} // namespace CycleManager


void MOBJ_UpdateTransformsRecursively_0() {
    if (!CycleManager::IsReady())
        return;

    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::ShutRow &row : cm.shutRows) {
        const uint8_t previousByte31 = row.byte31;
        const float nextY = row.pos.y + row.yStep;
        row.yStep = 0.0f;

        if (row.minY < nextY) {
            if (nextY < row.maxY) {
                row.pos.y = nextY;
                row.byte31 = 1;
            } else {
                row.pos.y = row.maxY;
                row.byte31 = 2;
            }
        } else {
            row.pos.y = row.minY;
            row.byte31 = 0;
        }

        if (row.byte31 != previousByte31)
            ++cm.shutDispatchStateChanges;
        CycleManager::UpdateSHUTRowObject(row);
        // Scene-object collision extents remain unavailable until the current MAPC/CVWK owner binds.
    }
}
namespace CycleManager {


float DistanceSquaredXZ(const Vector3 &lhs, const Vector3 &rhs) {
    const float dx = lhs.x - rhs.x;
    const float dz = lhs.z - rhs.z;
    return dx * dx + dz * dz;
}

float Distance(const Vector3 &lhs, const Vector3 &rhs) {
    const float dx = lhs.x - rhs.x;
    const float dy = lhs.y - rhs.y;
    const float dz = lhs.z - rhs.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool NormalizeVectorInPlace(Vector3 &value) {
    const float length = std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
    if (length <= 0.00000011920929f || !std::isfinite(length))
        return false;
    value.x /= length;
    value.y /= length;
    value.z /= length;
    return true;
}

int16_t Atan2Fixed16(float x, float z) {
    return static_cast<int16_t>(atan2_fixed16(x, z));
}
} // namespace CycleManager


using namespace CycleManager;
namespace CycleManager {


void ResetPublishedSceneState();
} // namespace CycleManager




bool PerformAicaSoundUpdates(CycleManager::NpcRecord &npc, const Vector3 &target, int16_t targetYaw, bool commit);

CycleManager::FrameCacheStats CollectCycleManagerFrameCacheStats() {
    // Deformed-pose/render-batch caches are owned by the current MOTN/D3T
    // owners.  Cycle keeps no renderer cache mirror.
    return {};
}


CycleManager::MobjCycle *npcFindEntry(uint32_t charaName) {
    for (CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.cycle->moveConfig->obj_id.name == charaName)
            return npc.cycle;
    }
    return nullptr;
}
namespace CycleManager {



CycleManager::NpcRecord *GetTrackedMobj(CycleManager::NpcRecord &npc) {
    return npc.cycle->trackedMobj ? CycleManager::CycleRecord(npc.cycle->trackedMobj) : nullptr;
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {


uint16_t g_mobjCollisionObjectCount = 0;

uint16_t g_mobjCollisionObjectCursor = 0;

uint16_t g_dynamicTurnCollisionVolumeCursor = 0;

uint16_t g_steeringIntersectionVolumeCursor = 0;


uint32_t g_currentMobjRectangleOffset = 0;


std::int32_t g_cycleCommandScanHasPosition = 0;

Vector3 g_cycleCommandScanPosition{};

constexpr CycleAreaCapacity kCycleAreaCapacities[]{
    {.areaId = SHEN_NAME(DCBN), .capacity = 2}, {.areaId = SHEN_NAME(DMAJ), .capacity = 0}, {.areaId = 0, .capacity = -1}};

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {




CycleManager::MobjData g_cycleRoot{.currentBranchOfs = 0,
    .obj_id = SHEN_NAME(AKIR),
    .programIdSuffix = SHEN_NAME(PRG1),
    .motionRadiusTimingScalar = 1.0f,
    .steeringBodyHalfWidth = 0.72f,
    .steeringBodyHalfDepth = 0.72f,
    .motionScalar18 = 176.0f,
    .motionScalar1C = 65.0f,
    .cycleClassCode = 0,
    .configWord2C = 18,
    .anchorGroupCode = 0,
    .motionVariantCode = 1,
    .packedMotionParams = {89, 89, 89, 89},
    .motionParamIds = {
        89, 95, 95, 95, 95, 95, 95, 593, 102, 641, 725, 665, 63, 97, 98, 99, 100, 603, 604, 605, 606, 57, 58, 620, 621, 89, 89, 89, 89, 89, 89, 89}};

CycleManager::MobjCycle g_cyclTalkCycleData = CycleManager::DefaultCycle();

CycleManager::NpcRecord g_cyclTalkCycle = [] {
    CycleManager::NpcRecord talkCycle{};
    talkCycle.cycle = &g_cyclTalkCycleData;
    talkCycle.cycle->moveConfig = &g_cycleRoot;
    talkCycle.cycle->humansIdx = 0;
    talkCycle.cycle->activeAnimId = 0x66;
    talkCycle.cycle->currentOpcode = CycleOpcode_InitTimedPath;
    talkCycle.cycle->modeValue = 0;
    talkCycle.cycle->steeringTarget = {};
    talkCycle.cycle->loadQueueDisposition = 1;
    talkCycle.cycle->areaID = SHEN_NAME(AKIR);
    talkCycle.cycle->pos = {};
    talkCycle.cycle->moveTargetPos = {};
    talkCycle.cycle->currentWaypointPos = {};
    talkCycle.cycle->xmptResumePos = {};
    talkCycle.cycle->dist = 0.0f;
    talkCycle.cycle->range = 0.0f;
    talkCycle.cycle->steeringState = static_cast<CycleSteeringState>(0);
    return talkCycle;
}();

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

bool TickMnrbBusStopUse(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry, uint32_t live);

void AttachEntityAndInitCycle(Character *character, bool deactivate);


void MoveProgramToList(CycleManager::NpcRecord &npc, CycleProgramList destination, CycleListInsert insert) {
    CycleManager::MobjEntry *const entry = npc.entry;
    CycleManager::MobjEntry *const previous = entry->prev_or_backlink;
    CycleManager::MobjEntry *const next = entry->nextDormantProgram;
    if (previous) {
        previous->nextDormantProgram = next;
        entry->prev_or_backlink = nullptr;
    } else if (CycleManager::ProgramListHead(npc.programList) == entry) {
        CycleManager::ProgramListHead(npc.programList) = next;
    }
    if (next)
        next->prev_or_backlink = previous;

    CycleManager::MobjEntry *&head = CycleManager::ProgramListHead(destination);
    if (insert == CycleListInsert::Head) {
        entry->nextDormantProgram = head;
        if (head)
            head->prev_or_backlink = entry;
        head = entry;
    } else if (head) {
        CycleManager::MobjEntry *last = head;
        while (last->nextDormantProgram)
            last = last->nextDormantProgram;
        last->nextDormantProgram = entry;
        entry->prev_or_backlink = last;
        entry->nextDormantProgram = nullptr;
    } else {
        head = entry;
        entry->nextDormantProgram = nullptr;
    }
    npc.programList = destination;
}

std::vector<CycleManager::NpcRecord *> SnapshotList(CycleProgramList list) {
    std::vector<CycleManager::NpcRecord *> result;
    for (CycleManager::MobjEntry *entry = CycleManager::ProgramListHead(list); entry; entry = entry->nextDormantProgram)
        result.push_back(CycleManager::EntryRecord(entry));
    return result;
}

uint32_t CountProgramsInList(CycleProgramList list) {
    uint32_t count = 0;
    for (const CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.programList == list)
            ++count;
    }
    return count;
}

void AttachEntityAndInitCycle(Character *character, bool deactivate) {
    if (!character || !g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return;

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    const size_t bucket = reinterpret_cast<uintptr_t>(character) % control->characterBuckets.size();
    HLib::Task *task = nullptr;
    for (auto *link = static_cast<CharacterBucketNode *>(control->characterBuckets[bucket]); link; link = link->next) {
        if (link->character == character) {
            task = link->task;
            break;
        }
    }
    if (!task)
        return;

    if (deactivate) {
        character->motionFlags &= 0xc0u;
        if (!task->initCallbackFnPtr) {
            task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
            task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        }
        task->mainExecutionIterations = 0;
        return;
    }

    if (task->initCallbackFnPtr) {
        if (task->autoInitTaskCallbackFnPtr && task->mainTaskCallbackFnPtr == std::bit_cast<HLib::TaskCallback>(&HLib_ProcessGenericAutoCleanup)) {
            task->autoInitTaskCallbackFnPtr = task->initCallbackFnPtr;
        } else {
            task->mainTaskCallbackFnPtr = task->initCallbackFnPtr;
        }
        task->initCallbackFnPtr = nullptr;
    }
    character->motionFlags |= 0x3fu;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




void UpdateSndStreaming(CycleManager::NpcRecord &npc) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    if (ctx->filename) {
        if (ctx->primarySoundBankId >= 0) {
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(ctx->primarySoundBankId));
            ctx->primarySoundBankId = -1;
        }
        if (ctx->secondarySoundBankId >= 0) {
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(ctx->secondarySoundBankId));
            ctx->secondarySoundBankId = -1;
        }
        UpdateEntityOverPositionWithFlags(npc, false);
        npcReleaseFace(ctx->obj);
        LoadFile(ctx->filename);
        ctx->filename = nullptr;
    }
    ctx->obj = nullptr;
    ctx->mot = nullptr;
    ctx->loadedBinHandle = nullptr;
    ctx->motnSteeringActive = 0;
}

void LinkCyclesInStack(int32_t enabled) {
    if (enabled) {
        g_cm_unk_flag = 1;
        return;
    }

    g_cm_unk_flag = 0;
    const std::vector<CycleManager::NpcRecord *> loadQueue = CycleManager::SnapshotList(CycleProgramList_Load);
    size_t firstToMove = 0;
    if (!loadQueue.empty() && g_bHasLoadedHuman) {
        loadQueue.front()->cycle->loadQueueDisposition = 2;
        firstToMove = 1;
    }
    for (size_t i = firstToMove; i < loadQueue.size(); ++i) {
        CycleManager::MoveProgramToList(*loadQueue[i], CycleProgramList_Near, CycleListInsert::Head);
    }
}

void ProcessSNDForCycles() {
    if (!CycleManager::IsReady())
        return;

    for (CycleProgramList source : {CycleProgramList_Far, CycleProgramList_Mid}) {
        for (CycleManager::NpcRecord *npc : CycleManager::SnapshotList(source)) {
            CycleManager::UpdateSndStreaming(*npc);
            CycleManager::MoveProgramToList(*npc, CycleProgramList_Near, CycleListInsert::Head);
        }
    }
}

void ProcessSNDForCyclesExtra() {
    if (!CycleManager::IsReady())
        return;

    for (CycleProgramList source : {CycleProgramList_SimSet, CycleProgramList_Promote}) {
        for (CycleManager::NpcRecord *npc : CycleManager::SnapshotList(source)) {
            CycleManager::UpdateSndStreaming(*npc);
            CycleManager::MoveProgramToList(*npc, CycleProgramList_Near, CycleListInsert::Head);
        }
    }

    const std::vector<CycleManager::NpcRecord *> loadQueue = CycleManager::SnapshotList(CycleProgramList_Load);
    if (!loadQueue.empty()) {
        loadQueue.front()->cycle->loadQueueDisposition = 2;
        for (size_t i = 1; i < loadQueue.size(); ++i) {
            CycleManager::MoveProgramToList(*loadQueue[i], CycleProgramList_Near, CycleListInsert::Head);
        }
    }
}






void ProcessCycle(bool processType) {
    if (!CycleManager::IsReady())
        return;

    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::MobjEntry *entry = g_NearQueue; entry;) {
        CycleManager::MobjEntry *const next = entry->nextDormantProgram;
        entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        CycleManager::MoveProgramToList(*CycleManager::EntryRecord(entry), CycleProgramList_Dormant, CycleListInsert::Head);
        entry = next;
    }
    if (processType) {
        for (CycleManager::MobjEntry *entry = g_SimSetHead; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::UpdateSndStreaming(npc);
            entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
            CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
            entry = next;
        }
    }
    for (CycleManager::MobjEntry *entry = g_MidQueue; entry;) {
        CycleManager::MobjEntry *const next = entry->nextDormantProgram;
        CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
        CycleManager::UpdateSndStreaming(npc);
        entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
        entry = next;
    }
    for (CycleManager::MobjEntry *entry = g_FarQueue; entry;) {
        CycleManager::MobjEntry *const next = entry->nextDormantProgram;
        if (processType || entry->ctx->currentOpcode != CycleOpcode_StartLinkedCycle) {
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::UpdateSndStreaming(npc);
            entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
            CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
        }
        entry = next;
    }
    CycleManager::CopyCycleValues();
    std::size_t slot = 0;
    for (uint32_t i = 0; cm.mscrOffset && i < cm.mscrCharaCount && slot < cm.npcRecords.size(); ++i) {
        uint32_t entryOffset = 0;
        if (!ReadValue(cm.bytes, cm.mscrOffset + 4 + i * 4, entryOffset))
            break;
        CycleManager::MobjData *const header = reinterpret_cast<CycleManager::MobjData *>(cm.bytes.data() + cm.mscrOffset + entryOffset);
        g_MOBJ_pData[slot].currentScheduleEntry = reinterpret_cast<CycleTimetableRow *>(reinterpret_cast<char *>(header) + header->currentBranchOfs);
        if (header->programIdSuffix != SHEN_NAME(PRG1))
            continue;
        g_MOBJ_pData[slot].ctx->moveConfig = header;
        g_MOBJ_pData[slot].ctx->steeringWaitFrames = 0;
        ++slot;
    }
    CycleManager::ZeroFields();
    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        if (npc.cycle->currentOpcode == CycleOpcode_StartLinkedCycle && static_cast<int32_t>(npc.cycle->time) >= 86400)
            npc.cycle->time -= 86400;
    }
    CycleManager::ProcessCycleInternal();
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {




void ClearProgramForStackReset(CycleManager::NpcRecord &npc) {
    npc.cycle->primarySoundBankId = -1;
    npc.cycle->secondarySoundBankId = -1;
    npc.cycle->loadedBinHandle = nullptr;
    npc.cycle->filename = nullptr;
    npc.cycle->motnSteeringActive = false;
    npc.cycle->doorAnimActive = 0;
    npc.cycle->xmptResumePending = 0;
    npc.cycle->obj = nullptr;
    npc.cycle->mot = nullptr;
    npc.cycle->bikeClearanceBlocked = false;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




void ResetStackLists() {
    for (CycleManager::NpcRecord *npc : CycleManager::SnapshotList(CycleProgramList_Running)) {
        CycleManager::ClearProgramForStackReset(*npc);
        CycleManager::MoveProgramToList(*npc, CycleProgramList_Dormant, CycleListInsert::Head);
        CycleManager::WriteCommandDword(npc->moveConfigOffset, 0);
    }

    for (CycleProgramList source : {CycleProgramList_SimSet, CycleProgramList_Mid, CycleProgramList_Far, CycleProgramList_Promote, CycleProgramList_Load}) {
        for (CycleManager::NpcRecord *npc : CycleManager::SnapshotList(source)) {
            CycleManager::ClearProgramForStackReset(*npc);
            CycleManager::MoveProgramToList(*npc, CycleProgramList_Near, CycleListInsert::Head);
        }
    }
}
} // namespace CycleManager





bool PerformAicaSoundUpdates(CycleManager::NpcRecord &npc, const Vector3 &target, int16_t targetYaw, bool commit) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    npc.cycle->flags &= ~CycleFlag_CheckForwardIntersectionRay;
    const float speed = npc.cycle->baseMoveSpeed;
    npc.cycle->moveTargetPos = target;
    npc.cycle->moveTargetYaw = targetYaw;
    npc.cycle->approachStartSpeed = speed;
    Vector3 delta{npc.cycle->pos.x - npc.cycle->moveTargetPos.x, npc.cycle->pos.y - npc.cycle->moveTargetPos.y, npc.cycle->pos.z - npc.cycle->moveTargetPos.z};
    float distance = 0.0f;
    if (delta.x > epsilon || delta.x < -epsilon || delta.z > epsilon || delta.z < -epsilon)
        distance = Vector3::NormalizeInPlace(delta);
    bool approaching = false;
    if (speed > distance) {
        CycleManager::FinishTargetApproachAndStopSounds(npc);
    } else {
        npc.cycle->workFloat4C = distance;
        approaching = CycleManager::TickTargetApproachWithSound(npc);
    }
    if (commit && npc.cycle->moveConfig->cycleClassCode == 6u) {
        const uint32_t area = CycleManager::CurrentAreaId();
        if (area == SHEN_NAME(D000) || area == SHEN_NAME(MFSY)) {
            Execute3DSoundCommand(area == SHEN_NAME(D000) ? 0x2205a9u : 0x5805a9u, 4u, &npc.cycle->pos, 6.0f, 60.0f, 0xffffffffu);
            Execute3DSoundCommand(area == SHEN_NAME(D000) ? 0x2505a9u : 0x5b05a9u, 4u, &npc.cycle->pos, 6.0f, 60.0f, 0xffffffffu);
        }
    }
    if (npc.cycle->motnSteeringActive) {
        if (npc.cycle->primarySoundBankId == -1)
            npc.cycle->primarySoundBankId = CycleManager::GetBikeSoundsForArea(npc);
    } else if (npc.cycle->primarySoundBankId >= 0) {
        ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
        npc.cycle->primarySoundBankId = -1;
    }
    npc.cycle->secondarySoundBankId = -1;
    return approaching;
}

} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void UpdateBikeReverseAvoidance(CycleManager::NpcRecord &npc, float observedSpeed, bool doForwardConeCheck);

void AdjustDrivePathForClearance(CycleManager::NpcRecord &npc);




bool IsCurrentTimetableRow(const CycleTimetableRow *row, int32_t time) {
    const int32_t *const command = reinterpret_cast<const int32_t *>(CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset)));
    if (row->timeOrEndMarker + 300 > time)
        return true;
    if (command[0] != CycleOpcode_StartLinkedCycle)
        return false;
    const int32_t endTime = command[2] >= 0 ? command[2] : row->timeOrEndMarker - command[2];
    return endTime > time;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {



void ZeroFields() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::MNRBEntry &entry : cm.mnrbEntries) {
        entry.reservationCount = 0;
        entry.reservationHead = nullptr;
    }
    if (!cm.mnrbReservationPoolAvailable)
        return;
    for (CycleManager::MNRBEntry::Reservation &node : cm.mnrbReservationPool) {
        node.prev = nullptr;
        node.next = nullptr;
        node.owner = nullptr;
        node.seatOrderKey = 0;
    }
}




void UpdateCurrentCursor(CycleManager::MobjCycle *cycle) {
    cycle->primarySoundBankId = -1;
    cycle->secondarySoundBankId = -1;
    cycle->filename = nullptr;
    cycle->loadedBinHandle = nullptr;
    cycle->motnSteeringActive = false;
    cycle->doorAnimActive = 0;
    cycle->xmptResumePending = 0;
    cycle->obj = nullptr;
    cycle->mot = nullptr;
    cycle->bikeClearanceBlocked = 0;
    cycle->currentAnimToken = 0;
    cycle->animHandleActive = 0;
    cycle->create_chara_name[0] = 0;
    cycle->proximityGroup = 0;
    for (CycleManager::TaggedPayload *payload = cycle->taggedPayloadList; payload;) {
        CycleManager::TaggedPayload *const next = payload->next;
        payload->next = g_CycleManager_Current;
        g_CycleManager_Current = payload;
        payload = next;
    }
    cycle->taggedPayloadList = nullptr;
    if (CycleMoveObjectRuntime *const runtime = cycle->moveObjRuntime) {
        runtime->next = g_freeMoveObjects;
        cycle->moveObjRuntime = nullptr;
        g_freeMoveObjects = runtime;
    }
}






void ProcessCycleInternal() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::ShutRow &row : cm.shutRows)
        row.lastDispatchTime = -1;
    CycleManager::CopyCycleValues();
    const int32_t time = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
    for (CycleManager::MobjEntry *entry = g_DormantPrograms; entry;) {
        CycleManager::MobjCycle *const ctx = entry->ctx;
        CycleManager::MobjEntry *const next = entry->nextDormantProgram;
        CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
        CycleTimetableRow *row = reinterpret_cast<CycleTimetableRow *>(reinterpret_cast<char *>(ctx->moveConfig) + ctx->moveConfig->currentBranchOfs);
        entry->currentScheduleEntry = row;
        for (CycleManager::TaggedPayload *payload = ctx->taggedPayloadList; payload;) {
            CycleManager::TaggedPayload *const following = payload->next;
            payload->next = g_CycleManager_Current;
            g_CycleManager_Current = payload;
            payload = following;
        }
        ctx->taggedPayloadList = nullptr;
        if (CycleMoveObjectRuntime *const runtime = ctx->moveObjRuntime) {
            runtime->next = g_freeMoveObjects;
            ctx->moveObjRuntime = nullptr;
            g_freeMoveObjects = runtime;
        }
        ctx->trackedMobj = nullptr;
        if (row) {
            int32_t rowTime = row->timeOrEndMarker;
            if (rowTime < time) {
                while (rowTime != -1) {
                    ctx->branchResumeCommandPtr = nullptr;
                    ctx->cycleCommandPtr = CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset));
                    if (CycleManager::IsCurrentTimetableRow(row, time)) {
                        entry->currentScheduleEntry = row;
                        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                        MOBJ_executeTask(entry, time, 0, 0);
                        ++row;
                        break;
                    }
                    int32_t programTime = rowTime;
                    while (CycleManager::ExecuteProgramStep(ctx, time, &programTime))
                        ;
                    rowTime = row[1].timeOrEndMarker;
                    ++row;
                    if (rowTime >= time)
                        break;
                }
            }
            if (ctx->currentOpcode != CycleOpcode_RemoveCharacter)
                CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
            entry->currentScheduleEntry = row;
        }
        entry = next;
    }
}






void Update(CycleManager::MobjEntry *currEntry) {
    const int32_t time = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
    if (!currEntry)
        return;
    CycleManager::MobjCycle *const ctx = currEntry->ctx;
    CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(currEntry);
    if (ctx->currentOpcode == CycleOpcode_UseMnrBEntry) {
        if (ctx->morsEntryOffset && ctx->morsQueueNodeOffset) {
            CycleManager::ReleaseMNRBReservation(npc, CycleManager::FindMNRBEntryByLineOffset(ctx->morsEntryOffset));
            ctx->morsEntryOffset = 0;
        }
        ctx->morsQueueNodeOffset = 0;
    }
    ctx->currentOpcode = CycleOpcode_RemoveCharacter;
    CycleTimetableRow *row = reinterpret_cast<CycleTimetableRow *>(reinterpret_cast<char *>(ctx->moveConfig) + ctx->moveConfig->currentBranchOfs);
    currEntry->currentScheduleEntry = row;
    if (!row)
        return;
    int32_t rowTime = row->timeOrEndMarker;
    if (rowTime < time) {
        while (rowTime != -1) {
            ctx->branchResumeCommandPtr = nullptr;
            ctx->cycleCommandPtr = CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset));
            if (CycleManager::IsCurrentTimetableRow(row, time)) {
                currEntry->currentScheduleEntry = row;
                MOBJ_executeTask(currEntry, time, 0, 0);
            } else {
                int32_t programTime = rowTime;
                while (CycleManager::ExecuteProgramStep(ctx, time, &programTime))
                    ;
            }
            rowTime = row[1].timeOrEndMarker;
            ++row;
            if (rowTime >= time)
                break;
        }
    }
    if (ctx->currentOpcode != CycleOpcode_RemoveCharacter)
        CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
    currEntry->currentScheduleEntry = row;
}

} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

int32_t UpdateClass6DoorCounter(int32_t counter, int32_t step, bool opening) {
    if (step == 0)
        step = 1;
    const int64_t next = static_cast<int64_t>(counter) + (opening ? static_cast<int64_t>(step) : -static_cast<int64_t>(step));
    return static_cast<int32_t>(std::clamp<int64_t>(next, 0, 500));
}

float Class6DoorBlend(int32_t counter) {
    const uint16_t phase = static_cast<uint16_t>(static_cast<int32_t>(static_cast<float>(counter) * 65.536003f));
    const int32_t folded = phase <= 0x8000u ? static_cast<int32_t>(phase) : -static_cast<int32_t>(static_cast<int16_t>(phase));
    return (1.0f - sin_fixed16(static_cast<uint16_t>(0x4000 - folded))) * 0.5f;
}

void UpdateDobuitaContainment() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    cm.isWithinDobuita = false;
    if (CycleManager::CurrentAreaId() != SHEN_NAME(D000))
        return;

    const Vector3 playerPosition = CycleManager::CurrentRyoPosition();
    uint8_t *const table =
        cm.schedule.empty() ? nullptr : CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(CPDA), 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    if (!table || cm.bytes.empty()) {
        cm.isWithinDobuita = true;
        return;
    }

    const uintptr_t begin = reinterpret_cast<uintptr_t>(cm.bytes.data());
    const uintptr_t address = reinterpret_cast<uintptr_t>(table);
    if (address < begin || address >= begin + cm.bytes.size()) {
        cm.isWithinDobuita = true;
        return;
    }

    size_t cursor = static_cast<size_t>(address - begin);
    bool insideAnyQuad = false;
    for (size_t row = 0; row < 1024u && cursor <= cm.bytes.size() && 32u <= cm.bytes.size() - cursor; ++row, cursor += 32u) {
        std::array<float, 8> values{};
        bool valid = true;
        for (size_t word = 0; word < values.size(); ++word)
            valid = valid && ReadValue(cm.bytes, cursor + word * sizeof(float), values[word]);
        if (!valid)
            break;
        if (std::fabs(values[0]) < 0.00000011920929f && std::fabs(values[1]) < 0.00000011920929f && std::fabs(values[2]) < 0.00000011920929f &&
            std::fabs(values[3]) < 0.00000011920929f) {
            break;
        }

        const std::array<Vector3, 4> corners{
            Vector3{values[0], 0.0f, values[1]}, Vector3{values[2], 0.0f, values[3]}, Vector3{values[4], 0.0f, values[5]}, Vector3{values[6], 0.0f, values[7]}};
        size_t corner = 0;
        for (; corner < corners.size(); ++corner) {
            const Vector3 &current = corners[corner];
            const Vector3 &next = corners[(corner + 1u) & 3u];
            const float cross = (playerPosition.x - current.x) * (next.z - current.z) - (playerPosition.z - current.z) * (next.x - current.x);
            if (cross < 0.0f)
                break;
        }
        insideAnyQuad = insideAnyQuad || corner == corners.size();
    }
    cm.isWithinDobuita = !insideAnyQuad;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {




CycleManager::MobjEntry *SortLinkedListByDistance(CycleManager::MobjEntry *wk) {
    CycleManager::MobjEntry *first = wk;
    if (!wk || !wk->nextDormantProgram)
        return first;
    const uint16_t yaw = static_cast<uint16_t>(g_fCamAccumYaw);
    const uint16_t folded = yaw <= 0x8000u ? yaw : static_cast<uint16_t>(-yaw);
    const float x = sin_fixed16(yaw) * 3.0f + g_tmp_cycle_v3.x;
    const float y = g_tmp_cycle_v3.y;
    const float z = g_tmp_cycle_v3.z - sin_fixed16(static_cast<uint16_t>(0x4000 - folded)) * 3.0f;
    for (CycleManager::MobjEntry *entry = first; entry; entry = entry->nextDormantProgram) {
        CycleManager::MobjCycle *const npc = entry->ctx;
        const int32_t classId = static_cast<int32_t>(npc->moveConfig->cycleClassCode);
        const float distance = (npc->pos.x - x) * (npc->pos.x - x) + (npc->pos.y - y) * (npc->pos.y - y) + (npc->pos.z - z) * (npc->pos.z - z);
        if (classId == 6)
            npc->cameraSortDistanceSq = 0.5f;
        else if (classId == 4 || classId == 5 || classId == 7)
            npc->cameraSortDistanceSq = npc->currentOpcode != CycleOpcode_DriveVehiclePath2D && distance < 225.0f ? 1.0f : distance;
        else
            npc->cameraSortDistanceSq = distance;
    }
    CycleManager::MobjEntry *const before = first->prev_or_backlink;
    const CycleManager::MobjCycle *best = first->ctx;
    for (CycleManager::MobjEntry *it = first->nextDormantProgram; it; it = it->nextDormantProgram) {
        if (!(best->cameraSortDistanceSq > it->ctx->cameraSortDistanceSq))
            continue;
        CycleManager::MobjEntry *const itPrev = it->prev_or_backlink;
        CycleManager::MobjEntry *const old = first;
        CycleManager::MobjEntry *const oldNext = first->nextDormantProgram;
        first = it;
        if (old == itPrev) {
            old->prev_or_backlink = it;
        } else {
            old->prev_or_backlink = itPrev;
            itPrev->nextDormantProgram = old;
        }
        CycleManager::MobjEntry *const itNext = it->nextDormantProgram;
        old->nextDormantProgram = itNext;
        if (itNext)
            itNext->prev_or_backlink = old;
        it->prev_or_backlink = before;
        if (it == oldNext) {
            it->nextDormantProgram = old;
        } else {
            it->nextDormantProgram = oldNext;
            oldNext->prev_or_backlink = it;
        }
        best = it->ctx;
        it = old;
    }
    return first;
}
namespace CycleManager {




CycleManager::MobjEntry *SortSimSetByCameraDistance() {
    CycleManager::MobjEntry *head = g_SimSetHead;
    if (!head)
        return head;
    CycleManager::MobjEntry *const second = head->nextDormantProgram;
    if (head == second || !second)
        return head;
    const uint16_t yaw = static_cast<uint16_t>(g_fCamAccumYaw);
    const uint16_t folded = yaw <= 0x8000u ? yaw : static_cast<uint16_t>(-yaw);
    const float x = sin_fixed16(yaw) * 3.0f + g_tmp_cycle_v3.x;
    const float y = g_tmp_cycle_v3.y;
    const float z = g_tmp_cycle_v3.z - sin_fixed16(static_cast<uint16_t>(0x4000 - folded)) * 3.0f;
    for (CycleManager::MobjEntry *entry = head; entry; entry = entry->nextDormantProgram) {
        CycleManager::MobjCycle *const npc = entry->ctx;
        const int32_t classId = static_cast<int32_t>(npc->moveConfig->cycleClassCode);
        const float distance = (npc->pos.x - x) * (npc->pos.x - x) + (npc->pos.y - y) * (npc->pos.y - y) + (npc->pos.z - z) * (npc->pos.z - z);
        if (classId == 6)
            npc->cameraSortDistanceSq = 0.5f;
        else if (classId == 4 || classId == 5 || classId == 7)
            npc->cameraSortDistanceSq = npc->currentOpcode != CycleOpcode_DriveVehiclePath2D && distance < 225.0f ? 1.0f : distance;
        else
            npc->cameraSortDistanceSq = distance;
    }
    int32_t pass = 0;
    CycleManager::MobjEntry *cur = head;
    do {
        const CycleManager::MobjCycle *best = cur->ctx;
        for (CycleManager::MobjEntry *it = cur->nextDormantProgram; it; it = it->nextDormantProgram) {
            if (!(best->cameraSortDistanceSq > it->ctx->cameraSortDistanceSq))
                continue;
            CycleManager::MobjEntry *const itPrev = it->prev_or_backlink;
            CycleManager::MobjEntry *const old = cur;
            CycleManager::MobjEntry *const oldPrev = cur->prev_or_backlink;
            CycleManager::MobjEntry *const oldNext = cur->nextDormantProgram;
            cur = it;
            if (old == itPrev) {
                old->prev_or_backlink = it;
            } else {
                old->prev_or_backlink = itPrev;
                itPrev->nextDormantProgram = old;
            }
            CycleManager::MobjEntry *const itNext = it->nextDormantProgram;
            old->nextDormantProgram = itNext;
            if (itNext)
                itNext->prev_or_backlink = old;
            it->prev_or_backlink = oldPrev;
            if (oldPrev)
                oldPrev->nextDormantProgram = it;
            if (it == oldNext) {
                it->nextDormantProgram = old;
            } else {
                it->nextDormantProgram = oldNext;
                oldNext->prev_or_backlink = it;
            }
            best = it->ctx;
            it = old;
            if (pass == 0)
                head = cur;
        }
        cur = cur->nextDormantProgram;
        ++pass;
    } while (cur);
    return head;
}

void UpdateCharaCycles() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (g_bDebugSkipCyclesForCharacters)
        return;
    int32_t maxEntitiesToSimulate = g_MaxEntitiesPerType;
    if (MaxNearNPCs < g_MaxEntitiesPerType)
        maxEntitiesToSimulate = MaxNearNPCs;
    if (g_prevCycleSeconds - 21600u > 46800u)
        maxEntitiesToSimulate = IsDBYO;
    g_tmp_cycle_v3 = g_CameraEyePos;
    const float radiusSquared = NPCDisplayRadius * NPCDisplayRadius;
    g_lastProximityGroup = -1;

    CycleManager::ResetPublishedSceneState();


    g_dynamicTurnCollisionVolumeCount = 0;
    CycleManager::AppendParkedVehicleSteeringVolumes();


    MOBJ_UpdateTransformsRecursively_0();
    CycleManager::CollectCollisionObjects();
    CycleManager::UpdateDobuitaContainment();
    int32_t activeCycleCount = numCharactersSimulated + g_farQueueCycleCount + g_perFrameNpcWorkCnt + g_midQueueCycleCount + g_promoteListCycleCount;
    numCharactersSimulated = 0;
    g_dormantProgramVisitCount = 0;
    g_nearQueueCycleCount = 0;
    g_promoteListCycleCount = 0;
    g_midQueueCycleCount = 0;
    g_farQueueCycleCount = 0;
    g_runningProgramCount = 0;
    cm.umbrellaRainActive = Weather_ProcessRain();
    const MapControlData *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const ScenarioTaskData *const scenario =
        control && control->scenarioTask ? reinterpret_cast<const ScenarioTaskData *>(control->scenarioTask->task_data_ptr) : nullptr;
    const bool globalSimulation = CycleManager::IsGlobalSimulationEnabled(scenario ? scenario->scenarioFunctions : nullptr);
    Character *const mapRyo = CycleManager::MapRyo();
    if (g_cyclTalkCycleActive) {
        g_cyclTalkCycleActive = TALK(mapRyo);
        if (!globalSimulation || !g_cyclTalkCycleActive || ShouldRun()) {
            g_shouldComputeBetweenTwoEntities = 0;
            g_cyclTalkCycleActive = 0;
        }
    } else if (globalSimulation && !g_bGlobalFlag && CycleManager::CurrentAreaId() == SHEN_NAME(MFSY)) {
        g_cyclTalkCycleActive = CycleManager::BeginRyoTalkWalk(mapRyo);
        if (g_cyclTalkCycleActive)
            g_shouldComputeBetweenTwoEntities = 1;
    }
    cm.center = CycleManager::CurrentRyoPosition();
    ++cm.updateFrames;
    if (cm.updateFrames == 1) {
        CycleManager::BindSHUTRowsToSceneObjects();
    }

    if (!cm.programs.empty()) {

        const int32_t currentDay = g_currentDateTime.day;
        const int32_t time = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
        if (g_tmp_new_day != currentDay && !g_LoadQueue) {
            g_tmp_new_day = static_cast<std::uint8_t>(currentDay);
            CycleManager::ProcessCycle(false);
        }
        g_prevCycleSeconds = static_cast<uint32_t>(time);
        ++cm.npcAnimSyncEpoch;
        if (!cm.npcAnimSyncEpoch)
            ++cm.npcAnimSyncEpoch;

        // Dormant programs start the timetable row whose time has passed, placed out of sight until it moves them; a program that
        // runs on goes near.
        for (CycleManager::MobjEntry *entry = g_DormantPrograms; entry;) {
            CycleTimetableRow *const row = entry->currentScheduleEntry;
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            if (row && row->timeOrEndMarker < time && row->timeOrEndMarker != -1) {
                entry->currentScheduleEntry = row + 1;
                entry->ctx->cycleCommandPtr = CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset));
                entry->ctx->branchResumeCommandPtr = nullptr;
                entry->ctx->pos = {10000.0f, 0.0f, 10000.0f};
                if (MOBJ_executeTask(entry, time, 0, 0))
                    CycleManager::MoveProgramToList(*CycleManager::EntryRecord(entry), CycleProgramList_Near, CycleListInsert::Head);
            }
            ++g_dormantProgramVisitCount;
            entry = next;
        }

        // Materialized programs out of the simulated set run without their characters; one that may be shown is promoted (or, a
        // vehicle blocked near the camera, held in the mid queue), one out of view goes near, and an ended one dormant.
        const float taskDistanceThreshold = g_perFrameNpcWorkCnt < 0 ? 25.0f : g_perFrameNpcWorkCnt <= 1 ? 225.0f : g_perFrameNpcWorkCnt <= 3 ? 100.0f : 25.0f;
        g_PromoteList = nullptr;
        for (CycleManager::MobjEntry *entry = g_FarQueue; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::MobjCycle *const ctx = entry->ctx;
            ++g_farQueueCycleCount;
            if (!MOBJ_executeTask(entry, time, 0, 0)) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
                CycleManager::UpdateSndStreaming(npc);
                entry = next;
                continue;
            }
            if (ctx->obj)
                ctx->obj->entity.node.position = ctx->pos;
            if (ctx->modeValue != 1u)
                ctx->flags &= ~2u;
            const bool blocked = UpdateNpcSteeringFromMOTN(npc, false);
            bool nearShown = !shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->range) || !shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range);
            for (CycleManager::MobjEntry *shown = g_SimSetHead; shown && !nearShown; shown = shown->nextDormantProgram)
                nearShown = shown->ctx->motnSteeringActive && CycleManager::DistanceSquared(ctx->pos, shown->ctx->pos) < 0.5776f;
            if (!nearShown && CycleManager::DistanceSquared(ctx->pos, cm.center) >= 0.5776f && !blocked) {
                // A talk-mover waits for its partner to be shown.
                const CycleManager::NpcRecord *const partner =
                    ctx->currentOpcode == CycleOpcode_BeginTalkMoveToTaggedPosition ? CycleManager::GetTrackedMobj(npc) : nullptr;
                if (partner) {
                    bool partnerShown = false;
                    for (CycleManager::MobjEntry *shown = g_SimSetHead; shown && !partnerShown; shown = shown->nextDormantProgram)
                        partnerShown = shown->ctx == partner->cycle;
                    if (!partnerShown) {
                        entry = next;
                        continue;
                    }
                }
                const uint32_t classId = ctx->moveConfig->cycleClassCode;
                bool crowded = false;
                if (((classId - 4u) & 0xFFFFFFFCu) == 0 && classId != 6u) {
                    for (CycleManager::MobjEntry *shown = g_SimSetHead; shown && !crowded; shown = shown->nextDormantProgram)
                        crowded = shown->ctx->motnSteeringActive && CycleManager::DistanceSquared(ctx->pos, shown->ctx->pos) < 9.0f;
                }
                if (!crowded)
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Promote, CycleListInsert::Head);
                entry = next;
                continue;
            }
            if (taskDistanceThreshold > CycleManager::DistanceSquared(ctx->pos, g_tmp_cycle_v3)) {
                const uint32_t classId = ctx->moveConfig->cycleClassCode;
                if (blocked && ((classId - 4u) & 0xFFFFFFFCu) == 0 && classId != 6u)
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Mid, CycleListInsert::Head);
            } else if (!shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->dist) || !shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range)) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
                CycleManager::UpdateSndStreaming(npc);
            }
            entry = next;
        }

        // The load queue's head owns the one human load. A head that stopped, left the view or was redirected drops a load in flight (a
        // pack not yet valid is waited for) and goes running for the script (disposition 1, without its character), dormant or near. A
        // head in view starts loading its human pack when both heaps hold a larger free block than the pack needs; once the pack is valid
        // the head takes it and its character, goes far, and is set up hidden at its cycle position. The rest of the queue, nearest the
        // camera first, runs without characters.
        g_perFrameNpcWorkCnt = 0;
        if (CycleManager::MobjEntry *const head = g_LoadQueue) {
            CycleManager::MobjEntry *const next = head->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(head);
            CycleManager::MobjCycle *const ctx = head->ctx;
            const bool running = MOBJ_executeTask(head, time, 0, 0) != 0;
            const bool shown =
                running && shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->dist) && shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range);
            PackSlot *const loaded = g_bHasLoadedHuman ? g_pszTempLoadedHumanSched : nullptr;
            if (loaded && !PACK_IsValidSlot(loaded)) {
                // The pack in flight is waited for.
            } else if (!shown || ctx->loadQueueDisposition) {
                if (ctx->loadQueueDisposition == 1) {
                    ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Running, CycleListInsert::Head);
                } else {
                    CycleManager::MoveProgramToList(npc, running ? CycleProgramList_Near : CycleProgramList_Dormant, CycleListInsert::Head);
                }
                ctx->loadQueueDisposition = 0;
                if (loaded) {
                    LoadFile(loaded);
                    g_pszTempLoadedHumanSched = nullptr;
                    g_bHasLoadedHuman = 0;
                }
            } else if (!loaded) {
                if (!PACK_HasValidPack()) {
                    const int32_t humansIdx = ctx->humansIdx;
                    const uint32_t *const humansSizes = reinterpret_cast<const uint32_t *>(&HumansIndexTable->name) + HumansIndexTable->numCharas;
                    const uint64_t size = HumansIndexTable->numCharas <= humansIdx ? 164864u : humansSizes[humansIdx] + 3096u;
                    if (CycleManager::LargestFreeTaskHeapBlock() > size && static_cast<uint32_t>(HLib_GetLargestMallocFreeBlock()) > size) {
                        g_pszTempLoadedHumanSched =
                            PACK_LoadHumanByIndex(HUMANS_AFS_Ptr, static_cast<uint32_t>(2 * humansIdx), static_cast<uint32_t>(2 * humansIdx + 1));
                        if (g_pszTempLoadedHumanSched) {
                            g_bHasLoadedHuman = 1;
                        }
                    }
                }
            } else {
                ctx->filename = loaded;
                g_pszTempLoadedHumanSched = nullptr;
                ctx->obj = CycleManager::FindCharacter(ctx->moveConfig->obj_id.name);
                if (!ctx->obj) {
                    uint32_t index = g_TotalActiveCycles;
                    if (index >= 10u)
                        index = 0;
                    g_CycleTable[2u * index] = ctx->moveConfig->obj_id.name;
                    g_TotalActiveCycles = index + 1;
                }
                CycleManager::MoveProgramToList(npc, CycleProgramList_Far, CycleListInsert::Head);
                // Vehicles: node 196 is cleared unless the vehicle drives its path between 07:00:01 and 17:29:59, and (but for class 6)
                // node 195 always.
                const uint32_t classId = ctx->moveConfig->cycleClassCode;
                Mt5RuntimeNode *const model = classId >= 4u && classId <= 7u && ctx->obj ? static_cast<Mt5RuntimeNode *>(ctx->obj->model) : nullptr;
                if (model && (ctx->currentOpcode != CycleOpcode_DriveVehiclePath2D || static_cast<uint32_t>(time - 25201) <= 37798u)) {
                    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, 196))
                        node->sceneBindingOffset = 0;
                }
                if (model && classId != 6u) {
                    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, 195))
                        node->sceneBindingOffset = 0;
                }
                if (ctx->primarySoundBankId == -1)
                    ctx->primarySoundBankId = CycleManager::GetBikeSoundsForArea(npc);
                ctx->mot = MOTN_getMOTM(ctx->obj);
                if (ctx->mot)
                    CycleManager::CreateCharacter(npc);
                std::memcpy(g_cycle_file_str, &ctx->moveConfig->obj_id.name, 4);
                ctx->loadedBinHandle = LoadFileIntoHeapMem(ctx->filename, g_cycle_file_str, make_shen_name('B', 'I', 'N', ' '));
                npcSetupFace(ctx);
                if (ctx->pFace) {
                    FACE_UpdateExpression(ctx->obj, 0, 0, 0);
                    ctx->faceNearActive = 0;
                }
                if (ctx->obj) {
                    CLMD_SetFlags(ctx->obj, 8, 1);
                    ccowDisableCharaFlags(ctx->obj, 7);
                    shenApplyTranslationToAll(ctx->obj->entity.node, ctx->pos);
                    CycleManager::SetNodeChainYaw(*ctx->obj, ctx->desiredPathYaw);
                }
                AttachEntityAndInitCycle(ctx->obj, true);
                g_bHasLoadedHuman = 0;
            }
            ++g_perFrameNpcWorkCnt;
            if (next) {
                CycleManager::MobjEntry *sorted = nullptr;
                if (g_LoadQueue == next) {
                    sorted = SortLinkedListByDistance(next);
                    g_LoadQueue = sorted;
                } else {
                    sorted = SortLinkedListByDistance(next);
                    g_LoadQueue->nextDormantProgram = sorted;
                }
                for (CycleManager::MobjEntry *entry = sorted; entry;) {
                    CycleManager::MobjEntry *const after = entry->nextDormantProgram;
                    CycleManager::NpcRecord &queued = *CycleManager::EntryRecord(entry);
                    if (!MOBJ_executeTask(entry, time, 0, 0)) {
                        CycleManager::MoveProgramToList(queued, CycleProgramList_Dormant, CycleListInsert::Head);
                    } else if (!shenIsLocalSphereInsideViewCullVolume(entry->ctx->pos, entry->ctx->dist) ||
                        !shenIsLocalSphereVisibleInScreenDepthTable(entry->ctx->pos, entry->ctx->range)) {
                        CycleManager::MoveProgramToList(queued, CycleProgramList_Near, CycleListInsert::Head);
                    }
                    ++g_perFrameNpcWorkCnt;
                    entry = after;
                }
            }
        }

        // Near programs run without characters. While loading is allowed, one in the area within the display radius and in view
        // takes its character if the scene has it (materialized), else joins the load queue, up to 12 active cycles.
        if (!g_scntReferencedCycleHeld) {
            for (CycleManager::MobjEntry *entry = g_NearQueue; entry;) {
                CycleManager::MobjEntry *const next = entry->nextDormantProgram;
                CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
                CycleManager::MobjCycle *const ctx = entry->ctx;
                if (!MOBJ_executeTask(entry, time, 0, 0)) {
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
                } else if (g_cm_unk_flag && ctx->areaID.name == CycleManager::CurrentAreaId()) {
                    const float radius = ctx->moveConfig->cycleClassCode <= 3u ? radiusSquared : 6400.0f;
                    if (radius > CycleManager::DistanceSquared(ctx->pos, g_tmp_cycle_v3) && shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->dist) &&
                        shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range) && activeCycleCount < 12) {
                        ctx->obj = CycleManager::FindCharacter(ctx->moveConfig->obj_id.name);
                        if (ctx->obj) {
                            ctx->mot = MOTN_getMOTM(ctx->obj);
                            CycleManager::MoveProgramToList(npc, CycleProgramList_Far, CycleListInsert::Head);
                            ctx->filename = nullptr;
                        } else {
                            CycleManager::MoveProgramToList(npc, CycleProgramList_Load, CycleListInsert::Tail);
                            ++activeCycleCount;
                        }
                    }
                }
                ++g_nearQueueCycleCount;
                entry = next;
            }
        }

        // The simulated set: each runs live, or held by the running conversation. One that leaves the area, the view and the display
        // radius (unless still visible, held or near Ryo) is demoted; one that dropped more than 2 under its steering target goes near.
        g_scntReferencedCycleHeld = 0;
        const ScenarioFunctionWork *const scnf = scenario ? scenario->scenarioFunctions : nullptr;
        for (CycleManager::MobjEntry *entry = g_SimSetHead; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::MobjCycle *const ctx = entry->ctx;
            npc.skipBlend = CycleManager::SimSetSkipBlend(npc, scnf, globalSimulation);
            ++numCharactersSimulated;
            if (!MOBJ_executeTask(entry, time, ctx->motnSteeringActive, npc.skipBlend ? 1u : 0u)) {
                if (ctx->currentOpcode != CycleOpcode_DoorUse) {
                    if (ctx->loadQueueDisposition == 1) {
                        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                        CycleManager::MoveProgramToList(npc, CycleProgramList_Running, CycleListInsert::Head);
                    } else {
                        CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
                    }
                    ctx->loadQueueDisposition = 0;
                    CycleManager::UpdateSndStreaming(npc);
                }
                entry = next;
                continue;
            }

            if (!ctx->obj) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
                CycleManager::UpdateSndStreaming(npc);
                entry = next;
                continue;
            }
            const float radius = ctx->moveConfig->cycleClassCode <= 3u ? radiusSquared : 3600.0f;
            const bool inArea = ctx->areaID.name == CycleManager::CurrentAreaId();
            const bool keep =
                (inArea && shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->range) && shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range) &&
                    (radius > CycleManager::DistanceSquared(ctx->pos, g_tmp_cycle_v3) || ctx->obj->renderOpacity > 0.0f)) ||
                npc.skipBlend || CycleManager::DistanceSquared(ctx->pos, cm.center) < 16.0f;
            if (keep) {
                if (ctx->prevMotnSteeringActive && std::fabs(ctx->pos.y - ctx->steeringTarget.y) > 2.0f) {
                    ctx->motnSteeringActive = 0;
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
                    CycleManager::UpdateSndStreaming(npc);
                }
            } else if (static_cast<uint16_t>(ctx->currentOpcode - CycleOpcode_RequestNearestDoorTransition) > 1u) {
                if (ctx->loadQueueDisposition == 1) {
                    ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Running, CycleListInsert::Head);
                    ctx->loadQueueDisposition = 0;
                    CycleManager::UpdateSndStreaming(npc);
                } else if (ctx->loadQueueDisposition != 0 || !inArea) {
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Near, CycleListInsert::Head);
                    ctx->loadQueueDisposition = 0;
                    CycleManager::UpdateSndStreaming(npc);
                } else {
                    CycleManager::MoveProgramToList(npc, CycleProgramList_Far, CycleListInsert::Head);
                    AttachEntityAndInitCycle(ctx->obj, true);
                }
            }
            entry = next;
        }

        // The simulated set, nearest the camera first, is shown up to the entity budget: a hidden one appears (PlayAnim_Wrapper_1) where
        // no shown cycle stands, one past the budget fades out, a held one is always shown. Shown riders publish their vehicles, and a
        // shown face looks near the camera.
        g_SimSetHead = CycleManager::SortSimSetByCameraDistance();
        numCharactersSimulated = 0;
        for (CycleManager::MobjEntry *entry = g_SimSetHead; entry; entry = entry->nextDormantProgram) {
            if (entry->ctx->motnSteeringActive || entry->ctx->xm_resume_state)
                ++numCharactersSimulated;
        }
        int32_t index = 0;
        for (CycleManager::MobjEntry *entry = g_SimSetHead; entry; entry = entry->nextDormantProgram, ++index) {
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::MobjCycle *const ctx = entry->ctx;
            const uint8_t steering = ctx->motnSteeringActive;
            const bool hidden = !steering && !ctx->xm_resume_state;
            bool appear = false;
            bool restoreOpacity = false;
            if (hidden) {
                appear = numCharactersSimulated < maxEntitiesToSimulate && !ctx->bikeClearanceBlocked;
            } else if ((index >= maxEntitiesToSimulate || ctx->bikeClearanceBlocked) && !ctx->xm_resume_state) {
                const float opacity = ctx->obj->renderOpacity - 0.1f;
                if (opacity < 0.0f) {
                    ctx->motnSteeringActive = 0;
                    AttachEntityAndInitCycle(ctx->obj, true);
                }
                ctx->obj->renderOpacity = opacity;
            } else if (ctx->xm_resume_state && !steering) {
                appear = true;
                restoreOpacity = true;
            }
            if (appear) {
                const uint32_t flags = ctx->flags;
                if (restoreOpacity || ctx->modeValue != 1u)
                    ctx->flags &= ~2u;
                const bool blocked = UpdateNpcSteeringFromMOTN(npc, false);
                if (restoreOpacity || ctx->modeValue != 1u)
                    ctx->flags = (ctx->flags & ~2u) | (flags & 2u);
                bool occupied = blocked;
                for (CycleManager::MobjEntry *shown = g_SimSetHead; shown && !occupied; shown = shown->nextDormantProgram)
                    occupied = shown->ctx->motnSteeringActive && CycleManager::DistanceSquared(ctx->pos, shown->ctx->pos) < 0.5776f;
                if (!occupied) {
                    if (restoreOpacity)
                        ctx->obj->renderOpacity = 1.0f;
                    else
                        ++numCharactersSimulated;
                    PlayAnim_Wrapper_1(npc, cm.umbrellaRainActive);
                }
            }
            if (!hidden && ctx->motnSteeringActive && ctx->moveObjRuntime) {
                CycleMoveObjectRuntime &runtime = *ctx->moveObjRuntime;
                CycleManager::ResolveVisibleBikeModels(runtime);
                if (runtime.model) {
                    runtime.renderOpacity = ctx->obj ? ctx->obj->renderOpacity : 0.0f;
                    runtime.pos = ctx->pos;
                    runtime.visibleNext = g_visibleMoveObjects;
                    g_visibleMoveObjects = &runtime;
                }
            }
            if (ctx->motnSteeringActive && ctx->pFace) {
                const bool nearCamera = CycleManager::DistanceSquared(ctx->pos, g_tmp_cycle_v3) < 25.0f;
                if (nearCamera != (ctx->faceNearActive != 0)) {
                    FACE_UpdateExpression(ctx->obj, nearCamera ? 1 : 0, 0, nearCamera ? 2 : 0);
                    ctx->faceNearActive = nearCamera;
                }
            }
            ctx->prevMotnSteeringActive = ctx->motnSteeringActive;
        }

        // Promoted programs join the simulated set hidden, at their cycle position and yaw.
        for (CycleManager::MobjEntry *entry = g_PromoteList; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::MobjCycle *const ctx = entry->ctx;
            CycleManager::MoveProgramToList(npc, CycleProgramList_SimSet, CycleListInsert::Head);
            if (ctx->obj) {
                shenApplyTranslationToAll(ctx->obj->entity.node, ctx->pos);
                CycleManager::SetNodeChainYaw(*ctx->obj, ctx->desiredPathYaw);
                ctx->obj->renderOpacity = 0.0f;
            }
            ctx->motnSteeringActive = 0;
            ctx->prevMotnSteeringActive = 0;
            CycleManager::RecomputeSteeringExtents(npc);
            ++g_promoteListCycleCount;
            entry = next;
        }

        // Mid programs (vehicles held near the camera) run without characters until out of view (materialized again).
        for (CycleManager::MobjEntry *entry = g_MidQueue; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            CycleManager::MobjCycle *const ctx = entry->ctx;
            if (!MOBJ_executeTask(entry, time, 0, 0)) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
                CycleManager::UpdateSndStreaming(npc);
            } else if (!shenIsLocalSphereInsideViewCullVolume(ctx->pos, ctx->range) || !shenIsLocalSphereVisibleInScreenDepthTable(ctx->pos, ctx->range)) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Far, CycleListInsert::Head);
                AttachEntityAndInitCycle(ctx->obj, true);
            }
            ++g_midQueueCycleCount;
            entry = next;
        }

        // Running programs belong to the script: their cycles follow their characters, and a conversation that references one holds
        // the cycle conversation.
        for (CycleManager::MobjEntry *entry = g_RunningPrograms; entry; entry = entry->nextDormantProgram) {
            CycleManager::MobjCycle *const ctx = entry->ctx;
            if (ctx->obj)
                ctx->pos = GetPositionForEntity(ctx->obj->entity.node);
            if (globalSimulation) {
                if (SCEN_HasActiveRuntimeObject() && scnf && (scnf->flags & 0x20000U) != 0) {
                    const std::span<const std::uint32_t> referenced = SCEN_ReferencedCharacterIds();
                    for (size_t i = 0; i < referenced.size() && i < 0x18u && referenced[i]; ++i) {
                        if (referenced[i] == ctx->moveConfig->obj_id.name) {
                            g_cycleConversationHoldActive = 1;
                            break;
                        }
                    }
                }
            } else {
                g_bGlobalFlag = 0;
                g_cycleConversationHoldActive = 0;
            }
            ++g_runningProgramCount;
        }
    }

    CycleManager::PublishVisibleParkedMoveObjects();




    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        if (npc.cycle->moveConfig->cycleClassCode != 6u)
            continue;
        CycleManager::UpdateClass6DoorNodes(npc);

        float dx = npc.cycle->pos.x - npc.cycle->steeringTarget.x;
        float dz = npc.cycle->pos.z - npc.cycle->steeringTarget.z;
        if (std::fabs(dx) < FLT_EPSILON)
            dx = 0.0f;
        if (std::fabs(dz) < FLT_EPSILON)
            dz = 0.0f;
        if (dx == 0.0f && dz == 0.0f) {
            npc.motnCycleYawDelta = 0;
            continue;
        }

        const int16_t targetYaw = static_cast<int16_t>(atan2_fixed16(dx, dz));
        npc.motnCycleYawDelta = Angle16Delta(static_cast<uint16_t>(targetYaw), static_cast<uint16_t>(npc.cycle->desiredPathYaw));
    }
    for (CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet); npc;
         npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        if (npc->cycle->moveConfig->cycleClassCode >= 4u)
            CycleManager::UpdateVehicleWheelNodes(*npc);
    }
}

void Process() {
    if (!g_updateRenderFrameMode || g_bProcessCharaCycles || !g_bShouldDraw)
        return;
    CycleManager::RenderTaggedPayloads();
    CycleManager::RenderMoveObjects();
    CycleManager::DrawUmbrellas();
}

void CoreCallback_Mo_Exec(HLibTaskData *) {
    if ((g_dbg_flags & 0x800) != 0 && g_bProcessCharaCycles) {
        if (gCycleManager) {
            gCycleManager = 0;
            return;
        }
        gCycleManager = 1;
    } else if (!gCycleManager) {
        return;
    }

    if (!g_bProcessCharaCycles)
        CycleManager::UpdateCharaCycles();
}

void MotionCallback_Mo_Exec2(HLibTaskData *) {
    if (gCycleManager) {
        CycleManager::UpdateMOTNSplines();
        CycleManager::Process();
        g_steeringIntersectionVolumeCount = 0;
    }
}









void Init() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    g_cycle_distance_check = CycleManager::CurrentAreaId() == SHEN_NAME(JOMO) ? 2.25f : 9.0f;
    if (!g_MOBJ_needs_new_update) {
        const std::int32_t seconds = static_cast<std::int32_t>(CycleManager::CurrentTimeSeconds());
        const std::int32_t previous = static_cast<std::int32_t>(g_prevCycleSeconds);
        if (previous < seconds - 600 || previous > seconds + 600)
            g_MOBJ_needs_new_update = 1;
    }
    g_cm_unk_flag = 1;
    g_cyclTalkCycleActive = 0;
    g_bHasLoadedHuman = 0;
    char sceneDirectory[32];
    std::snprintf(sceneDirectory, sizeof(sceneDirectory), "scene/%02d/stream", g_sceneId);
    TimeContext timeContext{};
    const timeContextType_t timeContextType = ConstructTimeContext(&timeContext);
    if (g_MOBJ_needs_new_update) {
        CycleManager::ClearProgramLists();
        cm.npcRecords.clear();
        cm.npcRecords.reserve(static_cast<std::size_t>(g_mobjCycleCapacity));
        MOBJ_ResetDynamicRuntimes();
        CycleManager::LoadMNRBAndMCIR();
        CycleManager::ClearAllMoveObjChairOccupancy();
        g_tmp_new_day = g_currentDateTime.day;
        const std::size_t capacity = static_cast<std::size_t>(g_MOBJ_num_entries < g_mobjCycleCapacity ? g_MOBJ_num_entries : g_mobjCycleCapacity);
        for (uint32_t i = 0; cm.mscrOffset && i < cm.mscrCharaCount && cm.npcRecords.size() < capacity; ++i) {
            uint32_t entryOffset = 0;
            if (!ReadValue(cm.bytes, cm.mscrOffset + 4 + i * 4, entryOffset))
                break;
            const uint32_t headerOffset = cm.mscrOffset + entryOffset;
            CycleManager::SelectBranchByFlagsAndDate(headerOffset, timeContextType, timeContext);
            CycleManager::MobjData *const header = reinterpret_cast<CycleManager::MobjData *>(cm.bytes.data() + headerOffset);
            if (header->programIdSuffix != SHEN_NAME(PRG1))
                continue;
            const std::size_t index = cm.npcRecords.size();
            CycleManager::MobjCycle *const ctx = &g_MOBJ_Cycles[index];
            *ctx = CycleManager::MobjCycle{};
            ctx->modeValue = 2;
            ctx->bbbbbbbb = 5;
            ctx->animMoveSpeedScale = 0.041666668f;
            ctx->primarySoundBankId = -1;
            ctx->secondarySoundBankId = -1;
            ctx->baseMoveSpeed = 1.0f;
            ctx->pathMoveSpeed = 1.0f;
            ctx->moveConfig = header;
            ctx->activeAnimId = header->motionParamIds[0];
            const uint32_t humansName = header->obj_id.name == SHEN_NAME(FUKU) && SCEN_GetCurrentScenarioStep() < 60 ? SHEN_NAME(FUK2) : header->obj_id.name;
            ctx->humansIdx = Humans_n_GetIndexForCharacter(humansName);
            CycleManager::MobjEntry *const entry = &g_MOBJ_pData[index];
            *entry = {.currentScheduleEntry = reinterpret_cast<CycleTimetableRow *>(reinterpret_cast<char *>(header) + header->currentBranchOfs),
                .ctx = ctx,
                .prev_or_backlink = index ? &g_MOBJ_pData[index - 1] : nullptr,
                .nextDormantProgram = nullptr};
            if (index)
                g_MOBJ_pData[index - 1].nextDormantProgram = entry;
            else
                g_DormantPrograms = entry;
            CycleManager::NpcRecord &npc = cm.npcRecords.emplace_back();
            npc.cycle = ctx;
            npc.entry = entry;
            npc.headerOffset = headerOffset;
            npc.moveConfigOffset = headerOffset;
            ctx->selectedMotionToken = CycleManager::SelectRandomMotionToken(npc);
            ctx->pos = {1000.0f, 1000.0f, 1000.0f};
            ctx->steeringTarget = ctx->pos;
            ctx->xmptResumePos = ctx->pos;
            if (header->cycleClassCode == 6) {
                ctx->dist = 20.0f;
                ctx->range = 10.0f;
            } else if (header->cycleClassCode == 4 || header->cycleClassCode == 5 || header->cycleClassCode == 7) {
                ctx->dist = 12.0f;
                ctx->range = 6.0f;
            } else {
                ctx->dist = 3.0f;
                ctx->range = 1.5f;
            }
            CycleManager::RecomputeSteeringExtents(npc);
        }
        CycleManager::TaggedPayload *const payloads = reinterpret_cast<CycleManager::TaggedPayload *>(g_pMOBJ_HeapHead);
        for (std::size_t index = 0; index < kCycleTaggedPayloadPoolSize; ++index) {
            payloads[index].model = nullptr;
            payloads[index].drawNext = nullptr;
            payloads[index].next = index + 1 < kCycleTaggedPayloadPoolSize ? &payloads[index + 1] : nullptr;
        }
        g_CycleManager_Current = payloads;
        g_CM_obj_list = nullptr;
    }
    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        if (npc.cycle->moveConfig->cycleClassCode != 6u)
            continue;
        npc.motnCycleBlend = 0.5f;
        npc.motnCycleState = 0;
        npc.cycle->selectedMotionToken = 0;
        npc.field148 = 0;
        npc.motnCycleYawDelta = 0;
    }
    HUMANS_AFS_Ptr = PACK_ReadAFS(sceneDirectory, "HUMANS.afs");
    g_pszTempLoadedHumanSched = nullptr;
    PackTaskData *const pack = g_packTaskData ? g_packTaskData : g_packTask ? reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask)) : nullptr;
    if (pack)
        pack->callbackBudget = 1024;
    MaxNearNPCs = 5;
    IsDBYO = (CycleManager::CurrentAreaId() == SHEN_NAME(DBYO) ? 1 : 0) + 4;

    cm.updateFrames = 0;
    cm.cycleLoadSettleFrameCounter = 0;
    cm.mobjFrames = 0;
    cm.mobjLastActive = 0;
    cm.mobjAdvancedTotal = 0;
    cm.mobjPathStartedTotal = 0;
    cm.mobjPathTickedTotal = 0;
    cm.mobjPathCompletedTotal = 0;
    cm.mobjNpcAnimAdvances = 0;
    cm.mobjNpcAnimDuplicateSyncs = 0;
    cm.moveObjSitBegins = 0;
    cm.moveObjSitAnchors = 0;
    cm.moveObjStandBegins = 0;
    cm.moveObjResumePathStarts = 0;
    cm.moveObjSitCompletions = 0;
    cm.steeringChecks = 0;
    cm.steeringAdjustments = 0;
    cm.steeringYawChecks = 0;
    cm.steeringYawAdjustments = 0;
    cm.steeringYawActorAvoidance = 0;
    cm.steeringYawRyoAvoidance = 0;
    cm.steeringYawVolumeAvoidance = 0;
    cm.steeringYawMapAvoidance = 0;
    cm.steeringActorClass0To3Hits = 0;
    cm.steeringActorClass4To7Hits = 0;
    cm.steeringMoveObjectActorHits = 0;
    cm.steeringRyoHits = 0;
    cm.steeringVolumeHits = 0;
    cm.steeringMotnClampAdjustments = 0;
    cm.steeringMapFallbackHits = 0;
    cm.npcAnimSyncEpoch = 0;
    cm.motnSpeedLookups = 0;
    cm.motnSpeedLookupHits = 0;
    cm.motnRootChannelHits = 0;
    cm.motnRootChannelFallbacks = 0;
    cm.motnRootLoopBoundarySteps = 0;
    cm.shutDispatches = 0;
    cm.shutDispatchMissing = 0;
    cm.shutDispatchStateChanges = 0;
    cm.taggedWaitBegins = 0;
    cm.taggedWaitCompletions = 0;
    cm.taggedWaitTimeouts = 0;
    cm.taggedWaitTargetMisses = 0;
    cm.talkMoveBegins = 0;
    cm.talkMoveCompletions = 0;
    cm.talkMoveTargetMisses = 0;
    cm.originalProgramCreateCharacterCommands = 0;
    cm.originalProgramCreateCharacterLoads = 0;
    cm.originalProgramCreateCharacterFailures = 0;
    cm.doorPseudoRecords = 0;
    cm.doorRequestAllocations = 0;
    cm.doorRequestPendingPolls = 0;
    cm.doorRequestActivePolls = 0;
    cm.doorRequestDonePolls = 0;
    cm.doorRequestFailedPolls = 0;
    cm.doorRequestNotReadyPolls = 0;
    cm.doorRequestAllocFailures = 0;
#if !SHENMUE_IS_PC
    cm.dynamicTurnCollisionVolumes.fill(CycleManager::IntersectionEntry{});
#endif
    g_dynamicTurnCollisionVolumeCount = 0;
#if !SHENMUE_IS_PC
    cm.intersectionEntries.fill(CycleManager::IntersectionEntry{});
#endif
    g_steeringIntersectionVolumeCount = 0;

    if (g_MOBJ_needs_new_update) {
        CycleManager::ZeroFields();
        CycleManager::ProcessCycleInternal();
    } else {
        for (CycleManager::NpcRecord &npc : cm.npcRecords)
            CycleManager::ClearProgramForStackReset(npc);
        CycleManager::TaggedPayload *const payloads = reinterpret_cast<CycleManager::TaggedPayload *>(g_pMOBJ_HeapHead);
        for (std::size_t index = 0; index < kCycleTaggedPayloadPoolSize; ++index)
            payloads[index].model = nullptr;
        if (CycleMoveObjectRuntime *const runtimes = MOBJ_DynamicRuntimes()) {
            for (std::size_t index = 0; index < kMobjDynamicRuntimeCount; ++index) {
                runtimes[index].model = nullptr;
                runtimes[index].shadowOrAuxModel = nullptr;
            }
        }
        for (CycleManager::MobjEntry *entry = g_NearQueue; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            if (CycleManager::SelectBranchByFlagsAndDate(npc.moveConfigOffset, timeContextType, timeContext) || !entry->ctx->areaID.name) {
                CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
                CycleManager::UpdateCurrentCursor(entry->ctx);
                CycleManager::Update(entry);
            }
            entry = next;
        }
        for (CycleManager::MobjEntry *entry = g_DormantPrograms; entry;) {
            CycleManager::MobjEntry *const next = entry->nextDormantProgram;
            if (CycleManager::SelectBranchByFlagsAndDate(CycleManager::EntryRecord(entry)->moveConfigOffset, timeContextType, timeContext) ||
                !entry->ctx->areaID.name) {
                CycleManager::UpdateCurrentCursor(entry->ctx);
                CycleManager::Update(entry);
            }
            entry = next;
        }
    }
    g_MOBJ_needs_new_update = 0;
}
} // namespace CycleManager





void PreLoad_InitCycleman() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    CycleManager::ClearProgramLists();
    g_MOBJ_pData = nullptr;
    g_MOBJ_num_entries = 0;
    g_MOBJ_Cycles = nullptr;
    if (static_cast<std::uint32_t>(g_actNumber - 1) > 3U)
        return;
    CycleManager::MoveObj_FileLoad("cm_d000.bin");
    std::snprintf(g_tmp_cm_path, sizeof(g_tmp_cm_path), "%s", "misc");
    struct_v16_2 MSCR{};
    if (cm.loaded && !cm.schedule.empty()) {
        if (uint8_t *const mscr = CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(MSCR), 0, static_cast<int32_t>(cm.schedule.size()) - 1))
            ReadValue(cm.bytes, static_cast<size_t>(mscr - cm.bytes.data()), MSCR);
    }
    g_MOBJ_needs_new_update = 1;
    g_MOBJ_num_entries = static_cast<std::int32_t>(MSCR.charaCnt);
    g_MOBJ_pData = static_cast<CycleManager::MobjEntry *>(HLib_AllocHeapMemBlock(32LL * g_MOBJ_num_entries, static_cast<std::int32_t>(SHEN_NAME(MOBJ))));
    g_MOBJ_Cycles = static_cast<CycleManager::MobjCycle *>(HLib_AllocHeapMemBlock(600LL * g_MOBJ_num_entries, static_cast<std::int32_t>(SHEN_NAME(MOBJ))));
    if (g_MOBJ_pData)
        (reinterpret_cast<HLib::TaskData *>(g_MOBJ_pData) - 1)->alignment |= 1;
    if (g_MOBJ_Cycles)
        (reinterpret_cast<HLib::TaskData *>(g_MOBJ_Cycles) - 1)->alignment |= 1;
    g_mobjCycleCapacity = g_MOBJ_pData && g_MOBJ_Cycles ? g_MOBJ_num_entries : 0;
    MOBJ_AllocateTaskMemory();
    MOBJ_SetupTaskPointer();
#if SHENMUE_IS_PC
    CycleManager::InitMemory();
#endif
    CycleManager::LoadMNRBAndMCIR();
    CycleManager::LoadSHUT();
    SetMOBJTaskPtr();
}



void *g_MOBJ_TaskData = nullptr;

void *g_MOBJ_TaskData_0 = nullptr;

void *g_MOBJ_TaskData_1 = nullptr;

void *g_MOBJ_TaskData_2 = nullptr;

void *g_MOBJ_TaskData_3 = nullptr;

void *g_MOBJ_TaskData_4 = nullptr;

void *g_MOBJ_TaskData_5 = nullptr;

void *g_MOBJ_TaskData_6 = nullptr;
namespace CycleManager {




int32_t EnsureHeapSpace() {
    g_MOBJ_TaskData = HLib_AllocHeapMemBlock(0x80, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_0 = HLib_AllocHeapMemBlock(0x100, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_1 = HLib_AllocHeapMemBlock(0x300, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_2 = HLib_AllocHeapMemBlock(0x100, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_3 = HLib_AllocHeapMemBlock(0x100, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_4 = HLib_AllocHeapMemBlock(0x100, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_5 = HLib_AllocHeapMemBlock(0x4C0, SHEN_NAME(MOBJ));
    g_MOBJ_TaskData_6 = HLib_AllocHeapMemBlock(0x1A0, SHEN_NAME(MOBJ));
    if (!g_MOBJ_TaskData_5 || !g_MOBJ_TaskData_6) {
        for (void *const block : {g_MOBJ_TaskData_6, g_MOBJ_TaskData_5, g_MOBJ_TaskData_4, g_MOBJ_TaskData_3, g_MOBJ_TaskData_2, g_MOBJ_TaskData_1,
                 g_MOBJ_TaskData_0, g_MOBJ_TaskData})
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(block), 0);
    }
    return 1;
}

void Setup(const char *InFilename) {
    char path[32];
    std::snprintf(path, sizeof(path), "scene/%02d/stream", g_sceneId);
    if (!CycleManager::EnsureHeapSpace()) {
        CycleManager::EnsureHeapSpace();
        return;
    }
    g_MOBJ_num_entries = CycleManager::ReadCyclemanForScene(InFilename);
    std::memset(g_MOBJ_TaskData_6, 0, 0x54);
    KDAN_ResetOccupancy();
    HumansIndexTable = static_cast<HumansEntry *>(ReadFile(path, "HUMANS.idx", SHEN_NAME(MOBJ)));
    gCycleManager = 1;
    g_bProcessCharaCycles = 0;
    CycleManager::Init();

    g_mobjCoreTask = CycleManager::EnqueueTask(CycleManager::CoreCallback_Mo_Exec, 4, 2, SHEN_NAME(MOBJ), "CycleManager_CoreCallback_Mo_Exec");
    g_mobjMotionTask = CycleManager::EnqueueTask(CycleManager::MotionCallback_Mo_Exec2, 11, 2, SHEN_NAME(MOBJ), "CycleManager_MotionCallback_Mo_Exec2");
}
} // namespace CycleManager


void InitCycleStack() {
    CycleManager::ResetStackLists();
}

extern void *g_renderObjectStorage;

extern void *g_renderIndexStorageA;

extern void *g_renderIndexStorageB;


std::int32_t g_hmdlInitDrainFlag = 0;




void Init_0() {
    g_cycleManagerRuntime.mainMenu = false;
    g_cycleManagerRuntime.inGame = true;

    if (!g_renderObjectStorage)
        return;
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    for (std::size_t slot = 0; slot < 1024; ++slot) {
        if (!objects.hashes[slot])
            continue;
        RenderArrayOwner &object = objects.objects[slot];
        object.referenceCount = 1;
        if ((object.flags & 0x40U) == 0) {
            const std::uintptr_t handle = object.handle;
            ReleaseHmdlRuntimeObject(handle);
            ReleaseHmdlRuntimeObject(handle + 1);
            continue;
        }
        for (std::uint32_t index = 0; index < object.childCount; ++index) {
            if (RenderArrayEntry *const child = object.children[index])
                child->retire = 2;
        }
    }
    const std::int32_t drainFlag = g_hmdlInitDrainFlag;
    g_hmdlInitDrainFlag = 0;
    g_hmdlEntryDirty = 1;
    while (ProcessHmdlResourceTransitions(0)) {
    }
    g_hmdlInitDrainFlag = drainFlag;
    for (std::size_t slot = 0; slot < 1024; ++slot) {
        const RenderArrayOwner &object = objects.objects[slot];
        if (objects.hashes[slot] && (object.flags & 0x40U) == 0)
            RemoveHmdlRuntimeObject(static_cast<std::uint32_t>(object.handle % 0xffffffffULL));
    }
    for (void *const storage : {g_renderIndexStorageA, g_renderIndexStorageB}) {
        HmdlHandleRemap &remap = *static_cast<HmdlHandleRemap *>(storage);
        for (std::size_t slot = 0; slot < 0x4000; ++slot) {
            remap.values[slot] = 0;
            remap.hashes[slot] = 0;
            remap.probes[slot] = 0;
        }
        remap.count = 0;
    }
}


void InitSkyGeometry(std::array<std::uint32_t, 9> &geometry) {
    for (std::uint32_t &element : geometry) {
        const RenderArrayOwner *const object = reinterpret_cast<const RenderArrayOwner *>(ResolveHmdlRuntimeObject(reinterpret_cast<std::uintptr_t>(&element)));
        if (object)
            d3t::ReleaseModelHandle(object->renderHandle);
    }
}



void LoadSkyGeometry(const char *root) {
    constexpr std::array<const char *, 9> kSkyGeometryFilenameTable{
        "HD_skyDome.fbx",
        "HD_skyHorizon0.fbx",
        "HD_skyHorizon1.fbx",
        "HD_skyHorizon2.fbx",
        "HD_skyLayer0.fbx",
        "HD_skyLayer1.fbx",
        "HD_Corona.fbx",
        "HD_Stars0.fbx",
        "HD_Moon.fbx",
    };
    CycleManager::RuntimeState &runtime = g_cycleManagerRuntime;
    char path[272];
    for (std::size_t index = 0; index < kSkyGeometryFilenameTable.size(); ++index) {
        std::sprintf(path, "%s/%s", root, kSkyGeometryFilenameTable[index]);
        const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(&runtime.skyGeometryRuntimeData[index]);
        d3t::FileRead_ALL(handle, path, 0, 0x1a0);
        SetupEngineObject(handle, handle, 2);
        SetupEngineObject(handle, reinterpret_cast<std::uintptr_t>(&runtime.skyProbeGeometryRuntimeData[index]), 4);
    }
}



void LoadSky(const char *directory) {
    char areaAir[272];
    char skyProbe[272];
    std::sprintf(areaAir, "%s/air", directory);
    std::sprintf(skyProbe, "/%s/HD_skyDome.fbx", areaAir);
    const char *const skyRoot = d3t::CheckForFile(skyProbe) ? areaAir : "/air";
    LoadSkyGeometry(skyRoot);
    g_skySkinningRoot = AllocateCachedSkinningBuffer("Sky");
    g_skyProbeSkinningRoot = AllocateCachedSkinningBuffer("SkyProbe");
    CycleManager::RuntimeState &runtime = g_cycleManagerRuntime;
    runtime.skyDirection = {0.0f, -1.0f, 0.0f};
    runtime.skyRoot = skyRoot;
    runtime.skyEnabled = true;
}

const MobjSection MOBJ_FindSection(std::uint32_t token) {
    for (const CycleManager::Section &section : g_cycleManagerRuntime.sections) {
        if (section.token == token) {
            return {shen_name{section.token}, section.offset, section.size};
        }
    }
    return {};
}
namespace CycleManager {


void ReleaseWorldOwnersForTeardown() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;

    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        if (npc.cycle->moveObjRuntime)
            CycleManager::ReleaseVehicleRuntime(npc);
        npc.releasedMoveObjRuntime = nullptr;
    }

    CycleMoveObjectRuntime *const runtimes = MOBJ_DynamicRuntimes();
    for (std::size_t index = 0; runtimes && index < kMobjDynamicRuntimeCount; ++index) {
        CycleMoveObjectRuntime &runtime = runtimes[index];
        if (runtime.ownerObjectId == 0)
            continue;
        runtime.activeFlag = 0;
    }

    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        if (npc.cycle->obj || npc.cycle->filename) {
            CycleManager::UpdateSndStreaming(npc);
        }
    }

    CycleManager::ResetPublishedSceneState();
    g_dynamicTurnCollisionVolumeCount = 0;
#if !SHENMUE_IS_PC
    cm.intersectionEntries.fill(CycleManager::IntersectionEntry{});
#endif
    g_steeringIntersectionVolumeCount = 0;
}


std::int32_t InitINSHForScene(std::uint32_t areaId, Vector3 &position, std::int32_t &yaw) {
    MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (g_cycleManagerRuntime.shutAreaID.name != areaId || !ryo)
        return -1;
    position = GetPositionForEntity(ryo->entity.node);
    std::int16_t nodeYaw = 0;
    for (CharacterNode *node = &ryo->entity.node; node; node = node->child)
        nodeYaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(nodeYaw) + static_cast<std::uint16_t>(node->rotation.y));
    yaw = nodeYaw;
    SelectCharacterMapQuery(ryo);
    std::int32_t rowIndex = 0;
    std::int32_t value8 = 0;
    std::int32_t valueC = 0;
    std::int32_t value10 = 0;
    const CollisionPointQueryOutputs outputs{.value4 = &rowIndex, .value8 = &value8, .valueC = &valueC, .value10 = &value10};
    if (!CheckIsPointIntersectWith(position, &outputs, REGION_INSH.name) || rowIndex - 1 < 0 ||
        static_cast<std::size_t>(rowIndex) > g_cycleManagerRuntime.shutRows.size())
        return -1;
    const CycleManager::ShutRow &row = g_cycleManagerRuntime.shutRows[static_cast<std::size_t>(rowIndex - 1)];
    if (!row.dword34)
        return -1;
    position = {std::bit_cast<float>(static_cast<std::uint32_t>(row.qword38)), std::bit_cast<float>(static_cast<std::uint32_t>(row.qword38 >> 32U)),
        std::bit_cast<float>(row.dword40)};
    yaw = static_cast<std::uint16_t>(row.angle3);
    return row.byte31 != 2;
}
} // namespace CycleManager


} // namespace shenmue::s1
