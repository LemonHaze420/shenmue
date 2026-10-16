#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1::CycleManager {

void ApplyCharacterTransform(Character &owner, const Vector3 &position, const Vector3i &rotation) {
    shenApplyTranslationToAll(owner.entity.node, position);
    ApplyEulerAnglesToNode(owner.entity.node, rotation);
}

// The MOTM root-motion accumulation bit the cycle manager raises for walking cycles and clears for rooted animations.
void EnableRootMotion(Character *character, bool enable) {
    if (MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr) {
        if (enable)
            manager->flags |= 1u;
        else
            manager->flags &= ~1u;
    }
}

int16_t NodeChainYaw(const Character *character) {
    int16_t yaw = 0;
    if (character) {
        for (const CharacterNode *node = &character->entity.node; node; node = node->child)
            yaw = static_cast<int16_t>(yaw + node->rotation.y);
    }
    return yaw;
}

void SetNodeChainYaw(Character &character, int16_t yaw) {
    for (const CharacterNode *child = character.entity.node.child; child; child = child->child)
        yaw = static_cast<int16_t>(yaw - child->rotation.y);
    character.entity.node.rotation.y = yaw;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


uint16_t MotionParamWord(const CycleManager::NpcRecord &npc, uint32_t wordIndex) {
    if (wordIndex >= std::size(npc.cycle->moveConfig->motionParamIds))
        return 0;
    return npc.cycle->moveConfig->motionParamIds[wordIndex];
}
} // namespace CycleManager


int32_t MOTN_getFrameCountFromAnimId(uint16_t animationId) {
    return MOTN_GetFrameCountFromAnimId(animationId);
}

float MOTN_CalculateRootMotionSpeedZ(CycleManager::NpcRecord &npc) {
    constexpr float kDefaultFrameSpeed = 0.041666668f;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    ++cm.motnSpeedLookups;
    if (!npc.cycle->activeAnimId)
        return kDefaultFrameSpeed;

    const uint16_t basePose = npc.cycle->moveConfig->anchorGroupCode <= 20u ? static_cast<uint16_t>(npc.cycle->moveConfig->anchorGroupCode) : 0xffffu;
    bool usedRootChannel = false;
    const float speed = MOTN_CalculateRootMotionSpeedZ(npc.cycle->activeAnimId, basePose, &usedRootChannel);
    ++cm.motnSpeedLookupHits;
    if (usedRootChannel)
        ++cm.motnRootChannelHits;
    else
        ++cm.motnRootChannelFallbacks;
    return speed;
}



float npcCalculateMovementDistanceStep(const CycleManager::NpcRecord &npc) {
    if (npc.cycle->moveObjRuntime || npc.cycle->moveConfig->cycleClassCode - 4u <= 3u)
        return npc.cycle->baseMoveSpeed;
    const uint16_t opcode = static_cast<uint16_t>(npc.cycle->currentOpcode);
    if ((opcode <= CycleOpcode_WalkPath3D && ((0x200800802ull >> opcode) & 1u) != 0) ||
        (opcode == CycleOpcode_StartLinkedCycle && npc.cycle->moveObjSubOpcode == CycleOpcode_WalkPath3D))
        return npc.cycle->baseMoveSpeed * npc.cycle->animMoveSpeedScale;
    return 0.0f;
}

