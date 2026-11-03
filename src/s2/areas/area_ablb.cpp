#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ablb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void ABLB_main_func(void *argument)
{
    HLibTask *task;
    AblbWork *work;
    unsigned char i;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetObjectFlagByIndex(0, 0);
    FTBL.SetObjectFlagByIndex(1, 0);
    FTBL.SetObjectFlagByIndex(2, 0);
    FTBL.SetObjectFlagByIndex(3, 0);
    FTBL.SetObjectFlagByIndex(4, 0);
    FTBL.SetObjectFlagByIndex(5, 0);
    FTBL.SetObjectFlagByIndex(6, 0);
    FTBL.SetObjectFlagByIndex(7, 0);
    FTBL.SetObjectFlagByIndex(8, 0);
    FTBL.SetObjectFlagByIndex(9, 0);
    FTBL.SetObjectFlagByIndex(10, 0);
    FTBL.SetObjectFlagByIndex(11, 0);
    FTBL.SetObjectFlagByIndex(12, 0);
    task = FTBL.HLib_EnqueueTaskWithParameter(AlleyBattleCallback, 5, 11, sizeof(AblbWork), TASK_EVNT);
    work = (AblbWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->phase = 0;
    work->field38 = 0;
    work->field3c = 0;
    work->npcBatchCount = FTBL.GetNpcBatchCountOverride(35);
    work->characters = Queue_EVT_AT_ReadChr(task, ablbCharacters);
    work->delay = 0;
    work->fadeOutFrames = 0;
    work->fadeInFrames = 0;
    work->reserved90 = 0;
    work->entry = g_entryPoint;
    work->elapsed = 0;
    work->field04 = 0;
    work->field08 = 0;
    work->field0c = -1;
    work->skyColour = 0;
    work->cloudColour = 0;
    work->colourBlend[0] = 0.0f;
    work->colourBlend[4] = 0.0f;
    work->colourBlend[1] = 0.0f;
    work->colourBlend[5] = 0.0f;
    work->colourBlend[2] = 0.0f;
    work->colourBlend[6] = 0.0f;
    work->colourBlend[3] = 0.0f;
    work->colourBlend[7] = 0.0f;
    work->colourBlend[8] = 0.0f;
    work->colourBlend[12] = 0.0f;
    work->colourBlend[9] = 0.0f;
    work->colourBlend[13] = 0.0f;
    work->colourBlend[10] = 0.0f;
    work->colourBlend[14] = 0.0f;
    work->colourBlend[11] = 0.0f;
    work->colourBlend[15] = 0.0f;
    work->outroFrames = 0;
    for (i = 0; i < 6; i++) {
        work->colours[0][i] = 0;
        work->colours[1][i] = 0;
        work->colours[2][i] = 0;
        work->colours[3][i] = 0;
    }
    work->colours[0][0] = work->colours[0][1] = 185;
    work->colours[1][0] = work->colours[1][1] = 35;
    work->colours[2][0] = work->colours[2][1] = 20;
    work->colours[3][0] = work->colours[3][1] = 0;
    work->fadeOutSeconds = 12;
    work->fadeInSeconds = 7;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)ablbPackage, 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, ablbPackage, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, ablbPackage, 0, 0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene(ablbFirstCutscene);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.vfmDispatchControl(20, 0, 0, 1);
    FTBL.SetObjectFlagByIndex(10, 0);
    FTBL.EV_SetPlayerFlag(12, 250, 0);
    FTBL.SetObjectFlagByIndex(0, 1);
    FTBL.SetObjectFlagByIndex(1, 1);
    FTBL.SetObjectFlagByIndex(2, 1);
    FTBL.SetObjectFlagByIndex(3, 1);
    FTBL.SetObjectFlagByIndex(4, 1);
    FTBL.SetObjectFlagByIndex(5, 1);
    FTBL.SetObjectFlagByIndex(6, 1);
    FTBL.SetObjectFlagByIndex(7, 1);
    FTBL.SetObjectFlagByIndex(8, 1);
    FTBL.SetObjectFlagByIndex(9, 1);
    FTBL.SetObjectFlagByIndex(10, 0);
    FTBL.SetObjectFlagByIndex(11, 1);
    FTBL.SetObjectFlagByIndex(12, 1);
    EventDebugRegister(ABLB_DEBUG_ID, AblbDebugExitCallback, work, 36000);
}
#elif defined(SHENMUE_EVENT_ABLB_AUTHOR)
static shen_name ablbAuthoredCharacters[] = {ABLB_HVA, ABLB_TGI, ABLB_SNC, ABLB_SAM, ABLB_LLY, ABLB_CLJ, ABLB_ENF, ABLB_TYZ, EVENT_CHARACTER_INVALID};

void QueueAuthor(unsigned char author)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(AuthorCallback, 4, 11, sizeof(AblbAuthorWork), TASK_EVNT);
    AblbAuthorWork *work = (AblbAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->phase = 1;
    work->author = author;
    PrepareAuthor();
    SelectAuthorPackage(author);
}

void AuthorCallback(void *data)
{
    AblbAuthorWork *work = (AblbAuthorWork *)data;
    switch (work->phase) {
    case 1:
        QueueAuthorLoop(work);
        break;
    case 3:
        FinishAuthor(work);
        break;
    }
}

