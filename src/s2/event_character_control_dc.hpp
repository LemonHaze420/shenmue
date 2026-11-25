#pragma once

#include "s2/scenario_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct MapEventCallbackEntry;

std::uint32_t GetCurrentControlInput(std::int32_t selector);
bool EV_SetCharaControllerFlags(shen_name name, std::uint32_t selector, std::int32_t operation);
void SetCharacterMapRegionEnabled(Character *character, std::uint32_t region, std::int32_t enabled);
std::int32_t IsCharacterMapRegionEnabled(Character *character, std::uint32_t region);
std::uint32_t GetCharacterMapRegion(Character *character);
void SetCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region, std::uint32_t enabled);
std::int32_t IsCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region);
std::uint32_t GetCharacterMapEventRegion(shen_name name);
void SetMapEventCallbacks(const MapEventCallbackEntry *callbacks);
float SetScenarioCharacterRadius(shen_name name, float radius);
std::int32_t GetNpcBatchCountOverride();
std::uint32_t EV_SetMapLightingPlacementEnabled(std::int32_t index, std::int32_t enabled);

} // namespace s2
} // namespace shenmue

#endif
