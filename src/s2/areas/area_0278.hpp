#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_sleep_dc.h"

#define AREA_0278_SYUEI SHEN_NAME4('S', 'Y', 'E', '_')
#define AREA_0278_FANGMEI SHEN_NAME4('K', 'N', 'K', '_')
#define AREA_0278_HUNHUI SHEN_NAME4('K', 'S', 'Y', '_')
#define AREA_0278_ZHOUSHAN SHEN_NAME4('K', 'S', 'M', '_')
#define AREA_0278_RETURN SHEN_NAME4('K', 'R', 'F', '1')
#define AREA_0278_A015 SHEN_NAME4('a', '0', '1', '5')
#define AREA_0278_A016 SHEN_NAME4('a', '0', '1', '6')
#define AREA_0278_A236 SHEN_NAME4('a', '2', '3', '6')
#define AREA_0278_A411 SHEN_NAME4('a', '4', '1', '1')
#define AREA_0278_A410 SHEN_NAME4('a', '4', '1', '0')
#define AREA_0278_A716 SHEN_NAME4('a', '7', '1', '6')
#define AREA_0278_AA34 SHEN_NAME4('a', 'a', '3', '4')
#define AREA_0278_L001 SHEN_NAME4('l', '0', '0', '1')
#define AREA_0278_L101 SHEN_NAME4('l', '1', '0', '1')
#define AREA_0278_L301 SHEN_NAME4('l', '3', '0', '1')
typedef struct Area0278LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} Area0278LoopParameter;
extern shen_name area0278Characters[];
void AreaMain(void);
void EventMain(void *taskData);
void QueueAuthor(AreaImmediateAuthorTaskParameter *parent);
void AuthorLoop(void *taskData);
void CreateCandles(void);
void FreeCandles(void);
void PrepareEnvironment(void);
void FinishEvent(AreaImmediateAuthorTaskParameter *parent);
void PrepareAuthorCharacters(short author);
void RestoreAuthorCharacters(void);
void HideAuthorDisplay(shen_name name);
void PrepareAuthorCharacter(shen_name name);
void RestoreAuthorCharacter(shen_name name);
void CreateTempleSmoke(void);
void CreateTempleFire(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0278_RYO{SHEN_NAME(RYO_)}, AREA_0278_SYUEI{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0278_FANGMEI{SHEN_NAME(KNK_)}, AREA_0278_HUNHUI{SHEN_NAME(KSY_)};
inline constexpr shen_name AREA_0278_ZHOUSHAN{SHEN_NAME(KSM_)}, AREA_0278_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0278_RETURN{SHEN_NAME(KRF1)};
inline constexpr shen_name AREA_0278_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l101), SHEN_NAME(l301)};
inline constexpr shen_name AREA_0278_CHARACTERS[]{AREA_0278_HUNHUI, AREA_0278_ZHOUSHAN, AREA_0278_SYUEI, AREA_0278_FANGMEI, shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0278_MODELS[]{
    SHEN_NAME(a015), SHEN_NAME(a016), SHEN_NAME(a236), SHEN_NAME(a411), SHEN_NAME(a410), SHEN_NAME(a716), SHEN_NAME(aa34)};
extern const ScriptAreaDefinition area0278ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
