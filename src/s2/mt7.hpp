#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "s2/legacy_draw.hpp"
#include "s2/face.hpp"

namespace shenmue::s2 {

struct MT7WbtrConstraint {
    Vector3 start;
    float startRadius;
    Vector3 end;
    float endRadius;
};
static_assert(sizeof(MT7WbtrConstraint) == 0x20);
inline constexpr MT7WbtrConstraint mt7WbtrConstraints[]{
    {{-0.128f, -0.052f, -0.005f}, 0.100f, {0.124f, -0.010f, 0.024f}, 0.075f},
    {{-0.080f, -0.059f, 0.000f}, 0.100f, {0.116f, 0.000f, 0.016f}, 0.085f},
    {{-0.062f, 0.107f, -0.133f}, 0.148f, {0.080f, -0.036f, 0.016f}, 0.075f},
    {{-0.057f, 0.024f, 0.075f}, 0.079f, {0.215f, -0.038f, 0.019f}, 0.065f}};
void ClampPointToSegmentRange(Vector3 *point, const Vector3 *start, const Vector3 *end, float startRadius, float endRadius);

struct MT7Header {
    std::uint32_t signature;
    std::uint32_t size;
    std::uint32_t firstNodeOffset;
    std::uint32_t textureCount;
};

static_assert(sizeof(MT7Header) == 0x10);

struct MT7NodeSource {
    std::int32_t id;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::uint32_t xb01Offset;
    std::uint32_t childOffset;
    std::uint32_t siblingOffset;
    std::uint32_t parentOffset;
};

static_assert(sizeof(MT7NodeSource) == 0x38);
static_assert(offsetof(MT7NodeSource, xb01Offset) == 0x28);
static_assert(offsetof(MT7NodeSource, childOffset) == 0x2c);
static_assert(offsetof(MT7NodeSource, parentOffset) == 0x34);

struct MT7TextureEntry {
    std::uint16_t width;
    std::uint16_t height;
    std::uint32_t unknown04;
    std::uint32_t unknown08;
    std::uint32_t textureIndex;
    char id[8];
};

struct MT7Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
    Vector4 colour;
    std::uint32_t ownerNode;
};

struct MT7Surface {
    std::uint32_t ownerNode;
    std::uint32_t textureIndex;
    std::uint32_t firstIndex;
    std::uint32_t indexCount;
    std::uint32_t primitive;
    std::uint16_t wrap;
    bool transparent;
    bool unlit;
};

struct MT7MdcWeightedVertex {
    std::uint32_t destinationByteOffset;
    float weight;
    Vector3 position;
    Vector3 normal;
};
static_assert(sizeof(MT7MdcWeightedVertex) == 0x20);
static_assert(offsetof(MT7MdcWeightedVertex, position) == 0x08);
static_assert(offsetof(MT7MdcWeightedVertex, normal) == 0x14);
inline std::int32_t g_mdcWeightTransformMode{};

struct MT7Node {
    std::uint32_t sourceOffset;
    std::int32_t id;
    std::int32_t parent;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::uint32_t meshVertexOffset;
    std::uint32_t meshVertexCount;
    std::uint16_t meshVertexStride;
    std::uint32_t xb01Offset;
    LegacyDrawCommandStreamStats commandStream;
    LegacyDrawResources drawResources;
    bool visible{true};
    const Matrix4x4 *transformOverride{};

    std::uint32_t meshType{};


    void *taskData{};
    std::int32_t firstChild{-1};
    std::int32_t nextSibling{-1};
    const MT7Node *borrowedMesh{};
    const LegacyNodeCommandOverride *commandOverride{};
    // FFBD node +0x0c is a FACE pointer rather than a translation component.
    FacialModelData *facialData{};


    MT7Node *runtimeChild{};
    MT7Node *runtimeSibling{};
    MT7Node *runtimeParent{};
    // Set only by an admission owner supplying writable source storage; clones borrow it.
    std::byte *runtimeMesh{};
    std::uint32_t modelMarker{};
    std::uint32_t mdcVertexLinkFlags{};
    Matrix4x4 mdcWeightMatrix{};
    std::span<const MT7MdcWeightedVertex> mdcWeightedVertices;
};

struct MT7VertexInfluence {
    std::uint32_t controller;
    float weight;
};

struct MT7WeightedVertex {
    Vector3 position;
    Vector3 normal;
    std::vector<MT7VertexInfluence> influences;
};

struct MT7VertexMapping {
    std::uint32_t sourceVertex;
    std::uint32_t destinationByteOffset;
};

