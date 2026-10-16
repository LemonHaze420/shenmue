#pragma once
#include "s2/areas/area_hpam.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name QPAM_RYO{SHEN_NAME(RYO_)};
inline constexpr std::array<std::int32_t, 57> qpamRestingHand{1456, 3639, 46606, 58257, 63535, 64809, 2912, 909, 2001, 727, 64080, 46422, 0, 64809, 46240, 0,
    364, 49700, 0, 0, 0, 0, 0, 48061, 0, 0, 47879, 0, 0, 50427, 0, 0, 0, 0, 0, 48970, 0, 0, 47332, 0, 0, 51156, 0, 0, 0, 0, 0, 46058, 0, 0, 47151, 0, 64445,
    52611, 0, 0, 64990};
inline constexpr std::array<std::int32_t, 57> qpamBuyingHand{0, 0, 2730, 0, 0, 546, 0, 0, 1820, 0, 0, 60805, 0, 0, 62261, 0, 546, 56983, 0, 0, 364, 0, 0, 62806,
    0, 364, 63535, 0, 0, 56072, 0, 0, 0, 0, 0, 63353, 0, 0, 64809, 0, 64445, 55345, 0, 0, 0, 0, 0, 62806, 0, 0, 546, 0, 0, 54435, 0, 0, 0};
void QpamInitNullsub();
void QpamMain(std::uintptr_t request);
void QpamCallback(void *data);
void QpamExitCallback(void *data);
void QpamIdentify(MapStandEventWork &work);
void QpamDescribe(MapStandEventWork &work, std::int32_t mode);
std::int32_t QpamBuy(MapStandEventWork &work);
std::int32_t QpamOwned(MapStandEventWork &work);
extern const ScriptAreaDefinition qpamScriptDefinition;
} // namespace shenmue::s2
