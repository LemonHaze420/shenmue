#include "s2/character_control_ground_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include <cfloat>

namespace shenmue {
namespace s2 {

// @verified - d
void ccmUpdateRootGroundHeight(CharacterControlModel *control) {
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
    do {
        SampleNativeMotionRoot(animation, static_cast<std::int32_t>(bone), -1.0f, &sampled);
    } while (0);
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
    if (!(rightGround > leftGround))
        rightGround = leftGround;
    rightGround -= lower;
    plymGetControllerVerticalAxis(&vertical, &verticalSecond);
    plymGetControllerGroundBlendedXZ(&blendX, &blendZ);
    plymGetControllerDeltaXZ(&firstDelta, &secondDelta);
    control->adjustedGroundHeight = lower;
    float lift = 0.0f;
    const Vector3 *raised, *other, *delta;
    if (mtwkIsRightFootTargetAhead(work)) {
        raised = &left;
        other = &right;
        delta = &firstDelta;
    } else {
        raised = &right;
        other = &left;
        delta = &secondDelta;
    }
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(playerGetRotY()));
    const Vector3 *const search = plymUpdatePlayerSearchLocation(0);
    matrix_stack::TranslateInverseVector(search);
    Matrix4x4::TransformPoint(*raised, raisedLocal);
    Matrix4x4::TransformPoint(*other, otherLocal);
    PopCurrentMatrix(1);
    if (raised->y - other->y > kCharacterGroundRaisedThreshold) {
        if (kCharacterGroundDeltaLimit > delta->z) {
            float distance = otherLocal.z;
            if (0.0f > distance)
                distance = -distance;
            const float positive = distance > 0.0f ? distance : 0.0f;
            float weight = positive / kCharacterGroundBlendDistance;
            weight = 0.0f > weight || 1.0f > weight ? (0.0f > weight ? 0.0f : weight) : 1.0f;
            if (plymGetSpeedZ() > 0.0f)
                lift = (rightGround * (1.0f - weight)) * kCharacterGroundLiftScale;
        }
        if (plymGetSpeedZ() > 0.0f) {
            if (!(kCharacterGroundFullLiftThreshold > plymGetInterpolationValue()))
                lift = rightGround;
            float height = lower + lift;
            if (control->previousAdjustedGroundHeight > height)
                height = InterpolateTo(height, control->previousAdjustedGroundHeight, kCharacterGroundDescentBlend);
            control->adjustedGroundHeight = height;
        }
    }
}

} // namespace s2
} // namespace shenmue

#endif
