#pragma once

#include "common/types.hpp"
#include "shenmue.h"

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
    void (*relocateCallback)(TaskData *, void *);
    std::uint32_t field_1c;
};

using TaskCallback = void (*)(TaskData *);
using TaskFunction = void (*)(std::intptr_t);

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
    TaskCallback initCallbackFnPtr;
    TaskCallback autoInitTaskCallbackFnPtr;
    std::uint32_t InitialCleanupCount;
    std::uint32_t AutoCleanupCount;
    TaskData *task_data_ptr;
};

static_assert(sizeof(TaskData) == 0x20);
static_assert(offsetof(TaskData, status) == 0x10);
static_assert(offsetof(TaskData, relocateCallback) == 0x18);
static_assert(sizeof(Task) == 0x40);
static_assert(offsetof(Task, taskFn) == 0x24);
static_assert(offsetof(Task, mainExecutionIterations) == 0x28);
static_assert(offsetof(Task, initCallbackFnPtr) == 0x2c);
static_assert(offsetof(Task, task_data_ptr) == 0x3c);

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
using TaskFunction = void (*)(std::intptr_t);

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

inline constexpr shen_name TASK_NONE{SHEN_NAME(NONE)};
inline constexpr shen_name TASK_ROOT{SHEN_NAME(ROOT)};
inline constexpr shen_name TASK_TASK{SHEN_NAME(TASK)};
inline constexpr std::size_t kTaskCount = SHENMUE_IS_DEMO ? 400 : 300;

struct TaskQueue {
    Task rootTask;
    Task tasks[kTaskCount - 1];
};

struct TasksInfo {
#if defined(SHENMUE_PLATFORM_DREAMCAST)
    TaskQueue taskQueue;
#endif
    Task *moduleTaskQueue[16];
    std::int32_t bTaskPauseEnqueue;
    std::int16_t CurrTaskIdx;
    std::int16_t NextTaskIdx;
    Task *pPreviousTask;
    Task *pCurrentNextInQueue;
    Task *pPreviousTaskNextInQueue;
    Task *pCurrentNextInQueueNextTask;
    std::int32_t CurrentTaskIdx;
#if !defined(SHENMUE_PLATFORM_DREAMCAST)
    std::int32_t unknown_d3t_flag;
    Task *pWeatherTasks[32];
    TaskQueue taskQueue;
#endif
    Task *pWeatherTaskModule[32];
#if defined(SHENMUE_PLATFORM_DREAMCAST)
    Task *pWeatherTasks[32];
#endif
};

#if defined(SHENMUE_PLATFORM_DREAMCAST)
static_assert(sizeof(TaskQueue) == kTaskCount * 0x40);
static_assert(offsetof(TasksInfo, taskQueue) == 0);
static_assert(offsetof(TasksInfo, moduleTaskQueue) == kTaskCount * 0x40);
static_assert(offsetof(TasksInfo, CurrentTaskIdx) == kTaskCount * 0x40 + 0x58);
static_assert(offsetof(TasksInfo, pWeatherTaskModule) == kTaskCount * 0x40 + 0x5c);
static_assert(offsetof(TasksInfo, pWeatherTasks) == kTaskCount * 0x40 + 0xdc);
static_assert(sizeof(TasksInfo) == kTaskCount * 0x40 + 0x15c);
#else
static_assert(sizeof(TaskQueue) == kTaskCount * 0x78);
static_assert(offsetof(TasksInfo, taskQueue) == 0x1b0);
static_assert(offsetof(TasksInfo, pWeatherTaskModule) == 0x1b0 + kTaskCount * 0x78);
static_assert(sizeof(TasksInfo) == 0x2b0 + kTaskCount * 0x78);
#endif

extern Task *NextTask;

#if SHENMUE_IS_DC
inline constexpr shen_name TASK_DEBUG_SIGNATURE{SHEN_NAME(TAKE)};
extern std::uint32_t g_currentTaskDebugValue;
[[noreturn]] void ExecuteTasks();
inline constexpr std::uintptr_t kHeapBegin = SHENMUE_IS_DEMO ? 0x0c2e0000 : 0x0c2ca000;
inline constexpr std::uintptr_t kHeapEnd = SHENMUE_IS_DEMO ? 0x0cd00000 : 0x0ced0000;

