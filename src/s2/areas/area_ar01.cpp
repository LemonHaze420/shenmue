#include "s2/areas/area_ar01.hpp"
#include "s2/lighting.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"

#include "s2/event_runtime.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/camera.hpp"
#include "s2/player.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/areas/area_ab00.hpp"
#include "s2/flag.hpp"
#include "s2/memo.hpp"
#include "s2/sleep_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/map_assets.hpp"
#include "s2/particle_effects.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <cstddef>
#include <cstdio>

namespace shenmue::s2::areas {
namespace {

std::uint8_t sleepLatch{};

SleepTriggerBox sleepTriggerBox{
    {177.0f, 90.0f, 305.0f}, 187.2f, {90.0f, 305.0f, 177.0f, 110.0f}, 313.0f, {187.2f, 110.0f, 313.0f}, {180.61f, 101.0f, 311.07f}, 1};

std::array<std::byte, 0x100> waveDistance{}, waveKernelA{}, waveKernelB{};

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int16_t innAreaPlayerFlag = 2045;
constexpr std::int32_t enableFogEffect = 0x780;
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name a005{SHEN_NAME(a005)};
constexpr Vector3 insideInnPosition{175.0f, 100.22f, 308.0f};
constexpr Vector3 outsideInnPosition{172.0f, 100.22f, 309.89999f};
constexpr std::uint16_t insideInnYaw = 0;
constexpr std::uint16_t outsideInnYaw = 0x4000;
constexpr std::int32_t changeInnTaskToken = static_cast<std::int32_t>(make_shen_name('i', 'n', 'n', ' '));
constexpr std::int16_t forcedSleepPlayerFlag = 957;
constexpr std::int16_t sleepPromptActivePlayerFlag = 1022;

constexpr shen_name dayLight{SHEN_NAME(0001)};
constexpr shen_name nightLight{SHEN_NAME(0000)};

constexpr Vector2 rainClip[]{{188.0f, 302.0f}, {190.0f, 316.0f}, {169.0f, 313.0f}, {172.0f, 301.0f}, {99.0f, 110.0f}};

void SetAR01InnSide(bool insideInn) {
    camCtrlSetContext(insideInn ? 1 : 0, 0);
    if (!insideInn) {
        EV_EnableCharacterDisplay(ryo);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, innAreaPlayerFlag, insideInn);
}

void eAR01_ApplyLightingAndAudio(std::int32_t nightTime) {
    EV_SetModelDataDefaultVertexShader();
    if (nightTime) {
        EV_SetObjectFlagByIndex(4, 1);
        EV_SetObjectFlagByIndex(5, 0);
        EV_lightLoad(nightLight);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_LoadAudio("AR01_000", 1);
        return;
    }

    EV_SetObjectFlagByIndex(4, 0);
    EV_lightLoad(dayLight);
    AssignLightProgram(0);
    EV_ReinitLights(1);
    std::int32_t fog = 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, innAreaPlayerFlag)) {
        EV_SetObjectFlagByIndex(5, GetCurrentWeather() ? 0 : 1);
    } else {
        EV_SetObjectFlagByIndex(5, 1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        fog = 1;
    }
    EV_SetWeatherInstruction(enableFogEffect, fog);
    EV_LoadAudio("AR01_000", 0);
}

void eAR01_SleepData_StartSleepAuthor(void *);
void eAR01_SleepData_SelectBadEnding(void *);
void eAR01_SleepData_StartWakeAuthor(void *);
void eAR01_SleepData_PlaceAfterWake(void *);
void eAR01_SleepData_Finish(void *);


void eAR01_SleepData_LoadPak(void *) {
    constexpr shen_name actors[]{shen_name{}};
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, forcedSleepPlayerFlag, 0);
    sleepLatch = 0;
    EV_stepSetFlags(0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    QueueSleepPackageActors("SLEP", actors, eAR01_SleepData_StartSleepAuthor);
}


void eAR01_SleepData_StartSleepAuthor(void *) {
    constexpr shen_name authors[]{SHEN_NAME(SLP0), SHEN_NAME(SLP1), SHEN_NAME(SLP2)};
    EV_LoadCutsceneFromCurrentScene("0006A000");
    EV_spmBeginSampleExecute();
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    sleepEventData.flags = 0;
    sleepEventData.continuation = eAR01_SleepData_SelectBadEnding;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if ((sleepEventData.flags & 1U) != 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if ((sleepEventData.flags & 0x40U) == 0) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      AuthSceneCtrl::UnusedAuthorFlag);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}


void eAR01_SleepData_SelectBadEnding(void *) {
    SelectBadEndingState(eAR01_SleepData_StartWakeAuthor);
}


void eAR01_SleepData_StartWakeAuthor(void *) {
    constexpr shen_name authors[]{SHEN_NAME(WUP0), SHEN_NAME(WUP1), SHEN_NAME(WUP2)};
    eAR01_ApplyLightingAndAudio(0);
    sleepEventData.flags = 0;
    sleepEventData.continuation = eAR01_SleepData_PlaceAfterWake;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if ((sleepEventData.flags & 1U) != 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if ((sleepEventData.flags & 0x40U) == 0) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      AuthSceneCtrl::UnusedAuthorFlag);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}


void eAR01_SleepData_PlaceAfterWake(void *) {
    using enum CameraState;
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {180.89999f, 100.22f, 309.5f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 10922, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    sleepEventData.flags = 1;
    sleepEventData.continuation = eAR01_SleepData_Finish;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    RefreshPadTask(2, 1);
    HLib::SetCurrentTaskTo(SleepMemoWaitCallback);
}


void eAR01_SleepData_Finish(void *) {
    EV_LoadCutsceneFromCurrentScene("AR01");
    EV_LoadAudio("AR01_000", 0);
    EV_spmBeginSampleExecute();
    FreeSleepPackageActors();
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}


void eAR01_EVT_CallSleepEvent2_Callback(void *) {
    QueueSleepCallback(eAR01_SleepData_LoadPak, eAB00_EVT_SleepEvent_SleepFunc2, 501);
}

void Queue_eAR01_EVT_CallSleepEvent2() {
    SetPlayerControlEnabled(0);
    EV_setHoldStatesFlags(0);
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    CameraUtilityParameters parameters{1, 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
    HLib::EnqueueTaskWithoutParameter(eAR01_EVT_CallSleepEvent2_Callback, 4, 0x0b, static_cast<std::int32_t>(SLEEP_EVENT_TASK.name));
}

void AR01_CheckSleep() {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, innAreaPlayerFlag)) {
        return;
    }

    if (shenTrySleepTrigger(&sleepTriggerBox) == 0) {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, forcedSleepPlayerFlag)) {
            sleepLatch = 1;
            return;
        }
        if (sleepLatch == 0 || !ryoNotBusy())
            return;
    }

    sleepLatch = 0;
    EV_stepSetFlags(0, 0);
    Queue_eAR01_EVT_CallSleepEvent2();
}

