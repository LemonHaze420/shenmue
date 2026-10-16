#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0184_AREA{SHEN_NAME(0184)};
inline constexpr shen_name AREA_0184_A191{SHEN_NAME(a191)};
inline constexpr shen_name AREA_0184_A192{SHEN_NAME(a192)};
inline constexpr shen_name AREA_0184_LKN_{SHEN_NAME(LKN_)};
inline constexpr shen_name AREA_0184_BKN_{SHEN_NAME(BKN_)};
inline constexpr shen_name AREA_0184_MYS_{SHEN_NAME(MYS_)};
inline constexpr shen_name AREA_0184_CHARACTERS[]{AREA_0184_LKN_, AREA_0184_BKN_, AREA_0184_MYS_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0184_MOTION[] = "0184A.MOT", AREA_0184_CONVERSATION[] = "0184";
struct E0184TaskParameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int16_t state;
    std::int16_t reserved0e;
};
struct E0184FrameParameter {
    E0184TaskParameter *parent;
    std::int16_t state;
};
static_assert(sizeof(E0184TaskParameter) == 0x10);
static_assert(offsetof(E0184TaskParameter, state) == 0x0c);
static_assert(sizeof(E0184FrameParameter) == 0x10);

extern const ScriptAreaDefinition area0184ScriptDefinition;

} // namespace shenmue::s2::areas
