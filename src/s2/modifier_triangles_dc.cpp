#include "s2/modifier_faces_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/modifier_volume_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/ninja.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#endif
#include <cstdio>
#include <cstring>
#include "s2/prop_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/search_dc.hpp"

namespace shenmue {
namespace s2 {

void DrawShadowVolumesWithProjection(ShadowVolume *const *volumes, const ShadowDrawParameters *parameters, ShadowProjectionCallback projection);
void SetShadowVolumeProjectionDepth();
void DrawShadowVolume(const ShadowVolume *volume, const ShadowDrawParameters *parameters);
void DrawShadowVolumeFaces(ShadowVolumeFace *faces, std::int32_t count, ShadowVolumeEdge *edges, const ShadowDrawParameters *parameters);
void DrawShadowVolumeClipWall(std::uint32_t nearType, std::uint32_t farType, const std::int16_t *vertices, const std::int16_t *edges);
std::uint32_t DrawShadowVolumeCap(const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges);
void DrawShadowVolumeSides(ShadowVolumeEdge *edges, std::int32_t count);
static void InitializeShadowStoreQueueTargets();
void DrawShadowVolumeTriangle(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third);
void DrawShadowVolumeQuad(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third, const ShadowVolumeVertex *fourth);
void DrawShadowVolumePentagon(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third,
    const ShadowVolumeVertex *fourth, const ShadowVolumeVertex *fifth);
void FinishShadowVolume();

void PrepareShadowVolumeProjection();
void PrepareShadowVolumeProjection();
Vector3 g_shadowVolumeDirection, g_shadowVolumeLightPosition = {}, g_shadowVolumeExtrusion = {};
std::int32_t g_shadowVolumeHasVertices;
std::uint32_t *g_shadowVolumeWritePointer;

// @verified - d
void InitializeShadowDrawBuffers(std::byte *memory, const ShadowDrawParameters &parameters) {
    g_shadowVertexCapacity = parameters.vertexCapacity;
    g_shadowClipCapacity = parameters.clipCapacity;
    g_shadowPolygonCapacity = parameters.polygonCapacity;
    const std::int32_t vertexSize = parameters.vertexCapacity * 0x10;
    const std::int32_t clipSize = parameters.clipCapacity * 0x10;
    const std::int32_t polygonSize = parameters.polygonCapacity * 0x0c;
    g_shadowVertices = reinterpret_cast<ShadowVolumeVertex *>(memory);
    g_shadowExtrudedVertices = reinterpret_cast<ShadowVolumeVertex *>(memory + vertexSize);
    std::int32_t offset = vertexSize + vertexSize;
    g_shadowNearPlaneVertices = reinterpret_cast<ShadowVolumeVertex *>(memory + offset);
    offset += vertexSize;
    g_shadowEdgeNearPoints = reinterpret_cast<ShadowVolumeVertex *>(memory + offset);
    offset += clipSize;
    g_shadowEdgeFarPoints = reinterpret_cast<ShadowVolumeVertex *>(memory + offset);
    offset += clipSize;
    g_shadowFaceLightVectors = reinterpret_cast<Vector3 *>(memory + offset);
    offset += polygonSize;
    g_shadowEdgeFlags = reinterpret_cast<std::uint8_t *>(memory + offset);
    kmSetCheapShadowMode(parameters.shadowStrength);
    InitializeShadowStoreQueueTargets();
}

// @verified - d
void DrawShadowVolumes(ShadowVolume *const *volumes, const ShadowDrawParameters *parameters) {
    DrawShadowVolumesWithProjection(volumes, parameters, PrepareShadowVolumeProjection);
}

// @verified - d
void DrawShadowVolumesWithProjection(ShadowVolume *const *volumes, const ShadowDrawParameters *parameters, ShadowProjectionCallback projection) {
    matrix_stack::Matrix4x4_Push(0);
    kmSetCheapShadowMode(parameters->shadowStrength);
    g_shadowVolumeDirection.x = parameters->lightDirection.x;
    g_shadowVolumeDirection.y = parameters->lightDirection.y;
    g_shadowVolumeDirection.z = parameters->lightDirection.z;
    Matrix4x4::AffineProjectionSwap();
    Matrix4x4::Transform(g_shadowVolumeDirection, g_shadowVolumeDirection);
    Matrix4x4::AffineProjectionSwap();
    if (parameters->flags & 1U) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(parameters->lightDirection, g_shadowVolumeLightPosition);
        PopCurrentMatrix(1);
    }
    projection();
    SetShadowVolumeProjectionDepth();
    if ((parameters->flags & 1U) == 0) {
        Vector3 &extrusion = g_shadowVolumeExtrusion;
        const float length = -parameters->length;
        extrusion.x = g_shadowVolumeDirection.x * length;
        extrusion.y = g_shadowVolumeDirection.y * length;
        extrusion.z = g_shadowVolumeDirection.z * length;
        Matrix4x4::Transform(extrusion, extrusion);
    }
    while (*volumes) {
        g_shadowVolumeHasVertices = 0;
        DrawShadowVolume(*volumes++, parameters);
    }
    PopCurrentMatrix(1);
}

// @verified - d
void SetShadowVolumeProjectionDepth() {
    Matrix4x4 projection;
    matrix_stack::GetMatrix(&projection);
    projection.rows[0].z = projection.rows[0].w;
    projection.rows[1].z = projection.rows[1].w;
    projection.rows[2].z = projection.rows[2].w;
    projection.rows[3].z = projection.rows[3].w;
    matrix_stack::LoadMatrix(&projection);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawShadowVolume(const ShadowVolume *volume, const ShadowDrawParameters *parameters) {
    std::uint32_t *flags = reinterpret_cast<std::uint32_t *>(g_shadowEdgeFlags);
    const std::int32_t edges = volume->edgeCount;
    std::uint32_t words = static_cast<std::uint32_t>(edges);
    words >>= 2;
    if (edges & 3)
        ++words;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(words); ++index)
        *flags++ = 0;
    if ((parameters->flags & 1U) == 0U) {
        DrawDirectionalShadowVolumeVertices(volume->vertices, volume->vertexCount, parameters);
    } else {
        const Vector3 *const vertices = volume->vertices;
        const ShadowVolumeFace *face = volume->faces;
        Vector3 *light = g_shadowFaceLightVectors;
        const float x = g_shadowVolumeLightPosition.x;
        const float y = g_shadowVolumeLightPosition.y;
        const float z = g_shadowVolumeLightPosition.z;
        std::uint32_t remaining = static_cast<std::uint32_t>(static_cast<std::int32_t>(volume->faceCount));
        while (remaining) {
            const Vector3 &vertex = vertices[face->vertices[0]];
            light->x = x - vertex.x;
            light->y = y - vertex.y;
            light->z = z - vertex.z;
            ++light;
            ++face;
            --remaining;
        }
        DrawPointLightShadowVolumeVertices(volume->vertices, volume->vertexCount, parameters);
    }
    DrawShadowVolumeFaces(volume->faces, volume->faceCount, volume->edges, parameters);
    DrawShadowVolumeSides(volume->edges, volume->edgeCount);
    FinishShadowVolume();
}
#else
void DrawShadowVolume(const ShadowVolume *volume, const ShadowDrawParameters *parameters) {
    const std::int32_t edges = volume->edgeCount;
    std::uint32_t words = static_cast<std::uint32_t>(edges);
    words >>= 2;
    if (edges & 3)
        ++words;
    std::uint32_t *flags = reinterpret_cast<std::uint32_t *>(g_shadowEdgeFlags);
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(words); ++index)
        *flags++ = 0;
    if ((parameters->flags & 1U) == 0U) {
        DrawDirectionalShadowVolumeVertices(volume->vertices, volume->vertexCount, parameters);
    } else {
        const Vector3 *const vertices = volume->vertices;
        const ShadowVolumeFace *face = volume->faces;
        Vector3 *light = g_shadowFaceLightVectors;
        std::uint32_t remaining = static_cast<std::uint32_t>(static_cast<std::int32_t>(volume->faceCount));
        while (remaining) {
            const Vector3 &vertex = vertices[face->vertices[0]];
            light->x = g_shadowVolumeLightPosition.x - vertex.x;
            light->y = g_shadowVolumeLightPosition.y - vertex.y;
            light->z = g_shadowVolumeLightPosition.z - vertex.z;
            ++light;
            ++face;
            --remaining;
        }
        DrawPointLightShadowVolumeVertices(volume->vertices, volume->vertexCount, parameters);
    }
    DrawShadowVolumeFaces(volume->faces, volume->faceCount, volume->edges, parameters);
    DrawShadowVolumeSides(volume->edges, volume->edgeCount);
    FinishShadowVolume();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawShadowVolumeFaces(ShadowVolumeFace *faces, std::int32_t count, ShadowVolumeEdge *edges, const ShadowDrawParameters *parameters) {
    const Vector3 *lights[2];
    lights[1] = g_shadowFaceLightVectors;
    lights[0] = &g_shadowVolumeDirection;
    const Vector3 **light = &lights[0];
    if (parameters->flags & 1U)
        light = &lights[1];
    for (; count > 0; --count) {
        if (am2Vector3Dot(&faces->normal, *light) < 0.0f) {
            const std::int16_t *edgeIndex = faces->edges;
            for (std::int32_t side = 3; side > 0; --side) {
                const std::int16_t index = *edgeIndex++;
                std::uint8_t *const flag = g_shadowEdgeFlags + index;
                if (*flag)
                    *flag = 2;
                else {
                    *flag = 1;
                    const std::int32_t first = edges[index].vertices[0];
                    const std::int32_t second = edges[index].vertices[1];
                    IntersectShadowVolumeEdge(g_shadowVertices[first], g_shadowVertices[second], g_shadowEdgeNearPoints[index]);
                    IntersectShadowVolumeEdge(g_shadowExtrudedVertices[first], g_shadowExtrudedVertices[second], g_shadowEdgeFarPoints[index]);
                }
            }
            const std::uint32_t nearType = DrawShadowVolumeCap(g_shadowVertices, faces->vertices, g_shadowEdgeNearPoints, faces->edges);
            const std::uint32_t farType = DrawShadowVolumeCap(g_shadowExtrudedVertices, faces->vertices, g_shadowEdgeFarPoints, faces->edges);
            if (nearType | farType)
                DrawShadowVolumeClipWall(nearType, farType, faces->vertices, faces->edges);
        }
        ++faces;
        ++lights[1];
    }
}
#else
// @verified - d
void DrawShadowVolumeFaces(ShadowVolumeFace *faces, std::int32_t count, ShadowVolumeEdge *edges, const ShadowDrawParameters *parameters) {
    const Vector3 *lights[2];
    lights[1] = g_shadowFaceLightVectors;
    lights[0] = &g_shadowVolumeDirection;
    const Vector3 **light = &lights[0];
    if (parameters->flags & 1U)
        light = &lights[1];
    for (; count > 0; --count) {
        if (am2Vector3Dot(&faces->normal, *light) < 0.0f) {
            const std::int16_t *edgeIndex = faces->edges;
            for (std::int32_t side = 3; side > 0; --side) {
                const std::int16_t index = *edgeIndex++;
                if (!g_shadowEdgeFlags[index]) {
                    g_shadowEdgeFlags[index] = 1;
                    const std::int32_t first = edges[index].vertices[0];
                    const std::int32_t second = edges[index].vertices[1];
                    IntersectShadowVolumeEdge(g_shadowVertices[first], g_shadowVertices[second], g_shadowEdgeNearPoints[index]);
                    IntersectShadowVolumeEdge(g_shadowExtrudedVertices[first], g_shadowExtrudedVertices[second], g_shadowEdgeFarPoints[index]);
                } else
                    g_shadowEdgeFlags[index] = 2;
            }
            const std::uint32_t nearType = DrawShadowVolumeCap(g_shadowVertices, faces->vertices, g_shadowEdgeNearPoints, faces->edges);
            const std::uint32_t farType = DrawShadowVolumeCap(g_shadowExtrudedVertices, faces->vertices, g_shadowEdgeFarPoints, faces->edges);
            if (nearType || farType)
                DrawShadowVolumeClipWall(nearType, farType, faces->vertices, faces->edges);
        }
        ++faces;
        ++lights[1];
    }
}
#endif

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawShadowVolumeClipWall(std::uint32_t nearType, std::uint32_t farType, const std::int16_t *vertices, const std::int16_t *edges) {
    if (nearType == 0) {
        if (farType == 7) {
            DrawShadowVolumeTriangle(&g_shadowNearPlaneVertices[vertices[0]], &g_shadowNearPlaneVertices[vertices[1]], &g_shadowNearPlaneVertices[vertices[2]]);
        } else {
            DrawShadowVolumeCap(g_shadowNearPlaneVertices, vertices, g_shadowEdgeFarPoints, edges);
        }
    } else if (nearType == 7) {
        if (farType == 7)
            return;
        if (farType == 0) {
            DrawShadowVolumeTriangle(&g_shadowNearPlaneVertices[vertices[0]], &g_shadowNearPlaneVertices[vertices[1]], &g_shadowNearPlaneVertices[vertices[2]]);
        } else {
            DrawShadowVolumeCap(g_shadowNearPlaneVertices, vertices, g_shadowEdgeFarPoints, edges);
        }
    } else if (farType == 0 || farType == 7) {
        DrawShadowVolumeCap(g_shadowNearPlaneVertices, vertices, g_shadowEdgeNearPoints, edges);
    } else if (nearType == farType) {
        switch (farType) {
        case 3:
        case 4:
            DrawShadowVolumeQuad(
                &g_shadowEdgeNearPoints[edges[0]], &g_shadowEdgeNearPoints[edges[2]], &g_shadowEdgeFarPoints[edges[2]], &g_shadowEdgeFarPoints[edges[0]]);
            break;
        case 2:
        case 5:
            DrawShadowVolumeQuad(
                &g_shadowEdgeNearPoints[edges[1]], &g_shadowEdgeNearPoints[edges[0]], &g_shadowEdgeFarPoints[edges[0]], &g_shadowEdgeFarPoints[edges[1]]);
            break;
        case 1:
        case 6:
            DrawShadowVolumeQuad(
                &g_shadowEdgeNearPoints[edges[2]], &g_shadowEdgeNearPoints[edges[1]], &g_shadowEdgeFarPoints[edges[1]], &g_shadowEdgeFarPoints[edges[2]]);
            break;
        }
    } else {
        if (nearType & 4U) {
            nearType = ~nearType & 7U;
            farType = ~farType & 7U;
        }
        switch (static_cast<std::int32_t>(nearType)) {
        case 1:
            if (farType == 3) {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[2]], &g_shadowEdgeNearPoints[edges[1]], &g_shadowNearPlaneVertices[vertices[1]],
                    &g_shadowEdgeFarPoints[edges[0]], &g_shadowEdgeFarPoints[edges[2]]);
            } else {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[1]], &g_shadowEdgeNearPoints[edges[2]], &g_shadowNearPlaneVertices[vertices[0]],
                    &g_shadowEdgeFarPoints[edges[0]], &g_shadowEdgeFarPoints[edges[1]]);
            }
            break;
        case 2:
            if (farType == 3) {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[0]], &g_shadowEdgeNearPoints[edges[1]], &g_shadowNearPlaneVertices[vertices[2]],
                    &g_shadowEdgeFarPoints[edges[2]], &g_shadowEdgeFarPoints[edges[0]]);
            } else {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[1]], &g_shadowEdgeNearPoints[edges[0]], &g_shadowNearPlaneVertices[vertices[0]],
                    &g_shadowEdgeFarPoints[edges[2]], &g_shadowEdgeFarPoints[edges[1]]);
            }
            break;
        case 3:
            if (farType == 2) {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[0]], &g_shadowEdgeNearPoints[edges[2]], &g_shadowNearPlaneVertices[vertices[2]],
                    &g_shadowEdgeFarPoints[edges[1]], &g_shadowEdgeFarPoints[edges[0]]);
            } else {
                DrawShadowVolumePentagon(&g_shadowEdgeNearPoints[edges[2]], &g_shadowEdgeNearPoints[edges[0]], &g_shadowNearPlaneVertices[vertices[1]],
                    &g_shadowEdgeFarPoints[edges[1]], &g_shadowEdgeFarPoints[edges[2]]);
            }
            break;
        }
    }
}
#else
void DrawShadowVolumeClipWall(std::uint32_t nearType, std::uint32_t farType, const std::int16_t *vertices, const std::int16_t *edges) {
    ShadowVolumeVertex *const nearPlane = g_shadowNearPlaneVertices;
    ShadowVolumeVertex *const nearPoints = g_shadowEdgeNearPoints;
    ShadowVolumeVertex *const farPoints = g_shadowEdgeFarPoints;
    if (nearType == 0 || nearType == 7) {
        if (nearType == 7 && farType == 7)
            return;
        if (farType == (nearType == 0 ? 7U : 0U)) {
            DrawShadowVolumeTriangle(&nearPlane[vertices[0]], &nearPlane[vertices[1]], &nearPlane[vertices[2]]);
            return;
        }
        DrawShadowVolumeCap(nearPlane, vertices, farPoints, edges);
        return;
    }
    if (farType == 0 || farType == 7) {
        DrawShadowVolumeCap(nearPlane, vertices, nearPoints, edges);
        return;
    }
    if (nearType == farType) {
        std::int16_t first;
        std::int16_t second;
        switch (nearType) {
        case 1:
        case 6:
            first = edges[2];
            second = edges[1];
            break;
        case 2:
        case 5:
            first = edges[1];
            second = edges[0];
            break;
        case 3:
        case 4:
            first = edges[0];
            second = edges[2];
            break;
        default:
            return;
        }
        DrawShadowVolumeQuad(&nearPoints[first], &nearPoints[second], &farPoints[second], &farPoints[first]);
        return;
    }
    if (nearType & 4U) {
        nearType = ~nearType & 7U;
        farType = ~farType & 7U;
    }
    std::int16_t edge;
    std::int16_t vertex;
    std::int16_t otherEdge;
    const ShadowVolumeVertex *corner;
    switch (nearType) {
    case 1:
        corner = &farPoints[edges[0]];
        if (farType == 3) {
            edge = edges[2];
            vertex = vertices[1];
            otherEdge = edges[1];
        } else {
            edge = edges[1];
            vertex = vertices[0];
            otherEdge = edges[2];
        }
        break;
    case 2:
        corner = &farPoints[edges[2]];
        if (farType == 3) {
            edge = edges[0];
            vertex = vertices[2];
            otherEdge = edges[1];
        } else {
            edge = edges[1];
            vertex = vertices[0];
            otherEdge = edges[0];
        }
        break;
    case 3:
        corner = &farPoints[edges[1]];
        if (farType == 2) {
            edge = edges[0];
            vertex = vertices[2];
            otherEdge = edges[2];
        } else {
            edge = edges[2];
            vertex = vertices[1];
            otherEdge = edges[0];
        }
        break;
    default:
        return;
    }
    DrawShadowVolumePentagon(&nearPoints[edge], &nearPoints[otherEdge], &nearPlane[vertex], corner, &farPoints[edge]);
}