void eAR01_EVT_AR_mainloop_Callback(void *) {
    if (ryoNotBusy())
        AR01_CheckSleep();
}

void eAR01_EVT_ChangeInn3_Callback(void *) {
    if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
        return;

    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}

void eAR01_EVT_ChangeInn2_Callback(void *) {
    if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
        return;

    eAR01_ApplyLightingAndAudio(EV_GetShenmueFlag(nightTimeFlag) ? 1 : 0);

    const bool isInsideInn = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, innAreaPlayerFlag) != 0;
    Vector3i rotation{};
    if (isInsideInn) {
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, insideInnPosition);
        GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::World);
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 60);
    } else {
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, outsideInnPosition);
        GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::World);
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
    }
    EV_SetGlobalShadowLightingEnabled(!isInsideInn);

    camCtrlSetFovTransitionTo(true);
    HLib::SetCurrentTaskTo(eAR01_EVT_ChangeInn3_Callback);
}

void eAR01_EVT_ChangeInn_Callback(void *) {
    MotionWork *const motionWork = g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr;
    if (motionWork && mtwkSetExtraFlag(motionWork, 58, 1) != 0) {
        return;
    }
    HLib::SetCurrentTaskTo(eAR01_EVT_ChangeInn2_Callback);
}

void Queue_eAR01_EVT_ChangeInn() {
    SetPlayerControlEnabled(0);
    const bool isInsideInn = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, innAreaPlayerFlag) != 0;
    const Vector3 &targetPosition = isInsideInn ? outsideInnPosition : insideInnPosition;
    const std::uint16_t targetYaw = isInsideInn ? outsideInnYaw : insideInnYaw;
    const bool destinationIsInsideInn = !isInsideInn;

    MotionWork *const motionWork = g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr;
    BeginMotionPositionTransition(motionWork, targetPosition, targetYaw);

    SetAR01InnSide(destinationIsInsideInn);

    CompleteMotionPositionTransitionRequest(motionWork);
    HLib::EnqueueTaskWithoutParameter(eAR01_EVT_ChangeInn_Callback, 6, 0x0b, changeInnTaskToken);
    WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 90);
}

