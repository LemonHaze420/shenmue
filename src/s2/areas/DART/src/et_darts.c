#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_dart.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void DartsAreaMain(EventDartsGame *entry)
{
    HLibTaskCallback callbacks[2] = {DartsInitStart, DartsPositionOpponentHands};
    HLibTask *task;
    DartsEventWork *work;
    if (entry->opponentMatch > 1)
        return;
    task = FTBL.HLib_EnqueueTaskWithParameter(callbacks[entry->opponentMatch], 4, 11, sizeof(DartsEventWork), DART_TASK);
    work = (DartsEventWork *)FTBL.HLib_GetTaskParameter(task);
    if (work) {
        FTBL.HLib_SetupTimedTask(task, 2);
        DartsInitializeRoot(work, task, entry);
        EventDebugRegister(DART_DEBUG_ID, DartsDebugExitCallback, work, 2000000000);
        EventDebugCheck(DART_DEBUG_ID, 4);
    }
}
#else
extern void *memset(void *destination, int value, unsigned int size);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern DartsOpponentInfo dartsOpponents[6];
static int dartsPlayerWon;

void DartsInitializeRoot(DartsEventWork *work, HLibTask *task, EventDartsGame *entry)
{
    memset(work, 0, sizeof(DartsEventWork));
    work->task = task;
    work->entry = entry;
    work->gameFlag = FTBL.SetPlayerGameFlags(4, -1);
    if (DartsSelectBoard(work, 0) != DARTS_COMPACT_BOARD) {
        work->standardBoard = 1;
        if (DartsIsOpponentMatch(work)) {
            if (work->entry->enemy->name == DART_XHO)
                FTBL.EV_EventSetUpHelpIcon_0(48, 0);
            else
                FTBL.EV_EventSetUpHelpIcon_0(76, 0);
        } else
            FTBL.EV_EventSetUpHelpIcon_0(47, 0);
    } else {
        work->standardBoard = 0;
        FTBL.EV_EventSetUpHelpIcon_0(3, 0);
    }
    FTBL.GetCharaPosition(DART_PLAYER, -1, &work->previousPosition, 0);
    FTBL.GetScenarioCharacterEulerAngles(DART_PLAYER, &work->previousRotation, 0);
    memcpy(work->previousBgm, FTBL.GetCurrentBgm(), 16);
    memcpy(work->previousSbt, FTBL.shenGetCurrentSbt(), 16);
    work->previousSoundProgram = FTBL.GetSelectedSoundSceneIndex();
    FTBL.EV_FreeAuthorPak(1, "0113");
    FTBL.EV_FreeAuthorPak(1, "0137");
    FTBL.SetPlayerControlEnabled(0);
    FTBL.CheckHelpIcon(1, 0, 0);
    FTBL.FinishLoading();
    FTBL.SetRefBoardParamsForCharaById(DART_PLAYER, 1);
    FTBL.EV_ControlPackageResources(2, 4, "DARTS", 0, 0);
    DartsInitializeScoreBanks(work);
    DartsPlaceCameraDummies(work);
}

void DartsDebugExitCallback(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    work->gameState = 0;
    FTBL.HLib_AssignCallbackForTask(work->task, DartsExit);
    FTBL.HLib_SetupTimedTask(work->task, 1);
}

void DartsExit(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int i;
    unsigned char plays;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTaskDelay(30);
        break;
    case 1:
        ++work->gameState;
        FTBL.CheckHelpIcon(0, 0, 0);
        DartsRestorePlayer(work);
        DartsRestoreOpponent(work);
        DartsRetireCoinAndArrows(work);
        FTBL.EV_FreeAuthorPak(0, "0137");
        FTBL.EV_FreeAuthorPak(0, "0113");
        for (i = 0; i <= 3; i++)
            if (work->childTasks[i])
                FTBL.HLib_DestroyTaskInternal(work->childTasks[i]);
        FTBL.shenSetTimeSimulationTo(1);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.EVS_LoopCamera(0, 0, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        EVT_AT_SetCam(-1, 0, 0, 0, 0, 0);
        if (FTBL.EV_GetPlayerFlag(11, 1014))
            FTBL.EV_scenQueueLoad(g_actNumber, DART_REWARD_AREA, 1);
        else {
            FTBL.EV_LoadCutsceneFromCurrentScene(work->previousBgm);
            FTBL.EV_LoadAudio(work->previousSbt, work->previousSoundProgram);
            EVT_AT_SetSample(0, 0, 0);
        }
        break;
    case 2:
        plays = FTBL.EV_GetPlayerFlag(15, 1);
        if (plays <= 254)
            plays++;
        FTBL.EV_SetPlayerFlag(16, 1, plays);
        if (!work->entry->opponentMatch && work->banks[1].score[3] > 2499999)
            FTBL.EV_SetPlayerFlag(12, 1732, 1);
        else if (!DartsIsOpponentMatch(work)) {
            FTBL.SetPlayerControlEnabled(1);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        } else
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EventDebugUnregister(DART_DEBUG_ID);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_HLKillOtherHierarchyTask(work->task);
        FTBL.HLib_ResolveDeferredCallback(work->entry->task);
        break;
    default:
        work->gameState = 0;
        break;
    }
}

void DartsRestorePlayer(DartsEventWork *work)
{
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(DART_PLAYER));
    FTBL.SetScenarioCharacterFlag40(DART_PLAYER, 1);
    FTBL.AT_SetLocalBaseChara(DART_PLAYER, -1);
    FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(DART_PLAYER), 0, 0);
    FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 1);
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 2, 1);
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 3, 1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 1, 1);
    EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x40000000, 0);
    if (DartsIsOpponentMatch(work)) {
        FTBL.EV_AT_SetPos(DART_PLAYER, 0x38000000, &work->previousPosition);
        FTBL.EV_AT_SetAngY_0(DART_PLAYER, 0x38000000, &work->previousRotation, 0);
    } else
        SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.0f, 0.0f, 1.5f, 0, 0);
    if (work->gameFlag)
        FTBL.SetPlayerGameFlags(4, 1);
}

void DartsRestoreOpponent(DartsEventWork *work)
{
    if (!DartsIsOpponentMatch(work))
        return;
    FTBL.SetScenarioCharacterFlag40(work->entry->enemy->name, 1);
    FTBL.AT_SetLocalBaseChara(work->entry->enemy->name, -1);
    FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->entry->enemy->name), 0, 0);
    FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 1);
    DartsBeginOpponentThrow(work->entry->enemy->name, 1, work->difficulty);
    FTBL.InitiateCleanupForSceneChara(work->entry->enemy->name);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->entry->enemy->name));
    if (g_currentMap == DART_AKS0 && work->entry->enemy->name == DART_XHO)
        SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, -0.3f, 0.0f, 3.6f, 0, 0);
    else
        SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 3.6f, 0, 0);
}

void DartsRetireCoinAndArrows(DartsEventWork *work)
{
    int i;
    EVT_AT_SetPos(DART_COIN, -1, 0.0f, 0.0f, 0.0f, 0);
    FTBL.EV_ToggleCharacterFlag(DART_COIN);
    FTBL.EV_DisableCharacterDisplay(DART_COIN);
    FTBL.ResetScenarioCharacterAttachment(DART_COIN);
    for (i = 0; i <= 4; i++) {
        EVT_AT_SetPos(GetDartsArrowName(0, i), -1, 0.0f, 0.0f, 0.0f, 0);
        FTBL.EV_ToggleCharacterFlag(GetDartsArrowName(0, i));
        FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(0, i));
        FTBL.ResetScenarioCharacterAttachment(GetDartsArrowName(0, i));
        if (DartsIsOpponentMatch(work)) {
            EVT_AT_SetPos(GetDartsArrowName(2, i), -1, 0.0f, 0.0f, 0.0f, 0);
            FTBL.EV_ToggleCharacterFlag(GetDartsArrowName(2, i));
            FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(2, i));
            FTBL.ResetScenarioCharacterAttachment(GetDartsArrowName(2, i));
        }
    }
}

void DartsPlaceCameraDummies(DartsEventWork *work)
{
    Vector3 position;
    int index;
    SetDartsPosition(work->entry->placement[work->entry->mode].character, DART_DUMMY0, 0.0f, 0.0f, 4.2f, 0, 0);
    if (DartsIsOpponentMatch(work)) {
        index = DartsOpponentIndex(work->entry->enemy->name);
        SetDartsPosition(work->entry->placement[work->entry->mode].character, DART_DUMMY1, 0.0f, 0.0f, 4.0f, 0, 0);
        FTBL.GetCharaPosition(DART_DUMMY1, -1, &position, 0);
        position.y += dartsOpponents[index].height;
        FTBL.EV_AT_SetPos(DART_DUMMY1, 0x38000000, &position);
    }
}

void DartsInitializeScoreBanks(DartsEventWork *work)
{
    memset(&work->banks[0], 0, sizeof(DartsScoreBank));
    memset(&work->banks[1], 0, sizeof(DartsScoreBank));
    memset(&work->banks[2], 0, sizeof(DartsScoreBank));
    work->banks[2].bestScore = work->entry->bestScore;
    work->banks[2].previousScore = work->entry->opponentScore;
    if (work->standardBoard) {
        work->banks[0].bestScore = work->banks[1].bestScore = FTBL.GetSavedMinigameScore(2);
        work->banks[0].previousScore = work->banks[1].previousScore = FTBL.GetSavedMinigameScore(3);
    } else {
        work->banks[1].bestScore = FTBL.GetSavedMinigameScore(12);
        work->banks[1].previousScore = FTBL.GetSavedMinigameScore(13);
    }
    work->scoreUiFlags = 0;
    work->field0e = 15;
}

void DartsInitStart(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int i;
    if (!work)
        FTBL.HLib_DestroyCurrentTask();
    else {
        FTBL.GetCurrentDateTime((DateTime *)&work->previousTime);
        FTBL.shenSetTimeSimulationTo(0);
        FTBL.SetRenderClockAndCompassTo(1);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(DART_PLAYER), 0);
        EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
        if (DartsIsOpponentMatch(work)) {
            switch (work->entry->enemy->name) {
            case DART_FCJ:
                FTBL.SetMGOTOSIDAMA(9, 1);
                break;
            case DART_HCY:
                FTBL.SetMGOTOSIDAMA(12, 1);
                break;
            case DART_HLN:
                FTBL.SetMGOTOSIDAMA(10, 1);
                break;
            case DART_JON:
                FTBL.SetMGOTOSIDAMA(13, 1);
                break;
            default:
                FTBL.SetMGOTOSIDAMA(11, 1);
                break;
            }
            for (i = 0; i <= 14; i++)
                FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(2, i));
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(work->entry->enemy->name), 0);
            DartsInitStart2(work);
        } else {
            FTBL.HLib_SetCurrentTaskTo(DartsInitStart2);
            FTBL.HLib_SetupTaskDelay(5);
        }
    }
}

void DartsInitStart2(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    do {
        FTBL.SetScenarioCharacterTransparency(DART_PLAYER, 1.0f);
        FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 0);
    } while (0);
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 2, 0);
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 3, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(DART_PLAYER, 1, 0);
    FTBL.SetScenarioCharacterFlag40(DART_PLAYER, 0);
    if (DartsIsOpponentMatch(work)) {
        FTBL.SetScenarioCharacterFlag40(work->entry->enemy->name, 0);
        FTBL.SetScenarioCharacterTransparency(work->entry->enemy->name, 1.0f);
        FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
        DartsInitializeGame(work);
        FTBL.SetPlayerGameFlags(4, 0);
        FTBL.HLib_SetCurrentTaskTo(DartsEnemyShot);
    } else {
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetSample(0, 0, 0);
        SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.0f, 0.0f, 0.6f, 0, 0);
        DartsCamera(work, 7200, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
        DartsLoadAudioBank(work, 1);
        FTBL.HLib_SetCurrentTaskTo(DartsMain);
    }
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(DART_PLAYER));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(DART_PLAYER), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(DART_PLAYER), 1, 1, 0);
    FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(DART_PLAYER), 0, 21, &dartsHandRotation);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(DART_PLAYER), 0, dartsHandPoseCommand, 0);
}

void DartsInitializeGame(DartsEventWork *work)
{
    int i;
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(DART_PLAYER), 0);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(work->entry->enemy->name), 0);
    FTBL.EV_EnsureCharacterDraw(DartsSelectBoard(work, 0));
    FTBL.EV_EnableCharacterDisplay(DartsSelectBoard(work, 0));
    for (i = 0; i <= 4; i++) {
        DartsHideBoardArrow(DartsSelectBoard(work, DART_PLAYER), i, 1);
        FTBL.EV_EnsureCharacterDraw(GetDartsArrowName(0, i));
        FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(0, i));
        FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(0, i));
        FTBL.AT_SetLocalBaseChara(GetDartsArrowName(0, i), DartsSelectBoard(work, DART_PLAYER));
        DartsHideBoardArrow(DartsSelectBoard(work, work->entry->enemy->name), i, 1);
        FTBL.EV_EnsureCharacterDraw(GetDartsArrowName(1, i));
        FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(1, i));
        FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(1, i));
        FTBL.AT_SetLocalBaseChara(GetDartsArrowName(1, i), DartsSelectBoard(work, work->entry->enemy->name));
    }
    DartsResetScoring(work);
    DartsResetRound(work);
    DartsStartChildTask(work, 1);
    DartsStartChildTask(work, 0);
    DartsStartChildTask(work, 2);
}

void DartsEnemyShot(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int index = DartsOpponentIndex(work->entry->enemy->name);
    work->actorControl[0] = 1;
    FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
    FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
    FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
    FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 0);
    DartsCamera(work, index + 7230, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
    DartsResponse(work, 27);
    SetDartsPosition(DartsSelectBoard(work, 0), work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0, 0);
    SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.8f, 0.0f, 4.7f, 1820, 0);
    EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
    FTBL.HLib_SetCurrentTaskTo(DartsEnemyPlaying);
    DartsEnemyPlaying(work);
}

void DartsRyoShot(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    work->actorControl[0] = 0;
    FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
    FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
    DartsAttachTurnArrows(work);
    FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(0, work->state));
    FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
    FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 0);
    DartsCamera(work, 7225, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
    DartsResponse(work, 28);
    SetDartsPosition(DartsSelectBoard(work, 0), work->entry->enemy->name, -1.0f, 0.0f, 3.5f, 0xa000, 0);
    SetDartsPosition(DartsSelectBoard(work, 0), work->entry->enemy->name, -1.0f, 0.0f, 3.5f, 0xa000, 60);
    SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.0f, 0.0f, 4.0f, 0, 0);
    EVT_AT_SetMot(DART_PLAYER, 6371, -1.0f, 1.0f, 0x80000000, 0);
    EVT_AT_SetMot(work->entry->enemy->name, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
    FTBL.HLib_SetCurrentTaskTo(DartsPlaying);
    FTBL.HLib_SetupTaskDelay(120);
}

void DartsAttachReadyArrows(void)
{
    Vector3i rotation = {0, 0, 0};
    Vector3i attachRotation = {0, 2001, 15469};
    Vector3 position = {0.0882f, -0.0112f, 0.0f};
    int i;
    for (i = 0; i <= 4; i++) {
        FTBL.EV_EnsureCharacterDraw(GetDartsArrowName(0, i));
        FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(0, i));
        FTBL.scenSetCharaPosRotById(GetDartsArrowName(0, i), &position, &rotation);
        FTBL.AttachScenarioCharacter(GetDartsArrowName(0, i), DART_PLAYER, 21, &dartsReadyArrowOffsets[i], &attachRotation);
    }
}

void DartsMain(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    void *voice;
    if (FTBL.GetPlayerStat(2) < 5) {
        voice = DartsResponse(work, 2);
        FTBL.HLib_SetCurrentTaskTo(DartsMainOk);
        FTBL.HLib_SetupTaskDelay(30);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
    } else
        FTBL.HLib_SetCurrentTaskTo(DartsAskPlay);
}

void DartsMainOk(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    if (work->requestIcon == 36) {
        DartsStartChildTask(work, 1);
        DartsStartChildTask(work, 0);
        FTBL.HLib_SetCurrentTaskTo(DartsInsertCoin);
    } else
        FTBL.HLib_SetCurrentTaskTo(DartsExit);
}

void DartsAskPlay(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    void *voice;
    switch (work->nextState) {
    case 0:
        ++work->nextState;
        DartsQueueCameraWait(work, 7207, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0));
        DartsResponse(work, 0);
        FTBL.HLib_SetupTaskDelay(135);
        break;
    case 1:
        ++work->nextState;
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(DART_PLAYER), 5, 0);
        EVT_AT_KsIcon_Enqueue(0xffff, 36, 154, 0xffff, 1800, 154, 2, 0);
        break;
    case 2:
        if (!FTBL.GetShouldRenderIcons(2)) {
            work->nextState = 0;
            work->requestIcon = FTBL.shenGetCtrlSpeakIconFlag();
            if (work->requestIcon == 36) {
                voice = DartsResponse(work, 1);
                if (DartsSelectBoard(work, 0) != DARTS_COMPACT_BOARD)
                    FTBL.SetMGOTOSIDAMA(8, 1);
            } else {
                voice = DartsResponse(work, 3);
                DartsCamera(work, 7205, DART_PLAYER, DART_PLAYER, 0);
            }
            FTBL.HLib_SetCurrentTaskTo(DartsMainOk);
            FTBL.HLib_SetupTaskDelay(30);
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
        }
        break;
    }
}

