#include "s1/shadow_volume.hpp"

#include "d3t/texture.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/file_loader.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/hlib.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/char.hpp"
#include "s1/task/pack.hpp"

#include <bit>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <d3d11.h>
#endif

#if defined(_WIN32) && defined(NDEBUG)
extern "C" _ACRTIMP void __cdecl _wassert(const wchar_t *message, const wchar_t *file, unsigned line);
#endif

namespace shenmue::s1 {

extern void *g_mobjData;

MoveShadowSettings g_moveShadow{};
std::int32_t g_moveShadowFileCount = 0;
std::array<MoveShadowFileEntry, 8> g_moveShadowFiles{};
ShadowCaster *g_shadowCasters = nullptr;
ShadowCaster *g_armShadowCasters = nullptr;
float g_shadowLength = 0.0f;
Vector3 g_shadowLightDirection{};
Vector3 g_shadowLightPosition{};
Vector3 g_shadowDirection{};
Vector3 g_shadowExtrusion{};
ShadowVolume *g_shadowCasterList[2]{};
ShadowVertex *g_shadowVertices = nullptr;
ShadowVertex *g_shadowExtrudedVertices = nullptr;
ShadowVertex *g_shadowNearPlaneVertices = nullptr;
ShadowVertex *g_shadowEdgeNearPoints = nullptr;
ShadowVertex *g_shadowEdgeFarPoints = nullptr;
Vector3 *g_shadowFaceLightVectors = nullptr;
std::uint8_t *g_shadowEdgeFlags = nullptr;
std::array<std::uint32_t, 50> g_shadowVolumeStripContext{};
std::array<std::uint32_t, 50> g_shadowVolumeLastStripContext{};
ShadowModifierVertex g_shadowModifierVertex{};
Vector3 g_shadowTriangle[3]{};
std::array<Vector3, kShadowTriangleVertexCapacity> g_shadowTriangleVertices{};
std::int32_t g_shadowTriangleVertexCount = 0;
std::int32_t g_shadowVolumeHasVertices = 0;
std::int32_t g_shadowVideoBufferVertex = 0;
std::uint64_t g_shadowVideoBufferFrame = 0;
std::int32_t g_shadowVideoBuffer = 0;
std::uint8_t g_shadowCapReversed = 0;
const std::array<std::int8_t, 40> g_shadowMatrixIndices{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

namespace {


constexpr MoveShadowSettings kMoveShadowDefaults{.flags = 0, .vertexCapacity = 1000, .edgeCapacity = 1000, .faceCapacity = 1000};
constexpr std::int32_t kMoveShadowScratchBlockSize = 0x17b40;
constexpr float kShadowNearPlane = 0.1f;
constexpr std::uint32_t kSvVertexShaderSlot = 13;
constexpr std::uint32_t kShadowProjectionPixelShaderSlot = 23;
constexpr std::uint32_t kPositionInputLayout = 13;

template <typename T> T *TaskDataAt(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + offset) : nullptr;
}

// The body of mvshadow.c's offset setters: a pointer stored relative to g_pCurrentTaskData, asserting that it fits.
std::int32_t MoveShadowTaskOffset(const std::byte *pointer, unsigned line) {
    if (!pointer)
        return 0;
    const std::ptrdiff_t offset = pointer - g_pCurrentTaskData;
    if (static_cast<std::uint64_t>(offset + 0x80000000LL) > 0xffffffffULL) {
#if defined(_WIN32)
        ::_wassert(L"((d3tSint64)offset <= INT_MAX && (d3tSint64)offset >= INT_MIN)", L"src\\mvshadow.c", line);
#else
        std::fprintf(stderr, "Assertion failed: ((d3tSint64)offset <= INT_MAX && (d3tSint64)offset >= INT_MIN), file src\\mvshadow.c, line %u\n", line);
        std::abort();
#endif
    }
    return static_cast<std::int32_t>(offset);
}


void MoveShadow_SetRootCaster(MoveShadowFile *file, const std::byte *caster) {
    file->root = MoveShadowTaskOffset(caster, 0x2c5);
}


void MoveShadowCaster_SetChild(ShadowCaster *caster, const std::byte *child) {
    caster->child = MoveShadowTaskOffset(child, 0x2c6);
}


void MoveShadowCaster_SetSourceVertices(ShadowCaster *caster, const std::byte *vertices) {
    caster->sourceVertices = MoveShadowTaskOffset(vertices, 0x2c8);
}


void MoveShadowCaster_SetSourceNormals(ShadowCaster *caster, const std::byte *normals) {
    caster->sourceNormals = MoveShadowTaskOffset(normals, 0x2c9);
}


void ShadowVolume_SetVertices(ShadowVolume *volume, const std::byte *vertices) {
    volume->vertices = MoveShadowTaskOffset(vertices, 0x2cb);
}


void ShadowVolume_SetEdges(ShadowVolume *volume, const std::byte *edges) {
    volume->edges = MoveShadowTaskOffset(edges, 0x2cc);
}


void ShadowVolume_SetFaces(ShadowVolume *volume, const std::byte *faces) {
    volume->faces = MoveShadowTaskOffset(faces, 0x2cd);
}



void MoveShadow_RelocateCasters(ShadowCaster *caster, std::byte *file) {
    for (;;) {
        MoveShadowCaster_SetSourceVertices(caster, file + caster->sourceVertices);
        MoveShadowCaster_SetSourceNormals(caster, file + caster->sourceNormals);
        ShadowVolume_SetVertices(&caster->volume, file + caster->volume.vertices);
        ShadowVolume_SetEdges(&caster->volume, file + caster->volume.edges);
        ShadowVolume_SetFaces(&caster->volume, file + caster->volume.faces);
        if (caster->child) {
            MoveShadowCaster_SetChild(caster, file + caster->child);
            MoveShadow_RelocateCasters(TaskDataAt<ShadowCaster>(caster->child), file);
        }
        if (!caster->sibling)
            return;
        caster->sibling = MoveShadowTaskOffset(file + caster->sibling, 0x2c7);
        caster = TaskDataAt<ShadowCaster>(caster->sibling);
    }
}



ShadowCaster *MoveShadow_LoadCasters() {
    MoveShadowFile *file = nullptr;
    char filename[16];
    char directory[16];
    std::sprintf(directory, "misc");
    std::sprintf(filename, "%s.MVS", "SYD_MUNE");
    std::int32_t count = g_moveShadowFileCount;
    const std::int32_t cached = count > 8 ? 8 : count;
    std::int32_t slot = 0;
    for (; slot < cached; ++slot) {
        if (std::strcmp(g_moveShadowFiles[slot].name, filename) == 0)
            file = g_moveShadowFiles[slot].file;
    }
    if (!file) {
        const auto first = static_cast<char>(g_moveShadowFileCount / 10 + '0');
        const auto second = static_cast<char>(g_moveShadowFileCount % 10 + '0');
        const auto token = static_cast<std::int32_t>(make_shen_name('M', 'V', first, second));
        if (std::strcmp("SYD_MUNE", "SYD_MUNE") == 0) {
            const std::byte *const table = static_cast<const std::byte *>(g_mobjData) + 0x10;
            std::int32_t selector = 0;
            std::int32_t offset = 0;
            std::int32_t size = 0;
            std::memcpy(&selector, table + 0xc, sizeof(selector));
            std::memcpy(&offset, table + selector + 0x98, sizeof(offset));
            std::memcpy(&size, table + selector + 0x9c, sizeof(size));
            file = static_cast<MoveShadowFile *>(HLib_AllocHeapMemBlock(size, token));
            std::memcpy(file, table + offset, static_cast<std::size_t>(size));
        } else {
            file = static_cast<MoveShadowFile *>(ReadFile(directory, filename, token));
            count = g_moveShadowFileCount;
        }
        if (file)
            (reinterpret_cast<HLib::TaskData *>(file) - 1)->alignment |= 1;
        if (slot < 8) {
            g_moveShadowFiles[slot].file = file;
            std::strcpy(g_moveShadowFiles[slot].name, filename);
        }
        g_moveShadowFileCount = count + 1;
    }
    if (!file->relocated) {
        auto *const bytes = reinterpret_cast<std::byte *>(file);
        MoveShadow_SetRootCaster(file, bytes + file->root);
        MoveShadow_RelocateCasters(TaskDataAt<ShadowCaster>(file->root), bytes);
        file->relocated = 1;
    }
    return TaskDataAt<ShadowCaster>(file->root);
}



void AddVertices(const ShadowVertex *a, const ShadowVertex *b, const ShadowVertex *c) {
    g_shadowModifierVertex.positions[0] = a->position;
    g_shadowModifierVertex.positions[1] = b->position;
    g_shadowModifierVertex.positions[2] = c->position;
    std::int32_t first = 0;
    if (g_shadowVolumeHasVertices)
        first = g_shadowTriangleVertexCount;
    else
        g_shadowVolumeHasVertices = 1;
    g_shadowTriangle[0] = a->position;
    g_shadowTriangle[1] = b->position;
    g_shadowTriangle[2] = c->position;
    if (first + 3 > kShadowTriangleVertexCapacity) {
        g_shadowTriangleVertexCount = 0;
        return;
    }
    g_shadowTriangleVertexCount = first + 3;
    g_shadowTriangleVertices[first] = g_shadowTriangle[0];
    g_shadowTriangleVertices[first + 1] = g_shadowTriangle[1];
    g_shadowTriangleVertices[first + 2] = g_shadowTriangle[2];
}

// g_shadowCapEmitters, indexed (clip0 << 2) | (clip1 << 1) | clip2: a face cap from the vertices on the near side of the 0.1 plane
// and the face's edge intersections with it. g_shadowCapReversed (set for the near cap) reverses the winding.


void AddShadowCap(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *, const std::int16_t *) {
    if (g_shadowCapReversed)
        AddVertices(&vertices[faceVertices[2]], &vertices[faceVertices[1]], &vertices[faceVertices[0]]);
    else
        AddVertices(&vertices[faceVertices[0]], &vertices[faceVertices[1]], &vertices[faceVertices[2]]);
}


void AddShadowCapWithThirdVertexClipped(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed) {
        AddVertices(&edgePoints[faceEdges[1]], &vertices[faceVertices[1]], &vertices[faceVertices[0]]);
        AddVertices(&edgePoints[faceEdges[1]], &vertices[faceVertices[0]], &edgePoints[faceEdges[2]]);
    } else {
        AddVertices(&edgePoints[faceEdges[2]], &vertices[faceVertices[0]], &vertices[faceVertices[1]]);
        AddVertices(&edgePoints[faceEdges[2]], &vertices[faceVertices[1]], &edgePoints[faceEdges[1]]);
    }
}


void AddShadowCapWithSecondVertexClipped(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed) {
        AddVertices(&edgePoints[faceEdges[0]], &vertices[faceVertices[0]], &vertices[faceVertices[2]]);
        AddVertices(&edgePoints[faceEdges[0]], &vertices[faceVertices[2]], &edgePoints[faceEdges[1]]);
    } else {
        AddVertices(&edgePoints[faceEdges[1]], &vertices[faceVertices[2]], &vertices[faceVertices[0]]);
        AddVertices(&edgePoints[faceEdges[1]], &vertices[faceVertices[0]], &edgePoints[faceEdges[0]]);
    }
}


