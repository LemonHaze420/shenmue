#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0038_AREA{SHEN_NAME(0038)};
inline constexpr shen_name AREA_0038_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0038_KEK_{SHEN_NAME(KEK_)};
inline constexpr shen_name AREA_0038_A058{SHEN_NAME(a058)};
inline constexpr shen_name AREA_0038_A059{SHEN_NAME(a059)};
inline constexpr shen_name AREA_0038_A060{SHEN_NAME(a060)};
inline constexpr shen_name AREA_0038_A061{SHEN_NAME(a061)};
inline constexpr shen_name AREA_0038_AA62{SHEN_NAME(aa62)};
inline constexpr shen_name AREA_0038_AA65{SHEN_NAME(aa65)};
inline constexpr shen_name AREA_0038_A476{SHEN_NAME(a476)};
inline constexpr shen_name AREA_0038_A057{SHEN_NAME(a057)};
inline constexpr shen_name AREA_0038_A037{SHEN_NAME(a037)};
inline constexpr shen_name AREA_0038_A420{SHEN_NAME(a420)};
inline constexpr shen_name AREA_0038_A421{SHEN_NAME(a421)};
inline constexpr shen_name AREA_0038_A127{SHEN_NAME(a127)};
inline constexpr shen_name AREA_0038_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0038_0001{SHEN_NAME(0001)};
inline constexpr shen_name AREA_0038_CHARACTERS[]{AREA_0038_CHO_, AREA_0038_KEK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0038_MOTION[] = "0038A.MOT", AREA_0038_CONVERSATION[] = "0038";
inline constexpr const char *AREA_0038_DEBUG_NAMES[]{"0038A040.c\n", "0038A000.c\n", "0038A010.c\n", "0038A030.c\n"};
struct E0038LoopParameter {
    AreaAuthorTaskParameter *parent;
    std::uint16_t currentFrame, reserved0a;
    std::uint32_t smoke;
};
static_assert(sizeof(E0038LoopParameter) == 0x10);
static_assert(offsetof(E0038LoopParameter, smoke) == 0x0c);
extern const ScriptAreaDefinition area0038ScriptDefinition;

} // namespace shenmue::s2::areas
