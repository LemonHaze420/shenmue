#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0151_AREA{SHEN_NAME(0151)};
inline constexpr shen_name AREA_0151_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0151_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0151_151A{SHEN_NAME(151A)};
inline constexpr shen_name AREA_0151_151B{SHEN_NAME(151B)};
inline constexpr char AREA_0151_MOTION[] = "0151A.MOT", AREA_0151_CONVERSATION[] = "0151";
struct E0151TaskParameter {
    HLib::Task *self;
    std::uint32_t selection;
    std::int16_t state;
    std::int16_t reserved0e;
};
struct E0151LoopParameter {
    E0151TaskParameter *parent;
};
static_assert(sizeof(E0151TaskParameter) == 0x10);
static_assert(offsetof(E0151TaskParameter, state) == 0x0c);
static_assert(sizeof(E0151LoopParameter) == 8);
extern const ScriptAreaDefinition area0151ScriptDefinition;

} // namespace shenmue::s2::areas
