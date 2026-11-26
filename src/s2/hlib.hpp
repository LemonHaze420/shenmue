#pragma once

#include "common/types.hpp"
#include "shenmue.h"

#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif

#if SHENMUE_S2_GCC295
#define SHENMUE_HLIB_CONSTANT static const
#define SHENMUE_HLIB_LAYOUT_NAME_(line) HLibLayoutCheck##line
#define SHENMUE_HLIB_LAYOUT_NAME(line) SHENMUE_HLIB_LAYOUT_NAME_(line)
#define SHENMUE_HLIB_LAYOUT(condition) typedef char SHENMUE_HLIB_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#else
#define SHENMUE_HLIB_CONSTANT inline constexpr
#define SHENMUE_HLIB_LAYOUT(condition) static_assert(condition)
#endif

// DC S2: HLib_Task : 0x40

namespace shenmue {
namespace s2 {
namespace HLib {

#if SHENMUE_IS_DC
#if SHENMUE_IS_DC && SHENMUE_IS_S2
SHENMUE_HLIB_CONSTANT std::uintptr_t DYNAMIC_HEAP_ADDRESS = 0x0c360000;
#else
SHENMUE_HLIB_CONSTANT std::uintptr_t DYNAMIC_HEAP_ADDRESS = 0x8c360000;
#endif
SHENMUE_HLIB_CONSTANT std::size_t DYNAMIC_HEAP_SIZE = 0xb60000;
#else
SHENMUE_HLIB_CONSTANT std::size_t DYNAMIC_HEAP_SIZE = 0x1c42000;
#endif

struct TaskData {
    shen_name name;
#if SHENMUE_IS_DC
    std::uint32_t contentSize;
#else
    std::int32_t unknown_04;
    std::uint64_t contentSize;
#endif
    TaskData *previous;
    TaskData *next;
    shen_name status;
    std::int32_t alignment;
    void *pointer;
#if SHENMUE_IS_DC
    std::int32_t unknown_1c;
#else
    std::int32_t unknown_30[4];
#endif
};

struct StaticStorageEntry {
    TaskData header;
    std::byte data[0x40];
};

#if SHENMUE_S2_GCC295
typedef StaticStorageEntry StorageEntry;
typedef void (*TaskCallback)(void *taskData);
#else
using StorageEntry = StaticStorageEntry;
using TaskCallback = void (*)(void *taskData);
#endif

struct CompiledEventSchedule {
    std::int32_t mainExecutionIterations;
    std::int32_t reserved;
};

struct LoopedTaskSchedule {
    std::uint16_t loopIntervalFrames;
#if SHENMUE_IS_DC
    std::int16_t remainingLoops;
#else
    std::uint16_t remainingLoops;
#endif
    std::uint32_t reserved;
};

struct TimedTaskSchedule {
    std::uint32_t reserved;
#if SHENMUE_IS_DC
    std::int32_t timeUntilExecution;
#else
    float timeUntilExecution;
#endif
};

struct Task {
    TaskCallback activeCallback;
    std::int32_t taskToken;
    std::uint8_t taskIndex;
    std::uint8_t nextTaskIndex;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::uint16_t freeTaskDataOnDestroy;
#else
    std::uint8_t freeTaskDataOnDestroy;
    std::uint8_t unused_0f;
#endif
    Task *next;
    Task *nextInQueue;
    Task *parent;
    Task *child;
    Task *cleanupParent;
    Task *previousSibling;
    Task *nextSibling;

