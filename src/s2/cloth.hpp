#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

struct ClothComponent;
struct ClothLink;
struct ClothVertexBinding;

inline constexpr shen_name CLOTH_OBJECT{SHEN_NAME(CLTH)};
inline constexpr shen_name CLOTH_CHARACTER_SRY{SHEN_NAME(SRY_)};
inline constexpr std::uint32_t CLOTH_WIND_COLUMN_STEP = 14112;
inline constexpr std::uint32_t CLOTH_OSCILLATION_STEP = 30;
inline constexpr std::uint32_t CLOTH_OSCILLATION_ROTATION_STEP = 4537;
inline constexpr std::array<Vector4, 4> clothDampingTable{
    {{0.5f, 0.5f, 0.5f, 0.5f}, {0.8f, 0.65f, 0.8f, 0.5f}, {0.2f, 0.1f, 0.2f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}};


struct ClothBoneCapsule {
    Vector3 position;
    float radius;
    std::uint8_t boneId;
    std::uint8_t reserved11;
    std::int16_t groupMask;  // +0x12, 1 << group, or -1
};
static_assert(sizeof(ClothBoneCapsule) == 0x14);


struct ClothVertexState {
    std::uint8_t collisionIndex;
    std::uint8_t flags;
    std::uint16_t reserved02;
    std::int32_t moved;
    ClothVertexBinding *current;
    ClothVertexBinding *previous;
    Vector3 velocity;
    float aboveRestLength;
    float leftRestLength;
    float rightRestLength;
    ClothVertexState *above;
    ClothVertexState *below;
    ClothVertexState *left;
    ClothVertexState *right;
};
static_assert(sizeof(ClothVertexState) == 0x50);
static_assert(offsetof(ClothVertexState, flags) == 0x01);
static_assert(offsetof(ClothVertexState, current) == 0x08);
static_assert(offsetof(ClothVertexState, previous) == 0x10);
static_assert(offsetof(ClothVertexState, velocity) == 0x18);
static_assert(offsetof(ClothVertexState, above) == 0x30);


struct ClothVertexBinding {
    std::uint16_t index;    // +0x00, 0xffff when unbound
    std::uint16_t field02;
    Vector3 position;
    Vector3 normalEndpoint;
};
static_assert(sizeof(ClothVertexBinding) == 0x1c);
static_assert(offsetof(ClothVertexBinding, position) == 0x04);
static_assert(offsetof(ClothVertexBinding, normalEndpoint) == 0x10);


#pragma pack(push, 1)
struct ClothObject {
    shen_name token;                      // +0x00, 'CLTH'
    std::uint8_t index;                   // +0x04, slot in ClothComponent::cloths
    std::uint8_t rows;                    // +0x05, descriptor +0x04
    std::uint8_t columns;                 // +0x06, descriptor +0x00
    std::uint8_t boneRecordCount;
    std::uint16_t flags;
    std::uint16_t nodeId;
    std::uint16_t field0c;
    std::array<std::byte, 2> reserved0e;
    std::int32_t vertexCount;             // +0x10, rows * columns
    std::array<std::byte, 4> reserved14;
    std::byte *descriptorData28;          // +0x18, cloth source + descriptor +0x28
    std::byte *descriptorData10;          // +0x20, cloth source + descriptor +0x10
    ClothBoneCapsule *boneBuffer;         // +0x28, the owner's CLCB
    std::byte *sourceMesh;                // +0x30, the node's XB01 mesh
    std::byte *mesh;                      // +0x38, 'CLMS' copy the simulation deforms
    std::uint32_t meshSize;
    std::array<std::byte, 4> reserved44;
    MT7Node *node;
    std::array<std::uint16_t, 9> field50;
    std::array<std::byte, 2> reserved62;
    std::array<float, 9> rowLengths;
    ClothComponent *owner;
    ClothLink *link;                      // +0x90, the 0xffa5 node under this one
    ClothVertexBinding *bindings;         // +0x98, 'CLVB'
    ClothVertexBinding *previousBindings; // +0xa0, 'CLVO'
    ClothVertexState *vertices;           // +0xa8, 'CLID'
    std::uint32_t reservedb0;
    Matrix4x4 transform;
    std::uint32_t reservedf4;
    void **columnLinks;                   // +0xf8, 'CLVP', one per column
};
#pragma pack(pop)
static_assert(sizeof(ClothObject) == 0x100);
static_assert(offsetof(ClothObject, vertexCount) == 0x10);
static_assert(offsetof(ClothObject, node) == 0x48);
static_assert(offsetof(ClothObject, owner) == 0x88);
static_assert(offsetof(ClothObject, vertices) == 0xa8);
static_assert(offsetof(ClothObject, rowLengths) == 0x64);
static_assert(offsetof(ClothObject, transform) == 0xb4);
static_assert(offsetof(ClothObject, columnLinks) == 0xf8);


struct ClothLink {
    ClothObject *primary;
    MT7Node *node;
    std::byte *sourceMesh;
    std::byte *mesh;              // +0x18, 'CLMS'
    std::byte *descriptorData;    // +0x20, cloth source + the primary's descriptor +0x2c
    ClothVertexBinding *bindings; // +0x28, 'CLVB'
};
static_assert(sizeof(ClothLink) == 0x30);




#pragma pack(push, 1)
struct ClothComponent {
    shen_name token;                             // +0x00, 'CLMD'
    std::uint16_t mode;
    std::uint8_t previousMode;
    std::uint8_t boneRecordCount;                // +0x07, records before the 0xff terminator
    std::uint8_t field08;
    std::uint8_t clothCount;
    std::uint8_t field0a;
    std::int8_t assignMode;                      // +0x0b, mtAssignClothFlags argument
    std::uint8_t resetCountdown;
    std::array<std::uint8_t, 3> field0d;
    Character *owner;
    MotionWork *motionWork;
    ClothBoneCapsule *boneBuffer;                // +0x20, 'CLCB', one capsule per bone record
    const std::byte *boneRecords;                // +0x28, 10-byte records
    MT7Node *model;
    std::array<ClothObject *, 4> cloths;         // +0x38, 'CLTH' objects
    shen_name name;
    float minimumCapsuleRadius;
    Vector3 windOffset;
    float windStrength;
    std::int32_t oscillation;
    std::int32_t oscillationRotation;
    std::byte *parameters;                       // +0x78, CLCX/CLCL record
    std::byte *source;                           // +0x80, CLSX/CLSG data
    std::uint32_t field88;
    float field8c;                               // +0x8c, clmdUtil preset value
    std::uint32_t field90;
    std::array<std::uint8_t, 4> savedParameters; // +0x94, parameter bytes +0x16..+0x19
    float field98;
    float field9c;
};
#pragma pack(pop)
static_assert(sizeof(ClothComponent) == 0xa0);
static_assert(offsetof(ClothComponent, assignMode) == 0x0b);
static_assert(offsetof(ClothComponent, owner) == 0x10);
static_assert(offsetof(ClothComponent, model) == 0x30);
static_assert(offsetof(ClothComponent, cloths) == 0x38);
static_assert(offsetof(ClothComponent, name) == 0x58);
static_assert(offsetof(ClothComponent, windOffset) == 0x60);
static_assert(offsetof(ClothComponent, parameters) == 0x78);
static_assert(offsetof(ClothComponent, field8c) == 0x8c);
static_assert(offsetof(ClothComponent, savedParameters) == 0x94);
static_assert(offsetof(ClothComponent, field9c) == 0x9c);


void mtAssignClothFlags(Character *character, std::int8_t assignMode);

void CLMD(Character *character, std::int8_t boneSet);

void clmdUtil(Character *character, std::int8_t mode);
void SetClothFlag(Character *character, std::uint8_t flags, std::int32_t enabled);
void RestoreClothMeshBindings(Character *character);

ClothComponent *FindCLMD(Character *character);

void PrepareClothForDraw(Character *character);
std::int64_t RecursivePrepareChildMatrices(MT7Node *model, MT7Node *node);

} // namespace shenmue::s2
