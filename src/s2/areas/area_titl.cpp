#include "s2/areas/area_titl.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {

void InitializeTitle() {
}
void TitleGameLocalNullsub() {
}
void RunTitle(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(TitleMainCallback, 6, 0x0b, sizeof(TitleWork), AREA_EVENT_TASK);
    TitleWork *work = static_cast<TitleWork *>(HLib::GetTaskParameter(task));
    work->flags = *reinterpret_cast<const std::uint32_t *>(argument);
    const std::uint32_t best = static_cast<std::uint32_t>(GetSavedMinigameScore(5));
    work->bestScore = best > 30000 ? best : 30000;
    work->previousScore = static_cast<std::uint32_t>(GetSavedMinigameScore(6));
    work->score = static_cast<std::uint32_t>(GetSavedMinigameScore(7));
    work->allowCancel = 0;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_SetNewSceneIndex();
    EV_spmBeginSampleExecute();
    scenCopyCharaToGame(QTE_GAME_TITLE);
    SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->score, &QTE_TITLE_SCORE_POSITIONS[0], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->bestScore, &QTE_TITLE_SCORE_POSITIONS[1], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->previousScore, &QTE_TITLE_SCORE_POSITIONS[2], 1, nullptr);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TITL"), 0);
    EV_FreeAuthorPak(1, "0115");
    SetLowQualityHands(ctrlGetCharaByName(TITL_RYO));
    SetHighModelXYZ(ctrlGetCharaByName(TITL_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(TITL_RYO), 1, 1, 0);
    for (std::uint32_t i = 0; i < 3; ++i) {
        HLib::Task *mittTask = HLib::EnqueueTaskWithParameter(TitleMittIdle, 6, 0x0b, sizeof(TitleMittWork), AREA_EVENT_TASK);
        TitleMittWork *mitt = static_cast<TitleMittWork *>(HLib::GetTaskParameter(mittTask));
        work->mitts[i] = mitt;
        mitt->task = mittTask;
        mitt->character = TITL_MITTS[i];
        mitt->initialAngle = 57344;
        mitt->raised = 0;
        mitt->phase = 0;
    }
    work->state = TitleWaitFadeClear;
    work->waitFrames = 0;
    InitializeTitleSeed(work);
    work->scoreSources[0] = &work->score;
    work->scoreSources[1] = &work->bestScore;
    work->scoreSources[2] = &work->previousScore;
    EV_EnsureCharacterDraw(QTE_GAME_TITLE);
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(TITL_RYO), 0xf002, MotionRegion::LowerBody);
    EventSetPositionAngle position{
        .cleanup = 0, .character = TITL_RYO, .parent = QTE_GAME_TITLE, .position = {0.0729999989271164f, 0, 1.531999945640564f}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = TITL_RYO, .animation = 0xf002, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf002);
    EV_SetCharaControllerFlags(TITL_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(TITL_RYO, 3, NpcCharacterControllerState::Disabled);
    work->step = 0;
}
namespace {
std::uint16_t SayTitlePrice(bool continued) {
    std::uint16_t frames = 50;
    if (continued)
        EV_PlayDialogue("0115A035");
    else if (shenGetRandomInt32(2)) {
        EV_PlayDialogue("0115A025");
        frames = 100;
    } else
        EV_PlayDialogue("0115A020");
    EVT_AT_SetCam(7305, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, -1);
    switch (shenGetRandomInt32(5)) {
    case 0:
        EVT_AT_SetSound(0x18d, frames + 10);
        break;
    case 1:
        EVT_AT_SetSound(0x18e, frames + 10);
        EVT_AT_SetSound(0x18f, frames + 40);
        break;
    case 2:
        EVT_AT_SetSound(0x190, frames + 10);
        break;
    case 3:
        EVT_AT_SetSound(0x191, frames + 10);
        break;
    case 4:
        EVT_AT_SetSound(0x192, frames + 10);
        break;
    default:
        break;
    }
    return frames;
}
} // namespace
void PrepareTitlePaidIntro() {
    EVT_AT_SetCam(7344, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, -1);
    EVT_AT_SetCam(7345, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, 200);
    EventSetMotion motion{.cleanup = 0, .character = TITL_RYO, .animation = 5313, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5313);
    EVT_AT_SetMot(TITL_RYO, 5313, 0, 1, 0x40000000U, 10);
    GetMotionFrameCount(5313);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-5), 120);
    EVT_AT_SetSound(381, 70);
    EVT_AT_SetSound(380, 190);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    *display = {.cleanup = 1, .character = TITL_RYO, .mode = 1, .delay = 200};
    task = HLib::EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 0x0b, sizeof(EventControlHand), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventControlHand *hand = static_cast<EventControlHand *>(HLib::GetTaskParameter(task));
    hand->cleanup = 1;
    hand->character = TITL_RYO;
    hand->side = 0;
    hand->command = reinterpret_cast<std::byte *>(titleCoinHandCommand);
    hand->ticks = 0;
    hand->delay = 1;
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling *cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = TITL_COIN;
    cling->character = TITL_RYO;
    cling->bone = 14;
    cling->position = {0.13699999451637268f, 0.0430000014603138f, -0.014999999664723873f};
    cling->rotation = {5461, 9284, 63351};
    cling->mode = 0;
    cling->delay = 80;
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = TITL_COIN;
    cling->character = shen_name{};
    cling->mode = 1;
    cling->delay = 180;
}
void PrepareTitleHands(TitleWork *work) {
    work->previousScore = work->score;
    work->score = 0;
    EventCling cling;
    cling.cleanup = 0;
    cling.target = TITL_COIN;
    cling.character = shen_name{};
    cling.mode = 1;
    cling.delay = 0;
    EVT_AT_Cling(&cling);
    scenCopyCharaToGame(QTE_GAME_TITLE);
    SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->score, &QTE_TITLE_SCORE_POSITIONS[0], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->bestScore, &QTE_TITLE_SCORE_POSITIONS[1], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->previousScore, &QTE_TITLE_SCORE_POSITIONS[2], 1, nullptr);
    EV_EnableCharacterDisplay(TITL_LIVES[0]);
    EV_EnableCharacterDisplay(TITL_LIVES[1]);
    EV_EnableCharacterDisplay(TITL_LIVES[2]);
    EVT_AT_SetCam(8342, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, 60);
    Queue_EVT_AT_CtrlHandUp_1(TITL_RYO, 0, reinterpret_cast<std::byte *>(titleFistHandCommand), 0, 60);
    Queue_EVT_AT_CtrlHandUp_1(TITL_RYO, 1, reinterpret_cast<std::byte *>(titleFistHandCommand), 0, 60);
    EVT_AT_SetMot(TITL_RYO, 5315, 0, 1, 0x40000000U, 60);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    *display = {.cleanup = 1, .character = TITL_RYO, .mode = 0, .delay = 60};
}
void FinishTitleMotion(TitleWork *work) {
    EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, -0.072999999f, 1.5319999f, 0, -1);
    EVT_AT_SetMot(TITL_RYO, work->score < work->bestScore ? 5318 : 5314, 0, 1, 0x40000000U, -1);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    *display = {.cleanup = 1, .character = TITL_RYO, .mode = 1, .delay = -1};
    EVS_LoopCamera(7304, QTE_GAME_TITLE, QTE_GAME_TITLE);
    if (work->score < work->bestScore) {
        const std::int32_t random = shenGetRandomInt32(3);
        EVT_AT_SetSound(405, -1);
        EVT_AT_SetSound(406 + random, 30);
    } else
        EVT_AT_SetSound(409 + shenGetRandomInt32(5), -1);
}
void TitleRewardCallback(void *) {
    if (!filmCheckFadeTrackColour()) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1733, 1);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TITL"), 0);
        HLib::MarkCurrentTaskForCleanup();
        scenFreeEvent("TITL.EV3");
    }
}
void TitleOmakeExitCallback(void *) {
    if (!filmCheckFadeTrackColour()) {
        HLib::MarkCurrentTaskForCleanup();
        scenFreeEvent("TITL.EV3");
        EV_scenQueueLoad(g_actNumber, TITL_OMAK, 1);
    }
}
void ExitTitle(TitleWork *work) {
    for (std::uint32_t i = 0; i < 3; ++i)
        HLib::DestroyTaskInternal(work->mitts[i]->task);
    hndlSetFlags(ctrlGetCharaByName(TITL_RYO));
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_SetNewSceneIndex();
    spmFadeOut();
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    }
    EV_FreeAuthorPak(0, "0115");
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_NPC_NewLoadSw(1);
    hndlSetFlags(ctrlGetCharaByName(TITL_RYO));
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        HLib::SetCurrentTaskTo(TitleOmakeExitCallback);
    else {
        EVS_LoopCamera(0, shen_name{}, shen_name{});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        if (work->score < 300000 || (work->flags & 0x10)) {
            HLib::MarkCurrentTaskForCleanup();
            SetPlayerControlEnabled(1);
            scenFreeEvent("TITL.EV3");
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TITL"), 0);
            SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        } else
            HLib::SetCurrentTaskTo(TitleRewardCallback);
    }
}
void TitleMainCallback(void *data) {
    TitleWork *work = static_cast<TitleWork *>(data);
    EV_EventSetUpHelpIcon_0(0x1b, 0);
    if (work->frame < work->waitFrames) {
        if (work->allowCancel && HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work->frame = work->waitFrames;
        } else
            ++work->frame;
        return;
    }
    work->allowCancel = 0;
    switch (work->state) {
    case TitleWaitFadeClear:
        if (!filmCheckFadeTrackColour()) {
            work->frame = 0;
            work->waitFrames = 0;
            work->state = TitleLoadIntro;
        }
        return;
    case TitleLoadIntro:
        EV_LoadCutsceneFromCurrentScene("0115E000.smp");
        ProcessAicaCommand(0x1200a0, 0);
        work->state = TitleAskPlay;
        break;
    case TitleAskPlay:
        work->waitFrames = SayTitlePrice((work->flags & 4) != 0);
        if (work->flags & 2) {
            SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~2U;
        }
        work->frame = 0;
        work->state = TitleShowChoice;
        work->allowCancel = 1;
        return;
    case TitleShowChoice:
        EVT_AT_KsIcon_Enqueue(-1, 36, 23, -1, 300, 23, 0, 0);
        work->state = TitleWaitChoice;
        [[fallthrough]];
    case TitleWaitChoice:
        if (shenGetCtrlSpeakIconFlag() == 36) {
            SetCurrentGameFlag(4, 1);
            if (work->flags & 1)
                work->state = TitlePrepareHands;
            else if (GetPlayerStat(PlayerStatsId::Cash) - 5 >= 0) {
                work->state = TitleTakeMoney;
                const char *voices[]{"0115A003", "0115A004", "0115A005", "0115A036", "0115A037", "0115A038"};
                EV_PlayDialogue(voices[((work->flags & 4) ? 3 : 0) + shenGetRandomInt32(3)]);
                HLib::SetupTaskDelay(40);
            } else
                work->state = TitleNoMoney;
        } else if (shenGetCtrlSpeakIconFlag() == 23 || !GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            const std::int32_t choice = (work->flags & 4) ? 0 : shenGetRandomInt32(3) + 1;
            const char *voices[]{"0115A039", "0115A009", "0115A010", "0115A011"};
            EV_PlayDialogue(voices[choice]);
            HLib::SetupTaskDelay(choice >= 2 ? 50 : 40);
            work->flags |= 0x10;
            work->state = TitleExit;
        }
        break;
    case TitleTakeMoney:
        PrepareTitlePaidIntro();
        work->frame = 0;
        work->waitFrames = 290;
        work->state = TitlePrepareHands;
        return;
    case TitlePrepareHands:
        PrepareTitleHands(work);
        work->frame = 0;
        work->waitFrames = 160;
        work->state = TitleEnterGame;
        work->allowCancel = 1;
        return;
    case TitlePostGame:
        FinishTitleMotion(work);
        SetSavedMinigameScore(SavedScoreType::QTETitle_0, work->bestScore);
        SetSavedMinigameScore(SavedScoreType::QTETitle_1, work->previousScore);
        SetSavedMinigameScore(SavedScoreType::QTETitle_2, work->score);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            InitializeTitleSeed(work);
            work->flags |= 4;
            work->state = TitleAskPlay;
        } else
            work->state = TitleExit;
        work->frame = 0;
        work->waitFrames = 150;
        work->allowCancel = 1;
        return;
    case TitleExit:
        if (work->flags & 8) {
            work->frame = 0;
            work->waitFrames = 0;
            work->allowCancel = 0;
            ExitTitle(work);
        } else {
            work->flags |= 8;
            SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(16);
        }
        return;
    case TitleEnterGame:
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(TITL_RYO), 0, reinterpret_cast<std::byte *>(titleFistHandCommand), 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(TITL_RYO), 1, reinterpret_cast<std::byte *>(titleFistHandCommand), 0);
        HLib::SetCurrentTaskTo(TitleGameCallback);
        return;
    case TitleResults:
        work->state = TitlePostGame;
        break;
    case TitleNoMoney: {
        const std::int32_t random = shenGetRandomInt32(3);
        const char *voices[]{"0115A006", "0115A007", "0115A008"};
        EV_PlayDialogue(voices[random]);
        work->frame = 0;
        work->state = TitleExit;
        work->waitFrames = random == 2 ? 79 : 44;
        return;
    }
    default:
        return;
    }
    work->frame = 0;
    work->waitFrames = 0;
    work->allowCancel = 0;
}
void TitleGameCallback(void *data) {
    TitleWork *work = static_cast<TitleWork *>(data);
    switch (work->state) {
    case TitleEnterGame:
        TitleGameLocalNullsub();
        InitializeTitleSeed(work);
        ShowElementByIndex(UIElement::QteMinigame);
        SetQteMinigameTitleButtons(work->indicators);
        EV_spmBeginSampleExecute();
        EV_LoadAudio("01150000.sbt", 2);
        spmFadeOut();
        work->state = TitleGameBoot;
        work->waitFrames = 5;
        break;
    case TitleGameBoot:
        EV_DisableCharacterDisplay(TITL_RYO);
        EVS_LoopCamera(7340, QTE_GAME_TITLE, QTE_GAME_TITLE);
        BuildTitlePattern(work);
        work->step = 0;
        work->state = TitleStartCamera;
        if (work->replayAudio) {
            EV_spmBeginSampleExecute();
            EV_LoadAudio("01150000.sbt", 2);
            spmFadeOut();
            work->replayAudio = 0;
            work->waitFrames = 31;
        }
        break;
    case TitleStartCamera:
        ConfigureTitleMitts(work);
        work->waitFrames = static_cast<std::uint16_t>(static_cast<std::uint32_t>(work->timingFrames) >= 10 ? 10 : work->timingFrames);
        work->state = TitleSetupRound;
        break;
    case TitleSetupRound: {
        work->requireB = 0;
        work->requireA = 0;
        work->requireX = 0;
        work->inputWindow = work->timingFrames;
        work->inputFrames = 0;
        work->firstComboFrame = 0;
        work->buttons = 0;
        const std::int32_t end = work->pattern[0] == 1 ? 2 : work->step + 1;
        for (std::int32_t i = work->step; i < end; ++i) {
            switch (work->pattern[i + 1]) {
            case 0:
            case 1:
                work->requireA = 1;
                break;
            case 3:
            case 4:
                work->requireX = 1;
                break;
            case 6:
            case 7:
                work->requireB = 1;
                break;
            default:
                break;
            }
        }
        work->frame = 1;
        work->state = TitlePrepareInput;
        work->waitFrames = static_cast<std::uint16_t>(work->inputWindow);
        break;
    }
    case TitlePrepareInput:
        if (PollTitleButtons(work)) {
            work->frame = 0;
            work->waitFrames = 0;
            work->state = TitleInput;
        } else if (work->frame < work->waitFrames)
            ++work->frame;
        else {
            work->frame = 0;
            work->waitFrames = 0;
            work->state = TitleInput;
        }
        break;
    case TitleInput:
        if (ConsumeTitleButtons(work)) {
            CalculateTitleTimingBonus(work);
            if (AdvanceTitlePatternStep(work)) {
                EVT_AT_ObjectDisp_1(TITL_RYO, 1, 0);
                ++work->successes;
                work->completedSteps = work->step;
                work->state = TitleScoreInput;
                work->step = 0;
            } else {
                ++work->successes;
                work->state = TitleSetupRound;
            }
        } else {
            work->completedSteps = work->step;
            work->step = 0;
            work->state = TitleScoreInput;
            EVT_AT_ObjectDisp_1(TITL_RYO, 1, 0);
        }
        break;
    case TitleScoreInput: {
        const std::uint16_t mode = work->pattern[0];
        const std::uint32_t length = (mode == 0 || mode == 3) ? 1 : mode <= 2 ? 2 : mode == 4 ? 3 : 5;
        if (work->completedSteps <= static_cast<std::uint32_t>(work->step))
            work->state = work->completedSteps >= length ? TitleNextRound : TitleLostRound;
        else {
            work->score += work->stepScore + static_cast<std::uint32_t>(static_cast<std::int64_t>(work->timingBonus[work->step]));
            if (work->score >= work->bestScore)
                work->bestScore = work->score;
            PlayTitleStepMotion(work, true);
            work->waitFrames = work->step ? 6 : 15;
            ++work->step;
        }
        break;
    }
    case TitleLostRound:
        ProcessAicaCommand(0x1200a0, 0);
        work->waitFrames = 30;
        work->step = 2;
        if (!work->skipEffect)
            PlayTitleStepMotion(work, false);
        EVT_AT_SetSound(0x194, -1);
        if (work->lives > 1) {
            if (work->lives == 3)
                EVT_AT_SetSound(418, 100);
            else if (work->lives == 2)
                EVT_AT_SetSound(416, 100);
            EVT_AT_SetCam(7303, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, -1);
        } else {
            if (work->pattern[0] == 4)
                SetTitleLaneCamera(9);
            else
                EVT_AT_SetCam(7305, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, -1);
            EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, -0.072999999f, 1.5319999f, 0, work->waitFrames + 60);
            EVT_AT_SetCam(7341, QTE_GAME_TITLE, QTE_GAME_TITLE, 0, 0, work->waitFrames + 60);
        }
        work->state = TitleLowerMitts;
        break;
    case TitleNextRound:
        AdvanceTitleDifficulty(work);
        work->state = TitleGameBoot;
        break;
    case TitleLowerMitts:
        for (std::uint32_t i = 0; i < 3; ++i)
            ApplyTitleMittBehavior(work->mitts[i], work->mitts[i]->raised ? TitleMittMode::Down : TitleMittMode::Sleep, 0, 0);
        work->state = TitleRoundFinished;
        if (work->lives <= 1)
            work->waitFrames = 60;
        break;
    case TitleRoundFinished:
        ShowTitleLives(work);
        if (work->lives-- == 1) {
            std::uint8_t flash = 6;
            if (work->score >= work->bestScore) {
                EV_spmBeginSampleExecute();
                EV_LoadAudio("01150000.sbt", 1);
                spmFadeOut();
                flash = 4;
            }
            QueueTitleScoreFlashes(work, flash);
            work->state = TitleResults;
            work->waitFrames = 220;
        } else {
            UpdateTitleScoreDisplay(work);
            work->replayAudio = 1;
            work->state = TitleGameBoot;
            EVT_AT_SetSound(0x1a1, 120);
            work->waitFrames = 140;
        }
        break;
    case TitleResults:
        FinishLoading();
        HLib::SetCurrentTaskTo(TitleMainCallback);
        DestroyElementByIndex(UIElement::QteMinigame);
        break;
    default:
        break;
    }
    if (work->state != TitleRoundFinished)
        UpdateTitleScoreDisplay(work);
    if (work->waitFrames && !work->frame) {
        HLib::SetupTaskDelay(work->waitFrames);
        work->waitFrames = 0;
    }
}
const ScriptAreaDefinition titlScriptDefinition{.areaId = QTE_GAME_TITLE, .field04 = 0, .init = InitializeTitle, .main = RunTitle};
} // namespace shenmue::s2::areas
