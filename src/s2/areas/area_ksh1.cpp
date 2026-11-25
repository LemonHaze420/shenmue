#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_api.h"
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"
enum { ryo = SHEN_NAME4('R', 'Y', 'O', '_'), shenhua = SHEN_NAME4('S', 'I', 'N', '_'), lightModeHome = 3, lightModeOutside = 4 };
static shen_name npcLoadCharacters[2] = {shenhua, 0xffffffff};
#define KSH1_HOME_RANGE(name, x, y, z) static Vector3 name = {x, y, z};
#include "ksh1_home_ranges.def"
#undef KSH1_HOME_RANGE
void AreaBaseSetCallback(int index, void (*callback)(unsigned int *parameters));
void AreaCleanup(int afternoon);
void LoadKSH1Audio(int afternoon);
void KSH1LightInitCallback(void *taskData);
void KSH1StartCallback(void *taskData);
void KSH1WaitCallback(void *taskData);
void KSH1MainLoopCallback(void *taskData);
void KSH1SetupLights(void);
void KSH1SetupLightsForCurrentTime(int *requestedMode);
void KSH1WaveCallback(void *taskData);
#else
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

#define KSH1_HOME_RANGE(name, x, y, z) constexpr Vector3 name{x, y, z};
#include "ksh1_home_ranges.def"
#undef KSH1_HOME_RANGE
constexpr Vector3 secondRangeStart{149.0f, 100.0f, 202.0f};
constexpr Vector3 secondRangeEnd{147.0f, 100.0f, 206.0f};

using HLibTask = HLib::Task;

void KSH1SetupLightsForCurrentTime(std::uint32_t *requestedMode);
void KSH1LightInitCallback(void *taskData);
void KSH1StartCallback(void *taskData);
void KSH1WaitCallback(void *taskData);
void KSH1MainLoopCallback(void *taskData);
void KSH1SetupLights(void);
void KSH1WaveCallback(void *taskData);
#endif

static int currentLightMode;
static HLibTask *waveTask;
#if defined(SHENMUE_EVENT_OVERLAY)
static int ksh1State;
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
static __inline__ void SetKSH1SearchCamera(unsigned int enabled) {
    struct {
        unsigned int first, second;
    } parameters;
    parameters.first = enabled ? (enabled != 0) : enabled;
    parameters.second = 0;
    FTBL.camCtrlUtil(17, (int)&parameters);
}
#else
void SetKSH1SearchCamera(std::uint32_t enabled) {
    const CameraUtilityParameters parameters{enabled, 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
}
} // namespace

void AreaKSH1Init() {
    currentLightMode = 0;
    waveTask = nullptr;
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
#else
void AreaKSH1Main(std::uint32_t startParameters)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    HLibTask *startTask;
    HLibTask *lightTask;
    startTask = 0;
#endif
    currentLightMode = 0;
    SetKSH1SearchCamera(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    SHENMUE_EVENT_USE(startTask);
    ksh1State = 0;
    if (!startParameters)
        startTask = FTBL.HLib_EnqueueTaskWithoutParameter(KSH1StartCallback, 4, 11, SHEN_NAME4('k', 'r', 'm', '3'));
    FTBL.EV_SetPlayerFlag(12, 1495, 0);
    FTBL.HLib_EnqueueTaskWithoutParameter(KSH1MainLoopCallback, 4, 11, SHEN_NAME4('k', 's', 'h', '1'));
    waveTask = FTBL.HLib_EnqueueTaskWithoutParameter(KSH1WaveCallback, 3, 2, SHEN_NAME4('k', 's', 'w', 'v'));
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0);
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, (void *)-1);
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KSH1LightInitCallback, 4, 11, SHEN_NAME4('s', 'l', 'e', 'p')), 1);
    AreaBaseSetCallback(1, (void (*)(unsigned int *))KSH1SetupLightsForCurrentTime);
    FTBL.ConfigureScenarioNodeMotion(SHEN_NAME4('u', 's', 'a', 'g'), 40965, 0, 32767, 0, 1, 0);
    FTBL.ConfigureScenarioNodeMotion(SHEN_NAME4('u', 's', 'a', 'g'), 0, -1, -1, -1, 32767, 4);
    FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('u', 's', 'a', 'g'));
    FTBL.srchSetTargetEnabled(SHEN_NAME4('M', 'A', '1', '7'), 0);
    FTBL.srchSetTargetEnabled(SHEN_NAME4('M', 'A', '1', '8'), 0);
    FTBL.srchSetTargetEnabled(SHEN_NAME4('M', 'A', '0', '9'), 0);
    if (!startParameters) {
        FTBL.EV_NPC_NewLoadSw_0(0);
        Queue_EVT_UTL_LoadNPCChara_3(npcLoadCharacters, startTask);
        FTBL.HLib_SetupTimedTask(startTask, 0);
    } else
        FTBL.PTYCUtils(shenhua, 3, 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(shenhua), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ryo), 0);
