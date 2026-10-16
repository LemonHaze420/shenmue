#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {



bool cycleCheckIsActorInsideForwardBlockingQuad(CycleManager::NpcRecord &npc, bool skipRyo) {
    const uint16_t yaw = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
    const float cosine = cos_fixed16(yaw);
    const float sine = sin_fixed16(static_cast<int16_t>(yaw));
    const Vector3 nearLeft{npc.cycle->pos.x - cosine, 0.0f, sine + npc.cycle->pos.z};
    const Vector3 nearRight{cosine + npc.cycle->pos.x, 0.0f, npc.cycle->pos.z - sine};
    const float farX = npc.cycle->pos.x - sine * 3.0f;
    const float farZ = npc.cycle->pos.z - cosine * 3.0f;
    const Vector3 farRight{cosine + farX, 0.0f, farZ - sine};
    const Vector3 farLeft{farX - cosine, 0.0f, sine + farZ};
    if (!skipRyo && check_if_point_in_quadrilateral(nearLeft, nearRight, farRight, farLeft, g_cycleManagerRuntime.center))
        return true;
    const CycleManager::NpcRecord *const tracked = CycleManager::GetTrackedMobj(npc);
    for (const CycleManager::NpcRecord *other = CycleManager::NextProgramInList(CycleProgramList_SimSet); other;
         other = CycleManager::NextProgramInList(CycleProgramList_SimSet, other)) {
        if (other->cycle->moveConfig->obj_id.name != npc.cycle->moveConfig->obj_id.name && other != tracked &&
            check_if_point_in_quadrilateral(nearLeft, nearRight, farRight, farLeft, other->cycle->pos))
            return true;
    }
    return false;
}
namespace CycleManager {


void OffsetTalkMoveTarget(CycleManager::NpcRecord &npc, const CycleManager::NpcRecord &tracked, int32_t yawOffset, float distance) {
    const uint16_t sideYaw = static_cast<uint16_t>(static_cast<uint16_t>(tracked.cycle->desiredPathYaw) + yawOffset);
    npc.cycle->moveTargetPos.x -= sin_fixed16(sideYaw) * distance;
    npc.cycle->moveTargetPos.z -= sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(sideYaw)) * distance;
}



