#pragma once
#include "s2/area_event.hpp"
#include "s2/event_movement.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct SoundEffectHandle;
inline constexpr shen_name QES_TASK{SHEN_NAME(EVNT)}, QES_INVALID{0xffffffffU};
inline constexpr shen_name QES_RYO{SHEN_NAME(RYO_)}, QES_REN{SHEN_NAME(REN_)}, QES_YUA{SHEN_NAME(YUA_)}, QES_CHAINSAW{SHEN_NAME(a392)};
inline constexpr shen_name QES_BASE{SHEN_NAME(BASE)}, QES_DUMMY{SHEN_NAME(DUM_)}, QES_BOX{SHEN_NAME(BOX_)}, QES_CEILING{SHEN_NAME(CEIL)};
inline constexpr shen_name QES_FLOORBOARD{SHEN_NAME(FLBD)}, QES_KK6{SHEN_NAME(KK6_)}, QES_KU1{SHEN_NAME(KU1_)}, QES_KN3{SHEN_NAME(KN3_)};
inline constexpr shen_name QES_WTC{SHEN_NAME(WTC_)}, QES_THG{SHEN_NAME(THG_)}, QES_SOY{SHEN_NAME(SOY_)}, QES_KT3{SHEN_NAME(KT3_)};
inline constexpr shen_name QES_KT5{SHEN_NAME(KT5_)}, QES_CJU{SHEN_NAME(CJU_)}, QES_CAMERA_WTC{SHEN_NAME(damy)}, QES_CAMERA_THG{SHEN_NAME(damB)};
inline constexpr shen_name QES_START_AUTHOR{SHEN_NAME(STRT)};
inline constexpr shen_name QES1_AREA{SHEN_NAME(QES1)};
inline constexpr shen_name QES1_READ_CHARACTERS[]{QES_RYO, QES_REN, QES_YUA, QES_INVALID};
inline constexpr shen_name QES1_KICK_CHARACTERS[]{QES_KK6, QES_KU1, QES_INVALID};
inline constexpr shen_name QES1_PAIR_CHARACTERS[]{QES_KN3, QES_THG, QES_WTC, QES_INVALID};
inline constexpr shen_name QES1_PURSUERS[]{QES_SOY, QES_KT3, QES_KT5, QES_CJU, QES_CJU, QES_INVALID};
inline constexpr EventMovementMotions QES1_MOVEMENT_MOTIONS{
    .defaultMotion = 0, .forwardMotion = 0, .reserved08 = 7879, .reserved0c = 0, .backwardMotion = 7879, .stopMotion = 0};
inline constexpr EventMovementWaypoint QES1_REN_ROUTE[]{{{217.69999694824219f, 100, 126.5f}, 270}, {{-1, -1, -1}, 0}};
inline constexpr EventMovementWaypoint QES1_RYO_PAIR_ROUTE[]{{{247.3000030517578f, 100, 126.80000305175781f}, 180}, {{-1, -1, -1}, 0}};
inline constexpr EventMovementWaypoint QES1_RYO_EXIT_ROUTE[]{{{251.39999389648438f, 100, 140.39999389648438f}, 270}, {{-1, -1, -1}, 0}};
inline constexpr std::int32_t QES1_CEILING_PARTS[]{196, 165, 160, 153, 162, 157, 164, 159, 152, 156, 163, 160, 195, 155, 166, 161, 154, 184, 185, 186, 193, 194,
    190, 191, 192, 187, 188, 189, 181, 182, 183, 170, 171, 172, 179, 180, 176, 177, 178, 173, 174, 175, 167, 168, 169};
inline constexpr Vector3 QES_PAIR_FIRST_POSITION{2.299999952316284f, 0, -2.700000047683716f};
inline constexpr Vector3 QES_PAIR_SECOND_POSITION{2.9000000953674316f, 0, -1.2000000476837158f};
inline constexpr Vector3 QES_PAIR_COUNTER_POSITION{-1.5f, 0, 0.800000011920929f};
inline constexpr Vector3 QES_PAIR_DOUBLE_FAILURE_POSITION{-0.20000000298023224f, 0, -1.899999976158142f};
inline constexpr Vector3 QES_PAIR_SPLIT_FAILURE_POSITIONS[]{{-0.30000001192092896f, 0, -1.7999999523162842f}, {0, 0, -2.700000047683716f}};
inline constexpr Vector3 QES_PAIR_SECOND_FAILURE_POSITION{0.20000000298023224f, 0, -2.4000000953674316f};
inline constexpr Vector3 QES_PAIR_COUNTER_FAILURE_POSITION{-0.800000011920929f, 0, -1.5f};
struct QesAngleWork {
    shen_name character;
    float degrees;
    std::int32_t frames;
};
struct QesSoundIntensityWork {
    std::int32_t initial, target, frames, phase;
    float step;
};
struct QesCameraWork {
    std::int32_t cleanup, camera;
    shen_name eye, target;
};
static_assert(sizeof(QesAngleWork) == 0x0c && sizeof(QesSoundIntensityWork) == 0x14 && sizeof(QesCameraWork) == 0x10);
extern SoundEffectHandle *qesSoundHandle;
extern float qesSoundIntensity, qesFloorHeight;
extern std::uint16_t qesMissedInputs;
extern shen_name qesPairedCharacters[3];
extern std::int16_t qesPairedPhase;
void QesAngleCallback(void *);
QesAngleWork *QueueQesAngle(shen_name character, float degrees, std::int32_t frames, std::int32_t delay);
void QesSoundIntensityCallback(void *);
void QesCameraCallback(void *);
void PrepareQesCharacter(shen_name character);
void QueueQesRetryFade(std::int32_t delay);
void InitializeQesPairedSequence(float x, float z, float degrees);
std::int32_t UpdateQesPairedSequence(std::int32_t *result);
} // namespace shenmue::s2
