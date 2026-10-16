#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_AKA3_GRID_BTC1{SHEN_NAME(BTC1)};

void AreaAKA3Init();
void AreaAKA3Main(std::uint32_t startParameters);
void AreaAKA3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKA3GameplayDefinition;

} // namespace shenmue::s2::areas
