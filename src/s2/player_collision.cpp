#include "s2/player_internal.hpp"

namespace shenmue::s2::player_detail {

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


Vector3 RotateAngleAxis(const Vector3 &origin, float amount, std::int16_t angle) {
    const auto back = static_cast<std::uint16_t>(angle + 0x8000);
    return {sin_fixed16(static_cast<std::int16_t>(back)) * amount + origin.x, origin.y, cos_fixed16(back) * amount + origin.z};
}



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


void ComputeVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &target, const Vector3 &reference, const Vector3 &referenceEnd) {
    const Vector3 direction{target.x - base.x, target.y - base.y, target.z - base.z};
    const Vector3 crossVector{referenceEnd.x - reference.x, referenceEnd.y - reference.y, referenceEnd.z - reference.z};
    SimdSolveVectorProjection(output, base, direction, reference, crossVector);
}


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



std::int32_t flddRaycastScene(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination) {
    float squaredDistance = 100.0f;
    const std::uint8_t previousFlags = state.collisionFlags;
    state.collisionFlags = 0;
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
            }
        }
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
}

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

void plymProbeMovementCollision(PlayerModelState &state) {
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
}

void plymProbeDesiredYawCollision(PlayerModelState &state) {
    float rightDistance = 10.0f;
    float leftDistance = 10.0f;
    state.desiredYawProbeDistance = 10.0f;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(state.worldPosition.x, state.worldPosition.y, state.worldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.desiredYaw));
    Vector3 forward{}, rightEnd{}, leftEnd{}, rightStart{}, leftStart{};
    Matrix4x4::TransformPoint(plymRayForward, forward);
    Matrix4x4::TransformPoint(plymRayRightEnd, rightEnd);
    Matrix4x4::TransformPoint(plymRayLeftEnd, leftEnd);
    Matrix4x4::TransformPoint(plymRayRightStart, rightStart);
    Matrix4x4::TransformPoint(plymRayLeftStart, leftStart);
    Matrix4x4::Pop(1);
    if (flddRaycastScene(state, state.worldPosition, forward)) {
        state.desiredYawProbeDistance = state.collisionDistance;
        if (state.collisionType == plymRayHitField) {
            Vector3s normal{};
            transformIntersectionResult(normal);
            state.desiredProbeYaw = static_cast<std::int16_t>(normal.y + 0x4000);
        }
        if (state.collisionType == plymRayHitCharacter) {
            const Vector3 toHit{state.fieldCollisionPoint.x - state.worldPosition.x, state.fieldCollisionPoint.y - state.worldPosition.y,
                state.fieldCollisionPoint.z - state.worldPosition.z};
            const std::int16_t bearing = PlymAngleXZ(toHit);
            const float ratio = std::fmin(1.0f, std::fmax(0.0f, 0.80000001f / std::sqrt(toHit.z * toHit.z + toHit.x * toHit.x)));
            const std::uint16_t inverse = ComputeInverseAngleRatio(ratio);
            const auto relative = static_cast<std::int16_t>(bearing - state.desiredYaw);
            const auto offset = static_cast<std::int16_t>(relative >= 0 ? relative - (0x4000 - inverse) : 0x4000 - inverse + relative);
            state.desiredProbeYaw = static_cast<std::int16_t>(state.desiredYaw + offset);
        }
    }
    const float forwardDistance = state.desiredYawProbeDistance;
    if (flddRaycastScene(state, leftStart, leftEnd))
        leftDistance = state.collisionDistance;
    if (flddRaycastScene(state, rightStart, rightEnd))
        rightDistance = state.collisionDistance;
    if (forwardDistance > leftDistance && leftDistance > rightDistance && leftDistance < 0.80000001f && rightDistance < 0.80000001f)
        state.desiredYawProbeDistance = (leftDistance + rightDistance) * 0.5f;
}

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
    const std::uint16_t motionId = GetMotionAnimationId(motionWork, MotionRegion::LowerBody);
    const bool correctionMotion = motionId == 0xf005U || motionId == 0xf006U || (motionId >= 0xf035U && motionId <= 0xf03dU);

    if (!correctionMotion ||
        ((movement.flags & 8U) != 0 && static_cast<std::int32_t>(movement.targetWorldYaw) == static_cast<std::int32_t>(stairYaw) &&
            static_cast<std::int32_t>(movement.targetHeightScale) == stepCount)) {
        return;
    }

    const float scale = movement.field160 * movement.field15c;
    Vector3 terminalRoot;
    SampleNativeMotionRoot(motionId, GetMotionRigBoneId(motionWork), -1.0f, terminalRoot);
    const float trackDistance = std::fabs(terminalRoot.z);
    const float sampleFrame = GetMotionFrame(motionWork, MotionRegion::LowerBody);
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
    GetMotionAnimationId(motionWork, MotionRegion::LowerBody);
    Vector3 location = GetCharacterWorldPosition(character);
    location.y += 1.0f;
    movement.flags &= ~0x10000U;
    movement.sceneGroundIndex = static_cast<std::int8_t>(scenFindIntersection(location, location.y));
    if (movement.sceneGroundIndex != -1)
        movement.flags |= 0x10000U;

    location = GetCharacterWorldPosition(character);
    Vector3 probe{location.x, static_cast<float>(static_cast<double>(location.y) + 1.5), location.z};
    const float ground = shenFindIntersectionWithScene(probe, probe.y);

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
        GetMotionControllerTargetPosition(motionWork, 6, rightFoot);
        GetMotionControllerTargetPosition(motionWork, 5, leftFoot);
        GetMotionBonePosition(motionWork, MB_RightFoot, &rightFootBone);
        GetMotionBonePosition(motionWork, MB_LeftFoot, &leftFootBone);
        const Vector3 &probeFoot = GetIsRHandAboveShoulder(motionWork) ? leftFoot : rightFoot;
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
    const std::uint16_t motionId = GetMotionAnimationId(motionWork, MotionRegion::LowerBody);
    const bool standardStairMotion = motionId == 0xf008U || motionId == 0xf009U || motionId == 0xf02aU || motionId == 0xf02bU;
    const bool trackedStairMotion = motionId == 0xf12eU || motionId == 0xf12fU;
    if ((!standardStairMotion && !trackedStairMotion) || (movement.flags & 8U) == 0 || movement.targetHeightScale == 0)
        return;

    movement.correctionCycleDistance = 0.0f;
    Vector3 terminalRoot;
    SampleNativeMotionRoot(motionId, GetMotionRigBoneId(motionWork), -1.0f, terminalRoot);
    const float trackDistance = std::fabs(terminalRoot.z);
    const float sampleFrame = GetMotionFrame(motionWork, MotionRegion::LowerBody);
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



