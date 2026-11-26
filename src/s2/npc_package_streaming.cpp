#include "enhancements/misc_mods.hpp"
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_data_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/character_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/math.hpp"
#include <cmath>
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#define SHENMUE_NPC_NULL 0
#define SHENMUE_NPC_NOEXCEPT
#define SHENMUE_NPC_SIGNED(value) static_cast<std::int32_t>(value)
#include "s2/player_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/qron_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "common/matrix_stack.hpp"
#include <cfloat>
#include <cstring>
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_query_dc.hpp"
#define SHENMUE_NPC_EPSILON FLT_EPSILON
#include "s2/motion_node_dc.hpp"
#else
#include "enhancements/misc_mods.hpp"
#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "s2/npc_package_internal.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace npc_package_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace npc_package_detail {

std::int32_t npcLoadFromPak(shen_name name);
void ReleaseNpcTransitionHumanPackages();
std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, bool fromHumans);
void NpcRegistryTask_Callback(void *);
std::uint32_t ReadU32(const std::byte *source);
void npcSetupHumans();
void ReleaseNpcRouteStream(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, bool clearPackageAlias);
void BeginNpcRouteStreamOut(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime);
bool IsInsideNpcNoSpawnRadius(const Vector3 &position, float radius);
bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry);
std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry &entry, bool reload);
void UpdateNpcStreamFade(NpcRouteLiveEntry &entry);

} // namespace npc_package_detail

std::uint64_t IsNpcRouteLoadedOrVisible(shen_name character);
std::int32_t QueryNpcRouteLiveState(shen_name character);
bool CanNpcEnterScenario(shen_name character);
bool BeginNpcSpecialMotion(shen_name character);
bool EndNpcSpecialMotion(shen_name character);
bool RequestNpcSpecialMotionTarget(shen_name character, const Vector3 *target);
bool ResetNpcSpecialMotionTarget(shen_name character);
bool RequestNpcSpecialMotionLookTarget(shen_name character, const Vector3 *target);
bool ResetNpcSpecialMotionLookTarget(shen_name character);
bool QueryNpcSceneCheck(shen_name character);
std::uint64_t IsNpcSceneMotionPlaying(shen_name character);
std::uint64_t QueryNpcSceneMotionPlaying(shen_name character);
std::uint64_t QueryNpcCameraMotionProfile(shen_name character, std::uint8_t mask);
bool NpcWait();
std::int32_t npcGetPosFor(shen_name character, Vector3 *position);
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character);
std::int64_t EV_CheckWarmStartNpcRoute(shen_name character);
std::int64_t EV_ForceLoadNpc(shen_name character);
std::uint64_t EV_npcStreamOrLoadHuman(shen_name character);
void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry &entry);
std::int64_t EV_npcLockUnlock(shen_name character);
std::int64_t EV_npcFree(shen_name character);
void EVT_UTL_LoadNPCChara_Callback_1(void *);
void EVT_UTL_LoadNPCChara_Callback_2(void *taskData);
shen_name npcFindAreaPointOccupant(shen_name owner);
std::int64_t EV_ResetFreeTalkFlags();
std::uint64_t CountNpcLiveStreams();
std::uint64_t IsNpcLoadedOrVisible(shen_name character);
std::uint64_t SetNpcRouteControlHold(shen_name character, std::int32_t enabled);
std::int64_t EV_npcUnknownFunc(shen_name character);
std::int64_t EV_npcUpdateFlags(shen_name character);
std::uint32_t GetNpcScriptRouteCompletion(shen_name character);
std::int32_t npcReleaseUnusedStreams();
std::int32_t PrepareNpcAllFromArray();
std::uint64_t GetNpcNewLoadState();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void CoreNPCTask();

bool IsPlayerControlEnabled();

Character *ctrlGetRootNode();

std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry *entry);

std::int32_t MeasureNpcReloadStreamCost(NpcRouteLiveEntry *entry);

std::int32_t RequestNpcStreamLoad(NpcRouteLiveEntry *entry);

void BindNpcRouteCharacter(NpcRouteLiveEntry *entry);

std::int32_t BeginNpcRouteStreamOut(NpcRouteLiveEntry *entry, std::int32_t forced);

std::int32_t UpdateNpcStreamFade(NpcRouteLiveEntry *entry);

void ReleaseNpcRouteStream(NpcRouteLiveEntry *entry);

void UpdateStreamedNpcMotion(NpcRouteLiveEntry *entry);

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

void TickNpcRouteSlices(std::int32_t currentTime);

void SetNpcSavedDateTimeFlag();

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

void TickNpcRouteSlices(std::int32_t currentTime);

void SetNpcSavedDateTimeFlag();

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

void TickNpcRouteSlices(std::int32_t currentTime);

void SetNpcSavedDateTimeFlag();

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcSavedDateTimeFlag();

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

void TickNpcRouteSlices(std::int32_t currentTime);

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

bool IsNpcRouteContextMoving(const NpcRouteStateContextPrefix *context);

void ResetNpcRouteContext(NpcRouteStateContextPrefix *context);

bool IsNpcRouteContextMoving(const NpcRouteStateContextPrefix *context);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

bool IsInsideNpcNoSpawnRadius(const Vector3 *position, float radius);

void TickNpcRouteSlices(std::int32_t currentTime);

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

void SetNpcRouteMotionState(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation);

void RestartNpcAreaRouteContext(Character *character, NpcRouteStateContextPrefix *context);

bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest);

void BindNpcRouteMotionWork(NpcRouteStateContextPrefix *context, shen_name name, MotionWork *work);

void BindNpcRouteAppliedMovement(NpcRouteStateContextPrefix *context, shen_name name);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

void ReleaseNpcTransitionHumanPackages();

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

void ReleaseNpcTransitionHumanPackages();

bool IsOfThisVisibilityType(const Character *character, CharaVisibilityMode mode);

void EV_SetCharDisplay(Character *character, CharaVisibilityMode mode, std::int32_t enabled);

std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);

std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

void SetNpcRouteMotionState(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation);

void RestartNpcAreaRouteContext(Character *character, NpcRouteStateContextPrefix *context);

bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest);

void BindNpcRouteMotionWork(NpcRouteStateContextPrefix *context, shen_name name, MotionWork *work);

void BindNpcRouteAppliedMovement(NpcRouteStateContextPrefix *context, shen_name name);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

bool IsInsideNpcNoSpawnRadius(const Vector3 *position, float radius);

void TickNpcRouteSlices(std::int32_t currentTime);

std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *state);

bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);

bool MoveForNpcInData(NpcRouteLiveEntry *entry, shen_name area, std::int32_t apply);

std::int32_t TickNpcRouteStair(Character *character, NpcRouteStateContextPrefix *context);

NpcAreaRouteState *GetNpcAreaPointRouteState(std::int32_t point);

shen_name GetNpcAreaPointOwnerName(std::int32_t point);

shen_name GetNpcAreaPointAreaName(std::int32_t point);

void ResumeNpcScheduleMotion(NpcRouteLiveEntry *entry);

const Vector3 *GetNpcAreaPointPosition(std::int32_t point);

std::uint32_t GetNpcAreaPointMoveFlags(std::int32_t point);

std::int32_t ResumeNpcRouteAfterSceneChange(shen_name name);

std::int32_t UpdateNpcSpecialRouteQueryState(NpcRouteLiveEntry *entry);

std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, std::uint32_t flags);

void SetNpcRouteMotionState(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation);

bool ResumeNpcTimedMotion(NpcRouteLiveEntry *entry);

void NpcRouteCollisionNullsub(void *);

std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion);

std::int32_t npcPositionQueryMode = 0;

std::int32_t UpdateNpcSpecialRouteQueryState(NpcRouteLiveEntry *entry);

static const std::uint16_t npcWalkingMotions[16] = {
    0xa0b, 0xf03e, 0xf05e, 0xf07e, 0xf084, 0xf09e, 0xf0d1, 0xf060, 0xf086, 0xf084, 0xf078, 0xf058, 0xf084, 0xf084, 0xf0a4, 0xf0a6};

static const std::uint16_t npcRunningMotions[16] = {
    0xf0a6, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006};

static const std::uint16_t npcSeatedMotions[6][3][3] = {0xe108, 0x803d, 0xe108, 0x803d, 0xe108, 0xe391, 0xe392, 0xe392, 0xe391, 0x8012, 0xe0f1, 0x8009, 0xe101,
    0xe101, 0xe101, 0x8024, 0xe0df, 0xe0f2, 0xe0e3, 0xe0e3, 0xe0e3, 0xe108, 0xe0f5, 0xe0f5, 0xe0e4, 0xe0e4, 0xe392, 0xe0e3, 0xe0e3, 0xe0e3, 0xe0f5, 0xe0f5,
    0xe0f5, 0xe0e4, 0xe0e4, 0xe0e4, 0xe0f7, 0xe107, 0xe108, 0xe392, 0xe391, 0xe392, 0xe107, 0xe0f7, 0xe107, 0xe101, 0xe101, 0xe0f2, 0xe0f0, 0xe0f0, 0xe0f0,
    0xe0f6, 0xe0f6, 0xe0f6};

static const std::uint16_t npcLeaningMotions[6][3][3] = {0x8033, 0x8032, 0x802e, 0x8034, 0x802f, 0x8034, 0x8030, 0x8030, 0x8031, 0x8001, 0x8001, 0x8004, 0x8004,
    0x8004, 0x8003, 0x8003, 0x8002, 0x8002, 0x8034, 0x8034, 0x8034, 0x8031, 0x8031, 0x8031, 0x8030, 0x8030, 0x8030, 0x8034, 0x8034, 0x8034, 0x8031, 0x8031,
    0x8031, 0x8030, 0x8030, 0x8030, 0x8034, 0x8034, 0x8034, 0x8034, 0x8034, 0x8034, 0x8018, 0x8018, 0x8018, 0x8003, 0x8003, 0x8003, 0x8003, 0x8003, 0x8003,
    0x8018, 0x8018, 0x8018};

std::int32_t npcLastHumanLoadResult;

void ResumeNpcScheduleMotion(NpcRouteLiveEntry *entry);

std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);

void ApplyNpcAreaMotion(NpcRouteLiveEntry *entry, std::uint16_t motion);

void NpcRouteCollisionNullsub(void *);

extern std::int32_t npcCloudyState;

std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion);

std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *context);

std::int32_t TickNpcAreaRouteState(NpcRouteLiveEntry *entry);

std::int32_t ResumeNpcRouteAfterSceneChange(shen_name name);

const Vector3 *GetNpcAreaPointPosition(std::int32_t point);

std::uint32_t GetNpcAreaPointMoveFlags(std::int32_t point);

bool InitializeNpcAreaRoute(NpcRouteLiveEntry *entry);

bool mtwkSetDrawOverrideData(MotionWork *work, const Matrix4x4 *frame);

void RestartNpcAreaRouteContext(Character *character, NpcRouteStateContextPrefix *context);

bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest);

std::int32_t TickNpcRouteStair(Character *character, NpcRouteStateContextPrefix *context);

std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);

void ApplyNpcAreaMotion(NpcRouteLiveEntry *entry, std::uint16_t motion);

extern DateTime npcCurrentDateTime;

extern std::uint32_t npcCurrentSeconds, npcRouteBuildCount;

extern std::int32_t npcCrowdPhase, npcReloadPassStarted;

extern float npcStreamThrottle, npcStreamThrottleWithBias, npcHalfHourWeatherForecast;

extern Vector3 npcPlayerPosition;

Vector3 npcPlayerCenter = {};

float npcPlayerDisplacement = 0.0f, npcPlayerRadius = 0.0f, npcCurrentRain = 0.0f;

std::int32_t npcPlayerSearchRequested = 0, npcCloudyState = 0;

shen_name npcNearbyDoor;

void mtwkSetEnvironmentEnabled(MotionWork *work, std::uint32_t enabled);

void PrepareNpcAreaRoutes();

void DrawNpcTransitions();

std::uint32_t hasPendingNpcScheduleSeeds;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const std::byte *pendingNpcScheduleSeeds = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DateTime npcSavedDateTime;
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t UpdateNpcViewVisibility(NpcRouteLiveEntry *entry);
NpcRouteLiveEntry *GetNpcRouteEntryByIndex(std::int32_t index);
shen_name npcFindAreaPointOccupant(shen_name owner);
std::int32_t AcquireNpcTimedMotionSlot(NpcRouteLiveEntry *entry);
std::int32_t IsNpcRegistryTaskActive() SHENMUE_NPC_NOEXCEPT;
std::uint32_t GetNpcNewLoadState() SHENMUE_NPC_NOEXCEPT;
CharacterDrawCallback mtwkAssignDrawCallback(Character *character, CharacterDrawCallback callback);
std::int32_t ReplaceNpcStateCallback(NpcRouteLiveEntry *entry, std::int32_t (*callback)(NpcRouteLiveEntry *));
bool npcExists(shen_name name);
bool IsNpcRouteMoving(shen_name name);
std::int32_t GetNpcRouteAvoidanceSide(shen_name name);
std::int32_t QueryNpcRouteLiveState(shen_name name);
bool QueryNpcSceneCheck(shen_name name);
std::int32_t AcquireNpcRuntimeEntryForScript(shen_name name, NpcRouteLiveEntry **result);
bool BeginNpcSpecialMotion(shen_name name);
std::int32_t BeginNpcFollowingMotion(shen_name name);
std::int32_t StopNpcFollowingMotion(shen_name name);
std::int32_t SetNpcSpecialRouteQueryPaused(shen_name name, std::int32_t enabled);
bool IsNpcSpecialTargetWithinRange(Character *character, const NpcRouteLiveEntry *entry);
bool CheckNpcSpecialMotionTarget(shen_name name, Character *character);
bool EndNpcSpecialMotion(shen_name name);
bool RequestNpcSpecialMotionTarget(shen_name name, const Vector3 *target);
bool ResetNpcSpecialMotionTarget(shen_name name);
bool RequestNpcSpecialMotionLookTarget(shen_name name, const Vector3 *target);
bool ResetNpcSpecialMotionLookTarget(shen_name name);
std::int32_t npcGetPosFor(shen_name name, Vector3 *position);
std::int32_t GetNpcRestingMotion(const Character *character);
std::uint16_t SelectNpcWalkingMotion(const Character *character);
std::uint16_t SelectNpcRunningMotion(const Character *character);
std::uint16_t SelectNpcStairDescendingMotion(Character *character);
std::uint16_t SelectNpcStairAlternateMotion(Character *character);
std::int32_t GetNpcProfileImageType(Character *character, std::int32_t age, std::int32_t sex, std::int32_t *type);
std::uint16_t SelectNpcProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::uint16_t SelectNpcLeaningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::uint16_t SelectNpcRecliningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);
std::int32_t GetNpcRunCapability(Character *character);
bool QueryNpcCameraMotionProfile(shen_name name, std::uint32_t flags);
bool IsNpcSceneMotionPlaying(shen_name name);
std::uint32_t EV_NPC_NewLoadSw(std::uint32_t state) SHENMUE_NPC_NOEXCEPT;
bool QueryNpcSceneMotionPlaying(shen_name name);
std::int32_t GetScenarioFollowEligibility(shen_name name);
std::uint32_t IsNpcRouteActive(shen_name name);
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name name);
std::int32_t EV_CheckWarmStartNpcRoute(shen_name name);
std::int32_t PrepareNpcRouteReload(shen_name name);
std::int32_t IsNpcRouteReloadPrepared(shen_name name);
void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry *entry);
std::int32_t EV_npcLockUnlock(shen_name name);
std::int32_t EV_npcUpdateFlags(shen_name name);
std::int32_t EV_npcUnknownFunc(shen_name name);
std::uint32_t SetNpcRouteControlHold(shen_name name, std::int32_t enabled);
std::int32_t GetNpcScriptRouteCompletion(shen_name name);
void NpcRegistryTask_Callback(void *);
std::int32_t npcReleasePak(shen_name name);
std::int32_t EV_ResetFreeTalkFlags();
std::int32_t npcReleaseUnusedStreams();
std::int32_t PrepareNpcAllFromArray();
std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT;
std::int32_t EV_ForceLoadNpc(shen_name name);
std::int32_t EV_npcStreamOrLoadHuman(shen_name name);
std::int32_t ReleaseNpcForRouteReload(shen_name name);
std::int32_t ReleaseNpcForRouteReloadAndStoreCount(shen_name name);
std::int32_t IsNpcRouteLoadedOrVisible(shen_name name) SHENMUE_NPC_NOEXCEPT;
std::int32_t EV_npcFree(shen_name name);
std::int32_t NpcWait() SHENMUE_NPC_NOEXCEPT;
std::int32_t CountNpcLiveStreams() SHENMUE_NPC_NOEXCEPT;
bool CanNpcEnterScenario(shen_name name);
void SaveNpcDateTime();
NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;
bool SetNpcRouteYaw(shen_name name, std::int32_t yaw);
void PlaceNpcOutsideDoor(NpcRouteLiveEntry *entry);
void CoreNPCTask();
std::int32_t ReleaseNpcScriptRouteSlot(shen_name name);
std::int32_t CreateScriptNpcRouteSlot(shen_name name);
void ResetNpcEntryToAreaPoint(NpcRouteLiveEntry *entry);
void BeginNpcAreaRoute(NpcRouteLiveEntry *entry, std::int32_t currentTime);
std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry *entry);
std::int32_t MeasureNpcReloadStreamCost(NpcRouteLiveEntry *entry);
std::int32_t RequestNpcStreamLoad(NpcRouteLiveEntry *entry);
void BindNpcRouteCharacter(NpcRouteLiveEntry *entry);
std::int32_t BeginNpcRouteStreamOut(NpcRouteLiveEntry *entry, std::int32_t forced);
std::int32_t UpdateNpcStreamFade(NpcRouteLiveEntry *entry);
void ReleaseNpcRouteStream(NpcRouteLiveEntry *entry);
void ComputeNpcTargetRotation(const Vector3 *position, const Vector3 *target, Vector3i *rotation);
std::int32_t TickNpcSpecialMotion(NpcRouteLiveEntry *entry);
void ReleaseNpcTimedMotion(NpcRouteLiveEntry *entry);
void UpdateStreamedNpcMotion(NpcRouteLiveEntry *entry);
void ApplyNpcCommandMotion(NpcRouteLiveEntry *entry, std::int32_t opcode);
std::int32_t QueueNpcMotionAttachment(NpcRouteLiveEntry *entry, std::uint16_t motion, std::int32_t flags, std::int32_t slot);
void ClearNpcMotionAttachment(NpcRouteLiveEntry *entry, std::int32_t slot);
void CaptureNpcMotionAttachmentFrame(NpcRouteLiveEntry *entry, NpcMotionAttachment *attachment);
void RequestNpcMotionAttachmentRegion(NpcRouteLiveEntry *entry, const NpcMotionAttachment *attachment, std::uint32_t region);
void EvaluateNpcMotionAttachments(NpcRouteLiveEntry *entry);
std::uint32_t SelectNpcRandomMotionAttachment(NpcRouteLiveEntry *entry);
void UpdateNpcMotionAttachments(NpcRouteLiveEntry *entry);
NpcRouteStateContextPrefix *FindNpcRouteStateContext(shen_name name);
std::int32_t BranchNpcIfWeekday(NpcRouteLiveEntry *entry);
std::int32_t BranchNpcIfWeekend(NpcRouteLiveEntry *entry);
void ClearNpcScheduleSeeds();
bool IsNpcScheduleSeedName(shen_name name);
void RestoreNpcScheduleSeedPayload(const void *payload);
#else
shen_name npcFindAreaPointOccupant(shen_name owner);
std::int64_t npcSetDoorAnswerMode(shen_name character, std::int32_t enabled);
std::uint64_t GetNpcNewLoadState();
std::int32_t QueryNpcRouteLiveState(shen_name character);
bool QueryNpcSceneCheck(shen_name character);
bool BeginNpcSpecialMotion(shen_name character);
bool EndNpcSpecialMotion(shen_name character);
bool RequestNpcSpecialMotionTarget(shen_name character, const Vector3 *target);
bool ResetNpcSpecialMotionTarget(shen_name character);
bool RequestNpcSpecialMotionLookTarget(shen_name character, const Vector3 *target);
bool ResetNpcSpecialMotionLookTarget(shen_name character);
std::int32_t npcGetPosFor(shen_name character, Vector3 *position);
std::int64_t MarkNpcRoutePartyControl(shen_name character);
std::int64_t ClearNpcRoutePartyControl(shen_name character);
std::uint64_t QueryNpcCameraMotionProfile(shen_name character, std::uint8_t mask);
void NpcRegistryTask_Nullsub(void *);
bool EnqueueNpcRegistryTask(std::uint8_t nextFunctionIndex);
bool PublishNpcRegistryTask();
bool npcIsNpcTaskActive();
std::uint64_t IsNpcSceneMotionPlaying(shen_name character);
std::uint64_t QueryNpcSceneMotionPlaying(shen_name character);
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character);
std::int64_t EV_CheckWarmStartNpcRoute(shen_name character);
std::int64_t npcWarmStartHumanRoute(shen_name character);
void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry &entry);
std::int64_t npcSetLock(shen_name character, std::int32_t preserveMotion);
std::int64_t EV_npcLockUnlock(shen_name character);
std::int64_t EV_ReleaseNpcLockPreservingMotion(shen_name character);
std::int64_t EV_npcUpdateFlags(shen_name character);
std::int64_t EV_npcUnknownFunc(shen_name character);
std::uint64_t SetNpcRouteControlHold(shen_name character, std::int32_t enabled);
std::uint32_t GetNpcScriptRouteCompletion(shen_name character);
std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry &entry, std::int32_t mode);
bool RequestNpcStreamOut(NpcRouteLiveEntry &entry, std::int32_t type);
bool CanRevealNpcStream(const NpcRouteLiveEntry &entry);
bool HasNpcRuntimeEntry(shen_name character);
std::int64_t EV_ResetFreeTalkFlags();
std::int64_t ResetNpcColdStreams();
std::uint64_t SetNpcColdResetMode(std::uint32_t mode);
std::int32_t npcReleaseUnusedStreams();
std::int32_t PrepareNpcAllFromArray();
void EV_ReconfigureNpcRoutesForStoryStep();
std::int64_t EV_ForceLoadNpc(shen_name character);
std::uint64_t EV_npcStreamOrLoadHuman(shen_name character);
std::uint64_t IsNpcRouteLoadedOrVisible(shen_name character);
std::int64_t EV_npcFree(shen_name character);
void EV_SetAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask, bool visible, bool lowQualityHands);
void SetAreaNpcLowQualityHands(bool enabled);
void SetAreaNpcCharactersVisible(bool enabled);
void AssignAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask);
void ResetConditionalNpcLoads();
std::uint32_t AreConditionalNpcLoadsPending();
HLib::Task *QueueConditionalNpcLoads(const ConditionalNpcLoadEntry *entries);
void AssignGenericNpcLoadContext(const shen_name *characters, HLib::Task *startupTask);
void SetGenericNpcLowQualityHands(bool enabled);
void ResetGenericEventNpcLoadState();
HLib::Task *Queue_EVT_UTL_LoadNPCChara(const shen_name *characters, HLib::Task *startupTask);
void ResetCompiledEventNpcLoadState();
HLib::Task *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLib::Task *startupTask);
void EVT_UTL_LoadNPCChara_Callback_1(void *);
void EnableAuthoredCharacterFlagsAndDisplay(shen_name character);
void EVT_UTL_LoadNPCChara_Callback_2(void *taskData);
std::uint32_t npcReleaseStreamsNearDoor(const Vector3 *point, float radius);
bool NpcWait();
void FreeNpcRegistryTask();
std::int32_t npcSetAreaName(shen_name area, const Vector3 *center, float radius, std::int32_t enabled);
std::uint64_t CountNpcLiveStreams();
std::uint64_t IsNpcLoadedOrVisible(shen_name character);
std::uint64_t IsNpcUsingHumansAfs(shen_name character);
std::int64_t EV_ForceLoadNpcFromHumans(shen_name character);
std::uint64_t EV_npcStreamOrLoadHumanFromHumans(shen_name character);
bool CanNpcEnterScenario(shen_name character);

