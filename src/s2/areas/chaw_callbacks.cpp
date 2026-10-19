#include "s2/areas/area_chaw.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {

void ChawStartCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio(CHAW_AUDIO_BANKS[g_actNumber], 1);
    EV_spmBeginSampleExecute();
    EVS_LoopCamera(1, CHAW_BASE, CHAW_BASE);
    if (!work->alternate)
        EV_PlayDialogue("0038A034");
    SetMotionRegionFlag(scenFindMotionwork(CHAW_RYO), 69, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0x17c8, MotionRegion::LowerBody);
    HLib::Task *fade = HLib::EnqueueTaskWithoutParameter(ChawFadeOutCallback, 6, 11, static_cast<std::int32_t>(CHAW_EVENT_TASK.name));
    HLib::SetupTimedTask(fade, 100);
    HLib::SetCurrentTaskTo(ChawInitCallback);
    HLib::SetupTaskDelay(131);
    work->helpFlags = 36;
    work->started = 1;
}

void ChawInitCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0x17c4, MotionRegion::LowerBody);
    FinishLoading();
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVS_LoopCamera(2, CHAW_BASE, CHAW_BASE);
    EV_EnsureCharacterDraw(CHAW_HAND);
    EV_EnableCharacterDisplay(CHAW_HAND);
    for (shen_name cup : work->cups)
        EV_EnableCharacterDisplay(cup);
    HLib::SetCurrentTaskTo(ChawPlayCallback);
    EV_AT_SetPos(CHAW_BASE, EventTransformFlags::AllAxes, {0, 10, 0});
    EV_EnableCharacterDisplay(CHAW_FACE);
    DateTime date;
    GetCurrentDateTime(date);
    if (CHAW_MODELS[work->entry].lighting.name || date.hour >= 19 || g_currentMap.name == SHEN_NAME(AKS0))
        EV_lightLoad(CHAW_AREA);
    if (work->entry != 21)
        for (std::int32_t cup = 0; cup < 4; ++cup) {
            const shen_name original{chawPlacements[work->entry].character.name + (static_cast<std::uint32_t>(cup) << 24)};
            if (DoesScenarioCharacterExist(original))
                EV_DisableCharacterDisplay(original);
        }
    EV_UpdateWeatherForMinute(1, 0, 1, 0);
    EV_UpdateWeatherForMinute(2, 0, 1, 0);
}

void ChawPlayCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    const std::int32_t carried = work->carriedCup;
    ++work->ticks;
    ChawUpdateHand(work);
    if (IsModeBitActive(1)) {
        constexpr std::int16_t hints[]{-1, 44, 58, -1, -1};
        const std::uint32_t *copy = reinterpret_cast<const std::uint32_t *>(CopyHudHintIds(hints));
        PublishHudHints(copy, true);
        if (g_inputBuffer[0].held & 4)
            SetCurrentHelpString(0x12);
        EV_DisableCharacterDisplay(CHAW_HAND);
    } else
        EV_EnableCharacterDisplay(CHAW_HAND);
    if (ChawLectureAllowsInput(work)) {
        DateTime date;
        GetCurrentDateTime(date);
        if (static_cast<std::uint8_t>(date.hour - 7) > 15) {
            if (work->entry != 21 && !IsModeBitActive(1)) {
                ChawQueueExit(work);
                return;
            }
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1021, 1);
            SetRenderClockAndCompassTo(1);
        }
        if (work->helpFlags & 32)
            CheckHelpIcon(32, 1, CheckHelpIconOp::FrameOverlay);
        if (work->helpFlags & 4)
            GetBattleDataConfig(0, SHEN_NAME(BTBA));
        std::uint16_t buttons = static_cast<std::uint16_t>(GetCurrentControlInput(2));
        if (!buttons)
            buttons = work->pendingButtons;
        if (IsModeBitActive(1))
            buttons = 0;
        const std::int32_t movement = work->handMovement;
        if (!movement)
            work->pendingButtons = 0;
        work->cupMoved = 0;
        switch (buttons) {
        case 1024:
            if (!movement && work->started) {
                ChawSetupChoice(20);
                HLib::SetCurrentTaskTo(ChawCheckChoiceCallback);
            }
            break;
        case 4096:
        case 8192:
            if (movement == 1)
                work->pendingButtons = buttons;
            else if ((buttons == 4096 && work->row != 0) || (buttons == 8192 && work->row != 2)) {
                const std::int32_t step = buttons == 4096 ? -1 : 1;
                ChawMoveCup(work, step);
                std::int32_t row = work->cupRows[carried] + step;
                if (row < 0)
                    row = 0;
                if (row > 2)
                    row = 2;
                work->cupRows[carried] = row;
                Loop3DSoundEffect(466);
            }
            break;
        case 16384:
        case 32768:
            if (movement != 2 && !work->error && work->enabled)
                ChawSelectCup(work, buttons == 16384 ? -1 : 1);
            else
                work->pendingButtons = buttons;
            break;
        default:
            break;
        }
        if (buttons != 1024 || movement || !work->started)
            GetBattleDataConfig(0, SHEN_NAME(BTIL));
    }
    const std::int32_t cell = work->selectedCup + 4 * work->row;
    work->handTarget = ChawGetHandTarget(cell);
    work->handYaw = ChawGetHandYaw(cell);
    if (work->handMovement == 2 && carried >= 0)
        ChawAttachCup(work, carried);
}

