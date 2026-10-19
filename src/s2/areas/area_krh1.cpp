#include "s2/areas/area_krh1.hpp"

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

constexpr std::int32_t enableFogEffect = 0x780;
constexpr std::int32_t sceneAndGlobalLighting = 0x0c;
constexpr std::int32_t completeLightingUpdate = 0x0f;
constexpr ButterflyRegion krh1ButterflyRegions[]{
    {0, 6, {1362.7525634765625f, 100.43499755859375f, 749.0584106445312f}},
    {0, 4, {1345.2142333984375f, 101.04499816894531f, 753.7396850585938f}},
    {0, 6, {1328.930419921875f, 100.51699829101562f, 754.0189208984375f}},
    {0, 4, {1324.57373046875f, 102.57869720458984f, 762.5322265625f}},
    {0, 6, {1302.1494140625f, 97.1177978515625f, 788.8693237304688f}},
    {0, 4, {1313.42578125f, 100.41600036621094f, 746.5206298828125f}},
};

void SetupKRH1WeatherFlags() {
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_SetWeatherInstruction(enableFogEffect, 1);
    EV_SetWeatherInstruction(completeLightingUpdate, 1);
    EV_SetWeatherInstruction(sceneAndGlobalLighting, 1);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
}

void eKRH1_ET_WAVE_KRH1_Callback(void *) {
    const auto colours = GetWeatherWaveColours();
    const Vector3 diffuse{(static_cast<float>(colours[1] >> 16) / 255.0f) * 0.007214f, (static_cast<float>(colours[1] >> 8) / 255.0f) * 0.00745f,
        (static_cast<float>(colours[1]) / 255.0f) * 0.02176f};
    const Vector3 specular{(static_cast<float>(colours[0] >> 16) / 255.0f) * 0.2549f, (static_cast<float>(colours[0] >> 8) / 255.0f) * 0.38f,
        (static_cast<float>(colours[0]) / 255.0f) * 0.2862f};
    BuildWaveLightingTable(0, 1.0f, 1.0f, diffuse, specular, 42.54f, 1.0f);
    ApplyWaveTextureResource(1);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1657) == 0) {
        EV_SetObjectFlagByIndex(2, 0);
        return;
    }
    ApplyWaveTextureResource(2);
    EV_SetObjectFlagByIndex(2, 1);
}

} // namespace

void AreaKRH1Init() {
}

void AreaKRH1Main(std::uint32_t startParameters) {
    if (startParameters != 0) {
        SetCharaKill(SHEN_NAME(a143), 1);
    }

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    if (static_cast<std::uint8_t>(dateTime.hour - 8) > 14U) {
        dateTime.hour = 8;
        dateTime.minute = 0;
        SetCurrentDateTimeTo(dateTime);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }

    (void)EV_FreeAuthorPak(1, "0503");
    SetupKRH1WeatherFlags();
    SetMapModelVertexShaderType(1, 1);
    SetMapModelVertexShaderType(2, 1);
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
    if (EV_GetShenmueFlag(0x0d8) != 0) {
        areaFlags |= AreaFlag_DeferPreAreaMainLoop;
    }
    HLib::EnqueueTaskWithoutParameter(eKRH1_ET_WAVE_KRH1_Callback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
}

void AreaKRH1Cleanup(std::int32_t, std::uint32_t) {
    SetupKRH1WeatherFlags();
}

const GameplayAreaDefinition areaKRH1GameplayDefinition{SHEN_NAME(KRH1), 0, AreaKRH1Init, AreaKRH1Main, AreaKRH1Cleanup};
} // namespace shenmue::s2::areas
