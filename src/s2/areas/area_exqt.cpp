#include "s2/areas/area_exqt.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {

void ExciteRemovedLogger() {
}
void RestoreExciteHands() {
    SetLowQualityHands(ctrlGetCharaByName(EXQT_RYO));
    SetHighModelXYZ(ctrlGetCharaByName(EXQT_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(EXQT_RYO), 1, 1, 0);
}
void ExciteAward(void *) {
    if (!filmCheckFadeTrackColour()) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1746, 1);
        HLib::MarkCurrentTaskForCleanup();
        scenFreeEvent("EXQT.EV3");
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("EXQT"), 0);
    }
}
void ExciteExitToOmake(void *) {
    if (!filmCheckFadeTrackColour()) {
        HLib::MarkCurrentTaskForCleanup();
        scenFreeEvent("EXQT.EV3");
        EV_scenQueueLoad(g_actNumber, EXQT_OMAKE, 1);
    }
}
void ExitExcite(ExciteState &state) {
    if (state.game.score >= state.game.best)
        SetSavedMinigameScore(SavedScoreType::ExciteQTE_BestScore, state.game.score);
    SetSavedMinigameScore(SavedScoreType::ExciteQTE_ContinuedScore, state.game.score);
    hndlSetFlags(ctrlGetCharaByName(EXQT_RYO));
    EV_SetCharaControllerFlags(EXQT_RYO, 2, static_cast<NpcCharacterControllerState>(1));
    EV_SetCharaControllerFlags(EXQT_RYO, 3, static_cast<NpcCharacterControllerState>(1));
    mtwkRequestMotion(scenFindMotionwork(EXQT_RYO), 0xf002, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(EXQT_RYO), 71, 0, MotionRegion::LowerBody);
    spmFadeOut();
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    }
    EV_FreeAuthorPak(0, "0116E");
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_NPC_NewLoadSw(1);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        HLib::SetCurrentTaskTo(ExciteExitToOmake);
    else {
        EVS_LoopCamera(0, shen_name{0}, shen_name{0});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        if (state.game.score < 200000 || (state.game.flags & 16)) {
            HLib::MarkCurrentTaskForCleanup();
            scenFreeEvent("EXQT.EV3");
            SetPlayerControlEnabled(1);
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("EXQT"), 0);
            SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        } else
            HLib::SetCurrentTaskTo(ExciteAward);
    }
}
} // namespace

