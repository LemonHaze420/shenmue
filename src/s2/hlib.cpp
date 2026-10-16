#include "s2/hlib.hpp"
#include "s2/pause.hpp"

#include <array>
#include <cstddef>
#include <cassert>
#include <limits>
#include <cstring>

namespace shenmue::s2::HLib {
namespace {

constexpr std::size_t kTaskCount = 300;
constexpr std::size_t kStaticStorageCount = 322;
constexpr std::uint32_t kHLib = SHEN_NAME(HLib);
constexpr std::uint32_t kNone = SHEN_NAME(NONE);
constexpr std::uint32_t kRoot = SHEN_NAME(ROOT);
constexpr std::uint32_t kTask = SHEN_NAME(TASK);

alignas(32) std::array<std::byte, DYNAMIC_HEAP_SIZE> dynamicHeap{};
alignas(32) std::array<StaticStorageEntry, kStaticStorageCount> staticStorage{};
alignas(32) StaticStorageEntry zeroEntry{};
Task rootTask{};
std::array<Task, kTaskCount - 1> taskQueue{};
std::array<Task *, 16> moduleTaskQueue{};
TaskData *dynamicFirstFree = nullptr;
TaskData *dynamicListHead = nullptr;
TaskData *dynamicFirstBlock = nullptr;
TaskData *dynamicListTail = nullptr;
TaskData *permanentFirstBlock = nullptr;
TaskData *permanentListTail = nullptr;
bool taskQueuePaused = false;
std::uint8_t pauseFirstIndex = 0;
std::uint8_t pauseLastIndex = 0;
Task *previousTask = nullptr;
Task *currentNextInQueue = nullptr;
Task *previousTaskNextInQueue = nullptr;
Task *currentNextInQueueNextTask = nullptr;
float scaledDeltaTick = 1.0f;
std::array<Task *, 32> postUpdateTasks{};
std::array<void *, 32> postUpdateData{};
std::int32_t postUpdateCount = 0;

struct PreservedCallback {
    Task *task{};
    TaskCallback callback{};
    void *taskData{};
};

std::array<PreservedCallback, 256> preservedCallbacks{};
std::int32_t preservedCallbackCount = 0;

TaskData *addBytes(TaskData *entry, std::uint64_t offset) {
    return reinterpret_cast<TaskData *>(reinterpret_cast<std::byte *>(entry) + offset);
}

TaskData *headerFromData(void *data) {
    return reinterpret_cast<TaskData *>(static_cast<std::byte *>(data) - sizeof(TaskData));
}

void *dataFromHeader(TaskData *entry) {
    return reinterpret_cast<std::byte *>(entry) + sizeof(TaskData);
}

void setEntry(TaskData *entry, std::uint32_t name, std::uint64_t size, std::uint32_t status, std::int32_t alignment) {
    entry->name.name = name;
    entry->contentSize = size;
    entry->status.name = status;
    entry->alignment = alignment;
}

Task *taskAt(std::size_t index) {
    return index == 0 ? &rootTask : &taskQueue[index - 1];
}

void destroyCurrentTaskCallback(void *) {
    DestroyCurrentTask();
}

void markTaskHierarchyForCleanup(Task *task) {
    for (Task *current = task; current; current = current->nextSibling) {
        current->activeCallback = destroyCurrentTaskCallback;
        if (current->child) {
            markTaskHierarchyForCleanup(current->child);
        }
    }
}

} // namespace

Task *CurrentTask = nullptr;
Task *NextTask = nullptr;
std::byte *DynamicHeapBase = nullptr;

void InitializeAllocator() {
    zeroEntry.header.name.name = SHEN_NAME(OREZ);
    zeroEntry.data[0] = static_cast<std::byte>('O');
    zeroEntry.data[1] = static_cast<std::byte>('R');
    zeroEntry.data[2] = static_cast<std::byte>('E');
    zeroEntry.data[3] = static_cast<std::byte>('Z');

    for (StaticStorageEntry &storage : staticStorage) {
        setEntry(&storage.header, static_cast<std::uint32_t>(' '), 0, HLIB_FREE_TOKEN.name, 0);
        storage.header.previous = &zeroEntry.header;
        storage.header.next = &zeroEntry.header;
    }

    DynamicHeapBase = dynamicHeap.data();
    std::memset(dynamicHeap.data(), 0, dynamicHeap.size());
    auto *const first = reinterpret_cast<TaskData *>(dynamicHeap.data());
    TaskData *const free = addBytes(first, sizeof(TaskData));
    TaskData *const tail = addBytes(first, DYNAMIC_HEAP_SIZE - sizeof(TaskData));

    setEntry(first, kHLib, 0, HLIB_USED_TOKEN.name, 0x80);
    first->previous = nullptr;
    first->next = free;
    setEntry(free, HLIB_FREE_TOKEN.name, DYNAMIC_HEAP_SIZE - 2 * sizeof(TaskData), HLIB_FREE_TOKEN.name, 0);
    free->previous = first;
    free->next = tail;
    setEntry(tail, kHLib, 0, HLIB_USED_TOKEN.name, 0x80);
    tail->previous = free;
    tail->next = nullptr;
    dynamicFirstFree = free;
    dynamicListHead = tail;
    dynamicFirstBlock = free;
    dynamicListTail = tail;
    permanentFirstBlock = nullptr;
    permanentListTail = nullptr;
}

void *AllocateTaskData(std::uint64_t size, shen_name name) {
    if (size == 0) {
        return zeroEntry.data;
    }

    if (size <= 0x20) {
        for (StaticStorageEntry &storage : staticStorage) {
            TaskData &entry = storage.header;
            if (entry.status.name != HLIB_USED_TOKEN.name) {
                entry.name = name;
                entry.contentSize = 0x60;
                entry.status.name = HLIB_USED_TOKEN.name;
                entry.alignment = 0;
                return dataFromHeader(&entry);
            }
        }
    }

    const std::uint64_t requestedSize = (size + 0x5f) & ~std::uint64_t{0x1f};
    for (TaskData *entry = dynamicFirstFree; entry; entry = entry->next) {
        TaskData *const next = entry->next;
        if (entry->status.name == HLIB_USED_TOKEN.name || entry->contentSize < requestedSize) {
            continue;
        }

        const std::uint64_t oldSize = entry->contentSize;
        const bool splitBlock = oldSize > requestedSize + sizeof(TaskData);
        const bool clearBlock = oldSize == requestedSize || splitBlock;
        if (splitBlock) {
            TaskData *const split = addBytes(entry, requestedSize);
            split->previous = entry;
            split->next = next;
            split->name.name = HLIB_FREE_TOKEN.name;
            split->contentSize = oldSize - requestedSize;
            split->status.name = HLIB_FREE_TOKEN.name;
            entry->next = split;
            if (next) {
                next->previous = split;
            }
            entry->contentSize = requestedSize;
        }

        entry->name = name;
        entry->status.name = HLIB_USED_TOKEN.name;
        entry->alignment = 0;
        if (clearBlock) {
            std::memset(dataFromHeader(entry), 0, static_cast<std::size_t>(entry->contentSize - sizeof(TaskData)));
        }
        return dataFromHeader(entry);
    }
    return nullptr;
}

void *AllocateStaticOrDynamicBlock(std::uint64_t size, shen_name name) {
    if (size == 0) {
        return zeroEntry.data;
    }

    std::uint64_t allocationSize = size;
    if (size <= 0x20) {
        allocationSize = 0x20;
        for (StaticStorageEntry &storage : staticStorage) {
            TaskData &entry = storage.header;
            if (entry.status.name != HLIB_USED_TOKEN.name) {
                entry.name = name;
                entry.contentSize = 0x60;
                entry.status.name = HLIB_USED_TOKEN.name;
                entry.alignment = 0;
                return dataFromHeader(&entry);
            }
        }
    }

    const std::uint64_t requestedSize = (allocationSize + 0x5f) & ~std::uint64_t{0x1f};
    for (TaskData *entry = dynamicListHead; entry; entry = entry->previous) {
        if (entry->status.name == HLIB_USED_TOKEN.name || entry->contentSize < requestedSize) {
            continue;
        }

        const std::uint64_t availableSize = entry->contentSize;
        if (availableSize == requestedSize || availableSize <= requestedSize + sizeof(TaskData)) {
            entry->name = name;
            entry->status.name = HLIB_USED_TOKEN.name;
            entry->alignment = 4;
            return dataFromHeader(entry);
        }

        const std::uint64_t remainingSize = availableSize - requestedSize;
        TaskData *const split = addBytes(entry, remainingSize);
        split->previous = entry;
        split->next = entry->next;
        if (entry->next) {
            entry->next->previous = split;
        }
        entry->next = split;
        entry->contentSize = remainingSize;
        setEntry(split, name.name, requestedSize, HLIB_USED_TOKEN.name, 4);
        return dataFromHeader(split);
    }
    return nullptr;
}

std::int64_t AllocateHeapSpace(void *taskData, std::uint64_t size, shen_name name) {
    const auto address = reinterpret_cast<std::uintptr_t>(taskData);
    const std::uintptr_t alignedAddress = address & ~std::uintptr_t{0x1f};
    const std::uint64_t blockSize = (address - alignedAddress + size + 0x7f) & ~std::uint64_t{0x1f};
    const std::uintptr_t blockAddress = alignedAddress - sizeof(TaskData);
    const std::uintptr_t endAddress = blockAddress + blockSize;
    if (!dynamicFirstFree || blockAddress <= reinterpret_cast<std::uintptr_t>(dynamicFirstFree) ||
        blockAddress >= reinterpret_cast<std::uintptr_t>(dynamicListHead)) {
        return 0;
    }
    for (TaskData *entry = dynamicFirstFree; entry->next; entry = entry->next) {
        TaskData *const next = entry->next;
        const auto entryAddress = reinterpret_cast<std::uintptr_t>(entry);
        const auto nextAddress = reinterpret_cast<std::uintptr_t>(next);
        if (entry->status.name != HLIB_FREE_TOKEN.name || entryAddress > blockAddress || endAddress > nextAddress) {
            continue;
        }
        auto *const block = reinterpret_cast<TaskData *>(blockAddress);
        block->name = name;
        block->contentSize = blockSize;
        block->status.name = HLIB_USED_TOKEN.name;
        if (entry != block) {
            entry->next = block;
            entry->contentSize = blockAddress - entryAddress;
            block->previous = entry;
            entry = block;
        }
        if (endAddress < nextAddress) {
            auto *const remainder = reinterpret_cast<TaskData *>(endAddress);
            remainder->previous = entry;
            setEntry(remainder, HLIB_FREE_TOKEN.name, nextAddress - endAddress, HLIB_FREE_TOKEN.name, 0);
            entry->next = remainder;
            entry = remainder;
        }
        next->previous = entry;
        entry->next = next;
        return 1;
    }
    return 0;
}

std::int64_t FreeTaskData(void *taskData) {
    return FreeHeapSpace(taskData, 0);
}

std::int64_t FreeHeapSpace(void *taskData, std::int32_t taskName) {
    if (!taskData) {
        return 0;
    }
    if (taskData == zeroEntry.data) {
        return 1;
    }

    TaskData *entry = headerFromData(taskData);
    if (entry->status.name != HLIB_USED_TOKEN.name) {
        return 0;
    }
    if ((entry->alignment & 0x10) != 0 && (static_cast<std::uint32_t>(taskName) & entry->name.name) != entry->name.name) {
        return 0;
    }

    if (entry->contentSize <= 0x60) {
        for (StaticStorageEntry &storage : staticStorage) {
            if (dataFromHeader(&storage.header) == taskData) {
                storage.header.name.name = HLIB_FREE_TOKEN.name;
                storage.header.contentSize = 0;
                storage.header.status.name = HLIB_FREE_TOKEN.name;
                return 1;
            }
        }
    }

    std::uint64_t size = entry->contentSize;
    TaskData *const previous = entry->previous;
    TaskData *const next = entry->next;
    entry->status.name = HLIB_FREE_TOKEN.name;
    entry->name.name = HLIB_FREE_TOKEN.name;

    if (previous->status.name == HLIB_FREE_TOKEN.name) {
        size += previous->contentSize;
        entry = previous;
        previous->next = next;
    }
    next->previous = entry;
    if (next->status.name == HLIB_FREE_TOKEN.name) {
        size += next->contentSize;
        entry->next = next->next;
        next->next->previous = entry;
    }
    entry->contentSize = size;
    return 1;
}

void *AnalyzeFreeHeap(std::uint64_t *totalFree, std::uint64_t *largestContiguousFreeBlock, std::uint64_t *totalFreeExcludingLargest) {
    std::uint64_t freeSize = 0;
    std::uint64_t largestSize = 0;
    void *largestBlock = nullptr;
    TaskData *entry = dynamicFirstFree;
    while (entry) {
        void *const data = dataFromHeader(entry);
        if ((reinterpret_cast<std::uintptr_t>(data) & 0x1fU) != 0)
            break;
        if (entry->status.name != HLIB_USED_TOKEN.name || entry->name.name == 0) {
            const std::uint64_t size = entry->contentSize - sizeof(TaskData);
            freeSize += size;
            if (largestSize < size) {
                largestSize = size;
                largestBlock = data;
            }
        }
        if (!entry->next || entry->next == dynamicListHead)
            break;
        entry = entry->next;
    }
    *totalFree = freeSize;
    *largestContiguousFreeBlock = largestSize;
    *totalFreeExcludingLargest = freeSize - largestSize;
    return largestBlock;
}

// @verified - p
std::uint64_t WouldAllocationFail(std::uint64_t requiredForRegion, std::int64_t requiredForHeap) {
    std::uint64_t excludedLargest, largestBlock, totalHeap, totalSpace, regionFree;
    AnalyzeFreeHeap(&excludedLargest, &largestBlock, &totalHeap);
    const bool exceedsHeap = static_cast<std::uint64_t>(requiredForHeap) + 32U > largestBlock;
    TryGetFreeHeapStats(&totalSpace, &regionFree);
    return static_cast<std::uint32_t>(exceedsHeap || (requiredForRegion != 0 && requiredForRegion > regionFree));
}

void *PartitionSceneHeap() {
    std::uint64_t totalFree = 0;
    std::uint64_t largestSize = 0;
    std::uint64_t remainingFree = 0;
    void *const largestBlock = AnalyzeFreeHeap(&totalFree, &largestSize, &remainingFree);
    TaskData *const block = headerFromData(largestBlock);
    TaskData *const previous = block->previous;
    TaskData *const next = block->next;
    previous->previous = nullptr;
    previous->contentSize = 0;
    next->next = nullptr;
    next->contentSize = 0;
    permanentFirstBlock = dynamicFirstFree;
    permanentListTail = dynamicListHead;
    dynamicFirstFree = block;
    dynamicListHead = next;
    dynamicFirstBlock = block;
    dynamicListTail = next;
    return largestBlock;
}

void ValidateOrResetHeap() {
    TaskData *const tail = dynamicListHead;
    TaskData *const previous = dynamicFirstFree->previous;
    if ((previous->alignment & 0x80) != 0 && (tail->alignment & 0x80) != 0) {
        InitializeAllocator();
    } else {
        TaskData *const first = previous->next;
        setEntry(first, HLIB_FREE_TOKEN.name, static_cast<std::uint64_t>(reinterpret_cast<std::byte *>(tail) - reinterpret_cast<std::byte *>(first)), HLIB_FREE_TOKEN.name, 0);
        first->next = tail;
        tail->previous = first;
    }
}

void *FreeUnusedHeapMem() {
    for (TaskData *entry = dynamicFirstBlock; entry != dynamicListTail; entry = entry->next) {
        if (entry->status.name == HLIB_USED_TOKEN.name && (entry->alignment & 1) == 0) {
            FreeHeapSpace(dataFromHeader(entry), -1);
        }
    }
    for (StaticStorageEntry &storage : staticStorage) {
        setEntry(&storage.header, static_cast<std::uint32_t>(' '), 0, HLIB_FREE_TOKEN.name, 0);
    }
    return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(staticStorage.data()) + sizeof(staticStorage) + offsetof(TaskData, status));
}

