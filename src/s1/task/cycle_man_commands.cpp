#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
namespace CycleManager {





void ApplyAnimationStateCommand(CycleManager::MobjCycle *ctx, char *command) {
    if (ctx->obj)
        CycleManager::SetNodeChainYaw(*ctx->obj, ctx->desiredPathYaw);
    const int32_t *const word = reinterpret_cast<const int32_t *>(command);
    if (static_cast<uint16_t>(word[1]) != 0x618u) {
        ctx->activeAnimId = static_cast<uint16_t>(word[1]);
        ctx->facingConstraintFlag = 0;
    }
    if (word[2] != 0x618)
        ctx->selectedMotionToken = static_cast<uint32_t>(word[2]);
    ctx->pathMoveSpeed = 1.0f;
    ctx->baseMoveSpeed = 1.0f;
    ctx->activeCommandRecord = command;
    ctx->animStateMode = static_cast<uint32_t>(word[3]);
}




uint32_t UpdateEntryStateAnimPoolTrigger(CycleManager::MobjCycle *ctx, char *record, uint32_t /*areaName*/, int32_t currentTime) {
    const int32_t *const word = reinterpret_cast<const int32_t *>(record);
    const int32_t trigger = word[1] < 0 ? currentTime - word[1] : word[1];
    ctx->scheduledTriggerTime = static_cast<uint32_t>(trigger);
    ctx->entryStateCommandOffset = static_cast<int32_t>(CycleManager::FileOffset(record));
    int32_t rows = 0;
    if (currentTime < trigger && word[2]) {
        for (const int32_t *row = reinterpret_cast<const int32_t *>(CycleManager::FilePointer(static_cast<uint32_t>(word[2]))); *row != -1; row += 4)
            ++rows;
    }
    ctx->morsCommandStatus = static_cast<uint32_t>(rows);
    ctx->morsAuxOffsetOrState = -1;
    return 12;
}

bool ApplyNpcCommandState(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t recordOffset, bool *positionChanged) {
    if (positionChanged)
        *positionChanged = false;

    switch (opcode) {
    case CycleOpcode_YieldOneFrame:
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;

    case CycleOpcode_SetAreaTag: {
        uint32_t area = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, area))
            return false;
        if (npc.cycle->areaID.name != area) {
            npc.cycle->areaID.name = area;
            npc.cycle->pos = {10000.0f, 10000.0f, 10000.0f};
            if (area == CycleManager::CurrentAreaId() && CycleManager::AreaCapacityExceeded(area))
                area = 0;
            npc.cycle->areaID.name = area;
            if (positionChanged)
                *positionChanged = true;
        }
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetActiveAnimAndMotionVariant: {
        uint16_t anim = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, anim))
            return false;
        CycleManager::ApplyActiveAnimAndMotionVariant(npc, anim, false);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetAnimationState:
        CycleManager::ApplyAnimationStateCommand(npc.cycle, CycleManager::FilePointer(recordOffset));
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    case CycleOpcode_PlayRootedAnim: {
        if (npc.cycle->doorAnimActive == 1u)
            CycleManager::FreeIfDoorOpeningAnim(npc);
        CycleManager::PlayAnim_Wrapper(npc, recordOffset);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetAuxiliaryAnim: {
        PlayAnim_Wrapper_0(npc, recordOffset);
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return true;
    }
    case CycleOpcode_SetPositionAndHeading: {
        Vector3 pos{};
        int16_t yaw = 0;
        if (!CycleManager::ReadCommandPositionYaw(recordOffset, pos, yaw))
            return false;
        npc.cycle->pos = pos;
        npc.yaw = yaw;
        npc.cycle->desiredPathYaw = yaw;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        if (positionChanged)
            *positionChanged = true;
        return true;
    }
    case CycleOpcode_SetPositionAndHeadingImmediate: {
        Vector3 pos{};
        int16_t yaw = 0;
        if (!CycleManager::ReadCommandPositionYaw(recordOffset, pos, yaw))
            return false;
        npc.cycle->pos = pos;
        npc.yaw = yaw;
        npc.cycle->desiredPathYaw = yaw;
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        if (positionChanged)
            *positionChanged = true;
        return true;
    }
    case CycleOpcode_SetActionMode: {
        uint32_t mode = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, mode))
            return false;
        npc.cycle->modeValue = mode;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        if (mode != 1u)
            CycleManager::StartDoorFacingHandoff(npc);
        return true;
    }
    case CycleOpcode_SetScriptVisibleState: {
        int32_t scriptVisibleState = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, scriptVisibleState))
            return false;
        npc.cycle->contextValue = scriptVisibleState;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetModeByte: {
        uint32_t mode = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, mode))
            return false;
        npc.cycle->proximityGroup = static_cast<uint8_t>(mode);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_GetOtherChara: {
        uint32_t chara = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, chara))
            return false;
        npc.cycle->humansIdx = Humans_n_GetIndexForCharacter(chara);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_RequestNearestDoorTransition:
    case CycleOpcode_DoorUse: {
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_BeginObjectInteraction:
    case CycleOpcode_BeginLinkedObjectInteraction:
        CycleManager::QueueObjectInteraction(npc, recordOffset);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    case CycleOpcode_EndObjectInteraction:
    case CycleOpcode_EndLinkedObjectInteraction: {
        if (CycleManager::TaggedPayload *const payload = CycleManager::FindTaggedPayload(npc, CycleManager::ReadCommandDwordOrZero(recordOffset, 4)))
            payload->lifetime = 7683;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_Transform:
        CommitXForm_RotateTranslateZYX(npc, recordOffset);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    case CycleOpcode_PlayEntryStateAnimPoolUntilTime:
        npc.cycle->activeCommandRecord = CycleManager::FilePointer(recordOffset);
        CycleManager::UpdateEntryStateAnimPoolTrigger(
            npc.cycle, npc.cycle->activeCommandRecord, CycleManager::CurrentAreaId(), static_cast<int32_t>(CycleManager::CurrentTimeSeconds()));
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    case CycleOpcode_SetShopShutterState: {
        uint32_t token = 0;
        uint32_t state = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, token) || !CycleManager::ReadCommandValue(recordOffset, 8, state)) {
            return false;
        }

        npc.cycle->morsCommandArg = token;
        npc.cycle->morsCommandStatus = state;
        npc.cycle->scheduledTriggerTime = 0;
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetVehicleActiveState: {
        uint32_t vehicleToken = 0;
        Vector3 pos{};
        uint32_t rawYaw = 0;
        uint32_t enabled = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, vehicleToken) || !CycleManager::ReadCommandValue(recordOffset, 8, pos.x) ||
            !CycleManager::ReadCommandValue(recordOffset, 12, pos.y) || !CycleManager::ReadCommandValue(recordOffset, 16, pos.z) ||
            !CycleManager::ReadCommandValue(recordOffset, 20, rawYaw) || !CycleManager::ReadCommandValue(recordOffset, 24, enabled) ||
            !CycleManager::IsReasonablePosition(pos)) {
            return false;
        }

        // Opcode 0x24 controls the separate move-object runtime.  Its
        // position is not the owning cycle character's position.
        npc.vehicleToken = vehicleToken;
        npc.vehicleCommandPos = pos;
        npc.vehicleCommandYaw = static_cast<int16_t>(rawYaw & 0xFFFFu);
        npc.vehicleCommandEnabled = enabled != 0;
        npc.cycle->contextValue = static_cast<int32_t>(vehicleToken);
        npc.cycle->morsCommandArg = npc.vehicleCommandEnabled ? 1u : 0u;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SetForcedMotionToken: {
        uint32_t token = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, token))
            return false;
        npc.cycle->forcedMotionToken = token;
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_SavePointerMarker: {
        npc.cycle->branchResumeCommandPtr = CycleManager::FilePointer(recordOffset + 4u);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_StartLinkedCycle: {
        if (!CycleManager::InitializeMoveObjContext(npc, recordOffset, false))
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        else
            npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    case CycleOpcode_CreateCharacter: {
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        constexpr size_t kImageModuleNameCapacity = 12u;
        const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
        const size_t nameOffset = static_cast<size_t>(recordOffset) + 4u;
        if (nameOffset > cm.bytes.size() || kImageModuleNameCapacity > cm.bytes.size() - nameOffset) {
            return false;
        }

        const uint8_t *const nameBegin = cm.bytes.data() + nameOffset;
        const uint8_t *const nameEnd = nameBegin + kImageModuleNameCapacity;
        const uint8_t *const terminator = std::find(nameBegin, nameEnd, uint8_t{0});
        if (terminator != nameEnd) {



            std::memcpy(npc.cycle->create_chara_name, nameBegin, static_cast<size_t>(terminator - nameBegin) + 1u);
            ++g_cycleManagerRuntime.originalProgramCreateCharacterCommands;

            if (npc.cycle->filename)
                CycleManager::CreateCharacter(npc);

            uint32_t retainPendingName = 0;
            if (!CycleManager::ReadCommandValue(recordOffset, 16, retainPendingName)) {
                return false;
            }
            if (!retainPendingName)
                npc.cycle->create_chara_name[0] = 0;
        }
        return true;
    }
    case CycleOpcode_SetSteeringProfile: {
        uint32_t profile = 0;
        if (!CycleManager::ReadCommandValue(recordOffset, 4, profile))
            return false;
        npc.cycle->steeringProfile = profile != 0;
        CycleManager::RecomputeSteeringExtents(npc);
        npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
        return true;
    }
    default:
        return false;
    }
}

