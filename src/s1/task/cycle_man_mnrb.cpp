#include "s1/task/cycle_man.hpp"
#include "s1/task/kdan.hpp"
#include "s1/motion.hpp"
#include "s1/collision.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/skinning_cache.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <cctype>
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/wexe.hpp"
#include "s1/mt5_types.hpp"
#include "s1/mt5_render.hpp"
#include "s1/face.hpp"
#include "s1/render_array.hpp"
#include "s1/player_control.hpp"
#include "s1/timing.hpp"
#include "s1/boot.hpp"
#include "s1/subtitles.hpp"
#include "s1/file_loader.hpp"

#include <cfloat>
#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
using namespace CycleManager;
namespace CycleManager {



CycleManager::MNRBEntry *FindMNRBEntry(uint32_t token) {
    if (!CycleManager::IsReady() || !token)
        return nullptr;
    for (CycleManager::MNRBEntry &entry : g_cycleManagerRuntime.mnrbEntries) {
        if (entry.name.name == token)
            return &entry;
    }
    return nullptr;
}
} // namespace CycleManager
 // namespace

namespace CycleManager {






uint32_t HandleTimedMorsApproach(CycleManager::MobjCycle *ctx, char *record, uint32_t live, int32_t time) {
    const int32_t *const word = reinterpret_cast<const int32_t *>(record);
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(static_cast<uint32_t>(word[1]));
    ctx->morsCommandOffset = CycleManager::FileOffset(record);
    ctx->morsEntryOffset = entry ? entry->lineDataOffset : 0;
    ctx->morsAuxOffsetOrState = 1;
    uint16_t animations[2] = {static_cast<uint16_t>(word[3]), static_cast<uint16_t>(word[4])};
    if (!animations[0])
        animations[0] = ctx->moveConfig->motionParamIds[0];
    if (!animations[1])
        animations[1] = ctx->moveConfig->motionParamIds[0];
    const int32_t deadline = word[2] <= 0 ? time - word[2] : word[2];
    if (time < static_cast<int32_t>(entry->activeTimeStart) || time > static_cast<int32_t>(entry->activeTimeEnd) || time > deadline) {
        ctx->morsCommandStatus = 0x3000F;
        return 1;
    }

    ctx->morsDeadlineTime = static_cast<uint32_t>(deadline);
    Vector3 spot{};
    if (!entry->ownerPointCount || !CycleManager::ReadMNRBVector(entry->ownerPointOffset, 0, spot)) {
        ctx->morsCommandStatus = 0x30002;
    } else {
        if (live)
            spot.y = MapProbe_UpdateIntersectionStateAtPoint({spot.x, ctx->pos.y, spot.z});
        Vector3 facing{};
        CycleManager::ReadMNRBVector(entry->ownerPointOffset, 1, facing);
        int16_t yaw = 0;
        if (entry->ownerPointCount <= 2) {
            yaw = static_cast<int16_t>(TruncateToInt32(facing.y * 8192.0f / 45.0f));
        } else {
            float dx = spot.x - facing.x;
            float dz = spot.z - facing.z;
            if (std::fabs(dx) < 0.00000011920929f)
                dx = 0.0f;
            if (std::fabs(dz) < 0.00000011920929f)
                dz = 0.0f;
            if (dx != 0.0f || dz != 0.0f)
                yaw = CycleManager::Atan2Fixed16(dx, dz);
        }
        const bool inReach = npcActivateIfNearInArea(*CycleManager::CycleRecord(ctx), 65536.0f, &spot, yaw, live != 0, ctx->moveConfig->motionParamIds[0]);
        ctx->morsCommandStatus = inReach ? 0x30001 : 0x30002;
    }

    if (static_cast<uint16_t>(ctx->morsCommandStatus) == 2) {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        const uint16_t animation = animations[(static_cast<uint32_t>(g_EnvPrngSeed) >> 16) & 1u];
        MotionInformationWork *const motion = ctx->mot;
        if (motion && ctx->obj) {
            if (ctx->activeAnimId == animation) {
                motion->playbackRate = ctx->moveConfig->motionRadiusTimingScalar;
                motion->motionFlags &= ~2u;
                motion->pendingAnimId = animation;
            } else {
                freeTalkConv_SetMoveAndFacingConstraint(
                    *ctx->obj, ctx->pos, ctx->desiredPathYaw, animation, 0.0f, ctx->moveConfig->motionRadiusTimingScalar, 0.0f, false);
            }
        }
        ctx->activeAnimId = animation;
    }
    return 0;
}
} // namespace CycleManager

namespace CycleManager {

CycleManager::MNRBEntry *FindMNRBEntryByLineOffset(uint32_t lineDataOffset) {
    for (CycleManager::MNRBEntry &entry : g_cycleManagerRuntime.mnrbEntries) {
        if (entry.lineDataOffset == lineDataOffset)
            return &entry;
    }
    return nullptr;
}

CycleManager::NpcRecord *FindNpcRecordByObjectId(uint32_t objectId) {
    for (CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.cycle->moveConfig->obj_id.name == objectId)
            return &npc;
    }
    return nullptr;
}

bool ReadMNRBVector(uint32_t tableOffset, uint32_t index, Vector3 &out) {
    if (!tableOffset)
        return false;
    const uint64_t offset = static_cast<uint64_t>(tableOffset) + static_cast<uint64_t>(index) * 12u;
    if (offset > g_cycleManagerRuntime.bytes.size() || sizeof(Vector3) > g_cycleManagerRuntime.bytes.size() - static_cast<size_t>(offset)) {
        return false;
    }
    return ReadValue(g_cycleManagerRuntime.bytes, static_cast<size_t>(offset), out);
}

CycleManager::MNRBEntry::Reservation *AllocSeatReservationNode() {
    if (g_cycleManagerRuntime.mnrbReservationPoolAvailable) {
        for (CycleManager::MNRBEntry::Reservation &node : g_cycleManagerRuntime.mnrbReservationPool) {
            if (!node.owner)
                return &node;
        }
    }
    return nullptr;
}


void RemoveSeatReservationNode(CycleManager::MNRBEntry *entry, CycleManager::MNRBEntry::Reservation *node) {
    if (!node || !entry || !entry->reservationCount)
        return;
    if (node->prev)
        node->prev->next = node->next;
    else
        entry->reservationHead = node->next;
    if (node->next)
        node->next->prev = node->prev;
    --entry->reservationCount;
    node->prev = nullptr;
    node->next = nullptr;
    node->owner = nullptr;
    node->seatOrderKey = 0;
}


void PruneMNRBReservations(CycleManager::MNRBEntry &entry) {
    CycleManager::MNRBEntry::Reservation *node = entry.reservationHead;
    while (node) {
        CycleManager::MNRBEntry::Reservation *const next = node->next;
        if (!node->owner || node->owner->cycle->currentOpcode != CycleOpcode_UseMnrBEntry)
            CycleManager::RemoveSeatReservationNode(&entry, node);
        node = next;
    }
}

void ReleaseMNRBReservation(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry) {
    CycleManager::MNRBEntry::Reservation *const node =
        npc.cycle->morsQueueNodeOffset ? &g_cycleManagerRuntime.mnrbReservationPool[npc.cycle->morsQueueNodeOffset - 1] : nullptr;
    CycleManager::RemoveSeatReservationNode(entry, node);
    npc.cycle->morsQueueNodeOffset = 0;
}

bool MNRBShutterGateOpen(const CycleManager::MNRBEntry &entry) {
    if (entry.ownerTokenCount < 2 || !entry.ownerTokenOffset)
        return true;
    uint32_t shutterToken = 0;
    if (!ReadValue(g_cycleManagerRuntime.bytes, entry.ownerTokenOffset + 4u, shutterToken))
        return false;
    for (const CycleManager::ShutRow &row : g_cycleManagerRuntime.shutRows) {
        if (row.token.name == shutterToken)
            return row.byte31 == 2;
    }
    return false;
}

int16_t Atan2ToFixed16(float x, float z) {
    return static_cast<int16_t>(atan2_fixed16(x, z));
}

bool shenFindBestFreeSeatIntervalRecursive(CycleManager::SeatInterval &interval, const CycleManager::MNRBEntry::Reservation *node, float selfHalfWidth,
    float requiredClearance, float axisDirX, float axisDirZ, float seatOriginX, float seatOriginZ) {
    if (!node) {
        const float length = interval.end - interval.start;
        if (interval.startBoundaryFree && interval.endBoundaryFree)
            return true;
        if (!interval.startBoundaryFree && !interval.endBoundaryFree)
            return length > requiredClearance + requiredClearance;
        return length > requiredClearance;
    }

    const CycleManager::MNRBEntry::Reservation &reservation = *node;
    CycleManager::NpcRecord *owner = reservation.owner;
    if (!owner)
        return false;

    const float halfWidth = std::max(owner->cycle->steeringHalfWidth + 0.2f, selfHalfWidth);
    float deltaX = reservation.seatPos.x - seatOriginX;
    float deltaZ = reservation.seatPos.z - seatOriginZ;
    if (std::fabs(deltaX) < 0.00000011920929f)
        deltaX = 0.0f;
    if (std::fabs(deltaZ) < 0.00000011920929f)
        deltaZ = 0.0f;
    const float projected = deltaZ * axisDirZ + deltaX * axisDirX;
    if (projected < interval.start || projected > interval.end)
        return false;

    CycleManager::SeatInterval left{interval.start, projected - halfWidth, interval.startBoundaryFree, interval.endBoundaryFree};

    CycleManager::SeatInterval right{projected + halfWidth, interval.end, false, interval.endBoundaryFree};
    const bool leftValid = left.end >= left.start &&
        shenFindBestFreeSeatIntervalRecursive(left, node->next, selfHalfWidth, requiredClearance, axisDirX, axisDirZ, seatOriginX, seatOriginZ);
    const bool rightValid = right.end >= right.start &&
        shenFindBestFreeSeatIntervalRecursive(right, node->next, selfHalfWidth, requiredClearance, axisDirX, axisDirZ, seatOriginX, seatOriginZ);
    if (leftValid && (!rightValid || left.end - left.start > right.end - right.start)) {
        interval = left;
        return true;
    }
    if (rightValid) {
        interval = right;
        return true;
    }
    return false;
}

bool FindSeatPositionAndFacingOnSeatLine(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry &entry, Vector3 &outPos, int16_t &outYaw) {
    Vector3 endpoint0{};
    Vector3 endpoint1{};
    if (entry.areaPointCount < 2 || !CycleManager::ReadMNRBVector(entry.areaPointOffset, 0, endpoint0) ||
        !CycleManager::ReadMNRBVector(entry.areaPointOffset, 1, endpoint1)) {
        return false;
    }

    float deltaX = endpoint0.x - endpoint1.x;
    float deltaZ = endpoint0.z - endpoint1.z;
    if (std::fabs(deltaX) < 0.00000011920929f)
        deltaX = 0.0f;
    if (std::fabs(deltaZ) < 0.00000011920929f)
        deltaZ = 0.0f;
    if (std::fabs(deltaX) < 0.00000011920929f && std::fabs(deltaZ) < 0.00000011920929f) {
        if (entry.reservationHead)
            return false;
        outPos = {endpoint0.x, npc.cycle->pos.y, endpoint0.z};
        outYaw = 0;
        return true;
    }

    const float length = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
    const float axisDirX = -deltaX / length;
    const float axisDirZ = -deltaZ / length;
    CycleManager::SeatInterval interval{0.0f, length, true, true};
    if (!shenFindBestFreeSeatIntervalRecursive(interval, entry.reservationHead, entry.radius, std::max(npc.cycle->steeringHalfWidth, entry.radius + 0.2f),
            axisDirX, axisDirZ, endpoint0.x, endpoint0.z)) {
        return false;
    }

    float distance = 0.0f;
    if (!interval.startBoundaryFree)
        distance = interval.endBoundaryFree ? interval.end : interval.start + (interval.end - interval.start) * 0.5f;
    else
        distance = interval.endBoundaryFree ? interval.start + (interval.end - interval.start) * 0.5f : interval.start;

    outPos = {endpoint0.x + axisDirX * distance, npc.cycle->pos.y, endpoint0.z + axisDirZ * distance};
    outYaw = static_cast<int16_t>(CycleManager::Atan2ToFixed16(deltaX, deltaZ) + 0x4000);
    return true;
}

bool MNRBMotionEnded(const CycleManager::NpcRecord &npc) {
    const MotionInformationWork *const motion = npc.cycle->mot;
    return !motion || (motion->motionFlags & 2u) != 0;
}

CycleManager::NpcRecord *GetMNRBLinkedOwner(const CycleManager::NpcRecord &npc) {
    if (!npc.morsLinkedOwnerObjectId)
        return nullptr;
    CycleManager::NpcRecord *owner = CycleManager::FindNpcRecordByObjectId(npc.morsLinkedOwnerObjectId);
    if (!owner || owner->cycle->currentOpcode != CycleOpcode_ScheduleMnrBEntry || static_cast<uint16_t>(owner->cycle->morsCommandStatus >> 16) != 0 ||
        static_cast<uint16_t>(owner->cycle->morsCommandStatus) != 3) {
        return nullptr;
    }
    return owner;
}

