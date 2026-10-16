#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/conversation.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0055_AREA{SHEN_NAME(0055)}, AREA_0055_AB00{SHEN_NAME(AB00)}, AREA_0055_WON{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0055_LIGHT{SHEN_NAME(0000)}, AREA_0055_NDAY{SHEN_NAME(NDAY)};
inline constexpr shen_name AREA_0055_A126{SHEN_NAME(a126)}, AREA_0055_A142{SHEN_NAME(a142)}, AREA_0055_A506{SHEN_NAME(a506)};
inline constexpr shen_name AREA_0055_AA50{SHEN_NAME(aa50)}, AREA_0055_A504{SHEN_NAME(a504)}, AREA_0055_A016{SHEN_NAME(a016)};
inline constexpr shen_name AREA_0055_A298{SHEN_NAME(a298)};
inline constexpr shen_name AREA_0055_CHARACTERS[]{AREA_0055_WON, AREA_AUTHOR_INVALID};
struct E0055TaskParameter {
    AreaAuthorTaskParameter author;
    ConversationBuffer *voice;
};
static_assert(sizeof(E0055TaskParameter) == 24);
static_assert(offsetof(E0055TaskParameter, voice) == 16);
extern const ScriptAreaDefinition area0055ScriptDefinition;
} // namespace shenmue::s2::areas
