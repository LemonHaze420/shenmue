#include "s1/task/motf.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
namespace {

constexpr std::int16_t kMotionFreeQueue = 12;
constexpr std::uint16_t kMotionFreePriority = 2;
constexpr std::int32_t kMotionFreeTaskToken = static_cast<std::int32_t>(SHEN_NAME(MOTF));
constexpr std::int32_t kTaskDataToken = static_cast<std::int32_t>(SHEN_NAME(TASK));
constexpr std::int32_t kUnfoAllocationToken = static_cast<std::int32_t>(SHEN_NAME(UNFO));
static_assert(kMotionFreeTaskToken == SHEN_NAME(MOTF));
static_assert(kTaskDataToken == SHEN_NAME(TASK));
static_assert(kUnfoAllocationToken == SHEN_NAME(UNFO));

struct MotionFreeTaskData {
    std::uint32_t reserved_00;
};

static_assert(sizeof(MotionFreeTaskData) == 0x04);

HLib::Task *g_motionFreeTask = nullptr;

} // namespace

InvalidatedMotionRange *g_invalidatedMotionRange = nullptr;

void MOTF_DrainInvalidatedRanges() {
    InvalidatedMotionRange *range = g_invalidatedMotionRange;
    while (range) {
        InvalidatedMotionRange *const next = range->next;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(range), 0);
        range = next;
    }
    g_invalidatedMotionRange = nullptr;
}

bool MOTF_RegisterInvalidatedRange(std::uint16_t firstAnimId, std::uint16_t endAnimId) {

    auto *const range = static_cast<InvalidatedMotionRange *>(HLib_AllocMemBlock(sizeof(InvalidatedMotionRange), kUnfoAllocationToken));
    if (!range)
        return false;

    range->endAnimId = endAnimId;
    range->firstAnimId = firstAnimId;
    range->reserved_04 = 0;
    range->next = nullptr;
    // ReallocateUNFO walks the old list by advancing the global head itself,
    // leaving it null before publishing the newly allocated range.
    g_invalidatedMotionRange = range;
    return true;
}

bool MOTF_IsAnimationInvalidated(std::uint16_t animationId) {
    for (const InvalidatedMotionRange *range = g_invalidatedMotionRange; range; range = range->next) {
        if (animationId >= range->firstAnimId && animationId <= range->endAnimId) {
            return true;
        }
    }
    return false;
}

void MotionFree_Callback(HLib::TaskData *) {
    MOTF_DrainInvalidatedRanges();
}

HLib::Task *MOTF_GetTask() {
    return g_motionFreeTask;
}

HLib::Task *MOTF_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(MotionFree_Callback, kMotionFreeQueue, kMotionFreePriority, kMotionFreeTaskToken);
    if (!task) {
        g_motionFreeTask = nullptr;
        return nullptr;
    }

    if (task->task_data_ptr) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        g_motionFreeTask = nullptr;
        return nullptr;
    }

    void *const data = HLib_AllocHeapMemBlock(sizeof(MotionFreeTaskData), kTaskDataToken);
    task->task_data_ptr = static_cast<HLib::TaskData *>(data);
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        g_motionFreeTask = nullptr;
        return nullptr;
    }

    HLib_EnableTokenCheck(data);
    g_motionFreeTask = task;
    return task;
}

void MOTF_Shutdown() {
    MOTF_DrainInvalidatedRanges();
    if (g_motionFreeTask)
        HLib_DestroyTask(g_motionFreeTask);
    g_motionFreeTask = nullptr;
}

} // namespace shenmue::s1
