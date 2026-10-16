#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0039_AREA{SHEN_NAME(0039)};
inline constexpr shen_name AREA_0039_A128{SHEN_NAME(a128)};
inline constexpr shen_name AREA_0039_A474{SHEN_NAME(a474)};
inline constexpr shen_name AREA_0039_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0039_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0039_A035{SHEN_NAME(a035)};
inline constexpr shen_name AREA_0039_A030{SHEN_NAME(a030)};
inline constexpr shen_name AREA_0039_AA83{SHEN_NAME(aa83)};
inline constexpr shen_name AREA_0039_A034{SHEN_NAME(a034)};
inline constexpr shen_name AREA_0039_A033{SHEN_NAME(a033)};
inline constexpr shen_name AREA_0039_TKM_{SHEN_NAME(TKM_)};
inline constexpr shen_name AREA_0039_TNZ_{SHEN_NAME(TNZ_)};
inline constexpr shen_name AREA_0039_ANC_{SHEN_NAME(ANC_)};
inline constexpr shen_name AREA_0039_GEN_{SHEN_NAME(GEN_)};
inline constexpr shen_name AREA_0039_SBO_{SHEN_NAME(SBO_)};
inline constexpr shen_name AREA_0039_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0039_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0039_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0039_KIC_{SHEN_NAME(KIC_)};
inline constexpr shen_name AREA_0039_CLY_{SHEN_NAME(CLY_)};
inline constexpr shen_name AREA_0039_HUR_{SHEN_NAME(HUR_)};
inline constexpr shen_name AREA_0039_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0039_0002{SHEN_NAME(0002)};
inline constexpr shen_name AREA_0039_0003{SHEN_NAME(0003)};
inline constexpr shen_name AREA_0039_0004{SHEN_NAME(0004)};
inline constexpr shen_name AREA_0039_0005{SHEN_NAME(0005)};
inline constexpr shen_name AREA_0039_WS00{SHEN_NAME(WS00)};
inline constexpr shen_name AREA_0039_WBBK{SHEN_NAME(WBBK)};
inline constexpr char AREA_0039_MOTION[] = "0039A.MOT", AREA_0039_CONVERSATION[] = "0039";
inline constexpr shen_name AREA_0039_CHARACTERS_0[]{AREA_0039_TKM_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_1[]{AREA_0039_TNZ_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_2[]{AREA_0039_ANC_, AREA_0039_GEN_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_3[]{AREA_0039_ANC_, AREA_0039_GEN_, AREA_0039_SBO_, AREA_0039_CHR_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_4[]{AREA_0039_SBO_, AREA_0039_SYE_, AREA_0039_CHR_, AREA_0039_ANC_, AREA_0039_GEN_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_5[]{AREA_0039_KUN_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_6[]{AREA_0039_KIC_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_7[]{AREA_0039_CLY_, AREA_AUTHOR_INVALID};
inline constexpr shen_name AREA_0039_CHARACTERS_8[]{AREA_0039_HUR_, AREA_AUTHOR_INVALID};
inline constexpr const shen_name *AREA_0039_CHARACTER_TABLES[]{AREA_0039_CHARACTERS_0, AREA_0039_CHARACTERS_1, AREA_0039_CHARACTERS_2, AREA_0039_CHARACTERS_3,
    AREA_0039_CHARACTERS_4, AREA_0039_CHARACTERS_5, AREA_0039_CHARACTERS_6, AREA_0039_CHARACTERS_7, AREA_0039_CHARACTERS_8};
extern const ScriptAreaDefinition area0039ScriptDefinition;

} // namespace shenmue::s2::areas
