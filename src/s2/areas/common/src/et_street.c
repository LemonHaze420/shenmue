#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_street.h"

#define STREET_FIGHT_WEIGHTED(statement) do { do { do { statement; } while (0); } while (0); } while (0)
#define STREET_FIGHT_SQRT(x) ({ float _root = (x); _root = __builtin_sqrtf(_root); _root; })

static Vector3 streetFightChenPosition = STREET_FIGHT_CHEN_POSITION;
static int streetFightChenAngle = STREET_FIGHT_CHEN_ANGLE;
static int actionChoicePhase = 0;
static int moneyMessagePhase = 0;
static shen_name streetFightAb00Audience[88] = {SHEN_NAME4('S', 'D', '5', '_'), SHEN_NAME4('S', 'D', '6', '_'), SHEN_NAME4('S', 'D', '7', '_'),
    SHEN_NAME4('S', 'H', 'S', '_'), SHEN_NAME4('W', 'D', '2', '_'), SHEN_NAME4('O', 'D', '2', '_'), SHEN_NAME4('R', 'E', '1', '_'),
    SHEN_NAME4('V', 'C', 'N', '_'), SHEN_NAME4('C', 'H', 'N', '_'), SHEN_NAME4('B', 'K', 'Y', '_'), SHEN_NAME4('B', 'R', 'T', '_'),
    SHEN_NAME4('B', 'Z', 'H', '_'), SHEN_NAME4('C', 'H', 'K', '_'), SHEN_NAME4('C', 'J', 'U', '_'), SHEN_NAME4('C', 'O', 'H', '_'),
    SHEN_NAME4('C', 'R', 'Y', '_'), SHEN_NAME4('E', 'N', 'C', '_'), SHEN_NAME4('E', 'N', 'T', '_'), SHEN_NAME4('G', 'Y', 'E', '_'),
    SHEN_NAME4('H', '0', '2', 'Q'), SHEN_NAME4('H', '0', '3', 'Q'), SHEN_NAME4('H', '0', '4', 'Q'), SHEN_NAME4('H', '0', '5', 'Q'),
    SHEN_NAME4('H', '0', '7', 'Q'), SHEN_NAME4('H', '0', '8', 'Q'), SHEN_NAME4('H', '0', '9', 'Q'), SHEN_NAME4('H', '1', '0', 'Q'),
    SHEN_NAME4('H', '1', '1', 'Q'), SHEN_NAME4('H', '1', '2', 'Q'), SHEN_NAME4('H', '1', '3', 'Q'), SHEN_NAME4('H', '1', '4', 'Q'),
    SHEN_NAME4('H', '1', '5', 'Q'), SHEN_NAME4('H', '1', '6', 'Q'), SHEN_NAME4('H', '1', '7', 'Q'), SHEN_NAME4('H', '1', '8', 'Q'),
    SHEN_NAME4('H', '1', '9', 'Q'), SHEN_NAME4('H', '2', '0', 'Q'), SHEN_NAME4('H', 'N', 'N', '_'), SHEN_NAME4('H', 'U', 'R', '_'),
    SHEN_NAME4('I', 'N', 'C', '_'), SHEN_NAME4('K', 'E', 'I', '_'), SHEN_NAME4('K', 'I', 'K', '_'), SHEN_NAME4('K', 'I', 'N', '_'),
    SHEN_NAME4('K', 'K', 'Y', '_'), SHEN_NAME4('K', 'M', 'Y', '_'), SHEN_NAME4('K', 'S', 'N', '_'), SHEN_NAME4('K', 'U', 'K', '_'),
    SHEN_NAME4('L', 'C', 'N', '_'), SHEN_NAME4('J', 'U', 'K', '_'), SHEN_NAME4('0', '7', 'C', '_'), SHEN_NAME4('0', '8', 'G', '_'),
    SHEN_NAME4('M', '0', '1', 'Q'), SHEN_NAME4('M', '0', '2', 'Q'), SHEN_NAME4('M', '0', '3', 'Q'), SHEN_NAME4('M', '0', '4', 'Q'),
    SHEN_NAME4('M', '0', '5', 'Q'), SHEN_NAME4('M', '0', '6', 'Q'), SHEN_NAME4('M', '0', '7', 'Q'), SHEN_NAME4('M', '0', '8', 'Q'),
    SHEN_NAME4('M', '0', '9', 'Q'), SHEN_NAME4('P', '0', '1', 'Q'), SHEN_NAME4('P', '0', '2', 'Q'), SHEN_NAME4('P', '0', '3', 'Q'),
    SHEN_NAME4('P', '0', '4', 'Q'), SHEN_NAME4('P', '0', '5', 'Q'), SHEN_NAME4('P', '0', '6', 'Q'), SHEN_NAME4('P', '0', '7', 'Q'),
    SHEN_NAME4('P', '0', '8', 'Q'), SHEN_NAME4('P', '0', '9', 'Q'), SHEN_NAME4('P', '1', '0', 'Q'), SHEN_NAME4('Y', '0', '1', 'Q'),
    SHEN_NAME4('Y', '0', '3', 'Q'), SHEN_NAME4('Y', '0', '4', 'Q'), SHEN_NAME4('Y', '0', '5', 'Q'), SHEN_NAME4('Y', '0', '7', 'Q'),
    SHEN_NAME4('Y', '0', '8', 'Q'), SHEN_NAME4('Y', '0', '9', 'Q'), SHEN_NAME4('Y', '1', '0', 'Q'), SHEN_NAME4('A', '0', '4', 'Q'),
    SHEN_NAME4('A', '0', '5', 'Q'), SHEN_NAME4('A', '0', '6', 'Q'), SHEN_NAME4('M', 'A', 'K', '_'), SHEN_NAME4('K', 'E', 'N', '_'),
    SHEN_NAME4('O', 'N', 'I', '_'), SHEN_NAME4('S', 'A', 'M', '_'), SHEN_NAME4('L', 'L', 'Y', '_'), SHEN_NAME4('C', 'L', 'J', '_'), STREET_FIGHT_INVALID};
static shen_name streetFightQgbtAudience[21] = {SHEN_NAME4('E', 'E', 'N', '_'), SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('C', 'Y', 'C', '_'),
    SHEN_NAME4('C', 'H', 'N', '_'), SHEN_NAME4('A', '0', '1', 'Q'), SHEN_NAME4('H', '1', '4', 'Q'), SHEN_NAME4('H', 'U', 'R', '_'),
    SHEN_NAME4('M', '0', '1', 'Q'), SHEN_NAME4('M', '0', '2', 'Q'), SHEN_NAME4('M', '0', '3', 'Q'), SHEN_NAME4('M', '0', '5', 'Q'),
    SHEN_NAME4('M', '0', '7', 'Q'), SHEN_NAME4('M', '0', '9', 'Q'), SHEN_NAME4('P', '0', '2', 'Q'), SHEN_NAME4('P', '0', '3', 'Q'),
    SHEN_NAME4('P', '0', '4', 'Q'), SHEN_NAME4('P', '0', '5', 'Q'), SHEN_NAME4('P', '0', '8', 'Q'), SHEN_NAME4('S', 'Y', 'C', '_'),
    SHEN_NAME4('Y', '0', '6', 'Q'), STREET_FIGHT_INVALID};
