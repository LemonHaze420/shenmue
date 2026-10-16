#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {


struct EffectTextureSource {
    std::uint32_t reserved00;
    std::uint32_t textureHandle;
    std::uint32_t renderHandle;
    std::uint32_t state; // 0: pending, 1: admitted, 2: unavailable
    std::uint32_t flags;
    std::array<char, 280> path;
};
static_assert(sizeof(EffectTextureSource) == 0x12c);
static_assert(offsetof(EffectTextureSource, textureHandle) == 4);
static_assert(offsetof(EffectTextureSource, state) == 12);
static_assert(offsetof(EffectTextureSource, path) == 20);

bool CopyEffectTextureSource(std::uint32_t key, EffectTextureSource &source);
bool LoadEffectTextureSource(const char *path, std::uint32_t key, std::uint32_t flags);
bool PumpEffectTextureSources();
void SweepEffectTextureSources();
std::uint32_t ResolveEffectTextureSource(std::uint32_t key, std::uint32_t projectionMode);

} // namespace shenmue::s2
