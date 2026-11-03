#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define KRK2_CHARACTER SHEN_NAME4('C', 'G', 'T', '1')
#define KRK2_EVENT_TASK TASK_EVNT
#define KRK2_ENABLE_FOG_EFFECT 0x780
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void eKRH2_ET_WAVE_KRK2_Callback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name KRK2_CHARACTER{SHEN_NAME(CGT1)};
inline constexpr shen_name KRK2_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr Vector3 KRK2_CHARACTER_POSITION{30.0f, 100.0f, 50.0f};

void AreaKRK2Init();
void AreaKRK2Main(std::uint32_t startParameters);
void AreaKRK2Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRK2GameplayDefinition;

} // namespace shenmue::s2::areas
#endif
