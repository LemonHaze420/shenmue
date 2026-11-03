#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_darts.h"

extern void *memset(void *destination, int value, unsigned int size);

#define DARTS_PACKAGE "DARTS"
#define DARTS_AREA "DART.EV3"
#define DARTS_ARROW_SLOTS 15
#define DARTS_ENTER_GAME(work) \
    { \
        FTBL.srchReset(); \
        FTBL.LoadAreaFunc_Wrapper_0(DARTS_AREA, 0); \
        FTBL.ExecuteCurrAreaMainFunc(DARTS_AREA, &(work)->game); \
        FTBL.HLib_SetupTaskDelay(0); \
    }
#define DARTS_CLEAR_REQUESTS() \
    { \
        FTBL.EV_SetPlayerFlag(12, 1419, 0); \
        FTBL.EV_SetPlayerFlag(12, 1420, 0); \
        FTBL.EV_SetPlayerFlag(12, 1422, 0); \
        FTBL.EV_SetPlayerFlag(12, 1423, 0); \
    }
#define DARTS_FSQRT(x) \
    do { \
        (x) = __builtin_sqrtf(x); \
    } while (0)
#define DARTS_RING_OFFSET(range, offset) \
    do { \
        distance = FTBL.GetRandomNumberInRange(range); \
        y = (offset); \
        x = y; \
    } while (0)

#define DARTS_DEFINE_PREPARE_PLACEMENT() \
    void EVT_PrepareDartsPlacement(EventDartsPlacement *placement) \
    { \
        int score = 0; \
        if (!FTBL.DoesScenarioCharacterExist(placement->character)) \
            return; \
        FTBL.EV_EnsureCharacterDraw(placement->character); \
        FTBL.EV_AT_SetPos(placement->character, 0x38000000, &placement->position); \
        FTBL.EV_AT_SetAngY_0(placement->character, 0x38000000, &placement->rotation, 0); \
        FTBL.scenCopyCharaToGame(placement->character); \
        if (placement->character != DARTS_COMPACT_BOARD) { \
            UpdateDartsScoreDisplay(placement->character, 0, 2); \
            UpdateDartsScoreDisplay(placement->character, 0, 3); \
            score = FTBL.GetSavedMinigameScore(2); \
            if (score < 777777) \
                score = 777777; \
            UpdateDartsScoreDisplay(placement->character, score, 0); \
            score = FTBL.GetSavedMinigameScore(3); \
            UpdateDartsScoreDisplay(placement->character, score, 1); \
        } else { \
            UpdateDartsCompactScoreDisplay(placement->character, score, 3); \
            score = FTBL.GetSavedMinigameScore(12); \
            if (score < 50) \
                score = 50; \
            UpdateDartsCompactScoreDisplay(placement->character, score, 0); \
            score = FTBL.GetSavedMinigameScore(13); \
            UpdateDartsCompactScoreDisplay(placement->character, score, 1); \
        } \
    }

typedef struct EventDartsAiScore {
    unsigned char aiParameters[2];
    short reserved02;
    int bestScore, opponentScore, opponentScoreFloor;
} EventDartsAiScore;
typedef struct EventDartsAiRecord {
    shen_name character;
    int motionA, motionB;
    signed char *pattern;
} EventDartsAiRecord;
typedef struct EventDartsSavedScores {
    shen_name character;
    int bestScore, opponentScore;
} EventDartsSavedScores;
typedef struct EventDartsArrowWork {
    shen_name character, arrow;
} EventDartsArrowWork;
typedef struct EventDartsPositionWork {
    int cleanupTask, hasRotation, hasPosition;
    Vector3i rotation;
    Vector3 position;
    shen_name parent, character;
} EventDartsPositionWork;
typedef struct EventDartsHelpWork {
    EventDartsHelpIcon *icons;
    HLibTask *task;
} EventDartsHelpWork;