void AssignDynamicAlignmentForTaskData(void *taskData) {
    if (taskData) {
        headerFromData(taskData)->alignment |= 0x10;
    }
}

void SetTaskDataMotionFlag(void *taskData) {
    if (taskData) {
        headerFromData(taskData)->alignment |= 1;
    }
}

void ClearTaskDataMark(void *taskData) {
    if (taskData)
        headerFromData(taskData)->alignment &= ~0x10;
}

void *GetDynamicUseStoragePtr() {
    return dataFromHeader(dynamicFirstFree);
}

void *GetPointerFromDynamicHeapOffsetBase(std::int32_t offset) {
    return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(DynamicHeapBase) + sizeof(StaticStorageEntry) + offset);
}

std::int32_t GetDynamicHeapOffset(const void *pointer) {
    const std::intptr_t offset = reinterpret_cast<std::intptr_t>(pointer) - reinterpret_cast<std::intptr_t>(DynamicHeapBase) - sizeof(StaticStorageEntry);
    assert(offset >= std::numeric_limits<std::int32_t>::min() && offset <= std::numeric_limits<std::int32_t>::max());
    return static_cast<std::int32_t>(offset);
}

std::uint32_t GetTaskDataName(const void *taskData) {
    if (!taskData || (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) != 0)
        return 0;
    const TaskData *header = headerFromData(const_cast<void *>(taskData));
    return header->status.name == HLIB_USED_TOKEN.name ? header->name.name : 0;
}

