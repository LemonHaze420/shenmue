#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct EmulatorPositionColourVertex {
    float x, y, z, w;
    std::uint32_t colour;
};
struct EmulatorPositionColourTextureVertex {
    float x, y, z, w;
    std::uint32_t colour;
    float u, v;
};
struct EmulatorPositionTextureVertex {
    float x, y, z, w, u, v;
};
static_assert(sizeof(EmulatorPositionColourVertex) == 20);
static_assert(sizeof(EmulatorPositionColourTextureVertex) == 28);
static_assert(sizeof(EmulatorPositionTextureVertex) == 24);

void SetEmulatorDefaultRenderFlags();
void BindEmulatorTexture(std::uint32_t texture);
void DrawEmulatorVertices(const void *data, std::uint32_t stride, std::uint32_t count, std::uint32_t layout, std::uint32_t pixelShader);
void LoadEmulatorFile(const char *path, void *destination, std::int32_t limit);
std::uint32_t LoadEmulatorImage(const char *path);

} // namespace shenmue::s2
