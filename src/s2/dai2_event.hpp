#pragma once

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
