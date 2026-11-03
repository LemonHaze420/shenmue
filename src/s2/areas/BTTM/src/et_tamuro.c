#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_bttm.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void TamuroEntry(void *argument)
{
    StartTamuro();
}
#else
const shen_name tamuroCharacters[] = {TAMURO_RYO, TAMURO_VUN, TAMURO_SBK, TAMURO_RKN, TAMURO_INVALID_CHARACTER};

void StartTamuro(void)
{
    HLibTask *task;
    TamuroWork *work;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    task = FTBL.HLib_EnqueueTaskWithParameter(TamuroMain, 4, 11, sizeof(TamuroWork), TASK_EVNT);
    work = (TamuroWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->mode = TAMURO_MODE_INIT;
    work->reserved10 = 0;
    EventDebugLog("gw : %x\n", work);
    EventDebugLog("htask : %x\n", work->task);
    EventDebugLog("End of initialyze\n");
}

void ExitTamuro(TamuroWork *work)
{
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(1);
}

Vector3 tamuroRyoPosition = {252.32f, 128.8f, 131.43f};
Vector3 tamuroVunPosition = {262.0f, 128.8f, 132.37f};
Vector3 tamuroSbkPosition = {258.35f, 128.8f, 134.49f};
Vector3 tamuroRknPosition = {259.53f, 128.8f, 131.26f};
Vector3 tamuroExitPosition = {246.4f, 128.8f, 127.8f};

void TamuroMain(void *data)
{
    TamuroWork *work = (TamuroWork *)data;
    int i;
    switch (work->mode) {
    case TAMURO_MODE_INIT:
        FTBL.SetPlayerControlEnabled(0);
        work->motions[0] = 0;
        if (FTBL.GetNpcNewLoadState() == 1)
            FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        for (i = 0; tamuroCharacters[i] != TAMURO_INVALID_CHARACTER; i++)
            FTBL.CheckWarmStartNpcRoute(tamuroCharacters[i]);
        work->npcIndex = 0;
        EventDebugLog("MODE_INIT2\n");
        work->mode = TAMURO_MODE_INIT2;
        break;
    case TAMURO_MODE_INIT2:
        for (;;) {
            shen_name name = tamuroCharacters[work->npcIndex];
            if (name != TAMURO_INVALID_CHARACTER) {
                if (FTBL.DoesScenarioCharacterExist(name)) {
                    work->npcLoaded = 0;
                    work->npcIndex++;
                } else {
                    work->npcLoaded = 1;
                    work->mode = TAMURO_MODE_LOAD_NPC;
                    return;
                }
            } else {
                work->mode = TAMURO_MODE_LOAD_PACKAGE;
                break;
            }
        }
        break;
    case TAMURO_MODE_LOAD_NPC:
        if (FTBL.ForceLoadNpc(tamuroCharacters[work->npcIndex])) {
            work->npcIndex++;
            work->mode = TAMURO_MODE_INIT2;
        }
        break;
    case TAMURO_MODE_LOAD_PACKAGE:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"BTTM", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 4, "BTTM", 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "BTTM", 0, 0);
        FTBL.EV_SetNewSceneIndex();
        FTBL.EV_LoadCutsceneFromCurrentScene("0286A000");
        FTBL.EV_LoadAudio("0286A000", 0);
        work->mode = TAMURO_MODE_START;
        break;
    case TAMURO_MODE_START:
        FTBL.SetWeatherUpdateEnabledTo(1);
        FTBL.RefreshWeatherForCurrentTime(0);
        FTBL.UpdateWeather();
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetLoadingBusy();
        work->thr = ThrCreate(TamuroSequenceThread, TAMURO_SEQUENCE_THREAD, work);
        work->threadActive = 1;
        EventDebugLog("MODE_MAIN\n");
        work->mode = TAMURO_MODE_MAIN;
        break;
    case TAMURO_MODE_MAIN:
        if (work->threadActive == 0) {
            EventDebugLog("MODE_MAIN2\n");
            work->mode = TAMURO_MODE_EXIT;
        }
        break;
    case TAMURO_MODE_EXIT:
        if (work->battleStatus != TAMURO_BATTLE_WON)
            FTBL.EV_AT_SetPos(TAMURO_RYO, 0x78000000, &tamuroExitPosition);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TAMURO_RYO), 0xf002, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(TAMURO_RYO), 71, 0, 0);
        EventDebugLog("MODE_EXIT\n");
        work->mode = TAMURO_MODE_EXIT2;
        break;
    case TAMURO_MODE_EXIT2:
        for (i = 0; tamuroCharacters[i] != TAMURO_INVALID_CHARACTER; i++) {
            if (work->npcLoaded)
                FTBL.npcFree(tamuroCharacters[i]);
            if (FTBL.IsNpcRouteLoadedOrVisible(tamuroCharacters[i]))
                FTBL.npcLockUnlock(tamuroCharacters[i]);
        }
        for (i = 0; work->motions[i]; i++)
            FTBL.DeleteMotion(work->motions[i]);
        FTBL.FinishLoading();
        EventDebugLog("MODE_EXIT2\n");
        work->mode = TAMURO_MODE_END;
        break;
    case TAMURO_MODE_END:
        ExitTamuro(work);
        break;
    default:
        EventDebugLog("mode error ! : %d\n", work->mode);
        FTBL.EventAssertNullsub(__FILE__, 321);
        break;
    }
}