EventDartsAiScore dartsAiScores[10] = {
    {{25, 0}, 0, 4333000, 2041000, 250000},
    {{20, 0}, 0, 5297000, 4340000, 250000},
    {{35, 0}, 0, 1058000, 770000, 250000},
    {{45, 25}, 0, 3745000, 892000, 250000},
    {{20, 0}, 0, 3745000, 1812000, 250000},
    {{20, 0}, 0, 5297000, 4667000, 500000},
    {{25, 0}, 0, 4333000, 2761000, 500000},
    {{30, 0}, 0, 3745000, 1812000, 500000},
    {{35, 0}, 0, 1058000, 1013000, 500000},
};
signed char xhoDartsPattern[] = {1, 12, 8, 25, 25, 50, 50, -1};
signed char hlnDartsPattern[] = {12, 18, 6, 16, -1};
signed char jonDartsPattern[] = {18, 6, 25, 2, 3, 25, 9, 8, 33, -1};
signed char fcjDartsPattern[] = {5, 18, 13, 15, 3, 25, 25, 25, 50, -1};
signed char hcyDartsPattern[] = {20, 13, 15, 19, 16, 14, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, -1};
EventDartsAiRecord dartsAiRecords[5] = {
    {SHEN_NAME4('F', 'C', 'J', '_'), 0x1a82, 0x1a81, fcjDartsPattern},
    {SHEN_NAME4('H', 'C', 'Y', '_'), 0x1a8e, 0x1a8d, hcyDartsPattern},
    {SHEN_NAME4('H', 'L', 'N', '_'), 0x1ab9, 0x1ab8, hlnDartsPattern},
    {SHEN_NAME4('J', 'O', 'N', '_'), 0x1a9a, 0x1a99, jonDartsPattern},
    {SHEN_NAME4('X', 'H', 'O', '_'), 0x1aa0, 0x1a9f, xhoDartsPattern},
};
#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
Vector3i dartsArrowRotation = {0x48df, 0x7a9f, 0xc111};
Vector3 dartsJonArrowOffset = {0.14f, -0.06f, 0.04f};
Vector3 dartsDefaultArrowOffset = {0.14f, -0.06f, -0.02f};
Vector3 dartsFcjHlnArrowOffset = {0.11f, -0.09f, -0.0f};
#endif
Vector3 dartsScoreStep = {-0.05f, 0.0f, 0.0f};
Vector3 dartsDigitStep = {-0.025f, 0.0f, 0.0f};
Vector3 dartsScoreOrigin = {0.0224f, 2.155f, -0.04f};
Vector3 dartsDigitOrigin = {0.1474f, 2.138f, -0.04f};
float dartsScoreRows[5] = {2.155f, 2.005f, 1.855f, 1.705f, 1.855f};
float dartsDigitRows[5] = {2.138f, 1.988f, 1.838f, 1.688f, 1.838f};
float dartsCompactSteps[4] = {-0.0474f, -0.0474f, 0.0f, -0.1061f};
Vector3 dartsCompactOrigins[4] = {{0.1905f, 1.7258f, -0.0388f}, {-0.0585f, 1.7258f, -0.0388f}, {0.0f, 0.0f, 0.0f}, {0.1344f, 1.91f, -0.0388f}};
Vector3 dartsCompactScales[4] = {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.8f, 1.6f, 1.0f}};
EventDartsSavedScores dartsSavedScores[5] = {
    {SHEN_NAME4('F', 'C', 'J', '_'), 16, 17},
    {SHEN_NAME4('H', 'C', 'Y', '_'), 18, 19},
    {SHEN_NAME4('H', 'L', 'N', '_'), 20, 21},
    {SHEN_NAME4('J', 'O', 'N', '_'), 22, 23},
    {SHEN_NAME4('X', 'H', 'O', '_'), 24, 25},
};

static Vector3 dartsCompactStep;
#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
static int dartsHelpState;
#endif
static shen_name dartsTemporaryCharacter;

void EventDartsEasyCallback(void *taskData);
void EventEnemyDartsCallback(void *taskData);
void EventDartsArrowCallback(void *taskData);
void DartsPositionCallback(void *taskData);
void DartsHelpIconCallback(void *taskData);
void EVT_PrepareDartsPlacement(EventDartsPlacement *placement);
int IsRyoFacingDartsCharacter(shen_name character, int fov);
void BeginDartsEvent(int mode);
void EndDartsEvent(void);
void SetDartsTemporaryCharacter(shen_name character);
#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
int SetDartsAiMotion(shen_name character, unsigned int flags);
#else
int SetDartsAiMotion(shen_name character);
#endif
void SetDartsPosition(shen_name parent, shen_name character, float x, float y, float z, int yaw, int delay);
void UpdateDartsScoreDisplay(shen_name board, int score, int row);
void UpdateDartsCompactScoreDisplay(shen_name board, int score, int row);
void QueueDartsPosition(shen_name parent, shen_name character, Vector3 *position, Vector3i *rotation, int delay);
HLibTask *QueueDartsArrow(shen_name character, shen_name arrow);
void RestoreOpponentDartsScores(shen_name character, int save, int *bestScore, int *opponentScore);
void PrepareDartsArrows(EventDartsWork *work);
int CalculateDartsPositionOrScore(char score, Vector3 *position);
shen_name GetDartsArrowName(int set, int index);

#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
void EVT_StartDartsGame(EventDartsGame *game, int placement)
{
    game->placement += placement;
    game->mode = placement;
    EVT_PrepareDartsPlacement(game->placement);
    FTBL.srchReset();
    FTBL.LoadAreaFunc_Wrapper_0(DARTS_AREA, 0);
    FTBL.ExecuteCurrAreaMainFunc(DARTS_AREA, game);
}

HLibTask *EVT_DartsEasySingle(EventDartsPlacement *placement, int count)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventDartsEasyCallback, 4, 11, sizeof(EventDartsWork), TASK_EVNT);
    EventDartsWork *work = (EventDartsWork *)FTBL.HLib_GetTaskParameter(task);
    int i;
    if (work) {
        memset(work, 0, sizeof(EventDartsWork));
        work->placement = count;
        work->game.opponentMatch = 0;
        work->game.task = task;
        work->game.placement = placement;
        EVT_EnsureDartsPackageMounted();
        for (i = 0; i < count; i++) {
            EVT_PrepareDartsPlacement(placement);
            placement++;
        }
    } else
        task = 0;
    return task;
}

