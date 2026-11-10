#include "s1/task/tmlt.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/atck.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/ui_runtime.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);

HLib::Task *g_TMLT_Task = nullptr;
std::int16_t g_tmp_tmlt_textures_initialised = 0;
std::array<std::int32_t, 10> g_tmltDigitPVRs{};
std::int32_t g_tmltDotPVR0 = 0;
std::int32_t g_tmltDotPVR1 = 0;
std::int16_t g_BeepIntervalFrames = 0;
std::int16_t g_BeepFrameCounter = 0;
std::uint8_t g_tmlt_Active = 0;
std::int16_t g_FrameCounter30 = 0;
std::int32_t g_curr_time_left = 0;

namespace {

constexpr std::int32_t kSpriteAllocationToken = static_cast<std::int32_t>(SHEN_NAME(LDTD));
constexpr std::uint32_t kTimeLimitWarningCommand = 0x000d00ab;


constexpr std::array<std::int16_t, 11> kBeepIntervalBySeconds{2, 2, 5, 7, 9, 13, 17, 21, 24, 27, 30};

void LoadDigitSurfaces() {
    constexpr const char *kDirectory = "sprite";
    constexpr const char *kFilename = "SWLDNUM.SPR";
    const std::uint64_t fileSize64 = shenOpenFile(kDirectory, kFilename);
    if (!fileSize64 || fileSize64 > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)())) {
        return;
    }

    void *const file = ReadFile(kDirectory, kFilename, kSpriteAllocationToken);
    if (!file)
        return;

    auto *const bytes = static_cast<std::byte *>(file);
    const std::size_t fileSize = static_cast<std::size_t>(fileSize64);
    std::size_t offset = 0;
    std::size_t surfaceIndex = 0;
    while (offset + 8 <= fileSize && surfaceIndex < g_tmltDigitPVRs.size()) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || chunkSize < 8 || chunkSize > fileSize - offset)
            break;

        g_tmltDigitPVRs[surfaceIndex++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}



void TimeLimitHUD_Update() {
    if (!g_updateRenderFrameMode || !g_tmp_tmlt_textures_initialised || !g_pATCK)
        return;

    ATCK_AssignNewDialPreset(1);
    ATCK_RaceHUDWriteNumberToStrip(36, 100 * (g_curr_time_left + 40 * (g_curr_time_left / 60)));
}

} // namespace

void InitTMLT(std::int16_t texturesInitialised) {
    g_tmp_tmlt_textures_initialised = texturesInitialised;
    if (!texturesInitialised) {
        LoadDigitSurfaces();
        g_tmltDotPVR0 = AllocatePVRSurface("TIME", 64, 0xffff, 0xf000, 0, true);
        g_tmltDotPVR1 = AllocatePVRSurface("\xa1\xa7", 64, 0xffff, 0xf000, 1, true);
    }
    g_TMLT_Task = HLib_EnqueueTaskWithoutParameter(TMLT_Callback, 6, 0, static_cast<std::int32_t>(TASK_TMLT.name));
    g_BeepIntervalFrames = 30;
    g_BeepFrameCounter = 30;
    g_tmlt_Active = 0;
    g_FrameCounter30 = 0;
}

void FreeTMLT() {
    if (!g_TMLT_Task)
        return;

    if (!g_tmp_tmlt_textures_initialised) {
        for (const std::int32_t pvr : g_tmltDigitPVRs)
            PVR_FreeAllocatedSurfaceByIndex(pvr);
        PVR_FreeAllocatedSurfaceByIndex(g_tmltDotPVR0);
        PVR_FreeAllocatedSurfaceByIndex(g_tmltDotPVR1);
    }
    HLib_DestroyTask(g_TMLT_Task);
    g_TMLT_Task = nullptr;
    g_tmp_tmlt_textures_initialised = 0;
}

void TMLT_Callback(HLib::TaskData *) {
    if (!g_tmlt_Active)
        return;

    if (++g_FrameCounter30 >= 30) {
        std::int32_t timeLeft = g_curr_time_left - 1;
        g_FrameCounter30 = 0;
        if (timeLeft <= 0)
            timeLeft = 0;
        g_curr_time_left = timeLeft;


        if (timeLeft < static_cast<std::int32_t>(kBeepIntervalBySeconds.size()))
            g_BeepIntervalFrames = kBeepIntervalBySeconds[static_cast<std::size_t>(timeLeft)];
    }

    TimeLimitHUD_Update();

    if (g_curr_time_left <= 10 && ++g_BeepFrameCounter >= g_BeepIntervalFrames) {
        g_BeepFrameCounter = 0;
        if (g_curr_time_left > 0)
            shenExecuteAICACommand(kTimeLimitWarningCommand, 0, 0);
    }
}

} // namespace shenmue::s1