#else
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
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
void KSH1LightInitCallback(void *taskData)
#else
void KSH1LightInitCallback(void *taskData)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    KSH1SetupLights();
    FTBL.HLib_DestroyCurrentTask();
#else
    KSH1SetupLights();
    HLib::DestroyCurrentTask();
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
#else
void AreaKSH1Cleanup(std::int32_t afternoon, std::uint32_t)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    KSH1SetupLights();
    LoadKSH1Audio(afternoon);
#else
    KSH1SetupLights();
    EV_LoadAudio("KSH1_000", afternoon ? 1 : 0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void LoadKSH1Audio(int afternoon) {
    if (afternoon)
        FTBL.EV_LoadAudio("KSH1_000", 1);
    else
        FTBL.EV_LoadAudio("KSH1_000", 0);
}
#else
namespace {
#endif

void KSH1StartCallback(void *taskData) {
    const Vector3 position = {214.8f, 91.5f, 235.5f};
    const Vector3i rotation = {0, 910, 0};
#if defined(SHENMUE_EVENT_OVERLAY)

    FTBL.ActivatePartyCompanion(shenhua);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.PTYCUtils(shenhua, 3, 1);
    FTBL.EV_AT_SetPos(shenhua, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(shenhua, 0x38000000, &rotation, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(shenhua), 17, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(shenhua), 71, 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(shenhua), 13, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(shenhua), 0x9502, 0);
    FTBL.PTYCUtils(shenhua, 3, 0);
    FTBL.shenToggleAreaFlags(3);
    FTBL.SetPlayerGameFlags(64, 1);
    FTBL.HLib_SetCurrentTaskTo(KSH1WaitCallback);
    FTBL.HLib_SetupTaskDelay(60);
#else

    EV_ActivatePartyCompanion(shenhua);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(shenhua, 3, 1);
    EV_AT_SetPos(shenhua, relativeXyz, position);
    EV_AT_SetAngY(shenhua, relativeXyz, rotation, nullptr);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    mtwkSetRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(shenhua), shenhuaAreaMotion, MotionRegion::LowerBody);
    EV_PTYCUtils(shenhua, 3, 0);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
    HLib::SetCurrentTaskTo(KSH1WaitCallback);
    HLib::SetupTaskDelay(60);
#endif
}

void KSH1WaitCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.IsScenarioBusy()) {
        FTBL.DebugLogToFileRemoved("player mode clear\n");
        FTBL.shenToggleAreaFlags(0);
        FTBL.HLib_DestroyCurrentTask();
    }
#else
    if (IsScenarioBusy()) {
        shenToggleAreaFlags(0);
        HLib::DestroyCurrentTask();
    }
#endif
}

void KSH1MainLoopCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.ryoNotBusy())
        KSH1SetupLights();
    if (FTBL.EV_GetShenmueFlag(0xf3))
        FTBL.HLib_SetupTimedTask(waveTask, 0);
#else
    if (ryoNotBusy()) {
        KSH1SetupLights();
    }
    if (EV_GetShenmueFlag(0xf3)) {
        HLib::SetupTimedTask(waveTask, 0);
    }
#endif
}

