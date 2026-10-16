#include "s1/motion_internal.hpp"

namespace shenmue::s1 {
void SetupAnimationSequence(Character *character, MotionSequenceWork &sequence);
}

namespace shenmue::s1::motion_detail {
MotionPointWork *FindMotionPointWork(Character *character);

void MotionComponentNoOp(Character *, void *);

Vector3 RotateMotionPoint(const Vector3 &point, std::int32_t yaw);

void HandleMapControlTaskStateTransition(Character *character, MotionPointWork &work);

CharacterMoveRequestWork *FindMoveRequest(Character *character);

MotionSequenceWork *FindMoveSequence(Character *character);

void PublishCubicSequenceScratch(Character *character, MotionInformationWork &motion);

std::uint16_t MoveRequestBasePoseAnimationId(std::int32_t basePoseTemplateIndex);

Vector3 MoveRequestLocalPoint(const Matrix4x4 &space, const Vector3 &point);

void SetMoveRequestNodeYaw(Character *character, std::int32_t yaw);

void SetMoveRequestBlendFrames(MotionInformationWork &motion, std::int32_t frames);

void MOTI_ApplyMoveRequestRate(MotionInformationWork &motion, CharacterMoveRequestWork &work);

void charaSubmitMoveRequestLook(Character *character, CharacterMoveRequestWork &work, float distance);

void charaReleaseMoveRequestLook(Character *character, const CharacterMoveRequestWork *work);

void MOTI_BeginMoveRequestCubicBlend(
    Character *character, MotionInformationWork &motion, std::int32_t yaw, const Vector3 &position, std::uint32_t animationId, float startFrame);

void XMPT_ApplyDoorRootMotionTarget(const MotionInformationWork &motion, CharacterMoveRequestWork &work, const Matrix4x4 &space, Vector3 &local);
} // namespace shenmue::s1::motion_detail

namespace shenmue::s1::CycleManager {
using namespace motion_detail;


void ManageStateEndAnimations(Character *character, CharacterMoveRequestWork *work);

} // namespace CycleManager

namespace shenmue::s1::motion_detail {

}

namespace shenmue::s1 {
void freeTalkConv_SetMoveAndFacingConstraint(Character &owner, const Vector3 &target, std::int32_t yaw, std::int32_t animationFlags, float startFrame,
    float playbackRate, float distance, bool forceAnimation);

void charaSubmitMoveRequest(Character *character, const Vector3 *target, std::int32_t targetYaw, std::int32_t animationId,
    const std::uint32_t *locomotionPayload, float playbackRate, std::uint32_t mode);

void charaCancelMoveRequest(Character *character, std::int32_t keepPosition);

bool charaIsActiveMode(Character *character, std::int32_t mode);

bool charaHasBlockingTPMRequest(Character *character);
}

namespace shenmue::s1::motion_detail {
const std::uint16_t *ScenarioMotionData(std::uint32_t offset);

Character *ScenarioMotionCharacter(ScenarioContext *context, std::uint32_t id);

std::uint16_t FacingYaw(float fromX, float fromZ, float toX, float toZ);

float PickTurnAnimation(Character *character, std::int32_t targetYaw, const std::uint16_t *motionData, std::uint32_t flags);

std::int64_t AssignFacingMotion(Character *character, std::int32_t targetYaw, float turnRate, std::uint32_t flags, const std::uint16_t *motionData);
}

namespace shenmue::s1 {
float ryoPickTurnAnimByYawDelta(Character *character, std::int32_t targetYaw);

std::int64_t CHAR_SCNFApplyFacingTowardPoint(ScenarioContext *context, void *rawParams);

std::int64_t CHAR_SCNFAssignStand(ScenarioContext *context, void *rawParams);

std::int64_t CHAR_SCNFApplyFacing(ScenarioContext *context, void *rawParams);

void Script_ReadMOTNPositionAtFrame(ScenarioContext *context, void *rawParams);

void SetupAnimationSequence(Character *character, MotionSequenceWork &sequence) {
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    if (sequence.state == 0) {
        sequence.state = 3;
        return;
    }
    if (sequence.state == 3) {
        if (static_cast<const MotionNodeRuntime *>(motion->activeNodes)->nodeId == 0xff || (motion->motionFlags & 0x10000U) != 0)
            return;

        motion->playbackRate = sequence.playbackRate;
        if (!(sequence.playbackRate >= 0.1f) || sequence.playbackRate > 6.0f)
            sequence.playbackRate = sequence.playbackRate >= 0.1f ? 6.0f : 0.1f;
        const Vector3 entityPosition = GetPositionForEntity(character->entity.node);
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(motion->rootWorldPosition.x, entityPosition.y, motion->rootWorldPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(motion->entityEuler.y, 0, 0);
        const Matrix4x4 space = workTransformMatrix;
        Matrix4x4::Pop(1);

        sequence.yawDelta = static_cast<std::uint16_t>(sequence.targetYaw - static_cast<std::uint32_t>(motion->entityEuler.y));
        const float dx = sequence.targetPosition.x - space.rows[3].x;
        const float dy = sequence.targetPosition.y - space.rows[3].y;
        const float dz = sequence.targetPosition.z - space.rows[3].z;
        const float localX = space.rows[0].x * dx + space.rows[0].y * dy + space.rows[0].z * dz;
        const float localY = space.rows[1].x * dx + space.rows[1].y * dy + space.rows[1].z * dz;
        const float localZ = space.rows[2].x * dx + space.rows[2].y * dy + space.rows[2].z * dz;
        motion->blendMode = 11;
        const std::uint32_t yawDelta = sequence.yawDelta;
        const float startFrame = std::fmax(sequence.startFrame, 0.0f);
        if ((yawDelta & 0x8000U) != 0) {
            if (0x8000U - (yawDelta & 0x7fffU) >= 0x2000U)
                sequence.directionBit = localZ >= 0.0f ? 1U : 0U;
            else
                sequence.directionBit = !(localX < 0.0f) && localX != 0.0f ? 1U : 0U;
        } else if (yawDelta >= 0x2000U) {
            sequence.directionBit = !(localZ > 0.0f) && localZ != 0.0f ? 1U : 0U;
        } else {
            sequence.directionBit = !(localX < 0.0f) && localX != 0.0f ? 1U : 0U;
        }

        const std::uint32_t blendRequest = (static_cast<std::uint32_t>(sequence.animationFlags) >> 16U) & 0x7fffU;
        if (blendRequest != 0) {
            motion->blendFrameCount = static_cast<std::uint16_t>(blendRequest <= 1U ? 2U : blendRequest);
        } else {
            const float distance = std::sqrt(localY * localY + localX * localX + localZ * localZ);
            motion->blendFrameCount = static_cast<std::uint16_t>(static_cast<std::int64_t>(
                (distance * 8.0f + 20.0f + std::fabs(static_cast<float>(yawDelta & 0xffffU)) * 0.00018249285f) / sequence.playbackRate));
        }

        shenApplyTranslationToAll(character->entity.node, sequence.targetPosition);
        std::int16_t localYaw = static_cast<std::int16_t>(sequence.targetYaw);
        for (CharacterNode *child = character->entity.node.child; child; child = child->child)
            localYaw = static_cast<std::int16_t>(localYaw - static_cast<std::uint16_t>(child->rotation.y));
        character->entity.node.rotation.y = localYaw;
        motion->pendingAnimId = static_cast<std::uint16_t>(sequence.animationFlags);
        g_initializeCubicSequence = true;
        if (startFrame != 0.0f)
            motion->scriptStartFrame = startFrame;
        if (sequence.animationFlags >= 0)
            motion->motionFlags &= ~0x80U;
        else
            motion->motionFlags |= 0x80U;
        sequence.state = 9;
        return;
    }

    if (sequence.state == 9) {
        if (motion->pendingAnimId) {
            motion->previousEntityEuler.y = static_cast<std::int32_t>(
                static_cast<std::uint16_t>(motion->extractedYaw) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(motion->extractedYawAux)) << 16U));
            motion->blendFrameCount = 5;
            sequence.state = 0;
            sequence.directionBit = 0;
            motion->rootTransformBlend = 0.5f;
            return;
        }
        shenApplyTranslationToAll(character->entity.node, sequence.targetPosition);
        if ((motion->motionFlags & 0x10000U) != 0) {
            std::int16_t localYaw = static_cast<std::int16_t>(sequence.targetYaw);
            for (CharacterNode *child = character->entity.node.child; child; child = child->child)
                localYaw = static_cast<std::int16_t>(localYaw - static_cast<std::uint16_t>(child->rotation.y));
            character->entity.node.rotation.y = localYaw;
        } else {
            sequence.state = 11;
        }
        g_initializeCubicSequence = false;
        return;
    }