std::uint64_t GetTaskDataSize(const void *taskData) {
    if (!taskData || (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) != 0)
        return 0;
    return headerFromData(const_cast<void *>(taskData))->contentSize - sizeof(TaskData);
}

bool ShrinkTaskData(void *taskData, std::uint64_t size) {
    if (!taskData)
        return false;
    if (size <= 0x20)
        return true;
    TaskData *entry = headerFromData(taskData);
    const std::uint64_t desiredSize = (size + 95) & ~std::uint64_t{31};
    if (entry->status.name != HLIB_USED_TOKEN.name || entry->contentSize < desiredSize)
        return false;
    if (entry->contentSize > desiredSize + sizeof(TaskData)) {
        TaskData *next = entry->next;
        TaskData *remainder = addBytes(entry, desiredSize);
        std::uint64_t remainderSize = entry->contentSize - desiredSize;
        remainder->name.name = HLIB_FREE_TOKEN.name;
        remainder->status.name = HLIB_FREE_TOKEN.name;
        remainder->previous = entry;
        entry->next = remainder;
        entry->contentSize = desiredSize;
        remainder->next = next;
        if (next->status.name != HLIB_USED_TOKEN.name) {
            remainderSize += next->contentSize;
            next = next->next;
            remainder->next = next;
        }
        next->previous = remainder;
        remainder->contentSize = remainderSize;
    }
    return true;
}

