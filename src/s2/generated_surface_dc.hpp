#pragma once
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {
#define GENERATED_SURFACE_STORAGE SHEN_NAME4('S', 'U', 'R', 'F')
#define GENERATED_SURFACE_PIXELS SHEN_NAME4('S', 'H', 'A', 'D')
static const std::uint32_t kGeneratedSurfaceLayouts[2]= {0x900, 0x100};
static const std::uint32_t kGeneratedSurfacePixelModes[4]= {0, 1, 2, 3};
struct GeneratedTextureSurface {
    std::int16_t width, height;
    std::uint8_t layout, pixelMode;
    std::int16_t mapCount;
    std::int16_t slots[2];
    void *pixels;
    std::uint8_t activeSlot;
    std::byte retained11[3];
};
typedef char Layout_generated_surface_dc_0[(sizeof(GeneratedTextureSurface) == 0x14) ? 1 : -1];
typedef char Layout_generated_surface_dc_1[(offsetof(GeneratedTextureSurface, slots) == 8) ? 1 : -1];
typedef char Layout_generated_surface_dc_2[(offsetof(GeneratedTextureSurface, pixels) == 0x0c) ? 1 : -1];
typedef char Layout_generated_surface_dc_3[(offsetof(GeneratedTextureSurface, activeSlot) == 0x10) ? 1 : -1];
std::int32_t AllocateGeneratedTextureSlot(std::uint32_t width, std::uint32_t height, std::uint32_t format);
GeneratedTextureSurface *AllocateSHAD_SRUF(std::uint32_t width, std::uint32_t height, std::int32_t layout, std::int32_t pixelMode);
void *GetGeneratedSurfacePixels(GeneratedTextureSurface *surface);
void PurgeGeneratedSurfacePixels(GeneratedTextureSurface *surface);
void CommitGeneratedSurface(GeneratedTextureSurface *surface);
void EnsureGeneratedSurfaceBackBuffer(GeneratedTextureSurface *surface);
std::int32_t RetireGeneratedSurface(GeneratedTextureSurface *surface);
std::int32_t GetGeneratedSurfaceTexture(GeneratedTextureSurface *surface);
std::int32_t GetGeneratedSurfaceBackTexture(GeneratedTextureSurface *surface);
void SwapGeneratedSurfaceBuffer(GeneratedTextureSurface *surface);
} } // namespace shenmue::s2

#endif
