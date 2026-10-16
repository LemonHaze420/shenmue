#pragma once

#include "s1/input.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/char.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::uint32_t kFBGGComponent = SHEN_NAME(VEHI);

#pragma pack(push, 4)

struct FBGGRowSample {
    Vector3 left;
    Vector3 right;
};


struct FBGGRowHeightRange {
    float baseHeight;
    std::int32_t start;
    std::int32_t end;
};




struct FBGGWork {
    std::uint32_t riderEngaged;
    std::uint32_t reserved004;
    std::int32_t speedTier;
    std::uint32_t reserved00c;
    std::uint64_t reserved010;
    std::uint32_t reserved018;
    Vector3 modelNode22Position;
    Vector3 modelNode17Position;
    Vector3 modelNode15Position;
    const FBGGRowSample *rows;
    std::int32_t engineSoundBank;
    std::int32_t brakeSoundBank;
    std::int32_t driftSoundBank;
    std::int32_t edgeScrapeSoundBank;
    std::int32_t edgeScrapeLateSoundBank;
    std::uint32_t previousSteeringInput;
    std::uint32_t initializeRequested;
    std::uint32_t exitRequested;
    std::uint32_t gaugeDecayTier;
    Vector3 lienProxyPosition;
    Vector3i lienProxyRotation;
    float gaugeDecay;
    std::int32_t rowIndex;
    float steeringAngle;
    float steeringVelocity;
    float previousSteeringTarget;
    std::int32_t driftAccumulator;
    std::int32_t weaveAccumulator;
    Vector3 roadEdgeContactPosition;
    std::uint32_t roadEdgeContact;
    Vector3 shadowProxyPosition;
    Vector3i shadowProxyRotation;
    std::int32_t roadEdgeContactDebounce;
    std::uint32_t reserved0cc;
};

#pragma pack(pop)

static_assert(sizeof(FBGGRowSample) == 0x18);
static_assert(sizeof(FBGGRowHeightRange) == 0x0c);
static_assert(sizeof(FBGGWork) == 0xd0);
static_assert(offsetof(FBGGWork, speedTier) == 0x08);
static_assert(offsetof(FBGGWork, modelNode22Position) == 0x1c);
static_assert(offsetof(FBGGWork, modelNode17Position) == 0x28);
static_assert(offsetof(FBGGWork, modelNode15Position) == 0x34);
static_assert(offsetof(FBGGWork, rows) == 0x40);
static_assert(offsetof(FBGGWork, engineSoundBank) == 0x48);
static_assert(offsetof(FBGGWork, edgeScrapeLateSoundBank) == 0x58);
static_assert(offsetof(FBGGWork, previousSteeringInput) == 0x5c);
static_assert(offsetof(FBGGWork, initializeRequested) == 0x60);
static_assert(offsetof(FBGGWork, exitRequested) == 0x64);
static_assert(offsetof(FBGGWork, gaugeDecayTier) == 0x68);
static_assert(offsetof(FBGGWork, lienProxyPosition) == 0x6c);
static_assert(offsetof(FBGGWork, lienProxyRotation) == 0x78);
static_assert(offsetof(FBGGWork, gaugeDecay) == 0x84);
static_assert(offsetof(FBGGWork, rowIndex) == 0x88);
static_assert(offsetof(FBGGWork, steeringAngle) == 0x8c);
static_assert(offsetof(FBGGWork, steeringVelocity) == 0x90);
static_assert(offsetof(FBGGWork, previousSteeringTarget) == 0x94);
static_assert(offsetof(FBGGWork, driftAccumulator) == 0x98);
static_assert(offsetof(FBGGWork, weaveAccumulator) == 0x9c);
static_assert(offsetof(FBGGWork, roadEdgeContactPosition) == 0xa0);
static_assert(offsetof(FBGGWork, roadEdgeContact) == 0xac);
static_assert(offsetof(FBGGWork, shadowProxyPosition) == 0xb0);
static_assert(offsetof(FBGGWork, shadowProxyRotation) == 0xbc);
static_assert(offsetof(FBGGWork, roadEdgeContactDebounce) == 0xc8);



extern Character *g_pTempFBGGChara;
extern std::int32_t g_fbggRiderMotionState;
extern float g_fbggGauge;
extern std::int32_t g_fbggRideHeading;
extern std::int32_t g_motnCameraFollowAngle;
extern std::uint32_t g_vehicleRideFrameCount;
extern Vector3 g_vehicleRightAnkleTarget;
extern Vector3 g_vehicleLeftAnkleTarget;

FBGGWork *FBGG_GetWork(Character *character);
void LoadAndSetupFBGG(Character *character);
void FBGG_Callback(Character *character, void *rawWork);
void ryoUpdateFollower(Character *rider, const InputSlot *input);
std::int32_t BuildMOTNNodeWorldTransforms(Mt5RuntimeNode *root, const Vector3 &translation, const Vector3i &euler, const Vector3 &offset, float pitchDegrees);

} // namespace shenmue::s1
