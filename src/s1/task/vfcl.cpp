#include "s1/task/vfcl.hpp"
#include "s1/motion.hpp"
#include "s1/task/char.hpp"

#include "s1/task/vfma.hpp"
#include "s1/matrix_stack.hpp"

#include <bit>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {
namespace {

constexpr std::uint32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::uint32_t kVfclToken = SHEN_NAME(VFCL);
constexpr std::uint32_t kCollisionName = SHEN_NAME(FCOL);
constexpr std::size_t kProbeCacheCount = 10;
constexpr std::size_t kParticipantCapacity = 16;

static_assert(offsetof(VFActor, contactProfileMasks) - offsetof(VFActor, previousContactProbePositions) == kProbeCacheCount * sizeof(Vector3));

Vector3 TransformPoint(const Vector3 &point, const Matrix4x4 &matrix) {
    return {
        point.x * matrix.rows[0].x + point.y * matrix.rows[1].x + point.z * matrix.rows[2].x + matrix.rows[3].x,
        point.x * matrix.rows[0].y + point.y * matrix.rows[1].y + point.z * matrix.rows[2].y + matrix.rows[3].y,
        point.x * matrix.rows[0].z + point.y * matrix.rows[1].z + point.z * matrix.rows[2].z + matrix.rows[3].z,
    };
}

const Matrix4x4 *FindPreviousNodeTransform(const MotionInformationWork *motion, std::uint8_t nodeId) {
    auto *const rows = motion && motion->previousNodes ? static_cast<const MotionNodeRuntime *>(motion->previousNodes) : nullptr;
    for (std::size_t index = 0; rows && index < 128 && rows[index].nodeId != 0xffU; ++index) {
        if (rows[index].nodeId == nodeId)
            return rows[index].transform;
    }
    return nullptr;
}

void CopyContactProbeCache(VFActor &actor) {
    std::memcpy(actor.previousContactProbePositions, actor.contactProbePositions, sizeof(actor.contactProbePositions));
}

void PrepareNativeFrame(VFCLTaskData &context, VFMAData *game) {
    constexpr std::uint32_t kForcedReaction = 0x10000000U;
    constexpr std::uint32_t kContactHit = 0x00001000U;
    for (std::size_t i = 0; i < 6; ++i) {
        context.contactDisplacement[i] = {};
        VFActor *const actor = game && game->characters[i] ? game->actors[i] : nullptr;
        if (!actor)
            continue;

        if ((actor->moveFlags & kForcedReaction) != 0) {
            actor->moveFlags &= ~kForcedReaction;
            GetTransformAndTransformVector3(*actor);
            CopyContactProbeCache(*actor);
            context.currentContact[i] = context.previousContact[i];
        } else {
            CopyContactProbeCache(*actor);
            context.currentContact[i] = context.previousContact[i];
            GetTransformAndTransformVector3(*actor);
        }
        actor->field436 = (actor->hitStateFlags2 & kContactHit) != 0 ? 0U : 0x9000U;
        std::memset(actor->contactProfileMasks, 0, sizeof(actor->contactProfileMasks));
        actor->stateFlags = (actor->stateFlags & ~0x20U) | 0x40U;
    }
}

int BuildNativeParticipants(VFCLParticipantTmp *participants, int capacity) {
    if (!participants || capacity <= 0)
        return 0;
    VFFightStage *const stage = g_pFightStage;
    if (!stage || !stage->enemySlots)
        return 0;

    int participantLen = 0;
    for (std::uint8_t i = 0; i < stage->maxEnemies && participantLen < capacity; ++i) {
        VFEnemySlot &enemy = stage->enemySlots[i];
        if (!enemy.character)
            continue;
        VFCLParticipantTmp &participant = participants[participantLen];
        if (enemy.state == 1U) {
            participant.type = VFParticipant::Sub;
            participant.index = static_cast<std::int8_t>(i);
            participant.link = 0;
            participant.fcmg = &enemy;
            participant.x = enemy.position.x;
            participant.z = enemy.position.z;
            ++participantLen;
        } else if (enemy.state == 2U && enemy.fighter) {
            participant.type = VFParticipant::Main;
            participant.index = static_cast<std::int8_t>(enemy.fighter->slotIndex);
            participant.link = enemy.fighter->collisionHitFlags != 0;
            participant.fcmg = enemy.fighter;
            participant.x = enemy.fighter->contactCurrentPosition.x;
            participant.z = enemy.fighter->contactCurrentPosition.z;
            ++participantLen;
        }
    }
    return participantLen;
}

void ResetNativeAirborneSlots(VFCLTaskData &context, VFMAData *game) {
    constexpr std::uint32_t kResetAfterCollision = 0x00008000U;
    std::memset(context.terrainOverlapFlags, 0, sizeof(context.terrainOverlapFlags));
    for (std::size_t i = 0; i < 6; ++i) {
        VFActor *const actor = game && game->characters[i] ? game->actors[i] : nullptr;
        if (!actor)
            continue;
        actor->moveFlags &= ~kResetAfterCollision;
        actor->terrainCollisionLatched = 0;
        actor->collisionHitFlags = 0;
    }
}

} // namespace