bool ReadKind0Point(const CycleManager::MNRBEntry &entry, uint32_t index, Vector3 &point) {
    return index < entry.areaPointCount && CycleManager::ReadMNRBVector(entry.areaPointOffset, index, point);
}

Vector3 TransformMNRBPointXZ(const CycleManager::NpcRecord &owner, const Vector3 &local, float y) {
    const float sine = sin_fixed16(static_cast<int16_t>(static_cast<uint16_t>(owner.cycle->desiredPathYaw)));
    const float cosine = cos_fixed16(static_cast<uint16_t>(owner.cycle->desiredPathYaw));
    return {owner.cycle->pos.x + (sine * local.z + cosine * local.x), y, owner.cycle->pos.z + (cosine * local.z - sine * local.x)};
}

bool SelectKind0RoutePoint(
    const CycleManager::NpcRecord &npc, const CycleManager::MNRBEntry &entry, const CycleManager::NpcRecord &owner, Vector3 &point, int16_t &yaw) {
    int32_t selector = 0;
    ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 8u, selector);
    uint32_t pointIndex = 5;
    if (selector < 0 && selector != -2 && selector >= 5 - static_cast<int32_t>(entry.areaPointCount)) {
        pointIndex = static_cast<uint32_t>(4 - selector);
    }
    Vector3 local{};
    if (!CycleManager::ReadKind0Point(entry, pointIndex, local))
        return false;
    point = CycleManager::TransformMNRBPointXZ(owner, local, npc.cycle->pos.y);
    yaw = static_cast<int16_t>(owner.cycle->desiredPathYaw - static_cast<int32_t>((local.y * -8192.0f) / 45.0f));
    return true;
}

bool MOVEOBJ(
    CycleManager::NpcRecord &npc, const CycleManager::MNRBEntry &entry, const CycleManager::MNRBEntry::Reservation &reservation, Vector3 &outPos, int16_t &outYaw) {
    CycleManager::NpcRecord *linkedOwner = CycleManager::GetMNRBLinkedOwner(npc);
    if (linkedOwner &&
        (linkedOwner->programList == CycleProgramList_Running ||
            static_cast<float>(static_cast<int32_t>(linkedOwner->cycle->selectedMotionToken)) / 500.0f <= 0.89999998f)) {
        linkedOwner = nullptr;
    }

    if (reservation.prev) {
        CycleManager::NpcRecord *previousOwner = reservation.prev->owner;
        bool usePrevious = previousOwner != nullptr;
        if (previousOwner && static_cast<uint16_t>(previousOwner->cycle->morsCommandStatus) == 6) {
            const int32_t frameCount = MOTN_getFrameCountFromAnimId(previousOwner->cycle->activeAnimId);
            const MotionInformationWork *const previousMotion = previousOwner->cycle->mot;
            usePrevious = !previousMotion || previousMotion->frameTimerRef < static_cast<float>(frameCount) * 0.5f;
        }
        if (usePrevious) {
            Vector3 line0{};
            Vector3 line1{};
            if (!CycleManager::ReadKind0Point(entry, 1, line0) || !CycleManager::ReadKind0Point(entry, 2, line1)) {
                return false;
            }
            const float lineDx = line1.x - line0.x;
            const float lineDz = line1.z - line0.z;
            const float lineLength = std::sqrt(lineDx * lineDx + lineDz * lineDz);
            const float axisX = (1.0f / lineLength) * lineDz;
            const float axisZ = (1.0f / lineLength) * lineDx;
            const float relX = previousOwner->cycle->moveTargetPos.x - line0.x;
            const float relZ = previousOwner->cycle->moveTargetPos.z - line0.z;
            float transverse = axisX * relX - axisZ * relZ;
            float longitudinal = axisX * relZ + axisZ * relX;
            if (std::fabs(transverse) < 0.01f)
                transverse = 0.0f;
            if (std::fabs(longitudinal) < 0.01f)
                longitudinal = 0.0f;
            const float clearance = std::max(previousOwner->cycle->steeringHalfWidth, entry.radius) + std::max(npc.cycle->steeringHalfWidth, entry.radius);
            const float radial = std::sqrt(transverse * transverse + longitudinal * longitudinal);
            float outTransverse = 0.0f;
            float outLongitudinal = 0.0f;
            if ((longitudinal >= 0.0f && std::fabs(transverse) < 0.00000011920929f) || radial <= clearance) {
                outLongitudinal = std::sqrt(clearance * clearance - transverse * transverse) + longitudinal;
            } else {
                const float inverse = 1.0f / radial;
                outTransverse = (transverse * inverse) * (radial - clearance);
                outLongitudinal = (longitudinal * inverse) * (radial - clearance);
            }
            outPos = {line0.x + axisZ * outLongitudinal + axisX * outTransverse, npc.cycle->pos.y, line0.z + axisX * outLongitudinal - axisZ * outTransverse};
            outYaw = CycleManager::Atan2Fixed16(outPos.x - previousOwner->cycle->moveTargetPos.x, outPos.z - previousOwner->cycle->moveTargetPos.z);
            const float dx = outPos.x - npc.cycle->pos.x;
            const float dz = outPos.z - npc.cycle->pos.z;
            return std::sqrt(dx * dx + dz * dz) > npc.cycle->steeringHalfWidth;
        }
    }

    Vector3 local{};
    if (linkedOwner && CycleManager::ReadKind0Point(entry, 3, local)) {
        outPos = CycleManager::TransformMNRBPointXZ(*linkedOwner, local, npc.cycle->pos.y);
        outYaw = static_cast<int16_t>(linkedOwner->cycle->desiredPathYaw + CycleManager::AngleDegreesToFixed16(local.y));
    } else if (CycleManager::ReadKind0Point(entry, 1, local)) {
        outPos = {local.x, npc.cycle->pos.y, local.z};
        outYaw = CycleManager::AngleDegreesToFixed16(local.y);
    } else {
        return false;
    }
    const float dx = outPos.x - npc.cycle->pos.x;
    const float dz = outPos.z - npc.cycle->pos.z;
    if (linkedOwner)
        return std::fabs(dx) >= 0.00000011920929f || std::fabs(dz) >= 0.00000011920929f;
    return std::sqrt(dx * dx + dz * dz) > npc.cycle->steeringHalfWidth;
}

bool ComputeMNRBReservationPoint(
    CycleManager::NpcRecord &npc, CycleManager::MNRBEntry &entry, const CycleManager::MNRBEntry::Reservation &reservation, Vector3 &outPos, int16_t &outYaw) {
    Vector3 endpoint0{};
    Vector3 endpoint1{};
    if (entry.areaPointCount < 2 || !CycleManager::ReadMNRBVector(entry.areaPointOffset, 0, endpoint0) ||
        !CycleManager::ReadMNRBVector(entry.areaPointOffset, 1, endpoint1)) {
        return false;
    }

    float x = endpoint0.x;
    float z = endpoint0.z;
    if (reservation.prev) {
        CycleManager::NpcRecord *previousOwner = reservation.prev->owner;
        if (previousOwner) {
            x = 0.0f;
            z = 0.0f;
            if (static_cast<uint16_t>(previousOwner->cycle->morsCommandStatus) < 4u) {
                const float dx = endpoint1.x - endpoint0.x;
                const float dz = endpoint1.z - endpoint0.z;
                const float inverseLength = 1.0f / std::sqrt(dx * dx + dz * dz);
                const float spacing = (std::max(npc.cycle->steeringHalfWidth, entry.radius) + previousOwner->cycle->steeringHalfWidth) + 0.2f;
                x = previousOwner->cycle->moveTargetPos.x + (inverseLength * dx) * spacing;
                z = previousOwner->cycle->moveTargetPos.z + (inverseLength * dz) * spacing;
            }
        }
    }

    const float width = std::max(npc.cycle->steeringHalfWidth, entry.radius);
    const float ownerDx = x - npc.cycle->pos.x;
    const float ownerDz = z - npc.cycle->pos.z;
    if (std::sqrt(ownerDx * ownerDx + ownerDz * ownerDz) <= width)
        return false;

    Vector3 ownerPoint{};
    if (!CycleManager::ReadMNRBVector(entry.ownerPointOffset, 0, ownerPoint))
        return false;
    outPos = {x, npc.cycle->pos.y, z};
    outYaw = CycleManager::Atan2ToFixed16(x - ownerPoint.x, z - ownerPoint.z);
    return true;
}


void SetMNRBAnimation(CycleManager::NpcRecord &npc, uint16_t animId) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (npc.cycle->motnSteeringActive && npc.cycle->obj && motion) {
        if (animId == npc.cycle->activeAnimId) {
            motion->motionFlags &= ~2u;
            motion->playbackRate = npc.cycle->moveConfig->motionRadiusTimingScalar;
            motion->pendingAnimId = animId;
        } else {
            freeTalkConv_SetMoveAndFacingConstraint(
                *npc.cycle->obj, npc.cycle->pos, npc.cycle->desiredPathYaw, animId, 0.0f, npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, false);
        }
    }
    npc.cycle->activeAnimId = animId;
}



void UpdateMNRBSoundGates(CycleManager::NpcRecord &npc, uint32_t gates) {
    const uint32_t area = CycleManager::CurrentAreaId();
    for (uint32_t bit = 1; bit <= 2; bit <<= 1) {
        if (npc.cycle->motnSteeringActive && ((npc.morsSoundGateMask ^ gates) & bit)) {
            uint32_t command = 0xffffffffu;
            if (area == SHEN_NAME(D000))
                command = gates & bit ? 0x2305a9u : 0x2405a9u;
            else if (area == SHEN_NAME(MFSY))
                command = gates & bit ? 0x5905a9u : 0x5a05a9u;
            if (command != 0xffffffffu)
                Execute3DSoundCommand(command, 4u, &npc.cycle->pos, 4.0f, 15.0f, 0xffffffffu);
        }
    }
    npc.morsSoundGateMask = gates;
}

void CheckBoundaryTriggerAndPlaySounds(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry &entry, uint32_t recordOffset) {
    npc.cycle->flags &= ~0x10u;
    npc.cycle->morsCommandOffset = recordOffset;
    npc.cycle->morsDeadlineTime = 0;
    npc.cycle->morsCommandArg = 0;

    int32_t triggerArg = 0;
    ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 8u, triggerArg);
    npc.cycle->scheduledTriggerTime = static_cast<uint32_t>(-triggerArg);

    uint32_t triggerMask = 0;
    Vector3 endpoint{};
    Vector3 ownerParams{};
    if (npc.cycle->scheduledTriggerTime && entry.areaPointCount > 1 && entry.ownerPointCount > 0 &&
        CycleManager::ReadMNRBVector(entry.areaPointOffset, 1, endpoint) && CycleManager::ReadMNRBVector(entry.ownerPointOffset, 0, ownerParams)) {
        Vector3 playerPos{};
        if (CycleManager::MapRyo())
            playerPos = CycleManager::CurrentRyoPosition();
        float dx = playerPos.x - endpoint.x;
        float dz = playerPos.z - endpoint.z;
        if (std::fabs(dx) < 0.00000011920929f)
            dx = 0.0f;
        if (std::fabs(dz) < 0.00000011920929f)
            dz = 0.0f;
        if (ownerParams.x > std::sqrt(dx * dx + dz * dz))
            triggerMask = 1;
    }

    for (const CycleManager::MNRBEntry::Reservation *reservation = entry.reservationHead; reservation; reservation = reservation->next) {
        CycleManager::NpcRecord *owner = reservation->owner;
        if (!owner || owner->cycle->currentOpcode != CycleOpcode_UseMnrBEntry || owner->cycle->morsEntryOffset != entry.lineDataOffset ||
            static_cast<uint16_t>(owner->cycle->morsCommandStatus) >= 13u) {
            continue;
        }

        int32_t selector = 0;
        ReadValue(g_cycleManagerRuntime.bytes, owner->cycle->morsCommandOffset + 8u, selector);
        triggerMask |= selector > 0 ? 1u : 2u;
        if (triggerMask == 3u)
            break;
    }

    if (triggerMask) {
        npc.cycle->morsCommandStatus = 3u;
        CycleManager::UpdateMNRBSoundGates(npc, triggerMask);
    } else {
        npc.cycle->morsCommandStatus = 15u;
    }
}





uint32_t HandleMorsType0Command(CycleManager::NpcRecord &npc, char *record, uint32_t live) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    if (ctx->areaID.name != CycleManager::CurrentAreaId())
        return 1;
    ctx->morsCommandStatus = 0;
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    ctx->morsCommandOffset = CycleManager::FileOffset(record);
    ctx->morsEntryOffset = entry ? entry->lineDataOffset : 0;
    ctx->morsQueueNodeOffset = 0;
    npc.morsLinkedOwnerObjectId = 0;
    if (entry && entry->ownerTokenCount)
        ReadValue(g_cycleManagerRuntime.bytes, entry->ownerTokenOffset, npc.morsLinkedOwnerObjectId);
    if (*reinterpret_cast<const int32_t *>(record + 8) <= 0) {
        ctx->morsCommandArg = ctx->modeValue;
        ctx->modeValue = 0;
        return CycleManager::TickMnrbBusStopUse(npc, entry, live) ? 1u : 0u;
    }
    ctx->modeValue = 0;
    CycleManager::StartDoorFacingHandoff(npc);
    return 0;
}



