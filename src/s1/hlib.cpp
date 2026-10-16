#include "s1/hlib.hpp"
#include "s1/timing.hpp"

#include <bit>
#include <cstring>

namespace shenmue::s1 {
namespace {

constexpr std::int32_t token(char a, char b, char c, char d) noexcept {
    return static_cast<std::int32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(b)) << 8) |
        (static_cast<std::int32_t>(static_cast<std::uint8_t>(c)) << 16) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(d)) << 24);
}

constexpr std::int32_t kNoneToken = token('N', 'O', 'N', 'E');
constexpr std::int32_t kRootToken = token('R', 'O', 'O', 'T');
constexpr std::int32_t kTaskDataToken = token('T', 'A', 'S', 'K');

} // namespace

HLib::TasksInfo g_TaskInfo{};
HLib::Task *CurrentTask = nullptr;
std::int32_t HLib_DeltaTime = 0;

namespace HLib {
Task *NextTask = nullptr;
}
std::int64_t HLib_UpdateFrameTiming() {
    HLib_DeltaTime = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
    return HLib_DeltaTime;
}

void HLib_TaskQueueSetNONE() {
    constexpr std::size_t taskCount = 300;
    for (std::size_t index = 0; index < taskCount; ++index) {
        HLib::Task &task = index == 0 ? g_TaskInfo.taskQueue.rootTask : g_TaskInfo.taskQueue.tasks[index - 1];
        std::memset(&task, 0, sizeof(task));
#if !defined(SHENMUE_PLATFORM_DREAMCAST)
        task.taskToken = kNoneToken;
#endif
        task.next = index + 1 == taskCount ? g_TaskInfo.taskQueue.tasks + 299 : &g_TaskInfo.taskQueue.tasks[index];
    }

    HLib::Task &root = g_TaskInfo.taskQueue.rootTask;
#if !defined(SHENMUE_PLATFORM_DREAMCAST)
    root.taskToken = kRootToken;
#endif
    root.mainTaskCallbackFnPtr = ROOT_Callback;
    root.taskIndex = 0;
    root.r8b = 1;
    root.next = &root;
    root.nextInQueue = &root;

    HLib::NextTask = g_TaskInfo.taskQueue.tasks;
    for (HLib::Task *&queueTail : g_TaskInfo.moduleTaskQueue) {
        queueTail = &root;
    }
    g_TaskInfo.bTaskPauseEnqueue = 0;
    for (HLib::Task *&weatherTask : g_TaskInfo.pWeatherTaskModule) {
        weatherTask = nullptr;
    }
    g_TaskInfo.CurrentTaskIdx = 0;
    CurrentTask = &root;
}

HLib::Task *HLib_EnqueueTaskWithoutParameter(HLib::TaskCallback callbackFunction, std::int16_t nextFunctionIndex, std::uint16_t r8b, std::int32_t taskToken) {
    if (nextFunctionIndex >= 16 || g_TaskInfo.bTaskPauseEnqueue || !HLib::NextTask) {
        return nullptr;
    }

    HLib::Task *task = HLib::NextTask;
    HLib::NextTask = task->next;

    HLib::Task *previous = g_TaskInfo.moduleTaskQueue[nextFunctionIndex];
    HLib::Task *following = previous->nextInQueue;
    task->nextInQueue = following;
    task->next = previous;
    previous->nextInQueue = task;
    following->next = task;

    task->taskIndex = nextFunctionIndex;
    task->r8b = static_cast<std::int16_t>(r8b);
    task->mainTaskCallbackFnPtr = callbackFunction;
    task->parent = CurrentTask;
    task->child = nullptr;
    task->cleanupParent = nullptr;
    task->initCallbackFnPtr = nullptr;
    task->autoInitTaskCallbackFnPtr = nullptr;
    task->task_data_ptr = nullptr;
    task->nextSibling = nullptr;
#if defined(SHENMUE_PLATFORM_DREAMCAST)
    (void)taskToken;
#else
    task->taskToken = taskToken;
#endif

    for (std::int32_t index = nextFunctionIndex; index < 16; ++index) {
        if (g_TaskInfo.moduleTaskQueue[index] == previous) {
            g_TaskInfo.moduleTaskQueue[index] = task;
        }
    }

    task->prevSibling = CurrentTask->cleanupParent;
    if (CurrentTask->cleanupParent) {
        CurrentTask->cleanupParent->nextSibling = task;
    } else {
        CurrentTask->child = task;
    }
    CurrentTask->cleanupParent = task;
    return task;
}

void HLib_UnpauseTaskEnqueue() {
    if (!g_TaskInfo.bTaskPauseEnqueue) {
        return;
    }
    g_TaskInfo.bTaskPauseEnqueue = 0;
    g_TaskInfo.pPreviousTask->nextInQueue = g_TaskInfo.pPreviousTaskNextInQueue;
    g_TaskInfo.pCurrentNextInQueue->next = g_TaskInfo.pCurrentNextInQueueNextTask;
}

void HLib_PauseTaskRangeExecution(std::uint16_t firstIndex, std::uint16_t lastIndex) {
    if (firstIndex == 0 || lastIndex >= 16 || firstIndex > lastIndex) {
        return;
    }
    if (g_TaskInfo.bTaskPauseEnqueue) {
        HLib_UnpauseTaskEnqueue();
    }

    g_TaskInfo.CurrTaskIdx = static_cast<std::int16_t>(firstIndex);
    g_TaskInfo.NextTaskIdx = static_cast<std::int16_t>(lastIndex);
    g_TaskInfo.bTaskPauseEnqueue = 1;
    g_TaskInfo.pPreviousTask = g_TaskInfo.moduleTaskQueue[firstIndex - 1];
    HLib::Task *const lastTask = g_TaskInfo.moduleTaskQueue[lastIndex];
    g_TaskInfo.pPreviousTaskNextInQueue = g_TaskInfo.pPreviousTask->nextInQueue;
    g_TaskInfo.pCurrentNextInQueue = lastTask->nextInQueue;
    g_TaskInfo.pCurrentNextInQueueNextTask = g_TaskInfo.pCurrentNextInQueue->next;
    g_TaskInfo.pPreviousTask->nextInQueue = g_TaskInfo.pCurrentNextInQueue;
    g_TaskInfo.pCurrentNextInQueue->next = g_TaskInfo.pPreviousTask;
}