void plymUpdateGroundBoundaryTurn(PlayerModelState &state) {
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
}



void plymMatchCharacterSpeed(PlayerModelState &state) {
    if (state.limitingCollisionDistance > 3.0f || state.normalizedMotionInput > 0.0f)
        return;
    Character *const character = state.limitingCollisionCharacter;
    const shen_name name = GetCharacterScenarioName(character);
    if (!character)
        return;
    NpcRouteStateContext *const route = FindNpcRouteStateContext(name);
    if (!route || route->owner->liveState != 1U || (route->flags & 3U) == 0 || (route->flags & 0x101008U) != 0 || route->moveState == 3U)
        return;
    if (Abs16(GetAllAngYForNode(&character->entity.node) - state.currentYaw) > 0x2000U)
        return;
    const Vector3 local = PlymToLocal(state.currentYaw, state.worldPosition, GetCharacterWorldPosition(character));
    if (std::fabs(local.x) > 0.40000001f)
        return;
    state.movementSelectionFlags |= selectionCharacterSpeedMatch;
    if (std::fabs(route->motionStep) >= 0.00000011920929f)
        state.movementResult = route->motionStep * 0.97500002f;
}



std::int32_t NpcAvoidanceSide(shen_name name) {
    const NpcRouteStateContext *const route = FindNpcRouteStateContext(name);
    if (!route || (route->flags & 3U) != 3U || !(route->avoidanceResult == 2U || static_cast<std::int8_t>(route->avoidanceResult) >= 5) ||
        route->field0a0 != 1U)
        return 0;
    return route->avoidanceAuxiliary;
}



void plymResolveCollisionSteering(PlayerModelState &state) {
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
            const std::int32_t avoidance = -NpcAvoidanceSide(GetCharacterScenarioName(character));
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
}



void plymUpdateCollisionTurnActivation(PlayerModelState &state) {
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
}



void plymUpdateCollisionTurnLatch(PlayerModelState &state) {
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
}



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



void plymResolveCollisionTurn(PlayerModelState &state) {
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
}



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

} // namespace shenmue::s2::player_detail
