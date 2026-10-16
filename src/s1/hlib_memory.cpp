#include "s1/hlib.hpp"

#include <bit>
#include <cassert>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#if defined(_WIN32) && defined(NDEBUG)
extern "C" _ACRTIMP void __cdecl _wassert(const wchar_t *message, const wchar_t *file, unsigned line);
#endif

namespace shenmue::s1 {

HLib::TaskData *g_pFirstTaskData = nullptr;
std::byte *g_pCurrentTaskData = nullptr;
HLib::TaskData *g_pFirstTaskData_0 = nullptr;
HLib::TaskData *g_pLastTaskData = nullptr;
HLib::TaskData *g_pLastTaskData_0 = nullptr;
std::byte *g_renderTaskBase = nullptr;
HLib::TaskData *g_mallocFirst = nullptr;
HLib::TaskData *g_mallocLast = nullptr;

std::uint32_t HLib_GetRenderTaskBaseOffset() {
    return HLib_GetTaskDataOffset(g_renderTaskBase);
}

std::uint32_t HLib_GetTaskDataOffset(const void *pointer) {
    if (!pointer)
        return 0;
    const std::uintptr_t offset = reinterpret_cast<std::uintptr_t>(pointer) - reinterpret_cast<std::uintptr_t>(g_pCurrentTaskData);
    if (offset > UINT32_MAX) {
#if defined(_WIN32)
        ::_wassert(L"(d3tUint64)temp64BitOffset <= UINT_MAX", L"src\\HLmallocs.c", 0x38dU);
#else
        std::fprintf(stderr, "Assertion failed: (d3tUint64)temp64BitOffset <= UINT_MAX, file src\\HLmallocs.c, line 909\n");
        std::abort();
#endif
    }
    return static_cast<std::uint32_t>(offset);
}

namespace {

constexpr std::size_t kArenaSize = 0x4000000;
constexpr std::size_t kMallocArenaSize = 0x1200000;
constexpr std::size_t kSmallBlockCount = 322;
constexpr std::int32_t kHLib = SHEN_NAME(HLib);

alignas(32) std::array<std::byte, kArenaSize> g_arena{};
alignas(32) std::array<HLib::TaskData, kSmallBlockCount * 2> g_smallBlocks{};
alignas(32) std::array<HLib::TaskData, 2> g_zero{};

void *g_mallocArena = nullptr;

HLib::TaskData *addBytes(HLib::TaskData *block, std::uint64_t bytes) {
    return reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + bytes);
}

void setFree(HLib::TaskData *block, std::uint64_t size) {
    block->taskToken = HLIB_FREE_TOKEN.name;
    block->content_size = static_cast<std::int64_t>(size);
    block->status = HLIB_FREE_TOKEN.name;
    block->alignment = 0;
}

HLib::TaskData *smallHeader(std::size_t index) {
    return &g_smallBlocks[index * 2];
}

void initializeArena(HLib::TaskData *base, std::size_t arenaSize, HLib::TaskData *&first, HLib::TaskData *&last) {
    std::memset(base, 0, arenaSize);
    first = addBytes(base, 0x40);
    last = addBytes(base, arenaSize - 0x40);

    base->taskToken = kHLib;
    base->next_ptr = first;
    base->status = HLIB_USED_TOKEN.name;
    base->alignment = 0x80;

    setFree(first, arenaSize - 0x80);
    first->prev_ptr = base;
    first->next_ptr = last;

    last->taskToken = kHLib;
    last->prev_ptr = first;
    last->status = HLIB_USED_TOKEN.name;
    last->alignment = 0x80;
}

void *allocateFromFront(std::uint64_t requestedSize, std::int32_t name) {
    const std::uint64_t size = (requestedSize + 95) & ~std::uint64_t{0x1f};
    for (HLib::TaskData *block = g_pFirstTaskData; block; block = block->next_ptr) {
        if (block->status == HLIB_USED_TOKEN.name || static_cast<std::uint64_t>(block->content_size) < size) {
            continue;
        }

        HLib::TaskData *const next = block->next_ptr;
        const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
        if (oldSize == size || oldSize <= size + 64) {
            block->taskToken = name;
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 0;
            return block + 1;
        }

        HLib::TaskData *const remainder = addBytes(block, size);
        remainder->prev_ptr = block;
        remainder->next_ptr = next;
        remainder->status = HLIB_FREE_TOKEN.name;
        remainder->taskToken = HLIB_FREE_TOKEN.name;
        remainder->content_size = static_cast<std::int64_t>(oldSize - size);
        block->next_ptr = remainder;
        if (next) {
            next->prev_ptr = remainder;
        }
        block->taskToken = name;
        block->status = HLIB_USED_TOKEN.name;
        block->alignment = 0;
        block->content_size = static_cast<std::int64_t>(size);
        return block + 1;
    }
    return nullptr;
}

} // namespace