void KSH1SetupLights(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndA, 0) ||
        FTBL.TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndB, 0)) {
#else
    if (TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndA, shen_name{}) ||
        TestCharaPositionWithinRange(ryo, &homeRangeStart, &homeRangeEndB, shen_name{}) ||
        TestCharaPositionWithinRange(ryo, &secondRangeStart, &secondRangeEnd, shen_name{})) {
#endif
        if (currentLightMode != lightModeHome) {
#if defined(SHENMUE_EVENT_OVERLAY)
            int mode = lightModeHome;
#else
            std::uint32_t mode = lightModeHome;
#endif
            KSH1SetupLightsForCurrentTime(&mode);
        }
    } else if (currentLightMode != lightModeOutside) {
#if defined(SHENMUE_EVENT_OVERLAY)
        int mode = lightModeOutside;
#else
        std::uint32_t mode = lightModeOutside;
#endif
        KSH1SetupLightsForCurrentTime(&mode);
    }
}

#if defined(SHENMUE_EVENT_OVERLAY)
void KSH1SetupLightsForCurrentTime(int *requestedMode)
#else
void KSH1SetupLightsForCurrentTime(std::uint32_t *requestedMode)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    struct {
        float x, y, z, w;
    } lighting = {1, .8f, .5f, .3f};
    int previousMode;
    FTBL.DebugLogToFileRemoved("call ov1 light %d\n", *requestedMode);
    previousMode = currentLightMode;
    if (previousMode) {
        if (*requestedMode > 2) {
            if (previousMode == *requestedMode) {
                FTBL.DebugLogToFileRemoved("KSH1 Light Setup!! no set\n");
                return;
            }
        } else
            *requestedMode += 2;
    }
    currentLightMode = *requestedMode;
    FTBL.DebugLogToFileRemoved("call ov1 light %d\n", currentLightMode);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.SetDefaultVS();
    FTBL.AssignLightParams(0, 0);
    FTBL.AssignLightParams(0, 1);
    FTBL.AssignLightParams(0, 2);
    FTBL.AssignLightParams(1, 4);
    FTBL.AssignLightParams(0, 5);
    FTBL.SetMapModelEnabled(1, 0);
    FTBL.SetMapModelEnabled(1, 1);
    FTBL.SetMapModelEnabled(1, 2);
    FTBL.SetMapModelEnabled(1, 4);
    FTBL.SetMapModelEnabled(1, 5);
#else
    if (currentLightMode != 0) {
        if (*requestedMode < static_cast<std::uint32_t>(lightModeHome)) {
            *requestedMode += 2;
        } else if (currentLightMode == static_cast<std::int32_t>(*requestedMode)) {
            return;
        }
    }
    currentLightMode = static_cast<std::int32_t>(*requestedMode);

    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
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
#endif
    if (*requestedMode == lightModeHome) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetNewWindPhaseAndSpeed(0, 0);
        FTBL.HLib_SetupTimedTask(waveTask, 0);
        FTBL.EV_SetWeatherInstruction(0x780, 0);
        FTBL.EV_SetWeatherInstruction(0xc, 0);
        FTBL.SetObjectFlagByIndex(4, 0);
#else
        SetNewWindPhaseAndSpeed(0, 0.0f);
        HLib::SetupTimedTask(waveTask, 0);
        EV_SetWeatherInstruction(enableFogEffect, 0);
        EV_SetWeatherInstruction(updateGlobalLighting, 0);
        EV_SetObjectFlagByIndex(4, 0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        if ((unsigned char)(dateTime.hour - 5) > 13) {
#else
        if (static_cast<std::uint8_t>(dateTime.hour - 5) > 13U) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
            FTBL.DebugLogToFileRemoved("KSH1 Light Setup!!  HOME - NIGHT.\n");
            FTBL.EV_lightLoad(SHEN_NAME4('N', 'I', 'G', 'H'));
            FTBL.AssignLightProgram(0);
            FTBL.AssignLightProgram(1);
            FTBL.AssignLightProgram(5);
#else
            EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 343, "KSH1 Light Setup!!  HOME - NIGHT.\n");
            EV_lightLoad(nightLight);
            AssignLightProgram(0);
            AssignLightProgram(1);
            AssignLightProgram(5);
#endif
        } else {
#if defined(SHENMUE_EVENT_OVERLAY)
            FTBL.DebugLogToFileRemoved("KSH1 Light Setup!!  HOME - DAY.\n");
            FTBL.AssignLightParams(1, 0);
            FTBL.AssignLightParams(1, 1);
            FTBL.AssignLightParams(1, 5);
#else
            EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 349, "KSH1 Light Setup!!  HOME - DAY.\n");
            EV_AssignLightParams(1, 0);
            EV_AssignLightParams(1, 1);
            EV_AssignLightParams(1, 5);
