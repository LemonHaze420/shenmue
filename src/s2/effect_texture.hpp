#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct LegacyEffectTexture {
    std::uint32_t kind;
    std::uint32_t planes;
    std::uint32_t pixelFlags;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t reserved14;
    std::uint32_t formatFlags;
    std::uint32_t reserved1c;
    std::uint64_t handle;
    std::array<std::byte, 8> reserved28;
};
static_assert(sizeof(LegacyEffectTexture) == 48);
static_assert(offsetof(LegacyEffectTexture, formatFlags) == 24);
static_assert(offsetof(LegacyEffectTexture, handle) == 32);

inline constexpr std::array<std::array<std::uint32_t, 2>, 23> kLegacyEffectFormatFlags{{
    {0x0100, 0x004}, {0x0200, 0x005}, {0x0300, 0x008}, {0x0400, 0x009},
    {0x0500, 0x024}, {0x0600, 0x025}, {0x0700, 0x024}, {0x0800, 0x025},
    {0x0900, 0x000}, {0x0a00, 0x001}, {0x0b00, 0x010}, {0x0c00, 0x011},
    {0x0d00, 0x004}, {0x0e00, 0x000}, {0x0f00, 0x001}, {0x1000, 0x008},
    {0x1100, 0x009}, {0x8000, 0x080}, {0x8700, 0x081}, {0x8100, 0x100},
    {0x8200, 0x101}, {0x8300, 0x200}, {0x8400, 0x201}}};
extern std::uint32_t g_legacyEffectTextureCount;
std::int32_t InitializeLegacyEffectTexture(LegacyEffectTexture &texture, std::uint32_t width, std::uint32_t height, std::uint16_t format);
std::int32_t UploadLegacyEffectPvr(LegacyEffectTexture &texture, std::span<const std::byte> image);
LegacyEffectTexture *CreateEffectTextureReference(
    std::uint32_t width, std::uint32_t height, std::uint16_t format, std::span<const std::byte> image, std::int32_t slot);
LegacyEffectTexture *GetEffectTextureReference(std::int32_t slot);
const std::uint32_t *GetEffectTextureReferenceDimensions(std::int32_t slot);
void InitializeEffectTextureReferences();
void DestroyEffectTextureReferences();

} // namespace shenmue::s2