bool UpdateTalkMoveObjectState(CycleManager::NpcRecord &npc, bool motnSteeringActive, bool ryoTalk) {
    CycleManager::NpcRecord *const tracked = CycleManager::GetTrackedMobj(npc);
    if (!tracked || tracked->cycle->currentOpcode == CycleOpcode_RemoveCharacter ||
        npc.cycle->workFloat4C > CycleManager::DistanceSquared(npc.cycle->pos, npc.cycle->currentWaypointPos) ||
        tracked->cycle->currentOpcode == CycleOpcode_FollowStairPath2D) {
        npc.cycle->trackedMobj = nullptr;
        return false;
    }

    npc.cycle->moveTargetPos = tracked->cycle->pos;
    const uint16_t walkAnim = CycleManager::MotionParamWord(npc, 0u);
    const uint16_t idleAnim = CycleManager::MotionParamWord(npc, 1u);
    const uint16_t followAnim = CycleManager::MotionParamWord(npc, 8u);
    const uint16_t fastFollowAnim = CycleManager::MotionParamWord(npc, 11u);
    const auto resumePartner = [](CycleManager::NpcRecord &partner) {
        CycleManager::NpcRecord *const leader = CycleManager::GetTrackedMobj(partner);
        CycleManager::NpcRecord &walker = leader ? *leader : partner;
        if (((walker.cycle->currentOpcode - 1u) & 0xffdfu) == 0u)
            walker.cycle->steeringCommand = CycleSteerCmd_Resume;
    };

    if (!ryoTalk) {
        const float separationX = tracked->cycle->pos.x - npc.cycle->pos.x;
        const float separationZ = tracked->cycle->pos.z - npc.cycle->pos.z;
        if (separationZ * separationZ + separationX * separationX > 9.0f)
            resumePartner(*tracked);
        if (motnSteeringActive) {
            if (charaHasBlockingTPMRequest(npc.cycle->obj))
                return true;
            npc.cycle->pos = npc.cycle->obj ? GetPositionForEntity(npc.cycle->obj->entity.node) : Vector3{};
        }
        if (npc.cycle->areaID.name != tracked->cycle->areaID.name) {
            if (motnSteeringActive)
                npc.cycle->activeAnimId = idleAnim;
            else
                npc.cycle->trackedMobj = nullptr;
            return npc.cycle->trackedMobj != nullptr;
        }
        if (npc.cycle->activeAnimId != idleAnim) {
            (void)shenUpdateSteeringState(npc);
            if (npc.cycle->pathTurnRequested) {
                npc.cycle->activeAnimId = walkAnim;
                npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x2000);
                return true;
            }
            if (((npc.cycle->steeringState - 4u) & 0xfcu) == 0u && npc.cycle->steeringState != CycleSteerState_RequestTurn) {
                npc.cycle->activeAnimId = walkAnim;
                resumePartner(*tracked);
                return true;
            }
        }
        if (npc.cycle->xm_resume_state) {
            if (!tracked->cycle->xm_resume_state) {
                tracked->cycle->xm_resume_state = 1;
                g_bGlobalFlag = 1;
            }
            if (!g_cycleConversationHoldActive) {
                if (npc.cycle->xm_resume_state == 1) {
                    npc.cycle->activeAnimId = walkAnim;
                    npc.cycle->xm_resume_state = 2;
                    npcSetMoveFacingMot(npc, true, false);
                } else if (npc.cycle->xm_resume_state == 2) {
                    const MotionInformationWork *const motion = npc.cycle->mot;
                    if (!motion || motion->forcedFrameTime > 10.0f || (motion->motionFlags & 2u) != 0u)
                        g_cycleConversationHoldActive = 1;
                }
            }
            return true;
        }
        if (tracked->cycle->xm_resume_state) {
            npc.cycle->xm_resume_state = 1;
            g_bGlobalFlag = 1;
            npc.cycle->activeAnimId = idleAnim;
            return true;
        }
    }

    if (motnSteeringActive) {
        const uint32_t frames = ++npc.talkMoveAvoidanceFrames;
        constexpr float kSideOffset = 1.4142135f;
        const float offset = frames <= 120u ? static_cast<float>(static_cast<double>(frames) * 1.4142135 * 0.0083333338) : kSideOffset;
        bool probeClearPath = true;
        switch (npc.cycle->talkMoveAvoidanceState) {
        case 2:
        case 3:
            CycleManager::OffsetTalkMoveTarget(npc, *tracked, npc.cycle->talkMoveAvoidanceState == 2 ? 0x2000 : -0x2000, offset);
            if (CheckIntersectionOnPoint(npc.cycle->pos.x, npc.cycle->pos.z, npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z) ||
                cycleCheckIsActorInsideForwardBlockingQuad(npc, ryoTalk)) {
                npc.talkMoveAvoidanceFrames = 0;
                npc.cycle->talkMoveAvoidanceState = npc.cycle->talkMoveAvoidanceState == 2 ? 4 : 5;
            }
            probeClearPath = false;
            break;
        case 4:
        case 5:
            if (frames < 120u)
                CycleManager::OffsetTalkMoveTarget(npc, *tracked, npc.cycle->talkMoveAvoidanceState == 4 ? 0x2000 : -0x2000, kSideOffset - offset);
            else
                npc.cycle->talkMoveAvoidanceState = 1;
            probeClearPath = false;
            break;
        default:
            if (frames > 360u && !npc.cycle->talkMoveFlags) {
                const Vector3 target = npc.cycle->moveTargetPos;
                CycleManager::OffsetTalkMoveTarget(npc, *tracked, 0x2000, kSideOffset);
                const bool leftClear = !CheckIntersectionOnPoint(npc.cycle->pos.x, npc.cycle->pos.z, npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z);
                npc.cycle->moveTargetPos = target;
                if (leftClear) {
                    npc.talkMoveAvoidanceFrames = 0;
                    npc.cycle->talkMoveAvoidanceState = 2;
                } else {
                    CycleManager::OffsetTalkMoveTarget(npc, *tracked, -0x2000, kSideOffset);
                    const bool rightClear =
                        !CheckIntersectionOnPoint(npc.cycle->pos.x, npc.cycle->pos.z, npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z);
                    npc.cycle->moveTargetPos = target;
                    if (rightClear) {
                        npc.talkMoveAvoidanceFrames = 0;
                        npc.cycle->talkMoveAvoidanceState = 3;
                    }
                }
            }
            break;
        }
        if (probeClearPath && ryoTalk && CheckIntersectionOnPoint(npc.cycle->pos.x, npc.cycle->pos.z, npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z))
            return false;
    }

    float dx = npc.cycle->moveTargetPos.x - npc.cycle->pos.x;
    const float dy = npc.cycle->moveTargetPos.y - npc.cycle->pos.y;
    float dz = npc.cycle->moveTargetPos.z - npc.cycle->pos.z;
    float distance = 0.0f;
    if (dx > 0.00000011920929f || dx < -0.00000011920929f || dz > 0.00000011920929f || dz < -0.00000011920929f) {
        Vector3 direction{dx, dy, dz};
        distance = Vector3::NormalizeInPlace(direction);
        dx = direction.x;
        dz = direction.z;
    }
    npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(-dx, -dz);

    const uint32_t trackedOpcode = tracked->cycle->currentOpcode;
    const bool trackedFollowsPath = trackedOpcode <= CycleOpcode_WalkPath3D && ((0x200000802ULL >> trackedOpcode) & 1u) != 0u;
    if (!trackedFollowsPath && distance <= 2.0f) {
        if (!ryoTalk) {
            npc.cycle->activeAnimId = idleAnim;
            return true;
        }
    } else {
        if (std::fabs(tracked->cycle->animMoveSpeedScale) < 0.00000011920929f)
            tracked->cycle->animMoveSpeedScale = MOTN_CalculateRootMotionSpeedZ(*tracked);
        if (std::fabs(tracked->cycle->baseMoveSpeed) < 0.00000011920929f)
            tracked->cycle->baseMoveSpeed = 0.0f;
        const float trackedSpeed = tracked->cycle->animMoveSpeedScale * tracked->cycle->baseMoveSpeed;
        if (npc.cycle->activeAnimId == followAnim) {
            if (distance < 0.80000001f) {
                npc.cycle->activeAnimId = walkAnim;
                return true;
            }
            if (trackedSpeed > 0.0625f)
                npc.cycle->activeAnimId = fastFollowAnim;
        } else if (npc.cycle->activeAnimId == fastFollowAnim) {
            if (distance < 1.2f) {
                npc.cycle->activeAnimId = walkAnim;
                return true;
            }
            if (trackedSpeed < 0.050000004f)
                npc.cycle->activeAnimId = followAnim;
        } else if (npc.cycle->activeAnimId != walkAnim) {
            npc.cycle->activeAnimId = followAnim;
        } else {
            if (distance <= 2.0f)
                return true;
            npc.cycle->activeAnimId = trackedSpeed < 0.0625f ? followAnim : fastFollowAnim;
        }
    }

    npc.cycle->animMoveSpeedScale = MOTN_CalculateRootMotionSpeedZ(npc);
    float progress = npc.cycle->animMoveSpeedScale;
    if (tracked->cycle->currentOpcode == CycleOpcode_InitTimedPath)
        progress = static_cast<float>(static_cast<double>(tracked->cycle->baseMoveSpeed * tracked->cycle->animMoveSpeedScale) * 0.9);
    if (distance >= 1.5f) {
        if (distance > 2.0f)
            progress *= std::min(2.0f, distance - 0.5f) / 1.5f;
    } else {
        progress *= distance / 1.5f;
    }
    npc.cycle->baseMoveSpeed = progress / npc.cycle->animMoveSpeedScale;

    if (motnSteeringActive && npc.cycle->obj) {
        CycleManager::AdjustDesiredYawForSteeringCollision(npc);
        Character *const character = npc.cycle->obj;
        int16_t currentYaw = 0;
        for (const CharacterNode *node = &character->entity.node; node; node = node->child)
            currentYaw = static_cast<int16_t>(currentYaw + node->rotation.y);
        const auto yawDelta = static_cast<int16_t>(npc.cycle->desiredPathYaw - currentYaw);
        if (yawDelta != 0) {
            if (yawDelta > 364)
                npc.cycle->desiredPathYaw = static_cast<int16_t>(currentYaw + 364);
            else if (yawDelta < -364)
                npc.cycle->desiredPathYaw = static_cast<int16_t>(currentYaw - 364);
            int16_t rootYaw = npc.cycle->desiredPathYaw;
            for (const CharacterNode *node = character->entity.node.child; node; node = node->child)
                rootYaw = static_cast<int16_t>(rootYaw - node->rotation.y);
            character->entity.node.rotation.y = rootYaw;
        }
    }
    npc.yaw = npc.cycle->desiredPathYaw;

    if (npc.cycle->steeringState == CycleSteerState_Normal || npc.cycle->steeringState == CycleSteerState_RequestTurn) {
        const uint16_t heading = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
        npc.cycle->pos.x += -sin_fixed16(static_cast<int16_t>(heading)) * progress;
        npc.cycle->pos.z += -cos_fixed16(heading) * progress;
    }
    return true;
}