void DartsInsertCoin(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    Vector3i rotation;
    Vector3 position, offset;
    int i;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        DartsResetScoring(work);
        for (i = 0; i <= 4; i++) {
            FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(0, i));
            FTBL.AT_SetLocalBaseChara(GetDartsArrowName(0, i), DartsSelectBoard(work, 0));
        }
        FTBL.SetPlayerGameFlags(4, 0);
        SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.0f, 0.0f, 0.6f, 0, 0);
        EVT_AT_SetMot(DART_PLAYER, 6369, -1.0f, 1.0f, 0x80000000, 0);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(DART_PLAYER), 1);
        DartsCamera(work, 7203, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
        DartsQueueCameraWait(work, 7204, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0));
        FTBL.AddThisToCashOrSegaCoins(-5, 2);
    case 1:
        switch ((int)FTBL.GetMotionFrame(FTBL.scenFindMotionwork(DART_PLAYER), 0)) {
        case 30:
            DartsPlaySample(1);
            FTBL.GetCharaPosition(DART_PLAYER, 14, &position, 0x40000000);
            FTBL.EV_AT_SetPos(DART_COIN, 0x38000000, &position);
            break;
        case 31:
            FTBL.EV_EnsureCharacterDraw(DART_COIN);
            FTBL.EV_EnableCharacterDisplay(DART_COIN);
            position.x = 0.124f;
            position.y = 0.012f;
            position.z = -0.085f;
            rotation.x = 0xfdde;
            rotation.y = 0xfb04;
            rotation.z = 0xb6c4;
            FTBL.AttachScenarioCharacter(DART_COIN, DART_PLAYER, 14, &position, &rotation);
            break;
        case 40:
            break;
        case 140:
            FTBL.EV_EnsureCharacterDraw(DartsSelectBoard(work, 0));
            FTBL.EV_EnableCharacterDisplay(DartsSelectBoard(work, 0));
            DartsPlaySample(0);
            FTBL.ResetScenarioCharacterAttachment(DART_COIN);
            FTBL.EV_DisableCharacterDisplay(DART_COIN);
            for (i = 0; i <= 4; i++)
                DartsHideBoardArrow(DartsSelectBoard(work, 0), i, 1);
            break;
        case 169:
            ++work->gameState;
            position.x = -0.045f;
            position.y = 0.0f;
            position.z = 0.6f;
            QueueDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, &position, 0, 0);
            DartsCamera(work, 7212, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
            EVT_AT_SetMot(DART_PLAYER, 6372, -1.0f, 1.0f, 0x80000000, 0);
            DartsPlaySample(2);
            break;
        }
        offset.x = offset.y = offset.z = 1.0f;
        FTBL.SetCharaTargetTransformOffset(DART_PLAYER, &offset, 1);
        break;
    case 2:
        if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(DART_PLAYER), 65, 1, 0)) {
            ++work->gameState;
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(DART_PLAYER), 0);
            break;
        }
        switch ((int)FTBL.GetMotionFrame(FTBL.scenFindMotionwork(DART_PLAYER), 0)) {
        case 55:
            DartsPlaySample(3);
            FTBL.GetCharaPosition(DART_PLAYER, 21, &position, 0x40000000);
            for (i = 0; i <= 4; i++)
                FTBL.EV_AT_SetPos(GetDartsArrowName(0, i), 0x38000000, &position);
            break;
        case 56:
            DartsAttachReadyArrows();
            break;
        case 125:
            DartsCamera(work, 7208, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
            break;
        case 150:
            break;
        case 180:
            FTBL.GetCharaPosition(DART_PLAYER, -1, &position, 0);
            FTBL.EV_AT_SetPos(GetDartsArrowName(0, 0), 0x38000000, &position);
            break;
        case 181:
            position.x = 0.0882f;
            position.y = -0.0112f;
            position.z = 0.0f;
            rotation.x = rotation.y = rotation.z = 0;
            FTBL.scenSetCharaPosRotById(GetDartsArrowName(0, 0), &position, &rotation);
            position.x = 0.137f;
            position.y = 0.028f;
            position.z = -0.068f;
            rotation.x = rotation.y = 0;
            rotation.z = 15834;
            FTBL.AttachScenarioCharacter(GetDartsArrowName(0, 0), DART_PLAYER, 14, &position, &rotation);
            break;
        }
        offset.x = offset.y = 0.0f;
        offset.z = 0.5f;
        FTBL.SetCharaTargetTransformOffset(DART_PLAYER, &offset, 1);
        break;
    case 3:
        work->gameState = 0;
        DartsResetRound(work);
        DartsStartChildTask(work, 2);
        FTBL.HLib_SetCurrentTaskTo(DartsPlaying);
        break;
    }
}

void DartsPlaying(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    Vector3 position;
    void *voice;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        if (work->state == 1) {
            voice = DartsResponse(work, 4);
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
            else
                FTBL.HLib_SetupTaskDelay(30);
        }
        break;
    case 1:
        ++work->gameState;
        if (work->standardBoard) {
            work->wobblePhaseX = (int)(FTBL.GetRandomNumberInRange(360.0f) * 8192.0f / 45.0f) & 0xffff;
            work->wobblePhaseY = (int)(FTBL.GetRandomNumberInRange(360.0f) * 8192.0f / 45.0f) & 0xffff;
        } else
            work->wobblePhaseY = 0;
        DartsAttachTurnArrows(work);
        break;
    case 2:
        work->gameState = 0;
        DartsShowTurnActor(work);
        FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(0, work->state));
        if (work->standardBoard) {
            SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, -0.015f, -0.002f, 1.8f, 0, 0);
            EVT_AT_SetMot(DART_PLAYER, 6371, 27.0f, 0.0f, 0x80000000, 0);
            FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 1);
            FTBL.SetScenarioCharacterTransparency(DART_PLAYER, 0.5f);
            FTBL.ResetScenarioCharacterAttachment(GetDartsArrowName(0, work->state));
            EVT_AT_SetPos(GetDartsArrowName(0, work->state), DartsSelectBoard(work, 0), -0.0903f, 1.6251f, 1.3136f, 0);
            EVT_AT_SetAng(GetDartsArrowName(0, work->state), DartsSelectBoard(work, 0), 58.2715f, 279.1571f, 119.5203f, 0);
            FTBL.AT_SetLocalBaseChara(DART_PLAYER, GetDartsArrowName(0, work->state));
            FTBL.GetCharaPosition(DART_PLAYER, -1, &work->dartPosition, 0);
            FTBL.GetScenarioCharacterEulerAngles(DART_PLAYER, (Vector3i *)&work->ryoRotation, 0);
            FTBL.AT_SetLocalBaseChara(DART_PLAYER, -1);
            FTBL.GetCharaPosition(GetDartsArrowName(0, work->state), -1, &position, 0x40000000);
            FTBL.EV_AT_SetPos(DART_COIN, 0x78000000, &position);
        } else {
            SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.0100000007f, -0.002f, 1.8f, 0, 0);
            EVT_AT_SetMot(DART_PLAYER, 6371, 44.0f, 0.0f, 0x80000000, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(DART_PLAYER), 1.0f, 0);
        }
        if (!DartsIsPlayerTurn(work))
            DartsComputeAiTarget(work);
        work->banks[0].ticker = 100000;
        work->banks[0].score[0] = 0;
        work->scoreUiFlags = 1;
        work->actorControl[1] = 5;
        DartsCamera(work, 7201, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0);
        FTBL.HLib_SetCurrentTaskTo(DartsPlayingLoop);
        DartsPlayingLoop(work);
        break;
    }
}

void DartsPlayingLoop(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int thrown;
    switch (work->gameState) {
    case 0:
        thrown = 0;
        if (DartsIsPlayerTurn(work)) {
            FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'B', 'A'));
            if (work->actorControl[1])
                work->actorControl[1]--;
            else if (FTBL.GetCurrentControlInput(1) == 1024)
                thrown = 1;
        } else if (work->field08) {
            work->field08 = 0;
            thrown = 1;
        } else
            work->field08 = DartsAiTriggerReady(work);
        if (thrown) {
            work->gameState = 0;
            if (work->standardBoard)
                DartsProjectImpact(work, &work->impactPosition);
            else
                DartsComputeCompactImpact(work, &work->impactPosition);
            DartsAttachTurnArrows(work);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(DART_PLAYER), 6370, 0);
            FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(DART_PLAYER), 70.0f, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(DART_PLAYER), 1.0f, 0);
            FTBL.HLib_SetCurrentTaskTo(DartsProjectileStart);
            break;
        }
    case 1:
        work->gameState = 0;
        if (work->standardBoard) {
            DartsWobbleVertical(work);
            DartsWobbleHorizontal(work);
            DartsRotateAimingArrow(work);
            QueueDartsPosition(GetDartsArrowName(0, work->state), DART_PLAYER, &work->dartPosition, (Vector3i *)&work->ryoRotation, 0);
        } else
            DartsCompactWobble(work);
        break;
    }
}

void DartsComputeAiTarget(DartsEventWork *work)
{
    int index = (int)(FTBL.GetRandomNumberInRange(1.0f) * 5.0f) % 5;
    int i = 0;
    while (i < 5 && work->usedBands[index]) {
        int next = index + 1;
        index = next % 5;
        ++i;
    }
    work->usedBands[index] = 1;
    work->targetBand = dartsAiBands[index];
}

void DartsEnemyPlaying(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int index = DartsOpponentIndex(work->entry->enemy->name);
    int arc, score;
    float duration;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        work->banks[0].ticker = 100000;
        work->scoreUiFlags = 1;
        FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
        FTBL.EV_SetCharacterAlphaEnabled(DART_PLAYER, 0);
        FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
        FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
        SetDartsPosition(DartsSelectBoard(work, 0), work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0, 0);
        SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0.8f, 0.0f, 4.7f, 1820, 0);
        EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
        FTBL.EV_EnableCharacterDisplay(GetDartsArrowName(1, work->state));
        FTBL.EV_EnsureCharacterDraw(GetDartsArrowName(1, work->state));
        if (DartsOpponentAttachBone(work->entry->enemy->name, work->difficulty) == 21)
            FTBL.AttachScenarioCharacter(GetDartsArrowName(1, work->state), work->entry->enemy->name,
                DartsOpponentAttachBone(work->entry->enemy->name, work->difficulty), &dartsJonGameArrowOffset, &dartsGameArrowRotation);
        else
            FTBL.AttachScenarioCharacter(GetDartsArrowName(1, work->state), work->entry->enemy->name,
                DartsOpponentAttachBone(work->entry->enemy->name, work->difficulty), &dartsGameArrowOffsets[index], &dartsGameArrowRotation);
        if (!work->actorControl[0])
            EVT_AT_SetCam(index + 7230, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), 0, 0, 1);
        DartsComputeAiTarget(work);
        work->banks[0].score[0] = DartsComputeAiImpactPoint(work);
        FTBL.HLib_SetupTaskDelay(DartsBeginOpponentThrow(work->entry->enemy->name, 1, work->difficulty));
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->entry->enemy->name), 71, 0, 0);
        break;
    case 1:
        work->gameState = 0;
        FTBL.ResetScenarioCharacterAttachment(GetDartsArrowName(1, work->state));
        if (work->entry->enemy->name == DART_FCJ) {
            arc = 1;
            duration = 26.0f;
        } else {
            arc = 0;
            duration = 13.0f;
        }
        DartsQueueProjectile(DartsSelectBoard(work, 0), GetDartsArrowName(1, work->state), 0, &work->impactPosition, duration, arc);
        DartsCamera(work, 7202, DART_COIN, DART_COIN, 0);
        DartsCamera(work, 7206, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0), (int)duration + 15);
        FTBL.HLib_SetCurrentTaskTo(DartsGameMain);
        FTBL.HLib_SetupTaskDelay((int)duration);
        break;
    }
}

int DartsComputeAiImpactPoint(DartsEventWork *work)
{
    Vector3 position, scaled;
    union { int value; volatile int observed; } accepted;
    float outer, inner, distance, radius;
    int remaining, iterations;
    int result;
    result = 0;
    accepted.value = result;
    outer = work->entry->aiParameters[0] * work->targetBand;
    remaining = 1;
    if (outer < 10.0f && !(work->entry->aiParameters[0] < 10.0f))
        outer = 10.0f;
    else if (outer < 10.0f)
        outer = work->entry->aiParameters[0];
    inner = work->entry->aiParameters[1] * work->targetBand;
    if (inner < 10.0f && work->entry->aiParameters[1] > 10.0f)
        inner = 10.0f;
    else if (outer < 10.0f)
        outer = work->entry->aiParameters[0];
    work->wobblePhaseX = (int)(FTBL.GetRandomNumberInRange(360.0f) * 8192.0f / 45.0f) & 0xffff;
    work->wobblePhaseY = (int)(FTBL.GetRandomNumberInRange(360.0f) * 8192.0f / 45.0f) & 0xffff;
    distance = 0.0f;
    iterations = 0;
    radius = 0.171f;
    while (remaining) {
        iterations++;
        position.y = DartsVerticalWobble(work->wobblePhaseX) * radius;
        work->wobblePhaseX += 18;
        position.x = radius * FTBL.CalculateFixedSine(work->wobblePhaseY);
        work->wobblePhaseY += 218;
        scaled.x = position.x;
        scaled.y = position.y * 1.0072135925292969f;
        distance = scaled.x * scaled.x + scaled.y * scaled.y;
        DART_FSQRT(distance);
        result = accepted.observed;
        if (result)
            remaining--;
        else {
            accepted.value = radius * outer / 100.0f > distance;
            if (accepted.value)
                accepted.value = distance > inner * radius / 100.0f;
        }
    }
    FTBL.DebugLogToFileRemoved("area = %f, len = %f, x = %f, y = %f\n", outer, distance, position.x, position.y);
    work->impactPosition.x = position.x;
    work->impactPosition.y = position.y + 1.4174f;
    work->impactPosition.z = -0.068200007f;
    if (iterations > 600)
        iterations = 600;
    result = iterations;
    return result;
}

void DartsProjectileStart(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    if (!(FTBL.GetMotionFrame(FTBL.scenFindMotionwork(DART_PLAYER), 0) < 26.0f)) {
        FTBL.ResetScenarioCharacterAttachment(GetDartsArrowName(0, work->state));
        DartsQueueProjectile(DartsSelectBoard(work, 0), GetDartsArrowName(0, work->state), 0, &work->impactPosition, 13.0f, 0);
        DartsCamera(work, 7202, DART_COIN, DART_COIN, 0);
        DartsQueueCameraWait(work, 7206, DartsSelectBoard(work, 0), DartsSelectBoard(work, 0));
        FTBL.HLib_SetCurrentTaskTo(DartsGameMain);
        FTBL.HLib_SetupTaskDelay(13);
    }
}

void DartsGameMain(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        work->banks[0].score[1] = DartsScoreImpact(work);
        if (work->banks[0].score[1] == 500000)
            work->banks[0].bullseyes++;
        work->scoreUiFlags = 0;
        break;
    case 1:
        work->gameState = 0;
        DartsHideBoardArrow(DartsSelectBoard(work, 0), 5 - work->state, 0);
        if (DartsIsOpponentMatch(work)) {
            if (!work->scoreStep)
                work->scoreStep = 1;
            else {
                work->state--;
                work->scoreStep = 0;
            }
        } else
            work->state--;
        if (work->state > 0)
            EVT_AT_SetMot(DART_PLAYER, 6371, 0.0f, 0.0f, 0x80000000, 0);
        else if (work->state <= 0)
            EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
        FTBL.HLib_SetCurrentTaskTo(DartsShotEnd);
        FTBL.HLib_SetupTaskDelay(15);
        break;
    }
}

