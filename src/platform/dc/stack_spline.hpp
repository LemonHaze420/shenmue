#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#include <cstdint>

namespace shenmue { namespace dc {

static const std::int32_t kStackSplineKeyLimit = 200;
std::int32_t AllocSPLI(float *coefficients, std::int32_t count, const float *times, const float *values, const float *slopes);

} } // namespace shenmue::dc

#endif
