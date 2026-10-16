#include "s2/navi.hpp"
#include "s2/navigation.hpp"

#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/ui.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/render_state.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"

#include <cmath>
#include <algorithm>
#include <cstdio>

#include <cstring>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
namespace {

constexpr NaviMapConfig mapConfigs[] = {
    {6012, -1, 2.5f, 248.0f, 350.0f, "AR00"},
    {6006, -1, 2.0f, 160.0f, 110.0f, "WE00"},
    {6007, -1, 1.5f, 180.0f, 175.0f, "WR00"},
    {6008, -1, 1.5f, 105.0f, 267.0f, "WK00"},
    {6010, -1, 1.5f, 262.0f, 198.0f, "WN00"},
    {6027, -1, 1.5f, 315.0f, 250.0f, "WB00"},
    {6011, -1, 1.5f, 65.0f, 155.0f, "WS00"},
    {6009, -1, 1.5f, 220.0f, 280.0f, "WT00"},
    {6014, -1, 2.0f, 223.0f, 531.0f, "AB00"},
    {6003, 2, 0.53f, 170.1f, 150.3f, "QC00"},
    {6004, 3, 0.53f, 187.2f, 91.8f, "QD00"},
    {6005, 4, 0.7f, 264.6f, 86.4f, "QE00"},
    {6023, 5, 0.7f, 228.6f, 147.6f, "QF00"},
    {6001, 0, 1.0f, 247.5f, 183.6f, "QA00"},
    {6000, -1, 2.0f, 222.3f, 146.7f, "Q100"},
    {-1, -1, 8.0f, -512.0f, -512.0f, nullptr},
    {-1, -1, 2.0f, 61.7775f, 107.275f, nullptr},
    {6015, -1, 2.0f, 226.0f, 245.0f, "AK00"},
    {6013, -1, 2.0f, 310.0f, 452.0f, "AS00"},
    {6020, -1, 1.875f, 189.5f, 168.5f, "QH00"},
    {6019, -1, 1.25f, 129.0f, 165.5f, "QR00"},
    {6021, -1, 1.25f, 262.5f, 179.0f, "QT00"},
    {6022, -1, 1.875f, 294.0f, 102.0f, "QY00"},
    {6001, 0, 1.0f, 247.5f, 183.6f, "QA99"},
    {6003, 2, 0.53f, 170.1f, 150.3f, "QC99"},
    {6004, 3, 0.53f, 187.2f, 91.8f, "QD99"},
    {6005, 4, 0.7f, 264.6f, 86.4f, "QE99"},
    {6023, 5, 0.7f, 228.6f, 147.6f, "QF99"},
    {6023, -1, 0.69f, 229.0f, 147.6f, "QF40"},
    {6001, -1, 1.0f, 247.0f, 183.0f, "QA11"},
    {6002, 1, 0.25f, 205.0f, 193.0f, "QB00"},
    {6003, -1, 0.53f, 170.1f, 150.3f, "QC00"},
};

constexpr NaviAreaConfig areaConfigs[] = {
    {SHEN_NAME(AR00), 0},
    {SHEN_NAME(AR01), 0},
    {SHEN_NAME(AR02), 0},
    {SHEN_NAME(AR03), 18},
    {SHEN_NAME(AK00), 17},
    {SHEN_NAME(AB00), 8},
    {SHEN_NAME(WE00), 1},
    {SHEN_NAME(WR00), 2},
    {SHEN_NAME(WK00), 3},
    {SHEN_NAME(WN00), 4},
    {SHEN_NAME(WB00), 5},
    {SHEN_NAME(WS00), 6},
    {SHEN_NAME(WT00), 7},
    {SHEN_NAME(KWWT), 15},
    {SHEN_NAME(VIEW), 15},
    {SHEN_NAME(KMZ1), 15},
    {SHEN_NAME(KMZ2), 15},
    {SHEN_NAME(KMZ3), 15},
    {SHEN_NAME(KMZ4), 15},
    {SHEN_NAME(KES1), 16},
    {SHEN_NAME(Q100), 19},
    {SHEN_NAME(Q200), 22},
    {SHEN_NAME(Q300), 21},
    {SHEN_NAME(QA00), 23},
    {SHEN_NAME(QAW1), 13},
    {SHEN_NAME(QAE1), 13},
    {SHEN_NAME(QAW6), 23},
    {SHEN_NAME(QAE6), 23},
    {SHEN_NAME(QJBT), 13},
    {SHEN_NAME(QA11), 29},
    {SHEN_NAME(QB00), 30},
    {SHEN_NAME(QC00), 24},
    {SHEN_NAME(QC01), 9},
    {SHEN_NAME(QC06), 24},
    {SHEN_NAME(QCAE), 31},
    {SHEN_NAME(QD00), 25},
    {SHEN_NAME(QD01), 10},
    {SHEN_NAME(QE00), 26},
    {SHEN_NAME(QE01), 11},
    {SHEN_NAME(QE06), 26},
    {SHEN_NAME(QLBT), 26},
    {SHEN_NAME(QF00), 12},
    {SHEN_NAME(QF01), 12},
    {SHEN_NAME(QF02), 12},
    {SHEN_NAME(QF39), 28},
    {SHEN_NAME(QF06), 12},
    {SHEN_NAME(QR00), 20},
    {SHEN_NAME(QSFD), 19},
    {SHEN_NAME(QSFA), 19},
    {SHEN_NAME(QSB1), 19},
    {SHEN_NAME(QTB2), 19},
    {SHEN_NAME(QTB3), 21},
    {SHEN_NAME(QTB4), 21},
    {SHEN_NAME(QTB1), 19},
    {SHEN_NAME(QSFC), 22},
    {SHEN_NAME(QSFB), 22},
    {SHEN_NAME(QSB1), 22},
    {SHEN_NAME(QGBT), 22},
};

constexpr NaviFloorImage floorImagesQA[] = {
    {23, 48, 32, 0, 0x3fc0}, {10, 33, 16, 1, 0x3800},
    {10, 26, 8, 2, 0xfc000}, {10, 36, 8, 2, 0x3c0},
};
constexpr NaviFloorImage floorImagesQB[] = {{-17, 12, 16, 0, 0x400}};
constexpr NaviFloorImage floorImagesQC[] = {{-10, -60, 16, 0, 0x9000}};
constexpr NaviFloorImage floorImagesQD[] = {{-43, 50, 16, 0, 0x9000}};
constexpr NaviFloorImage floorImagesQE[] = {{-65, -18, 16, 0, 0xff800}};
constexpr NaviFloorImage floorImagesQF[] = {
    {31, -25, 64, 0, 0xfffffffff0000000ULL}, {-12, -5, 8, 1, 0xcff4fff},
    {-12, -5, 8, 2, 0x300b000}, {11, 12, 32, 3, 0xf87ffff}, {11, 12, 32, 4, 0x780000},
};
constexpr NaviFloorDefinition floorDefinitions[] = {
    {"NVPTQA.SPR", 4, 0, floorImagesQA, 0xfffffffffffffc00ULL},
    {"NVPTQB.SPR", 1, 0, floorImagesQB, 0xffffffff0fffffffULL},
    {"NVPTQC.SPR", 1, 0, floorImagesQC, 0xffffffffffffffffULL},
    {"NVPTQD.SPR", 1, 0, floorImagesQD, 0xffffffffffffffffULL},
    {"NVPTQE.SPR", 1, 0, floorImagesQE, 0xffffffffffffffffULL},
    {"NVPTQF.SPR", 5, 0, floorImagesQF, 0x0fffffff},
};

std::array<std::uint32_t, 128> markers{};
std::uint32_t nextMarker{};
const std::uint32_t *markerSaveSource{};
NaviTrailStorage trailStorage{};
const NaviTrailStorage *trailSaveSource{};
shen_name retainedTrailMap{};

std::uint32_t CompactMarkers() noexcept {
    std::array<std::uint32_t, 128> compacted{};
    std::uint32_t count = 0;
    for (std::uint32_t index = 0; index < 128; ++index) {
        const std::uint32_t marker = markers[(nextMarker + index) & 0x7f];
        if (marker & 0xe)
            compacted[count++] = marker;
    }
    markers = compacted;
    nextMarker = count;
    return count;
}

constexpr std::size_t trailTextureSize = 512 * 512 * 4 + 176;
constexpr std::uint32_t trailColours[16] = {0xffffffff, 0xff000000, 0xff00ff00, 0xffffff00, 0xffff0000};
constexpr std::uint8_t stampBits[3][8] = {
    {0, 0, 0x18, 0x3c, 0x3c, 0x18, 0, 0},
    {0, 0, 0x10, 0x38, 0x7c, 0xfe, 0, 0},
    {0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18},
};
constexpr NaviAreaConfig trailTransfers[] = {
    {SHEN_NAME(KMZ1), SHEN_NAME(KMZ2)},
    {SHEN_NAME(KMZ2), SHEN_NAME(KMZ3)},
    {SHEN_NAME(KMZ3), SHEN_NAME(KMZ4)},
    {SHEN_NAME(KMZ3), SHEN_NAME(KMZ1)},
    {SHEN_NAME(KMZ4), SHEN_NAME(KMZ1)},
    {SHEN_NAME(KMZ2), SHEN_NAME(KMZ1)},
    {SHEN_NAME(KMZ1), SHEN_NAME(KMZ1)},
};

HLib::Task *naviTask{};
std::uint32_t naviFlags{};
std::int32_t naviPhase{-1};
std::uint32_t markerButtons{};
float triggerBlend{};
float displayOffsetX{};
float displayOffsetY{};
std::uint8_t trailStamp{};
std::uint32_t naviDrawPending{};
std::uint32_t naviScissorPending{};
std::uint32_t mapTexture{};
std::uint32_t backgroundTexture{};
std::array<std::uint32_t, 6> floorTextures{};
alignas(16) std::array<std::byte, trailTextureSize> retainedTrailTexture{};
std::array<std::uint32_t, 512 * 512> trailPixels{};
std::byte *activeTrailTexture{};

using NaviQuad = std::array<Vector2, 4>;
struct NaviIcon {
    CommonIconAtlas *atlas{};
    Vector4 uv{};
};
struct NaviDrawState {
    NaviTaskData *task{};
    Matrix4x4 transform{};
    float offsetX{};
    float offsetZ{};
    std::uint32_t flags{};
};
static_assert(offsetof(NaviDrawState, transform) == 8);
static_assert(offsetof(NaviDrawState, flags) == 0x50);

constexpr NaviQuad mapCorners{{
    {-1.0f, -1.0f},
    {1.0f, -1.0f},
    {-1.0f, 1.0f},
    {1.0f, 1.0f},
}};
std::array<NaviIcon, 6> naviIcons{};
NaviQuad mapQuad{};
NaviQuad playerQuad{};
NaviQuad backgroundQuad{};
NaviDrawState naviDrawState{};

Vector4 NaviIconUv(std::uint16_t id) {
    const auto rect = GetCommonIconRect(id);
    return {rect[0] * 0.125f, rect[1] * 0.125f, rect[2] * 0.125f, rect[3] * 0.125f};
}

void ResetNaviIcons() {
    constexpr std::uint16_t ids[]{0, 1044, 60, 61, 59, 516};
    naviIcons = {};
    for (std::size_t index = 1; index < naviIcons.size(); ++index) {
        naviIcons[index] = {GetCommonIconAtlas(ids[index]), NaviIconUv(ids[index])};
    }
    ReplaceCommonIconAtlasImage(1044, "/maps/dot.png?usage=0");
    ReplaceCommonIconAtlasImage(59, "/maps/hudicons.tga?usage=0");
    ReplaceCommonIconAtlasImage(516, "/maps/pink_cross.png?usage=0");
    naviIcons[1].uv.y = 0.133f;
    naviIcons[1].uv.w = 0.24f;
}

Vector2 TransformNaviPoint(const Matrix4x4 &matrix, float x, float z) {
    return {
        matrix.rows[0].x * x + matrix.rows[2].x * z + matrix.rows[3].x,
        matrix.rows[0].z * x + matrix.rows[2].z * z + matrix.rows[3].z,
    };
}

NaviQuad MakeNaviQuad(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, float aspect) {
    const float left = static_cast<float>(x - width / 2) * aspect;
    const float right = static_cast<float>(x - width / 2 + width) * aspect;
    const float top = static_cast<float>(y - height / 2);
    const float bottom = static_cast<float>(y - height / 2 + height);
    return {{{left, top}, {right, top}, {left, bottom}, {right, bottom}}};
}

// @verified - p
std::uint32_t RequestNaviDrawUpdate() {
    if (!naviDrawPending)
        naviDrawPending = 1;
    return naviDrawPending;
}

// @verified - p
std::uint32_t RequestNaviScissorUpdate() {
    if (!naviScissorPending)
        naviScissorPending = 1;
    return naviScissorPending;
}

void DrawNaviQuad(const NaviQuad &quad, std::uint32_t texture, float depth, const Vector4 &uv = {0.0f, 0.0f, 1.0f, 1.0f}, std::uint32_t diffuse = 0xffffffff,
    std::uint32_t specular = 0) {
    if (!texture)
        return;
    // SetDrawWithAlpha leaves V reversed unless strip flag 0x200
    // is set. NAVI's six reset strips retain only the alpha-test flag 0x400.
    const std::array<LegacyStripType3Vertex, 4> vertices{{
        {{quad[0].x, quad[0].y, depth}, uv.x, uv.w, diffuse, specular},
        {{quad[1].x, quad[1].y, depth}, uv.z, uv.w, diffuse, specular},
        {{quad[2].x, quad[2].y, depth}, uv.x, uv.y, diffuse, specular},
        {{quad[3].x, quad[3].y, depth}, uv.z, uv.y, diffuse, specular},
    }};
    DrawLegacyStripType3AlphaTestImmediate(vertices, texture);
}

void ReleaseTexture(std::uint32_t &texture) {
    if (texture)
        d3t::InitTexture2DResource(texture);
    texture = 0;
}

void WriteTextureWord(std::byte *bytes, std::size_t offset, std::uint32_t value) {
    std::memcpy(bytes + offset, &value, sizeof(value));
}

std::uint32_t ReadTextureWord(const std::byte *bytes) {
    std::uint32_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}

void InitializeTrailTexture(std::byte *bytes) {
    std::memset(bytes, 0, trailTextureSize);
    WriteTextureWord(bytes, 0, SHEN_NAME(TEXN));
    WriteTextureWord(bytes, 4, trailTextureSize);
    WriteTextureWord(bytes, 8, SHEN_NAME(NAV0));
    WriteTextureWord(bytes, 12, SHEN_NAME(AUTO));
    WriteTextureWord(bytes, 16, SHEN_NAME(GBIX));
    WriteTextureWord(bytes, 20, 4);
    WriteTextureWord(bytes, 28, SHEN_NAME(PVRT));
    WriteTextureWord(bytes, 32, 512 * 512 * 4 + 8);
    WriteTextureWord(bytes, 36, 0x8089);
    WriteTextureWord(bytes, 40, 512 | (512 << 16));
    std::byte *const dds = bytes + 48;
    WriteTextureWord(dds, 0, make_shen_name('D', 'D', 'S', ' '));
    WriteTextureWord(dds, 4, 124);
    WriteTextureWord(dds, 8, 4111);
    WriteTextureWord(dds, 12, 512);
    WriteTextureWord(dds, 16, 512);
    WriteTextureWord(dds, 20, 2048);
    constexpr std::uint32_t format[] = {32, 65, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};
    std::memcpy(dds + 76, format, sizeof(format));
    WriteTextureWord(dds, 108, 4096);
}

void OverrideMapTexture(std::uint32_t &texture, const char *image) {
    if (!texture)
        return;
    char filename[128];
    const std::size_t length = std::strcspn(image, "?");
    if (length >= sizeof(filename))
        return;
    std::memcpy(filename, image, length);
    filename[length] = '\0';
    if (!ArchiveData_GetFileSize(filename))
        return;
    std::uint32_t replacement = 0;
    d3t::LoadImageTexture(image, false, &replacement, 0);
    if (!replacement)
        return;
    ReleaseTexture(texture);
    texture = replacement;
}

void *LoadSpriteAllocation(const char *filename, std::size_t &size) {
    char path[128];
    std::snprintf(path, sizeof(path), "/SPRITE/%s", filename);
    size = static_cast<std::size_t>(ArchiveData_GetFileSize(path));
    if (!size)
        return nullptr;
    void *const allocation = HLib::AllocateTaskData(size, SHEN_NAME(NAVI));
    if (!allocation)
        return nullptr;
    if (ArchiveData_Read(path, 0, allocation, size))
        return allocation;
    HLib::FreeTaskData(allocation);
    return nullptr;
}

void UploadTrailTexture() {
    if (!activeTrailTexture)
        return;
    const std::uint32_t replacement = UploadTexn({activeTrailTexture, trailTextureSize});
    if (!replacement)
        return;
    ReleaseTexture(mapTexture);
    mapTexture = replacement;
}

void PlotTrailPoint(std::int16_t x, std::int16_t y) {
    trailStorage.points[trailStorage.next] = {x, y};
    if (++trailStorage.next >= 1022)
        trailStorage.next = 0;
    if (++trailStorage.count >= 1022)
        trailStorage.count = 1022;
    for (std::uint8_t column = 0; column < 3; ++column) {
        for (std::uint8_t row = 0; row < 3; ++row) {
            const std::uint32_t pixel = static_cast<std::uint16_t>(x + column) + (static_cast<std::uint32_t>(static_cast<std::uint16_t>(y + row - 1)) << 9);
            if (pixel >= trailPixels.size())
                continue;
            if (activeTrailTexture)
                WriteTextureWord(activeTrailTexture, 172 + 4 * pixel, trailColours[0]);
            trailPixels[pixel] = trailColours[0];
        }
    }
}

void RestoreTrailTexture() {
    std::int32_t visited = 0;
    std::int32_t index = trailStorage.next - 1;
    for (std::int32_t iteration = 0; iteration < 1024; ++iteration, --index) {
        if (index < 0)
            index = trailStorage.next - 1;
        if (index >= 1022)
            continue;
        if (visited++ >= trailStorage.count)
            break;
        for (std::uint8_t column = 0; column < 3; ++column) {
            for (std::uint8_t row = 0; row < 3; ++row) {
                const std::uint32_t pixel = static_cast<std::uint16_t>(column + trailStorage.points[index].x) +
                    (static_cast<std::uint32_t>(static_cast<std::uint16_t>(trailStorage.points[index].y + row + 1)) << 9);
                if (pixel >= trailPixels.size())
                    continue;
                WriteTextureWord(activeTrailTexture, 172 + 4 * pixel, trailColours[0]);
                trailPixels[pixel] = trailColours[0];
            }
        }
    }
    UploadTrailTexture();
}

void StampTrailTexture(std::int32_t x, std::int32_t y, std::uint8_t colour, std::int32_t pattern) {
    const std::uint8_t startX = x >= 3 ? static_cast<std::uint8_t>(x - 3) : 0;
    const std::uint8_t startY = y >= 3 ? static_cast<std::uint8_t>(y - 3) : 0;
    for (std::uint32_t row = 0; row < 8; ++row) {
        for (std::uint32_t column = 0; column < 8; ++column) {
            if (!(stampBits[pattern][row] & (1U << column)))
                continue;
            const std::uint32_t pixel = column + ((row + startY) << 9) + startX;
            if (activeTrailTexture)
                WriteTextureWord(activeTrailTexture, 172 + 4 * pixel, trailColours[colour & 0xf]);
            trailPixels[pixel] = trailColours[colour & 0xf];
        }
    }
    UploadTrailTexture();
}

bool LoadNaviFloorImages(const NaviFloorDefinition &definition) {
    for (std::uint32_t &texture : floorTextures)
        ReleaseTexture(texture);
    std::size_t size = 0;
    void *const allocation = LoadSpriteAllocation(definition.filename, size);
    if (!allocation)
        return false;
    const std::byte *cursor = static_cast<const std::byte *>(allocation);
    const std::byte *const end = cursor + size;
    std::size_t index = 0;
    while (cursor + 8 <= end && index < floorTextures.size()) {
        if (!ReadTextureWord(cursor))
            break;
        const std::uint32_t chunkSize = ReadTextureWord(cursor + 4);
        if (chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        floorTextures[index++] = UploadTexn({cursor, chunkSize});
        cursor += chunkSize;
    }
    HLib::FreeTaskData(allocation);
    for (index = 0; index < floorTextures.size(); ++index) {
        char image[128];
        std::snprintf(image, sizeof(image), "/maps/%s_%u.png?usage=0", definition.filename, static_cast<unsigned>(index));
        OverrideMapTexture(floorTextures[index], image);
    }
    return true;
}

void LoadNaviBackground() {
    std::size_t size = 0;
    void *const allocation = LoadSpriteAllocation("NVBACK.SPR", size);
    if (!allocation)
        return;
    backgroundTexture = UploadTexn({static_cast<const std::byte *>(allocation), size});
    HLib::FreeTaskData(allocation);
    OverrideMapTexture(backgroundTexture, "/maps/NVBACK_0.png?usage=0");
}

bool ConstructNaviMap(std::uint32_t configIndex) {
    if (configIndex >= std::size(mapConfigs))
        return false;
    const NaviMapConfig &config = mapConfigs[configIndex];
    bool ownsAllocation = true;
    void *allocation = nullptr;
    std::size_t size = 0;
    if (config.spriteStem) {
        displayOffsetX = 10.0f;
        displayOffsetY = 7.0f;
        char filename[32];
        std::snprintf(filename, sizeof(filename), "%sNAV.SPR", config.spriteStem);
        allocation = LoadSpriteAllocation(filename, size);
    } else {
        LoadNaviBackground();
        if (retainedTrailMap.name) {
            for (const NaviAreaConfig &transfer : trailTransfers) {
                if (transfer.map.name != retainedTrailMap.name || static_cast<std::uint32_t>(transfer.configIndex) != g_currentMap.name)
                    continue;
                allocation = retainedTrailTexture.data();
                ownsAllocation = false;
                trailPixels.fill(0);
                break;
            }
        }
        size = trailTextureSize;
        if (!allocation) {
            allocation = HLib::AllocateTaskData(size, SHEN_NAME(NAVI));
            if (allocation) {
                InitializeTrailTexture(static_cast<std::byte *>(allocation));
                std::memcpy(retainedTrailTexture.data(), allocation, size);
            }
        }
        naviFlags |= 2;
        displayOffsetX = 0.0f;
        displayOffsetY = -20.0f;
        retainedTrailMap = g_currentMap;
    }
    if (!allocation)
        return false;
    mapTexture = UploadTexn({static_cast<const std::byte *>(allocation), size});
    if (config.spriteStem) {
        char image[128];
        std::snprintf(image, sizeof(image), "/maps/%s_0.png?usage=0", config.spriteStem);
        OverrideMapTexture(mapTexture, image);
    }
    if (mapTexture) {
        activeTrailTexture = retainedTrailTexture.data();
        if (config.floorDefinition >= 0) {
            const NaviFloorDefinition &definition = floorDefinitions[config.floorDefinition];
            if (LoadNaviFloorImages(definition)) {
                naviFlags |= 0x10;
                NaviTaskData *const data = static_cast<NaviTaskData *>(HLib::GetTaskParameter(naviTask));
                data->definition = &definition;
            }
        }
    }
    if (ownsAllocation)
        HLib::FreeTaskData(allocation);
    return true;
}
bool NaviInputCancels(const LegacyInputSlot &input) {
    return (input.held & 0xf0c0) || input.analog0d > 5 || input.analog0c > 5 || std::abs(static_cast<std::int32_t>(input.analog0f) - 128) > 5 ||
        std::abs(static_cast<std::int32_t>(input.analog0e) - 128) > 5;
}

void DrawNaviGlyph(std::uint16_t icon, std::int32_t x, std::int32_t y, float aspect) {
    CommonIconAtlas *const atlas = GetCommonIconAtlas(icon);
    if (!atlas)
        return;
    DrawNaviQuad(MakeNaviQuad(x, y, 16, 16, aspect), atlas->textureHandle, 1.0f / 103.0f, NaviIconUv(icon), 0xffffffff, 0x003f3f3f);
}

void DrawNaviFloorNumber(std::int32_t floor, float aspect) {
    if (floor == -1000)
        return;
    if (g_currentMap == AREA_QA11) {
        DrawNaviGlyph(1047, 199, 463, aspect);
        return;
    }
    const std::int32_t displayed = floor > 0 ? floor : floor - 1;
    const std::uint32_t number = static_cast<std::uint32_t>(displayed < 0 ? -static_cast<std::int64_t>(displayed) : displayed);
    const std::int32_t right = displayed >= 10 ? 215 : 199;
    std::uint32_t remaining = number;
    std::uint32_t divisor = 10000000;
    bool started = false;
    for (std::int32_t digit = 0; digit < 8; ++digit, divisor /= 10) {
        const std::uint32_t value = remaining / divisor;
        remaining %= divisor;
        started |= value != 0;
        if (started || digit == 7)
            DrawNaviGlyph(static_cast<std::uint16_t>(1030 + value), right - (7 - digit) * 16, 463, aspect);
    }
    DrawNaviGlyph(1049, displayed >= 10 ? 231 : 215, 463, aspect);
    if (displayed <= 0)
        DrawNaviGlyph(1043, number < 10 ? 183 : 167, 463, aspect);
}

void DrawNaviFloors(const NaviDrawState &draw, float aspect) {
    const NaviTaskData &task = *draw.task;
    const NaviFloorDefinition *const definition = task.definition;
    if (!(draw.flags & 0x10) || !definition || task.stream < -9)
        return;
    const std::uint32_t floorBit = static_cast<std::uint32_t>(task.stream + 9);
    if (floorBit >= 64)
        return;
    for (std::int32_t index = 0; index < definition->imageCount; ++index) {
        const NaviFloorImage &image = definition->images[index];
        if (!(image.floorMask & (std::uint64_t{1} << floorBit)) || image.textureIndex < 0 ||
            static_cast<std::size_t>(image.textureIndex) >= floorTextures.size())
            continue;
        const float scale = task.config->scale;
        const float x = image.x * scale + draw.offsetX;
        const float z = image.z * scale + draw.offsetZ;
        const float halfWidth = image.width * 0.5f * scale;
        NaviQuad quad{};
        for (std::size_t corner = 0; corner < quad.size(); ++corner) {
            quad[corner] = TransformNaviPoint(draw.transform, x + mapCorners[corner].x * halfWidth, z + mapCorners[corner].y * halfWidth);
            quad[corner].x *= aspect;
        }
        DrawNaviQuad(quad, floorTextures[image.textureIndex], 1.0f - ComputeNaviAlphaLoss(0.70000005f + index * 0.01f), {0.0f, 0.0f, 1.0f, 1.0f}, 0xa0ffffff);
        if (!task.trigger && floorTextures[image.textureIndex])
            (void)RequestNaviDrawUpdate();
    }
}

bool NaviMarkerInside(const NaviTaskData &task, const Vector2 &point) {
    return task.trigger || (point.x > 32.0f * (3 - task.clipColumns) && point.y > 32.0f * (12 - task.clipRows) && point.x < 176.0f && point.y < 464.0f);
}

void DrawNaviMarkers(const NaviDrawState &draw, float aspect) {
    const NaviTaskData &task = *draw.task;
    const float zoom = 1.0f + 2.0f * (task.trigger / 255.0f * 0.60000002f);
    const float unitsPerCoordinate = task.config->scale * 128.0f / 256.0f;
    const std::int32_t floor = std::min(
        (task.stream == -1000 ? 63 : task.stream) + 8, 63);
    float layer = 0.60000002f;
    bool visible = false;
    for (std::uint32_t index = 0; index < markers.size(); ++index) {
        const std::uint32_t marker = markers[(nextMarker + index) & 0x7f];
        if (!(marker & 1) || !(marker & 0xe) || static_cast<std::int32_t>((marker >> 10) & 63) != floor)
            continue;
        const float x = static_cast<std::int8_t>(marker >> 16) * unitsPerCoordinate + draw.offsetX;
        const float z = static_cast<std::int8_t>(marker >> 24) * unitsPerCoordinate + draw.offsetZ;
        const Vector2 point = TransformNaviPoint(draw.transform, x, z);
        if (!NaviMarkerInside(task, point))
            continue;
        visible = true;
        const std::uint32_t iconIndex = marker & 8 ? 5 : ((marker >> 1) & 3) + 1;
        const NaviIcon &icon = naviIcons[iconIndex];
        layer += 0.00078125001f;
        if (icon.atlas)
            DrawNaviQuad(MakeNaviQuad(static_cast<std::int32_t>(point.x), static_cast<std::int32_t>(point.y), static_cast<std::int32_t>(zoom * 12.0f),
                             static_cast<std::int32_t>(zoom * 12.0f), aspect),
                icon.atlas->textureHandle, 1.0f - ComputeNaviAlphaLoss(layer), icon.uv);
    }
    const shen_name name = FindNavigationPartyCompanion({});
    if (Character *const character = FindCharacterByScenarioName(name)) {
        const Vector3 position = GetCharacterWorldPosition(character);
        const Vector2 point =
            TransformNaviPoint(draw.transform, position.x - task.config->originX + draw.offsetX, position.z - task.config->originZ + draw.offsetZ);
        if (NaviMarkerInside(task, point)) {
            visible = true;
            if (naviIcons[1].atlas)
                DrawNaviQuad(MakeNaviQuad(static_cast<std::int32_t>(point.x), static_cast<std::int32_t>(point.y), static_cast<std::int32_t>(zoom * 10.0f),
                                 static_cast<std::int32_t>(zoom * 10.0f), aspect),
                    naviIcons[1].atlas->textureHandle, 1.0f - ComputeNaviAlphaLoss(0.5f), naviIcons[1].uv);
        }
    }


    if (!task.trigger) {
        if (visible)
            (void)RequestNaviDrawUpdate();
        (void)RequestNaviScissorUpdate();
        SetLegacyStripScissor(false, 0, 0, 0, 0);
        SetLegacyNavigationRenderMode(0);
    }
    if (naviIcons[1].atlas && task.animation <= 21)
        DrawNaviQuad(playerQuad, naviIcons[1].atlas->textureHandle, 1.0f - ComputeNaviAlphaLoss(0.5f), naviIcons[1].uv);
}

} // namespace

bool DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth) {
    const float u0 = packet.uv[(packet.quad.flags & 0x100U) ? 2 : 0];
    const float u1 = packet.uv[(packet.quad.flags & 0x100U) ? 0 : 2];
    const float v0 = packet.uv[(packet.quad.flags & 0x200U) ? 1 : 3];
    const float v1 = packet.uv[(packet.quad.flags & 0x200U) ? 3 : 1];
    std::array<std::uint32_t, 3> diffuse{}, specular{};
    std::memcpy(diffuse.data(), packet.reservedColour.data(), sizeof(diffuse));
    std::memcpy(specular.data(), packet.reservedUv.data(), sizeof(specular));
    std::array<LegacyStripType3Vertex, 4> vertices{};
    for (std::size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = {{packet.quad.corners[i * 2], packet.quad.corners[i * 2 + 1], depth}, (i & 1U) ? u1 : u0, (i & 2U) ? v1 : v0,
            i && (packet.quad.flags & 8U) ? diffuse[i - 1] : packet.colour, i && (packet.quad.flags & 0x20U) ? specular[i - 1] : packet.secondaryColour};
    }
    std::int32_t previousSource{}, previousDestination{};
    SetLegacyFogMode(2);
    if (packet.quad.flags & 0x800U) {
        constexpr std::array<std::array<std::int32_t, 2>, 3> blendModes{{{8, 6}, {8, 10}, {0, 0}}};
        GetLegacyStripBlendModes(&previousSource, &previousDestination);
        const auto &blend = blendModes[packet.reservedTail];
        SetLegacyStripBlendModes(blend[0], blend[1]);
    }
    const bool drawn =
        (packet.quad.flags & 0x400U) ? DrawLegacyStripType3AlphaTestImmediate(vertices, texture) : DrawLegacyStripType3AlphaBlendImmediate(vertices, texture);
    if (packet.quad.flags & 0x800U)
        SetLegacyStripBlendModes(previousSource, previousDestination);
    SetLegacyFogMode(0);
    return drawn;
}

