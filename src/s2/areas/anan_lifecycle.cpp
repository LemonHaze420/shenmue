#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_anan.hpp"

const char ananCreateTaskError[] = "error!! can't create task!! in ANAN.EV2\n";

#define ANAN_FREE_PACKAGE(name) \
    if (FTBL.EV_ControlPackageResources(0, 5, name, 0, 0)) \
        FTBL.EV_ControlPackageResources(0, 6, name, 0, 0)

void AnanFinish(void *data)
{
    AnanWork *work = (AnanWork *)data;
    unsigned int lifecycle, entry;
    HLibTask *caller;
    int finish, index, step;
    for (;;) {
        EventDebugUnregister(ANAN_DEBUG_ID);
        finish = 1;
        if (!work) {
            caller = 0;
            FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
            break;
        }
        caller = work->caller;
        finish = 0;
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work && (work->lifecycle & 2)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
        }
        FTBL.spmFadeOut(0);
        if (work && work->ryoTask) {
            work->lifecycle |= 0x10000000U;
            break;
        }
        if (work->ananTask) {
            work->lifecycle |= 0x20000000U;
            break;
        }
        lifecycle = work->lifecycle;
        entry = work->entry;
        if (lifecycle & 0x200) {
            for (index = 0; ananCharacters[index] != ANAN_INVALID; index++) {
                AnanShowCharacter(ananCharacters[index]);
                FTBL.AT_SetLocalBaseChara(ananCharacters[index], ANAN_INVALID);
            }
            work->lifecycle = lifecycle & ~0x200U;
        }
        if (lifecycle & 0x400) {
            FTBL.npcSetAreaName(ANAN_NPC_REGION, 0, 0.0f, 0);
            work->lifecycle = lifecycle & ~0x400U;
        }
        work->lifecycle &= 0x3fffffffU;
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(ANAN_MAN));
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(ANAN_RYO));
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ANAN_MAN));
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ANAN_RYO));
        if (lifecycle & 0x100) {
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 17, 2);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0xf087, 0);
            FTBL.npcFree(ANAN_MAN);
            FTBL.npcLockUnlock(ANAN_MAN);
            work->lifecycle = lifecycle & ~0x100U;
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.spmFadeOut(0);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_spmBeginSampleExecute(0);
        ANAN_FREE_PACKAGE("ANANFIR2");
        ANAN_FREE_PACKAGE("ANANFIR3");
        ANAN_FREE_PACKAGE("ANANFIR1");
        ANAN_FREE_PACKAGE("ANANSCC");
        ANAN_FREE_PACKAGE("ANANSCC");
        ANAN_FREE_PACKAGE("ANANTIOF");
        ANAN_FREE_PACKAGE("ANANANGR");
        ANAN_FREE_PACKAGE("ANANSTRT");
        ANAN_FREE_PACKAGE("ANAN");
        if (lifecycle & 4) {
            FTBL.EV_FreeAuthorPak(0, "0020");
            work->lifecycle = lifecycle & ~4U;
        }
        step = entry & 1 ? 28 : 19;
        if ((int)entry < 0)
            FTBL.EV_stepSetFlags(step, 0);
        else if (entry & 0x40000000U)
            FTBL.EV_stepSetFlags(step, 1);
        else
            FTBL.EV_stepSetFlags(step, 2);
        FTBL.EV_EventSetUpHelpIcon_0(0, 0);
        if (!(work->lifecycle & 0x8000U)) {
            work->lifecycle |= 0x8000U;
            FTBL.FinishLoading();
        }
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        finish = 1;
        break;
    }
    if (finish) {
        FTBL.SetPlayerControlEnabled(1);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        if (caller)
            FTBL.HLib_ResolveDeferredCallback(caller);
        else {
            FTBL.DebugLogToFileRemoved("Error!!\nParent Task ID is none!!  in ANAN.EV3\n");
            FTBL.EV_scenQueueLoad(1, ANAN_EXIT_AREA, 80);
        }
    }
}

void AnanFinishAuthor(void *data)
{
    AnanWork *work = (AnanWork *)data;
    Vector3 position;
    if (work) {
        FTBL.spmFadeOut(0);
        FTBL.GetCharaPosition(ANAN_MAN, 0xffffffffU, &position, ANAN_WORLD);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 17, 2);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0xf087, 0);
        FTBL.EV_AT_SetPos(ANAN_MAN, ANAN_WORLD_ALL_AXES, &position);
    }
    FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanLoadBoard(void *data)
{
    AnanPakWork *pak = (AnanPakWork *)data;
    Vector3 origin, position;
    Vector3i rotation;
    HLibTask *caller;
    int state;
    if (!pak)
        return;
    if (!FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANANBORD", 0))
        return;
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    origin = ananBoardOrigin;
    rotation = ananBoardRotation;
    position = ananFirstEntryPlacement[2].position;
    FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    FTBL.EV_AT_SetPos(ANAN_BOARD, ANAN_WORLD_ALL_AXES, &position);
    rotation = ananBoardRotation;
    if (!FTBL.EV_GetPlayerFlag(11, 29))
        rotation.z = (rotation.z + 1092) & 0xffff;
    FTBL.EV_AT_SetAngY_0(ANAN_BOARD, ANAN_WORLD_ALL_AXES, &rotation, 0);
    caller = pak->caller;
    state = pak->npcState;
    FTBL.HLib_DestroyCurrentEventTask();
    FTBL.HLib_ResolveDeferredCallback(caller);
    FTBL.EV_NPC_NewLoadSw_0(state);
    if (!pak->specialEntry)
        FTBL.SetPlayerControlEnabled(1);
}

void AnanInitialize(void *data)
{
    AnanWork *work = (AnanWork *)data;
    Vector3 center;
    if (work) {
        work->lifecycle |= 0x100;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        center = ananBoardOrigin;
        FTBL.npcSetAreaName(ANAN_NPC_REGION, &center, 5.5f, 1);
        work->lifecycle |= 0x400;
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANAN", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "ANAN", 0, 0);
        FTBL.EV_LoadCutsceneFromCurrentScene("0020E000.smp");
        FTBL.EV_LoadAudio("0020E000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_FreeAuthorPak(1, "0020");
        work->lifecycle |= 4;
        FTBL.EV_AT_SetPos(ANAN_BASE, ANAN_WORLD_ALL_AXES, &ananBoardOrigin);
        FTBL.EV_AT_SetAngY_0(ANAN_BASE, ANAN_WORLD_ALL_AXES, &ananBoardRotation, 0);
        FTBL.CheckWarmStartNpcRoute(ANAN_MAN);
        FTBL.ForceLoadNpc(ANAN_MAN);
        FTBL.HLib_SetCurrentTaskTo(AnanWaitForNpc);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanWaitForNpc(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int index;
    if (work && FTBL.npcStreamOrLoadHuman(ANAN_MAN) == 1) {
        for (index = 0; ananCharacters[index] != ANAN_INVALID; index++) {
            AnanHideCharacter(ananCharacters[index]);
            FTBL.AT_SetLocalBaseChara(ananCharacters[index], ANAN_BASE);
        }
        FTBL.EV_EnableCharacterDisplay(ANAN_MAN);
        FTBL.EV_EnsureCharacterDraw(ANAN_MAN);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ANAN_RYO));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ANAN_MAN));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ANAN_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ANAN_MAN), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ANAN_RYO), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ANAN_MAN), 1, 1, 0);
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(ANAN_RYO));
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(ANAN_MAN));
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(ANAN_RYO), 1);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(ANAN_MAN), 1);
        work->lifecycle |= 0x200;
        FTBL.HLib_SetCurrentTaskTo(AnanBeginEntry);
        EventDebugGetState(ANAN_DEBUG_ID, 1);
        EventDebugCheck(ANAN_DEBUG_ID, 4);
    }
}

