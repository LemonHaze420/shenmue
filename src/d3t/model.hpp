#pragma once

#include "common/math.hpp"
#include "d3t/asset_remap.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue {


constexpr std::uint32_t kD3TModelMagic = 0x51ee9812;

struct D3TModelFileHeader {
    std::uint32_t magic;
    std::byte reserved_04[0x24];
    std::uint32_t nodeCount;
    std::uint32_t spriteCount;
    std::uint32_t probeCount;
    std::uint32_t materialCount;
    std::uint32_t materialParameterCount;
    std::uint32_t textureCount;
    std::uint32_t drawGroupCount;
    std::uint32_t submeshCount;
    std::uint32_t geometryCount;
    std::uint32_t skinCount;
    std::uint32_t boneCount;
    std::uint32_t referenceCount;
    std::uint32_t partCount;
    std::uint32_t flags;
    std::uint32_t stringsSize;
};

// Node table entry, copied as is into a cached skinning buffer by CreateDXBuffer_Wrapper: the name, the parent distance, subtree size
// and child count of the flattened tree, and the one-based draw group, sprite and probe (zero means none; the sprite and probe indices
// are rebased onto the buffer's own tables).
struct D3TModelNode {
    std::uint32_t nameOffset;
    std::uint16_t parentDistance;
    std::uint16_t subtreeSize;
    std::uint16_t childCount;
    std::uint16_t drawGroup;
    std::uint16_t sprite;
    std::uint16_t probe;
    std::uint32_t flags;
};



struct D3TModelSprite {
    Vector3 colour;
    float parameter0c;
    float range;
    float coneAngle;
    std::uint32_t type;
    std::uint8_t shadowMode;
    std::byte reserved1d[3];
    std::uint32_t textureHandle;
    std::uint32_t texture;
    float shadowBias;
    float parameter2c;
    std::uint32_t parameter30;
    std::uint32_t parameter34;
    std::uint32_t parameter38;
};

// Cube probe: the 52-byte records CreateDXBuffer_Wrapper appends to a skinning buffer's probe table. SetupOwnerProbes names a node
// "Probe <n>" for each and ConfigureEffectSlotAndUpdateFrustums copies the fields into the probe's effect slot.
static_assert(offsetof(D3TModelSprite, type) == 0x18);
static_assert(offsetof(D3TModelSprite, shadowMode) == 0x1c);
static_assert(offsetof(D3TModelSprite, textureHandle) == 0x20);
static_assert(offsetof(D3TModelSprite, shadowBias) == 0x28);
static_assert(offsetof(D3TModelSprite, parameter34) == 0x34);
#pragma pack(push, 4)
struct D3TModelProbe {
    float parameter00;
    float parameter04;
    std::uint64_t parameter08;
    std::uint32_t parameter10;
    std::uint64_t parameter14;
    std::uint32_t parameter1c;
    float parameter20[2];
    float parameter28;
    std::uint32_t parameter2c;
    std::uint8_t parameter30;
    std::byte reserved_31[3];
};
#pragma pack(pop)

// Material: a run of parameters (type, value, one-based texture) and the geometry format its shader permutations are built for.
struct D3TModelMaterial {
    std::uint32_t nameOffset;
    std::uint32_t firstParameter;
    std::uint32_t parameterCount;
    std::uint32_t blendType;
    std::uint32_t addressU;
    std::uint32_t addressV;
    float depthBias;
    float minimumLod;
    float maximumLod;
    std::int32_t anisotropyLevel;
    std::int32_t wetnessType;
    std::uint8_t castsShadows;
    std::uint8_t unlit;
    std::uint8_t subsurfaceScattering;
    std::uint8_t doubleSided;
    std::uint8_t clearCoat;
    std::uint8_t softParticle;
    std::byte reserved_32[2];
    std::uint32_t materialType;
    std::uint32_t vertexFormat;
};

struct D3TModelMaterialParameter {
    std::uint32_t type;
    Vector4 value;
    std::uint32_t texture;
};

// Texture reference: the string-table path and the mip description d3t_SelectMipForPVR streams from (PvrMipDescription).
struct D3TModelTexture {
    std::uint32_t pathOffset;
    std::byte reserved_04[4];
    std::uint16_t width;
    std::uint16_t height;
    std::uint8_t mipCount;
    std::uint8_t format;
    std::byte reserved_0e[2];
};

