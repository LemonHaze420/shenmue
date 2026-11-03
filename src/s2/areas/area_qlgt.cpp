#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qlgt.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QlgtMain(void *argument)
{
    QueueQlgtEvent(argument);
}
#else
const EventMoveWaypoint qlgtRouteFirst[] = {{{239.6f, 121.6f, 148.2f}, 90.0f}, {{235.5f, 121.6f, 148.2f}, 90.0f}, {{224.6f, 121.6f, 148.2f}, 180.0f},
    {{224.6f, 121.6f, 150.2f}, 90.0f}, {{-1.0f, -1.0f, -1.0f}, 0.0f}};
const signed char qlgtActionsFirst[] = {0, 0, 1, 0, 2};
const EventMoveWaypoint qlgtRouteSecond[] = {{{219.6f, 121.6f, 170.8f}, 90.0f}, {{210.0f, 121.6f, 170.8f}, 180.0f}, {{209.9f, 121.6f, 172.0f}, 180.0f},
    {{208.9f, 121.6f, 177.8f}, 90.0f}, {{206.7f, 121.6f, 177.8f}, 180.0f}, {{-1.0f, -1.0f, -1.0f}, 0.0f}};
const signed char qlgtActionsSecond[] = {0, 0, 0, 1, 0, 2};
const EventMoveWaypoint qlgtRouteThird[] = {{{199.4f, 121.6f, 159.7f}, 0.0f}, {{199.4f, 121.6f, 156.4f}, 0.0f}, {{199.9f, 121.6f, 152.9f}, 0.0f},
    {{200.2f, 121.6f, 150.5f}, 0.0f}, {{200.3f, 121.6f, 148.5f}, 270.0f}, {{202.0f, 121.6f, 148.2f}, 0.0f}, {{-1.0f, -1.0f, -1.0f}, 0.0f}};
const signed char qlgtActionsThird[] = {0, 0, 1, 0, 1, 0, 2};
const EventMoveWaypoint qlgtRouteFourth[] = {{{240.9f, 121.6f, 124.7f}, 225.0f}, {{242.7f, 121.6f, 126.4f}, 315.0f}, {{247.5f, 121.6f, 125.1f}, 180.0f},
    {{247.6f, 121.6f, 128.6f}, 270.0f}, {{-1.0f, -1.0f, -1.0f}, 0.0f}};
const signed char qlgtActionsFourth[] = {0, 0, 1, 0, 2};
const shen_name qlgtEnemiesFirst[] = {SHEN_NAME4('K', 'D', '6', '_'), SHEN_NAME4('T', 'H', 'G', '_'), SHEN_NAME4('K', 'T', '3', '_')};
const shen_name qlgtEnemiesSecond[] = {SHEN_NAME4('C', 'H', 'R', '_'), SHEN_NAME4('S', 'O', 'Y', '_'), SHEN_NAME4('K', 'T', '5', '_'),
    SHEN_NAME4('C', 'J', 'U', '_')};
const shen_name qlgtEnemiesThird[] = {SHEN_NAME4('K', 'U', '2', '_'), SHEN_NAME4('A', 'D', '3', '_'), SHEN_NAME4('W', 'T', 'C', '_'),
    SHEN_NAME4('K', 'N', '3', '_'), SHEN_NAME4('T', 'B', '2', '_')};
const shen_name qlgtEnemiesFourth[] = {SHEN_NAME4('M', 'O', 'N', '_'), SHEN_NAME4('S', 'D', '7', '_'), SHEN_NAME4('W', 'D', '4', '_'),
    SHEN_NAME4('A', 'D', '4', '_'), SHEN_NAME4('K', 'D', '7', '_'), SHEN_NAME4('Y', 'O', 'H', '_')};
const QlgtSafeZone qlgtSafeFirst[] = {{1, {237.0f, 121.6f, 147.7f}, 2.7f}, {1, {229.0f, 121.6f, 147.9f}, 2.5f}, {0, {0.0f, 0.0f, 0.0f}, 0.0f}};
const QlgtSafeZone qlgtSafeSecond[] = {{1, {210.0f, 121.6f, 171.5f}, 3.2f}, {0, {0.0f, 0.0f, 0.0f}, 0.0f}};
const QlgtSafeZone qlgtSafeThird[] = {{1, {199.4f, 121.6f, 156.8f}, 1.8f}, {1, {199.3f, 121.6f, 151.2f}, 1.8f}, {0, {0.0f, 0.0f, 0.0f}, 0.0f}};
const QlgtSafeZone qlgtSafeFourth[] = {{1, {245.0f, 121.6f, 125.5f}, 2.2f}, {0, {0.0f, 0.0f, 0.0f}, 0.0f}};
const QlgtCourse qlgtCourses[] = {
    {&qlgtPatterns[0], {222.2f, 121.6f, 134.9f}, {251.7f, 121.6f, 154.4f}, {{244.3f, 121.6f, 146.3f}, 180.0f}, 1.5f, {{225.5f, 121.6f, 150.0f}, 90.0f}, 3.3f,
        {224.2f, 121.6f, 151.5f}, 2.2f, {{234.0f, 121.6f, 142.0f}, 0.0f}, 11.0f, {{233.1f, 121.6f, 146.7f}, 0.0f}, {239.5f, 121.6f, 139.0f}, 5.0f, 3, 50,
        {233.5f, 121.6f, 140.5f}, 5.0f, {{222.3f, 121.6f, 155.0f}, 165.0f}, {{223.2f, 121.6f, 154.5f}, 115.0f}, {{249.0f, 121.6f, 152.0f}, 0.0f},
        qlgtEnemiesFirst, qlgtSafeFirst, qlgtRouteFirst, qlgtActionsFirst},
    {&qlgtPatterns[1], {201.0f, 121.6f, 164.1f}, {226.0f, 121.6f, 188.1f}, {{221.8f, 121.6f, 168.7f}, 90.0f}, 1.5f, {{207.5f, 121.6f, 176.8f}, 180.0f}, 3.3f,
        {208.0f, 121.6f, 179.0f}, 2.8f, {{218.0f, 121.6f, 179.5f}, 0.0f}, 10.5f, {{214.2f, 121.6f, 170.9f}, 0.0f}, {223.3f, 121.6f, 176.8f}, 5.0f, 4, 50,
        {218.0f, 121.6f, 174.4f}, 5.0f, {{201.9f, 121.6f, 178.4f}, 70.0f}, {{201.9f, 121.6f, 176.9f}, 175.0f}, {{220.5f, 121.6f, 166.7f}, 180.0f},
        qlgtEnemiesSecond, qlgtSafeSecond, qlgtRouteSecond, qlgtActionsSecond},
    {&qlgtPatterns[2], {185.0f, 121.6f, 144.6f}, {207.5f, 121.6f, 165.4f}, {{197.5f, 121.6f, 160.6f}, 270.0f}, 1.5f, {{201.0f, 121.6f, 148.9f}, 0.0f}, 3.3f,
        {202.5f, 121.6f, 147.5f}, 2.2f, {{194.0f, 121.6f, 150.5f}, 90.0f}, 10.0f, {{198.0f, 121.6f, 154.3f}, 90.0f}, {190.0f, 121.6f, 157.0f}, 4.0f, 4, 50,
        {192.0f, 121.6f, 151.8f}, 5.0f, {{206.4f, 121.6f, 147.5f}, 270.0f}, {{207.1f, 121.6f, 148.7f}, 320.0f}, {{198.5f, 121.6f, 162.5f}, 0.0f},
        qlgtEnemiesThird, qlgtSafeThird, qlgtRouteThird, qlgtActionsThird},
    {&qlgtPatterns[3], {231.4f, 121.6f, 115.6f}, {249.4f, 121.6f, 131.9f}, {{236.2f, 121.6f, 124.9f}, 180.0f}, 1.5f, {{246.2f, 121.6f, 128.4f}, 270.0f}, 3.3f,
        {247.5f, 121.6f, 128.4f}, 2.2f, {{243.0f, 121.6f, 122.4f}, 0.0f}, 7.0f, {{239.4f, 121.6f, 124.3f}, 0.0f}, {240.0f, 121.6f, 120.0f}, 4.0f, 4, 50,
        {245.0f, 121.6f, 120.0f}, 4.0f, {{247.8f, 121.6f, 132.0f}, 180.0f}, {{246.4f, 121.6f, 132.2f}, 230.0f}, {{235.0f, 121.6f, 126.0f}, 270.0f},
        qlgtEnemiesFourth, qlgtSafeFourth, qlgtRouteFourth, qlgtActionsFourth}};
