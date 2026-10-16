#pragma once

#include "s2/player.hpp"
#include "s2/area_event.hpp"

#include "s2/character.hpp"
#include "s2/area_jump.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/debug_text.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/input_bindings.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/motion_work.hpp"
#include "s2/memo.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/map_assets.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>

#include <array>
#include <cstring>

namespace shenmue::s2 {

// One loaded misc/ MT7 of Ryo's bag helper: the reader and its file block.
struct RyoBagModel {
    MT7Model model;
    void *source;
};

} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

inline constexpr std::array<shen_name, 3> plymDoorCompanionNames{PLAYER_COMPANION_REN, PLAYER_COMPANION_SHENHUA, PLAYER_COMPANION_JOY};

} // namespace shenmue::s2::player_detail
#include "s2/player_collision.hpp"

namespace shenmue::s2 {
namespace player_detail {}
using namespace player_detail;
extern PlayerRuntimeState g_playerRuntime;

} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

void UpdatePlayerPartyCompanions(PlayerModelState &state);
void plymInitializeMotionBinding(PlayerMovementState &movement);
void plymUpdateLockOnCharacter(PlayerModelState &state);
void plymBuildInteractionPlacementContext(PlayerModelState &state);
PlayerPlacementNode *plymUpdatePlacementChain(PlayerPlacementNode *root);
void plymUpdateScriptedPlacementTarget(PlayerModelState &state);
void plymUpdateInteractionPlacementTarget(PlayerModelState &state);
extern LegacyInputSlot playerInputSnapshot;
extern Vector3 publishedPlayerSearchLocation;
extern std::int16_t movingFrames, stationaryFrames, turningFrames;
Vector3 TransformWithActiveMatrix(const Vector3 &value);
Vector3 RotateAngleAxis(const Vector3 &origin, float amount, std::int16_t angle);
void SimdSolveVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &direction, const Vector3 &reference, const Vector3 &crossVector);
void ComputeVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &target, const Vector3 &reference, const Vector3 &referenceEnd);
float GetPlanarDistanceOrZero(const Vector3 &first, const Vector3 &second);
void UpdateTargetWorldMovementGate(PlayerModelState &state);
std::uint16_t GetCharacterAccumulatedYaw(const Character *character);
void ApplyPlayerMotionFlagCommand(PlayerMovementState &movement, const MotnPlayerMotionFlagCommand &command);
void ProcessPlayerMotionFlagCursor(PlayerMovementState &movement);
void ResetPlayerMotionFlagCursor(PlayerMovementState &movement);
std::uint8_t ReachedMotionRootMode(std::uint32_t motionId);
std::int16_t camCtrlGetPitchYawWithOffsetsFrom(const PlayerModelState &state);
void SynchronizePlayerSearchTransform(PlayerModelState &state);
float mtwkSampleCurrentRemainingTime(const PlayerMovementState &movement);
void SynchronizePlayerModelState(Character *character, PlayerModelState &state);
void UpdatePlayerDisplayAlpha(PlayerModelState &state);
void mtwkInitFlags(MotionWork *motionWork, std::uint32_t region);
void ctrlAssignCharaHandlerFor(PlayerModelState &state);
void ReleaseRyoBagModel(RyoBagModel *bag);
void modelFree(Character *character, void *payload);
RyoBagModel *LoadRyoBagModel(const char *filename, shen_name token, std::uint32_t mode);
std::int64_t plymCreateRyoBagCharacter(PlayerModelState &state, std::int32_t mode);
void plymSelectRyoBagMode(PlayerModelState &state);
void plymUpdateRyoBagVisibility(PlayerModelState &state);
void plymReleaseRyoBag(PlayerModelState &state, MotionWork *motionWork);
void plymApplyPendingRyoBagMode(PlayerModelState &state);
void plymUpdateRyoBag(PlayerModelState &state);
void InitializePlayerModelState(PlayerModelState &state);
std::uint8_t plymTriggerHistoryValue(std::uint8_t trigger);
void plymSampleInput(PlayerModelState &state);
extern std::int32_t largeTurnInputFrames;
float PlymSquaredDistance(const Vector3 &left, const Vector3 &right);
std::int16_t PlymAngleXZ(const Vector3 &vector);
std::uint16_t ComputeInverseAngleRatio(float ratio);
Vector3 TransformPlymPoint(const Matrix4x4 &transform, const Vector3 &point);
std::int32_t flddRaycastScene(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination);
void CastPlymSideRay(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination, PlymRayResult &ray, bool nearerOnly);
void SetPlymCharacterBoundary(PlayerModelState &state, std::int16_t bearing, float capsuleDistance, float horizontalDistance);
void SetPlymWallBoundary(PlayerModelState &state, std::int16_t wallYaw);
void plymProbeMovementCollision(PlayerModelState &state);
void plymProbeDesiredYawCollision(PlayerModelState &state);
void plymComputeTurnInput(PlayerModelState &state);



