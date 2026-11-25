#include "shenmue.h"
#include "s2/map_info.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/shadow_dc.hpp"
#else
#include "s2/world_render.hpp"
#endif

#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
float GetShadowRange();
void SHRG_Reader(const void *chunk);
void EV_SetAlignedShadowRange(float range);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float g_shadowRange;
float g_shadowRangeInverse;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EV_SetAlignedShadowRange(float range);
void SHRG_Reader(const void *chunk);
float GetShadowRange();
#else
void EV_SetAlignedShadowRange(float range);
void SHRG_Reader(const void *chunk);
float GetShadowRange();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EV_SetAlignedShadowRange(float range) {
    if (range < 0.1f)
        range = 0.1f;
    g_shadowRange = range;
    g_shadowRangeInverse = 1.0f / range;
}

#else

// @verified - p
void EV_SetAlignedShadowRange(float range) {
    g_shadowRange.x = fmax(0.1f, range);
    g_shadowRange.y = 1.0f / g_shadowRange.x;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float GetShadowRange() {
    return g_shadowRange;
}

#else

// @verified - p
float GetShadowRange() {
    return g_shadowRange.x;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SHRG_Reader(const void *chunk) {
    if (!chunk)
        EV_SetAlignedShadowRange(8.0f);
    else
        EV_SetAlignedShadowRange(*reinterpret_cast<const float *>(static_cast<const std::byte *>(chunk) + 8));
}

#else

// @verified - p
void SHRG_Reader(const void *chunk) {
    if (chunk) {
        float range;
        std::memcpy(&range, static_cast<const std::byte *>(chunk) + sizeof(MapInfoChunk), sizeof(range));
        g_shadowRange.x = fmax(0.1f, range);
        g_shadowRange.y = 1.0f / g_shadowRange.x;
    } else {
        g_shadowRange.x = defaultShadowRange;
        g_shadowRange.y = 1.0f / defaultShadowRange;
    }
}

#endif

} // namespace s2
} // namespace shenmue