float MOBJ_MovementDistanceStepFromMOTN(CycleManager::NpcRecord &npc, float scalarStep) {
    if (!std::isfinite(scalarStep) || scalarStep <= 0.0f)
        return 0.0f;
    if (!npc.cycle->obj || !npc.cycle->motnSteeringActive || !npc.cycle->activeAnimId)
        return scalarStep;
    MotionManagerWork *const motion = MOTM_GetWork(npc.cycle->obj);
    const uint16_t basePose = motion && motion->motion.basePoseTemplateIndex >= 0 && motion->motion.basePoseTemplateIndex <= 20 ?
        static_cast<uint16_t>(motion->motion.basePoseTemplateIndex) :
        0xffffu;
    const float frame = motion ? motion->motion.frameTimer : 0.0f;
    const float frameRef = motion ? motion->motion.frameTimerRef : -1.0f;
    const float end = frame + (motion ? motion->motion.playbackRate : 1.0f);
    Vector3 before{};
    Vector3 after{};
    if (frameRef < 0.0f || !MOTN_ReadRootPositionAtFrame(npc.cycle->activeAnimId, basePose, frameRef, before, true) ||
        !MOTN_ReadRootPositionAtFrame(npc.cycle->activeAnimId, basePose, end, after, true)) {
        return scalarStep;
    }
    const float dx = after.x - before.x;
    const float dz = after.z - before.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    return std::isfinite(distance) && distance > 0.0f && distance <= scalarStep ? distance : scalarStep;
}
namespace CycleManager {


uint16_t MotionParamDwordLow(const CycleManager::NpcRecord &npc, uint32_t dwordIndex) {
    return CycleManager::MotionParamWord(npc, dwordIndex * 2u);
}

uint16_t SelectRandomMotionToken(CycleManager::NpcRecord &npc) {
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const uint32_t seed = g_EnvPrngSeed;
    const uint32_t wordIndex = ((seed >> 16) & 3u) + 1u;
    return CycleManager::MotionParamWord(npc, wordIndex);
}

bool ApplyActiveAnimRootYaw(CycleManager::NpcRecord &npc) {
    if (!npc.cycle->obj)
        return false;

    int16_t rootYaw = npc.cycle->desiredPathYaw;
    for (CharacterNode *child = npc.cycle->obj->entity.node.child; child; child = child->child) {
        rootYaw = static_cast<int16_t>(rootYaw - static_cast<int16_t>(child->rotation.y));
    }
    npc.cycle->obj->entity.node.rotation.y = rootYaw;
    return true;
}


void ApplyActiveAnimAndMotionVariant(CycleManager::NpcRecord &npc, uint16_t activeAnim, bool facingConstraint) {
    npc.activeAnimRootYawPending = CycleManager::ApplyActiveAnimRootYaw(npc);
    npc.cycle->activeAnimId = activeAnim;
    npc.cycle->pathMoveSpeed = 1.0f;
    npc.cycle->baseMoveSpeed = 1.0f;
    npc.cycle->selectedMotionToken = CycleManager::SelectRandomMotionToken(npc);
    npc.cycle->animStateMode = 0;
    npc.cycle->facingConstraintFlag = facingConstraint;
}

uint32_t SelectObjectContextToken(uint32_t first, uint32_t second) {
    if (SCN_LooksLikeFourCcToken(second))
        return second;
    if (SCN_LooksLikeFourCcToken(first))
        return first;
    return second ? second : first;
}
} // namespace CycleManager




void npcSetupFace(CycleManager::MobjCycle *npc) {
    npc->pFace = nullptr;
    if (!npc->obj || !HLib_GetCharacterComponent(npc->obj, SHEN_NAME(MOTM)))
        return;
    npc->pFace = FACE_GetWork(npc->obj);
    if (!npc->pFace)
        return;
    PackTaskData *const pack = g_packTaskData ? g_packTaskData : g_packTask ? reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask)) : nullptr;
    pack->usePackCache = 1;
    (void)FACE_ScriptFaceMotion(npc->obj, 0);
    FACE_UpdateExpression(npc->obj, 1, 0, 2);
    pack->usePackCache = 0;
    FACE_SetLcgState(npc->obj, 0, 3, 1);
}
namespace CycleManager {




void ApplyNpcRecordToLocal(CycleManager::NpcRecord &npc) {
    Character *const local = npc.cycle->obj;
    if (!local || !npc.cycle->motnSteeringActive)
        return;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (npc.lastLocalAnimAdvanceEpoch == cm.npcAnimSyncEpoch) {
        ++cm.mobjNpcAnimDuplicateSyncs;
    } else {
        npc.lastLocalAnimAdvanceEpoch = cm.npcAnimSyncEpoch;
        ++cm.mobjNpcAnimAdvances;
    }
    Vector3 position = GetPositionForEntity(local->entity.node);
    position.y = MapProbe_UpdateIntersectionStateAtPoint({position.x, position.y + 100.0f, position.z});
    shenApplyTranslationToAll(local->entity.node, position);
    if (npc.activeAnimRootYawPending)
        (void)CycleManager::ApplyActiveAnimRootYaw(npc);
    npc.activeAnimRootYawPending = false;
}
} // namespace CycleManager




