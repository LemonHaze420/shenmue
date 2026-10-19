#pragma once

#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/hlib.hpp"

#include "s2/npc.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct LoadedMapPackage;
struct PakGlobalData;
inline constexpr shen_name NPC_HUMAN_INDEX_MEMORY{SHEN_NAME(NPCI)};
inline constexpr shen_name NPC_HUMAN_SLOT_MEMORY{SHEN_NAME(NPCH)};
inline constexpr shen_name NPC_ROUTE_TASK{SHEN_NAME(NPCT)};
inline constexpr shen_name NPC_SCRIPT_ROUTE_SLOT{SHEN_NAME(MOVE)};
inline constexpr shen_name NPC_SAM{SHEN_NAME(SAM_)}, NPC_LLY{SHEN_NAME(LLY_)}, NPC_YUA{SHEN_NAME(YUA_)};
inline constexpr shen_name TASK_NPC_LOAD{SHEN_NAME(load)};
extern std::int32_t g_usingCustomAuthorCharacterList;
extern std::int32_t npcPoolTarget;
extern Vector3 npcPlayerPosition;
extern float npcPlayerRadius;
enum class NpcHumanPakState : std::int32_t { Empty, Requested, Ready, Attached };
struct NpcHumanPakSlot {
    NpcHumanPakState state;
    std::uint32_t reserved04;
    PakGlobalData *pak;
};
static_assert(sizeof(NpcHumanPakSlot) == 0x10);
static_assert(offsetof(NpcHumanPakSlot, pak) == 8);

struct NpcRouteResetGroupHeader {
    shen_name area;
    std::int32_t reserved04;
};
struct NpcRouteResetEntry {
    shen_name character;
    std::int32_t nodeOffset;
    std::int32_t routeNode;
    std::int32_t parameter;
    std::int32_t parameterStep;
    Vector3 previousPosition;
    Vector3 position;
};
static_assert(sizeof(NpcRouteResetGroupHeader) == 8);
static_assert(sizeof(NpcRouteResetEntry) == 0x2c);
static_assert(offsetof(NpcRouteResetEntry, previousPosition) == 0x14);
static_assert(offsetof(NpcRouteResetEntry, position) == 0x20);

