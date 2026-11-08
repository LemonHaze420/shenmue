#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define Q200_EVENT_TASK TASK_EVNT
#define Q200_FOUNTAIN_TASK SHEN_NAME4('f', 'n', 't', 'n')
#define Q200_WORKER_1 SHEN_NAME4('c', 'h', 'w', '1')
#define Q200_WORKER_2 SHEN_NAME4('c', 'h', 'w', '2')
#define Q200_WORKER_3 SHEN_NAME4('c', 'h', 'w', '3')
#define Q200_WORKER_4 SHEN_NAME4('c', 'h', 'w', '4')
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void Q200LoadSceneAssets(int afternoon);
void Q200MainloopNullsub(void *taskData);
void Q200FountainCallback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name Q200_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name Q200_FOUNTAIN_TASK{SHEN_NAME(fntn)};
void AreaQ200Init();
void AreaQ200Main(std::uint32_t startParameters);
void AreaQ200Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQ200GameplayDefinition;

} // namespace shenmue::s2::areas
#endif
