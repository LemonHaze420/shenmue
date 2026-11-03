#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_sfs2.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Sfs2Main(StreetFightStartParameters *start)
{
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
#else
static const shen_name sfs2Characters[] = {STREET_FIGHT_RYO, SFS2_CYC, SFS2_VNE, SFS2_CHN, STREET_FIGHT_INVALID};
static const Vector3 sfs2PlayerPosition = {-4.8f, -0.0f, -5.0f};
static const Vector3 sfs2BookiePosition = {-2.36f, 0.5f, -0.8f};
static const int sfs2EntryFees[] = {10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static Vector3 sfs2ExitPosition = {331.09f, 84.25f, 84.46f};
static Vector3i sfs2ExitRotation = {0, 54613, 0};
static Sfs2QteDefinition sfs2QteDefinitions[16] = {
    {{10, 7, 45, 0}, {4, 4, 512, 0, 0, 0, 0, 0}},
    {{10, 7, 44, 0}, {4, 4, 512, 4, 0, 0, 0, 0}},
    {{10, 7, 43, 0}, {4, 4, 512, 256, 4, 0, 0, 0}},
    {{10, 7, 42, 0}, {4, 4, 512, 1024, 256, 0, 0, 0}},
    {{10, 7, 41, 0}, {4, 4, 512, 16, 512, 0, 0, 0}},
    {{10, 7, 40, 0}, {4, 4, 512, 64, 4, 0, 0, 0}},
    {{10, 6, 39, 0}, {4, 4, 512, 2048, 1024, 0, 0, 0}},
    {{10, 6, 38, 0}, {4, 4, 512, 256, 1024, 2048, 0, 0}},
    {{10, 6, 37, 0}, {4, 4, 512, 1024, 2048, 256, 0, 0}},
    {{10, 6, 36, 0}, {4, 4, 512, 256, 2048, 16, 256, 0}},
    {{10, 6, 35, 0}, {4, 4, 512, 1024, 2048, 1024, 2048, 0}},
    {{10, 6, 34, 0}, {4, 4, 512, 16, 2048, 64, 256, 0}},
    {{10, 5, 33, 0}, {4, 4, 512, 1, 64, 16, 4, 0}},
    {{10, 5, 32, 0}, {4, 4, 512, 64, 2048, 512, 4, 0}},
    {{10, 5, 31, 0}, {4, 4, 512, 1, 1024, 4, 256, 0}},
    {{10, 5, 30, 0}, {4, 4, 512, 16, 1024, 1, 2048, 256}},
};
static unsigned char sfs2QtePhase = 0;
static unsigned char sfs2UnusedPhase = 0;
static unsigned char sfs2TauntPollFrames = 20;
static unsigned char sfs2IdleVoiceFrames = 0;
static unsigned int sfs2LastSpokenAnimation = 0;
static int sfs2FirstStoryMatch = 1;

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
    work = InitializeNormal(task);
    work->startParameters = start;
    LoadNormalResources(work);
    EventDebugRegister(3, NormalDebugCallback, work, 36000);
    work->fadeFrames = start->scene;
}

void LoadNormalResources(StreetFightEventWork *work)
{
    work->motions[0] = 0;
    FTBL.EV_FreeAuthorPak(1, "0100");
    if (!FTBL.EV_ControlPackageResources(0, 5, "QSFB", 0, 0)) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QSFB", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
    FTBL.EV_ControlPackageResources(2, 4, "QSFB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QSFB", 0, 0);
    FTBL.CreateEventCharacter(3, 0, STREET_FIGHT_BASE);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0100E000");
    FTBL.EV_LoadAudio("0100E000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.flddSetPointFlagOnGrid(SFS2_BTC1, 1);
    FTBL.flddSetPointFlagOnGrid(SFS2_GRID_ORIGIN, 0);
    if (FTBL.EV_GetShenmueFlag(255))
        FTBL.SetNewNpcBatchCountOverride(1, 10);
}

void PlaceNormal(StreetFightEventWork *work)
{
    shen_name fighter = work->fighters[work->fighterIndex];
    SetStreetFightCharacterVisible(fighter, 1);
    work->playerPosition.x = sfs2PlayerPosition.x;
    work->playerPosition.y = sfs2PlayerPosition.y;
    work->playerPosition.z = sfs2PlayerPosition.z;
    work->bookiePosition.x = sfs2BookiePosition.x;
    work->bookiePosition.y = sfs2BookiePosition.y;
    work->bookiePosition.z = sfs2BookiePosition.z;
    EVT_AT_SetPos(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 334.5f, 84.25f, 80.72f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 90.0f, 0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(SFS2_CYC, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(fighter, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(SFS2_STAG, STREET_FIGHT_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, -9.2f, -0.5f, -10.4f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.5f, 0.0f, -1.0f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 17.0f, 0);
    FTBL.SetScenarioCharacterType(STREET_FIGHT_RYO, 4);
    FTBL.SetScenarioCharacterType(SFS2_CYC, 4);
    FTBL.SetScenarioCharacterType(fighter, 4);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFS2_CYC), 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(fighter), 1);
    FTBL.adfsSetCameraIdxForChara(SFS2_CYC, SFS2_BOOKIE_CAMERA);
    FTBL.adfsSetCameraIdxForChara(fighter, SFS2_FIGHTER_CAMERA);
    FTBL.EV_SetPlayerFlag(12, 1292, 0);
    sfs2QtePhase = 0;
    sfs2UnusedPhase = 0;
    sfs2IdleVoiceFrames = 0;
}

StreetFightEventWork *InitializeNormal(HLibTask *task)
{
    StreetFightEventWork *work = (StreetFightEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = SFS2_VNE;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->area = SFS2_PACKAGE;
    work->battleFrames = 53;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = FTBL.GetNpcBatchCountOverride(50);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, sfs2Characters);
    work->audienceTask = 0;
    work->hudEnabled = 0;
    work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForArea(g_currentMap);
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
    work->returnToQron = 0;
    return work;
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
    EventDebugUnregister(3);
    FTBL.AT_SetLocalBaseChara(SFS2_STAG, STREET_FIGHT_INVALID);
    ReleaseStreetFightAudience(work->audienceTask);
    work->audienceTask = 0;
    ET_AT_KillWork(work->characterReadTask);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioCharacterKillMode(STREET_FIGHT_BASE, 1);
    FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &sfs2ExitPosition);
    FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &sfs2ExitRotation, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
    if (work->hudEnabled)
        FTBL.ToggleStreetFighterUI();
    FTBL.GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (i = 0; work->motions[i]; i++)
        FTBL.DeleteMotion((void *)work->motions[i]);
    FTBL.EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1))
        FTBL.EV_ControlPackageResources(0, 6, "QSFB", 0, 0);
    if (!IsStreetFightBestOfThree())
        FTBL.LoadCutsceneAndSoundProgramForArea();
    else
        FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (IsStreetFightBestOfThree())
        FTBL.EV_scenQueueLoad(4, SFS2_OMAKE_SCENE, 1);
    else if (work->returnToQron)
        FTBL.EV_scenQueueLoad(g_actNumber, SFS2_QRON_SCENE, 81);
    else {
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
    }
    FTBL.flddSetPointFlagOnGrid(SFS2_GRID_ORIGIN, 1);
    FTBL.flddSetPointFlagOnGrid(SFS2_BTC1, 0);
    if (FTBL.EV_GetShenmueFlag(255))
        FTBL.SetNewNpcBatchCountOverride(1, 6);
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}

void MainCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 position, second, player, bookie;
    shen_name fighter = work->fighters[work->fighterIndex];
    unsigned int choice;
    EventDebugCheck(3, 4);
    switch (work->state) {
    case 0:
        if (!FTBL.EV_GetPlayerFlag(11, 714))
            sfs2FirstStoryMatch = 1;
        else
            sfs2FirstStoryMatch = 0;
        FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
        PlaceNormal(work);
        if (!work->audienceTask) {
            work->audienceTask = QueueStreetFightAudience(30, 0);
            AddStreetFightAudienceFighter(work->audienceTask, STREET_FIGHT_RYO);
            AddStreetFightAudienceFighter(work->audienceTask, fighter);
        }
        FTBL.GetCharaPosition(STREET_FIGHT_BASE, -1, &position, 0);
        position.y -= 0.5f;
        SetStreetFightAudienceCentre(work->audienceTask, &position);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, -1, &player, 0);
        FTBL.GetCharaPosition(SFS2_CYC, -1, &bookie, 0);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &work->playerPosition);
        FTBL.EV_AT_SetPos(SFS2_CYC, 0x38000000, &work->bookiePosition);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, -1, &position, 0x40000000);
        FTBL.GetCharaPosition(SFS2_CYC, -1, &second, 0x40000000);
        SetStreetFightAudienceCorridor(work->audienceTask, &position, &second);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &player);
        FTBL.EV_AT_SetPos(SFS2_CYC, 0x38000000, &bookie);
        FTBL.EV_NPC_NewLoadSw_0(1);
        QueueStreetFightNpcWait(work->startParameters->flags);
        work->state = 1;
        break;
    case 1:
        if (IsStreetFightAudienceArranged(work->audienceTask)) {
            if (sfs2FirstStoryMatch)
                QueueStreetFightSkipWait(FirstIntroConversation, work, 1);
            else
                QueueStreetFightSkipWait(RepeatIntroConversation, work, 1);
            work->state = 2;
        }
        break;
    case 2:
        FTBL.EV_SetPlayerFlag(12, 714, 1);
        EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 470, 0x7fff, 0xffff, 2, 0);
        FTBL.CheckHelpIcon(1, 0, 1);
        work->paidEntry = 0;
        work->state = 3;
        break;
    case 3:
        FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
        ReadStreetFightCharacterPosition(STREET_FIGHT_RYO, SFS2_CYC, 16);
        choice = PollStreetFightActionChoice();
        if (choice) {
            FTBL.CheckHelpIcon(1, 1, 2);
            ClearStreetFightLook(STREET_FIGHT_RYO, 15);
            switch (choice) {
            case 38:
                if (FTBL.GetPlayerStat(2) < sfs2EntryFees[work->rank])
                    work->state = 7;
                else {
                    work->state = 5;
                    work->paidEntry = 1;
                }
                break;
            case 470:
                FTBL.vfmDispatchControl(21, 0, 0, 1);
                work->state = 4;
                break;
            default:
                work->state = 6;
                break;
            }
        }
        break;
    case 4:
        if (!FTBL.vfmDispatchControl(21, 0, 0, -1))
            work->state = 2;
        break;
    case 5:
        QueueStreetFightSkipWait(AcceptConversation, work, 1);
        work->state = 8;
        break;
    case 6:
        QueueStreetFightSkipWait(DeclineConversation, work, 1);
        work->state = 16;
        break;
    case 7:
        QueueStreetFightSkipWait(InsufficientConversation, work, 1);
        work->state = 16;
        break;
    case 8:
        FTBL.HLib_SetCurrentTaskTo(BattleEntryCallback);
        work->state = 9;
        break;
    case 9:
        if (work->battleStatus == 3)
            work->state = 10;
        else
            work->state = 15;
        break;
    case 10:
        QueueStreetFightSkipWait(VictoryConversation, work, 1);
        if (!CanPerformForAreaFlags(g_currentMap) || IsStreetFightBestOfThree())
            work->state = 16;
        else
            switch (CommitStreetFightStoryRank(work->stage)) {
            case 1:
                work->state = 11;
                break;
            case 2:
                work->state = 11;
                break;
            default:
                work->state = 16;
                break;
            }
        break;
    case 11:
        QueueStreetFightSkipWait(VictoryAppearanceConversation, work, 1);
        if (FTBL.GetStreetFightProgress() > 9)
            work->state = 16;
        else
            work->state = 16;
        break;
    case 15:
        ResetStreetFightStoryFlag(work->stage);
        QueueStreetFightSkipWait(DefeatConversation, work, 1);
        work->state = 16;
        break;
    case 16:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        work->state = 17;
        break;
    case 17:
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (!IsStreetFightBestOfThree() && work->paidEntry) {
                if (FTBL.EV_vfBattleUtil(7, 0x40000004, -1)) {
                    if (!FTBL.vfmDispatchControl(14, 71, 0, -1) && FTBL.vfmDispatchControl(14, 135, 0, -1) == 2)
                        FTBL.QueueFightAdviceControl(5);
                } else if (!FTBL.vfmDispatchControl(14, 71, 0, -1) && FTBL.vfmDispatchControl(14, 135, 0, -1) == 2)
                    FTBL.QueueFightAdviceControl(20);
            }
            work->state = 18;
        }
        break;
    case 18:
        if (work->battleStatus && !IsStreetFightBestOfThree() && work->battleStatus == 3)
            FTBL.EV_SetPlayerFlag(12, 715, 1);
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
    FTBL.EV_SetPlayerFlag(12, 714, 1);
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
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_ResetFreeTalkFlags();
    ChangeStreetFightAudienceMode(work->audienceTask, 2, -1);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(fighter), 1, 1, 1.0f);
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
    work->state = 0;
    work->battleStatus = 0;
    work->auxiliaryState = 0;
    FTBL.EV_LoadAudio("0100E000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 200);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 199);
    StreetFightNullsub(STREET_FIGHT_RYO, fighter, 16, 800, 605);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 30.0f, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0.0f, 1.1f, 0, 0);
    StreetFightNullsub(SFS2_CYC, STREET_FIGHT_RYO, 15, 877, 535);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -0.34f, 0.5f, -0.33f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 37.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -1.4f, 0.5f, -1.5f, 680);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 37.0f, 680);
    EVT_AT_SetMot(fighter, 6849, 0.0f, 1.2f, 0x40000000, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 0.8f, 0x50000000, 680);
    StreetFightNullsub(fighter, STREET_FIGHT_RYO, 16, 790, 605);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    EVT_AT_SetVoice("0100I001", 40);
    EVT_AT_SetVoice("0100I002", 227);
    EVT_AT_SetVoice("0100I003", 335);
    EVT_AT_SetVoice("0100I004", 425);
    EVT_AT_SetVoice("0100I006", 525);
    switch (sfs2EntryFees[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100I008", 590);
        break;
    case 20:
        EVT_AT_SetVoice("0100I009", 590);
        break;
    case 30:
        EVT_AT_SetVoice("0100I010", 590);
        break;
    case 40:
        EVT_AT_SetVoice("0100I011", 590);
        break;
    default:
        EVT_AT_SetVoice("0100I005", 590);
        break;
    }
    EVT_AT_SetVoice("0100I007", 680);
    EVT_AT_SetCam(8099, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(8100, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 120);
    EVT_AT_SetCam(600, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 270);
    EVT_AT_SetCam(610, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 590);
    EVT_AT_SetCam(1403, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 680);
    return 750;
}

int RepeatIntroConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 225.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFS2_CYC, 16, 1, 0, 4352);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 30.0f, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0.0f, 1.1f, 0x40000000, 0);
    QueueStreetFightLook(SFS2_CYC, STREET_FIGHT_RYO, 15, 1, 0, 4352);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -1.4f, 0.5f, -1.5f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 37.0f, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 0.8f, 0x50000000, 0);
    QueueStreetFightLook(fighter, STREET_FIGHT_RYO, 16, 1, 0, 4352);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    switch (sfs2EntryFees[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100I008", 0);
        break;
    case 20:
        EVT_AT_SetVoice("0100I009", 0);
        break;
    case 30:
        EVT_AT_SetVoice("0100I010", 0);
        break;
    case 40:
        EVT_AT_SetVoice("0100I011", 0);
        break;
    default:
        EVT_AT_SetVoice("0100I005", 0);
        break;
    }
    EVT_AT_SetVoice("0100I007", 80);
    EVT_AT_SetCam(1403, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    return 160;
}

int AcceptConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    StreetFightNullsub(STREET_FIGHT_RYO, SFS2_CYC, 6, 0, 156);
    StreetFightNullsub(SFS2_CYC, STREET_FIGHT_RYO, 6, 0, 156);
    StreetFightNullsub(fighter, STREET_FIGHT_RYO, 6, 0, 156);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6852, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetVoice("0100A022", 30);
    EVT_AT_SetVoice("0100I012", 60);
    EVT_AT_SetCam(130, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 5);
    EVT_AT_SetCam(140, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 58);
    StartStreetFightAudienceOpening(work->audienceTask, 60);
    return 156;
}