const EventChaseParams qlgtChaseDefaults = {QLGT_REN, QLGT_REN_DUMMY, 0, {0.08f, 0.08f, 0.08f}, {0.0f, 0.0f, 0.0f}, 0.01f, 0.01f, 0.0f, 0.0f,
    {0.1f, 0.1f, 0.1f}, {0.0f, 0.0f, 0.0f}, 0.1f, 0.0f, 6.0f, 1.0f};
const unsigned short qlgtIdleMotions[] = {0x1d9c, 0x1dac, 0x1d9e, 0x1dac, 0x1dad, 0x1db0, 0x1daf, 0x1d9d};
const unsigned short qlgtBattleMotions[] = {0x1d99, 0x1db1, 0x1d9a, 0x1d99, 0x1db1, 0x1d9a, 0x1db3, 0x1db4, 0x1d9b, 0x1db2};
const shen_name qlgtHiddenDoors[] = {SHEN_NAME4('D', 'R', '0', '0'), SHEN_NAME4('D', 'R', '0', '1'), SHEN_NAME4('D', 'R', '0', '2'),
    SHEN_NAME4('D', 'R', '0', '3')};
const QlgtEntry qlgtEntries[] = {{0, 391}, {1, 392}, {2, 393}, {3, 394}, {0, 391}};
QlgtFlashPattern qlgtPatterns[] = {
    {2, {1, 2, 0, 1, 2, 0, 0, 1, 2, 2, 0, 0, 1, 2, 2, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {3, {1, 2, 1, 2, 0, 0, 1, 2, 2, 0, 1, 2, 0, 0, 1, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {3, {1, 2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 2, 1, 0, 1, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {2, {1, 2, 2, 0, 1, 1, 2, 0, 0, 1, 2, 0, 1, 2, 2, 1}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}}};
shen_name qlgtActors[] = {QLGT_RYO, QLGT_REN, QLGT_INVALID, QLGT_INVALID, QLGT_INVALID, QLGT_INVALID, QLGT_INVALID, QLGT_INVALID, QLGT_INVALID};
shen_name qlgtDrums[] = {QLGT_DRUM, 0};

void QueueQlgtEvent(const QlgtEntry *entry)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(QlgtStart, 4, 11, sizeof(QlgtWork), TASK_EVNT);
    QlgtWork *work = (QlgtWork *)FTBL.HLib_GetTaskParameter(task);
    HLibTask *subtask = FTBL.HLib_EnqueueTaskWithParameter(QlgtSubtaskCallback, 3, 11, sizeof(QlgtWork *), TASK_EVNT);
    *(QlgtWork **)FTBL.HLib_GetTaskParameter(subtask) = work;
    work->subtask = subtask;
    work->task = task;
    if (entry)
        work->entry = entry;
    else
        work->entry = &qlgtEntries[g_entryPoint];
    QlgtPrepare(work);
    EventDebugRegister(QLGT_DEBUG_ID, QlgtDebugExitCallback, work, 2000000000);
}

void QlgtDebugExitCallback(void *data)
{
    QlgtFinish((QlgtWork *)data);
}

void QlgtFinish(QlgtWork *work)
{
    QlgtCleanup(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->subtask);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(QLGT_DEBUG_ID);
    if (g_currentMap == QLGT_AREA) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

void QlgtSubtaskCallback(void *data)
{
}

void QlgtComputeSides(QlgtSequence *sequence)
{
    Vector3 position;
    FTBL.AT_SetLocalBaseChara(QLGT_ENEMY, QLGT_START);
    FTBL.GetCharaPosition(QLGT_ENEMY, -1, &position, 0);
    if (position.x < 0.0f)
        sequence->startSide = 1;
    else
        sequence->startSide = 0;
    FTBL.AT_SetLocalBaseChara(QLGT_ENEMY, QLGT_END);
    FTBL.GetCharaPosition(QLGT_ENEMY, -1, &position, 0);
    if (position.x < 0.0f)
        sequence->endSide = 1;
    else
        sequence->endSide = 0;
    FTBL.AT_SetLocalBaseChara(QLGT_ENEMY, QLGT_INVALID);
    sequence->waypoint = 0;
    sequence->grace = 10;
}

void QlgtPlaceEnemies(QlgtWork *work, shen_name parent, float radius, const unsigned short *motions, int delay)
{
    const QlgtCourse *course = work->course;
    float step = 360.0f / (float)course->enemyCount;
    float degrees;
    unsigned short angle;
    Vector3 position;
    int i;
    for (i = 0; i < course->enemyCount; i++) {
        degrees = step * (float)i;
        angle = degrees * 8192.0f / 45.0f;
        position.x = FTBL.CalculateFixedCosine(angle) * radius;
        position.z = -FTBL.CalculateFixedSine(angle) * radius;
        EVT_AT_SetPos(qlgtActors[i + 2], parent, position.x, 0.0f, position.z, delay);
        EVT_AT_SetAngY(qlgtActors[i + 2], parent, degrees + 90.0f, delay);
        EVT_AT_SetMot(qlgtActors[i + 2], motions[i], 0.0f, 1.0f, 0x40000000, delay);
        FTBL.SetScenarioCharacterDrawControl(qlgtActors[i + 2], 0, 0, 0.0f);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 1);
    }
}

void QlgtPlaceBattleEnemies(QlgtWork *work, shen_name parent, float radius, const unsigned short *motions, int delay)
{
    const QlgtCourse *course = work->course;
    float step = 360.0f / (float)course->enemyCount;
    float degrees;
    unsigned short angle;
    Vector3 position;
    int i;
    for (i = 0; i < course->enemyCount; i++) {
        degrees = step * (float)i;
        angle = degrees * 8192.0f / 45.0f;
        position.x = FTBL.CalculateFixedCosine(angle) * radius;
        position.z = -FTBL.CalculateFixedSine(angle) * radius;
        EVT_AT_SetPos(qlgtActors[i + 2], parent, position.x, 0.0f, position.z, delay);
        EVT_AT_SetAngY(qlgtActors[i + 2], parent, degrees + 150.0f, delay);
        EVT_AT_SetMot(qlgtActors[i + 2], motions[i], 0.0f, 1.0f, 0x40000000, delay);
    }
}

int QlgtIsInSafeZone(const QlgtSafeZone *zones)
{
    Vector3 position;
    int i;
    if (zones) {
        FTBL.GetCharaPosition(QLGT_RYO, -1, &position, 0x40000000);
        for (i = 4; zones->enabled; zones++) {
            if (FTBL.ComputeAngle(&position, zones->radius, 0, 0xff49, &zones->centre))
                return 1;
            if (--i == 0)
                break;
        }
    }
    return 0;
}

void QlgtStart(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    const QlgtCourse *course = work->course;
    int frames;
    EventDebugCheck(QLGT_DEBUG_ID, 4);
    FTBL.UnknownCharaFunc(QLGT_REN);
    FTBL.naviSetFlag(0);
    FTBL.CheckHelpIcon(1, 0, 0);
    FTBL.FinishLoading();
    EVT_AT_SetPos(QLGT_ENEMY, QLGT_INVALID, course->fail.x, work->height, course->fail.z, 0);
    EVT_AT_SetAngY(QLGT_ENEMY, QLGT_INVALID, 0.0f, 0);
    EVT_AT_SetPos(QLGT_BATTLE, QLGT_INVALID, course->failBattle.x, work->height, course->failBattle.z, 0);
    EVT_AT_SetAngY(QLGT_BATTLE, QLGT_INVALID, 0.0f, 0);
    EVT_AT_SetPos(QLGT_LIGHT, QLGT_INVALID, course->battle.position.x, work->height, course->battle.position.z, 0);
    EVT_AT_SetAngY(QLGT_LIGHT, QLGT_INVALID, course->battle.yaw, 0);
    EVT_AT_SetPos(QLGT_START, QLGT_INVALID, course->start.position.x, work->height, course->start.position.z, 0);
    EVT_AT_SetAngY(QLGT_START, QLGT_INVALID, course->start.yaw, 0);
    EVT_AT_SetPos(QLGT_END, QLGT_INVALID, course->end.position.x, work->height, course->end.position.z, 0);
    EVT_AT_SetAngY(QLGT_END, QLGT_INVALID, course->end.yaw, 0);
    EVT_AT_SetPos(QLGT_DRUM, QLGT_ENEMY, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QLGT_DRUM, QLGT_ENEMY, 0.0f, 0);
    QlgtComputeSides(sequence);
    QlgtPlaceEnemies(work, QLGT_ENEMY, 1.5f, qlgtIdleMotions, 0);
    FTBL.AT_SetLocalBaseChara(QLGT_RYO_DUMMY, QLGT_RYO);
    FTBL.AT_SetLocalBaseChara(QLGT_REN_DUMMY, QLGT_REN);
    EVT_AT_SetPos(QLGT_RYO_DUMMY, QLGT_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QLGT_RYO_DUMMY, QLGT_RYO, 0.0f, 0);
    EVT_AT_SetPos(QLGT_REN_DUMMY, QLGT_REN, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QLGT_REN_DUMMY, QLGT_REN, 0.0f, 0);
    EVT_AT_SetPos(QLGT_RYO, QLGT_START, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPos(QLGT_REN, QLGT_START, 0.0f, 0.0f, 0.0f, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QLGT_RYO), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QLGT_REN), 1);
    FTBL.EV_LoadAudio("0263A000", 0);
    EVT_AT_SetSample(0, 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    if (work->entry->course)
        frames = QlgtIntroduction(sequence->startSide, work->entry->course);
    else
        frames = QlgtFirstIntroduction(sequence->startSide);
    QlgtResetFlash(work->flash);
    FTBL.HLib_SetCurrentTaskTo(QlgtStartLecture);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QlgtStartLecture(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    EventChaseWork *chase = &sequence->chase;
    const QlgtCourse *course = work->course;
    int frames;
    FTBL.naviSetFlag(1);
    FTBL.CheckHelpIcon(0, 0, 0);
    frames = QlgtStartLectureCamera(course->enemyCount);
    FTBL.AT_SetLocalBaseChara(QLGT_REN_DUMMY, QLGT_INVALID);
    InitEventChase(chase, &qlgtChaseDefaults);
    FTBL.EV_AT_SetPos(QLGT_REN_DUMMY, 0x78000000, &chase->positions[chase->writeIndex]);
    FTBL.EV_AT_SetAngY_0(QLGT_REN_DUMMY, 0x78000000, &chase->rotations[chase->writeIndex], 0);
    if (sequence->startSide)
        EVT_AT_SetAngY(QLGT_REN_DUMMY, QLGT_START, 90.0f, 0);
    else
        EVT_AT_SetAngY(QLGT_REN_DUMMY, QLGT_START, -90.0f, 0);
    FTBL.HLib_SetCurrentTaskTo(QlgtRunLecture);
    FTBL.HLib_SetupTaskDelay(frames);
}

const unsigned int qlgtRouteMotions[] = {0xf002, 0xf002, 0xf002, 0xf002, 0x1d92, 0xf002, 0xf002};

void QlgtRunLecture(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    const QlgtCourse *course = work->course;
    if (!work->flash->transition) {
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(QLGT_REN), 71, 2, 0);
        sequence->movement = EVT_MV_MoveMain_Enqueue(0, QLGT_REN, course->route, qlgtRouteMotions, 1.0f, 0x80420000, 0);
        EVT_MV_SetMoveMainIndex(sequence->movement, sequence->waypoint);
        FTBL.HLib_SetCurrentTaskTo(QlgtWaitLecture);
        FTBL.HLib_SetupTaskDelay(10);
    }
}

void QlgtWaitLecture(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    const QlgtCourse *course = work->course;
    int active, frames, waypoint;
    active = EVT_MV_IsMoveMainActive(sequence->movement);
    if (!active) {
        EVT_MV_CancelMoveMain(sequence->movement);
        FTBL.AT_SetLocalBaseChara(QLGT_REN_DUMMY, QLGT_REN);
        EVT_AT_SetPos(QLGT_REN_DUMMY, QLGT_REN, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(QLGT_REN_DUMMY, QLGT_REN, 0.0f, 0);
        frames = QlgtLectureFinish(sequence->endSide);
        EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, frames - 10);
        EVT_AT_MenCtrl(10, 255, 0, 0, 0, 0, 0, 0, 0, frames);
        QlgtQueueSearchPose(QLGT_RYO, QLGT_START, sequence->startSide, frames);
        FTBL.HLib_SetCurrentTaskTo(QlgtSearch);
        FTBL.HLib_SetupTaskDelay(frames + 10);
    } else {
        waypoint = EVT_MV_GetMoveMainIndex(sequence->movement);
        if (sequence->waypoint != waypoint) {
            sequence->waypoint = waypoint;
            if (course->routeActions[waypoint] == 1) {
                EVT_MV_CancelMoveMain(sequence->movement);
                frames = EVT_AT_SetMot(QLGT_REN, 0xf002, 0.0f, 1.0f, 0, 0);
                FTBL.HLib_SetCurrentTaskTo(QlgtRunLecture);
                FTBL.HLib_SetupTaskDelay(frames - 30);
            } else if (course->routeActions[sequence->waypoint + 1] == 1) {
                EVT_MV_CancelMoveMain(sequence->movement);
                FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(QLGT_REN), 71, 2, 0);
                sequence->movement = EVT_MV_MoveMain_Enqueue(0, QLGT_REN, course->route, qlgtRouteMotions, 1.0f, 0x80220000, 0);
                EVT_MV_SetMoveMainIndex(sequence->movement, sequence->waypoint);
            }
        } else
            UpdateEventChase(&sequence->chase);
    }
}

void QlgtEndLecture(void *data)
{
    FTBL.EV_LoadAudio("0263A000", 1);
    EVT_AT_MenCtrl(10, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.FinishLoading();
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    EVT_AT_SetMot(QLGT_RYO, 0xf002, 2.0f, 1.0f, 0x40000000, 0);
    QLGT_SET_RYO_GRAVITY(1);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_SetCurrentTaskTo(QlgtPlay);
}

void QlgtPlay(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    const QlgtCourse *course = work->course;
    Vector3 position;
    int inside, exposed, failed, atStart, atGoal, frames;
    FTBL.SetPlayerGameFlags(6, 1);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.GetCharaPosition(QLGT_RYO, -1, &position, 0x40000000);
    inside = FTBL.TestCharaPositionWithinRange(QLGT_RYO, &course->boundsFirst, &course->boundsSecond, 0);
    exposed = FTBL.ComputeAngle(&position, course->enemyRadius, 0, 0xff49, &course->enemy.position);
    failed = FTBL.ComputeAngle(&position, course->failRadius, 0, 0xff49, &course->fail);
    atStart = FTBL.ComputeAngle(&position, course->startRadius, 0, 0xff49, &course->start.position);
    atGoal = FTBL.ComputeAngle(&position, course->goalRadius, 0, 0xff49, &course->goal);
    if (work->flash->state == 2 && !QlgtIsInSafeZone(course->safeZones) && exposed) {
        if (sequence->grace-- < 0)
            FTBL.HLib_SetCurrentTaskTo(QlgtFail);
    } else if (failed)
        FTBL.HLib_SetCurrentTaskTo(QlgtFail);
    else if (atStart) {
        FTBL.GetBattleDataConfig(0, QLGT_BATTLE_DATA);
        FTBL.GetBattleDataConfig(1, QLGT_BATTLE_DATA);
        if (FTBL.GetBattleDataConfig(2, QLGT_BATTLE_DATA)) {
            FTBL.SetPlayerControlEnabled(0);
            QLGT_SET_RYO_GRAVITY(0);
            EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, 0);
            EVT_AT_MenCtrl(10, 255, 0, 0, 0, 0, 0, 0, 0, 10);
            QlgtQueueSearchPose(QLGT_RYO, QLGT_START, sequence->startSide, 10);
            FTBL.HLib_SetCurrentTaskTo(QlgtSearch);
            FTBL.HLib_SetupTaskDelay(10);
        }
    } else if (atGoal) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetLoadingBusy();
        EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QlgtSuccess);
        FTBL.HLib_SetupTaskDelay(40);
    } else if (!inside) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetLoadingBusy();
        frames = QlgtReturnDialogue(0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_INVALID, course->returnPosition.position.x, course->returnPosition.position.z, course->returnPosition.yaw, frames);
        EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QlgtEndLecture);
        FTBL.HLib_SetupTaskDelay(frames);
    } else
        sequence->grace = 10;
}