static shen_name streetFightQsfbAudience[45] = {SHEN_NAME4('C', 'Y', 'C', '_'), SHEN_NAME4('C', 'H', 'N', '_'), SHEN_NAME4('A', '0', '4', 'Q'),
    SHEN_NAME4('A', '0', '5', 'Q'), SHEN_NAME4('H', '0', '1', 'Q'), SHEN_NAME4('H', '0', '2', 'Q'), SHEN_NAME4('H', '0', '3', 'Q'),
    SHEN_NAME4('H', '0', '4', 'Q'), SHEN_NAME4('H', '0', '5', 'Q'), SHEN_NAME4('H', '0', '6', 'Q'), SHEN_NAME4('H', '0', '7', 'Q'),
    SHEN_NAME4('H', '0', '8', 'Q'), SHEN_NAME4('H', '0', '9', 'Q'), SHEN_NAME4('H', '1', '0', 'Q'), SHEN_NAME4('H', '1', '1', 'Q'),
    SHEN_NAME4('H', '1', '2', 'Q'), SHEN_NAME4('H', '1', '3', 'Q'), SHEN_NAME4('H', '1', '5', 'Q'), SHEN_NAME4('H', '1', '7', 'Q'),
    SHEN_NAME4('H', '1', '9', 'Q'), SHEN_NAME4('H', '2', '0', 'Q'), SHEN_NAME4('M', '0', '1', 'Q'), SHEN_NAME4('M', '0', '3', 'Q'),
    SHEN_NAME4('M', '0', '4', 'Q'), SHEN_NAME4('M', '0', '5', 'Q'), SHEN_NAME4('M', '0', '6', 'Q'), SHEN_NAME4('M', '0', '7', 'Q'),
    SHEN_NAME4('M', '0', '8', 'Q'), SHEN_NAME4('M', '0', '9', 'Q'), SHEN_NAME4('P', '0', '1', 'Q'), SHEN_NAME4('P', '0', '2', 'Q'),
    SHEN_NAME4('P', '0', '3', 'Q'), SHEN_NAME4('P', '0', '4', 'Q'), SHEN_NAME4('P', '0', '5', 'Q'), SHEN_NAME4('Y', '0', '1', 'Q'),
    SHEN_NAME4('Y', '0', '3', 'Q'), SHEN_NAME4('Y', '0', '4', 'Q'), SHEN_NAME4('Y', '0', '7', 'Q'), SHEN_NAME4('Y', '0', '8', 'Q'),
    SHEN_NAME4('Y', '0', '9', 'Q'), SHEN_NAME4('Y', '1', '0', 'Q'), SHEN_NAME4('S', 'A', 'M', '_'), SHEN_NAME4('L', 'L', 'Y', '_'),
    SHEN_NAME4('C', 'L', 'J', '_'), STREET_FIGHT_INVALID};
static unsigned short streetFightBattleSounds[24] = {0x981, 0x982, 0x983, 0x984, 0x985, 0x986, 0x987, 0x988, 0x989, 0x98a, 0x98b, 0x98c, 0x98d, 0x98e,
    0x98f, 0x990, 0x991, 0x992, 0x993, 0x994, 0x995, 0x996, 0x997, 0x998};
static unsigned short streetFightArrivalSounds[7] = {0x974, 0x975, 0x976, 0x977, 0x978, 0x979, 0x97a};
static unsigned short streetFightOpeningSounds[4] = {0x97b, 0x97c, 0x97d, 0x97e};
static unsigned short streetFightDefeatSounds[4] = {0x999, 0x99a, 0x99b, 0x99c};
static unsigned short streetFightVictorySounds[5] = {0x99d, 0x99e, 0x99f, 0x9a0, 0x9a1};
StreetFightSkipWait streetFightSkipWaitWork;
int streetFightSkipped;

unsigned int ControlStreetFightActionChoice(int enabled)
{
    unsigned int choice = 0;
    if (!enabled) {
        actionChoicePhase = 0;
        return 0;
    }
    switch (actionChoicePhase) {
    case 0:
        if (FTBL.GetShouldRenderIcons(2))
            actionChoicePhase++;
        break;
    case 1:
        if (!FTBL.GetShouldRenderIcons(2))
            actionChoicePhase++;
        break;
    case 2:
        choice = FTBL.shenGetCtrlSpeakIconFlag();
        actionChoicePhase = 0;
        break;
    }
    return choice;
}

unsigned int PollStreetFightActionChoice(void)
{
    return ControlStreetFightActionChoice(1);
}

unsigned int ControlStreetFightMoneyMessage(int enabled)
{
    unsigned int done = 0;
    if (!enabled) {
        moneyMessagePhase = 0;
        return 0;
    }
    switch (moneyMessagePhase) {
    case 0:
        if (FTBL.GetShouldRenderIcons(4))
            moneyMessagePhase++;
        break;
    case 1:
        if (!FTBL.GetShouldRenderIcons(4))
            moneyMessagePhase++;
        break;
    case 2:
        done = 1;
        moneyMessagePhase = 0;
        break;
    }
    return done;
}

unsigned int PollStreetFightMoneyMessage(void)
{
    return ControlStreetFightMoneyMessage(1);
}

void SetStreetFightCharacterVisible(shen_name character, int visible)
{
    if (visible) {
        FTBL.EV_EnableCharacterDisplay(character);
        FTBL.EV_EnsureCharacterDraw(character);
    } else {
        FTBL.EV_DisableCharacterDisplay(character);
        FTBL.EV_ToggleCharacterFlag(character);
    }
}

void StreetFightLookCallback(void *data)
{
    StreetFightLookWork *p = (StreetFightLookWork *)data;
    if (p->frames || (p->flags & 1)) {
        if (p->flags & 0x11) {
            p->flags &= ~1;
            if (FTBL.DoesScenarioCharacterExist(p->character) && FTBL.DoesScenarioCharacterExist(p->target)) {
                Vector3 position;
                FTBL.GetCharaPosition(p->target, 5, &position, 0x40000000);
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(p->character), &position, 0);
                if (p->flags & 0x100)
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(p->character), 0, 3);
            }
        }
        if (p->frames > 0)
            p->frames--;
    } else {
        if (!(p->flags & 0x1000) && FTBL.DoesScenarioCharacterExist(p->character)) {
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(p->character), 0, 0);

            if (p->flags & 0x40000000)
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(p->character), 0, 3);
        }
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueStreetFightLook(shen_name character, shen_name target, int reserved08, int delay, int frames, unsigned short flags)
{
    Vector3 position;
    if (!delay) {
        FTBL.GetCharaPosition(target, 5, &position, 0x40000000);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(character), &position, 0);
        if (flags & 0x100)
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(character), 0, 3);
    }
    if (frames | delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightLookCallback, 6, 11, sizeof(StreetFightLookWork), TASK_EVNT);
        StreetFightLookWork *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (StreetFightLookWork *)FTBL.HLib_GetTaskParameter(task);
        p->character = character;
        p->target = target;
        p->reserved08 = reserved08;
        p->frames = frames;
        p->flags = flags | 1;
    }
}

