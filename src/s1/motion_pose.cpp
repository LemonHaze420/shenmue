#include "s1/motion_internal.hpp"

namespace shenmue::s1::motion_detail {
const MotionContactTarget *FindClosestFootContactTarget(const MotionContactTargetList &targets, const Vector3 &position, std::int32_t &indexOut);

void TransformPointByWorkMatrix(const Vector3 &value, Vector3 &output);

std::int16_t WrapS16(std::int32_t value);

void ReadHeadAngleAcceleration(MotionInformationWork &motion, std::int32_t type, std::int32_t direction, std::int16_t target, std::int16_t previous,
    std::int16_t current, std::int16_t velocity, std::int16_t &limit, float &scale);

void MOTN_readAndConvertPositionMOTNData(float frame, MotionNodeRuntime &node);

void MOTN_readAndConvertRotationMOTNData(float frame, MotionNodeRuntime &node);

void MOTN_ExtractAndProcessRootMotion(MotionNodeRuntime &root, float frame);

void MOTN_ApplyKeyframeDataAndTransformBone(MotionInformationWork &motion, MotionNodeRuntime *motionNode, Vector3 *lookOut, float frame);

bool MOTN_ApplyFKPoseWithRootMotion(
    MotionInformationWork &motion, MotionNodeRuntime *motionNodes, float frame, const Vector3 *rootPreTranslate, std::int8_t stage, Vector3 *lookOut);

void PublishMotionClock(Vector3i *entityEuler, std::uint32_t reverse, std::int32_t frameCount, float timerRef, float frameStart, float frameEnd,
    std::uint32_t mirror, float residual);

bool EvaluateActiveNodes(MotionInformationWork &motion);

bool EvaluatePreviousNodes(MotionInformationWork &motion);

void OffsetTargetNodes(MotionNodeRuntime *motionNodes, float x, float y, float z, bool includeY);

void MOTN_AlignNodesToTrackedNode(MotionInformationWork &motion, MotionNodeRuntime *motionNodes);

void MOTN_AlignBlendNodesToTrackedNode(MotionNodeRuntime *activeMotionNodes, MotionNodeRuntime *blendMotionNodes, std::int32_t tracked);

void MOTN_AlignBlendNodesToTrackedNodeXZ(MotionNodeRuntime *activeMotionNodes, MotionNodeRuntime *blendMotionNodes, std::int32_t tracked);

void MOTN_ApplyNodeOffsetPairs(MotionInformationWork &motion);

void MOTN_ApplyNodeConstraintCorrections(MotionInformationWork &motion);

std::uint32_t MOTN_SelectContainingConstraintEndpoint(const MotionNodeConstraintWorkspace &constraints, Vector4 &output);

std::uint32_t MOTN_ConstraintParentNode(std::uint32_t nodeId);

void MOTN_ResolveConstraintEndpoint(MotionInformationWork &motion, MotionConstraintEndpoint &endpoint);

void CalculateMidPoint(const Vector3 &pointA, const Vector3 &pointB, const Vector3 &point, Vector4 &output);

void CalculateMidPointBetweenVectors(const Vector3 &pointA, const Vector3 &pointB, const Vector3 &point, Vector4 &output);

void MOTN_SolveNodeConstraints(MotionInformationWork &motion);

void InterpolateJointPosition_CubicPolynomial(Vector3 &result, float amount, const MotionNodeRuntime &start, const MotionNodeRuntime &end);

void ApplyArmAimOffsetWithDamping(
    MotionNodeRuntime &ankle, const MotionContactTarget &target, Vector3 &current, Vector3 &damping, bool applyOffsets, float factor);

void ProjectToMaxDistance(MotionNodeRuntime &ankle, const MotionNodeRuntime &thigh, float maximum);

void ApplyArmTargetsAndLengthClamp_MOTN(MotionNodeRuntime *sourceMotionNodes, MotionNodeRuntime *activeMotionNodes);

void MOTN_SetNewMatrices(MotionNodeRuntime *sourceMotionNodes);

float LerpDouble(float a, float b, float t);

Vector3 PositionOf(const Vector4 &value);

void SetPosition(Vector4 &target, const Vector3 &value);

void MultiplyLerpedRotation(float t);

void VectorTransformWithRotationLerp(float t, const MotionNodeRuntime &source, const MotionNodeRuntime &target);

void TranslateVectorByConditions(const Vector3 &world, float *target, std::int32_t &blendFrames);

void BlendPositionWithGlobalTarget(float *target, std::int32_t &blendFrames);

void ApplyAnkleTarget(std::uint8_t ankleId, float reach, float *target);

void CalculateLocalToWorldTransformation(Vector4 &position);

void ApplyRotationTransform(MotionNodeRuntime &node, const Vector3 &target);

void Matrix4x4_ApplyVec3Rotation(const MotionNodeRuntime &node, const Vector3 &target);

void Vector3_LinearInterpolate(Vector3 &output, float t, const Vector3 &from, const Vector3 &to);

void CalculateAngleAndSetValue();

void PerformMatrixTransformationAndAnimationUpdate(const Vector3 &target, Vector3 &output);

MotionNodeRuntime *SearchBinaryTreeRecursive(std::uint32_t nodeId, MotionNodeRuntime *root);

void MOTN_RotateBonesInIKChainForFrame_Lerp(float t, MotionNodeRuntime &source, MotionNodeRuntime &target);

void PlaceSecondaryTargets(MotionNodeRuntime *sourceMotionNodes, const Matrix4x4 &anchor);

void Ryo_RecursiveIKForRealtimeMovement(MotionNodeRuntime &active, MotionNodeRuntime &blendSource);

void TranslateEuler(MotionNodeRuntime &node);

void TranslateOnly(MotionNodeRuntime &node);

void AdoptMotionMatrix(MotionNodeRuntime &node);

void ApplyRootTransform(MotionNodeRuntime &node);

void AimChildAtGrandchild(MotionNodeRuntime &node);

void ApplyTransformationWrapper(MotionNodeRuntime &node);

void AimChestChild(MotionNodeRuntime &node);

void ApplyChestTransform(MotionNodeRuntime &node);

void SolveTwoBoneChain(MotionNodeRuntime &anchor);

void BuildBoneTransform(MotionNodeRuntime &anchor);

void AnklePrepass();

void AdjustLookTargets(MotionNodeRuntime *node, std::size_t first, std::size_t second);

void AimHeadChild(MotionNodeRuntime &node, const Vector3 &target);

void ApplyHeadTransform(MotionNodeRuntime &node);

void Matrix4x4_preTranslate_RotateXYZ(const MotionNodeRuntime &node);

void MOTN_RotateAndTranslate(MotionNodeRuntime &node, MotionNodeRuntime &source);

void MOTN_LinkSecondaryNodes(MotionNodeRuntime &secondary, MotionNodeRuntime &active);

void MOTN_SampleSecondaryChain(MotionNodeRuntime &node, float frame);

void MOTN_SampleSecondaryHierarchy(MotionNodeRuntime &node, float frame, bool attached);

void CaptureSecondaryTarget(std::size_t index, const MotionNodeRuntime &motionNode);

void MOTN_CaptureEffectorsFromMatrices(MotionNodeRuntime *motionNodes);

void RotateVirtualBones(MotionNodeRuntime *motionNodes);

void ApplyHumanRig(MotionNodeRuntime *node, std::size_t firstLookTarget, std::size_t secondLookTarget);

void ApplyScriptedHuman35Rig(MotionNodeRuntime *node);

void ApplyCatRig(MotionNodeRuntime *node);

void ApplyDogLikeRig(MotionNodeRuntime *node);

void ApplyPnwRig(MotionNodeRuntime *node);

void ApplyYagRig(MotionNodeRuntime *node);

void MOTN_ApplyRigProgram(std::int32_t templateIndex, MotionNodeRuntime *motionNodes);

void RunMotionCallbacks(MotionInformationWork &motion);

bool StorePoseEvaluation(MotionInformationWork &motion, bool succeeded);

void PublishActiveClock(MotionInformationWork &motion);

bool ApplyActiveNodes(MotionInformationWork &motion);

bool ApplyPreviousNodes(MotionInformationWork &motion);

void AlignBlendNodes(MotionInformationWork &motion, MotionNodeRuntime *reference, MotionNodeRuntime *motionNodes);

void ApplyNodeConstraintPasses(MotionInformationWork &motion);

void StepBlendRamp(MotionInformationWork &motion);

void AdvanceActiveClock(MotionInformationWork &motion, float delta);

void AdvancePreviousClock(MotionInformationWork &motion, bool resetMode, float delta);

void RotateAroundAxis_PrecomputedTrig(const Vector3 &axis, float sine, float cosine);

bool Matrix4x4_BlendRotateByAngle(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end);

bool Matrix4x4_RotateAndBlend3D(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end);

void GenerateBlendedTransformationMatrix(float t, const MotionNodeRuntime &start, const MotionNodeRuntime &end);

void SetInterpAmountForAllKeyframesRecursively(MotionNodeRuntime *motionNode, float amount);

void MOTN_ProcessSequenceInterpolation(MotionInformationWork &motion);

bool CubicTargetSlot(std::uint8_t nodeId, std::int64_t &slot);

Vector3 TransformedTranslation(const Matrix4x4 &frame, const Vector3 &offset);

void CubicInterp(MotionNodeRuntime *blend, MotionNodeRuntime *active);

void MOTN_RotateBonesInIKChainForFrame_Cubic(float t, MotionNodeRuntime &start, MotionNodeRuntime &end);

void MOTN_RecursiveCubicBlend(MotionNodeRuntime &active, MotionNodeRuntime &blend);

float VectorAxis(const Vector3 &value, std::size_t axis) {
    return axis == 0 ? value.x : axis == 1 ? value.y : value.z;
}

void PushMotionMatrix() {
    if (!g_matrixStack.top.stack)
        return;
    *g_matrixStack.top.stack = workTransformMatrix;
    if (++g_matrixStack.top.info.index < g_matrixStack.top.info.capacity)
        ++g_matrixStack.top.stack;
}

void PopMotionMatrix() {
    if (!g_matrixStack.top.stack)
        return;
    std::int32_t count = g_matrixStack.top.info.index < 1 ? g_matrixStack.top.info.index : 1;
    std::int32_t pointerCount = count;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        pointerCount += g_matrixStack.top.info.capacity - g_matrixStack.top.info.index;
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - count);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

Vector3 LocalDirectionTo(const Matrix4x4 &matrix, const Vector3 &target) {
    const float dx = target.x - matrix.rows[3].x;
    const float dy = target.y - matrix.rows[3].y;
    const float dz = target.z - matrix.rows[3].z;
    return {
        matrix.rows[0].x * dx + matrix.rows[0].y * dy + matrix.rows[0].z * dz,
        matrix.rows[1].x * dx + matrix.rows[1].y * dy + matrix.rows[1].z * dz,
        matrix.rows[2].x * dx + matrix.rows[2].y * dy + matrix.rows[2].z * dz,
    };
}

const MotionContactTarget *FindClosestFootContactTarget(const MotionContactTargetList &targets, const Vector3 &position, std::int32_t &indexOut) {
    const MotionContactTarget *closest = nullptr;
    float closestDistanceSquared = 10000.0f;
    indexOut = 0;
    for (std::uint8_t index = 0; index < targets.count; ++index) {
        const MotionContactTarget &target = targets.entries[index];
        if (static_cast<std::uint8_t>(target.type - 7U) <= 11U)
            continue;
        const float dx = position.x - target.position.x;
        const float dy = position.y - target.position.y;
        const float dz = position.z - target.position.z;
        const float distanceSquared = dx * dx + dy * dy + dz * dz;
        if (distanceSquared >= closestDistanceSquared)
            continue;
        closestDistanceSquared = distanceSquared;
        closest = &target;
        indexOut = index;
    }
    return closest;
}

std::int16_t AdjustBoneRotation(MotionNodeRuntime &node, float blend, std::int16_t previousAngle) {
    MotionNodeRuntime *const child = node.children[0];
    MotionNodeRuntime *const grandchild = child->children[0];
    const Vector4 &tip = grandchild->children[0]->children[0]->transform->rows[3];
    const float x = tip.x;
    const float y = tip.y;
    const float z = tip.z;
    PushMotionMatrix();
    workTransformMatrix = *node.transform;
    Matrix4x4::preTranslate(child->bindPosition->x, child->bindPosition->y, child->bindPosition->z);
    Matrix4x4::SetViewMatrixFromEuler(child->bindEulerSource->y, child->bindEulerSource->x, child->bindEulerSource->z);
    const Matrix4x4 &frame = workTransformMatrix;
    const Vector3 local{(frame.rows[0].y * (y - frame.rows[3].y) + frame.rows[0].x * (x - frame.rows[3].x)) + frame.rows[0].z * (z - frame.rows[3].z),
        (frame.rows[1].y * (y - frame.rows[3].y) + frame.rows[1].x * (x - frame.rows[3].x)) + frame.rows[1].z * (z - frame.rows[3].z),
        (frame.rows[2].y * (y - frame.rows[3].y) + frame.rows[2].x * (x - frame.rows[3].x)) + frame.rows[2].z * (z - frame.rows[3].z)};
    Matrix4x4::RotateZY(local);
    Matrix4x4::InvertInPlace(nullptr);
    PushMotionMatrix();
    const Matrix4x4 &hand = *grandchild->transform;
    const float dy = y - hand.rows[3].y;
    const float dx = x - hand.rows[3].x;
    const float dz = z - hand.rows[3].z;
    const Vector3 handLocal{(dy * hand.rows[0].y + dx * hand.rows[0].x) + dz * hand.rows[0].z,
        (dy * hand.rows[1].y + dx * hand.rows[1].x) + dz * hand.rows[1].z, (dy * hand.rows[2].y + dx * hand.rows[2].x) + dz * hand.rows[2].z};
    workTransformMatrix = hand;
    Matrix4x4::RotateZY(handLocal);
    const Matrix4x4 aligned = workTransformMatrix;
    PopMotionMatrix();
    Matrix4x4::Multiply(&aligned);
    const Vector3i euler = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    workTransformMatrix = *grandchild->transform;
    const auto target = static_cast<std::int16_t>(
        static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(euler.x)) * blend - static_cast<float>(previousAngle)));
    const auto angle = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(target) * 0.02f + static_cast<float>(previousAngle)));
    const auto negated = static_cast<std::int16_t>(-angle);
    const auto folded = static_cast<std::uint16_t>(negated);
    Matrix4x4::RotateAroundX(sin_fixed16(negated), sin_fixed16(static_cast<std::int16_t>(0x4000 - (folded <= 0x8000U ? folded : 0x10000 - folded))));
    *grandchild->transform = workTransformMatrix;
    PopMotionMatrix();
    return angle;
}

bool IsFootPlantingExcludedAnimation(std::uint16_t animationId) {
    switch (animationId) {
    case 710:
    case 711:
    case 715:
    case 761:
    case 1334:
    case 1335:
        return true;
    default:
        return false;
    }
}

Vector3 BlendFootPlantingTarget(const MotionNodeRuntime &blendMotionNode, const MotionNodeRuntime &activeMotionNode) {
    const float amount = blendMotionNode.interpolationAmount;
    const float inverse = 1.0f - amount;
    return {blendMotionNode.effectorPosition.x * inverse + amount * activeMotionNode.effectorPosition.x,
        blendMotionNode.effectorPosition.y * inverse + amount * activeMotionNode.effectorPosition.y,
        blendMotionNode.effectorPosition.z * inverse + amount * activeMotionNode.effectorPosition.z};
}

float SolveRyoFootPlantHeight(float targetY, float groundY, bool previousApplied, float &solvedY, float &previousRawY) {
    const float offset = g_ryoFootPositions.z;
    float height = offset > 0.0f ? targetY + offset : targetY;
    float planted = static_cast<float>(static_cast<double>(groundY) + 0.117);
    const float gap = planted - height;
    if (!(previousApplied && gap >= 0.0f && 0.3 > static_cast<double>(gap)))
        planted = height;
    if (previousApplied) {
        const double rising = static_cast<double>(solvedY) + 0.05;
        const double falling = static_cast<double>(solvedY) - 0.02;
        const float rawDelta = height - previousRawY;
        if (static_cast<double>(planted) > rising && static_cast<double>(planted) > static_cast<double>(rawDelta + height) + 0.05)
            planted = static_cast<float>(rising);
        else if (falling > static_cast<double>(planted) && static_cast<double>(rawDelta + solvedY) - 0.02 > static_cast<double>(planted))
            planted = static_cast<float>(falling);
        previousRawY = height;
        height = planted;
    } else {
        previousRawY = height;
    }
    if (offset > 0.0f)
        height -= offset;
    solvedY = height;
    return height;
}

void TransformPointByWorkMatrix(const Vector3 &value, Vector3 &output) {
    output.x = value.x * workTransformMatrix.rows[0].x + value.y * workTransformMatrix.rows[1].x + value.z * workTransformMatrix.rows[2].x +
        workTransformMatrix.rows[3].x;
    output.y = value.x * workTransformMatrix.rows[0].y + value.y * workTransformMatrix.rows[1].y + value.z * workTransformMatrix.rows[2].y +
        workTransformMatrix.rows[3].y;
    output.z = value.x * workTransformMatrix.rows[0].z + value.y * workTransformMatrix.rows[1].z + value.z * workTransformMatrix.rows[2].z +
        workTransformMatrix.rows[3].z;
}

std::int16_t WrapS16(std::int32_t value) {
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(value));
}

void ReadHeadAngleAcceleration(MotionInformationWork &motion, std::int32_t type, std::int32_t direction, std::int16_t target, std::int16_t previous,
    std::int16_t current, std::int16_t velocity, std::int16_t &limit, float &scale) {
    if (type > 2)
        return;
    const std::size_t axis = static_cast<std::size_t>(type);
    const std::int16_t command = static_cast<std::int16_t>(motion.headLookAngleCommand & 0x7fffU);
    std::size_t tableIndex = axis;
    if (command != 102)
        tableIndex = command == 104 ? axis + 3U : axis + 6U;

    const std::int16_t remaining = WrapS16(current - previous);
    std::int16_t targetFromPrevious = WrapS16(target - previous);
    targetFromPrevious = WrapS16(targetFromPrevious - static_cast<std::int32_t>(static_cast<float>(velocity) * 0.33f));
    float positiveCurve = 0.0f;
    float negativeCurve = 0.0f;
    float brakingCurve = 0.0f;
    const float bias = kHeadCurveBias[axis];
    if (direction == 1) {
        if (remaining < targetFromPrevious) {
            if (previous < 0)
                brakingCurve = std::fabs(sin_fixed16(previous) * (bias + bias));
            else if (std::fabs(static_cast<float>(previous)) < 16384.0f)
                positiveCurve = -std::fabs(sin_fixed16(previous) * bias);
        } else if (velocity) {
            if ((static_cast<std::uint16_t>(current) & 0x8000U) == 0) {
                const std::int32_t delta = target - current;
                if (std::fabs(static_cast<float>(delta)) < 16384.0f) {
                    std::int32_t magnitude = -delta;
                    if (static_cast<std::uint32_t>(delta) <= 32768U)
                        magnitude = delta;
                    negativeCurve = -std::fabs(sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude)) * (bias * 0.5f));
                }
            }
        } else if (previous < 0) {
            brakingCurve = std::fabs(sin_fixed16(previous) * (bias + bias));
        } else if (std::fabs(static_cast<float>(previous)) < 16384.0f) {
            positiveCurve = -std::fabs(sin_fixed16(previous) * bias);
        }
    } else if (direction == -1) {
        if (remaining > targetFromPrevious) {
            if (previous >= 0)
                brakingCurve = std::fabs(sin_fixed16(previous) * (bias + bias));
            else if (std::fabs(static_cast<float>(previous)) < 16384.0f)
                positiveCurve = -std::fabs(sin_fixed16(previous) * bias);
        } else if (velocity) {
            if ((static_cast<std::uint16_t>(current) & 0x8000U) != 0) {
                const std::int32_t delta = target - current;
                if (std::fabs(static_cast<float>(delta)) < 16384.0f) {
                    std::int32_t magnitude = -delta;
                    if (static_cast<std::uint32_t>(delta) <= 32768U)
                        magnitude = delta;
                    negativeCurve = -std::fabs(sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude)) * (bias * 0.5f));
                }
            }
        } else if (previous >= 0) {
            brakingCurve = std::fabs(sin_fixed16(previous) * (bias + bias));
        } else if (std::fabs(static_cast<float>(previous)) < 16384.0f) {
            positiveCurve = -std::fabs(sin_fixed16(previous) * bias);
        }
    } else {
        brakingCurve = std::fabs(sin_fixed16(previous) * (bias + bias));
    }
    limit = WrapS16(static_cast<std::int32_t>((((brakingCurve + brakingCurve) + (positiveCurve + 0.7f)) + (negativeCurve + negativeCurve)) *
        static_cast<float>(kHeadAccelerationLimits[tableIndex])));
    scale = std::max(1.005f, ((kHeadAccelerationScales[tableIndex] + positiveCurve) + brakingCurve + negativeCurve) * kHeadAccelerationScales[tableIndex]);
}

void motnUpdate(MotionInformationWork &motion, HeadAngleUpdateRefs refs, std::int16_t target, std::int16_t &output, std::int32_t type) {
    const std::int32_t targetValue = target;
    const std::int32_t previousTarget = *refs.previousTarget;
    const std::int32_t total = targetValue - previousTarget;
    const std::int32_t currentValue = *refs.current;
    const std::int16_t progressed = WrapS16(currentValue - previousTarget);
    const std::int32_t remaining = targetValue - previousTarget - progressed;
    const std::int32_t denominator = static_cast<std::int32_t>(motion.headLookAngleCommand & 0x7fffU) - 100;
    std::int16_t step = WrapS16(4 * ((targetValue - currentValue) / denominator));
    const std::int16_t stopDistance = type == 2 ? 384 : 3072;
    const std::int32_t direction = remaining >= 0 ? 1 : -1;
    const std::uint32_t stopFlag = type == 0 ? 0x2000U : type == 1 ? 1U : type == 2 ? 2U : 0U;
    const std::uint32_t accelFlag = type == 0 ? 0x8000U : type == 1 ? 0x200U : type == 2 ? 0x400U : 0U;
    const std::uint32_t decelFlag = type == 0 ? 0x100000U : type == 1 ? 0x40000U : type == 2 ? 0x80000U : 0U;

    if (total != progressed && (motion.rootAngleFlags & stopFlag) == 0) {
        std::int16_t accelerationLimit{};
        float accelerationScale{};
        ReadHeadAngleAcceleration(motion, type, direction, target, static_cast<std::int16_t>(previousTarget), static_cast<std::int16_t>(currentValue),
            *refs.decelerationOrigin, accelerationLimit, accelerationScale);
        const bool secondHalf = (direction == 1 && progressed >= (total >> 1)) || (direction == -1 && progressed <= (total >> 1));
        if (secondHalf) {
            if ((motion.rootAngleFlags & decelFlag) == 0) {
                motion.rootAngleFlags |= decelFlag;
                *refs.decelerationOrigin = WrapS16(total >> 1);
            }
            if ((direction == 1 && progressed < total - *refs.decelerationOrigin) || (direction == -1 && progressed > total - *refs.decelerationOrigin)) {
                step = *refs.velocity;
                if (std::fabs(static_cast<float>(step)) > static_cast<float>(accelerationLimit)) {
                    step = WrapS16(accelerationLimit * direction);
                    *refs.acceleration = step;
                }
            } else if (std::fabs(static_cast<float>(*refs.velocity)) > 0.0f) {
                const float reduced = static_cast<float>(*refs.acceleration) / accelerationScale;
                if (std::fabs(static_cast<float>(*refs.acceleration)) / accelerationScale < 12.0f) {
                    *refs.acceleration = WrapS16(12 * direction);
                } else if (std::fabs(reduced) > static_cast<float>(accelerationLimit)) {
                    *refs.acceleration = WrapS16(accelerationLimit * direction);
                } else {
                    *refs.acceleration = WrapS16(static_cast<std::int32_t>(reduced));
                }
                step = *refs.acceleration;
            } else {
                step = *refs.acceleration;
            }
            if (std::fabs(static_cast<float>(step)) > static_cast<float>(accelerationLimit))
                step = WrapS16(accelerationLimit * direction);
        } else {
            std::int16_t velocity = *refs.velocity;
            if (static_cast<float>(accelerationLimit) > std::fabs(static_cast<float>(velocity))) {
                if ((motion.rootAngleFlags & accelFlag) == 0) {
                    *refs.acceleration = WrapS16(12 * direction);
                    motion.rootAngleFlags |= accelFlag;
                }
                step = WrapS16(static_cast<std::int32_t>(static_cast<float>(*refs.acceleration) * accelerationScale));
                *refs.acceleration = step;
                if (std::fabs(static_cast<float>(step)) > static_cast<float>(accelerationLimit)) {
                    if ((motion.rootAngleFlags & decelFlag) == 0) {
                        motion.rootAngleFlags |= decelFlag;
                        *refs.decelerationOrigin = progressed;
                    }
                    *refs.acceleration = WrapS16(static_cast<std::int32_t>(static_cast<float>(*refs.acceleration) / accelerationScale));
                    step = WrapS16(accelerationLimit * direction);
                }
            } else {
                if (std::fabs(static_cast<float>(velocity)) > static_cast<float>(accelerationLimit)) {
                    velocity = WrapS16(accelerationLimit * direction);
                    *refs.acceleration = velocity;
                }
                step = velocity;
                if ((motion.rootAngleFlags & decelFlag) == 0) {
                    motion.rootAngleFlags |= decelFlag;
                    *refs.decelerationOrigin = progressed;
                }
            }
        }
    }

    bool reset = false;
    if ((motion.rootAngleFlags & stopFlag) != 0 && std::fabs(static_cast<float>(remaining)) > static_cast<float>(stopDistance)) {
        step = 0;
        *refs.previousTarget = *refs.current;
        motion.rootAngleFlags &= ~(accelFlag | decelFlag | stopFlag);
        reset = true;
    }
    if ((motion.rootAngleFlags & stopFlag) != 0) {
        output = target;
    } else {
        const std::int32_t signedVelocity = direction * *refs.velocity;
        if (signedVelocity < 0 && !reset) {
            if (std::fabs(static_cast<float>(remaining)) > static_cast<float>(stopDistance)) {
                output = *refs.current;
                *refs.previousTarget = *refs.current;
                motion.rootAngleFlags &= ~(accelFlag | decelFlag);
            } else {
                output = target;
                motion.rootAngleFlags |= stopFlag;
            }
        } else if (std::fabs(static_cast<float>(remaining)) >= 12.0f) {
            const std::int16_t current = *refs.current;
            const std::int16_t candidate = WrapS16(step + current);
            output = candidate;
            const bool within = signedVelocity >= 0 && direction == 1 ? candidate <= target : direction != -1 || candidate >= target;
            if (!within) {
                if (reset) {
                    output = current;
                } else if ((motion.rootAngleFlags & decelFlag) != 0) {
                    output = target;
                    motion.rootAngleFlags |= stopFlag;
                }
            } else if (signedVelocity < 0 && reset) {
                output = current;
                *refs.previousTarget = current;
                motion.rootAngleFlags &= ~(accelFlag | decelFlag);
            }
        } else {
            output = target;
            motion.rootAngleFlags |= stopFlag;
        }
    }
    *refs.velocity = step;
}

