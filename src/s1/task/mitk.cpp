#include "s1/task/mitk.hpp"
#include "s1/boot.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/render_passes.hpp"
#include "s1/screen_space.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

HLib::Task *g_MITK_Task = nullptr;
std::array<std::int32_t, 18> g_memoryLoadingIconTexHandles{};
std::int16_t g_memoryLoadingIconAlpha = 0;
std::int16_t g_memoryLoadingIconAlphaStep = 0;
std::int8_t g_memoryLoadingIconTimeOfDay = 0;
std::uint8_t g_memoryLoadingIconFadeIn = 0;
extern std::uint8_t g_globalFlags[1];

namespace {

constexpr std::int32_t kTaskToken = static_cast<std::int32_t>(SHEN_NAME(MITK));
constexpr std::int32_t kSpriteToken = SHEN_NAME(SNEF);
constexpr std::size_t kSurfaceCount = 18;
constexpr std::size_t kQuadsPerLayout = 4;

static_assert(sizeof(MemoryQuadLayout) == 0x10);

bool LoadSurfaces() {
    constexpr const char *kDirectory = "sprite";
    constexpr const char *kFilename = "MEMORIES.SPR";
    const std::uint64_t fileSize64 = shenOpenFile(kDirectory, kFilename);
    if (!fileSize64 || fileSize64 > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)())) {
        return false;
    }

    void *const file = ReadFile(kDirectory, kFilename, kSpriteToken);
    if (!file)
        return false;

    const std::size_t fileSize = static_cast<std::size_t>(fileSize64);
    auto *const bytes = static_cast<std::byte *>(file);
    std::size_t offset = 0;
    std::size_t surfaceIndex = 0;
    while (surfaceIndex < kSurfaceCount && offset + 8 <= fileSize) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || chunkSize < 8 || chunkSize > fileSize - offset)
            break;

        g_memoryLoadingIconTexHandles[surfaceIndex++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
    return surfaceIndex != 0;
}

std::size_t LayoutOffset(std::int32_t hour) {
    if (hour == 2 || hour == 3)
        return 4;
    if (hour == 4 || hour == 5)
        return 8;
    if (hour >= 6 && hour <= 8)
        return 12;
    if (hour == 9 || hour == 10)
        return 16;
    return 0;
}

OriginalStrip BuildQuad(const MemoryQuadLayout &layout, std::uint32_t groupFlags, std::uint32_t colour) {
    OriginalStrip strip{};
    strip.flags = layout.flags | groupFlags;
    strip.corner0 = colour;
    strip.corner1 = colour;
    strip.corner2 = colour;
    strip.corner3 = colour;
    strip.normalizedY1 = 0.1f;
    strip.normalizedX2 = 0.1f;
    strip.normalizedY2 = 0.9f;
    strip.normalizedX3 = 0.9f;
    strip.normalizedY3 = 0.0f;

    const auto width = static_cast<std::int32_t>(layout.scaleX * 16.0f);
    AdjustAspectRatioMatrix(&strip, 5, layout.centerX, layout.centerY, width, 1, 3);

    const DisplayModeInfo *const resolution = GetScreenResolution();
    float aspectCorrection = 1.0f;
    if (resolution && resolution->width && resolution->height) {
        const float displayAspect = static_cast<float>(resolution->width) / static_cast<float>(resolution->height);
        aspectCorrection = 1.3333334f / displayAspect;
        if (aspectCorrection == 0.0f)
            aspectCorrection = 1.0f;
    }
    const float halfHeight = 8.0f * layout.scaleY / aspectCorrection;
    strip.y0 = static_cast<float>(layout.centerY) - halfHeight;
    strip.y1 = strip.y0;
    strip.y2 = static_cast<float>(layout.centerY) + halfHeight;
    strip.y3 = strip.y2;
    return strip;
}



void DrawLoadingMemoryIconQuads() {
    if (!g_updateRenderFrameMode)
        return;
    const std::size_t layoutOffset = LayoutOffset(g_memoryLoadingIconTimeOfDay);
    const std::uint32_t groupFlags = static_cast<std::uint32_t>(layoutOffset * 16);
    const std::uint32_t colour = (static_cast<std::uint32_t>(g_memoryLoadingIconAlpha) << 24U) | 0x00ffffffU;
    const auto &layouts =
        g_globalFlags[0] == static_cast<std::uint8_t>('P') || g_globalFlags[0] == static_cast<std::uint8_t>('V') ? kMemoryLayoutsPV : kMemoryLayouts;
    for (std::size_t index = 0; index < kQuadsPerLayout; ++index)
        static_cast<void>(BuildQuad(layouts[layoutOffset + index], groupFlags, colour));
}

} // namespace

void EnqueueMITK(std::int8_t timeOfDay, std::int16_t fadeFrames) {
    if (g_MITK_Task)
        return;

    LoadSurfaces();
    g_MITK_Task = HLib_EnqueueTaskWithoutParameter(MITK_Callback, 6, 0, kTaskToken);
    g_memoryLoadingIconTimeOfDay = timeOfDay;
    if (fadeFrames <= 0)
        fadeFrames = 1;
    g_memoryLoadingIconAlpha = 0;
    g_memoryLoadingIconFadeIn = 1;
    g_memoryLoadingIconAlphaStep = static_cast<std::int16_t>(256 / fadeFrames);
}

void MITK_Callback(HLib::TaskData *) {
    if (g_memoryLoadingIconFadeIn) {
        g_memoryLoadingIconAlpha = static_cast<std::int16_t>(g_memoryLoadingIconAlpha + g_memoryLoadingIconAlphaStep);
        if (g_memoryLoadingIconAlpha > 255)
            g_memoryLoadingIconAlpha = 255;
    } else {
        g_memoryLoadingIconAlpha = static_cast<std::int16_t>(g_memoryLoadingIconAlpha - g_memoryLoadingIconAlphaStep);
        if (g_memoryLoadingIconAlpha < 0)
            g_memoryLoadingIconAlpha = 0;
    }

    DrawLoadingMemoryIconQuads();
    if (g_memoryLoadingIconAlpha)
        return;

    for (const std::int32_t handle : g_memoryLoadingIconTexHandles)
        PVR_FreeAllocatedSurfaceByIndex(handle);
    HLib_DestroyTask(g_MITK_Task);
    g_MITK_Task = nullptr;
}

void MITK_Destroy() {
    if (g_MITK_Task) {
        for (const std::int32_t handle : g_memoryLoadingIconTexHandles)
            PVR_FreeAllocatedSurfaceByIndex(handle);
        HLib_DestroyTask(g_MITK_Task);
        g_MITK_Task = nullptr;
    }
    g_memoryLoadingIconAlpha = 0;
    g_memoryLoadingIconAlphaStep = 0;
    g_memoryLoadingIconTimeOfDay = 0;
    g_memoryLoadingIconFadeIn = 0;
}

void Script_SetMITKFlags(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const MITKParameters *>(rawParams);
    if (!params)
        return;

    if (params->command == MITKCommand::StartFadeIn) {
        EnqueueMITK(params->timeOfDay, params->fadeFrames);
        return;
    }
    if (params->command != MITKCommand::StartFadeOut)
        return;

    const std::int16_t factor = params->fadeFrames > 0 ? params->fadeFrames : 1;
    g_memoryLoadingIconFadeIn = 0;
    g_memoryLoadingIconAlphaStep = static_cast<std::int16_t>(256 / factor);
    g_memoryLoadingIconAlpha = 255;
}

} // namespace shenmue::s1
