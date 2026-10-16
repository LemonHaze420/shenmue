#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name WE00_BIKE{SHEN_NAME(bike)}, WE00_CLOSED_SIGN{SHEN_NAME(clse)};
inline constexpr std::uint32_t WE00_NIGHT_FLAG = 0xffU, WE00_BIKE_FLAG = 0x3b1U;
inline constexpr std::int32_t WE00_NIGHT_OBJECT_INDEX = 9;
inline constexpr char WE00_AUDIO_BANK[] = "WE00_000";
inline constexpr std::uint8_t WE00_OPENING_HOUR = 8, WE00_OPEN_HOUR_SPAN = 11;

void AreaWE00Init();
void AreaWE00Main(std::uint32_t startParameters);
void AreaWE00Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
std::int32_t UpdateWE00BikeVisibility();
void AreaWE00MainLoop(void *taskData);
extern const GameplayAreaDefinition areaWE00GameplayDefinition;

} // namespace shenmue::s2::areas
