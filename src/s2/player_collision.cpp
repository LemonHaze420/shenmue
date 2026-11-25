#include "shenmue.h"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/player_model_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/math.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/player_internal.hpp"
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int32_t mtwkGetEntityYaw(const MotionWork *work);
void OffsetPointAlongYaw(Vector3 *result, const Vector3 *point, float distance, std::int16_t yaw);
std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int16_t Vector3_GetFixedYaw(const Vector3 *direction);
void GetTransformedForwardVector(Vector3 *direction);
const std::uint32_t *flddGetCategoryRecord(std::uint32_t category);
#endif

namespace model_component_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right);
#endif

} // namespace model_component_detail

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
inline float SquareRoot(float value) {
    return ::sqrtf(value);
}


float plymProbeSpeedRatio(float distance, float speed);
void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state);
bool IsHeldTransitionReady(const PlayerModelState &state);

static const Vector3 plymRayForward = {0.0f, 0.0f, -10.0f};
static const Vector3 plymRayRightEnd = {0.34999999f, 0.0f, -10.0f};
static const Vector3 plymRayLeftEnd = {-0.34999999f, 0.0f, -10.0f};
static const Vector3 plymRayRightStart = {0.34999999f, 0.0f, 0.0f};
static const Vector3 plymRayLeftStart = {-0.34999999f, 0.0f, 0.0f};

static std::int16_t PlymAngleXZ(const Vector3 &vector) {
    return static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-vector.x, -vector.z));
}


static float ClampPlymRatio(float ratio) {
    return ratio < 0.0f ? 0.0f : ratio < 1.0f ? ratio : 1.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Vector3 TransformWithActiveMatrix(const Vector3 &value) {
    return {
        value.x * workTransformMatrix.rows[0].x + value.y * workTransformMatrix.rows[1].x + value.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        value.x * workTransformMatrix.rows[0].y + value.y * workTransformMatrix.rows[1].y + value.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        value.x * workTransformMatrix.rows[0].z + value.y * workTransformMatrix.rows[1].z + value.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z,
    };
}

// Native Vector3::RotateAngleAxis: the point `amount` along the back direction of the yaw.
Vector3 RotateAngleAxis(const Vector3 &origin, float amount, std::int16_t angle) {
    const auto back = static_cast<std::uint16_t>(angle + 0x8000);
    return {sin_fixed16(static_cast<std::int16_t>(back)) * amount + origin.x, origin.y, cos_fixed16(back) * amount + origin.z};
}

// Native SimdSolveVectorProjection: Gauss-Jordan on the XZ rows [direction | -crossVector | reference - base], swapping
// the rows for a pivot below FLT_EPSILON. Output stays at base for parallel lines or an unusable pivot.
void SimdSolveVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &direction, const Vector3 &reference, const Vector3 &crossVector) {
    constexpr float epsilon = 0.00000011920929f;
    output = base;
    if (std::fabs(direction.x * crossVector.z - direction.z * crossVector.x) < epsilon)
        return;
    float rows[2][3] = {{direction.x, -crossVector.x, reference.x - base.x}, {direction.z, -crossVector.z, reference.z - base.z}};
    if (std::fabs(rows[0][0]) < epsilon || std::fabs(rows[1][1]) < epsilon) {
        for (std::int32_t column = 0; column < 3; ++column) {
            const float value = rows[0][column];
            rows[0][column] = rows[1][column];
            rows[1][column] = value;
        }
    }
    if (std::fabs(rows[0][0]) < epsilon || std::fabs(rows[1][1]) < epsilon)
        return;
    for (std::int32_t pivot = 0; pivot < 2; ++pivot) {
        const float divisor = rows[pivot][pivot];
        for (std::int32_t column = pivot; column < 3; ++column)
            rows[pivot][column] /= divisor;
        const std::int32_t other = pivot ^ 1;
        const float factor = rows[other][pivot];
        for (std::int32_t column = pivot; column < 3; ++column)
            rows[other][column] -= factor * rows[pivot][column];
    }
    const float weight = rows[0][2];
    output = {weight * direction.x + base.x, weight * direction.y + base.y, weight * direction.z + base.z};
}

// Native ComputeVectorProjection: where the base->target line meets the reference->referenceEnd line in XZ.
void ComputeVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &target, const Vector3 &reference, const Vector3 &referenceEnd) {
    const Vector3 direction{target.x - base.x, target.y - base.y, target.z - base.z};
    const Vector3 crossVector{referenceEnd.x - reference.x, referenceEnd.y - reference.y, referenceEnd.z - reference.z};
    SimdSolveVectorProjection(output, base, direction, reference, crossVector);
}

// Native GetPlanarDistanceOrZero: the XZ distance, or zero when both deltas are below FLT_EPSILON.
float GetPlanarDistanceOrZero(const Vector3 &first, const Vector3 &second) {
    const float deltaZ = first.z - second.z;
    const float deltaX = first.x - second.x;
    if (std::fabs(deltaX) < 0.00000011920929f && std::fabs(deltaZ) < 0.00000011920929f)
        return 0.0f;
    return std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
}

float PlymSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    constexpr float epsilon = 0.00000011920929f;
    if (std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon)
        return 0.0f;
    return y * y + x * x + z * z;
}

// Vector3::ComputeFixed16AngleXZ returns inv_atanf2_i16(x, z).
std::int16_t PlymAngleXZ(const Vector3 &vector) {
    return static_cast<std::int16_t>(FixedAtan2(-vector.x, -vector.z));
}

std::uint16_t ComputeInverseAngleRatio(float ratio) {
    if (std::fabs(ratio) > 1.0f)
        return ratio > 0.0f ? 0 : 0x8000U;
    const float adjacent = std::sqrt(1.0f - ratio * ratio);
    const std::uint16_t normalized = adjacent == 0.0f ?
        (ratio > 0.0f ? 0x4000U : 0xc000U) :
        static_cast<std::uint16_t>(static_cast<std::int32_t>(std::atan(ratio / adjacent) * 65536.0f / 6.2831855f + 0.5f));
    return static_cast<std::uint16_t>(0x4000 - normalized);
}

