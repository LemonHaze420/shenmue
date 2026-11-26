#include "s2/camera_conversation_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstring>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/frame_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/modifier_vertices_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <math.h>
#include <float.h>
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/debug_screen_dc.hpp"
#endif
#include "s2/camera_search_dc.hpp"
#include "shenmue.h"
#include "s2/camera_state_query_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include <cmath>
#include "s2/character_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hlib.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scenario_dc.hpp"
#include "common/matrix_stack.hpp"
#include <cstdio>
#include "s2/model_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/window.hpp"
#include "s2/fight.hpp"
#include "s2/math.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/camera_roll_dc.hpp"
#include "common/native_sqrt.hpp"

namespace shenmue {
namespace s2 {

const CameraContextSetting cameraContextSettings[] = {{0.66f, 0x0800, 0}, {0.768f, 0x0634, 0}, {0.876f, 0x0468, 0}, {0.984f, 0x029c, 0}, {1.092f, 0x00d0, 0},
    {1.2f, -0x0100, 0}, {1.388f, -0x0259, 0}, {1.576f, -0x03b2, 0}, {1.764f, -0x050b, 0}, {1.952f, -0x0664, 0}, {2.14f, -0x07c0, 0}, {1.67f, -0x0800, 0}};

namespace {

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);

} // namespace

namespace {

typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

} // namespace

namespace {

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);

} // namespace

namespace {

typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

} // namespace

void ResumeConversationCamera();

void ResetConversationCameraState(ConversationCameraState *state);

void TickConversationActivation(ConversationCameraState *state);

void AdvanceConversationShotClock(ConversationCameraState *state);

void BeginConversationShot(ConversationCameraState *state);

void RefreshConversationSceneFlags(ConversationCameraState *state);

void UpdateConversationMemberAlpha(ConversationCameraState *state);
void MeasureConversationOffset(const Vector3 &point, ConversationOffset &offset);

static void AssignConversationShotTypes(ConversationCameraState *state);

static void ResetConversationShotTypeForFlag10(ConversationCameraState *state);

static void RecordConversationShot(ConversationCameraState *state);

static float GetConversationProgress(ConversationCameraState *state);

void EvaluateConversationShot(ConversationCameraState *state);
std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to);

void UpdateConversationActorPose(ConversationCameraState *state);

static void ReadConversationActorPose(ConversationCameraState *state);

static void SmoothConversationActorPose(ConversationCameraState *state);

static void BlendConversationEntryCamera(ConversationCameraState *state);

static void BlendConversationPreviousShot(ConversationCameraState *state);

static void ApplyConversationLookTurn(ConversationCameraState *state);

static void ApplyConversationSpecialModeOverride(ConversationCameraState *state);

static void BindConversationShotActors(ConversationCameraState *state);

static void AssignConversationShotAnchors(ConversationCameraState *state);

static void ReadConversationCameraPackets(ConversationCameraState *state);

static void CompressConversationCameraStream(ConversationCameraState *state);

static void CaptureConversationParticipantAlpha(ConversationCameraState *state);

static void RestoreConversationParticipantAlpha(ConversationCameraState *state);

static std::int32_t ConversationAbsAngle(std::int32_t angle);
static std::int16_t CameraTransitionAngle(std::int32_t base, std::int32_t target, float amount);

static void UpdateConversationAngleClass(ConversationCameraState *state);

void GenerateConversationCandidate(ConversationCameraState *state);

static void SelectConversationCameraSide(ConversationCameraState *state);

void UpdateConversationMirrorFlags(ConversationCameraState *state);

void ResetConversationCandidates(ConversationCameraState *state);

void FilterConversationCandidates(ConversationCameraState *state);

static void RestrictConversationCandidateAngles(ConversationCameraState *state);

static void ReadmitConversationCandidates(ConversationCameraState *state);

void SelectConversationCandidate(ConversationCameraState *state);

static void FilterConversationSceneCandidates(ConversationCameraState *state);

static void RetireRecentConversationCandidates(ConversationCameraState *state);

static void MarkConversationEdgeCandidates(ConversationCameraState *state);

static void CheckConversationCandidateCollisions(ConversationCameraState *state);

static bool AreConversationAngleCandidatesBlocked(ConversationCameraState *state);

static std::int32_t ConversationCollisionSegment(const Vector3 &start, const Vector3 &end, float radius);

static std::int32_t ConversationCollisionPoint(const Vector3 &point);

float ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen);

void resetIntersectionAndTest();

void UpdateEnvironmentFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFirstPersonFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFieldFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateQuadraticFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateMotionFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateMotionInterpolationFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *, FightActorRuntimePrefix *);

float CosineEaseValue(float base, float target, float amount);

std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);

float TangentEaseValue(float base, float target, float amount, float steepness);

struct FightCameraVelocity {
    Vector3 sum;
    float distance, divisor;
    std::int32_t closeEnemy;
};

void AccumulateFightCameraVelocity(FightCameraVelocity *velocity, FightRuntimePrefix *game, FightActorRuntimePrefix *self);

void vfUnknownCamRelated(FightRuntimePrefix *game);

void ResetFightCameraToParticipants(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

static void ResetFightCameraPattern();

void UpdateFightCameraPattern(FightRuntimePrefix *game);

std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);

static void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre);

static void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction, std::int32_t collide);

std::uint32_t camCtrlClearFlags(std::uint32_t mask);

static float FightCameraLength(const Vector3 &value);

void FightCameraBlendPoint(Vector3 &point, const Vector3 &other, float amount);

static void ScaleFightCameraVector(Vector3 &value, float length);

static std::int32_t FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin);

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);

void camCtrlApply(CameraRuntime &camera);

void camCtrlRecalcFov(CameraRuntime &camera);

void camCtrlSetup(CameraRuntime &camera);

void camCtrlHandleStates(CameraRuntime &camera);

void camCtrlSmoothMotion(CameraRuntime &camera);

void ApplyRotationMatrixFromTransform(CameraRuntime &camera);

void camCtrlCheckCollision(CameraRuntime &camera);

void camCtrlApplyJitterAdjustment(CameraRuntime &camera);

void camCtrlSetRotationXYZ(float x, float y, float duration);

std::uint32_t camCtrlGetFlags();

std::int32_t camCtrlSetState(std::int32_t state);

std::int32_t camCtrlGetState();

void camCtrlSetNextPosition(const Vector3 &position);

bool camCtrlIsTransitionActive();

void camCtrlSetTargetPosition(const Vector3 &position);

Vector3 *camCtrlGetNextCamPos(Vector3 *position);

Vector3 *camCtrlGetTargetPosition(Vector3 *position);

Vector3s *camCtrlGetRot(Vector3s *rotation);

std::int16_t camCtrlGetFov();

std::int32_t camCtrlSetPerspective(std::int16_t perspective);

std::int16_t camCtrlGetPerspective();

float camCtrlGetViewHeightScale();

Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output);

Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output);

void camCtrlSetFovTransitionTo(std::int32_t enabled);

std::int32_t camCtrlToggleSmoothing(std::int32_t enabled);

float camCtrlSetMotionScaleFactor(float factor);

std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);

void camCtrlSetInterpolationSpeed(float speed);

void camCtrlSetInterpolationFrameCount(std::int32_t frames);

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);

void ApplyRotationMatrixFromTransform(CameraRuntime &camera);

void camCtrlApply(CameraRuntime &camera);

void camCtrlRecalcFov(CameraRuntime &camera);

void camCtrlSetup(CameraRuntime &camera);

void camCtrlHandleStates(CameraRuntime &camera);

void camCtrlSmoothMotion(CameraRuntime &camera);

void camCtrlCheckCollision(CameraRuntime &camera);

void camCtrlApplyJitterAdjustment(CameraRuntime &camera);

std::uint32_t camCtrlGetFlags();

std::int32_t camCtrlSetState(std::int32_t state);

std::int32_t camCtrlGetState();

void camCtrlSetNextPosition(const Vector3 &position);

void camCtrlSetTargetPosition(const Vector3 &position);

Vector3 *camCtrlGetNextCamPos(Vector3 *position);

Vector3 *camCtrlGetTargetPosition(Vector3 *position);

Vector3s *camCtrlGetRot(Vector3s *rotation);

std::int16_t camCtrlGetFov();

Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output);

std::int32_t camCtrlSetPerspective(std::int16_t perspective);

std::int16_t camCtrlGetPerspective();

float camCtrlGetViewHeightScale();

Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output);

float camCtrlSetMotionScaleFactor(float factor);

void camCtrlSetFovTransitionTo(std::int32_t enabled);

std::int32_t camCtrlToggleSmoothing(std::int32_t enabled);

std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);

void camCtrlSetInterpolationSpeed(float speed);

void camCtrlSetInterpolationFrameCount(std::int32_t frames);

void camCtrlApply(CameraRuntime &camera);

void camCtrlApplyJitterAdjustment(CameraRuntime &camera);

void camCtrlSmoothMotion(CameraRuntime &camera);

void ApplyRotationMatrixFromTransform(CameraRuntime &camera);

void camCtrlCheckCollision(CameraRuntime &camera);

void camCtrlRecalcFov(CameraRuntime &camera);

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);

void camCtrlSetup(CameraRuntime &camera);

void camCtrlHandleStates(CameraRuntime &camera);

ConversationCameraState g_conversationCamera;

ConversationCameraState *g_conversationCameraState = &g_conversationCamera;

std::byte customConversationCameraParameters[sizeof(conversationMovingShotParameters[0])] = {};

ConversationCameraDescriptor conversationCameraDescriptors[65] = {
    {SHEN_FIELD(flags) 0x401U, SHEN_FIELD(parameters) conversationStaticShotParameters[0], SHEN_FIELD(id) 1},
    {SHEN_FIELD(flags) 0x401U, SHEN_FIELD(parameters) conversationStaticShotParameters[1], SHEN_FIELD(id) 2},
    {SHEN_FIELD(flags) 0x401U, SHEN_FIELD(parameters) conversationStaticShotParameters[2], SHEN_FIELD(id) 3},
    {SHEN_FIELD(flags) 0x801U, SHEN_FIELD(parameters) conversationStaticShotParameters[3], SHEN_FIELD(id) 4},
    {SHEN_FIELD(flags) 0x801U, SHEN_FIELD(parameters) conversationStaticShotParameters[4], SHEN_FIELD(id) 5},
    {SHEN_FIELD(flags) 0x801U, SHEN_FIELD(parameters) conversationStaticShotParameters[5], SHEN_FIELD(id) 6},
    {SHEN_FIELD(flags) 0x1001U, SHEN_FIELD(parameters) conversationStaticShotParameters[6], SHEN_FIELD(id) 7},
    {SHEN_FIELD(flags) 0x1001U, SHEN_FIELD(parameters) conversationStaticShotParameters[7], SHEN_FIELD(id) 8},
    {SHEN_FIELD(flags) 0x1001U, SHEN_FIELD(parameters) conversationStaticShotParameters[8], SHEN_FIELD(id) 9},
    {SHEN_FIELD(flags) 0x100401U, SHEN_FIELD(parameters) conversationStaticShotParameters[9], SHEN_FIELD(id) 10},
    {SHEN_FIELD(flags) 0x100401U, SHEN_FIELD(parameters) conversationStaticShotParameters[10], SHEN_FIELD(id) 11},
    {SHEN_FIELD(flags) 0x100801U, SHEN_FIELD(parameters) conversationStaticShotParameters[11], SHEN_FIELD(id) 12},
    {SHEN_FIELD(flags) 0xa402U, SHEN_FIELD(parameters) conversationMovingShotParameters[0], SHEN_FIELD(id) 101},
    {SHEN_FIELD(flags) 0xa402U, SHEN_FIELD(parameters) conversationMovingShotParameters[1], SHEN_FIELD(id) 102},
    {SHEN_FIELD(flags) 0xa402U, SHEN_FIELD(parameters) conversationMovingShotParameters[2], SHEN_FIELD(id) 103},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[3], SHEN_FIELD(id) 104},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[4], SHEN_FIELD(id) 105},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[5], SHEN_FIELD(id) 106},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[6], SHEN_FIELD(id) 107},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[7], SHEN_FIELD(id) 108},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[8], SHEN_FIELD(id) 109},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[9], SHEN_FIELD(id) 110},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[10], SHEN_FIELD(id) 111},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[11], SHEN_FIELD(id) 112},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[12], SHEN_FIELD(id) 113},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[13], SHEN_FIELD(id) 114},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[14], SHEN_FIELD(id) 115},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[15], SHEN_FIELD(id) 116},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[16], SHEN_FIELD(id) 117},
    {SHEN_FIELD(flags) 0xb002U, SHEN_FIELD(parameters) conversationMovingShotParameters[17], SHEN_FIELD(id) 118},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[18], SHEN_FIELD(id) 119},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[19], SHEN_FIELD(id) 120},
    {SHEN_FIELD(flags) 0xa802U, SHEN_FIELD(parameters) conversationMovingShotParameters[20], SHEN_FIELD(id) 121},
    {SHEN_FIELD(flags) 0x48802U, SHEN_FIELD(parameters) conversationMovingShotParameters[21], SHEN_FIELD(id) 122},
    {SHEN_FIELD(flags) 0x48802U, SHEN_FIELD(parameters) conversationMovingShotParameters[22], SHEN_FIELD(id) 123},
    {SHEN_FIELD(flags) 0x48802U, SHEN_FIELD(parameters) conversationMovingShotParameters[23], SHEN_FIELD(id) 124},
    {SHEN_FIELD(flags) 0x49002U, SHEN_FIELD(parameters) conversationMovingShotParameters[24], SHEN_FIELD(id) 125},
    {SHEN_FIELD(flags) 0x49002U, SHEN_FIELD(parameters) conversationMovingShotParameters[25], SHEN_FIELD(id) 126},
    {SHEN_FIELD(flags) 0x49002U, SHEN_FIELD(parameters) conversationMovingShotParameters[26], SHEN_FIELD(id) 127},
    {SHEN_FIELD(flags) 0x3004402U, SHEN_FIELD(parameters) conversationMovingShotParameters[27], SHEN_FIELD(id) 128},
    {SHEN_FIELD(flags) 0x3000802U, SHEN_FIELD(parameters) conversationMovingShotParameters[28], SHEN_FIELD(id) 129},
    {SHEN_FIELD(flags) 0x1004402U, SHEN_FIELD(parameters) conversationMovingShotParameters[29], SHEN_FIELD(id) 130},
    {SHEN_FIELD(flags) 0x1000802U, SHEN_FIELD(parameters) conversationMovingShotParameters[30], SHEN_FIELD(id) 131},
    {SHEN_FIELD(flags) 0x2004402U, SHEN_FIELD(parameters) conversationMovingShotParameters[31], SHEN_FIELD(id) 132},
    {SHEN_FIELD(flags) 0x2000802U, SHEN_FIELD(parameters) conversationMovingShotParameters[32], SHEN_FIELD(id) 133},
    {SHEN_FIELD(flags) 0x3041002U, SHEN_FIELD(parameters) conversationMovingShotParameters[33], SHEN_FIELD(id) 134},
    {SHEN_FIELD(flags) 0x1041002U, SHEN_FIELD(parameters) conversationMovingShotParameters[34], SHEN_FIELD(id) 135},
    {SHEN_FIELD(flags) 0x2041002U, SHEN_FIELD(parameters) conversationMovingShotParameters[35], SHEN_FIELD(id) 136},
    {SHEN_FIELD(flags) 0x200402U, SHEN_FIELD(parameters) conversationMovingShotParameters[36], SHEN_FIELD(id) 137},
    {SHEN_FIELD(flags) 0x200402U, SHEN_FIELD(parameters) conversationMovingShotParameters[37], SHEN_FIELD(id) 138},
    {SHEN_FIELD(flags) 0x200402U, SHEN_FIELD(parameters) conversationMovingShotParameters[38], SHEN_FIELD(id) 139},
    {SHEN_FIELD(flags) 0x200802U, SHEN_FIELD(parameters) conversationMovingShotParameters[39], SHEN_FIELD(id) 140},
    {SHEN_FIELD(flags) 0x200802U, SHEN_FIELD(parameters) conversationMovingShotParameters[40], SHEN_FIELD(id) 141},
    {SHEN_FIELD(flags) 0x200802U, SHEN_FIELD(parameters) conversationMovingShotParameters[41], SHEN_FIELD(id) 142},
    {SHEN_FIELD(flags) 0x400802U, SHEN_FIELD(parameters) conversationMovingShotParameters[42], SHEN_FIELD(id) 143},
    {SHEN_FIELD(flags) 0x400802U, SHEN_FIELD(parameters) conversationMovingShotParameters[43], SHEN_FIELD(id) 144},
    {SHEN_FIELD(flags) 0x400802U, SHEN_FIELD(parameters) conversationMovingShotParameters[44], SHEN_FIELD(id) 145},
    {SHEN_FIELD(flags) 0x800802U, SHEN_FIELD(parameters) conversationMovingShotParameters[45], SHEN_FIELD(id) 146},
    {SHEN_FIELD(flags) 0x800802U, SHEN_FIELD(parameters) conversationMovingShotParameters[46], SHEN_FIELD(id) 147},
    {SHEN_FIELD(flags) 0x800802U, SHEN_FIELD(parameters) conversationMovingShotParameters[47], SHEN_FIELD(id) 148},
    {SHEN_FIELD(flags) 0x8U, SHEN_FIELD(parameters) conversationFixedShotParameters[0], SHEN_FIELD(id) 0},
    {SHEN_FIELD(flags) 0x8U, SHEN_FIELD(parameters) conversationFixedShotParameters[1], SHEN_FIELD(id) 0},
    {SHEN_FIELD(flags) 0x8U, SHEN_FIELD(parameters) conversationFixedShotParameters[2], SHEN_FIELD(id) 0},
    {SHEN_FIELD(flags) 0x40000000U, SHEN_FIELD(parameters) customConversationCameraParameters, SHEN_FIELD(id) 0},
    {SHEN_FIELD(flags) 0xffffffffU, SHEN_FIELD(parameters) 0, SHEN_FIELD(id) 0},
};

SearchCameraState g_searchCameraState;
std::uint32_t g_searchCameraFov;
std::uint32_t g_searchCameraZoomFov;
float g_searchCameraZoomScale;
std::int32_t g_searchCameraYaw;
std::int32_t g_searchCameraPitch;

std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);

void camCtrlSetInterpolationSpeed(float speed);

void camCtrlSetInterpolationFrameCount(std::int32_t frames);

std::int32_t camCtrlSetState(std::int32_t state);

void camCtrlSetNextPosition(const Vector3 &position);

void camCtrlSetTargetPosition(const Vector3 &position);

Vector3s *camCtrlGetRot(Vector3s *rotation);

std::int32_t camCtrlSetPerspective(std::int16_t perspective);

std::int16_t camCtrlGetPerspective();

float camCtrlSetMotionScaleFactor(float factor);

void camCtrlSetFovTransitionTo(std::int32_t enabled);

void camCtrlCalculateFov(CameraRuntime &camera);

Vector3 *camCtrlGetNextCamPos(Vector3 *position);