// Draw group: an index range of one geometry buffer (drawn as submeshes). The skin and the part (the draw group's level-of-detail
// models, compared by their first draw group's leading word) are one-based.
struct D3TModelDrawGroup {
    std::uint32_t submeshCount;
    std::uint32_t indexCount;
    std::int32_t baseVertex;
    std::uint32_t reserved_0c;
    std::uint32_t firstIndex;
    std::uint32_t geometryIndex;
    std::uint32_t vertexFormat;
    std::uint32_t firstBillboard;
    std::uint32_t reserved_20;
    std::uint16_t firstSubmesh;
    std::uint16_t skin;
    std::uint16_t part;
    std::byte reserved_2a[2];
    Vector3 boundsMinimum;
    Vector3 boundsMaximum;
};

struct D3TModelSubmesh {
    std::uint32_t firstIndex;
    std::uint32_t primitiveCount;
    std::uint32_t material;
    Vector3 boundsMinimum;
    Vector3 boundsMaximum;
};

// Geometry buffer description; the vertex and index data follow the descriptions back to back.
struct D3TModelGeometryDescription {
    std::uint32_t vertexBytes;
    std::uint32_t indexFormat;
    std::uint32_t indexBytes;
    std::uint32_t vertexFormat;
};

struct D3TModelGeometryRuntime {
    std::uint32_t indexBuffer;
    std::uint32_t vertexBuffers[8];
    std::uint32_t vertexBufferCount;
    std::uint32_t inputLayout;
    std::uint32_t indexFormat;
};

// Per material: fourteen texture slots, then the eighteen shader permutations SetMasterShaderBindings requests.
struct D3TModelMaterialRuntime {
    std::uint32_t textures[14];
    std::uint32_t shaders[18];
    std::uint32_t constantBuffer;
    std::uint32_t flags;
    std::uint32_t sortKey;
};

// Skin: a run of the 16-bit bone node indices.
struct D3TModelSkin {
    std::uint32_t boneCount;
    std::uint32_t firstBone;
};

// A referenced model loaded beside this one (its handle is kept in referenceHandles).
struct D3TModelReference {
    std::uint32_t pathOffset;
    std::byte reserved_04[0x10];
};


struct D3TModelPart {
    std::uint32_t pathOffsets[7];
    std::uint16_t reserved_1c;
    std::uint16_t pathCount;
};

// Animation data d3t_ReadModelAnimations points into the image (the skinning TexSort reads it through d3t_GetPointerFromOffset): the
// expressions (0xd4 bytes each, named at +4), the animations (0x70 bytes each, each followed by its channel table and key data) and
// the bone maps (0x50 bytes each, each followed by its 64-byte entries).
struct D3TModelAnimation {
    char name[0x40];
    std::uint16_t channelCount;
    std::byte reserved_42[6];
    std::uint32_t *channels;
    std::uint64_t keyBytes;
    std::byte reserved_58[8];
    std::byte *keys;
    std::byte reserved_68[8];
};

struct D3TModelBoneMap {
    std::byte reserved_00[0x40];
    std::byte *entries;
    std::uint32_t entryCount;
    std::byte reserved_4c[4];
};

struct D3TModelAnimationSet {
    D3TModelAnimation *animations;
    std::uint32_t animationCount;
    std::uint32_t animationCapacity;
    std::byte *expressions;
    std::uint32_t expressionCount;
    std::uint32_t expressionCapacity;
    D3TModelBoneMap *boneMaps;
    std::uint32_t boneMapCount;
    std::uint32_t boneMapCapacity;
    std::uint8_t ownsStorage;
    std::byte reserved_31[7];
};


