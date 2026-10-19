#include "s2/debug.hpp"
#include "s2/areas/area_ksh1.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/search.hpp"
#include "s2/weather.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"

#include <array>

#include <limits>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name shenhua{SHEN_NAME(SIN_)};
constexpr std::array<shen_name, 2> npcLoadCharacters{shenhua, shen_name{0xffffffffU}};
constexpr shen_name rabbit{SHEN_NAME(usag)};
constexpr shen_name startTaskToken{SHEN_NAME(krm3)};
constexpr shen_name mainTaskToken{SHEN_NAME(ksh1)};
constexpr shen_name waveTaskToken{SHEN_NAME(kswv)};
constexpr shen_name sleepTaskToken{SHEN_NAME(slep)};
constexpr shen_name loadTaskToken{SHEN_NAME(load)};
constexpr shen_name searchMA17{SHEN_NAME(MA17)};
constexpr shen_name searchMA18{SHEN_NAME(MA18)};
constexpr shen_name searchMA09{SHEN_NAME(MA09)};
constexpr shen_name nightLight{SHEN_NAME(NIGH)};
constexpr shen_name homeLight{SHEN_NAME(HOME)};
constexpr std::uint32_t relativeXyz = 0x38000000U;
constexpr std::uint32_t shenhuaAreaMotion = 0x9502U;
constexpr std::int16_t areaPlayerFlag = 1495;
constexpr std::int32_t enableFogEffect = 0x780;
constexpr std::int32_t updateGlobalLighting = 0x0c;
constexpr std::int32_t updateOutdoorWeather = 0x0f;
constexpr std::int32_t lightModeHome = 3;
constexpr std::int32_t lightModeOutside = 4;

constexpr Vector3 homeRangeStart{140.60001f, 100.0f, 215.60001f};
constexpr Vector3 homeRangeEndA{148.3f, 100.0f, 201.60001f};
constexpr Vector3 homeRangeEndB{151.89999f, 100.0f, 209.7f};
constexpr Vector3 secondRangeStart{149.0f, 100.0f, 202.0f};
constexpr Vector3 secondRangeEnd{147.0f, 100.0f, 206.0f};

std::int32_t currentLightMode{};
HLib::Task *waveTask{};

void KSH1SetupLightsForCurrentTime(std::uint32_t *requestedMode);

void SetKSH1SearchCamera(std::uint32_t enabled) {
    const CameraUtilityParameters parameters{enabled, 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
}

void KSH1WaitCallback(void *) {
    if (IsScenarioBusy()) {
        shenToggleAreaFlags(0);
        HLib::DestroyCurrentTask();
    }
}

void KSH1StartCallback(void *) {
    const Vector3 position{214.8f, 91.5f, 235.5f};
    const Vector3i rotation{0, 910, 0};

    EV_ActivatePartyCompanion(shenhua);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(shenhua, 3, 1);
    EV_AT_SetPos(shenhua, relativeXyz, position);
    EV_AT_SetAngY(shenhua, relativeXyz, rotation, nullptr);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(shenhua), shenhuaAreaMotion, MotionRegion::LowerBody);
    EV_PTYCUtils(shenhua, 3, 0);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
    HLib::SetCurrentTaskTo(KSH1WaitCallback);
    HLib::SetupTaskDelay(60);
}

void KSH1SetupLights() {
    if (TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndA, shen_name{}) ||
        TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndB, shen_name{}) ||
        TestCharaPositionWithinRange(ryo, &secondRangeStart, &secondRangeEnd, shen_name{})) {
        if (currentLightMode != lightModeHome) {
            std::uint32_t mode = lightModeHome;
            KSH1SetupLightsForCurrentTime(&mode);
        }
    } else if (currentLightMode != lightModeOutside) {
        std::uint32_t mode = lightModeOutside;
        KSH1SetupLightsForCurrentTime(&mode);
    }
}

void KSH1MainLoopCallback(void *) {
    if (ryoNotBusy()) {
        KSH1SetupLights();
    }
    if (EV_GetShenmueFlag(0xf3)) {
        HLib::SetupTimedTask(waveTask, 0);
    }
}

void KSH1LightInitCallback(void *) {
    KSH1SetupLights();
    HLib::DestroyCurrentTask();
}

void KSH1WaveCallback(void *) {
    SetTextureAnimationUV(0.0f, -0.0046999999f);
    ApplyTextureAnimationResource(4);
}