Vector3 TransformPlymPoint(const Matrix4x4 &transform, const Vector3 &point) {
    Vector3 output{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&transform);
    Matrix4x4::TransformPoint(point, output);
    Matrix4x4::Pop(1);
    return output;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
std::int32_t flddRaycastScene(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int32_t firstHit = 0;
    float squared = 100.0f;
    Vector3 second, first;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    float squaredDistance = 100.0f;
    const std::uint8_t previousFlags = state.collisionFlags;
#endif
    state.collisionFlags = 0;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.collisionType = 0;
    state.collisionCharacter = 0;
    flddToggleFlag(0xfbd7U);
    if (FindFieldSegmentOffsetInWorld(&first, &destination, &origin)) {
        if (flddHasFoundIntersectionOfType(0x20U))
            firstHit |= 0x20U;
    }
    flddToggleFlag(0x428U);
    if (FindFieldSegmentOffsetInWorld(&second, &destination, &origin) &&
        (!firstHit ||
            !(model_component_detail::Vector3_ComputeSquaredDistance(origin, second) >
                model_component_detail::Vector3_ComputeSquaredDistance(origin, first)))) {
        state.fieldCollisionPoint = second;
        state.collisionFlags = 1;
        state.collisionType = 1;
        squared = model_component_detail::Vector3_ComputeSquaredDistance(state.worldPosition, second);
    }
    ccmUpdateEntityFlag(state.character);
    if (CheckPointInCylinder(&origin, &destination)) {
        state.collisionCharacter = GetPointFound();
        GetCharacterWorldPosition(state.collisionCharacter, &second);
        second.y = destination.y;
        const float margin = GetCharacterCollisionCapsuleMargin(state.collisionCharacter);
        const float distance = model_component_detail::Vector3_ComputeSquaredDistance(origin, second) - margin * margin;
        if (squared > distance) {
            state.fieldCollisionPoint = second;
            state.collisionFlags = 2;
            state.collisionType = 2;
            squared = distance;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.collisionType = plymRayHitNone;
    std::int32_t result = 0;
    state.collisionCharacter = nullptr;
    state.collisionDistance = 10.0f;
    SetFieldDataFlags(1032);
    SetFieldDataFlag(32);
    Vector3 hit{};
    if (RaycastForCollision(&hit, &destination, &origin)) {
        FieldRadiusMetadata step{};
        if (!flddHasFoundIntersectionOfType(32) || !flddQueryRadiusMetadata(hit, step) || step.stepCount == 0) {
            state.fieldCollisionPoint = hit;
            state.collisionType = plymRayHitField;
            squaredDistance = PlymSquaredDistance(state.worldPosition, hit);
            result = 1;
        } else {
            float climb = static_cast<float>(step.stepCount) * 0.22499999f + step.anchor.y + 0.40000001f - hit.y;
            if (climb >= 1.8f)
                climb = 1.8f;
            const Vector3 climbedOrigin{hit.x, hit.y + climb, hit.z};
            const Vector3 climbedDestination{destination.x, climb + destination.y, destination.z};
            flddToggleFlag(1064);
            Vector3 climbedHit{};
            if (RaycastForCollision(&climbedHit, &climbedDestination, &climbedOrigin)) {
                state.fieldCollisionPoint = climbedHit;
                state.collisionType = plymRayHitField;
                const float dx = climbedOrigin.x - climbedHit.x;
                const float dy = climbedOrigin.y - climbedHit.y;
                const float dz = climbedOrigin.z - climbedHit.z;
                float distance = PlymSquaredDistance(state.worldPosition, hit) + std::sqrt(dx * dx + dy * dy + dz * dz);
                if (distance >= 10.0f)
                    distance = 10.0f;
                squaredDistance = distance * distance;
                result = 1;
#endif
        }
    }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const float distance = sqrtf(squared);
    state.collisionDistance = distance;
    return static_cast<std::int8_t>(state.collisionFlags);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    }
    if ((previousFlags & 4U) == 0) {
        if (CharacterControlModel *const own = FindCharacterControlModel(state.characterNode))
            own->flags |= 0x400U;
        if (Character *const character = CheckPointInCylinder_Wrapper(origin, destination)) {
            state.collisionCharacter = character;
            Vector3 position = GetCharacterWorldPosition(character);
            position.y = destination.y;
            const float margin = GetCharacterCollisionCapsuleMargin(character);
            const float characterDistance = std::fmax(0.0f, PlymSquaredDistance(origin, position) - margin * margin);
            if (squaredDistance > characterDistance) {
                squaredDistance = characterDistance;
                result = 1;
                state.fieldCollisionPoint = position;
                state.collisionType = plymRayHitCharacter;
            }
        }
    }
    state.collisionDistance = std::fabs(squaredDistance) < 0.00000011920929f ? 0.0f : std::sqrt(squaredDistance);
    if (result) {
        state.collisionFlags |= 1U;
        state.collisionDistance = std::sqrt(squaredDistance);
    }
    return result;
#endif
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// One side-rail cast; a later rail replaces the earlier only when nearer.
void CastPlymSideRay(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination, PlymRayResult &ray, bool nearerOnly) {
    if (!flddRaycastScene(state, origin, destination) || (nearerOnly && !(ray.distance > state.collisionDistance)))
        return;
    ray.type = state.collisionType;
    ray.distance = state.collisionDistance;
    ray.point = state.fieldCollisionPoint;
    if (ray.type == plymRayHitField)
        transformIntersectionResult(ray.normal);
    else if (ray.type == plymRayHitCharacter)
        ray.character = state.collisionCharacter;
}

// Native collision-boundary construction around a character obstacle: the
// capsule's angular half-width, relative to the current yaw.
void SetPlymCharacterBoundary(PlayerModelState &state, std::int16_t bearing, float capsuleDistance, float horizontalDistance) {
    const float ratio = std::fmin(1.0f, std::fmax(0.0f, capsuleDistance / horizontalDistance));
    const std::uint16_t inverse = ComputeInverseAngleRatio(ratio);
    const auto halfWidth = static_cast<std::int16_t>(0x4000 - inverse);
    const auto relative = static_cast<std::int16_t>(bearing - state.currentYaw);
    const auto boundary = static_cast<std::int16_t>(relative < 0 ? relative - inverse : 0x4000 - halfWidth + relative);
    state.collisionBoundaryYaw = static_cast<std::int16_t>(boundary + state.currentYaw);
    state.secondaryCollisionBoundaryYaw = state.collisionBoundaryYaw;
    if (relative >= 0) {
        state.collisionLimitYawA = static_cast<std::int16_t>(bearing - halfWidth);
        state.collisionLimitYawB = static_cast<std::int16_t>(bearing + halfWidth);
    } else {
        state.collisionLimitYawB = static_cast<std::int16_t>(bearing - halfWidth);
        state.collisionLimitYawA = static_cast<std::int16_t>(bearing + halfWidth);
    }
}

void SetPlymWallBoundary(PlayerModelState &state, std::int16_t wallYaw) {
    const auto side = static_cast<std::int16_t>(static_cast<std::int16_t>(state.currentYaw - wallYaw) <= 0 ? -0x4000 : 0x4000);
    state.collisionBoundaryYaw = wallYaw;
    state.secondaryCollisionBoundaryYaw = wallYaw;
    state.collisionLimitYawA = static_cast<std::int16_t>(wallYaw + side);
    state.collisionLimitYawB = static_cast<std::int16_t>(state.collisionBoundaryYaw + side);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymProbeMovementCollision(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Vector3 forward, leftEnd, rightEnd, leftStart, rightStart;
    Vector2s centerNormal, rightNormal, leftNormal;
    Vector3 centerPoint, rightPoint, leftPoint, delta, position, local;
    float centerDistance, rightDistance, leftDistance;
    std::int32_t centerType, rightType, leftType;
    Character *centerCharacter, *rightCharacter, *leftCharacter;
    std::int16_t yaw, halfWidth, relative;
    leftType = 0;
    rightType = 0;
    centerType = 0;
    leftCharacter = 0;
    rightCharacter = 0;
    centerCharacter = 0;
    state.collisionProbeState = 0;
    state.forwardProbeDistance = 10.0f;
    state.limitingCollisionDistance = 10.0f;
    state.limitingCollisionCharacter = 0;
    state.collisionData = 0;
    state.nearestCollisionCharacter = 0;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&state.worldTransform);
    Matrix4x4::TransformPoint(plymRayForward, forward);
    Matrix4x4::TransformPoint(plymRayRightEnd, rightEnd);
    Matrix4x4::TransformPoint(plymRayLeftEnd, leftEnd);
    Matrix4x4::TransformPoint(plymRayRightStart, rightStart);
    Matrix4x4::TransformPoint(plymRayLeftStart, leftStart);
    PopCurrentMatrix(1);
    centerPoint = forward;
    rightPoint = rightEnd;
    leftPoint = leftEnd;
    centerDistance = rightDistance = leftDistance = 10.0f;
    if (flddRaycastScene(state, state.worldPosition, forward)) {
        centerDistance = state.collisionDistance;
        centerType = static_cast<std::int8_t>(state.collisionType);
        centerPoint = state.fieldCollisionPoint;
        if (static_cast<std::int8_t>(state.collisionType) == 1) {
            if (flddHasFoundIntersectionOfType(0x40U))
                state.collisionData = *flddGetCategoryRecord(6);
            transformIntersectionResult(&centerNormal);
            state.forwardSurfaceYaw = centerNormal.y + 0x8000;
        } else if (static_cast<std::int8_t>(state.collisionType) == 2) {
            centerCharacter = state.collisionCharacter;
            state.forwardSurfaceYaw = state.currentYaw + 0x8000;
        }
        state.nearestCollisionCharacter = state.collisionCharacter;
        state.forwardProbeDistance = centerDistance;
    }
    if (flddRaycastScene(state, rightStart, rightEnd)) {
        rightDistance = state.collisionDistance;
        rightType = static_cast<std::int8_t>(state.collisionType);
        rightPoint = state.fieldCollisionPoint;
        if (static_cast<std::int8_t>(state.collisionType) == 1)
            transformIntersectionResult(&rightNormal);
        else if (static_cast<std::int8_t>(state.collisionType) == 2)
            rightCharacter = state.collisionCharacter;
    }
    if (flddRaycastScene(state, leftStart, leftEnd)) {
        leftDistance = state.collisionDistance;
        leftType = static_cast<std::int8_t>(state.collisionType);
        leftPoint = state.fieldCollisionPoint;
        if (static_cast<std::int8_t>(state.collisionType) == 1)
            transformIntersectionResult(&leftNormal);
        else if (static_cast<std::int8_t>(state.collisionType) == 2)
            leftCharacter = state.collisionCharacter;
    }
    if (centerDistance < rightDistance && centerDistance < leftDistance) {
        state.limitingCollisionDistance = centerDistance;
        if (centerCharacter)
            state.limitingCollisionCharacter = centerCharacter;
        if (centerType == 1)
            state.hitNormalYaw = centerNormal.y;
    } else if (rightDistance < leftDistance) {
        state.limitingCollisionDistance = rightDistance;
        if (rightCharacter)
            state.limitingCollisionCharacter = rightCharacter;
        if (rightType == 1)
            state.hitNormalYaw = rightNormal.y;
    } else {
        state.limitingCollisionDistance = leftDistance;
        if (leftCharacter)
            state.limitingCollisionCharacter = leftCharacter;
        if (leftType == 1)
            state.hitNormalYaw = leftNormal.y;
    }
    if (centerDistance < rightDistance && centerDistance < leftDistance) {
        if (!centerCharacter) {
            state.collisionBoundaryYaw = centerNormal.y;
            state.secondaryCollisionBoundaryYaw = centerNormal.y;
            relative = state.currentYaw - state.collisionBoundaryYaw;
            if (relative > 0)
                relative = 0x4000;
            else
                relative = -0x4000;
            state.collisionLimitYawA = state.collisionBoundaryYaw + relative;
            state.collisionLimitYawB = state.collisionBoundaryYaw + relative;
        } else {
            am2Vector3Subtract(&delta, &centerPoint, &state.worldPosition);
            yaw = Vector3_GetFixedYaw(&delta);
            const float radius = GetCharacterCollisionCapsuleMargin(centerCharacter);
            const float margin = radius > 0.40000001f ? radius : 0.40000001f;
            float ratio = (margin + 0.40000001f) / SquareRoot(delta.x * delta.x + delta.z * delta.z);
            ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
            halfWidth = 0x4000 - ComputeInverseAngleRatio(ratio);
            relative = yaw - state.currentYaw;
            if (relative < 0)
                relative -= 0x4000 - halfWidth;
            else
                relative += 0x4000 - halfWidth;
            state.collisionBoundaryYaw = state.currentYaw + relative;
            state.secondaryCollisionBoundaryYaw = state.currentYaw + relative;
            relative = yaw - state.currentYaw;
            if (relative < 0) {
                state.collisionLimitYawA = yaw + halfWidth;
                state.collisionLimitYawB = yaw - halfWidth;
            } else {
                state.collisionLimitYawA = yaw - halfWidth;
                state.collisionLimitYawB = yaw + halfWidth;
            }
        }
    } else if (state.limitingCollisionCharacter) {
        GetCharacterWorldPosition(state.limitingCollisionCharacter, &position);
        am2Vector3Subtract(&delta, &position, &state.worldPosition);
        yaw = Vector3_GetFixedYaw(&delta);
        const float radius = GetCharacterCollisionCapsuleMargin(state.limitingCollisionCharacter);
        const float margin = radius > 0.40000001f ? radius : 0.40000001f;
        float ratio = (margin + 0.40000001f) / SquareRoot(delta.x * delta.x + delta.z * delta.z);
        ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
        halfWidth = 0x4000 - ComputeInverseAngleRatio(ratio);
        relative = yaw - state.currentYaw;
        if (relative < 0)
            relative -= 0x4000 - halfWidth;
        else
            relative += 0x4000 - halfWidth;
        state.collisionBoundaryYaw = state.currentYaw + relative;
        state.secondaryCollisionBoundaryYaw = state.currentYaw + relative;
        relative = yaw - state.currentYaw;
        if (relative < 0) {
            state.collisionLimitYawA = yaw + halfWidth;
            state.collisionLimitYawB = yaw - halfWidth;
        } else {
            state.collisionLimitYawA = yaw - halfWidth;
            state.collisionLimitYawB = yaw + halfWidth;
        }
    } else {
        am2Vector3Subtract(&delta, &rightPoint, &leftPoint);
        Vector3_GetFixedYaw(&delta);
        state.collisionBoundaryYaw = state.hitNormalYaw;
        state.secondaryCollisionBoundaryYaw = state.hitNormalYaw;
        relative = state.currentYaw - state.collisionBoundaryYaw;
        if (relative > 0)
            relative = 0x4000;
        else
            relative = -0x4000;
        state.collisionLimitYawA = state.collisionBoundaryYaw + relative;
        state.collisionLimitYawB = state.collisionBoundaryYaw + relative;
    }
    if (state.limitingCollisionCharacter) {
        GetCharacterWorldPosition(state.limitingCollisionCharacter, &position);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&state.inverseWorldTransform);
        Matrix4x4::TransformPoint(position, local);
        PopCurrentMatrix(1);
        if ((local.x < 0.0f ? -local.x : local.x) < 0.30000001f)
            state.collisionBoundaryYaw = state.currentYaw;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.forwardProbeDistance = 10.0f;
    state.limitingCollisionDistance = 10.0f;
    state.limitingCollisionCharacter = nullptr;
    state.collisionProbeState = 0;
    state.nearestCollisionCharacter = nullptr;
    state.nearestCharaProbeDistance = 10.0f;
    state.collisionData = 0;
    state.primaryRayCollisionCharacter = nullptr;
    const Vector3 forward = TransformPlymPoint(state.worldTransform, plymRayForward);
    const Vector3 rightEnd = TransformPlymPoint(state.worldTransform, plymRayRightEnd);
    const Vector3 leftEnd = TransformPlymPoint(state.worldTransform, plymRayLeftEnd);
    const Vector3 rightStart = TransformPlymPoint(state.worldTransform, plymRayRightStart);
    const Vector3 leftStart = TransformPlymPoint(state.worldTransform, plymRayLeftStart);

    PlymRayResult center{};
    center.point = forward;
    Character *centerCharacter = nullptr;
    if (flddRaycastScene(state, state.worldPosition, forward)) {
        center.type = state.collisionType;
        center.distance = state.collisionDistance;
        center.point = state.fieldCollisionPoint;
        if (center.type == plymRayHitField) {
            std::int32_t door{};
            if (flddHasFoundIntersectionOfType(0x40) && flddDecodeCategoryScalar(6, door))
                state.collisionData = static_cast<std::uint32_t>(door);
            transformIntersectionResult(center.normal);
            state.forwardSurfaceYaw = static_cast<std::int16_t>(center.normal.y + 0x8000);
        } else if (center.type == plymRayHitCharacter) {
            centerCharacter = state.collisionCharacter;
            state.forwardSurfaceYaw = static_cast<std::int16_t>(state.currentYaw + 0x8000);
        }
        state.primaryRayCollisionCharacter = state.collisionCharacter;
        state.forwardProbeDistance = center.distance;
    }

    PlymRayResult right{};
    right.point = rightEnd;
    PlymRayResult left{};
    left.point = leftEnd;
    CastPlymSideRay(state, rightStart, rightEnd, right, false);
    CastPlymSideRay(state, leftStart, leftEnd, left, false);
    const PlayerMovementState *const movement = state.movementState;
    if ((movement->flags & 8U) != 0 || movement->obstacleDistance < 0.75f) {
        const Vector3 wideRightEnd = TransformPlymPoint(state.worldTransform, plymWideRayRightEnd);
        const Vector3 wideLeftEnd = TransformPlymPoint(state.worldTransform, plymWideRayLeftEnd);
        const Vector3 wideRightStart = TransformPlymPoint(state.worldTransform, plymWideRayRightStart);
        const Vector3 wideLeftStart = TransformPlymPoint(state.worldTransform, plymWideRayLeftStart);
        CastPlymSideRay(state, wideRightStart, wideRightEnd, right, true);
        CastPlymSideRay(state, wideLeftStart, wideLeftEnd, left, true);
    }

    const float centerDistance = center.distance;
    if (centerCharacter) {
        state.nearestCharaProbeDistance = centerDistance;
        state.nearestCollisionCharacter = centerCharacter;
        if (right.character && (!left.character || right.distance <= left.distance) && centerDistance > right.distance) {
            state.nearestCharaProbeDistance = right.distance;
            state.nearestCollisionCharacter = right.character;
        } else if (left.character && centerDistance > left.distance && (!right.character || right.distance > left.distance)) {
            state.nearestCharaProbeDistance = left.distance;
            state.nearestCollisionCharacter = left.character;
        }
    } else if (right.character && (!left.character || right.distance <= left.distance)) {
        state.nearestCharaProbeDistance = right.distance;
        state.nearestCollisionCharacter = right.character;
    } else if (left.character) {
        state.nearestCharaProbeDistance = left.distance;
        state.nearestCollisionCharacter = left.character;
    }

    const bool centerLimits = right.distance > centerDistance && left.distance > centerDistance;
    if (centerLimits) {
        state.limitingCollisionDistance = centerDistance;
        if (centerCharacter)
            state.limitingCollisionCharacter = centerCharacter;
        if (center.type == plymRayHitField)
            state.hitNormalYaw = center.normal.y;
    } else if (left.distance <= right.distance) {
        state.limitingCollisionDistance = left.distance;
        if (left.character)
            state.limitingCollisionCharacter = left.character;
        if (left.type == plymRayHitField)
            state.hitNormalYaw = left.normal.y;
    } else {
        state.limitingCollisionDistance = right.distance;
        if (right.character)
            state.limitingCollisionCharacter = right.character;
        if (right.type == plymRayHitField)
            state.hitNormalYaw = right.normal.y;
    }

    if (centerLimits) {
        if (!centerCharacter) {
            SetPlymWallBoundary(state, center.normal.y);
        } else {
            const Vector3 toHit{center.point.x - state.worldPosition.x, center.point.y - state.worldPosition.y, center.point.z - state.worldPosition.z};
            const float horizontal = std::sqrt(toHit.z * toHit.z + toHit.x * toHit.x);
            const float margin = std::fmax(GetCharacterCollisionCapsuleMargin(centerCharacter), 0.40000001f) + 0.40000001f;
            const float ratioDistance = std::fabs(toHit.x) >= 0.00000011920929f ? margin : 0.0f;
            SetPlymCharacterBoundary(state, PlymAngleXZ(toHit), ratioDistance, std::fabs(toHit.x) >= 0.00000011920929f ? horizontal : 1.0f);
        }
    } else if (state.limitingCollisionCharacter) {
        const Vector3 obstacle = GetCharacterWorldPosition(state.limitingCollisionCharacter);
        const Vector3 toObstacle{obstacle.x - state.worldPosition.x, obstacle.y - state.worldPosition.y, obstacle.z - state.worldPosition.z};
        const float margin = std::fmax(GetCharacterCollisionCapsuleMargin(state.limitingCollisionCharacter), 0.40000001f) + 0.40000001f;
        SetPlymCharacterBoundary(state, PlymAngleXZ(toObstacle), margin, std::sqrt(toObstacle.z * toObstacle.z + toObstacle.x * toObstacle.x));
    } else {
        SetPlymWallBoundary(state, state.hitNormalYaw);
    }
    if (state.limitingCollisionCharacter) {
        const Vector3 local = TransformPlymPoint(state.inverseWorldTransform, GetCharacterWorldPosition(state.limitingCollisionCharacter));
        if (std::fabs(local.x) < 0.30000001f)
            state.collisionBoundaryYaw = state.currentYaw;
    }
#endif
}


// @verified - d
void plymProbeDesiredYawCollision(PlayerModelState &state) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Vector3 forward;
    Vector2s normal;
    Vector3 delta;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    float rightDistance = 10.0f;
    float leftDistance = 10.0f;
#endif
    state.desiredYawProbeDistance = 10.0f;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&state.worldPosition);
    Matrix4x4::RotateAroundY_Wrapper(state.desiredYaw);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(state.worldPosition.x, state.worldPosition.y, state.worldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.desiredYaw));
    Vector3 forward{}, rightEnd{}, leftEnd{}, rightStart{}, leftStart{};
#endif
    Matrix4x4::TransformPoint(plymRayForward, forward);

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    PopCurrentMatrix(1);
    if (!flddRaycastScene(state, state.worldPosition, forward))
        return;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    Matrix4x4::TransformPoint(plymRayRightEnd, rightEnd);
    Matrix4x4::TransformPoint(plymRayLeftEnd, leftEnd);
    Matrix4x4::TransformPoint(plymRayRightStart, rightStart);
    Matrix4x4::TransformPoint(plymRayLeftStart, leftStart);
    Matrix4x4::Pop(1);
    if (flddRaycastScene(state, state.worldPosition, forward)) {
#endif
    state.desiredYawProbeDistance = state.collisionDistance;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (static_cast<std::int8_t>(state.collisionType) == 1) {
        transformIntersectionResult(&normal);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        if (state.collisionType == plymRayHitField) {
            Vector3s normal{};
            transformIntersectionResult(normal);
#endif
        state.desiredProbeYaw = static_cast<std::int16_t>(normal.y + 0x4000);
    }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (static_cast<std::int8_t>(state.collisionType) == 2) {
        am2Vector3Subtract(&delta, &state.fieldCollisionPoint, &state.worldPosition);
        const std::int16_t yaw = Vector3_GetFixedYaw(&delta);
        float ratio = 0.80000001f / SquareRoot(delta.x * delta.x + delta.z * delta.z);
        ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
        const std::int16_t halfWidth = 0x4000 - ComputeInverseAngleRatio(ratio);
        std::int16_t relative = yaw - state.desiredYaw;
        if (relative < 0)
            relative += halfWidth;
        else
            relative -= halfWidth;
        state.desiredProbeYaw = state.desiredYaw + relative;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        if (state.collisionType == plymRayHitCharacter) {
            const Vector3 toHit{state.fieldCollisionPoint.x - state.worldPosition.x, state.fieldCollisionPoint.y - state.worldPosition.y,
                state.fieldCollisionPoint.z - state.worldPosition.z};
            const std::int16_t bearing = PlymAngleXZ(toHit);
            const float ratio = std::fmin(1.0f, std::fmax(0.0f, 0.80000001f / std::sqrt(toHit.z * toHit.z + toHit.x * toHit.x)));
            const std::uint16_t inverse = ComputeInverseAngleRatio(ratio);
            const auto relative = static_cast<std::int16_t>(bearing - state.desiredYaw);
            const auto offset = static_cast<std::int16_t>(relative >= 0 ? relative - (0x4000 - inverse) : 0x4000 - inverse + relative);
            state.desiredProbeYaw = static_cast<std::int16_t>(state.desiredYaw + offset);
#endif
    }

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    }
    const float forwardDistance = state.desiredYawProbeDistance;
    if (flddRaycastScene(state, leftStart, leftEnd))
        leftDistance = state.collisionDistance;
    if (flddRaycastScene(state, rightStart, rightEnd))
        rightDistance = state.collisionDistance;
    if (forwardDistance > leftDistance && leftDistance > rightDistance && leftDistance < 0.80000001f && rightDistance < 0.80000001f)
        state.desiredYawProbeDistance = (leftDistance + rightDistance) * 0.5f;
#endif
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ProbeReachedOrdinaryMovementCollision(PlayerMovementState &movement) {
    movement.flags &= ~0x80U;
    movement.obstacleDistance = 10.0f;
    movement.pendingPositionCorrection = 0.0f;
    movement.correctionCycleDistance = 0.0f;
    movement.correctionCycleCount = 0;

    const Vector3 location = GetCharacterWorldPosition(movement.character);
    Vector3 nearPoint{location.x, location.y + 0.22499999f, location.z};
    const Vector3 farPoint = RotateAngleAxis(nearPoint, 10.0f, movement.currentYaw);
    Vector3 intersection{};
    flddPushIntersectionFlag(0x20);
    bool found = FindNearestRayIntersection(&intersection, &farPoint, &nearPoint) != 0;
    if (!found) {
        nearPoint.y = location.y - 0.22499999f;
        flddPushIntersectionFlag(0x20);
        found = FindNearestRayIntersection(&intersection, &nearPoint, &farPoint) != 0;
    }
    if (!found || !flddHasFoundIntersectionOfType(0x20U)) {
        movement.pendingPositionCorrection = 0.0f;
        return;
    }

    // Native consumes these outputs even when the radius query leaves them unwritten.
    Vector3 stairAnchor;
    std::int32_t stairYaw;
    const std::int32_t stepCount = flddQueryStairOrigin(intersection, stairAnchor, stairYaw);
    const std::int32_t selectedStep = location.y - stairAnchor.y > static_cast<float>(stepCount) * 0.22499999f * 0.5f ? stepCount : 0;
    Vector3 wallPoint{0.0f, static_cast<float>(selectedStep) * 0.22499999f, selectedStep ? -(static_cast<float>(selectedStep - 1) * 0.30000001f) : 0.15f};
    Vector3 wallEnd{1.0f, 0.0f, 0.0f};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(stairAnchor.x, stairAnchor.y, stairAnchor.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(stairYaw));
    Matrix4x4::preTranslate(wallPoint.x, wallPoint.y, wallPoint.z);
    wallPoint = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::TransformPoint(wallEnd, wallEnd);
    Matrix4x4::Pop(1);
    ComputeVectorProjection(intersection, nearPoint, intersection, wallPoint, wallEnd);
    const float projectedX = nearPoint.x - intersection.x;
    const float projectedZ = nearPoint.z - intersection.z;
    movement.obstacleDistance = std::sqrt(projectedX * projectedX + projectedZ * projectedZ);

    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.currentYaw));
    Matrix4x4::preTranslate(-nearPoint.x, -nearPoint.y, -nearPoint.z);
    Matrix4x4::TransformPoint(intersection, local);
    Matrix4x4::Pop(1);
    if (local.z > 0.0f)
        movement.obstacleDistance = 0.0f;

    MotionWork *const motionWork = movement.motionWork;
    if (!motionWork)
        return;
    const std::uint16_t motionId = mtwkGetAnimationId(motionWork, MotionRegion::LowerBody);
    const bool correctionMotion = motionId == 0xf005U || motionId == 0xf006U || (motionId >= 0xf035U && motionId <= 0xf03dU);
    // Native compares the sign-extended target yaw with the full transformed wall yaw.
    if (!correctionMotion ||
        ((movement.flags & 8U) != 0 && static_cast<std::int32_t>(movement.targetWorldYaw) == static_cast<std::int32_t>(stairYaw) &&
            static_cast<std::int32_t>(movement.targetHeightScale) == stepCount)) {
        return;
    }

    const float scale = movement.field160 * movement.field15c;
    Vector3 terminalRoot;
    SampleNativeMotionRoot(motionId, mtwkGetRigBoneId(motionWork), -1.0f, terminalRoot);
    const float trackDistance = std::fabs(terminalRoot.z);
    const float sampleFrame = mtwkGetFrame(motionWork, MotionRegion::LowerBody);
    const float frameCount = static_cast<float>(GetMotionFrameCount(motionId)) - 1.0f;

    const float halfCycle = frameCount * 0.5f;
    float phase = halfCycle - sampleFrame;
    if (phase < 0.0f)
        phase += halfCycle;
    const float baseCycleDistance = phase / scale;
    const float perFrameDistance = trackDistance / frameCount;
    const float travelled = baseCycleDistance * (perFrameDistance * scale);
    const float obstacleDistance = movement.obstacleDistance;
    if (!(travelled >= obstacleDistance)) {
        const float cycleDistance = trackDistance * 0.5f;
        std::int32_t cycleCount = TruncateToInt32((obstacleDistance - travelled) / cycleDistance);
        float remainder = obstacleDistance - travelled - static_cast<float>(cycleCount) * cycleDistance;
        if (remainder > cycleDistance * 0.5f) {
            remainder -= cycleDistance;
            ++cycleCount;
        }
        movement.correctionCycleCount = static_cast<std::int16_t>(cycleCount);
        movement.pendingPositionCorrection = remainder;
        movement.correctionCycleDistance = static_cast<float>(cycleCount) * halfCycle + baseCycleDistance;
    } else {
        movement.flags |= 0x80U;
        movement.correctionCycleDistance = baseCycleDistance;
        movement.correctionCycleCount = 0;
        movement.pendingPositionCorrection = -(travelled - obstacleDistance);
    }
    if ((movement.flags & 0x00100000U) != 0 && obstacleDistance < 0.25f) {
        movement.pendingPositionCorrection = obstacleDistance;
    }
}

// @verified - p
void UpdateReachedFloorStairCollision(PlayerMovementState &movement) {
    Character *const character = movement.character;
    std::int32_t yaw = 0;
    bool publishTarget = false;
    bool retainTarget = false;
    ProbeReachedOrdinaryMovementCollision(movement);
    MotionWork *const motionWork = movement.motionWork;
    mtwkGetAnimationId(motionWork, MotionRegion::LowerBody);
    Vector3 location = GetCharacterWorldPosition(character);
    location.y += 1.0f;
    movement.flags &= ~0x10000U;
    movement.sceneGroundIndex = static_cast<std::int8_t>(scenFindIntersection(location, location.y));
    if (movement.sceneGroundIndex != -1)
        movement.flags |= 0x10000U;

    location = GetCharacterWorldPosition(character);
    Vector3 probe{location.x, static_cast<float>(static_cast<double>(location.y) + 1.5), location.z};
    const float ground = shenFindIntersectionWithScene(probe, probe.y);
    // Native leaves the first anchor, and the no-ground edge length, unwritten.
    Vector3 anchor;
    float edgeLength;
    std::int32_t stepCount = 0;
    if (ground != -std::numeric_limits<float>::max()) {
        probe.y = ground;
        stepCount = flddQueryStairRadius(probe, anchor, yaw, edgeLength);
    }
    const Vector3 upperProbe{probe.x, static_cast<float>(static_cast<double>(probe.y) + 1.5), probe.z};
    Vector3 upperAnchor{};
    std::int32_t upperYaw = 0;
    std::int32_t connectionId = 0;
    const std::int32_t upperCount = static_cast<std::int32_t>(flddQueryStairConnection(upperProbe, upperProbe.y, upperAnchor, upperYaw, connectionId));
    if (upperCount > 0 && std::fabs(anchor.y - upperAnchor.y) >= 0.22499999f) {
        stepCount = upperCount;
        anchor = upperAnchor;
        yaw = upperYaw;
    }

    Vector3 local{};
    if (stepCount <= 0) {
        if (movement.flags & 8U)
            SetCharacterControlModelFlags(character, movement.cachedCharacterControlFlags & ~0x800U);
        movement.flags &= 0xfffbfff7U;
    } else {
        const std::uint32_t controlFlags = GetCharacterControlModelFlags(character);
        const std::uint32_t flags = movement.flags;
        if (!(flags & 8U)) {
            movement.targetWorldPosition = anchor;
            movement.groundedStepIndex = 0;
            movement.targetStairEdgeLength = edgeLength;
            movement.targetWorldYaw = static_cast<std::int16_t>(yaw);
            movement.targetHeightScale = static_cast<std::int8_t>(stepCount);
            movement.cachedCharacterControlFlags = controlFlags;
            publishTarget = true;
        } else if (std::fabs(movement.targetWorldPosition.y - anchor.y) >= 0.22499999f) {
            if (flags & 0x40000U) {
                movement.targetWorldPosition = anchor;
                movement.targetStairEdgeLength = edgeLength;
                movement.targetWorldYaw = static_cast<std::int16_t>(yaw);
                movement.targetHeightScale = static_cast<std::int8_t>(stepCount);
                publishTarget = true;
            } else {
                anchor = movement.targetWorldPosition;
                yaw = movement.targetWorldYaw;
                stepCount = movement.targetHeightScale;
                edgeLength = movement.targetStairEdgeLength;
                retainTarget = true;
            }
        }
        movement.flags = (flags & ~0x40000U) | 8U;
        if (retainTarget)
            movement.flags |= 0x40000U;
        std::uint32_t nextFlags = (controlFlags & 0xfffff5bdU) | 0x100U;
        if (movement.rootYawMode == 0 && !(movement.flags & 0x100U))
            nextFlags |= 0x242U;
        else if (movement.playerMotionFlags & 2U)
            nextFlags |= 0x800U;
        SetCharacterControlModelFlags(character, nextFlags);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
        Matrix4x4::preTranslate(-anchor.x, -anchor.y, -anchor.z);
        Matrix4x4::TransformPoint(location, local);
        Matrix4x4::Pop(1);
        if (-0.01 > static_cast<double>(local.y))
            SetCharacterControlModelFlags(character, nextFlags | 0x40U);
    }

    const std::int32_t yawDistance = std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(motionWork->runtime.entityRotation.y - yaw)));
    const std::int32_t direction = yawDistance < 0x4000 ? 1 : -1;
    if (publishTarget)
        movement.groundedStepIndex = static_cast<std::int8_t>(yawDistance < 0x4000 ? 0 : stepCount);

    std::int32_t upperBodyStep = 0;
    std::int32_t footStep = 0;
    std::int32_t heightStep = 0;
    Vector3 rightFoot{};
    Vector3 leftFoot{};
    if (stepCount > 0) {
        Vector3 rightFootBone{};
        Vector3 leftFootBone{};
        mtwkGetControllerTargetPosition(motionWork, 6, rightFoot);
        mtwkGetControllerTargetPosition(motionWork, 5, leftFoot);
        mtwkGetBonePosition(motionWork, MB_RightFoot, &rightFootBone);
        mtwkGetBonePosition(motionWork, MB_LeftFoot, &leftFootBone);
        const Vector3 &probeFoot = mtwkIsRightHandAboveShoulder(motionWork) ? leftFoot : rightFoot;
        Vector3 footProbe{probeFoot.x, static_cast<float>(static_cast<double>(probeFoot.y) + 1.0), probeFoot.z};
        footProbe.y = shenFindIntersectionWithScene(footProbe, footProbe.y);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
        Matrix4x4::preTranslate(-anchor.x, -anchor.y, -anchor.z);
        Matrix4x4::TransformPoint(location, local);
        Matrix4x4::TransformPoint(rightFoot, rightFoot);
        Matrix4x4::TransformPoint(leftFoot, leftFoot);
        Matrix4x4::TransformPoint(rightFootBone, rightFootBone);
        Matrix4x4::TransformPoint(leftFootBone, leftFootBone);
        Matrix4x4::Pop(1);
        const float rightMidZ = -(rightFoot.z + rightFootBone.z);
        const float leftMidZ = -(leftFoot.z + leftFootBone.z);
        local.z = -local.z;
        upperBodyStep = 1 - static_cast<std::int32_t>(local.z / -0.30000001f);
        upperBodyStep = upperBodyStep < 0 ? 0 : (upperBodyStep < stepCount ? upperBodyStep : stepCount);
        std::int32_t rightStep = 1 - static_cast<std::int32_t>((rightMidZ * -0.5f) / 0.30000001f);
        rightStep = rightStep < 0 ? 0 : (rightStep < stepCount ? rightStep : stepCount);
        footStep = 1 - static_cast<std::int32_t>((leftMidZ * -0.5f) / 0.30000001f);
        footStep = footStep < 0 ? 0 : (footStep < stepCount ? footStep : stepCount);
        if (rightStep < footStep)
            footStep = rightStep;
        heightStep = static_cast<std::int32_t>(leftFoot.y / 0.22499999f);
        if (static_cast<std::int32_t>(rightFoot.y / 0.22499999f) < heightStep)
            heightStep = static_cast<std::int32_t>(rightFoot.y / 0.22499999f);
        heightStep = heightStep < 0 ? 0 : (heightStep < stepCount ? heightStep : stepCount);
    }
    if (movement.flags & 8U)
        movement.upperBodyStepIndex = static_cast<std::int8_t>(upperBodyStep);
    movement.stepDirection = 0;
    if (stepCount > 0) {
        if (direction == 1) {
            if (local.z > -0.30000001f && static_cast<float>(stepCount - 1) * 0.30000001f > local.z && footStep < stepCount && heightStep < stepCount) {
                movement.stepDirection = 1;
            }
        } else if (local.z > -0.075000003f && static_cast<float>(stepCount) * 0.30000001f > local.z && footStep > 0 && heightStep > 0) {
            movement.stepDirection = -1;
        }
        std::int32_t groundedStep = static_cast<std::int32_t>(leftFoot.y / 0.22499999f);
        if (static_cast<std::int32_t>(rightFoot.y / 0.22499999f) < groundedStep)
            groundedStep = static_cast<std::int32_t>(rightFoot.y / 0.22499999f);
        if (groundedStep < 0)
            groundedStep = 0;
        movement.groundedStepIndex = static_cast<std::int8_t>(groundedStep < movement.targetHeightScale ? groundedStep : movement.targetHeightScale);
    }
    if (publishTarget)
        movement.groundedStepIndex = static_cast<std::int8_t>(yawDistance < 0x4000 ? 0 : stepCount);
    if (movement.stepDirection && movement.targetHeightScale > 0) {
        const std::int8_t height = movement.targetHeightScale;
        std::uint16_t boundaryYaw = static_cast<std::uint16_t>(yaw);
        Vector3 boundary{-1.8f, 0.0f, 0.30000001f};
        if (movement.stepDirection > 0) {
            boundary.y = static_cast<float>(height) * 0.22499999f;
            boundary.z = static_cast<float>(-height) * 0.30000001f;
        } else {
            boundaryYaw = static_cast<std::uint16_t>(boundaryYaw + 0x8000);
        }
        movement.stairBoundaryYaw = static_cast<std::int16_t>(boundaryYaw);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(anchor.x, anchor.y, anchor.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        Matrix4x4::TransformPoint(boundary, movement.stairBoundaryAnchor);
        Matrix4x4::Pop(1);
    }
}

// @verified - p
void PrepareReachedStairPhaseCorrection(PlayerMovementState &movement) {
    MotionWork *const motionWork = movement.motionWork;
    const std::uint16_t motionId = mtwkGetAnimationId(motionWork, MotionRegion::LowerBody);
    const bool standardStairMotion = motionId == 0xf008U || motionId == 0xf009U || motionId == 0xf02aU || motionId == 0xf02bU;
    const bool trackedStairMotion = motionId == 0xf12eU || motionId == 0xf12fU;
    if ((!standardStairMotion && !trackedStairMotion) || (movement.flags & 8U) == 0 || movement.targetHeightScale == 0)
        return;

    movement.correctionCycleDistance = 0.0f;
    Vector3 terminalRoot;
    SampleNativeMotionRoot(motionId, mtwkGetRigBoneId(motionWork), -1.0f, terminalRoot);
    const float trackDistance = std::fabs(terminalRoot.z);
    const float sampleFrame = mtwkGetFrame(motionWork, MotionRegion::LowerBody);
    const float frameCount = static_cast<float>(GetMotionFrameCount(motionId)) - 1.0f;
    const float perFrameDistance = trackDistance / frameCount;
    const std::int32_t stepsPerPhase = trackedStairMotion || motionId == 0xf008U || motionId == 0xf02aU ? 2 : 1;
    const float phaseLength = frameCount / (trackedStairMotion ? 1.0f : 2.0f);
    float phase = phaseLength - sampleFrame;
    if (phase < 0.0f)
        phase += phaseLength;

    const std::int16_t yawDistance = static_cast<std::int16_t>(motionWork->runtime.entityRotation.y - movement.targetWorldYaw);
    const std::int32_t phaseStep = stepsPerPhase * (std::abs(static_cast<std::int32_t>(yawDistance)) < 0x4000 ? 1 : -1);
    std::int32_t stepIndex = static_cast<std::int32_t>(movement.previousStepIndex) + phaseStep;
    if (sampleFrame > phaseLength)
        stepIndex += phaseStep;

    Vector3 stairPoint{0.0f, static_cast<float>(stepIndex) * 0.22499999f, -(static_cast<float>(stepIndex) * 0.30000001f - 0.15f)};
    Vector3 stairEnd{1.0f, stairPoint.y, stairPoint.z};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(movement.targetWorldPosition.x, movement.targetWorldPosition.y, movement.targetWorldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(movement.targetWorldYaw));
    Matrix4x4::TransformPoint(stairPoint, stairPoint);
    Matrix4x4::TransformPoint(stairEnd, stairEnd);
    Matrix4x4::Pop(1);
    Vector3 location = GetCharacterWorldPosition(movement.character);
    const Vector3 ahead = RotateAngleAxis(location, 1.0f, GetAllAngYForNode(&movement.character->entity.node));
    Vector3 projected{};
    ComputeVectorProjection(projected, location, ahead, stairPoint, stairEnd);
    projected.y = stairPoint.y;
    location = GetCharacterWorldPosition(movement.character);
    const float projectedDistance = GetPlanarDistanceOrZero(location, projected);
    movement.correctionCycleDistance = phase;
    movement.correctionCycleCount = 0;
    movement.pendingPositionCorrection = projectedDistance - phase * perFrameDistance;
}

void ApplyReachedCollisionRootCorrection(PlayerMovementState &movement) {
    // PC v1.07 ApplyReachedCollisionRootCorrection consumes the FLDD obstruction handoff. Its
    // +0x164 input is the current root-motion step, not motion-speed field
    // +0x160.
    const float pendingMagnitude = std::fabs(movement.pendingPositionCorrection);
    if (pendingMagnitude <= 0.0f || !movement.character)
        return;

    float stepLimit = movement.rootMotionStep * ((movement.flags & 8U) != 0 ? 0.85000002f : 0.5f);
    stepLimit /= static_cast<float>(movement.correctionCycleCount + 1);
    stepLimit = std::fmax(stepLimit, 0.02f);
    float appliedStep = std::fmin(stepLimit, pendingMagnitude);
    if (movement.pendingPositionCorrection < 0.0f) {
        appliedStep = -appliedStep;
    }
    movement.pendingPositionCorrection -= appliedStep;
    if (std::fabs(movement.pendingPositionCorrection) < 0.02f) {
        movement.pendingPositionCorrection = 0.0f;
    }

    // TransformBackwardDirection starts from (0, 0, -1), scales by the
    // signed correction, then rotates by currentYaw.
    const float sine = sin_fixed16(movement.currentYaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(movement.currentYaw))));
    Vector3 corrected = GetCharacterWorldPosition(movement.character);
    corrected.x -= sine * appliedStep;
    corrected.z -= cosine * appliedStep;
    shenApplyTranslationToAll(&movement.character->entity.node, corrected);

    const float remaining = movement.pendingPositionCorrection;
    if (std::fabs(remaining) > movement.correctionCycleDistance * stepLimit && static_cast<std::int8_t>(movement.flags & 0xffU) < 0 &&
        movement.correctionCycleDistance > 0.0f) {
        float motionScale = std::fmax(0.75f, ((remaining / movement.correctionCycleDistance) + movement.rootMotionStep) / movement.rootMotionStep);
        if (motionScale >= 1.25f)
            motionScale = 1.25f;
        movement.field16c = motionScale;
    }
}

