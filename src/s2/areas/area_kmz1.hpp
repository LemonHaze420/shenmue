#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KMZ1_NEXT_MAP{SHEN_NAME(KRK2)};
void AreaKMZ1Init();
void AreaKMZ1Main(std::uint32_t startParameters);
void AreaKMZ1Cleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaKMZ1GameplayDefinition;
} // namespace shenmue::s2::areas