bool BeginRyoTalkWalk(Character *ryo) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    const uint32_t partnerId = scenGetFirstNonRyoChara();
    int32_t partnerIndex = 0;
    while (partnerIndex < static_cast<int32_t>(cm.npcRecords.size()) && cm.npcRecords[partnerIndex].cycle->moveConfig->obj_id.name != partnerId)
        ++partnerIndex;
    if (partnerIndex >= static_cast<int32_t>(cm.npcRecords.size()))
        return false;
    const CycleManager::NpcRecord &partner = cm.npcRecords[partnerIndex];
    if (partner.cycle->areaID.name != CycleManager::CurrentAreaId() || g_cycleTalkSuppressed ||
        !CheckIsPointIntersectWith(partner.cycle->pos, nullptr, SHEN_NAME(TALK)) || ((partner.cycle->currentOpcode - 1u) & 0xfffbu) != 0u ||
        g_pControlPadController->controlMode == 3)
        return false;

    CycleManager::NpcRecord &talk = g_cyclTalkCycle;
    talk.cycle->pos = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    talk.cycle->desiredPathYaw = CycleManager::NodeChainYaw(ryo);
    if (static_cast<uint16_t>(partner.cycle->desiredPathYaw - talk.cycle->desiredPathYaw + 0x4000) > 0x8000u) {
        g_bGlobalFlag = 1;
        return false;
    }
    CycleManager::EnableRootMotion(ryo, true);
    talk.cycle->obj = ryo;
    talk.cycle->mot = MOTN_getMOTM(ryo);
    talk.cycle->mot->motionFlags &= ~8u;
    talk.cycle->currentWaypointPos = {1000.0f, 1000.0f, 1000.0f};
    talk.cycle->workFloat4C = 1.0f;
    talk.talkMoveAvoidanceFrames = 0;
    talk.cycle->trackedMobj = partner.cycle;
    const int16_t bearing = CycleManager::Atan2Fixed16(talk.cycle->pos.x - partner.cycle->pos.x, talk.cycle->pos.z - partner.cycle->pos.z);
    talk.cycle->talkMoveAvoidanceState = static_cast<int16_t>(bearing - talk.cycle->desiredPathYaw) > 0 ? 3 : 2;
    return true;
}
} // namespace CycleManager