void AddShadowCapWithLastTwoVerticesClipped(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed)
        AddVertices(&edgePoints[faceEdges[2]], &edgePoints[faceEdges[0]], &vertices[faceVertices[0]]);
    else
        AddVertices(&vertices[faceVertices[0]], &edgePoints[faceEdges[0]], &edgePoints[faceEdges[2]]);
}


void AddShadowCapWithFirstVertexClipped(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed) {
        AddVertices(&edgePoints[faceEdges[2]], &vertices[faceVertices[2]], &vertices[faceVertices[1]]);
        AddVertices(&edgePoints[faceEdges[2]], &vertices[faceVertices[1]], &edgePoints[faceEdges[0]]);
    } else {
        AddVertices(&edgePoints[faceEdges[0]], &vertices[faceVertices[1]], &vertices[faceVertices[2]]);
        AddVertices(&edgePoints[faceEdges[0]], &vertices[faceVertices[2]], &edgePoints[faceEdges[2]]);
    }
}


void AddShadowCapWithOuterVerticesClipped(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed)
        AddVertices(&edgePoints[faceEdges[0]], &edgePoints[faceEdges[1]], &vertices[faceVertices[1]]);
    else
        AddVertices(&vertices[faceVertices[1]], &edgePoints[faceEdges[1]], &edgePoints[faceEdges[0]]);
}