void EventDartsEasyCallback(void *taskData)
{
    EventDartsWork *work = (EventDartsWork *)taskData;
    if (work) {
        if (work->state == 4)
            work->state = 0;
        if (FTBL.EV_GetPlayerFlag(11, 1419)) {
            DARTS_CLEAR_REQUESTS()
            work->state = 4;
            work->game.mode = 0;
            work->game.opponent = 0;
            BeginDartsEvent(0);
            DARTS_ENTER_GAME(work)
        } else if (FTBL.EV_GetPlayerFlag(11, 1420)) {
            DARTS_CLEAR_REQUESTS()
            work->state = 4;
            work->game.mode = 1;
            work->game.opponent = 0;
            BeginDartsEvent(0);
            DARTS_ENTER_GAME(work)
        } else if (FTBL.EV_GetPlayerFlag(11, 1422)) {
            DARTS_CLEAR_REQUESTS()
            work->state = 4;
            work->game.mode = 2;
            work->game.opponent = 0;
            BeginDartsEvent(0);
            DARTS_ENTER_GAME(work)
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *EVT_SetEnemyDarts(EventDartsPlacement *placement, EventDartsEnemy *enemy, int index)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventEnemyDartsCallback, 4, 11, sizeof(EventDartsWork), TASK_EVNT);
    EventDartsWork *work = (EventDartsWork *)FTBL.HLib_GetTaskParameter(task);
    if (work) {
        memset(work, 0, sizeof(EventDartsWork));
        work->task = task;
        work->matchTask = enemy->matchTask;
        work->placement = index;
        work->game.opponentMatch = 0;
        work->game.task = task;
        work->game.placement = placement;
        work->game.enemy = enemy;
        EVT_EnsureDartsPackageMounted();
        EVT_PrepareDartsPlacement(placement + index);
    } else
        task = 0;
    return task;
}

void EventEnemyDartsCallback(void *taskData)
{
    EventDartsWork *work = (EventDartsWork *)taskData;
    Vector3i rotation;
    int aiIndex;
    if (work) {
        if (FTBL.DoesScenarioCharacterExist(work->game.enemy->name)) {
    switch (work->state) {
    case 0:
        SetDartsAiMotion(work->game.enemy->name, 0x40000000);
        if (g_currentMap == SHEN_NAME4('A', 'K', 'S', '0') && work->game.enemy->name == SHEN_NAME4('X', 'H', 'O', '_'))
            SetDartsPosition(work->game.placement->character, work->game.enemy->name, -0.3f, 0.0f, 3.6f, 0, 1);
        else
            SetDartsPosition(work->game.placement->character, work->game.enemy->name, 0.0f, 0.0f, 3.6f, 0, 1);
    case 1:
        work->state = 2;
        FTBL.GetScenarioCharacterEulerAngles(work->game.placement->character, &rotation, 0x40000000);
        FTBL.npcSetFlags(work->game.enemy->name, rotation.y);
        FTBL.EV_EnableCharacterDisplay(work->game.placement->character);
        FTBL.EV_SetCharacterAlphaEnabled(work->game.enemy->name, 1);
        QueueDartsArrow(work->game.enemy->name, GetDartsArrowName(2, 14));
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(work->game.enemy->name), 3);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(work->game.enemy->name), 1);
        FTBL.EV_SetCharaControllerFlags(work->game.enemy->name, 2, 0);
        FTBL.EV_SetCharaControllerFlags(work->game.enemy->name, 3, 0);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.EV_SetCharaControllerFlags(work->game.enemy->name, 1, 0);
        RestoreOpponentDartsScores(work->game.enemy->name, 0, &work->game.bestScore, &work->game.opponentScore);
        PrepareDartsArrows(work);
        aiIndex = work->game.enemy->aiIndex;
        if (work->game.bestScore < dartsAiScores[aiIndex].bestScore)
            work->game.bestScore = dartsAiScores[aiIndex].bestScore;
        if (work->game.opponentScore == 0)
            work->game.opponentScore = dartsAiScores[aiIndex].opponentScore;
        else if (work->game.opponentScore < dartsAiScores[aiIndex].opponentScoreFloor)
            work->game.opponentScore = dartsAiScores[aiIndex].opponentScoreFloor;
        work->game.opponentScoreFloor = dartsAiScores[aiIndex].opponentScoreFloor;
        work->game.aiParameters[0] = dartsAiScores[aiIndex].aiParameters[0];
        work->game.aiParameters[1] = dartsAiScores[aiIndex].aiParameters[1];
        FTBL.scenCopyCharaToGame(work->game.placement->character);
        UpdateDartsScoreDisplay(work->game.placement->character, work->game.bestScore, 0);
        UpdateDartsScoreDisplay(work->game.placement->character, work->game.opponentScore, 1);
        UpdateDartsScoreDisplay(work->game.placement->character, 0, 2);
        if (dartsAiScores[aiIndex].opponentScore - dartsAiScores[aiIndex].opponentScoreFloor > work->game.opponentScore)
            UpdateDartsScoreDisplay(work->game.placement->character, work->game.opponentScore, 3);
        else
            UpdateDartsScoreDisplay(work->game.placement->character, dartsAiScores[aiIndex].opponentScore - dartsAiScores[aiIndex].opponentScoreFloor, 3);
        break;
    case 2:
        if (FTBL.EV_GetPlayerFlag(11, 1423)) {
            DARTS_CLEAR_REQUESTS()
            work->state = 3;
            work->game.mode = work->placement;
            work->game.opponentMatch = 1;
            work->game.opponent = DARTS_MATCH_OPPONENT;
            work->game.task = work->task;
            BeginDartsEvent(DARTS_MATCH_OPPONENT);
            SetDartsTemporaryCharacter(work->game.enemy->name);
            FTBL.srchReset();
            FTBL.LoadAreaFunc_Wrapper_0(DARTS_AREA, 0);
            FTBL.ExecuteCurrAreaMainFunc(DARTS_AREA, &work->game);
            FTBL.HLib_SetupTaskDelay(0);
        } else if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->game.enemy->name), 65, 1, 0) && FTBL.ryoNotBusy()) {
            SetDartsAiMotion(work->game.enemy->name, 0x2000000);
            if (g_currentMap == SHEN_NAME4('A', 'K', 'S', '0') && work->game.enemy->name == SHEN_NAME4('X', 'H', 'O', '_'))
                SetDartsPosition(work->game.placement->character, work->game.enemy->name, -0.3f, 0.0f, 3.6f, 0, 1);
            else
                SetDartsPosition(work->game.placement->character, work->game.enemy->name, 0.0f, 0.0f, 3.6f, 0, 1);
        }
        break;
    case 3:
        work->state = 1;
        EndDartsEvent();
        break;
    }
        } else
            EventDartsEasyCallback(work);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void BeginDartsEvent(int mode)
{
    FTBL.SetPlayerControlEnabled(0);
}

