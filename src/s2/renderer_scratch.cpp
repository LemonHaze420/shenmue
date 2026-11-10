#include "s2/renderer_scratch.hpp"
#include "d3t/threads.hpp"
#include <cstdlib>

namespace shenmue::s2 {
void InitializeRendererScratch() {
    if (d3t::g_globalMemory)
        return;
    const std::uint32_t threads = d3t::g_workerThreadCount;
    const std::uint32_t added = threads + 1;
    const std::uint32_t slices = added < threads ? threads : added;
    d3t::g_globalMemorySize = 0x200000;
    d3t::g_globalMemory = std::malloc(d3t::g_globalMemorySize * slices);
}
void FreeRendererScratch() {
    std::free(d3t::g_globalMemory);
    d3t::g_globalMemory = nullptr;
}
} // namespace shenmue::s2
