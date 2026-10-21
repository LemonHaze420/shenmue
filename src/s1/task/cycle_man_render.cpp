#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
namespace CycleManager {


void UpdateMoveObjectRenderNodes(CycleMoveObjectRuntime &runtime) {
    auto *const root = reinterpret_cast<Mt5RuntimeNode *>(runtime.model);
    std::byte *const modelBase = root ? MT5_ModelBaseForNode(root) : nullptr;
    if (!root || !modelBase)
        return;

    const auto findNode = [&](std::uint16_t nodeKind) { return FindRuntimeNodeByKind(root, modelBase, nodeKind); };
    const auto offsetNode = [&](std::int32_t offset) -> Mt5RuntimeNode * { return offset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + offset) : nullptr; };

    if (runtime.moveObjectKind == 5u) {
        if (Mt5RuntimeNode *const wheel = findNode(8))
            wheel->rotation.x = -runtime.yawVelocity;
        for (const std::uint16_t nodeKind : {std::uint16_t{23}, std::uint16_t{18}}) {
            if (Mt5RuntimeNode *const node = findNode(nodeKind)) {
                node->rotation.x = -runtime.yawVelocity;
                node->rotation.y = -runtime.steeringAngle;
            }
        }
        if (Mt5RuntimeNode *const suspension = findNode(154))
            suspension->rotation.z = -runtime.steeringAngle;
        return;
    }

    if (runtime.moveObjectKind == 4u)
        return;

    const std::uint32_t seconds = CycleManager::CurrentTimeSeconds();
    const bool daytimeState = runtime.activeFlag != 0 || seconds - 25201U <= 37798U;
    if (Mt5RuntimeNode *const steering = findNode(14))
        steering->rotation.y = daytimeState ? runtime.steeringAngle : 0;
    for (const std::uint16_t nodeKind : {std::uint16_t{23}, std::uint16_t{18}}) {
        if (Mt5RuntimeNode *const node = findNode(nodeKind))
            node->rotation.x = -runtime.yawVelocity;
    }
    if (Mt5RuntimeNode *const wheel = findNode(1)) {
        wheel->rotation.x = -runtime.wheelSpin;
        if (Mt5RuntimeNode *const child = offsetNode(wheel->childOffset)) {
            child->rotation.x = runtime.wheelSpin;
            if (Mt5RuntimeNode *const sibling = offsetNode(child->siblingOffset))
                sibling->rotation.x = runtime.wheelSpin;
        }
    }
    if (Mt5RuntimeNode *const output = findNode(10))
        output->rotation.x = runtime.animOutput;
    if (Mt5RuntimeNode *const body = findNode(196)) {
        if (daytimeState) {
            body->position.y = -100.0f;
        } else {
            body->position.y = runtime.modelAttachOffset.x;
            body->rotation.z = -runtime.animLeanAngle;
        }
    }
}
} // namespace CycleManager


} // namespace shenmue::s1

namespace shenmue::s1::CycleManager {




void AdvanceTaggedPayloadFades(CycleManager::NpcRecord &npc) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    const bool active = ctx->motnSteeringActive && ctx->obj && ctx->mot;
    CycleManager::TaggedPayload *previous = nullptr;
    for (CycleManager::TaggedPayload *payload = ctx->taggedPayloadList; payload;) {
        CycleManager::TaggedPayload *const next = payload->next;
        std::uint8_t state = static_cast<std::uint8_t>(payload->lifetime);
        std::uint8_t counter = static_cast<std::uint8_t>(payload->lifetime >> 8);
        bool remove = false;
        if (active) {
            if (state == 3u) {
                if (counter-- == 1u) {
                    if (payload->retainAfterFade) {
                        state = 0u;
                    } else {
                        remove = true;
                    }
                }
            } else if (state == 2u) {
                ++counter;
                if (counter >= 30u)
                    state = 1u;
            }
        } else if (state == 3u) {
            remove = true;
        } else if (state == 2u) {
            state = 1u;
            counter = 30u;
        }

        if (remove) {
            if (previous)
                previous->next = next;
            else
                ctx->taggedPayloadList = next;
            payload->next = g_CycleManager_Current;
            g_CycleManager_Current = payload;
        } else {
            payload->lifetime = static_cast<std::uint16_t>(state | (static_cast<std::uint16_t>(counter) << 8));
            previous = payload;
        }
        payload = next;
    }
}




