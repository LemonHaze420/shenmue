#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qdmr.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QdmrMain(void)
{
    int failed = 1;
    int previousLoad = FTBL.GetNpcNewLoadState();
    HLibTask *task, *timer = 0;
    QdmrWork *work;
    HLibTask **timerParent;
    char *bytes;
    int count, roomBytes;
    if (FTBL.ctrlGetCharaByName(QDMR_REN)) {
        FTBL.PTYCUtils(QDMR_REN, 8, 0);
        FTBL.npcFree(QDMR_REN);
    }
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    for (count = 0; qdmrRooms[count].door; count++)
        ;
    roomBytes = count * 4;
    do {
        task = FTBL.HLib_EnqueueTaskWithParameter(QdmrInitialize, 4, 11, sizeof(QdmrWork) + roomBytes, TASK_EVNT);
        if (task) {
            work = (QdmrWork *)FTBL.HLib_GetTaskParameter(task);
            if (work) {
                bytes = (char *)work;
                for (count = 0; count < (int)sizeof(QdmrWork); count++)
                    bytes[count] = 0;
                timer = FTBL.HLib_EnqueueTaskWithParameter(QdmrTimerCallback, 4, 11, sizeof(HLibTask *), TASK_EVNT);
                if (timer) {
                    timerParent = (HLibTask **)FTBL.HLib_GetTaskParameter(timer);
                    if (timerParent) {
                        bytes = (char *)timerParent;
                        for (count = 0; count < (int)sizeof(HLibTask *); count++)
                            bytes[count] = 0;
                        work->task = task;
                        work->timerTask = timer;
                        work->parent = FTBL.HLib_GetCurrentTask();
                        if (roomBytes) {
                            bytes = (char *)(work + 1);
                            work->roomFlags = (unsigned int *)bytes;
                            for (count = 0; count < roomBytes; count++)
                                bytes[count] = 0;
                        } else {
                            work->roomFlags = 0;
                        }
                        work->timer = 0;
                        work->timerFlags = 0;
                        work->timerFadeFrames = 0;
                        work->timerWarningThreshold = 900;
                        work->timerRequests = 0;
                        work->timerText = (QdmrHudText *)FTBL.CreateLegacyHudText();
                        if (work->timerText) {
                            *timerParent = task;
                            FTBL.spmFadeOut(0);
                            work->flags |= 0x20;
                            FTBL.EV_SetCanDoAreaJumpTo(0);
                            FTBL.EV_EventSetUpHelpIcon_0(16, 0);
                            FTBL.SetPlayerControlEnabled(0);
                            work->flags |= previousLoad ? 0x14 : 4;
                            work->flags |= 1;
                            FTBL.EV_stepSetFlags(446, 0);
                            failed = 0;
                        }
                    }
                }
            }
        }
    } while (0);
    if (failed) {
        if (timer)
            FTBL.HLib_DestroyTask(timer);
        if (task)
            FTBL.HLib_DestroyTask(task);
        if (previousLoad)
            FTBL.EV_NPC_NewLoadSw_0(1);
    }
}
#else
#define SHENMUE_AREA_LOOP_SOUND
#define SHENMUE_AREA_LOOP_MOTION_FRAMES
#define SHENMUE_AREA_LOOP_VOICE_REVISION 3
#include "s2/areas/area_loop_dc.c"

static char qdmrPackage[] = "QDMR";
static char qdmrCutscene[] = "0266A000";
static char qdmrConversation[] = "0266";
static shen_name qdmrYuanAuthor = SHEN_NAME4('E', 'N', 'D', '_');
static char qdmrYuanPackage[] = "QES3END";
static char qdmrYuanConversation[] = "0364";
static shen_name qdmrInitialAuthor = SHEN_NAME4('I', 'N', 'I', 'T');
static char qdmrInitialPackage[] = "QDMRINIT";
static shen_name qdmrInitialLight = SHEN_NAME4('I', 'N', 'I', 'T');
static Vector3 qdmrInitialLightOffset = {0, 64.8f, 0};
static shen_name qdmrTimeoutAuthor = SHEN_NAME4('T', 'O', 'V', 'R');
static char qdmrTimeoutPackage[] = "QDMRTOVR";
static char qdmrTransitionPackage[] = "QDMRTRNS";
static shen_name qdmrBingoOpeningAuthor = SHEN_NAME4('B', 'B', 'G', 'N');
static shen_name qdmrBingoWinAuthor = SHEN_NAME4('B', 'Q', 'I', 'T');
static char qdmrBingoOpeningPackage[] = "QDMRBGNB";
static char qdmrBingoWinPackage[] = "QDMRQITB";
static shen_name qdmrYuanNpcs[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('A', 'D', '1', '_'),
                                   SHEN_NAME4('K', 'T', '2', '_'), SHEN_NAME4('K', 'T', 'A', '_'), SHEN_NAME4('a', '4', '5', '2'), 0};
static char qdmrYuanCutscene[] = "0364A000";
static shen_name qdmrNpcs[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('P', '0', '2', '_'), SHEN_NAME4('P', '0', '4', '_'),
                               SHEN_NAME4('P', '0', '5', '_'), SHEN_NAME4('P', '0', '7', '_'), SHEN_NAME4('P', '0', '8', '_'),
                               SHEN_NAME4('P', '0', '9', '_'), 0};
static shen_name qdmrBingoNpcs[] = {SHEN_NAME4('K', 'F', '4', '_'), SHEN_NAME4('P', '0', '2', '_'), SHEN_NAME4('P', '0', '7', '_'),
                                    SHEN_NAME4('P', '0', '9', '_'), 0};
static int qdmrBingoStyles[] = {38, 3, 2, 2};
static int qdmrBingoHealth[] = {200, 80, 80, 80};
QdmrRoom qdmrRooms[] = {
    {SHEN_NAME4('R', '1', '0', '1'), SHEN_NAME4('R', '1', '0', '1')}, {SHEN_NAME4('R', '1', '0', '2'), SHEN_NAME4('R', '1', '0', '2')},
    {SHEN_NAME4('R', '1', '0', '3'), SHEN_NAME4('R', '1', '0', '3')}, {SHEN_NAME4('R', '1', '0', '4'), SHEN_NAME4('Q', 'F', '1', 'B')},
    {SHEN_NAME4('R', '1', '0', '5'), SHEN_NAME4('R', '1', '0', '5')}, {SHEN_NAME4('R', '1', '0', '6'), SHEN_NAME4('R', '1', '0', '6')},
    {SHEN_NAME4('R', '1', '0', '7'), SHEN_NAME4('R', '1', '0', '7')}, {SHEN_NAME4('R', '1', '0', '8'), SHEN_NAME4('R', '1', '0', '8')},
    {SHEN_NAME4('R', '1', '0', '9'), SHEN_NAME4('R', '1', '0', '9')}, {SHEN_NAME4('R', '1', '1', '0'), SHEN_NAME4('R', '1', '1', '0')},
    {SHEN_NAME4('R', '1', '1', '1'), SHEN_NAME4('R', '1', '1', '1')}, {SHEN_NAME4('R', '1', '1', '2'), SHEN_NAME4('Q', 'F', '2', 'B')},
    {SHEN_NAME4('R', '1', '1', '3'), SHEN_NAME4('R', '1', '1', '3')}, {SHEN_NAME4('R', '1', '1', '4'), SHEN_NAME4('R', '1', '1', '4')},
    {SHEN_NAME4('R', '1', '1', '5'), SHEN_NAME4('R', '1', '1', '5')}, {SHEN_NAME4('R', '1', '1', '6'), SHEN_NAME4('R', '1', '1', '6')},
    {SHEN_NAME4('R', '1', '1', '7'), SHEN_NAME4('R', '1', '1', '7')}, {SHEN_NAME4('R', '1', '1', '8'), SHEN_NAME4('R', '1', '1', '8')},
    {SHEN_NAME4('R', '1', '1', '9'), SHEN_NAME4('R', '1', '1', '9')}, {SHEN_NAME4('R', '1', '2', '0'), SHEN_NAME4('Q', 'F', '3', 'B')},
    {SHEN_NAME4('R', '1', '2', '1'), SHEN_NAME4('R', '1', '2', '1')}, {SHEN_NAME4('R', '1', '2', '2'), SHEN_NAME4('R', '1', '2', '2')},
    {SHEN_NAME4('R', '1', '2', '3'), SHEN_NAME4('R', '1', '2', '3')}, {SHEN_NAME4('R', '1', '2', '4'), SHEN_NAME4('Q', 'F', 'I', 'X')},
    {0, 0}};
QdmrDoorPlacement qdmrDoorPlacements[] = {
    {SHEN_NAME4('R', '1', '0', '1'), 200.71f, 152.55f, 49152},
    {SHEN_NAME4('R', '1', '0', '2'), 200.71f, 157.95f, 49152},
    {SHEN_NAME4('R', '1', '0', '3'), 200.71f, 163.35f, 49152},
    {SHEN_NAME4('R', '1', '0', '4'), 200.71f, 168.75f, 49152},
    {SHEN_NAME4('R', '1', '0', '5'), 200.71f, 174.15f, 49152},
    {SHEN_NAME4('R', '1', '0', '6'), 218.7f, 166.05f, 16384},
    {SHEN_NAME4('R', '1', '0', '7'), 218.7f, 160.65f, 16384},
    {SHEN_NAME4('R', '1', '0', '8'), 218.7f, 155.25f, 16384},
    {SHEN_NAME4('R', '1', '0', '9'), 230.85f, 149.4f, 32768},
    {SHEN_NAME4('R', '1', '1', '0'), 241.65f, 149.4f, 32768},
    {SHEN_NAME4('R', '1', '1', '1'), 251.11f, 152.55f, 49152},
    {SHEN_NAME4('R', '1', '1', '2'), 251.11f, 157.95f, 49152},
    {SHEN_NAME4('R', '1', '1', '3'), 251.11f, 163.35f, 49152},
    {SHEN_NAME4('R', '1', '1', '4'), 244.35f, 169.2f, 32768},
    {SHEN_NAME4('R', '1', '1', '5'), 255.15f, 169.2f, 32768},
    {SHEN_NAME4('R', '1', '1', '6'), 215.1f, 142.65f, 16384},
    {SHEN_NAME4('R', '1', '1', '7'), 215.1f, 137.25f, 16384},
    {SHEN_NAME4('R', '1', '1', '8'), 215.1f, 131.85f, 16384},
    {SHEN_NAME4('R', '1', '1', '9'), 215.09f, 126.45f, 16384},
    {SHEN_NAME4('R', '1', '2', '0'), 218.25f, 124.2f, 0},
    {SHEN_NAME4('R', '1', '2', '1'), 223.65f, 124.2f, 0},
    {SHEN_NAME4('R', '1', '2', '2'), 229.05f, 124.2f, 0},
    {SHEN_NAME4('R', '1', '2', '3'), 234.45f, 124.2f, 0},
    {SHEN_NAME4('R', '1', '2', '4'), 249.31f, 133.65f, 49152},
    {0, 0, 0, 0}};
QdmrDoorRoute qdmrDoorRoutes[] = {
    {SHEN_NAME4('R', '1', '0', '1'), 0},
    {SHEN_NAME4('R', '1', '0', '2'), 0},
    {SHEN_NAME4('R', '1', '0', '3'), 0},
    {SHEN_NAME4('R', '1', '0', '4'), 1},
    {SHEN_NAME4('R', '1', '0', '5'), 0},
    {SHEN_NAME4('R', '1', '0', '6'), 0},
    {SHEN_NAME4('R', '1', '0', '7'), 0},
    {SHEN_NAME4('R', '1', '0', '8'), 0},
    {SHEN_NAME4('R', '1', '0', '9'), 0},
    {SHEN_NAME4('R', '1', '1', '0'), 0},
    {SHEN_NAME4('R', '1', '1', '1'), 0},
    {SHEN_NAME4('R', '1', '1', '2'), 2},
    {SHEN_NAME4('R', '1', '1', '3'), 0},
    {SHEN_NAME4('R', '1', '1', '4'), 0},
    {SHEN_NAME4('R', '1', '1', '5'), 0},
    {SHEN_NAME4('R', '1', '1', '6'), 0},
    {SHEN_NAME4('R', '1', '1', '7'), 0},
    {SHEN_NAME4('R', '1', '1', '8'), 0},
    {SHEN_NAME4('R', '1', '1', '9'), 0},
    {SHEN_NAME4('R', '1', '2', '0'), 3},
    {SHEN_NAME4('R', '1', '2', '1'), 0},
    {SHEN_NAME4('R', '1', '2', '2'), 0},
    {SHEN_NAME4('R', '1', '2', '3'), 0},
    {SHEN_NAME4('R', '1', '2', '4'), 7},
    {0, 0}};

