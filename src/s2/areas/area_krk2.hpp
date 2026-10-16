#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name KRK2_CHARACTER{SHEN_NAME(CGT1)};
inline constexpr shen_name KRK2_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr Vector3 KRK2_CHARACTER_POSITION{30.0f, 100.0f, 50.0f};

void AreaKRK2Init();
void AreaKRK2Main(std::uint32_t startParameters);
void AreaKRK2Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRK2GameplayDefinition;

} // namespace shenmue::s2::areas
