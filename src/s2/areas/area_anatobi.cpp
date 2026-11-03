#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_anatobi.hpp"

Vector3 anaTobiStartPosition = {248.97f, 121.6f, 160.58f};
float anaTobiStartYaw = 4.894409f;
Vector3 anaTobiNoScale = {-1.0f, -1.0f, -1.0f};
int anaTobiReserved = 0;
shen_name anaTobiEnemyNames[] = {SHEN_NAME4('I', 'K', 'O', '_'), SHEN_NAME4('V', 'U', 'N', '_'), SHEN_NAME4('S', 'J', 'N', '_'),
    SHEN_NAME4('F', 'U', 'Y', '_'), SHEN_NAME4('S', 'B', 'O', '_'), 0};
shen_name anaTobiGuardNames[] = {SHEN_NAME4('M', 'O', 'N', '_'), SHEN_NAME4('S', 'C', 'X', '_'), SHEN_NAME4('G', 'E', 'N', '_'),
    SHEN_NAME4('R', 'K', 'N', '_'), SHEN_NAME4('I', 'I', 'I', '_'), SHEN_NAME4('S', 'E', 'E', '_'), SHEN_NAME4('M', 'O', 'K', '_'),
    SHEN_NAME4('C', 'H', 'R', '_'), SHEN_NAME4('E', 'H', 'Y', '_'), 0};
AnaTobiEnemy anaTobiEnemies[] = {{0, {237.69f, 121.6f, 166.25f}, 49253, 5, 70, 1009, 1, 0}, {1, {236.85f, 121.6f, 167.75f}, 49310, 10, 70, 0, 0, 0},
    {2, {233.03f, 121.6f, 137.47f}, 35306, 5, 90, 1010, 3, 0}, {3, {235.24f, 121.6f, 138.32f}, 33679, 10, 90, 0, 0, 0},
    {4, {237.28f, 121.6f, 136.56f}, 27939, 15, 90, 0, 0, 0}, {0, {223.13f, 121.6f, 179.46f}, 226, 2, 90, 1011, 4, 0},
    {1, {224.19f, 121.6f, 177.33f}, 25445, 4, 90, 0, 0, 0}, {2, {223.05f, 121.6f, 177.17f}, 40998, 6, 90, 0, 0, 0},
    {3, {190.19f, 121.6f, 155.65f}, 10836, 15, 90, 1012, 5, 0}, {4, {188.8f, 121.6f, 155.95f}, 53111, 20, 90, 0, 0, 0},
    {0, {189.66f, 121.6f, 156.99f}, 16068, 25, 90, 0, 0, 0}, {1, {189.66f, 121.6f, 155.99f}, 16068, 25, 90, 0, 0, 0}};