void ChawQueueExit(ChawEventWork *work) {
    work->state = 1;
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    nullsub();
    if (work->entry == 21) {
        HLib::SetCurrentTaskTo(ChawEndLectureCallback);
        work->lecturePhase = 13;
        HLib::SetupTaskDelay(150);
    } else {
        EV_PlayDialogue("0038A040");
        HLib::SetCurrentTaskTo(ChawStandUpCallback);
        HLib::Task *fade = HLib::EnqueueTaskWithoutParameter(ChawFadeOutCallback, 6, 11, static_cast<std::int32_t>(CHAW_EVENT_TASK.name));
        HLib::SetupTimedTask(fade, 35);
        HLib::SetupTaskDelay(65);
    }
}

void ChawCheckChoiceCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    const std::int16_t choice = shenGetCtrlSpeakIconFlag();
    if (choice == 34) {
        work->state = 1;
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        nullsub();
        EV_PlayDialogue("0038A033");
        HLib::SetCurrentTaskTo(ChawStandUpCallback);
        HLib::Task *fade = HLib::EnqueueTaskWithoutParameter(ChawFadeOutCallback, 6, 11, static_cast<std::int32_t>(CHAW_EVENT_TASK.name));
        HLib::SetupTimedTask(fade, 30);
        return;
    }
    if (choice != 149) {
        HLib::SetCurrentTaskTo(ChawPlayCallback);
        return;
    }
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    nullsub();
    work->progression = ChawGetProgression(work);
    switch (work->progression) {
    case 2: {
        chawPrize = 36;
        HLib::Task *clock = HLib::EnqueueTaskWithParameter(ChawForwardTimeCallback, 6, 11, 4, CHAW_EVENT_TASK);
        *static_cast<std::int32_t *>(HLib::GetTaskParameter(clock)) = 5;
        HLib::SetupTimedTask(clock, 29);
        EV_stepSetFlags(0, 49);
        EV_EventSetUpHelpIcon_0(0, 0);
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(ChawSuccessAuthorCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    }
    case 4:
        HLib::SetCurrentTaskTo(ChawSuccessBurinCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 6:
        work->state = 1;
        HLib::SetCurrentTaskTo(ChawGotoYamiutiCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 7:
        EV_EventSetUpHelpIcon_0(0, 0);
        HLib::SetCurrentTaskTo(ChawGotoDokuhebiCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 8:
        EV_EventSetUpHelpIcon_0(0, 0);
        HLib::SetCurrentTaskTo(ChawGotoSeiyaCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 9:
        work->state = 1;
        EV_EventSetUpHelpIcon_0(0, 0);
        HLib::SetCurrentTaskTo(ChawGotoKunCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 10:
        chawPrize = 39;
        work->state = 1;
        HLib::SetCurrentTaskTo(ChawGotoHyakusiCallback);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    default:
        work->state = 1;
        EV_DisableCharacterDisplay(CHAW_HAND);
        EV_ToggleCharacterFlag(CHAW_HAND);
        EV_AT_SetPos(CHAW_BASE, EventTransformFlags::AllAxes, chawPlacements[work->entry].position);
        EV_DisableCharacterDisplay(CHAW_FACE);
        ChawLoadAuthorCups(work);
        EVS_LoopCamera(work->cameraOffset + 3, CHAW_BASE, CHAW_BASE);
        ChawRestoreLighting(work);
        if (work->progression == 3)
            ChawQueueFadeAudioOut(320);
        HLib::SetCurrentTaskTo(ChawFailCallback);
        break;
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    npcResetDateTimeIfNeeded();
    work->advanceTime = 1;
    GetCurrentDateTime(chawTargetDate);
    float scale;
    if (work->progression > 1) {
        chawTargetDate.minute += 30;
        if (chawTargetDate.minute >= 60) {
            ++chawTargetDate.hour;
            chawTargetDate.minute -= 60;
        }
        scale = work->progression == 3 ? 150 : 1200;
    } else {
        chawTargetDate.hour += 2;
        if (chawTargetDate.hour >= 23) {
            chawTargetDate.hour = 23;
            chawTargetDate.minute = 0;
        }
        scale = 600;
    }
    UpdateTempHourMinTimeScale(chawTargetDate.hour, chawTargetDate.minute, scale);
    HLib::SetupTaskDelay(60);
}

void ChawSuccessBurinCallback(void *) {
}

void ChawFailCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    DateTime date;
    GetCurrentDateTime(date);
    const std::uint32_t time = date.minute + 100 * date.hour;
    if (time < static_cast<std::uint32_t>(chawTargetDate.minute + 100 * chawTargetDate.hour) && time <= 2300)
        return;
    if (work->progression == 3) {
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        ChawCleanup(work, false);
        SetPlayerControlEnabled(0);
        EV_stepSetFlags(2, 49);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(TOFB), 0);
    } else {
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(CHAW_RYO), FacialExpressionId::Sad, 0, 1);
        EVS_LoopCamera(4, CHAW_BASE, CHAW_BASE);
        if (work->wrongSign) {
            EV_PlayDialogue("0038A037");
            HLib::SetCurrentTaskTo(ChawFailMemoCallback);
        } else {
            EV_PlayDialogue("0038A036");
            HLib::SetCurrentTaskTo(ChawStandUpCallback);
            HLib::SetupTaskDelay(150);
        }
    }
}
void ChawFailMemoCallback(void *) {
    if (!cvsHasValidConverseData(nullptr)) {
        EV_PlayDialogue("0038A038");
        HLib::SetCurrentTaskTo(ChawStandUpCallback);
        HLib::SetupTaskDelay(62);
    }
}

void ChawStandUpCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    if (camCtrlCheckIsEventCam() || filmCheckFadeTrackColour() || cvsHasValidConverseData(nullptr))
        return;
    EV_NPC_NewLoadSw(1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(CHAW_RYO), FacialExpressionId::Normal, 0, 1);
    EV_AT_SetPos(CHAW_BASE, EventTransformFlags::AllAxes, chawPlacements[work->entry].position);
    EV_DisableCharacterDisplay(CHAW_FACE);
    if (work->progression != 1) {
        ChawRestoreLighting(work);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    EVS_LoopCamera(5, CHAW_BASE, CHAW_BASE);
    EV_DisableCharacterDisplay(CHAW_HAND);
    SetMotionRegionFlag(scenFindMotionwork(CHAW_RYO), 69, 2, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0x17c6, MotionRegion::LowerBody);
    ChawQueueFadeAudioOut(static_cast<std::int32_t>(GetMotionEndFrame(scenFindMotionwork(CHAW_RYO), MotionRegion::LowerBody) - 10));
    HLib::SetCurrentTaskTo(ChawFinishCallback);
}
void ChawFinishCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    if (chawPrize) {
        AddInventoryItem(chawPrize, 1, 1);
        chawPrize = 0;
    }
    if (SetMotionRegionFlag(scenFindMotionwork(CHAW_RYO), 65, 1, MotionRegion::LowerBody) && !filmCheckFadeTrackColour()) {
        ChawCleanup(work, true);
        if (work->state)
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}
} // namespace shenmue::s2