void AddShadowCapWithFirstTwoVerticesClipped(
    ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges) {
    if (g_shadowCapReversed)
        AddVertices(&edgePoints[faceEdges[1]], &edgePoints[faceEdges[2]], &vertices[faceVertices[2]]);
    else
        AddVertices(&vertices[faceVertices[2]], &edgePoints[faceEdges[2]], &edgePoints[faceEdges[1]]);
}


void Script_NoOp(ShadowVertex *, const std::int16_t *, ShadowVertex *, const std::int16_t *) {
}

std::int32_t ShadowClipType(const ShadowVertex *vertices, const std::int16_t *faceVertices) {
    return (((vertices[faceVertices[0]].clipped << 1) | vertices[faceVertices[1]].clipped) << 1) | vertices[faceVertices[2]].clipped;
}



void AddShadowVerticesForEdgeTypes(std::int32_t nearType, std::int32_t farType, const std::int16_t *faceVertices, const std::int16_t *faceEdges) {
    ShadowVertex *const nearPlane = g_shadowNearPlaneVertices;
    if (nearType == 0 || nearType == 7) {
        if (nearType == 7 && farType == 7)
            return;
        if (farType == (nearType == 0 ? 7 : 0)) {
            AddVertices(&nearPlane[faceVertices[0]], &nearPlane[faceVertices[1]], &nearPlane[faceVertices[2]]);
            return;
        }
        g_shadowCapEmitters[ShadowClipType(nearPlane, faceVertices)](nearPlane, faceVertices, g_shadowEdgeFarPoints, faceEdges);
        return;
    }
    if (farType == 0 || farType == 7) {
        g_shadowCapEmitters[ShadowClipType(nearPlane, faceVertices)](nearPlane, faceVertices, g_shadowEdgeNearPoints, faceEdges);
        return;
    }
    ShadowVertex *const nearPoints = g_shadowEdgeNearPoints;
    ShadowVertex *const farPoints = g_shadowEdgeFarPoints;
    if (nearType == farType) {
        std::int16_t first = 0;
        std::int16_t second = 0;
        switch (nearType) {
        case 1:
        case 6:
            first = faceEdges[2];
            second = faceEdges[1];
            break;
        case 2:
        case 5:
            first = faceEdges[1];
            second = faceEdges[0];
            break;
        case 3:
        case 4:
            first = faceEdges[0];
            second = faceEdges[2];
            break;
        default:
            return;
        }
        AddVertices(&nearPoints[first], &nearPoints[second], &farPoints[second]);
        AddVertices(&nearPoints[first], &farPoints[second], &farPoints[first]);
        return;
    }
    if ((nearType & 4) != 0) {
        nearType = ~nearType & 7;
        farType = ~farType & 7;
    }
    std::int16_t edge = 0;
    std::int16_t vertex = 0;
    std::int16_t otherEdge = 0;
    const ShadowVertex *farCorner = nullptr;
    switch (nearType) {
    case 1:
        farCorner = &farPoints[faceEdges[0]];
        if (farType == 3) {
            edge = faceEdges[2];
            vertex = faceVertices[1];
            otherEdge = faceEdges[1];
        } else {
            edge = faceEdges[1];
            vertex = faceVertices[0];
            otherEdge = faceEdges[2];
        }
        break;
    case 2:
        farCorner = &farPoints[faceEdges[2]];
        if (farType == 3) {
            edge = faceEdges[0];
            vertex = faceVertices[2];
            otherEdge = faceEdges[1];
        } else {
            edge = faceEdges[1];
            vertex = faceVertices[0];
            otherEdge = faceEdges[0];
        }
        break;
    case 3:
        farCorner = &farPoints[faceEdges[1]];
        if (farType == 2) {
            edge = faceEdges[0];
            vertex = faceVertices[2];
            otherEdge = faceEdges[2];
        } else {
            edge = faceEdges[2];
            vertex = faceVertices[1];
            otherEdge = faceEdges[0];
        }
        break;
    default:
        return;
    }
    AddVertices(&nearPoints[edge], &nearPoints[otherEdge], &nearPlane[vertex]);
    AddVertices(&nearPoints[edge], &nearPlane[vertex], farCorner);
    AddVertices(&nearPoints[edge], farCorner, &farPoints[edge]);
}

// The near-plane intersection PrepareShadowVerts records for an edge whose two points lie on opposite sides of the 0.1 plane
// (clipped = 1 when they do not).
void IntersectShadowEdge(const ShadowVertex &first, const ShadowVertex &second, ShadowVertex &point) {
    if (first.clipped == second.clipped) {
        point.clipped = 1;
        return;
    }
    const float t = (kShadowNearPlane - second.position.z) * (1.0f / (first.position.z - second.position.z));
    point.position.z = kShadowNearPlane;
    point.clipped = 0;
    point.position.x = (first.position.x - second.position.x) * t + second.position.x;
    point.position.y = (first.position.y - second.position.y) * t + second.position.y;
}