uint32_t HandleMorsType1Command(CycleManager::NpcRecord &npc, char *record) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    ctx->morsCommandOffset = CycleManager::FileOffset(record);
    ctx->morsQueueNodeOffset = 0;
    ctx->field_128 = 0;
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    ctx->morsEntryOffset = entry ? entry->lineDataOffset : 0;
    ctx->morsAuxOffsetOrState = static_cast<int32_t>(ctx->modeValue);
    ctx->morsCommandStatus = 0x10000u;
    if (ctx->modeValue) {
        CycleManager::StartDoorFacingHandoff(npc);
        ctx->modeValue = 0;
    }
    return 0;
}





uint32_t QueueChairGroupActivation(CycleManager::NpcRecord &npc, char *record, uint32_t live, int32_t time) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    ctx->morsCommandOffset = CycleManager::FileOffset(record);
    CycleManager::MNRBEntry *const group = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    int32_t seatOrderKey = *reinterpret_cast<const int32_t *>(record + 12);
    if (seatOrderKey <= 0)
        seatOrderKey = time - seatOrderKey;
    if (time >= seatOrderKey || group->reservationCount >= *reinterpret_cast<const uint32_t *>(record + 8) || !CycleManager::MNRBShutterGateOpen(*group))
        return 1;
    CycleManager::MNRBEntry::Reservation *const node = CycleManager::AllocSeatReservationNode();
    if (!node)
        std::abort();
    ctx->morsQueueNodeOffset = static_cast<uint32_t>(node - g_cycleManagerRuntime.mnrbReservationPool.data()) + 1;
    ++group->reservationCount;
    node->owner = &npc;
    node->next = nullptr;
    node->seatOrderKey = seatOrderKey;
    node->seatPos.x = *reinterpret_cast<const float *>(record + 16);
    node->seatPos.z = *reinterpret_cast<const float *>(record + 24);
    CycleManager::MNRBEntry::Reservation *last = group->reservationHead;
    if (last) {
        while (last->next)
            last = last->next;
        last->next = node;
    } else {
        group->reservationHead = node;
    }
    node->prev = last;
    Vector3 target{};
    int16_t yaw = 0;
    const bool placed = CycleManager::ComputeMNRBReservationPoint(npc, *group, *node, target, yaw);
    target.y = ctx->pos.y;
    if (live)
        target.y = MapProbe_UpdateIntersectionStateAtPoint(target);
    const bool walking = placed && npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, ctx->moveConfig->motionParamIds[0]);
    ctx->morsCommandStatus = walking ? 0x30001u : 0x30002u;
    return 0;
}




uint32_t BeginUseMnrbEntry(CycleManager::NpcRecord &npc, char *record, uint32_t live, int32_t time) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    ctx->morsEntryOffset = entry ? entry->lineDataOffset : 0;
    ctx->morsCommandOffset = 0;
    ctx->morsQueueNodeOffset = 0;
    if (entry)
        CycleManager::PruneMNRBReservations(*entry);
    uint32_t skipped = 1;
    if (!entry) {
        ctx->morsCommandStatus = 0;
    } else {
        ctx->morsCommandStatus = entry->entryKind << 16;
        if (entry->entryKind == 0)
            skipped = CycleManager::HandleMorsType0Command(npc, record, live);
        else if (entry->entryKind == 1)
            skipped = CycleManager::HandleMorsType1Command(npc, record);
        else if (entry->entryKind == 3)
            skipped = CycleManager::QueueChairGroupActivation(npc, record, live, time);
    }
    if (skipped)
        ctx->currentOpcode = CycleOpcode_AdvanceScript;
    return 56;
}





bool TickMnrbCounterUse(CycleManager::NpcRecord &npc, uint32_t live, int32_t time) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    Character *const obj = live ? ctx->obj : nullptr;
    MotionInformationWork *const mot = live ? npc.cycle->mot : nullptr;
    char *const record = CycleManager::FilePointer(ctx->morsCommandOffset);
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntryByLineOffset(ctx->morsEntryOffset);
    if (!entry)
        return true;
    const bool unblocked = !obj || !charaHasBlockingTPMRequest(obj);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    uint16_t servedAnim = *reinterpret_cast<const uint16_t *>(record + 36);
    uint16_t waitAnim = *reinterpret_cast<const uint16_t *>(record + 28 + ((g_EnvPrngSeed & 0x10000u) ? 4 : 0));
    if (!waitAnim)
        waitAnim = ctx->moveConfig->motionParamIds[0];
    if (!servedAnim)
        servedAnim = ctx->moveConfig->motionParamIds[0];
    CycleManager::MNRBEntry::Reservation *const node =
        ctx->morsQueueNodeOffset ? &g_cycleManagerRuntime.mnrbReservationPool[ctx->morsQueueNodeOffset - 1] : nullptr;
    uint16_t state = static_cast<uint16_t>(ctx->morsCommandStatus);
    bool done = false;
    switch (state) {
    case 0: {
        if (!unblocked)
            break;
        int32_t deadline = *reinterpret_cast<const int32_t *>(record + 12);
        if (deadline < 0)
            deadline = time - deadline;
        Vector3 target{};
        int16_t yaw = 0;
        if (deadline <= time || !CycleManager::MNRBShutterGateOpen(*entry) || entry->reservationCount >= *reinterpret_cast<const uint32_t *>(record + 8) ||
            !FindSeatPositionAndFacingOnSeatLine(npc, *entry, target, yaw)) {
            state = 4;
            break;
        }
        target.y = mapUpdateIntersectionStateAtLocalPoint({target.x, ctx->pos.y, target.z});
        CycleManager::MNRBEntry::Reservation *const added = CycleManager::AllocSeatReservationNode();
        if (!added)
            std::abort();
        ++entry->reservationCount;
        CycleManager::MNRBEntry::Reservation *last = entry->reservationHead;
        if (last) {
            while (last->next)
                last = last->next;
            last->next = added;
        } else {
            entry->reservationHead = added;
        }
        added->prev = last;
        added->seatPos = target;
        added->next = nullptr;
        added->owner = &npc;
        added->seatOrderKey = deadline;
        ctx->morsQueueNodeOffset = static_cast<uint32_t>(added - g_cycleManagerRuntime.mnrbReservationPool.data()) + 1;
        state = npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, waitAnim) ? 1 : 2;
        break;
    }
    case 1:
        if (!CycleManager::TickPositionAndHeadingMove(npc)) {
            state = 2;
        } else if (node && node->seatOrderKey < time) {
            UpdateEntityOverPositionWithFlags(npc, true);
            state = 4;
        } else if (ctx->workFloat4C < 0.0f) {
            state = 4;
        }
        break;
    case 2: {
        if (!node || (!ctx->field_128 && node->seatOrderKey < time)) {
            state = 4;
            break;
        }
        uint16_t anim = servedAnim;
        if (unblocked && ctx->field_128 == 2)
            state = 3;
        else
            anim = CycleManager::MNRBMotionEnded(npc) ? waitAnim : 0;
        if (obj)
            CycleManager::EnableRootMotion(obj, true);
        if (!anim)
            break;
        if (obj && mot) {
            if (ctx->activeAnimId != anim) {
                freeTalkConv_SetMoveAndFacingConstraint(
                    *obj, ctx->pos, ctx->desiredPathYaw, anim, 0.0f, ctx->moveConfig->motionRadiusTimingScalar, 0.0f, false);
            } else {
                mot->motionFlags &= ~2u;
                mot->playbackRate = ctx->moveConfig->motionRadiusTimingScalar;
                mot->pendingAnimId = anim;
            }
        }
        ctx->activeAnimId = anim;
        break;
    }
    case 3:
        if (!CycleManager::MNRBMotionEnded(npc))
            break;
        ctx->field_128 = 0;
        state = 4;
        break;
    case 4:
        CycleManager::ReleaseMNRBReservation(npc, entry);
        done = unblocked;
        ctx->modeValue = static_cast<uint32_t>(ctx->morsAuxOffsetOrState);
        break;
    default:
        break;
    }
    ctx->morsCommandStatus = 0x10000u | state;
    return done;
}






bool TickMnrbCounterOwner(CycleManager::NpcRecord &npc, uint32_t live, int32_t time) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    Character *const obj = live ? ctx->obj : nullptr;
    MotionInformationWork *const mot = live ? npc.cycle->mot : nullptr;
    ctx->selectedMotionToken = ctx->moveConfig->motionParamIds[3];
    char *const record = CycleManager::FilePointer(ctx->morsCommandOffset);
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(*reinterpret_cast<const uint32_t *>(record + 4));
    const int32_t deadline = static_cast<int32_t>(ctx->morsDeadlineTime);
    if (!CycleManager::MNRBMotionEnded(npc))
        return false;
    if (!entry)
        return true;
    const bool unblocked = !obj || !charaHasBlockingTPMRequest(obj);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const uint32_t seed = g_EnvPrngSeed;
    uint16_t serveAnim = *reinterpret_cast<const uint16_t *>(record + 20);
    uint16_t waitAnim = *reinterpret_cast<const uint16_t *>(record + 12 + ((seed & 0x10000u) ? 4 : 0));
    if (!waitAnim)
        waitAnim = ctx->moveConfig->motionParamIds[0];
    if (!serveAnim)
        serveAnim = ctx->moveConfig->motionParamIds[0];
    Vector3 start{};
    Vector3 end{};
    CycleManager::ReadMNRBVector(entry->ownerPointOffset, 0, start);
    CycleManager::ReadMNRBVector(entry->ownerPointOffset, 1, end);
    float dx = end.x - start.x;
    float dz = end.z - start.z;
    if (std::fabs(dx) < 0.00000011920929f)
        dx = 0.0f;
    if (std::fabs(dz) < 0.00000011920929f)
        dz = 0.0f;
    float length = std::sqrt(dz * dz + dx * dx);
    float axisX = 0.0f;
    float axisZ = 0.0f;
    int16_t yaw = 0;
    if (std::fabs(length) >= 0.00000011920929f) {
        yaw = static_cast<int16_t>(CycleManager::Atan2ToFixed16(-dx, -dz) + 0x4000);
        axisX = dx * (1.0f / length);
        axisZ = dz * (1.0f / length);
    } else {
        Vector3 endpoint{};
        CycleManager::ReadMNRBVector(entry->areaPointOffset, 0, endpoint);
        float ex = start.x - endpoint.x;
        float ez = start.z - endpoint.z;
        if (std::fabs(ex) < 0.00000011920929f)
            ex = 0.0f;
        if (std::fabs(ez) < 0.00000011920929f)
            ez = 0.0f;
        yaw = CycleManager::Atan2ToFixed16(ex, ez);
        length = 0.0f;
    }
    CycleManager::MNRBEntry::Reservation *waiting = entry->reservationHead;
    while (waiting && static_cast<uint16_t>(waiting->owner->cycle->morsCommandStatus) != 2)
        waiting = waiting->next;
    uint16_t state = static_cast<uint16_t>(ctx->morsCommandStatus);
    bool done = false;
    switch (state) {
    case 0: {
        if (!unblocked)
            break;
        int32_t until = *reinterpret_cast<const int32_t *>(record + 8);
        if (until < 0)
            until = time - until;
        ctx->morsQueueNodeOffset = 0;
        npc.morsLinkedOwnerObjectId = 0;
        ctx->morsDeadlineTime = static_cast<uint32_t>(until);
        if ((until <= time || static_cast<int32_t>(entry->activeTimeStart) > time || static_cast<int32_t>(entry->activeTimeEnd) < time) &&
            !entry->reservationHead) {
            state = 15;
            break;
        }
        Vector3 target{(length * 0.5f) * axisX + start.x, ctx->pos.y, (length * 0.5f) * axisZ + start.z};
        target.y = mapUpdateIntersectionStateAtLocalPoint(target);
        state = npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, waitAnim) ? 1 : 2;
        break;
    }
    case 1:
    case 3:
        if (!CycleManager::TickPositionAndHeadingMove(npc))
            state = 2;
        break;
    case 2:
        if (obj) {
            ctx->pos = GetPositionForEntity(obj->entity.node);
            ctx->desiredPathYaw = CycleManager::NodeChainYaw(obj);
            CycleManager::EnableRootMotion(obj, true);
        }
        if (!unblocked)
            break;
        if (waiting) {
            CycleManager::NpcRecord &client = *waiting->owner;
            Vector3 target{waiting->seatPos.x, ctx->pos.y, waiting->seatPos.z};
            client.cycle->field_128 = 1;
            npc.morsLinkedOwnerObjectId = client.cycle->moveConfig->obj_id.name;
            float relativeX = target.x - start.x;
            float relativeZ = target.z - start.z;
            if (std::fabs(relativeX) < 0.00000011920929f)
                relativeX = 0.0f;
            if (std::fabs(relativeZ) < 0.00000011920929f)
                relativeZ = 0.0f;
            const float segment = std::sqrt((end.z - start.z) * (end.z - start.z) + (end.x - start.x) * (end.x - start.x));
            if (std::fabs(segment) >= 0.00000011920929f) {
                const float stepX = (1.0f / segment) * (end.x - start.x);
                const float stepZ = (1.0f / segment) * (end.z - start.z);
                const float projection = stepZ * relativeZ + stepX * relativeX;
                if (projection < 0.0f) {
                    target.x = start.x;
                    target.z = start.z;
                } else if (projection <= segment) {
                    target.x = projection * stepX + start.x;
                    target.z = projection * stepZ + start.z;
                } else {
                    target.x = end.x;
                    target.z = end.z;
                }
            } else {
                target.x = start.x;
                target.z = start.z;
            }
            float faceX = target.x - waiting->seatPos.x;
            float faceZ = target.z - waiting->seatPos.z;
            if (std::fabs(faceX) < 0.00000011920929f)
                faceX = 0.0f;
            if (std::fabs(faceZ) < 0.00000011920929f)
                faceZ = 0.0f;
            const int16_t facing = std::fabs(faceX) >= 0.00000011920929f || std::fabs(faceZ) >= 0.00000011920929f ? CycleManager::Atan2ToFixed16(faceX, faceZ) :
                                                                                                                    ctx->desiredPathYaw;
            if (npcActivateIfNearInArea(npc, 65536.0f, &target, facing, live, serveAnim)) {
                state = 4;
            } else {
                client.cycle->field_128 = 2;
                state = 5;
            }
            break;
        }
        if (deadline < time) {
            state = 15;
            break;
        }
        {
            const uint32_t pace = LcgStep(seed);
            const float distance = length * (static_cast<float>((pace >> 16) & 0x7fffu) * 0.000030517578125f);
            Vector3 target{distance * axisX + start.x, ctx->pos.y, distance * axisZ + start.z};
            float fromX = ctx->pos.x - start.x;
            float fromZ = ctx->pos.z - start.z;
            if (std::fabs(fromX) < 0.00000011920929f)
                fromX = 0.0f;
            if (std::fabs(fromZ) < 0.00000011920929f)
                fromZ = 0.0f;
            const float difference = distance - std::sqrt(fromZ * fromZ + fromX * fromX);
            g_EnvPrngSeed = LcgStep(pace);
            bool pacing = false;
            if ((g_EnvPrngSeed & 0x10000u) && live == 1 && (difference > 0.89999998f || difference < -0.89999998f)) {
                target.y = mapUpdateIntersectionStateAtLocalPoint(target);
                pacing = npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, true, waitAnim);
            }
            if (pacing) {
                state = 3;
            } else if (waitAnim) {
                if (obj && mot) {
                    if (waitAnim != ctx->activeAnimId) {
                        freeTalkConv_SetMoveAndFacingConstraint(
                            *obj, ctx->pos, ctx->desiredPathYaw, waitAnim, 0.0f, ctx->moveConfig->motionRadiusTimingScalar, 0.0f, false);
                        ctx->activeAnimId = waitAnim;
                        break;
                    }
                    mot->motionFlags &= ~2u;
                    mot->playbackRate = ctx->moveConfig->motionRadiusTimingScalar;
                    mot->pendingAnimId = waitAnim;
                }
                ctx->activeAnimId = waitAnim;
            }
        }
        break;
    case 4:
    case 5:
        if (state == 4) {
            if (CycleManager::TickPositionAndHeadingMove(npc))
                break;
            CycleManager::NpcRecord *const client = CycleManager::FindNpcRecordByObjectId(npc.morsLinkedOwnerObjectId);
            npc.morsLinkedOwnerObjectId = 0;
            ctx->morsQueueNodeOffset = 0;
            if (client) {
                client->cycle->field_128 = 2;
                state = 5;
                break;
            }
        }
        if (obj && mot) {
            if (waitAnim == mot->currentAnimId) {
                mot->motionFlags &= ~2u;
                mot->playbackRate = ctx->moveConfig->motionRadiusTimingScalar;
                mot->pendingAnimId = waitAnim;
            } else {
                freeTalkConv_SetMoveAndFacingConstraint(
                    *obj, ctx->pos, ctx->desiredPathYaw, waitAnim, 0.0f, ctx->moveConfig->motionRadiusTimingScalar, 0.0f, false);
            }
        }
        ctx->activeAnimId = waitAnim;
        state = 2;
        break;
    case 15:
        if (unblocked) {
            if (waiting)
                state = 2;
            else
                done = true;
        }
        break;
    default:
        break;
    }
    ctx->morsCommandStatus = 0x10000u | state;
    return done;
}