Task *InitializeTasks(TaskCallback callback) {
    for (std::size_t index = 0; index < kTaskCount; ++index) {
        Task *const task = taskAt(index);
        std::memset(task, 0, sizeof(Task));
        task->taskToken = static_cast<std::int32_t>(kNone);
        task->next = index + 1 < kTaskCount ? taskAt(index + 1) : nullptr;
    }

    rootTask.taskToken = static_cast<std::int32_t>(kRoot);
    rootTask.activeCallback = callback;
    rootTask.taskIndex = 0;
    rootTask.nextTaskIndex = 1;
    rootTask.next = &rootTask;
    rootTask.nextInQueue = &rootTask;
    NextTask = &taskQueue[0];
    moduleTaskQueue.fill(&rootTask);
    postUpdateTasks.fill(nullptr);
    postUpdateData.fill(nullptr);
    postUpdateCount = 0;
    preservedCallbacks.fill({});
    preservedCallbackCount = 0;
    taskQueuePaused = false;
    CurrentTask = &rootTask;
    return &rootTask;
}

void MainLoop() {
    CurrentTask->activeCallback(CurrentTask->taskData);
    CurrentTask = CurrentTask->nextInQueue;
}

// @verified - p
bool HasActiveTaskCallback(const Task *task) {
    return task && task->activeCallback;
}

Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
    if (nextFunctionIndex >= 16 || taskQueuePaused || !NextTask) {
        return nullptr;
    }

    Task *const task = NextTask;
    NextTask = task->next;
    Task *const moduleTask = moduleTaskQueue[nextFunctionIndex];
    Task *const following = moduleTask->nextInQueue;
    task->next = moduleTask;
    task->nextInQueue = following;
    moduleTask->nextInQueue = task;
    following->next = task;
    task->taskIndex = nextFunctionIndex;
    task->nextTaskIndex = taskFlags;
    task->activeCallback = callback;
    task->parent = CurrentTask;
    task->child = nullptr;
    task->cleanupParent = nullptr;
    task->delayedCallback = nullptr;
    task->periodicCallback = nullptr;
    task->taskData = nullptr;
    task->nextSibling = nullptr;
    task->taskToken = taskToken;
    task->freeTaskDataOnDestroy = 0;
    task->unused_0f = 0;

    for (std::size_t index = nextFunctionIndex; index < moduleTaskQueue.size(); ++index) {
        if (moduleTaskQueue[index] == moduleTask) {
            moduleTaskQueue[index] = task;
        }
    }
    if (CurrentTask) {
        task->previousSibling = CurrentTask->cleanupParent;
        if (CurrentTask->cleanupParent) {
            CurrentTask->cleanupParent->nextSibling = task;
        } else {
            CurrentTask->child = task;
        }
        CurrentTask->cleanupParent = task;
    } else {
        task->previousSibling = nullptr;
    }
    return task;
}
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::uint8_t taskFlags, std::int32_t taskToken, char *description) {
    (void)description;
    return EnqueueTaskWithoutParameter(callback, CurrentTask->taskIndex, taskFlags, taskToken);
}

