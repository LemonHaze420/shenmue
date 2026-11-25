#pragma once

#include "s2/modifier_volume_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace dc { namespace sdk {
void SetVectorLength(Vector3 *output, const Vector3 *source, float length);
} } }

namespace shenmue { namespace s2 {

struct ShadowVolumeVertex {
    Vector3 position;
    float inverseDepth;
};
typedef char Layout_modifier_vertices_dc_0[(sizeof(ShadowVolumeVertex) == 0x10) ? 1 : -1];
typedef char Layout_modifier_vertices_dc_1[(offsetof(ShadowVolumeVertex, inverseDepth) == 0x0c) ? 1 : -1];
void IntersectShadowVolumeEdge(const ShadowVolumeVertex &first, const ShadowVolumeVertex &second, ShadowVolumeVertex &point);

} } // namespace shenmue::s2

#endif
