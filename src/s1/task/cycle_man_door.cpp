#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
namespace CycleManager {


CycleManager::ShutRow *FindSHUTRowByToken(uint32_t token) {
    if (!CycleManager::IsReady() || !token)
        return nullptr;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::ShutRow &row : cm.shutRows) {
        if (row.token.name == token)
            return &row;
    }
    return nullptr;
}

int16_t SubtractChildYawChain(int16_t yaw, const CharacterNode *child) {
    int32_t localYaw = yaw;
    uint32_t guard = 0;
    for (const CharacterNode *node = child; node && guard++ < 32; node = node->child)
        localYaw -= node->rotation.y;
    return static_cast<int16_t>(localYaw & 0xFFFF);
}


void AppendDynamicTurnVolume(const Vector3 &center, int16_t yaw, float halfWidth, float halfDepth) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (g_dynamicTurnCollisionVolumeCount >= cm.dynamicTurnCollisionVolumes.size())
        return;

    CycleManager::IntersectionEntry &entry = cm.dynamicTurnCollisionVolumes[g_dynamicTurnCollisionVolumeCount++];
    entry.operand = SCN_U32FromFloat(center.x);
    entry.offset = SCN_U32FromFloat(center.y);
    entry.dwordC = SCN_U32FromFloat(center.z);
    entry.dword10 = static_cast<uint16_t>(yaw);
    entry.dword14 = SCN_U32FromFloat(halfWidth);
    entry.dword18 = SCN_U32FromFloat(halfDepth);
    entry.dword1C = SCN_U32FromFloat(0.0f);
}

void AppendSHUTTurnVolumes(const CycleManager::ShutRow &row, float extentZ, float extentX) {




    const int32_t faceMask = static_cast<int32_t>(row.dword34);
    if (faceMask == 0 || faceMask == 15) {
        CycleManager::AppendDynamicTurnVolume(row.pos, row.angle1, extentZ, extentX);
        return;
    }

    const uint16_t yaw = static_cast<uint16_t>(row.angle1);
    const int16_t foldedYaw = yaw <= 0x8000u ? static_cast<int16_t>(yaw) : static_cast<int16_t>(-yaw);
    const float sinYaw = sin_fixed16(yaw);
    const float cosYaw = sin_fixed16(0x4000 - foldedYaw);
    uint32_t remainingMask = static_cast<uint32_t>(faceMask);
    for (uint32_t side = 0; side <= 0xC000u; side += 0x4000u) {
        if ((remainingMask & 1u) != 0) {
            const uint16_t sideYaw = static_cast<uint16_t>(side);
            const int16_t foldedSide = sideYaw <= 0x8000u ? static_cast<int16_t>(sideYaw) : static_cast<int16_t>(-sideYaw);
            const float lateral = sin_fixed16(sideYaw) * extentX;
            const float longitudinal = sin_fixed16(0x4000 - foldedSide) * extentZ;
            Vector3 center = row.pos;
            center.x += cosYaw * lateral - sinYaw * longitudinal;
            center.z += cosYaw * longitudinal + sinYaw * lateral;

            if ((side & 0x4000u) != 0) {
                CycleManager::AppendDynamicTurnVolume(center, row.angle1, extentZ, 0.01f);
            } else {
                CycleManager::AppendDynamicTurnVolume(center, row.angle1, 0.01f, extentX);
            }
        }
        remainingMask >>= 1u;
    }
}