void npcSetMoveFacingMot(CycleManager::NpcRecord &npc, bool enableRootMotion, bool facingConstraint) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion)
        return;
    if (npc.cycle->obj) {
        if (MotionManagerWork *const manager = MOTM_GetWork(npc.cycle->obj)) {
            if (enableRootMotion)
                manager->flags |= 1u;
            else
                manager->flags &= ~1u;
        }
    }
    motion->playbackRate = npc.cycle->baseMoveSpeed;
    if (motion->currentAnimId == npc.cycle->activeAnimId)
        return;
    motion->activeBasePoseAnimIndex = 7;
    if (npc.cycle->obj)
        freeTalkConv_SetMoveAndFacingConstraint(*npc.cycle->obj, npc.cycle->pos, npc.cycle->desiredPathYaw, npc.cycle->activeAnimId, 0.0f,
            npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, facingConstraint);
}


void cyclSubmitMoveRequest(CycleManager::NpcRecord &npc) {
    if (!npc.cycle->obj)
        return;
    const float deltaX = npc.cycle->currentWaypointPos.x - npc.cycle->pos.x;
    const float deltaZ = npc.cycle->currentWaypointPos.z - npc.cycle->pos.z;
    Vector3 target{npc.cycle->currentWaypointPos.x, npc.cycle->pos.y, npc.cycle->currentWaypointPos.z};
    if (!(0.040000003f > deltaZ * deltaZ + deltaX * deltaX)) {
        const uint16_t yaw = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
        target.x = npc.cycle->pos.x - sin_fixed16(static_cast<int16_t>(yaw)) * 0.2f;
        target.z = npc.cycle->pos.z - cos_fixed16(yaw) * 0.2f;
    }
    std::uint32_t locomotion[24]{npc.cycle->activeAnimId};
    charaSubmitMoveRequest(npc.cycle->obj, &target, npc.cycle->desiredPathYaw, 0, locomotion, 1.2f, 3);
}


void cycleRequestAnim(CycleManager::NpcRecord &npc, uint16_t animId) {
    if (npc.cycle->obj)
        charaSubmitMoveRequest(npc.cycle->obj, &npc.cycle->pos, npc.cycle->desiredPathYaw, animId, nullptr, 1.2f, 6);
}

void cyclFreeTalkConvSetMoveAndFacingConstraint(CycleManager::NpcRecord &npc, uint16_t animId) {
    if (npc.cycle->obj)
        freeTalkConv_SetMoveAndFacingConstraint(*npc.cycle->obj, npc.cycle->pos, npc.cycle->desiredPathYaw, animId, 0.0f, 1.0f, 0.0f, false);
}



void cyclUpdateCharaMoveRequestForAnimChange(CycleManager::NpcRecord &npc) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!npc.cycle->obj || !motion || charaHasBlockingTPMRequest(npc.cycle->obj))
        return;
    if (MotionManagerWork *const manager = MOTM_GetWork(npc.cycle->obj))
        manager->flags |= 1u;
    Vector3 position = GetPositionForEntity(npc.cycle->obj->entity.node);
    motion->playbackRate = npc.cycle->baseMoveSpeed;
    const uint16_t animId = npc.cycle->activeAnimId;
    if (motion->currentAnimId == animId)
        return;
    motion->activeBasePoseAnimIndex = 7;
    if (animId == npc.cycle->moveConfig->motionParamIds[8] || animId == npc.cycle->moveConfig->motionParamIds[11]) {
        const float deltaX = npc.cycle->moveTargetPos.x - position.x;
        const float deltaZ = npc.cycle->moveTargetPos.z - position.z;
        if (0.040000003f > deltaZ * deltaZ + deltaX * deltaX) {
            position.x = npc.cycle->moveTargetPos.x;
            position.z = npc.cycle->moveTargetPos.z;
        } else {
            const uint16_t yaw = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
            position.x = position.x - sin_fixed16(static_cast<int16_t>(yaw)) * 0.2f;
            position.z = position.z - cos_fixed16(yaw) * 0.2f;
        }
        std::uint32_t locomotion[24]{animId};
        charaSubmitMoveRequest(npc.cycle->obj, &position, npc.cycle->desiredPathYaw, 0, locomotion, 1.2f, 3);
    } else if (animId == npc.cycle->moveConfig->motionParamIds[0]) {
        charaSubmitMoveRequest(npc.cycle->obj, &position, npc.cycle->desiredPathYaw, animId, nullptr, 1.2f, 6);
    } else {
        freeTalkConv_SetMoveAndFacingConstraint(*npc.cycle->obj, position, npc.cycle->desiredPathYaw, animId, 0.0f,
            npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, npc.cycle->facingConstraintFlag);
    }
}