    manager->flags |= 1U;
    motion->blendFrameCount = 5;
    motion->blendMode = 1;
    sequence.state = 0;
    sequence.directionBit = 0;
    motion->frameStart = 0.0f;
    motion->rootTransformBlend = 0.5f;
}
}

namespace shenmue::s1::motion_detail {

MotionPointWork *FindMotionPointWork(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kMotionPointComponent) : nullptr;
    return component ? reinterpret_cast<MotionPointWork *>(component + 1) : nullptr;
}


void MotionComponentNoOp(Character *, void *) {
}

Vector3 RotateMotionPoint(const Vector3 &point, std::int32_t yaw) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - yaw));
    return {
        point.x * cosine + point.z * sine,
        point.y,
        point.z * cosine - point.x * sine,
    };
}

void HandleMapControlTaskStateTransition(Character *character, MotionPointWork &work) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MotionInformationWork &motion = manager->motion;

    const Vector3 position = GetPositionForEntity(character->entity.node);

    std::int16_t nodeYaw = 0;
    for (const CharacterNode *node = &character->entity.node; node; node = node->child)
        nodeYaw = static_cast<std::int16_t>(nodeYaw + static_cast<std::uint16_t>(node->rotation.y));
    const std::int32_t worldYaw = nodeYaw;
    const Vector3 rootOrigin{motion.rootWorldPosition.x, 0.0f, motion.rootWorldPosition.z};
    const Vector3 targetDelta{work.targetPosition.x - rootOrigin.x, work.targetPosition.y, work.targetPosition.z - rootOrigin.z};
    const Vector3 localDelta = RotateMotionPoint(targetDelta, -worldYaw);

    switch (work.state) {
    case 0:
        motion.blendMode = 4;
        motion.queuedBlendAnimId = kApproachingStanceAnimation;
        work.residualPosition.y = 0.0f;
        work.state = 1;
        break;
    case 1: {
        const std::uint16_t animationId = static_cast<std::uint16_t>(work.animationId);
        const std::int32_t frameCount = MOTN_GetFrameCountFromAnimId(animationId);
        // RoundToNearestInt floors values below 2^23; the truncating conversion that follows yields INT_MIN out of range.
        const float quotient = static_cast<float>(frameCount) / motion.playbackRate;
        const float floored = std::fabs(quotient) < 8388608.0f ? std::floor(quotient) : quotient;
        const std::int32_t wholeFrames = std::fabs(floored) < 2147483648.0f ? static_cast<std::int32_t>(floored) : std::numeric_limits<std::int32_t>::min();
        const float sampleFrame = static_cast<float>(wholeFrames) * motion.playbackRate;
        Vector3 rootStep{};
        g_motnHasYValue = 0;
        MOTN_ReadRootPositionAtFrame(animationId, static_cast<std::uint32_t>(motion.basePoseTemplateIndex), sampleFrame, rootStep, false);

        const float absX = std::fabs(localDelta.x);
        const float absZ = std::fabs(localDelta.z);
        std::int32_t steps = 0;
        if (absX > absZ) {
            work.residualPosition.x = 0.0f;
            const float ratio = absX / std::fabs(rootStep.x);
            steps = static_cast<std::int32_t>(ceil_float(ratio));
            motion.interpolationAmount = 1.0f - ratio / static_cast<float>(steps);
            work.residualPosition.z = localDelta.z - static_cast<float>(steps) * rootStep.z * (1.0f - motion.interpolationAmount);
        } else {
            work.residualPosition.z = 0.0f;
            const float ratio = absZ / std::fabs(rootStep.z);
            steps = static_cast<std::int32_t>(ceil_float(ratio));
            motion.interpolationAmount = 1.0f - ratio / static_cast<float>(steps);
            work.residualPosition.x = localDelta.x - static_cast<float>(steps) * rootStep.x * (1.0f - motion.interpolationAmount);
        }
        motion.reserved300 = (motion.reserved300 & 0xffffffff00000000ULL) | static_cast<std::uint32_t>(steps);

        const float motionFrames = static_cast<float>(frameCount - motnGetRootMotionEndMarker(animationId));
        const std::int32_t stanceFrameCount = MOTN_GetFrameCountFromAnimId(kApproachingStanceAnimation);
        work.transitionFrames =
            (motionFrames /
                (((static_cast<float>(frameCount) / static_cast<float>(stanceFrameCount) - 1.0f) * motion.interpolationAmount) + motion.playbackRate)) *
            static_cast<float>(steps);
        work.residualPosition = RotateMotionPoint(work.residualPosition, worldYaw);
        work.state = 2;
        break;
    }
    case 2: {
        if ((motion.motionFlags & 2U) != 0) {
            const std::uint32_t remaining = static_cast<std::uint32_t>(motion.reserved300) - 1U;
            motion.reserved300 = (motion.reserved300 & 0xffffffff00000000ULL) | remaining;
        }
        Vector3 next = position;
        next.x += work.residualPosition.x / work.transitionFrames;
        next.z += work.residualPosition.z / work.transitionFrames;
        shenApplyTranslationToAll(character->entity.node, next);
        if (static_cast<std::uint32_t>(motion.reserved300) == 0) {
            work.state = 0;
            motion.interpolationAmount = 0.0f;
            motion.blendMode = 0;
        }
        break;
    }
    default:
        break;
    }
}