void MOTN_ClampAnimationAngles(MotionInformationWork &motion, std::int16_t &yaw, std::int16_t &pitch) {
    const std::uint32_t flags = motion.rootAngleFlags;
    const std::uint32_t command = motion.headLookAngleCommand;
    if ((command & 0x8000U) != 0 && (command & 0x7fffU) >= 101U) {
        constexpr std::array<std::int16_t, 4> baseYaw{9102, 10012, 10922, 11832};
        constexpr std::array<float, 46> rigScale{1.4f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.04f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 0.8f, 1.2f, 1.04f, 1.0f,
            1.04f, 1.0f, 0.9f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.0f, 1.2f, 1.5f,
            1.2f, 1.0f, 1.2f, 1.0f};
        const std::int32_t pose = motion.activeBasePoseAnimIndex;
        const std::int32_t limitIndex = pose <= 7 ? 3 - (pose >> 1) : 0;
        std::int32_t scaleIndex = static_cast<std::int32_t>(motion.basePoseTemplateIndex) * 2;
        if (scaleIndex > 42)
            scaleIndex = 42;
        const float yawLimit = static_cast<float>(baseYaw.data()[limitIndex]) * rigScale.data()[scaleIndex];
        if (std::fabs(static_cast<float>(yaw)) > yawLimit) {
            yaw = static_cast<std::int16_t>(std::trunc(yaw < 0 ? -yawLimit : yawLimit));
            motion.rootAngleFlags |= 4U;
        }
        const std::uint16_t unsignedYaw = static_cast<std::uint16_t>(yaw);
        const std::int32_t magnitude = unsignedYaw <= 0x8000U ? unsignedYaw : 0x10000 - unsignedYaw;
        const float sineScale = std::fabs(sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude)));
        const float pitchLimit =
            pitch < 0 ? sineScale * (rigScale[scaleIndex + 1U] * 10922.0f) : sineScale * ((motion.rootAngleFlags & 0x80U) != 0 ? 2816.0f : 5632.0f);
        if (std::fabs(static_cast<float>(pitch)) > pitchLimit) {
            pitch = static_cast<std::int16_t>(std::trunc(pitch < 0 ? -pitchLimit : pitchLimit));
            motion.rootAngleFlags |= 8U;
        }
        return;
    }
    const std::int16_t maximumYaw = (flags & 0x400000U) != 0 ? 16384 : static_cast<std::int16_t>(14336 - (motion.activeBasePoseAnimIndex << 10));
    const std::int16_t minimumYaw = (flags & 0x400000U) != 0 ? -16384 : static_cast<std::int16_t>((motion.activeBasePoseAnimIndex << 10) - 14336);
    const std::int16_t maximumPitch = (flags & 0x400000U) != 0 ? 4608 : static_cast<std::int16_t>((8 - motion.nextBasePoseAnimIndex) << 9);
    if (yaw > maximumYaw) {
        yaw = maximumYaw;
        motion.rootAngleFlags |= 4U;
    }
    if (yaw < minimumYaw) {
        yaw = minimumYaw;
        motion.rootAngleFlags |= 4U;
    }
    if (pitch > maximumPitch) {
        pitch = maximumPitch;
        motion.rootAngleFlags |= 8U;
    }
    if ((motion.rootAngleFlags & 0x10U) != 0) {
        if (motion.cachedClampedLookAngle != maximumYaw && motion.cachedClampedLookAngle != minimumYaw) {
            motion.cachedClampedLookAngle = yaw;
        }
        yaw = motion.cachedClampedLookAngle;
    } else {
        motion.cachedClampedLookAngle = yaw;
    }
}

Matrix4x4 *MotionNodeMatrix(MotionNodeRuntime &node) {
    return node.transform ? node.transform : g_matrixStack.top.stack;
}

// MOTN_readAndConvertPositionMOTNData.
void MOTN_readAndConvertPositionMOTNData(float frame, MotionNodeRuntime &node) {
    const float time = static_cast<float>(static_cast<double>(frame) * kMotnFrameTime);
    MotnKeyframeChannel *channel = node.translationKeys;
    if (!channel) {
        node.effectorPosition.x = node.bindPosition->x;
        node.effectorPosition.y = node.bindPosition->y;
        node.effectorPosition.z = node.bindPosition->z;
        return;
    }
    float *const output = &node.effectorPosition.x;
    const float *const rootMotion = &g_RootMotionVector.x;
    const float *const bind = &node.bindPosition->x;
    for (std::size_t axis = 0; axis < 3; ++axis, ++channel) {
        if (!channel->present) {
            output[axis] = bind[axis];
            continue;
        }
        if (g_motnReverse) {
            float value = MOTN_readAndConvertData(*channel, time) * node.timeScale;
            if (g_motnMirror && axis == 0)
                value = -value;
            output[axis] = value + rootMotion[axis];
        } else {
            const float value = MOTN_CalculateKeyframeForTime(*channel, time, g_motnFrameCount) * node.timeScale;
            output[axis] = axis == 0 && g_motnMirror ? rootMotion[axis] - value : value + rootMotion[axis];
        }
    }
}

// MOTN_readAndConvertRotationMOTNData.
void MOTN_readAndConvertRotationMOTNData(float frame, MotionNodeRuntime &node) {
    const float time = static_cast<float>(static_cast<double>(frame) * kMotnFrameTime);
    MotnKeyframeChannel *channel = node.rotationKeys;
    if (!channel) {
        node.bindEuler = *node.bindEulerSource;
        return;
    }
    std::int16_t *const output = &node.bindEuler.x;
    for (std::size_t axis = 0; axis < 3; ++axis, ++channel) {
        if (!channel->present)
            continue;
        const float sample = g_motnReverse ? MOTN_readAndConvertData(*channel, time) : MOTN_CalculateKeyframeForTime(*channel, time, g_motnFrameCount);
        const std::int16_t value = Fixed16(sample * 65536.0f);
        output[axis] = value;
        if (g_motnMirror && node.nodeKind != 4) {
            const float scale = VectorAxis(kBoneDirections[node.nodeKind * 2U], axis);
            const float bias = VectorAxis(kBoneDirections[node.nodeKind * 2U + 1U], axis);
            output[axis] = Fixed16(static_cast<float>(value) * scale - bias * 65536.0f);
        }
    }
}

// MOTN_ExtractAndProcessRootMotion: samples the root at the clock published in the MOTN globals and folds the XZ travel since
// the reference frame (and the wrapped residual segment) into MOTN::RootMotionVector.
void MOTN_ExtractAndProcessRootMotion(MotionNodeRuntime &root, float frame) {
    float wrapX = 0.0f;
    float wrapZ = 0.0f;
    float referenceX = 0.0f;
    float referenceZ = 0.0f;
    g_RootMotionVector = {};
    float reference = g_motnTimerRef;
    if (!g_motnReverse && g_motnResidualTime > 0.0f) {
        const float end = ceil_float(g_motnFrameEnd);
        MOTN_readAndConvertPositionMOTNData(end - g_motnResidualTime, root);
        const float beforeX = -root.effectorPosition.x;
        const float beforeZ = -root.effectorPosition.z;
        MOTN_readAndConvertPositionMOTNData(end, root);
        reference = g_motnFrameStart;
        wrapX = beforeX + root.effectorPosition.x;
        wrapZ = beforeZ + root.effectorPosition.z;
        g_motnTimerRef = g_motnFrameStart;
    }
    if (reference >= 0.0f) {
        MOTN_readAndConvertPositionMOTNData(reference, root);
        referenceX = -root.effectorPosition.x;
        referenceZ = -root.effectorPosition.z;
    }
    g_RootMotionVector.x = referenceX + wrapX;
    g_RootMotionVector.z = referenceZ + wrapZ;
    MOTN_readAndConvertPositionMOTNData(frame, root);
    MOTN_readAndConvertRotationMOTNData(frame, root);
}

// MOTN_ApplyKeyframeDataAndTransformBone: samples every non-root row, moves the position (IK target) rows into the entity frame
// and applies the MOTI local/world target overrides. The head-look and ankle rows publish their keyed position first.
void MOTN_ApplyKeyframeDataAndTransformBone(MotionInformationWork &motion, MotionNodeRuntime *motionNode, Vector3 *lookOut, float frame) {
    for (; motionNode->nodeId != 0xff; ++motionNode) {
        MOTN_readAndConvertRotationMOTNData(frame, *motionNode);
        if (motionNode->templateParentOrMode != 4)
            continue;
        MOTN_readAndConvertPositionMOTNData(frame, *motionNode);
        const Vector3 keyed{motionNode->effectorPosition.x, motionNode->effectorPosition.y, motionNode->effectorPosition.z};
        std::int32_t slot = -1;
        switch (motionNode->nodeId) {
        case 3:
            slot = 1;
            break;
        case 6:
            slot = 2;
            if (lookOut)
                Matrix4x4::TransformPoint(keyed, *lookOut);
            break;
        case 11:
            slot = 3;
            if (lookOut)
                Matrix4x4::TransformPoint(keyed, motion.basePoseAnklePosition[0]);
            break;
        case 17:
            slot = 4;
            if (lookOut)
                Matrix4x4::TransformPoint(keyed, motion.basePoseAnklePosition[1]);
            break;
        case 21:
            slot = 5;
            break;
        case 26:
            slot = 6;
            break;
        case 33:
            slot = 7;
            break;
        default:
            break;
        }
        if (slot >= 0) {
            if (const Vector3 *const local = motion.localTargets[slot]; local && lookOut) {
                motionNode->effectorPosition.x = local->x;
                motionNode->effectorPosition.y = local->y;
                motionNode->effectorPosition.z = local->z;
            }
            if (const Vector3 *const world = motion.worldTargets[slot]; world && lookOut && static_cast<std::uint32_t>(slot - 3) > 1U) {
                motionNode->effectorPosition.x = world->x;
                motionNode->effectorPosition.y = world->y;
                motionNode->effectorPosition.z = world->z;
                continue;
            }
        }
        const Vector3 local{motionNode->effectorPosition.x, motionNode->effectorPosition.y, motionNode->effectorPosition.z};
        Vector3 world{};
        Matrix4x4::TransformPoint(local, world);
        motionNode->effectorPosition.x = world.x;
        motionNode->effectorPosition.y = world.y;
        motionNode->effectorPosition.z = world.z;
    }
}

// IsVectorWithinRadius: view-cone visibility of a world point against the current (camera) work matrix.
bool IsVectorWithinRadius(const Vector3 &point, float radius) {
    Vector3 view{};
    Matrix4x4::TransformPoint(point, view);
    const float depth = -view.z + radius;
    if (depth < 0.0f)
        return false;
    const Vector2 &scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depth * scale.x + radius;
    if (view.x > halfWidth || -halfWidth >= view.x)
        return false;
    const float halfHeight = depth * scale.y + radius;
    return view.y <= halfHeight && -halfHeight < view.y;
}

// MOTN_ApplyFKPoseWithRootMotion: samples one row set at the published clock, places the root in the entity frame and, when
// the root is inside the view cone (or the evaluation stage allows off-screen poses), samples and places the remaining rows.
bool MOTN_ApplyFKPoseWithRootMotion(
    MotionInformationWork &motion, MotionNodeRuntime *motionNodes, float frame, const Vector3 *rootPreTranslate, std::int8_t stage, Vector3 *lookOut) {
    MotionNodeRuntime &root = motionNodes[0];
    MOTN_ExtractAndProcessRootMotion(root, frame);
    if ((g_pMotion->motionFlags & 0x400U) != 0) {
        g_RootMotionVector.x -= root.effectorPosition.x;
        g_RootMotionVector.y -= root.effectorPosition.y;
        root.effectorPosition.x = 0.0f;
        root.effectorPosition.y = 0.0f;
        g_RootMotionVector.z -= root.effectorPosition.z;
        root.effectorPosition.z = 0.0f;
    } else if ((g_pMotion->motionFlags & 8U) != 0) {
        g_RootMotionVector.x -= root.effectorPosition.x;
        g_RootMotionVector.z -= root.effectorPosition.z;
        root.effectorPosition.x = 0.0f;
        root.effectorPosition.z = 0.0f;
    }
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    if (rootPreTranslate)
        Matrix4x4::preTranslate(rootPreTranslate->x, rootPreTranslate->y, rootPreTranslate->z);
    if (g_motnEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
    const Vector3 rootLocal{root.effectorPosition.x, root.effectorPosition.y, root.effectorPosition.z};
    Vector3 rootWorld{};
    Matrix4x4::TransformPoint(rootLocal, rootWorld);
    root.effectorPosition.x = rootWorld.x;
    root.effectorPosition.y = rootWorld.y;
    root.effectorPosition.z = rootWorld.z;
    Matrix4x4_Push();
    workTransformMatrix = g_pMotion->rootTransformCache;
    const bool visible = IsVectorWithinRadius(rootWorld, g_motnClipAngle);
    Matrix4x4::Pop(1);
    if (visible || stage < 2)
        MOTN_ApplyKeyframeDataAndTransformBone(motion, motionNodes + 1, lookOut, frame);
    Matrix4x4::Pop(1);
    return visible;
}

// The clock every evaluator publishes before its MOTN_ApplyFKPoseWithRootMotion pass.
void PublishMotionClock(Vector3i *entityEuler, std::uint32_t reverse, std::int32_t frameCount, float timerRef, float frameStart, float frameEnd,
    std::uint32_t mirror, float residual) {
    g_motnEntityEuler = entityEuler;
    g_motnReverse = reverse;
    g_motnFrameCount = frameCount;
    g_motnTimerRef = timerRef;
    g_motnFrameStart = frameStart;
    g_motnFrameEnd = frameEnd;
    g_motnMirror = mirror;
    g_motnResidualTime = residual;
}

// The active-clock publication and FK pass that opens every FCV mode evaluator. Returns false when the evaluator aborts.
bool EvaluateActiveNodes(MotionInformationWork &motion) {
    PublishMotionClock(&motion.entityEuler, motion.motionFlags & 0x20U, MOTN_GetFrameCountFromAnimId(motion.currentAnimId), motion.frameTimerRef,
        motion.frameStart, motion.frameEnd, (motion.motionFlags >> 17U) & 1U, motion.residualFrameTime);
    motion.poseEvaluationSucceeded = MOTN_ApplyFKPoseWithRootMotion(motion, static_cast<MotionNodeRuntime *>(motion.activeNodes), motion.frameTimer,
        &motion.rootPreTranslate, static_cast<std::int8_t>(motion.poseEvaluationStage), &motion.rootOffsetPosition);
    if (motion.poseEvaluationSucceeded)
        return true;
    const auto stage = static_cast<std::int8_t>(motion.poseEvaluationStage);
    if (stage >= 2) {
        motion.updateMode = 2;
        return false;
    }
    if (stage == 1)
        motion.updateMode = 1;
    return true;
}

// The previous-clock publication and FK pass of the previous-pose evaluators (IdleMaybe, Unknown_1).
bool EvaluatePreviousNodes(MotionInformationWork &motion) {
    PublishMotionClock(&motion.nextEntityEuler, motion.motionFlags & 0x20U, MOTN_GetFrameCountFromAnimId(motion.requestedAnimId), motion.previousFrameTimerRef,
        motion.previousFrameStart, motion.previousFrameEnd, (motion.motionFlags >> 18U) & 1U, motion.previousResidualFrameTime);
    return MOTN_ApplyFKPoseWithRootMotion(motion, static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), motion.previousFrameTimer,
        &motion.rootPreTranslate, static_cast<std::int8_t>(motion.poseEvaluationStage), &motion.previousRootOffsetPosition);
}

MotionNodeRuntime *FindTrackedNode(MotionNodeRuntime *motionNodes, std::uint32_t nodeId) {
    for (MotionNodeRuntime *motionNode = motionNodes; motionNode; ++motionNode) {
        if (motionNode->nodeId == nodeId)
            return motionNode;
        if (motionNode->nodeId == 0xff)
            return nullptr;
    }
    return nullptr;
}

// MOTN_CaptureTrackedNodePosition: stores the tracked node's previous evaluated effector.
void MOTN_CaptureTrackedNodePosition(MotionInformationWork &motion) {
    if (motion.updateMode) {
        motion.trackedNodeIndex = 10;
        return;
    }
    const std::uint32_t index = static_cast<std::uint32_t>(motion.trackedNodeIndex) & 0x7fffffffU;
    MotionNodeRuntime *const motionNode =
        index < kMotnTargetNodeIds.size() ? FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.previousNodes), kMotnTargetNodeIds[index]) : nullptr;
    if (motionNode && (motionNode->bindPositionPacked[1] & 2U) != 0) {
        g_motnTrackedNodePosition = {motionNode->effectorPosition.x, motionNode->effectorPosition.y, motionNode->effectorPosition.z};
        return;
    }
    motion.trackedNodeIndex = 10;
}

void OffsetTargetNodes(MotionNodeRuntime *motionNodes, float x, float y, float z, bool includeY) {
    for (std::size_t index = 0; index < 10; ++index) {
        MotionNodeRuntime *const motionNode = FindTrackedNode(motionNodes, kMotnTargetNodeIds[index]);
        if (!motionNode)
            continue;
        if (includeY) {
            const float nextY = motionNode->effectorPosition.y - y;
            motionNode->effectorPosition.x -= x;
            motionNode->effectorPosition.y = nextY;
            motionNode->effectorPosition.z -= z;
        } else {
            motionNode->effectorPosition.x -= x;
            motionNode->effectorPosition.z -= z;
        }
    }
}

// CompareNodeIDAndSubtractCoordinates (MOTN_AlignNodesToTrackedNode): keeps the tracked node pinned where it was last frame.
void MOTN_AlignNodesToTrackedNode(MotionInformationWork &motion, MotionNodeRuntime *motionNodes) {
    const std::int32_t tracked = motion.trackedNodeIndex;
    const std::uint32_t index = static_cast<std::uint32_t>(tracked) & 0x7fffffffU;
    if (index >= 10U || !motionNodes)
        return;
    MotionNodeRuntime *const motionNode = FindTrackedNode(motionNodes, kMotnTargetNodeIds[index]);
    if (!motionNode)
        return;
    const float x = motionNode->effectorPosition.x - g_motnTrackedNodePosition.x;
    const float z = motionNode->effectorPosition.z - g_motnTrackedNodePosition.z;
    const float y = tracked >= 0 ? 0.0f : motionNode->effectorPosition.y - g_motnTrackedNodePosition.y;
    OffsetTargetNodes(motionNodes, x, y, z, true);
}

// MOTN_AlignBlendNodesToTrackedNode: moves the blend rows by the tracked node's offset, or by the root offset
// between the two poses when no node is tracked.
void MOTN_AlignBlendNodesToTrackedNode(MotionNodeRuntime *activeMotionNodes, MotionNodeRuntime *blendMotionNodes, std::int32_t tracked) {
    const std::uint32_t index = static_cast<std::uint32_t>(tracked) & 0x7fffffffU;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (index == 10U) {
        x = blendMotionNodes->effectorPosition.x - activeMotionNodes->effectorPosition.x;
        z = blendMotionNodes->effectorPosition.z - activeMotionNodes->effectorPosition.z;
    } else if (MotionNodeRuntime *const motionNode = blendMotionNodes ? FindTrackedNode(blendMotionNodes, kMotnTargetNodeIds[index]) : nullptr) {
        x = motionNode->effectorPosition.x - g_motnTrackedNodePosition.x;
        y = motionNode->effectorPosition.y - g_motnTrackedNodePosition.y;
        z = motionNode->effectorPosition.z - g_motnTrackedNodePosition.z;
    }
    if (index == 10U || tracked >= 0)
        y = 0.0f;
    OffsetTargetNodes(blendMotionNodes, x, y, z, true);
}

// recompute_effector_pos (MOTN_AlignBlendNodesToTrackedNodeXZ): the STATE_PROCESS_COMPLETE variant, which shifts only X/Z and only
// once the offset reaches 0.3.
void MOTN_AlignBlendNodesToTrackedNodeXZ(MotionNodeRuntime *activeMotionNodes, MotionNodeRuntime *blendMotionNodes, std::int32_t tracked) {
    const std::uint32_t index = static_cast<std::uint32_t>(tracked) & 0x7fffffffU;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (index == 10U) {
        x = blendMotionNodes->effectorPosition.x - activeMotionNodes->effectorPosition.x;
        z = blendMotionNodes->effectorPosition.z - activeMotionNodes->effectorPosition.z;
    } else if (MotionNodeRuntime *const motionNode = blendMotionNodes ? FindTrackedNode(blendMotionNodes, kMotnTargetNodeIds[index]) : nullptr) {
        x = motionNode->effectorPosition.x - g_motnTrackedNodePosition.x;
        z = motionNode->effectorPosition.z - g_motnTrackedNodePosition.z;
        if (tracked < 0)
            y = motionNode->effectorPosition.y - g_motnTrackedNodePosition.y;
    }
    if (y * y + x * x + z * z >= 0.090000004f)
        OffsetTargetNodes(blendMotionNodes, x, 0.0f, z, false);
}

// HardlyNoticeable_halfFloatToFloat_1 (MOTN_ApplyNodeOffsetPairs): subtracts the queued half-float offsets from their rows.
void MOTN_ApplyNodeOffsetPairs(MotionInformationWork &motion) {
    auto *pair = static_cast<MotionNodeOffsetPair *>(motion.nodeOffsetPairCursor);
    if (!pair)
        return;
    for (;;) {
        if (MotionNodeRuntime *const motionNode =
                FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.activeNodes), static_cast<std::uint32_t>(pair->nodeId))) {
            motionNode->effectorPosition.x -= F16ToF32(pair->offset.x);
            motionNode->effectorPosition.y -= F16ToF32(pair->offset.y);
            motionNode->effectorPosition.z -= F16ToF32(pair->offset.z);
            motion.nodeConstraints = nullptr;
        }
        if ((pair->flags & 4U) != 0)
            break;
        motion.nodeOffsetPairCursor = ++pair;
    }
    motion.nodeOffsetPairCursor = nullptr;
}

// UpdateVectorIfMatched (MOTN_ApplyNodeConstraintCorrections).
void MOTN_ApplyNodeConstraintCorrections(MotionInformationWork &motion) {
    auto *const constraints = static_cast<MotionNodeConstraintWorkspace *>(motion.nodeConstraints);
    if (!constraints)
        return;
    for (MotionNodeConstraintState &state : constraints->states) {
        MotionNodeRuntime *const motionNode = FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.activeNodes), static_cast<std::uint32_t>(state.nodeId));
        if (!motionNode || (state.flags & 1U) == 0)
            continue;
        motionNode->effectorPosition.x -= state.correction.x;
        motionNode->effectorPosition.y -= state.correction.y;
        motionNode->effectorPosition.z -= state.correction.z;
    }
}

// ComparePointDistancesAndStoreSmaller: returns the endpoint whose sphere contains the other, or 255 when neither does.
std::uint32_t MOTN_SelectContainingConstraintEndpoint(const MotionNodeConstraintWorkspace &constraints, Vector4 &output) {
    const MotionConstraintEndpoint &first = constraints.endpoints[0];
    const MotionConstraintEndpoint &second = constraints.endpoints[1];
    const float dx = second.position.x - first.position.x;
    const float dy = second.position.y - first.position.y;
    const float dz = second.position.z - first.position.z;
    const float distance = std::sqrt((dy * dy + dx * dx) + dz * dz);
    if (first.radius >= distance + second.radius) {
        output = {first.position.x, first.position.y, first.position.z, first.radius};
        return 0;
    }
    if (second.radius >= distance + first.radius) {
        output = {second.position.x, second.position.y, second.position.z, second.radius};
        return 1;
    }
    return 255;
}

// MOTN_ConstraintParentNode: the parent joint whose evaluated matrix a mode-1 constraint pushes away from.
std::uint32_t MOTN_ConstraintParentNode(std::uint32_t nodeId) {
    switch (nodeId) {
    case 3:
        return 1;
    case 6:
        return 4;
    case 11:
        return 8;
    case 17:
        return 14;
    case 26:
        return 23;
    case 33:
        return 30;
    default:
        return 0;
    }
}

// GetCoordinatesFromTaskData: resolves a constraint endpoint to the evaluated position of a node of the character whose scenario id
// matches, falling back to this character's own root.
void MOTN_ResolveConstraintEndpoint(MotionInformationWork &motion, MotionConstraintEndpoint &endpoint) {
    MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const scenario = control && control->scenarioTask ? reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr) : nullptr;
    constexpr std::uint32_t kCharacterEntity = SHEN_NAME(CEWP);
    Character *selected = nullptr;
    if (control && scenario) {
        for (void *const bucketHead : control->characterBuckets) {
            for (auto *link = static_cast<CharacterBucketNode *>(bucketHead); link && !selected; link = link->next) {
                Character *const character = link->character;
                if (!character || character->entity.name != kCharacterEntity ||
                    ((character->flags.value & 1U) == 0U && ((character->flags.value & 2U) == 0U || (character->motionFlags & 1U) == 0U))) {
                    continue;
                }
                SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
                const CharacterIdLookupNode *node = scenario->characterBuckets[reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size()];
                while (node && node->character != character)
                    node = node->bucketNext;
                if ((node ? node->id : 0U) == static_cast<std::uint32_t>(endpoint.sourceId))
                    selected = character;
            }
            if (selected)
                break;
        }
    }
    MotionNodeRuntime *motionNode = nullptr;
    if (selected) {
        MotionManagerWork *const manager = MOTM_GetWork(selected);
        if (!manager)
            return;
        motionNode = FindTrackedNode(static_cast<MotionNodeRuntime *>(manager->motion.previousNodes), static_cast<std::uint32_t>(endpoint.nodeId));
    } else {
        motionNode = FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.previousNodes), 0);
    }
    if (!motionNode)
        return;
    endpoint.position = {motionNode->transform->rows[3].x, motionNode->transform->rows[3].y, motionNode->transform->rows[3].z};
}

// CalculateMidPoint: projects point onto the line through pointA and pointB; W receives the line parameter.
void CalculateMidPoint(const Vector3 &pointA, const Vector3 &pointB, const Vector3 &point, Vector4 &output) {
    const float lineX = pointB.x - pointA.x;
    const float pointX = point.x - pointA.x;
    const float lineY = pointB.y - pointA.y;
    const float pointY = point.y - pointA.y;
    const float lineZ = pointB.z - pointA.z;
    const float pointZ = point.z - pointA.z;
    if (std::fabs(lineX) < 0x1p-23f && std::fabs(lineY) < 0x1p-23f && std::fabs(lineZ) < 0x1p-23f && std::fabs(pointX) < 0x1p-23f &&
        std::fabs(pointY) < 0x1p-23f && std::fabs(pointZ) < 0x1p-23f) {
        output = {pointA.x, pointA.y, pointA.z, 0.0f};
    }
    const float lineLength = std::sqrt((lineY * lineY + lineX * lineX) + lineZ * lineZ);
    const float pointLength = std::sqrt((pointY * pointY + pointX * pointX) + pointZ * pointZ);
    const float parameter = ((((pointY * lineY + pointX * lineX) + pointZ * lineZ) / (pointLength * lineLength)) * pointLength) / lineLength;
    output.w = parameter;
    output.x = lineX * parameter + pointA.x;
    output.y = lineY * parameter + pointA.y;
    output.z = lineZ * parameter + pointA.z;
}

// CalculateMidPointBetweenVectors: CalculateMidPoint clamped to the segment.
void CalculateMidPointBetweenVectors(const Vector3 &pointA, const Vector3 &pointB, const Vector3 &point, Vector4 &output) {
    const float lineX = pointB.x - pointA.x;
    const float lineY = pointB.y - pointA.y;
    const float lineZ = pointB.z - pointA.z;
    CalculateMidPoint(pointA, pointB, point, output);
    if (output.w <= 0.0f) {
        output = {pointA.x, pointA.y, pointA.z, 0.0f};
    } else if (output.w < 1.0f) {
        output.x = lineX * output.w + pointA.x;
        output.y = lineY * output.w + pointA.y;
        output.z = lineZ * output.w + pointA.z;
    } else {
        output = {pointA.x + lineX, pointA.y + lineY, pointA.z + lineZ, 1.0f};
    }
}