bool SetDrawWithAlpha(const LegacySpritePacket &packet, std::int32_t textureIndex, float depth) {
    return DrawLegacySpritePacket(packet, ResolveNamedTextureHandle(textureIndex), depth);
}

bool naviDraw(const LegacySpritePacket &packet, std::int32_t textureIndex, float depth) {
    if (!(packet.quad.flags & 0x1000U))
        depth = depth < 0.0f ? std::fmax(1.0f, -depth) : 1.0f / (depth + 100.0f);
    return SetDrawWithAlpha(packet, textureIndex, depth);
}

namespace {
void DrawNaviFrameCallback(void *data) {
    DrawNaviFrame(reinterpret_cast<std::uintptr_t>(data) != 0);
}
} // namespace

// @verified - p
float ComputeNaviAlphaLoss(float order) {
    if (order >= 0.0f)
        return 1.0f / (order + 100.0f);
    return std::fmax(1.0f, -order);
}

void DrawNaviFrame(bool showFloorLabel) {
    const NaviDrawState &draw = naviDrawState;
    if (!naviTask || !draw.task || !mapTexture)
        return;
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    float width{};
    float height{};
    if (!context || !d3t::GetSceneFrameTargetSize(&width, &height))
        return;
    const float safeArea = GetS2UserInterfaceSafeArea();
    const float viewportHeight = safeArea * height;
    const bool scaleAspect = ShouldScaleS2AspectRatio();
    const float viewportWidth = scaleAspect ? viewportHeight * 4.0f / 3.0f : safeArea * width;
    const float aspect = scaleAspect ? 1.0f : GetS2AspectRatioAdjustment();
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const auto previousViewport = state.viewport;
    state.viewport = {(width - viewportWidth) * 0.5f, (height - viewportHeight) * 0.5f, viewportWidth, viewportHeight, 0.0f, 1.0f};
    state.viewportDirtyMask |= 1;
    const bool batching = IsStripBatchingEnabled();
    SetStripBatchingEnabled(0);
    SetLegacyStripDepthTest(false);
    if ((naviFlags & 2) && backgroundTexture)
        DrawNaviQuad(backgroundQuad, backgroundTexture, 1.0f / 100.9f, {0.0f, 0.0f, 1.0f, 1.0f}, 0x81000000);
    const NaviTaskData &task = *draw.task;
    if (showFloorLabel)
        DrawNaviFloorNumber(task.stream, aspect);
    if (!task.trigger) {
        const float clipWidth = viewportHeight * 4.0f / 3.0f;
        const float x = (width - (scaleAspect ? clipWidth : safeArea * width)) * 0.5f;
        const float y = (height - viewportHeight) * 0.5f;
        SetLegacyStripScissor(true, static_cast<std::int32_t>((32.0f * (3 - task.clipColumns) + displayOffsetX) / 640.0f * clipWidth + x),
            static_cast<std::int32_t>((32.0f * (12 - task.clipRows) + displayOffsetY) / 480.0f * viewportHeight + y),
            static_cast<std::int32_t>((displayOffsetX + 176.0f) / 640.0f * clipWidth + x),
            static_cast<std::int32_t>((displayOffsetY + 464.0f) / 480.0f * viewportHeight + y));
        SetLegacyNavigationRenderMode(2);
        (void)RequestNaviDrawUpdate();
        (void)RequestNaviScissorUpdate();
    }
    DrawNaviQuad(mapQuad, mapTexture, 1.0f - ComputeNaviAlphaLoss(0.80000001f), {0.0f, 0.0f, 1.0f, 1.0f}, 0xa0ffffff);
    DrawNaviFloors(draw, aspect);
    DrawNaviMarkers(draw, aspect);
    SetStripBatchingEnabled(batching);
    SetLegacyStripDepthTest(true);
    SetLegacyStripScissor(false, 0, 0, 0, 0);
    state.viewport = previousViewport;
    state.viewportDirtyMask |= 1;
}

