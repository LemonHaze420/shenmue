#pragma once

#include "common/types.hpp"

namespace shenmue {
namespace s2 {


std::int32_t CompareTokenIgnoreCase(const char *left, const char *right);
#if SHENMUE_S2_GCC295
std::int32_t MatchAreaNamePattern(shen_name area, shen_name pattern);
#else
bool MatchAreaNamePattern(shen_name area, shen_name pattern);
#endif

} // namespace s2
} // namespace shenmue
