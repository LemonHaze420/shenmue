#pragma once

#include "s2/player_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
#include "s2/date_time.hpp"
#endif

namespace shenmue {
namespace s2 {

static const std::size_t kPlayerModelFlagsOffset = 0, kPlayerModelPadFlagsOffset = 4;
static const std::size_t kPlayerHoldRequestModeOffset = 0x0e, kPlayerHeldStateModeOffset = 0x388;
static const std::size_t kPlayerHeldFlagsOffset = 0x386;
std::int32_t shenToggleAreaFlags(std::int32_t mode);
std::uint32_t EV_setHoldStatesFlags(std::int32_t mode);
bool plymIsSearchBlocked();
std::int32_t plymIsInterpolationActive();
std::uint32_t plymGetAreaExitState();
#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t plymFinishRegionInteraction();
std::int32_t plymGetSpecialFlag();
std::uint32_t plymGetSpecialMode();
std::uint32_t plymSetSpecialMode(std::int32_t enabled);
std::int32_t plymBeginSpecialMode();
std::int32_t plymSetSceneContext(std::int16_t startMinute, std::int16_t endMinute, std::int16_t act, shen_name area, std::int16_t entry);
std::int32_t BuildTargetDateFromFrameClock(DateTime &target);
float ctrlGetLookOverlayWeight();
std::int32_t plymSetCallback(void (*callback)());
std::int32_t plymScheduleCycleFlagCallback(std::int16_t minute);
void plymAdvanceTime(std::uint8_t hours, std::uint8_t minutes);
std::int32_t plymEndSpecialMode(std::int32_t mode);
std::int32_t plymCheckAuthorTargetRequested();
std::int32_t plymSetSpecialModeTargetPosition(const Vector3 *position, std::int32_t mode);
std::int32_t plymBeginSpecialTargetMotionSequence(shen_name name);
std::int32_t plymContinueSpecialTargetMotionSequence();
std::int32_t plymRequestSpecialPoseSequence(std::int32_t pose);
std::int32_t plymHasRyoBag(std::int32_t mode);
std::int32_t plymRequestRetryScene(std::int16_t act, shen_name area, std::int16_t entry);
struct Character;
std::uint8_t *InstructionAssignFigureMode(Character *character, std::uint8_t mode);
float InstructionPoseFrame(Character *character);
void InstructionSetRegionEvalTime(float frame, Character *character);
void InstructionSetLayerEndFrame(float frame, Character *character);
struct FightActorRuntimePrefix;
void vfPublishInstructionActorMotion(FightActorRuntimePrefix *actor, Character *character, shen_name name, std::int32_t motion);
std::uint32_t InstructionSetRegionFlag(Character *character);
float InstructionMotionFrame(Character *character);
std::uint32_t InstructionSetExtraFlag(Character *character, std::int32_t unused);
void InstructionSetLocalPosition(Character *character, const Vector3 *position);
void InstructionBonePosition(Character *character, std::int32_t bone, Vector3 *position);
void InstructionGetLocalPosition(const Character *character, Vector3 *position);
void InstructionSetLocalRotation(Character *character, const Vector3i *rotation);
void InstructionGetLocalRotation(const Character *character, Vector3i *rotation);
std::int32_t AngleDifference16(std::int32_t from, std::int32_t to);
#endif

} // namespace s2
} // namespace shenmue

#endif