void QdmrDoneCallback(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int wait = 1;
    int index;
    do {
        if (!work) {
            wait = 0;
            break;
        }
        for (index = 0; qdmrRooms[index].door; index++)
            FTBL.ReleaseDoor(qdmrRooms[index].door, QDMR_PLAYER);
        if (work->timerFlags & 0x80000000) {
            work->timerRequests |= 0x80000000;
            break;
        }
        if (!(work->flags & 0x20)) {
            FTBL.spmFadeOut(0);
            work->flags |= 0x20;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        }
        if (work->flags & 2) {
            SetQdmrPlayerHandState(QDMR_PLAYER, 0);
            work->flags &= ~2;
        }
        if (work->timerText) {
            work->timerText->release(work->timerText);
            work->timerText = 0;
        }
        FTBL.EV_NPC_NewLoadSw_0(1);
        wait = 0;
    } while (0);
    if (wait)
        return;
    FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, qdmrPackage, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, qdmrPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("QFIX");
    FTBL.EV_LoadAudio("QFIX", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_SetCanDoAreaJumpTo(1);
    FTBL.FinishLoading();
    FTBL.SetPlayerControlEnabled(1);
    FTBL.camCtrlUtil(15, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.HLib_MarkCurrentTaskForCleanup();
}

void QdmrInitialize(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(QdmrInitialize2);
        FTBL.HLib_SetupTimedTask(work->task, -1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrInitialize2(void *data)
{
    if (!data) {
        FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
    } else {
        FTBL.flddSetPointFlagOnGrid(QDMR_QFBA, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrStartInitialAuthor);
    }
}

void QdmrStartYuanAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        for (index = 0; qdmrRooms[index].door; index++) {
            work->roomFlags[index] &= ~2;
            if (work->roomFlags[index] & 1) {
                FTBL.ReleaseDoor(qdmrRooms[index].door, QDMR_PLAYER);
                work->roomFlags[index] &= ~1;
            }
        }
        if (!FTBL.qronSetFloor(17))
            break;
        FTBL.qronRepositionPlayerForFloor(17);
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_FreeAuthorPak(1, qdmrYuanConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitYuanNpcs);
        FTBL.HLib_SetupTimedTask(work->task, -1);
        QueueAreaLoopNpcs(qdmrYuanNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitYuanNpcs(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    FTBL.HLib_SetCurrentTaskTo(QdmrEndYuanAuthor);
    FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1;
    if (work) {
        FTBL.HLib_SetupTimedTask(work->task, -1);
        work->flags |= 1;
        QueueAreaLoopAuthor(qdmrYuanPackage, qdmrYuanAuthor, 0, -1, QdmrYuanAuthorCompletion);
    }
}

int QdmrYuanAuthorCompletion(int phase, int value, HLibTask *parent)
{
    QdmrWork *work;
    if (phase == 0) {
        FTBL.EV_ControlPackageResources(2, 2, qdmrYuanPackage, 0, 0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrYuanCutscene);
        FTBL.EV_LoadAudio(qdmrYuanCutscene, 0);
        FTBL.EV_spmBeginSampleExecute(0);
        work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
        if (work)
            work->flags &= ~0x20;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void QdmrEndYuanAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        RestoreAreaLoopCharacters(qdmrYuanNpcs, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrYuanConversation);
        FTBL.spmFadeOut(0);
        work->flags |= 0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrExitYuanAuthor);
        break;
    }
}

void QdmrExitYuanAuthor(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (!FTBL.qronSetFloor(18))
            break;
        FTBL.qronRepositionPlayerForFloor(18);
        FTBL.HLib_SetCurrentTaskTo(QdmrStartInitialAuthor);
        break;
    }
}

void QdmrStartInitialAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (!FTBL.qronSetFloor(18))
            break;
        FTBL.qronRepositionPlayerForFloor(18);
        FTBL.EV_FreeAuthorPak(1, qdmrConversation);
        FTBL.EV_ControlPackageResources(2, 2, qdmrPackage, 0, 0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndInitialAuthor);
        FTBL.HLib_SetupTimedTask(work->task, -1);
        work->flags |= 1;
        QueueAreaLoopAuthor(qdmrInitialPackage, qdmrInitialAuthor, 0, -1, QdmrInitialAuthorCompletion);
        break;
    }
}

int QdmrInitialAuthorCompletion(int phase, int value, HLibTask *parent)
{
    if (phase == 0) {
        Vector3 offset;
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.qronSetFloor(18);
        offset.x = 0;
        offset.y = FTBL.GetQronTemporaryFloorHeight() - 100.0f;
        offset.z = 0;
        FTBL.SetLightRootPosition(&offset);
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_lightLoad(qdmrInitialLight);
        FTBL.SetLightRootPosition(&qdmrInitialLightOffset);
        FTBL.PushGlobalLightingProgram();
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void QdmrEndInitialAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.spmFadeOut(0);
            work->flags |= 0x20;
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        FTBL.EV_FreeAuthorPak(0, qdmrConversation);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrExitInitialAuthor);
        break;
    }
}

void QdmrExitInitialAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 position;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (!FTBL.qronSetFloor(18))
            break;
        FTBL.qronRepositionPlayerForFloor(18);
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_LoadAudio(qdmrCutscene, 0);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
        work->timer = 1800;
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        position.x = 218.76f;
        position.z = 175.24f;
        rotation.x = 0;
        rotation.y = 10922;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(QDMR_PLAYER), 17, 0);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        break;
    }
}

void QdmrStartTimeoutAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (FTBL.srchHasActiveLockonTarget())
            break;
        FTBL.SetPlayerControlEnabled(0);
        work->flags |= 4;
        if (FTBL.drGetCurrentAuthorId()) {
            if (!FTBL.LeaveDoorSubscene(0))
                break;
            FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
            FTBL.EV_NPC_NewLoadSw_0(0);
        }
        FTBL.EV_FreeAuthorPak(1, qdmrConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitTimeoutNpcs);
        FTBL.HLib_SetupTimedTask(work->task, -1);
        QueueAreaLoopNpcs(qdmrNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitTimeoutNpcs(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        work->flags &= ~0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndTimeoutAuthor);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        if (work) {
            FTBL.HLib_SetupTimedTask(work->task, -1);
            work->flags |= 1;
            QueueAreaLoopAuthor(qdmrTimeoutPackage, qdmrTimeoutAuthor, 0, -1, QdmrStoryAuthorCompletion);
        }
        break;
    }
}

int QdmrStoryAuthorCompletion(int phase, int value, HLibTask *parent)
{
    switch (phase) {
    case 0:
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_SetPlayerFlag(12, 2097, 1);
        break;
    case 1:
        FTBL.EV_SetPlayerFlag(12, 2097, 0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        break;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void QdmrEndTimeoutAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        RestoreAreaLoopCharacters(qdmrNpcs, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrConversation);
        FTBL.spmFadeOut(0);
        work->flags |= 0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrExitTimeoutAuthor);
        FTBL.Queue_ST_LoadTitleFadeIn(QDMR_TIMEOUT_TITLE);
        FTBL.HLib_SetupTaskDelay(100);
        break;
    }
}

void QdmrExitTimeoutAuthor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(QdmrStartYuanAuthor);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        FTBL.HLib_SetupTaskDelay(30);
        FTBL.ST_LoadTitleFade();
        break;
    }
}

void QdmrStartTransition(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.EV_FreeAuthorPak(1, qdmrConversation);
        FTBL.SetPlayerControlEnabled(0);
        work->flags |= 4;
        FTBL.HLib_SetCurrentTaskTo(QdmrMoveToTransition);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrTransitionPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrMoveToTransition(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    Vector3 position;
    if (work) {
        FTBL.SetPlayerControlEnabled(0);
        work->flags |= 4;
        FTBL.GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, 0x40000000);
        FindClearRotationPathForAngY(&rotation.y);
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        QueueAreaLoopMotion(QDMR_PLAYER, 0x1c45, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 22);
        QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &position, &rotation, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrEndTransition);
        FTBL.HLib_SetupTaskDelay(-1);
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 1, 0, 0.7f);
        work->flags |= 2;
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, (void *)areaLoopHandPoses[40], 1);
        QueueAreaLoopCamera(qdmrTransitionPackage, 2000, QDMR_DUMMY, 0, 0);
        QueueAreaLoopCamera(qdmrTransitionPackage, 2001, QDMR_DUMMY, 220, 0);
        QueueAreaLoopCamera(qdmrTransitionPackage, 2002, QDMR_DUMMY, 540, 0);
        FTBL.SetScenarioCharacterAnimationFrame(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, FTBL.ctrlGetCharaByName(QDMR_TRANSITION_TARGET));
        position.x = 0.03f;
        position.y = -0.05f;
        position.z = -0.04f;
        rotation.x = 0xfd95;
        rotation.y = 0x8000;
        rotation.z = 0xdfdb;
        QueueAreaLoopAttachment(QDMR_PLAYER, 14, QDMR_TRANSITION_TARGET, 0, 540, &position, &rotation, 0);
        QueueAreaLoopLookAt(QDMR_PLAYER, QDMR_TRANSITION_TARGET, -1, 0, 0, 539, 0);
        QueueAreaLoopVoice("0266C001", 1, 0, -1, 0);
        QueueAreaLoopVoice("0266C004", 1, 100, -1, 0);
        QueueAreaLoopVoice("0266C002", 1, 220, -1, 0);
        QueueAreaLoopVoice("0266C003", 1, 381, -1, 0);
        QueueAreaLoopVoice("0266A012", 1, 541, -1, ResolveAreaLoopParent);
    } else {
        FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
    }
}

void QdmrEndTransition(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    HLibTaskCallback next = 0;
    for (;;) {
        if (!work) {
            next = QdmrDoneCallback;
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.spmFadeOut(0);
            work->flags |= 1;
            break;
        }
        FTBL.SetScenarioCharacterAnimationFrame(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        work->flags &= ~2;
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        FTBL.EV_ControlPackageResources(0, 6, qdmrTransitionPackage, 0, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrConversation);
        work->timerRequests |= 0x40000000;
        work->timer = 10800;
        next = QdmrInFree;
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        break;
    }
    if (next)
        FTBL.HLib_SetCurrentTaskTo(next);
}

void QdmrInFree(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 position;
    float x, z;
    int index, mode;
    shen_name door;
    unsigned short buttons;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work->flags & 1) {
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
            break;
        }
        for (index = 0; qdmrRooms[index].door; index++)
            if (!(work->roomFlags[index] & 1) && FTBL.ClaimDoor(qdmrRooms[index].door, QDMR_PLAYER))
                work->roomFlags[index] |= 1;
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        x = position.x - qdmrLadderPosition.x;
        z = position.z - qdmrLadderPosition.z;
        if (__builtin_fabs(x) < 1.1920929e-7f)
            x = 0;
        if (__builtin_fabs(z) < 1.1920929e-7f)
            z = 0;
        x *= x;
        z *= z;
        if (__builtin_fabs(x) < 1.1920929e-7f)
            x = 0;
        if (__builtin_fabs(z) < 1.1920929e-7f)
            z = 0;
        x += z;
        if (__builtin_fabs(x) < 1.1920929e-7f)
            x = 0;
        if (x < 4.0f) {
            FTBL.HLib_SetCurrentTaskTo(QdmrEscapeToLadder);
            FTBL.SetPlayerControlEnabled(0);
            work->flags |= 4;
            break;
        }
        work->flags |= 8;
        if (work->flags & 4) {
            FTBL.FinishLoading();
            FTBL.SetPlayerControlEnabled(1);
            work->flags &= ~4;
            break;
        }
        FTBL.SetPlayerGameFlags(6, 1);
        mode = work->timerFlags & 3;
        if (mode) {
            if (FTBL.GetSelectedSoundSceneIndex() != 2) {
                FTBL.EV_LoadAudio(qdmrCutscene, 2);
                FTBL.EV_spmBeginSampleExecute(0);
            }
        } else if (FTBL.GetSelectedSoundSceneIndex() != 0) {
            FTBL.EV_LoadAudio(qdmrCutscene, 0);
            FTBL.EV_spmBeginSampleExecute(0);
        }
        if (!mode) {
            x = position.x - 217.05f;
            z = position.z - 126.56f;
            if (__builtin_fabs(x) < 1.1920929e-7f)
                x = 0;
            if (__builtin_fabs(z) < 1.1920929e-7f)
                z = 0;
            x *= x;
            z *= z;
            if (__builtin_fabs(x) < 1.1920929e-7f)
                x = 0;
            if (__builtin_fabs(z) < 1.1920929e-7f)
                z = 0;
            x += z;
            if (__builtin_fabs(x) < 1.1920929e-7f)
                x = 0;
            if (x < 4.0f)
                work->timer = 0;
        }
        if (work->timer <= 0) {
            if (mode) {
                work->timerRequests |= 0x40000000;
                QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, 1, 0.0f, -1.0f, 1.0f, 16.0f, 0, 0);
                FTBL.SetPlayerControlEnabled(0);
                work->flags |= 4;
                FTBL.HLib_SetCurrentTaskTo(QdmrStartTimeoutAuthor);
                if (FTBL.srchHasActiveLockonTarget())
                    FTBL.srchRequestLockonExit();
            } else {
                FTBL.HLib_SetCurrentTaskTo(QdmrStartTransition);
            }
            break;
        }
        door = FindNearestQdmrDoor(work->roomFlags);
        buttons = 0;
        if (door) {
            if (FTBL.GetBattleDataConfig(1, QDMR_BTKK) && FTBL.GetBattleDataConfig(2, QDMR_BTKK))
                buttons = 1024;
            else
                FTBL.GetBattleDataConfig(0, QDMR_BTKK);
            if (FTBL.GetBattleDataConfig(1, QDMR_BTPP) && FTBL.GetBattleDataConfig(2, QDMR_BTPP))
                buttons |= 4;
            else
                FTBL.GetBattleDataConfig(0, QDMR_BTPP);
            if (FTBL.GetBattleDataConfig(1, QDMR_BTAA) && FTBL.GetBattleDataConfig(2, QDMR_BTAA))
                buttons |= 2;
            else
                FTBL.GetBattleDataConfig(0, QDMR_BTAA);
            if (buttons) {
                work->doorQteButtons = buttons;
                do {
                    FTBL.SetPlayerControlEnabled(0);
                    work->flags |= 4;
                    QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 8.0f, 0, 0);
                    FTBL.SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
                    do {
                        if (GetQdmrDoorVariant(door, work->roomFlags))
                            FTBL.spmFadeOut(0);
                    } while (0);
                    work->flags |= 1;
                    work->door = door;
                    FTBL.HLib_SetCurrentTaskTo(QdmrStartOpenDoor);
                } while (0);
            }
        } else {
            work->doorQteButtons = 0;
        }
        break;
    }
}

void QdmrStartOpenDoor(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        }
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &work->savedPosition, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, 0x40000000);
        work->savedYaw = rotation.y;
        GetQdmrDoorVariant(work->door, work->roomFlags);
        switch (GetQdmrDoorVariant(work->door, work->roomFlags)) {
        case 0:
        default:
            QdmrOpenNormalRoom(work);
            break;
        case 1:
            QdmrOpenFirstRoom(work);
            break;
        case 2:
            QdmrOpenSecondRoom(work);
            break;
        case 3:
            QdmrOpenThirdRoom(work);
            break;
        case 4:
            QdmrRepeatFirstRoom(work);
            break;
        case 5:
            QdmrRepeatSecondRoom(work);
            break;
        case 6:
            QdmrRepeatThirdRoom(work);
            break;
        case 7:
            MarkQdmrRoomVisited(work->door, work->roomFlags);
            FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
            work->timerRequests |= 0x40000000;
            QdmrOpenBingoRoom(work);
            work->battleRetries = 0;
            break;
        }
        break;
    }
}

void QdmrOpenNormalRoom(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        FTBL.EV_FreeAuthorPak(1, qdmrNormalConversation);
        FTBL.GetMagicRoomDebugState();
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitNormalRoom);
        break;
    }
}