float plymProbeSpeedRatio(float distance, float speed) {
    const float lower = (speed - 1.0f) * 0.2f + 0.80000001f;
    return std::fmin(1.0f, std::fmax(0.0f, (distance - lower) / (5.0f - lower)));
}

float PlymDistance(const Vector3 &left, const Vector3 &right) {
    return std::sqrt(PlymSquaredDistance(left, right));
}

Vector3 PlymToLocal(std::int16_t yaw, const Vector3 &origin, const Vector3 &point) {
    Vector3 local{};
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    Matrix4x4::TransformPoint(point, local);
    Matrix4x4::Pop(1);
    return local;
}

Vector3 PlymRotatedWorldProbe(const PlayerModelState &state, std::int16_t yaw, const Vector3 &probe) {
    Vector3 point{};
    Matrix4x4_Push(&state.worldTransform);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::TransformPoint(probe, point);
    Matrix4x4::Pop(1);
    return point;
}

// Native two-wall side probe: 2 m field rays either side publish the wall
// distances, the rotation-only lead foot and a wall-parallel turn target.
void plymProbeSideWalls(PlayerModelState &state) {
    state.movementSelectionFlags &= ~selectionObstacleTurnCandidate;
    std::int32_t hits = 0;
    std::int16_t leftYaw = 0;
    std::int16_t rightYaw = 0;
    state.leftSideProbeDistance = 10.0f;
    state.rightSideProbeDistance = 10.0f;
    const Vector3 left = TransformPlymPoint(state.worldTransform, plymSideProbeLeft);
    const Vector3 right = TransformPlymPoint(state.worldTransform, plymSideProbeRight);
    Vector3 hit{};
    flddToggleFlag(1064);
    if (RaycastForCollision(&hit, &left, &state.worldPosition)) {
        state.leftSideProbeDistance = PlymDistance(state.worldPosition, hit);
        if (state.leftSideProbeDistance < 0.75f) {
            Vector3s normal{};
            transformIntersectionResult(normal);
            hits = 1;
            leftYaw = static_cast<std::int16_t>(normal.y + (static_cast<std::int16_t>(state.currentYaw - normal.y) <= 0 ? -0x4000 : 0x4000));
        }
    }
    flddToggleFlag(1064);
    if (RaycastForCollision(&hit, &right, &state.worldPosition)) {
        state.rightSideProbeDistance = PlymDistance(state.worldPosition, hit);
        if (state.rightSideProbeDistance < 0.75f) {
            Vector3s normal{};
            transformIntersectionResult(normal);
            rightYaw = static_cast<std::int16_t>(normal.y + (static_cast<std::int16_t>(state.currentYaw - normal.y) <= 0 ? -0x4000 : 0x4000));
            ++hits;
        }
    }
    if (state.leftSideProbeDistance != state.rightSideProbeDistance)
        state.movementSubtype = state.leftSideProbeDistance > state.rightSideProbeDistance ? -1 : 1;
    if (hits) {
        state.requestedTurnYaw = static_cast<std::int16_t>(hits == 2 ? leftYaw + static_cast<std::int16_t>(rightYaw - leftYaw) / 2 : leftYaw + rightYaw);
        state.movementSelectionFlags |= selectionObstacleTurnCandidate;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateGroundBoundaryTurn(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const PlayerMovementState *const movement = state.movementState;
    Vector3 anchorLocal, playerLocal;
    std::int32_t yaw;
    state.groundBoundaryTurnYawDelta = 0;
    if (movement->flags & 8U) {
        state.movementSelectionFlags |= 0x4000U;
        state.movementSelectionFlags &= ~0x2000U;
        return;
    }
    if (movement->sceneGroundIndex == -1) {
        state.movementSelectionFlags &= ~0x4000U;
        state.movementSelectionFlags &= ~0x2000U;
        return;
    }
    if (movement->sceneGroundIndex != 1 && movement->sceneGroundIndex != 3 && movement->sceneGroundIndex != 5) {
        state.movementSelectionFlags &= ~0x4000U;
        state.movementSelectionFlags &= ~0x2000U;
        return;
    }
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-state.currentYaw);
    matrix_stack::TranslateInverseVector(&state.searchLocation);
    Matrix4x4::TransformPoint(movement->stairBoundaryAnchor, anchorLocal);
    PopCurrentMatrix(1);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement->stairBoundaryYaw);
    matrix_stack::TranslateInverseVector(&movement->stairBoundaryAnchor);
    Matrix4x4::TransformPoint(state.searchLocation, playerLocal);
    PopCurrentMatrix(1);
    if (!(state.movementSelectionFlags & 0x2000U)) {
        if (!(state.movementSelectionFlags & 0x4000U))
            return;
        if (playerLocal.z > -0.80000001f) {
            if (anchorLocal.x < 0.0f) {
                if (static_cast<std::int8_t>(state.packedDirection) < -20)
                    state.movementSelectionFlags |= 0x2000U;
                else if (static_cast<std::int8_t>(state.packedDirection) > 20)
                    state.movementSelectionFlags &= ~0x4000U;
            } else {
                if (static_cast<std::int8_t>(state.packedDirection) > 20)
                    state.movementSelectionFlags |= 0x2000U;
                else if (static_cast<std::int8_t>(state.packedDirection) < -20)
                    state.movementSelectionFlags &= ~0x4000U;
            }
        } else
            state.movementSelectionFlags &= ~0x4000U;
    }
    if (!(state.movementSelectionFlags & 0x2000U))
        return;
    state.movementSelectionFlags &= ~0x4000U;
    if (state.currentMovementMode == 2 || state.currentMovementMode == 6)
        state.movementSelectionFlags &= ~0x2000U;
    if (anchorLocal.x < 0.0f) {
        if (static_cast<std::int8_t>(state.packedDirection) > 20)
            state.movementSelectionFlags &= ~0x2000U;
    } else if (static_cast<std::int8_t>(state.packedDirection) < -20)
        state.movementSelectionFlags &= ~0x2000U;
    if (!(state.movementSelectionFlags & 0x2000U))
        return;
    playerLocal.z += 0.80000001f;
    if (playerLocal.z > 0.0f)
        yaw = movement->stairBoundaryYaw;
    else {
        yaw = dc::sdk::ArcTan2Fixed(playerLocal.x, playerLocal.z) - 0x8000;
        yaw = static_cast<std::int16_t>(yaw);
        const std::int32_t boundary = movement->stairBoundaryYaw - 0x4000;
        yaw += boundary;
        yaw = static_cast<std::int16_t>(yaw);
    }
    std::int32_t delta = static_cast<std::int16_t>(yaw - state.currentYaw);
    if (delta < 0)
        delta = -delta;
    if (delta > 0x4000)
        yaw = static_cast<std::int16_t>(yaw - 0x8000);
    if (!(playerLocal.z > 0.0f)) {
        if (anchorLocal.x < 0.0f)
            yaw = static_cast<std::int16_t>(yaw + 2048);
        else
            yaw = static_cast<std::int16_t>(yaw - 2048);
    }
    state.groundBoundaryTurnYawDelta = static_cast<std::int16_t>(yaw - state.currentYaw);
    state.turnSpeedScale = 8.0f;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const PlayerMovementState &movement = *state.movementState;
    state.groundBoundaryTurnYawDelta = 0;
    if (movement.flags & 8U) {
        state.movementSelectionFlags = (state.movementSelectionFlags & ~selectionGroundBoundaryTurnActive) | selectionGroundBoundaryTracking;
        return;
    }
    const auto ground = static_cast<std::uint8_t>(movement.sceneGroundIndex);
    if (ground == 0xffU || ((ground - 1U) & 0xf9U) != 0 || ground == 7U) {
        state.movementSelectionFlags &= ~(selectionGroundBoundaryTurnActive | selectionGroundBoundaryTracking);
        return;
    }
    const Vector3 anchorLocal = PlymToLocal(state.currentYaw, state.searchLocation, movement.stairBoundaryAnchor);
    Vector3 playerLocal = PlymToLocal(movement.stairBoundaryYaw, movement.stairBoundaryAnchor, state.searchLocation);
    const auto packedX = static_cast<std::int8_t>(state.packedDirection & 0xffU);
    if ((state.movementSelectionFlags & selectionGroundBoundaryTurnActive) == 0) {
        if ((state.movementSelectionFlags & selectionGroundBoundaryTracking) == 0)
            return;
        bool keepTracking = false;
        if (playerLocal.z > -0.80000001f) {
            if (anchorLocal.x >= 0.0f) {
                if (packedX > 20)
                    state.movementSelectionFlags |= selectionGroundBoundaryTurnActive;
                keepTracking = packedX >= -20;
            } else {
                if (packedX < -20)
                    state.movementSelectionFlags |= selectionGroundBoundaryTurnActive;
                keepTracking = packedX <= 20;
            }
        }
        if (!keepTracking)
            state.movementSelectionFlags &= ~selectionGroundBoundaryTracking;
    }
    if ((state.movementSelectionFlags & selectionGroundBoundaryTurnActive) == 0)
        return;
    state.movementSelectionFlags &= ~selectionGroundBoundaryTracking;
    if (((state.movementMode - 2U) & 0xfffffffbU) == 0)
        state.movementSelectionFlags &= ~selectionGroundBoundaryTurnActive;
    if (anchorLocal.x >= 0.0f ? packedX < -20 : packedX > 20)
        state.movementSelectionFlags &= ~selectionGroundBoundaryTurnActive;
    if ((state.movementSelectionFlags & selectionGroundBoundaryTurnActive) == 0)
        return;
    playerLocal.z += 0.80000001f;
    const std::int16_t edgeYaw = playerLocal.z <= 0.0f ?
        static_cast<std::int16_t>(FixedAtan2(playerLocal.x, playerLocal.z) + 0x8000 + movement.stairBoundaryYaw - 0x4000) :
        movement.stairBoundaryYaw;
    std::int16_t turnYaw = Abs16(edgeYaw - state.currentYaw) <= 0x4000U ? edgeYaw : static_cast<std::int16_t>(edgeYaw + 0x8000);
    if (playerLocal.z <= 0.0f)
        turnYaw = static_cast<std::int16_t>(turnYaw + (anchorLocal.x >= 0.0f ? -2048 : 2048));
    state.turnSpeedScale = 8.0f;
    state.groundBoundaryTurnYawDelta = static_cast<std::int16_t>(turnYaw - state.currentYaw);
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
void plymProbeSideWalls(PlayerModelState &state) {
    static const Vector3 leftProbe = {2.0f, 0.0f, 0.0f};
    static const Vector3 rightProbe = {-2.0f, 0.0f, 0.0f};
    std::int16_t count = 0, leftYaw = 0, rightYaw = 0;
    Vector3 left, right, hit;
    Vector2s leftNormal, rightNormal;
    state.movementSelectionFlags &= ~0x10U;
    state.leftSideProbeDistance = 10.0f;
    state.rightSideProbeDistance = 10.0f;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&state.worldTransform);
    Matrix4x4::TransformPoint(leftProbe, left);
    Matrix4x4::TransformPoint(rightProbe, right);
    PopCurrentMatrix(1);
    flddToggleFlag(0x428U);
    if (FindFieldSegmentOffsetInWorld(&hit, &left, &state.worldPosition)) {
        state.leftSideProbeDistance = CalculateDistanceBetweenPoints(&state.worldPosition, &hit);
        if (state.leftSideProbeDistance < 0.75f) {
            transformIntersectionResult(&leftNormal);
            leftYaw = static_cast<std::int16_t>(state.currentYaw - leftNormal.y) > 0 ? 0x4000 : -0x4000;
            leftYaw = static_cast<std::int16_t>(leftNormal.y + leftYaw);
            ++count;
        }
    }
    flddToggleFlag(0x428U);
    if (FindFieldSegmentOffsetInWorld(&hit, &right, &state.worldPosition)) {
        state.rightSideProbeDistance = CalculateDistanceBetweenPoints(&state.worldPosition, &hit);
        if (state.rightSideProbeDistance < 0.75f) {
            transformIntersectionResult(&rightNormal);
            const std::int32_t yawDelta = static_cast<std::int16_t>(state.currentYaw - rightNormal.y);
            const std::int32_t yawOffset = yawDelta > 0 ? 0x4000 : -0x4000;
            rightYaw = static_cast<std::int16_t>(rightNormal.y + yawOffset);
            ++count;
        }
    }
    if (count) {
        if (count == 2) {
            const std::int16_t delta = rightYaw - leftYaw;
            state.requestedTurnYaw = static_cast<std::int16_t>(leftYaw + delta / count);
        } else
            state.requestedTurnYaw = static_cast<std::int16_t>(leftYaw + rightYaw);
        state.movementSelectionFlags |= 0x10U;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymMatchCharacterSpeed(PlayerModelState &state) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (state.limitingCollisionDistance > 3.0f || state.movementInterpolation > 0.0f)
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (state.limitingCollisionDistance > 3.0f || state.normalizedMotionInput > 0.0f)
#endif
        return;
    Character *const character = state.limitingCollisionCharacter;
    const shen_name name = ctrlGetScenarioCharaName(character);

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (!character || !IsNpcRouteMoving(name))
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (!character)
#endif
        return;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int32_t yawDelta = static_cast<std::int16_t>(GetCharacterWorldYaw(character) - state.currentYaw);
    if (yawDelta < 0)
        yawDelta = -yawDelta;
    if (yawDelta > 0x2000)
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    NpcRouteStateContext *const route = FindNpcRouteStateContext(name);
    if (!route || route->owner->liveState != 1U || (route->flags & 3U) == 0 || (route->flags & 0x101008U) != 0 || route->moveState == 3U)
#endif
        return;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-state.currentYaw);
    matrix_stack::TranslateInverseVector(&state.worldPosition);
    Matrix4x4::TransformPoint(position, position);
    PopCurrentMatrix(1);
    if ((position.x < 0.0f ? -position.x : position.x) > 0.40000001f)
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (Abs16(GetAllAngYForNode(&character->entity.node) - state.currentYaw) > 0x2000U)
#endif
        return;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.movementSelectionFlags |= 0x20000U;
    const float step = GetNpcRouteCurrentStep(name);
    if (!(std::fabs(step) < 0.00000011920929f))
        state.movementResult = step * 0.97500002f;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const Vector3 local = PlymToLocal(state.currentYaw, state.worldPosition, GetCharacterWorldPosition(character));
    if (std::fabs(local.x) > 0.40000001f)
        return;
    state.movementSelectionFlags |= selectionCharacterSpeedMatch;
    if (std::fabs(route->motionStep) >= 0.00000011920929f)
        state.movementResult = route->motionStep * 0.97500002f;
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int32_t plymRaycastFromModel(PlayerModelState &state, const Vector3 &destination) {
    return flddRaycastScene(state, state.worldPosition, destination);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native route-NPC avoidance side: the signed auxiliary of an active NPC's
// arc-length avoidance result, or zero.
std::int32_t NpcAvoidanceSide(shen_name name) {
    const NpcRouteStateContext *const route = FindNpcRouteStateContext(name);
    if (!route || (route->flags & 3U) != 3U || !(route->avoidanceResult == 2U || static_cast<std::int8_t>(route->avoidanceResult) >= 5) ||
        route->field0a0 != 1U)
        return 0;
    return route->avoidanceAuxiliary;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymResolveCollisionSteering(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::uint32_t steerFlag = 4U;
    std::int16_t limitYaw, spread, opposite, turn, angle;
    turn = 0;
    state.previousCollisionTurnDelta = state.collisionTurnDelta;
    state.collisionTurnDelta = turn;
    state.collisionAvoidanceYaw = turn;
    if (!(state.limitingCollisionDistance < 5.0f) || (state.movementSelectionFlags & 0x20000U))
        return;
    angle = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(0.80000001f / state.limitingCollisionDistance));
    dc::sdk::CosFixed(angle);
    spread = angle;
    opposite = static_cast<std::int16_t>(-angle);
    limitYaw = static_cast<std::int16_t>(state.collisionLimitYawA - state.currentYaw);
    Vector3 negative, positive, boundary;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&state.worldTransform);
    Matrix4x4::RotateAroundY_Wrapper(angle);
    Matrix4x4::TransformPoint(plymRayForward, positive);
    matrix_stack::LoadMatrix(&state.worldTransform);
    Matrix4x4::RotateAroundY_Wrapper(-angle);
    Matrix4x4::TransformPoint(plymRayForward, negative);
    matrix_stack::LoadMatrix(&state.worldTransform);
    Matrix4x4::RotateAroundY_Wrapper(limitYaw);
    Matrix4x4::TransformPoint(plymRayForward, boundary);
    PopCurrentMatrix(1);
    float negativeDistance = 10.0f, positiveDistance = 10.0f, boundaryDistance = 10.0f;
    if (plymRaycastFromModel(state, negative))
        negativeDistance = state.collisionDistance;
    if (plymRaycastFromModel(state, positive))
        positiveDistance = state.collisionDistance;
    if (plymRaycastFromModel(state, boundary))
        boundaryDistance = state.collisionDistance;
    if (state.yawDelta) {
        std::int16_t target;
        turn = static_cast<std::int16_t>(state.currentYaw - state.collisionBoundaryYaw);
        if ((turn < 0 ? -turn : turn) < 2730) {
            turn = static_cast<std::int16_t>(state.desiredYaw - state.collisionBoundaryYaw);
            if (turn > 0)
                turn = 0x4000;
            else
                turn = -0x4000;
            target = static_cast<std::int16_t>(state.secondaryCollisionBoundaryYaw + turn);
        } else {
            turn = static_cast<std::int16_t>(state.desiredYaw - state.secondaryCollisionBoundaryYaw);
            if (turn > 0)
                turn = 0x4000;
            else
                turn = -0x4000;
            target = static_cast<std::int16_t>(state.secondaryCollisionBoundaryYaw + turn);
            if (plymProbeSpeedRatio(state.desiredYawProbeDistance, state.speedZ) < 0.2f) {
                turn = static_cast<std::int16_t>(state.currentYaw - state.secondaryCollisionBoundaryYaw);
                if (turn > 0)
                    turn = 0x4000;
                else
                    turn = -0x4000;
                target = static_cast<std::int16_t>(state.secondaryCollisionBoundaryYaw + turn);
            }
        }
        target = static_cast<std::int16_t>(target - state.currentYaw);
        state.collisionTurnDelta = target;
        state.movementSelectionFlags |= steerFlag;
        return;
    }
    if (state.limitingCollisionCharacter) {
        if (limitYaw > 0) {
            if (boundaryDistance > positiveDistance) {
                positiveDistance = boundaryDistance;
                spread = limitYaw;
            } else if (boundaryDistance > 3.0f) {
                positiveDistance = boundaryDistance;
                spread = limitYaw;
            }
        } else if (boundaryDistance > negativeDistance) {
            negativeDistance = boundaryDistance;
            opposite = limitYaw;
        } else if (boundaryDistance > 3.0f) {
            negativeDistance = boundaryDistance;
            opposite = limitYaw;
        }
    }
    float difference = positiveDistance - negativeDistance;
    if (difference < 0.0f)
        difference = -difference;
    if (state.limitingCollisionDistance > negativeDistance && state.limitingCollisionDistance > positiveDistance)
        return;
    state.movementSelectionFlags |= steerFlag;
    std::int32_t spacing;
    if (positiveDistance > 3.0f && negativeDistance > 3.0f) {
        spacing = difference > 4.0f ? 1 : 2;
    } else if (positiveDistance > 3.0f || negativeDistance > 3.0f) {
        spacing = difference > 1.0f ? 3 : 4;
    } else if (difference > 0.34999999f) {
        spacing = 5;
    } else {
        spacing = 6;
        if (positiveDistance < 0.60000002f && negativeDistance < 0.60000002f) {
            if (((state.limitingCollisionDistance - positiveDistance) < 0.0f ? -(state.limitingCollisionDistance - positiveDistance) :
                                                                               (state.limitingCollisionDistance - positiveDistance)) < 1.0f &&
                ((state.limitingCollisionDistance - negativeDistance) < 0.0f ? -(state.limitingCollisionDistance - negativeDistance) :
                                                                               (state.limitingCollisionDistance - negativeDistance)) < 1.0f)
                state.movementSelectionFlags |= 0x100000U;
        }
        if (state.limitingCollisionDistance < 1.5f &&
            (positiveDistance > negativeDistance ? positiveDistance : negativeDistance) + 0.050000001f > state.limitingCollisionDistance)
            state.movementSelectionFlags |= 0x100000U;
    }
    std::int32_t side;
    switch (spacing) {
    case 0:
    case 1:
    case 3:
    case 5:
    default:
        side = positiveDistance > negativeDistance ? -1 : 1;
        break;
    case 2:
    case 4:
    case 6:
        side = limitYaw > 0 ? -1 : 1;
        if (state.limitingCollisionCharacter) {
            const std::int32_t avoidance = -GetNpcRouteAvoidanceSide(ctrlGetScenarioCharaName(state.limitingCollisionCharacter));
            if (avoidance) {
                if (avoidance < 0)
                    side = 1;
                if (avoidance > 0)
                    side = -1;
            }
        }
        break;
    }
    if (state.movementSelectionFlags & 0x800U) {
        if (side == 1)
            ++state.collisionSideCounter;
        else
            state.collisionSideCounter = 0;
    } else if ((state.movementSelectionFlags & 0x400U) && side == -1) {
        ++state.collisionSideCounter;
    } else {
        state.collisionSideCounter = 0;
    }
    if (state.collisionSideCounter > 0) {
        if (state.collisionSideCounter > 1) {
            if (state.collisionSideCounter == 2)
                side = -side;
            if (state.collisionSideCounter > 5)
                state.collisionSideCounter = 0;
        }
    }
    turn = opposite;
    if (side < 0) {
        turn = spread;
        negativeDistance = positiveDistance;
    }
    angle = static_cast<std::int16_t>(turn < 0 ? -turn : turn);
    const float forward = negativeDistance * dc::sdk::CosFixed(angle) - state.limitingCollisionDistance;
    const float lateral = dc::sdk::SinFixed(angle) * negativeDistance;
    std::int16_t avoidanceYaw = 0;
    if (!(std::fabs(lateral) < 0.00000011920929f))
        avoidanceYaw = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(forward / lateral));
    state.collisionAvoidanceYaw = avoidanceYaw;
    if (state.forwardProbeDistance - state.limitingCollisionDistance > 1.5f && state.forwardProbeDistance > 3.0f)
        turn = static_cast<std::int16_t>((turn * 3) / 4);
    angle = static_cast<std::int16_t>(limitYaw + state.currentYaw);
    angle = static_cast<std::int16_t>(angle - state.currentYaw);
    if (angle * turn > 0 && (angle < 0 ? -angle : angle) < (turn < 0 ? -turn : turn)) {
        if (state.limitingCollisionCharacter) {
            turn = static_cast<std::int16_t>(turn - angle);
            turn = static_cast<std::int16_t>(turn / 2 + angle);
        } else {
            turn = angle;
        }
    }
    state.collisionTurnDelta = turn;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const float limit = state.limitingCollisionDistance;
    state.previousCollisionTurnDelta = state.collisionTurnDelta;
    state.collisionTurnDelta = 0;
    state.collisionAvoidanceYaw = 0;
    if (limit >= 5.0f || (state.movementSelectionFlags & selectionCharacterSpeedMatch))
        return;
    std::int16_t spread = std::fabs(limit) < 0.00000011920929f ? static_cast<std::int16_t>(0x8000) : AtanToFixedAngle(0.80000001f / limit);
    const auto limitYaw = static_cast<std::int16_t>(state.collisionLimitYawA - state.currentYaw);
    auto opposite = static_cast<std::int16_t>(-spread);
    const Vector3 positive = PlymRotatedWorldProbe(state, spread, plymRayForward);
    const Vector3 negative = PlymRotatedWorldProbe(state, static_cast<std::int16_t>(-spread), plymRayForward);
    const Vector3 limitProbe = PlymRotatedWorldProbe(state, limitYaw, plymRayForward);
    float negativeDistance = 10.0f;
    float positiveDistance = 10.0f;
    float limitDistance = 10.0f;
    if (flddRaycastScene(state, state.worldPosition, negative))
        negativeDistance = state.collisionDistance;
    if (flddRaycastScene(state, state.worldPosition, positive))
        positiveDistance = state.collisionDistance;
    if (flddRaycastScene(state, state.worldPosition, limitProbe))
        limitDistance = state.collisionDistance;
    if (state.yawDelta && (state.movementSelectionFlags & selectionImmediateCharacterCollision) == 0) {
        std::int16_t target{};
        if (Abs16(state.currentYaw - state.collisionBoundaryYaw) < 2730U) {
            target = static_cast<std::int16_t>(
                state.secondaryCollisionBoundaryYaw + (static_cast<std::int16_t>(state.desiredYaw - state.collisionBoundaryYaw) <= 0 ? -0x4000 : 0x4000));
        } else {
            const std::int16_t secondary = state.secondaryCollisionBoundaryYaw;
            target = static_cast<std::int16_t>(secondary + (static_cast<std::int16_t>(state.desiredYaw - secondary) <= 0 ? -0x4000 : 0x4000));
            if (plymProbeSpeedRatio(state.desiredYawProbeDistance, state.speed) < 0.2f)
                target = static_cast<std::int16_t>(secondary + (static_cast<std::int16_t>(state.currentYaw - secondary) <= 0 ? -0x4000 : 0x4000));
        }
        state.collisionTurnDelta = static_cast<std::int16_t>(target - state.currentYaw);
        state.movementSelectionFlags |= selectionCollisionTurnCandidate;
        return;
    }
    Character *const character = state.limitingCollisionCharacter;
    if (character) {
        if (limitYaw <= 0) {
            if (limitDistance > negativeDistance || limitDistance > 3.0f) {
                opposite = limitYaw;
                negativeDistance = limitDistance;
            }
        } else if (limitDistance > positiveDistance || limitDistance > 3.0f) {
            positiveDistance = limitDistance;
            spread = limitYaw;
        }
    }
    const float difference = std::fabs(positiveDistance - negativeDistance);
    if (limit > negativeDistance && limit > positiveDistance)
        return;
    state.movementSelectionFlags |= selectionCollisionTurnCandidate;
    bool symmetric{};
    if (positiveDistance <= 3.0f && negativeDistance <= 3.0f) {
        symmetric = difference <= 0.34999999f;
        if (symmetric) {
            if (positiveDistance < 0.60000002f && negativeDistance < 0.60000002f && std::fabs(limit - positiveDistance) < 1.0f &&
                std::fabs(limit - negativeDistance) < 1.0f) {
                state.movementSelectionFlags |= selectionCollisionTurnSuppressed;
            }
            if (limit < 1.5f && std::fmax(positiveDistance, negativeDistance) + 0.050000001f > limit)
                state.movementSelectionFlags |= selectionCollisionTurnSuppressed;
        }
    } else if (positiveDistance > 3.0f && negativeDistance > 3.0f) {
        symmetric = difference <= 4.0f;
    } else {
        symmetric = difference <= 1.0f;
    }
    std::int32_t side{};
    if (symmetric) {
        side = limitYaw <= 0 ? 1 : -1;
        if (character) {
            const std::int32_t avoidance = -NpcAvoidanceSide(ctrlGetScenarioCharaName(character));
            if (avoidance < 0)
                side = 1;
            if (avoidance > 0)
                side = -1;
        }
    } else {
        side = positiveDistance > negativeDistance ? -1 : 1;
    }
    if (((state.movementSelectionFlags & selectionCollisionTurnPositive) && side == 1) ||
        (!(state.movementSelectionFlags & selectionCollisionTurnPositive) && (state.movementSelectionFlags & selectionCollisionTurnNegative) && side == -1)) {
        ++state.collisionSideCounter;
    } else {
        state.collisionSideCounter = 0;
    }
    if (state.collisionSideCounter >= 2) {
        if (state.collisionSideCounter == 2)
            side = -side;
        if (state.collisionSideCounter > 5)
            state.collisionSideCounter = 0;
    }
    if (side >= 0) {
        spread = opposite;
        positiveDistance = negativeDistance;
    }
    const std::uint16_t magnitude = Abs16(spread);
    const float forward = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(magnitude))) * positiveDistance - limit;
    const float lateral = sin_fixed16(static_cast<std::int16_t>(magnitude)) * positiveDistance;
    if (std::fabs(lateral) >= 0.00000011920929f)
        state.collisionAvoidanceYaw = AtanToFixedAngle(forward / lateral);
    if (limitYaw * spread > 0 && Abs16(limitYaw) < magnitude)
        spread = character ? static_cast<std::int16_t>(limitYaw + static_cast<std::int16_t>(spread - limitYaw) / 2) : limitYaw;
    state.collisionTurnDelta = spread;
