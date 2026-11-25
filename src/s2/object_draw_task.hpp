#pragma once

#include "common/types.hpp"

namespace shenmue { namespace s2 {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define OBJECT_DRAW_SCENE_TASK SHEN_NAME(OBJE)
#else
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define OBJECT_DRAW_SCENE_TASK SHEN_NAME(OBJE)
#else
inline constexpr shen_name OBJECT_DRAW_SCENE_TASK{SHEN_NAME(OBJE)};
#endif
#endif
} } // namespace shenmue::s2
