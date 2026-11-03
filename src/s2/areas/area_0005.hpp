#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"

#define AREA_0005 SHEN_NAME4('0', '0', '0', '5')
#define AREA_0005_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0005_SYM SHEN_NAME4('S', 'Y', 'M', '_')
#define AREA_0005_KYG SHEN_NAME4('K', 'Y', 'G', '_')
#define AREA_0005_INVALID 0xffffffffU
#define AREA_0005_RECURSIVE_XYZ 0x78000000U

typedef struct Area0005EventWork {
    HLibTask *task;
    void *motion;
    short entryId, authorState, characterTable, loadState, characterIndex;
} Area0005EventWork;
typedef struct Area0005LoopWork {
    Area0005EventWork *parent;
    int smoke;
} Area0005LoopWork;

typedef struct Area0005WaveColours {
    unsigned int reserved00[5];
    unsigned int specular;
    unsigned int reserved18;
    unsigned int diffuse;
} Area0005WaveColours;

void e0005_EVT_Load_AuthChara_Enqueue(void *argument);
void e0005_EVT_Auth_Main(void *taskData);
void QueueAuthorLoop(Area0005EventWork *parent);
void e0005_EVT_Auth_Loop(void *taskData);
void e0005_EVT_Auth_Loop_0(void *taskData);
void e0005_EVT_Auth_Loop_0005A010_Callback(void *taskData);
void e0005_EVT_Auth_Loop_1(void *taskData);
void e0005_EVT_Auth_Loop_2(void *taskData);
void e0005_EVT_Load_AuthChara_Callback(void *taskData);
void e0005_EVT_Load_NpcChara(void *taskData);
void e0005_EVT_Load_NpcChara2_Callback(void *taskData);
void e0005_EVT_Exit_AuthChara_Callback(void *taskData);
void e0005_EVT_Exit_NpcChara_Callback(void *taskData);
void e0005_EVT_Exit_NpcChara2_Callback(void *taskData);
void CreateLighterSmoke(Area0005LoopWork *child);
void PlaceRyoForEntry(short entry);
void InitializeEventEnvironment(void);
void CompleteEvent(Area0005EventWork *event);
void PrepareArrivalCharacters(void);
void RestoreArrivalCharacters(void);
void PrepareLighterCharacters(void);
void RestoreLighterCharacters(void);
void PreparePhotographCharacters(void);
void RestorePhotographCharacters(void);
void PrepareAuthorCharacterMode(shen_name name);
void RestoreAuthorCharacterMode(shen_name name);
void PrepareCharacter(shen_name name);
void RestoreCharacter(shen_name name);
void e0005_ET_WAVE_AR01_Callback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area0005ScriptDefinition;
}
#endif