void InitializeExqt() {
}
void InitializeExcitePosition(ExciteState &state) {
    EV_EnsureCharacterDraw(EXQT_MACHINE);
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(EXQT_RYO), 0xf002, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(EXQT_RYO), 71, 0, MotionRegion::LowerBody);
    EVT_AT_SetPosAng(EXQT_RYO, EXQT_MACHINE, 0, .718f, 0, -1);
    EV_SetCharaControllerFlags(EXQT_RYO, 2, static_cast<NpcCharacterControllerState>(0));
    EV_SetCharaControllerFlags(EXQT_RYO, 3, static_cast<NpcCharacterControllerState>(0));
    state.game.state = 0;
}
void ResetExciteGame(ExciteState &state) {
    state.lives = 3;
    state.game.score = 0;
    state.game.successes = 0;
    state.game.cumulativeBonus = 0;
    state.game.timingBonus = 0;
    state.game.waitElapsed = 0;
    state.cycle = 0;
    state.patternByte = 0;
    state.patternStage = 0;
    state.immediateAdvance = 0;
    state.game.waitTarget = 40;
    state.patternScale = 3;
    state.tier = 0;
    state.timeScale = 8;
    ExciteInput &input = state.controls.excite.input;
    input.required = 0;
    input.pressed = 0;
    input.window = 0;
    input.elapsed = 0;
    input.attempts = 0;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *character = GetScenarioCharacterNode(control->scenarioTask, EXQT_MACHINE);
    if (character && character->model) {
        ShowElementByIndex(UIElement::QteMinigame);
        SetQteMinigameModel(static_cast<MT7Model *>(character->model));
    }
    InitializeExciteMachineParts();
    HideExciteMachineParts();
    ResetExciteMachineDisplay();
}
void RunExqt(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ExciteMain, 6, 11, sizeof(ExciteTaskWork), EXQT_TASK);
    ExciteState &state = static_cast<ExciteTaskWork *>(HLib::GetTaskParameter(task))->state;
    state.game.flags = *reinterpret_cast<const std::uint32_t *>(argument);
    state.game.score = 0;
    const std::int32_t best = GetSavedMinigameScore(0);
    state.game.best = best > 10000 ? best : 10000;
    state.game.continued = GetSavedMinigameScore(1);
    InitializeExcitePosition(state);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_SetNewSceneIndex();
    EV_spmBeginSampleExecute();
    RestoreExciteHands();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("EXQT"), 0);
}
void ExciteMain(void *data) {
    ExciteState &state = static_cast<ExciteTaskWork *>(data)->state;
    ExciteGameState &game = state.game;
    ExciteInput &input = state.controls.excite.input;
    UpdateExciteDisplay(state);
    EV_EventSetUpHelpIcon_0(4, 0);
    switch (game.state) {
    case 0:
        if (!filmCheckFadeTrackColour()) {
            InitializeExcitePosition(state);
            ResetExciteGame(state);
            EV_LoadCutsceneFromCurrentScene("0116E000.smp");
            EV_FreeAuthorPak(1, "0116");
            game.state = 1;
        }
        return;
    case 1:
        StartExciteIntro((game.flags & 4) != 0);
        if (game.flags & 2) {
            SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            game.flags &= ~2U;
        }
        game.waitElapsed = 0;
        game.waitTarget = 200;
        game.state = 2;
        return;
    case 2:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 3;
        game.waitElapsed = 0;
        return;
    case 3:
        if (shenGetCtrlSpeakIconFlag() == 36 || shenGetCtrlSpeakIconFlag() == 37) {
            SetCurrentGameFlag(5, 1);
            game.state = 4;
        } else if (shenGetCtrlSpeakIconFlag() == 34 || shenGetCtrlSpeakIconFlag() == 23 || !GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            game.flags |= 16;
            game.score = game.continued;
            game.state = 19;
        }
        return;
    case 4:
        if (game.flags & 1) {
            StartExciteFreeIntro();
            game.waitElapsed = 0;
            game.waitTarget = 28;
            game.state = 5;
        } else if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) - 5 >= 0) {
            StartExcitePaidIntro((game.flags & 4) != 0);
            game.waitElapsed = 0;
            game.waitTarget = 145;
            game.state = 5;
        } else {
            game.score = game.continued;
            game.state = 31;
        }
        return;
    case 5:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        hndlSetFlags(ctrlGetCharaByName(EXQT_RYO));
        game.waitElapsed = 0;
        game.waitTarget = 40;
        game.state = 6;
        return;
    case 6:
        SetSharedPrngSeed(7);
        ExciteRemovedLogger();
        HideExciteMachineParts();
        spmFadeOut();
        EV_LoadAudio("01160000.sbt", 1);
        EV_spmBeginSampleExecute();
        for (std::int32_t i = 0; i < 6; ++i)
            QueueExcitePart(170, (i & 1) == 0, 14 * i);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitElapsed = 0;
        game.waitTarget = 84;
        game.state = 7;
        return;
    case 7:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        SetExciteMachinePart(169, 1);
        SetExciteMachinePart(170, 1);
        SetExciteMachinePart(176, 1);
        UpdateExciteLives(state);
        game.waitElapsed = 0;
        game.waitTarget = 40;
        QueueExciteStageDisplay(state);
        state.patternStage = 1;
        game.state = 8;
        return;
    case 8:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        spmFadeOut();
        EV_LoadAudio("01160000.sbt", 0);
        EV_spmBeginSampleExecute();
        game.waitElapsed = 0;
        game.waitTarget = 0;
        game.state = 9;
        return;
    case 9:
        if (state.immediateAdvance == 1)
            game.state = 11;
        else {
            if (game.waitElapsed < game.waitTarget)
                break;
            game.state = 11;
            game.waitElapsed = 0;
        }
        return;
    case 11:
        BuildExciteInput(state);
        game.state = 12;
        return;
    case 12:
        if (PollExciteInput(input))
            game.state = 13;
        return;
    case 13:
        if (static_cast<std::int32_t>(input.window - input.elapsed) <= 0)
            game.timingBonus = 0;
        else {
            const float bonus = static_cast<float>(100 * (input.window - input.elapsed + 2) / input.window);
            game.timingBonus = bonus < 100 ? bonus : 100;
        }
        if (ConsumeExciteInput(input)) {
            ++game.successes;
            game.state = 15;
        } else {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.state = 16;
        }
        return;
    case 15: {
        const std::int32_t base = 1000 * game.successes;
        game.cumulativeBonus += game.timingBonus;
        game.state = 9;
        const std::int32_t score = static_cast<std::int32_t>(game.cumulativeBonus + base);
        game.score = score > 99999000 ? 99999000 : score;
        return;
    }
    case 16:
        state.immediateAdvance = 0;
        if (state.lives <= 1) {
            ExciteLostLife(true);
            state.retryFrames = 0;
        } else {
            --state.lives;
            UpdateExciteLives(state);
            ExciteLostLife(false);
            state.retryFrames = game.waitTarget ? game.waitTarget : 1;
        }
        game.waitElapsed = 0;
        game.waitTarget = 55;
        ProcessAicaCommand(0x1200a0, 0);
        EVT_AT_SetSound(0x187, -1);
        game.state = 17;
        return;
    case 17:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        if (state.retryFrames) {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.waitTarget = 60;
            spmFadeOut();
            EV_LoadAudio("01160000.sbt", 0);
            EV_spmBeginSampleExecute();
            game.state = 9;
        } else if (game.score < game.best)
            game.state = 23;
        else {
            game.best = game.score;
            game.state = 21;
        }
        return;
    case 18:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 12;
        game.waitElapsed = 0;
        return;
    case 19: {
        const std::uint32_t flags = game.flags;
        const std::int32_t choice = ((flags >> 1) & 2) + shenGetRandomInt32(((flags & 4) | 8) >> 2);
        if (choice == 0)
            EVT_AT_SetVoice("0116A010", 1, 1); // @text EN: "No, I won't." | JA: "やっぱり、やめよう。"
        else if (choice == 1 || choice == 2)
            EVT_AT_SetVoice("0116A011", 1, 1); // @text EN: "I guess not today." | JA: "今日はやめとくか。"
        else if (choice == 3)
            EVT_AT_SetVoice("0116A020", 1, 1); // @text EN: "Had enough for one day." | JA: "今日はもういいか…"
        else if (choice == 4)
            EVT_AT_SetVoice("0116A038", 1, 1); // @text EN: "I should get going." | JA: "ここまでにしよう…"
        HLib::SetupTaskDelay(40);
        game.state = 20;
        return;
    }
    case 20:
        DestroyElementByIndex(UIElement::QteMinigame);
        if (game.flags & 8) {
            ResetExciteMachineDisplay();
            ExitExcite(state);
            game.state = 0;
        } else {
            game.flags |= 8;
            SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(16);
        }
        return;
    case 21:
        EVT_AT_SetSound(0x185, -1);
        ExciteHighScoreReaction();
        game.waitElapsed = 0;
        game.waitTarget = 85;
        game.state = 22;
        return;
    case 22:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 23;
        game.waitElapsed = 0;
        return;
    case 23:
        HideExciteMachineParts();
        SetExciteMachinePart(166, 1);
        QueueExciteResultDisplay(state);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitElapsed = 0;
        game.waitTarget = 84;
        game.state = 24;
        return;
    case 24:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 25;
        game.waitElapsed = 0;
        return;
    case 25:
        FinishLoading();
        ExciteFinishedReaction(game.score, game.best);
        game.waitElapsed = 0;
        game.waitTarget = 100;
        game.state = 26;
        return;
    case 26:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            game.continued = game.score;
            ResetExciteGame(state);
            game.flags |= 4;
            RestoreExciteHands();
            game.state = 1;
        } else
            game.state = 20;
        return;
    case 31: {
        const std::int32_t choice = shenGetRandomInt32(3);
        EV_PlayDialogue(choice == 0 ? "0116A006" : choice == 1 ? "0116A007" : "0116A008");
        game.waitTarget = choice == 0 || choice == 1 ? 35 : 62;
        game.state = 32;
        game.waitElapsed = 0;
        return;
    }
    case 32:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 20;
        game.waitElapsed = 0;
        return;
    default:
        return;
    }
    ++game.waitElapsed;
}
const ScriptAreaDefinition exqtScriptDefinition{.areaId = EXQT_AREA, .field04 = 0, .init = InitializeExqt, .main = RunExqt};
} // namespace shenmue::s2::areas