    // DC/XB -0x12
    union {
        void *userData;
        TaskCallback postUpdateCallback;
    };
    union {
        std::uint64_t loopInterval;
        CompiledEventSchedule compiledEvent;
        LoopedTaskSchedule loopedTask;
        TimedTaskSchedule timedTask;
    };
    TaskCallback delayedCallback;
    TaskCallback periodicCallback;
    void *taskData;
};

#if SHENMUE_IS_DC
SHENMUE_HLIB_LAYOUT(sizeof(TaskData) == 0x20);
SHENMUE_HLIB_LAYOUT(offsetof(TaskData, contentSize) == 0x04);
SHENMUE_HLIB_LAYOUT(offsetof(TaskData, status) == 0x10);
SHENMUE_HLIB_LAYOUT(offsetof(TaskData, alignment) == 0x14);
SHENMUE_HLIB_LAYOUT(sizeof(Task) == 0x40);
SHENMUE_HLIB_LAYOUT(offsetof(Task, taskIndex) == 0x08);
SHENMUE_HLIB_LAYOUT(offsetof(Task, next) == 0x0c);
SHENMUE_HLIB_LAYOUT(offsetof(Task, userData) == 0x28);
SHENMUE_HLIB_LAYOUT(offsetof(Task, loopInterval) == 0x2c);
SHENMUE_HLIB_LAYOUT(offsetof(Task, delayedCallback) == 0x34);
SHENMUE_HLIB_LAYOUT(offsetof(Task, taskData) == 0x3c);
#else
SHENMUE_HLIB_LAYOUT(sizeof(TaskData) == 0x40);
SHENMUE_HLIB_LAYOUT(offsetof(TaskData, status) == 0x20);
SHENMUE_HLIB_LAYOUT(sizeof(StaticStorageEntry) == 0x80);
SHENMUE_HLIB_LAYOUT(offsetof(StaticStorageEntry, data) == 0x40);
SHENMUE_HLIB_LAYOUT(sizeof(CompiledEventSchedule) == 8);
SHENMUE_HLIB_LAYOUT(sizeof(LoopedTaskSchedule) == 8);
SHENMUE_HLIB_LAYOUT(sizeof(TimedTaskSchedule) == 8);
SHENMUE_HLIB_LAYOUT(sizeof(Task) == 0x70);
SHENMUE_HLIB_LAYOUT(offsetof(Task, taskIndex) == 0x0c);
SHENMUE_HLIB_LAYOUT(offsetof(Task, next) == 0x10);
SHENMUE_HLIB_LAYOUT(offsetof(Task, userData) == 0x48);
SHENMUE_HLIB_LAYOUT(offsetof(Task, loopInterval) == 0x50);
SHENMUE_HLIB_LAYOUT(offsetof(Task, delayedCallback) == 0x58);
SHENMUE_HLIB_LAYOUT(offsetof(Task, taskData) == 0x68);

#endif

extern Task *CurrentTask;
extern Task *NextTask;
extern std::byte *DynamicHeapBase;

void InitializeAllocator();
void *AllocateTaskData(std::size_t size, shen_name name);
void *AllocateStaticOrDynamicBlock(std::size_t size, shen_name name);
std::intptr_t AllocateHeapSpace(void *taskData, std::size_t size, shen_name name);
std::intptr_t FreeTaskData(void *taskData);
std::intptr_t FreeHeapSpace(void *taskData, std::int32_t taskName);
void *AnalyzeFreeHeap(std::size_t *totalFree, std::size_t *largestContiguousFreeBlock, std::size_t *totalFreeExcludingLargest);
std::intptr_t GetFreeHeapStats(std::size_t *totalFreeSpace, std::size_t *largestFree);
std::intptr_t TryGetFreeHeapStats(std::size_t *totalFreeSpace, std::size_t *largestFree);
std::size_t WouldAllocationFail(std::size_t requiredForRegion, std::intptr_t requiredForHeap);
void *AnalyzeFreeList(std::size_t *totalUsable, std::size_t *largestBlock, std::size_t *fragmentationOut);
void *PartitionSceneHeap();
void ValidateOrResetHeap();
void *FreeUnusedHeapMem();
void AssignDynamicAlignmentForTaskData(void *taskData);
void SetTaskDataMotionFlag(void *taskData);
// Native vfMarkFlag: clears bit 0x10 of the task-data header's alignment word before the owner frees it.
void ClearTaskDataMark(void *taskData);
void *GetDynamicUseStoragePtr();
void *GetPointerFromDynamicHeapOffsetBase(std::int32_t offset);
std::int32_t GetDynamicHeapOffset(const void *pointer);
std::uint32_t GetTaskDataName(const void *taskData);
std::size_t GetTaskDataSize(const void *taskData);
bool ShrinkTaskData(void *taskData, std::size_t size);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeTasks(TaskCallback callback);
#else
Task *InitializeTasks(TaskCallback callback);
#endif
void MainLoop();
#if SHENMUE_IS_DC
Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken);
#else
Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
#endif
#if SHENMUE_IS_DC
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::int8_t taskFlags, std::int32_t taskToken);
#else
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::uint8_t taskFlags, std::int32_t taskToken, char *description);
#endif
#if SHENMUE_IS_DC
Task *EnqueueTaskWithParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken);
#else
Task *EnqueueTaskWithParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::size_t size, shen_name taskToken);
#endif
#if SHENMUE_S2_GCC295
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken);
#else
#if SHENMUE_S2_GCC295
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken);
#else
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
#endif
#endif
#if SHENMUE_S2_GCC295
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken);
#else
#if SHENMUE_S2_GCC295
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken);
#else
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
#endif
#endif
std::intptr_t DestroyTaskInternal(Task *task);
std::intptr_t DestroyTask(Task *task);
std::intptr_t DestroyCurrentTask();
#if SHENMUE_IS_DC
void DestroyCurrentEventTask();
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PauseTaskRangeExecution(std::int32_t firstTaskIndex, std::int32_t lastTaskIndex);
#else
void PauseTaskRangeExecution(std::uint16_t firstTaskIndex, std::uint16_t lastTaskIndex);
#endif
void UnpauseTaskEnqueue();
void *GetTaskParameter(Task *task);
Task *GetCurrentTask();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t HasActiveTaskCallback(const Task *task);
#else
bool HasActiveTaskCallback(const Task *task);
#endif
Task *GetCurrentParentTask();
void *GetCurrentTaskData();
#if SHENMUE_S2_GCC295
std::int8_t GetCurrentTaskIndex();
#else
#if SHENMUE_S2_GCC295
std::int8_t GetCurrentTaskIndex();
#else
std::uint8_t GetCurrentTaskIndex();
#endif
#endif
std::intptr_t ExecuteCallbackNow(TaskCallback callback, void *taskData);
void ReplayPreservedCallbacks();
void ResetPreservedCallbacks();
Task *FindNextTaskInQueueByIndexWithDepth(std::uint8_t nextTaskIndex, std::int32_t searchDepth);
std::intptr_t AssignTaskDataToTask(Task *task, void *taskData);
std::intptr_t SetCurrentTaskData(void *taskData);
void SetCurrentTaskTo(TaskCallback callback);
void AssignCallbackForTask(Task *task, TaskCallback callback);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MarkCurrentTaskForCleanup();
#else
Task *MarkCurrentTaskForCleanup();
#endif
Task *HLKillOtherHierarchyTask(Task *task);
void EV_HLKillOtherHierarchyTask(Task *task);
std::intptr_t DestroyTasksFromIndex(std::uint8_t taskIndex);
void FreeCurrentTaskData();
std::intptr_t SetCurrentTaskCustomUserData(TaskCallback callback, void *callbackData);
void ProcessTaskPostUpdateCallbacks();
void SetupTimedTask(Task *task, std::int32_t tickTime);
void SetupTaskDelay(std::int32_t tickTime);
void UpdateLoopedTask(void *taskData);
void UpdateTimedTask(void *taskData);
void ResolveDeferredCallback(Task *task);

