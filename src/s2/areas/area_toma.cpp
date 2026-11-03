#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_toma.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void TomaMain(const TomaRequest *request)
{
    shen_name map;
    unsigned int argument;
    int selector;
    const TomaMapEntry *row;
    while (request) {
        map = request->map;
        argument = request->argument;
        selector = request->selector;
        row = FindTomaMap(map);
        if ((unsigned int)(selector - 1) <= 1 && !row)
            break;
        tomaRaceEntry = 0;
        switch (selector) {
        case 0:
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 1:
            QueueTomaWaitPackage(map, argument);
            FTBL.srchReset();
            FTBL.SetPlayerControlEnabled(0);
            FTBL.SetLoadingBusy();
            break;
        case 2:
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 100:
            QueueTomaFirstTalk();
            break;
        case 101:
            QueueTomaTalk();
            break;
        case 102:
            QueueTomaHaveFirstTalk();
            break;
        case 103:
            QueueTomaHaveTalk();
            break;
        case 200:
            QueueTomaBye();
            break;
        }
        break;
    }
}

int TomaUpdateAreaItems(shen_name map, shen_name motion)
{
    int result = 0;
    const TomaMapEntry *row;
    do {
        row = FindTomaMap(map);
        if (!row)
            break;
        TomaUpdateItems(row, motion);
        result = 1;
    } while (0);
    return result;
}

static TomaMapEntry tomaMaps[] = {
    {TOMA_ARZ0, TOMA_LYH,
     {{SHEN_NAME4('M', '4', '0', '1'), 0, 65, 8},
      {SHEN_NAME4('M', '4', '0', '2'), 0, 65, 8},
      {SHEN_NAME4('M', 'E', '0', '3'), 0, 64, 8},
      {SHEN_NAME4('M', 'E', '0', '4'), 0, 64, 8},
      {SHEN_NAME4('M', 'E', '0', '5'), 0, 63, 8},
      {SHEN_NAME4('M', 'E', '0', '6'), 0, 63, 8}}},
    {TOMA_WET0, TOMA_XGH,
     {{SHEN_NAME4('M', '4', '0', '1'), 0, 65, 8},
      {SHEN_NAME4('M', '4', '0', '2'), 0, 65, 8},
      {SHEN_NAME4('M', 'E', '0', '3'), 0, 64, 8},
      {SHEN_NAME4('M', 'E', '0', '4'), 0, 64, 8},
      {SHEN_NAME4('M', 'E', '0', '5'), 0, 63, 8},
      {SHEN_NAME4('M', 'E', '0', '6'), 0, 63, 8}}},
    {0, 0, {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
};

const TomaMapEntry *FindTomaMap(shen_name map)
{
    const TomaMapEntry *row = 0;
    int index;
    for (index = 0; tomaMaps[index].map; index++) {
        if (tomaMaps[index].map == map) {
            row = &tomaMaps[index];
            break;
        }
    }
    return row;
}

void TomaUpdateItems(const TomaMapEntry *row, shen_name motion)
{
    int index;
    if (!row)
        return;
    for (index = 0; index < 6; index++) {
        if (!row->offers[index].motion)
            continue;
        if (motion && motion != row->offers[index].motion)
            continue;
        if (FTBL.GetItemState(row->offers[index].item)) {
            FTBL.SetDisplayFlagForProp(row->offers[index].motion, 0);
            if (row->offers[index].target)
                FTBL.srchSetTargetEnabled(row->offers[index].target, 0);
        } else {
            FTBL.SetDisplayFlagForProp(row->offers[index].motion, 1);
            if (row->offers[index].target)
                FTBL.srchSetTargetEnabled(row->offers[index].target, 1);
        }
    }
}

int tomaRaceEntry;
#else
#define SHENMUE_AREA_LOOP_SOUND
#define SHENMUE_AREA_LOOP_MOTION_FRAMES
#define SHENMUE_AREA_LOOP_VOICE_REVISION 2
#include "s2/areas/area_loop_dc.c"

static int tomaRewards[] = {4075, 4076, 4074, 4075, 4076, 4083, 4084, 4085, 4086, 4050, 4058, 4105,
                            4019, 4122, 4068, 4069, 4070, 4162, 4163, 4164, 4165, 4166, 4167};
static char tomaPackage[] = "TOMA";
static char tomaConversation[] = "0110";
static char tomaPlusPackage[] = "TOMAPLUS";
static char tomaRaceConversation[] = "0122";

void QueueTomaWaitPackage(shen_name map, unsigned int argument)
{
    HLibTask *task;
    TomaWork *work;
    while ((task = FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackage, 4, 11, sizeof(TomaWork), TASK_EVNT)) &&
           (work = (TomaWork *)FTBL.HLib_GetTaskParameter(task))) {
        work->parent = FTBL.HLib_GetCurrentTask();
        work->task = task;
        work->flags = 1;
        work->map = map;
        work->argument = argument;
        FTBL.EV_NPC_NewLoadSw_0(0);
        task = 0;
        FTBL.EV_SetPlayerFlag(12, 2095, 1);
        break;
    }
    if (task)
        FTBL.HLib_DestroyTask(task);
}

void TomaWaitPackage(void *data)
{
    TomaWork *work = (TomaWork *)data;
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)tomaPackage, 0)) {
        const TomaMapEntry *row;
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.HLib_SetCurrentTaskTo(TomaStart);
        row = FindTomaMap(work->map);
        if (row) {
            shen_name names[2];
            FTBL.HLib_SetupTaskDelay(-1);
            names[0] = row->npc;
            names[1] = 0;
            QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
        }
    }
}

