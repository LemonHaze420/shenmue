#include "s1/task/cstc.hpp"

#include "common/types.hpp"

#include <bit>
#include <cstdint>

namespace shenmue::s1 {

void HLib_InertTaskCallback(HLib::TaskData *taskData);

namespace {

constexpr std::int16_t kCstcQueue = 1;
constexpr std::uint16_t kCstcOrder = 2;
constexpr std::int32_t kCstcToken = static_cast<std::int32_t>(SHEN_NAME(CSTC));
constexpr std::int32_t kTaskDataToken = static_cast<std::int32_t>(SHEN_NAME(TASK));
static_assert(kCstcToken == SHEN_NAME(CSTC));
static_assert(kTaskDataToken == SHEN_NAME(TASK));

HLib::Task *g_cstcTask = nullptr;

void DestroyCurrentTaskOnNextDispatch(HLib::Task *task) {
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace



HLib::Task *CSTC_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(HLib_InertTaskCallback, kCstcQueue, kCstcOrder, kCstcToken);
    if (!task) {
        g_cstcTask = nullptr;
        return nullptr;
    }
    if (task->task_data_ptr) {
        DestroyCurrentTaskOnNextDispatch(task);
        g_cstcTask = nullptr;
        return nullptr;
    }

    void *const data = HLib_AllocHeapMemBlock(0, kTaskDataToken);
    if (data)
        HLib_EnableTokenCheck(data);
    task->task_data_ptr = static_cast<HLib::TaskData *>(data);
    if (!data) {
        DestroyCurrentTaskOnNextDispatch(task);
        g_cstcTask = nullptr;
        return nullptr;
    }

    g_cstcTask = task;
    return task;
}

void CSTC_Destroy() {
    if (!g_cstcTask)
        return;
    HLib_DestroyTask(g_cstcTask);
    g_cstcTask = nullptr;
}

} // namespace shenmue::s1