#endif
}


// @verified - d
void plymUpdateCollisionTurnActivation(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const std::uint32_t candidate = state.movementSelectionFlags & 4U;
    std::int32_t activate = candidate ? 1 : 0;
    const std::int16_t boundaryDelta = static_cast<std::int16_t>(state.collisionBoundaryYaw - state.currentYaw);
    const std::int32_t boundaryDistance = boundaryDelta < 0 ? -boundaryDelta : boundaryDelta;
    if (boundaryDistance < 2730) {
        if (state.limitingCollisionCharacter) {
            if (std::fabs(state.movementInterpolation) < 0.00000011920929f)
                activate = false;
        } else {
            const std::int16_t yawDelta = state.yawDelta;
            const std::int32_t yawDistance = yawDelta < 0 ? -yawDelta : yawDelta;
            if (yawDistance <= 1024 && (state.movementSelectionFlags & 4U)) {
                const std::int16_t avoidanceDelta = state.collisionAvoidanceYaw;
                const std::int32_t avoidanceDistance = avoidanceDelta < 0 ? -avoidanceDelta : avoidanceDelta;
                if (avoidanceDistance < 2730)
                    activate = false;
            }
        }
    }
    const std::int16_t collisionDelta = static_cast<std::int16_t>(state.collisionBoundaryYaw - state.currentYaw);
    const std::int32_t collisionDistance = collisionDelta < 0 ? -collisionDelta : collisionDelta;
    if (collisionDistance < 13653 && state.collisionData)
        activate = false;
    if (state.currentMovementMode == 2 || state.currentMovementMode == 6 || state.motionState == 9 || state.motionState == 4) {
        state.movementSelectionFlags &= ~8U;
        state.inactiveDirection184 = 0;
        return;
    }
    const std::int8_t packedDirection = static_cast<std::int8_t>(state.packedDirection);
    const std::int32_t packedMagnitude = packedDirection < 0 ? -packedDirection : packedDirection;
    if (packedMagnitude > 20)
        state.inactiveDirection184 = 0;
    const float speedDelta = state.speedZ - 1.0f;
    float reach = 1.0f;
    reach += speedDelta * 2.25f;
    if (IsHeldTransitionReady(state))
        reach = 1.0f;
    if (state.limitingCollisionDistance > reach)
        activate = false;
    if (activate) {
        state.movementSelectionFlags |= 8U;
        state.inactiveDirection184 = 4;
    } else if (state.inactiveDirection184 > 0) {
        state.movementSelectionFlags |= 8U;
    }
    if (state.movementSelectionFlags & 0x2000U)
        state.movementSelectionFlags &= ~8U;
    if (!(state.previousMovementSelectionFlags & 8U) && (state.movementSelectionFlags & 8U)) {
        state.movementSelectionFlags |= 0x40000U;
        state.inactiveDirection17e = 0;
        if (!(state.movementSelectionFlags & 0x1000U))
            state.collisionLatchYaw = state.currentYaw;
    }
    if (state.movementSelectionFlags & 8U) {
        state.movementSelectionFlags &= ~0xc00U;
        if (state.collisionTurnDelta < 0)
            state.movementSelectionFlags |= 0x400U;
        else
            state.movementSelectionFlags |= 0x800U;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const std::uint32_t candidate = state.movementSelectionFlags & selectionCollisionTurnCandidate;
    bool activate = candidate != 0;
    if (Abs16(state.collisionBoundaryYaw - state.currentYaw) < 2730U) {
        if (state.limitingCollisionCharacter) {
            if (std::fabs(state.normalizedMotionInput) < 0.00000011920929f)
                activate = false;
        } else if (Abs16(state.yawDelta) <= 1024U && candidate && Abs16(state.collisionAvoidanceYaw) < 2730U) {
            activate = false;
        }
    }
    const std::int16_t charaYaw = state.currentYaw;
    if (Abs16(state.collisionBoundaryYaw - charaYaw) < 13653U && state.collisionData)
        activate = false;
    if (((state.movementMode - 2U) & 0xfffffffbU) == 0 || state.rootYawState == 9U || state.rootYawState == 4U) {
        state.movementSelectionFlags &= ~selectionCollisionTurnActive;
        state.inactiveDirection188 = 0;
        return;
    }
    const auto packedX = static_cast<std::int8_t>(state.packedDirection & 0xffU);
    if (std::abs(static_cast<std::int32_t>(packedX)) > 20)
        state.inactiveDirection188 = 0;
    const float reach = state.rootYawState ? (state.speed - 1.0f) * 2.25f + 1.0f : 1.0f;
    if (state.limitingCollisionDistance <= reach && activate) {
        state.inactiveDirection188 = 4;
        state.movementSelectionFlags |= selectionCollisionTurnActive;
    } else if (state.inactiveDirection188 > 0) {
        state.movementSelectionFlags |= selectionCollisionTurnActive;
    }
    if (state.movementSelectionFlags & selectionGroundBoundaryTurnActive)
        state.movementSelectionFlags &= ~selectionCollisionTurnActive;
    if ((state.previousMovementSelectionFlags & selectionCollisionTurnActive) == 0 && (state.movementSelectionFlags & selectionCollisionTurnActive)) {
        state.movementSelectionFlags |= selectionCollisionTurnLatched;
        state.inactiveDirection182 = 0;
        if ((state.movementSelectionFlags & selectionGroundBoundaryTurnCandidate) == 0)
            state.collisionLatchYaw = charaYaw;
    }
    if (state.movementSelectionFlags & selectionCollisionTurnActive) {
        state.movementSelectionFlags &= ~(selectionCollisionTurnNegative | selectionCollisionTurnPositive);
        state.movementSelectionFlags |= state.collisionTurnDelta >= 0 ? selectionCollisionTurnPositive : selectionCollisionTurnNegative;
    }
#endif
}


// @verified - d
void plymUpdateCollisionTurnLatch(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int16_t offset;
    const Vector3 *endProbe, *startProbe;
    Vector3 end, start;
    float distance;
    state.turnYawDelta = 0;
    if (state.movementSelectionFlags & 0x40000U) {
        if (!(state.movementSelectionFlags & 0x1000U)) {
            ++state.inactiveDirection17e;
            if (state.inactiveDirection17e > 90)
                state.movementSelectionFlags &= ~0x40000U;
        }
        if (state.yawDelta)
            state.movementSelectionFlags &= ~0x40000U;
        if (state.movementSelectionFlags & 0x8000U)
            state.movementSelectionFlags &= ~0x40000U;
        offset = static_cast<std::int16_t>(state.collisionLatchYaw - state.currentYaw);
        if ((offset < 0 ? -offset : offset) > 0x4000) {
            offset = (offset < -0x4000 ? -0x4000 : offset) < 0x4000 ? (offset < -0x4000 ? -0x4000 : offset) : 0x4000;
            state.collisionLatchYaw = static_cast<std::int16_t>(state.currentYaw + offset);
        }
    }
    offset = static_cast<std::int16_t>(state.collisionLatchYaw - state.currentYaw);
    state.movementSelectionFlags &= ~0x1000U;
    if (state.movementSelectionFlags & 8U)
        return;
    if (!(state.movementSelectionFlags & 0x40000U))
        return;
    if (offset > 0) {
        endProbe = &plymRayLeftEnd;
        startProbe = &plymRayLeftStart;
    } else {
        endProbe = &plymRayRightEnd;
        startProbe = &plymRayRightStart;
    }
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&state.worldPosition);
    Matrix4x4::RotateAroundY_Wrapper(state.collisionLatchYaw);
    Matrix4x4::TransformPoint(*endProbe, end);
    Matrix4x4::TransformPoint(*startProbe, start);
    PopCurrentMatrix(1);
    distance = 10.0f;
    if (flddRaycastScene(state, start, end))
        distance = state.collisionDistance;
    if (distance < 2.0f)
        return;
    if ((offset < 0 ? -offset : offset) <= 0x221) {
        state.movementSelectionFlags &= ~0x41000U;
    } else {
        state.movementSelectionFlags |= 0x1000U;
        state.turnYawDelta = offset;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.turnYawDelta = 0;
    if (state.movementSelectionFlags & selectionCollisionTurnLatched) {
        if ((state.movementSelectionFlags & selectionGroundBoundaryTurnCandidate) == 0 && ++state.inactiveDirection182 > 90)
            state.movementSelectionFlags &= ~selectionCollisionTurnLatched;
        if (state.yawDelta)
            state.movementSelectionFlags &= ~selectionCollisionTurnLatched;
        if (state.movementSelectionFlags & selectionSuppressCollisionSteering)
            state.movementSelectionFlags &= ~selectionCollisionTurnLatched;
        auto offset = static_cast<std::int16_t>(state.collisionLatchYaw - state.currentYaw);
        if (Abs16(offset) > 0x4000U) {
            offset = static_cast<std::int16_t>(std::clamp<std::int32_t>(offset, -0x4000, 0x4000));
            state.collisionLatchYaw = static_cast<std::int16_t>(offset + state.currentYaw);
        }
    }
    state.movementSelectionFlags &= ~selectionGroundBoundaryTurnCandidate;
    const auto latchDelta = static_cast<std::int16_t>(state.collisionLatchYaw - state.currentYaw);
    if ((state.movementSelectionFlags & selectionCollisionTurnActive) || (state.movementSelectionFlags & selectionCollisionTurnLatched) == 0)
        return;
    Vector3 end{};
    Vector3 start{};
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(state.worldPosition.x, state.worldPosition.y, state.worldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.collisionLatchYaw));
    Matrix4x4::TransformPoint(latchDelta <= 0 ? plymRayRightEnd : plymRayLeftEnd, end);
    Matrix4x4::TransformPoint(latchDelta <= 0 ? plymRayRightStart : plymRayLeftStart, start);
    Matrix4x4::Pop(1);
    if (flddRaycastScene(state, start, end) && state.collisionDistance < 2.0f)
        return;
    if (Abs16(latchDelta) >= 546U) {
        state.movementSelectionFlags |= selectionGroundBoundaryTurnCandidate;
        state.turnYawDelta = latchDelta;
    } else {
        state.movementSelectionFlags &= ~(selectionGroundBoundaryTurnCandidate | selectionCollisionTurnLatched);
    }
#endif
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native movement-context yaw constraint (PLYM pad flag 0x400000): stair and
// target-world contexts clamp or snap the desired yaw to the context heading.
bool plymConstrainMovementContextYaw(PlayerModelState &state) {
    const PlayerMovementState &movement = *state.movementState;
    if ((state.padFlags & 0x400000U) == 0)
        return false;
    const std::int16_t targetYaw = movement.targetWorldYaw;
    if (state.rootYawState == 0U || state.rootYawState == 9U) {
        std::int16_t charaYaw = state.currentYaw;
        if (state.rootYawState == 9U)
            charaYaw = static_cast<std::int16_t>(charaYaw + movement.rootYawOffset);
        if (static_cast<std::uint32_t>(Abs16(charaYaw - targetYaw)) - 8193U > 0x3ffeU) {
            if (!state.yawDelta)
                return true;
            const auto step = static_cast<std::int16_t>(std::clamp<std::int32_t>(static_cast<std::int16_t>(state.desiredYaw - charaYaw), -4095, 4095));
            if (static_cast<std::uint32_t>(Abs16(state.desiredYaw - targetYaw)) - 8193U > 0x3ffeU) {
                state.desiredYaw = static_cast<std::int16_t>(charaYaw + step);
            } else {
                state.activeMovementMode = 2;
                state.padFlags &= 0xf7fffdffU;
            }
            return true;
        }
        state.desiredYaw = Abs16(state.desiredYaw - targetYaw) >= 0x4000U ? static_cast<std::int16_t>(targetYaw + 0x8000) : targetYaw;
        state.padFlags &= 0xf7fffdffU;
        state.activeMovementMode = 2;
        return true;
    }
    auto offset = static_cast<std::int16_t>(state.desiredYaw - targetYaw);
    const std::uint16_t facing = Abs16(state.currentYaw - targetYaw);
    std::int16_t yaw = targetYaw;
    if (facing < 0x4000U) {
        if (facing <= 6144U) {
            if (offset >= 6144) {
                state.desiredYaw = static_cast<std::int16_t>(targetYaw + 6144);
                return true;
            }
            yaw = static_cast<std::int16_t>(yaw + std::max<std::int16_t>(offset, -6144));
        }
        state.desiredYaw = yaw;
        return true;
    }
    if (facing >= 26624U) {
        offset = static_cast<std::int16_t>(offset + 0x8000);
        if (offset >= 6144) {
            state.desiredYaw = static_cast<std::int16_t>(targetYaw - 26624);
            return true;
        }
        yaw = static_cast<std::int16_t>(yaw + std::max<std::int16_t>(offset, -6144));
    }
    state.desiredYaw = static_cast<std::int16_t>(yaw + 0x8000);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymResolveCollisionTurn(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int32_t kind;
    reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController)->motionScale = (kind = 0, state.turnSpeedScale = 1.0f);
    state.pendingTurnDelta = 0;
    state.previousMovementSelectionFlags = state.movementSelectionFlags;
    if ((state.movementSelectionFlags & 1U) == 0)
        state.movementSelectionFlags &= ~0xc00U;
    if ((state.movementSelectionFlags & 1U) == 0) {
        state.movementSelectionFlags &= ~0x3000000U;
        state.collisionSideCounter = 0;
    }
    state.movementSelectionFlags &= ~0x8001U;
    state.movementSelectionFlags &= ~0x10000cU;
    state.movementSelectionFlags &= ~0x20000U;
    if (state.movementSelectionFlags & 0x40000000U) {
        state.movementSelectionFlags &= ~0x40000000U;
        return;
    }
    if (state.movementInterpolation < 0.1f || state.interpolationValue < 0.1f)
        return;
    if (state.flags & 0x20000000U)
        return;
    if (!(state.flags & 0x10000000U))
        return;
    if (state.currentMovementMode == 2 || state.currentMovementMode == 6 || state.motionState == 9 || state.motionState == 4)
        state.movementSelectionFlags |= 0x8000U;
    plymUpdateGroundBoundaryTurn(state);
    plymProbeSideWalls(state);
    plymMatchCharacterSpeed(state);
    plymResolveCollisionSteering(state);
    if (state.inactiveDirection184 > 0)
        --state.inactiveDirection184;
    if (state.activeMovementMode == 2 || state.activeMovementMode == 6) {
        state.movementSelectionFlags &= ~0x41000U;
        state.movementSelectionFlags &= ~8U;
        state.inactiveDirection164 = 0;
        state.inactiveDirection184 = 0;
    }
    if (state.activeMovementMode == 2 || state.activeMovementMode == 6)
        return;
    plymUpdateCollisionTurnActivation(state);
    plymUpdateCollisionTurnLatch(state);
    if (state.movementSelectionFlags & 0x2000U) {
        state.pendingTurnDelta = state.groundBoundaryTurnYawDelta;
        kind = 0x2000;
    } else if (state.movementSelectionFlags & 8U) {
        kind = 8;
        state.pendingTurnDelta = state.collisionTurnDelta;
    } else if (state.movementSelectionFlags & 0x1000U) {
        state.pendingTurnDelta = state.turnYawDelta;
        kind = 0x1000;
    } else if (state.movementSelectionFlags & 0x10U) {
        state.pendingTurnDelta = state.requestedTurnYaw - state.currentYaw;
        kind = 0x10;
    }
    if (state.pendingTurnDelta) {
        state.movementSelectionFlags |= 1U;
        if (!state.yawDelta) {
            state.resolvedTurnYaw = state.currentYaw + state.pendingTurnDelta;
        } else if (state.yawDelta * state.pendingTurnDelta >= 0) {
            if ((state.pendingTurnDelta < 0 ? -state.pendingTurnDelta : state.pendingTurnDelta) > (state.yawDelta < 0 ? -state.yawDelta : state.yawDelta))
                state.resolvedTurnYaw = state.currentYaw + state.pendingTurnDelta;
            else
                state.movementSelectionFlags &= ~1U;
        } else {
            state.movementSelectionFlags &= ~1U;
            if (state.motionState == 2 || state.motionState == 6) {
                const float ratio = plymProbeSpeedRatio(state.desiredYawProbeDistance, state.speedZ);
                if ((state.movementSelectionFlags & 0x10U) && ratio < 0.2f) {
                    state.resolvedTurnYaw = state.currentYaw + state.pendingTurnDelta;
                    state.movementSelectionFlags |= 1U;
                }
            }
        }
        if (state.movementSelectionFlags & 0x2000U) {
            state.resolvedTurnYaw = state.currentYaw + state.pendingTurnDelta;
            state.movementSelectionFlags |= 1U;
        }
    }
    if ((state.movementSelectionFlags & 1U) == 0)
        return;
    const std::int16_t direction = state.resolvedTurnYaw - state.currentYaw;
    if ((state.previousMovementSelectionFlags & 1U) == 0) {
        state.movementSelectionFlags &= ~0x3000000U;
        if (direction < 0)
            state.movementSelectionFlags |= 0x2000000U;
        else
            state.movementSelectionFlags |= 0x1000000U;
    } else if (state.movementSelectionFlags & 0x1000000U) {
        if (direction < 0)
            state.movementSelectionFlags = (state.movementSelectionFlags & ~0x1000000U) | 0x6000000U;
    } else if ((state.movementSelectionFlags & 0x2000000U) && direction > 0) {
        state.movementSelectionFlags = (state.movementSelectionFlags & ~0x2000000U) | 0x5000000U;
    }
    state.desiredYaw = state.resolvedTurnYaw;
    plymProbeDesiredYawCollision(state);
    UpdatePlayerDeltaSpeedRatio(state);
    PlayerMovementState *const movement = reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController);
    switch (kind) {
    case 8:
    case 0x2000: {
        float rate = (state.limitingCollisionDistance - 0.80000001f) / 0.69999999f;
        const float clearance = 1.0f - ((rate < 0.0f ? 0.0f : rate) < 1.0f ? (rate < 0.0f ? 0.0f : rate) : 1.0f);
        rate = clearance + clearance + 1.5f;
        movement->motionScale = state.turnSpeedScale;
        if (rate > movement->motionScale)
            movement->motionScale = rate;
        break;
    }
    case 0x1000:
        movement->motionScale = 1.5f;
        break;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.turnSpeedScale = 1.0f;
    state.embeddedMovementState.yawInterpolationRate = 1.0f;
    std::int32_t turnKind = 0;
    state.pendingTurnDelta = 0;
    state.previousMovementSelectionFlags = state.movementSelectionFlags;
    state.movementSubtype = 0;
    if ((state.movementSelectionFlags & selectionTurnActive) == 0)
        state.movementSelectionFlags &= ~(selectionCollisionTurnNegative | selectionCollisionTurnPositive);
    if ((state.movementSelectionFlags & selectionTurnActive) == 0) {
        state.collisionSideCounter = 0;
        state.movementSelectionFlags &= ~(selectionTurnPositive | selectionTurnNegative);
    }
    state.movementSelectionFlags &= ~(selectionTurnActive | selectionCollisionTurnCandidate | selectionCollisionTurnActive |
        selectionSuppressCollisionSteering | selectionCharacterSpeedMatch | selectionCollisionTurnSuppressed);
    if (state.activeMovementMode == 6U)
        plymProbeSideWalls(state);
    if (state.movementSelectionFlags & selectionForcedYaw) {
        state.movementSelectionFlags &= ~selectionForcedYaw;
        return;
    }
    if ((state.flags & 0x20000000U) != 0 || (state.flags & 0x10000000U) == 0)
        return;
    if (((state.movementMode - 2U) & 0xfffffffbU) == 0 || state.rootYawState == 9U || state.rootYawState == 4U)
        state.movementSelectionFlags |= selectionSuppressCollisionSteering;
    plymUpdateGroundBoundaryTurn(state);
    const bool nearCharacter = (state.embeddedMovementState.flags & 8U) != 0 && state.nearestCollisionCharacter && state.nearestCharaProbeDistance < 1.25f;
    if (!nearCharacter && state.normalizedMotionInput < 0.1f)
        return;
    plymProbeSideWalls(state);
    plymMatchCharacterSpeed(state);
    plymResolveCollisionSteering(state);
    if (state.inactiveDirection188 > 0)
        --state.inactiveDirection188;
    if (((state.activeMovementMode - 2U) & 0xfffffffbU) == 0) {
        state.movementSelectionFlags &= ~(selectionCollisionTurnActive | selectionGroundBoundaryTurnCandidate | selectionCollisionTurnLatched);
        state.inactiveDirection168 = 0;
        state.inactiveDirection188 = 0;
        return;
    }
    plymUpdateCollisionTurnActivation(state);
    plymUpdateCollisionTurnLatch(state);
    const std::uint32_t selection = state.movementSelectionFlags;
    if (selection & selectionGroundBoundaryTurnActive) {
        state.pendingTurnDelta = state.groundBoundaryTurnYawDelta;
        turnKind = selectionGroundBoundaryTurnActive;
    } else if (selection & selectionCollisionTurnActive) {
        turnKind = selectionCollisionTurnActive;
        state.pendingTurnDelta = state.collisionTurnDelta;
        if ((selection & selectionCollisionTurnSuppressed) || ((state.flags & plymControlLockOnMovement) && state.limitingCollisionCharacter)) {
            state.pendingTurnDelta = 0;
            turnKind = 0;
        }
    } else if (selection & selectionGroundBoundaryTurnCandidate) {
        state.pendingTurnDelta = state.turnYawDelta;
        turnKind = selectionGroundBoundaryTurnCandidate;
    } else if (selection & selectionObstacleTurnCandidate) {
        turnKind = selectionObstacleTurnCandidate;
        state.pendingTurnDelta = static_cast<std::int16_t>(state.requestedTurnYaw - state.currentYaw);
    }
    const std::int16_t pending = state.pendingTurnDelta;
    if (pending) {
        const std::int16_t yawDelta = state.yawDelta;
        state.movementSelectionFlags |= selectionTurnActive;
        bool resolve = true;
        if (yawDelta && pending * yawDelta < 0) {
            state.movementSelectionFlags &= ~selectionTurnActive;
            resolve = ((state.rootYawState - 2U) & 0xfffffffbU) == 0 && (state.movementSelectionFlags & selectionObstacleTurnCandidate) &&
                plymProbeSpeedRatio(state.desiredYawProbeDistance, state.speed) < 0.05;
            if (resolve)
                state.movementSelectionFlags |= selectionTurnActive;
        } else if (yawDelta && std::abs(static_cast<std::int32_t>(pending)) <= std::abs(static_cast<std::int32_t>(yawDelta))) {
            state.movementSelectionFlags &= ~selectionTurnActive;
            resolve = false;
        }
        if (resolve)
            state.resolvedTurnYaw = static_cast<std::int16_t>(pending + state.currentYaw);
        if (state.movementSelectionFlags & selectionGroundBoundaryTurnActive) {
            state.resolvedTurnYaw = static_cast<std::int16_t>(pending + state.currentYaw);
            state.movementSelectionFlags |= selectionTurnActive;
        }
        if (state.movementSelectionFlags & selectionImmediateCharacterCollision) {
            state.resolvedTurnYaw = static_cast<std::int16_t>(pending + state.currentYaw);
            state.movementSelectionFlags |= selectionTurnActive;
        }
    }
    if ((state.movementSelectionFlags & selectionTurnActive) == 0)
        return;
    const std::int16_t resolved = state.resolvedTurnYaw;
    const auto direction = static_cast<std::int16_t>(resolved - state.currentYaw);
    std::uint32_t &flags = state.movementSelectionFlags;
    constexpr std::uint32_t directionMask = selectionTurnPositive | selectionTurnNegative | selectionTurnDirectionReversed;
    if ((state.previousMovementSelectionFlags & selectionTurnActive) == 0) {
        flags &= ~(selectionTurnPositive | selectionTurnNegative);
        flags |= direction >= 0 ? selectionTurnPositive : selectionTurnNegative;
    } else if (flags & selectionTurnPositive) {
        if (direction < 0)
            flags = (flags & ~directionMask) | selectionTurnNegative | selectionTurnDirectionReversed;
    } else if ((flags & selectionTurnNegative) && direction > 0) {
        flags = (flags & ~directionMask) | selectionTurnPositive | selectionTurnDirectionReversed;
    }
    state.desiredYaw = resolved;
    plymProbeDesiredYawCollision(state);
    UpdatePlayerDeltaSpeedRatio(state);
    if (turnKind == static_cast<std::int32_t>(selectionCollisionTurnActive) || turnKind == static_cast<std::int32_t>(selectionGroundBoundaryTurnActive)) {
        const float clearance = std::fmin(1.0f, std::fmax(0.0f, (state.limitingCollisionDistance - 0.80000001f) / 0.69999999f));
        const float rate = (1.0f - clearance) + (1.0f - clearance) + 1.5f;
        state.embeddedMovementState.yawInterpolationRate = rate > state.turnSpeedScale ? rate : state.turnSpeedScale;
    } else if (turnKind == static_cast<std::int32_t>(selectionGroundBoundaryTurnCandidate)) {
        state.embeddedMovementState.yawInterpolationRate = 1.5f;
    }
    plymConstrainMovementContextYaw(state);
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
float SimdSolveVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &direction, const Vector3 &reference, const Vector3 &crossVector) {
    output.x = base.x;
    output.y = base.y;
    output.z = base.z;
    if (std::fabs(direction.x * crossVector.z - crossVector.x * direction.z) < 0.00000011920929f)
        return 0.0f;
    float rows[2][3], value;
    std::int32_t pivot, row, column;
    rows[0][0] = direction.x;
    rows[0][1] = -crossVector.x;
    rows[0][2] = -base.x + reference.x;
    rows[1][0] = direction.z;
    rows[1][1] = -crossVector.z;
    rows[1][2] = -base.z + reference.z;
    for (pivot = 0; pivot < 2; ++pivot) {
        value = rows[pivot][pivot];
        for (column = pivot; column < 3; ++column)
            rows[pivot][column] /= value;
        for (row = 0; row < 2; ++row) {
            if (row == pivot)
                continue;
            value = rows[row][pivot];
            for (column = pivot; column < 3; ++column)
                rows[row][column] -= value * rows[pivot][column];
        }
    }
    value = rows[0][2];
    output.x = base.x + value * direction.x;
    output.y = base.y + value * direction.y;
    output.z = base.z + value * direction.z;
    return value;
}


// @verified - d
void ComputeVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &target, const Vector3 &reference, const Vector3 &referenceEnd) {
    Vector3 direction, crossVector;
    direction.x = target.x - base.x;
    direction.y = target.y - base.y;
    direction.z = target.z - base.z;
    crossVector.x = referenceEnd.x - reference.x;
    crossVector.y = referenceEnd.y - reference.y;
    crossVector.z = referenceEnd.z - reference.z;
    SimdSolveVectorProjection(output, base, direction, reference, crossVector);
}


