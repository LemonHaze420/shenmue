#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/math.hpp"
#include "common/types.hpp"
#define SHENMUE_MODEL_LAYOUT_NAME_(line) ShenmueModelLayoutLayoutCheck##line
#define SHENMUE_MODEL_LAYOUT_NAME(line) SHENMUE_MODEL_LAYOUT_NAME_(line)
#define SHENMUE_MODEL_LAYOUT(condition) typedef char SHENMUE_MODEL_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
using ::shenmue::shen_name;
#define MODEL_DEFORMATION_CHUNK shen_name(SHEN_NAME4('M', 'D', 'P', '7'))
#define MT7_MODEL_FILE_TYPE SHEN_NAME4('M', 'D', '0', '7')
struct MT7Node {
    std::int16_t id, kind;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    void *mesh;
    MT7Node *child, *sibling, *parent;
    const Matrix4x4 *transformOverride;
    void *taskData;
};
SHENMUE_MODEL_LAYOUT(sizeof(MT7Node) == 0x40);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Node, mesh) == 0x28);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Node, parent) == 0x34);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Node, transformOverride) == 0x38);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Node, taskData) == 0x3c);
struct MT7NodeDataHeader {
    shen_name type;
    std::uint32_t size;
};
SHENMUE_MODEL_LAYOUT(sizeof(MT7NodeDataHeader) == 8);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MT7NodeSoundData {
    MT7NodeDataHeader header;
    shen_name marker;
    std::uint32_t material, environment;
};
SHENMUE_MODEL_LAYOUT(sizeof(MT7NodeSoundData) == 0x14);
SHENMUE_MODEL_LAYOUT(offsetof(MT7NodeSoundData, material) == 0x0c);
SHENMUE_MODEL_LAYOUT(offsetof(MT7NodeSoundData, environment) == 0x10);
std::uint32_t GetModelSoundMaterial(const MT7Node *node);
std::uint32_t GetModelSoundEnvironment(const MT7Node *node);
#endif
struct MT7ModelChunk {
    shen_name name;
    std::uint32_t size;
    MT7Node *root;
    std::int32_t textureCount;
};
struct MT7TextureBinding {
    std::int16_t width, height;
    std::uint32_t type;
    const void *pixels;
    std::int32_t texture;
};
SHENMUE_MODEL_LAYOUT(offsetof(MT7TextureBinding, type) == 4);
SHENMUE_MODEL_LAYOUT(offsetof(MT7TextureBinding, pixels) == 8);
SHENMUE_MODEL_LAYOUT(offsetof(MT7TextureBinding, texture) == 0x0c);
struct MT7MeshHeader {
    std::int32_t state;
    std::uint32_t flags;
    Vector3 centre;
    float radius;
};
struct MT7MeshMaterialCommand {
    std::uint32_t flags;
    std::byte retainedState04[0x0c];
    Vector3 centre;
    std::byte retainedState1c[8];
    std::int8_t vertexFormat;
    std::byte retainedFormatState[0x27];
    std::uint32_t vertexBytes;
};
union MT7PacketNormal {
    Vector3 floating;
    struct {
        std::int8_t z, y, x;
        std::byte retained[9];
    } packed;
};
struct MT7PacketVertex {
    Vector3 position;
    MT7PacketNormal normal;
    float u, v;
};
enum MT7PrimitiveFlags { MT7PrimitiveTriangleList = 8U, MT7PrimitiveEnvironmentMapped = 0x100U };
SHENMUE_MODEL_LAYOUT(sizeof(MT7MeshHeader) == 0x18);
SHENMUE_MODEL_LAYOUT(sizeof(MT7PacketNormal) == 0x0c);
SHENMUE_MODEL_LAYOUT(sizeof(MT7PacketVertex) == 0x20);
SHENMUE_MODEL_LAYOUT(offsetof(MT7PacketVertex, normal) == 0x0c);
SHENMUE_MODEL_LAYOUT(offsetof(MT7PacketVertex, u) == 0x18);
SHENMUE_MODEL_LAYOUT(offsetof(MT7PacketVertex, v) == 0x1c);
SHENMUE_MODEL_LAYOUT(sizeof(MT7MeshMaterialCommand) == 0x50);
SHENMUE_MODEL_LAYOUT(offsetof(MT7MeshHeader, centre) == 8);
SHENMUE_MODEL_LAYOUT(offsetof(MT7MeshMaterialCommand, centre) == 0x10);
SHENMUE_MODEL_LAYOUT(offsetof(MT7MeshMaterialCommand, vertexFormat) == 0x24);
SHENMUE_MODEL_LAYOUT(offsetof(MT7MeshMaterialCommand, vertexBytes) == 0x4c);
struct MT7NodeDeformationData {
    shen_name nodeChunk;
    std::uint32_t nodeChunkSize;
    std::uint32_t retainedState[3];
    shen_name linksChunk;
    std::int32_t linkCount;
};
struct MT7VertexLink {
    std::uint32_t sourceOffset, destinationOffset;
};
SHENMUE_MODEL_LAYOUT(sizeof(MT7NodeDeformationData) == 0x1c);
SHENMUE_MODEL_LAYOUT(offsetof(MT7NodeDeformationData, linksChunk) == 0x14);
SHENMUE_MODEL_LAYOUT(offsetof(MT7NodeDeformationData, linkCount) == 0x18);
SHENMUE_MODEL_LAYOUT(sizeof(MT7VertexLink) == 8);
SHENMUE_MODEL_LAYOUT(sizeof(MT7ModelChunk) == 0x10);
SHENMUE_MODEL_LAYOUT(sizeof(MT7TextureBinding) == 0x10);
#define MODEL_INSTANCE_MEMORY shen_name(SHEN_NAME4('M', 'A', 'I', ' '))
void RelocateMT7Model(MT7ModelChunk *chunk);
void RelocateMT7Nodes(std::intptr_t base, MT7Node *node);
void BindMT7NodeTextures(MT7Node *node, MT7TextureBinding *textures);
void SetMT7Parents(MT7Node *node, MT7Node *parent);
MT7Node *FindMT7Node(MT7Node *node, std::int16_t id);
void SetMT7ClothNodeKinds(MT7Node *node);
void SetupModelForImageModule(MT7Node *node);
struct MT7Model {
    void *source;
    MT7Node *root;
    std::int32_t retainedTextureCount;
    std::int32_t entityType;
    void *face;
};
SHENMUE_MODEL_LAYOUT(sizeof(MT7Model) == 0x14);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Model, root) == 4);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Model, entityType) == 0x0c);
SHENMUE_MODEL_LAYOUT(offsetof(MT7Model, face) == 0x10);
MT7Model *ReadMT7Model(void *source, std::int32_t mode);
MT7Model *InitializeMT7Model(void *source);
MT7Model *LoadMT7Model(const char *directory, const char *filename);
MT7Node *ParseMT7ModelRoot(void *source, MT7Model **output);
void SetMT7ModelEntityType(MT7Model *model, const char *filename);
void ReleaseMT7Model(MT7Model *model);
} // namespace s2
} // namespace shenmue
#undef SHENMUE_MODEL_LAYOUT_NAME
#undef SHENMUE_MODEL_LAYOUT_NAME_
#undef SHENMUE_MODEL_LAYOUT
#endif
