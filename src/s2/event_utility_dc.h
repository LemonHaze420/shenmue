#ifndef SHENMUE_S2_EVENT_UTILITY_DC_H
#define SHENMUE_S2_EVENT_UTILITY_DC_H
#include "s2/events/event_dc.h"

#define TASK_NPC_LOAD SHEN_NAME4('l', 'o', 'a', 'd')
#define INVALID_CHARACTER 0xffffffffU
#define RYO_CHARACTER SHEN_NAME4('R', 'Y', 'O', '_')

void HideAuthorCharacter(shen_name character);
void ShowAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name prop);
void ShowAuthorProp(shen_name prop);

HLibTask *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLibTask *startupTask);
void EVT_UTL_LoadNPCChara_Callback(void *taskData);
void EVT_UTL_SetNPCCharaVisible(int visible);
int EVT_UTL_GetNPCCharaVisible(void);
void EVT_UTL_SetNPCLowQualityHands(int enabled);
int EVT_UTL_GetNPCLowQualityHands(void);
void EVT_UTL_FreeNPCChara(const shen_name *characters);
int EVT_UTL_IsNPCCharaLoaded(void);
void EnableAuthoredCharacterFlagsAndDisplay(shen_name character);
void DisableAuthoredCharacterFlagsAndDisplay(shen_name character);
void EVT_UTL_SaveDateTime(void);
void EVT_UTL_RestoreDateTime(void);

#endif