void StreetFightNullsub(shen_name character, shen_name target, int mode, int delay, int frames)
{
}

void ReadStreetFightCharacterPosition(shen_name character, shen_name target, int mode)
{
    Vector3 position;
    FTBL.GetCharaPosition(target, 5, &position, 0x40000000);
}

void ClearStreetFightLook(shen_name character, int mode)
{
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(character), 0, 0);
}

void StreetFightMotionSpeedCallback(void *data)
{
    StreetFightMotionSpeed *p = (StreetFightMotionSpeed *)data;
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(p->character), p->speed, 0);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

int QueueStreetFightMotionSpeed(shen_name character, float speed, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightMotionSpeedCallback, 6, 11, sizeof(StreetFightMotionSpeed), TASK_EVNT);
        StreetFightMotionSpeed *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (StreetFightMotionSpeed *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->speed = speed;
    } else {
        StreetFightMotionSpeed request;
        request.cleanup = 0;
        request.character = character;
        request.speed = speed;
        StreetFightMotionSpeedCallback(&request);
    }
    return delay;
}

HLibTask *QueueStreetFightAudience(unsigned char count, unsigned int mode)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightAudienceCallback, 6, 11, sizeof(StreetFightAudienceWork), TASK_EVNT);
    StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
    p->task = task;
    p->state = 0;
    p->reserved1a = 0;
    p->mode = mode;
    p->actionFrames = 0;
    p->soundFrequency = 2000;
    p->soundVolume = 90;
    p->radius = 4.75f;
    p->radiusRange = 2.0f;
    p->corridorStart.x = p->corridorStart.y = p->corridorStart.z = 0.0f;
    p->corridorEnd.x = p->corridorEnd.y = p->corridorEnd.z = 0.0f;
    p->corridorLength = 0.0f;
    p->soundCentre.x = p->soundCentre.y = p->soundCentre.z = 0.0f;
    if (count > 29)
        count = 30;
    p->entityCount = count;
    p->loopDelay = 0;
    p->lastSound = 0;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'G', 'B', 'T'):
        p->characterNames = streetFightQgbtAudience;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'B'):
        p->characterNames = streetFightQsfbAudience;
        break;
    case SHEN_NAME4('A', 'B', '0', '0'):
        p->characterNames = streetFightAb00Audience + 1;
        break;
    case SHEN_NAME4('Q', '1', '0', '0'):
    case SHEN_NAME4('Q', 'S', 'F', 'A'):
    case SHEN_NAME4('Q', 'S', 'F', 'C'):
    case SHEN_NAME4('Q', 'S', 'F', 'D'):
        p->characterNames = streetFightAb00Audience;
        break;
    default:
        p->characterNames = streetFightAb00Audience;
        break;
    }
    ClearStreetFightAudienceFighters(task);
    return task;
}

void ClearStreetFightAudienceFighters(HLibTask *task)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        int i;
        for (i = 0; i < 4; i++)
            p->fighters[i] = 0;
        p->fighterCount = 0;
    }
}

void AddStreetFightAudienceFighter(HLibTask *task, shen_name fighter)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        p->fighters[p->fighterCount++] = fighter;
        if (p->fighterCount > 4) {
            FTBL.DebugLogToFileRemoved("*** ID buffer over.\n");
            FTBL.EventAssertNullsub(__FILE__, 698);
        }
    }
}

void SetStreetFightAudienceCorridor(HLibTask *task, const Vector3 *start, const Vector3 *end)
{
    if (task) {
        StreetFightAudienceWork *p;
        if (IsStreetFightAudienceArranged(task))
            FTBL.DebugLogToFileRemoved("Audiences are already arranged.\n");
        p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        p->corridorStart.x = start->x;
        p->corridorStart.z = start->z;
        p->corridorEnd.x = end->x;
        p->corridorEnd.z = end->z;
        p->corridorLength = FTBL.CalculatePlanarDistanceBetweenPoints(start, end);
    }
}

void SetStreetFightAudienceCentre(HLibTask *task, const Vector3 *centre)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        p->soundCentre.x = centre->x;
        p->soundCentre.y = centre->y;
        p->soundCentre.z = centre->z;
        p->state = 2;
    }
}

void SetStreetFightAudienceRadius(HLibTask *task, float radius, float range)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        p->radius = radius;
        p->radiusRange = range;
        p->state = 2;
    }
}

unsigned int SetStreetFightAudienceMode(HLibTask *task, unsigned int bits, int set)
{
    StreetFightAudienceWork *p;
    unsigned int previous;
    if (!task)
        return 0;
    p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
    if (!p) {
        FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
        return 0;
    }
    previous = p->mode & bits;
    if (set > 0)
        p->mode |= bits;
    else if (set == 0)
        p->mode &= ~bits;
    return previous;
}

void PlaceStreetFightCharacter(shen_name character, float degrees, const Vector3 *position)
{
    EVT_AT_SetPos(character, 0xffffffffU, position->x, position->y, position->z, 0);
    EVT_AT_SetAngY(character, 0xffffffffU, degrees, 0);
}

int IsStreetFightAudienceArranged(HLibTask *task)
{
    if (!task)
        return 1;
    if (((StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task))->state > 2)
        return 1;
    return 0;
}

void StartStreetFightAudienceOpening(HLibTask *task, unsigned short delay)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!p) {
            FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
            return;
        }
        p->state = 4;
        if (!delay)
            delay = 1;
        p->loopDelay = delay;
    }
}

void ResumeStreetFightAudienceOpening(HLibTask *task)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!p) {
            FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
            return;
        }
        p->state = 4;
        p->loopDelay = 0;
    }
}

void StartStreetFightAudienceDefeat(HLibTask *task, unsigned short delay)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!p) {
            FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
            return;
        }
        p->state = 6;
        if (!delay)
            delay = 1;
        p->loopDelay = delay + 1;
    }
}

void StartStreetFightAudienceVictory(HLibTask *task, unsigned short delay)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!p) {
            FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
            return;
        }
        p->state = 7;
        if (!delay)
            delay = 1;
        p->loopDelay = delay;
    }
}

void ResumeStreetFightAudienceVictory(HLibTask *task)
{
    if (task) {
        StreetFightAudienceWork *p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!p) {
            FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
            return;
        }
        p->state = 7;
        p->loopDelay = 0;
    }
}