bool TickMnrbBusStopOwner(CycleManager::NpcRecord &npc) {
    npc.cycle->flags &= ~0x10u;
    CycleManager::MNRBEntry *const found = CycleManager::FindMNRBEntryByLineOffset(npc.cycle->morsEntryOffset);
    if (!found || static_cast<uint16_t>(npc.cycle->morsCommandStatus) != 3)
        return true;
    CycleManager::MNRBEntry &entry = *found;
    const int32_t now = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
    uint32_t gates = 0;
    for (const CycleManager::MNRBEntry::Reservation *reservation = entry.reservationHead; reservation; reservation = reservation->next) {
        CycleManager::NpcRecord *client = reservation->owner;
        if (client && client->cycle->currentOpcode == CycleOpcode_UseMnrBEntry && static_cast<uint16_t>(client->cycle->morsCommandStatus) < 13) {
            int32_t selector = 0;
            ReadValue(g_cycleManagerRuntime.bytes, client->cycle->morsCommandOffset + 8u, selector);
            gates |= selector > 0 ? 1u : 2u;
        }
        if (gates == 3)
            break;
    }
    Vector3 endpoint{};
    Vector3 parameters{};
    CycleManager::ReadMNRBVector(entry.areaPointOffset, 1, endpoint);
    CycleManager::ReadMNRBVector(entry.ownerPointOffset, 0, parameters);
    const Vector3 player = CycleManager::CurrentRyoPosition();
    const float dx = player.x - endpoint.x;
    const float dz = player.z - endpoint.z;

    const float distance = std::fabs(dx) >= 0.00000011920929f ? (std::fabs(dz) >= 0.00000011920929f ? std::sqrt(dx * dx + dz * dz) : dx) :
                                                                (std::fabs(dz) >= 0.00000011920929f ? dz : 0.0f);
    const int32_t trigger = static_cast<int32_t>(npc.cycle->scheduledTriggerTime);
    if (!(gates & 1u)) {
        if (parameters.x > distance) {
            if (trigger < 0 || trigger > now)
                gates |= 1u;
            if (trigger < 0)
                npc.cycle->scheduledTriggerTime = static_cast<uint32_t>(now - trigger);
        }
    } else if (trigger > 0) {
        npc.cycle->scheduledTriggerTime = static_cast<uint32_t>(now - trigger);
    }
    npc.cycle->morsDeadlineTime = parameters.x <= distance || static_cast<float>(static_cast<int32_t>(npc.cycle->selectedMotionToken)) / 500.0f <= 0.2f ?
        0 :
        npc.cycle->scheduledTriggerTime;
    CycleManager::UpdateMNRBSoundGates(npc, (gates & 2u) | ((gates & 1u) || npc.cycle->morsCommandArg ? 1u : 0u));
    return gates == 0 && npc.cycle->selectedMotionToken == 0 && npc.field148 == 0;
}




bool TickMnrbQueueOwner(CycleManager::NpcRecord &npc, uint32_t live) {
    CycleManager::MNRBEntry *const found = CycleManager::FindMNRBEntryByLineOffset(npc.cycle->morsEntryOffset);
    if (!found)
        return true;
    CycleManager::MNRBEntry &entry = *found;
    const int32_t now = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
    uint16_t state = static_cast<uint16_t>(npc.cycle->morsCommandStatus);
    Character *const character = live ? npc.cycle->obj : nullptr;
    MotionInformationWork *const motion = live ? npc.cycle->mot : nullptr;
    const bool unblocked = !character || !charaHasBlockingTPMRequest(character);
    npc.cycle->selectedMotionToken = npc.cycle->moveConfig->motionParamIds[3];
    uint16_t animations[3]{};
    for (uint32_t index = 0; index < 3; ++index) {
        ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x0Cu + index * 4u, animations[index]);
        if (!animations[index])
            animations[index] = npc.cycle->moveConfig->motionParamIds[0];
    }
    uint16_t animation = 0;
    bool animate = false;
    if (state == 1 && !CycleManager::TickPositionAndHeadingMove(npc)) {
        if (static_cast<int32_t>(entry.ownerPointCount) - 1 <= npc.cycle->morsAuxOffsetOrState) {
            state = 2;
        } else {
            Vector3 target{};
            Vector3 next{};
            CycleManager::ReadMNRBVector(entry.ownerPointOffset, static_cast<uint32_t>(npc.cycle->morsAuxOffsetOrState), target);
            ++npc.cycle->morsAuxOffsetOrState;
            CycleManager::ReadMNRBVector(entry.ownerPointOffset, static_cast<uint32_t>(npc.cycle->morsAuxOffsetOrState), next);
            const int16_t yaw = CycleManager::Atan2ToFixed16(target.x - next.x, target.z - next.z);
            target.y = npc.cycle->pos.y;
            if (!npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, npc.cycle->moveConfig->motionParamIds[0]))
                state = 2;
        }
        if (state == 2) {
            if (character && motion)
                CycleManager::EnableRootMotion(character, true);
            animate = true;
        }
    } else if (state == 2) {
        npc.cycle->flags &= ~0x10u;
        CycleManager::NpcRecord *first = entry.reservationHead ? entry.reservationHead->owner : nullptr;
        if (first && static_cast<uint16_t>(first->cycle->morsCommandStatus) >= 2) {
            state = 3;
            animation = animations[2];
            animate = true;
        } else if (CycleManager::MNRBMotionEnded(npc)) {
            if (static_cast<int32_t>(npc.cycle->morsDeadlineTime) < now && !entry.reservationHead)
                state = 15;
            else
                animate = true;
        }
    } else if (state == 3) {
        npc.cycle->flags &= ~0x10u;
        if (CycleManager::MNRBMotionEnded(npc))
            state = 2;
    }
    if (animate) {
        if (state != 3) {
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            animation = animations[(g_EnvPrngSeed >> 16) & 1u];
        }
        if (character && motion && npc.cycle->activeAnimId != animation) {
            Vector3 target{};
            CycleManager::ReadMNRBVector(entry.ownerPointOffset, entry.ownerPointCount - 2u, target);
            target.y = MapProbe_UpdateIntersectionStateAtPoint({target.x, npc.cycle->pos.y, target.z});
            freeTalkConv_SetMoveAndFacingConstraint(
                *character, target, npc.cycle->desiredPathYaw, animation, 0.0f, npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, false);
            npc.cycle->activeAnimId = animation;
        } else {
            CycleManager::SetMNRBAnimation(npc, animation);
        }
    }
    npc.cycle->morsCommandStatus = 0x30000u | state;
    return state == 15 && unblocked;
}






