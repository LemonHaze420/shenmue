#include "s1/d3t/scratch_memory.hpp"

#include "s1/initialization.hpp"

#include <atomic>
#include <cstdint>

namespace shenmue::s1 {

namespace {

struct ThreadScratchState {
    std::byte *current = nullptr;
    std::int64_t sliceIndex = -1;
    std::byte *base = nullptr;
};

std::atomic<std::uint32_t> g_threadSliceCounter{0};
thread_local ThreadScratchState g_threadScratch;

void EnsureThreadScratch() {
    if (g_threadScratch.sliceIndex != -1)
        return;
    if (!g_globalMemory)
        return;
    const std::uint32_t slice = g_threadSliceCounter.fetch_add(1, std::memory_order_seq_cst);
    g_threadScratch.sliceIndex = slice;
    auto *const global = static_cast<std::byte *>(g_globalMemory);
    g_threadScratch.current = global + static_cast<std::size_t>(g_globalMemorySize) * slice;
    g_threadScratch.base = g_threadScratch.current;
}

} // namespace

void *AllocateMemoryFromGlobalPool(std::size_t size) {
    EnsureThreadScratch();
    if (size == 0 || !g_threadScratch.current)
        return nullptr;
    std::byte *const allocation = g_threadScratch.current;
    g_threadScratch.current += size;
    return allocation;
}

void *GetGlobalPoolCheckpoint() {
    EnsureThreadScratch();
    return g_threadScratch.current;
}

void RestoreGlobalPoolCheckpoint(void *checkpoint) {
    EnsureThreadScratch();
    if (!g_threadScratch.base)
        return;
    g_threadScratch.current = static_cast<std::byte *>(checkpoint);
}

void ResetGlobalPoolSlices() {
    g_threadSliceCounter.store(0, std::memory_order_seq_cst);
    g_threadScratch = {};
}

} // namespace shenmue::s1