void DartsShotEnd(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    void *voice;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        FTBL.CheckHelpIcon(1, 1, 2);
        FTBL.SetLoadingBusy();
        if (work->standardBoard)
            work->banks[0].score[2] = work->banks[0].score[1] / 10000;
        else
            work->banks[0].score[2] = work->banks[0].score[1] >> 1;
        break;
    case 1:
        ++work->gameState;
        if (!work->banks[0].score[2])
            voice = DartsResponse(work, 5);
        else if (work->banks[0].score[2] <= 10)
            voice = DartsResponse(work, 6);
        else if (work->banks[0].score[2] <= 15)
            voice = DartsResponse(work, 7);
        else if (work->banks[0].score[2] <= 20)
            voice = DartsResponse(work, 8);
        else if (work->banks[0].score[2] <= 30)
            voice = DartsResponse(work, 9);
        else if (work->banks[0].score[2] <= 45)
            voice = DartsResponse(work, 10);
        else if (work->banks[0].score[2] == 50)
            voice = DartsResponse(work, 11);
        else
            voice = DartsResponse(work, 12);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
        else
            FTBL.HLib_SetupTaskDelay(30);
        break;
    case 2:
        ++work->gameState;
        work->banks[0].score[2] = work->banks[0].score[1];
        FTBL.HLib_SetupTaskDelay(24);
        break;
    case 3:
        ++work->gameState;
        DartsBeginScoreTransfer(work, &work->banks[0].score[2]);
        break;
    case 4:
        ++work->gameState;
        FTBL.HLib_SetupTaskDelay(36);
        break;
    case 5:
        ++work->gameState;
        if (work->banks[0].ticker > 0 && work->standardBoard) {
            work->scoreAnimationActive = 1;
            work->field0e |= 0x60;
            DartsBeginScoreTransfer(work, &work->banks[0].ticker);
            break;
        }
    case 6:
        ++work->gameState;
        if (work->scoreAnimationActive) {
            work->scoreAnimationActive = 0;
            FTBL.HLib_SetupTaskDelay(30);
            work->field0e &= 0x9f;
            break;
        }
    case 7:
        ++work->gameState;
        if (!DartsIsPlayerTurn(work) && DartsIsOpponentMatch(work))
            EVT_AT_SetMot(work->entry->enemy->name, 0xf002, 0.0f, 0.0f, 0x80000000, 0);
    case 8:
        ++work->gameState;
        FTBL.CheckHelpIcon(1, 0, 1);
        FTBL.FinishLoading();
        if (work->standardBoard && (!work->state || (work->state == 1 && work->scoreStep && DartsIsOpponentMatch(work)))) {
            if (++work->banks[0].throws <= 2) {
                if (work->banks[0].score[3] / 10000 >= work->banks[0].throws * 120) {
                    DartsNextGame(work);
                    FTBL.HLib_SetCurrentTaskTo(DartsNextGame);
                } else if (DartsIsOpponentMatch(work)) {
                    voice = DartsResponse(work, 29);
                    if (voice)
                        DartsQueueVoiceWait(work->task, voice);
                    else
                        FTBL.HLib_SetupTaskDelay(30);
                }
                break;
            } else if (DartsIsOpponentMatch(work)) {
                voice = DartsResponse(work, 15);
                if (voice)
                    DartsQueueVoiceWait(work->task, voice);
                else
                    FTBL.HLib_SetupTaskDelay(30);
                break;
            }
        }
    case 9:
        work->gameState = 0;
        work->pendingReset = 0;
        if (!work->state || (!work->playerWon && work->state == 1 && work->scoreStep && DartsIsOpponentMatch(work)))
            FTBL.HLib_SetCurrentTaskTo(DartsShotEndBlink);
        else {
            if (DartsIsOpponentMatch(work))
                DartsSwitchTurn(work);
            if (work->state == 5 && work->actorControl[0])
                FTBL.HLib_SetCurrentTaskTo(DartsRyoShot);
            else if (DartsIsPlayerTurn(work))
                FTBL.HLib_SetCurrentTaskTo(DartsPlaying);
            else
                FTBL.HLib_SetCurrentTaskTo(DartsEnemyPlaying);
        }
        break;
    }
}

void DartsShotEndBlink(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    if (work->pendingReset < 30) {
        work->field0e = 15;
        if (work->pendingReset % 4 < 2)
            work->field0e &= 254;
        work->pendingReset++;
    } else
        FTBL.HLib_SetCurrentTaskTo(DartsShotEndRecord);
}

void DartsShotEndRecord(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int record;
    work->field0e = 15;
    record = DartsRecordBest(work);
    if (record >= 0) {
        if (!DartsIsOpponentMatch(work) && record) {
            DartsPlaySample(9);
            DartsResponse(work, 13);
            FTBL.HLib_SetCurrentTaskTo(DartsPlayEnd);
            FTBL.HLib_SetupTaskDelay(105);
        } else {
            FTBL.HLib_SetCurrentTaskTo(DartsPlayEnd);
            FTBL.HLib_SetupTaskDelay(15);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(DartsPlayEnd);
}

int DartsRecordBest(DartsEventWork *work)
{
    int result = -1;
    work->banks[0].previousScore = work->banks[0].score[3];
    if (work->banks[0].bestScore < work->banks[0].score[3]) {
        work->banks[0].bestScore = work->banks[0].score[3];
        result = 0;
        if (work->standardBoard)
            result = work->banks[0].bestScore > 777777;
        else if (work->banks[0].bestScore > 50)
            result = 1;
    }
    return result;
}

void DartsNextGame(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    Vector3 step;
    int round = work->banks[0].throws + 1;
    int i, j;
    switch (work->nextState) {
    case 0:
        ++work->nextState;
        work->field0e = 0;
        work->impactPosition.x = -0.12760001f;
        work->impactPosition.y = 2.155f;
        work->impactPosition.z = -0.04f;
        work->nextGameText[0] = 0;
        if (round == 2) {
            work->nextGameText[1] = 1;
            work->nextGameText[2] = round;
        } else {
            work->nextGameText[1] = 3;
            work->nextGameText[2] = 2;
        }
        work->nextGameText[3] = 0;
        work->nextGameText[4] = 4;
        work->nextGameText[5] = 5;
        work->nextGameText[6] = 6;
        work->nextGameText[7] = 7;
        DartsPlaySample(10);
        if (round == 2)
            DartsResponse(work, 14);
        else if (round == 3)
            DartsResponse(work, 16);
        work->scoreTickInterval = 0;
    case 1:
        if (work->scoreTickInterval <= 219) {
            step.x = -0.05f;
            step.y = 0.0f;
            step.z = 0.0f;
            FTBL.scenCopyCharaToGame(DartsSelectBoard(work, 0));
            FTBL.SetPosRotForGame(DartsSelectBoard(work, 0), 256, &step, 9, 0);
            FTBL.UpdateGameNodeFlags(DartsSelectBoard(work, 0), round, &work->impactPosition, 1.0f, 0);
            for (j = 0; j <= 7; j++)
                if (work->scoreTickInterval >= j * 7)
                    DartsNextGameGlyph(work->nextGameText[j], j, DartsSelectBoard(work, 0));
            DartsShowBankScore(work, 2);
            if (work->scoreTickInterval % 4 < 2)
                DartsShowBankScore(work, 3);
            work->scoreTickInterval++;
        } else {
            i = 0;
            work->field0e = 15;
            if (DartsIsOpponentMatch(work)) {
                if (work->scoreStep)
                    work->playerWon = 1;
                else
                    i = 1;
            } else {
                work->playerWon = 1;
                i = 1;
            }
            if (i) {
                if (work->playerWon)
                    work->state = 5;
                work->playerWon = 0;
                for (i = 0; i <= 4; i++) {
                    FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(0, i));
                    work->usedBands[i] = 0;
                    DartsHideBoardArrow(DartsSelectBoard(work, DART_PLAYER), i, 1);
                    if (DartsIsOpponentMatch(work)) {
                        FTBL.EV_DisableCharacterDisplay(GetDartsArrowName(1, i));
                        DartsHideBoardArrow(DartsSelectBoard(work, work->entry->enemy->name), i, 1);
                    }
                }
            }
            work->nextState = 0;
            FTBL.HLib_SetCurrentTaskTo(DartsShotEnd);
        }
        break;
    }
}

void DartsNextGameGlyph(char code, int index, shen_name board)
{
    union {
        Vector3 value;
        float coordinates[3];
    } point;
    float *coordinate;
    int node;
    switch (code) {
    case 5:
        node = 292;
        break;
    case 2:
        node = 295;
        break;
    case 7:
        node = 296;
        break;
    case 4:
        node = 298;
        break;
    case 6:
        node = 330;
        break;
    case 1:
        node = 331;
        break;
    case 3:
        node = 335;
        break;
    default:
        node = 0;
        break;
    }
    if (index <= 3) {
        point.value.x = (index - 3) * 0.05f + 0.0224f;
        coordinate = point.coordinates + 1;
        *coordinate = 2.155f;
    } else {
        point.value.x = (index - 7) * 0.05f + 0.0224f;
        coordinate = point.coordinates + 1;
        *coordinate = 2.005f;
    }
    *++coordinate = -0.04f;
    if (node)
        FTBL.scenGetCharaGameNode(board, node, &point.value, 1.0f, 0);
}

void DartsPlayEnd(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int i;
    if (work->state) {
        DartsSwitchTurn(work);
        if (DartsIsPlayerTurn(work))
            FTBL.HLib_SetCurrentTaskTo(DartsPlaying);
        else
            FTBL.HLib_SetCurrentTaskTo(DartsEnemyPlaying);
    } else {
        FTBL.SetScenarioCharacterTransparency(DART_PLAYER, 1.0f);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(DART_PLAYER), 1);
        EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
        SetDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, -0.015f, -0.002f, 1.8f, 0, 0);
        for (i = 0; i <= 3; i++)
            if (work->childTasks[i]) {
                FTBL.HLib_DestroyTaskInternal(work->childTasks[i]);
                work->childTasks[i] = 0;
            }
        work->previousTime.minute += 15;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&work->previousTime);
        DartsSaveScores(work);
        if (DartsIsOpponentMatch(work)) {
            FTBL.SetScenarioCharacterTransparency(work->entry->enemy->name, 1.0f);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(work->entry->enemy->name), 1);
            EVT_AT_SetMot(work->entry->enemy->name, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
            FTBL.HLib_AssignCallbackForTask(work->task, DartsExitBattle);
        } else
            FTBL.HLib_AssignCallbackForTask(work->task, DartsContinuePrompt);
        FTBL.EV_DisableCharacterDisplay(DartsSelectBoard(work, 0));
    }
}

void DartsExitBattle(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    float x, z, first;
    int motion, index, result, delay, frames;
    FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
    FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
    index = DartsOpponentIndex(work->entry->enemy->name);
    result = DartsBattleResult(work);
    if (FTBL.EV_GetShenmueFlag(0x3e0) && g_currentMap == DART_AKS1) {
        if (result == 2)
            FTBL.EV_stepSetFlags(0x3e0, 0);
        else
            FTBL.EV_stepSetFlags(0x3e0, 1);
        FTBL.HLib_SetCurrentTaskTo(DartsExit);
        return;
    }
    switch (result) {
    case 2:
        motion = dartsOpponents[index].winMotion;
        delay = dartsOpponents[index].winTotal;
        first = dartsOpponents[index].winFrame;
        x = dartsOpponents[index].winX;
        z = dartsOpponents[index].winZ;
        delay = delay - first < 200.0f ? 0 : delay - 200 - first;
        EVT_AT_SetMot(DART_PLAYER, 6851, -1.0f, 0.2f, 0x40000000, 0);
        EVT_AT_SetMot(DART_PLAYER, 6851, 58.0f, 1.0f, 0x40000000, delay);
        DartsResponse(work, 21);
        EVT_AT_StartMoneyCount(work->entry->bet, 330);
        if (work->entry->enemy->name == DART_JON)
            work->difficulty++;
        DartsCamera(work, 7222, DART_DUMMY1, DART_DUMMY1, 1);
        DartsCamera(work, index + 7523, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, delay);
        EVT_AT_CharaDisp(DART_PLAYER, 0, 1);
        EVT_AT_CharaDisp(DART_PLAYER, delay, 0);
        frames = EVT_AT_SetMot(work->entry->enemy->name, motion, first, 1.0f, 0x40000000, 0);
        break;
    case 1:
        motion = dartsOpponents[index].drawMotion;
        delay = 160;
        first = 0.0f;
        x = dartsOpponents[5].loseX;
        z = dartsOpponents[5].loseZ;
        DartsResponse(work, 22);
        DartsCamera(work, 7228, DART_DUMMY1, DART_DUMMY1, 1);
        EVT_AT_CharaDisp(DART_PLAYER, 0, 1);
        EVT_AT_SetMot(work->entry->enemy->name, motion, first, 1.0f, 0x40000000, 0);
        frames = delay;
        break;
    default:
        if (work->entry->enemy->name == DART_JON) {
            if (DartsIsHardJon(work->entry->enemy->name, work->difficulty)) {
                motion = dartsOpponents[index].motions[13];
                delay = dartsOpponents[index].motions[15];
                first = dartsOpponents[index].loseFrame[1];
            } else {
                motion = dartsOpponents[index].motions[12];
                delay = dartsOpponents[index].motions[14];
                first = dartsOpponents[index].loseFrame[0];
            }
        } else if (FTBL.shenGetRandomInt32(2)) {
            motion = dartsOpponents[index].motions[13];
            delay = dartsOpponents[index].motions[15];
            first = dartsOpponents[index].loseFrame[1];
        } else {
            motion = dartsOpponents[index].motions[12];
            delay = dartsOpponents[index].motions[14];
            first = dartsOpponents[index].loseFrame[0];
        }
        x = dartsOpponents[index].loseX;
        z = dartsOpponents[index].loseZ;
        delay = delay - first < 150.0f ? 0 : delay - 150 - first;
        EVT_AT_SetMot(DART_PLAYER, 6343, -1.0f, 0.2f, 0x40000000, 0);
        EVT_AT_SetMot(DART_PLAYER, 6343, -1.0f, 1.0f, 0x40000000, delay);
        DartsResponse(work, 23);
        EVT_AT_StartMoneyCount(-work->entry->bet, 330);
        DartsCamera(work, 7229, DART_DUMMY1, DART_DUMMY1, 1);
        DartsCamera(work, index + 7523, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, delay);
        EVT_AT_CharaDisp(DART_PLAYER, 0, 1);
        EVT_AT_CharaDisp(DART_PLAYER, delay, 0);
        frames = EVT_AT_SetMot(work->entry->enemy->name, motion, first, 1.0f, 0x40000000, 0);
        break;
    }
    if (frames < delay)
        frames = delay;
    EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[7], -1.0f, 1.0f, 0x80000000, frames);
    EVT_AT_CharaDisp(DART_PLAYER, frames, 0);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, DART_PLAYER, x, 0.0f, z, 0x4000, 0);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0xeaaa, 0);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, DART_PLAYER, 1.2f, 0.0f, 4.2f, 0x4000, frames);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0xeaaa, frames);
    DartsCamera(work, index + 7540, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, frames);
    FTBL.HLib_SetCurrentTaskTo(DartsContinuePrompt);
    FTBL.HLib_SetupTaskDelay(frames);
}

void DartsSaveScores(DartsEventWork *work)
{
    if (work->standardBoard) {
        if (work->scoreStep) {
            DartsSwitchTurn(work);
            DartsRecordBest(work);
        }
        DartsStoreTurnBank(work);
        FTBL.SetSavedMinigameScore(2, work->banks[1].bestScore);
        FTBL.SetSavedMinigameScore(3, work->banks[1].previousScore);
        if (DartsIsOpponentMatch(work))
            RestoreOpponentDartsScores(work->entry->enemy->name, 1, &work->banks[2].bestScore, &work->banks[2].previousScore);
    } else {
        DartsStoreTurnBank(work);
        FTBL.SetSavedMinigameScore(12, work->banks[0].bestScore);
        FTBL.SetSavedMinigameScore(13, work->banks[0].previousScore);
    }
}

void DartsContinuePrompt(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    void *voice = 0;
    int reward = 0;
    int delay;
    if (!work->entry->opponentMatch && work->banks[1].score[3] > 2499999)
        reward = !FTBL.EV_GetPlayerFlag(11, 1014);
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        if (DartsIsOpponentMatch(work)) {
            switch (DartsBattleResult(work)) {
            case 2:
                voice = DartsResponse(work, 24);
                break;
            case 1:
                voice = DartsResponse(work, 25);
                break;
            case 0:
                voice = DartsResponse(work, 26);
                break;
            }
            FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
        } else {
            DartsCamera(work, 7205, DART_PLAYER, DART_PLAYER, 0);
            voice = DartsResponse(work, 17);
        }
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
        else
            FTBL.HLib_SetupTaskDelay(200);
        break;
    case 1:
        if (DartsIsOpponentMatch(work)) {
            delay = DartsCheckBet(work);
            if (!delay) {
                ++work->gameState;
                EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
                EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
                if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
                    if (work->difficulty) {
                        work->gameState = 0;
                        FTBL.HLib_SetCurrentTaskTo(DartsDoubleUp);
                    } else
                        EVT_AT_KsIcon_Enqueue(0xffff, 37, 154, 0xffff, 1800, 154, 2, 0);
                }
            } else if (delay > 0) {
                work->gameState = 0;
                if (DartsIsOpponentMatch(work)) {
                    EVT_AT_CharaDisp(work->entry->enemy->name, 0, 1);
                    EVT_AT_CharaDisp(work->entry->enemy->name, 80, 0);
                }
                FTBL.HLib_SetCurrentTaskTo(DartsExit);
                FTBL.HLib_SetupTaskDelay(delay);
            } else {
                if (DartsIsOpponentMatch(work)) {
                    EVT_AT_CharaDisp(work->entry->enemy->name, 0, 1);
                    EVT_AT_CharaDisp(work->entry->enemy->name, 80, 0);
                }
                FTBL.HLib_SetupTaskDelay(-delay);
            }
        } else if (FTBL.GetPlayerStat(2) < 5) {
            work->gameState = 0;
            voice = DartsResponse(work, 2);
            FTBL.HLib_SetCurrentTaskTo(DartsExit);
            FTBL.HLib_SetupTaskDelay(30);
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
        } else {
            ++work->gameState;
            if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0 && !reward)
                EVT_AT_KsIcon_Enqueue(0xffff, 37, 154, 0xffff, 1800, 154, 2, 0);
        }
        break;
    case 2:
        if (!FTBL.GetShouldRenderIcons(2)) {
            work->gameState = 0;
            if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0 && !reward && (work->entry->opponent == 1 || FTBL.shenGetCtrlSpeakIconFlag() == 37)) {
                if (DartsIsOpponentMatch(work)) {
                    DartsContinueOk(work);
                    break;
                }
                work->requestIcon = 36;
                voice = DartsResponse(work, 18);
                FTBL.HLib_SetCurrentTaskTo(DartsMainOk);
                FTBL.HLib_SetupTaskDelay(60);
            } else if (DartsIsOpponentMatch(work)) {
                delay = PlayDartsOpponentExit(work->entry->enemy->name);
                EVT_AT_SetCam(7317, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 80);
                FTBL.HLib_SetCurrentTaskTo(DartsExit);
                FTBL.HLib_SetupTaskDelay(delay);
            } else {
                voice = DartsResponse(work, 19);
                FTBL.HLib_SetCurrentTaskTo(DartsExit);
                FTBL.HLib_SetupTaskDelay(60);
            }
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
        }
        break;
    }
}