bool TickMnrbBusStopUse(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry, uint32_t live) {
    uint16_t state = static_cast<uint16_t>(npc.cycle->morsCommandStatus);
    const int32_t now = static_cast<int32_t>(CycleManager::CurrentTimeSeconds());
    Character *const character = live ? npc.cycle->obj : nullptr;
    MotionInformationWork *const motion = live ? npc.cycle->mot : nullptr;
    const bool unblocked = !character || !charaHasBlockingTPMRequest(character);
    npc.cycle->flags &= 0xFFFFFFDCu;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    uint16_t waitAnim = 0;
    uint16_t finalAnim = 0;
    uint16_t routeAnim = 0;
    ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x1Cu + ((g_EnvPrngSeed >> 16) & 1u) * 4u, waitAnim);
    ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x24u, finalAnim);
    ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x1Cu, routeAnim);
    int32_t selector = 0;
    ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 8u, selector);
    CycleManager::NpcRecord *owner = CycleManager::GetMNRBLinkedOwner(npc);
    const bool ownerRunning = owner && owner->programList == CycleProgramList_Running;
    if (ownerRunning ||
        (owner &&
            (selector > 0 ? static_cast<float>(static_cast<int32_t>(owner->cycle->selectedMotionToken)) / 500.0f <= 0.5f :
                            static_cast<float>(static_cast<int32_t>(owner->field148)) / 500.0f <= 0.89999998f))) {
        owner = nullptr;
    }
    Vector3 exitDelta{};
    Vector3 terminal{};
    CycleManager::ReadKind0Point(*entry, 0, exitDelta);
    CycleManager::ReadKind0Point(*entry, 6, terminal);
    Vector3 ownerTarget{65536.0f, npc.cycle->pos.y, 65536.0f};
    Vector3 routeOrigin{65536.0f, npc.cycle->pos.y, 65536.0f};
    int16_t routeYaw = 0;
    if (owner) {
        Vector3 local{};
        if (CycleManager::ReadKind0Point(*entry, 3, local))
            ownerTarget = CycleManager::TransformMNRBPointXZ(*owner, local, npc.cycle->pos.y);
        CycleManager::SelectKind0RoutePoint(npc, *entry, *owner, routeOrigin, routeYaw);
    }
    CycleManager::MNRBEntry::Reservation *reservation =
        npc.cycle->morsQueueNodeOffset ? &g_cycleManagerRuntime.mnrbReservationPool[npc.cycle->morsQueueNodeOffset - 1] : nullptr;
    if (ownerRunning && unblocked) {
        UpdateEntityOverPositionWithFlags(npc, false);
        ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x10u, npc.cycle->pos);
        npc.cycle->desiredPathYaw = 0;
        state = 15;
    }
    const uint16_t previousState = state;
    switch (state) {
    case 0:
        if (!unblocked)
            break;
        if (owner && owner->cycle->morsCommandArg) {
            state = selector > 0 ? 13 : 15;
            break;
        } else {
            int32_t deadline = 0;
            ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x0Cu, deadline);
            if (deadline < 0)
                deadline = now - deadline;
            reservation = CycleManager::AllocSeatReservationNode();
            if (!reservation)
                std::abort();
            reservation->owner = &npc;
            reservation->seatOrderKey = deadline;
            reservation->seatPos = selector > 0 ? npc.cycle->pos : Vector3{};
            reservation->prev = nullptr;
            reservation->next = entry->reservationHead;
            while (reservation->next) {
                int32_t nextSelector = 0;
                ReadValue(g_cycleManagerRuntime.bytes, reservation->next->owner->cycle->morsCommandOffset + 8u, nextSelector);
                if (selector > 0 && nextSelector <= 0)
                    break;
                reservation->prev = reservation->next;
                reservation->next = reservation->next->next;
            }
            if (reservation->prev)
                reservation->prev->next = reservation;
            else
                entry->reservationHead = reservation;
            if (reservation->next)
                reservation->next->prev = reservation;
            ++entry->reservationCount;
            npc.cycle->morsQueueNodeOffset = static_cast<uint32_t>(reservation - g_cycleManagerRuntime.mnrbReservationPool.data()) + 1;
            state = selector > 0 ? 1 : 8;
            if (selector <= 0)
                npc.cycle->scheduledTriggerTime = 500;
        }
        break;
    case 1:
        if (!CycleManager::TickPositionAndHeadingMove(npc)) {
            state = CycleManager::DistanceXZWithEpsilon(npc.cycle->pos, ownerTarget) < 0.1f ? 6 : 2;
        } else if (unblocked && npc.cycle->workFloat4C < 0.0f) {
            if (owner) {
                npc.cycle->workFloat4C = 1.0f;
            } else {
                UpdateEntityOverPositionWithFlags(npc, false);
                float dx = npc.cycle->pos.x - npc.cycle->moveTargetPos.x;
                float dz = npc.cycle->pos.z - npc.cycle->moveTargetPos.z;
                if (std::fabs(dx) < 0.00000011920929f)
                    dx = 0.0f;
                if (std::fabs(dz) < 0.00000011920929f)
                    dz = 0.0f;
                exitDelta.x -= dx;
                exitDelta.z -= dz;
                state = 13;
            }
        }
        break;
    case 2:
        npc.cycle->flags &= ~0x10u;
        if (unblocked) {
            CycleManager::EnableRootMotion(character, true);
            state = reservation && reservation->seatOrderKey > now ? 1 : 13;
        }
        break;
    case 3:
    case 4:
    case 5:
    case 7:
    case 10:
    case 12:
        break;
    case 6: {
        const float length = static_cast<float>(MOTN_getFrameCountFromAnimId(npc.cycle->activeAnimId));
        float progress = length;
        if (character && motion && length > 127.0f) {
            progress = MOTI_GetPlayRateScaledRemain(*motion);
            float fraction = 0.1f;
            if (std::fabs(length - 127.0f) >= 0.00000011920929f) {
                fraction = (progress - 127.0f) / (length - 127.0f);
                if (std::fabs(fraction) < 0.00000011920929f)
                    fraction = 0.0f;
            }
            float opacity = character->renderOpacity;
            if (1.0f - fraction <= opacity)
                opacity = 1.0f - fraction;
            character->renderOpacity = std::fmax(0.0f, opacity);
        }
        if (progress > length * 0.60000002f && reservation) {
            CycleManager::ReleaseMNRBReservation(npc, entry);
            reservation = nullptr;
        }
        if (!motion || (motion->motionFlags & 2u)) {
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            npc.cycle->pos = {1000.0f, 0.0f, 1000.0f};
            state = 15;
        }
        break;
    }
    case 8: {
        bool admitted = owner && reservation && reservation->seatOrderKey > now;
        if (admitted && reservation->prev) {
            CycleManager::NpcRecord *previousOwner = reservation->prev->owner;
            int32_t previousSelector = 0;
            if (previousOwner)
                ReadValue(g_cycleManagerRuntime.bytes, previousOwner->cycle->morsCommandOffset + 8u, previousSelector);
            admitted = !previousOwner || previousSelector > 0 || static_cast<uint16_t>(previousOwner->cycle->morsCommandStatus) >= 11;
        }
        if (admitted) {
            npc.cycle->pos.x = routeOrigin.x;
            npc.cycle->pos.z = routeOrigin.z;
            npc.cycle->desiredPathYaw = routeYaw;
            if (live)
                npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
            npc.cycle->activeAnimId = routeAnim;
            npc.morsMotionCursor = 0.0f;
            npc.cycle->requestAnimId = 0;
            state = 9;
        } else if (!reservation || reservation->seatOrderKey <= now) {
            npc.cycle->pos = {terminal.x, 0.0f, terminal.z};
            npc.cycle->desiredPathYaw = CycleManager::AngleDegreesToFixed16(terminal.y);
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            state = 15;
        }
        break;
    }
    case 9: {
        const float scalar =
            std::fabs(npc.cycle->moveConfig->motionRadiusTimingScalar) >= 0.00000011920929f ? npc.cycle->moveConfig->motionRadiusTimingScalar : 1.0f;
        npc.morsMotionCursor += (scalar * 0.5f) * static_cast<float>(Shenmue_nNumRenderedFrames);
        npc.cycle->requestAnimId = std::bit_cast<uint32_t>(npc.morsMotionCursor);
        const float length = static_cast<float>(MOTN_getFrameCountFromAnimId(npc.cycle->activeAnimId));
        Vector3 root{};
        g_motnHasYValue = 0;
        MOTN_ReadRootPositionAtFrame(routeAnim, npc.cycle->moveConfig->anchorGroupCode, std::fmin(length, npc.morsMotionCursor), root, false);
        const float sine = sin_fixed16(static_cast<int16_t>(static_cast<uint16_t>(npc.cycle->desiredPathYaw)));
        const float cosine = cos_fixed16(static_cast<uint16_t>(npc.cycle->desiredPathYaw));
        npc.cycle->pos.x = (root.x * cosine + root.z * sine) + routeOrigin.x;
        npc.cycle->pos.z = (root.z * cosine - root.x * sine) + routeOrigin.z;
        root.y = npc.cycle->pos.y;
        npc.cycle->pos.y = live ? MapProbe_UpdateIntersectionStateAtPoint(root) : 0.0f;
        if (motion && character) {
            CycleManager::EnableRootMotion(character, true);
            CycleManager::SetNodeChainYaw(*character, npc.cycle->desiredPathYaw);
            shenApplyTranslationToAll(character->entity.node, npc.cycle->pos);
            motion->scriptStartFrame = npc.morsMotionCursor;
            motion->motionFlags &= ~2u;
            motion->pendingAnimId = routeAnim;
        }
        if (npc.morsMotionCursor > length) {
            const int16_t yaw = CycleManager::AngleDegreesToFixed16(terminal.y);
            terminal.y = npc.cycle->pos.y;
            if (live)
                terminal.y = MapProbe_UpdateIntersectionStateAtPoint(terminal);
            state = npcActivateIfNearInArea(npc, 30.0f, &terminal, yaw, live, npc.cycle->moveConfig->motionParamIds[0]) ? 11 : 15;
        }
        break;
    }
    case 11:
        if (CycleManager::TickPositionAndHeadingMove(npc))
            npc.cycle->workFloat4C = 1.0f;
        else
            state = 15;
        break;
    case 14:
        npc.cycle->flags |= 3u;
        [[fallthrough]];
    case 13:
        if (!CycleManager::TickPositionAndHeadingMove(npc))
            ++state;
        break;
    case 15:
        CycleManager::ReleaseMNRBReservation(npc, entry);
        npc.cycle->modeValue = npc.cycle->morsCommandArg;
        npc.cycle->morsCommandStatus = state;
        return unblocked;
    default:
        break;
    }

    bool transition = state != previousState;
    while (transition) {
        const uint16_t entered = state;
        if (state == 1 && reservation) {
            Vector3 target{};
            int16_t yaw = 0;
            if (MOVEOBJ(npc, *entry, *reservation, target, yaw)) {
                const bool atOwner = std::fabs(target.x - ownerTarget.x) < 0.00000011920929f && std::fabs(target.z - ownerTarget.z) < 0.00000011920929f;
                reservation->seatPos.x = target.x;
                reservation->seatPos.z = target.z;
                if (live)
                    target.y = MapProbe_UpdateIntersectionStateAtPoint(target);
                if (!npcActivateIfNearInArea(npc, 16.0f, &target, yaw, live, atOwner ? finalAnim : npc.cycle->moveConfig->motionParamIds[0]))
                    state = 2;
                break;
            }
            state = 2;
        } else if (state == 2 && unblocked) {
            uint16_t anim = waitAnim;
            if (std::fabs(npc.cycle->pos.x - ownerTarget.x) < 0.00000011920929f && std::fabs(npc.cycle->pos.z - ownerTarget.z) < 0.00000011920929f) {
                anim = finalAnim;
                state = 6;
            } else if (!CycleManager::MNRBMotionEnded(npc)) {
                break;
            }
            CycleManager::SetMNRBAnimation(npc, anim);
        } else if (state == 13) {
            const Vector3 target{npc.cycle->pos.x + exitDelta.x, npc.cycle->pos.y, npc.cycle->pos.z + exitDelta.z};
            const int16_t yaw = CycleManager::AngleDegreesToFixed16(exitDelta.y);
            if (!npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, npc.cycle->moveConfig->motionParamIds[0]))
                state = 14;
            CycleManager::ReleaseMNRBReservation(npc, entry);
            reservation = nullptr;
        } else if (state == 14) {
            Vector3 target{};
            ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x10u, target);
            if (!npcActivateIfNearInArea(npc, 65536.0f, &target, 0, live, npc.cycle->moveConfig->motionParamIds[0]))
                state = 15;
        } else {
            break;
        }
        transition = state != entered;
    }
    npc.cycle->morsCommandStatus = state;
    return false;
}





bool TickMnrbQueueUse(CycleManager::NpcRecord &npc, CycleManager::MNRBEntry *entry, uint32_t live) {
    uint16_t state = static_cast<uint16_t>(npc.cycle->morsCommandStatus);
    CycleManager::MNRBEntry::Reservation *reservation =
        npc.cycle->morsQueueNodeOffset ? &g_cycleManagerRuntime.mnrbReservationPool[npc.cycle->morsQueueNodeOffset - 1] : nullptr;
    if (!reservation) {
        if (state != 4)
            return state != 15 || !npc.cycle->obj || !charaHasBlockingTPMRequest(npc.cycle->obj);
        if (!CycleManager::TickPositionAndHeadingMove(npc))
            npc.cycle->morsCommandStatus = 0x3000Fu;
        return static_cast<uint16_t>(npc.cycle->morsCommandStatus) == 15 && (!npc.cycle->obj || !charaHasBlockingTPMRequest(npc.cycle->obj));
    }
    uint16_t animations[3]{};
    for (uint32_t index = 0; index < 3; ++index) {
        ReadValue(g_cycleManagerRuntime.bytes, npc.cycle->morsCommandOffset + 0x1Cu + index * 4u, animations[index]);
        if (!animations[index])
            animations[index] = npc.cycle->moveConfig->motionParamIds[0];
    }
    uint32_t ownerId = 0;
    ReadValue(g_cycleManagerRuntime.bytes, entry->ownerTokenOffset, ownerId);
    CycleManager::NpcRecord *owner = CycleManager::FindNpcRecordByObjectId(ownerId);
    const bool admitted = owner && owner->cycle->currentOpcode == CycleOpcode_ScheduleMnrBEntry &&
        owner->cycle->morsEntryOffset == npc.cycle->morsEntryOffset && static_cast<uint16_t>(owner->cycle->morsCommandStatus) >= 2 && !reservation->prev;
    Vector3 endpoint{};
    CycleManager::ReadMNRBVector(entry->areaPointOffset, 0, endpoint);
    bool returnToSeat = false;
    bool setAnimation = false;
    uint16_t animation = animations[2];
    switch (state) {
    case 1:
        if (CycleManager::TickPositionAndHeadingMove(npc)) {
            returnToSeat = npc.cycle->workFloat4C < 0.0f;
        } else {
            state = CycleManager::DistanceXZWithEpsilon(npc.cycle->pos, endpoint) < 0.1f && admitted ? 3 : 2;
            setAnimation = true;
        }
        break;
    case 2: {
        npc.cycle->flags &= ~0x10u;
        if (reservation->seatOrderKey < static_cast<int32_t>(CycleManager::CurrentTimeSeconds())) {
            state = 15;
            break;
        }
        Vector3 target{};
        int16_t yaw = 0;
        if (CycleManager::ComputeMNRBReservationPoint(npc, *entry, *reservation, target, yaw) &&
            npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, npc.cycle->moveConfig->motionParamIds[0])) {
            state = 1;
            break;
        }
        if (npc.cycle->steeringHalfWidth > CycleManager::DistanceXZWithEpsilon(npc.cycle->pos, endpoint) && admitted) {
            state = 3;
            setAnimation = true;
        } else if (CycleManager::MNRBMotionEnded(npc)) {
            setAnimation = true;
        }
        if (setAnimation && live && npc.cycle->obj && npc.cycle->mot)
            CycleManager::EnableRootMotion(npc.cycle->obj, true);
        break;
    }
    case 3:
        npc.cycle->flags &= ~0x10u;
        returnToSeat = CycleManager::MNRBMotionEnded(npc);
        break;
    default:
        break;
    }
    if (setAnimation) {
        if (state == 2) {
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            animation = animations[(g_EnvPrngSeed & 0x10000u) ? 0 : 1];
        }
        CycleManager::SetMNRBAnimation(npc, animation);
    }
    if (returnToSeat) {
        Vector3 target{reservation->seatPos.x, npc.cycle->pos.y, reservation->seatPos.z};
        if (live)
            target.y = state == 3 ? mapUpdateIntersectionStateAtLocalPoint(target) : MapProbe_UpdateIntersectionStateAtPoint(target);
        const int16_t yaw = CycleManager::Atan2ToFixed16(endpoint.x - target.x, endpoint.z - target.z);
        CycleManager::ReleaseMNRBReservation(npc, entry);
        state = npcActivateIfNearInArea(npc, 65536.0f, &target, yaw, live, npc.cycle->moveConfig->motionParamIds[0]) ? 4 : 15;
    }
    npc.cycle->morsCommandStatus = 0x30000u | state;
    if (state == 15) {
        CycleManager::ReleaseMNRBReservation(npc, entry);
        return !npc.cycle->obj || !charaHasBlockingTPMRequest(npc.cycle->obj);
    }
    return false;
}



