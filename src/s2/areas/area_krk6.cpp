#include "s2/areas/area_krk6.hpp"
#include "s2/areas/area_fall_events.hpp"
#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#else
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {
void KRK6MainLoopCallback(void *data);
void KRK6WaveCallback(void *data);
} // namespace

void AreaKRK6Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
#else
void AreaKRK6Main(std::uint32_t)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime date;
    float distance[64], first[64], second[64];
    FTBL.WSCN_LoadWeatherData(FTBL.Filepath_Gen(0), "W_COND.BIN");
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.HLib_EnqueueTaskWithoutParameter(KRK6MainLoopCallback, 4, 0xb, TASK_EVNT);
    FTBL.GetCurrentDateTime((DateTime *)&date);
#else
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    HLib::EnqueueTaskWithoutParameter(KRK6MainLoopCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    DateTime date;
    shenGetCurrentDateTime(date);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if ((unsigned char)(date.hour - 8) > 14) {
#else
    if (static_cast<std::uint32_t>(date.hour - 8) > 14U) {
#endif
        date.hour = 8;
        date.minute = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
#else
        SetCurrentDateTimeTo(date);
#endif
        FTBL.dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaCleanup(0);
    {
        Vector3 lighting = {0.3f, 0.8f, 0.5f};
        void *firstKernel, *secondKernel;
        FTBL.GenerateFFT(0.5f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(3.92f, FTBL.generate_distance_falloff_row_major(1.29f, distance)));
        firstKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(1.035f, first));
        secondKernel = FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.564f, second));
        FTBL.fft_bins_load_pairwise_tables(firstKernel, secondKernel);
        FTBL.FFTUtil(0.0001f, 0.0001f);
        FTBL.set_lighting_flags(0.1f, &lighting);
    }
#else
    std::array<std::byte, 256> distanceWork, firstWork, secondWork;
    GenerateFFT(.5f);
    FillFftDistance(FillFftKernel(3.92f, GenerateDistanceFalloffRowMajor(1.29f, distanceWork)));
    void *second = FillFftKernel(.073f, GenerateDistanceFalloffKernel8x8(.564f, secondWork));
    void *first = FillFftKernel(.06f, GenerateDistanceFalloffKernel8x8(1.035f, firstWork));
    ApplyFft(first, second);
    FinalizeFft(.0001f, .0001f);
    SetWaveLighting(.1f, {.30000001192092896f, .800000011920929f, .5f});
#endif
    FTBL.SetMapModelVertexShaderType(0, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(KRK6WaveCallback, 3, 2, TASK_EVNT);
#else
    HLib::EnqueueTaskWithoutParameter(KRK6WaveCallback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
#endif
    QueueRockyAreaFallEvent();
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
    AreaKRK6Nullsub();
}
void AreaKRK6Nullsub() {
}
#else
void AreaKRK6Cleanup(std::int32_t, std::uint32_t) {
}

namespace {
#endif
void KRK6MainLoopCallback(void *data) {
#if !defined(SHENMUE_EVENT_OVERLAY)
    UpdateShenhuaOcclusionAlpha();
#endif
}

void KRK6WaveCallback(void *data) {
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
        FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, r, g, b, sr, sg, sb, power, 1.0f);
    }
    FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(0));
#else
    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];

    BuildWaveLightingTable(1, 1, 1,
        {static_cast<float>(diffuse >> 16) / 255.0f * .36070001125335693f, static_cast<float>(diffuse >> 8) / 255.0f * .3725000023841858f,
            static_cast<float>(diffuse) / 255.0f * .4352000057697296f},
        {static_cast<float>(specular >> 16) / 255.0f * .2549000084400177f, static_cast<float>(specular >> 8) / 255.0f * .3799999952316284f,
            static_cast<float>(specular) / 255.0f * .28619998693466187f},
        42.540000915527344f, 1);
    ApplyWaveTextureResource(0);
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const GameplayAreaDefinition areaKRK6GameplayDefinition{SHEN_NAME(KRK6), 0, AreaKRK6Init, AreaKRK6Main, AreaKRK6Cleanup};
} // namespace shenmue::s2::areas
#endif
