#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0048_AREA{SHEN_NAME(0048)}, AREA_0048_YTQB{SHEN_NAME(YTQB)}, AREA_0048_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0048_LIGHT{SHEN_NAME(0000)}, AREA_0048_SYE{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0048_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0048_A021_SHIFT{SHEN_NAME(a021) | 0x400U};
inline constexpr shen_name AREA_0048_A020_SHIFT{SHEN_NAME(a020) | 0x400U};
inline constexpr shen_name AREA_0048_A073{SHEN_NAME(a073)};
inline constexpr shen_name AREA_0048_A020{SHEN_NAME(a020)};
inline constexpr shen_name AREA_0048_A022{SHEN_NAME(a022)};
inline constexpr shen_name AREA_0048_A127{SHEN_NAME(a127) | 0x10000U};
inline constexpr shen_name AREA_0048_A128{SHEN_NAME(a128) | 0x10000U};
inline constexpr shen_name AREA_0048_A009{SHEN_NAME(a009) | 0x30100U};
inline constexpr shen_name AREA_0048_A041{SHEN_NAME(a041) | 0x100U};
inline constexpr shen_name AREA_0048_A021{SHEN_NAME(a021)};
inline constexpr shen_name AREA_0048_A417{SHEN_NAME(a417) | 0x20000U};
inline constexpr shen_name AREA_0048_AA84{SHEN_NAME(aa84) | 0x2010000U};
inline constexpr shen_name AREA_0048_A412{SHEN_NAME(a412)};
inline constexpr shen_name AREA_0048_AA63{SHEN_NAME(aa63) | 0x10000U};
inline constexpr shen_name AREA_0048_CHARACTERS_0[]{
    SHEN_NAME(RKN_),
    SHEN_NAME(SBK_),
    SHEN_NAME(SKH_),
    shen_name{0xffffffffU},
};
inline constexpr shen_name AREA_0048_CHARACTERS_1[]{
    SHEN_NAME(CHO_),
    SHEN_NAME(SBO_),
    SHEN_NAME(SCX_),
    SHEN_NAME(YOH_),
    SHEN_NAME(GEN_),
    SHEN_NAME(ESY_),
    shen_name{0xffffffffU},
};
inline constexpr shen_name AREA_0048_CHARACTERS_2[]{
    SHEN_NAME(CHO_),
    shen_name{0xffffffffU},
};
inline constexpr shen_name AREA_0048_CHARACTERS_3[]{
    SHEN_NAME(CHO_),
    SHEN_NAME(SEE_),
    SHEN_NAME(III_),
    SHEN_NAME(ESY_),
    SHEN_NAME(GEN_),
    SHEN_NAME(YUA_),
    shen_name{0xffffffffU},
};
inline constexpr shen_name AREA_0048_CHARACTERS_4[]{
    SHEN_NAME(CHO_),
    SHEN_NAME(ESY_),
    SHEN_NAME(GEN_),
    SHEN_NAME(III_),
    SHEN_NAME(SEE_),
    SHEN_NAME(MRC_),
    SHEN_NAME(RKN_),
    SHEN_NAME(SYE_),
    SHEN_NAME(YUA_),
    shen_name{0xffffffffU},
};
inline constexpr shen_name AREA_0048_CHARACTERS_5[]{
    SHEN_NAME(CHO_),
    SHEN_NAME(SEE_),
    SHEN_NAME(III_),
    SHEN_NAME(ESY_),
    SHEN_NAME(GEN_),
    SHEN_NAME(YUA_),
    shen_name{0xffffffffU},
};
inline constexpr const shen_name *AREA_0048_CHARACTERS[]{
    AREA_0048_CHARACTERS_0, AREA_0048_CHARACTERS_1, AREA_0048_CHARACTERS_2, AREA_0048_CHARACTERS_3, AREA_0048_CHARACTERS_4, AREA_0048_CHARACTERS_5};
inline constexpr shen_name AREA_0048_SELECTED_LIGHTS[]{
    shen_name{825241708U}, shen_name{825242220U}, SHEN_NAME(l701), shen_name{825242476U}, shen_name{825242732U}, shen_name{825242988U}};
struct E0048Light {
    std::uint32_t index;
    float base, amplitude;
};
struct E0048TimedLights {
    std::int32_t frame;
    const E0048Light *lights;
    std::int32_t count;
};
static_assert(sizeof(E0048Light) == 12);
static_assert(sizeof(E0048TimedLights) == 24);
inline constexpr E0048Light AREA_0048_LIGHTS_0[]{
    {44U, 1.2000000476837158f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_1[]{
    {44U, 1.2000000476837158f, 0.05000000074505806f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_2[]{
    {40U, 1.0f, 0.07999999821186066f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_3[]{
    {42U, 2.0f, 2.0f},
    {43U, 0.800000011920929f, 0.07999999821186066f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_4[]{
    {44U, 1.5f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_5[]{
    {42U, 1.0f, 0.20000000298023224f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_6[]{
    {42U, 1.0f, 0.4000000059604645f},
    {46U, 1.0f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_7[]{
    {42U, 1.5f, 0.4000000059604645f},
    {46U, 1.5f, 0.20000000298023224f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_8[]{
    {42U, 1.5f, 0.20000000298023224f},
    {46U, 1.5f, 0.10000000149011612f},
    {47U, 1.5f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_9[]{
    {46U, 2.0f, 0.10000000149011612f},
    {47U, 2.0f, 0.5f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_10[]{
    {46U, 1.0f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_11[]{
    {42U, 1.0f, 0.10000000149011612f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_12[]{
    {40U, 1.5f, 0.20000000298023224f},
    {42U, 1.0f, 0.10000000149011612f},
    {43U, 3.0f, 0.30000001192092896f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_13[]{
    {43U, 1.5f, 0.30000001192092896f},
    {44U, 1.5f, 0.15000000596046448f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_14[]{
    {43U, 1.5f, 0.05000000074505806f},
    {44U, 1.5f, 0.05000000074505806f},
};
inline constexpr E0048Light AREA_0048_LIGHTS_15[]{
    {44U, 1.5f, 0.10000000149011612f},
    {48U, 1.5f, 0.10000000149011612f},
};
inline constexpr E0048TimedLights AREA_0048_TIMED_LIGHTS[]{
    {1080, AREA_0048_LIGHTS_0, 1},
    {1890, AREA_0048_LIGHTS_1, 1},
    {2190, AREA_0048_LIGHTS_2, 1},
    {2890, AREA_0048_LIGHTS_3, 2},
    {4210, AREA_0048_LIGHTS_4, 1},
    {4460, AREA_0048_LIGHTS_5, 1},
    {4530, AREA_0048_LIGHTS_6, 2},
    {4750, AREA_0048_LIGHTS_7, 2},
    {4913, AREA_0048_LIGHTS_8, 3},
    {5180, AREA_0048_LIGHTS_9, 2},
    {5320, AREA_0048_LIGHTS_10, 1},
    {5720, AREA_0048_LIGHTS_11, 1},
    {6360, AREA_0048_LIGHTS_12, 3},
    {6800, AREA_0048_LIGHTS_13, 2},
    {7160, AREA_0048_LIGHTS_14, 2},
    {-1, AREA_0048_LIGHTS_15, 2},
};
extern const ScriptAreaDefinition area0048ScriptDefinition;
} // namespace shenmue::s2::areas