AnaTobiPoint anaTobiPoints[] = {{ANATOBI_POINT_PUT, 0, {254.69f, 121.6f, 167.38f}, 18072, 0},
    {ANATOBI_POINT_ENEMY, 0, {0.0f, 0.0f, 0.0f}, 0, (const char *)0}, {ANATOBI_POINT_ENEMY, 0, {0.0f, 0.0f, 0.0f}, 0, (const char *)1},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {249.48f, 121.6f, 165.92f}, 65479, "Start Position"},
    {ANATOBI_POINT_HOLE, 0, {249.31f, 121.6f, 155.93f}, 0, "HOLE1"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {247.59f, 121.6f, 147.91f}, 14279, "curve 01"},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)2},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)3},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)4},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {225.46f, 121.6f, 147.54f}, 30465, "curve 02B"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {223.02f, 121.6f, 155.4f}, 29921, "curve 02-1"},
    {ANATOBI_POINT_HOLE, 0, {220.34f, 121.6f, 167.76f}, 32767, "HOLE2"},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)5},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)7},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {217.18f, 121.6f, 172.54f}, 21794, "curve 03"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {208.69f, 121.6f, 177.8f}, 16599, "curve 04B"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {199.88f, 121.6f, 176.95f}, 2341, "curve 05"},
    {ANATOBI_POINT_HOLE, 0, {198.91f, 121.6f, 162.78f}, 483, "HOLE3"},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)8},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)9},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)10},
    {ANATOBI_POINT_ENEMY, 0, {230.92f, 121.6f, 136.3f}, 10, (const char *)11},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {199.66f, 121.6f, 148.89f}, 50236, "curve 06"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {215.51f, 121.6f, 147.8f}, 64356, "curve 07B"},
    {ANATOBI_POINT_HOLE, 0, {216.89f, 121.6f, 130.28f}, 99, "HOLE4"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {218.6f, 121.6f, 126.41f}, 52412, "curve 08B"},
    {ANATOBI_POINT_HOLE, 0, {234.69f, 121.6f, 126.13f}, 49137, "HOLE5"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {240.66f, 121.6f, 126.6f}, 48238, "elev.root"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {246.15f, 121.6f, 126.88f}, 34136, "elev.curve"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {247.64f, 121.6f, 134.16f}, 32503, "last.root"},
    {ANATOBI_POINT_MOVE_DIRECT, 0, {248.32f, 121.6f, 138.97f}, 47061, "last.curve"},
    {ANATOBI_POINT_CURVE, 0, {254.24f, 121.6f, 140.4f}, 46557, 0}, {ANATOBI_POINT_END, 0, {0.0f, 0.0f, 0.0f}, 0, 0}};
int anaTobiReservedBss[2] __attribute__((section(".bss")));
int anaTobiUnusedCommon[3];
int anaTobiCourseIndex;
char anaTobiTextBuffer[68];
AnaTobiWork anaTobiWork;
AnaTobiLoader anaTobiLoader;
char anaTobiLogText[ANATOBI_DEBUG_LINES][ANATOBI_DEBUG_LINE_LENGTH];

void StartAnaTobi(int argument)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.DebugLogToFileRemoved("==== AnaTobi ====\n");
    AnaTobiDebugInit();
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANATOBI_RYO), 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(ANATOBI_RYO), 0, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANATOBI_RYO), 0xf008, 0);
    anaTobiWork.phase = 0;
    anaTobiWork.argument = argument;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    AnaTobiLoadNpcs(0, anaTobiEnemyNames, QueueAnaTobiMain);
}

void QueueAnaTobiMain(void)
{
    FTBL.HLib_EnqueueTaskWithoutParameter(AnaTobiLoad, 6, 13, TASK_EVNT);
}

void AnaTobiLoad(void *data)
{
    Vector3 scale;
    switch (anaTobiWork.phase) {
    case 0:
        AnaTobiDebugPrint("PACK LOAD");
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)ANATOBI_PACKAGE, 0);
        anaTobiWork.phase++;
        break;
    case 1:
        FTBL.DebugLogToFileRemoved(".");
        if (!FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
            anaTobiWork.phase = 4;
        break;
    case 4:
        AnaTobiDebugPrint("PACK LOAD FINISH");
        FTBL.EV_ControlPackageResources(2, 4, ANATOBI_PACKAGE, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, ANATOBI_PACKAGE, 0, 0);
        FTBL.EV_SetCharaControllerFlags(ANATOBI_RYO, 1, 0);
        FTBL.EV_EnableCharacterDisplay(ANATOBI_RYO);
        FTBL.SetRefBoardParamsForCharaById(ANATOBI_HOLE, 1);
        scale.x = 9.0f;
        scale.y = 9.0f;
        scale.z = 9.0f;
        FTBL.mtwkSetMotionworkScale3(ANATOBI_HOLE, &scale);
        AnaTobiSetTask(AnaTobiNextPoint);
        anaTobiCourseIndex = 0;
        anaTobiWork.reserved01 = 0;
        break;
    }
}

