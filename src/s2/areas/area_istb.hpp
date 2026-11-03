#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define ISTB_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define ISTB_SIN SHEN_NAME4('S', 'I', 'N', '_')
#define ISTB_AREA_KRK2 SHEN_NAME4('K', 'R', 'K', '2')
#define ISTB_AREA_KRK4 SHEN_NAME4('K', 'R', 'K', '4')
#define ISTB_EXIT_KRK2 SHEN_NAME4('K', 'M', 'Z', '2')
#define ISTB_EXIT_KRK4 SHEN_NAME4('K', 'M', 'Z', '3')
#define ISTB_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define ISTB_AUTHOR_START SHEN_NAME4('I', 'S', 'I', '0')
#define ISTB_AUTHOR_RETRY SHEN_NAME4('I', 'S', 'I', '1')
#define ISTB_AUTHOR_THROW SHEN_NAME4('I', 'S', 'I', '2')
#define ISTB_AUTHOR_QTE SHEN_NAME4('I', 'S', 'I', '3')
#define ISTB_AUTHOR_START_KRK4 SHEN_NAME4('I', 'S', 'I', '5')
#define ISTB_AUTHOR_THROW_KRK4 SHEN_NAME4('I', 'S', 'I', '6')
#define ISTB_AUTHOR_QTE_KRK4 SHEN_NAME4('I', 'S', 'I', '7')
#define ISTB_AUTHOR_RETRY_KRK4 SHEN_NAME4('I', 'S', 'I', '8')
#define ISTB_SET_GRAVITY(name, enabled) do { FTBL.EV_SetCharaControllerFlags(name, 2, enabled); FTBL.EV_SetCharaControllerFlags(name, 3, enabled); } while (0)

typedef struct StoneWork {
    HLibTask *self;
    int selection, reserved08;
    unsigned short phase, retries;
    unsigned short qteAge, retryAge;
    int succeeded;
} StoneWork;

void AreaISTBMain(void);
void Init2_Callback(void *data);
void Main_Callback(void *data);
void Qte_Callback(void *data);
void Auth1_Callback(void *data);
void Auth2_Callback(void *data);
void RetryTitle_Callback(void *data);
int BeginQte(StoneWork *work);
void SpawnSplash_Callback(void *data);
void Splash_Callback(void *data);
void Exit_Callback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaISTBInit();
void AreaISTBMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaISTBScriptDefinition;
} // namespace shenmue::s2::areas
#endif