void EndDartsEvent(void)
{
    FTBL.SetPlayerControlEnabled(1);
}
#endif

void EVT_EnsureDartsPackageMounted(void)
{
    if (!FTBL.EV_ControlPackageResources(0, 5, DARTS_PACKAGE, 0, 0)) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)DARTS_PACKAGE, 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
}

void EVT_ReleaseDartsPackage(void)
{
    if (FTBL.EV_ControlPackageResources(0, 5, DARTS_PACKAGE, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, DARTS_PACKAGE, 0, 0);
}

#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
void EVT_StartDartsGame(EventDartsGame *game, int placement)
{
    game->placement += placement;
    EVT_PrepareDartsPlacement(game->placement);
    game->mode = placement;
    EVT_SetDartsHelpState(1);
    FTBL.srchReset();
    FTBL.LoadAreaFunc_Wrapper_0(DARTS_AREA, 0);
    FTBL.ExecuteCurrAreaMainFunc(DARTS_AREA, game);
}

HLibTask *EVT_DartsEasySingle(EventDartsPlacement *placement, int count)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventDartsEasyCallback, 4, 11, sizeof(EventDartsWork), TASK_EVNT);
    EventDartsWork *work = (EventDartsWork *)FTBL.HLib_GetTaskParameter(task);
    int i;
    if (work) {
        EVT_EnsureDartsPackageMounted();
        memset(work, 0, sizeof(EventDartsWork));
        work->placement = count;
        work->game.opponentMatch = 0;
        work->game.task = task;
        work->game.placement = placement;
        for (i = 0; i < count; i++) {
            EVT_PrepareDartsPlacement(placement);
            placement++;
        }
    } else
        task = 0;
    return task;
}