void QueueAuthorLoop(AblbAuthorWork *work)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(AuthorLoopCallback, 4, 11, sizeof(AblbAuthorLoopWork), TASK_EVNT);
    AblbAuthorLoopWork *loop = (AblbAuthorLoopWork *)FTBL.HLib_GetTaskParameter(task);
    loop->parent = work;
    loop->frame = 0;
    FTBL.SetMapModelDataDefaultVertexShader();
    switch (work->author) {
    case 0:
        FTBL.SetWeatherUpdateEnabledTo(1);
        FTBL.EV_lightLoad(ABLB_LIGHT_ALLEY);
        FTBL.PushGlobalLightingProgram();
        break;
    case 1:
        FTBL.SetWeatherUpdateEnabledTo(1);
        FTBL.EV_lightLoad(ABLB_LIGHT_FIGHT);
        FTBL.PushGlobalLightingProgram();
        break;
    case 2:
        FTBL.SetWeatherUpdateEnabledTo(0);
        FTBL.EV_lightLoad(ABLB_LIGHT_OUTRO);
        FTBL.PushGlobalLightingProgram();
        FTBL.EV_lightLoad(ABLB_LIGHT_OUTRO_OVERLAY);
        break;
    }
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_authSceneCtrl(work->author);
    work->phase = 2;
}

