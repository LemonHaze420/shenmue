#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0138_AREA{SHEN_NAME(0138)};
inline constexpr shen_name AREA_0138_A073{SHEN_NAME(a073)};
inline constexpr shen_name AREA_0138_A417{SHEN_NAME(a417)};
inline constexpr shen_name AREA_0138_AA50{SHEN_NAME(aa50)};
inline constexpr shen_name AREA_0138_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0138_HVA_{SHEN_NAME(HVA_)};
inline constexpr shen_name AREA_0138_TGI_{SHEN_NAME(TGI_)};
inline constexpr shen_name AREA_0138_SNC_{SHEN_NAME(SNC_)};
inline constexpr shen_name AREA_0138_SAM_{SHEN_NAME(SAM_)};
inline constexpr shen_name AREA_0138_LLY_{SHEN_NAME(LLY_)};
inline constexpr shen_name AREA_0138_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0138_ENF_{SHEN_NAME(ENF_)};
inline constexpr shen_name AREA_0138_TYZ_{SHEN_NAME(TYZ_)};
inline constexpr shen_name AREA_0138_CHARACTERS[]{
    AREA_0138_HVA_, AREA_0138_TGI_, AREA_0138_SNC_, AREA_0138_SAM_, AREA_0138_LLY_, AREA_0138_CLJ_, AREA_0138_ENF_, AREA_0138_TYZ_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0138_MOTION[] = "0138A.MOT", AREA_0138_CONVERSATION[] = "0138";
inline constexpr const char *AREA_0138_DEBUG_NAMES[]{"0138A000.c\n", "0138A010.c\n", "0138A020.c\n"};

inline constexpr const char *AREA_0138_MEDIA[]{"0138A000", "0138A010", "0138A020"};
inline constexpr const char *AREA_0138_MOTIONS[]{"0138A000.MOT", "0138A010.MOT", "0138A020.MOT"};
inline constexpr const char *AREA_0138_PACKAGES[]{"AB00", "AR02", "AKA3"};
inline constexpr shen_name AREA_0138_LIGHT_001{SHEN_NAME(l001)}, AREA_0138_LIGHT_101{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0138_LIGHT_201{SHEN_NAME(l201)}, AREA_0138_LIGHT_202{SHEN_NAME(l202)};
struct E0138TaskParameter {
    HLib::Task *self;
    std::uint32_t motions[3];
    std::int16_t selection, state;
};
static_assert(sizeof(E0138TaskParameter) == 0x18);
struct E0138LoopParameter {
    E0138TaskParameter *parent;
    std::int16_t state;
    std::byte reserved0a[6];
};
static_assert(sizeof(E0138LoopParameter) == 0x10);
extern const ScriptAreaDefinition area0138ScriptDefinition;

} // namespace shenmue::s2::areas