void QueueNaviFrameDraw(bool showFloorLabel) {
    if (!naviDrawState.task || !mapTexture)
        return;
    SetUpdateCallback(DrawNaviFrameCallback, reinterpret_cast<void *>(static_cast<std::uintptr_t>(showFloorLabel)), 900.0f);
}

void PrepareNaviFrame(void *taskData) {
    if (!taskData)
        return;
    NaviTaskData &task = *static_cast<NaviTaskData *>(taskData);
    if (!task.config || !g_cameraControl) {
        naviDrawState = {};
        return;
    }
    triggerBlend += (task.trigger / 255.0f - triggerBlend) * 0.60000002f;
    const float x = displayOffsetX + 96.0f + (320.0f - (displayOffsetX + 96.0f)) * triggerBlend;
    const float y = displayOffsetY + 384.0f + (240.0f - (displayOffsetY + 384.0f)) * triggerBlend;
    const float zoom = 1.0f + 2.0f * triggerBlend;
    task.stream = static_cast<std::int32_t>(GetQronNextStream());
    Vector3 position{};
    plymUpdatePlayerSearchLocation(&position);
    const float offsetX = task.config->originX - position.x;
    const float offsetZ = task.config->originZ - position.z;
    std::uint16_t rotationX = 12743;
    std::int16_t rows = 3;
    float scale = 1.25f;
    if (naviFlags & 4) {
        rotationX = 0;
        rows = 4;
        scale = 2.25f;
    }
    if (naviFlags & 2) {
        rotationX = 0;
        scale = 256.0f / (task.config->scale * 64.0f);
        const float texelScale = 512.0f / (task.config->scale * 128.0f);
        const std::int32_t tx = static_cast<std::int32_t>((position.x - task.config->originX) * texelScale + 256.0f) & 511;
        const std::int32_t ty = static_cast<std::int32_t>(-(position.z - task.config->originZ) * texelScale + 256.0f) & 511;
        if (naviFlags & 8) {
            StampTrailTexture(tx, ty, trailStamp, static_cast<std::uint8_t>(trailStamp) - 2);
            naviFlags &= ~8U;
        }
        const float dx = task.position.x - position.x;
        const float dz = task.position.z - position.z;
        const float threshold = navGetNumEntriesFor(g_currentMap) == 16 ? 2.5f : 16.0f;
        if (std::sqrt(dx * dx + dz * dz) > threshold) {
            task.position = position;
            PlotTrailPoint(static_cast<std::int16_t>(tx), static_cast<std::int16_t>(ty));
            UploadTrailTexture();
        }
    }
    const float aspect = ShouldScaleS2AspectRatio() ? 1.0f : GetS2AspectRatioAdjustment();
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(x, 0.0f, y);
    Matrix4x4::RotateAroundX_Wrapper(rotationX);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-g_cameraControl->nextRotation.yaw));
    Matrix4x4::Scale(scale * zoom, scale * zoom, scale * zoom);
    Matrix4x4 transform{};
    Matrix4x4::GetTransformMatrix(&transform);
    for (std::size_t corner = 0; corner < mapQuad.size(); ++corner) {
        mapQuad[corner] = TransformNaviPoint(
            transform, mapCorners[corner].x * 64.0f * task.config->scale + offsetX, mapCorners[corner].y * 64.0f * task.config->scale + offsetZ);
        mapQuad[corner].x *= aspect;
    }
    Matrix4x4::Pop(1);
    playerQuad = MakeNaviQuad(
        static_cast<std::int32_t>(x), static_cast<std::int32_t>(y), static_cast<std::int32_t>(zoom * 16.0f), static_cast<std::int32_t>(zoom * 16.0f), aspect);
    if ((naviFlags & 2) && backgroundTexture)
        backgroundQuad = MakeNaviQuad(static_cast<std::int32_t>(x), static_cast<std::int32_t>(y), static_cast<std::int32_t>(1.5600001f * 128.0f * zoom),
            static_cast<std::int32_t>(1.3f * 192.0f * zoom), aspect);
    task.clipColumns = 3;
    task.clipRows = rows;
    naviDrawState = {&task, transform, offsetX, offsetZ, naviFlags};
    if (NaviIsDisplayable(false))
        QueueNaviFrameDraw(NaviIsDisplayable(true));
}

