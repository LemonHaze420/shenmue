#include "s2/areas/area_krc1.hpp"
#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
typedef struct Vector4 {
    float x, y, z, w;
} Vector4;
typedef struct KRC1Environment {
    int state;
    void (*lighting)(int);
    void (*visibility)(int);
    void (*caveState)(int);
    void (*flicker)(int);
} KRC1Environment;
static float lightIntensity[2] = {0.0f, 0.0f};
static int firePhaseA = 0, smokePhaseA = 0, scalePhase = 0, firePhaseB = 0, unusedPhase = 0, smokePhaseB = 0;
static Vector3 caveCentre = {177.0f, 100.0f, 390.0f};
#else
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
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
static unsigned short firePalette[16] = {
#else
constexpr std::uint16_t firePalette[] = {
#endif
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6,
};

#if defined(SHENMUE_EVENT_OVERLAY)

static Vector4 emissive = {1.0f, 0.79f, 0.19f, 0.09f};
static float lightScale = 1.5f, unusedScale = 3.47f;
static int insideCave;
static HLibTask *waveTask;
static int waveEnabled;
static HLibTask *fireTask;
static int fireVisible, flickerEnabled;
static unsigned int fireEffect, smokeEffect;
static KRC1Environment environment;
static __inline__ int IsNight(const EventDateTime *time)
{
    return (unsigned char)(time->hour - 5) > 13;
}
void AreaCleanup(int afternoon);
void KRC1Ready(void *data);
void LoadKRC1Audio(int afternoon);
void KRC1MainLoop(void *data);
void KRC1Unused(void);
void CheckKRC1Cave(void *data);
void ApplyKRC1Lighting(int inside);
void QueueKRC1Wave(void);
void SetKRC1WaveState(int enabled);
void KRC1Wave(void *data);
void CreateKRC1Effects(void);
void SetKRC1EffectVisibility(int hidden);
void SetKRC1Flicker(int enabled);
void KRC1Fire(void *data);
#else

void ApplyKRC1Lighting(int insideCave);
void SetKRC1EffectVisibility(int hidden);
void SetKRC1WaveState(int enabled);
void SetKRC1Flicker(int enabled);
void KRC1MainLoop(void *data);
void KRC1Wave(void *data);
void KRC1Fire(void *data);
void CreateKRC1Effects(void);
void KRC1Ready(void *data);
bool IsNight(const DateTime &time)
{
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(time.hour) - 5) > 13U;
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
{
    EventDateTime time;
    FTBL.EV_FreeAuthorPak(1, "0507");
    FTBL.EV_FreeAuthorPak(1, "0509");
    FTBL.EV_FreeAuthorPak(1, "0517");
    FTBL.EV_FreeAuthorPak(1, "0518");
    FTBL.EV_FreeAuthorPak(1, "0524");
    FTBL.EV_FreeAuthorPak(1, "0525");
    FTBL.EV_FreeAuthorPak(1, "0530");
    FTBL.EV_FreeAuthorPak(1, "0531");
    FTBL.EV_SetPlayerFlag(12, 1495, 0);
    if (g_entryPoint == 1 || g_entryPoint == 70)
        insideCave = 1;
    else
        insideCave = 0;
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.1f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (IsNight(&time))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(KRC1MainLoop, 4, 11, SHEN_NAME4('k', 'r', 'c', '1'));
    firePhaseA = 0;
    smokePhaseA = 0;
    scalePhase = 0;
    firePhaseB = 0;
    unusedPhase = 0;
    smokePhaseB = 0;
    QueueKRC1Wave();
    CreateKRC1Effects();
    environment.state = 0;
    environment.lighting = ApplyKRC1Lighting;
    environment.visibility = SetKRC1EffectVisibility;
    environment.caveState = SetKRC1WaveState;
    environment.flicker = SetKRC1Flicker;
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, (unsigned int)&environment);
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KRC1Ready, 4, 11, SHEN_NAME4('s', 'l', 'e', 'p')), 10);
}
#endif