void SetScaledDeltaTick(float ticks);
float GetScaledDeltaTick();

#if SHENMUE_IS_DC
struct TaskTreeDescriptor {
    TaskCallback callback;
    shen_name taskToken;
    std::int8_t taskIndex;
    std::int8_t taskFlags;
    std::size_t size;
    const TaskTreeDescriptor *child;
    const TaskTreeDescriptor *sibling;
};
SHENMUE_HLIB_LAYOUT(sizeof(TaskTreeDescriptor) == 0x18);
SHENMUE_HLIB_LAYOUT(offsetof(TaskTreeDescriptor, size) == 0x0c);

void *FindTaskDataByName(std::uint32_t name);
void ReportAllocationFailure(std::size_t requestedSize, shen_name name, TaskData *block);
void SetHeapLowerBound(void *taskData);
void SetHeapUpperBound(void *taskData);
Task *EnqueueTaskRelativeIndex(TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::int32_t taskToken);
Task *QueueTaskAfterTaskSameIndex(Task *task, TaskCallback callback, std::int8_t taskFlags, std::int32_t taskToken);
Task *QueueTaskAfterTaskRelativeIndex(Task *task, TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::int32_t taskToken);
std::intptr_t DestroyTasksAtIndex(std::int8_t taskIndex, std::int8_t nextTaskIndex);
Task *GetTaskNext(const Task *task);
Task *GetTaskNextInQueue(const Task *task);
Task *GetTaskParent(const Task *task);
Task *GetTaskChild(const Task *task);
Task *GetTaskPreviousSibling(const Task *task);
Task *GetTaskNextSibling(const Task *task);
std::int8_t GetTaskIndex(const Task *task);
std::int8_t GetTaskNextTaskIndex(const Task *task);
Task *GetFirstTaskAtIndex(std::int8_t taskIndex);
Task *GetLastTaskAtIndex(std::int8_t taskIndex);
Task *FindTaskAtIndexWithDepth(std::int8_t taskIndex, std::int8_t nextTaskIndex, std::int32_t searchDepth);
void SetCurrentTaskToDestroy();
void SetTaskToDestroy(Task *task);
void ResolveCurrentDeferredCallback();
void SetupCurrentLoopedTask(std::int16_t intervalFrames);
void SetupLoopedTask(Task *task, std::int16_t intervalFrames);
void StopCurrentLoopedTask();
void StopLoopedTask(Task *task);
std::intptr_t TransferTaskData(Task *source, Task *destination);
void SetTaskKeepData(Task *task);
void ClearTaskKeepData(Task *task);
Task *EnqueueTaskWithParameterWrapper(TaskCallback callback, std::int8_t taskFlags, std::size_t size, shen_name taskToken);
Task *EnqueueTaskWithParameterRelativeIndex(TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::size_t size, shen_name taskToken);
Task *QueueTaskWithParameterAfterTask(
    Task *task, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken);
Task *CreateNewCurrentTaskWithParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken);
std::int32_t EnqueueTaskTree(const TaskTreeDescriptor *descriptor);
Task *CreateTaskTree(const TaskTreeDescriptor *descriptor);
#endif

} // namespace HLib
} // namespace s2
} // namespace shenmue

namespace shenmue {
namespace s2 {

// Native AllocateTASK: binds a dynamically aligned 'TASK' heap block as the task's data. Returns null for a null task or one that
// already owns data.
void *AllocateTASK(HLib::Task *task, std::size_t size);
// Native FreeTASK: frees the bound 'TASK' block and clears the binding.
void FreeTASK(HLib::Task *task);
#if SHENMUE_IS_DC
void *AllocateCurrentTASK(std::size_t size);
#endif

} // namespace s2
} // namespace shenmue
#if SHENMUE_S2_GCC295
#undef SHENMUE_HLIB_LAYOUT_NAME
#undef SHENMUE_HLIB_LAYOUT_NAME_
#endif
#undef SHENMUE_HLIB_CONSTANT
#undef SHENMUE_HLIB_LAYOUT
