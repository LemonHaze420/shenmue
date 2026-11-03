#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_tkyb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunTkybEvent(int argument)
{
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TKYB_RYO), TKYB_IDLE_MOTION, 0);
    QueueClearNpcs(TkybClearedCallback, 0);
    FTBL.flddSetPointFlagOnGrid(TKYB_BATTLE_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(TKYB_DEFAULT_GRID, 0);
}
#else
const shen_name tkybCharacters[] = {TKYB_RYO, TKYB_GEN, TKYB_RNT, TKYB_VUN, TKYB_CHO, TKYB_CHY, TKYB_IKO, TKYB_RIK, TKYB_SBK, TKYB_INVALID};
static signed char tkybStartDay;
static void *tkybMotion;

void TkybClearedCallback(void *parameter)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(TkybStartCallback, 4, 11, sizeof(TkybWork), TASK_EVNT);
    TkybInitialize(task);
}

void TkybInitialize(HLibTask *task)
{
    TkybWork *work = (TkybWork *)FTBL.HLib_GetTaskParameter(task);
    EventDateTime time;
    work->task = task;
    work->characters = Queue_EVT_AT_ReadChr(task, tkybCharacters);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    tkybStartDay = time.day;
    QueueEventAbort(TKYB_ABORT_SLOT, TkybAbortCallback, work, TKYB_ABORT_FRAMES);
}

void TkybStartCallback(void *data)
{
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "TKYB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "TKYB", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0040A000");
    FTBL.EV_FreeAuthorPak(1, "0040");
    EVT_AT_SetPos(TKYB_RYO, TKYB_INVALID, 45.0f, 102.24f, 245.0f, 0);
    EVT_AT_SetAngY(TKYB_RYO, TKYB_INVALID, 107.0f, 0);
    EVT_AT_SetPos(TKYB_VUN, TKYB_INVALID, 44.2f, 102.24f, 247.8f, 0);
    EVT_AT_SetAngY(TKYB_VUN, TKYB_INVALID, 307.0f, 0);
    EVT_AT_SetPos(TKYB_SBK, TKYB_INVALID, 42.7f, 102.24f, 247.6f, 0);
    EVT_AT_SetAngY(TKYB_SBK, TKYB_INVALID, 310.0f, 0);
    EVT_AT_SetPos(TKYB_RNT, TKYB_INVALID, 43.6f, 102.24f, 246.2f, 0);
    EVT_AT_SetAngY(TKYB_RNT, TKYB_INVALID, 280.0f, 0);
    EVT_AT_SetPos(TKYB_IKO, TKYB_INVALID, 42.2f, 102.24f, 244.4f, 0);
    EVT_AT_SetAngY(TKYB_IKO, TKYB_INVALID, 270.0f, 0);
    EVT_AT_SetPos(TKYB_RIK, TKYB_INVALID, 43.1f, 102.24f, 243.4f, 0);
    EVT_AT_SetAngY(TKYB_RIK, TKYB_INVALID, 245.0f, 0);
    EVT_AT_SetPos(TKYB_GEN, TKYB_INVALID, 41.6f, 102.24f, 243.2f, 0);
    EVT_AT_SetAngY(TKYB_GEN, TKYB_INVALID, 245.0f, 0);
    TkybPrepareCharacter(TKYB_RYO, 1);
    TkybPrepareCharacter(TKYB_VUN, 1);
    TkybPrepareCharacter(TKYB_SBK, 1);
    TkybPrepareCharacter(TKYB_RNT, 1);
    TkybPrepareCharacter(TKYB_IKO, 1);
    TkybPrepareCharacter(TKYB_RIK, 1);
    TkybPrepareCharacter(TKYB_GEN, 1);
    TkybPrepareCharacter(TKYB_PROP_A132, 0);
    TkybPrepareCharacter(TKYB_PROP_A129, 0);
    TkybPrepareCharacter(TKYB_PROP_A038, 0);
    TkybPrepareCharacter(TKYB_KNIFE_A, 0);
    TkybPrepareCharacter(TKYB_KNIFE_B, 0);
    TkybPrepareCharacter(TKYB_KNIFE_C, 0);
    TkybPrepareCharacter(TKYB_KNIFE_D, 0);
    TkybPrepareCharacter(TKYB_PROP_0001, 0);
    FTBL.HLib_SetCurrentTaskTo(TkybFadeInCallback);
}

void TkybFadeInCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetEventAbortFlags(TKYB_ABORT_SLOT, 4);
    FTBL.EV_LoadAudio("0040A000", 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB_A0", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(TkybKnifeStartCallback);
    QueueSkAuthorStart(work->task, TKYB_START_AUTHOR, TkybStartAuthorCallback, 0, 0);
}

void TkybStartAuthorCallback(void *data)
{
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 1195) {
        FTBL.SetIsSkySetup(0);
        FTBL.SetWeatherUpdateEnabledTo(0);
        FTBL.EV_lightLoad(1);
    } else if (frame == 1364) {
        FTBL.SetIsSkySetup(1);
        FTBL.SetWeatherUpdateEnabledTo(1);
        FTBL.EV_lightLoad(0);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
    }
}

void TkybKnifeStartCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    int i;
    work->frame = 200;
    for (i = 0; i < 5; i++)
        work->reserved10[i] = -1;
    work->dummyCounter = 0;
    FTBL.EV_ControlPackageResources(2, 4, "TKYB", 0, 0);
    FTBL.HLib_SetCurrentTaskTo(TkybKnifeCameraCallback);
    TkybUpdateKnives(work, work->frame);
}

void TkybKnifeCameraCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    work->frame += 7;
    FTBL.EVS_LoopCamera(1503, TKYB_DUMMY, TKYB_DUMMY);
    TkybUpdateKnives(work, work->frame);
    FTBL.HLib_SetCurrentTaskTo(TkybKnifeThrowCallback);
}

void TkybKnifeThrowCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    work->frame += 7;
    TkybUpdateKnives(work, work->frame);
    if (work->frame > 1000) {
        FTBL.SetShowQteFlagForButtonType(TKYB_QTE_BUTTON, 30, 0);
        FTBL.HLib_SetCurrentTaskTo(TkybKnifeQteCallback);
    }
}

void TkybKnifeQteCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    work->frame += 7;
    TkybUpdateKnives(work, work->frame);
    if (!FTBL.GetShouldRenderIcons(3))
        FTBL.HLib_SetCurrentTaskTo(TkybQteResultCallback);
}

const int tkybKnifeHandPart = 152;

