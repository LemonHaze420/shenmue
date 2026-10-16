#include "s1/mip_streaming.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/initialization.hpp"
#include "s1/simple_shader.hpp"

namespace shenmue::s1 {
namespace {
std::uint32_t g_mipMapTaskHandle = 0;
}

std::int32_t LoadMipMap_Wrapper(void *) {
    if (d3t::g_pendingMipMapRequestCount != 0 && !IsMainRenderHandleBusy() && !g_HasPPFXRendered) {
        d3t::LoadMipMaps();
    }
    return 0;
}

std::uint32_t QueuePendingMipMapLoads() {
    if (d3t::d3t_IsTaskActive(g_mipMapTaskHandle))
        return g_mipMapTaskHandle;
    const d3t::D3TTaskDescriptor task{.data = nullptr, .callback = LoadMipMap_Wrapper, .completion = nullptr, .priority = 4};
    g_mipMapTaskHandle = d3t::d3t_QueueTask(&task, 4);
    return g_mipMapTaskHandle;
}

void UpdateMipMaps() {
    if (d3t::g_pendingMipMapRequestCount != 0 && !IsMainRenderHandleBusy() && !g_HasPPFXRendered)
        QueuePendingMipMapLoads();
    d3t::g_mipMapFrameCounters.fill(0);
}
} // namespace shenmue::s1