void TomaStart(void *data)
{
    TomaWork *work = (TomaWork *)data;
    const TomaMapEntry *row;
    Vector3 ryoPosition, npcPosition;
    Vector3i ryoRotation, npcRotation;
    shen_name npc;
    shen_name prop = TOMA_PROP;
    while (!FTBL.filmCheckFadeTrackColour() && work && (row = FindTomaMap(work->map)) != 0) {
        FTBL.HLib_SetCurrentTaskTo(TomaDone);
        FTBL.HLib_SetupTaskDelay(-1);
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        npc = row->npc;
        FTBL.GetCharaPosition(TOMA_PLAYER, -1, &ryoPosition, 0x40000000);
        npcPosition = ryoPosition;
        ryoRotation.x = 0;
        ryoRotation.y = 0;
        ryoRotation.z = 0;
        npcRotation.x = 0;
        npcRotation.y = 0;
        npcRotation.z = 0;
        FTBL.EV_FreeAuthorPak(1, tomaConversation);
        switch (work->map) {
        case TOMA_ARZ0:
            QueueAreaLoopCamera(tomaPackage, 1550, prop, 0, 0);
            QueueAreaLoopCamera(tomaPackage, 1551, prop, 200, 0);
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110G001", 1, 10, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110G002", 1, 10, -1, 0);
                break;
            default:
                QueueAreaLoopVoice("0110G003", 1, 10, -1, 0);
                break;
            }
            QueueAreaLoopVoice("0110A012", 1, 130, -1, 0);
            ryoPosition.x = 331.51f;
            ryoPosition.z = 456.29f;
            ryoRotation.y = 0;
            npcPosition.x = 331.53f;
            npcPosition.z = 454.85f;
            npcRotation.y = 0x8000;
            break;
        case TOMA_WET0:
            QueueAreaLoopCamera(tomaPackage, 1550, prop, 0, 0);
            QueueAreaLoopCamera(tomaPackage, 1551, prop, 200, 0);
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110H001", 1, 10, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110H002", 1, 10, -1, 0);
                break;
            default:
                QueueAreaLoopVoice("0110H003", 1, 10, -1, 0);
                break;
            }
            QueueAreaLoopVoice("0110A014", 1, 130, -1, 0);
            ryoPosition.x = 185.0f;
            ryoPosition.z = 107.8f;
            ryoRotation.y = 0;
            npcPosition.x = 185.0f;
            npcPosition.z = 106.28f;
            npcRotation.y = 0x8000;
            break;
        }
        if (FTBL.shenGetRandomInt32(2) == 0)
            QueueAreaLoopVoice("0110A020", 1, 200, -1, ResolveAreaLoopParent);
        else
            QueueAreaLoopVoice("0110A021", 1, 200, -1, ResolveAreaLoopParent);
        QueueAreaLoopPosition(TOMA_PLAYER, 0, 0, &ryoPosition, &ryoRotation, 0);
        QueueAreaLoopPosition(npc, 0, 0, &npcPosition, &npcRotation, 0);
        QueueAreaLoopPosition(prop, 0, 0, &npcPosition, &npcRotation, 0);
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(TOMA_PLAYER));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(TOMA_PLAYER));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(TOMA_PLAYER), 1, 0, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(TOMA_PLAYER), 0, (void *)areaLoopHandPoses[18], 1);
        QueueAreaLoopMotion(TOMA_PLAYER, 0xf003, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 22);
        QueueAreaLoopMotion(npc, 0xf087, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, TomaChargeCompletion, 22);
        QueueAreaLoopMotion(TOMA_PLAYER, 0xf11f, 200, -1, -1.0f, -1.0f, 1.0f, 0.0f, TomaRewardCompletion, 22);
        if (work->flags & 1) {
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
        }
        break;
    }
}