unsigned int ChangeStreetFightAudienceMode(HLibTask *task, unsigned int bits, int set)
{
    StreetFightAudienceWork *p;
    unsigned int previous;
    if (!task)
        return 0;
    p = (StreetFightAudienceWork *)FTBL.HLib_GetTaskParameter(task);
    if (!p) {
        FTBL.DebugLogToFileRemoved("Audience Task is not exist.\n");
        return 0;
    }
    previous = p->mode & bits;
    if (set > 0)
        p->mode |= bits;
    else if (set < 0)
        p->mode &= ~bits;
    return previous;
}

void StreetFightAudienceCallback(void *data)
{
    StreetFightAudienceWork *p = (StreetFightAudienceWork *)data;
    int i;
    switch (p->state) {
    case 0:
    case 1:
        p->state = 2;
    case 2:
        for (i = 0; i < p->entityCount; i++)
            FindStreetFightAudienceSpace(p, i);
        p->state = 3;
        break;
    case 3:
        PlayStreetFightAudienceArrivalSounds(p);
        if (FTBL.IsFightActive())
            p->state = 5;
        break;
    case 4:
        PlayStreetFightAudienceOpeningSounds(p);
        if (FTBL.IsFightActive())
            p->state = 5;
        break;
    case 5:
        if (FTBL.IsFightActive()) {
            PlayStreetFightAudienceBattleSounds(p);
            if (!FTBL.EV_vfBattleUtil(3, 2, -1)) {
                ControlStreetFightAudienceVisibility(p);
                ControlStreetFightAudiencePositions(p);
            }
        }
        break;
    case 6:
        PlayStreetFightAudienceDefeatSounds(p);
        if (FTBL.IsFightActive()) {
            ControlStreetFightAudienceVisibility(p);
            ControlStreetFightAudiencePositions(p);
        }
        break;
    case 7:
        PlayStreetFightAudienceVictorySounds(p);
        if (FTBL.IsFightActive()) {
            ControlStreetFightAudienceVisibility(p);
            ControlStreetFightAudiencePositions(p);
        }
        break;
    }
}

void PlayStreetFightAudienceBattleSounds(StreetFightAudienceWork *p)
{
    unsigned int bank, moveFlags, actorFlags, frequency, sound;
    unsigned short i;
    short volume, change;
    if (p->mode & 2)
        return;
    moveFlags = 0;
    actorFlags = 0;
    for (i = 0; i < p->fighterCount; i++) {
        actorFlags |= FTBL.EV_vfBattleUtil(2, p->fighters[i], 0);
        moveFlags |= FTBL.EV_vfBattleUtil(2, p->fighters[i], 1);
    }
    if (actorFlags == 0xffffffffU || moveFlags == 0xffffffffU)
        return;
    if ((actorFlags & 0x140000) == 0x40000)
        frequency = 400;
    else if (actorFlags & 0x10000000)
        frequency = 500;
    else if (moveFlags & 0x4000008)
        frequency = 800;
    else if (p->actionFrames > 100) {
        p->actionFrames = 100;
        frequency = 2000;
    } else
        frequency = 1500;
    if (actorFlags & 0x100)
        p->actionFrames = 0;
    else
        p->actionFrames++;
    volume = 90;
    if (!FTBL.cvsHasValidConverseData(0)) {
        float rate = -0.0125f;
        volume = 110.0f - 400.0f * rate + p->soundFrequency * rate;
    }
    p->soundFrequency += (short)((frequency - p->soundFrequency) * 0.5f);
    change = volume - p->soundVolume;
    if ((change < 0 ? -change : change) > 1) {
        if (change < 0)
            change = -1;
        else
            change = 1;
    }
    p->soundVolume += change;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        bank = 13;
        break;
    default:
        bank = 0;
        break;
    }
    for (i = 0; i < p->entityCount; i++) {
        if (p->soundFrequency > 0 && FTBL.shenGetRandomInt32(p->soundFrequency) < 10) {
            do {
                sound = streetFightBattleSounds[FTBL.shenGetRandomInt32(24)];
            } while (sound == p->lastSound);
            p->lastSound = sound;
            p->materialBank = FTBL.FindMaterialSoundBank(sound);
            FTBL.Shenmue_Execute3DSoundCommand(p->soundVolume << 24 | bank << 19 | sound, &p->soundPositions[i], 2.0f, 30.0f);
        }
    }
}

void ControlStreetFightAudienceVisibility(StreetFightAudienceWork *p)
{
    Vector3 target, next, position;
    short rotation[4];
    float cameraDistance, distance, projection, alpha, step, current, change;
    int cameraAngle, characterAngle;
    const shen_name *names;
    shen_name name;
    FTBL.camCtrlGetNextCamPos(&next);
    FTBL.camCtrlGetTargetPosition(&target);
    FTBL.camCtrlGetRot(rotation);
    rotation[0] += 4096;
    cameraDistance = FTBL.CalculatePlanarDistanceBetweenPoints(&next, &target);
    for (names = p->characterNames; (name = *names) != STREET_FIGHT_INVALID; names++) {
        if (!FTBL.DoesScenarioCharacterExist(name))
            continue;
        FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
        cameraAngle = FTBL.InverseFixedAtan2(target.x - next.x, target.z - next.z);
        characterAngle = FTBL.InverseFixedAtan2(position.x - next.x, position.z - next.z);
        distance = FTBL.CalculatePlanarDistanceBetweenPoints(&next, &position);
        projection = FTBL.CalculateFixedCosine(cameraAngle - characterAngle) * distance;
        if (p->mode & 1) {
            if (cameraDistance > projection)
                SetStreetFightCharacterVisible(name, 0);
            else
                SetStreetFightCharacterVisible(name, 1);
        } else {
            if (cameraDistance > projection) {
                alpha = 1.0f + rotation[0] * -0.0001953125f;
                if (alpha > 1.0f)
                    alpha = 1.0f;
                else if (alpha < 0.2f)
                    alpha = 0.2f;
                step = 0.1f;
            } else {
                alpha = 1.0f;
                step = 0.05f;
            }
            current = FTBL.GetCharacterAlpha(FTBL.ctrlGetCharaByName(name));
            change = alpha - current;
            if (__builtin_fabs(change) > step) {
                if (change < 0.0f)
                    change = -step;
                else
                    change = step;
            }
            current += change;
            if (current > 1.0f)
                current = 1.0f;
            else if (current < 0.0f)
                current = 0.0f;
            FTBL.SetScenarioCharacterTransparency(name, current);
        }
    }
}

