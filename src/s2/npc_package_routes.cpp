#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/npc_data_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "common/matrix_stack.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/math.hpp"
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cfloat>
#include <cmath>

#define SHENMUE_NPC_EPSILON FLT_EPSILON
#include "s2/camera_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/money_icon.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/gauge.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/pause.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/memo_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include <stdio.h>
#include <string.h>
#define UI_STRLEN ::strlen
#define UI_NOEXCEPT
#define UI_ICON(name) name
#define UI_ZERO
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#endif
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#include "s2/event_character_model_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/party_companion.hpp"
#define SHENMUE_NPC_NULL 0
#define SHENMUE_NPC_NOEXCEPT
#define SHENMUE_NPC_SIGNED(value) static_cast<std::int32_t>(value)
#include "s2/player_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include "s2/texture_dc.hpp"
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/directory_cache.hpp"
#include <sg_gd.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/event_overlay.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "common/math.hpp"
#include "platform/dc/screen.hpp"
#define SPRITE_NULL 0
#define SPRITE_ZERO
#define SPRITE_INIT =
#define SPRITE_ARRAY(type, name, count) type name[count]
#define SPRITE_DATA(name) (name)
#define SPRITE_MEMCPY ::memcpy
#define SPRITE_ALIGN(name) name
#define SPRITE_MODE(name) name
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#endif
#include "s2/character_model_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_transparency_dc.hpp"
#endif
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/prop_dc.hpp"
#else
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
#include "s2/scenario_characters.hpp"
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

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

using namespace npc_package_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace npc_package_detail {

const std::byte *GetNpcAreaPoint(std::uint16_t point);
const std::byte *GetNpcRouteNode(std::uint16_t node);
std::uint64_t SampleNpcSplineWindow(const Vector3 *points, std::int32_t pointCount, float parameter, Vector3 *position);
bool SampleNpcRouteNode(std::uint16_t node, float parameter, Vector3 &position);
std::int32_t GetNpcRouteNodePointCount(std::int32_t route);
const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);
std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary);
std::int32_t SampleNpcRouteWindow(NpcRouteStateContext &context, Vector3 *position, bool maintainWindow);
std::uint16_t SelectNpcLocomotion(const NpcRouteStateContext &context, std::uint32_t selector);
void SetNpcLocomotionScale(NpcRouteStateContext &context, float scale);
bool RequestNpcRouteMotion(NpcRouteStateContext &context, bool explicitRequest);
std::uint8_t UpdateNpcRouteAvoidance(NpcRouteStateContext &context);
void UpdateNpcAmbientLook(NpcRouteStateContext &context);
void TickNpcRouteMovement(NpcRouteStateContext &context);
std::int32_t FindNearestNpcParameter(NpcRouteStateContext &context, const Vector3 &position);
void EnterNpcRoute(NpcRouteStateContext &context);
void BuildNpcRoute(NpcRouteStateContext &context);
void BindNpcRouteWindow(NpcRouteStateContext &context);
void TickNpcDoorEgress(NpcRouteStateContext &context);
void TickNpcRouteApproach(NpcRouteStateContext &context);
void TickNpcRouteTraversal(NpcRouteStateContext &context);
void TickNpcDoorIngress(NpcRouteStateContext &context);
bool FinishNpcRoute(NpcRouteStateContext &context);
void UpdateLinkedNpcRoute(NpcRouteStateContext &context);

} // namespace npc_package_detail

std::uint64_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);
bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);
std::uint32_t QueryNpcMoveObstruction(NpcRouteLiveEntry &entry, const Vector3 &target, bool door, std::int16_t angle, float probeRadius, float probeLength);
Vector3 NpcRouteAreaToScene(Vector3 position);
void SetNpcRouteMotionState(NpcRouteStateContext &context, std::int32_t operation, bool explicitRequest);
void ResumeNpcScheduleMotion(NpcRouteLiveEntry &entry);
bool TickNpcRouteStateContext(NpcRouteStateContext *state);
bool ReleaseNpcRouteControl(NpcRouteStateContext *context, shen_name character, std::int32_t yaw);
std::int32_t SampleNpcRoute(shen_name character, std::int32_t parameter, Vector3 *position, std::int32_t *yaw);
std::int32_t FindNearestNpcRouteParameter(shen_name character, const Vector3 *position);
float GetNpcRouteSegmentLength(shen_name character, std::int32_t parameter);
bool StartNpcScriptAreaRoute(shen_name character, shen_name oldPoint, shen_name newPoint, std::int32_t yaw);
void SetNpcScriptRouteSpeed(NpcRouteStateContext *context, shen_name character, float speed);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

struct NpcRouteAvoidanceCandidate {
    void *source;
    std::int32_t kind;
    float distance;
    std::int32_t yaw, yawDistance;
    NpcRouteAvoidanceCandidate *previous, *next;
};

typedef char NpcRouteAvoidanceCandidateSize[(sizeof(NpcRouteAvoidanceCandidate) == 0x1c) ? 1 : -1];

namespace {

struct NpcTransitionEntry {
    shen_name area, character;
    Vector3 position;
    std::uint16_t yaw, visibility;
    const std::uint16_t *motions;
};

} // namespace

const Vector3 *GetNpcRouteStartPoint(const NpcRouteSpline *routes, std::int16_t node);

bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags);

float GetNpcMotionRigScale(const Character *character);

void ResetNpcScriptContext(NpcRouteStateContextPrefix *context);

std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *state);

void InitializeNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, float scale);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary);

void BuildNpcSplineInterpolationMatrix(Matrix4x4 *matrix, float parameter);

void EnterNpcRoute();

void BuildNpcRoute();

void RequestNpcRouteDoor();

std::int32_t ResumeNpcRouteAfterSceneChange(shen_name name);

std::int32_t FinishNpcRoute();

void BindNpcRouteWindow();

void TickNpcDoorEgress();

void TickNpcDoorIngress();

void TransformNpcPointToYawFrame(const Vector3 *origin, std::int32_t yaw, Vector3 *point);

std::int32_t GetNpcRoutePointYaw(const Vector3 *start, const Vector3 *end);

std::int32_t AdvanceUnstreamedNpcRoute();

void TickNpcRouteApproach();

void TickNpcRouteTraversal();

void TickNpcRouteMovement();

void UpdateNpcAmbientLook();

std::int32_t UpdateNpcRouteAvoidance();

std::int32_t SampleNpcRouteWindow(NpcRouteStateContextPrefix *context, Vector3 *position, std::int32_t advance);

bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest);

std::int32_t FindNearestNpcParameter(NpcRouteStateContextPrefix *context, const Vector3 *position);

bool FindNpcRoutePath(NpcRouteStateContextPrefix *context);

std::int32_t RequestNpcSceneRoute(shen_name name, shen_name area);

void UpdateLinkedNpcRoute();

std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

std::uint32_t SelectNpcRouteMovementSlot();

std::int32_t ClearNpcSavedDateTimeFlag();

std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

std::uint32_t SelectNpcRouteMovementSlot();

std::int32_t ClearNpcSavedDateTimeFlag();

std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

std::uint32_t SelectNpcRouteMovementSlot();

std::int32_t ClearNpcSavedDateTimeFlag();

std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

std::uint32_t SelectNpcRouteMovementSlot();

std::int32_t ClearNpcSavedDateTimeFlag();

std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);

std::int32_t GetNpcRestingMotion(const Character *character);

std::uint16_t SelectNpcWalkingMotion(const Character *character);
std::uint16_t SelectNpcRunningMotion(const Character *character);

std::int32_t GetNpcRunCapability(Character *character);

NpcRouteLiveEntry *FindNpcRouteEntry(shen_name name) SHENMUE_NPC_NOEXCEPT;

std::int32_t ReleaseNpcScriptRouteSlot(shen_name name);

std::int32_t CreateScriptNpcRouteSlot(shen_name name);

std::int32_t BeginNpcRouteStreamOut(NpcRouteLiveEntry *entry, std::int32_t forced);

NpcRouteStateContextPrefix *FindNpcRouteStateContext(shen_name name);
float EstimateNpcRouteParameter(shen_name name, NpcRouteStateContextPrefix *context);

bool IsNpcScheduleSeedName(shen_name name);

void RestoreNpcScheduleSeedPayload(const void *payload);

void NpcRegistryTask_Nullsub(void *);

void NpcRegistryTask_Callback(void *);

HLib::Task *npc_Callback_Enqueue(std::int8_t index, std::int32_t act, shen_name area);

void ResetNpcRouteOrigin();

void LoadCurrentNpcSchedule(const char *filename, std::int32_t loadResources);

void NPC_Route_Control();

void EV_LoadNPCTable(const char *filename);

void InitializeNpcRouteContext(NpcRouteLiveEntry *entry);

void InitializeNpcInterpreterState(NpcRouteLiveEntry *entry, std::int32_t slot);

void SetNpcRouteMovementCount(std::int32_t count);

std::uint32_t SelectNpcRouteMovementSlot();

void LoadNPCFiles(NpcRouteSpline *routes);

std::int32_t MatchNpcAreaSelector(shen_name selector, shen_name area);

void LoadNpcMotionClipFile(const char *filename, std::int32_t slot, std::uint16_t fallbackMotion);

std::int32_t ReadNPCArea(shen_name area, std::int32_t loadResources, const char *filename);

std::int32_t GetNpcSavedDateTimeFlag();

void SetNpcSavedDateTimeFlag();

std::int32_t ClearNpcSavedDateTimeFlag();

void SaveNpcDateTime();

bool IsNpcScheduleSeedName(shen_name name);

NpcRouteStateContextPrefix *FindNpcRouteStateContext(shen_name name);

std::int32_t ReleaseNpcScriptRouteSlot(shen_name name);

std::int32_t CreateScriptNpcRouteSlot(shen_name name);

std::int32_t ReleaseNpcScriptRouteSlot(shen_name name);

std::int32_t PrepareNpcAreaRouteStore(std::int32_t count);

std::int32_t RegisterNpcAreaRouteState(NpcAreaRouteState *state, std::int32_t point);

std::int32_t ComputeNpcAreaPointYaw(const NpcRouteSpline *routes, std::int32_t point);

void SetNpcScheduleSeedName(shen_name value, shen_name expected, std::int32_t index);

extern std::int32_t npcPlayerSearchRequested;

extern DateTime npcCurrentDateTime;

Character *npcRouteCharacter;

MotionWork *npcRouteMotionWork;

NpcRouteLiveEntry *npcRouteOwner;

extern std::uint32_t npcRouteBuildCount;

Vector3 npcRouteTarget = {};

NpcRouteLiveEntry *npcSceneRouteOwner;

void SetNpcScheduleSeedName(shen_name value, shen_name expected, std::int32_t index);

namespace {

std::int16_t npcRouteForwardPath[72], npcRouteBackwardPath[72] = {};

std::int8_t npcRouteForwardConnections[72] = {}, npcRouteBackwardConnections[72] = {};

const NpcRouteSpline *npcRouteSearchTable = 0;

float *npcRouteSearchCost;

std::int16_t *npcRouteSearchPath;

std::int8_t *npcRouteSearchConnections;

std::int32_t npcRouteSearchCount = 0, npcRouteSearchPointCount = 0, npcRouteSearchDepth = 0;

std::uint8_t npcRouteSearchVisited[256];

std::uint8_t npcAreaRestrictionQueryFlag;

} // namespace

NpcRouteStateContextPrefix npcPreviewRouteContext;

shen_name npcPreviewRouteName, npcPreviewRouteArea;

std::int32_t ComputeNpcAreaPointYaw(const NpcRouteSpline *routes, std::int32_t point);

NpcRouteAvoidanceCandidate npcRouteAvoidanceCandidates[5];

NpcRouteAvoidanceCandidate *npcRouteAvoidanceFirst = 0, *npcRouteAvoidanceLast = 0;

extern Vector3 npcPlayerPosition, npcPlayerCenter;

extern float npcPlayerDisplacement, npcPlayerRadius;

namespace {

static const std::uint16_t npcTransitionMotions[6][3] = {
    {0x9906, 0x9908, 0}, {0x9907, 0x9905, 0}, {0x9903, 0x9904, 0}, {0x9902, 0x9901, 0}, {0xa001, 0xa002, 0}, {0xa004, 0xa005, 0}};

static const NpcTransitionEntry npcTransitionEntries[22] = {
    {SHEN_NAME4('A', 'R', '0', '2'), SHEN_NAME4('D', 'O', 'G', '0'), {233.100006f, 100.220001f, 434.390015f}, 0x0, 0, npcTransitionMotions[1]},
    {SHEN_NAME4('A', 'R', '0', '2'), SHEN_NAME4('C', 'A', 'T', '0'), {246.470001f, 101.120003f, 356.429993f}, 0xe38e, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('A', 'R', '0', '2'), SHEN_NAME4('C', 'A', 'T', '1'), {248.000000f, 101.120003f, 357.380005f}, 0x3048, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('A', 'R', '0', '2'), SHEN_NAME4('C', 'A', 'T', '2'), {247.389999f, 101.430000f, 357.359985f}, 0xfe93, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('A', 'R', '0', '3'), SHEN_NAME4('C', 'A', 'T', '3'), {288.079987f, 100.660004f, 457.500000f}, 0x1555, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('W', 'S', '0', '0'), SHEN_NAME4('D', 'O', 'G', '1'), {42.1199989f, 98.3000031f, 114.750000f}, 0x45b0, 0, npcTransitionMotions[0]},
    {SHEN_NAME4('W', 'S', '0', '0'), SHEN_NAME4('C', 'A', 'T', '4'), {66.4000015f, 101.180000f, 124.379997f}, 0x2fa4, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('W', 'S', '0', '0'), SHEN_NAME4('C', 'A', 'T', '0'), {66.1900024f, 101.180000f, 124.019997f}, 0xbe81, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('W', 'S', 'Y', '0'), SHEN_NAME4('C', 'A', 'T', '4'), {66.4000015f, 101.180000f, 124.379997f}, 0x2fa4, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('W', 'S', 'Y', '0'), SHEN_NAME4('C', 'A', 'T', '0'), {66.1900024f, 101.180000f, 124.019997f}, 0xbe81, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('W', 'K', '0', '0'), SHEN_NAME4('D', 'O', 'G', '2'), {100.500000f, 102.250000f, 289.679993f}, 0x7ddd, 0, npcTransitionMotions[1]},
    {SHEN_NAME4('W', 'K', '0', '0'), SHEN_NAME4('C', 'A', 'T', '4'), {120.080002f, 102.500000f, 321.779999f}, 0xe9f4, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('W', 'K', '0', '0'), SHEN_NAME4('C', 'A', 'T', '0'), {121.260002f, 102.269997f, 244.539993f}, 0x182d, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('W', 'K', '0', '0'), SHEN_NAME4('C', 'A', 'T', '1'), {135.250000f, 102.269997f, 289.799988f}, 0xe38e, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('W', 'T', '0', '0'), SHEN_NAME4('D', 'O', 'G', '3'), {243.179993f, 113.860001f, 254.380005f}, 0xa0b6, 0, npcTransitionMotions[1]},
    {SHEN_NAME4('W', 'T', '0', '0'), SHEN_NAME4('C', 'A', 'T', '2'), {249.559998f, 115.860001f, 340.559998f}, 0xa16c, 0, npcTransitionMotions[2]},
    {SHEN_NAME4('W', 'T', '0', '0'), SHEN_NAME4('C', 'A', 'T', '3'), {250.360001f, 115.860001f, 339.989990f}, 0x5111, 0, npcTransitionMotions[3]},
    {SHEN_NAME4('K', 'S', 'H', '1'), SHEN_NAME4('U', 'S', 'I', '0'), {161.550003f, 100.000000f, 219.910004f}, 0x3555, 0, npcTransitionMotions[4]},
    {SHEN_NAME4('K', 'S', 'H', '1'), SHEN_NAME4('U', 'S', 'I', '1'), {160.360001f, 100.000000f, 215.600006f}, 0x993e, 0, npcTransitionMotions[4]},
    {SHEN_NAME4('K', 'S', 'H', '1'), SHEN_NAME4('N', 'I', 'W', '0'), {147.160004f, 100.000000f, 193.940002f}, 0x18bf, 0, 0},
    {SHEN_NAME4('K', 'S', 'H', '1'), SHEN_NAME4('N', 'I', 'W', '1'), {146.479996f, 100.000000f, 191.419998f}, 0x6e14, 0, 0},
    {0, 0, {0.0f, 0.0f, 0.0f}, 0, 0, 0},
};

} // namespace

std::int32_t GetNpcRunCapability(Character *character);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcRouteSceneRotation(NpcRouteStateContextPrefix *context, shen_name name, const Vector3i *rotation) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    context->savedEuler.x = rotation->x;
    context->savedEuler.y = rotation->y;
    context->savedEuler.z = rotation->z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void InitializeQuickIconSprite() {
    quickIconPosition.x = -176.0f;
    quickIconPosition.y = 240.0f;
    quickIconPosition.velocity = 1.0f;
    quickIconPosition.reservedScale = 0.0f;
    quickIconPosition.selectedDirection = 0;
    quickIconPosition.depth = 3.0f;
    g_quickIconSprite.icon = 0x700;
    g_quickIconSprite.flags = 0;
    g_quickIconSprite.x = quickIconPosition.x;
    g_quickIconSprite.y = quickIconPosition.y;
    g_quickIconSprite.width = 64;
    g_quickIconSprite.height = 64;
    g_quickIconSprite.scaleX = 1.0f;
    g_quickIconSprite.scaleY = 1.0f;
    g_quickIconSprite.rotation = 0;
    g_quickIconSprite.depth = 3.0f;
    g_quickIconSprite.alignment = 5;
    g_quickIconPositionFlags = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t npcGetCanInteract(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    const NpcRouteStateContextPrefix *const context = &entry->stateContext;
    if (!entry)
        return 0;
    return context->flags & 1U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t IsNpcRouteReady(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    const NpcRouteStateContextPrefix *const context = &entry->stateContext;
    if (entry && (context->flags & 3U) == 3U)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool IsNpcRouteContextMoving(const NpcRouteStateContextPrefix *context) {
    if (!(context->flags & 3U) || (context->flags & 0x1000U))
        return false;
    if (static_cast<char>(context->phase) == 7) {
        if (static_cast<char>(context->avoidancePhase) == 5)
            return false;
        if (static_cast<char>(context->avoidanceResult) == 2)
            return false;
    } else if (static_cast<char>(context->moveState) == 3)
        return false;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcLocomotionScale(NpcRouteStateContextPrefix *context, shen_name name, float scale) {
    NpcRouteLiveEntry *entry;
    if (!context) {
        entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    } else {
        entry = context->owner;
    }
    context->motionStep /= context->motionScale;
    scale *= GetNpcMotionRigScale(entry->node);
    if (scale > 3.99f)
        scale = 3.99f;
    else if (scale < 0.5f)
        scale = 0.5f;
    context->motionScale = scale;
    context->motionStep *= scale;
    if (entry->node)
        mtwkSetSpeed(context->motionWork, context->motionScale, LowerBody);
}

#else

namespace npc_package_detail {

void InitializeNpcRouteResetGroups(NpcRouteResetGroupHeader *groups) {
    npcRouteResetGroup = groups;
    for (NpcRouteResetGroupHeader *group = groups; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name != npcLoadedArea.name)
                continue;
            entry->nodeOffset = 0;
            const std::int32_t pointCount = GetNpcRouteNodePointCount(entry->routeNode);
            const Vector3 *const points = GetNpcRouteNodeSplinePoints(entry->routeNode);
            InterpolateNpcSplineNormalized(points, pointCount, entry->parameter, &entry->position);
            entry->previousPosition = entry->position;
            entry->parameter = static_cast<std::int32_t>(static_cast<std::uint32_t>(entry->parameter) + entry->parameterStep);
            const std::int32_t nextPointCount = GetNpcRouteNodePointCount(entry->routeNode);
            const Vector3 *const nextPoints = GetNpcRouteNodeSplinePoints(entry->routeNode);
            InterpolateNpcSplineNormalized(nextPoints, nextPointCount, entry->parameter, &entry->position);
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
}

void ResetNpcTransitions() {
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        slot.character = {};
        slot.motionCursor = 0;
        slot.node = nullptr;
    }
    std::int8_t motionCacheIndex = 0;
    NpcTransitionRuntimeSlot *slot = npcTransitionRuntimeSlots.data();
    for (const NpcTransitionEntry &entry : npcTransitionEntries) {
        if (!entry.area.name)
            break;
        if (entry.area.name != g_currentMap.name)
            continue;
        slot->character = entry.character;
        slot->position = entry.position;
        slot->yaw = entry.yaw;
        slot->activationMode = entry.activationMode;
        slot->controllerToggle = entry.controllerToggle;
        slot->motions = entry.motions;
        if (entry.motions)
            slot->motionCacheIndex = motionCacheIndex++;
        ++slot;
    }
}
// @verified - p
void ResetNpcRoutes(NpcRouteResetGroupHeader *groups) {
    npcDrawActive = false;
    InitializeNpcRouteResetGroups(groups);
    ResetNpcTransitions();
}

void MeasureNpcRouteClearance(const Vector3 &start, const Vector3 &end, float &left, float &right, std::uint32_t moveFlags, std::int32_t step) {
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    if (std::abs(dx) < 0.001f && std::abs(dz) < 0.001f)
        return;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-dx, -dz) - 0x4000);
    const float sine = sin_fixed16(yaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw))));
    std::int32_t leftSteps = static_cast<std::int32_t>(std::abs(left * 10.0f));
    std::int32_t rightSteps = static_cast<std::int32_t>(std::abs(right * 10.0f));
    for (std::int32_t index = step; index <= leftSteps || index <= rightSteps; index += step) {
        const float distance = static_cast<float>(index) * 0.1f;
        const float x = distance * sine;
        const float z = distance * cosine;
        Vector3 resolved;
        if (index <= rightSteps) {
            Vector3 point{start.x - x - npcSchedule.areaOrigin.x, start.y, start.z - z - npcSchedule.areaOrigin.z};
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.30000001f;
            if (RaycastInWorldGrid(resolved, point, 0.40000001f))
                rightSteps = index - step;
        }
        if (index <= leftSteps) {
            Vector3 point{start.x + x - npcSchedule.areaOrigin.x, start.y, start.z + z - npcSchedule.areaOrigin.z};
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.30000001f;
            if (RaycastInWorldGrid(resolved, point, 0.40000001f))
                leftSteps = index - step;
        }
    }
    left = static_cast<float>(leftSteps) / 10.0f;
    right = static_cast<float>(rightSteps) / 10.0f;
}

void RebuildNpcRouteGround() {
    auto *const table = reinterpret_cast<NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.routeNodeCount; ++index) {
        NpcRouteNode &node = table[index];
        auto *const points = reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(table) + node.pointsOffset);
        const std::uint32_t flags = floorFlags | (node.flags & 0xffU);
        for (std::uint32_t point = 0; point < node.pointCount - 2; ++point) {
            Vector3 sampled;
            SampleNpcRouteNode(static_cast<std::uint16_t>(index), static_cast<float>(point), sampled);
            sampled.x -= npcSchedule.areaOrigin.x;
            sampled.z -= npcSchedule.areaOrigin.z;
            FindNpcIntersectionInScene(sampled, flags);
            points[point + 1].y = sampled.y;
        }
        points[0].y = points[1].y;
        points[node.pointCount - 1].y = points[node.pointCount - 2].y;
    }
}

std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary) {
    const std::int32_t clearanceStep = sampleStep == 1 ? 1 : 4;
    std::uint32_t presentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    Vector3 previous{};
    SampleNpcSplineWindow(points, pointCount, 0.0f, &previous);
    const std::int32_t segments = pointCount - 3;
    std::memset(auxiliary, 0, segments);
    std::int32_t parameter = 0;
    if (segments <= 0)
        return 1;
    do {
        float left = 3.0f, right = 3.0f;
        std::int32_t steps = 0;
        for (;;) {
            Vector3 next{};
            const std::int32_t nextParameter = parameter + sampleStep;
            if (!SampleNpcSplineWindow(points, pointCount, static_cast<float>(nextParameter) * 0.015625f, &next)) {
                previous = next;
                parameter = nextParameter;
                break;
            }
            const std::int32_t segment = parameter / 64;
            if (!auxiliary[segment]) {
                Vector3 ground = previous;
                ground.x -= npcSchedule.areaOrigin.x;
                ground.z -= npcSchedule.areaOrigin.z;
                FindNpcIntersectionInScene(ground, moveFlags);
                ground.y -= 0.22499999f;
                std::int32_t yaw{};
                if (flddQueryStairOrigin(ground, ground, yaw))
                    auxiliary[segment] = 1;
            }
            MeasureNpcRouteClearance(previous, next, left, right, moveFlags, clearanceStep);
            previous = next;
            const std::uint32_t currentTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
            if (IsLoadingDrawActive()) {
                const float elapsed = static_cast<float>(std::bit_cast<std::int32_t>(presentedTime - currentTime));
                if (static_cast<double>(elapsed) / 12.5 > 15000.0) {
                    DrawLoadingModes();
                    FlushQueuedStripType3Batches();
                    (void)ProfileFramePerformance();
                    presentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
                }
            }
            steps += sampleStep;
            parameter = nextParameter;
            if (steps >= 64)
                break;
        }
        *bounds++ = left;
        *bounds++ = right;
    } while (parameter / 64 < segments);
    return 1;
}

void RebuildNpcRouteLanes(std::int32_t sampleStep) {
    auto *const table = reinterpret_cast<NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.routeNodeCount; ++index) {
        NpcRouteNode &node = table[index];
        const auto *const points = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(table) + node.pointsOffset);
        auto *const bounds = reinterpret_cast<float *>(reinterpret_cast<std::byte *>(table) + node.laneBoundsOffset);
        auto *const auxiliary = reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::byte *>(table) + node.auxiliaryOffset);
        const std::uint32_t flags = floorFlags | (node.flags & 0xffU);
        Vector3 point = points[1];
        point.x -= npcSchedule.areaOrigin.x;
        point.z -= npcSchedule.areaOrigin.z;
        bool hasGround = FindNpcIntersectionInScene(point, flags);
        if (!hasGround) {
            point = points[node.pointCount - 2];
            point.x -= npcSchedule.areaOrigin.x;
            point.z -= npcSchedule.areaOrigin.z;
            hasGround = FindNpcIntersectionInScene(point, flags);
        }
        const std::int32_t segments = static_cast<std::int32_t>(node.pointCount) - 3;
        if (!hasGround) {
            std::fill_n(bounds, segments * 2, 0.40000001f);
            continue;
        }
        CheckNpcRouteIntersections(points, static_cast<std::int32_t>(node.pointCount), bounds, sampleStep, flags, auxiliary);
    }
}

void RebuildNpcAreaPointGround() {
    auto *const table = reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        std::byte *const record = npcSchedule.bytes.data() + npcSchedule.areaPointTableOffset + index * 0x44;
        const std::int16_t node = static_cast<std::int16_t>(ReadU16(record + 4));
        if (node < 0)
            continue;
        Vector3 point;
        std::memcpy(&point, record + 0x34, sizeof(point));
        point.x -= npcSchedule.areaOrigin.x;
        point.z -= npcSchedule.areaOrigin.z;
        FindNpcIntersectionInScene(point, floorFlags | (table[node].flags & 0xffU));
        std::memcpy(record + 0x38, &point.y, sizeof(point.y));
    }
}

std::int32_t NpcRouteYaw(float deltaX, float deltaZ) {
    if (std::abs(deltaX) < 0.001f && std::abs(deltaZ) < 0.001f)
        return 0;
    return static_cast<std::int16_t>(FixedAtan2(-deltaX, -deltaZ));
}

void SetNpcYawBits(NpcRouteLiveEntry &entry, std::int32_t yaw) {
    float packed{};
    std::memcpy(&packed, &yaw, sizeof(packed));
    WriteNpcLiveField(entry, npcYawOffset, packed);
}

bool SetNpcAreaPointTangentYaw(NpcRouteLiveEntry &entry, std::uint16_t point) {
    const std::byte *const areaPoint = GetNpcAreaPoint(point);
    if (!areaPoint)
        return false;
    const auto routeNode = static_cast<std::int16_t>(ReadU16(areaPoint + 4));
    if (routeNode < 0)
        return false;
    Vector3 sample;
    SampleNpcSplineWindow(GetNpcRouteNodeSplinePoints(routeNode), GetNpcRouteNodePointCount(routeNode),
        static_cast<float>(static_cast<std::int16_t>(ReadU16(areaPoint + 8))) * 0.015625f, &sample);
    const Vector3 pointPosition{ReadF32(areaPoint + 0x34), ReadF32(areaPoint + 0x38), ReadF32(areaPoint + 0x3c)};
    SetNpcYawBits(entry, NpcRouteYaw(sample.x - pointPosition.x, sample.z - pointPosition.z));
    return true;
}

const std::byte *ResolveNpcRouteData(const NpcRouteStateContext &context, std::int32_t offset) {
    return reinterpret_cast<const std::byte *>(context.stateTable) + offset;
}

bool SearchNpcRouteNodes(
    NpcRouteStateContext &context, NpcRouteSearch &search, std::int16_t state, std::int16_t target, const Vector3 &targetPosition, std::int32_t depth) {
    if (depth == 71)
        return false;
    const std::uint16_t nodeIndex = state & 0x7fff;
    const NpcRouteNode &node = context.stateTable[nodeIndex];
    if (nodeIndex == (target & 0x7fff)) {
        search.nodes[depth] = state;
        search.nodes[depth + 1] = -1;
        search.count = depth + 1;
        search.pointCount = static_cast<std::int32_t>(node.pointCount);
        search.length = ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
        return true;
    }
    npcSchedule.routeVisited[nodeIndex] = 1;
    std::array<std::int16_t, 4> candidates{};
    std::array<std::int8_t, 4> slots{};
    std::array<float, 4> distances{};
    std::int32_t count = 0;
    const std::int32_t first = state < 0 ? 4 : 0;
    for (std::int32_t slot = first; slot < first + 4; ++slot) {
        const std::int16_t next = node.connections[slot];
        const std::uint16_t nextIndex = next & 0x7fff;
        if (next == -1 || nextIndex >= npcSchedule.routeNodeCount)
            continue;
        const std::uint32_t flags = context.stateTable[nextIndex].flags;
        if ((flags & 0x200) || ((flags & 0x100) && !(context.flags & 0x300)) || npcSchedule.routeVisited[nextIndex])
            continue;
        candidates[count] = next;
        slots[count++] = static_cast<std::int8_t>(slot);
    }
    for (std::int32_t index = 0; index < count; ++index) {
        if ((candidates[index] & 0x7fff) != (target & 0x7fff))
            continue;
        if (SearchNpcRouteNodes(context, search, candidates[index], target, targetPosition, depth + 1)) {
            search.nodes[depth] = state;
            search.connections[depth] = slots[index];
            search.pointCount += static_cast<std::int32_t>(node.pointCount) - 2;
            search.length += ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
            return true;
        }
    }
    for (std::int32_t index = 0; index < count; ++index) {
        const NpcRouteNode &next = context.stateTable[candidates[index] & 0x7fff];
        const std::int32_t point = candidates[index] < 0 ? 1 : static_cast<std::int32_t>(next.pointCount) - 2;
        Vector3 position;
        std::memcpy(&position, ResolveNpcRouteData(context, next.pointsOffset) + point * sizeof(Vector3), sizeof(position));
        const float dx = position.x - targetPosition.x;
        const float dz = position.z - targetPosition.z;
        distances[index] = dz * dz + dx * dx;
    }
    std::array<std::int16_t, 4> ordered{};
    std::array<std::int8_t, 4> orderedSlots{};
    for (std::int32_t index = 0; index < count; ++index) {
        std::int32_t rank = 0;
        for (std::int32_t other = 0; other < count; ++other) {
            if (distances[index] > distances[other] || (distances[index] == distances[other] && other < index))
                ++rank;
        }
        ordered[rank] = candidates[index];
        orderedSlots[rank] = slots[index];
    }
    for (std::int32_t index = 0; index < count; ++index) {
        if (npcSchedule.routeVisited[ordered[index] & 0x7fff])
            continue;
        if (!SearchNpcRouteNodes(context, search, ordered[index], target, targetPosition, depth + 1))
            continue;
        search.nodes[depth] = state;
        search.connections[depth] = orderedSlots[index];
        search.pointCount += static_cast<std::int32_t>(node.pointCount) - 2;
        search.length += ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
        return true;
    }
    return false;
}

bool BuildNpcRouteNodePath(NpcRouteStateContext &context) {
    if ((context.startNode & 0x7fff) == (context.endNode & 0x7fff)) {
        context.routeNodes[0] = context.startNode;
        context.routeNodes[1] = -1;
        context.routeCount = 1;
        context.pointCount = static_cast<std::int16_t>(context.stateTable[context.startNode & 0x7fff].pointCount);
        return true;
    }
    const std::byte *const target = GetNpcAreaPoint(context.owner->pendingAreaPoint);
    Vector3 targetPosition;
    std::memcpy(&targetPosition, target + 0x34, sizeof(targetPosition));
    NpcRouteSearch forward;
    NpcRouteSearch backward;
    std::fill(npcSchedule.routeVisited.begin(), npcSchedule.routeVisited.end(), 0);
    const bool hasForward = SearchNpcRouteNodes(context, forward, context.startNode, context.endNode, targetPosition, 0);
    std::fill(npcSchedule.routeVisited.begin(), npcSchedule.routeVisited.end(), 0);
    const bool hasBackward = SearchNpcRouteNodes(context, backward, static_cast<std::int16_t>(context.startNode ^ 0x8000), context.endNode, targetPosition, 0);
    if (!hasForward && !hasBackward)
        return false;
    const NpcRouteSearch &selected = hasForward && (!hasBackward || forward.length <= backward.length) ? forward : backward;
    std::memcpy(context.routeNodes.data(), selected.nodes.data(), (selected.count + 1) * sizeof(std::int16_t));
    std::memcpy(context.routeConnections.data(), selected.connections.data(), selected.count + 1);
    context.routeCount = static_cast<std::uint8_t>(selected.count);
    context.pointCount = static_cast<std::int16_t>(selected.pointCount);
    return true;
}

std::uint16_t GetNpcRouteConnectionParameter(const std::byte *areaPoint, std::uint16_t state, std::uint16_t adjacentState, bool target) {
    const bool reversed = (state & 0x8000U) != 0;
    const std::byte *const routeNode = GetNpcRouteNode(state & 0x7fffU);
    if (!areaPoint || !routeNode)
        return 0;
    const std::size_t groupBase = target ? (reversed ? 15 : 10) : (reversed ? 5 : 0);
    const std::size_t connectionBase = target ? (reversed ? 0 : 4) : (reversed ? 4 : 0);
    const std::uint16_t sought = target ? static_cast<std::uint16_t>(adjacentState ^ 0x8000U) : adjacentState;
    std::size_t slot = 0;
    if (adjacentState != 0xffff) {
        do {
            ++slot;
            if (ReadU16(routeNode + 0x0c + (connectionBase + slot - 1) * 2) == sought)
                break;
        } while (slot < 4);
    }
    return ReadU16(areaPoint + 8 + (groupBase + slot) * 2);
}

// @verified - p
std::int32_t SampleNpcRouteWindow(NpcRouteStateContext &context, Vector3 *position, bool maintainWindow) {
    const std::uint16_t parameter = context.parameter;
    float amount = static_cast<float>(parameter) * 0.015625f;
    std::int32_t window = static_cast<std::int32_t>(amount);
    std::int32_t valid = 1;
    if (window > context.pointCount - 4) {
        amount = 0.99998999f;
        window = context.pointCount - 4;
        valid = 0;
    }
    if (maintainWindow && context.sampledWindow == window) {
        SampleNpcSplineWindow(context.spline.data(), 4, -amount, position);
        return valid;
    }
    const std::int16_t *route = context.routeNodes.data();
    if (maintainWindow && parameter >= context.sampledParameter) {
        route += context.sampledRouteIndex;
    } else {
        context.sampledPointBase = 0;
        context.sampledRouteIndex = 0;
    }
    std::int32_t base = context.sampledPointBase;
    std::int32_t length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
    context.sampledParameter = parameter;
    context.sampledWindow = static_cast<std::int16_t>(window);
    std::array<std::int16_t, 4> nodes{};
    std::array<std::int16_t, 4> points{};
    std::int32_t count = 0;
    for (; count < 4; ++count) {
        while (count + window > length + base) {
            if (*++route == -1)
                break;
            base += length;
            if (count < 2) {
                ++context.sampledRouteIndex;
                context.sampledPointBase = static_cast<std::int16_t>(base);
            }
            length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
        }
        if (*route == -1)
            break;
        nodes[count] = *route;
        points[count] = static_cast<std::int16_t>(window + count - base);
    }
    if (count == 2) {
        const NpcRouteNode &node = context.stateTable[nodes[1] & 0x7fff];
        const std::int32_t index = nodes[1] < 0 ? 1 : static_cast<std::int32_t>(node.pointCount) - 2;
        std::memcpy(position, ResolveNpcRouteData(context, node.pointsOffset) + index * sizeof(Vector3), sizeof(*position));
        return 0;
    }
    if (count == 3) {
        nodes[3] = nodes[2];
        points[3] = points[2] + 1;
    }
    for (std::int32_t index = 0; index < 4; ++index) {
        const NpcRouteNode &node = context.stateTable[nodes[index] & 0x7fff];
        const std::int32_t point = nodes[index] < 0 ? static_cast<std::int32_t>(node.pointCount) - points[index] - 1 : points[index];
        std::memcpy(&context.spline[index], ResolveNpcRouteData(context, node.pointsOffset) + point * sizeof(Vector3), sizeof(Vector3));
    }
    SampleNpcSplineWindow(context.spline.data(), 4, -amount, position);
    return valid;
}

void UpdateNpcRouteWindow(NpcRouteStateContext &context, std::int32_t parameter, bool maintainWindow) {
    std::int32_t window = parameter / 64;
    if (window > context.pointCount - 4)
        window = context.pointCount - 4;
    const std::int16_t *route = context.routeNodes.data();
    if (maintainWindow) {
        if (window == context.activeWindow)
            return;

        route += context.activeRouteIndex;
    } else {
        context.crossingPoint = 0;
        context.activeWindow = context.activePointBase = 0;
        context.activeRouteIndex = 0;
        context.activeNode = route[0];
        context.nextNode = route[1];
    }
    std::int32_t base = context.activePointBase;
    std::int32_t length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
    for (std::int32_t point = 0; point < 4; ++point) {
        while (point + window > length + base) {
            if (*++route == -1)
                break;
            base += length;
            if (point < 2) {
                ++context.activeRouteIndex;
                context.activePointBase = static_cast<std::int16_t>(base);
            }
            context.crossingPoint = static_cast<std::uint8_t>(point);
            length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
        }
        if (*route == -1)
            break;
        if (point == 1) {
            context.activeNode = *route;
            context.nextNode = route[1];
            context.activeSegment = static_cast<std::int16_t>(*route < 0 ? length - window + base - 2 : window - base);
        }
    }
    context.activeWindow = static_cast<std::int16_t>(window);
    context.moveFlags = (context.moveFlags & 0x1ff00U) | (context.stateTable[context.activeNode & 0x7fff].flags & 0xffU);
}

float ReadNpcRouteSegmentLength(const NpcRouteStateContext &context) {
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    if (context.crossingPoint == 2)
        return node.connectionLengths[context.routeConnections[context.activeRouteIndex]];

    return ReadF32(ResolveNpcRouteData(context, node.lengthsOffset) + (context.activeSegment + 1) * sizeof(float));
}

std::int32_t ReadNpcEulerYaw(const Vector3 &euler) {
    std::int32_t yaw;
    std::memcpy(&yaw, &euler.y, sizeof(yaw));
    return yaw;
}

void StoreNpcEulerYaw(Vector3 &euler, std::int32_t yaw) {
    std::memcpy(&euler.y, &yaw, sizeof(yaw));
}

Vector3 TransformNpcPoint(const Vector3 &point) {
    const Matrix4x4 &matrix = workTransformMatrix;
    return {point.x * matrix.rows[0].x + point.y * matrix.rows[1].x + point.z * matrix.rows[2].x + matrix.rows[3].x,
        point.x * matrix.rows[0].y + point.y * matrix.rows[1].y + point.z * matrix.rows[2].y + matrix.rows[3].y,
        point.x * matrix.rows[0].z + point.y * matrix.rows[1].z + point.z * matrix.rows[2].z + matrix.rows[3].z};
}

Vector3 NpcPositionInFrame(const Vector3 &point, const Vector3 &origin, std::int32_t yaw) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(yaw), 0, 0);
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    const Vector3 result = TransformNpcPoint(point);
    Matrix4x4::Pop(1);
    return result;
}

Vector3 OffsetNpcRoutePosition(const Vector3 &origin, std::int32_t yaw, float lateral) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::SetViewMatrixFromEuler(yaw, 0, 0);
    Matrix4x4::preTranslate(lateral, 0.0f, 0.0f);
    const Vector3 result{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return result;
}

Vector3 NpcAreaPosition(Vector3 position) {
    position.x += npcSchedule.areaOrigin.x;
    position.z += npcSchedule.areaOrigin.z;
    return position;
}

std::uint16_t SelectNpcLocomotion(const NpcRouteStateContext &context, std::uint32_t selector) {
    constexpr std::array<std::array<std::uint16_t, 15>, 4> motions{
        {{0xf03e, 0xf0ff, 0xf07e, 0xf084, 0xf09e, 0xf0d1, 0xf060, 0xf086, 0xf005, 0xf078, 0xf058, 0xf084, 0xf084, 0xf0a4, 0xf0a6},
            {0xf006, 0xf006, 0xf006, 0xf0e6, 0xf0e6, 0xf0e6, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006},
            {0xf056, 0xf056, 0xf083, 0xf056, 0xf0a3, 0xf0e9, 0xf077, 0xf09d, 0x8024, 0xf07d, 0xf05d, 0x8024, 0x8024, 0xf056, 0xf0f4},
            {0xf040, 0xf040, 0xf080, 0xf040, 0xf0a0, 0xf0d3, 0xf062, 0xf088, 0x8020, 0xf07a, 0xf05a, 0x8020, 0x8020, 0xf040, 0xf0f3}}};
    constexpr std::array<std::uint16_t, 4> defaults{0xf084, 0xf006, 0x8024, 0x8020};
    const std::int32_t rig = mtwkGetRigBoneId(context.motionWork);
    return rig >= 0 && rig < 15 ? motions[selector][rig] : defaults[selector];
}

std::uint16_t ComputeNpcRouteStride(NpcRouteStateContext &context) {
    if (context.routeFlags & 0x400000U)
        return 1;

    if (!context.owner->node)
        return 16;

    context.segmentLength = ReadNpcRouteSegmentLength(context);
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(std::ceil(context.motionStep * 64.0f / context.segmentLength)));
}

void InitializeNpcLocomotionProfile(NpcRouteStateContext &context) {
    if (context.flags & 0x40U)
        return;

    const bool running = (context.flags & 0x10U) != 0;
    context.maximumYawStep = running ? 1820 : 546;
    context.motionStep = running ? 0.1f : 0.039999999f;
    if (context.owner->node) {
        const std::uint16_t motion = SelectNpcLocomotion(context, running ? 1 : 0);
        const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
        if (sequence) {
            Vector3 displacement{};
            SampleNativeMotionRoot(motion, GetCharacterBoneId(context.owner->node), -1.0f, displacement);
            if (-displacement.z > 0.00000011920929f)
                context.motionStep = -(displacement.z / static_cast<float>(sequence->frameCount));
        }
        context.flags |= 0x40U;
    }
    context.motionStep *= context.motionScale;
    if (context.flags & 2U)
        context.stride = ComputeNpcRouteStride(context);
}

void SetNpcLocomotionScale(NpcRouteStateContext &context, float scale) {
    constexpr std::array<std::uint8_t, 16> scales{11, 11, 11, 10, 10, 11, 12, 12, 12, 12, 11, 10, 10, 11, 11, 10};
    float multiplier = 1.0f;
    if (context.owner->node) {
        const std::int32_t rig = mtwkGetRigBoneId(context.motionWork);
        if (rig >= 0 && rig <= 15)
            multiplier = static_cast<float>(scales[rig]) / 10.0f;
    }
    context.motionStep /= context.motionScale;
    const float requested = scale * multiplier;
    context.motionScale = requested <= 3.99f ? std::fmax(0.5f, requested) : 3.99f;
    context.motionStep *= context.motionScale;
    if (context.flags & 2U)
        context.stride = ComputeNpcRouteStride(context);
    if (context.owner->node)
        mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
}

void InterpolateNpcRouteLanes(NpcRouteStateContext &context, bool advanced) {
    float fraction = static_cast<float>(context.parameter & 63U) * 0.015625f;
    if (advanced && std::abs(fraction) < 0.00000011920929f && context.parameter)
        fraction = 1.0f;

    const float highDelta = context.nextLaneHigh - context.segmentLaneHigh;
    context.interpolatedLaneHigh = -context.segmentLength > highDelta ? context.nextLaneHigh :
        highDelta >= 0.0f                                             ? context.segmentLaneHigh :
                                                                        highDelta * fraction + context.segmentLaneHigh;
    const float lowDelta = context.nextLaneLow - context.segmentLaneLow;
    context.interpolatedLaneLow = lowDelta > context.segmentLength ? context.nextLaneLow :
        lowDelta <= 0.0f                                           ? context.segmentLaneLow :
                                                                     lowDelta * fraction + context.segmentLaneLow;
    context.laneHigh = context.interpolatedLaneHigh - 0.1f;
    context.laneLow = context.interpolatedLaneLow + 0.1f;
    if (context.laneLow > context.laneHigh) {
        if (context.laneHigh < 0.0f && context.laneLow < 0.0f)
            context.laneHigh = context.laneLow;
        else if (context.laneLow > 0.0f && context.laneHigh > 0.0f)
            context.laneLow = context.laneHigh;
        else
            context.laneHigh = context.laneLow = 0.0f;
    }
    if (context.laneOffset > context.laneHigh)
        context.laneOffset = context.laneHigh;
    else if (context.laneLow > context.laneOffset)
        context.laneOffset = context.laneLow;
}

void LoadNpcRouteLanes(NpcRouteStateContext &context) {
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    const bool reverse = context.activeNode < 0;
    std::int32_t segment = context.crossingPoint == 2 ? (reverse ? 0 : static_cast<std::int32_t>(node.pointCount) - 4) : context.activeSegment;
    context.stairFlags = static_cast<std::uint8_t>(ResolveNpcRouteData(context, node.auxiliaryOffset)[segment]);
    const std::byte *lanes = ResolveNpcRouteData(context, node.laneBoundsOffset) + segment * 8;
    context.segmentLaneHigh = ReadF32(lanes + (reverse ? 0 : 4));
    context.segmentLaneLow = -ReadF32(lanes + (reverse ? 4 : 0));
    const NpcRouteNode *next = &node;
    segment += reverse ? -1 : 1;
    bool nextReverse = reverse;
    if (context.crossingPoint == 2 || context.crossingPoint == 3) {
        next = &context.stateTable[context.nextNode & 0x7fff];
        nextReverse = context.nextNode < 0;
        segment = nextReverse ? static_cast<std::int32_t>(next->pointCount) - 4 : 0;
    } else if (segment < 0 || segment >= static_cast<std::int32_t>(node.pointCount) - 3) {
        context.nextLaneHigh = context.segmentLaneHigh;
        context.nextLaneLow = context.segmentLaneLow;
        InterpolateNpcRouteLanes(context, false);
        return;
    }
    context.stairFlags |= static_cast<std::uint8_t>(ResolveNpcRouteData(context, next->auxiliaryOffset)[segment]);
    lanes = ResolveNpcRouteData(context, next->laneBoundsOffset) + segment * 8;
    context.nextLaneHigh = ReadF32(lanes + (nextReverse ? 0 : 4));
    context.nextLaneLow = -ReadF32(lanes + (nextReverse ? 4 : 0));
    InterpolateNpcRouteLanes(context, false);
}

bool FitNpcRouteLane(NpcRouteStateContext &context, float clearance) {
    const Vector3 offset = NpcPositionInFrame(context.position, context.localPosition, context.tangentYaw);
    const float lateral = -offset.x;
    bool clipped = false;
    float result;
    if (offset.x <= 0.0f) {
        result = context.laneHigh - clearance;
        if (lateral <= result)
            result = lateral;
        else
            clipped = true;

        if (context.laneLow > result) {
            context.laneOffset = context.laneLow;
            return true;
        }
    } else {
        result = context.laneLow + clearance;
        if (result <= lateral)
            result = lateral;
        else
            clipped = true;
        if (result > context.laneHigh) {
            result = context.laneHigh;
            clipped = true;
        }
    }
    context.laneOffset = result;
    return clipped;
}

bool AdvanceUnstreamedNpc(NpcRouteStateContext &context) {
    if ((context.flags & 0x41000U) == 0x41000U)
        return false;
    float step = context.motionStep;
    if (context.routeFlags & 0x10000U)
        step *= 8.0f;
    if (context.flags & 0x80000U) {
        context.flags &= ~0x80000U;
        context.position = context.savedSplinePosition;
        context.moveTarget = context.savedMoveTarget;
    }
    const float x = context.moveTarget.x - context.localPosition.x;
    const float z = context.moveTarget.z - context.localPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    if (distance > 100.0f)
        context.localPosition = context.moveTarget;
    const bool arrived = step > distance;
    if (arrived)
        context.localPosition = context.moveTarget;
    else if (std::abs(distance) > 0.00000011920929f) {
        context.localPosition.x += x * step / distance;
        context.localPosition.y = context.moveTarget.y;
        context.localPosition.z += z * step / distance;
    }
    if (context.routeFlags & 0x800U) {
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
    }
    return arrived;
}

bool RequestNpcRouteTurn(NpcRouteStateContext &context, const Vector3 *target, std::uint16_t yaw, std::int32_t copyType) {
    mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
    mtwkCopyTransition(context.motionWork, target, yaw, 5, copyType);
    const bool result = mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
    return (context.flags & 0x10U) ? mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0) : result;
}

void ApplyNpcRouteEuler(NpcRouteStateContext &context) {
    Vector3i rotation;
    std::memcpy(&rotation, &context.savedEuler, sizeof(rotation));
    charaSetWorldRot(context.owner->node, rotation);
}

void RestoreNpcGroundControl(NpcRouteStateContext &context) {
    if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node)) {
        std::uint32_t flags = control->flags;
        if (npcSchedule.floor == -1000 || !(context.owner->packedMoveFlags & 0xff00U))
            flags |= 2U;
        flags = (flags & ~0x804U) | 4U;
        if (context.owner->flags & 0x8000000U)
            flags |= 0x800U;
        control->flags = flags;
    }
    context.flags &= ~0x200000U;
}

std::int32_t DetectNpcRouteStair(NpcRouteStateContext &context) {
    if (!(context.routeFlags & 4U) || !context.owner->node)
        return 0;

    if (context.avoidancePhase)
        return 2;

    if (!context.stairFlags) {
        if (context.flags & 0x200000U)
            RestoreNpcGroundControl(context);
        context.flags &= 0xffbeffffU;
        return 0;
    }

    if (context.routeFlags & 0x4040U) {
        context.localPosition = NpcRouteAreaToScene(context.localPosition);
        FindNpcIntersectionInScene(context.localPosition, context.moveFlags);
        context.scenePosition = context.localPosition;
        context.localPosition = NpcAreaPosition(context.localPosition);
        context.routeFlags &= ~0x4040U;
    }

    Vector3 position = NpcRouteAreaToScene(context.localPosition);
    position.y -= 0.22499999f;
    Vector3 origin;
    std::int32_t yaw;
    const std::int32_t stairs = flddQueryStairOrigin(position, origin, yaw);
    if (!stairs) {
        if (context.flags & 0x200000U)
            RestoreNpcGroundControl(context);

        context.flags &= 0xffbeffffU;
        return 0;
    }

    const std::int32_t stairFrameYaw = yaw;
    if (context.flags & 0x10000U)
        return 0;

    position = NpcPositionInFrame(NpcRouteAreaToScene(context.localPosition), origin, yaw);
    const std::int16_t difference = static_cast<std::int16_t>(context.tangentYaw - yaw);
    const std::int32_t magnitude = difference < 0 ? -static_cast<std::int32_t>(difference) : difference;
    context.stairCount = static_cast<std::uint16_t>(stairs);
    context.stairTarget.x = context.stairTarget.y = 0.0f;
    if (magnitude >= 0x4000) {
        if (position.z > 0.0f) {
            context.flags |= 0x10000U;
            return 0;
        }

        context.stairTarget.z = 0.1f;
        std::int32_t remaining = static_cast<std::int8_t>(1 - static_cast<std::int32_t>(position.z / 0.30000001f));
        if (remaining > stairs)
            remaining = stairs;

        context.stairRemaining = static_cast<std::int8_t>(remaining);
        yaw += 0x8000;
        position.y = static_cast<float>(context.stairRemaining) * 0.22499999f;
        position.z = 0.1f - static_cast<float>(context.stairRemaining) * 0.30000001f;
        context.motion = SelectNpcLocomotion(context, 3);
        context.flags &= ~0x800U;
    } else {
        std::int32_t entered = static_cast<std::int32_t>(position.z / -0.30000001f);
        context.stairRemaining = static_cast<std::int8_t>(entered);
        if (entered & 0x80)
            context.stairRemaining = 0;
        if (position.z < 0.0f)
            ++context.stairRemaining;

        entered = context.stairRemaining;
        if (entered >= stairs) {
            context.flags |= 0x10000U;
            return 0;
        }
        context.stairRemaining = static_cast<std::int8_t>(stairs - entered);
        position.y = static_cast<float>(entered) * 0.22499999f;
        position.z = static_cast<float>(entered - 1) * -0.30000001f - 0.1f;
        context.stairTarget.z = static_cast<float>(stairs - 1) * -0.30000001f - 0.1f;
        context.motion = SelectNpcLocomotion(context, 2);
        context.flags |= 0x800U;
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::SetViewMatrixFromEuler(stairFrameYaw, 0, 0);
    context.doorPosition = TransformNpcPoint(position);
    context.stairTarget = TransformNpcPoint(context.stairTarget);
    Matrix4x4::Pop(1);
    context.stairYaw = static_cast<std::int16_t>(yaw);
    StoreNpcEulerYaw(context.savedEuler, context.stairYaw);
    context.flags |= 0x400U;
    return 1;
}

// @verified - p
std::uint16_t GetMotnId(Character *character) {
    switch (GetCharacterBoneId(character)) {
    case 0:
        return 61503;
    case 1:
        return 61535;
    case 2:
        return 61567;
    case 3:
        return 61527;
    case 4:
        return 61599;
    case 5:
        return 61650;
    case 6:
        return 61537;
    case 7:
        return 61575;
    case 8: {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        return scenGetCharaName(control->scenarioTask, character).name == SHEN_NAME(REN_) ? 41476 : 61573;
    }
    case 9:
        return 61561;
    case 10:
        return 61529;
    case 11:
        return 61608;
    case 13:
        return 61605;
    case 14:
        return 61607;
    case 15:
        return 61674;
    default:
        return 61573;
    }
}

std::int32_t TickNpcRouteStair(NpcRouteStateContext &context) {
    Character *const character = context.owner->node;
    if (!(context.routeFlags & 4U) || !character)
        return 0;

    switch (context.avoidancePhase) {
    case 0:
        if (context.flags & 0x400U) {
            if ((context.flags & 0x1000U) || (context.routeFlags & 0x800U)) {
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.motion = GetMotnId(character);
                context.avoidancePhase = 3;
            } else {
                context.stairTarget = NpcAreaPosition(context.stairTarget);
                const MotnSequence *const sequence = FindLoadedMotionSequence(context.motion);
                context.stairMotionFrames = sequence ? static_cast<float>(sequence->frameCount) : 0.0f;
                context.avoidancePhase = context.stairRemaining == 1 ? 2 : 1;
            }
            mtwkRequestMotion(context.motionWork, context.motion, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(context.motionWork, 1);
            mtwkSetFrameTime(context.motionWork, 10, MotionRegion::LowerBody);
            if (context.flags & 0x1000U)
                mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            else {
                mtwkSetSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
                mtwkSetRegionFlag(context.motionWork, 69, mtwkHasDependentTrack(context.motionWork) ? 0 : 2, MotionRegion::LowerBody);
            }

            shenApplyTranslationToAll(&character->entity.node, context.doorPosition);
            ApplyNpcRouteEuler(context);
            if (CharacterControlModel *const control = FindCharacterControlModel(character))
                control->flags = (control->flags & ~0x806U) | 0x800U;

            context.flags |= 0x200000U;
        }
        return context.avoidancePhase;
    case 1:
        if (mtwkSetRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody)) {
            Vector3 position = NpcRouteAreaToScene(context.localPosition);
            position.y += (context.flags & 0x800U) ? 0.44999999f : -0.44999999f;
            const std::int32_t remaining = context.stairRemaining - 2;
            context.stairRemaining = static_cast<std::int8_t>(remaining < 0 ? 0 : remaining);
            if (context.stairRemaining == 1)
                context.avoidancePhase = 2;
            else if (!context.stairRemaining)
                context.avoidancePhase = 4;

            shenApplyTranslationToAll(&character->entity.node, position);
            if ((context.routeFlags & 0x800U) && context.stairRemaining) {
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.avoidancePhase = 3;
                mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            }
        } else if (context.routeFlags & 0x800U) {
            const float middleDistance = std::abs(mtwkGetPoseFrame(context.motionWork, MotionRegion::LowerBody) - context.stairMotionFrames * 0.5f);
            const bool nearMiddle = context.motionScale * 1.1f > middleDistance;
            if (nearMiddle || mtwkSetRegionFlag(context.motionWork, 68, 1, MotionRegion::LowerBody)) {
                if (nearMiddle) {
                    Vector3 position = NpcRouteAreaToScene(context.localPosition);
                    position.y += (context.flags & 0x800U) ? 0.22499999f : -0.22499999f;
                    shenApplyTranslationToAll(&character->entity.node, position);
                }
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.avoidancePhase = 3;
                mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            }
        }
        return context.avoidancePhase;
    case 2: {
        const Vector3 local = NpcPositionInFrame(context.localPosition, context.stairTarget, context.stairYaw);
        if (local.z > 0.0f)
            return context.avoidancePhase;
        Vector3 position = NpcRouteAreaToScene(context.localPosition);
        context.stairRemaining = 0;
        position.y += (context.flags & 0x800U) ? 0.22499999f : -0.22499999f;
        shenApplyTranslationToAll(&character->entity.node, position);
        break;
    }
    case 3:
        if (context.routeFlags & 0x800U) {
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
        }
        if (!(context.flags & 0x101000U))
            context.avoidancePhase = 0;
        return context.avoidancePhase;
    case 4:
        break;
    default:
        return context.avoidancePhase;
    }
    if (context.laneLow > context.lateralPosition || context.lateralPosition > context.laneHigh)
        context.routeFlags |= 0x800000U;

    if (context.routeFlags & 0x800U) {
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
        mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
    } else {
        mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
        context.motion = SelectNpcLocomotion(context, (context.flags & 0x10U) ? 1 : 0);
        mtwkRequestMotion(context.motionWork, context.motion, MotionRegion::LowerBody);
        mtwkSetRegionFlag(context.motionWork, 69, mtwkHasDependentTrack(context.motionWork) ? 2 : 0, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(context.motionWork, 1);
        mtwkSetFrameTime(context.motionWork, 6, MotionRegion::LowerBody);
    }
    context.flags = (context.flags & ~0x400U) | 0x10000U;
    context.avoidancePhase = 0;
    return 0;
}

bool RequestNpcRouteMotion(NpcRouteStateContext &context, bool explicitRequest) {
    Character *const character = context.owner->node;
    if (!character || !(context.flags & 3U))
        return false;
    mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
    if (context.flags & 0x1000U) {
        mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
        return true;
    }
    switch (context.phase) {
    case 5: {
        CharacterControlModel *const control = FindCharacterControlModel(character);
        std::uint32_t flags = control ? control->flags | 0x800U : 0x800U;
        if (context.moveState == 1 || (!context.moveState && explicitRequest)) {
            mtwkCopyTransition(context.motionWork, &context.sceneTarget, static_cast<std::uint16_t>(context.targetYaw), 0, 0);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
            mtwkSetTimeRelated(context.motionWork, 1, 5, context.doorMotion);
            mtwkSetRegionEvalTime(context.motionWork, 14.0f, MotionRegion::LowerBody);
            flags &= ~4U;
        } else if (context.moveState == 3) {
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.targetYaw), 13, 0);
            mtwkSetTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (control)
            control->flags = flags;
        return false;
    }
    case 6:
        if (context.moveState != 1 && (context.moveState || !explicitRequest))
            return false;
        mtwkCopyTransition(context.motionWork, &context.sceneTarget, static_cast<std::uint16_t>(context.targetYaw), 5, 0);
        mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        if (context.flags & 0x10U)
            mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0);
        return false;
    case 7: {
        const std::int32_t stair = DetectNpcRouteStair(context);
        if (!stair) {
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 5, 2);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
            if (context.flags & 0x10U)
                mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0);
        } else if (stair == 1 && !explicitRequest) {
            UpdateNpcRouteAvoidance(context);
            TickNpcRouteStair(context);
        }
        return !explicitRequest;
    }
    case 8: {
        if (context.moveState != 1 && (context.moveState || !explicitRequest)) {
            if (context.moveState == 3) {
                mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.targetYaw), 13, 0);
                mtwkSetTimeRelated(context.motionWork, 1, 5, 0);
            }
            return false;
        }
        Vector3 target = context.sceneTarget;
        if (context.otherName.name) {
            context.doorMotion =
                static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), character, &context.scenePosition));
            if (context.otherName.name != static_cast<std::uint32_t>(GetSelectedNpcDoorId())) {
                context.doorMotion = 0;
                context.otherName = {};
            } else if (context.doorMotion) {
                Vector3 track{};
                SampleNativeMotionRoot(context.doorMotion, GetCharacterBoneId(character), 15.0f, track);
                Matrix4x4_Push(&transformMatrix_prePass);
                Matrix4x4::SetViewMatrixFromEuler(context.targetYaw, 0, 0);
                track = TransformNpcPoint(track);
                Matrix4x4::Pop(1);
                target.x += track.x;
                target.z += track.z;
            }
        }
        mtwkCopyTransition(context.motionWork, &target, static_cast<std::uint16_t>(context.targetYaw), 6, 0);
        mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        if (context.flags & 0x10U)
            mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0);
        if (context.otherName.name && context.doorMotion) {
            mtwkSetTimeRelated(context.motionWork, 1, 5, context.doorMotion);
            mtwkSetRegionEvalTime(context.motionWork, 14.0f, MotionRegion::LowerBody);
        }
        return false;
    }
    default:
        return false;
    }
}

void InitializeNpcRouteRequest(NpcRouteStateContext &context, std::uint16_t current, std::uint16_t target) {
    context.phase = 0;
    context.flags = (context.flags & 0x20002U) | 1U;
    context.previousSegment = -10;
    context.otherName = {};
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.doorWaitFrames = -1;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.routeFlags = (context.owner->flags & 0x400000U) ? 0x100206U : 0x206U;
    context.scriptOwner = context.owner->name;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.currentAreaPoint = current;
    context.pendingAreaPoint = target;
    context.motionScale = 1.0f;
    context.motionWork = context.owner->motionWork;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node)) {
        std::uint32_t flags = (context.owner->flags & 0x8000000U) ? control->flags | 0x800U : control->flags & ~0x800U;
        if (npcSchedule.floor == -1000 || !(context.owner->packedMoveFlags & 0xff00U))
            flags |= 2U;
        SetCharacterControlModelFlags(context.owner->node, flags | 4U);
    }
}

std::int32_t AdvanceNpcRouteByDistance(const NpcRouteStateContext &context, float distance) {
    const std::int32_t direction = distance >= 0.0f ? 1 : -1;
    if (distance < 0.0f)
        distance = -distance;
    std::int32_t routeIndex = context.activeRouteIndex;
    std::int32_t segment = context.activeSegment;
    std::int32_t available = direction == 1 ? 64 - (context.parameter & 63) : context.parameter & 63;
    std::int32_t advanced = 0;
    bool connection = context.crossingPoint == 2;
    std::int32_t count = static_cast<std::int32_t>(context.stateTable[context.routeNodes[routeIndex] & 0x7fff].pointCount) - 3;
    for (;;) {
        const NpcRouteNode &node = context.stateTable[context.routeNodes[routeIndex] & 0x7fff];
        const float length = connection ? node.connectionLengths[context.routeConnections[routeIndex]] :
                                          ReadF32(ResolveNpcRouteData(context, node.lengthsOffset) + (segment + 1) * sizeof(float));
        const float remaining = distance - static_cast<float>(available) * length * 0.015625f;
        if (remaining <= 0.0f) {
            advanced += static_cast<std::int32_t>(distance * 64.0f / length);
            break;
        }
        advanced += available;
        segment += context.routeNodes[routeIndex] < 0 ? -direction : direction;
        connection = false;
        available = 64;
        distance = remaining;
        if (segment > count || segment < -1) {
            routeIndex += direction;
            if (routeIndex < 0 || routeIndex >= context.routeCount)
                break;
            count = static_cast<std::int32_t>(context.stateTable[context.routeNodes[routeIndex] & 0x7fff].pointCount) - 3;
            segment = (context.routeNodes[routeIndex] < 0) == (direction == 1) ? count - 1 : 0;
        } else if (segment == count || segment == -1) {
            connection = true;
        }
    }
    const std::int32_t result = context.parameter + direction * advanced;
    return result > context.highParameter ? context.highParameter : result < context.lowParameter ? context.lowParameter : result;
}

bool MeasureNpcAvoidanceCandidate(Vector3 &center, float radius, std::int32_t kind, float &distance) {
    center = TransformNpcPoint(center);
    if (center.z - radius - npcAvoidanceRadius > 0.0f)
        return false;
    const float lateral = center.x + npcAvoidanceLateral;
    if (npcAvoidanceLow - npcAvoidanceRadius > lateral + radius || lateral - radius > npcAvoidanceHigh + npcAvoidanceRadius)
        return false;
    const float forward = -(center.z + radius + npcAvoidanceRadius);
    if ((kind == 2 && forward < -1.0f) || forward > npcAvoidanceDistance)
        return false;
    if (forward < 0.0f) {
        const float combined = npcAvoidanceRadius + radius;
        if (std::abs(center.x) > combined || center.z > 0.0f) {
            if (center.x >= 0.0f) {
                const float high = lateral - combined;
                if (npcAvoidanceHigh > high)
                    npcAvoidanceHigh = high;
            } else {
                const float low = lateral + combined;
                if (low > npcAvoidanceLow)
                    npcAvoidanceLow = low;
            }
            if (npcAvoidanceLow > npcAvoidanceHigh)
                npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
            return false;
        }
    }
    distance = forward;
    return true;
}

void InsertNpcAvoidanceCandidate(std::uint16_t kind, NpcRouteLiveEntry *source, const Vector3 &local, const Vector3 &position, float radius, float distance) {
    NpcAvoidanceCandidate *insertion = npcAvoidanceFirst;
    while (insertion && distance > insertion->distance)
        insertion = insertion->next;
    if (!insertion)
        return;
    NpcAvoidanceCandidate *const candidate = npcAvoidanceLast;
    candidate->distance = distance;
    candidate->source = source;
    candidate->kind = kind;
    candidate->localX = local.x;
    candidate->localZ = local.z;
    candidate->position = position;
    candidate->lateralRadius = candidate->forwardRadius = radius;
    if (insertion == candidate)
        return;
    npcAvoidanceLast = candidate->previous;
    npcAvoidanceLast->next = nullptr;
    candidate->previous = insertion->previous;
    candidate->next = insertion;
    if (insertion->previous)
        insertion->previous->next = candidate;
    else
        npcAvoidanceFirst = candidate;
    insertion->previous = candidate;
}

void UpdateNpcStairAvoidance(NpcRouteStateContext &context, const NpcAvoidanceCandidate &candidate) {
    std::int32_t response = 0;
    if (candidate.kind != 3) {
        if (candidate.distance < 0.5f) {
            context.flags |= 0x400000U;
            response = 1;
        }
    } else {
        const NpcRouteLiveEntry &other = *candidate.source;
        const NpcRouteStateContext &route = other.stateContext;
        if (!(route.flags & 0x400U)) {
            if (candidate.distance < 0.5f)
                response = 2;
        } else {
            const std::int16_t delta = static_cast<std::int16_t>(route.stairYaw - context.stairYaw);
            const std::int32_t difference = delta < 0 ? -static_cast<std::int32_t>(delta) : delta;
            if (difference < 10) {
                if (std::abs(context.stairRemaining - route.stairRemaining) <= 4)
                    response = 2;
            } else if (difference > 32758) {
                if ((other.flags & 0x10000U) || (route.flags & 0x400000U) || (route.routeFlags & 0x10U)) {
                    context.flags |= 0x400000U;
                    response = 3;
                } else if (context.flags & 0x400000U)
                    response = 1;
                else if (context.stairRemaining >= route.stairRemaining)
                    response = 3;
                else if (std::abs(context.stairCount - context.stairRemaining - route.stairRemaining) <= 4)
                    response = 2;
            }
        }
    }
    if (!response) {
        context.flags &= 0xffafffffU;
        context.avoidanceResult = 0;
        SetNpcRouteMotionState(context, 3);
        context.avoidanceWaitFrames = 0;
        return;
    }
    if ((context.owner->flags & 0x10000U) || (context.routeFlags & 0x10U))
        response = 2;
    if (response == 2)
        context.avoidanceWaitFrames = 0;
    else if (response == 3)
        context.avoidanceWaitFrames = 90;
    context.avoidanceResult = 1;
    if (!(context.flags & 0x1000U)) {
        context.routeFlags |= 0x800U;
        context.flags |= 0x100000U;
    }
}

std::uint8_t UpdateNpcRouteAvoidance(NpcRouteStateContext &context) {
    const std::uint8_t previousResult = context.avoidanceResult;
    context.other = nullptr;
    if (previousResult == 4)
        context.field0a1 |= 0x20;
    else
        context.field0a1 &= ~0x20;
    context.avoidanceResult = context.field0a0 = 0;
    for (std::size_t index = 0; index < npcAvoidanceCandidates.size(); ++index) {
        NpcAvoidanceCandidate &candidate = npcAvoidanceCandidates[index];
        candidate.previous = index ? &npcAvoidanceCandidates[index - 1] : nullptr;
        candidate.next = index + 1 < npcAvoidanceCandidates.size() ? &npcAvoidanceCandidates[index + 1] : nullptr;
        candidate.distance = 10.0f;
        candidate.kind = candidate.flags = 0;
    }
    npcAvoidanceFirst = npcAvoidanceCandidates.data();
    npcAvoidanceLast = &npcAvoidanceCandidates.back();
    npcAvoidanceLow = context.laneLow;
    npcAvoidanceHigh = context.laneHigh;
    npcAvoidanceRadius = context.owner->streamRadius;
    npcAvoidanceLateral = context.laneOffset - context.laneStep;
    npcAvoidanceDistance = 10.0f;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(context.tangentYaw), 0, 0);
    const Vector3 &center = context.owner->streamCenter;
    Matrix4x4::preTranslate(-center.x, -center.y, -center.z);
    if (npcRegistryTask.areaCount) {
        for (NpcNoSpawnArea &area : npcRegistryTask.areas) {
            if (!area.name.name || std::abs(area.center.y - context.scenePosition.y) > 1.8f)
                continue;
            Vector3 local = area.center;
            float distance;
            if (MeasureNpcAvoidanceCandidate(local, area.radius, 2, distance) && context.motionStep * 30.0f + 3.0f > distance) {
                InsertNpcAvoidanceCandidate(2, reinterpret_cast<NpcRouteLiveEntry *>(&area), local, area.center, area.radius, distance);
            }
            npcAvoidanceDistance = npcAvoidanceLast->distance;
        }
    }
    if (npcPlayerPresent && std::abs(npcPlayerPosition.y - context.scenePosition.y) < 1.8f) {
        Vector3 local = npcPlayerCenter;
        float distance;
        if (MeasureNpcAvoidanceCandidate(local, npcPlayerRadius, 1, distance) &&
            std::fmax(context.motionStep, npcPlayerDisplacement) * 30.0f + 3.0f > distance) {
            InsertNpcAvoidanceCandidate(1, reinterpret_cast<NpcRouteLiveEntry *>(&npcPlayerPosition), local, npcPlayerCenter, npcPlayerRadius, distance);
        }
        npcAvoidanceDistance = npcAvoidanceLast->distance;
    }
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry && entry != npcCrowdBookmark; entry = entry->next) {
        if (!entry->node || entry->name.name == context.scriptOwner.name || (entry->flags & 0x80000040U) ||
            std::abs(entry->localPosition.y - context.scenePosition.y) > 1.8f)
            continue;
        Vector3 local = entry->streamCenter;
        float distance;
        if (MeasureNpcAvoidanceCandidate(local, entry->streamRadius, 3, distance) && context.motionStep * 30.0f + 3.0f > distance)
            InsertNpcAvoidanceCandidate(3, entry, local, entry->streamCenter, entry->streamRadius, distance);
    }
    Matrix4x4::Pop(1);
    NpcAvoidanceCandidate *obstacle = npcAvoidanceFirst;
    float obstacleLateral = 0.0f;
    float combinedRadius = 0.0f;
    while (obstacle && obstacle->kind) {
        obstacleLateral = obstacle->localX + npcAvoidanceLateral;
        combinedRadius = npcAvoidanceRadius + obstacle->lateralRadius;
        if (std::abs(obstacle->localX) <= combinedRadius)
            break;
        if (obstacle->distance < 0.0f) {
            if (obstacle->localX >= 0.0f)
                npcAvoidanceHigh = std::fmin(npcAvoidanceHigh, obstacleLateral - combinedRadius);
            else
                npcAvoidanceLow = std::fmax(npcAvoidanceLow, obstacleLateral + combinedRadius);
            if (npcAvoidanceLow > npcAvoidanceHigh)
                npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
        }
        obstacle = obstacle->next;
    }
    if (!obstacle || !obstacle->kind) {
        context.field0a1 &= ~8;
        return context.avoidanceResult;
    }
    float nearestZ = obstacle->localZ;
    const float forwardRadius = npcAvoidanceRadius + obstacle->forwardRadius;
    const float separation = npcAvoidanceRadius * 2.0f + 0.1f;
    for (NpcAvoidanceCandidate *candidate = npcAvoidanceFirst; candidate && candidate->kind;) {
        if (candidate != obstacle && candidate->kind != 4) {
            if (candidate->lateralRadius + obstacle->forwardRadius + separation > std::abs(candidate->localZ - obstacle->localZ) &&
                candidate->lateralRadius + obstacle->lateralRadius + separation > std::abs(candidate->localX - obstacle->localX)) {
                const float radius = (obstacle->lateralRadius + std::abs(candidate->localX - obstacle->localX) + candidate->lateralRadius) * 0.5f;
                if (radius > obstacle->lateralRadius) {
                    obstacle->localX = std::fmin(obstacle->localX - obstacle->lateralRadius, candidate->localX - candidate->lateralRadius) + radius;
                    obstacle->lateralRadius = radius;
                    obstacleLateral = npcAvoidanceLateral + obstacle->localX;
                    combinedRadius = npcAvoidanceRadius + radius;
                    nearestZ = std::fmin(candidate->localZ, nearestZ);
                    candidate->kind = 4;
                    candidate = npcAvoidanceFirst;
                    continue;
                }
            }
            if (candidate->distance < 0.0f) {
                const float lateral = npcAvoidanceLateral + candidate->localX;
                const float radius = candidate->lateralRadius + npcAvoidanceRadius;
                if (obstacle->localX >= 0.0f)
                    npcAvoidanceHigh = std::fmin(npcAvoidanceHigh, lateral - radius);
                else
                    npcAvoidanceLow = std::fmax(npcAvoidanceLow, lateral + radius);
                if (npcAvoidanceLow > npcAvoidanceHigh)
                    npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
            }
        }
        candidate = candidate->next;
    }
    if (nearestZ > 0.0f || (context.flags & 0x10400U) == 0x400U) {
        if (nearestZ <= 0.0f)
            UpdateNpcStairAvoidance(context, *obstacle);
        context.field0a1 &= ~8;
        return context.avoidanceResult;
    }
    std::int32_t side = context.avoidanceAuxiliary;
    if (obstacle->kind == 3 && obstacle->source->stateContext.phase == 7 && obstacle->source->stateContext.other == context.owner)
        side = obstacle->source->stateContext.avoidanceAuxiliary;
    else if (previousResult < 2 && !(context.field0a1 & 0x0a)) {
        const Vector3 local = NpcPositionInFrame(obstacle->position, center, ReadNpcEulerYaw(context.savedEuler));
        side = local.x < 0.0f ? 1 : -1;
    }
    std::uint8_t flags = static_cast<std::uint8_t>(context.field0a1) & ~8U;
    const std::int32_t sign = side == -1 ? -1 : 1;
    const float localX = obstacle->localX * static_cast<float>(sign);
    const float high = sign == -1 ? -npcAvoidanceLow : npcAvoidanceHigh;
    const float low = sign == -1 ? -npcAvoidanceHigh : npcAvoidanceLow;
    obstacleLateral *= static_cast<float>(sign);
    if (sign == -1)
        obstacle->flags |= 1U;
    float targetLane = obstacleLateral + combinedRadius + 0.1f;
    float retreatDistance = 0.0f;
    std::uint8_t result = 4;
    bool moveAside = localX <= combinedRadius * 0.5f && targetLane <= high && (localX <= 0.0f || obstacle->distance >= 0.15000001f);
    if (!moveAside) {
        if (low <= obstacleLateral - combinedRadius - 0.1f)
            side = -side;
        else if (targetLane > high) {
            if (forwardRadius + 0.5f <= -obstacle->localZ) {
                targetLane = localX <= combinedRadius * 0.5f ? high : low;
                flags |= 8U;
                moveAside = true;
            } else
                result = 1;
        } else if (forwardRadius + 0.15000001f <= -obstacle->localZ)
            moveAside = true;
        else if (!(context.flags & 0x10000U)) {
            flags &= ~4U;
            retreatDistance = forwardRadius + 0.15000001f + obstacle->localZ;
            result = 3;
        } else
            result = 1;
    }
    if (moveAside) {
        targetLane *= static_cast<float>(sign);
        const float distance = std::abs(targetLane - npcAvoidanceLateral);
        if (forwardRadius + 0.80000001f <= -obstacle->localZ || distance < 0.40000001f) {
            result = 2;
            context.runtime0fc = 0;
            if (std::abs(distance) >= 0.001f || std::abs(obstacle->distance) >= 0.001f) {
                const std::int16_t angle = static_cast<std::int16_t>(FixedAtan2(distance, obstacle->distance));
                context.runtime0fc = static_cast<std::uint32_t>(2 * std::abs(static_cast<std::int32_t>(angle)));
                if (context.runtime0fc > 0x4000U)
                    context.runtime0fc = 0x4000U;
            }
        } else if (distance >= 0.80000001f) {
            result = 6;
            flags &= ~2U;
        } else
            result = 5;
    } else if (result == 1 && obstacle->distance <= 0.5f) {
        const std::uint16_t parameter = context.parameter;
        context.parameter = static_cast<std::uint16_t>(AdvanceNpcRouteByDistance(context, 2.0f));
        if (context.parameter != context.highParameter) {
            Vector3 sample;
            SampleNpcRouteWindow(context, &sample, false);
            sample = NpcPositionInFrame(NpcRouteAreaToScene(sample), center, context.tangentYaw);
            const Vector3 local = NpcPositionInFrame(obstacle->position, center, context.tangentYaw);
            if (sample.z > local.z + forwardRadius)
                result = 0;
        }
        context.parameter = parameter;
    }
    context.avoidanceAuxiliary = static_cast<std::int8_t>(side);
    context.avoidanceResult = result;
    context.field0a1 = static_cast<std::int8_t>(flags);
    context.field0a0 = static_cast<std::uint8_t>(obstacle->kind);
    context.other = obstacle->source;
    switch (result) {
    case 0:
        context.other = nullptr;
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
        }
        break;
    case 1:
        context.other = nullptr;
        if (!(context.flags & 0x100000U)) {
            context.routeFlags |= 0x800U;
            context.flags |= 0x100000U;
        }
        break;
    case 2:
    case 5:
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
        }
        if (context.field0a1 & 2) {
            context.field0a1 &= ~2;
            RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
        }
        context.laneOffset = targetLane;
        context.laneStep = targetLane - context.lateralPosition;
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, targetLane);
        break;
    case 3:
        if (!(flags & 4U)) {
            const Vector3 behind = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw - 0x4000, retreatDistance);
            Vector3 target = NpcRouteAreaToScene(behind);
            target.y = 0.0f;
            context.flags |= 0x101000U;
            context.field0a1 = static_cast<std::int8_t>((flags & ~6U) | 4U);
            mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            const Vector3 &root = context.motionWork->runtime.rootPosition;
            const float x = target.x - root.x;
            const float z = target.z - root.z;
            const float maximum = GetMotionRigScale(static_cast<std::uint32_t>(mtwkGetRigBoneId(context.motionWork)), 8, 0) * 0.55000001f * 0.89999998f;
            const float distance = std::sqrt(x * x + z * z);
            if (distance > maximum)
                target = {root.x + x * (maximum / distance), root.y, root.z + z * (maximum / distance)};
            mtwkCopyTransition(context.motionWork, &target, static_cast<std::uint16_t>(context.tangentYaw), 6, 3);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
            if (context.flags & 0x10U)
                mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0);
            context.routeFlags |= 0x800000U;
        }
        break;
    case 4:
        context.other = nullptr;
        context.field0a1 &= ~0x20;
        break;
    case 6:
        if (!(flags & 2U)) {
            if (context.flags & 0x100000U) {
                context.routeFlags &= ~0x800U;
                context.flags &= ~0x101000U;
            }
            if (context.movementProgress <= 0.0f) {
                if (!(context.flags & 0x80000U)) {
                    context.flags |= 0x80000U;
                    context.savedSplinePosition = context.position;
                    context.savedMoveTarget = context.moveTarget;
                    context.field13c = context.laneOffset;
                }
                context.position = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw, -npcAvoidanceLateral);
                context.moveTarget = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw, targetLane - npcAvoidanceLateral);
            } else
                context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, targetLane);
            const Vector3 target = NpcRouteAreaToScene(context.moveTarget);
            context.laneOffset = targetLane;
            context.field0a1 = static_cast<std::int8_t>((flags & ~6U) | 2U);
            RequestNpcRouteTurn(context, &target, static_cast<std::uint16_t>(context.tangentYaw), 3);
        }
        break;
    }
    return result;
}

bool TestNpcForwardCylinder(const Vector3 &position, std::int16_t angle, float radius, float length) {
    const Vector3 local = TransformNpcPoint(position);
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-local.x, -local.z));
    return static_cast<float>(angle) > std::abs(static_cast<float>(yaw)) && local.z > -length && radius > std::abs(local.x);
}

bool UpdateNpcObstructionState(NpcRouteStateContext &context, bool blocked) {
    if (!blocked) {
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
            return true;
        }
    } else if (context.flags & 0x100000U) {
        if (!(context.owner->flags & 0x4010000U)) {
            context.owner->streamingCountdown = static_cast<std::uint16_t>((context.owner->streamingCountdown & 0xff00U) | 10U);
            if (++context.obstructionFrames > 300) {
                RequestNpcStreamOut(*context.owner, 2);
                context.obstructionFrames = 0;
                return true;
            }
        }
    } else {
        context.routeFlags |= 0x800U;
        context.obstructionFrames = 0;
        context.flags |= 0x100000U;
    }
    return false;
}

void UpdateNpcAmbientLook(NpcRouteStateContext &context) {
    if (!context.owner->node)
        return;

    if (context.doorWaitFrames < 0) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        }

        const std::int32_t interval = 120 * static_cast<std::int8_t>(context.owner->recordBase[5]);
        context.doorWaitFrames = static_cast<std::int16_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(interval)));
        if (context.doorWaitFrames < interval / 2)
            context.doorWaitFrames = static_cast<std::int16_t>(interval / 2);
    }

    if (--context.doorWaitFrames != 120)
        return;

    std::int32_t point = static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(npcSchedule.areaPointCount));
    if (npcSchedule.areaPointCount && point >= static_cast<std::int32_t>(npcSchedule.areaPointCount))
        point = static_cast<std::int32_t>(npcSchedule.areaPointCount) - 1;
    const std::byte *const areaPoint = GetNpcAreaPoint(static_cast<std::uint16_t>(point));
    if (!areaPoint)
        return;

    Vector3 target;
    std::memcpy(&target, areaPoint + 0x34, sizeof(target));
    Vector3 head{};
    mtwkGetBonePosition(context.motionWork, MB_Eyes, &head);
    if (!std::isnormal(head.y) && head.y != 0.0f)
        head.y = 0.0f;
    target.y = static_cast<float>(point % 25) * 0.0099999998f + head.y - static_cast<float>(static_cast<std::int8_t>(context.owner->recordBase[8]) - 1) * 0.1f;
    target = NpcRouteAreaToScene(target);
    mtwkUpdateWithPosition(context.motionWork, &target, 0);
    context.flags |= 0x20U;
}

void TickNpcRouteMovement(NpcRouteStateContext &context) {
    Character *const character = context.owner->node;
    if (!character) {
        if (AdvanceUnstreamedNpc(context)) {
            context.moveState = 2;
            StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
        }
        return;
    }
    if ((context.flags & 0x41000U) == 0x41000U)
        return;
    Vector3 offset = NpcPositionInFrame(context.moveTarget, context.localPosition, context.tangentYaw);
    context.lateralPosition = context.laneOffset - offset.x;
    context.laneStep = offset.x;
    context.movementProgress = offset.z;
    if (offset.z > -context.motionStep) {
        if (context.flags & 0x80000U) {
            context.flags &= ~0x80000U;
            context.laneOffset = context.field13c;
            context.position = context.savedSplinePosition;
            context.moveTarget = context.savedMoveTarget;
            offset = NpcPositionInFrame(context.moveTarget, context.localPosition, context.tangentYaw);
            context.lateralPosition = context.laneOffset - offset.x;
            context.laneStep = offset.x;
            context.movementProgress = offset.z;
        } else {
            context.extraStride =
                offset.z <= context.motionStep ? 0 : static_cast<std::uint16_t>(static_cast<std::int32_t>(offset.z * 64.0f / context.segmentLength));
            context.moveState = 2;
        }
    }
    if (context.owner->flags & 0x40U) {
        StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
        ApplyNpcRouteEuler(context);
        return;
    }
    DetectNpcRouteStair(context);
    bool largeCorrection = false;
    if (!(context.flags & 0x400U)) {
        const float fromLow = context.interpolatedLaneLow - context.lateralPosition;
        const float fromHigh = context.lateralPosition - context.interpolatedLaneHigh;
        if (fromLow > 0.0f || fromHigh > 0.0f) {
            if ((context.routeFlags & 0x800000U) && !(context.routeFlags & 0x80U)) {
                if (context.movementProgress < 0.0f && context.interpolatedLaneHigh - context.interpolatedLaneLow < 0.30000001f)
                    context.routeFlags &= ~0x800000U;
            } else {
                const float boundary = fromLow > 0.0f ? context.interpolatedLaneLow : context.interpolatedLaneHigh;
                offset.x -= boundary - context.lateralPosition;
                context.lateralPosition = boundary;
                context.laneStep = offset.x;
                offset = {-offset.x, -offset.y, -offset.z};
                Matrix4x4_Push(&transformMatrix_prePass);
                Matrix4x4::preTranslate(context.moveTarget.x, context.moveTarget.y, context.moveTarget.z);
                Matrix4x4::SetViewMatrixFromEuler(context.tangentYaw, 0, 0);
                const Vector3 position = TransformNpcPoint(offset);
                Matrix4x4::Pop(1);
                if (!(context.routeFlags & 0x80U) && (fromLow > 0.1f || fromHigh > 0.1f)) {
                    largeCorrection = true;
                    context.routeFlags |= 0x800000U;
                } else {
                    context.routeFlags = (context.routeFlags & ~0x880000U) | 0x80000U;
                    context.localPosition = position;
                    if (context.interpolatedLaneHigh - context.interpolatedLaneLow <= 0.2f)
                        context.laneCorrectionFrames = 0;
                    else if (++context.laneCorrectionFrames > 128)
                        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
                }
            }
        } else if (context.movementProgress < 0.0f) {
            context.routeFlags &= ~0x800000U;
            context.laneCorrectionFrames = 0;
        }
    }
    std::int32_t yawDelta = static_cast<std::int16_t>(context.tangentYaw - ReadNpcEulerYaw(context.savedEuler));
    if ((context.routeFlags & 2U) && !(context.owner->flags & 0x10U) && !(context.flags & 0x40000U)) {
        if (!UpdateNpcRouteAvoidance(context)) {
            if (context.flags & 0x100000U) {
                context.flags &= ~0x100000U;
                SetNpcRouteMotionState(context, 3);
            }
            if (context.field0a1 & 2) {
                context.field0a1 &= ~2;
                RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
            }
            context.avoidanceWaitFrames = 0;
            if (context.routeFlags & 0x200U)
                UpdateNpcAmbientLook(context);
        } else if ((context.flags & 0x1000U) && !(context.owner->flags & 0x10000U) && !(context.routeFlags & 0x10U) && ++context.avoidanceWaitFrames > 90) {
            context.avoidanceWaitFrames = 0;
            if (context.routeFlags & 0x1000000U) {
                context.owner->streamingCountdown = static_cast<std::uint16_t>((context.owner->streamingCountdown & 0xff00U) | 10U);
                RequestNpcStreamOut(*context.owner, 2);
                return;
            }
            const std::uint32_t flags = context.flags;
            const bool returning = (context.routeFlags & 0x4000000U) != 0;
            InitializeNpcRouteRequest(context, context.pendingAreaPoint, context.currentAreaPoint);
            SetNpcLocomotionScale(context, context.owner->motionSpeed);
            if (flags & 0x10U)
                SetNpcRouteMotionState(context, 1);
            if (!returning)
                context.routeFlags |= 0x4000000U;
            if (flags & 0x400000U)
                context.flags |= 0x400000U;
            if (flags & 0x200000U) {
                if (CharacterControlModel *const control = FindCharacterControlModel(character))
                    control->flags = (control->flags & ~0x806U) | 0x800U;
                context.flags |= 0x200000U;
            }
            return;
        }
    }
    if (!TickNpcRouteStair(context) && (context.routeFlags & 0x800U)) {
        mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 13, 0);
        mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
        return;
    }
    if (context.flags & 0x400U) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        }
        return;
    }
    if (mtwkSetExtraFlag(context.motionWork, 58, 1)) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        }
        context.field0a1 |= 1;
        StoreNpcEulerYaw(context.savedEuler, context.motionWork->runtime.entityRotation.y);
        if (mtwkCopyTransition(context.motionWork, nullptr, 0, 3, 0))
            RequestNpcRouteMotion(context, true);
    } else if (context.field0a1 & 1) {
        context.field0f4 = 0;
        const Vector3i rotation = GetCharacterWorldRotation(character);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        context.field0a1 &= ~7;
        if (context.moveState == 2)
            return;
        context.laneOffset = context.lateralPosition;
        if (context.laneOffset > context.laneHigh)
            context.laneOffset = context.laneHigh;
        else if (context.laneLow > context.laneOffset)
            context.laneOffset = context.laneLow;
        context.laneStep = context.laneOffset - context.lateralPosition;
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, context.laneOffset);
    }
    const std::uint8_t avoidance = context.avoidanceResult;
    if (avoidance == 6 || avoidance == 3 || (context.flags & 0x100000U))
        return;
    if (avoidance == 5) {
        RequestNpcRouteTurn(context, nullptr,
            static_cast<std::uint16_t>(NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z)), 2);
        return;
    }
    std::int32_t correction = avoidance == 2 ? static_cast<std::int32_t>(context.runtime0fc) :
                                               static_cast<std::int32_t>(std::abs(context.laneStep) * 16384.0f * (largeCorrection ? 3.0f : 1.0f));
    if (correction > 0x4000)
        correction = 0x4000;
    if (avoidance)
        context.field0a1 |= 0x10;
    else if (context.field0a1 & 0x10) {
        context.field0f4 = 0;
        context.field0a1 &= ~0x10;
    }
    const std::int32_t side = context.laneStep < 0.0f ? -1 : context.laneStep > 0.0f ? 1 : 0;
    const std::int32_t corrected = static_cast<std::int16_t>(yawDelta - correction * side);
    if (yawDelta * side >= 0 || largeCorrection)
        yawDelta = corrected;
    const std::int32_t magnitude = std::abs(yawDelta);
    if (magnitude > 10922 && (!avoidance || (context.field0a1 & 0x20))) {
        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + (avoidance == 2 ? corrected : yawDelta)), 2);
        return;
    }
    const std::int32_t direction = corrected < 0 ? -1 : corrected > 0 ? 1 : 0;
    context.field0f4 += static_cast<std::int32_t>(context.motionStep * 1000.0f);
    if (magnitude < static_cast<std::int32_t>(context.field0f4))
        context.field0f4 = magnitude;
    if (static_cast<std::int32_t>(context.field0f4) > context.maximumYawStep)
        context.field0f4 = context.maximumYawStep;
    if (avoidance == 2 && mtwkSetExtraFlag(context.motionWork, 58, 1) && static_cast<std::int32_t>(context.field0f4) == context.maximumYawStep) {
        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(corrected + ReadNpcEulerYaw(context.savedEuler)), 2);
    } else if (!mtwkSetExtraFlag(context.motionWork, 58, 1)) {
        StoreNpcEulerYaw(context.savedEuler, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + direction * context.field0f4));
        ApplyNpcRouteEuler(context);
    }
}

std::int32_t FindNearestNpcParameter(NpcRouteStateContext &context, const Vector3 &position) {
    Vector3 sampled;
    context.parameter = 0;
    SampleNpcRouteWindow(context, &sampled, false);
    std::int32_t nearest = 0;
    float x = position.x - sampled.x;
    float z = position.z - sampled.z;
    float distance = x * x + z * z;
    context.parameter += 32;
    std::int32_t valid;
    do {
        valid = SampleNpcRouteWindow(context, &sampled, true);
        x = position.x - sampled.x;
        z = position.z - sampled.z;
        const float candidate = x * x + z * z;
        if (distance > candidate) {
            nearest = context.parameter;
            distance = candidate;
        }
        context.parameter += 32;
    } while (valid == 1);
    context.parameter = static_cast<std::uint16_t>(nearest < 64 ? 0 : nearest - 64);
    const std::int32_t end = nearest + 64;
    SampleNpcRouteWindow(context, &sampled, false);
    x = position.x - sampled.x;
    z = position.z - sampled.z;
    if (distance > x * x + z * z) {
        nearest = context.parameter;
        distance = x * x + z * z;
    }
    ++context.parameter;
    valid = 1;
    while (context.parameter < end && valid == 1) {
        valid = SampleNpcRouteWindow(context, &sampled, true);
        x = position.x - sampled.x;
        z = position.z - sampled.z;
        if (distance > x * x + z * z) {
            nearest = context.parameter;
            distance = x * x + z * z;
        }
        ++context.parameter;
    }
    return nearest;
}

void EnterNpcRoute(NpcRouteStateContext &context) {
    if (context.flags & 2U) {
        context.startNode = context.crossingPoint == 2 && (context.parameter & 63U) > 32U ? context.nextNode : context.activeNode;
        context.routeFlags |= 0x804000U;
        context.endNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.pendingAreaPoint) + 4));
        context.flags = (context.flags & ~2U) | 4U;
        context.moveState = 0;
        context.phase = 1;
    } else if (context.currentAreaPoint != context.pendingAreaPoint || (context.flags & 0x300U)) {
        if (context.routeFlags & 0x20U)
            context.startNode = context.endNode = 0;
        else {
            context.startNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.currentAreaPoint) + 4));
            context.endNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.pendingAreaPoint) + 4));
        }
        context.flags &= ~4U;
        context.phase = 1;
    } else {
        context.phase = 9;
        context.flags &= ~4U;
    }
}

void BuildNpcRoute(NpcRouteStateContext &context) {
    if (npcRouteBuildCount >= 8 && !(context.flags & 0x100U))
        return;
    ++npcRouteBuildCount;
    if (!BuildNpcRouteNodePath(context)) {
        context.flags |= 0x80U;
        context.phase = 9;
    } else
        context.phase = 2;
}

void BindNpcRouteWindow(NpcRouteStateContext &context) {
    if ((context.flags & 4U) || (context.routeFlags & 0x20U)) {
        NpcRouteStateContext copy = context;
        context.lowParameter = static_cast<std::uint16_t>(FindNearestNpcParameter(copy, context.localPosition));
    } else {
        context.lowParameter = GetNpcRouteConnectionParameter(
            GetNpcAreaPoint(context.currentAreaPoint), context.routeNodes[0], context.routeCount > 1 ? context.routeNodes[1] : 0xffff, false);
    }
    if (context.routeFlags & 0x20U) {
        NpcRouteStateContext copy = context;
        context.highParameter = static_cast<std::uint16_t>(FindNearestNpcParameter(copy, context.targetPosition));
    } else {
        context.highParameter = static_cast<std::uint16_t>((context.pointCount - 3) * 64 -
            GetNpcRouteConnectionParameter(GetNpcAreaPoint(context.pendingAreaPoint), context.routeNodes[context.routeCount - 1],
                context.routeCount > 1 ? context.routeNodes[context.routeCount - 2] : 0xffff, true));
    }
    if (context.lowParameter > context.highParameter) {
        if (!(context.flags & 0x2000U)) {
            context.startNode ^= 0x8000;
            context.flags |= 0x2000U;
            context.phase = 1;
            return;
        }
        context.lowParameter = context.highParameter;
    }
    context.startNode = context.routeNodes[0];
    context.endNode = context.routeNodes[context.routeCount - 1];
    context.stride = 1;
    context.extraStride = 0;
    context.parameter = context.lowParameter;
    UpdateNpcRouteWindow(context, context.parameter);
    SampleNpcRouteWindow(context, &context.position, false);
    if (context.stateTable[context.activeNode & 0x7fff].flags & 0x400U)
        context.owner->flags |= 0x100000U;
    else {
        context.owner->flags &= ~0x100000U;
        context.owner->motionAttachments[0].flags &= ~0x40U;
    }
    context.phase = 6;
    if (!(context.flags & 0x204U) && !(context.routeFlags & 0x20U) && (context.owner->flags & 0x800U)) {
        const std::byte *const point = GetNpcAreaPoint(context.currentAreaPoint);
        if (!ReadU32(point + 0x40)) {
            context.otherName = shen_name{ReadU32(point + 0x30)};
            if (context.otherName.name)
                context.phase = 5;
        }
    }
    if (context.routeFlags & 1U) {
        context.flags |= 8U;
        context.savedPhase = context.phase;
        context.phase = 4;
        if ((context.flags & 0x8000U) && context.owner->node)
            InitiateCleanupForSceneChara(context.scriptOwner);
    }
    context.flags |= 2U;
    context.moveState = 0;
}

void TickNpcPausedRoute(NpcRouteStateContext &context) {
    if (!(context.routeFlags & 1U)) {
        if (context.flags & 0x8000U) {
            if (context.owner->node)
                InitiateCleanupForSceneChara(context.scriptOwner);
            context.flags &= ~0x8000U;
        }

        context.flags &= ~0x28U;
        context.phase = context.savedPhase;
        return;
    }

    if (!(context.flags & 0x8000U) || (context.routeFlags & 0x40000U))
        return;

    if (!(context.flags & 0x20U) && context.owner->node) {
        bool wait = false;
        if (mtwkSetExtraFlag(context.motionWork, 58, 1))
            wait = ++context.owner->counter416 <= 8;
        if (!wait && g_playerRuntime.character) {
            Vector3 head{};
            mtwkGetBonePosition(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(g_playerRuntime.character), MB_Eyes, &head);
            mtwkUpdateWithPosition(context.motionWork, &head, 1);
            context.flags |= 0x20U;
        }
    }
    DateTime date;
    shenGetCurrentDateTime(date);
    if (120 * (static_cast<std::int32_t>(date.minute) + 60 * date.hour) - context.waitStartTime > 1800) {
        if (context.owner->node)
            mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        context.flags &= ~0x20U;
        context.routeFlags &= ~1U;
        if (!(context.flags & 8U))
            context.flags &= ~0x8000U;
        NpcRouteLiveEntry &owner = *context.owner;
        if (context.flags & 0x100U) {
            if (owner.eyeControlMode) {
                owner.field338 = static_cast<std::uint8_t>(-owner.eyeControlMode);
                owner.eyeControlMode = 10;
            } else {
                owner.field338 = static_cast<std::uint8_t>(owner.routeDirectionMode);
                owner.routeDirectionMode = 10;
            }
        } else if (!(owner.flags & 0x10U)) {
            if (!(owner.field338 & 0x80U))
                owner.routeDirectionMode = 12;
            else
                owner.eyeControlMode = 12;
        }
    }
}

void CompleteNpcDoorEgress(NpcRouteStateContext &context, bool snap) {
    if (snap) {
        context.localPosition = context.doorPosition;
        StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
    } else if (context.owner->node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node))
            control->flags = (control->flags & ~0x804U) | 4U;
        SetCharacterType(context.owner->node, CharacterTypeDefault);
    }
    if (context.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner);
        context.otherName = {};
        context.owner->flags &= ~0x100U;
    }
    context.phase = 6;
    context.moveState = 0;
    if (!(context.stateTable[context.activeNode & 0x7fff].flags & 0x400U)) {
        context.owner->flags &= ~0x100000U;
        context.owner->motionAttachments[0].flags &= ~0x40U;
    }
}

void BeginNpcDoorEgressMotion(NpcRouteStateContext &context) {
    context.moveState = 1;
    context.doorMotion =
        static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.owner->node, &context.scenePosition));
    Vector3 track{};
    SampleNativeMotionRoot(context.doorMotion, GetCharacterBoneId(context.owner->node), 15.0f, track);

    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(context.targetYaw, 0, 0);
    track = TransformNpcPoint(track);
    Matrix4x4::Pop(1);
    context.sceneTarget.x += track.x;
    context.sceneTarget.z += track.z;
    context.sceneTarget = NpcRouteAreaToScene(context.sceneTarget);
    if (!FindNpcIntersectionInScene(context.sceneTarget, context.moveFlags)) {
        Vector3 floor = NpcRouteAreaToScene(context.doorPosition);
        FindNpcIntersectionInScene(floor, context.moveFlags);
        context.scenePosition.y = floor.y;
        shenApplyTranslationToAll(&context.owner->node->entity.node, context.scenePosition);
        context.localPosition = NpcAreaPosition(context.scenePosition);
    }
    RequestNpcRouteMotion(context, true);
}

void TickNpcDoorEgress(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    switch (context.moveState) {
    case 0: {
        owner.flags &= ~0x800U;
        if (!ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.moveTarget)) {
            CompleteNpcDoorEgress(context, false);
            return;
        }
        context.moveTarget = NpcAreaPosition(context.moveTarget);
        context.targetYaw = g_doorIngressRotation.y;
        context.savedSplinePosition = GetDoorIngressOrigin();
        context.sceneTarget = context.moveTarget;
        ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.doorPosition);
        context.doorPosition = NpcAreaPosition(context.doorPosition);
        context.savedSplinePosition = NpcAreaPosition(context.savedSplinePosition);
        const float x = context.position.x - context.moveTarget.x;
        const float z = context.position.z - context.moveTarget.z;
        if (std::abs(x) >= 0.001f || std::abs(z) >= 0.001f) {
            StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(x, z));
            if (NpcPositionInFrame(context.doorPosition, context.localPosition, ReadNpcEulerYaw(context.savedEuler)).z > 0.0f) {
                CompleteNpcDoorEgress(context, false);
                return;
            }
        }
        owner.flags |= 0x100000U;
        if (!ClaimDoor(context.otherName, context.scriptOwner)) {
            context.moveState = 3;
            owner.field342 = 0;
        } else if (owner.node)
            BeginNpcDoorEgressMotion(context);
        else {
            owner.flags |= 0x1000000U;
            context.doorWaitFrames = 150;
            context.moveState = 7;
        }
        break;
    }
    case 1:
        if (!owner.node) {
            CompleteNpcDoorEgress(context, true);
            return;
        }
        if (UpdateNpcObstructionState(context, QueryNpcMoveObstruction(owner, context.moveTarget, true, 0x4000, 0.44999999f, 1.5f) != 0))
            return;
        if (context.routeFlags & 0x800U) {
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
            mtwkSetTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (!(context.flags & 0x1000U) && !mtwkSetExtraFlag(context.motionWork, 58, 1)) {
            StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
            owner.flags |= 0x100U;
            context.moveState = 4;
            BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner, context.doorMotion, 14.0f, context.motionScale * 1.1f);
            mtwkSetSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
        }
        break;
    case 3:
        if (ClaimDoor(context.otherName, context.scriptOwner)) {
            if (owner.node)
                context.moveState = 6;
            else {
                owner.flags |= 0x1000000U;
                context.doorWaitFrames = 150;
                context.moveState = 7;
            }
        } else if (static_cast<std::int16_t>(++owner.field342) > 1800) {
            if (!owner.node) {
                owner.field342 = 0;
                CompleteNpcDoorEgress(context, true);
            } else {
                RequestNpcStreamOut(owner, 2);
                owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
            }
        }
        break;
    case 4:
        if (!owner.node)
            CompleteNpcDoorEgress(context, true);
        else if (mtwkSetRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody))
            CompleteNpcDoorEgress(context, false);
        break;
    case 6:
        if (owner.node)
            BeginNpcDoorEgressMotion(context);
        else
            CompleteNpcDoorEgress(context, true);
        break;
    case 7:
        if (owner.node) {
            context.doorWaitFrames = 0;
            context.moveState = 6;
        } else if (context.doorWaitFrames-- == 1) {
            owner.flags &= ~0x1000000U;
            CompleteNpcDoorEgress(context, true);
        }
        break;
    }
}

void TickNpcRouteApproach(NpcRouteStateContext &context) {
    if (!context.moveState) {
        const std::uint16_t parameter = context.parameter;
        ++context.parameter;
        Vector3 tangent;
        SampleNpcRouteWindow(context, &tangent, false);
        context.parameter = parameter;
        context.tangentYaw = NpcRouteYaw(tangent.x - context.position.x, tangent.z - context.position.z);
        LoadNpcRouteLanes(context);
        const bool outside = FitNpcRouteLane(context, 0.0f);
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, context.laneOffset);
        StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z));
        context.sceneTarget = NpcRouteAreaToScene(context.moveTarget);
        context.targetYaw = context.tangentYaw;
        if ((outside || (context.owner->flags & 0x8000000U)) && !(context.flags & 0x20000U)) {
            RequestNpcRouteMotion(context, true);
            context.moveState = 1;
            return;
        }
        if (context.owner->node) {
            mtwkSetSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            Vector3 position = NpcRouteAreaToScene(context.localPosition);
            if (context.stairFlags) {
                if (context.routeFlags & 0x4040U)
                    FindNpcIntersectionInScene(position, context.moveFlags);
                position.y -= 0.22499999f;
                Vector3 origin;
                std::int32_t yaw;
                if (flddQueryStairOrigin(position, origin, yaw)) {
                    context.moveState = 5;
                    mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 13, 0);
                    mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
                    return;
                }
            }
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 5, 2);
            if (context.flags & 0x10U)
                mtwkCopyTransition(context.motionWork, nullptr, 0, 8, 0);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        }
    } else if (context.moveState == 5) {
        if (context.owner->node && mtwkSetExtraFlag(context.motionWork, 58, 1))
            return;
    } else if (context.moveState == 1) {
        if (!context.owner->node) {
            if (!AdvanceUnstreamedNpc(context))
                return;
        } else {
            const bool blocked = !(context.owner->flags & 0x8000000U) && !(context.routeFlags & 0x10U) &&
                (QueryNpcMoveObstruction(*context.owner, context.moveTarget, false, 0x4000, context.owner->streamRadius, -1.0f) & 1U);
            if (UpdateNpcObstructionState(context, blocked))
                return;
            if (context.routeFlags & 0x800U) {
                mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
                mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
            }
            if (context.flags & 0x1000U)
                return;
            if (mtwkCopyTransition(context.motionWork, nullptr, 0, 3, 0)) {
                RequestNpcRouteMotion(context, true);
                return;
            }
            if (mtwkSetExtraFlag(context.motionWork, 58, 1))
                return;
        }
    } else
        return;
    context.phase = 7;
    context.moveState = 2;
    StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
    context.movementProgress = 0.0f;
    if (context.flags & 0x20000U)
        RequestNpcRouteMotion(context, true);
    context.routeFlags |= 0x800000U;
    context.flags |= 0x20000U;
}

void TickNpcRouteTraversal(NpcRouteStateContext &context) {
    if (context.moveState == 1) {
        TickNpcRouteMovement(context);
        return;
    }
    if (context.moveState != 2)
        return;
    UpdateNpcRouteWindow(context, context.parameter, true);
    if (context.activeSegment != context.previousSegment) {
        context.stride = ComputeNpcRouteStride(context);
        LoadNpcRouteLanes(context);
        context.previousSegment = context.activeSegment;
        if (context.crossingPoint == 3) {
            if (context.stateTable[context.nextNode & 0x7fff].flags & 0x400U)
                context.owner->flags |= 0x100000U;
            else {
                context.owner->flags &= ~0x100000U;
                context.owner->motionAttachments[0].flags &= ~0x40U;
            }
        }
    }
    context.parameter = static_cast<std::uint16_t>(context.parameter + context.stride + context.extraStride);
    if (!context.avoidanceResult) {
        context.laneOffset -= context.laneStep;
        if (context.laneOffset > context.laneHigh)
            context.laneOffset = context.laneHigh;
        else if (context.laneLow > context.laneOffset)
            context.laneOffset = context.laneLow;
    }
    InterpolateNpcRouteLanes(context, true);
    if (context.highParameter >= context.parameter) {
        const Vector3 previous = context.position;
        Vector3 sample;
        const bool valid = SampleNpcRouteWindow(context, &sample, true) != 0;
        context.position = sample;
        context.tangentYaw = NpcRouteYaw(sample.x - previous.x, sample.z - previous.z);
        context.moveTarget = OffsetNpcRoutePosition(sample, context.tangentYaw, context.laneOffset);
        if (valid) {
            context.moveState = 1;
            TickNpcRouteMovement(context);
            return;
        }
    }
    if (context.flags & 0x400U) {
        context.parameter = context.highParameter;
        context.moveState = 1;
        TickNpcRouteMovement(context);
        return;
    }
    if (!(context.flags & 0x40000U)) {
        context.routeFlags &= ~0x2000800U;
        context.flags &= ~0x101000U;
    }
    if (context.owner->node && (context.flags & 0x200000U))
        RestoreNpcGroundControl(context);
    context.flags &= ~0x20000U;
    context.phase = 8;
    context.moveState = 0;
    if (context.flags & 0x100U) {
        context.phase = 9;
        const std::int32_t yaw = NpcRouteYaw(npcPlayerPosition.x - context.scenePosition.x, npcPlayerPosition.z - context.scenePosition.z);
        const std::int32_t difference = static_cast<std::int16_t>(yaw - ReadNpcEulerYaw(context.savedEuler));
        const std::int32_t correction = difference < 0 ? std::min(difference + 20024, 0) : std::max(difference - 20024, 0);
        const std::uint16_t transitionYaw = static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + correction);
        StoreNpcEulerYaw(context.savedEuler, yaw);
        context.owner->counter416 = 0;
        if (context.owner->node) {
            mtwkCopyTransition(context.motionWork, nullptr, transitionYaw, 13, 0);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        }
    } else if (context.owner->node && (context.flags & 0x20U)) {
        mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        context.flags &= ~0x20U;
    }
}

void CompleteNpcDoorIngress(NpcRouteStateContext &context, bool snap) {
    if (snap)
        context.localPosition = context.doorPosition;
    if (context.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner);
        context.otherName = {};
        context.owner->flags = (context.owner->flags & ~0x900U) | 0x800U;
        if (context.owner->node)
            SetCharacterType(context.owner->node, 3);
    }
    context.phase = 9;
}

void TickNpcDoorIngress(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    switch (context.moveState) {
    case 0: {
        context.moveState = 1;
        if (!(context.routeFlags & 0x20U)) {
            const std::byte *const point = GetNpcAreaPoint(context.pendingAreaPoint);
            if (ReadU32(point + 0x40)) {
                CompleteNpcDoorIngress(context, false);
                return;
            }
            context.otherName = shen_name{ReadU32(point + 0x30)};
            if (context.otherName.name && ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.moveTarget)) {
                context.targetYaw = g_doorIngressRotation.y;
                ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.doorPosition);
                if (owner.node)
                    mtwkUpdateWithPosition(context.motionWork, &context.doorPosition, 0);
                context.doorPosition = NpcAreaPosition(context.doorPosition);
                if (ClaimDoor(context.otherName, context.scriptOwner))
                    owner.flags |= 0x100U;
                else {
                    context.moveState = 3;
                    owner.field342 = 0;
                }
                context.sceneTarget = context.moveTarget;
                context.moveTarget = NpcAreaPosition(context.moveTarget);
                owner.flags |= 0x100000U;
            } else {
                context.otherName = {};
                const std::int16_t route = static_cast<std::int16_t>(ReadU16(point + 4));
                Vector3 tangent;
                SampleNpcRouteNode(static_cast<std::uint16_t>(route), static_cast<float>(static_cast<std::int16_t>(ReadU16(point + 8))) * 0.015625f, tangent);
                Vector3 target;
                std::memcpy(&target, point + 0x34, sizeof(target));
                context.targetYaw = NpcRouteYaw(tangent.x - target.x, tangent.z - target.z) + 0x8000;
                context.moveTarget = context.doorPosition = target;
                context.sceneTarget = NpcRouteAreaToScene(target);
            }
        } else {
            context.targetYaw = static_cast<std::uint32_t>(context.arrivalYaw) | (static_cast<std::uint32_t>(context.field10a) << 16);
            context.moveTarget = context.doorPosition = context.targetPosition;
            context.otherName = {};
            context.sceneTarget = NpcRouteAreaToScene(context.targetPosition);
        }
        StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z));
        if (context.routeFlags & 0x20000U) {
            context.routeFlags &= ~0x20000U;
            context.targetYaw = static_cast<std::uint32_t>(context.arrivalYaw) | (static_cast<std::uint32_t>(context.field10a) << 16);
        }
        RequestNpcRouteMotion(context, true);
        return;
    }
    case 1:
        if (!owner.node) {
            if (AdvanceUnstreamedNpc(context))
                CompleteNpcDoorIngress(context, context.otherName.name != 0);
            return;
        }
        if (UpdateNpcObstructionState(context,
                !(owner.flags & 0x8000000U) && !(context.routeFlags & 0x10U) &&
                    QueryNpcMoveObstruction(owner, context.doorPosition, false, 0x4000, owner.streamRadius, -1.0f) != 0))
            return;
        if (context.routeFlags & 0x800U) {
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
            if (context.otherName.name)
                mtwkSetTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (context.flags & 0x1000U)
            return;
        if (mtwkCopyTransition(context.motionWork, nullptr, 0, 3, 0)) {
            RequestNpcRouteMotion(context, true);
            return;
        }
        if (mtwkSetExtraFlag(context.motionWork, 58, 1))
            return;
        if (!context.otherName.name) {
            CompleteNpcDoorIngress(context, false);
            return;
        }
        StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
        context.moveState = 4;
        if (CharacterControlModel *const control = FindCharacterControlModel(owner.node))
            control->flags = (control->flags | 4U) & ~0x800U;
        BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner, context.doorMotion, 14.0f, context.motionScale * 1.1f);
        mtwkSetSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
        mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        return;
    case 3:
        if (ClaimDoor(context.otherName, context.scriptOwner)) {
            context.moveState = 1;
            RequestNpcRouteMotion(context, true);
            owner.flags |= 0x100U;
            owner.field342 = 0;
        } else if (static_cast<std::int16_t>(++owner.field342) > 1800) {
            if (owner.node) {
                RequestNpcStreamOut(owner, 2);
                owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
            } else {
                owner.field342 = 0;
                CompleteNpcDoorIngress(context, true);
            }
        }
        return;
    case 4:
        if (!owner.node) {
            CompleteNpcDoorIngress(context, true);
            return;
        }
        if (mtwkSetRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody)) {
            Vector3 floor = NpcRouteAreaToScene(context.localPosition);
            if (FindNpcIntersectionInScene(floor, context.moveFlags) || (owner.flags & 0x200U)) {
                if (CharacterControlModel *const control = FindCharacterControlModel(owner.node))
                    control->flags = (control->flags & ~4U) | 0x800U;
            } else
                RequestNpcStreamOut(owner, 0);
            CompleteNpcDoorIngress(context, false);
        }
        return;
    }
}

bool FinishNpcRoute(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    if (context.routeFlags & 0x10U) {
        const std::uint32_t area = ReadU32(GetNpcAreaPoint(context.pendingAreaPoint)) & 0xffffffU;
        if (area == 0x4c4f47U || area == 0x4d4f44U)
            owner.flags |= 0x100000U;
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != context.scriptOwner.name)
                continue;
            entry.name = SHEN_NAME(MOVE);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            break;
        }
    }
    const std::uint32_t flags = context.flags;
    context.currentAreaPoint = context.pendingAreaPoint;
    context.phase = 0;
    if (flags & 0x100U) {
        context.flags = context.routeFlags = 0;
        InitializeNpcRouteRequest(context, owner.stateContext.currentAreaPoint, owner.pendingAreaPoint);
        SetNpcLocomotionScale(context, owner.motionSpeed);
        context.flags |= 0x200U;
        if (flags & 0x4000U) {
            context.flags |= 0x4000U;
            context.routeFlags |= 0x20000U;
        }
        if (!(owner.field338 & 0x80U))
            owner.routeDirectionMode = 11;
        else
            owner.eyeControlMode = 11;
        context.routeFlags |= 1U;
        context.flags |= 0x8000U;
        DateTime date;
        shenGetCurrentDateTime(date);
        context.waitStartTime = 120 * (static_cast<std::int32_t>(date.minute) + 60 * date.hour);
        return false;
    }
    if (context.routeFlags & 0x4000000U) {
        context.flags = 0;
        context.routeFlags = 0;
        InitializeNpcRouteRequest(context, owner.stateContext.currentAreaPoint, owner.pendingAreaPoint);
        SetNpcLocomotionScale(context, owner.motionSpeed);
        if (flags & 0x10U)
            SetNpcRouteMotionState(context, 1);
        return false;
    }
    context.routeFlags = 0;
    if (flags & 0x200U) {
        context.flags &= ~0x200U;
        context.routeFlags = 0x400U;
        owner.scheduleProgress = owner.field32f;
        owner.liveState = owner.field331;
        if (!(owner.field338 & 0x80U))
            owner.routeDirectionMode = static_cast<std::int8_t>(owner.field338);
        else
            owner.eyeControlMode = static_cast<std::int8_t>(-static_cast<std::int8_t>(owner.field338));
        if (context.flags & 0x4000U) {
            context.flags &= ~0x4000U;
            owner.currentCommand = owner.branchCommandStart;
            owner.scheduleProgress = 1;
        }
        context.moveFlags = (owner.packedMoveFlags & 0x100ffU) | (static_cast<std::uint32_t>(context.floor) << 8);
        if (npcScriptRouteOwner == &owner) {
            owner.flags &= ~0x200U;
            npcScriptRouteOwner = nullptr;
            UpdateNpcScheduleSeedSlot({}, owner.name, 0);
        }
    }
    context.flags = 0;
    return true;
}

void UpdateLinkedNpcRoute(NpcRouteStateContext &context) {
    if (context.phase != 7)
        return;
    NpcRouteLiveEntry &leader = *context.linkedRoute.character;
    std::int32_t request = 0;
    float distance = 0.0f;
    if (leader.stateContext.phase == 6)
        request = 1;
    else if (leader.stateContext.phase == 7) {
        const float x = leader.stateContext.localPosition.x - context.localPosition.x;
        const float z = leader.stateContext.localPosition.z - context.localPosition.z;
        distance = std::sqrt(x * x + z * z);
        if (distance > context.linkedRoute.leashDistance)
            request = leader.stateContext.parameter > context.parameter ? 2 : 1;
    }
    if (request == 1) {
        context.routeFlags |= 0x2000800U;
        if (context.owner->node && leader.node) {
            Vector3 head{};
            mtwkGetBonePosition(leader.motionWork, MB_Eyes, &head);
            mtwkUpdateWithPosition(context.motionWork, &head, 0);
        }
    } else if (request == 2) {
        leader.stateContext.routeFlags |= 0x2000800U;
        if (context.owner->node && leader.node) {
            Vector3 head{};
            mtwkGetBonePosition(context.motionWork, MB_Eyes, &head);
            mtwkUpdateWithPosition(leader.motionWork, &head, 0);
        }
    } else if (context.linkedRoute.leashDistance * 0.5f <= distance)
        return;
    if (request != 1 && (context.routeFlags & 0x2000000U) && !(context.owner->flags & 0x10U)) {
        context.routeFlags &= ~0x2000000U;
        if (context.owner->node)
            mtwkUpdateWithPosition(context.motionWork, nullptr, 0);
        SetNpcRouteMotionState(context, 3);
    }
    if (request != 2 && (leader.stateContext.routeFlags & 0x2000000U) && !(leader.flags & 0x10U)) {
        leader.stateContext.routeFlags &= ~0x2000000U;
        if (leader.node)
            mtwkUpdateWithPosition(leader.motionWork, nullptr, 0);
        SetNpcRouteMotionState(leader.stateContext, 3);
    }
}

// Restarts avoidance, the motion profile and stride, then takes a deferred script run-mode change.
void RestartNpcAreaRouteContext(NpcRouteStateContext &context) {
    context.routeFlags |= 0x40U;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    const bool profileWasReady = (context.flags & 0x40U) != 0;
    InitializeNpcLocomotionProfile(context);
    if (profileWasReady && (context.flags & 2U))
        context.stride = ComputeNpcRouteStride(context);
    if (context.routeFlags & 0x1000U)
        RefreshNpcScriptRouteMotion(context);
    context.flags &= 0xff9efbffU;
}

void InitializeStreamedNpcRoute(NpcRouteLiveEntry &entry, bool warm) {
    entry.flags = (entry.flags & ~3U) | NRSF_Live;
    entry.node = ctrlGetScenarioChara(entry.name);
    entry.motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(entry.node);
    npcSlotFlags[entry.poolIndex] |= 1U;
    entry.streamRadius = 0.30000001f;
    NpcRouteStateContext &context = entry.stateContext;
    context.motionWork = entry.motionWork;
    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    if (entry.eyeControlProfile)
        HandleNpcEyeControl(entry);
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        if (!(entry.flags & 0x200U)) {
            if ((npcSchedule.floor != -1000 && (entry.packedMoveFlags & 0xff00U)) || (entry.flags & 0x2000U))
                control->flags &= ~CCMF_ApplyGravity;
            if (entry.flags & 0x8000000U)
                control->flags = (control->flags & ~0x804U) | 0x800U;
        }
    }
    if (entry.flags & 0x800U)
        SetCharacterType(entry.node, 3);
    if (warm)
        return;
    if (entry.flags & 0xa000U)
        entry.localPosition.y = entry.forcedY;
    else if (!(entry.flags & 0x1000000U))
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
    charaSetWorldRot(entry.node, rotation);
    mtwkSetEnabled(entry.motionWork, true);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    if (entry.name.name == SHEN_NAME(REN_))
        mtwkSetTimeRelated(entry.motionWork, 1, 0, 0xa204);
    entry.flags &= ~0x1000000U;
    if (entry.flags & 8U) {
        if (entry.liveState == 1) {
            entry.streamRadius = 0.34999999f;
            if (context.routeFlags & 0x10U) {
                RestartNpcAreaRouteContext(context);
                RequestNpcRouteMotion(context, false);
                return;
            }
        }
        mtwkRequestMotion(entry.motionWork, SelectNpcIdleMotion(entry), MotionRegion::LowerBody);
        return;
    }
    switch (entry.liveState) {
    case 1:
        entry.streamRadius = 0.34999999f;
        RestartNpcAreaRouteContext(context);
        RequestNpcRouteMotion(context, false);
        return;
    case 3:
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        charaSetWorldRot(entry.node, rotation);
        if (!(entry.flags & 0x400U))
            break;
        mtwkSetRequestedRootMode(entry.motionWork, 0);
        ResumeNpcTimedMotion(entry);
        return;
    case 4:
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CCMF_ApplyGravity | CCMF_EvaluateMotion);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        SetCharacterType(entry.node, 3);
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        return;
    case 6:
        entry.currentMotionId = SelectNpcSeatedIdleMotion(entry);
        break;
    case 7:
        mtwkSetDrawOverrideData(entry.motionWork, nullptr);
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CCMF_EvaluateMotion | CCMF_VoxelSweep);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        if (entry.flags & 0x40000U) {
            const std::uint32_t frames = npcMotionClips[entry.clipOverrideIndex].frameCount;
            if (frames)
                entry.animationFrame =
                    static_cast<float>((npcFrameCount + 2 * static_cast<std::int8_t>(entry.recordBase[8])) % static_cast<std::int32_t>(frames));
        }
        SetNpcHumanVisibilityChannels(entry, false);
        return;
    case 8: {
        if (entry.streamCost == 1) {
            mtwkSetDrawOverrideData(entry.motionWork, nullptr);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~(CCMF_EvaluateMotion | CCMF_VoxelSweep);
            return;
        }
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
        return;
    }
    default:
        entry.currentMotionId = SelectNpcIdleMotion(entry);
        break;
    }
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
}

std::int64_t NpcRouteScheduleCallback(NpcRouteLiveEntry *entry) {
    NpcRouteStateContext &context = entry->stateContext;
    if (!(context.flags & 1U))
        return 0;
    if ((context.flags & 0x300U) || context.otherName.name)
        return 1;
    const std::uint32_t opcode = ReadU32(entry->currentCommand);
    switch (opcode) {
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 18:
    case 19:
    case 32:
    case 40:
        if (context.flags & 0x400U) {
            if (context.flags & 0x1000U)
                return 0;
            SetNpcRouteMotionState(context, 2);
            return 1;
        }
        if (opcode != 4 && opcode != 18)
            ReleaseNpcRouteControl(&context, {}, ReadNpcEulerYaw(entry->euler));
        return 0;
    default:
        return -1;
    }
}

bool BeginNpcAreaRoute(NpcRouteLiveEntry &entry, std::uint16_t targetPoint, float authoredScale) {
    if (!GetNpcAreaPoint(entry.currentAreaPoint) || !GetNpcAreaPoint(targetPoint))
        return false;
    InitializeNpcRouteRequest(entry.stateContext, entry.currentAreaPoint, targetPoint);
    if (entry.flags & 0x40U)
        entry.stateContext.routeFlags &= ~6U;
    entry.stateContext.scenePosition = entry.localPosition;
    entry.stateContext.localPosition = NpcAreaPosition(entry.localPosition);
    entry.stateContext.savedEuler = entry.euler;
    entry.stateContext.moveFlags = entry.packedMoveFlags;
    entry.motionSpeed = authoredScale;
    entry.flags &= ~0x8000U;
    SetNpcLocomotionScale(entry.stateContext, authoredScale);
    entry.pendingAreaPoint = targetPoint;
    entry.stateCallback = NpcRouteScheduleCallback;
    entry.liveState = 1;
    entry.scheduleProgress = 3;
    const std::uint32_t initial = ReadU32(entry.recordBase + 0x0c);
    entry.routeDirectionMode = static_cast<std::int8_t>(targetPoint == initial ? 4 : 2);
    return true;
}
} // namespace npc_package_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void TickNpcMotionClip(NpcRouteLiveEntry &entry) {
    const NpcMotionClip &clip = npcMotionClips[entry.clipOverrideIndex];
    if (!clip.frames)
        return;
    entry.animationFrame += HLib::GetScaledDeltaTick();
    if (entry.animationFrame >= static_cast<float>(clip.frameCount))
        entry.animationFrame -= static_cast<float>(clip.frameCount);
    if (!entry.node)
        return;
    const bool visible = camCtrlIsInsideDepthScaledRect(entry.localPosition, entry.node->radius);
    entry.node->renderFlags = static_cast<std::uint8_t>((entry.node->renderFlags & ~CRF_InRange) | (visible ? 0x10U : 0U));
    const Vector3 viewPosition = TransformNpcPoint(entry.localPosition);
    entry.node->viewPosition = viewPosition;
    const float magnitude = std::sqrt(viewPosition.x * viewPosition.x + viewPosition.y * viewPosition.y + viewPosition.z * viewPosition.z);
    entry.node->drawDistance = magnitude < 0.0000099999997f ? 0.0f : magnitude;
    mtwkSetDrawOverrideData(entry.motionWork, clip.frames + static_cast<std::int32_t>(entry.animationFrame) * clip.boneCount);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool SetNpcMotionAttachment(NpcRouteLiveEntry &entry, std::uint16_t motion, std::uint8_t flags, std::int32_t slot) {
    if (!motion)
        return false;
    NpcMotionAttachment &attachment = entry.motionAttachments[slot];
    attachment.motion = motion;
    attachment.flags = (attachment.flags & 0xffc1U) | (flags & 0x3eU);
    std::uint8_t remaining = flags & 0x1eU;
    for (std::int32_t index = 0; index < slot; ++index) {
        const NpcMotionAttachment &prior = entry.motionAttachments[index];
        if (prior.motion)
            remaining &= ~(prior.flags & 0x1eU);
    }
    return remaining != 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ClearNpcMotionAttachment(NpcRouteLiveEntry &entry, std::int32_t slot) {
    if (slot < 3)
        entry.motionAttachments[slot].flags = entry.motionAttachments[slot].motion = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcScriptRouteSpeed(NpcRouteStateContextPrefix *context, shen_name name, float scale) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    SetNpcLocomotionScale(context, shen_name(), scale);
}

#else

void SetNpcScriptRouteSpeed(NpcRouteStateContext *context, shen_name character, float speed) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    SetNpcLocomotionScale(*context, speed);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int64_t SetNpcRouteYaw(shen_name character, std::int32_t yaw) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        std::memcpy(&entry.euler.y, &yaw, sizeof(yaw));
        return 1;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int64_t PlaceNpcAtAreaPoint(shen_name character, shen_name areaPoint, const Vector3 *position, std::int32_t yaw) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (areaPoint.name) {
            std::size_t point = 0;
            while (point < npcSchedule.areaPointCount) {
                const std::uint32_t name = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(point)));
                if (name == areaPoint.name)
                    break;
                if (!name)
                    return 0;
                ++point;
            }
            if (point == npcSchedule.areaPointCount)
                return 0;
            entry.currentAreaPoint = static_cast<std::uint16_t>(point);
        }
        std::memcpy(&entry.euler.y, &yaw, sizeof(yaw));
        entry.scheduleProgress = 3;
        entry.liveState = 0;
        RestoreNpcAreaPointSeedPosition(entry);
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
        entry.worldPosition = NpcAreaPosition(entry.localPosition);
        if (entry.stateContext.flags & 1U)
            ReleaseNpcRouteControl(&entry.stateContext, {}, yaw);
        if (!areaPoint.name) {
            entry.localPosition = position ? *position : Vector3{};
            entry.worldPosition = NpcAreaPosition(entry.localPosition);
        }
        if (entry.node) {
            mtwkCopyTransition(entry.motionWork, nullptr, 0, 1, 0);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            mtwkSetRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
            mtwkSetExtraFlag(entry.motionWork, 14, 0);
            entry.currentMotionId = SelectNpcIdleMotion(entry);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
        }
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeNpcAreaPointTable(NpcAreaPoint *points) {
    npcAreaPoints = points;
    std::int32_t count = 0;
    std::uint32_t floorFlags;
    if (npcScheduleFloor != -1000) {
        floorFlags = static_cast<std::uint32_t>(GetQronNextStream());
        floorFlags <<= 8;
    } else {
        floorFlags = 0;
    }
    for (NpcAreaPoint *point = npcAreaPoints; point->area; ++point) {
        Vector3 position = point->position;
        position.x -= npcRouteOrigin.x;
        position.z -= npcRouteOrigin.z;
        FindNpcIntersectionInScene(position, floorFlags | (npcRouteNodes[point->routeIndex].flags & 0xffU));
        point->position.y = position.y;
        if (point->routeStateOffset) {
            ++count;
            point->routeStateOffset =
                static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(npcAreaPoints) + static_cast<std::uint32_t>(point->routeStateOffset));
        }
    }
    PrepareNpcAreaRouteStore(count);
    std::int32_t index = 0;
    for (NpcAreaPoint *point = npcAreaPoints; point->area; ++point, ++index)
        if (point->routeStateOffset)
            RegisterNpcAreaRouteState(reinterpret_cast<NpcAreaRouteState *>(static_cast<std::uintptr_t>(point->routeStateOffset)), index);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

const Vector3 *GetNpcRouteNodePoint(const NpcRouteSpline *routes, std::int32_t node) {
    return GetNpcRouteStartPoint(routes, node);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const Vector3 *GetNpcRouteEndPoint(const NpcRouteSpline *routes, std::int16_t node) {
    if (~node & 0x8000)
        node |= 0x8000;
    else
        node &= 0x7fff;
    return GetNpcRouteStartPoint(routes, node);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

NpcAreaPoint *GetNpcAreaPoint(std::int32_t point);

// @verified - d
std::int32_t GetNpcRouteStartParameter(NpcRouteStateContextPrefix *context, std::int32_t point) {
    std::int32_t slot = 0;
    const NpcAreaPoint *const area = GetNpcAreaPoint(point);
    const NpcRouteSpline *const route = &context->stateTable[area->routeIndex];
    if (~context->routePath[0] & 0x8000) {
        if (context->routePath[1] != -1)
            for (slot = 0; slot < 4; slot++)
                if (route->startConnections[slot] == context->routePath[1]) {
                    slot++;
                    break;
                }
        return area->routeParameters[0][slot];
    }
    if (context->routePath[1] != -1)
        for (slot = 0; slot < 4; slot++)
            if (route->startConnections[slot + 4] == context->routePath[1]) {
                slot++;
                break;
            }
    return area->routeParameters[1][slot];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteEndParameter(NpcRouteStateContextPrefix *context, std::int32_t point) {
    std::int32_t slot = 0;
    std::int16_t parameter;
    const NpcAreaPoint *const area = GetNpcAreaPoint(point);
    const NpcRouteSpline *const route = &context->stateTable[area->routeIndex];
    if (~context->routePath[context->routeCount - 1] & 0x8000) {
        if (context->routePath[1] != -1) {
            const std::int16_t previous = context->routePath[context->routeCount - 2] & 0x8000 ? context->routePath[context->routeCount - 2] & 0x7fff :
                                                                                                 context->routePath[context->routeCount - 2] | 0x8000;
            for (slot = 0; slot < 4; slot++)
                if (route->startConnections[slot + 4] == previous) {
                    slot++;
                    break;
                }
        }
        parameter = area->routeParameters[2][slot];
    } else {
        if (context->routePath[1] != -1) {
            const std::int16_t previous = context->routePath[context->routeCount - 2] & 0x8000 ? context->routePath[context->routeCount - 2] & 0x7fff :
                                                                                                 context->routePath[context->routeCount - 2] | 0x8000;
            for (slot = 0; slot < 4; slot++)
                if (route->startConnections[slot] == previous) {
                    slot++;
                    break;
                }
        }
        parameter = area->routeParameters[3][slot];
    }
    return (context->pointCount - 3) * 64 - parameter;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t UpdateNpcRouteWindow(NpcRouteStateContextPrefix *context, std::int32_t advance) {
    const NpcRouteSpline *const table = context->stateTable;
    std::int32_t window = static_cast<std::int16_t>(context->routeParameter / 64);
    if (window > context->pointCount - 4)
        window = context->pointCount - 4;
    const std::int16_t *route;
    if (!advance) {
        route = context->routePath;
        context->routeType = 0;
        context->activeRouteIndex = 0;
        context->activeNode = route[0];
        context->activePointBase = 0;
        context->activeNode = route[0];
        context->nextNode = route[1];
        context->activeWindow = 0;
    } else {
        if (window == context->activeWindow)
            return 1;
        route = context->routePath + context->activeRouteIndex;
    }
    std::int32_t base = context->activePointBase;
    std::int32_t length = table[*route & 0x7fff].pointCount - 2;
    for (std::int32_t point = 0; point < 4; ++point) {
        while (window + point > base + length) {
            ++route;
            base += length;
            if (*route == -1)
                break;
            if (point < 2) {
                ++context->activeRouteIndex;
                context->activePointBase = static_cast<std::int16_t>(base);
            }
            context->routeType = static_cast<std::uint8_t>(point);
            length = table[*route & 0x7fff].pointCount - 2;
        }
        if (*route == -1)
            break;
        if (point == 1) {
            context->activeNode = *route;
            context->nextNode = route[1];
            if (~context->activeNode & 0x8000)
                context->activeSegment = static_cast<std::int16_t>(window - base);
            else
                context->activeSegment = static_cast<std::int16_t>(length - (window - base) - 2);
        }
    }
    context->activeWindow = static_cast<std::int16_t>(window);
    context->moveFlags = (context->moveFlags & 0xff00U) | (table[context->activeNode & 0x7fff].flags & 0xffU);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route) {
    return npcRouteNodes[route].points;
}

#else

namespace npc_package_detail {

void SetNpcYaw(NpcRouteLiveEntry &entry, const std::byte *operand) {
    const float yaw = ReadF32(operand);
    WriteNpcLiveField(entry, npcYawOffset, yaw);
    if (entry.node) {
        entry.node->entity.node.rotation[1] = static_cast<std::int32_t>(ReadU32(operand));
    }
}

const std::byte *GetNpcAreaPoint(std::uint16_t point) {
    if (point >= npcSchedule.areaPointCount)
        return nullptr;
    const std::size_t offset = npcSchedule.areaPointTableOffset + static_cast<std::size_t>(point) * 0x44;
    return Contains(npcSchedule.bytes, offset, 0x44) ? npcSchedule.bytes.data() + offset : nullptr;
}

const std::byte *GetNpcRouteNode(std::uint16_t node) {
    if (node >= npcSchedule.routeNodeCount)
        return nullptr;
    const std::size_t offset = npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(node) * 0x4c;
    return Contains(npcSchedule.bytes, offset, 0x4c) ? npcSchedule.bytes.data() + offset : nullptr;
}

bool GetNpcRouteNodePoint(std::uint16_t node, std::uint32_t point, Vector3 &position) {
    const std::byte *const routeNode = GetNpcRouteNode(node);
    if (!routeNode || point >= ReadU32(routeNode + 4))
        return false;
    const std::uint64_t offset = static_cast<std::uint64_t>(npcSchedule.routeNodeTableOffset) + ReadU32(routeNode) + static_cast<std::uint64_t>(point) * 12;
    if (!Contains(npcSchedule.bytes, offset, sizeof(position)))
        return false;
    std::memcpy(&position, npcSchedule.bytes.data() + offset, sizeof(position));
    return true;
}

// @verified - p
std::uint64_t SampleNpcSplineWindow(const Vector3 *points, std::int32_t pointCount, float parameter, Vector3 *position) {
    std::uint32_t valid = 1;
    if (parameter < 0.0f)
        parameter = -parameter - static_cast<float>(static_cast<std::int32_t>(-parameter));
    std::int32_t index = static_cast<std::int32_t>(parameter);
    float fraction = parameter - static_cast<float>(index);
    if (parameter >= 0.0f) {
        if (parameter >= static_cast<float>(pointCount - 3)) {
            fraction = 1.0f;
            index = pointCount - 4;
            valid = 0;
        }
    } else {
        index = 0;
        fraction = 0.0f;
    }
    Matrix4x4 control{.rows = {
                          {points[index].x, points[index + 1].x, points[index + 2].x, points[index + 3].x},
                          {points[index].y, points[index + 1].y, points[index + 2].y, points[index + 3].y},
                          {points[index].z, points[index + 1].z, points[index + 2].z, points[index + 3].z},
                          {1.0f, 1.0f, 1.0f, 1.0f},
                      }};
    Matrix4x4 interpolation{};
    interpolation.rows[0].z = (fraction * fraction) * fraction;
    interpolation.rows[0].y = interpolation.rows[0].z * 6.0f;
    interpolation.rows[0].x = interpolation.rows[0].z * 6.0f;
    interpolation.rows[1].z = fraction * fraction;
    interpolation.rows[1].y = (fraction * fraction) + (fraction * fraction);
    interpolation.rows[2].z = fraction;
    interpolation.rows[3].w = 1.0f;
    Matrix4x4_Push(&interpolation);
    Matrix4x4::MultiplyWithPrePassMatrix(&npcSplineBasis);
    Matrix4x4::MultiplyWithPrePassMatrix(&control);
    Matrix4x4::GetTransformMatrix(&control);
    Matrix4x4::Pop(0);
    position->x = control.rows[0].z + control.rows[0].w;
    position->y = control.rows[1].z + control.rows[1].w;
    position->z = control.rows[2].z + control.rows[2].w;
    return valid;
}

bool SampleNpcRouteNode(std::uint16_t node, float parameter, Vector3 &position) {
    const std::byte *const routeNode = GetNpcRouteNode(node);
    if (!routeNode)
        return false;
    const std::uint32_t pointCount = ReadU32(routeNode + 4);
    if (pointCount < 4)
        return false;
    const std::size_t offset = npcSchedule.routeNodeTableOffset + ReadU32(routeNode);
    if (!Contains(npcSchedule.bytes, offset, pointCount * sizeof(Vector3)))
        return false;
    return SampleNpcSplineWindow(
               reinterpret_cast<const Vector3 *>(npcSchedule.bytes.data() + offset), static_cast<std::int32_t>(pointCount), parameter, &position) != 0;
}

std::int32_t GetNpcRouteNodePointCount(std::int32_t route) {
    const std::byte *const node = npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(route) * 0x4c;
    return static_cast<std::int32_t>(ReadU32(node + 4));
}

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route) {
    const std::byte *const node = npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(route) * 0x4c;
    const std::uint32_t offset = ReadU32(node);
    return offset ? reinterpret_cast<const Vector3 *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + offset) : nullptr;
}
} // namespace npc_package_detail

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteNodePointCount(std::int32_t route) {
    return npcRouteNodes[route].pointCount;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const Vector3 *GetNpcRouteStartPoint(const NpcRouteSpline *routes, std::int16_t node) {
    if (~node & 0x8000)
        return routes[node & 0x7fff].points + 1;
    return routes[node & 0x7fff].points + routes[node & 0x7fff].pointCount - 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags) {
    float height = position.y;
    std::int32_t floor, count, index;
    float previous, ceiling, candidate;
    if (npcScheduleFloor != -1000) {
        std::int16_t floorBits = static_cast<std::int16_t>(~0xff);
        floorBits &= moveFlags;
        floor = floorBits >> 8;
        if (!floor)
            floor = 1;
        if (npcScheduleFloor == floor || (moveFlags & 0x80000U)) {
            position.y = (3.6f + GetQronFloorHeight(floor)) - 0.1f;
            position.y = FindQronIntersectionHeight(&position, floor);
        } else
            position.y = FLT_MAX;
    } else {
        position.y = TransformClippedPoint(&position) + 1.8f;
        position.y = FindFieldIntersectionHeight(&position);
    }
    if (std::abs(FLT_MAX - position.y) < SHENMUE_NPC_EPSILON || std::abs(position.y + FLT_MAX) < SHENMUE_NPC_EPSILON) {
        position.y = height;
        return false;
    }
    count = moveFlags & 0xffU;
    if (count > 1)
        if (count <= 10) {
            height = position.y;
            for (index = 1; index < count; ++index) {
                previous = position.y;
                ceiling = 10.0f + previous;
                while (ceiling > position.y) {
                    position.y += 1.0f;
                    candidate = FindFieldIntersectionHeight(&position);
                    if (candidate > previous) {
                        height = candidate;
                        break;
                    }
                }
            }
            position.y = height;
        }
    return true;
}

#else

bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags) {
    constexpr float maximum = (std::numeric_limits<float>::max)();
    constexpr float epsilon = (std::numeric_limits<float>::epsilon)();
    const float savedY = position.y;
    std::uint8_t count = static_cast<std::uint8_t>(moveFlags);
    if (npcSchedule.floor == -1000) {
        const float bound = TransformClippedPoint(position, position.y) + 1.8f;
        if (bound != maximum)
            position.y = bound;
        position.y = shenFindIntersectionWithScene(position, bound);
    } else {
        std::int32_t floor = static_cast<std::int16_t>(moveFlags) >> 8;
        if (!floor)
            floor = count ? count : 1;
        if (npcSchedule.floor == floor || (moveFlags & 0x10000U)) {
            const float bound = (GetQronFloorHeight(floor) + 3.5999999f) - 0.1f;
            position.y = bound;
            position.y = FindQronIntersectionHeight(position, bound, floor);
        } else {
            position.y = maximum;
        }
        count = 0;
    }
    if (position.y == maximum || position.y == -maximum || std::abs(maximum - position.y) < epsilon || std::abs(position.y + maximum) < epsilon) {
        position.y = savedY;
        return false;
    }
    if (count >= 2 && count <= 10) {
        float height = position.y;
        for (std::int32_t floor = 1; floor < count; ++floor) {
            const float previous = position.y;
            const float ceiling = previous + 10.0f;
            while (ceiling > position.y) {
                position.y += 1.0f;
                const float candidate = shenFindIntersectionWithScene(position, position.y);
                if (candidate > previous) {
                    height = candidate;
                    break;
                }
            }
        }
        position.y = height;
    }
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t QueryNpcMoveObstruction(NpcRouteLiveEntry &entry, const Vector3 &target, bool door, std::int16_t angle, float probeRadius, float probeLength) {
    Vector3 origin = entry.localPosition;
    std::int32_t yaw;
    float radius;
    float length;
    if (door) {
        origin = NpcRouteAreaToScene(entry.stateContext.savedSplinePosition);
        yaw = entry.stateContext.targetYaw;
        radius = 0.44999999f;
        length = 1.5f;
    } else {
        const float x = target.x - entry.worldPosition.x;
        const float z = target.z - entry.worldPosition.z;
        if (entry.streamRadius > std::abs(x) && entry.streamRadius > std::abs(z))
            return 0;
        yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
        radius = probeRadius;
        length = (probeLength < 0.0f ? std::sqrt(x * x + z * z) : probeLength) + entry.streamRadius;
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(yaw), 0, 0);
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    std::uint32_t result = 0;
    if (TestNpcForwardCylinder(npcPlayerPosition, angle, npcPlayerRadius + radius, npcPlayerRadius + length))
        result = 1;
    if (!result && npcRegistryTask.areaCount) {
        for (const NpcNoSpawnArea &area : npcRegistryTask.areas) {
            if (area.name.name && TestNpcForwardCylinder(area.center, angle, area.radius + radius, area.radius + length)) {
                result = 1;
                break;
            }
        }
    }
    if (!result) {
        for (NpcRouteLiveEntry *other = npcCrowdHead; other && other != npcCrowdBookmark; other = other->next) {
            if (!other->node || other == &entry || (other->flags & 0x40U) || std::abs(other->localPosition.y - entry.localPosition.y) > 1.8f ||
                (door && (other->areaPointPrevious || other->areaPointNext)))
                continue;
            if (TestNpcForwardCylinder(other->streamCenter, angle, other->streamRadius + radius, other->streamRadius + length)) {
                result = (other->stateContext.flags & 1U) | 2U;
                break;
            }
        }
    }
    Matrix4x4::Pop(1);
    return result;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

Vector3 NpcRouteAreaToScene(Vector3 position) {
    position.x -= npcSchedule.areaOrigin.x;
    position.z -= npcSchedule.areaOrigin.z;
    return position;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ProjectNpcRouteHeights(NpcRouteSpline *routes) {
    std::uint32_t floorFlags;
    if (npcScheduleFloor != -1000)
        floorFlags = static_cast<std::uint32_t>(GetQronNextStream()) << 8;
    else
        floorFlags = 0;
    for (NpcRouteSpline *route = routes; route->points; ++route) {
        const std::uint32_t flags = floorFlags | (route->flags & 0xffU);
        for (std::int32_t point = 0; point < route->pointCount - 2; ++point) {
            Vector3 position;
            InterpolateNpcSplineNormalized(route->points, route->pointCount, point * 64, &position);
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            FindNpcIntersectionInScene(position, flags);
            route->points[point + 1].y = position.y;
        }
        route->points[0].y = route->points[1].y;
        route->points[route->pointCount - 1].y = route->points[route->pointCount - 2].y;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RebuildNpcRouteLanes(NpcRouteSpline *routes, std::int32_t measure, std::int32_t sampleStep) {
    if (sampleStep == 1) {
        ProjectNpcRouteHeights(routes);
        GetNpcScheduleFilename();
    }
    std::uint32_t floorFlags;
    std::int32_t segments, segment;
    if (npcScheduleFloor != -1000)
        floorFlags = static_cast<std::uint32_t>(GetQronNextStream()) << 8;
    else
        floorFlags = 0;
    for (; routes->points; ++routes) {
        float *bounds = routes->laneBounds;
        if (measure) {
            Vector3 position = routes->points[1];
            position.x -= npcRouteOrigin.x;
            position.z -= npcRouteOrigin.z;
            const std::uint32_t flags = floorFlags | (routes->flags & 0xffU);
            if (!FindNpcIntersectionInScene(position, flags)) {
                position = routes->points[routes->pointCount - 2];
                position.x -= npcRouteOrigin.x;
                position.z -= npcRouteOrigin.z;
                if (!FindNpcIntersectionInScene(position, flags)) {
                    segments = routes->pointCount - 3;
                    for (segment = 0; segment < segments; ++segment) {
                        bounds[1] = 0.40000001f;
                        bounds[0] = 0.40000001f;
                        bounds += 2;
                    }
                    continue;
                }
            }
            CheckNpcRouteIntersections(routes->points, routes->pointCount, bounds, sampleStep, flags, routes->auxiliary);
            if (sampleStep == 1) {
                segments = routes->pointCount - 3;
                for (segment = 0; segment < segments; ++segment)
                    for (std::int32_t wait = 1001; wait--;) {
                    }
                for (segment = 0; segment < segments; ++segment)
                    for (std::int32_t wait = 1001; wait--;) {
                    }
                for (std::int32_t point = 0; point < routes->pointCount; ++point)
                    for (std::int32_t wait = 1001; wait--;) {
                    }
            }
        } else {
            segments = routes->pointCount - 3;
            for (segment = 0; segment < segments; ++segment) {
                bounds[1] = 0.40000001f;
                bounds[0] = 0.40000001f;
                bounds += 2;
            }
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeNpcRouteTable(NpcRouteSpline *routes, std::int32_t storedClearance) {
    npcRouteNodes = routes;
    for (NpcRouteSpline *route = routes; route->points; ++route) {
        route->points = reinterpret_cast<Vector3 *>(reinterpret_cast<std::uintptr_t>(route->points) + reinterpret_cast<std::uintptr_t>(routes));
        route->segmentLengths = reinterpret_cast<float *>(reinterpret_cast<std::uintptr_t>(route->segmentLengths) + reinterpret_cast<std::uintptr_t>(routes));
        route->laneBounds = reinterpret_cast<float *>(reinterpret_cast<std::uintptr_t>(route->laneBounds) + reinterpret_cast<std::uintptr_t>(routes));
        route->auxiliary = reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::uintptr_t>(route->auxiliary) + reinterpret_cast<std::uintptr_t>(routes));
    }
    if (!storedClearance) {
        ProjectNpcRouteHeights(routes);
        RebuildNpcRouteLanes(routes, 1, 2);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcScriptRouteFlag(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation, std::uint32_t flags) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    if (!operation) {
        context->routeFlags &= ~flags;
        return;
    }
    context->routeFlags |= flags;
    if (flags & 0x100U) {
        CheckNpcRouteIntersections(context->stateTable->points, context->stateTable->pointCount, context->stateTable->laneBounds, 1,
            context->stateTable->flags & 0xffU, context->stateTable->auxiliary);
        const std::int32_t count = context->stateTable->pointCount - 3;
        for (std::int32_t index = 0; index < count; ++index) {
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SampleNpcRouteNodeWindow(shen_name name, std::int32_t parameter, const Vector3 *start, Vector3 *position, std::int32_t *yaw, std::int32_t step) {
    const NpcRouteSpline *route = npcRouteNodes;
    while (route->name != name) {
        if (!route->points)
            return 0;
        ++route;
    }
    Vector3 previous;
    Vector3 next;
    std::int32_t valid;
    if (!start) {
        InterpolateNpcSplineNormalized(route->points, route->pointCount, parameter, &previous);
        parameter += step;
        previous.x -= npcRouteOrigin.x;
        previous.z -= npcRouteOrigin.z;
        valid = InterpolateNpcSplineNormalized(route->points, route->pointCount, parameter, &next);
    } else {
        previous = *start;
        parameter += step;
        valid = InterpolateNpcSplineNormalized(route->points, route->pointCount, parameter, &next);
    }
    next.x -= npcRouteOrigin.x;
    next.z -= npcRouteOrigin.z;
    *position = next;
    next.x -= previous.x;
    next.y -= previous.y;
    next.z -= previous.z;
    if (!(std::abs(next.x) < 0.001f && std::abs(next.z) < 0.001f))
        *yaw = static_cast<std::int16_t>(InverseFixedAtan2(next.x, next.z));
    else
        *yaw = 0;
    return valid;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float GetNpcMotionRigScale(const Character *character) {
    if (!character)
        return 1.0f;
    const std::int32_t rig = charGetMotionRigType(character);
    return rig > 15 ? 1.0f : static_cast<float>(npcMotionRigScales[rig]) / 10.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcRouteMoveFlags(NpcRouteStateContextPrefix *context, shen_name name, std::uint32_t flags) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    context->moveFlags = flags;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t InitializeNpcAreaMovement(shen_name name, NpcRouteStateContextPrefix *context, std::int32_t current, std::int32_t next) {
    ResetNpcScriptContext(context);
    context->currentAreaPoint = current;
    context->pendingAreaPoint = next;
    context->scriptOwner = name;
    context->flags = (context->flags & 0x20002U) | 1U;
    context->routeFlags |= 0x206U;
    if (context->owner->flags & 0x400000U)
        context->routeFlags |= 0x100000U;
    InitializeNpcRouteMotion(context->owner->node, context, 1.0f);
    context->routePending = 1;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BindNpcRouteMotionWork(NpcRouteStateContextPrefix *context, shen_name name, MotionWork *work) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    context->motionWork = work;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BindNpcRouteAppliedMovement(NpcRouteStateContextPrefix *context, shen_name name) {
    NpcRouteLiveEntry *entry;
    if (!context) {
        entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    } else {
        entry = context->owner;
    }
    if (entry->node) {
        Vector3 movement;
        context->appliedMovement = ccmGetAppliedMovement(entry->node, &movement);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeNpcLocomotionProfile(Character *character, NpcRouteStateContextPrefix *context) {
    std::uint16_t motion = 0;
    if (context->flags & 0x40U)
        return;
    if (!(context->flags & 0x10U)) {
        context->maximumYawStep = 546;
        context->motionStep = 0.039999999f;
        if (character)
            motion = SelectNpcWalkingMotion(character);
    } else {
        context->maximumYawStep = 1820;
        context->motionStep = 0.1f;
        if (character)
            motion = SelectNpcRunningMotion(character);
    }
    if (character) {
        Vector3 displacement;
        SampleNativeMotionRoot(motion, charGetMotionRigType(character), -1.0f, &displacement);
        if (-displacement.z > 0.00000011920928955078125f)
            context->motionStep = -displacement.z / static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(motion)));
        context->flags |= 0x40U;
    }
    context->motionStep *= context->motionScale;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcRouteMotionState(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t operation) {
    NpcRouteLiveEntry *entry;
    if (!context) {
        entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    } else {
        entry = context->owner;
    }
    Character *const node = entry->node;
    if (context->flags & 1U) {
        switch (operation) {
        case 0:
            if (!(context->flags & 0x400U))
                if (context->flags & 0x10U) {
                    context->flags &= 0xffffefafU;
                    InitializeNpcLocomotionProfile(node, context);
                    RequestNpcRouteMotion(node, context, 0);
                }
            break;
        case 1:
            if (!(context->flags & 0x400U))
                if (!(context->flags & 0x10U)) {
                    context->flags = (context->flags & 0xffffefbfU) | 0x10U;
                    InitializeNpcLocomotionProfile(node, context);
                    RequestNpcRouteMotion(node, context, 0);
                }
            break;
        case 2:
            if (!(context->flags & 0x1000U))
                context->routeFlags |= 0x800U;
            break;
        case 3:
            context->flags &= ~0x1000U;
            context->routeFlags &= ~0x800U;
            RequestNpcRouteMotion(node, context, 0);
            break;
        }
    }
}

#else

void SetNpcRouteMotionState(NpcRouteStateContext &context, std::int32_t operation, bool explicitRequest) {
    if (!(context.flags & 1U))
        return;
    switch (operation) {
    case 0:
        if (!(context.flags & 0x10U))
            return;
        context.flags &= ~0x50U;
        InitializeNpcLocomotionProfile(context);
        break;
    case 1:
        if ((static_cast<std::uint8_t>(context.owner->recordBase[0x0b]) & 0x20U) || (context.flags & 0x10U))
            return;
        context.flags = (context.flags & ~0x50U) | 0x10U;
        InitializeNpcLocomotionProfile(context);
        break;
    case 2:
        if (!(context.flags & 0x1000U))
            context.routeFlags |= 0x800U;
        if ((context.owner->flags & 0x40U) && context.owner->node) {
            mtwkCopyTransition(context.motionWork, nullptr, 0, 1, 0);
            mtwkCopyTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            mtwkCopyTransition(context.motionWork, nullptr, 0, 2, 0);
        }
        return;
    case 3:
        if (context.routeFlags & 0x2000000U)
            return;
        if (!(context.flags & 0x100000U)) {
            context.flags &= ~0x1000U;
            context.routeFlags &= ~0x800U;
        }
        break;
    default:
        return;
    }
    RequestNpcRouteMotion(context, explicitRequest);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void TickNpcSpecialRouteContext(NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    if ((context.flags & 0x1400U) != 0x400U)
        return;
    if (npcSlotFlags[entry.poolIndex])
        context.routeFlags &= ~0x10000U;
    else
        context.routeFlags |= 0x10000U;
    TickNpcRouteStateContext(&context);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetNpcRouteFollowing(NpcRouteStateContextPrefix *context, shen_name name, shen_name other, float distance) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    if ((context->flags & 1U) == 0)
        return;
    context->followingEntry = FindNpcRouteEntry(other);
    if (context->followingEntry) {
        context->followingDistance = distance;
        context->routeFlags |= 0x1000000U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ResetNpcRouteAvoidance(NpcRouteStateContextPrefix *context) {
    context->field13a = 0;
    context->field13b = 0;
    context->avoidanceResult = 0;
    context->other = 0;
    context->avoidanceAuxiliary = -1;
    context->avoidanceFlags = 0;
    context->avoidancePhase = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ResetNpcScriptContext(NpcRouteStateContextPrefix *context) {
    context->routeFlags = 0;
    context->phase = 0;
    context->otherName = 0;
    context->stuckCounter = 0;
    context->laneStep = 0.0f;
    context->previousSegment = -1;
    context->retainedf8 = 0;
    context->doorWaitFrames = -1;
    context->avoidanceWaitFrames = 0;
    ResetNpcRouteAvoidance(context);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t StopNpcScriptRoute(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t yaw) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return 0;
        context = &entry->stateContext;
    }
    if (context->otherName) {
        ReleaseDoor(context->otherName, context->scriptOwner);
        context->otherName = 0;
    }
    if (yaw >= 0)
        context->savedEuler.y = yaw;
    Character *const character = context->owner->node;
    if (character) {
        if (context->flags & 0x400U) {
            std::uint32_t flags = ccmGetFlags(character);
            if (!(context->moveFlags & 0xff00U))
                flags |= 2U;
            flags = (flags | 4U) & ~0x800U;
            AssignCharacterControlFlags(character, flags);
        }
        mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 13, 0);
        if (context->flags & 0x20U) {
            mtwkUpdateWithPosition(context->motionWork, 0, 0);
            context->flags &= ~0x20U;
        }
    }
    if (context->routeFlags & 0x20U)
        context->owner->node = 0;
    ResetNpcScriptContext(context);
    context->flags = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RebuildNpcRouteContextAfterReload(NpcRouteStateContextPrefix *context, std::int32_t slotFlags, float offset) {
    if (context->flags & 1U) {
        if (context->routeFlags & 0x8000U) {
            StopNpcScriptRoute(context, shen_name(), 0);
            ReleaseNpcScriptRouteSlot(context->scriptOwner);
        } else {
            context->scenePosition = context->localPosition;
            context->scenePosition.x -= npcRouteOrigin.x;
            context->scenePosition.z -= npcRouteOrigin.z;
            if (static_cast<std::int8_t>(context->phase) != 7) {
                if (static_cast<std::int8_t>(context->phase) == 8 && !context->otherName && GetNpcAreaPointOwnerName(context->pendingAreaPoint))
                    context->moveState = 0;
            } else if (slotFlags & 2U) {
                const std::int32_t count = static_cast<std::int32_t>(offset / context->motionStep);
                bool retainPause;
                if (context->flags & 0x40000U) {
                    context->flags &= ~0x1000U;
                    retainPause = true;
                } else {
                    retainPause = false;
                }
                for (std::int32_t index = 0; index < count && TickNpcRouteStateContext(context) != 1; ++index) {
                }
                if (retainPause && (context->flags & 1U))
                    context->flags |= 0x1000U;
            }
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void TickNpcPausedRoute() {
    if (!(npcActiveRouteContext->routeFlags & 0x200000U)) {
        if ((npcActiveRouteContext->flags & 0x8000U) && !(npcActiveRouteContext->routeFlags & 0x40000U) &&
            (!npcRouteCharacter || !mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))) {
            if ((npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60) * 2 - npcActiveRouteContext->pauseStartTime > 7200)
                ResumeNpcRouteAfterSceneChange(npcActiveRouteContext->scriptOwner);
        }
    } else {
        npcActiveRouteContext->flags &= ~0x8000U;
        npcActiveRouteContext->phase = npcActiveRouteContext->savedPhase;
        npcActiveRouteContext->routeFlags &= ~0x200000U;
        npcActiveRouteContext->flags &= ~8U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *state) {
    npcActiveRouteContext = state;
    npcRouteCharacter = state->owner->node;
    npcRouteOwner = state->owner;
    if ((state->flags & 1U) == 0)
        return 1;
    {
        NpcRouteStateContextPrefix *context = npcActiveRouteContext;
        context->previousPosition = context->localPosition;
    }
    if (npcRouteCharacter) {
        npcRouteMotionWork = state->motionWork;
        GetCharacterWorldPosition(npcRouteCharacter, &npcActiveRouteContext->localPosition);
        if (npcActiveRouteContext->routeFlags & 0x40U) {
            if (npcRouteOwner->flags & 0xa000U)
                npcActiveRouteContext->localPosition.y = npcRouteOwner->forcedY;
            else
                FindNpcIntersectionInScene(npcActiveRouteContext->localPosition, npcActiveRouteContext->moveFlags);
            {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                context->scenePosition = context->localPosition;
                context->localPosition.x += npcRouteOrigin.x;
                context->localPosition.z += npcRouteOrigin.z;
            }
            SetCharacterWorldPosition(npcRouteCharacter, &npcActiveRouteContext->scenePosition);
            npcActiveRouteContext->routeFlags &= ~0x40U;
        } else {
            NpcRouteStateContextPrefix *context = npcActiveRouteContext;
            context->scenePosition = context->localPosition;
            context->localPosition.x += npcRouteOrigin.x;
            context->localPosition.z += npcRouteOrigin.z;
        }
    }
    if ((npcActiveRouteContext->routeFlags & 0x500000U) == 0x100000U) {
        if (static_cast<std::int8_t>(npcActiveRouteContext->routePending) != npcPlayerSearchRequested) {
            if (npcActiveRouteContext->flags & 0x41000U) {
                SetNpcRouteMotionState(npcActiveRouteContext, shen_name(), 3);
                npcActiveRouteContext->flags &= ~0x40000U;
            } else {
                npcActiveRouteContext->routeFlags |= 0x800U;
                npcActiveRouteContext->flags |= 0x40000U;
            }
        }
        npcActiveRouteContext->routePending = static_cast<std::uint8_t>(npcPlayerSearchRequested);
    }
    if (npcActiveRouteContext->routeFlags & 0x1000000U)
        UpdateLinkedNpcRoute();
    switch (static_cast<std::int8_t>(npcActiveRouteContext->phase)) {
    case 0:
        EnterNpcRoute();
        break;
    case 1:
        BuildNpcRoute();
        break;
    case 2:
        BindNpcRouteWindow();
        break;
    case 3:
        RequestNpcRouteDoor();
        break;
    case 4:
        TickNpcPausedRoute();
        break;
    case 5:
        TickNpcDoorEgress();
        break;
    case 6:
        TickNpcRouteApproach();
        break;
    case 7:
        TickNpcRouteTraversal();
        break;
    case 8:
        TickNpcDoorIngress();
        break;
    case 12:
        return FinishNpcRoute();
    }
    {
        NpcRouteStateContextPrefix *context = npcActiveRouteContext;
        context->scenePosition = context->localPosition;
        context->scenePosition.x -= npcRouteOrigin.x;
        context->scenePosition.z -= npcRouteOrigin.z;
    }
    if (npcActiveRouteContext->routeFlags & 0x80000U) {
        if (npcRouteCharacter)
            SetCharacterWorldPosition(npcRouteCharacter, &npcActiveRouteContext->scenePosition);
        npcActiveRouteContext->routeFlags &= ~0x80000U;
    }
    {
        NpcRouteLiveEntry *owner = npcRouteOwner;
        Vector3 *position = &owner->localPosition;
        NpcRouteStateContextPrefix *context = npcActiveRouteContext;
        *position = context->scenePosition;
        owner->worldPosition = context->localPosition;
        owner->rotation.y = context->savedEuler.y;
        owner->packedMoveFlags = context->moveFlags;
    }
    return 0;
}

#else

// PC v1.07 TickNpcRouteStateContext. Returns true only when
// the native route has retired, not when one phase or spline window advances.
bool TickNpcRouteStateContext(NpcRouteStateContext *state) {
    npcActiveRouteContext = state;
    NpcRouteStateContext &context = *state;
    NpcRouteLiveEntry &owner = *context.owner;
    if (!(context.flags & 1U))
        return true;
    Character *const character = owner.node;
    context.motionWork = owner.motionWork;
    if (character) {
        context.scenePosition = GetCharacterWorldPosition(character);
        if (context.routeFlags & 0x40U) {
            if (owner.flags & 0xa000U)
                context.scenePosition.y = owner.forcedY;
            else
                FindNpcIntersectionInScene(context.scenePosition, context.moveFlags);
            shenApplyTranslationToAll(&character->entity.node, context.scenePosition);
            context.routeFlags &= ~0x40U;
        }
        context.localPosition = NpcAreaPosition(context.scenePosition);
        if ((context.flags & 3U) && !(context.flags & 0x101008U) && context.moveState != 3) {
            if (!context.stuckCounter++)
                context.stuckPosition = context.localPosition;
            else {
                const float x = context.localPosition.x - context.stuckPosition.x;
                const float z = context.localPosition.z - context.stuckPosition.z;
                if (std::sqrt(x * x + z * z) <= 0.2f) {
                    if (static_cast<std::int16_t>(++context.stuckCounter) > 300) {
                        context.stuckCounter = 0;
                        RequestNpcStreamOut(owner, 2);
                        owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
                    }
                } else
                    context.stuckCounter = 0;
            }
        } else
            context.stuckCounter = 0;
    }
    if ((context.routeFlags & 0x100000U) && !(context.routeFlags & 0x400000U)) {
        const bool searching = plymIsSearchStartRequested();
        if (context.field036 != searching) {
            if (context.flags & 0x40000U) {
                context.flags &= ~0x40000U;
                SetNpcRouteMotionState(context, 3);
            } else {
                context.routeFlags |= 0x800U;
                context.flags |= 0x40000U;
            }
        }
        context.field036 = searching;
    }
    if ((context.flags & 0x100000U) && !(context.flags & 0x1000U) && !(context.routeFlags & 0x800U)) {
        context.flags &= ~0x100000U;
        SetNpcRouteMotionState(context, 3);
    }
    if (context.routeFlags & 0x1000000U)
        UpdateLinkedNpcRoute(context);
    switch (context.phase) {
    case 0:
        EnterNpcRoute(context);
        break;
    case 1:
        BuildNpcRoute(context);
        break;
    case 2:
        BindNpcRouteWindow(context);
        break;
    case 4:
        TickNpcPausedRoute(context);
        break;
    case 5:
        TickNpcDoorEgress(context);
        break;
    case 6:
        TickNpcRouteApproach(context);
        break;
    case 7:
        TickNpcRouteTraversal(context);
        break;
    case 8:
        TickNpcDoorIngress(context);
        break;
    case 9:
        return FinishNpcRoute(context);
    default:
        break;
    }
    context.scenePosition = NpcRouteAreaToScene(context.localPosition);
    if (context.routeFlags & 0x80000U) {
        if (character)
            shenApplyTranslationToAll(&character->entity.node, context.scenePosition);
        context.routeFlags &= ~0x80000U;
    }
    owner.localPosition = context.scenePosition;
    owner.worldPosition = context.localPosition;
    owner.euler.y = context.savedEuler.y;
    owner.packedMoveFlags = context.moveFlags;
    return false;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool ReleaseNpcRouteControl(NpcRouteStateContext *context, shen_name character, std::int32_t yaw) {
    if (!context)
        context = FindNpcRouteStateContext(character);
    if (!context)
        return false;
    NpcRouteLiveEntry &owner = *context->owner;
    if (context->otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context->otherName.name), context->scriptOwner);
        context->otherName = {};
    }
    if (yaw >= 0)
        std::memcpy(&context->savedEuler.y, &yaw, sizeof(yaw));
    if (owner.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(owner.node)) {
            std::uint32_t flags = (owner.flags & 0x8000000U) ? control->flags | 0x800U : control->flags & ~0x800U;
            if (context->flags & 0x200000U) {
                if (npcSchedule.floor == -1000 || !(context->moveFlags & 0xff00U))
                    flags |= 2U;
                flags |= 4U;
            }
            control->flags = flags;
        }
        std::int32_t savedYaw;
        std::memcpy(&savedYaw, &context->savedEuler.y, sizeof(savedYaw));
        mtwkCopyTransition(context->motionWork, nullptr, static_cast<std::uint16_t>(savedYaw), 13, 0);
        mtwkCopyTransition(context->motionWork, nullptr, 0, 2, 0);
        if (context->flags & 0x20U) {
            mtwkUpdateWithPosition(context->motionWork, nullptr, 0);
            context->flags &= ~0x20U;
        }
    }
    if (context->routeFlags & 0x10U) {
        const std::byte *const point = GetNpcAreaPoint(owner.currentAreaPoint);
        if (point) {
            const std::uint32_t area = ReadU32(point) & 0xffffffU;
            if (area == 0x4c4f47U || area == 0x4d4f44U)
                owner.flags |= 0x100000U;
        }
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != owner.name.name)
                continue;
            entry.name = SHEN_NAME(MOVE);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            break;
        }
    }
    context->flags = 0;
    context->routeFlags = 0;
    context->previousSegment = -10;
    context->doorWaitFrames = -1;
    context->phase = 0;
    context->otherName = {};
    context->laneStep = 0.0f;
    context->field0f4 = 0;
    context->avoidanceWaitFrames = 0;
    context->other = nullptr;
    context->field0a1 = 0;
    context->avoidancePhase = 0;
    context->stuckCounter = 0;
    context->avoidanceResult = 0;
    context->avoidanceAuxiliary = -1;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void RebuildNpcRouteCollision(bool reloading) {
    if (npcSchedule.bytes.empty())
        return;
    npcPositionQueryMode = 0;
    npcSchedule.floor = static_cast<std::int32_t>(GetQronNextStream());
    npcSchedule.areaOrigin = {};
    const auto areaPrefix = static_cast<unsigned char>(g_currentMap.name & 0xffU);
    for (std::size_t index = 0; index < 8; ++index) {
        if (static_cast<unsigned char>(npcSchedule.bytes[0x20 + index]) != areaPrefix)
            continue;
        npcSchedule.areaOrigin.x = ReadF32(npcSchedule.bytes.data() + 0x28 + index * 8);
        npcSchedule.areaOrigin.z = ReadF32(npcSchedule.bytes.data() + 0x2c + index * 8);
        break;
    }
    if (!(npcSchedule.routeFlags & 1U)) {
        RebuildNpcRouteGround();
        RebuildNpcRouteLanes(reloading ? 8 : 2);
    }
    RebuildNpcAreaPointGround();
    SetNpcAreaRouteOrigin(npcSchedule.areaOrigin);
    ActivateNpcAreaRoutes(true);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SampleNpcSplineWindow(const Vector3 *points, std::int32_t pointCount, float parameter, Vector3 *position) {
    std::int32_t valid = 1;
    if (parameter < 0.0f) {
        parameter *= -1.0f;
        parameter -= static_cast<float>(static_cast<std::int32_t>(parameter));
    }
    std::int32_t index = static_cast<std::int32_t>(parameter);
    float fraction = parameter - static_cast<float>(index);
    if (parameter < 0.0f) {
        index = 0;
        fraction = 0.0f;
    } else if (!(static_cast<float>(pointCount - 3) > parameter)) {
        index = pointCount - 4;
        fraction = 1.0f;
        valid = 0;
    }
    points += index;
    float control[16];
    for (std::int32_t point = 0; point < 4; ++point, ++points) {
        control[point] = points->x;
        control[point + 4] = points->y;
        control[point + 8] = points->z;
    }
    control[12] = control[13] = control[14] = control[15] = 1.0f;
    Matrix4x4 interpolation;
    BuildNpcSplineInterpolationMatrix(&interpolation, fraction);
    matrix_stack::Matrix4x4_Push(&interpolation);
    Matrix4x4::MultiplyWithPrePassMatrix(&npcSplineBasis);
    Matrix4x4::MultiplyWithPrePassMatrix(reinterpret_cast<Matrix4x4 *>(control));
    matrix_stack::GetMatrix(reinterpret_cast<Matrix4x4 *>(control));
    PopCurrentMatrix(0);
    position->x = control[3] + control[2];
    position->y = control[7] + control[6];
    position->z = control[11] + control[10];
    return valid;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SampleNpcSplineSegment(const Vector3 *points, Vector3 *position, float amount) {
    SampleNpcSplineWindow(points, 4, -amount, position);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float GetNpcRouteCurrentStep(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    return entry ? entry->stateContext.motionStep : 0.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t npcSetAreaName(shen_name name, const Vector3 *center, float radius, std::int32_t enabled) {
    std::int32_t index;
    if (enabled == 1) {
        for (index = 0; index < 8 && npcAreaRestrictions[index].name; ++index) {
        }
        if (index == 8)
            return 0;
        npcAreaRestrictions[index].name = name;
        NpcAreaRestriction *const area = &npcAreaRestrictions[index];
        area->center = *center;
        npcAreaRestrictions[index].radius = radius;
        FindNpcIntersectionInScene(npcAreaRestrictions[index].center, 0);
        npcAreaRestrictionCount = SHENMUE_NPC_SIGNED(static_cast<std::uint32_t>(npcAreaRestrictionCount) + 1U);
    } else {
        for (index = 0; index < 8; ++index) {
            if (npcAreaRestrictions[index].name == name) {
                npcAreaRestrictions[index].name = 0;
                npcAreaRestrictionCount = SHENMUE_NPC_SIGNED(static_cast<std::uint32_t>(npcAreaRestrictionCount) - 1U);
                break;
            }
        }
        if (index == 8)
            return 0;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool IsInsideNpcNoSpawnRadius(const Vector3 *position, float radius) {
    std::int32_t remaining = npcAreaRestrictionCount;
    for (std::int32_t index = 0; remaining && index <= 7 && npcAreaRestrictions[index].name; --remaining, ++index) {
        const float distance = CalculatePlanarMagnitude(npcAreaRestrictions[index].center.x - position->x, npcAreaRestrictions[index].center.z - position->z);
        if (npcAreaRestrictions[index].radius + radius > distance)
            return true;
    }
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t ComputeNpcRouteStride(NpcRouteStateContextPrefix *context) {
    if (context->routeFlags & 0x400000U)
        return 1;
    if (!context->owner->node)
        return 16;
    const NpcRouteSpline *const route = context->stateTable + (context->activeNode & 0x7fff);
    float length;
    if (context->routeType == 2) {
        const std::int32_t connection = context->routeConnections[context->activeRouteIndex];
        length = route->connectionLengths[connection];
    } else {
        length = route->segmentLengths[context->activeSegment + 1];
    }
    context->segmentLength = length;
    return static_cast<std::int32_t>(::ceilf((context->motionStep * 64.0f) / length));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RefreshNpcScriptRouteMotion(Character *character, NpcRouteStateContextPrefix *context) {
    if (character) {
        if (!GetNpcRunCapability(character))
            SetNpcRouteMotionState(context, context->scriptOwner, 1);
        context->routeFlags &= ~0x1000U;
    } else {
        context->routeFlags |= 0x1000U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RestartNpcAreaRouteContext(Character *character, NpcRouteStateContextPrefix *context) {
    ResetNpcRouteAvoidance(context);
    context->routeFlags |= 0x40U;
    if (context->flags & 2U)
        context->stride = static_cast<std::uint16_t>(ComputeNpcRouteStride(context));
    InitializeNpcLocomotionProfile(character, context);
    if (context->routeFlags & 0x1000U)
        RefreshNpcScriptRouteMotion(character, context);
    context->flags &= 0xfffefbffU;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ResetNpcRouteContext(NpcRouteStateContextPrefix *context) {
    context->motionWork = SHENMUE_NPC_NULL;
    context->flags &= 0xfffefbffU;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, float scale) {
    context->motionScale = 1.0f;
    InitializeNpcLocomotionProfile(character, context);
    SetNpcLocomotionScale(context, shen_name(), scale);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position) {
    return SampleNpcSplineWindow(points, pointCount, static_cast<float>(parameter) * 0.015625f, position);
}

#else

std::uint64_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position) {
    return SampleNpcSplineWindow(points, pointCount, static_cast<float>(parameter) * 0.015625f, position);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void MeasureNpcRouteClearance(const Vector3 &start, const Vector3 &end, float &left, float &right, std::uint32_t moveFlags, std::int32_t step) {
    std::int32_t rightSteps;
    std::int32_t leftSteps = static_cast<std::int32_t>(::fabsf(left * 10.0f));
    rightSteps = static_cast<std::int32_t>(::fabsf(right * 10.0f));
    Vector3 point, resolved;
    point.x = end.x - start.x;
    point.z = end.z - start.z;
    if (::fabsf(point.x) < 0.001f && ::fabsf(point.z) < 0.001f)
        return;
    const std::int32_t yaw = InverseFixedAtan2(point.x, point.z) - 0x4000;
    const float sine = dc::sdk::SinFixed(yaw);
    const float cosine = dc::sdk::CosFixed(yaw);
    for (std::int32_t index = step; index <= rightSteps || index <= leftSteps; index += step) {
        const float distance = static_cast<float>(index) * 0.1f;
        const float x = distance * sine;
        const float z = distance * cosine;
        if (index <= rightSteps) {
            point.x = start.x - x;
            point.z = start.z - z;
            point.x -= npcRouteOrigin.x;
            point.z -= npcRouteOrigin.z;
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.3f;
            if (RaycastInWorldGrid(&resolved, &point, 0.4f))
                rightSteps = index - step;
        }
        if (index <= leftSteps) {
            point.x = start.x + x;
            point.z = start.z + z;
            point.x -= npcRouteOrigin.x;
            point.z -= npcRouteOrigin.z;
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.3f;
            if (RaycastInWorldGrid(&resolved, &point, 0.4f))
                leftSteps = index - step;
        }
    }
    left = static_cast<float>(leftSteps) / 10.0f;
    right = static_cast<float>(rightSteps) / 10.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary) {
    Vector3 previous, next;
    Vector3i rotation;
    Vector3 ground;
    float left, right;
    std::int32_t parameter, steps, nextParameter;
    std::uint32_t presentedTime, currentTime;
    std::int32_t clearanceStep = sampleStep == 1 ? 1 : 4;
    presentedTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    InterpolateNpcSplineNormalized(points, pointCount, 0, &previous);
    std::memset(auxiliary, 0, pointCount - 3);
    parameter = 0;
    while (parameter / 64 < pointCount - 3) {
        left = right = 3.0f;
        steps = 0;
        while (steps < 64) {
            nextParameter = parameter + sampleStep;
            if (!InterpolateNpcSplineNormalized(points, pointCount, nextParameter, &next)) {
                parameter = nextParameter;
                previous = next;
                break;
            }
            if (!auxiliary[parameter / 64]) {
                ground = previous;
                ground.x -= npcRouteOrigin.x;
                ground.z -= npcRouteOrigin.z;
                FindNpcIntersectionInScene(ground, moveFlags);
                ground.y -= 0.225f;
                if (flddQueryStairOrigin(&ground, &ground, &rotation.y))
                    auxiliary[parameter / 64] = 1;
            }
            MeasureNpcRouteClearance(previous, next, left, right, moveFlags, clearanceStep);
            previous = next;
            currentTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
            if (IsLoadingDrawActive() && static_cast<float>(presentedTime - currentTime) / 12.5f > 15000.0f) {
                DrawLoadingModes();
                dc::sdk::SubmitFrame();
                presentedTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
            }
            steps += sampleStep;
            parameter += sampleStep;
        }
        bounds[0] = left;
        bounds[1] = right;
        bounds += 2;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
NpcAreaRouteState *GetNpcAreaPointRouteState(std::int32_t point) {
    return reinterpret_cast<NpcAreaRouteState *>(static_cast<std::uintptr_t>(npcAreaPoints[point].routeStateOffset));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
shen_name GetNpcAreaPointOwnerName(std::int32_t point) {
    return npcAreaPoints[point].owner;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
shen_name GetNpcAreaPointAreaName(std::int32_t point) {
    return npcAreaPoints[point].area;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t FindNpcAreaPointByName(shen_name name, std::int32_t useFirst) {
    const NpcAreaPoint *point = npcAreaPoints;
    for (std::int32_t index = 0; point->area; ++point, ++index)
        if (point->area == name)
            return index;
    if (useFirst)
        return 0;
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t StartNpcScriptAreaRoute(shen_name name, shen_name oldPoint, shen_name newPoint, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindNpcRouteEntry(name);
    if (!entry) {
        if (!CreateScriptNpcRouteSlot(name))
            return 0;
        entry = FindNpcRouteEntry(name);
    }
    entry->liveState = 1;
    entry->scheduleProgress = 3;
    NpcRouteStateContextPrefix &context = entry->stateContext;
    ResetNpcScriptContext(&context);
    context.stateTable = npcScriptRouteTable;
    context.currentAreaPoint = static_cast<std::int16_t>(FindNpcAreaPointByName(oldPoint, 1));
    context.pendingAreaPoint = static_cast<std::int16_t>(FindNpcAreaPointByName(newPoint, 1));
    context.scriptOwner = name;
    context.flags = (context.flags & 0x20002U) | 1U;
    context.routeFlags |= 0x16U;
    if (yaw >= 0) {
        context.routeFlags |= 0x20000U;
        context.arrivalYaw = yaw;
    }
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character) {
        GetCharacterWorldRotation(character, &context.savedEuler);
        GetCharacterWorldPosition(character, &context.scenePosition);
        context.localPosition = context.scenePosition;
        context.localPosition.x += npcRouteOrigin.x;
        context.localPosition.z += npcRouteOrigin.z;
        BindNpcRouteMotionWork(&context, shen_name(), charFindMotionwork(character));
        BindNpcRouteAppliedMovement(&context, shen_name());
    }
    InitializeNpcRouteMotion(character, &context, 1.0f);
    context.routePending = 1;
    entry->currentAreaPoint = context.currentAreaPoint;
    entry->pendingAreaPoint = context.pendingAreaPoint;
    return 1;
}

#else

bool StartNpcScriptAreaRoute(shen_name character, shen_name oldPoint, shen_name newPoint, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindActiveNpcEntry(character);
    Vector3 position{};
    if (entry)
        position = entry->localPosition;
    else {
        NpcRouteLiveEntry *slot = nullptr;
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &candidate = npcRouteEntries[index];
            if (candidate.name.name != character.name)
                continue;
            candidate.name = NPC_SCRIPT_ROUTE_SLOT;
            candidate.node = nullptr;
            candidate.motionWork = nullptr;
            break;
        }
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            if (npcRouteEntries[index].name.name != NPC_SCRIPT_ROUTE_SLOT.name)
                continue;
            slot = &npcRouteEntries[index];
            slot->name = character;
            break;
        }
        if (!slot)
            return false;
        entry = FindActiveNpcEntry(character);
    }
    NpcRouteStateContext &context = entry->stateContext;
    entry->liveState = 1;
    entry->scheduleProgress = 3;
    context.previousSegment = -10;
    context.doorWaitFrames = -1;
    context.routeFlags = 0;
    context.phase = 0;
    context.otherName = {};
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.stateTable = reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.currentAreaPoint = 0;
    context.pendingAreaPoint = 0;
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        const std::uint32_t pointName = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(index)));
        if (!pointName)
            break;
        if (pointName == oldPoint.name) {
            context.currentAreaPoint = static_cast<std::uint16_t>(index);
            break;
        }
    }
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        const std::uint32_t pointName = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(index)));
        if (!pointName)
            break;
        if (pointName == newPoint.name) {
            context.pendingAreaPoint = static_cast<std::uint16_t>(index);
            break;
        }
    }
    context.flags = (context.flags & 0x20002U) | 1U;
    context.scriptOwner = character;
    context.routeFlags |= 0x16U;
    if (yaw >= 0) {
        context.arrivalYaw = static_cast<std::uint16_t>(yaw);
        context.field10a = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) >> 16U);
        context.routeFlags |= 0x20000U;
    }
    Character *const node = ctrlGetScenarioChara(character);
    if (node) {
        const Vector3i rotation = GetCharacterWorldRotation(node);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        position = GetCharacterWorldPosition(node);
        entry->motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(node);
        context.motionWork = entry->motionWork;
    }
    if (std::fabs(position.x + 1300.0f) < 0.00000011920929f && std::fabs(position.z - 1000.0f) < 0.00000011920929f) {
        std::memcpy(&position, GetNpcAreaPoint(context.currentAreaPoint) + 0x34, sizeof(position));
        position.x -= npcSchedule.areaOrigin.x;
        position.z -= npcSchedule.areaOrigin.z;
    }
    context.scenePosition = position;
    context.localPosition = NpcAreaPosition(position);
    entry->node = node;
    context.motionScale = 1.0f;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    entry->currentAreaPoint = context.currentAreaPoint;
    entry->pendingAreaPoint = context.pendingAreaPoint;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetNpcScriptRouteFlags(NpcRouteStateContext *context, shen_name character, std::int32_t operation, std::uint32_t flags) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    if (!(context->flags & 1U))
        return;
    if (operation)
        context->routeFlags |= flags;
    else
        context->routeFlags &= ~flags;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t InitializeNpcMovement(shen_name name, NpcRouteSpline *routes, const Vector3 *origin, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindNpcRouteEntry(name);
    if (!entry) {
        if (!CreateScriptNpcRouteSlot(name))
            return 0;
        entry = FindNpcRouteEntry(name);
    }
    NpcRouteStateContextPrefix &context = entry->stateContext;
    ResetNpcScriptContext(&context);
    context.stateTable = routes;
    context.currentAreaPoint = 0;
    context.pendingAreaPoint = 1;
    context.targetOrigin = *origin;
    context.arrivalYaw = yaw;
    context.scriptOwner = name;
    context.moveFlags = routes->flags & 0xffU;
    context.flags = 1;
    context.routeFlags |= 0x80b6U;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character) {
        GetCharacterWorldRotation(character, &context.savedEuler);
        GetCharacterWorldPosition(character, &context.scenePosition);
        context.localPosition = context.scenePosition;
        context.localPosition.x += npcRouteOrigin.x;
        context.localPosition.z += npcRouteOrigin.z;
        entry->motionWork = charFindMotionwork(character);
        BindNpcRouteMotionWork(&context, shen_name(), entry->motionWork);
        BindNpcRouteAppliedMovement(&context, shen_name());
    }
    entry->node = character;
    InitializeNpcRouteMotion(character, &context, 1.0f);
    context.routePending = 1;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void ResetNpcTransitions() {
    std::int32_t index;
    for (index = 0; index < 4; ++index) {
        npcTransitionRuntimeSlots[index].character = 0;
        npcTransitionRuntimeSlots[index].motionCursor = 0;
        npcTransitionRuntimeSlots[index].node = 0;
    }
    std::int32_t slot = 0;
    std::int32_t animationSlot = 0;
    for (index = 0; npcTransitionEntries[index].area; ++index) {
        if (g_currentMap == npcTransitionEntries[index].area) {
            npcTransitionRuntimeSlots[slot].character = npcTransitionEntries[index].character;
            npcTransitionRuntimeSlots[slot].position = npcTransitionEntries[index].position;
            npcTransitionRuntimeSlots[slot].yaw = npcTransitionEntries[index].yaw;
            npcTransitionRuntimeSlots[slot].visibility = npcTransitionEntries[index].visibility;
            npcTransitionRuntimeSlots[slot].motionSequence = npcTransitionEntries[index].motions;
            if (npcTransitionRuntimeSlots[slot].motionSequence)
                npcTransitionRuntimeSlots[slot].animationSlot = animationSlot++;
            slot++;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ResumeNpcScheduleMotion(NpcRouteLiveEntry *entry) {
    std::int32_t phase = static_cast<std::int32_t>(npcFrameCount) + 2 * static_cast<std::int8_t>(entry->scriptRecord[8]);
    if (mtwkGetAnimationId(entry->motionWork, LowerBody))
        phase += 20;
    mtwkRequestMotion(entry->motionWork, entry->currentMotionId, LowerBody);
    mtwkSetRequestedRootMode(entry->motionWork, 1);
    mtwkSetFrameTime(entry->motionWork, 20, LowerBody);
    mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
    float frame;
    mtwkSetRegionEvalTime(
        entry->motionWork, frame = static_cast<float>(phase % (static_cast<std::int32_t>(GetMotionFrameCount(entry->currentMotionId)) - 1)), LowerBody);
    Vector3 track;
    SampleNativeMotionRoot(entry->currentMotionId, charGetMotionRigType(entry->node), frame, &track);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(entry->localPosition.x, entry->localPosition.y, entry->localPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(entry->rotation.x, entry->rotation.y, entry->rotation.z);
    Matrix4x4::preTranslate(track.x, 0.0f, track.z);
    matrix_stack::GetTranslation(&track);
    PopCurrentMatrix(1);
    SetCharacterWorldPosition(entry->node, &track);
    charaSetWorldRot(entry->node, entry->rotation);
    AssignCharacterControlFlags(entry->node, ccmGetFlags(entry->node) & ~6U);
}

#else

void ResumeNpcScheduleMotion(NpcRouteLiveEntry &entry) {
    std::int32_t phase = static_cast<std::int32_t>(npcFrameCount) + 2 * static_cast<std::int8_t>(entry.recordBase[8]);
    if (mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody))
        phase += 20;
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
    const MotnSequence &sequence = *FindLoadedMotionSequence(entry.currentMotionId);
    const float frame = static_cast<float>(phase % (sequence.frameCount - 1));
    mtwkSetRegionEvalTime(entry.motionWork, frame, MotionRegion::LowerBody);
    Vector3 track;
    SampleNativeMotionRoot(entry.currentMotionId, GetCharacterBoneId(entry.node), frame, track);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(entry.localPosition.x, entry.localPosition.y, entry.localPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::preTranslate(track.x, 0.0f, track.z);
    track = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    shenApplyTranslationToAll(&entry.node->entity.node, track);
    charaSetWorldRot(entry.node, rotation);
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        control->flags &= ~(CCMF_ApplyGravity | CCMF_EvaluateMotion);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BuildNpcSplineInterpolationMatrix(Matrix4x4 *matrix, float parameter) {
    static const Matrix4x4 base = {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
    *matrix = base;
    matrix->rows[2].z = parameter;
    matrix->rows[1].z = parameter * parameter;
    matrix->rows[0].z = parameter * matrix->rows[1].z;
    matrix->rows[1].y = matrix->rows[1].z + matrix->rows[1].z;
    matrix->rows[0].x = matrix->rows[0].y = matrix->rows[0].z * 6.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

NpcAreaPoint *GetNpcAreaPoint(std::int32_t point) {
    return npcAreaPoints + point;
}

// @verified - d
const Vector3 *GetNpcAreaPointPosition(std::int32_t point) {
    return &npcAreaPoints[point].position;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t GetNpcAreaPointMoveFlags(std::int32_t point) {
    return npcRouteNodes[npcAreaPoints[point].routeIndex].flags & 0xffU;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcAreaPointRouteNode(std::int32_t point) {
    return npcAreaPoints[point].routeIndex;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void EnterNpcRoute() {
    if (npcActiveRouteContext->flags & 2U) {
        if (npcActiveRouteContext->routeType == 2 && static_cast<std::int16_t>(npcActiveRouteContext->routeParameter % 64) > 32)
            npcActiveRouteContext->startNode = npcActiveRouteContext->nextNode;
        else
            npcActiveRouteContext->startNode = npcActiveRouteContext->activeNode;
        npcActiveRouteContext->routeFlags |= 0x4000U;
        npcActiveRouteContext->routeFlags |= 0x800000U;
        npcActiveRouteContext->endNode = static_cast<std::int16_t>(GetNpcAreaPointRouteNode(npcActiveRouteContext->pendingAreaPoint));
        npcActiveRouteContext->moveState = 0;
        npcActiveRouteContext->phase = 1;
        npcActiveRouteContext->flags = (npcActiveRouteContext->flags & ~2U) | 4U;
    } else {
        if (npcActiveRouteContext->currentAreaPoint == npcActiveRouteContext->pendingAreaPoint) {
            npcActiveRouteContext->phase = 12;
            npcActiveRouteContext->flags &= ~4U;
        } else {
            if (npcActiveRouteContext->routeFlags & 0x20U)
                npcActiveRouteContext->startNode = npcActiveRouteContext->endNode = 0;
            else {
                npcActiveRouteContext->startNode = static_cast<std::int16_t>(GetNpcAreaPointRouteNode(npcActiveRouteContext->currentAreaPoint));
                npcActiveRouteContext->endNode = static_cast<std::int16_t>(GetNpcAreaPointRouteNode(npcActiveRouteContext->pendingAreaPoint));
            }
            npcActiveRouteContext->phase = 1;
            npcActiveRouteContext->flags &= ~4U;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BuildNpcRoute() {
    if (static_cast<std::int32_t>(npcRouteBuildCount) <= 7 || (npcActiveRouteContext->flags & 0x100U)) {
        ++npcRouteBuildCount;
    } else {
        ++npcActiveRouteContext->stuckCounter;
        return;
    }
    npcRouteTarget = *GetNpcAreaPointPosition(npcActiveRouteContext->pendingAreaPoint);
    if (!FindNpcRoutePath(npcActiveRouteContext)) {
        npcActiveRouteContext->flags |= 0x80U;
        npcActiveRouteContext->phase = 12;
    } else
        npcActiveRouteContext->phase = 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RequestNpcRouteDoor() {
    if (npcRouteCharacter) {
        switch (static_cast<std::int8_t>(npcActiveRouteContext->moveState)) {
        case 0:
            RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
            npcActiveRouteContext->moveState = 1;
            return;
        case 1:
            if (mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))
                return;
            break;
        default:
            return;
        }
    }
    npcActiveRouteContext->phase = 1;
    npcActiveRouteContext->flags &= ~2U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t ResumeNpcRouteAfterSceneChange(shen_name name) {
    FindNpcRouteEntry(name);
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    (&entry->stateContext)->routeFlags |= 0x200000U;
    if ((&entry->stateContext)->flags & 0x100U) {
        if (entry->pendingRouteQueryState) {
            entry->retained312[0] = static_cast<std::byte>(-entry->pendingRouteQueryState);
            entry->pendingRouteQueryState = 10;
        } else {
            entry->retained312[0] = static_cast<std::byte>(entry->routeQueryState);
            entry->routeQueryState = 10;
        }
    } else {
        if (entry->flags & 0x10U)
            return 1;
        if (static_cast<std::int8_t>(entry->retained312[0]) < 0)
            entry->pendingRouteQueryState = 12;
        else
            entry->routeQueryState = 12;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t FinishNpcRoute() {
    if (npcActiveRouteContext->routeFlags & 0x10U)
        ReleaseNpcScriptRouteSlot(npcActiveRouteContext->scriptOwner);
    if (npcActiveRouteContext->routeFlags & 0x20U)
        npcRouteOwner->node = 0;
    npcActiveRouteContext->currentAreaPoint = npcActiveRouteContext->pendingAreaPoint;
    npcActiveRouteContext->phase = 0;
    if (npcActiveRouteContext->flags & 0x100U) {
        const std::uint32_t saved = npcActiveRouteContext->flags & 0x4000U;
        npcActiveRouteContext->routeFlags = 0;
        npcActiveRouteContext->flags = 0;
        InitializeNpcAreaMovement(
            npcActiveRouteContext->scriptOwner, npcActiveRouteContext, npcRouteOwner->stateContext.currentAreaPoint, npcRouteOwner->pendingAreaPoint);
        SetNpcLocomotionScale(npcActiveRouteContext, shen_name(), npcRouteOwner->motionSpeed);
        npcActiveRouteContext->flags |= 0x200U;
        if (saved) {
            npcActiveRouteContext->flags |= 0x4000U;
            npcActiveRouteContext->routeFlags |= 0x20000U;
        }
        if (static_cast<std::int8_t>(npcRouteOwner->retained312[0]) < 0)
            npcRouteOwner->pendingRouteQueryState = 11;
        else
            npcRouteOwner->routeQueryState = 11;
        npcActiveRouteContext->routeFlags |= 1U;
        npcActiveRouteContext->flags |= 0x8000U;
        npcActiveRouteContext->pauseStartTime = (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60) * 2;
        return 0;
    }
    if (npcActiveRouteContext->routeFlags & 0x4000000U) {
        npcActiveRouteContext->routeFlags = 0;
        npcActiveRouteContext->flags = 0;
        InitializeNpcAreaMovement(
            npcActiveRouteContext->scriptOwner, npcActiveRouteContext, npcRouteOwner->stateContext.currentAreaPoint, npcRouteOwner->pendingAreaPoint);
        SetNpcLocomotionScale(npcActiveRouteContext, shen_name(), npcRouteOwner->motionSpeed);
        return 0;
    }
    npcActiveRouteContext->routeFlags = 0;
    if (npcActiveRouteContext->flags & 0x200U) {
        npcActiveRouteContext->flags &= ~0x200U;
        npcActiveRouteContext->routeFlags = 0x400U;
        npcRouteOwner->scheduleProgress = npcRouteOwner->retained309;
        npcRouteOwner->liveState = npcRouteOwner->retained30b;
        if (static_cast<std::int8_t>(npcRouteOwner->retained312[0]) < 0)
            npcRouteOwner->pendingRouteQueryState = -static_cast<std::int8_t>(npcRouteOwner->retained312[0]);
        else
            npcRouteOwner->routeQueryState = static_cast<std::int8_t>(npcRouteOwner->retained312[0]);
        if (npcActiveRouteContext->flags & 0x4000U) {
            npcActiveRouteContext->flags &= ~0x4000U;
            npcRouteOwner->scheduleProgress = 1;
            npcRouteOwner->currentCommand = npcRouteOwner->branchCommandStart;
        }
        if (npcSceneRouteOwner == npcRouteOwner) {
            npcSceneRouteOwner = 0;
            SetNpcScheduleSeedName(shen_name(), npcRouteOwner->name, 0);
        }
    }
    npcActiveRouteContext->flags = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BindNpcRouteWindow() {
    NpcRouteStateContextPrefix nearest, target;
    if ((npcActiveRouteContext->flags & 4U) || (npcActiveRouteContext->routeFlags & 0x20U)) {
        nearest = *npcActiveRouteContext;
        npcActiveRouteContext->lowerParameter = FindNearestNpcParameter(&nearest, &npcActiveRouteContext->localPosition);
    } else
        npcActiveRouteContext->lowerParameter = GetNpcRouteStartParameter(npcActiveRouteContext, npcActiveRouteContext->currentAreaPoint);
    if (npcActiveRouteContext->routeFlags & 0x20U) {
        target = *npcActiveRouteContext;
        npcActiveRouteContext->upperParameter = FindNearestNpcParameter(&target, &npcActiveRouteContext->targetOrigin);
    } else
        npcActiveRouteContext->upperParameter = GetNpcRouteEndParameter(npcActiveRouteContext, npcActiveRouteContext->pendingAreaPoint);
    if (npcActiveRouteContext->lowerParameter > npcActiveRouteContext->upperParameter) {
        if (npcActiveRouteContext->flags & 0x2000U)
            npcActiveRouteContext->lowerParameter = npcActiveRouteContext->upperParameter;
        else {
            npcActiveRouteContext->startNode =
                npcActiveRouteContext->startNode & 0x8000 ? npcActiveRouteContext->startNode & 0x7fff : npcActiveRouteContext->startNode | 0x8000;
            npcActiveRouteContext->flags |= 0x2000U;
            npcActiveRouteContext->phase = 1;
            return;
        }
    }
    npcActiveRouteContext->startNode = npcActiveRouteContext->routePath[0];
    npcActiveRouteContext->endNode = npcActiveRouteContext->routePath[npcActiveRouteContext->routeCount - 1];
    npcActiveRouteContext->stride = 1;
    npcActiveRouteContext->routeParameter = npcActiveRouteContext->lowerParameter;
    UpdateNpcRouteWindow(npcActiveRouteContext, 0);
    SampleNpcRouteWindow(npcActiveRouteContext, &npcActiveRouteContext->position, 0);
    if (npcActiveRouteContext->stateTable[npcActiveRouteContext->activeNode & 0x7fff].flags & 0x400U)
        npcRouteOwner->motionAttachments[0].flags |= 0x40U;
    npcActiveRouteContext->phase = 6;
    if (!(npcActiveRouteContext->flags & 0x204U) && !(npcActiveRouteContext->routeFlags & 0x20U) && (npcRouteOwner->flags & 0x800U) &&
        !GetNpcAreaPointRouteState(npcActiveRouteContext->currentAreaPoint) &&
        (npcActiveRouteContext->otherName = GetNpcAreaPointOwnerName(npcActiveRouteContext->currentAreaPoint)) != 0)
        npcActiveRouteContext->phase = 5;
    if (npcActiveRouteContext->routeFlags & 1U) {
        npcActiveRouteContext->savedPhase = npcActiveRouteContext->phase;
        npcActiveRouteContext->phase = 4;
        npcActiveRouteContext->flags |= 8U;
    }
    npcActiveRouteContext->moveState = 0;
    npcActiveRouteContext->flags |= 2U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t UpdateNpcSpecialRouteQueryState(NpcRouteLiveEntry *entry) {
    if (!((&entry->stateContext)->flags & 0x200U) || !(entry->flags & 0x10U))
        return 0;
    if (static_cast<std::int8_t>(entry->retained312[0]) < 0) {
        if (entry->stateContext.pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[3])
            entry->pendingRouteQueryState = 4;
        else if (entry->stateContext.pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[4])
            entry->pendingRouteQueryState = 2;
        else
            entry->pendingRouteQueryState = 2;
    } else if (entry->stateContext.pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[3])
        entry->routeQueryState = 4;
    else if (entry->stateContext.pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[4])
        entry->routeQueryState = 2;
    else
        entry->routeQueryState = 2;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

NpcAreaRestriction *GetNpcAreaRestriction(shen_name name, std::uint32_t index) {
    if (index > 7)
        return 0;
    npcAreaRestrictionQueryFlag = 0;
    return &npcAreaRestrictions[index];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

struct NpcRouteSeedData {
    std::uint32_t flags, routeFlags;
    std::uint8_t retained312, retained309, retained30b;
    std::int16_t routeParameter, areaPoint, pendingAreaPoint, arrivalYaw;
    float x, z;
};

std::int32_t CaptureNpcRouteSeedData(const NpcRouteLiveEntry *entry, std::uint32_t *data) {
    NpcRouteSeedData *const seed = reinterpret_cast<NpcRouteSeedData *>(data);
    seed->flags = entry->stateContext.flags;
    seed->routeFlags = entry->stateContext.routeFlags;
    seed->retained312 = static_cast<std::uint8_t>(entry->retained312[0]);
    if (entry->stateContext.flags & 4U) {
        if (entry->stateContext.routeType == 2 && static_cast<std::int16_t>(entry->stateContext.routeParameter % 64) > 32)
            seed->areaPoint = entry->stateContext.nextNode;
        else
            seed->areaPoint = entry->stateContext.activeNode;
    } else
        seed->areaPoint = entry->stateContext.currentAreaPoint;
    seed->pendingAreaPoint = entry->stateContext.pendingAreaPoint;
    seed->x = entry->stateContext.localPosition.x;
    seed->z = entry->stateContext.localPosition.z;
    seed->arrivalYaw = *reinterpret_cast<const std::int16_t *>(&entry->stateContext.arrivalYaw);
    seed->routeParameter = entry->stateContext.routeParameter;
    seed->retained309 = entry->retained309;
    seed->retained30b = entry->retained30b;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t RestoreNpcRouteSeedData(NpcRouteLiveEntry *entry, const std::uint32_t *data) {
    const std::int32_t yaw = entry->rotation.y;
    Vector3 saved;
    saved.x = reinterpret_cast<const float *>(data)[5];
    saved.z = reinterpret_cast<const float *>(data)[6];
    saved.y = 0.0f;
    entry->worldPosition = saved;
    entry->localPosition = entry->worldPosition;
    entry->localPosition.x -= npcRouteOrigin.x;
    entry->localPosition.z -= npcRouteOrigin.z;
    entry->stateContext.scenePosition = entry->localPosition;
    entry->stateContext.localPosition = entry->worldPosition;
    if (data[0] & 0x100U) {
        if (data[0] & 0x4000U) {
            entry->rotation.y = reinterpret_cast<const std::int16_t *>(data)[9];
            entry->scheduleProgress = reinterpret_cast<const std::byte *>(data)[9];
            entry->liveState = reinterpret_cast<const std::byte *>(data)[10];
        } else {
            NpcRouteStateContextPrefix &context = entry->stateContext;
            context.flags |= 1U;
            entry->stateContext.phase = 7;
            entry->stateContext.pendingAreaPoint = reinterpret_cast<const std::int16_t *>(data)[8];
            if (data[0] & 4U) {
                entry->stateContext.activeNode = reinterpret_cast<const std::int16_t *>(data)[7];
                context.flags |= 2U;
            } else
                entry->stateContext.currentAreaPoint = reinterpret_cast<const std::int16_t *>(data)[7];
        }
        RequestNpcSceneRoute(entry->name, GetNpcAreaPointAreaName(reinterpret_cast<const std::int16_t *>(data)[8]));
        if (!(data[0] & 8U)) {
            NpcRouteStateContextPrefix &context = entry->stateContext;
            context.routeFlags = (context.routeFlags | 0x200000U) & ~1U;
            TickNpcPausedRoute();
        }
    } else if (data[0] & 0x8200U) {
        InitializeNpcAreaMovement(
            entry->name, &entry->stateContext, reinterpret_cast<const std::int16_t *>(data)[7], reinterpret_cast<const std::int16_t *>(data)[8]);
        SetNpcLocomotionScale(&entry->stateContext, shen_name(), entry->motionSpeed);
        NpcRouteStateContextPrefix &context = entry->stateContext;
        context.flags |= 0x200U;
        if (data[0] & 0x4000U) {
            context.flags |= 0x4000U;
            context.routeFlags |= 0x20000U;
        }
        entry->retained312[0] = reinterpret_cast<const std::byte *>(data)[8];
        if (data[0] & 0x8000U) {
            if (static_cast<std::int8_t>(entry->retained312[0]) < 0)
                entry->pendingRouteQueryState = 11;
            else
                entry->routeQueryState = 11;
            NpcRouteStateContextPrefix &queried = entry->stateContext;
            queried.routeFlags |= 1U;
            queried.flags |= 0x8000U;
            entry->stateContext.pauseStartTime = (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60) * 2;
        } else {
            entry->flags |= 0x10U;
            UpdateNpcSpecialRouteQueryState(entry);
            entry->flags &= ~0x10U;
        }
    } else {
        if (data[0] & 4U) {
            NpcRouteStateContextPrefix &context = entry->stateContext;
            entry->stateContext.activeNode = reinterpret_cast<const std::int16_t *>(data)[7];
            context.flags |= 2U;
        }
        InitializeNpcAreaMovement(
            entry->name, &entry->stateContext, reinterpret_cast<const std::int16_t *>(data)[7], reinterpret_cast<const std::int16_t *>(data)[8]);
        SetNpcLocomotionScale(&entry->stateContext, shen_name(), entry->motionSpeed);
    }
    if (data[0] & 0x10U)
        SetNpcRouteMotionState(&entry->stateContext, entry->name, 1);
    if (!(data[1] & 2U)) {
        NpcRouteStateContextPrefix &context = entry->stateContext;
        context.routeFlags &= ~2U;
    }
    if (!(data[1] & 4U)) {
        NpcRouteStateContextPrefix &context = entry->stateContext;
        context.routeFlags &= ~4U;
    }
    if (data[0] & 0x20000U) {
        NpcRouteStateContextPrefix &settling = entry->stateContext;
        settling.routeFlags |= 0x400000U;
        do
            TickNpcRouteStateContext(&entry->stateContext);
        while (entry->stateContext.phase != 7);
        std::int32_t target;
        if (data[0] & 4U) {
            NpcRouteStateContextPrefix temporary = entry->stateContext;
            target = FindNearestNpcParameter(&temporary, &saved);
        } else
            target = reinterpret_cast<const std::int16_t *>(data)[6];
        do
            TickNpcRouteStateContext(&entry->stateContext);
        while (entry->stateContext.routeParameter < target);
        NpcRouteStateContextPrefix &settled = entry->stateContext;
        settled.routeFlags &= ~0x400000U;
        if ((data[0] & 0x1000U) || (data[1] & 0x800U)) {
            NpcRouteStateContextPrefix &context = entry->stateContext;
            context.routeFlags |= 0x800U;
        }
    }
    entry->worldPosition = saved;
    entry->localPosition = entry->worldPosition;
    entry->localPosition.x -= npcRouteOrigin.x;
    entry->localPosition.z -= npcRouteOrigin.z;
    entry->stateContext.scenePosition = entry->localPosition;
    entry->stateContext.localPosition = entry->worldPosition;
    entry->stateContext.savedEuler.y = yaw;
    entry->rotation.y = yaw;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float MeasureNpcRouteParameterDistance(shen_name name, NpcRouteStateContextPrefix *context, std::int32_t distance) {
    if (!context) {
        context = FindNpcRouteStateContext(name);
        if (!context)
            return 0.0f;
    }
    const NpcRouteSpline *const splines = context->stateTable;
    std::int32_t direction;
    if (distance < 0) {
        direction = -1;
        distance = -distance;
    } else
        direction = 1;
    std::int32_t route = context->activeRouteIndex;
    const std::int16_t *const path = context->routePath;
    std::int32_t segment = context->activeSegment;
    std::int32_t step;
    if (direction == 1)
        step = 64 - static_cast<std::int16_t>(context->routeParameter % 64);
    else
        step = static_cast<std::int16_t>(context->routeParameter % 64);
    std::int32_t connection = context->routeType == 2;
    float total = 0.0f;
    float length;
    std::int32_t count;
    std::int32_t last = splines[path[route] & 0x7fff].pointCount - 3;
    for (;;) {
        if (connection)
            length = splines[path[route] & 0x7fff].connectionLengths[context->routeConnections[route]];
        else
            length = splines[path[route] & 0x7fff].segmentLengths[segment + 1];
        if (distance - step > 0) {
            distance -= step;
            total += step * length * (1.0f / 64.0f);
            step = 64;
            connection = 0;
            if (~path[route] & 0x8000)
                segment += direction;
            else
                segment -= direction;
            if (segment > last || segment < -1) {
                route += direction;
                if (route < 0 || route >= context->routeCount)
                    break;
                count = splines[path[route] & 0x7fff].pointCount;
                last = count - 3;
                if (~path[route] & 0x8000) {
                    if (direction == 1)
                        segment = 0;
                    else
                        segment = count - 4;
                } else
                    segment = direction == 1 ? last - 1 : 0;
            } else if (segment == last || segment == -1)
                connection = 1;
        } else {
            total += distance * length * (1.0f / 64.0f);
            break;
        }
    }
    total *= direction;
    return total;
}

// @verified - d
float QueryNpcRouteGeometry(shen_name name, std::int32_t parameter, std::int32_t mode, float *low, float *high) {
    const NpcRouteStateContextPrefix *context;
    const NpcRouteSpline *table;
    const std::int16_t *route;
    const float *lanes;
    std::int32_t window, base, count, point, segment, routeType, routeIndex, activeNode, nextNode = 0;
    float result = -1.0f, upper, lower, bound;
    context = FindNpcRouteStateContext(name);
    if (context && (context->flags & 3U) == 3U) {
        window = parameter / 64;
        if (window > context->pointCount - 4)
            window = context->pointCount - 4;
        table = context->stateTable;
        route = context->routePath;
        activeNode = routeIndex = routeType = segment = 0;
        base = 0;
        count = table[*route & 0x7fff].pointCount - 2;
        for (point = 0; point < 4; ++point) {
            while (window + point > base + count) {
                ++route;
                base += count;
                if (*route == -1)
                    break;
                do {
                    if (point < 2)
                        ++routeIndex;
                } while (0);
                routeType = point;
                count = table[*route & 0x7fff].pointCount - 2;
            }
            if (*route == -1)
                break;
            if (point == 1) {
                activeNode = *route;
                nextNode = route[1];
                if (~activeNode & 0x8000)
                    segment = window - base;
                else
                    segment = count - (window - base) - 2;
            }
        }
        switch (mode) {
        case 0:
            if (routeType == 2)
                upper = table[activeNode & 0x7fff].connectionLengths[context->routeConnections[routeIndex]];
            else
                upper = table[activeNode & 0x7fff].segmentLengths[segment + 1];
            result = upper * 0.015625f;
            break;
        case 1:
            lanes = table[activeNode & 0x7fff].laneBounds + segment * 2;
            if (~activeNode & 0x8000) {
                upper = lanes[1];
                bound = lanes[0];
            } else {
                upper = lanes[0];
                bound = lanes[1];
            }
            lower = -bound;
            if (routeType == 2 || routeType == 3) {
                if (~nextNode & 0x8000)
                    segment = 0;
                else
                    segment = table[nextNode & 0x7fff].pointCount - 4;
                lanes = table[nextNode & 0x7fff].laneBounds + segment * 2;
                if (~nextNode & 0x8000) {
                    if (upper > lanes[1])
                        upper = lanes[1];
                    bound = lanes[0];
                } else {
                    if (upper > lanes[0])
                        upper = lanes[0];
                    bound = lanes[1];
                }
                if (-bound > lower)
                    lower = -bound;
            }
            *low = lower;
            *high = upper;
            result = 1.0f;
            break;
        }
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float GetNpcRouteSegmentLength(shen_name name, std::int32_t parameter) {
    return QueryNpcRouteGeometry(name, parameter, 0, 0, 0);
}

#else

float GetNpcRouteSegmentLength(shen_name character, std::int32_t parameter) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source || (source->flags & 3U) != 3U)
        return -1.0f;
    NpcRouteStateContext context = *source;
    UpdateNpcRouteWindow(context, parameter);
    return ReadNpcRouteSegmentLength(context) * 0.015625f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

bool GetNpcRouteLaneBounds(shen_name name, std::int32_t parameter, float *low, float *high) {
    if (QueryNpcRouteGeometry(name, parameter, 1, low, high) == -1.0f)
        return false;
    return true;
}

#else

bool GetNpcRouteLaneBounds(shen_name character, std::int32_t parameter, float *low, float *high) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source || (source->flags & 3U) != 3U)
        return false;
    NpcRouteStateContext context = *source;
    UpdateNpcRouteWindow(context, parameter);
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    const std::byte *bounds = ResolveNpcRouteData(context, node.laneBoundsOffset) + context.activeSegment * 8;
    float left = ReadF32(bounds + (context.activeNode < 0 ? 4 : 0));
    float right = ReadF32(bounds + (context.activeNode < 0 ? 0 : 4));
    left = -left;
    if (context.crossingPoint == 2 || context.crossingPoint == 3) {
        const NpcRouteNode &next = context.stateTable[context.nextNode & 0x7fff];
        const std::int32_t segment = context.nextNode < 0 ? static_cast<std::int32_t>(next.pointCount) - 4 : 0;
        bounds = ResolveNpcRouteData(context, next.laneBoundsOffset) + segment * 8;
        right = std::fmin(right, ReadF32(bounds + (context.nextNode < 0 ? 0 : 4)));
        left = std::fmax(left, -ReadF32(bounds + (context.nextNode < 0 ? 4 : 0)));
    }
    *low = left;
    *high = right;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Caller-owned scripted route admission; the six MOVE entries belong to NPCC.
bool BindNpcScriptRoute(shen_name character, const NpcRouteNode *route, const Vector3 *origin, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindActiveNpcEntry(character);
    Vector3 position{};
    if (entry) {
        position = entry->localPosition;
    } else {
        for (std::size_t i = npcAuthoredEntryCount; i < npcRouteEntries.size(); ++i) {
            NpcRouteLiveEntry &candidate = npcRouteEntries[i];
            if (candidate.name.name != character.name)
                continue;
            candidate.name = NPC_SCRIPT_ROUTE_SLOT;
            candidate.node = nullptr;
            candidate.motionWork = nullptr;
            break;
        }
        for (std::size_t i = npcAuthoredEntryCount; i < npcRouteEntries.size(); ++i) {
            if (npcRouteEntries[i].name.name != NPC_SCRIPT_ROUTE_SLOT.name)
                continue;
            entry = &npcRouteEntries[i];
            entry->name = character;
            break;
        }
        if (!entry)
            return false;
    }
    NpcRouteStateContext &context = entry->stateContext;
    context.currentAreaPoint = 0;
    context.pendingAreaPoint = 1;
    context.otherName = {};
    context.routeFlags = 0;
    context.phase = 0;
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.previousSegment = -10;
    context.doorWaitFrames = -1;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.stateTable = route;
    context.targetPosition = *origin;
    context.arrivalYaw = static_cast<std::uint16_t>(yaw);
    context.field10a = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) >> 16U);
    context.scriptOwner = character;
    context.flags = 1;
    context.routeFlags = 32950;
    context.moveFlags = route->flags | (entry->packedMoveFlags & 0x1ff00U);
    Character *const node = ctrlGetScenarioChara(character);
    if (node) {
        const Vector3i rotation = GetCharacterWorldRotation(node);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        position = GetCharacterWorldPosition(node);
        entry->motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(node);
        context.motionWork = entry->motionWork;
    }
    if (std::fabs(position.x + 1300.0f) < 0.00000011920929f && std::fabs(position.z - 1000.0f) < 0.00000011920929f)
        position = *origin;
    context.scenePosition = position;
    context.localPosition = NpcAreaPosition(position);
    entry->node = node;
    context.motionScale = 1.0f;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void TickNpcDoorEgress() {
    std::int32_t complete = 0;
    Vector3 v;
    switch (static_cast<std::int8_t>(npcActiveRouteContext->moveState)) {
    case 0:
        npcRouteOwner->flags &= ~0x800U;
        if (!ComputeDoorIngress(
                static_cast<std::int32_t>(npcActiveRouteContext->otherName), &npcActiveRouteContext->scenePosition, &npcActiveRouteContext->moveTarget)) {
            complete = 1;
            break;
        }
        npcActiveRouteContext->moveTarget.x += npcRouteOrigin.x;
        npcActiveRouteContext->moveTarget.z += npcRouteOrigin.z;
        v.x = npcActiveRouteContext->position.x - npcActiveRouteContext->moveTarget.x;
        v.z = npcActiveRouteContext->position.z - npcActiveRouteContext->moveTarget.z;
        if (!(std::fabs(v.x) < 0.001f && std::fabs(v.z) < 0.001f)) {
            const std::int16_t yaw = InverseFixedAtan2(v.x, v.z);
            {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                v = context->moveTarget;
                TransformNpcPointToYawFrame(&context->localPosition, yaw, &v);
            }
            if (v.z > 0.0f) {
                complete = 1;
                break;
            }
        }
        npcActiveRouteContext->targetYaw = g_doorIngress.rotation.y;
        npcActiveRouteContext->sceneTarget = npcActiveRouteContext->moveTarget;
        ComputeDoorEgress(
            static_cast<std::int32_t>(npcActiveRouteContext->otherName), &npcActiveRouteContext->scenePosition, &npcActiveRouteContext->doorPosition);
        npcActiveRouteContext->doorPosition.x += npcRouteOrigin.x;
        npcActiveRouteContext->doorPosition.z += npcRouteOrigin.z;
        if (!npcRouteCharacter) {
            complete = 2;
            break;
        }
        npcActiveRouteContext->doorMotion =
            GetNpcDoorAnimation(static_cast<std::int32_t>(npcActiveRouteContext->otherName), npcRouteCharacter, &npcActiveRouteContext->scenePosition);
        SampleNativeMotionRoot(static_cast<std::uint16_t>(npcActiveRouteContext->doorMotion), charGetMotionRigType(npcRouteCharacter), 15.0f, &v);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::SetViewMatrixFromEuler(0, npcActiveRouteContext->targetYaw, 0);
        Matrix4x4::TransformPoint(v, v);
        PopCurrentMatrix(1);
        npcActiveRouteContext->sceneTarget.x += v.x;
        npcActiveRouteContext->sceneTarget.z += v.z;
        npcActiveRouteContext->sceneTarget.x -= npcRouteOrigin.x;
        npcActiveRouteContext->sceneTarget.z -= npcRouteOrigin.z;
        if (!FindNpcIntersectionInScene(npcActiveRouteContext->sceneTarget, npcActiveRouteContext->moveFlags)) {
            {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                v = context->doorPosition;
                v.x -= npcRouteOrigin.x;
                v.z -= npcRouteOrigin.z;
                FindNpcIntersectionInScene(v, context->moveFlags);
            }
            npcActiveRouteContext->scenePosition.y = v.y;
            SetCharacterWorldPosition(npcRouteCharacter, &npcActiveRouteContext->scenePosition);
            {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                context->localPosition = context->scenePosition;
                context->localPosition.x += npcRouteOrigin.x;
                context->localPosition.z += npcRouteOrigin.z;
            }
        }
        if (!ClaimDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner))
            npcActiveRouteContext->moveState = 3;
        else
            npcActiveRouteContext->moveState = 1;
        RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
        npcRouteOwner->motionAttachments[0].flags |= 0x40U;
        break;
    case 1:
        if (npcRouteCharacter) {
            if (mtwkCopyTransition(npcRouteMotionWork, 0, 0, 3, 0))
                RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
            else if (!mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1)) {
                npcRouteOwner->flags |= 0x100U;
                npcActiveRouteContext->moveState = 4;
                BeginDoorAnimation(static_cast<std::int32_t>(npcActiveRouteContext->otherName), npcActiveRouteContext->scriptOwner,
                    npcActiveRouteContext->doorMotion, 14.0f, npcActiveRouteContext->motionScale * 1.1f);
                mtwkSetSpeed(npcActiveRouteContext->motionWork, npcActiveRouteContext->motionScale * 1.1f, LowerBody);
            }
        } else
            complete = 2;
        break;
    case 4:
        if (npcRouteCharacter) {
            if (mtwkSetRegionFlag(npcRouteMotionWork, 0x41, 1, LowerBody)) {
                Character *const character = npcRouteCharacter;
                AssignCharacterControlFlags(character, ccmGetFlags(character) | 0x804U);
                complete = 1;
            }
        } else
            complete = 2;
        break;
    case 3:
        if (ClaimDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner) == 1) {
            npcActiveRouteContext->moveState = 1;
            RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
        }
        break;
    }
    if (complete) {
        if (complete == 2) {
            NpcRouteStateContextPrefix *context = npcActiveRouteContext;
            context->localPosition = context->doorPosition;
            context->savedEuler.y = context->targetYaw;
        }
        if (npcActiveRouteContext->otherName) {
            ReleaseDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner);
            npcActiveRouteContext->otherName = 0;
            npcRouteOwner->flags &= ~0x100U;
        }
        npcActiveRouteContext->phase = 6;
        npcActiveRouteContext->moveState = 0;
        npcRouteOwner->motionAttachments[0].flags &= ~0x40U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void TickNpcDoorIngress() {
    std::int32_t complete = 0;
    switch (static_cast<std::int8_t>(npcActiveRouteContext->moveState)) {
    case 0:
        npcActiveRouteContext->moveState = 1;
        if (!(npcActiveRouteContext->routeFlags & 0x20U)) {
            if (GetNpcAreaPointRouteState(npcActiveRouteContext->pendingAreaPoint)) {
                complete = 1;
                break;
            }
            npcActiveRouteContext->otherName = GetNpcAreaPointOwnerName(npcActiveRouteContext->pendingAreaPoint);
            if (npcActiveRouteContext->otherName &&
                ComputeDoorIngress(
                    static_cast<std::int32_t>(npcActiveRouteContext->otherName), &npcActiveRouteContext->scenePosition, &npcActiveRouteContext->moveTarget)) {
                npcActiveRouteContext->targetYaw = g_doorIngress.rotation.y;
                ComputeDoorEgress(
                    static_cast<std::int32_t>(npcActiveRouteContext->otherName), &npcActiveRouteContext->scenePosition, &npcActiveRouteContext->doorPosition);
                if (npcRouteCharacter)
                    mtwkUpdateWithPosition(npcRouteMotionWork, &npcActiveRouteContext->doorPosition, 0);
                npcActiveRouteContext->doorPosition.x += npcRouteOrigin.x;
                npcActiveRouteContext->doorPosition.z += npcRouteOrigin.z;
                if (!ClaimDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner))
                    npcActiveRouteContext->moveState = 3;
                else
                    npcRouteOwner->flags |= 0x100U;
                if (npcRouteCharacter)
                    AssignCharacterControlFlags(npcRouteCharacter, ccmGetFlags(npcRouteCharacter) | 0x800U);
                {
                    NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                    context->sceneTarget = context->moveTarget;
                    context->moveTarget.x += npcRouteOrigin.x;
                    context->moveTarget.z += npcRouteOrigin.z;
                }
                npcRouteOwner->motionAttachments[0].flags |= 0x40U;
            } else {
                npcActiveRouteContext->otherName = 0;
                npcActiveRouteContext->targetYaw = ComputeNpcAreaPointYaw(npcActiveRouteContext->stateTable, npcActiveRouteContext->pendingAreaPoint) + 0x8000;
                {
                    const Vector3 *position = GetNpcAreaPointPosition(npcActiveRouteContext->pendingAreaPoint);
                    NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                    context->moveTarget = *position;
                    context->sceneTarget = context->moveTarget;
                    context->sceneTarget.x -= npcRouteOrigin.x;
                    context->sceneTarget.z -= npcRouteOrigin.z;
                }
            }
        } else {
            npcActiveRouteContext->otherName = 0;
            npcActiveRouteContext->targetYaw = npcActiveRouteContext->arrivalYaw;
            {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                context->moveTarget = context->targetOrigin;
                context->sceneTarget = context->moveTarget;
                context->sceneTarget.x -= npcRouteOrigin.x;
                context->sceneTarget.z -= npcRouteOrigin.z;
            }
        }
        npcActiveRouteContext->savedEuler.y = GetNpcRoutePointYaw(&npcActiveRouteContext->localPosition, &npcActiveRouteContext->sceneTarget);
        if (npcActiveRouteContext->routeFlags & 0x20000U) {
            npcActiveRouteContext->routeFlags &= ~0x20000U;
            npcActiveRouteContext->targetYaw = npcActiveRouteContext->arrivalYaw;
        }
        RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
        break;
    case 1:
        if (npcActiveRouteContext->flags & 0x1000U)
            return;
        if (npcRouteCharacter) {
            if (npcActiveRouteContext->routeFlags & 0x800U) {
                mtwkCopyTransition(npcRouteOwner->motionWork, 0, npcRouteOwner->rotation.y, 13, 0);
                npcActiveRouteContext->flags |= 0x1000U;
                npcActiveRouteContext->routeFlags &= ~0x800U;
                if (npcActiveRouteContext->otherName)
                    mtwkSetTimeRelated(npcActiveRouteContext->motionWork, 1, 5, 0);
                break;
            }
            if (mtwkCopyTransition(npcRouteMotionWork, 0, 0, 3, 0)) {
                RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
                break;
            }
            if (mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))
                break;
            if (npcActiveRouteContext->otherName) {
                npcActiveRouteContext->moveState = 4;
                AssignCharacterControlFlags(npcRouteCharacter, ccmGetFlags(npcRouteCharacter) & ~0x804U);
                BeginDoorAnimation(static_cast<std::int32_t>(npcActiveRouteContext->otherName), npcActiveRouteContext->scriptOwner,
                    npcActiveRouteContext->doorMotion, 14.0f, npcActiveRouteContext->motionScale * 1.1f);
                mtwkSetSpeed(npcRouteMotionWork, npcActiveRouteContext->motionScale * 1.1f, LowerBody);
                mtwkUpdateWithPosition(npcRouteMotionWork, 0, 0);
            } else
                complete = 1;
        } else if (AdvanceUnstreamedNpcRoute() == 1) {
            if (npcActiveRouteContext->otherName)
                npcActiveRouteContext->localPosition = npcActiveRouteContext->doorPosition;
            complete = 1;
        }
        break;
    case 4:
        if (npcRouteCharacter) {
            if (mtwkSetRegionFlag(npcRouteMotionWork, 0x41, 1, LowerBody)) {
                NpcRouteStateContextPrefix *context = npcActiveRouteContext;
                Vector3 floor = context->localPosition;
                floor.x -= npcRouteOrigin.x;
                floor.z -= npcRouteOrigin.z;
                if (!FindNpcIntersectionInScene(floor, context->moveFlags) && !(npcRouteOwner->flags & 0x200U))
                    BeginNpcRouteStreamOut(npcRouteOwner, 0);
                else
                    AssignCharacterControlFlags(npcRouteCharacter, ccmGetFlags(npcRouteCharacter) | 0x804U);
                complete = 1;
            }
        } else {
            NpcRouteStateContextPrefix *context = npcActiveRouteContext;
            context->localPosition = context->doorPosition;
            complete = 1;
        }
        break;
    case 3:
        if (ClaimDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner) == 1) {
            npcActiveRouteContext->moveState = 1;
            RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
            npcRouteOwner->flags |= 0x100U;
        }
        break;
    }
    if (!complete)
        return;
    if (npcActiveRouteContext->otherName) {
        ReleaseDoor(npcActiveRouteContext->otherName, npcActiveRouteContext->scriptOwner);
        npcActiveRouteContext->otherName = 0;
        npcRouteOwner->flags = (npcRouteOwner->flags | 0x800U) & ~0x100U;
    }
    npcActiveRouteContext->phase = 12;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void BuildNpcRoutePointFrame(const Vector3 *origin, const Vector3i *rotation, Matrix4x4 *matrix, Vector3 *point) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(origin->x, origin->y, origin->z);
    Matrix4x4::SetViewMatrixFromEuler(rotation->x, rotation->y, rotation->z);
    matrix_stack::GetMatrix(matrix);
    PopCurrentMatrix(1);
    InverseTransformPointByMatrix(matrix, point);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void TransformNpcPointToYawFrame(const Vector3 *origin, std::int32_t yaw, Vector3 *point) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::SetViewMatrixFromEuler(0, -yaw, 0);
    Matrix4x4::preTranslate(-origin->x, -origin->y, -origin->z);
    Matrix4x4::TransformPoint(*point, *point);
    PopCurrentMatrix(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static const std::uint16_t npcStairAscendingMotions[15] = {
    0xf056, 0xf056, 0xf083, 0xf056, 0xf0a3, 0xf0e9, 0xf077, 0xf09d, 0x8029, 0xf07d, 0xf05d, 0x8029, 0x8029, 0xf056, 0xf0f4};

std::uint16_t SelectNpcStairAscendingMotion(Character *character) {
    const std::int32_t rig = charGetMotionRigType(character);
    return rig < 15 ? npcStairAscendingMotions[rig] : 0x8029;
}

std::uint16_t SelectNpcStairDescendingMotion(Character *character);
std::uint16_t SelectNpcStairAlternateMotion(Character *character);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t TickNpcRouteStair(Character *character, NpcRouteStateContextPrefix *context) {
    Vector3 origin;
    Vector3 point;
    Vector3i rotation;
    Matrix4x4 frame;
    std::uint32_t flags;
    std::int32_t yaw;
    if (!(context->routeFlags & 4U) || !character)
        return 0;
    switch (static_cast<std::int8_t>(context->avoidancePhase)) {
    case 0:
    case 9: {
        if (!context->stairFlags)
            break;
        if (context->routeFlags & 0x4040U) {
            context->localPosition.x -= npcRouteOrigin.x;
            context->localPosition.z -= npcRouteOrigin.z;
            FindNpcIntersectionInScene(context->localPosition, context->moveFlags);
            context->scenePosition = context->localPosition;
            context->localPosition.x += npcRouteOrigin.x;
            context->localPosition.z += npcRouteOrigin.z;
            context->routeFlags &= ~0x4040U;
        }
        point = context->localPosition;
        point.y -= 0.22499999f;
        point.x -= npcRouteOrigin.x;
        point.z -= npcRouteOrigin.z;
        const std::int32_t count = flddQueryStairOrigin(&point, &origin, &rotation.y);
        if (!count) {
            context->flags &= ~0x10000U;
            if (context->flags & 0x400U) {
                context->flags &= ~0x400U;
                RequestNpcRouteMotion(character, context, 0);
            }
            break;
        }
        if (context->flags & 0x10000U)
            break;
        point = context->localPosition;
        point.x -= npcRouteOrigin.x;
        point.z -= npcRouteOrigin.z;
        yaw = context->savedEuler.y;
        rotation.x = rotation.z = 0;
        BuildNpcRoutePointFrame(&origin, &rotation, &frame, &point);
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(rotation.y));
        context->stairCount = static_cast<std::uint16_t>(count);
        context->stairTarget.x = context->stairTarget.y = 0.0f;
        if (std::fabs(static_cast<float>(yaw)) < 16384.0f) {
            context->stairRemaining = static_cast<std::int8_t>(-(point.z / 0.30000001f));
            context->stairRemaining = context->stairRemaining < 0 ? 0 : context->stairRemaining;
            if (point.z < 0.0f)
                ++context->stairRemaining;
            if (context->stairRemaining >= count)
                break;
            point.y = static_cast<float>(context->stairRemaining) * 0.22499999f;
            point.z = static_cast<float>(context->stairRemaining - 1) * -0.30000001f - 0.1f;
            context->stairTarget.z = static_cast<float>(count - 1) * -0.30000001f - 0.1f;
            context->stairRemaining = static_cast<std::int8_t>(count - context->stairRemaining);
            context->motion = SelectNpcStairAscendingMotion(character);
            context->flags |= 0x800U;
        } else {
            if (point.z > 0.0f)
                break;
            context->stairRemaining = static_cast<std::int8_t>(-(point.z / 0.30000001f) + 1.0f);
            context->stairRemaining = context->stairRemaining > count ? count : context->stairRemaining;
            point.y = static_cast<float>(context->stairRemaining) * 0.22499999f;
            point.z = static_cast<float>(context->stairRemaining) * -0.30000001f + 0.1f;
            rotation.y += 0x8000;
            context->stairTarget.z = 0.1f;
            context->motion = SelectNpcStairDescendingMotion(character);
            context->flags &= ~0x800U;
        }
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&frame);
        Matrix4x4::TransformPoint(point, point);
        Matrix4x4::TransformPoint(context->stairTarget, context->stairTarget);
        PopCurrentMatrix(1);
        context->flags |= 0x400U;
        if (context->flags & 0x1000U) {
            context->motion = GetNpcRestingMotion(character);
            context->avoidancePhase = 5;
        } else {
            context->stairTarget.x += npcRouteOrigin.x;
            context->stairTarget.z += npcRouteOrigin.z;
            context->stairYaw = rotation.y;
            context->stairMotionFrames = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(context->motion)));
            if (context->stairRemaining == 1)
                context->avoidancePhase = 3;
            else
                context->avoidancePhase = 1;
        }
        flags = ccmGetFlags(character);
        mtwkRequestMotion(context->motionWork, context->motion, LowerBody);
        mtwkSetRequestedRootMode(context->motionWork, 1);
        mtwkSetFrameTime(context->motionWork, 10, LowerBody);
        if (!(context->flags & 0x1000U)) {
            mtwkSetSpeed(context->motionWork, context->motionScale * 1.1f, LowerBody);
        } else {
            if (mtwkIsRightFootLower(context->motionWork))
                mtwkSetRegionFlag(context->motionWork, 0x45, 0, LowerBody);
            else
                mtwkSetRegionFlag(context->motionWork, 0x45, 2, LowerBody);
        }
        SetCharacterWorldPosition(character, &point);
        charaSetWorldRot(character, rotation);
        context->savedEuler.y = rotation.y;
        flags &= ~6U;
        flags |= 0x800U;
        AssignCharacterControlFlags(character, flags);
        break;
    }
    case 8: {
        Vector3 offset;
        offset.x = offset.y = 0.0f;
        offset.z = -0.80000001f;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::SetViewMatrixFromEuler(0, context->savedEuler.y, 0);
        Matrix4x4::TransformPoint(offset, offset);
        PopCurrentMatrix(1);
        point = context->localPosition;
        point.x -= npcRouteOrigin.x;
        point.z -= npcRouteOrigin.z;
        offset.x += point.x;
        offset.z += point.z;
        offset.y += point.y;
        if (!ResolveCharacterCylinderOverlap(&offset, &offset, 0.2f))
            context->avoidancePhase = 9;
        break;
    }
    case 5:
        if (!(context->flags & 0x1000U))
            context->avoidancePhase = 0;
        break;
    case 2:
    case 4:
        if (context->routeFlags & 0x800U) {
            context->avoidancePhase = 5;
            context->flags |= 0x1000U;
            context->routeFlags &= ~0x800U;
            mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 13, 0);
            mtwkSetTimeRelated(context->motionWork, 1, 5, 0);
        } else if (!mtwkSetExtraFlag(context->motionWork, 0x3a, 1)) {
            flags = ccmGetFlags(character);
            flags &= ~6U;
            AssignCharacterControlFlags(character, flags);
            --context->avoidancePhase;
            context->savedEuler.y = context->stairYaw;
        }
        break;
    case 1:
        if (mtwkSetRegionFlag(context->motionWork, 0x41, 1, LowerBody)) {
            point = context->localPosition;
            point.x -= npcRouteOrigin.x;
            point.z -= npcRouteOrigin.z;
            if (context->flags & 0x800U)
                point.y += 0.44999999f;
            else
                point.y -= 0.44999999f;
            context->stairRemaining = context->stairRemaining - 2 < 0 ? 0 : context->stairRemaining - 2;
            if (context->stairRemaining == 1)
                context->avoidancePhase = 3;
            else if (!context->stairRemaining)
                context->avoidancePhase = 6;
            SetCharacterWorldPosition(character, &point);
        }
        if (context->routeFlags & 0x800U) {
            float middleDistance = mtwkGetFrame(context->motionWork, LowerBody) - context->stairMotionFrames * 0.5f;
            middleDistance = std::fabs(middleDistance);
            if (!mtwkSetRegionFlag(context->motionWork, 0x41, 1, LowerBody) && !(middleDistance < context->motionScale))
                break;
            if (middleDistance < context->motionScale) {
                point = context->localPosition;
                point.x -= npcRouteOrigin.x;
                point.z -= npcRouteOrigin.z;
                if (context->flags & 0x800U)
                    point.y += 0.22499999f;
                else
                    point.y -= 0.22499999f;
                SetCharacterWorldPosition(character, &point);
            }
            context->avoidancePhase = 5;
            context->flags |= 0x1000U;
            context->routeFlags &= ~0x800U;
            mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 13, 0);
        }
        break;
    case 3:
        point = context->localPosition;
        TransformNpcPointToYawFrame(&context->stairTarget, context->stairYaw, &point);
        if (point.z > 0.0f)
            break;
        context->stairRemaining = 0;
        context->avoidancePhase = 6;
        point = context->localPosition;
        point.x -= npcRouteOrigin.x;
        point.z -= npcRouteOrigin.z;
        if (context->flags & 0x800U)
            point.y += 0.22499999f;
        else
            point.y -= 0.22499999f;
        SetCharacterWorldPosition(character, &point);
    case 6:
        if (context->flags & 0x10U)
            mtwkRequestMotion(context->motionWork, context->motion = SelectNpcRunningMotion(character), LowerBody);
        else
            mtwkRequestMotion(context->motionWork, context->motion = SelectNpcWalkingMotion(character), LowerBody);
        if (!mtwkIsRightFootLower(context->motionWork))
            mtwkSetRegionFlag(context->motionWork, 0x45, 0, LowerBody);
        else
            mtwkSetRegionFlag(context->motionWork, 0x45, 2, LowerBody);
        mtwkSetRequestedRootMode(context->motionWork, 1);
        mtwkSetFrameTime(context->motionWork, 6, LowerBody);
        context->avoidancePhase = 7;
        break;
    case 7: {
        flags = ccmGetFlags(character);
        if (context->lateralPosition < context->laneLow || context->laneHigh < context->lateralPosition)
            context->routeFlags |= 0x800000U;
        context->avoidancePhase = 0;
        context->flags = (context->flags & ~0x400U) | 0x10000U;
        flags &= ~0x800U;
        if (!(context->moveFlags & 0xff00U))
            flags |= 2U;
        flags |= 4U;
        AssignCharacterControlFlags(character, flags);
        break;
    }
    }
    return static_cast<std::int8_t>(context->avoidancePhase);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void OffsetNpcRoutePosition(const Vector3 *origin, Vector3 *position, std::int32_t yaw, float lateral) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(origin->x, origin->y, origin->z);
    Matrix4x4::SetViewMatrixFromEuler(0, yaw, 0);
    Matrix4x4::preTranslate(lateral, 0.0f, 0.0f);
    matrix_stack::GetTranslation(position);
    PopCurrentMatrix(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float ComputeInitialNpcLaneOffset(NpcRouteStateContextPrefix *context, std::int32_t yaw, float clearance) {
    float offset = 0.0f;
    Vector3 point = context->position;
    TransformNpcPointToYawFrame(&context->localPosition, yaw, &point);
    const float low = context->laneLow;
    const float high = context->laneHigh;
    const float x = point.x;
    const float target = -x;
    if (x > offset) {
        if (low + clearance > target)
            offset = low + clearance;
        else
            offset = target;
        if (offset > high)
            offset = high;
    } else {
        if (target > high - clearance)
            offset = high - clearance;
        else
            offset = target;
        if (low > offset)
            offset = low;
    }
    return offset;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InterpolateNpcRouteLanes(NpcRouteStateContextPrefix *context) {
    float fraction = static_cast<float>(context->routeParameter % 64) * 0.015625f;
    if (std::abs(fraction) < FLT_EPSILON && context->routeParameter)
        fraction = 1.0f;
    const float highDelta = context->nextLaneHigh - context->segmentLaneHigh;
    if (highDelta < -context->segmentLength)
        context->interpolatedLaneHigh = context->nextLaneHigh;
    else if (highDelta < 0.0f)
        context->interpolatedLaneHigh = context->segmentLaneHigh + highDelta * fraction;
    else
        context->interpolatedLaneHigh = context->segmentLaneHigh;
    const float lowDelta = context->nextLaneLow - context->segmentLaneLow;
    if (lowDelta > context->segmentLength)
        context->interpolatedLaneLow = context->nextLaneLow;
    else if (lowDelta > 0.0f)
        context->interpolatedLaneLow = context->segmentLaneLow + lowDelta * fraction;
    else
        context->interpolatedLaneLow = context->segmentLaneLow;
    context->laneHigh = context->interpolatedLaneHigh - 0.14f;
    context->laneLow = context->interpolatedLaneLow + 0.14f;
    if (context->laneLow > context->laneHigh) {
        if (context->laneHigh < 0.0f && context->laneLow < 0.0f)
            context->laneHigh = context->laneLow;
        else if (context->laneLow > 0.0f && context->laneHigh > 0.0f)
            context->laneLow = context->laneHigh;
        else
            context->laneLow = context->laneHigh = 0.0f;
    }
    if (context->laneOffset > context->laneHigh)
        context->laneOffset = context->laneHigh;
    else if (context->laneOffset < context->laneLow)
        context->laneOffset = context->laneLow;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t LoadNpcRouteLanes(NpcRouteStateContextPrefix *context) {
    const NpcRouteSpline *node;
    const float *lanes;
    std::int32_t segment;
    if (context->routeType == 2) {
        if (~context->activeNode & 0x8000)
            segment = context->stateTable[context->activeNode & 0x7fff].pointCount - 4;
        else
            segment = 0;
    } else
        segment = context->activeSegment;
    node = &context->stateTable[context->activeNode & 0x7fff];
    context->stairFlags = node->auxiliary[segment];
    lanes = node->laneBounds + segment * 2;
    if (~context->activeNode & 0x8000) {
        context->segmentLaneHigh = lanes[1];
        context->segmentLaneLow = -lanes[0];
        ++segment;
    } else {
        context->segmentLaneHigh = lanes[0];
        context->segmentLaneLow = -lanes[1];
        --segment;
    }
    if (context->routeType == 2 || context->routeType == 3) {
        if (~context->nextNode & 0x8000)
            segment = 0;
        else
            segment = context->stateTable[context->nextNode & 0x7fff].pointCount - 4;
        node = &context->stateTable[context->nextNode & 0x7fff];
        context->stairFlags |= node->auxiliary[segment];
        lanes = node->laneBounds + segment * 2;
        if (~context->nextNode & 0x8000) {
            context->nextLaneHigh = lanes[1];
            context->nextLaneLow = -lanes[0];
        } else {
            context->nextLaneHigh = lanes[0];
            context->nextLaneLow = -lanes[1];
        }
    } else if (segment < 0 || segment >= node->pointCount - 3) {
        context->nextLaneHigh = context->segmentLaneHigh;
        context->nextLaneLow = context->segmentLaneLow;
    } else {
        lanes = node->laneBounds + segment * 2;
        context->stairFlags |= node->auxiliary[segment];
        if (~context->activeNode & 0x8000) {
            context->nextLaneHigh = lanes[1];
            context->nextLaneLow = -lanes[0];
        } else {
            context->nextLaneHigh = lanes[0];
            context->nextLaneLow = -lanes[1];
        }
    }
    context->minimumLaneHigh = context->segmentLaneHigh > context->nextLaneHigh ? context->nextLaneHigh : context->segmentLaneHigh;
    context->maximumLaneLow = context->segmentLaneLow < context->nextLaneLow ? context->nextLaneLow : context->segmentLaneLow;
    InterpolateNpcRouteLanes(context);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRoutePointYaw(const Vector3 *start, const Vector3 *end) {
    Vector3 delta;
    delta.x = end->x - start->x;
    delta.z = end->z - start->z;
    if (!(std::abs(delta.x) < 0.001f && std::abs(delta.z) < 0.001f))
        return static_cast<std::int16_t>(InverseFixedAtan2(delta.x, delta.z));
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SampleNpcRouteTangent(NpcRouteStateContextPrefix *context, std::int32_t step, std::int32_t *yaw) {
    Vector3 point;
    context->routeParameter += step;
    const std::int32_t result = SampleNpcRouteWindow(context, &point, 0);
    context->routeParameter -= step;
    point.x -= context->position.x;
    point.z -= context->position.z;
    if (!(std::abs(point.x) < 0.001f && std::abs(point.z) < 0.001f))
        *yaw = static_cast<std::int16_t>(InverseFixedAtan2(point.x, point.z));
    else
        *yaw = 0;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SampleAndOffsetNpcRouteTarget(Vector3 *position, float lateral, std::int32_t *yaw) {
    Vector3 delta = npcActiveRouteContext->position;
    Vector3 sample;
    const std::int32_t result = SampleNpcRouteWindow(npcActiveRouteContext, &sample, 1);
    npcActiveRouteContext->position = sample;
    am2Vector3Subtract(&delta, &sample, &delta);
    if (!(std::abs(delta.x) < 0.001f && std::abs(delta.z) < 0.001f))
        *yaw = static_cast<std::int16_t>(InverseFixedAtan2(delta.x, delta.z));
    else
        *yaw = 0;
    OffsetNpcRoutePosition(&sample, position, *yaw, lateral);
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t AdvanceUnstreamedNpcRoute() {
    std::int32_t result = 0;
    if (npcActiveRouteContext->flags & 0x1000U)
        return 0;
    float step;
    if (npcActiveRouteContext->routeFlags & 0x10000U)
        step = npcActiveRouteContext->motionStep * 8.0f;
    else
        step = npcActiveRouteContext->motionStep;
    if (npcActiveRouteContext->flags & 0x80000U) {
        npcActiveRouteContext->flags &= ~0x80000U;
        npcActiveRouteContext->position = npcActiveRouteContext->savedSplinePosition;
        npcActiveRouteContext->moveTarget = npcActiveRouteContext->savedMoveTarget;
    }
    Vector3 delta;
    delta.x = npcActiveRouteContext->moveTarget.x - npcActiveRouteContext->localPosition.x;
    delta.z = npcActiveRouteContext->moveTarget.z - npcActiveRouteContext->localPosition.z;
    delta.y = 0.0f;
    const float distance = Vector3::CalcMagnitude(delta);
    if (distance > 20.0f) {
        NpcRouteStateContextPrefix *context = npcActiveRouteContext;
        context->localPosition = context->moveTarget;
    }
    if (step > distance) {
        result = 1;
        {
            NpcRouteStateContextPrefix *context = npcActiveRouteContext;
            context->localPosition = context->moveTarget;
        }
    } else if (std::abs(distance) > FLT_EPSILON) {
        npcActiveRouteContext->localPosition.x += delta.x * step / distance;
        npcActiveRouteContext->localPosition.z += delta.z * step / distance;
        npcActiveRouteContext->localPosition.y = npcActiveRouteContext->moveTarget.y;
    }
    if (npcActiveRouteContext->routeFlags & 0x800U) {
        npcActiveRouteContext->flags |= 0x1000U;
        npcActiveRouteContext->routeFlags &= ~0x800U;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t AdvanceNpcRouteByDistance(shen_name name, NpcRouteStateContextPrefix *context, float distance) {
    if (!context && !(context = FindNpcRouteStateContext(name)))
        return 0;
    std::int32_t direction;
    const NpcRouteSpline *const table = context->stateTable;
    if (distance < 0.0f) {
        direction = -1;
        distance *= -1.0f;
    } else {
        direction = 1;
    }
    std::int32_t route = context->activeRouteIndex;
    const std::int16_t *const path = context->routePath;
    std::int32_t segment = context->activeSegment, available;
    if (direction == 1)
        available = 64 - static_cast<std::int16_t>(context->routeParameter % 64);
    else
        available = static_cast<std::int16_t>(context->routeParameter % 64);
    bool connection = context->routeType == 2;
    std::int32_t count = table[path[route] & 0x7fff].pointCount - 3, points;
    std::int32_t advanced = 0;
    float length, step;
    for (;;) {
        if (connection)
            length = table[path[route] & 0x7fff].connectionLengths[context->routeConnections[route]];
        else
            length = table[path[route] & 0x7fff].segmentLengths[segment + 1];
        step = length * available * 0.015625f;
        if (distance - step > 0.0f) {
            distance -= step;
            advanced += available;
            available = 64;
            connection = false;
            if (~path[route] & 0x8000)
                segment += direction;
            else
                segment -= direction;
            if (segment > count || segment < -1) {
                route += direction;
                if (route < 0 || route >= context->routeCount)
                    break;
                points = table[path[route] & 0x7fff].pointCount;
                count = points - 3;
                if (~path[route] & 0x8000) {
                    if (direction == 1)
                        segment = 0;
                    else
                        segment = points - 4;
                } else {
                    segment = 0;
                    if (direction == 1)
                        segment = count - 1;
                }
            } else if (segment == count || segment == -1)
                connection = true;
        } else {
            advanced += distance * 64.0f / length;
            break;
        }
    }
    advanced = advanced * direction + context->routeParameter;
    if (advanced > context->upperParameter)
        advanced = context->upperParameter;
    else if (advanced < context->lowerParameter)
        advanced = context->lowerParameter;
    return advanced;
}

extern "C" float fmodf(float value, float divisor);

float AdvanceEstimatedNpcRouteByDistance(shen_name name, NpcRouteStateContextPrefix *context, float distance) {
    if (!context && !(context = FindNpcRouteStateContext(name)))
        return 0.0f;
    const float parameter = EstimateNpcRouteParameter(0, context);
    const std::int32_t savedParameter = context->routeParameter;
    context->routeParameter = static_cast<std::int32_t>(parameter);
    UpdateNpcRouteWindow(context, 0);
    std::int32_t direction;
    const NpcRouteSpline *const table = context->stateTable;
    if (distance < 0.0f) {
        direction = -1;
        distance *= -1.0f;
    } else {
        direction = 1;
    }
    std::int32_t route = context->activeRouteIndex;
    const std::int16_t *const path = context->routePath;
    std::int32_t segment = context->activeSegment, points;
    float available;
    if (direction == 1)
        available = 64.0f - fmodf(parameter, 64.0f);
    else
        available = fmodf(parameter, 64.0f);
    bool connection = context->routeType == 2;
    std::int32_t count = table[path[route] & 0x7fff].pointCount - 3;
    float advanced = 0.0f;
    float length, step;
    for (;;) {
        if (connection)
            length = table[path[route] & 0x7fff].connectionLengths[context->routeConnections[route]];
        else
            length = table[path[route] & 0x7fff].segmentLengths[segment + 1];
        step = length * available * 0.015625f;
        if (distance - step > 0.0f) {
            distance -= step;
            advanced += available;
            available = 64.0f;
            connection = false;
            if (~path[route] & 0x8000)
                segment += direction;
            else
                segment -= direction;
            if (segment > count || segment < -1) {
                route += direction;
                if (route < 0 || route >= context->routeCount)
                    break;
                points = table[path[route] & 0x7fff].pointCount;
                count = points - 3;
                if (~path[route] & 0x8000) {
                    if (direction == 1)
                        segment = 0;
                    else
                        segment = points - 4;
                } else {
                    segment = 0;
                    if (direction == 1)
                        segment = count - 1;
                }
            } else if (segment == count || segment == -1)
                connection = true;
        } else {
            advanced += distance * 64.0f / length;
            break;
        }
    }
    advanced = advanced * direction + parameter;
    if (advanced > context->upperParameter)
        advanced = context->upperParameter;
    else if (advanced < context->lowerParameter)
        advanced = context->lowerParameter;
    context->routeParameter = savedParameter;
    UpdateNpcRouteWindow(context, 0);
    return advanced;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t CheckNpcStairApproach(NpcRouteStateContextPrefix *context) {
    if (!context->stairFlags)
        return 0;
    Vector3i rotation;
    Vector3 position = context->localPosition;
    position.x -= npcRouteOrigin.x;
    position.z -= npcRouteOrigin.z;
    if (context->routeFlags & 0x4040U)
        FindNpcIntersectionInScene(position, context->moveFlags);
    position.y -= 0.225f;
    return flddQueryStairOrigin(&position, &position, &rotation.y);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void TickNpcRouteApproach() {
    bool complete = false;
    switch (static_cast<std::int8_t>(npcActiveRouteContext->moveState)) {
    case 0:
        UpdateNpcRouteWindow(npcActiveRouteContext, 1);
        SampleNpcRouteWindow(npcActiveRouteContext, &npcActiveRouteContext->position, 1);
        SampleNpcRouteTangent(npcActiveRouteContext, 1, &npcActiveRouteContext->tangentYaw);
        LoadNpcRouteLanes(npcActiveRouteContext);
        npcActiveRouteContext->routeParameter += npcActiveRouteContext->stride;
        npcActiveRouteContext->laneOffset = ComputeInitialNpcLaneOffset(npcActiveRouteContext, npcActiveRouteContext->tangentYaw, 0.0f);
        SampleAndOffsetNpcRouteTarget(&npcActiveRouteContext->moveTarget, npcActiveRouteContext->laneOffset, &npcActiveRouteContext->tangentYaw);
        npcActiveRouteContext->savedEuler.y = GetNpcRoutePointYaw(&npcActiveRouteContext->localPosition, &npcActiveRouteContext->moveTarget);
        npcActiveRouteContext->targetYaw = npcActiveRouteContext->tangentYaw;
        npcActiveRouteContext->sceneTarget = npcActiveRouteContext->moveTarget;
        npcActiveRouteContext->sceneTarget.x -= npcRouteOrigin.x;
        npcActiveRouteContext->sceneTarget.z -= npcRouteOrigin.z;
        if (npcActiveRouteContext->flags & 0x20000U) {
            complete = true;
            if (npcRouteCharacter) {
                if (CheckNpcStairApproach(npcActiveRouteContext)) {
                    complete = false;
                    npcActiveRouteContext->moveState = 5;
                    mtwkCopyTransition(npcRouteMotionWork, 0, npcActiveRouteContext->tangentYaw, 13, 0);
                } else {
                    mtwkCopyTransition(npcRouteMotionWork, 0, npcActiveRouteContext->tangentYaw, 5, 2);
                    if (npcActiveRouteContext->flags & 0x10U)
                        mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, 0, 8, 0);
                }
            }
        } else {
            RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
            npcActiveRouteContext->moveState = 1;
        }
        break;
    case 5:
        do {
            if (!npcRouteCharacter || !mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))
                complete = true;
        } while (0);
        break;
    case 1:
        if (npcActiveRouteContext->flags & 0x1000U)
            return;
        if (npcRouteCharacter) {
            if (npcActiveRouteContext->routeFlags & 0x800U) {
                mtwkCopyTransition(npcRouteOwner->motionWork, 0, npcRouteOwner->rotation.y, 13, 0);
                npcActiveRouteContext->flags |= 0x1000U;
                npcActiveRouteContext->routeFlags &= ~0x800U;
                return;
            }
            if (mtwkCopyTransition(npcRouteMotionWork, 0, 0, 3, 0)) {
                RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
                break;
            }
            if (!mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))
                complete = true;
        } else if (AdvanceUnstreamedNpcRoute() == 1)
            complete = true;
        break;
    }
    if (complete) {
        npcActiveRouteContext->phase = 7;
        npcActiveRouteContext->moveState = 2;
        npcActiveRouteContext->savedEuler.y = npcActiveRouteContext->tangentYaw;
        if (npcActiveRouteContext->flags & 0x20000U)
            RequestNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1);
        npcActiveRouteContext->routeFlags |= 0x800000U;
        npcActiveRouteContext->flags |= 0x20000U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void TickNpcRouteTraversal() {
    switch (static_cast<std::int8_t>(npcActiveRouteContext->moveState)) {
    case 2: {
        UpdateNpcRouteWindow(npcActiveRouteContext, 1);
        if (npcActiveRouteContext->activeSegment != npcActiveRouteContext->previousSegment) {
            npcActiveRouteContext->stride = ComputeNpcRouteStride(npcActiveRouteContext);
            LoadNpcRouteLanes(npcActiveRouteContext);
            npcActiveRouteContext->previousSegment = npcActiveRouteContext->activeSegment;
            if (npcActiveRouteContext->routeType == 3) {
                if (npcActiveRouteContext->stateTable[npcActiveRouteContext->nextNode & 0x7fff].flags & 0x400U)
                    npcRouteOwner->motionAttachments[0].flags |= 0x40U;
                else
                    npcRouteOwner->motionAttachments[0].flags &= ~0x40U;
            }
        }
        const std::int32_t previousParameter = npcActiveRouteContext->routeParameter;
        npcActiveRouteContext->routeParameter += npcActiveRouteContext->stride;
        if (!(npcActiveRouteContext->avoidanceFlags & 2U) && !npcActiveRouteContext->avoidanceResult) {
            npcActiveRouteContext->laneOffset -= npcActiveRouteContext->laneStep;
            if (npcActiveRouteContext->laneOffset > npcActiveRouteContext->laneHigh)
                npcActiveRouteContext->laneOffset = npcActiveRouteContext->laneHigh;
            else if (npcActiveRouteContext->laneOffset < npcActiveRouteContext->laneLow)
                npcActiveRouteContext->laneOffset = npcActiveRouteContext->laneLow;
        }
        InterpolateNpcRouteLanes(npcActiveRouteContext);
        if (npcActiveRouteContext->upperParameter < npcActiveRouteContext->routeParameter ||
            !SampleAndOffsetNpcRouteTarget(&npcActiveRouteContext->moveTarget, npcActiveRouteContext->laneOffset, &npcActiveRouteContext->tangentYaw)) {
            npcActiveRouteContext->phase = 8;
            npcActiveRouteContext->flags &= ~0x20000U;
            if (npcActiveRouteContext->flags & 0x100U) {
                npcActiveRouteContext->phase = 12;
                npcActiveRouteContext->savedEuler.y =
                    ComputeNpcAreaPointYaw(npcActiveRouteContext->stateTable, npcActiveRouteContext->currentAreaPoint) + 0x8000;
                if (npcRouteCharacter)
                    mtwkCopyTransition(npcRouteMotionWork, 0, npcActiveRouteContext->savedEuler.y, 13, 0);
            }
            npcActiveRouteContext->moveState = 0;
            if (npcRouteCharacter && (npcActiveRouteContext->flags & 0x20U)) {
                mtwkUpdateWithPosition(npcRouteMotionWork, 0, 0);
                npcActiveRouteContext->flags &= ~0x20U;
            }
            break;
        }
        npcActiveRouteContext->moveState = 1;
        npcActiveRouteContext->targetParameter = AdvanceNpcRouteByDistance(0, npcActiveRouteContext, npcActiveRouteContext->motionStep * 30.0f + 3.0f);
        if (previousParameter / 64 != static_cast<std::int16_t>(npcActiveRouteContext->targetParameter / 64)) {
            npcActiveRouteContext->routeParameter = npcActiveRouteContext->targetParameter;
            UpdateNpcRouteWindow(npcActiveRouteContext, 1);
            npcActiveRouteContext->routeParameter = previousParameter;
            UpdateNpcRouteWindow(npcActiveRouteContext, 0);
            npcActiveRouteContext->routeParameter += npcActiveRouteContext->stride;
        }
    }
    case 1:
        TickNpcRouteMovement();
        break;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void TickNpcRouteMovement() {
    std::int32_t direction = 0;
    Vector3 offset;
    if (npcActiveRouteContext->flags & 0x1000U)
        return;
    if (!npcRouteCharacter) {
        if (AdvanceUnstreamedNpcRoute() == 1) {
            npcActiveRouteContext->moveState = 2;
            npcActiveRouteContext->savedEuler.y = npcActiveRouteContext->tangentYaw;
        }
        return;
    }
    if (!TickNpcRouteStair(npcRouteCharacter, npcActiveRouteContext) && (npcActiveRouteContext->routeFlags & 0x800U)) {
        mtwkCopyTransition(npcRouteOwner->motionWork, 0, npcRouteOwner->rotation.y, 13, 0);
        npcActiveRouteContext->flags |= 0x1000U;
        npcActiveRouteContext->routeFlags &= ~0x800U;
        return;
    }
    {
        NpcRouteStateContextPrefix *context = npcActiveRouteContext;
        offset = context->moveTarget;
        TransformNpcPointToYawFrame(&context->localPosition, context->tangentYaw, &offset);
    }
    npcActiveRouteContext->lateralPosition = npcActiveRouteContext->laneOffset - offset.x;
    npcActiveRouteContext->laneStep = offset.x;
    npcActiveRouteContext->movementProgress = offset.z;
    if (offset.z > -npcActiveRouteContext->motionStep) {
        if (npcActiveRouteContext->flags & 0x80000U) {
            npcActiveRouteContext->flags &= ~0x80000U;
            npcActiveRouteContext->laneOffset = npcActiveRouteContext->savedLaneOffset;
            npcActiveRouteContext->position = npcActiveRouteContext->savedSplinePosition;
            npcActiveRouteContext->moveTarget = npcActiveRouteContext->savedMoveTarget;
            offset = npcActiveRouteContext->moveTarget;
            TransformNpcPointToYawFrame(&npcActiveRouteContext->localPosition, npcActiveRouteContext->tangentYaw, &offset);
            npcActiveRouteContext->lateralPosition = npcActiveRouteContext->laneOffset - offset.x;
            npcActiveRouteContext->laneStep = offset.x;
            npcActiveRouteContext->movementProgress = offset.z;
        } else
            npcActiveRouteContext->moveState = 2;
    }
    if (npcRouteOwner->flags & 0x40U) {
        npcActiveRouteContext->savedEuler.y = npcActiveRouteContext->tangentYaw;
        charaSetWorldRot(npcRouteCharacter, npcActiveRouteContext->savedEuler);
        return;
    }
    if (npcActiveRouteContext->flags & 0x400U) {
        if (npcActiveRouteContext->flags & 0x20U) {
            npcActiveRouteContext->flags &= ~0x20U;
            mtwkUpdateWithPosition(npcRouteMotionWork, 0, 0);
        }
        return;
    }
    if (!(npcActiveRouteContext->flags & 0x400U)) {
        const float fromLow = npcActiveRouteContext->interpolatedLaneLow - npcActiveRouteContext->lateralPosition;
        const float fromHigh = -(npcActiveRouteContext->interpolatedLaneHigh - npcActiveRouteContext->lateralPosition);
        if (fromLow > 0.0f || fromHigh > 0.0f) {
            if (!(npcActiveRouteContext->routeFlags & 0x800000U)) {
                if (fromLow > 0.0f) {
                    offset.x -= fromLow;
                    npcActiveRouteContext->lateralPosition = npcActiveRouteContext->interpolatedLaneLow;
                } else {
                    offset.x += fromHigh;
                    npcActiveRouteContext->lateralPosition = npcActiveRouteContext->interpolatedLaneHigh;
                }
                npcActiveRouteContext->laneStep = offset.x;
                offset.x *= -1.0f;
                offset.z *= -1.0f;
                offset.y *= -1.0f;
                matrix_stack::PushUnitMatrix();
                matrix_stack::TranslateVector(&npcActiveRouteContext->moveTarget);
                Matrix4x4::SetViewMatrixFromEuler(0, npcActiveRouteContext->tangentYaw, 0);
                Matrix4x4::TransformPoint(offset, npcActiveRouteContext->localPosition);
                PopCurrentMatrix(1);
                npcActiveRouteContext->routeFlags |= 0x80000U;
            }
        } else
            npcActiveRouteContext->routeFlags &= ~0x800000U;
    }
    const std::int32_t yawDifference = static_cast<std::int16_t>(npcActiveRouteContext->tangentYaw - npcActiveRouteContext->savedEuler.y);
    npcActiveRouteContext->retainedfc = yawDifference;
    const std::int8_t previousAvoidance = npcActiveRouteContext->avoidanceResult;
    if (npcActiveRouteContext->routeFlags & 2U) {
        if (!UpdateNpcRouteAvoidance()) {
            if (npcActiveRouteContext->avoidanceFlags & 1U) {
                npcActiveRouteContext->avoidanceFlags &= ~1U;
                SetNpcRouteMotionState(npcActiveRouteContext, 0, 3);
            } else if (previousAvoidance > 6) {
                mtwkSetSpeed(npcActiveRouteContext->motionWork, npcActiveRouteContext->motionScale, LowerBody);
                mtwkCopyTransition(npcRouteMotionWork, 0, npcActiveRouteContext->tangentYaw, 5, 2);
                if (npcActiveRouteContext->flags & 0x10U)
                    mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, 0, 8, 0);
            }
            npcActiveRouteContext->avoidanceWaitFrames = 0;
        } else if (static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult) == 2 && !(npcActiveRouteContext->routeFlags & 0x10U) &&
            !IsNpcScheduleSeedName(npcActiveRouteContext->scriptOwner) && ++npcActiveRouteContext->avoidanceWaitFrames > 90) {
            InitializeNpcAreaMovement(
                npcActiveRouteContext->scriptOwner, npcActiveRouteContext, npcActiveRouteContext->pendingAreaPoint, npcActiveRouteContext->currentAreaPoint);
            SetNpcLocomotionScale(npcActiveRouteContext, 0, npcRouteOwner->motionSpeed);
            if (!(npcActiveRouteContext->routeFlags & 0x4000000U))
                npcActiveRouteContext->routeFlags |= 0x4000000U;
            else
                npcActiveRouteContext->routeFlags &= ~0x4000000U;
            return;
        }
    }
    if (!npcActiveRouteContext->avoidanceResult && (npcActiveRouteContext->routeFlags & 0x200U))
        UpdateNpcAmbientLook();
    if (mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1)) {
        if (npcActiveRouteContext->flags & 0x20U) {
            npcActiveRouteContext->flags &= ~0x20U;
            mtwkUpdateWithPosition(npcRouteMotionWork, 0, 0);
        }
        npcActiveRouteContext->savedEuler.y = mtwkGetEntityYaw(npcRouteMotionWork);
        npcActiveRouteContext->retainedf8 = 0;
        npcActiveRouteContext->avoidanceFlags |= 2U;
    } else if (npcActiveRouteContext->avoidanceFlags & 2U) {
        GetCharacterWorldRotation(npcRouteCharacter, &npcActiveRouteContext->savedEuler);
        npcActiveRouteContext->avoidanceFlags &= 0xf5U;
        if (static_cast<std::int8_t>(npcActiveRouteContext->moveState) != 2) {
            npcActiveRouteContext->laneOffset = npcActiveRouteContext->lateralPosition;
            if (npcActiveRouteContext->laneOffset > npcActiveRouteContext->laneHigh)
                npcActiveRouteContext->laneOffset = npcActiveRouteContext->laneHigh;
            else if (npcActiveRouteContext->laneOffset < npcActiveRouteContext->laneLow)
                npcActiveRouteContext->laneOffset = npcActiveRouteContext->laneLow;
            npcActiveRouteContext->laneStep = npcActiveRouteContext->laneOffset - npcActiveRouteContext->lateralPosition;
            OffsetNpcRoutePosition(
                &npcActiveRouteContext->position, &npcActiveRouteContext->moveTarget, npcActiveRouteContext->tangentYaw, npcActiveRouteContext->laneOffset);
            return;
        }
    }
    if (static_cast<std::uint8_t>(npcActiveRouteContext->avoidanceResult - 7U) < 2 || (npcActiveRouteContext->avoidanceFlags & 1U))
        return;
    offset.x = npcActiveRouteContext->moveTarget.x - npcActiveRouteContext->localPosition.x;
    offset.z = npcActiveRouteContext->moveTarget.z - npcActiveRouteContext->localPosition.z;
    std::int32_t targetYaw, targetDifference;
    if (!(std::fabs(offset.x) < 0.001f && std::fabs(offset.z) < 0.001f)) {
        targetYaw = InverseFixedAtan2(offset.x, offset.z);
        targetDifference = static_cast<std::int16_t>(targetYaw - npcActiveRouteContext->savedEuler.y);
    } else {
        targetYaw = npcActiveRouteContext->savedEuler.y;
        targetDifference = 0;
    }
    if (npcActiveRouteContext->laneHigh - npcActiveRouteContext->laneLow > 0.4f && !npcActiveRouteContext->avoidanceResult) {
        const float belowLow = npcActiveRouteContext->laneLow - npcActiveRouteContext->lateralPosition;
        const float aboveHigh = -(npcActiveRouteContext->laneHigh - npcActiveRouteContext->lateralPosition);
        if (belowLow > 0.0f || aboveHigh > 0.0f) {
            targetYaw = npcActiveRouteContext->tangentYaw + static_cast<std::int16_t>(targetYaw - npcActiveRouteContext->tangentYaw) / 2;
            mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, targetYaw, 5, 2);
            if (npcActiveRouteContext->flags & 0x10U)
                mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, 0, 8, 0);
            return;
        }
    }
    if (npcActiveRouteContext->avoidanceFlags & 8U)
        return;
    if (std::fabs(static_cast<float>(yawDifference)) > 8192.0f) {
        mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, npcActiveRouteContext->tangentYaw, 5, 2);
        if (npcActiveRouteContext->flags & 0x10U)
            mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, 0, 8, 0);
        return;
    }
    float amount = 0.0f;
    if (mtwkSetExtraFlag(npcRouteMotionWork, 0x3a, 1))
        return;
    if ((npcActiveRouteContext->routeFlags & 0x80U) && std::fabs(npcActiveRouteContext->laneStep) > 0.4f && npcActiveRouteContext->movementProgress < 0.0f) {
        amount = static_cast<float>(targetDifference);
        direction = targetDifference < 0 ? -1 : targetDifference > 0;
    } else if (std::fabs(npcActiveRouteContext->laneStep) > 0.2f && npcActiveRouteContext->movementProgress < 0.0f) {
        std::int32_t side;
        if (npcActiveRouteContext->laneStep < 0.0f)
            side = -1;
        else
            side = npcActiveRouteContext->laneStep > 0.0f;
        const std::int32_t relative = side * yawDifference;
        if (relative > 8192) {
            amount = static_cast<float>(yawDifference);
            direction = side;
        } else if (relative < 3640) {
            amount = static_cast<float>(targetDifference);
            direction = -side;
        }
    } else {
        amount = static_cast<float>(yawDifference);
        direction = yawDifference < 0 ? -1 : yawDifference > 0;
    }
    npcActiveRouteContext->retainedf8 = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(npcActiveRouteContext->retainedf8)) +
        (std::fabs(amount * 0.004f) * npcActiveRouteContext->motionStep * 20.0f + 2.0f));
    if (std::fabs(amount) < static_cast<float>(static_cast<std::int32_t>(npcActiveRouteContext->retainedf8)))
        npcActiveRouteContext->retainedf8 = static_cast<std::int32_t>(std::fabs(amount));
    if (static_cast<std::int32_t>(npcActiveRouteContext->retainedf8) > npcActiveRouteContext->maximumYawStep)
        npcActiveRouteContext->retainedf8 = npcActiveRouteContext->maximumYawStep;
    npcActiveRouteContext->savedEuler.y = (direction * npcActiveRouteContext->retainedf8 + npcActiveRouteContext->savedEuler.y) & 0xffff;
    charaSetWorldRot(npcRouteCharacter, npcActiveRouteContext->savedEuler);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateNpcAmbientLook() {
    if (npcRouteCharacter) {
        if (npcActiveRouteContext->doorWaitFrames < 0) {
            if (npcActiveRouteContext->flags & 0x20U) {
                npcActiveRouteContext->flags &= ~0x20U;
                mtwkUpdateWithPosition(npcRouteMotionWork, 0, 0);
            }
            const std::int32_t duration = static_cast<std::int8_t>(npcRouteOwner->scriptRecord[5]) * 120;
            npcActiveRouteContext->doorWaitFrames = static_cast<std::int16_t>(static_cast<float>(duration) * dc::sdk::RandomFloat());
            const std::int32_t half = duration >> 1;
            if (npcActiveRouteContext->doorWaitFrames < half)
                npcActiveRouteContext->doorWaitFrames = static_cast<std::int16_t>(half);
        }
        if (--npcActiveRouteContext->doorWaitFrames == 120) {
            const std::int32_t point = static_cast<std::int32_t>(static_cast<float>(npcScheduleAreaPointCount) * dc::sdk::RandomFloat());
            Vector3 target = *GetNpcAreaPointPosition(point);
            am2Vector3NormalizeInPlace(&target);
            Vector3 bone;
            mtwkGetBonePosition(npcRouteMotionWork, 6, &bone);
            target.x = npcActiveRouteContext->localPosition.x + target.x * 2.0f;
            target.z = npcActiveRouteContext->localPosition.z + target.z * 2.0f;
            if (!((*(std::uint32_t *)&bone.y > 0x007fffffU && *(std::uint32_t *)&bone.y <= 0x7f7fffffU) ||
                    (*(std::uint32_t *)&bone.y > 0x807fffffU && *(std::uint32_t *)&bone.y <= 0xff7fffffU) || *(std::uint32_t *)&bone.y == 0 ||
                    *(std::uint32_t *)&bone.y == 0x80000000U))
                bone.y = 0.0f;
            target.y =
                bone.y + static_cast<float>(point % 50) * 0.01f - static_cast<float>(static_cast<std::int8_t>(npcRouteOwner->scriptRecord[8]) + 1) * 0.2f;
            target.x -= npcRouteOrigin.x;
            target.z -= npcRouteOrigin.z;
            mtwkUpdateWithPosition(npcRouteMotionWork, &target, 0);
            npcActiveRouteContext->flags |= 0x20U;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ResetNpcRouteAvoidanceCandidates() {
    NpcRouteAvoidanceCandidate *candidate = npcRouteAvoidanceCandidates;
    for (std::int32_t index = 0; index < 5; ++index, ++candidate) {
        if (index)
            candidate->previous = candidate - 1;
        if (index != 4)
            candidate->next = candidate + 1;
        candidate->distance = 10.0f;
        candidate->kind = 0;
    }
    npcRouteAvoidanceCandidates[0].previous = 0;
    npcRouteAvoidanceCandidates[4].next = 0;
    npcRouteAvoidanceFirst = npcRouteAvoidanceCandidates;
    npcRouteAvoidanceLast = &npcRouteAvoidanceCandidates[4];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
NpcRouteAvoidanceCandidate *InsertNpcRouteAvoidanceCandidate(float distance, std::int32_t yaw, std::int32_t yawDistance) {
    NpcRouteAvoidanceCandidate *candidate = 0;
    for (NpcRouteAvoidanceCandidate *position = npcRouteAvoidanceFirst; position; position = position->next) {
        if (distance > position->distance)
            continue;
        candidate = npcRouteAvoidanceLast;
        candidate->distance = distance;
        candidate->yaw = yaw;
        candidate->yawDistance = yawDistance;
        NpcRouteAvoidanceCandidate *const previous = position->previous;
        if (!previous) {
            candidate->previous->next = previous;
            npcRouteAvoidanceLast = candidate->previous;
            npcRouteAvoidanceFirst = candidate;
            candidate->previous = previous;
            candidate->next = position;
            position->previous = candidate;
        } else if (position->next) {
            npcRouteAvoidanceLast->previous->next = 0;
            npcRouteAvoidanceLast = npcRouteAvoidanceLast->previous;
            position->previous->next = candidate;
            candidate->previous = position->previous;
            position->previous = candidate;
            candidate->next = position;
        }
        return candidate;
    }
    return candidate;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t UpdateNpcRouteAvoidance() {
    Vector3 position, center, headingFrame, routeFrame, future;
    void *other;
    std::int8_t previousResult;
    std::int32_t heading, yaw, angle, kind;
    NpcRouteAvoidanceCandidate *candidate;
    float centerX, centerZ, ownRadius, distance, x, z;
    float extent = 0.0f, combinedRadius = 0.0f;
    other = npcActiveRouteContext->other;
    npcActiveRouteContext->other = 0;
    previousResult = static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult);
    npcActiveRouteContext->avoidanceResult = 0;
    if (npcActiveRouteContext->avoidancePhase)
        return 5;
    heading = npcActiveRouteContext->savedEuler.y;
    centerX = npcRouteOwner->streamCenter.x;
    centerZ = npcRouteOwner->streamCenter.z;
    ownRadius = npcRouteOwner->streamRadius;
    ResetNpcRouteAvoidanceCandidates();
    distance = 10.0f;
    if (npcAreaRestrictionCount) {
        NpcAreaRestriction *area = npcAreaRestrictions;
        for (std::int32_t index = 0; index < 8; ++index, ++area) {
            if (!area->name || std::abs(area->center.y - npcActiveRouteContext->scenePosition.y) > 1.8f)
                continue;
            x = area->center.x - centerX;
            z = area->center.z - centerZ;
            extent = CalculatePlanarMagnitude(x, z) - ownRadius - area->radius;
            if (extent < -0.5f || extent > distance)
                continue;
            yaw = InverseFixedAtan2(x, z);
            angle = static_cast<std::int32_t>(std::abs(static_cast<float>(static_cast<std::int16_t>(yaw - heading))));
            if (angle > 0x4000 && other != area)
                continue;
            candidate = InsertNpcRouteAvoidanceCandidate(extent, yaw, angle);
            if (candidate) {
                candidate->source = area;
                candidate->kind = 2;
            }
        }
    }
    distance = npcRouteAvoidanceLast->distance;
    if (npcRouteTableReady && !(std::abs(npcPlayerPosition.y - (&npcActiveRouteContext->scenePosition)->y) > 1.8f))
        do {
            x = npcPlayerPosition.x - centerX;
            z = npcPlayerPosition.z - centerZ;
            extent = CalculatePlanarMagnitude(x, z) - ownRadius - npcPlayerRadius;
            if (extent > distance || (std::abs(x) < 0.001f && std::abs(z) < 0.001f))
                break;
            yaw = InverseFixedAtan2(x, z);
            angle = static_cast<std::int32_t>(std::abs(static_cast<float>(static_cast<std::int16_t>(yaw - heading))));
            if (angle > 0x4000 && other != &npcPlayerPosition)
                break;
            candidate = InsertNpcRouteAvoidanceCandidate(extent, yaw, angle);
            if (candidate) {
                candidate->source = &npcPlayerPosition;
                candidate->kind = 1;
            }
        } while (0);
    distance = npcRouteAvoidanceLast->distance;
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry != npcCrowdBookmark && entry; entry = entry->next) {
        if (!entry->node || entry->name == npcActiveRouteContext->scriptOwner || (entry->flags & 0x40U) ||
            std::abs(entry->localPosition.y - npcActiveRouteContext->scenePosition.y) > 1.8f)
            continue;
        x = entry->streamCenter.x - centerX;
        z = entry->streamCenter.z - centerZ;
        extent = CalculatePlanarMagnitude(x, z) - ownRadius - entry->streamRadius;
        if (extent > distance || (std::abs(x) < 0.001f && std::abs(z) < 0.001f))
            continue;
        yaw = InverseFixedAtan2(x, z);
        angle = static_cast<std::int32_t>(std::abs(static_cast<float>(static_cast<std::int16_t>(yaw - heading))));
        if (angle > 0x4000 && other != entry)
            continue;
        if (entry->stateContext.other == npcRouteOwner) {
            if (!(entry->stateContext.motionStep < npcActiveRouteContext->motionStep))
                continue;
            entry->stateContext.avoidanceResult = 0;
        }
        candidate = InsertNpcRouteAvoidanceCandidate(extent, yaw, angle);
        if (candidate) {
            candidate->source = entry;
            candidate->kind = 3;
        }
    }
    distance = npcRouteAvoidanceFirst->distance;
    const std::int32_t obstacleYaw = npcRouteAvoidanceFirst->yaw;
    other = npcRouteAvoidanceFirst->source;
    kind = npcRouteAvoidanceFirst->kind;
    switch (kind) {
    case 0:
        extent = 0.0f;
        break;
    case 1:
        position = npcPlayerCenter;
        extent = (npcActiveRouteContext->motionStep > npcPlayerDisplacement ? npcActiveRouteContext->motionStep : npcPlayerDisplacement) * 30.0f + 3.0f;
        combinedRadius = npcPlayerRadius + 0.05f;
        break;
    case 2:
        position = static_cast<NpcAreaRestriction *>(other)->center;
        extent = npcActiveRouteContext->motionStep * 30.0f + 3.0f;
        combinedRadius = static_cast<NpcAreaRestriction *>(other)->radius + 0.05f;
        break;
    case 3:
        position = static_cast<NpcRouteLiveEntry *>(other)->streamCenter;
        extent = npcActiveRouteContext->motionStep * 30.0f + 3.0f;
        combinedRadius = static_cast<NpcRouteLiveEntry *>(other)->streamRadius + 0.05f;
        break;
    }
    if (distance > extent)
        return 0;
    combinedRadius += ownRadius + 0.05f;
    NpcRouteLiveEntry *owner;
    center = (owner = npcRouteOwner)->streamCenter;
    routeFrame = position;
    TransformNpcPointToYawFrame(&center, npcActiveRouteContext->tangentYaw, &routeFrame);
    if (routeFrame.z > 0.0f)
        return 0;
    headingFrame = position;
    TransformNpcPointToYawFrame(&center, heading, &headingFrame);
    float lateralBase = npcActiveRouteContext->laneOffset - npcActiveRouteContext->laneStep;
    float obstacleLane = lateralBase + routeFrame.x;
    if (previousResult < 3 && !(npcActiveRouteContext->avoidanceFlags & 8U)) {
        const std::int8_t oldSide = npcActiveRouteContext->avoidanceAuxiliary;
        if (headingFrame.x < 0.0f)
            npcActiveRouteContext->avoidanceAuxiliary = 1;
        else
            npcActiveRouteContext->avoidanceAuxiliary = -1;
        if (previousResult == 2 && oldSide == npcActiveRouteContext->avoidanceAuxiliary)
            npcActiveRouteContext->avoidanceAuxiliary = -oldSide;
    }
    npcActiveRouteContext->retained88 = static_cast<std::uint8_t>(kind);
    float maximumLane;
    if (npcActiveRouteContext->avoidanceAuxiliary == -1) {
        routeFrame.x *= -1.0f;
        lateralBase *= -1.0f;
        obstacleLane *= -1.0f;
        maximumLane = -npcActiveRouteContext->laneLow;
        kind = 5;
    } else {
        maximumLane = npcActiveRouteContext->laneHigh;
        kind = 4;
    }
    float targetLane = obstacleLane + combinedRadius + 0.15f;
    if (routeFrame.x < -combinedRadius)
        npcActiveRouteContext->avoidanceResult = 0;
    else if (lateralBase < obstacleLane - combinedRadius || targetLane > maximumLane || (routeFrame.x > 0.0f && combinedRadius > std::abs(routeFrame.z))) {
        if (routeFrame.x < 0.0f && combinedRadius > std::abs(routeFrame.z))
            npcActiveRouteContext->avoidanceResult = 2;
        else {
            npcActiveRouteContext->avoidanceAuxiliary = -npcActiveRouteContext->avoidanceAuxiliary;
            npcActiveRouteContext->avoidanceResult = 4;
        }
    } else if (-routeFrame.z < combinedRadius + 0.6f) {
        const float difference = targetLane - lateralBase;
        if (difference < 0.4f) {
            if (difference < 0.2f)
                npcActiveRouteContext->avoidanceResult = 6;
            else if (std::abs(static_cast<float>(static_cast<std::int32_t>(npcActiveRouteContext->retainedfc))) < 2730.0f)
                npcActiveRouteContext->avoidanceResult = 8;
            else
                npcActiveRouteContext->avoidanceResult = 7;
        } else
            npcActiveRouteContext->avoidanceResult = 7;
    } else if (-routeFrame.z < combinedRadius + 1.6f)
        npcActiveRouteContext->avoidanceResult = 6;
    else
        npcActiveRouteContext->avoidanceResult = 3;
    npcActiveRouteContext->other = static_cast<NpcRouteLiveEntry *>(other);
    if (kind == 5) {
        lateralBase *= -1.0f;
        targetLane *= -1.0f;
    }
    if (static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult) == 3 && -routeFrame.z > combinedRadius + 3.0f) {
        if (lateralBase > 0.0f)
            npcActiveRouteContext->avoidanceAuxiliary = -1;
        else
            npcActiveRouteContext->avoidanceAuxiliary = 1;
    }
    if (npcActiveRouteContext->avoidanceResult == 2 && distance > combinedRadius) {
        const std::int16_t savedParameter = npcActiveRouteContext->routeParameter;
        npcActiveRouteContext->routeParameter = static_cast<std::int16_t>(AdvanceNpcRouteByDistance(shen_name(), npcActiveRouteContext, distance + distance));
        SampleNpcRouteWindow(npcActiveRouteContext, &future, 0);
        npcActiveRouteContext->routeParameter = savedParameter;
        future.x -= npcRouteOrigin.x;
        future.z -= npcRouteOrigin.z;
        heading = static_cast<std::int16_t>(obstacleYaw - heading);
        TransformNpcPointToYawFrame(&center, heading, &future);
        if (future.z > headingFrame.z)
            npcActiveRouteContext->avoidanceResult = 0;
    }
    if (npcActiveRouteContext->avoidanceResult == 2 && distance > combinedRadius) {
        const std::int16_t savedParameter = npcActiveRouteContext->routeParameter;
        npcActiveRouteContext->routeParameter = static_cast<std::int16_t>(AdvanceNpcRouteByDistance(shen_name(), npcActiveRouteContext, distance + distance));
        SampleNpcRouteWindow(npcActiveRouteContext, &future, 0);
        npcActiveRouteContext->routeParameter = savedParameter;
        future.x -= npcRouteOrigin.x;
        future.z -= npcRouteOrigin.z;
        heading = static_cast<std::int16_t>(obstacleYaw - heading);
        TransformNpcPointToYawFrame(&center, heading, &future);
        if (future.z > headingFrame.z)
            npcActiveRouteContext->avoidanceResult = 0;
    }
    angle = npcActiveRouteContext->tangentYaw;
    if (npcActiveRouteContext->avoidancePhase) {
        switch (static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult)) {
        case 3:
        case 6:
        case 7:
        case 8:
            if (!(npcActiveRouteContext->flags & 0x1000U)) {
                npcActiveRouteContext->routeFlags |= 0x800U;
                return static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult);
            }
            npcActiveRouteContext->avoidanceResult = 8;
            angle = npcActiveRouteContext->stairYaw;
            break;
        }
    }
    switch (static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult)) {
    case 0:
        npcActiveRouteContext->other = 0;
        break;
    case 2:
        npcActiveRouteContext->other = 0;
        if ((npcActiveRouteContext->avoidanceFlags & 1U) == 0) {
            if (npcActiveRouteContext->flags & 0x400U)
                npcActiveRouteContext->routeFlags |= 0x800U;
            else
                mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, npcActiveRouteContext->savedEuler.y, 13, 0);
            npcActiveRouteContext->avoidanceFlags |= 1U;
        }
        break;
    case 3:
    case 6:
        if (npcActiveRouteContext->avoidanceFlags & 1U) {
            npcActiveRouteContext->avoidanceFlags &= ~1U;
            SetNpcRouteMotionState(npcActiveRouteContext, shen_name(), 3);
        }
        npcActiveRouteContext->laneStep = targetLane - npcActiveRouteContext->lateralPosition;
        npcActiveRouteContext->laneOffset = targetLane;
        OffsetNpcRoutePosition(&npcActiveRouteContext->position, &npcActiveRouteContext->moveTarget, npcActiveRouteContext->tangentYaw, targetLane);
        break;
    case 7:
    case 8:
        if (npcActiveRouteContext->avoidanceFlags & 1U) {
            npcActiveRouteContext->avoidanceFlags &= ~1U;
            SetNpcRouteMotionState(npcActiveRouteContext, shen_name(), 3);
        }
        if (npcActiveRouteContext->movementProgress > 0.0f)
            OffsetNpcRoutePosition(&npcActiveRouteContext->position, &npcActiveRouteContext->moveTarget, angle, targetLane);
        else {
            if (!(npcActiveRouteContext->flags & 0x80000U)) {
                npcActiveRouteContext->flags |= 0x80000U;
                npcActiveRouteContext->savedSplinePosition = npcActiveRouteContext->position;
                npcActiveRouteContext->savedMoveTarget = npcActiveRouteContext->moveTarget;
                npcActiveRouteContext->savedLaneOffset = npcActiveRouteContext->laneOffset;
            }
            OffsetNpcRoutePosition(&npcActiveRouteContext->localPosition, &npcActiveRouteContext->position, angle, -lateralBase);
            OffsetNpcRoutePosition(&npcActiveRouteContext->localPosition, &npcActiveRouteContext->moveTarget, angle, targetLane - lateralBase);
        }
        npcActiveRouteContext->laneOffset = targetLane;
        position = npcActiveRouteContext->moveTarget;
        position.x -= npcRouteOrigin.x;
        position.z -= npcRouteOrigin.z;
        mtwkSetSpeed(npcActiveRouteContext->motionWork, npcActiveRouteContext->motionScale, LowerBody);
        if (static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult) == 7) {
            npcActiveRouteContext->avoidanceFlags |= 8U;
            mtwkCopyTransition(npcRouteMotionWork, &position, npcActiveRouteContext->tangentYaw, 5, 3);
        } else
            mtwkCopyTransition(npcRouteMotionWork, &position, 0, 7, 1);
        if (npcActiveRouteContext->flags & 0x10U)
            mtwkCopyTransition(npcActiveRouteContext->motionWork, 0, 0, 8, 0);
        break;
    }
    return static_cast<std::int8_t>(npcActiveRouteContext->avoidanceResult);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t SampleNpcRouteWindow(NpcRouteStateContextPrefix *context, Vector3 *position, std::int32_t advance) {
    std::int32_t valid = 1;
    const std::int16_t *route = context->routePath;
    float amount = static_cast<float>(context->routeParameter) * 0.015625f;
    std::int32_t window = static_cast<std::int32_t>(amount);
    const NpcRouteSpline *const table = context->stateTable;
    if (window > context->pointCount - 4) {
        window = context->pointCount - 4;
        amount = 0.99998999f;
        valid = 0;
    }
    if (advance) {
        if (context->sampledWindow == window) {
            SampleNpcSplineSegment(context->spline, position, amount);
            return valid;
        }
        if (context->routeParameter < context->sampledParameter)
            advance = 0;
        else
            route = context->routePath + context->sampledRouteIndex;
    }
    if (!advance) {
        route = context->routePath;
        context->sampledRouteIndex = advance;
        context->sampledPointBase = advance;
    }
    std::int32_t base = context->sampledPointBase;
    std::int32_t length = table[*route & 0x7fff].pointCount - 2;
    context->sampledWindow = static_cast<std::int16_t>(window);
    context->sampledParameter = context->routeParameter;
    std::int16_t nodes[4], points[4];
    std::int32_t count = 0;
    do {
        while (window + count > base + length) {
            ++route;
            base += length;
            if (*route == -1)
                break;
            if (count < 2) {
                ++context->sampledRouteIndex;
                context->sampledPointBase = static_cast<std::int16_t>(base);
            }
            length = table[*route & 0x7fff].pointCount - 2;
        }
        if (*route == -1)
            break;
        nodes[count] = *route;
        points[count] = static_cast<std::int16_t>(window - base + count);
        ++count;
    } while (count < 4);
    if (count == 2) {
        *position = *GetNpcRouteEndPoint(table, nodes[1]);
        return 0;
    }
    if (count == 3) {
        nodes[3] = nodes[2];
        points[3] = points[2] + 1;
    }
    Vector3 *const spline = context->spline;
    for (std::int32_t index = 0; index < 4; ++index) {
        const std::int32_t node = nodes[index] & 0x7fff;
        if (~nodes[index] & 0x8000)
            spline[index] = table[node].points[points[index]];
        else
            spline[index] = table[node].points[table[node].pointCount - points[index] - 1];
    }
    SampleNpcSplineSegment(context->spline, position, amount);
    return valid;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool RequestNpcRouteMotion(Character *character, NpcRouteStateContextPrefix *context, std::int32_t explicitRequest) {
    bool result = false;
    if (!character || !(context->flags & 3U))
        return false;
    mtwkSetSpeed(context->motionWork, context->motionScale, LowerBody);
    if (context->flags & 0x1000U)
        return true;
    switch (static_cast<std::int8_t>(context->phase)) {
    case 3:
        mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 13, 0);
        mtwkSetTimeRelated(context->motionWork, 1, 5, 0);
        break;
    case 5:
        switch (static_cast<std::int8_t>(context->moveState)) {
        case 0:
            if (!explicitRequest)
                break;
        case 1: {
            mtwkCopyTransition(context->motionWork, &context->sceneTarget, context->targetYaw, 0, 0);
            mtwkCopyTransition(context->motionWork, 0, 0, 2, 0);
            mtwkSetTimeRelated(context->motionWork, 1, 5, static_cast<std::uint16_t>(context->doorMotion));
            mtwkSetRegionEvalTime(context->motionWork, 14.0f, LowerBody);
            std::uint32_t flags = ccmGetFlags(character);
            flags &= ~4U;
            flags |= 0x800U;
            AssignCharacterControlFlags(character, flags);
            break;
        }
        case 3:
            mtwkCopyTransition(context->motionWork, 0, context->targetYaw, 13, 0);
            mtwkSetTimeRelated(context->motionWork, 1, 5, 0);
            break;
        }
        break;
    case 7:
        if (!explicitRequest)
            result = true;
        if (context->avoidanceFlags & 1U) {
            context->other = 0;
            mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 13, 0);
            break;
        }
        if (TickNpcRouteStair(character, context))
            break;
        mtwkCopyTransition(context->motionWork, 0, context->savedEuler.y, 5, 2);
        if (context->flags & 0x10U)
            mtwkCopyTransition(context->motionWork, 0, 0, 8, 0);
        break;
    case 6:
        switch (static_cast<std::int8_t>(context->moveState)) {
        case 0:
            if (!explicitRequest)
                break;
        case 1:
            mtwkCopyTransition(context->motionWork, &context->sceneTarget, context->targetYaw, 5, 0);
            mtwkCopyTransition(context->motionWork, 0, 0, 2, 0);
            if (context->flags & 0x10U)
                mtwkCopyTransition(context->motionWork, 0, 0, 8, 0);
            break;
        }
        break;
    case 8:
        switch (static_cast<std::int8_t>(context->moveState)) {
        case 0:
            if (!explicitRequest)
                break;
        case 1: {
            Vector3 target = context->sceneTarget;
            if (context->otherName) {
                context->doorMotion = GetNpcDoorAnimation(static_cast<std::int32_t>(context->otherName), character, &context->scenePosition);
                if (context->otherName != g_doorIngress.selectedDoor) {
                    context->doorMotion = 0;
                    context->otherName = 0;
                } else if (context->doorMotion) {
                    Vector3 track;
                    SampleNativeMotionRoot(static_cast<std::uint16_t>(context->doorMotion), charGetMotionRigType(character), 15.0f, &track);
                    matrix_stack::PushUnitMatrix();
                    Matrix4x4::SetViewMatrixFromEuler(0, context->targetYaw, 0);
                    Matrix4x4::TransformPoint(track, track);
                    PopCurrentMatrix(1);
                    target.x += track.x;
                    target.z += track.z;
                }
            }
            mtwkCopyTransition(context->motionWork, &target, context->targetYaw, 6, 0);
            mtwkCopyTransition(context->motionWork, 0, 0, 2, 0);
            if (context->flags & 0x10U)
                mtwkCopyTransition(context->motionWork, 0, 0, 8, 0);
            if (context->otherName && context->doorMotion) {
                mtwkSetTimeRelated(context->motionWork, 1, 5, static_cast<std::uint16_t>(context->doorMotion));
                mtwkSetRegionEvalTime(context->motionWork, 14.0f, LowerBody);
            }
            break;
        }
        case 3:
            mtwkCopyTransition(context->motionWork, 0, context->targetYaw, 13, 0);
            mtwkSetTimeRelated(context->motionWork, 1, 5, 0);
            break;
        }
        break;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t FindNearestNpcParameter(NpcRouteStateContextPrefix *context, const Vector3 *position) {
    Vector3 point;
    const float positionX = position->x, positionZ = position->z;
    context->routeParameter = 0;
    SampleNpcRouteWindow(context, &point, 0);
    float best = positionX - point.x;
    const float initialZ = positionZ - point.z;
    best = best * best + initialZ * initialZ;
    std::int32_t nearest = context->routeParameter;
    context->routeParameter += 64;
    std::int32_t valid;
    do {
        valid = SampleNpcRouteWindow(context, &point, 1);
        float distance = positionX - point.x;
        const float z = positionZ - point.z;
        distance = distance * distance + z * z;
        if (distance < best) {
            nearest = context->routeParameter;
            best = distance;
        }
        context->routeParameter += 64;
    } while (valid == 1);
    context->routeParameter = static_cast<std::int16_t>(nearest - 64);
    if (context->routeParameter < 0)
        context->routeParameter = 0;
    const std::int32_t end = nearest + 64;
    SampleNpcRouteWindow(context, &point, 0);
    float distance = positionX - point.x;
    const float z = positionZ - point.z;
    distance = distance * distance + z * z;
    if (distance < best) {
        nearest = context->routeParameter;
        best = distance;
    }
    ++context->routeParameter;
    if (context->routeParameter < end) {
        do {
            valid = SampleNpcRouteWindow(context, &point, 1);
            distance = positionX - point.x;
            const float z = positionZ - point.z;
            distance = distance * distance + z * z;
            if (distance < best) {
                nearest = context->routeParameter;
                best = distance;
            }
            ++context->routeParameter;
        } while (context->routeParameter < end && valid == 1);
    }
    return nearest;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const Vector3 *GetNpcRouteSearchPoint(const NpcRouteSpline *routes, std::int16_t state) {
    if (~state & 0x8000)
        return routes[state & 0x7fff].points + routes[state & 0x7fff].pointCount - 2;
    return routes[state & 0x7fff].points + 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool SearchNpcRouteNodes(std::int16_t state, std::int16_t target) {
    std::int8_t orderedSlots[4], slots[4];
    std::int16_t ordered[4], candidates[4];
    float distances[4];
    std::int32_t current, count, limit;
    if (npcRouteSearchDepth == 71)
        return false;
    ++npcRouteSearchDepth;
    const std::int16_t index = state & 0x7fff, targetIndex = target & 0x7fff;
    const NpcRouteSpline &node = npcRouteSearchTable[index];
    if (index == targetIndex) {
        npcRouteSearchCount = npcRouteSearchDepth;
        npcRouteSearchPointCount = node.pointCount - 1;
        *npcRouteSearchCost = node.segmentLengths[0];
        npcRouteSearchPath[npcRouteSearchDepth] = -1;
        --npcRouteSearchDepth;
        npcRouteSearchPath[npcRouteSearchDepth] = state;
        return true;
    }
    npcRouteSearchVisited[index] = 1;
    limit = (~state & 0x8000) ? 4 : 8;
    count = 0;
    for (current = limit - 4; current < limit; ++current) {
        const std::int16_t next = node.startConnections[current];
        const std::int32_t nextIndex = next & 0x7fff;
        if (!(npcRouteSearchTable[nextIndex].flags & 0x300U) && npcRouteSearchVisited[nextIndex] != 1) {
            slots[count] = static_cast<std::int8_t>(current);
            candidates[count] = next;
            ++count;
        }
    }
    limit = count;
    for (current = 0; current < limit; ++current) {
        const std::int16_t next = candidates[current];
        const std::int32_t nextIndex = next & 0x7fff;
        if (npcRouteSearchVisited[nextIndex] != 1 && targetIndex == nextIndex) {
            if (SearchNpcRouteNodes(next, target)) {
                --npcRouteSearchDepth;
                npcRouteSearchPath[npcRouteSearchDepth] = state;
                npcRouteSearchConnections[npcRouteSearchDepth] = slots[current];
                npcRouteSearchPointCount += node.pointCount - 2;
                *npcRouteSearchCost += node.segmentLengths[0];
                return true;
            }
        }
    }
    for (current = 0; current < limit; ++current) {
        Vector3 position = *GetNpcRouteSearchPoint(npcRouteSearchTable, candidates[current]);
        position.x -= npcRouteTarget.x;
        position.z -= npcRouteTarget.z;
        distances[current] = position.x * position.x + position.z * position.z;
    }
    for (current = 0; current < limit; ++current) {
        std::int32_t rank = 0;
        for (std::int32_t other = 0; other < limit; ++other) {
            if (current != other) {
                if (distances[other] < distances[current])
                    ++rank;
                else if (distances[current] == distances[other] && other < current)
                    ++rank;
            }
        }
        orderedSlots[rank] = slots[current];
        ordered[rank] = candidates[current];
    }
    for (current = 0; current < limit; ++current) {
        if (npcRouteSearchVisited[ordered[current] & 0x7fff] != 1) {
            if (SearchNpcRouteNodes(ordered[current], target)) {
                --npcRouteSearchDepth;
                npcRouteSearchPath[npcRouteSearchDepth] = state;
                npcRouteSearchConnections[npcRouteSearchDepth] = orderedSlots[current];
                npcRouteSearchPointCount += node.pointCount - 2;
                *npcRouteSearchCost += node.segmentLengths[0];
                return true;
            }
        }
    }
    --npcRouteSearchDepth;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool FindNpcRoutePath(NpcRouteStateContextPrefix *context) {
    std::int16_t start = context->startNode, target = context->endNode;
    std::int16_t *path = context->routePath;
    std::int8_t *connections = context->routeConnections;
    float forwardCost, backwardCost;
    std::int32_t forwardPoints, backwardPoints, forwardCount, backwardCount, useBackward, size;
    if ((start & 0x7fff) == (target & 0x7fff)) {
        path[0] = context->startNode;
        path[1] = -1;
        context->routeCount = 1;
        context->pointCount = context->stateTable[context->startNode & 0x7fff].pointCount;
        return true;
    }
    forwardPoints = backwardPoints = 0;
    forwardCost = backwardCost = 0.0f;
    npcRouteSearchTable = context->stateTable;
    npcRouteSearchPath = npcRouteForwardPath;
    npcRouteSearchConnections = npcRouteForwardConnections;
    npcRouteSearchCost = &forwardCost;
    npcRouteSearchDepth = npcRouteSearchPointCount = 0;
    std::memset(npcRouteSearchVisited, 0, npcScheduleRouteCount);
    if (SearchNpcRouteNodes(start, target)) {
        forwardCount = npcRouteSearchCount;
        forwardPoints = npcRouteSearchPointCount + 1;
    } else
        forwardCount = 0;
    if (~start & 0x8000)
        start |= 0x8000;
    else
        start &= 0x7fff;
    npcRouteSearchPath = npcRouteBackwardPath;
    npcRouteSearchConnections = npcRouteBackwardConnections;
    npcRouteSearchCost = &backwardCost;
    npcRouteSearchDepth = npcRouteSearchPointCount = 0;
    std::memset(npcRouteSearchVisited, 0, npcScheduleRouteCount);
    if (SearchNpcRouteNodes(start, target)) {
        backwardCount = npcRouteSearchCount;
        backwardPoints = npcRouteSearchPointCount + 1;
    } else
        backwardCount = 0;
    if (!forwardCount && !backwardCount)
        return false;
    if (!forwardCount)
        useBackward = 1;
    else if (!backwardCount)
        useBackward = 0;
    else if (forwardCost > backwardCost)
        useBackward = 1;
    else
        useBackward = 0;
    if (!useBackward) {
        size = forwardCount + 1;
        std::memcpy(path, npcRouteForwardPath, size * 2);
        std::memcpy(connections, npcRouteForwardConnections, size);
        context->routeCount = forwardCount;
        context->pointCount = forwardPoints;
    } else {
        size = backwardCount + 1;
        std::memcpy(path, npcRouteBackwardPath, size * 2);
        std::memcpy(connections, npcRouteBackwardConnections, size);
        context->routeCount = backwardCount;
        context->pointCount = backwardPoints;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool MoveForNpcInData(NpcRouteLiveEntry *entry, shen_name area, std::int32_t apply) {
    if (!entry->scheduleProgress)
        return false;
    std::int8_t live;
    if (static_cast<std::int8_t>(entry->liveState) == 2)
        live = entry->previousLiveState;
    else
        live = entry->liveState;
    switch (live) {
    case 4:
    case 8:
    case 9:
    case 11:
    case 12:
        return false;
    case 1:
        if (static_cast<std::int8_t>(entry->stateContext.phase) != 7)
            return false;
        break;
    }
    if (entry->flags & 0x800U)
        return false;
    if (!npcUsesMainSchedule)
        return false;
    if (!(entry->flags & 0x3000000U)) {
        if (SelectNpcRouteMovementSlot())
            entry->flags |= 0x1000000U;
        else
            entry->flags |= 0x2000000U;
    }
    if (entry->flags & 0x2000000U)
        return false;
    if (!apply && !area)
        return true;
    const std::int32_t point = FindNpcAreaPointByName(area, 0);
    if (point == -1)
        return false;
    NpcRouteStateContextPrefix *context;
    if (apply)
        context = &entry->stateContext;
    else {
        context = &npcPreviewRouteContext;
        npcPreviewRouteContext = entry->stateContext;
    }
    context->pendingAreaPoint = static_cast<std::int16_t>(point);
    context->scriptOwner = entry->name;
    std::int32_t newRoute;
    if ((context->flags & 1U) == 0) {
        context->currentAreaPoint = context->owner->currentAreaPoint;
        context->owner->pendingAreaPoint = context->currentAreaPoint;
        context->arrivalYaw = context->owner->rotation.y;
        newRoute = 1;
    } else if (context->flags & 0x4000U)
        newRoute = 1;
    else
        newRoute = 0;
    context->flags = (context->flags & 0x20002U) | 1U;
    context->routeFlags |= 7U;
    npcRouteCharacter = entry->node;
    npcActiveRouteContext = context;
    if (npcRouteCharacter)
        npcRouteMotionWork = context->motionWork;
    npcRouteOwner = entry;
    InitializeNpcRouteMotion(npcRouteCharacter, npcActiveRouteContext, 1.0f);
    npcActiveRouteContext->phase = 0;
    npcActiveRouteContext->routePending = 1;
    do {
        switch (static_cast<std::int8_t>(npcActiveRouteContext->phase)) {
        case 0:
            EnterNpcRoute();
            break;
        case 1:
            BuildNpcRoute();
            break;
        case 2:
            BindNpcRouteWindow();
            break;
        case 3:
            npcActiveRouteContext->phase = 1;
            break;
        case 12:
        default:
            if ((npcActiveRouteContext->flags & 2U) && apply && static_cast<std::int8_t>(entry->liveState) == 1) {
                InitializeNpcAreaMovement(
                    npcActiveRouteContext->scriptOwner, npcActiveRouteContext, npcRouteOwner->currentAreaPoint, npcRouteOwner->pendingAreaPoint);
                SetNpcLocomotionScale(npcActiveRouteContext, shen_name(), npcRouteOwner->motionSpeed);
            }
            if (apply)
                entry->flags = (entry->flags & ~0x1000000U) | 0x2000000U;
            return false;
        }
    } while (!(npcActiveRouteContext->flags & 2U));
    if (newRoute)
        npcActiveRouteContext->flags |= 0x4000U;
    npcActiveRouteContext->flags |= 0x100U;
    npcActiveRouteContext->routeFlags |= 0x100000U;
    RefreshNpcScriptRouteMotion(npcRouteCharacter, npcActiveRouteContext);
    if (apply) {
        entry->retained309 = entry->scheduleProgress;
        if (static_cast<std::int8_t>(entry->liveState) != 2) {
            entry->retained30b = entry->liveState;
            entry->liveState = 1;
        } else {
            entry->retained30b = entry->previousLiveState;
            entry->previousLiveState = 1;
        }
        entry->scheduleProgress = 3;
    } else {
        npcPreviewRouteName = entry->name;
        npcPreviewRouteArea = area;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

bool QueryNpcAreaSceneCheck(shen_name name, shen_name area) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return false;
    return MoveForNpcInData(entry, area, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t RequestNpcSceneRoute(shen_name name, shen_name area) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    std::int32_t result;
    if (name == npcPreviewRouteName && area == npcPreviewRouteArea) {
        entry->stateContext = npcPreviewRouteContext;
        entry->stateContext.motionWork = entry->motionWork;
        if (entry->node) {
            Vector3 movement;
            entry->stateContext.appliedMovement = ccmGetAppliedMovement(entry->node, &movement);
        }
        entry->retained309 = entry->scheduleProgress;
        if (static_cast<std::int8_t>(entry->liveState) != 2) {
            entry->retained30b = entry->liveState;
            entry->liveState = 1;
        } else {
            entry->retained30b = entry->previousLiveState;
            entry->previousLiveState = 1;
        }
        entry->scheduleProgress = 3;
        result = 1;
    } else
        result = MoveForNpcInData(entry, area, 1);
    if (result == 1) {
        if (npcSceneRouteOwner && npcSceneRouteOwner != entry) {
            const std::uint32_t flags = (&npcSceneRouteOwner->stateContext)->flags;
            if (flags & 0x100U) {
                const std::uint32_t paused = flags & 0x4000U;
                InitializeNpcAreaMovement(npcSceneRouteOwner->stateContext.scriptOwner, &npcSceneRouteOwner->stateContext,
                    npcSceneRouteOwner->stateContext.currentAreaPoint, npcSceneRouteOwner->pendingAreaPoint);
                SetNpcLocomotionScale(&npcSceneRouteOwner->stateContext, shen_name(), npcSceneRouteOwner->motionSpeed);
                (&npcSceneRouteOwner->stateContext)->flags |= 0x200U;
                if (paused) {
                    (&npcSceneRouteOwner->stateContext)->flags |= 0x4000U;
                    (&npcSceneRouteOwner->stateContext)->routeFlags |= 0x20000U;
                }
                if (static_cast<std::int8_t>(npcSceneRouteOwner->retained312[0]) < 0)
                    npcSceneRouteOwner->pendingRouteQueryState = 12;
                else
                    npcSceneRouteOwner->routeQueryState = 12;
            } else if ((&npcSceneRouteOwner->stateContext)->flags & 0x8000U) {
                npcSceneRouteOwner->stateContext.pauseStartTime = 0;
            }
        }
        npcSceneRouteOwner = entry;
        SetNpcScheduleSeedName(entry->name, shen_name(), 0);
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t RequestScenarioNpcSceneRoute(shen_name name, shen_name area) {
    return RequestNpcSceneRoute(name, area);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateLinkedNpcRoute() {
    NpcRouteStateContextPrefix *other;
    Vector3 delta;
    std::int32_t request = 0;
    float distance = 0.0f;
    if (static_cast<std::int8_t>(npcActiveRouteContext->phase) != 7)
        return;
    other = &npcActiveRouteContext->followingEntry->stateContext;
    if (other->phase == 6)
        request = 1;
    else if (static_cast<std::int8_t>(other->phase) == 7) {
        delta.x = other->localPosition.x - npcActiveRouteContext->localPosition.x;
        delta.z = other->localPosition.z - npcActiveRouteContext->localPosition.z;
        delta.y = 0.0f;
        distance = Vector3::CalcMagnitude(delta);
        if (distance > npcActiveRouteContext->followingDistance) {
            if (other->routeParameter <= npcActiveRouteContext->routeParameter)
                request = 1;
            else
                request = 2;
        }
    }
    switch (request) {
    case 1:
        npcActiveRouteContext->routeFlags |= 0x800U;
        npcActiveRouteContext->routeFlags |= 0x2000000U;
        if ((other->routeFlags & 0x2000000U) && !(npcActiveRouteContext->followingEntry->flags & 0x10U)) {
            if (other->flags & 0x1000U)
                SetNpcRouteMotionState(other, 0, 3);
            other->routeFlags &= ~0x2000000U;
        }
        break;
    case 2:
        other->routeFlags |= 0x800U;
        other->routeFlags |= 0x2000000U;
        if ((npcActiveRouteContext->routeFlags & 0x2000000U) && !(npcActiveRouteContext->owner->flags & 0x10U)) {
            if (npcActiveRouteContext->flags & 0x1000U)
                SetNpcRouteMotionState(npcActiveRouteContext, 0, 3);
            npcActiveRouteContext->routeFlags &= ~0x2000000U;
        }
        break;
    default:
        if (npcActiveRouteContext->followingDistance * 0.5f > distance) {
            if (npcActiveRouteContext->routeFlags & 0x2000000U) {
                if (npcActiveRouteContext->flags & 0x1000U)
                    SetNpcRouteMotionState(npcActiveRouteContext, 0, 3);
                npcActiveRouteContext->routeFlags &= ~0x2000000U;
            }
            if (other->routeFlags & 0x2000000U) {
                if (other->flags & 0x1000U)
                    SetNpcRouteMotionState(other, 0, 3);
                other->routeFlags &= ~0x2000000U;
            }
        }
        break;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteMotionMode(shen_name name) {
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return 0;
    if ((&entry->stateContext)->flags & 0x1000U)
        return 2;
    if ((&entry->stateContext)->flags & 0x10U)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

NpcRouteSpline *GetNpcScriptRouteTable() {
    return npcScriptRouteTable;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t HasNpcRouteNodeFlags(const NpcRouteStateContextPrefix *context, std::uint32_t mask) {
    if ((context->flags & 3U) != 3U)
        return 0;
    if (npcScriptRouteTable[context->activeNode & 0x7fff].flags & mask)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t FindNearestNpcRouteParameter(shen_name name, const Vector3 *position) {
    NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry)
        return -1;
    NpcRouteStateContextPrefix *const context = &entry->stateContext;
    if (!(context->flags & 3U))
        return -1;
    Vector3 point = *position;
    point.x += npcRouteOrigin.x;
    point.z += npcRouteOrigin.z;
    return FindNearestNpcParameter(context, &point);
}

#else

std::int32_t FindNearestNpcRouteParameter(shen_name character, const Vector3 *position) {
    NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    if (!context || (context->flags & 3U) == 0)
        return -1;
    return FindNearestNpcParameter(*context, NpcAreaPosition(*position));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SampleNpcRoute(shen_name name, std::int32_t parameter, Vector3 *position, std::int32_t *yaw) {
    Vector3 first, next;
    NpcRouteStateContextPrefix copy;
    NpcRouteStateContextPrefix *context;
    std::int32_t valid;
    context = FindNpcRouteStateContext(name);
    if (!context)
        return 0;
    copy = *context;
    context = &copy;
    if (parameter < context->lowerParameter)
        parameter = context->lowerParameter;
    if (parameter > context->upperParameter)
        parameter = context->upperParameter;
    context->routeParameter = parameter;
    SampleNpcRouteWindow(context, &first, 0);
    context->routeParameter += context->stride;
    valid = SampleNpcRouteWindow(context, &next, 1);
    context->position = next;
    context->routeParameter = parameter;
    next.x -= first.x;
    next.y -= first.y;
    next.z -= first.z;
    if (!(std::abs(next.x) < 0.001f && std::abs(next.z) < 0.001f))
        *yaw = static_cast<std::int16_t>(InverseFixedAtan2(next.x, next.z));
    else
        *yaw = 0;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(first.x, first.y, first.z);
    Matrix4x4::SetViewMatrixFromEuler(0, *yaw, 0);
    matrix_stack::GetTranslation(position);
    PopCurrentMatrix(1);
    position->x -= npcRouteOrigin.x;
    position->z -= npcRouteOrigin.z;
    return valid;
}

#else

std::int32_t SampleNpcRoute(shen_name character, std::int32_t parameter, Vector3 *position, std::int32_t *yaw) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source)
        return 0;
    NpcRouteStateContext context = *source;
    if (parameter < context.lowParameter)
        parameter = context.lowParameter;
    if (parameter > context.highParameter)
        parameter = context.highParameter;
    context.parameter = static_cast<std::uint16_t>(parameter);
    Vector3 current;
    Vector3 next;
    SampleNpcRouteWindow(context, &current, false);
    context.parameter = static_cast<std::uint16_t>(context.parameter + context.stride);
    const std::int32_t result = SampleNpcRouteWindow(context, &next, true);
    *yaw = NpcRouteYaw(next.x - current.x, next.z - current.z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(current.x, current.y, current.z);
    Matrix4x4::SetViewMatrixFromEuler(*yaw, 0, 0);
    *position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    position->x -= npcSchedule.areaOrigin.x;
    position->z -= npcSchedule.areaOrigin.z;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteLowParameter(shen_name name) {
    const NpcRouteStateContextPrefix *const context = FindNpcRouteStateContext(name);
    return context ? context->lowerParameter : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteHighParameter(shen_name name) {
    const NpcRouteStateContextPrefix *const context = FindNpcRouteStateContext(name);
    return context ? context->upperParameter : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteCurrentParameter(shen_name name) {
    const NpcRouteStateContextPrefix *const context = FindNpcRouteStateContext(name);
    return context ? context->routeParameter : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteMaximumParameter(shen_name name) {
    const NpcRouteStateContextPrefix *const context = FindNpcRouteStateContext(name);
    return context ? (context->pointCount - 3) * 64 : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

float EstimateNpcRouteParameter(shen_name name, NpcRouteStateContextPrefix *context) {
    if (!context && !(context = FindNpcRouteStateContext(name)))
        return 0.0f;
    Vector3 offset = context->position;
    TransformNpcPointToYawFrame(&context->localPosition, context->tangentYaw, &offset);
    const std::int16_t parameter = context->routeParameter;
    context->routeParameter = parameter - static_cast<std::int16_t>(context->stride);
    Vector3 sample;
    SampleNpcRouteWindow(context, &sample, 0);
    const float distance = CalculatePlanarDistanceBetweenPoints(&sample, &context->position);
    context->routeParameter = parameter;
    SampleNpcRouteWindow(context, &sample, 0);
    float estimate;
    if (std::fabs(distance) < FLT_EPSILON)
        estimate = context->routeParameter;
    else
        estimate = context->routeParameter + offset.z * static_cast<std::int16_t>(context->stride) / distance;
    return estimate;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetNpcRouteStride(shen_name name) {
    const NpcRouteStateContextPrefix *const context = FindNpcRouteStateContext(name);
    return context ? static_cast<std::int16_t>(context->stride) : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void RebuildNpcAreaPointGround(NpcAreaPoint *points) {
    std::uint32_t floorFlags;
    if (npcScheduleFloor != -1000) {
        floorFlags = static_cast<std::uint32_t>(GetQronNextStream());
        floorFlags <<= 8;
    } else {
        floorFlags = 0;
    }
    for (NpcAreaPoint *point = points; point->area; ++point) {
        Vector3 position = point->position;
        position.x -= npcRouteOrigin.x;
        position.z -= npcRouteOrigin.z;
        FindNpcIntersectionInScene(position, floorFlags | (npcRouteNodes[point->routeIndex].flags & 0xffU));
        point->position.y = position.y;
    }
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#undef SHENMUE_NPC_EPSILON

extern "C" void __attribute__((renesas)) shenmue_s2_event_SetNpcMotionScale(shenmue::s2::NpcRouteStateContextPrefix *context, std::uint32_t name, float scale) {
    shenmue::s2::SetNpcScriptRouteSpeed(context, shenmue::shen_name(name), scale);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_StartNpcScriptAreaRoute(
    std::uint32_t name, std::uint32_t oldPoint, std::uint32_t newPoint, std::int32_t yaw) {
    return shenmue::s2::StartNpcScriptAreaRoute(shenmue::shen_name(name), shenmue::shen_name(oldPoint), shenmue::shen_name(newPoint), yaw);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_InitializeNpcMovement(
    std::uint32_t name, shenmue::s2::NpcRouteSpline *routes, const shenmue::Vector3 *origin, std::int32_t yaw) {
    return shenmue::s2::InitializeNpcMovement(shenmue::shen_name(name), routes, origin, yaw);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_SetNpcFlag(
    shenmue::s2::NpcRouteStateContextPrefix *context, std::uint32_t name, std::int32_t operation, std::uint32_t flags) {
    shenmue::s2::SetNpcScriptRouteFlag(context, shenmue::shen_name(name), operation, flags);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_npcSampleRouteNodeWindow(
    std::uint32_t name, std::int32_t parameter, const shenmue::Vector3 *start, shenmue::Vector3 *position, std::int32_t *yaw, std::int32_t step) {
    return shenmue::s2::SampleNpcRouteNodeWindow(shenmue::shen_name(name), parameter, start, position, yaw, step);
}
#else
#endif
