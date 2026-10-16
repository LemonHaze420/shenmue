#include "s2/areas/game_afterburner_render.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/legacy_draw.hpp"
#include "s1/modules/harrier.hpp"
#include "d3t/texture.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
constexpr std::uint32_t kTransparent = 0x00818181;
std::array<std::uint8_t, 0x400000> spriteGraphics{};
std::array<std::uint8_t, 0x80000> backgroundGraphics{};
std::array<std::uint8_t, 0x8a08> carrierGraphics{};
std::array<std::uint8_t, 0x1000> roadSnapshot{};
std::array<std::uint32_t, 32768> paletteLookup{};
std::array<std::array<std::uint32_t, 8>, 128> tilePalettes{};
std::array<std::array<std::uint32_t, 16>, 256> spritePalettes{};
std::array<std::uint8_t, 128> tilePaletteDirty{};
std::array<std::uint8_t, 256> spritePaletteDirty{};
std::array<std::array<std::uint16_t, 2048>, 6> tileMaps{};
std::array<std::array<std::byte, 1088>, 1536> tilePixels{};
std::array<std::uint16_t, 1536> tileOccupancy{};
std::array<std::uint8_t, 1536> tileDirty{};
std::array<std::uint32_t, 1536> tileTextures{};
std::array<std::uint32_t, 224> spriteTextures{};
std::array<AfterBurnerSpriteCache, 224> spriteCache{};
std::array<std::uint32_t, 0x10000> spritePixels{};
std::array<std::array<std::int32_t, 32>, 3> rowOffsets{};
std::array<std::uint32_t, 7> replacementTextures{};
std::uint32_t fillTexture{}, patternTexture{}, blackTexture{}, spriteCursor{};