Task *EnqueueTaskWithParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::uint64_t size, shen_name taskToken) {
    Task *const task = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, static_cast<std::int32_t>(taskToken.name));
    if (!task) {
        return nullptr;
    }
    void *const taskData = AllocateTaskData(size, shen_name{kTask});
    AssignDynamicAlignmentForTaskData(taskData);
    task->taskData = taskData;
    if (taskData) {
        return task;
    }
    task->activeCallback = destroyCurrentTaskCallback;
    task->delayedCallback = nullptr;
    task->periodicCallback = nullptr;
    return nullptr;
}

Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
    Task *const oldCurrent = CurrentTask;
    CurrentTask = nullptr;
    Task *const result = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, taskToken);
    CurrentTask = oldCurrent;
    return result;
}

Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
    if (!newCurrent) {
        return nullptr;
    }
    Task *const oldCurrent = CurrentTask;
    CurrentTask = newCurrent;
    Task *const result = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, taskToken);
    CurrentTask = oldCurrent;
    return result;
}

// @verified - p
std::int64_t DestroyTaskInternal(Task *task) {
    if (!task || !task->activeCallback) {
        return 0;
    }
    const bool wasPaused = taskQueuePaused;
    if (wasPaused) {
        UnpauseTaskEnqueue();
    }
    if (task->taskIndex < 16) {
        for (std::size_t index = task->taskIndex; index < moduleTaskQueue.size(); ++index) {
            if (moduleTaskQueue[index] == task) {
                moduleTaskQueue[index] = task->next;
            }
        }
    }
    task->next->nextInQueue = task->nextInQueue;
    task->nextInQueue->next = task->next;
    task->next = NextTask;
    NextTask = task;
    task->activeCallback = nullptr;
    for (Task *child = task->child; child; child = child->nextSibling) {
        child->parent = nullptr;
    }
    if (task->parent) {
        if (task->parent->child == task) {
            task->parent->child = task->nextSibling;
        }
        if (task->parent->cleanupParent == task) {
            task->parent->cleanupParent = task->previousSibling;
        }
    }
    if (task->previousSibling) {
        task->previousSibling->nextSibling = task->nextSibling;
    }
    if (task->nextSibling) {
        task->nextSibling->previousSibling = task->previousSibling;
    }
    void *const taskData = task->taskData;
    task->previousSibling = nullptr;
    task->nextSibling = nullptr;
    task->child = nullptr;
    task->cleanupParent = nullptr;
    if (taskData && (task->freeTaskDataOnDestroy & 1U) == 0) {
        FreeHeapSpace(taskData, static_cast<std::int32_t>(kTask));
        task->taskData = nullptr;
    }
    if (wasPaused) {
        PauseTaskRangeExecution(pauseFirstIndex, pauseLastIndex);
    }
    return 1;
}