uint16_t WaitFramesFromCommand(uint32_t raw) {
    const uint64_t frames = static_cast<uint64_t>(raw) * 30u;
    return static_cast<uint16_t>(std::min<uint64_t>(std::numeric_limits<uint16_t>::max(), frames));
}




bool UpdateEntryStateAnimPool(CycleManager::NpcRecord &npc, uint32_t currentTime) {
    const int32_t rowCount = static_cast<int32_t>(npc.cycle->morsCommandStatus);
    Character *const local = npc.cycle->motnSteeringActive ? npc.cycle->obj : nullptr;
    MotionInformationWork *const motion = npc.cycle->motnSteeringActive ? npc.cycle->mot : nullptr;
    if (local)
        CycleManager::EnableRootMotion(local, true);
    const int32_t previous = npc.cycle->morsAuxOffsetOrState;
    if (motion && (motion->motionFlags & 2u) == 0 && previous != -1)
        return false;
    const bool triggered = static_cast<int32_t>(currentTime) >= static_cast<int32_t>(npc.cycle->scheduledTriggerTime);
    if (triggered || rowCount <= 0)
        return triggered;

    const char *const command = CycleManager::FilePointer(static_cast<uint32_t>(npc.cycle->entryStateCommandOffset));
    const char *const rows = CycleManager::FilePointer(*reinterpret_cast<const uint32_t *>(command + 8));
    int32_t rowIndex = 0;
    if (previous != -1) {
        const int32_t advanceMax = *reinterpret_cast<const int32_t *>(rows + 16 * previous + 12);
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        rowIndex = (static_cast<int32_t>((static_cast<uint32_t>(g_EnvPrngSeed) >> 16) & 0x7FFFu) % (advanceMax + 1) + previous + 1) % rowCount;
    }
    const char *const row = rows + 16 * rowIndex;
    float playbackScale = *reinterpret_cast<const float *>(row + 4);
    const uint16_t animId = *reinterpret_cast<const uint16_t *>(row);
    if (std::fabs(playbackScale) < 0.00000011920929f)
        playbackScale = npc.cycle->moveConfig->motionRadiusTimingScalar;
    if (local && motion) {
        if (npc.cycle->activeAnimId == animId) {
            if ((*reinterpret_cast<const uint32_t *>(row + 8) & 0x10000u) != 0)
                motion->motionFlags |= 0x80u;
            motion->motionFlags &= ~2u;
            motion->playbackRate = playbackScale;
            motion->pendingAnimId = animId;
        } else {
            const std::uint32_t animationFlags = animId | ((*reinterpret_cast<const std::uint32_t *>(row + 8) << 15) & 0x80000000U);
            freeTalkConv_SetMoveAndFacingConstraint(
                *local, npc.cycle->pos, npc.cycle->desiredPathYaw, static_cast<std::int32_t>(animationFlags), 0.0f, playbackScale, 0.0f, false);
        }
    }
    npc.cycle->activeAnimId = animId;
    npc.cycle->morsAuxOffsetOrState = rowIndex;
    return false;
}


void FinishTargetApproachAndStopSounds(CycleManager::NpcRecord &npc) {
    npc.cycle->pos = npc.cycle->moveTargetPos;
    npc.cycle->desiredPathYaw = npc.cycle->moveTargetYaw;
    npc.cycle->baseMoveSpeed = 0.0f;
    if (npc.cycle->primarySoundBankId >= 0) {
        ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
        npc.cycle->primarySoundBankId = -1;
    }
    if (npc.cycle->secondarySoundBankId >= 0) {
        ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->secondarySoundBankId));
        npc.cycle->secondarySoundBankId = -1;
    }
}