void AnanBeginEntry(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanPlacement *placements;
    int index, finish = 1;
    if (work) {
        if (work->entry & 1) {
            placements = ananFirstEntryPlacement;
            for (index = 0; placements[index].character != ANAN_INVALID; index++) {
                FTBL.EV_AT_SetPos(placements[index].character, ANAN_ALL_AXES, &placements[index].position);
                FTBL.EV_AT_SetAngY_0(placements[index].character, ANAN_ALL_AXES, &placements[index].rotation, 0);
            }
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANANSTRT", 0);
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(2, 2, "ANANSTRT", 0, 0);
            FTBL.HLib_SetCurrentTaskTo(AnanBeginFirstChoice);
            FTBL.EV_NPC_NewLoadSw_0(1);
            work->targets[0] = ANAN_INVALID;
            work->targets[1] = ANAN_INVALID;
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->lifecycle |= 2;
            if (QueueAnanAuthor(ANAN_START_AUTHOR, 0, ananAuthorCharacters, 0, 1))
                finish = 0;
        } else {
            placements = ananRepeatEntryPlacement;
            for (index = 0; placements[index].character != ANAN_INVALID; index++) {
                FTBL.EV_AT_SetPos(placements[index].character, ANAN_ALL_AXES, &placements[index].position);
                FTBL.EV_AT_SetAngY_0(placements[index].character, ANAN_ALL_AXES, &placements[index].rotation, 0);
            }
            FTBL.HLib_SetCurrentTaskTo(AnanExplain);
            finish = 0;
        }
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanBeginFirstChoice(void *data)
{
    AnanWork *work = (AnanWork *)data;
    short choices[4];
    int index;
    if (work) {
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf002, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.0f, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0xf087, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.0f, 0);
        for (index = 0; ananFirstChoicePlacement[index].character != ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananFirstChoicePlacement[index].character, ANAN_ALL_AXES, &ananFirstChoicePlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananFirstChoicePlacement[index].character, ANAN_ALL_AXES, &ananFirstChoicePlacement[index].rotation, 0);
        }
        QueueAnanCamera("ANAN", 1514, ANAN_BASE, 0);
        choices[0] = -1;
        choices[3] = -1;
        choices[1] = 90;
        choices[2] = 154;
        FTBL.SetupControlSpeakAndUI(choices, 800, 154, 2);
        FTBL.HLib_SetCurrentTaskTo(AnanWaitFirstChoice);
        QueueAnanCamera("ANAN", 1900, ANAN_BASE, 0);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanWaitFirstChoice(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int icons, index;
    AnanVoiceCallback callback;
    callback = AnanVoiceResumeCaller;
    if (work) {
        icons = FTBL.GetShouldRenderIcons(2);
        if (!icons) {
        if (FTBL.shenGetCtrlSpeakIconFlag() == 90) {
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf003, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.0f, 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0x67c5, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.0f, 0);
            FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(ANAN_MAN), 40.0f, 0);
            FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANAN_MAN), 40.0f, 0);
            FTBL.HLib_SetCurrentTaskTo(AnanExplain);
            FTBL.HLib_SetupTaskDelay(-1);
            QueueAnanVoice("0020A002", 1, 0, 80, 0);
            QueueAnanVoice("0020B003", 1, 100, 161, AnanVoiceResumeCaller);
            QueueAnanCamera("ANAN", 1902, ANAN_BASE, 100);
        } else {
            for (index = 0; ananDeclinePlacement[index].character != ANAN_INVALID; index++) {
                FTBL.EV_AT_SetPos(ananDeclinePlacement[index].character, ANAN_ALL_AXES, &ananDeclinePlacement[index].position);
                FTBL.EV_AT_SetAngY_0(ananDeclinePlacement[index].character, ANAN_ALL_AXES, &ananDeclinePlacement[index].rotation, 0);
            }
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf003, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.0f, 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0xf087, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.0f, 0);
            FTBL.HLib_SetCurrentTaskTo(AnanFinish);
            FTBL.HLib_SetupTaskDelay(-1);
            QueueAnanVoice("0020A001", 1, 0, 120, 0);
            QueueAnanVoice("0020B024", 1, 130, 200, callback);
            QueueAnanCamera("ANAN", 1902, ANAN_BASE, 130);
        }
        ANAN_FREE_PACKAGE("ANANSTRT");
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanExplain(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanVoiceCallback resume;
    int index;
    do {
        if (work) {
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work->lifecycle & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
            break;
        }
        for (index = 0; ananExplanationPlacement[index].character != ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananExplanationPlacement[index].character, ANAN_ALL_AXES, &ananExplanationPlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananExplanationPlacement[index].character, ANAN_ALL_AXES, &ananExplanationPlacement[index].rotation, 0);
        }
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf003, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ANAN_RYO), 71, 0, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.2f, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0x67c6, 0);
        FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(ANAN_MAN), 220.0f, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANAN_MAN), 220.0f, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ANAN_MAN), 71, 0, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.2f, 0);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(AnanInitializeGame);
        FTBL.HLib_SetupTaskDelay(-1);
        resume = AnanVoiceResumeCaller;
        QueueAnanVoice("0020B004", 1, 5, 110, 0);
        QueueAnanVoice("0020B005", 1, 130, 195, 0);
        QueueAnanVoice("0020A003", 1, 200, 250, 0);
        QueueAnanVoice("0020B006", 1, 255, 350, 0);
        QueueAnanVoice("0020A004", 1, 355, 385, 0);
        QueueAnanVoice("0020B007", 1, 400, 445, resume);
        QueueAnanCamera("ANAN", 1903, ANAN_BASE, 0);
        } else
            FTBL.HLib_SetCurrentTaskTo(AnanFinish);
    } while (0);
}