void PrepareShadowVerts(const ShadowFace *faces, std::int32_t faceCount, const ShadowEdge *edges) {
    const bool pointLight = (g_moveShadow.flags & 1U) != 0;
    const Vector3 *faceLight = g_shadowFaceLightVectors;
    for (std::int32_t remaining = faceCount; remaining > 0; --remaining, ++faces, ++faceLight) {
        const Vector3 &light = pointLight ? *faceLight : g_shadowDirection;
        const ShadowFace &face = *faces;
        if (!(0.0f > (face.normal.y * light.y + face.normal.x * light.x) + face.normal.z * light.z))
            continue;
        const std::int16_t pairs[3][2]{{face.vertices[0], face.vertices[1]}, {face.vertices[1], face.vertices[2]}, {face.vertices[2], face.vertices[0]}};
        for (const std::int16_t edgeIndex : face.edges) {
            std::uint8_t &flag = g_shadowEdgeFlags[edgeIndex];
            if (flag) {
                flag = 3;
                continue;
            }
            const ShadowEdge &edge = edges[edgeIndex];
            for (const auto &pair : pairs) {
                if (pair[0] == edge.vertices[0] && pair[1] == edge.vertices[1])
                    flag = 1;
                else if (pair[1] == edge.vertices[0] && pair[0] == edge.vertices[1])
                    flag = 2;
            }
            IntersectShadowEdge(g_shadowVertices[edge.vertices[0]], g_shadowVertices[edge.vertices[1]], g_shadowEdgeNearPoints[edgeIndex]);
            IntersectShadowEdge(g_shadowExtrudedVertices[edge.vertices[0]], g_shadowExtrudedVertices[edge.vertices[1]], g_shadowEdgeFarPoints[edgeIndex]);
        }
        g_shadowCapReversed = 1;
        const std::int32_t nearType = ShadowClipType(g_shadowVertices, face.vertices);
        g_shadowCapEmitters[nearType](g_shadowVertices, face.vertices, g_shadowEdgeNearPoints, face.edges);
        g_shadowCapReversed = 0;
        const std::int32_t farType = ShadowClipType(g_shadowExtrudedVertices, face.vertices);
        g_shadowCapEmitters[farType](g_shadowExtrudedVertices, face.vertices, g_shadowEdgeFarPoints, face.edges);
        if (nearType | farType)
            AddShadowVerticesForEdgeTypes(nearType, farType, face.vertices, face.edges);
    }
}



void AddShadowVerts(const ShadowEdge *edges, std::int32_t edgeCount) {
    std::int32_t firsts[256];
    std::int32_t seconds[256];
    std::int32_t silhouetteCount = 0;
    const std::uint8_t *flag = g_shadowEdgeFlags;
    for (std::int32_t index = 0; index < edgeCount; ++index, ++flag) {
        if (*flag == 1) {
            firsts[silhouetteCount] = edges[index].vertices[0];
            seconds[silhouetteCount++] = edges[index].vertices[1];
        } else if (*flag == 2) {
            firsts[silhouetteCount] = edges[index].vertices[1];
            seconds[silhouetteCount++] = edges[index].vertices[0];
        }
    }
    for (std::int32_t index = 0; index < silhouetteCount; ++index) {
        const std::int32_t a = firsts[index];
        const std::int32_t b = seconds[index];
        const ShadowVertex *corners[6]{};
        std::int32_t count = 0;
        const std::int32_t nearA = g_shadowVertices[a].clipped;
        std::int32_t side = nearA;
        if (!nearA)
            corners[count++] = &g_shadowVertices[a];
        const std::int32_t nearB = g_shadowVertices[b].clipped;
        if (nearA != nearB) {
            corners[count++] = &g_shadowVertices[b];
            side = nearB;
        }
        if (!nearB)
            corners[count++] = &g_shadowVertices[b];
        const std::int32_t farB = g_shadowExtrudedVertices[b].clipped;
        if (side != farB) {
            corners[count++] = &g_shadowNearPlaneVertices[b];
            side = farB;
        }
        if (!farB)
            corners[count++] = &g_shadowExtrudedVertices[b];
        const std::int32_t farA = g_shadowExtrudedVertices[a].clipped;
        if (side != farA) {
            corners[count++] = &g_shadowExtrudedVertices[b];
            side = farA;
        }
        if (!farA)
            corners[count++] = &g_shadowExtrudedVertices[a];
        if (side != nearA)
            corners[count++] = &g_shadowNearPlaneVertices[a];
        if (!count)
            continue;
        AddVertices(corners[0], corners[1], corners[2]);
        if (count == 3)
            continue;
        AddVertices(corners[0], corners[2], corners[3]);
        if (count != 4)
            AddVertices(corners[0], corners[3], corners[4]);
    }
}

// The far point and near-plane point TranslateShadow and TranslateShadowFromLightPoint derive from a near point.
void ExtrudeShadowVertex(std::int32_t index) {
    ShadowVertex &nearVertex = g_shadowVertices[index];
    ShadowVertex &farVertex = g_shadowExtrudedVertices[index];
    ShadowVertex &cut = g_shadowNearPlaneVertices[index];
    farVertex.position.x = g_shadowExtrusion.x + nearVertex.position.x;
    farVertex.position.y = g_shadowExtrusion.y + nearVertex.position.y;
    farVertex.position.z = g_shadowExtrusion.z + nearVertex.position.z;
    nearVertex.clipped = 0;
    farVertex.clipped = 0;
    if (kShadowNearPlane > nearVertex.position.z)
        nearVertex.clipped = 1;
    if (kShadowNearPlane > farVertex.position.z)
        farVertex.clipped = 1;
    IntersectShadowEdge(nearVertex, farVertex, cut);
}



