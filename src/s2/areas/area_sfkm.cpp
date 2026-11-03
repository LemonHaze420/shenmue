#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_sfkm.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void SfkmMain(StreetFightStartParameters *start)
{
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
#else
static const shen_name sfkmCharacters[] = {STREET_FIGHT_RYO, SFKM_OD2, SFKM_AW1, SFKM_CHN, STREET_FIGHT_INVALID};
static const Vector3 sfkmPlayerPosition = {-3.3f, -0.5f, -5.5f};
static const Vector3 sfkmBookiePosition = {-1.564f, 0.0f, -1.1f};
static const int sfkmEntryFees[] = {10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static Vector3 sfkmExitPosition = {264.81f, 94.83f, 48.99f};
static Vector3i sfkmExitRotation = {0, 63575, 0};
static int sfkmFirstDialogue;
static int sfkmRepeatDialogue;
static int sfkmLastDialogue;
static int sfkmFirstAttempt;

void QueueNormal(StreetFightStartParameters *start)
{
    HLibTask *task;
    StreetFightEventWork *work;
    if (start->signal)
        *start->signal = 1;
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(MainCallback, 5, 11, sizeof(StreetFightEventWork), TASK_EVNT);
    work = (StreetFightEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->startParameters = start;
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = FTBL.GetNpcBatchCountOverride(50);
    work->fighterIndex = 0;
    work->fighters[0] = SFKM_AW1;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->area = SFKM_PACKAGE;
    work->battleFrames = 0;
    work->rankHistory[0] = 0;
    work->rankHistory[1] = 0;
    work->rankHistory[2] = 0;
    work->audienceTask = 0;
    work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForArea(g_currentMap);
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
    work->returnToQron = 0;
    if (!FTBL.EV_GetPlayerFlag(11, 1787))
        sfkmFirstAttempt = 1;
    else
        sfkmFirstAttempt = 0;
    if (FTBL.EV_GetShenmueFlag(255))
        FTBL.SetNewNpcBatchCountOverride(2, 12);
    else
        FTBL.SetNewNpcBatchCountOverride(2, 15);
    EventDebugRegister(3, NormalDebugCallback, work, 36000);
}

void PlaceNormal(StreetFightEventWork *work)
{
    shen_name fighter = work->fighters[work->fighterIndex];
    SetStreetFightCharacterVisible(SFKM_OD2, 1);
    SetStreetFightCharacterVisible(fighter, 1);
    work->playerPosition.x = sfkmPlayerPosition.x;
    work->playerPosition.y = sfkmPlayerPosition.y;
    work->playerPosition.z = sfkmPlayerPosition.z;
    work->bookiePosition.x = sfkmBookiePosition.x;
    work->bookiePosition.y = sfkmBookiePosition.y;
    work->bookiePosition.z = sfkmBookiePosition.z;
    EVT_AT_SetPos(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 268.2f, 95.33f, 42.07f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 180.0f, 0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(SFKM_OD2, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(fighter, STREET_FIGHT_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, -9.2f, -0.5f, -10.4f, 0);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -1.288f, 0.0f, -2.2f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 17.0f, 0);
    FTBL.SetScenarioCharacterType(STREET_FIGHT_RYO, 4);
    FTBL.SetScenarioCharacterType(SFKM_OD2, 4);
    FTBL.SetScenarioCharacterType(fighter, 4);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(SFKM_OD2), 0);
    FTBL.SetFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFKM_OD2), 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(fighter), 1);
    FTBL.adfsSetCameraIdxForChara(SFKM_OD2, SFKM_BOOKIE_CAMERA);
    FTBL.adfsSetCameraIdxForChara(fighter, SFKM_FIGHTER_CAMERA);
    FTBL.LoadBlurModelForCharacter_0(SFKM_AW1, 0, 0.2f);
    FTBL.EV_SetPlayerFlag(12, 1292, 0);
}

void NormalDebugCallback(void *data)
{
    CleanupNormalCallback(data);
}

void CleanupNormalCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    StreetFightStartParameters *start = work->startParameters;
    int i;
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
    EventDebugUnregister(3);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(SFKM_AW1));
    ReleaseStreetFightAudience(work->audienceTask);
    work->audienceTask = 0;
    ET_AT_KillWork(work->characterReadTask);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioCharacterKillMode(STREET_FIGHT_BASE, 1);
    FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &sfkmExitPosition);
    FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &sfkmExitRotation, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
    FTBL.GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (i = 0; work->motions[i]; i++)
        FTBL.DeleteMotion((void *)work->motions[i]);
    FTBL.EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1))
        FTBL.EV_ControlPackageResources(0, 6, "QSFC", 0, 0);
    if (!IsStreetFightBestOfThree())
        FTBL.LoadCutsceneAndSoundProgramForArea();
    else
        FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (IsStreetFightBestOfThree())
        FTBL.EV_scenQueueLoad(4, SFKM_OMAKE_SCENE, 1);
    else if (work->returnToQron)
        FTBL.EV_scenQueueLoad(g_actNumber, SFKM_QRON_SCENE, 81);
    else {
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
    }
    if (FTBL.EV_GetShenmueFlag(255))
        FTBL.SetNewNpcBatchCountOverride(1, 10);
    else
        FTBL.SetNewNpcBatchCountOverride(1, 15);
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}

void MainCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 centre, position, second, player, bookie;
    shen_name fighter = work->fighters[work->fighterIndex];
    unsigned int choice;
    EventDebugCheck(3, 4);
    switch (work->state) {
    case 0:
        FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
        if (!FTBL.EV_ControlPackageResources(0, 5, "QSFC", 0, 0)) {
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QSFC", 0);
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        }
        FTBL.EV_FreeAuthorPak(1, "0100");
        work->characterReadTask = Queue_EVT_AT_ReadChr(work->task, sfkmCharacters);
        FTBL.EV_ControlPackageResources(2, 4, "QSFC", 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "QSFC", 0, 0);
        FTBL.CreateEventCharacter(3, 0, STREET_FIGHT_BASE);
        FTBL.EV_SetNewSceneIndex();
        FTBL.EV_LoadCutsceneFromCurrentScene("0100E000");
        FTBL.EV_LoadAudio("0100E000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.FinishLoading();
        FTBL.SetRenderClockAndCompassTo(1);
        if (!work->audienceTask) {
            centre.x = 268.2f;
            centre.y = 95.33f;
            centre.z = 42.07f;
            work->audienceTask = QueueStreetFightAudience(30, 0);
            AddStreetFightAudienceFighter(work->audienceTask, STREET_FIGHT_RYO);
            AddStreetFightAudienceFighter(work->audienceTask, fighter);
            SetStreetFightAudienceCentre(work->audienceTask, &centre);
        }
        work->state++;
        break;
    case 1:
        PlaceNormal(work);
        if (!work->fighterIndex)
            work->state = 2;
        else
            work->state = 3;
        FTBL.GetCharaPosition(STREET_FIGHT_BASE, -1, &position, 0);
        position.y -= 0.5f;
        SetStreetFightAudienceCentre(work->audienceTask, &position);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, -1, &player, 0);
        FTBL.GetCharaPosition(SFKM_OD2, -1, &bookie, 0);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &work->playerPosition);
        FTBL.EV_AT_SetPos(SFKM_OD2, 0x38000000, &work->bookiePosition);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, -1, &position, 0x40000000);
        FTBL.GetCharaPosition(SFKM_OD2, -1, &second, 0x40000000);
        SetStreetFightAudienceCorridor(work->audienceTask, &position, &second);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &player);
        FTBL.EV_AT_SetPos(SFKM_OD2, 0x38000000, &bookie);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SFKM_AW1));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFKM_AW1), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFKM_AW1), 1, 0, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        QueueStreetFightNpcWait(work->startParameters->flags);
        break;
    case 2:
        if (IsStreetFightAudienceArranged(work->audienceTask)) {
            if (!FTBL.EV_GetPlayerFlag(11, 740))
                QueueStreetFightSkipWait(FirstIntroConversation, work, 1);
            else
                QueueStreetFightSkipWait(RepeatIntroConversation, work, 1);
            work->state = 4;
        }
        break;
    case 4:
        FTBL.EV_SetPlayerFlag(12, 740, 1);
        EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 470, 0x7fff, 0xffff, 2, 0);
        FTBL.CheckHelpIcon(1, 0, 1);
        work->paidEntry = 0;
        work->state = 5;
        break;
    case 5:
        FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
        ReadStreetFightCharacterPosition(STREET_FIGHT_RYO, SFKM_OD2, 16);
        choice = PollStreetFightActionChoice();
        if (choice) {
            FTBL.CheckHelpIcon(1, 1, 2);
            ClearStreetFightLook(STREET_FIGHT_RYO, 15);
            switch (choice) {
            case 38:
                if (FTBL.GetPlayerStat(2) < sfkmEntryFees[work->rank])
                    work->state = 9;
                else {
                    work->state = 7;
                    work->paidEntry = 1;
                }
                break;
            case 470:
                FTBL.vfmDispatchControl(21, 0, 0, 1);
                work->state = 6;
                break;
            default:
                work->state = 8;
                break;
            }
        }
        break;
    case 6:
        if (!FTBL.vfmDispatchControl(21, 0, 0, -1))
            work->state = 4;
        break;
    case 7:
        QueueStreetFightSkipWait(AcceptConversation, work, 1);
        work->state = 10;
        break;
    case 8:
        QueueStreetFightSkipWait(DeclineConversation, work, 1);
        work->state = 20;
        break;
    case 9:
        QueueStreetFightSkipWait(InsufficientConversation, work, 1);
        work->state = 20;
        break;
    case 10:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        work->state = 11;
        break;
    case 11:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.HLib_SetCurrentTaskTo(BattleEntryCallback);
            work->state = 12;
        }
        break;
    case 12:
        work->rankHistory[work->fighterIndex] = 1;
        if (work->battleStatus == 3)
            work->state = 13;
        else if (work->battleStatus == 2)
            work->state = 18;
        else
            work->state = 19;
        break;
    case 13:
        QueueStreetFightSkipWait(VictoryConversation, work, 1);
        if (!CanPerformForAreaFlags(g_currentMap))
            work->state = 20;
        else
            switch (CommitStreetFightStoryRank(work->stage)) {
            case 1:
                work->state = 14;
                break;
            case 2:
                work->state = 14;
                break;
            default:
                work->state = 20;
                break;
            }
        break;
    case 14:
        QueueStreetFightSkipWait(PostVictoryConversation, work, 1);
        if (FTBL.GetStreetFightProgress() > 9)
            work->state = 20;
        else
            work->state = 20;
        break;
    case 18:
        ResetStreetFightStoryFlag(work->stage);
        QueueStreetFightSkipWait(DefeatConversation, work, 1);
        work->state = 20;
        break;
    case 19:
        QueueStreetFightSkipWait(TimeoutConversation, work, 1);
        work->state = 20;
        break;
    case 20:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        work->state = 21;
        break;
    case 21:
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (!IsStreetFightBestOfThree() && work->paidEntry && !FTBL.vfmDispatchControl(14, 67, 0, -1) && FTBL.vfmDispatchControl(14, 131, 0, -1) == 2)
                FTBL.QueueFightAdviceControl(8);
            work->state = 22;
        }
        break;
    case 22:
        if (work->battleStatus && !IsStreetFightBestOfThree() && work->battleStatus == 3)
            FTBL.EV_SetPlayerFlag(12, 741, 1);
        if (IsStreetFightBestOfThree() && work->paidEntry)
            FTBL.HLib_SetCurrentTaskTo(RetryBeginCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
        break;
    }
}

void RetryBeginCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    FTBL.EV_SetPlayerFlag(12, 740, 1);
    FTBL.dbgPrintEventMessage("「リトライしますか？\n");
    EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 0xffff, 0x7fff, 0xffff, 2, 0);
    ChangeStreetFightAudienceMode(work->audienceTask, 2, 1);
    FTBL.HLib_SetCurrentTaskTo(RetryChoiceCallback);
}

void RetryChoiceCallback(void *data)
{
    unsigned int choice = PollStreetFightActionChoice();
    if (choice) {
        FTBL.ctrlStopSubtitles();
        if (choice == 38)
            RetryResetCallback(data);
        else
            RetryCancelCallback(data);
    }
}

void RetryResetCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    FTBL.EV_ResetFreeTalkFlags();
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
    ChangeStreetFightAudienceMode(work->audienceTask, 2, -1);
    FTBL.EV_LoadAudio("0100E000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    work->state = 1;
    work->battleStatus = 0;
    work->auxiliaryState = 0;
    FTBL.HLib_SetCurrentTaskTo(MainCallback);
}

void RetryCancelCallback(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
}

int FirstIntroConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 199);
    ReadStreetFightCharacterPosition(STREET_FIGHT_RYO, SFKM_OD2, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 2, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 2, 16);
    FTBL.EV_SetCharacterAlphaEnabled(SFKM_OD2, 0);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 40.0f, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0.0f, 1.1f, 0x40000000, 0);
    EVT_AT_SetMot(SFKM_OD2, 6853, 0.0f, 1.1f, 0, 545);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -0.2f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, -15.0f, 0);
    EVT_AT_SetMot(fighter, 6852, 0.0f, 1.2f, 0x40000000, 0);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(fighter), 0, 2, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(fighter), 1, 2, 16);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 1, (void *)areaLoopHandPoses[SFKM_INTRO_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 0, (void *)areaLoopHandPoses[SFKM_INTRO_HAND_POSE], 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    EVT_AT_SetVoice("0100C001", 10);
    EVT_AT_SetVoice("0100C002", 90);
    EVT_AT_SetVoice("0100C003", 220);
    EVT_AT_SetVoice("0100C004", 400);
    switch (sfkmEntryFees[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100C008", 540);
        break;
    case 20:
        EVT_AT_SetVoice("0100C009", 540);
        break;
    case 30:
        EVT_AT_SetVoice("0100C010", 540);
        break;
    case 40:
        EVT_AT_SetVoice("0100C011", 540);
        break;
    default:
        EVT_AT_SetVoice("0100C005", 540);
        break;
    }
    EVT_AT_SetVoice("0100C021", 620);
    EVT_AT_SetVoice("0100C007", 690);
    EVT_AT_SetCam(100, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(8100, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 85);
    EVT_AT_SetCam(2309, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 220);
    EVT_AT_SetCam(2302, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 545);
    EVT_AT_SetCam(2317, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 735);
    return 880;
}

int RepeatIntroConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 398, 0.0f, 1.0f, 0x40000000, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFKM_OD2, 16, 1, 0, 4352);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, -3.29f, -0.5f, -5.47f, 0);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 2, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 2, 16);
    FTBL.EV_SetCharacterAlphaEnabled(SFKM_OD2, 0);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 40.0f, 0);
    EVT_AT_SetMot(SFKM_OD2, 6853, 0.0f, 1.1f, 0x40000000, 0);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -0.2f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, -15.0f, 0);
    EVT_AT_SetMot(fighter, 6852, 0.0f, 1.2f, 0x40000000, 0);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(fighter), 0, 2, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(fighter), 1, 2, 16);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 1, (void *)areaLoopHandPoses[SFKM_INTRO_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 0, (void *)areaLoopHandPoses[SFKM_INTRO_HAND_POSE], 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    switch (sfkmEntryFees[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100C008", 0);
        break;
    case 20:
        EVT_AT_SetVoice("0100C009", 0);
        break;
    case 30:
        EVT_AT_SetVoice("0100C010", 0);
        break;
    case 40:
        EVT_AT_SetVoice("0100C011", 0);
        break;
    default:
        EVT_AT_SetVoice("0100C005", 0);
        break;
    }
    EVT_AT_SetVoice("0100C007", 80);
    EVT_AT_SetCam(2317, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 1);
    return 270;
}

int AcceptConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    int frames;
    shen_name fighter = work->fighters[work->fighterIndex];
    unsigned char rank;
    frames = 200;
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6856, 0.0f, 1.0f, 0x40000000, 0);
    ReadStreetFightCharacterPosition(SFKM_OD2, STREET_FIGHT_RYO, 16);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 20.0f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 40.0f, 155);
    EVT_AT_SetMot(SFKM_OD2, 6854, 0.0f, 1.1f, 0x40000000, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -1.2f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 30.0f, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 1.0f, 0x50000000, 155);
    EVT_AT_SetCam(2304, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(2316, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 155);
    EVT_AT_SetVoice("0100A022", 60);
    StartStreetFightAudienceOpening(work->audienceTask, 100);
    rank = work->rank;
    if (!FTBL.EV_GetPlayerFlag(11, 1787)) {
        EVT_AT_SetVoice("0100C012", 100);
        if (!work->rankHistory[rank]) {
            frames = 310;
            EVT_AT_SetVoice("0100R001", 200);
        }
    } else {
        EVT_AT_SetMot(SFKM_OD2, 6854, 0.0f, 1.1f, 0, 155);
        EVT_AT_SetVoice("0100C015", 100);
        frames = 230;
        if (!FTBL.vfmDispatchControl(14, 131, 0, -1)) {
            frames = 310;
            EVT_AT_SetVoice("0100R004", 230);
        }
    }
    return frames;
}

int DeclineConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
    ReadStreetFightCharacterPosition(SFKM_OD2, STREET_FIGHT_RYO, 16);
    switch (FTBL.shenGetRandomInt32(3)) {
    case 0:
        EVT_AT_SetVoice("0100A038", 0);
        break;
    case 1:
        EVT_AT_SetVoice("0100A039", 0);
        break;
    case 2:
        EVT_AT_SetVoice("0100A040", 0);
        break;
    }
    EVT_AT_SetCam(2315, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    return 60;
}

int InsufficientConversation(void *data)
{
    EVT_AT_StartMoneyCount(0, 60);
    EVT_AT_SetVoice("0100A037", 0);
    EVT_AT_SetCam(2315, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    return 150;
}

void BattleEntryCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 0.0f, 0.0f, -1.1f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, -3.3f, -0.5f, -5.5f, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, 0.0f, 0.0f, 1.1f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 0.0f, 0);
    FTBL.HLib_SetCurrentTaskTo(BattlePreEventCallback);
}

void BattlePreEventCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.EV_LoadAudio("0100E000", 1);
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.vfmDispatchControl(2, 0, "QSFC", 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(fighter), 0, 1, 1, work->rank * 200 + 1000, 31);
    if (work->fighterIndex)
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(fighter), 0, 0x8000000, 4);
    FTBL.EV_vfBattleUtil(3, 1, 1);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_SetPlayerFlag(12, 1292, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(fighter, 3156, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetVoice("0100C013", 60);
    EVT_AT_SetVoice("0100C014", 140);
    EVT_AT_SetCam(2400, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(BattlePreInitCallback);
    FTBL.HLib_SetupTaskDelay(170);
}

void BattlePreInitCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 position;
    Vector3i rotation;
    if (FTBL.cvsHasValidConverseData(0) == 1)
        return;
    FTBL.GetCharaPosition(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, 0x40000000);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &rotation, 0);
    FTBL.GetScenarioCharacterEulerAngles(work->fighters[work->fighterIndex], &rotation, 0x40000000);
    FTBL.GetCharaPosition(work->fighters[work->fighterIndex], STREET_FIGHT_INVALID, &position, 0x40000000);
    FTBL.AT_SetLocalBaseChara(work->fighters[work->fighterIndex], STREET_FIGHT_INVALID);
    FTBL.EV_AT_SetPos(work->fighters[work->fighterIndex], 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(work->fighters[work->fighterIndex], 0x78000000, &rotation, 0);
    FTBL.HLib_SetCurrentTaskTo(BattleStartCallback);
    FTBL.HLib_SetupTaskDelay(10);
}

void BattleStartCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    int id;
    FTBL.vfmDispatchControl(16, 0, (const void *)(FTBL.vfmDispatchControl(14, 67, 0, -1) + 1), 1);
    FTBL.vfCopyPosFromEntity(0, 0, 268.2f, 42.07f, 2.5f);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(fighter), 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFKM_OD2), 0);
    FTBL.SetScenarioCharacterType(STREET_FIGHT_RYO, 0);
    FTBL.SetScenarioCharacterType(fighter, 0);
    FTBL.EV_vfBattleUtil(3, 0x800, 1);
    FTBL.EV_vfBattleUtil(3, 0x80000, 1);
    FTBL.vfmQueueFightStart(2, 99, 1.0f, 0, 60);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    if (work->rank > 0)
        FTBL.vfmDispatchControl(6, 0, (const void *)3, 1);
    FTBL.EV_SetPlayerFlag(12, 1787, 1);
    SetStreetFightStageFlag(work->stage);
    sfkmFirstDialogue = 0;
    sfkmRepeatDialogue = 0;
    sfkmLastDialogue = 0;
    if (!work->rank) {
        sfkmFirstDialogue = FTBL.AllocateVCDS_6(20, 157, fighter, STREET_FIGHT_RYO, 100.0f, 300, 0);
        sfkmLastDialogue = FTBL.AllocateVCDS_6(20, 159, fighter, STREET_FIGHT_RYO, 100.0f, 720, 0);
    } else
        sfkmRepeatDialogue = FTBL.AllocateVCDS_6(20, 159, fighter, STREET_FIGHT_RYO, 100.0f, 150, 0);
    id = FTBL.AllocateVCDS_2(30, -1, work->fighters[work->fighterIndex], 0, 1, 60.0f, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, (int (*)(void *))SelectFirstBattleVoice);
    id = FTBL.AllocateVCDS_12(30, -1, work->fighters[work->fighterIndex], 0, 1, 100.0f, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, (int (*)(void *))SelectSecondBattleVoice);
    FTBL.HLib_SetCurrentTaskTo(BattleWaitCallback);
}

short SelectFirstBattleVoice(void *data)
{
    unsigned short voices[4] = {165, 166, 167, 178};
    unsigned short *index = (unsigned short *)data;
    unsigned short voice = voices[*index];
    if (++*index > 3)
        *index = 0;
    return voice;
}

short SelectSecondBattleVoice(void *data)
{
    unsigned short voices[2] = {169, 170};
    unsigned short *index = (unsigned short *)data;
    unsigned short voice = voices[*index];
    if (++*index > 1)
        *index = 0;
    return voice;
}

void BattleWaitCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    unsigned int flags;
    if (FTBL.IsFightActive()) {
        flags = FTBL.GetFightFrame();
        if (flags & 2)
            QueueStreetFightPlayerReaction(fighter, 0);
        if (flags & 4)
            QueueStreetFightPlayerReaction(fighter, 1);
        if (!FTBL.btlGetCharaFlag(0, FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0.0f) || !FTBL.btlGetCharaFlag(0, FTBL.ctrlGetCharaByName(fighter), 0.0f)) {
            FTBL.SetFlagForTaskData();
            FTBL.SetCameraDialogueTransition(-32768, -1);
            sfkmFirstDialogue = 0;
            sfkmRepeatDialogue = 0;
            sfkmLastDialogue = 0;
        }
        if (sfkmFirstDialogue && FTBL.GetCameraDialogueFlags(sfkmFirstDialogue)) {
            FTBL.vfmDispatchControl(6, 0, (const void *)3, 1);
            FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
            sfkmFirstDialogue = 0;
        }
        if (sfkmRepeatDialogue && FTBL.GetCameraDialogueFlags(sfkmRepeatDialogue)) {
            FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
            sfkmRepeatDialogue = 0;
        }
        if (sfkmLastDialogue && FTBL.GetCameraDialogueFlags(sfkmLastDialogue)) {
            FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
            sfkmLastDialogue = 0;
        }
        if (!work->audienceResultPublished) {
            switch (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO))) {
            case 5:
            case 6:
            case 7:
                FTBL.EV_PlayDialogue("0100F017");
                work->audienceResultPublished++;
                break;
            case 3:
                StartStreetFightAudienceDefeat(work->audienceTask, 0);
                work->audienceResultPublished++;
                break;
            case 2:
                StartStreetFightAudienceVictory(work->audienceTask, 0);
                work->audienceResultPublished++;
                break;
            }
        }
    } else {
        work->audienceResultPublished = 0;
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) == 3)
            work->battleStatus = 3;
        else if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) == 2)
            work->battleStatus = 2;
        else
            work->battleStatus = 7;
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        FTBL.HLib_SetCurrentTaskTo(BattleEndCallback);
        FTBL.HLib_SetupTaskDelay(30);
    }
    work->frameCounter = 0;
}

void BattleEndCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1.0f);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(fighter), 1, 1, 1.0f);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(fighter), 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFKM_OD2), 1);
    FTBL.EV_LoadAudio("0100E000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_SetCharacterAlphaEnabled(SFKM_OD2, 0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(work->fighters[work->fighterIndex], STREET_FIGHT_BASE);
    FTBL.EV_SetPlayerFlag(12, 1292, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFKM_AW1), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFKM_AW1), 1, 0, 0);
    if (work->battleStatus == 3)
        RecordStreetFightVictory(work->stage);
    else
        RecordStreetFightDefeat(work->stage);
    work->frameCounter = 0;
    FTBL.HLib_SetCurrentTaskTo(MainCallback);
}

int VictoryConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, SFKM_OD2, 6, 10, 245);
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 16, 1);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0.0f, 1.1f, 0x40000000, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -1.7f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 17.0f, 0);
    EVT_AT_SetMot(fighter, 6851, 0.0f, 1.2f, 0x50000000, 0);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 1, (void *)areaLoopHandPoses[SFKM_VICTORY_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 0, (void *)areaLoopHandPoses[SFKM_VICTORY_HAND_POSE], 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    if (!FTBL.EV_GetPlayerFlag(11, 741)) {
        EVT_AT_SetVoice("0100R002", 30);
        EVT_AT_SetVoice("0100C017", 170);
    } else {
        EVT_AT_SetVoice("0100R023", 30);
        EVT_AT_SetVoice("0100C017", 100);
    }
    EVT_AT_StartMoneyCount(sfkmEntryFees[work->rank], 260);
    EVT_AT_SetCam(2307, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(210, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 240);
    return 360;
}

int PostVictoryConversation(void *data)
{
    return PlayStreetFightVictoryAppearance();
}

int DefeatConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, SFKM_OD2, 6, 10, 167);
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 16, 1);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, -10.0f, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0.0f, 1.1f, 0x40000000, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -1.2f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 30.0f, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 1.2f, 0x40000000, 0);
    EVT_AT_SetMot(fighter, 6849, 0.0f, 1.2f, 0x40000000, 180);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 1, (void *)areaLoopHandPoses[SFKM_DEFEAT_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 0, (void *)areaLoopHandPoses[SFKM_DEFEAT_HAND_POSE], 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    EVT_AT_SetVoice("0100C018", 30);
    if (sfkmFirstAttempt)
        EVT_AT_SetVoice("0100R003", 180);
    else
        EVT_AT_SetVoice("0100R006", 180);
    EVT_AT_SetCam(2308, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(2318, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 180);
    EVT_AT_SetCam(310, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 300);
    EVT_AT_StartMoneyCount(-sfkmEntryFees[work->rank], 330);
    return 450;
}

int TimeoutConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, SFKM_OD2, 6, 10, 167);
    EVT_AT_SetPos(SFKM_OD2, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFKM_OD2, STREET_FIGHT_BASE, -10.0f, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0.0f, 1.1f, 0x40000000, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.288f, 0.0f, -1.2f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 30.0f, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 1.2f, 0x50000000, 0);
    EVT_AT_SetVoice("0100C019", 30);
    EVT_AT_SetVoice("0100C020", 110);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    EVT_AT_SetCam(2308, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    return 200;
}
#endif
#else
#include "s2/areas/area_sfkm.hpp"
#include "s2/author.hpp"
#include "s2/event_call.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/npc.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/sk_event.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include "s2/model_component.hpp"
#include "s2/battle_qte.hpp"
#include "s2/step_event.hpp"
#include "s2/advice.hpp"