void ControlStreetFightAudiencePositions(StreetFightAudienceWork *p)
{
    Vector3 first, second, position, direction;
    float firstDistance, distance, push;
    const shen_name *names;
    shen_name name;
    if (FTBL.vfmDispatchControl(22, 0, 0, 0) != 8)
        return;
    FTBL.GetCharaPosition(p->fighters[0], -1, &first, 0x40000000);
    FTBL.GetCharaPosition(p->fighters[1], -1, &second, 0x40000000);
    for (names = p->characterNames; (name = *names) != STREET_FIGHT_INVALID; names++) {
        if (!FTBL.DoesScenarioCharacterExist(name))
            continue;
        FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
        firstDistance = FTBL.GetSquaredDistanceXZ(&position, &first);
        distance = FTBL.GetSquaredDistanceXZ(&position, &second);
        if (distance > firstDistance) {
            distance = firstDistance;
            direction.x = first.x;
            direction.y = first.y;
        } else {
            direction.x = second.x;
            direction.z = second.z;
        }
        if (distance < 1.47f) {
            push = 1.2f - STREET_FIGHT_SQRT(distance);
            direction.x = position.x - direction.x;
            direction.y = 0.0f;
            direction.z = position.z - direction.z;
            FTBL.Vector3_NormalizeInPlace(&direction);
            position.x += direction.x * push;
            position.z += direction.z * push;
            FTBL.EV_AT_SetPos(name, 0x78000000, &position);
        }
    }
}

void FindStreetFightAudienceSpace(StreetFightAudienceWork *p, int entity)
{
    Vector3 position;
    int attempts, overlaps, angle, i;
    float radius;
    attempts = 0;
    do {
        if (attempts && attempts % 100 == 0)
            FTBL.DebugLogToFileRemoved("*** Warning. No.%d is replaced many times.\n", entity);
        STREET_FIGHT_WEIGHTED(overlaps = 0);
        angle = FTBL.shenGetRandomInt32(0xffff);
        radius = FTBL.GetRandomNumberInRange(p->radiusRange) + p->radius;
        position.x = FTBL.CalculateFixedSine(-angle) * radius + p->soundCentre.x;
        position.y = p->soundCentre.y;
        position.z = p->soundCentre.z - FTBL.CalculateFixedCosine(-angle) * radius;
        if (attempts > 3000)
            break;
        if (entity) {
            for (i = 0; i < entity; i++) {
                if (FTBL.GetSquaredDistanceXZ(&position, &p->soundPositions[i]) < 1.0f) {
                    overlaps = 1;
                    attempts++;
                    break;
                }
            }
        }
        if (IsStreetFightAudienceCorridorBlocked(p, &position)) {
            attempts++;
            STREET_FIGHT_WEIGHTED(overlaps = 1);
        }
    } while (overlaps);
    p->soundPositions[entity].x = position.x;
    p->soundPositions[entity].y = position.y;
    p->soundPositions[entity].z = position.z;
}

int IsStreetFightAudienceCorridorBlocked(StreetFightAudienceWork *p, const Vector3 *point)
{
    Vector3 position, step;
    unsigned char segments, i;
    int blocked;
    if (__builtin_fabs(p->corridorLength) < 0.00000011920929f)
        return 0;
    position.x = p->corridorStart.x;
    position.z = p->corridorStart.z;
    blocked = 0;
    segments = p->corridorLength / 1.5f + 0.5f;
    step.x = (p->corridorEnd.x - p->corridorStart.x) / segments;
    step.z = (p->corridorEnd.z - p->corridorStart.z) / segments;
    for (i = 0; i < segments; i++) {
        if (FTBL.GetSquaredDistanceXZ(&position, point) < 2.25f) {
            blocked = 1;
            break;
        }
        position.x += step.x;
        position.z += step.z;
    }
    return blocked;
}

void PlayStreetFightAudienceArrivalSounds(StreetFightAudienceWork *p)
{
    unsigned int bank, sound;
    unsigned char i;
    if (p->mode & 2)
        return;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        bank = 13;
        break;
    default:
        bank = 0;
        break;
    }
    for (i = 0; i < p->entityCount; i++) {
        if (FTBL.shenGetRandomInt32(4000) < 10) {
            do {
                sound = streetFightArrivalSounds[FTBL.shenGetRandomInt32(7)];
            } while (sound == p->lastSound);
            p->lastSound = sound;
            p->materialBank = FTBL.FindMaterialSoundBank(sound);
            FTBL.Shenmue_Execute3DSoundCommand(bank << 19 | 0x6e000000 | sound, &p->soundPositions[i], 2.0f, 30.0f);
        }
    }
}

void PlayStreetFightAudienceOpeningSounds(StreetFightAudienceWork *p)
{
    unsigned int bank, sound;
    unsigned char i;
    if (p->mode & 2)
        return;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        bank = 13;
        break;
    default:
        bank = 0;
        break;
    }
    if (p->loopDelay) {
        if (--p->loopDelay == 0) {
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f00097f);
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f000980);
        }
        return;
    }
    for (i = 0; i < p->entityCount; i++) {
        if (FTBL.shenGetRandomInt32(8000) < 10) {
            do {
                sound = streetFightOpeningSounds[FTBL.shenGetRandomInt32(4)];
            } while (sound == p->lastSound);
            p->lastSound = sound;
            p->materialBank = FTBL.FindMaterialSoundBank(sound);
            FTBL.Shenmue_Execute3DSoundCommand(bank << 19 | 0x6e000000 | sound, &p->soundPositions[i], 2.0f, 30.0f);
        }
    }
}

void PlayStreetFightAudienceDefeatSounds(StreetFightAudienceWork *p)
{
    unsigned int bank, sound;
    unsigned char i;
    if (p->mode & 2)
        return;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        bank = 13;
        break;
    default:
        bank = 0;
        break;
    }
    if (p->loopDelay) {
        if (--p->loopDelay == 0) {
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f00097f);
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f000980);
        }
        return;
    }
    for (i = 0; i < p->entityCount; i++) {
        if (FTBL.shenGetRandomInt32(8000) < 10) {
            do {
                sound = streetFightDefeatSounds[FTBL.shenGetRandomInt32(4)];
            } while (sound == p->lastSound);
            p->lastSound = sound;
            p->materialBank = FTBL.FindMaterialSoundBank(sound);
            FTBL.Shenmue_Execute3DSoundCommand(bank << 19 | 0x6e000000 | sound, &p->soundPositions[i], 2.0f, 30.0f);
        }
    }
}

void PlayStreetFightAudienceVictorySounds(StreetFightAudienceWork *p)
{
    unsigned int bank, sound;
    unsigned char i;
    if (p->mode & 2)
        return;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        bank = 13;
        break;
    default:
        bank = 0;
        break;
    }
    if (p->loopDelay) {
        if (--p->loopDelay == 0) {
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f00097f);
            FTBL.Loop3DSoundEffect(bank << 19 | 0x7f000980);
        }
        return;
    }
    for (i = 0; i < p->entityCount; i++) {
        if (FTBL.shenGetRandomInt32(4000) < 10) {
            do {
                sound = streetFightVictorySounds[FTBL.shenGetRandomInt32(5)];
            } while (sound == p->lastSound);
            p->lastSound = sound;
            p->materialBank = FTBL.FindMaterialSoundBank(sound);
            FTBL.Shenmue_Execute3DSoundCommand(bank << 19 | 0x6e000000 | sound, &p->soundPositions[i], 2.0f, 30.0f);
        }
    }
}

