#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct RendererScratchState {
    std::byte reserved00[8];
    std::byte *current;
    std::int64_t sliceIndex;
    std::byte *base;
};
static_assert(sizeof(RendererScratchState) == 0x20);
static_assert(offsetof(RendererScratchState, current) == 8);
extern thread_local RendererScratchState g_rendererScratch;
extern void *g_rendererScratchMemory;
extern std::uint64_t g_rendererScratchSliceSize;
void InitializeRendererScratch();
void FreeRendererScratch();
void *GetRendererScratchBlock(std::size_t size);
} // namespace shenmue::s2