void EventDartsEasyCallback(void *taskData)
{
    EventDartsWork *work = (EventDartsWork *)taskData;
    if (work) {
        if (work->state == 4) {
            work->state = 0;
            EndDartsEvent();
        }
        if (FTBL.EV_GetPlayerFlag(11, 1419)) {
            work->state = 4;
            BeginDartsEvent(0);
            work->game.mode = 0;
            work->game.opponent = 0;
            DARTS_ENTER_GAME(work)
        } else if (FTBL.EV_GetPlayerFlag(11, 1420)) {
            work->state = 4;
            BeginDartsEvent(0);
            work->game.mode = 1;
            work->game.opponent = 0;
            DARTS_ENTER_GAME(work)
        } else if (FTBL.EV_GetPlayerFlag(11, 1422)) {
            work->state = 4;
            BeginDartsEvent(0);
            work->game.mode = 2;
            work->game.opponent = 0;
            DARTS_ENTER_GAME(work)
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

DARTS_DEFINE_PREPARE_PLACEMENT()

int IsRyoFacingDartsCharacter(shen_name character, int fov)
{
    Vector3 ryoPosition, position;
    int facing = -1;
    SHENMUE_EVENT_BOUNDARY();
    if (FTBL.DoesScenarioCharacterExist(character) && FTBL.DoesScenarioCharacterExist(SHEN_NAME4('R', 'Y', 'O', '_'))) {
        FTBL.GetCharaPosition(character, -1, &position, 0);
        FTBL.GetCharaPosition(SHEN_NAME4('R', 'Y', 'O', '_'), -1, &ryoPosition, 0);
        facing = FTBL.ComputeAngle(&position, 3.0f, 0, fov, &ryoPosition) != 0;
    }
    return facing;
}

void BeginDartsEvent(int mode)
{
    FTBL.SetPlayerControlEnabled(0);
    EVT_SetDartsHelpState(1);
    if (mode != 0) {
        if (mode == DARTS_MATCH_OPPONENT)
            FTBL.EV_EventSetUpHelpIcon_0(76, 0);
    }
}

void EndDartsEvent(void)
{
    FTBL.SetPlayerControlEnabled(1);
    EVT_SetDartsHelpState(0);
}
#endif

HLibTask *QueueDartsArrow(shen_name character, shen_name arrow)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventDartsArrowCallback, 4, 11, sizeof(EventDartsArrowWork), TASK_EVNT);
    EventDartsArrowWork *work = (EventDartsArrowWork *)FTBL.HLib_GetTaskParameter(task);
    if (work) {
        dartsTemporaryCharacter = 0;
        work->character = character;
        work->arrow = arrow;
    } else
        task = 0;
    return task;
}

#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
void EventDartsArrowCallback(void *taskData)
{
    EventDartsArrowWork *work = (EventDartsArrowWork *)taskData;
    if (work) {
        if (work->character == dartsTemporaryCharacter) {
            FTBL.HLib_DestroyCurrentTask();
            FTBL.ResetScenarioCharacterAttachment(work->arrow);
        } else if (FTBL.DoesScenarioCharacterExist(work->character) && FTBL.DoesScenarioCharacterExist(work->arrow)) {
            FTBL.EV_EnableCharacterDisplay(work->arrow);
            FTBL.EV_EnsureCharacterDraw(work->arrow);
            if (work->character == SHEN_NAME4('J', 'O', 'N', '_'))
                FTBL.AttachScenarioCharacter(work->arrow, work->character, 21, &dartsJonArrowOffset, &dartsArrowRotation);
            else if (work->character == SHEN_NAME4('F', 'C', 'J', '_') || work->character == SHEN_NAME4('H', 'L', 'N', '_'))
                FTBL.AttachScenarioCharacter(work->arrow, work->character, 14, &dartsFcjHlnArrowOffset, &dartsArrowRotation);
            else
                FTBL.AttachScenarioCharacter(work->arrow, work->character, 14, &dartsDefaultArrowOffset, &dartsArrowRotation);
        } else {
            FTBL.EV_DisableCharacterDisplay(work->arrow);
            FTBL.EV_ToggleCharacterFlag(work->arrow);
            FTBL.HLib_DestroyCurrentTask();
            FTBL.ResetScenarioCharacterAttachment(work->arrow);
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}
#else
void EventDartsArrowCallback(void *taskData)
{
    EventDartsArrowWork *work = (EventDartsArrowWork *)taskData;
    Vector3 offset;
    Vector3i rotation;
    if (work) {
        if (work->character == dartsTemporaryCharacter) {
            FTBL.HLib_DestroyCurrentTask();
            FTBL.ResetScenarioCharacterAttachment(work->arrow);
        } else if (FTBL.DoesScenarioCharacterExist(work->character) && FTBL.DoesScenarioCharacterExist(work->arrow)) {
            offset.x = 0.123f;
            offset.y = -0.058f;
            offset.z = -0.03f;
            rotation.x = 0x48df;
            rotation.y = 0x7a9f;
            rotation.z = 0xc111;
            FTBL.EV_EnableCharacterDisplay(work->arrow);
            FTBL.EV_EnsureCharacterDraw(work->arrow);
            if (work->character == SHEN_NAME4('J', 'O', 'N', '_'))
                FTBL.AttachScenarioCharacter(work->arrow, work->character, 21, &offset, &rotation);
            else
                FTBL.AttachScenarioCharacter(work->arrow, work->character, 14, &offset, &rotation);
        } else {
            FTBL.EV_DisableCharacterDisplay(work->arrow);
            FTBL.EV_ToggleCharacterFlag(work->arrow);
            FTBL.HLib_DestroyCurrentTask();
            FTBL.ResetScenarioCharacterAttachment(work->arrow);
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}
#endif

void SetDartsTemporaryCharacter(shen_name character)
{
    dartsTemporaryCharacter = character;
}

#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
int SetDartsAiMotion(shen_name character, unsigned int flags)
#else
int SetDartsAiMotion(shen_name character)
#endif
{
    int motion = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (character == dartsAiRecords[i].character) {
            if (FTBL.GetRandomNumberInRange(1.0f) < 0.5f)
                motion = dartsAiRecords[i].motionA;
            else
                motion = dartsAiRecords[i].motionB;
            break;
        }
    }
    if (!motion)
        motion = dartsAiRecords[0].motionA;
#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
    EVT_AT_SetMot(character, motion, -1.0f, 1.0f, flags, 1);
#else
    EVT_AT_SetMot(character, motion, -1.0f, 1.0f, 0x40000000, 1);
#endif
    return motion;
}

#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
HLibTask *EVT_SetEnemyDarts(EventDartsPlacement *placement, EventDartsEnemy *enemy, int index)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventEnemyDartsCallback, 4, 11, sizeof(EventDartsWork), TASK_EVNT);
    EventDartsWork *work = (EventDartsWork *)FTBL.HLib_GetTaskParameter(task);
    if (work) {
        EVT_EnsureDartsPackageMounted();
        memset(work, 0, sizeof(EventDartsWork));
        work->task = task;
        work->matchTask = enemy->matchTask;
        work->placement = index;
        work->game.opponentMatch = 0;
        work->game.task = task;
        work->game.placement = placement;
        work->game.enemy = enemy;
        EVT_PrepareDartsPlacement(placement + index);
    } else
        task = 0;
    return task;
}

void EventEnemyDartsCallback(void *taskData)
{
    EventDartsWork *work = (EventDartsWork *)taskData;
    int aiIndex;
    if (work) {
        if (FTBL.DoesScenarioCharacterExist(work->game.enemy->name)) {
    switch (work->state) {
    case 0:
        SetDartsAiMotion(work->game.enemy->name);
        SetDartsPosition(work->game.placement->character, work->game.enemy->name, 0.0f, 0.0f, 4.0f, 0, 1);
    case 1:
        work->state = 2;
        FTBL.UnknownCharaFunc(work->game.placement->character);
        FTBL.EV_EnableCharacterDisplay(work->game.placement->character);
        FTBL.EV_SetCharacterAlphaEnabled(work->game.enemy->name, 1);
        QueueDartsArrow(work->game.enemy->name, GetDartsArrowName(2, 14));
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(work->game.enemy->name), 3);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(work->game.enemy->name), 1);
        FTBL.scenCopyCharaToGame(work->game.placement->character);
        RestoreOpponentDartsScores(work->game.enemy->name, 0, &work->game.bestScore, &work->game.opponentScore);
        PrepareDartsArrows(work);
        aiIndex = work->game.enemy->aiIndex;
        if (work->game.bestScore < dartsAiScores[aiIndex].bestScore)
            work->game.bestScore = dartsAiScores[aiIndex].bestScore;
        if (work->game.opponentScore == 0)
            work->game.opponentScore = dartsAiScores[aiIndex].opponentScore;
        else if (work->game.opponentScore < dartsAiScores[aiIndex].opponentScoreFloor)
            work->game.opponentScore = dartsAiScores[aiIndex].opponentScoreFloor;
        work->game.opponentScoreFloor = dartsAiScores[aiIndex].opponentScoreFloor;
        work->game.aiParameters[0] = dartsAiScores[aiIndex].aiParameters[0];
        work->game.aiParameters[1] = dartsAiScores[aiIndex].aiParameters[1];
        UpdateDartsScoreDisplay(work->game.placement->character, work->game.bestScore, 0);
        UpdateDartsScoreDisplay(work->game.placement->character, work->game.opponentScore, 1);
        UpdateDartsScoreDisplay(work->game.placement->character, 0, 2);
        if (dartsAiScores[aiIndex].opponentScore - dartsAiScores[aiIndex].opponentScoreFloor > work->game.opponentScore)
            UpdateDartsScoreDisplay(work->game.placement->character, work->game.opponentScore, 3);
        else
            UpdateDartsScoreDisplay(work->game.placement->character, dartsAiScores[aiIndex].opponentScore - dartsAiScores[aiIndex].opponentScoreFloor, 3);
        break;
    case 2:
        if (FTBL.EVT_GetControllerInput(2, 1) == 1024 && work->matchTask) {
            FTBL.HLib_ResolveDeferredCallback(work->matchTask);
            SetDartsTemporaryCharacter(work->game.enemy->name);
            SetDartsPosition(work->game.placement->character, work->game.enemy->name, 0.0f, -10.0f, 0.0f, 0, 0);
            FTBL.HLib_DestroyCurrentTask();
        } else if (FTBL.EV_GetPlayerFlag(11, 1423)) {
            work->state = 3;
            BeginDartsEvent(DARTS_MATCH_OPPONENT);
            SetDartsTemporaryCharacter(work->game.enemy->name);
            work->game.mode = work->placement;
            work->game.opponentMatch = 1;
            work->game.opponent = DARTS_MATCH_OPPONENT;
            work->game.task = work->task;
            FTBL.srchReset();
            FTBL.LoadAreaFunc_Wrapper_0(DARTS_AREA, 0);
            FTBL.ExecuteCurrAreaMainFunc(DARTS_AREA, &work->game);
            FTBL.HLib_SetupTaskDelay(0);
        } else if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->game.enemy->name), 65, 1, 0))
            SetDartsAiMotion(work->game.enemy->name);
        break;
    case 3:
        work->state = 1;
        EndDartsEvent();
        break;
    }
        } else
            EventDartsEasyCallback(work);
    } else
        FTBL.HLib_DestroyCurrentTask();
}
#endif

