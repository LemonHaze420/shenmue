#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/qkqb_event.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QkqbMain(void *argument)
{
    Queue_eQKQB_EVT_StartQKuro((const int *)argument);
}
#elif defined(SHENMUE_EVENT_QKQB_CUTS)
int PlayQkqbIntro(shen_name enemy, int mode)
{
    int delay;
    EVT_AT_SetPos(QKQB_RYO, QKQB_BASE, 5.2f, 0.0f, 1.2f, 0);
    EVT_AT_SetAngY(QKQB_RYO, QKQB_BASE, 90.0f, 0);
    EVT_AT_SetPos(enemy, QKQB_BASE, -1.3f, 0.0f, -12.3f, 0);
    EVT_AT_SetAngY(enemy, QKQB_BASE, 180.0f, 0);
    if (mode == 3)
        EVT_AT_SetCam(7311, QKQB_BASE, QKQB_BASE, 0, 0, 125);
    else
        EVT_AT_SetCam(7305, QKQB_BASE, QKQB_BASE, 0, 0, 125);
    EVT_AT_SetCam(7301, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetCam(7304, QKQB_BASE, QKQB_BASE, 0, 0, 70);
    EVT_AT_SetMot(QKQB_RYO, 0xf005, 2.0f, 1.0f, 0x40000000U, 0);
    delay = EVT_AT_SetMot(QKQB_RYO, 8193, 2.0f, 1.0f, 0x20000000U, 120);
    EVT_AT_SetMot(QKQB_RYO, 8194, 2.0f, 1.0f, 0x20000000U, delay);
    EVT_AT_SetMot(enemy, 8219, 2.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetFaceLooks(QKQB_RYO, 1, 1, 1.0f, 0);
    EVT_AT_SetFaceLooks(QKQB_REN, 1, 1, 1.0f, 0);
    return delay + 80;
}

int PlayQkqbFailure(shen_name enemy, int mode)
{
    switch (mode) {
    case 0:
        EVT_AT_SetVoice("0299A003", 55);
        EVT_AT_SetVoice("0299D001", 70);
        EVT_AT_SetVoice("0299A007", 95);
        break;
    case 1:
        EVT_AT_SetVoice("0299A004", 50);
        EVT_AT_SetVoice("0299E001", 70);
        EVT_AT_SetVoice("0299A009", 110);
        break;
    case 2:
        EVT_AT_SetVoice("0299A003", 55);
        EVT_AT_SetVoice("0299F001", 70);
        EVT_AT_SetVoice("0299A010", 110);
        break;
    case 3:
        EVT_AT_SetVoice("0299A003", 55);
        EVT_AT_SetVoice("0299G001", 70);
        EVT_AT_SetVoice("0299A011", 110);
        break;
    default:
        EVT_AT_SetVoice("0299A004", 50);
        EVT_AT_SetVoice("0299H001", 70);
        EVT_AT_SetVoice("0299A012", 120);
        break;
    }
    EVT_AT_SetPosAng(QKQB_RYO, QKQB_RYO, 0.0f, -0.3f, -90.0f, 0);
    EVT_AT_SetPosAng(enemy, QKQB_RYO, -1.608f, -2.52417f, 0.0f, 0);
    EVT_AT_SetCam(7302, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetMot(QKQB_RYO, 8208, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(enemy, 8202, 0.0f, 1.0f, 0x40000000U, 0);
    return 160;
}

int PlayQkqbSuccess(shen_name enemy, int mode)
{
    int frames;
    Queue_EVT_AT_CtrlHandUp_1(QKQB_RYO, 0, (void *)areaLoopHandPoses[QKQB_OPEN_HAND_POSE], 10, 0);
    Queue_EVT_AT_CtrlHandUp_1(QKQB_RYO, 1, (void *)areaLoopHandPoses[QKQB_CLOSED_HAND_POSE], 10, 0);
    switch (mode) {
    case 0:
        frames = PlayQkqbSuccessCut0(enemy);
        break;
    case 1:
        frames = PlayQkqbSuccessCut1(enemy);
        break;
    case 2:
        frames = PlayQkqbSuccessCut2(enemy);
        break;
    case 3:
        frames = PlayQkqbSuccessCut3(enemy);
        break;
    default:
        frames = PlayQkqbSuccessCut4(enemy);
        break;
    }
    return frames;
}

int PlayQkqbSuccessCut4(shen_name enemy)
{
    EVT_AT_SetPosAng(QKQB_RYO, QKQB_RYO, 0.0f, -0.3f, -90.0f, 0);
    EVT_AT_SetPosAng(enemy, QKQB_RYO, -1.08371f, -1.93784f, 0.0f, 0);
    EVT_AT_SetCam(7306, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetMot(QKQB_RYO, 8207, 2.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(enemy, 8201, 2.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_DispBlur(QKQB_RYO, 1, 6, 1, 0, 10, 75);
    EVT_AT_DispBlur(QKQB_RYO, 4, 12, 1, 0, 10, 105);
    EVT_AT_DispBlur(QKQB_RYO, 1, 8, 1, 0, 5, 147);
    EVT_AT_DispBlur(QKQB_RYO, 2, 6, 1, 0, 10, 201);
    return 300;
}

int PlayQkqbSuccessCut2(shen_name enemy)
{
    EVT_AT_SetPosAng(QKQB_RYO, QKQB_RYO, 0.0f, -0.3f, -90.0f, 0);
    EVT_AT_SetPosAng(enemy, QKQB_RYO, -1.49543f, -2.30519f, 0.0f, 0);
    EVT_AT_SetCam(7307, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetMot(QKQB_RYO, 8205, 70.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(enemy, 8200, 70.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_DispBlur(QKQB_RYO, 1, 6, 1, 0, 10, 62);
    EVT_AT_DispBlur(QKQB_RYO, 1, 8, 1, 0, 12, 88);
    return 170;
}

int PlayQkqbSuccessCut3(shen_name enemy)
{
    EVT_AT_SetPosAng(QKQB_RYO, QKQB_RYO, 0.0f, -0.3f, -90.0f, 0);
    EVT_AT_SetPosAng(enemy, QKQB_RYO, -1.82542f, -2.21864f, 0.0f, 0);
    EVT_AT_SetCam(7308, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetMot(QKQB_RYO, 8206, 125.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(enemy, 8210, 125.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_DispBlur(QKQB_RYO, 1, 6, 1, 0, 10, 45);
    EVT_AT_DispBlur(QKQB_RYO, 7, 12, 1, 0, 10, 60);
    EVT_AT_DispBlur(QKQB_RYO, 1, 6, 1, 0, 10, 98);
    return 150;
}

int PlayQkqbSuccessCut1(shen_name enemy)
{
    EVT_AT_SetPosAng(QKQB_RYO, QKQB_RYO, 0.0f, -0.3f, -90.0f, 0);
    EVT_AT_SetPosAng(enemy, QKQB_RYO, -1.608f, -2.52417f, 0.0f, 0);
    EVT_AT_SetCam(7309, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetMot(QKQB_RYO, 8204, 60.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(enemy, 8199, 60.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_DispBlur(QKQB_RYO, 7, 12, 1, 0, 12, 75);
    EVT_AT_DispBlur(QKQB_RYO, 1, 8, 1, 0, 10, 100);
    return 200;
}

int PlayQkqbSuccessCut0(shen_name enemy)
{
    EVT_AT_SetPosAng(enemy, QKQB_RYO, 1.5f, -1.5f, 90.0f, 0);
    EVT_AT_SetPosAng(enemy, enemy, 1.0f, 1.0f, 0.0f, 50);
    EVT_AT_SetPosAng(QKQB_RYO, enemy, -0.5f, -1.5f, 180.0f, 50);
    EVT_AT_SetCam(7310, QKQB_BASE, QKQB_BASE, 0, 0, 0);
    EVT_AT_SetCam(7303, QKQB_BASE, QKQB_BASE, 0, 0, 50);
    EVT_AT_SetMot(QKQB_RYO, 8196, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(QKQB_RYO, 8195, 2.0f, 1.0f, 0x40000000U, 50);
    EVT_AT_SetMot(enemy, 8197, 0.0f, 1.0f, 0, 36);
    EVT_AT_SetMot(enemy, 8198, 2.0f, 1.0f, 0x40000000U, 50);
    EVT_AT_DispBlur(QKQB_RYO, 4, 12, 1, 0, 15, 20);
    EVT_AT_DispBlur(QKQB_RYO, 1, 8, 1, 0, 10, 50);
    return 100;
}
#else
static const QKQBAuthoredEntry qkqbAuthoredEntries[] = {
    {4, {216.1f, 0.0f, 145.8f}, 90.0f, {217.4f, 0.0f, 149.0f}, 5.0f, 1, 200, 2, {214.0f, 0.0f, 147.0f}, 90.0f, {16, 512, 0, 0}, 16, 24, 2},
    {2, {247.5f, 0.0f, 149.3f}, 180.0f, {250.2f, 0.0f, 147.7f}, 5.0f, 1, 300, 2, {248.6f, 0.0f, 149.5f}, 180.0f, {4, 512, 256, 0}, 24, 36, 3},
    {3, {250.4f, 0.0f, 165.5f}, 0.0f, {248.7f, 0.0f, 166.4f}, 3.0f, 1, 400, 2, {248.7f, 0.0f, 165.2f}, 0.0f, {256, 1, 256, 0}, 24, 36, 3},
    {1, {199.8f, 0.0f, 176.4f}, 0.0f, {197.5f, 0.0f, 177.9f}, 5.0f, 1, 500, 5, {199.0f, 0.0f, 176.3f}, 0.0f, {256, 64, 1024, 0}, 24, 36, 3},
    {0, {218.8f, 0.0f, 127.3f}, 270.0f, {217.6f, 0.0f, 124.2f}, 5.0f, 1, 600, 6, {219.0f, 0.0f, 125.9f}, -90.0f, {1024, 4, 4, 1024}, 32, 48, 4}};
static const shen_name qkqbEnemyNames[] = {QKQB_KF5, QKQB_KR3, QKQB_KF2, QKQB_KF3, QKQB_BF7};
static const int qkqbEntryPoints[] = {0, 1, 2, 3, 4};
static const int qkqbAudioVariants[] = {0, 0, 1, 1, 2};
static shen_name qkqbCharacterNames[] = {QKQB_RYO, QKQB_REN, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID,
    EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID};

void Queue_eQKQB_EVT_StartQKuro(const int *entry)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(eQKQB_EVT_StartQKuro_Callback, 4, 11, sizeof(QKQBEventWork), TASK_EVNT);
    QKQBEventWork *work = (QKQBEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    if (entry)
        work->entry = entry;
    else
        work->entry = &qkqbEntryPoints[g_entryPoint];
    InitializeQkqbResources(work);
    EventDebugRegister(QKQB_DEBUG_ID, QkqbDebugExitCallback, work, 2000000000);
}

void QkqbDebugExitCallback(void *data)
{
    FinishQkqbEvent((QKQBEventWork *)data);
}

void FinishQkqbEvent(QKQBEventWork *work)
{
    CleanupQkqbResources(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(QKQB_DEBUG_ID);
    if (g_currentMap == QKQB_AREA) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

void eQKQB_EVT_StartQKuro_Callback(void *data)
{
    QKQBEventWork *work = (QKQBEventWork *)data;
    const QKQBAuthoredEntry *authored = work->authored;
    int delay;
    EventDebugCheck(QKQB_DEBUG_ID, 4);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QKQB_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QKQB_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QKQB_RYO), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(QKQB_RYO, 0, (void *)areaLoopHandPoses[QKQB_OPEN_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(QKQB_RYO, 1, (void *)areaLoopHandPoses[QKQB_OPEN_HAND_POSE], 0, 0);
    FTBL.AT_SetLocalBaseChara(QKQB_DUMMY_RYO, QKQB_RYO);
    EVT_AT_SetPos(QKQB_BASE, EVENT_CHARACTER_INVALID, authored->basePosition.x, work->playerY, authored->basePosition.z, 0);
    EVT_AT_SetAngY(QKQB_BASE, EVENT_CHARACTER_INVALID, authored->baseYaw, 0);
    EVT_AT_SetPos(QKQB_DUMMY_RYO, QKQB_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QKQB_DUMMY_RYO, QKQB_RYO, 0.0f, 0);
    EVT_AT_SetPos(QKQB_REN, QKQB_BASE, 1000.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QKQB_REN, QKQB_BASE, 0.0f, 0);
    FTBL.EV_LoadAudio("0299E000", qkqbAudioVariants[*work->entry]);
    EVT_AT_SetSample(0, 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    delay = PlayQkqbIntro(qkqbCharacterNames[2], authored->cutMode);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    FTBL.qteBattleUtil(9, authored->commands[0], authored->commands[1], authored->commands[2], authored->commands[3]);
    FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    EVT_AT_QTE(0, authored->maximumQteFrames, 0, delay, 4);
    FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_WaitQTE_Callback);
    FTBL.HLib_SetupTaskDelay(delay);
}

void eQKQB_EVT_WaitQTE_Callback(void *data)
{
    const QKQBAuthoredEntry *authored = ((QKQBEventWork *)data)->authored;
    int delay;
    if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
        if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == authored->requiredCommands) {
            delay = PlayQkqbSuccess(qkqbCharacterNames[2], authored->cutMode);
            EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay);
            EVT_AT_SetSample(0, 1, delay);
            FTBL.qteBattleUtil(5, 0, 0, 0, 0);
            FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
            FTBL.HLib_SetupTaskDelay(delay + 60);
        } else {
            delay = PlayQkqbFailure(qkqbCharacterNames[2], authored->cutMode);
            FTBL.qteBattleUtil(5, 0, 0, 0, 0);
            FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_StartBattle_Callback);
            FTBL.HLib_SetupTaskDelay(delay);
        }
    }
}

void eQKQB_EVT_StartBattle_Callback(void *data)
{
    const QKQBAuthoredEntry *authored = ((QKQBEventWork *)data)->authored;
    int index;
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QKQB_RYO), 0, 0, 0, 0, 0);
    EVT_AT_SetPos(QKQB_RYO, QKQB_RYO, 0.0f, 0.2f, 0.0f, 0);
    for (index = 0; index < authored->enemyCount; index++) {
        EVT_AT_SetPos(qkqbCharacterNames[index + 2], qkqbCharacterNames[index + 2], 0.0f, 0.2f, 0.0f, 0);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qkqbCharacterNames[index + 2]), 0, 1, 1, authored->enemyHp, authored->fightStyle);
    }
    FTBL.vfCopyPosFromEntity(0, 0, authored->battlePosition.x, authored->battlePosition.z, authored->battleRadius);
    FTBL.EV_vfBattleUtil(3, 0x80, 1);
    FTBL.vfmQueueFightStart(authored->enemyCount + 1, 2, 1.0f, 1, -1);
    FTBL.EV_vfBattleUtil(1, 2, 1);
    FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_PlayBattle_Callback);
}

void eQKQB_EVT_PlayBattle_Callback(void *data)
{
    QKQBEventWork *work = (QKQBEventWork *)data;
    const QKQBAuthoredEntry *authored = work->authored;
    if (FTBL.IsFightActive())
        return;
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
    EVT_AT_SetSample(0, 1, 0);
    if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QKQB_RYO)) == 3) {
        EVT_AT_SetPos(QKQB_RYO, EVENT_CHARACTER_INVALID, authored->returnPosition.x, work->playerY, authored->returnPosition.z, 30);
        EVT_AT_SetAngY(QKQB_RYO, EVENT_CHARACTER_INVALID, authored->returnYaw, 30);
        EVT_AT_SetMot(QKQB_RYO, 0xf003, 1.0f, 1.0f, 0x40000000U, 30);
        FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
    } else {
        EVT_IncrementQteFailureCount();
        RestoreEventDateTime();
        FTBL.Queue_ST_LoadTitleFadeIn(QKQB_RETRY_TITLE);
        FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_RetryQKuro_Callback);
    }
    FTBL.HLib_SetupTaskDelay(60);
}

void eQKQB_EVT_RetryQKuro_Callback(void *data)
{
    FTBL.ST_LoadTitleFade();
    FTBL.HLib_SetCurrentTaskTo(eQKQB_EVT_StartQKuro_Callback);
    FTBL.HLib_SetupTaskDelay(30);
}

void eQKQB_EVT_EndQKuro_Callback(void *data)
{
    FinishQkqbEvent((QKQBEventWork *)data);
}

void InitializeQkqbResources(QKQBEventWork *work)
{
    Vector3 position;
    int index;
    work->authored = &qkqbAuthoredEntries[*work->entry];
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(work->authored->retryQteFrames);
    SaveEventDateTime();
    for (index = 0; index < work->authored->enemyCount; index++)
        qkqbCharacterNames[index + 2] = qkqbEnemyNames[*work->entry];
    work->characters = Queue_EVT_AT_ReadChr(work->task, qkqbCharacterNames);
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QKQB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QKQB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QKQB", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0299");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0299E000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.GetCharaPosition(QKQB_RYO, 0xffffffffU, &position, 0x40000000U);
    work->playerY = position.y;
}

void CleanupQkqbResources(QKQBEventWork *work)
{
    const QKQBAuthoredEntry *authored = work->authored;
    EVT_AT_SetPos(QKQB_RYO, EVENT_CHARACTER_INVALID, authored->returnPosition.x, work->playerY, authored->returnPosition.z, 0);
    EVT_AT_SetAngY(QKQB_RYO, EVENT_CHARACTER_INVALID, authored->returnYaw, 0);
    EVT_AT_SetMot(QKQB_RYO, 0xf003, 1.0f, 1.0f, 0x40000000U, 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QKQB_RYO));
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0299");
    FTBL.EV_ControlPackageResources(0, 6, "QKQB", 0, 0);
    FTBL.DeleteMotion(work->motion);
    ET_AT_KillWork(work->characters);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}
#endif
#else
#include "s2/qkqb_event.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, base{SHEN_NAME(BASE)}, invalid{0xffffffffU};
constexpr std::array<std::int32_t, 6> audioVariants{0, 0, 1, 1, 2, 0};

std::array<shen_name, 8> characterNames{ryo, ren, invalid, invalid, invalid, invalid, invalid, shen_name{}};
DateTime savedDateTime{};
std::int32_t handCommandOpen[19][3] = {{0, 0, 46606}, {0, 2182, 64444}, {0, 0, 1456}, {0, 64080, 49154}, {0, 908, 44239}, {0, 0, 53340}, {0, 0, 0},
    {0, 1456, 54796}, {0, 0, 43692}, {0, 727, 50974}, {0, 0, 0}, {0, 2912, 54797}, {0, 182, 44421}, {0, 1456, 50245}, {0, 0, 0}, {2912, 910, 51336},
    {726, 726, 47517}, {0, 2003, 50974}, {65518, 65427, 0}};
std::int32_t handCommandClosed[19][3] = {{1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335}, {0, 0, 60075},
    {0, 0, 64811}, {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256}, {0, 0, 0},
    {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};

void Position(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, character, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Angle(shen_name character, shen_name parent, float degrees) {
    EventSetAngle request{0, character, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void PositionAngle(shen_name character, shen_name parent, const Vector3 &position, float degrees) {
    EventSetPositionAngle request{0, character, parent, position, degrees, 0};
    EVT_AT_SetPosAng_Callback(&request);
}
void Motion(shen_name character, std::uint32_t animation, float frame) {
    EventSetMotion request{0, character, animation, frame, 1, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Camera(std::int32_t camera) {
    EventSetCamera request{0, camera, base, base, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void FadeFromBlack() {
    EventFilmControl request{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&request);
}
std::int32_t MotionFrames(std::uint16_t motion) {
    const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
    return sequence ? sequence->frameCount : 0;
}


std::int32_t Intro(shen_name enemy, std::int32_t mode) {
    Position(ryo, base, {5.2f, 0, 1.2f});
    Angle(ryo, base, 90);
    Position(enemy, base, {-1.3f, 0, -12.3f});
    Angle(enemy, base, 180);
    EVT_AT_SetCam(mode == 3 ? 7311 : 7305, base, base, 0, 0, 125);
    Camera(7301);
    EVT_AT_SetCam(7304, base, base, 0, 0, 70);
    Motion(ryo, 0xf005, 2);
    const std::int32_t delay = EVT_AT_SetMot(ryo, 8193, 2, 1, 0x20000000U, 120);
    EVT_AT_SetMot(ryo, 8194, 2, 1, 0x20000000U, delay);
    Motion(enemy, 8219, 2);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(ryo), 1, 1, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(ren), 1, 1, 1);
    return delay + 80;
}

std::int32_t Failure(shen_name enemy, std::int32_t mode) {
    switch (mode) {
    case 0: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299D001",1,70); EVT_AT_SetVoice("0299A007",1,95); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299D001 EN: "Who are you!?" | JA: "なんだキサマァ！" ; 0299A007 EN: "Can't be helped!" | JA: "仕方ないっ！"
    case 1: EVT_AT_SetVoice("0299A004",1,50); EVT_AT_SetVoice("0299E001",1,70); EVT_AT_SetVoice("0299A009",1,110); break; // @text 0299A004 EN: " " | JA: "うっ…" ; 0299E001 EN: "You=@!" | JA: "おっ…テメェ！" ; 0299A009 EN: "=@gotta fight!" | JA: "ちっ…やるか…！"
    case 2: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299F001",1,70); EVT_AT_SetVoice("0299A010",1,110); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299F001 EN: "Who are you!?" | JA: "なんだテメェは！？" ; 0299A010 EN: "Damn!" | JA: "しまった！"
    case 3: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299G001",1,70); EVT_AT_SetVoice("0299A011",1,110); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299G001 EN: "What are you doing?" | JA: "ここで何してる！？" ; 0299A011 EN: "There's no choice=@" | JA: "やるしかないか…"
    default: EVT_AT_SetVoice("0299A004",1,50); EVT_AT_SetVoice("0299H001",1,70); EVT_AT_SetVoice("0299A012",1,120); break; // @text 0299A004 EN: " " | JA: "うっ…" ; 0299H001 EN: "=@Who are you?" | JA: "おっ…何者だ！？" ; 0299A012 EN: "Damn=@!" | JA: "しまった…！"
    }
    PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
    PositionAngle(enemy, ryo, {-1.608f, 0, -2.524169921875f}, 0);
    Camera(7302);
    Motion(ryo, 8208, 0);
    Motion(enemy, 8202, 0);
    return 160;
}

std::int32_t Success(shen_name enemy, std::int32_t mode) {
    Character *const character = ctrlGetScenarioChara(ryo);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(handCommandOpen), 10);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(handCommandClosed), 10);
    switch (mode) {
    case 0:
        PositionAngle(enemy, ryo, {1.5f, 0, -1.5f}, 90);
        EVT_AT_SetPosAng(enemy, enemy, 1, 1, 0, 50);
        EVT_AT_SetPosAng(ryo, enemy, -0.5f, -1.5f, 180, 50);
        Camera(7310);
        EVT_AT_SetCam(7303, base, base, 0, 0, 50);
        Motion(ryo, 8196, 0);
        EVT_AT_SetMot(ryo, 8195, 2, 1, 0x40000000U, 50);
        EVT_AT_SetMot(enemy, 8197, 0, 1, 0, 36);
        EVT_AT_SetMot(enemy, 8198, 2, 1, 0x40000000U, 50);
        EVT_AT_DispBlur(ryo, 4, 12, 1, 15, 20);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 10, 50);
        return 100;
    case 1:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.608f, 0, -2.524169921875f}, 0);
        Camera(7309);
        Motion(ryo, 8204, 60);
        Motion(enemy, 8199, 60);
        EVT_AT_DispBlur(ryo, 7, 12, 1, 12, 75);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 10, 100);
        return 200;
    case 2:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.49543f, 0, -2.30519f}, 0);
        Camera(7307);
        Motion(ryo, 8205, 70);
        Motion(enemy, 8200, 70);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 62);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 12, 88);
        return 170;
    case 3:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.82542f, 0, -2.21864f}, 0);
        Camera(7308);
        Motion(ryo, 8206, 125);
        Motion(enemy, 8210, 125);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 45);
        EVT_AT_DispBlur(ryo, 7, 12, 1, 10, 60);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 98);
        return 150;
    default:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.08371f, 0, -1.93784f}, 0);
        Camera(7306);
        Motion(ryo, 8207, 2);
        Motion(enemy, 8201, 2);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 75);
        EVT_AT_DispBlur(ryo, 4, 12, 1, 10, 105);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 5, 147);
        EVT_AT_DispBlur(ryo, 2, 6, 1, 10, 201);
        return 300;
    }
}
void Setup(QKQBEventWork &work) {
    work.authored = &qkqbAuthoredEntries.data()[*work.entry];
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteFailureCount(0);
    EVT_SetQteRetryFrames(work.authored->retryQteFrames);
    shenGetCurrentDateTime(savedDateTime);
    for (std::int32_t i = 0; i < work.authored->enemyCount; ++i)
        characterNames.data()[2 + i] = qkqbEnemyNames.data()[*work.entry];
    work.characters = Queue_EVT_AT_ReadChr(work.task, characterNames.data());
    work.motionHandle = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QKQB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    EV_FreeAuthorPak(1, "0299");
    EV_SetNewSceneIndex();
    LoadCutsceneSoundProgram("0299E000", g_actNumber);
    shenSetClockAndCompassRenderTo(1);
    Vector3 position{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work.playerY = position.y;
}
void Cleanup(QKQBEventWork &work) {
    const auto &authored = *work.authored;
    Position(ryo, invalid, {authored.returnPosition.x, work.playerY, authored.returnPosition.z});
    Angle(ryo, invalid, authored.returnYaw);
    Motion(ryo, 0xf003, 1);
    hndlSetFlags(ctrlGetScenarioChara(ryo));
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0299");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    DeleteMotion(static_cast<std::int32_t>(work.motionHandle));
    ET_AT_KillWork(work.characters);
    FadeFromBlack();
    EV_NPC_NewLoadSw(1);
}

void InitQKQB() {
}
}