void QlgtSearch(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    FTBL.GetBattleDataConfig(0, QLGT_BATTLE_DATA);
    FTBL.GetBattleDataConfig(1, QLGT_BATTLE_DATA);
    if (FTBL.GetBattleDataConfig(2, QLGT_BATTLE_DATA)) {
        EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        if (sequence->startSide)
            EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, 0.0f, -0.7f, 90.0f, 10);
        else
            EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, 0.0f, -0.7f, -90.0f, 10);
        FTBL.HLib_SetCurrentTaskTo(QlgtEndLecture);
        FTBL.HLib_SetupTaskDelay(10);
    }
}

void QlgtSuccess(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    int frames;
    QLGT_SET_RYO_GRAVITY(0);
    FTBL.naviSetFlag(0);
    FTBL.EV_LoadAudio("0263A000", 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    frames = QlgtSuccessDialogue(sequence->endSide, work->entry->course, 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 30);
    EVT_AT_SetSample(0, 1, frames - 60);
    FTBL.HLib_SetCurrentTaskTo(QlgtEnd);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QlgtFail(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    int frames;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetLoadingBusy();
    QLGT_SET_RYO_GRAVITY(0);
    FTBL.naviSetFlag(0);
    FTBL.EV_LoadAudio("0263A000", 2);
    QlgtResetFlash(work->flash);
    frames = QlgtFailDialogue(sequence->endSide, work->entry->course);
    QlgtPlaceBattleEnemies(work, QLGT_BATTLE, 3.0f, qlgtBattleMotions, 230);
    FTBL.HLib_SetCurrentTaskTo(QlgtStartBattle);
    FTBL.HLib_SetupTaskDelay(frames);
}

const int qlgtOrdinaryHealth[] = {120, 50, 50, 50, 50, 50};
const int qlgtOrdinaryStyles[] = {5, 4, 3, 2, 2, 2};
const int qlgtAdvancedHealth[] = {120, 90, 80, 100, 60};
const int qlgtAdvancedStyles[] = {37, 36, 35, 38};

void QlgtStartBattle(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    const QlgtCourse *course = work->course;
    int i;
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QLGT_RYO), 0, 0, 0, 0, 0);
    for (i = 0; i < course->enemyCount; i++) {
        if ((unsigned int)(work->entry->course - 2) <= 1)
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 0, 1, 1, qlgtAdvancedHealth[i], qlgtAdvancedStyles[i]);
        else
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 0, 1, 1, qlgtOrdinaryHealth[i], qlgtOrdinaryStyles[i]);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 0, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 0, 0, 0);
    }
    FTBL.vfCopyPosFromEntity(0, 0, course->failBattle.x, course->failBattle.z, course->failBattleRadius);
    FTBL.EV_vfBattleUtil(3, 0x800, 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_RYO), 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_REN), 0);
    FTBL.SetScenarioCharacterType(QLGT_RYO, 0);
    FTBL.SetScenarioCharacterType(QLGT_REN, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 0, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 0, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 0, 0, 0);
    if (work->entry->course != 1) {
        FTBL.EV_DisableCharacterDisplay(QLGT_REN);
        FTBL.EV_ToggleCharacterFlag(QLGT_REN);
    }
    if (FTBL.EV_GetShenmueFlag(807)) {
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('D', 'R', '0', '0'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('D', 'R', '0', '0'));
    }
    if (FTBL.EV_GetShenmueFlag(808)) {
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('D', 'R', '0', '1'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('D', 'R', '0', '1'));
    }
    if (FTBL.EV_GetShenmueFlag(809)) {
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('D', 'R', '0', '2'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('D', 'R', '0', '2'));
    }
    FTBL.vfmQueueFightStart(course->enemyCount + 1, 2, 1.0f, 1, -1);
    FTBL.EV_vfBattleUtil(1, 2, 1);
    FTBL.HLib_SetCurrentTaskTo(QlgtPlayBattle);
}

