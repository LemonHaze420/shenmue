#include "s2/areas/area_krk6.hpp"
#include "s2/areas/area_fall_events.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
void KRK6MainLoopCallback(void *) {
    UpdateShenhuaOcclusionAlpha();
}
void KRK6WaveCallback(void *) {
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
}
} // namespace
void AreaKRK6Init() {
}
void AreaKRK6Main(std::uint32_t) {
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    HLib::EnqueueTaskWithoutParameter(KRK6MainLoopCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    DateTime date;
    GetCurrentDateTime(date);
    if (static_cast<std::uint32_t>(date.hour - 8) > 14U) {
        date.hour = 8;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
    std::array<std::byte, 256> distanceWork, firstWork, secondWork;
    GenerateFFT(.5f);
    FillFftDistance(FillFftKernel(3.92f, GenerateDistanceFalloffRowMajor(1.29f, distanceWork)));
    void *second = FillFftKernel(.073f, GenerateDistanceFalloffKernel8x8(.564f, secondWork));
    void *first = FillFftKernel(.06f, GenerateDistanceFalloffKernel8x8(1.035f, firstWork));
    ApplyFft(first, second);
    FinalizeFft(.0001f, .0001f);
    SetWaveLighting(.1f, {.30000001192092896f, .800000011920929f, .5f});
    SetMapModelVertexShaderType(0, 1);
    HLib::EnqueueTaskWithoutParameter(KRK6WaveCallback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    QueueRockyAreaFallEvent();
}
void AreaKRK6Cleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaKRK6GameplayDefinition{SHEN_NAME(KRK6), 0, AreaKRK6Init, AreaKRK6Main, AreaKRK6Cleanup};
} // namespace shenmue::s2::areas