bool TickUseMnrbEntry(CycleManager::NpcRecord &npc, uint32_t live, int32_t time) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntryByLineOffset(ctx->morsEntryOffset);
    if (entry)
        CycleManager::PruneMNRBReservations(*entry);
    const uint16_t kind = static_cast<uint16_t>(ctx->morsCommandStatus >> 16);
    bool done = true;
    if (entry && kind == 0)
        done = CycleManager::TickMnrbBusStopUse(npc, entry, live);
    else if (kind == 1)
        done = CycleManager::TickMnrbCounterUse(npc, live, time);
    else if (entry && kind == 3)
        done = CycleManager::TickMnrbQueueUse(npc, entry, live);
    if (!done)
        return false;
    if (ctx->morsEntryOffset && ctx->morsQueueNodeOffset) {
        CycleManager::ReleaseMNRBReservation(npc, CycleManager::FindMNRBEntryByLineOffset(ctx->morsEntryOffset));
        ctx->morsEntryOffset = 0;
    }
    return true;
}


bool McirRange(uint32_t relativeOffset, uint32_t count, uint32_t stride, uint32_t &absoluteOffset) {
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (!cm.mcirOffset || !relativeOffset || !stride)
        return false;
    const uint64_t begin = static_cast<uint64_t>(cm.mcirOffset) + relativeOffset;
    const uint64_t size = static_cast<uint64_t>(count) * stride;
    if (begin > cm.bytes.size() || size > cm.bytes.size() - static_cast<size_t>(begin))
        return false;
    absoluteOffset = static_cast<uint32_t>(begin);
    return true;
}


void ClearAllMoveObjChairOccupancy() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (!cm.mcirOffset)
        return;
    uint32_t groupCount = 0;
    uint32_t groupOffset = 0;
    uint32_t groups = 0;
    ReadValue(cm.bytes, cm.mcirOffset + 0x1Cu, groupCount);
    ReadValue(cm.bytes, cm.mcirOffset + 0x20u, groupOffset);
    if (!CycleManager::McirRange(groupOffset, groupCount, 32u, groups))
        return;
    const uint32_t empty = 0;
    for (uint32_t group = 0; group < groupCount; ++group) {
        uint32_t chairCount = 0;
        uint32_t chairOffset = 0;
        uint32_t chairs = 0;
        ReadValue(cm.bytes, groups + group * 32u + 0x18u, chairCount);
        ReadValue(cm.bytes, groups + group * 32u + 0x1Cu, chairOffset);
        if (!CycleManager::McirRange(chairOffset, chairCount, 32u, chairs))
            continue;
        for (uint32_t chair = 0; chair < chairCount; ++chair) {
            uint32_t positionCount = 0;
            uint32_t positionOffset = 0;
            uint32_t positions = 0;
            ReadValue(cm.bytes, chairs + chair * 32u + 0x18u, positionCount);
            ReadValue(cm.bytes, chairs + chair * 32u + 0x1Cu, positionOffset);
            if (!CycleManager::McirRange(positionOffset, positionCount, 36u, positions))
                continue;
            for (uint32_t position = 0; position < positionCount; ++position)
                std::memcpy(cm.bytes.data() + positions + position * 36u + 0x10u, &empty, sizeof(empty));
        }
    }
}



uint32_t MCIR_FindChairGroupById(uint32_t chairGroupId) {
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    uint32_t groupCount = 0;
    uint32_t groupOffset = 0;
    uint32_t groups = 0;
    if (!cm.mcirOffset || !ReadValue(cm.bytes, cm.mcirOffset + 0x1Cu, groupCount) || !ReadValue(cm.bytes, cm.mcirOffset + 0x20u, groupOffset) ||
        groupCount > 4096 || !CycleManager::McirRange(groupOffset, groupCount, 32u, groups))
        return 0;
    for (uint32_t index = 0; index < groupCount; ++index) {
        uint32_t id = 0;
        if (ReadValue(cm.bytes, groups + index * 32u, id) && id == chairGroupId)
            return groups + index * 32u;
    }
    return 0;
}




uint32_t MCIR_FindChairPosInfoByOccupantObjectId(uint32_t occupantObjectId, uint32_t chairGroup, uint32_t *chairRow) {
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    uint32_t chairCount = 0;
    uint32_t chairOffset = 0;
    uint32_t chairs = 0;
    if (!chairGroup || !ReadValue(cm.bytes, chairGroup + 0x18u, chairCount) || !ReadValue(cm.bytes, chairGroup + 0x1Cu, chairOffset) || chairCount > 4096 ||
        !CycleManager::McirRange(chairOffset, chairCount, 32u, chairs))
        return 0;
    for (uint32_t chair = 0; chair < chairCount; ++chair) {
        const uint32_t row = chairs + chair * 32u;
        uint32_t positionCount = 0;
        uint32_t positionOffset = 0;
        uint32_t positions = 0;
        if (!ReadValue(cm.bytes, row + 0x18u, positionCount) || !ReadValue(cm.bytes, row + 0x1Cu, positionOffset) || positionCount > 4096 ||
            !CycleManager::McirRange(positionOffset, positionCount, 36u, positions))
            continue;
        for (uint32_t position = 0; position < positionCount; ++position) {
            uint32_t occupant = 0;
            if (ReadValue(cm.bytes, positions + position * 36u + 0x10u, occupant) && occupant == occupantObjectId) {
                if (chairRow)
                    *chairRow = row;
                return positions + position * 36u;
            }
        }
    }
    return 0;
}



bool EndMoveObjForHistory(CycleManager::NpcRecord &npc) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
    npc.cycle->pathMoveSpeed = 1.0f;
    npc.cycle->baseMoveSpeed = 1.0f;
    const uint32_t chairGroup = MCIR_FindChairGroupById(npc.cycle->moveObjRef->chairGroupId);
    if (!chairGroup)
        return false;
    const uint32_t empty = 0;
    for (uint32_t seat = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, chairGroup); seat;
         seat = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, chairGroup))
        std::memcpy(cm.bytes.data() + seat + 0x10u, &empty, sizeof(empty));
    const uint32_t freeSeat = MCIR_FindChairPosInfoByOccupantObjectId(0, chairGroup);
    uint32_t count = 0;
    uint32_t offset = 0;
    ReadValue(cm.bytes, freeSeat ? freeSeat + 0x1Cu : chairGroup + 0x10u, count);
    ReadValue(cm.bytes, freeSeat ? freeSeat + 0x20u : chairGroup + 0x14u, offset);
    uint32_t points = 0;
    if (!count || !CycleManager::McirRange(offset, count, sizeof(Vector3), points))
        return false;
    return ReadValue(cm.bytes, points + (count - 1u) * sizeof(Vector3), npc.cycle->pos);
}

Vector3 MoveObjChairPresetOffset(const CycleManager::NpcRecord &npc) {
    for (const MoveObjChairAnchorAdjustment &entry : kMoveObjChairAnchorAdjustments) {
        if (entry.preset == npc.cycle->moveObjRef->chairGroupId && entry.anchorGroup == npc.cycle->moveConfig->anchorGroupCode)
            return {entry.localX, 0.0f, entry.localZ};
    }
    return {};
}

// The seat's position (chair-position row +0) moved by the chair group's preset offset for the cycle's rig, turned by the seat's yaw (+0xC).
Vector3 MoveObjPresetChairPosition(const CycleManager::NpcRecord &npc, uint32_t chairPos) {
    const Vector3 offset = CycleManager::MoveObjChairPresetOffset(npc);
    Vector3 seat{};
    uint32_t seatYaw = 0;
    if (chairPos) {
        ReadValue(g_cycleManagerRuntime.bytes, chairPos, seat);
        ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x0Cu, seatYaw);
    }
    const uint16_t yaw = static_cast<uint16_t>(seatYaw);
    const uint16_t folded = yaw <= 0x8000u ? yaw : static_cast<uint16_t>(-yaw);
    const float cosine = sin_fixed16(static_cast<uint16_t>(0x4000u - folded));
    const float sine = sin_fixed16(yaw);
    return {cosine * offset.x + sine * offset.z + seat.x, seat.y, cosine * offset.z - sine * offset.x + seat.z};
}

uint16_t FindMoveObjIdleMotion(const CycleManager::NpcRecord &npc) {
    uint32_t offset = CycleManager::FileOffset(npc.cycle->moveObjRef) + 20u;
    for (uint32_t index = 0; index < npc.cycle->moveObjSubcommandCount; ++index) {
        uint32_t opcode = 0;
        if (!ReadValue(g_cycleManagerRuntime.bytes, offset, opcode))
            return 0;
        switch (opcode) {
        case CycleOpcode_SetActiveAnimAndMotionVariant:
        case CycleOpcode_SetAnimationState: {
            uint16_t animation = 0;
            ReadValue(g_cycleManagerRuntime.bytes, offset + 4u, animation);
            return animation;
        }
        case CycleOpcode_WaitSeconds:
        case CycleOpcode_EndObjectInteraction:
        case CycleOpcode_EndLinkedObjectInteraction:
            offset += 8u;
            break;
        case CycleOpcode_BeginObjectInteraction:
        case CycleOpcode_BeginLinkedObjectInteraction:
            offset += 48u;
            break;
        default:
            return 0;
        }
    }
    return 0;
}



uint16_t SelectMoveObjSitMotion(const CycleManager::NpcRecord &npc, bool entering) {
    const uint32_t pairIndex = entering ? 0u : 1u;
    if (npc.cycle->areaID.name == SHEN_NAME(DGCT))
        return npc.cycle->moveConfig->motionParamIds[29u + pairIndex];

    uint32_t chairRow = 0;
    MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(npc.cycle->moveObjRef->chairGroupId), &chairRow);
    uint32_t chairMode = 0;
    uint32_t chairFlags = 0;
    float chairHeight = 0.0f;
    if (chairRow) {
        ReadValue(g_cycleManagerRuntime.bytes, chairRow + 0x00u, chairMode);
        ReadValue(g_cycleManagerRuntime.bytes, chairRow + 0x0Cu, chairFlags);
        ReadValue(g_cycleManagerRuntime.bytes, chairRow + 0x14u, chairHeight);
    }

    if (std::fabs(chairHeight - 0.45f) < 0.00000011920929f) {
        const uint32_t base = (!chairMode || chairFlags == 3u) ? 25u : 27u;
        return npc.cycle->moveConfig->motionParamIds[base + pairIndex];
    }
    if (std::fabs(chairHeight - 0.30f) < 0.00000011920929f)
        return npc.cycle->moveConfig->packedMotionParams[2u + pairIndex];
    if (std::fabs(chairHeight - 0.60f) < 0.00000011920929f)
        return npc.cycle->moveConfig->packedMotionParams[pairIndex];

    const auto publish = [&](std::uint32_t token) {
        std::uint32_t index = g_TotalActiveCycles;
        if (index >= 10u)
            index = 0;
        g_CycleTable[2u * index] = token;
        g_TotalActiveCycles = index + 1;
    };
    publish(npc.cycle->moveConfig->obj_id.name);
    publish(npc.cycle->moveObjRef->chairGroupId);
    publish(npc.cycle->areaID.name);
    return npc.cycle->moveConfig->packedMotionParams[pairIndex];
}