namespace shenmue::s2 {
namespace {
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
void MainCallback(void *);
void ReadBookiePosition(shen_name character) {
    Vector3 position;
    EV_GetCharaPosition(character, 5, &position, EventTransformFlags::World);
}
void DebugBattleEntryCallback(void *);
void DebugBattlePreEventCallback(void *);
void DebugBattlePreInitCallback(void *);
void DebugBattleStartCallback(void *);
void DebugBattleReadyGoCallback(void *);
void DebugBattleStartFightCallback(void *);
void DebugBattleWaitCallback(void *);
void DebugBattleWaitFadeCallback(void *);
void DebugBattleEndCallback(void *);
std::int32_t DebugContinueConversation(void *);
std::int32_t DebugCompletedConversation(void *);
std::int32_t DebugStopConversation(void *);

std::int64_t QueueDebugCharacterLoad(HLib::Task *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(StreetFightCharacterLoadCallback, 6, 0x0b, sizeof(StreetFightCharacterLoadWork), STREET_FIGHT_EVENT_TASK);
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(parent, 0);
    work->task = task;
    work->characters = debugLoadCharacters;
    work->parent = parent;
    work->index = 0;
    work->state = 0;
    return 0;
}

StreetFightEventWork *InitializeDebug(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = STREET_FIGHT_INVALID;
    work->package = SFKM_DEBUG_PACKAGE;
    work->battleFrames = 50;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->audienceTask = nullptr;
    work->completedOpponents = 0;
    work->hudEnabled = 0;
    work->fighterIndex = 0;
    const StreetFightFighterDefinition *const definition = GetStreetFightDebugFighterDefinition();
    debugOpponentCount = 5;
    work->fighterDefinitions[0] = definition;
    work->fighters[0] = definition->character;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFKM_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFC"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFKM_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}

void QueueDebug(StreetFightStartParameters *parameters) {
    if (parameters->signal)
        *parameters->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DebugMainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeDebug(task);
    work->startParameters = parameters;
    LoadDebugResources(work);
}

void PlaceDebugCharacters(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(SFKM_FUY);
    EV_EnsureCharacterDraw(SFKM_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFKM_BASE, .parent = STREET_FIGHT_INVALID, .position = SFKM_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFKM_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 180.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKM_BASE);
    AT_SetLocalBaseChara(SFKM_FUY, SFKM_BASE);
    AT_SetLocalBaseChara(fighter, SFKM_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFKM_BASE, .position = SFKM_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFKM_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFKM_FUY, .parent = SFKM_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFKM_FUY, .parent = SFKM_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFKM_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFKM_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFKM_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFKM_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKM_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKM_DEBUG_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ENMY"), 0);
    sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    if (parameters->signal)
        *parameters->signal = 0;
}

void DebugMainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    switch (work.state) {
    case 0:
        debugLoadCharacters[0] = fighter;
        debugLoadCharacters[1] = STREET_FIGHT_INVALID;
        QueueDebugCharacterLoad(work.task);
        work.state = 1;
        break;
    case 1: {
        PlaceDebugCharacters(&work);
        work.audienceTask = QueueStreetFightAudienceCount(30);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        Vector3 stage;
        EV_GetCharaPosition(SFKM_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFKM_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFKM_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFKM_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            const float x = player.x - bookie.x, z = player.z - bookie.z;
            audience->corridorLength = std::sqrt(x * x + z * z);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFKM_FUY, EventTransformFlags::AllAxes, savedBookie);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->radius = 5.4f;
            audience->radiusRange = 2.0f;
            audience->state = 2;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        work.state = 2;
        break;
    }
    case 2:
        EVT_AT_SetPos(
            fighter, STREET_FIGHT_INVALID, {SFKM_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFKM_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFKM_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfkmStreetFightConversation(StartStreetFightBookieConversation, &work);
            work.state = 4;
            break;
        }
        [[fallthrough]];
    case 4:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        ControlStreetFightActionChoice(0);
        work.state = 5;
        break;
    case 5: {
        ReadBookiePosition(SFKM_FUY);
        const std::uint32_t choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 38)
            work.state = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= 20 ? 6 : 8;
        else
            work.state = 7;
        break;
    }
    case 6:
        QueueSfkmStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfkmStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfkmStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
        work.state = 23;
        break;
    case 9:
        HLib::SetCurrentTaskTo(DebugBattleEntryCallback);
        work.state = 12;
        break;
    case 12:
        work.state = work.battleStatus == 3 ? 13 : 17;
        break;
    case 13:
        ++work.completedOpponents;
        AddStreetFightProgress(SFKM_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfkmStreetFightConversation(StreetFightVictoryConversation, &work);
        work.state = 14;
        break;
    case 14: {
        nullsub();
        const StreetFightFighterDefinition *const next = GetStreetFightDebugFighterDefinition();
        work.fighterDefinitions[0] = next;
        const shen_name nextFighter = next->character;
        work.fighters[0] = nextFighter;
        EV_NPC_NewLoadSw(0);
        if (fighter.name != STREET_FIGHT_INVALID.name) {
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[0] = audience->fighters[1] = audience->fighters[2] = audience->fighters[3] = shen_name{};
                audience->fighterCount = 0;
            }
            ReleaseStreetFightCharacter(fighter);
        }
        if (nextFighter.name != STREET_FIGHT_INVALID.name) {
            debugLoadCharacters[0] = nextFighter;
            debugLoadCharacters[1] = STREET_FIGHT_INVALID;
            QueueDebugCharacterLoad(work.task);
        }
        work.state = 15;
        break;
    }
    case 15:
        EV_NPC_NewLoadSw(1);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        QueueSfkmStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        ControlStreetFightActionChoice(0);
        work.state = 19;
        break;
    case 17:
        QueueSfkmStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfkmStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFKM_FUY);
        const std::uint32_t choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfkmStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfkmStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfkmStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfkmStreetFightConversation(DebugCompletedConversation, &work);
            work.state = 23;
        }
        break;
    case 23:
        work.state = 24;
        break;
    case 24:
        if (!filmCheckFadeTrackColour() && !GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            work.state = 25;
        break;
    case 25:
        if (work.audienceTask) {
            (void)HLib::GetTaskParameter(work.audienceTask);
            HLib::DestroyTaskInternal(work.audienceTask);
        }
        CleanupDebug(&work);
        ++work.state;
        break;
    default:
        break;
    }
}

void DebugBattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EventSetPosition position{0, STREET_FIGHT_RYO, SFKM_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFKM_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFKM_FUY, .parent = SFKM_BASE, .position = SFKM_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFKM_FUY, .parent = SFKM_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFKM_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFKM_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}

void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    const EventFilmControl film{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5};
    EVT_AT_MenCtrl(film);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1ae0, 0, 1, 0, 50);
    EV_PlayDialogue("0100B072");
    EventSetCamera camera{0, 2001, SFKM_BASE, SFKM_BASE, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::SetCurrentTaskTo(DebugBattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}

void DebugBattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    const shen_name fighter = work.fighters[work.fighterIndex];
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::SetCurrentTaskTo(DebugBattleStartCallback);
    HLib::SetupTaskDelay(10);
}

void DebugBattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    work.hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFC"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(ctrlGetScenarioChara(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFKM_DEBUG_OPPONENT_ORIGIN.x, SFKM_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    vfmQueueFightStart(2, 0x63, 1, 8, 30);
    work.hudEnabled = 0;
    AccessFightActorFlag411(1, ctrlGetScenarioChara(fighter), definition->guardMode);
    AccessFightActorGroupFloat(1, ctrlGetScenarioChara(fighter), definition->strikeScale);
    AccessFightActorGroupFloat(3, ctrlGetScenarioChara(fighter), definition->guardScale);
    AccessFightActorGroupFloat(5, ctrlGetScenarioChara(fighter), definition->guardScale);
    HLib::SetCurrentTaskTo(DebugBattleReadyGoCallback);
}

void DebugBattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100B073");
    HLib::SetCurrentTaskTo(DebugBattleStartFightCallback);
    HLib::SetupTaskDelay(40);
}

void DebugBattleStartFightCallback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(DebugBattleWaitCallback);
}

void DebugBattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (IsFightActive())
        return;
    if (GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) == 7) {
        work.battleStatus = 7;
        EV_PlayDialogue("0100B075");
    } else {
        work.battleStatus = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) == 3 ? 3 : 2;
        EVT_AT_SetVoice("0100B074", 1, 0); // @text EN: "勝負あり！" | JA: "勝負あり！"
    }
    HLib::SetCurrentTaskTo(DebugBattleWaitFadeCallback);
}

void DebugBattleWaitFadeCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        ProcessAicaCommand(0x0aa0, 30);
        EventFilmControl film{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        HLib::SetCurrentTaskTo(DebugBattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
    work.frameCounter = 0;
}

void DebugBattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 0);
    BeginSecondarySampleExecution(0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKM_BASE);
    AT_SetLocalBaseChara(fighter, SFKM_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFKM_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, SFKM_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFKM_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFKM_FUY, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_FUY, SFKM_BASE, 0, 0);
    EVT_AT_SetMot(SFKM_FUY, 0x1aed, 0, 1.1f, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_ACCEPT;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_WAIT;
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_INVITATION;
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = STREET_FIGHT_DEBUG_CHALLENGE;
    fourth->frames = 60;
    EVT_AT_SetCam(2006, SFKM_BASE, SFKM_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFKM_BASE, SFKM_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, SFKM_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKM_FUY, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_FUY, SFKM_BASE, 0, 0);
    EVT_AT_SetMot(SFKM_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_CALL;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_NO_OPPONENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFKM_BASE, SFKM_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}

std::int32_t DebugStopConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKM_FUY, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_FUY, SFKM_BASE, 0, 0);
    EVT_AT_SetMot(SFKM_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_STOP;
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_AGREEMENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_STOP_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFKM_BASE, SFKM_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}

std::uint32_t battleDialogueIds[3]{};
std::int32_t firstStoryMatch{};
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattlePreInitCallback(void *);
void BattleStartCallback(void *);
void BattleWaitCallback(void *);
void BattleEndCallback(void *);
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t VictoryAppearanceConversation(void *);
std::int32_t DefeatConversation(void *);
std::int32_t TimeoutConversation(void *);