void UpdateEntityOverPositionWithFlags(CycleManager::NpcRecord &npc, bool restoreIdle) {
    if (npc.cycle->moveConfig->cycleClassCode < 4u && !npc.cycle->moveObjRuntime)
        npc.cycle->steeringHalfDepth = npc.cycle->savedSteeringHalfDepth;
    if (const uint8_t pending = npc.cycle->xmptResumePending) {
        if (((pending - 3u) & 0xfdu) == 0 && npc.cycle->obj) {
            npc.cycle->desiredPathYaw = CycleManager::NodeChainYaw(npc.cycle->obj);
            npc.cycle->pos = GetPositionForEntity(npc.cycle->obj->entity.node);
            charaCancelMoveRequest(npc.cycle->obj, restoreIdle ? 1 : 0);
        }
        npc.cycle->xmptResumePending = 0;
        npc.cycle->workFloat4C = 0.0f;
    }
    if (restoreIdle)
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
}


void charaClearMotionRequests(Character *character) {
    if (!character)
        return;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(XMPT))) {
        auto *const request = reinterpret_cast<CharacterMoveRequestWork *>(component + 1);
        if (request->state)
            request->state = 0;
    }
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(WMPT))) {
        auto *const point = reinterpret_cast<MotionPointWork *>(component + 1);
        if (point->state)
            point->state = 0;
    }
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(YMPT))) {
        auto *const sequence = reinterpret_cast<MotionSequenceWork *>(component + 1);
        if (sequence->state)
            sequence->state = 0;
    }
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(LKPT))) {
        auto *const look = reinterpret_cast<LookPointWork *>(component + 1);
        if (look->active) {
            look->active = 0;
            look->moveRequestLook = 0;
        }
    }
}
namespace CycleManager {




void SubmitXMPTMoveWithCycleParams(CycleManager::NpcRecord &npc, Character &character, const Vector3 &target, int16_t yaw, uint16_t animId) {
    npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, npc.cycle->pos.y + 100.0f, npc.cycle->pos.z});
    shenApplyTranslationToAll(character.entity.node, npc.cycle->pos);
    const auto &ids = npc.cycle->moveConfig->motionParamIds;
    const std::uint32_t locomotion[24]{ids[8], ids[17], ids[18], ids[19], ids[20], 0, ids[13], ids[14], ids[15], ids[16], 8, 17, 26, 35, 14, 19, 16, ids[0],
        ids[0], ids[0], ids[0], ids[0], ids[0]};
    const float rate =
        std::fabs(npc.cycle->moveConfig->motionRadiusTimingScalar) < FLT_EPSILON ? 0.041666668f : npc.cycle->moveConfig->motionRadiusTimingScalar;
    charaSubmitMoveRequest(&character, &target, yaw, animId, locomotion, rate, 4);
}
} // namespace CycleManager