#endif
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_lightLoad(SHEN_NAME4('H', 'O', 'M', 'E'));
        FTBL.SetupLightConfig();
        FTBL.AssignLightProgram(3);
#else
        EV_lightLoad(homeLight);
        SetupLightConfig();
        AssignLightProgram(3);
#endif
        SetKSH1SearchCamera(1);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(shenhua), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ryo), 1);
#else
        SetPluralControlFor(ctrlGetCharaByName(shenhua), 0);
        SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
#endif
        return;
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SyncWindWithWDATWeatherState();
    FTBL.HLib_ResolveDeferredCallback(waveTask);
    FTBL.SetObjectFlagByIndex(4, 1);
    FTBL.EV_lightLoad(SHEN_NAME4('H', 'O', 'M', 'E'));
    FTBL.AssignLightProgram(3);
#else
    SyncWindWithWDATWeatherState();
    HLib::ResolveDeferredCallback(waveTask);
    EV_SetObjectFlagByIndex(4, 1);
    EV_lightLoad(homeLight);
    AssignLightProgram(3);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if ((unsigned char)(dateTime.hour - 5) > 13) {
#else
    if (static_cast<std::uint8_t>(dateTime.hour - 5) > 13U) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("KSH1 Light Setup!!  OUT - NIGHT.\n");
        FTBL.EV_SetWeatherInstruction(0x780, 0);
        FTBL.EV_SetWeatherInstruction(0xf, 0);
        FTBL.EV_SetWeatherInstruction(0xc, 0);
        FTBL.EV_lightLoad(SHEN_NAME4('N', 'I', 'G', 'H'));
        FTBL.SetupLightConfig();
        FTBL.AssignLightProgram(0);
        FTBL.AssignLightProgram(1);
        FTBL.AssignLightProgram(5);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(shenhua), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ryo), 1);
#else
        EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 369, "KSH1 Light Setup!!  OUT - NIGHT.\n");
        EV_SetWeatherInstruction(enableFogEffect, 0);
        EV_SetWeatherInstruction(updateOutdoorWeather, 0);
        EV_SetWeatherInstruction(updateGlobalLighting, 0);
        EV_lightLoad(nightLight);
        SetupLightConfig();
        AssignLightProgram(0);
        AssignLightProgram(1);
        AssignLightProgram(5);
#endif
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("KSH1 Light Setup!!  OUT - DAY.\n");
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.EV_SetWeatherInstruction(0x780, 1);
        FTBL.EV_SetWeatherInstruction(0xf, 1);
        FTBL.EV_SetWeatherInstruction(0xc, 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(shenhua), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ryo), 0);
#else
        EV_debug_log("KSH1\\eKSH1_et_KSH1.c", 381, "KSH1 Light Setup!!  OUT - DAY.\n");
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetWeatherInstruction(enableFogEffect, 1);
        EV_SetWeatherInstruction(updateOutdoorWeather, 1);
        EV_SetWeatherInstruction(updateGlobalLighting, 1);
#endif
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    SetPluralControlFor(ctrlGetCharaByName(shenhua), 0);
    SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
#endif
    SetKSH1SearchCamera(0);
}

void KSH1WaveCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.nlSetTexanimUV(0, -.0046999999f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(4));
#else
    SetTextureAnimationUV(0.0f, -0.0046999999f);
    ApplyTextureAnimationResource(4);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const GameplayAreaDefinition areaKSH1GameplayDefinition{SHEN_NAME(KSH1), 0, AreaKSH1Init, AreaKSH1Main, AreaKSH1Cleanup};

} // namespace shenmue::s2::areas
#endif
