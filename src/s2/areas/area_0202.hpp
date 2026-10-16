#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0202_AREA{SHEN_NAME(0202)};
inline constexpr shen_name AREA_0202_YAN_{SHEN_NAME(YAN_)};
inline constexpr shen_name AREA_0202_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0202_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0202_A212{SHEN_NAME(a212)};
inline constexpr shen_name AREA_0202_A057{SHEN_NAME(a057)};
inline constexpr shen_name AREA_0202_AA21{SHEN_NAME(aa21)};
inline constexpr shen_name AREA_0202_AA22{SHEN_NAME(aa22)};
inline constexpr shen_name AREA_0202_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0202_YAN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0202_MOTION[] = "0202A.MOT", AREA_0202_CONVERSATION[] = "0202";
inline constexpr const char *AREA_0202_DEBUG_NAMES[]{"0202A000.c\n", "0203A000.c\n"};

inline constexpr char AREA_0202_SECOND_MOTION[] = "0203A.MOT", AREA_0202_SECOND_CONVERSATION[] = "0203";
struct E0202TaskParameter {
    HLib::Task *self;
    std::uint32_t motion, secondMotion;
    std::int16_t selection, state;
    std::uint32_t reserved14;
};
struct E0202LoopParameter {
    E0202TaskParameter *parent;
};
static_assert(sizeof(E0202TaskParameter) == 0x18);
static_assert(offsetof(E0202TaskParameter, selection) == 0x10);
static_assert(offsetof(E0202TaskParameter, state) == 0x12);
static_assert(sizeof(E0202LoopParameter) == 8);

extern const ScriptAreaDefinition area0202ScriptDefinition;

} // namespace shenmue::s2::areas
