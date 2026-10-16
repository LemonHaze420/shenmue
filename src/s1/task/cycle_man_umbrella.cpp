#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1::CycleManager {



void Weather_SetRainModelsFromHumans() {
    constexpr uint64_t size = 164864u;
    const int32_t humansIdx = Humans_n_GetIndexForCharacter(SHEN_NAME(RAIN));
    if (CycleManager::LargestFreeTaskHeapBlock() > size && static_cast<uint32_t>(HLib_GetLargestMallocFreeBlock()) > size)
        gRainModelPtr = PACK_LoadHumanByIndex(HUMANS_AFS_Ptr, static_cast<uint32_t>(2 * humansIdx), static_cast<uint32_t>(2 * humansIdx + 1));
}


int32_t Weather_GetRainState() {
    static const std::array<uint32_t, 4> kRainStateTokens{SHEN_NAME(RAIN), SHEN_NAME(RAI2), SHEN_NAME(RAI1), SHEN_NAME(RAI3)};
    for (size_t i = 0; i < kRainStateTokens.size(); ++i) {
        Character *const character = CycleManager::FindCharacter(kRainStateTokens[i]);
        if (!character)
            return 0;
        g_rainStateModels[i] = character->model;
        if (!character->model)
            return 0;
    }
    return 1;
}

bool Weather_ProcessRain() {
    if (Weather_GetCurrentWeatherEffectMode() != 2 || !g_activeRainEntry)
        return false;

    const float intensity = g_activeRainEntry->intensity;
    if (std::fabs(intensity) < std::numeric_limits<float>::epsilon()) {
        g_activeRainEntry->intensity = 0.0f;
        return false;
    }
    if (intensity < 0.15000001f)
        return false;
    if (nRainState)
        return true;

    if (!gRainModelPtr) {
        if (!PACK_HasValidPack())
            Weather_SetRainModelsFromHumans();
        return false;
    }

    if (!PACK_IsValidSlot(gRainModelPtr))
        return false;
    if (CycleManager::FindCharacter(SHEN_NAME(RAIN)))
        nRainState = Weather_GetRainState();
    return false;
}


void FreeSecondaryMotion(MotionInformationWork &motion) {
    MOTN_ClearSecondaryMotion(motion);
    motion.secondaryNodeId = 0;
}


void BeginSecondaryBlendOut(MotionInformationWork &motion) {
    if (motion.motionControlLock || motion.secondaryBlendCountdown)
        return;
    motion.secondaryBlendFramesPrevious = motion.secondaryBlendFrames;
    const float countdown = static_cast<float>(motion.secondaryBlendFrames) * motion.secondaryBlendFrameScale;
    motion.secondaryBlendCountdown = static_cast<std::uint8_t>(static_cast<std::int32_t>(countdown));
}

// PlayAnim_Wrapper with the secondary blend forced to zero for the start, as the cycle manager's callers do.
void PlayOverlayAnim(CycleManager::NpcRecord &npc, std::uint16_t animId, std::uint16_t nodeId, std::uint32_t flags) {
    MotionInformationWork *const motion = npc.cycle->mot;
    const std::uint8_t blendFrames = motion->secondaryBlendFrames;
    motion->secondaryBlendFrames = 0;
    const float savedFrames = static_cast<float>(blendFrames);
    (void)::shenmue::s1::PlayAnim_Wrapper(npc.cycle->obj, motion, 0.0f, animId, nodeId, flags);
    npc.cycle->mot->secondaryBlendFrames = static_cast<std::uint8_t>(static_cast<std::int32_t>(savedFrames));
}