void ReleaseStreetFightAudience(HLibTask *task)
{
    if (task) {
        FTBL.HLib_GetTaskParameter(task);
        FTBL.HLib_DestroyTaskInternal(task);
    }
}

void QueueStreetFightPlayerReaction(shen_name fighter, int playerWon)
{
    if (!playerWon)
        EVT_AT_SetCam(500, SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'Y', 'O', '_'), 0, 0, 5);
    else
        EVT_AT_SetCam(501, SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'Y', 'O', '_'), 0, 0, 5);
    if (!playerWon)
        EVT_AT_SetCam(510, SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'Y', 'O', '_'), 0, 0, 56);
    else
        EVT_AT_SetCam(511, SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'Y', 'O', '_'), 0, 0, 56);
    EVT_RepeatCall(StreetFightReactionNullsub, 0, 55, 1);
    EVT_AT_SetFaceLooks(SHEN_NAME4('R', 'Y', 'O', '_'), 1, 8, 35.0f, 56);
    EVT_AT_SetFaceLooks(SHEN_NAME4('R', 'Y', 'O', '_'), 3, 0, 5.0f, 88);
    if (!playerWon)
        EVT_AT_SetCam(520, fighter, fighter, 0, 0, 96);
    else
        EVT_AT_SetCam(521, fighter, fighter, 0, 0, 96);
    EVT_RepeatCall(StreetFightReactionEndNullsub, 0, 136, 1);
    EVT_RepeatCall(StreetFightRestoreChaseCamera, 0, 136, 1);
}

void StreetFightRestoreChaseCamera(void *argument)
{
    FTBL.camCtrlSetState(10);
}

void StreetFightReactionNullsub(void *argument)
{
}

void StreetFightReactionEndNullsub(void *argument)
{
}

unsigned int CreateStreetFightDustEffect(void)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 volume = {0.0f, 0.0f, 0.0f};
    Vector3 velocity, variance;
    unsigned int effect;
    velocity.x = 0.0f;
    velocity.y = 0.01f;
    velocity.z = 0.0f;
    variance.x = 0.0f;
    variance.y = 0.0f;
    variance.z = 0.0f;
    effect = FTBL.CreateSmokeEffect(&position, &volume, 100, 0.001f, 0, 3);
    FTBL.SetSmokeEffectFadeFactor(effect, 10);
    FTBL.SetSmokeEffectFalloff(effect, 255);
    FTBL.SetParticleEffectColour(0, effect, 0, 142, 125, 110);
    FTBL.SetParticleEffectInitialScale(0, effect, 0.435294122f);
    FTBL.SetParticleEffectSize(0, effect, 3.13725495f, 3.17647052f);
    FTBL.SetParticleEffectBirthRate(0, effect, 0.0f);
    FTBL.SetParticleEffectVelocity(0, effect, &velocity, &variance);
    FTBL.SetSmokeEffectSpread(effect, 55.864f, 1.05882359f);
    FTBL.SetSmokeEffectRadialMotion(effect, 0.772549033f, 0.466666669f);
    FTBL.SetParticleEffectBirthRate(0, effect, 0.1f);
    return effect;
}

void StartStreetFightDust(StreetFightEventWork *work)
{
    if (work->dustEffect == STREET_FIGHT_INVALID)
        work->dustEffect = CreateStreetFightDustEffect();
}

void StopStreetFightDust(StreetFightEventWork *work)
{
    if (work->dustEffect != STREET_FIGHT_INVALID) {
        FTBL.SuspendParticleEffect(0, work->dustEffect);
        FTBL.FreeParticleEffect(0, work->dustEffect);
        work->dustEffect = STREET_FIGHT_INVALID;
    }
}

void QueueStreetFightSkipWait(StreetFightSkipFunction function, void *argument, int wait)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightSkipWaitCallback, 5, 11, sizeof(StreetFightSkipWait), TASK_SUEV);
    StreetFightSkipWait *p = (StreetFightSkipWait *)FTBL.HLib_GetTaskParameter(task);
    p->state = 0;
    p->function = function;
    p->argument = argument;
    p->skipped = 0;
    streetFightSkipped = 0;
    if (wait)
        FTBL.HLib_SetupTaskDelay(0);
}

void StreetFightSkipWaitCallback(void *data)
{
    StreetFightSkipWait *p = (StreetFightSkipWait *)data;
    int frames;
    switch (p->state) {
    case 0:
        frames = p->function(p->argument);
        p->pressed = 0;
        p->frames = frames;
        p->state++;
    case 1:
        if (p->frames)
            p->frames--;
        if (!p->frames) {
            p->state++;
            break;
        }
        if (FTBL.GetFightModeFlags() & 16) {
            if (!p->pressed) {
                if (FTBL.GetCurrentControlInput(1) >> 9 & 1) {
                    p->window = 20;
                    p->pressed++;
                }
            } else if (FTBL.GetCurrentControlInput(1) >> 9 & 1) {
                p->state++;
                p->skipped = 1;
            } else if (!--p->window)
                p->pressed = 0;
        }
        break;
    case 2:
        if (p->skipped)
            streetFightSkipped = 1;
        FTBL.HLib_ResolveDeferredCallback(FTBL.HLib_GetCurrentParentTask());
        FTBL.HLib_MarkCurrentTaskForCleanup();
        break;
    }
}

int IsStreetFightSkipped(void)
{
    return streetFightSkipped;
}

int CanPerformForAreaFlags(shen_name area)
{
    int type = 0, result = 0;
    if (IsStreetFightBestOfThree())
        return 0;
    switch (area) {
    case SHEN_NAME4('Q', 'T', 'B', '1'):
        if (FTBL.EV_GetShenmueFlag(601))
            result = 1;
        break;
    case SHEN_NAME4('Q', '1', '0', '0'):
        if (FTBL.EV_GetShenmueFlag(602))
            result = 1;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'A'):
        if (FTBL.EV_GetShenmueFlag(603))
            result = 1;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'B'):
        if (FTBL.EV_GetShenmueFlag(604))
            result = 1;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'C'):
        if (FTBL.EV_GetShenmueFlag(605))
            result = 1;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'D'):
        if (FTBL.EV_GetShenmueFlag(606))
            result = 1;
        break;
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
        if (FTBL.EV_GetShenmueFlag(295))
            result = 2;
        break;
    case SHEN_NAME4('Q', 'G', 'B', 'T'):
        if (FTBL.EV_GetShenmueFlag(294))
            result = 2;
        break;
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        if (FTBL.EV_GetShenmueFlag(296))
            result = 2;
        break;
    default:
        result = 0;
        break;
    }
    switch (result) {
    case 1:
        type = 1;
        break;
    case 2:
        type = 2;
        break;
    }
    return type;
}

