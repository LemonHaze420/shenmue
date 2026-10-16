#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

// DC S2: HLib_Task : 0x40

namespace shenmue::s2::HLib {

inline constexpr std::size_t DYNAMIC_HEAP_SIZE = 0x1c42000;

struct TaskData {
    shen_name name;
    std::int32_t unknown_04;
    std::uint64_t contentSize;
    TaskData *previous;
    TaskData *next;
    shen_name status;
    std::int32_t alignment;
    void *pointer;
    std::int32_t unknown_30[4];
};

struct StaticStorageEntry {
    TaskData header;
    std::byte data[0x40];
};

using StorageEntry = StaticStorageEntry;
using TaskCallback = void (*)(void *taskData);

struct CompiledEventSchedule {
    std::int32_t mainExecutionIterations;
    std::int32_t reserved;
};

struct LoopedTaskSchedule {
    std::uint16_t loopIntervalFrames;
    std::uint16_t remainingLoops;
    std::uint32_t reserved;
};

struct TimedTaskSchedule {
    std::uint32_t reserved;
    float timeUntilExecution;
};

struct Task {
    TaskCallback activeCallback;
    std::int32_t taskToken;
    std::uint8_t taskIndex;
    std::uint8_t nextTaskIndex;
    std::uint8_t freeTaskDataOnDestroy;
    std::uint8_t unused_0f;
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

#ifndef _DREAMCAST
static_assert(sizeof(TaskData) == 0x40);
static_assert(offsetof(TaskData, status) == 0x20);
static_assert(sizeof(StaticStorageEntry) == 0x80);
static_assert(offsetof(StaticStorageEntry, data) == 0x40);
static_assert(sizeof(CompiledEventSchedule) == 8);
static_assert(sizeof(LoopedTaskSchedule) == 8);
static_assert(sizeof(TimedTaskSchedule) == 8);
static_assert(sizeof(Task) == 0x70);
static_assert(offsetof(Task, taskIndex) == 0x0c);
static_assert(offsetof(Task, next) == 0x10);
static_assert(offsetof(Task, userData) == 0x48);
static_assert(offsetof(Task, loopInterval) == 0x50);
static_assert(offsetof(Task, delayedCallback) == 0x58);
static_assert(offsetof(Task, taskData) == 0x68);

#endif

extern Task *CurrentTask;
extern Task *NextTask;
extern std::byte *DynamicHeapBase;

void InitializeAllocator();
void *AllocateTaskData(std::uint64_t size, shen_name name);
void *AllocateStaticOrDynamicBlock(std::uint64_t size, shen_name name);
std::int64_t AllocateHeapSpace(void *taskData, std::uint64_t size, shen_name name);
std::int64_t FreeTaskData(void *taskData);
std::int64_t FreeHeapSpace(void *taskData, std::int32_t taskName);
void *AnalyzeFreeHeap(std::uint64_t *totalFree, std::uint64_t *largestContiguousFreeBlock, std::uint64_t *totalFreeExcludingLargest);
std::int64_t GetFreeHeapStats(std::uint64_t *totalFreeSpace, std::uint64_t *largestFree);
std::int64_t TryGetFreeHeapStats(std::uint64_t *totalFreeSpace, std::uint64_t *largestFree);
std::uint64_t WouldAllocationFail(std::uint64_t requiredForRegion, std::int64_t requiredForHeap);
void *AnalyzeFreeList(std::uint64_t *totalUsable, std::uint64_t *largestBlock, std::uint64_t *fragmentationOut);
void *PartitionSceneHeap();
void ValidateOrResetHeap();
void *FreeUnusedHeapMem();
void AssignDynamicAlignmentForTaskData(void *taskData);
void SetTaskDataMotionFlag(void *taskData);

void ClearTaskDataMark(void *taskData);
void *GetDynamicUseStoragePtr();
void *GetPointerFromDynamicHeapOffsetBase(std::int32_t offset);
std::int32_t GetDynamicHeapOffset(const void *pointer);
std::uint32_t GetTaskDataName(const void *taskData);
std::uint64_t GetTaskDataSize(const void *taskData);
bool ShrinkTaskData(void *taskData, std::uint64_t size);

Task *InitializeTasks(TaskCallback callback);
void MainLoop();
Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::uint8_t taskFlags, std::int32_t taskToken, char *description);
Task *EnqueueTaskWithParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::uint64_t size, shen_name taskToken);
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken);
std::int64_t DestroyTaskInternal(Task *task);
std::int64_t DestroyTask(Task *task);
std::int64_t DestroyCurrentTask();
void PauseTaskRangeExecution(std::uint16_t firstTaskIndex, std::uint16_t lastTaskIndex);
void UnpauseTaskEnqueue();
void *GetTaskParameter(Task *task);
Task *GetCurrentTask();
bool HasActiveTaskCallback(const Task *task);
Task *GetCurrentParentTask();
void *GetCurrentTaskData();
std::uint8_t GetCurrentTaskIndex();
std::int64_t ExecuteCallbackNow(TaskCallback callback, void *taskData);
void ReplayPreservedCallbacks();
void ResetPreservedCallbacks();
Task *FindNextTaskInQueueByIndexWithDepth(std::uint8_t nextTaskIndex, std::int32_t searchDepth);
std::int64_t AssignTaskDataToTask(Task *task, void *taskData);
std::int64_t SetCurrentTaskData(void *taskData);
void SetCurrentTaskTo(TaskCallback callback);
void AssignCallbackForTask(Task *task, TaskCallback callback);
Task *MarkCurrentTaskForCleanup();
Task *HLKillOtherHierarchyTask(Task *task);
void EV_HLKillOtherHierarchyTask(Task *task);
std::int64_t DestroyTasksFromIndex(std::uint8_t taskIndex);
void FreeCurrentTaskData();
std::int64_t SetCurrentTaskCustomUserData(TaskCallback callback, void *callbackData);
void ProcessTaskPostUpdateCallbacks();
void SetupTimedTask(Task *task, std::int32_t tickTime);
void SetupTaskDelay(std::int32_t tickTime);
void UpdateLoopedTask(void *taskData);
void UpdateTimedTask(void *taskData);
void ResolveDeferredCallback(Task *task);

void SetScaledDeltaTick(float ticks);
float GetScaledDeltaTick();

} // namespace shenmue::s2::HLib

namespace shenmue::s2 {



void *AllocateTASK(HLib::Task *task, std::uint64_t size);

void FreeTASK(HLib::Task *task);

} // namespace shenmue::s2
