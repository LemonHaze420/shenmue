#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"

typedef struct E0023TaskParameter {
    HLibTask *self;
    int reserved;
    short selection, state;
} E0023TaskParameter;
typedef struct E0023LoopParameter {
    E0023TaskParameter *parent;
    unsigned int smoke;
} E0023LoopParameter;
extern shen_name e0023Characters[];
void E0023Main(void);
void E0023MainCallback(void *data);
void QueueE0023Loop(E0023TaskParameter *parent);
void E0023LoopCallback(void *data);
void InitializeE0023Smoke(E0023LoopParameter *work);
void ConfigureE0023Environment(void);
void CleanupE0023(E0023TaskParameter *parent);
void SetupE0023Characters(void);
void RestoreE0023Characters(void);
void PrepareE0023Character(shen_name name);
void RestoreE0023Character(shen_name name);
void HideE0023Prop(shen_name name);
void ShowE0023Prop(shen_name name);
#else
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area0023ScriptDefinition;
}

#endif
