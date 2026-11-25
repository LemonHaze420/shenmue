#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
static unsigned short firePalette[16] = {
#include "kes1_fire_palette.inl"
};
static int tamaPhase = 0;
static Vector3 tamaReferencePosition = {115.73560333251953f, 106.06839752197266f, -14.931400299072266f};
static Vector3 firePosition = {66.2300034f, 101.5f, 139.139999f};
static Vector3 secondFirePosition = {53.9799995f, 96.1999969f, 117.300003f};
static unsigned int effect;
static int effectActive;
static int effectAttached;
static unsigned int transitionPending;
#define KES1_EFFECT effect
#define KES1_TRANSITION_PENDING transitionPending
#define KES1_EFFECT_ATTACHED effectAttached
#define KES1_EFFECT_ACTIVE effectActive
#define KES1_TAMA_PHASE tamaPhase
void AreaCleanup(int afternoon);
void KES1MainLoopCallback(void *taskData);
void RunKES1AreaTransition(int eventArgument);
void UpdateTamaTransparency(void);
void CreateKES1Fire(void);
void AttachKES1Fire(void);
void UpdateKES1TextureAnimation(void);
#else
#include "area_kes1.hpp"

#include "common/math.hpp"
#include "s2/area_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name tama{SHEN_NAME(TAMA)};
constexpr shen_name mainTaskToken{SHEN_NAME(main)};
constexpr Vector3 tamaReferencePosition{115.73560333251953f, 106.06839752197266f, -14.931400299072266f};

struct KES1State {
    std::uint32_t effect{};
    std::uint32_t transitionPending{};
    std::int32_t effectAttached{};
    std::int32_t effectActive{};
    std::int32_t tamaPhase{};
};

KES1State state{};

#define KES1_EFFECT state.effect
#define KES1_TRANSITION_PENDING state.transitionPending
#define KES1_EFFECT_ATTACHED state.effectAttached
#define KES1_EFFECT_ACTIVE state.effectActive
#define KES1_TAMA_PHASE state.tamaPhase
void KES1MainLoopCallback(void *taskData);
void RunKES1AreaTransition(std::uintptr_t eventArgument);
void UpdateTamaTransparency();
void CreateKES1Fire();
void LoadKES1MapPackage();
} // namespace

void AreaKES1Init() {
    state = {};
}

#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    HLibTask *task;
    EventDateTime dateTime;
#else
void AreaKES1Main(std::uint32_t) {
#endif
    FTBL.EV_SetPlayerFlag(12, 1757, 1);
    FTBL.EV_SetPlayerFlag(12, 2228, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithoutParameter(KES1MainLoopCallback, 4, 0x0b, SHEN_NAME4('m', 'a', 'i', 'n'));
    FTBL.HLib_SetupTimedTask(task, 10);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
        AreaCleanup(1);
    else
        AreaCleanup(1);
#else
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(KES1MainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(mainTaskToken.name));
    HLib::SetupTimedTask(task, 10);

    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
#endif
    FTBL.FreeWAIR();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
#else
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
#endif
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    CreateKES1Fire();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, (void *)-2);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"MPK01", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, (void *)-1);
    FTBL.EV_NPC_NewLoadSw_0(1);
#else
    ExecuteCurrAreaMainFunc_Wrapper(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(-2)));
    LoadKES1MapPackage();
    ExecuteCurrAreaMainFunc_Wrapper(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(-1)));
    EV_NPC_NewLoadSw(1);
#endif
    KES1_TRANSITION_PENDING = 0;
