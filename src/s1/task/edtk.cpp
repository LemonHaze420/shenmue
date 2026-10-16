#include "s1/task/edtk.hpp"

#include "platform/performance_counter.hpp"
#include "s1/audio_stream.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/render_passes.hpp"
#include "s1/screen_space.hpp"
#include "s1/subtitles.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

HLib::Task *g_pEDTK_Task = nullptr;
std::array<std::int32_t, 3> g_pEDTK_PVRs{};
float g_endingBackdropScrollX = 0.0f;
float g_endingBackdropScrollY = 0.0f;
std::int64_t g_pEDTK_StartTime = 0;
std::int32_t g_edtkSeqIdx = 0;
extern std::int32_t g_busy_reset;

namespace {

constexpr std::int32_t kTaskToken = static_cast<std::int32_t>(SHEN_NAME(EDTK));
constexpr std::int32_t kSpriteToken = static_cast<std::int32_t>(SHEN_NAME(EDMJ));

void LoadSurfaces() {
    constexpr const char *kDirectory = "sprite";
    constexpr const char *kFilename = "ENDMOJI0.SPR";
    const std::uint64_t fileSize64 = shenOpenFile(kDirectory, kFilename);
    if (!fileSize64 || fileSize64 > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)()))
        return;

    void *const file = ReadFile(kDirectory, kFilename, kSpriteToken);
    if (!file)
        return;

    const std::size_t fileSize = static_cast<std::size_t>(fileSize64);
    auto *const bytes = static_cast<std::byte *>(file);
    std::size_t offset = 0;
    std::size_t surfaceIndex = 0;
    while (offset + 8 <= fileSize && surfaceIndex < g_pEDTK_PVRs.size()) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || chunkSize < 8 || chunkSize > fileSize - offset)
            break;

        g_pEDTK_PVRs[surfaceIndex++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}

OriginalStrip MakeStrip(std::int32_t horizontalPosition, std::int32_t verticalPosition, std::int32_t width, std::int32_t height) {
    OriginalStrip strip{};
    strip.flags = 0x400;
    strip.corner0 = 0xffffffffU;
    strip.corner1 = 0xffffffffU;
    strip.corner2 = 0xffffffffU;
    strip.corner3 = 0xffffffffU;
    strip.vertexCount = 0;
    strip.normalizedY1 = 0.0f;
    strip.normalizedX2 = 0.0f;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    strip.normalizedY3 = 0.0f;
    AdjustAspectRatioMatrix(&strip, 6, horizontalPosition, verticalPosition, width, height, 4);
    return strip;
}


void DrawBackdrop() {
    if (!DX11::g_ppD3DDeviceContext)
        return;

    const std::int32_t y = static_cast<std::int32_t>(g_endingBackdropScrollY);
    std::array<OriginalStrip, 3> strips{{
        MakeStrip(static_cast<std::int32_t>(g_endingBackdropScrollX), y, 512, 512),
        MakeStrip(static_cast<std::int32_t>(g_endingBackdropScrollX - 512.0f), y, 512, 512),
        MakeStrip(static_cast<std::int32_t>(g_endingBackdropScrollX - 1024.0f), static_cast<std::int32_t>(g_endingBackdropScrollY - 42.0f), 128, 128),
    }};
    for (std::size_t index = 0; index < strips.size(); ++index)
        SetRenderFlags_UnmapAndDraw_0(&strips[index], g_pEDTK_PVRs[index], 6.0f);
}

void ProcessSequenceCue() {
    if (g_applicationSettings.savedDialogueMode <= 1 || !g_pEDTK_StartTime || static_cast<std::uint32_t>(g_edtkSeqIdx) >= g_edtkSeqData.size())
        return;

    const std::uint64_t counts = static_cast<std::uint64_t>(platform::performanceCounter() - g_pEDTK_StartTime);
    const double elapsed = static_cast<double>(counts) / static_cast<double>(static_cast<std::uint64_t>(platform::performanceCounterFrequency()));
    const float offset = g_usingJapaneseAudio == 1 ? g_runStartSpeed : 0.0f;
    const EdtkSequenceEntry &entry = g_edtkSeqData[static_cast<std::size_t>(g_edtkSeqIdx)];
    if (static_cast<float>(elapsed) < offset + entry.triggerTime)
        return;

    if (entry.subtitleId) {
        SubtitleStringId id{};
        std::memcpy(&id, entry.subtitleId, 8);
        const char *const text = ReadStringFromStringTableById(id);
        const MapControlData *const map = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCNT_ShowSubtitleText(map ? map->scenarioTask : nullptr, text, 3);
    } else {
        constexpr std::size_t kSubtitleUiIndex = 0x3a;
        UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
        if ((g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) &&
            (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1))
            subtitles.isVisible = 0;
    }
    ++g_edtkSeqIdx;
}

} // namespace

void EDTK_Callback(HLib::TaskData *) {
    if (!g_updateRenderFrameMode)
        return;

    DrawBackdrop();
    ProcessSequenceCue();
}

void EnqueueEDTK() {
    if (g_pEDTK_Task)
        return;

    g_pEDTK_Task = HLib_EnqueueTaskWithoutParameter(EDTK_Callback, 6, 0, kTaskToken);
    LoadSurfaces();
    g_pEDTK_StartTime = platform::performanceCounter();
}

void Script_EDTKUtils(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const EdtkUtilityParameters *>(rawParams);
    if (!params)
        return;

    switch (params->mode) {
    case EdtkUtilityMode::ResetState:
        g_endingBackdropScrollX = -256.0f;
        g_endingBackdropScrollY = 0.0f;
        g_pEDTK_Task = nullptr;
        g_pEDTK_StartTime = 0;
        g_edtkSeqIdx = 0;
        break;
    case EdtkUtilityMode::Enqueue:
        EnqueueEDTK();
        break;
    case EdtkUtilityMode::Destroy:
        if (g_pEDTK_Task) {
            for (const std::int32_t pvr : g_pEDTK_PVRs)
                PVR_FreeAllocatedSurfaceByIndex(pvr);
            HLib_DestroyTask(g_pEDTK_Task);
            g_pEDTK_Task = nullptr;
        }
        break;
    case EdtkUtilityMode::SetBackdropPosition:
        g_endingBackdropScrollX = params->width;
        g_endingBackdropScrollY = params->height;
        if (g_updateRenderFrameMode > 0)
            g_endingBackdropScrollX += static_cast<float>(g_updateRenderFrameMode) * 0.1875f;
        break;
    }
}

} // namespace shenmue::s1