void QueueAR01InnMapEvent(std::uintptr_t) {
    Queue_eAR01_EVT_ChangeInn();
}

constexpr std::array ar01MapEventCallbacks{
    MapEventCallbackEntry{QueueAR01InnMapEvent, 85, 0, 0},
    MapEventCallbackEntry{nullptr, -1, 0, 0},
};


void eAR01_SetupSleepEvents(std::uint32_t startParameters) {
    sleepTriggerBox.state = 1;
    if (startParameters) {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, sleepPromptActivePlayerFlag)) {
            areaFlags |= AreaFlag_KeepPlayerControlLocked;
            sleepLatch = 0;
            Queue_eAR01_EVT_CallSleepEvent2();
        }
    } else if (g_entryPoint == 86) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, forcedSleepPlayerFlag, 1);
        EV_stepSetFlags(0, 500);
        if (shenGetMinutesUntilNextFrameMinute()) {
            EV_setHoldStatesFlags(2);
            EV_setHoldStatesFlags(1);
        }
        areaFlags |= AreaFlag_KeepPlayerControlLocked;
        sleepTriggerBox.state = 1;
        Queue_eAR01_EVT_CallSleepEvent2();
    }
}


void eAR01_ET_WAVE_AR01_Callback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.75f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.75f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.8999999761581421f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f * 0.30000001192092896f},
        42.540000915527344f, 1.0f);
    SetTextureAnimationUV(0.00019999999494757503f, 0.00019999999494757503f);
    ApplyTextureAnimationResource(9);
    SetMapModelSlotEnabled(8, 0);
    SetMapModelSlotEnabled(9, 0);
    AdvanceWaterTextureAnimation();
}


void eAR01_EVT_Init() {
    waveDistance.fill(std::byte{});
    waveKernelA.fill(std::byte{});
    waveKernelB.fill(std::byte{});
    sleepLatch = 0;
}

void eAR01_EVT_Main(std::uint32_t startParameters) {
    if (startParameters) {
        EV_ApplyTranslationClockVisibility(startParameters);
    }
    Vector3 ryoPosition{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::Local);
    const bool isInsideInn = ryoPosition.x > 172.86f && ryoPosition.x < 186.87f && ryoPosition.z > 302.60001f && ryoPosition.z < 313.23999f;
    SetAR01InnSide(isInsideInn);
    EV_SetGlobalShadowLightingEnabled(!isInsideInn);

    HLib::EnqueueTaskWithoutParameter(eAR01_EVT_AR_mainloop_Callback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(main)));

    if (EV_GetShenmueFlag(nightTimeFlag)) {
        eAR01_ApplyLightingAndAudio(1);
    } else {
        eAR01_ApplyLightingAndAudio(0);
    }
    EV_GetMapEventTask(ar01MapEventCallbacks.data());
    InitializeWaterTextureAnimation(1);
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, waveDistance)));
    void *const kernelB = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, waveKernelB));
    void *const kernelA = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, waveKernelA));
    ApplyFft(kernelA, kernelB);
    FinalizeFft(0.0010000000474974513f, 0.0010000000474974513f);
    SetWaveLighting(0.10000000149011612f, {0.05979999899864197f, 0.9768000245094299f, 0.2054000049829483f});
    HLib::EnqueueTaskWithoutParameter(eAR01_ET_WAVE_AR01_Callback, 3, 2, static_cast<std::int32_t>(AREA_EVENT_TASK.name));
    SetMapModelVertexShaderType(10, 1);
    SetMapModelVertexShaderType(11, 1);
    eAR01_SetupSleepEvents(startParameters);
    ApplyWeatherRainAndRippleClip(rainClip);
    EV_ToggleCharacterFlag(a005);
    EV_DisableCharacterDisplay(a005);
    EV_SetCharacterAlphaEnabled(a005, true);
}

void eAR01_EVT_Cleanup(std::int32_t nightTime, std::uint32_t) {
    eAR01_ApplyLightingAndAudio(nightTime);
}

} // namespace

const GameplayAreaDefinition areaAR01GameplayDefinition{SHEN_NAME(AR01), 0, eAR01_EVT_Init, eAR01_EVT_Main, eAR01_EVT_Cleanup};

} // namespace shenmue::s2::areas
