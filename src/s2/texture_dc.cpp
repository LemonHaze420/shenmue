#include "s2/texture_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/memo_dc.hpp"

#include "platform/dc/mt7.hpp"
#include "s2/texture_cache_dc.hpp"
#include "platform/dc/screen.hpp"

#define TX_INIT =
#define TX_ZERO
#define TX_NULL 0
#define TX_ALIGN(bytes) __attribute__((aligned(bytes)))
#define TX_I32(value) static_cast<std::int32_t>(value)
#define TX_VALUE(type, value) static_cast<type>(value)
#include "s2/generated_surface_dc.hpp"
#include "auto_test.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/hlib.hpp"
#include <cstring>
#include "platform/dc/sdk.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "s2/scene_data.hpp"
#include "s2/title_dc.hpp"
#include "s2/pak_dc.hpp"
#include <cstdio>
#include <cmath>
namespace shenmue {
namespace s2 {

std::int32_t FindNamedTexture(std::uint32_t nameLow, std::uint32_t nameHigh, NamedTextureCell **previous, NamedTextureCell **found);
std::int32_t ReleaseNamedTexture(std::int32_t index);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::uint32_t bytes, std::int32_t *textures, MemoTextureName *names);
#else
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::uint32_t bytes, std::int32_t *textures);
#endif
void InitializeTextureCache(void *storage, std::int32_t surfaceCount, std::int32_t indexCount);
std::int32_t AllocateTextureCacheSurface();
std::uint32_t CreateTextureCacheSurface(const TextureImage *image, std::int32_t index, const void *pixels);
void ReloadTextureCacheSurface(std::int32_t surfaceIndex, const void *pixels);
void UpdateTextureSurfacePacketState(std::int32_t index);
std::int32_t ReleaseTextureIndex(std::int32_t index);

std::int32_t AllocateTextureCacheSurface();
std::uint32_t CreateTextureCacheSurface(const TextureImage *image, std::int32_t index, const void *pixels);
void ReloadTextureCacheSurface(std::int32_t surfaceIndex, const void *pixels);
void UpdateTextureSurfacePacketState(std::int32_t index);
std::int32_t ReleaseTextureIndex(std::int32_t index);
std::int32_t AllocateTextureCacheSurface();
std::uint32_t CreateTextureCacheSurface(const TextureImage *image, std::int32_t index, const void *pixels);
void ReloadTextureCacheSurface(std::int32_t surfaceIndex, const void *pixels);
void UpdateTextureSurfacePacketState(std::int32_t index);
std::int32_t ReleaseTextureIndex(std::int32_t index);

namespace {

TX_ALIGN(32) std::byte textureCacheStorage[kTextureCacheStorageBytes] TX_ZERO;
PaletteSlot effectPaletteSlots[1] TX_ZERO;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t namedTextureNameText[3];
#endif

} // namespace

NamedTextureCell *g_namedTextureCells = TX_NULL;
NamedTextureCell *g_freeNamedTextureCells = TX_NULL;
NamedTextureCell *g_namedTextureBanks[2] TX_ZERO;
std::int32_t g_namedTextureBank;

// @verified - d
void InitializeNamedTextureStorage(void *storage, std::int32_t count) {
    InitializeTextureCache(storage, count, count);
    g_namedTextureCells =
        reinterpret_cast<NamedTextureCell *>(static_cast<std::byte *>(storage) + ((count + 1) * 15 + (count + 32) * 2 + (count + 32) / 32) * 4);
    for (std::int32_t index = 0; index < count; ++index) {
        g_namedTextureCells[index].index = static_cast<std::int16_t>(index);
        g_namedTextureCells[index].references = 0;
        g_namedTextureCells[index].nameLow = 0;
        g_namedTextureCells[index].nameHigh = 0;
        g_namedTextureCells[index].next = &g_namedTextureCells[index + 1];
    }
    g_namedTextureCells[count - 1].next = 0;
    g_freeNamedTextureCells = g_namedTextureCells;
    g_namedTextureBanks[0] = 0;
    g_namedTextureBanks[1] = 0;
}