void UNFO_InitWrapper(MotionInformationWork &motion) {
    void *const activeNodes = motion.activeNodes;
    void *const blendTargetNodes = motion.blendTargetNodes;
    void *const nodeMatrixPool = motion.nodeMatrixPool;
    const std::int64_t activeNodesCookie = motion.activeNodesCookie;
    const std::int64_t blendTargetNodesCookie = motion.blendTargetNodesCookie;
    if (motion.transitionNodes) {
        g_motnRowBytesInUse -= 0x26bc;
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(motion.transitionNodes), 0);
    }
    motion.transitionNodes = nullptr;
    motion.transitionNodesCookie = 0;
    if (motion.secondaryNodes) {
        g_motnRowBytesInUse -= 0x26bc;
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(motion.secondaryNodes), 0);
    }
    motion.secondaryNodes = nullptr;
    motion.secondaryNodesCookie = 0;
    UNFO_Init(motion);
    motion.blendTargetNodes = blendTargetNodes;
    motion.nodeMatrixPool = nodeMatrixPool;
    motion.activeNodes = activeNodes;
    motion.activeNodesCookie = activeNodesCookie;
    motion.blendTargetNodesCookie = blendTargetNodesCookie;
    static_cast<MotionNodeRuntime *>(motion.activeNodes)->nodeId = 0xff;
    static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->nodeId = 0xff;
}



bool MOTI_GetBlendedEffectorPosition(const MotionInformationWork &motion, std::int32_t nodeId, Vector3 &position) {
    const MotionNodeRuntime *const active = MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(motion.activeNodes), nodeId);
    if (!motion.interpolationMode) {
        if (!active)
            return false;
        position = {active->effectorPosition.x, active->effectorPosition.y, active->effectorPosition.z};
        return true;
    }
    void *const blendRows = motion.interpolationMode == 4U ? motion.transitionNodes : motion.blendTargetNodes;
    const MotionNodeRuntime *const blend = MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(blendRows), nodeId);
    if (!active || !blend)
        return false;
    const float amount = blend->interpolationAmount;
    const double inverse = 1.0 - static_cast<double>(amount);
    position.x = static_cast<float>(static_cast<double>(blend->effectorPosition.x) * inverse + static_cast<double>(amount * active->effectorPosition.x));
    position.y = static_cast<float>(static_cast<double>(blend->effectorPosition.y) * inverse + static_cast<double>(amount * active->effectorPosition.y));
    position.z = static_cast<float>(static_cast<double>(blend->effectorPosition.z) * inverse + static_cast<double>(amount * active->effectorPosition.z));
    return true;
}


void cyclCopySecondaryNodeEuler(const MotionInformationWork &motion, std::int32_t nodeId) {
    const MotionNodeRuntime *const source = MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(motion.secondaryNodes), nodeId);
    MotionNodeRuntime *const target = MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(motion.activeNodes), nodeId);
    if (source && target)
        target->bindEuler = source->bindEuler;
}



