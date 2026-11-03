#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_knmn.hpp"
#if defined(SHENMUE_EVENT_ENTRY)
void RunKnmnEvent(void) {
    HLibTask *task;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    task = FTBL.HLib_EnqueueTaskWithoutParameter(KnmnMakeGameCallback, 4, 11, TASK_EVNT);
    FTBL.HLib_SetupTimedTask(task, 30);
    FTBL.flddSetPointFlagOnGrid(SHEN_NAME4('B', 'T', 'C', '1'), 1);
    FTBL.flddSetPointFlagOnGrid(SHEN_NAME4('0', '0', '0', '0'), 0);
}
#else
const shen_name knmnCharacters[] = {KNMN_RYO, KNMN_KEM, KNMN_INVALID};
const char knmnMasterMotion[] = "MASTER.MOT";
const char knmnAuthorPackage[] = "0026";
const char knmnPackage[] = "KNMN";
const char knmnAuthPackage[] = "KNMNAUTH";
const char knmnAuthor[] = "0026A000";
const char knmnFilename[] = "KNMN.EV3";
const char knmnReturnOverlay[] = "WKKT.EV2";
const char knmnBlurModel[] = "KEM_B.MT7";
const char knmnAudio[] = "0026A000.sbt";
const char knmnWazaTitle[] = "小擒打";
const char knmnLeafDirectory[] = "misc";
const char knmnLeafTexture[] = "REDLEAF.pvr";
int knmnLeafEffect = -1;

