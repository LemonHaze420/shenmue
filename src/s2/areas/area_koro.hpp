#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"

#define KORO_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define KORO_REN SHEN_NAME4('R', 'E', 'N', '_')
#define KORO_TRASH SHEN_NAME4('Z', 'G', 'B', 'G')
#define KORO_TASK SHEN_NAME4('k', 'o', 'u', 's')
#define KORO_PROP SHEN_NAME4('M', '7', '3', '6')
#define KORO_NEXT_AREA SHEN_NAME4('Q', 'R', 'R', '0')
#define KORO_LIGHT_DEFINITION SHEN_NAME4('0', '0', '0', '0')
#define KORO_ENTRY_INIT SHEN_NAME4('I', 'N', 'I', 'T')
#define KORO_ENTRY_DOOR SHEN_NAME4('D', 'O', 'O', 'R')
#define KORO_ENTRY_LIGHT SHEN_NAME4('L', 'I', 'G', 'T')
#define KORO_SCRAP_COUNT 10
typedef struct KoroWork {
    int age;
    int unused04;
    int phase;
    shen_name entry;
} KoroWork;
typedef struct KoroFlags {
    char lightOn, controlsRestored, approach;
} KoroFlags;
extern shen_name koroTrashScraps[KORO_SCRAP_COUNT];
extern shen_name koroBaseId;
extern KoroWork koroWork;
extern int koroLeftHand[57], koroRightHand[57];
extern Vector3i koroLeftHandPosition, koroRightHandPosition;
void AreaKOROMain(void *argument);
void KoroPlaceCharacter(shen_name name, const Vector3 *position, int yaw);
void KoroPlayMotion(shen_name name, unsigned int motion);
void KoroInitCallback(void *data);
void KoroRoamCallback(void *data);
void KoroOnLightCallback(void *data);
void KoroDoorCallback(void *data);
void KoroDoorDoneCallback(void *data);
void KoroDustBoxCallback(void *data);
void KoroSwitchRevealCallback(void *data);
void KoroPushSwitch(void);
void KoroSwitchLightCallback(void *data);
void KoroSwitchDoneCallback(void *data);
void KoroSetTaskWithDelay(HLibTaskCallback callback, int delay);
void KoroSetTask(HLibTaskCallback callback);
int KORO_PlayDialogue(int selection);
void QueueKoroLightLoop(void);
void KoroLightLoopCallback(void);
void InterpolateKoroLights(float alpha);
void KORO_SetLightState(int animate);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaKOROInit();
void AreaKOROMain(std::uintptr_t parameter);
std::int32_t KORO_PlayDialogue(std::int32_t selection);
void KORO_SetLightState(std::int32_t animate);
extern const ScriptAreaDefinition areaKOROScriptDefinition;
} // namespace shenmue::s2::areas
#endif
