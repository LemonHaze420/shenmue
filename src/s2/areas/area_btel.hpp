#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"

#define AREA_BTEL SHEN_NAME4('B', 'T', 'E', 'L')
#define BTEL_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define BTEL_BF1 SHEN_NAME4('B', 'F', '1', '_')
#define BTEL_BF7 SHEN_NAME4('B', 'F', '7', '_')
#define BTEL_BF8 SHEN_NAME4('B', 'F', '8', '_')
#define BTEL_KF5 SHEN_NAME4('K', 'F', '5', '_')
#define BTEL_BF9 SHEN_NAME4('B', 'F', '9', '_')
#define BTEL_KR3 SHEN_NAME4('K', 'R', '3', '_')
#define BTEL_LIFT SHEN_NAME4('R', '5', '9', '9')
#define BTEL_PROP_A SHEN_NAME4('a', '5', '1', '2')
#define BTEL_PROP_B SHEN_NAME4('a', '5', '1', '6')
#define BTEL_AUTHOR SHEN_NAME4('A', 'U', '_', '0')
#define BTEL_CAMERA_DUMMY SHEN_NAME4('d', 'a', 'm', 'y')
#define BTEL_DEFAULT_GRID SHEN_NAME4('0', '0', '0', '0')
#define BTEL_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define BTEL_DEBUG_ID 3
typedef struct BtelBattleWork {
    HLibTask *self;
    signed char result;
    unsigned char phase, retries, reserved07;
    int delay;
    EventReadCharactersWork *characters;
} BtelBattleWork;
typedef struct BtelAuthorWork {
    HLibTask *self;
    int reserved04;
    shen_name author;
    short phase;
    short reserved0e;
} BtelAuthorWork;
typedef struct BtelLiftFrame {
    shen_name lift;
    int frame, floor;
} BtelLiftFrame;
extern shen_name btelCharacters[];
void AreaBTELMain(void *argument);
void StartBtelAuthor(void);
void AuthorMain_Callback(void *data);
void StartBtelAuthorLoop(BtelAuthorWork *owner);
void AuthorLoop_Callback(void *data);
void PrepareBtelAuthor(void);
void FinishBtelAuthor(BtelAuthorWork *owner);
void HideBtelCharacters(void);
void ShowBtelCharacters(void);
void HideBtelCharacter(shen_name name);
void ShowBtelCharacter(shen_name name);
void HideBtelProp(shen_name name);
void ShowBtelProp(shen_name name);
void PreparePackage(void);
void BtelDebugExitCallback(void *data);
void Exit(BtelBattleWork *work);
void Battle_Callback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBTELInit();
void AreaBTELMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBTELScriptDefinition;
} // namespace shenmue::s2::areas
#endif
