#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"

namespace shenmue { namespace s2 {

extern std::int32_t g_actNumber;
extern shen_name g_currentMap;
extern std::int32_t g_entryPoint;

} } // namespace shenmue::s2

#endif
