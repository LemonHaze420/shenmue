#include "d3t/render_handles.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {
namespace {
std::array<RenderHandleEntry, 0x400> g_d3tRenderHandles{};

RenderHandleEntry *findEntry(std::uint32_t handle) {
    if (handle == 0) {
        return nullptr;
    }
    const std::uint32_t index = static_cast<std::uint16_t>(handle);
    if (index >= g_d3tRenderHandles.size()) {
        return nullptr;
    }
    RenderHandleEntry &entry = g_d3tRenderHandles[index];
    if (entry.generation != (handle >> 16U) || entry.active.load(std::memory_order_seq_cst) == 0) {
        return nullptr;
    }
    return &entry;
}

} // namespace

std::uint32_t d3t_AllocateRenderHandle() {
    for (std::uint32_t index = 0; index < g_d3tRenderHandles.size(); ++index) {
        RenderHandleEntry &entry = g_d3tRenderHandles[index];
        std::uint8_t expected = 0;
        if (!entry.active.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
            continue;
        }

        const std::uint32_t incremented = entry.generation + 1;
        const std::uint32_t generation = incremented < 0x1000000U ? incremented : 1U;
        entry.referenceCount.exchange(0, std::memory_order_seq_cst);
        entry.generation = generation;
        return (generation << 16U) | index;
    }
    return 0;
}

bool d3t_IsRenderHandleValid(std::uint32_t handle) {
    return findEntry(handle) != nullptr;
}

bool d3t_IsRenderHandlePending(std::uint32_t handle) {
    RenderHandleEntry *const entry = findEntry(handle);
    return entry && entry->referenceCount.load(std::memory_order_seq_cst) != 0;
}

bool d3t_AddRenderHandleReference(std::uint32_t handle) {
    RenderHandleEntry *const entry = findEntry(handle);
    if (!entry) {
        return false;
    }
    entry->referenceCount.fetch_add(1, std::memory_order_seq_cst);
    return true;
}

void d3t_DecrementRenderHandleReference(std::uint32_t handle) {
    if (RenderHandleEntry *const entry = findEntry(handle))
        entry->referenceCount.fetch_sub(1, std::memory_order_seq_cst);
}

void d3t_ReleaseRenderHandle(std::uint32_t &handle) {
    if (RenderHandleEntry *const entry = findEntry(handle)) {
        entry->referenceCount.fetch_sub(1, std::memory_order_seq_cst);
    }

    d3t_CloseRenderHandle(handle);
}

void d3t_CloseRenderHandle(std::uint32_t &handle) {
    if (d3t_IsRenderHandlePending(handle) || handle == 0) {
        return;
    }

    const std::uint32_t index = static_cast<std::uint16_t>(handle);
    if (index < g_d3tRenderHandles.size()) {
        RenderHandleEntry &entry = g_d3tRenderHandles[index];
        if (entry.generation == (handle >> 16U) &&
            (entry.referenceCount.load(std::memory_order_seq_cst) == 0 || entry.active.load(std::memory_order_seq_cst) == 0)) {
            std::uint8_t expected = 1;
            entry.active.compare_exchange_strong(expected, 0, std::memory_order_seq_cst);
        }
    }
    handle = 0;
}

} // namespace shenmue::d3t