float ScaledDelta(float value);
std::int16_t LerpFixedAngle(std::int16_t target, std::int16_t base, float amount);
std::uint8_t Abs8(std::int8_t value);
bool IsLookMovingRootState(std::uint32_t rootYawState);
void plymUpdateLookAround(PlayerModelState &state);
void plymUpdateCameraInput(PlayerModelState &state);
void UpdatePlayerYawFromInput(PlayerModelState &state);
void PreparePlayerMovementContext(PlayerModelState &state);
void ProbeReachedOrdinaryMovementCollision(PlayerMovementState &movement);
void UpdateReachedFloorStairCollision(PlayerMovementState &movement);
void PrepareReachedStairPhaseCorrection(PlayerMovementState &movement);
void ApplyReachedCollisionRootCorrection(PlayerMovementState &movement);
float ReachedMotionSpeed(std::uint32_t motionId);
bool CanRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested, float frame, float frameCount);
std::uint16_t SelectPlayerIdleMotion(PlayerMovementState &movement, bool &mirrored);
void plymAdjustControllerGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *work, void *);
void ResetPlayerMovementMotion(PlayerMovementState *movement, std::int32_t retainMotion);
bool IsPlayerOrdinaryGait(std::uint32_t motion);
bool IsPlayerStairGait(std::uint32_t motion);
void ConfigurePlayerGaitRequest(PlayerMotionRequest &request, MotionWork *work);
void ApplyPlayerIdleStop(PlayerMovementState &movement);
void ConfigurePlayerMotionRequest(PlayerMotionRequest &request, PlayerMovementState &movement);

void ApplyPlayerMotionRequest(PlayerMovementState &movement, std::uint32_t motionId, float previousFrame, float previousFrameCount, bool mirrored,
    std::int16_t requestedYaw, std::int16_t authoredYaw);

bool IsReachedRootConsumerTranslationMotion(std::uint16_t motionId);
std::uint16_t SelectReachedRootConsumerMotion(const PlayerMovementState &movement, std::int8_t stepDirection);
bool ApplyReachedRootConsumerTranslation(PlayerMovementState &movement, std::uint16_t animationId);
void UpdateReachedRootConsumer(PlayerMovementState &movement);
void FinalizeReachedRootConsumerScale(PlayerMovementState &movement);
float Lerp(float target, float base, float amount);
void UpdatePlayerCollisionSpeedScale(PlayerMovementState &movement);
void UpdatePlayerTargetYaw(PlayerMovementState &movement);
bool TryRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested);
void UpdatePlayerLocomotionRequest(PlayerMovementState &movement);
void ExecutePlayerMovement(PlayerMovementState &movement);
void UpdateReachedPlayerYawWriteback(PlayerModelState &state);
void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state);
void UpdatePlayerInterpolation(PlayerModelState &state);
float plymProbeSpeedRatio(float distance, float speed);
float PlymDistance(const Vector3 &left, const Vector3 &right);
Vector3 PlymToLocal(std::int16_t yaw, const Vector3 &origin, const Vector3 &point);
Vector3 PlymRotatedWorldProbe(const PlayerModelState &state, std::int16_t yaw, const Vector3 &probe);
void plymProbeSideWalls(PlayerModelState &state);
void plymUpdateGroundBoundaryTurn(PlayerModelState &state);
void plymMatchCharacterSpeed(PlayerModelState &state);
std::int32_t NpcAvoidanceSide(shen_name name);
void plymResolveCollisionSteering(PlayerModelState &state);
void plymUpdateCollisionTurnActivation(PlayerModelState &state);
void plymUpdateCollisionTurnLatch(PlayerModelState &state);
bool plymConstrainMovementContextYaw(PlayerModelState &state);
void plymResolveCollisionTurn(PlayerModelState &state);
void plymUpdateMotionInputState(PlayerModelState &state);
shen_name FindShenmueNodeFromScenarioTask(Character *character);
shen_name ptycFindDoorCompanion(shen_name name);
void plymUpdateRegionInteraction(PlayerModelState &state);
void plymUpdatePlacementNode(PlayerPlacementNode *node);
float plymGetApproachStepDistance(float previous, const Vector3 &delta);
int plymResolvePlacementMovement(PlayerModelState &state);

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

int plymBuildAuthorPlacementContext(PlayerModelState &state);
void plymUpdateAuthorPlacementTarget(PlayerModelState &state);
bool plymRecordPlacementHistory(PlayerModelState &state, const Vector3 &position);
const Vector3 *plymGetPlacementFromHistory(const PlayerModelState &state, int count);
void plymUpdateLockOnMovement(PlayerModelState &state);
bool plymCanLatchInteractionTarget(shen_name name);
void plymUpdateSpecialMotionSequence(PlayerModelState &state);
bool plymIsSpecialAuthorMotion(shen_name name);
bool plymHasScenarioSelectedLocationRoot();
void plymUpdateSpecialMode(PlayerModelState &state);
void plymSelectActiveMovementMode(PlayerModelState &state);
void plymResolveMovementMode(PlayerModelState &state);

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

bool scenIsRecordRunning(shen_name name);
Vector3 PlymHeadLookPoint(const PlayerModelState &state, std::uint16_t yaw, std::uint16_t pitch, bool withCharacterYaw);
void plymUpdateAutomaticLook(PlayerModelState &state);
void plymUpdateLookTarget(PlayerModelState &state);
HeldStateMode GetHeldStateMode(const PlayerModelState &state);
void SetHeldStateMode(PlayerModelState &state, HeldStateMode mode);
bool IsHeldTransitionReady(const PlayerModelState &state);
void PrepareHeldTransition();
bool StartHeldScenarioRecord(shen_name name);
void BeginHeldFade();
bool IsHeldFadeActive();
void QueueHeldSceneChange(PlayerModelState &state, std::int16_t act, shen_name area, std::int16_t entry);
void plymUpdateDuringSearchRequest(PlayerModelState &state);
void plymUpdateHeldStateCallback(PlayerModelState &state);
void plymUpdateHeldState(PlayerModelState &state);
void PlayerModelUpdate(Character *character, void *payload);

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {} // namespace shenmue::s2