bool TickTargetApproachWithSound(CycleManager::NpcRecord &npc) {
    if (!npc.cycle->motnSteeringActive) {
        CycleManager::FinishTargetApproachAndStopSounds(npc);
        return false;
    }
    npc.cycle->flags &= ~2u;
    if (npc.cycle->primarySoundBankId >= 0)
        PerformBikeSoundUpdate(npc, npc.cycle->baseMoveSpeed);
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    Vector3 direction{
        npc.cycle->moveTargetPos.x - npc.cycle->pos.x, npc.cycle->moveTargetPos.y - npc.cycle->pos.y, npc.cycle->moveTargetPos.z - npc.cycle->pos.z};
    float distance = 0.0f;
    if (direction.x > epsilon || direction.x < -epsilon || direction.z > epsilon || direction.z < -epsilon)
        distance = Vector3::NormalizeInPlace(direction);
    else
        direction = {};
    const float speed = std::max(0.0092592593f, (distance / npc.cycle->workFloat4C) * npc.cycle->approachStartSpeed);
    bool approaching = false;
    if (speed > distance) {
        CycleManager::FinishTargetApproachAndStopSounds(npc);
    } else {
        npc.cycle->baseMoveSpeed = speed;
        npc.cycle->pos.x = direction.x * speed + npc.cycle->pos.x;
        npc.cycle->pos.z = direction.z * speed + npc.cycle->pos.z;
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
        const int32_t turn = static_cast<int16_t>(static_cast<uint16_t>(npc.cycle->moveTargetYaw) - static_cast<uint16_t>(npc.cycle->desiredPathYaw));
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + std::clamp(turn, -364, 364));
        approaching = true;
    }
    if (npc.cycle->obj) {
        shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
        CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
    }
    return approaching;
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {




void npcUpdateBaseSpeed(CycleManager::NpcRecord &npc, uint16_t pathIndex) {
    float speed = npc.cycle->moveObjSpeedParam;
    const float base = npc.cycle->baseMoveSpeed;
    const BikeSteeringState state = npc.cycle->bikeSteeringState;
    if (static_cast<int32_t>(npc.cycle->reversePathFrames) <= 0) {
        if (state == BikeSteering_SlowForTurn || state == BikeSteering_SharpTurn) {
            float turnSpeed = 0.0f;
            if (npc.cycle->moveObjRuntime)
                turnSpeed = npc.cycle->moveObjRuntime->moveObjectKind == 5u ? 0.13888890f : speed * 0.5f;
            else
                turnSpeed = npc.cycle->moveConfig->cycleClassCode == 6u ? 0.23148148f : 0.32407409f;
            npc.cycle->baseMoveSpeed = std::max(0.0f, base > std::min(speed, turnSpeed) ? 0.89999998f * base : 1.1f * base);
            return;
        }
        if (static_cast<uint8_t>(state - 1u) <= 2u) {
            npc.cycle->baseMoveSpeed = std::max(0.0f, std::min(npc.cycle->approachStartSpeed, speed));
            return;
        }
        constexpr float crawlSpeed = 0.027777778f;
        if (state == BikeSteering_Crawl) {
            npc.cycle->baseMoveSpeed = std::max(0.0f, base > crawlSpeed ? (base - crawlSpeed) * 0.89999998f + crawlSpeed : crawlSpeed);
            return;
        }
        if (static_cast<int32_t>(npc.cycle->pathPointCount) - static_cast<int32_t>(pathIndex) <= 1) {
            npc.cycle->baseMoveSpeed = std::max(0.0f, base > crawlSpeed ? (base - crawlSpeed) * 0.995f + crawlSpeed : crawlSpeed);
            return;
        }
    }
    if (speed > base)
        speed = (speed - base) * 0.005f + base;
    npc.cycle->baseMoveSpeed = std::max(0.0f, speed);
}



float SnapshotMoveObjTransform(CycleManager::NpcRecord &npc, bool live) {
    float speed = npc.cycle->moveObjSpeedParam;
    if (live) {
        const float dx = npc.cycle->lastMoveObjPos.x - npc.cycle->pos.x;
        const float dz = npc.cycle->lastMoveObjPos.z - npc.cycle->pos.z;
        speed = std::sqrt(dz * dz + dx * dx);
        npcUpdateBaseSpeed(npc, static_cast<uint16_t>(npc.cycle->pathPointIndex));
    } else {
        npc.cycle->baseMoveSpeed = speed;
    }
    npc.cycle->lastMoveObjPos = npc.cycle->pos;
    npc.cycle->lastMoveObjYaw = npc.cycle->desiredPathYaw;
    return speed;
}

float DistanceXZWithEpsilon(const Vector3 &lhs, const Vector3 &rhs) {
    float deltaX = lhs.x - rhs.x;
    float deltaZ = lhs.z - rhs.z;
    if (std::abs(deltaX) < kCyclePositionEpsilon)
        deltaX = 0.0f;
    if (std::abs(deltaZ) < kCyclePositionEpsilon)
        deltaZ = 0.0f;
    return std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
}

bool TickFixedStepMoveToTarget(CycleManager::NpcRecord &npc) {
    float deltaX = npc.cycle->moveTargetPos.x - npc.cycle->pos.x;
    float deltaZ = npc.cycle->moveTargetPos.z - npc.cycle->pos.z;
    if (std::abs(deltaX) < kCyclePositionEpsilon)
        deltaX = 0.0f;
    if (std::abs(deltaZ) < kCyclePositionEpsilon)
        deltaZ = 0.0f;

    const float distance = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
    if (distance < kCyclePositionStep) {
        npc.cycle->pos = npc.cycle->moveTargetPos;
        npc.yaw = npc.cycle->moveTargetYaw;
        npc.cycle->desiredPathYaw = npc.cycle->moveTargetYaw;
        npc.cycle->activeAnimId = npc.cycle->tmpReqAnimId;
        npc.cycle->xmptResumePending = 0;
        return false;
    }

    const float inverseDistance = 1.0f / distance;
    float directionX = deltaX * inverseDistance;
    float directionZ = deltaZ * inverseDistance;
    if (std::abs(directionX) < kCyclePositionEpsilon)
        directionX = 0.0f;
    if (std::abs(directionZ) < kCyclePositionEpsilon)
        directionZ = 0.0f;
    npc.cycle->pos.x += directionX * kCyclePositionStep;
    npc.cycle->pos.z += directionZ * kCyclePositionStep;
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
    npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(-directionX, -directionZ);
    npc.yaw = npc.cycle->desiredPathYaw;
    return true;
}

uint32_t RandomPositionMoveRetryFrames() {
    uint32_t &seed = g_EnvPrngSeed;
    seed = LcgStep(seed);
    return 30u * ((((static_cast<uint32_t>(seed) >> 16) & 0x7FFFu) % 3u) + 1u);
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




bool TickPositionAndHeadingMove(CycleManager::NpcRecord &npc) {
    if (npc.cycle->moveConfig->cycleClassCode >= 4)
        return CycleManager::TickTargetApproachWithSound(npc);
    bool active = true;
    npc.cycle->resumePositionCapturePending = true;
    Character *chara = nullptr;
    MotionInformationWork *motion = nullptr;
    if (npc.cycle->motnSteeringActive) {
        motion = npc.cycle->mot;
        chara = npc.cycle->obj;
        if (motion) {
            npc.cycle->activeAnimId = motion->currentAnimId;
            CycleManager::EnableRootMotion(chara, true);
            npc.cycle->pos = chara ? GetPositionForEntity(chara->entity.node) : Vector3{};
            npc.cycle->desiredPathYaw = CycleManager::NodeChainYaw(chara);
        }
    }
    uint16_t animId = npc.cycle->moveConfig->motionParamIds[0];
    const bool blocked = chara && charaHasBlockingTPMRequest(chara);
    switch (npc.cycle->xmptResumePending) {
    case 0:
        npc.cycle->xmptResumePending = 1;
        active = CycleManager::TickFixedStepMoveToTarget(npc);
        break;
    case 1:
        if (!chara || !motion) {
            active = CycleManager::TickFixedStepMoveToTarget(npc);
            break;
        }
        if (!blocked) {
            npc.cycle->workFloat4C = 1.0f;
            CycleManager::SubmitXMPTMoveWithCycleParams(npc, *chara, npc.cycle->moveTargetPos, npc.cycle->moveTargetYaw, npc.cycle->tmpReqAnimId);
            npc.cycle->steeringBlockedFrames = 0;
            npc.cycle->xmptResumePending = 3;
        }
        return true;
    case 2:
    case 4: {
        const int32_t frames = static_cast<int32_t>(npc.cycle->steeringBlockedFrames) > 0 ? static_cast<int32_t>(npc.cycle->steeringBlockedFrames) - 1 : 0;
        npc.cycle->steeringBlockedFrames = static_cast<uint32_t>(frames);
        if (chara && motion) {
            if (blocked) {
                (void)CycleManager::TickFixedStepMoveToTarget(npc);
            } else if (frames <= 0) {
                Vector3 target = npc.cycle->moveTargetPos;
                int16_t yaw = npc.cycle->moveTargetYaw;
                if (npc.cycle->workFloat4C >= 0.0f) {
                    animId = npc.cycle->tmpReqAnimId;
                    npc.cycle->xmptResumePending = 3;
                } else {
                    npc.cycle->xmptResumePending = 5;
                    float deltaX = npc.cycle->currentWaypointPos.x - npc.cycle->pos.x;
                    float deltaZ = npc.cycle->currentWaypointPos.z - npc.cycle->pos.z;
                    if (std::fabs(deltaX) < kCyclePositionEpsilon)
                        deltaX = 0.0f;
                    if (std::fabs(deltaZ) < kCyclePositionEpsilon)
                        deltaZ = 0.0f;
                    yaw = std::fabs(deltaX) >= kCyclePositionEpsilon || std::fabs(deltaZ) >= kCyclePositionEpsilon ? CycleManager::Atan2Fixed16(deltaX, deltaZ) :
                                                                                                                     npc.cycle->desiredPathYaw;
                    target = npc.cycle->currentWaypointPos;
                }
                CycleManager::SubmitXMPTMoveWithCycleParams(npc, *chara, target, yaw, animId);
            }
            return true;
        }
        npc.cycle->xmptResumePending = 1;
        active = CycleManager::TickFixedStepMoveToTarget(npc);
        break;
    }
    case 3:
    case 5: {
        if (!chara || !motion) {
            UpdateEntityOverPositionWithFlags(npc, false);
            npc.cycle->steeringHalfDepth = 0.0f;
            active = CycleManager::TickFixedStepMoveToTarget(npc);
            if (active)
                return true;
            npc.cycle->xmptResumePending = 1;
            break;
        }
        const bool forward = npc.cycle->workFloat4C >= 0.0f;
        const Vector3 target = forward ? npc.cycle->moveTargetPos : npc.cycle->currentWaypointPos;
        const uint16_t targetAnimId = forward ? npc.cycle->tmpReqAnimId : animId;
        npc.cycle->xmptResumePending = forward ? 3u : 5u;
        npc.cycle->activeAnimId = motion->currentAnimId;
        if (charaIsActiveMode(chara, 4)) {
            if (CycleManager::DistanceXZWithEpsilon(npc.cycle->steeringTarget, npc.cycle->pos) >= 0.02f) {
                if (static_cast<int32_t>(--npc.cycle->steeringBlockedFrames) < 0)
                    npc.cycle->steeringBlockedFrames = 0;
            } else if (!blocked && ++npc.cycle->steeringBlockedFrames > 7u) {
                const uint8_t resumeState = npc.cycle->xmptResumePending;
                UpdateEntityOverPositionWithFlags(npc, true);
                npc.cycle->steeringHalfDepth = 0.0f;
                npc.cycle->xmptResumePending = resumeState;
                if (resumeState == 3u) {
                    npc.cycle->workFloat4C = -1.0f;
                    npc.cycle->xmptResumePending = 4;
                    npc.cycle->steeringBlockedFrames = 0;
                } else {
                    npc.cycle->workFloat4C = 1.0f;
                    npc.cycle->xmptResumePending = 2;
                    npc.cycle->steeringBlockedFrames = CycleManager::RandomPositionMoveRetryFrames();
                }
            }
            return true;
        }
        if (CycleManager::DistanceXZWithEpsilon(npc.cycle->pos, target) >= 0.1f) {
            if (forward) {
                npc.cycle->xmptResumePending = 4;
                npc.cycle->steeringBlockedFrames = 0;
                freeTalkConv_SetMoveAndFacingConstraint(
                    *chara, npc.cycle->pos, npc.cycle->desiredPathYaw, animId, 0.0f, npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, false);
                npc.cycle->activeAnimId = animId;
                npc.cycle->workFloat4C = -1.0f;
            } else {
                npc.cycle->xmptResumePending = 2;
                npc.cycle->workFloat4C = 1.0f;
                npc.cycle->steeringBlockedFrames = CycleManager::RandomPositionMoveRetryFrames();
            }
            return true;
        }
        if (forward) {
            active = false;
            npc.cycle->xmptResumePending = 0;
            npc.cycle->workFloat4C = 0.0f;
            npc.cycle->activeAnimId = targetAnimId;
        } else {
            npc.cycle->xmptResumePending = 2;
            npc.cycle->workFloat4C = 1.0f;
            npc.cycle->activeAnimId = targetAnimId;
            npc.cycle->steeringBlockedFrames = CycleManager::RandomPositionMoveRetryFrames();
        }
        break;
    }
    default:
        active = blocked;
        break;
    }
    if (!active && !npc.cycle->moveObjRuntime)
        npc.cycle->steeringHalfDepth = npc.cycle->savedSteeringHalfDepth;
    return active;
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {

void RequestTransitionAnimation(MotionInformationWork &motion, uint16_t animation) {
    motion.motionFlags &= ~2u;
    motion.pendingAnimId = animation;
    motion.blendFrameCount = 20;
    motion.blendMode = kCycleWalkCubicBlend;
    motion.playbackRate = 1.0f;
}





void TickWaitTurnAnimation(CycleManager::NpcRecord &npc) {
    MotionManagerWork *const manager = npc.cycle->obj ? MOTM_GetWork(npc.cycle->obj) : nullptr;
    MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    if (npc.cycle->motnSteeringActive && motion && (motion->motionFlags & 2u) == 0) {
        return;
    }

    npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + static_cast<int16_t>(npc.cycle->moveObjCommandOffset));
    npc.cycle->activeAnimId = static_cast<uint16_t>(npc.cycle->foundExistingMoveObjRuntime);
    if (!motion)
        return;

    motion->motionFlags &= ~0x80u;
    motion->pendingTurnEulerDelta.y = 0;
    if (!npc.cycle->motnSteeringActive || !npc.cycle->obj)
        return;

    npc.cycle->pos = GetPositionForEntity(npc.cycle->obj->entity.node);
    (void)CycleManager::ApplyActiveAnimRootYaw(npc);
    if (motion->currentAnimId == npc.cycle->activeAnimId)
        return;

    CycleManager::RequestTransitionAnimation(*motion, npc.cycle->activeAnimId);
    if (npc.cycle->activeAnimId == npc.cycle->moveConfig->motionParamIds[8]) {
        motion->forcedFrameTime = 8.0f;
        return;
    }
    npc.cycle->entryStateCommandOffset = static_cast<int16_t>(npc.cycle->currentOpcode);
    npc.cycle->currentOpcode = CycleOpcode_WaitMotionFullRestore;
}

bool TransitionSegmentStaysInsideUnitCircle(const Vector3 &start, const Vector3 &end) {
    const float startMagnitude = start.x * start.x + start.z * start.z;
    const float endMagnitude = end.x * end.x + end.z * end.z;
    if (startMagnitude < 1.0f && endMagnitude < 1.0f)
        return true;
    if (startMagnitude >= 1.0f)
        return false;

    const float dx = start.x - end.x;
    const float dz = start.z - end.z;
    return startMagnitude > dx * dx + dz * dz;
}



void TickWaitOutsideRadius(CycleManager::NpcRecord &npc) {
    if (CycleManager::TransitionSegmentStaysInsideUnitCircle(npc.cycle->pos, npc.cycle->moveTargetPos)) {
        return;
    }

    npc.cycle->currentOpcode = static_cast<CycleOpcode>(npc.cycle->bikeTokenOffset);
    if (npc.cycle->activeAnimId == static_cast<uint32_t>(npc.cycle->requestAnimId))
        return;
    npc.cycle->activeAnimId = static_cast<uint16_t>(npc.cycle->requestAnimId);
    if (npc.cycle->obj) {
        if (MotionManagerWork *const manager = MOTM_GetWork(npc.cycle->obj))
            CycleManager::RequestTransitionAnimation(manager->motion, npc.cycle->activeAnimId);
    }
}



void CompleteTransitionDelta(CycleManager::NpcRecord &npc) {
    npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + static_cast<int16_t>(npc.cycle->moveObjCommandOffset));
    npc.cycle->activeAnimId = static_cast<uint16_t>(npc.cycle->foundExistingMoveObjRuntime);
    npc.cycle->baseMoveSpeed = 0.0f;

    MotionManagerWork *const manager = npc.cycle->obj ? MOTM_GetWork(npc.cycle->obj) : nullptr;
    if (!manager)
        return;
    MotionInformationWork &motion = manager->motion;
    motion.blendMode = kCycleWalkCubicBlend;
    motion.blendFrameCount = 20;
    motion.pendingTurnEulerDelta.y = 0;
    motion.playbackRate = 1.0f;
    if (static_cast<int32_t>(npc.cycle->moveObjCommandOffset) < 0) {
        motion.motionFlags &= ~0x80u;
        motion.trackedNodeIndex = 6;
        motion.forcedFrameTime = static_cast<float>(MOTN_getFrameCountFromAnimId(npc.cycle->activeAnimId)) * 0.5f + 1.0f;
    } else {
        motion.trackedNodeIndex = 7;
        motion.forcedFrameTime = 1.0f;
    }
    CycleManager::RequestTransitionAnimation(motion, npc.cycle->activeAnimId);

    if (npc.cycle->motnSteeringActive && npc.cycle->obj) {
        manager->flags &= ~1u;
        npc.cycle->pos = GetPositionForEntity(npc.cycle->obj->entity.node);
        (void)CycleManager::ApplyActiveAnimRootYaw(npc);
    }
}



void TickTransitionDelta(CycleManager::NpcRecord &npc) {
    npc.cycle->pos.x += npc.cycle->moveObjRuntimeOffset;
    npc.cycle->pos.y += std::bit_cast<float>(npc.cycle->moveObjActivationPending);
    npc.cycle->pos.z += std::bit_cast<float>(npc.cycle->bikeTokenOffset);

    MotionManagerWork *const manager = npc.cycle->obj ? MOTM_GetWork(npc.cycle->obj) : nullptr;
    if (!npc.cycle->motnSteeringActive || !manager) {
        CycleManager::CompleteTransitionDelta(npc);
        return;
    }

    const MotionInformationWork &motion = manager->motion;
    if ((motion.motionFlags & kCycleMotionTransitionActive) == 0 && motion.frameTimer >= 10.0f &&
        npc.cycle->activeAnimId != npc.cycle->moveConfig->motionParamIds[8]) {
        CycleManager::CompleteTransitionDelta(npc);
    }
}

void TickWaitMotionFullRestore(CycleManager::NpcRecord &npc) {
    MotionManagerWork *const manager = npc.cycle->obj ? MOTM_GetWork(npc.cycle->obj) : nullptr;
    if (!manager || (manager->motion.motionFlags & kCycleMotionTransitionActive) == 0) {
        npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
    }
}


bool IsGlobalSimulationEnabled(const ScenarioFunctionWork *scnf) {
    if (g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0)
                return true;
        }
    }
    return scnf && ((scnf->flags >> 17U) & 1U) != 0;
}