void PlaceNormal(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_EnableCharacterDisplay(SFKM_OD2);
    EV_EnsureCharacterDraw(SFKM_OD2);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work.playerPosition = {-3.3f, -0.5f, -5.5f};
    work.bookiePosition = {-1.564f, 0, -1.1f};
    EVT_AT_SetPos(SFKM_BASE, STREET_FIGHT_INVALID, SFKM_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFKM_BASE, STREET_FIGHT_INVALID, 180, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKM_BASE);
    AT_SetLocalBaseChara(SFKM_OD2, SFKM_BASE);
    AT_SetLocalBaseChara(fighter, SFKM_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, SFKM_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_INTRO_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 225, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 0, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFKM_OD2, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
    mtwkSetRequestedRootMode(scenFindMotionwork(STREET_FIGHT_RYO), 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(SFKM_OD2), 0);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFKM_OD2), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(fighter), FacialNodeReplacement);
    adfsSetCameraIdxForChara(SFKM_OD2, SFKM_BOOKIE_VOICE);
    adfsSetCameraIdxForChara(fighter, SFKM_FIGHTER_VOICE);
    LoadBlurModelForCharacter(SFKM_AW1, nullptr, 0.2f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
}

void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0:
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        if (!EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFC"), 0)) {
            const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
            EV_pakUtils(
                PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFC"));
            EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        }
        EV_FreeAuthorPak(1, "0100");
        work.characterReadTask = Queue_EVT_AT_ReadChr(work.task, SFKM_CHARACTERS);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
        CreateEventCharacter(CharacterCreationHighModel, nullptr, SFKM_BASE);
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0100E000");
        EV_LoadAudio("0100E000", 0);
        EV_spmBeginSampleExecute();
        FinishLoading();
        shenSetClockAndCompassRenderTo(1);
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = fighter;
                audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->soundCentre = SFKM_STAGE_ORIGIN;
                audience->state = 2;
            }
        }
        ++work.state;
        return;
    case 1: {
        PlaceNormal(work);
        work.state = (work.fighterIndex != 0) + 2;
        Vector3 centre;
        EV_GetCharaPosition(SFKM_BASE, CHARACTER_TRANSLATION_ROOT, &centre, EventTransformFlags::Relative);
        centre.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = centre;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Relative);
        EV_GetCharaPosition(SFKM_OD2, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Relative);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFKM_OD2, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFKM_OD2, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            const float x = player.x - bookie.x, z = player.z - bookie.z;
            audience->corridorLength = std::sqrt(x * x + z * z);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFKM_OD2, EventTransformFlags::AllAxes, savedBookie);
        SetLowQualityHands(ctrlGetScenarioChara(SFKM_AW1));
        SetHighModelXYZ(ctrlGetScenarioChara(SFKM_AW1), 1, 0, 0);
        SetHighModelXYZ(ctrlGetScenarioChara(SFKM_AW1), 1, 1, 0);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        return;
    }
    case 2:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfkmStreetFightConversation(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 740) ? RepeatIntroConversation : FirstIntroConversation, &work);
            work.state = 4;
        }
        return;
    case 4:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 740, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 5;
        return;
    case 5: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFKM_OD2);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollStreetFightActionChoice());
        if (!choice)
            return;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 22) {
            if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= SFKM_ENTRY_FEES[work.rank]) {
                work.state = 7;
                work.paidEntry = 1;
            } else
                work.state = 9;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = 6;
        } else
            work.state = 8;
        return;
    }
    case 6:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 4;
        return;
    case 7:
        QueueSfkmStreetFightConversation(AcceptConversation, &work);
        work.state = 10;
        return;
    case 8:
        QueueSfkmStreetFightConversation(DeclineConversation, &work);
        work.state = 20;
        return;
    case 9:
        QueueSfkmStreetFightConversation(InsufficientConversation, &work);
        work.state = 20;
        return;
    case 10:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 11;
        return;
    case 11:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetCurrentTaskTo(BattleEntryCallback);
            work.state = 12;
        }
        return;
    case 12:
        work.rankHistory[work.fighterIndex] = 1;
        work.state = work.battleStatus == 3 ? 13 : work.battleStatus == 2 ? 18 : 19;
        return;
    case 13:
        QueueSfkmStreetFightConversation(VictoryConversation, &work);
        if (CanPerformForCurrentAreaFlags() && CommitStreetFightStoryRank(work.stage) - 1U <= 1U)
            work.state = 14;
        else
            work.state = 20;
        return;
    case 14:
        QueueSfkmStreetFightConversation(VictoryAppearanceConversation, &work);
        nullsub();
        work.state = 20;
        return;
    case 18:
        ResetStreetFightStoryFlag(work.stage);
        QueueSfkmStreetFightConversation(DefeatConversation, &work);
        work.state = 20;
        return;
    case 19:
        QueueSfkmStreetFightConversation(TimeoutConversation, &work);
        work.state = 20;
        return;
    case 20:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 21;
        return;
    case 21:
        if (!filmCheckFadeTrackColour()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry &&
                !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{67}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{131}, 0, -1) == 2)
                QueueFightAdviceControl(8);
            work.state = 22;
        }
        return;
    case 22:
        if (work.battleStatus && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.battleStatus == 3)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 741, 1);
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        return;
    default:
        return;
    }
}

std::int64_t SelectFirstBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFKM_FIRST_BATTLE_VOICES[index];
    if (++index >= 4)
        index = 0;
    return voice;
}
std::int64_t SelectSecondBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFKM_SECOND_BATTLE_VOICES[index];
    if (++index >= 2)
        index = 0;
    return voice;
}

void BattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, SFKM_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 180, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, {0, 0, 1.1f}, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 0, 0);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
}
void BattlePreEventCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EV_LoadAudio("0100E000", 1);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFC"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, static_cast<std::uint16_t>(200 * (work.rank + 5)), 31);
    if (work.fighterIndex)
        vfUpdateFlagsFor(ctrlGetScenarioChara(fighter), 0, 0x8000000U, 4);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    EV_NPC_NewLoadSw(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, 1, 0, 0);
    EVT_AT_SetMot(fighter, 3156, 0, 1, 0, 0);
    EVT_AT_SetVoice("0100C013", 1, 60); // @text EN: "Take your stance!" | JA: "それじゃあ、かまえて。"
    EVT_AT_SetVoice("0100C014", 1, 140); // @text EN: "Ready=@go!" | JA: "レディ・ゴー！"
    EventSetCamera camera{.cleanup = 0, .camera = 2400, .eye = SFKM_BASE, .target = SFKM_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(BattlePreInitCallback);
    HLib::SetupTaskDelay(170);
}
void BattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (cvsHasValidConverseData(0))
        return;
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    UnparentAndSetExistingPosRot(work.fighters[work.fighterIndex]);
    HLib::SetCurrentTaskTo(BattleStartCallback);
    HLib::SetupTaskDelay(10);
}
void BattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const std::int32_t level = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{67}, 0, -1));
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(level + 1), 1);
    vfCopyPosFromEntity(0, nullptr, SFKM_STAGE_ORIGIN.x, SFKM_STAGE_ORIGIN.z, 2.5f);
    SetPluralControlFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(fighter), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFKM_OD2), FacialNodeOriginal);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeDefault);
    SetScenarioCharacterType(fighter, CharacterTypeDefault);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(2, 0x63, 1, 0, 60);
    EV_SetCharacterAlphaEnabled(fighter, false);
    if (work.rank >= 1)
        vfmDispatchControl(FightControlOp::CameraMode, {}, 3, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1787, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    battleDialogueIds[0] = battleDialogueIds[1] = battleDialogueIds[2] = 0;
    if (work.rank)
        battleDialogueIds[0] = AllocatePeriodicBattleDialogue(20, 159, fighter, STREET_FIGHT_RYO, 150, 100, 0);
    else {
        battleDialogueIds[1] = AllocatePeriodicBattleDialogue(20, 157, fighter, STREET_FIGHT_RYO, 300, 100, 0);
        battleDialogueIds[2] = AllocatePeriodicBattleDialogue(20, 159, fighter, STREET_FIGHT_RYO, 720, 100, 0);
    }
    const std::uint32_t first = AllocateBattleDialogueCondition12(30, -1, fighter, shen_name{}, 1, 60, 1);
    AssignBattleDialogueCallback(first, SelectFirstBattleVoice);
    const std::uint32_t second = AllocateBattleDialogueCondition11(30, -1, fighter, shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(second, SelectSecondBattleVoice);
    HLib::SetCurrentTaskTo(BattleWaitCallback);
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    if (!IsFightActive()) {
        work.audienceResultPublished = 0;
        const std::int32_t status = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO));
        work.battleStatus = status == 3 ? 3 : status == 2 ? 2 : 7;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        HLib::SetCurrentTaskTo(BattleEndCallback);
        HLib::SetupTaskDelay(30);
    } else {
        const std::uint8_t flags = static_cast<std::uint8_t>(GetFightTaskFlags());
        if (flags & 2U)
            QueueStreetFightPlayerReaction(fighter, false);
        if (flags & 4U)
            QueueStreetFightPlayerReaction(fighter, true);
        if (!GetFightCharacterFlag(0, ctrlGetScenarioChara(STREET_FIGHT_RYO), 0) || !GetFightCharacterFlag(0, ctrlGetScenarioChara(fighter), 0)) {
            BeginFightDialogueRetirement();
            QueueBattleDialogue(-32768, -1);
            battleDialogueIds[0] = battleDialogueIds[1] = battleDialogueIds[2] = 0;
        } else {
            if (battleDialogueIds[1] && GetBattleDialogueCompletionCount(battleDialogueIds[1])) {
                vfmDispatchControl(FightControlOp::CameraMode, {}, 3, 1);
                EV_SetCharacterAlphaEnabled(fighter, false);
                battleDialogueIds[1] = 0;
            }
            if (battleDialogueIds[0] && GetBattleDialogueCompletionCount(battleDialogueIds[0])) {
                EV_SetCharacterAlphaEnabled(fighter, false);
                battleDialogueIds[0] = 0;
            }
            if (battleDialogueIds[2] && GetBattleDialogueCompletionCount(battleDialogueIds[2])) {
                EV_SetCharacterAlphaEnabled(fighter, false);
                battleDialogueIds[2] = 0;
            }
        }
        if (!work.audienceResultPublished) {
            const std::int32_t status = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO));
            if (status == 2 || status == 3) {
                if (work.audienceTask) {
                    StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                    if (audience) {
                        audience->state = status == 2 ? 7 : 6;
                        audience->loopDelay = status == 2 ? 1 : 2;
                    }
                }
                ++work.audienceResultPublished;
            } else if (static_cast<std::uint32_t>(status - 5) <= 2U) {
                EV_PlayDialogue("0100F017");
                ++work.audienceResultPublished;
            }
        }
    }
    work.frameCounter = 0;
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(fighter), FacialExpressionId::Angry, 1, 1);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(fighter), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFKM_OD2), FacialNodeReplacement);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    EV_SetCharacterAlphaEnabled(SFKM_OD2, false);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKM_BASE);
    AT_SetLocalBaseChara(fighter, SFKM_BASE);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(SFKM_AW1), 1, 0, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(SFKM_AW1), 1, 1, 0);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    work.frameCounter = 0;
    HLib::SetCurrentTaskTo(MainCallback);
}