struct D3TModelRecord {
    D3TModelFileHeader *file;
    std::byte *runtimeStorage;
    const char *strings;
    std::uint32_t stringsSize;
    std::uint32_t stringsCapacity;
    std::uint8_t ownsStrings;
    std::uint8_t flag0;
    std::uint8_t flag1;
    std::byte reserved_23;
    std::uint32_t nodeCount;
    D3TModelNode *nodes;
    Matrix4x4 *nodeTransforms;
    D3TModelMaterialRuntime *materialRuntime;
    std::uint32_t materialCount;
    std::byte reserved_44[4];
    D3TModelMaterial *materials;
    D3TModelMaterialParameter *materialParameters;
    std::uint32_t textureCount;
    std::byte reserved_5c[4];
    D3TModelTexture *textures;
    std::uint32_t instanceBuffer;
    std::uint32_t instanceCount;
    void *instanceMirror;
    std::uint32_t drawGroupCount;
    std::byte reserved_7c[4];
    D3TModelDrawGroup *drawGroups;
    std::uint32_t geometryCount;
    std::byte reserved_8c[4];
    D3TModelGeometryRuntime *geometry;
    std::uint32_t submeshCount;
    std::byte reserved_9c[4];
    D3TModelSubmesh *submeshes;
    std::uint32_t spriteCount;
    std::byte reserved_ac[4];
    D3TModelSprite *sprites;
    std::uint32_t probeCount;
    std::byte reserved_bc[4];
    D3TModelProbe *probes;
    std::uint32_t skinCount;
    std::byte reserved_cc[4];
    D3TModelSkin *skins;
    std::uint16_t *boneNodes;
    Matrix4x4 *bindPoses;
    D3TModelAnimationSet animations;
    d3t::D3TRawArray<D3TModelMaterialParameter> runtimeMaterialParameters;
    d3t::D3TRawArray<D3TModelTexture> runtimeTextures;
    std::uint32_t loadState;
    std::uint32_t settingsOffset;
    std::uint32_t referenceCount;
    std::byte reserved_14c[4];
    D3TModelReference *references;
    std::uint32_t *referenceHandles;
    std::byte *instanceBounds;
    std::uint32_t partCount;
    std::byte reserved_16c[4];
    D3TModelPart *parts;
    std::uint32_t *partHandles;
    void *runtimeAllocation180;
    std::uint64_t runtimeAllocation180Size;
    void *runtimeAllocation190;
    std::uint64_t runtimeAllocation190Size;
    Vector4 bounds;
    char name[0x80];
    std::uint64_t geometryBytes;
    std::uint64_t textureBytes;
};
static_assert(sizeof(D3TModelFileHeader) == 0x64);
static_assert(offsetof(D3TModelFileHeader, nodeCount) == 0x28);
static_assert(offsetof(D3TModelFileHeader, geometryCount) == 0x48);
static_assert(offsetof(D3TModelFileHeader, stringsSize) == 0x60);
static_assert(sizeof(D3TModelNode) == 0x14);
static_assert(sizeof(D3TModelSprite) == 0x3c);
static_assert(offsetof(D3TModelProbe, parameter14) == 0x14);
static_assert(offsetof(D3TModelProbe, parameter30) == 0x30);
static_assert(sizeof(D3TModelProbe) == 0x34);
static_assert(sizeof(D3TModelMaterial) == 0x3c);
static_assert(offsetof(D3TModelMaterial, blendType) == 0x0c);
static_assert(offsetof(D3TModelMaterial, wetnessType) == 0x28);
static_assert(offsetof(D3TModelMaterial, materialType) == 0x34);
static_assert(offsetof(D3TModelMaterial, vertexFormat) == 0x38);
static_assert(sizeof(D3TModelMaterialParameter) == 0x18);
static_assert(sizeof(D3TModelTexture) == 0x10);
static_assert(sizeof(D3TModelDrawGroup) == 0x44);
static_assert(offsetof(D3TModelDrawGroup, baseVertex) == 8);
static_assert(offsetof(D3TModelDrawGroup, firstIndex) == 0x10);
static_assert(offsetof(D3TModelDrawGroup, geometryIndex) == 0x14);
static_assert(offsetof(D3TModelDrawGroup, firstBillboard) == 0x1c);
static_assert(offsetof(D3TModelDrawGroup, firstSubmesh) == 0x24);
static_assert(offsetof(D3TModelDrawGroup, skin) == 0x26);
static_assert(offsetof(D3TModelDrawGroup, part) == 0x28);
static_assert(sizeof(D3TModelSubmesh) == 0x24);
static_assert(sizeof(D3TModelGeometryDescription) == 0x10);
static_assert(sizeof(D3TModelGeometryRuntime) == 0x30);
static_assert(offsetof(D3TModelGeometryRuntime, vertexBufferCount) == 0x24);
static_assert(offsetof(D3TModelGeometryRuntime, inputLayout) == 0x28);
static_assert(sizeof(D3TModelMaterialRuntime) == 0x8c);
static_assert(offsetof(D3TModelMaterialRuntime, shaders) == 0x38);
static_assert(offsetof(D3TModelMaterialRuntime, constantBuffer) == 0x80);
static_assert(offsetof(D3TModelMaterialRuntime, flags) == 0x84);
static_assert(offsetof(D3TModelMaterialRuntime, sortKey) == 0x88);
static_assert(sizeof(D3TModelReference) == 0x14);
static_assert(sizeof(D3TModelPart) == 0x20);
static_assert(sizeof(D3TModelAnimation) == 0x70);
static_assert(offsetof(D3TModelAnimation, keyBytes) == 0x50);
static_assert(offsetof(D3TModelAnimation, keys) == 0x60);
static_assert(sizeof(D3TModelBoneMap) == 0x50);
static_assert(sizeof(D3TModelAnimationSet) == 0x38);
static_assert(offsetof(D3TModelRecord, nodeCount) == 0x24);
static_assert(offsetof(D3TModelRecord, materialRuntime) == 0x38);
static_assert(offsetof(D3TModelRecord, materialCount) == 0x40);
static_assert(offsetof(D3TModelRecord, textures) == 0x60);
static_assert(offsetof(D3TModelRecord, instanceBuffer) == 0x68);
static_assert(offsetof(D3TModelRecord, instanceMirror) == 0x70);
static_assert(offsetof(D3TModelRecord, drawGroups) == 0x80);
static_assert(offsetof(D3TModelRecord, geometry) == 0x90);
static_assert(offsetof(D3TModelRecord, submeshes) == 0xa0);
static_assert(offsetof(D3TModelRecord, sprites) == 0xb0);
static_assert(offsetof(D3TModelRecord, probes) == 0xc0);
static_assert(offsetof(D3TModelRecord, skins) == 0xd0);
static_assert(offsetof(D3TModelRecord, bindPoses) == 0xe0);
static_assert(offsetof(D3TModelRecord, animations) == 0xe8);
static_assert(offsetof(D3TModelRecord, runtimeMaterialParameters) == 0x120);
static_assert(offsetof(D3TModelRecord, runtimeMaterialParameters) + offsetof(d3t::D3TRawArray<D3TModelMaterialParameter>, capacity) == 0x12c);
static_assert(offsetof(D3TModelRecord, runtimeTextures) == 0x130);
static_assert(offsetof(D3TModelRecord, runtimeTextures) + offsetof(d3t::D3TRawArray<D3TModelTexture>, capacity) == 0x13c);
static_assert(offsetof(D3TModelRecord, loadState) == 0x140);
static_assert(offsetof(D3TModelRecord, settingsOffset) == 0x144);
static_assert(offsetof(D3TModelRecord, referenceCount) == 0x148);
static_assert(offsetof(D3TModelRecord, references) == 0x150);
static_assert(offsetof(D3TModelRecord, instanceBounds) == 0x160);
static_assert(offsetof(D3TModelRecord, partCount) == 0x168);
static_assert(offsetof(D3TModelRecord, partHandles) == 0x178);
static_assert(offsetof(D3TModelRecord, runtimeAllocation190) == 0x190);
static_assert(offsetof(D3TModelRecord, bounds) == 0x1a0);
static_assert(offsetof(D3TModelRecord, name) == 0x1b0);
static_assert(offsetof(D3TModelRecord, geometryBytes) == 0x230);
static_assert(sizeof(D3TModelRecord) == 0x240);