// @verified - d
NamedTextureCell *FindNamedTexture(std::uint32_t nameLow, std::uint32_t nameHigh) {
    NamedTextureCell *cell = g_namedTextureBank ? *(g_namedTextureBanks + 1) : g_namedTextureBanks[0];
    while (cell) {
        if (cell->nameLow == nameLow && cell->nameHigh == nameHigh)
            return cell;
        cell = cell->next;
    }
    return TX_NULL;
}

// @verified - d
std::int32_t FindNamedTexture(std::uint32_t nameLow, std::uint32_t nameHigh, NamedTextureCell **previous, NamedTextureCell **found) {
    NamedTextureCell *cell = g_namedTextureBanks[g_namedTextureBank != 0];
    *previous = TX_NULL;
    *found = TX_NULL;
    while (cell) {
        if (cell->nameLow == nameLow && cell->nameHigh == nameHigh) {
            *found = cell;
            return cell->index;
        }
        *previous = cell;
        cell = cell->next;
    }
    return -1;
}

// @verified - d
std::int32_t FindNamedTextureIndex(std::uint32_t nameLow, std::uint32_t nameHigh) {
    const NamedTextureCell *const cell = FindNamedTexture(nameLow, nameHigh);
    if (cell)
        return cell->index;
    return -1;
}

// @verified - d
std::int32_t FindNamedTextureByIndex(std::int32_t index, NamedTextureCell **previous, NamedTextureCell **found) {
    NamedTextureCell *cell = g_namedTextureBanks[g_namedTextureBank != 0];
    *previous = TX_NULL;
    *found = TX_NULL;
    while (cell) {
        if (cell->index == index) {
            *found = cell;
            return cell->index;
        }
        *previous = cell;
        cell = cell->next;
    }
    return -1;
}

// @verified - d
std::int32_t AllocateNamedTextureCell(std::uint32_t nameLow, std::uint32_t nameHigh) {
    NamedTextureCell *cell = FindNamedTexture(nameLow, nameHigh);
    std::int32_t index = -1;
    if (cell)
        index = cell->index;
    if (index < 0) {
        cell = g_freeNamedTextureCells;
        if (cell) {
            g_freeNamedTextureCells = cell->next;
            NamedTextureCell **head;
            if (g_namedTextureBank) {
                head = &g_namedTextureBanks[1];
                cell->next = *head;
            } else {
                head = &g_namedTextureBanks[0];
                cell->next = *head;
            }
            *head = cell;
            cell->nameLow = nameLow;
            cell->nameHigh = nameHigh;
            index = cell->index;
            ++cell->references;
        } else
            index = -1;
    } else
        ++cell->references;
    return index;
}

