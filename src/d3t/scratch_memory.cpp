#include "d3t/scratch_memory.hpp"

namespace shenmue::d3t {

thread_local RendererScratchState g_rendererScratch{.reserved00 = {}, .current = nullptr, .sliceIndex = -1, .base = nullptr};
void *g_globalMemory = nullptr;
std::uint64_t g_globalMemorySize = 0;
std::atomic<std::uint32_t> g_threadSliceCounter{0};

namespace {

void EnsureThreadScratch() {
    if (g_rendererScratch.sliceIndex != -1)
        return;
    if (!g_globalMemory)
        return;
    const std::uint32_t slice = g_threadSliceCounter.fetch_add(1, std::memory_order_seq_cst);
    g_rendererScratch.sliceIndex = slice;
    g_rendererScratch.current = static_cast<std::byte *>(g_globalMemory) + static_cast<std::size_t>(g_globalMemorySize) * slice;
    g_rendererScratch.base = g_rendererScratch.current;
}

} // namespace

void *AllocateMemoryFromGlobalPool(std::size_t size) {
    EnsureThreadScratch();
    if (size == 0 || !g_rendererScratch.current)
        return nullptr;
    std::byte *const allocation = g_rendererScratch.current;
    g_rendererScratch.current += size;
    return allocation;
}

void *GetGlobalPoolCheckpoint() {
    EnsureThreadScratch();
    return g_rendererScratch.current;
}

void RestoreGlobalPoolCheckpoint(void *checkpoint) {
    EnsureThreadScratch();
    if (!g_rendererScratch.base)
        return;
    g_rendererScratch.current = static_cast<std::byte *>(checkpoint);
}

void ResetGlobalPoolSlices() {
    g_threadSliceCounter.store(0, std::memory_order_seq_cst);
    g_rendererScratch = {.reserved00 = {}, .current = nullptr, .sliceIndex = -1, .base = nullptr};
}

void *GetRendererScratchBlock(std::size_t size) {
    if (g_rendererScratch.sliceIndex == -1) {
        const std::uint32_t slice = g_threadSliceCounter.fetch_add(1, std::memory_order_seq_cst);
        g_rendererScratch.sliceIndex = slice;
        g_rendererScratch.current = static_cast<std::byte *>(g_globalMemory) + g_globalMemorySize * slice;
        g_rendererScratch.base = g_rendererScratch.current;
    }
    std::byte *const result = g_rendererScratch.current;
    g_rendererScratch.current += size;
    return result;
}

// @verified - p
void RestoreRendererScratchBlock(void *block) {
    if (block)
        g_rendererScratch.current = static_cast<std::byte *>(block);
}

} // namespace shenmue::d3t