void TranslateShadow(const Vector3 *vertices, std::int32_t vertexCount) {
    for (std::int32_t index = 0; index < vertexCount; ++index) {
        Matrix4x4::TransformPoint(vertices[index], g_shadowVertices[index].position);
        ExtrudeShadowVertex(index);
    }
}


void TranslateShadowFromLightPoint(const Vector3 *vertices, std::int32_t vertexCount) {
    const float length = g_shadowLength;
    for (std::int32_t index = 0; index < vertexCount; ++index) {
        const Vector3 &vertex = vertices[index];
        const float x = vertex.x - g_shadowLightPosition.x;
        g_shadowExtrusion.x = x;
        const float y = vertex.y - g_shadowLightPosition.y;
        g_shadowExtrusion.y = y;
        const float z = vertex.z - g_shadowLightPosition.z;
        const float scale = length / std::sqrt((x * x + y * y) + z * z);
        g_shadowExtrusion = {x * scale, y * scale, z * scale};
        Matrix4x4::Transform(g_shadowExtrusion, g_shadowExtrusion);
        Matrix4x4::TransformPoint(vertex, g_shadowVertices[index].position);
        ExtrudeShadowVertex(index);
    }
}


void SetCurrentRenderFlag(std::uint32_t setting, std::int32_t value) {
    SetRenderFlags(setting, value);
    g_currentRenderFlags[setting] = value;
}

// The D3T draw DoShadowPass inlines for each of its four passes.
void DrawShadowTriangles(std::uint32_t startVertex) {
#if defined(_WIN32)
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    const auto vertexCount = static_cast<std::uint32_t>(g_shadowTriangleVertexCount);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(vertexCount, startVertex);
#else
    static_cast<void>(startVertex);
#endif
}


void DrawShadowForEntity(const ShadowVolume *volume) {
    std::int32_t flagWords = volume->edgeCount >> 2;
    if ((volume->edgeCount & 3) != 0)
        ++flagWords;
    if (flagWords > 0)
        std::memset(g_shadowEdgeFlags, 0, 4 * static_cast<std::size_t>(flagWords));
    if ((g_moveShadow.flags & 1U) == 0) {
        TranslateShadow(TaskDataAt<Vector3>(volume->vertices), volume->vertexCount);
    } else {
        const Vector3 *const vertices = TaskDataAt<Vector3>(volume->vertices);
        const ShadowFace *const faces = TaskDataAt<ShadowFace>(volume->faces);
        const Vector3 light = g_shadowLightPosition;
        for (std::int32_t face = 0; face < volume->faceCount; ++face) {
            const Vector3 &vertex = vertices[faces[face].vertices[0]];
            g_shadowFaceLightVectors[face] = {light.x - vertex.x, light.y - vertex.y, light.z - vertex.z};
        }
        TranslateShadowFromLightPoint(vertices, volume->vertexCount);
    }
    PrepareShadowVerts(TaskDataAt<ShadowFace>(volume->faces), volume->faceCount, TaskDataAt<ShadowEdge>(volume->edges));
    AddShadowVerts(TaskDataAt<ShadowEdge>(volume->edges), volume->edgeCount);
    DoShadowPass();
}




void RenderShadowCasters() {
    Matrix4x4_Push();
    g_shadowDirection = g_shadowLightDirection;
    Matrix4x4::RotateMatrixCounterClockwise90();
    const Matrix4x4 &transposed = workTransformMatrix;
    const Vector3 light = g_shadowDirection;
    g_shadowDirection = {(transposed.rows[0].x * light.x + transposed.rows[1].x * light.y) + transposed.rows[2].x * light.z,
        (transposed.rows[0].y * light.x + transposed.rows[1].y * light.y) + transposed.rows[2].y * light.z,
        (transposed.rows[0].z * light.x + transposed.rows[1].z * light.y) + transposed.rows[2].z * light.z};
    Matrix4x4::RotateMatrixCounterClockwise90();
    if ((g_moveShadow.flags & 1U) != 0) {
        Matrix4x4_Push();
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::TransformPoint(g_shadowLightDirection, g_shadowLightPosition);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_Push(g_matrixStack.entries[2].stack);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    Matrix4x4 clip = workTransformMatrix;
    for (Vector4 &row : clip.rows)
        row.z = row.w;
    Matrix4x4::Pop(1);
    workTransformMatrix = clip;
    const float depthRange = g_tempFarClip - g_tempNearClip;
    const Matrix4x4 projection{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, g_tempNearClip / depthRange, 1.0f},
        {0.0f, 0.0f, (g_tempFarClip * g_tempNearClip) / (depthRange * 0.4f), 0.0f}}};
    SetActiveStack(MatrixStackType::Transparent);
    Matrix4x4_Push(&projection);
    Matrix4x4::RotateMatrixCounterClockwise90();
    const Matrix4x4 *const top = Matrix4x4::GetTransformMatrix();
    g_renderParams.constants[0] = top->rows[0];
    g_renderParams.constants[1] = top->rows[1];
    g_renderParams.constants[2] = top->rows[2];
    g_renderParams.constants[3] = top->rows[3];
    Matrix4x4::Pop(1);
    SetActiveStack(MatrixStackType::DepthPrepass);
    if ((g_moveShadow.flags & 1U) == 0) {
        g_shadowExtrusion = {g_shadowDirection.x * -g_shadowLength, g_shadowDirection.y * -g_shadowLength, g_shadowDirection.z * -g_shadowLength};
        Matrix4x4::Transform(g_shadowExtrusion, g_shadowExtrusion);
    }
    for (ShadowVolume *const *caster = g_shadowCasterList; *caster; ++caster) {
        g_shadowVolumeHasVertices = 0;
        DrawShadowForEntity(*caster);
    }
    Matrix4x4::Pop(1);
}