void UpdateNaviInteraction(const LegacyInputSlot &input) {
    constexpr shen_name navigation = SHEN_NAME(BTNV);
    constexpr shen_name marking = SHEN_NAME(BTMK);
    switch (naviPhase) {
    case 0:
        markerButtons = 0;
        if (btnCheck(navigation))
            ++naviPhase;
        else
            RegisterButtonMediation(navigation, nullptr, ButtonMediationBindY);
        break;
    case 1:
        if (!(input.held & 0x200))
            naviPhase = 2;
        else if (NaviInputCancels(input))
            naviPhase = 10;
        break;
    case 2:
        if (NaviInputCancels(input)) {
            naviPhase = 10;
            break;
        }
        markerButtons = btnCheck(marking);
        if (markerButtons) {
            ++naviPhase;
            if ((markerButtons & 0x406) && (input.held & 0x406) == 0x406)
                naviPhase = 9;
        }
        RegisterButtonMediation(
            marking, nullptr, (ButtonMediationShowActionHint | ButtonMediationBindX | ButtonMediationBindY | ButtonMediationBindA | ButtonMediationBindB));
        break;
    case 3:
        naviPhase = (input.held & 0x406) == 0x406 ? 9 : 4;
        RegisterButtonMediation(
            marking, nullptr, (ButtonMediationShowActionHint | ButtonMediationBindX | ButtonMediationBindY | ButtonMediationBindA | ButtonMediationBindB));
        break;
    case 4:
    case 5: {
        std::uint32_t type = (markerButtons >> 9) & 2;
        if (markerButtons & 4)
            type = 3;
        if (markerButtons & 2)
            type = 4;
        if (type) {
            Vector3 position{};
            plymUpdatePlayerSearchLocation(&position);
            AddNaviMarker(position, static_cast<std::int32_t>(type));
        }
        naviPhase = 10;
        break;
    }
    case 8:
        RemoveLatestNaviMarker();
        naviPhase = 10;
        break;
    case 9:
        RemoveNaviMapMarkers();
        [[fallthrough]];
    case 10:
        if (!(input.held & 0x200))
            naviPhase = 0;
        break;
    default:
        naviPhase = 0;
        break;
    }
}
void InitializeNaviInputState(NaviTaskData &task) {
    plymUpdatePlayerSearchLocation(&task.position);
    naviPhase = 0;
    triggerBlend = 0.0f;
    task.stream = static_cast<std::int32_t>(GetQronNextStream());
}