bool PlayAnim_Wrapper(Character *character, MotionInformationWork *motion, float startFrame, std::uint16_t animId, std::uint16_t nodeId, std::uint32_t flags) {
    if (!motion)
        return false;
    if (motion->currentAnimId) {
        (void)FIGP_PlayFightAnim_Wrapper(character, animId, static_cast<std::int16_t>(nodeId), startFrame, flags);
        return true;
    }
    MOTN_ClearSecondaryMotion(*motion);
    motion->secondaryNodeId = 0;
    motion->queuedSecondaryFlags = flags;
    motion->queuedSecondaryBlendFrames = motion->secondaryBlendFrames;
    motion->queuedSecondaryStartFrame = startFrame;
    motion->queuedSecondaryAnimId = animId;
    motion->queuedSecondaryNodeId = static_cast<std::int16_t>(nodeId);
    motion->queuedSecondaryEndFrame = -1.0f;
    return false;
}
namespace CycleManager {



void PlayAnim_Wrapper(CycleManager::NpcRecord &npc, std::uint32_t recordOffset) {
    if (npc.cycle->obj)
        CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
    MotionInformationWork *const motion = npc.cycle->mot;
    if (motion && npc.cycle->currentAnimToken) {
        if (!motion->motionControlLock && !motion->secondaryBlendCountdown) {
            motion->secondaryBlendFramesPrevious = motion->secondaryBlendFrames;
            motion->secondaryBlendCountdown =
                static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(motion->secondaryBlendFrames) * motion->secondaryBlendFrameScale));
        }
        npc.cycle->currentAnimToken = 0;
    }
    npc.cycle->currentAnimToken = CycleManager::CommandWord(recordOffset + 4);
    npc.cycle->pathMoveSpeed = 1.0f;
    npc.cycle->baseMoveSpeed = 1.0f;
    npc.cycle->activeCommandRecord = CycleManager::FilePointer(recordOffset);
    npc.cycle->field_130 = CycleManager::FilePointer(recordOffset);
    npc.cycle->selectedMotionToken = CycleManager::SelectRandomMotionToken(npc);
    npc.cycle->animStateMode = 0;
    npc.cycle->animHandleActive = false;
    if (motion && npc.cycle->currentAnimToken) {
        const std::uint16_t nodeId = static_cast<std::uint16_t>(CycleManager::CommandWord(recordOffset + 8));
        (void)::shenmue::s1::PlayAnim_Wrapper(
            npc.cycle->obj, motion, 0.0f, static_cast<std::uint16_t>(npc.cycle->currentAnimToken), nodeId, CycleManager::CommandWord(recordOffset + 12));
        if (CycleManager::CommandWord(recordOffset + 16))
            motion->secondaryLookupAnimId = 0xffffu;
    }
}


void FreeIfDoorOpeningAnim(CycleManager::NpcRecord &npc) {
    if (MotionInformationWork *const motion = npc.cycle->mot) {
        if (motion->secondaryAnimId == CycleManager::UmbrellaAnimationId())
            CycleManager::FreeSecondaryMotion(*motion);
        if (motion->mobjCallback == reinterpret_cast<void *>(cyclUmbrellaHandCallback))
            motion->mobjCallback = nullptr;
        if (motion->mobjCallbackUser == &npc)
            motion->mobjCallbackUser = nullptr;
    }
    npc.cycle->doorAnimActive = 0;
    CycleManager::RecomputeSteeringExtents(npc);
}
} // namespace CycleManager



bool npcApplyUmbrellaAnim(bool umbrella, CycleManager::NpcRecord &npc) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (npc.cycle->moveConfig->cycleClassCode < 2u && motion) {
        const std::uint16_t animId = umbrella ? CycleManager::UmbrellaAnimationId() : static_cast<std::uint16_t>(npc.cycle->currentAnimToken);
        CycleManager::PlayOverlayAnim(npc, animId, umbrella ? 14u : 3u, umbrella ? 1u : 0u);
        motion->mobjCallback = reinterpret_cast<void *>(cyclUmbrellaHandCallback);
        motion->mobjCallbackUser = &npc;
        npc.cycle->doorAnimActive = 1;
        const std::int32_t rainModel = npc.cycle->humansIdx % 2 + (npc.cycle->moveConfig->cycleClassCode == 1u ? 2 : 0);
        SetupEngineObject(reinterpret_cast<std::uintptr_t>(g_rainStateModels[static_cast<std::size_t>(rainModel)]), reinterpret_cast<std::uintptr_t>(&npc), 16);
    }
    return true;
}
namespace CycleManager {



void ApplyUmbrellaWalk(CycleManager::NpcRecord &npc) {
    MotionInformationWork *const motion = npc.cycle->mot;
    bool umbrella = true;
    if (npc.cycle->currentAnimToken) {
        if (!npc.cycle->animHandleActive)
            return;
        if (motion)
            CycleManager::FreeSecondaryMotion(*motion);
        umbrella = false;
    } else {
        if (!motion)
            return;
        if (motion->secondaryAnimId)
            CycleManager::FreeSecondaryMotion(*motion);
    }
    if (npcApplyUmbrellaAnim(umbrella, npc))
        CycleManager::RecomputeSteeringExtents(npc);
}
} // namespace CycleManager