void BuildShadowCasterVB(Mt5RuntimeNode *model, ShadowCaster *caster) {
    for (;;) {
        const Vector3 *source = TaskDataAt<Vector3>(caster->sourceVertices);
        Vector3 *const vertices = TaskDataAt<Vector3>(caster->volume.vertices);
        const Vector3 *normals = TaskDataAt<Vector3>(caster->sourceNormals);
        ShadowFace *const faces = TaskDataAt<ShadowFace>(caster->volume.faces);
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, caster->nodeId)) {
            const Matrix4x4 *const transform = HMDL_TransformForNode(node, node->transformOffset);
            workTransformMatrix = transform ? *transform : *g_matrixStack.top.stack;
        }
        for (std::int32_t index = 0; index < caster->volume.vertexCount; ++index)
            Matrix4x4::TransformPoint(source[index], vertices[index]);
        for (std::int32_t index = 0; index < caster->volume.faceCount; ++index)
            Matrix4x4::Transform(normals[index], faces[index].normal);
        workTransformMatrix = g_matrixStack.top.stack[-1];
        g_shadowCasterList[0] = &caster->volume;
        RenderShadowCasters();
        if (ShadowCaster *const child = TaskDataAt<ShadowCaster>(caster->child))
            BuildShadowCasterVB(model, child);
        caster = TaskDataAt<ShadowCaster>(caster->sibling);
        if (!caster)
            return;
    }
}


void UpdateShadowVB(Mt5RuntimeNode *model) {
    g_renderParams.constants[16] = {1.0f, 1.0f, 1.0f, g_shadowBlackness};
    Matrix4x4_Push();
    BuildShadowCasterVB(model, g_shadowCasters);
    Matrix4x4::Pop(1);
}



void Recursive_RenderShadowCasters(Matrix4x4 *matrices, ShadowCaster *caster) {
    for (;;) {
        const Vector3 *source = TaskDataAt<Vector3>(caster->sourceVertices);
        Vector3 *const vertices = TaskDataAt<Vector3>(caster->volume.vertices);
        const Vector3 *normals = TaskDataAt<Vector3>(caster->sourceNormals);
        ShadowFace *const faces = TaskDataAt<ShadowFace>(caster->volume.faces);
        const std::int32_t matrixIndex = caster->nodeId == -65 ? 0 : g_shadowMatrixIndices.data()[caster->nodeId];
        const Matrix4x4 *const matrix = matrices + matrixIndex;
        workTransformMatrix = matrix ? *matrix : *g_matrixStack.top.stack;
        if (!matrixIndex)
            Matrix4x4::RotateAroundZ(sin_fixed16(0x4000), sin_fixed16(0));
        for (std::int32_t index = 0; index < caster->volume.vertexCount; ++index)
            Matrix4x4::TransformPoint(source[index], vertices[index]);
        for (std::int32_t index = 0; index < caster->volume.faceCount; ++index)
            Matrix4x4::Transform(normals[index], faces[index].normal);
        workTransformMatrix = g_matrixStack.top.stack[-1];
        g_shadowCasterList[0] = &caster->volume;
        RenderShadowCasters();
        if (ShadowCaster *const child = TaskDataAt<ShadowCaster>(caster->child))
            Recursive_RenderShadowCasters(matrices, child);
        caster = TaskDataAt<ShadowCaster>(caster->sibling);
        if (!caster)
            return;
    }
}


void BeginShadowPassForEntity(Matrix4x4 *matrices) {
    Matrix4x4_Push();
    Recursive_RenderShadowCasters(matrices, g_armShadowCasters);
    Matrix4x4::Pop(1);
}

} // namespace

const std::array<ShadowCapEmitter, 8> g_shadowCapEmitters{
    AddShadowCap,
    AddShadowCapWithThirdVertexClipped,
    AddShadowCapWithSecondVertexClipped,
    AddShadowCapWithLastTwoVerticesClipped,
    AddShadowCapWithFirstVertexClipped,
    AddShadowCapWithOuterVerticesClipped,
    AddShadowCapWithFirstTwoVerticesClipped,
    Script_NoOp,
};



void SetupShadowVB(void *scratch, const MoveShadowSettings *settings) {
    const std::int32_t vertexBytes = 16 * settings->vertexCapacity;
    const std::int32_t edgeBytes = 16 * settings->edgeCapacity;
    auto *const base = static_cast<std::byte *>(scratch);
    g_shadowVertices = reinterpret_cast<ShadowVertex *>(base);
    g_shadowExtrudedVertices = reinterpret_cast<ShadowVertex *>(base + vertexBytes);
    std::int32_t offset = vertexBytes + vertexBytes;
    g_shadowNearPlaneVertices = reinterpret_cast<ShadowVertex *>(base + offset);
    offset += vertexBytes;
    g_shadowEdgeNearPoints = reinterpret_cast<ShadowVertex *>(base + offset);
    offset += edgeBytes;
    g_shadowEdgeFarPoints = reinterpret_cast<ShadowVertex *>(base + offset);
    offset += edgeBytes;
    g_shadowFaceLightVectors = reinterpret_cast<Vector3 *>(base + offset);
    g_shadowEdgeFlags = reinterpret_cast<std::uint8_t *>(base + offset + 12 * settings->faceCapacity);
    g_shadowVolumeStripContext.fill(0);
    g_shadowVolumeLastStripContext.fill(0);
    g_shadowVolumeStripContext[0] |= 0x300007U;
    g_shadowVolumeLastStripContext[0] |= 0x300007U;
    g_shadowVolumeStripContext[42] = 0;
    g_shadowVolumeLastStripContext[42] = 0x20000000U;
    g_shadowVolumeStripContext[1] = 1;
    g_shadowVolumeStripContext[2] = 1;
    g_shadowVolumeStripContext[6] = 0;
    g_shadowVolumeStripContext[7] = 0;
    g_shadowVolumeLastStripContext[1] = 1;
    g_shadowVolumeLastStripContext[2] = 1;
    g_shadowVolumeLastStripContext[6] = 0;
    g_shadowVolumeLastStripContext[7] = 0;
    g_shadowModifierVertex.parameterControlWord = 0xf0000000U;
    if (!g_shadowVideoBuffer) {
        const BufferDescription description{.flags = 1, .size = 6291456, .count = 12, .flags3 = 2, .buffer = nullptr, .name = "ShadowVB"};
        g_shadowVideoBuffer = CreateBuffer(&description);
    }
}



