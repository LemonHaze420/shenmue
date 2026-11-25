#pragma once

#include "s2/shadow_render_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

typedef void (*ShadowProjectionCallback)();
extern Vector3 g_shadowVolumeDirection, g_shadowVolumeLightPosition, g_shadowVolumeExtrusion;
extern std::int32_t g_shadowVolumeHasVertices;
extern std::uint32_t *g_shadowVolumeWritePointer;
void ApplyShadowVolumeProjection(const Matrix4x4 *projection, const Matrix4x4 *viewport);
void PrepareShadowVolumeProjection();
void SetShadowVolumeProjectionDepth();
void DrawShadowVolumesWithProjection(ShadowVolume *const *volumes, const ShadowDrawParameters *parameters, ShadowProjectionCallback projection);
void DrawShadowVolume(const ShadowVolume *volume, const ShadowDrawParameters *parameters);
void DrawDirectionalShadowVolumeVertices(Vector3 *vertices, std::int32_t count, const ShadowDrawParameters *parameters);
void DrawPointLightShadowVolumeVertices(Vector3 *vertices, std::int32_t count, const ShadowDrawParameters *parameters);
void DrawShadowVolumeFaces(ShadowVolumeFace *faces, std::int32_t count, ShadowVolumeEdge *edges, const ShadowDrawParameters *parameters);
void DrawShadowVolumeSides(ShadowVolumeEdge *edges, std::int32_t count);
void FinishShadowVolume();

} // namespace s2
} // namespace shenmue

#endif
