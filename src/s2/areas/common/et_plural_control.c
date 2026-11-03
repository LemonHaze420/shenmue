#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_plural_control.h"

void Queue_ET_PluralControl(const PluralControlEntry *entries)
{
    HLibTask *task;
    PluralControlWork *work;
    int i;
    task = FTBL.HLib_EnqueueTaskWithParameter(ET_PluralControl_Callback, 5, 11, sizeof(PluralControlWork), TASK_EVNT);
    work = (PluralControlWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->entries = entries;
    work->state = 0;
    work->count = 0;
    for (i = 0; i < 8; i++) {
        work->characters[i].character = 0;
        work->characters[i].previous = 1;
    }
}

void ET_PluralControl_Callback(void *data)
{
    PluralControlWork *work = (PluralControlWork *)data;
    const PluralControlEntry *entry;
    short frame = FTBL.EV_authSceneCtrl(-6);
    shen_name character;
    int i;
    switch (work->state) {
    case 0:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 1;
        break;
    case 1:
        entry = work->entries;
        while (entry->frame == frame) {
            if (FTBL.DoesScenarioCharacterExist(entry->character)) {
                ET_PluralControl_AddCharacter(work, entry->character);
                FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(entry->character), entry->state);
            }
            entry++;
        }
        work->entries = entry;
        if (entry->frame < 0 || !FTBL.EV_authSceneCtrl(-1))
            work->state = 2;
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            character = work->characters[i].character;
            if (character && FTBL.DoesScenarioCharacterExist(character))
                FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(character), work->characters[i].previous);
        }
        FTBL.HLib_DestroyTaskInternal(work->task);
        break;
    }
}

void ET_PluralControl_AddCharacter(PluralControlWork *work, shen_name character)
{
    int i, found = 0;
    short previous;
    for (i = 0; i < 8; i++)
        if (work->characters[i].character == character) {
            found = 1;
            break;
        }
    if (!found) {
        work->characters[work->count].character = character;
        previous = FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(character), 1);
        work->characters[work->count].previous = previous;
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(character), previous);
        work->count++;
    }
}
#endif