int TomaChargeCompletion(int phase, int value, HLibTask *parent)
{
    TomaWork *work = (TomaWork *)FTBL.HLib_GetTaskParameter(parent);
    const TomaMapEntry *row = 0;
    if (work)
        row = FindTomaMap(work->map);
    if (phase == 0 && row) {
        int index;
        for (index = 0; index < 6; index++) {
            if (row->offers[index].motion == work->argument) {
                FTBL.AddThisToCashOrSegaCoins(-row->offers[index].price, 2);
                break;
            }
        }
    }
    return 1;
}

int TomaRewardCompletion(int phase, int value, HLibTask *parent)
{
    TomaWork *work = (TomaWork *)FTBL.HLib_GetTaskParameter(parent);
    const TomaMapEntry *row = 0;
    if (work)
        row = FindTomaMap(work->map);
    if (phase == 0 && row) {
        int index;
        for (index = 0; index < 6; index++) {
            if (row->offers[index].motion == work->argument) {
                int reward = FTBL.shenGetRandomInt32(100);
                if (reward < 25)
                    reward = tomaRewards[FTBL.shenGetRandomInt32(9)];
                else if (reward < 40)
                    reward = tomaRewards[FTBL.shenGetRandomInt32(5) + 9];
                else
                    reward = tomaRewards[FTBL.shenGetRandomInt32(9) + 14];
                FTBL.AddInventoryItem(reward, 1, 1);
                break;
            }
        }
    }
    return 1;
}

void TomaDone(void *data)
{
    TomaWork *work = (TomaWork *)data;
    const TomaMapEntry *row;
    while (!FTBL.GetShouldRenderIcons(5)) {
        if (work) {
            if (FTBL.filmCheckFadeTrackColour())
                break;
            if (work->flags & 1) {
                work->flags &= ~1;
                break;
            }
            row = FindTomaMap(work->map);
            if (row) {
                if (work->argument) {
                    shen_name names[2];
                    names[0] = row->npc;
                    names[1] = 0;
                    RestoreAreaLoopCharacters(names, 0);
                    work->argument = 0;
                    FTBL.EV_NPC_NewLoadSw_0(0);
                    FTBL.PrepareNpcAllFromArray();
                } else if (!FTBL.ForceLoadNpc(row->npc)) {
                    break;
                }
            }
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        }
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(TOMA_PLAYER));
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(TOMA_PLAYER));
        FTBL.EV_FreeAuthorPak(0, tomaConversation);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.FinishLoading();
        FTBL.HLib_MarkCurrentTaskForCleanup();
        if (FTBL.EV_ControlPackageResources(0, 5, tomaPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, tomaPackage, 0, 0);
        FTBL.EV_SetPlayerFlag(12, 2095, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.AssignLightParams(1, 1);
        FTBL.EV_lightLoad(TOMA_DEFAULT_LIGHT);
        FTBL.AssignLightProgram(0);
        FTBL.CheckIsArgEquZero(1, 1);
        SetAreaLoopCharacterControl(TOMA_PLAYER, 0);
        FTBL.SetPlayerControlEnabled(1);
        break;
    }
}

