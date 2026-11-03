#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_mqte.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunMqte(void)
{
    forestTestObstacle = 0;
    forestTestSelection = 0;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_EventSetUpHelpIcon_0(35, 0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    QueueClearNpcs(QueueForestQte, 0);
}
#else
#define MQTE_FREE_PACKAGE(name)                                         \
    if (FTBL.EV_ControlPackageResources(0, 5, name, 0, 0))              \
        FTBL.EV_ControlPackageResources(0, 6, name, 0, 0)
#define MQTE_LOAD_PACKAGE(name) FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)name, 0)

const ForestQteCommand forestQteCommands[7] = {{4, DodgeLeftSuccess, DodgeLeftFailure, 4.0f, 1.2f}, {0x40, DodgeRightSuccess, DodgeRightFailure, 4.0f, 1.2f},
    {0x400, HurdleJumpSuccess, HurdleJumpFailure, 4.0f, 1.0f}, {0x100, PitJumpSuccess, PitJumpFailure, 4.0f, 2.0f},
    {1, RootJumpSuccess, RootJumpFailure, 4.0f, 1.5f}, {0x10, DuckSuccess, DuckFailure, 4.0f, 1.5f}, {0x10, DuckSuccess, DuckFailure, 4.0f, 1.5f}};
const ForestShenfaMotion forestShenfaMotions[7] = {{7517, 24, 5.0f, 1.5f, 1.0f, 0}, {7517, 24, 5.0f, 1.5f, 1.0f, 1}, {7516, 45, 0.0f, 1.5f, 1.1f, 0},
    {7515, 28, 0.0f, 1.1f, 1.3f, 0}, {7512, 30, 0.0f, 1.0f, 1.2f, 0}, {7501, 37, 5.0f, 1.0f, 1.5f, 0}, {7501, 37, 5.0f, 1.0f, 1.5f, 0}};
const shen_name forestTestObjects[] = {SHEN_NAME4('H', '_', '_', '_'), SHEN_NAME4('H', 'I', '_', '_'), SHEN_NAME4('T', '_', '_', '_'),
    SHEN_NAME4('L', 'R', '_', '_'), SHEN_NAME4('I', '_', '_', '_'), SHEN_NAME4('M', '_', '_', '_'), SHEN_NAME4('H', 'T', '_', '_'),
    SHEN_NAME4('D', '_', '_', '_'), SHEN_NAME4('L', '_', '_', '_'), SHEN_NAME4('T', 'I', '_', '_'), SHEN_NAME4('R', '_', '_', '_'),
    SHEN_NAME4('L', 'R', 'I', '_'), SHEN_NAME4('R', 'L', 'H', '_'), SHEN_NAME4('R', 'L', 'R', '_'), SHEN_NAME4('T', 'D', 'H', '_'),
    SHEN_NAME4('K', '_', '_', '_'), SHEN_NAME4('L', 'R', 'K', '_'), SHEN_NAME4('L', 'R', 'T', '_'), 0};
ForestObstacle forestObstacles[18] = {{0.0f, 0.0f, {{3, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, 0.0f, {{3, 0, 0.0f}, {2, 0, -0.4f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, -3.3f, {{4, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, 1.5f, {{1, 0, 0.4f}, {0, 0, -1.4f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, -2.0f, {{2, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, -1.0f, {{2, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, 0.0f, {{3, 0, 0.0f}, {4, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, -2.0f, {{5, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.4f, 0.0f, {{1, 0, 0.5f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, -4.0f, {{4, 0, 0.0f}, {2, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, -1.5f, {{0, 0, -0.4f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.6f, 1.7f, {{1, 0, 0.0f}, {0, 0, 0.0f}, {2, 0, 0.0f}, {7, 0, 0.0f}}}, {-0.4f, 1.7f, {{0, 0, 0.0f}, {1, 0, 0.0f}, {3, 0, 0.0f}, {7, 0, 0.0f}}},
    {-0.3f, 1.6f, {{0, 0, 0.0f}, {1, 0, 0.0f}, {0, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, 2.0f, {{4, 0, 0.0f}, {5, 0, 0.0f}, {3, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, -2.0f, {{6, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}, {7, 0, 0.0f}}}, {0.0f, 1.7f, {{1, 0, 0.0f}, {0, 0, 0.0f}, {6, 0, 0.0f}, {7, 0, 0.0f}}},
    {0.0f, 1.4f, {{1, 0, 0.0f}, {0, 0, 0.0f}, {4, 0, 0.0f}, {7, 0, 0.0f}}}};
shen_name forestCharacters[] = {MQTE_RYO, MQTE_SHENFA, MQTE_INVALID};
HLibTask *forestShenfaTask = 0;
int forestTestObstacle __attribute__((section(".bss")));
int forestTestSelection __attribute__((section(".bss")));
Vector3 forestObstaclePosition __attribute__((section(".bss")));
float forestObstacleDistance __attribute__((section(".bss")));
int forestShenfaTicks __attribute__((section(".bss")));
int forestLastCommand __attribute__((section(".bss")));
HLibTask *forestDummyTask __attribute__((section(".bss")));
int forestFailures __attribute__((section(".bss")));
int forestRetries __attribute__((section(".bss")));
int forestRainStarted __attribute__((section(".bss")));
int forestStage;

void QueueForestQte(void *data)
{
    HLibTask *task;
    ForestQteWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(ForestQteMain, 4, 11, sizeof(ForestQteWork), MQTE_TASK);
    forestDummyTask = FTBL.HLib_EnqueueTaskWithoutParameter(ForestQteDummyControl, 5, 11, MQTE_DUMMY_TASK);
    work = (ForestQteWork *)FTBL.HLib_GetTaskParameter(task);
    memset(work, 0, sizeof(ForestQteWork));
    work->task = task;
    work->phase = 0;
    work->state = 0;
    work->command = 0;
    work->obstacle = 0;
    work->completedUnits = 0;
    work->previousUnit = -1;
    work->character = MQTE_SHENFA;
    work->characters = Queue_EVT_AT_ReadChr(task, forestCharacters);
    forestObstacleDistance = 0.0f;
    forestFailures = 0;
    forestRetries = 0;
    forestRainStarted = 0;
    forestStage = 0;
    EventDebugRegister(MQTE_DEBUG_ID, ForestDebugSkip, work, 10000000);
}

int SetupForest(ForestQteWork *work)
{
    FTBL.EV_SetNewSceneIndex();
    if (g_currentMap == MQTE_AREA) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"MQTETEST", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"MQTE", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "MQTE", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "MQTE", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0517A000");
    FTBL.EV_FreeAuthorPak(1, "0517");
    FTBL.SetLoadingBusy();
    FTBL.CreateEventCharacter(3, 0, MQTE_BASE);
    FTBL.CreateEventCharacter(3, 0, MQTE_CAMERA);
    FTBL.CreateEventCharacter(3, 0, MQTE_DUMMY_RYO);
    FTBL.CreateEventCharacter(3, 0, MQTE_DUMMY_SHENFA);
    EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0);
    EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0);
    FTBL.WSCN_LoadWeatherData(FTBL.Filepath_Gen(0), "W_COND.BIN");
    Queue_EVT_AT_CtrlHandUp_1(MQTE_RYO, 0, (void *)areaLoopHandPoses[MQTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(MQTE_RYO, 1, (void *)areaLoopHandPoses[MQTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(MQTE_SHENFA, 0, (void *)areaLoopHandPoses[MQTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(MQTE_SHENFA, 1, (void *)areaLoopHandPoses[MQTE_HAND_POSE], 0, 0);
    EventDebugCheck(MQTE_DEBUG_ID, 4);
    if (g_currentMap != MQTE_AREA)
        work->phase = 1;
    else
        work->phase = 3;
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    return 0;
}

void ForestDebugSkip(void *data)
{
    ForestQteWork *work = (ForestQteWork *)data;
    work->state = 0;
    MQTE_FREE_PACKAGE("KWW1");
    MQTE_FREE_PACKAGE("KWA1");
    MQTE_FREE_PACKAGE("KWA2");
    MQTE_FREE_PACKAGE("KWA3");
    if (forestStage == 2)
        work->phase = 11;
    else
        work->phase = 1;
    forestFailures = 0;
    forestRetries = 0;
    EventDebugUnregister(MQTE_DEBUG_ID);
    EventDebugRegister(MQTE_DEBUG_ID, ForestDebugSkip, work, 10000000);
    if (forestStage == 2)
        EventDebugGetState(MQTE_DEBUG_ID, 4);
    else
        EventDebugCheck(MQTE_DEBUG_ID, 4);
    forestStage++;
}

int ForestFinish(ForestQteWork *work)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(ForestEnd);
    return 30;
}

void ForestEnd(void *data)
{
    ForestQteWork *work = (ForestQteWork *)data;
    shen_name destination = MQTE_DESTINATION;
    EventDateTime date;
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.GetCurrentDateTime((DateTime *)&date);
    if (date.hour < 19) {
        date.hour = 19;
        date.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    }
    FTBL.FinishLoading();
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.SetScenarioCharacterKillMode(MQTE_DUMMY_RYO, 1);
    FTBL.SetScenarioCharacterKillMode(MQTE_DUMMY_SHENFA, 1);
    FTBL.HLib_DestroyTaskInternal(forestDummyTask);
    if (forestShenfaTask)
        FTBL.HLib_DestroyTaskInternal(forestShenfaTask);
    ET_AT_KillWork(work->characters);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetScenarioCharacterKillMode(MQTE_BASE, 1);
    FTBL.SetScenarioCharacterKillMode(MQTE_CAMERA, 1);
    EventDebugUnregister(MQTE_DEBUG_ID);
    FTBL.EV_FreeAuthorPak(0, "0517");
    MQTE_FREE_PACKAGE("MQTETEST");
    MQTE_FREE_PACKAGE("MQTE");
    FTBL.scenFreeEvent("MQTE.EV3");
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_stepSetFlags(235, 0);
    FTBL.EV_scenQueueLoad(g_actNumber, destination, 0);
}

void ForestQteMain(void *data)
{
    ForestQteWork *work = (ForestQteWork *)data;
    int delay = 0;
    switch (work->phase) {
    case 0:
        delay = SetupForest(work);
        break;
    case 1:
        delay = RunForestAuthor(work, 0);
        break;
    case 2:
        delay = RunForestRoute(work);
        break;
    case 3:
        delay = BeginForestObstacle(work);
        break;
    case 4:
        delay = ApproachForestObstacle(work);
        break;
    case 5:
        delay = BeginForestQte(work);
        break;
    case 6:
        delay = ResumeForestObstacle(work);
        break;
    case 7:
        delay = FinishForestObstacle(work);
        break;
    case 8:
        delay = RunForestAuthor(work, 1);
        break;
    case 10:
        delay = ForestFinalFailure(work);
        break;
    case 9:
        delay = ForestRetry(work);
        break;
    case 11:
        delay = ForestFinish(work);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int PositionForestRunners(ForestQteWork *work)
{
    Vector3 position;
    int yaw;
    FTBL.EV_mazeUtil(forestStage, 14, &position);
    yaw = FTBL.GetMazeObjectData(forestStage, 14);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, position.x, position.y, position.z, 0);
    EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 0);
    FTBL.EV_mazeUtil(forestStage, 18, &position);
    yaw = FTBL.GetMazeObjectData(forestStage, 18);
    EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, position.x, position.y, position.z, 0);
    EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0x40000000U, 0);
    EVT_AT_SetMot(MQTE_SHENFA, MQTE_SHENFA_RUN_MOTION, 0.0f, 1.1f, 0x40000000U, 0);
    return 0;
}

int RunForestRoute(ForestQteWork *work)
{
    int delay = 0;
    int road;
    switch (work->state) {
    case 0:
        forestFailures = 0;
        forestRainStarted = 0;
        work->completedUnits = 0;
        FTBL.clmdUtil(FTBL.ctrlGetCharaByName(MQTE_SHENFA), 20);
        if (!FTBL.EV_GetScreenFilmIsNotShowing())
            EVT_AT_MenCtrl(60, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        PositionForestRunners(work);
        EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0);
        EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0);
        QueueForestRoadCameras(work->completedUnits, 0);
        FTBL.EV_LoadAudio("0517A000", 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->state++;
        break;
    case 1:
        UpdateForestCharacterRoute(MQTE_RYO);
        UpdateForestCharacterRoute(MQTE_SHENFA);
        UpdateForestShenfaSpacing();
        work->obstacle = FTBL.mazeCheckInsideQuad(FTBL.ctrlGetCharaByName(MQTE_SHENFA));
        road = ForestCharacterRoadIndex(MQTE_SHENFA);
        if (road > FTBL.SelectMazeTerrainField(forestStage) - 30) {
            EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
            if ((forestFailures < 5 || forestRetries > 10) && (unsigned int)g_entryPoint < 90) {
                if (forestStage == 2)
                    work->phase = 11;
                else
                    work->phase = 1;
                forestStage++;
                forestFailures = 0;
                forestRetries = 0;
                work->state = 0;
            } else {
                work->phase = 8;
                work->state = 0;
            }
            delay = 60;
        } else if (work->obstacle != -1 && work->previousUnit != work->obstacle
            && FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(MQTE_RYO), 0) == MQTE_RUN_MOTION) {
            work->previousUnit = work->obstacle;
            EnterForestObstacle(work);
        }
        break;
    }
    return delay;
}

void ForestQteDummyControl(void *data)
{
    if (FTBL.DoesScenarioCharacterExist(MQTE_DUMMY_RYO)) {
        RealignForestDummy(MQTE_DUMMY_RYO, MQTE_RYO);
        RealignForestDummy(MQTE_DUMMY_SHENFA, MQTE_SHENFA);
    }
}

int ForestCharacterRoadIndex(shen_name name)
{
    Vector3 position;
    int unit, road;
    unit = road = -1;
    FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    FTBL.mazeFindIndexForCurrentDistanceForMazeFile(&position, &unit, &road);
    return road;
}

void RealignForestDummy(shen_name dummy, shen_name character)
{
    Vector3 position, target;
    Vector3i rotation, targetRotation;
    FTBL.GetCharaPosition(dummy, -1, &position, 0x40000000);
    FTBL.GetCharaPosition(character, -1, &target, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(dummy, &rotation, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(character, &targetRotation, 0x40000000);
    position.x = ForestLerp(target.x, position.x, 0.2f);
    position.z = ForestLerp(target.z, position.z, 0.2f);
    position.y = target.y;
    rotation.y = ForestBlendAngle(targetRotation.y, rotation.y, 0.06f);
    FTBL.EV_AT_SetPos(dummy, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(dummy, 0x78000000, &rotation, 0);
}

void UpdateForestCharacterRoute(shen_name name)
{
    Vector3 next, position;
    Vector3i rotation;
    int road;
    float x, z;
    road = ForestCharacterRoadIndex(name);
    FTBL.EV_mazeUtil(forestStage, road + 1, &next);
    FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0x40000000);
    if (FTBL.CalculatePlanarDistanceBetweenPoints(&next, &position) > 1.5f) {
        x = next.x - position.x;
        z = next.z - position.z;
        rotation.y = ForestBlendAngle((unsigned short)FTBL.InverseFixedAtan2(x, z), rotation.y, 0.1f);
        FTBL.EV_AT_SetAngY_0(name, 0x78000000, &rotation, 0);
    }
}

void UpdateForestShenfaSpacing(void)
{
    Vector3 shenfa, ryo, offset = {1.0f, 1.0f, 1.0f};
    float distance;
    FTBL.GetCharaPosition(MQTE_SHENFA, -1, &shenfa, 0x40000000);
    FTBL.GetCharaPosition(MQTE_RYO, -1, &ryo, 0x40000000);
    distance = FTBL.CalculatePlanarDistanceBetweenPoints(&shenfa, &ryo);
    if (distance > 20.0f)
        offset.z = -1.0f;
    else if (distance > 10.0f)
        offset.z = -0.7f;
    else if (distance > 8.0f)
        offset.z = -0.5f;
    else if (distance > 5.0f)
        offset.z = -0.3f;
    else if (distance > 3.0f)
        offset.z = -0.2f;
    else
        offset.z = 0.0f;
    FTBL.SetCharaTargetTransformOffset(MQTE_SHENFA, &offset, 1);
}

void EnterForestObstacle(ForestQteWork *work)
{
    Vector3 position;
    int yaw;
    FTBL.GetMazeObjectPosition(work->obstacle, &position);
    yaw = FTBL.GetMazeObjectYaw(work->obstacle);
    EVT_AT_SetPos(MQTE_BASE, MQTE_INVALID, position.x, position.y, position.z, 0);
    EVT_AT_SetAngY(MQTE_BASE, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 0);
    SelectForestObstacle(work);
    BeginForestObstacle(work);
    work->state = 0;
}

void SelectForestObstacle(ForestQteWork *work)
{
    switch (FTBL.GetMazeUnitAuthoredName(work->obstacle)) {
    case SHEN_NAME4('1', 'R', 'J', '1'):
        work->obstacle = 0;
        break;
    case SHEN_NAME4('2', 'R', 'A', '1'):
        work->obstacle = 1;
        break;
    case SHEN_NAME4('1', '3', 'T', '1'):
        work->obstacle = 2;
        break;
    case SHEN_NAME4('2', 'O', 'B', '0'):
        work->obstacle = 3;
        break;
    case SHEN_NAME4('1', 'O', 'H', '0'):
        work->obstacle = 4;
        break;
    case SHEN_NAME4('1', 'O', 'H', '1'):
        work->obstacle = 5;
        break;
    case SHEN_NAME4('2', 'O', 'D', '0'):
        work->obstacle = 6;
        break;
    case SHEN_NAME4('1', 'I', 'K', '1'):
        work->obstacle = 7;
        break;
    case SHEN_NAME4('1', 'O', 'L', '0'):
        work->obstacle = 8;
        break;
    case SHEN_NAME4('2', '3', 'C', '1'):
        work->obstacle = 9;
        break;
    case SHEN_NAME4('1', 'O', 'R', '0'):
        work->obstacle = 10;
        break;
    case SHEN_NAME4('3', 'O', 'B', '1'):
        work->obstacle = 11;
        break;
    case SHEN_NAME4('3', 'O', 'D', '1'):
        work->obstacle = 12;
        break;
    case SHEN_NAME4('3', 'O', 'C', '1'):
        work->obstacle = 13;
        break;
    case SHEN_NAME4('3', 'O', 'F', '1'):
        work->obstacle = 14;
        break;
    case SHEN_NAME4('1', 'I', 'K', '2'):
        work->obstacle = 15;
        break;
    case SHEN_NAME4('3', '3', 'A', '1'):
        work->obstacle = 16;
        break;
    case SHEN_NAME4('3', '3', 'E', '1'):
        work->obstacle = 17;
        break;
    default:
        FTBL.EventAssertNullsub(__FILE__, 844);
        break;
    }
}

int BeginForestObstacle(ForestQteWork *work)
{
    int i;
    short kind;
    if (g_currentMap == MQTE_AREA) {
        forestTestObstacle = forestTestSelection;
        work->obstacle = forestTestObstacle;
        for (i = 0; forestTestObjects[i]; i++)
            EVT_AT_CharaDisp(forestTestObjects[i], 0, 1);
        EVT_AT_CharaDisp(forestTestObjects[forestTestObstacle], 0, 0);
        EVT_AT_SetPos(MQTE_BASE, MQTE_INVALID, 0.0f, 10.0f, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_BASE, MQTE_INVALID, 0.0f, 0);
        EVT_AT_SetPos(forestTestObjects[forestTestObstacle], MQTE_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(forestTestObjects[forestTestObstacle], MQTE_BASE, 0.0f, 0);
    }
    EVT_AT_SetPos(work->character, MQTE_BASE, forestObstacles[work->obstacle].x, 0.0f, forestObstacles[work->obstacle].z + 8.0f, 0);
    EVT_AT_SetAngY(work->character, MQTE_BASE, 0.0f, 0);
    if (work->character == MQTE_SHENFA) {
        EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0);
        SetForestObstacleCamera(MQTE_DUMMY_SHENFA, work->obstacle);
        EVT_AT_CharaDisp(MQTE_RYO, 0, 1);
        EVT_AT_SetMot(work->character, MQTE_SHENFA_RUN_MOTION, 0.0f, 1.0f, 0x40000000U, 0);
    } else {
        EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0);
        SetForestObstacleCamera(MQTE_DUMMY_RYO, work->obstacle);
        EVT_AT_CharaDisp(MQTE_RYO, 0, 0);
        EVT_AT_SetMot(work->character, MQTE_RUN_MOTION, 0.0f, 1.0f, 0x40000000U, 0);
    }
    forestLastCommand = 0;
    work->command = 0;
    work->phase = 4;
    if (work->character == MQTE_SHENFA && FTBL.shenGetRandomInt32(3) == 0) {
        kind = forestObstacles[work->obstacle].commands[work->command].kind;
        switch (kind) {
        case 0:
        case 1:
            break;
        case 2:
            EVT_AT_SetVoice("0517B009", 0);
            break;
        case 3:
            EVT_AT_SetVoice("0517B009", 0);
            break;
        case 4:
            EVT_AT_SetVoice("0517B008", 0);
            break;
        case 5:
            EVT_AT_SetVoice("0517B008", 0);
            break;
        }
    }
    return 0;
}

int ApproachForestObstacle(ForestQteWork *work)
{
    int delay = 0;
    short kind;
    int frames;
    unsigned int flags;
    float range;
    UpdateForestObstacleTarget(work);
    if (work->character == MQTE_RYO) {
        range = !work->command ? 6.5f : 5.0f;
        if (range > forestObstacleDistance)
            work->phase = 5;
    } else {
        kind = forestObstacles[work->obstacle].commands[work->command].kind;
        if (forestObstacleDistance < forestShenfaMotions[kind].distance) {
            frames = (forestShenfaMotions[kind].end - (int)forestShenfaMotions[kind].start) / forestShenfaMotions[kind].speed;
            flags = forestShenfaMotions[kind].mirrored ? 0x20000000U : 0;
            EVT_AT_SetMot(work->character, forestShenfaMotions[kind].animation, forestShenfaMotions[kind].start, forestShenfaMotions[kind].speed, flags, 0);
            EVT_AT_SetMot(work->character, MQTE_SHENFA_RUN_MOTION, 0.0f, 1.1f, flags, frames);
            delay = forestShenfaMotions[kind].end;
            work->command++;
            if (forestObstacles[work->obstacle].commands[work->command].kind == 7 || work->command == 3)
                work->phase = 7;
        }
    }
    return delay;
}

void UpdateForestObstacleTarget(ForestQteWork *work)
{
    Vector3 position, base, target = {0.0f, 0.0f, forestObstacles[work->obstacle].z};
    Vector3i rotation;
    float cameraX, cameraZ;
    int i;
    short current, previous;
    FTBL.GetCharaPosition(work->character, -1, &position, 0x40000000);
    FTBL.GetCharaPosition(MQTE_BASE, -1, &base, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(MQTE_BASE, &rotation, 0x40000000);
    if (!work->command) {
        i = 1;
        target.z = forestQteCommands[forestObstacles[work->obstacle].commands[i - 1].kind].offset * 0.5f + target.z;
        cameraX = forestObstacles[work->obstacle].commands[i - 1].cameraOffset;
    } else {
        for (i = 1; i <= work->command; i++) {
            current = forestObstacles[work->obstacle].commands[i].kind;
            previous = forestObstacles[work->obstacle].commands[i - 1].kind;
            if ((current == 0 || current == 1) && (previous == 0 || previous == 1))
                target.z -= 3.0f;
            else
                target.z -= 4.0f;
        }
        target.z = forestQteCommands[forestObstacles[work->obstacle].commands[i - 1].kind].offset * 0.5f + target.z;
        cameraX = forestObstacles[work->obstacle].commands[i - 1].cameraOffset;
    }
    cameraZ = target.z;
    FTBL.ApplyScenarioTranslationAndRotation(&base, &rotation, &target);
    forestObstaclePosition = target;
    forestObstacleDistance = FTBL.CalculatePlanarDistanceBetweenPoints(&position, &forestObstaclePosition);
    EVT_AT_SetPos(MQTE_CAMERA, MQTE_BASE, cameraX + forestObstacles[work->obstacle].x, 0.0f, cameraZ, 0);
    EVT_AT_SetAngY(MQTE_CAMERA, MQTE_BASE, 0.0f, 0);
}

int BeginForestQte(ForestQteWork *work)
{
    short kind, previous;
    int adjacent;
    kind = forestObstacles[work->obstacle].commands[work->command].kind;
    adjacent = 0;
    if (work->command > 0) {
        previous = forestObstacles[work->obstacle].commands[work->command - 1].kind;
        adjacent = (kind == 0 || kind == 1) && (previous == 0 || previous == 1);
    }
    EVT_AT_SetSound(2419, 0);
    if (adjacent)
        QueueForestQteWait(work->result, forestQteCommands[kind].button, 24, forestRetries * 4 + 16, forestQteCommands[kind].success,
            forestQteCommands[kind].failure, 0);
    else
        QueueForestQteWait(work->result, forestQteCommands[kind].button, 24, forestRetries * 4 + 16, forestQteCommands[kind].success,
            forestQteCommands[kind].failure, 5);
    work->command++;
    if (forestObstacles[work->obstacle].commands[work->command].kind == 7 || work->command == 3) {
        work->phase = 7;
        forestLastCommand = 1;
    } else
        work->phase = 6;
    return 0;
}

int ResumeForestObstacle(ForestQteWork *work)
{
    UpdateForestObstacleTarget(work);
    work->phase = 4;
    return 0;
}

int FinishForestObstacle(ForestQteWork *work)
{
    int delay = 0;
    Vector3 position;
    int yaw;
    if (g_currentMap == MQTE_AREA) {
        delay = 60;
        work->phase = 3;
    } else if (work->character == MQTE_RYO) {
        work->completedUnits++;
        if (work->completedUnits == FTBL.GetMazeTerrainField()) {
            FTBL.EV_mazeUtil(forestStage, FTBL.SelectMazeTerrainField(forestStage) - 48, &position);
            yaw = FTBL.GetMazeObjectData(forestStage, FTBL.SelectMazeTerrainField(forestStage) - 48);
            EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, position.x, position.y, position.z, 30);
            EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 30);
            FTBL.EV_mazeUtil(forestStage, FTBL.SelectMazeTerrainField(forestStage) - 45, &position);
            yaw = FTBL.GetMazeObjectData(forestStage, FTBL.SelectMazeTerrainField(forestStage) - 45);
            EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, position.x, position.y, position.z, 30);
            EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 30);
            EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0x40000000U, 30);
            EVT_AT_SetMot(MQTE_SHENFA, MQTE_SHENFA_RUN_MOTION, 0.0f, 1.1f, 0x40000000U, 30);
            EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 30);
            EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 30);
        }
        QueueForestRoadCameras(work->completedUnits, 30);
        if (forestShenfaTask)
            FTBL.HLib_DestroyTaskInternal(forestShenfaTask);
        forestShenfaTask = 0;
        work->phase = 2;
        work->state = 1;
        EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0);
    } else {
        delay = 30;
        forestShenfaTask = FTBL.HLib_EnqueueTaskWithoutParameter(ForestQteRunShenfa, 4, 11, MQTE_RUN_TASK);
        work->phase = 3;
        forestShenfaTicks = delay;
    }
    if (work->character == MQTE_RYO)
        work->character = MQTE_SHENFA;
    else
        work->character = MQTE_RYO;
    return delay;
}

void ForestQteRunShenfa(void *data)
{
    UpdateForestCharacterRoute(MQTE_SHENFA);
    if (forestShenfaTicks-- < 0)
        UpdateForestShenfaSpacing();
}

int DodgeLeftSuccess(void)
{
    PositionForestQteReaction(0.6f);
    if (forestLastCommand)
        EVT_AT_SetCam(3300, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7503, 9.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 20);
    return -9;
}

int DodgeLeftFailure(void)
{
    PositionForestQteReaction(0.7f);
    if (forestLastCommand)
        EVT_AT_SetCam(3310, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A013", 0);
    EVT_AT_SetMot(MQTE_RYO, 7506, 13.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 63);
    return 48;
}

int DodgeRightSuccess(void)
{
    PositionForestQteReaction(0.6f);
    if (forestLastCommand)
        EVT_AT_SetCam(3400, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7503, 9.0f, 1.0f, 0x20000000U, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0x20000000U, 20);
    return -9;
}

int DodgeRightFailure(void)
{
    PositionForestQteReaction(0.7f);
    if (forestLastCommand)
        EVT_AT_SetCam(3410, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A013", 0);
    EVT_AT_SetMot(MQTE_RYO, 7506, 13.0f, 1.0f, 0x20000000U, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0x20000000U, 63);
    return 48;
}

int PitJumpSuccess(void)
{
    PositionForestQteReaction(0.8f);
    if (forestLastCommand)
        EVT_AT_SetCam(3000, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7490, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 30);
    return 1;
}

int PitJumpFailure(void)
{
    PositionForestQteReaction(0.9f);
    if (forestLastCommand)
        EVT_AT_SetCam(3010, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A005", 0);
    EVT_AT_SetMot(MQTE_RYO, 7498, 10.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, 7497, 0.0f, 1.0f, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0x40000000U, 150);
    return 121;
}

int HurdleJumpSuccess(void)
{
    PositionForestQteReaction(0.9f);
    if (forestLastCommand)
        EVT_AT_SetCam(3100, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7495, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 30);
    return 1;
}

int HurdleJumpFailure(void)
{
    Vector3 position, ryo;
    float x, z;
    int road;
    unsigned short yaw;
    if (forestShenfaTask)
        FTBL.HLib_DestroyTaskInternal(forestShenfaTask);
    forestShenfaTask = 0;
    FTBL.GetCharaPosition(MQTE_RYO, -1, &ryo, 0x40000000);
    if (g_currentMap != MQTE_AREA) {
        road = ForestCharacterRoadIndex(MQTE_RYO);
        FTBL.EV_mazeUtil(forestStage, road + 5, &position);
        FTBL.EV_AT_SetPos(MQTE_SHENFA, 0x78000000, &position);
        x = position.x - ryo.x;
        z = position.z - ryo.z;
        yaw = FTBL.InverseFixedAtan2(x, z);
        EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, position.x, position.y, position.z, 0);
        EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 0);
    }
    PositionForestQteReaction(0.9f);
    EVT_AT_SetCam(4000, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetCam(4001, MQTE_DUMMY_SHENFA, MQTE_DUMMY_SHENFA, 0, 0, 50);
    EVT_AT_SetCam(4002, MQTE_DUMMY_RYO, MQTE_DUMMY_RYO, 0, 0, 120);
    EVT_AT_SetMot(MQTE_RYO, 7499, 1.0f, 0.8f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, 7499, 40.0f, 0.0f, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, 7499, 40.0f, 1.0f, 0, 170);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 180);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0.0f, 0.0f, 0, 0);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0.0f, 1.0f, 0, 50);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30.0f, 0.0f, 0, 80);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30.0f, 1.0f, 0, 150);
    EVT_AT_SetMot(MQTE_SHENFA, MQTE_SHENFA_RUN_MOTION, 0.0f, 1.1f, 0, 185);
    EVT_AT_SetVoice("0517A008", 0);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 4, 8, 0.6f, 0);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 0, 0, 1.0f, 185);
    switch (FTBL.shenGetRandomInt32(4)) {
    case 0:
        EVT_AT_SetVoice("0517B004", 70);
        break;
    case 1:
        EVT_AT_SetVoice("0517B005", 70);
        break;
    case 2:
        EVT_AT_SetVoice("0517B006", 70);
        break;
    case 3:
        EVT_AT_SetVoice("0517B007", 70);
        break;
    }
    switch (FTBL.shenGetRandomInt32(4)) {
    case 0:
        EVT_AT_SetVoice("0517A015", 115);
        break;
    case 1:
        EVT_AT_SetVoice("0517A016", 115);
        break;
    case 2:
        EVT_AT_SetVoice("0517A017", 115);
        break;
    case 3:
        EVT_AT_SetVoice("0517A019", 115);
        break;
    }
    return 185;
}

int RootJumpSuccess(void)
{
    PositionForestQteReaction(0.6f);
    if (forestLastCommand)
        EVT_AT_SetCam(3200, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7502, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 7.0f, 1.1f, 0, 20);
    return -4;
}

int RootJumpFailure(void)
{
    PositionForestQteReaction(0.7f);
    if (forestLastCommand)
        EVT_AT_SetCam(3210, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A013", 0);
    EVT_AT_SetMot(MQTE_RYO, 7505, 10.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 70);
    return 46;
}

int DuckSuccess(void)
{
    PositionForestQteReaction(0.6f);
    if (forestLastCommand)
        EVT_AT_SetCam(3500, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A004", 0);
    EVT_AT_SetMot(MQTE_RYO, 7500, 5.0f, 1.2f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 27);
    return -2;
}

int DuckFailure(void)
{
    PositionForestQteReaction(0.6f);
    if (forestLastCommand)
        EVT_AT_SetCam(3510, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice("0517A014", 0);
    EVT_AT_SetMot(MQTE_RYO, 7504, 5.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, MQTE_RUN_MOTION, 0.0f, 1.1f, 0, 54);
    return 25;
}

void PositionForestQteReaction(float distance)
{
    Vector3 local, world, offset = {0.0f, 0.0f, distance};
    Vector3i rotation = {0, 0, 0};
    float x;
    FTBL.AT_SetLocalBaseChara(MQTE_RYO, MQTE_BASE);
    FTBL.GetCharaPosition(MQTE_RYO, -1, &local, 0);
    FTBL.GetScenarioCharacterEulerAngles(MQTE_BASE, &rotation, 0);
    FTBL.GetCharaPosition(MQTE_RYO, -1, &world, 0x40000000);
    x = local.x;
    FTBL.ApplyScenarioTranslationAndRotation(&forestObstaclePosition, &rotation, &offset);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, offset.x, world.y, offset.z, 0);
    FTBL.GetCharaPosition(MQTE_RYO, -1, &local, 0);
    local.x = x;
    FTBL.EV_AT_SetPos(MQTE_RYO, 0x38000000, &local);
    FTBL.AT_SetLocalBaseChara(MQTE_RYO, MQTE_INVALID);
}

short ForestBlendAngle(int target, int current, float rate)
{
    return (short)(int)((short)(target - current) * rate + current);
}

float ForestLerp(float target, float current, float rate)
{
    return (target - current) * rate + current;
}

void QueueForestQteWait(int *result, unsigned int button, int referenceFrames, int maximumFrames, SkQteCallback success, SkQteCallback failure,
    int delay)
{
    HLibTask *task;
    SkWaitQteWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(ForestQteWait_Callback, 6, 11, sizeof(SkWaitQteWork), TASK_EVNT);
    work = (SkWaitQteWork *)FTBL.HLib_GetTaskParameter(task);
    result[0] = 0;
    result[3] = 0;
    work->task = task;
    work->success = success;
    work->failure = failure;
    work->button = button;
    work->maximumFrames = maximumFrames;
    work->referenceFrames = referenceFrames;
    work->elapsed = result;
    work->status = (unsigned short *)(result + 2);
    work->succeeded = result + 3;
    work->phase = 0;
    work->parent = FTBL.HLib_GetCurrentTask();
    if (delay > 4) {
        delay -= 4;
        work->referenceFrames += 4;
    }
    if (work->maximumFrames > work->referenceFrames - 4)
        work->maximumFrames = work->referenceFrames - 4;
    FTBL.HLib_SetupTimedTask(work->parent, 0);
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
}

void ForestQteWait_Callback(void *data)
{
    SkWaitQteWork *work = (SkWaitQteWork *)data;
    int delay = 0;
    unsigned int status;
    SkQteCallback callback;
    switch (work->phase) {
    case 0:
        FTBL.SetShowQteFlagForButtonType(work->button, work->maximumFrames, 10);
        work->phase++;
        break;
    case 1:
        (*work->elapsed)++;
        if (FTBL.GetShouldRenderIcons(3))
            break;
        status = FTBL.GetCurrentQteStatus();
        *work->status = status & 0xfff;
        if ((unsigned short)(status & 0xfff) == work->button)
            *work->succeeded = 1;
        else
            *work->succeeded = 0;
        delay = work->referenceFrames - *work->elapsed;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(work->task, delay);
        work->phase++;
        break;
    case 2:
        if (*work->succeeded)
            callback = work->success;
        else {
            forestFailures++;
            ForestQteRainCheck();
            callback = work->failure;
        }
        if (callback)
            delay = callback();
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(work->parent, delay);
        else
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTask(work->task);
        break;
    }
}

void ForestQteRainCheck(void)
{
    if (forestFailures == 5 && !forestRainStarted) {
        forestRainStarted = 1;
        FTBL.UpdateWeatherForMinute(1, 0, 200, 1.5f);
    }
}

int RunForestAuthor(ForestQteWork *work, int failure)
{
    EventDateTime date;
    shen_name authors[2] = {MQTE_SUCCESS, MQTE_FAILURE};
    HLibTaskCallback callback = 0;
    int delay = 0;
    if (forestShenfaTask)
        FTBL.HLib_DestroyTaskInternal(forestShenfaTask);
    forestShenfaTask = 0;
    MQTE_FREE_PACKAGE("KWW1");
    MQTE_FREE_PACKAGE("KWA1");
    MQTE_FREE_PACKAGE("KWA2");
    MQTE_FREE_PACKAGE("KWA3");
    switch (work->state) {
    case 0:
        FTBL.clmdUtil(FTBL.ctrlGetCharaByName(MQTE_SHENFA), 0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        if (!failure) {
            FTBL.GetCurrentDateTime((DateTime *)&date);
            switch (forestStage) {
            case 0:
                date.hour = 16;
                date.minute = 30;
                break;
            case 1:
                date.hour = 17;
                date.minute = 30;
                break;
            case 2:
                date.hour = 18;
                date.minute = 30;
                break;
            }
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
            FTBL.FreeWAIR();
            FTBL.UpdateWeather();
        }
        FTBL.UpdateTempHourMinTimeScale(23, 30, 0.001f);
        if (!failure) {
            FTBL.Weather_FreeActiveFIXC();
            FTBL.AllocFIXC();
            FTBL.FreeWAIR();
            FTBL.GetWeatherRainPressure();
            FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
            FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
            FTBL.UpdateWeather();
            switch (forestStage) {
            case 0:
                MQTE_LOAD_PACKAGE("KWW1");
                break;
            case 1:
                MQTE_LOAD_PACKAGE("KWA1");
                break;
            case 2:
                MQTE_LOAD_PACKAGE("KWA2");
                break;
            default:
                work->phase = 2;
                work->state = 0;
                break;
            }
        } else {
            FTBL.EV_LoadAudio("0517A000", 2);
            switch (forestStage) {
            case 0:
                MQTE_LOAD_PACKAGE("KWA1");
                break;
            case 1:
                MQTE_LOAD_PACKAGE("KWA2");
                break;
            case 2:
                FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
                work->phase = 10;
                work->state = 0;
                delay = 30;
                break;
            default:
                FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
                work->phase = 9;
                work->state = 0;
                delay = 30;
                break;
            }
        }
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        if (forestStage == 0 && !failure)
            FTBL.EV_SetPlayerFlag(12, 551, 0);
        else if (forestStage == 1 && failure == 1) {
            FTBL.EV_SetWeatherInstruction(12, 0);
            FTBL.ReinitializeLights(2);
            FTBL.EV_lightLoad(MQTE_FAILURE_LIGHT);
            FTBL.PushGlobalLightingProgram();
        } else if (forestStage == 2 && !failure) {
            FTBL.EV_SetWeatherInstruction(12, 0);
            FTBL.ReinitializeLights(2);
            FTBL.EV_lightLoad(MQTE_SUCCESS_LIGHT);
            FTBL.PushGlobalLightingProgram();
        } else {
            FTBL.EV_SetWeatherInstruction(12, 1);
            FTBL.EV_SetWeatherInstruction(1920, 1);
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
            FTBL.FreeWAIR();
            FTBL.UpdateWeather();
        }
        if (forestStage == 1 && !failure)
            FTBL.SetObjectFlagByIndex(3, 0);
        if (forestStage == 2 && !failure) {
            ForestFireflyStart();
            callback = ForestFireflyAuthor;
        }
        if (work->phase == 1 || work->phase == 8) {
            FTBL.mazeSetTerrainDrawEnabled(0);
            QueueSkAuthorStart(work->task, authors[failure], callback, 0, 0);
            work->state++;
        }
        break;
    case 1:
        FTBL.UpdateTempHourMinTimeScale(23, 30, 35.0f);
        ForestFireflyStop();
        FTBL.mazeSetTerrainDrawEnabled(1);
        FTBL.EV_SetPlayerFlag(12, 551, 1);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_SetWeatherInstruction(12, 1);
        FTBL.EV_SetWeatherInstruction(1920, 1);
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        if (work->phase == 1)
            work->phase = 2;
        else
            work->phase = 9;
        MQTE_FREE_PACKAGE("KWW1");
        MQTE_FREE_PACKAGE("KWA1");
        MQTE_FREE_PACKAGE("KWA2");
        MQTE_FREE_PACKAGE("KWA3");
        if (!forestStage)
            FTBL.SetMazeUnitDrawDistance(50.0f);
        else
            FTBL.SetMazeUnitDrawDistance(60.0f);
        work->state = 0;
        ForestQteMain(work);
        break;
    }
    return delay;
}

int ForestRetry(ForestQteWork *work)
{
    int delay = 0;
    if (work->state++ < 90) {
        if (work->state == 1) {
            FTBL.Weather_FreeActiveFIXC();
            FTBL.AllocFIXC();
            FTBL.FreeWAIR();
            FTBL.GetWeatherRainPressure();
            FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
            FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
            FTBL.UpdateWeather();
            FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.Queue_ST_LoadTitleFadeIn(MQTE_RETRY);
            FTBL.Queue_ST_LoadTitleFadeIn(0);
            FTBL.FinishLoading();
        }
    } else {
        forestRetries++;
        work->state = 0;
        work->phase = 1;
        FTBL.ST_LoadTitleFade();
        delay = 40;
        FTBL.SetLoadingBusy();
    }
    return delay;
}

int ForestFinalFailure(ForestQteWork *work)
{
    Vector3 position, ryo;
    float x, z;
    int road;
    unsigned short yaw;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 120, 0, 0, 0);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, -550.3f, 0.0f, -346.5f, 0);
    EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, 270.0f, 0);
    EVT_AT_SetPos(MQTE_CAMERA, MQTE_RYO, 0.0f, 10.0f, 0.0f, 0);
    EVT_AT_SetAngY(MQTE_CAMERA, MQTE_RYO, 0.0f, 0);
    FTBL.GetCharaPosition(MQTE_RYO, -1, &ryo, 0x40000000);
    road = ForestCharacterRoadIndex(MQTE_RYO);
    FTBL.EV_mazeUtil(forestStage, road + 5, &position);
    FTBL.EV_AT_SetPos(MQTE_SHENFA, 0x78000000, &position);
    x = position.x - ryo.x;
    z = position.z - ryo.z;
    yaw = FTBL.InverseFixedAtan2(x, z);
    EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, position.x, position.y, position.z, 0);
    EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, yaw / MQTE_ANGLE_UNITS, 0);
    EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0.0f, 0);
    EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0.0f, 0);
    EVT_AT_SetFaceLooks(MQTE_RYO, 6, 8, 1.0f, 0);
    EVT_AT_SetFaceLooks(MQTE_RYO, 0, 0, 1.0f, 380);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 13, 8, 1.0f, 0);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 0, 0, 1.0f, 380);
    EVT_AT_SetCam(7003, MQTE_INVALID, MQTE_INVALID, 0, 0, 0);
    EVT_AT_SetCam(7004, MQTE_INVALID, MQTE_INVALID, 0, 0, 50);
    EVT_AT_SetCam(7000, MQTE_INVALID, MQTE_INVALID, 0, 0, 90);
    EVT_AT_SetCam(7001, MQTE_INVALID, MQTE_INVALID, 0, 0, 170);
    EVT_AT_SetCam(7002, MQTE_INVALID, MQTE_INVALID, 0, 0, 290);
    EVT_AT_SetMot(MQTE_RYO, 7499, 1.0f, 0.8f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, 7493, 0.0f, 0.8f, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, 7493, 0.0f, 0.8f, 0x40000000U, 90);
    EVT_AT_SetMot(MQTE_RYO, 7493, 280.0f, 0.8f, 0x40000000U, 290);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0.0f, 0.0f, 0, 0);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0.0f, 1.0f, 0, 50);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30.0f, 0.0f, 0, 80);
    EVT_AT_SetPoint(MQTE_SHENFA, MQTE_CAMERA, 5, 3, 1, 170, 150, 0);
    EVT_AT_SetVoice("0517A005", 0);
    EVT_AT_SetVoice("0517B034", 50);
    EVT_AT_SetVoice("0517A043", 90);
    EVT_AT_SetVoice("0517B035", 170);
    EVT_AT_SetVoice("0517A044", 290);
    EVT_AT_MenCtrl(30, 120, 0, 0, 0, 255, 0, 0, 0, 320);
    work->phase = 9;
    return 380;
}
#endif
#endif