void SetupMoveShadow() {
    g_moveShadowFileCount = 0;
    g_shadowCasters = MoveShadow_LoadCasters();
    g_armShadowCasters = nullptr;
    HLib::TaskData *block = g_pFirstTaskData;
    while (block && (block->status == HLIB_USED_TOKEN.name || static_cast<std::uint64_t>(block->content_size) < kMoveShadowScratchBlockSize))
        block = block->next_ptr;
    void *scratch = nullptr;
    if (block) {
        HLib::TaskData *const next = block->next_ptr;
        const auto size = static_cast<std::uint64_t>(block->content_size);
        if (size != kMoveShadowScratchBlockSize && size > kMoveShadowScratchBlockSize + 0x40) {
            auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + kMoveShadowScratchBlockSize);
            remainder->prev_ptr = block;
            remainder->next_ptr = next;
            remainder->status = HLIB_FREE_TOKEN.name;
            remainder->taskToken = HLIB_FREE_TOKEN.name;
            remainder->content_size = static_cast<std::int64_t>(size - kMoveShadowScratchBlockSize);
            block->next_ptr = remainder;
            if (next)
                next->prev_ptr = remainder;
            block->taskToken = static_cast<std::int32_t>(SHEN_NAME(MVSW));
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 0;
            block->content_size = kMoveShadowScratchBlockSize;
        } else {
            block->taskToken = static_cast<std::int32_t>(SHEN_NAME(MVSW));
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 0;
        }
        scratch = block + 1;
    }
    g_moveShadow = kMoveShadowDefaults;
    g_shadowStrength = 127;
    g_shadowBlackness = 0.5f;
    g_shadowFlag = 0;
    SetupShadowVB(scratch, &g_moveShadow);
}





void UpdateStencilShadowVolumesForParts(const Vector3 &worldPosition, void *model, void *, Character *lightOverride) {
    std::uint32_t flags = g_moveShadow.flags & ~1U;
    g_moveShadow.flags = flags;
    std::uint32_t lightMask = g_defaultLightMask;
    float lightRadius = g_defaultLightRadius;
    if (lightOverride) {
        lightMask = lightOverride->lightMask;
        lightRadius = lightOverride->lightRadius;
    }
    std::uint32_t bit = 1;
    for (std::int32_t index = 0; index < 32; ++index, bit = std::rotl(bit, 1)) {
        if ((bit & lightMask) == 0 || static_cast<std::uint32_t>(index) >= 0x80U || g_LightData->slots[index].enabled != 1)
            continue;
        const ShenmueLightSlot &light = g_LightData->slots[index];
        const std::uint32_t type = light.modeFlags & ~8U;
        const Matrix4x4 &view = workTransformMatrix;
        float upward = 0.0f;
        if (type == 2) {
            Vector3 direction = light.direction;
            Vector3::NormalizeInPlace(direction);
            upward = direction.y;
            flags &= ~1U;
            g_moveShadow.flags = flags;
            g_shadowLightDirection = {-((view.rows[0].x * direction.x + view.rows[1].x * direction.y) + view.rows[2].x * direction.z),
                -((view.rows[0].y * direction.x + view.rows[1].y * direction.y) + view.rows[2].y * direction.z),
                -((view.rows[0].z * direction.x + view.rows[1].z * direction.y) + view.rows[2].z * direction.z)};
        } else {
            if (type - 3 > 1)
                continue;
            Vector3 lightPoint = light.position;
            Vector3 direction{lightPoint.x - worldPosition.x, lightPoint.y - worldPosition.y, lightPoint.z - worldPosition.z};
            if (std::sqrt((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z) > lightRadius)
                continue;
            Vector3::NormalizeInPlace(direction);
            upward = direction.y;
            if (g_shadowFlag != 0) {
                Matrix4x4::TransformPoint(lightPoint, lightPoint);
                g_shadowLightDirection = lightPoint;
                flags |= 1U;
                g_moveShadow.flags = flags;
            } else {
                g_shadowLightDirection = {(view.rows[0].x * direction.x + view.rows[1].x * direction.y) + view.rows[2].x * direction.z,
                    (view.rows[0].y * direction.x + view.rows[1].y * direction.y) + view.rows[2].y * direction.z,
                    (view.rows[0].z * direction.x + view.rows[1].z * direction.y) + view.rows[2].z * direction.z};
            }
        }
        if (0.0f > upward) {
            const auto length = static_cast<float>(1.8 / static_cast<double>(-upward));
            g_shadowLength = 50.0f < length ? 50.0f : length;
        } else {
            g_shadowLength = 50.0f;
        }
        UpdateShadowVB(static_cast<Mt5RuntimeNode *>(model));
        flags = g_moveShadow.flags;
    }
}



void EvaluateShadowSourcesForEntity(Character *character) {
    if (!character)
        return;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(HNDR));
    if (!component)
        return;
    auto *const matrices = static_cast<Matrix4x4 *>(reinterpret_cast<HandModelWork *>(component + 1)->jointMatrices);
    if (!matrices)
        return;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    std::uint32_t flags = g_moveShadow.flags & ~1U;
    g_moveShadow.flags = flags;
    g_shadowLength = 50.0f;
    g_shadowStrength = 128;
    std::uint32_t bit = 1;
    for (std::int32_t index = 0; index < 32; ++index, bit = std::rotl(bit, 1)) {
        if ((charGetLightMask(character) & bit) == 0 || static_cast<std::uint32_t>(index) >= 0x80U || g_LightData->slots[index].enabled != 1)
            continue;
        const ShenmueLightSlot &light = g_LightData->slots[index];
        const std::uint32_t type = light.modeFlags & ~8U;
        const Matrix4x4 &view = workTransformMatrix;
        if (type == 2) {
            const Vector3 &d = light.direction;
            Vector3 direction{(view.rows[0].x * d.x + view.rows[1].x * d.y) + view.rows[2].x * d.z,
                (view.rows[0].y * d.x + view.rows[1].y * d.y) + view.rows[2].y * d.z, (view.rows[0].z * d.x + view.rows[1].z * d.y) + view.rows[2].z * d.z};
            Vector3::NormalizeInPlace(direction);
            g_shadowLightDirection = {-direction.x, -direction.y, -direction.z};
            flags &= ~1U;
            g_moveShadow.flags = flags;
        } else {
            if (type - 3 > 1)
                continue;
            const Vector3 d{light.position.x - position.x, light.position.y - position.y, light.position.z - position.z};
            if (std::sqrt((d.x * d.x + d.y * d.y) + d.z * d.z) > charGetLightRadius(character))
                continue;
            Vector3 direction{(view.rows[0].x * d.x + view.rows[1].x * d.y) + view.rows[2].x * d.z,
                (view.rows[0].y * d.x + view.rows[1].y * d.y) + view.rows[2].y * d.z, (view.rows[0].z * d.x + view.rows[1].z * d.y) + view.rows[2].z * d.z};
            Vector3::NormalizeInPlace(direction);
            g_shadowLightDirection = direction;
        }
        BeginShadowPassForEntity(matrices);
        flags = g_moveShadow.flags;
    }
}




