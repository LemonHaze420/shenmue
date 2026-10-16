#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name ARBT_AREA{SHEN_NAME(ARBT)}, ARBT_BASE{SHEN_NAME(BASE)}, ARBT_RYO{SHEN_NAME(RYO_)}, ARBT_TKR{SHEN_NAME(TKR_)};
inline constexpr shen_name ARBT_QLN{SHEN_NAME(QLN_)}, ARBT_KNT{SHEN_NAME(KNT_)}, ARBT_AA1{SHEN_NAME(AA1_)}, ARBT_LYO{SHEN_NAME(LYO_)};
inline constexpr shen_name ARBT_HAK{SHEN_NAME(HAK_)}, ARBT_EIN{SHEN_NAME(EIN_)}, ARBT_PAY{SHEN_NAME(PAY_)};
inline constexpr shen_name ARBT_DUMA{SHEN_NAME(DUMA)}, ARBT_DUMT{SHEN_NAME(DUMT)}, ARBT_DUMS{SHEN_NAME(DUMS)};
inline constexpr shen_name ARBT_DUMB{SHEN_NAME(DUMB)}, ARBT_DUMN{SHEN_NAME(DUMN)}, ARBT_EG0R{SHEN_NAME(EG0R)}, ARBT_EG0L{SHEN_NAME(EG0L)};
inline constexpr shen_name ARBT_EG1R{SHEN_NAME(EG1R)}, ARBT_EG1L{SHEN_NAME(EG1L)}, ARBT_CRATE_FIRST{SHEN_NAME(ABBA)}, ARBT_PALLET_FIRST{SHEN_NAME(ABNA)};
inline constexpr shen_name ARBT_PALLET_SECOND_ROW{SHEN_NAME(ABNE)}, ARBT_AK00{SHEN_NAME(AK00)}, ARBT_Q100{SHEN_NAME(Q100)};
inline constexpr shen_name ARBT_LIGHT_QCAE{SHEN_NAME(QCAE)}, ARBT_LIGHT_AKA3{SHEN_NAME(AKA3)}, ARBT_LIGHT_AKB0{SHEN_NAME(AKB0)};
inline constexpr shen_name ARBT_CHARACTERS[]{ARBT_RYO, ARBT_TKR, ARBT_QLN, ARBT_KNT, ARBT_AA1, ARBT_LYO, ARBT_HAK, ARBT_EIN, EVENT_CHARACTER_INVALID};
inline constexpr shen_name ARBT_PAY_CROWD[]{ARBT_KNT, ARBT_AA1, ARBT_LYO, ARBT_HAK, ARBT_EIN};
inline constexpr shen_name ARBT_PROP_R024{SHEN_NAME(R024)}, ARBT_PROP_R005{SHEN_NAME(R005)}, ARBT_PROP_R018{SHEN_NAME(R018)};
inline constexpr shen_name ARBT_PROP_R007{SHEN_NAME(R007)}, ARBT_PROP_R010{SHEN_NAME(R010)}, ARBT_PROP_R014{SHEN_NAME(R014)};
inline constexpr shen_name ARBT_PROP_R013{SHEN_NAME(R013)}, ARBT_PROP_R012{SHEN_NAME(R012)}, ARBT_PROP_R030{SHEN_NAME(R030)};
inline constexpr shen_name ARBT_HIDDEN_PROPS[]{
    ARBT_PROP_R024, ARBT_PROP_R005, ARBT_PROP_R018, ARBT_PROP_R007, ARBT_PROP_R010, ARBT_PROP_R014, ARBT_PROP_R013, ARBT_PROP_R012, ARBT_PROP_R030};
struct ArbtDescriptor {
    std::int32_t mode, shift;
};
inline constexpr ArbtDescriptor ARBT_DESCRIPTORS[]{{.mode = 0, .shift = 0}, {.mode = 0, .shift = 1}, {.mode = 0, .shift = 2}, {.mode = 0, .shift = 3},
    {.mode = 1, .shift = 0}, {.mode = 1, .shift = 1}, {.mode = 1, .shift = 2}, {.mode = 1, .shift = 3}, {.mode = 2, .shift = 0}};
