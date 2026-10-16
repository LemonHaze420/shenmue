#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

namespace HLib {

#if defined(SHENMUE_PLATFORM_DREAMCAST)

static_assert(sizeof(void *) == 4, "The recovered Dreamcast HLib ABI requires 32-bit pointers");

struct TaskData {
    shen_name taskToken;
    std::uint32_t content_size;
    TaskData *prev_ptr;
    TaskData *next_ptr;
    shen_name status;
    std::uint32_t alignment;
    std::uint32_t field_18;
    std::uint32_t field_1c;
};

using TaskCallback = void (*)(TaskData *);
using TaskFunction = void (*)(std::int64_t);

struct Task {
    TaskCallback mainTaskCallbackFnPtr;
    std::int16_t taskIndex;
    std::int16_t r8b;
    Task *next;
    Task *nextInQueue;
    Task *parent;
    Task *child;
    Task *cleanupParent;
    Task *prevSibling;
    Task *nextSibling;
    TaskFunction taskFn;
    std::uint32_t mainExecutionIterations;
    std::uint32_t dReserved;
    TaskCallback initCallbackFnPtr;
    TaskCallback autoInitTaskCallbackFnPtr;
    std::uint32_t InitialCleanupCount;
    std::uint32_t AutoCleanupCount;
    TaskData *task_data_ptr;
};

static_assert(sizeof(TaskData) == 0x20);
static_assert(offsetof(TaskData, status) == 0x10);
static_assert(sizeof(Task) == 0x44);
static_assert(offsetof(Task, task_data_ptr) == 0x40);

#else

struct TaskData {
    std::int32_t taskToken;
    std::int32_t field_4;
    std::int64_t content_size;
    TaskData *prev_ptr;
    TaskData *next_ptr;
    std::int32_t status;
    std::int32_t alignment;
    void *task_data;
    std::uint8_t *data[2];
};

using TaskCallback = void (*)(TaskData *);
using TaskFunction = void (*)(std::int64_t);

struct Task {
    TaskCallback mainTaskCallbackFnPtr;
    std::int32_t taskToken;
    std::int16_t taskIndex;
    std::int16_t r8b;
    Task *next;
    Task *nextInQueue;
    Task *parent;
    Task *child;
    Task *cleanupParent;
    Task *prevSibling;
    Task *nextSibling;
    TaskFunction taskFn;
    std::uint32_t mainExecutionIterations;
    std::uint32_t dReserved;
    TaskCallback initCallbackFnPtr;
    TaskCallback autoInitTaskCallbackFnPtr;
    std::int32_t InitialCleanupCount;
    std::int32_t AutoCleanupCount;
    TaskData *task_data_ptr;
};

static_assert(sizeof(TaskData) == 0x40);
static_assert(offsetof(TaskData, status) == 0x20);
static_assert(sizeof(Task) == 0x78);
static_assert(offsetof(Task, task_data_ptr) == 0x70);

#endif

struct TaskQueue {
    Task rootTask;
    Task tasks[299];
};

struct TasksInfo {
    Task *moduleTaskQueue[16];
    std::int32_t bTaskPauseEnqueue;
    std::int16_t CurrTaskIdx;
    std::int16_t NextTaskIdx;
    Task *pPreviousTask;
    Task *pCurrentNextInQueue;
    Task *pPreviousTaskNextInQueue;
    Task *pCurrentNextInQueueNextTask;
    std::int32_t CurrentTaskIdx;
    std::int32_t unknown_d3t_flag;
    Task *pWeatherTasks[32];
    TaskQueue taskQueue;
    Task *pWeatherTaskModule[32];
};

#if defined(SHENMUE_PLATFORM_DREAMCAST)
static_assert(sizeof(TaskQueue) == 0x4fb0);
static_assert(offsetof(TasksInfo, taskQueue) == 0xe0);
static_assert(offsetof(TasksInfo, pWeatherTaskModule) == 0x5090);
static_assert(sizeof(TasksInfo) == 0x5110);
#else
static_assert(sizeof(TaskQueue) == 0x8ca0);
static_assert(offsetof(TasksInfo, taskQueue) == 0x1b0);
static_assert(offsetof(TasksInfo, pWeatherTaskModule) == 0x8e50);
static_assert(sizeof(TasksInfo) == 0x8f50);
#endif

extern Task *NextTask;

} // namespace HLib

extern HLib::TasksInfo g_TaskInfo;
extern HLib::Task *CurrentTask;
extern HLib::TaskData *g_pFirstTaskData;
extern std::byte *g_pCurrentTaskData;
extern HLib::TaskData *g_pFirstTaskData_0;
extern std::int32_t HLib_DeltaTime;
extern HLib::TaskData *g_pLastTaskData;
extern HLib::TaskData *g_pLastTaskData_0;
extern std::byte *g_renderTaskBase;

extern HLib::TaskData *g_mallocFirst;
extern HLib::TaskData *g_mallocLast;

void HLib_TaskQueueSetNONE();
std::int64_t HLib_UpdateFrameTiming();
void HLib_Init();
std::uint64_t HLib_Malloc();
std::int64_t HLib_CanAllocate64ByteBlock(std::int32_t blockCount);
std::uint64_t HLib_GetLargestMallocFreeBlock();
void HLib_FreeUseSpaces();
void *UpdateTaskData_0();
void *HLib_AllocHeapMemBlock(std::int64_t size, std::int32_t token);
void *HLib_AllocMemBlock(std::uint64_t size, std::int32_t token);
HLib::Task *HLib_EnqueueTaskWithoutParameter(HLib::TaskCallback callbackFunction, std::int16_t nextFunctionIndex, std::uint16_t r8b, std::int32_t taskToken);
std::int64_t HLib_DestroyTask(HLib::Task *task);
std::int64_t HLib_DestroyCurrentTask();
void HLib_PauseTaskRangeExecution(std::uint16_t firstIndex, std::uint16_t lastIndex);
void HLib_UnpauseTaskEnqueue();
HLib::TaskData *HLib_GetTaskDataForTask(HLib::Task *task);
std::uint32_t HLib_GetRenderTaskBaseOffset();
std::uint32_t HLib_GetTaskDataOffset(const void *pointer);
void HLib_SetTaskCallbackToInitCallback(HLib::TaskData *);
void HLib_ProcessGenericAutoCleanup();
void SetMainCallbackToInitForCleanup(HLib::Task *task);
void MODU_DestroyAllTasks(HLib::Task *task);
void ExecuteWeatherTaskQueue_Internal();
void HLib_EnableTokenCheck(void *taskData);
void HLib_DisableTokenCheck(void *taskData);

void ROOT_Callback(HLib::TaskData *params);
std::uint64_t HLib_FreeTaskData(HLib::TaskData *taskData, std::int32_t token);

} // namespace shenmue::s1