void QdmrWaitNormalRoom(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (!FTBL.GetMagicRoomState3())
            break;
        FTBL.HLib_SetCurrentTaskTo(QdmrNormalRoomPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrNormalPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrNormalRoomPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin, position;
    Vector3i rotation;
    shen_name door;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndNormalRoom);
        FTBL.HLib_SetupTaskDelay(-1);
        door = work->door;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A008", 1, 40, -1, 0);
            break;
        case 1:
            QueueAreaLoopVoice("0266A002", 1, 40, -1, 0);
            break;
        case 2:
            QueueAreaLoopVoice("0266A003", 1, 40, -1, 0);
            break;
        default:
            QueueAreaLoopVoice("0266A005", 1, 40, -1, 0);
            break;
        }
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0266A004", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        default:
            QueueAreaLoopVoice("0266A001", 1, 100, -1, ResolveAreaLoopParent);
            break;
        }
        if (work->doorQteButtons & 1024) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 80, 0, &position, &rotation, 0);
            position.x = 1.4f;
            position.y = 0;
            position.z = 0.8f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            rotation.y += 16384;
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7233, 0, 70, 35.0f, -1.0f, 0.5f, 0.0f, 0, 6);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 71, 10, 140.0f, -1.0f, 0.8f, 16.0f, 0, 4);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 80, 110, 148.0f, -1.0f, 0.8f, 0.0f, 0, 6);
            QueueAreaLoopCamera(qdmrNormalPackage, 3100, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3101, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3102, QDMR_DUMMY, 80, 0);
            QueueQdmrDoorMotion(door, 0, 7234, 0.0f, 1.6f);
            QueueQdmrDoorMotion(door, 15, 7234, 15.0f, 0.0f);
        } else if (work->doorQteButtons & 4) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 0, 70, 40.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 30);
            QueueAreaLoopCamera(qdmrNormalPackage, 3200, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3201, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3202, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 30, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 45, 7234, 15.0f, 0.0f);
        } else {
            position.x = -0.1f;
            position.y = 0;
            position.z = 1.5f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7239, 0, 70, 70.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 13);
            QueueAreaLoopCamera(qdmrNormalPackage, 3000, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3001, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3002, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 12, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 27, 7234, 15.0f, 0.0f);
        }
        work->doorQteButtons = 0;
        break;
    }
}

void QdmrEndNormalRoom(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            QueueAreaLoopSound(2141, 30);
            QueueAreaLoopSound(2142, 50);
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrNormalConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetMagicRoomDebugState();
        FTBL.doorLoopRigidMotion(work->door, 7234, -1.0f, 1.0f);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.EV_ControlPackageResources(0, 5, qdmrNormalPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qdmrNormalPackage, 0, 0);
        rotation.x = 0;
        rotation.y = work->savedYaw;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &work->savedPosition);
        FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
        FTBL.HLib_SetupTaskDelay(10);
        break;
    }
}

void QdmrOpenFirstRoom(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        work->flags |= 0x20;
        FTBL.EV_FreeAuthorPak(1, qdmrFirstConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitFirstRoomPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrFirstPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitFirstRoomPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrFirstRoomPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrFirstNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrFirstRoomPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 base, origin, position;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(QdmrBeginFirstRoomFight);
        FTBL.HLib_SetupTaskDelay(-1);
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.5f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.7f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 260, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, 259, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 260, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 350, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueQdmrDoorMotion(work->door, 0, 7234, 0.0f, 1.0f);
        QueueQdmrDoorMotion(work->door, 15, 7234, 15.0f, 0.0f);
        QueueQdmrDoorMotion(work->door, 351, 7234, -1.0f, 1.0f);
        QueueAreaLoopSound(2331, 0);
        QueueAreaLoopCamera(qdmrFirstPackage, 4000, QDMR_DUMMY, 0, 0);
        QueueAreaLoopCamera(qdmrFirstPackage, 4001, QDMR_DUMMY, 150, 0);
        QueueAreaLoopCamera(qdmrFirstPackage, 4002, QDMR_DUMMY, 260, 0);
        QueueAreaLoopCamera(qdmrFirstPackage, 4003, QDMR_DUMMY, 350, 0);
        QueueAreaLoopVoice("0266E001", 1, 150, -1, 0);
        QueueAreaLoopVoice("0266A027", 1, 260, -1, 0);
        QueueAreaLoopVoice("0266E002", 1, 350, -1, 0);
        QueueAreaLoopVoice("0266E003", 1, 510, -1, ResolveAreaLoopParent);
        QueueQdmrFadeOutSound(510);
        work->flags |= 0x20;
        position.x = 205.36f;
        position.z = 166.8f;
        rotation.y = 0xa0fe;
        QueueAreaLoopPosition(QDMR_SBK, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_SBK, 7241, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        position.x = 204.94f;
        position.z = 168.56f;
        rotation.y = 0xe3e9;
        QueueAreaLoopPosition(QDMR_ANC, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_ANC, 7235, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        break;
    }
}

void QdmrBeginFirstRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.EV_LoadAudio(qdmrCutscene, 3);
        FTBL.EV_spmBeginSampleExecute(0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrFirstConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        SetQdmrRoomProps(1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF1B, 0);
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, qdmrFirstPackage, 1);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, 0, 0, work->battleRetries * 100 + 200, 0);
        for (index = 0; qdmrFirstNpcs[index]; index++) {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qdmrFirstNpcs[index]), 0, 1, 1, qdmrFirstHealth[index], qdmrFirstStyles[index]);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrFirstNpcs[index]), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrFirstNpcs[index]), 0, 0x200000, 2048);
        }
        FTBL.vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        FTBL.HLib_SetCurrentTaskTo(QdmrFirstRoomFight);
        break;
    }
}

void QdmrFirstRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin;
    Vector3i rotation;
    Vector3 position;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.IsFightActive())
            break;
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
                QueueAreaLoopSound(2141, 30);
                QueueAreaLoopSound(2142, 50);
            }
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.spmFadeOut(0);
            work->flags |= 1;
            break;
        }
        SetQdmrRoomProps(0);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF1B, 1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFB1, 0);
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrFirstNpcs, 0);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrFirstPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrFirstPackage, 0, 0);
            rotation.x = 0;
            rotation.z = 0;
            GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
            position.x = 0.5f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &position);
            rotation.y += 0x8000;
            FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
            FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
            FTBL.camCtrlUtil(15, 0);
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
        } else {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrFirstPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrFirstPackage, 0, 0);
            work->battleRetries++;
            FTBL.HLib_SetCurrentTaskTo(QdmrBeginRetry);
        }
        break;
    }
}

void QdmrOpenSecondRoom(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        work->flags |= 0x20;
        FTBL.EV_FreeAuthorPak(1, qdmrSecondConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitSecondRoomPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrSecondPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitSecondRoomPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrSecondRoomPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrSecondNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrSecondRoomPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 base, origin, position;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(QdmrBeginSecondRoomFight);
        FTBL.HLib_SetupTaskDelay(-1);
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.5f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.7f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 260, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, 259, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 260, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 350, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueQdmrDoorMotion(work->door, 0, 7234, 0.0f, 1.0f);
        QueueQdmrDoorMotion(work->door, 15, 7234, 15.0f, 0.0f);
        QueueQdmrDoorMotion(work->door, 351, 7234, -1.0f, 1.0f);
        QueueAreaLoopSound(2331, 0);
        QueueAreaLoopCamera(qdmrSecondPackage, 5000, QDMR_DUMMY, 0, 0);
        QueueAreaLoopCamera(qdmrSecondPackage, 5001, QDMR_DUMMY, 150, 0);
        QueueAreaLoopCamera(qdmrSecondPackage, 5002, QDMR_DUMMY, 260, 0);
        QueueAreaLoopCamera(qdmrSecondPackage, 5003, QDMR_DUMMY, 350, 0);
        QueueAreaLoopVoice("0266F001", 1, 150, -1, 0);
        QueueAreaLoopVoice("0266A029", 1, 260, -1, 0);
        QueueAreaLoopVoice("0266F002", 1, 350, -1, 0);
        QueueAreaLoopVoice("0266F003", 1, 510, -1, ResolveAreaLoopParent);
        QueueQdmrFadeOutSound(510);
        work->flags |= 0x20;
        position.x = 254.8f;
        position.z = 156.1f;
        rotation.y = 0xa000;
        QueueAreaLoopPosition(QDMR_KU8, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KU8, 7241, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        position.x = 255.3f;
        position.z = 157.7f;
        rotation.y = 0xeaaa;
        QueueAreaLoopPosition(QDMR_KHO, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KHO, 7235, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        break;
    }
}

void QdmrBeginSecondRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.EV_LoadAudio(qdmrCutscene, 3);
        FTBL.EV_spmBeginSampleExecute(0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrSecondConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        SetQdmrRoomProps(1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF2B, 0);
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, qdmrSecondPackage, 1);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, 0, 0, work->battleRetries * 100 + 200, 0);
        for (index = 0; qdmrSecondNpcs[index]; index++) {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qdmrSecondNpcs[index]), 0, 1, 1, qdmrSecondHealth[index], qdmrSecondStyles[index]);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrSecondNpcs[index]), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrSecondNpcs[index]), 0, 0x200000, 2048);
        }
        FTBL.vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        FTBL.HLib_SetCurrentTaskTo(QdmrSecondRoomFight);
        break;
    }
}

void QdmrSecondRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin;
    Vector3i rotation;
    Vector3 position;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.IsFightActive())
            break;
        if (!(work->flags & 1)) {
            if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
                QueueAreaLoopSound(2141, 30);
                QueueAreaLoopSound(2142, 50);
                FTBL.spmFadeOut(0);
                work->flags |= 0x20;
            }
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        SetQdmrRoomProps(0);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF2B, 1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFB2, 0);
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrSecondNpcs, 0);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrSecondPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrSecondPackage, 0, 0);
            rotation.x = 0;
            rotation.z = 0;
            GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
            position.x = 0.5f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &position);
            rotation.y += 0x8000;
            FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
            FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
            FTBL.camCtrlUtil(15, 0);
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
        } else {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrSecondPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrSecondPackage, 0, 0);
            work->battleRetries++;
            FTBL.HLib_SetCurrentTaskTo(QdmrBeginRetry);
        }
        break;
    }
}

void QdmrOpenThirdRoom(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        work->flags |= 0x20;
        FTBL.EV_FreeAuthorPak(1, qdmrThirdConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitThirdRoomPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrThirdPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitThirdRoomPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->flags &= ~0x20;
        FTBL.HLib_SetCurrentTaskTo(QdmrThirdRoomPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrThirdNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrThirdRoomPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 base, origin, position;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(QdmrBeginThirdRoomFight);
        FTBL.HLib_SetupTaskDelay(-1);
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.5f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
        base = origin;
        position.x = 0;
        position.y = 0;
        position.z = -0.7f;
        FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &position);
        QueueAreaLoopPosition(QDMR_PLAYER, 260, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, 259, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 260, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueAreaLoopMotion(QDMR_PLAYER, 7240, 350, 89, -1.0f, -1.0f, 8.0f, 0.0f, 0, 6);
        QueueQdmrDoorMotion(work->door, 0, 7234, 0.0f, 1.0f);
        QueueQdmrDoorMotion(work->door, 15, 7234, 15.0f, 0.0f);
        QueueQdmrDoorMotion(work->door, 351, 7234, -1.0f, 1.0f);
        QueueAreaLoopSound(2331, 90);
        QueueAreaLoopCamera(qdmrThirdPackage, 6000, QDMR_DUMMY, 0, 0);
        QueueAreaLoopCamera(qdmrThirdPackage, 6001, QDMR_DUMMY, 150, 0);
        QueueAreaLoopCamera(qdmrThirdPackage, 6002, QDMR_DUMMY, 260, 0);
        QueueAreaLoopCamera(qdmrThirdPackage, 6003, QDMR_DUMMY, 350, 0);
        QueueAreaLoopVoice("0266G001", 1, 150, -1, 0);
        QueueAreaLoopVoice("0266A027", 1, 260, -1, 0);
        QueueAreaLoopVoice("0266G002", 1, 350, -1, 0);
        QueueAreaLoopVoice("0266G003", 1, 430, -1, 0);
        QueueAreaLoopVoice("0266G004", 1, 510, -1, ResolveAreaLoopParent);
        QueueQdmrFadeOutSound(560);
        work->flags |= 0x20;
        position.x = 219.02f;
        position.z = 119.31f;
        rotation.y = 12797;
        QueueAreaLoopPosition(QDMR_UYD, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_UYD, 7236, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        position.x = 216.75f;
        position.z = 119.36f;
        rotation.y = 0xcd82;
        QueueAreaLoopPosition(QDMR_KT7, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KT7, 7242, 0, 600, -1.0f, -1.0f, 1.0f, 0.0f, 0, 6);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        break;
    }
}

void QdmrBeginThirdRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.EV_LoadAudio(qdmrCutscene, 3);
        FTBL.EV_spmBeginSampleExecute(0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrThirdConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        SetQdmrRoomProps(1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF3B, 0);
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, qdmrThirdPackage, 1);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, 0, 0, work->battleRetries * 100 + 200, 0);
        for (index = 0; qdmrThirdNpcs[index]; index++) {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qdmrThirdNpcs[index]), 0, 1, 1, qdmrThirdHealth[index], qdmrThirdStyles[index]);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrThirdNpcs[index]), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrThirdNpcs[index]), 0, 0x200000, 2048);
        }
        FTBL.vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        FTBL.HLib_SetCurrentTaskTo(QdmrThirdRoomFight);
        break;
    }
}

void QdmrThirdRoomFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin;
    Vector3i rotation;
    Vector3 position;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.IsFightActive())
            break;
        if (!(work->flags & 1)) {
            if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
                QueueAreaLoopSound(2141, 30);
                QueueAreaLoopSound(2142, 50);
                FTBL.spmFadeOut(0);
                work->flags |= 0x20;
            }
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        SetQdmrRoomProps(0);
        FTBL.flddSetPointFlagOnGrid(QDMR_QF3B, 1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFB3, 0);
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrThirdNpcs, 0);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrThirdPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrThirdPackage, 0, 0);
            rotation.x = 0;
            rotation.z = 0;
            GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
            position.x = 0.5f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &position);
            rotation.y += 0x8000;
            FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
            FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
            FTBL.camCtrlUtil(15, 0);
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
        } else {
            QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            if (FTBL.EV_ControlPackageResources(0, 5, qdmrThirdPackage, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, qdmrThirdPackage, 0, 0);
            work->battleRetries++;
            FTBL.HLib_SetCurrentTaskTo(QdmrBeginRetry);
        }
        break;
    }
}

