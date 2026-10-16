#pragma once
#include "s2/areas/qes_event.hpp"
namespace shenmue::s2 {
extern SoundEffectHandle *qes2SoundHandle;
extern float qes2SoundIntensity, qes2FloorHeight;
extern std::int16_t qes2EntryMode, qes2FirstPairedPhase, qes2SecondPairedPhase;
extern shen_name qes2FirstPairedCharacters[3], qes2SecondPairedCharacters[3];
inline constexpr Vector3 QES2_PAIR_FIRST_POSITION{-2.299999952316284f, 0, -2.700000047683716f};
inline constexpr Vector3 QES2_PAIR_SECOND_POSITION{-2.9000000953674316f, 0, -1.2000000476837158f};
inline constexpr Vector3 QES2_PAIR_COUNTER_POSITION{1.5f, 0, 0.800000011920929f};
inline constexpr Vector3 QES2_PAIR_DOUBLE_FAILURE_POSITION{0.20000000298023224f, 0, -1.899999976158142f};
inline constexpr Vector3 QES2_PAIR_SPLIT_POSITIONS[]{{0.30000001192092896f, 0, -1.7999999523162842f}, {0, 0, -2.700000047683716f}};
inline constexpr Vector3 QES2_PAIR_SECOND_FAILURE_POSITION{-0.20000000298023224f, 0, -2.4000000953674316f};
inline constexpr Vector3 QES2_PAIR_COUNTER_FAILURE_POSITION{0.800000011920929f, 0, -1.5f};
inline constexpr Vector3 QES2_SECOND_PAIR_FIRST_POSITION{1.899999976158142f, 0, -2.200000047683716f};
inline constexpr Vector3 QES2_SECOND_PAIR_WAIT_POSITION{-1.2000000476837158f, 0, 1.2999999523162842f};
inline constexpr Vector3 QES2_SECOND_PAIR_SPLIT_POSITION{-0.20000000298023224f, 0, -3.4000000953674316f};
inline constexpr Vector3 QES2_SECOND_PAIR_DOUBLE_FAILURE_POSITION{0.4000000059604645f, 0, -1.7000000476837158f};
inline constexpr Vector3 QES2_SECOND_PAIR_RECOVERY_POSITION{1.100000023841858f, 0, -1.5f};
inline constexpr Vector3 QES2_SECOND_PAIR_FAILURE_POSITION{-0.30000001192092896f, 0, -1.7000000476837158f};
inline constexpr Vector3 QES2_SECOND_PAIR_COUNTER_FAILURE_POSITION{-0.20000000298023224f, 0, -1.7999999523162842f};
void Qes2SoundIntensityCallback(void *);
void InitializeQes2FirstPairedSequence(float x, float z, float degrees);
std::int32_t UpdateQes2FirstPairedSequence(std::int32_t *result);
void InitializeQes2SecondPairedSequence(float x, float z, float degrees);
std::int32_t UpdateQes2SecondPairedSequence(std::int32_t *result);
} // namespace shenmue::s2
