#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/texture_cache_dc.hpp"
#include "s2/palette_dc.hpp"
#include "platform/dc/ninja.hpp"

namespace shenmue { namespace dc { namespace sdk {

std::uint32_t g_polygonContextMode = 0;
std::uint32_t g_modelTextureIdBase = 0;

std::uint32_t PrepareMT7MeshMaterial(void *mesh, const void *textures, std::uint32_t *material, std::uint32_t **next, std::int32_t remap) {
    bool loadedTexture = false;
    std::uint32_t result = 0;
    *next = 0;
    if (!material) {
        if (reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->state < 0)
            return 0;
        material = static_cast<std::uint32_t *>(mesh) + 6;
        if (!*material)
            return 0;
    }
    do {
        const std::int32_t index = static_cast<std::int32_t>(material[8]);
        std::int32_t texture = index;
        if (index >= 0) {
            if (remap == 1)
                texture = reinterpret_cast<const s2::MT7TextureBinding *>(textures)[index].texture;
            const std::uint32_t globalIndex = static_cast<std::uint32_t>(texture) + g_modelTextureIdBase;
            s2::TextureIndexMap &mapping = s2::g_textureIndexMap[static_cast<std::int32_t>(globalIndex)];
            if (!mapping.references)
                mapping.surface = s2::AllocateTextureCacheSurface();
            ++mapping.references;
            texture = mapping.surface;
            s2::TextureCacheSurface &surface = s2::g_textureCacheSurfaces[texture];
            if (!surface.references) {
                const s2::TextureImage *const image =
                    reinterpret_cast<const s2::TextureImage *>(reinterpret_cast<const s2::MT7TextureBinding *>(textures) + index);
                result |= s2::CreateTextureCacheSurface(image, texture, image->pixels);
                s2::UpdateTextureSurfacePacketState(texture);
                surface.index = texture;

                loadedTexture = true;
            }
            ++surface.references;
        }
        if (index < 0) {
            material[0] |= 1U;
            material[1] |= 0x400000U;
        }
        material[0] = (material[0] & 0xfffcffffU) | 0x8c0000U | (g_polygonContextMode << 16U);
        if (texture < 0)
            s2::ClearTextureSurfacePacketState(material);
        else
            s2::BindTextureSurfacePacketState(material, texture);
        const std::uint32_t format = (material[3] >> 27U) & 7U;
        if (!s2::g_paletteDisabled) {
            if (g_mt7PaletteData && format - 5U < 2U) {
                const std::int32_t index = static_cast<std::int32_t>(material[15]);
                if (index < s2::g_paletteSlotCount) {
                    s2::PaletteSlot &slot = s2::g_paletteSlots[index];
                    std::int32_t firstBlock;
                    if (!slot.references) {
                        const s2::PaletteData &palette = reinterpret_cast<const s2::PaletteData *>(g_mt7PaletteData)[index];
                        slot.mode = static_cast<std::uint8_t>(palette.mode);
                        slot.blockCount = static_cast<std::uint8_t>((static_cast<std::uint32_t>(palette.colourCount) + 15U) >> 4U);
                        slot.references = 1;
                        slot.index = static_cast<std::int16_t>(index);
                        firstBlock = s2::FindFreePaletteBlocks(palette.mode, palette.colourCount);
                        if (firstBlock < 0) {
                            firstBlock = 0;
                        } else {
                            slot.firstBlock = static_cast<std::int16_t>(firstBlock);
                            s2::ReservePaletteBlocks(firstBlock, palette.colourCount);
                            njSetPaletteData(firstBlock * 16, palette.colourCount, const_cast<void *>(palette.colours));
                        }
                    } else {
                        slot.references = static_cast<std::int16_t>(static_cast<std::uint16_t>(slot.references) + 1U);
                        firstBlock = slot.firstBlock;
                    }
                    material[3] = (material[3] & 0xf81fffffU) | (static_cast<std::uint32_t>(firstBlock) << 21U);
                }
            }
        } else if (format - 5U < 2U) {
            material[3] = (material[3] & 0xf81fffffU) | (material[15] << 21U);
        }
        MT7TexturePreparationNullsub();
        material = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(material) + material[19] + 0x50U);
        if (!*material)
            return result;
        if (loadedTexture) {
            *next = material;
            return result;
        }
    } while (true);
}

} } } // namespace shenmue::dc::sdk

namespace shenmue { namespace s2 {

std::uint32_t ReleaseMT7MeshMaterials(void *mesh, const void *textures, std::int32_t remap) {
    if (reinterpret_cast<const MT7MeshHeader *>(mesh)->state < 0)
        return 0;
    std::uint32_t *material = static_cast<std::uint32_t *>(mesh) + 6;
    if (!*material)
        return 0;
    std::uint32_t result = 0;
    do {
        std::int32_t index = static_cast<std::int32_t>(material[8]);
        if (index >= 0) {
            if (remap && textures)
                index = reinterpret_cast<const MT7TextureBinding *>(textures)[index].texture;
            const std::int32_t resolved = static_cast<std::int32_t>(static_cast<std::uint32_t>(index) + dc::sdk::g_modelTextureIdBase);
            TextureIndexMap &mapping = g_textureIndexMap[resolved];
            if (mapping.references && --mapping.references == 0) {
                const std::int32_t slot = mapping.surface;
                TextureCacheSurface &surface = g_textureCacheSurfaces[slot];
                if (!surface.creationStatus && kmFreeTexture(&surface.surface))
                    result = 1;
                surface.references = 0;
                surface.index = -1;
                FreeTextureCacheSurface(slot);
                mapping.surface = -1;

            }
        }
        if (!g_paletteDisabled && dc::sdk::g_mt7PaletteData && ((material[3] >> 27U) & 7U) - 5U < 2U)
            ReleasePaletteSlot(static_cast<std::int32_t>(material[15]));
        material = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(material) + material[19] + 0x50U);
    } while (*material);
    return result;
}

} } // namespace shenmue::s2

#endif