namespace npc_package_detail {

std::int32_t npcLoadFromPak(shen_name name);
bool npcReleasePak(shen_name name);
bool npcReleaseReadyPak(shen_name name);
void ReleaseNpcTransitionHumanPackages();
void ReleaseReadyNpcTransitionHumanPackages();
bool IsNpcGloballyBlocked(const NpcRouteLiveEntry &entry);
std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, bool fromHumans);
void NpcRegistryTask_Callback(void *);
std::uint16_t ReadU16(const std::byte *source);
std::uint32_t ReadU32(const std::byte *source);
void ReleaseNativeNpcHumanPackages();
void FreeNativeNpcHumans();
bool AllocateNPCH();
void npcSetupHumans();
std::int16_t FindNpcHumanPackageIndex(shen_name name);
void ReleaseNpcRouteStream(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, bool clearPackageAlias);
void BeginNpcRouteStreamOut(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime);
bool PublishExistingNpcCharacter(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, Character *character);
bool EnsureNpcRouteStreamReady(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime);
float NpcDistanceXZ(const Vector3 &first, const Vector3 &second);
bool IsInsideNpcNoSpawnRadius(const Vector3 &position, float radius);
std::int32_t AdjustedNpcStreamTarget();
bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry);
std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry &entry, bool reload);
void RankNpcStreamEntry(NpcRouteLiveEntry &entry, std::uint8_t &slotFlags, const Vector3 &cameraPosition);
void UpdateNpcStreamFade(NpcRouteLiveEntry &entry);
void EvaluateNpcMotionAttachments(NpcRouteLiveEntry &entry);
void UpdateNpcMotionAttachments(NpcRouteLiveEntry &entry);
bool CheckNpcCachedVisibility(NpcRouteLiveEntry &entry, const Vector3 &position, float radius);
void UpdateNpcFrameCulling(NpcRouteLiveEntry &entry);
std::int32_t FindConditionalNpcLoadSlot(shen_name name);
void ConditionalNpcLoadCheckCallback(void *data);
void ConditionalNpcLoadCallback(void *data);

} // namespace npc_package_detail
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
CharacterDrawCallback mtwkAssignDrawCallback(Character *character, CharacterDrawCallback callback) {
    if (character) {
        const CharacterDrawCallback previous = character->collisionCheck;
        character->collisionCheck = callback;
        return previous;
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t IsNpcRegistryTaskActive() SHENMUE_NPC_NOEXCEPT {
    return npcRegistryTask != SHENMUE_NPC_NULL;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetNpcNewLoadState() SHENMUE_NPC_NOEXCEPT {
    return npcNewLoadState;
}

#else

std::uint64_t GetNpcNewLoadState() {
    return npcNewLoadState;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t EV_NPC_NewLoadSw(std::uint32_t state) SHENMUE_NPC_NOEXCEPT {
    npcNewLoadState = state;
    return state;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool QueryNpcSceneMotionPlaying(shen_name name) {
    return IsNpcRegistryTaskActive() ? IsNpcSceneMotionPlaying(name) : false;
}

std::int32_t NpcRegistryUnusedQueryA() {
    if (IsNpcRegistryTaskActive())
        return 0;
    return 0;
}

std::int32_t NpcRegistryUnusedQueryB() {
    if (IsNpcRegistryTaskActive())
        return 0;
    return 0;
}

#else

// @verified - p
std::uint64_t QueryNpcSceneMotionPlaying(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    return IsNpcSceneMotionPlaying(character);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void NpcRegistryTask_Callback(void *) {
    npcHumansInUse = 1;
    npcHasCachedPak = 0;
    PrepareNpcAreaRoutes();
    CoreNPCTask();
    DrawNpcTransitions();
    npcHumansInUse = 0;
}
#endif

#else

namespace npc_package_detail {

std::int32_t npcLoadFromPak(shen_name name) {
    if (!npcHumanIndex)
        return -1;
    const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
    for (std::int32_t index = 0; index < npcHumanCount; ++index)
        if (names[index] == name.name)
            return index;
    return -1;
}

bool npcReleasePak(shen_name name) {
    const std::int32_t index = npcLoadFromPak(name);
    if (index < 0)
        return false;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    if (slot.state == NpcHumanPakState::Empty || slot.state == NpcHumanPakState::Requested) {
        slot.state = NpcHumanPakState::Empty;
        return false;
    }
    if (slot.state != NpcHumanPakState::Ready && slot.state != NpcHumanPakState::Attached)
        return false;
    DestroyPakFileAFS(slot.pak);
    slot.pak = nullptr;
    slot.state = NpcHumanPakState::Empty;
    return true;
}

bool npcReleaseReadyPak(shen_name name) {
    const std::int32_t index = npcLoadFromPak(name);
    if (index < 0 || npcHumanSlots[index].state != NpcHumanPakState::Ready)
        return false;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    DestroyPakFileAFS(slot.pak);
    slot.pak = nullptr;
    slot.state = NpcHumanPakState::Empty;
    return true;
}

void ReleaseNpcTransitionHumanPackages() {
    for (NpcRouteResetGroupHeader *group = npcRouteResetGroup; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name != npcLoadedArea.name)
                continue;
            npcReleasePak(entry->character);
            entry->nodeOffset = 0;
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        if (!slot.character.name)
            continue;
        npcReleasePak(slot.character);
        slot.node = nullptr;
    }
}

void ReleaseReadyNpcTransitionHumanPackages() {
    for (NpcRouteResetGroupHeader *group = npcRouteResetGroup; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name == npcLoadedArea.name && npcReleaseReadyPak(entry->character))
                entry->nodeOffset = 0;
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        if (slot.character.name && npcReleaseReadyPak(slot.character))
            slot.node = nullptr;
    }
}

bool IsNpcGloballyBlocked(const NpcRouteLiveEntry &entry) {
    if (entry.stateContext.flags & 1U) {
        if ((entry.stateContext.flags & 0x300U) || (entry.stateContext.routeFlags & 0x10U))
            return false;
        if (!(entry.flags & 0x4000000U))
            return true;
    }
    return (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x10U) != 0;
}

std::int32_t npcLoadHuman(shen_name packageName, shen_name characterName, bool fromHumans) {
    if (!npcHumanSlots)
        return 0;
    const std::int32_t index = npcLoadFromPak(packageName);
    if (index < 0)
        return -1;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    if (slot.state == NpcHumanPakState::Attached)
        return 1;
    if (slot.state == NpcHumanPakState::Ready) {
        const std::int32_t readiness = GetPakReadiness(slot.pak);
        if (readiness < 0) {
            npcReleasePak(packageName);
            return -6;
        }
        if (!readiness)
            return 0;
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const character = scenFindChara(control ? control->scenarioTask : nullptr, characterName);
        if (!character) {
            npcReleasePak(packageName);
            return -5;
        }
        char filename[9] = {};
        std::memcpy(filename, &characterName.name, 4);
        std::memcpy(filename + 4, ".bin", 5);
        AttachScenarioFreeTalkCharacter(character, FileLoaderView(slot.pak, filename, HAND_BANK_EXTENSION));
        slot.state = NpcHumanPakState::Attached;
        return 1;
    }
    if (slot.state == NpcHumanPakState::Empty)
        slot.state = NpcHumanPakState::Requested;
    if (slot.state != NpcHumanPakState::Requested)
        return -8;
    if (PakLoadWait())
        return 0;
    const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
    const std::int32_t requested = static_cast<std::int32_t>(names[index + npcHumanCount] + 3096U);
    if (!requested) {
        slot.state = NpcHumanPakState::Empty;
        return -2;
    }
    const std::uint64_t size = static_cast<std::uint64_t>(static_cast<std::int64_t>(requested));
    std::uint64_t totalFree;
    std::uint64_t fragmentation;
    HLib::TryGetFreeHeapStats(&totalFree, &fragmentation);
    if (size > totalFree) {
        slot.state = NpcHumanPakState::Empty;
        return -4;
    }
    if (!npcHumansInUse || !npcHasCachedPak) {
        HLib::AnalyzeFreeHeap(&totalFree, &npcLargestDynamicBlock, &fragmentation);
        npcLargestDynamicBlock = npcLargestDynamicBlock > 65536U ? npcLargestDynamicBlock - 65536U : 0;
        if (totalFree < 3862528U)
            npcLargestDynamicBlock = 0;
        HLib::AnalyzeFreeList(&totalFree, &npcLargestSceneBlock, &fragmentation);
        npcLargestSceneBlock = npcLargestSceneBlock > 65536U ? npcLargestSceneBlock - 65536U : 0;
        if (totalFree < 1572864U)
            npcLargestSceneBlock = 0;
        npcHasCachedPak = true;
    }
    if (size > npcLargestDynamicBlock || size > npcLargestSceneBlock) {
        slot.state = NpcHumanPakState::Empty;
        return -3;
    }
    slot.pak = RequestPakFromAfs(npcHumansAfs, index * 2, static_cast<std::int16_t>(index * 2 + 1), fromHumans ? 1U : 0U);
    if (slot.pak) {
        slot.state = NpcHumanPakState::Ready;
        npcHasCachedPak = false;
    }
    return 0;
}
void NpcRegistryTask_Callback(void *) {
    if (HLib::GetCurrentTask() != npcRegistryTask.current) {
        HLib::DestroyCurrentTask();
        return;
    }
    npcHumansInUse = 1;
    npcHasCachedPak = false;
    TickNpcRouteLiveEntries();
    DrawNpcTransitions();
    npcHumansInUse = 0;
}

std::uint16_t ReadU16(const std::byte *source) {
    std::uint16_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

std::uint32_t ReadU32(const std::byte *source) {
    std::uint32_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

void ReleaseNativeNpcHumanPackages() {
    if (npcHumanIndex && npcHumanSlots) {
        const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
        for (std::int32_t index = 0; index < npcHumanCount; ++index)
            npcReleasePak(shen_name{names[index]});
    }
    npcHasCachedPak = false;
}

void FreeNativeNpcHumans() {
    ReleaseNativeNpcHumanPackages();
    HLib::FreeTaskData(npcHumanSlots);
    HLib::FreeTaskData(npcHumanIndex);
    npcHumanSlots = nullptr;
    npcHumanIndex = nullptr;
    npcHumanCount = 0;
    npcHasCachedPak = false;
}

// Native AllocateNPCH (npcInitData): HUMANS.idx and one pack slot per human, kept for the act.
bool AllocateNPCH() {
    char directory[32];
    std::snprintf(directory, sizeof(directory), "scene/%02d/npc", g_loadedActNumber);
    std::uint32_t indexSize = 0;
    void *indexData = LoadLegacySceneFile(directory, "HUMANS.idx", NPC_HUMAN_INDEX_MEMORY, &indexSize);
    if (!indexData)
        return false;
    npcHumanIndex = static_cast<std::byte *>(indexData);
    if (indexSize < 4) {
        FreeNativeNpcHumans();
        return false;
    }
    npcHumanCount = static_cast<std::int32_t>(ReadU32(npcHumanIndex));
    if (npcHumanCount <= 0 || 4ULL + static_cast<std::uint64_t>(npcHumanCount) * 8U > indexSize) {
        FreeNativeNpcHumans();
        return false;
    }
    npcHumanSlots = static_cast<NpcHumanPakSlot *>(HLib::AllocateTaskData(sizeof(NpcHumanPakSlot) * npcHumanCount, NPC_HUMAN_SLOT_MEMORY));
    if (!npcHumanSlots) {
        FreeNativeNpcHumans();
        return false;
    }
    HLib::SetTaskDataMotionFlag(npcHumanIndex);
    HLib::SetTaskDataMotionFlag(npcHumanSlots);
    for (std::int32_t index = 0; index < npcHumanCount; ++index) {
        npcHumanSlots[index].state = NpcHumanPakState::Empty;
        npcHumanSlots[index].pak = nullptr;
    }
    return true;
}

// Native npcSetupHumans (npc_Callback_Enqueue): each map mounts HUMANS.afs and empties every pack slot.
void npcSetupHumans() {
    if (!npcHumanSlots)
        return;
    char directory[32];
    std::snprintf(directory, sizeof(directory), "scene/%02d/npc", g_actNumber);
    npcHumansAfs = MountPakAfs(directory, "HUMANS.afs");
    SetupPakSpace(1024);
    for (std::int32_t index = 0; index < npcHumanCount; ++index) {
        npcHumanSlots[index].state = NpcHumanPakState::Empty;
        npcHumanSlots[index].pak = nullptr;
    }
}

std::int16_t FindNpcHumanPackageIndex(shen_name name) {
    for (std::size_t index = 0; index < npcSchedule.humanPackages.size(); ++index) {
        if (npcSchedule.humanPackages[index].name != name.name)
            continue;
        if (index > static_cast<std::size_t>((std::numeric_limits<std::int16_t>::max)())) {
            return -1;
        }
        return static_cast<std::int16_t>(index);
    }
    return -1;
}

void ReleaseNpcRouteStream(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, bool clearPackageAlias) {
    npcReleasePak(ReadNpcLiveField<shen_name>(entry, npcLoadHumanNameOffset));
    npcActiveStreamCost -= entry.streamCost;
    if (entry.flags & 0x10U) {
        entry.liveState = entry.previousLiveState;
        entry.flags &= ~0x00004010U;
        entry.specialMotionPhase = 0;
    }
    entry.flags &= ~0xa2000017U;
    entry.node = nullptr;
    entry.motionWork = nullptr;
    if (entry.flags & 0x400U)
        static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle)->character = nullptr;
    npcSlotFlags[entry.poolIndex] &= ~1U;
    entry.streamRouteChangeKind = 0;
    entry.field335 = 0;
    if (entry.stateContext.flags & 0x101U) {
        entry.stateContext.flags &= 0xff9efbffU;
        entry.stateContext.motionWork = nullptr;
    }
    entry.loadHumanResult = 8;
    runtime.character = nullptr;
    runtime.packageAlias = {};
    runtime.streamState = NpcHumanStreamState::Inactive;
    if (clearPackageAlias) {
        const shen_name noAlias{};
        WriteNpcLiveField(entry, npcLoadHumanNameOffset, noAlias);
    }
}

void BeginNpcRouteStreamOut(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime) {
    if (runtime.streamState == NpcHumanStreamState::Inactive && !runtime.character && !entry.node) {
        return;
    }
    entry.flags &= ~NRSF_Live;
    entry.flags |= NRSF_FadeOut;
    runtime.streamState = NpcHumanStreamState::ReleasePending;
}

bool PublishExistingNpcCharacter(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, Character *character) {
    if (!character)
        return false;
    MotionWork *const motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character);
    if (!motionWork)
        return false;
    const std::uint32_t streamFlags = entry.flags;
    entry.node = character;
    entry.motionWork = motionWork;
    runtime.character = character;
    runtime.streamState = NpcHumanStreamState::Live;
    entry.flags &= ~(NRSF_Requested | NRSF_FadeOut | NRSF_StartHidden);
    entry.flags |= NRSF_Live;
    npcSlotFlags[entry.poolIndex] |= 1U;
    entry.streamRouteChangeKind = 0;
    entry.streamRadius = 0.30000001f;
    InitializeStreamedNpcRoute(entry);
    // HandleCharaStates -> UpdateStreamedNpcMotion publishes the MTWK update bit;
    // StreamNpcCharacters then applies its two explicit hidden-entry gates.
    motionWork->runtime.callbackFlags |= 1U;
    SetFaceFlagsForChara_0(character, (streamFlags & (NRSF_AbstractActive | NRSF_StartHidden)) != NRSF_StartHidden);
    if (entry.name.name == SHEN_NAME(SIN_)) {
        SetLowQualityHands(character);
        SetCharacterHandRegionEnabled(character, 0, true, 1);
        SetCharacterHandRegionEnabled(character, 1, true, 1);
        SetCharacterType(character, CharacterTypeStencilShadow);
    }
    const bool startHidden = (streamFlags & (NRSF_AbstractActive | NRSF_ForceHidden)) == 0 && (streamFlags & NRSF_StartHidden) != 0;
    if ((streamFlags & NRSF_ForceHidden) != 0 || startHidden) {
        character->renderFlags &= ~CRF_All;
        if (CharacterControlModel *const control = FindCharacterControlModel(character)) {
            control->flags &= ~CCMF_VoxelSweep;
        }
        if (startHidden)
            entry.flags |= NRSF_LoadedHidden;
    }
    const std::int8_t loaded = 1;
    WriteNpcLiveField(entry, npcLoadHumanResultOffset, loaded);
    BindNpcRouteCharacter(entry.name, character);
    return true;
}

bool EnsureNpcRouteStreamReady(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime) {
    const shen_name alias = ReadNpcLiveField<shen_name>(entry, npcLoadHumanNameOffset);
    runtime.packageAlias = alias;
    const std::int32_t result = npcLoadHuman(alias, entry.name, (entry.flags & NRSF_UseHumansAfs) != 0);
    npcLastHumanLoadResult = result;
    WriteNpcLiveField(entry, npcLoadHumanResultOffset, static_cast<std::int8_t>(result));
    if (!result) {
        runtime.streamState = NpcHumanStreamState::Requested;
        return false;
    }
    entry.streamRouteChangeKind = 0;
    if (result < 0) {
        runtime.streamState = NpcHumanStreamState::Missing;
        return false;
    }
    return PublishExistingNpcCharacter(entry, runtime, ctrlGetScenarioChara(entry.name));
}

float NpcDistanceXZ(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x;
    const float z = first.z - second.z;
    return std::sqrt(x * x + z * z);
}

bool IsInsideNpcNoSpawnRadius(const Vector3 &position, float radius) {
    std::int32_t remaining = npcRegistryTask.areaCount;
    for (const NpcNoSpawnArea &area : npcRegistryTask.areas) {
        if (!remaining || !area.name.name)
            break;
        if (area.radius + radius > NpcDistanceXZ(area.center, position))
            return true;
        --remaining;
    }
    return false;
}

std::int32_t AdjustedNpcStreamTarget() {
    std::int32_t target = npcPoolTarget;
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_DRAW_DISTANCE
    if (mods::Enabled(mods::Id::DrawDistance))
        target += 66;
#endif
    if (npcPoolMinimum != -1 && npcPoolMinimum <= target)
        return std::max(npcPoolMinimum, target - static_cast<std::int32_t>(static_cast<float>(target) * npcCrowdDensityScale * 0.5f));
    return target;
}

bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry) {
    NpcRouteRuntimeRecord &runtime = *GetNpcRuntimeRecord(entry);
    npcLastHumanLoadResult = 0;
    if (!entry.streamRouteChangeKind && (entry.flags & 2U)) {
        npcLastHumanLoadResult = 1;
        return true;
    }
    if (!entry.streamRouteChangeKind && (entry.flags & 4U)) {
        entry.streamRouteChangeKind = 0;
        entry.field335 = 0;
        entry.flags = (entry.flags & ~0x1006U) | 2U;
        runtime.streamState = NpcHumanStreamState::Live;
        if (!(entry.flags & 0x80000000U))
            npcSetAlphaOnOff(entry.node, true);
        npcLastHumanLoadResult = 1;
        return true;
    }
    if (!entry.streamRouteChangeKind) {
        if (!npcNewLoadState && (entry.flags & 0x1000U)) {
            entry.flags &= ~1U;
            return true;
        }
        if (AdjustedNpcStreamTarget() <= npcActiveStreamCost && !(entry.flags & 8U))
            return true;
        entry.flags |= 1U;
        entry.field342 = 0;
        npcActiveStreamCost += entry.streamCost;
        entry.streamRouteChangeKind = 1;
    }
    if (entry.streamRouteChangeKind != 1)
        return false;
    if (EnsureNpcRouteStreamReady(entry, runtime))
        return true;
    if (runtime.streamState == NpcHumanStreamState::Missing) {
        entry.streamRouteChangeKind = 0;
        entry.flags &= ~1U;
        npcActiveStreamCost -= entry.streamCost;
        npcWaitSawLoadFailure = true;
    }
    return false;
}

std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry &entry, bool reload) {
    if (entry.flags & (reload ? 3U : 7U))
        return entry.streamCost;
    if ((entry.flags & 0x20800028U) || (!npcColdResetMode && IsNpcGloballyBlocked(entry)))
        return 0;
    Vector3 floor = entry.localPosition;
    if (entry.flags & 0x2000U)
        entry.localPosition.y = entry.forcedY;
    else if (!FindNpcIntersectionInScene(floor, entry.packedMoveFlags) && !(entry.flags & 0x1000000U))
        return 0;
    if (reload) {
        if ((entry.stateContext.flags & 1U) && IsInsideNpcNoSpawnRadius(entry.localPosition, 0.40000001f))
            return 0;
        std::int8_t countdown = static_cast<std::int8_t>(entry.streamingCountdown);
        if (countdown) {
            --countdown;
            entry.streamingCountdown = static_cast<std::uint16_t>((entry.streamingCountdown & 0xff00U) | static_cast<std::uint8_t>(countdown));
            if (countdown > 0)
                return 0;
            entry.streamingCountdown &= 0xff00U;
        }
        entry.flags |= 0x1000U;
        RequestNpcStreamLoad(entry);
    }
    return entry.streamCost;
}

void RankNpcStreamEntry(NpcRouteLiveEntry &entry, std::uint8_t &slotFlags, const Vector3 &cameraPosition) {
    Vector3 position;
    if (std::abs(entry.localPosition.y) >= 0.00000011920929f)
        position = TransformNpcPoint(entry.localPosition);
    else
        position = {cameraPosition.x - entry.localPosition.x, cameraPosition.y - entry.localPosition.y, cameraPosition.z - entry.localPosition.z};
    position.z += 4.5f;
    const float first = std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    position.z += npcStreamThrottleWithBias;
    float distance = first + std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    if (distance >= 50.0f)
        slotFlags &= ~2U;
    else
        slotFlags |= 2U;
    if (entry.streamDistanceBias != 0.0f)
        distance = std::fmax(0.0f, distance - entry.streamDistanceBias);
    const float key = (entry.flags & 0x200U) ? 0.0f : distance + 0.0099999998f;
    NpcRouteLiveEntry *insertion = &entry;
    if (entry.previous) {
        if (entry.previous->streamSortKey <= key && entry.next) {
            for (NpcRouteLiveEntry *cursor = entry.next; cursor && cursor->streamSortKey < key; cursor = cursor->next)
                insertion = cursor;
        } else {
            for (NpcRouteLiveEntry *cursor = entry.previous; cursor && key < cursor->streamSortKey; cursor = cursor->previous)
                insertion = cursor;
        }
    } else {
        for (NpcRouteLiveEntry *cursor = entry.next; cursor && cursor->streamSortKey < key; cursor = cursor->next)
            insertion = cursor;
    }
    entry.streamSortKey = key;
    if (insertion == &entry || insertion->next == &entry)
        return;
    if (entry.previous)
        entry.previous->next = entry.next;
    else
        npcCrowdHead = entry.next;
    if (entry.next)
        entry.next->previous = entry.previous;
    entry.next = insertion->next;
    entry.previous = insertion;
    if (entry.next)
        entry.next->previous = &entry;
    insertion->next = &entry;
}

void UpdateNpcStreamFade(NpcRouteLiveEntry &entry) {
    if (!IsOfThisVisibilityType(entry.node, 5))
        entry.field335 = 15;
    if (!entry.field335) {
        npcSetAlphaOnOff(entry.node, false);
        ++entry.field335;
    } else if (entry.field335 == 15) {
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
    } else if (std::abs(entry.node->alpha) >= 0.00000011920929f) {
        npcSetAlphaOnOff(entry.node, false);
    } else
        entry.field335 = 15;
}

void EvaluateNpcMotionAttachments(NpcRouteLiveEntry &entry) {
    for (NpcMotionAttachment &attachment : entry.motionAttachments) {
        if (!attachment.motion) {
            attachment.flags = 0;
            continue;
        }
        attachment.frame = -12.0f;
        for (std::uint32_t region = 1; region < 5; ++region) {
            if (!(attachment.flags & (1U << region)) || !mtwkSetRegionFlag(entry.motionWork, 72, 1, static_cast<MotionRegion>(region)) ||
                mtwkGetAnimationId(entry.motionWork, static_cast<MotionRegion>(region)) != attachment.motion)
                continue;
            attachment.frame = mtwkGetFrame(entry.motionWork, static_cast<MotionRegion>(region));
            if (mtwkSetRegionFlag(entry.motionWork, 68, 1, static_cast<MotionRegion>(region)))
                attachment.frame -= 12.0f;
            break;
        }
    }
    bool completed[3]{};
    for (std::uint32_t region = 1; region < 5; ++region) {
        std::int32_t selected = -1;
        for (std::int32_t slot = 0; slot < 3; ++slot) {
            const NpcMotionAttachment &attachment = entry.motionAttachments[slot];
            if (attachment.motion && !(attachment.flags & 0x40U) && (attachment.flags & (1U << region))) {
                selected = slot;
                break;
            }
        }
        if (selected == -1) {
            mtwkRequestRegionDetach(entry.motionWork, static_cast<MotionRegion>(region));
            continue;
        }
        const NpcMotionAttachment &attachment = entry.motionAttachments[selected];
        std::uint16_t active = mtwkGetAnimationId(entry.motionWork, static_cast<MotionRegion>(region));
        if (!mtwkSetRegionFlag(entry.motionWork, 72, 1, static_cast<MotionRegion>(region)))
            active = 0;
        const std::uint16_t queued = mtwkGetQueuedAnimationId(entry.motionWork, static_cast<MotionRegion>(region));
        if ((!active && !queued) || (active && active != attachment.motion) || (queued && queued != attachment.motion)) {
            const MotnSequence *const sequence = FindLoadedMotionSequence(attachment.motion);
            const float last = sequence ? static_cast<float>(sequence->frameCount) - 1.0f : -1.0f;
            if (last < 0.0f)
                continue;
            float frame = (attachment.frame + 12.0f) * mtwkGetSpeed(entry.motionWork, static_cast<MotionRegion>(region));
            if (attachment.flags & 0x20U) {
                while (frame > last)
                    frame -= last;
            } else
                frame = std::fmin(last, frame);
            mtwkRequestMotion(entry.motionWork, attachment.motion, static_cast<MotionRegion>(region));
            mtwkSetRegionEvalTime(entry.motionWork, frame, static_cast<MotionRegion>(region));
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            mtwkSetFrameTime(entry.motionWork, 12, static_cast<MotionRegion>(region));
        } else if (!(attachment.flags & 0x20U) && mtwkSetRegionFlag(entry.motionWork, 65, 1, static_cast<MotionRegion>(region))) {
            mtwkRequestRegionDetach(entry.motionWork, static_cast<MotionRegion>(region));
            completed[selected] = true;
        }
    }
    for (std::int32_t slot = 0; slot < 3; ++slot)
        if (completed[slot])
            ClearNpcMotionAttachment(entry, slot);
}

void UpdateNpcMotionAttachments(NpcRouteLiveEntry &entry) {
    if (entry.controlFlags & 0x20U)
        return;
    if (!entry.node) {
        entry.idleCounter = 0;
        return;
    }
    if (mtwkSetExtraFlag(entry.motionWork, 60, 1))
        return;
    NpcMotionAttachment &first = entry.motionAttachments[0];
    if ((entry.controlFlags & 0x10U) || (entry.flags & 0x100000U))
        first.flags |= 0x40U;
    if (npcCrowdDensityScale >= 0.15f) {
        const std::uint16_t rain = RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe353);
        if ((first.motion && first.motion != rain) || (!first.motion && !(entry.flags & 0x10U)))
            SetNpcMotionAttachment(entry, rain, 40, 0);
    } else if (first.motion && !(entry.flags & 0x10U))
        ClearNpcMotionAttachment(entry, 0);
    if (entry.controlFlags & 3U) {
        mtwkRequestRegionDetach(entry.motionWork, MotionRegion::LowerBody);
        if (entry.controlFlags & 1U) {
            ClearNpcMotionAttachment(entry, 0);
            ClearNpcMotionAttachment(entry, 1);
            ClearNpcMotionAttachment(entry, 2);
        }
    } else {
        EvaluateNpcMotionAttachments(entry);
        if ((entry.controlFlags & 4U) && !entry.motionAttachments[2].motion) {
            if (++entry.idleCounter <= 90)
                return;
            if (IncrementSharedPrngFloat() * 3.0f <= 1.0f && !SetNpcMotionAttachment(entry, 32861, 20, 2))
                ClearNpcMotionAttachment(entry, 2);
        }
    }
    entry.idleCounter = 0;
}

// @verified - p
bool CheckNpcCachedVisibility(NpcRouteLiveEntry &entry, const Vector3 &position, float radius) {
    if (entry.node->field99)
        return (entry.node->field99 & 1U) != 0;
    if (!camCtrlIsInsideHorizontalDepthRect(position, IsCharacterMirroredForDraw(entry.node) ? 5.0f : 0.60000002f))
        return false;
    const bool visible = camCtrlCheckPosition(position, radius);
    entry.node->field99 = visible ? 3 : 2;
    return visible;
}

void UpdateNpcFrameCulling(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    mtwkSetCulled(entry.motionWork, false);
    if (npcUsePositionCullCallback) {
        if ((entry.flags & 0x1058U) || (entry.stateContext.flags & 1U) || (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x20U))
            return;
        if (CheckNpcCachedVisibility(entry, GetCharacterWorldPosition(entry.node), 0.60000002f))
            return;
        mtwkSetCulled(entry.motionWork, true);
        entry.node->renderFlags &= ~CRF_InRange;
        entry.node->position = {};
        return;
    }
    if ((entry.flags & 0x10U) || (entry.routeMotionState != 16 && !(entry.flags & 0x8000U)))
        return;
    Vector3 position = GetCharacterWorldPosition(entry.node);
    if (!CheckNpcCachedVisibility(entry, position, 0.40000001f)) {
        mtwkSetCulled(entry.motionWork, true);
        entry.node->renderFlags &= ~CRF_InRange;
        entry.node->position = {};
    } else {
        if (!mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody))
            return;
        position = TransformNpcPoint(position);
        if (position.x * position.x + position.y * position.y + position.z * position.z <= 729.0f)
            return;
        mtwkSetCulled(entry.motionWork, true);
        entry.node->renderFlags |= CRF_InRange;
    }
    const std::uint16_t counter = static_cast<std::uint16_t>((entry.field2d8 >> 16) + 1);
    entry.field2d8 = (entry.field2d8 & ~0xffff0000ULL) | (static_cast<std::uint64_t>(counter) << 16);
}
} // namespace npc_package_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry &entry, std::int32_t mode) {
    if (entry.loadHumanResult < 0)
        entry.loadHumanResult = 8;
    if (entry.flags & 8U)
        return 2;
    if (entry.flags & NRSF_Pinned)
        return 3;
    if (!(entry.flags & 7U))
        return 4;
    FlushQueuedLegacyDrawPasses();
    if (!mode || (mode == 2 && (entry.flags & 1U))) {
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
        return 1;
    }
    if (mode != 1)
        return 1;
    const std::int32_t pending = entry.flags & 1U;
    if (pending)
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
    if ((entry.flags & 0x1aU) == 2) {
        entry.field335 = 0;
        entry.flags = (entry.flags & ~6U) | 4U;
    }
    if (entry.flags & 4U)
        UpdateNpcStreamFade(entry);
    return pending;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool RequestNpcStreamOut(NpcRouteLiveEntry &entry, std::int32_t type) {
    if (!(entry.flags & 2U) || (entry.flags & 0x18U))
        return false;
    entry.flags = (entry.flags & ~6U) | 4U;
    if (type == 1)
        entry.flags |= 0x20000000U;
    else if (type == 2)
        entry.flags |= 0x22000000U;
    entry.field335 = 0;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool CanRevealNpcStream(const NpcRouteLiveEntry &entry) {
    if (entry.streamRadius + npcPlayerRadius > NpcDistanceXZ(npcPlayerCenter, entry.streamCenter))
        return false;
    if (entry.streamCost == 1)
        return true;
    if (IsInsideNpcNoSpawnRadius(entry.streamCenter, entry.streamRadius))
        return false;
    std::int32_t cost = npcActiveStreamCost - entry.streamCost;
    for (const NpcRouteLiveEntry *other = npcCrowdHead; other && cost; other = other->next) {
        if (other == &entry || !(other->flags & 7U))
            continue;
        cost -= other->streamCost;
        if (!(other->flags & 0x80000041U) && other->streamCost != 1 && (!(other->flags & 0x8000000U) || !(entry.flags & 0x8000000U)) &&
            std::abs(entry.localPosition.y - other->localPosition.y) <= 1.8f &&
            other->streamRadius + entry.streamRadius > NpcDistanceXZ(other->streamCenter, entry.streamCenter))
            return false;
    }
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool HasNpcRuntimeEntry(shen_name character) {
    return FindActiveNpcEntry(character) != nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t npcReleasePak(shen_name name) {
    const std::int32_t index = npcLoadFromPak(name);
    if (index < 0)
        return 0;
    switch (npcHumanSlots[index].state) {
    case NpcHumanPakState::Empty:
    case NpcHumanPakState::Requested:
        npcHumanSlots[index].state = NpcHumanPakState::Empty;
        return 0;
    case NpcHumanPakState::Ready:
    case NpcHumanPakState::Attached: {
        CancelPakSlot(npcHumanSlots[index].pak);
        NpcHumanPakSlot *const slots = npcHumanSlots;
        slots[index].pak = SHENMUE_NPC_NULL;
        slots[index].state = NpcHumanPakState::Empty;
        return 1;
    }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t npcLoadFromPak(shen_name name) SHENMUE_NPC_NOEXCEPT {
    const std::uint32_t *names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex);
    const std::int32_t count = static_cast<std::int32_t>(*names++);
    for (std::int32_t index = 0; index < count; ++index)
        if (name == *names++)
            return index;
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t npcViewCullingEnabled = 0;

bool SelectNpcScheduleBranch(NpcRouteLiveEntry *entry, std::int32_t index);

std::int32_t UpdateNpcViewVisibility(NpcRouteLiveEntry *entry) {
    if (entry->node) {
        SetCharacterUnknownFlag(entry->node, 0);
        if (!npcViewCullingEnabled)
            return 1;
        if (entry->flags & 0x1058U)
            return 1;
        Vector3 position;
        GetCharacterWorldPosition(entry->node, &position);
        if (camCtrlIsInsideDepthScaledRect(position, 1.6f))
            return 1;
        SetCharacterUnknownFlag(entry->node, 1);
    }
    return 0;
}

NpcRouteLiveEntry *GetNpcRouteEntryByIndex(std::int32_t index) {
    return reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage) + index;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool SetNpcRouteYaw(shen_name name, std::int32_t yaw) {
    NpcRouteLiveEntry *entry;
    if ((entry = FindNpcRouteEntry(name)) == 0)
        return false;
    entry->rotation.y = yaw;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
shen_name npcFindAreaPointOccupant(shen_name owner) {
    const std::uint32_t area = npcLoadedArea & 0xffffU;
    for (std::int32_t point = 0; point < npcScheduleAreaPointCount; ++point) {
        if (GetNpcAreaPointOwnerName(point) != owner || static_cast<std::uint16_t>(GetNpcAreaPointAreaName(point)) != area)
            continue;
        const NpcRouteLiveEntry *entry = reinterpret_cast<const NpcRouteLiveEntry *>(npcEntryStorage);
        for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
            if ((entry->flags & 0x800U) && entry->currentAreaPoint == point)
                return entry->name;
    }
    return shen_name();
}

#else

// The NPC standing at one of the owner's area points in the loaded area, on the current
// floor.
shen_name npcFindAreaPointOccupant(shen_name owner) {
    for (std::uint16_t point = 0; point < npcSchedule.areaPointCount; ++point) {
        const std::byte *const record = GetNpcAreaPoint(point);
        if (!record || ReadU32(record + 0x30) != owner.name || static_cast<std::uint16_t>(ReadU32(record)) != static_cast<std::uint16_t>(npcLoadedArea.name))
            continue;
        for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
            const NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (!(entry.flags & 0x800U) || entry.currentAreaPoint != point)
                continue;
            if (npcSchedule.floor != -1000 && npcSchedule.floor != static_cast<std::int8_t>(entry.packedMoveFlags >> 8U))
                continue;
            return entry.name;
        }
    }
    return {};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t AcquireNpcTimedMotionSlot(NpcRouteLiveEntry *entry) {
    if (npcTimedMotionCount == 2)
        return 0;
    for (std::int32_t index = 0; index < 2; ++index) {
        if (static_cast<std::int16_t>(npcTimedMotionSlots[index].occupied) != 1) {
            npcTimedMotionSlots[index].occupied = 1;
            entry->timedMotionHandle = &npcTimedMotionSlots[index];
            entry->timedMotionHandle->character = 0;
            ++npcTimedMotionCount;
            return 1;
        }
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Parks (1) or restores (0) an answering NPC's eye-control or route-direction mode, saving
// the other in +0x338 (negated for an eye mode).
std::int64_t npcSetDoorAnswerMode(shen_name character, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        const std::uint32_t flags = entry.flags;
        if (!enabled) {
            if (flags & 0x200000U) {
                const auto saved = static_cast<std::int8_t>(entry.field338);
                if (saved >= 0)
                    entry.routeDirectionMode = saved;
                else
                    entry.eyeControlMode = static_cast<std::int8_t>(-saved);
                entry.flags = flags & ~0x200000U;
            }
        } else if (!(flags & 0x200000U)) {
            const auto eye = static_cast<std::uint8_t>(entry.eyeControlMode);
            if (eye) {
                entry.eyeControlMode = 13;
                entry.field338 = static_cast<std::uint8_t>(-eye);
            } else {
                entry.field338 = static_cast<std::uint8_t>(entry.routeDirectionMode);
                entry.routeDirectionMode = 13;
            }
            entry.flags = flags | 0x200000U;
        }
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseNpcTimedMotion(NpcRouteLiveEntry *entry) {
    if (!entry->timedMotionHandle)
        return;
    if (entry->timedMotionHandle->character)
        EV_SetCharDisplay(entry->timedMotionHandle->character, CVM_DisplayEnabled, 0);
    entry->flags &= ~0x400U;
    entry->timedMotionHandle->occupied = 0;
    --npcTimedMotionCount;
    entry->timedMotionHandle = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ReplaceNpcStateCallback(NpcRouteLiveEntry *entry, std::int32_t (*callback)(NpcRouteLiveEntry *)) {
    if (!callback)
        return 0;
    entry->stateCallback = callback;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool npcExists(shen_name name) {
    if (FindNpcRouteEntry(name))
        return true;
    return false;
}

std::int32_t HasHiddenNpcWithoutRoute() {
    const NpcRouteLiveEntry *entry = reinterpret_cast<const NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
        if ((entry->flags & 0x40U) && ((&entry->stateContext)->flags & 1U) == 0)
            return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsNpcRouteMoving(shen_name name) {
    const NpcRouteLiveEntry *entry;
    if ((entry = FindNpcRouteEntry(name)) == 0)
        return false;
    if (entry->liveState != 1)
        return false;
    return IsNpcRouteContextMoving(&entry->stateContext);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetNpcRouteAvoidanceSide(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry) {
        const NpcRouteStateContextPrefix *const context = &entry->stateContext;
        if ((context->flags & 3U) == 3U) {
            const std::int8_t result = static_cast<std::int8_t>(context->avoidanceResult);
            if (result != 3 && result <= 5)
                return 0;
            if (static_cast<std::int8_t>(context->retained88) == 1)
                return context->avoidanceAuxiliary;
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool CanNpcEnterScenario(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return false;
    if (!entry->node)
        return false;
    if ((entry->flags & 2U) == 0)
        return false;
    if (entry->flags & 0x10U)
        return true;
    if (static_cast<std::int8_t>(entry->liveState) == 8)
        return false;
    if (entry->flags & 0x40U)
        return false;
    const std::uint32_t restrictedFlags = entry->flags;
    if (restrictedFlags & 0x100U)
        return false;
    if (restrictedFlags & 0x80000U)
        return false;
    if ((entry->flags & 0x88U) == 8U)
        return false;
    if ((GetNpcAreaSpecialMotionFlags(*entry) & 0x80U) == 0)
        return true;
    return false;
}

#else

bool CanNpcEnterScenario(shen_name character) {
    const NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node || !(entry->flags & 2U))
        return false;
    const std::uint32_t flags = entry->flags;
    if ((flags & 0x4010U) != 0x10U) {
        if (entry->liveState == 7 || (flags & 0x140U))
            return false;
        if (flags & 8U) {
            if (!(flags & 0x80U))
                return false;
        } else if (flags & 0x80000U)
            return false;
        if (GetNpcAreaSpecialMotionFlags(*entry) & 0x80U)
            return false;
    }
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(character));
    return !party || !(party->flags & 2U) || party->behaviorState != 8;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool CheckNpcSpecialMotionTarget(shen_name name, Character *character) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !entry->node)
        return false;
    if (!(entry->flags & 2U))
        return false;
    if (!(entry->flags & 0x10U))
        return false;
    return IsNpcSpecialTargetWithinRange(character, entry);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t QueryNpcRouteLiveState(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !entry->node || !(entry->flags & 2U) || static_cast<std::int8_t>(entry->liveState) == 8 || (entry->flags & 0x80100U))
        return -1;
    if (entry->pendingRouteQueryState)
        return entry->pendingRouteQueryState;
    return entry->routeQueryState;
}

#else

std::int32_t QueryNpcRouteLiveState(shen_name character) {
    const NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node)
        return -1;
    const std::uint32_t flags = entry->flags;
    if (!(flags & 2U) || entry->liveState == 7 || (flags & 0x140U))
        return -1;
    if (flags & 8U) {
        if (!(flags & 0x80U))
            return -1;
    } else if (flags & 0x80000U)
        return -1;
    return entry->eyeControlMode ? static_cast<std::int32_t>(entry->eyeControlMode) : static_cast<std::int32_t>(entry->routeDirectionMode);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool QueryNpcSceneCheck(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    return entry && MoveForNpcInData(entry, shen_name(), 0);
}

#else

bool QueryNpcSceneCheck(shen_name character) {
    NpcRouteLiveEntry *const entry = character.name ? FindActiveNpcEntry(character) : nullptr;
    return entry && MoveForNpcInData(*entry, 0, false);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t AcquireNpcRuntimeEntryForScript(shen_name name, NpcRouteLiveEntry **result) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !entry->node)
        return 0;
    *result = entry;
    if (entry->flags & 0x10U)
        return -1;
    entry->flags |= 0x10U;
    entry->previousLiveState = entry->liveState;
    entry->animationFrame = static_cast<std::int16_t>(static_cast<std::int32_t>(mtwkGetFrame(entry->motionWork, LowerBody)));
    GetCharacterWorldPosition(entry->node, &entry->specialMotionRestorePosition);
    UpdateNpcSpecialRouteQueryState(entry);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool BeginNpcSpecialMotion(shen_name name) {
    NpcRouteLiveEntry *entry;
    const std::int32_t acquired = AcquireNpcRuntimeEntryForScript(name, &entry);
    if (!acquired)
        return false;
    if (acquired == -1)
        return true;
    PausePartyConversation(name);
    entry->specialMotionPhase = 0;
    switch (static_cast<std::int8_t>(entry->previousLiveState)) {
    case 4:
        mtwkSetSpeed(entry->motionWork, 0.0f, LowerBody);
        break;
    case 9:
        if (GetNpcAreaSpecialMotionFlags(*entry) & 2U)
            mtwkSetSpeed(entry->motionWork, 0.0f, LowerBody);
        entry->commandFlags |= 0x40U;
        break;
    case 1:
        break;
    default:
        mtwkSetSpeed(entry->motionWork, 1.0f, LowerBody);
        break;
    }
    entry->liveState = 2;
    entry->flags &= ~0x800000U;
    return true;
}

#else

bool BeginNpcSpecialMotion(shen_name character) {
    if (!npcRegistryTask.current)
        return false;
    NpcRouteLiveEntry *entry{};
    const std::int32_t acquired = AcquireNpcRuntimeEntryForScript(character, &entry);
    if (!acquired)
        return false;
    if (acquired == -1)
        return true;
    SetPartyConversationPaused(character, true);
    entry->specialMotionPhase = 0;
    if (entry->previousLiveState == 8) {
        if (GetNpcAreaSpecialMotionFlags(*entry) & 2U)
            mtwkSetSpeed(entry->motionWork, 0.0f, MotionRegion::LowerBody);
        entry->commandFlags |= 0x40U;
    } else if (entry->previousLiveState != 1) {
        mtwkSetSpeed(entry->motionWork, entry->previousLiveState == 4 ? 0.0f : 1.0f, MotionRegion::LowerBody);
    }
    entry->liveState = 2;
    entry->counter416 = 0;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BeginNpcFollowingMotion(shen_name name) {
    NpcRouteLiveEntry *entry;
    const std::int32_t acquired = AcquireNpcRuntimeEntryForScript(name, &entry);
    if (!acquired)
        return 0;
    if (acquired == -1)
        return 1;
    entry->specialMotionPhase = 0;
    entry->liveState = 13;
    entry->flags &= ~0x800000U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SetNpcSpecialRouteQueryPaused(shen_name name, std::int32_t enabled) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (enabled) {
        if (entry->flags & 0x200000U)
            return 1;
        if (entry->pendingRouteQueryState) {
            entry->retained312[0] = static_cast<std::byte>(-entry->pendingRouteQueryState);
            entry->pendingRouteQueryState = 13;
        } else {
            entry->retained312[0] = static_cast<std::byte>(entry->routeQueryState);
            entry->routeQueryState = 13;
        }
        entry->flags |= 0x200000U;
    } else {
        if (!(entry->flags & 0x200000U))
            return 1;
        if (static_cast<std::int8_t>(entry->retained312[0]) < 0)
            entry->pendingRouteQueryState = -static_cast<std::int8_t>(entry->retained312[0]);
        else
            entry->routeQueryState = static_cast<std::int8_t>(entry->retained312[0]);
        entry->flags &= ~0x200000U;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsNpcSpecialTargetWithinRange(Character *character, const NpcRouteLiveEntry *entry) {
    if (!character || static_cast<std::int8_t>(entry->stateContext.phase) != 7)
        return false;
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    position.x = entry->localPosition.x - position.x;
    position.z = entry->localPosition.z - position.z;
    if (CalculatePlanarMagnitude(position.x, position.z) > 2.0f)
        return false;
    Vector3i rotation;
    GetCharacterWorldRotation(character, &rotation);
    if (static_cast<std::int16_t>(rotation.y - entry->rotation.y) > 0x2000)
        return false;
    if (std::abs(position.x) < 0.001f && std::abs(position.z) < 0.001f)
        return false;
    const std::int32_t magnitude =
        static_cast<std::int32_t>(std::abs(static_cast<float>(static_cast<std::int16_t>(InverseFixedAtan2(position.x, position.z) - entry->rotation.y))));
    if (static_cast<std::uint32_t>(magnitude - 0x2000) > 0x3c71U)
        return false;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool EndNpcSpecialMotion(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    ResumePartyConversation(name);
    mtwkUpdateWithPosition(entry->motionWork, 0, 0);
    mtwkRequestTargetFacing(entry->motionWork, 0, 0);
    if (entry->flags & 8U) {
        entry->liveState = entry->previousLiveState;
        entry->specialMotionPhase = 0;
    } else {
        entry->flags |= 0x4000U;
    }
    return true;
}

#else

bool EndNpcSpecialMotion(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    SetPartyConversationPaused(character, false);
    mtwkUpdateWithPosition(entry->motionWork, nullptr, 0);
    mtwkRequestTargetFacing(entry->motionWork, nullptr, 0);
    if (entry->flags & 8U) {
        entry->flags &= ~0x4010U;
        entry->liveState = entry->previousLiveState;
        entry->specialMotionPhase = 0;
        if (entry->motionWork)
            mtwkSetTimeRelated(entry->motionWork, 0, 0, 0);
    } else {
        entry->specialMotionPhase = 10;
        entry->flags |= 0x4000U;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t StopNpcFollowingMotion(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (!(entry->flags & 0x10U))
        return 0;
    entry->liveState = 2;
    entry->specialMotionPhase = 0;
    mtwkCopyTransition(entry->motionWork, 0, entry->rotation.y, 13, 0);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool RequestNpcSpecialMotionTarget(shen_name name, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 3;
        entry->specialMotionTarget = *target;
    }
    return true;
}

bool ApplyNpcSpecialMotionTarget(shen_name name, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    mtwkUpdateWithPosition(entry->motionWork, target, 1);
    return true;
}

#else

bool RequestNpcSpecialMotionTarget(shen_name character, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 3;
        entry->specialMotionTarget = *target;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool ResetNpcSpecialMotionTarget(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 4;
        mtwkUpdateWithPosition(entry->motionWork, 0, 0);
    }
    return true;
}

#else

bool ResetNpcSpecialMotionTarget(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 5;
        mtwkUpdateWithPosition(entry->motionWork, nullptr, 0);
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool RequestNpcSpecialMotionLookTarget(shen_name name, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 5;
        mtwkUpdateWithPosition(entry->motionWork, 0, 2);
        entry->specialMotionTarget = *target;
    }
    return true;
}

#else

bool RequestNpcSpecialMotionLookTarget(shen_name character, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 6;
        mtwkUpdateWithPosition(entry->motionWork, nullptr, 2);
        entry->specialMotionTarget = *target;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool ResetNpcSpecialMotionLookTarget(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U))
        entry->specialMotionPhase = 6;
    return true;
}

#else

bool ResetNpcSpecialMotionLookTarget(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U))
        entry->specialMotionPhase = 7;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetNpcRunCapability(Character *character) {
    if (!character)
        return -1;
    switch (charGetMotionRigType(character)) {
    case 3:
    case 4:
    case 5:
        return 0;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool QueryNpcCameraMotionProfile(shen_name name, std::uint32_t flags) {
    static const std::uint16_t profilesA[] = {0x8066, 0x8065, 0x8064, 0x802a, 0x805e, 0x802d, 0x804b, 0x9504, 0};
    static const std::uint16_t profilesB[] = {0x805f, 0x8060, 0x8061, 0};
    static const std::uint16_t profilesC[] = {0x804b, 0xe393, 0xe14a, 0};
    bool result = false;
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return false;
    if (flags & 1U)
        for (std::int32_t index = 0; profilesA[index]; ++index)
            if (entry->currentMotionId == profilesA[index]) {
                result = true;
                break;
            }
    if (flags & 2U)
        for (std::int32_t index = 0; profilesB[index]; ++index)
            if (entry->currentMotionId == profilesB[index]) {
                result = true;
                break;
            }
    if (flags & 4U) {
        if (entry->routeMotionState != 23 && !GetNpcRunCapability(entry->node)) {
            if (GetNpcAreaSpecialMotionFlags(*entry) & 1U)
                result = true;
        } else {
            for (std::int32_t index = 0; profilesC[index]; ++index)
                if (entry->currentMotionId == profilesC[index]) {
                    result = true;
                    break;
                }
        }
    }
    return result;
}

#else

// @verified - p
std::uint64_t QueryNpcCameraMotionProfile(shen_name character, std::uint8_t mask) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry)
        return 0;
    std::uint32_t result = 0;
    if (mask & 1U) {
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_ONE_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == NPC_CAMERA_PROFILE_ONE_MOTIONS[index]) {
                result = 1;
                break;
            }
        }
    }
    if (mask & 2U) {
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_TWO_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == NPC_CAMERA_PROFILE_TWO_MOTIONS[index]) {
                result = 1;
                break;
            }
        }
    }
    if ((mask & 8U) && entry->node) {
        const std::int32_t boneId = GetCharacterBoneId(entry->node);
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_REMAPPED_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == RemapNpcMotionForRig(boneId, NPC_CAMERA_PROFILE_REMAPPED_MOTIONS[index])) {
                result = 1;
                break;
            }
        }
    }
    if (mask & 4U) {
        if (entry->routeMotionState == 23 || !entry->node || static_cast<std::uint32_t>(GetCharacterBoneId(entry->node)) - 3U > 2U) {
            for (std::size_t index = 0; NPC_CAMERA_PROFILE_FOUR_MOTIONS[index]; ++index) {
                if (entry->currentMotionId == NPC_CAMERA_PROFILE_FOUR_MOTIONS[index])
                    return 1;
            }
        } else if (GetNpcAreaSpecialMotionFlags(*entry) & 1U)
            return 1;
    }
    return result;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void NpcRegistryTask_Nullsub(void *) {
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native npc_Callback_Enqueue: always forgets the previous NPC task (the
// module switch released it) and queues NPCT; areas without NPC routes keep
// the task alive as a nullsub and drop the pending pointer.
bool EnqueueNpcRegistryTask(std::uint8_t nextFunctionIndex) {
    npcLoadedArea = g_currentMap;
    npcNewLoadState = 0;
    npcRegistryTask.current = nullptr;
    npcRegistryTask.pending = HLib::EnqueueTaskWithoutParameter(NpcRegistryTask_Callback, nextFunctionIndex, 2, static_cast<std::int32_t>(NPC_ROUTE_TASK.name));
    npcSetupHumans();
    if (!IsNpcRouteArea(g_currentMap)) {
        HLib::AssignCallbackForTask(npcRegistryTask.pending, NpcRegistryTask_Nullsub);
        npcRegistryTask.pending = nullptr;
        FreeNpcRouteLiveEntries();
        return true;
    }
    return npcRegistryTask.pending != nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool PublishNpcRegistryTask() {
    if (!npcRegistryTask.pending)
        return false;
    // NPC_Route_Control retains this producer pointer after publication;
    // FTBL729 remains admitted until NPCT teardown, including cached reloads.
    npcRegistryTask.current = npcRegistryTask.pending;
    npcRegistryTask.areas = {};
    npcRegistryTask.areaCount = 0;
    if (!LoadCurrentNpcSchedule(true))
        return false;
    PublishNpcRouteLiveEntries(npcRouteEntries);
    EV_NPC_NewLoadSw(1);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool npcIsNpcTaskActive() {
    return npcRegistryTask.current != nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetScenarioFollowEligibility(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || !(entry->scriptRecord[11] & 4U))
        return 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t IsNpcRouteActive(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    return entry ? entry->stateContext.flags & 1U : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return SHENMUE_NPC_NULL;
    if (!(entry->flags & 8U))
        npcPoolTarget =
            static_cast<std::int32_t>(static_cast<std::uint32_t>(npcPoolTarget) - static_cast<std::uint32_t>(static_cast<std::int32_t>(entry->streamCost)));
    entry->flags |= 0x88U;
    entry->controlFlags |= 2U;
    if (entry->node)
        SelectCharacterFacialNode(entry->node, 1);
    return entry;
}

#else

// @verified - p
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character) {
    if (!npcRegistryTask.current)
        return nullptr;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NRSF_AbstractActive)) {
            npcPoolTarget -= entry.streamCost;
            if ((entry.flags & (NRSF_Requested | NRSF_StartHidden)) == (NRSF_Requested | NRSF_StartHidden))
                ReleaseNpcRouteStream(entry, npcRouteRuntimeRecords[index], false);
        }
        entry.flags |= NRSF_AbstractActive | NRSF_Unknown80;
        entry.controlFlags |= 2U;
        if (entry.node) {

#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(entry.node)->runtime.callbackFlags &= ~8U;
            SetFaceFlagsForChara_0(entry.node, FacialNodeReplacement);
        }
        return &entry;
    }
    return nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_CheckWarmStartNpcRoute(shen_name name) {
    if (WarmStartNpcRoute(name))
        return 1;
    return 0;
}

#else

std::int64_t EV_CheckWarmStartNpcRoute(shen_name character) {
    return WarmStartNpcRoute(character) != nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Warm-starts the route and selects the HUMANS AFS load path.
std::int64_t npcWarmStartHumanRoute(shen_name character) {
    NpcRouteLiveEntry *const entry = WarmStartNpcRoute(character);
    if (!entry)
        return 0;
    entry->flags |= NRSF_UseHumansAfs;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t PrepareNpcRouteReload(shen_name name) {
    NpcRouteLiveEntry *const entry = WarmStartNpcRoute(name);
    if (!entry)
        return 0;
    entry->flags |= 0x40000000U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry *entry) {
    if (!entry->node)
        return;
    mtwkRequestRegionDetach(entry->motionWork, LowerBody);
    mtwkSetRequestedRootMode(entry->motionWork, 1);
    mtwkSetFrameTime(entry->motionWork, 20, LowerBody);
    switch (static_cast<std::int8_t>(entry->liveState)) {
    case 1: {
        NpcRouteStateContextPrefix *const context = &entry->stateContext;
        if (context->flags & 0x10U)
            entry->currentMotionId = SelectNpcRunningMotion(entry->node);
        else
            entry->currentMotionId = SelectNpcWalkingMotion(entry->node);
        break;
    }
    case 2:
        return;
    case 6:
        entry->currentMotionId = SelectNpcLeaningProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        break;
    case 4: {
        Character *const node = entry->node;
        AssignCharacterControlFlags(node, ccmGetFlags(node) & ~6U);
    }
    case 3:
    case 7:
        SetCharacterWorldPosition(entry->node, &entry->localPosition);
        break;
    case 9: {
        NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
        entry->currentMotionId = ResolveNpcAreaAnimation(state, entry);
        if (!entry->currentMotionId)
            entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
                static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        ApplyNpcAreaMotion(entry, entry->currentMotionId);
        break;
    }
    default:
        entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        break;
    }
    mtwkRequestMotion(entry->motionWork, entry->currentMotionId, LowerBody);
    charaSetWorldRot(entry->node, entry->rotation);
}

#else

void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    for (std::uint32_t region = 1; region < 5; ++region)
        entry.motionWork->runtime.regionFlags[region] |= 0x400U;
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    switch (entry.liveState) {
    case 1:
        entry.currentMotionId = SelectNpcLocomotion(entry.stateContext, (entry.stateContext.flags & 0x10U) ? 1 : 0);
        break;
    case 2:
        return;
    case 3:
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        if (entry.flags & 0x400U) {
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            return;
        }
        break;
    case 4:
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CCMF_ApplyGravity | CCMF_EvaluateMotion);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        break;
    case 6:
        entry.currentMotionId = SelectNpcSeatedIdleMotion(entry);
        break;
    case 8: {
        NpcAreaRouteState &state = *GetNpcAreaPointRouteState(entry.currentAreaPoint);
        if (entry.specialAnimationContext)
            entry.currentMotionId = npcResolveAnimationForRoute(state, entry);
        if (!entry.specialAnimationContext)
            entry.currentMotionId = npcResolveAnimationForRoute(state, entry);
        if (!entry.currentMotionId)
            entry.currentMotionId = SelectNpcIdleMotion(entry);
        ApplyNpcAreaMotion(entry, entry.currentMotionId);
        if (((entry.routeMotionState - 16) & 0xfb) == 0) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags |= CCMF_StairMode;
            entry.commandFlags |= 0x800U;
        }
        break;
    }
    default:
        entry.currentMotionId = SelectNpcIdleMotion(entry);
        break;
    }
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    charaSetWorldRot(entry.node, rotation);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t npcSetLock(shen_name character, std::int32_t preserveMotion) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (ReadU32(entry.recordBase) == SHEN_NAME(MOVE))
            return 0;
        if (entry.flags & NRSF_AbstractActive) {
            npcPoolTarget += entry.streamCost;
            entry.flags &= ~(NRSF_AbstractActive | NRSF_UseHumansAfs);
            entry.controlFlags &= ~2U;
            if (!preserveMotion)
                ResumeNpcMotionAfterUnlock(entry);
            if (entry.node)
                SetFaceFlagsForChara_0(entry.node, FacialNodeOriginal);
        }
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_npcLockUnlock(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || *reinterpret_cast<const std::uint32_t *>(entry->scriptRecord) == SHEN_NAME4('M', 'O', 'V', 'E'))
        return 0;
    if (!(entry->flags & 8U))
        return 1;
    npcPoolTarget += entry->streamCost;
    entry->flags &= 0xbffffff7U;
    entry->controlFlags &= ~2U;
    ResumeNpcMotionAfterUnlock(entry);
    if (entry->node)
        SelectCharacterFacialNode(entry->node, 0);
    return 1;
}

#else

std::int64_t EV_npcLockUnlock(shen_name character) {
    return npcSetLock(character, 0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t EV_ReleaseNpcLockPreservingMotion(shen_name character) {
    return npcSetLock(character, 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t IsNpcRouteLoadedOrVisible(shen_name name) SHENMUE_NPC_NOEXCEPT {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (entry->flags & 8U)
        return 1;
    return 0;
}

#else

// @verified - p
std::uint64_t IsNpcRouteLoadedOrVisible(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags >> 3) & 1U;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t IsNpcRouteReloadPrepared(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (entry->flags & 0x40000000U)
        return 1;
    return 0;
}

std::int32_t PinNpcRouteStream(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (!(entry->flags & 0x200U)) {
        entry->flags |= 0x200U;
        EV_ResetFreeTalkFlags();
        npcCrowdPhase = 0;
    }
    return 1;
}

std::int32_t UnpinNpcRouteStream(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    entry->flags &= ~0x200U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_npcUpdateFlags(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry && (entry->flags & 8U)) {
        entry->flags |= 0x80U;
        return 1;
    }
    return 0;
}

#else

// @verified - p
std::int64_t EV_npcUpdateFlags(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & 8U))
            return 0;
        entry.flags |= 0x80U;
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_npcUnknownFunc(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry && (entry->flags & 8U)) {
        entry->flags &= ~0x80U;
        return 1;
    }
    return 0;
}

#else

// @verified - p
std::int64_t EV_npcUnknownFunc(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & 8U))
            return 0;
        entry.flags &= ~0x80U;
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetNpcScriptRouteCompletion(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (!(entry->flags & 8U))
        return 0;
    if (!(entry->flags & 0x80U))
        return 0;
    if (!(GetNpcAreaSpecialMotionFlags(*entry) & 0x80U))
        return 1;
    return 0;
}

#else

// @verified - p
std::uint32_t GetNpcScriptRouteCompletion(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if ((entry.flags & 0x88U) != 0x88U)
            return 0;
        return (GetNpcAreaSpecialMotionFlags(entry) ^ 0x80U) >> 7;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_ResetFreeTalkFlags() {
    if (IsNpcRegistryTaskActive()) {
        ReleaseNpcTransitionHumanPackages();
        NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
        for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
            if (!(entry->flags & 8U)) {
                if (!(entry->flags & 0x200U) && (entry->flags & 7U))
                    ReleaseNpcRouteStream(entry);
            }
    }
    return 1;
}

#else

std::int64_t EV_ResetFreeTalkFlags() {
    if (!npcRegistryTask.current)
        return 1;
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (entry.loadHumanResult < 0) {
            entry.loadHumanResult = 8;
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        if (!(entry.flags & (NRSF_AbstractActive | NRSF_Pinned)) && (entry.flags & 7U)) {
            FlushQueuedLegacyDrawPasses();
            ReleaseNpcRouteStream(entry, runtime, false);
        }
    }
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t ResetNpcColdStreams() {
    if (!npcRegistryTask.current)
        return 1;
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.stateContext.flags & 1U) {
            if ((entry.stateContext.flags & 0x300U) || (entry.stateContext.routeFlags & 0x10U))
                continue;
            if (!(entry.flags & 0x4000000U)) {
                SetNpcStreamLifecycleState(entry, 1);
                continue;
            }
        }
        if (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x10U)
            SetNpcStreamLifecycleState(entry, 1);
    }
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint64_t SetNpcColdResetMode(std::uint32_t mode) {
    if (!mode)
        ResetNpcColdStreams();
    npcColdResetMode = mode;
    return mode;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t PrepareNpcAllFromArray() {
    std::int32_t released = 1;
    if (IsNpcRegistryTaskActive()) {
        EV_NPC_NewLoadSw(0);
        ReleaseNpcTransitionHumanPackages();
        NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
        for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry) {
            const std::uint32_t flags = entry->flags;
            if (flags & 8U)
                continue;
            if ((flags & 0x200U) || !(flags & 7U))
                continue;
            if (flags & 1U)
                ReleaseNpcRouteStream(entry);
            else
                released = 0;
            if (entry->flags & 2U)
                BeginNpcRouteStreamOut(entry, 0);
            if (entry->flags & 4U)
                UpdateNpcStreamFade(entry);
        }
    }
    return released;
}

#else

// Returns 1 once no unpinned, non-abstract stream is still live or fading out.
std::int32_t PrepareNpcAllFromArray() {
    std::int32_t released = 1;
    if (!npcRegistryTask.current)
        return released;
    if (GetNpcNewLoadState() != 0)
        EV_NPC_NewLoadSw(0);
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (ReadNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset) < 0) {
            WriteNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset, 8);
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        std::uint32_t pending = 4;
        if ((entry.flags & NRSF_AbstractActive) != 0) {
            pending = 2;
        } else if ((entry.flags & NRSF_Pinned) != 0) {
            pending = 3;
        } else if ((entry.flags & (NRSF_Requested | NRSF_Live | NRSF_FadeOut)) != 0) {
            FlushQueuedLegacyDrawPasses();
            pending = entry.flags & NRSF_Requested;
            if (pending != 0)
                ReleaseNpcRouteStream(entry, runtime, false);
            if ((entry.flags & (NRSF_Live | NRSF_AbstractActive | 0x10U)) == NRSF_Live) {
                entry.field335 = 0;
                entry.flags = (entry.flags & ~(NRSF_Live | NRSF_FadeOut)) | NRSF_FadeOut;
            }
            if ((entry.flags & NRSF_FadeOut) != 0)
                UpdateNpcStreamFade(entry);
        }
        if (pending == 0)
            released = 0;
    }
    return released;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void EV_ReconfigureNpcRoutesForStoryStep() {
    if (!npcRegistryTask.current)
        return;
    EV_ResetFreeTalkFlags();
    WarmBootstrapNpcRoutes();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ReleaseLoadedNpcTransitionPackages();

// @verified - d
std::int32_t npcReleaseUnusedStreams() {
    GetNpcNewLoadState();
    ReleaseLoadedNpcTransitionPackages();
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
        if (!(entry->flags & 8U)) {
            if (!(entry->flags & 0x200U) && (entry->flags & 1U))
                ReleaseNpcRouteStream(entry);
        }
    return 1;
}

#else

// Releases every unpinned, non-abstract stream that is still only requested.
std::int32_t npcReleaseUnusedStreams() {
    if (!npcRegistryTask.current)
        return 1;
    ReleaseReadyNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (ReadNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset) < 0) {
            WriteNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset, 8);
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        if ((entry.flags & (NRSF_AbstractActive | NRSF_Pinned)) == 0 && (entry.flags & (NRSF_Requested | NRSF_Live | NRSF_FadeOut)) != 0) {
            FlushQueuedLegacyDrawPasses();
            if (entry.flags & NRSF_Requested)
                ReleaseNpcRouteStream(entry, runtime, false);
        }
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_ForceLoadNpc(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry) {
        entry->flags &= ~0x1000U;
        return RequestNpcStreamLoad(entry);
    }
    if (npcLoadHuman(name, name, 0) > 0)
        return 1;
    return 0;
}

#else

std::int64_t EV_ForceLoadNpc(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NRSF_StartHidden;
            return RequestNpcStreamLoad(entry);
        }
    }
    return npcLoadHuman(character, character, false) > 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ReleaseNpcForRouteReload(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry) {
        entry->flags &= ~0x1000U;
        return RequestNpcStreamLoad(entry);
    }
    if (npcLoadHuman(name, name, 0x40000000U) > 0)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_npcStreamOrLoadHuman(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry) {
        entry->flags &= ~0x1000U;
        RequestNpcStreamLoad(entry);
    } else
        npcLastHumanLoadResult = npcLoadHuman(name, name, 0);
    return npcLastHumanLoadResult;
}

#else

std::uint64_t EV_npcStreamOrLoadHuman(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NRSF_StartHidden;
            RequestNpcStreamLoad(entry);
            return static_cast<std::uint32_t>(npcLastHumanLoadResult);
        }
    }
    npcLastHumanLoadResult = npcLoadHuman(character, character, false);
    return static_cast<std::uint32_t>(npcLastHumanLoadResult);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ReleaseNpcForRouteReloadAndStoreCount(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry) {
        entry->flags &= ~0x1000U;
        RequestNpcStreamLoad(entry);
    } else
        npcLastHumanLoadResult = npcLoadHuman(name, name, 0x40000000U);
    return npcLastHumanLoadResult;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t EV_npcFree(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return npcReleasePak(name);
    if (entry->flags & 7U)
        ReleaseNpcRouteStream(entry);
    return 1;
}

#else

std::int64_t EV_npcFree(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            if (entry.flags & (NRSF_Requested | NRSF_Live | NRSF_FadeOut)) {
                ReleaseNpcRouteStream(entry, npcRouteRuntimeRecords[index], false);
            }
            return 1;
        }
    }
    return npcReleasePak(character);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void EV_SetAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask, bool visible, bool lowQualityHands) {
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadCharacters = characters;
    areaNpcLoadStartupTask = startupTask;
    areaNpcLoadCharactersVisible = visible;
    areaNpcLoadLowQualityHands = lowQualityHands;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetAreaNpcLowQualityHands(bool enabled) {
    areaNpcLoadLowQualityHands = enabled;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetAreaNpcCharactersVisible(bool enabled) {
    areaNpcLoadCharactersVisible = enabled;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void AssignAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask) {
    areaNpcLoadPhase = 0;
    areaNpcLoadCharacters = characters;
    areaNpcLoadStartupTask = startupTask;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace npc_package_detail {

std::int32_t FindConditionalNpcLoadSlot(shen_name name) {
    for (std::size_t index = 0; index < conditionalNpcLoadSlots.size(); ++index)
        if (conditionalNpcLoadSlots[index].character.name == name.name)
            return static_cast<std::int32_t>(index);
    return -1;
}
void ConditionalNpcLoadCheckCallback(void *data) {
    auto &work = *static_cast<ConditionalNpcLoadWork *>(data);
    for (std::int32_t index = 0; work.entries[index].character.name != 0xffffffffU; ++index) {
        const auto &entry = work.entries[index];
        const std::int32_t slot = FindConditionalNpcLoadSlot(entry.character);
        if (slot == -1 && !work.admitted[index] && (entry.storyFlag <= 0 || EV_GetShenmueFlag(entry.storyFlag)) && (!entry.condition || entry.condition())) {
            work.phase = 0;
            work.entryIndex = 0;
            EV_NPC_NewLoadSw(0);
            for (std::int32_t other = 0; work.entries[other].character.name != 0xffffffffU; ++other) {
                const shen_name name = work.entries[other].character;
                if (IsNpcLoadedOrVisible(name)) {
                    EV_npcLockUnlock(name);
                    EV_npcFree(name);
                    work.admitted[other] = 0;
                }
            }
            conditionalNpcLoadSlots.fill({});
            HLib::SetCurrentTaskTo(ConditionalNpcLoadCallback);
            return;
        }
        if (entry.storyFlag != -1) {
            const auto retire = [&] {
                const std::int32_t owned = FindConditionalNpcLoadSlot(entry.character);
                if (owned == -1)
                    return;
                if (conditionalNpcLoadSlots[owned].entryIndex == index) {
                    conditionalNpcLoadSlots[owned].character = {};
                    EV_npcLockUnlock(entry.character);
                    EV_npcFree(entry.character);
                }
                work.admitted[index] = 0;
            };
            if (!EV_GetShenmueFlag(entry.storyFlag))
                retire();
            if (entry.condition && !entry.condition())
                retire();
        }
    }
    conditionalNpcLoadsReady = 1;
}
void ConditionalNpcLoadCallback(void *data) {
    auto &work = *static_cast<ConditionalNpcLoadWork *>(data);
    const auto *entry = work.entries + work.entryIndex;
    if (!work.phase) {
        if (PrepareNpcAllFromArray())
            ++work.phase;
        return;
    }
    if (work.phase == 1) {
        for (; entry->character.name != 0xffffffffU; ++entry, ++work.entryIndex) {
            if (entry->storyFlag > 0 && !EV_GetShenmueFlag(entry->storyFlag))
                continue;
            if (!DoesScenarioCharacterExist(entry->character) || FindConditionalNpcLoadSlot(entry->character) == -1) {
                if (!entry->condition || entry->condition()) {
                    ++work.phase;
                    EV_CheckWarmStartNpcRoute(entry->character);
                    return;
                }
                work.admitted[work.entryIndex] = 0;
            }
        }
        EV_NPC_NewLoadSw(1);
        HLib::SetCurrentTaskTo(ConditionalNpcLoadCheckCallback);
        return;
    }
    if (work.phase == 2 && EV_ForceLoadNpc(entry->character)) {
        std::size_t slot = 0;
        while (conditionalNpcLoadSlots[slot].character.name)
            ++slot;
        conditionalNpcLoadSlots[slot] = {entry->character, work.entryIndex};
        EV_EnableCharacterDisplay(entry->character);
        EV_AT_SetPos(entry->character, EventTransformFlags::World | EventTransformFlags::AllAxes, entry->position);
        EV_AT_SetAngY(entry->character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, entry->yaw, 0});
        if (entry->motion != 0xffffffffU)
            mtwkRequestMotion(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(ctrlGetScenarioChara(entry->character)), static_cast<std::uint16_t>(entry->motion), MotionRegion::LowerBody);
        work.admitted[work.entryIndex++] = 1;
        work.phase = 1;
    }
}
} // namespace npc_package_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetConditionalNpcLoads() {
    conditionalNpcLoadsReady = 0;
    conditionalNpcLoadSlots.fill({});
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t AreConditionalNpcLoadsPending() {
    return conditionalNpcLoadsReady == 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *QueueConditionalNpcLoads(const ConditionalNpcLoadEntry *entries) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ConditionalNpcLoadCheckCallback, 4, 0x0b, sizeof(ConditionalNpcLoadWork), SHEN_NAME(EVNT));
    auto *const work = static_cast<ConditionalNpcLoadWork *>(HLib::GetTaskParameter(task));
    if (work) {
        *work = {};
        work->entries = entries;
        ResetConditionalNpcLoads();
        HLib::SetupTimedTask(task, 5);
    } else
        HLib::DestroyTaskInternal(task);
    return task;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void AssignGenericNpcLoadContext(const shen_name *characters, HLib::Task *startupTask) {
    genericNpcLoadPhase = 0;
    genericNpcLoadCharacters = characters;
    genericNpcLoadStartupTask = startupTask;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetGenericNpcLowQualityHands(bool enabled) {
    genericNpcLoadLowQualityHands = enabled;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetGenericEventNpcLoadState() {
    genericNpcLoadCharacters = nullptr;
    genericNpcLoadPhase = 0;
    genericNpcLoadLowQualityHands = 1;
    genericNpcLoadCharactersVisible = 1;
    genericNpcLoadStartupTask = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *Queue_EVT_UTL_LoadNPCChara(const shen_name *characters, HLib::Task *startupTask) {
    AssignGenericNpcLoadContext(characters, startupTask);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startupTask)
        HLib::SetupTimedTask(startupTask, 0);
    return HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(TASK_NPC_LOAD.name));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetCompiledEventNpcLoadState() {
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadLowQualityHands = 1;
    areaNpcLoadCharactersVisible = 1;
    areaNpcLoadStartupTask = nullptr;
    areaNpcLoadCharacters = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLib::Task *startupTask) {
    areaNpcLoadCharacters = characters;
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadStartupTask = startupTask;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startupTask)
        HLib::SetupTimedTask(startupTask, 0);
    return HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(TASK_NPC_LOAD.name));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void EVT_UTL_LoadNPCChara_Callback_1(void *) {
    const shen_name character = *genericNpcLoadCharacters;
    if (!genericNpcLoadPhase) {
        EV_CheckWarmStartNpcRoute(character);
        (void)EV_ForceLoadNpc(character);
        genericNpcLoadPhase = 1;
        return;
    }
    if (!EV_ForceLoadNpc(character))
        return;
    if (genericNpcLoadCharactersVisible) {
        EV_EnableCharacterDisplay(character);
        EV_EnsureCharacterDraw(character);
    } else {
        EV_DisableCharacterDisplay(character);
        EV_ToggleCharacterFlag(character);
    }
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    if (genericNpcLoadLowQualityHands)
        SetLowQualityHands(ctrlGetScenarioChara(character));
    ++genericNpcLoadCharacters;
    genericNpcLoadPhase = 0;
    if (genericNpcLoadCharacters->name == 0xffffffffU) {
        if (genericNpcLoadStartupTask)
            HLib::ResolveDeferredCallback(genericNpcLoadStartupTask);
        HLib::DestroyCurrentTask();
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void EnableAuthoredCharacterFlagsAndDisplay(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_EnsureCharacterDraw(character);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void EVT_UTL_LoadNPCChara_Callback_2(void *taskData) {
    (void)taskData;
    const shen_name character = *areaNpcLoadCharacters;

    if (areaNpcLoadPhase != 0) {
        if (!EV_ForceLoadNpc(character))
            return;

        if (areaNpcLoadCharactersVisible != 0) {
            EnableAuthoredCharacterFlagsAndDisplay(character);
        } else {
            EV_DisableCharacterDisplay(character);
            EV_ToggleCharacterFlag(character);
            EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetRefBoardParamsFor(character, 1);
            EV_SetCharacterAlphaEnabled(character, false);
        }

        if (areaNpcLoadLowQualityHands != 0) {
            SetLowQualityHands(ctrlGetScenarioChara(character));
        }

        ++areaNpcLoadCharacters;
        areaNpcLoadPhase = 0;
        if (areaNpcLoadCharacters->name == 0xffffffffU) {
            g_usingCustomAuthorCharacterList = 1;
            if (areaNpcLoadStartupTask)
                HLib::ResolveDeferredCallback(areaNpcLoadStartupTask);
            HLib::DestroyCurrentTask();
        }
        return;
    }

    EV_CheckWarmStartNpcRoute(character);
    EV_ForceLoadNpc(character);
    areaNpcLoadPhase = 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Before a door entry, sends every streamed NPC within the radius of the point (XZ) back to
// its abstract route; 0 when one refused.
std::uint32_t npcReleaseStreamsNearDoor(const Vector3 *point, float radius) {
    std::uint32_t released = 1;
    if (!npcRegistryTask.current)
        return released;
    if (npcNewLoadState)
        EV_NPC_NewLoadSw(0);
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    std::int32_t cost = npcActiveStreamCost;
    const float radiusSquared = radius * radius;
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry && cost; entry = entry->next) {
        if (!(entry->flags & 7U))
            continue;
        cost -= entry->streamCost;
        const float dx = entry->localPosition.x - point->x;
        const float dz = entry->localPosition.z - point->z;
        if (dx * dx + dz * dz <= radiusSquared && SetNpcStreamLifecycleState(*entry, 1) == 0)
            released = 0;
    }
    return released;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcWait() SHENMUE_NPC_NOEXCEPT {
    if (!IsNpcRegistryTaskActive())
        return 0;
    if (!npcReloadPassStarted)
        return 1;
    const NpcRouteLiveEntry *entry = reinterpret_cast<const NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
        if (!(entry->flags & 8U) && (entry->flags & 1U))
            return 1;
    return 0;
}

#else

// @verified - p
bool NpcWait() {
    if (!npcRegistryTask.current)
        return false;
    if (!npcReloadPassStarted || npcWaitBypassActive)
        return true;
    if (npcWaitSawLoadFailure || static_cast<std::int32_t>(++npcWaitFrames) > 150)
        return false;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (!(entry.flags & NRSF_AbstractActive) && (entry.flags & NRSF_Requested))
            return true;
    }
    return false;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void FreeNpcRegistryTask() {
    FreeNpcRouteLiveEntries();
    areaNpcLoadCharacters = nullptr;
    areaNpcLoadStartupTask = nullptr;
    areaNpcLoadPhase = 0;
    HLib::Task *const pending = npcRegistryTask.pending;
    HLib::Task *const current = npcRegistryTask.current;
    npcRegistryTask = {};
    if (pending)
        HLib::DestroyTask(pending);
    if (current && current != pending)
        HLib::DestroyTask(current);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t npcSetAreaName(shen_name area, const Vector3 *center, float radius, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;

    if (enabled == 1) {
        for (NpcNoSpawnArea &slot : npcRegistryTask.areas) {
            if (slot.name.name != 0)
                continue;
            ++npcRegistryTask.areaCount;
            slot = NpcNoSpawnArea{area, *center, radius};
            return 1;
        }
        return 0;
    }

    for (NpcNoSpawnArea &slot : npcRegistryTask.areas) {
        if (slot.name.name != area.name)
            continue;
        --npcRegistryTask.areaCount;
        slot.name = {};
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CountNpcLiveStreams() SHENMUE_NPC_NOEXCEPT {
    if (!IsNpcRegistryTaskActive() || !npcReloadPassStarted)
        return 0;
    std::int32_t count = 0;
    const NpcRouteLiveEntry *entry = reinterpret_cast<const NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
        if (entry->flags & 2U)
            ++count;
    return count;
}

#else

std::uint64_t CountNpcLiveStreams() {
    if (!npcRegistryTask.current || !npcReloadPassStarted)
        return 0;
    std::uint32_t count = 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index)
        if (npcRouteEntries[index].flags & NRSF_Live)
            ++count;
    return count;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t IsNpcLoadedOrVisible(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags & NRSF_AbstractActive) != 0;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t IsNpcUsingHumansAfs(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags & NRSF_UseHumansAfs) != 0;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t EV_ForceLoadNpcFromHumans(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NRSF_StartHidden;
            return RequestNpcStreamLoad(entry);
        }
    }
    return npcLoadHuman(character, character, true) > 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t EV_npcStreamOrLoadHumanFromHumans(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NRSF_StartHidden;
            RequestNpcStreamLoad(entry);
            return static_cast<std::uint32_t>(npcLastHumanLoadResult);
        }
    }
    npcLastHumanLoadResult = npcLoadHuman(character, character, true);
    return static_cast<std::uint32_t>(npcLastHumanLoadResult);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SaveNpcDateTime() {
    shenGetCurrentDateTime(npcSavedDateTime);
    npcSetCurrentTimeSimStateEnabled();
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT {
    if (!IsNpcRegistryTaskActive())
        return SHENMUE_NPC_NULL;
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcRouteEntryCount; ++index, ++entry)
        if (name == entry->name)
            return entry;
    return SHENMUE_NPC_NULL;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PlaceNpcOutsideDoor(NpcRouteLiveEntry *entry) {
    entry->flags &= ~0x800U;
    if (GetNpcAreaPointRouteState(entry->currentAreaPoint))
        return;
    const std::int32_t door = static_cast<std::int32_t>(GetNpcAreaPointOwnerName(entry->currentAreaPoint));
    if (!door)
        return;
    Vector3 target;
    if (!ComputeDoorIngress(door, &entry->localPosition, &target))
        return;
    ComputeDoorEgress(door, &entry->localPosition, &target);
    entry->localPosition = target;
    entry->worldPosition = target;
    entry->worldPosition.x += npcRouteOrigin.x;
    entry->worldPosition.z += npcRouteOrigin.z;
    entry->flags |= 0x800U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void CoreNPCTask() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (control->ryo) {
        npcRouteTableReady = 1;
        npcPlayerSearchRequested = IsPlayerControlEnabled();
        Vector3 delta = npcPlayerPosition;
        plymUpdatePlayerSearchLocation(&npcPlayerPosition);
        am2Vector3Subtract(&delta, &npcPlayerPosition, &delta);
        npcPlayerDisplacement = Vector3::CalcMagnitude(delta);
        if (std::fabs(npcPlayerDisplacement) < FLT_EPSILON)
            npcPlayerDisplacement = 0.0099999998f;
        npcPlayerRadius = mtwkGetClusterPosition(charFindMotionwork(ctrlGetRootNode()), &npcPlayerCenter);
        npcNearbyDoor = GetNearbyDoorForNpc(&npcPlayerPosition);
        npcScheduleFloor = GetQronNextStream();
        const float displacement = npcPlayerDisplacement < 0.039999999f ? 0.0f : npcPlayerDisplacement > 0.17f ? 0.17f : npcPlayerDisplacement;
        npcStreamThrottle += displacement;
        if (npcStreamThrottle > displacement * 100.0f)
            npcStreamThrottle = displacement * 100.0f;
        npcStreamThrottleWithBias = npcStreamThrottle + 4.5f;
    } else {
        npcRouteTableReady = 0;
        npcNearbyDoor = 0;
        npcStreamThrottleWithBias = 4.5f;
    }
    ++npcFrameCount;
    npcRouteBuildCount = 0;
    npcCurrentRain = GetWeatherPressure();
    npcHalfHourWeatherForecast = GetMaximumWeatherRainForecast(30);
    if (!(npcCurrentRain < 0.15f))
        npcCloudyState = 1;
    else
        npcCloudyState = 0;
    shenGetCurrentDateTime(npcCurrentDateTime);
    const std::int32_t seconds = ComputeSecondsSince1980(npcCurrentDateTime);
    npcCurrentSeconds = seconds;
    TickNpcRouteSlices(2 * (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60 + npcCurrentDateTime.second));
    NpcRouteLiveEntry *entry = npcCrowdHead;
    switch (npcCrowdPhase) {
    case 5: {
        std::int32_t skipped = 0, cost = 0, candidate;
        for (; cost < npcPoolTarget && entry; entry = entry->next) {
            if (entry->flags & 0x100000U)
                continue;
            candidate = MeasureNpcStreamCost(entry);
            if (!candidate)
                ++skipped;
            if (skipped == 20)
                break;
            cost += candidate;
        }
        npcCrowdBookmark = entry;
        break;
    }
    case 6: {
        if (npcCrowdBookmark)
            for (entry = npcCrowdBookmark; entry; entry = entry->next)
                if (!(entry->flags & 0x200U))
                    BeginNpcRouteStreamOut(entry, 0);
        entry = npcCrowdHead;
        NpcRouteLiveEntry *retire = 0, *replacement = 0;
        for (; entry && entry != npcCrowdBookmark; entry = entry->next) {
            if (entry->flags & 0x200U)
                continue;
            if (!replacement) {
                if (entry->streamSortKey > 20.0f)
                    break;
                if (entry->flags & 0x20000000U)
                    replacement = entry;
                else if (!(entry->flags & 2U) && entry->loadHumanResult < 0)
                    replacement = entry;
            } else if (entry->flags & 2U)
                retire = entry;
        }
        if (retire && replacement) {
            if (replacement->flags & 0x20000000U)
                replacement->flags &= ~0x20000000U;
            else
                BeginNpcRouteStreamOut(retire, 1);
        }
        break;
    }
    case 7:
        if (GetNpcNewLoadState()) {
            npcReloadPassStarted = 1;
            std::int32_t skipped = 0, cost = 0, candidate;
            for (; cost < npcPoolTarget && entry; entry = entry->next) {
                if (entry->flags & 0x100000U)
                    continue;
                candidate = MeasureNpcReloadStreamCost(entry);
                if (!candidate)
                    ++skipped;
                if (skipped == 20)
                    break;
                cost += candidate;
            }
        }
        break;
    }
    npcCrowdPhase = ++npcCrowdPhase % 8;
    std::int32_t renderedCount = 0;
    for (entry = npcCrowdHead; entry; entry = entry->next) {
        if (entry->node)
            mtwkSetUnknownField(entry->motionWork, renderedCount++ <= 4);
        if (entry->flags & 4U)
            UpdateNpcStreamFade(entry);
    }
    for (entry = npcCrowdHead; entry; entry = entry->next) {
        if (!entry->node)
            continue;
        switch (static_cast<std::int8_t>(entry->liveState)) {
        case 8:
        case 11:
        case 12:
            break;
        default:
            GetCharacterWorldPosition(entry->node, &entry->localPosition);
            SetNpcRouteScenePosition(&entry->stateContext, shen_name(), &entry->localPosition);
        }
        entry->streamRadius = mtwkGetClusterPosition(entry->motionWork, &entry->streamCenter);
        if (entry->streamRadius < 0.2f)
            entry->streamRadius = 0.2f;
    }
    for (entry = npcCrowdHead; entry;) {
        if (entry->flags & 1U) {
            RequestNpcStreamLoad(entry);
            if (entry->flags & 1U)
                entry = entry->next;
            else
                entry = npcCrowdHead;
        } else
            entry = entry->next;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ReleaseNpcScriptRouteSlot(shen_name name) {
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage) + npcAuthoredEntryCount;
    for (std::int32_t index = 0; index < 6; ++index, ++entry) {
        if (entry->name == name) {
            entry->name = SHEN_NAME4('M', 'O', 'V', 'E');
            entry->node = 0;
            entry->motionWork = 0;
            return 1;
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CreateScriptNpcRouteSlot(shen_name name) {
    ReleaseNpcScriptRouteSlot(name);
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage) + npcAuthoredEntryCount;
    for (std::int32_t index = 0; index < 6; ++index, ++entry) {
        if (entry->name != SHEN_NAME4('M', 'O', 'V', 'E'))
            continue;
        entry->name = name;
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetNpcEntryToAreaPoint(NpcRouteLiveEntry *entry) {
    entry->localPosition = *GetNpcAreaPointPosition(entry->currentAreaPoint);
    entry->localPosition.x -= npcRouteOrigin.x;
    entry->localPosition.z -= npcRouteOrigin.z;
    entry->packedMoveFlags = (entry->packedMoveFlags & 0xff00U) | GetNpcAreaPointMoveFlags(entry->currentAreaPoint);
    if (entry->flags & 0xa000U)
        entry->localPosition.y = entry->forcedY;
    entry->worldPosition = entry->localPosition;
    entry->worldPosition.x += npcRouteOrigin.x;
    entry->worldPosition.z += npcRouteOrigin.z;
    entry->previousPosition = entry->localPosition;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void BeginNpcAreaRoute(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    ReplaceNpcStateCallback(entry, NpcAreaCompletionCallback);
    InitializeNpcAreaRoute(entry);
    entry->scheduleProgress = 3;
    entry->liveState = 9;
    if (GetNpcAreaPointRouteState(entry->currentAreaPoint)->flags & 8U)
        npcSlotFlags[entry->poolIndex] |= 4U;
    else
        npcSlotFlags[entry->poolIndex] &= ~4U;
    while (InterpretNpcAreaCommand(entry, currentTime)) {
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry *entry) {
    if (entry->flags & 7U)
        return entry->streamCost;
    if (entry->flags & 0x28U)
        return 0;
    Vector3 position = entry->localPosition;
    if (!FindNpcIntersectionInScene(position, entry->packedMoveFlags)) {
        if (npcLoadedArea == shen_name(SHEN_NAME4('T', 'E', 'S', 'T')))
            entry->localPosition.y = 0.0f;
        else if (entry->flags & 0x2000U)
            entry->localPosition.y = entry->forcedY;
        else if (!entry->attachedCharacter)
            return 0;
    }
    return entry->streamCost;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t MeasureNpcReloadStreamCost(NpcRouteLiveEntry *entry) {
    if (entry->flags & 7U)
        return entry->streamCost;
    if (entry->flags & 0x20000028U)
        return 0;
    Vector3 position = entry->localPosition;
    if (!FindNpcIntersectionInScene(position, entry->packedMoveFlags)) {
        if (npcLoadedArea == shen_name(SHEN_NAME4('T', 'E', 'S', 'T')))
            entry->localPosition.y = 0.0f;
        else if (entry->flags & 0x2000U)
            entry->localPosition.y = entry->forcedY;
        else if (!entry->attachedCharacter)
            return 0;
    }
    if (((&entry->stateContext)->flags & 1U) && IsInsideNpcNoSpawnRadius(&entry->localPosition, 0.40000001f))
        return 0;
    entry->flags |= 0x1000U;
    RequestNpcStreamLoad(entry);
    return entry->streamCost;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t RequestNpcStreamLoad(NpcRouteLiveEntry *entry) {
    npcLastHumanLoadResult = 0;
    switch (static_cast<std::int8_t>(entry->streamRouteChangeKind)) {
    case 0:
        if (entry->flags & 2U) {
            npcLastHumanLoadResult = 1;
            return 1;
        }
        if (entry->flags & 4U) {
            entry->flags = (entry->flags | 1U) & ~4U;
            entry->streamRouteChangeKind = 2;
            return 0;
        }
        if (!GetNpcNewLoadState() && (entry->flags & 0x1000U)) {
            entry->flags &= ~1U;
            return 1;
        }
        if (npcPoolTarget <= npcActiveStreamCost && !(entry->flags & 8U))
            return 1;
        entry->flags |= 1U;
        npcActiveStreamCost += entry->streamCost;
        entry->streamRetryTicks = 0;
        entry->streamRouteChangeKind = 1;
    case 1: {
        const std::int32_t result = npcLoadHuman(entry->loadHumanName, entry->name, entry->flags & 0x40000000U);
        entry->loadHumanResult = result;
        if (!result)
            break;
        npcLastHumanLoadResult = result;
        entry->streamRouteChangeKind = 0;
        entry->flags &= ~1U;
        if (result > 0) {
            BindNpcRouteCharacter(entry);
            if ((entry->flags & 0x1008U) != 0x1000U)
                SelectCharacterFacialNode(entry->node, 1);
            else
                SelectCharacterFacialNode(entry->node, 0);
            if (entry->name == SHEN_NAME4('S', 'I', 'N', '_')) {
                SetLowQualityHands(entry->node);
                SetCharacterHandRegionEnabled(AllocateHNDL(entry->node), 1, 0, 1);
                SetCharacterHandRegionEnabled(AllocateHNDR(entry->node), 1, 0, 1);
            }
            if (entry->flags & 0x40U) {
                cpamSetFlag(entry->node, 0);
                SetCharacterAlpha(entry->node, 0.0f);
                AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~8U);
            }
            entry->flags &= ~0x1000U;
            return 1;
        }
        if (result < 0)
            npcActiveStreamCost -= entry->streamCost;
        break;
    }
    case 2:
        if (!entry->fadeCounter) {
            entry->streamRouteChangeKind = 0;
            entry->fadeCounter = 0;
            entry->flags = (entry->flags & ~1U) | 2U;
            npcLastHumanLoadResult = 1;
            return 1;
        }
        --entry->fadeCounter;
        SetCharacterAlpha(entry->node, 1.0f - static_cast<float>(entry->fadeCounter) / 15.0f);
        break;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

inline NpcRouteStateContextPrefix *GetNpcRouteStateContext(NpcRouteLiveEntry *entry) {
    return &entry->stateContext;
}

void BindNpcRouteCharacter(NpcRouteLiveEntry *entry) {
    entry->flags = (entry->flags & ~1U) | 2U;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    entry->node = scenFindChara(control->scenarioTask, entry->name);
    entry->motionWork = charFindMotionwork(entry->node);
    npcSlotFlags[entry->poolIndex] |= 1U;
    BindNpcRouteMotionWork(&entry->stateContext, shen_name(), entry->motionWork);
    BindNpcRouteAppliedMovement(&entry->stateContext, shen_name());
    entry->localPosition = entry->worldPosition;
    entry->localPosition.x -= npcRouteOrigin.x;
    entry->localPosition.z -= npcRouteOrigin.z;
    UpdateStreamedNpcMotion(entry);
    if ((static_cast<std::int16_t>(entry->packedMoveFlags) & ~0xff) || (entry->flags & 0x2000U))
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~2U);
    if (entry->flags & 0x08000000U)
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~0xcU);
    mtwkSetRequestedRootMode(entry->motionWork, 1);
    mtwkSetFrameTime(entry->motionWork, 20, LowerBody);
    MapControlData *const currentControl = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (scenGetCharaName(currentControl->scenarioTask, entry->node) == SHEN_NAME4('R', 'E', 'N', '_'))
        mtwkSetTimeRelated(entry->motionWork, 1, 0, 0xa204);
    if (entry->flags & 8U) {
        if (static_cast<std::int8_t>(entry->liveState) == 1 && (GetNpcRouteStateContext(entry)->routeFlags & 0x10U)) {
            RestartNpcAreaRouteContext(entry->node, GetNpcRouteStateContext(entry));
            if (RequestNpcRouteMotion(entry->node, GetNpcRouteStateContext(entry), 0))
                TickNpcRouteStair(entry->node, GetNpcRouteStateContext(entry));
        } else {
            mtwkRequestMotion(entry->motionWork,
                SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
                    static_cast<std::int8_t>(entry->scriptRecord[8])),
                LowerBody);
        }
        return;
    }
    std::int32_t applyMotion = 0;
    switch (static_cast<std::int8_t>(entry->liveState)) {
    case 1: {
        NpcRouteStateContextPrefix *const context = &entry->stateContext;
        RestartNpcAreaRouteContext(entry->node, context);
        if (RequestNpcRouteMotion(entry->node, context, 0))
            TickNpcRouteStair(entry->node, context);
        break;
    }
    case 3:
    case 7:
        SetCharacterWorldPosition(entry->node, &entry->localPosition);
        charaSetWorldRot(entry->node, entry->rotation);
        if (entry->flags & 0x400U) {
            mtwkSetRequestedRootMode(entry->motionWork, 0);
            ResumeNpcTimedMotion(entry);
        }
        applyMotion = 1;
        break;
    case 6:
        entry->currentMotionId = SelectNpcLeaningProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        applyMotion = 1;
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        break;
    case 4:
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~6U);
        SetCharacterWorldPosition(entry->node, &entry->localPosition);
        mtwkAssignDrawCallback(entry->node, NpcRouteCollisionNullsub);
        applyMotion = 1;
        break;
    case 8:
        mtwkSetDrawOverrideData(entry->motionWork, 0);
        AssignCharacterControlFlags(entry->node, 0);
        SetCharacterWorldPosition(entry->node, &entry->localPosition);
        if (entry->flags & 0x40000U)
            entry->animationFrame = static_cast<std::int16_t>(static_cast<std::int32_t>(npcFrameCount + 2 * static_cast<std::int8_t>(entry->scriptRecord[8])) %
                static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)]));
        break;
    case 9: {
        NpcAreaRouteState *const state = GetNpcAreaPointRouteState(entry->currentAreaPoint);
        if (entry->specialAnimationContext) {
            entry->currentMotionId = ResolveNpcAreaAnimation(state, entry);
            if (!entry->specialAnimationContext)
                entry->currentMotionId = ResolveNpcAreaAnimation(state, entry);
        } else {
            entry->currentMotionId = ResolveNpcAreaAnimation(state, entry);
        }
        if (!entry->currentMotionId)
            entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
                static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        ApplyNpcAreaMotion(entry, entry->currentMotionId);
        break;
    }
    default:
        entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        applyMotion = 1;
        break;
    }
    if (applyMotion == 1) {
        mtwkRequestMotion(entry->motionWork, entry->currentMotionId, LowerBody);
        mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BeginNpcRouteStreamOut(NpcRouteLiveEntry *entry, std::int32_t forced) {
    if (!(entry->flags & 2U))
        return 0;
    if (entry->flags & 0x18U)
        return 0;
    entry->flags = (entry->flags & ~2U) | 4U;
    if (forced)
        entry->flags |= 0x20000000U;
    entry->fadeCounter = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t UpdateNpcStreamFade(NpcRouteLiveEntry *entry) {
    if (!IsOfThisVisibilityType(entry->node, CVM_MotionworkDisplayCheck))
        entry->fadeCounter = 15;
    switch (entry->fadeCounter) {
    case 0:
        ++entry->fadeCounter;
        break;
    case 15:
        ReleaseNpcRouteStream(entry);
        return 1;
    default:
        SetCharacterAlpha(entry->node, 1.0f - static_cast<float>(entry->fadeCounter) / 15.0f);
        entry->fadeCounter = static_cast<std::int8_t>(entry->fadeCounter + 1);
        break;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseNpcRouteStream(NpcRouteLiveEntry *entry) {
    npcReleasePak(entry->loadHumanName);
    npcActiveStreamCost =
        SHENMUE_NPC_SIGNED(static_cast<std::uint32_t>(npcActiveStreamCost) - static_cast<std::uint32_t>(static_cast<std::int32_t>(entry->streamCost)));
    entry->flags &= 0xdffffff8U;
    entry->node = SHENMUE_NPC_NULL;
    entry->motionWork = SHENMUE_NPC_NULL;
    if (entry->flags & 0x400U)
        entry->timedMotionHandle->character = SHENMUE_NPC_NULL;
    npcSlotFlags[entry->poolIndex] &= 0xfeU;
    entry->fadeCounter = 0;
    entry->streamRouteChangeKind = 0;
    if (entry->flags & 0x10U) {
        entry->liveState = entry->previousLiveState;
        entry->specialMotionPhase = 0;
        entry->flags &= ~0x10U;
        entry->flags &= ~0x4000U;
    }
    NpcRouteStateContextPrefix *context = &entry->stateContext;
    if (context->flags & 1U)
        ResetNpcRouteContext(context);
    entry->loadHumanResult = 8;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ComputeNpcTargetRotation(const Vector3 *position, const Vector3 *target, Vector3i *rotation) {
    Vector3 direction;
    direction.x = target->x - position->x;
    direction.y = target->y - position->y;
    float &zField = direction.z;
    const float z = target->z - position->z;
    zField = z;
    if (!(std::abs(direction.x) < 0.001f && std::abs(z) < 0.001f))
        rotation->y = static_cast<std::int16_t>(InverseFixedAtan2(direction.x, direction.z));
    else
        rotation->y = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t TickNpcSpecialMotion(NpcRouteLiveEntry *entry) {
    if (!entry->node)
        return 0;
    if ((entry->flags & 0x4000U) && static_cast<std::int8_t>(entry->specialMotionPhase) < 9) {
        entry->flags &= ~0x4000U;
        entry->specialMotionPhase = 9;
    }
    if ((GetNpcRouteStateContext(entry)->flags & 0x1400U) == 0x400U) {
        if (!npcSlotFlags[entry->poolIndex])
            GetNpcRouteStateContext(entry)->routeFlags |= 0x10000U;
        else
            GetNpcRouteStateContext(entry)->routeFlags &= ~0x10000U;
        TickNpcRouteStateContext(GetNpcRouteStateContext(entry));
    }
    if (static_cast<std::int8_t>(entry->previousLiveState) == 9)
        TickNpcAreaRouteState(entry);
    Vector3 position;
    Vector3i facing, rotation;
    std::uint16_t motion;
    switch (static_cast<std::int8_t>(entry->specialMotionPhase)) {
    case 0:
    case 1:
    case 7:
    case 8:
        break;
    case 2:
        if (!mtwkSetRegionFlag(entry->motionWork, 0x41, 1, LowerBody))
            break;
        if (mtwkSetExtraFlag(entry->motionWork, 0x3a, 1))
            break;
        motion = SelectNpcIdleSpecialMotion(entry, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
            static_cast<std::int8_t>(entry->scriptRecord[8]));
        if (!motion) {
            mtwkSetSpeed(entry->motionWork, 0.0f, LowerBody);
            entry->specialMotionPhase = 0;
            break;
        }
        if (motion != mtwkGetAnimationId(entry->motionWork, LowerBody))
            mtwkRequestMotion(entry->motionWork, motion, LowerBody);
        break;
    case 3:
        mtwkUpdateWithPosition(entry->motionWork, &entry->specialMotionTarget, 1);
        if (GetNpcRouteStateContext(entry)->flags & 0x400U) {
            GetNpcRouteStateContext(entry)->routeFlags |= 0x800U;
            entry->specialMotionPhase = 0;
            break;
        }
        if (static_cast<std::int8_t>(entry->previousLiveState) == 4) {
            entry->specialMotionPhase = 0;
            break;
        }
        entry->specialMotionPhase = 2;
        GetCharacterWorldPosition(entry->node, &position);
        GetCharacterWorldRotation(entry->node, &rotation);
        ComputeNpcTargetRotation(&position, &entry->specialMotionTarget, &facing);
        facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) - static_cast<std::int16_t>(rotation.y));
        if (static_cast<std::int8_t>(entry->previousLiveState) == 9) {
            std::uint32_t flags = GetNpcAreaSpecialMotionFlags(*entry);
            if (flags & 1U) {
                const std::uint32_t transition = SelectNpcAreaTransitionMotion(entry);
                motion = static_cast<std::uint16_t>(transition);
                if (motion && motion != mtwkGetAnimationId(entry->motionWork, LowerBody)) {
                    mtwkRequestMotion(entry->motionWork, motion, LowerBody);
                    flags = transition >> 16U;
                }
                if (flags & 4U) {
                    mtwkUpdateWithPosition(entry->motionWork, &entry->specialMotionTarget, 0);
                } else {
                    mtwkUpdateWithPosition(entry->motionWork, &entry->specialMotionTarget, 1);
                    if (flags & 8U) {
                        mtwkUpdateWithPosition(entry->motionWork, 0, 7);
                    } else if (flags & 0x70U) {
                        if (static_cast<std::int16_t>(facing.y) >= 0) {
                            if (flags & 0x10U)
                                mtwkUpdateWithPosition(entry->motionWork, 0, 10);
                            else
                                mtwkUpdateWithPosition(entry->motionWork, 0, 9);
                            if (flags & 0x20U)
                                mtwkUpdateWithPosition(entry->motionWork, 0, 13);
                        } else {
                            if (flags & 0x10U)
                                mtwkUpdateWithPosition(entry->motionWork, 0, 13);
                            else
                                mtwkUpdateWithPosition(entry->motionWork, 0, 12);
                            if (flags & 0x20U)
                                mtwkUpdateWithPosition(entry->motionWork, 0, 10);
                        }
                    }
                }
                entry->specialMotionPhase = 0;
                break;
            }
        }
        motion = 0;
        if (static_cast<std::int8_t>(entry->previousLiveState) == 9) {
            motion = static_cast<std::uint16_t>(SelectNpcAreaTransitionMotion(entry));
            if (motion)
                entry->specialMotionPhase = 0;
        }
        if (entry->flags & 0x800000U)
            break;
        entry->flags |= 0x800000U;
        if (static_cast<std::int16_t>(facing.y) >= 0) {
            if (static_cast<std::int16_t>(facing.y) > 0x4e38)
                facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) - 0x4e38);
        } else if (static_cast<std::int16_t>(facing.y) < -0x4e38)
            facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) + 0x4e38);
        if (!motion) {
            motion = SelectNpcSpecialMotion(entry, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
                static_cast<std::int8_t>(entry->scriptRecord[8]));
            if (!motion) {
                mtwkSetSpeed(entry->motionWork, 0.0f, LowerBody);
                entry->specialMotionPhase = 0;
                break;
            }
        }
        if (!facing.y && motion == mtwkGetAnimationId(entry->motionWork, LowerBody))
            break;
        facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) + static_cast<std::int16_t>(rotation.y));
        mtwkCopyTransition(entry->motionWork, 0, facing.y, 13, 0);
        mtwkSetTimeRelated(entry->motionWork, 1, 5, motion);
        mtwkSetTimeRelated(entry->motionWork, 1, 0, motion);
        break;
    case 4:
        if (GetNpcRouteStateContext(entry)->flags & 0x100U)
            break;
        entry->specialMotionPhase = 0;
        GetCharacterWorldRotation(entry->node, &rotation);
        if (static_cast<std::int16_t>(rotation.y) != static_cast<std::int16_t>(entry->rotation.y))
            mtwkCopyTransition(entry->motionWork, 0, entry->rotation.y, 13, 0);
        break;
    case 5:
        entry->specialMotionPhase = 0;
        if (!QueryNpcSceneMotionPlaying(entry->name))
            mtwkRequestTargetFacing(entry->motionWork, &entry->specialMotionTarget, 0);
        else
            mtwkRequestTargetFacing(entry->motionWork, &entry->specialMotionTarget, 2);
        if (static_cast<std::int8_t>(entry->previousLiveState) == 4 || static_cast<std::int8_t>(entry->previousLiveState) == 9 ||
            (GetNpcRouteStateContext(entry)->flags & 0x400U))
            break;
        GetCharacterWorldPosition(entry->node, &position);
        GetCharacterWorldRotation(entry->node, &rotation);
        ComputeNpcTargetRotation(&position, &entry->specialMotionTarget, &facing);
        facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) - static_cast<std::int16_t>(rotation.y));
        if (static_cast<std::int16_t>(facing.y) >= 0) {
            if (static_cast<std::int16_t>(facing.y) > 0x4e38)
                facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) - 0x4e38);
        } else if (static_cast<std::int16_t>(facing.y) < -0x4e38)
            facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) + 0x4e38);
        facing.y = static_cast<std::int16_t>(static_cast<std::int16_t>(facing.y) + static_cast<std::int16_t>(rotation.y));
        mtwkCopyTransition(entry->motionWork, 0, facing.y, 13, 0);
        motion = SelectNpcSpecialMotion(entry, static_cast<std::int8_t>(entry->scriptRecord[5]), static_cast<std::int8_t>(entry->scriptRecord[4]),
            static_cast<std::int8_t>(entry->scriptRecord[8]));
        if (!motion) {
            mtwkSetSpeed(entry->motionWork, 0.0f, LowerBody);
            break;
        }
        mtwkSetTimeRelated(entry->motionWork, 1, 5, motion);
        mtwkSetTimeRelated(entry->motionWork, 1, 0, motion);
        break;
    case 6:
        entry->specialMotionPhase = 0;
        mtwkRequestTargetFacing(entry->motionWork, 0, 0);
        break;
    case 9:
        if (GetNpcRouteStateContext(entry)->flags & 0x8000U)
            ResumeNpcRouteAfterSceneChange(entry->name);
        mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
        switch (static_cast<std::int8_t>(entry->previousLiveState)) {
        case 5:
            entry->specialMotionPhase = 11;
            position = entry->worldPosition;
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            mtwkCopyTransition(entry->motionWork, &position, entry->rotation.y, 6, 0);
            break;
        case 4:
        case 9:
            entry->liveState = entry->previousLiveState;
            entry->specialMotionPhase = 0;
            entry->flags &= ~0x10U;
            entry->commandFlags &= ~0x40U;
            ResumeNpcAreaSpecialMotion(entry);
            return 0;
        case 1:
            entry->liveState = entry->previousLiveState;
            entry->specialMotionPhase = 0;
            entry->flags &= ~0x10U;
            SetNpcRouteMotionState(GetNpcRouteStateContext(entry), shen_name(), 3);
            return 0;
        default: {
            entry->specialMotionPhase = 10;
            mtwkCopyTransition(entry->motionWork, &entry->specialMotionRestorePosition, entry->rotation.y, 6, 0);
            mtwkSetTimeRelated(entry->motionWork, 0, 6, 0);
            mtwkSetTimeRelated(entry->motionWork, 1, 5, entry->currentMotionId);
            MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (scenGetCharaName(control->scenarioTask, entry->node) == SHEN_NAME4('R', 'E', 'N', '_'))
                mtwkSetTimeRelated(entry->motionWork, 1, 0, 0xa204);
            mtwkSetRegionEvalTime(entry->motionWork, static_cast<float>(entry->animationFrame), LowerBody);
            break;
        }
        }
        break;
    case 10:
    case 11:
        if (!mtwkSetExtraFlag(entry->motionWork, 0x3a, 1)) {
            if (static_cast<std::int8_t>(entry->specialMotionPhase) == 11)
                ResumeNpcScheduleMotion(entry);
            entry->liveState = entry->previousLiveState;
            entry->specialMotionPhase = 0;
            entry->flags &= ~0x10U;
            return 0;
        }
        break;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void UpdateStreamedNpcMotion(NpcRouteLiveEntry *entry) {
    if (!entry->node)
        return;
    FindNpcIntersectionInScene(entry->localPosition, entry->packedMoveFlags);
    if (entry->flags & 0xa000U)
        entry->localPosition.y = entry->forcedY;
    SetCharacterWorldPosition(entry->node, &entry->localPosition);
    charaSetWorldRot(entry->node, entry->rotation);
    if (camCtrlIsInsideDepthScaledRect(entry->localPosition, 2.0f))
        entry->streamRetryTicks = 0;
    else
        ++entry->streamRetryTicks;
    mtwkSetEnabled(entry->motionWork, 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ApplyNpcCommandMotion(NpcRouteLiveEntry *entry, std::int32_t opcode) {
    if (!entry->node || (entry->currentMotionId == mtwkGetAnimationId(entry->motionWork, LowerBody) && mtwkIsMotionNonLooping(entry->motionWork, 0)))
        return;
    switch (opcode) {
    case 8:
        mtwkSetRequestedRootMode(entry->motionWork, 1);
        mtwkSetFrameTime(entry->motionWork, 20, LowerBody);
        AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~6U);
        mtwkAssignDrawCallback(entry->node, NpcRouteCollisionNullsub);
        break;
    case 40:
        mtwkSetRequestedRootMode(entry->motionWork, 0);
        ResumeNpcTimedMotion(entry);
        break;
    case 7:
    case 10:
    case 16:
        mtwkSetRequestedRootMode(entry->motionWork, 1);
        mtwkSetFrameTime(entry->motionWork, 20, LowerBody);
        break;
    }
    if (entry->flags & 0xa000U)
        entry->localPosition.y = entry->forcedY;
    else
        FindNpcIntersectionInScene(entry->localPosition, entry->packedMoveFlags);
    SetCharacterWorldPosition(entry->node, &entry->localPosition);
    charaSetWorldRot(entry->node, entry->rotation);
    mtwkRequestMotion(entry->motionWork, entry->currentMotionId, LowerBody);
    mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
NpcRouteStateContextPrefix *FindNpcRouteStateContext(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    return &entry->stateContext;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t npcGetPosFor(shen_name name, Vector3 *position) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (!npcPositionQueryMode) {
        if (!entry->node)
            return 0;
        npcPositionQueryMode = 1;
    }
    Vector3 local = entry->worldPosition;
    local.x -= npcRouteOrigin.x;
    local.z -= npcRouteOrigin.z;
    *position = local;
    return 1;
}

#else

std::int32_t npcGetPosFor(shen_name character, Vector3 *position) {
    if (!npcRegistryTask.current)
        return 0;
    for (const NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!npcPositionQueryMode) {
            if (!entry.node)
                return 0;
            npcPositionQueryMode = 1;
        }
        *position = entry.worldPosition;
        position->x -= npcSchedule.areaOrigin.x;
        position->z -= npcSchedule.areaOrigin.z;
        return 1;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t MarkNpcRoutePartyControl(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NRSF_AbstractActive))
            return 0;
        entry.flags |= NRSF_Unknown80;
        return 1;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t ClearNpcRoutePartyControl(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NRSF_AbstractActive))
            return 0;
        entry.flags &= ~NRSF_Unknown80;
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t SetNpcRouteControlHold(shen_name name, std::int32_t enabled) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    std::int32_t previous;
    if (!entry)
        return 0;
    previous = static_cast<std::int32_t>(entry->controlFlags) >> 5 & 1;
    if (enabled)
        entry->controlFlags |= 0x20U;
    else
        entry->controlFlags &= ~0x20U;
    return previous;
}

#else

// @verified - p
std::uint64_t SetNpcRouteControlHold(shen_name character, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        const std::uint32_t previous = entry.controlFlags;
        entry.controlFlags = enabled ? previous | 0x20U : previous & ~0x20U;
        return (previous & 0x20U) != 0;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t QueueNpcMotionAttachment(NpcRouteLiveEntry *entry, std::uint16_t motion, std::int32_t flags, std::int32_t slot) {
    if (!motion)
        return 0;
    entry->motionAttachments[slot].motion = static_cast<std::uint16_t>(motion);
    entry->motionAttachments[slot].flags = (entry->motionAttachments[slot].flags & 0xffc1U) | (flags & 0x3eU);
    std::uint16_t available = flags & 0x1eU;
    for (std::int32_t index = 0; index < slot; ++index)
        if (entry->motionAttachments[index].motion)
            available &= ~(entry->motionAttachments[index].flags & 0x1eU);
    if (available)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CaptureNpcMotionAttachmentFrame(NpcRouteLiveEntry *entry, NpcMotionAttachment *attachment) {
    for (std::int32_t region = 1; region < 5; ++region) {
        const MotionRegion part = static_cast<MotionRegion>(region);
        if (((attachment->flags >> region) & 1) && mtwkSetRegionFlag(entry->motionWork, 0x48, 1, part) &&
            attachment->motion == mtwkGetAnimationId(entry->motionWork, part)) {
            attachment->frame = mtwkGetFrame(entry->motionWork, part);
            if (mtwkSetRegionFlag(entry->motionWork, 0x44, 1, part))
                attachment->frame -= 12.0f;
            return;
        }
    }
    attachment->frame = -12.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RequestNpcMotionAttachmentRegion(NpcRouteLiveEntry *entry, const NpcMotionAttachment *attachment, std::uint32_t region) {
    const MotionRegion part = static_cast<MotionRegion>(region);
    float frame = attachment->frame + 12.0f;
    frame *= mtwkGetSpeed(entry->motionWork, part);
    const float last = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(attachment->motion))) - 1.0f;
    if (last < 0.0f)
        return;
    if (attachment->flags & 0x20U) {
        while (frame > last)
            frame -= last;
    } else if (frame > last)
        frame = last;
    mtwkRequestMotion(entry->motionWork, attachment->motion, part);
    mtwkSetRegionEvalTime(entry->motionWork, frame, part);
    mtwkSetRequestedRootMode(entry->motionWork, 1);
    mtwkSetFrameTime(entry->motionWork, 12, part);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EvaluateNpcMotionAttachments(NpcRouteLiveEntry *entry) {
    std::int32_t complete[3] = {0, 0, 0};
    std::int32_t slot;
    for (slot = 0; slot < 3; ++slot) {
        if (entry->motionAttachments[slot].motion)
            CaptureNpcMotionAttachmentFrame(entry, &entry->motionAttachments[slot]);
        else
            entry->motionAttachments[slot].flags = 0;
    }
    for (std::int32_t region = 1; region < 5; ++region) {
        std::int32_t used = 0;
        for (slot = 0; slot < 3; ++slot) {
            if (entry->motionAttachments[slot].motion && !(entry->motionAttachments[slot].flags & 0x40)) {
                const std::uint16_t flags = entry->motionAttachments[slot].flags;
                if ((flags >> region) & 1) {
                    used |= flags & (1 << region);
                    std::uint16_t active = mtwkGetAnimationId(entry->motionWork, static_cast<MotionRegion>(region));
                    if (!mtwkSetRegionFlag(entry->motionWork, 0x48, 1, static_cast<MotionRegion>(region)))
                        active = 0;
                    const std::uint16_t queued = mtwkGetQueuedAnimationId(entry->motionWork, static_cast<MotionRegion>(region));
                    if ((!active && !queued) || (active && entry->motionAttachments[slot].motion != active) ||
                        (queued && entry->motionAttachments[slot].motion != queued))
                        RequestNpcMotionAttachmentRegion(entry, &entry->motionAttachments[slot], region);
                    else if (!(entry->motionAttachments[slot].flags & 0x20) &&
                        mtwkSetRegionFlag(entry->motionWork, 0x41, 1, static_cast<MotionRegion>(region))) {
                        mtwkRequestRegionDetach(entry->motionWork, static_cast<MotionRegion>(region));
                        complete[slot] = 1;
                    }
                    break;
                }
            }
        }
        if (!used)
            mtwkRequestRegionDetach(entry->motionWork, static_cast<MotionRegion>(region));
    }
    for (slot = 0; slot < 3; ++slot)
        if (complete[slot]) {
            entry->motionAttachments[slot].motion = 0;
            entry->motionAttachments[slot].flags = 0;
        }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsNpcSceneMotionPlaying(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (entry && entry->node) {
        for (std::int32_t region = 1; region < 5; ++region) {
            const MotionRegion part = static_cast<MotionRegion>(region);
            if (mtwkSetRegionFlag(entry->motionWork, 0x48, 1, part)) {
                const std::uint32_t animation = mtwkGetAnimationId(entry->motionWork, part);
                std::uint32_t motion;
                if (entry->flags & 0x10U)
                    motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe364);
                else
                    motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe353);
                if (animation == motion)
                    return true;
            }
        }
    }
    return false;
}

#else

// @verified - p
std::uint64_t IsNpcSceneMotionPlaying(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node)
        return 0;
    for (std::uint32_t region = 1; region < 5; ++region) {
        if (mtwkSetRegionFlag(entry->motionWork, 72, 1, static_cast<MotionRegion>(region)) &&
            mtwkGetAnimationId(entry->motionWork, static_cast<MotionRegion>(region)) == RemapNpcMotionForRig(GetCharacterBoneId(entry->node), NPC_SCENE_MOTION))
            return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ClearNpcMotionAttachment(NpcRouteLiveEntry *entry, std::int32_t slot) {
    NpcMotionAttachment *attachment = &entry->motionAttachments[slot];
    if (slot < 3)
        attachment->flags = attachment->motion = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t SelectNpcRandomMotionAttachment(NpcRouteLiveEntry *) {
    if (dc::sdk::RandomFloat() * 3.0f > 1.0f)
        return 0;
    return 0x148075U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void UpdateNpcMotionAttachments(NpcRouteLiveEntry *entry) {
    std::uint32_t motion;
    if (entry->controlFlags & 0x20U)
        return;
    if (!entry->node) {
        entry->attachmentFrameCounter = 0;
        return;
    }
    if (mtwkSetExtraFlag(entry->motionWork, 0x3c, 1))
        return;
    if (entry->controlFlags & 0x10U)
        entry->motionAttachments[0].flags |= 0x40U;
    if (npcCloudyState) {
        if (entry->flags & 0x10U)
            motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe364);
        else
            motion = RemapNpcMotionForRig(charGetMotionRigType(entry->node), 0xe353);
        const std::uint32_t attachedMotion = entry->motionAttachments[0].motion;
        const std::uint32_t preserveMotion = 0x10U;
        if (attachedMotion != 0) {
            if (attachedMotion == motion)
                goto updateAttachmentRegions;
        } else if (entry->flags & preserveMotion) {
            goto updateAttachmentRegions;
        }
        QueueNpcMotionAttachment(entry, static_cast<std::uint16_t>(motion), 0x28, 0);
    } else if (entry->motionAttachments[0].motion && (entry->flags & 0x10U) == 0) {
        entry->motionAttachments[0].motion = 0;
        entry->motionAttachments[0].flags = 0;
    }
updateAttachmentRegions:
    if (entry->controlFlags & 3U) {
        mtwkRequestRegionDetach(entry->motionWork, LowerBody);
        if (entry->controlFlags & 1U) {
            for (std::uint32_t slot = 0; slot < 3; ++slot) {
                entry->motionAttachments[slot].motion = 0;
                entry->motionAttachments[slot].flags = 0;
            }
        }
    } else {
        EvaluateNpcMotionAttachments(entry);
        if ((entry->controlFlags & 4U) && !entry->motionAttachments[2].motion) {
            if (++entry->attachmentFrameCounter <= 90U)
                return;
            motion = SelectNpcRandomMotionAttachment(entry);
            if (motion && !QueueNpcMotionAttachment(entry, static_cast<std::uint16_t>(motion), motion >> 16U, 2)) {
                entry->motionAttachments[2].motion = 0;
                entry->motionAttachments[2].flags = 0;
            }
        }
    }
    entry->attachmentFrameCounter = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t BranchNpcIfWeekday(NpcRouteLiveEntry *entry) {
    std::int32_t branched = 0;
    const std::int8_t week = static_cast<std::int8_t>(npcCurrentDateTime.week);
    if (week != 0 && week != 6)
        branched = SelectNpcScheduleBranch(entry, *reinterpret_cast<const std::int32_t *>(entry->currentCommand + 4));
    if (!branched)
        entry->currentCommand += 8;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t BranchNpcIfWeekend(NpcRouteLiveEntry *entry) {
    std::int32_t branched = 0;
    const std::int8_t week = static_cast<std::int8_t>(npcCurrentDateTime.week);
    if (week == 0 || week == 6)
        branched = SelectNpcScheduleBranch(entry, *reinterpret_cast<const std::int32_t *>(entry->currentCommand + 4));
    if (!branched)
        entry->currentCommand += 8;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ClearNpcScheduleSeeds() {
    for (std::int32_t index = 0; index < 2; ++index)
        npcScheduleSeedNames[index] = shen_name();
    hasPendingNpcScheduleSeeds = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsNpcScheduleSeedName(shen_name name) {
    for (std::int32_t index = 0; index < 2; ++index)
        if (npcScheduleSeedNames[index] == name)
            return true;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RestoreNpcScheduleSeedPayload(const void *payload) {
    pendingNpcScheduleSeeds = static_cast<const std::byte *>(payload);
    hasPendingNpcScheduleSeeds = 1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcWalkingMotion(const Character *character) {
    const std::int32_t rig = charGetMotionRigType(character);
    return rig < 15 ? (npcWalkingMotions + 1)[rig] : 0xf084;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcRunningMotion(const Character *character) {
    const std::int32_t rig = charGetMotionRigType(character);
    return rig < 15 ? (npcRunningMotions + 1)[rig] : 0xf006;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetNpcRestingMotion(const Character *character) {
    switch (charGetMotionRigType(character)) {
    case 0:
        return 0xf03f;
    case 1:
        return 0xf05f;
    case 2:
        return 0xf07f;
    case 3:
        return 0xf057;
    case 4:
        return 0xf09f;
    case 5:
        return 0xf0d2;
    case 6:
        return 0xf061;
    case 7:
        return 0xf087;
    case 8: {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        return scenGetCharaName(control->scenarioTask, character) == SHEN_NAME4('R', 'E', 'N', '_') ? 0xa204 : 0xf085;
    }
    case 9:
        return 0xf079;
    case 10:
        return 0xf059;
    case 11:
        return 0xf0a8;
    case 13:
        return 0xf0a5;
    case 14:
        return 0xf0a7;
    case 15:
        return 0xf0ea;
    default:
        return 0xf085;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcStairDescendingMotion(Character *character) {
    static const std::uint16_t motions[16] = {
        0xf0f4, 0xf040, 0xf040, 0xf080, 0xf040, 0xf0a0, 0xf0d3, 0xf062, 0xf088, 0x8025, 0xf07a, 0xf05a, 0x8025, 0x8025, 0xf040, 0xf0f3};
    const std::int32_t rig = charGetMotionRigType(character);
    return rig < 15 ? (motions + 1)[rig] : 0x8025;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcStairAlternateMotion(Character *character) {
    static const std::uint16_t motions[16] = {
        0xf0f3, 0x8067, 0x8067, 0x8067, 0x8067, 0x806a, 0x806a, 0x8068, 0x8069, 0x8069, 0x8069, 0x8069, 0x8069, 0x8069, 0x8067, 0x8067};
    const std::int32_t rig = charGetMotionRigType(character);
    return rig < 15 ? (motions + 1)[rig] : 0x8069;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t GetNpcProfileImageType(Character *character, std::int32_t age, std::int32_t sex, std::int32_t *type) {
    if (character) {
        const std::int32_t source = GetCharacterImageSourceType(character);
        *type = source / 32;
    } else
        *type = 0;
    std::int32_t group;
    if (age < 13)
        group = 2;
    else if (age > 55)
        group = 4;
    else
        group = 0;
    if (sex)
        ++group;
    if (*type > 2)
        group = -1;
    return group;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcLeaningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile);

std::uint16_t SelectNpcProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile) {
    std::int32_t type;
    const std::int32_t group = GetNpcProfileImageType(character, age, sex, &type);
    return group == -1 ? 0x8028 : npcSeatedMotions[group][profile][type];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcLeaningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile) {
    std::int32_t type;
    const std::int32_t group = GetNpcProfileImageType(character, age, sex, &type);
    return group == -1 ? 0x8034 : npcLeaningMotions[group][profile][type];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint16_t SelectNpcRecliningProfileMotion(Character *character, std::int32_t age, std::int32_t sex, std::int32_t profile) {
    static const std::uint16_t motions[6][3][3] = {0xe38b, 0xe38b, 0xe38b, 0xe38a, 0xe38c, 0xe38c, 0xe38a, 0xe38a, 0xe38a, 0xe388, 0xe388, 0xe388, 0xe244,
        0xe244, 0xe244, 0xe387, 0xe387, 0xe387, 0xe249, 0xe249, 0xe249, 0xe38a, 0xe38a, 0xe38a, 0xe249, 0xe249, 0xe249, 0xe249, 0xe249, 0xe249, 0xe249, 0xe249,
        0xe249, 0xe387, 0xe387, 0xe387, 0xe24a, 0xe24a, 0xe24a, 0xe38a, 0xe38a, 0xe38a, 0xe389, 0xe389, 0xe389, 0xe24a, 0xe24a, 0xe24a, 0xe387, 0xe387, 0xe387,
        0xe389, 0xe389, 0xe389};
    std::int32_t type;
    const std::int32_t group = GetNpcProfileImageType(character, age, sex, &type);
    return group == -1 ? 0xe38a : motions[group][profile][type];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry *entry, std::int32_t mode) {
    if (entry->loadHumanResult < 0)
        entry->loadHumanResult = 8;
    if (entry->flags & 8U)
        return 2;
    if (entry->flags & 0x200U)
        return 3;
    if (!(entry->flags & 7U))
        return 4;
    if (mode == 1) {
        const std::int32_t pending = (entry->flags & 1U) != 0;
        if (pending)
            ReleaseNpcRouteStream(entry);
        if (entry->flags & 2U)
            BeginNpcRouteStreamOut(entry, 0);
        if (entry->flags & 4U)
            UpdateNpcStreamFade(entry);
        return pending;
    }
    if (!mode || (mode == 2 && (entry->flags & 1U)))
        ReleaseNpcRouteStream(entry);
    return 1;
}

std::uint32_t npcColdResetMode = 1;

std::uint32_t SetNpcColdResetMode(std::uint32_t mode) {
    if (!mode && IsNpcRegistryTaskActive()) {
        ReleaseNpcTransitionHumanPackages();
        NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
        for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry) {
            if (entry->stateContext.flags & 1U) {
                if ((entry->stateContext.flags & 0x300U) || (entry->stateContext.routeFlags & 0x10U))
                    continue;
                if (!(entry->flags & 0x4000000U)) {
                    SetNpcStreamLifecycleState(entry, 1);
                    continue;
                }
            }
            if (static_cast<std::uint8_t>(entry->scriptRecord[0x0b]) & 0x10U)
                SetNpcStreamLifecycleState(entry, 1);
        }
    }
    npcColdResetMode = mode;
    return mode;
}

std::int32_t ResetAllNpcRouteStates() {
    if (IsNpcRegistryTaskActive()) {
        ReleaseNpcTransitionHumanPackages();
        NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
        for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry)
            SetNpcStreamLifecycleState(entry, 2);
    }
    return 1;
}

std::int32_t RegisterNpcFlags(shen_name name, std::uint32_t flags, std::int32_t operation) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    switch (operation) {
    case 0: entry->controlFlags &= ~flags; break;
    case 1: entry->controlFlags |= flags; break;
    case 2: return (entry->controlFlags & flags) != 0;
    }
    return 1;
}

void StopNpcFollowingMotionForEvent(shen_name name) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry || *reinterpret_cast<const std::uint32_t *>(entry->scriptRecord) == SHEN_NAME4('M', 'O', 'V', 'E'))
        return;
    if (entry->flags & 8U) {
        npcPoolTarget += entry->streamCost;
        entry->flags &= 0xbffffff7U;
        entry->controlFlags &= ~2U;
        if (entry->node)
            SelectCharacterFacialNode(entry->node, 0);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t FindNpcAreaPointByName(shen_name name, std::int32_t mode);

std::int32_t npcStartPlayEntityAtPos(shen_name name, shen_name pointName, const Vector3 *position, std::int32_t yaw) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if (pointName) {
        const std::int32_t point = FindNpcAreaPointByName(pointName, 0);
        if (point < 0)
            return 0;
        entry->currentAreaPoint = static_cast<std::int16_t>(point);
    }
    entry->rotation.y = yaw;
    entry->scheduleProgress = 3;
    entry->liveState = 0;
    ResetNpcEntryToAreaPoint(entry);
    FindNpcIntersectionInScene(entry->localPosition, entry->packedMoveFlags);
    entry->worldPosition = entry->localPosition;
    entry->worldPosition.x += npcRouteOrigin.x;
    entry->worldPosition.z += npcRouteOrigin.z;
    if (entry->stateContext.flags & 1U)
        StopNpcScriptRoute(&entry->stateContext, shen_name(), yaw);
    if (!pointName) {
        if (position)
            entry->localPosition = *position;
        else
            entry->localPosition.x = entry->localPosition.y = entry->localPosition.z = 0.0f;
        entry->worldPosition = entry->localPosition;
        entry->worldPosition.x += npcRouteOrigin.x;
        entry->worldPosition.z += npcRouteOrigin.z;
    }
    if (entry->node) {
        mtwkCopyTransition(entry->motionWork, 0, 0, 1, 0);
        SetCharacterWorldPosition(entry->node, &entry->localPosition);
        charaSetWorldRot(entry->node, entry->rotation);
        mtwkSetRegionFlag(entry->motionWork, 0x47, 0, LowerBody);
        mtwkSetExtraFlag(entry->motionWork, 14, 0);
        entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
            static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8]));
        mtwkRequestMotion(entry->motionWork, entry->currentMotionId, LowerBody);
    }
    return 1;
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetNpcNewLoadState() {
    return shenmue::s2::GetNpcNewLoadState();
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_ForceLoadNpc(std::uint32_t name) {
    return shenmue::s2::EV_ForceLoadNpc(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcStreamOrLoadHuman(std::uint32_t name) {
    return shenmue::s2::EV_npcStreamOrLoadHuman(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_ReleaseNpcForRouteReload(std::uint32_t name) {
    return shenmue::s2::ReleaseNpcForRouteReload(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_ReleaseNpcForRouteReloadAndStoreCount(std::uint32_t name) {
    return shenmue::s2::ReleaseNpcForRouteReloadAndStoreCount(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_IsNpcRouteLoadedOrVisible(std::uint32_t name) {
    return shenmue::s2::IsNpcRouteLoadedOrVisible(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_NpcWait() {
    return shenmue::s2::NpcWait();
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_CountNpcLiveStreams() {
    return shenmue::s2::CountNpcLiveStreams();
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcGetFlagFor(std::uint32_t name) {
    return shenmue::s2::npcGetCanInteract(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_CheckWarmStartNpcRoute(std::uint32_t name) {
    return shenmue::s2::EV_CheckWarmStartNpcRoute(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_PrepareNpcRouteReload(std::uint32_t name) {
    return shenmue::s2::PrepareNpcRouteReload(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_IsNpcRouteReloadPrepared(std::uint32_t name) {
    return shenmue::s2::IsNpcRouteReloadPrepared(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcFree(std::uint32_t name) {
    return shenmue::s2::EV_npcFree(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcUpdateFlags(std::uint32_t name) {
    return shenmue::s2::EV_npcUpdateFlags(shenmue::shen_name(name));
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_UnknownCharaFunc(std::uint32_t name) {
    return shenmue::s2::EV_npcUnknownFunc(shenmue::shen_name(name));
}
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetNpcRouteUpdatePaused(std::uint32_t name, std::int32_t enabled) {
    return shenmue::s2::SetNpcRouteControlHold(shenmue::shen_name(name), enabled);
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_IsNpcAnimationActive(std::uint32_t name) {
    return shenmue::s2::GetNpcScriptRouteCompletion(shenmue::shen_name(name));
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_NPC_NewLoadSw_0(std::uint32_t state) {
    return shenmue::s2::EV_NPC_NewLoadSw(state);
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_EV_ResetFreeTalkFlags() {
    return shenmue::s2::EV_ResetFreeTalkFlags();
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_PrepareNpcAllFromArray() {
    return shenmue::s2::PrepareNpcAllFromArray();
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcSetAreaName(
    std::uint32_t name, const shenmue::Vector3 *center, std::int32_t enabled, float radius) {
    return shenmue::s2::npcSetAreaName(shenmue::shen_name(name), center, radius, enabled);
}

#undef SHENMUE_NPC_NULL
#undef SHENMUE_NPC_NOEXCEPT
#undef SHENMUE_NPC_SIGNED
#else
#endif
