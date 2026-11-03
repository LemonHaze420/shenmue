#ifndef SHENMUE_S2_AREAS_COMMON_ET_PLURAL_CONTROL_H
#define SHENMUE_S2_AREAS_COMMON_ET_PLURAL_CONTROL_H

#include "s2/event_queued_dc.h"

typedef struct PluralControlEntry {
    int frame;
    shen_name character;
    int state;
} PluralControlEntry;

typedef struct PluralControlCharacter {
    shen_name character;
    int previous;
} PluralControlCharacter;

typedef struct PluralControlWork {
    HLibTask *task;
    unsigned char state;
    const PluralControlEntry *entries;
    unsigned char count;
    PluralControlCharacter characters[8];
} PluralControlWork;

void Queue_ET_PluralControl(const PluralControlEntry *entries);
void ET_PluralControl_Callback(void *data);
void ET_PluralControl_AddCharacter(PluralControlWork *work, shen_name character);

#endif
