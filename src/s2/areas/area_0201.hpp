#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0201_AREA{SHEN_NAME(0201)};
inline constexpr shen_name AREA_0201_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0201_QRR0{SHEN_NAME(QRR0)};
inline constexpr shen_name AREA_0201_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0201_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0201_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0201_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0201_A270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0201_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0201_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0201_DESK{SHEN_NAME(DESK)};
inline constexpr shen_name AREA_0201_LAMP{SHEN_NAME(LAMP)};
inline constexpr shen_name AREA_0201_A358{SHEN_NAME(a358)};
inline constexpr shen_name AREA_0201_CHARACTERS[]{AREA_0201_CLJ_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0201_CONVERSATION[] = "0201";

struct E0201TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0201TaskParameter) == 0x10);
struct E0201LoopParameter {
    E0201TaskParameter *parent;
};
static_assert(sizeof(E0201LoopParameter) == 8);
inline constexpr shen_name AREA_0201_LIGHT{SHEN_NAME(l101)}, AREA_0201_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0201_AUTHORS[]{SHEN_NAME(REN1), SHEN_NAME(REN2)};
inline constexpr const char *AREA_0201_DEBUG_NAMES[]{"0201A000.c\n", "0201A010.c\n"};
extern const ScriptAreaDefinition area0201ScriptDefinition;

} // namespace shenmue::s2::areas
