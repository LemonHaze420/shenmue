#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0257_AREA SHEN_NAME4('0', '2', '5', '7')
#define AREA_0257_ANP1 SHEN_NAME4('A', 'N', 'P', '1')
#define AREA_0257_CHN_ SHEN_NAME4('C', 'H', 'N', '_')
#define AREA_0257_A428 SHEN_NAME4('a', '4', '2', '8')
#define AREA_0257_A438 SHEN_NAME4('a', '4', '3', '8')
#define AREA_0257_A534 SHEN_NAME4('a', '5', '3', '4')
#define AREA_0257_CONVERSATION "0257"
extern shen_name AREA_0257_CHARACTERS[];
extern unsigned int AREA_0257_ITEMS[];
void E0257Main(void);
void E0257MainCallback(void *);
void E0257LoopCallback(void *);
void QueueE0257Author(AreaImmediateAuthorTaskParameter *);
void ConfigureE0257Environment(void);
void CleanupE0257(AreaImmediateAuthorTaskParameter *);
void SetupE0257Characters(void);
void RestoreE0257Characters(void);
void PrepareE0257Character(shen_name);
void RestoreE0257Character(shen_name);
void HideE0257Prop(shen_name);
void ShowE0257Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0257_AREA{SHEN_NAME(0257)};
inline constexpr shen_name AREA_0257_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0257_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0257_A428{SHEN_NAME(a428)};
inline constexpr shen_name AREA_0257_A438{SHEN_NAME(a438)};
inline constexpr shen_name AREA_0257_A534{SHEN_NAME(a534)};
inline constexpr shen_name AREA_0257_CHARACTERS[]{AREA_0257_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0257_MOTION[] = "0257A.MOT", AREA_0257_CONVERSATION[] = "0257";
inline constexpr std::uint32_t AREA_0257_ITEMS[]{8001, 8002, 8003, 0xffffffffU};
extern const ScriptAreaDefinition area0257ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
