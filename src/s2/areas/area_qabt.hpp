#pragma once
#include "s2/areas/area_arbt.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name QABT_AREA{SHEN_NAME(QABT)}, QABT_AW2{SHEN_NAME(AW2_)}, QABT_RNP{SHEN_NAME(RNP_)};
inline constexpr shen_name QABT_CHARACTERS[]{ARBT_RYO, QABT_AW2, QABT_RNP, ARBT_KNT, ARBT_AA1, ARBT_LYO, ARBT_HAK, ARBT_EIN, EVENT_CHARACTER_INVALID};
struct QabtDescriptor {
    std::int32_t mode, shift;
};
inline constexpr QabtDescriptor QABT_DESCRIPTORS[]{{.mode = 0, .shift = 0}, {.mode = 0, .shift = 1}, {.mode = 0, .shift = 2}, {.mode = 0, .shift = 3},
    {.mode = 1, .shift = 0}, {.mode = 1, .shift = 1}, {.mode = 1, .shift = 2}, {.mode = 1, .shift = 3}, {.mode = 2, .shift = 0}};
struct QabtCarryWork {
    HLib::TaskCallback phase;
    std::int32_t success, frames;
    std::uint32_t direction;
    std::int32_t presses, intensity;
    float speed;
    std::int32_t forwardSteps, turns, failures, consecutiveFailures, idleCount;
    std::uint32_t reserved34;
    const ArbtRouteStep *route;
};
struct QabtWork {
    EventReadCharactersWork *characters;
    QabtCarryWork carry;
    ArbtCrateWork crates;
    std::uint64_t reserved170;
    HLib::Task *self;
    std::uint32_t reserved180;
    std::int32_t firstJob;
    DateTime time;
    std::byte reserved18f[3];
    DateTime endTime;
    std::byte reserved199[3];
    std::int32_t delivered, failures, idleWarnings, perfectStreak;
    std::uint32_t reserved1ac;
    const QabtDescriptor *descriptor;
    std::int32_t authorFrames, payEndFrame, payFrame, cash;
};
static_assert(sizeof(QabtCarryWork) == 0x40 && offsetof(QabtCarryWork, route) == 0x38);
static_assert(sizeof(QabtWork) == 0x1c8 && offsetof(QabtWork, crates) == 0x48 && offsetof(QabtWork, self) == 0x178);
static_assert(offsetof(QabtWork, endTime) == 0x192 && offsetof(QabtWork, descriptor) == 0x1b0);
static_assert(offsetof(QabtWork, payEndFrame) == 0x1bc && offsetof(QabtWork, payFrame) == 0x1c0 && offsetof(QabtWork, cash) == 0x1c4);
extern Vector3 qabtCameraOffset, qabtDestinationOffset;
extern Vector3i qabtCameraRotation, qabtCrateRotation, qabtDestinationRotation;
void InitializeQabtEvent();
void RunQabtEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition qabtScriptDefinition;
void QabtCarryTurnInit(void *);
std::int32_t SelectNextQabtCrate(ArbtCrateWork &);
void CreateQabtCrates(ArbtCrateWork &);
void SetInitialQabtCarrySettings(QabtCarryWork &, std::int32_t crate);
std::int32_t QabtIntroduceJob();
std::int32_t QabtExplainFirstCarry();
std::int32_t QabtExplainCarry();
std::int32_t QabtIntroduceNextCrate();
std::int32_t QabtStartWork(std::int32_t delay);
std::int32_t QabtLiftHighCrate(ArbtCrateWork &);
std::int32_t QabtLiftLowCrate(ArbtCrateWork &);
std::int32_t QabtPutHighCrate(ArbtCrateWork &, std::int32_t streak, shen_name destination);
std::int32_t QabtPutLowCrate(ArbtCrateWork &, std::int32_t streak, shen_name destination);
std::int32_t QabtPraiseCarry(std::int32_t streak, std::int32_t delay);
std::int32_t QabtWarnCarryOnce();
std::int32_t QabtWarnCarryTwice();
std::int32_t QabtWarnCarryThird();
std::int32_t QabtWarnCarryFourth();
std::int32_t QabtWarnCarryRepeatedly();
std::int32_t QabtWarnWrongForward();
std::int32_t QabtWarnWrongDouble(std::uint32_t direction);
std::int32_t QabtWarnWrongMultiple();
std::int32_t QabtWarnIdleForward(std::int32_t count);
std::int32_t QabtWarnIdlePut(std::int32_t count);
std::int32_t QabtWalkForward(float speed, std::int32_t delay);
std::int32_t QabtQueueDirection(std::uint32_t direction, std::int32_t intensity, std::int32_t delay);
std::int32_t QabtWalkDirection(float speed, std::int32_t intensity, std::uint32_t direction);
std::int32_t QabtFailCarry(ArbtCrateWork &, shen_name crate, std::int32_t intensity, std::uint32_t direction);
std::int32_t QabtAnnounceTimeOver();
std::int32_t QabtPayFirstJob(std::int32_t cash, std::int32_t crates);
std::int32_t QabtPayGoodJob(std::int32_t cash, std::int32_t crates);
std::int32_t QabtPayAverageJob(std::int32_t cash, std::int32_t crates);
std::int32_t QabtPayPoorJob(std::int32_t cash, std::int32_t crates);
} // namespace shenmue::s2::areas
