#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0207_AREA{SHEN_NAME(0207)};
inline constexpr shen_name AREA_0207_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0207_AB05{SHEN_NAME(ab05)};
inline constexpr shen_name AREA_0207_A202{SHEN_NAME(a202)};
inline constexpr shen_name AREA_0207_A203{SHEN_NAME(a203)};
inline constexpr shen_name AREA_0207_A247{SHEN_NAME(a247)};
inline constexpr shen_name AREA_0207_A269{SHEN_NAME(a269)};
inline constexpr shen_name AREA_0207_A267{SHEN_NAME(a267)};
inline constexpr shen_name AREA_0207_A268{SHEN_NAME(a268)};
inline constexpr shen_name AREA_0207_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0207_SCX_{SHEN_NAME(SCX_)};
inline constexpr shen_name AREA_0207_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0207_CHARACTERS[]{AREA_0207_CHR_, AREA_0207_SCX_, AREA_0207_TGY_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0207_MOTION[] = "0207A.MOT", AREA_0207_CONVERSATION[] = "0207";
inline constexpr const char *AREA_0207_DEBUG_NAMES[]{"0207A000.c\n", "0208A000.c\n"};

inline constexpr char AREA_0207_SECOND_MOTION[] = "0208A.MOT", AREA_0207_SECOND_CONVERSATION[] = "0208";
struct E0207TaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::uint32_t secondMotion;
    std::int16_t selection;
    std::int16_t state;
    std::int32_t field14;
};
struct E0207LoopTaskParameter {
    E0207TaskParameter *parent;
};
static_assert(sizeof(E0207TaskParameter) == 0x18);
static_assert(offsetof(E0207TaskParameter, motion) == 8);
static_assert(offsetof(E0207TaskParameter, secondMotion) == 12);
static_assert(offsetof(E0207TaskParameter, selection) == 16);
static_assert(offsetof(E0207TaskParameter, state) == 18);
static_assert(sizeof(E0207LoopTaskParameter) == 8);

extern const ScriptAreaDefinition area0207ScriptDefinition;

} // namespace shenmue::s2::areas