struct D3TModelTable {
    D3TModelRecord records[256];
    std::uint32_t generations[256];
    std::uint32_t cursor;
    std::byte reserved_24404[4];
};

// The data of a model load's async record: the instance handle the load was issued for and the model handle it produced.
struct D3TModelLoad {
    std::uint32_t instanceHandle;
    std::uint32_t modelHandle;
};

// The data of a referenced model's async record: where to store its model handle, and the instance handle.
struct D3TModelReferenceLoad {
    std::uint32_t *modelHandle;
    std::uint32_t instanceHandle;
};

static_assert(offsetof(D3TModelTable, generations) == 0x24000);
static_assert(offsetof(D3TModelTable, cursor) == 0x24400);
static_assert(sizeof(D3TModelTable) == 0x24408);
static_assert(sizeof(D3TModelLoad) == 8);
static_assert(sizeof(D3TModelReferenceLoad) == 0x10);

namespace d3t {
void ReadModel(D3TModelRecord &model, void *image, const char *sourcePath, std::int32_t textureMipMode);
extern D3TModelTable g_d3tModels;
std::uint32_t AllocateModelHandle();
D3TModelRecord *ResolveModel(std::uint32_t handle);
void ReleaseModel(D3TModelRecord &model);
void ReleaseModelHandle(std::uint32_t handle);
const char *GetModelSettings(std::uint32_t handle);
void CopyModelMaterials(std::uint32_t destinationHandle, std::uint32_t sourceHandle);
bool SetupShaderReferences(std::uint32_t handle);

std::uint32_t DispatchModelLoad(const char *path, std::uint32_t instanceHandle);
}

} // namespace shenmue