void SelectAuthorPackage(int author)
{
    if (author == 0) {
        if (FTBL.EV_ControlPackageResources(0, 5, "AB00", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "AB00", 0, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"AKA3", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "AKA3", 0, 0);
    } else {
        if (FTBL.EV_ControlPackageResources(0, 5, "AKA3", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "AKA3", 0, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"AB00", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "AB00", 0, 0);
    }
}

void AuthorLoopCallback(void *data)
{
    AblbAuthorLoopWork *work = (AblbAuthorLoopWork *)data;
    work->frame = FTBL.EV_authSceneCtrl(-6);
    switch (work->frame) {
    case 1:
        HideAuthoredCharacters();
        break;
    case 500:
        if (work->parent->author == 2)
            FTBL.EV_NPC_NewLoadSw_0(1);
        break;
    case 600:
        if (work->parent->author == 2)
            FTBL.EV_SetPlayerFlag(12, 250, 1);
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        ShowAuthoredCharacters();
        work->parent->phase = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void PrepareAuthor(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0138");
}

void FinishAuthor(AblbAuthorWork *work)
{
    FTBL.HLib_DestroyTask(work->task);
}

void HideAuthoredCharacters(void)
{
    int i;
    HideAuthoredCharacter(ABLB_RYO);
    HideDecoration(ABLB_A073);
    HideDecoration(ABLB_A417);
    HideDecoration(ABLB_AA50);
    HideDecoration(ABLB_A014);
    for (i = 0; ablbAuthoredCharacters[i] != EVENT_CHARACTER_INVALID; i++)
        HideAuthoredCharacter(ablbAuthoredCharacters[i]);
}

void ShowAuthoredCharacters(void)
{
    int i;
    ShowAuthoredCharacter(ABLB_RYO);
    ShowDecoration(ABLB_A073);
    ShowDecoration(ABLB_A417);
    ShowDecoration(ABLB_AA50);
    ShowDecoration(ABLB_A014);
    for (i = 0; ablbAuthoredCharacters[i] != EVENT_CHARACTER_INVALID; i++)
        ShowAuthoredCharacter(ablbAuthoredCharacters[i]);
}

void HideAuthoredCharacter(shen_name character)
{
    FTBL.SetScenarioCharacterRadius(character, 50.0f);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowAuthoredCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideDecoration(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowDecoration(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}
#else
const shen_name ablbCharacters[] = {ABLB_RYO, ABLB_HVA, ABLB_TGI, ABLB_SNC, ABLB_SAM, ABLB_LLY, ABLB_CLJ, ABLB_ENF, ABLB_TYZ, ABLB_H01, ABLB_H02, ABLB_H03,
    ABLB_H04, ABLB_H05, ABLB_H06, EVENT_CHARACTER_INVALID};
const char ablbPackage[] = "ABLB";
const char ablbFirstCutscene[] = "0138A010";
static char ablbColourBlendInitialized = 0;

void AlleyBattleCallback(void *data)
{
    AblbWork *work = (AblbWork *)data;
    switch (work->phase) {
    case 0:
        SetBattleCharacterVisible(ABLB_H01, 0);
        SetBattleCharacterVisible(ABLB_H02, 0);
        SetBattleCharacterVisible(ABLB_H03, 0);
        SetBattleCharacterVisible(ABLB_H04, 0);
        SetBattleCharacterVisible(ABLB_H05, 0);
        SetBattleCharacterVisible(ABLB_H06, 0);
        FTBL.EV_LoadAudio(ablbFirstCutscene, 0);
        FTBL.EV_spmBeginSampleExecute(0);
        QueueAuthor(1);
        work->phase = 1;
        break;
    case 1:
        if (FTBL.EV_authSceneCtrl(-1))
            work->phase = 2;
        break;
    case 2:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.EV_LoadCutsceneFromCurrentScene("0138A020");
            FTBL.EV_LoadAudio("0138A020", 0);
            FTBL.EV_spmBeginSampleExecute(0);
            QueueAuthor(2);
            work->phase = 4;
        }
        break;
    case 4:
        if (FTBL.EV_authSceneCtrl(-1))
            work->phase = 5;
        break;
    case 5:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            if (FTBL.EV_authSceneCtrl(-13) && !FTBL.EV_GetPlayerFlag(11, 250)) {
                FTBL.EV_NPC_NewLoadSw_0(1);
                FTBL.EV_SetPlayerFlag(12, 250, 1);
            }
            work->phase = 7;
        }
        break;
    case 7:
        FTBL.SetWeatherUpdateEnabledTo(1);
        ablbColourBlendInitialized = 0;
        SetupBattle(work);
        break;
    case 8:
        FTBL.HLib_SetupTaskDelay(work->delay);
        work->phase = 9;
        break;
    case 9:
        BeginFight(work);
        break;
    case 10:
        if (--work->elapsed == 0) {
            FTBL.vfmDispatchControl(10, 0, 0, 1);
            FTBL.SetScreenFilmFade(30 * work->fadeOutSeconds, 0, 0, 0, 0, work->colours[0][0], work->colours[1][0], work->colours[2][0],
                work->colours[3][0]);
            work->fadeOutFrames = 30 * work->fadeOutSeconds;
            SaveWeatherColours(work);
            work->phase = 11;
        }
        break;
    case 11:
        if (--work->fadeOutFrames >= 0)
            BlendWeatherColours(work);
        else {
            FTBL.SetScreenFilmFade(30 * work->fadeInSeconds, work->colours[0][0], work->colours[1][0], work->colours[2][0], work->colours[3][0], 255, 0,
                0, 0);
            work->fadeInFrames = 30 * work->fadeInSeconds;
            work->phase = 12;
        }
        break;
    case 12:
        if (--work->fadeInFrames < 0) {
            FTBL.check_dead_status(1);
            FTBL.EV_FreeAuthorPak(0, "0138");
            FTBL.EV_stepSetFlags(513, 0);
            work->phase = 13;
        }
        break;
    case 13:
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (work->entry == 99)
                work->phase = 6;
            else {
                FTBL.EV_NPC_NewLoadSw_0(0);
                FTBL.EV_ResetFreeTalkFlags();
                FTBL.EV_FreeAuthorPak(1, "S2BV");
                FTBL.SetObjectFlagByIndex(0, 0);
                FTBL.SetObjectFlagByIndex(1, 0);
                FTBL.SetObjectFlagByIndex(2, 0);
                FTBL.SetObjectFlagByIndex(3, 0);
                FTBL.SetObjectFlagByIndex(4, 0);
                FTBL.SetObjectFlagByIndex(5, 0);
                FTBL.SetObjectFlagByIndex(6, 0);
                FTBL.SetObjectFlagByIndex(7, 0);
                FTBL.SetObjectFlagByIndex(8, 0);
                FTBL.SetObjectFlagByIndex(9, 0);
                FTBL.SetObjectFlagByIndex(10, 0);
                FTBL.SetObjectFlagByIndex(11, 0);
                FTBL.SetObjectFlagByIndex(12, 0);
                work->phase = 14;
            }
        }
        break;
    case 14:
        if (!work->outroFrames)
            OutroVoices();
        if (++work->outroFrames > 430)
            work->phase = 15;
        else if (work->outroFrames == 400)
            FTBL.spmFadeOut(0);
        break;
    case 15:
        ExitAlleyBattle(work);
        break;
    }
}

void SetupBattle(AblbWork *work)
{
    Vector3 position, target;
    SetBattleCharacterVisible(ABLB_HVA, 0);
    SetBattleCharacterVisible(ABLB_TGI, 0);
    SetBattleCharacterVisible(ABLB_SNC, 0);
    SetBattleCharacterVisible(ABLB_SAM, 0);
    SetBattleCharacterVisible(ABLB_LLY, 0);
    SetBattleCharacterVisible(ABLB_CLJ, 0);
    SetBattleCharacterVisible(ABLB_ENF, 0);
    SetBattleCharacterVisible(ABLB_TYZ, 0);
    SetBattleCharacterVisible(ABLB_A014, 0);
    SetBattleCharacterVisible(ABLB_AA50, 0);
    SetBattleCharacterVisible(ABLB_A073, 0);
    SetBattleCharacterVisible(ABLB_A417, 0);
    SetBattleCharacterVisible(ABLB_H01, 1);
    SetBattleCharacterVisible(ABLB_H02, 1);
    SetBattleCharacterVisible(ABLB_H03, 1);
    SetBattleCharacterVisible(ABLB_H04, 1);
    SetBattleCharacterVisible(ABLB_H05, 1);
    SetBattleCharacterVisible(ABLB_H06, 1);
    EVT_AT_SetMot(ABLB_RYO, 306, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_RYO, EVENT_CHARACTER_INVALID, 251.66f, 96.85f, 492.23f, 0);
    EVT_AT_SetAngY(ABLB_RYO, EVENT_CHARACTER_INVALID, 85.94f, 0);
    EVT_AT_SetMot(ABLB_H01, 3632, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H01, EVENT_CHARACTER_INVALID, 249.83f, 96.85f, 490.44f, 0);
    EVT_AT_SetAngY(ABLB_H01, EVENT_CHARACTER_INVALID, 235.94f, 0);
    EVT_AT_SetMot(ABLB_H02, 3636, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H02, EVENT_CHARACTER_INVALID, 249.74f, 96.85f, 494.19f, 0);
    EVT_AT_SetAngY(ABLB_H02, EVENT_CHARACTER_INVALID, 300.92f, 0);
    EVT_AT_SetMot(ABLB_H03, 3640, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H03, EVENT_CHARACTER_INVALID, 251.32f, 96.85f, 494.13f, 0);
    EVT_AT_SetAngY(ABLB_H03, EVENT_CHARACTER_INVALID, 5.91f, 0);
    EVT_AT_SetMot(ABLB_H04, 3644, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H04, EVENT_CHARACTER_INVALID, 252.08f, 96.85f, 490.98f, 0);
    EVT_AT_SetAngY(ABLB_H04, EVENT_CHARACTER_INVALID, 140.96f, 0);
    EVT_AT_SetMot(ABLB_H05, 3648, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H05, EVENT_CHARACTER_INVALID, 247.76f, 96.85f, 492.93f, 0);
    EVT_AT_SetAngY(ABLB_H05, EVENT_CHARACTER_INVALID, 295.92f, 0);
    EVT_AT_SetMot(ABLB_H06, 3632, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(ABLB_H06, EVENT_CHARACTER_INVALID, 248.72f, 96.85f, 489.74f, 0);
    EVT_AT_SetAngY(ABLB_H06, EVENT_CHARACTER_INVALID, 265.93f, 0);
    position.x = 251.15961f;
    position.y = 98.82f;
    position.z = 487.0921f;
    target.x = 250.2169952392578f;
    target.y = 98.05000305175781f;
    target.z = 491.5154113769531f;
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetNextPosition(&position);
    FTBL.camCtrlSetTargetPosition(&target);
    FTBL.EV_LoadAudio("0138A020", 1);
    FTBL.EV_spmBeginSampleExecute(0);
    work->delay = 30;
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.camCtrlSetPerspective(0x1c71);
    work->phase = 8;
}

void BeginFight(AblbWork *work)
{
    unsigned char i;
    FTBL.shenStartFGST(9, 20, 0, 6);
    FTBL.vfmDispatchControl(2, 0, ablbPackage, 1);
    AddGeneratorPoints();
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_RYO), 0, 0, 0, 0, 0);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_RYO), 0, 0x20000, 31);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H01), 0, 1, 1, 100, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H01), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H01), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H02), 0, 1, 1, 100, 3);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H02), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H02), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H03), 0, 1, 1, 100, 4);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H03), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H03), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H04), 0, 1, 1, 100, 5);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H04), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H04), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H05), 0, 1, 1, 100, 6);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H05), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H05), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(ABLB_H06), 0, 1, 1, 100, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H06), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(ABLB_H06), 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 3, 3636, 254.02f, 96.85f, 490.84f, 115);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 4, 3640, 253.92f, 96.85f, 494.84f, 35);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 5, 3644, 251.32f, 96.85f, 496.24f, 0);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 6, 3648, 248.12f, 96.85f, 494.54f, 285);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 2, 3632, 247.92f, 96.85f, 490.94f, 215);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 3, 3636, 249.02f, 96.85f, 489.44f, 240);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 4, 3640, 253.22f, 96.85f, 489.54f, 125);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 5, 3644, 254.82f, 96.85f, 492.74f, 80);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 6, 3648, 253.12f, 96.85f, 496.44f, 20);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 2, 3632, 248.92f, 96.85f, 496.24f, 340);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 3, 3636, 254.83f, 96.85f, 489.54f, 125);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 4, 3640, 253.02f, 96.85f, 495.84f, 20);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 5, 3644, 252.92f, 96.85f, 489.54f, 170);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.StartFightDefeat(100, 6, 3648, 253.72f, 96.85f, 490.64f, 90);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor((void *)ABLB_DEAD, 0, 0x200000, 2048);
    FTBL.vfCopyPosFromEntity(0, 0, 250.95f, 492.93f, 4.8f);
    FTBL.EV_vfBattleUtil(3, 0x800, 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(ABLB_RYO), 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ABLB_RYO), 0, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ABLB_RYO), 0, 0, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ABLB_RYO), 1);
    for (i = 0; ablbCharacters[i] != EVENT_CHARACTER_INVALID; i++)
        FTBL.SetScenarioCharacterType(ablbCharacters[i], 0);
    FTBL.vfmQueueFightStart(6, 2, 1.0f, 1, -1);
    work->elapsed = 690;
    work->phase = 10;
}