void AnaTobiNextPoint(void *data)
{
    short kind;
    anaTobiWork.point = &anaTobiPoints[anaTobiCourseIndex++];
    kind = anaTobiWork.point->kind;
    FTBL.DebugLogToFileRemoved("trap:%d\n", kind);
    switch (kind) {
    case ANATOBI_POINT_HOLE:
        AnaTobiSetTask(AnaTobiHoleInit);
        break;
    case ANATOBI_POINT_MOVE:
        AnaTobiSetTask(AnaTobiMoveInit);
        break;
    case ANATOBI_POINT_MOVE_DIRECT:
        AnaTobiSetTask(AnaTobiMoveDirectInit);
        break;
    case ANATOBI_POINT_CURVE:
        AnaTobiSetTask(AnaTobiCurveInit);
        break;
    case ANATOBI_POINT_PUT:
        AnaTobiSetTask(AnaTobiPutInit);
        break;
    case ANATOBI_POINT_ENEMY:
        AnaTobiSetTask(AnaTobiEnemyInit);
        break;
    default:
        FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, 0);
        AnaTobiSetTask(AnaTobiExit);
        break;
    }
}

void AnaTobiHoleInit(void *data)
{
    Vector3 position;
    Vector3i rotation;
    FTBL.DebugLogToFileRemoved("Run:\n");
    AnaTobiDebugPrint(anaTobiWork.point->name);
    AnaTobiPlaceBase();
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 7.0f;
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 17, 0);
    FTBL.EV_AT_SetPos(ANATOBI_RYO, ANATOBI_POSITION_FLAGS, &position);
    FTBL.EV_AT_SetAngY_0(ANATOBI_RYO, ANATOBI_POSITION_FLAGS, &rotation, 0);
    FTBL.EV_AT_SetPos(ANATOBI_HOLE, ANATOBI_POSITION_FLAGS, &anaTobiWork.point->position);
    rotation.x = ANATOBI_ANGLE(anaTobiWork.point->yaw);
    rotation.y = 0x8000;
    rotation.z = 0xc000;
    FTBL.EV_AT_SetAngY_0(ANATOBI_HOLE, ANATOBI_POSITION_FLAGS, &rotation, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANATOBI_RYO), 0xf003, 0);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANATOBI_RYO), 1.0f, 0);
    AnaTobiDebugPrint("Motion Camera");
    AnaTobiSetTask(AnaTobiHoleJump);
}

void AnaTobiHoleJump(void *data)
{
    Vector3 position;
    unsigned int key;
    FTBL.GetCharaPosition(ANATOBI_RYO, -1, &position, 0);
    anaTobiWork.frames++;
    switch (anaTobiWork.phase) {
    case 0:
        if (anaTobiWork.frames == 2) {
            FTBL.SetShowQteFlagForButtonType(256, 25, 0);
            anaTobiWork.phase++;
        }
        break;
    case 1:
        if (position.z < 1.5f) {
            key = FTBL.GetCurrentQteStatus() & 0xfff;
            sprintf(anaTobiTextBuffer, "Key:%4X", (unsigned short)key);
            AnaTobiDebugPrint(anaTobiTextBuffer);
            if (key == 256) {
                AnaTobiDebugPrint("QTE:O.K.");
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANATOBI_RYO), 8392, 0);
                position.x = 1.5f;
                position.y = 1.5f;
                position.z = 1.5f;
                FTBL.SetCharaTargetTransformOffset(ANATOBI_RYO, &position, 1);
                FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANATOBI_RYO), 17.0f, 0);
                anaTobiWork.phase++;
            } else {
                AnaTobiDebugPrint("QTE:N.G.");
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANATOBI_RYO), 8390, 0);
                position.x = 1.0f;
                position.y = 1.0f;
                position.z = 1.5f;
                FTBL.SetCharaTargetTransformOffset(ANATOBI_RYO, &position, 1);
                FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANATOBI_RYO), 10.0f, 0);
                FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
                anaTobiWork.phase = 10;
            }
        }
        break;
    case 2:
        if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(ANATOBI_RYO), 0) > 40.0f)
            anaTobiWork.phase += 2;
        break;
    case 3:
        if (position.z < -3.0f)
            anaTobiWork.phase++;
        break;
    case 4:
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        AnaTobiDebugPrint("TRACE CAMERA");
        AnaTobiSetTask(AnaTobiNextPoint);
        break;
    case 10:
        if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(ANATOBI_RYO), 0) > 140.0f) {
            anaTobiWork.phase += 2;
            anaTobiCourseIndex = 0;
            FTBL.SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
            AnaTobiShowMessage(10);
            anaTobiWork.phase = 4;
        }
        break;
    case 11:
        if (position.z < -3.0f)
            anaTobiWork.phase = 4;
        break;
    }
}

