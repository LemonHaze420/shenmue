#pragma once
#include "s2/areas/area_0516.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0516_EVENT{SHEN_NAME(0516)};
inline constexpr shen_name AREA_0516_EVENT_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0516_EVENT_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0516_EVENT_a287{SHEN_NAME(a287)}, AREA_0516_EVENT_a288{SHEN_NAME(a288)};
inline constexpr shen_name AREA_0516_EVENT_a277{SHEN_NAME(a277)}, AREA_0516_EVENT_a704{SHEN_NAME(a704)};
inline constexpr shen_name AREA_0516_EVENT_aa33{SHEN_NAME(aa33)}, AREA_0516_EVENT_aa75{SHEN_NAME(aa75)};
inline constexpr shen_name AREA_0516_EVENT_ab38{SHEN_NAME(ab38)}, AREA_0516_EVENT_a067{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0516_EVENT_a292{SHEN_NAME(a292)}, AREA_0516_EVENT_aa39{SHEN_NAME(aa39)};
inline constexpr shen_name AREA_0516_EVENT_a956{SHEN_NAME(a956)}, AREA_0516_EVENT_a922{SHEN_NAME(a922)};
inline constexpr shen_name AREA_0516_EVENT_a924{SHEN_NAME(a924)}, AREA_0516_EVENT_aa36{SHEN_NAME(aa36)};
inline constexpr shen_name AREA_0516_EVENT_aa40{SHEN_NAME(aa40)}, AREA_0516_EVENT_aa41{SHEN_NAME(aa41)};
inline constexpr shen_name AREA_0516_EVENT_aa20{SHEN_NAME(aa20)};
inline constexpr shen_name AREA_0516_EVENT_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l101), SHEN_NAME(l011), SHEN_NAME(l021)};
inline constexpr shen_name AREA_0516_EVENT_CHARACTERS[]{AREA_0516_EVENT_SIN, shen_name{0xffffffffU}};
struct Area0516Parameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int32_t selection;
    std::int16_t state;
    std::byte reserved12[6];
};
struct Area0516AuthorParameter {
    Area0516Parameter *parent;
};
struct Area0516FireParameter {
    std::uint32_t effect, frame;
    float width, height, scalar, radiusX, radiusY, gain;
};
static_assert(sizeof(Area0516Parameter) == 24 && offsetof(Area0516Parameter, state) == 16);
static_assert(sizeof(Area0516AuthorParameter) == 8 && sizeof(Area0516FireParameter) == 32);
void Initialize0516();
void Start0516(std::uintptr_t argument);
void Update0516(void *parameter);
void Queue0516Author(Area0516Parameter *parent);
void Cleanup0516(Area0516Parameter *parent);
void Setup0516Effects(std::int32_t selection);
void Prepare0516Characters(std::int32_t selection);
void Restore0516Characters(std::int32_t selection);
void Update0516FireLighting(std::int16_t firstLight);
void Update0516FirstAuthor(void *parameter);
void Update0516SecondAuthor(void *parameter);
void Update0516ThirdAuthor(void *parameter);
void Update0516FourthAuthor(void *parameter);
void Queue0516FireStart(std::uint32_t effect, std::int32_t position);
void Update0516FireStart(void *parameter);
extern const ScriptAreaDefinition area0516ScriptDefinition;
} // namespace shenmue::s2::areas