void AddGeneratorPoints(void)
{
    FTBL.vfAddGeneratorPoint(249.12f, 96.85f, 489.24f);
    FTBL.vfAddGeneratorPoint(246.92f, 96.85f, 493.04f);
    FTBL.vfAddGeneratorPoint(248.12f, 96.85f, 495.74f);
    FTBL.vfAddGeneratorPoint(253.02f, 96.85f, 496.24f);
    FTBL.vfAddGeneratorPoint(254.92f, 96.85f, 496.94f);
    FTBL.vfAddGeneratorPoint(253.12f, 96.85f, 489.34f);
}

void AblbDebugExitCallback(void *data)
{
    ExitAlleyBattle((AblbWork *)data);
}

void ExitAlleyBattle(AblbWork *work)
{
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ABLB_RYO), 0);
    FTBL.vfmDispatchControl(20, 0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    FTBL.SetWeatherUpdateEnabledTo(1);
    ablbColourBlendInitialized = 0;
    EventDebugUnregister(ABLB_DEBUG_ID);
    ET_AT_KillWork(work->characters);
    FTBL.GetNpcBatchCountOverride(work->npcBatchCount);
    FTBL.EV_ControlPackageResources(0, 6, ablbPackage, 0, 0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.EV_scenQueueLoad(2, ABLB_NEXT_AREA, 6);
}

void SaveWeatherColours(AblbWork *work)
{
    unsigned int sky, cloud;
    FTBL.SetWeatherUpdateEnabledTo(0);
    sky = FTBL.GetWeatherCloudOrSkyColour(0);
    cloud = FTBL.GetWeatherCloudOrSkyColour(1);
    work->colours[0][2] = sky;
    work->colours[1][2] = sky;
    work->colours[2][2] = sky;
    work->colours[3][2] = sky;
    work->colours[0][2] = work->colours[0][2] >> 24 & 255;
    work->colours[1][2] = work->colours[1][2] >> 16 & 255;
    work->colours[2][2] = work->colours[2][2] >> 8 & 255;
    work->colours[3][2] = work->colours[3][2] & 255;
    work->colours[0][4] = cloud;
    work->colours[1][4] = cloud;
    work->colours[2][4] = cloud;
    work->colours[3][4] = cloud;
    work->colours[0][4] = work->colours[0][4] >> 24 & 255;
    work->colours[1][4] = work->colours[1][4] >> 16 & 255;
    work->colours[2][4] = work->colours[2][4] >> 8 & 255;
    work->colours[3][4] = work->colours[3][4] & 255;
    if (work->entry == 99) {
        FTBL.DebugLogToFileRemoved("G_alpha : %d\t T_alpha : %d\n", work->colours[0][2], work->colours[0][1]);
        FTBL.DebugLogToFileRemoved("G_red   : %d\t T_red   : %d\n", work->colours[1][2], work->colours[1][1]);
        FTBL.DebugLogToFileRemoved("G_green : %d\t T_green : %d\n", work->colours[2][2], work->colours[2][1]);
        FTBL.DebugLogToFileRemoved("G_blue  : %d\t T_blue  : %d\n", work->colours[3][2], work->colours[3][1]);
        FTBL.DebugLogToFileRemoved("Time    : %d\t Time2   : %d\n", work->fadeOutSeconds, work->fadeInSeconds);
    }
}

void BlendWeatherColours(AblbWork *work)
{
    float red, green, blue;
    if (!ablbColourBlendInitialized) {
        red = work->colours[1][1] * 2.0f;
        green = work->colours[2][1] * 2.0f;
        blue = work->colours[3][1];
        work->colourBlend[0] = work->colours[0][3];
        work->colourBlend[1] = work->colours[1][3];
        work->colourBlend[2] = work->colours[2][3];
        work->colourBlend[3] = work->colours[3][3];
        work->colourBlend[4] = work->colours[0][2];
        work->colourBlend[5] = work->colours[1][2];
        work->colourBlend[7] = work->colours[3][2];
        work->colourBlend[6] = work->colours[2][2];
        work->colourBlend[8] = work->colours[0][5];
        work->colourBlend[9] = work->colours[1][5];
        work->colourBlend[10] = work->colours[2][5];
        work->colourBlend[11] = work->colours[3][5];
        work->colourBlend[12] = work->colours[0][4];
        work->colourBlend[13] = work->colours[1][4];
        work->colourBlend[14] = work->colours[2][4];
        work->colourBlend[15] = work->colours[3][4];
        work->colourBlend[1] = (red - work->colourBlend[5]) / (30 * work->fadeOutSeconds);
        work->colourBlend[2] = (green - work->colourBlend[6]) / (30 * work->fadeOutSeconds);
        work->colourBlend[3] = (blue - work->colourBlend[7]) / (30 * work->fadeOutSeconds);
        work->colourBlend[9] = (red - work->colourBlend[13]) / (30 * work->fadeOutSeconds);
        work->colourBlend[10] = (green - work->colourBlend[14]) / (30 * work->fadeOutSeconds);
        work->colourBlend[11] = (blue - work->colourBlend[15]) / (30 * work->fadeOutSeconds);
        ablbColourBlendInitialized = 1;
    }
    if (work->fadeOutFrames == 0) {
        work->colours[0][2] = work->colours[0][1];
        work->colours[1][2] = work->colours[1][1];
        work->colours[2][2] = work->colours[2][1];
        work->colours[3][2] = work->colours[3][1];
        work->colours[0][4] = work->colours[0][1];
        work->colours[1][4] = work->colours[1][1];
        work->colours[2][4] = work->colours[2][1];
        work->colours[3][4] = work->colours[3][1];
    } else if (work->fadeOutFrames > 0) {
        work->colourBlend[5] += work->colourBlend[1];
        work->colourBlend[6] += work->colourBlend[2];
        work->colourBlend[7] += work->colourBlend[3];
        work->colours[0][2] = (int)work->colourBlend[4] << 24 & 0xff000000;
        work->colours[1][2] = (int)work->colourBlend[5] << 16 & 0xff0000;
        work->colours[2][2] = (int)work->colourBlend[6] << 8 & 0xff00;
        work->colours[3][2] = (int)work->colourBlend[7] & 0xff;
        work->skyColour = work->colours[0][2] | work->colours[1][2] | work->colours[2][2] | work->colours[3][2];
        work->colourBlend[12] += work->colourBlend[8];
        work->colourBlend[13] += work->colourBlend[9];
        work->colourBlend[14] += work->colourBlend[10];
        work->colourBlend[15] += work->colourBlend[11];
        work->colours[0][4] = (int)work->colourBlend[12] << 24 & 0xff000000;
        work->colours[1][4] = (int)work->colourBlend[13] << 16 & 0xff0000;
        work->colours[2][4] = (int)work->colourBlend[14] << 8 & 0xff00;
        work->colours[3][4] = (int)work->colourBlend[15] & 0xff;
        work->cloudColour = work->colours[0][4] | work->colours[1][4] | work->colours[2][4] | work->colours[3][4];
    }
    FTBL.SetCloudAndSkyColour(work->skyColour, work->cloudColour);
}

void SetBattleCharacterVisible(shen_name character, int visible)
{
    if (visible) {
        FTBL.EV_EnableCharacterDisplay(character);
        FTBL.EV_EnsureCharacterDraw(character);
    } else {
        FTBL.EV_DisableCharacterDisplay(character);
        FTBL.EV_ToggleCharacterFlag(character);
    }
}

void OutroVoices(void)
{
    EVT_AT_SetVoice("S2BVA004", 10);
    EVT_AT_SetSound(14, 43);
    EVT_AT_SetVoice("S2BVB030", 46);
    EVT_AT_SetVoice("S2BVC006", 86);
    EVT_AT_SetSound(34, 106);
    EVT_AT_SetSound(16, 115);
    EVT_AT_SetVoice("S2BVA007", 116);
    EVT_AT_SetSound(34, 153);
    EVT_AT_SetVoice("S2BVB014", 153);
    EVT_AT_SetSound(16, 205);
    EVT_AT_SetVoice("S2BVA012", 206);
    EVT_AT_SetSound(38, 228);
    EVT_AT_SetVoice("S2BVA013", 233);
    EVT_AT_SetSound(38, 374);
    EVT_AT_SetVoice("S2BVC011", 376);
}
#endif
#else
#include "s2/areas/area_ablb.hpp"
#include "s2/pause.hpp"
#include "s2/areas/area_ablb_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace alley_battle {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr std::array<shen_name, 6> enemies{{SHEN_NAME(H01_), SHEN_NAME(H02_), SHEN_NAME(H03_), SHEN_NAME(H04_), SHEN_NAME(H05_), SHEN_NAME(H06_)}};
constexpr std::array<shen_name, 4> decorations{{SHEN_NAME(a073), SHEN_NAME(a417), SHEN_NAME(aa50), SHEN_NAME(a014)}};
bool colourBlendInitialized{};
struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    std::uint16_t author, phase;
};
struct AuthorLoopWork {
    AuthorWork *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(AuthorWork) == 0x10 && offsetof(AuthorWork, author) == 0x0c);
