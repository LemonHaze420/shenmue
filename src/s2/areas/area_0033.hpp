#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0033_AREA{SHEN_NAME(0033)};
inline constexpr shen_name AREA_0033_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0033_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0033_A045{SHEN_NAME(a045)};
inline constexpr shen_name AREA_0033_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0033_A353{SHEN_NAME(a353)};
inline constexpr shen_name AREA_0033_A048{SHEN_NAME(a048)};
inline constexpr shen_name AREA_0033_SYP_{SHEN_NAME(SYP_)};
inline constexpr shen_name AREA_0033_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0033_SYX_{SHEN_NAME(SYX_)};
inline constexpr shen_name AREA_0033_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0033_CHARACTERS[]{AREA_0033_SYP_, AREA_0033_KUN_, AREA_0033_SYX_, AREA_0033_SYE_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0033_CONVERSATION[] = "0033";

struct E0033TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::int8_t mode;
    std::uint8_t nextDay;
    std::byte reserved0e[2];
};
static_assert(sizeof(E0033TaskParameter) == 0x10);
struct E0033LoopParameter {
    E0033TaskParameter *parent;
};
static_assert(sizeof(E0033LoopParameter) == 8);
inline constexpr shen_name AREA_0033_WTA0{SHEN_NAME(WTA0)}, AREA_0033_WTSH{SHEN_NAME(WTSH)};
inline constexpr shen_name AREA_0033_LIGHT_0{SHEN_NAME(0000)}, AREA_0033_LIGHT_1{SHEN_NAME(0001)}, AREA_0033_FIRE{SHEN_NAME(fire)};
inline constexpr shen_name AREA_0033_AUTHOR_192A{SHEN_NAME(192A)}, AREA_0033_AUTHOR_192B{SHEN_NAME(192B)};
inline constexpr shen_name AREA_0033_TABLE_0[]{SHEN_NAME(SYX_), SHEN_NAME(SYE_), AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0033_TABLE_1[]{SHEN_NAME(SYP_), SHEN_NAME(KUN_), AREA_AUTHOR_INVALID};
inline constexpr const shen_name *AREA_0033_SELECTED_CHARACTERS[]{AREA_0033_TABLE_0, AREA_0033_TABLE_1};
inline constexpr const char *AREA_0033_DEBUG_NAMES[]{
    "0033A000.c\n", "0033A010.c\n", "0033A011.c\n", "0033A012.c\n", "0033A020.c\n", "0033A021.c\n", "0033A022.c\n", "0033A030.c\n", "0033A040.c\n"};
extern const ScriptAreaDefinition area0033ScriptDefinition;

} // namespace shenmue::s2::areas