void QlgtPlayBattle(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    if (!FTBL.IsFightActive()) {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QLGT_RYO)) == 3) {
            FTBL.EV_stepSetFlags(work->entry->step, 0);
            FTBL.HLib_SetCurrentTaskTo(QlgtWin);
        } else {
            RestoreEventDateTime();
            FTBL.Queue_ST_LoadTitleFadeIn(QLGT_RETRY_TITLE);
            FTBL.HLib_SetCurrentTaskTo(QlgtRetry);
        }
        QlgtTrackDrumTransparency(1);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetupTaskDelay(60);
    } else
        QlgtTrackDrumTransparency(0);
}

void QlgtWin(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    QlgtSequence *sequence = &work->sequence;
    int frames;
    if (work->entry->course != 1) {
        FTBL.EV_EnableCharacterDisplay(QLGT_REN);
        FTBL.EV_EnsureCharacterDraw(QLGT_REN);
    }
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_RYO), 1);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_REN), 1);
    FTBL.SetScenarioCharacterType(QLGT_RYO, 1);
    FTBL.SetScenarioCharacterType(QLGT_REN, 1);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 1, 0, 0);
    FTBL.EV_LoadAudio("0263A000", 0);
    EVT_AT_SetSample(0, 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    frames = QlgtWinDialogue(sequence->endSide, work->entry->course);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 30);
    EVT_AT_SetSample(0, 1, frames - 60);
    FTBL.HLib_SetCurrentTaskTo(QlgtEnd);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QlgtRetry(void *data)
{
    QlgtWork *work = (QlgtWork *)data;
    const QlgtCourse *course = work->course;
    int i;
    if (work->entry->course != 1) {
        FTBL.EV_EnableCharacterDisplay(QLGT_REN);
        FTBL.EV_EnsureCharacterDraw(QLGT_REN);
    }
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_RYO), 1);
    FTBL.SetScenarioCharacterType(QLGT_RYO, 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QLGT_REN), 1);
    FTBL.SetScenarioCharacterType(QLGT_REN, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QLGT_REN), 1, 0, 0);
    for (i = 0; i < course->enemyCount; i++) {
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 1);
        FTBL.SetScenarioCharacterType(qlgtActors[i + 2], 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(qlgtActors[i + 2]), 1, 0, 0);
    }
    FTBL.ST_LoadTitleFade();
    FTBL.HLib_SetCurrentTaskTo(QlgtStart);
    FTBL.HLib_SetupTaskDelay(30);
}

void QlgtEnd(void *data)
{
    QlgtFinish((QlgtWork *)data);
}

QlgtFlashWork *QlgtQueueFlash(const QlgtFlashPattern *pattern)
{
    HLibTask *task;
    QlgtFlashWork *flash;
    if (!pattern)
        return 0;
    task = FTBL.HLib_EnqueueTaskWithParameter(QlgtFlash, 4, 11, sizeof(QlgtFlashWork), TASK_EVNT);
    FTBL.HLib_SetupTimedTask(task, 60);
    flash = (QlgtFlashWork *)FTBL.HLib_GetTaskParameter(task);
    flash->task = task;
    flash->pattern = pattern;
    flash->index = 0;
    flash->remaining = pattern->frames[flash->index];
    flash->state = pattern->states[flash->index];
    flash->transition = flash->state;
    flash->reserved18 = 0;
    return flash;
}

void QlgtKillFlash(QlgtFlashWork *flash)
{
    if (flash)
        FTBL.EV_HLKillOtherHierarchyTask(flash->task);
}

void QlgtSetFlashLights(QlgtFlashWork *flash, int enabled)
{
    shen_name prop;
    int i;
    for (i = 0; i < flash->pattern->lights;)
        FTBL.SetLightSceneIntensity(++i, (float)enabled);
    for (i = 0, prop = QLGT_FIRST_PROP; i < 10; i++, prop += 0x1000000)
        FTBL.SetDisplayFlagForProp(prop, !enabled);
}

void QlgtResetFlash(QlgtFlashWork *flash)
{
    flash->index = 0;
    flash->remaining = flash->pattern->frames[flash->index];
    flash->state = flash->pattern->states[flash->index];
}

void QlgtFlash(void *data)
{
    QlgtFlashWork *flash = (QlgtFlashWork *)data;
    float random;
    if (flash->remaining-- > 0) {
        flash->transition = 4;
        if (flash->state == 1) {
            if (flash->remaining & 1)
                QlgtSetFlashLights(flash, 0);
            else
                QlgtSetFlashLights(flash, 1);
        }
        return;
    }
    flash->index = (flash->index + 1) % 16;
    flash->remaining = flash->pattern->frames[flash->index];
    flash->state = flash->pattern->states[flash->index];
    flash->transition = flash->state;
    random = FTBL.GetRandomNumberInRange(100.0f);
    switch (flash->state) {
    case 0:
        QlgtSetFlashLights(flash, 0);
        EVT_AT_SetSound(0x570, 0);
        break;
    case 2:
        QlgtSetFlashLights(flash, 1);
        if (random < 50.0f)
            EVT_AT_SetSound(0x56f, 0);
        else
            EVT_AT_SetSound(0x571, 0);
        break;
    case 1:
        if (random < 30.0f)
            EVT_AT_SetSound(0x579, 0);
        else if (random < 60.0f)
            EVT_AT_SetSound(0x57a, 0);
        else
            EVT_AT_SetSound(0x57b, 0);
        break;
    case 3:
        QlgtResetFlash(flash);
        break;
    }
}