CharacterMoveRequestWork *FindMoveRequest(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kMoveRequestComponent) : nullptr;
    return component ? reinterpret_cast<CharacterMoveRequestWork *>(component + 1) : nullptr;
}

MotionSequenceWork *FindMoveSequence(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kMoveSequenceComponent) : nullptr;
    return component ? reinterpret_cast<MotionSequenceWork *>(component + 1) : nullptr;
}

void PublishCubicSequenceScratch(Character *character, MotionInformationWork &motion) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kCubicScratchComponent);
    g_motnCubicScratch = component ? reinterpret_cast<MotionCubicSequenceScratch *>(component + 1) : nullptr;
    g_cubicSequenceMotion = &motion;
}


std::uint16_t MoveRequestBasePoseAnimationId(std::int32_t basePoseTemplateIndex) {
    return kBasePoseAnimationIds[basePoseTemplateIndex > 20 || basePoseTemplateIndex < 0 ? 0 : basePoseTemplateIndex];
}

Vector3 MoveRequestLocalPoint(const Matrix4x4 &space, const Vector3 &point) {
    const float dx = point.x - space.rows[3].x;
    const float dy = point.y - space.rows[3].y;
    const float dz = point.z - space.rows[3].z;
    return {
        space.rows[0].x * dx + space.rows[0].y * dy + space.rows[0].z * dz,
        space.rows[1].x * dx + space.rows[1].y * dy + space.rows[1].z * dz,
        space.rows[2].x * dx + space.rows[2].y * dy + space.rows[2].z * dz,
    };
}

void SetMoveRequestNodeYaw(Character *character, std::int32_t yaw) {
    std::int16_t localYaw = static_cast<std::int16_t>(yaw);
    for (CharacterNode *child = character->entity.node.child; child; child = child->child)
        localYaw = static_cast<std::int16_t>(localYaw - static_cast<std::uint16_t>(child->rotation.y));
    character->entity.node.rotation.y = localYaw;
}

void SetMoveRequestBlendFrames(MotionInformationWork &motion, std::int32_t frames) {
    motion.blendFrameCount = static_cast<std::uint16_t>(frames);
    if (static_cast<std::uint16_t>(frames) < 5U)
        motion.blendFrameCount = 5;
}


void MOTI_ApplyMoveRequestRate(MotionInformationWork &motion, CharacterMoveRequestWork &work) {
    motion.rootAngleFlags |= 0x800000U;
    if (work.playbackRate < 0.1f || work.playbackRate > 10.0f)
        work.playbackRate = 1.0f;
    motion.playbackRate = work.playbackRate;
}


void charaSubmitMoveRequestLook(Character *character, CharacterMoveRequestWork &work, float distance) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    MotionInformationWork &motion = manager->motion;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLookPointComponent);
    auto *lookPoint = component ? reinterpret_cast<LookPointWork *>(component + 1) : nullptr;
    if (!lookPoint) {
        lookPoint = static_cast<LookPointWork *>(AllocateHeapMemoryMODL(character, kLookPointComponent, sizeof(LookPointWork), LKPT_Callback));
        if (!lookPoint)
            return;
        lookPoint->active = 0;
        lookPoint->moveRequestLook = 0;
    }
    if (work.mode > 6 || ((0x5aU >> work.mode) & 1U) == 0)
        return;
    if (lookPoint->active || ((motion.rootAngleFlags & 0x200040U) == 0x200000U && lookPoint->moveRequestLook != 1))
        return;

    const float headY = motion.worldTargets[2] ? motion.worldTargets[2]->y : motion.rootOffsetPosition.y;
    Vector3 target{work.targetPosition.x, headY, work.targetPosition.z};
    if (distance < 0.7f) {
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(work.targetPosition.x, 0.0f, work.targetPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(work.targetYaw, 0, 0);
        Matrix4x4::preTranslate(0.0f, headY, -0.5f);
        target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
    }
    SetCharaLookAtPosition(character, 15, &target, 0);
    lookPoint->moveRequestLook = 1;
}


void charaReleaseMoveRequestLook(Character *character, const CharacterMoveRequestWork *work) {
    if (!character || !work)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    CharacterComponentNode *const component = manager ? HLib_GetCharacterComponent(character, kLookPointComponent) : nullptr;
    if (!component)
        return;
    auto *const lookPoint = reinterpret_cast<LookPointWork *>(component + 1);
    if (lookPoint->moveRequestLook != 1)
        return;
    if ((manager->motion.rootAngleFlags & 0x40U) == 0)
        SetCharaLookAtPosition(character, -15, nullptr, 0);
    lookPoint->moveRequestLook = 0;
}


