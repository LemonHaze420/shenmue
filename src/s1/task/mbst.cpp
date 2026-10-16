#include "s1/task/mbst.hpp"

#include "s1/d3t/scratch_memory.hpp"
#include "s1/motion.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <cstdint>

namespace shenmue::s1 {
namespace {

constexpr std::int32_t kTaskDataToken = static_cast<std::int32_t>(SHEN_NAME(TASK));
constexpr std::int32_t kMbstTaskToken = static_cast<std::int32_t>(SHEN_NAME(MBST));
constexpr std::uint32_t kMtmwPayloadToken = SHEN_NAME(MTMW);
constexpr std::uint32_t kCharacterHandlerToken = SHEN_NAME(CEWP);
constexpr std::uint32_t kMotionBudgetCandidate = 2U;
constexpr std::uint32_t kMotionBudgetSelected = 4U;
static_assert(kTaskDataToken == SHEN_NAME(TASK));
static_assert(kMbstTaskToken == SHEN_NAME(MBST));
static_assert(kMtmwPayloadToken == SHEN_NAME(MTMW));
static_assert(kCharacterHandlerToken == SHEN_NAME(CEWP));

HLib::Task *g_mbstTask = nullptr;

MapControlData *CurrentMapControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

template <typename Callback> void ForEachMapCharacter(Callback &&callback) {
    MapControlData *const control = CurrentMapControl();
    if (!control)
        return;
    for (void *const head : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            if (node->character)
                callback(node->character);
        }
    }
}

bool IsRetainedCharacter(const Character *character) {
    if (!character || character->entity.name != kCharacterHandlerToken)
        return false;
    const std::uint32_t flags = character->flags.value;
    return (flags & CharacterFlagUnknown) != 0U || ((flags & 2U) != 0U && (character->motionFlags & 1U) != 0U);
}

bool PassesMotionBudgetView(const Character &character, const Vector3 &position) {
    const Matrix4x4 &view = workTransformMatrix;
    const Vector3 viewPosition{
        position.x * view.rows[0].x + position.y * view.rows[1].x + position.z * view.rows[2].x + view.rows[3].x,
        position.x * view.rows[0].y + position.y * view.rows[1].y + position.z * view.rows[2].y + view.rows[3].y,
        position.x * view.rows[0].z + position.y * view.rows[1].z + position.z * view.rows[2].z + view.rows[3].z,
    };

    const Vector2 &scale = prevScreenSpaceToWorldScale;
    const float clip = character.clipAngle;
    const float depth = -viewPosition.z + clip;
    if (!(depth >= 0.0f))
        return false;
    const float xLimit = depth * scale.x + clip;
    const float yLimit = depth * scale.y + clip;
    return viewPosition.x <= xLimit && -xLimit < viewPosition.x && viewPosition.y <= yLimit && -yLimit < viewPosition.y;
}

float DistanceSquaredToCameraTarget(const Vector3 &position) {
    const float dx = position.x - g_CameraEyePos.x;
    const float dz = position.z - g_CameraEyePos.z;
    return dx * dx + dz * dz;
}

std::int32_t CountReusableMotions() {
    std::int32_t count = 0;
    ForEachMapCharacter([&](Character *character) {
        const MotionManagerWork *const manager = MOTM_GetWork(character);
        if (manager && (manager->flags & (kMotionBudgetCandidate | kMotionBudgetSelected)) == kMotionBudgetCandidate && manager->motion.activeNodes) {
            ++count;
        }
    });
    return count;
}

std::int32_t CaptureReusableMotions(MotionManagerWork **output, std::int32_t capacity) {
    if (!output || capacity <= 0)
        return 0;
    std::int32_t count = 0;
    ForEachMapCharacter([&](Character *character) {
        MotionManagerWork *const manager = MOTM_GetWork(character);
        if (!manager || (manager->flags & (kMotionBudgetCandidate | kMotionBudgetSelected)) != kMotionBudgetCandidate || !manager->motion.activeNodes) {
            return;
        }
        if (count < capacity)
            output[count++] = manager;
    });
    return count;
}

void RefreshMotionBudget(MBST_t *mbst, bool releaseUnselected) {
    if (!mbst)
        return;
    if (mbst->capacity == 0) {
        MBST_ResetActiveMotionRows(mbst);
        return;
    }

    void *const checkpoint = GetGlobalPoolCheckpoint();
    MBSTSelectionEntry fallback{};
    MBSTSelectionEntry *entries = &fallback;
    if (mbst->capacity > 0) {
        const std::size_t allocationSize = static_cast<std::size_t>(mbst->capacity) * sizeof(MBSTSelectionEntry);
        entries = static_cast<MBSTSelectionEntry *>(AllocateMemoryFromGlobalPool(allocationSize));
        if (!entries) {
            RestoreGlobalPoolCheckpoint(checkpoint);
            return;
        }
    }

    const std::int32_t count = MBST_SelectMotionCandidates(mbst, entries, nullptr);
    MBST_CommitMotionCandidates(mbst, entries, count, releaseUnselected);
    RestoreGlobalPoolCheckpoint(checkpoint);
}

void ConfigureTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace

HLib::Task *MBST_GetTask() {
    return g_mbstTask;
}

MBST_t *MBST_GetWork() {
    return g_mbstTask && g_mbstTask->task_data_ptr ? reinterpret_cast<MBST_t *>(g_mbstTask->task_data_ptr) : nullptr;
}

void MBST_ResetActiveMotionRows(MBST_t *mbst) {
    if (!mbst)
        return;
    ForEachMapCharacter([&](Character *character) {
        MotionManagerWork *const manager = MOTM_GetWork(character);
        if (!manager || (manager->flags & kMotionBudgetCandidate) == 0U || !manager->motion.activeNodes) {
            return;
        }
        MOTI_TransferPrimaryRows(&manager->motion, nullptr);
        --mbst->state;
    });
}

bool MBST_IsVisibleMotionCandidate(const Character *character) {
    if (!IsRetainedCharacter(character) || (character->motionFlags & 4U) == 0U) {
        return false;
    }
    const MotionManagerWork *const manager = MOTM_GetWork(const_cast<Character *>(character));
    if (!manager || (manager->flags & kMotionBudgetCandidate) == 0U) {
        return false;
    }

    const Vector3 position = GetPositionForEntity(const_cast<CharacterNode &>(character->entity.node));
    return PassesMotionBudgetView(*character, position);
}

std::int32_t MBST_SelectMotionCandidates(MBST_t *mbst, MBSTSelectionEntry *entries, MotionManagerWork *excludedMotion) {
    if (!mbst || !entries || !CurrentMapControl())
        return 0;

    const std::int32_t capacity = mbst->capacity;
    std::int32_t selected = 0;
    std::int32_t farthestIndex = 0;
    float farthestDistance = -1.0f;

    ForEachMapCharacter([&](Character *character) {
        if (!IsRetainedCharacter(character))
            return;
        MotionManagerWork *const motion = MOTM_GetWork(character);
        if (!motion || (motion->flags & kMotionBudgetCandidate) == 0U) {
            return;
        }




        motion->flags &= ~kMotionBudgetSelected;
        if (motion == excludedMotion || (character->motionFlags & 4U) == 0U) {
            return;
        }

        const Vector3 position = GetPositionForEntity(character->entity.node);
        if (!PassesMotionBudgetView(*character, position))
            return;
        const float distance = DistanceSquaredToCameraTarget(position);
        if (selected < capacity) {
            entries[selected] = {motion, distance, 0};
            motion->flags |= kMotionBudgetSelected;
            if (distance > farthestDistance) {
                farthestDistance = distance;
                farthestIndex = selected;
            }
            ++selected;
            return;
        }

        if (farthestDistance <= distance)
            return;
        if (entries[farthestIndex].motion) {
            entries[farthestIndex].motion->flags &= ~kMotionBudgetSelected;
        }
        entries[farthestIndex] = {motion, distance, 0};
        motion->flags |= kMotionBudgetSelected;

        farthestIndex = 0;
        farthestDistance = entries[0].distanceSquared;
        for (std::int32_t index = 1; index < selected; ++index) {
            if (entries[index].distanceSquared > farthestDistance) {
                farthestDistance = entries[index].distanceSquared;
                farthestIndex = index;
            }
        }
    });
    return selected;
}

bool MBST_CommitMotionCandidates(MBST_t *mbst, const MBSTSelectionEntry *entries, std::int32_t count, bool releaseUnselected) {
    if (!mbst)
        return false;
    if (count > 0 && !entries)
        return false;

    void *const checkpoint = GetGlobalPoolCheckpoint();
    const std::int32_t reusableCapacity = CountReusableMotions();
    auto **reusable = reusableCapacity > 0 ?
        static_cast<MotionManagerWork **>(AllocateMemoryFromGlobalPool(static_cast<std::size_t>(reusableCapacity) * sizeof(MotionManagerWork *))) :
        nullptr;
    const std::int32_t reusableCount = reusable ? CaptureReusableMotions(reusable, reusableCapacity) : 0;

    std::int32_t reusableIndex = 0;
    std::int32_t committed = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        MotionManagerWork *const manager = entries[index].motion;
        if (!manager)
            continue;
        if (!manager->motion.activeNodes) {
            if (reusableIndex < reusableCount) {
                MOTI_TransferPrimaryRows(&reusable[reusableIndex]->motion, &manager->motion);
                ++reusableIndex;
            } else {
                MOTI_TransferPrimaryRows(nullptr, &manager->motion);
                ++mbst->state;
            }
        }
        ++committed;
    }