void KRC1Ready(void *data)
{
    FTBL.EV_setHoldStatesFlags(0);
    FTBL.HLib_DestroyCurrentTask();
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
{
    ApplyKRC1Lighting(insideCave);
    LoadKRC1Audio(afternoon);
}

void LoadKRC1Audio(int afternoon)
{
    if (afternoon)
        FTBL.EV_LoadAudio("KRC1_000", 1);
    else
        FTBL.EV_LoadAudio("KRC1_000", 0);
}
#endif

void KRC1MainLoop(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.ryoNotBusy())
        KRC1Unused();
#else
    (void)ryoNotBusy();
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void KRC1Unused(void) {}

void CheckKRC1Cave(void *data)
{
    Vector3 position;
    FTBL.GetCharaPosition(SHEN_NAME4('R', 'Y', 'O', '_'), 0xffffffff, &position, 0);
    position.y = caveCentre.y;
    if (FTBL.ComputeAngle(&position, 27.0f, 0, 0x8000, &caveCentre)) {
        if (!insideCave) {
            ApplyKRC1Lighting(1);
            insideCave = 1;
        }
    } else {
        if (insideCave) {
            ApplyKRC1Lighting(0);
            insideCave = 0;
        }
    }
}
#endif

void ApplyKRC1Lighting(int insideCave)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.SetDefaultVS();
    FTBL.ReinitFogTasks(0);
    FTBL.AssignLightParams(0, 0);
    FTBL.AssignLightParams(0, 1);
    FTBL.AssignLightParams(0, 2);
    FTBL.AssignLightParams(0, 3);
    FTBL.AssignLightParams(0, 4);
    FTBL.AssignLightParams(0, 5);
    FTBL.AssignLightParams(0, 6);
    FTBL.AssignLightParams(0, 7);
    FTBL.AssignLightParams(0, 8);
    FTBL.SetMapModelEnabled(1, 0);
    FTBL.SetMapModelEnabled(1, 1);
    FTBL.SetMapModelEnabled(1, 2);
    FTBL.SetMapModelEnabled(1, 3);
    FTBL.SetMapModelEnabled(1, 4);
    FTBL.SetMapModelEnabled(1, 5);
    FTBL.SetMapModelEnabled(1, 6);
    FTBL.SetMapModelEnabled(1, 7);
    FTBL.SetMapModelEnabled(1, 8);
    if (insideCave) {
        SetKRC1WaveState(0);
        FTBL.EV_SetWeatherInstruction(0x780, 0);
        FTBL.EV_SetWeatherInstruction(0xc, 0);
        FTBL.SetObjectFlagByIndex(0, 1);
        FTBL.SetObjectFlagByIndex(4, 1);
        FTBL.SetObjectFlagByIndex(5, 1);
        FTBL.SetObjectFlagByIndex(6, 0);
        FTBL.SetObjectFlagByIndex(7, 0);
        FTBL.SetObjectFlagByIndex(8, 0);
        if (IsNight(&time)) {
            FTBL.DebugLogToFileRemoved("KRC1 Light Setup!!  CAVE IN NIGHT ");
            FTBL.SetObjectFlagByIndex(2, 0);
            FTBL.SetObjectFlagByIndex(3, 1);
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '1'));
            FTBL.AssignLightProgram(0);
            FTBL.AssignLightProgram(6);
            FTBL.AssignLightProgram(7);
            FTBL.AssignLightProgram(8);
            FTBL.SetupLightConfig();
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '3'));
            if (FTBL.EV_GetPlayerFlag(11, 684)) {
                FTBL.AssignLightProgram(3);
                FTBL.AssignLightProgram(5);
                FTBL.DebugLogToFileRemoved(" 684 ON\n ");
            } else {
                FTBL.AssignLightProgram(3);
                FTBL.DebugLogToFileRemoved(" 684 OFF\n ");
            }
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '4'));
        } else {
            FTBL.DebugLogToFileRemoved("KRC1 Light Setup!!  CAVE IN DAY.\n");
            FTBL.SetObjectFlagByIndex(2, 1);
            FTBL.SetObjectFlagByIndex(3, 0);
            FTBL.AssignLightParams(1, 0);
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '2'));
            FTBL.AssignLightProgram(2);
            FTBL.AssignLightProgram(3);
            FTBL.AssignLightProgram(4);
            FTBL.AssignLightProgram(5);
            FTBL.AssignLightProgram(6);
            FTBL.AssignLightProgram(7);
            FTBL.AssignLightProgram(8);
        }
    } else {
        SetKRC1WaveState(1);
        FTBL.SetObjectFlagByIndex(0, 1);
        FTBL.SetObjectFlagByIndex(2, 0);
        FTBL.SetObjectFlagByIndex(3, 0);
        FTBL.SetObjectFlagByIndex(4, 1);
        FTBL.SetObjectFlagByIndex(5, 1);
        FTBL.SetObjectFlagByIndex(6, 1);
        if (IsNight(&time)) {
            FTBL.SetObjectFlagByIndex(7, 0);
            FTBL.SetObjectFlagByIndex(8, 1);
            FTBL.DebugLogToFileRemoved("KRC1 Light Setup!!  CAVE OUT NIGHT.\n");
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '3'));
            FTBL.AssignLightProgram(4);
            FTBL.AssignLightProgram(5);
            FTBL.EV_SetWeatherInstruction(0x780, 0);
            FTBL.EV_SetWeatherInstruction(0xc, 0);
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '1'));
            FTBL.AssignLightProgram(0);
            FTBL.AssignLightProgram(6);
            FTBL.AssignLightProgram(7);
            FTBL.AssignLightProgram(8);
            FTBL.SetupLightConfig();
            FTBL.CheckIsArgEquZero(1, 1);
        } else {
            FTBL.SetObjectFlagByIndex(7, 1);
            FTBL.SetObjectFlagByIndex(8, 0);
            FTBL.DebugLogToFileRemoved("KRC1 Light Setup!!  CAVE OUT DAY.\n");
            FTBL.AssignLightParams(1, 6);
            FTBL.AssignLightParams(1, 7);
            FTBL.AssignLightParams(1, 8);
            FTBL.EV_lightLoad(SHEN_NAME4('M', '0', '0', '2'));
            FTBL.AssignLightProgram(4);
            FTBL.AssignLightProgram(5);
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
            FTBL.EV_SetWeatherInstruction(0x780, 1);
            FTBL.EV_SetWeatherInstruction(0xf, 1);
            FTBL.EV_SetWeatherInstruction(0xc, 1);
        }
    }
