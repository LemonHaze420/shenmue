#pragma once

#include "s2/modifier_vertices_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

typedef void (*ShadowVolumeCapCallback)(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void DrawShadowVolumeTriangle(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third);
void DrawShadowVolumeQuad(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third,
    const ShadowVolumeVertex *fourth);
void DrawShadowVolumePentagon(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third,
    const ShadowVolumeVertex *fourth, const ShadowVolumeVertex *fifth);
void ShadowVolumeCapUnclipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapThirdClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapSecondClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapSecondThirdClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapFirstClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapFirstThirdClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapFirstSecondClipped(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
void ShadowVolumeCapNullsub(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *);
static const ShadowVolumeCapCallback shadowVolumeCapCallbacks[8]= {ShadowVolumeCapUnclipped, ShadowVolumeCapThirdClipped,
    ShadowVolumeCapSecondClipped, ShadowVolumeCapSecondThirdClipped, ShadowVolumeCapFirstClipped, ShadowVolumeCapFirstThirdClipped,
    ShadowVolumeCapFirstSecondClipped, ShadowVolumeCapNullsub};
std::uint32_t DrawShadowVolumeCap(const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points,
    const std::int16_t *edges);
void DrawShadowVolumeClipWall(std::uint32_t nearType, std::uint32_t farType, const std::int16_t *vertices, const std::int16_t *edges);

} } // namespace shenmue::s2

#endif
