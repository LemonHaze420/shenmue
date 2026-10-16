#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0200_AREA{SHEN_NAME(0200)}, AREA_0200_QR00{SHEN_NAME(QR00)}, AREA_0200_BASE{SHEN_NAME(base)};
inline constexpr shen_name AREA_0200_A088{SHEN_NAME(a088) | 0x800U};
inline constexpr shen_name AREA_0200_A237{SHEN_NAME(a016) | 0x1020200U};
inline constexpr shen_name AREA_0200_A238{SHEN_NAME(a020) | 0x8010200U};
inline constexpr shen_name AREA_0200_A142{SHEN_NAME(a142) | 0x200U};
inline constexpr shen_name AREA_0200_A143{SHEN_NAME(a143) | 0x200U};
inline constexpr shen_name AREA_0200_A219{SHEN_NAME(a219) | 0x20000U};
inline constexpr shen_name AREA_0200_AB20{SHEN_NAME(ab20) | 0x2010000U};
inline constexpr shen_name AREA_0200_A814{SHEN_NAME(a814)};
inline constexpr shen_name AREA_0200_A816{SHEN_NAME(a816)};
inline constexpr shen_name AREA_0200_A818{SHEN_NAME(a818)};
inline constexpr shen_name AREA_0200_A820{SHEN_NAME(a820)};
inline constexpr shen_name AREA_0200_CHARACTERS[]{
    SHEN_NAME(OJ5_),
    SHEN_NAME(BUS_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0200ScriptDefinition;
} // namespace shenmue::s2::areas