void SetDartsPosition(shen_name parent, shen_name character, float x, float y, float z, int yaw, int delay)
{
    Vector3i rotation;
    Vector3 position;
    rotation.x = rotation.z = 0;
    position.x = x;
    position.y = y;
    position.z = z;
    rotation.y = yaw;
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 71, 0, 0);
    QueueDartsPosition(parent, character, &position, &rotation, delay);
}

#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
DARTS_DEFINE_PREPARE_PLACEMENT()
#endif

void UpdateDartsScoreDisplay(shen_name board, int score, int row)
{
    Vector3 scale, position;
    int high, low, i;
    scale.x = 0.5f;
    scale.y = 0.5f;
    scale.z = 0.5f;
    high = score / 10000;
    low = score % 10000;
    position.x = dartsScoreOrigin.x;
    position.y = dartsScoreRows[row];
    position.z = dartsScoreOrigin.z;
    FTBL.SetPosRotForGame(board, 256, &dartsScoreStep, 9999, 0);
    FTBL.UpdateGameNodeFlags(board, high, &position, 1.0f, 0);
    position.x = dartsDigitStep.x * 4.0f + 0.1474f;
    position.y = dartsDigitRows[row];
    position.z = -0.04f;
    FTBL.scenGetCharaGameNode(board, 336, &position, 1.0f, &scale);
    position.x = dartsDigitOrigin.x;
    position.y = dartsDigitRows[row];
    position.z = dartsDigitOrigin.z;
    FTBL.SetPosRotForGame(board, 256, &dartsDigitStep, 9, 0);
    for (i = 0; i < 4; i++) {
        FTBL.UpdateGameNodeFlags(board, low % 10, &position, 1.0f, &scale);
        position.x += dartsDigitStep.x;
        position.y += dartsDigitStep.y;
        position.z += dartsDigitStep.z;
        low /= 10;
    }
}