void MOTI_BeginMoveRequestCubicBlend(
    Character *character, MotionInformationWork &motion, std::int32_t yaw, const Vector3 &position, std::uint32_t animationId, float startFrame) {
    if (character) {
        SetMoveRequestNodeYaw(character, yaw);
        shenApplyTranslationToAll(character->entity.node, position);
    }
    motion.blendMode = 11;
    motion.pendingAnimId = static_cast<std::uint16_t>(animationId);
    if (startFrame == 0.0f)
        motion.forcedFrameTime = static_cast<float>((animationId >> 16U) & 0x7fffU);
    else
        motion.scriptStartFrame = startFrame;
    g_initializeCubicSequence = true;
    if ((animationId & 0x80000000U) == 0)
        motion.motionFlags &= ~0x80U;
    else
        motion.motionFlags |= 0x80U;
}



void XMPT_ApplyDoorRootMotionTarget(const MotionInformationWork &motion, CharacterMoveRequestWork &work, const Matrix4x4 &space, Vector3 &local) {
    local = MoveRequestLocalPoint(space, {work.targetPosition.x, 0.0f, work.targetPosition.z});
    const float distance = std::sqrt(local.x * local.x + local.y * local.y + local.z * local.z);
    float frameScale = 10.0f;
    switch (static_cast<std::uint16_t>(work.animationId)) {
    case 0x0286U:
    case 0x0277U:
    case 0x027dU:
    case 0x027cU:
    case 0x0033U:
        frameScale = 20.0f;
        break;
    case 0x0035U:
    case 0x0289U:
        frameScale = 34.0f;
        break;
    default:
        break;
    }
    const float endFrame = static_cast<float>((work.animationId >> 16) & 0x7fff);
    const float approachFrame = distance * 1.5f * frameScale;
    work.startFrame = static_cast<float>(static_cast<std::int32_t>(endFrame < approachFrame ? endFrame : approachFrame));
    const float sampleFrame = work.startFrame - motion.playbackRate;
    Vector3 root{};
    if (sampleFrame > 0.0f) {
        g_motnHasYValue = 1;
        MOTN_ReadRootPositionAtFrame(static_cast<std::uint16_t>(work.animationId), 0, sampleFrame, root, true);
    }
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::SetViewMatrixFromEuler(work.targetYaw, 0, 0);
    Matrix4x4::preTranslate(work.animationId < 0 ? -root.x : root.x, root.y, root.z);
    const float offsetX = workTransformMatrix.rows[3].x;
    const float offsetZ = workTransformMatrix.rows[3].z;
    Matrix4x4::Pop(1);
    work.targetPosition.x += offsetX;
    work.targetPosition.z += offsetZ;
    local = MoveRequestLocalPoint(space, {work.targetPosition.x, 0.0f, work.targetPosition.z});
}
} // namespace shenmue::s1::motion_detail

namespace shenmue::s1::CycleManager {
using namespace motion_detail;


void ManageStateEndAnimations(Character *character, CharacterMoveRequestWork *work) {
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    if (!work)
        return;

    const std::uint16_t walkAnimationId = work->locomotionPayload[0] ? static_cast<std::uint16_t>(work->locomotionPayload[0]) : kCommonWalkAnimationId;
    const std::int16_t targetYaw = static_cast<std::int16_t>(work->targetYaw);
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(motion->rootWorldPosition.x, 0.0f, motion->rootWorldPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(motion->entityEuler.y, 0, 0);
    const Matrix4x4 space = workTransformMatrix;
    Matrix4x4::Pop(1);
    Vector3 local = MoveRequestLocalPoint(space, work->targetPosition);
    std::int32_t localYaw = 0;
    if (static_cast<double>(std::fabs(local.x)) >= 0.001 || static_cast<double>(std::fabs(local.z)) >= 0.001)
        localYaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-local.x, -local.z));