void DartsContinueOk(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int delay;
    DartsInitializeGame(work);
    delay = DartsAcceptBet(work);
    FTBL.HLib_SetCurrentTaskTo(DartsEnemyShot);
    FTBL.HLib_SetupTaskDelay(delay);
}

void DartsDoubleUp(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    void *voice;
    switch (work->gameState) {
    case 0:
        ++work->gameState;
        if (work->difficulty == 1)
            EVT_AT_KsIcon_Enqueue(212, 213, 214, 154, 1800, 154, 2, 0);
        else
            EVT_AT_KsIcon_Enqueue(215, 216, 217, 154, 1800, 154, 2, 0);
        break;
    case 1:
        if (!FTBL.GetShouldRenderIcons(2)) {
            work->gameState = 0;
            switch (FTBL.shenGetCtrlSpeakIconFlag()) {
            case 212:
                work->entry->bet = 10;
                break;
            case 213:
                work->entry->bet = 20;
                break;
            case 214:
                work->entry->bet = 30;
                break;
            case 215:
                work->entry->bet = 100;
                work->entry->aiParameters[0] = 5;
                work->entry->aiParameters[1] = 0;
                FTBL.EV_SetPlayerFlag(12, 502, 1);
                FTBL.EV_LoadAudio("01130000.sbt", 6);
                break;
            case 216:
                work->entry->bet = 200;
                work->entry->aiParameters[0] = 5;
                work->entry->aiParameters[1] = 0;
                FTBL.EV_SetPlayerFlag(12, 502, 1);
                FTBL.EV_LoadAudio("01130000.sbt", 6);
                break;
            case 217:
                work->entry->bet = 300;
                work->entry->aiParameters[0] = 5;
                work->entry->aiParameters[1] = 0;
                FTBL.EV_SetPlayerFlag(12, 502, 1);
                FTBL.EV_LoadAudio("01130000.sbt", 6);
                break;
            default:
                work->entry->bet = 0;
                DartsResponse(work, 20);
                break;
            }
            if (work->entry->bet) {
                if (FTBL.GetPlayerStat(2) < work->entry->bet) {
                    voice = DartsResponse(work, 2);
                    if (voice)
                        DartsQueueVoiceWait(work->task, voice);
                    else
                        FTBL.HLib_SetupTaskDelay(30);
                } else
                    FTBL.HLib_SetCurrentTaskTo(DartsContinueOk);
            } else {
                FTBL.HLib_SetCurrentTaskTo(DartsExit);
                FTBL.HLib_SetupTaskDelay(120);
            }
        }
        break;
    }
}

void DartsBeginScoreTransfer(DartsEventWork *work, int *score)
{
    if (work->standardBoard)
        work->pendingScore = *score / 10000;
    else
        work->pendingScore = *score;
    if (!work->pendingScore) {
        work->scoreTarget = score;
        FTBL.HLib_SetCurrentTaskTo(DartsScoreAdd);
    } else {
        if (work->pendingScore < 20) {
            work->scoreTick[1] = 1;
            work->scoreTickInterval = 2;
        } else if (work->pendingScore < 40) {
            work->scoreTick[1] = 1;
            work->scoreTickInterval = 1;
        } else {
            work->scoreTick[1] = 2;
            work->scoreTickInterval = 1;
        }
        if (work->standardBoard) {
            work->scoreTick[1] *= 10000;
            work->pendingScore *= 10000;
        }
        work->scoreTick[0] = work->scoreTickInterval;
        DartsPlaySample(7);
        if (!DartsAdvanceScore(work, score)) {
            work->scoreTarget = score;
            FTBL.HLib_SetCurrentTaskTo(DartsScoreAdd);
            return;
        }
    }
    work->banks[0].score[3] += *score;
    *score = 0;
}

int DartsAdvanceScore(DartsEventWork *work, int *score)
{
    int done = 0;
    if (!work->pendingScore) {
        DartsPlaySample(8);
        done = 1;
    } else if (!work->scoreTick[0]) {
        if (work->pendingScore < work->scoreTick[1])
            work->scoreTick[1] = work->pendingScore;
        work->pendingScore -= work->scoreTick[1];
        work->banks[0].score[3] += work->scoreTick[1];
        *score -= work->scoreTick[1];
        work->scoreTick[0] = work->scoreTickInterval;
    } else if (work->scoreTick[0])
        work->scoreTick[0]--;
    return done;
}

void DartsScoreAdd(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    if (DartsAdvanceScore(work, work->scoreTarget)) {
        work->banks[0].score[3] += *work->scoreTarget;
        *work->scoreTarget = 0;
        FTBL.HLib_SetCurrentTaskTo(DartsShotEnd);
    }
}

void DartsProjectImpact(DartsEventWork *work, Vector3 *impact)
{
    shen_name coin = DART_COIN;
    Vector3 origin, current;
    float currentScreen[2], originScreen[2];
    shen_name arrow = FTBL.SCEN_FindChar_Wrapper(GetDartsArrowName(0, work->state));
    origin.x = -0.119f;
    origin.y = 1.5891f;
    origin.z = 1.3261f;
    FTBL.AT_SetLocalBaseChara(coin, -1);
    FTBL.GetCharaPosition(coin, -1, &current, 0);
    FTBL.Vector4_ProjectToScreen(&current, currentScreen);
    FTBL.AT_SetLocalBaseChara(coin, arrow);
    FTBL.EV_AT_SetPos(coin, 0x38000000, &origin);
    FTBL.AT_SetLocalBaseChara(coin, -1);
    FTBL.GetCharaPosition(coin, -1, &origin, 0);
    FTBL.Vector4_ProjectToScreen(&origin, originScreen);
    impact->x = (currentScreen[0] - originScreen[0]) * 0.166f / 85.5f;
    impact->y = (originScreen[1] - currentScreen[1]) * 0.166f / 85.5f + 1.4174f;
    impact->z = -0.068200007f;
    FTBL.EV_AT_SetPos(coin, 0x38000000, &current);
}

void DartsComputeCompactImpact(DartsEventWork *work, Vector3 *impact)
{
    float frame = FTBL.GetMotionFrame(FTBL.scenFindMotionwork(DART_PLAYER), 0);
    if (work->wobblePhaseY <= 22)
        impact->x = work->wobblePhaseY * 0.0090909088f;
    else if (work->wobblePhaseY <= 44) {
        float offset = work->wobblePhaseY - 22;
        float horizontal = 0.0090909088f;
        offset *= horizontal;
        horizontal = 0.2f - offset;
        impact->x = horizontal;
    } else if (work->wobblePhaseY <= 67)
        impact->x = (work->wobblePhaseY - 44) * -0.0086956527f;
    else
        impact->x = -0.2f + (work->wobblePhaseY - 67) * 0.0086956527f;
    if (frame <= 13.0f)
        impact->y = frame * -0.2f / 13.0f;
    else if (frame <= 37.0f) {
        frame -= 13.0f;
        impact->y = -0.2f + frame * 0.4f / 24.0f;
    } else {
        frame -= 37.0f;
        impact->y = 0.2f - frame * 0.2f / 7.0f;
    }
    impact->y += 1.4174f;
    impact->z = -0.068200007f;
    impact->x += FTBL.GetRandomNumberInRange(17.0f) * 0.002f - 0.017f;
    impact->y += FTBL.GetRandomNumberInRange(17.0f) * 0.002f - 0.017f;
}

float DartsVerticalWobble(int phase)
{
    return FTBL.CalculateFixedSine(phase * 22) * 0.3f + FTBL.CalculateFixedSine(phase * 31) * 0.37f + FTBL.CalculateFixedSine(phase * 103) * 0.15f
        + FTBL.CalculateFixedSine(phase * 147) * 0.18f;
}

void DartsWobbleHorizontal(DartsEventWork *work)
{
    Vector3 position;
    shen_name resolved;
    float offset;
    int next;
    resolved = FTBL.SCEN_FindChar_Wrapper(GetDartsArrowName(0, work->state));
    next = work->wobblePhaseY + 218;
    offset = FTBL.CalculateFixedSine(work->wobblePhaseY) * 0.04f;
    FTBL.CalculateFixedSine(next);
    FTBL.GetCharaPosition(GetDartsArrowName(0, work->state), -1, &position, 0);
    position.x = offset + DART_AIM_X;
    FTBL.EV_AT_SetPos(GetDartsArrowName(0, work->state), 0x38000000, &position);
    FTBL.AT_SetLocalBaseChara(DART_COIN, resolved);
    FTBL.GetCharaPosition(DART_COIN, -1, &position, 0);
    position.x = offset + DART_AIM_X;
    FTBL.EV_AT_SetPos(DART_COIN, 0x38000000, &position);
    FTBL.AT_SetLocalBaseChara(DART_COIN, -1);
    work->wobblePhaseY = next;
}

void DartsRotateAimingArrow(DartsEventWork *work)
{
    Vector3i rotation;
    Vector3 position;
    float distance;
    FTBL.GetCharaPosition(GetDartsArrowName(0, work->state), -1, &position, 0);
    position.x -= DART_AIM_X;
    position.y -= DART_AIM_Y;
    position.z += 0.0378f;
    rotation.y = FTBL.InverseFixedAtan2(-position.x, -position.z);
    distance = position.x * position.x + position.z * position.z;
    DART_FSQRT(distance);
    rotation.x = 10279 - FTBL.InverseFixedAtan2(-position.y, -distance);
    rotation.y = 50706 - rotation.y;
    rotation.z = 22478;
    FTBL.EV_AT_SetAngY_0(GetDartsArrowName(0, work->state), 0x38000000, &rotation, 0);
}

void DartsAttachTurnArrows(DartsEventWork *work)
{
    Vector3i rotation;
    Vector3 position;
    position.x = 0.0882f;
    position.y = -0.0112f;
    position.z = 0.0f;
    rotation.x = rotation.y = rotation.z = 0;
    FTBL.scenSetCharaPosRotById(GetDartsArrowName(0, work->state), &position, &rotation);
    if (work->standardBoard) {
        position.x = 0.136f;
        position.y = 0.024f;
        position.z = -0.073f;
        rotation.x = 62077;
        rotation.y = 2000;
        rotation.z = 15104;
    } else {
        position.x = 0.137f;
        position.y = 0.028f;
        position.z = -0.068f;
        rotation.x = rotation.y = 0;
        rotation.z = 15834;
    }
    FTBL.AttachScenarioCharacter(GetDartsArrowName(0, work->state), DartsTurnCharacter(work),
        DartsOpponentAttachBone(DartsTurnCharacter(work), work->difficulty), &position, &rotation);
}

void DartsWobbleVertical(DartsEventWork *work)
{
    Vector3 position;
    shen_name resolved;
    float offset;
    int next;
    resolved = FTBL.SCEN_FindChar_Wrapper(GetDartsArrowName(0, work->state));
    next = work->wobblePhaseX + 18;
    offset = DartsVerticalWobble(work->wobblePhaseX);
    DartsVerticalWobble(next);
    FTBL.GetCharaPosition(GetDartsArrowName(0, work->state), -1, &position, 0);
    position.y = offset * 0.05f + DART_AIM_Y;
    FTBL.EV_AT_SetPos(GetDartsArrowName(0, work->state), 0x38000000, &position);
    FTBL.AT_SetLocalBaseChara(DART_COIN, resolved);
    FTBL.GetCharaPosition(DART_COIN, -1, &position, 0);
    position.y = offset * 0.05f + DART_AIM_Y;
    FTBL.EV_AT_SetPos(DART_COIN, 0x38000000, &position);
    FTBL.AT_SetLocalBaseChara(DART_COIN, -1);
    work->wobblePhaseX = next;
}

void DartsCompactWobble(DartsEventWork *work)
{
    Vector3i rotation;
    int angle;
    work->wobblePhaseY %= 90;
    if (work->wobblePhaseY <= 22)
        angle = work->wobblePhaseY * 41;
    else if (work->wobblePhaseY <= 44)
        angle = 910 - (work->wobblePhaseY - 22) * 41;
    else if (work->wobblePhaseY <= 67) {
        angle = work->wobblePhaseY - 44;
        angle = -angle * 39;
    }
    else
        angle = (work->wobblePhaseY - 67) * 39 - 910;
    rotation.x = 0;
    rotation.y = -angle;
    rotation.z = 0;
    QueueDartsPosition(DartsSelectBoard(work, 0), DART_PLAYER, 0, &rotation, 0);
    work->wobblePhaseY++;
}

void DartsResetScoring(DartsEventWork *work)
{
    work->banks[1].score[1] = 0;
    work->banks[1].score[3] = 0;
    work->banks[1].ticker = 100000;
    work->banks[1].bullseyes = 0;
    work->banks[2].score[1] = 0;
    work->banks[2].score[3] = 0;
    work->banks[2].ticker = 100000;
    work->banks[2].bullseyes = 0;
    work->playerWon = work->scoreStep = 0;
    if (DartsIsOpponentMatch(work)) {
        if (work->field10)
            DartsSetTurn(work, 0);
        else
            DartsSetTurn(work, 1);
    } else if (work->entry->opponent == 1)
        DartsSetTurn(work, 1);
    else
        DartsSetTurn(work, 0);
}

void DartsResetRound(DartsEventWork *work)
{
    int i;
    work->state = 5;
    if (work->standardBoard) {
        if (DartsIsOpponentMatch(work) && work->entry->enemy->name == DART_XHO) {
            work->banks[0].throws = 3;
            work->banks[1].throws = 3;
            work->banks[2].throws = 3;
        } else {
            work->banks[0].throws = 0;
            work->banks[1].throws = 0;
            work->banks[2].throws = 0;
        }
    } else
        work->banks[0].throws = 3;
    work->banks[0].score[0] = 0;
    work->banks[1].score[0] = 0;
    work->banks[2].score[0] = 0;
    for (i = 0; i <= 4; i++)
        work->usedBands[i] = 0;
}

int DartsScoreImpact(DartsEventWork *work)
{
    int score;
    if (work->standardBoard)
        score = CalculateDartsPositionOrScore(-1, &work->impactPosition);
    else
        score = DartsCompactScore(&work->impactPosition);
    return score;
}

int DartsCompactScore(Vector3 *position)
{
    const volatile Vector3 *point = position;
    float x = point->x;
    float y = (point->y - 1.4174f) * 1.0072135925292969f;
    float radius = x * x + y * y;
    int score;
    DART_FSQRT(radius);
    if (radius <= 0.016f)
        score = 100;
    else if (radius <= 0.047f)
        score = 50;
    else if (radius <= 0.079f)
        score = 40;
    else if (radius <= 0.11f)
        score = 30;
    else if (radius <= 0.147f)
        score = 20;
    else if (radius <= 0.201f)
        score = 10;
    else
        score = 0;
    return score;
}

void DartsHideBoardArrow(shen_name board, char index, char hidden)
{
    Vector3i rotation;
    Vector3 position;
    position.x = position.y = position.z = 0.0f;
    rotation.x = rotation.y = rotation.z = 0;
    if (hidden)
        position.z -= 10.05f;
    if (index <= 4)
        FTBL.scenApplyPosOrRotToChildId(board, index + 153, &position, &rotation, 0);
}

int DartsIsPlayerTurn(DartsEventWork *work)
{
    int playerTurn = 1;
    if (DartsIsOpponentMatch(work))
        playerTurn = work->enemyTurn == 0;
    return playerTurn;
}

