#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
std::uint8_t patiInitializationBytes[8];
void CopyPatiArgument(char *target, const char *source) {
    for (std::int32_t i = 0; i < 7; ++i) {
        target[i] = source[i];
        if (!source[i])
            break;
    }
    target[7] = 0;
}
void HidePatiActiveBall(PatiEventWork *work) {
    if (work->game.activeBall >= 0) {
        EV_DisableCharacterDisplay(work->game.ballCharacters[work->game.activeBall].character);
        work->game.activeBall = -1;
    }
}
void SetPatiPlayerCamera(PatiEventWork *work) {
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, work->delay);
}
void QueuePatiEvent(const char *argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PatiEventCallback, 5, 0x0b, sizeof(PatiEventWork), SHEN_NAME(EVNT));
    PatiEventWork *work = static_cast<PatiEventWork *>(HLib::GetTaskParameter(task));
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->soundScene = GetSelectedSoundSceneIndex();
    std::strcpy(work->sbt, shenGetCurrentSbt());
    std::strcpy(work->bgm, GetCurrentBgm());
    EV_GetCharaPosition(PATI_PLAYER, CHARACTER_TRANSLATION_ROOT, &work->game.savedPlayerPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(PATI_PLAYER, &work->game.savedPlayerRotation, EventTransformFlags::World);
    work->game.parent = task;
    work->played = 0;
    work->payout = 0;
    work->pendingMoney = 0;
    work->game.balls = 0;
    work->game.wins = 0;
    work->game.result = 0;
    work->dealer = {};
    work->challengeComplete = 0;
    work->state = 0;
    work->intro = 1;
    work->totalBet = 0;
    work->savedMotion = -1;
    for (PatiBallCharacter &ball : work->game.ballCharacters)
        ball.character = {};
    g_patiCameraId = 0;
    g_patiCameraEye = {};
    g_patiCameraTarget = {};
    g_patiCameraMoving = 1;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1290, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1291, 0);
    LockPatiEvent(work, argument);
}
} // namespace
// @verified - p
void PATIMain(std::uintptr_t argument) {
    patiGameplayPlacements[0].yaw = static_cast<std::int32_t>(PATI_HAND_MODEL.name);
    g_patiModelAlpha = 0.0f;
    g_patiVersus = 0;
    QueuePatiEvent(reinterpret_cast<const char *>(argument));
}
// @verified - p
void PATIInit() {
    patiGameplayPlacements[0].yaw = static_cast<std::int32_t>(PATI_HAND_MODEL.name);
    g_patiVersus = 0;
    g_patiModelAlpha = 0.0f;
    g_patiMuseumMode = 0;
    g_patiSingleMode = 0;
    g_patiMinimumBet = 0;
    g_patiCameraMoving = 0;
    g_patiCameraPosition = {};
    g_patiCameraLookAt = {};
    g_patiFilteredEye = {};
    g_patiFilteredTarget = {};
    g_patiDesiredEye = {};
    g_patiDesiredTarget = {};
    g_patiCameraHeight = 0.0f;
    g_patiCameraStep = 0;
    g_patiSelectionAccepted = 0;
    g_patiInspectRequested = 0;
    g_patiHeightRepeat = 0;
    g_patiBoardSize = 0;
    g_patiBoardRotation = {};
    g_patiBoardPosition = {};
    g_patiPlayerCamera = 0;
    g_patiDealerCamera = 0;
    g_patiCameraId = 0;
    g_patiCameraEye = {};
    g_patiCameraTarget = {};
    g_patiCameraTime = 0;
    ResetPatiNailModelState();
    g_patiDealerInputClock = 0;
    g_patiDealerInputIndex = 0;
    for (std::uint8_t &value : patiInitializationBytes)
        value = 0;
}
const ScriptAreaDefinition patiScriptDefinition{PATI_AREA, 0, PATIInit, PATIMain};
// @verified - p
void PatiEventCallback(void *data) {
    auto *work = static_cast<PatiEventWork *>(data);
    g_patiCameraTime = 99999;
    work->delay = 0;
    if (GetShouldRenderIcons(RenderIconSlot::Any))
        return;
    const std::uint8_t extended = static_cast<std::uint8_t>(GetCurrentControlInput(INPUT_HELD));
    const std::int8_t entryState = work->state;
    switch (entryState) {
    case 0:
        if (!LoadPatiEvent(work))
            return;
        for (std::uint32_t i = 0; i < 4; ++i) {
            g_patiMotionCache[i].character = {};
            g_patiMotionCache[i].age = i;
        }
        CopyPatiArgument(work->selectedName, work->argument);
        work->game.mode = 0;
        if (!std::memcmp(work->argument, "PVSJ", 4)) {
            FindPatiDealerBoards(work, work->dealerGroup);
            CopyPatiArgument(work->selectedName, work->versusBoard);
            work->option = 0;
            work->type = 8;
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 26;
            std::memcpy(&work->dealer, work->opponent, 4);
        } else if (work->argument[0] == 'B' && work->argument[1] == 'D' && static_cast<std::uint8_t>(work->argument[2] - '0') <= 9 &&
            static_cast<std::uint8_t>(work->argument[3] - '0') <= 9) {

            FindPatiDealerBoards(work, nullptr);
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 26;
        } else {
            std::memcpy(&work->dealer, work->argument, 4);
            FindPatiDealerBoards(work, work->argument);
            ConfigurePatiEventBoard(work, work->selectedName);
            work->state = g_patiMuseumMode ? 1 : 14;
        }
        if (work->nearbyDealer.name && work->nearbyDealer.name != work->dealer.name)
            EV_npcLockUnlock(work->nearbyDealer);
        adfsSetCameraIdxForChara(work->dealer, SHEN_NAME(damy));
        if (FindPatiDealer(work).name)
            CreatePatiEventResources(work);
        else {
            PatiMissingDealer(work);
            work->state = 61;
        }
        break;
    case 1:
        EV_EventSetUpHelpIcon_0(144, 0);
        PositionPatiPlayer(work);
        PatiMuseumIntro(work);
        work->state = 2;
        break;
    case 2:
    case 20:
        EV_DisableCharacterDisplay(PATI_PLAYER);
        if (DoesScenarioCharacterExist(PATI_BACKPACK))
            EV_DisableCharacterDisplay(PATI_BACKPACK);
        CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
        InitializePatiBoardCamera(work);
        work->selectionFrames = 0;
        [[fallthrough]];
    case 3:
    case 21:
        SetupPatiBoard(0);
        work->state = entryState < 20 ? 4 : 22;
        break;
    case 4:
    case 22: {
        if (++work->selectionFrames >= 5400 || shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        const std::int32_t selection = SelectPatiBoardInput(work);
        if (!selection) {
            if (!g_patiSelectionAccepted) {
                if (extended & 2)
                    g_patiInspectRequested = 1;
                GetBattleDataConfig(0, PATI_INSPECT);
            }
            if (g_patiInspectRequested)
                work->state = entryState == 4 ? 6 : 24;
        } else {
            if (DoesScenarioCharacterExist(PATI_BACKPACK))
                EV_EnableCharacterDisplay(PATI_BACKPACK);
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            if (selection > 0) {
                ConfigurePatiEventBoard(work, work->selectedName);
                SetPatiBet(work);
                work->state = entryState == 4 ? 5 : 23;
            } else if (entryState == 4) {
                EV_EnableCharacterDisplay(PATI_PLAYER);
                PatiCancel(work);
                work->state = 61;
            } else
                work->state = work->type == 8 ? 61 : 13;
        }
        break;
    }
    case 5:
        PatiMuseumBoardSelected(work);
        ConfigurePatiEventBoard(work, work->selectedName);
        work->state = 8;
        break;
    case 6:
    case 24: {
        g_patiInspectRequested = 0;
        if (entryState == 6)
            g_patiCameraStep = 0;
        const Vector3i rotation{0, work->choices[work->selectedBoard].rotation.y, 0};
        AT_SetLocalBaseChara(PATI_CAMERA, shen_name{0xffffffffU});
        EV_AT_SetPos(PATI_CAMERA, EventTransformFlags::World | EventTransformFlags::AllAxes, work->choices[work->selectedBoard].position);
        EV_AT_SetAngY(PATI_CAMERA, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        SetPatiCamera(979, PATI_CAMERA, PATI_CAMERA, 0);
        work->state = entryState == 6 ? 7 : 25;
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        [[fallthrough]];
    }
    case 7:
    case 25:
        if (++work->selectionFrames >= 5400 || shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        if (GetBattleDataConfig(2, PATI_ACCEPT))
            g_patiSelectionAccepted = 1;
        else if (!g_patiSelectionAccepted && !(extended & 2)) {
            g_patiCameraStep = 8;
            work->state = entryState < 20 ? 4 : 22;
            camCtrlGetNextCamPos(&g_patiCameraPosition);
            camCtrlGetTargetPosition(&g_patiCameraLookAt);
            CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
        }
        if (g_patiCameraStep < 30)
            ++g_patiCameraStep;
        else if (g_patiSelectionAccepted) {
            g_patiSelectionAccepted = 0;
            g_patiInspectRequested = 0;
            g_patiCameraStep = -15;
            if (DoesScenarioCharacterExist(PATI_BACKPACK))
                EV_EnableCharacterDisplay(PATI_BACKPACK);
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = entryState < 20 ? 5 : 23;
        }
        if (!g_patiSelectionAccepted) {
            GetBattleDataConfig(0, PATI_ACCEPT);
            GetBattleDataConfig(0, PATI_INSPECT);
        }
        break;
    case 8:
        PositionPatiPlayer(work);
        work->state = 9;
        break;
    case 9:
        EV_DisableCharacterDisplay(PATI_PLAYER);
        SetupPatiBoard(2);
        EVT_AT_KsIcon_Enqueue(402, 36, 369, 34, 10800, 0xffff, 0, work->delay);
        work->state = 10;
        break;
    case 10:
        if (shenGetMinutesUntilNextFrameMinute() > 0) {
            const std::uint16_t choice = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
            if (choice == 36) {
                work->state = 12;
                break;
            }
            if (choice == 369) {
                work->state = 11;
                break;
            }
            if (choice == 402) {
                PatiMuseumReselect(work);
                work->state = 2;
                break;
            }
        }
        EV_EnableCharacterDisplay(PATI_PLAYER);
        PatiCancel(work);
        work->state = 61;
        break;
    case 11:
        EV_EnableCharacterDisplay(PATI_PLAYER);
        PatiExplainMuseumBoard(work);
        work->state = 9;
        break;
    case 12:
        work->intro = 0;
        PatiMuseumPlay(work);
        work->state = 26;
        break;
    case 13:
        if (g_patiMuseumMode)
            work->state = 9;
        else {
            FindPatiDealerBoards(work, work->argument);
            ConfigurePatiEventBoard(work, work->selectedName);
            work->state = 15;
        }
        break;
    case 14:
        PositionPatiPlayer(work);
        PatiGreeting(work);
        work->state = 15;
        break;
    case 15:
        if (work->type == 8)
            work->state = 20;
        else {
            EV_EnableCharacterDisplay(PATI_PLAYER);
            SetPatiPlayerCamera(work);
            EVT_AT_KsIcon_Enqueue(-1, 36, 369, 34, 10800, 0xffff, 0, work->delay);
            work->state = 16;
        }
        break;
    case 16:
        if (shenGetMinutesUntilNextFrameMinute() > 0) {
            const std::uint16_t choice = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
            if (choice == 36) {
                work->state = 18;
                break;
            }
            if (choice == 369) {
                work->state = 17;
                break;
            }
        }
        PatiCancel(work);
        work->state = 61;
        break;
    case 17:
        PatiExplainRules(work);
        work->state = 13;
        break;
    case 18:
        if (g_patiSingleMode) {
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->intro = 0;
            work->state = 26;
        } else {
            PatiAcceptPlay(work);
            PatiChooseBoard(work);
            work->state = 20;
        }
        break;
    case 19:
        FindPatiDealerBoards(work, work->argument);
        work->state = 20;
        break;
    case 23:
        work->intro = 0;
        PatiBoardSelected(work);
        work->state = 26;
        break;
    case 26:
        CopyPatiArgument(work->previousBoard, work->selectedName);
        if (work->type == 8) {
            EV_EnableCharacterDisplay(PATI_PLAYER);
            work->state = 64;
        } else
            work->state = 27;
        break;
    case 27:
        if ((work->option == 1 || work->option == 2) && !std::strncmp(work->excludedBoard, work->board, 4)) {
            PatiRejectSameDoubleBoard(work);
            work->state = 19;
        } else if ((work->option == 4 || work->option == 5) && std::strncmp(work->excludedBoard, work->board, 4)) {
            PatiRejectOtherChallengeBoard(work);
            work->state = 19;
        }
        if (work->state == 27)
            work->state = 28;
        break;
    case 28:
        if (work->bet > static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash))) {
            EV_EnableCharacterDisplay(PATI_PLAYER);
            work->state = 60;
            break;
        }
        [[fallthrough]];
    case 29:
        work->round = 0;
        work->prizeCount = 0;
        work->prizeIndex = 0;
        work->remainingGames = 1;
        work->state = work->option == 7 ? 31 : 30;
        break;
    case 30:
        SetupPatiBoard(2);
        EV_DisableCharacterDisplay(PATI_PLAYER);
        PlacePatiBet(work);
        work->state = 34;
        break;
    case 31:
        EV_EventSetUpHelpIcon_0(147, static_cast<std::uint8_t>(work->delay));
        PatiSelectGameCount(work);
        work->state = 32;
        break;
    case 32:
        SetupPatiBoard(2);
        SetMoneyRelatedUIElement_0(5 * work->bet, 10800, work->bet, 15);
        work->delay += 5;
        work->state = 33;
        break;
    case 33: {
        SetPatiHelpTopic(work->option, static_cast<std::int8_t>(work->type));
        const std::int32_t result = static_cast<std::int32_t>(GetNumBet());
        work->remainingGames = result <= 0 ? result : result / work->bet;
        if (work->remainingGames <= 0) {
            if (work->remainingGames == -2 && shenGetMinutesUntilNextFrameMinute() <= 0)
                work->state = 61;
            else {
                InitializePatiBoardCamera(work);
                work->state = 22;
            }
        } else
            work->state = 30;
        break;
    }
    case 34:
        EV_DisableCharacterDisplay(PATI_PLAYER);
        work->game.balls = 0;
        work->game.wins = 0;
        [[fallthrough]];
    case 35:
        work->game.result = 0;
        patiGameplayPlacements[0].yaw = static_cast<std::int32_t>(PATI_HAND_MODEL.name);
        QueuePatiBall(&work->game);
        HLib::SetupTaskDelay(0);
        work->state = 36;
        break;
    case 36:
        if (work->game.result == -4) {
            HidePatiActiveBall(work);
            PatiCancel(work);
            work->state = 37;
        } else if (work->game.result == 4) {
            HidePatiActiveBall(work);
            PatiClosing(work);
            work->state = 37;
        } else if (work->game.result == -1) {
            HidePatiActiveBall(work);
            PatiStuck(work);
            work->state = 35;
        } else {
            const PatiDealer *dealer = PATI_DEALERS;
            while (dealer->character.name && dealer->character.name != work->dealer.name)
                ++dealer;
            if (dealer->playedFlag >= 0)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(dealer->playedFlag), 1);
            if (work->dealer.name == SHEN_NAME(SF7_))
                SetCurrentGameFlag(7, 1);
            work->played = 1;
            if (HasPatiWon(work)) {
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1290, 1);
                work->state = 38;
            } else {
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1291, 1);
                work->state = 39;
            }
        }
        break;
    case 37:
        RefundPatiBet(work);
        work->state = 61;
        break;
    case 38:
        HidePatiActiveBall(work);
        switch (work->option) {
        case 0:
        case 3:
        case 6:
        case 8:
            PatiWin(work);
            work->state = 40;
            break;
        case 1:
        case 2:
            if (!work->round)
                PatiFirstDoubleWin(work);
            else if (work->round == 5)
                PatiFinalDoubleWin(work);
            else
                PatiDoubleWin(work);
            work->state = work->round == 5 ? 47 : 42;
            if (!work->round)
                work->pendingMoney += GetPatiPayout(work);
            else
                work->pendingMoney *= 2;
            break;
        case 4:
        case 5:
            KeepPatiPrize(work);
            if (!work->round) {
                PatiFirstChallengeWin(work);
                work->state = 53;
            } else if ((work->option == 4 && work->round >= 2) || work->round >= 4) {
                if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1721) == 1 || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1722) == 1)
                    PatiFinalChallengeWin(work);
                else
                    PatiPrizeExhausted(work);
                work->state = 50;
            } else {
                PatiChallengeWin(work);
                work->state = 53;
            }
            break;
        case 7:
            KeepPatiPrize(work);
            PatiPrizeResult(work);
            work->state = 49;
            break;
        default:
            work->state = 61;
            break;
        }
        break;
    case 39:
        HidePatiActiveBall(work);
        work->pendingMoney = 0;
        switch (work->option) {
        case 0:
        case 3:
        case 6:
        case 7:
        case 8:
            PatiLose(work);
            work->state = 41;
            break;
        case 1:
        case 2:
            if (work->round)
                PatiDoubleLose(work);
            else
                PatiLose(work);
            work->state = 48;
            break;
        case 4:
        case 5:
            if (!work->round)
                PatiFirstChallengeLose(work);
            else if ((work->option == 4 && work->round >= 2) || work->round >= 4)
                PatiFinalChallengeLose(work);
            else
                PatiChallengeLose(work);
            if (shenGetMinutesUntilNextFrameMinute() <= 0) {
                PatiClosing(work);
                work->state = 61;
            } else {
                PatiAfterDouble(work);
                work->state = 13;
            }
            break;
        default:
            work->state = 61;
            break;
        }
        break;
    case 40:
        PayPatiWinnings(work);
        work->state = 41;
        break;
    case 41:
        if (shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiClosing(work);
            work->state = 61;
        } else {
            PatiOfferReplay(work);
            work->state = 13;
        }
        break;
    case 42:
        ConfigurePatiEventBoard(work, work->nextBoard);
        InitializePatiBoardCamera(work);
        SetupPatiBoard(2);
        PatiOfferDouble(work);
        work->state = 43;
        break;
    case 43:
    case 54:
        EVT_AT_KsIcon_Enqueue(-1, 74, 75, -1, 10800, 0xffff, 0, work->delay);
        work->state = entryState == 43 ? 44 : 55;
        break;
    case 44:
        work->state = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) == 74 ? 45 : 46;
        break;
    case 45:
        PatiAcceptContinue(work);
        ++work->round;
        work->state = 34;
        break;
    case 46:
        PatiDeclineContinue(work);
        work->state = 47;
        break;
    case 47: {
        const std::int32_t amount = work->pendingMoney;
        work->pendingMoney = 0;
        SetPatiPlayerCamera(work);
        PATI_StartMoneyCount(static_cast<std::uint32_t>(amount), work->delay);
        work->delay += 5;
        work->state = work->round == 5 ? 61 : 48;
        break;
    }
    case 48:
        PatiAfterDouble(work);
        work->state = 13;
        break;
    case 49:
        if (AwardPatiPrize(work)) {
            if (--work->remainingGames > 0)
                work->state = 34;
            else if (shenGetMinutesUntilNextFrameMinute() <= 0) {
                PatiClosing(work);
                work->state = 37;
            } else
                work->state = 41;
        }
        break;
    case 50:
        work->state = 51;
        break;
    case 51:
        if (AwardPatiPrize(work))
            work->state = 52;
        break;
    case 52:
        work->challengeComplete = 1;
        work->state = 61;
        break;
    case 53:
        ConfigurePatiEventBoard(work, work->nextBoard);
        InitializePatiBoardCamera(work);
        SetupPatiBoard(2);
        PatiOfferChallenge(work);
        work->state = 54;
        break;
    case 55:
        work->state = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) == 74 ? 56 : 57;
        break;
    case 56:
        PatiAcceptContinue(work);
        if ((work->option == 4 && work->round >= 1) || work->round >= 3)
            PatiFinalChallengeRound(work);
        ++work->round;
        work->state = 34;
        break;
    case 57:
        PatiDeclineContinue(work);
        work->state = 58;
        break;
    case 58:
        if (AwardPatiPrize(work))
            work->state = 59;
        break;
    case 59:
        if (shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiClosing(work);
            work->state = 61;
        } else {
            PatiAfterChallenge(work);
            work->state = 13;
        }
        break;
    case 60:
        if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < g_patiMinimumBet) {
            PatiInsufficientFunds(work);
            work->state = 61;
        } else {
            PatiLowerStake(work);
            work->state = 13;
        }
        break;
    case 61:
        BeginPatiExit(work);
        work->state = 62;
        break;
    case 62:
        if (RestorePatiEvent(work))
            work->state = 63;
        break;
    case 63:
        FreePatiEvent(work);
        break;
    case 64:
        g_patiVersus = 1;
        work->totalRounds = 0;
        work->playerWins = 0;
        work->dealerWins = 0;
        work->game.targetVariation = 10000;
        work->game.targetX = 0;
        work->state = 68;
        break;
    case 65:
        SetupPatiBoard(2);
        work->game.ballLimit = 1;
        work->game.winLimit = 1;
        work->game.missLimit = 1;
        work->game.reserved36 = 0;
        work->game.balls = 0;
        work->game.wins = 0;
        [[fallthrough]];
    case 66:
        EV_DisableCharacterDisplay(work->dealer);
        work->game.mode = 1;
        work->game.result = 0;
        patiGameplayPlacements[0].yaw = static_cast<std::int32_t>(PATI_HAND_MODEL.name);
        QueuePatiBall(&work->game);
        HLib::SetupTaskDelay(0);
        work->state = 67;
        break;
    case 67:
        EV_EnableCharacterDisplay(work->dealer);
        if (work->game.result == -4 || work->game.result == 4) {
            HidePatiActiveBall(work);
            PatiVersusClosing(work);
            work->state = 61;
        } else if (work->game.result == -1) {
            HidePatiActiveBall(work);
            PatiVersusStuck(work);
            work->state = 66;
        } else {
            HidePatiActiveBall(work);
            if (work->game.wins > 0) {
                PatiPlayerVersusWin(work);
                ++work->playerWins;
            } else
                PatiPlayerVersusLose(work);
            ++work->totalRounds;
            work->state = 71;
            if (work->totalRounds == 2) {
                if (work->playerWins == 2 && !work->dealerWins) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1290, 1);
                    work->state = 72;
                } else if (!work->playerWins && work->dealerWins == 2) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1291, 1);
                    work->state = 73;
                }
            } else if (work->totalRounds != 1) {
                if (work->playerWins > work->dealerWins) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1290, 1);
                    work->state = 72;
                } else if (work->playerWins < work->dealerWins) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1291, 1);
                    work->state = 73;
                }
            }
        }
        break;
    case 68:
        work->game.ballLimit = 1;
        work->game.winLimit = 1;
        work->game.missLimit = 1;
        work->game.reserved36 = 0;
        work->game.balls = 0;
        work->game.wins = 0;
        SetupPatiBoard(2);
        [[fallthrough]];
    case 69:
        EV_DisableCharacterDisplay(work->dealer);
        work->game.mode = 2;
        work->game.result = 0;
        patiGameplayPlacements[0].yaw = static_cast<std::int32_t>(PATI_OPPONENT_HAND.name);
        QueuePatiBall(&work->game);
        HLib::SetupTaskDelay(0);
        work->state = 70;
        break;
    case 70:
        EV_EnableCharacterDisplay(work->dealer);
        if (work->game.result == 4) {
            HidePatiActiveBall(work);
            PatiVersusClosing(work);
            work->state = 61;
        } else if (work->game.result == -1) {
            HidePatiActiveBall(work);
            PatiVersusStuck(work);
            work->state = 69;
        } else {
            HidePatiActiveBall(work);
            if (work->game.wins > 0) {
                PatiDealerVersusWin(work);
                ++work->dealerWins;
            } else
                PatiDealerVersusLose(work);
            if (work->totalRounds == 2) {
                if (work->dealerWins - work->playerWins >= 2) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1291, 1);
                    work->state = 73;
                } else if (work->playerWins > work->dealerWins) {
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1290, 1);
                    work->state = 72;
                }
            }
            if (work->state == 70) {
                PatiPlayerTurn(work);
                work->state = 65;
            }
        }
        break;
    case 71:
        if (shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiVersusClosing(work);
            work->state = 61;
        } else {
            if (work->totalRounds == 3)
                PatiVersusSuddenDeath(work);
            else
                PatiNextVersusRound(work);
            work->state = 68;
        }
        if (work->totalRounds >= 3 && shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiVersusClosing(work);
            work->state = 61;
        }
        break;
    case 72:
        PatiVersusWin(work);
        work->state = 74;
        break;
    case 73:
        PatiVersusLose(work);
        work->state = 75;
        break;
    case 74:
    case 75: {
        if (entryState == 75)
            SetPatiPlayerCamera(work);
        const std::uint32_t cash = static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash));
        const std::uint32_t amount = static_cast<std::uint32_t>(work->bet);
        const std::uint32_t target = entryState == 74 ? cash + amount : cash - amount;
        AnimateMoneyAmount(static_cast<std::int32_t>(cash), static_cast<std::int32_t>(target), 0);
        SetPlayerStat(PlayerStatsId::Cash, target);
        work->delay += 5;
        work->state = 61;
        break;
    }
    default:
        break;
    }
    if (work->delay > 0)
        HLib::SetupTaskDelay(work->delay);
}
} // namespace shenmue::s2