    if (releaseUnselected) {
        for (std::int32_t index = reusableIndex; index < reusableCount; ++index) {
            MOTI_TransferPrimaryRows(&reusable[index]->motion, nullptr);
            --mbst->state;
        }
    }
    RestoreGlobalPoolCheckpoint(checkpoint);
    return committed != 0;
}

void MBST_Callback(HLib::TaskData *taskData) {
    auto *const mbst = reinterpret_cast<MBST_t *>(taskData);
    if (!mbst)
        return;

    if (mbst->capacity != 0) {
        RefreshMotionBudget(mbst, false);
        return;
    }
    MBST_ResetActiveMotionRows(mbst);
    if (CurrentTask) {
        if (!CurrentTask->initCallbackFnPtr) {
            CurrentTask->initCallbackFnPtr = CurrentTask->mainTaskCallbackFnPtr;
            CurrentTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
            CurrentTask->mainExecutionIterations = 0;
        }
        CurrentTask->mainExecutionIterations = 0;
    }
}

HLib::Task *MBST_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(MBST_Callback, 5, 2, kMbstTaskToken);
    if (!task || task->task_data_ptr) {
        ConfigureTaskForDestruction(task);
        g_mbstTask = nullptr;
        return nullptr;
    }

    auto *const data = static_cast<MBST_t *>(HLib_AllocHeapMemBlock(sizeof(MBST_t), kTaskDataToken));
    if (!data) {
        ConfigureTaskForDestruction(task);
        g_mbstTask = nullptr;
        return nullptr;
    }

    HLib_EnableTokenCheck(data);
    data->taskToken = kMtmwPayloadToken;
    data->capacity = 0;
    data->state = 0;
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    g_mbstTask = task;
    return task;
}