void KnmnOwnerNullsub(void *data) {
}
void KnmnMakeGameCallback(void *data) {
    short entry = g_entryPoint;
    HLibTask *owner, *child;
    unsigned char mode;
    KnmnOwnerWork *retained;
    KnmnGameWork *work;
    owner = FTBL.HLib_EnqueueTaskWithParameter(KnmnOwnerNullsub, 3, 11, sizeof(KnmnOwnerWork), TASK_EVNT);
    if (g_currentMap == KNMN_AREA) {
        switch (entry) {
        case 96:
            if (g_currentMap == KNMN_AREA)
                EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 5);
            child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAuthWazaCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
            mode = 2;
            break;
        case 97:
            child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAutoCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
            mode = 2;
            break;
        case 98:
            child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAuthFirstCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
            mode = 0;
            break;
        default:
            child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAutoCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
            mode = 3;
            break;
        }
    } else if (FTBL.EV_GetShenmueFlag(0xa1)) {
        child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAutoCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
        mode = 2;
    } else if (FTBL.EV_GetShenmueFlag(0x95)) {
        child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAuthFirstCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
        mode = 0;
    } else if (FTBL.EV_GetShenmueFlag(0x19)) {
        child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAuthFirstCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
        mode = 1;
    } else {
        child = FTBL.HLib_EnqueueTaskWithParameter(KnmnAutoCallback, 5, 11, sizeof(KnmnGameWork), TASK_EVNT);
        mode = 3;
    }
    retained = (KnmnOwnerWork *)FTBL.HLib_GetTaskParameter(owner);
    work = (KnmnGameWork *)FTBL.HLib_GetTaskParameter(child);
    retained->task = owner;
    retained->child = child;
    work->owner = owner;
    work->task = child;
    work->state = 0;
    work->mode = mode;
    work->round = 0;
    knmnLeafEffect = -1;
    work->characters = Queue_EVT_AT_ReadChr(child, knmnCharacters);
    if (g_currentMap == KNMN_AREA)
        EVT_AT_MenCtrl(0, 255, 0, 0, 0, 0, 0, 0, 0, -1);
    KnmnLoadPackages(work);
    EventDebugRegister(3, KnmnDebugEndCallback, work, 18000);
    FTBL.HLib_DestroyCurrentTask();
}
void KnmnLoadPackages(KnmnGameWork *work) {
    FTBL.SetScenarioFollowCharacter(0);
    FTBL.EV_SetNewSceneIndex();
    work->masterMotion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, knmnMasterMotion);
    FTBL.EV_FreeAuthorPak(1, knmnAuthorPackage);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)knmnPackage, 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    if (work->mode != 3) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)knmnAuthPackage, 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
    FTBL.EV_ControlPackageResources(2, 4, (void *)knmnPackage, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, (void *)knmnPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene(knmnAuthor);
}
void KnmnStopCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    if (g_currentMap == KNMN_AREA) {
        if (work->mode == 0 || work->mode == 2)
            FTBL.HLib_SetCurrentTaskTo(KnmnAuthFirstCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(KnmnAutoCallback);
    } else {
        FTBL.spmFadeOut(0);
        FTBL.HLib_SetCurrentTaskTo(KnmnEndCallback);
        if (FTBL.EV_GetScreenFilmIsNotShowing()) {
            if (work->mode != 2)
                FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_SetupTaskDelay(60);
        }
    }
}
void KnmnDebugEndCallback(void *data) {
    if (knmnLeafEffect != -1)
        FTBL.FreeParticleEffect(3, knmnLeafEffect);
    KnmnEndCallback(data);
}
void KnmnEndCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    HLibTask *task;
    if (knmnLeafEffect != -1) {
        FTBL.FreeParticleEffect(3, knmnLeafEffect);
        knmnLeafEffect = -1;
    }
    switch (work->mode) {
    case 3:
        FTBL.EV_stepSetFlags(0x42, 0);
        break;
    case 0:
        FTBL.EV_stepSetFlags(0x95, 0);
        break;
    case 1:
        FTBL.EV_stepSetFlags(0x19, 0);
        break;
    case 2:
        FTBL.EV_stepSetFlags(0xa1, 0);
        break;
    }
    FTBL.flddSetPointFlagOnGrid(SHEN_NAME4('0', '0', '0', '0'), 1);
    FTBL.flddSetPointFlagOnGrid(SHEN_NAME4('B', 'T', 'C', '1'), 0);
    FTBL.InitiateCleanupForSceneChara(KNMN_KEM);
    ET_AT_KillWork(work->characters);
    if (FTBL.EV_ControlPackageResources(0, 5, (void *)knmnPackage, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, (void *)knmnPackage, 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, (void *)knmnAuthPackage, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, (void *)knmnAuthPackage, 0, 0);
    FTBL.EV_FreeAuthorPak(0, knmnAuthorPackage);
    FTBL.DeleteMotion(work->masterMotion);
    FTBL.EV_HLKillOtherHierarchyTask(work->owner);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetPlayerControlEnabled(0);
    EventDebugUnregister(3);
    FTBL.scenFreeEvent(knmnFilename);
    if (work->mode == 2) {
        FTBL.LoadAreaFunc_Wrapper_0(knmnReturnOverlay, 0);
        FTBL.ExecuteCurrAreaMainFunc(knmnReturnOverlay, (void *)1);
    } else {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_SetMot(KNMN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        if (work->mode == 0) {
            EVT_AT_SetPos(KNMN_RYO, KNMN_INVALID, 46.9f, 102.3f, 247.7f, 0);
            EVT_AT_SetAngY(KNMN_RYO, KNMN_INVALID, 62, 0);
        } else {
            EVT_AT_SetPos(KNMN_RYO, KNMN_INVALID, 43.1f, 102.3f, 243.1f, 0);
            EVT_AT_SetAngY(KNMN_RYO, KNMN_INVALID, 90, 0);
        }
        FTBL.EV_NPC_NewLoadSw_0(0);
        task = FTBL.HLib_EnqueueTaskWithoutParameter(KnmnWaitNpcCallback, 4, 11, SHEN_NAME4('w', 'n', 'p', 'c'));
        FTBL.HLib_SetupTimedTask(task, 30);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.CheckWarmStartNpcRoute(KNMN_KEM);
        FTBL.ForceLoadNpc(KNMN_KEM);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
}
void KnmnWaitNpcCallback(void *data) {
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.npcLockUnlock(KNMN_KEM);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_DestroyCurrentTask();
}
void KnmnInitBattleCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    work->state = 0;
    EVT_AT_SetPos(KNMN_RYO, KNMN_INVALID, 47.2f, 102.5f, 242.4f, 0);
    EVT_AT_SetPos(KNMN_KEM, KNMN_INVALID, 42.4f, 102.5f, 242.5f, 0);
    EVT_AT_SetAngY(KNMN_RYO, KNMN_INVALID, 90, 0);
    EVT_AT_SetAngY(KNMN_KEM, KNMN_INVALID, 270, 0);
    EVT_AT_SetCam(1007, KNMN_INVALID, KNMN_INVALID, 0, 0, 0);
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(KNMN_RYO), 0, 0, 0, 0, 0);
    FTBL.vfCopyPosFromEntity(0, 0, 43.2f, 249.3f, 5);
    work->battleFrames = 0;
    FTBL.LoadBlurModelForCharacter_0(KNMN_KEM, knmnBlurModel, 1);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(KNMN_KEM));
    if ((unsigned char)work->mode <= 1) {
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(KNMN_KEM), 0, 1, 1, 200, 9);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(KNMN_KEM), 0, 0x08000000U, 0);
        FTBL.vfmQueueFightStart(2, 99, 1, 0, -1);
        KnmnInstallBattleDialogues(810);
        KnmnInstallFirstEndConditions();
        FTBL.HLib_SetCurrentTaskTo(KnmnWaitBattleFirstCallback);
    } else {
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(KNMN_KEM), 0, 1, 1, 200, 9);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(KNMN_KEM), 0, 0x08000000U, 0);
        FTBL.vfmQueueFightStart(2, 99, 1, 0, 60);
        KnmnInstallBattleDialogues(810);
        FTBL.HLib_SetCurrentTaskTo(KnmnWaitBattleRegularCallback);
    }
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void KnmnWaitBattleRegularCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    int player, opponent;
    switch (work->state) {
    case 0:
        player = FTBL.btlGetCharaFlag(1, FTBL.ctrlGetCharaByName(KNMN_RYO), 0);
        opponent = FTBL.btlGetCharaFlag(1, FTBL.ctrlGetCharaByName(KNMN_KEM), 0);
        if (!FTBL.IsFightActive() || player <= 30 || opponent <= 30) {
            if (FTBL.IsFightActive())
                FTBL.check_dead_status(0);
            work->state++;
        }
        break;
    case 1:
        if (!FTBL.IsFightActive()) {
            work->state = 0;
            FTBL.HLib_SetCurrentTaskTo(KnmnStopCallback);
        }
        break;
    }
    if (work->battleFrames == 5400)
        FTBL.check_dead_status(1);
    else
        work->battleFrames++;
}
void KnmnWaitBattleFirstCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    if (!FTBL.IsFightActive()) {
        if (FTBL.IsFightActive())
            FTBL.check_dead_status(0);
        if (work->mode == 0) {
            FTBL.HLib_SetCurrentTaskTo(KnmnStopCallback);
            QueueSkAuthorStart(work->task, SHEN_NAME4('E', 'N', 'D', '_'), 0, 0, 0);
        } else {
            FTBL.HLib_SetCurrentTaskTo(KnmnAuthFirstEndCallback);
        }
        FTBL.SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 0;
    }
    if (KnmnTickFirstEndTimer() || work->battleFrames == 5400) {
        FTBL.check_dead_status(1);
    } else {
        work->battleFrames++;
        if (work->battleFrames == 5385 && !FTBL.filmCheckFadeTrackColour())
            FTBL.SetScreenFilmFade(10, 0, 0, 0, 0, 255, 0, 0, 0);
    }
}
void KnmnInitButokuCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    if (work->round == 0) {
        EVT_AT_SetCam(1007, KNMN_INVALID, KNMN_INVALID, 0, 0, 0);
        EVT_AT_SetPos(KNMN_RYO, KNMN_INVALID, 47.2f, 102.3f, 242.4f, 0);
        EVT_AT_SetPos(KNMN_KEM, KNMN_INVALID, 42.4f, 102.3f, 242.5f, 0);
        EVT_AT_SetAngY(KNMN_RYO, KNMN_INVALID, 90, 0);
        EVT_AT_SetAngY(KNMN_KEM, KNMN_INVALID, 270, 0);
    } else {
        EVT_AT_SetAngY(KNMN_RYO, KNMN_RYO, 180, 0);
    }
    FTBL.LoadBlurModelForCharacter_0(KNMN_KEM, knmnBlurModel, 1);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(KNMN_KEM));
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(KNMN_RYO), 0, 0, 0, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(KNMN_KEM), 0, 1, 1, 200, 9);
    work->battleFrames = 0;
    FTBL.vfCopyPosFromEntity(0, 0, 43.2f, 249.3f, 5);
    FTBL.vfmQueueFightStart(2, 99, 1, 0, -1);
    KnmnInstallBattleDialogues(810);
    if (work->round == 0)
        KnmnInstallButokuEndConditions();
    else
        KnmnRequestButokuBattleEnd();
    work->state = 0;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(KnmnWaitButokuCallback);
}
void KnmnWaitButokuCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    switch (work->state) {
    case 0:
        if (!FTBL.IsFightActive()) {
            if (FTBL.IsFightActive())
                FTBL.check_dead_status(0);
            work->state++;
        }
        break;
    case 1:
        if (!FTBL.IsFightActive()) {
            work->state = 0;
            if (work->round == 0)
                FTBL.HLib_SetCurrentTaskTo(KnmnAuthTomoCallback);
            else
                FTBL.HLib_SetCurrentTaskTo(KnmnAuthShoukindaCallback);
            FTBL.SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
        }
        break;
    }
    if (KnmnTickButokuEndTimer() || work->battleFrames == 5400) {
        FTBL.check_dead_status(1);
    } else {
        work->battleFrames++;
        if (work->battleFrames == 5385 && !FTBL.filmCheckFadeTrackColour())
            FTBL.SetScreenFilmFade(10, 0, 0, 0, 0, 255, 0, 0, 0);
        work->battleFrames++;
    }
}
void KnmnAutoCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    EventDebugCheck(3, 4);
    FTBL.EV_LoadAudio(knmnAudio, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    EVT_AT_SetPos(KNMN_RYO, KNMN_INVALID, 45.4f, 102.2f, 244.1f, 0);
    EVT_AT_SetPos(KNMN_KEM, KNMN_INVALID, 45.4f, 102.2f, 245.1f, 0);
    EVT_AT_SetAngY(KNMN_RYO, KNMN_INVALID, 180, 0);
    EVT_AT_SetAngY(KNMN_KEM, KNMN_INVALID, 0, 0);
    EVT_AT_SetMot(KNMN_RYO, 5194, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(KNMN_KEM, 5193, 0, 1, 0x40000000U, 0);
    work->state = 0;
    if (work->mode != 2)
        FTBL.HLib_SetCurrentTaskTo(KnmnInitBattleCallback);
    else
        FTBL.HLib_SetCurrentTaskTo(KnmnInitButokuCallback);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 30);
    FTBL.HLib_SetupTaskDelay(30);
}
void KnmnButokuCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    switch (work->state) {
    case 0:
        if (FTBL.filmCheckFadeTrackColour())
            return;
        work->practiceTask = KnmnQueuePractice(work);
        knmnLeafEffect = -1;
        work->state++;
        break;
    case 1:
        if (!KnmnFinishPractice(work->practiceTask))
            return;
        if (knmnLeafEffect != -1) {
            FTBL.FreeParticleEffect(3, knmnLeafEffect);
            knmnLeafEffect = -1;
        }
        work->state++;
        break;
    default:
        work->state = 0;
        FTBL.HLib_SetCurrentTaskTo(KnmnStopCallback);
        break;
    }
}
void KnmnAuthFirstCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    if (work->state == 0) {
        FTBL.SetScreenFilmFade(60, 255, 0, 0, 0, 0, 0, 0, 0);
        EventDebugCheck(3, 4);
        FTBL.EV_LoadAudio(knmnAudio, 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_authSceneCtrl(SHEN_NAME4('F', 'I', 'R', 'S'));
        work->state++;
    } else if (!FTBL.EV_authSceneCtrl(-1)) {
        work->state = 0;
        if (work->mode != 2)
            FTBL.HLib_SetCurrentTaskTo(KnmnInitBattleCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(KnmnInitButokuCallback);
    }
}
void KnmnAuthFirstEndCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    switch (work->state) {
    case 0:
        QueueSkAuthorStart(work->task, SHEN_NAME4('B', 'T', 'F', 'I'), 0, 0, 0);
        work->state++;
        break;
    case 1:
        work->state = 0;
        work->mode = 2;
        FTBL.EV_stepSetFlags(0x19, 0);
        FTBL.HLib_SetCurrentTaskTo(KnmnButokuCallback);
        QueueSkAuthorStart(work->task, SHEN_NAME4('S', 'H', 'O', '2'), 0, 0, 0);
        break;
    }
}
void KnmnAuthTomoCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    Vector3 target;
    switch (work->state) {
    case 0:
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 3);
        FTBL.GetCharaPosition(KNMN_KEM, 5, &target, 0x40000000);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), &target, 0);
        FTBL.EV_authSceneCtrl(SHEN_NAME4('T', 'O', 'M', 'O'));
        work->state++;
        break;
    case 1:
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 3);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 0);
        work->state++;
    default:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            work->state = 0;
            work->round++;
            FTBL.EV_LoadAudio(knmnAudio, 0);
            FTBL.EV_spmBeginSampleExecute(0);
            FTBL.HLib_SetCurrentTaskTo(KnmnInitButokuCallback);
        }
        break;
    }
}
void KnmnAuthShoukindaCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    Vector3 target;
    switch (work->state) {
    case 0:
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 3);
        FTBL.GetCharaPosition(KNMN_KEM, 5, &target, 0x40000000);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), &target, 0);
        FTBL.EV_authSceneCtrl(SHEN_NAME4('S', 'H', 'O', 'U'));
        work->state++;
        break;
    case 1:
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 3);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO), 0, 0);
        work->state++;
    case 2:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state++;
        break;
    case 3:
        QueueSkAuthorStart(work->task, SHEN_NAME4('S', 'H', 'O', '2'), 0, 0, 0);
        work->state++;
        break;
    default:
        work->state = 0;
        FTBL.HLib_SetCurrentTaskTo(KnmnButokuCallback);
        break;
    }
}
void KnmnAuthWazaCallback(void *data) {
    KnmnGameWork *work = (KnmnGameWork *)data;
    if (work->state == 0) {
        work->state++;
        knmnLeafEffect = -1;
    } else if (!FTBL.EV_authSceneCtrl(-1)) {
        work->state = 0;
        if (knmnLeafEffect != -1) {
            FTBL.FreeParticleEffect(3, knmnLeafEffect);
            knmnLeafEffect = -1;
        }
        FTBL.HLib_SetCurrentTaskTo(KnmnStopCallback);
    } else
        KnmnWazaFrameEvents(work);
}