void AnanBeginAngry(void *data)
{
    AnanWork *work = (AnanWork *)data;
    AnanVoiceCallback resume;
    int index;
    if (work) {
        for (index = 0; ananAngryPlacement[index].character == ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananAngryPlacement[index].character, ANAN_ALL_AXES, &ananAngryPlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananAngryPlacement[index].character, ANAN_ALL_AXES, &ananAngryPlacement[index].rotation, 0);
        }
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0x67c4, 0);
        FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(ANAN_MAN), 180.0f, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANAN_MAN), 180.0f, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.0f, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 9, 2);
        FTBL.HLib_SetCurrentTaskTo(AnanPlayAngryAuthor);
        FTBL.HLib_SetupTaskDelay(-1);
        resume = AnanVoiceResumeCaller;
        QueueAnanVoice("0020B028", 1, 10, 85, resume);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanPlayAngryAuthor(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    while (work) {
        if (FTBL.filmCheckFadeTrackColour()) {
            finish = 0;
            break;
        }
        {
            if (work->lifecycle & 2) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->lifecycle &= ~2U;
            } else {
                FTBL.EV_NPC_NewLoadSw_0(0);
                FTBL.EV_ResetFreeTalkFlags();
                FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANANANGR", 0);
                FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
                FTBL.EV_ControlPackageResources(2, 2, "ANANANGR", 0, 0);
                FTBL.HLib_SetCurrentTaskTo(AnanFinishAuthor);
                FTBL.EV_NPC_NewLoadSw_0(1);
                work->targets[0] = ANAN_INVALID;
                work->targets[1] = ANAN_INVALID;
                if (!QueueAnanAuthor(ANAN_ANGRY_AUTHOR, 0, ananAuthorCharacters, 1, 0))
                    break;
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            }
        }
        finish = 0;
        break;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanBeginTimeOver(void *data)
{
    AnanWork *work = (AnanWork *)data;
    unsigned int motion;
    int index;
    float time;
    AnanVoiceCallback callback;
    if (work) {
        for (index = 0; ananAngryPlacement[index].character == ANAN_INVALID; index++) {
            FTBL.EV_AT_SetPos(ananTimeOverPlacement[index].character, ANAN_ALL_AXES, &ananTimeOverPlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananTimeOverPlacement[index].character, ANAN_ALL_AXES, &ananTimeOverPlacement[index].rotation, 0);
        }
        if (AnanIsStraight(&work->game)) {
            motion = 0x67c3;
            time = 180.0f;
        } else {
            motion = 0x67c8;
            time = 180.0f;
        }
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), motion, 0);
        FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(ANAN_MAN), time, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANAN_MAN), time, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 1.0f, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 9, 2);
        FTBL.HLib_SetCurrentTaskTo(AnanPlayTimeOverAuthor);
        FTBL.HLib_SetupTaskDelay(-1);
        callback = AnanVoiceResumeCaller;
        QueueAnanVoice("0020B054", 1, 30, 135, callback);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanPlayTimeOverAuthor(void *data)
{
    static char successPackage[] = "ANANSCC";
    static char timeOverPackage[] = "ANANTIOF";
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    char *package;
    shen_name author;
    int fadeOut;
    HLibTaskCallback next;
    int finishBeforeUnload;
    while (work) {
        if (FTBL.filmCheckFadeTrackColour()) {
            finish = 0;
            break;
        }
        if (work->lifecycle & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
        } else {
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_ResetFreeTalkFlags();
            if (AnanIsStraight(&work->game)) {
                package = successPackage;
                author = ANAN_SUCCESS_AUTHOR;
                work->entry |= 0x80000000U;
                fadeOut = 1;
                next = AnanFinishAuthor;
                finishBeforeUnload = 0;
            } else {
                work->entry |= 0x40000000U;
                package = timeOverPackage;
                author = ANAN_TIME_FAIL_AUTHOR;
                fadeOut = 0;
                next = AnanFinishTimeOver;
                finishBeforeUnload = 1;
            }
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)package, 0);
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(2, 2, package, 0, 0);
            FTBL.HLib_SetCurrentTaskTo(next);
            FTBL.EV_NPC_NewLoadSw_0(1);
            work->targets[0] = ANAN_INVALID;
            work->targets[1] = ANAN_INVALID;
            if (!QueueAnanAuthor(author, 0, ananAuthorCharacters, fadeOut, finishBeforeUnload))
                break;
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            if (!fadeOut)
                work->lifecycle |= 2;
        }
        finish = 0;
        break;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanFinishTimeOver(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    if (work) {
        FTBL.HLib_SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_TIME_END_AUTHOR, 0, ananAuthorCharacters, 1, 0)) {
            work->lifecycle &= ~2U;
            finish = 0;
        }
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanSuccess(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    while (work) {
        AnanUpdatePose(&work->game);
        AnanSetTargets(work->game.tilt, 0, 1);
        if (FTBL.filmCheckFadeTrackColour()) {
            finish = 0;
            break;
        }
        if (work->lifecycle & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
        } else {
            work->targets[0] = ANAN_INVALID;
            work->entry |= 0x80000000U;
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_ResetFreeTalkFlags();
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANANSCC", 0);
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(2, 2, "ANANSCC", 0, 0);
            FTBL.HLib_SetCurrentTaskTo(AnanFinishAuthor);
            FTBL.EV_NPC_NewLoadSw_0(1);
            work->targets[0] = ANAN_INVALID;
            work->targets[1] = ANAN_INVALID;
            if (!QueueAnanAuthor(ANAN_SUCCESS_AUTHOR, 0, ananAuthorCharacters, 1, 0))
                break;
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        }
        finish = 0;
        break;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanFailure(void *data)
{
    static char failurePackage[] = "ANANFIR1";
    static char fifthPackage[] = "ANANFIR3";
    AnanWork *work = (AnanWork *)data;
    char *package;
    shen_name author;
    HLibTaskCallback next;
    int finish = 1;
    while (work) {
        AnanUpdatePose(&work->game);
        AnanSetTargets(work->game.tilt, 0, 1);
        if (FTBL.filmCheckFadeTrackColour()) {
            finish = 0;
            break;
        }
        if (work->lifecycle & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
        } else {
            work->targets[0] = ANAN_INVALID;
            if (work->game.failures <= 4) {
                next = AnanOfferRetry;
                FTBL.DebugLogToFileRemoved("%d
", work->game.failures);
                package = failurePackage;
                author = ANAN_FAIL_AUTHOR;
                FTBL.EV_authSceneCtrl(-8);
            } else {
                work->entry |= 0x80000000U;
                next = AnanFinishFifthFailure;
                package = fifthPackage;
                author = ANAN_FIFTH_AUTHOR;
            }
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_ResetFreeTalkFlags();
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)package, 0);
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(2, 2, package, 0, 0);
            FTBL.HLib_SetCurrentTaskTo(next);
            FTBL.EV_NPC_NewLoadSw_0(1);
            work->targets[0] = ANAN_INVALID;
            work->targets[1] = ANAN_INVALID;
            if (!QueueAnanAuthor(author, 0, ananAuthorCharacters, 0, 1))
                break;
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->lifecycle |= 2;
        }
        finish = 0;
        break;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanOfferRetry(void *data)
{
    AnanWork *work = (AnanWork *)data;
    short choices[4];
    Vector3 position;
    Vector3i rotation;
    if (work) {
        position.x = 0.488f;
        position.y = -0.005f;
        position.z = 2.409f;
        FTBL.EV_AT_SetPos(ANAN_RYO, ANAN_ALL_AXES, &position);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf003, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.0f, 0);
        rotation.x = 0;
        rotation.y = 13471;
        rotation.z = 0;
        FTBL.EV_AT_SetAngY_0(ANAN_RYO, ANAN_ALL_AXES, &rotation, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_RYO), 17, 0);
        FTBL.EV_authSceneCtrl(-7);
        choices[0] = -1;
        choices[3] = -1;
        choices[1] = 152;
        choices[2] = 153;
        FTBL.SetupControlSpeakAndUI(choices, 800, 153, 2);
        QueueAnanCamera("ANAN", 1905, ANAN_BASE, 0);
        FTBL.HLib_SetCurrentTaskTo(AnanWaitRetry);
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanWaitRetry(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int icons, index, finish = 1;
    if (work) {
        icons = FTBL.GetShouldRenderIcons(2);
        if (!icons) {
            ANAN_FREE_PACKAGE("ANANFIR1");
            if (FTBL.shenGetCtrlSpeakIconFlag() == 152) {
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 17, 0);
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_RYO), 17, 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_RYO), 0);
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_RYO), 0xf003, 0);
                FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_RYO), 1.0f, 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0x67c2, 0);
                FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 0.9f, 0);
                FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(ANAN_MAN), 0);
                for (index = 0; ananRetryPlacement[index].character != ANAN_INVALID; index++) {
                    FTBL.EV_AT_SetPos(ananRetryPlacement[index].character, ANAN_ALL_AXES, &ananRetryPlacement[index].position);
                    FTBL.EV_AT_SetAngY_0(ananRetryPlacement[index].character, ANAN_ALL_AXES, &ananRetryPlacement[index].rotation, 0);
                }
                FTBL.HLib_SetCurrentTaskTo(AnanRetry);
                finish = 0;
                FTBL.HLib_SetupTaskDelay(-1);
                SHENMUE_EVENT_BOUNDARY();
                QueueAnanCamera("ANAN", 1906, ANAN_BASE, 0);
                QueueAnanVoice("0020A017", 1, 0, 100, 0);
                QueueAnanVoice("0020B022", 1, 101, 150, AnanVoiceResumeCaller);
            } else {
                FTBL.EV_NPC_NewLoadSw_0(0);
                FTBL.EV_ResetFreeTalkFlags();
                FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ANANFIR2", 0);
                FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
                FTBL.EV_ControlPackageResources(2, 2, "ANANFIR2", 0, 0);
                FTBL.HLib_SetCurrentTaskTo(AnanFinishFailure);
                FTBL.EV_NPC_NewLoadSw_0(1);
                work->targets[0] = ANAN_INVALID;
                work->targets[1] = ANAN_INVALID;
                if (QueueAnanAuthor(ANAN_FAIL_FIRST_AUTHOR, 0, ananAuthorCharacters, 0, 1))
                    finish = 0;
            }
        } else
            finish = 0;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanFinishFailure(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    if (work) {
        FTBL.HLib_SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FAIL_END_AUTHOR, 0, ananAuthorCharacters, 1, 0)) {
            work->lifecycle &= ~2U;
            finish = 0;
        }
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanFinishFifthFailure(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int finish = 1;
    if (work) {
        FTBL.HLib_SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FIFTH_END_AUTHOR, 0, ananAuthorCharacters, 1, 0)) {
            work->lifecycle &= ~2U;
            finish = 0;
        }
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanTrackRyo(void *data)
{
    AnanWork *work;
    Vector3 point;
    int done = 1;
    if (*(AnanWork **)data) {
        work = *(AnanWork **)data;
        do {
            if (work->lifecycle & 0x10000000U) {
                if (work->lifecycle & 0x40000000U) {
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_RYO), 0, 0);
                    work->lifecycle &= ~0x40000000U;
                }
                work->ryoTask = 0;
            } else if (work->targets[0] != ANAN_INVALID) {
                if (work->targets[0])
                    FTBL.GetMotionBonePosition(FTBL.scenFindMotionwork(work->targets[0]), 5, &point);
                else
                    point = work->positions[0];
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_RYO), &point, work->positionModes[0]);
                if (work->transitionFrames[0])
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_RYO), 0, (work->transitionFrames[0] << 8) + 4);
                else
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_RYO), 0, 3);
                work->lifecycle |= 0x40000000U;
                done = 0;
            } else {
                if (work->lifecycle & 0x40000000U) {
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_RYO), 0, 0);
                    work->lifecycle &= ~0x40000000U;
                }
                done = 0;
            }
        } while (0);
    }
    if (done)
        FTBL.HLib_DestroyCurrentEventTask();
}

