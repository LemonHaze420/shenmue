#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
typedef struct Area0014LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} Area0014LoopParameter;
extern shen_name characters[];
void Main(void);
void MainCallback(void *data);
void LoopCallback(void *data);
void ConfigureEnvironment(void);
void SetupCharacters(void);
void RestoreCharacters(void);
void Prepare0014Character(shen_name name);
void Restore0014Character(shen_name name);
void Hide0014Prop(shen_name name);
void Show0014Prop(shen_name name);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area0014ScriptDefinition;
}

#endif
