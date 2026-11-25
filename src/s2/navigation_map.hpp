#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstdint>

namespace shenmue {
namespace s2 {

void ResetNavigationMapState();
void LoadNavigationMap();
void ReleaseNavigationMap();
void SetNavigationMapEnabled(std::int32_t enabled);
std::int32_t GetNavigationMapLoaded();
void UpdateNavigationMap();

} // namespace s2
} // namespace shenmue

#endif
