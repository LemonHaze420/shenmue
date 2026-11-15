#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"

extern void *area0017Voice, *area0017Archive;

#define AREA_0017_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0017_QLN SHEN_NAME4('Q', 'L', 'N', '_')
#define AREA_0017_DEY SHEN_NAME4('D', 'E', 'Y', '_')
#define AREA_0017_QRN SHEN_NAME4('Q', 'R', 'N', '_')
#define AREA_0017_SON SHEN_NAME4('S', 'O', 'N', '_')
#define AREA_0017_TKR SHEN_NAME4('T', 'K', 'R', '_')
#define AREA_0017_LIGHT SHEN_NAME4('l', '0', '0', '1')
#define AREA_0017_DESTINATION SHEN_NAME4('A', 'R', 'B', 'T')
typedef struct Area0017TaskParameter {
    HLibTask *self;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    void *motion;
#endif
    short selection;
    short state;
} Area0017TaskParameter;
typedef struct Area0017LoopTaskParameter {
    Area0017TaskParameter *parent;
    short state;
} Area0017LoopTaskParameter;
extern const shen_name *const area0017Characters[];
extern const shen_name area0017CleanupCharacters[];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern const int area0017ArchiveRanges[3][2];
#endif
extern const shen_name area0017Characters0[], area0017Characters1[], area0017Characters2[];

void Main(void);
void MainCallback(void *taskData);
void QueueLoop(Area0017TaskParameter *parent);
void Loop0(void *taskData);
void Loop1(void *taskData);
void Loop2(void *taskData);
void ConfigureArchive(int selection);
void Setup0017(void);
void Cleanup(Area0017TaskParameter *parent);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetupCharacters(void);
#else
void SetupCharacters(short selection);
#endif
void RestoreCharacters(void);
void PrepareCharacter(shen_name name);
void RestoreCharacter(shen_name name);
void PrepareProp(shen_name name);
void RestoreProp(shen_name name);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area0017ScriptDefinition;
}
#endif