void AnaTobiMoveInit(void *data)
{
    FTBL.DebugLogToFileRemoved("Move:\n");
    AnaTobiDebugPrint("MOVE:");
    AnaTobiSetTask(AnaTobiMoveNext);
    FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, EVENT_CHARACTER_INVALID);
}

void AnaTobiMoveNext(void *data)
{
    AnaTobiSetTask(AnaTobiNextPoint);
}

void AnaTobiMoveDirectInit(void *data)
{
    FTBL.DebugLogToFileRemoved("MoveDirect:\n");
    AnaTobiDebugPrint(anaTobiWork.point->name);
    FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, EVENT_CHARACTER_INVALID);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(ANATOBI_RYO), &anaTobiWork.point->position, anaTobiWork.point->yaw, 5, 0);
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(ANATOBI_RYO), 0, 0, 8, 0);
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(ANATOBI_RYO), 0, 0, 11, 0);
    AnaTobiSetTask(AnaTobiMoveDirectWait);
}

void AnaTobiMoveDirectWait(void *data)
{
    if (!FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 58, 1))
        AnaTobiSetTask(AnaTobiNextPoint);
}

void AnaTobiCurveInit(void *data)
{
    if (0)
        AnaTobiDebugPrint("PCKTM01G");
    AnaTobiDebugPrint(anaTobiWork.point->name);
    FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, EVENT_CHARACTER_INVALID);
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(ANATOBI_RYO), &anaTobiWork.point->position, anaTobiWork.point->yaw, 6, 0);
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(ANATOBI_RYO), 0, 0, 11, 0);
    AnaTobiSetTask(AnaTobiCurveWait);
}

void AnaTobiCurveWait(void *data)
{
    if (!FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 58, 1))
        AnaTobiSetTask(AnaTobiNextPoint);
}

void AnaTobiEnemyInit(void *data)
{
    Vector3 position;
    Vector3i rotation;
    int index, character;
    HLibTask *task;
    AnaTobiEnemyWork *work;
    index = (int)anaTobiWork.point->name;
    character = anaTobiEnemies[index].character;
    AnaTobiDebugPrint("Enemy");
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), 17, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), 71, 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), 0, 0);
    FTBL.EV_AT_SetPos(anaTobiEnemyNames[character], ANATOBI_POSITION_FLAGS, &anaTobiEnemies[index].position);
    rotation.x = 0;
    rotation.y = ANATOBI_ANGLE(anaTobiEnemies[index].yaw);
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(anaTobiEnemyNames[character], ANATOBI_POSITION_FLAGS, &rotation, 0);
    AnaTobiSetTask(AnaTobiNextPoint);
    task = FTBL.HLib_EnqueueTaskWithParameter(AnaTobiEnemyRun, 6, 11, sizeof(AnaTobiEnemyWork), TASK_EVNT);
    work = (AnaTobiEnemyWork *)FTBL.HLib_GetTaskParameter(task);
    work->frames = 0;
    work->character = character;
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), 8388, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), 0, 0);
    if (anaTobiEnemies[index].camera) {
        FTBL.EVS_LoopCamera(anaTobiEnemies[index].camera, 0, 0);
        FTBL.GetCharaPosition(ANATOBI_RYO, -1, &position, ANATOBI_TARGET_FLAGS);
        FTBL.RequestMotionTargetFacing(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), &position, 1);
    }
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(anaTobiEnemyNames[character]), &position, 1);
}

