#include "s2/areas/common/et_street_debug.h"

static Vector3 debugOpponentOrigin = STREET_FIGHT_DEBUG_ORIGIN;
static Vector3 debugExitPosition = STREET_FIGHT_DEBUG_EXIT_POSITION;
static Vector3i debugExitRotation = STREET_FIGHT_DEBUG_EXIT_ROTATION;
static short debugPrizes[11] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
static signed char debugWinIncrements[11] = {0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
static shen_name debugCharacters[3] = {STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, STREET_FIGHT_INVALID};
static int debugOpponentCount;
static shen_name debugLoadCharacters[2];

void QueueDebug(StreetFightStartParameters *parameters)
{
    HLibTask *task;
    StreetFightEventWork *work;
    if (parameters->signal)
        *parameters->signal = 1;
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(DebugMainCallback, 5, 11, sizeof(StreetFightEventWork), TASK_EVNT);
    work = InitializeDebug(task);
    work->startParameters = parameters;
    LoadDebugResources(work);
    EventDebugRegister(3, DebugCleanupCallback, work, 36000);
}

StreetFightEventWork *InitializeDebug(HLibTask *task)
{
    StreetFightEventWork *work = (StreetFightEventWork *)FTBL.HLib_GetTaskParameter(task);
    const StreetFightFighterDefinition *definition;
    int index;
    work->task = task;
    index = 0;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = STREET_FIGHT_INVALID;
    work->area = STREET_FIGHT_DEBUG_AREA;
    work->battleFrames = 50;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = FTBL.GetNpcBatchCountOverride(30);
    work->audienceTask = 0;
    work->completedOpponents = 0;
    work->hudEnabled = 0;
    work->fighterIndex = 0;
    definition = (const StreetFightFighterDefinition *)FTBL.ConversationResultNullsub();
    work->fighterDefinitions[0] = definition;
    work->fighters[index] = definition->character;
    debugOpponentCount = 5;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, debugCharacters);
    return work;
}

static const Vector3 debugPlayerPosition = {-3.3f, STREET_FIGHT_DEBUG_PLAYER_Y, -5.5f};
static const Vector3 debugBookiePosition = {-1.864f, 0.0f, -1.8f};

