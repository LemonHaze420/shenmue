#pragma once
#include "s2/areas/qes_event.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QES3_AREA{SHEN_NAME(QES3)}, QES3_DUMMY{SHEN_NAME(DDUM)}, QES3_POLE{SHEN_NAME(a073)}, QES3_KT8{SHEN_NAME(KT8_)};
inline constexpr shen_name QES3_ELEVATOR{SHEN_NAME(R596)}, QES3_QTE_AUTHOR{SHEN_NAME(QTE_)}, QES3_FIGHT_AUTHOR{SHEN_NAME(BTL_)};
inline constexpr shen_name QES3_END_AUTHOR{SHEN_NAME(END_)}, QES3_RETRY_TITLE{SHEN_NAME(RTR2)}, QES3_FIGHT_LIGHT{SHEN_NAME(l001)};
inline constexpr shen_name QES3_READ_CHARACTERS[]{QES_RYO, QES_REN, QES3_KT8, QES_YUA, QES_INVALID};
inline constexpr shen_name QES3_CLEANUP_CHARACTERS[]{SHEN_NAME(KT2_), SHEN_NAME(AD1_), SHEN_NAME(KTA_), QES_INVALID};
inline constexpr Vector3 QES3_CHASE_RYO{-1.7000000476837158f, 0, -2.4000000953674316f}, QES3_CHASE_REN{-3.799999952316284f, 0, -1.600000023841858f};
inline constexpr Vector3 QES3_CHASE_YUA{12.300000190734863f, 0, 1.100000023841858f}, QES3_CHASE_KT8{-4.099999904632568f, 0, 22.600000381469727f};
inline constexpr Vector3 QES3_POLE_RYO{-0.20000000298023224f, 0, -12}, QES3_POLE_REN{0.20000000298023224f, 0, -9};
inline constexpr Vector3 QES3_POLE_KT8{-1.399999976158142f, 0, 1.7000000476837158f};
inline constexpr Vector3 QES3_FIRST_CHAIN_SUCCESS{0.25999999046325684f, 0, 1.940000057220459f};
inline constexpr Vector3 QES3_SECOND_CHAIN_SUCCESS{0.09000000357627869f, 0, 1.5499999523162842f};
inline constexpr Vector3 QES3_CHAIN_FAILURES[]{
    {0.12999999523162842f, 0, -0.4699999988079071f}, {0.019999999552965164f, 0, -0.8700000047683716f}, {-0.2199999988079071f, 0, -0.8799999952316284f}};

struct Qes3EventWork {
    HLib::Task *task;
    std::int16_t phase, subphase, nextSubphase, reserved0e;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed, reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded, timer, reserved2c;
};
struct Qes3SparkWork {
    std::int16_t index, reserved02;
    Vector3 offset;
    std::int32_t frames, retireFrames;
};
struct Qes3SparkBank {
    std::int16_t primary[4];
    Vector3 previous[4];
    std::int16_t secondary[4];
    float rates[4];
};
static_assert(sizeof(Qes3EventWork) == 0x30 && offsetof(Qes3EventWork, characters) == 0x10);
static_assert(offsetof(Qes3EventWork, qteElapsed) == 0x18 && offsetof(Qes3EventWork, qteStatus) == 0x20);
static_assert(offsetof(Qes3EventWork, qteSucceeded) == 0x24 && offsetof(Qes3EventWork, timer) == 0x28);
static_assert(sizeof(Qes3SparkWork) == 0x18 && offsetof(Qes3SparkWork, frames) == 0x10);
static_assert(sizeof(Qes3SparkBank) == 0x50 && offsetof(Qes3SparkBank, previous) == 8);
static_assert(offsetof(Qes3SparkBank, secondary) == 0x38 && offsetof(Qes3SparkBank, rates) == 0x40);
extern std::int32_t qes3MotionHandle, qes3RetryCanSkip;
extern float qes3FloorHeight, qes3SoundIntensity;
extern SoundEffectHandle *qes3SoundHandle;
extern Qes3SparkBank qes3Sparks;
void Qes3Init();
void Qes3Main(std::uintptr_t);
void Qes3Cleanup(void *);
extern const ScriptAreaDefinition qes3Definition;
} // namespace shenmue::s2
