#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0523_RETURN_MAP SHEN_NAME4('0', '5', '0', '4')
#define AREA_0523_a150 SHEN_NAME4('a', '1', '5', '0')
#define AREA_0523_a149 SHEN_NAME4('a', '1', '4', '9')
#define AREA_0523_a151 SHEN_NAME4('a', '1', '5', '1')
#define AREA_0523_a152 SHEN_NAME4('a', '1', '5', '2')
#define AREA_0523_a153 SHEN_NAME4('a', '1', '5', '3')
#define AREA_0523_a154 SHEN_NAME4('a', '1', '5', '4')
#define AREA_0523_a155 SHEN_NAME4('a', '1', '5', '5')
#define AREA_0523_a156 SHEN_NAME4('a', '1', '5', '6')
#define AREA_0523_a157 SHEN_NAME4('a', '1', '5', '7')
#define AREA_0523_a158 SHEN_NAME4('a', '1', '5', '8')
#define AREA_0523_a159 SHEN_NAME4('a', '1', '5', '9')
#define AREA_0523_a160 SHEN_NAME4('a', '1', '6', '0')
#define AREA_0523_a161 SHEN_NAME4('a', '1', '6', '1')
#define AREA_0523_a162 SHEN_NAME4('a', '1', '6', '2')
#define AREA_0523_a179 SHEN_NAME4('a', '1', '7', '9')
#define AREA_0523_a178 SHEN_NAME4('a', '1', '7', '8')
#define AREA_0523_a175 SHEN_NAME4('a', '1', '7', '5')
#define AREA_0523_a176 SHEN_NAME4('a', '1', '7', '6')
#define AREA_0523_a177 SHEN_NAME4('a', '1', '7', '7')
#define AREA_0523_a180 SHEN_NAME4('a', '1', '8', '0')
#define AREA_0523_a181 SHEN_NAME4('a', '1', '8', '1')
#define AREA_0523_a182 SHEN_NAME4('a', '1', '8', '2')
#define AREA_0523_a183 SHEN_NAME4('a', '1', '8', '3')
#define AREA_0523_a184 SHEN_NAME4('a', '1', '8', '4')
#define AREA_0523_a188 SHEN_NAME4('a', '1', '8', '8')
#define AREA_0523_a189 SHEN_NAME4('a', '1', '8', '9')
#define AREA_0523_a186 SHEN_NAME4('a', '1', '8', '6')
#define AREA_0523_a185 SHEN_NAME4('a', '1', '8', '5')
#define AREA_0523_a187 SHEN_NAME4('a', '1', '8', '7')
#define AREA_0523_a174 SHEN_NAME4('a', '1', '7', '4')
extern const shen_name area0523Props[];
void Start0523(void);
void Update0523(void *parameter);
void Queue0523Author(AreaAuthorTaskParameter *parent);
void Update0523Author(void *parameter);
void Setup0523Waves(void);
void Cleanup0523(AreaAuthorTaskParameter *parent);
void Setup0523Characters(void);
void Restore0523Characters(void);
void Update0523Waves(void *parameter);
#else
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
#endif