    const auto *const activeRows = static_cast<const MotionNodeRuntime *>(motion->activeNodes);
    const auto endTransition = [&]() {
        work->state = 11;
        motion->previousEntityEuler.y = static_cast<std::int32_t>(
            static_cast<std::uint16_t>(motion->extractedYaw) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(motion->extractedYawAux)) << 16U));
    };

    switch (work->state) {
    case 0:
        if (work->mode == 2 || work->mode == 6)
            work->state = 6;
        else if (work->mode == 1 || work->mode == 3 || work->mode == 4)
            work->state = 1;
        else
            work->state = 3;
        return;
    case 1: {
        if (activeRows->nodeId == 0xff || (motion->motionFlags & 0x10000U) != 0)
            return;
        MOTI_ApplyMoveRequestRate(*motion, *work);
        float distance = std::sqrt(local.z * local.z + local.x * local.x);
        if (work->mode == 3) {
            distance = 0.7f;
            work->yawDelta = static_cast<std::int16_t>(targetYaw - static_cast<std::uint16_t>(motion->entityEuler.y));
        } else {
            work->yawDelta = static_cast<std::int16_t>(localYaw);
            if (distance < 0.7f) {
                work->state = 3;
                return;
            }
        }
        if (std::fabs(static_cast<float>(static_cast<std::int16_t>(work->yawDelta))) > 21845.0f) {
            charaSubmitMoveRequestLook(character, *work, distance);
            const std::int32_t clamped = static_cast<std::int16_t>(work->yawDelta) < 0 ? -21845 : 21845;
            work->yawDelta = clamped;
            work->currentYaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(motion->entityEuler.y) + clamped);
            work->directionBit = (clamped & 0x8000) != 0 ? 1U : 0U;
            motion->blendFrameCount = static_cast<std::uint16_t>(
                static_cast<std::int32_t>((std::fabs(static_cast<float>(clamped & 0x7fff)) * 0.00048664759f + 16.0f) / work->playbackRate));
            work->distance = 1.0f;
            work->workPosition = {motion->rootWorldPosition.x, character->entity.node.position.y, motion->rootWorldPosition.z};
            MOTI_BeginMoveRequestCubicBlend(
                character, *motion, work->currentYaw, work->workPosition, MoveRequestBasePoseAnimationId(motion->basePoseTemplateIndex), 0.0f);
            work->state = 2;
            return;
        }
        work->state = 3;
        return;
    }
    case 2:
        if (motion->pendingAnimId) {
            endTransition();
            return;
        }
        shenApplyTranslationToAll(character->entity.node, work->workPosition);
        if ((motion->motionFlags & 0x10000U) != 0)
            SetMoveRequestNodeYaw(character, work->currentYaw);
        else
            work->state = 3;
        g_initializeCubicSequence = false;
        return;
    case 3: {
        if (activeRows->nodeId == 0xff || (motion->motionFlags & 0x10000U) != 0)
            return;
        MOTI_ApplyMoveRequestRate(*motion, *work);
        work->yawDelta = static_cast<std::int16_t>(static_cast<std::uint16_t>(work->targetYaw) - static_cast<std::uint16_t>(motion->entityEuler.y));
        if (work->mode == 5)
            XMPT_ApplyDoorRootMotionTarget(*motion, *work, space, local);
        const float distance = std::sqrt(local.z * local.z + local.x * local.x);
        if (distance < 0.7f && (work->mode == 0 || work->mode == 4 || work->mode == 5)) {
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::SetViewMatrixFromEuler(-work->targetYaw, 0, 0);
            Matrix4x4::preTranslate(motion->rootWorldPosition.x - work->targetPosition.x, 0.0f, motion->rootWorldPosition.z - work->targetPosition.z);
            const float offsetX = workTransformMatrix.rows[3].x;
            Matrix4x4::Pop(1);
            const std::int16_t yawDelta = static_cast<std::int16_t>(work->yawDelta);
            if (yawDelta >= 0)
                work->directionBit = offsetX < 0.0f ? 1U : 0U;
            else
                work->directionBit = offsetX <= 0.0f ? 1U : 0U;

            std::int32_t frames;
            const float turnFrames = std::fabs(static_cast<float>(yawDelta)) * 0.000030517578f * 24.0f;
            if (work->mode == 4) {
                work->distance = 1.0f;
                frames = static_cast<std::int32_t>(
                    static_cast<float>(static_cast<std::int32_t>(turnFrames + ((distance / 0.7f) * 16.0f + 20.0f))) / work->playbackRate);
            } else {
                work->distance = 0.0f;
                if (work->mode == 5) {
                    frames = static_cast<std::int32_t>((std::sqrt(local.y * local.y + local.x * local.x + local.z * local.z) * 8.0f + 16.0f +
                                                           std::fabs(static_cast<float>(work->yawDelta & 0x7fff)) * 0.00018249285f) /
                        work->playbackRate);
                } else {
                    frames = static_cast<std::int32_t>(
                        static_cast<float>(static_cast<std::int32_t>(turnFrames + ((distance / 0.7f) * 16.0f + 10.0f))) / work->playbackRate);
                }
            }
            SetMoveRequestBlendFrames(*motion, frames);
            work->workPosition = work->targetPosition;
            work->currentYaw = targetYaw;
            MOTI_BeginMoveRequestCubicBlend(
                character, *motion, work->targetYaw, work->workPosition, static_cast<std::uint32_t>(work->animationId), work->startFrame);
            work->state = 8;
            return;
        }

        const float step = distance >= 0.05f ? (distance >= 0.7f ? -0.2f : -0.1f) : 0.0f;
        const float nodeY = character->entity.node.position.y;
        std::int32_t turnYaw = localYaw;
        if (work->mode == 3) {
            const std::int16_t yawDelta = static_cast<std::int16_t>(targetYaw - static_cast<std::uint16_t>(motion->entityEuler.y));
            work->yawDelta = yawDelta;
            turnYaw = yawDelta;
            work->currentYaw = targetYaw;
        } else {
            work->yawDelta = static_cast<std::int16_t>(localYaw);
            work->currentYaw = static_cast<std::int16_t>(localYaw) + static_cast<std::int16_t>(motion->entityEuler.y);
        }
        charaSubmitMoveRequestLook(character, *work, distance);
        Matrix4x4_Push(&space);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(turnYaw));
        Matrix4x4::preTranslate(0.0f, 0.0f, step);
        work->workPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        work->workPosition.y = nodeY;
        motion->pendingAnimId = walkAnimationId;
        motion->forcedFrameTime = 8.0f;
        work->state = 4;
        g_initializeCubicSequence = true;
        SetMoveRequestBlendFrames(*motion,
            static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(
                                          std::fabs(static_cast<float>(static_cast<std::int16_t>(work->yawDelta))) * 0.000030517578f * 25.0f + 10.0f)) /
                work->playbackRate));
        SetMoveRequestNodeYaw(character, work->currentYaw);
        shenApplyTranslationToAll(character->entity.node, work->workPosition);
        motion->blendMode = 11;
        work->distance = 0.0f;
        const bool mirroredWalk = shenLookupAnimationIndex(walkAnimationId, 19) != nullptr;
        if (static_cast<std::int16_t>(work->yawDelta) >= 0) {
            if (mirroredWalk)
                motion->motionFlags |= 0x80U;
            work->directionBit = 0;
        } else {
            if (!mirroredWalk)
                motion->motionFlags |= 0x80U;
            work->directionBit = 1;
        }
        return;
    }
    case 4:
        if (motion->pendingAnimId) {
            endTransition();
            return;
        }
        shenApplyTranslationToAll(character->entity.node, work->workPosition);
        if ((motion->motionFlags & 0x10000U) != 0) {
            SetMoveRequestNodeYaw(character, work->currentYaw);
        } else if (work->mode == 1 || work->mode == 3) {
            work->state = 11;
        } else {
            work->state = 5;
            charaReleaseMoveRequestLook(character, work);
        }
        g_initializeCubicSequence = false;
        return;
    case 5: {
        if (motion->pendingAnimId) {
            endTransition();
            return;
        }
        const std::int32_t turn = static_cast<std::int16_t>(localYaw);
        const std::int32_t step = std::fabs(static_cast<double>(turn)) <= 2730.0 ? turn : turn / 8;
        work->currentYaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(motion->entityEuler.y) + step);
        SetMoveRequestNodeYaw(character, work->currentYaw);
        if (std::sqrt(local.z * local.z + local.x * local.x) < 0.3f || (local.x >= 0.0f && local.z >= 0.0f))
            work->state = 7;
        return;
    }
    case 6:
        if (activeRows->nodeId != 0xff && (motion->motionFlags & 0x10000U) == 0) {
            MOTI_ApplyMoveRequestRate(*motion, *work);
            work->state = 7;
        }
        return;
    case 7: {
        auto *const rows = static_cast<MotionNodeRuntime *>(motion->activeNodes);
        const MotionNodeRuntime *const rightArm = MOTN_FindNodeRow(rows, 0x1a);
        const MotionNodeRuntime *const leftHandTarget = MOTN_FindNodeRow(rows, 0x21);
        if (rightArm && leftHandTarget)
            work->directionBit = leftHandTarget->effectorPosition.y > rightArm->effectorPosition.y ? 1U : 0U;
        const std::int16_t yawDelta = static_cast<std::int16_t>(static_cast<std::uint16_t>(motion->entityEuler.y) - targetYaw);
        work->currentYaw = targetYaw;
        work->yawDelta = yawDelta;
        work->workPosition = work->targetPosition;
        if ((work->mode == 4 || work->mode == 6) && std::fabs(static_cast<float>(yawDelta)) > 21845.0f) {
            charaSubmitMoveRequestLook(character, *work, 0.0f);
            const std::int32_t clamped = yawDelta < 0 ? -21845 : 21845;
            work->yawDelta = clamped;
            work->currentYaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(motion->entityEuler.y) - clamped);
        }
        work->distance = 0.0f;
        MOTI_BeginMoveRequestCubicBlend(
            character, *motion, work->currentYaw, work->workPosition, static_cast<std::uint32_t>(work->animationId), work->startFrame);
        work->state = 8;
        const std::int16_t remainingTurn = static_cast<std::int16_t>(motion->extractedYaw - targetYaw);
        const std::int32_t frames = static_cast<std::int32_t>(
            static_cast<float>(static_cast<std::int32_t>(std::fabs(static_cast<float>(remainingTurn)) * 0.000030517578f * 20.0f + 10.0f)) / work->playbackRate);
        motion->blendFrameCount = static_cast<std::uint16_t>(frames);
        if (motion->currentAnimId == 724 || static_cast<std::uint16_t>(work->animationId) == 724)
            motion->blendFrameCount = static_cast<std::uint16_t>(static_cast<std::uint16_t>(frames) >> 1U);
        if (motion->blendFrameCount < 5U)
            motion->blendFrameCount = 5;
        return;
    }
    case 8:
        if (motion->pendingAnimId) {
            endTransition();
            return;
        }
        shenApplyTranslationToAll(character->entity.node, work->workPosition);
        if ((motion->motionFlags & 0x10000U) != 0)
            SetMoveRequestNodeYaw(character, work->currentYaw);
        else
            work->state = static_cast<std::uint16_t>(work->currentYaw) == static_cast<std::uint16_t>(targetYaw) ? 11 : 7;
        g_initializeCubicSequence = false;
        return;
    case 10: {
        if (motion->pendingAnimId) {
            endTransition();
            return;
        }
        if ((motion->motionFlags & 0x10000U) != 0) {
            SetMoveRequestNodeYaw(character, work->currentYaw);
            shenApplyTranslationToAll(character->entity.node, work->workPosition);
            g_initializeCubicSequence = false;
            return;
        }
        std::int16_t worldYaw = 0;
        for (CharacterNode *node = &character->entity.node; node; node = node->child)
            worldYaw = static_cast<std::int16_t>(worldYaw + static_cast<std::uint16_t>(node->rotation.y));
        work->targetYaw = worldYaw;
        work->currentYaw = worldYaw;
        work->yawDelta = 0;
        work->workPosition = GetPositionForEntity(character->entity.node);
        motion->blendMode = 11;
        g_initializeCubicSequence = true;
        motion->pendingAnimId = MoveRequestBasePoseAnimationId(motion->basePoseTemplateIndex);
        SetMoveRequestNodeYaw(character, work->currentYaw);
        shenApplyTranslationToAll(character->entity.node, work->workPosition);
        motion->blendFrameCount = 10;
        work->state = 8;
        return;
    }
    default:
        work->state = 0;
        work->startFrame = 0.0f;
        charaReleaseMoveRequestLook(character, work);
        motion->rootAngleFlags &= ~0x800000U;
        motion->playbackRate = work->savedPlaybackRate;
        motion->blendFrameCount = 5;
        motion->blendMode = 1;
        motion->rootTransformBlend = 0.5f;
        g_initializeCubicSequence = false;
        return;
    }
}

} // namespace CycleManager

