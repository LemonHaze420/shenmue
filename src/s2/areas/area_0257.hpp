#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0257_AREA{SHEN_NAME(0257)};
inline constexpr shen_name AREA_0257_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0257_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0257_A428{SHEN_NAME(a428)};
inline constexpr shen_name AREA_0257_A438{SHEN_NAME(a438)};
inline constexpr shen_name AREA_0257_A534{SHEN_NAME(a534)};
inline constexpr shen_name AREA_0257_CHARACTERS[]{AREA_0257_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0257_MOTION[] = "0257A.MOT", AREA_0257_CONVERSATION[] = "0257";
inline constexpr std::uint32_t AREA_0257_ITEMS[]{8001, 8002, 8003, 0xffffffffU};
extern const ScriptAreaDefinition area0257ScriptDefinition;

} // namespace shenmue::s2::areas
