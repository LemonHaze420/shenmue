#include "s2/modifier_vertices_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

// @verified - d
void SetVectorLength(Vector3 *output, const Vector3 *source, float length) {
    const float scale = length / njScalor(source);
    output->x = source->x * scale;
    output->y = source->y * scale;
    output->z = scale * source->z;
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

namespace shenmue {
namespace s2 {

// @verified - d
void IntersectShadowVolumeEdge(const ShadowVolumeVertex &first, const ShadowVolumeVertex &second, ShadowVolumeVertex &point) {
    if ((*reinterpret_cast<const std::int32_t *>(&first.inverseDepth) ^ *reinterpret_cast<const std::int32_t *>(&second.inverseDepth)) >= 0) {
        point.inverseDepth = -1.0f;
        return;
    }
    const float inverseSpan = 1.0f / (first.position.z - second.position.z);
    const float nearClip = dc::sdk::g_mt7NearClip;
    point.inverseDepth = 1.0f / nearClip;
    point.position.z = nearClip;
    const float amount = (nearClip - second.position.z) * inverseSpan;
    point.position.x = (first.position.x - second.position.x) * amount + second.position.x;
    point.position.y = (first.position.y - second.position.y) * amount + second.position.y;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawDirectionalShadowVolumeVertices(Vector3 *vertices, std::int32_t count, const ShadowDrawParameters *) {
    ShadowVolumeVertex *near = g_shadowVertices;
    ShadowVolumeVertex *far = g_shadowExtrudedVertices;
    ShadowVolumeVertex *points = g_shadowNearPlaneVertices;
    const float nearClip = dc::sdk::g_mt7NearClip;
    for (std::int32_t index = 0; index < count; ++points, ++index, ++vertices, ++near, ++far) {
        Matrix4x4::TransformPoint(*vertices, near->position);
        far->position.x = near->position.x + g_shadowVolumeExtrusion.x;
        far->position.y = near->position.y + g_shadowVolumeExtrusion.y;
        float *farDepth = &far->position.z;
        const float *nearDepth = &near->position.z;
        const Vector3 *extrusion = &g_shadowVolumeExtrusion;
        *farDepth = *nearDepth + extrusion->z;
        if (nearClip > *nearDepth)
            near->inverseDepth = -1.0f;
        else
            near->inverseDepth = 1.0f / *nearDepth;
        if (nearClip > far->position.z)
            far->inverseDepth = -1.0f;
        else
            far->inverseDepth = 1.0f / far->position.z;
        IntersectShadowVolumeEdge(*near, *far, *points);
    }
}
#else
void DrawDirectionalShadowVolumeVertices(Vector3 *vertices, std::int32_t count, const ShadowDrawParameters *) {
    const float nearClip = dc::sdk::g_mt7NearClip;
    if (count <= 0)
        return;
    ShadowVolumeVertex *near = g_shadowVertices;
    ShadowVolumeVertex *far = g_shadowExtrudedVertices;
    ShadowVolumeVertex *points = g_shadowNearPlaneVertices;
    for (std::int32_t index = 0; index < count; ++points, ++index, ++vertices, ++near, ++far) {
        Matrix4x4::TransformPoint(*vertices, near->position);
        far->position.x = near->position.x + g_shadowVolumeExtrusion.x;
        far->position.y = near->position.y + g_shadowVolumeExtrusion.y;
        far->position.z = near->position.z + g_shadowVolumeExtrusion.z;
        if (nearClip > near->position.z)
            near->inverseDepth = -1.0f;
        else
            near->inverseDepth = 1.0f / near->position.z;
        if (nearClip > far->position.z)
            far->inverseDepth = -1.0f;
        else
            far->inverseDepth = 1.0f / far->position.z;
        IntersectShadowVolumeEdge(*near, *far, *points);
    }
}
#endif

void DrawPointLightShadowVolumeVertices(Vector3 *vertices, std::int32_t count, const ShadowDrawParameters *parameters) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    Vector3 *extrusion = &g_shadowVolumeExtrusion;
#else
    Vector3 *const extrusion = &g_shadowVolumeExtrusion;
#endif
    ShadowVolumeVertex *near = g_shadowVertices;
    ShadowVolumeVertex *far = g_shadowExtrudedVertices;
    ShadowVolumeVertex *points = g_shadowNearPlaneVertices;
    const float length = parameters->length;
    const float nearClip = dc::sdk::g_mt7NearClip;
    for (std::int32_t index = 0; index < count; ++points, ++index, ++vertices, ++near, ++far) {
        am2Vector3Subtract(extrusion, vertices, &g_shadowVolumeLightPosition);
        dc::sdk::SetVectorLength(extrusion, extrusion, length);
        Matrix4x4::Transform(*extrusion, *extrusion);
        Matrix4x4::TransformPoint(*vertices, near->position);
        far->position.x = near->position.x + extrusion->x;
        far->position.y = near->position.y + extrusion->y;
        far->position.z = near->position.z + extrusion->z;
        if (nearClip > near->position.z)
            near->inverseDepth = -1.0f;
        else
            near->inverseDepth = 1.0f / near->position.z;
        if (nearClip > far->position.z)
            far->inverseDepth = -1.0f;
        else
            far->inverseDepth = 1.0f / far->position.z;
        IntersectShadowVolumeEdge(*near, *far, *points);
    }
}

} // namespace s2
} // namespace shenmue

#endif