void QdmrRepeatFirstRoom(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        FTBL.EV_FreeAuthorPak(1, qdmrNormalConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitFirstRepeatPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrNormalPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitFirstRepeatPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.HLib_SetCurrentTaskTo(QdmrFirstRepeatPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrFirstNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrFirstRepeatPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin, position;
    Vector3i rotation;
    shen_name door;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        position.x = 204.66f;
        position.z = 167.3f;
        rotation.x = 0;
        rotation.y = 0xa0fe;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_SBK, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_SBK, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        position.x = 205.54f;
        position.z = 169.26f;
        rotation.x = 0;
        rotation.y = 0xe3e9;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_ANC, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_ANC, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        FTBL.SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndFirstRepeat);
        FTBL.HLib_SetupTaskDelay(-1);
        door = work->door;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A008", 1, 40, -1, 0);
            break;
        case 1:
            QueueAreaLoopVoice("0266A002", 1, 40, -1, 0);
            break;
        case 2:
            QueueAreaLoopVoice("0266A003", 1, 40, -1, 0);
            break;
        default:
            QueueAreaLoopVoice("0266A005", 1, 40, -1, 0);
            break;
        }
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0266A004", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        default:
            QueueAreaLoopVoice("0266A001", 1, 100, -1, ResolveAreaLoopParent);
            break;
        }
        if (work->doorQteButtons & 1024) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 80, 0, &position, &rotation, 0);
            position.x = 1.4f;
            position.y = 0;
            position.z = 0.8f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            rotation.y += 16384;
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7233, 0, 70, 35.0f, -1.0f, 0.5f, 0.0f, 0, 6);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 71, 10, 140.0f, -1.0f, 0.8f, 16.0f, 0, 4);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 80, 110, 148.0f, -1.0f, 0.8f, 0.0f, 0, 6);
            QueueAreaLoopCamera(qdmrNormalPackage, 3100, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3101, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3102, QDMR_DUMMY, 80, 0);
            QueueQdmrDoorMotion(door, 0, 7234, 0.0f, 1.6f);
            QueueQdmrDoorMotion(door, 15, 7234, 15.0f, 0.0f);
        } else if (work->doorQteButtons & 4) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 0, 70, 40.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 30);
            QueueAreaLoopCamera(qdmrNormalPackage, 3200, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3201, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3202, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 30, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 45, 7234, 15.0f, 0.0f);
        } else {
            position.x = -0.1f;
            position.y = 0;
            position.z = 1.5f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7239, 0, 70, 70.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 13);
            QueueAreaLoopCamera(qdmrNormalPackage, 3000, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3001, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3002, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 12, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 27, 7234, 15.0f, 0.0f);
        }
        work->doorQteButtons = 0;
        break;
    }
}

void QdmrEndFirstRepeat(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            QueueAreaLoopSound(2141, 30);
            QueueAreaLoopSound(2142, 50);
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrFirstNpcs, 0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrNormalConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.doorLoopRigidMotion(work->door, 7234, -1.0f, 1.0f);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.EV_ControlPackageResources(0, 5, qdmrNormalPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qdmrNormalPackage, 0, 0);
        rotation.x = 0;
        rotation.y = work->savedYaw;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &work->savedPosition);
        FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        break;
    }
}

void QdmrRepeatSecondRoom(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        FTBL.EV_FreeAuthorPak(1, qdmrNormalConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitSecondRepeatPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrNormalPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitSecondRepeatPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.HLib_SetCurrentTaskTo(QdmrSecondRepeatPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrSecondNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrSecondRepeatPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin, position;
    Vector3i rotation;
    shen_name door;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        position.x = 254.3f;
        position.z = 156.5f;
        rotation.x = 0;
        rotation.y = 0xa000;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_KU8, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KU8, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        position.x = 254.6f;
        position.z = 158.0f;
        rotation.x = 0;
        rotation.y = 10922;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_KHO, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KHO, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        FTBL.SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndSecondRepeat);
        FTBL.HLib_SetupTaskDelay(-1);
        door = work->door;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A008", 1, 40, -1, 0);
            break;
        case 1:
            QueueAreaLoopVoice("0266A002", 1, 40, -1, 0);
            break;
        case 2:
            QueueAreaLoopVoice("0266A003", 1, 40, -1, 0);
            break;
        default:
            QueueAreaLoopVoice("0266A005", 1, 40, -1, 0);
            break;
        }
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0266A004", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        default:
            QueueAreaLoopVoice("0266A001", 1, 100, -1, ResolveAreaLoopParent);
            break;
        }
        if (work->doorQteButtons & 1024) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 80, 0, &position, &rotation, 0);
            position.x = 1.4f;
            position.y = 0;
            position.z = 0.8f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            rotation.y += 16384;
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7233, 0, 70, 35.0f, -1.0f, 0.5f, 0.0f, 0, 6);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 71, 10, 140.0f, -1.0f, 0.8f, 16.0f, 0, 4);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 80, 110, 148.0f, -1.0f, 0.8f, 0.0f, 0, 6);
            QueueAreaLoopCamera(qdmrNormalPackage, 3100, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3101, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3102, QDMR_DUMMY, 80, 0);
            QueueQdmrDoorMotion(door, 0, 7234, 0.0f, 1.6f);
            QueueQdmrDoorMotion(door, 15, 7234, 15.0f, 0.0f);
        } else if (work->doorQteButtons & 4) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 0, 70, 40.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 30);
            QueueAreaLoopCamera(qdmrNormalPackage, 3200, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3201, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3202, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 30, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 45, 7234, 15.0f, 0.0f);
        } else {
            position.x = -0.1f;
            position.y = 0;
            position.z = 1.5f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7239, 0, 70, 70.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 13);
            QueueAreaLoopCamera(qdmrNormalPackage, 3000, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3001, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3002, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 12, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 27, 7234, 15.0f, 0.0f);
        }
        work->doorQteButtons = 0;
        break;
    }
}

void QdmrEndSecondRepeat(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            QueueAreaLoopSound(2141, 30);
            QueueAreaLoopSound(2142, 50);
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrSecondNpcs, 0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrNormalConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.doorLoopRigidMotion(work->door, 7234, -1.0f, 1.0f);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.EV_ControlPackageResources(0, 5, qdmrNormalPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qdmrNormalPackage, 0, 0);
        rotation.x = 0;
        rotation.y = work->savedYaw;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &work->savedPosition);
        FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        break;
    }
}

void QdmrRepeatThirdRoom(void *data)
{
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        FTBL.EV_FreeAuthorPak(1, qdmrNormalConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitThirdRepeatPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrNormalPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitThirdRepeatPackage(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (!package || !FTBL.LeaveDoorSubscene(package))
            break;
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.HLib_SetCurrentTaskTo(QdmrThirdRepeatPresentation);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrThirdNpcs, ResolveAreaLoopParent);
        break;
    }
}

void QdmrThirdRepeatPresentation(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3 origin, position;
    Vector3i rotation;
    shen_name door;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        position.x = 219.02f;
        position.z = 120.21f;
        rotation.x = 0;
        rotation.y = 20075;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_UYD, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_UYD, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        position.x = 217.25f;
        position.z = 120.36f;
        rotation.x = 0;
        rotation.y = 0xed82;
        rotation.z = 0;
        QueueAreaLoopPosition(QDMR_KT7, 0, 0, &position, &rotation, 0);
        QueueAreaLoopMotion(QDMR_KT7, 7259, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        FTBL.SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndThirdRepeat);
        FTBL.HLib_SetupTaskDelay(-1);
        door = work->door;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(door, &origin, &rotation.y);
        QueueAreaLoopPosition(QDMR_DUMMY, 0, 0, &origin, &rotation, 0);
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A008", 1, 40, -1, 0);
            break;
        case 1:
            QueueAreaLoopVoice("0266A002", 1, 40, -1, 0);
            break;
        case 2:
            QueueAreaLoopVoice("0266A003", 1, 40, -1, 0);
            break;
        default:
            QueueAreaLoopVoice("0266A005", 1, 68, -1, 0);
            break;
        }
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0266A004", 1, 100, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0266A007", 1, 100, -1, ResolveAreaLoopParent);
            break;
        default:
            QueueAreaLoopVoice("0266A001", 1, 100, -1, ResolveAreaLoopParent);
            break;
        }
        if (work->doorQteButtons & 1024) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 80, 0, &position, &rotation, 0);
            position.x = 1.4f;
            position.y = 0;
            position.z = 0.8f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            rotation.y += 16384;
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7233, 0, 70, 35.0f, -1.0f, 0.5f, 0.0f, 0, 6);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 71, 10, 140.0f, -1.0f, 0.8f, 16.0f, 0, 4);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 80, 110, 148.0f, -1.0f, 0.8f, 0.0f, 0, 6);
            QueueAreaLoopCamera(qdmrNormalPackage, 3100, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3101, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3102, QDMR_DUMMY, 80, 0);
            QueueQdmrDoorMotion(door, 0, 7234, 0.0f, 1.6f);
            QueueQdmrDoorMotion(door, 15, 7234, 15.0f, 0.0f);
        } else if (work->doorQteButtons & 4) {
            position.x = -0.4f;
            position.y = 0;
            position.z = 0.7f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7238, 0, 70, 40.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 30);
            QueueAreaLoopCamera(qdmrNormalPackage, 3200, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3201, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3202, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 30, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 45, 7234, 15.0f, 0.0f);
        } else {
            position.x = -0.1f;
            position.y = 0;
            position.z = 1.5f;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
            QueueAreaLoopPosition(QDMR_PLAYER, 0, 0, &position, &rotation, 0);
            QueueAreaLoopMotion(QDMR_PLAYER, 7239, 0, 70, 70.0f, -1.0f, 1.5f, 0.0f, 0, 6);
            QueueAreaLoopSound(2331, 13);
            QueueAreaLoopCamera(qdmrNormalPackage, 3000, QDMR_DUMMY, 0, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3001, QDMR_DUMMY, 40, 0);
            QueueAreaLoopCamera(qdmrNormalPackage, 3002, QDMR_DUMMY, 81, 0);
            QueueQdmrDoorMotion(door, 12, 7234, 0.0f, 1.0f);
            QueueQdmrDoorMotion(door, 27, 7234, 15.0f, 0.0f);
        }
        work->doorQteButtons = 0;
        break;
    }
}

void QdmrEndThirdRepeat(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    Vector3i rotation;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            QueueAreaLoopSound(2141, 30);
            QueueAreaLoopSound(2142, 50);
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        if (!FTBL.LeaveDoorSubscene(0))
            break;
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_NPC_NewLoadSw_0(0);
        RestoreAreaLoopCharacters(qdmrThirdNpcs, 0);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.EV_FreeAuthorPak(0, qdmrNormalConversation);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.doorLoopRigidMotion(work->door, 7234, -1.0f, 1.0f);
        FTBL.ReleaseDoor(work->door, QDMR_PLAYER);
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        if (FTBL.EV_ControlPackageResources(0, 5, qdmrNormalPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qdmrNormalPackage, 0, 0);
        rotation.x = 0;
        rotation.y = work->savedYaw;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(QDMR_PLAYER, 0x78000000, &work->savedPosition);
        FTBL.EV_AT_SetAngY_0(QDMR_PLAYER, 0x78000000, &rotation, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
        FTBL.EV_LoadAudio(qdmrCutscene, 2);
        break;
    }
}

void QdmrOpenBingoRoom(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        SetQdmrPlayerHandState(QDMR_PLAYER, 1);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        work->flags |= 0x20;
        FTBL.EV_FreeAuthorPak(1, qdmrConversation);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitBingoPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), qdmrBingoPackage, ResolveAreaLoopParent);
        break;
    }
}

void QdmrWaitBingoPackage(void *data)
{
    if (data) {
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitBingoNpcs);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(qdmrBingoNpcs, ResolveAreaLoopParent);
    } else {
        FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
    }
}

void QdmrWaitBingoNpcs(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    shen_name package;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        package = GetQdmrRoomPackage(work->door);
        if (package) {
            if (!FTBL.LeaveDoorSubscene(package))
                break;
            FTBL.EV_NPC_NewLoadSw_0(0);
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        FTBL.HLib_SetCurrentTaskTo(QdmrEndBingoOpening);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopAuthor(qdmrBingoOpeningPackage, qdmrBingoOpeningAuthor, 0, -1, QdmrBingoOpeningAuthorCompletion);
        break;
    }
}