void AnaTobiEnemyRun(void *data)
{
    AnaTobiEnemyWork *work = (AnaTobiEnemyWork *)data;
    Vector3 position;
    Vector3i rotation;
    int frames, mirror;
    frames = work->frames++;
    FTBL.SetDebugScreenTextPosition(35, work->character + 10);
    FTBL.DebugLogToScreen("(%d)", work->character);
    if (frames == anaTobiEnemies[work->character].messageFrame) {
        AnaTobiShowMessage(anaTobiEnemies[work->character].message);
        FTBL.GetCharaPosition(ANATOBI_RYO, -1, &position, ANATOBI_TARGET_FLAGS);
        FTBL.GetScenarioCharacterEulerAngles(ANATOBI_RYO, &rotation, ANATOBI_TARGET_FLAGS);
        FTBL.EV_SetCharaControllerFlags(anaTobiEnemyNames[work->character], 2, 1);
        FTBL.EV_SetCharaControllerFlags(anaTobiEnemyNames[work->character], 3, 1);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), &position, rotation.y, 0, 0);
    }
    if (frames >= anaTobiEnemies[work->character].messageFrame + 5 && (mirror = frames & 3) == 0) {
        FTBL.GetCharaPosition(ANATOBI_RYO, -1, &position, ANATOBI_TARGET_FLAGS);
        FTBL.GetScenarioCharacterEulerAngles(ANATOBI_RYO, &rotation, ANATOBI_TARGET_FLAGS);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), &position, rotation.y, 7, mirror);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), 0, 0, 8, mirror);
    }
    if (frames == anaTobiEnemies[work->character].endFrame) {
        if (anaTobiEnemies[work->character].camera) {
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
        }
        FTBL.RequestMotionTargetFacing(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), 0, 0);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), 0, 0, 1, 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(anaTobiEnemyNames[work->character]), 8388, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void AnaTobiPutInit(void *data)
{
    Vector3 position;
    Vector3i rotation;
    FTBL.DebugLogToFileRemoved("Put:\n");
    AnaTobiDebugPrint("PUT:");
    FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, EVENT_CHARACTER_INVALID);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANATOBI_RYO), 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(ANATOBI_RYO), 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 17, 0);
    position.x = anaTobiWork.point->position.x;
    position.y = anaTobiWork.point->position.y;
    position.z = anaTobiWork.point->position.z;
    rotation.x = 0;
    rotation.y = ANATOBI_ANGLE(anaTobiWork.point->yaw);
    rotation.z = 0;
    FTBL.EV_AT_SetPos(ANATOBI_RYO, ANATOBI_POSITION_FLAGS, &position);
    FTBL.EV_AT_SetAngY_0(ANATOBI_RYO, ANATOBI_POSITION_FLAGS, &rotation, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANATOBI_RYO), 0xf003, 0);
    AnaTobiSetTask(AnaTobiNextPoint);
}

void AnaTobiShowMessage(int message)
{
    switch (message) {
    case 1:
        QueueEventMessage("いたぞ、あいつらだ！", 0, 58);
        QueueEventMessage("●青やべっ！", 60, 58);
        break;
    case 2:
        QueueEventMessage("●青よっ！", 0, 58);
        break;
    case 3:
        QueueEventMessage("あいつらだ、追え！", 0, 58);
        break;
    case 4:
        QueueEventMessage("見つけたぞ。逃がすな！", 0, 58);
        break;
    case 5:
        QueueEventMessage("あいつらだ、逃がすな！", 0, 58);
        break;
    case 10:
        QueueEventMessage("●青くそっ。しくじったぜ。", 0, 58);
        break;
    }
}

