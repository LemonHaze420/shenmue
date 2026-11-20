#include "shenmue.h"
#include "s2/map_info.hpp"

#include "s2/world_render.hpp"

#include <cmath>
#include <cstring>

namespace shenmue { namespace s2 {

// @verified - p
float GetShadowRange() {
    return g_shadowRange.x;
}

// @verified - p
void SHRG_Reader(const void *chunk) {
    if (chunk) {
        float range;
        std::memcpy(&range, static_cast<const std::byte *>(chunk) + sizeof(MapInfoChunk), sizeof(range));
        g_shadowRange.x = std::fmax(0.1f, range);
        g_shadowRange.y = 1.0f / g_shadowRange.x;
    } else {
        g_shadowRange.x = defaultShadowRange;
        g_shadowRange.y = 1.0f / defaultShadowRange;
    }
}

// @verified - p
void EV_SetAlignedShadowRange(float range) {
    g_shadowRange.x = std::fmax(0.1f, range);
    g_shadowRange.y = 1.0f / g_shadowRange.x;
}


} } // namespace shenmue::s2
