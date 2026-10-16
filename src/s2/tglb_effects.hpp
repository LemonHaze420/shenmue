#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct TogyuEventWork;
inline constexpr shen_name togyuSmokeTaskToken{SHEN_NAME(SMOK)};
inline constexpr shen_name togyuBattleVoiceTaskToken{SHEN_NAME(EVNT)};

struct TogyuSmokeControlWork {
    std::uint32_t mode;
    std::uint32_t reserved04;
    TogyuEventWork *event;
};
struct TogyuBattleVoiceWork {
    std::int32_t phase;
    std::int32_t cueGroup;
    std::int32_t waitFrames;
};
static_assert(sizeof(TogyuSmokeControlWork) == 0x10);
static_assert(offsetof(TogyuSmokeControlWork, event) == 8);
static_assert(sizeof(TogyuBattleVoiceWork) == 0x0c);

inline constexpr Vector3 togyuSmokePosition{248.0f, 244.0f, 134.0f};
inline constexpr Vector3 togyuSmokeVolume{2.0f, 0.1f, 1.5f};
inline constexpr Vector3 togyuSmokeVelocity{0.3f, 0.01f, 0.0f};
inline constexpr Vector3 togyuSmokeVelocityVariance{0.0f, 0.005f, 0.0f};
inline constexpr Vector3 victorySmokePosition{258.02f, 244.01f, 135.59f};
inline constexpr Vector3 victorySmokeVolume{};
inline constexpr Vector3 victorySmokeRiseVelocity{0.0f, 0.003f, 0.0f};
inline constexpr Vector3 victorySmokeSlowVelocity{0.0f, 0.001f, 0.0f};
inline constexpr Vector3 victorySmokeVerticalVariance{0.0f, 0.002f, 0.0f};
inline constexpr Vector3 victorySmokeVelocityVariance{0.07f, 0.003f, 0.07f};
inline constexpr std::int32_t togyuSmokeCount = 100;
inline constexpr std::int32_t togyuSmokeProfile = 3;
inline constexpr float togyuSmokeInitialBirthRate = 0.001f;
inline constexpr shen_name togyuSmokeFlags{};
inline constexpr std::int32_t togyuSmokeFadeFactor = 16, togyuSmokeFalloff = 96;
inline constexpr std::int32_t victorySmokeFadeFactor = 64, victorySmokeFalloff = 80;
inline constexpr float togyuSmokeInitialScale = 0.9f, togyuSmokeSize = 1.0f, togyuSmokeBirthRate = 0.03f;
inline constexpr float togyuSmokeSpreadFalloff = 55.86399841308594f, togyuSmokeSpreadOpacity = 1.058823585510254f;
inline constexpr float togyuSmokeRadialVelocity = 0.772549033164978f, togyuSmokeRadialDecay = 0.46666666865348816f;
inline constexpr float victorySmokeSize = 0.7f, victorySmokeBirthRate = 0.9f;
inline constexpr float victorySmokeSpreadFalloff = 0.20999999344348907f, victorySmokeSpreadOpacity = 0.800000011920929f;
inline constexpr std::int32_t togyuBattleVoiceDelay = 150, togyuBattleVoiceAdmissionDelay = 10;
inline constexpr std::array<std::array<std::uint16_t, 4>, 3> togyuBattleVoiceCues{{{216, 217, 218, 219}, {220, 221, 222, 223}, {224, 225, 226, 227}}};
inline constexpr std::array<std::array<std::uint16_t, 3>, 3> togyuLowHealthVoiceCues{{{228, 229, 230}, {231, 232, 233}, {234, 235, 236}}};
inline constexpr std::array<std::uint16_t, 9> togyuHitVoiceCues{238, 241, 245, 239, 243, 247, 240, 244, 248};
inline constexpr std::array<std::uint16_t, 3> togyuMotionVoiceCues{237, 242, 246};

extern HLib::Task *togyuSmokeTask;
extern HLib::Task *togyuBattleVoiceTask;
extern std::int32_t togyuVictorySmokeId;
extern std::int32_t togyuMotionVoiceIndex;

HLib::Task *QueueTogyuSmokeControl(TogyuEventWork *event);
HLib::Task *QueueTogyuBattleVoiceControl();
std::int32_t CreateTogyuSmoke();
void SuspendTogyuSmoke(TogyuEventWork *event, float unused);
void UpdateTogyuSmoke(TogyuEventWork *event);
std::int32_t CreateTogyuVictorySmoke();
std::int32_t ConfigureTogyuVictorySmoke();
void TogyuSmokeControlCallback(void *taskData);
void TogyuBattleVoiceControlCallback(void *taskData);
std::int64_t SelectTogyuHitVoice(void *callbackArgument);
std::int64_t SelectTogyuMotionVoice(void *callbackArgument);

} // namespace shenmue::s2
