#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0175_AREA{SHEN_NAME(0175)};
inline constexpr shen_name AREA_0175_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0175_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0175_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0175_A417{SHEN_NAME(a417)};
inline constexpr shen_name AREA_0175_CHARACTERS[]{AREA_0175_JOP_, AREA_0175_WON_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0175_MOTION[] = "0175A.MOT", AREA_0175_CONVERSATION[] = "0175";
inline constexpr shen_name AREA_0175_AR00{SHEN_NAME(AR00)}, AREA_0175_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0175_AUTHOR_A{SHEN_NAME(175A)}, AREA_0175_AUTHOR_B{SHEN_NAME(175B)};
struct E0175TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0175TaskParameter) == 0x10);
struct E0175LoopParameter {
    E0175TaskParameter *parent;
};
static_assert(sizeof(E0175LoopParameter) == 8);
extern const ScriptAreaDefinition area0175ScriptDefinition;
} // namespace shenmue::s2::areas