void UpdateSHUTRowObject(CycleManager::ShutRow &row) {
    if (!CycleManager::IsReady())
        return;
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (cm.shutAreaID.name && cm.shutAreaID.name != CycleManager::CurrentAreaId())
        return;
    if (Character *const primary = row.token.name ? CycleManager::FindCharacter(row.token.name) : nullptr) {
        ApplyCharacterTransform(*primary, row.pos, {0, CycleManager::SubtractChildYawChain(row.angle1, primary->entity.node.child), 0});
        primary->motionFlags = static_cast<std::uint8_t>(row.byte31 != 2 ? (primary->motionFlags | 0x3fU) : (primary->motionFlags & 0xc0U));
        if (row.byte31 != 2) {
            CharacterComponentNode *const component = HLib_GetCharacterComponent(primary, SHEN_NAME(CCOW));
            const auto *const control = component ? reinterpret_cast<const std::byte *>(component + 1) : nullptr;
            Vector3 extents{};
            if (control)
                std::memcpy(&extents, control + 0x1c, sizeof(extents));
            CycleManager::AppendSHUTTurnVolumes(row, extents.z, extents.x);
        }
    }
    if (Character *const paired = row.modelToken.name ? CycleManager::FindCharacter(row.modelToken.name) : nullptr) {
        ApplyCharacterTransform(*paired, row.targetPos, {0, CycleManager::SubtractChildYawChain(row.angle2, paired->entity.node.child), 0});
        const bool shown = row.modelToken.name != 0 && row.byte31 != 0;
        paired->motionFlags = static_cast<std::uint8_t>(shown ? (paired->motionFlags | 0x3fU) : (paired->motionFlags & 0xc0U));
    }
}

void BindSHUTRowsToSceneObjects() {
    if (!CycleManager::IsReady())
        return;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (cm.shutRows.empty())
        return;
    if (cm.shutAreaID.name && cm.shutAreaID.name != CycleManager::CurrentAreaId())
        return;
    uint32_t primaryBound = 0;
    uint32_t primaryMissing = 0;
    uint32_t primaryModeled = 0;
    uint32_t pairedBound = 0;
    uint32_t pairedMissing = 0;
    uint32_t pairedModeled = 0;
    for (CycleManager::ShutRow &row : cm.shutRows) {
        const Character *const primary = row.token.name ? CycleManager::FindCharacter(row.token.name) : nullptr;
        if (primary) {
            ++primaryBound;
            if (primary->model)
                ++primaryModeled;
        } else if (row.token.name)
            ++primaryMissing;
        const Character *const paired = row.modelToken.name ? CycleManager::FindCharacter(row.modelToken.name) : nullptr;
        if (paired) {
            ++pairedBound;
            if (paired->model)
                ++pairedModeled;
        } else if (row.modelToken.name)
            ++pairedMissing;
        CycleManager::UpdateSHUTRowObject(row);
    }
}

bool DispatchSceneTask(uint32_t token, uint32_t dispatchState, uint32_t currentTime) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    ++cm.shutDispatches;

    CycleManager::ShutRow *row = CycleManager::FindSHUTRowByToken(token);
    if (!row) {
        ++cm.shutDispatchMissing;
        return false;
    }

    if (dispatchState <= 3u) {
        // A closing (odd) dispatch is refused while the active script's character or Ryo stands in this shop's INSH region (region
        // value = the row's 1-based index); for Ryo it also publishes the row's exit placement for Script_GetCycleManagerState. Both
        // tests use the ambient map query set.
        std::int32_t regionIndex = 0;
        std::int32_t value8 = 0;
        std::int32_t valueC = 0;
        std::int32_t value10 = 0;
        const CollisionPointQueryOutputs outputs{.value4 = &regionIndex, .value8 = &value8, .valueC = &valueC, .value10 = &value10};
        const auto *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        const std::uint32_t scriptedName = SCEN_HasActiveRuntimeObject() && SCEN_HasActiveScript() ? SCEN_GetActiveRuntimeObjectName() : 0;
        Character *const scripted = scriptedName && control && control->scenarioTask ? CHID_Find(control->scenarioTask, scriptedName) : nullptr;
        if (scripted && (dispatchState & 1u) != 0 && CheckIsPointIntersectWith(GetPositionForEntity(scripted->entity.node), &outputs, REGION_INSH.name) &&
            regionIndex == static_cast<std::int32_t>(row->cycleIndex))
            return true;
        Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
        if (ryo && row->dword34 && (dispatchState & 1u) != 0 && CheckIsPointIntersectWith(GetPositionForEntity(ryo->entity.node), &outputs, REGION_INSH.name) &&
            regionIndex == static_cast<std::int32_t>(row->cycleIndex)) {
            g_cycleShopExitPositionXY = row->qword38;
            g_cycleShopExitPositionZ = static_cast<std::int32_t>(row->dword40);
            g_cycleShopExitYaw = static_cast<std::uint16_t>(row->angle3);
            g_cycleShopExitPublished = 1;
            return true;
        }
    }

    if (dispatchState >= 4u) {
        if (row->lastDispatchTime > static_cast<std::int32_t>(currentTime))
            return false;
        row->lastDispatchTime = static_cast<std::int32_t>(currentTime);
    }

    const uint8_t previousByte31 = row->byte31;
    row->lastDispatchState = dispatchState;
    bool result = true;

    switch (dispatchState) {
    case 0:
    case 2:
        row->yStep = 0.01f;
        result = previousByte31 != 2;
        break;
    case 1:
    case 3:
        row->yStep = -0.01f;
        result = previousByte31 != 0;
        break;
    case 4:
    case 6:
        row->pos.y = row->maxY;
        row->byte31 = 2;
        result = false;
        break;
    case 5:
    case 7:
        row->pos.y = row->minY;
        row->byte31 = 0;
        result = false;
        break;
    default:
        break;
    }

    if (row->byte31 != previousByte31)
        ++cm.shutDispatchStateChanges;

    return result;
}