// CalculateMidPointAndCompareDistances (MOTN_SolveNodeConstraints): keeps each constrained row outside the capsule between the two
// endpoints, recording the correction MOTN_ApplyNodeConstraintCorrections later removes.
void MOTN_SolveNodeConstraints(MotionInformationWork &motion) {
    auto *const constraints = static_cast<MotionNodeConstraintWorkspace *>(motion.nodeConstraints);
    if (!constraints)
        return;
    MOTN_ResolveConstraintEndpoint(motion, constraints->endpoints[0]);
    MOTN_ResolveConstraintEndpoint(motion, constraints->endpoints[1]);
    Vector3 origin{};
    bool anyInside = false;
    for (MotionNodeConstraintState &state : constraints->states) {
        auto *const motionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
        MotionNodeRuntime *const motionNode = FindTrackedNode(motionNodes, static_cast<std::uint32_t>(state.nodeId));
        if (!motionNode)
            continue;
        const Vector3 effector{motionNode->effectorPosition.x, motionNode->effectorPosition.y, motionNode->effectorPosition.z};
        Vector4 nearest{};
        CalculateMidPointBetweenVectors(constraints->endpoints[0].position, constraints->endpoints[1].position, effector, nearest);
        const std::uint32_t contained = MOTN_SelectContainingConstraintEndpoint(*constraints, nearest);
        const float radius = contained == 255 ?
            (constraints->endpoints[1].radius - constraints->endpoints[0].radius) * nearest.w + constraints->endpoints[0].radius :
            constraints->endpoints[contained].radius;
        const Vector3 center{nearest.x, nearest.y, nearest.z};
        const float offsetX = effector.x - center.x;
        const float offsetY = effector.y - center.y;
        const float offsetZ = effector.z - center.z;
        const float reach = radius + state.radiusBias;
        const float reachSquared = reach * reach;
        const float distanceSquared = (offsetY * offsetY + offsetX * offsetX) + offsetZ * offsetZ;
        bool inside = false;
        if (reachSquared <= distanceSquared) {
            state.flags &= ~1U;
            inside = anyInside;
        } else {
            Vector3 anchor{};
            bool pushFromAnchor = true;
            switch (state.mode) {
            case 0: {
                const float scale = std::sqrt(reachSquared) / std::sqrt(distanceSquared);
                const Vector3 surface{center.x + offsetX * scale, center.y + offsetY * scale, center.z + offsetZ * scale};
                state.correction = {effector.x - surface.x, motionNode->effectorPosition.y - surface.y, motionNode->effectorPosition.z - surface.z};
                pushFromAnchor = false;
                break;
            }
            case 1: {
                const std::uint32_t parentId = MOTN_ConstraintParentNode(static_cast<std::uint32_t>(state.nodeId));
                anchor = origin;
                if (MotionNodeRuntime *const parent = FindTrackedNode(motionNodes, parentId)) {
                    anchor = {parent->transform->rows[3].x, parent->transform->rows[3].y, parent->transform->rows[3].z};
                    origin = anchor;
                }
                break;
            }
            case 2:
                if ((state.flags & 1U) == 0)
                    state.persistentOffset = {effector.x - state.previousEffectorPosition.x, motionNode->effectorPosition.y - state.previousEffectorPosition.y,
                        motionNode->effectorPosition.z - state.previousEffectorPosition.z};
                anchor = {motionNode->effectorPosition.x - state.persistentOffset.x, motionNode->effectorPosition.y - state.persistentOffset.y,
                    motionNode->effectorPosition.z - state.persistentOffset.z};
                origin = anchor;
                break;
            case 3:
            case 4: {
                anchor = {effector.x, effector.y + (state.mode == 3 ? 0.2f : -0.2f), effector.z};
                origin = anchor;
                Vector4 foot{};
                CalculateMidPoint(anchor, effector, center, foot);
                const float along = std::sqrt(reachSquared -
                    (((center.y - foot.y) * (center.y - foot.y) + (center.x - foot.x) * (center.x - foot.x)) + (center.z - foot.z) * (center.z - foot.z)));
                const float axisX = effector.x - foot.x;
                const float axisY = anchor.y - foot.y;
                const float axisZ = effector.z - foot.z;
                const float length = std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
                state.correction.x = 0.0f;
                state.correction.y = motionNode->effectorPosition.y - ((along / length) * axisY + foot.y);
                state.correction.z = 0.0f;
                pushFromAnchor = false;
                break;
            }
            case 5:
                anchor = g_CameraEyePos;
                origin = anchor;
                break;
            default:
                pushFromAnchor = false;
                break;
            }
            if (pushFromAnchor) {
                Vector4 foot{};
                CalculateMidPoint(anchor, effector, center, foot);
                const float axisX = anchor.x - foot.x;
                const float axisY = anchor.y - foot.y;
                const float axisZ = anchor.z - foot.z;
                const float along = std::sqrt(reachSquared -
                    (((center.y - foot.y) * (center.y - foot.y) + (center.x - foot.x) * (center.x - foot.x)) + (center.z - foot.z) * (center.z - foot.z)));
                const float scale = along / std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
                const float surfaceX = axisX * scale + foot.x;
                const float surfaceZ = axisZ * scale + foot.z;
                state.correction.x = effector.x - surfaceX;
                state.correction.y = motionNode->effectorPosition.y - (axisY * scale + foot.y);
                state.correction.z = motionNode->effectorPosition.z - surfaceZ;
            }
            state.flags |= 1U;
            inside = true;
            anyInside = true;
        }
        state.previousEffectorPosition = {motionNode->effectorPosition.x, motionNode->effectorPosition.y, motionNode->effectorPosition.z};
        if (inside) {
            constraints->endpoints[0].flags |= 1U;
            constraints->endpoints[1].flags |= 1U;
        } else {
            constraints->endpoints[0].flags &= ~1U;
            constraints->endpoints[1].flags &= ~1U;
        }
    }
}

// InterpolateJointPosition_CubicPolynomial: the cubic-sequence position of a target row between the previous pose and the new one,
// using the tangents Shenmue_CubicInterp published into the VMPT scratch.
void InterpolateJointPosition_CubicPolynomial(Vector3 &result, float amount, const MotionNodeRuntime &start, const MotionNodeRuntime &end) {
    if (amount < 0x1p-23f) {
        result = {start.effectorPosition.x, start.effectorPosition.y, start.effectorPosition.z};
        return;
    }
    if (1.0f - amount < 0x1p-23f) {
        result = {end.effectorPosition.x, end.effectorPosition.y, end.effectorPosition.z};
        return;
    }
    if (g_motnArmBlendMode == 2) {
        if (start.nodeId == 11) {
            result = g_motnLeftAnkleCubic;
            return;
        }
        if (start.nodeId == 17) {
            result = g_motnRightAnkleCubic;
            return;
        }
    }
    const float s = amount * 0.33333334f - 0.0f;
    if (!start.translationKeys || !end.translationKeys) {
        const float cubic = s * s * s * 9.0f;
        const float cubic6 = static_cast<float>((static_cast<double>(cubic) + static_cast<double>(cubic)) * 3.0);
        const float square27 = static_cast<float>(static_cast<double>(s * s) * 3.0 * 9.0);
        const float square3 = s * s * 3.0f;
        result.x =
            ((((cubic6 - square27) + 1.0f) * start.bindPosition->x + (square27 - cubic6) * end.bindPosition->x) + (((cubic - square3) - square3) + s) * 0.0f) +
            (cubic - square3) * 0.0f;
        result.y =
            ((((cubic6 - square27) + 1.0f) * start.bindPosition->y + (square27 - cubic6) * end.bindPosition->y) + (((cubic - square3) - square3) + s) * 0.0f) +
            (cubic - square3) * 0.0f;
        result.z =
            ((((cubic6 - square27) + 1.0f) * start.bindPosition->z + (square27 - cubic6) * end.bindPosition->z) + (((cubic - square3) - square3) + s) * 0.0f) +
            (cubic - square3) * 0.0f;
        return;
    }
    std::uint32_t step = 0;
    switch (start.nodeId) {
    case 3:
        step = 1;
        break;
    case 6:
        step = 2;
        break;
    case 11:
        step = 3;
        break;
    case 17:
        step = 4;
        break;
    case 21:
        step = 5;
        break;
    case 26:
        step = 6;
        break;
    case 33:
        step = 7;
        break;
    case 38:
        step = 8;
        break;
    case 42:
        step = 9;
        break;
    default:
        break;
    }
    MotionCubicSequenceScratch &scratch = *g_motnCubicScratch;
    Vector3 from{start.effectorPosition.x, start.effectorPosition.y, start.effectorPosition.z};
    Vector3 to{end.effectorPosition.x, end.effectorPosition.y, end.effectorPosition.z};
    switch (start.nodeId) {
    case 17:
        from = scratch.startAnchor[0];
        to = scratch.endAnchor[0];
        break;
    case 11:
        from = scratch.startAnchor[1];
        to = scratch.endAnchor[1];
        break;
    case 33:
        from = scratch.startAnchor[2];
        to = scratch.endAnchor[2];
        break;
    case 26:
        from = scratch.startAnchor[3];
        to = scratch.endAnchor[3];
        break;
    case 6:
        from = scratch.startAnchor[4];
        to = scratch.endAnchor[4];
        break;
    default:
        break;
    }
    const float square3 = s * s * 3.0f;
    const float cubic = s * s * s * 9.0f;
    const float cubic6 = static_cast<float>((static_cast<double>(cubic) + static_cast<double>(cubic)) * 3.0);
    const float square27 = static_cast<float>(static_cast<double>(s * s) * 3.0 * 9.0);
    result.x = ((((cubic6 - square27) + 1.0f) * from.x + (square27 - cubic6) * to.x) + (((cubic - square3) - square3) + s) * scratch.startTangent[step].x) +
        (cubic - square3) * scratch.endTangent[step].x;
    result.y = ((((cubic6 - square27) + 1.0f) * from.y + (square27 - cubic6) * to.y) + (((cubic - square3) - square3) + s) * scratch.startTangent[step].y) +
        (cubic - square3) * scratch.endTangent[step].y;
    result.z = ((((cubic6 - square27) + 1.0f) * from.z + (square27 - cubic6) * to.z) + (((cubic - square3) - square3) + s) * scratch.startTangent[step].z) +
        (cubic - square3) * scratch.endTangent[step].z;
    auto *const activeRoot = static_cast<MotionNodeRuntime *>(g_pMotion->activeNodes);
    switch (start.nodeId) {
    case 0:
        result.y = g_motnSequenceRootLift + result.y;
        return;
    case 6:
    case 11:
    case 17:
    case 26:
    case 33:
        Matrix4x4_Push();
        workTransformMatrix = *MotionNodeMatrix(*activeRoot);
        Matrix4x4::preTranslate(result.x, result.y, result.z);
        result = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        break;
    default:
        return;
    }
    if (start.nodeId == 26 || start.nodeId == 33) {
        const bool leadHand = g_cubicSequenceDirection ? start.nodeId == 33 : start.nodeId == 26;
        result.y = (leadHand ? g_motnSequenceLeadHandLift : g_motnSequenceTrailHandLift) + result.y;
    }
}

// ApplyArmAimOffsetWithDamping: pushes an ankle effector out of the nearest contact sphere, carrying the change of push between
// spheres as a decaying damping offset.
void ApplyArmAimOffsetWithDamping(
    MotionNodeRuntime &ankle, const MotionContactTarget &target, Vector3 &current, Vector3 &damping, bool applyOffsets, float factor) {
    const float dx = ankle.effectorPosition.x - target.position.x;
    const float dy = ankle.effectorPosition.y - target.position.y;
    const float dz = ankle.effectorPosition.z - target.position.z;
    const float distance = std::sqrt((dy * dy + dx * dx) + dz * dz);
    const float reach = g_motnContactRadiusBias + target.radius;
    float scale = 0.0f;
    if (distance >= 0x1p-23f)
        scale = reach * 0.5f <= distance ? (reach * reach) / ((distance * 4.0f) * distance) : (reach - distance) / distance;
    const Vector3 push{scale * dx, scale * dy, scale * dz};
    if (applyOffsets) {
        damping.x = (current.x - push.x) + damping.x;
        damping.y = (current.y - push.y) + damping.y;
        damping.z = (current.z - push.z) + damping.z;
    }
    damping.x = std::fabs(damping.x) <= 0.001f ? 0.0f : static_cast<float>(static_cast<double>(damping.x) * 0.9);
    damping.y = std::fabs(damping.y) <= 0.001f ? 0.0f : static_cast<float>(static_cast<double>(damping.y) * 0.9);
    damping.z = std::fabs(damping.z) <= 0.001f ? 0.0f : static_cast<float>(static_cast<double>(damping.z) * 0.9);
    ankle.effectorPosition.x = (push.x + damping.x) * factor + ankle.effectorPosition.x;
    ankle.effectorPosition.y = (push.y + damping.y) * factor + ankle.effectorPosition.y;
    ankle.effectorPosition.z = (push.z + damping.z) * factor + ankle.effectorPosition.z;
    current = push;
}

// ProjectToMaxDistance: keeps an ankle effector within the leg length measured before the contact push.
void ProjectToMaxDistance(MotionNodeRuntime &ankle, const MotionNodeRuntime &thigh, float maximum) {
    const float dx = thigh.transform->rows[3].x - ankle.effectorPosition.x;
    const float dy = thigh.transform->rows[3].y - ankle.effectorPosition.y;
    const float dz = thigh.transform->rows[3].z - ankle.effectorPosition.z;
    const float distance = std::sqrt((dy * dy + dx * dx) + dz * dz);
    if (distance > maximum && distance > 0x1p-23f) {
        const float scale = (distance - maximum) / distance;
        ankle.effectorPosition.x = scale * dx + ankle.effectorPosition.x;
        ankle.effectorPosition.y = scale * dy + ankle.effectorPosition.y;
        ankle.effectorPosition.z = scale * dz + ankle.effectorPosition.z;
    }
}

MotionNodeRuntime *FindMotionNodeFrom(MotionNodeRuntime *motionNode, std::uint8_t nodeId) {
    if (motionNode->nodeId == nodeId)
        return motionNode;
    while (motionNode->nodeId != 0xff) {
        ++motionNode;
        if (motionNode->nodeId == nodeId)
            return motionNode;
    }
    return nullptr;
}

// ApplyArmTargetsAndLengthClamp_MOTN: blends the ankle effectors from the source pose, caches them, and (outside the scripted update
// modes, for non-Ryo characters with contact IK) pushes them out of the frame's contact spheres.
void ApplyArmTargetsAndLengthClamp_MOTN(MotionNodeRuntime *sourceMotionNodes, MotionNodeRuntime *activeMotionNodes) {
    MotionInformationWork &motion = *g_pMotion;
    g_motnRightAnkleNode = nullptr;
    g_motnLeftAnkleNode = nullptr;
    g_motnRightAnkleSourceNode = nullptr;
    g_motnLeftAnkleSourceNode = nullptr;
    const bool cubicMode = ((motion.interpolationMode - 7U) & 0xfbU) == 0;
    if (cubicMode && !g_motnArmBlendMode) {
        g_motnArmBlendMode = 1;
        return;
    }
    const float contactBlend = motion.contactIKBlend;
    motion.contactIKBlend = 0.0f;
    if (!activeMotionNodes)
        return;
    MotionNodeRuntime *const rightThigh = FindMotionNodeFrom(activeMotionNodes, 14);
    if (!rightThigh)
        return;
    MotionNodeRuntime *const leftThigh = FindMotionNodeFrom(activeMotionNodes, 8);
    if (!leftThigh)
        return;
    MotionNodeRuntime *const rightAnkle = FindMotionNodeFrom(rightThigh, 17);
    g_motnRightAnkleNode = rightAnkle;
    if (!rightAnkle)
        return;
    MotionNodeRuntime *const leftAnkle = FindMotionNodeFrom(leftThigh, 11);
    g_motnLeftAnkleNode = leftAnkle;
    if (!leftAnkle)
        return;
    g_motnRightAnkleSampled = {rightAnkle->effectorPosition.x, rightAnkle->effectorPosition.y, rightAnkle->effectorPosition.z};
    g_motnLeftAnkleSampled = {leftAnkle->effectorPosition.x, leftAnkle->effectorPosition.y, leftAnkle->effectorPosition.z};
    MotionNodeRuntime *sourceRight = nullptr;
    MotionNodeRuntime *sourceLeft = nullptr;
    if (sourceMotionNodes) {
        sourceRight = FindMotionNodeFrom(sourceMotionNodes, 17);
        g_motnRightAnkleSourceNode = sourceRight;
        if (!sourceRight)
            return;
        sourceLeft = FindMotionNodeFrom(sourceMotionNodes, 11);
        g_motnLeftAnkleSourceNode = sourceLeft;
        if (!sourceLeft)
            return;
        if (g_motnIgnoreArmIK) {
            sourceRight->effectorPosition.x = motion.cachedAnklePosition[1].x;
            sourceRight->effectorPosition.y = motion.cachedAnklePosition[1].y;
            sourceRight->effectorPosition.z = motion.cachedAnklePosition[1].z;
            sourceLeft->effectorPosition.x = motion.cachedAnklePosition[0].x;
            sourceLeft->effectorPosition.y = motion.cachedAnklePosition[0].y;
            sourceLeft->effectorPosition.z = motion.cachedAnklePosition[0].z;
        }
        g_motnRightAnkleSourceSampled = {sourceRight->effectorPosition.x, sourceRight->effectorPosition.y, sourceRight->effectorPosition.z};
        g_motnLeftAnkleSourceSampled = {sourceLeft->effectorPosition.x, sourceLeft->effectorPosition.y, sourceLeft->effectorPosition.z};
        if (!cubicMode) {
            float time = sourceRight->interpolationAmount;
            float inverse = 1.0f - time;
            rightAnkle->effectorPosition.x = sourceRight->effectorPosition.x * inverse + time * rightAnkle->effectorPosition.x;
            rightAnkle->effectorPosition.y = inverse * sourceRight->effectorPosition.y + time * rightAnkle->effectorPosition.y;
            rightAnkle->effectorPosition.z = inverse * sourceRight->effectorPosition.z + time * rightAnkle->effectorPosition.z;
            time = sourceLeft->interpolationAmount;
            inverse = 1.0f - time;
            leftAnkle->effectorPosition.x = inverse * sourceLeft->effectorPosition.x + time * leftAnkle->effectorPosition.x;
            leftAnkle->effectorPosition.y = inverse * sourceLeft->effectorPosition.y + time * leftAnkle->effectorPosition.y;
            leftAnkle->effectorPosition.z = inverse * sourceLeft->effectorPosition.z + time * leftAnkle->effectorPosition.z;
        } else {
            InterpolateJointPosition_CubicPolynomial(g_motnRightAnkleCubic, sourceRight->interpolationAmount, *sourceRight, *rightAnkle);
            rightAnkle->effectorPosition.x = g_motnRightAnkleCubic.x;
            rightAnkle->effectorPosition.y = g_motnRightAnkleCubic.y;
            rightAnkle->effectorPosition.z = g_motnRightAnkleCubic.z;
            InterpolateJointPosition_CubicPolynomial(g_motnLeftAnkleCubic, sourceLeft->interpolationAmount, *sourceLeft, *leftAnkle);
            leftAnkle->effectorPosition.x = g_motnLeftAnkleCubic.x;
            leftAnkle->effectorPosition.y = g_motnLeftAnkleCubic.y;
            leftAnkle->effectorPosition.z = g_motnLeftAnkleCubic.z;
        }
    }
    motion.cachedAnklePosition[1] = {rightAnkle->effectorPosition.x, rightAnkle->effectorPosition.y, rightAnkle->effectorPosition.z};
    motion.cachedAnklePosition[0] = {leftAnkle->effectorPosition.x, leftAnkle->effectorPosition.y, leftAnkle->effectorPosition.z};
    if (g_currentMotionUpdateMode != 0 || g_motnIsRyo == 1 || motion.updateMode || (motion.rootAngleFlags & 0x800U) == 0)
        return;
    auto *const targets = reinterpret_cast<MotionContactTargetList *>(static_cast<std::uintptr_t>(motion.frameContactOverlay));
    if (!targets || !targets->entries)
        return;
    motion.contactIKBlend = contactBlend;
    if (!motion.contactIKEnabled && contactBlend < 0x1p-23f)
        return;
    const Matrix4x4 &rightThighMatrix = *rightThigh->transform;
    const float rightX = rightThighMatrix.rows[3].x - rightAnkle->effectorPosition.x;
    const float rightY = rightThighMatrix.rows[3].y - rightAnkle->effectorPosition.y;
    const float rightZ = rightThighMatrix.rows[3].z - rightAnkle->effectorPosition.z;
    const float rightLength = std::sqrt((rightY * rightY + rightX * rightX) + rightZ * rightZ);
    const Matrix4x4 &leftThighMatrix = *leftThigh->transform;
    const float leftX = leftThighMatrix.rows[3].x - leftAnkle->effectorPosition.x;
    const float leftY = leftThighMatrix.rows[3].y - leftAnkle->effectorPosition.y;
    const float leftZ = leftThighMatrix.rows[3].z - leftAnkle->effectorPosition.z;
    const float leftLength = std::sqrt((leftY * leftY + leftX * leftX) + leftZ * leftZ);
    if (g_motnCubicScratch) {
        for (std::int32_t index = 0; index < targets->count; ++index) {
            MotionContactTarget &target = targets->entries[index];
            if (static_cast<std::uint8_t>(target.type - 7U) > 11U) {
                target.position.x -= motion.appliedPositionOffset.x;
                target.position.y -= motion.appliedPositionOffset.y;
                target.position.z -= motion.appliedPositionOffset.z;
            }
        }
    }
    g_motnContactRadiusBias = motion.contactTargetRadiusBias;
    std::int32_t rightIndex = 0;
    std::int32_t leftIndex = 0;
    float rightBest = 10000.0f;
    float leftBest = 10000.0f;
    for (std::int32_t index = 0; index < targets->count; ++index) {
        const MotionContactTarget &target = targets->entries[index];
        if (static_cast<std::uint8_t>(target.type - 7U) <= 11U)
            continue;
        float distance = (rightAnkle->effectorPosition.y - target.position.y) * (rightAnkle->effectorPosition.y - target.position.y);
        if (rightBest > distance) {
            distance += (rightAnkle->effectorPosition.x - target.position.x) * (rightAnkle->effectorPosition.x - target.position.x);
            if (rightBest > distance) {
                distance += (rightAnkle->effectorPosition.z - target.position.z) * (rightAnkle->effectorPosition.z - target.position.z);
                if (rightBest > distance) {
                    rightBest = distance;
                    rightIndex = index;
                }
            }
        }
        distance = (leftAnkle->effectorPosition.y - target.position.y) * (leftAnkle->effectorPosition.y - target.position.y);
        if (leftBest > distance) {
            distance += (leftAnkle->effectorPosition.x - target.position.x) * (leftAnkle->effectorPosition.x - target.position.x);
            if (leftBest > distance) {
                distance += (leftAnkle->effectorPosition.z - target.position.z) * (leftAnkle->effectorPosition.z - target.position.z);
                if (leftBest > distance) {
                    leftBest = distance;
                    leftIndex = index;
                }
            }
        }
    }
    if (motion.contactIKEnabled) {
        if (motion.contactIKBlend < 1.0f)
            motion.contactIKBlend = motion.contactIKBlend + 0.1f;
    } else {
        motion.contactIKBlend = motion.contactIKBlend - 0.1f;
    }
    if (motion.contactIKBlend > 1.0f)
        motion.contactIKBlend = 1.0f;
    else if (motion.contactIKBlend < 0.0f)
        motion.contactIKBlend = 0.0f;
    const std::int32_t previousRight = motion.previousFootContactTargetIndex[1];
    ApplyArmAimOffsetWithDamping(*rightAnkle, targets->entries[rightIndex], motion.footTargetCurrent[1], motion.footTargetDamping[1],
        rightIndex != previousRight && previousRight != -1, motion.contactIKBlend);
    motion.previousFootContactTargetIndex[1] = rightIndex;
    const std::int32_t previousLeft = motion.previousFootContactTargetIndex[0];
    ApplyArmAimOffsetWithDamping(*leftAnkle, targets->entries[leftIndex], motion.footTargetCurrent[0], motion.footTargetDamping[0],
        leftIndex != previousLeft && previousLeft != -1, motion.contactIKBlend);
    motion.previousFootContactTargetIndex[0] = leftIndex;
    ProjectToMaxDistance(*rightAnkle, *rightThigh, rightLength);
    ProjectToMaxDistance(*leftAnkle, *leftThigh, leftLength);
    if (sourceMotionNodes) {
        sourceRight->effectorPosition.x = rightAnkle->effectorPosition.x;
        sourceRight->effectorPosition.y = rightAnkle->effectorPosition.y;
        sourceRight->effectorPosition.z = rightAnkle->effectorPosition.z;
        sourceLeft->effectorPosition.x = leftAnkle->effectorPosition.x;
        sourceLeft->effectorPosition.y = leftAnkle->effectorPosition.y;
        sourceLeft->effectorPosition.z = leftAnkle->effectorPosition.z;
    }
    if (cubicMode) {
        g_motnRightAnkleCubic = {rightAnkle->effectorPosition.x, rightAnkle->effectorPosition.y, rightAnkle->effectorPosition.z};
        g_motnLeftAnkleCubic = {leftAnkle->effectorPosition.x, leftAnkle->effectorPosition.y, leftAnkle->effectorPosition.z};
        g_motnArmBlendMode = 2;
    }
}

// MOTN_SetNewMatrices: after the skeleton is built, restores the ankle effectors to their sampled (pre-contact, pre-blend) positions.
void MOTN_SetNewMatrices(MotionNodeRuntime *sourceMotionNodes) {
    if (!g_motnRightAnkleNode || !g_motnLeftAnkleNode)
        return;
    g_motnRightAnkleNode->effectorPosition.x = g_motnRightAnkleSampled.x;
    g_motnRightAnkleNode->effectorPosition.y = g_motnRightAnkleSampled.y;
    g_motnRightAnkleNode->effectorPosition.z = g_motnRightAnkleSampled.z;
    g_motnLeftAnkleNode->effectorPosition.x = g_motnLeftAnkleSampled.x;
    g_motnLeftAnkleNode->effectorPosition.z = g_motnLeftAnkleSampled.z;
    g_motnLeftAnkleNode->effectorPosition.y = g_motnLeftAnkleSampled.y;
    if (sourceMotionNodes) {
        if (!g_motnRightAnkleSourceNode || !g_motnLeftAnkleSourceNode)
            return;
        g_motnRightAnkleSourceNode->effectorPosition.x = g_motnRightAnkleSourceSampled.x;
        g_motnRightAnkleSourceNode->effectorPosition.y = g_motnRightAnkleSourceSampled.y;
        g_motnRightAnkleSourceNode->effectorPosition.z = g_motnRightAnkleSourceSampled.z;
        g_motnLeftAnkleSourceNode->effectorPosition.x = g_motnLeftAnkleSourceSampled.x;
        g_motnLeftAnkleSourceNode->effectorPosition.z = g_motnLeftAnkleSourceSampled.z;
        g_motnLeftAnkleSourceNode->effectorPosition.y = g_motnLeftAnkleSourceSampled.y;
    }
    g_pMotion->appliedPositionOffset = {};
}

float LerpDouble(float a, float b, float t) {
    return static_cast<float>(static_cast<double>(a) * (1.0 - static_cast<double>(t)) + static_cast<double>(t * b));
}

Vector3 WorkLocalPoint(const Vector3 &point) {
    const Matrix4x4 &m = workTransformMatrix;
    const float dx = point.x - m.rows[3].x;
    const float dy = point.y - m.rows[3].y;
    const float dz = point.z - m.rows[3].z;
    return {(m.rows[0].y * dy + m.rows[0].x * dx) + m.rows[0].z * dz, (m.rows[1].y * dy + m.rows[1].x * dx) + m.rows[1].z * dz,
        (m.rows[2].y * dy + m.rows[2].x * dx) + m.rows[2].z * dz};
}