void CollectTaggedPayloadDraws(CycleManager::NpcRecord &npc) {
    for (CycleManager::TaggedPayload *payload = npc.cycle->taggedPayloadList; payload; payload = payload->next) {
        if (!payload->model) {
            if (Character *const modelOwner = CycleManager::FindCharacter(payload->command[2]))
                payload->model = modelOwner->model;
            continue;
        }
        const uint32_t poseIndex = payload->command[5];
        if (poseIndex < 15u) {
            if (payload->mode == 0u) {
                mhndApplyCharaHandPose(npc.cycle->obj, 1, static_cast<std::int32_t>(poseIndex), 10);
            } else if (payload->mode == 1u) {
                mhndApplyCharaHandPose(npc.cycle->obj, 0, static_cast<std::int32_t>(poseIndex), 10);
            } else if (payload->mode == 2u || payload->mode == 3u) {
                mhndApplyCharaHandPose(npc.cycle->obj, 2, static_cast<std::int32_t>(poseIndex), 10);
            }
        }
        payload->drawNext = g_CM_obj_list;
        g_CM_obj_list = payload;
    }
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {





void RenderTaggedPayloads() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::NpcRecord &npc : cm.npcRecords)
        CycleManager::AdvanceTaggedPayloadFades(npc);

    g_CM_obj_list = nullptr;
    for (CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet); npc;
         npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        const MotionInformationWork *const motion = npc->cycle->mot;
        if (npc->cycle->motnSteeringActive && motion && npc->cycle->obj && !motion->updateMode)
            CycleManager::CollectTaggedPayloadDraws(*npc);
    }

    for (CycleManager::TaggedPayload *payload = g_CM_obj_list; payload; payload = payload->drawNext) {
        CycleManager::NpcRecord *const npc = CycleManager::CycleRecord(payload->owner);
        const std::uint8_t state = static_cast<std::uint8_t>(payload->lifetime);
        if (!state)
            continue;
        Matrix4x4_Push();
        float alpha = npc->cycle->obj->renderOpacity;
        if (state >= 2u)
            alpha *= static_cast<float>(static_cast<float>(payload->lifetime >> 8) / 30.0);
        Matrix4x4::SetActiveTransformToPrePass();
        bool direct = true;
        if (ProcessMatrixTransformState(*npc, *payload, payload->mode)) {
            CycleManager::ApplyTaggedPayloadPlacement(*payload);
            static_cast<void>(SubmitRiggedSceneObjectForDrawing(payload->model, nullptr, 0, 0));
            direct = !CachedAttachmentOwnsSubmission(payload->model, alpha, workTransformMatrix);
        }
        Matrix4x4::Pop(1);
        const float drawAlpha = alpha <= 0.99f ? alpha : 1.0f;
        if (direct && ProcessMatrixTransformState(*npc, *payload, payload->mode)) {
            CycleManager::ApplyTaggedPayloadPlacement(*payload);
            d3t::MT5_SubmitObjectNode(payload->model, drawAlpha);
        }
        Matrix4x4::Pop(1);
        if ((npc->cycle->obj->flags.value & 0x80u) != 0u && g_dbg_draw_enabled) {
            Matrix4x4_Push();
            Matrix4x4::Multiply(&g_objectDebugMatrix);
            if (ProcessMatrixTransformState(*npc, *payload, payload->mode)) {
                CycleManager::ApplyTaggedPayloadPlacement(*payload);
                d3t::MT5_SubmitObjectNode(payload->model, drawAlpha);
            }
            Matrix4x4::Pop(1);
        }
    }
}

void RenderMoveObjects() {
    for (CycleMoveObjectRuntime *runtime = g_visibleMoveObjects; runtime; runtime = runtime->visibleNext) {
        CycleManager::UpdateMoveObjectRenderNodes(*runtime);
        SubmitCycleMoveObject(*runtime);
    }
}
} // namespace CycleManager


} // namespace shenmue::s1