bool SimSetSkipBlend(CycleManager::NpcRecord &npc, const ScenarioFunctionWork *scnf, bool globalSimulation) {
    if (globalSimulation) {
        if (SCEN_HasActiveRuntimeObject() && scnf && (scnf->flags & 0x20000U) != 0) {
            const std::span<const std::uint32_t> referenced = SCEN_ReferencedCharacterIds();
            for (size_t i = 0; i < referenced.size() && i < 0x18u && referenced[i]; ++i) {
                if (referenced[i] == npc.cycle->moveConfig->obj_id.name) {
                    if (!npc.cycle->xm_resume_state)
                        npc.cycle->xm_resume_state = 1;
                    g_scntReferencedCycleHeld = 1;
                    return true;
                }
            }
        }
    } else {
        g_bGlobalFlag = 0;
        g_cycleConversationHoldActive = 0;
    }
    if (!npc.cycle->xm_resume_state)
        return false;
    if (!globalSimulation)
        npc.cycle->xm_resume_state = 0;
    return true;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {





uint32_t MOBJ_executeTaskInternal(CycleManager::MobjEntry *CM, int32_t time, uint32_t motnSteeringActive, uint32_t skipBlend) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(CM);
    CycleManager::MobjCycle *const ctx = CM->ctx;
    char *command = ctx->cycleCommandPtr;
    if (!command) {
        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        return 0;
    }
    const uint8_t proximityGroup = ctx->proximityGroup;
    MotionInformationWork *const mot = npc.cycle->mot;
    Character *const obj = ctx->obj;
    int32_t currentOpcode = static_cast<int16_t>(ctx->currentOpcode);
    const int32_t entryOpcode = currentOpcode;
    if (obj) {
        obj->eventGroup = proximityGroup;
        SelectCharacterMapQuery(obj);
        g_lastProximityGroup = -1;
    } else if (g_lastProximityGroup != static_cast<int8_t>(proximityGroup)) {
        mapQuerySelectActiveGridSection(proximityGroup);
        g_lastProximityGroup = static_cast<int8_t>(proximityGroup);
    }
    ctx->xformDirty = 0;
    if (skipBlend) {
        CycleManager::HoldForConversation(npc);
        return 1;
    }

    // ground: the live tail that sets the character on the ground under it; commands: the state ended and the program goes on.
    bool ground = true;
    bool commands = false;
    if (currentOpcode == CycleOpcode_InitTimedPath) {
        if (motnSteeringActive) {
            if (!CycleManager::UpdateMoveObjectPathing(npc, false))
                currentOpcode = CycleOpcode_RemoveCharacter;
        } else {
            ground = false;
            commands = !npcUpdateMovementPath(npc);
        }

        if (currentOpcode == CycleOpcode_RemoveCharacter || commands) {
            if (ctx->moveObjPathActive) {
                ctx->currentOpcode = CycleOpcode_StartLinkedCycle;
                ctx->moveObjSubOpcode = 0;
                return 1;
            }
        }
    } else if (ctx->currentOpcode == CycleOpcode_UseMnrBEntry) {
        if (CycleManager::TickUseMnrbEntry(npc, motnSteeringActive, time))
            currentOpcode = CycleOpcode_RemoveCharacter;
    } else if (ctx->currentOpcode == CycleOpcode_ScheduleMnrBEntry) {
        const uint16_t kind = static_cast<uint16_t>(ctx->morsCommandStatus >> 16);
        if (kind == 0     ? CycleManager::TickMnrbBusStopOwner(npc) :
                kind == 1 ? CycleManager::TickMnrbCounterOwner(npc, motnSteeringActive, time) :
                            kind != 3 || CycleManager::TickMnrbQueueOwner(npc, motnSteeringActive))
            currentOpcode = CycleOpcode_RemoveCharacter;
    } else if (ctx->currentOpcode == CycleOpcode_PlayEntryStateAnimPoolUntilTime) {
        if (CycleManager::UpdateEntryStateAnimPool(npc, static_cast<uint32_t>(time)))
            currentOpcode = CycleOpcode_RemoveCharacter;
    } else {
        switch (currentOpcode) {
        case CycleOpcode_SetActiveAnimAndMotionVariant:
            if (motnSteeringActive && mot && (mot->motionFlags & 2u) == 0) {
                npcSetMoveFacingMot(npc, true, false);
                break;
            }
            if (motnSteeringActive && obj && mot)
                ctx->pos = GetPositionForEntity(obj->entity.node);
            currentOpcode = CycleOpcode_RemoveCharacter;
            break;
        case CycleOpcode_SetPositionAndHeading:
            if (CycleManager::TickPositionAndHeadingMove(npc))
                break;
            if (!motnSteeringActive || !mot || ctx->tmpReqAnimId == ctx->moveConfig->motionParamIds[0])
                currentOpcode = CycleOpcode_RemoveCharacter;
            else
                ctx->currentOpcode = CycleOpcode_SetActiveAnimAndMotionVariant;
            break;
        case CycleOpcode_ScheduleBoundary:
        case CycleOpcode_WaitHold:
            // The next timetable row once its time has passed, else the branch resume point; with neither the cycle waits.
            if (CycleTimetableRow *const row = CM->currentScheduleEntry) {
                if (row->timeOrEndMarker < time && row->timeOrEndMarker != -1) {
                    CM->currentScheduleEntry = row + 1;
                    command = CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset));
                    ctx->branchResumeCommandPtr = nullptr;
                    currentOpcode = CycleOpcode_RemoveCharacter;
                } else if (ctx->branchResumeCommandPtr) {
                    command = ctx->branchResumeCommandPtr;
                    currentOpcode = CycleOpcode_RemoveCharacter;
                }
            }
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            if (ctx->baseMoveSpeed < 0.1f)
                ctx->baseMoveSpeed = 1.0f;
            npcSetMoveFacingMot(npc, true, false);
            break;
        case CycleOpcode_RideVehiclePath2D:
            if (!ctx->moveObjRuntime || !CycleManager::TickBikeMoveObjectRuntime(npc, motnSteeringActive)) {
                currentOpcode = CycleOpcode_RemoveCharacter;
                break;
            }
            if (!obj)
                break;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            if (mot) {
                CycleManager::EnableRootMotion(obj, true);
                if (mot->currentAnimId != ctx->activeAnimId) {
                    mot->blendMode = 1;
                    mot->blendFrameCount = 10;
                    mot->motionFlags &= ~2u;
                    mot->pendingAnimId = ctx->activeAnimId;
                }
            }
            shenApplyTranslationToAll(obj->entity.node, ctx->pos);
            ApplyEulerAnglesToNode(obj->entity.node, ctx->moveObjRuntime->renderEuler);
            break;
        case CycleOpcode_WaitSeconds:
            if (time > static_cast<int32_t>(ctx->steeringWaitFrames) && (!motnSteeringActive || !mot || (mot->motionFlags & 2u) != 0))
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            if (ctx->baseMoveSpeed < 0.1f)
                ctx->baseMoveSpeed = 1.0f;
            npcSetMoveFacingMot(npc, true, false);
            break;
        case CycleOpcode_BeginTalkMoveToTaggedPosition:
            if (!CycleManager::UpdateTalkMoveObjectState(npc, motnSteeringActive, false)) {
                currentOpcode = CycleOpcode_RemoveCharacter;
                ++cm.talkMoveCompletions;
            }
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            cyclUpdateCharaMoveRequestForAnimChange(npc);
            if (mot)
                mot->playbackRate = ctx->baseMoveSpeed;
            break;
        case CycleOpcode_WaitForTaggedCycleInRadius:
        case CycleOpcode_WaitForTaggedTalkMoveInRadius:
            if (CycleManager::UpdateTaggedWait(npc, static_cast<uint32_t>(time)))
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            if (ctx->baseMoveSpeed < 0.1f)
                ctx->baseMoveSpeed = 1.0f;
            npcSetMoveFacingMot(npc, true, false);
            break;
        case CycleOpcode_RequestNearestDoorTransition:
        case CycleOpcode_DoorUse:
            if (!CycleManager::PollDoorRequest(npc)) {
                currentOpcode = CycleOpcode_RemoveCharacter;
                if (!motnSteeringActive)
                    commands = true;
            }
            break;
        case CycleOpcode_FollowStairPath2D: {
            const bool walking = npcKaidanUpdateStairWalk(npc, motnSteeringActive);
            if (!walking) {
                currentOpcode = CycleOpcode_RemoveCharacter;
            }
            if (motnSteeringActive)
                npcKaidanSetStairMotion(npc);
            if (!obj)
                break;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            shenApplyTranslationToAll(obj->entity.node, ctx->pos);
            CycleManager::SetNodeChainYaw(*obj, ctx->desiredPathYaw);
            break;
        }
        case CycleOpcode_StartLinkedCycle:
            if (CycleManager::ExecuteMoveObjState(npc, motnSteeringActive != 0, time))
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            switch (ctx->moveObjPhase) {
            case CycleMoveObjPhase_BeginPath:
            case CycleMoveObjPhase_ProcessSubcommands:
            case CycleMoveObjPhase_ProcessSubcommandsEnd:
            case CycleMoveObjPhase_BeginReturnPath:
            case CycleMoveObjPhase_ResumePath:
                break;
            case CycleMoveObjPhase_SetIdleAnim:
                if (!ctx->b_paused)
                    npcSetMoveFacingMot(npc, true, false);
                break;
            case CycleMoveObjPhase_SnapToAnchor:
                if (!ctx->b_paused)
                    npcSetMoveFacingMot(npc, true, ctx->facingConstraintFlag);
                break;
            case CycleMoveObjPhase_ProcessSubcommandsActive:
                npcSetMoveFacingMot(npc, false, false);
                break;
            case CycleMoveObjPhase_CalcReturnAnchor:
                if (mot && mot->previousAnimId == 0xFFFFu)
                    mot->playbackRate = 0.0f;
                break;
            case CycleMoveObjPhase_SetReturnArea:
                npcSetMoveFacingMot(npc, true, ctx->facingConstraintFlag);
                break;
            default:
                npcSetMoveFacingMot(npc, true, false);
                break;
            }
            break;
        case CycleOpcode_PlayRootedAnim:
            if (!*reinterpret_cast<const int32_t *>(ctx->activeCommandRecord + 0x14) || !motnSteeringActive || !ctx->currentAnimToken || !mot ||
                (mot->motionFlags & 0x200u) != 0)
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            npcSetMoveFacingMot(npc, true, false);
            break;
        case CycleOpcode_SetAnimationState:
            if (!motnSteeringActive || !mot || (mot->motionFlags & 2u) != 0)
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            npcSetMoveFacingMot(npc, true, false);
            break;
        case CycleOpcode_DriveVehiclePath2D: {
            const bool driving = DoBikeStuff_0(npc, motnSteeringActive);
            if (!driving) {
                currentOpcode = CycleOpcode_RemoveCharacter;
            }
            if (!obj)
                break;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            shenApplyTranslationToAll(obj->entity.node, ctx->pos);
            CycleManager::SetNodeChainYaw(*obj, ctx->desiredPathYaw);
            break;
        }
        case CycleOpcode_DriveForkliftPath2D:
            if (!ctx->moveObjRuntime || !DoBikeStuff(npc, motnSteeringActive)) {
                currentOpcode = CycleOpcode_RemoveCharacter;
                break;
            }
            if (!obj)
                break;
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            npcSetMoveFacingMot(npc, false, false);
            shenApplyTranslationToAll(obj->entity.node, ctx->pos);
            ApplyEulerAnglesToNode(obj->entity.node, ctx->moveObjRuntime->renderEuler);
            break;
        case CycleOpcode_WaitTurnAnimation:
            CycleManager::TickWaitTurnAnimation(npc);
            break;
        case CycleOpcode_WaitOutsideRadius:
            CycleManager::TickWaitOutsideRadius(npc);
            break;
        case CycleOpcode_WalkPath3D:
            if (!CycleManager::TickPathFollowMotion(npc, motnSteeringActive)) {
                currentOpcode = CycleOpcode_RemoveCharacter;
                if (ctx->moveObjPathActive) {
                    ctx->currentOpcode = CycleOpcode_StartLinkedCycle;
                    ctx->moveObjSubOpcode = 0;
                    return 1;
                }
            }
            break;
        case CycleOpcode_SetVehicleActiveState:
            if (!CycleManager::TickVehicleActiveState(npc))
                currentOpcode = CycleOpcode_RemoveCharacter;
            if (!ctx->obj)
                break;
            shenApplyTranslationToAll(ctx->obj->entity.node, ctx->pos);
            CycleManager::SetNodeChainYaw(*ctx->obj, ctx->desiredPathYaw);
            break;
        case CycleOpcode_ApplyTransitionDelta:
            CycleManager::TickTransitionDelta(npc);
            break;
        case CycleOpcode_WaitMotionFullRestore:
            CycleManager::TickWaitMotionFullRestore(npc);
            break;
        case CycleOpcode_ScriptTransformOverride:
            if (!motnSteeringActive) {
                ground = false;
                break;
            }
            npcSetMoveFacingMot(npc, true, false);
            if (!ctx->obj)
                break;
            shenApplyTranslationToAll(ctx->obj->entity.node, ctx->pos);
            CycleManager::SetNodeChainYaw(*ctx->obj, ctx->desiredPathYaw);
            break;
        case CycleOpcode_SetShopShutterState:
            if (cm.shutRows.empty() || !CycleManager::DispatchSceneTask(ctx->morsCommandArg, ctx->morsCommandStatus, 0))
                currentOpcode = CycleOpcode_RemoveCharacter;
            break;
        case CycleOpcode_AuxiliaryAnimationTransition:
            PlayAnim_Wrapper_2(npc, motnSteeringActive);
            break;
        case CycleOpcode_SetAuxiliaryAnim:
            break;
        case CycleOpcode_BikeDismountInteraction:
            CycleManager::TickBikeMoveObjectTransition(npc);
            break;
        case CycleOpcode_BikeMoveInteraction:
            CycleManager::TickBikeMoveObjectRelease(npc);
            break;
        case CycleOpcode_FalseReturnMarker:
            if (!motnSteeringActive)
                currentOpcode = CycleOpcode_RemoveCharacter;
            break;
        default:
            currentOpcode = CycleOpcode_RemoveCharacter;
            break;
        }
    }

    if (!commands) {
        if (ground && motnSteeringActive)
            CycleManager::ApplyNpcRecordToLocal(npc);
        if (currentOpcode != CycleOpcode_RemoveCharacter && ctx->currentOpcode != CycleOpcode_RemoveCharacter)
            return 1;
    }

    if (entryOpcode > 46 || ((0x610000038300ull >> entryOpcode) & 1u) == 0)
        ctx->snapToPathStart = 0;

    // The commands: each sets the cycle's state and ends the pass, except the area-conditional ones of another area, a 0x3D of a
    // pass that is not live, and a timetable row or branch resume point taken at a 4 or 0x12. The end marker ends the program.
    const uint32_t area = CycleManager::CurrentAreaId();
    while (true) {
        char *const record = command;
        const int32_t opcode = *reinterpret_cast<const int32_t *>(record);
        const int32_t *const word = reinterpret_cast<const int32_t *>(record);
        const uint32_t recordOffset = CycleManager::FileOffset(record);
        command = record + 4;
        ctx->currentOpcode = static_cast<CycleOpcode>(opcode);
        if (opcode == 0 || opcode >= 62)
            return 0;

        bool waitForRow = false;
        switch (opcode) {
        case CycleOpcode_UseMnrBEntry:
            ctx->activeCommandRecord = record;
            ctx->cycleCommandPtr = record + CycleManager::BeginUseMnrbEntry(npc, record, motnSteeringActive, time);
            return 1;
        case CycleOpcode_ScheduleMnrBEntry:
            ctx->activeCommandRecord = record;
            ctx->cycleCommandPtr = record + AppendCycleEntry(ctx, record, motnSteeringActive, time);
            return 1;
        case CycleOpcode_PlayEntryStateAnimPoolUntilTime:
            ctx->activeCommandRecord = record;
            command = record + CycleManager::UpdateEntryStateAnimPoolTrigger(ctx, record, area, time);
            ctx->cycleCommandPtr = command;
            return 1;
        case CycleOpcode_InitTimedPathIfAreaMatches:
            if (ctx->areaID.name != area) {
                command = record + 0x10;
                continue;
            }
            ctx->currentOpcode = CycleOpcode_InitTimedPath;
            [[fallthrough]];
        case CycleOpcode_InitTimedPath:
            if (ctx->moveObjRuntime && CycleManager::BeginBikeMoveObjectRelease(npc)) {
                ctx->cycleCommandPtr = record;
                return 1;
            }
            if (CycleManager::InitTimedPathMotion(npc, recordOffset, motnSteeringActive != 0))
                ++cm.mobjPathStartedTotal;
            ctx->cycleCommandPtr = record + 0x10;
            return 1;
        case CycleOpcode_SetActiveAnimAndMotionVariant:
            CycleManager::ApplyActiveAnimAndMotionVariant(npc, static_cast<uint16_t>(word[1]), false);
            command = record + 8;
            if (motnSteeringActive)
                npcSetMoveFacingMot(npc, true, false);
            ctx->cycleCommandPtr = command;
            return 1;
        case CycleOpcode_SetPositionAndHeading: {
            if (ctx->moveObjRuntime && CycleManager::BeginBikeMoveObjectRelease(npc)) {
                ctx->cycleCommandPtr = record;
                return 1;
            }
            // A following animation command is taken with the move, and its animation is the one the cycle arrives on.
            command = record + 0x14;
            uint16_t animId = ctx->moveConfig->motionParamIds[0];
            const int32_t next = *reinterpret_cast<const int32_t *>(command);
            if (next == CycleOpcode_SetActiveAnimAndMotionVariant) {
                CycleManager::ApplyActiveAnimAndMotionVariant(npc, *reinterpret_cast<const uint16_t *>(command + 4), false);
                command += 8;
                animId = ctx->activeAnimId;
            } else if (next == CycleOpcode_SetAnimationState) {
                CycleManager::ApplyAnimationStateCommand(ctx, command);
                command += 16;
                animId = ctx->activeAnimId;
            }
            const Vector3 target{std::bit_cast<float>(word[1]), std::bit_cast<float>(word[2]), std::bit_cast<float>(word[3])};
            if (!npcActivateIfNearInArea(npc, 16.0f, &target, static_cast<int16_t>(word[4]), motnSteeringActive != 0, animId))
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = command;
            return 1;
        }
        case CycleOpcode_ScheduleBoundary:
            ctx->steeringWaitFrames = static_cast<uint32_t>(word[1]);
            ctx->currentOpcode = CycleOpcode_WaitHold;
            command = record + 8;
            waitForRow = true;
            break;
        case CycleOpcode_WaitHold:
            waitForRow = true;
            break;
        case CycleOpcode_RideVehiclePath2D:
            if (CycleManager::InitVehiclePath2D(npc, recordOffset, static_cast<uint32_t>(opcode)))
                ++cm.mobjPathStartedTotal;
            else
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = record + 0x18;
            return 1;
        case CycleOpcode_WaitSecondsIfAreaMatches:
            if (ctx->areaID.name != area) {
                command = record + 8;
                continue;
            }
            ctx->currentOpcode = CycleOpcode_WaitSeconds;
            [[fallthrough]];
        case CycleOpcode_WaitSeconds:
            ctx->steeringWaitFrames = static_cast<uint32_t>(time + word[1]);
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_SetAreaTag:
            CycleManager::ApplyNpcCommandState(npc, static_cast<uint32_t>(opcode), recordOffset, nullptr);
            ctx->currentOpcode = CycleOpcode_SetAreaTag;
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_SetActionMode:
            ctx->modeValue = static_cast<uint32_t>(word[1]);
            if (ctx->modeValue != 1u)
                CycleManager::StartDoorFacingHandoff(npc);
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_BeginTalkMoveToTaggedPosition:
            CycleManager::BeginTalkMoveObjectState(npc, recordOffset);
            ctx->cycleCommandPtr = record + 0x18;
            return 1;
        case CycleOpcode_WaitForTaggedCycleInRadius:
        case CycleOpcode_WaitForTaggedTalkMoveInRadius:
            CycleManager::BeginTaggedWait(npc, static_cast<uint32_t>(opcode), recordOffset, static_cast<uint32_t>(time));
            ctx->cycleCommandPtr = record + 0xC;
            return 1;
        case CycleOpcode_RequestNearestDoorTransition:
            CycleManager::StartDoorRequest(npc);
            ctx->cycleCommandPtr = record + 4;
            return 1;
        case CycleOpcode_SetScriptVisibleState:
            ctx->contextValue = word[1];
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_BeginObjectInteraction:
            CycleManager::QueueObjectInteraction(npc, recordOffset);
            ctx->cycleCommandPtr = record + 0x30;
            return 1;
        case CycleOpcode_EndObjectInteraction:
            if (CycleManager::TaggedPayload *const payload = CycleManager::FindTaggedPayload(npc, static_cast<uint32_t>(word[1])))
                payload->lifetime = 7683;
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_PayloadYield:
            ctx->cycleCommandPtr = record + 0x20;
            return 1;
        case CycleOpcode_FollowStairPath2D:
            if (CycleManager::InitStairPath2D(npc, recordOffset))
                ++cm.mobjPathStartedTotal;
            ctx->cycleCommandPtr = record + 0x14;
            return 1;
        case CycleOpcode_StartLinkedCycle:
            if (!CycleManager::InitializeMoveObjContext(npc, recordOffset, time < 300))
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = record + 4 * word[4];
            return 1;
        case CycleOpcode_PlayRootedAnim:
            if (ctx->doorAnimActive == 1)
                CycleManager::FreeIfDoorOpeningAnim(npc);
            CycleManager::PlayAnim_Wrapper(npc, recordOffset);
            if (motnSteeringActive)
                npcSetMoveFacingMot(npc, true, false);
            ctx->cycleCommandPtr = record + 0x18;
            return 1;
        case CycleOpcode_SetAnimationState:
            CycleManager::ApplyAnimationStateCommand(ctx, record);
            if (motnSteeringActive)
                npcSetMoveFacingMot(npc, true, false);
            ctx->cycleCommandPtr = record + 0x10;
            return 1;
        case CycleOpcode_DriveVehiclePath2D:
            InitBikePathRuntime(npc, record);
            ++cm.mobjPathStartedTotal;
            ctx->cycleCommandPtr = record + 0x14;
            return 1;
        case CycleOpcode_DriveForkliftPath2D:
            if (CycleManager::InitVehiclePath2D(npc, recordOffset, static_cast<uint32_t>(opcode))) {
                ++cm.mobjPathStartedTotal;
                if (ctx->moveObjRuntime)
                    (void)DoBikeStuff(npc, motnSteeringActive != 0);
            } else {
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
            }
            ctx->cycleCommandPtr = record + 0x1C;
            return 1;
        case CycleOpcode_ArmOutsideRadiusFallback:
            ctx->cycleCommandPtr = record + 0x24;
            return 1;
        case CycleOpcode_SavePointerMarker:
            ctx->branchResumeCommandPtr = command;
            ctx->cycleCommandPtr = command;
            return 1;
        case CycleOpcode_WalkPath3DIfAreaMatches:
            if (ctx->areaID.name != area) {
                command = record + 0xC;
                continue;
            }
            ctx->currentOpcode = CycleOpcode_WalkPath3D;
            [[fallthrough]];
        case CycleOpcode_WalkPath3D:
            if (ctx->moveObjRuntime && CycleManager::BeginBikeMoveObjectRelease(npc)) {
                ctx->cycleCommandPtr = record;
                return 1;
            }
            if (CycleManager::InitPathFollowMotion(npc, recordOffset, motnSteeringActive != 0))
                ++cm.mobjPathStartedTotal;
            ctx->cycleCommandPtr = record + 0xC;
            return 1;
        case CycleOpcode_SetVehicleActiveState:
            CycleManager::ApplyNpcCommandState(npc, static_cast<uint32_t>(opcode), recordOffset, nullptr);
            CycleManager::BeginVehicleActiveState(npc, true);
            ctx->cycleCommandPtr = record + 0x1C;
            return 1;
        case CycleOpcode_SetModeByte:
            ctx->proximityGroup = static_cast<uint8_t>(word[1]);
            if (ctx->proximityGroup > 1)
                ctx->proximityGroup = 0;
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_SetShopShutterState:
            if (!CycleManager::BeginShopShutterCommand(npc, record))
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = record + 0x24;
            return 1;
        case CycleOpcode_GetOtherChara:
            ctx->humansIdx = Humans_n_GetIndexForCharacter(static_cast<uint32_t>(word[1]));
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_CreateCharacter:
            CycleManager::ApplyNpcCommandState(npc, static_cast<uint32_t>(opcode), recordOffset, nullptr);
            ctx->cycleCommandPtr = record + 0x14;
            return 1;
        case CycleOpcode_SetAuxiliaryAnim:
            PlayAnim_Wrapper_0(npc, recordOffset);
            ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = record + 0x10;
            return 1;
        case CycleOpcode_Transform:
            CommitXForm_RotateTranslateZYX(npc, recordOffset);
            ctx->cycleCommandPtr = record + 0xC;
            return 1;
        case CycleOpcode_SetPositionAndHeadingImmediate:
            ctx->pos = {std::bit_cast<float>(word[1]), std::bit_cast<float>(word[2]), std::bit_cast<float>(word[3])};
            ctx->desiredPathYaw = static_cast<int16_t>(word[4]);
            ctx->currentOpcode = CycleOpcode_AdvanceScript;
            ctx->cycleCommandPtr = record + 0x14;
            return 1;
        case CycleOpcode_SetForcedMotionToken:
            ctx->forcedMotionToken = static_cast<uint32_t>(word[1]);
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_SetSteeringProfile:
            ctx->steeringProfile = word[1] != 0;
            CycleManager::RecomputeSteeringExtents(npc);
            ctx->cycleCommandPtr = record + 8;
            return 1;
        case CycleOpcode_FalseReturnMarker:
            if (!motnSteeringActive)
                continue;
            ctx->cycleCommandPtr = command;
            return 1;
        default:
            ctx->cycleCommandPtr = command;
            return 1;
        }

        // 4 and 0x12: the next timetable row once its time has passed, else the branch resume point; with neither the cycle waits.
        if (waitForRow) {
            CycleTimetableRow *const row = CM->currentScheduleEntry;
            if (row && row->timeOrEndMarker < time && row->timeOrEndMarker != -1) {
                CM->currentScheduleEntry = row + 1;
                command = CycleManager::FilePointer(static_cast<uint32_t>(row->commandStreamOffset));
                ctx->branchResumeCommandPtr = nullptr;
                ctx->currentOpcode = CycleOpcode_AdvanceScript;
                continue;
            }
            if (row && ctx->branchResumeCommandPtr) {
                command = ctx->branchResumeCommandPtr;
                continue;
            }
            ctx->cycleCommandPtr = command;
            return 1;
        }
    }
}