const int qlgtLightSlots[] = {1, 4, 6, 8};
const shen_name qlgtLightDefinitions[] = {SHEN_NAME4('0', '0', '0', '1'), SHEN_NAME4('0', '0', '0', '2'), SHEN_NAME4('0', '0', '0', '3'),
    SHEN_NAME4('0', '0', '0', '4')};

void QlgtPrepare(QlgtWork *work)
{
    Vector3 position;
    int i;
    EVT_AT_CharaDisp(qlgtHiddenDoors[work->entry->course], 0, 1);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    SaveEventDateTime();
    EVT_MV_ResetMoveMain();
    QlgtClaimDoors(work);
    FTBL.GetCharaPosition(QLGT_RYO, -1, &position, 0x40000000);
    work->height = position.y;
    work->course = &qlgtCourses[work->entry->course];
    for (i = 0; i < work->course->enemyCount; i++)
        qlgtActors[i + 2] = work->course->enemies[i];
    work->characters = Queue_EVT_AT_ReadChr(work->task, qlgtActors);
    work->flash = QlgtQueueFlash(work->course->pattern);
    work->motionBank = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    work->previousNavigation = FTBL.naviSetFlag(0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QLGT", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QLGT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QLGT", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0263");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0263A000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    for (i = 1; i < 10; i++)
        FTBL.SetObjectFlagByIndex(i, 0);
    FTBL.SetObjectFlagByIndex(qlgtLightSlots[work->entry->course], 1);
    FTBL.SetObjectFlagByIndex(qlgtLightSlots[work->entry->course] + 1, 1);
    FTBL.SetDefaultVSByIndex(qlgtLightSlots[work->entry->course]);
    FTBL.SetDefaultVSByIndex(qlgtLightSlots[work->entry->course] + 1);
    FTBL.EV_lightLoad(qlgtLightDefinitions[work->entry->course]);
    FTBL.EV_EventSetUpHelpIcon_0(88, 0);
    FTBL.EV_SetPlayerFlag(12, 679, 1);
}

void QlgtCleanup(QlgtWork *work)
{
    const QlgtCourse *course = work->course;
    int i;
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QLGT_RYO), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QLGT_REN), 0);
    EVT_AT_SetPosAng(QLGT_RYO, QLGT_INVALID, course->ryoExit.position.x, course->ryoExit.position.z, course->ryoExit.yaw, 0);
    EVT_AT_SetPosAng(QLGT_REN, QLGT_INVALID, course->renExit.position.x, course->renExit.position.z, course->renExit.yaw, 0);
    FTBL.npcUpdateFlags(QLGT_REN);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.EV_SetPlayerFlag(12, 679, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    for (i = 1; i < 10; i++)
        FTBL.SetObjectFlagByIndex(i, 1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(qlgtLightSlots[work->entry->course]);
    FTBL.AssignLightProgram(qlgtLightSlots[work->entry->course] + 1);
    FTBL.qronSetFloor(FTBL.GetQronNextStream());
    FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0263");
    FTBL.EV_ControlPackageResources(0, 6, "QLGT", 0, 0);
    FTBL.naviSetFlag(work->previousNavigation);
    FTBL.DeleteMotion(work->motionBank);
    QlgtReleaseDoors(work);
    EVT_MV_CancelAllMoveMain();
    QlgtKillFlash(work->flash);
    ET_AT_KillWork(work->characters);
    FTBL.EV_NPC_NewLoadSw_0(1);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_CharaDisp(qlgtHiddenDoors[work->entry->course], 0, 0);
    FTBL.EV_stepSetFlags(work->entry->step, 1);
}

const shen_name qlgtDoors[] = {SHEN_NAME4('R', '1', '1', '1'), SHEN_NAME4('R', '1', '1', '2'), SHEN_NAME4('R', '1', '1', '3'), SHEN_NAME4('R', '1', '0', '3'),
    SHEN_NAME4('R', '1', '1', '0'), SHEN_NAME4('R', '1', '0', '9'), SHEN_NAME4('R', '1', '1', '5'), SHEN_NAME4('R', '1', '1', '4'),
    SHEN_NAME4('R', '1', '0', '1'), SHEN_NAME4('R', '1', '0', '2'), SHEN_NAME4('R', '1', '0', '4'), SHEN_NAME4('R', '1', '0', '5'),
    SHEN_NAME4('R', '1', '0', '6'), SHEN_NAME4('R', '1', '0', '7'), SHEN_NAME4('R', '1', '0', '8'), SHEN_NAME4('R', '1', '1', '6'),
    SHEN_NAME4('R', '1', '1', '7'), SHEN_NAME4('R', '1', '1', '8'), SHEN_NAME4('R', '1', '1', '9'), SHEN_NAME4('R', '1', '2', '3'),
    SHEN_NAME4('R', '1', '2', '2'), SHEN_NAME4('R', '1', '2', '1'), SHEN_NAME4('R', '1', '2', '0'), SHEN_NAME4('R', '1', '2', '4'),
    SHEN_NAME4('R', '1', '4', '0')};

void QlgtClaimDoors(QlgtWork *work)
{
    int i;
    for (i = 0; i < QLGT_DOOR_COUNT; i++)
        FTBL.ClaimDoor(qlgtDoors[i], QLGT_DOOR_OWNER);
}

void QlgtReleaseDoors(QlgtWork *work)
{
    int i;
    for (i = 0; i < QLGT_DOOR_COUNT; i++)
        FTBL.ReleaseDoor(qlgtDoors[i], QLGT_DOOR_OWNER);
}

void QlgtTrackDrumTransparency(int mode)
{
    Vector3 camera, position;
    float alpha;
    int i;
    FTBL.camCtrlGetNextCamPos(&camera);
    for (i = 0; qlgtDrums[i]; i++) {
        if (FTBL.ctrlGetCharaByName(qlgtDrums[i])) {
            if (mode == 1)
                alpha = 1.0f;
            else {
                FTBL.GetCharaPosition(qlgtDrums[i], -1, &position, 0x40000000);
                alpha = QLGT_SQRT((camera.x - position.x) * (camera.x - position.x) + (camera.y - position.y) * (camera.y - position.y)
                    + (camera.z - position.z) * (camera.z - position.z)) - 0.6f;
                alpha /= 2.4f;
                if (alpha < 0.0f)
                    alpha = 0.0f;
            }
            if (alpha >= 1.0f)
                FTBL.EV_SetCharacterAlphaEnabled(qlgtDrums[i], 0);
            else {
                FTBL.EV_SetCharacterAlphaEnabled(qlgtDrums[i], 1);
                FTBL.SetScenarioCharacterTransparency(qlgtDrums[i], alpha);
            }
        }
    }
}

void QlgtQueueSearchPose(shen_name character, shen_name parent, int side, int delay)
{
    EVT_AT_SetPosAng(character, parent, 0.0f, 0.0f, 0.0f, delay);
    if (side) {
        EVT_AT_SetCam(7008, parent, parent, 0, 0, delay);
        EVT_AT_SetMot(character, 0x1d86, 0.0f, 0.7f, 0x40000000, delay);
    } else {
        EVT_AT_SetCam(7002, parent, parent, 0, 0, delay);
        EVT_AT_SetMot(character, 0x1d86, 0.0f, 0.7f, 0x60000000, delay);
    }
}

int QlgtFirstIntroduction(int side)
{
    int frames;
    EVT_AT_SetVoice("0263A001", 30);
    EVT_AT_SetVoice("0263B001", 80);
    EVT_AT_SetVoice("0263A002", 120);
    EVT_AT_SetVoice("0263B002", 170);
    EVT_AT_SetVoice("0263B003", 290);
    EVT_AT_SetVoice("0263B004", 380);
    EVT_AT_SetVoice("0263B005", 470);
    EVT_AT_SetVoice("0263A003", 590);
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QLGT_REN));
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QLGT_REN), 16, 1);
    if (!side) {
        EVT_AT_SetCam(7020, QLGT_START, QLGT_START, 0, 0, 0);
        EVT_AT_SetCam(7021, QLGT_START, QLGT_START, 0, 0, 40);
        EVT_AT_SetCam(7034, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, 0, 80);
        EVT_AT_SetCam(7035, QLGT_START, QLGT_START, 0, 0, 110);
        EVT_AT_SetCam(7024, QLGT_START, QLGT_START, 0, 0, 290);
        EVT_AT_SetCam(7025, QLGT_LIGHT, QLGT_LIGHT, 0, 0, 330);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, 380);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_START, -2.9f, 0.0f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, -2.2f, -0.7f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, -0.3f, -0.5f, -90.0f, 330);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, 90.0f, 290);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, 40.0f, 330);
        frames = EVT_AT_SetMot(QLGT_RYO, 7590, 20.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_RYO, 7592, 0.0f, 1.0f, 0, frames);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x40000000, 290);
        EVT_AT_SetMot(QLGT_REN, 7588, 20.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7584, 0.0f, 1.0f, 0x40000000, 110);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x40000000, 290);
        EVT_AT_SetPoint(QLGT_RYO, QLGT_REN, 5, 1, 1, 290, 240, 0x80000000);
    }
    return 530;
}

