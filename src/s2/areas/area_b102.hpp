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
