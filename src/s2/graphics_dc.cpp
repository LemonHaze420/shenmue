#include "s2/initialization_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/frame_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include <cstring>
#include "platform/dc/sdk.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
// @verified - d
void SetLightModelColour(std::uint32_t index, const Vector4 *colour) {
    std::memcpy(&g_lightRecords[index][0x14 / 4], colour, sizeof(*colour));
}

// @verified - d
void SetLightAttenuation(std::uint32_t index, float constant, float linear, float quadratic) {
    g_lightRecords[index][0x4c / 4] = floatAsU32(constant);
    g_lightRecords[index][0x50 / 4] = floatAsU32(linear);
    g_lightRecords[index][0x54 / 4] = floatAsU32(quadratic);
}

std::uint32_t g_modifierVolumeMode = 0;
std::int32_t g_modifierVolumeListControl = 0;
std::int32_t g_printCharacterWidth = 0;
std::int32_t g_printCharacterHeight = 0;

// @verified - d
void SetModifierVolumeMode(std::uint32_t mode) {
    g_modifierVolumeMode = (mode == 2 ? 2U : 1U) << 29;
}

// @verified - d
void SetModifierVolumeListControl(std::int32_t control) {
    g_modifierVolumeListControl = control == -1 ? -1 : 1;
}

// @verified - d
void SetPrintCharacterSize(std::int32_t width, std::int32_t height) {
    g_printCharacterWidth = width;
    g_printCharacterHeight = height;
}
} // namespace sdk
} // namespace dc
} // namespace shenmue
namespace shenmue {
namespace s2 {
void ReadWeatherRipplePvrTexture(const void *data);
std::int32_t g_spriteBufferCapacities[4];
std::int32_t g_commonIconTextureCount;
// @verified - d
void LoadCommonIconSprites(PakGlobalData *package) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_commonIconTextures) / sizeof(g_commonIconTextures[0])); ++index) {
        g_commonIconTextures[index] = -1;
    }
    const std::uint32_t bytes = GetPakFileSize(package, "COMICON.SPR", PACKAGE_BINARY_EXTENSION);
    const void *const texture = FileLoader(package, "COMICON.SPR", PACKAGE_BINARY_EXTENSION);
    SetNamedTextureBank(2);
    g_commonIconTextureCount = ReadTitleSpriteTextures(texture, bytes, g_commonIconTextures);
}
// @verified - d
void LoadLoadScreenSpriteTexture(PakGlobalData *package) {
    const TextureChunk *const texture = reinterpret_cast<const TextureChunk *>(FileLoader(package, "LD_nl.SPR", PACKAGE_BINARY_EXTENSION));
    if (texture) {
        SetNamedTextureBank(2);
        g_loadScreenSpriteTexture = LoadAndFindTEXN_Wrapper(texture);
    }
}
void InitializeSprites(std::int32_t useCurrentArea, std::int32_t initializeCommonTextures) {
    const MapRenderProfile &profile = MatchMapRenderProfile(useCurrentArea ? g_currentMap : shen_name());
    dc::sdk::SetVertexWorkBase(reinterpret_cast<void *>(0x0cec0000));
    g_spriteBufferCapacities[3] = static_cast<std::int32_t>(profile.field10);
    dc::sdk::SetTextureMemorySize(g_spriteBufferCapacities[3]);
    InitializeTextureCellStorage();
    g_spriteBufferCapacities[2] = static_cast<std::int32_t>(profile.field0c);
    dc::sdk::SetPunchThroughBufferSize(g_spriteBufferCapacities[2]);
    g_spriteBufferCapacities[0] = static_cast<std::int32_t>(profile.field04);
    g_spriteBufferCapacities[1] = static_cast<std::int32_t>(profile.field08);
    dc::sdk::SetVertexBufferSizes(0, g_spriteBufferCapacities[0], g_spriteBufferCapacities[1], 0, 0);
    for (std::size_t rangeIndex95 = 0; rangeIndex95 < sizeof(g_vertexBufferHighWater) / sizeof((g_vertexBufferHighWater)[0]); ++rangeIndex95) {
        std::uint32_t &highWater = (g_vertexBufferHighWater)[rangeIndex95];
        highWater = 0;
    }
    g_additionalVertexBufferHighWater = 0;
    kmSetCheapShadowMode(0x80);
    dc::sdk::SetModifierVolumeMode(1);
    dc::sdk::SetModifierVolumeListControl(1);
    dc::sdk::SetBorderColour(0);
    if (!initializeCommonTextures) {
        dc::sdk::InitializePrint(0);
    } else {
        PakLoadTaskEntry_Enqueue(3, 0);
        ReadPakFileFromDirectory("/MISC", "TEXTURES", 4);
        pakLoadFlush();
        PakGlobalData *const package = GetPakLoadTaskDataPointer();
        if (package) {
            const void *const ripple = FileLoader(package, "w_hamon.pvr", PACKAGE_BINARY_EXTENSION);
            if (ripple)
                ReadWeatherRipplePvrTexture(ripple);
            dc::sdk::InitializePrint(0);
            SetNamedTextureBank(2);
            LoadCommonIconSprites(package);
            LoadLoadScreenSpriteTexture(package);
            ClearNamedTextureBank();
        }
        FreePakLoadTask();
    }
    dc::sdk::EnablePrint();
    dc::sdk::SetPrintCharacterSize(16, 16);
}
} // namespace s2
} // namespace shenmue
#endif