int QlgtIntroduction(int side, int course)
{
    switch (course) {
    case 1:
        EVT_AT_SetCam(7032, QLGT_START, QLGT_START, 0, 0, 0);
        EVT_AT_SetCam(7037, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, 210);
        EVT_AT_SetVoice("0263A001", 30);
        EVT_AT_SetVoice("0263A010", 50);
        EVT_AT_SetVoice("0263B002", 110);
        EVT_AT_SetVoice("0263B014", 210);
        EVT_AT_SetVoice("0263A008", 320);
        break;
    case 2:
        EVT_AT_SetCam(7023, QLGT_START, QLGT_START, 0, QLGT_NEGATIVE_ZERO, 0);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, 210);
        EVT_AT_SetVoice("0263A009", 30);
        EVT_AT_SetVoice("0263A021", 50);
        EVT_AT_SetVoice("0263B013", 110);
        EVT_AT_SetVoice("0263B019", 210);
        EVT_AT_SetVoice("0263A004", 320);
        break;
    default:
        EVT_AT_SetCam(7023, QLGT_START, QLGT_START, 0, QLGT_NEGATIVE_ZERO, 0);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, 210);
        EVT_AT_SetVoice("0263A020", 30);
        EVT_AT_SetVoice("0263A014", 50);
        EVT_AT_SetVoice("0263B018", 110);
        EVT_AT_SetVoice("0263B028", 210);
        EVT_AT_SetVoice("0263A008", 320);
        break;
    }
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QLGT_REN));
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QLGT_REN), 16, 1);
    if (side) {
        EVT_AT_SetPoint(QLGT_RYO, qlgtActors[2], 5, 1, 1, 0, 345, 0x80000000);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_START, 2.0f, 0.0f, 90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, 1.0f, -0.7f, 90.0f, 0);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, -120.0f, 210);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, 0.3f, -0.5f, 90.0f, 210);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7584, 20.0f, 0.0f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7584, 20.0f, 1.0f, 0x60000000, 60);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x60000000, 210);
    } else {
        EVT_AT_SetPoint(QLGT_RYO, qlgtActors[2], 5, 1, 1, 0, 345, 0x80000000);
        EVT_AT_SetCam(7023, QLGT_START, QLGT_START, 0, 0, 0);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, 210);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_START, -2.0f, 0.0f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, -1.0f, -0.7f, -90.0f, 0);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, 120.0f, 210);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, -0.3f, -0.5f, -90.0f, 210);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7584, 20.0f, 0.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7584, 20.0f, 1.0f, 0x40000000, 60);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x40000000, 210);
    }
    return 345;
}

int QlgtStartLectureCamera(int enemyCount)
{
    FTBL.GetRandomNumberInRange(100.0f);
    EVT_AT_SetCam(7011, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, 0, 0);
    return 1;
}

int QlgtLectureFinish(int side)
{
    int frames;
    if (side) {
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, 0.0f, -0.5f, 0.0f, 0);
        EVT_AT_SetCam(7006, QLGT_END, QLGT_END, 0, QLGT_NEGATIVE_ZERO, 0);
        frames = EVT_AT_SetMot(QLGT_REN, 7583, 50.0f, 1.0f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_REN, 0x1d87, 0.0f, 1.0f, 0x20000000, frames);
    } else {
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, 0.0f, -0.5f, 0.0f, 0);
        EVT_AT_SetCam(7006, QLGT_END, QLGT_END, 0, 0, 0);
        frames = EVT_AT_SetMot(QLGT_REN, 7583, 50.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 0x1d87, 0.0f, 1.0f, 0, frames);
    }
    return frames - 20;
}

int QlgtSuccessDialogue(int side, int course, int delay)
{
    int frames = delay + 285;
    switch (course) {
    case 0:
        EVT_AT_SetVoice("0263B006", delay + 5);
        EVT_AT_SetVoice("0263B007", delay + 85);
        EVT_AT_SetVoice("0263A004", delay + 175);
        break;
    case 1:
        EVT_AT_SetVoice("0263B015", delay + 5);
        EVT_AT_SetVoice("0263B012", delay + 75);
        EVT_AT_SetVoice("0263A008", delay + 165);
        break;
    case 2:
        EVT_AT_SetVoice("0263B020", delay + 5);
        EVT_AT_SetVoice("0263B021", delay + 75);
        EVT_AT_SetVoice("0263A022", delay + 205);
        break;
    default:
        EVT_AT_SetVoice("0263B029", delay + 5);
        EVT_AT_SetVoice("0263B007", delay + 75);
        EVT_AT_SetVoice("0263A016", delay + 165);
        break;
    }
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QLGT_REN));
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QLGT_REN), 16, 1);
    if (side) {
        EVT_AT_SetPoint(QLGT_RYO, QLGT_REN, 5, 1, 1, delay, frames - delay, 0x80000000);
        EVT_AT_SetCam(7022, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, QLGT_NEGATIVE_ZERO, delay);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, delay + 80);
        EVT_AT_SetCam(7033, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, delay + 150);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, 1.0f, 0.0f, 0.0f, delay);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_END, -2.0f, -0.5f, -90.0f, delay);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, -0.5f, -0.3f, -90.0f, delay + 80);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, 30.0f, delay + 80);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x60000000, delay);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x60000000, delay);
    } else {
        EVT_AT_SetPoint(QLGT_RYO, QLGT_REN, 5, 1, 1, delay, frames - delay, 0x80000000);
        EVT_AT_SetCam(7022, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, 0, delay);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, delay + 80);
        EVT_AT_SetCam(7033, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, delay + 150);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, -1.0f, 0.0f, 0.0f, delay);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_END, 2.0f, -0.5f, 90.0f, delay);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, 0.5f, -0.3f, 90.0f, delay + 80);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, -30.0f, delay + 80);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x40000000, delay);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x40000000, delay);
    }
    return frames;
}

int QlgtFailDialogue(int side, int course)
{
    switch (course) {
    case 0:
        EVT_AT_SetVoice("0263F001", 60);
        EVT_AT_SetVoice("0263A006", 130);
        EVT_AT_SetVoice("0263B010", 160);
        EVT_AT_SetVoice("0263F002", 270);
        break;
    case 1:
        EVT_AT_SetVoice("0263D001", 60);
        EVT_AT_SetVoice("0263A006", 130);
        EVT_AT_SetVoice("0263B010", 160);
        EVT_AT_SetVoice("0263D002", 270);
        break;
    case 2:
        EVT_AT_SetVoice("0263H001", 60);
        EVT_AT_SetVoice("0263A018", 130);
        EVT_AT_SetVoice("0263B024", 160);
        EVT_AT_SetVoice("0263H002", 270);
        break;
    default:
        EVT_AT_SetVoice("0263J001", 60);
        EVT_AT_SetVoice("0263A006", 130);
        EVT_AT_SetVoice("0263B033", 160);
        EVT_AT_SetVoice("0263J002", 270);
        break;
    }
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QLGT_REN));
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QLGT_REN), 16, 1);
    EVT_AT_CharaDisp(QLGT_RYO, 0, 1);
    EVT_AT_CharaDisp(QLGT_RYO, 230, 0);
    if (side) {
        EVT_AT_SetCam(7025, QLGT_LIGHT, QLGT_LIGHT, 0, QLGT_NEGATIVE_ZERO, 0);
        EVT_AT_SetCam(7027, qlgtActors[2], qlgtActors[2], 0, QLGT_NEGATIVE_ZERO, 60);
        EVT_AT_SetCam(7029, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, QLGT_NEGATIVE_ZERO, 150);
        EVT_AT_SetCam(7030, QLGT_BATTLE, QLGT_BATTLE, 0, 0, 230);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, -0.2f, 0.0f, 90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, -1.0f, -0.7f, 90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_BATTLE, 0.0f, 0.0f, 0.0f, 230);
        EVT_AT_SetMot(QLGT_RYO, 13, 0.0f, 1.0f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7585, 35.0f, 0.1f, 0x60000000, 150);
        EVT_AT_SetMot(qlgtActors[2], 0x1dae, 0.0f, 1.0f, 0x60000000, 60);
    } else {
        EVT_AT_SetCam(7025, QLGT_LIGHT, QLGT_LIGHT, 0, 0, 0);
        EVT_AT_SetCam(7027, qlgtActors[2], qlgtActors[2], 0, 0, 60);
        EVT_AT_SetCam(7029, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, 0, 150);
        EVT_AT_SetCam(7030, QLGT_BATTLE, QLGT_BATTLE, 0, 0, 230);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, 0.2f, 0.0f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_START, 1.0f, -0.7f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_BATTLE, 0.0f, 0.0f, 0.0f, 230);
        EVT_AT_SetMot(QLGT_RYO, 13, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_REN, 7585, 35.0f, 0.1f, 0x40000000, 150);
        EVT_AT_SetMot(qlgtActors[2], 0x1dae, 0.0f, 1.0f, 0x40000000, 60);
    }
    return 340;
}

