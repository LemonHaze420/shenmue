#pragma once

#include "s2/area_event.hpp"
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name KMZ4_DANDELION_TASK{SHEN_NAME(bomR)};

void AreaKMZ4Init();
void AreaKMZ4Main(std::uint32_t startParameters);
void AreaKMZ4Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKMZ4GameplayDefinition;

} // namespace shenmue::s2::areas
