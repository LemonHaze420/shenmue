#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1::CycleManager {

CycleMoveObjectRuntime *MOBJ_DynamicRuntimes() {
    return g_tmp_MOBJ ? reinterpret_cast<CycleMoveObjectRuntime *>(g_tmp_MOBJ) : nullptr;
}



void MOBJ_ResetDynamicRuntimes() {
    if (!g_tmp_MOBJ)
        return;
    CycleMoveObjectRuntime *const runtimes = MOBJ_DynamicRuntimes();
    for (std::size_t index = 0; index + 1 < kMobjDynamicRuntimeCount; ++index)
        runtimes[index].next = &runtimes[index + 1];
    runtimes[kMobjDynamicRuntimeCount - 1].next = nullptr;
    g_freeMoveObjects = runtimes;
    g_visibleMoveObjects = nullptr;
    g_parkedMoveObjects = nullptr;
}


void UnlinkParkedMoveObject(CycleMoveObjectRuntime *runtime) {
    if (!runtime || !g_parkedMoveObjects)
        return;
    if (g_parkedMoveObjects == runtime) {
        g_parkedMoveObjects = runtime->next;
        return;
    }
    for (CycleMoveObjectRuntime *previous = g_parkedMoveObjects; previous->next; previous = previous->next) {
        if (previous->next == runtime) {
            previous->next = runtime->next;
            runtime->activeFlag = 0;
            return;
        }
    }
}

std::size_t MOBJ_DynamicRuntimeCount() {
    const CycleMoveObjectRuntime *const runtimes = MOBJ_DynamicRuntimes();
    if (!runtimes)
        return 0;
    return static_cast<std::size_t>(
        std::count_if(runtimes, runtimes + kMobjDynamicRuntimeCount, [](const CycleMoveObjectRuntime &runtime) { return runtime.ownerObjectId != 0; }));
}



void TransformMatrixWithEulerAngles(Matrix4x4 &matrix) {
    const float upX = matrix.rows[1].x;
    const float upZ = matrix.rows[1].z;
    if (!(static_cast<double>(std::fabs(upX)) >= 0.001 || static_cast<double>(std::fabs(upZ)) >= 0.001))
        return;
    const std::uint16_t heading = Shenmue_Atan2Fixed(upX, upZ);
    Matrix4x4_Push();
    workTransformMatrix = matrix;
    const Vector3i euler = CHAR_EulerAnglesFromMatrix(matrix);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-euler.x));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-euler.y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-euler.z));
    Matrix4x4::RotateAroundX(sin_fixed16(0x4000), sin_fixed16(0));
    Matrix4x4::RotateAroundY(sin_fixed16(-0x4000), sin_fixed16(0));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-heading));
    for (std::size_t row = 0; row < 3; ++row) {
        matrix.rows[row].x = workTransformMatrix.rows[row].x;
        matrix.rows[row].y = workTransformMatrix.rows[row].y;
        matrix.rows[row].z = workTransformMatrix.rows[row].z;
    }
    Matrix4x4::Pop(1);
}





bool ProcessMatrixTransformState(const CycleManager::NpcRecord &npc, const CycleManager::TaggedPayload &payload, std::uint32_t mode) {
    const MotionInformationWork *const motion = npc.cycle->mot;
    Matrix4x4 frame{};
    const Matrix4x4 *attachment = &frame;
    if (mode == 10u) {
        if (!motion)
            return false;
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(motion->rootPreTranslate.x, motion->rootPreTranslate.y, motion->rootPreTranslate.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(motion->entityEuler.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(motion->entityEuler.x));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(motion->entityEuler.z));
        frame = workTransformMatrix;
        Matrix4x4::Pop(1);
    } else if (mode == 11u) {
        if (!motion)
            return false;
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(npc.cycle->xmptResumePos.x, npc.cycle->xmptResumePos.y, npc.cycle->xmptResumePos.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(npc.cycle->desiredPathYaw));
        frame = workTransformMatrix;
        Matrix4x4::Pop(1);
    } else {
        const std::uint8_t boneId = mode < kCycleTaggedPayloadBoneIds.size() ? kCycleTaggedPayloadBoneIds[mode] : 0u;
        attachment = GetTransformForFACE_ForSpecificChara(npc.cycle->obj, boneId);
        if (!attachment)
            return false;
    }
    if (payload.command[3] == 1u) {
        frame = *attachment;
        TransformMatrixWithEulerAngles(frame);
        attachment = &frame;
    }
    Matrix4x4::Multiply(attachment);
    return true;
}

