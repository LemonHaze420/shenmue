#pragma once
#include "shenmue.h"
#define SHENMUE_MODS_ENABLED (SHENMUE_IS_PC && (SHENMUE_IS_S1 || SHENMUE_IS_S2))
#if SHENMUE_MODS_ENABLED
#include <array>
#include <cstdint>
#include <span>
#include "common/math.hpp"

struct cJSON;
namespace Json { class Value; }
namespace shenmue::mods {
enum class Id : std::uint8_t {
    FrameRate, SkyMovement, EnemyGauge, Cassettes, Crouch, DrawDistance, Widescreen, FirstPerson, PhotoMode,
    HideHud, FreezeTime, TimeSpeed, NoBedtime, DisableWeather, RainDensity, RainOpacity, CloudLevel, RainLevel, SnowLevel,
    SkipSplash, SkipCutscenes, QteSuccess, InfiniteStamina, FullShadows, SunLatitude, CrateArrows, CameraDistance, MapSeams, WeatherOverride, Count
};
enum class Kind : std::uint8_t { Toggle, List, Slider };
struct Option {
    Id id;
    Kind kind;
    const char *key;
    const char *label;
    const char *description;
    float initial;
    float minimum;
    float maximum;
};
std::span<const Option> Options();
const Option &Spec(Id id);
float Get(Id id);
bool Enabled(Id id);
void Set(Id id, float value);
const char *ChoiceLabel(Id id, std::int32_t index);
std::int32_t ChoiceValue(Id id, std::int32_t index);
void Reset();
void Read(const Json::Value &settings);
void Write(Json::Value &settings);
void ReadCJson(const cJSON *settings);
cJSON *WriteCJson();
bool Changed();
void Record();
void Revert();
void Defaults();
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_CAMERA
using CameraKey = bool (*)(std::uint32_t key);
bool AdjustCamera(bool allowed, float seconds, std::uint32_t area, CameraKey key, Vector3 &eye, Vector3 &target);
void ResetCamera();
bool FreeCameraActive();
bool FirstPersonActive();
#endif
}
#endif