void ResetStreetFightStoryFlag(int stage)
{
    int flag;
    if (IsStreetFightBestOfThree() || !CanPerformForAreaFlags(g_currentMap))
        return;
    switch (stage) {
    case 11:
        flag = 601;
        break;
    case 2:
        flag = 602;
        break;
    case 5:
        flag = 603;
        break;
    case 7:
        flag = 604;
        break;
    case 3:
        flag = 605;
        break;
    case 10:
        flag = 606;
        break;
    default:
        flag = 0;
        break;
    }
    if (flag)
        FTBL.EV_stepSetFlags(flag, 2);
}

int CommitStreetFightStoryRank(int stage)
{
    int result = 0, flag, progress, count;
    if (IsStreetFightBestOfThree())
        return 0;
    switch (stage) {
    case 11:
        flag = 601;
        progress = 2;
        break;
    case 2:
        flag = 602;
        progress = 1;
        break;
    case 5:
        flag = 603;
        progress = 6;
        break;
    case 7:
        flag = 604;
        progress = 4;
        break;
    case 3:
        flag = 605;
        progress = 5;
        break;
    case 10:
        flag = 606;
        progress = 3;
        break;
    case 1:
        flag = 295;
        progress = 0;
        break;
    case 12:
        flag = 294;
        progress = 0;
        break;
    case 8:
        flag = 296;
        progress = 0;
        break;
    default:
        flag = 0;
        progress = 0;
        break;
    }
    switch (CanPerformForAreaFlags(g_currentMap)) {
    case 1:
        FTBL.AddStreetFightProgress(progress);
        if (FTBL.GetStreetFightProgress() <= 9) {
            FTBL.EV_stepSetFlags(flag, 1);
            result = 1;
        } else {
            FTBL.EV_stepSetFlags(flag, 0);
            result = 2;
        }
        break;
    case 2:
        count = 0;
        if (FTBL.EV_GetPlayerFlag(11, 530))
            count++;
        if (FTBL.EV_GetPlayerFlag(11, 529))
            count++;
        if (FTBL.EV_GetPlayerFlag(11, 531))
            count++;
        if (count <= 1) {
            FTBL.EV_stepSetFlags(flag, 1);
            result = 0;
        } else {
            FTBL.EV_stepSetFlags(flag, 0);
            result = 2;
        }
        break;
    }
    return result;
}

int PlayStreetFightVictoryAppearance(void)
{
    SetStreetFightCharacterVisible(SHEN_NAME4('C', 'H', 'N', '_'), 1);
    FTBL.EV_SetCharacterAlphaEnabled(SHEN_NAME4('C', 'H', 'N', '_'), 0);
    EVT_AT_SetPos(SHEN_NAME4('C', 'H', 'N', '_'), 0xffffffffU, streetFightChenPosition.x, streetFightChenPosition.y, streetFightChenPosition.z, 0);
    EVT_AT_SetAngY(SHEN_NAME4('C', 'H', 'N', '_'), 0xffffffffU, streetFightChenAngle, 0);
    EVT_AT_SetMot(SHEN_NAME4('C', 'H', 'N', '_'), 9666, 0.0f, 1.0f, 0x40000000, 0);
    FTBL.EV_ControlPackageResources(2, 4, STREET_FIGHT_PACKAGE, 0, 0);
    EVT_AT_SetCam(9901, SHEN_NAME4('C', 'H', 'N', '_'), SHEN_NAME4('C', 'H', 'N', '_'), 0, 0, 0);
    return 60;
}

int PlayStreetFightKakuInvitation(void)
{
    FTBL.EV_ControlPackageResources(2, 4, STREET_FIGHT_PACKAGE, 0, 0);
    EVT_AT_SetCam(9902, SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('R', 'Y', 'O', '_'), 0, 0, 0);
    QueueEventMessage("カク「陽が落ちてから龍影街に来な…", 0, 90);
    QueueEventMessage("涼「龍影街…」", 90, 60);
    return 150;
}

void PlaceStreetFightCharacterForArea(shen_name area, shen_name character)
{
    Vector3 position;
    Vector3i rotation;
    position.x = position.z = 0.0f;
    position.y = 10000.0f;
    rotation.x = rotation.y = rotation.z = 0;
    switch (area) {
    case SHEN_NAME4('Q', '1', '0', '0'):
        position.x = 199.1f;
        position.y = 100.0f;
        position.z = 135.3f;
        rotation.y = 12;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'A'):
        position.x = 176.2f;
        position.y = 100.0f;
        position.z = 239.3f;
        rotation.y = 22;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'B'):
        position.x = 333.5f;
        position.y = 84.25f;
        position.z = 77.4f;
        rotation.y = 220;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'C'):
        position.x = 270.6f;
        position.y = 94.81f;
        position.z = 35.5f;
        rotation.y = 165;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'D'):
        position.x = 136.2f;
        position.y = 100.0f;
        position.z = 106.2f;
        rotation.y = 200;
        break;
    case SHEN_NAME4('Q', 'T', 'B', '1'):
        position.x = 207.7f;
        position.y = 100.0f;
        position.z = 247.1f;
        rotation.y = 75;
        break;
    }
    EVT_AT_SetPos(character, 0xffffffffU, position.x, position.y, position.z, 0);
    EVT_AT_SetAngY(character, 0xffffffffU, rotation.y, 0);
}

int IsStreetFightBestOfThree(void)
{
    if (FTBL.EV_GetPlayerFlag(11, 1014))
        return 1;
    return 0;
}

int GetStreetFightStageOrder(int stage)
{
    int order = 0;
    switch (stage) {
    case 0:
        order = 11;
        break;
    case 2:
        order = 2;
        break;
    case 11:
        order = 1;
        break;
    case 5:
        order = 4;
        break;
    case 7:
        order = 5;
        break;
    case 3:
        order = 8;
        break;
    case 10:
        order = 3;
        break;
    case 12:
        order = 7;
        break;
    case 1:
        order = 6;
        break;
    case 4:
        order = 10;
        break;
    case 8:
        order = 9;
        break;
    }
    return order;
}

int GetStreetFightStageFlag(int stage)
{
    int flag = 0;
    switch (stage) {
    case 0:
        flag = 14;
        break;
    case 2:
        flag = 17;
        break;
    case 11:
        flag = 16;
        break;
    case 5:
        flag = 15;
        break;
    case 7:
        flag = 20;
        break;
    case 3:
        flag = 19;
        break;
    case 10:
        flag = 18;
        break;
    case 12:
        flag = 21;
        break;
    case 1:
        flag = 22;
        break;
    case 4:
        flag = 24;
        break;
    case 8:
        flag = 23;
        break;
    }
    return flag;
}

void SetStreetFightStageFlag(int stage)
{
    FTBL.SetMGOTOSIDAMA(GetStreetFightStageFlag(stage), 1);
}

int StreetFightFalseNullsub(void)
{
    return 0;
}

void ResetStreetFightRounds(StreetFightEventWork *work)
{
    int i;
    work->round = 0;
    work->wins = work->losses = 0;
    for (i = 0; i < 3; i++)
        work->roundResults[i] = 0;
}

