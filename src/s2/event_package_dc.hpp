#pragma once

#include "s2/pak_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

void *LoadCurrentPakBinary(const char *filename);
std::uint32_t GetCurrentPakBinarySize(const char *filename);

} } // namespace shenmue::s2

#endif
