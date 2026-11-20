#pragma once

#include "common/types.hpp"

namespace shenmue { namespace s2 {


std::int32_t CompareTokenIgnoreCase(const char *left, const char *right);
bool MatchAreaNamePattern(shen_name area, shen_name pattern);

} } // namespace shenmue::s2
