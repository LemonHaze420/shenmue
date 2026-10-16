#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0036_AREA{SHEN_NAME(0036)};
inline constexpr shen_name AREA_0036_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0036_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0036_A104{SHEN_NAME(a104)};
inline constexpr shen_name AREA_0036_A051{SHEN_NAME(a051)};
inline constexpr shen_name AREA_0036_AB08{SHEN_NAME(ab08)};
inline constexpr shen_name AREA_0036_SYP_{SHEN_NAME(SYP_)};
inline constexpr shen_name AREA_0036_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0036_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0036_CHARACTERS[]{AREA_0036_SYP_, AREA_0036_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0036_CONVERSATION[] = "0036";

struct E0036TaskParameter {
    HLib::Task *self;
    std::uint32_t authorCommand;
    std::int16_t state;
    std::byte reserved0e[2];
};
static_assert(sizeof(E0036TaskParameter) == 0x10);
struct E0036LoopParameter {
    E0036TaskParameter *parent;
};
static_assert(sizeof(E0036LoopParameter) == 8);
inline constexpr shen_name AREA_0036_WTA0{SHEN_NAME(WTA0)}, AREA_0036_WBBK{SHEN_NAME(WBBK)};
inline constexpr shen_name AREA_0036_AUTHOR_0034{SHEN_NAME(0034)}, AREA_0036_NEXT_DAY{SHEN_NAME(NDAY)};
inline constexpr shen_name AREA_0036_LIGHT_001{SHEN_NAME(l001)}, AREA_0036_LIGHT_0001{SHEN_NAME(0001)};
inline constexpr shen_name AREA_0036_LIGHT_0000{SHEN_NAME(0000)}, AREA_0036_TEAR_CHARACTER{SHEN_NAME(ab08)};
inline constexpr shen_name AREA_0036_TEAR_MODEL{SHEN_NAME(ab08)};
inline constexpr std::int32_t AREA_0036_TEAR_PARTS[]{159, 158, 157, 156, 155, 154, 153, 152};
extern const ScriptAreaDefinition area0036ScriptDefinition;

} // namespace shenmue::s2::areas
