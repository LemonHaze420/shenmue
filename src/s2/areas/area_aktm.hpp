#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/sk_event_dc.h"

#define AKTM_DEBUG_ID 3
#define AKTM_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AKTM_TGI SHEN_NAME4('T', 'G', 'I', '_')
#define AKTM_HVA SHEN_NAME4('H', 'V', 'A', '_')
#define AKTM_HV6 SHEN_NAME4('H', 'V', '6', '_')
#define AKTM_HV7 SHEN_NAME4('H', 'V', '7', '_')
#define AKTM_HV8 SHEN_NAME4('H', 'V', '8', '_')
#define AKTM_KEI SHEN_NAME4('K', 'E', 'I', '_')
#define AKTM_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define AKTM_PROP SHEN_NAME4('a', '0', '1', '5')
#define AKTM_LIGHT SHEN_NAME4('l', '0', '7', '1')
#define AKTM_ABLB SHEN_NAME4('A', 'B', 'L', 'B')
#define AKTM_START_AUTHOR SHEN_NAME4('S', 'T', 'R', 'T')
#define AKTM_WIN_AUTHOR SHEN_NAME4('W', 'I', 'N', '_')

typedef struct AktmBattleWork {
    HLibTask *self;
    short phase, reserved06;
    EventReadCharactersWork *characters;
} AktmBattleWork;

void AreaAKTMMain(void);
void QueueBattle_Callback(void *argument);
int SetupAktmBattle(AktmBattleWork *work);
int FinishAktmBattle(AktmBattleWork *work);
void End_Callback(void *data);
void Battle_Callback(void *data);
int StartAktmAuthor(AktmBattleWork *work);
int StartAktmWinAuthor(AktmBattleWork *work);
int InitializeFight(AktmBattleWork *work);
int WaitAktmFight(AktmBattleWork *work);
int RetryAktmFight(AktmBattleWork *work);
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
int StartAktmFight(AktmBattleWork *work);
int ReloadAktmRetryTitle(AktmBattleWork *work);
#endif
void ResetOpponent(shen_name name);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAKTMInit();
void AreaAKTMMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaAKTMScriptDefinition;
} // namespace shenmue::s2::areas
#endif
