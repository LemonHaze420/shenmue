#pragma once

#include <cstdint>

namespace shenmue::s1 {

#define SCENARIO_ENUM_BEGIN(name) enum name : std::int32_t {
#define SCENARIO_ENUM_VALUE(table, name, value) name = value,
#define SCENARIO_ENUM_END(name) };
#include "s1/task/scen/commands.def"
#undef SCENARIO_ENUM_BEGIN
#undef SCENARIO_ENUM_VALUE
#undef SCENARIO_ENUM_END

} // namespace shenmue::s1
