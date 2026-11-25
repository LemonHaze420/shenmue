#pragma once

#include "s2/shadow_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

struct ShadowVolumeEdge {
    std::int16_t vertices[2];
};
struct ShadowVolumeFace {
    Vector3 normal;
    std::int16_t vertices[3], edges[3];
};
struct ShadowVolume {
    Vector3 *vertices;
    ShadowVolumeEdge *edges;
    ShadowVolumeFace *faces;
    std::int16_t vertexCount, edgeCount, faceCount;
};
typedef char Layout_shadow_render_dc_0[(sizeof(ShadowVolumeEdge) == 4) ? 1 : -1];
typedef char Layout_shadow_render_dc_1[(sizeof(ShadowVolumeFace) == 0x18) ? 1 : -1];
typedef char Layout_shadow_render_dc_2[(offsetof(ShadowVolumeFace, vertices) == 0x0c) ? 1 : -1];
typedef char Layout_shadow_render_dc_3[(offsetof(ShadowVolumeFace, edges) == 0x12) ? 1 : -1];
typedef char Layout_shadow_render_dc_4[(sizeof(ShadowVolume) == 0x14) ? 1 : -1];
typedef char Layout_shadow_render_dc_5[(offsetof(ShadowVolume, vertexCount) == 0x0c) ? 1 : -1];
typedef char Layout_shadow_render_dc_6[(offsetof(ShadowVolume, edgeCount) == 0x0e) ? 1 : -1];
typedef char Layout_shadow_render_dc_7[(offsetof(ShadowVolume, faceCount) == 0x10) ? 1 : -1];
typedef char Layout_shadow_render_dc_8[(offsetof(ShadowShapeNode, data) + sizeof(void *) * 2 + offsetof(ShadowVolume, vertexCount) == offsetof(ShadowShapeNode, vertexCount)) ? 1 : -1];
typedef char Layout_shadow_render_dc_9[(offsetof(ShadowShapeNode, data) + sizeof(void *) * 2 + offsetof(ShadowVolume, faceCount) == offsetof(ShadowShapeNode, faceCount)) ? 1 : -1];
static const float shadowVolumeMaximumLength = 50.0f, shadowVolumeNominalHeight = 1.8f;
extern std::int32_t g_shadowVolumesDisabled, g_shadowStrengthLimit;
extern ShadowVolume *g_mapShadowVolumes[2];
void DrawShadowVolumes(ShadowVolume *const *volumes, const ShadowDrawParameters *parameters);

} } // namespace shenmue::s2

#endif