void ProbeReachedOrdinaryMovementCollision(PlayerMovementState &movement) {
    Vector3 location, nearPoint, farPoint, intersection, stairAnchor, wallEnd, local, wallPoint;
    std::int32_t stairYaw, steps;
    movement.flags &= ~0x80U;
    movement.pendingPositionCorrection = 0.0f;
    movement.correctionCycleDistance = 0.0f;
    movement.correctionCycleCount = 0;
    GetCharacterWorldPosition(movement.character, &location);
    nearPoint.x = location.x;
    nearPoint.y = location.y + 0.22499999f;
    nearPoint.z = location.z;
    OffsetPointAlongYaw(&farPoint, &nearPoint, 10.0f, movement.currentYaw);
    flddPushIntersectionFlag(0x20);
    std::int32_t found = FindNearestRayIntersection(&intersection, &farPoint, &nearPoint);
    if (!found) {
        nearPoint.y = location.y - 0.22499999f;
        flddPushIntersectionFlag(0x20);
        found = FindNearestRayIntersection(&intersection, &nearPoint, &farPoint);
    }
    if (!found || !flddHasFoundIntersectionOfType(0x20)) {
        movement.pendingPositionCorrection = 0.0f;
        return;
    }
    steps = flddQueryStairOrigin(&intersection, &stairAnchor, &stairYaw);
    std::int32_t selectedStep = 0;
    if (location.y - stairAnchor.y > static_cast<float>(steps) * 0.22499999f * 0.5f)
        selectedStep = steps;
    wallPoint.x = 0.0f;
    wallPoint.y = static_cast<float>(selectedStep) * 0.22499999f;
    if (!selectedStep)
        wallPoint.z = 0.15000001f;
    else
        wallPoint.z = -(static_cast<float>(selectedStep - 1) * 0.30000001f);
    wallEnd.x = 1.0f;
    wallEnd.y = 0.0f;
    wallEnd.z = 0.0f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&stairAnchor);
    Matrix4x4::RotateAroundY_Wrapper(stairYaw);
    matrix_stack::TranslateVector(&wallPoint);
    matrix_stack::GetTranslation(&wallPoint);
    Matrix4x4::TransformPoint(wallEnd, wallEnd);
    PopCurrentMatrix(1);
    ComputeVectorProjection(intersection, nearPoint, intersection, wallPoint, wallEnd);
    const float dx = nearPoint.x - intersection.x, dz = nearPoint.z - intersection.z;
    movement.obstacleDistance = SquareRoot(dx * dx + dz * dz);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement.currentYaw);
    matrix_stack::TranslateInverseVector(&nearPoint);
    Matrix4x4::TransformPoint(intersection, local);
    PopCurrentMatrix(1);
    if (local.z > 0.0f)
        movement.obstacleDistance = 0.0f;
    const std::int32_t animation = mtwkGetAnimationId(movement.motion, static_cast<MotionRegion>(0));
    if (animation != 0xf005 && animation != 0xf03d && animation != 0xf03c && animation != 0xf03b && animation != 0xf03a && animation != 0xf039 &&
        animation != 0xf038 && animation != 0xf037 && animation != 0xf036 && animation != 0xf035 && animation != 0xf006)
        return;
    if ((movement.flags & 8U) && movement.targetWorldYaw == stairYaw && movement.targetHeightScale == steps)
        return;
    const float scale = movement.secondScale * movement.firstScale;
    float trackDistance, cycleDistance, frame, halfCycle, perFrame, phase, travelled;
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(movement.motion), -1.0f, &nearPoint);
    trackDistance = nearPoint.z;
    if (trackDistance < 0.0f)
        trackDistance = -trackDistance;
    cycleDistance = trackDistance * 0.5f;
    frame = mtwkGetFrame(movement.motion, static_cast<MotionRegion>(0));
    halfCycle = static_cast<float>(GetMotionFrameCount(animation)) - 1.0f;
    perFrame = trackDistance / halfCycle * scale;
    halfCycle *= 0.5f;
    phase = halfCycle - frame;
    if (phase < 0.0f)
        phase += halfCycle;
    phase /= scale;
    travelled = phase * perFrame;
    if (movement.obstacleDistance <= travelled) {
        movement.pendingPositionCorrection = -(travelled - movement.obstacleDistance);
        movement.correctionCycleDistance = phase;
        movement.correctionCycleCount = 0;
        movement.flags |= 0x80U;
    } else {
        float remainder = movement.obstacleDistance;
        remainder -= travelled;
        std::int32_t count = static_cast<std::int32_t>(remainder / cycleDistance);
        remainder -= static_cast<float>(count) * cycleDistance;
        if (remainder > 0.5f * cycleDistance) {
            remainder -= cycleDistance;
            ++count;
        }
        movement.pendingPositionCorrection = remainder;
        movement.correctionCycleDistance = static_cast<float>(count) * halfCycle + phase;
        movement.correctionCycleCount = static_cast<std::int16_t>(count);
    }
    if ((movement.flags & 0x100000U) && movement.obstacleDistance < 0.25f)
        movement.pendingPositionCorrection = movement.obstacleDistance;
}


