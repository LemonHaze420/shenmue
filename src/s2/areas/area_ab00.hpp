#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAB00Init();
void AreaAB00Main(std::uint32_t startParameters);
void AreaAB00Cleanup(std::int32_t afternoon, std::uint32_t flags);
void eAB00_EVT_SleepEvent_SleepFunc2(void *);
extern const GameplayAreaDefinition areaAB00GameplayDefinition;
} // namespace shenmue::s2::areas