void HLib_Init() {
    for (std::size_t index = 0; index < kSmallBlockCount; ++index) {
        HLib::TaskData *const block = smallHeader(index);
        block->taskToken = 0x20;
        block->content_size = 0;
        block->prev_ptr = g_zero.data();
        block->next_ptr = g_zero.data();
        block->status = HLIB_FREE_TOKEN.name;
        block->alignment = 0;
    }

    HLib::TaskData *const base = reinterpret_cast<HLib::TaskData *>(g_arena.data());
    initializeArena(base, kArenaSize, g_pFirstTaskData, g_pLastTaskData);
    g_pCurrentTaskData = reinterpret_cast<std::byte *>(base) + 0x80;
    g_pFirstTaskData_0 = g_pFirstTaskData;
    g_pLastTaskData_0 = g_pLastTaskData;
}

std::uint64_t HLib_Malloc() {
    void *const allocation = ::operator new(kMallocArenaSize, std::align_val_t{32}, std::nothrow);
    g_mallocArena = allocation;
    if (!allocation) {
        return 0;
    }
    initializeArena(static_cast<HLib::TaskData *>(allocation), kMallocArenaSize, g_mallocFirst, g_mallocLast);
    return 1;
}

std::uint64_t HLib_GetLargestMallocFreeBlock() {
    std::uint64_t largestFreePayload = 0;
    for (HLib::TaskData *block = g_mallocFirst; block; block = block->next_ptr) {
        const auto payloadAddress = reinterpret_cast<std::uintptr_t>(block + 1);
        if ((payloadAddress & 0x1f) != 0) {
            break;
        }
        if (block->status != HLIB_USED_TOKEN.name || block->taskToken == 0) {
            const std::uint64_t payloadSize = static_cast<std::uint64_t>(block->content_size) - 0x40;
            if (largestFreePayload < payloadSize) {
                largestFreePayload = payloadSize;
            }
        }
        if (!block->next_ptr || block->next_ptr == g_mallocLast) {
            break;
        }
    }

    return largestFreePayload;
}

std::int64_t HLib_CanAllocate64ByteBlock(std::int32_t blockCount) {
    std::uint64_t totalFreePayload = 0;
    std::uint64_t largestFreePayload = 0;
    HLib::TaskData *block = g_mallocFirst;
    do {
        const std::uintptr_t payloadAddress = reinterpret_cast<std::uintptr_t>(block + 1);
        if ((payloadAddress & 0x1f) != 0) {
            break;
        }
        if (block->status != HLIB_USED_TOKEN.name || block->taskToken == 0) {
            const std::uint64_t payloadSize = static_cast<std::uint64_t>(block->content_size) - 0x40;
            totalFreePayload += payloadSize;
            if (largestFreePayload < payloadSize) {
                largestFreePayload = payloadSize;
            }
        }
        if (!block->next_ptr || block->next_ptr == g_mallocLast) {
            break;
        }
        block = block->next_ptr;
    } while (block);

    if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(totalFreePayload)) <= 0) {
        return 0;
    }
    const std::uint32_t requested = static_cast<std::uint32_t>(blockCount) << 6;
    const std::uint32_t remaining = static_cast<std::uint32_t>(largestFreePayload) - requested;
    return std::bit_cast<std::int32_t>(remaining) > 0 ? 1 : 0;
}
void HLib_FreeUseSpaces() {
    for (HLib::TaskData *block = g_pFirstTaskData_0; block != g_pLastTaskData_0; block = block->next_ptr) {
        if (block->status == HLIB_USED_TOKEN.name && (block->alignment & 1) == 0) {
            HLib_FreeTaskData(block + 1, -1);
        }
    }
    for (std::size_t index = 0; index < kSmallBlockCount; ++index) {
        HLib::TaskData *const block = smallHeader(index);
        block->status = 0x20;
        block->task_data = nullptr;
        const std::uint64_t freeToken = static_cast<std::uint32_t>(HLIB_FREE_TOKEN.name);
        std::memcpy(block + 1, &freeToken, sizeof(freeToken));
    }
}

void *UpdateTaskData_0() {
    std::uint64_t largestPayload = 0;
    HLib::TaskData *selected = nullptr;
    for (HLib::TaskData *block = g_pFirstTaskData; block; block = block->next_ptr) {
        const auto payloadAddress = reinterpret_cast<std::uintptr_t>(block + 1);
        std::uint32_t payloadSize = 0;
        if ((payloadAddress & 0x1f) == 0) {
            if (block->status == HLIB_USED_TOKEN.name && block->taskToken != 0) {
                if (block->next_ptr == g_pLastTaskData || !block->next_ptr) {
                    break;
                }
                continue;
            }
            payloadSize = static_cast<std::uint32_t>(block->content_size) - 0x40;
        }
        if (largestPayload < payloadSize) {
            largestPayload = payloadSize;
            selected = block;
        }
        if ((payloadAddress & 0x1f) != 0 || block->next_ptr == g_pLastTaskData || !block->next_ptr) {
            break;
        }
    }

    HLib::TaskData *const next = selected->next_ptr;
    HLib::TaskData *const previous = selected->prev_ptr;
    g_pFirstTaskData = selected;
    g_pLastTaskData = next;
    g_pFirstTaskData_0 = selected;
    previous->prev_ptr = nullptr;
    previous->content_size = 0;
    next->next_ptr = nullptr;
    next->content_size = 0;
    g_pLastTaskData_0 = next;
    return selected + 1;
}