void QueueTomaFirstTalk(void)
{
    while (FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackageFirstTalk, 4, 11, 0, TASK_EVNT)) {
        FTBL.EV_SetNewSceneIndex();
        FTBL.spmFadeOut(0);
        tomaRaceEntry = 1;
        FTBL.EV_FreeAuthorPak(1, tomaRaceConversation);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        break;
    }
}

void TomaWaitPackageFirstTalk(void *data)
{
    while (!FTBL.filmCheckFadeTrackColour()) {
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        FTBL.HLib_SetCurrentTaskTo(TomaWaitNpcsFirstTalk);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), tomaPlusPackage, ResolveAreaLoopParent);
        break;
    }
}

void TomaWaitNpcsFirstTalk(void *data)
{
    shen_name names[2];
    FTBL.HLib_SetCurrentTaskTo(TomaInitializeTalkAuthor);
    FTBL.HLib_SetupTaskDelay(-1);
    names[0] = TOMA_XGH;
    names[1] = 0;
    QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
}

void TomaInitializeTalkAuthor(void *data)
{
    FTBL.EV_ControlPackageResources(2, 2, tomaPlusPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0122E000");
    FTBL.EV_LoadAudio("0122E000", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(TomaStartFirstTalkChoices);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_INIT_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void TomaStartFirstTalkChoices(void *data)
{
    short choices[4];
    while (!FTBL.filmCheckFadeTrackColour()) {
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(TomaCheckFirstTalkChoices);
        choices[0] = -1;
        choices[3] = -1;
        choices[1] = 398;
        choices[2] = 154;
        FTBL.SetupControlSpeakAndUI(choices, 800, 154, 2);
        break;
    }
}

void TomaCheckFirstTalkChoices(void *data)
{
    if (!FTBL.GetShouldRenderIcons(2)) {
        if (FTBL.shenGetCtrlSpeakIconFlag() == 398) {
            FTBL.HLib_SetCurrentTaskTo(TomaGoRaceFirstTalk);
            if (tomaRaceEntry)
                FTBL.EV_stepSetFlags(1140, 0);
            else
                FTBL.EV_stepSetFlags(997, 0);
        } else {
            FTBL.HLib_SetCurrentTaskTo(TomaExitPlus);
            FTBL.HLib_SetupTaskDelay(-1);
            QueueAreaLoopAuthor(0, TOMA_DECLINE_AUTHOR, 0, -1, ResolveAreaLoopParent);
        }
    }
}

void TomaGoRaceFirstTalk(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(TomaGoRace);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_GO_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void TomaGoRace(void *data)
{
    FTBL.EV_FreeAuthorPak(0, tomaRaceConversation);
    FTBL.spmFadeOut(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_scenQueueLoad(g_actNumber, TOMA_RACE_AREA, tomaRaceEntry);
}

void TomaExitPlus(void *data)
{
    shen_name names[2];
    while (!FTBL.filmCheckFadeTrackColour()) {
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.spmFadeOut(0);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_spmBeginSampleExecute(0);
        names[0] = TOMA_XGH;
        names[1] = 0;
        RestoreAreaLoopCharacters(names, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_FreeAuthorPak(0, tomaRaceConversation);
        FTBL.camCtrlSetState(27);
        FTBL.SetScenarioFollowCharacter(0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        if (FTBL.EV_ControlPackageResources(0, 5, tomaPlusPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, tomaPlusPackage, 0, 0);
        SetAreaLoopCharacterControl(TOMA_PLAYER, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.AssignLightParams(1, 1);
        FTBL.EV_lightLoad(TOMA_DEFAULT_LIGHT);
        FTBL.AssignLightProgram(0);
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.SetRefBoardParamsForCharaById(TOMA_PLAYER, 1);
        FTBL.SetPlayerControlEnabled(1);
        break;
    }
}

void QueueTomaTalk(void)
{
    while (FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackageTalk, 4, 11, 0, TASK_EVNT)) {
        tomaRaceEntry = 0;
        FTBL.EV_SetNewSceneIndex();
        FTBL.spmFadeOut(0);
        FTBL.EV_FreeAuthorPak(1, tomaRaceConversation);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        break;
    }
}

void TomaWaitPackageTalk(void *data)
{
    while (!FTBL.filmCheckFadeTrackColour()) {
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        FTBL.HLib_SetCurrentTaskTo(TomaWaitNpcsTalk);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), tomaPlusPackage, ResolveAreaLoopParent);
        break;
    }
}

void TomaWaitNpcsTalk(void *data)
{
    shen_name names[2];
    FTBL.HLib_SetCurrentTaskTo(TomaInitializeRepeatTalkAuthor);
    FTBL.HLib_SetupTaskDelay(-1);
    names[0] = TOMA_XGH;
    names[1] = 0;
    QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
}

void TomaInitializeRepeatTalkAuthor(void *data)
{
    FTBL.EV_ControlPackageResources(2, 2, tomaPlusPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0122E000");
    FTBL.EV_LoadAudio("0122E000", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(TomaStartFirstTalkChoices);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_INIT_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void QueueTomaBye(void)
{
    while (FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackageBye, 4, 11, 0, TASK_EVNT)) {
        FTBL.EV_SetNewSceneIndex();
        FTBL.spmFadeOut(0);
        FTBL.EV_FreeAuthorPak(1, tomaRaceConversation);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        break;
    }
}

void TomaWaitPackageBye(void *data)
{
    while (!FTBL.filmCheckFadeTrackColour()) {
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        FTBL.HLib_SetCurrentTaskTo(TomaWaitNpcsBye);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), tomaPlusPackage, ResolveAreaLoopParent);
        FTBL.EV_stepSetFlags(1141, 0);
        break;
    }
}

void TomaWaitNpcsBye(void *data)
{
    shen_name names[2];
    FTBL.HLib_SetCurrentTaskTo(TomaBye);
    FTBL.HLib_SetupTaskDelay(-1);
    names[0] = TOMA_XGH;
    names[1] = 0;
    QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
}

void TomaBye(void *data)
{
    FTBL.EV_ControlPackageResources(2, 2, tomaPlusPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0122E000");
    FTBL.EV_LoadAudio("0122E000", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(TomaExitPlus);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_BYE_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void QueueTomaHaveFirstTalk(void)
{
    while (FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackageHaveFirstTalk, 4, 11, 0, TASK_EVNT)) {
        tomaRaceEntry = 2;
        FTBL.EV_FreeAuthorPak(1, tomaRaceConversation);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        break;
    }
}

void TomaWaitPackageHaveFirstTalk(void *data)
{
    while (!FTBL.filmCheckFadeTrackColour()) {
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        FTBL.HLib_SetCurrentTaskTo(TomaWaitNpcsHaveFirstTalk);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), tomaPlusPackage, ResolveAreaLoopParent);
        break;
    }
}

void TomaWaitNpcsHaveFirstTalk(void *data)
{
    shen_name names[2];
    FTBL.HLib_SetCurrentTaskTo(TomaInitializeHaveTalkAuthor);
    FTBL.HLib_SetupTaskDelay(-1);
    names[0] = TOMA_XGH;
    names[1] = 0;
    QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
}

void TomaInitializeHaveTalkAuthor(void *data)
{
    FTBL.EV_ControlPackageResources(2, 2, tomaPlusPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0122E000");
    FTBL.EV_LoadAudio("0122E000", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(TomaStartHaveFirstTalkChoices);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_HAVE_INIT_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void TomaStartHaveFirstTalkChoices(void *data)
{
    short choices[4];
    while (!FTBL.filmCheckFadeTrackColour()) {
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(TomaCheckHaveFirstTalkChoices);
        choices[0] = -1;
        choices[3] = -1;
        choices[1] = 398;
        choices[2] = 154;
        FTBL.SetupControlSpeakAndUI(choices, 800, 154, 2);
        break;
    }
}

void TomaCheckHaveFirstTalkChoices(void *data)
{
    if (!FTBL.GetShouldRenderIcons(2)) {
        if (FTBL.shenGetCtrlSpeakIconFlag() == 398) {
            FTBL.HLib_SetCurrentTaskTo(TomaGoRaceHaveFirstTalk);
            if (tomaRaceEntry)
                FTBL.EV_stepSetFlags(1142, 0);
            else
                FTBL.EV_stepSetFlags(1143, 0);
        } else {
            FTBL.HLib_SetCurrentTaskTo(TomaExitPlus);
            FTBL.HLib_SetupTaskDelay(-1);
            QueueAreaLoopAuthor(0, TOMA_DECLINE_AUTHOR, 0, -1, ResolveAreaLoopParent);
        }
    }
}

void TomaGoRaceHaveFirstTalk(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(TomaGoRace);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_HAVE_GO_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

void TomaGoRaceHave(void *data)
{
    FTBL.EV_FreeAuthorPak(0, tomaRaceConversation);
    FTBL.spmFadeOut(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_scenQueueLoad(g_actNumber, TOMA_RACE_AREA, tomaRaceEntry);
}

void QueueTomaHaveTalk(void)
{
    while (FTBL.HLib_EnqueueTaskWithParameter(TomaWaitPackageHaveTalk, 4, 11, 0, TASK_EVNT)) {
        tomaRaceEntry = 0;
        FTBL.EV_FreeAuthorPak(1, tomaRaceConversation);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        break;
    }
}

void TomaWaitPackageHaveTalk(void *data)
{
    while (!FTBL.filmCheckFadeTrackColour()) {
        SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
        FTBL.HLib_SetCurrentTaskTo(TomaWaitNpcsHaveTalk);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), tomaPlusPackage, ResolveAreaLoopParent);
        break;
    }
}

void TomaWaitNpcsHaveTalk(void *data)
{
    shen_name names[2];
    FTBL.HLib_SetCurrentTaskTo(TomaInitializeRepeatHaveTalkAuthor);
    FTBL.HLib_SetupTaskDelay(-1);
    names[0] = TOMA_XGH;
    names[1] = 0;
    QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
}

void TomaInitializeRepeatHaveTalkAuthor(void *data)
{
    FTBL.EV_ControlPackageResources(2, 2, tomaPlusPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0122E000");
    FTBL.EV_LoadAudio("0122E000", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.HLib_SetCurrentTaskTo(TomaStartHaveFirstTalkChoices);
    FTBL.HLib_SetupTaskDelay(-1);
    QueueAreaLoopAuthor(0, TOMA_HAVE_INIT_AUTHOR, 0, -1, ResolveAreaLoopParent);
}

#define SHENMUE_AREA_LOOP_HAND_POSE_TABLE
#include "s2/areas/area_loop_dc.c"
#endif
#else
#include "s2/areas/area_toma.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
std::int32_t tomaRaceEntry;
void InitToma() {
    tomaRaceEntry = 0;
}
void TomaSearchNullsub() {
}
void TomaMain(std::uintptr_t argument) {
    const TomaRequest *const request = reinterpret_cast<const TomaRequest *>(argument);
    if (!request)
        return;
    const TomaMapEntry *const row = FindTomaMap(request->map);
    if ((request->selector == 1 || request->selector == 2) && !row)
        return;
    tomaRaceEntry = 0;
    switch (request->selector) {
    case 0:
    case 2:
        SetPlayerControlEnabled(1);
        break;
    case 1:
        QueueTomaWaitPackage(request->map, request->argument);
        srchReset();
        SetPlayerControlEnabled(0);
        TomaSearchNullsub();
        break;
    case 100:
        QueueTomaFirstTalk();
        break;
    case 101:
        QueueTomaTalk();
        break;
    case 102:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageHaveFirstTalk, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            tomaRaceEntry = 2;
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    case 103:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageHaveTalk, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            tomaRaceEntry = 0;
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    case 200:
        if (HLib::EnqueueTaskWithParameter(TomaWaitPackageBye, 4, 0x0b, 0, AREA_EVENT_TASK)) {
            EV_SetNewSceneIndex();
            spmFadeOut();
            EV_FreeAuthorPak(1, "0122");
            SetPlayerControlEnabled(0);
            EV_NPC_NewLoadSw(0);
        }
        break;
    default:
        return;
    }
}
void QueueTomaWaitPackage(shen_name map, std::uint32_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TomaWaitPackage, 4, 0x0b, sizeof(TomaWork), AREA_EVENT_TASK);
    if (!task)
        return;
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->state = 1;
    work->map = map;
    work->argument = argument;
    EV_NPC_NewLoadSw(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2095, 1);
}
void TomaWaitPackage(void *parameter) {
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TOMA")))
        return;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_NPC_NewLoadSw(1);
    HLib::SetCurrentTaskTo(TomaStart);
    const TomaMapEntry *const row = FindTomaMap(work->map);
    if (row) {
        HLib::SetupTaskDelay(-1);
        const shen_name characters[]{row->character, {}};
        QueueTomaNpcs(characters);
    }
}
void TomaStart(void *parameter) {
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (filmCheckFadeTrackColour() || !work)
        return;
    const TomaMapEntry *const row = FindTomaMap(work->map);
    if (!row)
        return;
    HLib::SetCurrentTaskTo(TomaDone);
    HLib::SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    Vector3 playerPosition;
    EV_GetCharaPosition(TOMA_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    Vector3 sellerPosition = playerPosition;
    std::int32_t yaw = 0;
    EV_FreeAuthorPak(1, "0110");
    if (work->map.name == TOMA_WET0.name) {
        QueueTomaCamera(1550, 0);
        QueueTomaCamera(1551, 200);
        const std::int32_t voice = shenGetRandomInt32(3);
        QueueTomaVoice(voice == 0 ? "0110H001" : voice == 1 ? "0110H002" : "0110H003", 10, nullptr);
        QueueTomaVoice("0110A014", 130, nullptr);
        playerPosition.x = 185.0f;
        playerPosition.z = 107.8f;
        sellerPosition.x = 185.0f;
        sellerPosition.z = 106.27999877929688f;
        yaw = 0x8000;
    } else if (work->map.name == TOMA_ARZ0.name) {
        QueueTomaCamera(1550, 0);
        QueueTomaCamera(1551, 200);
        const std::int32_t voice = shenGetRandomInt32(3);
        QueueTomaVoice(voice == 0 ? "0110G001" : voice == 1 ? "0110G002" : "0110G003", 10, nullptr);
        QueueTomaVoice("0110A012", 130, nullptr);
        playerPosition.x = 331.510009765625f;
        playerPosition.z = 456.2900085449219f;
        sellerPosition.x = 331.5299987792969f;
        sellerPosition.z = 454.8500061035156f;
        yaw = 0x8000;
    }
    QueueTomaVoice(shenGetRandomInt32(2) == 0 ? "0110A020" : "0110A021", 200, ResolveAreaLoopParent);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = TOMA_PLAYER, .flags = 3, .position = playerPosition, .rotation = {0, 0, 0}, .completion = nullptr}, 0);
    if (row->character.name)
        QueueAreaLoopPosition({.parent = HLib::GetCurrentTask(),
                                  .character = row->character,
                                  .flags = 3,
                                  .position = sellerPosition,
                                  .rotation = {0, yaw, 0},
                                  .completion = nullptr},
            0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = TOMA_PROP, .flags = 3, .position = sellerPosition, .rotation = {0, yaw, 0}, .completion = nullptr}, 0);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(TOMA_PLAYER));
    SetLowQualityHands(ctrlGetCharaByName(TOMA_PLAYER));
    SetHighModelXYZ(ctrlGetCharaByName(TOMA_PLAYER), 1, 0, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(TOMA_PLAYER), 0, reinterpret_cast<std::byte *>(TOMA_HAND_POSE.data()), 1);
    QueueTomaMotion(TOMA_PLAYER, 0xf003, nullptr, 0);
    if (row->character.name)
        QueueTomaMotion(row->character, 0xf087, TomaChargeCompletion, 0);
    QueueTomaMotion(TOMA_PLAYER, 0xf11f, TomaRewardCompletion, 200);
    if (work->state & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state &= ~1U;
    }
}
std::int64_t TomaChargeCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(parent));
    const TomaMapEntry *const row = work ? FindTomaMap(work->map) : nullptr;
    if (phase == 0 && row)
        for (const TomaOffer &offer : row->offers)
            if (offer.motion.name == work->argument) {
                AddThisToCashOrSegaCoins(-static_cast<std::int16_t>(offer.price), ECashOrCoins::Cash2);
                break;
            }
    return 1;
}
std::int64_t TomaRewardCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    TomaWork *const work = static_cast<TomaWork *>(HLib::GetTaskParameter(parent));
    const TomaMapEntry *const row = work ? FindTomaMap(work->map) : nullptr;
    if (phase == 0 && row)
        for (const TomaOffer &offer : row->offers)
            if (offer.motion.name == work->argument) {
                const std::int32_t category = shenGetRandomInt32(100);
                const std::uint32_t item = category < 25 ? TOMA_REWARDS[shenGetRandomInt32(9)] :
                    category < 40                        ? TOMA_REWARDS[shenGetRandomInt32(5) + 9] :
                                                           TOMA_REWARDS[shenGetRandomInt32(9) + 14];
                AddInventoryItem(item, 1, 1);
                break;
            }
    return 1;
}
void TomaDone(void *parameter) {
    using enum CameraState;
    if (GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        return;
    TomaWork *const work = static_cast<TomaWork *>(parameter);
    if (work) {
        if (filmCheckFadeTrackColour())
            return;
        if (work->state & 1U) {
            work->state &= ~1U;
            return;
        }
        const TomaMapEntry *const row = FindTomaMap(work->map);
        if (row) {
            if (work->argument) {
                const shen_name characters[]{row->character, {}};
                RestoreAreaLoopCharacters(characters);
                work->argument = 0;
                EV_NPC_NewLoadSw(0);
                PrepareNpcAllFromArray();
            } else if (!EV_ForceLoadNpc(row->character))
                return;
        }
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    HasCharacterMotionWork(ctrlGetCharaByName(TOMA_PLAYER));
    hndlSetFlags(ctrlGetCharaByName(TOMA_PLAYER));
    EV_FreeAuthorPak(0, "0110");
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    FinishLoading();
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("TOMA"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOMA"), 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2095, 0);
    EV_NPC_NewLoadSw(1);
    EV_SetModelDataDefaultVertexShader();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(TOMA_DEFAULT_LIGHT);
    AssignLightProgram(0);
    CheckIsArgEquZero(1);
    SetAreaLoopCharacterControl(TOMA_PLAYER, 0);
    SetPlayerControlEnabled(1);
}
const ScriptAreaDefinition tomaScriptDefinition{.areaId = TOMA_AREA, .field04 = 0, .init = InitToma, .main = TomaMain};
} // namespace shenmue::s2::areas
#endif
