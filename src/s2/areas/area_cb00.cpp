#include "s2/areas/area_cb00.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_call.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2 {
std::int32_t cb00QteFrames;
namespace {
void PlayAuthor(Cb00EventWork *work, std::int32_t index) {
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(CB00_AUTHORS[index].name));
    work->frames = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining));
}
void Film(std::int32_t frames, std::int32_t initial, std::int32_t target, float delay, bool queued) {
    EventFilmControl request{.cleanup = queued ? 1 : 0,
        .frames = frames,
        .startAlpha = initial,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = target,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = delay};
    if (queued) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 11, sizeof(EventFilmControl), CB00_TASK);
        RegisterEventTask(task);
        *static_cast<EventFilmControl *>(HLib::GetTaskParameter(task)) = request;
    } else
        EVT_AT_MenCtrl_Callback(&request);
}
void RestoreLightingCallback(void *) {
    Cb00RestoreSceneLighting();
}
void QueuePrompt(Cb00EventWork *work, std::int16_t status, std::int32_t extension = 0) {
    QueueCb00Qte(work->task, status, static_cast<std::int16_t>(cb00QteFrames + extension), work->frames - cb00QteFrames - 7);
}
void DisplayCheck(std::int32_t enabled) {
    charaSetDisplayEnabledFlag(FindCharacterByScenarioName(CB00_RYO), enabled);
    charaSetDisplayEnabledFlag(FindCharacterByScenarioName(CB00_REN), enabled);
}
void TruckPosition() {
    EventSetPosition position{.cleanup = 0, .character = CB00_TRUCK, .parent = CB00_CENTRE, .position = CB00_TRUCK_EXIT_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
}
void QueueTruckDynamics(float endFrame, float frame, float delay) {
    Queue_EVT_AT_PlayDynamix(CB00_TRUCK, "TAKE", frame, endFrame, 30, CB00_TAKE_PARTS, 2, static_cast<std::int32_t>(delay));
}
} // namespace
void Cb00Init() {
    cb00QteFrames = 16;
    cb00BridgeRequestTask = nullptr;
    cb00BridgeTask = nullptr;
}
void Cb00CameraCallback(void *) {
    EventSetPosition position{.cleanup = 0, .character = CB00_DUMMY, .parent = CB00_RYO, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = CB00_DUMMY, .parent = CB00_RYO, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
}
void Cb00Initialize(HLib::Task *task) {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    Cb00EventWork *const work = static_cast<Cb00EventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->exitArea = CB00_EXIT_AREA;
    work->exitEntry = 0;
    work->stepFlag = 0;
    work->phase = 0;
    work->frames = 0;
    work->boardFailure = 0;
    work->qteStatus = 0;
    work->authorVariant = 0;
    work->battleFailure = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, CB00_READ_CHARACTERS);
    work->smoke = -1;
    const Vector3 scale{1, 1, 1.5f};
    mtwkSetMotionworkScale3(CB00_BRIDGE, &scale);
    GetCurrentDateTime(work->date);
    Cb00InitializationNullsub();
    SetRenderClockAndCompassTo(1);
    EV_setHoldStatesFlags(0);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Queue);
    Vector3 position{};
    EV_scenApplyPosOrRotToPartOf(CB00_TRUCK, 0xa5, &position, nullptr, EventCharacterPartTransformMode::Set);
}
void Cb00Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00Callback, 4, 11, sizeof(Cb00EventWork), CB00_TASK);
    Cb00Initialize(task);
    HLib::EnqueueTaskWithoutParameter(Cb00CameraCallback, 4, 11, static_cast<std::int32_t>(CB00_TASK.name));
}
void Cb00Callback(void *data) {
    Cb00EventWork *const work = static_cast<Cb00EventWork *>(data);
    switch (work->phase) {
    case 0:
        Cb00SetupCharacters();
        work->phase = 2;
        work->frames = -1;
        break;
    case 1:
        EV_LoadAudio("0063E000", 0);
        EV_EnableCharacterDisplay(CB00_REN);
        EV_EnableCharacterDisplay(CB00_RYO);
        cb00QteFrames += 4;
        if (cb00QteFrames > 24)
            cb00QteFrames = 24;
        SetCurrentDateTimeTo(work->date);
        work->phase = 2;
        work->boardFailure = 0;
        work->authorVariant = 0;
        work->battleFailure = 0;
        work->frames = -1;
        break;
    case 2:
        Film(10, 255, 0, 5, true);
        Cb00SetupSceneLighting();
        Cb00ResetProps();
        work->failures = 0;
        PlayAuthor(work, 17);
        work->phase = 3;
        Film(10, 0, 255, static_cast<float>(work->frames - 20), work->frames != 20);
        break;
    case 3:
        camCtrlToggleSmoothing(1);
        Cb00SetupAuthorLighting();
        Film(10, 255, 0, 1, true);
        PlayAuthor(work, 0);
        QueuePrompt(work, 16);
        work->phase = 4;
        break;
    case 4:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            camCtrlToggleSmoothing(0);
            work->frames = -1;
            work->phase = (work->qteStatus & 0xfff) == 16 ? 5 : 6;
        }
        break;
    case 5:
        camCtrlToggleSmoothing(0);
        QueueCb00Blur(10, 50);
        PlayAuthor(work, 1);
        Queue_EVT_AT_PlayDynamix(CB00_PIPE, "PIPE", 2, 190, 7, CB00_PIPE_PARTS, 2, 0);
        work->failures = 0;
        work->phase = 8;
        break;
    case 6:
        PlayAuthor(work, 2);
        Queue_EVT_AT_PlayDynamix(CB00_PIPE, "PIPE", 2, 190, 7, CB00_PIPE_PARTS, 2, 0);
        QueueCb00Smoke(work, CB00_PIPE, nullptr, {0.005f, 0, 0}, 17, 39, 1);
        work->phase = 8;
        break;
    case 8:
        PlayAuthor(work, 3);
        QueuePrompt(work, 256);
        work->phase = 9;
        if (work->failures > 0)
            EVT_AT_ObjectDisp_1(CB00_REN, 1, 1);
        break;
    case 9:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            work->frames = -1;
            work->phase = (work->qteStatus & 0xfff) == 256 ? 10 : 11;
        }
        break;
    case 10:
        EVT_AT_ObjectDisp_1(CB00_REN, 5, 1);
        QueueCb00Blur(0, 20);
        PlayAuthor(work, 4);
        work->phase = 12;
        break;
    case 11:
        EV_DisableCharacterDisplay(CB00_REN);
        EVT_AT_SetMot(CB00_REN, 0xf002, 0, 1, 0, 0);
        QueueCb00Smoke(work, CB00_RYO, nullptr, {0.005f, 0, 0}, 18, 70, 1);
        if (work->boardFailure == 1) {
            PlayAuthor(work, 19);
            QueueCb00Blur(10, 50);
        } else {
            PlayAuthor(work, 5);
            QueueCb00Blur(10, 50);
            if (work->boardFailure)
                EVT_AT_SetSound(379, 20);
            else {
                Queue_EVT_AT_PlayDynamix(CB00_BOARD, "BORD", 2, 206, 10, CB00_BOARD_PARTS, 2, 0);
                EVT_AT_SetSound(379, 30);
            }
        }
        if (++work->failures >= 3) {
            Film(30, 0, 255, 0, false);
            work->frames = 30;
            work->phase = 40;
        } else
            work->phase = 8;
        work->boardFailure = 1;
        break;
    case 12:
        DisplayCheck(0);
        EV_EnableCharacterDisplay(CB00_REN);
        PlayAuthor(work, 6);
        QueuePrompt(work, 64);
        Queue_EVT_AT_PlayDynamix(CB00_PLANK_LEFT, "CANK", 2, 274, 3, nullptr, 2, 33);
        Queue_EVT_AT_PlayDynamix(CB00_PLANK_RIGHT, "CANK", 2, 274, 3, nullptr, 2.5f, 32);
        work->phase = 14;
        break;
    case 14:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            DisplayCheck(1);
            work->frames = -1;
            work->failures = 0;
            work->phase = (work->qteStatus & 0xfff) == 64 ? 15 : 17;
        }
        break;
    case 15:
        PlayAuthor(work, 7);
        work->phase = 18;
        break;
    case 17:
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(CB00_RYO), 0);
        PlayAuthor(work, 8);
        work->phase = 18;
        break;
    case 18: {
        std::int32_t cleanup = 0;
        Cb00BridgeEndCallback(&cleanup);
        DisplayCheck(0);
        PlayAuthor(work, work->authorVariant == 1 ? 20 : 16);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 11, sizeof(EventSmokeWork), CB00_TASK);
        HLib::SetupTimedTask(task, 28);
        EventSmokeWork *const smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
        smoke->state = 2;
        smoke->character = CB00_RYO;
        smoke->effect = 18;
        smoke->remaining = 8;
        smoke->profile = 2;
        smoke->finishDelay = 1;
        smoke->startDelay = 0;
        smoke->birthFraction = 0.1f;
        smoke->birthStep = 0.05f;
        if (!work->authorVariant)
            Cb00StartBridge();
        work->phase = 19;
        break;
    }
    case 19:
        work->frames = Cb00BeginCrossing();
        QueuePrompt(work, 4);
        EVT_AT_SetVoice("0063A023", 1, 30);
        work->phase = 20;
        break;
    case 20:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            EV_DisableCharacterDisplay(CB00_REN);
            if ((work->qteStatus & 0xfff) == 4) {
                EV_PlayDialogue("0063A024");
                work->phase = 21;
            } else
                work->phase = 22;
            work->frames = -1;
        }
        break;
    case 21:
        work->frames = Cb00CrossingFirstSuccess();
        QueuePrompt(work, 64, 7);
        EVT_AT_SetVoice("0063A025", 1, work->frames - cb00QteFrames - 7);
        work->phase = 23;
        break;
    case 22:
        EV_PlayDialogue("0063A039");
        work->phase = 18;
        work->frames = Cb00CrossingFailure(work, 0);
        break;
    case 23:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            if ((work->qteStatus & 0xfff) == 64) {
                EV_PlayDialogue("0063A026");
                work->phase = 24;
            } else
                work->phase = 25;
            work->frames = -1;
        }
        break;
    case 24:
        work->frames = Cb00CrossingSecondSuccess();
        QueuePrompt(work, 4);
        EVT_AT_SetVoice("0063A025", 1, work->frames - cb00QteFrames - 7);
        work->phase = 26;
        break;
    case 25:
        EV_PlayDialogue("0063A038");
        work->phase = 18;
        work->frames = Cb00CrossingFailure(work, 1);
        break;
    case 26:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
            if ((work->qteStatus & 0xfff) == 4) {
                EV_PlayDialogue("0063A028");
                work->phase = 27;
            } else
                work->phase = 28;
            work->frames = -1;
        }
        break;
    case 27:
        work->frames = Cb00FinishCrossing();
        EVT_AT_SetVoice("0063A029", 1, work->frames - 12);
        work->phase = 29;
        break;
    case 28:
        EV_PlayDialogue("0063A039");
        work->phase = 18;
        work->frames = Cb00CrossingFailure(work, 2);
        break;
    case 29: {
        DisplayCheck(1);
        mtwkSetRequestedRootMode(scenFindMotionwork(CB00_RYO), 0);
        std::int32_t cleanup = 0;
        Cb00BridgeEndCallback(&cleanup);
        for (std::int32_t index = 0; index < 10; ++index)
            EV_SetObjectFlagByIndex(index, index >= 6);
        PlayAuthor(work, 9);
        for (std::int32_t index = 0; index < 10; ++index)
            EV_SetObjectFlagByIndex(index, index != 8);
        QueueEventCallFunction(RestoreLightingCallback, nullptr, 101);
        work->phase = 30;
        break;
    }
    case 30:
        Cb00RestoreSceneLighting();
        ProcessAicaCommand(0x1200a0, 0);
        EV_LoadAudio("0063E000", 1);
        EV_EnableCharacterDisplay(CB00_REN);
        PlayAuthor(work, 10);
        work->frames -= 4;
        work->phase = 31;
        break;
    case 31:
        if (--work->frames <= 0) {
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
            qteBattleUtil(BattleQteMode::SetWait, 2, 6, cb00QteFrames + cb00QteFrames / 2);
            qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256, 1);
            qteBattleUtil(BattleQteMode::Start);
            work->phase = 32;
        }
        break;
    case 32:
        if (qteBattleUtil(BattleQteMode::GetResult)) {
            work->frames = -1;
            work->phase = qteBattleUtil(BattleQteMode::GetResult) == 1 ? 33 : 38;
        }
        break;
    case 33: {
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        PlayAuthor(work, 12);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00DormantRetireCallback, 6, 11, sizeof(Cb00DormantRetireWork), CB00_TASK);
        HLib::SetupTimedTask(task, 20);
        Cb00DormantRetireWork *const retire = static_cast<Cb00DormantRetireWork *>(HLib::GetTaskParameter(task));
        retire->source = CB00_REN;
        retire->target = CB00_RYO;
        retire->reserved08 = 0;
        retire->frames = 20;
        QueueTruckDynamics(static_cast<float>(2 * work->frames) + 2, 2, 82);
        work->phase = 34;
        break;
    }
    case 34:
        PlayAuthor(work, 13);
        QueueTruckDynamics(static_cast<float>(2 * work->frames) + 23, 22, 1);
        work->phase = 35;
        break;
    case 35:
        PlayAuthor(work, 14);
        TruckPosition();
        QueueTruckDynamics(static_cast<float>(2 * work->frames) + 23, 22, 1);
        work->phase = 36;
        break;
    case 36: {
        PlayAuthor(work, 15);
        TruckPosition();
        QueueTruckDynamics(static_cast<float>(2 * work->frames) + 22, 22, 1);
        QueueCb00TruckQuake(-0.2f);
        const Vector3 position{318.8999938964844f, 103.19999694824219f, 404.70001220703125f};
        QueueCb00Smoke(work, shen_name{0}, &position, {0.001f, 0.001f, 0.001f}, 40, work->frames - 1, 0);
        work->phase = 39;
        if (g_entryPoint == 99) {
            work->phase = 42;
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        break;
    }
    case 37:
        Film(10, 0, 255, static_cast<float>(work->frames - 10), work->frames != 10);
        work->phase = 41;
        work->exitArea = CB00_EXIT_AREA;
        work->exitEntry = 0;
        work->stepFlag = 0;
        break;
    case 38:
        work->battleFailure = 1;
        Cb00SetupRetryLighting();
        PlayAuthor(work, 11);
        work->phase = 1;
        work->stepFlag = 1;
        break;
    case 39:
        PlayAuthor(work, 18);
        work->stepFlag = 0;
        work->phase = 41;
        work->exitArea = CB00_EXIT_AREA;
        work->exitEntry = 0;
        break;
    case 40:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        work->frames += 90;
        work->phase = 1;
        break;
    case 41:
        work->exitArea = CB00_EXIT_AREA;
        work->exitEntry = 0;
        Cb00Exit(work);
        break;
    case 42:
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        work->frames = 100000;
        break;
    default:
        break;
    }
    if (work->frames != -1)
        HLib::SetCurrentTaskTo(Cb00WaitFrameCallback);
}
void Cb00WaitSkipCallback(void *) {
    if (!Author_GetController()->skipToNext)
        HLib::SetCurrentTaskTo(Cb00Callback);
}
void Cb00WaitFadeCallback(void *data) {
    Cb00EventWork *const work = static_cast<Cb00EventWork *>(data);
    if (--work->frames <= 0) {
        work->frames = 0;
        HLib::SetCurrentTaskTo(Cb00Callback);
    }
}
void Cb00WaitFrameCallback(void *data) {
    Cb00EventWork *const work = static_cast<Cb00EventWork *>(data);
    const std::int32_t frames = --work->frames;
    if (work->battleFailure == 1 && work->phase == 1) {
        if (frames <= 90) {
            Film(90, 0, 255, 0, false);
            work->frames = 150;
            HLib::SetCurrentTaskTo(Cb00WaitFadeCallback);
            return;
        }
    } else if (frames <= 0) {
        work->frames = 0;
        HLib::SetCurrentTaskTo(Cb00Callback);
        return;
    }
    if (work->phase == 1) {
        if (CheckMediatedDoublePress() && GetCompletedGameCount()) {
            Author_RequestAbort();
            work->frames = 0;
            HLib::SetCurrentTaskTo(Cb00WaitSkipCallback);
        }
    } else if (static_cast<std::uint32_t>(work->phase - 33) <= 3 && CheckMediatedDoublePress() && GetCompletedGameCount()) {
        Author_RequestAbort();
        work->phase = 41;
        work->stepFlag = 0;
        work->exitArea = CB00_EXIT_AREA;
        work->exitEntry = 0;
        work->frames = 0;
        HLib::SetCurrentTaskTo(Cb00WaitSkipCallback);
    }
}
void Cb00Exit(Cb00EventWork *work) {
    DateTime current;
    GetCurrentDateTime(current);
    if (static_cast<std::uint8_t>(work->date.hour - 5) > 17) {
        work->date.minute = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(work->date.minute) + 30));
        SetCurrentDateTimeTo(work->date);
    } else if (current.hour < 23) {
        current.hour = 23;
        current.minute = 0;
        SetCurrentDateTimeTo(current);
    }
    ET_AT_KillWork(work->characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_FreeAuthorPak(0, "0063");
    qteBattleUtil(BattleQteMode::RetireAndFree);
    SetRenderClockAndCompassTo(0);
    EV_stepSetFlags(work->stepFlag, 0x84);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_scenQueueLoad(g_actNumber, work->exitArea, work->exitEntry);
}
void Cb00QteStartCallback(void *data) {
    Cb00QteWork *const work = static_cast<Cb00QteWork *>(data);
    SetShowQteFlag(static_cast<std::uint16_t>(work->status), work->maximumFrames, 2);
    HLib::SetCurrentTaskTo(Cb00QteWaitCallback);
}
void Cb00QteWaitCallback(void *data) {
    Cb00QteWork *const work = static_cast<Cb00QteWork *>(data);
    Cb00EventWork *const parent = static_cast<Cb00EventWork *>(HLib::GetTaskParameter(work->parent));
    parent->qteStatus = static_cast<std::int16_t>(EV_GetCurrentQteStatus());
    if (parent->qteStatus != -1)
        HLib::DestroyCurrentTask();
}
void QueueCb00Qte(HLib::Task *parent, std::int16_t status, std::int16_t maximumFrames, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00QteStartCallback, 6, 11, sizeof(Cb00QteWork), CB00_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    Cb00QteWork *const work = static_cast<Cb00QteWork *>(HLib::GetTaskParameter(task));
    work->status = status;
    work->maximumFrames = maximumFrames;
    work->parent = parent;
    static_cast<Cb00EventWork *>(HLib::GetTaskParameter(parent))->qteStatus = -1;
}
const ScriptAreaDefinition cb00Definition{.areaId = CB00_AREA, .field04 = 0, .init = Cb00Init, .main = Cb00Main};
} // namespace shenmue::s2
