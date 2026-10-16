#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name WT00_FIELD_POINT{SHEN_NAME(BTC1)};
void AreaWT00Init();
void AreaWT00Main(std::uint32_t startParameters);
void AreaWT00Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWT00GameplayDefinition;

} // namespace shenmue::s2::areas