// @verified - p
std::int64_t DestroyTask(Task *task) {
    return DestroyTaskInternal(task);
}

// @verified - p
std::int64_t DestroyCurrentTask() {
    return DestroyTaskInternal(CurrentTask);
}

void PauseTaskRangeExecution(std::uint16_t firstTaskIndex, std::uint16_t lastTaskIndex) {
    const auto first = static_cast<std::uint8_t>(firstTaskIndex);
    const auto last = static_cast<std::uint8_t>(lastTaskIndex);
    if (first == 0 || last >= 16 || first > last) {
        return;
    }
    if (taskQueuePaused) {
        UnpauseTaskEnqueue();
    }
    pauseFirstIndex = first;
    pauseLastIndex = last;
    taskQueuePaused = true;
    previousTask = moduleTaskQueue[first - 1];
    Task *const lastTask = moduleTaskQueue[last];
    currentNextInQueue = lastTask->nextInQueue;
    previousTaskNextInQueue = previousTask->nextInQueue;
    currentNextInQueueNextTask = currentNextInQueue->next;
    previousTask->nextInQueue = currentNextInQueue;
    currentNextInQueue->next = previousTask;
}

void UnpauseTaskEnqueue() {
    if (!taskQueuePaused) {
        return;
    }
    taskQueuePaused = false;
    previousTask->nextInQueue = previousTaskNextInQueue;
    currentNextInQueue->next = currentNextInQueueNextTask;
}

