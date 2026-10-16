#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name CHARACTER_QDKJ_TARGET{SHEN_NAME(a321)};

void AreaQDKJInit();
void AreaQDKJMain(std::uint32_t startParameters);
void AreaQDKJCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQDKJGameplayDefinition;

} // namespace shenmue::s2::areas
