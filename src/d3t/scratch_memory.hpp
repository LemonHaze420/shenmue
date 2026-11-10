#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct RendererScratchState {
    std::byte reserved00[8];
    std::byte *current;
    std::int64_t sliceIndex;
    std::byte *base;
};
static_assert(sizeof(RendererScratchState) == 0x20);
static_assert(offsetof(RendererScratchState, current) == 8);
static_assert(offsetof(RendererScratchState, sliceIndex) == 0x10);
static_assert(offsetof(RendererScratchState, base) == 0x18);

extern thread_local RendererScratchState g_rendererScratch;
extern void *g_globalMemory;
extern std::uint64_t g_globalMemorySize;
extern std::atomic<std::uint32_t> g_threadSliceCounter;

void *AllocateMemoryFromGlobalPool(std::size_t size);
void *GetGlobalPoolCheckpoint();
void RestoreGlobalPoolCheckpoint(void *checkpoint);
void ResetGlobalPoolSlices();
void *GetRendererScratchBlock(std::size_t size);
void RestoreRendererScratchBlock(void *block);

} // namespace shenmue::d3t