int RecordStreetFightRound(StreetFightEventWork *work)
{
    int i, result;
    if (work->battleStatus != 3)
        work->roundResults[work->round] = -1;
    else
        work->roundResults[work->round] = 1;
    work->wins = work->losses = 0;
    for (i = 0; i <= work->round; i++) {
        if (work->roundResults[i] > 0)
            work->wins++;
        else
            work->losses++;
    }
    if (work->wins > 1)
        result = 1;
    else if (work->losses > 1)
        result = 2;
    else {
        result = 0;
        work->round++;
    }
    return result;
}

int GetStreetFightStageForArea(shen_name area)
{
    int stage = 0;
    switch (area) {
    case SHEN_NAME4('A', 'B', '0', '0'):
        stage = 0;
        break;
    case SHEN_NAME4('Q', 'T', 'B', '1'):
        stage = 11;
        break;
    case SHEN_NAME4('Q', '1', '0', '0'):
        stage = 2;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'A'):
        stage = 5;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'B'):
        stage = 7;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'C'):
        stage = 3;
        break;
    case SHEN_NAME4('Q', 'S', 'F', 'D'):
        stage = 10;
        break;
    case SHEN_NAME4('Q', 'G', 'B', 'T'):
        stage = 12;
        break;
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
        stage = 1;
        break;
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
        stage = 4;
        break;
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        stage = 8;
        break;
    }
    return stage;
}

int GetStreetFightWinLimit(int stage)
{
    int limit = 0;
    switch (stage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
        limit = 15;
        break;
    }
    return limit;
}

int GetStreetFightLossLimit(int stage)
{
    int limit = 0;
    switch (stage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
        limit = 15;
        break;
    }
    return limit;
}

int RecordStreetFightVictory(int stage)
{
    int wins = FTBL.vfmDispatchControl(14, stage | 64, 0, -1);
    if (wins < GetStreetFightWinLimit(stage))
        wins++;
    FTBL.vfmDispatchControl(14, stage | 64, (const void *)wins, 1);
    FTBL.vfmDispatchControl(14, stage | 128, 0, 1);
    return FTBL.vfmDispatchControl(14, stage | 64, 0, -1);
}

int RecordStreetFightDefeat(int stage)
{
    int wins, losses;
    wins = FTBL.vfmDispatchControl(14, stage | 64, 0, -1);
    if (wins > 0)
        wins--;
    FTBL.vfmDispatchControl(14, stage | 64, (const void *)wins, 1);
    losses = FTBL.vfmDispatchControl(14, stage | 128, 0, -1);
    if (losses < GetStreetFightLossLimit(stage))
        losses++;
    FTBL.vfmDispatchControl(14, stage | 128, (const void *)losses, 1);
    return FTBL.vfmDispatchControl(14, stage | 64, 0, -1);
}

int GetHelpIconForArea(shen_name area)
{
    int icon = 20;
    if (!IsStreetFightBestOfThree()) {
        switch (area) {
        case SHEN_NAME4('A', 'B', '0', '0'):
            icon = 109;
            break;
        case SHEN_NAME4('Q', '1', '0', '0'):
            icon = 91;
            break;
        case SHEN_NAME4('Q', 'T', 'B', '1'):
            icon = 89;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'A'):
            icon = 95;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'B'):
            icon = 97;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'C'):
            icon = 100;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'D'):
            icon = 93;
            break;
        case SHEN_NAME4('Q', 'G', 'B', 'T'):
            icon = 102;
            break;
        case SHEN_NAME4('Q', 'J', 'B', 'T'):
            icon = 99;
            break;
        case SHEN_NAME4('Q', 'K', 'B', 'T'):
            icon = 106;
            break;
        case SHEN_NAME4('Q', 'L', 'B', 'T'):
            icon = 103;
            break;
        }
    } else {
        switch (area) {
        case SHEN_NAME4('A', 'B', '0', '0'):
            icon = 109;
            break;
        case SHEN_NAME4('Q', '1', '0', '0'):
            icon = 91;
            break;
        case SHEN_NAME4('Q', 'T', 'B', '1'):
            icon = 89;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'A'):
            icon = 95;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'B'):
            icon = 97;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'C'):
            icon = 100;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'D'):
            icon = 93;
            break;
        case SHEN_NAME4('Q', 'G', 'B', 'T'):
            icon = 150;
            break;
        case SHEN_NAME4('Q', 'J', 'B', 'T'):
            icon = 149;
            break;
        case SHEN_NAME4('Q', 'K', 'B', 'T'):
            icon = 152;
            break;
        case SHEN_NAME4('Q', 'L', 'B', 'T'):
            icon = 151;
            break;
        }
    }
    return icon;
}

void StreetFightNpcWaitBeginCallback(void *data)
{
    StreetFightNpcWaitWork *p = (StreetFightNpcWaitWork *)data;
    p->frames++;
    if (FTBL.NpcWait() || p->frames > 60) {
        p->frames = 0;
        FTBL.HLib_SetCurrentTaskTo(StreetFightNpcWaitEndCallback);
    }
}

void StreetFightNpcWaitEndCallback(void *data)
{
    StreetFightNpcWaitWork *p = (StreetFightNpcWaitWork *)data;
    p->frames++;
    if (!FTBL.NpcWait() || FTBL.CountNpcLiveStreams() > 4 || p->frames > 150) {
        FTBL.HLib_ResolveDeferredCallback(p->parent);
        FTBL.HLib_DestroyCurrentTask();
    }
}

int QueueStreetFightNpcWait(unsigned int flags)
{
    int result = 0;
    if (flags & 16) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightNpcWaitBeginCallback, 5, 11, sizeof(StreetFightNpcWaitWork), TASK_EVNT);
        StreetFightNpcWaitWork *p = (StreetFightNpcWaitWork *)FTBL.HLib_GetTaskParameter(task);
        p->self = task;
        p->frames = 0;
        p->parent = FTBL.HLib_GetCurrentTask();
        result = 1;
        FTBL.HLib_SetupTaskDelay(0);
    }
    return result;
}

void QueueStreetFightHighModel(shen_name character, int selector, int state, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 11, sizeof(StreetFightHighModelWork), TASK_EVNT);
        StreetFightHighModelWork *p = (StreetFightHighModelWork *)FTBL.HLib_GetTaskParameter(task);
        p->task = task;
        p->selector = selector;
        p->character = character;
        p->state = state;
        FTBL.HLib_SetupTimedTask(task, delay);
    } else {
        StreetFightHighModelWork request;
        request.task = 0;
        request.selector = selector;
        request.character = character;
        request.state = state;
        StreetFightHighModelCallback(&request);
    }
}

void StreetFightHighModelCallback(void *data)
{
    StreetFightHighModelWork *p = (StreetFightHighModelWork *)data;
    switch (p->selector) {
    case 0:
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(p->character), p->state, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(p->character), p->state, 1, 0);
        break;
    case 1:
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(p->character), p->state);
        break;
    }
    if (p->task)
        FTBL.HLib_DestroyCurrentTask();
}
#endif