#if !defined(SHENMUE_EVENT_OVERLAY)
    SetCharacterUngroundedShadowMode(1);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaKES1Cleanup(std::int32_t, std::uint32_t) {
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
void LoadKES1MapPackage() {
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("MPK01"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

#endif

void KES1MainLoopCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    UpdateKES1TextureAnimation();
#else
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2228) == 0) {
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(12);
        SetTextureAnimationUV(0.0f, 0.090000004f);
        ApplyTextureAnimationResource(13);
    }
#endif

    if (FTBL.ryoNotBusy() || KES1_TRANSITION_PENDING != 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.GetCurrentDateTime((DateTime *)&dateTime);
#else
        DateTime dateTime{};
        shenGetCurrentDateTime(dateTime);
#endif
        if (dateTime.hour >= 23 && FTBL.EV_GetShenmueFlag(0x1ce) != 0)
            RunKES1AreaTransition(2);
        else if (dateTime.hour >= 19 && FTBL.EV_GetShenmueFlag(0x2dc) != 0)
            RunKES1AreaTransition(0);
    }

    if (FTBL.EV_GetShenmueFlag(0xe3) != 0)
        UpdateTamaTransparency();

    if (FTBL.EV_GetPlayerFlag(11, 1757) != 0) {
        if (KES1_EFFECT_ATTACHED == 0 && FTBL.ryoNotBusy() && FTBL.EV_GetShenmueFlag(0x1ce) != 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
            AttachKES1Fire();
#else
            SetFirePosition(KES1_EFFECT, 1, &KES1_SECOND_FIRE_POSITION);
            KES1_EFFECT_ATTACHED = 1;
#endif
        }
        if (KES1_EFFECT_ACTIVE == 0 && FTBL.EV_GetShenmueFlag(0x1ce) != 0) {
            FTBL.ResumeParticleEffect(4, KES1_EFFECT);
            KES1_EFFECT_ACTIVE = 1;
        }
    } else if (KES1_EFFECT_ACTIVE != 0 && FTBL.EV_GetShenmueFlag(0x2dc) != 0) {
        FTBL.SuspendParticleEffect(4, KES1_EFFECT);
        KES1_EFFECT_ACTIVE = 0;
#if !defined(SHENMUE_EVENT_OVERLAY)
        return;
#endif
    }

    if (KES1_EFFECT_ACTIVE != 0 && FTBL.EV_GetShenmueFlag(0x2b2) != 0) {
        KES1_EFFECT_ACTIVE = 0;
        FTBL.SuspendParticleEffect(4, KES1_EFFECT);
    }
}

#if defined(SHENMUE_EVENT_OVERLAY)
void RunKES1AreaTransition(int eventArgument) {
#else
void RunKES1AreaTransition(std::uintptr_t eventArgument) {
#endif
    KES1_TRANSITION_PENDING = 1;
    if (FTBL.EV_GetScreenFilmIsNotShowing()) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.shenToggleAreaFlags(0);
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    } else if (!FTBL.filmCheckFadeTrackColour()) {
#if defined(SHENMUE_EVENT_OVERLAY)
        if (eventArgument == 0)
            FTBL.EV_stepSetFlags(0x2dc, 0);
        else
            FTBL.EV_stepSetFlags(0x1ce, 0);
        FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, (void *)eventArgument);
#else
        EV_stepSetFlags(0, eventArgument != 0 ? 0x1ce : 0x2dc);
        ExecuteCurrAreaMainFunc_Wrapper(2, eventArgument);
#endif
        KES1_TRANSITION_PENDING = 0;
    }
}

