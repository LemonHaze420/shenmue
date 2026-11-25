#include "s2/areas/area_krh1.hpp"
#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/butterfly.hpp"
extern unsigned int areaFlags;
#else

#include "s2/area_event.hpp"
#include "s2/conversation.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/pak.hpp"
#include "s2/map_assets.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>

namespace shenmue::s2::areas {
namespace {
void SetupKRH1WeatherFlags();
void eKRH1_ET_WAVE_KRH1_Callback(void *);
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
ButterflyRegion krh1ButterflyRegions[] = {
#else
constexpr ButterflyRegion krh1ButterflyRegions[] = {
#endif
    {0, 6, {1362.7525634765625f, 100.43499755859375f, 749.0584106445312f}},
    {0, 4, {1345.2142333984375f, 101.04499816894531f, 753.7396850585938f}},
    {0, 6, {1328.930419921875f, 100.51699829101562f, 754.0189208984375f}},
    {0, 4, {1324.57373046875f, 102.57869720458984f, 762.5322265625f}},
    {0, 6, {1302.1494140625f, 97.1177978515625f, 788.8693237304688f}},
    {0, 4, {1313.42578125f, 100.41600036621094f, 746.5206298828125f}},
};

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
void AreaKRH1Init() {}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
#else
void AreaKRH1Main(std::uint32_t startParameters)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    float distance[64], first[64], second[64];
    void *firstKernel, *secondKernel;
#endif
    if (startParameters != 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '1', '4', '3'), 1);
#else
        SetCharaKill(SHEN_NAME(a143), 1);
#endif
    }

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
#else
    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
#endif
    if ((unsigned char)(dateTime.hour - 8) > 14U) {
        dateTime.hour = 8;
        dateTime.minute = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&dateTime);
#else
        SetCurrentDateTimeTo(dateTime);
#endif
        FTBL.dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }

    (void)FTBL.EV_FreeAuthorPak(1, "0503");
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaCleanup(0);
#else
    SetupKRH1WeatherFlags();
#endif
    FTBL.SetMapModelVertexShaderType(1, 1);
    FTBL.SetMapModelVertexShaderType(2, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    {
        Vector3 lighting = {0.3f, 0.8f, 0.5f};
        FTBL.GenerateFFT(0.5f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(1.1f, FTBL.generate_distance_falloff_row_major(0.3f, distance)));
        firstKernel = FTBL.fft_fill_krn(0.05f, FTBL.generate_distance_falloff_kernel_8x8(0.3f, first));
        secondKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(0.1f, second));
        FTBL.fft_bins_load_pairwise_tables(firstKernel, secondKernel);
        FTBL.FFTUtil(0.0f, -0.002f);
        FTBL.set_lighting_flags(1.0f, &lighting);
        QueueButterflies(krh1ButterflyRegions, 16);
#else
    std::array<std::byte, 0x100> distanceWork, firstWork, secondWork;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(1.1f, GenerateDistanceFalloffRowMajor(0.3f, distanceWork)));
    void *const secondKernel = FillFftKernel(0.06f, GenerateDistanceFalloffKernel8x8(0.1f, secondWork));
    void *const firstKernel = FillFftKernel(0.05f, GenerateDistanceFalloffKernel8x8(0.3f, firstWork));
    ApplyFft(firstKernel, secondKernel);
    FinalizeFft(0.0f, -0.002f);
    SetWaveLighting(1.0f, Vector3{0.3f, 0.8f, 0.5f});
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_MoveButterfly_Callback, 4, 0x0d, 0x410, TASK_BUTTERFLY);
    auto *const work = static_cast<ButterflyTaskData *>(HLib::GetTaskParameter(task));
    for (auto &butterfly : work->motion) {
        butterfly.velocity = {};
        butterfly.filteredVelocity = {};
        butterfly.target = {};
        butterfly.regionIndex = 0;
        butterfly.phase = 0;
        butterfly.phaseStep = 0;
        butterfly.flags = 0;
    }
    work->butterflyCount = 16;
    work->regions = krh1ButterflyRegions;
#endif
        if (FTBL.EV_GetShenmueFlag(0x0d8) != 0) {
            areaFlags |= 2;
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.HLib_EnqueueTaskWithoutParameter(WaveKRH1, 3, 2, SHEN_NAME4('w', 'a', 'v', 'e'));
#else
    FTBL.HLib_EnqueueTaskWithoutParameter(eKRH1_ET_WAVE_KRH1_Callback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    }
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
#else
void SetupKRH1WeatherFlags()
#endif
{
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.EV_SetWeatherInstruction(0x780, 1);
    FTBL.EV_SetWeatherInstruction(0x0f, 1);
    FTBL.EV_SetWeatherInstruction(0x0c, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
#else
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
#endif
    FTBL.AssignLightParams(1, 1);
    FTBL.AssignLightParams(1, 2);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void WaveKRH1(void *taskData)
#else
void eKRH1_ET_WAVE_KRH1_Callback(void *taskData)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    unsigned char *weather = (unsigned char *)FTBL.GetInterpolatedWeatherValues();
    unsigned int *colours = (unsigned int *)(weather + 0x34);
    float r = (unsigned char)(colours[7] >> 16) / 255.0f;
    float g = (unsigned char)(colours[7] >> 8) / 255.0f;
    float b = weather[0x50] / 255.0f;
    float sr = (unsigned char)(colours[5] >> 16) / 255.0f;
    float sg = (unsigned char)(colours[5] >> 8) / 255.0f;
    float sb = weather[0x48] / 255.0f;
    r *= 0.3607f;
    g *= 0.3725f;
    b *= 0.4352f;
    sr *= 0.2549f;
    sg *= 0.38f;
    sb *= 0.2862f;
    {
        float power = 42.54f;
        FTBL.BuildLightingTableFrom(0, 1.0f, 1.0f, r, g, b, sr, sg, sb, power, 1.0f);
    }
#else
    const auto colours = GetWeatherWaveColours();
    const Vector3 diffuse{(static_cast<float>(colours[1] >> 16) / 255.0f) * 0.007214f, (static_cast<float>(colours[1] >> 8) / 255.0f) * 0.00745f,
                          (static_cast<float>(colours[1]) / 255.0f) * 0.02176f};
    const Vector3 specular{(static_cast<float>(colours[0] >> 16) / 255.0f) * 0.2549f, (static_cast<float>(colours[0] >> 8) / 255.0f) * 0.38f,
                           (static_cast<float>(colours[0]) / 255.0f) * 0.2862f};
    BuildWaveLightingTable(0, 1.0f, 1.0f, diffuse, specular, 42.54f, 1.0f);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(1));
    if (FTBL.EV_GetPlayerFlag(11, 1657) != 0) {
        FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(2));
        FTBL.SetObjectFlagByIndex(2, 1);
    } else {
        FTBL.SetObjectFlagByIndex(2, 0);
    }
#else
    ApplyWaveTextureResource(1);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1657) == 0) {
        EV_SetObjectFlagByIndex(2, 0);
        return;
    }
    ApplyWaveTextureResource(2);
    EV_SetObjectFlagByIndex(2, 1);
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
void AreaKRH1Cleanup(std::int32_t, std::uint32_t)
{
    SetupKRH1WeatherFlags();
}
const GameplayAreaDefinition areaKRH1GameplayDefinition{SHEN_NAME(KRH1), 0, AreaKRH1Init, AreaKRH1Main, AreaKRH1Cleanup};
} // namespace shenmue::s2::areas
#endif
