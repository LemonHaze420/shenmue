#include "s2/character_control_update_dc.hpp"
#include "s2/character_control_ground_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include <cfloat>
#include <cmath>

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void mtwkTranslateRigInterpolated(MotionWork *work, float x, float y, float z);
#endif

// @verified - d
void CCM_Callback(Character *character, void *data) {
    CharacterControlModel *const control = static_cast<CharacterControlModel *>(data);
    Vector3 world, point, contact, pending, offset;
    if (!flddIsValid())
        control->flags &= ~(CCMF_UpdateHeight | CCMF_ApplyGravity | CCMF_EvaluateMotion);
    control->flags &= ~CCMF_HeightStep;
    control->appliedMovement.x = 0.0f;
    control->appliedMovement.y = 0.0f;
    control->appliedMovement.z = 0.0f;
    if (control->flags & CCMF_ResetMotion) {
        *reinterpret_cast<float *>(&control->field04) = 0.0f;
        control->verticalVelocity = 0.0f;
        *reinterpret_cast<float *>(&control->field0c) = 0.0f;
    }
    if ((control->flags & CCMF_UpdateHeight) == 0)
        return;
    GetCharacterWorldPosition(character, &world);
    GetCharacterPositionDelta(character, &pending);
    offset = pending;
    point.x = world.x + offset.x;
    point.y = world.y + offset.y + kCharacterControlDefaultDimension;
    point.z = world.z + offset.z;
    control->previousGroundHeight = control->groundHeight;
    point.y += 1.0f;
    control->groundHeight = FindFieldIntersectionHeight(&point);
    point.y -= 1.0f;
    if (control->groundHeight != -FLT_MAX) {
        transformIntersectionResult(reinterpret_cast<Vector2s *>(&control->groundPitch));
        if (!(kCharacterControlDefaultDimension > control->groundHeight - control->previousGroundHeight)) {
            control->flags |= CCMF_HeightStep;
            control->groundHeight = world.y;
        }
    } else {
        control->groundHeight = world.y;
        control->groundPitch = 0;
        control->groundYaw = 0;
    }
    control->previousAdjustedGroundHeight = control->adjustedGroundHeight;
    control->adjustedGroundHeight = control->groundHeight;
    ccmUpdateRootGroundHeight(control);
    const float groundCenter = control->adjustedGroundHeight + kCharacterControlDefaultDimension;
    if (control->flags & CCMF_ApplyGravity) {
        control->verticalVelocity += kCharacterControlGravity;
        point.y += control->verticalVelocity;
    }
    if (HasCharacterScenarioName(character, "BCYL"))
        control->collisionRadius = kCharacterControlBicycleRadius;
    if (control->flags & CCMF_EvaluateMotion) {
        if ((control->flags & CCMF_SnapToGround) && point.y < groundCenter + kCharacterControlSnapTolerance) {
            if (control->flags & CharacterControlUseConfiguredCapsule)
                point.y = InterpolateTo(groundCenter, point.y - control->verticalVelocity, 0.5f);
            else
                point.y = InterpolateTo(groundCenter, point.y - control->verticalVelocity, 0.5f);
            control->verticalVelocity = 0.0f;
        }
        if (control->flags & CCMF_FineRaycast) {
            if (control->flags & CCMF_FieldDataDebug)
                flddToggleFlag(32);
            if (RaycastInWorldGrid(&contact, &point, control->collisionRadius)) {
                point.x = InterpolateTo(contact.x, point.x, kCharacterControlCollisionBlend);
                point.z = InterpolateTo(contact.z, point.z, kCharacterControlCollisionBlend);
            }
        }
        offset.x = point.x - world.x;
        offset.y = point.y - world.y - kCharacterControlDefaultDimension;
        offset.z = point.z - world.z;
        if (control->flags & CCMF_AlternateProbe) {
            point.x = world.x + offset.x;
            point.y = world.y + offset.y + kCharacterControlAlternateProbeHeight;
            point.z = world.z + offset.z;
            if (FindFieldIntersectionHeight(&point) == -FLT_MAX) {
                if (control->flags & CCMF_ApplyGravity)
                    control->verticalVelocity -= control->verticalVelocity;
                if (control->flags & CharacterControlUseConfiguredCapsule) {
                    offset.x = 0.0f;
                    offset.y = 0.0f;
                    offset.z = 0.0f;
                } else
                    offset.y = 0.0f;
            }
        }
    }
    float moveX = offset.x - pending.x;
    float moveY = offset.y - pending.y;
    float moveZ = offset.z - pending.z;
    if (!(moveY > 0.0f) && !HasCharacterScenarioName(character, "BCYL"))
        control->verticalVelocity = moveY;
    offset.y = pending.y;
    control->collisionAdjustment.y = moveY;
    moveY = 0.0f;
    SetCharacterPositionDelta(character, &offset);
    MotionWork *const work = charFindMotionwork(character);
    if (work && (!(FLT_EPSILON > std::fabs(moveX)) || !(FLT_EPSILON > std::fabs(moveY)) || !(FLT_EPSILON > std::fabs(moveZ))))
        mtwkTranslateRigInterpolated(work, moveX, moveY, moveZ);
    if (FLT_EPSILON > std::fabs(moveX))
        moveX = 0.0f;
    if (FLT_EPSILON > std::fabs(moveY))
        moveY = 0.0f;
    if (FLT_EPSILON > std::fabs(moveZ))
        moveZ = 0.0f;
    control->appliedMovement.x = moveX;
    control->appliedMovement.y = moveY;
    control->appliedMovement.z = moveZ;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ccmUpdateRootGroundHeightBlended(CharacterControlModel *control) {
    if (!(control->flags & CCMF_RootOwner))
        return;
    MotionWork *const work = charFindMotionwork(control->owner);
    if (!work)
        return;
    Vector3 right, left, firstDelta, secondDelta, sampled, otherLocal, raisedLocal;
    float vertical, verticalSecond, blendX, blendZ;
    mtwkGetControllerTargetPosition(work, 6, right);
    mtwkGetControllerTargetPosition(work, 5, left);
    const std::uint16_t animation = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    const std::uint32_t bone = mtwkGetRigBoneId(work);
    SampleNativeMotionRoot(animation, static_cast<std::int32_t>(bone), -1.0f, &sampled);
    float speed = sampled.z;
    if (0.0f > speed)
        speed = -speed;
    if (IsPlayerControlEnabled()) {
        am2Vector3AddInPlace(&right, &control->frameDisplacement);
        am2Vector3AddInPlace(&left, &control->frameDisplacement);
    }
    const float rightY = right.y, leftY = left.y;
    right.y += kCharacterGroundProbeHeight;
    left.y += kCharacterGroundProbeHeight;
    float rightGround = FindFieldIntersectionHeight(&right);
    float leftGround = FindFieldIntersectionHeight(&left);
    right.y = rightY;
    left.y = leftY;
    if (rightGround != -FLT_MAX)
        right.y = rightGround;
    if (leftGround != -FLT_MAX)
        left.y = leftGround;
    if (rightGround == -FLT_MAX)
        rightGround = FLT_MAX;
    if (leftGround == -FLT_MAX)
        leftGround = FLT_MAX;
    float lower = leftGround > rightGround ? rightGround : leftGround;
    if (!(control->flags & CCMF_HeightStep))
        lower = control->groundHeight > lower ? lower : control->groundHeight;
    plymGetControllerVerticalAxis(&vertical, &verticalSecond);
    plymGetControllerGroundBlendedXZ(&blendX, &blendZ);
    plymGetControllerDeltaXZ(&firstDelta, &secondDelta);
    control->adjustedGroundHeight = lower;
    const Vector3 *raised, *other, *delta;
    float axis, blend;
    if (mtwkIsRightFootTargetAhead(work)) {
        raised = &left;
        other = &right;
        axis = vertical;
        delta = &firstDelta;
        blend = blendZ;
    } else {
        raised = &right;
        other = &left;
        axis = verticalSecond;
        delta = &secondDelta;
        blend = blendX;
    }
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(playerGetRotY()));
    const Vector3 *const search = plymUpdatePlayerSearchLocation(0);
    matrix_stack::TranslateInverseVector(search);
    Matrix4x4::TransformPoint(*raised, raisedLocal);
    Matrix4x4::TransformPoint(*other, otherLocal);
    PopCurrentMatrix(1);
    const float rise = raised->y - other->y;
    if (rise > kCharacterGroundRaisedThreshold && (speed > 0.5f || (0.5f > speed && -0.01f > delta->z))) {
        float distance = raisedLocal.z - otherLocal.z;
        if (0.0f > distance)
            distance = -distance;
        float weight = (0.0f > distance ? -distance : distance) / 0.6f;
        weight = 0.0f > weight || 1.0f > weight ? (0.0f > weight ? 0.0f : weight) : 1.0f;
        float lift = 0.0f;
        if (kCharacterGroundDeltaLimit > delta->z) {
            lift = rise * (1.0f - weight);
            control->adjustedGroundHeight = lower + lift;
        }
        if (axis > 0.05f) {
            if (blend > 0.45f)
                control->adjustedGroundHeight += blend - lift;
            else if (blend > 0.275f)
                control->adjustedGroundHeight += (blend - lift) * 0.5f;
        }
    }
    if (rise > 0.01f && control->adjustedGroundHeight < control->previousAdjustedGroundHeight)
        control->adjustedGroundHeight = control->previousAdjustedGroundHeight;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
