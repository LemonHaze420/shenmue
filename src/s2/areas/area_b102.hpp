#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_loop_dc.h"

#define B102_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define B102_ANCHOR SHEN_NAME4('b', '1', '0', '2')
#define B102_AREA_WTR0 SHEN_NAME4('W', 'T', 'R', '0')
#define B102_AREA_QCAC SHEN_NAME4('Q', 'C', 'A', 'C')
#define B102_AREA_QCAD SHEN_NAME4('Q', 'C', 'A', 'D')
typedef struct BayItemShopEntry {
    shen_name prop, target;
    short item, cost;
} BayItemShopEntry;
typedef struct BayItemShop {
    shen_name area, npc;
    BayItemShopEntry items[6];
} BayItemShop;
typedef struct BayItemShopWork {
    HLibTask *parent;
    HLibTask *task;
    unsigned int flags;
    shen_name area, prop;
} BayItemShopWork;
const BayItemShop *FindBayItemShop(shen_name area);
void B102_main_func(const shen_name *argument);
void BayItemShopWaitPackageCallback(void *data);
void BayItemShopStartCallback(void *data);
void BayItemShopWaitItemEffectCallback(void *data);
void BayItemShopDoneCallback(void *data);
void BayItemShopQueueWait(shen_name area, shen_name prop);
int BayItemShopChargeItem(int phase, int value, HLibTask *parent);
int BayItemShopDeliverItem(int phase, int value, HLibTask *parent);
int BayItemShopUpdateAreaItems(shen_name area, shen_name prop);
void BayItemShopUpdateItems(const BayItemShop *shop, shen_name prop);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name B102_EVENT_TASK{SHEN_NAME(EVNT)};
struct BayItemShopEntry {
    shen_name prop, target;
    std::int16_t item, cost;
};
struct BayItemShop {
    shen_name area, npc;
    std::array<BayItemShopEntry, 6> items;
};
struct BayItemShopWork {
    HLib::Task *parent;
    HLib::Task *task;
    std::uint32_t flags;
    shen_name area, prop;
    std::uint32_t reserved1c;
};
static_assert(sizeof(BayItemShopEntry) == 12);
static_assert(sizeof(BayItemShop) == 80);
static_assert(sizeof(BayItemShopWork) == 32);
static_assert(offsetof(BayItemShopWork, area) == 20);
const BayItemShop *FindBayItemShop(shen_name area);
void B102_main_func(std::uintptr_t argument);
void BayItemShopWaitPackageCallback(void *);
void BayItemShopStartCallback(void *);
void BayItemShopWaitItemEffectCallback(void *);
void BayItemShopDoneCallback(void *);
extern const ScriptAreaDefinition areaB102ScriptDefinition;
} // namespace shenmue::s2
#endif