struct NpcTransitionEntry {
    shen_name area;
    shen_name character;
    Vector3 position;
    std::int16_t yaw;
    std::int16_t activationMode;
    float controllerToggle;
    std::uint32_t reserved1c;
    const std::uint16_t *motions;
};
struct NpcTransitionRuntimeSlot {
    shen_name character;
    Vector3 position;
    std::int16_t yaw;
    std::int16_t activationMode;
    std::uint32_t reserved14;
    const std::uint16_t *motions;
    Character *node;
    std::int8_t motionCacheIndex;
    std::uint8_t motionCursor;
    std::uint8_t field2a;
    std::uint8_t field2b;
    float controllerToggle;
};
static_assert(sizeof(NpcTransitionEntry) == 0x28);
static_assert(sizeof(NpcTransitionRuntimeSlot) == 0x30);
static_assert(offsetof(NpcTransitionRuntimeSlot, motions) == 0x18);
static_assert(offsetof(NpcTransitionRuntimeSlot, node) == 0x20);
static_assert(offsetof(NpcTransitionRuntimeSlot, controllerToggle) == 0x2c);
inline constexpr std::uint16_t npcDogIdleMotions[]{0x9907, 0x9907, 0x9905, 0};
inline constexpr std::uint16_t npcDogMovingMotions[]{0x9908, 0x9908, 0x9908, 0x9906, 0, 0, 0, 0};
inline constexpr std::uint16_t npcCatMovingMotions[]{0x9904, 0x9903, 0, 0};
inline constexpr std::uint16_t npcCatIdleMotions[]{0x9902, 0x9902, 0x9901, 0};
inline constexpr std::uint16_t npcCowIdleMotions[]{0xa001, 0xa001, 0xa002, 0};
inline constexpr NpcTransitionEntry npcTransitionEntries[]{
    {SHEN_NAME(AR02), SHEN_NAME(DOG0), {0x1.d23334p+7f, 0x1.90e148p+6f, 0x1.b4p+8f}, 16384, 0, 0.4f, 0, npcDogIdleMotions},
    {SHEN_NAME(AR02), SHEN_NAME(CAT0), {0x1.ecf0a4p+7f, 0x1.947ae2p+6f, 0x1.646e14p+8f}, -7282, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(AR02), SHEN_NAME(CAT1), {0x1.fp+7f, 0x1.947ae2p+6f, 0x1.658p+8f}, -12216, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(AR02), SHEN_NAME(CAT2), {0x1.eec7aep+7f, 0x1.95b852p+6f, 0x1.655c28p+8f}, -365, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(AR03), SHEN_NAME(CAT3), {0x1.20147ap+8f, 0x1.92a3d8p+6f, 0x1.c98p+8f}, 5461, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(WS00), SHEN_NAME(DOG1), {0x1.50f5c2p+5f, 0x1.893334p+6f, 0x1.cbp+6f}, 17840, 0, 0.4f, 0, npcDogMovingMotions},
    {SHEN_NAME(WS00), SHEN_NAME(CAT4), {0x1.08ccccp+6f, 0x1.94b852p+6f, 0x1.f13334p+6f}, 24576, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(WS00), SHEN_NAME(CAT0), {0x1.08c29p+6f, 0x1.94b852p+6f, 0x1.f0147ap+6f}, -16767, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(WSY0), SHEN_NAME(CAT4), {0x1.08ccccp+6f, 0x1.94b852p+6f, 0x1.f13334p+6f}, 24576, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(WSY0), SHEN_NAME(CAT0), {0x1.08c29p+6f, 0x1.94b852p+6f, 0x1.f0147ap+6f}, -16767, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(WK00), SHEN_NAME(DOG2), {0x1.8cp+6f, 0x1.9ad70ap+6f, 0x1.21ccccp+8f}, -11852, 0, 0.4f, 0, npcDogIdleMotions},
    {SHEN_NAME(WK00), SHEN_NAME(CAT4), {0x1.e051ecp+6f, 0x1.9ap+6f, 0x1.41c7aep+8f}, -5644, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(WK00), SHEN_NAME(CAT0), {0x1.e50a3ep+6f, 0x1.99147ap+6f, 0x1.e9147ap+7f}, 6189, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(WK00), SHEN_NAME(CAT1), {0x1.0e8p+7f, 0x1.99147ap+6f, 0x1.21ccccp+8f}, -7282, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(WT00), SHEN_NAME(DOG3), {0x1.e65c28p+7f, 0x1.c770a4p+6f, 0x1.fcc29p+7f}, -24394, 0, 0.4f, 0, npcDogIdleMotions},
    {SHEN_NAME(WT00), SHEN_NAME(CAT2), {0x1.f31eb8p+7f, 0x1.cf70a4p+6f, 0x1.548f5cp+8f}, -24212, 0, 0.3f, 0, npcCatMovingMotions},
    {SHEN_NAME(WT00), SHEN_NAME(CAT3), {0x1.f4b852p+7f, 0x1.cf70a4p+6f, 0x1.53fd7p+8f}, 20753, 0, 0.3f, 0, npcCatIdleMotions},
    {SHEN_NAME(KSH1), SHEN_NAME(USI0), {0x1.40ae14p+7f, 100.0f, 0x1.b9051ep+7f}, 8192, 0, 0.5f, 0, npcCowIdleMotions},
    {SHEN_NAME(KSH1), SHEN_NAME(USI1), {0x1.44851ep+7f, 100.0f, 0x1.b16666p+7f}, -11742, 0, 0.5f, 0, npcCowIdleMotions},
    {SHEN_NAME(KSH1), SHEN_NAME(USI2), {0x1.2728f6p+7f, 100.0f, 0x1.828p+7f}, 15291, 0, 0.5f, 0, npcCowIdleMotions},
    {},
};
inline constexpr Matrix4x4 npcSplineBasis{.rows = {
                                              {-0.16666667f, 0.5f, -0.5f, 0.16666667f},
                                              {0.5f, -1.0f, 0.0f, 0.66666669f},
                                              {-0.5f, 0.5f, 0.5f, 0.16666667f},
                                              {0.16666667f, 0.0f, 0.0f, 0.0f},
                                          }};