void LoadDebugResources(StreetFightEventWork *work)
{
    work->motions[0] = 0;
    FTBL.EV_FreeAuthorPak(1, STREET_FIGHT_DEBUG_SCENE);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ENMY", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)STREET_FIGHT_PACKAGE, 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, STREET_FIGHT_PACKAGE, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, STREET_FIGHT_PACKAGE, 0, 0);
    FTBL.CreateEventCharacter(3, 0, STREET_FIGHT_DEBUG_BASE);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene(STREET_FIGHT_DEBUG_CUTSCENE);
    FTBL.EV_LoadAudio(STREET_FIGHT_DEBUG_CUTSCENE, 0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
}

void DebugCleanupCallback(void *data)
{
    CleanupDebug((StreetFightEventWork *)data);
}

void CleanupDebug(StreetFightEventWork *work)
{
    StreetFightStartParameters *parameters = work->startParameters;
    int i;
    EventDebugUnregister(3);
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    FTBL.SetScenarioCharacterKillMode(STREET_FIGHT_DEBUG_BASE, 1);
    FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &debugExitPosition);
    FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &debugExitRotation, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
    if (work->hudEnabled)
        FTBL.ToggleStreetFighterUI();
    FTBL.GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (i = 0; work->motions[i]; i++)
        FTBL.DeleteMotion((void *)work->motions[i]);
    FTBL.EV_FreeAuthorPak(0, STREET_FIGHT_DEBUG_SCENE);
    if (!(parameters->flags & 1))
        FTBL.EV_ControlPackageResources(0, 6, STREET_FIGHT_PACKAGE, 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "ENMY", 0, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.camCtrlSetState(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    if (parameters->signal)
        *parameters->signal = 0;
}

void PlaceDebugCharacters(StreetFightEventWork *work)
{
    shen_name fighter = work->fighters[work->fighterIndex];
    SetStreetFightCharacterVisible(STREET_FIGHT_BOOKIE, 1);
    SetStreetFightCharacterVisible(fighter, 1);
    work->playerPosition.x = debugPlayerPosition.x;
    work->playerPosition.y = debugPlayerPosition.y;
    work->playerPosition.z = debugPlayerPosition.z;
    work->bookiePosition.x = debugBookiePosition.x;
    work->bookiePosition.y = debugBookiePosition.y;
    work->bookiePosition.z = debugBookiePosition.z;
    EVT_AT_SetPos(STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_INVALID, STREET_FIGHT_DEBUG_BASE_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_INVALID, STREET_FIGHT_DEBUG_BASE_ANGLE, 0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE);
    FTBL.AT_SetLocalBaseChara(fighter, STREET_FIGHT_DEBUG_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, -9.2f, -0.5f, -10.4f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 225.0f, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 1.5f, 0.0f, -1.0f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 0);
    FTBL.SetScenarioCharacterType(STREET_FIGHT_RYO, 4);
    FTBL.SetScenarioCharacterType(STREET_FIGHT_BOOKIE, 4);
    FTBL.SetScenarioCharacterType(fighter, 4);
}

void StreetFightCharacterLoadCallback(void *data)
{
    StreetFightCharacterLoadWork *work = (StreetFightCharacterLoadWork *)data;
    shen_name character;
    int i, result;
    switch (work->state) {
    case 0:
        for (i = 0; work->characters[i] != STREET_FIGHT_INVALID; i++)
            FTBL.CheckWarmStartNpcRoute(work->characters[i]);
        work->index = 0;
        work->state++;
        break;
    case 1:
        if (work->characters[work->index] != STREET_FIGHT_INVALID) {
            character = work->characters[work->index];
            FTBL.DebugLogToFileRemoved("[%c%c%c%c] ", character & 0xff, character >> 8 & 0xff, character >> 16 & 0xff, character >> 24);
            if (FTBL.DoesScenarioCharacterExist(work->characters[work->index])) {
                FTBL.DebugLogToFileRemoved("Exist\n");
                work->state = 3;
            } else {
                FTBL.DebugLogToFileRemoved("Loading\n");
                work->state = 2;
            }
        } else {
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyTaskInternal(work->task);
        }
        break;
    case 2:
        result = FTBL.npcStreamOrLoadHuman(work->characters[work->index]);
        switch (result) {
        case 1:
            work->state++;
            break;
        case -4:
        case -3:
            FTBL.EV_ResetFreeTalkFlags();
            break;
        }
        if (result < 0)
            EventDebugPrintf(4, 10, "NPC LOAD ERR:%d\n", result);
        break;
    case 3:
        FTBL.DebugLogToFileRemoved("Completed.\n");
        character = work->characters[work->index];
        FTBL.SetRefBoardParamsForCharaById(character, 1);
        FTBL.EV_SetCharacterAlphaEnabled(character, 0);
        FTBL.EV_SetCharaControllerFlags(character, 2, 0);
        FTBL.EV_SetCharaControllerFlags(character, 3, 0);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.EV_SetCharaControllerFlags(character, 1, 0);
        FTBL.AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
        FTBL.figpSetFlagsBlendForBlur(character, 2, 1);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(character), 1);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 0, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 3, 2);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 3);
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), 7, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 66, 2, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 69, 2, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 71, 2, 0);
        SetStreetFightCharacterVisible(character, 1);
        FTBL.SetScenarioCharacterType(character, 4);
        FTBL.AT_SetLocalBaseChara(character, STREET_FIGHT_DEBUG_BASE);
        work->index++;
        work->state = 1;
        break;
    }
}

void QueueStreetFightCharacterLoad(HLibTask *parent, const shen_name *characters)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightCharacterLoadCallback, 6, 11, sizeof(StreetFightCharacterLoadWork), TASK_EVNT);
    StreetFightCharacterLoadWork *work = (StreetFightCharacterLoadWork *)FTBL.HLib_GetTaskParameter(task);
    FTBL.HLib_SetupTimedTask(parent, 0);
    work->task = task;
    work->parent = parent;
    work->characters = characters;
    work->index = 0;
    work->state = 0;
}