// The payload's placement in its attachment frame: its position, then Z, Y and X rotation.
void ApplyTaggedPayloadPlacement(const CycleManager::TaggedPayload &payload) {
    Matrix4x4::preTranslate(std::bit_cast<float>(payload.payload[0]), std::bit_cast<float>(payload.payload[1]), std::bit_cast<float>(payload.payload[2]));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(payload.payload[5]));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(payload.payload[4]));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(payload.payload[3]));
}

CycleManager::TaggedPayload *FindTaggedPayload(CycleManager::NpcRecord &npc, uint32_t identifier) {
    CycleManager::TaggedPayload *payload = npc.cycle->taggedPayloadList;
    while (payload && payload->command[1] != identifier)
        payload = payload->next;
    return payload;
}




bool QueueObjectInteraction(CycleManager::NpcRecord &npc, uint32_t offset) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    const uint32_t *const command = reinterpret_cast<const uint32_t *>(CycleManager::FilePointer(offset));
    CycleManager::TaggedPayload *payload = CycleManager::FindTaggedPayload(npc, command[1]);
    const bool reused = payload != nullptr;
    if (!payload) {
        payload = g_CycleManager_Current;
        if (payload) {
            g_CycleManager_Current = payload->next;
            payload->next = nullptr;
        }
    }
    if (!payload)
        return false;
    if (static_cast<int32_t>(command[4]) >= 12) {
        payload->next = g_CycleManager_Current;
        g_CycleManager_Current = payload;
        return false;
    }
    payload->mode = command[4];
    payload->model = nullptr;
    for (size_t i = 0; i < payload->payload.size(); ++i)
        payload->payload[i] = command[6 + i];
    payload->owner = ctx;
    payload->command = command;
    payload->retainAfterFade = 0;
    if (!reused) {
        payload->next = ctx->taggedPayloadList;
        ctx->taggedPayloadList = payload;
    }
    payload->lifetime = 2;
    return true;
}