struct MT7WbtrDeformer {
    std::uint32_t nodeIndex;
    std::vector<std::pair<std::uint32_t, Matrix4x4>> bindMatrices;
    std::vector<MT7WeightedVertex> vertices;
    std::vector<MT7VertexMapping> mappings;

    mutable std::vector<std::pair<Vector3, Vector3>> evaluatedVertices;
};

struct MT7MdcVertexLink {
    std::uint32_t nodeIndex;
    std::uint32_t sourceByteOffset;
    std::uint32_t destinationByteOffset;
};

struct MT7Model {
    std::string label;
    std::uint32_t signature{};
    std::int32_t entityType{-1};
    std::span<const std::byte> source;
    const void *vertexMemoryOwner{};
    std::vector<MT7TextureEntry> textures;
    std::vector<MT7Node> nodes;
    std::vector<MT7Vertex> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<MT7Surface> surfaces;
    std::vector<MT7WbtrDeformer> wbtrDeformers;
    std::vector<MT7MdcVertexLink> mdcVertexLinks;
    // ReadModelData's MDP7/MDPX copy map is distinct from MDC's draw-time map.
    std::vector<MT7MdcVertexLink> mdpVertexLinks;
    std::unique_ptr<FacialModelResource> facial;
    // IDs admitted from the file's TXT7 chunk, released with the model.
    std::vector<std::array<std::uint32_t, 2>> embeddedTextureIds;
};
void InitializeMT7RuntimeLinks(MT7Model &model);
void BindMT7WritableRuntimeMeshes(MT7Model &model, std::span<std::byte> sourceAllocation);
bool IsModelXB01(const void *mesh);
void *CopyForDirectX(const void *mesh, std::uint32_t slot, std::uint32_t byteOffset, std::int64_t reserved, std::int32_t preserveContents);
std::int64_t CopyBufferToDirectX_Wrapper(const void *mesh, std::uint32_t slot);
std::uint64_t IsMDPXorMDP7(const MT7Node *node);



MT7Node *mtGetTransform(const MT7Node *node);
MT7Node *mtGetSibling(const MT7Node *node);
std::uint32_t mt7GetXb01Offset(const MT7Node *node);
void mt7_UpdateChildOffset(MT7Node *node, MT7Node *child);
void mt7_UpdateParentOffset(MT7Node *node, MT7Node *parent);
void mt7_UpdateSiblingOffset(MT7Node *node, MT7Node *sibling);
// The node adopts the source node's mesh (a null source mesh clears it).
void mt7_CopyXB01Offset(MT7Node *node, const MT7Node *source);


const MT7Node *GetMT7ReaderData(const MT7Model *model);

void FilepathBuilder(
    const MT7Node *data, const char *directory, const char *subdirectory, const char *stem, std::uint16_t flags, std::uint32_t threadHandle = 0);

bool ReadModelFormat7(std::span<const std::byte> bytes, const char *label, MT7Model &model);

bool ReadModelFormat7(std::span<const std::byte> bytes, const char *label, MT7Model &model, std::uint16_t flags);
void ReleaseModelFormat7(MT7Model &model);
void ReleaseModelFormat7Storage(MT7Model &model);
void AssignFlags(MT7Model *model);
extern Matrix4x4 g_modelNodeTransformBase;
void CaptureModelNodeTransformBase();
void RestoreModelNodeTransformBase();
std::int64_t FindMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *node, const MT7Node *target);
void GetMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *root, const MT7Node *target);
void ApplyMT7NodeTransform(const MT7Node &node);

MT7Node *GetChildObjectByIndex(MT7Model &model, MT7Node *node, std::uint16_t id);


void mtSetUseCustomReadOverride();
void mtResetOverrideFlag();
bool mtIsMeshTypeOverridden(const MT7Model &model, const MT7Node &node);
const LegacyVertexBufferBinding *FindMT7MutableVertexBuffer(const MT7Node &node);
void InterpolateMdcWeightMatrix(float weight, const Matrix4x4 *base, const Matrix4x4 *authored, Matrix4x4 *output);
void InterpolateNormalizedMdcWeightMatrix(float weight, const Matrix4x4 *base, const Matrix4x4 *authored, Matrix4x4 *output);



const Matrix4x4 *SetMT7NodeTransformOverride(MT7Node &node, const Matrix4x4 *transform);
const Matrix4x4 *GetMT7NodeTransformOverride(const MT7Node &node);
void ClearMT7NodeTransformOverrides(MT7Node *root);

} // namespace shenmue::s2