#define PLAYER_STAIR_ABS(x) ((x) < 0 ? -(x) : (x))
#define PLAYER_STAIR_MIN(a, b) ((a) < (b) ? (a) : (b))

void UpdateReachedFloorStairCollision(PlayerMovementState &movement) {
    Vector3 location, local, anchor, probe, rightTarget, leftTarget, rightBone, leftBone, rightMid, leftMid, upperProbe, upperAnchor, boundary;
    std::int32_t steps, upperSteps, yawDistance, direction, upperStep, rightStep, leftStep, footStep, rightTargetLevel, leftTargetLevel;
    std::int32_t step, rightHeight, leftHeight, lowest, height, yaw, upperYaw, connectionId;
    Character *const character = movement.character;
    std::int32_t publishTarget = 0, retainTarget = 0;
    ProbeReachedOrdinaryMovementCollision(movement);
    MotionWork *const work = movement.motion;
    mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    GetCharacterWorldPosition(movement.character, &location);
    location.y += 1.0f;
    movement.flags &= ~0x10000U;
    movement.sceneGroundIndex = scenFindIntersection(&location);
    if (movement.sceneGroundIndex != -1)
        movement.flags |= 0x10000U;
    GetCharacterWorldPosition(movement.character, &location);
    probe = location;
    probe.y += 1.5f;
    probe.y = FindFieldIntersectionHeight(&probe);
    steps = flddQueryStairOrigin(&probe, &anchor, &yaw);
    upperProbe = probe;
    upperProbe.y += 1.5f;
    upperSteps = flddQueryStairConnection(&upperProbe, &upperAnchor, &upperYaw, &connectionId);
    if (upperSteps > 0 && PLAYER_STAIR_ABS(anchor.y - upperAnchor.y) >= 0.22499999f) {
        steps = upperSteps;
        anchor = upperAnchor;
        yaw = upperYaw;
    }
    if (steps > 0) {
        std::uint32_t control = ccmGetFlags(character);
        if (!(movement.flags & 8U)) {
            movement.targetWorldPosition.x = anchor.x;
            movement.targetWorldPosition.y = anchor.y;
            movement.targetWorldPosition.z = anchor.z;
            movement.targetWorldYaw = yaw;
            movement.targetHeightScale = steps;
            movement.cachedCharacterControlFlags = control;
            movement.groundedStepIndex = 0;
            publishTarget = 1;
        } else if (PLAYER_STAIR_ABS(movement.targetWorldPosition.y - anchor.y) >= 0.22499999f) {
            if (movement.flags & 0x40000U) {
                movement.targetWorldPosition.x = anchor.x;
                movement.targetWorldPosition.y = anchor.y;
                movement.targetWorldPosition.z = anchor.z;
                movement.targetWorldYaw = yaw;
                movement.targetHeightScale = steps;
                publishTarget = 1;
            } else {
                anchor.x = movement.targetWorldPosition.x;
                anchor.y = movement.targetWorldPosition.y;
                anchor.z = movement.targetWorldPosition.z;
                yaw = movement.targetWorldYaw;
                steps = movement.targetHeightScale;
                retainTarget = 1;
            }
        }
        movement.flags = (movement.flags | 8U) & ~0x40000U;
        if (retainTarget)
            movement.flags |= 0x40000U;
        control &= ~0x42U;
        control |= 0x100U;
        control &= ~0x800U;
        if (!movement.rootYawMode && !(movement.flags & 0x100U))
            control |= 0x42U;
        else if (movement.playerMotionFlags & 2U)
            control |= 0x800U;
        AssignCharacterControlFlags(character, control);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-yaw);
        Matrix4x4::preTranslate(-anchor.x, -anchor.y, -anchor.z);
        Matrix4x4::TransformPoint(location, local);
        PopCurrentMatrix(1);
        if (local.y < -0.01f) {
            control |= 0x40U;
            AssignCharacterControlFlags(character, control);
        }
    } else {
        if (movement.flags & 8U)
            AssignCharacterControlFlags(character, movement.cachedCharacterControlFlags);
        movement.flags &= 0xfffbfff7U;
    }
    direction = 0;
    yawDistance = PLAYER_STAIR_ABS(static_cast<std::int16_t>(mtwkGetEntityYaw(work) - yaw));
    if (yawDistance < 0x2aaa)
        direction = 1;
    if (yawDistance > 0x5555)
        direction = -1;
    if (publishTarget) {
        if (yawDistance < 0x4000)
            movement.groundedStepIndex = 0;
        else
            movement.groundedStepIndex = steps;
    }
    mtwkGetControllerTargetPosition(work, 6, rightTarget);
    mtwkGetControllerTargetPosition(work, 5, leftTarget);
    mtwkGetBonePosition(work, 0x20, &rightBone);
    mtwkGetBonePosition(work, 0x1b, &leftBone);
    if (mtwkIsRightFootTargetAhead(work))
        probe = leftTarget;
    else
        probe = rightTarget;
    probe.y += 1.0f;
    probe.y = FindFieldIntersectionHeight(&probe);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    Matrix4x4::preTranslate(-anchor.x, -anchor.y, -anchor.z);
    Matrix4x4::TransformPoint(location, local);
    Matrix4x4::TransformPoint(rightTarget, rightTarget);
    Matrix4x4::TransformPoint(leftTarget, leftTarget);
    Matrix4x4::TransformPoint(rightBone, rightBone);
    Matrix4x4::TransformPoint(leftBone, leftBone);
    PopCurrentMatrix(1);
    am2Vector3Add(&rightMid, &rightBone, &rightBone);
    am2Vector3Add(&leftMid, &leftBone, &leftBone);
    am2Vector3Add(&rightMid, &rightTarget, &rightBone);
    am2Vector3Add(&leftMid, &leftTarget, &leftBone);
    local.z = -local.z;
    rightTarget.z = -rightTarget.z;
    leftTarget.z = -leftTarget.z;
    rightMid.z = -rightMid.z;
    leftMid.z = -leftMid.z;
    step = static_cast<std::int32_t>(local.z / 0.30000001f) + 1;
    if (step < 0)
        step = 0;
    upperStep = PLAYER_STAIR_MIN(step, steps);
    rightTargetLevel = static_cast<std::int32_t>(rightTarget.z / 0.30000001f) + 1;
    if (rightTargetLevel < 0)
        rightTargetLevel = 0;
    rightTargetLevel = PLAYER_STAIR_MIN(rightTargetLevel, steps);
    leftTargetLevel = static_cast<std::int32_t>(leftTarget.z / 0.30000001f) + 1;
    if (leftTargetLevel < 0)
        leftTargetLevel = 0;
    leftTargetLevel = PLAYER_STAIR_MIN(leftTargetLevel, steps);
    step = static_cast<std::int32_t>(rightMid.z * 0.5f / 0.30000001f) + 1;
    if (step < 0)
        step = 0;
    rightStep = PLAYER_STAIR_MIN(step, steps);
    step = static_cast<std::int32_t>(leftMid.z * 0.5f / 0.30000001f) + 1;
    if (step < 0)
        step = 0;
    leftStep = PLAYER_STAIR_MIN(step, steps);
    footStep = PLAYER_STAIR_MIN(rightStep, leftStep);
    rightHeight = static_cast<std::int32_t>(rightTarget.y / 0.22499999f);
    leftHeight = static_cast<std::int32_t>(leftTarget.y / 0.22499999f);
    lowest = PLAYER_STAIR_MIN(rightHeight, leftHeight);
    if (lowest < 0)
        lowest = 0;
    height = PLAYER_STAIR_MIN(lowest, steps);
    if (movement.flags & 8U)
        movement.upperBodyStepIndex = upperStep;
    movement.stepDirection = 0;
    if (steps > 0) {
        if (direction == 1) {
            if (local.z > -0.15000001f && static_cast<float>(steps - 1) * 0.30000001f > local.z && footStep < steps && height < steps)
                movement.stepDirection = direction;
        } else if (direction == -1) {
            if (local.z > -0.075000003f && static_cast<float>(steps) * 0.30000001f > local.z && footStep > 0 && height > 0)
                movement.stepDirection = direction;
        }
    }
    rightHeight = static_cast<std::int32_t>(rightTarget.y / 0.22499999f);
    leftHeight = static_cast<std::int32_t>(leftTarget.y / 0.22499999f);
    footStep = PLAYER_STAIR_MIN(rightHeight, leftHeight);
    footStep = PLAYER_STAIR_MIN(footStep < 0 ? 0 : footStep, movement.targetHeightScale);
    movement.groundedStepIndex = footStep;
    if (publishTarget) {
        if (yawDistance < 0x4000)
            movement.groundedStepIndex = 0;
        else
            movement.groundedStepIndex = steps;
    }
    if (!movement.stepDirection)
        return;
    if (movement.targetHeightScale > 0) {
        if (movement.stepDirection > 0) {
            boundary.x = -1.8f;
            boundary.y = static_cast<float>(movement.targetHeightScale) * 0.22499999f;
            boundary.z = static_cast<float>(-movement.targetHeightScale) * 0.30000001f;
            movement.stairBoundaryYaw = yaw;
        } else {
            boundary.x = -1.8f;
            boundary.y = 0.0f;
            boundary.z = 0.30000001f;
            movement.stairBoundaryYaw = yaw + 0x8000;
        }
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&anchor);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::TransformPoint(boundary, movement.stairBoundaryAnchor);
        PopCurrentMatrix(1);
    }
}