struct ArbtRouteStep {
    std::int16_t intensity, forwardSteps;
    float frames;
    std::uint32_t direction;
};
static_assert(sizeof(ArbtRouteStep) == 12);
inline constexpr float ARBT_CARRY_SPEEDS[]{2.3f, 2.3f, 2.3f, 2.4f, 2.4f, 2.4f};
inline constexpr std::int8_t ARBT_PRESS_COUNTS[]{1, 1, 1, 2, 3, 4, 1, 1};
extern const ArbtRouteStep arbtCarryRoutes[54][16];
struct ArbtCarryWork {
    HLib::TaskCallback phase;
    std::int32_t success, frames;
    std::uint32_t direction;
    std::int32_t presses, intensity;
    float speed;
    std::int32_t forwardSteps, turns, failures, consecutiveFailures, idleCount;
    std::uint32_t reserved34;
    const ArbtRouteStep *route;
};
struct ArbtCrateWork {
    HLib::Task *moveTask;
    std::int32_t moveFrames;
    std::uint32_t reserved0c;
    HLib::Task *dropTask;
    float dropHeight;
    std::int32_t index, limit;
    shen_name crates[16], pallets[16], destinations[16];
    std::int32_t stacked[16];
    std::uint32_t reserved124;
};
struct ArbtWork {
    EventReadCharactersWork *characters;
    ArbtCarryWork carry;
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
    const ArbtDescriptor *descriptor;
    std::int32_t authorFrames, cash, payFrame, payEndFrame;
};
static_assert(sizeof(ArbtCarryWork) == 0x40 && offsetof(ArbtCarryWork, route) == 0x38);
static_assert(sizeof(ArbtCrateWork) == 0x128 && offsetof(ArbtCrateWork, stacked) == 0xe4);
static_assert(sizeof(ArbtWork) == 0x1c8 && offsetof(ArbtWork, crates) == 0x48 && offsetof(ArbtWork, self) == 0x178);
static_assert(offsetof(ArbtWork, endTime) == 0x192 && offsetof(ArbtWork, descriptor) == 0x1b0 && offsetof(ArbtWork, payEndFrame) == 0x1c4);
extern Vector3 arbtCameraOffset, arbtDestinationOffset;
struct ArbtCameraRotationState {
    Vector3i angles;
    std::int32_t yaw;
};
static_assert(sizeof(ArbtCameraRotationState) == 16 && offsetof(ArbtCameraRotationState, yaw) == 12);
extern ArbtCameraRotationState arbtCameraRotation;
extern ArbtCameraRotationState arbtCrateRotation, arbtDestinationRotation;
void InitializeArbtEvent();
void RunArbtEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition arbtScriptDefinition;
void ArbtCarryTurnInit(void *);
void ArbtMoveBox(void *);
void ArbtDropBox(void *);
void ArbtTrackLiftHand(void *);
void QueueArbtDropBox(ArbtCrateWork &, std::int32_t delay);
std::int32_t SelectNextArbtCrate(ArbtCrateWork &);
void CreateArbtCrates(ArbtCrateWork &);
void SetInitialArbtCarrySettings(ArbtCarryWork &, std::int32_t crate);
std::int32_t ArbtIntroduceJob();
std::int32_t ArbtExplainFirstCarry();
std::int32_t ArbtExplainCarry();
std::int32_t ArbtIntroduceNextCrate();
std::int32_t ArbtStartWork(std::int32_t delay);
std::int32_t ArbtLiftHighCrate(ArbtCrateWork &);
std::int32_t ArbtLiftLowCrate(ArbtCrateWork &);
std::int32_t ArbtPutHighCrate(ArbtCrateWork &, std::int32_t streak, shen_name destination);
std::int32_t ArbtPutLowCrate(ArbtCrateWork &, std::int32_t streak, shen_name destination);
std::int32_t ArbtPraiseCarry(std::int32_t streak, std::int32_t delay);
std::int32_t ArbtWarnCarryOnce();
std::int32_t ArbtWarnCarryTwice();
std::int32_t ArbtWarnCarryThird();
std::int32_t ArbtWarnCarryFourth();
std::int32_t ArbtWarnCarryRepeatedly();
std::int32_t ArbtWarnWrongForward();
std::int32_t ArbtWarnWrongDouble(std::uint32_t direction);
std::int32_t ArbtWarnWrongMultiple();
std::int32_t ArbtWarnIdleForward(std::int32_t count);
std::int32_t ArbtWarnIdlePut(std::int32_t count);
std::int32_t ArbtWalkForward(float speed, std::int32_t delay);
std::int32_t ArbtQueueDirection(std::uint32_t direction, std::int32_t intensity, std::int32_t delay);
std::int32_t ArbtWalkDirection(float speed, std::int32_t intensity, std::uint32_t direction);
std::int32_t ArbtFailCarry(ArbtCrateWork &, shen_name crate, std::int32_t intensity, std::uint32_t direction);
std::int32_t ArbtAnnounceTimeOver();
std::int32_t ArbtPayFirstJob(std::int32_t cash, std::int32_t crates);
std::int32_t ArbtPayGoodJob(std::int32_t cash, std::int32_t crates);
std::int32_t ArbtPayAverageJob(std::int32_t cash, std::int32_t crates);
std::int32_t ArbtPayPoorJob(std::int32_t cash, std::int32_t crates);
} // namespace shenmue::s2::areas