std::int64_t HLib_DestroyTask(HLib::Task *task) {
    if (!task || !task->mainTaskCallbackFnPtr) {
        return 0;
    }

    const bool wasPaused = g_TaskInfo.bTaskPauseEnqueue != 0;
    if (wasPaused) {
        HLib_UnpauseTaskEnqueue();
    }

    if (task->taskIndex < 16) {
        for (std::int32_t index = task->taskIndex; index < 16; ++index) {
            if (g_TaskInfo.moduleTaskQueue[index] == task) {
                g_TaskInfo.moduleTaskQueue[index] = task->next;
            }
        }
    }

    task->next->nextInQueue = task->nextInQueue;
    task->nextInQueue->next = task->next;
    task->next = HLib::NextTask;
    HLib::NextTask = task;
    task->mainTaskCallbackFnPtr = nullptr;

    for (HLib::Task *child = task->child; child; child = child->nextSibling) {
        child->parent = nullptr;
    }
    if (task->parent) {
        if (task->parent->child == task) {
            task->parent->child = task->nextSibling;
        }
        if (task->parent->cleanupParent == task) {
            task->parent->cleanupParent = task->prevSibling;
        }
    }
    if (task->prevSibling) {
        task->prevSibling->nextSibling = task->nextSibling;
    }
    if (task->nextSibling) {
        task->nextSibling->prevSibling = task->prevSibling;
    }

    HLib::TaskData *const taskData = task->task_data_ptr;
    task->prevSibling = nullptr;
    task->nextSibling = nullptr;
    task->child = nullptr;
    task->cleanupParent = nullptr;
    if (taskData) {
        HLib_FreeTaskData(taskData, kTaskDataToken);
        task->task_data_ptr = nullptr;
    }
    if (wasPaused) {
        HLib_PauseTaskRangeExecution(static_cast<std::uint16_t>(g_TaskInfo.CurrTaskIdx), static_cast<std::uint16_t>(g_TaskInfo.NextTaskIdx));
    }
    return 1;
}

std::int64_t HLib_DestroyCurrentTask() {
    return HLib_DestroyTask(CurrentTask);
}

HLib::TaskData *HLib_GetTaskDataForTask(HLib::Task *task) {
    return task ? task->task_data_ptr : nullptr;
}

void HLib_SetTaskCallbackToInitCallback(HLib::TaskData *) {
    if (CurrentTask->mainExecutionIterations-- == 1) {
        CurrentTask->mainTaskCallbackFnPtr = CurrentTask->initCallbackFnPtr;
        CurrentTask->initCallbackFnPtr = nullptr;
    }
}

void HLib_ProcessGenericAutoCleanup() {
    if (CurrentTask->AutoCleanupCount-- == 1) {
        CurrentTask->AutoCleanupCount = CurrentTask->InitialCleanupCount;
        CurrentTask->autoInitTaskCallbackFnPtr(CurrentTask->task_data_ptr);
    }
}

void SetMainCallbackToInitForCleanup(HLib::Task *task) {
    if (!task || !task->initCallbackFnPtr) {
        return;
    }
    if (task->autoInitTaskCallbackFnPtr && task->mainTaskCallbackFnPtr == std::bit_cast<HLib::TaskCallback>(&HLib_ProcessGenericAutoCleanup)) {
        task->autoInitTaskCallbackFnPtr = task->initCallbackFnPtr;
        task->initCallbackFnPtr = nullptr;
    } else {
        task->mainTaskCallbackFnPtr = task->initCallbackFnPtr;
        task->initCallbackFnPtr = nullptr;
    }
}

void MODU_DestroyAllTasks(HLib::Task *task) {
    while (task) {
        HLib::Task *const child = task->child;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        if (!child) {
            break;
        }
        HLib::Task *const grandchild = child->child;
        child->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        if (grandchild) {
            MODU_DestroyAllTasks(grandchild);
        }
        task = child->nextSibling;
    }
}

void ExecuteWeatherTaskQueue_Internal() {
    HLib::Task *const previousCurrentTask = CurrentTask;
    std::int32_t taskIndex = 0;
    if (g_TaskInfo.CurrentTaskIdx <= 0) {
        g_TaskInfo.CurrentTaskIdx = 0;
        return;
    }

    do {
        HLib::Task *const parameter = g_TaskInfo.pWeatherTasks[taskIndex];
        CurrentTask = g_TaskInfo.pWeatherTaskModule[taskIndex];
        CurrentTask->taskFn(reinterpret_cast<std::int64_t>(parameter));
        ++taskIndex;
    } while (taskIndex < g_TaskInfo.CurrentTaskIdx);

    g_TaskInfo.CurrentTaskIdx = 0;
    CurrentTask = previousCurrentTask;
}

void HLib_EnableTokenCheck(void *taskData) {
    if (taskData) {
        auto *const header = static_cast<HLib::TaskData *>(taskData) - 1;
        header->alignment |= 0x10;
    }
}

void HLib_DisableTokenCheck(void *taskData) {
    if (taskData) {
        auto *const header = static_cast<HLib::TaskData *>(taskData) - 1;
        header->alignment &= ~0x10;
    }
}

} // namespace shenmue::s1