namespace shenmue::s1::motion_detail {

}

namespace shenmue::s1 {

bool WMPT_Request(Character *character, const Vector3 &target, std::uint32_t animationId, float timeOffset) {
    MotionPointWork *work = FindMotionPointWork(character);
    if (!work) {
        work = static_cast<MotionPointWork *>(AllocateHeapMemoryMODL(character, kMotionPointComponent, sizeof(MotionPointWork), MotionComponentNoOp));
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return false;
    }
    work->state = 0;
    if (manager->motion.pendingAnimId != static_cast<std::uint16_t>(animationId)) {
        return false;
    }
    work->animationId = animationId;
    work->targetPosition = target;
    work->originalPlaybackRate = manager->motion.playbackRate;
    work->timeOffset = timeOffset == -1.0f ? manager->motion.playbackRate : timeOffset;
    HandleMapControlTaskStateTransition(character, *work);
    return true;
}

bool WMPT_IsActive(Character *character) {
    const MotionPointWork *const work = FindMotionPointWork(character);
    return work && work->state != 0;
}

void freeTalkConv_SetMoveAndFacingConstraint(Character &owner, const Vector3 &target, std::int32_t yaw, std::int32_t animationFlags, float startFrame,
    float playbackRate, float distance, bool forceAnimation) {
    MotionSequenceWork *sequence = FindMoveSequence(&owner);
    if (sequence && sequence->state != 0)
        return;
    if (!sequence) {
        sequence = static_cast<MotionSequenceWork *>(AllocateHeapMemoryMODL(&owner, kMoveSequenceComponent, 0x30, MotionComponentNoOp));
        if (!sequence)
            return;
    }

    const CharacterMoveRequestWork *const request = FindMoveRequest(&owner);
    if (request && request->state != 0)
        return;
    if (!HLib_GetCharacterComponent(&owner, kCubicScratchComponent))
        AllocateHeapMemoryMODL(&owner, kCubicScratchComponent, 0x168, MotionComponentNoOp);

    if (!MOTM_GetWork(&owner)) {
        const auto *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, &owner);
        return;
    }