bool MoveObjFacingConstraint(const CycleManager::NpcRecord &npc) {
    const uint32_t chairPos =
        MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(npc.cycle->moveObjRef->chairGroupId));
    if (!chairPos)
        return true;

    uint32_t pointCount = 0;
    uint32_t relativeOffset = 0;
    ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x14u, pointCount);
    ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x18u, relativeOffset);
    if (pointCount < 2u || !relativeOffset)
        return true;

    uint32_t pointTable = 0;
    if (!CycleManager::McirRange(relativeOffset, pointCount, 12u, pointTable))
        return true;

    Vector3 previous{};
    if (!ReadValue(g_cycleManagerRuntime.bytes, pointTable + (pointCount - 2u) * 12u, previous)) {
        return true;
    }

    const Vector3 chairPosition = CycleManager::MoveObjPresetChairPosition(npc, chairPos);
    const float deltaX = chairPosition.x - previous.x;
    const float deltaZ = chairPosition.z - previous.z;
    if (std::fabs(deltaX) < 0.001f && std::fabs(deltaZ) < 0.001f)
        return true;

    const uint16_t approachYaw = static_cast<uint16_t>(CycleManager::Atan2Fixed16(deltaX, deltaZ));
    const uint16_t baseYaw = static_cast<uint16_t>(npc.cycle->moveObjBaseYaw);
    if (baseYaw < approachYaw)
        return static_cast<uint16_t>(approachYaw - baseYaw) <= 0x8000u;
    return static_cast<uint16_t>(baseYaw - approachYaw) > 0x8000u;
}


Vector3 MoveObjAdjustedChairPosition(CycleManager::NpcRecord &npc, uint32_t chairPos, bool entering, uint16_t animId, bool facingConstraint) {
    Vector3 rootOffset{};
    if (!(entering && npc.cycle->b_paused)) {
        g_motnHasYValue = 1;
        const int32_t frameCount = MOTN_getFrameCountFromAnimId(animId);
        const int32_t rootMotionEnd = motnGetRootMotionEndMarker(animId);
        const float sampleFrame = static_cast<float>(frameCount - rootMotionEnd) - 1.0f;
        (void)MOTN_ReadRootPositionAtFrame(animId, npc.cycle->moveConfig->anchorGroupCode, sampleFrame, rootOffset, true);
        if (!facingConstraint)
            rootOffset.x = -rootOffset.x;
        rootOffset.y = 0.0f;
    }
    const Vector3 adjustedChairPosition = CycleManager::MoveObjPresetChairPosition(npc, chairPos);

    const uint16_t yaw = static_cast<uint16_t>(static_cast<uint16_t>(npc.cycle->moveObjBaseYaw) + (entering ? 0x8000u : 0u));
    return RotateAroundYAndTransformWorldCoords(rootOffset, adjustedChairPosition, yaw);
}

void SetMoveObjPathSteeringProfile(CycleManager::NpcRecord &npc, bool pathPhase) {
    if (pathPhase) {
        if (npc.cycle->steeringProfile == 1u)
            npc.cycle->steeringProfile = 2u;
    } else if (npc.cycle->steeringProfile == 2u) {
        npc.cycle->steeringProfile = 1u;
    }
    CycleManager::RecomputeSteeringExtentsForNpc(npc);
}

bool BeginMoveObjPath(CycleManager::NpcRecord &npc, uint32_t relativeOffset, uint32_t pointCount, bool live) {
    uint32_t pointTable = 0;
    if (!pointCount || !CycleManager::McirRange(relativeOffset, pointCount, sizeof(Vector3), pointTable))
        return false;
    const bool started = CycleManager::InitPathFollowMotion(npc, pointTable, pointCount, live);
    npc.cycle->currentOpcode = CycleOpcode_StartLinkedCycle;
    npc.cycle->moveObjSubOpcode = CycleOpcode_WalkPath3D;
    return started;
}

void EndMoveObjPayload(CycleManager::NpcRecord &npc, uint32_t offset) {
    uint32_t identifier = 0;
    ReadValue(g_cycleManagerRuntime.bytes, offset + 4u, identifier);
    if (CycleManager::TaggedPayload *const payload = CycleManager::FindTaggedPayload(npc, identifier))
        payload->lifetime = 7683;
}


uint32_t MoveObjChairArea(uint32_t preset) {
    switch (preset) {
    case SHEN_NAME(DSBA):
    case SHEN_NAME(DTKY):
    case SHEN_NAME(DRHT):
    case SHEN_NAME(DGCT):
    case SHEN_NAME(DSK1):
    case SHEN_NAME(HDG1):
    case SHEN_NAME(DSUS):
    case SHEN_NAME(FBEA):
    case SHEN_NAME(DCHA):
    case SHEN_NAME(DSLI):
    case SHEN_NAME(DPIZ):
    case SHEN_NAME(DRME):
    case SHEN_NAME(MKYU):
        return preset;
    case SHEN_NAME(KPA1):
        return SHEN_NAME(DKPA);
    case SHEN_NAME(BYO2):
    case SHEN_NAME(BYO3):
    case SHEN_NAME(GRKN):
    case SHEN_NAME(ECHO):
        return SHEN_NAME(DBYO);
    case SHEN_NAME(JAZ2):
        return SHEN_NAME(DJAZ);
    case SHEN_NAME(MAJ1):
    case SHEN_NAME(MAJ2):
        return SHEN_NAME(DMAJ);
    case SHEN_NAME(SZM1):
    case SHEN_NAME(SZM2):
    case SHEN_NAME(SZM3):
    case SHEN_NAME(SZM4):
        return SHEN_NAME(DSZM);
    case SHEN_NAME(MO01):
    case SHEN_NAME(JIM1):
    case SHEN_NAME(JIM2):
        return SHEN_NAME(MALP);
    case SHEN_NAME(SYO1):
    case SHEN_NAME(SYO2):
        return SHEN_NAME(MSYO);
    case SHEN_NAME(MARK):
    case SHEN_NAME(HTSI):
    case SHEN_NAME(TAKE):
    case SHEN_NAME(TYMK):
    case SHEN_NAME(YOHI):
        return SHEN_NAME(MTEK);
    default:
        return SHEN_NAME(____);
    }
}

bool HasMoveObjChairPreset(const CycleManager::NpcRecord &npc) {
    const uint32_t chairGroupId = npc.cycle->moveObjRef->chairGroupId;
    if (CycleManager::MoveObjChairArea(chairGroupId) == SHEN_NAME(____))
        return false;
    bool hasAdjustments = false;
    for (const MoveObjChairAnchorAdjustment &entry : kMoveObjChairAnchorAdjustments) {
        if (entry.preset != chairGroupId)
            continue;
        hasAdjustments = true;
        if (entry.anchorGroup == npc.cycle->moveConfig->anchorGroupCode)
            return true;
    }
    return !hasAdjustments;
}

bool MoveObjAnchorIsClear(const CycleManager::NpcRecord &npc) {
    if (Character *const ryo = CycleManager::MapRyo(); ryo && npc.cycle->areaID.name == CycleManager::CurrentAreaId()) {
        const Vector3 playerPosition = GetPositionForEntity(ryo->entity.node);
        if (g_fGlobalTurningRadius + npc.cycle->steeringHalfWidth + 0.1f > CycleManager::Distance(npc.cycle->moveObjAnchorPos, playerPosition))
            return false;
    }
    if (CycleManager::MoveObjChairArea(npc.cycle->moveObjRef->chairGroupId) == SHEN_NAME(MTEK))
        return true;
    for (const CycleProgramList list : {CycleProgramList_SimSet, CycleProgramList_Running}) {
        for (const CycleManager::NpcRecord *other = CycleManager::NextProgramInList(list); other; other = CycleManager::NextProgramInList(list, other)) {
            if (other->cycle->moveConfig->obj_id.name != npc.cycle->moveConfig->obj_id.name && other->cycle->areaID.name == npc.cycle->areaID.name &&
                other->cycle->steeringHalfWidth + npc.cycle->steeringHalfWidth > CycleManager::Distance(npc.cycle->moveObjAnchorPos, other->cycle->pos))
                return false;
        }
    }
    return true;
}

bool MoveObjStandIsBlocked(const CycleManager::NpcRecord &npc) {
    if (npc.cycle->areaID.name != CycleManager::CurrentAreaId() || npc.cycle->currentOpcode != CycleOpcode_StartLinkedCycle)
        return false;
    const uint32_t mappedArea = CycleManager::MoveObjChairArea(npc.cycle->moveObjRef->chairGroupId);
    for (const CycleProgramList list : {CycleProgramList_SimSet, CycleProgramList_Far, CycleProgramList_Near, CycleProgramList_Running}) {
        for (const CycleManager::NpcRecord *other = CycleManager::NextProgramInList(list); other; other = CycleManager::NextProgramInList(list, other)) {
            if (other->cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name || other->cycle->areaID.name != CycleManager::CurrentAreaId())
                continue;
            if (other->cycle->currentOpcode == CycleOpcode_RideVehiclePath2D || other->cycle->currentOpcode == CycleOpcode_DriveVehiclePath2D) {
                if (npc.cycle->areaID.name != SHEN_NAME(D000) && (npc.cycle->areaID.name != SHEN_NAME(MFSY) || mappedArea != SHEN_NAME(MTEK)))
                    return true;
            } else if (other->cycle->currentOpcode == CycleOpcode_StartLinkedCycle && mappedArea != SHEN_NAME(____) &&
                mappedArea == CycleManager::MoveObjChairArea(other->cycle->moveObjRef->chairGroupId)) {
                if (other->cycle->moveObjPhase - 1u <= 1u || other->cycle->moveObjPhase - 9u <= 3u)
                    return true;
            }
        }
    }
    return false;
}

void RecordMoveObjDiagnostic(const CycleManager::NpcRecord &npc) {
    uint32_t index = g_TotalActiveCycles;
    if (index >= 10u)
        index = 0;
    g_CycleTable[2u * index] = npc.cycle->moveConfig->obj_id.name;
    g_TotalActiveCycles = index + 1;
}

bool AdvanceMoveObjSubcommand(CycleManager::NpcRecord &npc, bool live, int32_t time) {
    const MotionInformationWork *const motion = npc.cycle->mot;
    switch (npc.cycle->moveObjSubOpcode) {
    case 0:
    case CycleOpcode_BeginObjectInteraction:
    case CycleOpcode_EndObjectInteraction:
    case CycleOpcode_BeginLinkedObjectInteraction:
    case CycleOpcode_EndLinkedObjectInteraction:
        break;
    case CycleOpcode_SetActiveAnimAndMotionVariant:
        if (live && motion && !(motion->motionFlags & 2u)) {
            if ((npc.cycle->moveObjPhase == CycleMoveObjPhase_SnapToAnchor || npc.cycle->moveObjPhase == CycleMoveObjPhase_SetReturnArea) &&
                npc.cycle->areaID.name == CycleManager::CurrentAreaId()) {
                CycleManager::AppendDynamicTurnVolume(npc.cycle->moveObjAnchorPos, 0, npc.cycle->steeringHalfWidth, 0.0f);
                npc.cycle->flags &= ~CycleFlag_CheckStaticSteeringVolumes;
            }
            return false;
        }
        break;
    case CycleOpcode_SetAnimationState:
        if (live && motion && !(motion->motionFlags & 2u))
            return false;
        break;
    case CycleOpcode_WaitSeconds:
        if (time <= static_cast<int32_t>(npc.cycle->steeringWaitFrames) || (motion && !(motion->motionFlags & 2u)))
            return false;
        break;
    case CycleOpcode_WalkPath3D:
        if (CycleManager::TickPathFollowMotion(npc, live))
            return false;
        break;
    case 0x27:
        CycleManager::RecordMoveObjDiagnostic(npc);
        break;
    default:
        CycleManager::RecordMoveObjDiagnostic(npc);
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return true;
    }

    if (npc.cycle->moveObjPhase == CycleMoveObjPhase_SetIdleAnim || npc.cycle->moveObjPhase == CycleMoveObjPhase_CalcReturnAnchor) {
        if (!CycleManager::MoveObjAnchorIsClear(npc) ||
            (npc.cycle->moveObjPhase == CycleMoveObjPhase_CalcReturnAnchor && CycleManager::MoveObjStandIsBlocked(npc)))
            return false;
        if (npc.cycle->areaID.name == CycleManager::CurrentAreaId()) {
            CycleManager::AppendDynamicTurnVolume(npc.cycle->moveObjAnchorPos, 0, npc.cycle->steeringHalfWidth, 0.0f);
            npc.cycle->flags &= ~CycleFlag_CheckStaticSteeringVolumes;
        }
    }
    switch (npc.cycle->moveObjPhase) {
    case CycleMoveObjPhase_Uninitialized:
    case CycleMoveObjPhase_BeginPath:
    case CycleMoveObjPhase_SetIdleAnim:
    case CycleMoveObjPhase_EndSubcommands:
    case CycleMoveObjPhase_CalcReturnAnchor:
    case CycleMoveObjPhase_SetReturnArea:
    case CycleMoveObjPhase_Finished:
        npc.cycle->moveObjPhase = static_cast<CycleMoveObjPhase>(npc.cycle->moveObjPhase + 1);
        break;
    case CycleMoveObjPhase_SnapToAnchor:
        npc.cycle->moveObjSubcommandIndex = 0;
        npc.cycle->moveObjSubcommandPtr = CycleManager::FilePointer(CycleManager::FileOffset(npc.cycle->moveObjRef) + 20u);
        npc.cycle->pos = npc.cycle->moveObjAnchorPos;
        if (npc.cycle->obj)
            shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
        npc.cycle->moveObjPhase = CycleMoveObjPhase_ProcessSubcommands;
        break;
    case CycleMoveObjPhase_ProcessSubcommands:
    case CycleMoveObjPhase_ProcessSubcommandsActive:
    case CycleMoveObjPhase_ProcessSubcommandsEnd:
        if (static_cast<int32_t>(npc.cycle->moveObjSubcommandIndex) >= static_cast<int32_t>(npc.cycle->moveObjSubcommandCount - 1u)) {
            if (npc.cycle->moveObjPhase == CycleMoveObjPhase_ProcessSubcommandsEnd) {
                npc.cycle->moveObjPhase = CycleMoveObjPhase_EndSubcommands;
            } else {
                npc.cycle->moveObjPhase =
                    npc.cycle->moveObjPhase == CycleMoveObjPhase_ProcessSubcommandsActive && time > static_cast<int32_t>(npc.cycle->time) ?
                    CycleMoveObjPhase_ProcessSubcommandsEnd :
                    CycleMoveObjPhase_ProcessSubcommandsActive;
                npc.cycle->moveObjSubcommandIndex = 0;
                npc.cycle->moveObjSubcommandPtr = CycleManager::FilePointer(CycleManager::FileOffset(npc.cycle->moveObjRef) + 20u);
            }
        } else {
            ++npc.cycle->moveObjSubcommandIndex;
        }
        break;
    case CycleMoveObjPhase_BeginReturnPath:
    case CycleMoveObjPhase_ResumePath:
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        break;
    default:
        CycleManager::RecordMoveObjDiagnostic(npc);
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return true;
    }
    return true;
}

