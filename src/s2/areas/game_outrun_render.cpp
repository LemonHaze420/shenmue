#include "s2/areas/game_outrun_render.hpp"

#include "s2/areas/game_outrun_board.hpp"
#include "s2/legacy_draw.hpp"
#include "s1/modules/harrier.hpp"
#include "d3t/texture.hpp"

#include <array>
#include <cstring>

namespace shenmue::s2 {
namespace {

constexpr std::uint32_t kTransparent = 0x00818181;
std::array<std::uint8_t, 0x200000> spriteGraphics{};
std::array<std::uint8_t, 0x80000> backgroundGraphics{};
std::array<std::uint8_t, 0x800> coinGraphics{};
std::array<std::uint32_t, 32768> paletteLookup{};
std::array<std::array<std::uint32_t, 8>, 128> tilePalettes{};
std::array<std::array<std::uint32_t, 16>, 128> spritePalettes{};
std::array<std::uint8_t, 128> tilePaletteDirty{}, spritePaletteDirty{};
std::array<std::array<std::uint16_t, 2048>, 6> tileMaps{};
std::array<std::array<std::byte, 4160>, 384> tilePixels{};
std::array<std::uint8_t, 384> tileDirty{};
std::array<std::uint16_t, 384> tileOccupancy{};
std::array<std::uint32_t, 384> tileTextures{};
std::array<std::uint32_t, 160> spriteTextures{};
std::array<OutRunSpriteCache, 160> spriteCache{};
std::array<std::uint32_t, 0x10000> spritePixels{};
std::array<EmulatorPositionColourTextureVertex, 896> roadVertices{};
std::array<std::uint8_t, 448> roadActive{};
std::array<float, 448> roadPosition{}, roadU{};
std::array<std::uint32_t, 448> roadColour{};
std::uint32_t fillTexture{}, patternTexture{}, spriteCursor{}, averageColour{}, previousAverageColour{};

std::uint16_t MemoryWord(std::size_t offset) {
    return static_cast<std::uint16_t>((s1::g_harrierMemory[offset] << 8) + s1::g_harrierMemory[offset + 1]);
}
std::uint32_t Colour(std::size_t offset) {
    return paletteLookup[MemoryWord(offset) & 0x7fff];
}

void DrawTextureQuad(float left, float top, float right, float bottom, float z, float u0, float v0, float u1, float v1) {
    const std::array<EmulatorPositionTextureVertex, 4> vertices{{
        {.x = left, .y = top, .z = z, .w = 1, .u = u0, .v = v0},
        {.x = right, .y = top, .z = z, .w = 1, .u = u1, .v = v0},
        {.x = left, .y = bottom, .z = z, .w = 1, .u = u0, .v = v1},
        {.x = right, .y = bottom, .z = z, .w = 1, .u = u1, .v = v1},
    }};
    DrawEmulatorVertices(vertices.data(), 24, 4, 32, 16);
}
void DrawColourQuad(float top, float bottom, float z, std::uint32_t colour) {
    const std::array<EmulatorPositionColourVertex, 4> vertices{{
        {.x = 0, .y = top, .z = z, .w = 1, .colour = colour},
        {.x = 640, .y = top, .z = z, .w = 1, .colour = colour},
        {.x = 0, .y = bottom, .z = z, .w = 1, .colour = colour},
        {.x = 640, .y = bottom, .z = z, .w = 1, .colour = colour},
    }};
    DrawEmulatorVertices(vertices.data(), 20, 4, 30, 14);
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
        std::memcpy(header, &signature, 2);
        const std::uint64_t size = 4150, planes = 0x200001, resolution = 2834;
        std::memcpy(header + 2, &size, 8);
        constexpr std::uint32_t offset = 54, infoSize = 40, extent = 32, zero = 0;
        std::memcpy(header + 10, &offset, 4);
        std::memcpy(header + 14, &infoSize, 4);
        std::memcpy(header + 18, &extent, 4);
        std::memcpy(header + 22, &extent, 4);
        std::memcpy(header + 26, &planes, 8);
        std::memcpy(header + 34, &zero, 4);
        std::memcpy(header + 38, &resolution, 4);
        std::memcpy(header + 42, &resolution, 8);
        std::memcpy(header + 50, &zero, 4);
    }
}
void InitializeSpriteCache() {
    spriteTextures.fill(0);
    for (std::size_t index = 0; index < 160; ++index) {
        OutRunSpriteCache &entry = spriteCache[index];
        entry.textureSlot = static_cast<std::int32_t>(index);
        entry.cachedPattern = -1;
        entry.claimed = 0;
        entry.cachedHeight = -1;
    }
    spriteCursor = 0;
    averageColour = kTransparent;
    previousAverageColour = kTransparent;
    for (std::array<std::uint32_t, 16> &palette : spritePalettes)
        palette.fill(kTransparent);
}
void UpdatePalettes() {
    for (std::size_t index = 0; index < 128; ++index) {
        tilePaletteDirty[index] = 0;
        tilePalettes[index][0] = kTransparent;
        for (std::size_t entry = 1; entry < 8; ++entry) {
            const std::uint32_t colour = Colour(0xb0000 + index * 16 + entry * 2);
            if (tilePalettes[index][entry] != colour) {
                tilePaletteDirty[index] = 1;
                tilePalettes[index][entry] = colour;
            }
        }
        spritePaletteDirty[index] = 0;
        spritePalettes[index][0] = kTransparent;
        spritePalettes[index][15] = kTransparent;
        for (std::size_t entry = 1; entry < 15; ++entry) {
            const std::uint32_t colour = Colour(0xb1000 + index * 32 + entry * 2);
            if (spritePalettes[index][entry] != colour) {
                spritePaletteDirty[index] = 1;
                spritePalettes[index][entry] = colour;
            }
        }
    }
    previousAverageColour = averageColour;
    const std::uint32_t first = paletteLookup[MemoryWord(0xb0800)];
    const std::uint32_t second = paletteLookup[MemoryWord(0xb0802)];
    averageColour = 0xff000000 + ((75 * ((first & 0xff0000) + (second & 0xff0000)) / 200) & 0xff0000) +
        ((75 * ((first & 0xff00) + (second & 0xff00)) / 200) & 0xff00) +
        static_cast<std::uint8_t>(75 * (static_cast<std::uint8_t>(first) + static_cast<std::uint8_t>(second)) / 200);
}

void CopyTileMap(std::int32_t x, std::int32_t y, const std::uint8_t *source, std::uint16_t *destination) {
    std::int32_t columns = 64, rows = 32;
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
    const std::int32_t clippedColumns = (328 - x) / 8, clippedRows = (232 - y) / 8;
    if (columns > clippedColumns)
        columns = clippedColumns;
    if (rows > clippedRows)
        rows = clippedRows;
    for (std::int32_t row = 0; row < rows; ++row) {
        for (std::int32_t column = 0; column < columns; ++column) {
            *destination++ = static_cast<std::uint16_t>((source[0] << 8) + source[1]);
            source += 2;
        }
        destination += 64 - columns;
        source += 2 * (64 - columns);
    }
}
void UpdateTile(std::int32_t x, std::int32_t y, std::int32_t bankOffset, std::uint16_t pattern, std::uint16_t palette) {
    const std::int32_t index = 16 * (y / 4) + bankOffset + x / 4;
    const std::int32_t cellX = x & 3, cellY = y & 3;
    const std::uint16_t mask = static_cast<std::uint16_t>(1U << (cellX + 4 * cellY));
    tileDirty[index] = 1;
    if (kOutRunTileOccupied[pattern])
        tileOccupancy[index] |= mask;
    else
        tileOccupancy[index] &= static_cast<std::uint16_t>(~mask);
    const std::byte *const colours = reinterpret_cast<const std::byte *>(tilePalettes.data()) + 4 * palette;
    for (std::size_t row = 0; row < 8; ++row) {
        for (std::size_t column = 0; column < 8; ++column) {
            std::uint32_t colour = kTransparent;
            if (kOutRunTileOccupied[pattern])
                std::memcpy(&colour, colours + backgroundGraphics[64 * pattern + 8 * row + column], sizeof(colour));
            const std::size_t offset = 32 * (cellX + 32 * cellY) + row * 128 + column * 4;
            std::memcpy(tilePixels[index].data() + offset, &colour, sizeof(colour));
        }
    }
}
void UpdateTileTextures(std::int32_t bank) {
    const std::int32_t rows = bank == 0 ? 28 : 32, columns = bank == 0 ? 40 : 64;
    for (std::int32_t y = 0; y < rows; ++y) {
        for (std::int32_t x = 0; x < columns; ++x) {
            const std::uint16_t code = tileMaps[bank][64 * y + x];
            std::uint16_t &previous = tileMaps[bank + 3][64 * y + x];
            const std::uint16_t paletteIndex = bank == 0 ? (code >> 9) & 31 : (code >> 6) & 127;
            if (tilePaletteDirty[paletteIndex] || code != previous) {
                previous = code;
                UpdateTile(x, y, bank * 128, bank == 0 ? code & 511 : code & 8191, bank == 0 ? (code >> 6) & 248 : (code >> 3) & 1016);
            }
        }
    }
    for (std::int32_t index = bank * 128; index < bank * 128 + 128; ++index) {
        if (!tileDirty[index])
            continue;
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
void DrawTileBank(std::int32_t bank, std::int32_t x, std::int32_t y, float z) {
    const std::int32_t left = x * 2;
    std::int32_t top = y * 2 + 16;
    std::int32_t index = bank * 128;
    for (std::int32_t row = 0; row < 8; ++row, top += 64) {
        std::int32_t currentX = left;
        for (std::int32_t column = 0; column < 16; ++column, ++index, currentX += 64) {
            if (static_cast<std::uint32_t>(currentX + 63) <= 702 && static_cast<std::uint32_t>(top + 47) <= 510 && tileOccupancy[index]) {
                BindEmulatorTexture(tileTextures[index]);
                DrawTextureQuad(static_cast<float>(currentX), static_cast<float>(top), static_cast<float>(currentX + 64), static_cast<float>(top + 64), z, 0,
                    .99900001f, 1, 0);
            }
        }
    }
}
void DrawScrollingLayer(std::int32_t bank) {
    const std::size_t pageOffset = bank == 2 ? 0xa0e82 : 0xa0e80;
    const std::size_t xOffset = bank == 2 ? 0xa0e9a : 0xa0e98;
    const std::size_t yOffset = bank == 2 ? 0xa0e92 : 0xa0e90;
    const std::uint16_t pages = MemoryWord(pageOffset);
    const std::int32_t x = ((s1::g_harrierMemory[xOffset + 1] - (s1::g_harrierMemory[xOffset] << 8)) & 511) - 704;
    const std::int32_t rawY = (s1::g_harrierMemory[yOffset + 1] - (s1::g_harrierMemory[yOffset] << 8)) & 511;
    const std::int32_t y = rawY <= 480 ? -rawY : 512 - rawY;
    CopyTileMap(x, y + 256, s1::g_harrierMemory + 0x90000 + 4096 * (pages >> 12), tileMaps[bank].data());
    CopyTileMap(x + 512, y + 256, s1::g_harrierMemory + 0x90000 + 4096 * ((pages >> 8) & 15), tileMaps[bank].data());
    CopyTileMap(x, y, s1::g_harrierMemory + 0x90000 + 4096 * (pages >> 4), tileMaps[bank].data());
    CopyTileMap(x + 512, y, s1::g_harrierMemory + 0x90000 + 4096 * (pages & 15), tileMaps[bank].data());
    UpdateTileTextures(bank);
    SetEmulatorDefaultRenderFlags();
    const float z = bank == 2 ? .026666664f : .023333332f;
    DrawTileBank(bank, x, y, z);
    DrawTileBank(bank, x + 512, y, z);
    DrawTileBank(bank, x, y + 256, z);
    DrawTileBank(bank, x + 512, y + 256, z);
}

// @verified - p
void SwapSpriteCache(OutRunSpriteCache &target, OutRunSpriteCache &donor) {
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
bool MatchesSprite(const OutRunSpriteCache &entry, std::int32_t pattern, std::int32_t palette, std::int32_t height) {
    return entry.cachedPattern == pattern && entry.cachedPalette == palette && entry.cachedHeight == height;
}
void PrepareSpriteCache(std::int32_t index, std::int32_t pattern, std::int32_t palette, std::int32_t height, std::int16_t width) {
    OutRunSpriteCache &target = spriteCache[index];
    OutRunSpriteCache *match = nullptr;
    if (!(target.hasAverage && averageColour != previousAverageColour)) {
        const std::int32_t lower = index - 32 >= 0 ? index - 32 : 0;
        for (std::int32_t candidate = index; candidate >= lower; --candidate)
            if (MatchesSprite(spriteCache[candidate], pattern, palette, height)) {
                match = &spriteCache[candidate];
                break;
            }
        const std::int32_t upper = index + 32 < 128 ? index + 32 : 128;
        if (!match)
            for (std::int32_t candidate = index + 1; candidate < upper; ++candidate)
                if (MatchesSprite(spriteCache[candidate], pattern, palette, height)) {
                    match = &spriteCache[candidate];
                    break;
                }
        if (!match)
            for (std::int32_t candidate = 128; candidate < 160; ++candidate)
                if (MatchesSprite(spriteCache[candidate], pattern, palette, height)) {
                    match = &spriteCache[candidate];
                    break;
                }
    }
    if (!match) {
        target.width = width;
        target.cache = nullptr;
        target.pattern = pattern;
        target.palette = static_cast<std::int16_t>(palette);
        target.height = static_cast<std::int16_t>(height);
    } else if (match->claimed)
        target.cache = match;
    else {
        SwapSpriteCache(target, *match);
        target.claimed = 1;
        target.cache = &target;
    }
}
void DecodeSprite(std::int32_t index, OutRunSpriteCache &target) {
    if (!(target.hasAverage && averageColour != previousAverageColour)) {
        const std::int32_t lower = index - 32 >= 0 ? index - 32 : 0;
        for (std::int32_t candidate = index - 1; candidate >= lower; --candidate) {
            if (MatchesSprite(spriteCache[candidate], target.pattern, target.palette, target.height)) {
                target.cache = &spriteCache[candidate];
                return;
            }
        }
    }
    spriteCursor = spriteCursor + 1 < 32 ? spriteCursor + 1 : 0;
    SwapSpriteCache(target, spriteCache[spriteCursor + 128]);
    target.cache = &target;
    target.claimed = 1;
    d3t::InitTexture2DResource(spriteTextures[target.textureSlot]);
    target.cachedPalette = target.palette;
    target.cachedHeight = target.height;
    target.cachedPattern = target.pattern;
    std::array<std::uint32_t, 16> colours = spritePalettes[target.palette & 127];
    if (target.hasAverage)
        colours[10] = averageColour;
    const std::byte *const palette = reinterpret_cast<const std::byte *>(colours.data());
    const std::uint8_t *source = spriteGraphics.data() + 8 * target.pattern;
    std::uint32_t *destination = spritePixels.data();
    const std::uint32_t height = static_cast<std::uint16_t>(target.height);
    for (std::uint32_t row = 0; row < height; ++row) {
        std::int32_t written = 0;
        while (written < target.width) {
            for (std::int32_t pixel = 0; pixel < 8; ++pixel) {
                const std::uint8_t colourOffset = target.flip ? source[-pixel - 1] : source[pixel];
                std::memcpy(destination++, palette + colourOffset, 4);
            }
            source += target.flip ? -8 : 8;
            written += 8;
            if ((target.flip ? source[1] : source[-2]) == 60) {
                while (written < target.width) {
                    source += target.flip ? -8 : 8;
                    for (std::int32_t pixel = 0; pixel < 8; ++pixel)
                        *destination++ = kTransparent;
                    written += 8;
                }
                break;
            }
        }
        if (target.flip)
            source += 2 * target.width;
    }
    const d3t::Texture2DDescription description{.width = static_cast<std::uint32_t>(target.width),
        .height = height,
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
}
void DrawSprites() {
    for (OutRunSpriteCache &entry : spriteCache) {
        entry.claimed = 0;
        if (spritePaletteDirty[static_cast<std::uint16_t>(entry.cachedPalette) & 127])
            entry.cachedPattern = -1;
    }
    std::int32_t count = 128;
    for (std::int32_t index = 0; index < 128; ++index) {
        const std::uint8_t *const source = g_outRunRoadSnapshot.data() + 16 * index;
        std::uint32_t sentinel;
        std::memcpy(&sentinel, source, 4);
        if ((sentinel & 0xffff00fe) == 0xffff00fe) {
            count = index;
            break;
        }
        OutRunSpriteCache &entry = spriteCache[index];
        if (static_cast<std::int8_t>(source[8]) >= 0) {
            entry.active = 0;
            continue;
        }
        entry.active = 1;
        entry.hasAverage = source[6] & 64;
        std::int32_t x = ((source[5] - (source[4] << 8)) & 511) - 192;
        if (x > 320)
            x -= 512;
        const std::int32_t width = (source[4] & 126) ? 4 * (source[4] & 126) : 256;
        std::int32_t pattern = source[3] + ((source[2] + ((source[0] & 6) << 7)) << 8);
        float xScale = 512.0f / static_cast<float>((source[7] + (source[6] << 8)) & 1023);
        const std::int32_t authoredHeight = source[10] + 1;
        std::int32_t height =
            static_cast<std::int32_t>(static_cast<float>(authoredHeight) / (512.0f / static_cast<float>(((source[8] << 8) + source[9]) & 1023)));
        if (height < 1)
            height = 1;
        switch (source[8] & 96) {
        case 0:
            ++pattern;
            entry.flip = 1;
            xScale = -xScale;
            break;
        case 32:
            ++pattern;
            entry.flip = 1;
            break;
        case 64:
            entry.flip = 0;
            xScale = -xScale;
            break;
        case 96:
            entry.flip = 0;
            break;
        }
        entry.left = static_cast<float>(2 * x);
        entry.right = static_cast<float>(width) * xScale + static_cast<float>(width) * xScale + static_cast<float>(2 * x);
        entry.top = static_cast<float>(2 * source[1] + 16);
        entry.bottom = static_cast<float>(2 * (authoredHeight + source[1]) + 16) + .99989998f;
        PrepareSpriteCache(index, pattern, source[11], height, static_cast<std::int16_t>(width));
    }
    SetEmulatorDefaultRenderFlags();
    for (std::int32_t index = 0; index < count; ++index) {
        OutRunSpriteCache &entry = spriteCache[index];
        if (!entry.active)
            continue;
        if (!entry.cache)
            DecodeSprite(index, entry);
        BindEmulatorTexture(spriteTextures[entry.cache->textureSlot]);
        DrawTextureQuad(entry.left, entry.top, entry.right, entry.bottom, .0099999998f, 0, .99902344f, 1, .0009765625f);
    }
}

void ProduceRoad(std::size_t source, std::size_t pattern, std::uint16_t mask, std::int32_t lane) {
    for (std::size_t row = 0; row < 224; ++row) {
        const std::size_t index = lane * 224 + row;
        const std::uint16_t word = MemoryWord(source + row * 2);
        if ((word & 0xc00) != mask) {
            roadActive[index] = 0;
            continue;
        }
        roadActive[index] = 1;
        const std::uint32_t offset = 2 * (word & 511);
        roadU[index] = static_cast<float>(offset) * .00024414062f;
        const std::int32_t position = 1528 - (MemoryWord(pattern + offset) & 4095);
        roadPosition[index] = static_cast<float>(position) + static_cast<float>(position);
        roadColour[index] = MemoryWord(0xe0c00 + offset);
    }
}
void DrawRoadPass(std::int32_t lane, std::int32_t pass) {
    constexpr std::array<float, 4> depths{.012575955f, .012087674f, .011599393f, .011111111f};
    constexpr std::array<float, 4> offsets{.75f, .5f, .25f, 0};
    SetEmulatorDefaultRenderFlags();
    BindEmulatorTexture(patternTexture);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::size_t index = lane * 224 + row;
        EmulatorPositionColourTextureVertex *const vertices = roadVertices.data() + row * 4;
        for (std::size_t vertex = 0; vertex < 4; ++vertex)
            vertices[vertex].z = depths[pass];
        const float left = roadActive[index] ? (pass == 0 ? 0 : roadPosition[index]) : -10;
        const float right = roadActive[index] ? (pass == 0 ? 640 : roadPosition[index] + 1024) : -10;
        vertices[0].x = left;
        vertices[1].x = right;
        vertices[2].x = left;
        vertices[3].x = right;
        if (!roadActive[index])
            continue;
        const float u = roadU[index] + offsets[pass];
        std::size_t colourOffset;
        if (pass == 0)
            colourOffset = 0xb0840 + ((roadColour[index] >> 7) & 30);
        else {
            const bool alternate = (row & 7) == 7 ? roadColour[index] != 0 : static_cast<std::uint8_t>(roadColour[index]) != 0;
            colourOffset = 0xb0800 + 4 * (3 - pass) + (alternate ? 2 : 0);
        }
        const std::uint32_t colour = Colour(colourOffset);
        for (std::size_t vertex = 0; vertex < 4; ++vertex) {
            vertices[vertex].u = u;
            vertices[vertex].colour = colour;
        }
    }
    DrawEmulatorVertices(roadVertices.data(), 28, 894, 31, 15);
}
void DrawRoads() {
    const std::uint8_t flags = g_arcadeRoadControl;
    if (!(flags & 2))
        ProduceRoad(0xe0000, 0xe0400, 0, 0);
    if (flags & 1)
        ProduceRoad(0xe0200, 0xe0800, 0, 1);
    if (!(flags & 2))
        DrawRoadPass(0, 0);
    if ((flags & 3) == 3)
        DrawRoadPass(1, 0);
    for (std::int32_t pass = 1; pass < 4; ++pass) {
        if (!(flags & 2))
            DrawRoadPass(0, pass);
        if (flags & 1)
            DrawRoadPass(1, pass);
    }
}
void DrawForeground() {
    for (std::size_t row = 0; row < 29; ++row)
        for (std::size_t column = 0; column < 41; ++column)
            tileMaps[0][row * 64 + column] = MemoryWord(0xa0030 + row * 128 + column * 2);
    UpdateTileTextures(0);
    SetEmulatorDefaultRenderFlags();
    DrawTileBank(0, 0, 0, .0090909088f);
}

} // namespace

void InitializeOutRunGraphics() {
    for (std::uint32_t value = 0; value < 32768; ++value) {
        const std::uint32_t red = 255 * (((value >> 12) & 1) + 2 * (value & 15)) / 31;
        const std::uint32_t green = 255 * (((value >> 13) & 1) + ((value >> 3) & 30)) / 31;
        const std::uint32_t blue = 255 * (((value >> 14) & 1) + ((value >> 7) & 30)) / 31;
        paletteLookup[value] = 0xff000000 | (red << 16) | (green << 8) | blue;
    }
    LoadEmulatorFile("XOUT/datas/orxobj.bin", spriteGraphics.data(), 0);
    LoadEmulatorFile("XOUT/datas/orxbg.bin", backgroundGraphics.data(), 0);
    LoadEmulatorFile("XOUT/datas/orcoins.bin", coinGraphics.data(), 0);
    InitializeSpriteCache();
    InitializeTileCache();
    std::memcpy(backgroundGraphics.data() + 31744, coinGraphics.data() + 832, 832);
    fillTexture = LoadEmulatorImage("XOUT/datas/fill.bmp");
    patternTexture = LoadEmulatorImage("XOUT/datas/oricpat.bmp");
}

// @verified - p
void InitializeOutRunRoadVertices() {
    g_outRunRoadSnapshot.fill(0xff);
    for (std::size_t row = 0; row < 224; ++row) {
        const float top = static_cast<float>(row) + static_cast<float>(row) + 16;
        for (std::size_t vertex = 0; vertex < 4; ++vertex) {
            EmulatorPositionColourTextureVertex &entry = roadVertices[row * 4 + vertex];
            entry.w = 1;
            entry.u = static_cast<float>(vertex & 1);
            entry.v = .80000001f;
            entry.y = vertex < 2 ? top : top + 2;
        }
    }
}

// @verified - p
void ReleaseOutRunGraphics() {
    for (std::uint32_t texture : spriteTextures)
        d3t::InitTexture2DResource(texture);
    for (std::uint32_t texture : tileTextures)
        d3t::InitTexture2DResource(texture);
    d3t::InitTexture2DResource(patternTexture);
    d3t::InitTexture2DResource(fillTexture);
}

void RenderOutRunFrame() {
    UpdatePalettes();
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    BindEmulatorTexture(fillTexture);
    for (std::size_t row = 0; row < 224; ++row) {
        const std::uint16_t word = MemoryWord(0xe0000 + row * 2);
        if ((word & 0x800) == 0x800) {
            const float top = static_cast<float>(row * 2 + 16);
            DrawColourQuad(top, top + 2, .1f, Colour(0xb0f00 + 2 * (word & 63)));
        }
    }
    DrawScrollingLayer(2);
    DrawScrollingLayer(1);
    DrawRoads();
    DrawSprites();
    DrawForeground();
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 0);
    BindEmulatorTexture(fillTexture);
    DrawColourQuad(0, 16, .001f, 0xff000000);
    DrawColourQuad(464, 480, .001f, 0xff000000);
}

} // namespace shenmue::s2