Vector3 PositionOf(const Vector4 &value) {
    return {value.x, value.y, value.z};
}

void SetPosition(Vector4 &target, const Vector3 &value) {
    target.x = value.x;
    target.y = value.y;
    target.z = value.z;
}

// Matrix4x4_Lerp: blends the X column and Y column of two rotations and re-orthonormalises; returns false when degenerate.
bool Matrix4x4_Lerp(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end) {
    const double inverse = 1.0 - static_cast<double>(t);
    const float ax = static_cast<float>(static_cast<double>(t * end.rows[0].x) + static_cast<double>(start.rows[0].x) * inverse);
    output.rows[0].x = ax;
    const float ay = static_cast<float>(static_cast<double>(start.rows[1].x) * inverse + static_cast<double>(t * end.rows[1].x));
    output.rows[1].x = ay;
    const float az = static_cast<float>(static_cast<double>(start.rows[2].x) * inverse + static_cast<double>(t * end.rows[2].x));
    output.rows[2].x = az;
    const float length = std::sqrt((ay * ay + ax * ax) + az * az);
    if (std::fabs(length) > 0x1p-23f) {
        output.rows[0].x = ax / length;
        output.rows[1].x = ay / length;
        output.rows[2].x = az / length;
    }
    const float yx = static_cast<float>(static_cast<double>(start.rows[0].y) * inverse + static_cast<double>(t * end.rows[0].y));
    const float yy = static_cast<float>(static_cast<double>(start.rows[1].y) * inverse + static_cast<double>(t * end.rows[1].y));
    const float yz = static_cast<float>(static_cast<double>(start.rows[2].y) * inverse + static_cast<double>(t * end.rows[2].y));
    const float crossY = yx * az - yz * ax;
    const float crossX = yz * ay - yy * az;
    const float crossZ = yy * ax - yx * ay;
    const float normalX = crossY * az - crossZ * ay;
    const float normalY = crossZ * ax - crossX * az;
    const float normalZ = crossX * ay - crossY * ax;
    const float normalLength = std::sqrt((normalY * normalY + normalX * normalX) + normalZ * normalZ);
    if (std::fabs(normalLength) <= 0x1p-23f)
        return false;
    output.rows[0].y = normalX / normalLength;
    output.rows[1].y = normalY / normalLength;
    output.rows[2].y = normalZ / normalLength;
    const float crossLength = std::sqrt((crossY * crossY + crossX * crossX) + crossZ * crossZ);
    if (std::fabs(crossLength) <= 0x1p-23f)
        return false;
    output.rows[2].w = 0.0f;
    output.rows[3].x = 0.0f;
    output.rows[1].w = 0.0f;
    output.rows[0].w = 0.0f;
    output.rows[3].y = 0.0f;
    output.rows[3].z = 0.0f;
    output.rows[3].w = 1.0f;
    output.rows[0].z = crossX / crossLength;
    output.rows[1].z = crossY / crossLength;
    output.rows[2].z = crossZ / crossLength;
    return true;
}

void MultiplyLerpedRotation(float t) {
    Matrix4x4 blended{};
    Matrix4x4::Multiply(Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
}

// VectorTransformWithRotationLerp: translates by the blended effector and rotates by the blended pair of euler rotations; with
// MOTN::NeedsRotation the entity angles of each pose are folded in and the translation restarts from an unrotated basis.
void VectorTransformWithRotationLerp(float t, const MotionNodeRuntime &source, const MotionNodeRuntime &target) {
    Matrix4x4::preTranslate(LerpDouble(source.effectorPosition.x, target.effectorPosition.x, t),
        LerpDouble(source.effectorPosition.y, target.effectorPosition.y, t), LerpDouble(source.effectorPosition.z, target.effectorPosition.z, t));
    if (g_motnNeedsRotation) {
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
    }
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    if (g_motnNeedsRotation && g_motnNextEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(source.bindEuler.y, source.bindEuler.x, source.bindEuler.z);
    g_motnBlendStart = workTransformMatrix;
    workTransformMatrix = transformMatrix_prePass;
    if (g_motnNeedsRotation && g_motnEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(target.bindEuler.y, target.bindEuler.x, target.bindEuler.z);
    g_motnBlendEnd = workTransformMatrix;
    Matrix4x4::Pop(1);
    MultiplyLerpedRotation(t);
    g_motnNeedsRotation = 0;
}

// TranslateVectorByConditions: moves an ankle target toward its world target, capped at 99% of the leg reach measured from the
// previous frame's knee, and eased in over the slow-ramp blend frames.
void TranslateVectorByConditions(const Vector3 &world, float *target, std::int32_t &blendFrames) {
    if (g_currentMotionUpdateMode) {
        blendFrames = 8;
        target[0] = world.x;
        target[1] = world.y;
        target[2] = world.z;
        return;
    }
    float x = world.x;
    float y = world.y;
    float z = world.z;
    if (g_motnAnkleKneeValid) {
        const float dx = g_motnAnkleKnee->x - x;
        const float dy = g_motnAnkleKnee->y - y;
        const float dz = g_motnAnkleKnee->z - z;
        const float distance = std::sqrt((dy * dy + dx * dx) + dz * dz);
        g_motnAnkleReach = static_cast<float>(static_cast<double>(g_motnAnkleReach) * 0.99);
        if (distance > g_motnAnkleReach) {
            const float scale = (distance - g_motnAnkleReach) / distance;
            x = x + dx * scale;
            y = y + dy * scale;
            z = z + dz * scale;
        }
    }
    if ((g_pMotion->rootAngleFlags & 0x1000000U) != 0) {
        const float ramp = std::clamp(static_cast<float>(blendFrames) * 0.125f, 0.0f, 1.0f);
        const float yBlend = (1.0f - ramp) * target[1];
        const float zBlend = (1.0f - ramp) * target[2] + ramp * z;
        target[0] = (1.0f - ramp) * target[0] + ramp * x;
        target[2] = zBlend;
        target[1] = yBlend + ramp * y;
    } else {
        target[0] = x;
        target[1] = y;
        target[2] = z;
    }
}

// BlendPositionWithGlobalTarget: while the slow ramp releases an ankle target, eases toward the previous frame's ankle.
void BlendPositionWithGlobalTarget(float *target, std::int32_t &blendFrames) {
    if (g_currentMotionUpdateMode) {
        blendFrames = 0;
        return;
    }
    if (!g_motnAnkleReleaseValid || (g_pMotion->rootAngleFlags & 0x1000000U) == 0)
        return;
    const Vector3 release = *g_motnAnkleRelease;
    const float ramp = std::clamp(static_cast<float>(blendFrames) * 0.125f, 0.0f, 1.0f);
    const float yBlend = (1.0f - ramp) * target[1];
    const float zBlend = (1.0f - ramp) * target[2] + ramp * release.z;
    target[0] = (1.0f - ramp) * target[0] + ramp * release.x;
    target[2] = zBlend;
    target[1] = yBlend + ramp * release.y;
}

// The ankle-target preparation shared by MOTN_BuildBoneTransformFromData, MOTN_RotateAndTranslate and the IK-chain lerp.
void ApplyAnkleTarget(std::uint8_t ankleId, float reach, float *target) {
    if (ankleId == 17) {
        if (const Vector3 *const world = g_pMotion->worldTargets[4]) {
            g_motnAnkleKnee = &g_motnRightKnee;
            g_motnAnkleKneeValid = g_motnRightKneeValid;
            g_motnAnkleReach = reach;
            TranslateVectorByConditions(*world, target, g_pMotion->rightAnkleTargetBlendFrames);
        } else if (g_pMotion->rightAnkleTargetBlendFrames) {
            g_motnAnkleRelease = &g_motnRightAnkle;
            g_motnAnkleReleaseValid = g_motnRightAnkleValid;
            BlendPositionWithGlobalTarget(target, g_pMotion->rightAnkleTargetBlendFrames);
        }
    } else if (ankleId == 11) {
        if (const Vector3 *const world = g_pMotion->worldTargets[3]) {
            g_motnAnkleKnee = &g_motnLeftKnee;
            g_motnAnkleKneeValid = g_motnLeftKneeValid;
            g_motnAnkleReach = reach;
            TranslateVectorByConditions(*world, target, g_pMotion->leftAnkleTargetBlendFrames);
        } else if (g_pMotion->leftAnkleTargetBlendFrames) {
            g_motnAnkleRelease = &g_motnLeftAnkle;
            g_motnAnkleReleaseValid = g_motnLeftAnkleValid;
            BlendPositionWithGlobalTarget(target, g_pMotion->leftAnkleTargetBlendFrames);
        }
    }
}

// CalculateLocalToWorldTransformation: re-expresses a leg target in the chest frame rotated by the current turn angle.
void CalculateLocalToWorldTransformation(Vector4 &position) {
    MotionInformationWork &motion = *g_pMotion;
    if (static_cast<std::int8_t>(motion.rootAngleFlags) < 0 || ((motion.motionFlags & 0x800000U) == 0 && motion.currentTurnAngle == 0))
        return;
    const Matrix4x4 &skin = g_skinningTransformMatrix;
    const float x = position.x - skin.rows[3].x;
    const float y = position.y - skin.rows[3].y;
    const float z = position.z - skin.rows[3].z;
    const Vector3 local{(skin.rows[0].y * y + skin.rows[0].x * x) + skin.rows[0].z * z, (skin.rows[1].y * y + skin.rows[1].x * x) + skin.rows[1].z * z,
        (skin.rows[2].x * x + skin.rows[2].y * y) + skin.rows[2].z * z};
    Matrix4x4_Push();
    workTransformMatrix = skin;
    const auto turn = static_cast<std::uint16_t>(motion.currentTurnAngle);
    const std::int16_t magnitude = turn <= 0x8000U ? motion.currentTurnAngle : static_cast<std::int16_t>(-turn);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude));
    Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(turn)), cosine);
    Vector3 world{};
    Matrix4x4::TransformPoint(local, world);
    SetPosition(position, world);
    Matrix4x4::Pop(1);
}

// ApplyRotationTransform: rotates by the row's euler, stores the row matrix, then aims at target.
void ApplyRotationTransform(MotionNodeRuntime &node, const Vector3 &target) {
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
    *MotionNodeMatrix(node) = workTransformMatrix;
    Matrix4x4::RotateZY(WorkLocalPoint(target));
}

// Matrix4x4_ApplyVec3Rotation: the bind-euler variant of ApplyRotationTransform used by the head-look blend.
void Matrix4x4_ApplyVec3Rotation(const MotionNodeRuntime &node, const Vector3 &target) {
    Matrix4x4::SetViewMatrixFromEuler(node.bindEulerSource->y, node.bindEulerSource->x, node.bindEulerSource->z);
    Matrix4x4::RotateZY(WorkLocalPoint(target));
}

// Vector3_LinearInterpolate.
void Vector3_LinearInterpolate(Vector3 &output, float t, const Vector3 &from, const Vector3 &to) {
    output.x = LerpDouble(from.x, to.x, t);
    output.y = LerpDouble(from.y, to.y, t);
    output.z = LerpDouble(from.z, to.z, t);
}

// CalculateAngleAndSetValue: turns the upper body toward the look target by the part of the target yaw beyond 45 degrees.
void CalculateAngleAndSetValue() {
    MotionInformationWork &motion = *g_pMotion;
    motion.rootAngleFlags &= ~0x4000U;
    const std::uint32_t flags = motion.rootAngleFlags;
    Vector3 target{};
    if ((flags & 0x40U) != 0) {
        target = motion.rootOffsetPosition;
    } else if (const Vector3 *const world = motion.worldTargets[2]) {
        target = *world;
    } else {
        motion.motionFlags &= ~0x400000U;
        motion.spineTurnCompensationAngle = 0;
        motion.targetTurnAngle = 0;
        motion.rootAngleFlags = flags | 0x2000U;
        motion.currentTurnAngle = 0;
        return;
    }
    g_skinningTransformMatrix = workTransformMatrix;
    const Vector3 local = WorkLocalPoint(target);
    const auto yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-local.z, -local.y));
    std::int16_t turn = 0;
    if (!(std::fabs(static_cast<float>(yaw)) < 8192.0f) && (motion.rootAngleFlags & 0x40U) == 0) {
        turn = static_cast<std::int16_t>(yaw >= 0 ? yaw - 0x2000 : yaw + 0x2000);
        if (turn >= 0) {
            if (turn > 0x2000)
                turn = 0x2000;
        } else if (std::fabs(static_cast<float>(turn)) > 8192.0f) {
            turn = -8192;
        }
    }
    motion.targetTurnAngle = turn;
    if ((motion.headLookAngleCommand & 0x7fffU) == 101U) {
        motion.rootAngleFlags |= 0x2000U;
        motion.currentTurnAngle = turn;
        return;
    }
    std::int16_t output = 0;
    motnUpdate(motion, {&motion.turnCurveState[2], &motion.turnCurveState[3], &motion.turnCurveState[1], &motion.turnCurveState[0], &motion.currentTurnAngle},
        turn, output, 0);
    motion.currentTurnAngle = output;
}

// MOTN_RotateX: applies the upper-body turn angle to the chest frame and records it as the spine compensation.
void MOTN_RotateX() {
    MotionInformationWork &motion = *g_pMotion;
    const std::uint32_t flags = motion.rootAngleFlags;
    if ((flags & 0x40U) != 0 && !motion.currentTurnAngle) {
        motion.motionFlags &= ~0x400000U;
        motion.rootAngleFlags = flags | 0x2000U;
    }
    const std::uint32_t current = motion.rootAngleFlags;
    if ((current & 0x80U) != 0 || ((motion.motionFlags & 0x400000U) == 0 && !motion.currentTurnAngle)) {
        motion.spineTurnCompensationAngle = 0;
        motion.targetTurnAngle = 0;
        motion.currentTurnAngle = 0;
        motion.rootAngleFlags = current | 0x2000U;
        return;
    }
    if ((motion.motionFlags & 0x400000U) != 0 || !motion.currentTurnAngle) {
        if ((motion.headLookAngleCommand & 0x8000U) != 0 && (motion.headLookAngleCommand & 0x7fffU) >= 0x65U)
            CalculateAngleAndSetValue();
    } else {
        const std::uint32_t command = motion.headLookAngleCommand;
        motion.headLookAngleCommand = 0x8068U;
        const bool forcedRootTarget = (current & 0x40U) == 0;
        if (forcedRootTarget)
            motion.rootAngleFlags = current | 0x40U;
        CalculateAngleAndSetValue();
        motion.headLookAngleCommand = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(command)));
        if (forcedRootTarget)
            motion.rootAngleFlags &= ~0x40U;
    }
    if ((motion.motionFlags & 0x4000U) != 0)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.currentTurnAngle));
    motion.spineTurnCompensationAngle = motion.currentTurnAngle;
}

// PerformMatrixTransformationAndAnimationUpdate: turns the head toward target in the skinning (chest) frame, ramping yaw and pitch,
// and writes the aim point half a unit ahead of the head into output.
void PerformMatrixTransformationAndAnimationUpdate(const Vector3 &target, Vector3 &output) {
    MotionInformationWork &motion = *g_pMotion;
    motion.rootAngleFlags &= ~0x1cU;
    const std::uint32_t initialFlags = motion.rootAngleFlags;
    const Vector3 targetPoint = (initialFlags & 0x40U) != 0 ? motion.rootOffsetPosition : target;
    const Matrix4x4 &skin = g_skinningTransformMatrix;
    const Vector3 localTarget = LocalDirectionTo(skin, targetPoint);
    const Vector3 localRoot = LocalDirectionTo(skin, motion.rootOffsetPosition);
    const std::uint32_t command = motion.headLookAngleCommand;
    const bool extended = (command & 0x8000U) != 0 && (command & 0x7fffU) >= 101U;
    std::int16_t yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-localTarget.z, -localTarget.y));
    if (extended) {
        if ((motion.motionFlags & 0x400000U) != 0)
            yaw = WrapS16(static_cast<std::int32_t>(yaw) + motion.currentTurnAngle - motion.targetTurnAngle);
        motion.headLookTimer = WrapS16(2 * motion.targetTurnAngle);
    } else if (std::fabs(static_cast<float>(yaw)) > 28672.0f) {
        motion.rootAngleFlags = initialFlags | 0x10U;
    }
    const float horizontal = std::sqrt(localTarget.z * localTarget.z + localTarget.y * localTarget.y);
    std::int16_t pitch = static_cast<std::int16_t>(-static_cast<std::int16_t>(Shenmue_Atan2Fixed(localTarget.x, horizontal)));
    if ((motion.motionFlags & 0x4000U) != 0)
        MOTN_ClampAnimationAngles(motion, yaw, pitch);
    if ((command & 0x7fffU) == 101U) {
        motion.rootAngleFlags |= 0x2002U;
    } else if (extended) {
        auto *const blend = reinterpret_cast<std::int16_t *>(&motion.lookAtBlendTimer);
        std::int16_t yawOut = 0;
        motnUpdate(motion, {&blend[1], &motion.lookAtState, &blend[0], &motion.headLookPreviousTimer, &motion.headLookAngleBase}, yaw, yawOut, 1);
        auto *const hold = reinterpret_cast<std::int16_t *>(&motion.lookAtHoldTimer);
        std::int16_t pitchOut = 0;
        motnUpdate(motion, {&hold[1], reinterpret_cast<std::int16_t *>(&motion.lookAtFlags), &hold[0], &motion.lookAtModeBackup, &motion.lookAtMode}, pitch,
            pitchOut, 2);
        yaw = yawOut;
        pitch = pitchOut;
    } else {
        const std::int32_t step = static_cast<std::int32_t>(command);
        const std::int32_t pitchDifference = static_cast<std::int32_t>(pitch) - motion.lookAtMode;
        if (static_cast<float>(step >> 1) < std::fabs(static_cast<float>(pitchDifference)))
            pitch = static_cast<std::int16_t>(pitchDifference >= 0 ? (step >> 1) + motion.lookAtMode : motion.lookAtMode - (step >> 1));
        else
            motion.rootAngleFlags |= 2U;
        const std::int32_t yawDifference = static_cast<std::int32_t>(yaw) - motion.headLookAngleBase;
        if (static_cast<float>(step) < std::fabs(static_cast<float>(yawDifference))) {
            yaw = static_cast<std::int16_t>(yawDifference >= 0 ? step + motion.headLookAngleBase : motion.headLookAngleBase - step);
            motion.headLookTimer = 0;
        } else {
            motion.rootAngleFlags |= 1U;
            motion.headLookAngleBase = yaw;
            motion.headLookTimer = 0;
        }
    }
    const std::int16_t rootYaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-localRoot.z, -localRoot.y));
    motion.headLookAngleBase = yaw;
    motion.lookAtMode = pitch;
    if ((motion.motionFlags & 0x4000U) != 0) {
        Matrix4x4_Push();
        workTransformMatrix = skin;
        const auto yawTurn = static_cast<std::uint16_t>(static_cast<std::uint16_t>(yaw) + 0x8000U);
        const std::int16_t yawMagnitude = yawTurn <= 0x8000U ? static_cast<std::int16_t>(yawTurn) : static_cast<std::int16_t>(-yawTurn);
        const float yawCosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - yawMagnitude));
        Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(yawTurn)), yawCosine);
        const std::int16_t pitchMagnitude = static_cast<std::uint16_t>(pitch) <= 0x8000U ? pitch : static_cast<std::int16_t>(-pitch);
        const float pitchCosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - pitchMagnitude));
        Matrix4x4::RotateAroundZ(sin_fixed16(pitch), pitchCosine);
        Matrix4x4::preTranslate(0.0f, 0.5f, 0.0f);
        output = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        if (!extended && (motion.motionFlags & 0x400000U) != 0)
            motion.currentTurnAngle = static_cast<std::int16_t>((static_cast<std::int32_t>(motion.headLookAngleBase) - rootYaw) >> 1);
    }
    if ((motion.rootAngleFlags & 0x2043U) == 0x2043U) {
        motion.motionFlags &= ~0xc00000U;
        motion.headLookAngleCommand = 2048U;
        motion.rootAngleFlags &= ~0x220040U;
    }
}

// SearchBinaryTreeRecursive.
MotionNodeRuntime *SearchBinaryTreeRecursive(std::uint32_t nodeId, MotionNodeRuntime *root) {
    if (!root)
        return nullptr;
    if (nodeId == root->nodeId)
        return root;
    for (std::uint8_t child = 0; child < root->childCount; ++child) {
        if (MotionNodeRuntime *const found = SearchBinaryTreeRecursive(nodeId, root->children[child]))
            return found;
    }
    return nullptr;
}

// MOTN_RotateBonesInIKChainForFrame_Lerp: the blended counterpart of MOTN_RotateAndTranslate. Mode 1 aims the first joint at the
// blended grandchild; mode 2 aims the chain at the blended (and ankle-targeted) end effector and solves the two-bone angles.
void MOTN_RotateBonesInIKChainForFrame_Lerp(float t, MotionNodeRuntime &source, MotionNodeRuntime &target) {
    float reach = 0.0f;
    if (target.flipFlags == 1) {
        MotionNodeRuntime &sourceChild = *source.children[0];
        MotionNodeRuntime &targetChild = *target.children[0];
        const MotionNodeRuntime &sourceEnd = *sourceChild.children[0];
        const MotionNodeRuntime &targetEnd = *targetChild.children[0];
        VectorTransformWithRotationLerp(t, source, target);
        Matrix4x4_Push();
        const Vector3 aim{LerpDouble(sourceEnd.effectorPosition.x, targetEnd.effectorPosition.x, t),
            LerpDouble(sourceEnd.effectorPosition.y, targetEnd.effectorPosition.y, t),
            LerpDouble(sourceEnd.effectorPosition.z, targetEnd.effectorPosition.z, t)};
        if (targetEnd.translationKeys) {
            Matrix4x4::preTranslate(targetChild.bindPosition->x, targetChild.bindPosition->y, targetChild.bindPosition->z);
            Matrix4x4::RotateZY(WorkLocalPoint(aim));
        } else {
            VectorTransformWithRotationLerp(t, sourceChild, targetChild);
        }
        if (target.nodeId == 19) {
            const std::uint32_t flags = g_pMotion->rootAngleFlags;
            if ((flags & 0x80U) == 0 && (flags & 0x20000U) != 0 && g_pMotion->currentTurnAngle)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-(g_pMotion->currentTurnAngle >> 1)));
        }
        *MotionNodeMatrix(targetChild) = workTransformMatrix;
        Matrix4x4::Pop(1);
        return;
    }
    if (target.flipFlags != 2)
        return;
    MotionNodeRuntime &targetUpper = *target.children[0];
    MotionNodeRuntime &sourceUpper = *source.children[0];
    MotionNodeRuntime &targetLower = *targetUpper.children[0];
    MotionNodeRuntime &sourceLower = *sourceUpper.children[0];
    const MotionNodeRuntime &targetEnd = *targetLower.children[0];
    const MotionNodeRuntime &sourceEnd = *sourceLower.children[0];
    Vector3 aim{LerpDouble(sourceEnd.effectorPosition.x, targetEnd.effectorPosition.x, t),
        LerpDouble(sourceEnd.effectorPosition.y, targetEnd.effectorPosition.y, t), LerpDouble(sourceEnd.effectorPosition.z, targetEnd.effectorPosition.z, t)};
    ApplyAnkleTarget(sourceEnd.nodeId, sourceLower.bindPosition->x + sourceEnd.bindPosition->x, &aim.x);
    Matrix4x4::preTranslate(source.effectorPosition.x, source.effectorPosition.y, source.effectorPosition.z);
    Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(source.bindEuler.y, source.bindEuler.x, source.bindEuler.z);
    const Vector3 startLocal = WorkLocalPoint(aim);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(source.bindEuler.y, source.bindEuler.x, source.bindEuler.z);
    Matrix4x4::RotateZY(startLocal);
    g_motnBlendStart = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(target.bindEuler.y, target.bindEuler.x, target.bindEuler.z);
    const Vector3 endLocal = WorkLocalPoint(aim);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(target.bindEuler.y, target.bindEuler.x, target.bindEuler.z);
    Matrix4x4::RotateZY(endLocal);
    g_motnBlendEnd = workTransformMatrix;
    Matrix4x4::Pop(1);
    MultiplyLerpedRotation(t);
    Matrix4x4_Push();
    if (targetEnd.translationKeys) {
        Matrix4x4::preTranslate(targetUpper.bindPosition->x, targetUpper.bindPosition->y, targetUpper.bindPosition->z);
        const Vector3 local = WorkLocalPoint(aim);
        reach = std::sqrt((local.y * local.y + local.x * local.x) + local.z * local.z);
        Matrix4x4::RotateZY(local);
        const float upper = targetLower.bindPosition->x;
        const float lower = targetEnd.bindPosition->x;
        const float cosine = std::clamp(((upper * upper + reach * reach) - lower * lower) / ((upper + upper) * reach), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (!targetLower.flipFlags)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, cosine);
    } else {
        VectorTransformWithRotationLerp(t, sourceUpper, targetUpper);
    }
    *MotionNodeMatrix(targetUpper) = workTransformMatrix;
    if (targetEnd.translationKeys) {
        Matrix4x4::preTranslate(targetLower.bindPosition->x, targetLower.bindPosition->y, targetLower.bindPosition->z);
        const float upper = targetLower.bindPosition->x;
        const float lower = targetEnd.bindPosition->x;
        const float cosine = std::clamp(((upper * upper + lower * lower) - reach * reach) / ((upper + upper) * lower), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (targetLower.flipFlags == 1)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, -cosine);
    } else {
        VectorTransformWithRotationLerp(t, sourceLower, targetLower);
    }
    *MotionNodeMatrix(targetLower) = workTransformMatrix;
    Matrix4x4::Pop(1);
}

// Transforms the secondary pose's target positions (captured relative to its anchor) back into the current anchor frame.
void PlaceSecondaryTargets(MotionNodeRuntime *sourceMotionNodes, const Matrix4x4 &anchor) {
    MotionInformationWork &motion = *g_pMotion;
    for (std::size_t index = 1; index < 10; ++index) {
        MotionNodeRuntime *found = nullptr;
        if (sourceMotionNodes) {
            if (kMotnTargetNodeIds[index] == sourceMotionNodes->nodeId)
                found = sourceMotionNodes;
            for (std::uint8_t child = 0; !found && child < sourceMotionNodes->childCount; ++child)
                found = SearchBinaryTreeRecursive(kMotnTargetNodeIds[index], sourceMotionNodes->children[child]);
        }
        if (!found)
            continue;
        if (const Vector3 *const world = motion.worldTargets[index]; world && static_cast<std::uint32_t>(index - 3) > 1U) {
            found->effectorPosition.x = world->x;
            found->effectorPosition.y = world->y;
            found->effectorPosition.z = world->z;
        } else {
            Matrix4x4_Push();
            workTransformMatrix = anchor;
            Vector3 placed{};
            Matrix4x4::TransformPoint(g_motnSecondaryTargets[index - 1], placed);
            SetPosition(found->effectorPosition, placed);
            Matrix4x4::Pop(1);
        }
        if (found->nodeId == 6) {
            Matrix4x4_Push();
            workTransformMatrix = anchor;
            Matrix4x4::TransformPoint(g_motnSecondaryTargets[index - 1], motion.previousRootOffsetPosition);
            Matrix4x4::Pop(1);
        }
    }
}