void cyclUmbrellaHandCallback(MotionInformationWork *motion, void *user) {
    if (!motion)
        return;
    CycleManager::NpcRecord *const npc = static_cast<CycleManager::NpcRecord *>(user);
    Character *const character = npc->cycle->obj;
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionNodeRuntime *const previousRows = manager ? static_cast<MotionNodeRuntime *>(manager->motion.previousNodes) : nullptr;
    const MotionNodeRuntime *const hand = MOTN_FindNodeRow(previousRows, 17);
    const Vector3 handPos =
        hand ? Vector3{hand->transform->rows[3].x, hand->transform->rows[3].y, hand->transform->rows[3].z} : GetPositionForEntity(character->entity.node);
    const MotionNodeRuntime *const root = MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(motion->secondaryNodes), 0);
    if (!root)
        return;
    float x = handPos.x - root->effectorPosition.x;
    float y = handPos.y - root->effectorPosition.y;
    const float z = handPos.z - root->effectorPosition.z;
    const std::uint16_t halfTurn = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(motion->currentTurnAngle) * 0.5f));
    float depth = cos_fixed16(halfTurn) * z - sin_fixed16(static_cast<int16_t>(halfTurn)) * x;
    Vector3 pivot{};
    Vector3 reach{};
    if (npc->cycle->animHandleActive) {
        x += 0.05f;
        if (!MOTI_GetBlendedEffectorPosition(*motion, 0, pivot) || !MOTI_GetBlendedEffectorPosition(*motion, 3, reach))
            return;
    } else {
        if (const MotionNodeRuntime *const row = MOTN_FindNodeRow(previousRows, 0))
            pivot = WorldToEntityLocalPoint(character->entity.node, {row->transform->rows[3].x, row->transform->rows[3].y, row->transform->rows[3].z});
        if (const MotionNodeRuntime *const row = MOTN_FindNodeRow(previousRows, 4))
            reach = WorldToEntityLocalPoint(character->entity.node, {row->transform->rows[3].x, row->transform->rows[3].y, row->transform->rows[3].z});
    }
    const float dx = reach.x - pivot.x;
    const float dy = reach.y - pivot.y;
    const float dz = reach.z - pivot.z;
    if (std::fabs(dx) >= 0.001 || std::fabs(dy) >= 0.001 || std::fabs(dz) >= 0.001) {
        const float dx2 = dx * dx;
        const float dz2 = dz * dz;
        depth -= std::sqrt(dx2 + dz2);
        const float lift = std::sqrt(dy * dy + dx2 + dz2) - dy;
        x += lift * 0.2f;
        y -= lift * 0.8f;
    }
    Vector3 origin{};
    if (!MOTI_GetBlendedEffectorPosition(*motion, 0, origin))
        return;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    const std::uint16_t spineYaw = static_cast<std::uint16_t>(motion->spineYaw);
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<int16_t>(spineYaw)), cos_fixed16(spineYaw));
    const Matrix4x4 &work = workTransformMatrix;
    const Vector3 target{work.rows[1].x * y + work.rows[0].x * x + work.rows[2].x * depth + work.rows[3].x,
        work.rows[0].y * x + work.rows[1].y * y + work.rows[2].y * depth + work.rows[3].y,
        work.rows[1].z * y + work.rows[0].z * x + work.rows[2].z * depth + work.rows[3].z};
    Matrix4x4::Pop();
    const std::int32_t slot = getMOTNType(17);
    if (manager && slot != -1)
        AssignMotionWorldTarget(manager->motion, static_cast<std::size_t>(slot), target);
}




void cyclUmbrellaReleaseCallback(MotionInformationWork *motion, void *user) {
    if (!motion)
        return;
    CycleManager::NpcRecord *const npc = static_cast<CycleManager::NpcRecord *>(user);
    MotionInformationWork *const npcMotion = npc ? npc->cycle->mot : nullptr;
    if (!npcMotion)
        return;
    if (npc->cycle->animHandleActive) {
        const Vector3 ankle = npc->umbrellaReleaseAnklePos;
        MotionManagerWork *const manager = MOTM_GetWork(npc->cycle->obj);
        const std::int32_t slot = getMOTNType(11);
        if (manager && slot != -1)
            AssignMotionWorldTarget(manager->motion, static_cast<std::size_t>(slot), ankle);
        cyclCopySecondaryNodeEuler(*npcMotion, 7);
        cyclCopySecondaryNodeEuler(*npcMotion, 8);
        cyclCopySecondaryNodeEuler(*npcMotion, 12);
    }
    if (npcMotion->secondaryBlendCountdown < 2U) {
        npc->cycle->doorAnimActive = 0;
        if (npcMotion->mobjCallback == reinterpret_cast<void *>(cyclUmbrellaReleaseCallback)) {
            npcMotion->mobjCallback = nullptr;
            npcMotion->mobjCallbackUser = nullptr;
        }
    }
}