#undef PLAYER_STAIR_MIN
#undef PLAYER_STAIR_ABS

// @verified - d
void PrepareReachedStairPhaseCorrection(PlayerMovementState &movement) {
    const std::uint32_t animation = mtwkGetAnimationId(movement.motion, static_cast<MotionRegion>(0));
    if (animation != 0xf02bU && animation != 0xf009U && animation != 0xf12fU && animation != 0xf12eU && animation != 0xf02aU && animation != 0xf008U)
        return;
    if (!(movement.flags & 8U) || !movement.targetHeightScale)
        return;
    std::int32_t steps = 1;
    if (animation == 0xf02aU || animation == 0xf008U)
        steps = 2;
    std::int32_t phases = 2;
    if (animation - 0xf12eU <= 1U) {
        phases = 1;
        steps = 2;
    }
    movement.correctionCycleDistance = 0.0f;
    Vector3 terminal;
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(movement.motion), -1.0f, &terminal);
    float trackDistance = terminal.z;
    if (trackDistance < 0.0f)
        trackDistance = -trackDistance;
    const float frame = mtwkGetFrame(movement.motion, static_cast<MotionRegion>(0));
    float phaseLength = static_cast<float>(GetMotionFrameCount(animation)) - 1.0f;
    const float perFrame = trackDistance / phaseLength;
    phaseLength = phaseLength / static_cast<float>(phases);
    float phase = phaseLength - frame;
    if (phase < 0.0f)
        phase += phaseLength;
    const float travelled = phase * perFrame;
    float stepLength = 0.30000001f / dc::sdk::CosFixed(static_cast<std::int16_t>(movement.currentYaw - movement.targetWorldYaw));
    if (stepLength < 0.0f)
        stepLength = -stepLength;
    Vector3 position;
    GetCharacterWorldPosition(movement.character, &position);
    const float distance = GetPlanarDistance(&movement.rootConsumerPosition, &position) + travelled;
    const float correction =
        frame <= phaseLength ? stepLength * static_cast<float>(steps) - distance : (stepLength + stepLength) * static_cast<float>(steps) - distance;
    movement.pendingPositionCorrection = correction;
}