int QlgtCallFailDialogue(int side, int course)
{
    return QlgtFailDialogue(side, course);
}

int QlgtWinDialogue(int side, int course)
{
    int frames;
    switch (course) {
    case 0:
        frames = 305;
        EVT_AT_SetVoice("0263B011", 5);
        EVT_AT_SetVoice("0263A007", 145);
        EVT_AT_SetVoice("0263B012", 185);
        EVT_AT_SetVoice("0263A008", 265);
        break;
    case 1:
        frames = 305;
        EVT_AT_SetVoice("0263B017", 5);
        EVT_AT_SetVoice("0263A007", 145);
        EVT_AT_SetVoice("0263B012", 185);
        EVT_AT_SetVoice("0263A008", 265);
        break;
    case 2:
        frames = 375;
        EVT_AT_SetVoice("0263B025", 5);
        EVT_AT_SetVoice("0263A019", 145);
        EVT_AT_SetVoice("0263B026", 185);
        EVT_AT_SetVoice("0263A008", 345);
        break;
    default:
        frames = 335;
        EVT_AT_SetVoice("0263B034", 5);
        EVT_AT_SetVoice("0263A024", 145);
        EVT_AT_SetVoice("0263B035", 185);
        EVT_AT_SetVoice("0263A025", 325);
        break;
    }
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QLGT_REN));
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QLGT_REN), 16, 1);
    if (side) {
        EVT_AT_SetPoint(QLGT_RYO, QLGT_REN, 5, 1, 1, 0, frames, 0x80000000);
        EVT_AT_SetCam(7022, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, QLGT_NEGATIVE_ZERO, 0);
        EVT_AT_SetCam(7036, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, 140);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, 0, 180);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, 1.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_END, -2.0f, -0.5f, -90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, -0.5f, -0.3f, -90.0f, 180);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, 30.0f, 180);
        EVT_AT_SetMot(QLGT_RYO, 0xf005, 0.0f, 0.7f, 0x60000000, 0);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0x20000000, 70);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x60000000, 0);
    } else {
        EVT_AT_SetPoint(QLGT_RYO, QLGT_REN, 5, 1, 1, 0, frames, 0x80000000);
        EVT_AT_SetCam(7022, QLGT_REN_DUMMY, QLGT_REN_DUMMY, 0, 0, 0);
        EVT_AT_SetCam(7036, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, 140);
        EVT_AT_SetCam(7026, QLGT_RYO_DUMMY, QLGT_RYO_DUMMY, 0, QLGT_NEGATIVE_ZERO, 180);
        EVT_AT_SetPosAng(QLGT_REN, QLGT_END, -1.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_END, 2.0f, -0.5f, 90.0f, 0);
        EVT_AT_SetPosAng(QLGT_RYO, QLGT_REN, 0.5f, -0.3f, 90.0f, 180);
        EVT_AT_SetAngY(QLGT_REN, QLGT_REN, -30.0f, 180);
        EVT_AT_SetMot(QLGT_RYO, 0xf005, 0.0f, 0.7f, 0x40000000, 0);
        EVT_AT_SetMot(QLGT_RYO, 7595, 0.0f, 1.0f, 0, 70);
        EVT_AT_SetMot(QLGT_REN, 7587, 0.0f, 1.0f, 0x40000000, 0);
    }
    return frames;
}

int QlgtEmptyDialogue(void)
{
    return 10;
}

int QlgtReturnDialogue(int delay)
{
    int frames = delay + 30;
    if (FTBL.GetRandomNumberInRange(100.0f) < 50.0f)
        EVT_AT_SetVoice("0263A026", 5);
    else
        EVT_AT_SetVoice("0263A027", 5);
    EVT_AT_SetMot(QLGT_RYO, 0xf002, 0.0f, 1.0f, 0, frames);
    return frames;
}
#endif
#else
#include "s2/areas/area_qlgt.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {

