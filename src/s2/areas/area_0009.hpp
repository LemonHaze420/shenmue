#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"

#define AREA_0009_AREA SHEN_NAME4('0', '0', '0', '9')
#define AREA_0009_AR02 SHEN_NAME4('A', 'R', '0', '2')
#define AREA_0009_RYO_ SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0009_LIGHT SHEN_NAME4('0', '0', '0', '0')

void Main0009(void);
void MainCallback0009(void *data);
void QueueLoop0009(AreaAuthorTaskParameter *parent);
void LoopCallback0009(void *data);
void ConfigureEnvironment0009(void);
void Cleanup0009(AreaAuthorTaskParameter *parent);
void SetupCharacters0009(void);
void RestoreCharacters0009(void);
void SetupAuthorCharacter0009(shen_name character);
void RestoreAuthorCharacter0009(shen_name character);
void HideCharacter0009(shen_name character);
void ShowCharacter0009(shen_name character);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr char AREA_0009_CUE[] = "0009A010";
extern const ScriptAreaDefinition area0009ScriptDefinition;
}
#endif