void *GetTaskParameter(Task *task) {
    return task ? task->taskData : nullptr;
}
Task *GetCurrentTask() {
    return CurrentTask;
}
Task *GetCurrentParentTask() {
    return CurrentTask ? CurrentTask->parent : nullptr;
}
void *GetCurrentTaskData() {
    return CurrentTask ? CurrentTask->taskData : nullptr;
}

std::uint8_t GetCurrentTaskIndex() {
    return CurrentTask ? CurrentTask->taskIndex : 0;
}

std::int64_t ExecuteCallbackNow(TaskCallback callback, void *taskData) {
    callback(taskData);

    const std::uint8_t taskIndex = GetCurrentTaskIndex();
    if ((g_loadingFlag & 0x102U) == 0 && static_cast<std::uint8_t>(taskIndex - 3U) <= 10U) {
        if (preservedCallbackCount >= static_cast<std::int32_t>(preservedCallbacks.size())) {
            return 0;
        }
        PreservedCallback &preserved = preservedCallbacks[static_cast<std::size_t>(preservedCallbackCount++)];
        preserved.task = GetCurrentTask();
        preserved.callback = callback;
        preserved.taskData = taskData;
    }
    return 1;
}

void ReplayPreservedCallbacks() {
    Task *const savedCurrentTask = CurrentTask;
    std::int32_t index = 0;
    while (index < preservedCallbackCount) {
        const PreservedCallback &preserved = preservedCallbacks[static_cast<std::size_t>(index++)];
        CurrentTask = preserved.task;
        if (preserved.callback)
            preserved.callback(preserved.taskData);
    }
    CurrentTask = savedCurrentTask;
}

void ResetPreservedCallbacks() {
    preservedCallbackCount = 0;
}

Task *FindNextTaskInQueueByIndexWithDepth(std::uint8_t nextTaskIndex, std::int32_t searchDepth) {
    Task *task = &rootTask;
    while (task->nextTaskIndex != nextTaskIndex || --searchDepth > 0) {
        task = task->nextInQueue;
        if (task == &rootTask) {
            return nullptr;
        }
    }
    return task;
}

std::int64_t AssignTaskDataToTask(Task *task, void *taskData) {
    if (!task || task->taskData) {
        return 0;
    }
    task->taskData = taskData;
    return 1;
}

std::int64_t SetCurrentTaskData(void *taskData) {
    return AssignTaskDataToTask(CurrentTask, taskData);
}

void SetCurrentTaskTo(TaskCallback callback) {
    AssignCallbackForTask(CurrentTask, callback);
}

void AssignCallbackForTask(Task *task, TaskCallback callback) {
    if (!task) {
        return;
    }
    task->activeCallback = callback;
    task->delayedCallback = nullptr;
    task->periodicCallback = nullptr;
}

Task *MarkCurrentTaskForCleanup() {
    if (!CurrentTask) {
        return nullptr;
    }
    CurrentTask->activeCallback = destroyCurrentTaskCallback;
    if (CurrentTask->child) {
        HLKillOtherHierarchyTask(CurrentTask->child);
    }
    return CurrentTask;
}

Task *HLKillOtherHierarchyTask(Task *task) {
    if (task) {
        markTaskHierarchyForCleanup(task);
    }
    return task;
}


void EV_HLKillOtherHierarchyTask(Task *task) {
    if (!task)
        return;
    task->activeCallback = destroyCurrentTaskCallback;
    Task *const child = task->child;
    if (!child)
        return;
    child->activeCallback = destroyCurrentTaskCallback;
    if (child->child)
        HLKillOtherHierarchyTask(child->child);
    if (child->nextSibling)
        HLKillOtherHierarchyTask(child->nextSibling);
}

