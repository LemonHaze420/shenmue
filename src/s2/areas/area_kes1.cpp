#include "s2/areas/area_kes1.hpp"

#include "s2/area_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scene_data.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"

#include <cmath>
#include <array>
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

void LoadKES1MapPackage() {
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("MPK01"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

void CreateKES1Fire() {
    state.effectActive = 0;
    if (EV_GetShenmueFlag(0x2b2) != 0) {
        state.effectAttached = 1;
        return;
    }
    state.effectActive = 1;
    state.effectAttached = 0;
    state.effect = static_cast<std::uint32_t>(CreateFireEffect(KES1_FIRE_POSITION, 0.4f, 0.7f, 0, 4));
    SetFirePalette(state.effect, KES1_FIRE_PALETTE.data());
    SetFireFalloff(state.effect, 3.14f, 4.94f, 0.78f);
    SetFireFalloffScalar(state.effect, -0.15f);
    SetParticleEffectPosition(4, state.effect, &KES1_FIRE_POSITION, &KES1_FIRE_POSITION);
    ResumeParticleEffect(4, state.effect);
    if (!EV_GetShenmueFlag(0x1ce))
        return;
    SetFirePosition(state.effect, 1, &KES1_SECOND_FIRE_POSITION);
    state.effectAttached = 1;
}

void RunKES1AreaTransition(std::uintptr_t eventArgument) {
    state.transitionPending = 1;
    if (EV_GetScreenFilmIsNotShowing()) {
        SetPlayerControlEnabled(0);
        shenToggleAreaFlags(0);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    } else if (!filmCheckFadeTrackColour()) {
        EV_stepSetFlags(0, eventArgument != 0 ? 0x1ce : 0x2dc);
        ExecuteCurrAreaMainFunc_Wrapper(2, eventArgument);
        state.transitionPending = 0;
    }
}

void UpdateTamaTransparency() {
    Vector3 ryoPosition{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::World);
    const float dx = ryoPosition.x - tamaReferencePosition.x;
    const float dz = ryoPosition.z - tamaReferencePosition.z;
    const float squaredDistance = dx * dx + dz * dz;
    const float clampedDistance = squaredDistance >= 3.0f ? std::fmin(13.0f, squaredDistance) : 3.0f;
    const float distanceScale = 1.0f - (clampedDistance - 3.0f) * 0.1f;
    const float amplitude = (distanceScale * 0.5f + 0.2f) * 0.5f;
    SetScenarioCharacterTransparency(tama, sin_fixed16(static_cast<std::int16_t>(state.tamaPhase)) * amplitude + amplitude);
    state.tamaPhase += static_cast<std::int32_t>(((distanceScale * 5.0f + 5.0f) * 8192.0f) / 45.0f);
}

void KES1MainLoopCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2228) == 0) {
        SetTextureAnimationUV(0.0f, 0.015f);
        ApplyTextureAnimationResource(12);
        SetTextureAnimationUV(0.0f, 0.090000004f);
        ApplyTextureAnimationResource(13);
    }

    if (ryoNotBusy() || state.transitionPending != 0) {
        DateTime dateTime{};
        GetCurrentDateTime(dateTime);
        if (dateTime.hour >= 23 && EV_GetShenmueFlag(0x1ce) != 0)
            RunKES1AreaTransition(2);
        else if (dateTime.hour >= 19 && EV_GetShenmueFlag(0x2dc) != 0)
            RunKES1AreaTransition(0);
    }

    if (EV_GetShenmueFlag(0xe3) != 0)
        UpdateTamaTransparency();

    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1757) != 0) {
        if (state.effectAttached == 0 && ryoNotBusy() && EV_GetShenmueFlag(0x1ce) != 0) {
            SetFirePosition(state.effect, 1, &KES1_SECOND_FIRE_POSITION);
            state.effectAttached = 1;
        }
        if (state.effectActive == 0 && EV_GetShenmueFlag(0x1ce) != 0) {
            ResumeParticleEffect(4, state.effect);
            state.effectActive = 1;
        }
    } else if (state.effectActive != 0 && EV_GetShenmueFlag(0x2dc) != 0) {
        SuspendParticleEffect(4, state.effect);
        state.effectActive = 0;
        return;
    }

    if (state.effectActive != 0 && EV_GetShenmueFlag(0x2b2) != 0) {
        state.effectActive = 0;
        SuspendParticleEffect(4, state.effect);
    }
}

} // namespace

void AreaKES1Init() {
    state = {};
}

void AreaKES1Main(std::uint32_t) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1757, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 1);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(KES1MainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(mainTaskToken.name));
    HLib::SetupTimedTask(task, 10);

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_SetWeatherInstruction(12, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    CreateKES1Fire();
    ExecuteCurrAreaMainFunc_Wrapper(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(-2)));
    LoadKES1MapPackage();
    ExecuteCurrAreaMainFunc_Wrapper(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(-1)));
    EV_NPC_NewLoadSw(1);
    state.transitionPending = 0;
    SetCharacterUngroundedShadowMode(1);
}

void AreaKES1Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaKES1GameplayDefinition{
    .areaId = SHEN_NAME(KES1),
    .padding04 = 0,
    .init = &AreaKES1Init,
    .main = &AreaKES1Main,
    .cleanup = &AreaKES1Cleanup,
};

} // namespace shenmue::s2::areas
