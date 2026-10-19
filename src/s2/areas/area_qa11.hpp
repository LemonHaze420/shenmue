#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QA11_FIRST_PIGEON{SHEN_NAME(hat0)};
inline constexpr shen_name QA11_SECOND_PIGEON{SHEN_NAME(hatA)};
inline constexpr Vector3 QA11_FIRST_PIGEON_ORIGIN{232.0f, 139.70001f, 221.0f};
inline constexpr Vector3 QA11_SECOND_PIGEON_ORIGIN{251.0f, 139.70001f, 200.0f};
void AreaQA11Init();
void AreaQA11Main(std::uint32_t startParameters);
void AreaQA11Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQA11GameplayDefinition;

} // namespace shenmue::s2::areas
