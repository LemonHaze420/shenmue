#include "s2/hiki.hpp"

#include "d3t/object_store.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"

#include <cstring>

namespace shenmue::s2 {
Vector3 g_hikiPosition{};
std::int32_t g_hikiTextureIndex{};
std::uint32_t g_effectDiffuseMask{};
HLib::Task *g_hikiTask{};
HikiModelNode *g_hikiModel{};
void *g_hikiModelImage{};
std::uint8_t *g_hikiAlphaData{};

namespace {
Vector3 hikiLocalPosition{};
std::int32_t hikiYaw{};
std::int32_t hikiYawReserved{};
std::int16_t hikiScrollU{};
std::int16_t hikiScrollV{};
std::int32_t hikiSegmentCount{};
float hikiSegmentSpacing{};

// @verified - p
std::uint32_t GetEffectTextureDimensionCode(std::int32_t dimension) {
    switch (dimension) {
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}

HikiTextureHeader *ParseHikiTexture(std::int32_t index, std::span<const std::byte> image, std::uint32_t flags) {
    const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(image.data());
    const std::uint32_t width = static_cast<std::uint32_t>(static_cast<std::int16_t>(words[6]));
    const std::uint32_t height = static_cast<std::uint32_t>(static_cast<std::int16_t>(words[6] >> 16U));
    HikiTextureRecord &record = g_hikiTextures[index];
    if (InitializeLegacyEffectTexture(record.texture, width, height, static_cast<std::uint16_t>(words[5])) != 0 ||
        UploadLegacyEffectPvr(record.texture, image) != 0)
        return nullptr;
    const std::uint32_t dimensionBits = (GetEffectTextureDimensionCode(static_cast<std::int32_t>(record.texture.width)) << 3U) |
        GetEffectTextureDimensionCode(static_cast<std::int32_t>(record.texture.height));
    record.flags = flags;
    record.header.words[3] = static_cast<std::uint32_t>(record.texture.handle);
    if (flags & 1U) {
        record.header.words[0] = 0x808c000eU;
        record.header.words[1] = 0xca800000U;
        record.header.words[2] = dimensionBits | 0x20002440U;
    } else {
        record.header.words[0] = 0x8200000eU;
        record.header.words[1] = 0xcb800000U;
        record.header.words[2] = dimensionBits | 0x94002440U;
    }
    if (flags & 2U)
        record.header.words[2] &= 0xff3fffffU;
    else
        record.header.words[2] |= 0x00800000U;
    if (flags & 4U)
        record.header.words[2] |= 0x001000c0U;
    else
        record.header.words[2] = (record.header.words[2] & ~0x00100000U) | 0x40U;
    return &record.header;
}
} // namespace

// @verified - p
void ResetHikiTextures() {
    g_effectDiffuseMask = 0xffffffffU;
    for (HikiTextureRecord &record : g_hikiTextures)
        record.texture.handle = 0;
}

// @verified - p
std::int64_t ResetHikiTaskData() {
    g_hikiTextureIndex = -1;
    g_hikiAlphaData = nullptr;
    g_hikiModelImage = nullptr;
    g_hikiModel = nullptr;
    g_hikiTask = nullptr;
    return 0;
}

// @verified - p
void AssignHikiPosition(const Vector3 &position) {
    g_hikiPosition = position;
}

// @verified - p
void AssignHikiLocalPosition(const Vector3 &position) {
    hikiLocalPosition = position;
}

// @verified - p
void AssignHikiRotation(std::int32_t yaw) {
    hikiYaw = yaw;
}

// @verified - p
void AssignHikiTextureScroll(std::int16_t u, std::int16_t v) {
    hikiScrollU = u;
    hikiScrollV = v;
}

void HIKI_Enqueue() {
    if (g_hikiTextureIndex >= 0)
        return;
    g_hikiTextureIndex = 0;
    void *image = nullptr;
    std::uint32_t size = 0;
    if (!(image = LoadLegacySceneFile("MISC", "SIRANAMI.PVR", TASK_HIKI)))
        return;
    size = static_cast<std::uint32_t>(HLib::GetTaskDataSize(image));
    ParseHikiTexture(g_hikiTextureIndex, {static_cast<const std::byte *>(image), size}, 6);
    HLib::FreeTaskData(image);
    image = nullptr;
    if (!(image = LoadLegacySceneFile("MISC", "TRSEA4.MT6", TASK_HIKI)))
        return;
    g_hikiModelImage = image;
    const std::uint32_t *const words = static_cast<const std::uint32_t *>(image);
    g_hikiModel = reinterpret_cast<HikiModelNode *>(static_cast<std::byte *>(image) + words[2]);
    if (!g_hikiModel)
        return;
    d3t::RegisterObjectSource(g_hikiModel, HIKI_MODEL_PATH, HIKI_MODEL_FLAGS);
    RelocateHikiModel(*g_hikiModel, image);
    hikiScrollU = 0;
    hikiScrollV = -6;
    HikiModelEntity *const entity =
        g_hikiModel->entityOffset ? static_cast<HikiModelEntity *>(HLib::GetPointerFromDynamicHeapOffsetBase(g_hikiModel->entityOffset)) : nullptr;
    InitializeHikiTextureCoordinates(4, 4, *entity);
    g_hikiPosition = {};
    hikiYaw = 0;
    hikiYawReserved = 0;
    hikiLocalPosition = {.x = 0, .y = 0, .z = 12.0f};
    hikiSegmentSpacing = 64.0f;
    const bool small = (g_currentMap.name & 0xffU) == 'K';
    hikiSegmentCount = small ? 1 : 2;
    if ((image = LoadLegacySceneFile("MISC", small ? "ALPHTBL1.BIN" : "ALPHTBL2.BIN", TASK_HIKI))) {
        g_hikiAlphaData = static_cast<std::uint8_t *>(image);
        g_hikiTask = HLib::EnqueueTaskWithoutParameter(Hiki_Callback, 13, 8, static_cast<std::int32_t>(TASK_HIKI.name));
    }
}

// @verified - p
void Hiki_Callback(void *) {
    if (static_cast<double>(g_hikiOpacity) < 0.0001)
        return;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(g_hikiPosition.x, g_hikiPosition.y + 0.01f, g_hikiPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<std::uint32_t>(hikiYaw) + 32767U));
    if (!d3t::HashModel(g_hikiModel, GetLegacyPerspectiveProjection())) {
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(g_hikiPosition.x, g_hikiPosition.y + 0.01f, g_hikiPosition.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(hikiYaw));
        Matrix4x4::preTranslate(hikiLocalPosition.x, hikiLocalPosition.y, hikiLocalPosition.z);
        for (std::int32_t index = 0; index < hikiSegmentCount + 1; ++index) {
            HikiModelEntity &entity = *static_cast<HikiModelEntity *>(HLib::GetPointerFromDynamicHeapOffsetBase(g_hikiModel->entityOffset));
            if (index) {
                ParseHikiData(std::span<const std::uint8_t, HIKI_ALPHA_GRID_SIZE>{g_hikiAlphaData + HIKI_ALPHA_GRID_SIZE * index, HIKI_ALPHA_GRID_SIZE});
                ApplyHikiTexture(g_hikiTextureIndex, entity);
                g_effectDiffuseMask = HIKI_DIFFUSE_MASK;
            } else {
                InitializeHikiWaveHeights(GetWaveHeights(), 0xffffffffU);
                g_effectDiffuseMask = HIKI_DIFFUSE_MASK;
                ParseHikiData(std::span<const std::uint8_t, HIKI_ALPHA_GRID_SIZE>{g_hikiAlphaData, HIKI_ALPHA_GRID_SIZE});
                const std::int16_t scrollV = static_cast<std::int16_t>(TruncateToInt32(shenGetScaledDeltaTickTime(static_cast<float>(hikiScrollV))));
                const std::int16_t scrollU = static_cast<std::int16_t>(TruncateToInt32(shenGetScaledDeltaTickTime(static_cast<float>(hikiScrollU))));
                ScrollHikiTexture(g_hikiTextureIndex, scrollU, scrollV, entity);
            }
            DrawLegacyEffectEntity(entity);
            Matrix4x4::preTranslate(0, 0, -hikiSegmentSpacing);
        }
    }
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s2