const ScriptAreaDefinition qkqbScriptDefinition{SHEN_NAME(QKQB), 0, InitQKQB, Queue_eQKQB_EVT_StartQKuro};

void Queue_eQKQB_EVT_StartQKuro(std::uintptr_t entry) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(eQKQB_EVT_StartQKuro_Callback, 4, 0x0b, sizeof(QKQBEventWork), SHEN_NAME(EVNT));
    auto &work = *static_cast<QKQBEventWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.entry = entry ? reinterpret_cast<const std::int32_t *>(entry) : &qkqbEntryPoints.data()[g_entryPoint];
    Setup(work);
}
void eQKQB_EVT_StartQKuro_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto &authored = *work.authored;
    Character *const character = ctrlGetScenarioChara(ryo);
    SetLowQualityHands(character);
    SetHighModelXYZ(character, true, 0, 0);
    SetHighModelXYZ(character, true, 1, 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(handCommandOpen), 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(handCommandOpen), 0);
    AT_SetLocalBaseChara(SHEN_NAME(DUMR), ryo);
    Position(base, invalid, {authored.basePosition.x, work.playerY, authored.basePosition.z});
    Angle(base, invalid, authored.baseYaw);
    Position(SHEN_NAME(DUMR), ryo, {});
    Angle(SHEN_NAME(DUMR), ryo, 0);
    Position(ren, base, {1000, 0, 0});
    Angle(ren, base, 0);
    EV_LoadAudio("0299E000", audioVariants.data()[*work.entry]);
    EV_spmBeginSampleExecute();
    FadeFromBlack();
    const std::int32_t delay = Intro(characterNames[2], authored.cutMode);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    qteBattleUtil(BattleQteMode::SetFirstCommands, authored.commands[0], authored.commands[1], authored.commands[2], authored.commands[3]);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    EVT_AT_QTE(0, authored.maximumQteFrames, 4, delay);
    HLib::SetCurrentTaskTo(eQKQB_EVT_WaitQTE_Callback);
    HLib::SetupTaskDelay(delay);
}
void eQKQB_EVT_WaitQTE_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    // 27122d tests EAX for zero: FBQT publishes +1/-1 only after its result lifetime.
    if (qteBattleUtil(BattleQteMode::GetResult) == 0)
        return;
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == work.authored->requiredCommands) {
        const std::int32_t delay = Success(characterNames[2], work.authored->cutMode);
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(delay)});
        EVT_AT_FadeBGM(0, 1, delay);
        qteBattleUtil(BattleQteMode::RetireAndFree);
        HLib::SetCurrentTaskTo(eQKQB_EVT_WaitPA_Callback);
        work.remainingFrames = delay + 60;
    } else {
        const std::int32_t delay = Failure(characterNames[2], work.authored->cutMode);
        qteBattleUtil(BattleQteMode::RetireAndFree);
        HLib::SetCurrentTaskTo(eQKQB_EVT_StartBattle_Callback);
        HLib::SetupTaskDelay(delay);
    }
}
void eQKQB_EVT_WaitPA_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (authGetFinished() && IsScreenFilmNotShowing(control->screenFilmTask))
        work.remainingFrames = 0;
    if (work.remainingFrames-- <= 0)
        HLib::SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
}
void eQKQB_EVT_StartBattle_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto &authored = *work.authored;
    shenStartFGST(10, 10, 0, 0);
    PublishFightFormation(ctrlGetScenarioChara(ryo), nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    Position(ryo, ryo, {0, 0.2f, 0});
    for (std::int32_t i = 0; i < authored.enemyCount; ++i) {
        const shen_name enemy = characterNames.data()[2 + i];
        Position(enemy, enemy, {0, 0.2f, 0});
        PublishFightFormation(ctrlGetScenarioChara(enemy), nullptr, 1, 1, static_cast<std::uint16_t>(authored.enemyHp),
            static_cast<std::uint8_t>(authored.fightStyle), 0, 0, 0, 0, 0);
    }
    vfCopyPosFromEntity(0, nullptr, authored.battlePosition.x, authored.battlePosition.z, authored.battleRadius);
    g_pFightStage->battleFlags |= 0x80U;
    vfmQueueFightStart(static_cast<std::uint8_t>(authored.enemyCount + 1), 2, 1, 1, -1);
    EV_camCtrlToggleCombatCamera(2, 1);
    HLib::SetCurrentTaskTo(eQKQB_EVT_PlayBattle_Callback);
}
void eQKQB_EVT_PlayBattle_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    if (g_fightActive)
        return;
    EventFilmControl fade{0, 30, 0, 0, 0, 0, 255, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&fade);
    spmFadeOut();
    if (GetBtlStatus(ctrlGetScenarioChara(ryo)) == 3) {
        EVT_AT_SetPos(ryo, invalid, {work.authored->returnPosition.x, work.playerY, work.authored->returnPosition.z}, 30);
        EVT_AT_SetAngY(ryo, invalid, work.authored->returnYaw, 30);
        EVT_AT_SetMot(ryo, 0xf003, 1, 1, 0x40000000U, 30);
        HLib::SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
    } else {
        const std::uint32_t failures = EVT_GetQteFailureCount();
        if (failures < 2)
            EVT_SetQteFailureCount(failures + 1);
        SetCurrentDateTimeTo(savedDateTime);
        shenSetTimeSimulationTo(1);
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
        HLib::SetCurrentTaskTo(eQKQB_EVT_RetryQKuro_Callback);
    }
    HLib::SetupTaskDelay(60);
}
void eQKQB_EVT_RetryQKuro_Callback(void *) {
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(eQKQB_EVT_StartQKuro_Callback);
    HLib::SetupTaskDelay(30);
}
void eQKQB_EVT_EndQKuro_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    Cleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == SHEN_NAME(QKQB)) {
        scenSetTaskFlags(6);
        SetPlayerControlEnabled(0);
        const auto module =
            static_cast<SystemModule>(((g_actNumber >> 31) & static_cast<std::int32_t>(SystemModule::Game)) + static_cast<std::int32_t>(SystemModule::Game));
        shenQueueChangeModuleAndScene(module, g_actNumber, g_currentMap, g_entryPoint);
    }
}
} // namespace shenmue::s2
#endif