void PlayAnim_Wrapper_1(CycleManager::NpcRecord &npc, bool raining) {
    AttachEntityAndInitCycle(npc.cycle->obj, false);
    npc.cycle->motnSteeringActive = true;
    if (npc.cycle->obj) {
        shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
        CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
    }
    if (MotionInformationWork *const motion = npc.cycle->mot) {
        charaClearMotionRequests(npc.cycle->obj);
        UNFO_InitWrapper(*motion);
        motion->motionFlags &= ~0x100u;
        motion->headLookAngleCommand = 0x8068u;
        motion->activeBasePoseAnimIndex = 5;
        motion->poseEvaluationStage = 0;
        motion->motionFlags &= ~2u;
        motion->pendingAnimId = npc.cycle->activeAnimId;
        motion->motionFlags |= 0x10000000u;
        OSAGE_Initialize(npc.cycle->obj);
    }
    const std::uint32_t recordOpcode =
        CycleManager::FileOffset(npc.cycle->field_130) ? CycleManager::CommandWord(CycleManager::FileOffset(npc.cycle->field_130)) : 0;
    if (npc.cycle->currentAnimToken && CycleManager::FileOffset(npc.cycle->field_130)) {
        if (recordOpcode == CycleOpcode_PlayRootedAnim) {
            CycleManager::PlayAnim_Wrapper(npc, CycleManager::FileOffset(npc.cycle->field_130));
        } else if (npc.cycle->mot) {
            CycleManager::PlayOverlayAnim(npc, static_cast<std::uint16_t>(npc.cycle->currentAnimToken), npc.cycle->doorAnimActive ? 3u : 7u, 0);
            npc.cycle->animHandleActive = true;
        }
    }
    if (!npc.cycle->mot)
        return;
    if (raining) {
        if (npc.cycle->modeValue == 1u)
            CycleManager::ApplyUmbrellaWalk(npc);
        else
            CycleManager::FreeIfDoorOpeningAnim(npc);
        return;
    }
    CycleManager::FreeIfDoorOpeningAnim(npc);
    if (npc.cycle->currentAnimToken && recordOpcode == CycleOpcode_SetAuxiliaryAnim) {
        CycleManager::PlayOverlayAnim(npc, static_cast<std::uint16_t>(npc.cycle->currentAnimToken), npc.cycle->doorAnimActive ? 3u : 7u, 0);
        npc.cycle->animHandleActive = true;
    }
}
namespace CycleManager {




void StartDoorFacingHandoff(CycleManager::NpcRecord &npc) {
    if (npc.cycle->doorAnimActive != 1u)
        return;
    if (!npc.cycle->mot || !npc.cycle->obj) {
        CycleManager::FreeIfDoorOpeningAnim(npc);
        return;
    }
    npc.cycle->savedOpcodeBeforeMoveObj = npc.cycle->currentOpcode;
    npc.cycle->currentOpcode = CycleOpcode_AuxiliaryAnimationTransition;
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
    freeTalkConv_SetMoveAndFacingConstraint(*npc.cycle->obj, npc.cycle->pos, npc.cycle->desiredPathYaw, npc.cycle->activeAnimId, 0.0f, 1.0f, 0.0f, false);
}
} // namespace CycleManager