void GetTransformAndTransformVector3(VFActor &actor) {
    VFCLTaskData &context = *static_cast<VFCLTaskData *>(actor.game->collisionData);
    const std::size_t slotIndex = static_cast<std::int8_t>(actor.slotIndex);
    const auto *const motion = static_cast<const MotionInformationWork *>(actor.motion);
    const std::byte *const profile = actor.contactVolumeRecords;
    const Matrix4x4 stackTop = *g_matrixStack.top.stack;
    const std::uint8_t mode = motion && motion->poseEvaluationSucceeded ? 0U : motion ? motion->poseEvaluationStage : 0U;
    const Matrix4x4 *retainedTransform = nullptr;

    if (motion && profile) {
        for (std::size_t index = 0; index < kProbeCacheCount; ++index) {
            const std::byte *const row = profile + index * 0x14U;
            const std::uint8_t selector = std::to_integer<std::uint8_t>(row[0x10]);
            if (mode == 1U && selector != 2U) {
                actor.contactProbePositions[index] = actor.contactProbePositions[0];
                continue;
            }
            if (mode == 2U) {
                if (const Matrix4x4 *const root = FindPreviousNodeTransform(motion, 0U)) {
                    actor.contactProbePositions[index] = {
                        root->rows[3].x,
                        root->rows[3].y,
                        root->rows[3].z,
                    };
                }
                continue;
            }

            if (mode == 0U) {
                retainedTransform = FindPreviousNodeTransform(motion, selector);
            } else if (mode == 1U) {
                retainedTransform = FindPreviousNodeTransform(motion, 2U);
            }
            const Matrix4x4 &transform = retainedTransform ? *retainedTransform : stackTop;
            Vector3 local{};
            std::memcpy(&local, row, sizeof(local));
            actor.contactProbePositions[index] = TransformPoint(local, transform);
        }
    }

    const Matrix4x4 *const root = FindPreviousNodeTransform(motion, kVFContactRootNode);
    workTransformMatrix = root ? *root : stackTop;
    context.previousContact[slotIndex] = TransformPoint(kVFContactRootOffset, workTransformMatrix);
}

HLib::Task *EnqueueVFCL(VFMAData *game) {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(VFCL_Callback, 8, 2, static_cast<std::int32_t>(kVfclToken));
    if (!task)
        return nullptr;
    if (task->task_data_ptr) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }

    auto *const data = static_cast<VFCLTaskData *>(HLib_AllocHeapMemBlock(0x148, static_cast<std::int32_t>(kTaskDataToken)));
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }
    HLib_EnableTokenCheck(data);
    data->name = kCollisionName;
    data->owner = game;
    game->collisionData = data;
    return task;
}

void VFCL_Callback(HLib::TaskData *taskData) {
    auto *const context = reinterpret_cast<VFCLTaskData *>(taskData);
    if (!context || !context->owner)
        return;


    matrix_stack::Matrix4x4_Push();

    VFMAData *const game = context->owner;
    PrepareNativeFrame(*context, game);

    std::array<VFCLParticipantTmp, kParticipantCapacity> participants{};
    const int participantLen = BuildNativeParticipants(participants.data(), static_cast<int>(participants.size()));

    vfBuildContactsAndResolvePushOrHit(context, participants.data(), participantLen, VFCLPhase::BroadPhase);
    vfBuildContactsAndResolvePushOrHit(context, participants.data(), participantLen, VFCLPhase::AttackDefense);

    vfclEnforceRingBoundary(context);
    vfclAdjustToGroundOrSlope(context);
    ResetNativeAirborneSlots(*context, game);
    vfResolveTerrainColi(context);
    vfUpdateStageObjects(game);
    (void)vfclResolvePairedContactDrift(game);
    vfResolveTerrainColi(context);
    check_radius_intersections();
    (void)barelyUsed(game);

    Matrix4x4::Pop(1);
}

} // namespace shenmue::s1
