#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWB00Init();
void AreaWB00Main(std::uint32_t startParameters);
void AreaWB00Cleanup(std::int32_t night, std::uint32_t flags);
void SetWarehouseBooksVisible(bool visible);
void ResetWarehouseBookCharacters();
extern const GameplayAreaDefinition areaWB00GameplayDefinition;
} // namespace shenmue::s2::areas