shen_name DartsTurnCharacter(DartsEventWork *work)
{
    return DartsIsPlayerTurn(work) ? DART_PLAYER : work->entry->enemy->name;
}

int DartsIsOpponentMatch(DartsEventWork *work)
{
    return work->entry->opponent == DARTS_MATCH_OPPONENT;
}

shen_name DartsSelectBoard(DartsEventWork *work, shen_name actor)
{
    shen_name board = work->entry->placement[work->entry->mode].character;
    if (DartsIsOpponentMatch(work)) {
        if (actor) {
            if (actor == DART_PLAYER && work->entry->placement[work->entry->mode].parent)
                board = work->entry->placement[work->entry->mode].parent;
        } else if (DartsIsPlayerTurn(work) && work->entry->placement[work->entry->mode].parent)
            board = work->entry->placement[work->entry->mode].parent;
    }
    return board;
}
shen_name DartsTurnArrowName(DartsEventWork *work, int index)
{
    shen_name arrow = GetDartsArrowName(0, index);
    if (DartsIsOpponentMatch(work) && !DartsIsPlayerTurn(work))
        arrow = GetDartsArrowName(1, index);
    return arrow;
}

int DartsIsMatchDecided(DartsEventWork *work)
{
    int decided = 0;
    int other, otherBest, mine, mineBest, limit;
    if (DartsIsOpponentMatch(work)) {
        if (work->entry->enemy->name == DART_XHO) {
            if (DartsIsPlayerTurn(work)) {
                otherBest = work->banks[2].bullseyes + work->state;
                other = work->banks[2].bullseyes;
            } else {
                otherBest = work->banks[1].bullseyes + work->state;
                other = work->banks[1].bullseyes;
            }
            if (work->scoreStep) {
                mineBest = work->banks[0].bullseyes + work->state - 1;
                mine = work->banks[0].bullseyes;
            } else {
                mineBest = work->banks[0].bullseyes + work->state;
                mine = work->banks[0].bullseyes;
            }
        } else {
            if (DartsIsPlayerTurn(work)) {
                otherBest = work->state * 700000 + work->banks[2].score[3];
                other = work->banks[2].score[3];
            } else {
                otherBest = work->state * 700000 + work->banks[1].score[3];
                other = work->banks[1].score[3];
            }
            if (work->scoreStep) {
                mineBest = (work->state - 1) * 700000 + work->banks[0].score[3];
                mine = work->banks[0].score[3];
            } else {
                mineBest = work->state * 700000 + work->banks[0].score[3];
                mine = work->banks[0].score[3];
            }
            if (work->banks[0].throws <= 1) {
                limit = (work->banks[0].throws + 1) * 1200000;
                if (otherBest >= limit && mineBest >= limit)
                    return decided;
            }
        }
        if (otherBest < mine || mineBest < other)
            decided = 1;
    }
    return decided;
}

int DartsBattleResult(DartsEventWork *work)
{
    int player, enemy, result;
    if (FTBL.EV_GetShenmueFlag(0x3e0)) {
        player = work->banks[1].score[3];
        enemy = work->banks[2].score[3];
    } else {
        player = work->banks[1].score[3] / 10000;
        enemy = work->banks[2].score[3] / 10000;
    }
    if (work->entry->enemy->name == DART_XHO) {
        if (work->banks[1].bullseyes > work->banks[2].bullseyes)
            result = 2;
        else
            result = work->banks[1].bullseyes == work->banks[2].bullseyes;
    } else if (player > enemy)
        result = 2;
    else
        result = player == enemy;
    return result;
}

void DartsShowTurnActor(DartsEventWork *work)
{
    if (DartsIsOpponentMatch(work)) {
        if (DartsIsPlayerTurn(work)) {
            FTBL.EV_EnableCharacterDisplay(DART_PLAYER);
            FTBL.EV_DisableCharacterDisplay(work->entry->enemy->name);
        } else {
            FTBL.EV_DisableCharacterDisplay(DART_PLAYER);
            FTBL.EV_EnableCharacterDisplay(work->entry->enemy->name);
        }
    }
}

void DartsSetTurn(DartsEventWork *work, char enemyTurn)
{
    work->enemyTurn = enemyTurn;
    if (!enemyTurn)
        work->banks[0] = work->banks[1];
    else {
        DartsLoadAudioBank(work, 2);
        work->banks[0] = work->banks[2];
    }
}

int DartsSwitchTurn(DartsEventWork *work)
{
    signed char previous = work->enemyTurn;
    work->enemyTurn = previous == 0;
    if (!work->enemyTurn) {
        work->banks[2] = work->banks[0];
        work->banks[0] = work->banks[1];
    } else {
        DartsLoadAudioBank(work, 2);
        work->banks[1] = work->banks[0];
        work->banks[0] = work->banks[2];
    }
    return previous;
}

void DartsStoreTurnBank(DartsEventWork *work)
{
    if (!work->enemyTurn)
        work->banks[1] = work->banks[0];
    else
        work->banks[2] = work->banks[0];
}
int DartsAiTriggerReady(DartsEventWork *work)
{
    Vector3 impact;
    float x, y, outer, inner, distance;
    int ready = 0;
    if (work->standardBoard) {
        DartsProjectImpact(work, &impact);
        y = impact.y * 1.0072135925292969f;
        x = impact.x;
        outer = work->entry->aiParameters[0] * work->targetBand;
        if (outer < 10.0f && !(work->entry->aiParameters[0] < 10.0f))
            outer = 10.0f;
        else if (outer < 10.0f)
            outer = work->entry->aiParameters[0];
        inner = work->entry->aiParameters[1] * work->targetBand;
        if (inner < 10.0f && work->entry->aiParameters[1] > 10.0f)
            inner = 10.0f;
        else if (outer < 10.0f)
            outer = work->entry->aiParameters[0];
        if (!work->field09) {
            DART_FSQRT_OF(distance, x * x + y * y);
            ready = outer * 0.2f / 100.0f > distance;
            if (ready)
                ready = distance > inner * 0.2f / 100.0f;
        }
    }
    return ready;
}

void DartsShowBankScore(DartsEventWork *work, int row)
{
    int score = 0;
    int high, low;
    switch (row) {
    case 0:
        score = work->banks[0].bestScore;
        if (score < 777777)
            score = 777777;
        break;
    case 1:
        score = work->banks[0].previousScore;
        break;
    case 2:
        score = work->banks[0].ticker;
        break;
    case 3:
        score = work->banks[0].score[3];
        break;
    case 4:
        score = work->banks[0].score[1];
        break;
    }
    high = score / 10000;
    low = score - high * 10000;
    if (row == 3 && (work->field0e & 32))
        low = FTBL.shenGetRandomInt32(10000);
    if (row == 2 && (work->field0e & 64))
        low = FTBL.shenGetRandomInt32(10000);
    score = high * 10000 + low;
    UpdateDartsScoreDisplay(DartsSelectBoard(work, 0), score, row);
}

void DartsShowCompactScore(DartsEventWork *work, int row)
{
    int score = 0;
    switch (row) {
    case 0:
        score = work->banks[0].bestScore;
        if (score < 50)
            score = 50;
        break;
    case 1:
        score = work->banks[0].previousScore;
        break;
    case 3:
        score = work->banks[0].score[3];
        break;
    }
    UpdateDartsCompactScoreDisplay(DartsSelectBoard(work, 0), score, row);
}

HLibTask *DartsQueueVoiceWait(HLibTask *parent, void *conversation)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DartsWaitVoice, 4, 11, sizeof(DartsVoiceWaitWork), DART_TASK);
    DartsVoiceWaitWork *wait = (DartsVoiceWaitWork *)FTBL.HLib_GetTaskParameter(task);
    if (wait) {
        wait->frames = 1000;
        wait->conversation = conversation;
        wait->parent = parent;
        FTBL.HLib_SetupTimedTask(parent, 0);
    }
    return task;
}

void DartsWaitVoice(void *data)
{
    DartsVoiceWaitWork *wait = (DartsVoiceWaitWork *)data;
    if (wait) {
        if (!FTBL.cvsHasValidConverseData(wait->conversation)) {
            FTBL.HLib_ResolveDeferredCallback(wait->parent);
            FTBL.HLib_DestroyCurrentTask();
        } else if (--wait->frames <= 0) {
            FTBL.HLib_ResolveDeferredCallback(wait->parent);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

int DartsPlaySample(unsigned int sample)
{
    switch (sample) {
    case 1:
        EVT_AT_SetSound(381, 0);
        break;
    case 0:
        EVT_AT_SetSound(380, 0);
        break;
    case 2:
        EVT_AT_SetSound(384, 0);
        break;
    case 3:
        EVT_AT_SetSound(385, 0);
        break;
    case 4:
    case 5:
        if (FTBL.GetRandomNumberInRange(2.0f) < 1.0f)
            EVT_AT_SetSound(382, 0);
        else
            EVT_AT_SetSound(383, 0);
        break;
    case 6:
        EVT_AT_SetSound(204, 0);
        break;
    case 7:
        EVT_AT_SetSound(388, 0);
        EVT_AT_SetSound(387, 0);
        break;
    case 8:
        EVT_AT_SetSound(388, 0);
        break;
    case 9:
        EVT_AT_SetSound(389, 0);
        break;
    case 10:
        EVT_AT_SetSound(389, 0);
        break;
    }
    return 0;
}

void *DartsResponse(DartsEventWork *work, int index)
{
    void *response;
    if (DartsIsOpponentMatch(work))
        response = DartsExecuteResponse(index, DartsIsOpponentMatch(work), DartsIsPlayerTurn(work), work->playerWon, work->entry->enemy->name);
    else
        response = DartsExecuteResponse(index, DartsIsOpponentMatch(work), DartsIsPlayerTurn(work), work->playerWon, -1);
    return response;
}

HLibTask *DartsQueueCameraWait(DartsEventWork *root, int camera, shen_name eye, shen_name target)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DartsCameraWait, 4, 11, sizeof(DartsCameraWaitWork), DART_TASK);
    DartsCameraWaitWork *wait = (DartsCameraWaitWork *)FTBL.HLib_GetTaskParameter(task);
    if (wait) {
        wait->root = root;
        wait->camera = camera;
        wait->eye = eye;
        wait->target = target;
        FTBL.HLib_SetupTimedTask(task, 2);
    } else
        FTBL.DebugLogToFileRemoved("CamctrlDartsWait Error\n");
    return task;
}

void DartsCameraWait(void *data)
{
    DartsCameraWaitWork *wait = (DartsCameraWaitWork *)data;
    if (!FTBL.camCtrlCheckIsEventCam()) {
        DartsCamera(wait->root, wait->camera, wait->eye, wait->target, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void DartsCamera(DartsEventWork *work, int camera, shen_name eye, shen_name target, int delay)
{
    switch (camera) {
    case 7201:
        if (work->standardBoard)
            EVT_AT_SetCam(7201, eye, target, 0, 0, delay);
        else
            EVT_AT_SetCam(7218, eye, target, 0, 0, delay);
        break;
    case 7207:
        EVT_AT_SetCam(7221, eye, target, 0, 0, delay);
        break;
    default:
        EVT_AT_SetCam(camera, eye, target, 0, 0, delay);
        break;
    }
}

void DartsLoadAudioBank(DartsEventWork *work, int bankMode)
{
    int sceneIndices[5] = {3, 4, 1, 2, 5};
    int index;
    if (work->audioBank != bankMode) {
        switch (bankMode) {
        case 1:
            FTBL.EV_LoadCutsceneFromCurrentScene("01130000.smp");
            FTBL.EV_LoadAudio("01130000.sbt", 0);
            break;
        case 2:
            FTBL.EV_LoadCutsceneFromCurrentScene("01130000.smp");
            if (DartsIsHardJon(work->entry->enemy->name, work->difficulty))
                FTBL.EV_LoadAudio("01130000.sbt", 6);
            else {
                index = DartsOpponentIndex(work->entry->enemy->name);
                FTBL.EV_LoadAudio("01130000.sbt", sceneIndices[index]);
            }
            break;
        }
        work->audioBank = bankMode;
    }
}

void DartsStartChildTask(DartsEventWork *work, int kind)
{
    HLibTaskCallback callbacks[4] = {DartsScoreDisplayTask, DartsFrameCounterTask, DartsScoreTickerTask, DartsOpponentChatterTask};
    HLibTask *task;
    DartsChildTaskWork *child;
    if (kind <= 3) {
        if (!work->childTasks[kind]) {
            task = FTBL.HLib_EnqueueTaskWithParameter(callbacks[kind], 4, 11, sizeof(DartsChildTaskWork), DART_TASK);
            work->childTasks[kind] = task;
            child = (DartsChildTaskWork *)FTBL.HLib_GetTaskParameter(task);
            if (child) {
                child->root = work->task;
                child->work = work;
                child->reserved = 0;
            } else if (!child)
                FTBL.EventAssertNullsub(__FILE__, 3817);
        } else
            FTBL.DebugLogToFileRemoved("Task Already Created %d\n", kind);
    } else
        FTBL.DebugLogToFileRemoved("Tmp Task Number Error %d\n", kind);
}

void DartsScoreDisplayTask(void *data)
{
    DartsChildTaskWork *child = (DartsChildTaskWork *)data;
    DartsEventWork *work = child->work;
    char flags = work->field0e;
    if (flags) {
        FTBL.scenCopyCharaToGame(DartsSelectBoard(work, 0));
        if (DartsSelectBoard(work, 0) != DARTS_COMPACT_BOARD) {
            if (flags & 2)
                DartsShowBankScore(work, 0);
            if (flags & 4)
                DartsShowBankScore(work, 1);
            if (flags & 8)
                DartsShowBankScore(work, 2);
            if (flags & 1)
                DartsShowBankScore(work, 3);
            if (flags & 16)
                DartsShowBankScore(work, 4);
        } else {
            if (flags & 2)
                DartsShowCompactScore(work, 0);
            if (flags & 4)
                DartsShowCompactScore(work, 1);
            if (flags & 1)
                DartsShowCompactScore(work, 3);
        }
    }
}

void DartsFrameCounterTask(void *data)
{
    DartsChildTaskWork *child = (DartsChildTaskWork *)data;
    child->work->frames++;
}

void DartsScoreTickerTask(void *data)
{
    DartsChildTaskWork *child = (DartsChildTaskWork *)data;
    if (child->work->scoreUiFlags) {
        if (child->work->standardBoard) {
            if (child->work->banks[0].ticker > 0) {
                child->work->banks[0].ticker = 100000 - child->work->banks[0].score[0] * 166;
                child->work->banks[0].score[0]++;
            } else
                child->work->banks[0].ticker = 0;
            if (DartsIsOpponentMatch(child->work) && DartsIsPlayerTurn(child->work) && child->work->banks[0].ticker / 10000 == 4
                && !child->work->childTasks[3])
                DartsStartChildTask(child->work, 3);
        }
    } else if (child->work->childTasks[3]) {
        FTBL.EV_HLKillOtherHierarchyTask(child->work->childTasks[3]);
        child->work->childTasks[3] = 0;
    }
}

void DartsOpponentChatterTask(void *data)
{
    DartsChildTaskWork *child = (DartsChildTaskWork *)data;
    DartsPlayOpponentChatter(child->work->entry->enemy->name);
    FTBL.HLib_SetupTaskDelay(800);
}

void DartsPositionOpponentHands(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    if (work) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(work->entry->enemy->name));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->entry->enemy->name), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->entry->enemy->name), 1, 1, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(work->entry->enemy->name), 0, dartsHandPoseCommand, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(work->entry->enemy->name), 1, dartsHandPoseCommand, 0);
        FTBL.EV_SetCharacterAlphaEnabled(work->entry->enemy->name, 0);
        FTBL.SetRefBoardParamsForCharaById(work->entry->enemy->name, 1);
        QueueDartsArrow(work->entry->enemy->name, GetDartsArrowName(2, 14));
        DartsLoadAudioBank(work, 2);
        EVT_AT_SetSample(0, 0, 0);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        if (FTBL.EV_GetShenmueFlag(0x3e0) && g_currentMap == DART_AKS1)
            FTBL.HLib_SetCurrentTaskTo(DartsInitStart);
        else
            FTBL.HLib_SetCurrentTaskTo(DartsOpponentThrowCallback(work->entry->enemy->name));
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void DartsOpponentAdmission(void *data)
{
    DartsEventWork *work = (DartsEventWork *)data;
    int index = DartsOpponentIndex(work->entry->enemy->name);
    int delay;
    switch (work->gameState) {
    case 0:
        work->gameState++;
        delay = DartsAdmissionIntroduction(work);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        work->gameState++;
        delay = DartsAdmissionDemonstration(work);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 2:
        work->gameState++;
        delay = DartsAdmissionOffer(work);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 3:
        delay = DartsCheckBet(work);
        if (!delay) {
            work->gameState++;
            EVT_AT_KsIcon_Enqueue(0xffff, 74, 75, 0xffff, 1800, 75, 2, 0);
            EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
            EVT_AT_CharaDisp(work->entry->enemy->name, 0, 1);
        } else if (delay > 0) {
            work->gameState = 0;
            EVT_AT_CharaDisp(work->entry->enemy->name, 0, 1);
            EVT_AT_CharaDisp(work->entry->enemy->name, 80, 0);
            FTBL.HLib_SetCurrentTaskTo(DartsExit);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_CharaDisp(work->entry->enemy->name, 0, 1);
            EVT_AT_CharaDisp(work->entry->enemy->name, 80, 0);
            FTBL.HLib_SetupTaskDelay(-delay);
        }
        break;
    case 4:
        if (!FTBL.GetShouldRenderIcons(2)) {
            work->gameState = 0;
            if (FTBL.shenGetCtrlSpeakIconFlag() == 74) {
                delay = DartsAcceptBet(work);
                FTBL.HLib_SetCurrentTaskTo(DartsInitStart);
                FTBL.HLib_SetupTaskDelay(delay);
            } else {
                delay = DartsDeclineBet(work);
                FTBL.HLib_SetCurrentTaskTo(DartsExit);
                FTBL.HLib_SetupTaskDelay(delay);
            }
        } else if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->entry->enemy->name), 65, 1, 0))
            EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[7], -1.0f, 1.0f, 0x80000000, 0);
        break;
    }
}