void UpdateDartsCompactScoreDisplay(shen_name board, int score, int row)
{
    dartsCompactStep.x = dartsCompactSteps[row];
    dartsCompactStep.y = dartsCompactStep.z = 0.0f;
    FTBL.SetPosRotForGame(board, 256, &dartsCompactStep, 9999, 0);
    FTBL.UpdateGameNodeFlags(board, score, &dartsCompactOrigins[row], 1.0f, &dartsCompactScales[row]);
}

void QueueDartsPosition(shen_name parent, shen_name character, Vector3 *position, Vector3i *rotation, int delay)
{
    EventDartsPositionWork local;
    HLibTask *task;
    EventDartsPositionWork *work;
    if (delay) {
        task = FTBL.HLib_EnqueueTaskWithParameter(DartsPositionCallback, 4, 11, sizeof(EventDartsPositionWork), TASK_EVNT);
        work = (EventDartsPositionWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            return;
        memset(work, 0, sizeof(EventDartsPositionWork));
        work->cleanupTask = 1;
        if (rotation) {
            work->hasRotation = 1;
            work->rotation = *rotation;
        }
        if (position) {
            work->hasPosition = 1;
            work->position = *position;
        }
        work->parent = parent;
        work->character = character;
        FTBL.HLib_SetupTimedTask(task, delay);
    } else {
        memset(&local, 0, sizeof(EventDartsPositionWork));
        if (rotation) {
            local.hasRotation = 1;
            local.rotation = *rotation;
        }
        if (position) {
            local.hasPosition = 1;
            local.position = *position;
        }
        local.parent = parent;
        local.character = character;
        DartsPositionCallback(&local);
    }
}

void DartsPositionCallback(void *taskData)
{
    EventDartsPositionWork *work = (EventDartsPositionWork *)taskData;
    shen_name previous;
    if (!work)
        return;
    previous = FTBL.SCEN_FindChar_Wrapper(work->character);
    FTBL.AT_SetLocalBaseChara(work->character, work->parent);
    if (work->hasPosition)
        FTBL.EV_AT_SetPos(work->character, 0x38000000, &work->position);
    if (work->hasRotation)
        FTBL.EV_AT_SetAngY_0(work->character, 0x38000000, &work->rotation, 0);
    FTBL.AT_SetLocalBaseChara(work->character, previous);
#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(work->character), 16, 1);
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(work->character));
#endif
    if (work->cleanupTask)
        FTBL.HLib_DestroyCurrentTask();
}

#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
void EVT_SetDartsHelpState(int state)
{
    if (state <= 2)
        dartsHelpState = state;
}

void EVT_QueueDartsHelpIcons(EventDartsHelpIcon *icons)
{
    HLibTask *task;
    EventDartsHelpWork *work;
    dartsHelpState = 0;
    task = FTBL.HLib_EnqueueTaskWithParameter(DartsHelpIconCallback, 4, 11, sizeof(EventDartsHelpWork), TASK_EVNT);
    work = (EventDartsHelpWork *)FTBL.HLib_GetTaskParameter(task);
    work->icons = icons;
    work->task = task;
}

void DartsHelpIconCallback(void *taskData)
{
    EventDartsHelpWork *work = (EventDartsHelpWork *)taskData;
    int shown, i, facing;
    if (work) {
    switch (dartsHelpState) {
    case 0:
        shown = 0;
        for (i = 0; work->icons[i].icon != -1; i++) {
            facing = work->icons[i].character ? IsRyoFacingDartsCharacter(work->icons[i].character, work->icons[i].fov) : -1;
            if (facing > 0)
                continue;
            if (facing >= 0)
                continue;
            if (IsRyoFacingDartsCharacter(work->icons[i].alternate, work->icons[i].fov)) {
                shown = 1;
                FTBL.CheckHelpIcon(1, 0, 3);
                FTBL.EV_EventSetUpHelpIcon_0((unsigned char)work->icons[i].icon, 0);
            }
        }
        if (!shown)
            FTBL.EV_EventSetUpHelpIcon_0(0, 0);
        break;
    case 1:
        FTBL.CheckHelpIcon(1, 0, 3);
        break;
    case 2:
        break;
    }
    } else
        FTBL.HLib_DestroyCurrentTask();
}
#endif

void RestoreOpponentDartsScores(shen_name character, int save, int *bestScore, int *opponentScore)
{
    int i;
    for (i = 0; i < 5; i++)
        if (dartsSavedScores[i].character == character)
            break;
    if (character > 4)
        return;
    if (save) {
        FTBL.SetSavedMinigameScore(dartsSavedScores[i].bestScore, *bestScore);
        FTBL.SetSavedMinigameScore(dartsSavedScores[i].opponentScore, *opponentScore);
    } else {
        *bestScore = FTBL.GetSavedMinigameScore(dartsSavedScores[i].bestScore);
        *opponentScore = FTBL.GetSavedMinigameScore(dartsSavedScores[i].opponentScore);
    }
}

