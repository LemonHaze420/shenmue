#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0037_AREA{SHEN_NAME(0037)};
inline constexpr shen_name AREA_0037_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0037_SRY_{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0037_A115{SHEN_NAME(a115)};
inline constexpr shen_name AREA_0037_A054{SHEN_NAME(a054)};
inline constexpr shen_name AREA_0037_A055{SHEN_NAME(a055)};
inline constexpr shen_name AREA_0037_A092{SHEN_NAME(a092)};
inline constexpr shen_name AREA_0037_A536{SHEN_NAME(a536)};
inline constexpr shen_name AREA_0037_A538{SHEN_NAME(a538)};
inline constexpr shen_name AREA_0037_A057{SHEN_NAME(a057)};
inline constexpr shen_name AREA_0037_A404{SHEN_NAME(a404)};
inline constexpr shen_name AREA_0037_A405{SHEN_NAME(a405)};
inline constexpr shen_name AREA_0037_A403{SHEN_NAME(a403)};
inline constexpr shen_name AREA_0037_A406{SHEN_NAME(a406)};
inline constexpr shen_name AREA_0037_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0037_CHARACTERS[]{AREA_0037_SYE_, AREA_0037_SRY_, AREA_0037_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0037_MOTION[] = "0037A.MOT", AREA_0037_CONVERSATION[] = "0037";
inline constexpr const char *AREA_0037_DEBUG_NAMES[]{"0037A000.c\n", "0037A030.c\n", "0037A040.c\n", "0037A035.c\n", "0037A050.c\n"};
struct E0037LoopParameter {
    AreaAuthorTaskParameter *parent;
    std::uint32_t leaves, reserved0c;
};
static_assert(sizeof(E0037LoopParameter) == 0x10);
static_assert(offsetof(E0037LoopParameter, leaves) == 8);
extern const ScriptAreaDefinition area0037ScriptDefinition;

} // namespace shenmue::s2::areas
