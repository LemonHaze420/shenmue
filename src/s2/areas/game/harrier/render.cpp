#include "s2/areas/game/harrier/render.hpp"
#include "s2/legacy_draw.hpp"
#include "s1/modules/harrier.hpp"
#include "d3t/texture.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
constexpr std::uint32_t kTransparent = 0x00818181;
std::array<std::uint8_t, 0x200000> spriteGraphics{};
std::array<std::uint8_t, 0x40000> backgroundGraphics{};
std::array<std::uint32_t, 32768> paletteLookup{};
std::array<std::array<std::uint32_t, 8>, 128> tilePalettes{};
std::array<std::array<std::uint32_t, 16>, 64> spritePalettes{};
std::array<std::uint8_t, 128> tilePaletteDirty{};
std::array<std::uint8_t, 64> spritePaletteDirty{};
std::array<std::array<std::uint16_t, 2048>, 6> tileMaps{};
std::array<std::array<std::byte, 4160>, 384> tilePixels{};
std::array<std::uint16_t, 384> tileOccupancy{};
std::array<std::uint8_t, 384> tileDirty{};
std::array<std::uint32_t, 384> tileTextures{};
std::array<std::uint32_t, 96> spriteTextures{};
std::array<HarrierSpriteCache, 96> spriteCache{};
std::array<std::uint32_t, 0x10000> spritePixels{};
std::uint32_t fillTexture{}, patternTexture{}, spriteCursor{}, averageColour{}, previousAverageColour{};

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
void TintedQuad(float left, float top, float right, float bottom, float v, std::uint32_t colour) {
    const std::array<EmulatorPositionColourTextureVertex, 4> vertices{{
        {.x = left, .y = top, .z = .5f, .w = 1, .colour = colour, .u = 0, .v = v},
        {.x = right, .y = top, .z = .5f, .w = 1, .colour = colour, .u = 1, .v = v},
        {.x = left, .y = bottom, .z = .5f, .w = 1, .colour = colour, .u = 0, .v = v},
        {.x = right, .y = bottom, .z = .5f, .w = 1, .colour = colour, .u = 1, .v = v},
    }};
    DrawEmulatorVertices(vertices.data(), 28, 4, 31, 15);
}
void InitializeTileCache() {
    for (std::array<std::uint16_t, 2048> &map : tileMaps)
        map.fill(0);
    for (std::array<std::uint32_t, 8> &palette : tilePalettes)
        palette.fill(kTransparent);
    for (std::size_t index = 0; index < 384; ++index) {
        tileTextures[index] = 0;
        tileDirty[index] = 0;
        tileOccupancy[index] = 0;
        std::byte *const header = tilePixels[index].data() + 2;
        const std::uint16_t signature = 0x4d42;
        const std::uint32_t size = 4150, offset = 54, infoSize = 40, width = 32, height = 32, planes = 0x200001, zero = 0, resolution = 2834;
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
    for (std::size_t index = 0; index < 96; ++index) {
        HarrierSpriteCache &entry = spriteCache[index];
        entry.textureSlot = static_cast<std::int32_t>(index);
        entry.cachedPattern = -1;
        entry.cachedPalette = 0;
        entry.cachedHeight = -1;
    }
    spriteCursor = 0;
    averageColour = previousAverageColour = kTransparent;
    for (std::array<std::uint32_t, 16> &palette : spritePalettes)
        palette.fill(kTransparent);
}
void UpdatePalettes() {
    for (std::size_t index = 0; index < 128; ++index) {
        tilePalettes[index][0] = kTransparent;
        tilePaletteDirty[index] = 0;
        for (std::size_t entry = 1; entry < 8; ++entry) {
            const std::uint32_t colour = Colour(0xb0000 + index * 16 + entry * 2);
            if (tilePalettes[index][entry] != colour) {
                tilePalettes[index][entry] = colour;
                tilePaletteDirty[index] = 1;
            }
        }
    }
    for (std::size_t index = 0; index < 64; ++index) {
        spritePalettes[index][0] = kTransparent;
        spritePaletteDirty[index] = 0;
        for (std::size_t entry = 1; entry < 16; ++entry) {
            const std::uint32_t colour = Colour(0xb0800 + index * 32 + entry * 2);
            if (spritePalettes[index][entry] != colour) {
                spritePalettes[index][entry] = colour;
                spritePaletteDirty[index] = 1;
            }
        }
    }
    previousAverageColour = averageColour;
    std::uint32_t red = 0, green = 0, blue = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::uint32_t colour = paletteLookup[MemoryWord(0xb0070 + index * 2)];
        red += colour & 0xff0000;
        green += colour & 0xff00;
        blue += colour & 0xff;
    }
    averageColour = 0xff000000 + ((75 * (red >> 2) / 100) & 0xff0000) + ((75 * (green >> 2) / 100) & 0xff00) + (75 * (blue >> 2) / 100);
}
void CopyTileMap(std::int32_t x, std::int32_t y, const std::uint8_t *source, std::uint16_t *destination) {
    std::int32_t rows = 32, columns = 64;
    if (y < -8) {
        const std::int32_t skipped = ((-9 - y) >> 3) + 1;
        source += skipped * 128;
        destination += skipped * 64;
        rows -= skipped;
        y += skipped * 8;
    }
    if (x < -8) {
        const std::int32_t skipped = ((-9 - x) >> 3) + 1;
        source += skipped * 2;
        destination += skipped;
        columns -= skipped;
        x += skipped * 8;
    }
    if (columns > (328 - x) / 8)
        columns = (328 - x) / 8;
    if (rows > (232 - y) / 8)
        rows = (232 - y) / 8;
    for (std::int32_t row = 0; row < rows; ++row) {
        for (std::int32_t column = 0; column < columns; ++column)
            destination[column] = Word(source + column * 2);
        source += 128;
        destination += 64;
    }
}
void DecodeTile(std::int32_t column, std::int32_t row, std::int32_t bank, std::uint16_t pattern, std::uint16_t palette) {
    const std::size_t index = bank + 16 * (row / 4) + column / 4;
    tileDirty[index] = 1;
    const std::uint16_t bit = static_cast<std::uint16_t>(1u << ((column & 3) + 4 * (row & 3)));
    const bool occupied = harrierTileOccupancy[pattern] != 0;
    if (occupied)
        tileOccupancy[index] |= bit;
    else
        tileOccupancy[index] &= static_cast<std::uint16_t>(~bit);
    const std::byte *const paletteBytes = reinterpret_cast<const std::byte *>(tilePalettes.data()) + palette * 4;
    for (std::size_t y = 0; y < 8; ++y) {
        std::byte *const output = tilePixels[index].data() + y * 128 + 32 * ((column & 3) + 32 * (row & 3));
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
            const std::size_t palette = bank ? ((word >> 5) & 127) : ((word >> 8) & 15);
            if (tilePaletteDirty[palette] || tileMaps[bank + 3][index] != word) {
                tileMaps[bank + 3][index] = word;
                DecodeTile(column, row, bank * 128, word & (bank ? 0xfff : 0xff), bank ? ((word >> 2) & 0x3f8) : ((word >> 5) & 0x78));
            }
        }
    for (std::size_t index = bank * 128; index < static_cast<std::size_t>((bank + 1) * 128); ++index)
        if (tileDirty[index]) {
            tileDirty[index] = 0;
            d3t::InitTexture2DResource(tileTextures[index]);
            const d3t::Texture2DDescription description{.width = 32,
                .height = 32,
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
    std::size_t index = bank * 128;
    for (std::int32_t row = 0; row < 8; ++row)
        for (std::int32_t column = 0; column < 16; ++column, ++index) {
            const std::int32_t left = 2 * x + 64 * column, top = 2 * y + 16 + 64 * row;
            if (static_cast<std::uint32_t>(left + 63) <= 702 && static_cast<std::uint32_t>(top + 47) <= 510 && tileOccupancy[index]) {
                BindEmulatorTexture(tileTextures[index]);
                TextureQuad(left, top, left + 64, top + 64, 0, .99900001f, 1, 0);
            }
        }
}
void SwapSpriteCache(HarrierSpriteCache &target, HarrierSpriteCache &donor) {
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
void SelectSpriteCache(std::int32_t index, std::int32_t pattern, std::int16_t palette, std::uint16_t height, std::uint16_t width) {
    HarrierSpriteCache &entry = spriteCache[index];
    HarrierSpriteCache *found = nullptr;
    if ((entry.palette & 0x80) || averageColour == previousAverageColour) {
        const std::int32_t first = index > 32 ? index - 32 : 0;
        for (std::int32_t candidate = index; candidate >= first; --candidate) {
            if (spriteCache[candidate].cachedPattern == pattern && spriteCache[candidate].cachedPalette == palette) {
                found = &spriteCache[candidate];
                break;
            }
        }
        const std::int32_t end = index + 32 < 64 ? index + 32 : 64;
        if (!found)
            for (std::int32_t candidate = index + 1; candidate < end; ++candidate) {
                if (spriteCache[candidate].cachedPattern == pattern && spriteCache[candidate].cachedPalette == palette) {
                    found = &spriteCache[candidate];
                    break;
                }
            }
        if (!found)
            for (std::int32_t candidate = 64; candidate < 96; ++candidate) {
                if (spriteCache[candidate].cachedPattern == pattern && spriteCache[candidate].cachedPalette == palette) {
                    found = &spriteCache[candidate];
                    break;
                }
            }
    }
    if (found) {
        if (!found->claimed) {
            SwapSpriteCache(entry, *found);
            entry.claimed = 1;
            found = &entry;
        }
        entry.cache = found;
    } else {
        entry.width = width;
        entry.cache = nullptr;
        entry.pattern = pattern;
        entry.palette = palette;
        entry.height = height;
    }
}
void DecodeSprite(std::int32_t index, HarrierSpriteCache &entry) {
    const std::int32_t first = index > 32 ? index - 32 : 0;
    if ((entry.palette & 0x80) || averageColour == previousAverageColour) {
        for (std::int32_t candidate = index - 1; candidate >= first; --candidate) {
            HarrierSpriteCache &previous = spriteCache[candidate];
            if (entry.pattern == previous.cachedPattern && entry.palette == previous.cachedPalette) {
                entry.cache = &previous;
                return;
            }
        }
    }
    spriteCursor = spriteCursor + 1 >= 32 ? 0 : spriteCursor + 1;
    SwapSpriteCache(entry, spriteCache[64 + spriteCursor]);
    entry.cache = &entry;
    entry.claimed = 1;
    d3t::InitTexture2DResource(spriteTextures[entry.textureSlot]);
    entry.cachedHeight = static_cast<std::int16_t>(entry.height);
    entry.cachedPattern = entry.pattern;
    entry.cachedPalette = entry.palette;
    std::array<std::uint32_t, 16> palette = spritePalettes[entry.palette & 63];
    if (!(entry.palette & 0x80))
        palette[10] = averageColour;
    const std::byte *const paletteBytes = reinterpret_cast<const std::byte *>(palette.data());
    const std::uint8_t *source = spriteGraphics.data() + 8 * entry.pattern;
    std::uint32_t *output = spritePixels.data();
    for (std::size_t row = 0; row < entry.height; ++row) {
        for (std::int32_t block = 0; block < entry.width; block += 8)
            for (std::int32_t pixel = 0; pixel < 8; ++pixel)
                std::memcpy(output++, paletteBytes + *source++, 4);
    }
    const d3t::Texture2DDescription description{.width = entry.width,
        .height = entry.height,
        .arraySize = 1,
        .mipLevels = 1,
        .format = 3,
        .bindFlags = 0,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = spritePixels.data()};
    spriteTextures[entry.textureSlot] = d3t::CreateTexture2D(&description);
}
void DrawSprites() {
    for (HarrierSpriteCache &entry : spriteCache) {
        if (spritePaletteDirty[entry.cachedPalette & 63])
            entry.cachedPattern = -1;
        entry.claimed = 0;
    }
    std::int32_t count = 0;
    for (; count < 64; ++count) {
        const std::uint8_t *const source = s1::g_harrierMemory + 0xc0000 + count * 16;
        if (source[0] == 0xff)
            break;
        HarrierSpriteCache &entry = spriteCache[count];
        entry.active = 0;
        if (!source[0])
            continue;
        const std::uint16_t position = Word(source + 2);
        const std::int32_t width = (source[5] & 31) ? 8 * (source[5] & 31) : 256;
        std::int32_t pattern = 8 * (position & 0x7000) + (Word(source + 6) & 0x7fff), x = (position & 0x1ff) - 192;
        float xScale = (128 - (source[8] & 127)) / 128.f;
        const float yScale = (128 - (source[9] & 127)) / 128.f;
        if (source[6] & 0x80) {
            ++pattern;
            x += static_cast<std::int32_t>(width * xScale);
            xScale = -xScale;
        } else
            pattern += width >> 3;
        entry.left = 2.f * x;
        entry.right = 2.f * x + 2.f * width * xScale;
        entry.top = 2.f * source[0] + 16;
        entry.bottom = 2.f * source[1] + 16;
        const std::int32_t height = static_cast<std::int32_t>((source[0] - source[1]) / yScale);
        if (height < 1)
            continue;
        entry.active = 1;
        SelectSpriteCache(count, pattern, source[4], static_cast<std::uint16_t>(height), static_cast<std::uint16_t>(width));
    }
    SetEmulatorDefaultRenderFlags();
    for (std::int32_t index = 0; index < count; ++index) {
        HarrierSpriteCache &entry = spriteCache[index];
        if (!entry.active)
            continue;
        if (!entry.cache)
            DecodeSprite(index, entry);
        BindEmulatorTexture(spriteTextures[entry.cache->textureSlot]);
        TextureQuad(entry.left, entry.top, entry.right, entry.bottom, 0, 0, 1, .99902344f);
    }
}
void DrawSolidLines(std::uint16_t selection) {
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    BindEmulatorTexture(fillTexture);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::uint16_t control = MemoryWord(0xe0000 + row * 2);
        if ((control & 0x600) == selection) {
            const std::size_t palette = s1::g_harrierMemory[0xe0401 + 2 * (control & 0xff)] & 63;
            ColourQuad(0, 16.f + 2 * row, 640, 18.f + 2 * row, Colour(0xb0f80 + 2 * palette));
        }
    }
}
void DrawTexturedLines(std::uint16_t selection) {
    SetEmulatorDefaultRenderFlags();
    BindEmulatorTexture(patternTexture);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::uint16_t control = MemoryWord(0xe0000 + row * 2);
        if ((control & 0x600) == selection) {
            const std::uint32_t index = 2 * (control & 0xff);
            const std::uint8_t *const position = s1::g_harrierMemory + 0xe0200 + index;
            const float left = 2.f * (-24 - ((position[1] - (position[0] << 8)) & 0x1ff));
            const bool reversed = (s1::g_harrierMemory[0xe0601 + index] & 2) != 0;
            const float v = index * .0009765625f;
            TintedQuad(left, 16.f + 2 * row, left + 1024, 18.f + 2 * row, v, Colour(0xb0070 + (reversed ? 2 : 0)));
            TintedQuad(left, 16.f + 2 * row, left + 1024, 18.f + 2 * row, v + .5f, Colour(0xb0070 + (reversed ? 4 : 6)));
        }
    }
}
void DrawScrollingLayer(std::int32_t bank, std::uint16_t pages, bool repeatPage) {
    const std::size_t xOffset = bank == 1 ? 0x98ffa : 0x98ff8;
    const std::int32_t x = ((s1::g_harrierMemory[xOffset + 1] - (s1::g_harrierMemory[xOffset] << 8)) & 0x1ff) - 712;
    const std::int32_t y = -s1::g_harrierMemory[bank == 1 ? 0x98f27 : 0x98f25];
    for (std::int32_t quadrant = 0; quadrant < 4; ++quadrant) {
        const std::size_t page = repeatPage ? 0 : (pages >> (quadrant * 4)) & 15;
        CopyTileMap(x + (quadrant & 1) * 512, y + (quadrant >> 1) * 256, s1::g_harrierMemory + 0x90000 + page * 4096, tileMaps[bank].data());
    }
    UpdateTileBank(bank);
    SetEmulatorDefaultRenderFlags();
    for (std::int32_t quadrant = 0; quadrant < 4; ++quadrant)
        DrawTileBank(bank, x + (quadrant & 1) * 512, y + (quadrant >> 1) * 256);
}
void DrawForeground() {
    const std::uint8_t *source = s1::g_harrierMemory + 0x98030;
    std::uint16_t *destination = tileMaps[0].data();
    for (std::size_t row = 0; row < 29; ++row) {
        for (std::size_t column = 0; column < 41; ++column)
            destination[column] = Word(source + column * 2);
        source += 128;
        destination += 64;
    }
    UpdateTileBank(0);
    SetEmulatorDefaultRenderFlags();
    DrawTileBank(0, 0, 0);
}
} // namespace