void UpdateNaviInputAnimation(NaviTaskData &task, const LegacyInputSlot &input) {
    task.trigger = 0;
    if (input.held & 0x200)
        task.trigger = AreS2TriggersSwapped() ? input.analog0d : input.analog0c;
    if (++task.animation >= 28)
        task.animation = 0;
}

bool InitializeNaviResources(HLib::Task *task, std::int32_t configIndex) {
    // The previous map's task-owned resources are retired before the module
    // switch. This initializer only overwrites handles from the freed use space.
    naviTask = nullptr;
    naviDrawState = {};
    activeTrailTexture = nullptr;
    mapTexture = 0;
    backgroundTexture = 0;
    floorTextures.fill(0);
    ResetNaviIcons();
    naviTask = task;
    auto *const data = static_cast<NaviTaskData *>(HLib::GetTaskParameter(task));
    bool loaded = false;
    if (data && configIndex >= 0 && static_cast<std::size_t>(configIndex) < std::size(mapConfigs)) {
        data->config = &mapConfigs[configIndex];
        data->reserved08 = 0;
        data->definition = nullptr;
        data->trigger = 0;
        data->animation = 0;
        naviFlags = 0x20;
        loaded = ConstructNaviMap(static_cast<std::uint32_t>(configIndex));
        if (!loaded) {
            ReleaseTexture(mapTexture);
            ReleaseTexture(backgroundTexture);
            naviFlags &= ~0x20U;
        }
        const std::uint32_t map = g_currentMap.name;
        if ((map & 0xff) == 'Q' && (static_cast<std::uint8_t>((map >> 8) - 'A') <= 5 || map == SHEN_NAME(QJBT) || map == SHEN_NAME(QLBT)))
            naviFlags |= 4;
        if ((naviFlags & 2) && configIndex != 16 && activeTrailTexture)
            RestoreTrailTexture();
    }
    SelectNaviMapMarkers(g_currentMap);
    return loaded;
}