void MBST_SetCapacity(std::int16_t capacity) {
    MBST_t *const mbst = MBST_GetWork();
    if (!mbst)
        return;
    mbst->capacity = capacity;
    RefreshMotionBudget(mbst, true);
}

void MBST_Refresh(bool releaseUnselected) {
    RefreshMotionBudget(MBST_GetWork(), releaseUnselected);
}

void MBST_Shutdown() {
    MBST_ResetActiveMotionRows(MBST_GetWork());
    if (g_mbstTask)
        HLib_DestroyTask(g_mbstTask);
    g_mbstTask = nullptr;
}

void MBST_PrepareMotionManagerCleanup(MotionManagerWork *excludedManager) {
    MBST_t *const mbst = MBST_GetWork();
    if (!mbst)
        return;

    void *const checkpoint = GetGlobalPoolCheckpoint();
    MBSTSelectionEntry fallback{};
    MBSTSelectionEntry *entries = nullptr;
    if (mbst->capacity > 0) {
        entries = static_cast<MBSTSelectionEntry *>(AllocateMemoryFromGlobalPool(static_cast<std::size_t>(mbst->capacity) * sizeof(MBSTSelectionEntry)));
        if (!entries) {
            RestoreGlobalPoolCheckpoint(checkpoint);
            return;
        }
    } else if (mbst->capacity < 0) {


        entries = &fallback;
    }

    std::int32_t count = 0;
    if (entries) {
        count = MBST_SelectMotionCandidates(mbst, entries, excludedManager);
    } else {


        ForEachMapCharacter([](Character *character) {
            if (!IsRetainedCharacter(character))
                return;
            if (MotionManagerWork *const manager = MOTM_GetWork(character); manager && (manager->flags & kMotionBudgetCandidate) != 0U) {
                manager->flags &= ~kMotionBudgetSelected;
            }
        });
    }
    MBST_CommitMotionCandidates(mbst, entries, count, true);
    RestoreGlobalPoolCheckpoint(checkpoint);
}

} // namespace shenmue::s1