struct LoadedNpcPackage {
    shen_name name{};
    const LoadedMapPackage *pak{};
    std::array<char, 14> modelName{};
    std::span<const std::byte> model;
    std::span<const std::byte> characterTable;
    std::span<const std::byte> behavior;
    std::size_t references{};
};



struct NpcRouteRecordSource {
    shen_name name{};
    const std::byte *recordBase{};
    std::int16_t humanPackageIndex{-1};
};

enum class NpcScheduleSeedKind : std::int8_t {
    Empty = 0,
    DelayWait = 1,
    ScriptState = 2,
    AreaPointState = 3,
};

struct NpcSavedRouteState {
    std::uint32_t flags;
    std::uint32_t routeFlags;
    std::uint8_t routeMode;
    std::uint8_t scheduleProgress;
    std::uint8_t liveState;
    std::int8_t phase;
    std::uint16_t parameter;
    std::uint16_t currentAreaPoint;
    std::uint16_t pendingAreaPoint;
    std::uint16_t arrivalYaw;
    float localX;
    float localZ;
};
static_assert(sizeof(NpcSavedRouteState) == 0x1c);
static_assert(offsetof(NpcSavedRouteState, routeMode) == 0x08);
static_assert(offsetof(NpcSavedRouteState, localX) == 0x14);

struct NpcScheduleSeed {
    NpcScheduleSeedKind kind;
    std::int8_t routeDirectionOrEyeMode;
    std::uint8_t packedMoveFlagsHigh;
    std::uint8_t controlBit4;
    shen_name name;
    std::uint32_t flags;
    std::uint16_t currentCommandOffset;
    std::uint16_t resumeCommandOffset;
    std::uint8_t scheduleProgress;
    std::uint8_t liveState;
    std::uint8_t activeBranchIndex;
    std::uint8_t currentOpcode;
    std::uint16_t currentAreaPoint;
    std::uint16_t pendingAreaPoint;
    std::uint16_t yaw;
    std::uint16_t packedMoveFlagsLow;
    std::uint16_t attachmentMotion;
    std::uint16_t attachmentFlags;
    std::uint32_t controlFlagsAndForcedY;
    std::array<std::uint16_t, 4> localTriggerOffsets;
    std::array<std::uint16_t, 16> globalTriggerOffsets;
    NpcSavedRouteState state;
};
static_assert(sizeof(NpcScheduleSeed) == 0x68);
static_assert(offsetof(NpcScheduleSeed, currentCommandOffset) == 0x0c);
static_assert(offsetof(NpcScheduleSeed, controlFlagsAndForcedY) == 0x20);
static_assert(offsetof(NpcScheduleSeed, localTriggerOffsets) == 0x24);
static_assert(offsetof(NpcScheduleSeed, globalTriggerOffsets) == 0x2c);
static_assert(offsetof(NpcScheduleSeed, state) == 0x4c);

inline constexpr std::size_t NpcScheduleSeedBufferSize = 2 * sizeof(NpcScheduleSeed);
void CaptureNpcScheduleSeeds(std::span<std::byte, NpcScheduleSeedBufferSize> destination);
void SetPendingNpcScheduleSeedBuffer(std::span<const std::byte, NpcScheduleSeedBufferSize> source);
void ResetNpcScheduleSeeds();

