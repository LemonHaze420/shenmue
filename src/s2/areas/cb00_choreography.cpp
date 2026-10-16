#include "s2/areas/area_cb00.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/weather.hpp"
#include <cstring>
namespace shenmue::s2 {
void Cb00SetupCharacters() {
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    SetMotionRegionFlag(scenFindMotionwork(CB00_RYO), 71, 2, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(scenFindMotionwork(CB00_RYO), 0);
    SetMotionRegionFlag(scenFindMotionwork(CB00_REN), 71, 2, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(scenFindMotionwork(CB00_REN), 0);
    SetLowQualityHands(FindCharacterByScenarioName(CB00_REN));
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_FreeAuthorPak(1, "0063");
    SetScenarioCharacterRadius(CB00_RYO, 10);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(CB00_RYO), FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(CB00_RYO), 0, 1, 1);
    SetScenarioCharacterType(CB00_REN, CharacterTypeStencilShadow);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    for (shen_name character : CB00_TRUCKS)
        SetPluralControlFor(FindCharacterByScenarioName(character), 0);
}
void Cb00ResetProps() {
    EventSetPosition position{.cleanup = 0, .character = CB00_PIPE, .parent = CB00_CENTRE, .position = CB00_PIPE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = CB00_PIPE, .parent = CB00_CENTRE, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    Queue_EVT_AT_PlayDynamix(CB00_PIPE, "PIPE", 2, 4, 7, CB00_PIPE_PARTS, 2, 0);
    position.character = CB00_BOARD;
    position.position = CB00_BOARD_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = CB00_BOARD;
    EVT_AT_SetAng_Callback(&angle);
    Queue_EVT_AT_PlayDynamix(CB00_BOARD, "BORD", 2, 4, 10, CB00_BOARD_PARTS, 2, 0);
    position.character = CB00_PLANK_LEFT;
    position.position = {};
    EVT_AT_SetPosRot(&position);
    angle.character = CB00_PLANK_LEFT;
    EVT_AT_SetAng_Callback(&angle);
    position.character = CB00_PLANK_RIGHT;
    position.position = CB00_PLANK_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = CB00_PLANK_RIGHT;
    EVT_AT_SetAng_Callback(&angle);
    Vector3 zero{};
    EV_scenApplyPosOrRotToPartOf(CB00_BRIDGE, 0x99, &zero, nullptr, EventCharacterPartTransformMode::Set);
    Queue_EVT_AT_PlayDynamix(CB00_PLANK_LEFT, "CANK", 2, 4, 3, nullptr, 2, 0);
    Queue_EVT_AT_PlayDynamix(CB00_PLANK_RIGHT, "CANK", 2, 4, 3, nullptr, 2, 0);
    position.character = CB00_TRUCK;
    position.position = CB00_TRUCK_INITIAL_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = CB00_TRUCK;
    angle.rotation.y = 49152;
    EVT_AT_SetAng_Callback(&angle);
}
void Cb00SetupSceneLighting() {
    for (std::int32_t index = 0; index < 10; ++index)
        EV_SetObjectFlagByIndex(index, index >= 6);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_UpdateWeather();
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint8_t>(time.hour - 6) <= 12) {
        EV_SetWeatherInstruction(0x0f, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_ReinitLights(1);
        EV_SetModelDataDefaultVertexShader();
    } else {
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_ReinitLights(1);
        EV_lightLoad(CB00_LIGHT);
        PushGlobalLightingProgram();
    }
}
void Cb00RestoreSceneLighting() {
    for (std::int32_t index = 0; index < 10; ++index)
        EV_SetObjectFlagByIndex(index, index >= 6);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_UpdateWeather();
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint8_t>(time.hour - 6) <= 12) {
        EV_SetWeatherInstruction(0x0f, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_ReinitLights(1);
        EV_SetModelDataDefaultVertexShader();
    } else {
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_ReinitLights(1);
        EV_lightLoad(CB00_LIGHT);
    }
}
void Cb00SetupRetryLighting() {
    for (std::int32_t index = 0; index < 10; ++index)
        EV_SetObjectFlagByIndex(index, 1);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_UpdateWeather();
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint8_t>(time.hour - 6) <= 12) {
        EV_SetWeatherInstruction(0x0f, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_ReinitLights(1);
        EV_SetModelDataDefaultVertexShader();
    } else {
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_ReinitLights(1);
        EV_lightLoad(CB00_LIGHT);
    }
}
void Cb00SetupAuthorLighting() {
    for (std::int32_t index = 0; index < 10; ++index)
        EV_SetObjectFlagByIndex(index, index != 8);
    SetMapControlStatusFlag(0, 0);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetIndexForLightEntry(0, 0);
    EV_FreeWAIR();
    SetWeatherUpdateEnabledTo(0);
    EV_ReinitLights(1);
    EV_lightLoad(CB00_LIGHT);
    PushGlobalLightingProgram();
    SetDefaultVSByIndex(9);
}
std::int32_t Cb00StartBridge() {
    Cb00BridgeEnqueue(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 11, sizeof(StepObjectDisplayWork), CB00_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->character = CB00_REN;
    work->mode = 1;
    work->delay = 27;
    return 70;
}
std::int32_t Cb00BeginCrossing() {
    mtwkSetRequestedRootMode(scenFindMotionwork(CB00_RYO), 3);
    Cb00BridgeEnqueue(45);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00BridgeCharacterCallback, 6, 11, sizeof(Cb00BridgeCharacterWork), CB00_TASK);
    if (task) {
        Cb00BridgeCharacterWork *const work = static_cast<Cb00BridgeCharacterWork *>(HLib::GetTaskParameter(task));
        if (work) {
            work->character = CB00_RYO;
            work->cleanup = 45;
            HLib::SetupTimedTask(task, 45);
        }
    }
    EVT_AT_SetMot(CB00_RYO, 6245, 15, 1, 0x40000000U, 0);
    EVT_AT_SetMot(CB00_RYO, 6220, 0, 1, 0x20000000U, 49);
    EventSetPosition position{.cleanup = 0, .character = CB00_RYO, .parent = CB00_CENTRE, .position = CB00_CROSSING_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = CB00_RYO, .parent = CB00_CENTRE, .degrees = 270, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EVS_LoopCamera(181, CB00_CENTRE, CB00_CENTRE);
    return 74;
}
std::int32_t Cb00CrossingFirstSuccess() {
    QueueCb00BridgeRequest(0x10000000U, 0, 124);
    QueueCb00BridgeRequest(0x10000000U, 0.005f, 160);
    EventSetCamera camera{.cleanup = 0, .camera = 182, .eye = CB00_CENTRE, .target = CB00_CENTRE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(183, CB00_CENTRE, CB00_CENTRE, 0, 0, 66);
    EVT_AT_SetMot(CB00_RYO, 6226, 1, 1, 0x20000000U, 0);
    EVT_AT_SetMot(CB00_RYO, 6247, 0, 1, 0x20000000U, 30);
    EVT_AT_SetMot(CB00_RYO, 6247, 0, 1, 0x20000000U, 75);
    EVT_AT_SetMot(CB00_RYO, 6220, 0, 1, 0, 94);
    return 119;
}
std::int32_t Cb00CrossingSecondSuccess() {
    QueueCb00BridgeRequest(0x10000000U, 0, 124);
    QueueCb00BridgeRequest(0x10000000U, 0.005f, 149);
    EVT_AT_SetCam(195, CB00_CENTRE, CB00_CENTRE, 0, 0, 80);
    EVT_AT_SetMot(CB00_RYO, 6226, 1, 1, 0, 0);
    EVT_AT_SetMot(CB00_RYO, 6247, 0, 1, 0, 30);
    EVT_AT_SetMot(CB00_RYO, 6220, 0, 1, 0x20000000U, 75);
    return 100;
}
std::int32_t Cb00FinishCrossing() {
    QueueCb00BridgeRequest(0x10000000U, 0, 124);
    QueueCb00BridgeRequest(0x04000000U, 0, 200);
    QueueCb00BridgeRequest(0x02000000U, 0, 220);
    QueueCb00BridgeRequest(0x01000000U, 0, 222);
    EVT_AT_SetCam(186, CB00_CENTRE, CB00_CENTRE, 0, 0, 75);
    EVT_AT_SetPos(CB00_RYO, CB00_CENTRE, {2.2f, 8.100000381469727f, 18.700000762939453f}, 75);
    EVT_AT_SetMot(CB00_RYO, 6226, 1, 1, 0x20000000U, 0);
    EVT_AT_SetMot(CB00_RYO, 6247, 0, 1, 0, 30);
    EVT_AT_SetMot(CB00_RYO, 6258, 0, 1, 0x40000000U, 75);
    return 121;
}
std::int32_t Cb00CrossingFailure(Cb00EventWork *parent, std::int32_t outcome) {
    parent->authorVariant = 1;
    if (cb00BridgeRequestTask) {
        const std::uint32_t flags = 0x04000000U;
        std::memcpy(static_cast<std::byte *>(HLib::GetTaskParameter(cb00BridgeRequestTask)) + 4, &flags, 4);
        QueueCb00BridgeRequest(0x10000000U, 0.0075f, 20);
        QueueCb00BridgeRequest(0x10000000U, 0, 40);
        QueueCb00BridgeRequest(0x02000000U, 0, 90);
    }
    if (outcome == 0) {
        EventSetCamera camera{.cleanup = 0, .camera = 187, .eye = CB00_DUMMY, .target = CB00_DUMMY, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(193, CB00_CENTRE, CB00_CENTRE, 0, 0, 40);
        EVT_AT_SetCam(196, CB00_CENTRE, CB00_CENTRE, 0, 0, 58);
    } else if (outcome == 1) {
        EventSetCamera camera{.cleanup = 0, .camera = 194, .eye = CB00_CENTRE, .target = CB00_CENTRE, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(191, CB00_DUMMY, CB00_DUMMY, 0, 0, 62);
        EVT_AT_SetCam(192, CB00_DUMMY, CB00_DUMMY, 0, 0, 78);
    } else if (outcome == 2) {
        EventSetCamera camera{.cleanup = 0, .camera = 187, .eye = CB00_DUMMY, .target = CB00_DUMMY, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(188, CB00_RYO, CB00_RYO, 0, 0, 57);
        EVT_AT_SetCam(189, CB00_DUMMY, CB00_DUMMY, 0, 0, 78);
    }
    EVT_AT_SetMot(CB00_RYO, 6230, 0, 1, outcome == 1 ? 0x20000000U : 0, 0);
    QueueCb00Smoke(parent, CB00_RYO, nullptr, {0.005f, 0.005f, 0.005f}, 59, 36, 1);
    if (++parent->failures < 3)
        return 96;
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 45,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 30});
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00BridgeEndCallback, 6, 11, 4, CB00_TASK);
    if (task) {
        std::int32_t *const cleanup = static_cast<std::int32_t *>(HLib::GetTaskParameter(task));
        if (cleanup) {
            *cleanup = 1;
            HLib::SetupTimedTask(task, 90);
        }
    }
    parent->phase = 40;
    return 90;
}
} // namespace shenmue::s2
