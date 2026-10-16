#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0236_AREA{SHEN_NAME(0236)};
inline constexpr shen_name AREA_0236_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0236_A502{SHEN_NAME(a502)};
inline constexpr shen_name AREA_0236_A336{SHEN_NAME(a336)};
inline constexpr shen_name AREA_0236_AB19{SHEN_NAME(ab19)};
inline constexpr shen_name AREA_0236_AB18{SHEN_NAME(ab18)};
inline constexpr shen_name AREA_0236_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0236_YUA_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0236_MOTION[] = "0236A.MOT", AREA_0236_CONVERSATION[] = "0236";
inline constexpr const char *AREA_0236_DEBUG_NAMES[]{
    "0236A000.c\n", "0236A050.c\n", "0236A070.c\n", "0236A080.c\n", "0236A100.c\n", "0236A010.c\n", "0236A020.c\n", "0236A030.c\n"};

struct E0236TaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int32_t selection;
    std::int16_t state;
};
struct E0236FrameTaskParameter {
    E0236TaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(E0236TaskParameter) == 0x18);
static_assert(offsetof(E0236TaskParameter, motion) == 8);
static_assert(offsetof(E0236TaskParameter, selection) == 12);
static_assert(offsetof(E0236TaskParameter, state) == 16);
static_assert(sizeof(E0236FrameTaskParameter) == 0x10);
static_assert(offsetof(E0236FrameTaskParameter, currentFrame) == 8);

extern const ScriptAreaDefinition area0236ScriptDefinition;

} // namespace shenmue::s2::areas