std::span<const NaviAreaConfig> GetNaviAreaConfigs() noexcept {
    return areaConfigs;
}
std::span<const NaviMapConfig> GetNaviMapConfigs() noexcept {
    return mapConfigs;
}
std::span<const NaviFloorDefinition> GetNaviFloorDefinitions() noexcept {
    return floorDefinitions;
}

bool IsNaviActive() noexcept {
    return static_cast<std::uint32_t>(naviPhase) - 2 <= 3;
}

bool IsNaviTaskEnabled() noexcept {
    return naviTask && HLib::GetTaskParameter(naviTask) && (naviFlags & 0x20) != 0;
}

void SetNaviTaskEnabled(bool enabled) {
    if (!naviTask)
        return;
    if (enabled) {
        HLib::ResolveDeferredCallback(naviTask);
        naviFlags |= 0x20;
    } else {
        HLib::SetupTimedTask(naviTask, 0);
        naviFlags &= ~0x20U;
    }
}

bool IsNaviDisplayOverridden() noexcept {
    return (naviFlags & 1) != 0;
}

bool SetNaviDisplayOverride(bool enabled) noexcept {
    const bool previous = (naviFlags & 1) != 0;
    if (enabled)
        naviFlags |= 1;
    else
        naviFlags &= ~1U;
    return previous;
}

