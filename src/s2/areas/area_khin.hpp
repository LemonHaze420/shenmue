#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name KHIN_AREA{SHEN_NAME(KHIN)}, KHIN_TASK{SHEN_NAME(EVNT)}, KHIN_RYO{SHEN_NAME(RYO_)}, KHIN_HAND{SHEN_NAME(TE_R)};
inline constexpr shen_name KHIN_INVALID{0xffffffffU}, KHIN_NONE{0};
inline constexpr shen_name KHIN_WE00{SHEN_NAME(WE00)}, KHIN_QAW1{SHEN_NAME(QAW1)}, KHIN_QJBT{SHEN_NAME(QJBT)}, KHIN_OMAK{SHEN_NAME(OMAK)};
struct KhinInput {
    std::int32_t unused;
    std::uint32_t selector;
};
struct KhinCharacterRow {
    shen_name character;
    std::uint32_t motion;
    const char *const *voices;
    shen_name firstItem, secondItem;
};
struct KhinWork {
    HLib::Task *task;
    HLib::Task *caller;
    const KhinCharacterRow *row;
    std::uint8_t state;
    std::byte reserved19[3];
    std::int32_t remaining;
    std::int8_t nextSelector;
    std::byte reserved21[7];
};
static_assert(sizeof(KhinInput) == 8 && offsetof(KhinInput, selector) == 4);
static_assert(sizeof(KhinCharacterRow) == 0x18 && offsetof(KhinCharacterRow, voices) == 8);
static_assert(sizeof(KhinWork) == 0x28 && offsetof(KhinWork, state) == 0x18 && offsetof(KhinWork, remaining) == 0x1c);
static_assert(offsetof(KhinWork, nextSelector) == 0x20);
extern std::uint32_t khinSelector;
void InitializeKhinEvent();
void RunKhinEvent(std::uintptr_t argument);
void KhinPurchaseCallback(void *data);
void KhinPurchaseExitCallback(void *data);
void KhinSlotCallback(void *data);
void KhinSlotExitCallback(void *data);
void KhinReturnToMenuCallback(void *data);
std::int32_t KhinXhgDeposit(KhinWork &work);
std::int32_t KhinGstDeposit(KhinWork &work);
std::int32_t KhinAgnDeposit(KhinWork &work);
std::int32_t KhinXhgWithdrawal(KhinWork &work);
std::int32_t KhinGstWithdrawal(KhinWork &work);
std::int32_t KhinAgnWithdrawal(KhinWork &work);
std::int32_t KhinXhgAward(KhinWork &work);
std::int32_t KhinGstAward(KhinWork &work);
std::int32_t KhinAgnAward(KhinWork &work);
std::int32_t KhinXhgLeave(KhinWork &work);
std::int32_t KhinGstLeave(KhinWork &work);
std::int32_t KhinAgnLeave(KhinWork &work);
std::int32_t KhinXhgCancel(KhinWork &work);
std::int32_t KhinGstCancel(KhinWork &work);
std::int32_t KhinAgnCancel(KhinWork &work);
std::int32_t KhinTokensGreeting(KhinWork &work);
void KhinDisplayTokensCallback(void *);
void KhinTokenReward3000Callback(void *);
void KhinTokenReward10000Callback(void *);
void KhinTokenReward50000Callback(void *);
void KhinTokenReward200000Callback(void *);
void KhinTokenReward500000Callback(void *);
void KhinTokenReward1000000Callback(void *);
inline constexpr ScriptAreaDefinition khinScriptDefinition{.areaId = KHIN_AREA, .field04 = 0, .init = InitializeKhinEvent, .main = RunKhinEvent};
} // namespace shenmue::s2