void TkybUpdateKnives(TkybWork *work, int frame)
{
    Vector3 offset, position, base;
    Vector3i rotation, angles;
    shen_name knives[5] = {TKYB_KNIFE_D, TKYB_KNIFE_C, TKYB_KNIFE_B, TKYB_KNIFE_A, TKYB_INVALID};
    float alphas[4] = {0.2f, 0.4f, 0.6f, 1.0f};
    int delays[4] = {50, 25, 12, 0};
    int i, elapsed;
    position.x = 43.29f;
    position.y = 102.12f;
    position.z = 241.29f;
    angles.x = 0;
    angles.y = 25486;
    angles.z = 0;
    for (i = 0; knives[i] != TKYB_INVALID; i++) {
        FTBL.EV_AT_SetPos(knives[i], 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(knives[i], 0x78000000, &angles, 0);
        elapsed = frame - delays[i];
        if (elapsed < 0)
            elapsed = 0;
        rotation.x = 0;
        rotation.y = (unsigned short)(360.0f * elapsed / 1800.0f * 8192.0f / 45.0f);
        rotation.z = 0xc000;
        base.x = 0.0f;
        base.y = 1.5f - elapsed * 0.00026499998f;
        base.z = 0.0f;
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -0.15f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &offset);
        FTBL.scenApplyPosOrRotToChildId(knives[i], tkybKnifeHandPart, &offset, &rotation, 0);
        FTBL.SetScenarioCharacterTransparency(knives[i], alphas[i]);
    }
    if (i)
        i--;
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.15f;
    FTBL.ApplyScenarioTranslationAndRotation(&offset, &rotation, &position);
    offset = position;
    FTBL.GetCharaPosition(knives[i], -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(knives[i], &angles, 0x40000000);
    FTBL.ApplyScenarioTranslationAndRotation(&position, &angles, &offset);
    FTBL.EV_AT_SetPos(TKYB_DUMMY, 0x78000000, &offset);
    FTBL.EV_AT_SetAngY_0(TKYB_DUMMY, 0x78000000, &angles, 0);
    work->dummyCounter = (work->dummyCounter + 1) % 5;
}

void TkybQteResultCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    int i;
    if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A0", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "TKYB_A0", 0, 0);
    FTBL.EV_DisableCharacterDisplay(TKYB_KNIFE_D);
    FTBL.EV_DisableCharacterDisplay(TKYB_KNIFE_C);
    FTBL.EV_DisableCharacterDisplay(TKYB_KNIFE_B);
    FTBL.EV_DisableCharacterDisplay(TKYB_KNIFE_A);
    if ((FTBL.GetCurrentQteStatus() & 0xfff) == TKYB_QTE_BUTTON) {
        for (i = 1; tkybCharacters[i] != TKYB_INVALID; i++)
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(tkybCharacters[i]), TKYB_IDLE_MOTION, 0);
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(TKYB_CHO));
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB_A1", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(TKYB_CHO));
        work->result = TKYB_RESULT_QTE_WON;
        FTBL.EV_LoadAudio("0040A000", 2);
        FTBL.HLib_SetCurrentTaskTo(TkybExitCallback);
        QueueSkAuthorStart(work->task, TKYB_QTE_OK_AUTHOR, 0, 0, 0);
    } else {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB_A2", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(TkybFightSetupCallback);
        QueueSkAuthorStart(work->task, TKYB_QTE_NG_AUTHOR, 0, 0, 0);
    }
}

void TkybFightSetupCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    Vector3 position, target;
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A1", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A1", 0, 0);
    } while (0);
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A2", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A2", 0, 0);
    } while (0);
    FTBL.EV_LoadAudio("0040A000", 1);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TKYB_GEN), TKYB_IDLE_MOTION, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TKYB_RIK), TKYB_IDLE_MOTION, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TKYB_CHY), TKYB_IDLE_MOTION, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    tkybMotion = FTBL.ReadMotiFromDirectory(1, TKYB_WK00, "ZAKO.MOT");
    position.x = 40.6915f;
    position.y = 104.4752f;
    position.z = 249.244f;
    target.x = 42.5222f;
    target.y = 103.2522f;
    target.z = 240.9865f;
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetNextPosition(&position);
    FTBL.camCtrlSetTargetPosition(&target);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.vfmDispatchControl(2, 0, "TKYB", 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_RYO), 0, 0, 0, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_CHO), 0, 1, 0, 500, 20);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_VUN), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_VUN), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_VUN), 0, 0x200000, 2048);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_VUN), 0, 0x8000000, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_SBK), 0, 1, 1, 40, 3);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_SBK), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_SBK), 0, 0x200000, 2048);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_SBK), 0, 0x8000000, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_RNT), 0, 1, 1, 40, 4);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RNT), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RNT), 0, 0x200000, 2048);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RNT), 0, 0x8000000, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_IKO), 0, 1, 1, 40, 5);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_IKO), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_IKO), 0, 0x200000, 2048);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_IKO), 0, 0x8000000, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_RIK), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RIK), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RIK), 0, 0x200000, 2048);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_RIK), 0, 0x8000000, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TKYB_GEN), 0, 1, 1, 90, 6);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TKYB_GEN), 0, 0x8000000, 2);
    FTBL.vfCopyPosFromEntity(0, 0, 45.0f, 245.0f, 15.0f);
    FTBL.vfmQueueFightStart(4, 2, 1.0f, 1, -1);
    work->fightStage = 0;
    FTBL.HLib_SetCurrentTaskTo(TkybFightWaitCallback);
}

void TkybFightWaitCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    if (!FTBL.IsFightActive()) {
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.EV_LoadAudio("0040A000", 2);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(TKYB_RYO)) == 3)
            FTBL.HLib_SetCurrentTaskTo(TkybBattleWonCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(TkybBattleLostCallback);
        FTBL.HLib_SetupTaskDelay(30);
    } else {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(TKYB_RYO)) == 2)
            FTBL.check_dead_status(0);
        TkybFightUpdate(work);
    }
}

void TkybFightUpdate(TkybWork *work)
{
    float frame = FTBL.GetMotionFrame(FTBL.scenFindMotionwork(TKYB_CHO), 0);
    switch (work->fightStage) {
    case 0:
        if ((unsigned int)FTBL.vfmDispatchControl(9, 0, (const void *)1, -1) <= 2)
            return;
        FTBL.EV_vfBattleUtil(5, TKYB_CHO, 1);
        break;
    case 1:
        FTBL.SetFightStateFlags();
        FTBL.EV_DisableCharacterDisplay(TKYB_VUN);
        FTBL.EV_DisableCharacterDisplay(TKYB_SBK);
        FTBL.EV_DisableCharacterDisplay(TKYB_RNT);
        FTBL.EV_DisableCharacterDisplay(TKYB_IKO);
        FTBL.EV_DisableCharacterDisplay(TKYB_RIK);
        FTBL.EV_DisableCharacterDisplay(TKYB_GEN);
        FTBL.EV_DisableCharacterDisplay(TKYB_RYO);
        FTBL.vfmDispatchControl(7, TKYB_VUN, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_SBK, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_RNT, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_IKO, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_RIK, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_GEN, (const void *)0x1000000, 1);
        FTBL.vfmDispatchControl(7, TKYB_RYO, (const void *)0x1000000, 1);
        FTBL.EV_vfBattleUtil(4, 1, 1);
        EVT_AT_SetVoice("0040B028", 0);
        EVT_AT_SetPos(TKYB_CHO, TKYB_INVALID, 39.08f, 102.25f, 242.05f, 0);
        EVT_AT_SetAngY(TKYB_CHO, TKYB_INVALID, 280.0f, 0);
        EVT_AT_SetCam(8000, TKYB_CHO, TKYB_CHO, 0, 0, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(TKYB_CHO), 0);
        EVT_AT_SetMot(TKYB_CHO, 3691, 0.0f, 1.0f, 0, 0);
        break;
    case 2:
        if (65.0f > frame)
            return;
        EVT_AT_SetVoice("0040B029", 0);
        break;
    case 3:
        if (180.0f > frame)
            return;
        FTBL.EV_EnableCharacterDisplay(TKYB_VUN);
        FTBL.EV_EnableCharacterDisplay(TKYB_SBK);
        FTBL.EV_EnableCharacterDisplay(TKYB_RNT);
        FTBL.EV_EnableCharacterDisplay(TKYB_IKO);
        FTBL.EV_EnableCharacterDisplay(TKYB_RIK);
        FTBL.EV_EnableCharacterDisplay(TKYB_GEN);
        FTBL.EV_EnableCharacterDisplay(TKYB_RYO);
        FTBL.EV_DisableCharacterDisplay(TKYB_CHO);
        FTBL.camCtrlSetState(10);
        FTBL.EV_vfBattleUtil(4, 1, 0);
        FTBL.SetFightFrame(1);
        break;
    default:
        return;
    }
    work->fightStage++;
}

void TkybBattleWonCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    FTBL.EV_DisableCharacterDisplay(TKYB_VUN);
    FTBL.EV_DisableCharacterDisplay(TKYB_SBK);
    FTBL.EV_DisableCharacterDisplay(TKYB_RNT);
    FTBL.EV_DisableCharacterDisplay(TKYB_IKO);
    FTBL.EV_DisableCharacterDisplay(TKYB_RIK);
    FTBL.EV_DisableCharacterDisplay(TKYB_GEN);
    FTBL.EV_DisableCharacterDisplay(TKYB_CHY);
    FTBL.EV_EnableCharacterDisplay(TKYB_CHO);
    FTBL.EV_SetCharacterAlphaEnabled(TKYB_CHO, 0);
    FTBL.DeleteMotion(tkybMotion);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB_A3", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.flddSetPointFlagOnGrid(TKYB_DEFAULT_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(TKYB_BATTLE_GRID, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work->result = TKYB_RESULT_BATTLE_WON;
    FTBL.HLib_SetCurrentTaskTo(TkybExitCallback);
    QueueSkAuthorStart(work->task, TKYB_BATTLE_OK_AUTHOR, 0, 0, 0);
}

void TkybBattleLostCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    FTBL.EV_EnableCharacterDisplay(TKYB_CHO);
    FTBL.DeleteMotion(tkybMotion);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TKYB_A4", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work->result = TKYB_RESULT_BATTLE_LOST;
    FTBL.HLib_SetCurrentTaskTo(TkybExitCallback);
    QueueSkAuthorStart(work->task, TKYB_BATTLE_NG_AUTHOR, 0, 0, 0);
}

void TkybAbortCallback(void *data)
{
    ((TkybWork *)data)->result = TKYB_RESULT_QTE_WON;
    TkybExitCallback(data);
}

void TkybExitCallback(void *data)
{
    TkybWork *work = (TkybWork *)data;
    EventDateTime time;
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A0", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A0", 0, 0);
    } while (0);
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A1", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A1", 0, 0);
    } while (0);
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A2", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A2", 0, 0);
    } while (0);
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A3", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A3", 0, 0);
    } while (0);
    do {
        if (FTBL.EV_ControlPackageResources(0, 5, "TKYB_A4", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "TKYB_A4", 0, 0);
    } while (0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (time.day == tkybStartDay && time.hour <= 22) {
        time.hour = 23;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    }
    TkybRestoreCharacter(TKYB_PROP_0001, 0);
    TkybRestoreCharacter(TKYB_KNIFE_D, 0);
    TkybRestoreCharacter(TKYB_KNIFE_C, 0);
    TkybRestoreCharacter(TKYB_KNIFE_B, 0);
    TkybRestoreCharacter(TKYB_KNIFE_A, 0);
    TkybRestoreCharacter(TKYB_PROP_A038, 0);
    TkybRestoreCharacter(TKYB_PROP_A129, 0);
    TkybRestoreCharacter(TKYB_PROP_A132, 0);
    TkybRestoreCharacter(TKYB_GEN, 1);
    TkybRestoreCharacter(TKYB_RIK, 1);
    TkybRestoreCharacter(TKYB_IKO, 1);
    TkybRestoreCharacter(TKYB_RNT, 1);
    TkybRestoreCharacter(TKYB_SBK, 1);
    TkybRestoreCharacter(TKYB_VUN, 1);
    TkybRestoreCharacter(TKYB_RYO, 1);
    ClearEventAbortFlags(TKYB_ABORT_SLOT, 4);
    CancelEventAbort(TKYB_ABORT_SLOT);
    FTBL.flddSetPointFlagOnGrid(TKYB_DEFAULT_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(TKYB_BATTLE_GRID, 0);
    FTBL.EV_FreeAuthorPak(0, "0040");
    ET_AT_KillWork(work->characters);
    FTBL.EV_ControlPackageResources(0, 6, "TKYB", 0, 0);
    FTBL.scenFreeEvent("TKYB.EV3");
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (work->result == TKYB_RESULT_BATTLE_LOST) {
        FTBL.EV_scenQueueLoad(1, TKYB_WTA0, 80);
        FTBL.EV_stepSetFlags(52, 1);
    } else {
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_stepSetFlags(52, 0);
    }
}

void TkybPrepareCharacter(shen_name character, int authored)
{
    float reflector[4];
    if (authored) {
        reflector[0] = 1.0f;
        reflector[1] = 1.0f;
        reflector[2] = 1.0f;
        reflector[3] = 1.0f;
        FTBL.SetScenarioReflectorBoardValues(character, reflector);
        FTBL.SetRefBoardParamsForCharaById(character, 1);
    }
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 0);
    if (authored)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void TkybRestoreCharacter(shen_name character, int authored)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    if (authored)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 1);
    FTBL.EV_SetCharaControllerFlags(character, 1, 1);
    if (authored)
        FTBL.SetRefBoardParamsForCharaById(character, 0);
}
#endif
#endif
