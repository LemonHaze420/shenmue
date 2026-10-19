#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QA22_FANS[]{
    SHEN_NAME(FAN0),
    SHEN_NAME(FAN1),
    SHEN_NAME(FAN2),
    SHEN_NAME(FAN3),
};
void AreaQA22Init();
void AreaQA22Main(std::uint32_t startParameters);
void AreaQA22Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQA22GameplayDefinition;

} // namespace shenmue::s2::areas
