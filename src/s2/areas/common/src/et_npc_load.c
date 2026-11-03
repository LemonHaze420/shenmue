#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_npc_load.h"

extern void *memset(void *destination, int value, unsigned int size);

#define CONDITIONAL_NPC_LOAD_SLOTS 50
typedef struct ConditionalNpcLoadSlot {
    shen_name character;
    int entryIndex;
} ConditionalNpcLoadSlot;
typedef struct ConditionalNpcLoadWork {
    ConditionalNpcLoadEntry *entries;
    char phase, entryIndex;
    short reserved06;
    int admitted[CONDITIONAL_NPC_LOAD_SLOTS];
} ConditionalNpcLoadWork;

static int conditionalNpcLoadsReady;
static ConditionalNpcLoadSlot conditionalNpcLoadSlots[CONDITIONAL_NPC_LOAD_SLOTS];

void ConditionalNpcLoadCheckCallback(void *data);
void ConditionalNpcLoadCallback(void *data);
int FindConditionalNpcLoadSlot(shen_name name);
int IsConditionalNpcLoadPending(void);

void QueueConditionalNpcLoads(ConditionalNpcLoadEntry *entries)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(ConditionalNpcLoadCheckCallback, 4, 11, sizeof(ConditionalNpcLoadWork), TASK_EVNT);
    ConditionalNpcLoadWork *work = (ConditionalNpcLoadWork *)FTBL.HLib_GetTaskParameter(task);
    int i;
    if (work == 0) {
        FTBL.HLib_DestroyTaskInternal(task);
        FTBL.EventAssertNullsub(__FILE__, 72);
    } else {
        memset(work, 0, sizeof(ConditionalNpcLoadWork));
        work->entries = entries;
        conditionalNpcLoadsReady = 0;
        for (i = 0; i < CONDITIONAL_NPC_LOAD_SLOTS; i++) {
            conditionalNpcLoadSlots[i].character = 0;
            conditionalNpcLoadSlots[i].entryIndex = 0;
        }
        FTBL.HLib_SetupTimedTask(task, 5);
    }
}

void ConditionalNpcLoadCheckCallback(void *data)
{
    ConditionalNpcLoadWork *work = (ConditionalNpcLoadWork *)data;
    ConditionalNpcLoadEntry *entry;
    int index, switched = 0;
    if (!FTBL.ryoNotBusy())
        return;
    for (index = 0, entry = work->entries; entry->character != 0xffffffffU; index++, entry++) {
        if (FindConditionalNpcLoadSlot(entry->character) == -1 && work->admitted[index] == 0) {
            if ((entry->storyFlag <= 0 || FTBL.EV_GetShenmueFlag(entry->storyFlag)) && (entry->condition == 0 || entry->condition())) {
                work->entryIndex = 0;
                work->phase = 0;
                FTBL.EV_NPC_NewLoadSw_0(0);
                for (index = 0, entry = work->entries; entry->character != 0xffffffffU; index++, entry++) {
                    if (FTBL.IsNpcRouteLoadedOrVisible(entry->character)) {
                        FTBL.npcLockUnlock(entry->character);
                        FTBL.npcFree(entry->character);
                        work->admitted[index] = 0;
                    }
                }
                for (index = 0; index < CONDITIONAL_NPC_LOAD_SLOTS; index++) {
                    conditionalNpcLoadSlots[index].character = 0;
                    conditionalNpcLoadSlots[index].entryIndex = 0;
                }
                switched = 1;
                FTBL.HLib_SetCurrentTaskTo(ConditionalNpcLoadCallback);
                break;
            }
        } else if (entry->storyFlag != -1) {
            if (!FTBL.EV_GetShenmueFlag(entry->storyFlag) && FindConditionalNpcLoadSlot(entry->character) != -1) {
                if (conditionalNpcLoadSlots[FindConditionalNpcLoadSlot(entry->character)].entryIndex == index) {
                    conditionalNpcLoadSlots[FindConditionalNpcLoadSlot(entry->character)].character = 0;
                    FTBL.npcLockUnlock(entry->character);
                    FTBL.npcFree(entry->character);
                }
                work->admitted[index] = 0;
            }
            if (entry->condition && !entry->condition() && FindConditionalNpcLoadSlot(entry->character) != -1) {
                if (conditionalNpcLoadSlots[FindConditionalNpcLoadSlot(entry->character)].entryIndex == index) {
                    conditionalNpcLoadSlots[FindConditionalNpcLoadSlot(entry->character)].character = 0;
                    FTBL.npcLockUnlock(entry->character);
                    FTBL.npcFree(entry->character);
                }
                work->admitted[index] = 0;
            }
        }
    }
    if (!switched)
        conditionalNpcLoadsReady = 1;
}

void ConditionalNpcLoadCallback(void *data)
{
    ConditionalNpcLoadWork *work = (ConditionalNpcLoadWork *)data;
    ConditionalNpcLoadEntry *entry = work->entries;
    int slot, result;
    Vector3i rotation;
    entry += work->entryIndex;
    switch (work->phase) {
    case 0:
        if (FTBL.PrepareNpcAllFromArray())
            work->phase++;
        break;
    case 1:
        for (; entry->character != 0xffffffffU; entry++, work->entryIndex++) {
            if (entry->storyFlag > 0 && !FTBL.EV_GetShenmueFlag(entry->storyFlag))
                continue;
            if (!FTBL.DoesScenarioCharacterExist(entry->character) || FindConditionalNpcLoadSlot(entry->character) == -1) {
                if (entry->condition && (result = entry->condition()) == 0) {
                    work->admitted[work->entryIndex] = result;
                    continue;
                }
                work->phase++;
                FTBL.CheckWarmStartNpcRoute(entry->character);
                return;
            }
        }
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.HLib_SetCurrentTaskTo(ConditionalNpcLoadCheckCallback);
        break;
    case 2:
        if (FTBL.ForceLoadNpc(entry->character)) {
            for (slot = 0; conditionalNpcLoadSlots[slot].character; slot++)
                ;
            conditionalNpcLoadSlots[slot].character = entry->character;
            conditionalNpcLoadSlots[slot].entryIndex = work->entryIndex;
            FTBL.EV_EnableCharacterDisplay(entry->character);
            FTBL.EV_AT_SetPos(entry->character, 0x78000000, &entry->position);
            rotation.x = 0;
            rotation.y = entry->yaw;
            rotation.z = 0;
            FTBL.EV_AT_SetAngY_0(entry->character, 0x78000000, &rotation, 0);
            if (entry->motion != 0xffffffffU)
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(entry->character), entry->motion, 0);
            work->admitted[work->entryIndex] = 1;
            work->entryIndex++;
            work->phase = 1;
        }
        break;
    }
}

int FindConditionalNpcLoadSlot(shen_name name)
{
    int i;
    for (i = 0; i < CONDITIONAL_NPC_LOAD_SLOTS; i++)
        if (conditionalNpcLoadSlots[i].character == name)
            return i;
    return -1;
}

int IsConditionalNpcLoadPending(void)
{
    if (!conditionalNpcLoadsReady)
        return 1;
    return 0;
}

unsigned int AreConditionalNpcLoadsPending(void)
{
    return IsConditionalNpcLoadPending() ? 1 : 0;
}
#endif