    const Vector3 current = GetPositionForEntity(owner.entity.node);
    const float dx = current.x - target.x;
    const float dy = current.y - target.y;
    const float dz = current.z - target.z;
    if (std::sqrt(dy * dy + dx * dx + dz * dz) > 3.0f)
        return;

    Matrix4x4_TranslateAndRotate(*reinterpret_cast<const CharacterIdTransform *>(&owner.entity.node));
    sequence->targetPosition.x = target.x;
    sequence->targetPosition.y = MapProbe_UpdateIntersectionStateAtPoint(target);
    sequence->startFrame = startFrame;
    sequence->distance = distance;
    sequence->playbackRate = playbackRate;
    sequence->targetPosition.z = target.z;
    sequence->targetYaw = static_cast<std::uint32_t>(yaw);
    sequence->animationFlags = animationFlags;
    sequence->directionBit = 0;
    sequence->yawDelta = 0;
    if (forceAnimation)
        sequence->animationFlags = static_cast<std::int32_t>(static_cast<std::uint32_t>(animationFlags) | 0x80000000U);
    SetupAnimationSequence(&owner, *sequence);
}

// Called from MOTM_Callback. An active YMPT sequence owns the frame; otherwise an active XMPT request runs, and
// WMPT runs whenever XMPT does not. Only the one-frame cubic initialization latch is cleared per character; the
// published scratch, direction and distance persist until the next owner publishes them.
void ManageMotionAnimations(Character *character) {
    g_initializeCubicSequence = false;
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionSequenceWork *const sequence = FindMoveSequence(character);
    if (sequence && sequence->state) {
        SetupAnimationSequence(character, *sequence);
        g_cubicSequenceDirection = sequence->directionBit != 0;
        g_cubicSequenceDistance = sequence->distance;
        if (sequence->state && manager)
            PublishCubicSequenceScratch(character, manager->motion);
    }
    CharacterMoveRequestWork *const work = FindMoveRequest(character);
    if ((sequence && sequence->state) || !work || !work->state) {
        MotionPointWork *const point = FindMotionPointWork(character);
        if (point && point->state)
            HandleMapControlTaskStateTransition(character, *point);
        return;
    }
    CycleManager::ManageStateEndAnimations(character, work);
    g_cubicSequenceDirection = work->directionBit != 0;
    g_cubicSequenceDistance = work->distance;
    if (work->state && manager)
        PublishCubicSequenceScratch(character, manager->motion);
}

void charaSubmitMoveRequest(Character *character, const Vector3 *target, std::int32_t targetYaw, std::int32_t animationId,
    const std::uint32_t *locomotionPayload, float playbackRate, std::uint32_t mode) {
    CharacterMoveRequestWork *work = FindMoveRequest(character);
    if (!work) {
        work = static_cast<CharacterMoveRequestWork *>(
            AllocateHeapMemoryMODL(character, kMoveRequestComponent, sizeof(CharacterMoveRequestWork), MotionComponentNoOp));
        if (!work)
            return;
    }
    if (!character || !HLib_GetCharacterComponent(character, kCubicScratchComponent))
        AllocateHeapMemoryMODL(character, kCubicScratchComponent, 0x168, MotionComponentNoOp);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MotionInformationWork &motion = manager->motion;
    SelectCharacterMapQuery(character);
    if (target) {
        work->targetPosition.x = target->x;
        work->targetPosition.y = MapProbe_UpdateIntersectionStateAtPoint(*target);
        work->targetPosition.z = target->z;
    } else {
        work->targetPosition = motion.rootWorldPosition;
    }
    work->targetYaw = targetYaw;
    work->animationId = animationId;
    if (mode > 6)
        return;
    work->state = 0;
    work->mode = mode;
    work->yawDelta = 0;
    work->savedPlaybackRate = motion.playbackRate;
    work->startFrame = 0.0f;
    work->playbackRate = playbackRate == -1.0f ? motion.playbackRate : playbackRate;
    if (locomotionPayload)
        std::memcpy(work->locomotionPayload, locomotionPayload, sizeof(work->locomotionPayload));
    else
        work->locomotionPayload[0] = 0;
    charaReleaseMoveRequestLook(character, work);
    CycleManager::ManageStateEndAnimations(character, work);
}


void charaCancelMoveRequest(Character *character, std::int32_t keepPosition) {
    CharacterMoveRequestWork *const work = FindMoveRequest(character);
    if (!work)
        return;
    if (!keepPosition) {
        work->state = 11;
        return;
    }
    if (work->state != 4) {
        std::int16_t worldYaw = 0;
        for (CharacterNode *node = &character->entity.node; node; node = node->child)
            worldYaw = static_cast<std::int16_t>(worldYaw + static_cast<std::uint16_t>(node->rotation.y));
        work->currentYaw = worldYaw;
        work->workPosition = GetPositionForEntity(character->entity.node);
    }
    work->state = 10;
}

bool charaIsActiveMode(Character *character, std::int32_t mode) {
    const CharacterMoveRequestWork *const work = FindMoveRequest(character);
    return work && work->state != 0 && static_cast<std::int32_t>(work->mode) == mode;
}