void AreaQLGTInit() {
}
void AreaQLGTMain(std::uintptr_t parameter) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(qlgt::Start, 4, 0x0b, sizeof(qlgt::Work), qlgt::taskName);
    qlgt::Work &work = *static_cast<qlgt::Work *>(HLib::GetTaskParameter(task));
    work.reserved08 = 0;
    work.task = task;
    work.retry = 0;
    work.entry = parameter ? reinterpret_cast<const qlgt::Entry *>(parameter) : &qlgt::entries[g_entryPoint];
    qlgt::Prepare(work);
}
const ScriptAreaDefinition areaQLGTScriptDefinition{.areaId = qlgt::area, .field04 = 0, .init = AreaQLGTInit, .main = AreaQLGTMain};
namespace qlgt {
namespace {
void EventNull() {
}
void Gravity(bool enabled) {
    const NpcCharacterControllerState state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(ryo, 2, state);
    EV_SetCharaControllerFlags(ryo, 3, state);
}
void ReattachRenCamera() {
    AT_SetLocalBaseChara(renDummy, ren);
    EVT_AT_SetPos(renDummy, ren, {0, 0, 0}, 0);
    EVT_AT_SetAngY(renDummy, ren, 0, 0);
}
void FinishLecture(Work &work) {
    ReattachRenCamera();
    const std::int32_t frames = LectureFinish(work.endSide);
    Film(10, 0, 255, frames - 10);
    Film(10, 255, 0, frames);
    QueueSearchPose(work.startSide, frames);
    HLib::SetCurrentTaskTo(Search);
    HLib::SetupTaskDelay(frames + 10);
}
} // namespace
void Start(void *data) {
    Work &work = *static_cast<Work *>(data);
    const Course &course = *work.course;
    EV_npcUnknownFunc(ren);
    SetNaviDisplayOverride(false);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    FinishLoading();
    EVT_AT_SetPos(fail, invalid, {course.fail.x, work.height, course.fail.z}, 0);
    EVT_AT_SetAngY(fail, invalid, 0, 0);
    EVT_AT_SetPos(enemy, invalid, {course.failBattle.x, work.height, course.failBattle.z}, 0);
    EVT_AT_SetAngY(enemy, invalid, 0, 0);
    EVT_AT_SetPos(light, invalid, {course.battle.position.x, work.height, course.battle.position.z}, 0);
    EVT_AT_SetAngY(light, invalid, course.battle.yaw, 0);
    EVT_AT_SetPos(start, invalid, {course.start.position.x, work.height, course.start.position.z}, 0);
    EVT_AT_SetAngY(start, invalid, course.start.yaw, 0);
    EVT_AT_SetPos(end, invalid, {course.end.position.x, work.height, course.end.position.z}, 0);
    EVT_AT_SetAngY(end, invalid, course.end.yaw, 0);
    EVT_AT_SetPos(drum, enemy, {0, 0, 0}, 0);
    EVT_AT_SetAngY(drum, enemy, 0, 0);
    Vector3 position;
    AT_SetLocalBaseChara(enemy, start);
    EV_GetCharaPosition(enemy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.startSide = position.x < 0;
    AT_SetLocalBaseChara(enemy, end);
    EV_GetCharaPosition(enemy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.endSide = position.x < 0;
    AT_SetLocalBaseChara(enemy, invalid);
    work.waypoint = 0;
    work.grace = 10;
    PlaceEnemies(work, 1.5f);
    AT_SetLocalBaseChara(ryoDummy, ryo);
    AT_SetLocalBaseChara(renDummy, ren);
    EVT_AT_SetPos(ryoDummy, ryo, {0, 0, 0}, 0);
    EVT_AT_SetAngY(ryoDummy, ryo, 0, 0);
    EVT_AT_SetPos(renDummy, ren, {0, 0, 0}, 0);
    EVT_AT_SetAngY(renDummy, ren, 0, 0);
    EVT_AT_SetPos(ryo, start, {0, 0, 0}, 0);
    EVT_AT_SetPos(ren, start, {0, 0, 0}, 0);
    SetPluralControlFor(ctrlGetScenarioChara(ryo), 1);
    SetPluralControlFor(ctrlGetScenarioChara(ren), 1);
    EV_LoadAudio("0263A000", 0);
    EV_spmBeginSampleExecute();
    Film(30, 255, 0);
    const std::int32_t frames = work.entry->course ? Introduction(work.startSide, work.entry->course) : FirstIntroduction(work.startSide);
    ResetFlash(*work.flash);
    TrackDrumTransparency(true);
    work.remaining = frames;
    HLib::SetCurrentTaskTo(StartLectureWait);
}
void StartLectureWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (authGetFinished() || (work.retry && HasPressedCancel())) {
        work.remaining = 0;
        EV_CleanupMapControlTask();
        FlashWork &flash = *static_cast<FlashWork *>(HLib::GetTaskParameter(flashTask));
        if (work.entry->course) {
            SkipIntroduction(work.startSide);
            flash.index = 9;
            flash.remaining = 15;
        } else {
            SkipFirstIntroduction(work.startSide);
            flash.index = 15;
            flash.remaining = 20;
        }
    }
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(StartLecture);
}
void StartLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    SetNaviDisplayOverride(true);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    GetRandomNumberInRange(100);
    Camera(7011, renDummy);
    AT_SetLocalBaseChara(renDummy, invalid);
    // Only the chase prefix is initialized; the other 31 history samples remain allocator-owned bytes.
    std::memcpy(&work.chase, &chaseDefaults, offsetof(EventChaseWork, positions));
    EV_GetCharaPosition(ren, CHARACTER_TRANSLATION_ROOT, &work.chase.positions[0], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ren, &work.chase.rotations[work.chase.writeIndex], EventTransformFlags::World);
    EV_AT_SetPos(renDummy, EventTransformFlags::World | EventTransformFlags::AllAxes, work.chase.positions[work.chase.writeIndex]);
    EV_AT_SetAngY(renDummy, EventTransformFlags::World | EventTransformFlags::AllAxes, work.chase.rotations[work.chase.writeIndex]);
    EVT_AT_SetAngY(renDummy, start, work.startSide ? 90.0f : -90.0f, 0);
    HLib::SetCurrentTaskTo(RunLecture);
    HLib::SetupTaskDelay(1);
}
void RunLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.flash->transition) {
        if (authGetFinished() || (work.retry && HasPressedCancel()))
            FinishLecture(work);
    } else {
        mtwkSetRegionFlag(scenFindMotionwork(ren), 71, 2, MotionRegion::LowerBody);
        work.movement = static_cast<std::int32_t>(QueueEventMovement(nullptr, ren, work.course->route, &routeMotions, 1, 0x80420000U));
        if (eventMovementStates[work.movement]) {
            EventMovementWork *movement = static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work.movement]));
            movement->index = work.waypoint;
            StartEventMovementSegment(movement);
            ConfigureEventMovementMotions(movement);
            movement->active = 1;
        }
        HLib::SetCurrentTaskTo(WaitLecture);
        HLib::SetupTaskDelay(10);
    }
}
void WaitLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (!eventMovementStates[work.movement] || authGetFinished() || (work.retry && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        CancelEventMovement(work.movement);
        FinishLecture(work);
        return;
    }
    const std::int32_t waypoint =
        eventMovementStates[work.movement] ? static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work.movement]))->index : -1;
    if (work.waypoint == waypoint) {
        UpdateEventChase(&work.chase);
        return;
    }
    work.waypoint = waypoint;
    if (work.course->routeActions[waypoint] == 1) {
        CancelEventMovement(work.movement);
        const std::int32_t frames = EVT_AT_SetMot(ren, 0xf002, 0, 1, 0, 0);
        HLib::SetCurrentTaskTo(RunLecture);
        HLib::SetupTaskDelay(frames - 30);
    } else if (work.course->routeActions[waypoint + 1] == 1) {
        CancelEventMovement(work.movement);
        mtwkSetRegionFlag(scenFindMotionwork(ren), 71, 2, MotionRegion::LowerBody);
        work.movement = static_cast<std::int32_t>(QueueEventMovement(nullptr, ren, work.course->route, &routeMotions, 1, 0x80220000U));
        SetEventMovementSegment(work.movement, work.waypoint);
    }
}
void EndLecture(void *) {
    using enum CameraState;
    EV_LoadAudio("0263A000", 1);
    Film(10, 255, 0);
    FinishLoading();
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EVT_AT_SetMot(ryo, 0xf002, 2, 1, 0x40000000U, 0);
    Gravity(true);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(Play);
}
void Play(void *data) {
    Work &work = *static_cast<Work *>(data);
    const Course &course = *work.course;
    SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const bool inside = TestCharaPositionWithinRange(ryo, &course.boundsFirst, &course.boundsSecond, shen_name{});
    const bool exposed = ComputeAngle(position, course.enemyRadius, 0, 65353, course.enemy.position);
    const bool failed = ComputeAngle(position, course.failRadius, 0, 65353, course.fail);
    const bool atStart = ComputeAngle(position, course.startRadius, 0, 65353, course.start.position);
    const bool atGoal = ComputeAngle(position, course.goalRadius, 0, 65353, course.goal);
    if (work.flash->state == 2 && !IsInSafeZone(course.safeZones) && exposed) {
        const std::int32_t grace = work.grace--;
        if (grace < 0)
            HLib::SetCurrentTaskTo(Fail);
        return;
    }
    if (failed)
        HLib::SetCurrentTaskTo(Fail);
    else if (atStart) {
        GetBattleDataConfig(0, battleData);
        GetBattleDataConfig(1, battleData);
        if (GetBattleDataConfig(2, battleData)) {
            SetPlayerControlEnabled(0);
            Gravity(false);
            Film(10, 0, 255);
            Film(10, 255, 0, 10);
            QueueSearchPose(work.startSide, 10);
            HLib::SetCurrentTaskTo(Search);
            HLib::SetupTaskDelay(10);
        }
    } else if (atGoal) {
        SetPlayerControlEnabled(0);
        EventNull();
        Film(10, 0, 255);
        HLib::SetCurrentTaskTo(Success);
        HLib::SetupTaskDelay(40);
    } else if (inside)
        work.grace = 10;
    else {
        SetPlayerControlEnabled(0);
        EventNull();
        const std::int32_t frames = ReturnDialogue();
        EVT_AT_SetPosAng(ryo, invalid, course.returnPosition.position.x, course.returnPosition.position.z, course.returnPosition.yaw, frames);
        Film(10, 0, 255);
        HLib::SetCurrentTaskTo(EndLecture);
        HLib::SetupTaskDelay(frames);
    }
}
void Search(void *data) {
    Work &work = *static_cast<Work *>(data);
    GetBattleDataConfig(0, battleData);
    GetBattleDataConfig(1, battleData);
    if (GetBattleDataConfig(2, battleData)) {
        Film(10, 0, 255);
        EVT_AT_SetPosAng(ryo, start, 0, -.7f, work.startSide ? 90.0f : -90.0f, 10);
        HLib::SetCurrentTaskTo(EndLecture);
        HLib::SetupTaskDelay(10);
    }
}
void Success(void *data) {
    Work &work = *static_cast<Work *>(data);
    Gravity(false);
    SetNaviDisplayOverride(false);
    EV_LoadAudio("0263A000", 0);
    Film(30, 255, 0);
    const std::int32_t frames = SuccessDialogue(work.endSide, work.entry->course);
    Film(30, 0, 255, frames - 30);
    if (frames == 60)
        spmFadeOut();
    else
        EVT_AT_FadeBGM(0, 1, frames - 60);
    work.remaining = frames;
    HLib::SetCurrentTaskTo(SuccessWait);
}
void SuccessWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (authGetFinished()) {
        work.remaining = 0;
        EV_CleanupMapControlTask();
    }
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(End);
}
void Fail(void *data) {
    Work &work = *static_cast<Work *>(data);
    SetPlayerControlEnabled(0);
    EventNull();
    Gravity(false);
    SetNaviDisplayOverride(false);
    EV_LoadAudio("0263A000", 2);
    ResetFlash(*work.flash);
    const std::int32_t frames = FailDialogue(work.endSide, work.entry->course);
    PlaceBattleEnemies(work, 3);
    HLib::SetCurrentTaskTo(FailWait);
    work.remaining = frames;
}
void FailWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(StartBattle);
}
void End(void *data) {
    Work &work = *static_cast<Work *>(data);
    Cleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == area.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
} // namespace qlgt
} // namespace shenmue::s2::areas
#endif