int QdmrBingoOpeningAuthorCompletion(int phase, int value, HLibTask *parent)
{
    QdmrWork *work;
    Vector3i rotation;
    Vector3 playerPosition, position;
    float x, z;
    int index;
    switch (phase) {
    case 0:
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 4);
        FTBL.EV_spmBeginSampleExecute(0);
        work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
        if (work) {
            work->flags &= ~0x20;
            FTBL.SetDoorSubjectEnabled(work->door, 0);
        }
        break;
    case 1:
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
        if (work)
            FTBL.SetDoorSubjectEnabled(work->door, 1);
        FTBL.EV_DisableCharacterDisplay(QDMR_AA99);
        FTBL.EV_DisableCharacterDisplay(QDMR_A020);
        if (FTBL.EV_authSceneCtrl(-13)) {
            if (work)
                work->flags |= 1;
            FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        FTBL.spmFadeOut(0);
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 3);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueAreaLoopMotion(QDMR_PLAYER, 13, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
        rotation.x = 0;
        rotation.z = 0;
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &playerPosition, 0x40000000);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        for (index = 0; qdmrBingoNpcs[index]; index++) {
            QueueAreaLoopMotion(qdmrBingoNpcs[index], 3632, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 0);
            FTBL.GetCharaPosition(qdmrBingoNpcs[index], -1, &position, 0x40000000);
            x = playerPosition.x - position.x;
            z = playerPosition.z - position.z;
            if (__builtin_fabs(x) < 1.1920929e-7f)
                x = 0;
            if (__builtin_fabs(z) < 1.1920929e-7f)
                z = 0;
            if (__builtin_fabs(x) < 1.1920929e-7f && __builtin_fabs(z) < 1.1920929e-7f)
                rotation.y = 0;
            else
                rotation.y = FTBL.InverseFixedAtan2(x, z);
            FTBL.EV_AT_SetAngY_0(qdmrBingoNpcs[index], 0x78000000, &rotation, 0);
        }
        break;
    case 2:
        if (FTBL.EV_authSceneCtrl(-5) - 32 == value) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
            if (work)
                work->flags |= 1;
        }
        break;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void QdmrEndBingoOpening(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    int index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        FTBL.EV_FreeAuthorPak(0, qdmrConversation);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        SetQdmrPlayerHandState(QDMR_PLAYER, 0);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.EV_ControlPackageResources(2, 2, qdmrPackage, 0, 0);
        SetQdmrRoomProps(1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFIX, 0);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFBA, 1);
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, qdmrBingoPackage, 1);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDMR_PLAYER), 0, 0, 0, work->battleRetries * 100 + 200, 0);
        for (index = 0; qdmrBingoNpcs[index]; index++) {
            if (qdmrBingoNpcs[index] == QDMR_KF4) {
                FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 1, 1, qdmrBingoHealth[index], qdmrBingoStyles[index]);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 0x400000, 32);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 0x200000, 2048);
            } else {
                FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 1, 1, qdmrBingoHealth[index], qdmrBingoStyles[index]);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 0x400000, 128);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qdmrBingoNpcs[index]), 0, 0x200000, 2048);
            }
        }
        FTBL.vfmQueueFightStart(4, 99, 1.0f, 0, -1);
        FTBL.HLib_SetCurrentTaskTo(QdmrBingoFight);
        work->flags &= ~1;
        break;
    }
}

void QdmrBingoFight(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        if (FTBL.IsFightActive())
            break;
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            break;
        }
        FTBL.EV_ControlPackageResources(2, 2, qdmrPackage, 0, 0);
        SetQdmrRoomProps(0);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFIX, 1);
        FTBL.flddSetPointFlagOnGrid(QDMR_QFBA, 0);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            SetQdmrPlayerHandState(QDMR_PLAYER, 1);
            SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
            FTBL.EV_FreeAuthorPak(1, qdmrConversation);
            FTBL.HLib_SetCurrentTaskTo(QdmrBingoWin);
            FTBL.HLib_SetupTaskDelay(-1);
            FTBL.EV_DisableCharacterDisplay(QDMR_P02);
            FTBL.EV_DisableCharacterDisplay(QDMR_P07);
            FTBL.EV_DisableCharacterDisplay(QDMR_P09);
            FTBL.spmFadeOut(0);
            work->flags |= 0x20;
            QueueAreaLoopAuthor(qdmrBingoWinPackage, qdmrBingoWinAuthor, 0, -1, QdmrBingoWinAuthorCompletion);
        } else {
            FTBL.HLib_SetCurrentTaskTo(QdmrBingoLose);
            FTBL.Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
            FTBL.HLib_SetupTaskDelay(50);
        }
        break;
    }
}

int QdmrBingoWinAuthorCompletion(int phase, int value, HLibTask *parent)
{
    QdmrWork *work;
    switch (phase) {
    case 0:
        FTBL.EV_LoadCutsceneFromCurrentScene(qdmrCutscene);
        FTBL.EV_LoadAudio(qdmrCutscene, 5);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
        if (work)
            work->flags &= ~1;
        break;
    case 1:
        work = (QdmrWork *)FTBL.HLib_GetTaskParameter(parent);
        if (work)
            work->flags |= 0x21;
        break;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void QdmrBingoLose(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    if (FTBL.LeaveDoorSubscene(0)) {
        FTBL.ST_LoadTitleFade();
        RestoreAreaLoopCharacters(qdmrBingoNpcs, 0);
        if (FTBL.EV_ControlPackageResources(0, 5, qdmrBingoPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qdmrBingoPackage, 0, 0);
        if (work) {
            work->battleRetries++;
            QdmrOpenBingoRoom(work);
        } else {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
        }
    }
}

void QdmrBingoWin(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    RestoreAreaLoopCharacters(qdmrBingoNpcs, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, qdmrBingoPackage, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, qdmrBingoPackage, 0, 0);
    FTBL.EV_FreeAuthorPak(0, qdmrConversation);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    SetQdmrPlayerHandState(QDMR_PLAYER, 0);
    FTBL.camCtrlUtil(15, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    if (work)
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
    QueueAreaLoopMotion(QDMR_PLAYER, 0xf002, 0, -1, 0.0f, -1.0f, 1.0f, 0.0f, 0, 2);
    FTBL.EV_stepSetFlags(298, 0);
    FTBL.EV_stepSetFlags(309, 0);
    FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
}

void QdmrBeginRetry(void *data)
{
    QdmrWork *work = (QdmrWork *)data;
    if ((work->timerFlags & 3) == 1)
        work->timerRequests |= 0x40000000;
    if (FTBL.filmCheckFadeTrackColour())
        return;
    if (work->flags & 1) {
        FTBL.Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
        FTBL.HLib_SetCurrentTaskTo(QdmrRetryTitle);
        FTBL.HLib_SetupTaskDelay(50);
        FTBL.spmFadeOut(0);
        work->flags |= 0x20;
    } else {
        FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1;
    }
}

void QdmrRetryTitle(void *data)
{
    FTBL.ST_LoadTitleFade();
    FTBL.HLib_SetCurrentTaskTo(QdmrStartYuanAuthor);
    FTBL.HLib_SetupTaskDelay(50);
}

void QdmrTimerCallback(void *data)
{
    HLibTask **parent = (HLibTask **)data;
    QdmrWork *work;
    int flags;
    unsigned int colour, component;
    int retire = 0;
    int centiseconds, minutes, tens, seconds, tenths, hundredths;
    char separator, *text;
    if (parent && *parent && (work = (QdmrWork *)FTBL.HLib_GetTaskParameter(*parent)) != 0 && work->timerText) {
        if (work->timer > 0 && !FTBL.GetShouldRenderIcons(1))
            work->timer--;
        while (work->timerRequests & 0xc0000000) {
            if (work->timerRequests & 0x40000000) {
                if (work->timerFlags & 2) {
                    if (work->timerFlags & 1)
                        work->timerFlags &= ~1;
                    else
                        work->timerFlags |= 1;
                    work->timerFadeFrames = 16 - work->timerFadeFrames;
                } else {
                    work->timerFadeFrames = 16;
                    work->timerFlags |= 2;
                }
                work->timerRequests &= ~0x40000000;
            }
            if (work->timerRequests < 0) {
                if (!(work->timerFlags & 0x40000000)) {
                    work->timerFlags |= 0x40000000;
                    if (work->timerFlags & 1) {
                        if (!(work->timerFlags & 2))
                            work->timerRequests |= 0x40000000;
                    } else if (work->timerFlags & 2) {
                        work->timerRequests |= 0x40000000;
                    }
                } else {
                    work->timerRequests &= 0x7fffffff;
                }
            }
        }
        flags = work->timerFlags;
        while (flags & 0xc0000003) {
            if (flags & 2) {
                if (--work->timerFadeFrames <= 0) {
                    if (work->timerFlags & 1) {
                        work->timerFlags &= ~1;
                        flags &= ~1;
                    } else {
                        work->timerFlags |= 1;
                        flags |= 1;
                    }
                    work->timerFlags &= ~2;
                } else {
                    flags |= 1;
                }
                flags &= ~2;
            }
            if (flags & 3) {
                if (flags & 2) {
                    if (flags & 1)
                        colour = 255 * work->timerFadeFrames / 16;
                    else
                        colour = 255 - 255 * work->timerFadeFrames / 16;
                } else {
                    colour = 255;
                }
                colour <<= 24;
                if (work->timer > work->timerWarningThreshold + 16) {
                    colour |= 0xffffff;
                } else if (work->timer < work->timerWarningThreshold - 16) {
                    colour |= 0xff0000;
                } else {
                    component = 255 * (work->timer - work->timerWarningThreshold + 16) / 32;
                    colour |= component << 8 | 0xff0000 | component;
                }
                work->timerText->colour = colour;
                centiseconds = (int)((float)work->timer * 3.3333333f);
                if (centiseconds < 0)
                    centiseconds = 0;
                text = work->timerText->text;
                minutes = centiseconds / 6000 % 10;
                tens = centiseconds / 1000 % 6;
                seconds = centiseconds / 100 % 10;
                tenths = centiseconds / 10 % 10;
                hundredths = centiseconds % 10;
                separator = tenths == 1 || tenths == 4 ? ' ' : ':';
                text[0] = minutes + '0';
                text[1] = separator;
                text[2] = tens + '0';
                text[3] = seconds + '0';
                text[4] = separator;
                text[5] = tenths + '0';
                text[6] = hundredths + '0';
                text[7] = 0;
                work->timerText->depth = 8.0f;
                work->timerText->y = 64.0f;
                work->timerText->draw(work->timerText);
                if ((unsigned int)(centiseconds - 1) <= 998) {
                    switch (centiseconds / 333) {
                    case 0:
                        if (work->timer % 10 == 0)
                            FTBL.Loop3DSoundEffect(171);
                        break;
                    case 1:
                        if (work->timer % 15 == 0)
                            FTBL.Loop3DSoundEffect(171);
                        break;
                    default:
                        if (work->timer % 30 == 0)
                            FTBL.Loop3DSoundEffect(171);
                        break;
                    }
                }
                flags &= ~1;
            }
            if (flags & 0x40000000) {
                if (!(flags & 3)) {
                    work->timerFlags = (work->timerFlags & 0xbfffffff) | 0x80000000;
                    retire = 1;
                }
                flags &= ~0x40000000;
            }
            if (flags < 0) {
                retire = 1;
                flags &= 0x7fffffff;
            }
        }
        if (work->flags & 8)
            FTBL.SetPlayerGameFlags(6, 1);
    } else {
        retire = 1;
    }
    if (retire)
        FTBL.HLib_DestroyCurrentEventTask();
}

void QdmrEscapeToLadder(void *data)
{
    Vector3 position;
    for (;;) {
        if (!data) {
            FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
            break;
        }
        FTBL.EV_FreeAuthorPak(1, qdmrConversation);
        QueueAreaLoopVoice("0266A009", 1, 0, -1, 0);
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        position.x = qdmrEscapePosition.x;
        position.z = qdmrEscapePosition.z;
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QDMR_PLAYER), &position, qdmrEscapeYaw, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QdmrWaitLadderMotion);
        break;
    }
}

void QdmrWaitLadderMotion(void *data)
{
    if (data) {
        if (!FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(QDMR_PLAYER), 58, 1)) {
            FTBL.HLib_SetCurrentTaskTo(QdmrInFree);
            FTBL.EV_FreeAuthorPak(0, qdmrConversation);
        }
    } else {
        FTBL.HLib_SetCurrentTaskTo(QdmrDoneCallback);
    }
}

shen_name GetQdmrRoomPackage(shen_name door)
{
    shen_name package = 0;
    int index;
    for (index = 0; qdmrRooms[index].door; index++) {
        if (qdmrRooms[index].door == door) {
            package = qdmrRooms[index].package;
            break;
        }
    }
    return package;
}

int GetQdmrDoorPlacement(shen_name door, Vector3 *position, int *yaw)
{
    int found = 0;
    int index;
    for (index = 0; qdmrDoorPlacements[index].door; index++) {
        if (qdmrDoorPlacements[index].door == door) {
            if (position) {
                position->x = qdmrDoorPlacements[index].x;
                position->y = FTBL.GetQronTemporaryFloorHeight();
                position->z = qdmrDoorPlacements[index].z;
            }
            found = 1;
            if (yaw)
                *yaw = qdmrDoorPlacements[index].yaw;
            break;
        }
    }
    return found;
}

shen_name FindNearestQdmrDoor(unsigned int *roomFlags)
{
    shen_name result = 0;
    Vector3 position, direction;
    Vector3i rotation, yaw;
    float nearest, x, z, xx, zz, distance;
    int index;
    if (roomFlags) {
        FTBL.GetCharaPosition(QDMR_PLAYER, -1, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, 0x40000000);
        nearest = -1.0f;
        for (index = 0; qdmrDoorPlacements[index].door && (roomFlags[index] & 1); index++) {
            x = position.x - qdmrDoorPlacements[index].x;
            z = position.z - qdmrDoorPlacements[index].z;
            if (__builtin_fabs(x) < 1.1920929e-7f)
                x = 0;
            if (__builtin_fabs(z) < 1.1920929e-7f)
                z = 0;
            xx = x * x;
            zz = z * z;
            if (__builtin_fabs(xx) < 1.1920929e-7f)
                xx = 0;
            if (__builtin_fabs(zz) < 1.1920929e-7f)
                zz = 0;
            distance = zz + xx;
            if (__builtin_fabs(distance) < 1.1920929e-7f)
                distance = 0;
            if (distance < 4.0f) {
                direction.x = 0;
                direction.y = 0;
                direction.z = 4.0f - distance;
                if (__builtin_fabs(direction.z) < 1.1920929e-7f)
                    direction.z = 0;
                yaw.x = 0;
                yaw.y = rotation.y - qdmrDoorPlacements[index].yaw;
                yaw.z = 0;
                FTBL.ApplyScenarioTranslationAndRotation(0, &yaw, &direction);
                if (direction.z >= nearest) {
                    nearest = direction.z;
                    if (nearest > -0.1f)
                        result = qdmrDoorPlacements[index].door;
                }
            }
        }
    }
    return result;
}

int GetQdmrDoorVariant(shen_name door, unsigned int *roomFlags)
{
    int variant = 0;
    int index;
    do {
        for (index = 0; qdmrDoorRoutes[index].door; index++) {
            if (qdmrDoorRoutes[index].door == door) {
                variant = qdmrDoorRoutes[index].variant;
                if (roomFlags[index] & 2) {
                    if (variant == 1)
                        variant = 4;
                    if (variant == 2)
                        variant = 5;
                    if (variant == 3)
                        variant = 6;
                }
                break;
            }
        }
    } while (0);
    return variant;
}

void SetQdmrPlayerHandState(shen_name character, int scripted)
{
    if (character && FTBL.ctrlGetCharaByName(character)) {
        if (scripted) {
            FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(character));
            FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(character));
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(character), 1, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(character), 1, 1, 0);
            FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(character), 1, (void *)areaLoopHandPoses[18], 1);
            FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(character), 0, (void *)areaLoopHandPoses[18], 1);
        } else {
            FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(character));
            FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(character));
        }
    }
}