void ReleaseStreetFightCharacter(shen_name character)
{
    FTBL.DebugLogToFileRemoved("[%c%c%c%c] Free.\n", character & 0xff, character >> 8 & 0xff, character >> 16 & 0xff, character >> 24);
    FTBL.npcFree(character);
    if (FTBL.IsNpcRouteLoadedOrVisible(character))
        FTBL.npcLockUnlock(character);
}

void DebugMainCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    const StreetFightFighterDefinition *definition = work->fighterDefinitions[work->fighterIndex];
    Vector3 position, bookie, savedPlayer, savedBookie;
    unsigned int choice;
    EventDebugPrintf(4, 4, "%d\n", work->state);
    switch (work->state) {
    case 0:
        debugLoadCharacters[0] = fighter;
        debugLoadCharacters[1] = STREET_FIGHT_INVALID;
        QueueStreetFightCharacterLoad(work->task, debugLoadCharacters);
        work->state = 1;
        break;
    case 1:
        EventDebugCheck(3, 4);
        PlaceDebugCharacters(work);
        work->audienceTask = QueueStreetFightAudience(30, 0);
        AddStreetFightAudienceFighter(work->audienceTask, STREET_FIGHT_RYO);
        AddStreetFightAudienceFighter(work->audienceTask, fighter);
        FTBL.GetCharaPosition(STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_INVALID, &position, 0);
        position.y -= 0.5f;
        SetStreetFightAudienceCentre(work->audienceTask, &position);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, &savedPlayer, 0);
        FTBL.GetCharaPosition(STREET_FIGHT_BOOKIE, STREET_FIGHT_INVALID, &savedBookie, 0);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &work->playerPosition);
        FTBL.EV_AT_SetPos(STREET_FIGHT_BOOKIE, 0x38000000, &work->bookiePosition);
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, &position, 0x40000000);
        FTBL.GetCharaPosition(STREET_FIGHT_BOOKIE, STREET_FIGHT_INVALID, &bookie, 0x40000000);
        SetStreetFightAudienceCorridor(work->audienceTask, &position, &bookie);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x38000000, &savedPlayer);
        FTBL.EV_AT_SetPos(STREET_FIGHT_BOOKIE, 0x38000000, &savedBookie);
        SetStreetFightAudienceRadius(work->audienceTask, 5.4f, 2.0f);
        if (definition->blurModel)
            FTBL.LoadBlurModelForCharacter_0(fighter, definition->blurModel, 1.0f);
        else
            FTBL.LoadBlurModelForCharacter_0(fighter, 0, 0.2f);
        work->state = 2;
        break;
    case 2:
        EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, debugOpponentOrigin.x - 1.0f, debugOpponentOrigin.y + 0.5f, debugOpponentOrigin.z, 0);
        work->state = 3;
        break;
    case 3:
        FTBL.EV_NPC_NewLoadSw_0(1);
        if (IsStreetFightAudienceArranged(work->audienceTask)) {
            QueueStreetFightSkipWait(StartStreetFightBookieConversation, work, 1);
            work->state = 4;
            break;
        }
    case 4:
        EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 0xffff, 0x7fff, 0xffff, 2, 0);
        ControlStreetFightActionChoice(0);
        work->state = 5;
        break;
    case 5:
        ReadStreetFightCharacterPosition(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 16);
        choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        ClearStreetFightLook(STREET_FIGHT_RYO, 15);
        if (choice == 38) {
            if (FTBL.GetPlayerStat(2) < 20)
                work->state = 8;
            else
                work->state = 6;
        } else
            work->state = 7;
        break;
    case 6:
        QueueStreetFightSkipWait(AcceptStreetFightConversation, work, 1);
        work->state = 9;
        break;
    case 7:
        QueueStreetFightSkipWait(DeclineStreetFightConversation, work, 1);
        work->state = 23;
        break;
    case 8:
        QueueStreetFightSkipWait(InsufficientStreetFightFundsConversation, work, 1);
        work->state = 23;
        break;
    case 9:
        FTBL.HLib_SetCurrentTaskTo(DebugBattleEntryCallback);
        work->state = 12;
        break;
    case 12:
        if (work->battleStatus == 3)
            work->state = 13;
        else if (work->battleStatus == 2)
            work->state = 17;
        else
            work->state = 17;
        break;
    case 13:
        work->completedOpponents++;
        if (work->completedOpponents > 10)
            FTBL.DebugLogToFileRemoved("ERROR : win_times %d\n", work->completedOpponents);
        FTBL.AddStreetFightProgress(debugWinIncrements[work->completedOpponents]);
        QueueStreetFightSkipWait(StreetFightVictoryConversation, work, 1);
        work->state = 14;
        break;
    case 14: {
        const StreetFightFighterDefinition *next;
        shen_name previous = fighter;
        FTBL.ConversationNullsubA();
        next = (const StreetFightFighterDefinition *)FTBL.ConversationResultNullsub();
        work->fighterDefinitions[0] = next;
        work->fighters[0] = next->character;
        fighter = work->fighters[0];
        FTBL.EV_NPC_NewLoadSw_0(0);
        if (previous != STREET_FIGHT_INVALID) {
            ClearStreetFightAudienceFighters(work->audienceTask);
            ReleaseStreetFightCharacter(previous);
        }
        if (fighter != STREET_FIGHT_INVALID) {
            debugLoadCharacters[0] = fighter;
            debugLoadCharacters[1] = STREET_FIGHT_INVALID;
            QueueStreetFightCharacterLoad(work->task, debugLoadCharacters);
        }
        work->state = 15;
        break;
    }
    case 15:
        FTBL.EV_NPC_NewLoadSw_0(1);
        AddStreetFightAudienceFighter(work->audienceTask, STREET_FIGHT_RYO);
        AddStreetFightAudienceFighter(work->audienceTask, fighter);
        if (definition->blurModel)
            FTBL.LoadBlurModelForCharacter_0(fighter, definition->blurModel, 1.0f);
        else
            FTBL.LoadBlurModelForCharacter_0(fighter, 0, 0.2f);
        QueueStreetFightSkipWait(StreetFightNextOpponentConversation, work, 1);
        work->state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 0xffff, 0x7fff, 0xffff, 2, 0);
        ControlStreetFightActionChoice(0);
        work->state = 19;
        break;
    case 17:
        QueueStreetFightSkipWait(StreetFightDefeatConversation, work, 1);
        work->state = 23;
        break;
    case 18:
        QueueStreetFightSkipWait(StreetFightWithdrawalConversation, work, 1);
        work->state = 23;
        break;
    case 19:
        ReadStreetFightCharacterPosition(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 16);
        choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        ClearStreetFightLook(STREET_FIGHT_RYO, 15);
        work->state = choice == 38 ? 20 : 21;
        break;
    case 20:
        QueueStreetFightSkipWait(DebugContinueConversation, work, 1);
        work->state = 22;
        break;
    case 22:
        if (work->completedOpponents >= debugOpponentCount) {
            QueueStreetFightSkipWait(DebugCompletedConversation, work, 1);
            work->state = 23;
        } else {
            QueueStreetFightSkipWait(StreetFightNextMatchConversation, work, 1);
            work->state = 9;
        }
        break;
    case 21:
        QueueStreetFightSkipWait(DebugStopConversation, work, 1);
        work->state = 23;
        break;
    case 23:
        work->state = 24;
        break;
    case 24:
        if (!FTBL.filmCheckFadeTrackColour() && !FTBL.GetShouldRenderIcons(4))
            work->state = 25;
        break;
    case 25:
        ReleaseStreetFightAudience(work->audienceTask);
        CleanupDebug(work);
        work->state++;
        break;
    }
}

int StartStreetFightBookieConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.BeginSecondarySampleExecution(0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 225.0f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 225.0f, 200);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 199);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 16, 800, 605);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_BOOKIE, STREET_FIGHT_RYO, 15, 877, 535);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, -0.05f, -1.3f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 0);
    EVT_AT_SetMot(fighter, 6875, 0.0f, 0.8f, 0x40000000, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    EVT_AT_SetVoice("0100B038", 50);
    EVT_AT_SetVoice("0100B067", 132);
    EVT_AT_SetVoice("0100B068", 240);
    EVT_AT_SetVoice("0100B069", 330);
    EVT_AT_SetVoice("0100B070", 412);
    if (g_currentMap == SHEN_NAME4('Q', 'S', 'F', 'B'))
        EVT_AT_SetCam(8099, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    else
        EVT_AT_SetCam(100, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    EVT_AT_SetCam(2004, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 120);
    EVT_AT_SetCam(2006, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 250);
    EVT_AT_SetCam(120, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 350);
    return 420;
}

int AcceptStreetFightConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6895, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 0, 156);
    StreetFightNullsub(STREET_FIGHT_BOOKIE, STREET_FIGHT_RYO, 6, 0, 156);
    StreetFightNullsub(fighter, STREET_FIGHT_RYO, 6, 0, 156);
    EVT_AT_SetVoice("0100A022", 30);
    EVT_AT_SetVoice("0100B055", 60);
    EVT_AT_SetPoint(STREET_FIGHT_BOOKIE, STREET_FIGHT_RYO, 5, 3, 0, 50, 156, 0);
    EVT_AT_SetPoint(fighter, STREET_FIGHT_RYO, 5, 1, 0, 70, 156, 0);
    EVT_AT_SetCam(130, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    EVT_AT_SetCam(141, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 28);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 126);
    return 156;
}