enum HeapBlockFlag : std::uint32_t {
    KeepOnReset = 0x01,
    Relocatable = 0x02,
    AllocatedFromEnd = 0x04,
    Relocated = 0x08,
    RelocationDisabled = 0x80,
};
#endif

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
// Native g_hlib_end_ptrs[0] and g_hlib_last_data_ptr: the HLib_Malloc arena's first block and its closing block.
extern HLib::TaskData *g_mallocFirst;
extern HLib::TaskData *g_mallocLast;

void HLib_TaskQueueSetNONE();
#if SHENMUE_IS_DC
void HLib_InitTasks(HLib::TaskCallback rootCallback);
HLib::Task *HLib_EnqueueTaskWithParameter(HLib::TaskCallback callback, std::int16_t queue, std::uint16_t order, std::uint32_t size);
HLib::Task *HLib_FindTaskByOrder(std::int16_t order, std::int32_t occurrence);
void HLib_DelayTask(HLib::Task *task, std::uint32_t frames);
void HLib_ResumeTask(HLib::Task *task);
std::int32_t HLib_QueueWeatherCallback(HLib::TaskFunction callback, HLib::Task *parameter);
std::int32_t HLib_DestroyTasksInQueue(std::int16_t queue, std::int16_t order);
std::int32_t HLib_DestroyTasksFromQueue(std::int32_t firstQueue);
std::int32_t HLib_SetTaskData(HLib::Task *task, HLib::TaskData *data);
std::int32_t HLib_TransferTaskData(HLib::Task *source, HLib::Task *destination);
#endif
std::int64_t HLib_UpdateFrameTiming();
void HLib_Init();
#if SHENMUE_IS_DC
void HLib_AllocationFailed();
void HLib_KeepMemBlockOnReset(void *data);
std::int32_t HLib_ShrinkMemBlock(void *data, std::uint32_t size);
std::uint32_t HLib_GetAllocationToken(const void *data);
HLib::TaskData *HLib_GetNextAllocation(HLib::TaskData *data);
std::uint32_t HLib_GetAllocationSize(const void *data);
void HLib_GetHeapStats(std::uint32_t *total, std::uint32_t *largest, std::uint32_t *fragmented);
#endif
std::uint64_t HLib_Malloc();
std::int64_t HLib_CanAllocate64ByteBlock(std::int32_t blockCount);
std::uint64_t HLib_GetLargestMallocFreeBlock();
void HLib_FreeUseSpaces();
void *UpdateTaskData_0();
#if SHENMUE_IS_DC
void *HLib_AllocHeapMemBlock(std::uint32_t size, std::int32_t token);
void *HLib_AllocMemBlock(std::uint32_t size, std::int32_t token);
#else
void *HLib_AllocHeapMemBlock(std::int64_t size, std::int32_t token);
void *HLib_AllocMemBlock(std::uint64_t size, std::int32_t token);
#endif
HLib::Task *HLib_EnqueueTaskWithoutParameter(HLib::TaskCallback callbackFunction, std::int16_t nextFunctionIndex, std::uint16_t r8b, std::int32_t taskToken);
std::int64_t HLib_DestroyTask(HLib::Task *task);
std::int64_t HLib_DestroyCurrentTask();
void HLib_PauseTaskRangeExecution(std::uint16_t firstIndex, std::uint16_t lastIndex);
void HLib_UnpauseTaskEnqueue();
#if SHENMUE_IS_DC
HLib::TaskData *HLib_GetTaskDataForTask(HLib::Task *task) asm("_shenmue_dc_task_data");
#else
HLib::TaskData *HLib_GetTaskDataForTask(HLib::Task *task);
#endif
HLib::TaskData *HLib_AllocateTaskData(HLib::Task *task, std::uint32_t size);
void HLib_FreeTaskDataForTask(HLib::Task *task);
void HLib_SetTaskMainCallback(HLib::Task *task, HLib::TaskCallback callback);
void HLib_SetCurrentTaskMainCallback(HLib::TaskCallback callback);
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
#if SHENMUE_IS_DC
std::uint32_t HLib_FreeTaskData(HLib::TaskData *taskData, std::int32_t token);
#else
std::uint64_t HLib_FreeTaskData(HLib::TaskData *taskData, std::int32_t token);
#endif

} // namespace shenmue::s1