void *HLib_AllocHeapMemBlock(std::int64_t requestedSize, std::int32_t name) {
    if (requestedSize == 0) {
        return &g_zero[1];
    }
    if (static_cast<std::uint64_t>(requestedSize) <= 32) {
        for (std::size_t index = 0; index < kSmallBlockCount; ++index) {
            HLib::TaskData *const block = smallHeader(index);
            if (block->status != HLIB_USED_TOKEN.name) {
                block->taskToken = name;
                block->content_size = 96;
                block->status = HLIB_USED_TOKEN.name;
                block->alignment = 0;
                return block + 1;
            }
        }
        requestedSize = 32;
    }
    return allocateFromFront(static_cast<std::uint64_t>(requestedSize), name);
}

void *HLib_AllocMemBlock(std::uint64_t requestedSize, std::int32_t name) {
    if (requestedSize == 0) {
        return &g_zero[1];
    }
    if (requestedSize <= 32) {
        for (std::size_t index = 0; index < kSmallBlockCount; ++index) {
            HLib::TaskData *const block = smallHeader(index);
            if (block->status != HLIB_USED_TOKEN.name) {
                block->taskToken = name;
                block->content_size = 96;
                block->status = HLIB_USED_TOKEN.name;
                block->alignment = 0;
                return block + 1;
            }
        }
        requestedSize = 32;
    }

    const std::uint64_t size = (requestedSize + 95) & ~std::uint64_t{0x1f};
    for (HLib::TaskData *block = g_pLastTaskData; block; block = block->prev_ptr) {
        if (block->status == HLIB_USED_TOKEN.name || static_cast<std::uint64_t>(block->content_size) < size) {
            continue;
        }
        const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
        if (oldSize == size || oldSize <= size + 64) {
            block->taskToken = name;
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 4;
            return block + 1;
        }

        const std::uint64_t remaining = oldSize - size;
        HLib::TaskData *const used = addBytes(block, remaining);
        used->prev_ptr = block;
        used->next_ptr = block->next_ptr;
        block->next_ptr->prev_ptr = used;
        block->next_ptr = used;
        block->content_size = static_cast<std::int64_t>(remaining);
        used->taskToken = name;
        used->content_size = static_cast<std::int64_t>(size);
        used->status = HLIB_USED_TOKEN.name;
        used->alignment = 4;
        return used + 1;
    }
    return nullptr;
}

std::uint64_t HLib_FreeTaskData(HLib::TaskData *taskData, std::int32_t name) {
    if (!taskData) {
        return 0;
    }
    if (taskData == &g_zero[1]) {
        return 1;
    }

    HLib::TaskData *block = taskData - 1;
    if (block->status != HLIB_USED_TOKEN.name || ((block->alignment & 0x10) != 0 && (name & block->taskToken) != block->taskToken)) {
        return 0;
    }

    std::uint64_t size = static_cast<std::uint64_t>(block->content_size);
    if (size <= 96) {
        for (std::size_t index = 0; index < kSmallBlockCount; ++index) {
            if (smallHeader(index) + 1 == taskData) {
                block->status = HLIB_FREE_TOKEN.name;
                block->taskToken = HLIB_FREE_TOKEN.name;
                block->content_size = 0;
                return 1;
            }
        }
    }

    HLib::TaskData *const previous = block->prev_ptr;
    HLib::TaskData *const next = block->next_ptr;
    block->status = HLIB_FREE_TOKEN.name;
    block->taskToken = HLIB_FREE_TOKEN.name;
    if (previous && previous->status == HLIB_FREE_TOKEN.name) {
        size += static_cast<std::uint64_t>(previous->content_size);
        block = previous;
        previous->next_ptr = next;
    }
    const bool mergeNext = next && next->status == HLIB_FREE_TOKEN.name;
    if (next)
        next->prev_ptr = block;
    if (mergeNext) {
        size += static_cast<std::uint64_t>(next->content_size);
        block->next_ptr = next->next_ptr;
        if (next->next_ptr)
            next->next_ptr->prev_ptr = block;
    }
    block->content_size = static_cast<std::int64_t>(size);
    return 1;
}

} // namespace shenmue::s1
