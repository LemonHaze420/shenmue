#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0056_AREA{SHEN_NAME(0056)};
inline constexpr shen_name AREA_0056_A358{SHEN_NAME(a358)};
inline constexpr shen_name AREA_0056_A118{SHEN_NAME(a118)};
inline constexpr shen_name AREA_0056_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0056_SAM_{SHEN_NAME(SAM_)};
inline constexpr shen_name AREA_0056_LLY_{SHEN_NAME(LLY_)};
inline constexpr shen_name AREA_0056_CHARACTERS[]{AREA_0056_CLJ_, AREA_0056_SAM_, AREA_0056_LLY_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0056_CONVERSATION[] = "0056";

struct E0056TaskParameter {
    HLib::Task *self;
    std::uint32_t selection;
    std::int16_t state;
    std::byte reserved0e[2];
};
static_assert(sizeof(E0056TaskParameter) == 0x10);
struct E0056LoopParameter {
    E0056TaskParameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(E0056LoopParameter) == 0x10);
inline constexpr shen_name AREA_0056_LIGHT{SHEN_NAME(0000)}, AREA_0056_ARKQ{SHEN_NAME(ARKQ)};
inline constexpr shen_name AREA_0056_AUTHORS[]{SHEN_NAME(056A), SHEN_NAME(056B), SHEN_NAME(056C)};
extern const ScriptAreaDefinition area0056ScriptDefinition;

} // namespace shenmue::s2::areas
