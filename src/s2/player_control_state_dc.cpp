#include "s2/player_control_state_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/calendar_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#endif
#include "s2/player_model_dc.hpp"
#include "s2/player_movement_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/fight.hpp"
#include "s2/motion_work_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/motion_ground_dc.hpp"
#endif
#include <cstring>

namespace shenmue {
namespace s2 {

std::int32_t plymBeginSpecialTargetMotionSequence(shen_name name);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t plymGetSpecialMode() {
    return (g_playerRuntimePointer->flags >> 10U) & 1U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t plymSetSpecialMode(std::int32_t enabled) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const std::uint32_t previous = runtime->flags;
    if (enabled)
        runtime->flags |= 0x400U;
    else
        runtime->flags &= ~0x400U;
    return (previous >> 10U) & 1U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymGetSpecialFlag() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    return runtime->model && (runtime->flags & 0x4000U) ? (runtime->model->padFlags & 0x20U) != 0 : -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymBeginSpecialMode() {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model || ((runtime->flags & 0x40000U) && (model->specialModeFlags & 1U)))
        return -1;
    runtime->flags |= 0x40000U;
    runtime->flags |= 0x80000U;
    model->specialModeFlags = 0x1001U;
    model->specialModeState = 0;
    model->specialModeFrame = 0;
    model->specialModeAuxState = 0;
    *reinterpret_cast<std::uint8_t *>(&model->retainedHeldFlags) = 0;
    plymSetSpecialModeTargetCharacter(GetActiveScenarioCharacter());
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymFinishRegionInteraction() {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model || !(model->padFlags & 0x800U))
        return -1;
    if (static_cast<std::int8_t>(model->regionInteractionState) != 4)
        return -1;
    ++model->regionInteractionState;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymHasRyoBag(std::int32_t mode) {
    PlayerModelState *model = g_playerRuntimePointer->model;
    if (!model)
        return 0;
    if (!(model->helperFlags & 2U))
        return 0;
    if (mode != 1)
        return 1;
    if (static_cast<std::int8_t>(model->activeHelperMode) == 1)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymCheckAuthorTargetRequested() {
    return plymEvaluateAuthorTargetRequest();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymSetSpecialModeTargetPosition(const Vector3 *position, std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U) || !model)
        return -1;
    model->specialModeState = 2;
    model->specialModeTargetPosition.x = position->x;
    model->specialModeTargetPosition.y = position->y;
    model->specialModeTargetPosition.z = position->z;
    if (mode)
        model->specialModeFlags |= 0xa0U;
    ++*reinterpret_cast<std::uint8_t *>(&model->retainedHeldFlags);
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymBeginSpecialTargetMotionSequence(shen_name name) {
    PlayerRuntimeState *runtime = g_playerRuntimePointer;
    PlayerModelState *model = runtime->model;
    if ((runtime->flags & 0x40000U) && name) {
        model->specialModeState = 3;
        model->specialModeTargetId = name;
        return 0;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymContinueSpecialTargetMotionSequence() {
    PlayerRuntimeState *runtime = g_playerRuntimePointer;
    PlayerModelState *model = runtime->model;
    if ((runtime->flags & 0x40000U) && model->specialModeTargetId) {
        model->specialModeState = 4;
        return 0;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymRequestSpecialPoseSequence(std::int32_t) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U))
        return -1;
    model->specialModeState = 7;
    *reinterpret_cast<std::uint8_t *>(&model->specialModeCounter) = 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymRequestRetryScene(std::int16_t act, shen_name area, std::int16_t entry) {
    PlayerModelState *model = g_playerRuntimePointer->model;
    if (model && !model->heldStateMode) {
        model->heldStateMode = 7;
        model->retryActId = act;
        model->retryAreaId = area;
        model->retryEntryId = entry;
        model->heldStateStep = 0;
        return 0;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymRequestClockSet(std::int32_t hour, std::int32_t minute) {
    PlayerModelState *model = g_playerRuntimePointer->model;
    if (model && !model->heldStateMode) {
        model->heldStateFlags |= 0x400;
        model->requestedClockHour = static_cast<std::int8_t>(hour);
        model->requestedClockMinute = static_cast<std::int8_t>(minute);
        return 0;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymEndSpecialMode(std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U) || !model)
        return -1;
    runtime->flags &= ~0x80000U;
    model->specialModeFlags &= 0xfefdU;
    model->specialModeFrame = 0;
    model->specialModeState = 0;
    model->padFlags |= 0x200000U;
    switch (mode) {
    case 1:
        model->specialModeState = 6;
        return 0;
    case 2:
        model->specialModeState = 6;
        model->specialModeFlags |= 0x20U;
        return 0;
    case 3:
        runtime->flags |= 0x80000U;
        model->specialModeFlags |= 0x100U;
        return 0;
    default:
        runtime->flags &= ~0x40000U;
        model->flags &= ~0x40000U;
    case 0:
        model->specialModeFlags = (model->specialModeFlags & 0xfffaU) | 0x2100U;
        model->movementState->flags &= ~2U;
        return 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymSetSceneContext(std::int16_t startMinute, std::int16_t endMinute, std::int16_t act, shen_name area, std::int16_t entry) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    model->holdWindowStartMinute = startMinute;
    model->holdWindowEndMinute = endMinute;
    model->sceneActId = act;
    model->sceneAreaId = area;
    model->sceneEntryId = entry;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t BuildTargetDateFromFrameClock(DateTime &target) {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    shenGetCurrentDateTime(target);
    const std::int16_t total = model->holdWindowEndMinute;
    const std::int16_t hour = static_cast<std::int16_t>(total / 60);
    const std::int16_t minute = static_cast<std::int16_t>(total % 60);
    if (hour < 6) {
        if (target.hour >= 6)
            AddDaysAndCalculateWeekday(target, 1);
    } else if (target.hour < 6) {
        shenGetDayInMonth(target, target);
    }
    target.hour = static_cast<std::int8_t>(hour);
    target.minute = static_cast<std::int8_t>(minute);
    target.second = 0;
    target.flags = 0;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t EV_plymRequestSceneContext(std::int16_t minute, std::int16_t act, shen_name area, std::int16_t entry) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    plymSetRequestedSceneContext(minute, act, area, entry);
    model->heldStateFlags |= 0x10;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float ctrlGetLookOverlayWeight() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control->ryo->alpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymSetCallback(void (*callback)()) {
    PlayerModelState *model;
    if ((model = g_playerRuntimePointer->model) == 0)
        return -1;
    model->heldStateCallback = callback;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymIsRetrySceneStepPending() {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    if (model->heldStateMode == 6 && static_cast<std::int8_t>(model->heldStateStep) > 0)
        return 0;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymScheduleCycleFlagCallback(std::int16_t minute) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model || model->heldStateMode)
        return -1;
    model->heldStateFlags &= ~0x4000;
    if (minute)
        model->heldStateFlags |= 0x4000U;
    model->heldStateTriggerMinute = minute;
    if (model->heldStateFlags & 0x4000U) {
        PlayerRuntimeState *const runtime = g_playerRuntimePointer;
        runtime->field08 |= 0x10;
        runtime->scheduledCallbackMinute = minute;
    } else {
        PlayerRuntimeState *const runtime = g_playerRuntimePointer;
        runtime->field08 &= ~0x10;
        runtime->scheduledCallbackMinute = minute;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void plymAdvanceTime(std::uint8_t hours, std::uint8_t minutes) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model || model->heldStateMode)
        return;
    model->timeAdvanceTargetHour = static_cast<std::int8_t>(hours);
    model->timeAdvanceTargetMinute = static_cast<std::int8_t>(minutes);
    model->heldStateMode = 5;
    model->heldStateStep = 0;
    model->heldStateFlags &= 0xfeffU;
}
#endif

// @verified - d
std::int32_t shenToggleAreaFlags(std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model)
        return -1;
    if (mode) {
        runtime->flags |= 0x4000U;
        model->motionInputState = static_cast<std::uint8_t>(mode);
        model->padFlags = (model->padFlags | 0x40U) & ~0x20U;
    } else {
        runtime->flags &= ~0x4000U;
        model->padFlags |= 0x20U;
        model->motionInputState = static_cast<std::uint8_t>(mode);
    }
    return 0;
}

// @verified - d
bool plymIsSearchBlocked() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model)
        return false;
    if ((model->flags & 0x100U) && model->heldStateMode)
        return true;
    if (runtime->flags & 0x01000000U)
        return true;
    if (model->padFlags & 0x800000U)
        return true;
    if (!(runtime->flags & 2U))
        return true;
    return false;
}

// @verified - d
std::int32_t plymIsInterpolationActive() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    if (!model)
        return 0;
    return model->movementInterpolation > 0.5f;
}

// @verified - d
std::uint32_t plymGetAreaExitState() {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return 0;
    if (static_cast<std::int8_t>(model->heldStateMode) == 5)
        return 0xffffffffU;
    if (model->heldStateFlags & 0x100U)
        return 1;
    return 0;
}

// @verified - d
std::uint32_t EV_setHoldStatesFlags(std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model)
        return UINT32_MAX;
    std::uint32_t previous = runtime->flags >> 8;
    previous &= 1U;
    switch (mode) {
    case 0:
        runtime->flags &= ~0x100U;
        break;
    case 1:
        runtime->flags |= 0x100U;
        break;
    case 2:
        runtime->field08 &= ~3;
        model->heldStateFlags &= ~6;
        break;
    case 3:
        runtime->field08 |= 3;
        break;
    }
    return previous;
}

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void __attribute__((renesas)) shenmue_s2_event_plymAdvanceTime(std::uint8_t hours, std::uint8_t minutes) {
    shenmue::s2::plymAdvanceTime(hours, minutes);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

void InstructionAssignMotScaleAndDir(Character *character, std::uint8_t scale, std::uint8_t direction) {
    figpAssignMotScaleAndDirFor(character, scale, direction);
}

std::uint8_t *InstructionAssignFigureMode(Character *character, std::uint8_t mode) {
    return figpAssignModeForCharacter(character, mode);
}

float InstructionPoseFrame(Character *character) {
    return mtwkGetFrame(charFindMotionwork(character), static_cast<MotionRegion>(0));
}

void InstructionSampleRootPosition(Character *character, std::uint16_t motion, Vector3 *position) {
    Vector3i rotation;
    GetCharacterLocalRotation(character, &rotation);
    SampleNativeMotionRoot(motion, mtwkGetRigBoneId(charFindMotionwork(character)), -1.0f, position);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(rotation.y);
    Matrix4x4::TransformPoint(*position, *position);
    PopCurrentMatrix(1);
}

void InstructionSetRegionEvalTime(float frame, Character *character) {
    MotionWork *const work = charFindMotionwork(character);
    if (frame > 0.0f)
        mtwkSetRegionEvalTime(work, frame, static_cast<MotionRegion>(0));
}

void InstructionSetLayerEndFrame(float frame, Character *character) {
    MotionWork *const work = charFindMotionwork(character);
    if (frame > 0.0f)
        mtwkSetLayerEndFrame(work, frame, static_cast<MotionRegion>(0));
}

void vfPublishInstructionActorMotion(FightActorRuntimePrefix *actor, Character *character, shen_name name, std::int32_t motion) {
    std::byte *const owner = static_cast<std::byte *>(actor->game->dialogue);
    charFindMotionwork(character);
    if (name == *reinterpret_cast<const std::uint32_t *>(owner + 0x46c)) {
        *reinterpret_cast<std::int32_t *>(owner + 0x4a8) = motion;
        *reinterpret_cast<std::int32_t *>(owner + 0x4a0) = GetCharacterWorldYaw(character);
    } else {
        *reinterpret_cast<std::int32_t *>(owner + 0x4ac) = motion;
        *reinterpret_cast<std::int32_t *>(owner + 0x4a4) = GetCharacterWorldYaw(character);
    }
    *reinterpret_cast<std::int32_t *>(reinterpret_cast<std::byte *>(actor) + 0x318) = motion;
    *reinterpret_cast<std::int32_t *>(reinterpret_cast<std::byte *>(actor) + 0x864) = 0;
}

std::uint32_t InstructionSetRegionFlag(Character *character) {
    return mtwkSetRegionFlag(charFindMotionwork(character), 0x41, 1, static_cast<MotionRegion>(0));
}

void InstructionApplyRootDelta(Character *character) {
    Vector3 delta;
    Vector3 scale;
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    mtwkUpdateRootDeltaVector(charFindMotionwork(character), &delta);
    delta.x *= scale.x;
    delta.y *= scale.y;
    delta.z *= scale.z;
    AddCharacterChildSpacePosition(character, &delta);
}

void InstructionRevertRootDelta(Character *character) {
    Vector3 delta;
    Vector3 scale;
    scale.x = -1.0f;
    scale.y = -1.0f;
    scale.z = -1.0f;
    mtwkUpdateRootDeltaVector(charFindMotionwork(character), &delta);
    delta.x *= scale.x;
    delta.y *= scale.y;
    delta.z *= scale.z;
    AddCharacterChildSpacePosition(character, &delta);
}

void InstructionSetMotionEnabled(Character *character, std::int8_t enabled) {
    SetCharacterMotionEnabled(character, enabled);
}

void InstructionSetLayerLoopStartTime(float time, Character *character) {
    mtwkSetLayerLoopStartTime(charFindMotionwork(character), time, static_cast<MotionRegion>(0));
}

float InstructionMotionFrame(Character *character) {
    return mtwkGetFrame(charFindMotionwork(character), static_cast<MotionRegion>(0));
}

std::uint32_t InstructionSetExtraFlag(Character *character, std::int32_t) {
    return mtwkSetExtraFlag(charFindMotionwork(character), 6, 0);
}

void InstructionSetLocalPosition(Character *character, const Vector3 *position) {
    SetCharacterLocalPosition(character, position);
}

void InstructionBonePosition(Character *character, std::int32_t bone, Vector3 *position) {
    charaGetMotWorldPos(character, bone, position);
}

void InstructionGetLocalPosition(const Character *character, Vector3 *position) {
    GetCharacterLocalPosition(character, position);
}

void InstructionSetLocalRotation(Character *character, const Vector3i *rotation) {
    SetCharacterLocalRotation(character, rotation);
}

void InstructionGetLocalRotation(const Character *character, Vector3i *rotation) {
    GetCharacterLocalRotation(character, rotation);
}

void InstructionGetMotionYawRotation(const Character *character, Vector3i *rotation) {
    rotation->x = 0;
    rotation->z = 0;
    rotation->y = static_cast<std::uint16_t>(charGetMotionWorkYaw(character));
}

std::int32_t AngleDifference16(std::int32_t from, std::int32_t to) {
    const std::int32_t backward = to - from;
    const std::int32_t forward = from - to;
    if ((forward & 0xffff) < (backward & 0xffff))
        return from > to ? -forward : -(forward & 0xffff);
    if (from > to)
        return backward & 0xffff;
    return backward & 0xffff;
}

std::uint16_t InstructionHeadingToPoint(const Vector3 *from, const Vector3 *to) {
    Vector3 delta;
    delta.x = to->x - from->x;
    delta.z = to->z - from->z;
    std::uint32_t angle = dc::sdk::ArcTan2Fixed(delta.x, delta.z);
    angle += 0x8000;
    return angle;
}

} // namespace s2
} // namespace shenmue

#endif

#endif