void InitializeHarrierGraphics() {
    for (std::uint32_t value = 0; value < 32768; ++value) {
        const std::uint32_t red = 255 * (((value >> 12) & 1) + 2 * (value & 15)) / 31;
        const std::uint32_t green = 255 * (((value >> 13) & 1) + ((value >> 3) & 30)) / 31;
        const std::uint32_t blue = 255 * (((value >> 14) & 1) + ((value >> 7) & 30)) / 31;
        paletteLookup[value] = 0xff000000 | (red << 16) | (green << 8) | blue;
    }
    LoadEmulatorFile("XHAR/datas/SHXOBJ.BIN", spriteGraphics.data(), 0);
    LoadEmulatorFile("XHAR/datas/SHXBG.BIN", backgroundGraphics.data(), 0);
    InitializeSpriteCache();
    InitializeTileCache();
    fillTexture = LoadEmulatorImage("XHAR/datas/FILL.BMP");
    patternTexture = LoadEmulatorImage("XHAR/datas/SHICPAT.BMP");
}
void ReleaseHarrierGraphics() {
    for (std::uint32_t texture : spriteTextures)
        d3t::InitTexture2DResource(texture);
    for (std::uint32_t texture : tileTextures)
        d3t::InitTexture2DResource(texture);
    d3t::InitTexture2DResource(patternTexture);
    d3t::InitTexture2DResource(fillTexture);
}
void RenderHarrierFrame() {
    UpdatePalettes();
    DrawSolidLines(0x200);
    DrawTexturedLines(0);
    DrawScrollingLayer(1, MemoryWord(0x98e9c), false);
    const std::uint16_t secondPages = MemoryWord(0x98e9e);
    if (!secondPages)
        DrawScrollingLayer(2, 0, true);
    DrawSolidLines(0x600);
    DrawTexturedLines(0x400);
    DrawSprites();
    if (secondPages)
        DrawScrollingLayer(2, secondPages, false);
    DrawForeground();
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    ColourQuad(0, 0, 640, 16, 0xff000000);
    ColourQuad(0, 464, 640, 480, 0xff000000);
}
} // namespace shenmue::s2

namespace shenmue::s1 {
void HarrierLoadAndInitResources() {
    s2::InitializeHarrierGraphics();
}
} // namespace shenmue::s1