std::span<NpcRouteLiveEntry> CreateNpcRouteLiveEntries(std::span<const NpcRouteRecordSource> records);
std::span<NpcRouteLiveEntry> GetNpcRouteLiveEntries();
void FreeNpcRouteLiveEntries();
void RebuildNpcRouteCollision(bool reloading);
bool ReleaseNpcRouteControl(NpcRouteStateContext *context, shen_name character, std::int32_t yaw);
bool TickNpcRouteStateContext(NpcRouteStateContext *context);
void SetNpcRouteMotionState(NpcRouteStateContext &context, std::int32_t operation, bool explicitRequest = true);
bool BindNpcScriptRoute(shen_name character, const NpcRouteNode *route, const Vector3 *origin, std::int32_t yaw);
bool StartNpcScriptAreaRoute(shen_name character, shen_name oldPoint, shen_name newPoint, std::int32_t yaw);
void SetNpcScriptRouteSpeed(NpcRouteStateContext *context, shen_name character, float speed);
void SetNpcScriptRouteFlags(NpcRouteStateContext *context, shen_name character, std::int32_t operation, std::uint32_t flags);
void TickNpcSpecialRouteContext(NpcRouteLiveEntry &entry);
void ResumeNpcScheduleMotion(NpcRouteLiveEntry &entry);
Vector3 NpcRouteAreaToScene(Vector3 position);
std::uint32_t QueryNpcMoveObstruction(NpcRouteLiveEntry &entry, const Vector3 &target, bool door, std::int16_t angle, float probeRadius, float probeLength);
bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);
bool RequestNpcStreamOut(NpcRouteLiveEntry &entry, std::int32_t type);
bool CanRevealNpcStream(const NpcRouteLiveEntry &entry);
bool SetNpcMotionAttachment(NpcRouteLiveEntry &entry, std::uint16_t motion, std::uint8_t flags, std::int32_t slot);
void ClearNpcMotionAttachment(NpcRouteLiveEntry &entry, std::int32_t slot);
void TickNpcMotionClip(NpcRouteLiveEntry &entry);
std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry &entry, std::int32_t mode);
std::int64_t npcGetCurrentTimeSimState();
std::int64_t ResetCurrentTimeSimState();
std::int64_t npcSetCurrentTimeSimStateEnabled();
std::int64_t npcResetDateTimeIfNeeded();
std::int64_t npcAdvanceTime(std::int32_t hour, std::int32_t minute);
void BuildRouteTimeWindow(std::int32_t startTick, std::int32_t endTick, std::int32_t skipAuthoredSeed);

bool EnqueueNpcRegistryTask(std::uint8_t nextFunctionIndex);


bool PublishNpcRegistryTask();
void FreeNpcRegistryTask();
bool NpcWait();
std::int32_t npcSetAreaName(shen_name area, const Vector3 *center, float radius, std::int32_t enabled);
std::int64_t RegisterNpcFlags(shen_name character, std::uint32_t flags, std::int32_t operation);


std::int64_t PlaceNpcAtAreaPoint(shen_name character, shen_name areaPoint, const Vector3 *position, std::int32_t yaw);
std::int32_t npcGetPosFor(shen_name character, Vector3 *position);
// HasNpcRuntimeEntry/CanNpcEnterScenario: published NPCT presence and scenario admission.
bool HasNpcRuntimeEntry(shen_name character);
std::uint64_t IsNpcRouteLoadedOrVisible(shen_name character);
bool CanNpcEnterScenario(shen_name character);
// BeginNpcSpecialMotion/EndNpcSpecialMotion acquire and release the controller.
bool BeginNpcSpecialMotion(shen_name character);
bool EndNpcSpecialMotion(shen_name character);
// ResetNpcSpecialMotionTarget, RequestNpcSpecialMotionLookTarget and
// ResetNpcSpecialMotionLookTarget complement RequestNpcSpecialMotionTarget.
bool ResetNpcSpecialMotionTarget(shen_name character);
bool RequestNpcSpecialMotionLookTarget(shen_name character, const Vector3 *target);
bool ResetNpcSpecialMotionLookTarget(shen_name character);


bool QueryNpcSceneCheck(shen_name character);
inline constexpr std::uint16_t NPC_SCENE_MOTION{0xe353};
std::uint64_t QueryNpcSceneMotionPlaying(shen_name character);
inline constexpr std::array<std::uint16_t, 12> NPC_CAMERA_PROFILE_ONE_MOTIONS{0x8050, 0x804f, 0x804e, 0x80f9, 0x80f8, 0x80fb, 0x803f, 0x9504, 0, 0, 0, 0};
inline constexpr std::array<std::uint16_t, 8> NPC_CAMERA_PROFILE_TWO_MOTIONS{0x80f5, 0x80f6, 0x80f7, 0x9107, 0, 0, 0, 0};
inline constexpr std::array<std::uint16_t, 4> NPC_CAMERA_PROFILE_FOUR_MOTIONS{0x803f, 0xe382, 0xe14a, 0};
inline constexpr std::array<std::uint16_t, 6> NPC_CAMERA_PROFILE_REMAPPED_MOTIONS{0xe122, 0xe144, 0xe243, 0xe254, 0xe265, 0};
std::uint64_t QueryNpcCameraMotionProfile(shen_name character, std::uint8_t mask);
// Moves the NPC onto the given area route and makes it the script-route owner (SCEN follow requests).
bool RequestNpcAreaTransfer(shen_name character, std::uint32_t area);