int DeclineConversation(void *data)
{
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
    EVT_AT_SetCam(132, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    return 60;
}

int InsufficientConversation(void *data)
{
    EVT_AT_StartMoneyCount(0, 60);
    EVT_AT_SetCam(134, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0100A037", 0);
    return 150;
}

void BattleEntryCallback(void *data)
{
    if (!IsStreetFightBestOfThree())
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(BattlePreEventCallback);
    FTBL.HLib_SetupTaskDelay(30);
}

void BattlePreEventCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 position;
    Vector3i rotation;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 0.0f, 0.5f, -1.1f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, -3.8f, 0.0f, -2.4f, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 220.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, 0.0f, 0.5f, 1.1f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0.0f, 0.5f, 0x40000000, 0);
    EVT_AT_SetMot(fighter, 3090, 0.0f, 0.5f, 0x40000000, 0);
    FTBL.GetCharaPosition(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, 0x40000000);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &rotation, 0);
    ClearStreetFightLook(STREET_FIGHT_RYO, 15);
    ClearStreetFightLook(SFS2_CYC, 15);
    ClearStreetFightLook(work->fighters[work->fighterIndex], 15);
    FTBL.GetScenarioCharacterEulerAngles(work->fighters[work->fighterIndex], &rotation, 0x40000000);
    FTBL.GetCharaPosition(work->fighters[work->fighterIndex], STREET_FIGHT_INVALID, &position, 0x40000000);
    FTBL.AT_SetLocalBaseChara(work->fighters[work->fighterIndex], STREET_FIGHT_INVALID);
    FTBL.EV_AT_SetPos(work->fighters[work->fighterIndex], 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(work->fighters[work->fighterIndex], 0x78000000, &rotation, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, fighter, 16, 0, 260);
    StreetFightNullsub(SFS2_CYC, STREET_FIGHT_RYO, 15, 0, 260);
    StreetFightNullsub(fighter, STREET_FIGHT_RYO, 16, 0, 260);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0.0f, 0.5f, 0x40000000, 0);
    work->hudEnabled = 1;
    if (FTBL.EV_vfBattleUtil(7, 0x40000004, -1))
        FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.vfmDispatchControl(2, 0, "QSFB", 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(fighter), 0, 1, 1, 150, 13);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 8, 0);
    FTBL.EV_vfBattleUtil(3, 1, 1);
    FTBL.EV_vfBattleUtil(3, 0x400, 1);
    FTBL.vfmDispatchControl(16, 0, (const void *)(FTBL.vfmDispatchControl(14, 71, 0, -1) + 1), 1);
    FTBL.vfCopyPosFromEntity(0, 0, 334.5f, 80.72f, 2.5f);
    FTBL.EV_SetPlayerFlag(12, 1292, 1);
    if (sfs2FirstStoryMatch)
        EVT_AT_SetVoice("0100V004", 30);
    else
        EVT_AT_SetVoice("0100V005", 30);
    EVT_AT_SetCam(1401, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    FTBL.EV_LoadAudio("0100E000", 1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(BattleStartFirstCallback);
}

void BattleStartFirstCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (!FTBL.camCtrlCheckIsEventCam()) {
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFS2_CYC), 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(work->fighters[work->fighterIndex]), 0);
        EVT_AT_SetCam(1402, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
        EVT_AT_SetVoice("0100I013", 30);
        EVT_AT_SetVoice("0100I014", 80);
        FTBL.HLib_SetCurrentTaskTo(BattleStartSecondCallback);
    }
}

void BattleStartSecondCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (!FTBL.camCtrlCheckIsEventCam() && FTBL.cvsHasValidConverseData(0) != 1) {
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFS2_VNE), 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFS2_CYC), 0);
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO));
        FTBL.EV_vfBattleUtil(3, 0x800, 1);
        FTBL.EV_vfBattleUtil(1, 2, 1);
        FTBL.EV_vfBattleUtil(3, 0x80000, 1);
        FTBL.vfmQueueFightStart(2, 99, 1.0f, 0, 30);
        FTBL.EV_SetCharacterAlphaEnabled(SFS2_VNE, 0);
        if (work->rank > 2)
            FTBL.EV_vfBattleUtil(9, SFS2_VNE, 1);
        FTBL.EV_SetPlayerFlag(12, 1786, 1);
        SetStreetFightStageFlag(work->stage);
        work->hudEnabled = 0;
        FTBL.HLib_SetCurrentTaskTo(BattleWaitCallback);
    }
}

void BattleWaitCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    if (FTBL.IsFightActive()) {
        if (FTBL.GetFightRuntime(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) & 8)
            BeginFinishingMotion(work);
        if (!work->auxiliaryState && FTBL.vfGetPlayerGuardEffeTimer() > 0) {
            ControlBattleQte(work);
            if (!--sfs2TauntPollFrames) {
                sfs2TauntPollFrames = 20;
                ControlFighterTaunt(work);
            }
            if (!(FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 0) & 0x40000)) {
                if (FTBL.cvsHasValidConverseData(0) != 1) {
                    if ((FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 1) & 4) && (FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 2) & 0x100) &&
                        FTBL.vfGetPlayerGuardEffeTimer() > 6 && sfs2LastSpokenAnimation != FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0)) {
                        switch (FTBL.shenGetRandomInt32(4)) {
                        case 0:
                            FTBL.EV_PlayDialogue("0100V012");
                            break;
                        case 1:
                            FTBL.EV_PlayDialogue("0100V016");
                            break;
                        case 2:
                            FTBL.EV_PlayDialogue("0100V014");
                            break;
                        case 3:
                            FTBL.EV_PlayDialogue("0100V015");
                            break;
                        }
                        sfs2LastSpokenAnimation = FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0);
                    }
                    if (sfs2LastSpokenAnimation != FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0))
                        sfs2LastSpokenAnimation = 0;
                }
                if (!(FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 0) & 0x100))
                    sfs2IdleVoiceFrames++;
                else
                    sfs2IdleVoiceFrames = 0;
                if (FTBL.cvsHasValidConverseData(0) != 1 && FTBL.vfGetPlayerGuardEffeTimer() > 6 && sfs2IdleVoiceFrames > 120) {
                    switch (FTBL.shenGetRandomInt32(3)) {
                    case 0:
                        FTBL.EV_PlayDialogue("0100V001");
                        break;
                    case 1:
                        FTBL.EV_PlayDialogue("0100V002");
                        break;
                    case 2:
                        FTBL.EV_PlayDialogue("0100V003");
                        break;
                    }
                    sfs2IdleVoiceFrames = 0;
                }
            }
            if (!FTBL.EV_vfBattleUtil(7, 0x40000004, -1) && work->rank > 2 && FTBL.vfGetPlayerGuardEffeTimer() == 25 - work->rank)
                FTBL.EV_vfBattleUtil(9, fighter, 0);
        }
        if ((unsigned char)(FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) - 5) <= 2 && !work->audienceResultPublished) {
            StartStreetFightAudienceVictory(work->audienceTask, 5);
            FTBL.EV_PlayDialogue("0100I020");
            work->audienceResultPublished++;
        }
    } else {
        if (work->auxiliaryState)
            work->battleStatus = 3;
        else {
            if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) == 3)
                work->battleStatus = 3;
            else
                work->battleStatus = 2;
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        FTBL.spmFadeOut(0);
        FTBL.HLib_SetupTaskDelay(30);
        FTBL.HLib_SetCurrentTaskTo(BattleEndCallback);
    }
}

void BattleEndCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    if (!FTBL.filmCheckFadeTrackColour()) {
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1.0f);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(fighter), 1, 1, 1.0f);
        FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_BASE);
        FTBL.AT_SetLocalBaseChara(fighter, STREET_FIGHT_BASE);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFS2_CYC), 1);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(fighter), 1);
        FTBL.SetScenarioCharacterTransparency(SFS2_CYC, 1.0f);
        FTBL.EV_SetPlayerFlag(12, 1292, 0);
        if (work->battleStatus == 3)
            RecordStreetFightVictory(work->stage);
        else
            RecordStreetFightDefeat(work->stage);
        FTBL.HLib_SetCurrentTaskTo(MainCallback);
    }
}

int VictoryConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6854, 0.0f, 1.0f, 0, 100);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0.0f, 1.1f, 0x40000000, 0);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(fighter), 6, 1, 1.0f);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -1.4f, 0.5f, -1.5f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 17.0f, 0);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 0.8f, 0x50000000, 0);
    EVT_AT_StartMoneyCount(sfs2EntryFees[work->rank], 105);
    EVT_AT_SetVoice("0100I016", 35);
    if (sfs2FirstStoryMatch)
        EVT_AT_SetVoice("0100V006", 120);
    else
        EVT_AT_SetVoice("0100V007", 120);
    EVT_AT_SetCam(200, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(210, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 110);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_LoadAudio("0100E000", 0);
    return 285;
}

int VictoryAppearanceConversation(void *data)
{
    return PlayStreetFightVictoryAppearance();
}

int DefeatConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_BASE, 180.0f, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6853, 0.0f, 1.0f, 0, 110);
    EVT_AT_SetPos(SFS2_CYC, STREET_FIGHT_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(SFS2_CYC, STREET_FIGHT_BASE, 0.0f, 1);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0.0f, 1.1f, 0, 0);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(fighter), 3, 1, 1.0f);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, -1.488f, 0.5f, -1.1f, 1);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 17.0f, 1);
    EVT_AT_SetMot(fighter, 6850, 0.0f, 0.8f, 0x50000000, 0);
    EVT_AT_SetVoice("0100I019", 35);
    if (sfs2FirstStoryMatch) {
        EVT_AT_SetVoice("0100V008", 155);
        EVT_AT_SetVoice("0100V009", 300);
    } else {
        EVT_AT_SetVoice("0100V010", 155);
        EVT_AT_SetVoice("0100V011", 240);
    }
    EVT_AT_StartMoneyCount(-sfs2EntryFees[work->rank], 140);
    EVT_AT_SetCam(300, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 120);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_LoadAudio("0100E000", 0);
    if (sfs2FirstStoryMatch)
        return 410;
    return 345;
}

void ConfigureBattleQte(int rank)
{
    int commands[8];
    int i;
    FTBL.qteBattleUtil(8, sfs2QteDefinitions[rank].parameters[0], sfs2QteDefinitions[rank].parameters[1], sfs2QteDefinitions[rank].parameters[2], 0);
    for (i = 0; i < 8; i++)
        commands[i] = 0;
    for (i = 0; i < 8; i++) {
        commands[i] = sfs2QteDefinitions[rank].commands[i];
        if (!commands[i])
            break;
    }
    FTBL.qteBattleUtil(9, commands[0], commands[1], commands[2], commands[3]);
    FTBL.qteBattleUtil(10, commands[4], commands[5], commands[6], commands[7]);
}

void ControlBattleQte(StreetFightEventWork *work)
{
    shen_name fighter = work->fighters[work->fighterIndex];
    int result;
    switch (sfs2QtePhase) {
    case 0:
        if (CanStartBattleQte(work)) {
            ConfigureBattleQte((unsigned short)FTBL.vfmDispatchControl(14, 71, 0, -1));
            FTBL.qteBattleUtil(1, 0, 0, 0, 0);
            FTBL.qteBattleUtil(2, 0, 0, 0, 0);
            sfs2QtePhase = 1;
        }
        break;
    case 1:
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        sfs2QtePhase = 2;
        break;
    case 2:
        result = FTBL.qteBattleUtil(4, 0, 0, 0, 0);
        if (!result)
            break;
        if (result == 1) {
            FTBL.EV_vfBattleUtil(4, 0x1000, 1);
            FTBL.EV_vfBattleUtil(3, 0x10, 1);
            EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, 334.5f, 84.25f, 80.72f, 0);
            EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, 334.5f, 84.25f, 80.72f, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, 90.0f, 0);
            EVT_AT_SetAngY(fighter, STREET_FIGHT_INVALID, 90.0f, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(fighter), 71, 0, 0);
            FTBL.vfmDispatchControl(7, STREET_FIGHT_RYO, (const void *)0x8000c7b, 1);
            EVT_AT_SetCam(8400, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 0);
            EVT_AT_SetCamRot(0.0f, 0.25f, 0.85f, 72);
            StartStreetFightAudienceDefeat(work->audienceTask, 75);
            switch (work->rank % 3) {
            case 0:
                EVT_AT_SetVoice("0100V017", 62);
                break;
            case 1:
                EVT_AT_SetVoice("0100V018", 62);
                break;
            case 2:
                EVT_AT_SetVoice("0100V019", 62);
                break;
            }
            EVT_AT_SetVoice("0100I015", 92);
            sfs2QtePhase = 3;
        } else {
            FTBL.vfmDispatchControl(7, STREET_FIGHT_RYO, (const void *)283, 1);
            sfs2QtePhase = 4;
        }
        break;
    case 3:
        if (!FTBL.IsFightActive()) {
            FTBL.qteBattleUtil(5, 0, 0, 0, 0);
            sfs2QtePhase = 5;
        }
        break;
    case 4:
        FTBL.qteBattleUtil(5, 0, 0, 0, 0);
        sfs2QtePhase++;
        break;
    case 5:
        sfs2QtePhase++;
        break;
    }
}

int CanStartBattleQte(StreetFightEventWork *work)
{
    if (FTBL.EV_vfBattleUtil(7, 0x40000004, -1) && FTBL.vfGetPlayerGuardEffeTimer() <= 4)
        return 1;
    return 0;
}

void BeginFinishingMotion(StreetFightEventWork *work)
{
    unsigned char rank = work->rank;
    work->auxiliaryState = 1;
    StartStreetFightAudienceDefeat(work->audienceTask, 5);
    switch (rank % 3) {
    case 0:
        FTBL.EV_PlayDialogue("0100V017");
        break;
    case 1:
        FTBL.EV_PlayDialogue("0100V018");
        break;
    case 2:
        FTBL.EV_PlayDialogue("0100V019");
        break;
    }
    if (FTBL.EV_vfBattleUtil(2, SFS2_VNE, 12) & 1)
        FTBL.vfmDispatchControl(7, SFS2_VNE, (const void *)3198, 1);
    else
        FTBL.vfmDispatchControl(7, SFS2_VNE, (const void *)3228, 1);
    EVT_AT_SetVoice("0100I015", 35);
    FTBL.EV_vfBattleUtil(4, 1, 1);
    FTBL.HLib_SetCurrentTaskTo(WaitMotionCallback);
}

void WaitMotionCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(SFS2_VNE), 0) == 3198 || FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(SFS2_VNE), 0) == 3228) {
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(SFS2_VNE), 0) > FTBL.GetMotionEndFrame(FTBL.scenFindMotionwork(SFS2_VNE), 0) - 30.0f) {
                FTBL.vfmDispatchControl(10, 0, 0, 1);
                FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            }
        }
    }
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(SFS2_VNE), 65, 1, 0)) {
        if (work->auxiliaryState == 1)
            FTBL.check_dead_status(1);
        FTBL.HLib_SetCurrentTaskTo(BattleWaitCallback);
    }
    if ((FTBL.GetFightRuntime(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) & 8) && !work->auxiliaryState)
        BeginFinishingMotion(work);
}