void AnanTrackMan(void *data)
{
    AnanWork *work;
    Vector3 point;
    int done = 1;
    if (*(AnanWork **)data) {
        work = *(AnanWork **)data;
        do {
            if (work->lifecycle & 0x20000000U) {
                if (work->lifecycle & 0x80000000U) {
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_MAN), 0, 0);
                    work->lifecycle &= ~0x80000000U;
                }
                work->ananTask = 0;
            } else if (work->targets[1] != ANAN_INVALID) {
                if (work->targets[1])
                    FTBL.GetMotionBonePosition(FTBL.scenFindMotionwork(work->targets[1]), 5, &point);
                else
                    point = work->positions[1];
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_MAN), &point, work->positionModes[1]);
                if (work->transitionFrames[1])
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_MAN), 0, (work->transitionFrames[1] << 8) + 4);
                else
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_MAN), 0, 3);
                work->lifecycle |= 0x80000000U;
                done = 0;
            } else {
                if (work->lifecycle & 0x80000000U) {
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(ANAN_MAN), 0, 0);
                    work->lifecycle &= ~0x80000000U;
                }
                done = 0;
            }
        } while (0);
    }
    if (done)
        FTBL.HLib_DestroyCurrentEventTask();
}

void AnanDebugSkip(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int flag;
    while (work) {
        FTBL.HLib_AssignCallbackForTask(work->self, AnanFinish);
        FTBL.HLib_SetupTimedTask(work->self, 1);
        if (work->entry & 0xc0000000U)
            break;
        flag = work->entry & 1 ? 28 : 19;
        FTBL.EV_stepSetFlags(flag, 0);
        AnanSetBoardTilt(0.0f);
        break;
    }
}

void AnanHideCharacter(shen_name character)
{
    float reflector[4];
    int hasMotion = FTBL.scenFindMotionwork(character) != 0;
    if (hasMotion) {
        reflector[0] = 1.0f;
        reflector[1] = 1.0f;
        reflector[2] = 1.0f;
        reflector[3] = 1.0f;
        FTBL.SetScenarioReflectorBoardValues(character, reflector);
        FTBL.SetRefBoardParamsForCharaById(character, 1);
    }
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 0);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void AnanShowCharacter(shen_name character)
{
    int hasMotion = FTBL.scenFindMotionwork(character) != 0;
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 1);
    FTBL.EV_SetCharaControllerFlags(character, 1, 1);
    if (hasMotion)
        FTBL.SetRefBoardParamsForCharaById(character, 0);
}

