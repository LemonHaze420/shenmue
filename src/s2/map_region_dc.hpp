#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"
#include <cstddef>

namespace shenmue { namespace s2 {

struct MapRegionEntry {
    shen_name name, map;
    float bounds[4];
};
struct MapRegionFallback {
    shen_name name, map;
};
typedef char Layout_map_region_dc_0[(sizeof(MapRegionEntry) == 0x18) ? 1 : -1];
typedef char Layout_map_region_dc_1[(offsetof(MapRegionEntry, bounds) == 8) ? 1 : -1];
typedef char Layout_map_region_dc_2[(sizeof(MapRegionFallback) == 8) ? 1 : -1];
extern shen_name g_mapRegionNames[6];
shen_name FindMapRegionName(const Vector3 *position);
shen_name scenFindMapRegionName(const Vector3 *position);
void RebuildScenarioEventObjectMapNames();
bool scenIsInArea(shen_name name);
bool ResolveCurrentMapOperand(std::uint32_t index);

} } // namespace shenmue::s2

#endif