void TamuroSequenceThread(void *data)
{
    Thr *thr = (Thr *)data;
    TamuroWork *work = (TamuroWork *)ThrGetWork(thr);
    unsigned int positionFlags = 0x78000000;
    ThrSetDebugId(ThrGetId(thr));
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            FTBL.EV_AT_SetPos(TAMURO_RYO, positionFlags, &tamuroRyoPosition);
            FTBL.EV_AT_SetPos(TAMURO_VUN, positionFlags, &tamuroVunPosition);
            FTBL.EV_AT_SetPos(TAMURO_SBK, positionFlags, &tamuroSbkPosition);
            FTBL.EV_AT_SetPos(TAMURO_RKN, positionFlags, &tamuroRknPosition);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 1, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_VUN, 1, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_SBK, 1, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RKN, 1, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 2, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 3, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_VUN, 2, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_VUN, 3, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_SBK, 2, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_SBK, 3, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RKN, 2, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RKN, 3, 0);
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            ThrWaitFrames(thr, 101, 1, 10);
            break;
        case 101:
            ThrMakeChild(thr, TamuroBattleThread, TAMURO_BATTLE_THREAD, work, 0);
            ThrWaitChildExit(thr, 102, 1, 0);
            break;
        case 102:
            ThrChangeSeq(thr, 108, 1);
            break;
        case 108:
            work->threadActive = 0;
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}

void TamuroBattleThread(void *data)
{
    Thr *thr = (Thr *)data;
    TamuroWork *work = (TamuroWork *)ThrGetWork(thr);
    ThrSetDebugId(ThrGetId(thr));
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.EV_EnableCharacterDisplay(TAMURO_RYO);
            FTBL.EV_EnableCharacterDisplay(TAMURO_VUN);
            FTBL.EV_EnableCharacterDisplay(TAMURO_SBK);
            FTBL.EV_EnableCharacterDisplay(TAMURO_RKN);
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            FTBL.shenStartFGST(10, 10, 0, 0);
            FTBL.vfmDispatchControl(2, 0, "BTTM", 1);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TAMURO_RYO), 0, 0, 0, 0, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TAMURO_VUN), 0, 1, 1, 100, 2);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TAMURO_SBK), 0, 1, 1, 100, 3);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TAMURO_RKN), 0, 1, 1, 100, 4);
            FTBL.vfmQueueFightStart(4, 99, 1.0f, 0, -1);
            ThrChangeSeq(thr, 101, 1);
            break;
        case 101:
            if (!FTBL.IsFightActive())
                ThrChangeSeq(thr, 12, 1);
            break;
        case 12:
            if (!FTBL.IsFightActive()) {
                work->battleStatus = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(TAMURO_RYO));
                if (work->battleStatus == TAMURO_BATTLE_WON) {
                    FTBL.EV_stepSetFlags(TAMURO_WIN_STEP_FLAG, 0);
                    FTBL.dbgPrintEventMessage("鍵ゲット！\n");
                }
                ThrChangeSeq(thr, 104, 1);
            }
            break;
        case 104:
            ThrWaitFrames(thr, 105, 1, 30);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        case 105:
            if (work->battleStatus == TAMURO_BATTLE_WON)
                FTBL.ctrlStopSubtitles();
            FTBL.figpAssignModeFor(TAMURO_RYO, 0);
            FTBL.figpAssignModeFor(TAMURO_VUN, 0);
            FTBL.figpAssignModeFor(TAMURO_SBK, 0);
            FTBL.figpAssignModeFor(TAMURO_RKN, 0);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 1, 1);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 2, 1);
            FTBL.EV_SetCharaControllerFlags(TAMURO_RYO, 3, 1);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
#endif
#endif
