#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct RenderHandleEntry {
    std::atomic<std::uint32_t> referenceCount;
    std::uint32_t generation;
    std::atomic<std::uint8_t> active;
    std::byte reserved_09[3];
};

static_assert(sizeof(RenderHandleEntry) == 0xc);

std::uint32_t d3t_AllocateRenderHandle();
bool d3t_IsRenderHandleValid(std::uint32_t handle);
bool d3t_IsRenderHandlePending(std::uint32_t handle);
bool d3t_AddRenderHandleReference(std::uint32_t handle);
void d3t_DecrementRenderHandleReference(std::uint32_t handle);
void d3t_CloseRenderHandle(std::uint32_t &handle);
void d3t_ReleaseRenderHandle(std::uint32_t &handle);

} // namespace shenmue::d3t