int QueueAnanAuthor(shen_name author, AnanAuthorCallback frameCallback, shen_name *characters, int fadeOut, int finishBeforeUnload)
{
    HLibTask *task;
    AnanAuthWork *work;
    shen_name character;
    int count, index, started = 0;
    do {
        count = 0;
        if (characters) {
            for (count = 0; characters[count] != ANAN_INVALID; count++)
                ;
            count++;
        }
        task = FTBL.HLib_EnqueueTaskWithParameter(AnanInitializeAuthor, 4, 11, count * 8 + 28, ANAN_TASK);
        if (task) {
            work = (AnanAuthWork *)FTBL.HLib_GetTaskParameter(task);
            if (work) {
                work->caller = FTBL.HLib_GetCurrentTask();
                work->frameCallback = frameCallback;
                work->fadeOut = fadeOut;
                work->finishBeforeUnload = finishBeforeUnload;
                work->author = author;
                if (count) {
                    work->characters = (shen_name *)(work + 1);
                    work->parents = work->characters + count;
                    for (index = 0; characters[index] != ANAN_INVALID; index++) {
                        character = characters[index];
                        work->characters[index] = character;
                        work->parents[index] = FTBL.SCEN_FindChar_Wrapper(character);
                        FTBL.AT_SetLocalBaseChara(character, ANAN_FREE);
                    }
                    work->characters[index] = ANAN_INVALID;
                    work->parents[index] = ANAN_INVALID;
                } else {
                    work->characters = 0;
                    work->parents = 0;
                }
                FTBL.HLib_SetupTaskDelay(-1);
                started = 1;
            }
        }
    } while (0);
    if (!started && task)
        FTBL.HLib_DestroyTask(task);
    return started;
}

void AnanInitializeAuthor(void *data)
{
    AnanAuthWork *work = (AnanAuthWork *)data;
    if (work) {
        FTBL.EV_authSceneCtrl(work->author);
        AnanWaitAuthor(work);
        FTBL.HLib_SetCurrentTaskTo(AnanWaitAuthor);
    }
}

void AnanWaitAuthor(void *data)
{
    AnanAuthWork *work = (AnanAuthWork *)data;
    int frame, done = 0, index;
    do {
        if (!work)
            break;
        frame = FTBL.EV_authSceneCtrl(-6);
        if (work->frameCallback && work->frameCallback(frame)) {
            FTBL.EV_authSceneCtrl(-4);
            done = 1;
        }
        if (work->fadeOut && frame + 32 == FTBL.EV_authSceneCtrl(-5))
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        if ((FTBL.EV_authSceneCtrl(-5) - 1 <= frame && work->finishBeforeUnload) || !FTBL.EV_authSceneCtrl(-1) || done) {
            if (work->characters && work->parents)
                for (index = 0; work->characters[index] != ANAN_INVALID; index++)
                    FTBL.AT_SetLocalBaseChara(work->characters[index], work->parents[index]);
            FTBL.HLib_ResolveDeferredCallback(work->caller);
            FTBL.HLib_DestroyCurrentEventTask();
        }
    } while (0);
}

void QueueAnanVoice(const char *text, int voice, int startTicks, int endTicks, AnanVoiceCallback callback)
{
    HLibTask *parent, *task;
    AnanVoiceWork *work;
    int length, index;
    parent = FTBL.HLib_GetCurrentTask();
    if (text && endTicks > 0) {
        for (length = 0; text[length]; length++)
            ;
        task = FTBL.HLib_EnqueueTaskWithParameter(AnanVoiceLoop, 4, 11, startTicks > 0 ? length + 28 : 27, ANAN_TASK);
        if (task) {
            work = (AnanVoiceWork *)(FTBL.HLib_GetTaskParameter(task));
            if (work) {
                work->parent = parent;
                work->startTicks = startTicks;
                work->endTicks = endTicks;
                work->voice = voice;
                work->callback = callback;
                work->conversation = 0;
                if (startTicks <= 0) {
                    if (voice) {
                        work->conversation = FTBL.EV_PlayDialogue(text);
                        if (!work->conversation) {
                            FTBL.HLib_DestroyTask(task);
                            if (callback)
                                callback(0, (int)parent);
                        } else if (callback)
                            callback(1, 0);
                    } else {
                        FTBL.dbgPrintEventMessage(text);
                        if (callback)
                            callback(1, 0);
                    }
                } else {
                    for (index = 0; text[index]; index++)
                        work->text[index] = text[index];
                    work->text[index] = 0;
                }
            } else {
                FTBL.HLib_DestroyTask(task);
                if (callback)
                    callback(0, (int)parent);
            }
        }
    } else if (callback)
        callback(0, (int)parent);
}

void AnanVoiceLoop(void *data)
{
    AnanVoiceWork *work = (AnanVoiceWork *)data;
    if (work) {
        if (!--work->startTicks) {
            if (work->voice) {
                work->conversation = FTBL.EV_PlayDialogue(work->text);
                if (!work->conversation) {
                    FTBL.HLib_DestroyCurrentEventTask();
                    if (work->callback)
                        work->callback(0, (int)work->parent);
                } else if (work->callback)
                    work->callback(1, 0);
            } else {
                FTBL.dbgPrintEventMessage(work->text);
                if (work->callback)
                    work->callback(1, 0);
            }
        }
        if (!--work->endTicks) {
            if (work->startTicks <= 0) {
                if (work->voice) {
                    if (work->conversation && FTBL.cvsHasValidConverseData(work->conversation))
                        FTBL.scenCvsCtrlFree(work->conversation);
                    work->conversation = 0;
                } else
                    FTBL.ctrlStopSubtitles();
            }
            FTBL.HLib_DestroyCurrentTask();
            if (work->callback)
                work->callback(3, (int)work->parent);
        }
        if (work->callback)
            work->callback(2, work->endTicks);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void AnanVoiceResumeCaller(int event, int argument)
{
    if (event == 3)
        FTBL.HLib_ResolveDeferredCallback((HLibTask *)argument);
}

void AnanVoiceUnlockInput(int event, int argument)
{
    AnanWork *work;
    if (event == 3) {
        work = (AnanWork *)FTBL.HLib_GetTaskParameter((HLibTask *)argument);
        if (work)
            work->game.gameFlags &= ~0x80000000U;
    }
}

void QueueAnanCamera(const char *package, int camera, shen_name base, int delay)
{
    HLibTask *task;
    AnanCameraWork *work;
    int length, index;
    if (delay <= 0) {
        if (package && *package)
            FTBL.EV_ControlPackageResources(2, 4, package, 0, 0);
        if (base)
            FTBL.EVS_LoopCamera(camera, base, base);
        else
            FTBL.EVS_LoopCamera(camera, 0, 0);
    } else {
        length = 0;
        if (package)
            for (; package[length]; length++)
                ;
        length += 16;
        task = FTBL.HLib_EnqueueTaskWithParameter(AnanCamera, 4, 11, length, ANAN_TASK);
        if (task) {
            work = (AnanCameraWork *)FTBL.HLib_GetTaskParameter(task);
            if (work) {
                work->camera = camera;
                work->base = base;
                work->delay = delay;
                for (index = 0; index < length; index++)
                    work->package[index] = package[index];
                work->package[index] = 0;
            } else
                FTBL.HLib_DestroyTask(task);
        }
    }
}

void AnanCamera(void *data)
{
    AnanCameraWork *work = (AnanCameraWork *)data;
    char *package;
    if (work) {
        if (--work->delay <= 0) {
            package = work->package;
            if (package && *package)
                FTBL.EV_ControlPackageResources(2, 4, package, 0, 0);
            if (work->base)
                FTBL.EVS_LoopCamera(work->camera, work->base, work->base);
            else
                FTBL.EVS_LoopCamera(work->camera, 0, 0);
            FTBL.HLib_DestroyCurrentEventTask();
        }
    } else
        FTBL.HLib_DestroyCurrentEventTask();
}

void AnanSetBoardTilt(float tilt)
{
    Vector3i rotation;
    AnanGetBoardRotation(&rotation, tilt);
    FTBL.EV_AT_SetAngY_0(ANAN_BOARD, ANAN_ALL_AXES, &rotation, 0);
}

void AnanGetBoardRotation(Vector3i *rotation, float tilt)
{
    if (rotation) {
        rotation->x = 0;
        rotation->y = 0;
        rotation->z = (int)(tilt * 1274.0f);
    }
}

void AnanGetHandPosition(float tilt, Vector3 *output)
{
    Vector3 origin, point;
    Vector3i rotation;
    origin.x = 0.0f;
    origin.y = 0.0f;
    origin.z = 0.0f;
    AnanGetBoardRotation(&rotation, tilt);
    point.x = -1.23f;
    point.y = -0.3f;
    point.z = 0.0f;
    FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &point);
    FTBL.GetCharaPosition(ANAN_BOARD, 0xffffffffU, &origin, ANAN_WORLD);
    FTBL.GetScenarioCharacterEulerAngles(ANAN_BOARD, &rotation, ANAN_WORLD);
    FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &point);
    if (output)
        *output = point;
}

