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
#if !SHENMUE_IS_DC
    std::uint32_t alphaOperation;
    float textureDistance;
    float textureDistanceW;
    float textureDistanceZ;
#endif
};
typedef char MapRenderProfileLayout518[(sizeof(MapRenderProfile) == (SHENMUE_IS_DC ? 0x20 : 0x30)) ? 1 : -1];
typedef char MapRenderProfileLayout592[(offsetof(MapRenderProfile, farDistance) == 0x14) ? 1 : -1];
typedef char MapRenderProfileLayout656[(offsetof(MapRenderProfile, nearDistance) == 0x18) ? 1 : -1];
typedef char MapRenderProfileLayout721[(offsetof(MapRenderProfile, npcRadius) == 0x1c) ? 1 : -1];

#if SHENMUE_IS_DC
static const MapRenderProfile initialMapRenderProfile = {SHEN_FIELD(areaPattern) 0, SHEN_FIELD(field04) 0x30000, SHEN_FIELD(field08) 0xe0000, SHEN_FIELD(field0c) 0x30000,
    SHEN_FIELD(field10) 0x400000, SHEN_FIELD(farDistance) 140.0f, SHEN_FIELD(nearDistance) 140.0f, SHEN_FIELD(npcRadius) 48.0f};
extern const MapRenderProfile *g_mapRenderProfiles;
#endif

const MapRenderProfile &MatchMapRenderProfile(shen_name area);

} } // namespace shenmue::s2