// Ryo_RecursiveIKForRealtimeMovement: builds the active skeleton while blending every row from the source pose (the previous pose,
// the transition rows, or at the secondary attachment node the secondary pose) by the source row's interpolation amount.
void Ryo_RecursiveIKForRealtimeMovement(MotionNodeRuntime &active, MotionNodeRuntime &blendSource) {
    MotionInformationWork &motion = *g_pMotion;
    MotionNodeRuntime *source = &blendSource;
    std::uint8_t firstChild = 0;
    if (blendSource.nodeId == motion.secondaryNodeId && blendSource.nodeId) {
        source = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
        if (source->interpolationAmount < 0x1p-23f && !MOTN_GetAnimationFlag(motion.secondaryAnimId)) {
            MOTN_ApplyBoneHierarchyTransform(active, *source);
            return;
        }
        PlaceSecondaryTargets(source, workTransformMatrix);
    }
    MotionNodeRuntime &target = *source->pairedSourceNode;
    const float t = source->interpolationAmount;
    const bool pushed = (active.bindPositionPacked[1] & 1U) != 0;
    if (pushed)
        Matrix4x4_Push();
    switch (target.templateParentOrMode) {
    case 0:
    case 1:
        VectorTransformWithRotationLerp(t, *source, target);
        break;
    case 2:
        if (active.nodeId == 1) {
            const MotionNodeRuntime &targetEnd = *target.children[0]->children[0];
            const MotionNodeRuntime &sourceEnd = *source->children[0]->children[0];
            const double inverse = 1.0 - static_cast<double>(t);
            const Vector3 aim{
                static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.x) + static_cast<double>(sourceEnd.effectorPosition.x) * inverse),
                static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.y) + static_cast<double>(sourceEnd.effectorPosition.y) * inverse),
                static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.z) + static_cast<double>(sourceEnd.effectorPosition.z) * inverse)};
            workTransformMatrix = g_motnRootInterpTransform;
            Matrix4x4::Multiply(&g_motnInterpTransform);
            ApplyRotationTransform(*source, aim);
            g_motnBlendStart = workTransformMatrix;
            workTransformMatrix = g_motnRootInterpTransform;
            Matrix4x4::Multiply(&g_motnFinalInterpTransform);
            ApplyRotationTransform(target, aim);
            g_motnBlendEnd = workTransformMatrix;
            workTransformMatrix = g_motnRootInterpTransform;
            MultiplyLerpedRotation(t);
            MOTN_RotateX();
            *MotionNodeMatrix(*active.children[0]) = workTransformMatrix;
        } else if (active.nodeId != 4) {
            MOTN_RotateBonesInIKChainForFrame_Lerp(t, *source, target);
        } else {
            Matrix4x4::preTranslate(LerpDouble(source->effectorPosition.x, target.effectorPosition.x, t),
                LerpDouble(source->effectorPosition.y, target.effectorPosition.y, t), LerpDouble(source->effectorPosition.z, target.effectorPosition.z, t));
            Vector3_LinearInterpolate(motion.rootOffsetPosition, t, motion.previousRootOffsetPosition, motion.rootOffsetPosition);
            Vector3 aim{};
            if (const Vector3 *const world = motion.worldTargets[2]) {
                aim = *world;
            } else {
                const MotionNodeRuntime &targetEnd = *target.children[0]->children[0];
                const MotionNodeRuntime &sourceEnd = *source->children[0]->children[0];
                const double inverse = 1.0 - static_cast<double>(t);
                aim = {static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.x) + static_cast<double>(sourceEnd.effectorPosition.x) * inverse),
                    static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.y) + static_cast<double>(sourceEnd.effectorPosition.y) * inverse),
                    static_cast<float>(static_cast<double>(t * targetEnd.effectorPosition.z) + static_cast<double>(sourceEnd.effectorPosition.z) * inverse)};
            }
            PerformMatrixTransformationAndAnimationUpdate(aim, aim);
            Matrix4x4_Push();
            Matrix4x4_Push();
            ApplyRotationTransform(*source, aim);
            g_motnBlendStart = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::InvertInPlace(nullptr);
            Matrix4x4::Multiply(&g_motnBlendStart);
            g_motnBlendStart = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4_Push();
            ApplyRotationTransform(target, aim);
            g_motnBlendEnd = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::InvertInPlace(nullptr);
            Matrix4x4::Multiply(&g_motnBlendEnd);
            g_motnBlendEnd = workTransformMatrix;
            Matrix4x4::Pop(1);
            if (motion.blendScratch[3] == 0.0f) {
                MultiplyLerpedRotation(t);
            } else {
                Matrix4x4 blended{};
                if (Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd))
                    g_motnBlendEnd = blended;
                Matrix4x4_Push();
                Matrix4x4_Push();
                Matrix4x4_ApplyVec3Rotation(target, aim);
                g_motnBlendStart = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::InvertInPlace(nullptr);
                Matrix4x4::Multiply(&g_motnBlendStart);
                g_motnBlendStart = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::Multiply(Matrix4x4_Lerp(blended, motion.blendScratch[3], g_motnBlendEnd, g_motnBlendStart) ? &blended : &g_motnBlendStart);
            }
            *MotionNodeMatrix(*active.children[0]) = workTransformMatrix;
        }
        break;
    case 3:
        workTransformMatrix = *MotionNodeMatrix(active);
        break;
    case 4:
        Matrix4x4::preTranslate(active.bindPosition->x, active.bindPosition->y, active.bindPosition->z);
        if (active.flags)
            g_motnNeedsRotation = 1;
        if (active.nodeId == 3 && static_cast<std::int8_t>(motion.rootAngleFlags) >= 0) {
            g_skinningTransformMatrix = workTransformMatrix;
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(&target, 11))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(&target, 17))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(source, 11))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(source, 17))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
        }
        if (active.nodeId == 38 && static_cast<std::int8_t>(motion.rootAngleFlags) < 0)
            g_skinningTransformMatrix = workTransformMatrix;
        break;
    case 6: {
        Matrix4x4::preTranslate(LerpDouble(source->effectorPosition.x, target.effectorPosition.x, t),
            LerpDouble(source->effectorPosition.y, target.effectorPosition.y, t), LerpDouble(source->effectorPosition.z, target.effectorPosition.z, t));
        g_motnRootInterpTransform = workTransformMatrix;
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        if (g_motnNextEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(source->bindEuler.y, source->bindEuler.x, source->bindEuler.z);
        g_motnBlendStart = workTransformMatrix;
        g_motnInterpTransform = workTransformMatrix;
        workTransformMatrix = transformMatrix_prePass;
        if (g_motnEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(target.bindEuler.y, target.bindEuler.x, target.bindEuler.z);
        g_motnBlendEnd = workTransformMatrix;
        g_motnFinalInterpTransform = workTransformMatrix;
        Matrix4x4::Pop(1);
        MultiplyLerpedRotation(t);
        g_motnNeedsRotation = 0;
        firstChild = motion.updateMode != 0 ? 1 : 0;
        break;
    }
    default:
        break;
    }
    *MotionNodeMatrix(active) = workTransformMatrix;
    active.bindPositionPacked[1] |= 2U;
    if (motion.updateMode && active.nodeId == 3) {
        if (pushed)
            Matrix4x4::Pop(1);
        return;
    }
    for (std::uint8_t child = firstChild; child < active.childCount; ++child)
        Ryo_RecursiveIKForRealtimeMovement(*active.children[child], *source->children[child]);
    if ((active.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// Matrix4x4_TranslateViewMatrixEuler (TranslateEuler): translate by the row effector, rotate by its euler, store.
void TranslateEuler(MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
}

// ApplyTranslationToShenmueTransformMatrix (TranslateOnly): translate by the bind offset and store; node 38 captures the skinning
// frame for rigs whose look targets are not chest-relative.
void TranslateOnly(MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.bindPosition->x, node.bindPosition->y, node.bindPosition->z);
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
    if (node.nodeId == 38 && static_cast<std::int8_t>(g_pMotion->rootAngleFlags) < 0)
        g_skinningTransformMatrix = workTransformMatrix;
}

// The rig programs' inline "work = *row->xform; row flag |= 2".
void AdoptMotionMatrix(MotionNodeRuntime &node) {
    workTransformMatrix = *MotionNodeMatrix(node);
    node.bindPositionPacked[1] |= 2U;
}

// transformMatrixWithPreTranslateAndRotate (ApplyRootTransform): root row in the entity frame.
void ApplyRootTransform(MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    if (g_motnEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
}

// applyTransformationsAndReturnMatrix: aims the row's child at its keyed grandchild (or applies the child's euler) and, for
// humanoid rigs with the half-turn flag, counter-rotates by half the turn angle.
void AimChildAtGrandchild(MotionNodeRuntime &node) {
    MotionNodeRuntime &child = *node.children[0];
    const MotionNodeRuntime &grandchild = *child.children[0];
    if (grandchild.translationKeys) {
        Matrix4x4::preTranslate(child.bindPosition->x, child.bindPosition->y, child.bindPosition->z);
        Matrix4x4::RotateZY(WorkLocalPoint(PositionOf(grandchild.effectorPosition)));
    } else {
        TranslateEuler(child);
    }
    const std::uint32_t flags = g_pMotion->rootAngleFlags;
    if ((flags & 0x80U) == 0 && (flags & 0x20000U) != 0 && g_pMotion->currentTurnAngle)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-(g_pMotion->currentTurnAngle >> 1)));
    *MotionNodeMatrix(child) = workTransformMatrix;
}

// ApplyTransformationToTransformMatrix (ApplyTransformationWrapper).
void ApplyTransformationWrapper(MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
    Matrix4x4_Push();
    AimChildAtGrandchild(node);
    Matrix4x4::Pop(1);
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
}

// Matrix4x4_TransformMatrixByVector: the chest variant of the child aim, which applies the upper-body turn through MOTN_RotateX.
void AimChestChild(MotionNodeRuntime &node) {
    MotionNodeRuntime &child = *node.children[0];
    const MotionNodeRuntime &grandchild = *child.children[0];
    if (grandchild.translationKeys) {
        Matrix4x4::preTranslate(child.bindPosition->x, child.bindPosition->y, child.bindPosition->z);
        Matrix4x4::RotateZY(WorkLocalPoint(PositionOf(grandchild.effectorPosition)));
    } else {
        TranslateEuler(child);
    }
    MOTN_RotateX();
    *MotionNodeMatrix(child) = workTransformMatrix;
}

// Matrix4x4_TransformMatrixWithPreTranslateAndRotate (ApplyChestTransform).
void ApplyChestTransform(MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
    Matrix4x4_Push();
    AimChestChild(node);
    Matrix4x4::Pop(1);
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
}

// MOTN_BuildBoneTransformFromData: the two-bone limb solve below an anchor row, with the ankle world-target handling.
void SolveTwoBoneChain(MotionNodeRuntime &anchor) {
    MotionNodeRuntime &upper = *anchor.children[0];
    MotionNodeRuntime &lower = *upper.children[0];
    MotionNodeRuntime &end = *lower.children[0];
    ApplyAnkleTarget(end.nodeId, lower.bindPosition->x + end.bindPosition->x, &end.effectorPosition.x);
    float reach = 0.0f;
    if (end.translationKeys) {
        Matrix4x4::preTranslate(upper.bindPosition->x, upper.bindPosition->y, upper.bindPosition->z);
        const Vector3 local = WorkLocalPoint(PositionOf(end.effectorPosition));
        reach = std::sqrt((local.y * local.y + local.x * local.x) + local.z * local.z);
        Matrix4x4::RotateZY(local);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + reach * reach) - lowerLength * lowerLength) / ((upperLength + upperLength) * reach), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (!lower.flipFlags)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, cosine);
    } else {
        TranslateEuler(upper);
    }
    *MotionNodeMatrix(upper) = workTransformMatrix;
    if (end.translationKeys) {
        Matrix4x4::preTranslate(lower.bindPosition->x, lower.bindPosition->y, lower.bindPosition->z);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + lowerLength * lowerLength) - reach * reach) / ((upperLength + upperLength) * lowerLength), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (lower.flipFlags == 1)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, -cosine);
    } else {
        TranslateEuler(lower);
    }
    *MotionNodeMatrix(lower) = workTransformMatrix;
}

// BuildBoneTransformFromData (BuildBoneTransform).
void BuildBoneTransform(MotionNodeRuntime &anchor) {
    Matrix4x4::preTranslate(anchor.effectorPosition.x, anchor.effectorPosition.y, anchor.effectorPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(anchor.bindEuler.y, anchor.bindEuler.x, anchor.bindEuler.z);
    Matrix4x4_Push();
    SolveTwoBoneChain(anchor);
    Matrix4x4::Pop(1);
    *MotionNodeMatrix(anchor) = workTransformMatrix;
    anchor.bindPositionPacked[1] |= 2U;
}

// preTranslate_seemsToRotate_Wrapper_1 (AnklePrepass): keeps the position and replaces the basis with the entity rotation.
void AnklePrepass() {
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (g_motnEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
}

// motnUnknown (AdjustLookTargets): captures the skinning (chest) frame and re-expresses the two hand targets in the turned chest.
void AdjustLookTargets(MotionNodeRuntime *node, std::size_t first, std::size_t second) {
    if (static_cast<std::int8_t>(g_pMotion->rootAngleFlags) < 0)
        return;
    g_skinningTransformMatrix = workTransformMatrix;
    CalculateLocalToWorldTransformation(node[second].effectorPosition);
    CalculateLocalToWorldTransformation(node[first].effectorPosition);
}

// preTranslate_RotationWrapper: aims the head row's child at the look target (or applies its euler when it has no keyed target).
void AimHeadChild(MotionNodeRuntime &node, const Vector3 &target) {
    MotionNodeRuntime &child = *node.children[0];
    if (child.children[0]->translationKeys) {
        Matrix4x4::preTranslate(child.bindPosition->x, child.bindPosition->y, child.bindPosition->z);
        Matrix4x4::RotateZY(WorkLocalPoint(target));
    } else {
        TranslateEuler(child);
    }
    *MotionNodeMatrix(child) = workTransformMatrix;
}

// ApplyHeadLookTransform (ApplyHeadTransform): the head row. The look-blend ramp (blendScratch[3]) blends the aimed head rotation
// with the bind-rotation aim.
void ApplyHeadTransform(MotionNodeRuntime &node) {
    MotionInformationWork &motion = *g_pMotion;
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    const MotionNodeRuntime &lookTarget = *node.children[0]->children[0];
    Vector3 aim = PositionOf(lookTarget.effectorPosition);
    PerformMatrixTransformationAndAnimationUpdate(PositionOf(lookTarget.effectorPosition), aim);
    if (motion.blendScratch[3] == 0.0f) {
        Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
        Matrix4x4_Push();
        AimHeadChild(node, aim);
        Matrix4x4::Pop(1);
    } else {
        Matrix4x4_Push();
        Matrix4x4_Push();
        ApplyRotationTransform(node, aim);
        g_motnBlendEnd = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(&g_motnBlendEnd);
        g_motnBlendEnd = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4_Push();
        Matrix4x4_ApplyVec3Rotation(node, aim);
        g_motnBlendStart = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(&g_motnBlendStart);
        g_motnBlendStart = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4 blended{};
        Matrix4x4::Multiply(Matrix4x4_Lerp(blended, motion.blendScratch[3], g_motnBlendEnd, g_motnBlendStart) ? &blended : &g_motnBlendStart);
        *MotionNodeMatrix(*node.children[0]) = workTransformMatrix;
    }
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
}

// Matrix4x4_preTranslate_RotateXYZ.
void Matrix4x4_preTranslate_RotateXYZ(const MotionNodeRuntime &node) {
    Matrix4x4::preTranslate(node.effectorPosition.x, node.effectorPosition.y, node.effectorPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(node.bindEuler.y, node.bindEuler.x, node.bindEuler.z);
}

// MOTN_RotateAndTranslate: the unblended chain step of the bone hierarchy (and secondary) traversal. Mode 1 aims the child at its
// grandchild (the head through PerformMatrixTransformationAndAnimationUpdate); mode 2 solves the two-bone limb.
void MOTN_RotateAndTranslate(MotionNodeRuntime &node, MotionNodeRuntime &source) {
    float reach = 0.0f;
    if (node.flipFlags == 1) {
        MotionNodeRuntime &sourceChild = *source.children[0];
        MotionNodeRuntime &child = *node.children[0];
        MotionNodeRuntime &sourceEnd = *sourceChild.children[0];
        g_motnLookTarget = PositionOf(sourceEnd.effectorPosition);
        if (source.nodeId == 4)
            PerformMatrixTransformationAndAnimationUpdate(PositionOf(sourceEnd.effectorPosition), g_motnLookTarget);
        if (sourceEnd.translationKeys) {
            Matrix4x4::preTranslate(child.bindPosition->x, child.bindPosition->y, child.bindPosition->z);
            Matrix4x4::RotateZY(WorkLocalPoint(g_motnLookTarget));
        } else {
            Matrix4x4::preTranslate(sourceChild.effectorPosition.x, sourceChild.effectorPosition.y, sourceChild.effectorPosition.z);
            Matrix4x4::SetViewMatrixFromEuler(sourceChild.bindEuler.y, sourceChild.bindEuler.x, sourceChild.bindEuler.z);
        }
        if (source.nodeId == 1) {
            MOTN_RotateX();
        } else if (source.nodeId == 19) {
            const std::uint32_t flags = g_pMotion->rootAngleFlags;
            if ((flags & 0x80U) == 0 && (flags & 0x20000U) != 0 && g_pMotion->currentTurnAngle)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-(g_pMotion->currentTurnAngle >> 1)));
        }
        *MotionNodeMatrix(child) = workTransformMatrix;
        return;
    }
    if (node.flipFlags != 2)
        return;
    MotionNodeRuntime &sourceUpper = *source.children[0];
    MotionNodeRuntime &upper = *node.children[0];
    MotionNodeRuntime &sourceLower = *sourceUpper.children[0];
    MotionNodeRuntime &lower = *upper.children[0];
    MotionNodeRuntime &sourceEnd = *sourceLower.children[0];
    const MotionNodeRuntime &end = *lower.children[0];
    ApplyAnkleTarget(sourceEnd.nodeId, sourceLower.bindPosition->x + sourceEnd.bindPosition->x, &sourceEnd.effectorPosition.x);
    if (sourceEnd.translationKeys) {
        Matrix4x4::preTranslate(upper.bindPosition->x, upper.bindPosition->y, upper.bindPosition->z);
        const Vector3 local = WorkLocalPoint(PositionOf(sourceEnd.effectorPosition));
        reach = std::sqrt((local.y * local.y + local.x * local.x) + local.z * local.z);
        Matrix4x4::RotateZY(local);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + reach * reach) - lowerLength * lowerLength) / ((upperLength + upperLength) * reach), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (!lower.flipFlags)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, cosine);
    } else {
        Matrix4x4::preTranslate(sourceUpper.effectorPosition.x, sourceUpper.effectorPosition.y, sourceUpper.effectorPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(sourceUpper.bindEuler.y, sourceUpper.bindEuler.x, sourceUpper.bindEuler.z);
    }
    *MotionNodeMatrix(upper) = workTransformMatrix;
    if (sourceEnd.translationKeys) {
        Matrix4x4::preTranslate(lower.bindPosition->x, lower.bindPosition->y, lower.bindPosition->z);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + lowerLength * lowerLength) - reach * reach) / ((upperLength + upperLength) * lowerLength), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (lower.flipFlags == 1)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, -cosine);
    } else {
        Matrix4x4::preTranslate(sourceLower.effectorPosition.x, sourceLower.effectorPosition.y, sourceLower.effectorPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(sourceLower.bindEuler.y, sourceLower.bindEuler.x, sourceLower.bindEuler.z);
    }
    *MotionNodeMatrix(lower) = workTransformMatrix;
}

// MOTN_ApplyBoneHierarchyTransform: the unblended traversal used for secondary overlays and the scripted update modes. node holds
// the matrices; source supplies the sampled values (the active rows themselves, or the secondary rows below the attachment node).
void MOTN_ApplyBoneHierarchyTransform(MotionNodeRuntime &node, MotionNodeRuntime &nodeSource) {
    MotionInformationWork &motion = *g_pMotion;
    MotionNodeRuntime *source = &nodeSource;
    std::uint8_t firstChild = 0;
    if (node.nodeId == motion.secondaryNodeId && motion.secondaryNodeId) {
        auto *const secondary = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
        if (secondary->interpolationAmount > 0x1p-23f || MOTN_GetAnimationFlag(motion.secondaryAnimId)) {
            Ryo_RecursiveIKForRealtimeMovement(node, *secondary);
            return;
        }
        const Matrix4x4 anchor = workTransformMatrix;
        for (std::size_t index = 1; index < 10; ++index) {
            MotionNodeRuntime *const motionNode = FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.sourceNodes), kMotnTargetNodeIds[index]);
            if (!motionNode)
                continue;
            if (const Vector3 *const world = motion.worldTargets[index]; world && static_cast<std::uint32_t>(index - 3) > 1U) {
                motionNode->effectorPosition.x = world->x;
                motionNode->effectorPosition.y = world->y;
                motionNode->effectorPosition.z = world->z;
            } else {
                Matrix4x4_Push();
                workTransformMatrix = anchor;
                Vector3 placed{};
                Matrix4x4::TransformPoint(g_motnSecondaryTargets[index - 1], placed);
                SetPosition(motionNode->effectorPosition, placed);
                Matrix4x4::Pop(1);
            }
            if (motionNode->nodeId == 6) {
                Matrix4x4_Push();
                workTransformMatrix = anchor;
                Matrix4x4::TransformPoint(g_motnSecondaryTargets[index - 1], motion.rootOffsetPosition);
                Matrix4x4::Pop(1);
            }
        }
        source = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
    }
    const bool pushed = (node.bindPositionPacked[1] & 1U) != 0;
    if (pushed)
        Matrix4x4_Push();
    switch (node.templateParentOrMode) {
    case 0:
    case 1:
        Matrix4x4_preTranslate_RotateXYZ(*source);
        break;
    case 2:
        if (node.nodeId == 4) {
            ApplyHeadTransform(*source);
        } else {
            Matrix4x4_preTranslate_RotateXYZ(*source);
            Matrix4x4_Push();
            MOTN_RotateAndTranslate(node, *source);
            Matrix4x4::Pop(1);
        }
        break;
    case 3:
        workTransformMatrix = *MotionNodeMatrix(node);
        break;
    case 4:
        Matrix4x4::preTranslate(node.bindPosition->x, node.bindPosition->y, node.bindPosition->z);
        if (node.flags) {
            const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            if (g_motnEntityEuler)
                Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        }
        if (source->nodeId == 3 && static_cast<std::int8_t>(motion.rootAngleFlags) >= 0) {
            g_skinningTransformMatrix = workTransformMatrix;
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(source, 11))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
            if (MotionNodeRuntime *const motionNode = FindMotionNodeFrom(source, 17))
                CalculateLocalToWorldTransformation(motionNode->effectorPosition);
        }
        if (source->nodeId == 38 && static_cast<std::int8_t>(motion.rootAngleFlags) < 0)
            g_skinningTransformMatrix = workTransformMatrix;
        break;
    case 6: {
        Matrix4x4::preTranslate(source->effectorPosition.x, source->effectorPosition.y, source->effectorPosition.z);
        g_motnRootInterpTransform = workTransformMatrix;
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        if (g_motnEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(source->bindEuler.y, source->bindEuler.x, source->bindEuler.z);
        g_motnFinalInterpTransform = workTransformMatrix;
        g_motnInterpTransform = workTransformMatrix;
        const std::uint8_t updateMode = motion.updateMode;
        Matrix4x4::Pop(1);
        Matrix4x4::Multiply(&g_motnFinalInterpTransform);
        firstChild = updateMode != 0 ? 1 : 0;
        break;
    }
    default:
        break;
    }
    *MotionNodeMatrix(node) = workTransformMatrix;
    node.bindPositionPacked[1] |= 2U;
    if (motion.updateMode && node.nodeId == 3) {
        if (pushed)
            Matrix4x4::Pop(1);
        return;
    }
    for (std::uint8_t child = firstChild; child < node.childCount; ++child)
        MOTN_ApplyBoneHierarchyTransform(*node.children[child], *source->children[child]);
    if ((node.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// ComputeLinearRamp01.
float ComputeLinearRamp01(std::int32_t current, std::int32_t total) {
    const float frames = static_cast<float>(total);
    const float half = frames * 0.5f;
    const float value = static_cast<float>(current);
    if (0x1p-23f > frames)
        return 1.0f;
    if (half <= value)
        return 1.0f - (((half + half) - value) / (half + half));
    return (value * 0.5f) / half;
}

// MOTN_GetSmoothedFrameDelta: the render-frame-mode scale applied to every clock advance.
float MOTN_GetSmoothedFrameDelta(float delta) {
    if (g_updateRenderFrameMode > 0)
        return delta * 1.1875f;
    if (g_updateRenderFrameMode == 0)
        return delta * 0.0625f;
    return delta;
}

// MOTN_LinkSecondaryNodes: pairs the secondary rows with the active rows along the tree.
void MOTN_LinkSecondaryNodes(MotionNodeRuntime &secondary, MotionNodeRuntime &active) {
    secondary.pairedSourceNode = &active;
    for (std::uint8_t child = 0; child < secondary.childCount; ++child)
        MOTN_LinkSecondaryNodes(*secondary.children[child], *active.children[child]);
}

// MOTN_ReadAndConvertMOTNDataAndTransformChain: the secondary pose's limb chain at the secondary clock.
void MOTN_SampleSecondaryChain(MotionNodeRuntime &node, float frame) {
    if (node.flipFlags == 1) {
        MotionNodeRuntime &child = *node.children[0];
        MotionNodeRuntime &end = *child.children[0];
        MOTN_readAndConvertPositionMOTNData(frame, end);
        if (end.translationKeys) {
            Matrix4x4::preTranslate(child.bindPosition->x, child.bindPosition->y, child.bindPosition->z);
            Matrix4x4::RotateZY(WorkLocalPoint(PositionOf(end.effectorPosition)));
        } else {
            MOTN_readAndConvertRotationMOTNData(frame, child);
            Matrix4x4_preTranslate_RotateXYZ(child);
        }
        *MotionNodeMatrix(child) = workTransformMatrix;
        return;
    }
    if (node.flipFlags != 2)
        return;
    MotionNodeRuntime &upper = *node.children[0];
    MotionNodeRuntime &lower = *upper.children[0];
    MotionNodeRuntime &end = *lower.children[0];
    MOTN_readAndConvertPositionMOTNData(frame, end);
    float reach = 0.0f;
    if (end.translationKeys) {
        Matrix4x4::preTranslate(upper.bindPosition->x, upper.bindPosition->y, upper.bindPosition->z);
        const Vector3 local = WorkLocalPoint(PositionOf(end.effectorPosition));
        reach = std::sqrt((local.y * local.y + local.x * local.x) + local.z * local.z);
        Matrix4x4::RotateZY(local);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + reach * reach) - lowerLength * lowerLength) / ((upperLength + upperLength) * reach), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (!lower.flipFlags)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, cosine);
    } else {
        MOTN_readAndConvertRotationMOTNData(frame, upper);
        Matrix4x4_preTranslate_RotateXYZ(upper);
    }
    *MotionNodeMatrix(upper) = workTransformMatrix;
    if (end.translationKeys) {
        Matrix4x4::preTranslate(lower.bindPosition->x, lower.bindPosition->y, lower.bindPosition->z);
        const float upperLength = lower.bindPosition->x;
        const float lowerLength = end.bindPosition->x;
        const float cosine =
            std::clamp(((upperLength * upperLength + lowerLength * lowerLength) - reach * reach) / ((upperLength + upperLength) * lowerLength), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (lower.flipFlags == 1)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, -cosine);
    } else {
        MOTN_readAndConvertRotationMOTNData(frame, lower);
        Matrix4x4_preTranslate_RotateXYZ(lower);
    }
    *MotionNodeMatrix(lower) = workTransformMatrix;
}