void KnmnWazaFrameEvents(void *data) {
    Vector3 size = {.022f, .05f, .022f};
    switch (FTBL.EV_authSceneCtrl(-6)) {
    case 50:
        KnmnCreateLeafEmitter(&knmnLeafEffect, 80, .5f);
        FTBL.ResumeParticleEffect(3, knmnLeafEffect);
        size.y = .04f;
        return;
    case 68:
        FTBL.ProcessAicaCommand(0x1200a0, 0);
        return;
    case 70:
        FTBL.QueueExtraTelop(knmnWazaTitle, 0);
        return;
    case 71:
        FTBL.EV_LoadAudio(knmnAuthor, 0);
        return;
    case 200:
        FTBL.SetParticleEffectBirthRate(3, knmnLeafEffect, 0);
        return;
    case 420:
        FTBL.SuspendParticleEffect(3, knmnLeafEffect);
        return;
    }
}
void KnmnCreateLeafEmitter(int *effect, int count, float birthRate) {
    Vector3 position = {39.31f, 105.8f, 240.9f};
    Vector3 volume = {1.5f, .02f, 2};
    Vector3 velocity = {0, -.04f, 0};
    Vector3 variance = {.012f, -.01f, .012f};
    *effect = FTBL.CreateFallingParticleEffect(&position, &volume, count, birthRate, 0x400, knmnLeafDirectory, knmnLeafTexture, -1);
    FTBL.SetParticleEffectVelocity(3, *effect, &velocity, &variance);
    FTBL.SetParticleEffectSize(3, *effect, .02f, .06f);
}

