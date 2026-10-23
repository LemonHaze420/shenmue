#include "s2/debug.hpp"
#include "s2/areas/area_ak00.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_assets.hpp"
#include "s2/pati.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/lighting.hpp"

#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t suppressAudioFlag = 0x51;
constexpr std::int32_t bikeStoryFlag = 0x3b2;
constexpr std::int16_t suppressWavePlayerFlag = 2228;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name bike{SHEN_NAME(bike)};
constexpr shen_name waveTaskToken{SHEN_NAME(wave)};

struct AK00State {
    std::array<std::byte, 0x100> distanceWork{};
    std::array<std::byte, 0x100> firstKernelWork{};
    std::array<std::byte, 0x100> secondKernelWork{};
    std::int32_t initializing{};
};

AK00State state{};

void SetBikeEnabledForStoryFlag() {
    const bool enabled = EV_GetShenmueFlag(bikeStoryFlag) != 0;
    if (!FindCharacterByScenarioName(bike))
        return;

    if (enabled) {
        EnsureScenarioCharacterDraw(bike);
        EnableScenarioCharacterDisplay(bike);
        ToggleFlagOnFieldEntity(bike, 1);
    } else {
        DisableScenarioCharacterDisplay(bike);
        ToggleScenarioCharacterFlag(bike);
        ToggleFlagOnFieldEntity(bike, 0);
    }
}

bool IsPlayerInAK00ShadowRegion() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control->ryo)
        return false;

    Vector3 location{};
    plymUpdatePlayerSearchLocation(&location);
    return location.y < 97.0f && location.x > 270.0f && location.x < 280.0f && location.z > 250.0f && location.z < 270.0f;
}

void UpdateAK00Wave(void *) {
    SetGlobalShadowFadeRequested(IsPlayerInAK00ShadowRegion());
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, suppressWavePlayerFlag) != 0) {
        return;
    }

    bool update = state.initializing != 0;
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    if ((dateTime.minute & 7) == 0 && dateTime.second == 0)
        update = true;

    if (!update) {
        Vector3 playerPosition{};
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
        update = playerPosition.x <= 246.5f;
    }
    if (!update)
        return;

    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.75f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.75f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.8999999761581421f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f * 0.30000001192092896f},
        42.540000915527344f, 1.0f);
    SetMapModelSlotEnabled(3, 0);
    SetMapModelSlotEnabled(4, 0);
    AdvanceWaterTextureAnimation();
}

void ConfigureAK00Fft() {
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, state.distanceWork)));
    void *const secondKernel = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, state.secondKernelWork));
    void *const firstKernel = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, state.firstKernelWork));
    ApplyFft(firstKernel, secondKernel);
    FinalizeFft(0.0010000000474974513f, -0.0010000000474974513f);
}

void ApplyAK00LightingAndAudio(std::int32_t afternoon) {
    if (afternoon != 0) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(7, 1);
        EV_SetObjectFlagByIndex(6, 0);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(6, 1);
    }
    SetBikeEnabledForStoryFlag();

    if (state.initializing == 0 && EV_GetShenmueFlag(suppressAudioFlag) == 0) {
        EV_LoadAudio("AK00_000", afternoon != 0 ? 1 : 0);
    }
}

} // namespace

void AreaAK00Init() {
    state = {};
}

void AreaAK00Main(std::uint32_t) {
    state.initializing = 1;
    ResetSoundProgramPhaseFlags(1);
    ApplyAK00LightingAndAudio(EV_GetShenmueFlag(nightTimeFlag));
    InitPati();
    SetGamble();

    InitializeWaterTextureAnimation(4);
    ConfigureAK00Fft();
    SetWaveLighting(0.1f, Vector3{0.05979999899864197f, 0.9768000245094299f, 0.2054000049829483f});
    HLib::EnqueueTaskWithoutParameter(UpdateAK00Wave, 3, 2, static_cast<std::int32_t>(waveTaskToken.name));
    SetMapModelVertexShaderType(3, 1);
    SetMapModelVertexShaderType(4, 1);
    UpdateAK00Wave(nullptr);
    state.initializing = 0;
    SetMainMapMinimumModelHeight(100.0f);
}

void AreaAK00Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyAK00LightingAndAudio(afternoon);
}

const GameplayAreaDefinition areaAK00GameplayDefinition{SHEN_NAME(AK00), 0, AreaAK00Init, AreaAK00Main, AreaAK00Cleanup};

} // namespace shenmue::s2::areas
