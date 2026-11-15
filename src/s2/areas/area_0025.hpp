#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"
typedef struct E0025TaskParameter {
    HLibTask *self;
    int reserved;
    short selection, state;
} E0025TaskParameter;
typedef struct E0025LoopParameter { E0025TaskParameter *parent; } E0025LoopParameter;
extern shen_name e0025Characters[];
void E0025Main(void);
void E0025MainCallback(void *data);
void QueueE0025Loop(E0025TaskParameter *parent);
void E0025LoopCallback(void *data);
void InitializeE0025Smoke(void);
void ConfigureE0025Environment(void);
void CleanupE0025(E0025TaskParameter *parent);
void SetupE0025Characters(void);
void RestoreE0025Characters(void);
void PrepareE0025Character(shen_name name);
void RestoreE0025Character(shen_name name);
void HideE0025Prop(shen_name name);
void ShowE0025Prop(shen_name name);
#else
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area0025ScriptDefinition;
}

#endif