void EV_LoadNPCRoute(const char *filename);
const LoadedNpcPackage *LoadNpcPackage(shen_name name, const char *scene = "01");
void ReleaseNpcPackage(const LoadedNpcPackage *package);
void FreeNpcPackages();
void npcInitData();
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character);
std::int64_t EV_CheckWarmStartNpcRoute(shen_name character);
std::int64_t npcWarmStartHumanRoute(shen_name character);
std::int32_t npcReleaseUnusedStreams();
std::int32_t PrepareNpcAllFromArray();
std::int64_t EV_ForceLoadNpc(shen_name character);
std::uint64_t EV_npcStreamOrLoadHuman(shen_name character);
std::int64_t EV_npcLockUnlock(shen_name character);
std::int64_t EV_npcFree(shen_name character);
std::int64_t npcSetLock(shen_name character, std::int32_t preserveMotion);
std::int64_t EV_ReleaseNpcLockPreservingMotion(shen_name character);
std::uint64_t SetNpcColdResetMode(std::uint32_t mode);
std::int64_t ResetNpcColdStreams();
std::uint64_t CountNpcLiveStreams();
std::uint64_t IsNpcUsingHumansAfs(shen_name character);
std::int64_t EV_ForceLoadNpcFromHumans(shen_name character);
std::uint64_t EV_npcStreamOrLoadHumanFromHumans(shen_name character);
std::uint64_t SetNpcRouteControlHold(shen_name character, std::int32_t enabled);
std::int64_t EV_npcUnknownFunc(shen_name character);
std::int64_t EV_npcUpdateFlags(shen_name character);
std::uint32_t GetNpcScriptRouteCompletion(shen_name character);

void ReleaseNpcScriptRouteOwner();

struct ConditionalNpcLoadEntry {
    shen_name character;
    Vector3 position;
    std::int32_t yaw;
    std::uint32_t motion;
    std::int32_t storyFlag;
    std::uint32_t reserved1c;
    std::uint32_t (*condition)();
};
static_assert(sizeof(ConditionalNpcLoadEntry) == 0x28);
static_assert(offsetof(ConditionalNpcLoadEntry, condition) == 0x20);
HLib::Task *QueueConditionalNpcLoads(const ConditionalNpcLoadEntry *entries);
std::uint32_t AreConditionalNpcLoadsPending();
void ResetConditionalNpcLoads();

HLib::Task *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLib::Task *startupTask);
void EV_SetAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask, bool visible = false, bool lowQualityHands = false);
void AssignAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask);
void SetAreaNpcLowQualityHands(bool enabled);
void SetAreaNpcCharactersVisible(bool enabled);
void AssignGenericNpcLoadContext(const shen_name *characters, HLib::Task *startupTask);
void ResetCompiledEventNpcLoadState();
void SetGenericNpcLowQualityHands(bool enabled);
void ResetGenericEventNpcLoadState();
HLib::Task *Queue_EVT_UTL_LoadNPCChara(const shen_name *characters, HLib::Task *startupTask);
void EnableAuthoredCharacterFlagsAndDisplay(shen_name character);
void EVT_UTL_LoadNPCChara_Callback_1(void *taskData);
void EVT_UTL_LoadNPCChara_Callback_2(void *taskData);

std::int64_t EV_NPC_NewLoadSw(std::uint32_t state);
std::uint32_t npcReleaseStreamsNearDoor(const Vector3 *point, float radius);
shen_name npcFindAreaPointOccupant(shen_name owner);
std::int64_t npcSetDoorAnswerMode(shen_name character, std::int32_t enabled);
std::int64_t EV_ResetFreeTalkFlags();
void EV_ReconfigureNpcRoutesForStoryStep();
std::uint64_t GetNpcNewLoadState();
std::uint64_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

std::int64_t InitializeNpcFlags();

} // namespace shenmue::s2