// MOTN_SampleSecondaryHierarchy: walks the secondary FCV pose, recording the attachment
// node's parent frame and matrices from the attachment node down.
void MOTN_SampleSecondaryHierarchy(MotionNodeRuntime &node, float frame, bool attached) {
    MotionInformationWork &motion = *g_pMotion;
    if (node.nodeId == motion.secondaryNodeId) {
        g_motnSecondaryAnchor = workTransformMatrix;
        attached = true;
        motion.sourceNodes = &node;
    }
    const bool pushed = (node.bindPositionPacked[1] & 1U) != 0;
    if (pushed)
        Matrix4x4_Push();
    std::uint8_t firstChild = 0;
    switch (node.templateParentOrMode) {
    case 0:
    case 1:
        MOTN_readAndConvertRotationMOTNData(frame, node);
        Matrix4x4_preTranslate_RotateXYZ(node);
        break;
    case 2:
        MOTN_readAndConvertRotationMOTNData(frame, node);
        Matrix4x4_preTranslate_RotateXYZ(node);
        Matrix4x4_Push();
        MOTN_SampleSecondaryChain(node, frame);
        Matrix4x4::Pop(1);
        break;
    case 3:
        workTransformMatrix = *MotionNodeMatrix(node);
        break;
    case 4:
        Matrix4x4::preTranslate(node.bindPosition->x, node.bindPosition->y, node.bindPosition->z);
        if (node.flags) {
            const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(position.x, position.y, position.z);
        }
        break;
    case 6:
        MOTN_readAndConvertRotationMOTNData(frame, node);
        Matrix4x4_preTranslate_RotateXYZ(node);
        firstChild = motion.secondaryNodeId < 19 ? 1 : 0;
        break;
    default:
        break;
    }
    if (attached) {
        *MotionNodeMatrix(node) = workTransformMatrix;
        node.bindPositionPacked[1] |= 2U;
    }
    for (std::uint8_t child = firstChild; child < node.childCount; ++child) {
        if (node.children[child]->nodeId == g_pMotion->secondaryNodeId) {
            *MotionNodeMatrix(node) = workTransformMatrix;
            node.bindPositionPacked[1] |= 2U;
        }
        MOTN_SampleSecondaryHierarchy(*node.children[child], frame, attached);
    }
    if ((node.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// Captures a secondary row's effector relative to the attachment frame (the InVector entries Ryo_RecursiveIK re-places).
void CaptureSecondaryTarget(std::size_t index, const MotionNodeRuntime &motionNode) {
    const Matrix4x4 &anchor = g_motnSecondaryAnchor;
    const float x = motionNode.effectorPosition.x - anchor.rows[3].x;
    const float y = motionNode.effectorPosition.y - anchor.rows[3].y;
    const float z = motionNode.effectorPosition.z - anchor.rows[3].z;
    g_motnSecondaryTargets[index] = {(anchor.rows[0].y * y + anchor.rows[0].x * x) + anchor.rows[0].z * z,
        (anchor.rows[1].y * y + anchor.rows[1].x * x) + anchor.rows[1].z * z, (anchor.rows[2].y * y + anchor.rows[2].x * x) + anchor.rows[2].z * z};
}

// MOTN_CaptureEffectorsFromMatrices: end-effector rows take their evaluated matrix translation.
void MOTN_CaptureEffectorsFromMatrices(MotionNodeRuntime *motionNodes) {
    for (MotionNodeRuntime *motionNode = motionNodes; motionNode->nodeId != 0xff; ++motionNode) {
        if (((motionNode->templateParentOrMode - 4U) & 0xfdU) == 0) {
            motionNode->effectorPosition.x = motionNode->transform->rows[3].x;
            motionNode->effectorPosition.y = motionNode->transform->rows[3].y;
            motionNode->effectorPosition.z = motionNode->transform->rows[3].z;
        }
    }
}

// RotateVirtualBones: when the fixed-record primary pose drives the rig, re-derives the eulers of the virtual joints from the
// evaluated matrices (undoing the wrist and spine compensation rotations).
void RotateVirtualBones(MotionNodeRuntime *motionNodes) {
    constexpr std::array<std::array<std::uint8_t, 3>, 11> kVirtualBones{
        {{0, 1, 2}, {3, 4, 5}, {38, 4, 5}, {7, 8, 9}, {9, 10, 10}, {13, 14, 15}, {15, 16, 16}, {22, 23, 24}, {24, 25, 25}, {29, 30, 31}, {31, 32, 32}}};
    MotionInformationWork &motion = *g_pMotion;
    Matrix4x4_Push();
    for (const std::array<std::uint8_t, 3> &bones : kVirtualBones) {
        if (!motionNodes)
            continue;
        MotionNodeRuntime *const parent = FindMotionNodeFrom(motionNodes, bones[0]);
        if (!parent || (parent->bindPositionPacked[1] & 2U) == 0)
            continue;
        MotionNodeRuntime *const joint = FindMotionNodeFrom(motionNodes, bones[1]);
        if (!joint || (joint->bindPositionPacked[1] & 2U) == 0)
            continue;
        MotionNodeRuntime *const child = FindMotionNodeFrom(motionNodes, bones[2]);
        if (!child || (child->bindPositionPacked[1] & 2U) == 0)
            continue;
        workTransformMatrix = *MotionNodeMatrix(*parent);
        if (parent->nodeId == 9)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.rightHandRotationAngle));
        if (parent->nodeId == 15)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.leftHandRotationAngle));
        if (parent->nodeId == 3)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.spineTurnCompensationAngle));
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(child->transform);
        if (child->nodeId == 2)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-motion.spineTurnCompensationAngle));
        if (child->nodeId == 9)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.rightHandRotationAngle));
        if (child->nodeId == 15)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion.leftHandRotationAngle));
        const Vector3i euler = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
        joint->bindEuler = {static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    }
    Matrix4x4::Pop(1);
}

// SeeminglyDoesNothing (MOTN_UpdateSecondaryMotion): advances the secondary overlay's blend weight and samples its pose, capturing
// the target rows relative to the attachment frame.
void MOTN_UpdateSecondaryMotion(MotionInformationWork &motion) {
    if (motion.secondaryBlendSeed && (static_cast<std::uint32_t>(motion.motionScaleFrames) + 5U) > 10U)
        --motion.secondaryBlendSeed;
    const std::uint8_t seed = motion.secondaryBlendSeed;
    const std::int32_t previousFrames = motion.secondaryBlendFramesPrevious;
    float weight = ComputeLinearRamp01(previousFrames - seed, previousFrames);
    const float baseScale = motion.baseMotionScale;
    if (1.0f - baseScale > 0x1p-23f)
        weight = std::min(baseScale, weight);
    auto *const secondaryMotionNodes = static_cast<MotionNodeRuntime *>(motion.secondaryNodes);
    auto *const activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    if (secondaryMotionNodes->pairedSourceNode != activeMotionNodes) {
        const float scale = motion.secondaryBlendFrameScale;
        if (1.0f - scale > 0x1p-23f && (static_cast<std::uint32_t>(motion.motionScaleFrames) + 5U) > 10U) {
            motion.motionScaleBlendStart = scale;
            motion.motionScaleFrames = 4;
        }
    }
    const std::int32_t scaleFrames = motion.motionScaleFrames;
    if (static_cast<std::uint32_t>(scaleFrames) + 5U <= 10U) {
        if (seed && 1.0f - motion.secondaryBlendFrameScale < 0x1p-23f) {
            motion.motionScaleBlendStart = baseScale;
            motion.secondaryBlendSeed = 0;
        }
        motion.motionScaleFrames = scaleFrames - 1;
        weight = 1.0f - (std::fabs(static_cast<float>(scaleFrames)) * (1.0f - motion.motionScaleBlendStart)) / 5.0f;
    }
    if (secondaryMotionNodes->pairedSourceNode != activeMotionNodes && 1.0f - motion.secondaryBlendFrameScale < 0x1p-23f)
        MOTN_LinkSecondaryNodes(*secondaryMotionNodes, *activeMotionNodes);
    if ((motion.motionFlags & 0x4000000U) != 0) {
        const float remaining = motion.secondaryFrameEnd - motion.secondaryFrameTimer;
        if (static_cast<float>(previousFrames) > remaining) {
            weight = ComputeLinearRamp01(static_cast<std::int32_t>(remaining + 1.0f), previousFrames);
            motion.motionScaleFrames = 10;
        }
    } else if (const std::uint8_t countdown = motion.secondaryBlendCountdown) {
        if (secondaryMotionNodes->pairedSourceNode != activeMotionNodes || (static_cast<std::uint32_t>(motion.motionScaleFrames) + 5U) <= 10U) {
            motion.secondaryBlendCountdown = static_cast<std::uint8_t>(countdown + 1);
        } else {
            weight = ComputeLinearRamp01(countdown, previousFrames);
            motion.motionScaleFrames = 10;
        }
    }
    motion.secondaryBlendFrameScale = weight;
    for (MotionNodeRuntime *motionNode = static_cast<MotionNodeRuntime *>(motion.secondaryNodes); motionNode && motionNode->nodeId != 0xff; ++motionNode)
        motionNode->interpolationAmount = 1.0f - weight;
    if (!MOTN_GetAnimationFlag(motion.secondaryAnimId)) {
        g_motnFrameCount = MOTN_GetFrameCountFromAnimId(motion.secondaryAnimId);
        g_motnMirror = (motion.motionFlags >> 20U) & 1U;
        g_motnReverse = 0;
        g_motnEntityEuler = nullptr;
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        const float frame = motion.secondaryFrameTimer;
        MotionNodeRuntime &root = *static_cast<MotionNodeRuntime *>(motion.secondaryNodes);
        g_RootMotionVector = {};
        g_pMotion->sourceNodes = nullptr;
        MOTN_readAndConvertPositionMOTNData(frame, root);
        MOTN_SampleSecondaryHierarchy(root, frame, false);
        Matrix4x4::Pop(1);
        auto *const source = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
        if (!source)
            return;
        auto *const secondary = static_cast<MotionNodeRuntime *>(motion.secondaryNodes);
        if (secondary->interpolationAmount != 1.0f && MOTN_GetAnimationFlag(motion.currentAnimId)) {
            const std::int32_t right = g_pMotion->rightHandRotationAngle;
            const std::int32_t left = g_pMotion->leftHandRotationAngle;
            g_pMotion->rightHandRotationAngle = 0;
            g_pMotion->leftHandRotationAngle = 0;
            RotateVirtualBones(secondary);
            g_pMotion->rightHandRotationAngle = right;
            g_pMotion->leftHandRotationAngle = left;
            return;
        }
        for (std::size_t index = 1; index < 10; ++index) {
            MotionNodeRuntime *found = kMotnTargetNodeIds[index] == source->nodeId ? source : nullptr;
            for (std::uint8_t child = 0; !found && child < source->childCount; ++child)
                found = SearchBinaryTreeRecursive(kMotnTargetNodeIds[index], source->children[child]);
            if (found)
                CaptureSecondaryTarget(index - 1, *found);
        }
        return;
    }
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    MOTN_UpdateRawSecondaryPose(motion, *static_cast<MotionNodeRuntime *>(motion.secondaryNodes));
    if (!MOTN_GetAnimationFlag(motion.currentAnimId)) {
        MOTN_CaptureEffectorsFromMatrices(static_cast<MotionNodeRuntime *>(motion.secondaryNodes));
        if (motion.sourceNodes) {
            for (std::size_t index = 1; index < 10; ++index) {
                if (MotionNodeRuntime *const found = FindTrackedNode(static_cast<MotionNodeRuntime *>(motion.sourceNodes), kMotnTargetNodeIds[index]))
                    CaptureSecondaryTarget(index - 1, *found);
            }
        }
    }
    Matrix4x4::Pop(1);
}

// The per-template rig programs (MOTN::motnPtr_const[].applyFunc).
void ApplyHumanRig(MotionNodeRuntime *node, std::size_t firstLookTarget, std::size_t secondLookTarget) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    TranslateEuler(node[10]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[11]);
    BuildBoneTransform(node[12]);
    node[13].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[14]);
    TranslateOnly(node[15]);
    AnklePrepass();
    TranslateEuler(node[16]);
    TranslateEuler(node[17]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[18]);
    AdoptMotionMatrix(node[19]);
    TranslateOnly(node[20]);
    AdjustLookTargets(&node[21], firstLookTarget, secondLookTarget);
    Matrix4x4_Push();
    ApplyHeadTransform(node[21]);
    AdoptMotionMatrix(node[22]);
    Matrix4x4_Push();
    TranslateOnly(node[23]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[24]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[25]);
    BuildBoneTransform(node[26]);
    node[27].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[28]);
    TranslateOnly(node[29]);
    TranslateEuler(node[30]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[31]);
    BuildBoneTransform(node[32]);
    node[33].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[34]);
    TranslateOnly(node[35]);
    TranslateEuler(node[36]);
}

void ApplyScriptedHuman35Rig(MotionNodeRuntime *node) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[10]);
    BuildBoneTransform(node[11]);
    node[12].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[13]);
    TranslateOnly(node[14]);
    AnklePrepass();
    TranslateEuler(node[15]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[16]);
    AdoptMotionMatrix(node[17]);
    TranslateOnly(node[18]);
    AdjustLookTargets(&node[19], 14, 8);
    Matrix4x4_Push();
    ApplyHeadTransform(node[19]);
    AdoptMotionMatrix(node[20]);
    Matrix4x4_Push();
    TranslateOnly(node[21]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[22]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[23]);
    BuildBoneTransform(node[24]);
    node[25].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[26]);
    TranslateOnly(node[27]);
    TranslateEuler(node[28]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[29]);
    BuildBoneTransform(node[30]);
    node[31].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[32]);
    TranslateOnly(node[33]);
    TranslateEuler(node[34]);
}

void ApplyCatRig(MotionNodeRuntime *node) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[10]);
    BuildBoneTransform(node[11]);
    node[12].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[13]);
    TranslateOnly(node[14]);
    AnklePrepass();
    TranslateEuler(node[15]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[16]);
    TranslateEuler(node[17]);
    TranslateEuler(node[18]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[19]);
    AdoptMotionMatrix(node[20]);
    TranslateOnly(node[21]);
    AdjustLookTargets(&node[22], 17, 11);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[22]);
    AdoptMotionMatrix(node[23]);
    TranslateOnly(node[24]);
    ApplyHeadTransform(node[25]);
    AdoptMotionMatrix(node[26]);
    Matrix4x4_Push();
    TranslateOnly(node[27]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[28]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[29]);
    BuildBoneTransform(node[30]);
    node[31].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[32]);
    TranslateOnly(node[33]);
    TranslateEuler(node[34]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[35]);
    BuildBoneTransform(node[36]);
    node[37].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[38]);
    TranslateOnly(node[39]);
    TranslateEuler(node[40]);
}

void ApplyDogLikeRig(MotionNodeRuntime *node) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[10]);
    BuildBoneTransform(node[11]);
    node[12].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[13]);
    TranslateOnly(node[14]);
    AnklePrepass();
    TranslateEuler(node[15]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[16]);
    TranslateEuler(node[17]);
    TranslateEuler(node[18]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[19]);
    AdoptMotionMatrix(node[20]);
    TranslateOnly(node[21]);
    AdjustLookTargets(&node[22], 19, 13);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[22]);
    AdoptMotionMatrix(node[23]);
    TranslateOnly(node[24]);
    ApplyHeadTransform(node[25]);
    AdoptMotionMatrix(node[26]);
    Matrix4x4_Push();
    TranslateOnly(node[27]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[28]);
    Matrix4x4_Push();
    TranslateEuler(node[29]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[30]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[31]);
    BuildBoneTransform(node[32]);
    node[33].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[34]);
    TranslateOnly(node[35]);
    TranslateEuler(node[36]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[37]);
    BuildBoneTransform(node[38]);
    node[39].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[40]);
    TranslateOnly(node[41]);
    TranslateEuler(node[42]);
}

void ApplyPnwRig(MotionNodeRuntime *node) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[10]);
    BuildBoneTransform(node[11]);
    node[12].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[13]);
    TranslateOnly(node[14]);
    AnklePrepass();
    TranslateEuler(node[15]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[16]);
    AdoptMotionMatrix(node[17]);
    TranslateOnly(node[18]);
    AdjustLookTargets(&node[19], 8, 14);
    Matrix4x4_Push();
    ApplyHeadTransform(node[19]);
    AdoptMotionMatrix(node[20]);
    Matrix4x4_Push();
    TranslateOnly(node[21]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[22]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[23]);
    BuildBoneTransform(node[24]);
    node[25].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[26]);
    TranslateOnly(node[27]);
    TranslateEuler(node[28]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[29]);
    BuildBoneTransform(node[30]);
    node[31].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[32]);
    TranslateOnly(node[33]);
    TranslateEuler(node[34]);
}

void ApplyYagRig(MotionNodeRuntime *node) {
    ApplyRootTransform(node[0]);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[1]);
    AdoptMotionMatrix(node[2]);
    TranslateOnly(node[3]);
    Matrix4x4_Push();
    TranslateEuler(node[4]);
    BuildBoneTransform(node[5]);
    node[6].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[7]);
    TranslateOnly(node[8]);
    AnklePrepass();
    TranslateEuler(node[9]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[10]);
    BuildBoneTransform(node[11]);
    node[12].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[13]);
    TranslateOnly(node[14]);
    AnklePrepass();
    TranslateEuler(node[15]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[16]);
    Matrix4x4::Pop(1);
    ApplyChestTransform(node[17]);
    AdoptMotionMatrix(node[18]);
    TranslateOnly(node[19]);
    AdjustLookTargets(&node[20], 20, 14);
    Matrix4x4_Push();
    ApplyTransformationWrapper(node[20]);
    AdoptMotionMatrix(node[21]);
    TranslateOnly(node[22]);
    ApplyHeadTransform(node[23]);
    AdoptMotionMatrix(node[24]);
    Matrix4x4_Push();
    TranslateOnly(node[25]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[26]);
    Matrix4x4_Push();
    TranslateEuler(node[27]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[28]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[29]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    TranslateEuler(node[30]);
    BuildBoneTransform(node[31]);
    node[32].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[33]);
    TranslateOnly(node[34]);
    TranslateEuler(node[35]);
    Matrix4x4::Pop(1);
    TranslateEuler(node[36]);
    BuildBoneTransform(node[37]);
    node[38].bindPositionPacked[1] |= 2U;
    AdoptMotionMatrix(node[39]);
    TranslateOnly(node[40]);
    TranslateEuler(node[41]);
}

void MOTN_ApplyRigProgram(std::int32_t templateIndex, MotionNodeRuntime *motionNodes) {
    switch (templateIndex) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
    case 10:
    case 12:
    case 13:
        ApplyHumanRig(motionNodes, 14, 8);
        break;
    case 4:
    case 5:
    case 7:
        ApplyHumanRig(motionNodes, 8, 14);
        break;
    case 9:
    case 11:
    case 14:
    case 15:
        ApplyScriptedHuman35Rig(motionNodes);
        break;
    case 16:
        ApplyCatRig(motionNodes);
        break;
    case 17:
        ApplyYagRig(motionNodes);
        break;
    case 18:
    case 20:
        ApplyDogLikeRig(motionNodes);
        break;
    case 19:
        ApplyPnwRig(motionNodes);
        break;
    default:
        break;
    }
}

void RunMotionCallbacks(MotionInformationWork &motion) {
    if (motion.mobjCallback)
        reinterpret_cast<MotionEvaluationCallback>(motion.mobjCallback)(&motion, motion.mobjCallbackUser);
    if (motion.motionCallback)
        reinterpret_cast<MotionEvaluationCallback>(motion.motionCallback)(&motion, motion.motionCallbackUser);
}

// Returns false when a failed pose evaluation aborts the evaluator (stage >= 2).
bool StorePoseEvaluation(MotionInformationWork &motion, bool succeeded) {
    motion.poseEvaluationSucceeded = succeeded;
    if (succeeded)
        return true;
    const auto stage = static_cast<std::int8_t>(motion.poseEvaluationStage);
    if (stage >= 2) {
        motion.updateMode = 2;
        return false;
    }
    if (stage == 1)
        motion.updateMode = 1;
    return true;
}

void PublishActiveClock(MotionInformationWork &motion) {
    PublishMotionClock(&motion.entityEuler, motion.motionFlags & 0x20U, MOTN_GetFrameCountFromAnimId(motion.currentAnimId), motion.frameTimerRef,
        motion.frameStart, motion.frameEnd, (motion.motionFlags >> 17U) & 1U, motion.residualFrameTime);
}

bool ApplyActiveNodes(MotionInformationWork &motion) {
    return MOTN_ApplyFKPoseWithRootMotion(motion, static_cast<MotionNodeRuntime *>(motion.activeNodes), motion.frameTimer, &motion.rootPreTranslate,
        static_cast<std::int8_t>(motion.poseEvaluationStage), &motion.rootOffsetPosition);
}

bool ApplyPreviousNodes(MotionInformationWork &motion) {
    PublishMotionClock(&motion.nextEntityEuler, motion.motionFlags & 0x20U, MOTN_GetFrameCountFromAnimId(motion.requestedAnimId), motion.previousFrameTimerRef,
        motion.previousFrameStart, motion.previousFrameEnd, (motion.motionFlags >> 18U) & 1U, motion.previousResidualFrameTime);
    return MOTN_ApplyFKPoseWithRootMotion(motion, static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), motion.previousFrameTimer,
        &motion.rootPreTranslate, static_cast<std::int8_t>(motion.poseEvaluationStage), &motion.previousRootOffsetPosition);
}

void AlignBlendNodes(MotionInformationWork &motion, MotionNodeRuntime *reference, MotionNodeRuntime *motionNodes) {
    if ((motion.motionFlags & 0x40000000U) != 0)
        MOTN_AlignBlendNodesToTrackedNodeXZ(reference, motionNodes, motion.trackedNodeIndex);
    else
        MOTN_AlignBlendNodesToTrackedNode(reference, motionNodes, motion.trackedNodeIndex);
}

void ApplyNodeConstraintPasses(MotionInformationWork &motion) {
    MOTN_ApplyNodeOffsetPairs(motion);
    MOTN_SolveNodeConstraints(motion);
    MOTN_ApplyNodeConstraintCorrections(motion);
}

// The standard blend ramp: blendTargetMotionNodes take (total - remaining) / total after the countdown step.
void StepBlendRamp(MotionInformationWork &motion) {
    if (motion.blendFramesRemaining)
        --motion.blendFramesRemaining;
    const float ramp = ComputeLinearRamp01(motion.blendFramesTotal - motion.blendFramesRemaining, motion.blendFramesTotal);
    for (MotionNodeRuntime *motionNode = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes); motionNode && motionNode->nodeId != 0xff; ++motionNode)
        motionNode->interpolationAmount = ramp;
}

// Advances the active clock by the playback rate; the end latch is STATE_SHOULD_SKIP.
void AdvanceActiveClock(MotionInformationWork &motion, float delta) {
    if (MaintainFrameRate(motion.frameTimer, motion.frameTimerRef, motion.residualFrameTime, motion.previousAnimId == 0xffffU,
            (motion.motionFlags & 0x20U) != 0, motnGetRootMotionEndMarker(motion.currentAnimId) != 0, motion.frameStart, motion.frameEnd,
            MOTN_GetSmoothedFrameDelta(delta))) {
        motion.motionFlags |= 2U;
    }
}

// Advances the previous-pose clock (the inlined no-reset maintainFrameRate of IdleMaybe mode 2 and Unknown_1).
void AdvancePreviousClock(MotionInformationWork &motion, bool resetMode, float delta) {
    MaintainFrameRate(motion.previousFrameTimer, motion.previousFrameTimerRef, motion.previousResidualFrameTime, resetMode, (motion.motionFlags & 0x20U) != 0,
        motnGetRootMotionEndMarker(motion.requestedAnimId) != 0, motion.previousFrameStart, motion.previousFrameEnd, delta);
}

// MOTN_EvaluateActiveMotion: interpolation mode 0 (active pose only).
bool MOTN_EvaluateActiveMotion(MotionInformationWork &motion) {
    PublishActiveClock(motion);
    if (!StorePoseEvaluation(motion, ApplyActiveNodes(motion)))
        return false;
    auto *const activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    MOTN_AlignNodesToTrackedNode(motion, activeMotionNodes);
    ApplyNodeConstraintPasses(motion);
    RunMotionCallbacks(motion);
    ApplyArmTargetsAndLengthClamp_MOTN(nullptr, static_cast<MotionNodeRuntime *>(motion.activeNodes));
    if (motion.secondaryAnimId || motion.updateMode)
        MOTN_ApplyBoneHierarchyTransform(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.activeNodes));
    else
        MOTN_ApplyRigProgram(motion.basePoseTemplateIndex, static_cast<MotionNodeRuntime *>(motion.activeNodes));
    MOTN_SetNewMatrices(nullptr);
    AdvanceActiveClock(motion, motion.playbackRate);
    return true;
}

