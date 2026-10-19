#include "s2/debug.hpp"
#include "s2/map_control.hpp"
#include "s2/areas/area_krc1.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {
struct KRC1Runtime {
    std::int32_t insideCave;
    std::int32_t waveEnabled;
    std::int32_t fireVisible;
    std::int32_t flickerEnabled;
    std::uint32_t fireEffect;
    std::uint32_t smokeEffect;
    std::uint32_t firePhaseA;
    std::uint32_t firePhaseB;
    std::uint32_t smokePhaseA;
    std::uint32_t smokePhaseB;
    std::uint32_t scalePhase;
    std::uint32_t unusedPhase;
    float lightIntensityX;
    float lightIntensityY;
    Vector4 emissive;
    float lightScale;
};
KRC1Runtime runtime{};
struct KRC1EnvironmentCallbacks {
    std::uint32_t state;
    std::uint32_t padding04;
    void (*lighting)(std::int32_t);
    void (*visibility)(std::int32_t);
    void (*caveState)(std::int32_t);
    void (*flicker)(std::int32_t);
};
static_assert(sizeof(KRC1EnvironmentCallbacks) == 40);
static_assert(offsetof(KRC1EnvironmentCallbacks, lighting) == 8);
KRC1EnvironmentCallbacks environment{};
constexpr shen_name caveNight{SHEN_NAME(M001)};
constexpr shen_name caveDay{SHEN_NAME(M002)};
constexpr shen_name caveFill{SHEN_NAME(M003)};
constexpr shen_name caveFire{SHEN_NAME(M004)};
constexpr std::uint16_t firePalette[]{
    0x0311,
    0x0311,
    0x0411,
    0x0511,
    0x0612,
    0x0712,
    0x0a32,
    0x0b32,
    0x0b42,
    0x0c50,
    0x0d70,
    0x0e70,
    0x0e80,
    0x0fa0,
    0x0fc3,
    0x0fd6,
};
bool IsNight(const DateTime &time) {
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(time.hour) - 5) > 13U;
}
void ApplyKRC1Lighting(std::int32_t insideCave) {
    DateTime time;
    GetCurrentDateTime(time);
    SetMapModelDataDefaultVertexShader();
    SetDefaultVS();
    ReinitFogTasks(0);
    for (std::int32_t index = 0; index != 9; ++index)
        EV_AssignLightParams(0, index);
    for (std::int32_t index = 0; index != 9; ++index)
        SetMapModelCommandRoutingEnabled(1, index);
    DateTime secondTime;
    GetCurrentDateTime(secondTime);
    if (insideCave) {
        runtime.waveEnabled = 0;
        EV_SetObjectFlagByIndex(6, 0);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(4, 1);
        EV_SetObjectFlagByIndex(5, 1);
        EV_SetObjectFlagByIndex(6, 0);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 0);
        if (IsNight(time)) {
            EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 357, "KRC1 Light Setup!!  CAVE IN NIGHT ");
            EV_SetObjectFlagByIndex(2, 0);
            EV_SetObjectFlagByIndex(3, 1);
            EV_lightLoad(caveNight);
            AssignLightProgram(0);
            AssignLightProgram(6);
            AssignLightProgram(7);
            AssignLightProgram(8);
            SetupLightConfig();
            EV_lightLoad(caveFill);
            AssignLightProgram(3);
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 684)) {
                AssignLightProgram(5);
                EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 370, " 684 ON\n ");
            } else {
                EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 373, " 684 OFF\n ");
            }
            EV_lightLoad(caveFire);
        } else {
            EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 377, "KRC1 Light Setup!!  CAVE IN DAY.\n");
            EV_SetObjectFlagByIndex(2, 1);
            EV_SetObjectFlagByIndex(3, 0);
            EV_AssignLightParams(1, 0);
            EV_lightLoad(caveDay);
            AssignLightProgram(2);
            AssignLightProgram(3);
            AssignLightProgram(4);
            AssignLightProgram(5);
            AssignLightProgram(6);
            AssignLightProgram(7);
            AssignLightProgram(8);
        }
        return;
    }
    runtime.waveEnabled = 1;
    EV_SetObjectFlagByIndex(6, 1);
    EV_SetObjectFlagByIndex(7, IsNight(secondTime) ? 0 : 1);
    EV_SetObjectFlagByIndex(8, IsNight(secondTime) ? 1 : 0);
    EV_SetObjectFlagByIndex(0, 1);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 0);
    EV_SetObjectFlagByIndex(4, 1);
    EV_SetObjectFlagByIndex(5, 1);
    EV_SetObjectFlagByIndex(6, 1);
    if (IsNight(time)) {
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 1);
        EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 402, "KRC1 Light Setup!!  CAVE OUT NIGHT.\n");
        EV_lightLoad(caveFill);
        AssignLightProgram(4);
        AssignLightProgram(5);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_lightLoad(caveNight);
        AssignLightProgram(0);
        AssignLightProgram(6);
        AssignLightProgram(7);
        AssignLightProgram(8);
        SetupLightConfig();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_SetObjectFlagByIndex(7, 1);
        EV_SetObjectFlagByIndex(8, 0);
        EV_debug_log("KRC1\\eKRC1_et_KRC1.c", 418, "KRC1 Light Setup!!  CAVE OUT DAY.\n");
        EV_AssignLightParams(1, 6);
        EV_AssignLightParams(1, 7);
        EV_AssignLightParams(1, 8);
        EV_lightLoad(caveDay);
        AssignLightProgram(4);
        AssignLightProgram(5);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_SetWeatherInstruction(0x0f, 1);
        EV_SetWeatherInstruction(0x0c, 1);
    }
}
void SetKRC1EffectVisibility(std::int32_t hidden) {
    runtime.fireVisible = hidden;
    if (hidden) {
        SuspendParticleEffect(4, runtime.fireEffect);
        SuspendParticleEffect(0, runtime.smokeEffect);
    } else {
        ResumeParticleEffect(4, runtime.fireEffect);
        ResumeParticleEffect(0, runtime.smokeEffect);
    }
}
void SetKRC1WaveState(std::int32_t enabled) {
    DateTime time;
    GetCurrentDateTime(time);
    runtime.waveEnabled = enabled;
    if (!enabled) {
        EV_SetObjectFlagByIndex(6, 0);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 0);
    } else {
        EV_SetObjectFlagByIndex(6, 1);
        EV_SetObjectFlagByIndex(7, IsNight(time) ? 0 : 1);
        EV_SetObjectFlagByIndex(8, IsNight(time) ? 1 : 0);
    }
}
void SetKRC1Flicker(std::int32_t enabled) {
    runtime.flickerEnabled = enabled;
}
void KRC1MainLoop(void *) {
    (void)ryoNotBusy();
}
void KRC1Wave(void *) {
    if (runtime.waveEnabled) {
        SetTextureAnimationUV(0.01f, 0.0f);
        ApplyTextureAnimationResource(6);
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(7);
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(8);
    }
}
void KRC1Fire(void *) {
    if (!runtime.fireVisible || !runtime.flickerEnabled)
        return;
    const float fireB = sin_fixed16(static_cast<std::int16_t>(runtime.firePhaseB));
    const float fireA = sin_fixed16(static_cast<std::int16_t>(runtime.firePhaseA));
    runtime.firePhaseA += 1456;
    runtime.firePhaseB += 10922;
    runtime.lightIntensityX = (fireB * fireA) * 0.01f + 1.1f;
    runtime.lightIntensityY = (fireB * fireA) * 0.01f + 1.1f;
    const float smokeB = sin_fixed16(static_cast<std::int16_t>(runtime.smokePhaseB));
    const float smokeA = sin_fixed16(static_cast<std::int16_t>(runtime.smokePhaseA));
    runtime.smokePhaseA += 910;
    runtime.smokePhaseB += 5461;
    runtime.emissive.z = (smokeB * smokeA) * 0.02f + 0.23999999f;
    runtime.lightScale = sin_fixed16(static_cast<std::int16_t>(runtime.scalePhase)) * 0.1f + 1.5f;
    runtime.scalePhase += 1274;
    (void)sin_fixed16(static_cast<std::int16_t>(runtime.unusedPhase));
    runtime.unusedPhase += 12014;
    SetLightFalloff(12, &runtime.lightIntensityX);
    SetLightRuntimeColour(12, runtime.emissive);
    SetLightSceneIntensity(12, runtime.lightScale);
}
void CreateKRC1Effects() {
    const Vector3 position{154.74001f, 100.125f, 392.17999f};
    runtime.fireEffect = static_cast<std::uint32_t>(CreateFireEffect(position, 0.5f, 0.5f, 0, 0));
    SetFirePalette(runtime.fireEffect, firePalette);
    SetFireFalloff(runtime.fireEffect, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(runtime.fireEffect, -0.2980392277240753f);
    SetParticleEffectSize(4, runtime.fireEffect, 0.5490196347236633f, 0.8235294222831726f);
    SetParticleEffectPosition(4, runtime.fireEffect, &position, &position);
    SetFireAlpha(runtime.fireEffect, 0.25f);
    HLib::EnqueueTaskWithoutParameter(KRC1Fire, 3, 2, static_cast<std::int32_t>(SHEN_NAME(fire)));
    runtime.fireVisible = 0;
    runtime.flickerEnabled = 0;
    runtime.smokeEffect = static_cast<std::uint32_t>(CreateSmokeEffect(position, Vector3{}, 50, 0.01f, shen_name{}, 0));
    SetSmokeEffectFalloff(runtime.smokeEffect, 32);
    SetSmokeEffectFadeFactor(runtime.smokeEffect, 144);
    SetParticleEffectColour(0, runtime.smokeEffect, 0, 96, 96, 96);
    SetParticleEffectInitialScale(0, runtime.smokeEffect, 0.2f);
    SetParticleEffectSize(0, runtime.smokeEffect, 0.5f, 0.5f);
    const Vector3 velocity{0.0f, 0.005f, 0.0f};
    const Vector3 variance{0.001f, 0.003f, 0.001f};
    SetParticleEffectVelocity(0, runtime.smokeEffect, velocity, variance);
}
void KRC1Ready(void *) {
    EV_setHoldStatesFlags(0);
    HLib::DestroyCurrentTask();
}
} // namespace
void AreaKRC1Init() {
    runtime = {};
    runtime.emissive = {1.0f, 0.79000002f, 0.19f, 0.09f};
    runtime.lightScale = 1.5f;
    environment = {};
}
void AreaKRC1Main(std::uint32_t) {
    constexpr const char *packages[]{"0507", "0509", "0517", "0518", "0524", "0525", "0530", "0531"};
    for (const char *package : packages)
        EV_FreeAuthorPak(1, package);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    runtime.insideCave = g_entryPoint == 1 || g_entryPoint == 70;
    EV_UpdateWeatherForMinute(0, 0, 0, 0.1f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    DateTime time;
    GetCurrentDateTime(time);
    ApplyKRC1Lighting(runtime.insideCave);
    EV_LoadAudio("KRC1_000", IsNight(time) ? 1 : 0);
    HLib::EnqueueTaskWithoutParameter(KRC1MainLoop, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(krc1)));
    runtime.firePhaseA = 0;
    runtime.smokePhaseA = 0;
    runtime.scalePhase = 0;
    runtime.firePhaseB = 0;
    runtime.unusedPhase = 0;
    runtime.smokePhaseB = 0;
    HLib::EnqueueTaskWithoutParameter(KRC1Wave, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
    runtime.waveEnabled = 1;
    CreateKRC1Effects();
    environment.state = 0;
    environment.lighting = ApplyKRC1Lighting;
    environment.visibility = SetKRC1EffectVisibility;
    environment.caveState = SetKRC1WaveState;
    environment.flicker = SetKRC1Flicker;
    ExecuteCurrAreaMainFunc_Wrapper(2, reinterpret_cast<std::uintptr_t>(&environment));
    HLib::Task *const ready = HLib::EnqueueTaskWithoutParameter(KRC1Ready, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(slep)));
    HLib::SetupTimedTask(ready, 10);
}
void AreaKRC1Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyKRC1Lighting(runtime.insideCave);
    EV_LoadAudio("KRC1_000", afternoon ? 1 : 0);
}
const GameplayAreaDefinition areaKRC1GameplayDefinition{SHEN_NAME(KRC1), 0, AreaKRC1Init, AreaKRC1Main, AreaKRC1Cleanup};
} // namespace shenmue::s2::areas
