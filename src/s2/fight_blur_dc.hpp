#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstdint>

namespace shenmue {
namespace s2 {

extern std::uint8_t blurModelFlag, blurModelPreviousFlag, g_savedFightBlurFlag, streetFightProgress;
extern std::int32_t accumulatedFightBlur;
void blurInit();

} // namespace s2
} // namespace shenmue

#endif