bool ProcessMoveObjInlineCommand(CycleManager::NpcRecord &npc, int32_t time) {
    uint32_t opcode = 0;
    if (!ReadValue(g_cycleManagerRuntime.bytes, CycleManager::FileOffset(npc.cycle->moveObjSubcommandPtr), opcode))
        return false;
    uint32_t size = 8;

    const uint32_t offset = CycleManager::FileOffset(npc.cycle->moveObjSubcommandPtr);
    const bool setupPass = npc.cycle->moveObjPhase == CycleMoveObjPhase_ProcessSubcommands;
    const bool activePass = npc.cycle->moveObjPhase == CycleMoveObjPhase_ProcessSubcommandsActive;
    const bool teardownPass = npc.cycle->moveObjPhase == CycleMoveObjPhase_ProcessSubcommandsEnd;
    npc.cycle->moveObjSubOpcode = 0;
    switch (opcode) {
    case CycleOpcode_SetActiveAnimAndMotionVariant:
        if (activePass) {
            CycleManager::ApplyNpcCommandState(npc, opcode, offset, nullptr);
            npc.cycle->moveObjPathActive = false;
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_WaitSeconds:
        if (activePass) {
            uint32_t duration = 0;
            ReadValue(g_cycleManagerRuntime.bytes, offset + 4u, duration);
            npc.cycle->steeringWaitFrames = static_cast<uint32_t>(time) + duration;
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_BeginObjectInteraction:
        size = 48;
        if (activePass) {
            CycleManager::QueueObjectInteraction(npc, offset);
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_EndObjectInteraction:
        if (activePass) {
            CycleManager::EndMoveObjPayload(npc, offset);
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_SetAnimationState:
        size = 16;
        if (activePass) {
            CycleManager::ApplyNpcCommandState(npc, opcode, offset, nullptr);
            npc.cycle->moveObjPathActive = true;
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_BeginLinkedObjectInteraction:
        size = 48;
        if (setupPass) {
            CycleManager::QueueObjectInteraction(npc, offset);
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    case CycleOpcode_EndLinkedObjectInteraction:
        if (teardownPass) {
            CycleManager::EndMoveObjPayload(npc, offset);
            npc.cycle->moveObjSubOpcode = opcode;
        }
        break;
    default:
        npc.cycle->moveObjPathActive = false;
        npc.cycle->moveObjPhase = CycleMoveObjPhase_EndSubcommands;
        CycleManager::RecordMoveObjDiagnostic(npc);
        return false;
    }
    npc.cycle->currentOpcode = CycleOpcode_StartLinkedCycle;
    npc.cycle->moveObjSubcommandPtr += size;
    return true;
}




bool ExecuteMoveObjState(CycleManager::NpcRecord &npc, bool live, int32_t time) {
    npc.cycle->flags &= ~CycleFlag_CheckMapIntersections;
    if (!CycleManager::AdvanceMoveObjSubcommand(npc, live, time))
        return false;
    if (npc.cycle->currentOpcode == CycleOpcode_AdvanceScript) {
        ++g_cycleManagerRuntime.moveObjSitCompletions;
        return true;
    }

    const uint32_t chairGroupId = npc.cycle->moveObjRef->chairGroupId;
    switch (npc.cycle->moveObjPhase) {
    case CycleMoveObjPhase_BeginPath: {
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        const uint32_t chairPos = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(chairGroupId));
        if (!chairPos) {
            npc.cycle->moveObjPhase = CycleMoveObjPhase_Finished;
            npc.cycle->moveObjSubOpcode = 0;
            break;
        }
        ++g_cycleManagerRuntime.moveObjSitBegins;
        ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x0Cu, npc.cycle->moveObjBaseYaw);
        const bool approachSide = CycleManager::MoveObjFacingConstraint(npc);
        const uint16_t animation = CycleManager::SelectMoveObjSitMotion(npc, true);
        const Vector3 approachPosition = CycleManager::MoveObjAdjustedChairPosition(npc, chairPos, true, animation, approachSide);
        npc.cycle->moveObjAnchorPos = CycleManager::MoveObjPresetChairPosition(npc, chairPos);
        uint32_t count = 0;
        uint32_t offset = 0;
        uint32_t points = 0;
        ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x14u, count);
        ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x18u, offset);
        if (count && CycleManager::McirRange(offset, count, sizeof(Vector3), points))
            std::memcpy(g_cycleManagerRuntime.bytes.data() + points + (count - 1u) * sizeof(Vector3), &approachPosition, sizeof(Vector3));
        npc.cycle->moveObjSubOpcode = 0;
        if (!npc.cycle->b_paused) {
            npc.cycle->moveObjPathActive = true;
            CycleManager::BeginMoveObjPath(npc, offset, count, live);
        }
        break;
    }
    case CycleMoveObjPhase_SetIdleAnim:
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        npc.cycle->moveObjSubOpcode = 0;
        if (!npc.cycle->b_paused)
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        break;
    case CycleMoveObjPhase_SnapToAnchor: {
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        const uint32_t chairPos = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(chairGroupId));
        const bool approachSide = CycleManager::MoveObjFacingConstraint(npc);
        const uint16_t animation = CycleManager::SelectMoveObjSitMotion(npc, true);
        npc.cycle->pos = CycleManager::MoveObjAdjustedChairPosition(npc, chairPos, true, animation, approachSide);
        if (npc.cycle->obj)
            shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
        ++g_cycleManagerRuntime.moveObjSitAnchors;
        npc.cycle->moveTargetYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
        npc.cycle->moveObjSubOpcode = npc.cycle->b_paused ? 0u : CycleOpcode_SetActiveAnimAndMotionVariant;
        if (!npc.cycle->b_paused)
            CycleManager::ApplyActiveAnimAndMotionVariant(npc, animation, !approachSide);
        break;
    }
    case CycleMoveObjPhase_ProcessSubcommands:
    case CycleMoveObjPhase_ProcessSubcommandsActive:
    case CycleMoveObjPhase_ProcessSubcommandsEnd:
        CycleManager::SetMoveObjPathSteeringProfile(npc, false);
        CycleManager::ProcessMoveObjInlineCommand(npc, time);
        break;
    case CycleMoveObjPhase_EndSubcommands:
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, false);
        break;
    case CycleMoveObjPhase_CalcReturnAnchor: {
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, false);
        const uint32_t chairPos = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(chairGroupId));
        const bool approachSide = CycleManager::MoveObjFacingConstraint(npc);
        const uint16_t animation = CycleManager::SelectMoveObjSitMotion(npc, false);
        npc.cycle->moveObjAnchorPos = CycleManager::MoveObjAdjustedChairPosition(npc, chairPos, false, animation, approachSide);
        npc.cycle->moveObjSubOpcode = 0;
        break;
    }
    case CycleMoveObjPhase_SetReturnArea: {
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        const bool approachSide = CycleManager::MoveObjFacingConstraint(npc);
        const uint16_t animation = CycleManager::SelectMoveObjSitMotion(npc, false);
        npc.cycle->moveTargetYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->moveObjBaseYaw);
        npc.cycle->moveObjSubOpcode = CycleOpcode_SetActiveAnimAndMotionVariant;
        CycleManager::ApplyActiveAnimAndMotionVariant(npc, animation, !approachSide);
        ++g_cycleManagerRuntime.moveObjStandBegins;
        break;
    }
    case CycleMoveObjPhase_BeginReturnPath: {
        npc.cycle->moveObjPathActive = true;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        const uint32_t chairPos = MCIR_FindChairPosInfoByOccupantObjectId(npc.cycle->moveConfig->obj_id.name, MCIR_FindChairGroupById(chairGroupId));
        const bool approachSide = CycleManager::MoveObjFacingConstraint(npc);
        const uint16_t animation = CycleManager::SelectMoveObjSitMotion(npc, false);
        npc.cycle->pos = CycleManager::MoveObjAdjustedChairPosition(npc, chairPos, false, animation, approachSide);
        uint32_t count = 0;
        uint32_t offset = 0;
        if (chairPos) {
            ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x1Cu, count);
            ReadValue(g_cycleManagerRuntime.bytes, chairPos + 0x20u, offset);
            const uint32_t occupant = 0;
            std::memcpy(g_cycleManagerRuntime.bytes.data() + chairPos + 0x10u, &occupant, sizeof(occupant));
        }
        npc.cycle->moveObjSubOpcode = CycleOpcode_WalkPath3D;
        CycleManager::BeginMoveObjPath(npc, offset, count, live);
        break;
    }
    case CycleMoveObjPhase_ResumePath: {
        npc.cycle->moveObjPathActive = true;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        const uint32_t chairGroup = MCIR_FindChairGroupById(chairGroupId);
        if (!chairGroup) {
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
            return true;
        }
        uint32_t count = 0;
        uint32_t offset = 0;
        ReadValue(g_cycleManagerRuntime.bytes, chairGroup + 0x10u, count);
        ReadValue(g_cycleManagerRuntime.bytes, chairGroup + 0x14u, offset);
        npc.cycle->moveObjSubOpcode = CycleOpcode_WalkPath3D;
        ++g_cycleManagerRuntime.moveObjResumePathStarts;
        CycleManager::BeginMoveObjPath(npc, offset, count, live);
        break;
    }
    default:
        npc.cycle->moveObjPathActive = false;
        CycleManager::SetMoveObjPathSteeringProfile(npc, true);
        CycleManager::RecordMoveObjDiagnostic(npc);
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return true;
    }
    npc.cycle->currentOpcode = CycleOpcode_StartLinkedCycle;
    return false;
}
} // namespace CycleManager
  // namespace

namespace CycleManager {

bool MnrBEntryAllowsTalk(const CycleManager::NpcRecord &npc) {
    if (npc.cycle->xm_resume_state)
        return true;
    const int32_t status = static_cast<int32_t>(npc.cycle->morsCommandStatus);
    const int32_t kind = status >> 16;

    if (kind)
        return kind == 1 || kind == 3;
    if (!npc.cycle->morsCommandOffset)
        return true;
    const CycleManager::MNRBEntry *const entry = CycleManager::FindMNRBEntry(CycleManager::ReadCommandDwordOrZero(npc.cycle->morsCommandOffset, 4));

    if (!entry || !entry->ownerTokenOffset)
        return true;
    uint32_t partnerId = 0;
    ReadValue(g_cycleManagerRuntime.bytes, entry->ownerTokenOffset, partnerId);
    const CycleManager::NpcRecord *const partner = CycleManager::FindNpcRecordByObjectId(partnerId);
    if (!partner || partner->cycle->currentOpcode != CycleOpcode_ScheduleMnrBEntry || partner->cycle->morsCommandStatus != 3U ||
        static_cast<uint16_t>(status) >= 13U)
        return true;
    if (static_cast<int32_t>(CycleManager::ReadCommandDwordOrZero(npc.cycle->morsCommandOffset, 8)) <= 0)
        return false;
    return 0.5f >= static_cast<float>(static_cast<int32_t>(partner->cycle->selectedMotionToken)) / 500.0f;
}
} // namespace CycleManager

} // namespace shenmue::s1