std::uint16_t Word(const std::uint8_t *bytes) {
    return static_cast<std::uint16_t>((bytes[0] << 8) + bytes[1]);
}
std::uint16_t MemoryWord(std::size_t offset) {
    return Word(s1::g_harrierMemory + offset);
}
std::uint32_t Colour(std::size_t offset) {
    return paletteLookup[MemoryWord(offset) & 0x7fff];
}
void TextureQuad(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1) {
    const std::array<EmulatorPositionTextureVertex, 4> vertices{{
        {.x = left, .y = top, .z = .5f, .w = 1, .u = u0, .v = v0},
        {.x = right, .y = top, .z = .5f, .w = 1, .u = u1, .v = v0},
        {.x = left, .y = bottom, .z = .5f, .w = 1, .u = u0, .v = v1},
        {.x = right, .y = bottom, .z = .5f, .w = 1, .u = u1, .v = v1},
    }};
    DrawEmulatorVertices(vertices.data(), 24, 4, 32, 16);
}
void ColourQuad(float left, float top, float right, float bottom, std::uint32_t colour) {
    const std::array<EmulatorPositionColourVertex, 4> vertices{{
        {.x = left, .y = top, .z = .5f, .w = 1, .colour = colour},
        {.x = right, .y = top, .z = .5f, .w = 1, .colour = colour},
        {.x = left, .y = bottom, .z = .5f, .w = 1, .colour = colour},
        {.x = right, .y = bottom, .z = .5f, .w = 1, .colour = colour},
    }};
    DrawEmulatorVertices(vertices.data(), 20, 4, 30, 14);
}
void TintedQuad(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1, std::uint32_t colour) {
    const std::array<EmulatorPositionColourTextureVertex, 4> vertices{{
        {.x = left, .y = top, .z = .5f, .w = 1, .colour = colour, .u = u0, .v = v0},
        {.x = right, .y = top, .z = .5f, .w = 1, .colour = colour, .u = u1, .v = v0},
        {.x = left, .y = bottom, .z = .5f, .w = 1, .colour = colour, .u = u0, .v = v1},
        {.x = right, .y = bottom, .z = .5f, .w = 1, .colour = colour, .u = u1, .v = v1},
    }};
    DrawEmulatorVertices(vertices.data(), 28, 4, 31, 15);
}
void InitializeTileCache() {
    for (std::array<std::uint16_t, 2048> &map : tileMaps)
        map.fill(0);
    for (std::array<std::uint32_t, 8> &palette : tilePalettes)
        palette.fill(kTransparent);
    for (std::size_t index = 0; index < 1536; ++index) {
        tileTextures[index] = 0;
        tileDirty[index] = 0;
        tileOccupancy[index] = 0;
        std::byte *const header = tilePixels[index].data() + 2;
        const std::uint16_t signature = 0x4d42;
        const std::uint32_t size = 1078, offset = 54, infoSize = 40, width = 32, height = 8, planes = 0x200001, zero = 0, resolution = 2834;
        std::memcpy(header, &signature, 2);
        std::memcpy(header + 2, &size, 4);
        std::memcpy(header + 10, &offset, 4);
        std::memcpy(header + 14, &infoSize, 4);
        std::memcpy(header + 18, &width, 4);
        std::memcpy(header + 22, &height, 4);
        std::memcpy(header + 26, &planes, 4);
        std::memcpy(header + 34, &zero, 4);
        std::memcpy(header + 38, &resolution, 4);
        std::memcpy(header + 42, &resolution, 4);
        std::memcpy(header + 50, &zero, 4);
    }
}
void InitializeSpriteCache() {
    spriteTextures.fill(0);
    for (std::size_t index = 0; index < 224; ++index) {
        AfterBurnerSpriteCache &entry = spriteCache[index];
        entry.textureSlot = static_cast<std::int32_t>(index);
        entry.cachedPattern = -1;
        entry.cachedPalette = 0;
        entry.cachedHeight = -1;
    }
    spriteCursor = 0;
    for (std::array<std::uint32_t, 16> &palette : spritePalettes)
        palette.fill(kTransparent);
}
void UpdatePalettes() {
    for (std::size_t index = 0; index < 128; ++index) {
        tilePalettes[index][0] = kTransparent;
        tilePaletteDirty[index] = 0;
        for (std::size_t entry = 1; entry < 8; ++entry) {
            const std::uint32_t colour = Colour(0xb3800 + index * 16 + entry * 2);
            if (tilePalettes[index][entry] != colour) {
                tilePalettes[index][entry] = colour;
                tilePaletteDirty[index] = 1;
            }
        }
    }
    for (std::size_t index = 0; index < 256; ++index) {
        spritePalettes[index][0] = kTransparent;
        spritePalettes[index][15] = kTransparent;
        spritePaletteDirty[index] = 0;
        for (std::size_t entry = 1; entry < 15; ++entry) {
            const std::uint32_t colour = Colour(0xb0000 + index * 32 + entry * 2);
            if (spritePalettes[index][entry] != colour) {
                spritePalettes[index][entry] = colour;
                spritePaletteDirty[index] = 1;
            }
        }
    }
}
void CopyTileMap(const std::int32_t *offsets, std::int32_t x, std::int32_t y, const std::uint8_t *source, std::uint16_t *destination) {
    const std::int32_t end = y + 256 > 224 ? 224 : y + 256;
    if (y < -8) {
        const std::int32_t skipped = ((-9 - y) >> 3) + 1;
        source += skipped * 128;
        destination += skipped * 64;
        y += skipped * 8;
    }
    while (y < end) {
        std::int32_t left = x + *offsets++;
        const std::int32_t right = left + 512 > 320 ? 320 : left + 512;
        const std::uint8_t *row = source;
        std::uint16_t *output = destination;
        if ((left + 7) / 8 < 0) {
            const std::int32_t skip = (left + 7) / -8;
            left += skip * 8;
            row += skip * 2;
            output += skip;
        }
        while (left < right) {
            *output++ = Word(row);
            row += 2;
            left += 8;
        }
        source += 128;
        destination += 64;
        y += 8;
    }
}
void DecodeTile(std::int32_t column, std::int32_t row, std::int32_t bank, std::uint16_t pattern, std::uint16_t palette) {
    const std::size_t index = bank + 16 * row + column / 4;
    tileDirty[index] = 1;
    const std::uint16_t bit = static_cast<std::uint16_t>(1u << (column & 3));
    const bool occupied = afterBurnerTileOccupancy[pattern] != 0;
    if (occupied)
        tileOccupancy[index] |= bit;
    else
        tileOccupancy[index] &= static_cast<std::uint16_t>(~bit);
    const std::byte *const paletteBytes = reinterpret_cast<const std::byte *>(tilePalettes.data()) + palette * 4;
    for (std::size_t y = 0; y < 8; ++y) {
        std::byte *output = tilePixels[index].data() + y * 128 + (column & 3) * 32;
        for (std::size_t x = 0; x < 8; ++x) {
            std::uint32_t colour = kTransparent;
            if (occupied)
                std::memcpy(&colour, paletteBytes + backgroundGraphics[pattern * 64 + y * 8 + x], 4);
            std::memcpy(output + x * 4, &colour, 4);
        }
    }
}
void UpdateTileBank(std::int32_t bank) {
    const std::int32_t rows = bank ? 32 : 28, columns = bank ? 64 : 40;
    for (std::int32_t row = 0; row < rows; ++row)
        for (std::int32_t column = 0; column < columns; ++column) {
            const std::size_t index = row * 64 + column;
            const std::uint16_t word = tileMaps[bank][index];
            const std::size_t palette = bank ? ((word >> 6) & 127) : ((word >> 9) & 31);
            if (tilePaletteDirty[palette] || tileMaps[bank + 3][index] != word) {
                tileMaps[bank + 3][index] = word;
                DecodeTile(column, row, bank * 512, word & (bank ? 0x1fff : 0x1ff), bank ? ((word >> 3) & 0x3f8) : ((word >> 6) & 0xf8));
            }
        }
    for (std::size_t index = bank * 512; index < static_cast<std::size_t>((bank + 1) * 512); ++index)
        if (tileDirty[index]) {
            tileDirty[index] = 0;
            d3t::InitTexture2DResource(tileTextures[index]);
            const d3t::Texture2DDescription description{.width = 32,
                .height = 8,
                .arraySize = 1,
                .mipLevels = 1,
                .format = 3,
                .bindFlags = 0,
                .flags = 0,
                .sampleCount = 1,
                .sampleQuality = 0,
                .reserved_24 = 0,
                .initialData = tilePixels[index].data()};
            tileTextures[index] = d3t::CreateTexture2D(&description);
        }
}
void DrawTileBank(std::int32_t bank, std::int32_t x, std::int32_t y) {
    std::int32_t top = y * 2 + 16;
    const std::int32_t *offsets = rowOffsets[0].data() + bank * 32 + (y + 7) / 8;
    for (std::int32_t row = 0; row < 32 && top < 464; ++row, top += 16, ++offsets)
        if (top > 0) {
            std::int32_t left = 2 * x + 2 * (*offsets);
            for (std::int32_t column = 0; column < 16; ++column, left += 64) {
                const std::size_t index = bank * 512 + row * 16 + column;
                if (left > -64 && left < 640 && tileOccupancy[index]) {
                    BindEmulatorTexture(tileTextures[index]);
                    TextureQuad(left, top, left + 64, top + 16, 0, .99900001f, 1, 0);
                }
            }
        }
}
void UpdateRowOffsets() {
    for (std::size_t bank = 0; bank < 3; ++bank) {
        std::int32_t base = 0;
        if (bank) {
            base = MemoryWord(0xa0e96 + bank * 2) & 0x3ff;
            if (base > 704)
                base &= 0x1ff;
            base -= 704;
        }
        rowOffsets[bank].fill(base);
        if (bank)
            for (std::size_t row = 0; row < 28; ++row) {
                const std::uint16_t value = MemoryWord(0xa0f40 + bank * 64 + row * 2);
                if (!(value & 0x8000)) {
                    std::int32_t delta = value & 0x3ff;
                    if (delta >= 512)
                        delta -= 1024;
                    rowOffsets[bank][row] += delta;
                }
            }
    }
}
void SwapSpriteCache(AfterBurnerSpriteCache &target, AfterBurnerSpriteCache &donor) {
    if (&target == &donor)
        return;
    const std::int32_t slot = target.textureSlot;
    target.textureSlot = donor.textureSlot;
    donor.textureSlot = slot;
    const std::int32_t pattern = donor.cachedPattern;
    donor.cachedPattern = target.cachedPattern;
    target.cachedPattern = pattern;
    donor.cachedPattern = target.cachedPalette;
    target.cachedPalette = donor.cachedPalette;
    donor.cachedPattern = target.cachedHeight;
    target.cachedHeight = donor.cachedHeight;
}
void SelectSpriteCache(std::int32_t index, std::int32_t pattern, std::int32_t palette, std::int32_t height, std::uint16_t width) {
    AfterBurnerSpriteCache &target = spriteCache[index];
    const std::int32_t begin = index > 32 ? index - 32 : 0, end = index + 32 < 192 ? index + 32 : 192;
    AfterBurnerSpriteCache *match = nullptr;
    for (std::int32_t entry = index; entry >= begin; --entry)
        if (spriteCache[entry].cachedPattern == pattern && spriteCache[entry].cachedHeight == height && spriteCache[entry].cachedPalette == palette) {
            match = &spriteCache[entry];
            break;
        }
    if (!match)
        for (std::int32_t entry = index + 1; entry < end; ++entry)
            if (spriteCache[entry].cachedPattern == pattern && spriteCache[entry].cachedHeight == height && spriteCache[entry].cachedPalette == palette) {
                match = &spriteCache[entry];
                break;
            }
    if (!match)
        for (std::int32_t entry = 192; entry < 224; ++entry)
            if (spriteCache[entry].cachedPattern == pattern && spriteCache[entry].cachedHeight == height && spriteCache[entry].cachedPalette == palette) {
                match = &spriteCache[entry];
                break;
            }
    if (!match) {
        target.width = width;
        target.cache = nullptr;
        target.pattern = pattern;
        target.palette = static_cast<std::int16_t>(palette);
        target.height = static_cast<std::uint16_t>(height);
        return;
    }
    if (!match->claimed) {
        SwapSpriteCache(target, *match);
        match = &target;
        target.claimed = 1;
    }
    target.cache = match;
}
void CreateSpriteTexture(std::int32_t index, AfterBurnerSpriteCache &target) {
    const std::int32_t begin = index > 32 ? index - 32 : 0;
    for (std::int32_t entry = index - 1; entry >= begin; --entry)
        if (target.pattern == spriteCache[entry].cachedPattern && target.height == spriteCache[entry].cachedHeight &&
            target.palette == spriteCache[entry].cachedPalette) {
            target.cache = &spriteCache[entry];
            return;
        }
    if (++spriteCursor >= 32)
        spriteCursor = 0;
    SwapSpriteCache(target, spriteCache[192 + spriteCursor]);
    target.cache = &target;
    target.claimed = 1;
    d3t::InitTexture2DResource(spriteTextures[target.textureSlot]);
    target.cachedPattern = target.pattern;
    target.cachedPalette = target.palette;
    target.cachedHeight = static_cast<std::int16_t>(target.height);
    const std::uint8_t *source = spriteGraphics.data() + 8 * (target.pattern & 0x7fffff);
    std::uint32_t *output = spritePixels.data();
    const std::byte *const palette = reinterpret_cast<const std::byte *>(spritePalettes[target.palette].data());
    for (std::uint32_t row = 0; row < target.height; ++row) {
        std::uint32_t column = 0;
        while (column < target.width) {
            for (std::uint32_t pixel = 0; pixel < 8; ++pixel)
                std::memcpy(output++, palette + (target.flip ? source[-1 - static_cast<int>(pixel)] : source[pixel]), 4);
            source += target.flip ? -8 : 8;
            column += 8;
            if ((target.flip ? source[1] : source[-2]) == 60) {
                while (column < target.width) {
                    for (std::uint32_t pixel = 0; pixel < 8; ++pixel)
                        *output++ = kTransparent;
                    source += target.flip ? -8 : 8;
                    column += 8;
                }
                break;
            }
        }
        if (target.flip)
            source += 2 * target.width;
    }
    if (target.width && target.height) {
        const d3t::Texture2DDescription description{.width = target.width,
            .height = target.height,
            .arraySize = 1,
            .mipLevels = 1,
            .format = 3,
            .bindFlags = 0,
            .flags = 0,
            .sampleCount = 1,
            .sampleQuality = 0,
            .reserved_24 = 0,
            .initialData = spritePixels.data()};
        spriteTextures[target.textureSlot] = d3t::CreateTexture2D(&description);
    } else
        spriteTextures[target.textureSlot] = 0;
}
void DrawSprites() {
    for (AfterBurnerSpriteCache &entry : spriteCache) {
        if (spritePaletteDirty[entry.cachedPalette])
            entry.cachedPattern = -1;
        entry.claimed = 0;
    }
    std::int32_t count = 192;
    for (std::int32_t index = 0; index < 192; ++index) {
        const std::uint8_t *source = s1::g_harrierMemory + 0xc0000 + 16 * index;
        std::uint32_t sentinel;
        std::memcpy(&sentinel, source, 4);
        if ((sentinel & 0xffff00feU) == 0xffff00feU) {
            count = index;
            break;
        }
        AfterBurnerSpriteCache &target = spriteCache[index];
        const float zoom = static_cast<float>(Word(source + 6) & 0x3ff);
        const std::int32_t screenHeight = static_cast<std::int32_t>(source[11]) + 1;
        if (!(source[0] & 0x40) && source[11] != 0xff && zoom > 100) {
            target.active = 1;
            float scale = 512.f / zoom;
            std::int32_t pattern = source[3] + ((source[2] + ((source[0] & 14) << 7)) << 8);
            std::int32_t x = ((source[5] - (source[4] << 8)) & 0x1ff) - 192;
            if (x >= 320)
                x -= 512;
            std::int32_t width = 4 * (source[4] & 0x3e);
            if (!width)
                width = 256;
            std::int32_t height = static_cast<std::int32_t>(screenHeight / (512.f / (Word(source + 8) & 0x3ff)));
            if (height < 1)
                height = 1;
            const std::uint8_t flags = source[8] & 0xf0;
            switch (flags & 0x90) {
            case 0:
                target.top = 2 * source[1] + 16;
                target.bottom = 2 * (source[1] - screenHeight) + 16;
                break;
            case 0x10:
                width = 256 - width;
                pattern -= height * (width >> 3);
                target.bottom = 2 * source[1] + 16;
                target.top = 2 * (source[1] - screenHeight) + 16;
                break;
            case 0x80:
                target.top = 2 * source[1] + 16;
                target.bottom = 2 * (source[1] + screenHeight) + 16;
                break;
            case 0x90:
                width = 256 - width;
                pattern -= height * (width >> 3);
                target.bottom = 2 * source[1] + 16;
                target.top = 2 * (source[1] + screenHeight) + 16;
                break;
            }
            switch (flags & 0x60) {
            case 0:
                ++pattern;
                target.flip = 1;
                scale = -scale;
                break;
            case 0x20:
                ++pattern;
                target.flip = 1;
                break;
            case 0x40:
                target.flip = 0;
                scale = -scale;
                break;
            case 0x60:
                target.flip = 0;
                break;
            }
            target.left = 2 * x;
            target.right = 2 * x + 2 * (width * scale);
            SelectSpriteCache(index, target.flip ? pattern | 0x40000000 : pattern, source[13], height, static_cast<std::uint16_t>(width));
        } else
            target.active = 0;
    }
    SetEmulatorDefaultRenderFlags();
    std::int32_t entry = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        AfterBurnerSpriteCache &target = spriteCache[entry];
        if (target.active) {
            if (!target.cache)
                CreateSpriteTexture(index, target);
            const std::uint32_t texture = spriteTextures[target.cache->textureSlot];
            if (!texture)
                continue;
            BindEmulatorTexture(texture);
            TextureQuad(target.left, target.top, target.right, target.bottom, 0, .99902344f, 1, .0009765625f);
        }
        ++entry;
    }
}
void DrawSolidRoad(std::int32_t mode) {
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    BindEmulatorTexture(fillTexture);
    if (!mode) {
        ColourQuad(0, 16, 640, 464, Colour(0xb2e40));
        return;
    }
    const std::uint8_t *source = roadSnapshot.data() + (mode == 2 ? 512 : 0);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::uint16_t control = Word(source + row * 2);
        if (control & 0x800) {
            const std::uint32_t colour = mode == 1 ? 0 : Colour((control & 63) ? 0xb2f00 + 2 * (control & 63) : 0xb2e40);
            ColourQuad(0, static_cast<float>(row * 2 + 16), 640, static_cast<float>(row * 2 + 18), colour);
        }
    }
}
void DrawRoadStrips(std::size_t control, std::size_t position) {
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    const std::uint32_t colour = Colour(0xb2e00);
    for (std::size_t row = 0; row < 224; ++row)
        if (!(MemoryWord(control + row * 2) & 0x800)) {
            const float left = static_cast<float>(1360 - static_cast<std::int32_t>(MemoryWord(position + row * 2) & 0xfff)) * 2;
            ColourQuad(left, static_cast<float>(row * 2 + 16), left + 1024, static_cast<float>(row * 2 + 18), colour);
        }
}
void DrawTexturedRoad() {
    SetEmulatorDefaultRenderFlags();
    BindEmulatorTexture(patternTexture);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::uint16_t control = Word(roadSnapshot.data() + 512 + row * 2);
        if (control & 0x800)
            continue;
        const std::size_t index = 2 * (control & 0x1ff);
        const float left = 2.f * (1360 - static_cast<std::int32_t>(Word(roadSnapshot.data() + 2048 + index) & 0xfff));
        const float top = static_cast<float>(row * 2 + 16), bottom = top + 2;
        const std::uint16_t variant = Word(roadSnapshot.data() + 3072 + index);
        const std::uint32_t background = Colour(0xb0000 + ((variant & 0x100) ? 3682 : 3680));
        TintedQuad(0, top, 640, bottom, 0, .75f, 1, .75f, background);
        const std::size_t entries[3]{variant ? 3610U : 3600U, variant ? 3614U : 3606U, variant ? 3608U : 3602U};
        for (std::size_t pass = 0; pass < 3; ++pass) {
            const float v = static_cast<float>(index) * .00024414062f + static_cast<float>(pass) * .25f;
            TintedQuad(left, top, left + 1024, bottom, 0, v, 1, v, Colour(0xb0000 + entries[pass]));
        }
    }
}
void DrawReplacement(std::size_t index, std::int32_t width, std::int32_t height) {
    BindEmulatorTexture(replacementTextures[index]);
    const float left = (640 - width) * .5f, top = index == 4 ? 67.f : 16 + (448 - height) * .5f;
    TextureQuad(left, top, 640 - left, index == 4 ? top + height : 464 - (448 - height) * .5f, 0, .99902344f, 1, .0009765625f);
    if (index != 4) {
        BindEmulatorTexture(blackTexture);
        TextureQuad(0, 16, 640, 72, 0, .99902344f, 1, .0009765625f);
        TextureQuad(0, 408, 640, 464, 0, .99902344f, 1, .0009765625f);
    }
}
std::uint32_t HashTilePage(const std::uint8_t *source) {
    std::uint32_t hash = 24301;
    for (std::size_t index = 0; index < 1024; ++index, source += 4)
        hash = source[3] + 65599U * source[2] + 0x2e86d0bfU * source[0] + 8261505U * source[1] + 0x43ec5f01U * hash;
    return hash;
}
void DrawScrollingLayer(std::int32_t bank) {
    const std::uint16_t pages = MemoryWord(0xa0e7e + bank * 2);
    const std::size_t yOffset = 0xa0e8e + bank * 2;
    std::int32_t y = -((s1::g_harrierMemory[yOffset + 1] - (s1::g_harrierMemory[yOffset] << 8)) & 0x1ff);
    if (y < -480)
        y += 512;
    const std::uint8_t *source = s1::g_harrierMemory + 0x90000;
    CopyTileMap(rowOffsets[bank].data(), 0, y + 256, source + 4096 * (pages >> 12), tileMaps[bank].data());
    CopyTileMap(rowOffsets[bank].data(), 512, y + 256, source + 4096 * ((pages >> 8) & 15), tileMaps[bank].data());
    const std::uint8_t *hashedPage = source + 4096 * ((pages >> 4) & 15);
    CopyTileMap(rowOffsets[bank].data(), 0, y, hashedPage, tileMaps[bank].data());
    const std::uint32_t hash = HashTilePage(hashedPage);
    CopyTileMap(rowOffsets[bank].data(), 512, y, source + 4096 * (pages & 15), tileMaps[bank].data());
    UpdateTileBank(bank);
    SetEmulatorDefaultRenderFlags();
    DrawTileBank(bank, 0, y);
    bool replaced = true;
    if (bank == 2) {
        if (hash == 0x3758bcfc)
            DrawReplacement(5, 528, 330);
        else if (hash == 0xb2381075)
            DrawReplacement(6, 528, 330);
        else
            replaced = false;
    } else
        switch (hash) {
        case 0x78b33449:
            DrawReplacement(0, 276, 144);
            break;
        case 0x8347e85a:
            DrawReplacement(1, 640, 274);
            break;
        case 0x8627e91a:
            DrawReplacement(2, 528, 330);
            break;
        case 0xd5da6dce:
            DrawReplacement(3, 528, 330);
            break;
        case 0x6d170457:
            DrawReplacement(4, 528, 231);
            break;
        default:
            replaced = false;
            break;
        }
    if (!replaced)
        DrawTileBank(bank, 512, y);
    DrawTileBank(bank, 0, y + 256);
    DrawTileBank(bank, 512, y + 256);
}
} // namespace