int DartsAdmissionIntroduction(DartsEventWork *work)
{
    int index = DartsOpponentIndex(work->entry->enemy->name);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, DART_PLAYER, 1.0f, 0.0f, 4.0f, 0x4000, 0);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 4.2f, 0, 0);
    EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
    EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[6], -1.0f, 1.0f, 0x80000000, 0);
    PrepareDartsOpponentAdmission(work->entry->enemy->name);
    EVT_AT_SetCam(index + 7415, DART_DUMMY1, DART_DUMMY1, 0, 0, 0);
    return dartsOpponents[index].motions[9];
}

int DartsAdmissionDemonstration(DartsEventWork *work)
{
    int arc, score;
    float duration;
    SetDartsTemporaryCharacter(work->entry->enemy->name);
    if (work->entry->enemy->name == DART_FCJ) {
        arc = 1;
        duration = 26.0f;
    } else {
        arc = 0;
        duration = 13.0f;
    }
    score = work->entry->opponentScoreFloor;
    DartsQueueAdmissionScore(work->entry->placement[work->entry->mode].character, work->entry->bestScore, work->entry->opponentScore, 0, score, 55);
    DartsQueueProjectile(work->entry->placement[work->entry->mode].character, GetDartsArrowName(2, 14), score, 0, duration, arc);
    EVT_AT_SetCam(7311, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 10);
    EVT_AT_SetCam(7312, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 70);
    return 215;
}

HLibTask *DartsQueueAdmissionScore(shen_name board, int best, int previous, int current, int amount, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DartsAdmissionScoreTask, 4, 11, sizeof(DartsAdmissionScoreWork), DART_TASK);
    DartsAdmissionScoreWork *score = (DartsAdmissionScoreWork *)FTBL.HLib_GetTaskParameter(task);
    if (score) {
        score->soundStarted = 0;
        score->board = board;
        score->scores[0] = best;
        score->scores[1] = previous;
        score->scores[2] = current;
        score->scores[3] = previous - amount;
        score->remaining = amount;
        FTBL.scenCopyCharaToGame(board);
        UpdateDartsScoreDisplay(score->board, score->scores[0], 0);
        UpdateDartsScoreDisplay(score->board, score->scores[1], 1);
        UpdateDartsScoreDisplay(score->board, score->scores[2], 2);
        UpdateDartsScoreDisplay(score->board, score->scores[3], 3);
        if (delay)
            FTBL.HLib_SetupTimedTask(task, delay);
    } else
        task = 0;
    return task;
}

void DartsAdmissionScoreTask(void *data)
{
    DartsAdmissionScoreWork *score = (DartsAdmissionScoreWork *)data;
    if (score) {
        if (score->remaining < 2) {
            score->scores[3] += score->remaining;
            DartsPlaySample(8);
            FTBL.HLib_DestroyCurrentTask();
        } else {
            score->scores[3] += 20000;
            score->remaining -= 20000;
            if (!score->soundStarted) {
                score->soundStarted = 1;
                DartsPlaySample(7);
            }
        }
        FTBL.scenCopyCharaToGame(score->board);
        UpdateDartsScoreDisplay(score->board, score->scores[0], 0);
        UpdateDartsScoreDisplay(score->board, score->scores[1], 1);
        UpdateDartsScoreDisplay(score->board, score->scores[2], 2);
        UpdateDartsScoreDisplay(score->board, score->scores[3], 3);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

int DartsAdmissionOffer(DartsEventWork *work)
{
    int index = DartsOpponentIndex(work->entry->enemy->name);
    int frames = PlayDartsOpponentAdmission(work->entry->enemy->name);
    switch (work->entry->enemy->name) {
    case DART_FCJ:
        EVT_AT_SetCam(7290, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 0);
        work->entry->bet = 10;
        if (!FTBL.EV_GetPlayerFlag(11, 478))
            FTBL.EV_SetPlayerFlag(12, 478, 1);
        break;
    case DART_HCY:
        EVT_AT_SetCam(7310, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 0);
        work->entry->bet = 50;
        if (!FTBL.EV_GetPlayerFlag(11, 479))
            FTBL.EV_SetPlayerFlag(12, 479, 1);
        break;
    case DART_HLN:
        EVT_AT_SetCam(7290, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 0);
        work->entry->bet = 5;
        if (!FTBL.EV_GetPlayerFlag(11, 477))
            FTBL.EV_SetPlayerFlag(12, 477, 1);
        break;
    case DART_JON:
        EVT_AT_SetCam(7310, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 0);
        work->entry->bet = 10;
        if (!FTBL.EV_GetPlayerFlag(11, 476))
            FTBL.EV_SetPlayerFlag(12, 476, 1);
        break;
    default:
        EVT_AT_SetCam(7310, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 0);
        work->entry->bet = 30;
        if (!FTBL.EV_GetPlayerFlag(11, 475))
            FTBL.EV_SetPlayerFlag(12, 475, 1);
        break;
    }
    EVT_AT_CharaDisp(GetDartsArrowName(2, 14), 0, 1);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0, 0);
    EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[8], -1.0f, 1.0f, 0x80000000, 0);
    return frames;
}

int DartsAcceptBet(DartsEventWork *work)
{
    int delay, frames, index;
    switch (work->entry->enemy->name) {
    case DART_HLN:
    default:
        delay = 40;
        break;
    case DART_FCJ:
        delay = 50;
        break;
    case DART_JON:
        delay = 50;
        break;
    case DART_HCY:
        delay = 100;
        break;
    }
    frames = PlayDartsBetResponse(work->entry->enemy->name, work->difficulty, work->entry->bet);
    index = DartsOpponentIndex(work->entry->enemy->name);
    SetDartsPosition(work->entry->placement[work->entry->mode].character, work->entry->enemy->name, 0.0f, 0.0f, 4.0f, 0, 0);
    EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[10], -1.0f, 1.0f, 0x80000000, 0);
    EVT_AT_SetCam(index + 7550, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, delay);
    EVT_AT_CharaDisp(work->entry->enemy->name, delay, 0);
    EVT_AT_CharaDisp(DART_PLAYER, delay, 1);
    EVT_AT_CharaDisp(DART_PLAYER, frames + 10, 0);
    return frames;
}

int DartsDeclineBet(DartsEventWork *work)
{
    int index = DartsOpponentIndex(work->entry->enemy->name);
    int frames = EVT_AT_SetMot(work->entry->enemy->name, dartsOpponents[index].motions[11], -1.0f, 1.0f, 0x80000000, 0);
    if (frames < 120)
        frames = 120;
    DartsResponse(work, 20);
    EVT_AT_SetCam(7317, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 80);
    EVT_AT_CharaDisp(work->entry->enemy->name, 80, 0);
    work->gameState = 0;
    return frames;
}

int DartsCheckBet(DartsEventWork *work)
{
    int frames = 0;
    if (FTBL.GetPlayerStat(2) < work->entry->bet) {
        frames = PlayDartsOpponentResult(work->entry->enemy->name, work->entry->bet);
        EVT_AT_SetMot(DART_PLAYER, 0xf002, -1.0f, 1.0f, 0x80000000, 0);
        EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
        EVT_AT_SetCam(7317, work->entry->placement[work->entry->mode].character, work->entry->placement[work->entry->mode].character, 0, 0, 80);
        if (work->entry->enemy->name == DART_FCJ)
            switch (work->entry->bet) {
            case 10:
                work->entry->bet = 5;
                break;
            case 5:
                work->entry->bet = 1;
                break;
            }
    }
    return frames;
}

int DartsOpponentIndex(shen_name opponent)
{
    int i;
    for (i = 0; i <= 4; i++)
        if (opponent == dartsOpponents[i].name)
            break;
    return i;
}

HLibTaskCallback DartsOpponentThrowCallback(shen_name opponent)
{
    HLibTaskCallback callback = 0;
    int index = DartsOpponentIndex(opponent);
    if (index <= 4)
        callback = dartsOpponents[index].throwCallback;
    if (!callback)
        callback = DartsOpponentAdmission;
    return callback;
}

int DartsIsHardJon(shen_name opponent, int difficulty)
{
    if (difficulty > 1 && opponent == DART_JON)
        return 1;
    return 0;
}

int DartsOpponentAttachBone(shen_name opponent, int difficulty)
{
    int bone;
    if (!DartsIsHardJon(opponent, difficulty))
        bone = dartsOpponents[DartsOpponentIndex(opponent)].bone;
    else
        bone = 14;
    return bone;
}

unsigned int DartsBeginOpponentThrow(shen_name opponent, int mode, int difficulty)
{
    int index, frames;
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(opponent), 13, 0);
    index = DartsOpponentIndex(opponent);
    if (mode)
        frames = dartsOpponents[index].motions[2];
    else
        frames = dartsOpponents[index].motions[3];
    if (mode) {
        if (DartsIsHardJon(opponent, difficulty))
            EVT_AT_SetMot(opponent, dartsOpponents[index].motions[0], -1.0f, 1.0f, 0x20000000, 0);
        else
            EVT_AT_SetMot(opponent, dartsOpponents[index].motions[0], -1.0f, 1.0f, 0, 0);
    } else if (DartsIsHardJon(opponent, difficulty))
        EVT_AT_SetMot(opponent, dartsOpponents[index].motions[1], -1.0f, 1.0f, 0x20000000, 0);
    else
        EVT_AT_SetMot(opponent, dartsOpponents[index].motions[1], -1.0f, 1.0f, 0, 0);
    return frames;
}

void DartsQueueProjectile(shen_name board, shen_name arrow, int score, Vector3 *impact, float duration, int arc)
{
    Vector3 position;
    Vector3i rotation;
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DartsProjectileTask, 4, 11, sizeof(DartsProjectileWork), DART_TASK);
    DartsProjectileWork *projectile = (DartsProjectileWork *)FTBL.HLib_GetTaskParameter(task);
    if (!projectile)
        return;
    projectile->frames = (int)duration - 1;
    projectile->board = board;
    projectile->arrow = arrow;
    projectile->arcStep = 0;
    if (arc)
        projectile->arcPhase = (unsigned short)(int)(360.0f / duration * 8192.0f / 45.0f);
    else
        projectile->arcPhase = 0;
    FTBL.GetCharaPosition(arrow, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(arrow, &rotation, 0x40000000);
    FTBL.EV_AT_SetPos(DART_COIN, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(DART_COIN, 0x78000000, &rotation, 0);
    FTBL.EV_DisableCharacterDisplay(DART_COIN);
    FTBL.EV_EnsureCharacterDraw(DART_COIN);
    if (impact)
        projectile->target = *impact;
    else {
        score /= 10000;
        CalculateDartsPositionOrScore(score, &projectile->target);
    }
    FTBL.GetCharaPosition(arrow, -1, &position, 0);
    FTBL.GetScenarioCharacterEulerAngles(arrow, &rotation, 0);
    SetDartsPosition(board, arrow, projectile->target.x, projectile->target.y, projectile->target.z, 0xc000, 0);
    FTBL.GetCharaPosition(arrow, -1, &projectile->target, 0);
    FTBL.GetScenarioCharacterEulerAngles(arrow, &projectile->targetRotation, 0);
    projectile->step.x = (projectile->target.x - position.x) / duration;
    projectile->step.y = (projectile->target.y - position.y) / duration;
    projectile->step.z = (projectile->target.z - position.z) / duration;
    position.x += projectile->step.x;
    position.y += projectile->step.y;
    position.z += projectile->step.z;
    FTBL.EV_AT_SetPos(arrow, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(arrow, 0x38000000, &rotation, 0);
    DartsPlaySample(6);
}

void DartsProjectileTask(void *data)
{
    DartsProjectileWork *projectile = (DartsProjectileWork *)data;
    Vector3 position;
    Vector3i rotation;
    if (projectile) {
        if (projectile->frames) {
            projectile->frames--;
            FTBL.GetCharaPosition(projectile->arrow, -1, &position, 0);
            position.x += projectile->step.x;
            position.y += projectile->step.y + FTBL.CalculateFixedSine(projectile->arcStep) * 0.05f;
            position.z += projectile->step.z;
            FTBL.EV_AT_SetPos(projectile->arrow, 0x38000000, &position);
            FTBL.EV_AT_SetAngY_0(projectile->arrow, 0x38000000, &projectile->targetRotation, 0);
            projectile->arcStep += projectile->arcPhase;
        } else {
            FTBL.EV_AT_SetPos(projectile->arrow, 0x38000000, &projectile->target);
            FTBL.EV_AT_SetAngY_0(projectile->arrow, 0x38000000, &projectile->targetRotation, 0);
            DartsPlaySample(4);
            FTBL.HLib_DestroyCurrentTask();
        }
        FTBL.GetCharaPosition(projectile->arrow, -1, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(projectile->arrow, &rotation, 0x40000000);
        FTBL.EV_AT_SetPos(DART_COIN, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(DART_COIN, 0x78000000, &rotation, 0);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void *DartsExecuteResponse(int index, int opponentMatch, int playerTurn, int playerWon, shen_name opponent)
{
    FTBL.scenCvsCtrlFree(0);
    dartsPlayerWon = playerWon;
    return dartsResponseCallbacks[index](opponentMatch, playerTurn, opponent);
}

const DartsResponseCallback dartsResponseCallbacks[30] = {
    PlayDartsOpeningResponse,
    PlayDartsReadyResponse,
    PlayDartsExitResponse,
    PlayDartsContinueResponse,
    PlayDartsLastThrowResponse,
    PlayDartsZeroScoreResponse,
    PlayDartsLowScoreResponse,
    PlayDartsLowerMiddleScoreResponse,
    PlayDartsMiddleScoreResponse,
    PlayDartsUpperMiddleScoreResponse,
    PlayDartsUpperScoreResponse,
    PlayDartsBullseyeResponse,
    PlayDartsHighScoreResponse,
    PlayDartsEndThrowsResponse,
    PlayDartsRoundResultResponse,
    PlayDartsBonusResultResponse,
    PlayDartsFinalResultResponse,
    PlayDartsGameOverResponse,
    PlayDartsReplayResponse,
    PlayDartsStopResponse,
    PlayDartsDeclineOpponentResponse,
    PlayDartsWonBetResponse,
    PlayDartsOpponentPaymentResponse,
    PlayDartsLostBetResponse,
    PlayDartsOpponentReplayResponse,
    PlayDartsOpponentBonusReplayResponse,
    PlayDartsOpponentFinalResponse,
    PlayDartsThrowStartResponse,
    PlayDartsThrowResponse,
    PlayDartsRoundOverResponse
};

void *PlayDartsOpeningResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.5f) {
        voice = FTBL.EV_PlayDialogue("0113A001");
    } else {
        voice = FTBL.EV_PlayDialogue("0113A002");
    }
    return voice;
}

void *PlayDartsReadyResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A003");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A004");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A005");
        }
    }
    return voice;
}

void *PlayDartsExitResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A006");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A007");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A008");
        }
    }
    return voice;
}

void *PlayDartsContinueResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A009");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A010");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A011");
        }
    }
    return voice;
}

void *PlayDartsLastThrowResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (playerTurn) {
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0113A032");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0113A033");
            } else {
                voice = FTBL.EV_PlayDialogue("0113A034");
            }
        }
    } else {
        voice = FTBL.EV_PlayDialogue("0113A033");
    }
    return voice;
}