uint32_t MOBJ_executeTask(CycleManager::MobjEntry *CM, int32_t time, uint32_t motnSteeringActive, uint32_t skipBlend) {
    CycleManager::MobjCycle *const ctx = CM->ctx;
    const uint32_t classId = ctx->moveConfig->cycleClassCode;
    if (ctx->obj && motnSteeringActive && !ctx->moveObjRuntime && (((classId - 4u) & 0xFFFFFFFCu) != 0 || classId == 5u) &&
        (ctx->currentOpcode != CycleOpcode_StartLinkedCycle || static_cast<uint32_t>(ctx->moveObjPhase - 4u) > 2u))
        ctx->pos = GetPositionForEntity(ctx->obj->entity.node);
    if (ctx->resumePositionCapturePending)
        ctx->xmptResumePos = ctx->pos;
    const Vector3 position = ctx->pos;
    const int16_t desiredPathYaw = ctx->desiredPathYaw;
    const shen_name areaID = ctx->areaID;
    const CycleOpcode currentOpcode = ctx->currentOpcode;
    const uint32_t moveObjSubOpcode = ctx->moveObjSubOpcode;
    ctx->previousFlags = ctx->flags;
    ctx->flags = 0xFFFFFFFFu;
    ctx->resumePositionCapturePending = 0;
    const uint32_t result = MOBJ_executeTaskInternal(CM, time, motnSteeringActive, skipBlend);
    if (ctx->areaID.name != areaID.name)
        ctx->snapToPathStart = 1;
    else if (currentOpcode > 46u || ((0x610000038300ull >> currentOpcode) & 1u) == 0)
        ctx->snapToPathStart = 0;
    const MotionInformationWork *const motion = CM->ctx->mot;
    ctx->steeringTarget = position;
    ctx->steeringTargetYaw = desiredPathYaw;
    const uint16_t animId = motion ? motion->currentAnimId : ctx->activeAnimId;
    if (!skipBlend && (ctx->lastMoveObjAnimId != animId || ctx->currentOpcode != currentOpcode || ctx->moveObjSubOpcode != moveObjSubOpcode) &&
        (!motion || ctx->lastMoveObjAnimId == animId || motion->frameTimer == motion->frameStart)) {
        ctx->xmptResumePos = ctx->pos;
        if (ctx->lastMoveObjAnimId == animId)
            ctx->resumePositionCapturePending = 1;
    }
    ctx->lastMoveObjAnimId = animId;
    return result;
}

} // namespace shenmue::s1