#endif

#else
#include "s2/areas/area_knmn.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/ui.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2::areas {
std::int32_t knmnLeafEffect = -1, knmnEndRequested{}, knmnEndTimer{};
std::uint32_t knmnConditionalDialogues[3]{};
CompiledSequenceWork *knmnQteDialogue{};
namespace {
struct KnmnOwnerWork {
    HLib::Task *task, *child;
};
void KnmnOwnerNullsub(void *) {
}
void KnmnFade(std::int32_t frames, bool reveal) {
    SetScreenFilmFade(frames, reveal ? 255 : 0, 0, 0, 0, reveal ? 0 : 255, 0, 0, 0);
}
void KnmnQueueFade(std::int32_t frames, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, 0x2c, KNMN_TASK);
    RegisterEventTask(task);
    EventFilmControl &film = *static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
    film.cleanup = 1;
    film.frames = frames;
    film.startAlpha = 255;
    film.startRed = 0;
    film.startGreen = 0;
    film.startBlue = 0;
    film.targetAlpha = 0;
    film.targetRed = 0;
    film.targetGreen = 0;
    film.targetBlue = 0;
    film.delay = delay;
}
void KnmnQueueStepAuthor(HLib::Task *parent, shen_name author) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, 0x28, KNMN_TASK);
    StepAuthorStartWork &work = *static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.parent = parent;
    work.command = author.name;
    work.callback = nullptr;
    work.callbackData = nullptr;
    work.phase = 1;
    work.reserved16 = 0;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    HLib::SetupTimedTask(parent, 0);
}
void KnmnPosition(shen_name character, const Vector3 &position) {
    EventSetPosition work{.cleanup = 0, .character = character, .parent = KNMN_INVALID, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&work);
}
void KnmnAngle(shen_name character, float degrees, shen_name parent = KNMN_INVALID) {
    EventSetAngle work{.cleanup = 0, .character = character, .parent = parent, .degrees = degrees, .delay = 0};
    EVT_AT_SetAngY_Callback(&work);
}
void KnmnMotion(shen_name character, std::uint32_t motion) {
    EventSetMotion work{.cleanup = 0, .character = character, .animation = motion, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&work);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion));
}
void KnmnBattleCamera() {
    EventSetCamera work{.cleanup = 0, .camera = 1007, .eye = KNMN_INVALID, .target = KNMN_INVALID, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&work);
    camCtrlDisableFilmBars();
}
void KnmnLoadPackages(KnmnGameWork &work) {
    SetScenarioFollowCharacter(shen_name{});
    EV_SetNewSceneIndex();
    work.masterMotion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "MASTER.MOT"));
    EV_FreeAuthorPak(1, "0026");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("KNMN"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    if (work.mode != 3) {
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("KNMNAUTH"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KNMN"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KNMN"), 0);
    EV_LoadCutsceneFromCurrentScene("0026A000");
}
void KnmnMakeGameCallback(void *) {
    const std::int16_t entry = static_cast<std::int16_t>(g_entryPoint);
    HLib::Task *owner = HLib::EnqueueTaskWithParameter(KnmnOwnerNullsub, 3, 0x0b, 0x10, KNMN_TASK);
    HLib::TaskCallback callback = KnmnAutoCallback;
    std::uint8_t mode = 3;
    if (g_currentMap.name == KNMN_AREA.name) {
        if (entry == 96) {
            KnmnQueueFade(30, 5);
            callback = KnmnAuthWazaCallback;
            mode = 2;
        } else if (entry == 97)
            mode = 2;
        else if (entry == 98) {
            callback = KnmnAuthFirstCallback;
            mode = 0;
        }
    } else if (EV_GetShenmueFlag(0xa1))
        mode = 2;
    else if (EV_GetShenmueFlag(0x95)) {
        callback = KnmnAuthFirstCallback;
        mode = 0;
    } else if (EV_GetShenmueFlag(0x19)) {
        callback = KnmnAuthFirstCallback;
        mode = 1;
    }
    HLib::Task *child = HLib::EnqueueTaskWithParameter(callback, 5, 0x0b, 0x38, KNMN_TASK);
    KnmnOwnerWork &retained = *static_cast<KnmnOwnerWork *>(HLib::GetTaskParameter(owner));
    retained.task = owner;
    retained.child = child;
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(HLib::GetTaskParameter(child));
    work.owner = owner;
    work.task = child;
    work.state = 0;
    work.mode = mode;
    work.round = 0;
    knmnLeafEffect = -1;
    work.characters = Queue_EVT_AT_ReadChr(child, KNMN_CHARACTERS);
    if (g_currentMap.name == KNMN_AREA.name)
        KnmnQueueFade(0, -1);
    KnmnLoadPackages(work);
    HLib::DestroyCurrentTask();
}
void KnmnWaitNpcCallback(void *) {
    EV_NPC_NewLoadSw(1);
    EV_npcLockUnlock(KNMN_KEM);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
bool KnmnTickEndTimer() {
    if (!knmnEndRequested)
        return false;
    if (!knmnEndTimer || !--knmnEndTimer) {
        check_dead_status(1);
        return true;
    }
    if (knmnEndTimer == 10 && !filmCheckFadeTrackColour())
        KnmnFade(10, false);
    return false;
}
void KnmnPrepareBattle(KnmnGameWork &work) {
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(KNMN_RYO), nullptr, 0, 0, 0, 0);
    vfCopyPosFromEntity(0, nullptr, 43.20000076293945f, 249.3000030517578f, 5);
    work.battleFrames = 0;
    LoadBlurModelForCharacter(KNMN_KEM, "KEM_B.MT7", 1);
    SetLowQualityHands(ctrlGetScenarioChara(KNMN_KEM));
}
} // namespace
void InitializeKnmnEvent() {
    knmnLeafEffect = -1;
    knmnConditionalDialogues[0] = 0;
    knmnConditionalDialogues[1] = 0;
    knmnConditionalDialogues[2] = 0;
    knmnEndRequested = 0;
    knmnEndTimer = 0;
    knmnQteDialogue = nullptr;
}
void RunKnmnEvent(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    KnmnFade(1, false);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(KnmnMakeGameCallback, 4, 0x0b, static_cast<std::int32_t>(KNMN_TASK.name));
    HLib::SetupTimedTask(task, 30);
    flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 1);
    flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
}
void KnmnAuthFirstCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        KnmnFade(60, true);
        EV_LoadAudio("0026A000.sbt", 0);
        EV_spmBeginSampleExecute();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(FIRS)));
        ++work.state;
    } else if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        work.state = 0;
        HLib::SetCurrentTaskTo(work.mode == 2 ? KnmnInitButokuCallback : KnmnInitBattleCallback);
    }
}
void KnmnAutoCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    EV_LoadAudio("0026A000.sbt", 0);
    EV_spmBeginSampleExecute();
    KnmnPosition(KNMN_RYO, Vector3{.x = 45.4000015f, .y = 102.199997f, .z = 244.100006f});
    KnmnPosition(KNMN_KEM, Vector3{.x = 45.4000015f, .y = 102.199997f, .z = 245.100006f});
    KnmnAngle(KNMN_RYO, 180);
    KnmnAngle(KNMN_KEM, 0);
    KnmnMotion(KNMN_RYO, 5194);
    KnmnMotion(KNMN_KEM, 5193);
    work.state = 0;
    HLib::SetCurrentTaskTo(work.mode == 2 ? KnmnInitButokuCallback : KnmnInitBattleCallback);
    KnmnQueueFade(30, 30);
    HLib::SetupTaskDelay(30);
}
void KnmnInitBattleCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    work.state = 0;
    KnmnPosition(KNMN_RYO, Vector3{.x = 47.2000008f, .y = 102.5f, .z = 242.399994f});
    KnmnPosition(KNMN_KEM, Vector3{.x = 42.4000015f, .y = 102.5f, .z = 242.5f});
    KnmnAngle(KNMN_RYO, 90);
    KnmnAngle(KNMN_KEM, 270);
    KnmnBattleCamera();
    KnmnPrepareBattle(work);
    shenInitFightData(ctrlGetScenarioChara(KNMN_KEM), nullptr, 1, 1, 200, 9);
    vfUpdateFlagsFor(ctrlGetScenarioChara(KNMN_KEM), 0, 0x08000000U, 0);
    vfmQueueFightStart(2, 99, 1, 0, work.mode <= 1 ? -1 : 60);
    KnmnInstallBattleDialogues();
    if (work.mode <= 1) {
        KnmnInstallFirstEndConditions();
        HLib::SetCurrentTaskTo(KnmnWaitBattleFirstCallback);
    } else
        HLib::SetCurrentTaskTo(KnmnWaitBattleRegularCallback);
    KnmnFade(30, true);
}
void KnmnInitButokuCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (work.round)
        KnmnAngle(KNMN_RYO, 180, KNMN_RYO);
    else {
        KnmnBattleCamera();
        KnmnPosition(KNMN_RYO, Vector3{.x = 47.2000008f, .y = 102.300003f, .z = 242.399994f});
        KnmnPosition(KNMN_KEM, Vector3{.x = 42.4000015f, .y = 102.300003f, .z = 242.5f});
        KnmnAngle(KNMN_RYO, 90);
        KnmnAngle(KNMN_KEM, 270);
    }
    LoadBlurModelForCharacter(KNMN_KEM, "KEM_B.MT7", 1);
    SetLowQualityHands(ctrlGetScenarioChara(KNMN_KEM));
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(KNMN_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(KNMN_KEM), nullptr, 1, 1, 200, 9);
    work.battleFrames = 0;
    vfCopyPosFromEntity(0, nullptr, 43.20000076293945f, 249.3000030517578f, 5);
    vfmQueueFightStart(2, 99, 1, 0, -1);
    knmnEndRequested = 0;
    KnmnInstallBattleDialogues();
    if (work.round) {
        knmnEndRequested = 1;
        knmnEndTimer = 300;
    } else
        KnmnInstallButokuEndConditions();
    work.state = 0;
    KnmnFade(30, true);
    HLib::SetCurrentTaskTo(KnmnWaitButokuCallback);
}
void KnmnStopCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (g_currentMap.name == KNMN_AREA.name)
        HLib::SetCurrentTaskTo((work.mode & 0xfd) ? KnmnAutoCallback : KnmnAuthFirstCallback);
    else {
        spmFadeOut();
        HLib::SetCurrentTaskTo(KnmnEndCallback);
        if (EV_GetScreenFilmIsNotShowing()) {
            if (work.mode != 2)
                KnmnFade(60, false);
            HLib::SetupTaskDelay(60);
        }
    }
}
void KnmnWaitBattleFirstCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!IsFightActive()) {
        if (IsFightActive())
            check_dead_status(0);
        if (work.mode)
            HLib::SetCurrentTaskTo(KnmnAuthFirstEndCallback);
        else {
            HLib::SetCurrentTaskTo(KnmnStopCallback);
            KnmnQueueStepAuthor(work.task, SHEN_NAME(END_));
        }
        KnmnFade(10, true);
        work.state = 0;
    }
    if (KnmnTickEndTimer())
        return;
    if (work.battleFrames == 5400) {
        check_dead_status(1);
        return;
    }
    if (++work.battleFrames == 5385 && !filmCheckFadeTrackColour())
        KnmnFade(10, false);
}
void KnmnWaitButokuCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        if (!IsFightActive()) {
            if (IsFightActive())
                check_dead_status(0);
            ++work.state;
        }
    } else if (work.state == 1 && !IsFightActive()) {
        work.state = 0;
        HLib::SetCurrentTaskTo(work.round ? KnmnAuthShoukindaCallback : KnmnAuthTomoCallback);
        KnmnFade(10, true);
    }
    if (KnmnTickEndTimer())
        return;
    if (work.battleFrames == 5400) {
        check_dead_status(1);
        return;
    }
    if (++work.battleFrames == 5385 && !filmCheckFadeTrackColour())
        KnmnFade(10, false);
    ++work.battleFrames;
}
void KnmnAuthFirstEndCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        KnmnQueueStepAuthor(work.task, SHEN_NAME(BTFI));
        ++work.state;
    } else if (work.state == 1) {
        work.mode = 2;
        work.state = 0;
        EV_stepSetFlags(0, 0x19);
        HLib::SetCurrentTaskTo(KnmnButokuCallback);
        KnmnQueueStepAuthor(work.task, SHEN_NAME(SHO2));
    }
}
void KnmnAuthTomoCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 3);
        Vector3 target;
        EV_GetCharaPosition(KNMN_KEM, 5, &target, EventTransformFlags::World);
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), &target, 0);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(TOMO)));
        ++work.state;
    } else {
        if (work.state == 1) {
            mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 3);
            mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
            ++work.state;
        }
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            ++work.round;
            work.state = 0;
            EV_LoadAudio("0026A000.sbt", 0);
            EV_spmBeginSampleExecute();
            HLib::SetCurrentTaskTo(KnmnInitButokuCallback);
        }
    }
}
void KnmnAuthShoukindaCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    switch (work.state) {
    case 0: {
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 3);
        Vector3 target;
        EV_GetCharaPosition(KNMN_KEM, 5, &target, EventTransformFlags::World);
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), &target, 0);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(SHOU)));
        ++work.state;
        break;
    }
    case 1:
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 3);
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
        ++work.state;
        [[fallthrough]];
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work.state;
        break;
    case 3:
        KnmnQueueStepAuthor(work.task, SHEN_NAME(SHO2));
        ++work.state;
        break;
    default:
        work.state = 0;
        HLib::SetCurrentTaskTo(KnmnButokuCallback);
        break;
    }
}
void KnmnButokuCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        if (filmCheckFadeTrackColour())
            return;
        HLib::Task *task = HLib::EnqueueTaskWithParameter(KnmnPracticeMainCallback, 4, 0x0b, 0x688, KNMN_TASK);
        KnmnPracticeWork &practice = *static_cast<KnmnPracticeWork *>(HLib::GetTaskParameter(task));
        practice.task = task;
        practice.state = 0;
        practice.frames = 0;
        practice.active = 1;
        practice.fightSuspended = 0;
        practice.game = &work;
        work.practiceTask = task;
        knmnLeafEffect = -1;
        ++work.state;
    } else if (work.state == 1) {
        if (static_cast<KnmnPracticeWork *>(HLib::GetTaskParameter(work.practiceTask))->state != 6)
            return;
        HLib::DestroyTaskInternal(work.practiceTask);
        if (knmnLeafEffect != -1)
            FreeParticleEffect(3, knmnLeafEffect);
        knmnLeafEffect = -1;
        ++work.state;
    } else {
        work.state = 0;
        HLib::SetCurrentTaskTo(KnmnStopCallback);
    }
}
void KnmnEndCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    const std::uint8_t mode = work.mode;
    if (knmnLeafEffect != -1) {
        FreeParticleEffect(3, knmnLeafEffect);
        knmnLeafEffect = -1;
    }
    if (mode <= 3) {
        const std::int32_t flags[]{0x95, 0x19, 0xa1, 0x42};
        EV_stepSetFlags(0, flags[mode]);
    }
    flddSetPointFlagOnGrid(SHEN_NAME(0000), 1);
    flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 0);
    InitiateCleanupForSceneChara(KNMN_KEM);
    ET_AT_KillWork(work.characters);
    for (const char *package : {"KNMN", "KNMNAUTH"})
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_FreeAuthorPak(0, "0026");
    DeleteMotion(static_cast<std::int32_t>(work.masterMotion));
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.owner);
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    SetPlayerControlEnabled(0);
    scenFreeEvent("KNMN.EV3");
    if (mode == 2) {
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        LoadAreaFunc_Wrapper_0("WKKT.EV2", 0);
        ExecuteAreaMainFunction("WKKT.EV2", 1);
    } else {
        sbtLoadProgram();
        EV_spmBeginSampleExecute();
        KnmnMotion(KNMN_RYO, 0xf003);
        KnmnPosition(
            KNMN_RYO, mode ? Vector3{.x = 43.0999985f, .y = 102.300003f, .z = 243.100006f} : Vector3{.x = 46.9000015f, .y = 102.300003f, .z = 247.699997f});
        KnmnAngle(KNMN_RYO, mode ? 90 : 62);
        EV_NPC_NewLoadSw(0);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(KnmnWaitNpcCallback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(wNpc)));
        HLib::SetupTimedTask(task, 30);
        KnmnFade(30, true);
        EV_CheckWarmStartNpcRoute(KNMN_KEM);
        EV_ForceLoadNpc(KNMN_KEM);
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
}
void KnmnAuthWazaCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        work.state = 1;
        knmnLeafEffect = -1;
    } else if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        KnmnWazaFrameEvents();
    else {
        work.state = 0;
        if (knmnLeafEffect != -1) {
            FreeParticleEffect(3, knmnLeafEffect);
            knmnLeafEffect = -1;
        }
        HLib::SetCurrentTaskTo(KnmnStopCallback);
    }
}
const ScriptAreaDefinition knmnScriptDefinition{.areaId = KNMN_AREA, .field04 = 0, .init = InitializeKnmnEvent, .main = RunKnmnEvent};
} // namespace shenmue::s2::areas
#endif