void CommitXForm_RotateTranslateZYX(CycleManager::NpcRecord &npc, uint32_t offset) {
    CycleManager::TaggedPayload *const payload = CycleManager::FindTaggedPayload(npc, CycleManager::ReadCommandDwordOrZero(offset, 4u));
    if (!payload)
        return;
    const uint32_t mode = CycleManager::ReadCommandDwordOrZero(offset, 8u);
    Matrix4x4::SetActiveTransformToPrePass();
    ProcessMatrixTransformState(npc, *payload, mode);
    Matrix4x4::InvertInPlace(nullptr);
    ProcessMatrixTransformState(npc, *payload, payload->mode);
    CycleManager::ApplyTaggedPayloadPlacement(*payload);
    payload->payload[0] = std::bit_cast<std::uint32_t>(workTransformMatrix.rows[3].x);
    payload->payload[1] = std::bit_cast<std::uint32_t>(workTransformMatrix.rows[3].y);
    payload->payload[2] = std::bit_cast<std::uint32_t>(workTransformMatrix.rows[3].z);
    const Matrix4x4 placement = workTransformMatrix;
    const Vector3i euler = CHAR_EulerAnglesFromMatrix(placement);
    payload->payload[3] = static_cast<std::uint32_t>(euler.x);
    payload->payload[4] = static_cast<std::uint32_t>(euler.y);
    payload->payload[5] = static_cast<std::uint32_t>(euler.z);
    Matrix4x4::Pop(1);
    payload->mode = mode;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {




void CreateCharacter(CycleManager::NpcRecord &npc) {
    Character *const chara = npc.cycle->obj;
    const char *const name = reinterpret_cast<const char *>(npc.cycle->create_chara_name);
    if (!chara || !name[0])
        return;
    const ImageModuleWork *const previousImage = IMGM_GetWork(chara);
    const uint16_t previousTemplate = previousImage ? previousImage->basePoseId : 0u;
    MDLM_ActivateCharacterImage(chara, name, 0);
    MotionManagerWork *const motion = MOTM_GetWork(chara);
    if (!motion)
        return;
    const ImageModuleWork *const image = IMGM_GetWork(chara);
    if ((image ? image->basePoseId : 0u) != previousTemplate)
        MOTI_Free(motion->motion);
    CLMD_Flag(chara);
    OSAGE_Refresh(chara);
    ++g_cycleManagerRuntime.originalProgramCreateCharacterLoads;
}


bool InitializeMoveObjContext(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool history) {
    uint32_t count = 0;
    uint32_t dwords = 0;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (!CycleManager::ReadCommandValue(recordOffset, 12, count) || !CycleManager::ReadCommandValue(recordOffset, 16, dwords) || dwords < 5 ||
        dwords > UINT32_MAX / 4u || recordOffset > cm.bytes.size() || dwords * 4u > cm.bytes.size() - recordOffset)
        return false;
    npc.cycle->moveObjRef = reinterpret_cast<CycleMoveObjCommand *>(CycleManager::FilePointer(recordOffset));
    npc.cycle->moveObjSubcommandCount = count;
    npc.cycle->moveObjSubcommandIndex = 0;
    npc.cycle->moveObjSubcommandPtr = CycleManager::FilePointer(recordOffset + 20u);
    npc.cycle->moveObjPhase = CycleMoveObjPhase_Uninitialized;
    npc.cycle->moveObjSubOpcode = 0;
    npc.cycle->b_paused = false;
    npc.cycle->moveObjPathActive = false;
    const uint32_t chairGroup = MCIR_FindChairGroupById(npc.cycle->moveObjRef->chairGroupId);
    if (!chairGroup)
        return false;
    const uint32_t objectId = npc.cycle->moveConfig->obj_id.name;
    uint32_t chairPos = MCIR_FindChairPosInfoByOccupantObjectId(objectId, chairGroup);
    if (chairPos) {
        npc.cycle->b_paused = true;
    } else {
        const int32_t time = npc.cycle->moveObjRef->time;
        npc.cycle->time = time < 0 ? CycleManager::CurrentTimeSeconds() - static_cast<uint32_t>(time) : static_cast<uint32_t>(time);
        npc.cycle->b_paused = history;

        chairPos = MCIR_FindChairPosInfoByOccupantObjectId(0, chairGroup);
        if (chairPos) {
            std::memcpy(cm.bytes.data() + chairPos + 0x10u, &objectId, sizeof(objectId));
            Vector3 seat{};
            uint32_t pointCount = 0;
            uint32_t pointOffset = 0;
            ReadValue(cm.bytes, chairPos, seat);
            ReadValue(cm.bytes, chairPos + 0x14u, pointCount);
            ReadValue(cm.bytes, chairPos + 0x18u, pointOffset);
            Vector3 penultimate{};
            if (pointCount >= 2 && ReadValue(cm.bytes, cm.mcirOffset + pointOffset + (pointCount - 2u) * sizeof(Vector3), penultimate) &&
                std::fabs(seat.x - penultimate.x) < 0.001f && std::fabs(seat.y - penultimate.y) < 0.001f && std::fabs(seat.z - penultimate.z) < 0.001f)
                npc.cycle->b_paused = true;
        } else if (history) {
            return false;
        }
    }
    const bool diagnosticArea = !npc.cycle->b_paused && npc.cycle->areaID.name == CycleManager::CurrentAreaId() &&
        (npc.cycle->areaID.name == CYCLE_CHAIR_AREA_DRME.name || npc.cycle->areaID.name == CYCLE_CHAIR_AREA_DSLI.name);
    if (diagnosticArea || (npc.cycle->b_paused && chairPos && !CycleManager::HasMoveObjChairPreset(npc))) {
        uint32_t index = g_TotalActiveCycles;
        if (index >= 10u)
            index = 0;
        g_CycleTable[2u * index] = objectId;
        if (++index >= 10u)
            index = 0;
        g_CycleTable[2u * index] = npc.cycle->moveObjRef->chairGroupId;
        g_TotalActiveCycles = index + 1u;
    }
    if (diagnosticArea)
        return false;
    if (npc.cycle->b_paused) {
        const uint16_t animation = CycleManager::FindMoveObjIdleMotion(npc);
        if (animation)
            npc.cycle->activeAnimId = animation;
        if (chairPos) {
            ReadValue(cm.bytes, chairPos + 0x0Cu, npc.cycle->moveObjBaseYaw);
            npc.cycle->moveTargetYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
            npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
            npc.cycle->pos = CycleManager::MoveObjPresetChairPosition(npc, chairPos);
        }
    }
    return true;
}

bool BeginTaggedWait(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t recordOffset, uint32_t currentTime) {
    uint32_t targetObjectId = 0;
    float radius = 0.0f;
    const std::vector<uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (!ReadValue(bytes, static_cast<size_t>(recordOffset) + 4u, targetObjectId) || !ReadValue(bytes, static_cast<size_t>(recordOffset) + 8u, radius)) {
        npc.cycle->trackedMobj = nullptr;
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    }
    npc.cycle->steeringWaitFrames = currentTime + 1200u;
    npc.cycle->workFloat4C = (radius + 1.0f) * (radius + 1.0f);
    npc.cycle->trackedMobj = npcFindEntry(targetObjectId);
    npc.cycle->currentOpcode = static_cast<CycleOpcode>(opcode);
    ++g_cycleManagerRuntime.taggedWaitBegins;
    if (!npc.cycle->trackedMobj) {
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        ++g_cycleManagerRuntime.taggedWaitTargetMisses;
        return false;
    }
    return true;
}

bool UpdateTaggedWait(CycleManager::NpcRecord &npc, uint32_t currentTime) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    CycleManager::NpcRecord *const tracked = CycleManager::GetTrackedMobj(npc);
    const bool timedOut = npc.cycle->steeringWaitFrames < currentTime;
    bool complete = false;

    if (npc.cycle->currentOpcode == CycleOpcode_WaitForTaggedCycleInRadius) {
        if (timedOut) {
            uint32_t nextOpcode = 0;
            const bool followedByTalkMove = tracked && npc.cycle->motnSteeringActive &&
                ReadValue(cm.bytes, CycleManager::FileOffset(npc.cycle->cycleCommandPtr), nextOpcode) && nextOpcode == CycleOpcode_BeginTalkMoveToTaggedPosition;
            complete = !followedByTalkMove;
        } else if (!tracked) {
            complete = true;
        } else {
            const float dx = npc.cycle->pos.x - tracked->cycle->pos.x;
            const float dy = npc.cycle->pos.y - tracked->cycle->pos.y;
            const float dz = npc.cycle->pos.z - tracked->cycle->pos.z;
            complete = dx * dx + dy * dy + dz * dz < npc.cycle->workFloat4C;
        }
    } else if (npc.cycle->currentOpcode == CycleOpcode_WaitForTaggedTalkMoveInRadius) {
        if (timedOut || !tracked) {
            complete = true;
        } else if (tracked->cycle->currentOpcode == CycleOpcode_BeginTalkMoveToTaggedPosition) {
            const float dx = npc.cycle->pos.x - tracked->cycle->pos.x;
            const float dy = npc.cycle->pos.y - tracked->cycle->pos.y;
            const float dz = npc.cycle->pos.z - tracked->cycle->pos.z;
            complete = dx * dx + dy * dy + dz * dz < npc.cycle->workFloat4C;
        }
    }

    if (!complete)
        return false;

    npc.cycle->trackedMobj = nullptr;
    npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
    ++cm.taggedWaitCompletions;
    if (timedOut)
        ++cm.taggedWaitTimeouts;
    return true;
}
} // namespace CycleManager


} // namespace shenmue::s1