void *PlayDartsZeroScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0113A016");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A017");
        }
    } else {
        if (playerTurn) {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f || random < 2.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D042");
                } else {
                    voice = FTBL.EV_PlayDialogue("0137D043");
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F044");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F045");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F046");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C023");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C038");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C039");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B038");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B039");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B052");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E034");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E035");
                    } else {
                        if (random < 1.5f || random >= 2.0f && random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137E036");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137E037");
                        }
                    }
                }
                break;
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D026");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D027");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D028");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F028");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F029");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F030");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C024");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C025");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C023");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B029");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B030");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B031");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E023");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E024");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137E026");
                        } else {
                            if (random < 2.5f) {
                                voice = FTBL.EV_PlayDialogue("0137E027");
                            } else {
                                voice = FTBL.EV_PlayDialogue("0137E025");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsLowScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        voice = FTBL.EV_PlayDialogue("0113A013");
    } else {
        if (playerTurn) {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D038");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D039");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D040");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F041");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F042");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F043");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C035");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C036");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C037");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B035");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B036");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B037");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E034");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E035");
                    } else {
                        if (random < 1.5f || random >= 2.0f && random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137E036");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137E037");
                        }
                    }
                }
                break;
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D023");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D024");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D025");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F025");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F026");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F027");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C020");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C021");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C022");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B050");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B051");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B028");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E023");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E024");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137E026");
                        } else {
                            if (random < 2.5f) {
                                voice = FTBL.EV_PlayDialogue("0137E027");
                            } else {
                                voice = FTBL.EV_PlayDialogue("0137E025");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsLowerMiddleScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    return PlayDartsMiddleScoreResponse(mode, playerTurn, opponent);
}

void *PlayDartsMiddleScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        if (random < 1.5f) {
            voice = FTBL.EV_PlayDialogue("0113A018");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A019");
        }
    } else {
        if (playerTurn) {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D035");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D036");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D037");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F038");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F039");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F040");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C032");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C033");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C034");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B025");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B026");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B027");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E034");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E035");
                    } else {
                        if (random < 1.5f || random >= 2.0f && random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137E036");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137E037");
                        }
                    }
                }
                break;
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D020");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D021");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D022");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F022");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F023");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F024");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C017");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C018");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C019");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B025");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B026");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B027");
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E023");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E024");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137E026");
                        } else {
                            if (random < 2.5f) {
                                voice = FTBL.EV_PlayDialogue("0137E027");
                            } else {
                                voice = FTBL.EV_PlayDialogue("0137E025");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsUpperMiddleScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    return PlayDartsUpperScoreResponse(mode, playerTurn, opponent);
}

void *PlayDartsUpperScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    if (mode)
        voice = PlayDartsHighScoreResponse(mode, playerTurn, opponent);
    else
        voice = FTBL.EV_PlayDialogue("0113A014");
    return voice;
}

void *PlayDartsBullseyeResponse(int mode, int playerTurn, shen_name opponent)
{
    float random;
    if (mode && opponent == DART_XHO) {
        random = FTBL.GetRandomNumberInRange(3.0f);
        if (playerTurn) {
            if (random < 1.0f)
                return FTBL.EV_PlayDialogue("0137E031");
            else if (random < 1.5f)
                return FTBL.EV_PlayDialogue("0137E032");
            else if (random < 2.5f)
                return FTBL.EV_PlayDialogue("0137E033");
            else
                return FTBL.EV_PlayDialogue("0137E022");
        } else if (random < 1.0f)
            return FTBL.EV_PlayDialogue("0137E019");
        else if (random < 1.5f)
            return FTBL.EV_PlayDialogue("0137E020");
        else if (random < 2.5f)
            return FTBL.EV_PlayDialogue("0137E021");
        else
            return FTBL.EV_PlayDialogue("0137E022");
    }
    return PlayDartsHighScoreResponse(mode, playerTurn, opponent);
}

void *PlayDartsHighScoreResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        voice = FTBL.EV_PlayDialogue("0113A015");
    } else {
        if (playerTurn) {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D032");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D033");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D034");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F034");
                } else {
                    if (random < 1.5f) {
                        voice = FTBL.EV_PlayDialogue("0137F035");
                    } else {
                        if (random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137F036");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137F037");
                        }
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C030");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C029");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C031");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B021");
                } else {
                    if (random < 1.5f) {
                        voice = FTBL.EV_PlayDialogue("0137B022");
                    } else {
                        if (random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137B023");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137B024");
                        }
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E034");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E035");
                    } else {
                        if (random < 1.5f || random >= 2.0f && random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137E036");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137E037");
                        }
                    }
                }
                break;
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D017");
                } else {
                    if (random < 1.5f) {
                        voice = FTBL.EV_PlayDialogue("0137D018");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D019");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F019");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F020");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F021");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C014");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C015");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C016");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B021");
                } else {
                    if (random < 1.5f) {
                        voice = FTBL.EV_PlayDialogue("0137B022");
                    } else {
                        if (random < 2.5f) {
                            voice = FTBL.EV_PlayDialogue("0137B023");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137B024");
                        }
                    }
                }
                break;
            default:
                if (random < 0.5f) {
                    voice = FTBL.EV_PlayDialogue("0137E023");
                } else {
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137E024");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137E026");
                        } else {
                            if (random < 2.5f) {
                                voice = FTBL.EV_PlayDialogue("0137E027");
                            } else {
                                voice = FTBL.EV_PlayDialogue("0137E025");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsEndThrowsResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (playerTurn) {
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0113A023");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0113A024");
            } else {
                voice = FTBL.EV_PlayDialogue("0113A025");
            }
        }
    } else {
        voice = FTBL.EV_PlayDialogue("0113A025");
    }
    return voice;
}

void *PlayDartsRoundResultResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0113A026");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0113A027");
            } else {
                voice = FTBL.EV_PlayDialogue("0113A028");
            }
        }
    } else {
        if (playerTurn) {
            if (dartsPlayerWon) {
                switch (opponent) {
                case DART_FCJ:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D065");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137D066");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137D067");
                        }
                    }
                    break;
                case DART_HCY:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F061");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137F062");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137F063");
                        }
                    }
                    break;
                case DART_HLN:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C053");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137C054");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137C055");
                        }
                    }
                    break;
                case DART_JON:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B059");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137B060");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137B061");
                        }
                    }
                    break;
                default:
                    voice = FTBL.EV_PlayDialogue("0137E049");
                    break;
                }
            } else {
                voice = FTBL.EV_PlayDialogue("0113A035");
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D059");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D060");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D061");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F055");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F056");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F057");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C047");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C048");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C049");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B053");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B054");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B055");
                    }
                }
                break;
            default:
                voice = FTBL.EV_PlayDialogue("0137E019");
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsBonusResultResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (playerTurn) {
        voice = FTBL.EV_PlayDialogue("0113A035");
    } else {
        switch (opponent) {
        case DART_FCJ:
            if (random < 1.5f) {
                voice = FTBL.EV_PlayDialogue("0137D068");
            } else {
                voice = FTBL.EV_PlayDialogue("0137D069");
            }
            break;
        case DART_HCY:
            if (random < 1.5f) {
                voice = FTBL.EV_PlayDialogue("0137F064");
            } else {
                voice = FTBL.EV_PlayDialogue("0137F065");
            }
            break;
        case DART_HLN:
            if (random < 1.5f) {
                voice = FTBL.EV_PlayDialogue("0137C056");
            } else {
                voice = FTBL.EV_PlayDialogue("0137C057");
            }
            break;
        case DART_JON:
            if (random < 1.5f) {
                voice = FTBL.EV_PlayDialogue("0137B062");
            } else {
                voice = FTBL.EV_PlayDialogue("0137B063");
            }
            break;
        default:
            if (random < 1.0f) {
                voice = FTBL.EV_PlayDialogue("0137E056");
            } else {
                voice = FTBL.EV_PlayDialogue("0137E057");
            }
            break;
        }
    }
    return voice;
}

void *PlayDartsFinalResultResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (!mode) {
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0113A029");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0113A030");
            } else {
                voice = FTBL.EV_PlayDialogue("0113A031");
            }
        }
    } else {
        if (playerTurn) {
            if (dartsPlayerWon) {
                switch (opponent) {
                case DART_FCJ:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D065");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137D066");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137D067");
                        }
                    }
                    break;
                case DART_HCY:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F061");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137F062");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137F063");
                        }
                    }
                    break;
                case DART_HLN:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C053");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137C054");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137C055");
                        }
                    }
                    break;
                case DART_JON:
                    if (random < 1.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B059");
                    } else {
                        if (random < 2.0f) {
                            voice = FTBL.EV_PlayDialogue("0137B060");
                        } else {
                            voice = FTBL.EV_PlayDialogue("0137B061");
                        }
                    }
                    break;
                default:
                    voice = FTBL.EV_PlayDialogue("0137E049");
                    break;
                }
            } else {
                voice = FTBL.EV_PlayDialogue("0113A035");
            }
        } else {
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137D059");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137D060");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137D061");
                    }
                }
                break;
            case DART_HCY:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137F055");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137F056");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137F057");
                    }
                }
                break;
            case DART_HLN:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137C047");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137C048");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137C049");
                    }
                }
                break;
            case DART_JON:
                if (random < 1.0f) {
                    voice = FTBL.EV_PlayDialogue("0137B053");
                } else {
                    if (random < 2.0f) {
                        voice = FTBL.EV_PlayDialogue("0137B054");
                    } else {
                        voice = FTBL.EV_PlayDialogue("0137B055");
                    }
                }
                break;
            default:
                voice = FTBL.EV_PlayDialogue("0137E019");
                break;
            }
        }
    }
    return voice;
}

void *PlayDartsGameOverResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A035");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A036");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A037");
        }
    }
    return voice;
}

void *PlayDartsReplayResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A038");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A039");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A040");
        }
    }
    return voice;
}

void *PlayDartsStopResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (random < 1.0f) {
        voice = FTBL.EV_PlayDialogue("0113A041");
    } else {
        if (random < 2.0f) {
            voice = FTBL.EV_PlayDialogue("0113A042");
        } else {
            voice = FTBL.EV_PlayDialogue("0113A043");
        }
    }
    return voice;
}

void *PlayDartsDeclineOpponentResponse(int mode, int playerTurn, shen_name opponent)
{
    if (opponent == DART_HCY) {
        EVT_AT_SetVoice("0137A012", 0);
        EVT_AT_SetVoice("0137F010", 80);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 80);
    } else
        PlayDartsOpponentExit(opponent);
    return 0;
}

void *PlayDartsWonBetResponse(int mode, int playerTurn, shen_name opponent)
{
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        EVT_AT_SetVoice("0137D070", 0);
        EVT_AT_SetVoice("0137D071", 350);
        EVT_AT_SetVoice("0137D054", 400);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 0);
        break;
    case DART_HCY:
        EVT_AT_SetVoice("0137F048", 0);
        EVT_AT_SetVoice("0137F066", 160);
        EVT_AT_SetVoice("0137F067", 360);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 0);
        break;
    case DART_HLN:
        EVT_AT_SetVoice("0137C045", 0);
        EVT_AT_SetVoice("0137C060", 240);
        EVT_AT_SetVoice("0137C061", 290);
        return 0;
    case DART_JON:
        if (FTBL.EV_GetPlayerFlag(11, 502)) {
            EVT_AT_SetVoice("0137B046", 0);
            EVT_AT_SetVoice("0137B070", 120);
            EVT_AT_SetVoice("0137B071", 270);
        } else {
            EVT_AT_SetVoice("0137B046", 0);
            EVT_AT_SetVoice("0137B067", 120);
            EVT_AT_SetVoice("0137B068", 270);
        }
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 0);
        break;
    default:
        EVT_AT_SetVoice("0137E060", 0);
        EVT_AT_SetVoice("0137E061", 160);
        EVT_AT_SetVoice("0137E062", 380);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 0);
        break;
    }
    return 0;
}

void *PlayDartsOpponentPaymentResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        voice = FTBL.EV_PlayDialogue("0137D058");
        break;
    case DART_HCY:
        voice = FTBL.EV_PlayDialogue("0137F052");
        break;
    case DART_HLN:
        voice = FTBL.EV_PlayDialogue("0137C046");
        break;
    case DART_JON:
        voice = FTBL.EV_PlayDialogue("0137B049");
        break;
    default:
        voice = FTBL.EV_PlayDialogue("0137E050");
        break;
    }
    return voice;
}

void *PlayDartsLostBetResponse(int mode, int playerTurn, shen_name opponent)
{
    EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 0);
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        EVT_AT_SetVoice("0137D072", 0);
        EVT_AT_SetVoice("0137D056", 150);
        EVT_AT_SetVoice("0137D073", 310);
        break;
    case DART_HCY:
        EVT_AT_SetVoice("0137F068", 0);
        EVT_AT_SetVoice("0137F050", 90);
        EVT_AT_SetVoice("0137F069", 240);
        break;
    case DART_HLN:
        EVT_AT_SetVoice("0137C043", 0);
        EVT_AT_SetVoice("0137C058", 100);
        EVT_AT_SetVoice("0137C059", 190);
        break;
    case DART_JON:
        if (FTBL.EV_GetPlayerFlag(11, 502)) {
            EVT_AT_SetVoice("0137B066", 0);
            EVT_AT_SetVoice("0137B044", 60);
            EVT_AT_SetVoice("0137B065", 230);
        } else {
            EVT_AT_SetVoice("0137B066", 0);
            EVT_AT_SetVoice("0137B064", 60);
            EVT_AT_SetVoice("0137B065", 230);
        }
        break;
    default:
        EVT_AT_SetVoice("0137E058", 0);
        EVT_AT_SetVoice("0137E047", 90);
        EVT_AT_SetVoice("0137E059", 260);
        break;
    }
    return 0;
}

void *PlayDartsOpponentReplayResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice = 0;
    EVT_AT_SetFaceLooks(opponent, 0, 30, 1.0f, 0);
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        voice = FTBL.EV_PlayDialogue("0137D055");
        break;
    case DART_HCY:
        voice = FTBL.EV_PlayDialogue("0137F049");
        break;
    case DART_HLN:
        voice = FTBL.EV_PlayDialogue("0137C044");
        break;
    case DART_JON:
        EVT_AT_SetVoice("0137B047", 0);
        EVT_AT_SetVoice("0137B073", 110);
        break;
    default:
        voice = FTBL.EV_PlayDialogue("0137E063");
        break;
    }
    return voice;
}

void *PlayDartsOpponentBonusReplayResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice = 0;
    EVT_AT_SetFaceLooks(opponent, 0, 30, 1.0f, 0);
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        voice = FTBL.EV_PlayDialogue("0137D074");
        break;
    case DART_HCY:
        voice = FTBL.EV_PlayDialogue("0137F049");
        break;
    case DART_HLN:
        voice = FTBL.EV_PlayDialogue("0137C062");
        break;
    case DART_JON:
        voice = FTBL.EV_PlayDialogue("0137B047");
        break;
    default:
        EVT_AT_SetVoice("0137E064", 0);
        EVT_AT_SetVoice("0137E065", 70);
        break;
    }
    return voice;
}

void *PlayDartsOpponentFinalResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    EVT_AT_SetFaceLooks(opponent, 0, 30, 1.0f, 0);
    FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        voice = FTBL.EV_PlayDialogue("0137D057");
        break;
    case DART_HCY:
        voice = FTBL.EV_PlayDialogue("0137F051");
        break;
    case DART_HLN:
        voice = FTBL.EV_PlayDialogue("0137C044");
        break;
    case DART_JON:
        voice = FTBL.EV_PlayDialogue("0137B047");
        break;
    default:
        voice = FTBL.EV_PlayDialogue("0137E049");
        break;
    }
    return voice;
}

void *PlayDartsThrowStartResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random;
    EVT_AT_SetFaceLooks(opponent, 0, 30, 1.0f, 0);
    random = FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137D014");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137D015");
            } else {
                voice = FTBL.EV_PlayDialogue("0137D016");
            }
        }
        break;
    case DART_HCY:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137F016");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137F017");
            } else {
                voice = FTBL.EV_PlayDialogue("0137F018");
            }
        }
        break;
    case DART_HLN:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137C011");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137C012");
            } else {
                voice = FTBL.EV_PlayDialogue("0137C013");
            }
        }
        break;
    case DART_JON:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137B018");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137B019");
            } else {
                voice = FTBL.EV_PlayDialogue("0137B020");
            }
        }
        break;
    default:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137E016");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137E017");
            } else {
                voice = FTBL.EV_PlayDialogue("0137E018");
            }
        }
        break;
    }
    return voice;
}

void *PlayDartsThrowResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice;
    float random;
    random = FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137D029");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137D030");
            } else {
                voice = FTBL.EV_PlayDialogue("0137D031");
            }
        }
        break;
    case DART_HCY:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137F031");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137F032");
            } else {
                voice = FTBL.EV_PlayDialogue("0137F033");
            }
        }
        break;
    case DART_HLN:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137C026");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137C027");
            } else {
                voice = FTBL.EV_PlayDialogue("0137C028");
            }
        }
        break;
    case DART_JON:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137B032");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137B033");
            } else {
                voice = FTBL.EV_PlayDialogue("0137B034");
            }
        }
        break;
    default:
        if (random < 1.0f) {
            voice = FTBL.EV_PlayDialogue("0137E028");
        } else {
            if (random < 2.0f) {
                voice = FTBL.EV_PlayDialogue("0137E029");
            } else {
                voice = FTBL.EV_PlayDialogue("0137E030");
            }
        }
        break;
    }
    return voice;
}