void InitializeAfterBurnerGraphics() {
    for (std::uint32_t value = 0; value < 32768; ++value) {
        const std::uint32_t red = 255 * (((value >> 12) & 1) + 2 * (value & 15)) / 31;
        const std::uint32_t green = 255 * (((value >> 13) & 1) + ((value >> 3) & 30)) / 31;
        const std::uint32_t blue = 255 * (((value >> 14) & 1) + ((value >> 7) & 30)) / 31;
        paletteLookup[value] = 0xff000000 | (red << 16) | (green << 8) | blue;
    }
    LoadEmulatorFile("xab/datas/ABXOBJ.BIN", spriteGraphics.data(), 0);
    LoadEmulatorFile("xab/datas/ABXBG.BIN", backgroundGraphics.data(), 0);
    InitializeSpriteCache();
    InitializeTileCache();
    fillTexture = LoadEmulatorImage("xab/datas/FILL.BMP");
    patternTexture = LoadEmulatorImage("xab/datas/ABICPAT.BMP");
    LoadEmulatorFile("xab/replacementdata/aircraftcarrier.data", carrierGraphics.data(), 28320);
    LoadEmulatorFile("xab/replacementdata/aircraftcarrier.data", carrierGraphics.data() + 176, 28320);
    LoadEmulatorFile("xab/replacementdata/aircraftcarriersmall.data", carrierGraphics.data() + 0x6e60, 7080);
    constexpr const char *paths[]{"xab/replacementdata/0.png", "xab/replacementdata/1.png", "xab/replacementdata/2.png", "xab/replacementdata/3.png",
        "xab/replacementdata/4.png", "xab/replacementdata/5.png", "xab/replacementdata/6.png"};
    for (std::size_t index = 0; index < 7; ++index)
        replacementTextures[index] = LoadEmulatorImage(paths[index]);
    blackTexture = LoadEmulatorImage("xab/replacementdata/black.png");
}
void ReleaseAfterBurnerGraphics() {
    for (std::uint32_t texture : spriteTextures)
        d3t::InitTexture2DResource(texture);
    for (std::uint32_t texture : tileTextures)
        d3t::InitTexture2DResource(texture);
    d3t::InitTexture2DResource(patternTexture);
    d3t::InitTexture2DResource(fillTexture);
    for (std::uint32_t texture : replacementTextures)
        d3t::InitTexture2DResource(texture);
}
void SnapshotAfterBurnerRoad() {
    std::memcpy(roadSnapshot.data(), s1::g_harrierMemory + 0xe0000, roadSnapshot.size());
}
void RenderAfterBurnerFrame(const GameArcadeRuntime &runtime) {
    if (!(s1::g_harrierMemory[0xd0005] & 0x20) || runtime.displaySuppressed) {
        SetEngineFlag(147, 0);
        SetEngineFlag(59, 0);
        ColourQuad(0, 0, 640, 480, 0xff000000);
        return;
    }
    UpdatePalettes();
    DrawSolidRoad(0);
    if ((g_arcadeRoadControl & 0xf0) == 0x40)
        DrawRoadStrips(0xe0000, 0xe0400);
    if ((g_arcadeRoadControl & 15) == 3) {
        DrawSolidRoad(2);
        DrawTexturedRoad();
    } else if ((g_arcadeRoadControl & 15) == 4) {
        DrawSolidRoad(1);
        DrawRoadStrips(0xe0000, 0xe0400);
    }
    DrawSprites();
    UpdateRowOffsets();
    DrawScrollingLayer(2);
    DrawScrollingLayer(1);
    CopyTileMap(rowOffsets[0].data(), 0, 0, s1::g_harrierMemory + 0xa0030, tileMaps[0].data());
    UpdateTileBank(0);
    SetEmulatorDefaultRenderFlags();
    DrawTileBank(0, 0, 0);
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    ColourQuad(0, 0, 640, 16, 0xff000000);
    ColourQuad(0, 464, 640, 480, 0xff000000);
}
} // namespace shenmue::s2