#else
    DateTime time;
    shenGetCurrentDateTime(time);
    SetMapModelDataDefaultVertexShader();
    SetDefaultVS();
    ReinitFogTasks(0);
    for (std::int32_t index = 0; index != 9; ++index)
        EV_AssignLightParams(0, index);
    for (std::int32_t index = 0; index != 9; ++index)
        SetMapModelCommandRoutingEnabled(1, index);
    DateTime secondTime;
    shenGetCurrentDateTime(secondTime);
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
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void QueueKRC1Wave(void)
{
    waveTask = FTBL.HLib_EnqueueTaskWithoutParameter(KRC1Wave, 3, 2, SHEN_NAME4('w', 'a', 'v', 'e'));
    waveEnabled = 1;
}
#endif

void SetKRC1WaveState(int enabled)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    waveEnabled = enabled;
    if (enabled) {
        FTBL.SetObjectFlagByIndex(6, 1);
        if (IsNight(&time)) {
            FTBL.SetObjectFlagByIndex(7, 0);
            FTBL.SetObjectFlagByIndex(8, 1);
        } else {
            FTBL.SetObjectFlagByIndex(7, 1);
            FTBL.SetObjectFlagByIndex(8, 0);
        }
    } else {
        FTBL.SetObjectFlagByIndex(6, 0);
        FTBL.SetObjectFlagByIndex(7, 0);
        FTBL.SetObjectFlagByIndex(8, 0);
    }
#else
    DateTime time;
    shenGetCurrentDateTime(time);
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
#endif
}

void KRC1Wave(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    if (waveEnabled) {
        FTBL.nlSetTexanimUV(0.01f, 0.0f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(6));
        FTBL.nlSetTexanimUV(0.0f, 0.015f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(7));
        FTBL.nlSetTexanimUV(0.0f, 0.015f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(8));
    }
#else
    if (runtime.waveEnabled) {
        SetTextureAnimationUV(0.01f, 0.0f);
        ApplyTextureAnimationResource(6);
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(7);
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(8);
    }
#endif
}