#endif
// @verified - d
std::uint32_t DrawShadowVolumeCap(
    const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    std::uint32_t type = 0;
    for (std::int32_t index = 0; index < 3; ++index) {
        type <<= 1;
        type |= *reinterpret_cast<const std::uint32_t *>(&vertices[indices[index]].inverseDepth) >> 31U;
    }
    shadowVolumeCapCallbacks[type](vertices, indices, points, edges);
    return type;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHADOW_SIDE_SIGN(vertex) (*reinterpret_cast<const std::int32_t *>(&(vertex)->inverseDepth))
void DrawShadowVolumeSides(ShadowVolumeEdge *edges, std::int32_t count) {
    const char *edgeFlags = reinterpret_cast<const char *>(g_shadowEdgeFlags);
    const ShadowVolumeVertex *nearPoints = g_shadowEdgeNearPoints;
    const ShadowVolumeVertex *farPoints = g_shadowEdgeFarPoints;
    while (count-- > 0) {
        if (*edgeFlags == 1) {
            const ShadowVolumeVertex *corners[5];
            const std::int32_t first = edges->vertices[0];
            const std::int32_t second = edges->vertices[1];
            std::int32_t size = 0;
            std::int32_t sign = SHADOW_SIDE_SIGN(&g_shadowVertices[first]);
            if (sign >= 0)
                corners[size++] = &g_shadowVertices[first];
            std::int32_t next = SHADOW_SIDE_SIGN(&g_shadowVertices[second]);
            if ((sign ^ next) < 0) {
                corners[size++] = nearPoints;
                sign = next;
            }
            if (next >= 0)
                corners[size++] = &g_shadowVertices[second];
            next = SHADOW_SIDE_SIGN(&g_shadowExtrudedVertices[second]);
            if ((sign ^ next) < 0) {
                corners[size++] = &g_shadowNearPlaneVertices[second];
                sign = next;
            }
            if (next >= 0)
                corners[size++] = &g_shadowExtrudedVertices[second];
            next = SHADOW_SIDE_SIGN(&g_shadowExtrudedVertices[first]);
            if ((sign ^ next) < 0) {
                corners[size++] = farPoints;
                sign = next;
            }
            if (next >= 0)
                corners[size++] = &g_shadowExtrudedVertices[first];
            if ((SHADOW_SIDE_SIGN(&g_shadowVertices[first]) ^ sign) < 0)
                corners[size++] = &g_shadowNearPlaneVertices[first];
            if (size) {
                if (size == 3)
                    DrawShadowVolumeTriangle(corners[0], corners[1], corners[2]);
                else if (size == 4) {
                    DrawShadowVolumeTriangle(corners[0], corners[1], corners[2]);
                    DrawShadowVolumeTriangle(corners[0], corners[2], corners[3]);
                } else {
                    if (size != 5)
                        asm volatile("trapa #0x30" : : : "memory");
                    DrawShadowVolumeTriangle(corners[0], corners[1], corners[2]);
                    DrawShadowVolumeTriangle(corners[0], corners[2], corners[3]);
                    DrawShadowVolumeTriangle(corners[0], corners[3], corners[4]);
                }
            }
        }
        ++edges;
        ++edgeFlags;
        ++nearPoints;
        ++farPoints;
    }
}
#undef SHADOW_SIDE_SIGN
#else
// @verified - d
void DrawShadowVolumeSides(ShadowVolumeEdge *edges, std::int32_t count) {
    const ShadowVolumeVertex *const near = g_shadowVertices;
    const ShadowVolumeVertex *const far = g_shadowExtrudedVertices;
    const ShadowVolumeVertex *const nearPlane = g_shadowNearPlaneVertices;
    const ShadowVolumeVertex *const nearPoints = g_shadowEdgeNearPoints;
    const ShadowVolumeVertex *const farPoints = g_shadowEdgeFarPoints;
    const std::uint8_t *const edgeFlags = g_shadowEdgeFlags;
    for (std::int32_t index = 0; index < count; ++index) {
        if (edgeFlags[index] != 1)
            continue;
        const std::int16_t first = edges[index].vertices[0];
        const std::int16_t second = edges[index].vertices[1];
        const ShadowVolumeVertex *corners[6];
        std::int32_t size = 0;
        std::uint32_t sign = floatAsU32(near[first].inverseDepth);
        if (!(sign >> 31))
            corners[size++] = &near[first];
        std::uint32_t next = floatAsU32(near[second].inverseDepth);
        if ((sign ^ next) >> 31) {
            corners[size++] = &nearPoints[index];
            sign = next;
        }
        if (!(next >> 31))
            corners[size++] = &near[second];
        next = floatAsU32(far[second].inverseDepth);
        if ((sign ^ next) >> 31) {
            corners[size++] = &nearPlane[second];
            sign = next;
        }
        if (!(next >> 31))
            corners[size++] = &far[second];
        next = floatAsU32(far[first].inverseDepth);
        if ((sign ^ next) >> 31) {
            corners[size++] = &farPoints[index];
            sign = next;
        }
        if (!(next >> 31))
            corners[size++] = &far[first];
        next = floatAsU32(near[first].inverseDepth);
        if ((sign ^ next) >> 31)
            corners[size++] = &nearPlane[first];
        if (!size)
            continue;
        if (size != 3 && size != 4 && size != 5)
            asm volatile("trapa #0x30" : : : "memory");
        DrawShadowVolumeTriangle(corners[0], corners[1], corners[2]);
        if (size != 3) {
            DrawShadowVolumeTriangle(corners[0], corners[2], corners[3]);
            if (size != 4)
                DrawShadowVolumeTriangle(corners[0], corners[3], corners[4]);
        }
    }
}
#endif

static void InitializeShadowStoreQueueTargets() {
    g_shadowStripHeader[0] = 0x81000000U;
    g_shadowStripHeader[1] = 0;
    g_shadowStripLastHeader[0] = 0x81000040U;
    g_shadowStripLastHeader[1] = 0x20000000U;
}

// @verified - d
void DrawShadowVolumeTriangle(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third) {
    std::uint32_t *output;
    if (!g_shadowVolumeHasVertices) {
        g_shadowVolumeHasVertices = 1;
        const std::uint32_t buffer = dc::sdk::g_sdkVertexBufferPointers[1];
        struct StoreQueueAddressControls {
            std::uint32_t queue0;
            std::uint32_t queue1;
        };
        volatile StoreQueueAddressControls *const addressControl = reinterpret_cast<volatile StoreQueueAddressControls *>(dc::sdk::kStoreQueueAddressControl0);
        addressControl->queue0 = 0x0c;
        addressControl->queue1 = 0x0c;
        output = reinterpret_cast<std::uint32_t *>((buffer & 0x03ffffffU) | 0xe0000000U);
        output[0] = g_shadowStripHeader[0];
        output[1] = g_shadowStripHeader[1];
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 8;
    } else {
        output = g_shadowVolumeWritePointer;
    }
    float inverseDepth = first->inverseDepth;
    output[0] = 0xffffffffU;
    float *coordinates = reinterpret_cast<float *>(output + 1);
    *coordinates++ = first->position.x * inverseDepth;
    *coordinates++ = first->position.y * inverseDepth;
    *coordinates = inverseDepth;
    inverseDepth = second->inverseDepth;
    coordinates = reinterpret_cast<float *>(output + 4);
    *coordinates++ = second->position.x * inverseDepth;
    *coordinates++ = second->position.y * inverseDepth;
    *coordinates = inverseDepth;
    inverseDepth = third->inverseDepth;
    *reinterpret_cast<float *>(output + 7) = third->position.x * inverseDepth;
    asm volatile("pref @%0" : : "r"(output) : "memory");
    coordinates = reinterpret_cast<float *>(output + 8);
    coordinates[0] = third->position.y * inverseDepth;
    coordinates[1] = inverseDepth;
    asm volatile("pref @%0" : : "r"(output + 8) : "memory");
    g_shadowVolumeWritePointer = reinterpret_cast<std::uint32_t *>(coordinates) + 8;
}

// @verified - d
void DrawShadowVolumeQuad(
    const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third, const ShadowVolumeVertex *fourth) {
    DrawShadowVolumeTriangle(first, second, third);
    DrawShadowVolumeTriangle(first, third, fourth);
}

// @verified - d
void DrawShadowVolumePentagon(const ShadowVolumeVertex *first, const ShadowVolumeVertex *second, const ShadowVolumeVertex *third,
    const ShadowVolumeVertex *fourth, const ShadowVolumeVertex *fifth) {
    DrawShadowVolumeTriangle(first, second, third);
    DrawShadowVolumeTriangle(first, third, fourth);
    DrawShadowVolumeTriangle(first, fourth, fifth);
}

// @verified - d
void ShadowVolumeCapUnclipped(const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *, const std::int16_t *) {
    DrawShadowVolumeTriangle(&vertices[indices[0]], &vertices[indices[1]], &vertices[indices[2]]);
}

// @verified - d
void ShadowVolumeCapThirdClipped(const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeQuad(&points[edges[2]], &vertices[indices[0]], &vertices[indices[1]], &points[edges[1]]);
}

// @verified - d
void ShadowVolumeCapSecondClipped(
    const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeQuad(&points[edges[1]], &vertices[indices[2]], &vertices[indices[0]], &points[edges[0]]);
}

// @verified - d
void ShadowVolumeCapSecondThirdClipped(
    const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeTriangle(&vertices[indices[0]], &points[edges[0]], &points[edges[2]]);
}

// @verified - d
void ShadowVolumeCapFirstClipped(const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeQuad(&points[edges[0]], &vertices[indices[1]], &vertices[indices[2]], &points[edges[2]]);
}

// @verified - d
void ShadowVolumeCapFirstThirdClipped(
    const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeTriangle(&vertices[indices[1]], &points[edges[1]], &points[edges[0]]);
}

// @verified - d
void ShadowVolumeCapFirstSecondClipped(
    const ShadowVolumeVertex *vertices, const std::int16_t *indices, const ShadowVolumeVertex *points, const std::int16_t *edges) {
    DrawShadowVolumeTriangle(&vertices[indices[2]], &points[edges[2]], &points[edges[1]]);
}

// @verified - d
void ShadowVolumeCapNullsub(const ShadowVolumeVertex *, const std::int16_t *, const ShadowVolumeVertex *, const std::int16_t *) {
}

// @verified - d
void FinishShadowVolume() {
    if (g_shadowVolumeHasVertices) {
        std::uint32_t *output = g_shadowVolumeWritePointer;
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 8;
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output -= 24;
        output[0] = g_shadowStripHeader[0];
        output[1] = g_shadowStripHeader[1];
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 8;
        output[0] = g_shadowStripLastHeader[0];
        output[1] = g_shadowStripLastHeader[1];
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 24;
        dc::sdk::g_sdkVertexBufferPointers[1] =
            (dc::sdk::g_sdkVertexBufferPointers[1] & 0xfc000000U) | (reinterpret_cast<std::uintptr_t>(output) & 0x03ffffffU);
    }
}

} // namespace s2
} // namespace shenmue

#endif
