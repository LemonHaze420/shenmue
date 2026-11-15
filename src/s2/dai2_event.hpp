#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define DAI2_AUTHOR_RIGHT SHEN_NAME4('D', 'A', 'I', 'R')
#define DAI2_AUTHOR_LEFT SHEN_NAME4('D', 'A', 'I', 'L')
#define DAI2_LOOK_TASK SHEN_NAME4('l', 'o', 'o', 'k')
#define DAI2_RYO SHEN_NAME4('R', 'Y', 'O', '_')

void DAI2Main(int selector);
void DAI2Setup(void);
void DAI2WaitAuthor(void *taskData);
#else
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name DAI2_AUTHOR_RIGHT{SHEN_NAME(DAIR)}, DAI2_AUTHOR_LEFT{SHEN_NAME(DAIL)};
inline constexpr shen_name DAI2_LOOK_TASK{SHEN_NAME(look)};

void DAI2Init();
void DAI2Main(std::uintptr_t argument);
void DAI2WaitAuthor(void *taskData);
extern const ScriptAreaDefinition dai2ScriptDefinition;

} // namespace shenmue::s2
#endif