void motAssignCallbackReactions(CycleManager::NpcRecord &npc) {
    if (npc.cycle->doorAnimActive != 1u || npc.cycle->moveConfig->cycleClassCode >= 2u)
        return;
    if (RenderArrayEntry *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(&npc)))) {
        if (entry->retire != 2U)
            entry->retire = 1;
        g_hmdlEntryDirty = 1;
    }
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion) {
        npc.cycle->doorAnimActive = 0;
        return;
    }
    const std::uint8_t blendFrames = motion->secondaryBlendFrames;
    motion->secondaryBlendFrames = 30;
    CycleManager::BeginSecondaryBlendOut(*motion);
    motion->secondaryBlendFrames = blendFrames;
    if (motion->mobjCallback != reinterpret_cast<void *>(cyclUmbrellaHandCallback)) {
        npc.cycle->doorAnimActive = 0;
        return;
    }
    npc.cycle->doorAnimActive = 2;
    motion->mobjCallback = reinterpret_cast<void *>(cyclUmbrellaReleaseCallback);
    if (npc.cycle->currentAnimToken && npc.cycle->obj)
        mtwkGetCharaBonePosition(npc.cycle->obj, 11, &npc.umbrellaReleaseAnklePos);
}



void PlayAnim_Wrapper_2(CycleManager::NpcRecord &npc, bool steeringActive) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion || !steeringActive) {
        npc.cycle->doorAnimActive = 0;
        npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
        CycleManager::RecomputeSteeringExtents(npc);
        return;
    }
    if (npc.cycle->doorAnimActive != 1u) {
        if (npc.cycle->doorAnimActive)
            return;
        npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
        if (npc.cycle->animHandleActive) {
            CycleManager::PlayOverlayAnim(npc, static_cast<std::uint16_t>(npc.cycle->currentAnimToken), 7u, 0);
            motion->playbackRate = 1.0f;
        }
        CycleManager::RecomputeSteeringExtents(npc);
        return;
    }
    if ((motion->motionFlags & 0x10000u) == 0) {
        if (npc.cycle->animHandleActive)
            motion->playbackRate = 0.0f;
        motAssignCallbackReactions(npc);
    }
}


void PlayAnim_Wrapper_0(CycleManager::NpcRecord &npc, std::uint32_t recordOffset) {
    const std::uint32_t animId = CycleManager::CommandWord(recordOffset + 4);
    MotionInformationWork *const motion = npc.cycle->mot;
    if (animId) {
        if (npc.cycle->currentAnimToken == animId)
            return;
        npc.cycle->currentAnimToken = animId;
        npc.cycle->field_130 = CycleManager::FilePointer(recordOffset);
        if (!npc.cycle->doorAnimActive) {
            if (motion) {
                (void)::shenmue::s1::PlayAnim_Wrapper(npc.cycle->obj, motion, 0.0f, static_cast<std::uint16_t>(animId), 7u, 0);
                npc.cycle->animHandleActive = true;
                return;
            }
        } else {
            CycleManager::FreeIfDoorOpeningAnim(npc);
            (void)npcApplyUmbrellaAnim(false, npc);
        }
        npc.cycle->animHandleActive = true;
        return;
    }
    if (!npc.cycle->currentAnimToken || !npc.cycle->animHandleActive)
        return;
    npc.cycle->currentAnimToken = 0;
    if (!npc.cycle->doorAnimActive) {
        if (motion) {
            CycleManager::BeginSecondaryBlendOut(*motion);
            npc.cycle->animHandleActive = false;
            return;
        }
    } else {
        if (motion)
            CycleManager::FreeSecondaryMotion(*motion);
        CycleManager::ApplyUmbrellaWalk(npc);
    }
    npc.cycle->animHandleActive = false;
}
namespace CycleManager {


void UpdateMOTNSplines() {
    (void)QueryFrameTime();
    for (CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet); npc;
         npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        if (npc->cycle->motnSteeringActive)
            (void)UpdateNpcSteeringFromMOTN(*npc, true);
    }
    (void)QueryFrameTime();
}
} // namespace CycleManager


} // namespace shenmue::s1