void CreateKRC1Effects(void)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {154.74001f, 100.125f, 392.17999f};
    Vector3 spawn = {0.0f, 0.0f, 0.0f};
    Vector3 velocity = {0.0f, 0.005f, 0.0f};
    Vector3 variance = {0.001f, 0.003f, 0.001f};
    fireEffect = FTBL.CreateFireEffect(&position, 0.5f, 0.5f, 0, 0);
    FTBL.SetFirePalette(fireEffect, firePalette);
    FTBL.SetFireFalloff(fireEffect, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    FTBL.SetFireFalloffScalar(fireEffect, -0.2980392277240753f);
    FTBL.SetParticleEffectSize(4, fireEffect, 0.5490196347236633f, 0.8235294222831726f);
    FTBL.SetParticleEffectPosition(4, fireEffect, &position, &position);
    FTBL.EventFunction882(fireEffect, 0.25f);
    fireTask = FTBL.HLib_EnqueueTaskWithoutParameter(KRC1Fire, 3, 2, SHEN_NAME4('f', 'i', 'r', 'e'));
    fireVisible = 0;
    flickerEnabled = 0;
    smokeEffect = FTBL.CreateSmokeEffect(&position, &spawn, 50, 0.01f, 0, 0);
    FTBL.SetSmokeEffectFadeFactor(smokeEffect, 32);
    FTBL.SetSmokeEffectFalloff(smokeEffect, 144);
    FTBL.SetParticleEffectColour(0, smokeEffect, 0, 96, 96, 96);
    FTBL.SetParticleEffectInitialScale(0, smokeEffect, 0.2f);
    FTBL.SetParticleEffectSize(0, smokeEffect, 0.5f, 0.5f);
    FTBL.SetParticleEffectVelocity(0, smokeEffect, &velocity, &variance);
#else
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
#endif
}

void SetKRC1EffectVisibility(int hidden)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    fireVisible = hidden;
    if (hidden) {
        FTBL.ResumeParticleEffect(4, fireEffect);
        FTBL.ResumeParticleEffect(0, smokeEffect);
    } else {
        FTBL.SuspendParticleEffect(4, fireEffect);
        FTBL.SuspendParticleEffect(0, smokeEffect);
    }
#else
    runtime.fireVisible = hidden;
    if (hidden) {
        SuspendParticleEffect(4, runtime.fireEffect);
        SuspendParticleEffect(0, runtime.smokeEffect);
    } else {
        ResumeParticleEffect(4, runtime.fireEffect);
        ResumeParticleEffect(0, runtime.smokeEffect);
    }
#endif
}

void SetKRC1Flicker(int enabled)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    flickerEnabled = enabled;
#else
    runtime.flickerEnabled = enabled;
#endif
}

void KRC1Fire(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    if (fireVisible && flickerEnabled) {
        lightIntensity[0] = (FTBL.CalculateFixedSine(firePhaseA) * FTBL.CalculateFixedSine(firePhaseB)) * 0.33f + 1.3f;
        lightIntensity[1] = lightIntensity[0];
        firePhaseA += 1456;
        firePhaseB += 10922;
        emissive.z = (FTBL.CalculateFixedSine(smokePhaseA) * FTBL.CalculateFixedSine(smokePhaseB)) * 0.02f + 0.23999999f;
        smokePhaseA += 910;
        smokePhaseB += 5461;
        lightScale = FTBL.CalculateFixedSine(scalePhase) * 0.1f + 1.5f;
        scalePhase += 1274;
        unusedScale = FTBL.CalculateFixedSine(unusedPhase) * 0.5f + 3.47f;
        unusedPhase += 12014;
        FTBL.SetLightFalloff(12, lightIntensity);
        FTBL.SetLightRuntimeColour(12, (float *)&emissive);
        FTBL.SetLightSceneIntensity(12, lightScale);
    }
#else
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
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

void AreaKRC1Init()
{
    runtime = {};
    runtime.emissive = {1.0f, 0.79000002f, 0.19f, 0.09f};
    runtime.lightScale = 1.5f;
    environment = {};
}

void AreaKRC1Main(std::uint32_t)
{
    constexpr const char *packages[]{"0507", "0509", "0517", "0518", "0524", "0525", "0530", "0531"};
    for (const char *package : packages)
        EV_FreeAuthorPak(1, package);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    runtime.insideCave = g_entryPoint == 1 || g_entryPoint == 70;
    EV_UpdateWeatherForMinute(0, 0, 0, 0.1f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    DateTime time;
    shenGetCurrentDateTime(time);
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

void AreaKRC1Cleanup(std::int32_t afternoon, std::uint32_t)
{
    ApplyKRC1Lighting(runtime.insideCave);
    EV_LoadAudio("KRC1_000", afternoon ? 1 : 0);
}

const GameplayAreaDefinition areaKRC1GameplayDefinition{SHEN_NAME(KRC1), 0, AreaKRC1Init, AreaKRC1Main, AreaKRC1Cleanup};
} // namespace shenmue::s2::areas
#endif