static_assert(sizeof(AuthorLoopWork) == 0x10);
void Objects(bool visible) {
    for (int i = 0; i < 13; ++i)
        EV_SetObjectFlagByIndex(i, visible && i != 10);
}
void SetAuthoredRadius(shen_name character) {
    SetScenarioCharacterRadius(character, 50.0f);
}
void AuthoredFlags(bool enabled) {
    if (!enabled)
        SetAuthoredRadius(ryo);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 6, enabled ? 0 : 2);
    EV_SetCharacterAlphaEnabled(ryo, enabled);
    for (auto name : decorations)
        EV_SetCharacterAlphaEnabled(name, enabled);
    for (auto name : authoredCharacters) {
        if (name.name == 0xffffffffU)
            break;
        if (!enabled)
            SetAuthoredRadius(name);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, enabled ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, enabled);
    }
}
void SelectAuthorPackage(std::uint16_t author) {
    const char *discard = author ? "AKA3" : "AB00";
    const char *load = author ? "AB00" : "AKA3";
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(discard), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(discard), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(load));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(load), 0);
}
void AuthorLoopCallback(void *parameter) {
    auto &work = *static_cast<AuthorLoopWork *>(parameter);
    work.frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.frame == 1)
        AuthoredFlags(false);
    else if (work.frame == 500 && work.parent->author == 2)
        EV_NPC_NewLoadSw(1);
    else if (work.frame == 600 && work.parent->author == 2)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        AuthoredFlags(true);
        work.parent->phase = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void QueueAuthorLoop(AuthorWork &work) {
    auto *task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 0x0b, sizeof(AuthorLoopWork), AREA_EVENT_TASK);
    auto *loop = static_cast<AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop->parent = &work;
    loop->frame = 0;
    EV_SetModelDataDefaultVertexShader();
    switch (work.author) {
    case 0:
        SetWeatherUpdateEnabledTo(1);
        EV_lightLoad(SHEN_NAME(l001));
        PushGlobalLightingProgram();
        break;
    case 1:
        SetWeatherUpdateEnabledTo(1);
        EV_lightLoad(SHEN_NAME(l101));
        PushGlobalLightingProgram();
        break;
    case 2:
        SetWeatherUpdateEnabledTo(0);
        EV_lightLoad(SHEN_NAME(l201));
        PushGlobalLightingProgram();
        EV_lightLoad(SHEN_NAME(l202));
        break;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author));
    work.phase = 2;
}
void AuthorCallback(void *parameter) {
    auto &work = *static_cast<AuthorWork *>(parameter);
    if (work.phase == 1)
        QueueAuthorLoop(work);
    else if (work.phase == 3) {
        EV_CleanupMapControlTask();
        HLib::DestroyTask(work.task);
    }
}
void QueueAuthor(std::uint8_t author) {
    auto *task = HLib::EnqueueTaskWithParameter(AuthorCallback, 4, 0x0b, sizeof(AuthorWork), AREA_EVENT_TASK);
    auto *work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->author = author;
    work->phase = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0138");
    SelectAuthorPackage(author);
}
void Pose(shen_name character, std::uint32_t animation, const Vector3 &position, float degrees) {
    EventSetMotion motion{0, character, animation, 0, 1, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    EventSetPosition pos{0, character, shen_name{0xffffffffU}, position, 0};
    EVT_AT_SetPosRot(&pos);
    EventSetAngle angle{0, character, shen_name{0xffffffffU}, degrees, 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void SetupBattle(AlleyBattleWork &work) {
    using enum CameraState;
    for (auto name : authoredCharacters) {
        if (name.name == 0xffffffffU)
            break;
        DisableScenarioCharacterDisplay(name);
        ToggleScenarioCharacterFlag(name);
    }
    for (auto name : decorations) {
        DisableScenarioCharacterDisplay(name);
        ToggleScenarioCharacterFlag(name);
    }
    for (auto name : enemies) {
        EnableScenarioCharacterDisplay(name);
        EnsureScenarioCharacterDraw(name);
    }
    Pose(ryo, 306, {251.66000366210938f, 96.8499984741211f, 492.2300109863281f}, 85.94000244140625f);
    Pose(enemies[0], 3632, {249.8300018310547f, 96.8499984741211f, 490.44000244140625f}, 235.94000244140625f);
    Pose(enemies[1], 3636, {249.74000549316406f, 96.8499984741211f, 494.19000244140625f}, 300.9200134277344f);
    Pose(enemies[2], 3640, {251.32000732421875f, 96.8499984741211f, 494.1300048828125f}, 5.909999847412109f);
    Pose(enemies[3], 3644, {252.0800018310547f, 96.8499984741211f, 490.9800109863281f}, 140.9600067138672f);
    Pose(enemies[4], 3648, {247.75999450683594f, 96.8499984741211f, 492.92999267578125f}, 295.9200134277344f);
    Pose(enemies[5], 3632, {248.72000122070312f, 96.8499984741211f, 489.739990234375f}, 265.92999267578125f);
    camCtrlSetState(Event);
    camCtrlSetNextPosition({251.15961f, 98.82f, 487.0921f});
    camCtrlSetTargetPosition({250.2169952392578f, 98.05000305175781f, 491.5154113769531f});
    EV_LoadAudio("0138A020", 1);
    EV_spmBeginSampleExecute();
    work.delay = 30;
    EventFilmControl film{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    camCtrlSetFov(0x1c71);
    work.phase = 8;
}
void FightFlags(Character *character) {
    vfUpdateFlagsFor(character, 0, 0x400000, 128);
    vfUpdateFlagsFor(character, 0, 0x200000, 2048);
}
void BeginFight(AlleyBattleWork &work) {
    shenStartFGST(9, 20, 0, 6);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("ABLB"), 1);
    constexpr Vector3 generators[]{{249.12f, 96.849998f, 489.23999f}, {246.92f, 96.849998f, 493.04001f}, {248.12f, 96.849998f, 495.73999f},
        {253.02f, 96.849998f, 496.23999f}, {254.92f, 96.849998f, 496.94f}, {253.12f, 96.849998f, 489.34f}};
    for (const auto &p : generators)
        vfAddGeneratorPoint(p.x, p.y, p.z);
    auto *player = ctrlGetCharaByName(ryo);
    shenInitFightData(player, nullptr, 0, 0, 0, 0);
    vfUpdateFlagsFor(player, 0, 0x20000, 31);
    for (std::size_t i = 0; i < enemies.size(); ++i) {
        auto *actor = ctrlGetCharaByName(enemies[i]);
        shenInitFightData(actor, nullptr, 1, 1, 100, static_cast<std::uint8_t>(i == 5 ? 2 : i + 2));
        FightFlags(actor);
    }

    for (const auto &spawn : reinforcements) {
        PublishFightRespawn(spawn.hp, spawn.style, spawn.animation, spawn.position.x, spawn.position.y, spawn.position.z, spawn.yaw);
        FightFlags(reinterpret_cast<Character *>(std::uintptr_t{SHEN_NAME(DEAD)}));
    }
    vfCopyPosFromEntity(0, nullptr, 250.95f, 492.92999f, 4.8f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    SetFaceFlagsForChara_0(player, FacialNodeOriginal);
    SetHighModelXYZ(player, 0, 1, 0);
    SetHighModelXYZ(player, 0, 0, 0);
    SetPluralControlFor(player, 1);
    for (auto name : characters) {
        if (name.name == 0xffffffffU)
            break;
        SetCharacterType(ctrlGetScenarioChara(name), CharacterTypeDefault);
    }
    vfmQueueFightStart(6, 2, 1, 1, -1);
    work.elapsed = 690;
    work.phase = 10;
}
void SaveWeatherColours(AlleyBattleWork &work) {
    SetWeatherUpdateEnabledTo(0);
    const auto sky = GetWeatherCloudOrSkyColour(0), cloud = GetWeatherCloudOrSkyColour(1);
    for (int i = 0; i < 4; ++i) {
        work.colours[i][2] = (sky >> (24 - 8 * i)) & 255;
        work.colours[i][4] = (cloud >> (24 - 8 * i)) & 255;
    }
}
void BlendWeatherColours(AlleyBattleWork &work) {
    auto &blend = work.colourBlend;
    if (!colourBlendInitialized) {
        colourBlendInitialized = true;
        blend[0] = std::bit_cast<float>(work.colours[0][3]);
        blend[8] = std::bit_cast<float>(work.colours[0][5]);
        for (int i = 0; i < 4; ++i) {
            blend[4 + i] = static_cast<float>(work.colours[i][2]);
            blend[12 + i] = static_cast<float>(work.colours[i][4]);
        }
        const float frames = 30.0f * work.fadeOutSeconds;
        for (int i = 1; i < 4; ++i) {
            const float target = static_cast<float>(work.colours[i][1]) * (i == 3 ? 1.0f : 2.0f);
            blend[i] = (target - blend[4 + i]) / frames;
            blend[8 + i] = (target - blend[12 + i]) / frames;
        }
    }
    if (!work.fadeOutFrames) {
        for (int i = 0; i < 4; ++i)
            work.colours[i][2] = work.colours[i][4] = work.colours[i][1];
    } else if (work.fadeOutFrames > 0) {
        work.skyColour = work.cloudColour = 0;
        for (int i = 0; i < 4; ++i) {
            if (i)
                blend[4 + i] += blend[i];
            blend[12 + i] += blend[8 + i];
            const auto sky = static_cast<std::uint32_t>(static_cast<std::int32_t>(blend[4 + i])) << (24 - 8 * i);
            const auto cloud = static_cast<std::uint32_t>(static_cast<std::int32_t>(blend[12 + i])) << (24 - 8 * i);
            work.colours[i][2] = static_cast<std::int32_t>(sky);
            work.colours[i][4] = static_cast<std::int32_t>(cloud);
            work.skyColour |= sky;
            work.cloudColour |= cloud;
        }
    }
    SetCloudAndSkyColour(work.skyColour, work.cloudColour);
}
void OutroVoices() {
    EVT_AT_SetVoice("S2BVA004", 1, 3);
    EVT_AT_SetSound(14, 17);
    EVT_AT_SetVoice("S2BVB030", 1, 18);
    EVT_AT_SetVoice("S2BVC006", 1, 33);
    EVT_AT_SetSound(34, 35);
    EVT_AT_SetSound(16, 39);
    EVT_AT_SetVoice("S2BVA007", 1, 51);
    EVT_AT_SetSound(34, 66);
    EVT_AT_SetVoice("S2BVB014", 1, 66);
    EVT_AT_SetSound(16, 83);
    EVT_AT_SetVoice("S2BVA012", 1, 84);
    EVT_AT_SetSound(38, 90);
    EVT_AT_SetVoice("S2BVA013", 1, 100);
    EVT_AT_SetSound(38, 125);
    EVT_AT_SetVoice("S2BVC011", 1, 130);
}
void Exit(AlleyBattleWork &work) {
    SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    EV_FreeAuthorPak(0, "S2BV");
    SetWeatherUpdateEnabledTo(1);
    colourBlendInitialized = false;
    ET_AT_KillWork(work.characters);
    GetNpcBatchCountOverride(work.npcBatchCount);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_scenQueueLoad(2, SHEN_NAME(0055), 6);
}
} // namespace alley_battle
void ABLB_init_func() {
    alley_battle::colourBlendInitialized = false;
}
void ABLB_main_func(std::uintptr_t) {
    using namespace alley_battle;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    for (int i = 0; i < 13; ++i)
        EV_SetObjectFlagByIndex(i, 0);
    auto *task = HLib::EnqueueTaskWithParameter(AlleyBattleCallback, 5, 0x0b, sizeof(AlleyBattleWork), AREA_EVENT_TASK);
    auto *work = static_cast<AlleyBattleWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->field3c = work->field40 = 0;
    work->npcBatchCount = GetNpcBatchCountOverride(35);
    work->characters = Queue_EVT_AT_ReadChr(task, characters.data());
    work->entry = static_cast<std::uint8_t>(g_entryPoint);
    work->reservedA0 = work->delay = 0;
    work->fadeOutFrames = work->fadeInFrames = 0;
    work->elapsed = 0;
    work->field08 = 0;
    work->field10 = -1;
    work->skyColour = work->cloudColour = 0;
    std::memset(work->colourBlend, 0, sizeof(work->colourBlend));
    work->outroFrames = 0;
    std::memset(work->colours, 0, sizeof(work->colours));
    work->colours[0][0] = work->colours[0][1] = 185;
    work->colours[1][0] = work->colours[1][1] = 35;
    work->colours[2][0] = work->colours[2][1] = 20;
    work->fadeOutSeconds = 12;
    work->fadeInSeconds = 1;
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ABLB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0138A010");
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
    EV_SetObjectFlagByIndex(10, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 0);
    Objects(true);
}
void AlleyBattleCallback(void *parameter) {
    using namespace alley_battle;
    auto &work = *static_cast<AlleyBattleWork *>(parameter);
    switch (work.phase) {
    case 0:
        for (auto name : enemies) {
            DisableScenarioCharacterDisplay(name);
            ToggleScenarioCharacterFlag(name);
        }
        EV_LoadAudio("0138A010", 0);
        EV_spmBeginSampleExecute();
        QueueAuthor(1);
        work.phase = 1;
        break;
    case 1:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.phase = 2;
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_LoadCutsceneFromCurrentScene("0138A020");
            EV_LoadAudio("0138A020", 0);
            EV_spmBeginSampleExecute();
            QueueAuthor(2);
            work.phase = 4;
        }
        break;
    case 4:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.phase = 5;
        break;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 250)) {
                EV_NPC_NewLoadSw(1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 1);
            }
            work.phase = 7;
        }
        break;
    case 7:
        SetWeatherUpdateEnabledTo(1);
        colourBlendInitialized = false;
        SetupBattle(work);
        break;
    case 8:
        HLib::SetupTaskDelay(work.delay);
        work.phase = 9;
        break;
    case 9:
        BeginFight(work);
        break;
    case 10:
        if (work.elapsed-- == 1) {
            vfmDispatchControl(FightControlOp::SpriteDepth, {}, 0, 1);
            SetScreenFilmFade(
                30 * work.fadeOutSeconds, 0, 0, 0, 0, work.colours[0][0] & 255, work.colours[1][0] & 255, work.colours[2][0] & 255, work.colours[3][0] & 255);
            work.fadeOutFrames = 30 * work.fadeOutSeconds;
            SaveWeatherColours(work);
            work.phase = 11;
        }
        break;
    case 11:
        if (--work.fadeOutFrames < 0) {
            SetScreenFilmFade(
                30 * work.fadeInSeconds, work.colours[0][0] & 255, work.colours[1][0] & 255, work.colours[2][0] & 255, work.colours[3][0] & 255, 255, 0, 0, 0);
            work.phase = 12;
            work.fadeInFrames = 30 * work.fadeInSeconds;
        } else
            BlendWeatherColours(work);
        break;
    case 12:
        if (--work.fadeInFrames < 0) {
            check_dead_status(1);
            EV_FreeAuthorPak(0, "0138");
            EV_stepSetFlags(0, 513);
            work.phase = 13;
        }
        break;
    case 13:
        if (!filmCheckFadeTrackColour()) {
            if (work.entry == 99)
                work.phase = 6;
            else {
                EV_NPC_NewLoadSw(0);
                EV_ResetFreeTalkFlags();
                EV_FreeAuthorPak(1, "S2BV");
                Objects(false);
                work.phase = 14;
            }
        }
        break;
    case 14:
        if (!work.outroFrames)
            OutroVoices();
        if (++work.outroFrames <= 180) {
            if (work.outroFrames == 150)
                spmFadeOut();
        } else
            work.phase = 15;
        break;
    case 15:
        Exit(work);
        break;
    default:
        return;
    }
}
const ScriptAreaDefinition areaABLBScriptDefinition{.areaId = SHEN_NAME(ABLB), .field04 = 0, .init = ABLB_init_func, .main = ABLB_main_func};
} // namespace shenmue::s2
#endif
