#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_readnpc.h"

static void AdmitEventReadCharacter(shen_name character);

EventNpcReadWork *QueueEventNpcRead(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventNpcReadCallback, 4, 11, sizeof(EventNpcReadWork), TASK_EVNT);
    EventNpcReadWork *work = (EventNpcReadWork *)FTBL.HLib_GetTaskParameter(task);
    shen_name *character;
    unsigned int *state, *request;
    int count;
    work->busy = 0;
    work->loadingCharacter = EVENT_NPC_READ_INVALID;
    state = work->states;
    request = work->requests;
    character = work->characters;
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, character++, state++, request++) {
        *character = EVENT_NPC_READ_INVALID;
        *state = 0;
        *request = 0;
    }
    return work;
}

void EventNpcReadCallback(void *data)
{
    EventNpcReadWork *work = (EventNpcReadWork *)data;
    unsigned int *state = work->states;
    unsigned int *request = work->requests;
    shen_name *character = work->characters;
    int result;
    int count;
    unsigned int current;
    if (work->busy) {
        if (work->loadingCharacter != EVENT_NPC_READ_INVALID) {
            result = FTBL.npcStreamOrLoadHuman(work->loadingCharacter);
            if (result == 1) {
                AdmitEventReadCharacter(work->loadingCharacter);
                work->busy = 0;
                work->loadingCharacter = EVENT_NPC_READ_INVALID;
            } else {
                if (result < 0)
                    FTBL.DebugLogToFileRemoved("-------- Error %d : %c%c%c%c --------\n", result, work->loadingCharacter & 255,
                        work->loadingCharacter >> 8 & 255, work->loadingCharacter >> 16 & 255, work->loadingCharacter >> 24);
                return;
            }
        } else
            work->busy = 0;
    }
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, character++, state++, request++) {
        if (*character == EVENT_NPC_READ_INVALID) {
            *state = 0;
            *request = 0;
        } else {
            switch (*request) {
            case 1:
                current = *state;
                if (current == 0) {
                    if (!work->busy) {
                        FTBL.CheckWarmStartNpcRoute(*character);
                        work->busy = 1;
                        work->loadingCharacter = *character;
                        *state = 1;
                        *request = 0;
                    }
                } else {
                    if (current <= 2)
                        *state = 3;
                    *request = 0;
                }
                break;
            case 2:
                if (*state)
                    *state = 2;
                else
                    *character = EVENT_NPC_READ_INVALID;
                *request = 0;
                break;
            case 3:
                if (*state) {
                    FTBL.npcFree(*character);
                    if (FTBL.IsNpcRouteLoadedOrVisible(*character))
                        FTBL.npcLockUnlock(*character);
                }
                *character = EVENT_NPC_READ_INVALID;
                *state = 0;
                *request = 0;
                break;
            default:
                if (*state == 1)
                    *state = 3;
                *request = 0;
                break;
            }
        }
    }
}

static void AdmitEventReadCharacter(shen_name character)
{
    do {
    FTBL.SetRefBoardParamsForCharaById(character, 1);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    } while (0);
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.SetScenarioCharacterDrawControl(character, 1, 1, 0.0f);
    FTBL.AT_SetLocalBaseChara(character, EVENT_NPC_READ_INVALID);
    FTBL.figpSetFlagsBlendForBlur(character, 2, 1);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(character), 1);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 3, 2);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), 7, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 66, 2, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 69, 2, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 71, 2, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(character), 1);
}

int IsEventNpcReadBusy(EventNpcReadWork *work)
{
    return work->busy;
}

void RequestEventNpcCharacters(EventNpcReadWork *work, const shen_name *characters)
{
    int cursor = 0;
    const shen_name *name;
    shen_name *character;
    unsigned int *state, *request;
    int count;
    work->busy = 1;
    state = work->states;
    request = work->requests;
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, state++, request++)
        if ((*state == 3 && *request != 3) || *request == 1)
            *request = 2;
    for (name = characters; *name != EVENT_NPC_READ_INVALID; name++) {
        character = work->characters;
        request = work->requests;
        for (count = EVENT_NPC_READ_COUNT; count > 0; count--, character++, request++) {
            if (*character == *name) {
                *request = 1;
                break;
            }
            if (work->characters[cursor] != EVENT_NPC_READ_INVALID && ++cursor >= EVENT_NPC_READ_COUNT) {
                cursor = 0;
                FTBL.DebugLogToFileRemoved("-------- Error : RTC_ID Over --------\n");
            }
        }
        if (count == 0) {
            work->characters[cursor] = *name;
            work->requests[cursor] = 1;
        }
    }
}

int AreEventNpcReadsFree(EventNpcReadWork *work)
{
    unsigned int *state = work->states;
    int count;
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, state++)
        if (*state)
            return 0;
    return 1;
}

void ReleaseEventNpcRequests(EventNpcReadWork *work)
{
    unsigned int *request = work->requests;
    int count;
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, request++)
        *request = 3;
}

void RetireEventNpcRequests(EventNpcReadWork *work)
{
    unsigned int *state = work->states;
    unsigned int *request = work->requests;
    int count;
    for (count = EVENT_NPC_READ_COUNT; count > 0; count--, state++, request++)
        if (*state == 2 || *request == 2)
            *request = 3;
}
#endif