void ResetNaviInputPhase() noexcept {
    naviPhase = 0;
}

void ReleaseNaviResources() {
    naviTask = nullptr;
    naviDrawState = {};
    naviPhase = 0;
    naviFlags = 0;
    activeTrailTexture = nullptr;
    ReleaseTexture(mapTexture);
    ReleaseTexture(backgroundTexture);
    for (std::uint32_t &texture : floorTextures)
        ReleaseTexture(texture);
    naviIcons = {};
}

std::int32_t navGetNumEntriesFor(shen_name map) noexcept {
    for (const NaviAreaConfig &area : areaConfigs) {
        if (!area.map.name)
            break;
        if (area.map.name == map.name)
            return area.configIndex;
    }
    return -1;
}

void ResetNaviSavedData() noexcept {
    markers.fill(0);
    nextMarker = 0;
    markerSaveSource = nullptr;
    trailStorage = {};
    trailSaveSource = nullptr;
    retainedTrailMap = {};
}

std::uint32_t LoadNaviMarkers(const std::uint32_t *source) noexcept {
    markerSaveSource = source;
    if (!source)
        return 0;
    nextMarker = 0;
    std::memcpy(markers.data(), source, sizeof(markers));
    return CompactMarkers();
}

std::uint32_t SaveNaviMarkers(std::uint32_t *destination) noexcept {
    const std::uint32_t count = CompactMarkers();
    std::memcpy(destination, markers.data(), sizeof(markers));
    return count;
}

