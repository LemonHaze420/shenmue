#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#include "common/types.hpp"

namespace shenmue { namespace dc {

std::int32_t SignedDivide(std::int32_t numerator, std::int32_t denominator) asm("_shenmue_dc_divide_impl");
std::int32_t SignedModulo(std::int32_t numerator, std::int32_t denominator) asm("_shenmue_dc_modulo_impl");

} } // namespace shenmue::dc

#endif
