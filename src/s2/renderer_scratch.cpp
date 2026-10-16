#include "s2/renderer_scratch.hpp"
#include "d3t/threads.hpp"
#include <atomic>
#include <cstdlib>

namespace shenmue::s2 {
thread_local RendererScratchState g_rendererScratch{.reserved00 = {}, .current = nullptr, .sliceIndex = -1, .base = nullptr};
void *g_rendererScratchMemory = nullptr;
std::uint64_t g_rendererScratchSliceSize = 0;
namespace {
std::atomic<std::uint32_t> g_rendererScratchSliceCounter{0};
}
void InitializeRendererScratch() {
    if (g_rendererScratchMemory)
        return;
    const std::uint32_t threads = d3t::g_workerThreadCount;
    const std::uint32_t added = threads + 1;
    const std::uint32_t slices = added < threads ? threads : added;
    g_rendererScratchSliceSize = 0x200000;
    g_rendererScratchMemory = std::malloc(g_rendererScratchSliceSize * slices);
}
void FreeRendererScratch() {
    std::free(g_rendererScratchMemory);
    g_rendererScratchMemory = nullptr;
}
void *GetRendererScratchBlock(std::size_t size) {
    if (g_rendererScratch.sliceIndex == -1) {
        const std::uint32_t slice = g_rendererScratchSliceCounter.fetch_add(1, std::memory_order_seq_cst);
        g_rendererScratch.sliceIndex = slice;
        g_rendererScratch.current = static_cast<std::byte *>(g_rendererScratchMemory) + g_rendererScratchSliceSize * slice;
        g_rendererScratch.base = g_rendererScratch.current;
    }
    std::byte *const result = g_rendererScratch.current;
    g_rendererScratch.current += size;
    return result;
}
} // namespace shenmue::s2