bool charaHasBlockingTPMRequest(Character *character) {
    if (!character)
        return false;
    const MotionSequenceWork *const sequence = FindMoveSequence(character);
    const CharacterMoveRequestWork *const work = FindMoveRequest(character);
    if (work && work->state != 0)
        return work->mode != 5;
    return sequence && sequence->state != 0;
}
}

namespace shenmue::s1::motion_detail {

const std::uint16_t *ScenarioMotionData(std::uint32_t offset) {
    return offset && g_pCurrentTaskData ?
        reinterpret_cast<const std::uint16_t *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(offset))) :
        nullptr;
}

Character *ScenarioMotionCharacter(ScenarioContext *context, std::uint32_t id) {
    return context && context->task && id ? CHID_Find(context->task->task, id) : nullptr;
}

std::uint16_t FacingYaw(float fromX, float fromZ, float toX, float toZ) {
    return Shenmue_Atan2Fixed(-(toX - fromX), -(toZ - fromZ));
}

float PickTurnAnimation(Character *character, std::int32_t targetYaw, const std::uint16_t *motionData, std::uint32_t flags) {
    const std::int32_t accumulatedYaw = cpctGetAccumYaw(character);
    const std::uint16_t sector =
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(targetYaw) - static_cast<std::uint16_t>(accumulatedYaw) + 0x1000U) >> 13U;

    std::uint16_t animationId;
    if (sector == 0) {
        animationId = (flags & 0x200000U) != 0 && motionData ? motionData[7] : 89;
    } else {
        animationId = motionData ? motionData[sector - 1U] : kDefaultTurnAnimationIds[sector - 1U];
        if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
            manager->motion.motionFlags &= ~2U;
            manager->motion.pendingAnimId = animationId;
        }
    }

    const float frameCount = static_cast<float>(MOTN_GetFrameCountFromAnimId(animationId));
    std::int32_t animationYaw = cpctGetAccumYaw(character);
    if (sector != 0) {
        animationYaw = static_cast<std::uint16_t>(static_cast<std::uint32_t>(animationYaw) + sector * 0x2000U);
    }
    const std::int32_t yawDifference = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(targetYaw) - static_cast<std::uint32_t>(animationYaw));
    float degrees = static_cast<float>(yawDifference) / 182.04445f;
    if (degrees >= 180.0f)
        degrees = static_cast<float>(static_cast<double>(degrees) - 360.0);
    else if (degrees <= -180.0f)
        degrees = static_cast<float>(static_cast<double>(degrees) + 360.0);
    return degrees / frameCount;
}

std::int64_t AssignFacingMotion(Character *character, std::int32_t targetYaw, float turnRate, std::uint32_t flags, const std::uint16_t *motionData) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!character || !manager)
        return 0;

    const float delta = turnRate * -8192.0f / 45.0f;
    const std::int32_t yawDelta =
        std::isfinite(delta) && delta >= -2147483648.0f && delta < 2147483648.0f ? static_cast<std::int32_t>(delta) : std::numeric_limits<std::int32_t>::min();
    character->entity.node.rotation.y =
        static_cast<std::uint16_t>(static_cast<std::uint32_t>(character->entity.node.rotation.y) - static_cast<std::uint32_t>(yawDelta));
    if ((manager->motion.motionFlags & 2U) == 0)
        return 0;

    character->entity.node.rotation.y = targetYaw;
    const std::uint16_t animationId = (flags & 0x200000U) != 0 && motionData ? motionData[7] : 89;
    manager->motion.motionFlags &= ~2U;
    manager->motion.pendingAnimId = animationId;
    return 0x10000;
}
}

namespace shenmue::s1 {

float ryoPickTurnAnimByYawDelta(Character *character, std::int32_t targetYaw) {
    return PickTurnAnimation(character, targetYaw, nullptr, 0);
}

std::int64_t CHAR_SCNFApplyFacingTowardPoint(ScenarioContext *context, void *rawParams) {
    auto *const params = static_cast<std::uint32_t *>(rawParams);
    if (!params || !params[1] || !g_pCurrentTaskData)
        return 0;

    Character *const character = ScenarioMotionCharacter(context, params[0]);
    const auto *const target = reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(params[1])));
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const std::uint16_t targetYaw = FacingYaw(position.x, position.z, target->x, target->z);
    const std::uint16_t *const motionData = ScenarioMotionData(params[2]);
    const float turnRate = PickTurnAnimation(character, targetYaw, motionData, params[5]);
    params[3] = std::bit_cast<std::uint32_t>(turnRate);
    params[4] = targetYaw;
    return AssignFacingMotion(character, targetYaw, turnRate, params[5], motionData);
}

std::int64_t CHAR_SCNFAssignStand(ScenarioContext *context, void *rawParams) {
    auto *const params = static_cast<std::uint32_t *>(rawParams);
    if (!params)
        return 0;

    Character *const character = ScenarioMotionCharacter(context, params[0]);
    const std::uint16_t targetYaw = static_cast<std::uint16_t>(params[1]);
    const std::uint16_t *const motionData = ScenarioMotionData(params[2]);
    const float turnRate = PickTurnAnimation(character, targetYaw, motionData, params[5]);
    params[3] = std::bit_cast<std::uint32_t>(turnRate);
    params[4] = targetYaw;
    return AssignFacingMotion(character, targetYaw, turnRate, params[5], motionData);
}

std::int64_t CHAR_SCNFApplyFacing(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params)
        return 0;
    return AssignFacingMotion(ScenarioMotionCharacter(context, params[0]), static_cast<std::int32_t>(params[4]), std::bit_cast<float>(params[3]), params[5],
        ScenarioMotionData(params[2]));
}

void Script_ReadMOTNPositionAtFrame(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params || !params[3] || !g_pCurrentTaskData)
        return;
    auto *const output = reinterpret_cast<Vector3 *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(params[3])));
    *output = {};

    Character *const character = context && context->task && params[0] ? CHID_Find(context->task->task, params[0]) : nullptr;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    g_motnHasYValue = 1;
    MOTN_ReadRootPositionAtFrame(
        static_cast<std::uint16_t>(params[1]), static_cast<std::uint32_t>(manager->motion.basePoseTemplateIndex), std::bit_cast<float>(params[2]), *output);
}
}
