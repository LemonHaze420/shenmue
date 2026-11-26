#include "s2/hlib.hpp"
#if !SHENMUE_S2_GCC295
#include "s2/frame_pacing.hpp"
#endif
#include "s2/pause.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/startup_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/input_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_EU)
#include "s2/frame_dc.hpp"
#endif
#endif
#if !SHENMUE_IS_DC
#include "s2/mt7.hpp"
#endif
#if SHENMUE_S2_GCC295
#include <assert.h>
#include <string.h>
#define SHENMUE_HLIB_CONSTANT const
#define SHENMUE_HLIB_ALIGN __attribute__((aligned(32)))
#define SHENMUE_HLIB_NULL 0
#define SHENMUE_HLIB_COUNT(value) (sizeof(value) / sizeof(value[0]))
#define SHENMUE_HLIB_VALUE(type, value) type(value)
#define SHENMUE_HLIB_HEAP dynamicHeap
#define SHENMUE_HLIB_MEMSET ::memset
#define SHENMUE_HLIB_STATIC_ITERATION for (std::size_t storageIndex = 0; storageIndex < kStaticStorageCount; ++storageIndex)
#define SHENMUE_HLIB_INT_MIN (-2147483647 - 1)
#define SHENMUE_HLIB_INT_MAX 2147483647
namespace shenmue {
namespace s2 {
namespace HLib {
#else
#include <span>

#include <array>
#include <cstddef>
#include <cassert>
#include <limits>
#include <cstring>
#define SHENMUE_HLIB_CONSTANT constexpr
#define SHENMUE_HLIB_ALIGN alignas(32)
#define SHENMUE_HLIB_NULL nullptr
#define SHENMUE_HLIB_COUNT(value) value.size()
#define SHENMUE_HLIB_VALUE(type, value)                                                                                                                        \
    type {                                                                                                                                                     \
        value                                                                                                                                                  \
    }
#define SHENMUE_HLIB_HEAP dynamicHeap.data()
#define SHENMUE_HLIB_MEMSET std::memset
#define SHENMUE_HLIB_STATIC_ITERATION for (StaticStorageEntry &storage : staticStorage)
#define SHENMUE_HLIB_INT_MIN std::numeric_limits<std::int32_t>::min()
#define SHENMUE_HLIB_INT_MAX std::numeric_limits<std::int32_t>::max()
namespace shenmue::s2::HLib {
#endif
namespace {

SHENMUE_HLIB_CONSTANT std::size_t kTaskCount = 300;
SHENMUE_HLIB_CONSTANT std::size_t kStaticStorageCount = SHENMUE_IS_DC ? 0 : 322;
#if SHENMUE_S2_GCC295
const std::uint32_t kHLib =
    static_cast<std::uint32_t>('H') | static_cast<std::uint32_t>('L') << 8U | static_cast<std::uint32_t>('i') << 16U | static_cast<std::uint32_t>('b') << 24U;
#else
constexpr std::uint32_t kHLib = SHEN_NAME(HLib);
#endif
#if SHENMUE_S2_GCC295
const std::uint32_t kNone =
    static_cast<std::uint32_t>('N') | static_cast<std::uint32_t>('O') << 8U | static_cast<std::uint32_t>('N') << 16U | static_cast<std::uint32_t>('E') << 24U;
#else
constexpr std::uint32_t kNone = SHEN_NAME(NONE);
#endif
#if SHENMUE_S2_GCC295
const std::uint32_t kRoot =
    static_cast<std::uint32_t>('R') | static_cast<std::uint32_t>('O') << 8U | static_cast<std::uint32_t>('O') << 16U | static_cast<std::uint32_t>('T') << 24U;
#else
constexpr std::uint32_t kRoot = SHEN_NAME(ROOT);
#endif
#if SHENMUE_S2_GCC295
const std::uint32_t kTask =
    static_cast<std::uint32_t>('T') | static_cast<std::uint32_t>('A') << 8U | static_cast<std::uint32_t>('S') << 16U | static_cast<std::uint32_t>('K') << 24U;
#else
constexpr std::uint32_t kTask = SHEN_NAME(TASK);
#endif

#if SHENMUE_S2_GCC295
std::byte *const dynamicHeap = reinterpret_cast<std::byte *>(DYNAMIC_HEAP_ADDRESS);
#elif SHENMUE_IS_DC
std::span<std::byte, DYNAMIC_HEAP_SIZE> dynamicHeap{reinterpret_cast<std::byte *>(DYNAMIC_HEAP_ADDRESS), DYNAMIC_HEAP_SIZE};
#else
alignas(32) std::array<std::byte, DYNAMIC_HEAP_SIZE> dynamicHeap{};
#endif
#if SHENMUE_S2_GCC295
SHENMUE_HLIB_ALIGN StaticStorageEntry staticStorage[kStaticStorageCount];
SHENMUE_HLIB_ALIGN StaticStorageEntry zeroEntry;
Task taskStorage[kTaskCount];
Task *moduleTaskQueue[16];
#else
alignas(32) std::array<StaticStorageEntry, kStaticStorageCount> staticStorage{};
alignas(32) StaticStorageEntry zeroEntry{};
Task rootTask{};
std::array<Task, kTaskCount - 1> taskQueue{};
std::array<Task *, 16> moduleTaskQueue{};
#endif
TaskData *dynamicFirstFree = SHENMUE_HLIB_NULL;
TaskData *dynamicListHead = SHENMUE_HLIB_NULL;
TaskData *dynamicFirstBlock = SHENMUE_HLIB_NULL;
TaskData *dynamicListTail = SHENMUE_HLIB_NULL;
TaskData *permanentFirstBlock = SHENMUE_HLIB_NULL;
TaskData *permanentListTail = SHENMUE_HLIB_NULL;
#if SHENMUE_IS_DC
struct TaskQueuePauseState {
#if SHENMUE_IS_DC
    std::int32_t paused;
    std::int8_t firstIndex, lastIndex;
#else
    bool paused;
    std::uint8_t firstIndex, lastIndex;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Task *previousTask;
    Task *currentNextInQueue;
    Task *previousTaskNextInQueue;
    Task *currentNextInQueueNextTask;
#endif
};
TaskQueuePauseState taskQueuePause;
#else
bool taskQueuePaused = false;
std::uint8_t pauseFirstIndex = 0;
std::uint8_t pauseLastIndex = 0;
#endif
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Task *previousTask = SHENMUE_HLIB_NULL;
Task *currentNextInQueue = SHENMUE_HLIB_NULL;
Task *previousTaskNextInQueue = SHENMUE_HLIB_NULL;
Task *currentNextInQueueNextTask = SHENMUE_HLIB_NULL;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t taskCycleIndex;
std::int32_t taskCycleRange;
std::int32_t taskCycleLimit;
#endif
float scaledDeltaTick = 1.0f;
#if SHENMUE_S2_GCC295
Task *postUpdateTasks[32];
void *postUpdateData[32] = {};
#else
std::array<Task *, 32> postUpdateTasks{};
std::array<void *, 32> postUpdateData{};
#endif
std::int32_t postUpdateCount;

struct PreservedCallback {
#if SHENMUE_S2_GCC295
    Task *task;
    TaskCallback callback;
    void *taskData;
#else
    Task *task{};
    TaskCallback callback{};
    void *taskData{};
#endif
};

#if SHENMUE_S2_GCC295
PreservedCallback preservedCallbacks[256];
#else
std::array<PreservedCallback, 256> preservedCallbacks{};
#endif
std::int32_t preservedCallbackCount = 0;
#if SHENMUE_IS_DC
std::int32_t taskTreeCount = 0;
Task *taskTreeRoot = SHENMUE_HLIB_NULL;
#endif

TaskData *addBytes(TaskData *entry, std::size_t offset) {
    return reinterpret_cast<TaskData *>(reinterpret_cast<std::byte *>(entry) + offset);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
inline TaskData *headerFromData(void *data) {
#else
TaskData *headerFromData(void *data) {
#endif
    return reinterpret_cast<TaskData *>(static_cast<std::byte *>(data) - sizeof(TaskData));
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
inline void *dataFromHeader(TaskData *entry) {
#else
void *dataFromHeader(TaskData *entry) {
#endif
    return reinterpret_cast<std::byte *>(entry) + sizeof(TaskData);
}

void setEntry(TaskData *entry, std::uint32_t name, std::size_t size, std::uint32_t status, std::int32_t alignment) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    entry->name = name;
#else
    entry->name.name = name;
#endif
    entry->contentSize = size;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    entry->status = status;
#else
    entry->status.name = status;
#endif
    entry->alignment = alignment;
}

Task *taskAt(std::size_t index) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return &taskStorage[index];
#else
    return index == 0 ? &rootTask : &taskQueue[index - 1];
#endif
}

void destroyCurrentTaskCallback(void *) {
    DestroyCurrentTask();
}

void markTaskHierarchyForCleanup(Task *task) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (task) {
        task->activeCallback = reinterpret_cast<TaskCallback>(DestroyCurrentTask);
        if (task->child)
            markTaskHierarchyForCleanup(task->child);
        if (task->nextSibling)
            markTaskHierarchyForCleanup(task->nextSibling);
    }
#else
    for (Task *current = task; current; current = current->nextSibling) {
        current->activeCallback = destroyCurrentTaskCallback;
        if (current->child) {
            markTaskHierarchyForCleanup(current->child);
        }
    }
#endif
}

} // namespace

Task *CurrentTask = SHENMUE_HLIB_NULL;
Task *NextTask = SHENMUE_HLIB_NULL;
std::byte *DynamicHeapBase = SHENMUE_HLIB_NULL;

void InitializeAllocator() {
#if !SHENMUE_IS_DC
    ReleaseMT7TaskNodeCopies(SHENMUE_HLIB_HEAP, dynamicHeap.size());
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    zeroEntry.header.name = SHEN_NAME(OREZ);
#else
    zeroEntry.header.name.name = SHEN_NAME(OREZ);
#endif
    zeroEntry.data[0] = static_cast<std::byte>('O');
    zeroEntry.data[1] = static_cast<std::byte>('R');
    zeroEntry.data[2] = static_cast<std::byte>('E');
    zeroEntry.data[3] = static_cast<std::byte>('Z');

    SHENMUE_HLIB_STATIC_ITERATION {
#if SHENMUE_S2_GCC295
        StaticStorageEntry &storage = staticStorage[storageIndex];
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        setEntry(&storage.header, static_cast<std::uint32_t>(' '), 0, static_cast<std::uint32_t>(HLIB_FREE_TOKEN), 0);
#else
        setEntry(&storage.header, static_cast<std::uint32_t>(' '), 0, HLIB_FREE_TOKEN.name, 0);
#endif
        storage.header.previous = &zeroEntry.header;
        storage.header.next = &zeroEntry.header;
    }

    DynamicHeapBase = SHENMUE_HLIB_HEAP;
#if !SHENMUE_IS_DC
    SHENMUE_HLIB_MEMSET(SHENMUE_HLIB_HEAP, 0, dynamicHeap.size());
#endif
#if SHENMUE_S2_GCC295
    TaskData *const first = reinterpret_cast<TaskData *>(SHENMUE_HLIB_HEAP);
#else
    auto *const first = reinterpret_cast<TaskData *>(SHENMUE_HLIB_HEAP);
#endif
    TaskData *const free = addBytes(first, sizeof(TaskData));
    TaskData *const tail = addBytes(first, DYNAMIC_HEAP_SIZE - sizeof(TaskData));

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    setEntry(first, kHLib, 0, static_cast<std::uint32_t>(HLIB_USED_TOKEN), SHENMUE_IS_DC ? -0x80 : 0x80);
#else
    setEntry(first, kHLib, 0, HLIB_USED_TOKEN.name, SHENMUE_IS_DC ? -0x80 : 0x80);
#endif
    first->previous = SHENMUE_HLIB_NULL;
    first->next = free;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    setEntry(free, static_cast<std::uint32_t>(HLIB_FREE_TOKEN), DYNAMIC_HEAP_SIZE - 2 * sizeof(TaskData), static_cast<std::uint32_t>(HLIB_FREE_TOKEN), 0);
#else
    setEntry(free, HLIB_FREE_TOKEN.name, DYNAMIC_HEAP_SIZE - 2 * sizeof(TaskData), HLIB_FREE_TOKEN.name, 0);
#endif
    free->previous = first;
    free->next = tail;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    setEntry(tail, kHLib, 0, static_cast<std::uint32_t>(HLIB_USED_TOKEN), SHENMUE_IS_DC ? -0x80 : 0x80);
#else
    setEntry(tail, kHLib, 0, HLIB_USED_TOKEN.name, SHENMUE_IS_DC ? -0x80 : 0x80);
#endif
    tail->previous = free;
    tail->next = SHENMUE_HLIB_NULL;
    dynamicFirstFree = free;
    dynamicListHead = tail;
    dynamicFirstBlock = free;
    dynamicListTail = tail;
    permanentFirstBlock = SHENMUE_HLIB_NULL;
    permanentListTail = SHENMUE_HLIB_NULL;
}

void *AllocateTaskData(std::size_t size, shen_name name) {
    if (!SHENMUE_IS_DC && size == 0) {
        return zeroEntry.data;
    }

    if (!SHENMUE_IS_DC && size <= 0x20) {
        SHENMUE_HLIB_STATIC_ITERATION {
#if SHENMUE_S2_GCC295
            StaticStorageEntry &storage = staticStorage[storageIndex];
#endif
            TaskData &entry = storage.header;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            if (static_cast<std::uint32_t>(entry.status) != static_cast<std::uint32_t>(HLIB_USED_TOKEN)) {
#else
            if (entry.status.name != HLIB_USED_TOKEN.name) {
#endif
                entry.name = name;
                entry.contentSize = 0x60;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                entry.status = static_cast<std::uint32_t>(HLIB_USED_TOKEN);
#else
                entry.status.name = HLIB_USED_TOKEN.name;
#endif
                entry.alignment = 0;
                return dataFromHeader(&entry);
            }
        }
    }

    const std::size_t requestedSize = (size + sizeof(TaskData) + 0x1f) & ~SHENMUE_HLIB_VALUE(std::size_t, 0x1f);
    for (TaskData *entry = dynamicFirstFree; entry; entry = entry->next) {
        TaskData *const next = entry->next;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (static_cast<std::uint32_t>(entry->status) == static_cast<std::uint32_t>(HLIB_USED_TOKEN) || entry->contentSize < requestedSize) {
#else
        if (entry->status.name == HLIB_USED_TOKEN.name || entry->contentSize < requestedSize) {
#endif
            continue;
        }

        const std::size_t oldSize = entry->contentSize;
        const bool splitBlock = oldSize > requestedSize + sizeof(TaskData);
        const bool clearBlock = oldSize == requestedSize || splitBlock;
        if (splitBlock) {
            TaskData *const split = addBytes(entry, requestedSize);
            split->previous = entry;
            split->next = next;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            split->name = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
#else
            split->name.name = HLIB_FREE_TOKEN.name;
#endif
            split->contentSize = oldSize - requestedSize;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            split->status = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
#else
            split->status.name = HLIB_FREE_TOKEN.name;
#endif
            entry->next = split;
            if (next) {
                next->previous = split;
            }
            entry->contentSize = requestedSize;
        }

        entry->name = name;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        entry->status = static_cast<std::uint32_t>(HLIB_USED_TOKEN);
#else
        entry->status.name = HLIB_USED_TOKEN.name;
#endif
        entry->alignment = 0;
        if (!SHENMUE_IS_DC && clearBlock) {
            SHENMUE_HLIB_MEMSET(dataFromHeader(entry), 0, static_cast<std::size_t>(entry->contentSize - sizeof(TaskData)));
        }
        return dataFromHeader(entry);
    }
#if SHENMUE_IS_DC
    ReportAllocationFailure(requestedSize, name, SHENMUE_HLIB_NULL);
#endif
    return SHENMUE_HLIB_NULL;
}

void *AllocateStaticOrDynamicBlock(std::size_t size, shen_name name) {
    if (!SHENMUE_IS_DC && size == 0) {
        return zeroEntry.data;
    }

    std::size_t allocationSize = size;
    if (!SHENMUE_IS_DC && size <= 0x20) {
        allocationSize = 0x20;
        SHENMUE_HLIB_STATIC_ITERATION {
#if SHENMUE_S2_GCC295
            StaticStorageEntry &storage = staticStorage[storageIndex];
#endif
            TaskData &entry = storage.header;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            if (static_cast<std::uint32_t>(entry.status) != static_cast<std::uint32_t>(HLIB_USED_TOKEN)) {
#else
            if (entry.status.name != HLIB_USED_TOKEN.name) {
#endif
                entry.name = name;
                entry.contentSize = 0x60;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                entry.status = static_cast<std::uint32_t>(HLIB_USED_TOKEN);
#else
                entry.status.name = HLIB_USED_TOKEN.name;
#endif
                entry.alignment = 0;
                return dataFromHeader(&entry);
            }
        }
    }

    const std::size_t requestedSize = (allocationSize + sizeof(TaskData) + 0x1f) & ~SHENMUE_HLIB_VALUE(std::size_t, 0x1f);
    for (TaskData *entry = dynamicListHead; entry; entry = entry->previous) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (static_cast<std::uint32_t>(entry->status) == static_cast<std::uint32_t>(HLIB_USED_TOKEN) || entry->contentSize < requestedSize) {
#else
        if (entry->status.name == HLIB_USED_TOKEN.name || entry->contentSize < requestedSize) {
#endif
            continue;
        }

        const std::size_t availableSize = entry->contentSize;
        if (availableSize == requestedSize || availableSize <= requestedSize + sizeof(TaskData)) {
            entry->name = name;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            entry->status = static_cast<std::uint32_t>(HLIB_USED_TOKEN);
#else
            entry->status.name = HLIB_USED_TOKEN.name;
#endif
            entry->alignment = 4;
            return dataFromHeader(entry);
        }

        const std::size_t remainingSize = availableSize - requestedSize;
        TaskData *const split = addBytes(entry, remainingSize);
        split->previous = entry;
        split->next = entry->next;
        if (entry->next) {
            entry->next->previous = split;
        }
        entry->next = split;
        entry->contentSize = remainingSize;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        setEntry(split, static_cast<std::uint32_t>(name), requestedSize, static_cast<std::uint32_t>(HLIB_USED_TOKEN), 4);
#else
        setEntry(split, name.name, requestedSize, HLIB_USED_TOKEN.name, 4);
#endif
        return dataFromHeader(split);
    }
#if SHENMUE_IS_DC
    ReportAllocationFailure(requestedSize, name, SHENMUE_HLIB_NULL);
#endif
    return SHENMUE_HLIB_NULL;
}

std::intptr_t AllocateHeapSpace(void *taskData, std::size_t size, shen_name name) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const shen_name freeToken = SHEN_NAME4('f', 'R', 'E', 'e');
    const shen_name usedToken = SHEN_NAME4('U', 'S', 'E', ' ');
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(taskData);
    taskData = reinterpret_cast<void *>(address & ~std::uintptr_t(0x1f));
    size += address - reinterpret_cast<std::uintptr_t>(taskData);
    size = (size + sizeof(TaskData) + 31) & ~std::size_t(0x1f);
    taskData = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(taskData) - sizeof(TaskData));
    TaskData *const block = static_cast<TaskData *>(taskData);
    TaskData *const end = reinterpret_cast<TaskData *>(reinterpret_cast<std::uintptr_t>(block) + size);
    if (!(block > dynamicFirstFree && dynamicListHead > block))
        return 0;
    TaskData *entry = dynamicFirstFree;
    while (entry->next) {
        TaskData *const next = entry->next;
        if (entry->status == freeToken && entry <= block && end <= next) {
            block->name = name;
            block->contentSize = size;
            block->status = usedToken;
            block->alignment = 0;
            if (entry != block) {
                entry->next = block;
                entry->contentSize = reinterpret_cast<std::uintptr_t>(block) - reinterpret_cast<std::uintptr_t>(entry);
                block->previous = entry;
                entry = block;
            }
            if (end < next) {
                end->name = freeToken;
                end->contentSize = reinterpret_cast<std::uintptr_t>(next) - reinterpret_cast<std::uintptr_t>(end);
                end->status = freeToken;
                end->alignment = 0;
                end->previous = entry;
                entry->next = end;
                entry = end;
            }
            next->previous = entry;
            entry->next = next;
            return 1;
        }
        entry = entry->next;
    }
    return 0;
#else
#if SHENMUE_S2_GCC295
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(taskData);
#else
    const auto address = reinterpret_cast<std::uintptr_t>(taskData);
#endif
    const std::uintptr_t alignedAddress = address & ~SHENMUE_HLIB_VALUE(std::uintptr_t, 0x1f);
    const std::size_t blockSize = (address - alignedAddress + size + 0x7f) & ~SHENMUE_HLIB_VALUE(std::size_t, 0x1f);
    const std::uintptr_t blockAddress = alignedAddress - sizeof(TaskData);
    const std::uintptr_t endAddress = blockAddress + blockSize;
    if (!dynamicFirstFree || blockAddress <= reinterpret_cast<std::uintptr_t>(dynamicFirstFree) ||
        blockAddress >= reinterpret_cast<std::uintptr_t>(dynamicListHead)) {
        return 0;
    }
    for (TaskData *entry = dynamicFirstFree; entry->next; entry = entry->next) {
        TaskData *const next = entry->next;
#if SHENMUE_S2_GCC295
        const std::uintptr_t entryAddress = reinterpret_cast<std::uintptr_t>(entry);
#else
        const auto entryAddress = reinterpret_cast<std::uintptr_t>(entry);
#endif
#if SHENMUE_S2_GCC295
        const std::uintptr_t nextAddress = reinterpret_cast<std::uintptr_t>(next);
#else
        const auto nextAddress = reinterpret_cast<std::uintptr_t>(next);
#endif
        if (entry->status.name != HLIB_FREE_TOKEN.name || entryAddress > blockAddress || endAddress > nextAddress) {
            continue;
        }
#if SHENMUE_S2_GCC295
        TaskData *const block = reinterpret_cast<TaskData *>(blockAddress);
#else
        auto *const block = reinterpret_cast<TaskData *>(blockAddress);
#endif
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
#if SHENMUE_S2_GCC295
            TaskData *const remainder = reinterpret_cast<TaskData *>(endAddress);
#else
            auto *const remainder = reinterpret_cast<TaskData *>(endAddress);
#endif
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
#endif
}

#if SHENMUE_IS_DC
void ReportAllocationFailure(std::size_t, shen_name, TaskData *) {
}
#endif

std::intptr_t FreeTaskData(void *taskData) {
    return FreeHeapSpace(taskData, 0);
}

std::intptr_t FreeHeapSpace(void *taskData, std::int32_t taskName) {
    if (!taskData) {
        return 0;
    }
    if (taskData == zeroEntry.data) {
        return 1;
    }

    TaskData *entry = headerFromData(taskData);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (static_cast<std::uint32_t>(entry->status) != static_cast<std::uint32_t>(HLIB_USED_TOKEN)) {
#else
    if (entry->status.name != HLIB_USED_TOKEN.name) {
#endif
        return 0;
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if ((entry->alignment & 0x10) != 0 &&
        (static_cast<std::uint32_t>(taskName) & static_cast<std::uint32_t>(entry->name)) != static_cast<std::uint32_t>(entry->name)) {
#else
    if ((entry->alignment & 0x10) != 0 && (static_cast<std::uint32_t>(taskName) & entry->name.name) != entry->name.name) {
#endif
        return 0;
    }

    if (entry->contentSize <= 0x60) {
        SHENMUE_HLIB_STATIC_ITERATION {
#if SHENMUE_S2_GCC295
            StaticStorageEntry &storage = staticStorage[storageIndex];
#endif
            if (dataFromHeader(&storage.header) == taskData) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                storage.header.name = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
#else
                storage.header.name.name = HLIB_FREE_TOKEN.name;
#endif
                storage.header.contentSize = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                storage.header.status = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
#else
                storage.header.status.name = HLIB_FREE_TOKEN.name;
#endif
                return 1;
            }
        }
    }

#if !SHENMUE_IS_DC
    ReleaseMT7TaskNodeCopies(taskData, static_cast<std::size_t>(entry->contentSize - sizeof(TaskData)));
#endif
    std::size_t size = entry->contentSize;
    TaskData *const previous = entry->previous;
    TaskData *const next = entry->next;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    entry->status = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
    entry->name = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);

    if (static_cast<std::uint32_t>(previous->status) == static_cast<std::uint32_t>(HLIB_FREE_TOKEN)) {
#else
    entry->status.name = HLIB_FREE_TOKEN.name;
    entry->name.name = HLIB_FREE_TOKEN.name;

    if (previous->status.name == HLIB_FREE_TOKEN.name) {
#endif
        size += previous->contentSize;
        entry = previous;
        previous->next = next;
    }
    next->previous = entry;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (static_cast<std::uint32_t>(next->status) == static_cast<std::uint32_t>(HLIB_FREE_TOKEN)) {
#else
    if (next->status.name == HLIB_FREE_TOKEN.name) {
#endif
        size += next->contentSize;
        entry->next = next->next;
        next->next->previous = entry;
    }
    entry->contentSize = size;
    return 1;
}

static void *GetNextTaskData(void *taskData);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void *AnalyzeFreeList(std::size_t *totalUsable, std::size_t *largestBlock, std::size_t *fragmentationOut) {
    std::size_t total = 0;
    std::size_t largest = 0;
    void *largestData = SHENMUE_HLIB_NULL;
    for (void *data = GetDynamicUseStoragePtr(); data; data = GetNextTaskData(data)) {
        if (GetTaskDataName(data) == 0) {
            const std::size_t size = GetTaskDataSize(data);
            total += size;
            if (largest < size) {
                largest = size;
                largestData = data;
            }
        }
    }
    *totalUsable = total;
    *largestBlock = largest;
    *fragmentationOut = total - largest;
    return largestData;
}
#endif

void *AnalyzeFreeHeap(std::size_t *totalFree, std::size_t *largestContiguousFreeBlock, std::size_t *totalFreeExcludingLargest) {
    std::size_t freeSize = 0;
    std::size_t largestSize = 0;
    void *largestBlock = SHENMUE_HLIB_NULL;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    for (void *data = GetDynamicUseStoragePtr(); data; data = GetNextTaskData(data)) {
        if (GetTaskDataName(data) == 0) {
            const std::size_t size = GetTaskDataSize(data);
#else
    TaskData *entry = dynamicFirstFree;
    while (entry) {
        void *const data = dataFromHeader(entry);
        if ((reinterpret_cast<std::uintptr_t>(data) & 0x1fU) != 0)
            break;
        if (entry->status.name != HLIB_USED_TOKEN.name || entry->name.name == 0) {
            const std::size_t size = entry->contentSize - sizeof(TaskData);
#endif
            freeSize += size;
            if (largestSize < size) {
                largestSize = size;
                largestBlock = data;
            }
        }
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        if (!entry->next || entry->next == dynamicListHead)
            break;
        entry = entry->next;
#endif
    }
    *totalFree = freeSize;
    *largestContiguousFreeBlock = largestSize;
    *totalFreeExcludingLargest = freeSize - largestSize;
    return largestBlock;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void syMallocStat(std::uint32_t *totalFreeSpace, std::uint32_t *largestFree);

// @verified - d
std::intptr_t GetFreeHeapStats(std::size_t *totalFreeSpace, std::size_t *largestFree) {
    std::uint32_t total, largest;
    syMallocStat(&total, &largest);
    *totalFreeSpace = total;
    *largestFree = largest;
    return 0;
}

// @verified - d
std::intptr_t TryGetFreeHeapStats(std::size_t *totalFreeSpace, std::size_t *largestFree) {
    const std::intptr_t result = GetFreeHeapStats(totalFreeSpace, largestFree);
    if (result) {
        *largestFree = 0;
        *totalFreeSpace = 0;
    }
    return result;
}
#endif

// @verified - pd
std::size_t WouldAllocationFail(std::size_t requiredForRegion, std::intptr_t requiredForHeap) {
    std::size_t excludedLargest, largestBlock, totalHeap, totalSpace, regionFree;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    AnalyzeFreeList(&excludedLargest, &largestBlock, &totalHeap);
#else
    AnalyzeFreeHeap(&excludedLargest, &largestBlock, &totalHeap);
#endif
    const bool exceedsHeap = static_cast<std::size_t>(requiredForHeap) + 32U > largestBlock;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    QueryFreeTextureMemory(&totalSpace, &regionFree);
#else
    TryGetFreeHeapStats(&totalSpace, &regionFree);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return static_cast<std::uint32_t>((requiredForRegion != 0 && requiredForRegion > regionFree) | exceedsHeap);
#else
    return static_cast<std::uint32_t>(exceedsHeap || (requiredForRegion != 0 && requiredForRegion > regionFree));
#endif
}

void *PartitionSceneHeap() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::size_t totalFree, largestSize, remainingFree;
    void *const largestBlock = AnalyzeFreeList(&totalFree, &largestSize, &remainingFree);
    TaskData *const block = headerFromData(largestBlock);
    TaskData *const previous = block->previous;
    TaskData *const next = block->next;
    previous->previous = SHENMUE_HLIB_NULL;
    previous->contentSize = 0;
    next->next = SHENMUE_HLIB_NULL;
    next->contentSize = 0;
    permanentFirstBlock = dynamicFirstFree;
    permanentListTail = dynamicListHead;
    dynamicFirstFree = block;
    dynamicListHead = next;
    return largestBlock;
#else
    std::size_t totalFree = 0;
    std::size_t largestSize = 0;
    std::size_t remainingFree = 0;
    void *const largestBlock = AnalyzeFreeHeap(&totalFree, &largestSize, &remainingFree);
    TaskData *const block = headerFromData(largestBlock);
    TaskData *const previous = block->previous;
    TaskData *const next = block->next;
    previous->previous = SHENMUE_HLIB_NULL;
    previous->contentSize = 0;
    next->next = SHENMUE_HLIB_NULL;
    next->contentSize = 0;
    permanentFirstBlock = dynamicFirstFree;
    permanentListTail = dynamicListHead;
    dynamicFirstFree = block;
    dynamicListHead = next;
    dynamicFirstBlock = block;
    dynamicListTail = next;
    return largestBlock;
#endif
}

void ValidateOrResetHeap() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    TaskData *previous = dynamicFirstFree->previous;
    TaskData *const tail = dynamicListHead;
    if ((previous->alignment & 0x80) && (tail->alignment & 0x80)) {
        InitializeAllocator();
    } else {
        previous = previous->next;
        const shen_name freeToken = SHEN_NAME4('f', 'R', 'E', 'e');
        previous->name = freeToken;
        previous->contentSize = reinterpret_cast<std::uintptr_t>(tail) - reinterpret_cast<std::uintptr_t>(previous);
        previous->next = tail;
        previous->status = freeToken;
        previous->alignment = 0;
        tail->previous = previous;
    }
#else
    TaskData *const tail = dynamicListHead;
    TaskData *const previous = dynamicFirstFree->previous;
    if ((previous->alignment & 0x80) != 0 && (tail->alignment & 0x80) != 0) {
        InitializeAllocator();
    } else {
        TaskData *const first = previous->next;
#if !SHENMUE_IS_DC
        ReleaseMT7TaskNodeCopies(first, static_cast<std::size_t>(reinterpret_cast<std::byte *>(tail) - reinterpret_cast<std::byte *>(first)));
#endif
        setEntry(first, HLIB_FREE_TOKEN.name, static_cast<std::size_t>(reinterpret_cast<std::byte *>(tail) - reinterpret_cast<std::byte *>(first)),
            HLIB_FREE_TOKEN.name, 0);
        first->next = tail;
        tail->previous = first;
    }
#endif
}

#if SHENMUE_IS_DC
void SetHeapLowerBound(void *taskData) {
    TaskData *const header = headerFromData(taskData);
    if (taskData) {
        const std::uint32_t status = header->status;
        if (status == HLIB_USED_TOKEN) {
            header->previous = SHENMUE_HLIB_NULL;
            permanentFirstBlock = dynamicFirstFree;
            dynamicFirstFree = header->next;
        }
    }
}

void SetHeapUpperBound(void *taskData) {
    TaskData *const header = headerFromData(taskData);
    if (taskData) {
        const std::uint32_t status = header->status;
        if (status == HLIB_USED_TOKEN) {
            header->next = SHENMUE_HLIB_NULL;
            permanentListTail = dynamicListHead;
            dynamicListHead = header;
        }
    }
}
#endif

void *FreeUnusedHeapMem() {
#if SHENMUE_IS_DC
    TaskData *entry = dynamicFirstFree;
    while (entry != dynamicListHead) {
        if (entry->status == HLIB_USED_TOKEN && (entry->alignment & 1) == 0) {
            FreeHeapSpace(dataFromHeader(entry), -1);
            entry = dynamicFirstFree;
        } else {
            entry = entry->next;
        }
    }
    return SHENMUE_HLIB_NULL;
#else
    for (TaskData *entry = dynamicFirstBlock; entry != dynamicListTail; entry = entry->next) {
        if (entry->status.name == HLIB_USED_TOKEN.name && (entry->alignment & 1) == 0) {
            FreeHeapSpace(dataFromHeader(entry), -1);
        }
    }
    SHENMUE_HLIB_STATIC_ITERATION {
#if SHENMUE_S2_GCC295
        StaticStorageEntry &storage = staticStorage[storageIndex];
#endif
        setEntry(&storage.header, static_cast<std::uint32_t>(' '), 0, HLIB_FREE_TOKEN.name, 0);
    }
    return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(staticStorage.data()) + sizeof(staticStorage) + offsetof(TaskData, status));
#endif
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
#if SHENMUE_IS_DC
    TaskData *entry = dynamicFirstFree;
    ++entry;
    return entry;
#else
    return dataFromHeader(dynamicFirstFree);
#endif
}

void *GetPointerFromDynamicHeapOffsetBase(std::int32_t offset) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    return reinterpret_cast<void *>(offset);
#else
    return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(DynamicHeapBase) + sizeof(StaticStorageEntry) + offset);
#endif
}

std::int32_t GetDynamicHeapOffset(const void *pointer) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    return reinterpret_cast<std::int32_t>(pointer);
#else
    const std::intptr_t offset = reinterpret_cast<std::intptr_t>(pointer) - reinterpret_cast<std::intptr_t>(DynamicHeapBase) - sizeof(StaticStorageEntry);
    assert(offset >= SHENMUE_HLIB_INT_MIN && offset <= SHENMUE_HLIB_INT_MAX);
    return static_cast<std::int32_t>(offset);
#endif
}

std::uint32_t GetTaskDataName(const void *taskData) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (taskData && (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) == 0) {
        const TaskData *header = headerFromData(const_cast<void *>(taskData));
        const std::uint32_t status = static_cast<std::uint32_t>(header->status);
        if (status == static_cast<std::uint32_t>(HLIB_USED_TOKEN))
            return static_cast<std::uint32_t>(header->name);
    }
#else
    if (!taskData || (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) != 0)
#endif
    return 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const TaskData *header = headerFromData(const_cast<void *>(taskData));
    return header->status.name == HLIB_USED_TOKEN.name ? header->name.name : 0;
#endif
}

static void *GetNextTaskData(void *taskData) {
    if (taskData && (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) == 0) {
        TaskData *const next = headerFromData(taskData)->next;
        if (next != dynamicListHead && next)
            return dataFromHeader(next);
    }
    return SHENMUE_HLIB_NULL;
}

std::size_t GetTaskDataSize(const void *taskData) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (taskData && (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) == 0)
        return headerFromData(const_cast<void *>(taskData))->contentSize - sizeof(TaskData);
#else
    if (!taskData || (reinterpret_cast<std::uintptr_t>(taskData) & 0x1fU) != 0)
#endif
    return 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return headerFromData(const_cast<void *>(taskData))->contentSize - sizeof(TaskData);
#endif
}

#if SHENMUE_IS_DC
void *FindTaskDataByName(std::uint32_t name) {
    for (void *data = GetDynamicUseStoragePtr(); data; data = GetNextTaskData(data)) {
        if (GetTaskDataName(data) == name)
            return data;
    }
    return SHENMUE_HLIB_NULL;
}
#endif

bool ShrinkTaskData(void *taskData, std::size_t size) {
    if (!taskData)
        return false;
    if (!SHENMUE_IS_DC && size <= 0x20)
        return true;
    TaskData *entry = headerFromData(taskData);
    const std::size_t desiredSize = (size + sizeof(TaskData) + 31) & ~SHENMUE_HLIB_VALUE(std::size_t, 31);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (static_cast<std::uint32_t>(entry->status) != static_cast<std::uint32_t>(HLIB_USED_TOKEN) || entry->contentSize < desiredSize)
#else
    if (entry->status.name != HLIB_USED_TOKEN.name || entry->contentSize < desiredSize)
#endif
        return false;
    if (entry->contentSize > desiredSize + sizeof(TaskData)) {
        TaskData *next = entry->next;
        TaskData *remainder = addBytes(entry, desiredSize);
        std::size_t remainderSize = entry->contentSize - desiredSize;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        remainder->name = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
        remainder->status = static_cast<std::uint32_t>(HLIB_FREE_TOKEN);
#else
        remainder->name.name = HLIB_FREE_TOKEN.name;
        remainder->status.name = HLIB_FREE_TOKEN.name;
#endif
        remainder->previous = entry;
        entry->next = remainder;
        entry->contentSize = desiredSize;
        remainder->next = next;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (static_cast<std::uint32_t>(next->status) != static_cast<std::uint32_t>(HLIB_USED_TOKEN)) {
#else
        if (next->status.name != HLIB_USED_TOKEN.name) {
#endif
            remainderSize += next->contentSize;
            next = next->next;
            remainder->next = next;
        }
        next->previous = remainder;
        remainder->contentSize = remainderSize;
    }
    return true;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeTasks(TaskCallback callback) {
#else
Task *InitializeTasks(TaskCallback callback) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t index = 0;
    do {
        const std::int32_t nextIndex = index + 1;
        SHENMUE_HLIB_MEMSET(&taskStorage[index], 0, sizeof(Task));
        taskStorage[index].taskToken = static_cast<std::int32_t>(kNone);
        taskStorage[index].next = &taskStorage[nextIndex];
        index = nextIndex;
    } while (index < static_cast<std::int32_t>(kTaskCount));
#else
    for (std::size_t index = 0; index < kTaskCount; ++index) {
        Task *const task = taskAt(index);
        SHENMUE_HLIB_MEMSET(task, 0, sizeof(Task));
        task->taskToken = static_cast<std::int32_t>(kNone);
        task->next = index + 1 < kTaskCount ? taskAt(index + 1) : SHENMUE_HLIB_NULL;
    }
#endif

    {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        Task &rootTask = taskStorage[0];
        Task *const taskQueue = taskStorage + 1;
#endif
        rootTask.taskToken = static_cast<std::int32_t>(kRoot);
        rootTask.activeCallback = callback;
        rootTask.taskIndex = 0;
        rootTask.nextTaskIndex = 1;
        rootTask.next = &rootTask;
        rootTask.nextInQueue = &rootTask;
        NextTask = &taskQueue[0];
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    taskQueuePause.paused = 0;
#endif
#if SHENMUE_S2_GCC295
    for (index = 0; index < static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(moduleTaskQueue)); ++index)
        moduleTaskQueue[index] = &taskStorage[0];
    for (index = 0; index < static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(postUpdateTasks)); ++index)
        postUpdateTasks[index] = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    for (std::size_t index = 0; index < SHENMUE_HLIB_COUNT(postUpdateData); ++index)
        postUpdateData[index] = 0;
#endif
#else
    moduleTaskQueue.fill(&rootTask);
    postUpdateTasks.fill(SHENMUE_HLIB_NULL);
    postUpdateData.fill(SHENMUE_HLIB_NULL);
#endif
    postUpdateCount = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#if SHENMUE_S2_GCC295
    for (std::size_t index = 0; index < SHENMUE_HLIB_COUNT(preservedCallbacks); ++index) {
        preservedCallbacks[index].task = 0;
        preservedCallbacks[index].callback = 0;
        preservedCallbacks[index].taskData = 0;
    }
#else
    preservedCallbacks.fill({});
#endif
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    CurrentTask = &taskStorage[0];
#else
    preservedCallbackCount = 0;
    taskQueuePaused = false;
    CurrentTask = &rootTask;
    return &rootTask;
#endif
}

void MainLoop() {
    CurrentTask->activeCallback(CurrentTask->taskData);
    CurrentTask = CurrentTask->nextInQueue;
}

// @verified - p
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t HasActiveTaskCallback(const Task *task) {
#else
bool HasActiveTaskCallback(const Task *task) {
#endif
#if SHENMUE_IS_DC
    if (task) {
        return task->activeCallback != SHENMUE_HLIB_NULL;
    }
    return false;
#else
    return task && task->activeCallback;
#endif
}

#if SHENMUE_IS_DC
Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken) {
#else
Task *EnqueueTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (nextFunctionIndex >= 16 || taskQueuePause.paused || !NextTask) {
#else
    if (nextFunctionIndex >= 16 || taskQueuePaused || !NextTask) {
#endif
        return SHENMUE_HLIB_NULL;
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
    task->child = SHENMUE_HLIB_NULL;
    task->cleanupParent = SHENMUE_HLIB_NULL;
    task->delayedCallback = SHENMUE_HLIB_NULL;
    task->periodicCallback = SHENMUE_HLIB_NULL;
    task->taskData = SHENMUE_HLIB_NULL;
    task->nextSibling = SHENMUE_HLIB_NULL;
    task->taskToken = taskToken;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    *reinterpret_cast<std::uint16_t *>(&task->freeTaskDataOnDestroy) = 0;
    for (int index = nextFunctionIndex; index < 16; ++index) {
        if (moduleTaskQueue[index] == moduleTask)
            moduleTaskQueue[index] = task;
    }
    if (CurrentTask) {
        if ((task->previousSibling = CurrentTask->cleanupParent) != SHENMUE_HLIB_NULL)
            CurrentTask->cleanupParent->nextSibling = task;
        else
            CurrentTask->child = task;
        CurrentTask->cleanupParent = task;
    } else {
        task->previousSibling = SHENMUE_HLIB_NULL;
    }
#else
    task->freeTaskDataOnDestroy = 0;
    task->unused_0f = 0;

    for (std::size_t index = nextFunctionIndex; index < SHENMUE_HLIB_COUNT(moduleTaskQueue); ++index) {
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
        task->previousSibling = SHENMUE_HLIB_NULL;
    }
#endif
    return task;
}
#if SHENMUE_IS_DC
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::int8_t taskFlags, std::int32_t taskToken) {
#else
Task *EnqueueTaskWithoutParameterWrapper(TaskCallback callback, std::uint8_t taskFlags, std::int32_t taskToken, char *description) {
    (void)description;
#endif
    return EnqueueTaskWithoutParameter(callback, CurrentTask->taskIndex, taskFlags, taskToken);
}

#if SHENMUE_IS_DC
Task *EnqueueTaskRelativeIndex(TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::int32_t taskToken) {
    return EnqueueTaskWithoutParameter(callback, static_cast<std::int8_t>(indexDelta + CurrentTask->taskIndex), taskFlags, taskToken);
}
#endif

#if SHENMUE_IS_DC
Task *EnqueueTaskWithParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
#else
Task *EnqueueTaskWithParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::size_t size, shen_name taskToken) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Task *const task = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, static_cast<std::int32_t>(static_cast<std::uint32_t>(taskToken)));
#else
    Task *const task = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, static_cast<std::int32_t>(taskToken.name));
#endif
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
#if SHENMUE_IS_DC
    if (AllocateTASK(task, size))
        return task;
    AssignCallbackForTask(task, reinterpret_cast<TaskCallback>(DestroyCurrentTask));
    return SHENMUE_HLIB_NULL;
#else
    void *const taskData = AllocateTaskData(size, SHENMUE_HLIB_VALUE(shen_name, kTask));
    AssignDynamicAlignmentForTaskData(taskData);
    task->taskData = taskData;
    if (taskData) {
        return task;
    }
    task->activeCallback = destroyCurrentTaskCallback;
    task->delayedCallback = SHENMUE_HLIB_NULL;
    task->periodicCallback = SHENMUE_HLIB_NULL;
    return SHENMUE_HLIB_NULL;
#endif
}

#if SHENMUE_IS_DC
Task *EnqueueTaskWithParameterWrapper(TaskCallback callback, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    return EnqueueTaskWithParameter(callback, CurrentTask->taskIndex, taskFlags, size, taskToken);
}

Task *EnqueueTaskWithParameterRelativeIndex(TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    return EnqueueTaskWithParameter(callback, static_cast<std::int8_t>(indexDelta + CurrentTask->taskIndex), taskFlags, size, taskToken);
}

Task *QueueTaskWithParameterAfterTask(
    Task *task, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    if (!task)
        return SHENMUE_HLIB_NULL;
    Task *const oldCurrent = CurrentTask;
    CurrentTask = task;
    Task *const result = EnqueueTaskWithParameter(callback, nextFunctionIndex, taskFlags, size, taskToken);
    CurrentTask = oldCurrent;
    return result;
}

Task *QueueTaskWithParameterAfterTaskSameIndex(Task *task, TaskCallback callback, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    if (task)
        QueueTaskWithParameterAfterTask(task, callback, task->taskIndex, taskFlags, size, taskToken);
    return SHENMUE_HLIB_NULL;
}

Task *QueueTaskWithParameterAfterTaskRelativeIndex(
    Task *task, TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    if (task)
        return QueueTaskWithParameterAfterTask(task, callback, static_cast<std::int8_t>(indexDelta + task->taskIndex), taskFlags, size, taskToken);
    return SHENMUE_HLIB_NULL;
}
#endif

#if SHENMUE_S2_GCC295
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken) {
#else
#if SHENMUE_S2_GCC295
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken) {
#else
Task *CreateNewCurrentTaskWithoutParameter(TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
#endif
#endif
    Task *const oldCurrent = CurrentTask;
    CurrentTask = SHENMUE_HLIB_NULL;
    Task *const result = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, taskToken);
    CurrentTask = oldCurrent;
    return result;
}

#if SHENMUE_IS_DC
Task *CreateNewCurrentTaskWithParameter(TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::size_t size, shen_name taskToken) {
    Task *const oldCurrent = CurrentTask;
    CurrentTask = SHENMUE_HLIB_NULL;
    Task *const result = EnqueueTaskWithParameter(callback, nextFunctionIndex, taskFlags, size, taskToken);
    CurrentTask = oldCurrent;
    return result;
}
#endif

#if SHENMUE_S2_GCC295
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken) {
#else
#if SHENMUE_S2_GCC295
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::int8_t nextFunctionIndex, std::int8_t taskFlags, std::int32_t taskToken) {
#else
Task *QueueTaskAfterCurrentTask(Task *newCurrent, TaskCallback callback, std::uint8_t nextFunctionIndex, std::uint8_t taskFlags, std::int32_t taskToken) {
#endif
#endif
    if (!newCurrent) {
        return SHENMUE_HLIB_NULL;
    }
    Task *const oldCurrent = CurrentTask;
    CurrentTask = newCurrent;
    Task *const result = EnqueueTaskWithoutParameter(callback, nextFunctionIndex, taskFlags, taskToken);
    CurrentTask = oldCurrent;
    return result;
}

#if SHENMUE_IS_DC
Task *QueueTaskAfterTaskSameIndex(Task *task, TaskCallback callback, std::int8_t taskFlags, std::int32_t taskToken) {
    if (task)
        return QueueTaskAfterCurrentTask(task, callback, task->taskIndex, taskFlags, taskToken);
    return SHENMUE_HLIB_NULL;
}

Task *QueueTaskAfterTaskRelativeIndex(Task *task, TaskCallback callback, std::int8_t indexDelta, std::int8_t taskFlags, std::int32_t taskToken) {
    if (task)
        return QueueTaskAfterCurrentTask(task, callback, static_cast<std::int8_t>(indexDelta + task->taskIndex), taskFlags, taskToken);
    return SHENMUE_HLIB_NULL;
}
#endif

// @verified - p
std::intptr_t DestroyTaskInternal(Task *task) {
    if (!task || !task->activeCallback) {
        return 0;
    }
#if SHENMUE_IS_DC
    const std::int32_t wasPaused = taskQueuePause.paused;
    if (wasPaused) {
        UnpauseTaskEnqueue();
    }
    for (std::int32_t index = static_cast<std::int8_t>(task->taskIndex); index < 16; ++index) {
        if (moduleTaskQueue[index] == task) {
            moduleTaskQueue[index] = task->next;
        }
    }
#else
    const bool wasPaused = taskQueuePaused;
    if (wasPaused) {
        UnpauseTaskEnqueue();
    }
    if (task->taskIndex < 16) {
        for (std::size_t index = task->taskIndex; index < SHENMUE_HLIB_COUNT(moduleTaskQueue); ++index) {
            if (moduleTaskQueue[index] == task) {
                moduleTaskQueue[index] = task->next;
            }
        }
    }
#endif
    task->next->nextInQueue = task->nextInQueue;
    task->nextInQueue->next = task->next;
    task->next = NextTask;
    NextTask = task;
    task->activeCallback = SHENMUE_HLIB_NULL;
    for (Task *child = task->child; child; child = child->nextSibling) {
        child->parent = SHENMUE_HLIB_NULL;
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Task *const parent = task->parent;
    if (parent) {
        if (parent->child == task) {
            parent->child = task->nextSibling;
#else
    if (task->parent) {
        if (task->parent->child == task) {
            task->parent->child = task->nextSibling;
#endif
        }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (parent->cleanupParent == task) {
            parent->cleanupParent = task->previousSibling;
#else
        if (task->parent->cleanupParent == task) {
            task->parent->cleanupParent = task->previousSibling;
#endif
        }
    }
    if (task->previousSibling) {
        task->previousSibling->nextSibling = task->nextSibling;
    }
    if (task->nextSibling) {
        task->nextSibling->previousSibling = task->previousSibling;
    }
#if SHENMUE_IS_DC
    task->previousSibling = SHENMUE_HLIB_NULL;
    task->nextSibling = SHENMUE_HLIB_NULL;
    task->child = SHENMUE_HLIB_NULL;
    task->cleanupParent = SHENMUE_HLIB_NULL;
    if (task->taskData && (task->freeTaskDataOnDestroy & 1U) == 0)
        FreeTASK(task);
#else
    void *const taskData = task->taskData;
    task->previousSibling = SHENMUE_HLIB_NULL;
    task->nextSibling = SHENMUE_HLIB_NULL;
    task->child = SHENMUE_HLIB_NULL;
    task->cleanupParent = SHENMUE_HLIB_NULL;
    if (taskData && (task->freeTaskDataOnDestroy & 1U) == 0) {
        FreeHeapSpace(taskData, static_cast<std::int32_t>(kTask));
        task->taskData = SHENMUE_HLIB_NULL;
    }
#endif
    if (wasPaused) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        PauseTaskRangeExecution(taskQueuePause.firstIndex, taskQueuePause.lastIndex);
#else
        PauseTaskRangeExecution(pauseFirstIndex, pauseLastIndex);
#endif
    }
    return 1;
}

// @verified - p
std::intptr_t DestroyTask(Task *task) {
    return DestroyTaskInternal(task);
}

// @verified - p
std::intptr_t DestroyCurrentTask() {
    return DestroyTaskInternal(CurrentTask);
}

#if SHENMUE_IS_DC
// @verified - d
void DestroyCurrentEventTask() {
    DestroyTaskInternal(CurrentTask);
}

void DestroyAllTasksAtIndex(std::int8_t taskIndex) {
    DestroyTasksAtIndex(taskIndex, -1);
}

std::intptr_t DestroyTasksAtIndex(std::int8_t taskIndex, std::int8_t nextTaskIndex) {
    if (!taskQueuePause.paused) {
        std::int32_t queueIndex = taskIndex - 1;
        if (static_cast<std::uint8_t>(queueIndex) <= 14) {
            Task *task = moduleTaskQueue[queueIndex];
            for (task = task->nextInQueue;; task = task->nextInQueue) {
                if (static_cast<std::int8_t>(task->taskIndex) == taskIndex) {
                    if (nextTaskIndex == -1 || static_cast<std::int8_t>(task->nextTaskIndex) == nextTaskIndex)
                        task->activeCallback = reinterpret_cast<TaskCallback>(DestroyCurrentTask);
                } else {
                    return 1;
                }
            }
        }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PauseTaskRangeExecution(std::int32_t firstTaskIndex, std::int32_t lastTaskIndex) {
    const std::int8_t first = static_cast<std::int8_t>(firstTaskIndex);
    const std::int8_t last = static_cast<std::int8_t>(lastTaskIndex);
    if (first > 0) {
        if (last > 15 || first > last)
            return 0;
        if (taskQueuePause.paused)
            UnpauseTaskEnqueue();
        Task *const previous = moduleTaskQueue[first - 1];
        Task *const next = moduleTaskQueue[last]->nextInQueue;
        taskQueuePause.firstIndex = first;
        taskQueuePause.lastIndex = last;
        taskQueuePause.previousTask = previous;
        taskQueuePause.currentNextInQueue = next;
        taskQueuePause.previousTaskNextInQueue = previous->nextInQueue;
        taskQueuePause.currentNextInQueueNextTask = next->next;
        taskQueuePause.paused = 1;
        previous->nextInQueue = next;
        next->next = previous;
        return 1;
    }
    return 0;
}
#else
void PauseTaskRangeExecution(std::uint16_t firstTaskIndex, std::uint16_t lastTaskIndex) {
#if SHENMUE_S2_GCC295
    const std::uint8_t first = static_cast<std::uint8_t>(firstTaskIndex);
#else
    const auto first = static_cast<std::uint8_t>(firstTaskIndex);
#endif
#if SHENMUE_S2_GCC295
    const std::uint8_t last = static_cast<std::uint8_t>(lastTaskIndex);
#else
    const auto last = static_cast<std::uint8_t>(lastTaskIndex);
#endif
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
#endif

void UnpauseTaskEnqueue() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (!taskQueuePause.paused)
        return;
    Task *const previous = taskQueuePause.previousTask;
    Task *const next = taskQueuePause.currentNextInQueue;
    previous->nextInQueue = taskQueuePause.previousTaskNextInQueue;
    next->next = taskQueuePause.currentNextInQueueNextTask;
    taskQueuePause.paused = 0;
#else
    if (!taskQueuePaused) {
        return;
    }
    taskQueuePaused = false;
    previousTask->nextInQueue = previousTaskNextInQueue;
    currentNextInQueue->next = currentNextInQueueNextTask;
#endif
}

#if SHENMUE_IS_DC
std::int32_t IsTaskEnqueuePaused() {
    return taskQueuePause.paused;
}

void SetTaskCycleLimit(std::int32_t limit) {
    taskCycleLimit = limit;
}

void AdvanceTaskCycleIndex() {
    taskCycleIndex = (taskCycleIndex + 1) % (taskCycleRange / 22 + 1);
}
#endif

void *GetTaskParameter(Task *task) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (!task) {
        return SHENMUE_HLIB_NULL;
#else
    return task ? task->taskData : SHENMUE_HLIB_NULL;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return task->taskData;
}
#endif
Task *GetCurrentTask() {
    return CurrentTask;
}
#if SHENMUE_IS_DC
Task *GetCurrentTaskNext() {
    return GetTaskNext(CurrentTask);
}
Task *GetTaskNext(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->next;
}
Task *GetCurrentTaskNextInQueue() {
    return GetTaskNextInQueue(CurrentTask);
}
Task *GetTaskNextInQueue(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->nextInQueue;
}
#endif
Task *GetCurrentParentTask() {
#if SHENMUE_IS_DC
    return GetTaskParent(CurrentTask);
#else
    return CurrentTask ? CurrentTask->parent : SHENMUE_HLIB_NULL;
#endif
}
#if SHENMUE_IS_DC
Task *GetTaskParent(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->parent;
}
Task *GetCurrentTaskChild() {
    return GetTaskChild(CurrentTask);
}
Task *GetTaskChild(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->child;
}
Task *GetCurrentTaskPreviousSibling() {
    return GetTaskPreviousSibling(CurrentTask);
}
Task *GetTaskPreviousSibling(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->previousSibling;
}
Task *GetCurrentTaskNextSibling() {
    return GetTaskNextSibling(CurrentTask);
}
Task *GetTaskNextSibling(const Task *task) {
    if (!task) {
        return SHENMUE_HLIB_NULL;
    }
    return task->nextSibling;
}
#endif
void *GetCurrentTaskData() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return GetTaskParameter(CurrentTask);
#else
    return CurrentTask ? CurrentTask->taskData : SHENMUE_HLIB_NULL;
#endif
}

#if SHENMUE_S2_GCC295
std::int8_t GetCurrentTaskIndex() {
#else
#if SHENMUE_S2_GCC295
std::int8_t GetCurrentTaskIndex() {
#else
std::uint8_t GetCurrentTaskIndex() {
#endif
#endif
#if SHENMUE_IS_DC
    return GetTaskIndex(CurrentTask);
#else
    return CurrentTask ? CurrentTask->taskIndex : 0;
#endif
}

#if SHENMUE_IS_DC
std::int8_t GetTaskIndex(const Task *task) {
    return task ? static_cast<std::int8_t>(task->taskIndex) : 0;
}

std::int8_t GetCurrentTaskNextTaskIndex() {
    return GetTaskNextTaskIndex(CurrentTask);
}

std::int8_t GetTaskNextTaskIndex(const Task *task) {
    return task ? static_cast<std::int8_t>(task->nextTaskIndex) : 0;
}

Task *GetFirstTaskAtIndex(std::int8_t taskIndex) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Task &rootTask = taskStorage[0];
#endif
    if (taskIndex == 0)
        return &rootTask;
    const int previousIndex = taskIndex - 1;
    if (static_cast<std::uint8_t>(previousIndex) < 15) {
        Task *const task = moduleTaskQueue[previousIndex]->nextInQueue;
        if (static_cast<std::int8_t>(task->taskIndex) == taskIndex)
            return task;
    }
    return SHENMUE_HLIB_NULL;
}

Task *GetLastTaskAtIndex(std::int8_t taskIndex) {
    if (static_cast<std::uint8_t>(taskIndex - 1) < 15) {
        Task *const task = moduleTaskQueue[taskIndex];
        if (static_cast<std::int8_t>(task->taskIndex) == taskIndex)
            return task;
    }
    return SHENMUE_HLIB_NULL;
}
#endif

std::intptr_t ExecuteCallbackNow(TaskCallback callback, void *taskData) {
#if defined(SHENMUE_TARGET_S2_DC_EU)
    if (g_renderCallbackState)
#endif
    callback(taskData);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if ((g_loadingFlag & 0x102U) == 0 && static_cast<std::uint8_t>(GetCurrentTaskIndex() - 3U) <= 10U) {
        if (preservedCallbackCount >= static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(preservedCallbacks)))
#else
    const std::uint8_t taskIndex = GetCurrentTaskIndex();
    if ((g_loadingFlag & 0x102U) == 0 && static_cast<std::uint8_t>(taskIndex - 3U) <= 10U) {
        if (preservedCallbackCount >= static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(preservedCallbacks))) {
#endif
            return 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        preservedCallbacks[static_cast<std::size_t>(preservedCallbackCount)].task = GetCurrentTask();
        preservedCallbacks[static_cast<std::size_t>(preservedCallbackCount)].callback = callback;
        preservedCallbacks[static_cast<std::size_t>(preservedCallbackCount)].taskData = taskData;
        ++preservedCallbackCount;
#else
        }
        PreservedCallback &preserved = preservedCallbacks[static_cast<std::size_t>(preservedCallbackCount++)];
        preserved.task = GetCurrentTask();
        preserved.callback = callback;
        preserved.taskData = taskData;
#endif
    }
    return 1;
}

#if SHENMUE_IS_DC
void CancelPreservedCallbacks(void *taskData) {
    for (std::int32_t index = 0; index < preservedCallbackCount; ++index) {
        if (preservedCallbacks[index].taskData == taskData)
            preservedCallbacks[index].callback = SHENMUE_HLIB_NULL;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t debugPauseBlinkFrame = 0;

namespace {

void ExecDebugPause_Callback(void *) {
    const std::uint32_t entryFlags = g_loadingFlag;
    if (entryFlags & 0xeU) {
        if (!(entryFlags & 0x10U)) {
            debugPauseBlinkFrame = (debugPauseBlinkFrame + 1) & 31;
            if (debugPauseBlinkFrame <= 16) {
                SetDebugScreenTextPosition(17, 15);
                shenmue_s2_event_DebugLogToScreen("PAUSE");
            }
            if (g_inputBuffer[0].held == 6)
                g_loadingFlag |= 0x10U;
        }
        if ((g_loadingFlag & 0x20U) || (g_inputBuffer[0].pressedPriority & 0x800U)) {
            g_loadingFlag &= ~0x1eU;
            if (!(g_loadingFlag & 0x100U))
                UnpauseTaskEnqueue();
        }
    } else {
        if (!(g_loadingFlag & 0x20U) && (g_inputBuffer[0].pressedPriority & 0x800U)) {
            g_loadingFlag |= 2U;
            if (!(g_loadingFlag & 0x100U)) {
                PauseTaskRangeExecution(3, 13);
                debugPauseBlinkFrame = 0;
            }
        }
        if (!(g_loadingFlag & 2U)) {
            g_loadingFlag &= ~0x100U;
            UnpauseTaskEnqueue();
        }
    }
    if (entryFlags & 0x102U) {
        Task *const savedCurrentTask = CurrentTask;
        for (std::int32_t index = 0; index < preservedCallbackCount; ++index) {
            CurrentTask = preservedCallbacks[index].task;
            (*preservedCallbacks[index].callback)(preservedCallbacks[index].taskData);
        }
        CurrentTask = savedCurrentTask;
    }
    if (!(g_loadingFlag & 2U))
        preservedCallbackCount = 0;
}

} // namespace

void ExecDebugPause() {
    g_execPauseTask = EnqueueTaskWithParameter(ExecDebugPause_Callback, 15, 2, sizeof(std::uint32_t), SHEN_NAME4('P', 'A', 'U', 'S'));
    g_loadingFlag = 1;
}
#endif

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
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::int8_t index = static_cast<std::int8_t>(nextTaskIndex);
    Task *task = &taskStorage[0];
    do {
        if (static_cast<std::int8_t>(task->nextTaskIndex) == index && --searchDepth <= 0)
            return task;
        task = task->nextInQueue;
    } while (task != &taskStorage[0]);
    return SHENMUE_HLIB_NULL;
#else
    Task *task = &rootTask;
    while (task->nextTaskIndex != nextTaskIndex || --searchDepth > 0) {
        task = task->nextInQueue;
        if (task == &rootTask) {
            return SHENMUE_HLIB_NULL;
        }
    }
    return task;
#endif
}

#if SHENMUE_IS_DC
Task *FindTaskAtIndexWithDepth(std::int8_t taskIndex, std::int8_t nextTaskIndex, std::int32_t searchDepth) {
    Task *task = GetFirstTaskAtIndex(taskIndex);
    do {
        if (static_cast<std::int8_t>(task->nextTaskIndex) == nextTaskIndex && --searchDepth <= 0)
            return task;
        task = task->nextInQueue;
    } while (static_cast<std::int8_t>(task->taskIndex) == taskIndex);
    return SHENMUE_HLIB_NULL;
}
#endif

std::intptr_t AssignTaskDataToTask(Task *task, void *taskData) {
    if (!task || task->taskData) {
        return 0;
    }
    task->taskData = taskData;
    return 1;
}

std::intptr_t SetCurrentTaskData(void *taskData) {
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
    task->delayedCallback = SHENMUE_HLIB_NULL;
    task->periodicCallback = SHENMUE_HLIB_NULL;
}

#if SHENMUE_IS_DC
void SetCurrentTaskToDestroy() {
    SetCurrentTaskTo(reinterpret_cast<TaskCallback>(DestroyCurrentTask));
}

void SetTaskToDestroy(Task *task) {
    AssignCallbackForTask(task, reinterpret_cast<TaskCallback>(DestroyCurrentTask));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MarkCurrentTaskForCleanup() {
    EV_HLKillOtherHierarchyTask(CurrentTask);
}
#else
Task *MarkCurrentTaskForCleanup() {
    if (!CurrentTask) {
        return SHENMUE_HLIB_NULL;
    }
    CurrentTask->activeCallback = destroyCurrentTaskCallback;
    if (CurrentTask->child) {
        HLKillOtherHierarchyTask(CurrentTask->child);
    }
    return CurrentTask;
}
#endif

Task *HLKillOtherHierarchyTask(Task *task) {
#if SHENMUE_IS_DC
    if (task) {
        task->activeCallback = destroyCurrentTaskCallback;
        if (task->child)
            markTaskHierarchyForCleanup(task->child);
    }
#else
    if (task) {
        markTaskHierarchyForCleanup(task);
    }
#endif
    return task;
}

// Native RAX is incidental; task callbacks discard the forwarded value.
void EV_HLKillOtherHierarchyTask(Task *task) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (task) {
        task->activeCallback = reinterpret_cast<TaskCallback>(DestroyCurrentTask);
        if (task->child)
            markTaskHierarchyForCleanup(task->child);
    }
#else
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
#endif
}

#if SHENMUE_IS_DC
std::intptr_t DestroyTasksFromIndexMatching(std::int8_t taskIndex, std::int8_t nextTaskIndex) {
    if (taskQueuePause.paused)
        return 0;
    for (; taskIndex < 16; ++taskIndex)
        DestroyTasksAtIndex(taskIndex, nextTaskIndex);
    return 1;
}
#endif

std::intptr_t DestroyTasksFromIndex(std::uint8_t taskIndex) {
#if SHENMUE_IS_DC
    return DestroyTasksFromIndexMatching(static_cast<std::int8_t>(taskIndex), -1);
#else
    if (taskQueuePaused) {
        return 0;
    }
    for (; taskIndex < SHENMUE_HLIB_COUNT(moduleTaskQueue); ++taskIndex) {
        if (taskIndex != 0) {
            Task *task = moduleTaskQueue[taskIndex - 1]->nextInQueue;
            while (task->taskIndex == taskIndex) {
                task->activeCallback = destroyCurrentTaskCallback;
                task = task->nextInQueue;
            }
        }
    }
    return 1;
#endif
}

void FreeCurrentTaskData() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    FreeTASK(CurrentTask);
#else
    if (CurrentTask && CurrentTask->taskData) {
        FreeHeapSpace(CurrentTask->taskData, static_cast<std::int32_t>(kTask));
        CurrentTask->taskData = SHENMUE_HLIB_NULL;
    }
#endif
}

#if SHENMUE_IS_DC
std::int32_t GetTaskToken(const Task *task) {
    if (task != SHENMUE_HLIB_NULL)
        return 0;
    return task->taskToken;
}

std::int32_t SetTaskToken(Task *task, std::int32_t taskToken) {
    if (task)
        return 0;
    const std::int32_t previous = task->taskToken;
    task->taskToken = taskToken;
    return previous;
}

std::intptr_t TransferCurrentTaskData(Task *destination) {
    return TransferTaskData(CurrentTask, destination);
}

std::intptr_t TransferTaskData(Task *source, Task *destination) {
    if (!source)
        return 0;
    if (!destination)
        return 0;
    void *const taskData = destination->taskData;
    if (taskData)
        return 0;
    destination->taskData = source->taskData;
    source->taskData = taskData;
    return 1;
}

void SetCurrentTaskKeepData() {
    SetTaskKeepData(CurrentTask);
}

void SetTaskKeepData(Task *task) {
    if (task)
        *reinterpret_cast<std::uint16_t *>(&task->freeTaskDataOnDestroy) |= 1U;
}

void ClearCurrentTaskKeepData() {
    ClearTaskKeepData(CurrentTask);
}

void ClearTaskKeepData(Task *task) {
    if (task)
        *reinterpret_cast<std::uint16_t *>(&task->freeTaskDataOnDestroy) &= ~1U;
}
#endif

std::intptr_t SetCurrentTaskCustomUserData(TaskCallback callback, void *callbackData) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (postUpdateCount < static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(postUpdateTasks))) {
        CurrentTask->postUpdateCallback = callback;
        postUpdateTasks[postUpdateCount] = CurrentTask;
        postUpdateData[postUpdateCount] = callbackData;
        ++postUpdateCount;
        return 1;
    }
#else
    if (postUpdateCount >= static_cast<std::int32_t>(SHENMUE_HLIB_COUNT(postUpdateTasks))) {
#endif
    return 0;
}
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    CurrentTask->postUpdateCallback = callback;
    const std::size_t index = static_cast<std::size_t>(postUpdateCount++);
    postUpdateTasks[index] = CurrentTask;
    postUpdateData[index] = callbackData;
    return 1;
}
#endif

void ProcessTaskPostUpdateCallbacks() {
    Task *const savedCurrentTask = CurrentTask;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    for (std::int32_t index = 0; index < postUpdateCount; ++index) {
        CurrentTask = postUpdateTasks[index];
        CurrentTask->postUpdateCallback(postUpdateData[index]);
#else
    std::int32_t index = 0;
    while (index < postUpdateCount) {
        const std::size_t queueIndex = static_cast<std::size_t>(index++);
        CurrentTask = postUpdateTasks[queueIndex];
        CurrentTask->postUpdateCallback(postUpdateData[queueIndex]);
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    CurrentTask = savedCurrentTask;
#endif
    postUpdateCount = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    CurrentTask = savedCurrentTask;
#endif
}

#if SHENMUE_IS_DC
std::int32_t EnqueueTaskTree(const TaskTreeDescriptor *descriptor) {
    Task *const savedCurrentTask = CurrentTask;
    std::int32_t result = 1;
    CurrentTask = EnqueueTaskWithParameter(descriptor->callback, descriptor->taskIndex, descriptor->taskFlags, descriptor->size, descriptor->taskToken);
    if (!CurrentTask) {
        if (taskTreeCount)
#if SHENMUE_IS_S2
            EV_HLKillOtherHierarchyTask(taskTreeRoot);
#else
            HLKillOtherHierarchyTask(taskTreeRoot);
#endif
        result = 0;
    } else {
        ++taskTreeCount;
    }
    if (result && descriptor->child)
        result = EnqueueTaskTree(descriptor->child);
    CurrentTask = savedCurrentTask;
    if (result && descriptor->sibling)
        result = EnqueueTaskTree(descriptor->sibling);
    return result;
}

Task *CreateTaskTree(const TaskTreeDescriptor *descriptor) {
    taskTreeCount = 0;
    taskTreeRoot = NextTask;
    if (EnqueueTaskTree(descriptor))
        return taskTreeRoot;
    return SHENMUE_HLIB_NULL;
}
#endif

void SetupTimedTask(Task *task, std::int32_t tickTime) {
    if (!task) {
        return;
    }
#if SHENMUE_IS_DC
    if (!task->delayedCallback) {
        task->delayedCallback = task->activeCallback;
        task->timedTask.timeUntilExecution = tickTime;
        task->activeCallback = UpdateTimedTask;
    }
    task->timedTask.timeUntilExecution = tickTime;
#else
    if (!task->delayedCallback) {
        task->delayedCallback = task->activeCallback;
        task->activeCallback = UpdateTimedTask;
    }
    task->timedTask.timeUntilExecution = static_cast<float>(tickTime);
#endif
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
#if SHENMUE_IS_DC
    if (--CurrentTask->timedTask.timeUntilExecution == 0) {
        CurrentTask->activeCallback = CurrentTask->delayedCallback;
        CurrentTask->delayedCallback = SHENMUE_HLIB_NULL;
    }
#else
    if (CurrentTask->timedTask.timeUntilExecution <= 0.0f) {
        return;
    }
    CurrentTask->timedTask.timeUntilExecution -= shenGetScaledDeltaTickTime(1.0f);
    if (CurrentTask->timedTask.timeUntilExecution <= 0.0f) {
        CurrentTask->timedTask.timeUntilExecution = 0.0f;
        CurrentTask->activeCallback = CurrentTask->delayedCallback;
        CurrentTask->delayedCallback = SHENMUE_HLIB_NULL;
    }
#endif
}

#if SHENMUE_IS_DC
void ResolveCurrentDeferredCallback() {
    ResolveDeferredCallback(CurrentTask);
}
#endif

void ResolveDeferredCallback(Task *task) {
    if (!task || !task->delayedCallback) {
        return;
    }
    if (task->periodicCallback && task->activeCallback == UpdateLoopedTask) {
        task->periodicCallback = task->delayedCallback;
    } else {
        task->activeCallback = task->delayedCallback;
    }
    task->delayedCallback = SHENMUE_HLIB_NULL;
}

#if SHENMUE_IS_DC
void SetupCurrentLoopedTask(std::int16_t intervalFrames) {
    SetupLoopedTask(CurrentTask, intervalFrames);
}

void SetupLoopedTask(Task *task, std::int16_t intervalFrames) {
    if (!task)
        return;
    if (!task->periodicCallback) {
        task->periodicCallback = task->activeCallback;
        task->activeCallback = UpdateLoopedTask;
    }
    task->loopedTask.loopIntervalFrames = static_cast<std::uint16_t>(intervalFrames);
    task->loopedTask.remainingLoops = static_cast<std::uint16_t>(intervalFrames);
}

void StopCurrentLoopedTask() {
    StopLoopedTask(CurrentTask);
}

void StopLoopedTask(Task *task) {
    if (!task || !task->periodicCallback)
        return;
    if (task->delayedCallback && task->activeCallback == UpdateTimedTask)
        task->delayedCallback = task->periodicCallback;
    else
        task->activeCallback = task->periodicCallback;
    task->periodicCallback = SHENMUE_HLIB_NULL;
}
#endif

void SetScaledDeltaTick(float ticks) {
    scaledDeltaTick = ticks;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t g_memoryMonitorActive;
void DisableMemoryMonitor() {
    g_memoryMonitorActive = 0;
}
#endif

float GetScaledDeltaTick() {
    return scaledDeltaTick;
}

#if SHENMUE_S2_GCC295
}
}
} // namespace shenmue::s2::HLib
namespace shenmue {
namespace s2 {
#else
} // namespace shenmue::s2::HLib

namespace shenmue::s2 {
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ExecuteTasks() {
    HLib::CurrentTask = &HLib::taskStorage[0];
    for (;;) {
        g_currentTaskName = static_cast<std::uint32_t>(HLib::CurrentTask->taskToken);
        HLib::CurrentTask->activeCallback(HLib::CurrentTask->taskData);
        HLib::CurrentTask = HLib::CurrentTask->nextInQueue;
    }
}
#endif

#if SHENMUE_IS_DC
void *AllocateCurrentTASK(std::size_t size) {
    return AllocateTASK(HLib::CurrentTask, size);
}
#endif

// @verified - p
void *AllocateTASK(HLib::Task *task, std::size_t size) {
    if (!task || task->taskData)
        return SHENMUE_HLIB_NULL;
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
    task->taskData = SHENMUE_HLIB_NULL;
}

#if SHENMUE_S2_GCC295
}
} // namespace shenmue::s2
#else
} // namespace shenmue::s2
#endif

#undef SHENMUE_HLIB_CONSTANT
#undef SHENMUE_HLIB_ALIGN
#undef SHENMUE_HLIB_NULL
#undef SHENMUE_HLIB_COUNT
#undef SHENMUE_HLIB_VALUE
#undef SHENMUE_HLIB_HEAP
#undef SHENMUE_HLIB_MEMSET
#undef SHENMUE_HLIB_STATIC_ITERATION
#undef SHENMUE_HLIB_INT_MIN
#undef SHENMUE_HLIB_INT_MAX