uint32_t CurrentTimeSeconds() {
    const GameDateTime &time = g_currentDateTime;
    return static_cast<uint32_t>(time.hours) * 3600u + static_cast<uint32_t>(time.minutes) * 60u + static_cast<uint32_t>(time.seconds);
}

} // namespace CycleManager


DSWK_t *DSWK_GetLocalTaskData() {
    if (!CycleManager::IsReady() || !g_cycleManagerRuntime.doorTaskData)
        return nullptr;
    return reinterpret_cast<DSWK_t *>(g_cycleManagerRuntime.doorTaskData);
}
namespace CycleManager {


void SetDoorTransitionDormant(CycleManager::NpcRecord &npc) {
    npc.cycle->pos.x = 1000.0f;
    npc.cycle->pos.y = 0.0f;
    npc.cycle->pos.z = 1000.0f;
    npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
    npc.cycle->actionTimerOrDoorHandle = -1;
}
} // namespace CycleManager


bool npcActivateIfNearInArea(CycleManager::NpcRecord &npc, float radius, const Vector3 *target, int16_t targetYaw, bool commit, uint16_t animId) {
    npc.cycle->savedSteeringHalfDepth = npc.cycle->steeringHalfDepth;
    if (npc.cycle->moveConfig->cycleClassCode >= 4u) {
        return target && PerformAicaSoundUpdates(npc, *target, targetYaw, commit);
    }
    npc.cycle->currentWaypointPos = npc.cycle->pos;
    if (target) {
        npc.cycle->moveTargetPos = {target->x, std::isnormal(target->y) || target->y == 0.0f ? target->y : 0.0f, target->z};
        npc.cycle->moveTargetYaw = targetYaw;
    }
    if (!std::isnormal(npc.cycle->pos.y) && npc.cycle->pos.y != 0.0f)
        npc.cycle->pos.y = 0.0f;
    if (!std::isnormal(npc.cycle->moveTargetPos.y) && npc.cycle->moveTargetPos.y != 0.0f)
        npc.cycle->moveTargetPos.y = 0.0f;
    float deltaX = npc.cycle->pos.x - npc.cycle->moveTargetPos.x;
    float deltaZ = npc.cycle->pos.z - npc.cycle->moveTargetPos.z;
    if (std::fabs(deltaX) < 0.00000011920929f)
        deltaX = 0.0f;
    if (std::fabs(deltaZ) < 0.00000011920929f)
        deltaZ = 0.0f;
    if (std::sqrt(deltaZ * deltaZ + deltaX * deltaX) <= radius && CycleManager::CurrentAreaId() == npc.cycle->areaID.name) {
        npc.cycle->xmptResumePending = 1;
        npc.cycle->animStateMode = 0;
        if (!npc.cycle->moveObjRuntime)
            npc.cycle->steeringHalfDepth = 0.0f;
        npc.cycle->tmpReqAnimId = animId;
        npc.cycle->animMoveSpeedScale = 0.041666668f;
        npc.cycle->workFloat4C = 1.0f;
        if (commit) {
            if (MotionInformationWork *const motion = npc.cycle->mot)
                motion->playbackRate = npc.cycle->moveConfig->motionRadiusTimingScalar;
        }
        return CycleManager::TickPositionAndHeadingMove(npc);
    }
    npc.cycle->pos = npc.cycle->moveTargetPos;
    npc.cycle->desiredPathYaw = npc.cycle->moveTargetYaw;
    npc.cycle->activeAnimId = animId;
    return false;
}
namespace CycleManager {


bool PrepareDoorTransitionCandidate(CycleManager::NpcRecord &npc, Vector3 &approachPos, int32_t &approachYaw) {
    DSWK_t *const doorSupport = reinterpret_cast<DSWK_t *>(DSWK_CurrentTaskDataAt(0, sizeof(DSWK_t)));
    if (!doorSupport || !DSWK_TryComputeNearestDoorApproachPose(*doorSupport, approachPos, approachYaw)) {
        return false;
    }
    return true;
}

bool StartDoorRequest(CycleManager::NpcRecord &npc) {
    if (npc.cycle->actionTimerOrDoorHandle >= 0)
        return true;

    // The original only enters the DSWK hand-off while the cycle has a live
    // character/MOTN work context.  Inactive cycles take the dormant branch;
    // allocating a request for one leaves the scenario door task with an owner
    // that CYCLE_CMD_CHECK_SCNF_HAS_CVWK can never resolve.
    if (!npc.cycle->motnSteeringActive || !npc.cycle->obj) {
        CycleManager::SetDoorTransitionDormant(npc);
        return true;
    }

    Vector3 approachPos = npc.cycle->pos;
    int32_t approachYaw = npc.cycle->desiredPathYaw;
    if (!CycleManager::PrepareDoorTransitionCandidate(npc, approachPos, approachYaw)) {
        CycleManager::SetDoorTransitionDormant(npc);
        return true;
    }

    const float dx = npc.cycle->pos.x - approachPos.x;
    const float dy = npc.cycle->pos.y - approachPos.y;
    const float dz = npc.cycle->pos.z - approachPos.z;
    const float distanceSq = dx * dx + dy * dy + dz * dz;
    if (distanceSq >= g_cycle_distance_check) {
        CycleManager::SetDoorTransitionDormant(npc);
        return true;
    }

    npc.cycle->currentWaypointPos = approachPos;
    npc.cycle->moveTargetYaw = static_cast<int16_t>(approachYaw);
    if (MotionInformationWork *const motion = npc.cycle->mot)
        motion->playbackRate = 1.0f;
    npc.cycle->moveTargetPos = npc.cycle->pos;
    npc.cycle->desiredPathYaw = static_cast<int16_t>(approachYaw);
    npc.cycle->actionTimerOrDoorHandle = -1;
    npcActivateIfNearInArea(npc, 16.0f, nullptr, 0, npc.cycle->motnSteeringActive, npc.cycle->moveConfig->motionParamIds[0]);
    npc.cycle->currentOpcode = CycleOpcode_RequestNearestDoorTransition;

    return true;
}




bool PollDoorRequest(CycleManager::NpcRecord &npc) noexcept {
    if (npc.cycle->currentOpcode != CycleOpcode_RequestNearestDoorTransition && npc.cycle->currentOpcode != CycleOpcode_DoorUse)
        return false;
    DSWK_t *const doorSupport = reinterpret_cast<DSWK_t *>(DSWK_CurrentTaskDataAt(0, sizeof(DSWK_t)));
    MotionInformationWork *const motion = npc.cycle->motnSteeringActive ? npc.cycle->mot : nullptr;
    if (npc.cycle->currentOpcode == CycleOpcode_RequestNearestDoorTransition) {
        if (npc.cycle->actionTimerOrDoorHandle == -1 && CycleManager::TickPositionAndHeadingMove(npc))
            return true;
        Vector3 approach = npc.cycle->pos;
        int32_t approachYaw = 0;
        if (!doorSupport || !DSWK_TryComputeNearestDoorApproachPose(*doorSupport, approach, approachYaw)) {
            CycleManager::SetDoorTransitionDormant(npc);
            return true;
        }
        if (IsTargetPointBlockedByRyoOrActor(npc, approach, 4.0f))
            return true;
        if (!motion) {
            npc.cycle->pos = {10000.0f, 10000.0f, 10000.0f};
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
            return false;
        }
        const auto allocate = [&]() {
            npc.cycle->actionTimerOrDoorHandle =
                DSWK_AllocateDoorRequest(*doorSupport, npc.cycle->moveConfig->obj_id.name, npc.cycle->pos, motion->basePoseTemplateIndex);
            if (npc.cycle->actionTimerOrDoorHandle < 0)
                ++g_cycleManagerRuntime.doorRequestAllocFailures;
            else
                ++g_cycleManagerRuntime.doorRequestAllocations;
        };
        if (npc.cycle->actionTimerOrDoorHandle == -1) {
            const float dx = npc.cycle->pos.x - approach.x;
            const float dy = npc.cycle->pos.y - approach.y;
            const float dz = npc.cycle->pos.z - approach.z;
            if (g_cycle_distance_check <= dy * dy + dx * dx + dz * dz) {
                CycleManager::SetDoorTransitionDormant(npc);
                return true;
            }
            allocate();
            return true;
        }
        const DSWKDoorRequestState state = DSWK_PollDoorRequest(*doorSupport, npc.cycle->actionTimerOrDoorHandle);
        if (state == DSWK_REQ_NOT_READY) {
            ++g_cycleManagerRuntime.doorRequestNotReadyPolls;
            allocate();
        } else if (state == DSWK_REQ_FAILED || state == DSWK_REQ_FREE) {
            ++g_cycleManagerRuntime.doorRequestFailedPolls;
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        } else if (state == DSWK_REQ_PENDING) {
            ++g_cycleManagerRuntime.doorRequestPendingPolls;
        } else {
            ++(state == DSWK_REQ_ACTIVE ? g_cycleManagerRuntime.doorRequestActivePolls : g_cycleManagerRuntime.doorRequestDonePolls);
            npc.cycle->currentWaypointPos = approach;
            npc.cycle->currentOpcode = CycleOpcode_DoorUse;
            motion->playbackRate = 1.0f;
            npc.cycle->pathMoveSpeed = 1.0f;
            npc.cycle->baseMoveSpeed = 1.0f;
        }
        return true;
    }

    npc.cycle->flags &= ~0x81U;
    if (!npc.cycle->motnSteeringActive) {
        npc.cycle->pos = {10000.0f, 10000.0f, 10000.0f};
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    }
    if (doorSupport && DSWK_PollDoorRequest(*doorSupport, npc.cycle->actionTimerOrDoorHandle) == DSWK_REQ_ACTIVE) {
        ++g_cycleManagerRuntime.doorRequestActivePolls;
        CycleManager::AppendDynamicTurnVolume(npc.cycle->currentWaypointPos, 0, 1.5f, 0.0f);
        return true;
    }
    ++g_cycleManagerRuntime.doorRequestDonePolls;
    npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
    if (npc.cycle->obj) {
        npc.cycle->pos = GetPositionForEntity(npc.cycle->obj->entity.node);
        npc.cycle->desiredPathYaw = CycleManager::NodeChainYaw(npc.cycle->obj);
    }
    return false;
}



uint32_t BeginShopShutterCommand(CycleManager::NpcRecord &npc, const char *record) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    ctx->morsCommandArg = *reinterpret_cast<const uint32_t *>(record + 4);
    ctx->morsCommandStatus = *reinterpret_cast<const uint32_t *>(record + 8);
    ctx->scheduledTriggerTime = 0;
    const uint16_t animId = ctx->moveConfig->motionParamIds[0];
    MotionInformationWork *const motion = npc.cycle->mot;
    if (ctx->obj && motion) {
        if (animId == ctx->activeAnimId) {
            motion->playbackRate = ctx->moveConfig->motionRadiusTimingScalar;
            motion->motionFlags &= ~2u;
            motion->pendingAnimId = animId;
        } else {
            freeTalkConv_SetMoveAndFacingConstraint(
                *ctx->obj, ctx->pos, ctx->desiredPathYaw, animId, 0.0f, ctx->moveConfig->motionRadiusTimingScalar, 0.0f, false);
        }
        ctx->activeAnimId = animId;
    }
    ctx->activeAnimId = ctx->moveConfig->motionParamIds[0];
    return 1;
}
} // namespace CycleManager


} // namespace shenmue::s1