// @verified - d
void ReleaseNamedTextureCell(NamedTextureCell *previous, NamedTextureCell *cell) {
    if (previous)
        previous->next = cell->next;
    else if (g_namedTextureBank)
        *(g_namedTextureBanks + 1) = cell->next;
    else
        g_namedTextureBanks[0] = cell->next;

    cell->next = g_freeNamedTextureCells;
    g_freeNamedTextureCells = cell;
    cell->references = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetNamedTextureSurface(std::int32_t index) {
    return g_textureIndexMap[index + dc::sdk::g_modelTextureIdBase].surface;
}
#endif

std::int32_t LoadNamedTexture(const TextureImage *image, const std::uint32_t *name) {
    std::int32_t index = AllocateNamedTextureCell(name[0], name[1]);
    if (index < 0)
        return -1;
    const std::uint32_t globalIndex = static_cast<std::uint32_t>(index) + dc::sdk::g_modelTextureIdBase;
    if (!g_textureIndexMap[TX_I32(globalIndex)].references)
        g_textureIndexMap[TX_I32(globalIndex)].surface = AllocateTextureCacheSurface();
    ++g_textureIndexMap[TX_I32(globalIndex)].references;
    const std::int32_t slot = g_textureIndexMap[TX_I32(globalIndex)].surface;
    TextureCacheSurface &surface = g_textureCacheSurfaces[slot];
    std::uint32_t result = 0;
    if (!surface.references) {
        result = CreateTextureCacheSurface(image, slot, image->pixels);
        UpdateTextureSurfacePacketState(slot);
        surface.index = slot;
    }
    ++surface.references;
    if (result) {
        ReleaseNamedTexture(index);
        index = -1;
    }
    return index;
}

std::int32_t ReleaseNamedTexture(std::int32_t index) {
    NamedTextureCell *previous;
    NamedTextureCell *cell;
    index = FindNamedTextureByIndex(index, &previous, &cell);
    if (index < 0)
        return -1;

    --cell->references;
    if (static_cast<std::int16_t>(cell->references) > 0)
        return 0;
    const std::int32_t result = ReleaseTextureIndex(index);
    ReleaseNamedTextureCell(previous, cell);
    return result;
}

void DestroyEffectTextureReferences() {
    NamedTextureCell *cell = g_namedTextureBanks[0];
    while (cell) {
        ReleaseTextureIndex(cell->index);
        NamedTextureCell *const next = cell->next;
        cell->next = g_freeNamedTextureCells;
        g_freeNamedTextureCells = cell;
        cell->references = 0;
        cell = next;
    }
    g_namedTextureBanks[0] = TX_NULL;
    ReleasePaletteReference(0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
char *FormatNamedTextureName(const std::uint32_t *name) {
    char *text = reinterpret_cast<char *>(namedTextureNameText);
    namedTextureNameText[0] = name[0];
    namedTextureNameText[1] = name[1];
    namedTextureNameText[2] = 0;
    for (std::int32_t i = 0; i < 8; ++i) {
        if (static_cast<std::uint8_t>(text[i]) > 'z')
            text[i] = '.';
    }
    return text;
}
#endif

// @verified - d
std::int32_t LoadAndFindTEXN(const TextureChunk *chunk) {
    const std::uint32_t *cursor = reinterpret_cast<const std::uint32_t *>(chunk);
    const std::uint32_t tag = *cursor++;
    if (tag != SHEN_NAME(TEXN))
        return -1;
    ++cursor;
    const std::uint32_t *const name = cursor;
    cursor += 2;
    const std::byte *const image = reinterpret_cast<const std::byte *>(cursor);
    NamedTextureRequest request;
    request.image.width = *reinterpret_cast<const std::int16_t *>(image + 0x18);
    request.image.height = *reinterpret_cast<const std::int16_t *>(image + 0x1a);
    request.image.type = *reinterpret_cast<const std::uint32_t *>(image + 0x14);
    request.image.pixels = image + 0x1c;
    request.index = LoadNamedTexture(&request.image, name);
    return request.index;
}

void InitializeTextureCellStorage() {
    g_namedTextureBank = 0;
    InitializeNamedTextureStorage(textureCacheStorage, kTextureCacheCapacity);
    dc::sdk::ResetScreenTextureState();
    InitializePaletteSlots(effectPaletteSlots, 1);
    njSetPaletteMode(2);
    const PaletteData *const palette = &kDefaultEffectPalette;
    if (LoadPaletteReference(palette->index, *palette) == 0)
        SetSelectedPalette(palette->index);
}

// @verified - d
void InitSceneEffects() {
    DestroyEffectTextureReferences();
}

void ReleaseNamedTexture_Wrapper(std::int32_t index) {
    ReleaseNamedTexture(index);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FlushTextureImage(const std::uint32_t *image) {
    const std::uint32_t tag = *image;
    if (tag == SHEN_NAME(GBIX))
        image += 3;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(image + 4) & ~static_cast<std::uintptr_t>(31);
    const std::int16_t *const dimensions = reinterpret_cast<const std::int16_t *>(image + 3);
    const std::int32_t bytes = dimensions[0] * 2 * dimensions[1];
    dc::sdk::PurgeDataCache(reinterpret_cast<void *>(address), bytes / 32 + 1);
}
#endif

// @verified - d
void PurgeGeneratedSurfacePixels(GeneratedTextureSurface *surface) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(surface->pixels) & ~static_cast<std::uintptr_t>(31);
    const std::int32_t bytes = surface->height * 2 * surface->width;
    dc::sdk::PurgeDataCache(reinterpret_cast<void *>(address), bytes / 32 + 1);
}

namespace {

// @verified - d
void TextureMotionNullsub(const TextureChunk *) {
}

} // namespace

std::int16_t ConvertRGB565ToGrey(std::int16_t colour) {
    std::int16_t blue = static_cast<std::int16_t>((colour & 0x1f) * 5);
    std::int16_t green = static_cast<std::int16_t>(((colour & 0x7e0) >> 6) << 3);
    std::int16_t red = static_cast<std::int16_t>(((colour & 0xf800) >> 11) * 3);
    std::int16_t grey = static_cast<std::int16_t>(blue + (green + red)) >> 4;
    green = static_cast<std::int16_t>(static_cast<std::uint32_t>(grey << 8) >> 2);
    red = static_cast<std::int16_t>(grey << 11);
    colour = static_cast<std::int16_t>(red + green + grey);
    blue = colour & 0x1f;
    green = (colour >> 5) & 0x3f;
    red = (colour >> 11) & 0x1f;
    blue = static_cast<std::int16_t>(g_mstsValue2 * 2 + blue);
    if (blue > 31)
        blue = 31;
    if (blue < 0)
        blue = 0;
    blue &= 0x1f;
    green = static_cast<std::int16_t>(g_mstsValue1 * 4 + green);
    if (green > 63)
        green = 63;
    if (green < 0)
        green = 0;
    green = static_cast<std::int16_t>(static_cast<std::uint8_t>(green << 2) << 3);
    red = static_cast<std::int16_t>(g_mstsValue0 * 2 + red);
    if (red > 31)
        red = 31;
    if (red < 0)
        red = 0;
    return static_cast<std::int16_t>(static_cast<std::int16_t>(red << 11) + green + blue);
}

std::int16_t ConvertARGB1555ToGrey(std::int16_t colour) {
    const std::int16_t alpha = static_cast<std::int16_t>(colour & 0x8000);
    std::int16_t blue = static_cast<std::int16_t>((colour & 0x1f) * 5);
    std::int16_t green = static_cast<std::int16_t>(((colour & 0x3e0) >> 5) * 8);
    std::int16_t red = static_cast<std::int16_t>(((colour & 0x7c00) >> 10) * 3);
    std::int16_t grey = static_cast<std::int16_t>(blue + green + red) >> 4;
    green = static_cast<std::int16_t>(grey << 5);
    red = static_cast<std::int16_t>(grey << 10);
    colour = static_cast<std::int16_t>((red + green + grey) | static_cast<std::int16_t>(colour & alpha));
    blue = colour & 0x1f;
    green = (colour >> 5) & 0x1f;
    red = (colour >> 10) & 0x1f;
    blue = static_cast<std::int16_t>(g_mstsValue2 * 2 + blue);
    if (blue > 31)
        blue = 31;
    if (blue < 0)
        blue = 0;
    green = static_cast<std::int16_t>(g_mstsValue1 * 2 + green);
    if (green > 31)
        green = 31;
    if (green < 0)
        green = 0;
    red = static_cast<std::int16_t>(g_mstsValue0 * 2 + red);
    if (red > 31)
        red = 31;
    if (red < 0)
        red = 0;
    return static_cast<std::int16_t>(
        ((red & 0x1f) << 10) + (static_cast<std::uint8_t>((green & 0x1f) << 3) << 2) + (blue & 0x1f) | static_cast<std::int16_t>(colour & alpha));
}

std::int16_t ConvertARGB4444ToGrey(std::int16_t colour) {
    const std::int16_t alpha = static_cast<std::int16_t>(colour & 0xf000);
    std::int16_t blue = static_cast<std::int16_t>((colour & 0xf) * 5);
    std::int16_t green = static_cast<std::int16_t>(((colour & 0xf0) >> 4) * 8);
    std::int16_t red = static_cast<std::int16_t>(((colour & 0xf00) >> 8) * 3);
    std::int16_t grey = static_cast<std::int16_t>(blue + green + red) >> 4;
    green = static_cast<std::int16_t>(grey << 4);
    red = static_cast<std::int16_t>(grey << 8);
    colour = static_cast<std::int16_t>((red + green + grey) | static_cast<std::int16_t>(colour & alpha));
    blue = colour & 0xf;
    green = (colour >> 4) & 0xf;
    red = (colour >> 8) & 0xf;
    blue = static_cast<std::int16_t>(g_mstsValue2 + blue);
    if (blue > 15)
        blue = 15;
    if (blue < 0)
        blue = 0;
    green = static_cast<std::int16_t>(g_mstsValue1 + green);
    if (green > 15)
        green = 15;
    if (green < 0)
        green = 0;
    red = static_cast<std::int16_t>(g_mstsValue0 + red);
    if (red > 15)
        red = 15;
    if (red < 0)
        red = 0;
    return static_cast<std::int16_t>(((red & 0xf) << 8) + ((green << 4) & 0xf0) + (blue & 0xf) | static_cast<std::int16_t>(colour & alpha));
}

// @verified - d
std::int32_t LoadAndFindTEXN_Wrapper(const TextureChunk *chunk) {
    FlushTextureImage(chunk->globalIndexHeader);
    if (textureMotionFlag)
        TextureMotionNullsub(chunk);
    return LoadAndFindTEXN(chunk);
}

// @verified - d
void SetNamedTextureBank(std::int32_t bank) {
    g_namedTextureBank = bank;
}

// @verified - d
void ClearNamedTextureBank() {
    g_namedTextureBank = 0;
}

// @verified - d
std::int32_t ReadTitleSpriteTextures(const void *source, std::uint32_t size, std::int32_t *textures) {
    const TextureChunk *chunk = reinterpret_cast<const TextureChunk *>(source);
    const std::byte *const end = reinterpret_cast<const std::byte *>(source) + size;
    std::int32_t count = 0;
    while (reinterpret_cast<const std::byte *>(chunk) < end && chunk->name) {
        textures[count] = LoadAndFindTEXN_Wrapper(chunk);
        chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
        ++count;
    }
    return count;
}

// @verified - d
std::int32_t LoadTextureChunks(const TextureChunk *chunks, std::int32_t *textures) {
    const TextureChunk *chunk = chunks;
    std::int32_t count = 0;
    while (chunk->name == SHEN_NAME(TEXN)) {
        do {
            textures[count++] = LoadAndFindTEXN_Wrapper(chunk);
        } while (0);
        chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
    }
    return count;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t LoadTextureChunks(const TextureChunk *source, std::uint32_t bytes, std::int32_t *textures, MemoTextureName *names) {
    const TextureChunk *chunk = source;
    const std::byte *const end = reinterpret_cast<const std::byte *>(chunk) + bytes;
    std::int32_t count = 0;
    while (reinterpret_cast<const std::byte *>(chunk) < end && chunk->name) {
        const std::int32_t texture = LoadAndFindTEXN_Wrapper(chunk);
        textures[count] = texture;
        const std::uint32_t *name = &chunk->nameLow;
        names[count].texture = texture;
        names[count].nameLow = *name++;
        names[count].nameHigh = *name;
        chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
        ++count;
    }
    return count;
}
#endif

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::uint32_t bytes, std::int32_t *textures) {
    const std::byte *const end = reinterpret_cast<const std::byte *>(chunk) + bytes;
    std::int32_t count = 0;
    while (reinterpret_cast<const std::byte *>(chunk) < end && chunk->name) {
        textures[count++] = LoadAndFindTEXN_Wrapper(chunk);
        chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
    }
    return count;
}
#endif

// @verified - d
std::int32_t ReadTempTextureEntries(const void *source, TempTextureEntry *entries) {
    const TextureChunk *chunk = reinterpret_cast<const TextureChunk *>(source);
    std::int32_t count = 0;
    while (chunk->name == SHEN_NAME(TEXN)) {
        entries[count].texture = LoadAndFindTEXN_Wrapper(chunk);
        entries[count].width = static_cast<std::uint16_t>(chunk->width);
        entries[count].height = static_cast<std::uint16_t>(chunk->height);
        chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
        ++count;
    }
    return count;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const std::uint32_t *GetTextureChunkData(const TextureChunk *chunk, std::int16_t offset) {
    return reinterpret_cast<const std::uint32_t *>(chunk + 1) + offset;
}

void CopyTextureChunkTile(TextureChunk *destination, const TextureChunk *source, std::int16_t offset) {
    std::uint32_t *image = destination->globalIndexHeader;
    std::uint32_t *const target = image + 7 + offset;
    image[5] = 0x900;
    const std::int32_t dimensions = source->globalIndexHeader[6];
    const std::int16_t height = static_cast<std::int16_t>(dimensions >> 16), width = static_cast<std::int16_t>(dimensions);
    const std::int16_t size = height * width * 2;
    const std::uint32_t *const pixels = source->globalIndexHeader + 7;
    std::int32_t column = 0, row = 0;
    for (std::int32_t index = 0; index < static_cast<std::int16_t>(size / 4); ++index) {
        (target + row)[column] = pixels[index];
        if (++column > 15) {
            row += 128;
            column = 0;
        }
    }
}

// @verified - d
std::int32_t ReplacePtxTexture(std::int32_t index, const TextureChunk *texture) {
    FlushTextureImage(texture->globalIndexHeader);
    texture++;
    ReloadTextureCacheSurface(g_textureIndexMap[index].surface, texture);
    return index;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void QueryFreeTextureMemory(KMUINT32 *total, KMUINT32 *largest) {
    if (kmGetFreeTextureMem(total, largest) != 0) {
        *largest = 0;
        *total = 0;
    }
}
#endif

// @verified - d
std::int32_t AllocateGeneratedTextureSlot(std::uint32_t width, std::uint32_t height, std::uint32_t format) {
    const std::int32_t surfaceIndex = AllocateTextureCacheSurface();
    if (surfaceIndex < 0)
        return -1;
    const std::int32_t index = AllocateNamedTextureCell(static_cast<std::uint32_t>(surfaceIndex), UINT32_MAX);
    if (index < 0)
        return -1;
    const std::uint32_t globalIndex = static_cast<std::uint32_t>(index) + dc::sdk::g_modelTextureIdBase;
    g_textureIndexMap[TX_I32(globalIndex)].surface = surfaceIndex;
    ++g_textureIndexMap[TX_I32(globalIndex)].references;
    TextureCacheSurface &surface = g_textureCacheSurfaces[surfaceIndex];
    surface.creationStatus =
        kmCreateTextureSurface(&surface.surface, static_cast<std::int32_t>(width), static_cast<std::int32_t>(height), static_cast<KMTEXTURETYPE>(format));
    ++g_textureCreationCount;
    UpdateTextureSurfacePacketState(surfaceIndex);
    surface.index = surfaceIndex;
    ++surface.references;

    return index;
}

// @verified - d
GeneratedTextureSurface *AllocateSHAD_SRUF(std::uint32_t width, std::uint32_t height, std::int32_t layout, std::int32_t pixelMode) {
    GeneratedTextureSurface *surface;
    if (static_cast<std::uint32_t>(layout) <= 1 && static_cast<std::uint32_t>(pixelMode) <= 3) {
        const std::int32_t slot = AllocateGeneratedTextureSlot(width, height, kGeneratedSurfaceLayouts[layout] | kGeneratedSurfacePixelModes[pixelMode]);
        if (slot < 0)
            return 0;
        const std::uint32_t bytes = width * height * 2;
        void *const pixels = HLib::AllocateTaskData(bytes, GENERATED_SURFACE_PIXELS);
        std::memset(pixels, 0, bytes);
        surface = static_cast<GeneratedTextureSurface *>(HLib::AllocateTaskData(sizeof(*surface), GENERATED_SURFACE_STORAGE));
        if (surface) {
            surface->width = static_cast<std::int16_t>(width);
            surface->height = static_cast<std::int16_t>(height);
            surface->layout = static_cast<std::uint8_t>(layout);
            surface->pixelMode = static_cast<std::uint8_t>(pixelMode);
            surface->mapCount = 0;
            surface->slots[0] = static_cast<std::int16_t>(slot);
            surface->slots[1] = -1;
            surface->activeSlot = 0;
            surface->pixels = pixels;
        }
    } else
        surface = 0;
    return surface;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetTextureStorageSize(std::uint32_t format, std::int32_t height, std::int32_t width) {
    switch (format & 0xff00U) {
    case 0x300:
        if (height != width)
            return 0;
        switch (height) {
        case 64:
            return 0xc00;
        case 128:
            return 0x1800;
        case 256:
            return 0x4800;
        case 512:
            return 0x10800;
        default:
            return 0;
        }
    case 0x500:
        return static_cast<std::int32_t>(static_cast<std::uint32_t>(height) * static_cast<std::uint32_t>(width)) / 2;
    default:
        return 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool ParseGuiPtx(TextureChunk *chunk, std::int32_t height, std::int32_t width, std::uint32_t idLow, std::uint32_t idHigh, std::uint32_t format) {
    TextureChunk *texture = chunk;
    const std::int32_t bytes = GetTextureStorageSize(format, height, width);
    if (!(reinterpret_cast<std::uintptr_t>(texture) & 3U) && bytes > 0) {
        do {
            texture->name = SHEN_NAME(TEXN);
        } while (0);
        texture->size = static_cast<std::uint32_t>(bytes) + 0x2cU;
        texture->globalIndexHeader[0] = SHEN_NAME(GBIX);
        texture->globalIndexHeader[2] = 0;
        texture->globalIndexHeader[1] = 4;
        char *imageName = reinterpret_cast<char *>(&texture->imageName);
        imageName[0] = 'P';
        imageName[1] = 'V';
        imageName[2] = 'R';
        imageName[3] = 'T';
        texture->height = static_cast<std::int16_t>(height);
        texture->width = static_cast<std::int16_t>(width);
        texture->imageSize = bytes;
        texture->type = format;
        texture->nameLow = idLow;
        texture->nameHigh = idHigh;
        return true;
    }
    return false;
}
#endif

// @verified - d
void EnsureGeneratedSurfaceBackBuffer(GeneratedTextureSurface *surface) {
    if (surface->slots[1] == -1)
        surface->slots[1] = static_cast<std::int16_t>(AllocateGeneratedTextureSlot(
            surface->width, surface->height, kGeneratedSurfaceLayouts[surface->layout] | kGeneratedSurfacePixelModes[surface->pixelMode]));
}

void DestroyGeneratedSurface(GeneratedTextureSurface *surface) {
    if (!surface)
        return;
    if (surface->slots[0] != -1)
        ReleaseNamedTexture_Wrapper(surface->slots[0]);
    if (surface->slots[1] != -1)
        ReleaseNamedTexture_Wrapper(surface->slots[1]);
    HLib::FreeTaskData(surface->pixels);
    HLib::FreeTaskData(surface);
}

// @verified - d
std::int32_t RetireGeneratedSurface(GeneratedTextureSurface *surface) {
    std::int32_t slot;
    if (surface) {
        slot = surface->slots[0];
        if (surface->slots[1] != -1)
            ReleaseNamedTexture_Wrapper(surface->slots[1]);
        HLib::FreeTaskData(surface->pixels);
        HLib::FreeTaskData(surface);
    } else
        slot = -1;
    return slot;
}

// @verified - d
std::int32_t GetGeneratedSurfaceTexture(GeneratedTextureSurface *surface) {
    return surface->slots[surface->activeSlot];
}

// @verified - d
void *GetGeneratedSurfacePixels(GeneratedTextureSurface *surface) {
    ++surface->mapCount;
    return surface->pixels;
}

// @verified - d
void CommitGeneratedSurface(GeneratedTextureSurface *surface) {
    if (surface && --surface->mapCount == 0) {
        PurgeGeneratedSurfacePixels(surface);
        const std::int32_t index = surface->slots[surface->activeSlot + dc::sdk::g_modelTextureIdBase];
        ReloadTextureCacheSurface(g_textureIndexMap[index].surface, surface->pixels);
    }
}

// @verified - d
std::int32_t GetGeneratedSurfaceBackTexture(GeneratedTextureSurface *surface) {
    return surface->slots[surface->activeSlot ^ 1U];
}

// @verified - d
void SwapGeneratedSurfaceBuffer(GeneratedTextureSurface *surface) {
    surface->activeSlot ^= 1U;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetGeneratedSurfaceInfo(GeneratedTextureSurface *surface, std::int16_t *info) {
    info[0] = surface->width;
    info[1] = surface->height;
    info[2] = surface->slots[surface->activeSlot];
    info[3] = surface->slots[surface->activeSlot ^ 1U];
}
#endif

} // namespace s2
} // namespace shenmue
#undef TX_INIT
#undef TX_ZERO
#undef TX_NULL
#undef TX_ALIGN
#undef TX_I32
#undef TX_VALUE

#endif