void PrepareDartsArrows(EventDartsWork *work)
{
    Vector3 position;
    signed char *pattern;
    shen_name arrow;
    int i, index;
    for (i = 0; i < 5; i++)
        if (dartsAiRecords[i].character == work->game.enemy->name)
            break;
    if (i > 4)
        return;
    pattern = dartsAiRecords[i].pattern;
    index = 0;
    while (*pattern != -1) {
        arrow = GetDartsArrowName(2, index);
        FTBL.EV_EnableCharacterDisplay(arrow);
        FTBL.EV_EnsureCharacterDraw(arrow);
        CalculateDartsPositionOrScore(*pattern++, &position);
        SetDartsPosition(work->game.placement->character, arrow, position.x, position.y, position.z, 0xc000, 0);
        index++;
    }
    for (; index < DARTS_ARROW_SLOTS; index++) {
        arrow = GetDartsArrowName(2, index);
        FTBL.EV_DisableCharacterDisplay(arrow);
        FTBL.EV_ToggleCharacterFlag(arrow);
    }
}

#if SHENMUE_ET_DARTS_REVISION >= ET_DARTS_REVISION_DART
static const char sectors[21] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, -1};
#endif

int CalculateDartsPositionOrScore(char score, Vector3 *position)
{
    int result = 0;
#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
    char sectors[21] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, -1};
#endif
    int ring, k, found, angleIndex;
    float x, y, distance, angle, step;
    if (score == -1) {
        x = position->x;
        y = (position->y - 1.4174f) * 1.00721359f;
        distance = x * x + y * y;
        DARTS_FSQRT(distance);
        if (distance <= 0.012f)
            result = 50;
        else if (distance <= 0.024f)
            result = 25;
        else {
            angle = (float)FTBL.InverseFixedAtan2(-x, -y) / 182.044449f + 9.0f;
            if (!(360.0f > angle))
                angle -= 360.0f;
            angle /= 18.0f;
            result = (int)angle;
            if (result > 19)
                result = 0;
            result = sectors[result];
            if (distance > 0.088f) {
                if (!(distance > 0.104f))
                    result *= 3;
                else if (distance > 0.1492f) {
                    result *= 2;
                    if (distance > 0.166f)
                        result = 0;
                }
            }
        }
#if SHENMUE_ET_DARTS_REVISION < ET_DARTS_REVISION_DART
        FTBL.DebugLogToFileRemoved("score %d\n", result);
#endif
        result *= 10000;
    } else {
        x = y = distance = 0.0f;
        if (score == 50) {
            distance = FTBL.GetRandomNumberInRange(0.012f);
            angle = FTBL.GetRandomNumberInRange(360.0f);
        } else if (score == 25) {
            distance = FTBL.GetRandomNumberInRange(y = 0.012f);
            angle = FTBL.GetRandomNumberInRange(360.0f);
            x = y;
        } else {
            found = 0;
            for (ring = 0; ring <= 2; ring++) {
                for (k = 0, step = -9.0f; sectors[k] != -1; k++, step += 18.0f) {
                    if (ring == 0) {
                        if (score == sectors[k]) {
                            found = 1;
                            break;
                        }
                    } else if (ring == 1) {
                        if (score == sectors[k] * 2) {
                            found = 1;
                            break;
                        }
                    } else if (ring == 2) {
                        if (score == sectors[k] * 3) {
                            found = 1;
                            break;
                        }
                    }
                }
                if (found)
                    break;
            }
            if (ring > 2)
                return 0;
            angle = step + FTBL.GetRandomNumberInRange(18.0f);
            switch (ring) {
            case 0:
                DARTS_RING_OFFSET(0.088f - 0.024f, 0.024f);
                break;
            case 1:
                DARTS_RING_OFFSET(0.166f - 0.1492f, 0.1492f);
                break;
            case 2:
                DARTS_RING_OFFSET(0.104f - 0.088f, 0.088f);
                break;
            }
        }
        x = (x + distance) * FTBL.CalculateFixedSine(angleIndex = (unsigned short)(int)(angle * 8192.0f / 45.0f));
        y = (y + distance) * FTBL.CalculateFixedCosine(angleIndex);
        position->x = x;
        position->y = y / 1.00721359f + 1.4174f;
        position->z = -0.068200007f;
    }
    return result;
}

shen_name GetDartsArrowName(int set, int index)
{
    if (set == 0) {
        index %= 5;
        return SHEN_NAME4('Y', 'A', '0', '0') + ((char)(index % 10) << 24 | (unsigned char)(index / 10) << 16);
    } else if (set != 1) {
        index %= 15;
        return SHEN_NAME4('Y', 'a', '0', '0') + ((char)(index % 10) << 24 | (unsigned char)(index / 10) << 16);
    } else {
        index %= 5;
        return SHEN_NAME4('Y', 'a', '0', '0') + ((char)(index % 10) << 24 | (unsigned char)(index / 10) << 16);
    }
}
#endif
