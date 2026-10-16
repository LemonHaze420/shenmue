#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/math.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0547_MAP{SHEN_NAME(KWW1)}, AREA_0547_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0547_RYO{SHEN_NAME(RYO_)}, AREA_0547_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0547_BUTTERFLY{SHEN_NAME(bomT)}, AREA_0547_CONTROL{SHEN_NAME(tefc)}, AREA_0547_WAKE{SHEN_NAME(wake)};
inline constexpr shen_name AREA_0547_CHARACTERS[]{AREA_0547_SIN, shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0547_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01)};
inline constexpr shen_name AREA_0547_PROPS[]{SHEN_NAME(aa00), SHEN_NAME(a996), SHEN_NAME(a998), SHEN_NAME(aa02)};
inline constexpr shen_name AREA_0547_BUTTERFLY_MODELS[]{SHEN_NAME(FL00), SHEN_NAME(FL01), SHEN_NAME(FL02), SHEN_NAME(FL03), SHEN_NAME(FL04), SHEN_NAME(FL00),
    SHEN_NAME(FL00), SHEN_NAME(FL00), SHEN_NAME(FL01), SHEN_NAME(FL01), SHEN_NAME(FL01), SHEN_NAME(FL00), SHEN_NAME(FL00), SHEN_NAME(FL00), SHEN_NAME(FL00),
    SHEN_NAME(FL01)};
inline constexpr Vector3 AREA_0547_SPAWNS[]{{96.2229996f, 101.5f, 81.0123978f}, {104.333298f, 101.5f, 84.1619034f}, {96.686203f, 101.5f, 74.691597f},
    {94.4000015f, 101.5f, 66.2751999f}, {104.333298f, 101.5f, 84.1619034f}};
struct Area0547Parameter {
    HLib::Task *self;
    std::uint32_t motions[2];
    std::int32_t selection;
    std::int16_t state;
    std::byte reserved16[2];
};
struct Area0547AuthorParameter {
    Area0547Parameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
struct Area0547Butterfly {
    Vector3 position;
    Vector3i rotation;
    Vector3 velocity;
    std::byte reserved24[12];
    Vector3 acceleration;
    std::byte reserved3c[12];
    std::int32_t delay, initialDelay, state;
    std::int32_t animation;
    std::int32_t animationDelay;
    shen_name model;
    std::uint16_t flightPhase, flightReserved, flutterPhase, flutterReserved;
    float height;
    std::int32_t flightSpeedIndex, flutterSpeedIndex;
};
struct Area0547ButterflyParameter {
    Area0547Butterfly butterflies[1000];
};
static_assert(sizeof(Area0547Parameter) == 24 && sizeof(Area0547AuthorParameter) == 16);
static_assert(sizeof(Area0547Butterfly) == 116 && offsetof(Area0547Butterfly, state) == 80);
static_assert(sizeof(Area0547ButterflyParameter) == 0x1c520);
static_assert(offsetof(Area0547Parameter, selection) == 16 && offsetof(Area0547Parameter, state) == 20);
static_assert(offsetof(Area0547Butterfly, animation) == 84 && offsetof(Area0547Butterfly, flightPhase) == 96);
static_assert(offsetof(Area0547Butterfly, flutterPhase) == 100 && offsetof(Area0547Butterfly, height) == 104);
void Initialize0547();
void Finish0547AuthorNoop();
void Start0547(std::uintptr_t argument);
void Update0547(void *data);
void Queue0547Author(Area0547Parameter *parent);
void Update0547Author(void *data);
void Initialize0547Butterflies(void *data);
void Update0547Butterflies(void *data);
void Update0547ButterflyControl(void *data);
void Update0547Wake(void *data);
void Finish0547Loading(void *data);
void Setup0547Environment();
void Prepare0547Characters();
void Restore0547Characters();
void Cleanup0547(Area0547Parameter *parent);
extern const ScriptAreaDefinition area0547ScriptDefinition;
} // namespace shenmue::s2::areas