Vector3 *camCtrlGetTargetPosition(Vector3 *position);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool IsCharacterInCurrentMap(const Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymCheckAuthorTargetRequested();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymHasRyoBag(std::int32_t mode);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool CheckCharacterCylinderIntersection(const Vector3 &start, const Vector3 &end);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool CheckCharacterSphereIntersection(const Vector3 &point, float radius);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ccmUpdateEntityFlag(Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t conversationPacketScratch[0x60] = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlBuildScriptCamSpline(std::int32_t name, const void *eye, const void *target);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlCreateSpline(const std::uint32_t *record, const void *eye, const void *target);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlHandleQueuedSpline();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t cameraMotionMode;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float cameraMotionPlayerAlpha = 1.0f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float fightCameraSelfHeight = 1.35000002f, fightCameraOtherHeight = 1.3f;
static float fightCameraMinimumDistance = 2.6600001f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightCameraBoneHeightEnabled = -1;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float fightCameraEyeScale = 0.8f, fightCameraFocusEyeScale = 0.8f, fightCameraLeadScale = 1.0f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightCameraTargetOwnBone, fightCameraFirstPersonEyeMode = 2, fightCameraFirstPersonEyeOrbit;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float cameraMotionDistance, cameraMotionDoubleDistance, cameraMotionDistanceSquared;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float cameraMotionDoubleDistanceSquared, cameraMotionSecondaryDistance, cameraMotionSecondaryDoubleDistance;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t cameraMotionPatternSet;
static std::int32_t fightCameraLinkedKind;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 fightCameraEye, fightCameraTarget, fightCameraDesiredEye, fightCameraDesiredTarget;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 fightCameraEyeDirection, fightCameraTargetDirection, fightCameraOffset, fightCameraGoalOffset;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightCameraFocusFrames, fightCameraSpeedDown, fightCameraSpeedUp, fightCameraGoalLift;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightCameraRayHit, fightCameraSide, fightCameraLinkedSide, fightCameraLinkedFrames, fightCameraHasIntersectionTest;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Character *fightMotionCameraTarget;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 fightMotionCameraSavedEye, fightMotionCameraSavedTarget, fightMotionCameraSegmentTarget;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 fightMotionCameraWaypoints[6];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightMotionCameraWaypointCount, fightMotionCameraWaypointIndex, fightMotionCameraState, fightMotionCameraUpdateState;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t fightMotionCameraTime, fightMotionCameraPreviousPerspective, fightMotionCameraInitializeEye;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Character *fightCameraLinkedCharacter;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static FightActorRuntimePrefix *fightCameraLinkedActor;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float fightCameraDistanceSetting, fightCameraHeightSetting, fightCameraCurrentSpeed, fightCameraHeightOffset;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static FightCameraVelocity fightCameraVelocity;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct FightCameraPatternState {
    std::uint32_t flags;
    std::int32_t frame;
    std::uint32_t state;
    float phaseX, phaseY;
    std::int32_t previousIndex, index;
    float speed;
    std::int32_t previousVariant, variant, roll, goalRoll;
};
static FightCameraPatternState fightCameraPattern;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const float fightCameraPhaseTargets[8][2] = {{0, 0}, {-0.2f, 0.2f}, {-0.30000001f, 0.1f}, {0, 0.5f}, {1.5f, 0}, {2, 0.5f}, {3.5f, 0.5f}, {6, 0.5f}};
static const float fightCameraPatternSpeeds[5][3] = {{80, 160, 240}, {80, 120, 160}, {80, 160, 240}, {80, 180, 300}, {80, 140, 200}};
static const std::uint8_t fightCameraPatterns[5][8] = {
    {0, 0, 0, 2, 3, 3, 4, 5}, {0, 0, 0, 2, 2, 2, 4, 5}, {0, 0, 0, 2, 3, 4, 4, 5}, {0, 0, 2, 2, 5, 5, 6, 7}, {0, 0, 2, 2, 2, 2, 4, 4}};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_cameraMathWork = 0;
#endif

// @verified - d
void InitializeConversationCameraState() {
    ResetConversationCameraState(g_conversationCameraState);
}

// @verified - d
std::int32_t SetConversationCameraEnabled(std::int32_t enabled) {
    if (!(g_conversationCameraState->flags & 2U))
        return 0;
    if (!enabled)
        g_conversationCameraState->flags |= 0x100U;
    else if (enabled > 0)
        g_conversationCameraState->flags &= ~0x100U;
    return g_conversationCameraState->flags & 0x100U ? 0 : 1;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int16_t ConversationBlendAngle(std::int16_t first, std::int16_t last, float amount) {
    const std::int16_t difference = static_cast<std::int16_t>(last - first);
    if (::fabsf(amount) < FLT_EPSILON || ::fabsf(static_cast<float>(difference)) < FLT_EPSILON)
        return first;
    return static_cast<std::int16_t>(static_cast<std::uint32_t>(first) + TruncateToInt32(difference * amount));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 ConversationBlendPosition(const Vector3 &first, const Vector3 &last, float amount) {
    Vector3 result;
    result.x = CameraTransitionValue(first.x, last.x, amount);
    result.y = CameraTransitionValue(first.y, last.y, amount);
    result.z = CameraTransitionValue(first.z, last.z, amount);
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ActivateConversationCameraStream(const ConversationDialogueRecord *stream) {
    ConversationCameraState *state = g_conversationCameraState;
    const std::int32_t cameraState = camCtrlGetState();
    if (cameraState == 6 || cameraState == 8)
        return;
    state->root = ctrlGetRootNode();
    state->rootScenarioName = ctrlGetScenarioCharaName(state->root);
    if (!state->root)
        return;
    std::uint32_t nextFlags = 0x6000000U;
    nextFlags |= state->flags & 0xb0100400U;
    state->flags = nextFlags;
    if (IsActiveScenarioCameraSuppressed())
        state->flags |= 8U;
    if (!state->shotGenerationCount)
        state->flags |= 4U;
    if (plymCheckAuthorTargetRequested())
        state->flags |= 0x400000U;
    state->sceneFlags = 0;
    if (plymCheckAuthorTargetRequested())
        state->sceneFlags |= 0x10U;
    state->shotDurationFrames = 0;
    state->shotFrame = 0;
    state->shotProgress = 0.0f;
    state->activationFrames = 0;
    state->overrideElapsed = 0;
    state->totalFrames = 0;
    state->viewUpdateCount = 0;
    state->lookDelayFrames = 0;
    state->lookTurnFrames = 0;
    state->actorDistance = 0.0f;
    state->actorHeightDelta = 0.0f;
    state->source = stream;
    state->preparedStream = stream;
    state->shotCount = 0;
    state->shotIndex = 0;
    state->memberCount = 0;
    for (std::int32_t index = 0; index < 5; ++index) {
        if (index <= 1)
            state->memberSeen[index] = 1;
        else
            state->memberSeen[index] = 0;
    }
    for (std::int32_t index = 0; index < 24; ++index)
        state->shotFlags[index] = 0;
    state->currentShotFlags = 0;
    for (std::int32_t index = 0; index < 24; ++index)
        state->shotTypes[index] = -1;
    state->shotStartTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    CompressConversationCameraStream(state);
    ReadConversationCameraPackets(state);
    AssignConversationShotAnchors(state);
    BindConversationShotActors(state);
    UpdateConversationActorPose(state);
    RefreshConversationSceneFlags(state);
    if (state->flags & 0x80000U)
        UpdateConversationAngleClass(state);
    SelectConversationCameraSide(state);
    AssignConversationShotTypes(state);
    state->activationFrames = 7;
    if (state->sceneFlags & 0x10U)
        state->activationFrames = 1;
    TickConversationActivation(state);
    camCtrlGetNextCamPos(&state->framePreviousEye);
    camCtrlGetTargetPosition(&state->framePreviousTarget);
    state->framePreviousFieldOfView = static_cast<std::uint16_t>(camCtrlGetPerspective());
    CaptureConversationParticipantAlpha(state);
    state->flags |= 0x202U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EndConversationCameraStream() {
    ConversationCameraState *state = g_conversationCameraState;
    if (!(state->flags & 2U))
        return;
    state->flags &= ~2U;
    state->flags &= ~0x100000U;
    state->flags &= ~0x8000400U;
    std::int32_t cut = 1;
    if (state->currentShotFlags & 8U)
        cut = 0;
    else if (state->shotType == 1 && state->targetScenarioName == state->specialTargetName) {
        Vector3 root, eye, delta;
        std::int16_t directionAngles[2];
        Vector3s cameraAngles;
        charaGetMotWorldPos(state->root, 5, &root);
        const std::int16_t yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(state->root));
        camCtrlGetNextCamPos(&eye);
        camCtrlGetRot(&cameraAngles);
        am2Vector3Subtract(&delta, &eye, &root);
        Vector3_ToFixed16EulerAngles(delta, *reinterpret_cast<Vector3s *>(directionAngles));
        const std::int16_t dy = static_cast<std::int16_t>(directionAngles[1] - yaw);
        int magnitude;
        if (dy < 0)
            magnitude = -dy;
        else
            magnitude = dy;
        if (0x2aaa < magnitude) {
            int cx = cameraAngles.x;
            int cy = static_cast<std::int16_t>(cameraAngles.y - yaw);
            if (cy < 0)
                cy = -cy;
            if (cy <= 0x1fff) {
                if (cx < 0)
                    cx = -cx;
                if (cx <= 0x1fff)
                    cut = 0;
            }
        }
    }
    if (cut)
        state->flags |= 0x20U;
    RestoreConversationParticipantAlpha(state);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline std::int32_t GetConversationHistoryIndex(std::int32_t current, std::int32_t back) {
    std::int32_t index = current - back;
    if (index < 0)
        index += 4;
    return index;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RestoreConversationCamera() {
    ConversationCameraState &state = *g_conversationCameraState;
    state.flags &= ~2U;
    state.flags &= ~0x80U;
    state.shotGenerationCount = 0;
    state.forcedCandidate = -1;
    camCtrlGetState();
    for (std::int32_t index = 1; index <= 3; ++index) {
        const std::int32_t previous = GetConversationHistoryIndex(state.historyIndex, index + 1);
        state.history[previous].candidate = -1;
        state.history[previous].shotType = -1;
    }
    if (camCtrlGetState() == 7) {
        if (!GetScenarioFollowCharacter()) {
            camCtrlSetState(0);
            if (state.flags & 0x20U)
                camCtrlSetFovTransitionTo(1);
            else {
                camCtrlSetInterpolationMode(1);
                camCtrlSetInterpolationSpeed(0.02f);
            }
        } else if (state.flags & 0x20U) {
            camCtrlSetFovTransitionTo(1);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResumeConversationCameraWrapper() {
    ResumeConversationCamera();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResumeConversationCamera() {
    ConversationCameraState *state = g_conversationCameraState;
    if (camCtrlGetState() == 7)
        state->flags |= 0x82U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateConversationCamera() {
    ConversationCameraState &state = *g_conversationCameraState;
    if (!(state.flags & 2U))
        return;
    if (state.flags & 0x100U)
        return;
    state.root = ctrlGetRootNode();
    ++state.viewUpdateCount;
    state.framePreviousEye = state.eye;
    state.framePreviousTarget = state.target;
    state.framePreviousFieldOfView = state.fieldOfView;
    TickConversationActivation(&state);
    AdvanceConversationShotClock(&state);
    EvaluateConversationShot(&state);
    if (!(state.flags & 0x1000000U)) {
        camCtrlSetNextPosition(state.eye);
        camCtrlSetTargetPosition(state.target);
        camCtrlSetPerspective(static_cast<std::int16_t>(state.fieldOfView));
        UpdateConversationMemberAlpha(&state);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void PauseConversationCameraClock(std::int32_t paused) {
    if (!(g_conversationCameraState->flags & 2U))
        return;
    if (paused)
        g_conversationCameraState->pauseStartTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    else
        g_conversationCameraState->shotStartTime -=
            g_conversationCameraState->pauseStartTime - *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t camCtrlHoldConversationShot(std::int32_t release) {
    ConversationCameraState *state = g_conversationCameraState;
    if (!(state->flags & 2U))
        return 0;
    if (!release)
        state->flags |= 0x100U;
    else
        state->flags &= ~0x100U;
    if (release >= 0)
        TickConversationActivation(state);
    return release;
}
#endif

// @verified - d
void ResetConversationCameraState(ConversationCameraState *state) {
    std::memset(state, 0, sizeof(*state));
    state->forcedCandidate = -1;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    state->historyIndex = 0;
    state->previousHistory = 0;
#else
    state->collisionPhase = 0;
    state->historyIndex = 0;
#endif
    for (std::int32_t index = 0; index < 4; ++index) {
        state->history[index].candidate = -1;
        state->history[index].duplicateCandidate = -1;
        state->history[index].shotType = -1;
    }
    ResetConversationCandidates(state);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TickConversationActivation(ConversationCameraState *state) {
    if (state->activationFrames > 0) {
        --state->activationFrames;
        state->flags |= 0x1000000U;
        if (!state->activationFrames && (state->flags & 0x1000000U)) {
            state->flags &= ~0x1000000U;
            camCtrlSetState(7);
            camCtrlGetNextCamPos(&state->previousEye);
            camCtrlGetTargetPosition(&state->previousTarget);
            state->previousFieldOfView = static_cast<std::uint16_t>(camCtrlGetPerspective());
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AdvanceConversationShotClock(ConversationCameraState *state) {
    state->flags &= ~0x40U;
    const std::uint32_t now = *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister);
    float elapsed = static_cast<float>(state->shotStartTime - now);
    elapsed = elapsed * 30.0f / 12500000.0f;
    const float duration = state->shots[state->shotIndex]->duration * 30.0f;
    if (elapsed >= duration && state->shotIndex < state->shotCount - 1) {
        state->shotStartTime = now;
        camCtrlGetNextCamPos(&state->previousEye);
        camCtrlGetTargetPosition(&state->previousTarget);
        state->previousFieldOfView = static_cast<std::uint16_t>(camCtrlGetPerspective());
        state->entryBlendFrames = 0;
        ++state->shotIndex;
        state->actorDistance = 1.0f;
        state->actorHeightDelta = 0.0f;
        if (!(state->flags & 0x4000U))
            state->flags &= ~0x8000U;
        state->shotFrame = 0;
    }
    if (state->shotIndex == state->shotCount - 1 && elapsed >= duration)
        state->flags |= 0x40000000U;
    const ConversationDialogueRecord *const shot = state->shots[state->shotIndex];
    state->shotDurationFrames = static_cast<std::int16_t>(static_cast<std::int32_t>(shot->duration * 30.0f));
    if (!state->shotFrame) {
        state->flags |= 0x40U;
        BeginConversationShot(state);
    }
    state->flags &= ~0x400U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BeginConversationShot(ConversationCameraState *state) {
    ++state->shotGenerationCount;
    state->flags &= ~0x10U;
    state->flags &= ~0x2800U;
    state->flags &= ~0x30000U;
    state->flags &= ~0x600000U;
    state->flags &= ~0x8000000U;
    state->specialOverrideFrames = 0;
    state->shotScratch = 0;
    state->groundBias = 0.1f;
    state->currentShotFlags = state->shotFlags[state->shotIndex];
    state->shotType = state->shotTypes[state->shotIndex];
    BindConversationShotActors(state);
    RefreshConversationSceneFlags(state);
    if (state->flags & 0x400000U)
        state->flags |= 0x200000U;
    ResetConversationShotTypeForFlag10(state);
    if (state->flags & 0x10U) {
        ResetConversationCandidates(state);
        SelectConversationCandidate(state);
        return;
    }
    const ConversationShotHistory *const previous = state->previousHistory;
    if (previous && (state->currentShotFlags & 2U) && state->eyeScenarioName == state->rootScenarioName && previous->eyeName == state->eyeScenarioName &&
        previous->targetName != state->targetScenarioName && previous->shotType == 1)
        state->flags |= 0x10000U;
    UpdateConversationMirrorFlags(state);
    UpdateConversationActorPose(state);
    ResetConversationCandidates(state);
    FilterConversationCandidates(state);
    SelectConversationCandidate(state);
    RecordConversationShot(state);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RefreshConversationSceneFlags(ConversationCameraState *state) {
    state->sceneFlags = 0;
    if (state->flags & 0x80000U)
        state->sceneFlags = 2;
    if (state->flags & 0x800000U)
        state->sceneFlags |= 4U;
    if (QueryNpcRouteLiveState(state->specialTargetName) == 13)
        state->sceneFlags |= 8U;
    Vector3 first, second;
    charaGetMotWorldPos(state->root, 6, &first);
    charaGetMotWorldPos(state->specialTarget, 6, &second);
    flddPushIntersectionFlag(0x1000);
    if (TestFieldWorldSegment(&first, &second))
        state->sceneFlags |= 0x20U;
    if (QueryNpcCameraMotionProfile(state->specialTargetName, 1))
        state->sceneFlags |= 0x80U;
    if (QueryNpcCameraMotionProfile(state->specialTargetName, 2))
        state->sceneFlags |= 0x40U;
    if (QueryNpcCameraMotionProfile(state->specialTargetName, 4))
        state->sceneFlags = (state->sceneFlags | 0x100U) & ~0xc0U;
    if (QueryNpcSceneMotionPlaying(state->specialTargetName))
        state->sceneFlags |= 0x200U;
    if (plymCheckAuthorTargetRequested())
        state->sceneFlags |= 0x10U;
    if (state->sceneFlags & 0x10U)
        state->flags |= 0x400000U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateConversationMemberAlpha(ConversationCameraState *state) {
    std::int32_t hideTarget = 0;
    std::int32_t index;
    if (!(state->flags & 0x200U))
        return;
    hideTarget = !(state->flags & 0x10U) && !state->shotType && state->memberCount < 3 && !(state->currentShotFlags & 8U);
    for (index = 0; index < state->memberCount; ++index) {
        Character *const character = state->members[index];
        if (state->root == character) {
            float alpha = state->eyeActor == character && hideTarget ? 0.0f : 1.0f;
            if (state->flags & 0x1000U)
                alpha = 1.0f;
            character->alpha = alpha;
        } else if (state->eyeActor == character && hideTarget) {
            cpamSetFlag(character, 0);
            character->alpha = 0.0f;
        } else {
            if (((state->alphaParticipants + index)->flags >> 7) & 1U) {
                if (!shenCheckCharaHasAlphaManager(character))
                    cpamSetFlag(character, 1);
            } else
                character->alpha = 1.0f;
            if (state->flags & 0x1000U)
                character->alpha = 0.0f;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline std::int8_t ConversationRecordFlags(const ConversationDialogueRecord *record) {
    return reinterpret_cast<const std::int8_t *>(&record->command)[3];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline std::size_t ConversationRecordBytes(const ConversationDialogueRecord *record) {
    return static_cast<std::int8_t>(record->command) * 4;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline const ConversationDialogueRecord *NextConversationRecord(const ConversationDialogueRecord *record) {
    return reinterpret_cast<const ConversationDialogueRecord *>(reinterpret_cast<const std::byte *>(record) + ConversationRecordBytes(record));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void AssignConversationShotTypes(ConversationCameraState *state) {
    std::uint32_t previousType = 1;
    if (state->shotCount <= 0)
        return;
    const ConversationShotHistory *history = state->previousHistory;
    std::int32_t historyType;
    shen_name historyTarget;
    if (history) {
        historyType = history->shotType;
        historyTarget = history->targetName;
    } else {
        historyType = -1;
        historyTarget = shen_name();
    }
    for (std::int32_t shot = 0; shot < state->shotCount; ++shot) {
        const std::int8_t flags = state->shotFlags[shot];
        Character *partner = state->partners[shot];
        std::uint32_t type;
        if (!shot) {
            type = 1;
            if (historyType == 1)
                type = historyTarget != state->targetScenarioName;
        } else {
            type = 1;
            if (previousType == 1)
                type = 0;
            else if (previousType == 3)
                type = 0;
        }
        if (flags & 2)
            type = 1;
        if ((state->flags & 0x40000U) && state->actorHeightDelta > 0.0f && state->targetAnchors[shot] == state->root)
            type = 0;
        if ((state->sceneFlags & 0x28U) && state->targetAnchors[shot] == state->root)
            type = 0;
        if (flags & 8) {
            const std::int32_t blocked = AreConversationAngleCandidatesBlocked(state);
            type = !blocked;
            if (!blocked && (state->orientationClass < 0 ? -state->orientationClass : state->orientationClass) <= 4) {
                state->targetAnchors[shot] = partner;
                state->eyeAnchors[shot] = state->root;
            } else {
                state->targetAnchors[shot] = state->root;
                state->eyeAnchors[shot] = partner;
            }
            if ((state->sceneFlags & 0x28U) && type == 1 && state->targetAnchors[shot] == state->root)
                type = 0;
        }
        if (!shot) {
            if (state->flags & 8U)
                type = 0;
        } else if (state->shotTypes[shot - 1] == static_cast<std::int32_t>(type) && state->targetAnchors[shot - 1] == state->targetAnchors[shot]) {
            const std::uint32_t flipped = !type;
            type = flipped;
            if ((state->sceneFlags & 0x28U) && flipped == 1 && state->targetAnchors[shot - 1] == state->root) {
                state->targetAnchors[shot] = partner;
                state->eyeAnchors[shot] = state->root;
            }
            if ((flags & 8) && !type) {
                state->targetAnchors[shot] = state->root;
                state->eyeAnchors[shot] = partner;
            }
        }
        state->shotTypes[shot] = static_cast<std::int8_t>(type);
        previousType = type;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ResetConversationShotTypeForFlag10(ConversationCameraState *state) {
    if (state->flags & 0x10U) {
        state->shotTypes[state->shotIndex] = 0;
        state->shotType = state->shotTypes[state->shotIndex];
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void RecordConversationShot(ConversationCameraState *state) {
    ConversationShotHistory &history = state->history[state->historyIndex];
    if (++state->historyIndex > 3)
        state->historyIndex = 0;
    history.candidate = state->candidateIndex;
    history.duplicateCandidate = state->candidateIndex;
    history.shotType = state->shotType;
    history.targetName = state->targetScenarioName;
    history.eyeName = state->eyeScenarioName;
    state->previousHistory = &history;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void AdvanceConversationFrame(ConversationCameraState *state) {
    if (state->shotFrame <= state->shotDurationFrames)
        ++state->shotFrame;
    state->shotProgress = GetConversationProgress(state);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static float GetConversationProgress(ConversationCameraState *state) {
    const float frame = static_cast<float>(state->shotFrame);
    float duration;
    if (state->shotDurationFrames <= 179)
        duration = 180.0f;
    else
        duration = static_cast<float>(state->shotDurationFrames);
    const float value = frame / duration;
    state->interpolationProgress = value;
    float progress = (value < 0.0f ? 0.0f : value) < 1.0f ? (value < 0.0f ? 0.0f : value) : 1.0f;
    if (state->flags & 0x10000U)
        state->interpolationProgress = progress = TangentEaseValue(0.0f, 1.0f, value, 0.95f);
    if (::fabsf(progress) < FLT_EPSILON)
        progress = 0.0f;
    static const float conversationProgressWeights[11][2] = {{0.0f, 1.0f}, {0.0f, 0.25f}, {0.25f, 0.25f}, {0.5f, 0.25f}, {0.75f, 0.25f}, {0.0f, 0.75f},
        {0.25f, 0.75f}, {0.0f, 0.5f}, {0.25f, 0.5f}, {0.5f, 0.5f}, {0.0f, 1.0f}};
    std::int32_t weight = 0;
    if (state->shotType == 1)
        weight = static_cast<std::int8_t>(state->candidates[state->candidateIndex].collisionClass);
    float base = conversationProgressWeights[weight][0];
    return base + conversationProgressWeights[weight][1] * progress;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EvaluateConversationShot(ConversationCameraState *state) {
    AdvanceConversationFrame(state);
    UpdateConversationActorPose(state);
    GenerateConversationCandidate(state);
    BlendConversationPreviousShot(state);
    ApplyConversationLookTurn(state);
    ApplyConversationSpecialModeOverride(state);
    BlendConversationEntryCamera(state);
    Vector3 player;
    Vector3 result;
    Vector3 ray;
    ray.x = state->eye.x;
    ray.y = state->eye.y + 0.5f;
    ray.z = state->eye.z;
    plymUpdatePlayerSearchLocation(&player);
    if (ray.y < player.y)
        ray.y = player.y + 0.5f;
    const float floor = FindFieldIntersectionHeight(&ray);
    if (floor != -FLT_MAX) {
        if (state->eye.y < floor + 0.15f) {
            state->eye.y = floor + 0.05f + state->groundBias;
            state->groundBias = InterpolateTo(0.0f, state->groundBias, 0.02f);
        } else {
            state->groundBias = 0.1f;
        }
    }
    flddPushIntersectionFlag(8);
    if (FindFieldSegmentOffsetInWorld(&result, &state->eye, &state->target))
        state->eye.y = result.y;
    if (static_cast<std::int16_t>(state->fieldOfView) > 12742)
        state->fieldOfView = 12743;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DetectConversationActorMovement(ConversationCameraState *state) {
    Vector3 rootPosition, specialPosition, rootDelta, specialDelta;
    state->flags &= ~0x800U;
    GetCharacterWorldPosition(state->root, &rootPosition);
    GetCharacterWorldPosition(state->specialTarget, &specialPosition);
    am2Vector3Subtract(&rootDelta, &rootPosition, reinterpret_cast<Vector3 *>(state->retained4f9 + 3));
    am2Vector3Subtract(&specialDelta, &specialPosition, reinterpret_cast<Vector3 *>(state->retained4f9 + 15));
    *reinterpret_cast<Vector3 *>(state->retained4f9 + 3) = rootPosition;
    *reinterpret_cast<Vector3 *>(state->retained4f9 + 15) = specialPosition;
    if (state->flags & 0x40U)
        return;
    SetDebugScreenTextPosition(20, 20);
    if (am2Vector3LengthSquared(&rootDelta) > 0.025f * 0.025f) {
        shenmue_s2_event_DebugLogToScreen("type A\n");
        state->flags |= 0x800U;
    } else if (am2Vector3LengthSquared(&specialDelta) > 0.025f * 0.025f) {
        shenmue_s2_event_DebugLogToScreen("type B\n");
        state->flags |= 0x800U;
    } else {
        shenmue_s2_event_DebugLogToScreen("type C\n");
        state->flags &= ~0x800U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateConversationActorPose(ConversationCameraState *state) {
    state->flags &= ~0x40000U;
    state->previousActorTarget = state->actorTarget;
    state->previousActorEye = state->actorEye;
    state->previousActorYaw = state->actorYaw;
    if (state->flags & 0x10U) {
        charaGetMotWorldPos(state->root, 5, &state->actorTarget);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&state->actorTarget);
        Matrix4x4::RotateAroundY_Wrapper(GetCharacterWorldYaw(state->root));
        Matrix4x4::preTranslate(0.0f, 0.0f, -3.0f);
        matrix_stack::GetTranslation(&state->actorEye);
        PopCurrentMatrix(1);
        state->actorYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(state->root));
    } else {
        ReadConversationActorPose(state);
        SmoothConversationActorPose(state);
    }
    state->actorDistance = CalculatePlanarDistanceBetweenPoints(&state->actorTarget, &state->actorEye);
    state->actorHeightDelta = state->actorEye.y - state->actorTarget.y;
    Vector3 delta;
    am2Vector3Subtract(&delta, &state->actorEye, &state->actorTarget);
    Vector3_ToFixed16EulerAngles(delta, state->actorDirection);
    if ((state->actorHeightDelta < 0.0f ? -state->actorHeightDelta : state->actorHeightDelta) > 0.35f)
        state->flags |= 0x40000U;
    if ((state->flags & 0x40000U) && state->actorHeightDelta < 0.0f)
        state->actorDirection.x /= 2;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ReadConversationActorPose(ConversationCameraState *state) {
    Vector3 position, sum;
    std::int32_t count;
    std::int32_t index;
    switch (state->shotType) {
    case 0:
    case 1:
    default:
        charaGetMotWorldPos(state->targetActor, 5, &state->actorTarget);
        charaGetMotWorldPos(state->eyeActor, 5, &state->actorEye);
        state->actorYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(state->targetActor));
        if (state->sceneFlags & 0x10U)
            state->actorYaw = static_cast<std::int16_t>(plymGetAuthorPlacementYaw(state->specialTargetName));
        break;
    case 3:
        sum.x = sum.y = sum.z = 0.0f;
        count = 0;
        for (index = 0; index < state->memberCount; ++index) {
            if (state->members[index] != state->eyeActor) {
                charaGetMotWorldPos(state->members[index], 5, &position);
                am2Vector3AddInPlace(&sum, &position);
                ++count;
            }
        }
        state->actorTarget.x = sum.x / static_cast<float>(count);
        state->actorTarget.y = sum.y / static_cast<float>(count);
        state->actorTarget.z = sum.z / static_cast<float>(count);
        charaGetMotWorldPos(state->eyeActor, 5, &state->actorEye);
        state->actorYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(state->eyeActor));
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define CONVERSATION_ABS(x) ((x) < 0.0f ? -(x) : (x))

static void SmoothConversationActorPose(ConversationCameraState *state) {
    Vector3 eyeDelta;
    Vector3 targetDelta;
    Vector3 head;
    Vector3 screen;
    std::uint32_t mode = 0;
    if (state->flags & 0x40U)
        return;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(mtwkGetBoneTransform(charFindMotionwork(state->targetActor), 5));
    Matrix4x4::preTranslate(0.06549f, 0.0f, 0.08197f);
    matrix_stack::GetTranslation(&head);
    PopCurrentMatrix(1);
    float margin = ProjectConversationActorToScreen(head, screen) * 0.1f;
    margin = CONVERSATION_ABS(margin) > 16.0f ? CONVERSATION_ABS(margin) : 16.0f;
    if (CONVERSATION_ABS(screen.x) > 320.0f - margin || CONVERSATION_ABS(screen.y) > 240.0f - margin)
        mode |= 0x2000U;
    if (state->flags & 0x8000U)
        mode &= ~0x2000U;
    if (state->sceneFlags & 0x10U)
        mode |= 0x800U;
    if (mode) {
        if ((state->flags & 0x2000U) && state->specialOverrideFrames > 30)
            state->specialOverrideFrames = 30;
        state->flags |= 0x2000U;
    } else {
        if (state->specialOverrideFrames < 4)
            state->flags &= ~0x2800U;
        if (state->specialOverrideFrames == 60)
            state->flags &= ~0x2800U;
    }
    if (!(state->flags & 0x2800U)) {
        if (!state->shotIndex) {
            state->actorTarget.x = InterpolateTo(state->actorTarget.x, state->previousActorTarget.x, 0.02f);
            state->actorTarget.y = InterpolateTo(state->actorTarget.y, state->previousActorTarget.y, 0.02f);
            state->actorTarget.z = InterpolateTo(state->actorTarget.z, state->previousActorTarget.z, 0.02f);
            state->actorEye.x = InterpolateTo(state->actorEye.x, state->previousActorEye.x, 0.02f);
            state->actorEye.y = InterpolateTo(state->actorEye.y, state->previousActorEye.y, 0.02f);
            state->actorEye.z = InterpolateTo(state->actorEye.z, state->previousActorEye.z, 0.02f);
        } else {
            state->actorTarget = state->previousActorTarget;
            state->actorEye = state->previousActorEye;
        }
        state->specialOverrideFrames = 0;
        return;
    }
    float amount = static_cast<float>(++state->specialOverrideFrames) / 60.0f;
    amount = CosineEaseValue(0.0f, 1.0f, amount);
    state->specialOverrideFrames = state->specialOverrideFrames > 60 ? 60 : state->specialOverrideFrames;
    if (state->sceneFlags & 0x10U)
        amount = 1.0f;
    am2Vector3Subtract(&targetDelta, &state->actorTarget, &state->previousActorTarget);
    am2Vector3Subtract(&eyeDelta, &state->actorEye, &state->previousActorEye);
    float distance = CONVERSATION_ABS(targetDelta.y);
    float vertical;
    if (distance < 0.01f)
        vertical = 0.25f;
    else if (distance < 0.3f)
        vertical = (distance - 0.01f) / (0.3f - 0.01f) * (1.0f - 0.25f) + 0.25f;
    else
        vertical = 1.0f;
    distance = ::sqrtf(targetDelta.x * targetDelta.x + targetDelta.z * targetDelta.z);
    float horizontal;
    if (distance < 0.01f)
        horizontal = 0.25f;
    else if (distance < 0.1f)
        horizontal = (distance - 0.01f) / (0.1f - 0.01f) * (1.0f - 0.25f) + 0.25f;
    else
        horizontal = 1.0f;
    horizontal *= amount * 0.5f;
    vertical *= amount * 0.5f;
    state->actorTarget.x = InterpolateTo(state->actorTarget.x, state->previousActorTarget.x, horizontal);
    state->actorTarget.y = InterpolateTo(state->actorTarget.y, state->previousActorTarget.y, vertical);
    state->actorTarget.z = InterpolateTo(state->actorTarget.z, state->previousActorTarget.z, horizontal);
    distance = CONVERSATION_ABS(eyeDelta.y);
    if (distance < 0.01f)
        vertical = 0.15f;
    else if (distance < 0.3f)
        vertical = (distance - 0.01f) / (0.3f - 0.01f) * (1.0f - 0.15f) + 0.15f;
    else
        vertical = 1.0f;
    distance = ::sqrtf(eyeDelta.x * eyeDelta.x + eyeDelta.z * eyeDelta.z);
    if (distance < 0.01f)
        horizontal = 0.25f;
    else if (distance < 0.1f)
        horizontal = (distance - 0.01f) / (0.1f - 0.01f) * (1.0f - 0.25f) + 0.25f;
    else
        horizontal = 1.0f;
    horizontal *= amount * 0.5f;
    vertical *= amount * 0.5f;
    state->actorEye.x = InterpolateTo(state->actorEye.x, state->previousActorEye.x, horizontal);
    state->actorEye.y = InterpolateTo(state->actorEye.y, state->previousActorEye.y, vertical);
    state->actorEye.z = InterpolateTo(state->actorEye.z, state->previousActorEye.z, horizontal);
    state->actorYaw = CameraLerpAngle(state->actorYaw, state->previousActorYaw, 0.1f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FitConversationEyeDistance(ConversationCameraState *state) {
    if (!state->shotType)
        return;
    Vector3 view;
    std::int16_t angles[2];
    Vector3 world, local, extent;
    am2Vector3Subtract(&view, &state->target, &state->eye);
    Vector3_ToFixed16EulerAngles(view, *reinterpret_cast<Vector3s *>(angles));
    extent.x = extent.y = extent.z = 0.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundX_Wrapper(-angles[0]);
    Matrix4x4::RotateAroundY_Wrapper(-angles[1]);
    matrix_stack::TranslateInverseVector(&state->eye);
    for (std::int32_t index = 0; index < state->memberCount; ++index) {
        Character *const member = state->members[index];
        if (member == state->eyeActor)
            continue;
        charaGetMotWorldPos(member, 5, &world);
        Matrix4x4::TransformPoint(world, local);
        if (local.z > 0.0f)
            continue;
        if (CONVERSATION_ABS(extent.x) < CONVERSATION_ABS(local.x))
            extent = local;
    }
    PopCurrentMatrix(1);
    const float scale = 320.0f / dc::sdk::TanFixed(static_cast<std::int16_t>(state->fieldOfView) / 2);
    float screenX = scale * extent.x / extent.z;
    float limit = 288.0f;
    if (!(CONVERSATION_ABS(screenX) > limit))
        return;
    float push = scale * CONVERSATION_ABS(extent.x) / limit - CONVERSATION_ABS(extent.z);
    push = (push < 0.0f ? 0.0f : push) < 4.0f ? (push < 0.0f ? 0.0f : push) : 4.0f;
    am2Vector3NormalizeInPlace(&view);
    am2Vector3Scale(&view, &view, -push);
    am2Vector3AddInPlace(&state->eye, &view);
    if (FindFieldSegmentOffsetInWorld(&world, &state->eye, &state->target))
        state->eye = world;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FitConversationFieldOfView(ConversationCameraState *state) {
    if (!state->shotType)
        return;
    if (state->shotType == 1)
        return;
    Vector3 view;
    std::int16_t angles[2];
    Vector3 world, local, extent;
    am2Vector3Subtract(&view, &state->target, &state->eye);
    Vector3_ToFixed16EulerAngles(view, *reinterpret_cast<Vector3s *>(angles));
    extent.x = extent.y = extent.z = 0.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundX_Wrapper(-angles[0]);
    Matrix4x4::RotateAroundY_Wrapper(-angles[1]);
    matrix_stack::TranslateInverseVector(&state->eye);
    for (std::int32_t index = 0; index < state->memberCount; ++index) {
        Character *const member = state->members[index];
        if (member == state->eyeActor)
            continue;
        charaGetMotWorldPos(member, 5, &world);
        Matrix4x4::TransformPoint(world, local);
        if (local.z > 0.0f)
            continue;
        if (CONVERSATION_ABS(extent.x) < CONVERSATION_ABS(local.x))
            extent = local;
    }
    PopCurrentMatrix(1);
    float screenX = 320.0f / dc::sdk::TanFixed(static_cast<std::int16_t>(state->fieldOfView) / 2) * extent.x / extent.z;
    float limit = 304.0f;
    if (CONVERSATION_ABS(screenX) > limit) {
        const float ratio = CONVERSATION_ABS(limit * extent.z / extent.x);
        const std::uint32_t fieldOfView = dc::sdk::ArcTanFixed(320.0f / ratio) * 2;
        state->fieldOfView = static_cast<std::uint16_t>(fieldOfView);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void BlendConversationEntryCamera(ConversationCameraState *state) {
    if (!(state->flags & 0x20000U))
        return;
    const float amount = static_cast<float>(++state->entryBlendFrames) / 30.0f;
    if (amount > 1.0f)
        return;
    state->eye.x = CameraTransitionValue(state->previousEye.x, state->eye.x, amount);
    state->eye.y = CameraTransitionValue(state->previousEye.y, state->eye.y, amount);
    state->eye.z = CameraTransitionValue(state->previousEye.z, state->eye.z, amount);
    state->target.x = CameraTransitionValue(state->previousTarget.x, state->target.x, amount);
    state->target.y = CameraTransitionValue(state->previousTarget.y, state->target.y, amount);
    state->target.z = CameraTransitionValue(state->previousTarget.z, state->target.z, amount);
    state->fieldOfView = CameraTransitionAngle(static_cast<std::int16_t>(state->previousFieldOfView), static_cast<std::int16_t>(state->fieldOfView), amount);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void BlendConversationPreviousShot(ConversationCameraState *state) {
    if (!(state->flags & 0x10000U))
        return;
    const float amount = state->interpolationProgress;
    if (amount > 1.0f)
        return;
    const float targetAmount = (amount * 1.2f < 0.0f ? 0.0f : amount * 1.2f) < 1.0f ? (amount * 1.2f < 0.0f ? 0.0f : amount * 1.2f) : 1.0f;
    state->eye.x = CameraTransitionValue(state->previousEye.x, state->eye.x, amount);
    state->eye.y = CameraTransitionValue(state->previousEye.y, state->eye.y, amount);
    state->eye.z = CameraTransitionValue(state->previousEye.z, state->eye.z, amount);
    state->target.x = CameraTransitionValue(state->previousTarget.x, state->target.x, targetAmount);
    state->target.y = CameraTransitionValue(state->previousTarget.y, state->target.y, targetAmount);
    state->target.z = CameraTransitionValue(state->previousTarget.z, state->target.z, targetAmount);
    state->fieldOfView = static_cast<std::uint16_t>(
        CameraTransitionAngle(static_cast<std::int16_t>(state->previousFieldOfView), static_cast<std::int16_t>(state->fieldOfView), amount));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void ApplyConversationLookTurn(ConversationCameraState *state) {
    std::int32_t turnFrames = 60;
    if (state->currentShotFlags & 8U)
        state->fieldOfView = static_cast<std::uint16_t>(camCtrlGetFov());
    if (state->flags & 0x4000U)
        if (!(state->flags & 0x8000U))
            if (state->currentShotFlags & 8U)
                state->flags |= 0x8000U;
    if (!(state->currentShotFlags & 8U))
        return;
    if (!(state->flags & 0x8000U))
        return;
    if (++state->lookDelayFrames > 9) {
        float ease = static_cast<float>(++state->lookTurnFrames) / static_cast<float>(turnFrames);
        ease = (ease < 0.0f ? 0.0f : ease) < 1.0f ? (ease < 0.0f ? 0.0f : ease) : 1.0f;
        ease = CosineEaseValue(0.0f, 1.0f, ease);
        Vector3 view;
        std::int16_t lookRotation[2], rotation[2];
        am2Vector3Subtract(&view, &state->target, &state->eye);
        Vector3_ToFixed16EulerAngles(view, *reinterpret_cast<Vector3s *>(rotation));
        const float distance = am2Vector3Length(&view);
        am2Vector3Subtract(&view, &state->lookLocation, &state->eye);
        Vector3_ToFixed16EulerAngles(view, *reinterpret_cast<Vector3s *>(lookRotation));
        rotation[0] = CameraTransitionAngle(rotation[0], lookRotation[0], ease);
        rotation[1] = CameraTransitionAngle(rotation[1], lookRotation[1], ease);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&state->eye);
        Matrix4x4::RotateAroundY_Wrapper(rotation[1]);
        Matrix4x4::RotateAroundX_Wrapper(rotation[0]);
        Matrix4x4::preTranslate(0.0f, 0.0f, -distance);
        matrix_stack::GetTranslation(&state->target);
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void ApplyConversationSpecialModeOverride(ConversationCameraState *state) {
    std::int32_t first = 0;
    if (plymIsSpecialModeCameraRequested()) {
        if (!(state->flags & 0x1000U) && !state->overrideElapsed) {
            state->flags |= 0x1000U;
            first = 1;
        }
    } else
        state->flags &= ~0x1000U;
    if (state->overrideElapsed > 75)
        state->flags &= ~0x1000U;
    if (!(state->flags & 0x1000U))
        return;
    ++state->overrideElapsed;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(mtwkGetBoneTransform(charFindMotionwork(state->root), 14));
    Matrix4x4::preTranslate(0.17f, 0.05f, -0.03f);
    matrix_stack::GetTranslation(&state->target);
    Matrix4x4::SetViewMatrixFromEuler(16384, 25486, 16384);
    Matrix4x4::preTranslate(0.0f, 0.0f, 1.0f);
    Vector3 facing;
    matrix_stack::GetTranslation(&facing);
    matrix_stack::UnitCurrentMatrix();
    matrix_stack::TranslateVector(&state->target);
    Matrix4x4::RotateAroundY_Wrapper(CopyHeading(state->target, facing));
    Matrix4x4::RotateAroundX_Wrapper(0x2e38);
    Matrix4x4::preTranslate(0.0f, 0.0f, -0.5f);
    matrix_stack::GetTranslation(&state->eye);
    PopCurrentMatrix(1);
    if (!first) {
        state->eye.x = InterpolateTo(state->eye.x, state->framePreviousEye.x, 0.05f);
        state->eye.y = InterpolateTo(state->eye.y, state->framePreviousEye.y, 0.05f);
        state->eye.z = InterpolateTo(state->eye.z, state->framePreviousEye.z, 0.05f);
        state->target.x = InterpolateTo(state->target.x, state->framePreviousTarget.x, 0.05f);
        state->target.y = InterpolateTo(state->target.y, state->framePreviousTarget.y, 0.05f);
        state->target.z = InterpolateTo(state->target.z, state->framePreviousTarget.z, 0.05f);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static float ConversationParameterFloat(const ConversationCameraDescriptor &descriptor, std::size_t offset) {
    float value;
    __builtin_memcpy(&value, static_cast<const std::byte *>(descriptor.parameters) + offset, sizeof(value));
    return value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int16_t ConversationParameterAngle(const ConversationCameraDescriptor &descriptor, std::size_t offset) {
    std::int16_t value;
    __builtin_memcpy(&value, static_cast<const std::byte *>(descriptor.parameters) + offset, sizeof(value));
    return value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 TransformConversationPoint(const Vector3 &point, const Vector3 &origin, std::int16_t yaw, std::int16_t pitch) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::RotateAroundX_Wrapper(pitch);
    Vector3 result;
    Matrix4x4::TransformPoint(point, result);
    PopCurrentMatrix(1);
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void BindConversationShotActors(ConversationCameraState *state) {
    const ConversationDialogueRecord *const shot = state->shots[state->shotIndex];
    state->packetSpeaker = shot->speaker;
    state->packetListener = shot->listener;
    state->targetActor = state->targetAnchors[state->shotIndex];
    state->eyeActor = state->eyeAnchors[state->shotIndex];
    if (state->eyeActor == state->root)
        state->specialTarget = state->targetActor;
    else
        state->specialTarget = state->eyeActor;
    state->specialTargetName = ctrlGetScenarioCharaName(state->specialTarget);
    state->targetScenarioName = ctrlGetScenarioCharaName(state->targetActor);
    state->eyeScenarioName = ctrlGetScenarioCharaName(state->eyeActor);
    if (state->targetActor == state->eyeActor || !IsCharacterInCurrentMap(state->targetActor) || !IsCharacterInCurrentMap(state->eyeActor))
        state->flags |= 0x10U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void AssignConversationShotAnchors(ConversationCameraState *state) {
    for (std::int32_t shot = 0; shot < state->shotCount; ++shot) {
        Character *speaker = state->shots[shot]->speaker, *listener = state->shots[shot]->listener;
        if (speaker != state->root && listener != state->root)
            listener = state->root;
        Character *partner;
        if (speaker == state->root)
            partner = listener;
        else
            partner = speaker;
        Character *target = speaker, *eye = listener;
        if (!shot && (state->flags & 4U))
            if (!(state->flags & 8U)) {
                target = partner;
                eye = state->root;
            }
        if (state->flags & 0x20000000U) {
            target = speaker;
            eye = listener;
        }
        state->targetAnchors[shot] = target;
        state->eyeAnchors[shot] = eye;
        state->partners[shot] = partner;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void ReadConversationCameraPackets(ConversationCameraState *state) {
    std::int32_t shotCount = 0, memberCount = 0;
    std::int32_t totalFrames = 0;
    const ConversationDialogueRecord *input = state->preparedStream;
    const ConversationDialogueRecord *next = NextConversationRecord(input);
    const ConversationDialogueRecord *packet = input;
    while ((reinterpret_cast<const std::int8_t *>(&input->command)[3] & 1) == 0) {
        if (input->speaker) {
            std::int32_t index;
            for (index = 0; index < memberCount && state->members[index] != input->speaker; ++index) {
            }
            if (index == memberCount)
                state->members[memberCount++] = input->speaker;
        }
        if (input->listener) {
            std::int32_t index;
            for (index = 0; index < memberCount && state->members[index] != input->listener; ++index) {
            }
            if (index == memberCount)
                state->members[memberCount++] = input->listener;
        }
        if (ConversationRecordFlags(packet) & 0x40U)
            state->shotFlags[shotCount] |= 1U;
        if (ConversationRecordFlags(packet) & 2U) {
            state->shotFlags[shotCount] |= 8U;
            state->flags |= 0x80000U;
        }
        if (ConversationRecordFlags(packet) & 8U) {
            state->shotFlags[shotCount] |= 0x40U;
            state->flags |= 0x800000U;
        }
        state->shots[shotCount++] = input;
        totalFrames += static_cast<std::int16_t>(static_cast<std::int32_t>(input->duration * 30.0f));
        input = next;
        next = NextConversationRecord(input);
        packet = input;
    }
    state->shotCount = static_cast<std::int8_t>(shotCount);
    state->memberCount = static_cast<std::int8_t>(memberCount);
    state->totalFrames = static_cast<std::int16_t>(totalFrames);
    if (shotCount > 0)
        state->shotFlags[shotCount - 1] |= 0x10U;
    if (state->memberCount > 2) {
        for (std::int32_t shot = 0; shot < state->shotCount; ++shot) {
            input = state->shots[shot];
            for (std::int32_t member = 0; member < state->memberCount; ++member) {
                if (state->memberSeen[member] != 1 && (state->members[member] == input->speaker || state->members[member] == input->listener)) {
                    state->memberSeen[member] = 1;
                    state->shotFlags[shot] |= 2U;
                }
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void CompressConversationCameraStream(ConversationCameraState *state) {
    std::int32_t processed;
    std::int32_t mergeRemainder = 0;
    std::int32_t retainFirst;
    std::int32_t merged = 0;
    const ConversationDialogueRecord *input = state->preparedStream;
    std::byte *output = reinterpret_cast<std::byte *>(conversationPacketScratch);
    retainFirst = 0;
    processed = 0;
    ConversationDialogueRecord *previous = 0;
    if ((ConversationRecordFlags(input) & 1) == 0) {
        do {
            std::int32_t shouldMerge = 0;
            ConversationDialogueRecord *const prev = previous;
            const float duration = input->duration;
            previous = reinterpret_cast<ConversationDialogueRecord *>(output);
            if (!processed) {
                std::memcpy(output, input, ConversationRecordBytes(input));
                if (duration <= 2.1f)
                    retainFirst = 1;
                output += ConversationRecordBytes(input);
                merged = 0;
            } else if (retainFirst) {
                output = reinterpret_cast<std::byte *>(conversationPacketScratch);
                std::memcpy(output, input, ConversationRecordBytes(input));
                const ConversationDialogueRecord *const first = state->preparedStream;
                previous = reinterpret_cast<ConversationDialogueRecord *>(output);
                reinterpret_cast<ConversationDialogueRecord *>(conversationPacketScratch)->duration += first->duration;
                reinterpret_cast<std::uint8_t *>(conversationPacketScratch)[3] = static_cast<std::uint8_t>(
                    reinterpret_cast<std::uint8_t *>(conversationPacketScratch)[3] | reinterpret_cast<const std::uint8_t *>(first)[3] | 0x40U);
                output += ConversationRecordBytes(input);
                retainFirst = 0;
                ++merged;
            } else {
                if (duration <= 2.1f)
                    shouldMerge = 1;
                if (merged > 1 || prev->duration > 6.3f)
                    shouldMerge = 0;
                if (mergeRemainder)
                    shouldMerge = 1;
                if (shouldMerge) {
                    previous = prev;
                    previous->duration += duration;
                    reinterpret_cast<std::uint8_t *>(prev)[3] =
                        static_cast<std::uint8_t>(reinterpret_cast<std::uint8_t *>(prev)[3] | reinterpret_cast<const std::uint8_t *>(input)[3] | 0x40U);
                    ++merged;
                } else {
                    std::memcpy(output, input, ConversationRecordBytes(input));
                    output += ConversationRecordBytes(input);
                    merged = 0;
                }
            }
            if (ConversationRecordFlags(input) & 2)
                mergeRemainder = 1;
            input = NextConversationRecord(input);
            ++processed;
        } while ((ConversationRecordFlags(input) & 1) == 0);
    }
    std::memcpy(output, input, ConversationRecordBytes(input));
    state->preparedStream = reinterpret_cast<const ConversationDialogueRecord *>(conversationPacketScratch);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void CaptureConversationParticipantAlpha(ConversationCameraState *state) {
    std::int32_t index;
    for (index = 0; index < state->memberCount; ++index) {
        Character *character = state->members[index];
        ConversationParticipantAlpha *const saved = &state->alphaParticipants[index];
        saved->name = ctrlGetScenarioCharaName(character);
        const std::uint32_t compatibility = (GetCharacterMotionCompatibilityResult(character) & 0x3fU) << 1;
        saved->flags = (saved->flags & 0xffffff81U) | compatibility;
        const std::uint32_t alphaManager = (static_cast<std::uint32_t>(shenCheckCharaHasAlphaManager(character)) & 1U) << 7;
        do {
            saved->flags = (saved->flags & ~0x80U) | alphaManager;
        } while (0);
        SetCharacterMotionClipMode(character, 0);
    }
    state->alphaParticipants[index].name = shen_name(0xffffffffU);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void RestoreConversationParticipantAlpha(ConversationCameraState *state) {
    for (ConversationParticipantAlpha *saved = state->alphaParticipants; saved->name != 0xffffffffU; ++saved) {
        Character *character = ctrlGetScenarioChara(saved->name);
        if (!character || !IsCharacterInCurrentMap(character))
            continue;
        SetCharacterMotionClipMode(character, static_cast<std::uint8_t>((saved->flags >> 1) & 0x3fU));
        MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (control->ryo == character)
            character->alpha = 1.0f;
        else
            cpamSetFlag(character, 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t ConversationAbsAngle(std::int32_t angle) {
    return angle < 0 ? -angle : angle;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void UpdateConversationAngleClass(ConversationCameraState *state) {
    state->orientationClass = 0;
    if (!(state->flags & 0x100000U) || state->root == state->specialTarget)
        return;
    Vector3 origin, partner, difference, delta, local;
    GetCharacterWorldPosition(state->root, &origin);
    GetCharacterWorldPosition(state->specialTarget, &partner);
    am2Vector3Subtract(&difference, &partner, &origin);
    const std::int16_t axis = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(difference.x, difference.z)) - 0x8000);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-axis);
    matrix_stack::TranslateInverseVector(&origin);
    Matrix4x4::TransformPoint(state->lookLocation, local);
    Matrix4x4::TransformPoint(partner, partner);
    PopCurrentMatrix(1);
    std::int32_t kind;
    std::int16_t angle;
    if (local.z < partner.z) {
        am2Vector3Subtract(&delta, &local, &partner);
        angle = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(delta.x, delta.z)) - 0x8000);
        if ((angle < 0 ? -angle : angle) < 0x1555)
            kind = 1;
        else if ((angle < 0 ? -angle : angle) < 0x2aaa)
            kind = 2;
        else
            kind = 3;
    } else if (local.z < 0.0f)
        kind = 4;
    else {
        delta = local;
        angle = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(delta.x, delta.z)) - 0x8000);
        angle = static_cast<std::int16_t>(angle - 0x8000);
        if ((angle < 0 ? -angle : angle) < 0x1555)
            kind = 7;
        else if ((angle < 0 ? -angle : angle) < 0x2aaa)
            kind = 6;
        else
            kind = 5;
    }
    if (local.x < 0.0f)
        kind = -kind;
    state->orientationClass = static_cast<std::int8_t>(kind);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void GenerateConversationTypeOne(ConversationCameraState *state) {
    const float amount = state->shotProgress;
    Vector3 *const eye = &state->eye, *const target = &state->target;
    const ConversationCameraDescriptor *const descriptor = &conversationCameraDescriptors[state->candidateIndex];
    if (descriptor->flags & 1U) {
        const float *const positions = static_cast<const float *>(descriptor->parameters);
        float signX = 1.0f;
        if (state->mirrorFlags & 1U)
            signX = -1.0f;
        float signZ = 1.0f;
        if (state->mirrorFlags & 2U)
            signZ = -1.0f;
        float values[12];
        values[0] = positions[1] * signX;
        values[1] = positions[2];
        values[2] = positions[3] * signZ;
        values[3] = positions[4] * signX;
        values[4] = positions[5];
        values[5] = positions[6] * signZ;
        values[6] = positions[7] * signX;
        values[7] = positions[8];
        values[8] = positions[9] * signZ;
        values[9] = positions[10] * signX;
        values[10] = positions[11];
        values[11] = positions[12] * signZ;
        eye->x = CameraTransitionValue(values[0], values[6], amount);
        eye->y = CameraTransitionValue(values[1], values[7], amount);
        eye->z = CameraTransitionValue(values[2], values[8], amount);
        target->x = CameraTransitionValue(values[3], values[9], amount);
        target->y = CameraTransitionValue(values[4], values[10], amount);
        target->z = CameraTransitionValue(values[5], values[11], amount);
        state->fieldOfView = static_cast<std::int32_t>(
            CameraTransitionValue(reinterpret_cast<const std::int16_t *>(positions)[0], reinterpret_cast<const std::int16_t *>(positions)[1], amount));
        if (!(descriptor->flags & 0x80U) && !(state->flags & 0x200000U)) {
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&state->actorTarget);
            Matrix4x4::RotateAroundY_Wrapper(state->actorDirection.y);
            Matrix4x4::RotateAroundX_Wrapper(state->actorDirection.x);
            Matrix4x4::TransformPoint(*target, *target);
            Matrix4x4::TransformPoint(*eye, *eye);
            PopCurrentMatrix(1);
        } else {
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&state->actorTarget);
            Matrix4x4::RotateAroundY_Wrapper(state->actorYaw);
            Matrix4x4::TransformPoint(*target, *target);
            Matrix4x4::TransformPoint(*eye, *eye);
            PopCurrentMatrix(1);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void GenerateConversationTypeTwo(ConversationCameraState *state) {
    struct ConversationTypeTwoTable {
        std::int16_t startFov;
        std::int16_t endFov;
        ConversationOffset offsets[4];
    };
    const float progress = state->shotProgress;
    Vector3 *const eye = &state->eye;
    Vector3 *const target = &state->target;
    const ConversationCameraDescriptor *const descriptor = &conversationCameraDescriptors[state->candidateIndex];
    if (!(descriptor->flags & 2U))
        return;
    const ConversationTypeTwoTable *const nearTable = reinterpret_cast<const ConversationTypeTwoTable *>(descriptor->parameters);
    const ConversationTypeTwoTable *const farTable = nearTable + 1;
    float sign = 1.0f;
    if (state->mirrorFlags & 1U)
        sign = -1.0f;
    float distanceBlend;
    if (descriptor->flags & 0x200000U)
        distanceBlend = (0.9f - state->actorDistance) / 0.29999995f;
    else
        distanceBlend = (3.0f - state->actorDistance) / 2.0999999f;
    float heightDelta = state->actorHeightDelta;
    if (heightDelta < 0.0f)
        heightDelta = 0.0f;
    float heightScale = 0.0f;
    ConversationOffset eyeOffset, targetOffset;
    ConversationOffset points[4];
    const ConversationOffset *nearOffset = nearTable->offsets;
    points[0].distance = CameraTransitionValue(farTable->offsets[0].distance, nearOffset->distance, distanceBlend);
    points[0].height = CameraTransitionValue(farTable->offsets[0].height, nearOffset->height, distanceBlend) + points[0].distance * heightScale;
    points[0].angle = static_cast<std::int32_t>(static_cast<float>(CameraTransitionAngle(farTable->offsets[0].angle, nearOffset->angle, distanceBlend)) * sign);
    ++nearOffset;
    points[1].distance = CameraTransitionValue(farTable->offsets[1].distance, nearOffset->distance, distanceBlend);
    points[1].height = CameraTransitionValue(farTable->offsets[1].height, nearOffset->height, distanceBlend) + points[1].distance * heightScale;
    points[1].angle = static_cast<std::int32_t>(static_cast<float>(CameraTransitionAngle(farTable->offsets[1].angle, nearOffset->angle, distanceBlend)) * sign);
    ++nearOffset;
    points[2].distance = CameraTransitionValue(farTable->offsets[2].distance, nearOffset->distance, distanceBlend);
    points[2].height = CameraTransitionValue(farTable->offsets[2].height, nearOffset->height, distanceBlend) + points[2].distance * heightScale;
    points[2].angle = static_cast<std::int32_t>(static_cast<float>(CameraTransitionAngle(farTable->offsets[2].angle, nearOffset->angle, distanceBlend)) * sign);
    ++nearOffset;
    points[3].distance = CameraTransitionValue(farTable->offsets[3].distance, nearOffset->distance, distanceBlend);
    points[3].height = CameraTransitionValue(farTable->offsets[3].height, nearOffset->height, distanceBlend) + points[3].distance * heightScale;
    points[3].angle = static_cast<std::int32_t>(static_cast<float>(CameraTransitionAngle(farTable->offsets[3].angle, nearOffset->angle, distanceBlend)) * sign);
    eyeOffset.distance = CameraTransitionValue(points[0].distance, points[2].distance, progress);
    eyeOffset.height = CameraTransitionValue(points[0].height, points[2].height, progress);
    eyeOffset.angle = CameraTransitionAngle(points[0].angle, points[2].angle, progress);
    targetOffset.distance = CameraTransitionValue(points[1].distance, points[3].distance, progress);
    targetOffset.height = CameraTransitionValue(points[1].height, points[3].height, progress);
    targetOffset.angle = CameraTransitionAngle(points[1].angle, points[3].angle, progress);
    RotateConversationOffset(eyeOffset, *eye);
    RotateConversationOffset(targetOffset, *target);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&state->actorTarget);
    Matrix4x4::RotateAroundY_Wrapper(state->actorDirection.y);
    Matrix4x4::RotateAroundX_Wrapper(state->actorDirection.x);
    Matrix4x4::TransformPoint(*target, *target);
    PopCurrentMatrix(1);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&state->actorEye);
    Matrix4x4::RotateAroundY_Wrapper(state->actorDirection.y + 0x8000);
    Matrix4x4::RotateAroundX_Wrapper(-state->actorDirection.x);
    Matrix4x4::TransformPoint(*eye, *eye);
    PopCurrentMatrix(1);
    state->fieldOfView = static_cast<std::uint16_t>(CameraTransitionAngle(nearTable->startFov, nearTable->endFov, progress));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void GenerateConversationTypeEight(ConversationCameraState *state) {
    Vector3 *const eye = &state->eye;
    Vector3 *const target = &state->target;
    Vector3 centre, difference;
    std::int16_t angle;
    do {
        am2Vector3Add(&centre, &state->actorTarget, &state->actorEye);
        am2Vector3Scale(&centre, &centre, 0.5f);
        am2Vector3Subtract(&difference, &state->actorTarget, &state->actorEye);
        am2Vector3Length(&difference);
    } while (0);
    *target = centre;
    switch (static_cast<std::int8_t>(state->candidateIndex % 3)) {
    case 0:
    default:
        angle = 0;
        break;
    case 1:
        angle = 0x2aaa;
        break;
    case 2:
        angle = static_cast<std::int16_t>(0xd556);
        break;
    }
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&centre);
    Matrix4x4::RotateAroundY_Wrapper(state->actorYaw);
    Matrix4x4::RotateAroundY_Wrapper(angle);
    Matrix4x4::RotateAroundY_Wrapper(0xfc72);
    Matrix4x4::preTranslate(0.0f, 0.0f, -3.0f);
    matrix_stack::GetTranslation(eye);
    PopCurrentMatrix(1);
    state->fieldOfView = static_cast<std::uint16_t>(camCtrlGetFov());
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void GenerateConversationFixedCandidate(ConversationCameraState *state) {
    Vector3 sum;
    am2Vector3Add(&sum, &state->actorTarget, &state->actorEye);
    am2Vector3Scale(&state->target, &sum, 0.5f);
    state->eye = state->fixedPosition;
    state->fieldOfView = static_cast<std::uint16_t>(camCtrlGetFov());
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GenerateConversationCandidate(ConversationCameraState *state) {
    if (state->flags & 0x8000000U) {
        GenerateConversationFixedCandidate(state);
        return;
    }
    switch (static_cast<std::int32_t>((state->candidateIndex + conversationCameraDescriptors)->flags & 0xfU)) {
    case 1:
        GenerateConversationTypeOne(state);
        break;
    case 2:
        GenerateConversationTypeTwo(state);
        break;
    case 8:
        GenerateConversationTypeEight(state);
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void SampleConversationCandidate(Vector3 &eye, Vector3 &target, std::int32_t candidate, float progress) {
    Vector3 previousEye, previousTarget;
    std::int16_t previousFov;
    float previousProgress;
    std::int8_t previousType, previousCandidate;
    ConversationCameraState *state = g_conversationCameraState;
    previousType = state->shotType;
    previousCandidate = state->candidateIndex;
    previousEye = state->eye;
    previousTarget = state->target;
    previousFov = state->fieldOfView;
    previousProgress = state->shotProgress;
    state->shotType = state->candidates[candidate].group;
    state->candidateIndex = static_cast<std::int8_t>(candidate);
    state->shotProgress = progress;
    GenerateConversationCandidate(state);
    eye = state->eye;
    target = state->target;
    state->shotType = previousType;
    state->candidateIndex = previousCandidate;
    state->eye = previousEye;
    state->target = previousTarget;
    state->fieldOfView = previousFov;
    state->shotProgress = previousProgress;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void SelectConversationCameraSide(ConversationCameraState *state) {
    if (state->sceneFlags & 8U) {
        const std::int32_t answer = drGetAnswerSide(0);
        if (answer)
            switch (answer) {
            case 1:
            case 2:
            case 3:
                state->side = 0;
                return;
            case 4:
                state->side = 1;
                return;
            case 5:
            case 6:
                break;
            }
    }
    if (state->flags & 0x80000U) {
        if (state->orientationClass >= 0)
            state->side = 1;
        else
            state->side = 0;
        return;
    }
    if (!(state->flags & 4U))
        return;
    if (state->sceneFlags & 0x40U) {
        state->side = 1;
        return;
    }
    state->side = 0;
    if (state->memberCount <= 1)
        return;
    Vector3 root, partner, point, local, direction;
    Vector2s angles;
    std::int32_t positiveHits, negativeHits, depth, side;
    charaGetMotWorldPos(state->root, 5, &root);
    charaGetMotWorldPos(state->specialTarget, 5, &partner);
    am2Vector3Subtract(&direction, &partner, &root);
    Vector3_ToFixed16EulerAngles(direction, reinterpret_cast<Vector3s &>(angles));
    float spacing = am2Vector3Length(&direction);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&root);
    Matrix4x4::RotateAroundY_Wrapper(angles.y);
    Matrix4x4::RotateAroundX_Wrapper(angles.x);
    positiveHits = 0;
    negativeHits = 0;
    spacing /= 11.0f;
    for (depth = 0; depth < 15; ++depth) {
        for (side = 0; side < 10; ++side) {
            local.x = static_cast<float>(side) * 0.2f;
            local.y = 0.0f;
            local.z = -spacing * static_cast<float>(depth - 2);
            Matrix4x4::TransformPoint(local, point);
            if (ConversationCollisionSegment(root, point, 0.1f))
                ++positiveHits;
            local.x = static_cast<float>(side) * -0.2f;
            local.y = 0.0f;
            local.z = -spacing * static_cast<float>(depth - 2);
            Matrix4x4::TransformPoint(local, point);
            if (ConversationCollisionSegment(root, point, 0.1f))
                ++negativeHits;
        }
    }
    PopCurrentMatrix(1);
    if ((positiveHits - negativeHits < 0 ? negativeHits - positiveHits : positiveHits - negativeHits) > 30) {
        if (positiveHits >= negativeHits)
            state->side = 1;
        else
            state->side = 0;
        return;
    }
    GetCharacterWorldPosition(state->root, &root);
    GetCharacterWorldPosition(state->specialTarget, &partner);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-shenGetRotYFromNode(state->root));
    matrix_stack::TranslateInverseVector(&root);
    Matrix4x4::TransformPoint(partner, partner);
    PopCurrentMatrix(1);
    if (partner.x < 0.0f)
        state->side = 1;
    else
        state->side = 0;
    if ((partner.x < 0.0f ? -partner.x : partner.x) < 0.001f)
        state->side = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateConversationMirrorFlags(ConversationCameraState *state) {
    std::int32_t mirror = 0;
    if (state->flags & 0x20000000U)
        return;
    const ConversationShotHistory *const previous = state->previousHistory;
    if (previous && ((previous->candidate + conversationCameraDescriptors)->flags & 0x100U))
        state->side ^= 1U;
    if (state->flags & 0x10000U) {
        Vector3 origin, partner, local, delta;
        std::int16_t angles[2];
        GetCharacterWorldPosition(state->root, &origin);
        GetCharacterWorldPosition(state->specialTarget, &partner);
        am2Vector3Subtract(&delta, &partner, &origin);
        Vector3_ToFixed16EulerAngles(delta, *reinterpret_cast<Vector3s *>(angles));
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-angles[1]);
        matrix_stack::TranslateInverseVector(&origin);
        Matrix4x4::TransformPoint(state->previousEye, local);
        PopCurrentMatrix(1);
        if (local.x < 0.0f)
            state->side = 1;
        else
            state->side = 0;
        if ((local.x < 0.0f ? -local.x : local.x) < 0.001f)
            state->side = 0;
    }
    if (state->eyeScenarioName == state->rootScenarioName) {
        if (state->side)
            mirror |= 1;
        else
            mirror &= ~1;
    } else if (!state->side)
        mirror |= 1;
    else
        mirror &= ~1;
    state->mirrorFlags = mirror;
}
#endif

// @verified - d
void ResetConversationCandidates(ConversationCameraState *state) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t groupZero = 0;
    std::int32_t groupOne = 0;
    ConversationCandidateState *candidate = state->candidates;
    ConversationCameraDescriptor *descriptor = conversationCameraDescriptors;
    std::int32_t index = 0;
    for (; !(descriptor->flags & 0x80000000U); ++candidate) {
        candidate->flags = 0;
        candidate->collisionClass = 0;
        if (descriptor->flags & 0x40000000U) {
            candidate->group = static_cast<std::uint8_t>(-1);
            candidate->groupIndex = 0;
            if (!(state->flags & 0x20000000U))
                descriptor->flags &= ~3U;
        }
        if (descriptor->flags & 1U) {
            candidate->group = 0;
            candidate->groupIndex = static_cast<std::uint8_t>(groupZero++);
        } else if (descriptor->flags & 10U) {
            candidate->group = 1;
            candidate->groupIndex = static_cast<std::uint8_t>(groupOne++);
        }
        ++index;
        if (index >= 96)
            break;
        ++descriptor;
    }
    state->candidateCount = static_cast<std::int8_t>(index);
#else
    std::uint8_t groupZero = 0, groupOne = 0;
    std::int32_t index = 0;
    while (index < 96 && !(conversationCameraDescriptors[index].flags & 0x80000000U)) {
        ConversationCandidateState &candidate = state->candidates[index];
        ConversationCameraDescriptor &descriptor = conversationCameraDescriptors[index];
        candidate.flags = 0;
        candidate.collisionClass = 0;
        if (descriptor.flags & 0x40000000U) {
            candidate.group = 0xff;
            candidate.groupIndex = 0;
            if (!(state->flags & 0x20000000U))
                descriptor.flags &= ~3U;
        }
        if (descriptor.flags & 1U) {
            candidate.group = 0;
            candidate.groupIndex = groupZero++;
        } else if (descriptor.flags & 10U) {
            candidate.group = 1;
            candidate.groupIndex = groupOne++;
        }
        ++index;
    }
    state->candidateCount = static_cast<std::uint8_t>(index);
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FilterConversationCandidates(ConversationCameraState *state) {
    if ((state->flags & 0x400U) && state->shotType == 1)
        state->flags |= 0x8000000U;
    if ((state->flags & 0x400000U) && state->shotType == 1) {
        for (std::int32_t index = 0; index < state->candidateCount; ++index) {
            if (state->candidates[index].group != 1)
                state->candidates[index].flags = 1;
            if (!(conversationCameraDescriptors[index].flags & 8U))
                state->candidates[index].flags = 1;
        }
        return;
    }
    for (std::int32_t index = 0; index < state->candidateCount; ++index)
        if (conversationCameraDescriptors[index].flags & 8U)
            state->candidates[index].flags = 1;
    FilterConversationSceneCandidates(state);
    CheckConversationCandidateCollisions(state);
    RetireRecentConversationCandidates(state);
    MarkConversationEdgeCandidates(state);
    RestrictConversationCandidateAngles(state);
    ReadmitConversationCandidates(state);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void RestrictConversationCandidateAngles(ConversationCameraState *state) {
    std::uint32_t admitted = 0;
    std::uint32_t mask = 0;
    if ((state->flags & 0x40000U) && state->actorHeightDelta > 0.0f && state->shotType == 1)
        mask |= 0x40000U;
    ConversationCandidateState *candidate = state->candidates;
    const ConversationCameraDescriptor *descriptor = conversationCameraDescriptors;
    for (std::int32_t index = 0; index < state->candidateCount; ++index, ++candidate, ++descriptor)
        if ((candidate->flags & 1) == 0)
            admitted |= descriptor->flags & mask;
    if (!admitted)
        return;
    if (!(admitted & 0x7000000U))
        return;
    const std::uint32_t angleMask = admitted & 0x7000000U;
    candidate = state->candidates;
    descriptor = conversationCameraDescriptors;
    for (std::int32_t index = 0; index < state->candidateCount; ++index, ++candidate, ++descriptor) {
        if ((descriptor->flags & 0x7000000U) != angleMask)
            candidate->flags |= 1U;
        else
            candidate->flags = static_cast<std::uint8_t>(static_cast<std::int8_t>(candidate->flags) & ~1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ReadmitConversationCandidates(ConversationCameraState *state) {
    std::int32_t index;
    std::int32_t count = 0;
    ConversationCandidateState *candidate = state->candidates;
    for (index = 0; index < state->candidateCount; ++index, ++candidate) {
        if ((candidate->flags & 1) == 0) {
            ++count;
            break;
        }
    }
    if (count > 0)
        return;
    count = 0;
    candidate = state->candidates;
    for (index = 0; index < state->candidateCount; ++index, ++candidate) {
        if (candidate->flags & 0x12)
            continue;
        if (candidate->flags & 4) {
            candidate->flags = static_cast<std::uint8_t>(static_cast<std::int8_t>(candidate->flags) & ~1);
            ++count;
        }
    }
    if (count > 0)
        return;
    if (state->flags & 0x40000U)
        state->candidates[13].flags &= ~1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SelectConversationCandidate(ConversationCameraState *state) {
    std::int8_t available[128];
    std::int32_t chosen = 0;
    std::uint32_t count = 0;
    const ConversationCandidateState *candidate = state->candidates;
    std::int32_t index;
    for (index = 0; index < state->candidateCount; ++index, ++candidate)
        if ((candidate->flags & 1U) == 0)
            available[count++] = static_cast<std::int8_t>(index);
    ++state->groupSelections[state->shotType];
    const std::uint8_t selection = state->groupSelections[state->shotType];
    switch (state->shotType) {
    case 0:
        if (count == 0) {
            const std::int32_t parity = selection & 1;
            candidate = state->candidates;
            for (index = 0; index < state->candidateCount; ++index, ++candidate)
                if (static_cast<std::int8_t>(candidate->group) == state->shotType && static_cast<std::int8_t>(candidate->groupIndex) == parity)
                    chosen = index;
        } else
            chosen = available[selection % count];
        break;
    case 1:
    case 3:
        if (count == 0) {
            const std::int32_t parity = selection & 1;
            candidate = state->candidates;
            for (index = 0; index < state->candidateCount; ++index, ++candidate)
                if (static_cast<std::int8_t>(candidate->group) == state->shotType && static_cast<std::int8_t>(candidate->groupIndex) == parity)
                    chosen = index;
        } else
            chosen = available[selection % count];
        break;
    default:
        chosen = 0;
        break;
    }
    if ((state->flags & 0x10000U) && state->previousHistory && state->shotType == state->previousHistory->shotType)
        chosen = state->previousHistory->candidate;
    state->candidateIndex = static_cast<std::int8_t>(chosen);
    if (state->forcedCandidate != -1) {
        state->candidateIndex = state->forcedCandidate;
        state->shotType = state->candidates[state->candidateIndex].group;
        state->shotTypes[state->shotIndex] = state->shotType;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline const std::int8_t *ConversationOrientationGroups() {
    static const std::int8_t groups[8] = {0, 1, 2, 3, 3, 3, 2, 1};
    return groups;
}

static void FilterConversationSceneCandidates(ConversationCameraState *state) {
    std::uint32_t disallowed = 0;
    std::uint32_t anyRequired = 0;
    std::uint32_t allRequired = 0;
    std::uint32_t angleMask = 0;
    switch (state->shotType) {
    case 0:
        allRequired |= 1U;
        break;
    case 1:
        allRequired |= 2U;
        break;
    }
    if (!(state->flags & 0x40000U)) {
        disallowed |= 0x40000U;
    } else {
        disallowed |= 0x2000U;
        if (state->shotType == 1 && state->targetActor == state->root && state->eyeActor != state->targetActor)
            disallowed |= 0xc00U;
    }
    if (state->currentShotFlags & 8U) {
        if (state->shotType == 1 && state->orientationClass) {
            std::int32_t orientation = state->orientationClass;
            if (orientation < 0)
                orientation = -orientation;
            angleMask = static_cast<std::uint32_t>(ConversationOrientationGroups()[orientation]);
            angleMask = (angleMask & 7U) << 24;
        }
    } else {
        disallowed |= 0x7000000U;
    }
    if (plymHasRyoBag(0)) {
        disallowed |= 0x4000U;
        if (state->shotType == 1 && state->targetActor != state->root && state->eyeActor == state->root)
            disallowed |= 0x400U;
    }
    if ((state->sceneFlags & 0x10U) && state->shotType == 0)
        disallowed |= 0xc00U;
    disallowed |= 0x100000U;
    if (state->actorDistance < 0.9f)
        disallowed |= 0x8000U;
    else
        disallowed |= 0x200000U;
    if (state->sceneFlags & 0x80U) {
        if (state->shotType == 1)
            allRequired |= 0x400000U;
    } else {
        disallowed |= 0x400000U;
    }
    if (state->sceneFlags & 0x40U) {
        if (state->shotType == 1)
            allRequired |= 0x800000U;
    } else {
        disallowed |= 0x800000U;
    }
    ConversationCandidateState *candidate = state->candidates;
    const ConversationCameraDescriptor *descriptor = conversationCameraDescriptors;
    for (std::int32_t index = 0; index < state->candidateCount; ++index, ++candidate, ++descriptor) {
        if (descriptor->flags & disallowed)
            candidate->flags |= 0x11U;
        if (anyRequired && !(descriptor->flags & anyRequired))
            candidate->flags |= 0x11U;
        if (allRequired && (descriptor->flags & allRequired) != allRequired)
            candidate->flags |= 0x11U;
        if (angleMask && (descriptor->flags & 0x7000000U) != angleMask)
            candidate->flags |= 0x11U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void RetireRecentConversationCandidates(ConversationCameraState *state) {
    for (std::int32_t index = 0; index < 4; ++index) {
        const ConversationShotHistory *const history = reinterpret_cast<const ConversationShotHistory *>(
            reinterpret_cast<const std::byte *>(state) + (index * sizeof(ConversationShotHistory) + offsetof(ConversationCameraState, history)));
        if (history->candidate != -1)
            state->candidates[history->candidate].flags |= 5U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void MarkConversationEdgeCandidates(ConversationCameraState *state) {
    ConversationCandidateState *candidate = state->candidates;
    const ConversationCameraDescriptor *descriptor = conversationCameraDescriptors;
    for (std::int32_t index = 0; index < 4; ++index, ++candidate, ++descriptor) {
        if (candidate->flags & 1U)
            continue;
        if ((descriptor->flags & 0x200U) && static_cast<std::int8_t>(candidate->collisionClass) != 10)
            candidate->flags |= 0x11U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void CheckConversationCandidateCollisions(ConversationCameraState *state) {
    static const std::uint8_t collisionClasses[16] = {0, 1, 2, 7, 3, 1, 8, 5, 4, 1, 2, 7, 9, 9, 6, 10};
    Vector3 eye0, target0, eye1, target1, eye2, target2, eye3, target3, eye4, target4;
    std::int32_t index;
    std::int32_t targetHit0, targetHit1, targetHit2, targetHit3, targetHit4;
    std::int32_t actorTargetHit0, actorTargetHit1, actorTargetHit2, actorTargetHit3, actorTargetHit4;
    std::int32_t actorEyeHit0, actorEyeHit1, actorEyeHit2, actorEyeHit3, actorEyeHit4;
    std::int32_t edgeHit0, edgeHit1, edgeHit2, edgeHit3;
    std::int32_t pointHit0, pointHit1, pointHit2, pointHit3, pointHit4;
    for (index = 0; index < state->candidateCount; ++index) {
        ConversationCandidateState *candidate = &state->candidates[index];
        if (candidate->flags & 1U)
            continue;
        switch (static_cast<std::int8_t>(candidate->group)) {
        case 0:
            SampleConversationCandidate(eye0, target0, index, 0.0f);
            SampleConversationCandidate(eye1, target0, index, 1.0f);
            if (ConversationCollisionSegment(eye0, eye1, 0.22f))
                candidate->flags = 1;
            break;
        case 1: {
            SampleConversationCandidate(eye0, target0, index, 0.0f);
            SampleConversationCandidate(eye1, target1, index, 0.25f);
            SampleConversationCandidate(eye2, target2, index, 0.5f);
            SampleConversationCandidate(eye3, target3, index, 0.75f);
            SampleConversationCandidate(eye4, target4, index, 1.0f);
            targetHit0 = ConversationCollisionSegment(eye0, target0, 0.22f);
            targetHit1 = ConversationCollisionSegment(eye1, target1, 0.22f);
            targetHit2 = ConversationCollisionSegment(eye2, target2, 0.22f);
            targetHit3 = ConversationCollisionSegment(eye3, target3, 0.22f);
            targetHit4 = ConversationCollisionSegment(eye4, target4, 0.22f);
            actorTargetHit0 = ConversationCollisionSegment(eye0, state->actorTarget, 0.22f);
            actorTargetHit1 = ConversationCollisionSegment(eye1, state->actorTarget, 0.22f);
            actorTargetHit2 = ConversationCollisionSegment(eye2, state->actorTarget, 0.22f);
            actorTargetHit3 = ConversationCollisionSegment(eye3, state->actorTarget, 0.22f);
            actorTargetHit4 = ConversationCollisionSegment(eye4, state->actorTarget, 0.22f);
            actorEyeHit0 = ConversationCollisionSegment(eye0, state->actorEye, 0.22f);
            actorEyeHit1 = ConversationCollisionSegment(eye1, state->actorEye, 0.22f);
            actorEyeHit2 = ConversationCollisionSegment(eye2, state->actorEye, 0.22f);
            actorEyeHit3 = ConversationCollisionSegment(eye3, state->actorEye, 0.22f);
            actorEyeHit4 = ConversationCollisionSegment(eye4, state->actorEye, 0.22f);
            edgeHit0 = ConversationCollisionSegment(eye0, eye1, 0.22f);
            edgeHit1 = ConversationCollisionSegment(eye1, eye2, 0.22f);
            edgeHit2 = ConversationCollisionSegment(eye2, eye3, 0.22f);
            edgeHit3 = ConversationCollisionSegment(eye3, eye4, 0.22f);
            pointHit0 = ConversationCollisionPoint(eye0);
            pointHit1 = ConversationCollisionPoint(eye1);
            pointHit2 = ConversationCollisionPoint(eye2);
            pointHit3 = ConversationCollisionPoint(eye3);
            pointHit4 = ConversationCollisionPoint(eye4);
            std::int32_t mask = 0;
            if (!(targetHit0 | targetHit1 | actorTargetHit0 | actorTargetHit1 | actorEyeHit0 | actorEyeHit1 | edgeHit0 | pointHit0 | pointHit1))
                mask |= 1;
            if (!(targetHit1 | targetHit2 | actorTargetHit1 | actorTargetHit2 | actorEyeHit1 | actorEyeHit2 | edgeHit1 | pointHit1 | pointHit2))
                mask |= 2;
            if (!(targetHit2 | targetHit3 | actorTargetHit2 | actorTargetHit3 | actorEyeHit2 | actorEyeHit3 | edgeHit2 | pointHit2 | pointHit3))
                mask |= 4;
            if (!(targetHit3 | targetHit4 | actorTargetHit3 | actorTargetHit4 | actorEyeHit3 | actorEyeHit4 | edgeHit3 | pointHit3 | pointHit4))
                mask |= 8;
            reinterpret_cast<std::int8_t &>(candidate->collisionClass) = collisionClasses[mask & 0xF];
            if (!reinterpret_cast<std::int8_t &>(candidate->collisionClass))
                candidate->flags |= 3U;
            break;
        }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static bool AreConversationAngleCandidatesBlocked(ConversationCameraState *state) {
    ResetConversationCandidates(state);
    std::int32_t orientation = state->orientationClass;
    if (orientation < 0)
        orientation = -orientation;
    std::uint32_t mask = static_cast<std::uint32_t>(ConversationOrientationGroups()[orientation]);
    mask = (mask & 7U) << 24;
    const ConversationCameraDescriptor *descriptor = conversationCameraDescriptors;
    ConversationCandidateState *candidate = state->candidates;
    std::int32_t index;
    for (index = 0; index < state->candidateCount; ++index, ++candidate, ++descriptor) {
        if ((descriptor->flags & 2U) == 0) {
            candidate->flags = 1;
            continue;
        }
        const std::uint32_t flags = descriptor->flags;
        if (!(flags & 0x7000000U)) {
            candidate->flags = 1;
        } else if ((flags & 0x7000000U) != mask) {
            candidate->flags = 1;
        }
    }
    CheckConversationCandidateCollisions(state);
    candidate = state->candidates;
    for (index = 0; index < state->candidateCount; ++index, ++candidate)
        if ((candidate->flags & 1) == 0)
            return false;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t ConversationCollisionSegment(const Vector3 &start, const Vector3 &end, float radius) {
    ConversationCameraState &state = *g_conversationCameraState;
    std::int32_t hit = 0;
    if (state.flags & 0x2000000U) {
        flddToggleFlag(0x1000);
        hit = GlobalRayIntersectionTest(start, end, radius);
    }
    if (state.flags & 0x4000000U) {
        ccmUpdateEntityFlag(state.targetActor);
        ccmUpdateEntityFlag(state.eyeActor);
        hit |= CheckCharacterCylinderIntersection(start, end);
    }
    return hit;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t ConversationCollisionPoint(const Vector3 &point) {
    ConversationCameraState &state = *g_conversationCameraState;
    std::int32_t hit = 0;
    if (state.flags & 0x2000000U)
        hit = IsWithinRadiusFromVector3PolyIntersectTest(&point, 0.25f);
    if (state.flags & 0x4000000U) {
        ccmUpdateEntityFlag(state.targetActor);
        ccmUpdateEntityFlag(state.eyeActor);
        hit |= CheckCharacterSphereIntersection(point, 0.25f);
    }
    return hit;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RotateConversationOffset(const ConversationOffset &offset, Vector3 &result) {
    result.x = -offset.distance * dc::sdk::SinFixed(offset.angle);
    result.y = offset.height;
    result.z = -offset.distance * dc::sdk::CosFixed(offset.angle);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MeasureConversationOffset(const Vector3 &point, ConversationOffset &offset) {
    offset.distance = ::sqrtf(point.x * point.x + point.z * point.z);
    offset.angle = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(point.x, point.z)) + 0x8000);
    offset.height = point.y;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlConversationUnsupported() {
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsConversationCameraCharacter(const Character *character) {
    ConversationCameraState *state = g_conversationCameraState;
    if (!(state->flags & 2U))
        return false;
    for (std::int32_t index = 0; index < state->memberCount; ++index)
        if (state->members[index] == character)
            return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void OffsetPointAlongYaw(Vector3 *result, const Vector3 *point, float distance, std::int16_t yaw) {
    const std::int16_t angle = static_cast<std::int16_t>(yaw + 0x8000);
    result->x = dc::sdk::SinFixed(angle) * distance + point->x;
    result->y = point->y;
    result->z = dc::sdk::CosFixed(angle) * distance + point->z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float UnprojectViewPointToWorld(float x, float y, float depth, Vector3 *world) {
    const float scale = -depth / camCtrlGetViewHeightScale();
    Vector3 view;
    view.x = x * scale;
    view.y = y * scale;
    view.z = depth;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetViewMatrix(0));
    Matrix4x4::TransformPoint(view, *world);
    PopCurrentMatrix(1);
    return scale;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen) {
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetProjectionMatrix(0));
    Matrix4x4::TransformPoint(world, screen);
    PopCurrentMatrix(1);
    float scale = 0.0f;
    if (!(1.1920928955078125e-07f > __builtin_fabsf(screen.z))) {
        scale = camCtrlGetViewHeightScale() / -screen.z;
        screen.x *= scale;
        screen.y *= scale;
    }
    return scale;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void *GetCameraMathWork() {
    return &g_cameraMathWork;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void TouchCameraMathWork(void *) {
    GetCameraMathWork();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetTransformedForwardVector(Vector3 *direction) {
    direction->x = 0.0f;
    direction->y = 0.0f;
    direction->z = -1.0f;
    Matrix4x4::Transform(*direction, *direction);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
shen_name GetCameraSpecialTargetName() {
    return (g_conversationCameraState->flags & 2U) ? g_conversationCameraState->specialTargetName : shen_name(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlConversationTargetUnsupported() {
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t camCtrlSetConversationLookTarget(const Vector3 *location) {
    g_conversationCameraState->lookLocation.x = location->x;
    g_conversationCameraState->lookLocation.y = location->y;
    g_conversationCameraState->lookLocation.z = location->z;
    g_conversationCameraState->flags |= 0x100000U;
    return 0;
}
#endif

// @verified - d
std::int32_t camCtrlSetConversationFlag4000(std::int32_t enabled) {
    ConversationCameraState *state = g_conversationCameraState;
    if (enabled)
        state->flags |= 0x4000U;
    else
        state->flags &= ~0x4000U;
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlSetConversationForcedCandidate(std::int32_t candidate) {
    if (candidate < 0 || candidate >= g_conversationCameraState->candidateCount) {
        g_conversationCameraState->forcedCandidate = -1;
        return -1;
    }
    g_conversationCameraState->forcedCandidate = candidate;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlSetInterpModeTo(std::uint8_t mode) {
    if (cameraMotionMode == 3) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        control->ryo->alpha = 1.0f;
    }
    cameraMotionMode = mode;
    switch (mode) {
    case 2:
        cameraMotionDistance = 7.5f;
        cameraMotionSecondaryDistance = 6.0f;
        break;
    case 3: {
        cameraMotionDistance = 1000.0f;
        cameraMotionSecondaryDistance = 1000.0f;
        cameraMotionPlayerAlpha = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo->alpha;
        if (0.30000001f > cameraMotionPlayerAlpha)
            cameraMotionPlayerAlpha = 0.30000001f;
        break;
    }
    default:
        cameraMotionDistance = 2.5f;
        cameraMotionSecondaryDistance = 2.0f;
        break;
    }
    cameraMotionDoubleDistance = cameraMotionDistance + cameraMotionDistance;
    cameraMotionDistanceSquared = cameraMotionDistance * cameraMotionDistance;
    cameraMotionDoubleDistanceSquared = cameraMotionDoubleDistance * cameraMotionDoubleDistance;
    cameraMotionSecondaryDoubleDistance = cameraMotionSecondaryDistance + cameraMotionSecondaryDistance;
    cameraMotionPatternSet = 0;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', '1', '0', '0'):
    case SHEN_NAME4('Q', '3', '0', '0'):
    case SHEN_NAME4('A', 'B', '0', '0'):
    case SHEN_NAME4('Q', 'T', 'B', '1'):
    case SHEN_NAME4('Q', 'S', 'F', 'A'):
    case SHEN_NAME4('Q', 'S', 'F', 'B'):
    case SHEN_NAME4('Q', 'S', 'F', 'D'):
    case SHEN_NAME4('Q', 'G', 'B', 'T'):
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
        cameraMotionPatternSet = 1;
        break;
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        cameraMotionPatternSet = 3;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'C'):
        cameraMotionPatternSet = 4;
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsEarlyAuthFightCamera() {
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (game && (game->stateFlags & 0x80000U)) {
        if (game->authCameraFrame > 4)
            return false;
        return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsAuthFightCamera() {
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return false;
    if (game->stateFlags & 0x80000U)
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetFightCameraLinkedKind(std::uint8_t kind) {
    if (kind < 4)
        fightCameraLinkedKind = kind;
    if (static_cast<std::uint8_t>(kind - 1) < 3) {
        fightCameraLinkedSide = fightCameraSide;
        fightCameraLinkedFrames = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint8_t camCtrlGetInterpMode() {
    return static_cast<std::uint8_t>(cameraMotionMode);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t GetFightCameraLinkedKind() {
    return static_cast<std::uint8_t>(fightCameraLinkedKind);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlSetFightDistance(float distance) {
    fightCameraDistanceSetting = distance;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlSetFightHeight(float height) {
    fightCameraHeightSetting = height;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeCameraGoal() {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (control->ryo)
        control->ryo->alpha = 1.0f;
    resetIntersectionAndTest();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void resetIntersectionAndTest() {
    camCtrlGetNextCamPos(&fightCameraEye);
    camCtrlGetTargetPosition(&fightCameraTarget);
    if (cameraMotionMode >= 0 && (cameraMotionMode < 2 || cameraMotionMode == 7)) {
        Vector3 result;
        if (FightCameraRaycast(result, fightCameraEye, fightCameraTarget))
            FightCameraCopy(result, fightCameraEye);
    }
    camCtrlSetFovTransitionTo(1);
    camCtrlSetPerspective(static_cast<std::int16_t>(camCtrlGetPerspective()));
    fightCameraFocusFrames = 0;
    fightCameraSpeedDown = 0;
    fightCameraSpeedUp = 0;
    fightCameraGoalLift = 0;
    fightCameraRayHit = 0;
    fightCameraSide = 1;
    fightCameraGoalOffset.x = fightCameraGoalOffset.y = fightCameraGoalOffset.z = 0.0f;
    fightCameraOffset.x = fightCameraOffset.y = fightCameraOffset.z = 0.0f;
    fightCameraEyeDirection.x = fightCameraEyeDirection.y = fightCameraEyeDirection.z = 0.0f;
    fightCameraTargetDirection.x = fightCameraTargetDirection.y = fightCameraTargetDirection.z = 0.0f;
    fightCameraLinkedKind = 0;
    fightCameraLinkedCharacter = 0;
    fightCameraLinkedActor = 0;
    ResetFightCameraPattern();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlEvaluateGoal() {
    camCtrlClearFlags(0x400U);
    if (IsWazamakiInactive())
        return;
    camCtrlSetNextPosition(fightCameraEye);
    camCtrlSetTargetPosition(fightCameraTarget);
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game || game->playerSlot == -1)
        return;
    Character *const character = game->characters[game->playerSlot];
    if (!character || game->slotTeams[game->playerSlot] == 1 || game->slotTeams[game->playerSlot] == 3)
        return;
    FightActorRuntimePrefix *const self = game->actors[game->playerSlot];
    FightActorRuntimePrefix *other;
    if (fightCameraLinkedActor)
        other = fightCameraLinkedActor;
    else
        other = self->actor;
    if (game->stateFlags & 0x10U)
        return;
    FightCameraCopy(fightCameraTarget, fightCameraDesiredTarget);
    FightCameraCopy(fightCameraEye, fightCameraDesiredEye);
    switch (cameraMotionMode) {
    case 0:
    case 1:
    case 7:
        UpdateEnvironmentFightCamera(game, self, other);
        break;
    case 2:
        UpdateFieldFightCamera(game, self, other);
        break;
    case 3:
        UpdateFirstPersonFightCamera(game, self, other);
        break;
    case 4:
    case 8:
        UpdateQuadraticFightCamera(game, self, other);
        break;
    case 5:
        UpdateMotionFightCamera(game, self, other);
        break;
    case 6:
        UpdateMotionInterpolationFightCamera(game, self, other);
        break;
    }
    Vector3 ray, result, origin, centre, offset;
    FightCameraSubtract(fightCameraDesiredTarget, fightCameraDesiredEye, ray);
    const float distance = FightCameraLength(ray);
    ScaleFightCameraVector(ray, distance + 0.1f);
    result.x = fightCameraDesiredTarget.x + ray.x;
    result.y = fightCameraDesiredTarget.y + ray.y;
    result.z = fightCameraDesiredTarget.z + ray.z;
    if (cameraMotionMode == 3) {
        origin.x = self->contactPosition.x;
        origin.y = result.y;
        origin.z = self->contactPosition.z;
        FightCameraRaycast(result, result, origin);
    } else {
        FightCameraRaycast(result, result, fightCameraDesiredTarget);
    }
    FightCameraSubtract(fightCameraDesiredTarget, result, ray);
    const float clipped = FightCameraLength(ray) - 0.05f;
    if (distance > clipped) {
        ScaleFightCameraVector(ray, clipped);
        fightCameraDesiredEye.x = fightCameraDesiredTarget.x + ray.x;
        fightCameraDesiredEye.y = fightCameraDesiredTarget.y + ray.y;
        fightCameraDesiredEye.z = fightCameraDesiredTarget.z + ray.z;
    }
    MoveFightCameraToward(fightCameraDesiredEye, fightCameraEye, fightCameraEyeDirection, 0);
    if (cameraMotionMode == 3) {
        switch (fightCameraFirstPersonEyeMode) {
        case 0:
            FightCameraCopy(fightCameraDesiredEye, fightCameraEye);
            break;
        case 1:
            fightCameraTarget.x = self->contactPosition.x;
            fightCameraTarget.z = self->contactPosition.z;
            break;
        }
        if (fightCameraFirstPersonEyeOrbit) {
            centre.x = self->contactPosition.x;
            centre.y = fightCameraEye.y;
            centre.z = self->contactPosition.z;
            FightCameraSubtract(centre, fightCameraEye, offset);
            if (self->hitStateFlags & 0x20000000U)
                ScaleFightCameraVector(offset, fightCameraFocusEyeScale);
            else
                ScaleFightCameraVector(offset, fightCameraEyeScale);
            fightCameraEye.x = centre.x + offset.x;
            fightCameraEye.z = centre.z + offset.z;
        }
    }
    MoveFightCameraToward(fightCameraDesiredTarget, fightCameraTarget, fightCameraTargetDirection, 0);
    float maximumY = 100000.0f;
    switch (g_currentMap) {
    case SHEN_NAME4('T', 'O', 'F', 'B'):
    case SHEN_NAME4('W', 'K', '0', '0'):
    case SHEN_NAME4('C', 'W', 'O', 'N'):
        maximumY = flddGetIntersectionFor(character, self->contactPosition) + 2.5f;
        break;
    case SHEN_NAME4('G', 'A', 'B', 'T'):
        maximumY = flddGetIntersectionFor(character, self->contactPosition) + 3.5f;
        break;
    default:
        if (GetQronNextStream() != -1000)
            maximumY = GetQronTemporaryFloorHeight() + 3.5f;
        break;
    }
    if (fightCameraEye.y > maximumY)
        fightCameraEye.y = maximumY;
    if (fightCameraTarget.y > maximumY)
        fightCameraTarget.y = maximumY;
    camCtrlSetNextPosition(fightCameraEye);
    camCtrlSetTargetPosition(fightCameraTarget);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 CameraVector(float x, float y, float z) {
    Vector3 result = {x, y, z};
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateEnvironmentFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    Vector3 position, raised, target, offset, work, heading, span, eye, resolved, delta, positive, negative, contact;
    if (!game->characters[game->playerSlot])
        return;
    if (static_cast<std::int32_t>(self->moveAngleTableIndex) < 30 && (game->stateFlags & 1) == 0)
        UpdateFightCameraPattern(game);
    position.x = self->contactPosition.x;
    position.y = self->targetGroundY;
    position.z = self->contactPosition.z;
    raised.x = self->contactPosition.x;
    raised.y = self->targetGroundY + 1.0f;
    raised.z = self->contactPosition.z;
    const std::uint32_t focus = self->hitStateFlags & 0x20000000;
    std::int32_t reduce = 0;
    if (game->type == 1 || game->type == 5)
        reduce = 1;
    switch (cameraMotionMode) {
    case 1:
        fightCameraCurrentSpeed = fightCameraDistanceSetting;
        fightCameraHeightOffset = fightCameraHeightSetting;
        break;
    case 7:
        fightCameraCurrentSpeed = 2.1768f;
        fightCameraHeightOffset = 0.5f;
        break;
    default:
        fightCameraCurrentSpeed = 2.1768f;
        fightCameraHeightOffset = 0.0f;
        break;
    }
    if (static_cast<std::int32_t>(game->stateFlags) >= 0) {
        fightCameraHeightOffset = 0.0f;
        if (fightCameraPattern.speed != 0.0f)
            fightCameraCurrentSpeed = fightCameraPattern.speed;
    }
    FightCameraVelocity *velocity = &fightCameraVelocity;
    AccumulateFightCameraVelocity(velocity, game, self);
    if (focus) {
        fightCameraFocusFrames++;
        if (fightCameraFocusFrames > 64.0f)
            fightCameraFocusFrames = 64;
    } else {
        fightCameraFocusFrames--;
        if (fightCameraFocusFrames < 0)
            fightCameraFocusFrames = 0;
    }
    if (velocity->closeEnemy && !focus) {
        fightCameraSpeedDown++;
        if (fightCameraSpeedDown > 64.0f)
            fightCameraSpeedDown = 64;
        fightCameraSpeedUp--;
        if (fightCameraSpeedUp < 0)
            fightCameraSpeedUp = 0;
    } else {
        fightCameraSpeedUp++;
        if (fightCameraSpeedUp > 64.0f)
            fightCameraSpeedUp = 64;
        fightCameraSpeedDown--;
        if (fightCameraSpeedDown < 0)
            fightCameraSpeedDown = 0;
    }
    work.x = position.x;
    work.y = position.y;
    work.z = position.z;
    if (::fabsf(velocity->divisor) >= FLT_EPSILON) {
        work.x += velocity->sum.x / velocity->divisor;
        work.y += velocity->sum.y / velocity->divisor;
        work.z += velocity->sum.z / velocity->divisor;
    }
    offset.x = work.x - position.x;
    offset.y = work.y - position.y;
    offset.z = work.z - position.z;
    if (reduce) {
        offset.x *= 0.6f;
        offset.y *= 0.6f;
        offset.z *= 0.6f;
    }
    float factor;
    if (fightCameraFocusFrames == 0)
        factor = 0.0f;
    else
        factor = fightCameraFocusFrames * 0.015625f;
    heading.x = -dc::sdk::SinFixed(static_cast<std::int16_t>(self->temporaryFacingAngle));
    heading.y = offset.y;
    heading.z = -dc::sdk::CosFixed(static_cast<std::int16_t>(self->temporaryFacingAngle));
    factor = 1.0f - factor;
    FightCameraBlendPoint(offset, heading, factor);
    if (::fabsf(velocity->divisor) >= FLT_EPSILON)
        factor = fightCameraCurrentSpeed + velocity->distance / velocity->divisor * 0.75f * factor + fightCameraSpeedUp * 0.015625f;
    else
        factor = fightCameraCurrentSpeed + fightCameraSpeedUp * 0.015625f;
    if (static_cast<std::int32_t>(game->stateFlags) < 0)
        factor *= 60.0f / (camCtrlGetPerspective() / 182.04445f);
    if (static_cast<std::uint32_t>(game->state) > 3) {
        FightCameraSubtract(fightCameraTarget, fightCameraEye, span);
        factor = FightCameraLength(span);
    }
    if (factor > 9.0f)
        factor = 9.0f;
    const float changeX = offset.x - fightCameraGoalOffset.x, changeY = offset.y - fightCameraGoalOffset.y, changeZ = offset.z - fightCameraGoalOffset.z;
    if (::fabsf(changeX) >= FLT_EPSILON)
        fightCameraGoalOffset.x += changeX * 0.33333334f;
    if (::fabsf(changeY) >= FLT_EPSILON)
        fightCameraGoalOffset.y += changeY * 0.33333334f;
    if (::fabsf(changeZ) >= FLT_EPSILON)
        fightCameraGoalOffset.z += changeZ * 0.33333334f;
    target.x = fightCameraGoalOffset.x + position.x;
    target.y = fightCameraGoalOffset.y + position.y + 0.84f + fightCameraPattern.phaseY;
    target.z = fightCameraGoalOffset.z + position.z;
    if (static_cast<std::uint32_t>(game->state) > 3)
        target.y = fightCameraTarget.y;
    ClampFightCameraPoint(fightCameraDesiredTarget, target);
    work.x = fightCameraEye.x - fightCameraTarget.x;
    work.y = 0.0f;
    work.z = fightCameraEye.z - fightCameraTarget.z;
    const float length = FightCameraLength(work);
    if (::fabsf(length) >= FLT_EPSILON) {
        work.x /= length;
        work.z /= length;
    } else {
        work.x = dc::sdk::SinFixed(static_cast<std::int16_t>(self->temporaryFacingAngle));
        work.z = dc::sdk::CosFixed(static_cast<std::int16_t>(self->temporaryFacingAngle));
    }
    if (focus) {
        std::int16_t angle = FightCameraYawBetween(0.0f, 0.0f, work.x, work.z);
        const std::int16_t behind = static_cast<std::int16_t>(static_cast<std::int16_t>(self->temporaryFacingAngle) + 0x8000);
        std::int16_t turn = static_cast<std::int16_t>(behind - angle);
        turn = turn > 2048 ? static_cast<std::int16_t>(turn - 2048) : turn < -2048 ? static_cast<std::int16_t>(turn + 2048) : 0;
        turn = static_cast<std::int16_t>(turn / 4);
        turn = turn < -2048 ? -2048 : turn > 2048 ? 2048 : turn;
        angle = static_cast<std::int16_t>(angle + turn);
        work.x = -dc::sdk::SinFixed(angle);
        work.y = 0.0f;
        work.z = -dc::sdk::CosFixed(angle);
    }
    eye.x = fightCameraDesiredTarget.x + work.x * factor;
    eye.z = fightCameraDesiredTarget.z + work.z * factor;
    eye.y = fightCameraDesiredTarget.y + 0.44000003f - fightCameraSpeedDown * 0.015625f * 0.8f + (fightCameraPattern.phaseX - fightCameraPattern.phaseY);
    eye.y = eye.y > self->targetGroundY + 0.05f + fightCameraHeightOffset ? eye.y : self->targetGroundY + 0.05f + fightCameraHeightOffset;
    ClampFightCameraPoint(fightCameraDesiredEye, eye);
    if (other && !focus) {
        resolved.x = fightCameraDesiredEye.x;
        resolved.y = fightCameraDesiredEye.y;
        resolved.z = fightCameraDesiredEye.z;
        const std::int16_t current = FightCameraYawBetween(fightCameraDesiredTarget.x, fightCameraDesiredTarget.z, resolved.x, resolved.z);
        std::int16_t yaw = FightCameraYawBetween(position.x, position.z, other->contactPosition.x, other->contactPosition.z);
        if (static_cast<std::int16_t>(yaw - current) > 0)
            fightCameraSide = 1;
        else
            fightCameraSide = -1;
        std::int32_t near = 0;
        const std::int16_t back = static_cast<std::int16_t>(yaw + 0x8000);
        yaw = static_cast<std::int16_t>(yaw + 0x4000);
        delta.x = other->contactPosition.x - self->contactPosition.x;
        delta.y = other->targetGroundY - self->targetGroundY;
        delta.z = other->contactPosition.z - self->contactPosition.z;
        std::int16_t turn = 0;
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < cameraMotionDistanceSquared) {
            near = 1;
            if (!(fightCameraPattern.state & 2) && (yaw - current < 0 ? current - yaw : yaw - current) > 0x4000)
                yaw = static_cast<std::int16_t>(yaw + 0x8000);
            if (yaw == back) {
                std::int32_t distance = yaw - current;
                if (distance < 0)
                    distance = current - yaw;
                if (distance > 5460) {
                    if (yaw > current)
                        yaw = static_cast<std::int16_t>(yaw - 5461);
                    else
                        yaw = static_cast<std::int16_t>(yaw + 5461);
                } else
                    yaw = current;
            } else {
                const std::int16_t difference = static_cast<std::int16_t>(current - back);
                if ((difference < 0 ? -difference : difference) < 8192) {
                    if (yaw - back > 0)
                        yaw = static_cast<std::int16_t>(back + 8192);
                    else
                        yaw = static_cast<std::int16_t>(back - 8192);
                } else if ((difference < 0 ? -difference : difference) > 21845) {
                    if (yaw - back > 0)
                        yaw = static_cast<std::int16_t>(back + 21845);
                    else
                        yaw = static_cast<std::int16_t>(back - 21845);
                } else
                    yaw = current;
            }
            float reach = 3.0f;
            delta.x = fightCameraDesiredTarget.x - dc::sdk::SinFixed(yaw) * reach;
            delta.y = resolved.y;
            delta.z = fightCameraDesiredTarget.z - dc::sdk::CosFixed(yaw) * reach;
            if (FightCameraRaycast(positive, delta, fightCameraDesiredTarget)) {
                yaw = static_cast<std::int16_t>(yaw + 0x8000);
                delta.x = fightCameraDesiredTarget.x - dc::sdk::SinFixed(yaw) * reach;
                delta.y = resolved.y;
                delta.z = fightCameraDesiredTarget.z - dc::sdk::CosFixed(yaw) * reach;
                if (FightCameraRaycast(positive, delta, fightCameraDesiredTarget))
                    yaw = current;
            }
            turn = static_cast<std::int16_t>(yaw - current);
            turn = turn > 2048 ? static_cast<std::int16_t>(turn - 2048) : turn < -2048 ? static_cast<std::int16_t>(turn + 2048) : 0;
            turn = static_cast<std::int16_t>(turn / 4);
            turn = turn < -2048 ? -2048 : turn > 2048 ? 2048 : turn;
        }
        if (near && turn) {
            delta.x = resolved.x - fightCameraDesiredTarget.x;
            delta.y = resolved.y - fightCameraDesiredTarget.y;
            delta.z = resolved.z - fightCameraDesiredTarget.z;
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::UnitCurrentMatrix();
            matrix_stack::TranslateVector(&fightCameraDesiredTarget);
            Matrix4x4::RotateAroundY_Wrapper(turn);
            Matrix4x4::TransformPoint(delta, resolved);
            PopCurrentMatrix(1);
            if (!FightCameraRaycast(positive, resolved, raised)) {
                fightCameraDesiredEye.x = resolved.x;
                fightCameraDesiredEye.y = resolved.y;
                fightCameraDesiredEye.z = resolved.z;
            }
        }
    }
    fightCameraRayHit = FightCameraRaycast(resolved, fightCameraDesiredEye, raised);
    if (fightCameraRayHit == 1) {
        if (FightCameraRaycast(resolved, fightCameraDesiredEye, fightCameraEye)) {
            fightCameraDesiredEye.x = resolved.x;
            fightCameraDesiredEye.z = resolved.z;
        } else {
            FightCameraSubtract(raised, resolved, delta);
            FightCameraLength(delta);
            FightCameraSubtract(raised, fightCameraDesiredEye, delta);
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::UnitCurrentMatrix();
            matrix_stack::TranslateVector(&raised);
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::RotateAroundY_Wrapper(0x2000);
            Matrix4x4::TransformPoint(delta, positive);
            PopCurrentMatrix(1);
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::RotateAroundY_Wrapper(0xe000);
            Matrix4x4::TransformPoint(delta, negative);
            PopCurrentMatrix(1);
            PopCurrentMatrix(1);
            std::int32_t usePositive = 0;
            if (!FightCameraRaycast(contact, positive, position))
                usePositive = 1;
            else {
                FightCameraSubtract(raised, contact, contact);
                const float positiveDistance = FightCameraLength(contact);
                if (FightCameraRaycast(contact, negative, position)) {
                    FightCameraSubtract(raised, contact, contact);
                    if (positiveDistance > FightCameraLength(contact))
                        usePositive = 1;
                }
            }
            if (usePositive) {
                resolved.x = positive.x;
                resolved.z = positive.z;
            } else {
                resolved.x = negative.x;
                resolved.z = negative.z;
            }
        }
        fightCameraGoalLift++;
        if (fightCameraGoalLift > 64)
            fightCameraGoalLift = 64;
    } else {
        resolved.x = fightCameraDesiredEye.x;
        resolved.z = fightCameraDesiredEye.z;
        fightCameraGoalLift--;
        if (fightCameraGoalLift < 0)
            fightCameraGoalLift = 0;
    }
    float driftX = resolved.x - fightCameraDesiredEye.x - fightCameraOffset.x;
    float driftZ = resolved.z - fightCameraDesiredEye.z - fightCameraOffset.z;
    if (::fabsf(driftX) >= FLT_EPSILON)
        fightCameraOffset.x += driftX * 0.33333334f;
    if (::fabsf(driftZ) >= FLT_EPSILON)
        fightCameraOffset.z += driftZ * 0.33333334f;
    fightCameraDesiredEye.x += fightCameraOffset.x;
    fightCameraDesiredEye.y += fightCameraGoalLift * 0.015625f * 1.5f;
    fightCameraDesiredEye.z += fightCameraOffset.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateFirstPersonFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    Vector3 base, average, selfBone, otherBone, delta, heading, toTarget, toEye, eye, offset;
    float selfHeight, otherHeight, perspectiveScale, blend;
    std::int32_t followup = 0;
    if ((self->activeMotionFlags & 0x8000000) || (self->groundContactFlags & 1))
        followup = 1;
    const std::uint32_t knocked = self->hitStateFlags & 0xc0000U;
    charaGetMotWorldPos(self->character, 3, &selfBone);
    charaGetMotWorldPos(other->character, 3, &otherBone);
    if (fightCameraBoneHeightEnabled) {
        selfHeight = (selfBone.y - self->targetGroundY) + (fightCameraSelfHeight - 1.2f);
        otherHeight = (otherBone.y - other->targetGroundY) + (fightCameraOtherHeight - 1.2f);
    } else {
        selfHeight = fightCameraSelfHeight;
        otherHeight = fightCameraOtherHeight;
    }
    perspectiveScale = 60 / (camCtrlGetPerspective() / 182.04445f);
    base.x = self->contactPosition.x;
    base.y = self->targetGroundY;
    base.z = self->contactPosition.z;
    if ((knocked || (self->hitStateFlags & 0x10000U)) && !(self->hitStateFlags & 0x8000U)) {
        EV_figpAssignFlagsFor(self->character, 0, 1);
        figpAssignModeForCharacter(self->character, 0);
        charaGetMotWorldPos(self->character, 5, &fightCameraDesiredEye);
        if (fightCameraTargetOwnBone)
            charaGetMotWorldPos(self->character, 7, &fightCameraDesiredTarget);
        else
            charaGetMotWorldPos(other->character, 5, &fightCameraDesiredTarget);
        FightCameraSubtract(fightCameraDesiredTarget, fightCameraDesiredEye, delta);
        const float distance = FightCameraLength(delta) + perspectiveScale * fightCameraLeadScale;
        ScaleFightCameraVector(delta, perspectiveScale * fightCameraLeadScale);
        fightCameraDesiredEye.x += delta.x;
        fightCameraDesiredEye.y += delta.y;
        fightCameraDesiredEye.z += delta.z;
        const float minimumY = self->targetGroundY + 0.15000001f;
        fightCameraDesiredTarget.y = fightCameraDesiredTarget.y > minimumY ? fightCameraDesiredTarget.y : minimumY;
        if (minimumY > fightCameraDesiredEye.y) {
            const float y = fightCameraDesiredTarget.y - minimumY;
            const float length = ::sqrtf(distance * distance - y * y);
            delta.y = 0;
            ScaleFightCameraVector(delta, length);
            fightCameraDesiredEye.x = fightCameraDesiredTarget.x + delta.x;
            fightCameraDesiredEye.y = minimumY;
            fightCameraDesiredEye.z = fightCameraDesiredTarget.z + delta.z;
        }
        return;
    }
    average.x = average.y = average.z = 0;
    std::int32_t count = 0;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (game->characters[i] && game->slotTeams[i] == 1) {
            struct FightSlotFacing {
                std::byte retained[0x64];
                std::uint16_t facingDifferences[6];
            };
            if (reinterpret_cast<const FightSlotFacing *>(self->slot)->facingDifferences[i] < 0x4000) {
                average.x += game->actors[i]->position.x;
                average.y += game->actors[i]->targetGroundY;
                average.z += game->actors[i]->position.z;
                count++;
            }
        }
    }
    if (count) {
        average.x /= count;
        average.y /= count;
        average.z /= count;
    }
    FightCameraCopy(fightCameraTarget, fightCameraDesiredTarget);
    if (followup) {
        fightCameraFocusFrames++;
        if (fightCameraFocusFrames > 64.0f)
            fightCameraFocusFrames = 64;
    } else {
        fightCameraFocusFrames--;
        if (fightCameraFocusFrames < 0)
            fightCameraFocusFrames = 0;
    }
    if (fightCameraFocusFrames)
        blend = fightCameraFocusFrames * 0.015625f;
    else
        blend = 0;
    if (!count)
        blend = 1.0f;
    FightCameraSubtract(base, fightCameraDesiredTarget, toTarget);
    const float distance = FightCameraLength(toTarget);
    heading.x = base.x - dc::sdk::SinFixed(static_cast<std::int16_t>(self->temporaryFacingAngle)) * distance;
    heading.y = base.y;
    heading.z = base.z - dc::sdk::CosFixed(static_cast<std::int16_t>(self->temporaryFacingAngle)) * distance;
    FightCameraBlendPoint(heading, average, blend);
    heading.y += otherHeight;
    ClampFightCameraPoint(fightCameraDesiredTarget, heading);
    FightCameraSubtract(base, fightCameraDesiredTarget, toEye);
    toEye.y -= selfHeight;
    const float length = FightCameraLength(toEye) + perspectiveScale * (followup ? fightCameraFocusEyeScale : fightCameraEyeScale);
    ScaleFightCameraVector(toEye, length);
    eye.x = fightCameraDesiredTarget.x - toEye.x;
    eye.y = fightCameraDesiredTarget.y - toEye.y;
    eye.z = fightCameraDesiredTarget.z - toEye.z;
    eye.y = eye.y > self->targetGroundY + 0.15000001f ? eye.y : self->targetGroundY + 0.15000001f;
    ClampFightCameraPoint(fightCameraDesiredEye, eye);
    const std::int16_t yaw = FightCameraYawBetween(fightCameraDesiredTarget.x, fightCameraDesiredTarget.z, fightCameraDesiredEye.x, fightCameraDesiredEye.z);
    const std::int16_t facing = static_cast<std::int16_t>(static_cast<std::int16_t>(self->temporaryFacingAngle) - 0x8000);
    std::int16_t turn = static_cast<std::int16_t>(facing - yaw);
    turn = turn > 2048 ? static_cast<std::int16_t>(turn - 2048) : turn < -2048 ? static_cast<std::int16_t>(turn + 2048) : 0;
    turn = static_cast<std::int16_t>(turn / 4);
    turn = turn < -1152 ? -1152 : turn > 1152 ? 1152 : turn;
    FightCameraSubtract(fightCameraDesiredTarget, fightCameraDesiredEye, offset);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    matrix_stack::TranslateVector(&fightCameraDesiredTarget);
    Matrix4x4::RotateAroundY_Wrapper(turn);
    Matrix4x4::TransformPoint(offset, fightCameraDesiredEye);
    PopCurrentMatrix(1);
    if (static_cast<std::uint32_t>(game->state) > 3) {
        cameraMotionPlayerAlpha += 0.050000001f;
        if (cameraMotionPlayerAlpha > 1.0f)
            cameraMotionPlayerAlpha = 1.0f;
    } else {
        cameraMotionPlayerAlpha -= 0.1f;
        if (cameraMotionPlayerAlpha < 0.30000001f)
            cameraMotionPlayerAlpha = 0.30000001f;
    }
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    static_cast<Character *>(control->ryo)->alpha = cameraMotionPlayerAlpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateFieldFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    Vector3 delta, eye, first, second;
    first.x = self->contactPosition.x;
    first.y = self->targetGroundY + 1.0f;
    first.z = self->contactPosition.z;
    second.x = other->contactPosition.x;
    second.y = other->targetGroundY + 1.0f;
    second.z = other->contactPosition.z;
    FightCameraSubtract(first, second, delta);
    fightCameraDesiredTarget.x = first.x + delta.x * 0.5f;
    fightCameraDesiredTarget.y = first.y + delta.y * 0.5f;
    fightCameraDesiredTarget.z = first.z + delta.z * 0.5f;
    const float separation = FightCameraLength(delta);
    float distance = separation * 1.3f;
    if (distance < fightCameraMinimumDistance)
        distance = fightCameraMinimumDistance;
    std::int16_t currentYaw = FightCameraYawBetween(fightCameraDesiredTarget.x, fightCameraDesiredTarget.z, fightCameraDesiredEye.x, fightCameraDesiredEye.z);
    std::int16_t desiredYaw = FightCameraYawBetween(first.x, first.z, second.x, second.z);
    desiredYaw = static_cast<std::int16_t>(desiredYaw - 0x4000);
    if (separation > 5.0f) {
        float degrees;
        if (separation > 10.0f)
            degrees = 90.0f;
        else
            degrees = (separation - 5.0f) * 18.0f;
        desiredYaw = static_cast<std::int16_t>(desiredYaw + static_cast<std::int32_t>(8192.0f * degrees / 45.0f));
    }
    currentYaw = static_cast<std::int16_t>(desiredYaw - currentYaw);
    if (static_cast<std::int16_t>(currentYaw + 0x4000) < 0)
        currentYaw = static_cast<std::int16_t>(currentYaw - 0x8000);
    currentYaw = currentYaw > 2048 ? static_cast<std::int16_t>(currentYaw - 2048) : currentYaw < -2048 ? static_cast<std::int16_t>(currentYaw + 2048) : 0;
    currentYaw = static_cast<std::int16_t>(currentYaw / 4);
    currentYaw = currentYaw < -2048 ? -2048 : currentYaw > 2048 ? 2048 : currentYaw;
    const std::int16_t turn = currentYaw;
    delta.x = fightCameraDesiredEye.x - fightCameraDesiredTarget.x;
    delta.y = fightCameraDesiredEye.y - fightCameraDesiredTarget.y;
    delta.z = fightCameraDesiredEye.z - fightCameraDesiredTarget.z;
    ScaleFightCameraVector(delta, distance);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    matrix_stack::TranslateVector(&fightCameraDesiredTarget);
    Matrix4x4::RotateAroundY_Wrapper(turn);
    Matrix4x4::TransformPoint(delta, eye);
    PopCurrentMatrix(1);
    eye.y = fightCameraDesiredTarget.y;
    if (FightCameraRaycast(eye, eye, fightCameraDesiredTarget)) {
        FightCameraSubtract(fightCameraDesiredTarget, eye, delta);
        const float horizontalSquared = delta.x * delta.x + delta.z * delta.z;
        eye.y = ::sqrtf(distance * distance - horizontalSquared) + eye.y;
    }
    fightCameraDesiredEye.x = eye.x;
    fightCameraDesiredEye.y = eye.y;
    fightCameraDesiredEye.z = eye.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t camCtrlGetNextRoll();

void UpdateQuadraticFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    Vector3 position, linked, relative, direction, result, lifted;
    if (IsEarlyAuthFightCamera()) {
        fightCameraLinkedKind = 2;
        fightCameraLinkedCharacter = self->character;
        fightCameraLinkedActor = self;
    }
    if (fightCameraLinkedKind > 3)
        return;
    if (fightCameraLinkedKind < 1)
        return;
    if (cameraMotionMode == 4) {
        if (!other || (other->hitStateFlags & 0x8000U) || (game->state == 4 && FindBattleStatusEntry(game->playerCharacter) == 3)) {
            vfUnknownCamRelated(game);
            ResetFightCameraToParticipants(game, self, other);
            return;
        }
    } else if (other->animationTime > static_cast<float>(other->motionFrames)) {
        vfUnknownCamRelated(game);
        return;
    }
    position.x = self->contactPosition.x;
    position.y = self->targetGroundY;
    position.z = self->contactPosition.z;
    if (IsEarlyAuthFightCamera()) {
        position.x = other->targetPosition.x;
        position.z = other->targetPosition.z;
    }
    FightCameraCopy(fightCameraLinkedActor->contactPosition, linked);
    if (IsEarlyAuthFightCamera()) {
        linked.x = other->targetPosition.x;
        linked.z = other->targetPosition.z;
        FightCameraCopy(linked, fightCameraDesiredTarget);
    } else
        ClampFightCameraPoint(fightCameraDesiredTarget, linked);
    float radius = 60.0f / (static_cast<float>(camCtrlGetPerspective()) / 182.04445f);
    if (fightCameraLinkedKind != 3)
        radius += radius;
    FightCameraSubtract(fightCameraTarget, fightCameraEye, direction);
    direction.y = 0.0f;
    ScaleFightCameraVector(direction, radius);
    direction.x += fightCameraDesiredTarget.x;
    direction.z += fightCameraDesiredTarget.z;
    switch (fightCameraLinkedKind) {
    case 1:
        direction.y = other->targetGroundY + 0.2f;
        break;
    case 2:
        direction.y = other->targetGroundY + 0.1f + other->animationTime * 0.05f;
        if (direction.y > other->targetGroundY + 2.75f)
            direction.y = other->targetGroundY + 2.75f;
        break;
    case 3:
        direction.y = fightCameraDesiredEye.y;
        break;
    }
    ClampFightCameraPoint(fightCameraDesiredEye, direction);
    const std::int16_t current =
        FightCameraYawBetween(fightCameraDesiredTarget.x, fightCameraDesiredTarget.z, fightCameraDesiredEye.x, fightCameraDesiredEye.z);
    std::int16_t angle = FightCameraYawBetween(position.x, position.z, other->contactPosition.x, other->contactPosition.z);
    std::int16_t difference;
    switch (fightCameraLinkedKind) {
    case 1:
        angle = static_cast<std::int16_t>(angle - fightCameraLinkedSide * 4096);
        break;
    case 2:
        angle = static_cast<std::int16_t>(angle + 0x8000);
        angle = static_cast<std::int16_t>(angle + (fightCameraLinkedFrames * 208 + 1024) * fightCameraLinkedSide);
        break;
    case 3: {
        const FightSpawnPoint &spawn = self->game->spawnPoints[self->nearestSpawnPoint];
        const float *const spawnXZ = &spawn.x;
        angle = FightCameraYawBetween(spawnXZ[0], spawnXZ[1], fightCameraLinkedActor->contactPosition.x, fightCameraLinkedActor->contactPosition.z);
        difference = static_cast<std::int16_t>(::fabsf(static_cast<float>(angle - current)));
        if (difference < 0x2000) {
            FightCameraCopy(fightCameraDesiredEye, lifted);
            if (!(g_combatCameraFlags & 2U))
                lifted.y = 126.0f;
            ClampFightCameraPoint(fightCameraDesiredEye, lifted);
        } else if (difference > 0x4000)
            g_combatCameraFlags |= 2U;
        break;
    }
    }
    difference = static_cast<std::int16_t>(angle - current);
    difference = difference < -0x6000 ? -0x6000 : (difference > 0x6000 ? 0x6000 : difference);
    FightCameraSubtract(fightCameraDesiredTarget, fightCameraDesiredEye, relative);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    matrix_stack::TranslateVector(&fightCameraDesiredTarget);
    Matrix4x4::RotateAroundY_Wrapper(difference);
    Matrix4x4::TransformPoint(relative, result);
    PopCurrentMatrix(1);
    if (fightCameraLinkedKind != 3) {
        if (FightCameraRaycast(result, result, fightCameraEye)) {
            fightCameraHasIntersectionTest = 0;
            return;
        }
        if (fightCameraHasIntersectionTest)
            ++fightCameraLinkedFrames;
    }
    FightCameraCopy(result, fightCameraDesiredEye);
    if (fightCameraLinkedKind == 3) {
        std::int16_t roll = camCtrlGetNextRoll();
        if (roll < 0x2000)
            roll = static_cast<std::int16_t>(roll + 0x40);
        camCtrlSetNextRoll(roll);
        if (fightCameraLinkedActor->contactPosition.y < 121.0f) {
            if ((g_combatCameraFlags & 1U) == 0) {
                WaitForNextTask_0(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, 1, 10);
                g_combatCameraFlags |= 1U;
            }
        }
    }
    if (IsEarlyAuthFightCamera()) {
        FightCameraCopy(fightCameraDesiredEye, fightCameraEye);
        FightCameraCopy(fightCameraDesiredTarget, fightCameraTarget);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BeginMotionUpdateCamera(Character *target) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    MotionWork *const motion = charFindMotionwork(control->ryo);
    fightMotionCameraTarget = target;
    SetPlayerControlEnabled(0);
    camCtrlSetInterpModeTo(5);
    resetIntersectionAndTest();
    camCtrlSetFovTransitionTo(0);
    camCtrlSetState(10);
    FightCameraCopy(fightCameraEye, fightMotionCameraSavedEye);
    FightCameraCopy(fightCameraTarget, fightMotionCameraSavedTarget);
    mtwkRequestMotion(motion, 0xf01b, static_cast<MotionRegion>(0));
    fightMotionCameraUpdateState = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateMotionFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const player = control->ryo;
    MotionWork *const motion = charFindMotionwork(player);
    Vector3 bone, position, focus, eye, direction, offset;
    charaGetMotWorldPos(player, 5, &bone);
    FightCameraCopy(bone, eye);
    if (fightMotionCameraTarget) {
        GetCharacterWorldPosition(fightMotionCameraTarget, &position);
        focus.x = position.x;
        focus.y = position.y + 1.0f;
        focus.z = position.z;
    } else {
        focus.x = (self->position.x + other->position.x) * 0.5f;
        focus.y = (self->position.y + other->position.y) * 0.5f + 1.0f;
        focus.z = (self->position.z + other->position.z) * 0.5f;
    }
    const MotionRegion region = static_cast<MotionRegion>(0);
    switch (fightMotionCameraUpdateState) {
    case 0:
        if (mtwkGetAnimationId(motion, region) == 0xf01bU && mtwkGetFrame(motion, region) > 20.0f) {
            cpamSetFlag(player, 1);
            fightMotionCameraUpdateState = 1;
        }
        break;
    case 1:
        FightCameraCopy(eye, fightCameraDesiredEye);
        FightCameraCopy(focus, fightCameraDesiredTarget);
        if (mtwkGetAnimationId(motion, region) == 0xf01bU && mtwkSetRegionFlag(motion, 65, 1, region)) {
            mtwkRequestMotion(motion, 0xf01aU, region);
            SetHelpIconMenuFlags(320, 1);
            camCtrlToggleSmoothing(1);
            camCtrlSetMotionScaleFactor(1.5f);
            fightMotionCameraUpdateState = 2;
        }
        break;
    case 2:
        FightCameraSubtract(eye, focus, direction);
        direction.y = 0.0f;
        ScaleFightCameraVector(direction, 1.0f);
        eye.x += direction.x;
        eye.z += direction.z;
        FightCameraCopy(eye, fightCameraDesiredEye);
        FightCameraCopy(focus, fightCameraDesiredTarget);
        mtwkSetRegionEvalTime(motion, 0.0f, region);
        if (g_inputBuffer[0].held & 4U) {
            float perspective = static_cast<float>(camCtrlGetPerspective()) / 182.04445f;
            if ((g_inputBuffer[0].held & 0x40U) && perspective < 60.0f)
                perspective += 2.0f;
            else if ((g_inputBuffer[0].held & 0x80U) && perspective > 10.0f)
                perspective -= 2.0f;
            camCtrlSetPerspective(static_cast<std::int16_t>(static_cast<std::int32_t>((perspective * 8192.0f) / 45.0f)));
        } else {
            camCtrlToggleSmoothing(0);
            ResetHelpIconsToItemPrompt();
            fightMotionCameraUpdateState = 3;
        }
        break;
    case 3: {
        FightCameraCopy(eye, fightCameraDesiredEye);
        FightCameraCopy(focus, fightCameraDesiredTarget);
        mtwkSetRegionEvalTime(motion, 0.0f, region);
        const float perspective = static_cast<float>(camCtrlGetPerspective()) / 182.04445f;
        if (perspective < 56.0f)
            camCtrlSetPerspective(static_cast<std::int16_t>(static_cast<std::int32_t>(((perspective + 4.0f) * 8192.0f) / 45.0f)));
        else
            fightMotionCameraUpdateState = 4;
        break;
    }
    case 4:
        FightCameraCopy(fightMotionCameraSavedEye, fightCameraDesiredEye);
        FightCameraCopy(fightMotionCameraSavedTarget, fightCameraDesiredTarget);
        if (mtwkGetAnimationId(motion, region) == 0xf01aU && mtwkSetRegionFlag(motion, 65, 1, region)) {
            camCtrlSetInterpModeTo(0);
            camCtrlSetFovTransitionTo(0);
            camCtrlSetState(0);
            cpamSetFlag(player, 0);
            SetPlayerControlEnabled(1);
            EV_camCtrlToggleCombatCamera(4U, 0);
            fightMotionCameraUpdateState = 5;
        }
        break;
    }
    FightCameraSubtract(bone, fightCameraEye, offset);
    float alpha = FightCameraLength(offset) - 0.2f;
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    player->alpha = alpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResetMotionCameraWaypoints() {
    fightMotionCameraWaypointCount = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AppendMotionCameraWaypoint(const Vector3 &position) {
    if (fightMotionCameraWaypointCount < 5) {
        FightCameraCopy(position, fightMotionCameraWaypoints[fightMotionCameraWaypointCount]);
        ++fightMotionCameraWaypointCount;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BeginMotionInterpolationCamera(Character *target) {
    (void)HLib::GetTaskParameter(g_mapControlTask);
    fightMotionCameraTarget = target;
    if (target) {
        charaGetMotWorldPos(target, 5, &fightMotionCameraWaypoints[fightMotionCameraWaypointCount]);
        ++fightMotionCameraWaypointCount;
    }
    SetPlayerControlEnabled(0);
    camCtrlSetInterpModeTo(6);
    resetIntersectionAndTest();
    camCtrlSetFovTransitionTo(0);
    camCtrlSetState(10);
    FightCameraCopy(fightCameraEye, fightMotionCameraSavedEye);
    FightCameraCopy(fightCameraTarget, fightMotionCameraSavedTarget);
    fightMotionCameraState = 0;
    fightMotionCameraWaypointIndex = 0;
    fightMotionCameraInitializeEye = 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateMotionInterpolationFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *, FightActorRuntimePrefix *) {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    Vector3 bone, point, offset;
    charaGetMotWorldPos(player, 5, &bone);
    bone.y += 0.5f;
    FightCameraCopy(bone, fightCameraDesiredEye);
    if (fightMotionCameraInitializeEye) {
        fightMotionCameraInitializeEye = 0;
        FightCameraCopy(bone, fightCameraEye);
        const std::int32_t yaw = GetCharacterWorldYaw(player);
        point.x = CalculateFixedSine(-yaw) * 5.0f + bone.x;
        point.z = bone.z + -CalculateFixedCosine(-yaw) * 5.0f;
        point.y = bone.y + 0.0f;
        FightCameraCopy(point, fightCameraTarget);
    }
    fightMotionCameraPreviousPerspective = camCtrlGetPerspective();
    camCtrlSetPerspective(3640);
    switch (fightMotionCameraState) {
    case 0:
        FightCameraCopy(fightCameraTarget, fightMotionCameraSegmentTarget);
        if (fightMotionCameraWaypointIndex == fightMotionCameraWaypointCount) {
            fightMotionCameraTime = 60;
            if (fightMotionCameraTarget)
                fightMotionCameraState = 2;
            else
                fightMotionCameraState = 3;
        } else {
            fightMotionCameraState = 1;
            if (fightMotionCameraTarget && fightMotionCameraWaypointIndex == fightMotionCameraWaypointCount - 1)
                fightMotionCameraTime = 10;
            else
                fightMotionCameraTime = 60;
        }
        break;
    case 1: {
        std::int32_t time = fightMotionCameraTime;
        if (time <= 14)
            time = 15;
        FightCameraCopy(fightMotionCameraSegmentTarget, point);
        const Vector3 &waypoint = fightMotionCameraWaypoints[fightMotionCameraWaypointIndex];
        FightCameraBlendPoint(point, waypoint, static_cast<float>(time - 15) / 45.0f);
        ClampFightCameraPoint(fightCameraDesiredTarget, point);
        if (--fightMotionCameraTime == 0) {
            ++fightMotionCameraWaypointIndex;
            fightMotionCameraState = 0;
        }
        break;
    }
    case 2:
        charaGetMotWorldPos(fightMotionCameraTarget, 5, &fightCameraDesiredTarget);
        FightCameraSubtract(fightMotionCameraSegmentTarget, fightCameraDesiredEye, point);
        FightCameraLength(point);
        ScaleFightCameraVector(point, 1.5f);
        point.x += fightMotionCameraSegmentTarget.x;
        point.y += fightMotionCameraSegmentTarget.y;
        point.z += fightMotionCameraSegmentTarget.z;
        ClampFightCameraPoint(fightCameraDesiredEye, point);
        if (--fightMotionCameraTime == 0)
            fightMotionCameraState = 4;
        break;
    case 3:
        if (--fightMotionCameraTime == 0)
            fightMotionCameraState = 4;
        break;
    case 4:
        FightCameraCopy(fightMotionCameraSavedEye, fightCameraDesiredEye);
        FightCameraCopy(fightMotionCameraSavedTarget, fightCameraDesiredTarget);
        camCtrlSetPerspective(static_cast<std::int16_t>(fightMotionCameraPreviousPerspective));
        camCtrlSetInterpModeTo(0);
        camCtrlSetFovTransitionTo(0);
        camCtrlSetState(0);
        cpamSetFlag(player, 0);
        SetPlayerControlEnabled(1);
        break;
    }
    FightCameraSubtract(bone, fightCameraEye, offset);
    float alpha = FightCameraLength(offset) - 0.2f;
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    player->alpha = alpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t LerpInteger(std::int32_t target, std::int32_t base, float amount) {
    return static_cast<std::int32_t>((static_cast<float>(target) - static_cast<float>(base)) * amount + static_cast<float>(base));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t LerpIntegerWrapper(std::int32_t target, std::int32_t base, float amount) {
    return LerpInteger(target, base, amount);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount) {
    const std::int16_t difference = static_cast<std::int16_t>(target - base);
    return static_cast<std::int16_t>(static_cast<float>(difference) * amount + static_cast<float>(base));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t CameraLerpAngleWrapper(std::int32_t target, std::int32_t base, float amount) {
    return CameraLerpAngle(target, base, amount);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int16_t CameraTransitionAngle(std::int32_t base, std::int32_t target, float amount) {
    if (::fabsf(amount) < FLT_EPSILON)
        return base;
    const float difference = static_cast<std::int16_t>(target - base);
    if (::fabsf(difference) < FLT_EPSILON)
        return base;
    return static_cast<std::int16_t>(static_cast<std::int32_t>(amount * difference) + base);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float CosineEaseValue(float base, float target, float amount) {
    if (::fabsf(target - base) < 1.1920929e-07f)
        return base;
    amount = (amount < 0.0f ? 0.0f : amount) < 1.0f ? (amount < 0.0f ? 0.0f : amount) : 1.0f;
    const std::int32_t angle = static_cast<std::int32_t>(((amount * 180.0f) * 8192.0f) / 45.0f);
    const float result = base + ((target - base) * (1.0f - dc::sdk::CosFixed(static_cast<std::uint16_t>(angle)))) * 0.5f;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float SineEaseValue(float base, float target, float amount) {
    if (::fabsf(target - base) < 1.1920929e-07f)
        return base;
    float t = amount;
    t = (t < 0.0f ? 0.0f : t) < 1.0f ? (t < 0.0f ? 0.0f : t) : 1.0f;
    float ease;
    if (t < 0.5f)
        ease = dc::sdk::SinFixed(static_cast<std::uint16_t>(static_cast<std::int32_t>(((t * 180.0f) * 8192.0f) / 45.0f)));
    else
        ease = 2.0f - dc::sdk::SinFixed(static_cast<std::uint16_t>(static_cast<std::int32_t>(((t * 180.0f) * 8192.0f) / 45.0f)));
    const float result = base + ((target - base) * ease) * 0.5f;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float TangentEaseValue(float base, float target, float amount, float steepness) {
    if (::fabsf(target - base) < 1.1920929e-07f)
        return base;
    amount = (amount < 0.0f ? 0.0f : amount) < 1.0f ? (amount < 0.0f ? 0.0f : amount) : 1.0f;
    steepness = (steepness < 0.001f ? 0.001f : steepness) < 1.0f ? (steepness < 0.001f ? 0.001f : steepness) : 1.0f;
    if (steepness == 1.0f)
        steepness = 0.9999f;
    const float range = steepness * 16384.0f;
    const float tangent = dc::sdk::TanFixed(static_cast<std::int16_t>(range));
    return static_cast<float>(static_cast<std::int32_t>(dc::sdk::ArcTanFixed(tangent * amount))) / range;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float SmoothStepEaseValue(float base, float target, float amount) {
    if (::fabsf(target - base) < 1.1920929e-07f)
        return base;
    amount = (amount < 0.0f ? 0.0f : amount) < 1.0f ? (amount < 0.0f ? 0.0f : amount) : 1.0f;
    const float weight = 3.0f * amount * amount - 2.0f * amount * amount * amount;
    const float result = base + (target - base) * weight;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float CameraTransitionValue(float base, float target, float amount) {
    if (::fabsf(amount) < FLT_EPSILON)
        return base;
    const float difference = target - base;
    if (::fabsf(difference) < FLT_EPSILON)
        return base;
    return base + amount * difference;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlRetainPreviousPose(CameraRuntime &camera) {
    camera.position.x = camera.nextPosition.x;
    camera.position.y = camera.nextPosition.y;
    camera.position.z = camera.nextPosition.z;
    camera.target.x = camera.targetPosition.x;
    camera.target.y = camera.targetPosition.y;
    camera.target.z = camera.targetPosition.z;
    camera.rotation.x = camera.nextRotation.x;
    camera.rotation.y = camera.nextRotation.y;
    camera.rotation.z = camera.nextRotation.z;
    camera.targetFovAngle = camera.currentFovAngle;
    camera.targetOrbitDistance = camera.orbitDistance;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlResetDefaultState(CameraRuntime &camera) {
    camera.defaultState = -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlComputePreviousPositionNullsub(CameraRuntime &) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlComputeOrbitPosition(CameraRuntime &camera) {
    const float distance = camera.orbitDistance;
    if (distance < 0.0f)
        return;
    const std::int16_t pitch = static_cast<std::int16_t>(-camera.nextRotation.x);
    const std::int16_t yaw = camera.nextRotation.y;
    const float vertical = distance * CalculateFixedSine(pitch);
    const float horizontal = distance * CalculateFixedCosine(pitch);
    camera.nextPosition.x = CalculateFixedSine(yaw) * horizontal + camera.targetPosition.x;
    camera.nextPosition.y = vertical + camera.targetPosition.y;
    camera.nextPosition.z = CalculateFixedCosine(yaw) * horizontal + camera.targetPosition.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlComputeLookAtPosition(CameraRuntime &camera) {
    camera.nextPosition.x = camera.previousPosition.x;
    camera.nextPosition.y = camera.previousPosition.y;
    camera.nextPosition.z = camera.previousPosition.z;
    const float x = camera.targetPosition.x - camera.nextPosition.x;
    const float y = camera.targetPosition.y - camera.nextPosition.y;
    const float z = camera.targetPosition.z - camera.nextPosition.z;
    camera.nextRotation.x = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(y, ::sqrtf(x * x + z * z)));
    camera.nextRotation.y = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(x, z)) - 0x8000);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlComputeAnchorPosition(CameraRuntime &camera) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(camera.anchorOffset.x, camera.anchorOffset.y, camera.anchorOffset.z);
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::preTranslate(0.0f, 0.0f, -camera.zOffset);
    matrix_stack::GetTranslation(&camera.nextPosition);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlComputeAimedAnchorPosition(CameraRuntime &camera) {
    const float x = camera.targetPosition.x - camera.anchorOffset.x;
    const float y = camera.targetPosition.y - camera.anchorOffset.y;
    const float z = camera.targetPosition.z - camera.anchorOffset.z;
    camera.nextRotation.x = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(y, shenmue::NativeSqrt(x * x + z * z)));
    camera.nextRotation.y = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(x, z)) - 0x8000);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(camera.anchorOffset.x, camera.anchorOffset.y, camera.anchorOffset.z);
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::preTranslate(0.0f, 0.0f, -camera.zOffset);
    matrix_stack::GetTranslation(&camera.nextPosition);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlComputeTargetFromRotation(CameraRuntime &camera) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(camera.nextPosition.x, camera.nextPosition.y, camera.nextPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::RotateAroundZ_Wrapper(camera.nextRotation.z);
    Matrix4x4::preTranslate(0.0f, 0.0f, -camera.orbitDistance);
    matrix_stack::GetTranslation(&camera.targetPosition);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateCameraTransitions(CameraRuntime &camera) {
    CameraRuntime easedCamera;
    CameraRuntime steppedCamera;
    float progress;
    switch (camera.interpolationMode) {
    case 1:
        progress = InterpolateTo(1.0f, camera.interpolationElapsed, camera.interpolationSpeed);
        camera.interpolationElapsed = progress;
        if (progress > 0.99989998f) {
            camera.interpolationMode = 0;
            progress = 1.0f;
        }
        camera.nextPosition.x = InterpolateTo(camera.nextPosition.x, camera.interpolationEye.x, progress);
        camera.nextPosition.y = InterpolateTo(camera.nextPosition.y, camera.interpolationEye.y, progress);
        camera.nextPosition.z = InterpolateTo(camera.nextPosition.z, camera.interpolationEye.z, progress);
        camera.nextRotation.x = CameraLerpAngle(camera.nextRotation.x, camera.interpolationRotation.x, progress);
        camera.nextRotation.y = CameraLerpAngle(camera.nextRotation.y, camera.interpolationRotation.y, progress);
        camera.nextRotation.z = CameraLerpAngle(camera.nextRotation.z, camera.interpolationRotation.z, progress);
        camera.currentFovAngle = CameraLerpAngle(camera.currentFovAngle, camera.interpolationFov, progress);
        break;
    case 3:
        camera.interpolationPendingState = static_cast<std::int16_t>(camera.interpolationPendingState + 1);
        if (camera.interpolationPendingState >= camera.interpolationFrameCount) {
            camera.interpolationPendingState = camera.interpolationFrameCount;
            camera.interpolationMode = 0;
        }
        progress = static_cast<float>(camera.interpolationPendingState) / static_cast<float>(camera.interpolationFrameCount);
        camera.nextPosition.x = CameraTransitionValue(camera.interpolationEye.x, camera.nextPosition.x, progress);
        camera.nextPosition.y = CameraTransitionValue(camera.interpolationEye.y, camera.nextPosition.y, progress);
        camera.nextPosition.z = CameraTransitionValue(camera.interpolationEye.z, camera.nextPosition.z, progress);
        camera.nextRotation.x = CameraTransitionAngle(camera.interpolationRotation.x, camera.nextRotation.x, progress);
        camera.nextRotation.y = CameraTransitionAngle(camera.interpolationRotation.y, camera.nextRotation.y, progress);
        camera.nextRotation.z = CameraTransitionAngle(camera.interpolationRotation.z, camera.nextRotation.z, progress);
        camera.currentFovAngle = CameraTransitionAngle(camera.interpolationFov, camera.currentFovAngle, progress);
        break;
    case 7: {
        if (camera.interpolationSourceMode != 2) {
            camera.interpolationMode = 0;
            return;
        }
        progress = InterpolateTo(1.0f, camera.interpolationElapsed, camera.interpolationSpeed);
        camera.interpolationElapsed = progress;
        if (progress > 0.99989998f) {
            camera.interpolationMode = 0;
            progress = 1.0f;
        }
        easedCamera = camera;
        easedCamera.targetPosition.x = InterpolateTo(camera.targetPosition.x, camera.interpolationTarget.x, progress);
        easedCamera.targetPosition.y = InterpolateTo(camera.targetPosition.y, camera.interpolationTarget.y, progress);
        easedCamera.targetPosition.z = InterpolateTo(camera.targetPosition.z, camera.interpolationTarget.z, progress);
        easedCamera.orbitDistance = InterpolateTo(camera.orbitDistance, camera.interpolationDistance, progress);
        easedCamera.nextRotation.x = CameraLerpAngle(camera.nextRotation.x, camera.interpolationRotation.x, progress);
        easedCamera.nextRotation.y = CameraLerpAngle(camera.nextRotation.y, camera.interpolationRotation.y, progress);
        easedCamera.nextRotation.z = CameraLerpAngle(camera.nextRotation.z, camera.interpolationRotation.z, progress);
        easedCamera.currentFovAngle = CameraLerpAngle(camera.currentFovAngle, camera.interpolationFov, progress);
        camCtrlComputeOrbitPosition(easedCamera);
        camera.nextPosition.x = easedCamera.nextPosition.x;
        camera.nextPosition.y = easedCamera.nextPosition.y;
        camera.nextPosition.z = easedCamera.nextPosition.z;
        camera.nextRotation.x = easedCamera.nextRotation.x;
        camera.nextRotation.y = easedCamera.nextRotation.y;
        camera.nextRotation.z = easedCamera.nextRotation.z;
        camera.currentFovAngle = easedCamera.currentFovAngle;
        break;
    }
    case 8: {
        if (camera.interpolationSourceMode != 2) {
            camera.interpolationMode = 0;
            return;
        }
        camera.interpolationPendingState = static_cast<std::int16_t>(camera.interpolationPendingState + 1);
        if (camera.interpolationPendingState >= camera.interpolationFrameCount) {
            camera.interpolationPendingState = camera.interpolationFrameCount;
            camera.interpolationMode = 0;
        }
        progress = static_cast<float>(camera.interpolationPendingState) / static_cast<float>(camera.interpolationFrameCount);
        steppedCamera = camera;
        steppedCamera.targetPosition.x = InterpolateTo(camera.targetPosition.x, camera.interpolationTarget.x, progress);
        steppedCamera.targetPosition.y = InterpolateTo(camera.targetPosition.y, camera.interpolationTarget.y, progress);
        steppedCamera.targetPosition.z = InterpolateTo(camera.targetPosition.z, camera.interpolationTarget.z, progress);
        steppedCamera.orbitDistance = InterpolateTo(camera.orbitDistance, camera.interpolationDistance, progress);
        steppedCamera.nextRotation.x = CameraLerpAngle(camera.nextRotation.x, camera.interpolationRotation.x, progress);
        steppedCamera.nextRotation.y = CameraLerpAngle(camera.nextRotation.y, camera.interpolationRotation.y, progress);
        steppedCamera.nextRotation.z = CameraLerpAngle(camera.nextRotation.z, camera.interpolationRotation.z, progress);
        steppedCamera.currentFovAngle = CameraLerpAngle(camera.currentFovAngle, camera.interpolationFov, progress);
        camCtrlComputeOrbitPosition(steppedCamera);
        camera.nextPosition.x = steppedCamera.nextPosition.x;
        camera.nextPosition.y = steppedCamera.nextPosition.y;
        camera.nextPosition.z = steppedCamera.nextPosition.z;
        camera.nextRotation.x = steppedCamera.nextRotation.x;
        camera.nextRotation.y = steppedCamera.nextRotation.y;
        camera.nextRotation.z = steppedCamera.nextRotation.z;
        camera.currentFovAngle = steppedCamera.currentFovAngle;
        break;
    }
    default:
        camera.interpolationMode = 0;
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define CAMERA_ABS(value) ((value) < 0 ? -(value) : (value))
// @verified - d
void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera) {
    if (!(camera.flags & 0x100U))
        return;
    if (camera.flags & 1U)
        return;
    if (CAMERA_ABS(camera.nextPosition.x - camera.currentPosition.x) < 0.0001f)
        camera.currentPosition.x = camera.nextPosition.x;
    if (CAMERA_ABS(camera.nextPosition.y - camera.currentPosition.y) < 0.0001f)
        camera.currentPosition.y = camera.nextPosition.y;
    if (CAMERA_ABS(camera.nextPosition.z - camera.currentPosition.z) < 0.0001f)
        camera.currentPosition.z = camera.nextPosition.z;
    if (CAMERA_ABS(camera.nextRotation.x - camera.currentRotation.x) < 2)
        camera.currentRotation.x = camera.nextRotation.x;
    if (CAMERA_ABS(camera.nextRotation.y - camera.currentRotation.y) < 2)
        camera.currentRotation.y = camera.nextRotation.y;
    if (CAMERA_ABS(camera.nextRotation.z - camera.currentRotation.z) < 2)
        camera.currentRotation.z = camera.nextRotation.z;
    if (CAMERA_ABS(camera.currentFovAngle - camera.currentFovInterpolationAngle) < 2)
        camera.currentFovInterpolationAngle = camera.currentFovAngle;
    camera.nextPosition.x = InterpolateTo(camera.nextPosition.x, camera.currentPosition.x, 0.23f);
    camera.nextPosition.y = InterpolateTo(camera.nextPosition.y, camera.currentPosition.y, 0.23f);
    camera.nextPosition.z = InterpolateTo(camera.nextPosition.z, camera.currentPosition.z, 0.23f);
    camera.nextRotation.x = CameraLerpAngle(camera.nextRotation.x, camera.currentRotation.x, 0.30000001f);
    camera.nextRotation.y = CameraLerpAngle(camera.nextRotation.y, camera.currentRotation.y, 0.30000001f);
    camera.nextRotation.z = CameraLerpAngle(camera.nextRotation.z, camera.currentRotation.z, 0.30000001f);
    camera.currentFovAngle = CameraLerpAngle(camera.currentFovAngle, camera.currentFovInterpolationAngle, 0.5f);
}
#undef CAMERA_ABS
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlApplyNextPositionOffsets(CameraRuntime &camera) {
    camera.nextPosition.x += camera.jitter.x;
    camera.nextPosition.y += camera.jitter.y;
    if (camera.rotationCallback)
        camera.rotationCallback();
    camera.nextPosition.x += camera.rotationOffset.x;
    camera.nextPosition.y += camera.rotationOffset.y;
    camera.nextPosition.z += camera.rotationOffset.z;
    camera.nextRotation.x = static_cast<std::int16_t>(camera.nextRotation.x + camera.angularOffset.x);
    camera.nextRotation.y = static_cast<std::int16_t>(camera.nextRotation.y + camera.angularOffset.y);
    camera.nextRotation.z = static_cast<std::int16_t>(camera.nextRotation.z + camera.angularOffset.z);
    ApplyRotationMatrixFromTransform(camera);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlComputeNextPosition(CameraRuntime &camera) {
    switch (camera.interpolationSourceMode) {
    case 1:
    default:
        camCtrlComputeLookAtPosition(camera);
        camera.nextTargetPosition.x = camera.targetPosition.x;
        camera.nextTargetPosition.y = camera.targetPosition.y;
        camera.nextTargetPosition.z = camera.targetPosition.z;
        break;
    case 0:
        camCtrlComputePreviousPositionNullsub(camera);
        camera.nextPosition.x = camera.previousPosition.x;
        camera.nextPosition.y = camera.previousPosition.y;
        camera.nextPosition.z = camera.previousPosition.z;
        break;
    case 2:
        camCtrlComputeOrbitPosition(camera);
        camera.previousPosition.x = camera.nextPosition.x;
        camera.previousPosition.y = camera.nextPosition.y;
        camera.previousPosition.z = camera.nextPosition.z;
        camera.nextTargetPosition.x = camera.targetPosition.x;
        camera.nextTargetPosition.y = camera.targetPosition.y;
        camera.nextTargetPosition.z = camera.targetPosition.z;
        break;
    case 3:
        camCtrlComputeAnchorPosition(camera);
        break;
    }
    UpdateCameraTransitions(camera);
    camCtrlCheckCollision(camera);
    camCtrlInterpolateNextPositionRotation(camera);
    camera.currentPosition.x = camera.nextPosition.x;
    camera.currentPosition.y = camera.nextPosition.y;
    camera.currentPosition.z = camera.nextPosition.z;
    camera.currentRotation.x = camera.nextRotation.x;
    camera.currentRotation.y = camera.nextRotation.y;
    camera.currentRotation.z = camera.nextRotation.z;
    camera.currentFovInterpolationAngle = camera.currentFovAngle;
    camCtrlApplyNextPositionOffsets(camera);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void CameraSplineProjectionNullsub(CameraRuntime &) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void CameraMatrixProjectionNullsub(CameraRuntime &) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlSetFlag40000000(std::int32_t mode) {
    CameraRuntime *camera = g_cameraControl;
    const std::int32_t previous = (camera->flags & 0x40000000U) != 0;
    if (mode < 0) {
        camera->flags ^= 0x40000000U;
        return previous;
    }
    if (mode)
        camera->flags |= 0x40000000U;
    else
        camera->flags &= ~0x40000000U;
    return previous;
}
#endif

// @verified - d
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position) {
    if (position) {
        ConversationCameraState *state = g_conversationCameraState;
        state->fixedPosition = *position;
        state->flags |= 0x400U;
    } else {
        g_conversationCameraState->flags &= ~0x400U;
    }
    return 0;
}

// @verified - d
std::int32_t IsConversationCameraShotEstablished() {
    ConversationCameraState *state = g_conversationCameraState;
    if (!(state->flags & 2U))
        return 0;
    if (state->flags & 0x8000000U)
        return 1;
    return 0;
}

// @verified - d
void srchSetupCameraFov() {
    g_searchCameraState.fovStyle = 0;
    g_searchCameraState.phase = 0;
    g_searchCameraState.lookBase.x = 0.0f;
    g_searchCameraState.lookBase.y = 0.0f;
    g_searchCameraState.lookBase.z = 0.0f;
    g_searchCameraState.rotationY = 0;
    g_searchCameraState.lockedOnAngle = 0.0f;
    g_searchCameraFov = 10922;
    g_searchCameraZoomFov = 5461;
    g_searchCameraZoomScale = -1.0f;
    g_searchCameraYaw = g_searchCameraPitch = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlSetConversationFlag20000000(std::int32_t enabled) {
    if (enabled)
        g_conversationCameraState->flags |= 0x20000000U;
    else
        g_conversationCameraState->flags &= ~0x20000000U;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsConversationCameraSequenceFinished() {
    ConversationCameraState *state = g_conversationCameraState;
    if (!(state->flags & 2U))
        return 1;
    if (state->flags & 0x40000000U)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ConversationCameraProjectionNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void CameraProjectionPostCallback(CameraRuntime &camera) {
    CameraSplineProjectionNullsub(camera);
    CameraMatrixProjectionNullsub(camera);
    ConversationCameraProjectionNullsub();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlProjectCameraMatrices(void *) {
    CameraRuntime &camera = *g_cameraControl;
    camCtrlRecalcFov(camera);
    camCtrlCreatePerspectiveMatrix(camera);
    camCtrlSetup(camera);
    const CameraUpdateCallback projectionCallback = *reinterpret_cast<CameraUpdateCallback *>(camera.retainedCallbackState);
    if (projectionCallback)
        projectionCallback(&camera);
    CameraProjectionPostCallback(camera);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlSetFloats(float position, float angle) {
    CameraRuntime *const camera = g_cameraControl;
    if (position == 0.0f && angle == 0.0f) {
        position = 0.23f;
        angle = 0.30000001f;
    }
    camera->positionInterpolationRate.x = position;
    camera->positionInterpolationRate.y = position;
    camera->positionInterpolationRate.z = position;
    camera->rotationInterpolationRate.x = angle;
    camera->rotationInterpolationRate.y = angle;
    camera->rotationInterpolationRate.z = angle;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void plymComputeConvTargetAnchor(Vector3 *anchor) {
    GetPlayerTargetWorldPosition(anchor);
    const float speed = plymGetSpeedZ();
    const float offset = speed < 1.0f ? 0.0f : (speed - 1.0f) * 0.3f + 0.2f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(anchor);
    Matrix4x4::RotateAroundY_Wrapper(plymGetAngY());
    Matrix4x4::preTranslate(0.0f, 0.0f, -offset);
    matrix_stack::GetTranslation(anchor);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void CameraScaleTo(Vector3 &value, float length) {
    Vector3::NormalizeInPlace(value);
    value = Vector3::Scale(value, length);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float CameraDistance(const Vector3 &first, const Vector3 &second) {
    const Vector3 delta = Vector3::Subtract(first, second);
    return Vector3::CalcMagnitude(delta);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void TransformIntersectionNormal(Vector3 *normal);

std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera) {
    Vector3 hit, position, target, normal, delta, start, precise;
    float distance = camera.orbitDistance;
    std::int32_t found = 0;
    float limit = distance;
    if (limit < 0.0f)
        distance = 0.1f;
    const float vertical = distance * CalculateFixedSine(-camera.nextRotation.x);
    const float horizontal = distance * CalculateFixedCosine(-camera.nextRotation.x);
    position.x = CalculateFixedSine(camera.nextRotation.y) * horizontal + camera.targetPosition.x;
    position.y = vertical + camera.targetPosition.y;
    position.z = CalculateFixedCosine(camera.nextRotation.y) * horizontal + camera.targetPosition.z;
    target.x = camera.targetPosition.x;
    target.y = camera.targetPosition.y;
    target.z = camera.targetPosition.z;
    if (camera.orbitDistance > 0.5f) {
        am2Vector3Subtract(&delta, &target, &position);
        dc::sdk::SetVectorLength(&delta, &delta, -0.05f);
        am2Vector3AddInPlace(&target, &delta);
    }
    if (FindNearestRayIntersection(&hit, &position, &target)) {
        if (CalculateDistanceBetweenPoints(&target, &hit) < 0.1f) {
            am2Vector3Subtract(&delta, &hit, &target);
            dc::sdk::SetVectorLength(&delta, &delta, 0.01f);
            am2Vector3Add(&start, &hit, &delta);
            if (FindNearestRayIntersection(&precise, &position, &start))
                hit = precise;
        }
        TransformIntersectionNormal(&normal);
        am2Vector3Subtract(&delta, &target, &hit);
        const std::int32_t angle = Vector3_ComputeAngleBetween(normal, delta);
        const float cosine = dc::sdk::SinFixed(0x4000 - angle);
        float amount = ::fabsf(cosine) >= FLT_EPSILON ? 0.2f / cosine : 0.0f;
        amount = amount < 0.3f ? amount : 0.3f;
        am2Vector3NormalizeInPlace(&delta);
        am2Vector3Scale(&delta, &delta, amount);
        am2Vector3Add(&position, &hit, &delta);
        found = 1;
    }
    if (!found) {
        Vector3 rightStart, leftStart, rightEnd, leftEnd;
        rightStart.x = 0.2f;
        rightStart.y = 0.0f;
        rightStart.z = 0.0f;
        leftStart.x = -0.2f;
        leftStart.y = 0.0f;
        leftStart.z = 0.0f;
        rightEnd.x = 0.2f;
        rightEnd.y = 0.0f;
        rightEnd.z = 0.0f;
        leftEnd.x = -0.2f;
        leftEnd.y = 0.0f;
        leftEnd.z = 0.0f;
        rightStart.z = leftStart.z = -distance;
        float leftDistance = distance;
        float rightDistance = distance;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&target);
        Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y + 0x8000);
        Matrix4x4::RotateAroundX_Wrapper(-camera.nextRotation.x);
        Matrix4x4::TransformPoint(rightStart, rightStart);
        Matrix4x4::TransformPoint(leftStart, leftStart);
        Matrix4x4::TransformPoint(rightEnd, rightEnd);
        Matrix4x4::TransformPoint(leftEnd, leftEnd);
        PopCurrentMatrix(1);
        if (FindNearestRayIntersection(&hit, &rightStart, &rightEnd)) {
            rightDistance = CalculateDistanceBetweenPoints(&rightEnd, &hit);
            if (rightDistance < 0.1f)
                rightDistance = distance;
        }
        if (FindNearestRayIntersection(&hit, &leftStart, &leftEnd)) {
            leftDistance = CalculateDistanceBetweenPoints(&leftEnd, &hit);
            if (leftDistance < 0.1f)
                leftDistance = distance;
        }
        if (distance > rightDistance || distance > leftDistance) {
            found = 1;
            limit = leftDistance > rightDistance ? rightDistance : leftDistance;
        }
    }
    if (found) {
        distance = CalculateDistanceBetweenPoints(&camera.targetPosition, &position);
        camera.orbitDistance = distance;
        if (distance > limit)
            camera.orbitDistance = limit;
    }
    return found;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlInterpolateApplyCameraAngY(CameraRuntime &camera) {
    const float distance = camera.orbitDistance;
    if (camera.flags & 0x200U) {
        camera.interpolationTicks = 8;
        return;
    }
    if (camera.orbitDistance > camera.targetOrbitDistance) {
        if (camera.interpolationTicks > 0) {
            camera.orbitDistance = InterpolateTo(distance, camera.targetOrbitDistance, 0.005f);
            if (((distance - camera.orbitDistance) < 0.0f ? -(distance - camera.orbitDistance) : (distance - camera.orbitDistance)) < 0.001f)
                camera.orbitDistance = distance;
        } else {
            camera.orbitDistance = InterpolateTo(distance, camera.targetOrbitDistance, 0.1f);
            if (((distance - camera.orbitDistance) < 0.0f ? -(distance - camera.orbitDistance) : (distance - camera.orbitDistance)) < 0.001f)
                camera.orbitDistance = distance;
        }
        if (--camera.interpolationTicks < 0)
            camera.interpolationTicks = 0;
        return;
    }
    if (static_cast<std::uint8_t>(camera.interpolationTicks - 1) <= 6U) {
        camera.orbitDistance = InterpolateTo(distance, camera.targetOrbitDistance, 0.1f);
        if (((distance - camera.orbitDistance) < 0.0f ? -(distance - camera.orbitDistance) : (distance - camera.orbitDistance)) < 0.001f)
            camera.orbitDistance = distance;
        ++camera.interpolationTicks;
        if (camera.interpolationTicks < 5)
            camera.interpolationTicks = 5;
    } else {
        camera.interpolationTicks = 8;
    }
    if (++camera.interpolationTicks > 7)
        camera.interpolationTicks = 8;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void camCtrlUpdateFollowCamera(CameraRuntime &camera) {
    if (camera.flags & 0x800000U) {
        if (camera.previousState != 7 || !camCtrlIsTransitionActive()) {
            camera.flags &= ~0x800000U;
        } else if (!plymIsLookAllowed(1)) {
            camCtrlSetInterpolationSpeed(-1.0f);
            camera.flags &= ~0x800000U;
        }
    }
    if (camera.previousState == 7 && camCtrlIsTransitionActive() && !plymIsLookAllowed(1))
        camCtrlSetInterpolationSpeed(-1.0f);
    camCtrlSetFloats(0.23f, 0.30000001f);
    if (camera.flags & 0x200U)
        camera.currentFovAngle = camera.fovAngle;
    else
        camera.currentFovAngle = CameraLerpAngle(camera.fovAngle, camera.targetFovAngle, 0.5f);
    if (!ctrlGetRootNode())
        return;
    const std::int16_t yaw = plymGetAngY();
    Vector3 anchor;
    plymComputeConvTargetAnchor(&anchor);
    camera.targetPosition.x = anchor.x;
    camera.targetPosition.y = anchor.y + camera.targetHeightDistance;
    camera.targetPosition.z = anchor.z;
    std::int16_t pitch = camera.basePitchBias;
    std::int16_t ground[2];
    camCtrlQueryPitchYaw(*reinterpret_cast<Vector3s *>(ground));
    std::int32_t groundPitch = ground[0];
    if (groundPitch < 0)
        groundPitch = -groundPitch;
    if (groundPitch > 546) {
        Vector3 forward;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(ground[1]);
        Matrix4x4::RotateAroundX_Wrapper(ground[0]);
        Matrix4x4::RotateAroundY_Wrapper(yaw - ground[1]);
        GetTransformedForwardVector(&forward);
        PopCurrentMatrix(1);
        Vector3_ToFixed16EulerAngles(forward, *reinterpret_cast<Vector3s *>(ground));
        pitch -= ground[0];
        camera.nextRotation.x = CameraLerpAngle(pitch, camera.nextRotation.x, 0.08f);
        camera.ticksRemaining = 10;
    } else {
        if (camera.ticksRemaining > 0)
            --camera.ticksRemaining;
        if (camera.ticksRemaining <= 0)
            camera.nextRotation.x = CameraLerpAngle(pitch, camera.nextRotation.x, 0.1f);
    }
    camera.nextRotation.y = CameraLerpAngle(yaw, camera.nextRotation.y, 0.30000001f);
    camera.nextRotation.z = 0;
    if (camera.flags & 0x200U) {
        camera.nextRotation.y = yaw;
        camera.nextRotation.x = pitch;
    }
    camera.orbitDistance = camera.defaultOrbitDistance;
    if (camera.nextRotation.x < 0) {
        const float rise = 2.6500001f - (camera.targetPosition.y - anchor.y);
        float heightLimit = rise / dc::sdk::SinFixed(-camera.nextRotation.x);
        if (heightLimit < 0.2f)
            heightLimit = 0.2f;
        if (camera.orbitDistance > heightLimit)
            camera.orbitDistance = heightLimit;
    }
    if (camera.contextMode == 10)
        camera.orbitDistance = 0.01f;
    camera.interpolationSourceMode = 2;
    camera.flags |= 0x500U;
    camCtrlGetIdealCameraPosition(camera);
    if (!(camera.flags & 8U))
        camCtrlInterpolateApplyCameraAngY(camera);
    if (camera.orbitDistance < 3.1f && camera.contextMode != 10) {
        float height = (3.1f - camera.orbitDistance) / 3.1f * 0.7f;
        height = height < 0.7f ? height : 0.7f;
        camera.targetPosition.y += height;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void camCtrlAutoCenterTargetInView(CameraRuntime &camera) {
    if (!ctrlGetRootNode())
        return;
    if (camera.flags & 0x800000U) {
        if (camera.previousState != 7 || !camCtrlIsTransitionActive()) {
            camera.flags &= ~0x800000U;
        } else if (!plymIsLookAllowed(1)) {
            camCtrlSetInterpolationSpeed(-1.0f);
            camera.flags &= ~0x800000U;
        }
    }
    if (camera.previousState == 7 && camCtrlIsTransitionActive() && !plymIsLookAllowed(1))
        camCtrlSetInterpolationSpeed(-1.0f);
    std::int16_t delta = plymGetAngY() - camera.nextRotation.y;
    if ((delta < 0 ? -delta : delta) > 0x3fff) {
        camera.collisionPhase = 2;
        camCtrlSetInterpolationMode(1);
        return;
    }
    if (camera.flags & 0x200U)
        camera.currentFovAngle = camera.fovAngle;
    else
        camera.currentFovAngle = CameraLerpAngle(camera.fovAngle, camera.targetFovAngle, 0.5f);
    Vector3 view;
    plymComputeConvTargetAnchor(&view);
    view.y += 1.6f;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&camera.projectionMatrix);
    Matrix4x4::TransformPoint(view, view);
    PopCurrentMatrix(1);
    if (view.x * view.x + view.z * view.z < 0.25f)
        return;
    if ((view.z < 0.0f ? -view.z : view.z) < 0.3f)
        return;
    if (view.z > 0.0f) {
        camera.collisionPhase = 2;
        camCtrlSetInterpolationMode(1);
        return;
    }
    std::int16_t quarter = camera.currentFovAngle / 4;
    std::int32_t angle = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(view.x / -view.z));
    if (angle < 0)
        angle = -angle;
    delta = angle - quarter;
    if (delta > 0) {
        if (view.x < 0.0f)
            camera.nextRotation.y += delta;
        else
            camera.nextRotation.y -= delta;
    }
    quarter = camera.verticalFovAngle / 4;
    angle = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(view.y / -view.z));
    if (angle < 0)
        angle = -angle;
    delta = angle - quarter;
    if (delta > 0) {
        if (view.y < 0.0f)
            camera.nextRotation.x -= delta;
        else
            camera.nextRotation.x += delta;
    }
    camera.interpolationSourceMode = 0;
    camera.flags |= 0x500U;
    Vector3 anchor;
    plymComputeConvTargetAnchor(&anchor);
    anchor.y += camera.targetHeightDistance;
    camCtrlSetTargetPosition(anchor);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlSwitchColiPhase(CameraRuntime &camera) {
    if (plymIntersectionTest()) {
        if (!camera.collisionPhase)
            camera.collisionPhase = 1;
    } else if (camera.collisionPhase) {
        camera.collisionPhase = 0;
    }
    if (camera.collisionPhase != 1) {
        if (camera.flags & 0x200000U) {
            const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
            const std::uint16_t triggerMask = GetNormalTriggerMapping() ? 0x80U : 0x40U;
            if (input->pressedPriority & triggerMask) {
                const std::int32_t index = camera.contextMode != 10 ? 11 : 5;
                camera.targetHeightDistance = cameraContextSettings[index].distanceFromPlayer;
                camera.basePitchBias = cameraContextSettings[index].basePitchBias;
                if (index != 11)
                    camera.contextMode = 0;
                else
                    camera.contextMode = 10;
                camera.flags |= 8U;
                camCtrlSetInterpolationMode(3);
                camCtrlSetInterpolationFrameCount(30);
            }
        }
        camCtrlUpdateFollowCamera(camera);
    } else {
        camCtrlAutoCenterTargetInView(camera);
    }
    if (camera.contextMode == 10) {
        float weight;
        const float current = ctrlGetLookOverlayWeight();
        if (camera.flags & 0x200000U) {
            weight = InterpolateTo(0.0f, current, 0.1f);
            if (weight < 0.001f)
                weight = 0.0f;
        } else
            weight = 0.0f;
        ctrlSetLookOverlayWeight(weight);
    }
}
#endif

// @verified - d
std::uint32_t shenApplyLockonFlag(std::uint32_t flags) {
    const std::uint32_t previous = flags & g_searchCameraState.flags;
    g_searchCameraState.flags |= flags;
    return previous;
}

// @verified - d
bool camCtrlSwitchToLockedOn(std::int32_t fovStyle) {
    SearchCameraState *state = &g_searchCameraState;
    const std::int32_t previous = camCtrlGetState();
    if (camCtrlGetState() != 24) {
        g_searchCameraZoomScale = -1.0f;
        g_searchCameraYaw = g_searchCameraPitch = 0;
        g_searchCameraFov = 10922;
        g_searchCameraZoomFov = 5461;
    }
    camCtrlSetState(24);
    state->phase = 1;
    state->fovStyle = fovStyle;
    state->flags = 0;
    if (previous == 7 && (camCtrlGetFlags() & 0x200U))
        state->flags |= 2U;
    return true;
}

// @verified - d
bool camCtrlSetLookAroundMode() {
    SearchCameraState *state = &g_searchCameraState;
    g_searchCameraZoomScale = -1.0f;
    g_searchCameraYaw = g_searchCameraPitch = 0;
    g_searchCameraFov = 10922;
    g_searchCameraZoomFov = 5461;
    if (camCtrlGetState() != 24)
        return false;
    camCtrlSetState(22);
    camCtrlSetInterpolationMode(1);
    state->fovStyle = 0;
    state->phase = 0;
    state->lookBase.x = 0.0f;
    state->lookBase.y = 0.0f;
    state->lookBase.z = 0.0f;
    state->rotationY = 0;
    state->lockedOnAngle = 0.0f;
    state->lookTarget.x = 0.0f;
    state->lookTarget.y = 0.0f;
    state->lookTarget.z = 0.0f;
    return true;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlSetCamSnapAngY(CameraRuntime &camera) {
    Vector3 position;
    camCtrlSetFloats(0.23f, 0.30000001f);
    Character *const target = ctrlGetScenarioChara(camera.targetCharacter);
    if (!target)
        return;
    GetCharacterWorldPosition(target, &position);
    camera.targetPosition.x = position.x;
    camera.targetPosition.y = position.y + camera.targetHeightDistance;
    camera.targetPosition.z = position.z;
    camera.orbitDistance = 3.1f;
    const std::int16_t yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(target));
    const std::int16_t pitch = camera.basePitchBias;
    camera.nextRotation.x = pitch;
    camera.nextRotation.y = yaw;
    camera.nextRotation.z = 0;
    camera.interpolationSourceMode = 2;
    camera.flags |= 0x500U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define CAMERA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CAMERA_MAX(a, b) ((a) < (b) ? (b) : (a))
#define CAMERA_ABS(value) ((value) < 0 ? -(value) : (value))

void camCtrlHandleLookAroundCam(CameraRuntime &camera) {
    Vector3 anchor;
    Vector2s base, look;
    Vector3 eyeTarget, target;
    float pitchRatio, zoom, zoomLift, zoomOrbit = 0.0f, heightLift, distance, blend, current, weight;
    std::int32_t inputState, zoomScale;
    std::int16_t previous, fov;
    bool restorePrevious = false;
    if (!ctrlGetRootNode())
        return;
    camera.interpolationSourceMode = 2;
    camera.flags |= 0x500U;
    if (plymIntersectionTest()) {
        if (camera.collisionPhase != 2) {
            camera.lookPitchDelta = 0;
            camera.lookYawDelta = 0;
            camCtrlSetState(0);
            camCtrlSetInterpolationMode(1);
            return;
        }
    } else if (camera.collisionPhase) {
        camera.collisionPhase = 0;
    }
    plymSampleLookInput(base, look);
    inputState = plymGetLookInputState();
    if (inputState == -1) {
        camera.lookPitchDelta = 0;
        camera.lookYawDelta = 0;
        camCtrlSetState(0);
        return;
    }
    if (inputState > 0) {
        camera.nextRotation.x = static_cast<std::int16_t>(base.x + look.x + camera.basePitchBias);
        camera.nextRotation.y = static_cast<std::int16_t>(base.y + look.y);
        camera.pitchBase = base.x;
        camera.yawBase = base.y;
        camera.lookPitchDelta = look.x;
        camera.lookYawDelta = look.y;
    } else {
        camera.lookPitchDelta = CameraLerpAngle(0, camera.lookPitchDelta, 0.08f);
        camera.lookYawDelta = CameraLerpAngle(0, camera.lookYawDelta, 0.08f);
        camera.pitchBase = camera.basePitchBias;
        camera.yawBase = static_cast<std::int16_t>(plymGetAngY());
        camera.nextRotation.x = static_cast<std::int16_t>(camera.pitchBase + camera.lookPitchDelta);
        camera.nextRotation.y = static_cast<std::int16_t>(camera.yawBase + camera.lookYawDelta);
        if (CAMERA_ABS(camera.lookPitchDelta) < 64 && CAMERA_ABS(camera.lookYawDelta) < 64) {
            camera.lookPitchDelta = 0;
            camera.lookYawDelta = 0;
            camCtrlSetState(0);
            restorePrevious = true;
        }
    }
    pitchRatio = static_cast<float>(camera.nextRotation.x) / 10922.0f;
    pitchRatio = CAMERA_MIN(CAMERA_MAX(pitchRatio, -1.0f), 1.0f);
    zoomScale = plymGetLookZoomScale();
    zoom = static_cast<float>(static_cast<std::uint8_t>(zoomScale)) * 0.00390625f;
    plymComputeConvTargetAnchor(&anchor);
    target.x = anchor.x;
    target.y = anchor.y + camera.targetHeightDistance;
    target.z = anchor.z;
    zoomLift = heightLift = 0.0f;
    if (camera.contextMode != 10) {
        zoomLift = zoom * 0.4f;
        if (camera.nextRotation.x < 0) {
            zoomLift *= 1.0f - CAMERA_ABS(pitchRatio);
            zoomLift += CAMERA_ABS(pitchRatio) * 0.7f;
        }
        target.y += zoomLift;
        camera.targetPosition.x = target.x;
        camera.targetPosition.y = target.y;
        camera.targetPosition.z = target.z;
        camera.orbitDistance = 3.1f;
        camCtrlGetIdealCameraPosition(camera);
        camCtrlInterpolateApplyCameraAngY(camera);
        if (camera.orbitDistance < 3.1f) {
            heightLift = (3.1f - camera.orbitDistance) / 3.1f * 0.7f;
            heightLift = CAMERA_MIN(heightLift, 0.7f);
            heightLift *= 1.0f - zoom;
            if (camera.nextRotation.x < 0)
                heightLift *= 1.0f - CAMERA_ABS(pitchRatio);
            target.y += heightLift;
        }
    }
    distance = camera.orbitDistance;
    if (camera.contextMode == 10)
        distance = 0.01f;
    eyeTarget = target;
    if (zoom > 0.0f) {
        zoomOrbit = 2.5f;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&target);
        Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
        Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
        Matrix4x4::preTranslate(0.0f, 0.0f, -zoomOrbit);
        matrix_stack::GetTranslation(&eyeTarget);
        PopCurrentMatrix(1);
    }
    if (camera.nextRotation.x < 0) {
        float nearest = 0.11f, farthest = 3.1f;
        const float limit = nearest + (farthest - nearest) * (1.0f - CAMERA_ABS(pitchRatio));
        if (distance > limit)
            distance = limit;
    } else if (camera.nextRotation.x > 0) {
        const float floor = anchor.y + 0.55f;
        if (floor > dc::sdk::SinFixed(-camera.nextRotation.x) * distance + target.y) {
            dc::sdk::SinFixed(camera.nextRotation.x);
            const float limit = (floor - target.y) / dc::sdk::SinFixed(-camera.nextRotation.x);
            if (distance > limit)
                distance = limit;
        }
    }
    camera.orbitDistance = (zoomOrbit + distance) * (1.0f - zoom);
    if (zoom > 0.0f)
        camera.flags &= ~0x400U;
    if (camera.flags & 0x8000000U) {
        previous = static_cast<std::int16_t>(camCtrlGetPerspective());
        fov = static_cast<std::int16_t>(camCtrlGetFov());
        if (camera.orbitDistance < 0.01f + zoomOrbit) {
            float amount = 0.01f + zoomOrbit - camera.orbitDistance;
            camera.orbitDistance = 0.01f + zoomOrbit;
            amount /= 0.01f + zoomOrbit;
            fov = static_cast<std::int16_t>(fov - static_cast<std::int32_t>((static_cast<float>(fov) - 5461.0f) * amount));
        }
        if (::fabsf(zoom) >= FLT_EPSILON)
            fov = CameraLerpAngle(fov, previous, 0.5f);
        else
            fov = CameraLerpAngle(fov, previous, 0.25f);
        camCtrlSetPerspective(fov);
    }
    blend = zoom / (distance / (zoomOrbit + distance));
    blend = CAMERA_MIN(CAMERA_MAX(blend, 0.0f), 1.0f);
    eyeTarget.y -= zoomLift + heightLift;
    zoomLift = blend * 0.4f;
    heightLift = 0.0f;
    if (camera.nextRotation.x < 0) {
        zoomLift *= 1.0f - CAMERA_ABS(pitchRatio);
        zoomLift += CAMERA_ABS(pitchRatio) * 0.7f;
    }
    if (distance < 3.1f) {
        heightLift = (3.1f - distance) / 3.1f * 0.7f;
        heightLift = CAMERA_MIN(heightLift, 0.7f);
        heightLift *= 1.0f - blend;
        if (camera.nextRotation.x < 0)
            heightLift *= 1.0f - CAMERA_ABS(pitchRatio);
    }
    if (camera.contextMode == 10)
        zoomLift = heightLift = 0.0f;
    eyeTarget.y += zoomLift + heightLift;
    camera.targetPosition.x = eyeTarget.x;
    camera.targetPosition.y = eyeTarget.y;
    camera.targetPosition.z = eyeTarget.z;
    current = ctrlGetLookOverlayWeight();
    weight = 1.0f;
    if (zoom > 0.0f) {
        weight = (camera.orbitDistance - zoomOrbit) / 3.1f;
        weight = CAMERA_MIN(CAMERA_MAX(weight, 0.0f), 1.0f);
    }
    if (current > weight) {
        weight = InterpolateTo(weight, current, 0.4f);
        if (weight < 0.001f)
            weight = 0.0f;
    } else {
        weight = InterpolateTo(weight, current, 0.1f);
        if (weight > 0.999f)
            weight = 1.0f;
    }
    if (camera.contextMode == 10)
        weight = 0.0f;
    ctrlSetLookOverlayWeight(weight);
    if (restorePrevious)
        camCtrlSetInterpolationMode(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchSetFovStyle(std::int32_t fovStyle) {
    const std::int32_t previous = g_searchCameraState.fovStyle;
    g_searchCameraState.fovStyle = fovStyle;
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static float SearchCameraEase(const SearchCameraState &state) {
    Vector3 position;
    camCtrlGetNextCamPos(&position);
    position.x -= state.cameraPosition.x;
    position.y -= state.cameraPosition.y;
    position.z -= state.cameraPosition.z;
    float amount = NativeSqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    amount = 1.0f - 0.13600001f * amount;
    if (0.25f > amount)
        amount = 0.25f;
    return amount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlSetInterpolationSourceMode(std::uint8_t mode);

static void ResetLockedOnCameraPosition(SearchCameraState &state) {
    state.cameraPosition = state.lookTarget;
    camCtrlSetInterpolationSourceMode(1);
    camCtrlSetNextPosition(state.cameraPosition);
    camCtrlSetTargetPosition(state.lookBase);
    camCtrlSetPerspective(10922);
    camCtrlSetInterpolationMode(1);
    camCtrlSetInterpolationSpeed(SearchCameraEase(state));
    state.distance = 7.5f;
    state.retained44[0] = 7.5f;
    state.retained44[1] = 0.0f;
    state.retained3a[0] = 0;
    state.retained3a[1] = 0;
    state.retained3a[2] = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t camCtrlOrFlags(std::uint32_t mask);

void UpdateLockedOnCamera() {
    SearchCameraState &state = g_searchCameraState;
    camCtrlOrFlags(0x100U);
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    if (!input)
        return;
    switch (state.phase) {
    case 0:
        break;
    case 1: {
        Vector3 target = state.lookTarget;
        const float vertical = target.y - state.lookBase.y;
        float radius;
        if (static_cast<std::uint32_t>(state.fovStyle - 2) <= 3U) {
            float delta = target.x - state.lookBase.x;
            radius = vertical * vertical + delta * delta;
            delta = target.z - state.lookBase.z;
            radius += delta * delta;
            radius = NativeSqrt(radius);
            if (state.fovStyle == 5) {
                g_searchCameraFov = 10012;
                g_searchCameraZoomFov = 5461;
            } else {
                float height = (NativeSqrt(radius) - 2.0f) * 0.3f;
                float minimum;
                switch (state.fovStyle) {
                case 2:
                    minimum = 0.7f;
                    break;
                case 4:
                    minimum = 1.15f;
                    break;
                default:
                    minimum = 0.85f;
                    break;
                }
                if (height < minimum)
                    height = minimum;
                g_searchCameraFov = dc::sdk::ArcTan2Fixed(height * 0.57735002f, radius);
                if (static_cast<std::int32_t>(g_searchCameraFov) < 3640)
                    g_searchCameraFov = 3640;
                g_searchCameraZoomFov = dc::sdk::ArcTan2Fixed(0.28867501f, radius);
                if (static_cast<std::int32_t>(g_searchCameraZoomFov) < 1820)
                    g_searchCameraZoomFov = 1820;
            }
        } else {
            radius = 0.75f;
            g_searchCameraFov = 10922;
            g_searchCameraZoomFov = 5461;
        }
        std::int32_t pitch = dc::sdk::ArcTan2Fixed(vertical, radius);
        pitch &= 0xffff;
        if ((state.flags & 1U) == 0) {
            if (vertical > 0.0f && pitch > 3640)
                pitch = 3640;
            else if (vertical < 0.0f && pitch < 61895)
                pitch = 61895;
        }
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&state.lookBase);
        Matrix4x4::RotateAroundY_Wrapper(state.rotationY);
        Matrix4x4::RotateAroundX_Wrapper(pitch);
        Matrix4x4::preTranslate(0.0f, 0.0f, -radius);
        matrix_stack::GetTranslation(&state.cameraPosition);
        Matrix4x4::preTranslate(0.0f, 0.0f, -7.5f);
        matrix_stack::GetTranslation(&state.offsetPosition);
        PopCurrentMatrix(1);
        state.cameraPosition = state.lookTarget;
        camCtrlSetNextPosition(state.cameraPosition);
        camCtrlSetTargetPosition(state.lookBase);
        if (state.flags & 2U)
            camCtrlSetFovTransitionTo(1);
        else
            camCtrlSetInterpolationMode(1);
        camCtrlSetPerspective(10922);
        if (static_cast<std::uint32_t>(state.fovStyle - 2) <= 3U)
            camCtrlSetInterpolationSpeed(SearchCameraEase(state));
        else
            camCtrlSetInterpolationSpeed(0.25f);
        state.distance = 7.5f;
        state.retained44[0] = 7.5f;
        state.retained44[1] = 0.0f;
        state.retained3a[0] = 0;
        state.retained3a[1] = 0;
        state.retained3a[2] = 0;
        state.phase = 2;
        break;
    }
    case 2:
        camCtrlSetNextPosition(state.cameraPosition);
        state.phase = 3;
        camCtrlGetRot(&state.initialRotation);
        break;
    case 3:
        if (state.fovStyle == 6)
            ResetLockedOnCameraPosition(state);
        break;
    }
    if (state.phase) {
        Vector3 pivot = state.lookTarget;
        Vector3 position, target;
        std::int32_t yawTarget, angle;
        if (g_searchCameraZoomScale < 0.0f) {
            angle = 0;
            yawTarget = 0;
        } else {
            yawTarget = (128 - input->leftStickX) * 128;
            angle = (128 - input->leftStickY) * 10922 / 128;
        }
        g_searchCameraYaw = LerpInteger(yawTarget, g_searchCameraYaw, 0.06f) & 0xffff;
        g_searchCameraPitch = LerpInteger(angle, g_searchCameraPitch, 0.06f) & 0xffff;
        if (g_searchCameraYaw & 0x8000)
            g_searchCameraYaw |= 0xffff0000;
        if (g_searchCameraPitch & 0x8000)
            g_searchCameraPitch |= 0xffff0000;
        float amount;
        if (static_cast<std::uint32_t>(state.fovStyle - 6) <= 1U)
            amount = GetNormalTriggerMapping() ? input->triggers[1] : input->triggers[0];
        else
            amount = plymGetLookZoomScale() * 0.00390625f;
        if (amount < 0.0f)
            amount = 0.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        if (g_searchCameraZoomScale < 0.0f) {
            g_searchCameraZoomScale = -0.001f;
            if (amount < 0.04f)
                g_searchCameraZoomScale = 0.0f;
        } else {
            g_searchCameraZoomScale = InterpolateTo(amount, g_searchCameraZoomScale, 0.2f);
        }
        target = state.lookBase;
        position = state.cameraPosition;
        if (state.lockedOnAngle > 0.0f) {
            Vector3 delta;
            delta.x = position.x - target.x;
            delta.y = position.y - target.y;
            delta.z = position.z - target.z;
            amount = Vector3::CalcMagnitude(delta);
            const float maximum = state.lockedOnAngle / dc::sdk::TanFixed(5461);
            float factor;
            if (amount > maximum)
                factor = 1.0f;
            else
                factor = 1.0f - g_searchCameraZoomScale;
            factor *= (maximum - amount) / amount;
            position.x += factor * delta.x;
            position.y += factor * delta.y;
            position.z += factor * delta.z;
            pivot = position;
        }
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&pivot);
        Matrix4x4::RotateAroundY_Wrapper(g_searchCameraYaw);
        matrix_stack::Matrix4x4_Push(0);
        float dx, dy, dz;
        dx = position.x - pivot.x;
        dy = position.y - pivot.y;
        dz = position.z - pivot.z;
        const float sine = dc::sdk::SinFixed(g_searchCameraPitch);
        const float cosine = dc::sdk::CosFixed(g_searchCameraPitch);
        float distance = NativeSqrt(dy * dy + dx * dx + dz * dz);
        Matrix4x4::preTranslate(cosine * dx, dy, cosine * dz);
        matrix_stack::GetTranslation(&position);
        position.y += sine * distance;
        if (position.y < pivot.y - 1.5f)
            position.y = pivot.y - 1.5f;
        PopCurrentMatrix(1);
        dx = target.x - pivot.x;
        dy = target.y - pivot.y;
        dz = target.z - pivot.z;
        distance = NativeSqrt(dy * dy + dx * dx + dz * dz);
        Matrix4x4::preTranslate(cosine * dx, dy, cosine * dz);
        matrix_stack::GetTranslation(&target);
        target.y += sine * distance;
        PopCurrentMatrix(1);
        std::int32_t fov = static_cast<std::int32_t>(g_searchCameraFov);
        angle = fov - g_searchCameraZoomFov;
        if (g_searchCameraZoomScale > 0.0f)
            fov -= static_cast<std::int32_t>(angle * g_searchCameraZoomScale);
        camCtrlSetPerspective(static_cast<std::int16_t>(fov));
        if (state.fovStyle != 6) {
            camCtrlSetNextPosition(position);
            camCtrlSetTargetPosition(target);
        }
    } else {
        g_searchCameraZoomScale = -1.0f;
        g_searchCameraYaw = g_searchCameraPitch = 0;
    }
    if (static_cast<std::uint32_t>(state.fovStyle - 2) <= 3U && state.phase) {
        float weight = ctrlGetLookOverlayWeight() - 0.1f;
        if (weight < 0.0f)
            weight = 0.0f;
        ctrlSetLookOverlayWeight(weight);
    }
    state.flags &= ~2U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlCallback(void *) {
    CameraRuntime &camera = *g_cameraControl;
    camCtrlRetainPreviousPose(camera);
    camera.interpolationSourceMode = 1;
    camera.flags &= ~0x100U;
    camera.flags &= ~0x400U;
    camera.flags &= ~0x80U;
    camera.flags &= ~8U;
    if (static_cast<std::int8_t>(camera.fovLerpIterations) > 0) {
        --camera.fovLerpIterations;
        camera.flags |= 0x200U;
    }
    camCtrlResetDefaultState(camera);
    camCtrlHandleStates(camera);
    if (camera.flags & 1U)
        camera.flags &= ~0x100U;
    if (camera.flags & 0x200U) {
        camera.flags &= ~0x200U;
        camera.flags &= ~0x100U;
        if (camera.interpolationMode)
            camera.interpolationMode = 0;
    }
    camCtrlApplyJitterAdjustment(camera);
    camCtrlSmoothMotion(camera);
    camCtrlComputeNextPosition(camera);
    camCtrlApply(camera);
    HLib::ExecuteCallbackNow(camCtrlProjectCameraMatrices, &camera);
    PublishGlobalLighting();
    camCtrlSetupDepthScaling();
    camCtrlSubmitFieldDepth();
    camera.flags &= ~1U;
}
#endif

// @verified - d
void srchSetLookBase(const Vector3 *base) {
    SearchCameraState *state = &g_searchCameraState;
    state->lookBase = *base;
}

// @verified - d
void srchGetLookBase(Vector3 *base) {
    SearchCameraState *state = &g_searchCameraState;
    if (base)
        *base = state->lookBase;
}

// @verified - d
void srchSetRotY(std::int16_t yaw) {
    g_searchCameraState.rotationY = yaw;
}

// @verified - d
void camCtrlSetLockedOnAngle(float angle) {
    g_searchCameraState.lockedOnAngle = angle;
}

// @verified - d
void srchSetLookTarget(const Vector3 *target) {
    SearchCameraState *state = &g_searchCameraState;
    if (target)
        state->lookTarget = *target;
    else {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterWorldPosition(control->ryo, &state->lookTarget);
        state->lookTarget.y += 1.6f;
    }
}

// @verified - d
void srchGetLookTarget(Vector3 *target) {
    SearchCameraState *state = &g_searchCameraState;
    if (target)
        *target = state->lookTarget;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AccumulateFightCameraVelocity(FightCameraVelocity *velocity, FightRuntimePrefix *game, FightActorRuntimePrefix *self) {
    std::int32_t closeEnemy;
    const std::int32_t playerSlot = game->playerSlot;
    Vector3 base, sum, delta;
    base.x = self->contactPosition.x;
    base.y = self->targetGroundY;
    base.z = self->contactPosition.z;
    float distanceSum, divisor;
    distanceSum = sum.x = sum.y = sum.z = 0.0f;
    divisor = 1;
    closeEnemy = 0;
    for (std::int32_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || i == playerSlot)
            continue;
        FightActorRuntimePrefix *const other = game->actors[i];
        delta.x = other->contactPosition.x - base.x;
        delta.y = other->targetGroundY - base.y;
        delta.z = other->contactPosition.z - base.z;
        float distance = delta.x * delta.x + delta.z * delta.z;
        float vertical = delta.y;
        if (vertical < 0.0f)
            vertical = -vertical;
        if (!(cameraMotionDoubleDistanceSquared > distance) || !(cameraMotionSecondaryDoubleDistance > vertical))
            continue;
        float weight = 1;
        distance = NativeSqrt(distance);
        if (distance > cameraMotionDistance)
            weight *= (cameraMotionDoubleDistance - distance) / cameraMotionDistance;
        if (vertical > cameraMotionSecondaryDistance)
            weight *= (cameraMotionSecondaryDoubleDistance - vertical) / cameraMotionSecondaryDistance;
        std::int32_t enemy = 0;
        if (game->slotTeams[i] == 1 || game->slotTeams[i] == 3)
            enemy = 1;
        if (!enemy)
            weight *= 0.5f;
        sum.x += (weight * delta.x) * weight;
        sum.y += (weight * delta.y) * weight;
        sum.z += (weight * delta.z) * weight;
        distanceSum += (distance * weight) * weight;
        divisor += weight;
        if (cameraMotionDistance * 1.4f > distance && enemy)
            closeEnemy = 1;
    }
    if (!(0.00000011920929f > ::fabsf(divisor))) {
        sum.x /= divisor;
        sum.y /= divisor;
        sum.z /= divisor;
    }
    velocity->sum.x = sum.x;
    velocity->sum.y = sum.y;
    velocity->sum.z = sum.z;
    velocity->distance = distanceSum;
    velocity->divisor = divisor;
    velocity->closeEnemy = closeEnemy;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind) {
    FightRuntimePrefix *const game = actor->game;
    fightCameraLinkedCharacter = actor->character;
    fightCameraLinkedActor = actor;
    SetFightCameraLinkedKind(kind);
    if (cameraMotionMode != 8)
        game->stateFlags |= 1U;
    float scale;
    if (*reinterpret_cast<std::int8_t *>(fightCameraLinkedActor) == game->playerSlot)
        scale = 1.0f;
    else
        scale = 0.2f;
    if (fightCameraLinkedKind == 3) {
        SetBlurModel(fightCameraLinkedCharacter, 0, scale);
        EV_figpAssignFlagsFor(fightCameraLinkedCharacter, 0, 1);
        figpAssignMotScaleAndDirFor(fightCameraLinkedCharacter, 5, 2);
        figpAssignModeForCharacter(fightCameraLinkedCharacter, 1);
    } else {
        if (cameraMotionMode == 8 || cameraMotionMode == 4)
            mtwkSetUseEnvFlag(0);
        else
            mtwkSetUnknownField(fightCameraLinkedActor->motion, 0);
        if (*reinterpret_cast<std::int8_t *>(fightCameraLinkedActor) != game->playerSlot)
            SetBlurModel(fightCameraLinkedCharacter, 0, scale);
        EV_figpAssignFlagsFor(fightCameraLinkedCharacter, 0, 1);
        figpAssignMotScaleAndDirFor(fightCameraLinkedCharacter, 5, 1);
        figpAssignModeForCharacter(fightCameraLinkedCharacter, 1);
    }
    fightCameraHasIntersectionTest = 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void vfUnknownCamRelated(FightRuntimePrefix *game) {
    const std::uint8_t mode = static_cast<std::uint8_t>(cameraMotionMode);
    if ((mode == 4 || mode == 8) && fightCameraLinkedCharacter) {
        figpAssignModeForCharacter(fightCameraLinkedCharacter, 0);
        EV_figpAssignFlagsFor(fightCameraLinkedCharacter, 0, 0);
        if (mode == 8 || mode == 4)
            mtwkSetUseEnvFlag(1);
        else {
            mtwkSetUnknownField(charFindMotionwork(fightCameraLinkedCharacter), 1);
            game->stateFlags &= ~1U;
        }
        camCtrlSetInterpModeTo(0);
        resetIntersectionAndTest();
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetFightCameraToParticipants(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other) {
    Vector3 direction;
    if (!self || !other || self == other)
        return;
    fightCameraTarget.x = self->contactPosition.x;
    fightCameraTarget.y = self->targetGroundY + 1.4f;
    fightCameraTarget.z = self->contactPosition.z;
    direction.x = other->contactPosition.x - fightCameraTarget.x;
    direction.y = 0.0f;
    direction.z = other->contactPosition.z - fightCameraTarget.z;
    ScaleFightCameraVector(direction, (60.0f / (static_cast<float>(camCtrlGetPerspective()) / 182.04445f)) * 1.5f);
    fightCameraEye.x = fightCameraTarget.x + direction.x;
    fightCameraEye.y = self->targetGroundY + 1.3f;
    fightCameraEye.z = fightCameraTarget.z + direction.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ResetFightCameraPattern() {
    fightCameraPattern.flags = fightCameraPattern.frame = fightCameraPattern.state = 0;
    fightCameraPattern.phaseX = fightCameraPattern.phaseY = 0.0f;
    fightCameraPattern.speed = 160.0f / (static_cast<float>(camCtrlGetPerspective()) / 182.04445f);
    fightCameraPattern.roll = fightCameraPattern.goalRoll = 0;
    fightCameraPattern.previousIndex = fightCameraPattern.index = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateFightCameraPattern(FightRuntimePrefix *game) {
    if (static_cast<std::uint32_t>(game->state) > 3 || game->elapsedTicks < 60) {
        camCtrlSetNextRoll(0);
        return;
    }
    if (fightCameraLinkedKind != 0 || static_cast<std::int32_t>(game->stateFlags) < 0)
        return;
    float perspective = static_cast<float>(camCtrlGetPerspective()) / 182.04445f;
    if (perspective < 60)
        perspective -= (60 - perspective) * 0.5f;
    perspective = perspective < 10 ? 10 : perspective > 90 ? 90 : perspective;
    FightCameraPatternState *const pattern = &fightCameraPattern;
    if (pattern->frame == pattern->frame / 90 * 90) {
        pattern->previousIndex = pattern->index;
        pattern->index = fightCameraPatterns[cameraMotionPatternSet][static_cast<std::int32_t>(dc::sdk::RandomFloat() * 8)];
        pattern->previousVariant = pattern->variant;
        pattern->variant = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 4) % 2;
        switch (pattern->index) {
        case 1:
        case 6:
        case 7:
            fightCameraPattern.variant = 0;
            break;
        }
    }
    if (fightCameraPattern.index != fightCameraPattern.previousIndex) {
        float step = fightCameraPhaseTargets[fightCameraPattern.index][0];
        step -= fightCameraPattern.phaseX;
        if (step < -0.25f)
            step = -0.25f;
        else if (step > 0.25f)
            step = 0.25f;
        fightCameraPattern.phaseX += step;
        step = fightCameraPhaseTargets[fightCameraPattern.index][1];
        step -= fightCameraPattern.phaseY;
        if (step < -0.25f)
            step = -0.25f;
        else if (step > 0.25f)
            step = 0.25f;
        fightCameraPattern.phaseY += step;
    }
    if (fightCameraPattern.variant != fightCameraPattern.previousVariant) {
        float speed = fightCameraPatternSpeeds[cameraMotionPatternSet][fightCameraPattern.variant] / perspective;
        if (speed > 9.5f)
            speed = 9.5f;
        float step = speed - fightCameraPattern.speed;
        if (step < -0.25f)
            step = -0.25f;
        else if (step > 0.25f)
            step = 0.25f;
        fightCameraPattern.speed += step;
    }
    if (fightCameraPattern.goalRoll != fightCameraPattern.roll) {
        if (fightCameraPattern.roll > fightCameraPattern.goalRoll + 64)
            fightCameraPattern.roll -= 64;
        else if (fightCameraPattern.roll < fightCameraPattern.goalRoll - 64)
            fightCameraPattern.roll += 64;
        camCtrlSetNextRoll(static_cast<std::int16_t>(fightCameraPattern.roll));
    }
    ++fightCameraPattern.frame;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlShakeFightCamera(float amount) {
    camCtrlSetShakeAmplitude(0.0f, amount, 0.5f);
}
#endif

// @verified - d
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action) {
    const std::uint32_t previous = g_combatCameraFlags & mask;
    if (!action)
        g_combatCameraFlags &= ~mask;
    else if (action > 0)
        g_combatCameraFlags |= mask;
    return previous;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre) {
    point.x = point.x < centre.x - 0.05f ? centre.x - 0.05f : point.x > centre.x + 0.05f ? centre.x + 0.05f : point.x;
    point.y = point.y < centre.y - 0.1f ? centre.y - 0.1f : point.y > centre.y + 0.1f ? centre.y + 0.1f : point.y;
    point.z = point.z < centre.z - 0.05f ? centre.z - 0.05f : point.z > centre.z + 0.05f ? centre.z + 0.05f : point.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t FightCameraYawBetween(float x1, float z1, float x2, float z2) {
    return static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(x1 - x2, z1 - z2));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction, std::int32_t collide) {
    Vector3 delta, position;
    delta.x = (desired.x - retained.x) / 5.0f;
    delta.y = (desired.y - retained.y) / 5.0f;
    delta.z = (desired.z - retained.z) / 5.0f;
    const float distance = FightCameraLength(delta);
    if (::fabsf(distance) >= FLT_EPSILON) {
        float velocity = ((direction.x * delta.x + direction.y * delta.y) + direction.z * delta.z) / distance;
        if ((velocity > 0.0f && distance <= 0.0f) || (velocity <= 0.0f && distance > 0.0f))
            velocity = 0.0f;
        const float nextVelocity = velocity + 0.032000002f;
        if ((nextVelocity < 0.0f ? -nextVelocity : nextVelocity) < (distance < 0.0f ? -distance : distance))
            ScaleFightCameraVector(delta, velocity + 0.032000002f);
        position.x = retained.x + delta.x;
        position.y = retained.y + delta.y;
        position.z = retained.z + delta.z;
        if (collide)
            FightCameraRaycast(position, position, retained);
        delta.x = position.x - retained.x;
        delta.y = position.y - retained.y;
        delta.z = position.z - retained.z;
        retained.x += delta.x;
        retained.y += delta.y;
        retained.z += delta.z;
        direction.x = delta.x;
        direction.y = delta.y;
        direction.z = delta.z;
    } else {
        direction.x = 0.0f;
        direction.y = 0.0f;
        direction.z = 0.0f;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FightCameraSubtract(const Vector3 &from, const Vector3 &to, Vector3 &result) {
    result.x = to.x - from.x;
    result.y = to.y - from.y;
    result.z = to.z - from.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float FightCameraLength(const Vector3 &value) {
    return am2Vector3Length(&value);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FightCameraCopy(const Vector3 &source, Vector3 &destination) {
    destination.x = source.x;
    destination.y = source.y;
    destination.z = source.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FightCameraBlendPoint(Vector3 &point, const Vector3 &other, float amount) {
    if (amount < 0.0f)
        amount = 0.0f;
    else if (amount > 1.0f)
        amount = 1.0f;
    point.x = point.x * amount + other.x * (1.0f - amount);
    point.y = point.y * amount + other.y * (1.0f - amount);
    point.z = point.z * amount + other.z * (1.0f - amount);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ScaleFightCameraVector(Vector3 &value, float length) {
    dc::sdk::SetVectorLength(&value, &value, length);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin) {
    const shen_name map = g_currentMap;
    if (map == SHEN_NAME(QKBT))
        return 0;
    flddToggleFlag(0x806U);
    return FindNearestRayIntersection(&result, &destination, &origin);
}
#endif

// @verified - d
bool shenCheckCurrentFoVModeIs2() {
    if (g_searchCameraState.phase == 2)
        return true;
    return false;
}

} // namespace s2
} // namespace shenmue

#endif