void UpdateTamaTransparency() {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position;
    float distance, amplitude;
    FTBL.GetCharaPosition(SHEN_NAME4('R', 'Y', 'O', '_'), -1, &position, 0x40000000);
    distance = FTBL.GetSquaredDistanceXZ(&position, &tamaReferencePosition);
    if (distance < 3.0f)
        distance = 3.0f;
    else if (distance > 13.0f)
        distance = 13.0f;
    distance = 1.0f - (distance - 3.0f) * 0.1f;
    amplitude = (distance * 0.5f + 0.2f) * 0.5f;
    FTBL.SetScenarioCharacterTransparency(SHEN_NAME4('T', 'A', 'M', 'A'), FTBL.CalculateFixedSine(KES1_TAMA_PHASE) * amplitude + amplitude);
    KES1_TAMA_PHASE += (unsigned short)(((distance * 5.0f + 5.0f) * 8192.0f) / 45.0f);
#else
    Vector3 ryoPosition{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::World);
    const float dx = ryoPosition.x - tamaReferencePosition.x;
    const float dz = ryoPosition.z - tamaReferencePosition.z;
    const float squaredDistance = dx * dx + dz * dz;
    const float clampedDistance = squaredDistance >= 3.0f ? std::fmin(13.0f, squaredDistance) : 3.0f;
    const float distanceScale = 1.0f - (clampedDistance - 3.0f) * 0.1f;
    const float amplitude = (distanceScale * 0.5f + 0.2f) * 0.5f;
    SetScenarioCharacterTransparency(tama, sin_fixed16(static_cast<std::int16_t>(KES1_TAMA_PHASE)) * amplitude + amplitude);
    KES1_TAMA_PHASE += static_cast<std::int32_t>(((distanceScale * 5.0f + 5.0f) * 8192.0f) / 45.0f);
#endif
}

void CreateKES1Fire() {
    KES1_EFFECT_ACTIVE = 0;
    if (FTBL.EV_GetShenmueFlag(0x2b2) != 0) {
        KES1_EFFECT_ATTACHED = 1;
        return;
    }
    KES1_EFFECT_ACTIVE = 1;
    KES1_EFFECT_ATTACHED = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
    KES1_EFFECT = FTBL.CreateFireEffect(&firePosition, 0.4f, 0.7f, 0, 4);
    FTBL.SetFirePalette(KES1_EFFECT, firePalette);
#else
    KES1_EFFECT = static_cast<std::uint32_t>(CreateFireEffect(KES1_FIRE_POSITION, 0.4f, 0.7f, 0, 4));
    FTBL.SetFirePalette(KES1_EFFECT, KES1_FIRE_PALETTE.data());
#endif
    FTBL.SetFireFalloff(KES1_EFFECT, 3.14f, 4.94f, 0.78f);
    FTBL.SetFireFalloffScalar(KES1_EFFECT, -0.15f);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetParticleEffectPosition(4, KES1_EFFECT, &firePosition, &firePosition);
#else
    SetParticleEffectPosition(4, KES1_EFFECT, &KES1_FIRE_POSITION, &KES1_FIRE_POSITION);
#endif
    FTBL.ResumeParticleEffect(4, KES1_EFFECT);
    if (!FTBL.EV_GetShenmueFlag(0x1ce))
        return;
#if defined(SHENMUE_EVENT_OVERLAY)
    AttachKES1Fire();
#else
    SetFirePosition(KES1_EFFECT, 1, &KES1_SECOND_FIRE_POSITION);
    KES1_EFFECT_ATTACHED = 1;
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AttachKES1Fire(void) {
    FTBL.SetFirePosition(KES1_EFFECT, 1, &secondFirePosition);
    KES1_EFFECT_ATTACHED = 1;
}
void UpdateKES1TextureAnimation(void) {
    if (!FTBL.EV_GetPlayerFlag(11, 2228)) {
        FTBL.nlSetTexanimUV(0, 0.015f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(12));
        FTBL.nlSetTexanimUV(0, 0.090000004f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(13));
    }
}

#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const GameplayAreaDefinition areaKES1GameplayDefinition{
    .areaId = SHEN_NAME(KES1),
    .padding04 = 0,
    .init = &AreaKES1Init,
    .main = &AreaKES1Main,
    .cleanup = &AreaKES1Cleanup,
};

} // namespace shenmue::s2::areas

#endif
#undef KES1_EFFECT
#undef KES1_TRANSITION_PENDING
#undef KES1_EFFECT_ATTACHED
#undef KES1_EFFECT_ACTIVE
#undef KES1_TAMA_PHASE