int DeclineStreetFightConversation(void *data)
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
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 120);
    return 150;
}

int InsufficientStreetFightFundsConversation(void *data)
{
    EVT_AT_StartMoneyCount(0, 60);
    EVT_AT_SetVoice("0100A037", 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 120);
    return 150;
}

void DebugBattleEntryCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 0.0f, 0.0f, -1.1f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, -3.3f, -0.5f, -5.5f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, 0.0f, 1.1f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    FTBL.HLib_SetCurrentTaskTo(DebugBattlePreEventCallback);
}

void DebugBattlePreEventCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_LoadAudio("0100E000", 1);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    StreetFightNullsub(STREET_FIGHT_RYO, fighter, 16, 0, 260);
    StreetFightNullsub(STREET_FIGHT_BOOKIE, STREET_FIGHT_RYO, 15, 0, 260);
    StreetFightNullsub(fighter, STREET_FIGHT_RYO, 16, 0, 260);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6880, 0.0f, 1.0f, 0, 50);
    EVT_AT_SetVoice("0100B072", 0);
    EVT_AT_SetCam(2001, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(DebugBattlePreInitCallback);
    FTBL.HLib_SetupTaskDelay(60);
}

void DebugBattlePreInitCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 position;
    Vector3i rotation;
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
    FTBL.HLib_SetCurrentTaskTo(DebugBattleStartCallback);
    FTBL.HLib_SetupTaskDelay(10);
}

void DebugBattleStartCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    const StreetFightFighterDefinition *definition = work->fighterDefinitions[work->fighterIndex];
    work->hudEnabled = 1;
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.EV_vfBattleUtil(3, 8, 1);
    FTBL.vfmDispatchControl(2, 0, STREET_FIGHT_PACKAGE, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 0, 0);
    FTBL.vfmDispatchControl(2, 0, "ENMY", 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(fighter), 0, 1, 1, definition->hitPoints, definition->fightStyle);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(fighter), 0, 0x8000000, definition->difficulty);
    FTBL.EV_vfBattleUtil(3, 1, 1);
    FTBL.vfCopyPosFromEntity(0, 0, debugOpponentOrigin.x, debugOpponentOrigin.z, 2.5f);
    FTBL.vfmQueueFightStart(2, 99, 1.0f, 8, 30);
    work->hudEnabled = 0;
    FTBL.GetSetFightActorByte(1, FTBL.ctrlGetCharaByName(fighter), definition->guardMode);
    FTBL.GetSetFightActorFloat(1, FTBL.ctrlGetCharaByName(fighter), definition->strikeScale);
    FTBL.GetSetFightActorFloat(3, FTBL.ctrlGetCharaByName(fighter), definition->guardScale);
    FTBL.GetSetFightActorFloat(5, FTBL.ctrlGetCharaByName(fighter), definition->guardScale);
    FTBL.HLib_SetCurrentTaskTo(DebugBattleReadyGoCallback);
}