void AnaTobiExit(void *data)
{
    FTBL.DebugLogToFileRemoved("EXIT\n");
    AnaTobiFreeNpcs();
    FTBL.EV_ControlPackageResources(0, 6, ANATOBI_PACKAGE, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
    FTBL.HLib_DestroyTaskInternal(anaTobiWork.debugTask);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetPlayerControlEnabled(1);
}

void AnaTobiPlaceBase(void)
{
    Vector3 position;
    Vector3i rotation;
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 17, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ANATOBI_RYO), 71, 0, 0);
    FTBL.AT_SetLocalBaseChara(ANATOBI_RYO, ANATOBI_BASE);
    position.x = anaTobiWork.point->position.x;
    position.y = anaTobiWork.point->position.y;
    position.z = anaTobiWork.point->position.z;
    rotation.x = 0;
    rotation.y = ANATOBI_ANGLE(anaTobiWork.point->yaw);
    rotation.z = 0;
    FTBL.EV_AT_SetPos(ANATOBI_BASE, ANATOBI_POSITION_FLAGS, &position);
    FTBL.EV_AT_SetAngY_0(ANATOBI_BASE, ANATOBI_POSITION_FLAGS, &rotation, 0);
}

void AnaTobiSetTaskDelayed(HLibTaskCallback callback, int delay)
{
    anaTobiWork.phase = 0;
    anaTobiWork.timer = 0;
    anaTobiWork.frames = 0;
    FTBL.HLib_SetCurrentTaskTo(callback);
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}

void AnaTobiSetTask(HLibTaskCallback callback)
{
    anaTobiWork.phase = 0;
    anaTobiWork.timer = 0;
    anaTobiWork.frames = 0;
    FTBL.HLib_SetCurrentTaskTo(callback);
}

void AnaTobiDebugDraw(void *data)
{
    int i;
    for (i = 0; i < ANATOBI_DEBUG_LINES; i++) {
        FTBL.SetDebugScreenTextPosition(1, i + 10);
        FTBL.DebugLogToScreen(anaTobiLogText[i]);
    }
}

void AnaTobiDebugPrint(const char *text)
{
    int i;
    for (i = 0; i < ANATOBI_DEBUG_LINES - 1; i++)
        strcpy(anaTobiLogText[i], anaTobiLogText[i + 1]);
    if (text)
        FTBL.DebugLogToFileRemoved(text);
    else
        strcpy(anaTobiLogText[ANATOBI_DEBUG_LINES - 1], "");
}

void AnaTobiDebugInit(void)
{
    int i;
    for (i = 0; i < ANATOBI_DEBUG_LINES; i++)
        strcpy(anaTobiLogText[i], "");
    anaTobiWork.debugTask = FTBL.HLib_EnqueueTaskWithoutParameter(AnaTobiDebugDraw, 6, 13, TASK_EVNT);
}

void AnaTobiNpcLoadWait(void *data)
{
    int i;
    if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
        return;
    if (anaTobiLoader.names) {
        for (i = 0; anaTobiLoader.names[i]; i++) {
            if (!FTBL.ForceLoadNpc(anaTobiLoader.names[i])) {
                sprintf(anaTobiTextBuffer, "Load:%d", i);
                AnaTobiDebugPrint(anaTobiTextBuffer);
                return;
            }
        }
    }
    anaTobiLoader.callback();
    FTBL.HLib_DestroyCurrentTask();
}

void AnaTobiLoadNpcs(const char *package, shen_name *names, void (*callback)(void))
{
    int i;
    anaTobiLoader.callback = callback;
    anaTobiLoader.names = names;
    anaTobiLoader.package = package;
    if (package)
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)anaTobiLoader.package, 0);
    if (anaTobiLoader.names) {
        for (i = 0; anaTobiLoader.names[i]; i++) {
            FTBL.CheckWarmStartNpcRoute(anaTobiLoader.names[i]);
            FTBL.ForceLoadNpc(anaTobiLoader.names[i]);
        }
    }
    FTBL.HLib_EnqueueTaskWithoutParameter(AnaTobiNpcLoadWait, 6, 13, TASK_EVNT);
}

void AnaTobiFreeNpcs(void)
{
    int i;
    if (anaTobiLoader.package)
        FTBL.EV_ControlPackageResources(0, 6, anaTobiLoader.package, 0, 0);
    if (anaTobiLoader.names) {
        for (i = 0; anaTobiLoader.names[i]; i++)
            FTBL.npcLockUnlock(anaTobiLoader.names[i]);
    }
}
#else
#endif
