#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0504_RETURN{SHEN_NAME(0505)};
inline constexpr shen_name AREA_0504_EVENT{SHEN_NAME(auth)};
inline constexpr shen_name AREA_0504_AUTHOR{SHEN_NAME(scwk)};
inline constexpr shen_name AREA_0504_WAVE{SHEN_NAME(wave)};
inline constexpr shen_name AREA_0504_SPLASH{SHEN_NAME(sibu)};
inline constexpr shen_name AREA_0504_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0504_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0504_BOAT{SHEN_NAME(BN0_)};
inline constexpr shen_name AREA_0504_PROP{SHEN_NAME(a015)};
inline constexpr shen_name AREA_0504_CHARACTERS[]{AREA_0504_BOAT, shen_name{0xffffffffU}};
struct Area0504Parameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
struct Area0504AuthorParameter {
    Area0504Parameter *parent;
};
struct Area0504SplashParameter {
    std::uint32_t effect;
    std::int16_t birthFrame, freeFrame;
};
static_assert(sizeof(Area0504Parameter) == 16 && sizeof(Area0504AuthorParameter) == 8 && sizeof(Area0504SplashParameter) == 8);
static_assert(offsetof(Area0504Parameter, selection) == 8 && offsetof(Area0504Parameter, state) == 10);
static_assert(offsetof(Area0504SplashParameter, birthFrame) == 4 && offsetof(Area0504SplashParameter, freeFrame) == 6);
void Initialize0504();
void Start0504(std::uintptr_t argument);
void Update0504(void *data);
void Setup0504Environment();
void Prepare0504Characters();
void Restore0504Characters();
void Update0504Author(void *data);
void Update0504Waves(void *data);
void Update0504CameraRideHeight();
void Update0504CharacterHeight(shen_name character, shen_name heightSource);
void Create0504Splash(shen_name character, std::uint32_t count, float size, std::uint32_t bone);
void Update0504Splash(void *data);
void Cleanup0504(Area0504Parameter *parent);
extern const ScriptAreaDefinition area0504ScriptDefinition;
} // namespace shenmue::s2::areas
