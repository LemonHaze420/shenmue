#pragma once

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;





struct MoveShadowSettings {
    std::uint32_t flags;
    std::int32_t vertexCapacity;
    std::int32_t edgeCapacity;
    std::int32_t faceCapacity;
};

static_assert(sizeof(MoveShadowSettings) == 0x10);

// The volume DrawShadowForEntity draws (a caster's +0x14): its view-transformed vertices, edges (two vertex indices) and faces.
struct ShadowVolume {
    std::int32_t vertices;
    std::int32_t edges;
    std::int32_t faces;
    std::int16_t vertexCount;
    std::int16_t edgeCount;
    std::int16_t faceCount;
};

// One SYD_MUNE.MVS caster (0x28 bytes in the file): the model node it follows (the HMDL node kind; for arm casters the
// g_shadowMatrixIndices index, -65 selecting matrix 0), its children and siblings, its source vertices and face normals.
struct ShadowCaster {
    std::int16_t nodeId;
    std::int16_t reserved02;
    std::int32_t child;
    std::int32_t sibling;
    std::int32_t sourceVertices;
    std::int32_t sourceNormals;
    ShadowVolume volume;
};

struct ShadowEdge {
    std::int16_t vertices[2];
};

// A face: its transformed normal, its three vertices and its three edges (edge n joins vertex n and vertex n + 1).
struct ShadowFace {
    Vector3 normal;
    std::int16_t vertices[3];
    std::int16_t edges[3];
};

// The SYD_MUNE.MVS header ('MVSD'): relocated is set once MoveShadow_RelocateCasters has rebased the file.
struct MoveShadowFile {
    std::uint32_t magic;
    std::int16_t reserved04;
    std::int16_t relocated;
    std::int32_t root;
};

// One MVSW scratch point: a view-clip position and whether it lies in front of the 0.1 near plane (clipped = 1 when not).
struct ShadowVertex {
    Vector3 position;
    std::int32_t clipped;
};


struct MoveShadowFileEntry {
    MoveShadowFile *file;
    char name[16];
};



struct ShadowModifierVertex {
    std::uint32_t parameterControlWord;
    Vector3 positions[3];
};

static_assert(sizeof(ShadowVolume) == 0x14);
static_assert(offsetof(ShadowVolume, vertexCount) == 0x0c);
static_assert(offsetof(ShadowVolume, faceCount) == 0x10);
static_assert(sizeof(ShadowCaster) == 0x28);
static_assert(offsetof(ShadowCaster, child) == 0x04);
static_assert(offsetof(ShadowCaster, sourceVertices) == 0x0c);
static_assert(offsetof(ShadowCaster, sourceNormals) == 0x10);
static_assert(offsetof(ShadowCaster, volume) == 0x14);
static_assert(offsetof(ShadowCaster, volume) + offsetof(ShadowVolume, vertexCount) == 0x20);
static_assert(sizeof(ShadowFace) == 0x18);
static_assert(offsetof(ShadowFace, vertices) == 0x0c);
static_assert(offsetof(ShadowFace, edges) == 0x12);
static_assert(sizeof(MoveShadowFile) == 0x0c);
static_assert(offsetof(MoveShadowFile, relocated) == 0x06);
static_assert(sizeof(ShadowVertex) == 0x10);
static_assert(sizeof(MoveShadowFileEntry) == 0x18);
static_assert(sizeof(ShadowModifierVertex) == 0x28);

using ShadowCapEmitter = void (*)(ShadowVertex *vertices, const std::int16_t *faceVertices, ShadowVertex *edgePoints, const std::int16_t *faceEdges);

inline constexpr std::int32_t kShadowTriangleVertexCapacity = 2048;
inline constexpr std::int32_t kShadowVideoBufferVertexCapacity = 524288;

extern MoveShadowSettings g_moveShadow;
extern std::int32_t g_moveShadowFileCount;
extern std::array<MoveShadowFileEntry, 8> g_moveShadowFiles;
extern ShadowCaster *g_shadowCasters;
extern ShadowCaster *g_armShadowCasters;
extern float g_shadowLength;
extern Vector3 g_shadowLightDirection;
extern Vector3 g_shadowLightPosition;
extern Vector3 g_shadowDirection;
extern Vector3 g_shadowExtrusion;
extern ShadowVolume *g_shadowCasterList[2];
extern ShadowVertex *g_shadowVertices;
extern ShadowVertex *g_shadowExtrudedVertices;
extern ShadowVertex *g_shadowNearPlaneVertices;
extern ShadowVertex *g_shadowEdgeNearPoints;
extern ShadowVertex *g_shadowEdgeFarPoints;
extern Vector3 *g_shadowFaceLightVectors;
extern std::uint8_t *g_shadowEdgeFlags;
extern std::array<std::uint32_t, 50> g_shadowVolumeStripContext;
extern std::array<std::uint32_t, 50> g_shadowVolumeLastStripContext;
extern ShadowModifierVertex g_shadowModifierVertex;
extern Vector3 g_shadowTriangle[3];
extern std::array<Vector3, kShadowTriangleVertexCapacity> g_shadowTriangleVertices;
extern std::int32_t g_shadowTriangleVertexCount;
extern std::int32_t g_shadowVolumeHasVertices;
extern std::int32_t g_shadowVideoBufferVertex;
extern std::uint64_t g_shadowVideoBufferFrame;
extern std::int32_t g_shadowVideoBuffer;
extern std::uint8_t g_shadowCapReversed;
extern const std::array<ShadowCapEmitter, 8> g_shadowCapEmitters;
extern const std::array<std::int8_t, 40> g_shadowMatrixIndices;

void SetupMoveShadow();
void SetupShadowVB(void *scratch, const MoveShadowSettings *settings);
void UpdateStencilShadowVolumesForParts(const Vector3 &worldPosition, void *model, void *reserved, Character *lightOverride);
void EvaluateShadowSourcesForEntity(Character *character);
void DoShadowPass();

} // namespace shenmue::s1