shen_name ananCharacters[] = {ANAN_RYO, ANAN_MAN, ANAN_BOARD, ANAN_STOOL, ANAN_INVALID};
shen_name ananAuthorCharacters[] = {ANAN_RYO, ANAN_MAN, ANAN_STOOL, ANAN_INVALID};
Vector3 ananBoardOrigin = {95.2169037f, 98.1999969f, 130.546097f};
Vector3i ananBoardRotation = {0, 0x6000, 0};
AnanPlacement ananFirstEntryPlacement[] = {{ANAN_RYO, {8.78f, -0.1f, -3.56f}, {0, 0x60ec, 0}}, {ANAN_MAN, {-0.09f, 0.0f, 2.38f}, {0, 0x2740, 0}},
    {ANAN_BOARD, {1.23f, 3.23f, 0.0f}, {0, 0, 0xfbbc}}, {ANAN_STOOL, {-1.43f, -0.2f, 0.46f}, {0, 0x3999, 0}}, {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananFirstChoicePlacement[] = {{ANAN_RYO, {2.14f, -0.1f, 3.46f}, {0, 0x1555, 0}}, {ANAN_MAN, {0.0f, 0.0f, 2.44f}, {0, 0x327d, 0}},
    {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananDeclinePlacement[] = {{ANAN_RYO, {2.14f, -0.1f, 3.46f}, {0, 0x1555, 0}}, {ANAN_MAN, {0.0f, 0.0f, 2.44f}, {0, 0xc222, 0}},
    {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananRepeatEntryPlacement[] = {{ANAN_RYO, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {ANAN_MAN, {0.0f, 0.0f, 2.44f}, {0, 0x327d, 0}},
    {ANAN_BOARD, {1.23f, 3.23f, 0.0f}, {0, 0, 0xfbbc}}, {ANAN_STOOL, {-1.43f, -0.2f, 0.46f}, {0, 0x3999, 0}}, {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananExplanationPlacement[] = {{ANAN_RYO, {0.0f, 0.0f, 1.92f}, {0, 0, 0}}, {ANAN_MAN, {-1.37f, 1.27f, 0.69f}, {0, 0xfe93, 0}},
    {ANAN_STOOL, {-1.35f, -0.2f, 0.46f}, {0, 0x3999, 0}}, {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananRetryPlacement[] = {{ANAN_RYO, {0.49f, 0.0f, 2.4f}, {0, 0x341f, 0}}, {ANAN_MAN, {-0.69f, 0.0f, 2.4f}, {0, 0xb41f, 0}},
    {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananAngryPlacement[] = {{ANAN_MAN, {-1.3f, 1.27f, 0.43f}, {0, 0xfddd, 0}}, {ANAN_STOOL, {-1.35f, -0.2f, 0.46f}, {0, 0x3999, 0}},
    {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
AnanPlacement ananTimeOverPlacement[] = {{ANAN_MAN, {-1.3f, 1.27f, 0.43f}, {0, 0xfddd, 0}}, {ANAN_STOOL, {-1.35f, -0.2f, 0.46f}, {0, 0x3999, 0}},
    {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
#else
#include "s2/areas/area_anan.hpp"
#include "s2/areas/anan_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2::areas {
void AnanFinish(void *data) {
    using enum CameraState;
    AnanWork *work = static_cast<AnanWork *>(data);
    HLib::Task *caller = nullptr;
    if (work) {
        caller = work->caller;
        if (filmCheckFadeTrackColour())
            return;
        if (work->lifecycle & 2U) {
            AnanFilm(32, 0, 255);
            work->lifecycle &= ~2U;
        }
        spmFadeOut();
        if (work->ryoTask) {
            work->lifecycle |= 0x10000000U;
            return;
        }
        if (work->ananTask) {
            work->lifecycle |= 0x20000000U;
            return;
        }
        const std::uint32_t entry = work->entry, lifecycle = work->lifecycle;
        if (lifecycle & 0x200U) {
            for (const shen_name *name = ananCharacters; name->name != ANAN_INVALID.name; ++name) {
                const bool hasMotion = scenFindMotionwork(*name) != nullptr;
                EV_SetCharacterAlphaEnabled(*name, true);
                if (hasMotion)
                    mtwkSetExtraFlag(scenFindMotionwork(*name), 6, 0);
                EV_SetCharaControllerFlags(*name, 0, NpcCharacterControllerState::Enabled);
                EV_SetCharaControllerFlags(*name, 1, NpcCharacterControllerState::Enabled);
                if (hasMotion)
                    EV_SetRefBoardParamsFor(*name, 0);
                AT_SetLocalBaseChara(*name, ANAN_INVALID);
            }
            work->lifecycle = lifecycle & ~0x200U;
        }
        if (lifecycle & 0x400U) {
            npcSetAreaName(ANAN_NPC_REGION, nullptr, 0, 0);
            work->lifecycle = lifecycle & ~0x400U;
        }
        work->lifecycle &= 0x3fffffffU;
        FindMotionWork(ctrlGetCharaByName(ANAN_MAN));
        FindMotionWork(ctrlGetCharaByName(ANAN_RYO));
        hndlSetFlags(ctrlGetCharaByName(ANAN_MAN));
        hndlSetFlags(ctrlGetCharaByName(ANAN_RYO));
        if (lifecycle & 0x100U) {
            mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 2);
            mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
            mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
            EV_npcFree(ANAN_MAN);
            EV_npcLockUnlock(ANAN_MAN);
            work->lifecycle = lifecycle & ~0x100U;
        }
        AnanFilm(32, 255, 0);
        spmFadeOut();
        sbtLoadProgram();
        EV_spmBeginSampleExecute();
        const char *packages[]{"ANANFIR2", "ANANFIR3", "ANANFIR1", "ANANSCC", "ANANSCC", "ANANTIOF", "ANANANGR", "ANANSTRT", "ANAN"};
        for (const char *package : packages) {
            if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
                EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
        }
        if (lifecycle & 4U) {
            EV_FreeAuthorPak(0, "0020");
            work->lifecycle = lifecycle & ~4U;
        }
        const std::int32_t flag = (entry & 1U) ? 28 : 19;
        const std::uint8_t mode = (entry & 0x80000000U) ? 0 : (entry & 0x40000000U) ? 1 : 2;
        EV_stepSetFlags(mode, flag);
        EV_EventSetUpHelpIcon_0(0, 0);
        if (!(work->lifecycle & 0x8000U)) {
            work->lifecycle |= 0x8000U;
            FinishLoading();
        }
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else
        AnanFilm(0, 255, 0);
    SetPlayerControlEnabled(1);
    HLib::MarkCurrentTaskForCleanup();
    if (caller)
        HLib::ResolveDeferredCallback(caller);
    else
        EV_scenQueueLoad(1, ANAN_EXIT_AREA, 80);
}
void AnanFinishAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        spmFadeOut();
        Vector3 position;
        EV_GetCharaPosition(ANAN_MAN, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 2);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
        EV_AT_SetPos(ANAN_MAN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanInitialize(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    work->lifecycle |= 0x100U;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    Vector3 center = ANBD_BOARD_ORIGIN;
    npcSetAreaName(ANAN_NPC_REGION, &center, 5.5f, 1);
    work->lifecycle |= 0x400U;
    AnanLoadPackage("ANAN");
    EV_LoadCutsceneFromCurrentScene("0020E000.smp");
    EV_LoadAudio("0020E000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0020");
    work->lifecycle |= 4;
    EV_AT_SetPos(ANAN_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, ANBD_BOARD_ORIGIN);
    EV_AT_SetAngY(ANAN_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, ANBD_BOARD_ROTATION);
    EV_CheckWarmStartNpcRoute(ANAN_MAN);
    EV_ForceLoadNpc(ANAN_MAN);
    HLib::SetCurrentTaskTo(AnanWaitForNpc);
}
void AnanWaitForNpc(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work || EV_npcStreamOrLoadHuman(ANAN_MAN) != 1)
        return;
    for (const shen_name *name = ananCharacters; name->name != ANAN_INVALID.name; ++name) {
        AnanSuspendCharacter(*name);
        AT_SetLocalBaseChara(*name, ANAN_BASE);
    }
    EV_EnableCharacterDisplay(ANAN_MAN);
    EV_EnsureCharacterDraw(ANAN_MAN);
    SetLowQualityHands(ctrlGetCharaByName(ANAN_RYO));
    SetLowQualityHands(ctrlGetCharaByName(ANAN_MAN));
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_MAN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_RYO), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_MAN), 1, 1, 0);
    FindMotionWork(ctrlGetCharaByName(ANAN_RYO));
    FindMotionWork(ctrlGetCharaByName(ANAN_MAN));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ANAN_RYO), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ANAN_MAN), FacialNodeReplacement);
    work->lifecycle |= 0x200U;
    HLib::SetCurrentTaskTo(AnanBeginEntry);
}
void AnanBeginEntry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (!(work->entry & 1U)) {
        ApplyAnanPlacements(ananRepeatEntryPlacement);
        HLib::SetCurrentTaskTo(AnanExplain);
        return;
    }
    ApplyAnanPlacements(ananFirstEntryPlacement);
    AnanLoadPackage("ANANSTRT");
    HLib::SetCurrentTaskTo(AnanBeginFirstChoice);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    AnanFilm(32, 255, 0);
    work->lifecycle |= 2;
    if (!QueueAnanAuthor(ANAN_START_AUTHOR, 0, 1))
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanBeginFirstChoice(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf002, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    ApplyAnanPlacements(ananFirstChoicePlacement);
    AnanSelectCamera(1514);
    std::int16_t choices[]{-1, 90, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    HLib::SetCurrentTaskTo(AnanWaitFirstChoice);
    AnanSelectCamera(1900);
}
void AnanWaitFirstChoice(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 90) {
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
        mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c5, MotionRegion::LowerBody);
        mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
        mtwkSetLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 40, MotionRegion::LowerBody);
        mtwkSetRegionEvalTime(scenFindMotionwork(ANAN_MAN), 40, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(AnanExplain);
        HLib::SetupTaskDelay(-1);
        QueueAnanVoice("0020A002", 0, 80, nullptr);
        QueueAnanVoice("0020B003", 100, 161, AnanVoiceResumeCaller);
        QueueAnanCamera(1902, 100);
    } else {
        ApplyAnanPlacements(ananDeclinePlacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
        mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
        mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(AnanFinish);
        HLib::SetupTaskDelay(-1);
        QueueAnanVoice("0020A001", 0, 120, nullptr);
        QueueAnanVoice("0020B024", 130, 200, AnanVoiceResumeCaller);
        QueueAnanCamera(1902, 130);
    }
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("ANANSTRT"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ANANSTRT"), 0);
}
void AnanExplain(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    ApplyAnanPlacements(ananExplanationPlacement);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkSetRegionFlag(scenFindMotionwork(ANAN_RYO), 71, 0, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1.2f, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c6, MotionRegion::LowerBody);
    mtwkSetLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 220, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(scenFindMotionwork(ANAN_MAN), 220, MotionRegion::LowerBody);
    mtwkSetRegionFlag(scenFindMotionwork(ANAN_MAN), 71, 0, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1.2f, MotionRegion::LowerBody);
    AnanFilm(32, 255, 0);
    HLib::SetCurrentTaskTo(AnanInitializeGame);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B004", 5, 110, nullptr);
    QueueAnanVoice("0020B005", 130, 195, nullptr);
    QueueAnanVoice("0020A003", 200, 250, nullptr);
    QueueAnanVoice("0020B006", 255, 350, nullptr);
    QueueAnanVoice("0020A004", 355, 385, nullptr);
    QueueAnanVoice("0020B007", 400, 445, AnanVoiceResumeCaller);
    AnanSelectCamera(1903);
}
void AnanBeginAngry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }

    if (ananAngryPlacement[0].character.name == ANAN_INVALID.name) {
        const AnanPlacement *row = ananAngryPlacement;
        do {
            EV_AT_SetPos(row->character, EventTransformFlags::AllAxes, row->position);
            EV_AT_SetAngY(row->character, EventTransformFlags::AllAxes, row->rotation);
            ++row;
        } while (row->character.name == ANAN_INVALID.name);
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c4, MotionRegion::LowerBody);
    mtwkSetLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
    HLib::SetCurrentTaskTo(AnanPlayAngryAuthor);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B028", 10, 85, AnanVoiceResumeCaller);
}
void AnanPlayAngryAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage("ANANANGR");
    HLib::SetCurrentTaskTo(AnanFinishAuthor);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(ANAN_ANGRY_AUTHOR, 1, 0))
        AnanFilm(32, 255, 0);
    else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanBeginTimeOver(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (ananAngryPlacement[0].character.name == ANAN_INVALID.name) {
        std::uint32_t i = 0;
        do {
            EV_AT_SetPos(ananTimeOverPlacement[i].character, EventTransformFlags::AllAxes, ananTimeOverPlacement[i].position);
            EV_AT_SetAngY(ananTimeOverPlacement[i].character, EventTransformFlags::AllAxes, ananTimeOverPlacement[i].rotation);
            ++i;
        } while (ananAngryPlacement[i].character.name == ANAN_INVALID.name);
    }
    const bool straight = AnanIsStraight(work);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), straight ? 0x67c3 : 0x67c8, MotionRegion::LowerBody);
    mtwkSetLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
    HLib::SetCurrentTaskTo(AnanPlayTimeOverAuthor);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B054", 30, 135, AnanVoiceResumeCaller);
}
void AnanPlayTimeOverAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const bool straight = AnanIsStraight(work);
    work->entry |= straight ? 0x80000000U : 0x40000000U;
    AnanLoadPackage(straight ? "ANANSCC" : "ANANTIOF");
    HLib::SetCurrentTaskTo(straight ? AnanFinishAuthor : AnanFinishTimeOver);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(straight ? ANAN_SUCCESS_AUTHOR : ANAN_TIME_FAIL_AUTHOR, straight ? 1 : 0, straight ? 0 : 1)) {
        AnanFilm(32, 255, 0);
        if (!straight)
            work->lifecycle |= 2;
    } else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFinishTimeOver(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_TIME_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanSuccess(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    AnanUpdatePose(work);
    AnanSetTargets(false);
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    work->entry |= 0x80000000U;
    work->targets[0] = ANAN_INVALID;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage("ANANSCC");
    HLib::SetCurrentTaskTo(AnanFinishAuthor);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(ANAN_SUCCESS_AUTHOR, 1, 0))
        AnanFilm(32, 255, 0);
    else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    AnanUpdatePose(work);
    AnanSetTargets(false);
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    work->targets[0] = ANAN_INVALID;
    const bool fifth = work->failures >= 5;
    if (fifth)
        work->entry |= 0x80000000U;
    else
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage(fifth ? "ANANFIR3" : "ANANFIR1");
    HLib::SetCurrentTaskTo(fifth ? AnanFinishFifthFailure : AnanOfferRetry);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(fifth ? ANAN_FIFTH_AUTHOR : ANAN_FAIL_AUTHOR, 0, 1)) {
        AnanFilm(32, 255, 0);
        work->lifecycle |= 2;
    } else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanOfferRetry(void *data) {
    if (!data) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    const Vector3 position{std::bit_cast<float>(0x3ef9db23U), std::bit_cast<float>(0xbba3d70aU), 2.4089999f};
    EV_AT_SetPos(ANAN_RYO, EventTransformFlags::AllAxes, position);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    const Vector3i rotation{0, 13471, 0};
    EV_AT_SetAngY(ANAN_RYO, EventTransformFlags::AllAxes, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_RYO), 17, 0);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    std::int16_t choices[]{-1, 152, 34, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    AnanSelectCamera(1905);
    HLib::SetCurrentTaskTo(AnanWaitRetry);
}
void AnanWaitRetry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("ANANFIR1"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ANANFIR1"), 0);
    if (shenGetCtrlSpeakIconFlag() != 152) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        AnanLoadPackage("ANANFIR2");
        HLib::SetCurrentTaskTo(AnanFinishFailure);
        EV_NPC_NewLoadSw(1);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (!QueueAnanAuthor(ANAN_FAIL_FIRST_AUTHOR, 0, 1))
            HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 0);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_RYO), 17, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c2, MotionRegion::LowerBody);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), .89999998f, MotionRegion::LowerBody);
    mtwkSetEnabled(scenFindMotionwork(ANAN_MAN), 0);
    ApplyAnanPlacements(ananRetryPlacement);
    HLib::SetCurrentTaskTo(AnanRetry);
    HLib::SetupTaskDelay(-1);
    AnanSelectCamera(1906);
    QueueAnanVoice("0020A017", 0, 100, nullptr);
    QueueAnanVoice("0020B022", 101, 150, AnanVoiceResumeCaller);
}
void AnanFinishFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FAIL_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFinishFifthFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FIFTH_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanRetry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    AnanFilm(32, 255, 0);
    work->lifecycle |= 2;
    HLib::SetCurrentTaskTo(AnanRestartGame);
    HLib::SetupTaskDelay(-1);
    QueueAnanWait();
    AnanStopLift(0, 1);
    AnanSelectCamera(1911);
    QueueAnanVoice("0020B023", 32, 142, AnanVoiceUnlockInput);
    work->gameFlags |= 0x80000000U;
    work->targets[0] = ANAN_MAN;
    work->targets[1] = ANAN_RYO;
    ApplyAnanPlacements(ananGamePlacement);
}
void AnanAskStraight(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (work->gameFlags & 0x80000000U)
        return;
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    HLib::SetCurrentTaskTo(AnanWaitStraight);
    AnanGetHandPosition(work->tilt, &work->positions[0]);
}
void AnanWaitStraight(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    QueueAnanWait();
    work->gameFlags |= 0x80000000U;
    HLib::TaskCallback next;
    if (shenGetCtrlSpeakIconFlag() == 82) {
        work->targets[1] = ANAN_RYO;
        AnanSelectCamera(1716);
        QueueAnanVoice("0020A012", 10, 90, AnanVoiceUnlockInput);
        AnanSelectCamera(1904);
        if (AnanIsStraight(work))
            next = AnanSuccess;
        else {
            ++work->failures;
            next = AnanFailure;
        }
    } else {
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        AnanSelectCamera(1757);
        next = AnanRestartGame;
        QueueAnanVoice("0020B023", 0, 110, AnanVoiceUnlockInput);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
        AnanSetLiftFrame(work->tilt);
        AnanGetHandPosition(work->tilt, &work->positions[1]);
        work->targets[0] = shen_name{};
        work->positionModes[0] = 0;
        work->positionModes[1] = 0;
        work->targets[1] = ANAN_INVALID;
    }
    HLib::SetCurrentTaskTo(next);
    HLib::SetupTaskDelay(-1);
}
} // namespace shenmue::s2::areas
#endif
