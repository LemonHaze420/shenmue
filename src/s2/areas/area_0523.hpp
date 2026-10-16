#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0523_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0523_RYO{SHEN_NAME(RYO_)}, AREA_0523_RETURN_MAP{SHEN_NAME(0504)};
inline constexpr shen_name AREA_0523_a150{SHEN_NAME(a150)};
inline constexpr shen_name AREA_0523_a149{SHEN_NAME(a149)};
inline constexpr shen_name AREA_0523_a151{SHEN_NAME(a151)};
inline constexpr shen_name AREA_0523_a152{SHEN_NAME(a152)};
inline constexpr shen_name AREA_0523_a153{SHEN_NAME(a153)};
inline constexpr shen_name AREA_0523_a154{SHEN_NAME(a154)};
inline constexpr shen_name AREA_0523_a155{SHEN_NAME(a155)};
inline constexpr shen_name AREA_0523_a156{SHEN_NAME(a156)};
inline constexpr shen_name AREA_0523_a157{SHEN_NAME(a157)};
inline constexpr shen_name AREA_0523_a158{SHEN_NAME(a158)};
inline constexpr shen_name AREA_0523_a159{SHEN_NAME(a159)};
inline constexpr shen_name AREA_0523_a160{SHEN_NAME(a160)};
inline constexpr shen_name AREA_0523_a161{SHEN_NAME(a161)};
inline constexpr shen_name AREA_0523_a162{SHEN_NAME(a162)};
inline constexpr shen_name AREA_0523_a179{SHEN_NAME(a179)};
inline constexpr shen_name AREA_0523_a178{SHEN_NAME(a178)};
inline constexpr shen_name AREA_0523_a175{SHEN_NAME(a175)};
inline constexpr shen_name AREA_0523_a176{SHEN_NAME(a176)};
inline constexpr shen_name AREA_0523_a177{SHEN_NAME(a177)};
inline constexpr shen_name AREA_0523_a180{SHEN_NAME(a180)};
inline constexpr shen_name AREA_0523_a181{SHEN_NAME(a181)};
inline constexpr shen_name AREA_0523_a182{SHEN_NAME(a182)};
inline constexpr shen_name AREA_0523_a183{SHEN_NAME(a183)};
inline constexpr shen_name AREA_0523_a184{SHEN_NAME(a184)};
inline constexpr shen_name AREA_0523_a188{SHEN_NAME(a188)};
inline constexpr shen_name AREA_0523_a189{SHEN_NAME(a189)};
inline constexpr shen_name AREA_0523_a186{SHEN_NAME(a186)};
inline constexpr shen_name AREA_0523_a185{SHEN_NAME(a185)};
inline constexpr shen_name AREA_0523_a187{SHEN_NAME(a187)};
inline constexpr shen_name AREA_0523_a174{SHEN_NAME(a174)};
inline constexpr shen_name AREA_0523_PROPS[]{AREA_0523_a150, AREA_0523_a149, AREA_0523_a151, AREA_0523_a152, AREA_0523_a153, AREA_0523_a154, AREA_0523_a155,
    AREA_0523_a156, AREA_0523_a157, AREA_0523_a158, AREA_0523_a159, AREA_0523_a160, AREA_0523_a161, AREA_0523_a162, AREA_0523_a179, AREA_0523_a178,
    AREA_0523_a175, AREA_0523_a176, AREA_0523_a177, AREA_0523_a180, AREA_0523_a181, AREA_0523_a182, AREA_0523_a183, AREA_0523_a184, AREA_0523_a188,
    AREA_0523_a189, AREA_0523_a186, AREA_0523_a185, AREA_0523_a187, AREA_0523_a174, shen_name{0xffffffffU}};
struct Area0523Parameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection, state;
};
struct Area0523AuthorParameter {
    Area0523Parameter *parent;
};
static_assert(sizeof(Area0523Parameter) == 16 && offsetof(Area0523Parameter, state) == 14);
static_assert(sizeof(Area0523AuthorParameter) == 8);
void Initialize0523();
void Start0523(std::uintptr_t argument);
void Update0523(void *parameter);
void Setup0523Waves();
void Queue0523Author(Area0523Parameter *parent);
void Update0523Author(void *parameter);
void Update0523Waves(void *parameter);
extern const ScriptAreaDefinition area0523ScriptDefinition;
} // namespace shenmue::s2::areas