void cyclApplyUmbrellaTransform(const CycleManager::NpcRecord &npc) {
    const float widthScale = static_cast<std::int32_t>(npc.cycle->moveConfig->configWord2C) >= 15 ? 0.95f : 0.85f;
    const MotionInformationWork &motion = *npc.cycle->mot;
    Vector3 spanStart{};
    Vector3 spanEnd{};
    Vector3 grip{};
    mtwkGetCharaBonePosition(npc.cycle->obj, 16, &spanStart);
    mtwkGetCharaBonePosition(npc.cycle->obj, 17, &spanEnd);
    const float dx = spanStart.x - spanEnd.x;
    const float dy = spanStart.y - spanEnd.y;
    const float dz = spanStart.z - spanEnd.z;
    const float span = std::sqrt((dy * dy + dx * dx) + dz * dz);
    mtwkGetCharaBonePosition(npc.cycle->obj, 18, &grip);
    Matrix4x4::preTranslate(grip.x, grip.y, grip.z);
    const std::uint16_t spineYaw = static_cast<std::uint16_t>(motion.spineYaw);
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<int16_t>(spineYaw)), cos_fixed16(spineYaw));
    if (npc.cycle->doorAnimActive == 2) {
        const std::int32_t release = static_cast<std::int32_t>(static_cast<float>(motion.secondaryBlendCountdown) / -30.0f * 16384.0f);
        const std::uint16_t tip = static_cast<std::uint16_t>(-16384 - release);
        Matrix4x4::RotateAroundX(sin_fixed16(static_cast<int16_t>(tip)), cos_fixed16(tip));
    }
    if (npc.cycle->animHandleActive)
        Matrix4x4::preTranslate(span * -0.15f, 0.0f, span * -0.4f);
    else
        Matrix4x4::preTranslate(span * -0.2f, 0.0f, span * -0.3f);
    if (npc.cycle->moveConfig->obj_id.name == SHEN_NAME(RKKI) || npc.cycle->moveConfig->obj_id.name == SHEN_NAME(RINS)) {
        float lift = 1.0f;
        if (motion.currentTurnAngle <= 3000)
            lift = motion.currentTurnAngle >= 0 ? static_cast<float>(motion.currentTurnAngle) / 3000.0f : 0.0f;
        Matrix4x4::RotateAroundZ(sin_fixed16(static_cast<int16_t>(910)), cos_fixed16(910));
        const std::uint16_t tilt = static_cast<std::uint16_t>(static_cast<std::int32_t>((35.0f - lift * 15.0f) * 8192.0f / 45.0f));
        Matrix4x4::RotateAroundX(sin_fixed16(static_cast<int16_t>(tilt)), cos_fixed16(tilt));
    } else {
        Matrix4x4::RotateAroundZ(sin_fixed16(static_cast<int16_t>(910)), cos_fixed16(910));
        Matrix4x4::RotateAroundX(sin_fixed16(static_cast<int16_t>(3640)), cos_fixed16(3640));
    }
    Matrix4x4::preTranslate(0.0f, 0.05f, 0.0f);
    Matrix4x4::Scale(widthScale, 1.0f, widthScale);
}




void cyclDrawUmbrella(CycleManager::NpcRecord &npc) {
    if (npc.cycle->moveConfig->cycleClassCode >= 2u)
        return;
    const MotionInformationWork &motion = *npc.cycle->mot;
    float alpha = npc.cycle->obj->renderOpacity;
    if (npc.cycle->doorAnimActive == 2) {
        const float countdown = static_cast<float>(motion.secondaryBlendCountdown);
        if (24.0f > countdown) {
            alpha *= (countdown - 9.0f) / 15.0f;
            if (0.01 > static_cast<double>(alpha))
                alpha = 0.0f;
        }
    }
    Matrix4x4::SetActiveTransformToPrePass();
    cyclApplyUmbrellaTransform(npc);
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(&npc, nullptr, 0, 0));
    const bool cached = CachedAttachmentOwnsSubmission(&npc, alpha, workTransformMatrix);
    Matrix4x4::Pop(1);
    if (cached)
        return;
    Matrix4x4_Push();
    cyclApplyUmbrellaTransform(npc);
    const std::int32_t rainModel = npc.cycle->humansIdx % 2 + (npc.cycle->moveConfig->cycleClassCode == 1u ? 2 : 0);
    d3t::MT5_SubmitObjectNode(g_rainStateModels[static_cast<std::size_t>(rainModel)], alpha <= 0.99f ? alpha : 1.0f);
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




void DrawUmbrellas() {
    for (CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet); npc;
         npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        if (!npc->cycle->doorAnimActive)
            continue;
        const MotionInformationWork *const motion = npc->cycle->mot;
        if (!motion || !npc->cycle->obj) {
            npc->cycle->doorAnimActive = 0;
            continue;
        }
        if (!motion->updateMode)
            cyclDrawUmbrella(*npc);
        MHND_ApplyCycleUmbrellaPose(npc->cycle->obj);
    }
}
} // namespace CycleManager


} // namespace shenmue::s1