void KSH1SetupLightsForCurrentTime(std::uint32_t *requestedMode) {
    if (currentLightMode != 0) {
        if (*requestedMode < static_cast<std::uint32_t>(lightModeHome)) {
            *requestedMode += 2;
        } else if (currentLightMode == static_cast<std::int32_t>(*requestedMode)) {
            return;
        }
    }
    currentLightMode = static_cast<std::int32_t>(*requestedMode);

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    SetMapModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(0, 0);
    EV_AssignLightParams(0, 1);
    EV_AssignLightParams(0, 2);
    EV_AssignLightParams(1, 4);
    EV_AssignLightParams(0, 5);
    SetMapModelCommandRoutingEnabled(1, 0);
    SetMapModelCommandRoutingEnabled(1, 1);
    SetMapModelCommandRoutingEnabled(1, 2);
    SetMapModelCommandRoutingEnabled(1, 4);
    SetMapModelCommandRoutingEnabled(1, 5);

    if (*requestedMode == lightModeHome) {
        SetNewWindPhaseAndSpeed(0, 0.0f);
        HLib::SetupTimedTask(waveTask, 0);
        EV_SetWeatherInstruction(enableFogEffect, 0);
        EV_SetWeatherInstruction(updateGlobalLighting, 0);
        EV_SetObjectFlagByIndex(4, 0);
        if (static_cast<std::uint8_t>(dateTime.hour - 5) > 13U) {
            EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 343, "KSH1 Light Setup!!  HOME - NIGHT.\n");
            EV_lightLoad(nightLight);
            AssignLightProgram(0);
            AssignLightProgram(1);
            AssignLightProgram(5);
        } else {
            EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 349, "KSH1 Light Setup!!  HOME - DAY.\n");
            EV_AssignLightParams(1, 0);
            EV_AssignLightParams(1, 1);
            EV_AssignLightParams(1, 5);
        }
        EV_lightLoad(homeLight);
        SetupLightConfig();
        AssignLightProgram(3);
        SetKSH1SearchCamera(1);
        SetPluralControlFor(ctrlGetCharaByName(shenhua), 0);
        SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
        return;
    }

    SyncWindWithWDATWeatherState();
    HLib::ResolveDeferredCallback(waveTask);
    EV_SetObjectFlagByIndex(4, 1);
    EV_lightLoad(homeLight);
    AssignLightProgram(3);
    if (static_cast<std::uint8_t>(dateTime.hour - 5) > 13U) {
        EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 369, "KSH1 Light Setup!!  OUT - NIGHT.\n");
        EV_SetWeatherInstruction(enableFogEffect, 0);
        EV_SetWeatherInstruction(updateOutdoorWeather, 0);
        EV_SetWeatherInstruction(updateGlobalLighting, 0);
        EV_lightLoad(nightLight);
        SetupLightConfig();
        AssignLightProgram(0);
        AssignLightProgram(1);
        AssignLightProgram(5);
    } else {
        EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 381, "KSH1 Light Setup!!  OUT - DAY.\n");
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetWeatherInstruction(enableFogEffect, 1);
        EV_SetWeatherInstruction(updateOutdoorWeather, 1);
        EV_SetWeatherInstruction(updateGlobalLighting, 1);
    }
    SetPluralControlFor(ctrlGetCharaByName(shenhua), 0);
    SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
    SetKSH1SearchCamera(0);
}

} // namespace

void AreaKSH1Init() {
    currentLightMode = 0;
    waveTask = nullptr;
}

void AreaKSH1Main(std::uint32_t startParameters) {
    currentLightMode = 0;
    SetKSH1SearchCamera(0);

    HLib::Task *startTask = nullptr;
    if (startParameters == 0) {
        startTask = HLib::EnqueueTaskWithoutParameter(KSH1StartCallback, 4, 0x0b, static_cast<std::int32_t>(startTaskToken.name));
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, areaPlayerFlag, 0);
    HLib::EnqueueTaskWithoutParameter(KSH1MainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(mainTaskToken.name));
    waveTask = HLib::EnqueueTaskWithoutParameter(KSH1WaveCallback, 3, 2, static_cast<std::int32_t>(waveTaskToken.name));
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    ExecuteCurrAreaMainFunc_Wrapper(2, std::numeric_limits<std::uintptr_t>::max());
    HLib::Task *const lightTask = HLib::EnqueueTaskWithoutParameter(KSH1LightInitCallback, 4, 0x0b, static_cast<std::int32_t>(sleepTaskToken.name));
    HLib::SetupTimedTask(lightTask, 1);
    areaBaseCallbacks[0] = KSH1SetupLightsForCurrentTime;
    ConfigureScenarioNodeMotion(rabbit, 40965, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    ConfigureScenarioNodeMotion(rabbit, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    DisableScenarioCharacterDisplay(rabbit);
    srchSetTargetEnabled(searchMA17, 0);
    srchSetTargetEnabled(searchMA18, 0);
    srchSetTargetEnabled(searchMA09, 0);

    if (startParameters != 0) {
        EV_PTYCUtils(shenhua, 3, 1);
    } else {
        EV_NPC_NewLoadSw(0);
        AssignAreaNpcLoadContext(npcLoadCharacters.data(), startTask);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        if (startTask) {
            HLib::SetupTimedTask(startTask, 0);
        }
        HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(loadTaskToken.name));
        HLib::SetupTimedTask(startTask, 0);
    }

    SetPluralControlFor(ctrlGetCharaByName(shenhua), 0);
    SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
}

void AreaKSH1Cleanup(std::int32_t afternoon, std::uint32_t) {
    KSH1SetupLights();
    EV_LoadAudio("KSH1_000", afternoon ? 1 : 0);
}

const GameplayAreaDefinition areaKSH1GameplayDefinition{SHEN_NAME(KSH1), 0, AreaKSH1Init, AreaKSH1Main, AreaKSH1Cleanup};

} // namespace shenmue::s2::areas