void DoShadowPass() {
    if (!g_shadowVolumeHasVertices || g_shadowTriangleVertexCount <= 0)
        return;
    const std::int32_t vertexCount = g_shadowTriangleVertexCount;
    SetCurrentRenderFlag(59, 1);
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::uint32_t vertexShader = g_originalLambVertexShaders[kSvVertexShaderSlot];
    if (state.vertexShader != vertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = 0xffffffffU;
    }
    state.vertexShader = vertexShader;
    if (state.pixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = 0;
    if (state.inputLayout != g_fvfLayouts[kPositionInputLayout]) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = g_fvfLayouts[kPositionInputLayout];
    }
    if (state.topology != 0) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = 0;
    }
    std::int32_t startVertex = g_shadowVideoBufferVertex;
    if (startVertex + vertexCount > kShadowVideoBufferVertexCapacity)
        startVertex = 0;
    g_shadowVideoBufferVertex = startVertex;
    const std::uint64_t frame = d3t::GetNumRenderedFrames();
    if (frame != g_shadowVideoBufferFrame) {
        g_shadowVideoBufferFrame = frame;
        startVertex = 0;
        g_shadowVideoBufferVertex = 0;
    }
    if (void *const mapped = GetWriteMappedBufferPointer(context, g_shadowVideoBuffer, (startVertex != 0) + 1, 12U * static_cast<std::uint32_t>(startVertex)))
        std::memcpy(mapped, g_shadowTriangleVertices.data(), 12 * static_cast<std::size_t>(vertexCount));
    DX11_CopySceneNodeToGPU(context, g_shadowVideoBuffer);
#if defined(_WIN32)
    if (context->resourceCache[0]) {
        static_cast<ID3D11DeviceContext *>(context->deviceContext)->Unmap(reinterpret_cast<ID3D11Resource *>(context->resourceCache[0]), 0);
        context->resourceCache[0] = 0;
    }
#endif
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(g_shadowVideoBuffer))
        state.vertexBufferDirtyMask |= 1U;
    state.vertexBufferHandles[0] = static_cast<std::uint32_t>(g_shadowVideoBuffer);
    g_shadowVideoBufferVertex += g_shadowTriangleVertexCount;
    SetCurrentRenderFlag(64, 0);
    SetCurrentRenderFlag(52, 1);
    SetCurrentRenderFlag(57, 518);
    SetCurrentRenderFlag(56, 517);
    SetCurrentRenderFlag(2, 240);
    SetCurrentRenderFlag(1, 16);
    SetCurrentRenderFlag(53, 7680);
    SetCurrentRenderFlag(55, 7680);
    SetCurrentRenderFlag(54, 7682);
    SetCurrentRenderFlag(147, 2305);
    CommitLegacyScreenRenderState(context, false);
    DrawShadowTriangles(static_cast<std::uint32_t>(startVertex));
    SetCurrentRenderFlag(54, 7683);
    SetCurrentRenderFlag(147, 2304);
    CommitLegacyScreenRenderState(context, false);
    DrawShadowTriangles(static_cast<std::uint32_t>(startVertex));
    SetCurrentRenderFlag(171, 5);
    SetCurrentRenderFlag(62, 770);
    SetCurrentRenderFlag(63, 772);
    SetCurrentRenderFlag(1, 0);
    SetCurrentRenderFlag(2, 15);
    SetCurrentRenderFlag(54, 7680);
    const std::uint32_t pixelShader = g_simplePixelShaders[kShadowProjectionPixelShaderSlot];
    if (state.pixelShader != pixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = pixelShader;
    BindLegacyPixelTexture(context, ResolveStripTextureHandle(d3t::g_defaultWhiteTexture));
    SetCurrentRenderFlag(147, 2305);
    CommitLegacyScreenRenderState(context, false);
    DrawShadowTriangles(static_cast<std::uint32_t>(startVertex));
    SetCurrentRenderFlag(147, 2304);
    CommitLegacyScreenRenderState(context, false);
    DrawShadowTriangles(static_cast<std::uint32_t>(startVertex));
    for (const std::uint32_t setting : {171U, 62U, 63U, 53U, 55U, 56U, 1U, 2U, 54U, 64U, 52U, 57U})
        SetCurrentRenderFlag(setting, g_defaultRenderFlags[setting]);
    g_shadowTriangleVertexCount = 0;
}

} // namespace shenmue::s1
