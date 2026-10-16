#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaTBAKInit();
void AreaTBAKMain(std::uint32_t);
void AreaTBAKCleanup(std::int32_t afternoon, std::uint32_t);
extern const GameplayAreaDefinition areaTBAKGameplayDefinition;
} // namespace shenmue::s2::areas