void ControlFighterTaunt(StreetFightEventWork *work)
{
    shen_name fighter = work->fighters[work->fighterIndex];
    Vector3 player, opponent;
    float distance;
    unsigned char divisor;
    if (FTBL.cvsHasValidConverseData(0) != 1) {
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, -1, &player, 0x40000000);
        FTBL.GetCharaPosition(fighter, -1, &opponent, 0x40000000);
        distance = FTBL.CalculatePlanarDistanceBetweenPoints(&player, &opponent);
        if (work->rank <= 3)
            divisor = FTBL.vfmDispatchControl(14, 71, 0, -1) + 2;
        else
            divisor = 99;
        if (!(FTBL.vfGetPlayerGuardEffeTimer() % divisor) && !(FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 0) & 1) && distance > 1.5f) {
            if (work->rank & 1)
                FTBL.EV_PlayDialogue("0100V020");
            else
                FTBL.EV_PlayDialogue("0100V021");
            FTBL.vfmDispatchControl(7, fighter, (const void *)3193, 1);
            FTBL.HLib_SetCurrentTaskTo(WaitMotionCallback);
        }
    }
}
#endif
#else
#include "s2/areas/area_sfs2.hpp"
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
#include "s2/math.hpp"
#include "s2/field_collision.hpp"
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
std::int32_t choicePhase{};
std::uint32_t ControlChoice(std::int32_t enabled) {
    if (!enabled) {
        choicePhase = 0;
        return 0;
    }
    switch (choicePhase) {
    case 0:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 2: {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    default:
        return 0;
    }
}
std::uint32_t PollChoice() {
    if (!choicePhase) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 1) {
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 2) {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    return 0;
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
std::int32_t DebugNextMatchConversation(void *);

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
    work->package = SFS2_DEBUG_PACKAGE;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFS2_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFS2_BASE);
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
    EV_EnableCharacterDisplay(SFS2_FUY);
    EV_EnsureCharacterDraw(SFS2_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, 0, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFS2_BASE, .parent = STREET_FIGHT_INVALID, .position = SFS2_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFS2_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 90.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFS2_BASE);
    AT_SetLocalBaseChara(SFS2_FUY, SFS2_BASE);
    AT_SetLocalBaseChara(fighter, SFS2_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFS2_BASE, .position = SFS2_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFS2_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFS2_FUY, .parent = SFS2_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFS2_FUY, .parent = SFS2_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFS2_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFS2_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFS2_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFS2_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFS2_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFS2_DEBUG_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
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
        EV_GetCharaPosition(SFS2_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFS2_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFS2_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFS2_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFS2_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFS2_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFS2_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFS2_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfs2StreetFightConversation(StartStreetFightBookieConversation, &work);
            work.state = 4;
            break;
        }
        [[fallthrough]];
    case 4:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 5;
        break;
    case 5: {
        ReadBookiePosition(SFS2_FUY);
        const std::uint32_t choice = ControlChoice(1);
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
        QueueSfs2StreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfs2StreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfs2StreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFS2_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfs2StreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSfs2StreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfs2StreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfs2StreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFS2_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfs2StreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfs2StreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfs2StreetFightConversation(DebugNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfs2StreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFS2_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFS2_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFS2_FUY, .parent = SFS2_BASE, .position = SFS2_DEBUG_BATTLE_ENTRY_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFS2_FUY, .parent = SFS2_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFS2_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFS2_BASE, .degrees = 0, .delay = 0};
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
    EventSetCamera camera{0, 2001, SFS2_BASE, SFS2_BASE, 0, {}, 0, 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFB"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(ctrlGetScenarioChara(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFS2_DEBUG_OPPONENT_ORIGIN.x, SFS2_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFS2_BASE);
    AT_SetLocalBaseChara(fighter, SFS2_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFS2_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, SFS2_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFS2_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFS2_FUY, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_FUY, SFS2_BASE, 0, 0);
    EVT_AT_SetMot(SFS2_FUY, 0x1aed, 0, 1.1f, 0, 0);
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
    EVT_AT_SetCam(2006, SFS2_BASE, SFS2_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFS2_BASE, SFS2_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, SFS2_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFS2_FUY, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_FUY, SFS2_BASE, 0, 0);
    EVT_AT_SetMot(SFS2_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFS2_BASE, SFS2_BASE, 0, 0, 5);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFS2_FUY, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_FUY, SFS2_BASE, 0, 0);
    EVT_AT_SetMot(SFS2_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFS2_BASE, SFS2_BASE, 0, 0, 5);
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

std::int32_t DebugNextMatchConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_DEBUG_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 225, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
    message->message = STREET_FIGHT_DEBUG_OPPONENT_ACCEPT;
    message->frames = 60;
    EVT_AT_SetVoice("0100B071", 1, 60); // @text EN: "よし、決まりだ。" | JA: "よし、決まりだ。"
    EVT_AT_SetCam(130, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    return 120;
}

std::int32_t firstStoryMatch{1};
std::uint8_t qtePhase{}, tauntPollFrames{20}, idleVoiceFrames{};
std::uint32_t lastSpokenAnimation{};
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattleStartFirstCallback(void *);
void BattleStartSecondCallback(void *);
void BattleWaitCallback(void *);
void BattleEndCallback(void *);
void WaitMotionCallback(void *);
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t VictoryAppearanceConversation(void *);
std::int32_t DefeatConversation(void *);

void SetAudienceState(StreetFightEventWork &work, std::uint8_t state, std::int16_t delay) {
    if (!work.audienceTask)
        return;
    StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
    if (audience) {
        audience->state = state;
        audience->loopDelay = delay;
    }
}
void PlaceNormal(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work.playerPosition = {-4.8f, -0.0f, -5};
    work.bookiePosition = {-2.36f, .5f, -.8f};
    EVT_AT_SetPos(SFS2_BASE, STREET_FIGHT_INVALID, SFS2_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFS2_BASE, STREET_FIGHT_INVALID, 90, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFS2_BASE);
    AT_SetLocalBaseChara(SFS2_CYC, SFS2_BASE);
    AT_SetLocalBaseChara(fighter, SFS2_BASE);
    AT_SetLocalBaseChara(SFS2_STAG, SFS2_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, SFS2_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 225, 0);
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 0, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_FIGHTER_INITIAL_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFS2_CYC, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFS2_CYC), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(fighter), FacialNodeReplacement);
    adfsSetCameraIdxForChara(SFS2_CYC, SFS2_BOOKIE_VOICE);
    adfsSetCameraIdxForChara(fighter, SFS2_FIGHTER_VOICE);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    qtePhase = 0;
    idleVoiceFrames = 0;
}
void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0: {
        firstStoryMatch = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 714) == 0;
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        PlaceNormal(work);
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            }
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = fighter;
            }
        }
        Vector3 playerPosition, bookiePosition, firstPosition, secondPosition, origin;
        EV_GetCharaPosition(SFS2_BASE, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::Local);
        origin.y -= .5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = origin;
            audience->state = 2;
        }
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::Local);
        EV_GetCharaPosition(SFS2_CYC, CHARACTER_TRANSLATION_ROOT, &bookiePosition, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFS2_CYC, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &firstPosition, EventTransformFlags::World);
        EV_GetCharaPosition(SFS2_CYC, CHARACTER_TRANSLATION_ROOT, &secondPosition, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = firstPosition.x;
            audience->corridorStart.z = firstPosition.z;
            audience->corridorEnd.x = secondPosition.x;
            audience->corridorEnd.z = secondPosition.z;
            audience->corridorLength = GetPlanarDistance(&firstPosition, &secondPosition);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, playerPosition);
        EV_AT_SetPos(SFS2_CYC, EventTransformFlags::AllAxes, bookiePosition);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.state = 1;
        break;
    }
    case 1:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfs2StreetFightConversation(firstStoryMatch ? FirstIntroConversation : RepeatIntroConversation, &work);
            work.state = 2;
        }
        break;
    case 2:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 714, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 3;
        break;
    case 3: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFS2_CYC);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
        if (!choice)
            return;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 22) {
            if (GetPlayerStat(PlayerStatsId::Cash) >= static_cast<std::uint64_t>(SFS2_ENTRY_FEES[work.rank])) {
                work.state = 5;
                work.paidEntry = 1;
            } else
                work.state = 7;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = 4;
        } else
            work.state = 6;
        break;
    }
    case 4:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 2;
        break;
    case 5:
        QueueSfs2StreetFightConversation(AcceptConversation, &work);
        work.state = 8;
        break;
    case 6:
        QueueSfs2StreetFightConversation(DeclineConversation, &work);
        work.state = 16;
        break;
    case 7:
        QueueSfs2StreetFightConversation(InsufficientConversation, &work);
        work.state = 16;
        break;
    case 8:
        HLib::SetCurrentTaskTo(BattleEntryCallback);
        work.state = 9;
        break;
    case 9:
        work.state = work.battleStatus == 3 ? 10 : 15;
        break;
    case 10:
        QueueSfs2StreetFightConversation(VictoryConversation, &work);
        if (!CanPerformForCurrentAreaFlags() || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ||
            static_cast<std::uint32_t>(CommitStreetFightStoryRank(work.stage) - 1) > 1)
            work.state = 16;
        else
            work.state = 11;
        break;
    case 11:
        QueueSfs2StreetFightConversation(VictoryAppearanceConversation, &work);
        nullsub();
        work.state = 16;
        break;
    case 15:
        ResetStreetFightStoryFlag(work.stage);
        QueueSfs2StreetFightConversation(DefeatConversation, &work);
        work.state = 16;
        break;
    case 16:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 17;
        break;
    case 17:
        if (filmCheckFadeTrackColour())
            return;
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry) {
            if (EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1)) {
                if (!vfmDispatchControl(FightControlOp::MotionNibble, shen_name{71}, 0, -1) &&
                    vfmDispatchControl(FightControlOp::MotionNibble, shen_name{135}, 0, -1) == 2)
                    QueueFightAdviceControl(5);
            } else if (!vfmDispatchControl(FightControlOp::MotionNibble, shen_name{71}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{135}, 0, -1) == 2)
                QueueFightAdviceControl(20);
        }
        work.state = 18;
        break;
    case 18:
        if (work.battleStatus && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.battleStatus == 3)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 715, 1);
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        break;
    default:
        break;
    }
}
void ConfigureBattleQte(std::int32_t rank) {
    const std::int16_t *const definition = SFS2_QTE_COMMANDS[rank];
    qteBattleUtil(BattleQteMode::SetWait, definition[0], definition[1], definition[2], 0);
    std::int32_t commands[8]{};
    for (std::int32_t i = 0; i < 8; ++i) {
        commands[i] = static_cast<std::int32_t>(static_cast<std::uint16_t>(definition[4 + i * 2])) |
            static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::uint16_t>(definition[5 + i * 2])) << 16);
        if (!commands[i])
            break;
    }
    qteBattleUtil(BattleQteMode::SetFirstCommands, commands[0], commands[1], commands[2], commands[3]);
    qteBattleUtil(BattleQteMode::SetLastCommands, commands[4], commands[5], commands[6], commands[7]);
}
void ControlBattleQte(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (qtePhase) {
    case 0:
        if (EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1) && GetFightPlayerGuardEffectTimerSeconds() <= 4) {
            ConfigureBattleQte(static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{71}, 0, -1)));
            qteBattleUtil(BattleQteMode::ResetStates);
            qteBattleUtil(BattleQteMode::Queue);
            qtePhase = 1;
        }
        break;
    case 1:
        qteBattleUtil(BattleQteMode::Start);
        qtePhase = 2;
        break;
    case 2: {
        const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
        if (!result)
            return;
        if (result == 1) {
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x10}, 1);
            EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, SFS2_STAGE_ORIGIN, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, 90, 0);
            EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, SFS2_STAGE_ORIGIN, 0);
            EVT_AT_SetAngY(fighter, STREET_FIGHT_INVALID, 90, 0);
            mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
            mtwkSetRegionFlag(scenFindMotionwork(fighter), 71, 0, MotionRegion::LowerBody);
            vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000c7b, 1);
            EVT_AT_SetCam(8400, SFS2_BASE, SFS2_BASE, 0, 0, 0);
            QueueCameraShake(0, .25f, .85000002f, 72);
            SetAudienceState(work, 6, 76);
            const std::uint8_t rank = static_cast<std::uint8_t>(work.rank);
            EVT_AT_SetVoice(rank % 3 == 0 ? "0100V017" : rank % 3 == 1 ? "0100V018" : "0100V019", 1, 62); // @text 0100V017 EN: "No!" | JA: "しまった！" ; 0100V019 EN: "I missed!" | JA: "ぬかったわ！"
            EVT_AT_SetVoice("0100I015", 1, 92); // @text EN: "Game!" | JA: "勝負あり！"
            qtePhase = 3;
        } else {
            vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x11b, 1);
            qtePhase = 4;
        }
        break;
    }
    case 3:
        if (!IsFightActive()) {
            qteBattleUtil(BattleQteMode::RetireAndFree);
            qtePhase = 5;
        }
        break;
    case 4:
        qteBattleUtil(BattleQteMode::RetireAndFree);
        ++qtePhase;
        break;
    case 5:
        ++qtePhase;
        break;
    default:
        break;
    }
}
void BeginFinishingMotion(StreetFightEventWork &work) {
    const std::uint8_t rank = static_cast<std::uint8_t>(work.rank);
    work.auxiliaryState = 1;
    SetAudienceState(work, 6, 6);
    EV_PlayDialogue(rank % 3 == 0 ? "0100V017" : rank % 3 == 1 ? "0100V018" : "0100V019");
    vfmDispatchControl(FightControlOp::QueuedAction, SFS2_VNE, EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFS2_VNE, 12) & 1 ? 3198 : 3228, 1);
    EVT_AT_SetVoice("0100I015", 1, 35); // @text EN: "Game!" | JA: "勝負あり！"
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
    HLib::SetCurrentTaskTo(WaitMotionCallback);
}
void ControlFighterTaunt(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    if (cvsHasValidConverseData(0))
        return;
    Vector3 playerPosition, fighterPosition;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &fighterPosition, EventTransformFlags::World);
    const float distance = GetPlanarDistance(&playerPosition, &fighterPosition);
    const std::uint8_t divisor = work.rank > 3 ? 99 : static_cast<std::uint8_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{71}, 0, -1) + 2);
    if (!(GetFightPlayerGuardEffectTimerSeconds() % divisor) && !(EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) & 1) && distance > 1.5f) {
        EV_PlayDialogue((work.rank & 1) ? "0100V020" : "0100V021");
        vfmDispatchControl(FightControlOp::QueuedAction, fighter, 0xc79, 1);
        HLib::SetCurrentTaskTo(WaitMotionCallback);
    }
}
void BattleEntryCallback(void *) {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
    HLib::SetupTaskDelay(30);
}
void BattlePreEventCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, SFS2_BATTLE_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 180, 0);
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, SFS2_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 220, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_BATTLE_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, .5f, 0x40000000U, 0);
    EVT_AT_SetMot(fighter, 3090, 0, .5f, 0x40000000U, 0);
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
    mtwkUpdateWithPosition(scenFindMotionwork(SFS2_CYC), nullptr, 0);
    mtwkUpdateWithPosition(scenFindMotionwork(fighter), nullptr, 0);
    Vector3 position;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, .5f, 0x40000000U, 0);
    work.hudEnabled = 1;
    if (EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1))
        qteBattleUtil(BattleQteMode::Load);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFB"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, 150, 13);
    vfUpdateFlagsFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 8, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x400}, 1);
    const std::intptr_t rank = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{71}, 0, -1);
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(rank + 1), 1);
    vfCopyPosFromEntity(0, nullptr, SFS2_DEBUG_OPPONENT_ORIGIN.x, SFS2_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    EVT_AT_SetVoice(firstStoryMatch ? "0100V004" : "0100V005", 1, 30); // @text 0100V004 EN: "His eyes have strength." | JA: "ふむ、いい目をしておる。" ; 0100V005 EN: "I'm looking forward to this bout." | JA: "ひとつ、お手合わせを願おうか。"
    EVT_AT_SetCam(1401, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    EV_LoadAudio("0100E000", 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(BattleStartFirstCallback);
    HLib::SetupTaskDelay(155);
}
void BattleStartFirstCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFS2_CYC), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(work.fighters[work.fighterIndex]), FacialNodeOriginal);
    EVT_AT_SetCam(1402, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
    EVT_AT_SetVoice("0100I013", 1, 30); // @text EN: "Take your stance!" | JA: "それじゃあ、かまえて。"
    EVT_AT_SetVoice("0100I014", 1, 80); // @text EN: "Ready, go!" | JA: "レディ・ゴー！"
    HLib::SetCurrentTaskTo(BattleStartSecondCallback);
    HLib::SetupTaskDelay(133);
}
void BattleStartSecondCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFS2_VNE), FacialNodeOriginal);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFS2_CYC), FacialNodeOriginal);
    hndlSetFlags(ctrlGetScenarioChara(STREET_FIGHT_RYO));
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(2, 99, 1, 0, 30);
    EV_SetCharacterAlphaEnabled(SFS2_VNE, false);
    if (work.rank >= 3)
        EV_vfBattleUtil(FightBattleUtilOp::MoveFlag40000, SFS2_VNE, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1786, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    work.hudEnabled = 0;
    HLib::SetCurrentTaskTo(BattleWaitCallback);
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    if (!IsFightActive()) {
        if (work.auxiliaryState)
            work.battleStatus = 3;
        else {
            work.battleStatus = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) == 3 ? 3 : 2;
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        spmFadeOut();
        HLib::SetupTaskDelay(30);
        HLib::SetCurrentTaskTo(BattleEndCallback);
        return;
    }
    if (GetFightActorFlags(ctrlGetScenarioChara(STREET_FIGHT_RYO)) & 8)
        BeginFinishingMotion(work);
    if (!work.auxiliaryState && GetFightPlayerGuardEffectTimerSeconds() > 0) {
        ControlBattleQte(work);
        if (!--tauntPollFrames) {
            tauntPollFrames = 20;
            ControlFighterTaunt(work);
        }
        if (!(EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) & 0x40000)) {
            if (!cvsHasValidConverseData(0)) {
                if ((EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 1) & 4) &&
                    (EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 2) & 0x100) && GetFightPlayerGuardEffectTimerSeconds() > 6 &&
                    lastSpokenAnimation != mtwkGetAnimationId(scenFindMotionwork(STREET_FIGHT_RYO), MotionRegion::LowerBody)) {
                    switch (shenGetRandomInt32(4)) {
                    case 0:
                        EV_PlayDialogue("0100V012");
                        break;
                    case 1:
                        EV_PlayDialogue("0100V016");
                        break;
                    case 2:
                        EV_PlayDialogue("0100V014");
                        break;
                    case 3:
                        EV_PlayDialogue("0100V015");
                        break;
                    default:
                        break;
                    }
                    lastSpokenAnimation = mtwkGetAnimationId(scenFindMotionwork(STREET_FIGHT_RYO), MotionRegion::LowerBody);
                }
                if (lastSpokenAnimation != mtwkGetAnimationId(scenFindMotionwork(STREET_FIGHT_RYO), MotionRegion::LowerBody))
                    lastSpokenAnimation = 0;
            }
            if (EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) & 0x100)
                idleVoiceFrames = 0;
            else
                ++idleVoiceFrames;
            if (!cvsHasValidConverseData(0) && GetFightPlayerGuardEffectTimerSeconds() > 6 && idleVoiceFrames > 120) {
                switch (shenGetRandomInt32(3)) {
                case 0:
                    EV_PlayDialogue("0100V001");
                    break;
                case 1:
                    EV_PlayDialogue("0100V002");
                    break;
                case 2:
                    EV_PlayDialogue("0100V003");
                    break;
                default:
                    break;
                }
                idleVoiceFrames = 0;
            }
        }
        if (!EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1) && work.rank >= 3 &&
            GetFightPlayerGuardEffectTimerSeconds() == static_cast<std::uint32_t>(25 - work.rank))
            EV_vfBattleUtil(FightBattleUtilOp::MoveFlag40000, fighter, 0);
    }
    if (static_cast<std::uint32_t>(GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) - 5) <= 2 && !work.audienceResultPublished) {
        SetAudienceState(work, 7, 5);
        EV_PlayDialogue("0100I020");
        ++work.audienceResultPublished;
    }
}
void WaitMotionCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (mtwkGetAnimationId(scenFindMotionwork(SFS2_VNE), MotionRegion::LowerBody) == 3198 ||
        mtwkGetAnimationId(scenFindMotionwork(SFS2_VNE), MotionRegion::LowerBody) == 3228) {
        if (!filmCheckFadeTrackColour()) {
            const double frame = mtwkGetFrame(scenFindMotionwork(SFS2_VNE), MotionRegion::LowerBody);
            if (frame > mtwkGetEndFrame(scenFindMotionwork(SFS2_VNE), MotionRegion::LowerBody) - 30.0) {
                vfmDispatchControl(FightControlOp::SpriteDepth, {}, 0, 1);
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            }
        }
    }
    if (mtwkSetRegionFlag(scenFindMotionwork(SFS2_VNE), 65, 1, MotionRegion::LowerBody)) {
        if (work.auxiliaryState == 1)
            check_dead_status(1);
        HLib::SetCurrentTaskTo(BattleWaitCallback);
    }
    if ((GetFightActorFlags(ctrlGetScenarioChara(STREET_FIGHT_RYO)) & 8) && !work.auxiliaryState)
        BeginFinishingMotion(work);
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (filmCheckFadeTrackColour())
        return;
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(fighter), FacialExpressionId::Angry, 1, 1);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFS2_BASE);
    AT_SetLocalBaseChara(fighter, SFS2_BASE);
    SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 1, 0);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFS2_CYC), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(fighter), FacialNodeReplacement);
    SetScenarioCharacterTransparency(SFS2_CYC, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    HLib::SetCurrentTaskTo(MainCallback);
}