bool TALK(Character *ryo) {
    CycleManager::NpcRecord &talk = g_cyclTalkCycle;
    MotionInformationWork *const motion = talk.cycle->mot;
    const CycleManager::NpcRecord *const partner = CycleManager::GetTrackedMobj(talk);
    if (partner && partner->cycle->obj && CheckIsPointIntersectWith(partner->cycle->pos, nullptr, SHEN_NAME(TALK))) {
        talk.cycle->pos = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
        if (CycleManager::UpdateTalkMoveObjectState(talk, true, true)) {
            talk.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(talk.cycle->pos);
            if (ryo) {
                int16_t rootYaw = talk.cycle->desiredPathYaw;
                for (const CharacterNode *node = ryo->entity.node.child; node; node = node->child)
                    rootYaw = static_cast<int16_t>(rootYaw - node->rotation.y);
                ryo->entity.node.rotation.y = rootYaw;
            }
            if (motion->currentAnimId != talk.cycle->activeAnimId) {
                motion->motionFlags &= ~2u;
                motion->pendingAnimId = talk.cycle->activeAnimId;
            }
            motion->playbackRate = talk.cycle->baseMoveSpeed;
            if (g_bGlobalFlag)
                g_shouldComputeBetweenTwoEntities = 0;
            return true;
        }
    }
    talk.cycle->activeAnimId = CycleManager::MotionParamWord(talk, 0u);
    if (motion->currentAnimId != talk.cycle->activeAnimId) {
        motion->motionFlags &= ~2u;
        motion->pendingAnimId = talk.cycle->activeAnimId;
        motion->playbackRate = 1.0f;
    }
    return false;
}
namespace CycleManager {


bool BeginTalkMoveObjectState(CycleManager::NpcRecord &npc, uint32_t recordOffset) {
    uint32_t targetObjectId = 0;
    Vector3 talkMovePos{};
    uint16_t talkMoveFlags = 0;
    const std::vector<uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (!ReadValue(bytes, static_cast<size_t>(recordOffset) + 4u, targetObjectId) || !ReadValue(bytes, static_cast<size_t>(recordOffset) + 8u, talkMovePos.x) ||
        !ReadValue(bytes, static_cast<size_t>(recordOffset) + 12u, talkMovePos.y) ||
        !ReadValue(bytes, static_cast<size_t>(recordOffset) + 16u, talkMovePos.z) ||
        !ReadValue(bytes, static_cast<size_t>(recordOffset) + 20u, talkMoveFlags)) {
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    }
    npc.cycle->trackedMobj = npcFindEntry(targetObjectId);
    CycleManager::NpcRecord *const tracked = CycleManager::GetTrackedMobj(npc);
    if (!tracked || npc.cycle->areaID.name != tracked->cycle->areaID.name || CycleManager::DistanceSquared(npc.cycle->pos, tracked->cycle->pos) >= 25.0f) {
        npc.cycle->trackedMobj = nullptr;
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        ++g_cycleManagerRuntime.talkMoveTargetMisses;
        return false;
    }

    npc.cycle->currentWaypointPos = talkMovePos;
    npc.cycle->workFloat4C = 9.0f;
    npc.cycle->steeringState = CycleSteerState_Normal;
    npc.cycle->steeringCommand = CycleSteerCmd_None;
    npc.cycle->reversePath = 0;
    npc.cycle->pathTurnRequested = 0;
    npc.cycle->steeringBlockedFrames = 0;
    npc.cycle->reversePathFrames = 0;
    npc.cycle->steeringWaitFrames = 0;
    npc.cycle->talkMoveFlags = talkMoveFlags;
    npc.cycle->talkMoveAvoidanceState = 1;
    npc.cycle->currentOpcode = CycleOpcode_BeginTalkMoveToTaggedPosition;
    ++g_cycleManagerRuntime.talkMoveBegins;
    return CycleManager::UpdateTalkMoveObjectState(npc, false, false);
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {



void HoldForConversation(CycleManager::NpcRecord &npc) {
    Character *const obj = npc.cycle->obj;
    MotionInformationWork *const motion = npc.cycle->mot;
    if (obj && charaHasBlockingTPMRequest(obj))
        return;
    if (npc.cycle->xmptResumePending) {
        if (obj && charaIsActiveMode(obj, 4)) {
            UpdateEntityOverPositionWithFlags(npc, true);
            npc.cycle->xmptResumePos = npc.cycle->pos;
        }
        npc.cycle->xmptResumePending = 0;
        return;
    }
    switch (npc.cycle->currentOpcode) {
    case CycleOpcode_InitTimedPath:
    case CycleOpcode_WalkPath3D:
        if (!g_bGlobalFlag && CycleManager::CurrentAreaId() == SHEN_NAME(MFSY) && CheckIsPointIntersectWith(npc.cycle->pos, nullptr, SHEN_NAME(TALK)) &&
            g_pControlPadController && g_pControlPadController->controlMode != 3) {
            npc.cycle->xformDirty = true;
            return;
        }
        g_bGlobalFlag = 1;
        npc.cycle->steeringCommand = CycleSteerCmd_Resume;
        (void)shenUpdateSteeringState(npc);
        if (!npc.cycle->xm_resume_state || npc.cycle->xm_resume_state >= 3 || !obj || !motion)
            return;
        if (npc.cycle->xm_resume_state == 1) {
            CycleManager::EnableRootMotion(obj, true);
            charaSubmitMoveRequest(obj, &npc.cycle->pos, npc.cycle->desiredPathYaw, npc.cycle->moveConfig->motionParamIds[0], nullptr,
                npc.cycle->moveConfig->motionRadiusTimingScalar, 6);
            npc.cycle->xm_resume_state = 2;
            return;
        }
        if (charaIsActiveMode(obj, 6))
            return;
        npc.cycle->xm_resume_state = 3;
        g_cycleConversationHoldActive = 1;
        npc.cycle->xformDirty = true;
        return;
    case CycleOpcode_SetActiveAnimAndMotionVariant:
    case CycleOpcode_ScheduleBoundary:
    case CycleOpcode_WaitSeconds:
    case CycleOpcode_WaitForTaggedCycleInRadius:
    case CycleOpcode_WaitHold:
    case CycleOpcode_PayloadYield:
    case CycleOpcode_UseMnrBEntry:
    case CycleOpcode_ScheduleMnrBEntry:
    case CycleOpcode_PlayEntryStateAnimPoolUntilTime:
    case CycleOpcode_WaitForTaggedTalkMoveInRadius:
        if (npc.cycle->motnSteeringActive && obj && motion && motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[8] &&
            npc.cycle->animStateMode == 1)
            motion->playbackRate = 0.0f;
        break;
    case CycleOpcode_BeginTalkMoveToTaggedPosition:
        if (CycleManager::UpdateTalkMoveObjectState(npc, npc.cycle->motnSteeringActive, false) && npc.cycle->motnSteeringActive && obj) {
            Vector3 position = GetPositionForEntity(obj->entity.node);
            position.y = MapProbe_UpdateIntersectionStateAtPoint(position);
            shenApplyTranslationToAll(obj->entity.node, position);
        }
        npc.cycle->xformDirty = true;
        return;
    default:
        if (npc.cycle->motnSteeringActive && obj && motion && (npc.cycle->animStateMode == 1 || npc.cycle->currentOpcode == CycleOpcode_SetPositionAndHeading))
            motion->playbackRate = 0.0f;
        break;
    }
    g_cycleConversationHoldActive = 1;
    npc.cycle->xformDirty = true;
}

} // namespace shenmue::s1::CycleManager