std::int32_t LoadNaviTrail(const NaviTrailStorage *source) noexcept {
    trailSaveSource = source;
    if (!source)
        return 0;
    std::memcpy(&trailStorage, source, sizeof(trailStorage));
    return trailStorage.count;
}

std::int32_t SaveNaviTrail(NaviTrailStorage *destination) noexcept {
    std::memcpy(destination, &trailStorage, sizeof(trailStorage));
    return trailStorage.count;
}

void SelectNaviMapMarkers(shen_name map) noexcept {
    std::int32_t mapIndex = navGetNumEntriesFor(map);
    for (std::size_t index = 0; index < std::size(areaConfigs); ++index) {
        if (!areaConfigs[index].map.name)
            break;
        if (areaConfigs[index].map.name == map.name) {
            mapIndex = static_cast<std::int32_t>(index);
            break;
        }
    }
    for (std::uint32_t &marker : markers) {
        marker &= ~1U;
        if ((marker & 0xe) && static_cast<std::int32_t>((marker >> 4) & 0x3f) == mapIndex)
            marker |= 1;
    }
}

void RemoveLatestNaviMarker() noexcept {
    std::int32_t last = -1;
    for (std::uint32_t index = 0; index < 128; ++index) {
        const std::uint32_t slot = (nextMarker + index) & 0x7f;
        if ((markers[slot] & 1) && !(markers[slot] & 8))
            last = static_cast<std::int32_t>(slot);
    }
    if (last < 0)
        return;
    markers[last] &= ~0xfU;
    CompactMarkers();
}

void RemoveNaviMapMarkers() noexcept {
    for (std::uint32_t &marker : markers) {
        if ((marker & 1) && !(marker & 8))
            marker &= ~0xfU;
    }
    CompactMarkers();
}

std::int32_t AddNaviMarker(const Vector3 &position, std::int32_t type) {
    if (!naviTask)
        return -1;
    NaviTaskData *const data = static_cast<NaviTaskData *>(HLib::GetTaskParameter(naviTask));
    if (!data)
        return -1;
    const std::int32_t configIndex = navGetNumEntriesFor(g_currentMap);
    if (configIndex < 0)
        return -1;
    std::uint32_t mapIndex = static_cast<std::uint32_t>(configIndex);
    for (std::size_t index = 0; index < std::size(areaConfigs); ++index) {
        if (!areaConfigs[index].map.name)
            break;
        if (areaConfigs[index].map.name == g_currentMap.name) {
            mapIndex = static_cast<std::uint32_t>(index);
            break;
        }
    }
    std::int32_t floor = data->stream == -1000 ? 63 : data->stream;
    floor += 8;
    if (floor < 0)
        floor = 0;
    if (floor > 63)
        floor = 63;
    const std::uint32_t slot = nextMarker;
    if (++nextMarker >= 128)
        nextMarker = 0;
    std::uint32_t markerType = 0;
    switch (type) {
    case 3:
        markerType = 1;
        break;
    case 4:
        markerType = 2;
        break;
    case 6:
        markerType = 4;
        break;
    case 7:
        markerType = 5;
        break;
    case 8:
        markerType = 6;
        break;
    default:
        break;
    }
    const float scale = 256.0f / (data->config->scale * 128.0f);
    const std::uint8_t x = static_cast<std::uint8_t>(static_cast<std::int32_t>((position.x - data->config->originX) * scale));
    const std::uint8_t z = static_cast<std::uint8_t>(static_cast<std::int32_t>((position.z - data->config->originZ) * scale));
    markers[slot] = 1 | (2 * markerType + 2) | ((mapIndex & 0x3f) << 4) | ((static_cast<std::uint32_t>(floor) & 0x3f) << 10) |
        (static_cast<std::uint32_t>(x) << 16) | (static_cast<std::uint32_t>(z) << 24);
    return static_cast<std::int32_t>(nextMarker);
}

std::int32_t PlaceNaviMarker(const Vector3 &position, std::int32_t type) {
    switch (type) {
    case 1:
        return AddNaviMarker(position, 2);
    case 2:
        return AddNaviMarker(position, 3);
    case 3:
        return AddNaviMarker(position, 4);
    case 4:
    case 5:
        return AddNaviMarker(position, 6);
    case 6:
        return AddNaviMarker(position, 7);
    case 7:
        return AddNaviMarker(position, 8);
    default:
        return AddNaviMarker(position, 0);
    }
}

void AddNaviTrailLocations(std::span<const Vector3> locations) {
    if (!naviTask)
        return;
    const NaviTaskData *const data = static_cast<const NaviTaskData *>(HLib::GetTaskParameter(naviTask));
    if (!data || !(naviFlags & 2))
        return;
    for (const Vector3 &location : locations) {
        const float scale = 512.0f / (data->config->scale * 128.0f);
        const std::int32_t x = static_cast<std::int32_t>((location.x - data->config->originX) * scale + 256.0f) & 0x1ff;
        const std::int32_t y = static_cast<std::int32_t>(-(location.z - data->config->originZ) * scale + 256.0f) & 0x1ff;
        PlotTrailPoint(static_cast<std::int16_t>(x), static_cast<std::int16_t>(y));
    }
    UploadTrailTexture();
}

} // namespace shenmue::s2