// TranslationMaybe (MOTN_EvaluateStandardBlend): the standard blend from the frozen previous pose.
bool MOTN_EvaluateStandardBlend(MotionInformationWork &motion) {
    PublishActiveClock(motion);
    if (!StorePoseEvaluation(motion, ApplyActiveNodes(motion)))
        return false;
    auto *const activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *const blendMotionNodes = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    MOTN_AlignNodesToTrackedNode(motion, activeMotionNodes);
    AlignBlendNodes(motion, activeMotionNodes, blendMotionNodes);
    ApplyNodeConstraintPasses(motion);
    StepBlendRamp(motion);
    g_motnNextEntityEuler = &motion.nextEntityEuler;
    RunMotionCallbacks(motion);
    ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    Ryo_RecursiveIKForRealtimeMovement(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    MOTN_SetNewMatrices(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    if (!motion.blendFramesRemaining)
        motion.motionFlags &= ~0x10000U;
    AdvanceActiveClock(motion, motion.playbackRate);
    return true;
}

// IdleMaybe (MOTN_EvaluatePreviousPoseBlend): modes 2, 5, 6 and 9 keep sampling the previous animation on its own clock while the
// new one holds its first frame.
bool MOTN_EvaluatePreviousPoseBlend(MotionInformationWork &motion) {
    PublishActiveClock(motion);
    ApplyActiveNodes(motion);
    if (!StorePoseEvaluation(motion, ApplyPreviousNodes(motion)))
        return false;
    const std::uint8_t mode = motion.interpolationMode;
    if (mode != 6 && mode != 9) {
        MOTN_AlignNodesToTrackedNode(motion, static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
        AlignBlendNodes(motion, static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    }
    ApplyNodeConstraintPasses(motion);
    StepBlendRamp(motion);
    g_motnEntityEuler = &motion.entityEuler;
    g_motnNextEntityEuler = &motion.nextEntityEuler;
    RunMotionCallbacks(motion);
    ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    Ryo_RecursiveIKForRealtimeMovement(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    MOTN_SetNewMatrices(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    if (!motion.blendFramesRemaining)
        motion.motionFlags &= ~0x10000U;
    const std::uint8_t currentMode = motion.interpolationMode;
    if (currentMode == 2) {
        AdvancePreviousClock(motion, false, MOTN_GetSmoothedFrameDelta(motion.smoothedFrameDelta));
        return true;
    }
    if (((currentMode - 5U) & 0xfbU) == 0) {
        AdvancePreviousClock(motion, (motion.rootAngleFlags & 0x1000U) != 0, MOTN_GetSmoothedFrameDelta(motion.smoothedFrameDelta));
        return true;
    }
    if (currentMode == 6) {
        if (MaintainFrameRate(motion.previousFrameTimer, motion.previousFrameTimerRef, motion.previousResidualFrameTime, (motion.rootAngleFlags & 0x1000U) != 0,
                (motion.motionFlags & 0x20U) != 0, motnGetRootMotionEndMarker(motion.requestedAnimId) != 0, motion.previousFrameStart, motion.previousFrameEnd,
                MOTN_GetSmoothedFrameDelta(motion.smoothedFrameDelta))) {
            const MotionNodeRuntime &blendRoot = *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
            motion.rootBlendScratch[0] = blendRoot.effectorPosition.x;
            motion.rootBlendScratch[2] = blendRoot.effectorPosition.z;
        }
    }
    return true;
}

// Unknown_1 (MOTN_EvaluateFrameCarryBlend): mode 3 advances both clocks, carrying the previous animation to its end.
bool MOTN_EvaluateFrameCarryBlend(MotionInformationWork &motion) {
    PublishActiveClock(motion);
    if (!StorePoseEvaluation(motion, ApplyActiveNodes(motion)))
        return false;
    ApplyPreviousNodes(motion);
    auto *const activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *const blendMotionNodes = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    MOTN_AlignNodesToTrackedNode(motion, activeMotionNodes);
    AlignBlendNodes(motion, activeMotionNodes, blendMotionNodes);
    ApplyNodeConstraintPasses(motion);
    StepBlendRamp(motion);
    g_motnNextEntityEuler = &motion.nextEntityEuler;
    g_motnEntityEuler = &motion.entityEuler;
    RunMotionCallbacks(motion);
    ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    Ryo_RecursiveIKForRealtimeMovement(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    MOTN_SetNewMatrices(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    if (!motion.blendFramesRemaining)
        motion.motionFlags &= ~0x10000U;
    AdvancePreviousClock(motion, false, MOTN_GetSmoothedFrameDelta(motion.smoothedFrameDelta));
    AdvanceActiveClock(motion, motion.playbackRate);
    return true;
}

// RunningMaybe (MOTN_EvaluateTransitionNodesBlend): mode 4 blends toward the transition (lookup) motion sampled on the active clock
// scaled by the two motions' root-motion lengths.
bool MOTN_EvaluateTransitionNodesBlend(MotionInformationWork &motion) {
    PublishActiveClock(motion);
    if (!StorePoseEvaluation(motion, ApplyActiveNodes(motion)))
        return false;
    auto *const transitionMotionNodes = static_cast<MotionNodeRuntime *>(motion.transitionNodes);
    const float weight = 1.0f - motion.interpolationAmount;
    for (MotionNodeRuntime *motionNode = transitionMotionNodes; motionNode && motionNode->nodeId != 0xff; ++motionNode)
        motionNode->interpolationAmount = weight;
    const std::uint16_t lookupAnimId = motion.lookupAnimId;
    const float lookupLength = static_cast<float>(MOTN_GetFrameCountFromAnimId(lookupAnimId) - motnGetRootMotionEndMarker(lookupAnimId));
    const float scale =
        lookupLength / static_cast<float>(MOTN_GetFrameCountFromAnimId(motion.currentAnimId) - motnGetRootMotionEndMarker(motion.currentAnimId));
    g_motnFrameCount = MOTN_GetFrameCountFromAnimId(lookupAnimId);
    g_motnReverse = motion.motionFlags & 0x20U;
    g_motnMirror = (motion.motionFlags >> 19U) & 1U;
    g_motnTimerRef = scale * motion.frameTimerRef;
    g_motnFrameStart = scale * motion.frameStart;
    g_motnFrameEnd = scale * motion.frameEnd;
    g_motnResidualTime = scale * motion.residualFrameTime;
    MOTN_ApplyFKPoseWithRootMotion(motion, transitionMotionNodes, scale * motion.frameTimer, &motion.rootPreTranslate,
        static_cast<std::int8_t>(motion.poseEvaluationStage), &motion.previousRootOffsetPosition);
    auto *const activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    MOTN_AlignNodesToTrackedNode(motion, activeMotionNodes);
    if ((static_cast<std::uint32_t>(motion.trackedNodeIndex) & 0x7fffffffU) < 10U)
        AlignBlendNodes(motion, activeMotionNodes, static_cast<MotionNodeRuntime *>(motion.transitionNodes));
    g_motnNextEntityEuler = &motion.entityEuler;
    RunMotionCallbacks(motion);
    ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.transitionNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    Ryo_RecursiveIKForRealtimeMovement(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.transitionNodes));
    MOTN_SetNewMatrices(static_cast<MotionNodeRuntime *>(motion.transitionNodes));
    const float ratio =
        static_cast<float>(MOTN_GetFrameCountFromAnimId(motion.currentAnimId)) / static_cast<float>(MOTN_GetFrameCountFromAnimId(motion.lookupAnimId));
    const double delta = (static_cast<double>(ratio) - 1.0) * static_cast<double>(motion.interpolationAmount) + static_cast<double>(motion.playbackRate);
    AdvanceActiveClock(motion, static_cast<float>(delta));
    return true;
}

// ApplyBasePose: makes the requested animation active, turning the previous active rows into the blend source.
std::int32_t ApplyBasePose(MotionInformationWork &motion, std::uint16_t animationId) {
    motion.previousFrameTimer = motion.frameTimer;
    motion.previousFrameStart = motion.frameStart;
    motion.previousFrameEnd = motion.frameEnd;
    motion.previousResidualFrameTime = motion.residualFrameTime;
    motion.requestedAnimId = motion.currentAnimId;
    motion.smoothedFrameDelta = motion.previousPlaybackRate;
    const float timerRef = motion.frameTimerRef;
    motion.previousFrameTimerRef = timerRef;
    motion.previousPlaybackRate2 = timerRef;
    motion.frameTimerRef = -1.0f;
    motion.frameTimer = 0.0f;
    motion.rootBlendScratch[0] = motion.previousRootPreTranslate.x;
    motion.rootBlendScratch[1] = motion.previousRootPreTranslate.y;
    motion.rootBlendScratch[2] = motion.previousRootPreTranslate.z;
    g_pMotion = &motion;
    g_motnBasePoseOnly = 1;
    motion.motionFlags = (motion.motionFlags & 0x20000U) != 0 ? motion.motionFlags | 0x40000U : motion.motionFlags & ~0x40000U;
    if (motion.previousAnimId == 0xffffU)
        motion.rootAngleFlags |= 0x1000U;
    else
        motion.rootAngleFlags &= ~0x1000U;
    if (!MOTN_GetDataForAnimId(animationId))
        animationId = kBasePoseAnimationIds[motion.basePoseTemplateIndex];
    motion.currentAnimId = animationId;
    const MotnAnimationHeader *const animation = MOTN_GetDataForAnimId(animationId);
    std::uint16_t chained = 0;
    if (const std::uint8_t *const record = shenLookupAnimationIndex(animationId, 1))
        std::memcpy(&chained, record + 2, sizeof(chained));
    motion.previousAnimId = chained;
    float frameCount = static_cast<float>(MOTN_GetFrameCountFromAnimId(animationId));
    if (motnGetRootMotionEndMarker(animationId))
        frameCount = static_cast<float>(static_cast<double>(frameCount) - 0.0001);
    motion.residualFrameTime = 0.0f;
    motion.frameStart = motion.scriptStartFrame;
    if (motion.scriptStartFrame >= frameCount)
        motion.frameStart = frameCount - 1.0f;
    motion.frameEnd = motion.scriptEndFrame;
    if (motion.scriptEndFrame == 0.0f || motion.scriptEndFrame >= frameCount)
        motion.frameEnd = frameCount - 1.0f;
    motion.frameTimer = (motion.motionFlags & 0x20U) != 0 ? motion.frameEnd : motion.frameStart;
    const float reference = motion.frameTimer - motion.playbackRate;
    motion.frameTimerRef = reference < 0.0f ? -1.0f : reference;
    std::swap(motion.activeNodesCookie, motion.blendTargetNodesCookie);
    std::swap(motion.activeNodes, motion.blendTargetNodes);
    motion.scriptStartFrame = 0.0f;
    motion.scriptEndFrame = 0.0f;
    motion.previousRootOffsetPosition = motion.rootOffsetPosition;
    motion.requestedInterpolationMode = motion.blendMode;
    if (motion.blendMode == 11)
        motion.requestedInterpolationMode = g_motnCubicScratch ? 11 : 1;
    auto *blendMotionNodes = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    if ((motion.motionFlags & 0x10000040U) != 0x40U || blendMotionNodes->nodeId == 0xff) {
        motion.motionFlags &= ~0x10000U;
    } else {
        if ((motion.motionFlags & 0x10000U) != 0) {
            g_motnIgnoreArmIK = 1;
            motion.requestedInterpolationMode = 1;
            if (!MOTN_GetAnimationFlag(motion.requestedAnimId) && !MOTN_GetAnimationFlag(animationId)) {
                MOTN_CaptureEffectorsFromMatrices(blendMotionNodes);
                if (MotionNodeRuntime *const look = FindMotionNodeFrom(blendMotionNodes, 6))
                    SetPosition(look->effectorPosition, motion.rootOffsetPosition);
            }
            if (MOTN_GetAnimationFlag(motion.requestedAnimId) && MOTN_GetAnimationFlag(motion.currentAnimId))
                RotateVirtualBones(blendMotionNodes);
        }
        if (MOTN_GetAnimationFlag(motion.requestedAnimId) && !MOTN_GetAnimationFlag(motion.currentAnimId)) {
            MOTN_CaptureEffectorsFromMatrices(blendMotionNodes);
            if (MotionNodeRuntime *const look = FindMotionNodeFrom(blendMotionNodes, 6))
                SetPosition(look->effectorPosition, motion.worldTargets[2] ? *motion.worldTargets[2] : motion.rootOffsetPosition);
        }
        if (!MOTN_GetAnimationFlag(motion.requestedAnimId) && MOTN_GetAnimationFlag(motion.currentAnimId))
            RotateVirtualBones(blendMotionNodes);
        const std::uint8_t requested = motion.requestedInterpolationMode;
        motion.motionFlags |= 0x10000U;
        if (static_cast<std::uint8_t>(requested - 2U) <= 1U)
            motion.blendFramesTotal =
                static_cast<std::uint16_t>(static_cast<std::int32_t>((motion.previousFrameEnd - motion.previousFrameTimer) / motion.smoothedFrameDelta));
        else
            motion.blendFramesTotal = motion.blendFrameCount;
    }
    motion.motionFlags &= ~0x10000000U;
    motion.blendFramesRemaining = motion.blendFramesTotal;
    motion.nextEntityEuler = motion.previousEntityEuler;
    motion.previousTurnEulerDelta = motion.lastTurnEulerDelta;
    motion.activeTurnEulerDelta = motion.pendingTurnEulerDelta;
    motion.pendingTurnEulerDelta = {};
    const std::uint32_t flags = motion.motionFlags;
    const bool mirrored =
        (flags & 0x1000000U) != 0 ? (flags & 0x80U) != 0 : ((flags >> 7U) & 1U) != (shenLookupAnimationIndex(motion.currentAnimId, 19) != nullptr ? 1U : 0U);
    motion.motionFlags = mirrored ? flags | 0x20000U : flags & ~0x20000U;
    auto *const motionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *keyStorage = reinterpret_cast<MotnKeyframeTriplet *>(motion.activeNodesCookie);
    if ((animation->sourceRig & 0x8000U) == 0) {
        motion.motionFlags &= ~0x1000U;
        MOTN_LoadAnimation(motion, motionNodes, animationId, static_cast<std::uint16_t>(motion.basePoseTemplateIndex), keyStorage,
            static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
        if ((motion.motionFlags & 0x20000U) != 0)
            MOTN_IncrementDataOffsets(motionNodes);
        motion.motionFlags &= ~1U;
        return 0;
    }
    motion.motionFlags |= 0x1000U;
    MOTN_LoadAnimation(motion, motionNodes, animationId, static_cast<std::uint16_t>(motion.basePoseTemplateIndex), keyStorage,
        static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    return 2;
}

// Matrix4x4_RecursiveRotateMatrix: re-expresses a subtree relative to the current work matrix, then applies scaledFrame.
void Matrix4x4_RecursiveRotateMatrix(MotionNodeRuntime &node, const Matrix4x4 &scaledFrame) {
    Matrix4x4_Push();
    Matrix4x4::Multiply(node.transform);
    *MotionNodeMatrix(node) = workTransformMatrix;
    workTransformMatrix = scaledFrame;
    Matrix4x4::Multiply(node.transform);
    *MotionNodeMatrix(node) = workTransformMatrix;
    Matrix4x4::Pop(1);
    for (std::uint8_t child = 0; child < node.childCount; ++child)
        Matrix4x4_RecursiveRotateMatrix(*node.children[child], scaledFrame);
}

// ApplyBoneTransformRecursive: premultiplies every row matrix of a subtree by transform.
void ApplyBoneTransformRecursive(MotionNodeRuntime &node, const Matrix4x4 *transform) {
    Matrix4x4_Push();
    workTransformMatrix = transform ? *transform : *g_matrixStack.top.stack;
    Matrix4x4::Multiply(node.transform);
    *MotionNodeMatrix(node) = workTransformMatrix;
    Matrix4x4::Pop(1);
    for (std::uint8_t child = 0; child < node.childCount; ++child)
        ApplyBoneTransformRecursive(*node.children[child], transform);
}

// AddFloatToIntArray: offsets every row matrix translation of a subtree.
void AddFloatToIntArray(MotionNodeRuntime &node, const Vector3 &offset) {
    node.transform->rows[3].x = node.transform->rows[3].x + offset.x;
    node.transform->rows[3].y = offset.y + node.transform->rows[3].y;
    node.transform->rows[3].z = offset.z + node.transform->rows[3].z;
    for (std::uint8_t child = 0; child < node.childCount; ++child)
        AddFloatToIntArray(*node.children[child], offset);
}

// Captures a row's evaluated translation for the ankle release/knee scratch (mainMotionUpdate, before PlayMotionForRyo).
MotionNodeRuntime *CaptureAnkleScratch(MotionNodeRuntime *motionNode, std::uint8_t nodeId, std::int32_t &valid, Vector3 &position) {
    MotionNodeRuntime *const found = FindMotionNodeFrom(motionNode, nodeId);
    if (found && (found->bindPositionPacked[1] & 2U) != 0) {
        valid = 1;
        position = {found->transform->rows[3].x, found->transform->rows[3].y, found->transform->rows[3].z};
    }
    return found;
}

void ClearEvaluatedFlags(MotionNodeRuntime *motionNodes) {
    for (MotionNodeRuntime *motionNode = motionNodes; motionNode && motionNode->nodeId != 0xff; ++motionNode)
        motionNode->bindPositionPacked[1] &= ~2U;
}

// RotateAroundAxis_PrecomputedTrig: multiplies the work matrix by the rotation about a unit axis given its sine and cosine.
void RotateAroundAxis_PrecomputedTrig(const Vector3 &axis, float sine, float cosine) {
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;
    const float xz = (z * x) * (1.0f - cosine);
    const float xy = (y * x) * (1.0f - cosine);
    const float yz = (z * y) * (1.0f - cosine);
    Matrix4x4 rotation{};
    rotation.rows[0] = {((1.0f - x * x) * cosine) + x * x, z * sine + xy, xz - y * sine, 0.0f};
    rotation.rows[1] = {xy - z * sine, ((1.0f - y * y) * cosine) + y * y, x * sine + yz, 0.0f};
    rotation.rows[2] = {y * sine + xz, yz - x * sine, ((1.0f - z * z) * cosine) + z * z, 0.0f};
    rotation.rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    Matrix4x4::Multiply(&rotation);
}

// Matrix4x4_BlendRotateByAngle: rotates the start X axis toward the end X axis by the blend share of the angle between them, then
// rebuilds an orthonormal basis from the linearly blended Y axis.
bool Matrix4x4_BlendRotateByAngle(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end) {
    const float s0 = start.rows[0].x;
    const float s1 = start.rows[1].x;
    const float s2 = start.rows[2].x;
    const float e0 = end.rows[0].x;
    const float e1 = end.rows[1].x;
    const float e2 = end.rows[2].x;
    float axisX = s1 * e2 - s2 * e1;
    float axisY = e0 * s2 - s0 * e2;
    float axisZ = s0 * e1 - e0 * s1;
    const float length = std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
    const float dot = (s0 * e0 + e1 * s1) + s2 * e2;
    float x = s0;
    float y = s1;
    float z = s2;
    if (static_cast<float>(std::fabs(static_cast<double>(length))) > 0x1p-23f || dot <= 0.0f) {
        float angle = 32768.0f;
        if (static_cast<float>(std::fabs(static_cast<double>(length))) > 0x1p-23f) {
            angle = static_cast<float>(static_cast<std::int32_t>(Shenmue_Atan2Fixed(length, dot)));
        } else {
            axisX = t * end.rows[0].y + (1.0f - t) * start.rows[0].y;
            axisY = t * end.rows[1].y + (1.0f - t) * start.rows[1].y;
            axisZ = t * end.rows[2].y + (1.0f - t) * start.rows[2].y;
        }
        const auto turn = static_cast<std::int32_t>(angle * t);
        const float axisLength = std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
        if (0x1p-23f > axisLength)
            return false;
        const Vector3 axis{axisX / axisLength, axisY / axisLength, axisZ / axisLength};
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        const auto magnitude = static_cast<std::uint16_t>(turn);
        const std::uint32_t folded = magnitude <= 0x8000U ? magnitude : 0x10000U - magnitude;
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
        RotateAroundAxis_PrecomputedTrig(axis, sin_fixed16(static_cast<std::int16_t>(turn)), cosine);
        const Matrix4x4 &m = workTransformMatrix;
        x = ((m.rows[1].x * s1 + m.rows[0].x * s0) + m.rows[2].x * s2) + m.rows[3].x;
        y = ((m.rows[1].y * s1 + m.rows[0].y * s0) + m.rows[2].y * s2) + m.rows[3].y;
        z = ((m.rows[0].z * s0 + m.rows[1].z * s1) + m.rows[2].z * s2) + m.rows[3].z;
        Matrix4x4_Push();
        Matrix4x4::Pop(1);
        Matrix4x4::Pop(1);
        const float norm = std::sqrt((y * y + x * x) + z * z);
        if (0x1p-23f > norm)
            return false;
        output.rows[0].x = x / norm;
        output.rows[1].x = y / norm;
        output.rows[2].x = z / norm;
    } else {
        output.rows[0].x = s0;
        output.rows[1].x = s1;
        output.rows[2].x = s2;
    }
    const double inverse = 1.0 - static_cast<double>(t);
    const auto m01 = static_cast<float>(static_cast<double>(start.rows[0].y) * inverse + static_cast<double>(t * end.rows[0].y));
    const auto m11 = static_cast<float>(static_cast<double>(start.rows[1].y) * inverse + static_cast<double>(t * end.rows[1].y));
    const auto m21 = static_cast<float>(static_cast<double>(start.rows[2].y) * inverse + static_cast<double>(t * end.rows[2].y));
    const float c2 = m11 * x - m01 * y;
    const float c1 = m01 * z - m21 * x;
    const float c0 = m21 * y - m11 * z;
    const float n0 = c1 * z - c2 * y;
    const float n1 = c2 * x - c0 * z;
    const float n2 = c0 * y - c1 * x;
    const float normalLength = std::sqrt((n1 * n1 + n0 * n0) + n2 * n2);
    if (!(std::fabs(static_cast<double>(normalLength)) > 1.1920928955078125e-07))
        return false;
    output.rows[0].y = n0 / normalLength;
    output.rows[1].y = n1 / normalLength;
    output.rows[2].y = n2 / normalLength;
    const float crossLength = std::sqrt((c1 * c1 + c0 * c0) + c2 * c2);
    if (!(std::fabs(static_cast<double>(crossLength)) > 1.1920928955078125e-07))
        return false;
    output.rows[3].w = 1.0f;
    output.rows[2].w = 0.0f;
    output.rows[3].x = 0.0f;
    output.rows[1].w = 0.0f;
    output.rows[0].w = 0.0f;
    output.rows[3].y = 0.0f;
    output.rows[3].z = 0.0f;
    output.rows[0].z = c0 / crossLength;
    output.rows[1].z = c1 / crossLength;
    output.rows[2].z = c2 / crossLength;
    return true;
}

// Matrix4x4_RotateAndBlend3D: blends two frames by first removing their yaw difference (tracked across a turn in
// MOTN::NeedsRotation passes), blending the remainder, then re-applying the unblended share of the yaw.
bool Matrix4x4_RotateAndBlend3D(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end) {
    float ax = start.rows[0].x;
    float az = start.rows[2].x;
    const float startLength = std::sqrt((ax * ax + 0.0f) + az * az);
    if (0x1p-23f > startLength)
        return false;
    ax /= startLength;
    az /= startLength;
    float bx = end.rows[0].x;
    float bz = end.rows[2].x;
    const float endLength = std::sqrt((bx * bx + 0.0f) + bz * bz);
    if (0x1p-23f > endLength)
        return false;
    bx /= endLength;
    bz /= endLength;
    float sine = bx * az - bz * ax;
    const float cosine = bz * az + bx * ax;
    const auto magnitude = static_cast<float>(std::fabs(static_cast<double>(sine)));
    std::int32_t angle = 0;
    if (static_cast<float>(std::fabs(static_cast<double>(magnitude))) > 0x1p-23f) {
        angle = static_cast<std::int32_t>(Shenmue_Atan2Fixed(magnitude, cosine));
    } else if (cosine <= 0.0f) {
        angle = 0x8000;
        sine = 1.0f;
    }
    std::int32_t previousAngle = g_motnBlendAngleDifference;
    float previousSine = g_motnBlendAngleSine;
    if (!g_motnNeedsRotation) {
        g_motnBlendAngleSine = sine;
        g_motnBlendAngleDifference = angle;
        previousAngle = angle;
        previousSine = sine;
    }
    if (angle > 0x4000 && 0.0f > end.rows[1].y * start.rows[1].y)
        angle -= 0x8000;
    if (g_motnNeedsRotation && previousAngle > 0x5000 && 0.0f > previousSine * sine)
        angle -= 0x10000;
    if (0.0f > sine)
        angle = -angle;
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(-angle, 0, 0);
    Matrix4x4::Multiply(&start);
    const Matrix4x4 unturned = workTransformMatrix;
    Matrix4x4_BlendRotateByAngle(output, t, unturned, end);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(static_cast<std::int32_t>((1.0f - t) * static_cast<float>(angle)), 0, 0);
    Matrix4x4::Multiply(&output);
    output = workTransformMatrix;
    Matrix4x4::Pop(1);
    return true;
}

// GenerateBlendedTransformationMatrix: the cubic-sequence counterpart of VectorTransformWithRotationLerp.
void GenerateBlendedTransformationMatrix(float t, const MotionNodeRuntime &start, const MotionNodeRuntime &end) {
    Matrix4x4::preTranslate(LerpDouble(start.effectorPosition.x, end.effectorPosition.x, t), LerpDouble(start.effectorPosition.y, end.effectorPosition.y, t),
        LerpDouble(start.effectorPosition.z, end.effectorPosition.z, t));
    const std::uint32_t needsRotation = g_motnNeedsRotation;
    if (needsRotation) {
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
    }
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    if (needsRotation && g_motnNextEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(start.bindEuler.y, start.bindEuler.x, start.bindEuler.z);
    g_motnBlendStart = workTransformMatrix;
    workTransformMatrix = transformMatrix_prePass;
    if (needsRotation && g_motnEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(end.bindEuler.y, end.bindEuler.x, end.bindEuler.z);
    g_motnBlendEnd = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4 blended{};
    const bool valid = needsRotation ? Matrix4x4_RotateAndBlend3D(blended, t, g_motnBlendStart, g_motnBlendEnd) :
                                       Matrix4x4_BlendRotateByAngle(blended, t, g_motnBlendStart, g_motnBlendEnd);
    Matrix4x4::Multiply(valid ? &blended : &g_motnBlendEnd);
    g_motnNeedsRotation = 0;
}

// SetInterpAmountForAllKeyframesRecursively.
void SetInterpAmountForAllKeyframesRecursively(MotionNodeRuntime *motionNode, float amount) {
    if (!motionNode)
        return;
    motionNode->interpolationAmount = amount;
    for (std::uint8_t child = 0; child < motionNode->childCount; ++child)
        SetInterpAmountForAllKeyframesRecursively(motionNode->children[child], amount);
}

// MOTN_ProcessSequenceInterpolation: sets the per-limb blend amounts of a cubic sequence step, stretching the blend when the root
// has to rise or drop, and samples the lift curve for the root and the two hands.
void MOTN_ProcessSequenceInterpolation(MotionInformationWork &motion) {
    g_motnSequenceLiftCurve = {.present = 5,
        .keyframeCount = 0,
        .reserved04 = g_motnSequenceLiftCurve.reserved04,
        .block3Data = kMotnSequenceLiftFrames.data(),
        .sizeData = g_motnSequenceLiftCurve.sizeData,
        .block4Data = kMotnSequenceLiftTypes.data(),
        .block5Begin = kMotnSequenceLiftValues.data(),
        .block5Cursor = nullptr,
        .block5End = nullptr};
    auto *activeMotionNodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *blendMotionNodes = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    const float total = static_cast<float>(motion.blendFramesTotal);
    float progress = (total - static_cast<float>(motion.blendFramesRemaining)) / total;
    progress = progress + progress;
    const float dx = activeMotionNodes->effectorPosition.x - blendMotionNodes->effectorPosition.x;
    const float dy = activeMotionNodes->effectorPosition.y - blendMotionNodes->effectorPosition.y;
    const float dz = activeMotionNodes->effectorPosition.z - blendMotionNodes->effectorPosition.z;
    const float rootDistance = std::sqrt((dy * dy + dx * dx) + dz * dz);
    MotionNodeRuntime *const head = FindMotionNodeFrom(blendMotionNodes, 4);
    MotionNodeRuntime *const firstLeg = FindMotionNodeFrom(blendMotionNodes, g_cubicSequenceDirection ? 14 : 8);
    MotionNodeRuntime *const secondLeg = FindMotionNodeFrom(blendMotionNodes, g_cubicSequenceDirection ? 8 : 14);
    MotionNodeRuntime *const leadArm = FindMotionNodeFrom(blendMotionNodes, g_cubicSequenceDirection ? 30 : 23);
    MotionNodeRuntime *const trailArm = FindMotionNodeFrom(blendMotionNodes, g_cubicSequenceDirection ? 23 : 30);
    const bool initialize = g_initializeCubicSequence;
    if (initialize) {
        MotionNodeRuntime *const activeLook = FindMotionNodeFrom(activeMotionNodes, 6);
        MotionNodeRuntime *const blendLook = FindMotionNodeFrom(blendMotionNodes, 6);
        SetPosition(activeLook->effectorPosition, motion.rootOffsetPosition);
        SetPosition(blendLook->effectorPosition, motion.previousRootOffsetPosition);
    }
    const float blendRise = blendMotionNodes->effectorPosition.y - motion.rootBlendScratch[1];
    const float activeRise = static_cast<MotionNodeRuntime *>(motion.activeNodes)->effectorPosition.y - motion.rootPreTranslate.y;
    const auto riseDifference = static_cast<float>(std::fabs(static_cast<double>(blendRise - activeRise)));
    const float threshold =
        (MOTN_GetRigType(static_cast<std::uint16_t>(motion.basePoseTemplateIndex))->nodes[0].position.y * 0.5f) / MOTN_GetRigType(0)->nodes[0].position.y;
    float amount = 0.0f;
    if (threshold > riseDifference) {
        amount = 1.0f < progress * 0.5f ? 1.0f : progress * 0.5f;
    } else if (activeRise - blendRise >= 0.0f) {
        if (initialize) {
            const auto frames = static_cast<std::uint16_t>(motion.blendFrameCount * 3U);
            motion.blendFrameCount = frames;
            motion.blendFramesTotal = frames;
            motion.blendFramesRemaining = frames;
            if (frames)
                motion.blendFramesRemaining = static_cast<std::uint16_t>(frames - 1U);
            progress = (static_cast<float>(frames) - static_cast<float>(motion.blendFramesRemaining)) / static_cast<float>(frames);
            progress = progress + progress;
        }
        amount = progress - 1.0f;
        if (0.0f > amount)
            amount = 0.0f;
    } else {
        if (initialize) {
            const auto frames = static_cast<std::uint16_t>(static_cast<std::int64_t>(static_cast<float>(motion.blendFrameCount) * 1.5f));
            motion.blendFrameCount = frames;
            motion.blendFramesTotal = frames;
            motion.blendFramesRemaining = frames;
            if (frames)
                motion.blendFramesRemaining = static_cast<std::uint16_t>(frames - 1U);
            progress = (static_cast<float>(frames) - static_cast<float>(motion.blendFramesRemaining)) / static_cast<float>(frames);
            progress = progress + progress;
        }
        amount = 1.0f < progress * 0.5f ? 1.0f : progress * 0.5f;
    }
    for (MotionNodeRuntime *motionNode = blendMotionNodes; motionNode && motionNode->nodeId != 0xff; ++motionNode)
        motionNode->interpolationAmount = amount;
    const float half = progress * 0.5f;
    const float limb = half > 1.0f ? 1.0f : half;
    SetInterpAmountForAllKeyframesRecursively(head, limb);
    SetInterpAmountForAllKeyframesRecursively(firstLeg, limb);
    SetInterpAmountForAllKeyframesRecursively(secondLeg, limb);
    const float blendWindow = motion.rootTransformBlend + motion.rootTransformBlend;
    const float armProgress = progress / blendWindow;
    SetInterpAmountForAllKeyframesRecursively(leadArm, 1.0f < armProgress ? 1.0f : armProgress);
    const float trail = armProgress - (2.0f - blendWindow) / blendWindow;
    SetInterpAmountForAllKeyframesRecursively(trailArm, 0.0f > trail ? 0.0f : trail);
    const float distanceScale = rootDistance > 0.69999999f ? 1.0f : rootDistance / 0.69999999f;
    const float distance = g_cubicSequenceDistance * distanceScale;
    const float rootAmount = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->interpolationAmount;
    const auto rootTime = static_cast<float>(static_cast<double>(rootAmount * 20.25f) * kMotnFrameTime);
    g_motnSequenceRootLift =
        ((MOTN_CalculateKeyframeForTime(g_motnSequenceLiftCurve, rootTime, g_motnFrameCount) - 0.14f) * 0.30000001f - 0.0099999998f) * distance;
    const auto leadTime = static_cast<float>(static_cast<double>(leadArm->interpolationAmount * 20.25f) * kMotnFrameTime);
    g_motnSequenceLeadHandLift =
        ((MOTN_CalculateKeyframeForTime(g_motnSequenceLiftCurve, leadTime, g_motnFrameCount) - 0.14f) * 0.30000001f - 0.0099999998f) * distance;
    const auto trailTime = static_cast<float>(static_cast<double>(trailArm->interpolationAmount * 20.25f) * kMotnFrameTime);
    const float trailLift = MOTN_CalculateKeyframeForTime(g_motnSequenceLiftCurve, trailTime, g_motnFrameCount) - 0.14f;
    g_motnSequenceActiveFrame = motion.frameTimer;
    g_motnSequenceTrailHandLift = (trailLift * 0.30000001f - 0.0099999998f) * distance;
    g_motnSequencePreviousFrame = motion.previousFrameTimer;
}

// The per-step cubic scratch slot of a target node (the nodeIDsMaybe index).
bool CubicTargetSlot(std::uint8_t nodeId, std::int64_t &slot) {
    switch (nodeId) {
    case 0:
        slot = 0;
        return true;
    case 3:
        slot = 1;
        return true;
    case 6:
        slot = 2;
        return true;
    case 11:
        slot = 3;
        return true;
    case 17:
        slot = 4;
        return true;
    case 21:
        slot = 5;
        return true;
    case 26:
        slot = 6;
        return true;
    case 33:
        slot = 7;
        return true;
    case 38:
        slot = 8;
        return true;
    case 42:
        slot = 9;
        return true;
    default:
        return false;
    }
}

Vector3 TransformedTranslation(const Matrix4x4 &frame, const Vector3 &offset) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    const Matrix4x4 translation = workTransformMatrix;
    workTransformMatrix = frame;
    Matrix4x4::Multiply(&translation);
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}

// Shenmue_CubicInterp: fills the VMPT cubic scratch with each target row's start/end tangents (the FCV slopes of the two
// motions, in their entity frames and relative to the root) and anchor positions, in the frames the cubic blend of the root uses.
void CubicInterp(MotionNodeRuntime *blend, MotionNodeRuntime *active) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 nextFrame = transformMatrix_prePass;
    const auto previousTime = static_cast<float>(static_cast<double>(g_motnSequencePreviousFrame) * kMotnFrameTime);
    const auto activeTime = static_cast<float>(static_cast<double>(g_motnSequenceActiveFrame) * kMotnFrameTime);
    if (g_motnNextEntityEuler) {
        Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
        nextFrame = workTransformMatrix;
    }
    Matrix4x4::SetViewMatrixFromEuler(blend->bindEuler.y, blend->bindEuler.x, blend->bindEuler.z);
    Matrix4x4 entityFrame = transformMatrix_prePass;
    g_motnInterpTransform = workTransformMatrix;
    workTransformMatrix = transformMatrix_prePass;
    if (g_motnEntityEuler) {
        Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        entityFrame = workTransformMatrix;
    }
    Matrix4x4::SetViewMatrixFromEuler(active->bindEuler.y, active->bindEuler.x, active->bindEuler.z);
    MotionInformationWork &motion = *g_pMotion;
    g_motnFinalInterpTransform = workTransformMatrix;
    const float rise = (active->effectorPosition.y - motion.rootPreTranslate.y) - (blend->effectorPosition.y - motion.rootBlendScratch[1]);
    const float threshold =
        (MOTN_GetRigType(static_cast<std::uint16_t>(motion.basePoseTemplateIndex))->nodes[0].position.y * 0.5f) / MOTN_GetRigType(0)->nodes[0].position.y;
    const bool split = !(threshold > rise);
    Matrix4x4 startRotation{};
    Matrix4x4 endRotation{};
    float endFactor = 0.0f;
    if (!split) {
        Matrix4x4_RotateAndBlend3D(startRotation, motion.rootTransformBlend, g_motnInterpTransform, g_motnFinalInterpTransform);
        endFactor = 1.0f - motion.rootTransformBlend;
    } else {
        const float blendAmount = motion.rootTransformBlend;
        const float first = blendAmount > 0.5f ? (blendAmount - 0.5f) + (blendAmount - 0.5f) : 0.0f;
        endFactor = 0.5f > blendAmount ? (0.5f - blendAmount) + (0.5f - blendAmount) : 0.0f;
        Matrix4x4_RotateAndBlend3D(startRotation, first, g_motnInterpTransform, g_motnFinalInterpTransform);
    }
    Matrix4x4_RotateAndBlend3D(endRotation, endFactor, g_motnInterpTransform, g_motnFinalInterpTransform);
    Matrix4x4 startBasis = startRotation;
    Matrix4x4 endBasis = endRotation;
    g_motnBlendStart = g_motnInterpTransform;
    const MotionNodeRuntime &blendRoot = *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    g_motnInterpTransform.rows[3].x = blendRoot.effectorPosition.x;
    g_motnInterpTransform.rows[3].y = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->effectorPosition.y;
    g_motnInterpTransform.rows[3].z = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->effectorPosition.z;
    g_motnBlendStart.rows[3].x = 0.0f;
    g_motnBlendStart.rows[3].y = 0.0f;
    g_motnBlendEnd = g_motnFinalInterpTransform;
    g_motnBlendStart.rows[3].z = 0.0f;
    const MotionNodeRuntime &activeRoot = *static_cast<MotionNodeRuntime *>(motion.activeNodes);
    g_motnFinalInterpTransform.rows[3].x = activeRoot.effectorPosition.x;
    g_motnFinalInterpTransform.rows[3].y = activeRoot.effectorPosition.y;
    g_motnFinalInterpTransform.rows[3].z = activeRoot.effectorPosition.z;
    g_motnBlendEnd.rows[3].x = 0.0f;
    g_motnBlendEnd.rows[3].y = 0.0f;
    g_motnBlendEnd.rows[3].z = 0.0f;
    Matrix4x4::InvertInPlace(&g_motnInterpTransform);
    Matrix4x4::InvertInPlace(&g_motnBlendStart);
    Matrix4x4::InvertInPlace(&g_motnFinalInterpTransform);
    Matrix4x4::InvertInPlace(&g_motnBlendEnd);
    MotionCubicSequenceScratch &scratch = *g_motnCubicScratch;
    std::int64_t slot = 0;
    for (; blend->nodeId != 0xff; ++blend, ++active) {
        MotnKeyframeChannel *startKeys = blend->translationKeys;
        MotnKeyframeChannel *endKeys = active->translationKeys;
        if (((blend->templateParentOrMode - 4U) & 0xfdU) != 0)
            continue;
        CubicTargetSlot(blend->nodeId, slot);
        if (!startKeys || !endKeys)
            continue;
        Vector3 &startTangent = scratch.startTangent[slot];
        Vector3 &endTangent = scratch.endTangent[slot];
        g_motnFrameCount = MOTN_GetFrameCountFromAnimId(motion.requestedAnimId);
        float *const startAxes[3]{&startTangent.x, &startTangent.y, &startTangent.z};
        for (std::size_t axis = 0; axis < 3; ++axis, ++startKeys) {
            *startAxes[axis] = startKeys->present ? MOTN_EvaluateKeyframeSlope(*startKeys, previousTime) : 0.0f;
            if (!axis && (motion.motionFlags & 0x40000U) != 0)
                startTangent.x = -startTangent.x;
        }
        g_motnFrameCount = MOTN_GetFrameCountFromAnimId(motion.currentAnimId);
        float *const endAxes[3]{&endTangent.x, &endTangent.y, &endTangent.z};
        for (std::size_t axis = 0; axis < 3; ++axis, ++endKeys) {
            *endAxes[axis] = endKeys->present ? MOTN_EvaluateKeyframeSlope(*endKeys, activeTime) : 0.0f;
            if (!axis && (motion.motionFlags & 0x20000U) != 0)
                endTangent.x = -endTangent.x;
        }
        workTransformMatrix = nextFrame;
        Matrix4x4::preTranslate(startTangent.x, startTangent.y, startTangent.z);
        startTangent = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = entityFrame;
        Matrix4x4::preTranslate(endTangent.x, endTangent.y, endTangent.z);
        endTangent = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        if (!blend->nodeId) {
            const float blendAmount = motion.rootTransformBlend;
            float startFactor = blendAmount;
            float endShare = 1.0f - blendAmount;
            if (split) {
                startFactor = blendAmount > 0.5f ? (blendAmount - 0.5f) + (blendAmount - 0.5f) : 0.0f;
                endShare = 0.5f > blendAmount ? (0.5f - blendAmount) + (0.5f - blendAmount) : 0.0f;
            }
            Vector3 root{};
            InterpolateJointPosition_CubicPolynomial(root, startFactor, *blend, *active);
            startRotation.rows[3].x = root.x;
            startRotation.rows[3].z = root.z;
            startRotation.rows[3].y = root.y;
            startBasis.rows[3].x = 0.0f;
            startBasis.rows[3].y = 0.0f;
            startBasis.rows[3].z = 0.0f;
            Matrix4x4::InvertInPlace(&startRotation);
            Matrix4x4::InvertInPlace(&startBasis);
            InterpolateJointPosition_CubicPolynomial(root, endShare, *blend, *active);
            endRotation.rows[3].x = root.x;
            endRotation.rows[3].z = root.z;
            endRotation.rows[3].y = root.y;
            endBasis.rows[3].x = 0.0f;
            endBasis.rows[3].y = 0.0f;
            endBasis.rows[3].z = 0.0f;
            Matrix4x4::InvertInPlace(&endRotation);
            Matrix4x4::InvertInPlace(&endBasis);
        }
        const std::uint8_t nodeId = blend->nodeId;
        if (nodeId == 17 || nodeId == 11 || nodeId == 6) {
            const std::size_t anchor = nodeId == 17 ? 0 : nodeId == 11 ? 1 : 4;
            scratch.startAnchor[anchor] = TransformedTranslation(g_motnInterpTransform, PositionOf(blend->effectorPosition));
            scratch.endAnchor[anchor] = TransformedTranslation(g_motnFinalInterpTransform, PositionOf(active->effectorPosition));
            startTangent = {startTangent.x - scratch.startTangent[0].x, startTangent.y - scratch.startTangent[0].y, startTangent.z - scratch.startTangent[0].z};
            endTangent = {endTangent.x - scratch.endTangent[0].x, endTangent.y - scratch.endTangent[0].y, endTangent.z - scratch.endTangent[0].z};
            startTangent = TransformedTranslation(g_motnBlendStart, startTangent);
            endTangent = TransformedTranslation(g_motnBlendEnd, endTangent);
        }
        if (nodeId != 26 && nodeId != 33)
            continue;
        const std::size_t anchor = nodeId == 26 ? 3 : 2;
        const bool leading = g_cubicSequenceDirection ? nodeId == 33 : nodeId == 26;
        if (leading) {
            scratch.startAnchor[anchor] = TransformedTranslation(g_motnInterpTransform, PositionOf(blend->effectorPosition));
            scratch.endAnchor[anchor] = TransformedTranslation(startRotation, PositionOf(active->effectorPosition));
        } else {
            scratch.startAnchor[anchor] = TransformedTranslation(endRotation, PositionOf(blend->effectorPosition));
            scratch.endAnchor[anchor] = TransformedTranslation(g_motnFinalInterpTransform, PositionOf(active->effectorPosition));
        }
        startTangent = {startTangent.x - scratch.startTangent[0].x, startTangent.y - scratch.startTangent[0].y, startTangent.z - scratch.startTangent[0].z};
        endTangent = {endTangent.x - scratch.endTangent[0].x, endTangent.y - scratch.endTangent[0].y, endTangent.z - scratch.endTangent[0].z};
        if (leading) {
            startTangent = TransformedTranslation(g_motnBlendStart, startTangent);
            endTangent = TransformedTranslation(startBasis, endTangent);
        } else {
            startTangent = TransformedTranslation(endBasis, startTangent);
            endTangent = TransformedTranslation(g_motnBlendEnd, endTangent);
        }
    }
}

// MOTN_RotateBonesInIKChainForFrame_Cubic: the cubic-sequence counterpart of MOTN_RotateBonesInIKChainForFrame_Lerp.
void MOTN_RotateBonesInIKChainForFrame_Cubic(float t, MotionNodeRuntime &start, MotionNodeRuntime &end) {
    float reach = 0.0f;
    if (end.flipFlags == 1) {
        MotionNodeRuntime &startChild = *start.children[0];
        MotionNodeRuntime &endChild = *end.children[0];
        GenerateBlendedTransformationMatrix(t, start, end);
        Matrix4x4_Push();
        Vector3 aim{};
        InterpolateJointPosition_CubicPolynomial(aim, t, *startChild.children[0], *endChild.children[0]);
        if (endChild.children[0]->translationKeys) {
            Matrix4x4::preTranslate(endChild.bindPosition->x, endChild.bindPosition->y, endChild.bindPosition->z);
            Matrix4x4::RotateZY(WorkLocalPoint(aim));
        } else {
            GenerateBlendedTransformationMatrix(t, startChild, endChild);
        }
        if (end.nodeId == 19) {
            const std::uint32_t flags = g_pMotion->rootAngleFlags;
            if ((flags & 0x80U) == 0 && (flags & 0x20000U) != 0 && g_pMotion->currentTurnAngle)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-(g_pMotion->currentTurnAngle >> 1)));
        }
        *MotionNodeMatrix(endChild) = workTransformMatrix;
        Matrix4x4::Pop(1);
        return;
    }
    if (end.flipFlags != 2)
        return;
    MotionNodeRuntime &endUpper = *end.children[0];
    MotionNodeRuntime &startUpper = *start.children[0];
    MotionNodeRuntime &startLower = *startUpper.children[0];
    MotionNodeRuntime &endLower = *endUpper.children[0];
    MotionNodeRuntime &startEnd = *startLower.children[0];
    const MotionNodeRuntime &endEnd = *endLower.children[0];
    Vector3 aim{};
    InterpolateJointPosition_CubicPolynomial(aim, t, startEnd, endEnd);
    if (startEnd.nodeId == 11 || startEnd.nodeId == 17) {
        Vector4 local{aim.x, aim.y, aim.z, 0.0f};
        CalculateLocalToWorldTransformation(local);
        aim = PositionOf(local);
        ApplyAnkleTarget(startEnd.nodeId, startLower.bindPosition->x + startEnd.bindPosition->x, &aim.x);
    }
    Matrix4x4::preTranslate(start.effectorPosition.x, start.effectorPosition.y, start.effectorPosition.z);
    Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(start.bindEuler.y, start.bindEuler.x, start.bindEuler.z);
    const Vector3 startLocal = WorkLocalPoint(aim);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(start.bindEuler.y, start.bindEuler.x, start.bindEuler.z);
    Matrix4x4::RotateZY(startLocal);
    g_motnBlendStart = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(end.bindEuler.y, end.bindEuler.x, end.bindEuler.z);
    const Vector3 endLocal = WorkLocalPoint(aim);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(end.bindEuler.y, end.bindEuler.x, end.bindEuler.z);
    Matrix4x4::RotateZY(endLocal);
    g_motnBlendEnd = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4 blended{};
    Matrix4x4::Multiply(Matrix4x4_BlendRotateByAngle(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
    Matrix4x4_Push();
    if (endEnd.translationKeys) {
        Matrix4x4::preTranslate(endUpper.bindPosition->x, endUpper.bindPosition->y, endUpper.bindPosition->z);
        const Vector3 local = WorkLocalPoint(aim);
        reach = std::sqrt((local.y * local.y + local.x * local.x) + local.z * local.z);
        Matrix4x4::RotateZY(local);
        const float upper = endLower.bindPosition->x;
        const float lower = endEnd.bindPosition->x;
        const float cosine = std::clamp(((upper * upper + reach * reach) - lower * lower) / ((upper + upper) * reach), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (!endLower.flipFlags)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, cosine);
    } else {
        GenerateBlendedTransformationMatrix(t, startUpper, endUpper);
    }
    *MotionNodeMatrix(endUpper) = workTransformMatrix;
    if (endEnd.translationKeys) {
        Matrix4x4::preTranslate(endLower.bindPosition->x, endLower.bindPosition->y, endLower.bindPosition->z);
        const float upper = endLower.bindPosition->x;
        const float lower = endEnd.bindPosition->x;
        const float cosine = std::clamp(((upper * upper + lower * lower) - reach * reach) / ((upper + upper) * lower), -1.0f, 1.0f);
        float sine = std::sqrt(1.0f - cosine * cosine);
        if (endLower.flipFlags == 1)
            sine = -sine;
        Matrix4x4::RotateAroundZ(sine, -cosine);
    } else {
        GenerateBlendedTransformationMatrix(t, startLower, endLower);
    }
    *MotionNodeMatrix(endLower) = workTransformMatrix;
    Matrix4x4::Pop(1);
}

// RecursiveFunc (MOTN_RecursiveCubicBlend): the cubic-sequence counterpart of Ryo_RecursiveIKForRealtimeMovement for modes 7 and 11.
void MOTN_RecursiveCubicBlend(MotionNodeRuntime &active, MotionNodeRuntime &blend) {
    MotionInformationWork &motion = *g_pMotion;
    if (blend.nodeId == motion.secondaryNodeId && blend.nodeId) {
        Ryo_RecursiveIKForRealtimeMovement(active, blend);
        return;
    }
    MotionNodeRuntime &paired = *blend.pairedSourceNode;
    const float t = blend.interpolationAmount;
    if ((active.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4_Push();
    std::uint8_t firstChild = 0;
    switch (active.templateParentOrMode) {
    case 0:
    case 1:
        GenerateBlendedTransformationMatrix(t, blend, paired);
        break;
    case 2: {
        if (active.nodeId != 1 && active.nodeId != 4) {
            MOTN_RotateBonesInIKChainForFrame_Cubic(t, blend, paired);
            break;
        }
        Vector3 aim{};
        Matrix4x4 blended{};
        if (active.nodeId == 1) {
            InterpolateJointPosition_CubicPolynomial(aim, t, *blend.children[0]->children[0], *paired.children[0]->children[0]);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(blend.effectorPosition.x, blend.effectorPosition.y, blend.effectorPosition.z);
            Matrix4x4::SetViewMatrixFromEuler(blend.bindEuler.y, blend.bindEuler.x, blend.bindEuler.z);
            const Vector3 startLocal = WorkLocalPoint(aim);
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::SetViewMatrixFromEuler(blend.bindEuler.y, blend.bindEuler.x, blend.bindEuler.z);
            Matrix4x4::RotateZY(startLocal);
            g_motnBlendStart = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(paired.effectorPosition.x, paired.effectorPosition.y, paired.effectorPosition.z);
            Matrix4x4::SetViewMatrixFromEuler(paired.bindEuler.y, paired.bindEuler.x, paired.bindEuler.z);
            const Vector3 endLocal = WorkLocalPoint(aim);
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::SetViewMatrixFromEuler(paired.bindEuler.y, paired.bindEuler.x, paired.bindEuler.z);
            Matrix4x4::RotateZY(endLocal);
            g_motnBlendEnd = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::Multiply(Matrix4x4_BlendRotateByAngle(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
            MOTN_RotateX();
        } else {
            InterpolateJointPosition_CubicPolynomial(aim, t, blend, paired);
            Matrix4x4::preTranslate(aim.x, aim.y, aim.z);
            InterpolateJointPosition_CubicPolynomial(aim, t, *blend.children[0]->children[0], *paired.children[0]->children[0]);
            motion.rootOffsetPosition = aim;
            if (const Vector3 *const world = motion.worldTargets[2])
                aim = *world;
            PerformMatrixTransformationAndAnimationUpdate(aim, aim);
            Matrix4x4_Push();
            Matrix4x4_Push();
            ApplyRotationTransform(blend, aim);
            g_motnBlendStart = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::InvertInPlace(nullptr);
            Matrix4x4::Multiply(&g_motnBlendStart);
            g_motnBlendStart = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4_Push();
            ApplyRotationTransform(paired, aim);
            g_motnBlendEnd = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::InvertInPlace(nullptr);
            Matrix4x4::Multiply(&g_motnBlendEnd);
            g_motnBlendEnd = workTransformMatrix;
            Matrix4x4::Pop(1);
            if (motion.blendScratch[3] == 0.0f) {
                Matrix4x4::Multiply(Matrix4x4_BlendRotateByAngle(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
            } else {
                if (Matrix4x4_BlendRotateByAngle(blended, t, g_motnBlendStart, g_motnBlendEnd))
                    g_motnBlendEnd = blended;
                Matrix4x4_Push();
                Matrix4x4_Push();
                Matrix4x4_ApplyVec3Rotation(paired, aim);
                g_motnBlendStart = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::InvertInPlace(nullptr);
                Matrix4x4::Multiply(&g_motnBlendStart);
                g_motnBlendStart = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::Multiply(
                    Matrix4x4_BlendRotateByAngle(blended, motion.blendScratch[3], g_motnBlendEnd, g_motnBlendStart) ? &blended : &g_motnBlendStart);
            }
        }
        *MotionNodeMatrix(*active.children[0]) = workTransformMatrix;
        break;
    }
    case 3:
        workTransformMatrix = *MotionNodeMatrix(active);
        break;
    case 4:
        Matrix4x4::preTranslate(active.bindPosition->x, active.bindPosition->y, active.bindPosition->z);
        if (active.flags)
            g_motnNeedsRotation = 1;
        if (active.nodeId == 3 && static_cast<std::int8_t>(motion.rootAngleFlags) >= 0) {
            g_skinningTransformMatrix = workTransformMatrix;
            ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
        }
        if (active.nodeId == 38 && static_cast<std::int8_t>(g_pMotion->rootAngleFlags) < 0)
            g_skinningTransformMatrix = workTransformMatrix;
        break;
    case 6: {
        Vector3 root{};
        InterpolateJointPosition_CubicPolynomial(root, t, blend, paired);
        const bool localOrRoot = (motion.motionFlags & 8U) != 0;
        Matrix4x4::preTranslate(localOrRoot ? paired.effectorPosition.x : root.x, root.y, localOrRoot ? paired.effectorPosition.z : root.z);
        g_motnRootInterpTransform = workTransformMatrix;
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        if (g_motnNextEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(blend.bindEuler.y, blend.bindEuler.x, blend.bindEuler.z);
        g_motnBlendStart = workTransformMatrix;
        g_motnInterpTransform = workTransformMatrix;
        workTransformMatrix = transformMatrix_prePass;
        if (g_motnEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(paired.bindEuler.y, paired.bindEuler.x, paired.bindEuler.z);
        g_motnBlendEnd = workTransformMatrix;
        g_motnFinalInterpTransform = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4 blended{};
        Matrix4x4::Multiply(Matrix4x4_RotateAndBlend3D(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
        g_motnNeedsRotation = 0;
        firstChild = g_pMotion->updateMode != 0 ? 1 : 0;
        break;
    }
    default:
        break;
    }
    *MotionNodeMatrix(active) = workTransformMatrix;
    active.bindPositionPacked[1] |= 2U;
    if (g_pMotion->updateMode && active.nodeId == 3) {
        if ((active.bindPositionPacked[1] & 1U) != 0)
            Matrix4x4::Pop(1);
        return;
    }
    for (std::uint8_t child = firstChild; child < active.childCount; ++child)
        MOTN_RecursiveCubicBlend(*active.children[child], *blend.children[child]);
    if ((active.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// UnknownType_0 (MOTN_EvaluateCubicSequenceBlend): modes 7, 10 and 11.
bool MOTN_EvaluateCubicSequenceBlend(MotionInformationWork &motion) {
    if (g_initializeCubicSequence) {
        PublishActiveClock(motion);
        if (!StorePoseEvaluation(motion, ApplyActiveNodes(motion)))
            return false;
    } else {
        g_motnEntityEuler = &motion.entityEuler;
        g_motnFrameCount = MOTN_GetFrameCountFromAnimId(motion.currentAnimId);
        g_motnFrameStart = motion.frameStart;
        g_motnTimerRef = motion.frameTimerRef;
        g_motnMirror = (motion.motionFlags >> 17U) & 1U;
        g_motnResidualTime = motion.residualFrameTime;
        g_motnFrameEnd = motion.frameEnd;
    }
    if (motion.blendFramesRemaining)
        --motion.blendFramesRemaining;
    MOTN_ProcessSequenceInterpolation(motion);
    g_motnNextEntityEuler = &motion.nextEntityEuler;
    RunMotionCallbacks(motion);
    if (g_initializeCubicSequence) {
        Matrix4x4_Push();
        CubicInterp(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
        Matrix4x4::Pop(1);
    }
    g_motnArmBlendMode = 0;
    ApplyArmTargetsAndLengthClamp_MOTN(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes), static_cast<MotionNodeRuntime *>(motion.activeNodes));
    if (((motion.interpolationMode - 7U) & 0xfbU) != 0)
        Ryo_RecursiveIKForRealtimeMovement(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    else
        MOTN_RecursiveCubicBlend(*static_cast<MotionNodeRuntime *>(motion.activeNodes), *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    MOTN_SetNewMatrices(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    if (motion.blendFramesRemaining == 1)
        motion.motionFlags &= ~0x10000U;
    return true;
}
}