void DebugBattleReadyGoCallback(void *data)
{
    EVT_AT_SetVoice("0100B073", 0);
    FTBL.HLib_SetCurrentTaskTo(DebugBattleStartFightCallback);
    FTBL.HLib_SetupTaskDelay(40);
}

void DebugBattleStartFightCallback(void *data)
{
    FTBL.EV_vfBattleUtil(4, 1, 0);
    FTBL.HLib_SetCurrentTaskTo(DebugBattleWaitCallback);
}

void DebugBattleWaitCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (FTBL.IsFightActive())
        return;
    if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) == 7) {
        work->battleStatus = 7;
        EVT_AT_SetVoice("0100B075", 0);
    } else if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO)) == 3) {
        work->battleStatus = 3;
        EVT_AT_SetVoice("0100B074", 0);
    } else {
        work->battleStatus = 2;
        EVT_AT_SetVoice("0100B074", 0);
    }
    FTBL.HLib_SetCurrentTaskTo(DebugBattleWaitFadeCallback);
}

void DebugBattleWaitFadeCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (!FTBL.IsFightActive()) {
        FTBL.ProcessAicaCommand(0xaa0, 30);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(DebugBattleEndCallback);
        FTBL.HLib_SetupTaskDelay(30);
    }
    work->frameCounter = 0;
}

void DebugBattleEndCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    FTBL.EV_LoadAudio("0100E000", 0);
    FTBL.BeginSecondarySampleExecution(0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE);
    FTBL.AT_SetLocalBaseChara(work->fighters[work->fighterIndex], STREET_FIGHT_DEBUG_BASE);
    FTBL.EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    FTBL.EV_SetCharacterAlphaEnabled(STREET_FIGHT_BOOKIE, 0);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    FTBL.HLib_SetCurrentTaskTo(DebugMainCallback);
    work->frameCounter = 0;
}

int StreetFightVictoryConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -0.588f, 0.0f, -1.9f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 0);
    EVT_AT_SetMot(fighter, 6851, 0.0f, 1.0f, 0x50000000, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    QueueEventMessage("胴元「兄ちゃん、やるねえ。」", 6, 60);
    EVT_AT_SetCam(2007, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 5);
    EVT_AT_SetCam(210, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 118);
    return 120;
}

int StreetFightNextOpponentConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 10, 530);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -0.588f, 0.0f, -1.9f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 0);
    EVT_AT_SetMot(fighter, 6875, 0.0f, 0.8f, 0x50000000, 0);
    QueueEventMessage("胴元「まだまだイケそうだな。」", 1, 59);
    QueueEventMessage("胴元「もうひと勝負、受けねえか？」", 60, 60);
    switch (work->completedOpponents) {
    case 1:
        QueueEventMessage("胴元「勝ったら４０ドル出すぜ。」", 120, 60);
        break;
    case 2:
        QueueEventMessage("胴元「勝ったら６０ドル出すぜ。」", 120, 60);
        break;
    case 3:
        QueueEventMessage("胴元「勝ったら８０ドル出すぜ。」", 120, 60);
        break;
    case 4:
        QueueEventMessage("胴元「勝ったら１００ドル出すぜ。」", 120, 60);
        break;
    case 5:
        QueueEventMessage("胴元「勝ったら１２０ドル出すぜ。」", 120, 60);
        break;
    }
    return 180;
}

int StreetFightDefeatConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 10, 435);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 1);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 1);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -0.988f, 0.0f, -1.2f, 1);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 1);
    EVT_AT_SetMot(fighter, 6898, 0.0f, 1.0f, 0x40000000, 0);
    FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(fighter), 1.0f, 0);
    EVT_AT_SetMot(fighter, 6898, 1.0f, 1.0f, 0x40000000, 60);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    EVT_AT_SetVoice("0100B077", 35);
    EVT_AT_SetCam(2008, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 300);
    EVT_AT_StartMoneyCount(-20, 180);
    EVT_AT_MenCtrl(40, 0, 0, 0, 0, 255, 0, 0, 0, 250);
    return 290;
}

int StreetFightWithdrawalConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 10, 435);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 1);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 1);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -0.988f, 0.0f, -1.2f, 1);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 17.0f, 1);
    EVT_AT_SetMot(fighter, 6898, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
    EVT_AT_SetVoice("0100B078", 35);
    EVT_AT_SetVoice("0100B079", 140);
    EVT_AT_SetCam(107, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 300);
    EVT_AT_MenCtrl(40, 0, 0, 0, 0, 255, 0, 0, 0, 250);
    return 280;
}

int DebugContinueConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 0.0f, -0.05f, -1.3f, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 17.0f, 60);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, -10000.0f, -1.3f, 0);
    if (work->completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -3.3f, STREET_FIGHT_DEBUG_PLAYER_Y, -5.5f, 60);
        EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 225.0f, 60);
    }
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    QueueEventMessage("涼「ああ。」", 0, 30);
    QueueEventMessage("胴元「よし待ってな。」", 30, 30);
    QueueEventMessage("胴元「さあさあ、この兄ちゃん、＆まだ"
                      "若えが、けっこうなツワモノだ。」",
        60, 90);
    QueueEventMessage("胴元「さあ誰か、こいつと勝負するやつはいな"
                      "いか？」",
        150, 60);
    EVT_AT_SetCam(2006, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 150);
    return 210;
}

int DebugCompletedConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 0.0f, -0.05f, -1.3f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 17.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 10, 530);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, -10000.0f, -1.3f, 0);
    QueueEventMessage("胴元「さあ誰か！！」", 0, 30);
    QueueEventMessage("胴元「・・・っと、いねえようだな。」", 60, 60);
    QueueEventMessage("胴元「ほら、兄ちゃん、賞金だ。」", 120, 60);
    EVT_AT_SetCam(2007, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 5);
    EVT_AT_StartMoneyCount(debugPrizes[work->completedOpponents], 140);
    EVT_AT_MenCtrl(40, 0, 0, 0, 0, 255, 0, 0, 0, 180);
    return 240;
}

int StreetFightNextMatchConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, -3.3f, STREET_FIGHT_DEBUG_PLAYER_Y, -5.5f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_DEBUG_BASE, 225.0f, 0);
    QueueEventMessage("敵「ワシがやろう。」", 0, 60);
    EVT_AT_SetVoice("0100B071", 60);
    EVT_AT_SetCam(130, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 0);
    return 120;
}

int DebugStopConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, work->playerPosition.x, work->playerPosition.y, work->playerPosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_DEBUG_BASE, 180.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0f, 1.0f, 0, 0);
    StreetFightNullsub(STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, 6, 10, 530);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, work->bookiePosition.x, work->bookiePosition.y, work->bookiePosition.z, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_DEBUG_BASE, 0.0f, 0);
    EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 6893, 0.0f, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_DEBUG_BASE, 0.0f, -10000.0f, -1.3f, 0);
    QueueEventMessage("涼「いや、もうよしとくよ。」", 0, 60);
    QueueEventMessage("胴元「そうか・・・。」", 60, 60);
    QueueEventMessage("胴元「しゃあねえ、ほら、賞金だ。」", 120, 60);
    EVT_AT_SetCam(2007, STREET_FIGHT_DEBUG_BASE, STREET_FIGHT_DEBUG_BASE, 0, 0, 5);
    EVT_AT_StartMoneyCount(debugPrizes[work->completedOpponents], 140);
    EVT_AT_MenCtrl(40, 0, 0, 0, 0, 255, 0, 0, 0, 180);
    return 240;
}
