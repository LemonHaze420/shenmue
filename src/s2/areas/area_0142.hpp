#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0142_AREA{SHEN_NAME(0142)};
inline constexpr shen_name AREA_0142_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0142_A540{SHEN_NAME(a540)};
inline constexpr shen_name AREA_0142_CHARACTERS[]{AREA_0142_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0142_CONVERSATION[] = "0142";

struct E0142TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0142TaskParameter) == 0x10);
struct E0142LoopParameter {
    E0142TaskParameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(E0142LoopParameter) == 0x10);
inline constexpr shen_name AREA_0142_LIGHT{SHEN_NAME(l001)}, AREA_0142_NEXT_MAP{SHEN_NAME(0200)};
extern const ScriptAreaDefinition area0142ScriptDefinition;

} // namespace shenmue::s2::areas