int FindClearRotationPathForAngY(int *yaw)
{
    int result = 0;
    Vector3i rotation;
    int intersections[8];
    float angle;
    int firstSector, index;
    if (yaw) {
        FTBL.GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, 0);
        angle = (float)rotation.y / 182.04445f + 22.5f;
        if (__builtin_fabs(angle) < 1.1920929e-7f)
            angle = 0;
        while (angle >= 360.0f) {
            angle -= 360.0f;
            if (__builtin_fabs(angle) < 1.1920929e-7f)
                angle = 0;
        }
        while (angle < 0) {
            angle += 360.0f;
            if (__builtin_fabs(angle) < 1.1920929e-7f)
                angle = 0;
        }
        firstSector = (int)angle / 45;
        rotation.x = 0;
        rotation.z = 0;
        for (index = 0; index < 8; index++) {
            Vector3 origin, destination, intersection;
            FTBL.GetCharaPosition(QDMR_PLAYER, -1, &origin, 0);
            destination.x = 0;
            destination.y = 0.3f;
            destination.z = -1.8f;
            rotation.y = (unsigned short)(int)((float)(firstSector + index) * 45.0f * 8192.0f / 45.0f);
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &destination);
            intersections[index] = (FTBL.FindNearestRayIntersection(&intersection, &destination, &origin) & 1) == 0;
        }
        for (index = 0; index < 24; index++) {
            if ((intersections[(index + 7) % 8] && intersections[index % 8] && intersections[(index + 1) % 8] && index >= 16) ||
                ((intersections[(index + 6) % 8] || intersections[(index + 2) % 8]) && intersections[(index + 7) % 8] && intersections[index % 8] &&
                 intersections[(index + 1) % 8] && index >= 8) ||
                (intersections[(index + 6) % 8] && intersections[(index + 7) % 8] && intersections[index % 8] && intersections[(index + 1) % 8] &&
                 intersections[(index + 2) % 8])) {
                result = 1;
                *yaw = (unsigned short)(int)((float)(firstSector + index) * 45.0f * 8192.0f / 45.0f);
                break;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

void QueueQdmrDoorMotion(shen_name door, int delay, int motion, float startFrame, float speed)
{
    HLibTask *task;
    QdmrDoorMotionWork *work;
    door &= ~0x20;
    if (delay > 0) {
        while ((task = FTBL.HLib_EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 11, sizeof(QdmrDoorMotionWork), TASK_EVNT)) &&
               (work = (QdmrDoorMotionWork *)FTBL.HLib_GetTaskParameter(task))) {
            work->door = door;
            work->frames = delay;
            work->motion = motion;
            work->startFrame = startFrame;
            work->speed = speed;
            break;
        }
    } else {
        FTBL.doorLoopRigidMotion(door, motion, startFrame, speed);
    }
}

void QdmrDoorMotionCallback(void *data)
{
    QdmrDoorMotionWork *work = (QdmrDoorMotionWork *)data;
    if (work && --work->frames <= 0) {
        FTBL.doorLoopRigidMotion(work->door, work->motion, work->startFrame, work->speed);
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void MarkQdmrRoomVisited(shen_name door, unsigned int *roomFlags)
{
    Vector3 position;
    int index;
    if (!door || !roomFlags)
        return;
    for (index = 0; qdmrRooms[index].door; index++) {
        if (qdmrRooms[index].door == door) {
            roomFlags[index] &= ~1;
            if (!(roomFlags[index] & 4)) {
                GetQdmrDoorPlacement(door, &position, 0);
                roomFlags[index] |= 4;
                FTBL.GetNpcAnimationState(&position, 4);
            }
            if (!(roomFlags[index] & 2))
                roomFlags[index] |= 2;
        }
    }
}

void SetQdmrRoomProps(int enabled)
{
    int hidden = !enabled;
    FTBL.flddSetPointFlagOnGrid(LIGHT_DEFAULT_DEFINITION, hidden);
    FTBL.flddSetPointFlagOnGrid(QDMR_QF0X, hidden);
    FTBL.flddSetPointFlagOnGrid(QDMR_PROP, hidden);
}

void QueueQdmrFadeOutSound(int delay)
{
    HLibTask *task;
    if (delay > 0 && (task = FTBL.HLib_EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 11, 0, TASK_EVNT)) != 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    else
        FTBL.spmFadeOut(0);
}

void QdmrFadeOutSoundCallback(void *data)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_DestroyCurrentEventTask();
}

char qdmrNormalPackage[] = "QDMRNRML";
char qdmrNormalConversation[] = "0266";
char qdmrFirstPackage[] = "QDMRBTL1";
char qdmrFirstConversation[] = "0266";
shen_name qdmrFirstNpcs[] = {SHEN_NAME4('S', 'B', 'K', '_'), SHEN_NAME4('A', 'N', 'C', '_'), 0};
int qdmrFirstStyles[] = {2, 3};
int qdmrFirstHealth[] = {100, 100};
char qdmrSecondPackage[] = "QDMRBTL2";
char qdmrSecondConversation[] = "0266";
shen_name qdmrSecondNpcs[] = {SHEN_NAME4('K', 'U', '8', '_'), SHEN_NAME4('K', 'H', 'O', '_'), 0};
int qdmrSecondStyles[] = {4, 2};
int qdmrSecondHealth[] = {100, 100};
char qdmrThirdPackage[] = "QDMRBTL3";
char qdmrThirdConversation[] = "0266";
shen_name qdmrThirdNpcs[] = {SHEN_NAME4('U', 'Y', 'D', '_'), SHEN_NAME4('K', 'T', '7', '_'), 0};
int qdmrThirdStyles[] = {3, 4};
int qdmrThirdHealth[] = {100, 100};
char qdmrBingoPackage[] = "QDMRBBTL";
Vector3 qdmrEscapePosition = {218.74f, 164.8f, 175.28f};
int qdmrEscapeYaw = 10922;
Vector3 qdmrLadderPosition = {224.17f, 164.8f, 178.94f};

#define SHENMUE_AREA_LOOP_HAND_POSE_TABLE
#include "s2/areas/area_loop_dc.c"
#endif
#else
#include "s2/areas/area_qdmr.hpp"
#include "s2/areas/area_loop.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/scene_data.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
void QdmrInitialize(void *);
void QdmrInitialize2(void *);
void QdmrStartInitialAuthor(void *);
void QdmrEndInitialAuthor(void *);
void QdmrExitInitialAuthor(void *);
void QdmrInFree(void *);
void QdmrStartTimeoutAuthor(void *);
void QdmrWaitTimeoutNpcs(void *);
void QdmrEndTimeoutAuthor(void *);
void QdmrExitTimeoutAuthor(void *);
void QdmrStartYuanAuthor(void *);
void QdmrWaitYuanNpcs(void *);
void QdmrEndYuanAuthor(void *);
void QdmrExitYuanAuthor(void *);
void QdmrBeginRetry(void *);
void QdmrRetryTitle(void *);
void QdmrStartTransition(void *);
void QdmrMoveToTransition(void *);
void QdmrEndTransition(void *);
void QdmrStartOpenDoor(void *);
void QdmrEscapeToLadder(void *);
void QdmrWaitLadderMotion(void *);
void QdmrOpenFirstRoom(void *);
void QdmrOpenSecondRoom(void *);
void QdmrOpenThirdRoom(void *);
void QdmrRepeatFirstRoom(void *);
void QdmrRepeatSecondRoom(void *);
void QdmrRepeatThirdRoom(void *);
void QdmrOpenBingoRoom(void *);
void QdmrOpenNormalRoom(void *);
void QdmrWaitFirstRoomPackage(void *);
void QdmrWaitSecondRoomPackage(void *);
void QdmrWaitThirdRoomPackage(void *);
void QdmrWaitFirstRepeatPackage(void *);
void QdmrWaitSecondRepeatPackage(void *);
void QdmrWaitThirdRepeatPackage(void *);
void QdmrWaitBingoPackage(void *);
void QdmrWaitNormalRoom(void *);
void QdmrFirstRoomPresentation(void *);
void QdmrSecondRoomPresentation(void *);
void QdmrThirdRoomPresentation(void *);
void QdmrFirstRepeatPresentation(void *);
void QdmrSecondRepeatPresentation(void *);
void QdmrThirdRepeatPresentation(void *);
void QdmrWaitBingoNpcs(void *);
void QdmrNormalRoomPresentation(void *);
void QdmrBeginFirstRoomFight(void *);
void QdmrBeginSecondRoomFight(void *);
void QdmrBeginThirdRoomFight(void *);
void QdmrFirstRoomFight(void *);
void QdmrSecondRoomFight(void *);
void QdmrThirdRoomFight(void *);
void QdmrEndFirstRepeat(void *);
void QdmrEndSecondRepeat(void *);
void QdmrEndThirdRepeat(void *);
void QdmrEndNormalRoom(void *);
void QdmrEndBingoOpening(void *);
void QdmrBingoFight(void *);
void QdmrBingoWin(void *);
void QdmrBingoLose(void *);
void QdmrAdmissionNoop() {
}

void QdmrMain(std::uintptr_t) {
    const std::uint64_t previousLoad = GetNpcNewLoadState();
    if (ctrlGetCharaByName(QDMR_REN)) {
        EV_PTYCUtils(QDMR_REN, 8, 0);
        EV_npcFree(QDMR_REN);
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    std::int32_t count = 0;
    while (QDMR_ROOMS[count].door.name)
        ++count;
    const std::int32_t roomBytes = 4 * count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QdmrInitialize, 4, 0x0b, sizeof(QdmrEventWork) + roomBytes, AREA_EVENT_TASK);
    if (task) {
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(task));
        if (work) {
            std::memset(static_cast<void *>(work), 0, sizeof(*work));
            HLib::Task *const timer = HLib::EnqueueTaskWithParameter(QdmrTimerCallback, 4, 0x0b, sizeof(HLib::Task *), AREA_EVENT_TASK);
            if (timer) {
                HLib::Task **const timerParent = static_cast<HLib::Task **>(HLib::GetTaskParameter(timer));
                if (timerParent) {
                    *timerParent = nullptr;
                    work->task = task;
                    work->timerTask = timer;
                    work->parent = HLib::GetCurrentTask();
                    if (roomBytes) {
                        work->roomFlags = reinterpret_cast<std::uint32_t *>(work + 1);
                        if (roomBytes > 0)
                            std::memset(work->roomFlags, 0, roomBytes);
                    } else
                        work->roomFlags = nullptr;
                    work->timer = 0;
                    work->timerFlags = 0;
                    work->timerFadeFrames = 0;
                    work->timerWarningThreshold = 900;
                    work->timerRequests = 0;
                    work->timerText = CreateLegacyHudText();
                    if (work->timerText) {
                        *timerParent = task;
                        spmFadeOut();
                        work->flags |= 0x20U;
                        EV_SetCanDoAreaJumpTo(0);
                        EV_EventSetUpHelpIcon_0(10, 0);
                        SetPlayerControlEnabled(0);
                        work->flags |= (previousLoad ? 0x10U : 0U) | 5U;
                        EV_stepSetFlags(0, 0x1be);
                        work->canSkipIntro = 0;
                        return;
                    }
                }
                HLib::DestroyTask(timer);
            }
        }
        HLib::DestroyTask(task);
    }
    if (previousLoad)
        EV_NPC_NewLoadSw(1);
}
void QdmrInitialize(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrInitialize2);
    HLib::SetupTimedTask(work->task, -1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMR");
}
void QdmrInitialize2(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    flddSetPointFlagOnGrid(QDMR_QFBA, 0);
    HLib::SetCurrentTaskTo(QdmrStartInitialAuthor);
}
void QdmrStartInitialAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        if (!SetQronFloor(18))
            return;
        RepositionPlayerForQronFloor(18);
        EV_FreeAuthorPak(1, "0266");
        EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrEndInitialAuthor);
        HLib::SetupTimedTask(work->task, -1);
        work->flags |= 1U;
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->canSkipIntro)
            EV_SetAuthorActiveTo(1);
        QueueAreaLoopAuthor("QDMRINIT\0", QDMR_INIT_LIGHT, QdmrInitialAuthorCompletion);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrEndInitialAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        EV_FreeAuthorPak(0, "0266");
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        HLib::SetCurrentTaskTo(QdmrExitInitialAuthor);
    } else {
        spmFadeOut();
        work->flags |= 0x20U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitInitialAuthor(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!SetQronFloor(18))
        return;
    RepositionPlayerForQronFloor(18);
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_LoadAudio("0266A000", 0);
    EV_spmBeginSampleExecute();
    work->flags &= ~0x20U;
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    work->timer = 1800;
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 218.75999f;
    position.z = 175.24001f;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = {0, 10922, 0}, .completion = nullptr}, 0);
    mtwkSetExtraFlag(scenFindMotionwork(QDMR_PLAYER), 17, 0);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrInFree(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index)
        if (!(work->roomFlags[index] & 1U) && ClaimDoor(QDMR_ROOMS[index].door, QDMR_PLAYER))
            work->roomFlags[index] |= 1U;
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    float x = position.x - QDMR_LADDER_POSITION.x, z = position.z - QDMR_LADDER_POSITION.z;
    if (std::fabs(x) < 1.1920929e-7f)
        x = 0;
    if (std::fabs(z) < 1.1920929e-7f)
        z = 0;
    float xx = x * x, zz = z * z;
    if (std::fabs(xx) < 1.1920929e-7f)
        xx = 0;
    if (std::fabs(zz) < 1.1920929e-7f)
        zz = 0;
    float distance = xx + zz;
    if (std::fabs(distance) < 1.1920929e-7f || distance < 4) {
        HLib::SetCurrentTaskTo(QdmrEscapeToLadder);
        SetPlayerControlEnabled(0);
        work->flags |= 4U;
        return;
    }
    work->flags |= 8U;
    if (work->flags & 4U) {
        FinishLoading();
        SetPlayerControlEnabled(1);
        work->flags &= ~4U;
        return;
    }
    SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    const std::int32_t timerMode = work->timerFlags & 3;
    const std::int64_t soundScene = timerMode ? 2 : 0;
    if (GetSelectedSoundSceneIndex() != soundScene) {
        EV_LoadAudio("0266A000", soundScene);
        EV_spmBeginSampleExecute();
    }
    if (!timerMode) {
        x = position.x - 217.05f;
        z = position.z - 126.56f;
        if (std::fabs(x) < 1.1920929e-7f)
            x = 0;
        if (std::fabs(z) < 1.1920929e-7f)
            z = 0;
        xx = x * x;
        zz = z * z;
        if (std::fabs(xx) < 1.1920929e-7f)
            xx = 0;
        if (std::fabs(zz) < 1.1920929e-7f)
            zz = 0;
        distance = xx + zz;
        if (std::fabs(distance) < 1.1920929e-7f || distance < 4)
            work->timer = 0;
    }
    if (work->timer <= 0) {
        if (!timerMode) {
            HLib::SetCurrentTaskTo(QdmrStartTransition);
            return;
        }
        work->timerRequests |= 0x40000000U;
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 0,
                                .motion = 0xf002,
                                .duration = 1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        HLib::SetCurrentTaskTo(QdmrStartTimeoutAuthor);
        if (srchHasActiveLockonTarget())
            srchReset();
        SetPlayerControlEnabled(0);
        work->flags |= 4U;
        return;
    }
    const shen_name door = FindNearestQdmrDoor(work->roomFlags);
    std::uint16_t buttons = 0;
    if (door.name) {
        if (GetBattleDataConfig(1, QDMR_BTKK) && GetBattleDataConfig(2, QDMR_BTKK))
            buttons = 1024;
        else
            GetBattleDataConfig(0, QDMR_BTKK);
        if (GetBattleDataConfig(1, QDMR_BTPP) && GetBattleDataConfig(2, QDMR_BTPP))
            buttons |= 4U;
        else
            GetBattleDataConfig(0, QDMR_BTPP);
        if (GetBattleDataConfig(1, QDMR_BTAA) && GetBattleDataConfig(2, QDMR_BTAA))
            buttons |= 2U;
        else
            GetBattleDataConfig(0, QDMR_BTAA);
        if (buttons) {
            work->doorQteButtons = buttons;
            SetPlayerControlEnabled(0);
            work->flags |= 4U;
            QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                    .character = QDMR_PLAYER,
                                    .flags = 0,
                                    .motion = 0xf002,
                                    .duration = -1,
                                    .elapsed = 0,
                                    .startFrame = 0,
                                    .endFrame = -1,
                                    .speed = 1,
                                    .frameTime = 8,
                                    .reserved2c = 0,
                                    .completion = nullptr},
                0);
            SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
            if (GetQdmrDoorVariant(door, work->roomFlags))
                spmFadeOut();
            work->flags |= 1U;
            work->door = door;
            HLib::SetCurrentTaskTo(QdmrStartOpenDoor);
        }
    } else
        work->doorQteButtons = 0;
}
void QdmrStartOpenDoor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &work->savedPosition, EventTransformFlags::World);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::World);
    work->savedYaw = rotation.y;
    switch (GetQdmrDoorVariant(work->door, work->roomFlags)) {
    case 1:
        QdmrOpenFirstRoom(work);
        break;
    case 2:
        QdmrOpenSecondRoom(work);
        break;
    case 3:
        QdmrOpenThirdRoom(work);
        break;
    case 4:
        QdmrRepeatFirstRoom(work);
        break;
    case 5:
        QdmrRepeatSecondRoom(work);
        break;
    case 6:
        QdmrRepeatThirdRoom(work);
        break;
    case 7:
        MarkQdmrRoomVisited(work->door, work->roomFlags);
        ReleaseDoor(work->door.name, QDMR_PLAYER);
        work->timerRequests |= 0x40000000U;
        QdmrOpenBingoRoom(work);
        work->battleRetries = 0;
        break;
    default:
        QdmrOpenNormalRoom(work);
        break;
    }
}
void QdmrEscapeToLadder(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_FreeAuthorPak(1, "0266");
    QueueAreaLoopVoice("0266A009", 0, -1, nullptr, 0);
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = QDMR_LADDER_POSITION.x;
    position.z = QDMR_LADDER_POSITION.z;
    mtwkCopyTransition(scenFindMotionwork(QDMR_PLAYER), &position, 10922, 0, 0);
    HLib::SetCurrentTaskTo(QdmrWaitLadderMotion);
}
void QdmrWaitLadderMotion(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!mtwkSetExtraFlag(scenFindMotionwork(QDMR_PLAYER), 58, 1)) {
        HLib::SetCurrentTaskTo(QdmrInFree);
        EV_FreeAuthorPak(0, "0266");
    }
}
void QdmrStartTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (srchHasActiveLockonTarget())
        srchReset();
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    if (GetCurrentAuthorId().name) {
        if (!doorLoadAuthorSilently({}))
            return;
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_NPC_NewLoadSw(0);
    }
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitTimeoutNpcs);
    HLib::SetupTimedTask(work->task, -1);
    QueueAreaLoopNpcs(QDMR_NPCS, ResolveAreaLoopParent);
}
void QdmrWaitTimeoutNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    work->flags &= ~0x20U;
    HLib::SetCurrentTaskTo(QdmrEndTimeoutAuthor);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetupTimedTask(work->task, -1);
    work->flags |= 1U;
    if (work->canSkipIntro)
        EV_SetAuthorActiveTo(1);
    QueueAreaLoopAuthor("QDMRTOVR\0", QDMR_TIMEOUT_AUTHOR, QdmrStoryAuthorCompletion);
}
void QdmrEndTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        RestoreAreaLoopCharacters(QDMR_NPCS);
        EV_FreeAuthorPak(0, "0266");
        spmFadeOut();
        work->flags |= 0x20U;
        HLib::SetCurrentTaskTo(QdmrExitTimeoutAuthor);
        Queue_ST_LoadTitleFadeIn(QDMR_TIMEOUT_TITLE);
        HLib::SetupTaskDelay(100);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitTimeoutAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrStartYuanAuthor);
    EV_spmBeginSampleExecute();
    work->flags &= ~0x20U;
    HLib::SetupTaskDelay(30);
    ST_LoadTitleFade();
    work->canSkipIntro = 1;
}
void QdmrStartYuanAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
            work->roomFlags[index] &= ~2U;
            if (work->roomFlags[index] & 1U) {
                ReleaseDoor(QDMR_ROOMS[index].door.name, QDMR_PLAYER);
                work->roomFlags[index] &= ~1U;
            }
        }
        if (!SetQronFloor(17))
            return;
        RepositionPlayerForQronFloor(17);
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_FreeAuthorPak(1, "0364");
        HLib::SetCurrentTaskTo(QdmrWaitYuanNpcs);
        HLib::SetupTimedTask(work->task, -1);
        QueueAreaLoopNpcs(QDMR_YUAN_NPCS, ResolveAreaLoopParent);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrWaitYuanNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrEndYuanAuthor);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetupTimedTask(work->task, -1);
    work->flags |= 1U;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (work->canSkipIntro)
        EV_SetAuthorActiveTo(1);
    QueueAreaLoopAuthor("QES3END\0", QDMR_YUAN_AUTHOR, QdmrYuanAuthorCompletion);
}
void QdmrEndYuanAuthor(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        RestoreAreaLoopCharacters(QDMR_YUAN_NPCS);
        EV_FreeAuthorPak(0, "0364");
        spmFadeOut();
        work->flags |= 0x20U;
        HLib::SetCurrentTaskTo(QdmrExitYuanAuthor);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrExitYuanAuthor(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (SetQronFloor(18)) {
        RepositionPlayerForQronFloor(18);
        HLib::SetCurrentTaskTo(QdmrStartInitialAuthor);
    }
}
void QdmrBeginRetry(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    work->canSkipIntro = 1;
    if ((work->timerFlags & 3) == 1)
        work->timerRequests |= 0x40000000U;
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
        HLib::SetCurrentTaskTo(QdmrRetryTitle);
        HLib::SetupTaskDelay(50);
        spmFadeOut();
        work->flags |= 0x20U;
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
}
void QdmrRetryTitle(void *) {
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(QdmrStartYuanAuthor);
    HLib::SetupTaskDelay(50);
}
void QdmrStartTransition(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_FreeAuthorPak(1, "0266");
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    HLib::SetCurrentTaskTo(QdmrMoveToTransition);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRTRNS");
}
void QdmrMoveToTransition(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetPlayerControlEnabled(0);
    work->flags |= 4U;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::World);
    FindClearRotationPathForAngY(&rotation.y);
    Vector3 position;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0x16,
                            .motion = 0x1c45,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    HLib::SetCurrentTaskTo(QdmrEndTransition);
    HLib::SetupTaskDelay(-1);
    SetQdmrPlayerHandState(work, true);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QDMR_PLAYER), static_cast<std::int32_t>(FacialExpressionId::Angry), 0, 0.69999999f);
    work->flags |= 2U;
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDMR_PLAYER), 0, reinterpret_cast<std::byte *>(QDMR_TRANSITION_HAND_POSE.data()), 1);
    QueueAreaLoopCamera("QDMRTRNS", 2000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRTRNS", 2001, QDMR_DUMMY, nullptr, 220);
    QueueAreaLoopCamera("QDMRTRNS", 2002, QDMR_DUMMY, nullptr, 540);
    SetCharacterHandSecondaryPoseWork(ctrlGetCharaByName(QDMR_PLAYER), 0, ctrlGetCharaByName(QDMR_TRANSITION_TARGET));
    position = {0.029999999f, -0.050000001f, -0.039999999f};
    rotation = {64917, 32768, 57307};
    QueueQdmrTransitionAttachment(&position, &rotation);
    QueueQdmrTransitionLookAt();
    QueueAreaLoopVoice("0266C001", 0, -1, nullptr, 0);
    QueueAreaLoopVoice("0266C004", 0, -1, nullptr, 100);
    QueueAreaLoopVoice("0266C002", 0, -1, nullptr, 220);
    QueueAreaLoopVoice("0266C003", 0, -1, nullptr, 381);
    QueueAreaLoopVoice("0266A012", 0, -1, ResolveAreaLoopParent, 541);
}
void QdmrEndTransition(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->flags |= 1U;
        return;
    }
    SetCharacterHandSecondaryPoseWork(ctrlGetCharaByName(QDMR_PLAYER), 0, nullptr);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    work->flags &= ~2U;
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRTRNS"), 0);
    EV_FreeAuthorPak(0, "0266");
    work->timerRequests |= 0x40000000U;
    work->timer = 10800;
    HLib::SetCurrentTaskTo(QdmrInFree);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
}
void QdmrOpenFirstRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitFirstRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL1");
}
void QdmrOpenSecondRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitSecondRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL2");
}
void QdmrOpenThirdRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitThirdRoomPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBTL3");
}
void QdmrRepeatFirstRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitFirstRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrRepeatSecondRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitSecondRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrRepeatThirdRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitThirdRepeatPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrOpenBingoRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    work->flags |= 0x20U;
    EV_FreeAuthorPak(1, "0266");
    HLib::SetCurrentTaskTo(QdmrWaitBingoPackage);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRBBTL");
}
void QdmrOpenNormalRoom(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    SetQdmrPlayerHandState(work, true);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
    EV_FreeAuthorPak(1, "0266");
    SelectMagicRoomForPlayer();
    HLib::SetCurrentTaskTo(QdmrWaitNormalRoom);
}
void QdmrWaitFirstRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrFirstRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_FIRST_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitSecondRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrSecondRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_SECOND_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitThirdRoomPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_NPC_NewLoadSw(0);
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 1);
        EV_spmBeginSampleExecute();
        work->flags &= ~0x20U;
        HLib::SetCurrentTaskTo(QdmrThirdRoomPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_THIRD_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitFirstRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrFirstRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_FIRST_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitSecondRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrSecondRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_SECOND_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitThirdRepeatPackage(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (!QDMR_ROOMS[index].package.name || !doorLoadAuthorSilently(QDMR_ROOMS[index].package))
            return;
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 2);
        EV_spmBeginSampleExecute();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QdmrThirdRepeatPresentation);
        HLib::SetupTaskDelay(-1);
        QueueAreaLoopNpcs(QDMR_THIRD_ROOM_NPCS, ResolveAreaLoopParent);
        return;
    }
}
void QdmrWaitBingoPackage(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrWaitBingoNpcs);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopNpcs(QDMR_BINGO_NPCS, ResolveAreaLoopParent);
}
void QdmrWaitNormalRoom(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (!IsMagicRoomReady())
        return;
    HLib::SetCurrentTaskTo(QdmrNormalRoomPresentation);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), "QDMRNRML");
}
void QdmrFirstRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginFirstRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    Loop3DSoundEffect(0x91b);
    QueueAreaLoopCamera("QDMRBTL1", 4000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL1", 4001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL1", 4002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL1", 4003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266E001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A027", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266E002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266E003", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 510);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 205.36f;
    position.z = 166.8f;
    rotation.y = 41214;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_SBK, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_SBK,
                            .flags = 6,
                            .motion = 7241,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 204.94f;
    position.z = 168.56f;
    rotation.y = 58345;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_ANC, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_ANC,
                            .flags = 6,
                            .motion = 7235,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrSecondRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginSecondRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    Loop3DSoundEffect(0x91b);
    QueueAreaLoopCamera("QDMRBTL2", 5000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL2", 5001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL2", 5002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL2", 5003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266F001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A029", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266F002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266F003", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 510);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 254.8f;
    position.z = 156.10001f;
    rotation.y = 40960;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KU8, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KU8,
                            .flags = 6,
                            .motion = 7241,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 255.3f;
    position.z = 157.7f;
    rotation.y = 60074;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KHO, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KHO,
                            .flags = 6,
                            .motion = 7235,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrThirdRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    HLib::SetCurrentTaskTo(QdmrBeginThirdRoomFight);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    Vector3i rotation;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    Vector3 position{0, 0, -0.5f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    position = {0, 0, -0.69999999f};
    scenApplyTransRot(&origin, &rotation, &position);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 0xf002,
                            .duration = 259,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        260);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 6,
                            .motion = 7240,
                            .duration = 89,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 8,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        350);
    const shen_name door{work->door.name & 0xffffffdfU};
    doorLoopRigidMotion(door, 7234, 0, 1);
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (pause) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
        if (motion)
            *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
    }
    HLib::Task *const resume = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
    if (resume) {
        QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(resume));
        if (motion)
            *motion = {.door = door, .frames = 351, .startFrame = -1, .speed = 1, .motion = 7234};
    }
    QueueQdmrLoopSound(0x91b, 90);
    QueueAreaLoopCamera("QDMRBTL3", 6000, QDMR_DUMMY, nullptr, 0);
    QueueAreaLoopCamera("QDMRBTL3", 6001, QDMR_DUMMY, nullptr, 150);
    QueueAreaLoopCamera("QDMRBTL3", 6002, QDMR_DUMMY, nullptr, 260);
    QueueAreaLoopCamera("QDMRBTL3", 6003, QDMR_DUMMY, nullptr, 350);
    QueueAreaLoopVoice("0266G001", 0, -1, nullptr, 150);
    QueueAreaLoopVoice("0266A027", 0, -1, nullptr, 260);
    QueueAreaLoopVoice("0266G002", 0, -1, nullptr, 350);
    QueueAreaLoopVoice("0266G003", 0, -1, nullptr, 430);
    QueueAreaLoopVoice("0266G004", 0, -1, ResolveAreaLoopParent, 510);
    HLib::Task *const fade = HLib::EnqueueTaskWithParameter(QdmrFadeOutSoundCallback, 4, 0x0b, 0, AREA_EVENT_TASK);
    if (fade)
        HLib::SetupTimedTask(fade, 560);
    else
        spmFadeOut();
    work->flags |= 0x20U;
    position.x = 219.02f;
    position.z = 119.31f;
    rotation.y = 12797;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_UYD, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_UYD,
                            .flags = 6,
                            .motion = 7236,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 216.75f;
    position.z = 119.36f;
    rotation.y = 52610;
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KT7, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KT7,
                            .flags = 6,
                            .motion = 7242,
                            .duration = 600,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
}
void QdmrFirstRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 204.66f;
    position.z = 167.3f;
    rotation = {0, 41214, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_SBK, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_SBK,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 205.53999f;
    position.z = 169.25999f;
    rotation = {0, 58345, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_ANC, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_ANC,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndFirstRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        scenApplyTransRot(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrSecondRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 254.3f;
    position.z = 156.5f;
    rotation = {0, 40960, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KU8, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KU8,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 254.60001f;
    position.z = 158.0f;
    rotation = {0, 10922, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KHO, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KHO,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndSecondRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        scenApplyTransRot(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrThirdRepeatPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = 219.02f;
    position.z = 120.21f;
    rotation = {0, 20075, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_UYD, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_UYD,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    position.x = 217.25f;
    position.z = 120.36f;
    rotation = {0, 60802, 0};
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_KT7, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_KT7,
                            .flags = 2,
                            .motion = 7259,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = -1,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndThirdRepeat);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, first == 3 ? 68 : 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        scenApplyTransRot(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrNormalRoomPresentation(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    Vector3 position;
    Vector3i rotation;
    SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndNormalRoom);
    HLib::SetupTaskDelay(-1);
    Vector3 origin;
    rotation.x = 0;
    rotation.z = 0;
    GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
    QueueAreaLoopPosition(
        {.parent = HLib::GetCurrentTask(), .character = QDMR_DUMMY, .flags = 3, .position = origin, .rotation = rotation, .completion = nullptr}, 0);
    const std::int32_t first = shenGetRandomInt32(4);
    const char *const firstVoice = first == 0 ? "0266A008" : first == 1 ? "0266A002" : first == 2 ? "0266A003" : "0266A005";
    QueueAreaLoopVoice(firstVoice, 0, -1, nullptr, 40);
    const std::int32_t second = shenGetRandomInt32(4);
    const char *const secondVoice = second == 1 ? "0266A004" : second == 0 || second == 2 ? "0266A007" : "0266A001";
    QueueAreaLoopVoice(secondVoice, 0, -1, ResolveAreaLoopParent, 100);
    const shen_name door{work->door.name & 0xffffffdfU};
    if (work->doorQteButtons & 0x400U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 80);
        position = {1.4f, 0, 0.80000001f};
        scenApplyTransRot(&origin, &rotation, &position);
        rotation.y += 0x4000;
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7233,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 35,
                                .endFrame = -1,
                                .speed = 0.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 4,
                                .motion = 7238,
                                .duration = 10,
                                .elapsed = 0,
                                .startFrame = 140,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 16,
                                .reserved2c = 0,
                                .completion = nullptr},
            71);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 110,
                                .elapsed = 0,
                                .startFrame = 148,
                                .endFrame = -1,
                                .speed = 0.80000001f,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            80);
        QueueAreaLoopCamera("QDMRNRML", 3100, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3101, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3102, QDMR_DUMMY, nullptr, 80);
        doorLoopRigidMotion(door, 7234, 0, 1.6f);
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 15, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else if (work->doorQteButtons & 4U) {
        position = {-0.40000001f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7238,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 40,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 30);
        QueueAreaLoopCamera("QDMRNRML", 3200, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3201, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3202, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 30, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 45, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    } else {
        position = {-0.1f, 0, 1.5f};
        scenApplyTransRot(&origin, &rotation, &position);
        QueueAreaLoopPosition(
            {.parent = HLib::GetCurrentTask(), .character = QDMR_PLAYER, .flags = 3, .position = position, .rotation = rotation, .completion = nullptr}, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 6,
                                .motion = 7239,
                                .duration = 70,
                                .elapsed = 0,
                                .startFrame = 70,
                                .endFrame = -1,
                                .speed = 1.5,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        QueueQdmrLoopSound(0x91b, 13);
        QueueAreaLoopCamera("QDMRNRML", 3000, QDMR_DUMMY, nullptr, 0);
        QueueAreaLoopCamera("QDMRNRML", 3001, QDMR_DUMMY, nullptr, 40);
        QueueAreaLoopCamera("QDMRNRML", 3002, QDMR_DUMMY, nullptr, 81);
        HLib::Task *const open = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (open) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(open));
            if (motion)
                *motion = {.door = door, .frames = 12, .startFrame = 0, .speed = 1, .motion = 7234};
        }
        HLib::Task *const pause = HLib::EnqueueTaskWithParameter(QdmrDoorMotionCallback, 4, 0x0b, sizeof(QdmrDoorMotionWork), AREA_EVENT_TASK);
        if (pause) {
            QdmrDoorMotionWork *const motion = static_cast<QdmrDoorMotionWork *>(HLib::GetTaskParameter(pause));
            if (motion)
                *motion = {.door = door, .frames = 27, .startFrame = 15, .speed = 0, .motion = 7234};
        }
    }
    work->doorQteButtons = 0;
}
void QdmrBeginFirstRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF1B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_FIRST_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_FIRST_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_FIRST_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_FIRST_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrFirstRoomFight);
}
void QdmrFirstRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive() || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->flags |= 1U;
        return;
    }
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF1B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB1, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_FIRST_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL1"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndFirstRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_FIRST_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrBeginSecondRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF2B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_SECOND_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_SECOND_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_SECOND_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_SECOND_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrSecondRoomFight);
}
void QdmrSecondRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
            spmFadeOut();
            work->flags |= 0x20U;
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF2B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB2, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_SECOND_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL2"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndSecondRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_SECOND_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrBeginThirdRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    EV_LoadAudio("0266A000", 3);
    EV_spmBeginSampleExecute();
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QF3B, 0);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_THIRD_ROOM_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_THIRD_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_THIRD_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_THIRD_ROOM_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(2, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrThirdRoomFight);
}
void QdmrThirdRoomFight(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive())
        return;
    if (!(work->flags & 1U)) {
        if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
            QueueQdmrLoopSound(0x85d, 30);
            QueueQdmrLoopSound(0x85e, 50);
            spmFadeOut();
            work->flags |= 0x20U;
        }
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QF3B, 1);
    flddSetPointFlagOnGrid(QDMR_QFB3, 0);
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_THIRD_ROOM_NPCS);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0);
        Vector3 origin;
        Vector3i rotation;
        rotation.x = 0;
        rotation.z = 0;
        GetQdmrDoorPlacement(work->door, &origin, &rotation.y);
        Vector3 position{0.5f, 0, 0.69999999f};
        scenApplyTransRot(&origin, &rotation, &position);
        EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rotation.y += 0x8000;
        EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        HLib::SetCurrentTaskTo(QdmrInFree);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    } else {
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 2,
                                .motion = 0xf002,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBTL3"), 0);
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        HLib::SetCurrentTaskTo(QdmrBeginRetry);
    }
}
void QdmrEndThirdRepeat(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    if (!doorLoadAuthorSilently({}))
        return;
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_NPC_NewLoadSw(0);
    RestoreAreaLoopCharacters(QDMR_THIRD_ROOM_NPCS);
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    EV_LoadAudio("0266A000", 2);
}
void QdmrEndNormalRoom(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        QueueQdmrLoopSound(0x85d, 30);
        QueueQdmrLoopSound(0x85e, 50);
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    EV_FreeAuthorPak(0, "0266");
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    LeaveMagicRoom();
    doorLoopRigidMotion(work->door, 7234, -1, 1);
    ReleaseDoor(work->door.name, QDMR_PLAYER);
    MarkQdmrRoomVisited(work->door, work->roomFlags);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRNRML"), 0);
    EV_AT_SetPos(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
    EV_AT_SetAngY(QDMR_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, work->savedYaw, 0}, nullptr);
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 0,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    HLib::SetCurrentTaskTo(QdmrInFree);
    HLib::SetupTaskDelay(10);
}
void QdmrWaitBingoNpcs(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != work->door.name)
            continue;
        if (QDMR_ROOMS[index].package.name) {
            if (!doorLoadAuthorSilently(QDMR_ROOMS[index].package))
                return;
            EV_NPC_NewLoadSw(0);
        }
        break;
    }
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1U;
    HLib::SetCurrentTaskTo(QdmrEndBingoOpening);
    HLib::SetupTaskDelay(-1);
    QueueAreaLoopAuthor("QDMRBGNB", QDMR_BINGO_OPEN_AUTHOR, QdmrBingoOpeningAuthorCompletion);
}
void QdmrEndBingoOpening(void *parameter) {
    using enum CameraState;
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    EV_FreeAuthorPak(0, "0266");
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 0);
    flddSetPointFlagOnGrid(QDMR_QF0X, 0);
    flddSetPointFlagOnGrid(QDMR_PROP, 0);
    flddSetPointFlagOnGrid(QDMR_QFIX, 0);
    flddSetPointFlagOnGrid(QDMR_QFBA, 1);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 1);
    shenInitFightData(
        ctrlGetCharaByName(QDMR_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100U * (static_cast<std::uint32_t>(work->battleRetries) + 2U)), 0);
    for (std::size_t index = 0; QDMR_BINGO_NPCS[index].name; ++index) {
        shenInitFightData(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), nullptr, 1, 1, static_cast<std::uint16_t>(QDMR_BINGO_ROOM_HEALTH[index]),
            static_cast<std::uint8_t>(QDMR_BINGO_ROOM_STYLES[index]));
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), 0, 0x400000, QDMR_BINGO_NPCS[index].name == QDMR_KF4.name ? 32 : 128);
        vfUpdateFlagsFor(ctrlGetCharaByName(QDMR_BINGO_NPCS[index]), 0, 0x200000, 2048);
    }
    vfmQueueFightStart(4, 0x63, 1, 0, -1);
    HLib::SetCurrentTaskTo(QdmrBingoFight);
    work->flags &= ~1U;
}
void QdmrBingoFight(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
        return;
    }
    if (IsFightActive() || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 1U)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
        return;
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
    flddSetPointFlagOnGrid(QDMR_DEFAULT_LIGHT, 1);
    flddSetPointFlagOnGrid(QDMR_QF0X, 1);
    flddSetPointFlagOnGrid(QDMR_PROP, 1);
    flddSetPointFlagOnGrid(QDMR_QFIX, 1);
    flddSetPointFlagOnGrid(QDMR_QFBA, 0);
    if (GetBtlStatus(ctrlGetCharaByName(QDMR_PLAYER)) == 3) {
        SetQdmrPlayerHandState(work, true);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 1);
        EV_FreeAuthorPak(1, "0266");
        HLib::SetCurrentTaskTo(QdmrBingoWin);
        HLib::SetupTaskDelay(-1);
        EV_DisableCharacterDisplay(QDMR_P02);
        EV_DisableCharacterDisplay(QDMR_P07);
        EV_DisableCharacterDisplay(QDMR_P09);
        spmFadeOut();
        work->flags |= 0x20U;
        QueueAreaLoopAuthor("QDMRQITB", QDMR_BINGO_WIN_AUTHOR, QdmrBingoWinAuthorCompletion);
    } else {
        HLib::SetCurrentTaskTo(QdmrBingoLose);
        Queue_ST_LoadTitleFadeIn(QDMR_RETRY_TITLE);
        HLib::SetupTaskDelay(50);
    }
}
void QdmrBingoWin(void *parameter) {
    using enum CameraState;
    RestoreAreaLoopCharacters(QDMR_BINGO_NPCS);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0);
    EV_FreeAuthorPak(0, "0266");
    SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
    if (ctrlGetCharaByName(QDMR_PLAYER)) {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    if (parameter)
        ExecuteCleanupFuncForAreaWithCachedScene();
    QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                            .character = QDMR_PLAYER,
                            .flags = 2,
                            .motion = 0xf002,
                            .duration = -1,
                            .elapsed = 0,
                            .startFrame = 0,
                            .endFrame = -1,
                            .speed = 1,
                            .frameTime = 0,
                            .reserved2c = 0,
                            .completion = nullptr},
        0);
    EV_stepSetFlags(0, 0x12a);
    EV_stepSetFlags(0, 0x135);
    HLib::SetCurrentTaskTo(QdmrDoneCallback);
}
void QdmrBingoLose(void *parameter) {
    if (!doorLoadAuthorSilently({}))
        return;
    ST_LoadTitleFade();
    RestoreAreaLoopCharacters(QDMR_BINGO_NPCS);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMRBBTL"), 0);
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (work) {
        work->battleRetries = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->battleRetries) + 1U);
        QdmrOpenBingoRoom(work);
    } else
        HLib::SetCurrentTaskTo(QdmrDoneCallback);
}
} // namespace
const ScriptAreaDefinition qdmrDefinition{.areaId = QDMR_AREA, .field04 = 0, .init = QdmrAdmissionNoop, .main = QdmrMain};
} // namespace shenmue::s2
#endif