std::int64_t DestroyTasksFromIndex(std::uint8_t taskIndex) {
    if (taskQueuePaused) {
        return 0;
    }
    for (; taskIndex < moduleTaskQueue.size(); ++taskIndex) {
        if (taskIndex != 0) {
            Task *task = moduleTaskQueue[taskIndex - 1]->nextInQueue;
            while (task->taskIndex == taskIndex) {
                task->activeCallback = destroyCurrentTaskCallback;
                task = task->nextInQueue;
            }
        }
    }
    return 1;
}

void FreeCurrentTaskData() {
    if (CurrentTask && CurrentTask->taskData) {
        FreeHeapSpace(CurrentTask->taskData, static_cast<std::int32_t>(kTask));
        CurrentTask->taskData = nullptr;
    }
}

std::int64_t SetCurrentTaskCustomUserData(TaskCallback callback, void *callbackData) {
    if (postUpdateCount >= static_cast<std::int32_t>(postUpdateTasks.size())) {
        return 0;
    }
    CurrentTask->postUpdateCallback = callback;
    const std::size_t index = static_cast<std::size_t>(postUpdateCount++);
    postUpdateTasks[index] = CurrentTask;
    postUpdateData[index] = callbackData;
    return 1;
}

void ProcessTaskPostUpdateCallbacks() {
    Task *const savedCurrentTask = CurrentTask;
    std::int32_t index = 0;
    while (index < postUpdateCount) {
        const std::size_t queueIndex = static_cast<std::size_t>(index++);
        CurrentTask = postUpdateTasks[queueIndex];
        CurrentTask->postUpdateCallback(postUpdateData[queueIndex]);
    }
    postUpdateCount = 0;
    CurrentTask = savedCurrentTask;
}

void SetupTimedTask(Task *task, std::int32_t tickTime) {
    if (!task) {
        return;
    }
    if (!task->delayedCallback) {
        task->delayedCallback = task->activeCallback;
        task->activeCallback = UpdateTimedTask;
    }
    task->timedTask.timeUntilExecution = static_cast<float>(tickTime);
}

void SetupTaskDelay(std::int32_t tickTime) {
    SetupTimedTask(CurrentTask, tickTime);
}

void UpdateLoopedTask(void *) {
    if (CurrentTask->loopedTask.remainingLoops-- == 1) {
        CurrentTask->loopedTask.remainingLoops = CurrentTask->loopedTask.loopIntervalFrames;
        CurrentTask->periodicCallback(CurrentTask->taskData);
    }
}

void UpdateTimedTask(void *) {
    if (CurrentTask->timedTask.timeUntilExecution <= 0.0f) {
        return;
    }
    CurrentTask->timedTask.timeUntilExecution -= scaledDeltaTick;
    if (CurrentTask->timedTask.timeUntilExecution <= 0.0f) {
        CurrentTask->timedTask.timeUntilExecution = 0.0f;
        CurrentTask->activeCallback = CurrentTask->delayedCallback;
        CurrentTask->delayedCallback = nullptr;
    }
}

void ResolveDeferredCallback(Task *task) {
    if (!task || !task->delayedCallback) {
        return;
    }
    if (task->periodicCallback && task->activeCallback == UpdateLoopedTask) {
        task->periodicCallback = task->delayedCallback;
    } else {
        task->activeCallback = task->delayedCallback;
    }
    task->delayedCallback = nullptr;
}

void SetScaledDeltaTick(float ticks) {
    scaledDeltaTick = ticks;
}

float GetScaledDeltaTick() {
    return scaledDeltaTick;
}

} // namespace shenmue::s2::HLib

namespace shenmue::s2 {

// @verified - p
void *AllocateTASK(HLib::Task *task, std::uint64_t size) {
    if (!task || task->taskData)
        return nullptr;
    void *const taskData = HLib::AllocateTaskData(size, SHEN_NAME(TASK));
    HLib::AssignDynamicAlignmentForTaskData(taskData);
    task->taskData = taskData;
    return taskData;
}

// @verified - p
void FreeTASK(HLib::Task *task) {
    if (!task || !task->taskData)
        return;
    HLib::FreeHeapSpace(task->taskData, static_cast<std::int32_t>(SHEN_NAME(TASK)));
    task->taskData = nullptr;
}

} // namespace shenmue::s2