// @verified - d
void ApplyReachedCollisionRootCorrection(PlayerMovementState &movement) {
    float magnitude, limit, step, scale;
    Vector3 direction, position;
    magnitude = movement.pendingPositionCorrection < 0.0f ? -movement.pendingPositionCorrection : movement.pendingPositionCorrection;
    if (!(magnitude > 0.0f))
        return;
    limit = movement.rootMotionStep * 0.5f * (1.0f / static_cast<float>(movement.correctionCycleCount + 1));
    limit = limit > 0.02f ? limit : 0.02f;
    step = magnitude > limit ? limit : magnitude;
    if (movement.pendingPositionCorrection < 0.0f)
        step = -step;
    movement.pendingPositionCorrection -= step;
    if ((movement.pendingPositionCorrection < 0.0f ? -movement.pendingPositionCorrection : movement.pendingPositionCorrection) < 0.02f)
        movement.pendingPositionCorrection = 0.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::Scale(step, step, step);
    Matrix4x4::RotateAroundY_Wrapper(movement.currentYaw);
    GetTransformedForwardVector(&direction);
    PopCurrentMatrix(1);
    GetCharacterWorldPosition(movement.character, &position);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&position);
    Matrix4x4::TransformPoint(direction, position);
    PopCurrentMatrix(1);
    SetCharacterWorldPosition(movement.character, &position);
    if (movement.correctionCycleDistance * limit <
            (movement.pendingPositionCorrection < 0.0f ? -movement.pendingPositionCorrection : movement.pendingPositionCorrection) &&
        (movement.flags & 0x80U) && movement.correctionCycleDistance > 0.0f) {
        float ratio = movement.pendingPositionCorrection / movement.correctionCycleDistance;
        scale = (ratio + movement.rootMotionStep) / movement.rootMotionStep;
        movement.verticalMotionScale = (scale < 0.75f ? 0.75f : scale) < 1.25f ? (scale < 0.75f ? 0.75f : scale) : 1.25f;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native motion-input owner (PLYM +0x0e, published by shenToggleAreaFlags and
// the scripted approach callers).
void plymUpdateMotionInputState(PlayerModelState &state) {
    switch (state.motionInputState) {
    case 1:
        state.desiredYaw = state.currentYaw;
        state.activeMovementMode = 1;
        state.motionInputEcho = 0.0f;
        if (state.rootYawState == 0U)
            state.padFlags |= 0x20U;
        break;
    case 2:
        state.desiredYaw = state.currentYaw;
        state.activeMovementMode = state.requestedMovementMode;
        state.motionInputEcho = 0.0f;
        if (state.requestedMovementMode == 4U)
            state.activeMovementMode = 3;
        break;
    case 3:
        state.desiredYaw = state.currentYaw;
        state.activeMovementMode = 3;
        state.motionInputEcho = 0.0f;
        break;
    case 5:
    case 8: {
        const PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
        state.activeMovementMode = node->distance >= 0.55000001f && (state.rootYawState || node->distance >= 0.80000001f) ? 3 : 1;
        if (state.activeMovementMode == 3)
            state.desiredYaw = node->characterBearingYaw;
        if (state.activeMovementMode == 1 && !state.rootYawState) {
            state.padFlags |= 0x20U;
            if (state.motionInputState == 5)
                state.motionInputState = 0;
        }
        break;
    }
    case 7:
        plymUpdateScriptedPlacementTarget(state);
        break;
    case 10:
        plymUpdateInteractionPlacementTarget(state);
        break;
    default:
        break;
    }
}
#endif

} // namespace player_detail

} // namespace s2

} // namespace shenmue
