#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QCAE_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name QCAE_EVENT_TASK{SHEN_NAME(EVNT)};
struct QCAEMainloopData {
    std::uint64_t state;
    HLib::TaskCallback callback;
    std::uint64_t field10;
    std::uint64_t field18;
    std::uint64_t field20;
};
static_assert(sizeof(QCAEMainloopData) == 0x28);
static_assert(offsetof(QCAEMainloopData, callback) == 8);
void AreaQCAEInit();
void AreaQCAEMain(std::uint32_t startParameters);
void AreaQCAECleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQCAEGameplayDefinition;

} // namespace shenmue::s2::areas
