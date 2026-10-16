#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QF40_JOY{SHEN_NAME(JOP_)};
inline constexpr shen_name QF40_DOOR{SHEN_NAME(R342)};
inline constexpr shen_name QF40_EVENT_TASK{SHEN_NAME(EVNT)};
extern std::int32_t qf40HurryFrames;
extern std::int32_t qf40BattleState;
void AreaQF40Init();
void AreaQF40Main(std::uint32_t startParameters);
void AreaQF40Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQF40GameplayDefinition;

} // namespace shenmue::s2::areas