std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 199);
    ReadBookiePosition(SFKM_OD2);
    hmdlSetFlagsFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 2, 16);
    EV_SetCharacterAlphaEnabled(SFKM_OD2, false);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 40, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetMot(SFKM_OD2, 6853, 0, 1.1f, 0, 545);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_GREETING_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, -15, 0);
    EVT_AT_SetMot(fighter, 6852, 0, 1.2f, 0x40000000U, 0);
    hmdlSetFlagsFor(ctrlGetScenarioChara(fighter), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetScenarioChara(fighter), 1, 2, 16);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 0, reinterpret_cast<std::byte *>(SFKM_GREETING_HAND), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 1, reinterpret_cast<std::byte *>(SFKM_GREETING_HAND), 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 2});
    EVT_AT_SetVoice("0100C001", 1, 10); // @text EN: "Now, come and see this!" | JA: "さあさあ、ごらんあれ。"
    EVT_AT_SetVoice("0100C002", 1, 90); // @text EN: "Zhouhong Kong here\nis not just any expert." | JA: "ここにいる孔周鴻は\nただの達人じゃねえぞ。"
    EVT_AT_SetVoice("0100C003", 1, 220); // @text EN: "When training in the mountains,\nhe killed a tiger with his bare hands!" | JA: "山ごもりの最中にトラと出くわし、\nそれを素手で仕留めた豪傑だ。"
    EVT_AT_SetVoice("0100C004", 1, 400); // @text EN: "He lost his eye in that battle,\nbut that's proof of his bravery!" | JA: "その時に失った片目こそ、\n勇者の勲章だぜ。"
    switch (SFKM_ENTRY_FEES[work.rank]) {
    case 10:
        EVT_AT_SetVoice("0100C008", 1, 540); // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
        break;
    case 20:
        EVT_AT_SetVoice("0100C009", 1, 540); // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
        break;
    case 30:
        EVT_AT_SetVoice("0100C010", 1, 540); // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
        break;
    case 40:
        EVT_AT_SetVoice("0100C011", 1, 540); // @text EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！"
        break;
    default:
        EVT_AT_SetVoice("0100C005", 1, 540); // @text EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
        break;
    }
    EVT_AT_SetVoice("0100C021", 1, 620); // @text EN: "The time limit is sixty seconds." | JA: "制限時間は６０秒。"
    EVT_AT_SetVoice("0100C007", 1, 690); // @text EN: "Anyone want to challenge this man?" | JA: "さあ、誰かこの豪傑に\n挑戦したいヤツはいねえか？"
    EVT_AT_SetCam(100, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    EVT_AT_SetCam(8100, SFKM_BASE, SFKM_BASE, 0, 0, 85);
    EVT_AT_SetCam(2309, SFKM_BASE, SFKM_BASE, 0, 0, 220);
    EVT_AT_SetCam(2302, SFKM_BASE, SFKM_BASE, 0, 0, 545);
    EVT_AT_SetCam(2317, SFKM_BASE, SFKM_BASE, 0, 0, 735);
    return 880;
}
std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 398, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFKM_OD2);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 225, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, SFKM_PLAYER_REPEAT_POSITION, 0);
    hmdlSetFlagsFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 2, 16);
    EV_SetCharacterAlphaEnabled(SFKM_OD2, false);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 40, 0);
    EVT_AT_SetMot(SFKM_OD2, 6853, 0, 1.1f, 0x40000000U, 0);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_GREETING_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, -15, 0);
    EVT_AT_SetMot(fighter, 6852, 0, 1.2f, 0x40000000U, 0);
    hmdlSetFlagsFor(ctrlGetScenarioChara(fighter), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetScenarioChara(fighter), 1, 2, 16);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 0, reinterpret_cast<std::byte *>(SFKM_GREETING_HAND), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 1, reinterpret_cast<std::byte *>(SFKM_GREETING_HAND), 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 2});
    switch (SFKM_ENTRY_FEES[work.rank]) {
    case 10:
        EVT_AT_SetVoice("0100C008", 1, 0); // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
        break;
    case 20:
        EVT_AT_SetVoice("0100C009", 1, 0); // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
        break;
    case 30:
        EVT_AT_SetVoice("0100C010", 1, 0); // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
        break;
    case 40:
        EVT_AT_SetVoice("0100C011", 1, 0); // @text EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！"
        break;
    default:
        EVT_AT_SetVoice("0100C005", 1, 0); // @text EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
        break;
    }
    EVT_AT_SetVoice("0100C007", 1, 80); // @text EN: "Anyone want to challenge this man?" | JA: "さあ、誰かこの豪傑に\n挑戦したいヤツはいねえか？"
    EVT_AT_SetCam(2317, SFKM_BASE, SFKM_BASE, 0, 0, 1);
    return 270;
}
std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6856, 0, 1, 0x40000000U, 0);
    ReadBookiePosition(STREET_FIGHT_RYO);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 20, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 40, 155);
    EVT_AT_SetMot(SFKM_OD2, 6854, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_ACCEPT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 30, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1, 0x50000000U, 155);
    EVT_AT_SetCam(2304, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    EVT_AT_SetCam(2316, SFKM_BASE, SFKM_BASE, 0, 0, 155);
    EVT_AT_SetVoice("0100A022", 1, 60); // @text EN: "I'll do it." | JA: "俺がやろう。"
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience) {
            audience->state = 4;
            audience->loopDelay = 100;
        }
    }
    const std::uint8_t rank = static_cast<std::uint8_t>(work.rank);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1787)) {
        EVT_AT_SetMot(SFKM_OD2, 6854, 0, 1.1f, 0, 155);
        EVT_AT_SetVoice("0100C015", 1, 100); // @text EN: "All right.\nGet up there." | JA: "よし、決まりだ。\nさ、兄ちゃん上がんな。"
        if (vfmDispatchControl(FightControlOp::MotionNibble, shen_name{131}, 0, -1))
            return 230;
        EVT_AT_SetVoice("0100R004", 1, 230); // @text EN: "It won't be the same this time." | JA: "以前のようにはいかんぞ。"
        return 310;
    }
    EVT_AT_SetVoice("0100C012", 1, 100); // @text EN: "All right, here's a young man!" | JA: "よし。兄ちゃんに決まりだ！"
    if (work.rankHistory[rank])
        return 200;
    EVT_AT_SetVoice("0100R001", 1, 200); // @text EN: "He's young,\nbut looks like he can fight." | JA: "若いが、多少はできそうだな。"
    return 310;
}
std::int32_t DeclineConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    ReadBookiePosition(STREET_FIGHT_RYO);
    switch (shenGetRandomInt32(3)) {
    case 0:
        EVT_AT_SetVoice("0100A038", 1, 0); // @text EN: "No, I don't think so." | JA: "やめておこう…"
        break;
    case 1:
        EVT_AT_SetVoice("0100A039", 1, 0); // @text EN: "Maybe next time." | JA: "またにするか…"
        break;
    case 2:
        EVT_AT_SetVoice("0100A040", 1, 0); // @text EN: "I might try next time." | JA: "また今度にするか…"
        break;
    }
    EVT_AT_SetCam(2315, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    return 60;
}
std::int32_t InsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const money = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    money->cleanup = 1;
    money->delay = 60;
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    EVT_AT_SetCam(2315, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    return 150;
}
std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    SetClothFlag(ctrlGetScenarioChara(STREET_FIGHT_RYO), 16, 1);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, 0, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_WIN_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6851, 0, 1.2f, 0x50000000U, 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 0, reinterpret_cast<std::byte *>(SFKM_VICTORY_HAND), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 1, reinterpret_cast<std::byte *>(SFKM_VICTORY_HAND), 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 741)) {
        EVT_AT_SetVoice("0100R023", 1, 30); // @text EN: "I need more kung fu=@" | JA: "俺の功夫もまだまだか…"
        EVT_AT_SetVoice("0100C017", 1, 100); // @text EN: "Amazing.\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
    } else {
        EVT_AT_SetVoice("0100R002", 1, 30); // @text EN: "I lost.\nHe was in my blind spot." | JA: "やられた…\n死角に回り込まれるとは…"
        EVT_AT_SetVoice("0100C017", 1, 170); // @text EN: "Amazing.\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
    }
    PATI_StartMoneyCount(static_cast<std::uint32_t>(SFKM_ENTRY_FEES[work.rank]), 260);
    EVT_AT_SetCam(2307, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    EVT_AT_SetCam(210, SFKM_BASE, SFKM_BASE, 0, 0, 240);
    return 360;
}
std::int32_t VictoryAppearanceConversation(void *) {
    return static_cast<std::int32_t>(PlayStreetFightVictoryAppearance());
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    SetClothFlag(ctrlGetScenarioChara(STREET_FIGHT_RYO), 16, 1);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, -10, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_ACCEPT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 30, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1.2f, 0x40000000U, 180);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 0, reinterpret_cast<std::byte *>(SFKM_DEFEAT_HAND), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(fighter), 1, reinterpret_cast<std::byte *>(SFKM_DEFEAT_HAND), 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EVT_AT_SetVoice("0100C018", 1, 30); // @text EN: "Too bad.\nBut it was a good match." | JA: "おしかったな、兄ちゃん。\nでも、いいセンいってたぜ。"
    EVT_AT_SetVoice(firstStoryMatch ? "0100R003" : "0100R006", 1, 180); // @text 0100R003 EN: "Train yourself and come back.\nI'll be waiting." | JA: "もっと腕をみがいてこい。\n待ってるぞ。" ; 0100R006 EN: "You need more kung fu.\nWe'll have a match again." | JA: "功夫が足りんな。\n出直してこい。"
    EVT_AT_SetCam(2308, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    EVT_AT_SetCam(2318, SFKM_BASE, SFKM_BASE, 0, 0, 180);
    EVT_AT_SetCam(310, SFKM_BASE, SFKM_BASE, 0, 0, 300);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFKM_ENTRY_FEES[work.rank]), 330);
    return 450;
}
std::int32_t TimeoutConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKM_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKM_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(SFKM_OD2, SFKM_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKM_OD2, SFKM_BASE, -10, 0);
    EVT_AT_SetMot(SFKM_OD2, 6855, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFKM_BASE, SFKM_FIGHTER_ACCEPT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFKM_BASE, 30, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1.2f, 0x50000000U, 0);
    EVT_AT_SetVoice("0100C019", 1, 30); // @text EN: "It's a draw,\nso I can't ask you for your money." | JA: "引き分けじゃ金は取れねえな。"
    EVT_AT_SetVoice("0100C020", 1, 110); // @text EN: "Come and try again." | JA: "また今度挑戦してくれ。"
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EVT_AT_SetCam(2308, SFKM_BASE, SFKM_BASE, 0, 0, 0);
    return 200;
}

void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 740, 1);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI_Internal(choices, 0x7fff, -1, 2, 0);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2U;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}
void ResetToMain(StreetFightEventWork &work) {
    EV_ResetFreeTalkFlags();
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    work.state = 1;
    work.auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
}
void RetryChoiceCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const std::uint16_t choice = static_cast<std::uint16_t>(PollStreetFightActionChoice());
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82)
        ResetToMain(work);
    else
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
}
void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    SetPluralControlFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0);
    hndlSetFlags(ctrlGetScenarioChara(SFKM_AW1));
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFKM_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKM_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKM_DEBUG_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFKM_RETRY_AREA, 1);
    else if (work.reserved178)
        EV_scenQueueLoad(g_actNumber, SFKM_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    SetNewNpcBatchCountOverride(NpcFillType::FrontFill, EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) ? 10 : 15);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->startParameters = start;
    work->state = work->battleStatus = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->fighterIndex = 0;
    work->fighters[0] = SFKM_AW1;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->package = SFKM_DEBUG_PACKAGE;
    work->battleFrames = 0;
    work->rankHistory[0] = work->rankHistory[1] = work->rankHistory[2] = 0;
    work->audienceTask = nullptr;
    work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->reserved178 = 0;
    firstStoryMatch = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1787) == 0;
    SetNewNpcBatchCountOverride(NpcFillType::BackFill, EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) ? 12 : 15);
}
void Init() {
    battleDialogueIds[0] = battleDialogueIds[1] = battleDialogueIds[2] = 0;
    firstStoryMatch = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

} // namespace
const ScriptAreaDefinition areaSfkmScriptDefinition{.areaId = STREET_FIGHT_SFKM, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
#endif