void *PlayDartsRoundOverResponse(int mode, int playerTurn, shen_name opponent)
{
    void *voice = 0;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    if (mode) {
        if (playerTurn)
            voice = FTBL.EV_PlayDialogue("0113A035");
        else
            switch (opponent) {
            case DART_FCJ:
                if (random < 1.0f)
                    voice = FTBL.EV_PlayDialogue("0137D062");
                else if (random < 2.0f)
                    voice = FTBL.EV_PlayDialogue("0137D063");
                else
                    voice = FTBL.EV_PlayDialogue("0137D064");
                break;
            case DART_HCY:
                if (random < 1.0f)
                    voice = FTBL.EV_PlayDialogue("0137F058");
                else if (random < 2.0f)
                    voice = FTBL.EV_PlayDialogue("0137F059");
                else
                    voice = FTBL.EV_PlayDialogue("0137F060");
                break;
            case DART_HLN:
                if (random < 1.0f)
                    voice = FTBL.EV_PlayDialogue("0137C050");
                else if (random < 2.0f)
                    voice = FTBL.EV_PlayDialogue("0137C051");
                else
                    voice = FTBL.EV_PlayDialogue("0137C052");
                break;
            case DART_JON:
                if (random < 1.0f)
                    voice = FTBL.EV_PlayDialogue("0137B056");
                else if (random < 2.0f)
                    voice = FTBL.EV_PlayDialogue("0137B057");
                else
                    voice = FTBL.EV_PlayDialogue("0137B058");
                break;
            }
    }
    return voice;
}

void PrepareDartsOpponentAdmission(shen_name opponent)
{
    switch (opponent) {
    case DART_FCJ:
        if (FTBL.EV_GetPlayerFlag(11, 478))
            EVT_AT_SetVoice("0137D008", 0);
        else {
            EVT_AT_SetVoice("0137D001", 0);
            EVT_AT_SetVoice("0137A011", 50);
        }
        break;
    case DART_HCY:
        if (FTBL.EV_GetPlayerFlag(11, 479))
            EVT_AT_SetVoice("0137F008", 0);
        else {
            EVT_AT_SetVoice("0137F001", 0);
            EVT_AT_SetVoice("0137A011", 50);
        }
        break;
    case DART_HLN:
        if (FTBL.EV_GetPlayerFlag(11, 477))
            EVT_AT_SetVoice("0137C007", 0);
        else {
            EVT_AT_SetVoice("0137C001", 0);
            EVT_AT_SetVoice("0137A010", 50);
        }
        break;
    case DART_JON:
        if (FTBL.EV_GetPlayerFlag(11, 476))
            EVT_AT_SetVoice("0137B007", 0);
        else {
            EVT_AT_SetVoice("0137B001", 0);
            EVT_AT_SetVoice("0137B002", 120);
        }
        break;
    default:
        if (FTBL.EV_GetPlayerFlag(11, 475)) {
            EVT_AT_SetVoice("0137E010", 0);
            EVT_AT_SetVoice("0137E011", 150);
        } else {
            EVT_AT_SetVoice("0137E052", 0);
            EVT_AT_SetVoice("0137E053", 180);
        }
        break;
    }
}

int PlayDartsOpponentAdmission(shen_name opponent)
{
    int frames;
    switch (opponent) {
    case DART_FCJ:
        if (FTBL.EV_GetPlayerFlag(11, 478)) {
            frames = 120;
            EVT_AT_SetVoice("0137D009", 0);
        } else {
            frames = 540;
            EVT_AT_SetVoice("0137D003", 0);
            EVT_AT_SetVoice("0137D004", 100);
            EVT_AT_SetVoice("0137D005", 160);
            EVT_AT_SetVoice("0137D006", 270);
            EVT_AT_SetVoice("0137D007", 450);
        }
        break;
    case DART_HCY:
        if (FTBL.EV_GetPlayerFlag(11, 479)) {
            frames = 160;
            EVT_AT_SetVoice("0137F009", 0);
        } else {
            frames = 890;
            EVT_AT_SetVoice("0137F002", 0);
            EVT_AT_SetVoice("0137F003", 130);
            EVT_AT_SetVoice("0137F004", 270);
            EVT_AT_SetVoice("0137F005", 400);
            EVT_AT_SetVoice("0137F006", 530);
            EVT_AT_SetVoice("0137F007", 700);
        }
        break;
    case DART_HLN:
        if (FTBL.EV_GetPlayerFlag(11, 477)) {
            frames = 60;
            EVT_AT_SetVoice("0137C008", 0);
        } else {
            frames = 450;
            EVT_AT_SetVoice("0137C002", 0);
            EVT_AT_SetVoice("0137C003", 100);
            EVT_AT_SetVoice("0137C004", 170);
            EVT_AT_SetVoice("0137C005", 260);
            EVT_AT_SetVoice("0137C006", 390);
        }
        break;
    case DART_JON:
        if (FTBL.EV_GetPlayerFlag(11, 476)) {
            if (FTBL.EV_GetPlayerFlag(11, 502)) {
                frames = 230;
                EVT_AT_SetVoice("0137B003", 0);
                EVT_AT_SetVoice("0137B006", 110);
            } else {
                frames = 230;
                EVT_AT_SetVoice("0137B003", 0);
                EVT_AT_SetVoice("0137B006", 110);
            }
        } else {
            frames = 450;
            EVT_AT_SetVoice("0137B003", 0);
            EVT_AT_SetVoice("0137B004", 110);
            EVT_AT_SetVoice("0137B005", 210);
            EVT_AT_SetVoice("0137B006", 340);
        }
        break;
    default:
        if (FTBL.EV_GetPlayerFlag(11, 475)) {
            frames = 70;
            EVT_AT_SetVoice("0137E012", 0);
        } else {
            frames = 390;
            EVT_AT_SetVoice("0137E054", 0);
            EVT_AT_SetVoice("0137A017", 60);
            EVT_AT_SetVoice("0137E055", 90);
            EVT_AT_SetVoice("0137E007", 130);
            EVT_AT_SetVoice("0137E008", 190);
            EVT_AT_SetVoice("0137E009", 320);
        }
        break;
    }
    return frames;
}

int PlayDartsBetResponse(shen_name opponent, int difficulty, int bet)
{
    int frames;
    switch (opponent) {
    case DART_FCJ:
        frames = 120;
        EVT_AT_SetVoice("0137A014", 0);
        EVT_AT_SetVoice("0137D013", 30);
        EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 50);
        break;
    case DART_HCY:
        frames = 160;
        EVT_AT_SetVoice("0137A016", 0);
        EVT_AT_SetVoice("0137F015", 100);
        EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 100);
        break;
    case DART_HLN:
        frames = 120;
        EVT_AT_SetVoice("0137A003", 0);
        EVT_AT_SetVoice("0137C010", 30);
        EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 40);
        break;
    case DART_JON:
        if (difficulty) {
            frames = 130;
            switch (bet) {
            case 10:
                EVT_AT_SetVoice("0137A004", 0);
                EVT_AT_SetVoice("0137B016", 50);
                break;
            case 20:
                EVT_AT_SetVoice("0137A005", 0);
                EVT_AT_SetVoice("0137B016", 50);
                break;
            case 30:
                EVT_AT_SetVoice("0137A006", 0);
                EVT_AT_SetVoice("0137B016", 50);
                break;
            case 100:
                EVT_AT_SetVoice("0137A007", 0);
                EVT_AT_SetVoice("0137B017", 50);
                break;
            case 200:
                EVT_AT_SetVoice("0137A008", 0);
                EVT_AT_SetVoice("0137B017", 50);
                break;
            case 300:
                EVT_AT_SetVoice("0137A009", 0);
                EVT_AT_SetVoice("0137B017", 50);
                break;
            }
        } else {
            frames = 130;
            EVT_AT_SetVoice("0137A003", 0);
            EVT_AT_SetVoice("0137B016", 50);
        }
        EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 50);
        break;
    default:
        frames = 130;
        EVT_AT_SetVoice("0137A003", 0);
        EVT_AT_SetVoice("0137E015", 30);
        EVT_AT_SetFaceLooks(opponent, 3, 30, 1.0f, 40);
        break;
    }
    return frames;
}

int PlayDartsOpponentResult(shen_name opponent, int bet)
{
    int frames;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        EVT_AT_SetVoice("0137A013", 0);
        switch (bet) {
        case 10:
            frames = -120;
            EVT_AT_SetVoice("0137D011", 60);
            break;
        case 5:
            frames = -140;
            EVT_AT_SetVoice("0137D012", 60);
            break;
        default:
            frames = 170;
            EVT_AT_SetVoice("0137D010", 60);
            EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 80);
            break;
        }
        break;
    case DART_HCY:
        frames = 200;
        EVT_AT_SetVoice("0137A012", 0);
        EVT_AT_SetVoice("0137F010", 80);
        break;
    case DART_HLN:
        frames = 120;
        EVT_AT_SetVoice("0137A002", 0);
        EVT_AT_SetVoice("0137C009", 50);
        break;
    case DART_JON:
        frames = 160;
        EVT_AT_SetVoice("0137A002", 0);
        if (random < 1.0f)
            EVT_AT_SetVoice("0137B010", 50);
        else if (random < 1.5f)
            EVT_AT_SetVoice("0137B011", 50);
        else
            EVT_AT_SetVoice("0137B012", 50);
        if (random < 1.0f)
            EVT_AT_SetVoice("0137B013", 100);
        else if (random < 1.5f)
            EVT_AT_SetVoice("0137B014", 100);
        else
            EVT_AT_SetVoice("0137B015", 100);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 50);
        break;
    default:
        frames = 140;
        EVT_AT_SetVoice("0137A002", 0);
        EVT_AT_SetVoice("0137E014", 50);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 50);
        break;
    }
    return frames;
}

int PlayDartsOpponentExit(shen_name opponent)
{
    int frames;
    float random = FTBL.GetRandomNumberInRange(3.0f);
    switch (opponent) {
    case DART_FCJ:
        frames = 190;
        EVT_AT_SetVoice("0137A012", 0);
        EVT_AT_SetVoice("0137D010", 80);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 80);
        break;
    case DART_HCY:
        frames = 400;
        EVT_AT_SetVoice("0137A012", 0);
        EVT_AT_SetVoice("0137F010", 80);
        if (random < 1.0f)
            EVT_AT_SetVoice("0137F011", 210);
        else if (random < 1.5f)
            EVT_AT_SetVoice("0137F012", 210);
        else if (random < 2.5f)
            EVT_AT_SetVoice("0137F013", 210);
        else
            EVT_AT_SetVoice("0137F014", 210);
        break;
    case DART_HLN:
        frames = 120;
        EVT_AT_SetVoice("0137A001", 0);
        EVT_AT_SetVoice("0137C009", 60);
        break;
    case DART_JON:
        frames = 160;
        EVT_AT_SetVoice("0137A001", 0);
        if (random < 1.0f)
            EVT_AT_SetVoice("0137B010", 50);
        else if (random < 1.5f)
            EVT_AT_SetVoice("0137B011", 50);
        else
            EVT_AT_SetVoice("0137B012", 50);
        if (random < 1.0f)
            EVT_AT_SetVoice("0137B013", 100);
        else if (random < 1.5f)
            EVT_AT_SetVoice("0137B014", 100);
        else
            EVT_AT_SetVoice("0137B015", 100);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 50);
        break;
    default:
        frames = 150;
        EVT_AT_SetVoice("0137A001", 0);
        EVT_AT_SetVoice("0137E013", 60);
        EVT_AT_SetFaceLooks(opponent, 5, 30, 1.0f, 80);
        break;
    }
    return frames;
}

void DartsPlayOpponentChatter(shen_name opponent)
{
    int index = FTBL.shenGetRandomInt32(3);
    switch (opponent) {
    case DART_FCJ:
        QueueEventVoiceList(0, dartsFcjChatter[index], 200, 0, 0);
        break;
    case DART_HCY:
        QueueEventVoiceList(0, dartsHcyChatter[index], 200, 0, 0);
        break;
    case DART_HLN:
        QueueEventVoiceList(0, dartsHlnChatter[index], 200, 0, 0);
        break;
    case DART_JON:
        QueueEventVoiceList(0, dartsJonChatter[index], 200, 0, 0);
        break;
    default:
        QueueEventVoiceList(0, dartsXhoChatter[index], 200, 0, 0);
        break;
    }
}

DartsOpponentInfo dartsOpponents[6] = {
    {DART_FCJ, 1.45f, 14, {6786, 6785, 199, 221, 6786, 30, 6790, 6787, 6846, 199, 6789, 6788, 6825, 6826, 390, 470}, {70.0f, 40.0f}, 1.2f, 4.2f, 6827, 430, 70.0f, 1.3f, 4.2f, 6824, 0},
    {DART_HCY, 1.54f, 14, {6798, 6797, 245, 245, 6798, 60, 6802, 6799, 6847, 135, 6801, 6800, 6829, 6830, 300, 250}, {0.0f, 0.0f}, 1.2f, 4.8f, 6831, 340, 0.0f, 1.3f, 4.7f, 6828, 0},
    {DART_HLN, 1.25f, 14, {6841, 6840, 279, 242, 6841, 50, 6845, 6842, 6850, 157, 6844, 6843, 6822, 6822, 200, 200}, {0.0f, 0.0f}, 0.9f, 4.3f, 6823, 250, 0.0f, 0.9f, 4.3f, 6842, 0},
    {DART_JON, 1.52f, 21, {6810, 6809, 145, 215, 6810, 70, 6814, 6811, 6848, 144, 6813, 6812, 6833, 6834, 245, 240}, {0.0f, 0.0f}, 1.3f, 4.6f, 6835, 470, 100.0f, 1.0f, 4.4f, 6832, 0},
    {DART_XHO, 1.53f, 14, {6816, 6815, 180, 150, 6816, 60, 6820, 6817, 6849, 165, 6819, 6818, 6837, 6838, 300, 245}, {0.0f, 0.0f}, 1.3f, 4.6f, 6839, 380, 0.0f, 1.4f, 4.5f, 6836, 0},
    {0, 1.5f, 14, {6816, 6815, 180, 150, 6816, 60, 6820, 6817, 6849, 165, 6819, 6818, 6837, 6838, 300, 245}, {0.0f, 0.0f}, 1.0f, 4.4f, 6839, 380, 0.0f, 1.0f, 4.4f, 6836, 0}
};
Vector3i dartsHandRotation = {64863, 3458, 64361};
int dartsHandPoseCommand[19][3] = {{0, 64444, 63534}, {0, 0, 62624}, {0, 63716, 2002}, {0, 64444, 55890}, {0, 0, 56982}, {0, 65172, 61350}, {0, 0, 1456}, {0, 364, 56800}, {0, 64627, 57528}, {0, 0, 61531}, {0, 65354, 2002}, {0, 65354, 60986}, {0, 64444, 56982}, {0, 0, 63898}, {0, 65354, 2730}, {0, 64809, 60075}, {0, 64809, 61168}, {0, 65172, 62442}, {0, 364, 4368}};
Vector3 dartsReadyArrowOffsets[5] = {{0.158f, 0.009f, 0.004f}, {0.145f, 0.01f, -0.002f}, {0.128f, 0.017f, -0.007f}, {0.135f, 0.008f, 0.0f}, {0.15f, 0.009f, 0.004f}};
float dartsAiBands[5] = {0.5f, 0.6f, 0.8f, 0.9f, 1.0f};
Vector3i dartsGameArrowRotation = {18655, 31391, 49425};
Vector3 dartsJonGameArrowOffset = {0.14f, -0.06f, 0.04f};
Vector3 dartsGameArrowOffsets[5] = {{0.12f, -0.09f, -0.01f}, {0.14f, -0.06f, -0.02f}, {0.105f, -0.07f, -0.01f}, {0.14f, -0.06f, -0.02f}, {0.14f, -0.06f, -0.02f}};
const char *dartsFcjChatter[4][4] = {{"0137D044", "0137D045", "0137D046", 0}, {"0137D047", "0137D048", "0137D049", 0}, {"0137D050", "0137D051", "0137D046", 0}, {"0137D052", "0137D053", "0137D049", 0}};
const char *dartsHcyChatter[3][2] = {{"0137F053", 0}, {"0137F054", 0}, {"0137F047", 0}};
const char *dartsHlnChatter[3][2] = {{"0137C040", 0}, {"0137C041", 0}, {"0137C042", 0}};
const char *dartsJonChatter[3][2] = {{"0137B040", 0}, {"0137B041", 0}, {"0137B042", 0}};
const char *dartsXhoChatter[4][4] = {{"0137E040", 0, 0, 0}, {"0137E041", "0137E042", 0, 0}, {"0137E043", "0137E044", "0137E045", 0}, {"0137E046", 0, 0, 0}};
#endif
#endif
