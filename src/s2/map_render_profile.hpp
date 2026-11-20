#pragma once

#include "common/types.hpp"
#include "shenmue.h"

#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

struct MapRenderProfile {
    std::uint32_t areaPattern;
    std::uint32_t field04;
    std::uint32_t field08;
    std::uint32_t field0c;
    std::uint32_t field10;
    float farDistance;
    float nearDistance;
    float npcRadius;
    std::uint32_t alphaOperation;
    float textureDistance;
    float textureDistanceW;
    float textureDistanceZ;
};
typedef char MapRenderProfileLayout518[(sizeof(MapRenderProfile) == (SHENMUE_IS_DC ? 0x20 : 0x30)) ? 1 : -1];
typedef char MapRenderProfileLayout592[(offsetof(MapRenderProfile, farDistance) == 0x14) ? 1 : -1];
typedef char MapRenderProfileLayout656[(offsetof(MapRenderProfile, nearDistance) == 0x18) ? 1 : -1];
typedef char MapRenderProfileLayout721[(offsetof(MapRenderProfile, npcRadius) == 0x1c) ? 1 : -1];


const MapRenderProfile &MatchMapRenderProfile(shen_name area);

} } // namespace shenmue::s2