const char *FeeVoice(std::int32_t fee) {
    switch (fee) {
    case 10:
        return "0100I008"; // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
    case 20:
        return "0100I009"; // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
    case 30:
        return "0100I010"; // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
    case 40:
        return "0100I011"; // @text EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！"
    default:
        return "0100I005"; // @text EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
    }
}
std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 225, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 225, 200);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0, 1, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 199);
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 30, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_FIGHTER_INTRO_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 37, 0);
    EVT_AT_SetPos(fighter, SFS2_BASE, {-1.4f, .5f, -1.5f}, 680);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 37, 680);
    EVT_AT_SetMot(fighter, 6849, 0, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(fighter, 6850, 0, .80000001f, 0x50000000U, 680);
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
    EVT_AT_SetVoice("0100I001", 1, 40); // @text EN: "Ma Wenyuan here, is one of the best\nwarriors of martial arts." | JA: "さあさあ、この馬文円こそは\n当代きっての拳法の名手だ。"
    EVT_AT_SetVoice("0100I002", 1, 227); // @text EN: "His Chinese martial arts technique is\nPassive Raid Style." | JA: "中国拳法の高等技術、化勁を使って、"
    EVT_AT_SetVoice("0100I003", 1, 335); // @text EN: "He'll gracefully parry\nthe opponent's attack." | JA: "相手の攻撃を華麗に受け流す。"
    EVT_AT_SetVoice("0100I004", 1, 425); // @text EN: "Strike him even once,\nand be the winner!" | JA: "この文円に一撃でも\n入れたら勝ちだ。"
    EVT_AT_SetVoice("0100I006", 1, 525); // @text EN: "The time limit is thirty seconds." | JA: "制限時間は３０秒。"
    EVT_AT_SetVoice(FeeVoice(SFS2_ENTRY_FEES[work.rank]), 1, 590);
    EVT_AT_SetVoice("0100I007", 1, 680); // @text EN: "Anyone strong enough\nto challenge this man?" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    EVT_AT_SetCam(8099, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    EVT_AT_SetCam(8100, SFS2_BASE, SFS2_BASE, 0, 0, 120);
    EVT_AT_SetCam(600, SFS2_BASE, SFS2_BASE, 0, 0, 270);
    EVT_AT_SetCam(610, SFS2_BASE, SFS2_BASE, 0, 0, 590);
    EVT_AT_SetCam(1403, SFS2_BASE, SFS2_BASE, 0, 0, 680);
    return 750;
}
std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFS2_CYC)->reserved08 = 16;
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 30, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0, 1.1f, 0x40000000U, 0);
    QueueStreetFightLook(SFS2_CYC, STREET_FIGHT_RYO)->reserved08 = 15;
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_FIGHTER_REPEAT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 37, 0);
    EVT_AT_SetMot(fighter, 6850, 0, .80000001f, 0x50000000U, 0);
    QueueStreetFightLook(fighter, STREET_FIGHT_RYO)->reserved08 = 16;
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
    EVT_AT_SetVoice(FeeVoice(SFS2_ENTRY_FEES[work.rank]), 1, 0);
    EVT_AT_SetVoice("0100I007", 1, 80); // @text EN: "Anyone strong enough\nto challenge this man?" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    EVT_AT_SetCam(1403, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    return 160;
}
std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6852, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    EVT_AT_SetVoice("0100I012", 1, 60); // @text EN: "Good." | JA: "よし。決まりだ。"
    EVT_AT_SetCam(130, SFS2_BASE, SFS2_BASE, 0, 0, 5);
    EVT_AT_SetCam(140, SFS2_BASE, SFS2_BASE, 0, 0, 58);
    SetAudienceState(work, 4, 60);
    return 156;
}
std::int32_t DeclineConversation(void *) {
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
    default:
        break;
    }
    EVT_AT_SetCam(132, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    return 60;
}
std::int32_t InsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const work = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->delay = 60;
    EVT_AT_SetCam(134, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    return 150;
}
std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6854, 0, 1, 0, 100);
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 0, 0);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0, 1.1f, 0x40000000U, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(fighter), FacialExpressionId::Suffer, 1, 1);
    EVT_AT_SetPos(fighter, SFS2_BASE, SFS2_FIGHTER_REPEAT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6850, 0, .80000001f, 0x50000000U, 0);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(SFS2_ENTRY_FEES[work.rank]), 105);
    EVT_AT_SetVoice("0100I016", 1, 35); // @text EN: "Good match.\nHere you go." | JA: "やるじゃないか、兄ちゃん。\nほらよ。"
    EVT_AT_SetVoice(firstStoryMatch ? "0100V006" : "0100V007", 1, 120); // @text 0100V006 EN: "I've been defeated.\nYou have much kung fu." | JA: "おそれいった。なかなかの功夫を\n積んでいるようだ。" ; 0100V007 EN: "Defeated=@\nMy time has passed=@" | JA: "私の負けだ。\n後生おそるべし、か。"
    EVT_AT_SetCam(200, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    EVT_AT_SetCam(210, SFS2_BASE, SFS2_BASE, 0, 0, 110);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    EV_LoadAudio("0100E000", 0);
    return 285;
}
std::int32_t PlayVictoryAppearance() {
    EV_EnableCharacterDisplay(SFS2_CHN);
    EV_EnsureCharacterDraw(SFS2_CHN);
    EV_SetCharacterAlphaEnabled(SFS2_CHN, false);
    EVT_AT_SetPos(SFS2_CHN, STREET_FIGHT_INVALID, SFS2_VICTORY_APPEARANCE_POSITION, 0);
    EVT_AT_SetAngY(SFS2_CHN, STREET_FIGHT_INVALID, 7, 0);
    EVT_AT_SetMot(SFS2_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    EVT_AT_SetCam(9901, SFS2_CHN, SFS2_CHN, 0, 0, 0);
    return 60;
}
std::int32_t VictoryAppearanceConversation(void *) {
    return PlayVictoryAppearance();
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFS2_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFS2_BASE, 180, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6853, 0, 1, 0, 110);
    EVT_AT_SetPos(SFS2_CYC, SFS2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFS2_CYC, SFS2_BASE, 0, 1);
    EVT_AT_SetMot(SFS2_CYC, 6855, 0, 1.1f, 0, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(fighter), FacialExpressionId::Smile, 1, 1);
    EVT_AT_SetPos(fighter, SFS2_BASE, {-1.488f, .5f, -1.1f}, 1);
    EVT_AT_SetAngY(fighter, SFS2_BASE, 17, 1);
    EVT_AT_SetMot(fighter, 6850, 0, .80000001f, 0x50000000U, 0);
    EVT_AT_SetVoice("0100I019", 1, 35); // @text EN: "Too bad.\nBut your moves were notable." | JA: "残念だったな。\nでもいい動きしてたぜ。"
    if (firstStoryMatch) {
        EVT_AT_SetVoice("0100V008", 1, 155); // @text EN: "Youth always relies on mere force." | JA: "ホッホッホッ…\n若いうちはみな剛の力に頼る。"
        EVT_AT_SetVoice("0100V009", 1, 300); // @text EN: "But you cannot split a feather with an ax." | JA: "じゃが、オノで羽毛は断てぬよ。"
    } else {
        EVT_AT_SetVoice("0100V010", 1, 155); // @text EN: "Your moves are still not refined." | JA: "まだまだ動きが荒い。"
        EVT_AT_SetVoice("0100V011", 1, 240); // @text EN: "You need more kung fu." | JA: "もっと功夫を積みなされ。"
    }
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFS2_ENTRY_FEES[work.rank]), 140);
    EVT_AT_SetCam(300, SFS2_BASE, SFS2_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, SFS2_BASE, SFS2_BASE, 0, 0, 120);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    EV_LoadAudio("0100E000", 0);
    return firstStoryMatch ? 410 : 345;
}
void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 714, 1);
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
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_ResetFreeTalkFlags();
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(fighter), FacialExpressionId::Angry, 1, 1);
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    work.state = work.battleStatus = work.auxiliaryState = 0;
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(MainCallback);
}
void RetryChoiceCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
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
    AT_SetLocalBaseChara(SFS2_STAG, STREET_FIGHT_INVALID);
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFS2_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFS2_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFS2_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work.hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFS2_RETRY_AREA, 1);
    else if (work.reserved178)
        EV_scenQueueLoad(g_actNumber, SFS2_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    flddSetPointFlagOnGrid(SFS2_GRID_ORIGIN, 1);
    flddSetPointFlagOnGrid(SFS2_BTC1, 0);
    if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED))
        SetNewNpcBatchCountOverride(NpcFillType::FrontFill, 6);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = SFS2_VNE;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->package = SFS2_RESOURCE_PACKAGE;
    work->battleFrames = 53;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFS2_CHARACTERS);
    work->audienceTask = nullptr;
    work->hudEnabled = work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->reserved178 = 0;
    return work;
}
void LoadNormalResources(StreetFightEventWork &work) {
    work.motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFB"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFB"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFB"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFS2_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
    flddSetPointFlagOnGrid(SFS2_BTC1, 1);
    flddSetPointFlagOnGrid(SFS2_GRID_ORIGIN, 0);
    if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED))
        SetNewNpcBatchCountOverride(NpcFillType::FrontFill, 10);
}
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = start;
    LoadNormalResources(*work);
    work->fadeFrames = static_cast<std::uint8_t>(start->scene);
}
void Init() {
    qtePhase = 0;
    tauntPollFrames = 20;
    idleVoiceFrames = 0;
    lastSpokenAnimation = 0;
    firstStoryMatch = 1;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

} // namespace
const ScriptAreaDefinition areaSfs2ScriptDefinition{.areaId = STREET_FIGHT_SFS2, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
#endif
