#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define CLOTH_COMPONENT SHEN_NAME4('C', 'L', 'M', 'D')
#define CLOTH_SOURCE SHEN_NAME4('C', 'L', 'S', 'G')
#define CLOTH_PARAMETERS SHEN_NAME4('C', 'L', 'C', 'L')
#define MEMORY_CLOTH_OBJECT SHEN_NAME4('C', 'L', 'T', 'H')
#define MEMORY_CLOTH_MESH SHEN_NAME4('C', 'L', 'M', 'S')
#define MEMORY_CLOTH_VERTICES SHEN_NAME4('C', 'L', 'I', 'D')
#define MEMORY_CLOTH_BINDINGS SHEN_NAME4('C', 'L', 'V', 'B')
#define MEMORY_CLOTH_PREVIOUS_BINDINGS SHEN_NAME4('C', 'L', 'V', 'O')
#define MEMORY_CLOTH_COLUMNS SHEN_NAME4('C', 'L', 'P', 'V')
#define MEMORY_CLOTH_BONES SHEN_NAME4('C', 'L', 'C', 'B')
static const std::uint32_t kClothBoneCapsuleBytes = 0x14;
static const std::uint32_t kClothOscillationStep = 30, kClothOscillationRotationStep = 4537;
#define CLOTH_WIND_SOURCE_CHARACTER SHEN_NAME4('a', '3', '3', '4')
struct ClothComponent;
struct ClothObject;
struct ClothVertexBinding;
struct MotionWork;
struct ClothBoneCapsule {
    Vector3 position;
    float radius;
    std::uint8_t boneId, retained11;
    std::uint16_t groupMask;
};
typedef char ClothBoneCapsuleSizeCheck[sizeof(ClothBoneCapsule) == 0x14 ? 1 : -1];
struct ClothVertexState {
    union {
        std::uint8_t bindingIndex, collisionIndex;
    };
    std::uint8_t flags;
    std::byte retainedBindingState[2];
    union {
        std::uint32_t retainedFlags;
        std::int32_t moved;
    };
    ClothVertexBinding *current, *previous;
    Vector3 velocity;
    float aboveRestLength, leftRestLength, rightRestLength;
    ClothVertexState *above, *below, *left, *right;
};
struct ClothVertexBinding {
    std::int16_t sourceOffset;
    std::uint16_t flags;
    Vector3 position, normalEndpoint;
};
struct ClothLink {
    ClothObject *primary;
    MT7Node *node;
    void *sourceMesh, *mesh;
    std::byte *descriptorData;
    ClothVertexBinding *bindings;
};
struct ClothObject {
    shen_name token;
    std::uint8_t index, rows, columns, boneRecordCount;
    std::uint16_t flags, nodeId, descriptorIndex;
    std::byte retainedDescriptorState[2];
    std::int32_t vertexCount;
    std::byte *descriptorData28, *descriptorData10;
    void *boneBuffer, *sourceMesh, *mesh;
    std::uint32_t meshSize;
    MT7Node *node;
    std::int16_t bindingIndices[9];
    std::byte retainedRowState[2];
    float rowLengths[9];
    ClothComponent *owner;
    ClothLink *link;
    ClothVertexBinding *bindings, *previousBindings;
    ClothVertexState *vertices;
    std::byte retainedTransformState[4];
    Matrix4x4 transform;
    void **columnLinks;
};
struct ClothComponent {
    shen_name token;
    std::uint16_t mode;
    std::uint8_t previousMode, boneRecordCount, retainedControlState, clothCount, updatedClothCount;
    std::uint8_t assignMode;
    std::uint8_t resetCountdown;
    std::byte retainedAllocationState[3];
    Character *owner;
    MotionWork *motionWork;
    void *boneBuffer;
    const std::byte *boneRecords;
    MT7Node *model;
    ClothObject *cloths[4];
    shen_name character;
    float minimumCapsuleRadius;
    Vector3 windVelocity;
    float windStrength;
    std::int32_t oscillation, oscillationRotation;
    std::byte *parameters, *source;
    Vector3 windOffset;
    std::uint8_t savedParameters[3];
    std::byte retainedParameterState;
};
typedef char Layout_cloth_dc_0[(sizeof(ClothVertexState) == 0x38) ? 1 : -1];
typedef char Layout_cloth_dc_1[(offsetof(ClothVertexState, velocity) == 0x10) ? 1 : -1];
typedef char Layout_cloth_dc_2[(sizeof(ClothVertexBinding) == 0x1c) ? 1 : -1];
typedef char Layout_cloth_dc_3[(sizeof(ClothLink) == 0x18) ? 1 : -1];
typedef char Layout_cloth_dc_4[(sizeof(ClothObject) == 0xc4) ? 1 : -1];
typedef char Layout_cloth_dc_5[(offsetof(ClothObject, sourceMesh) == 0x20) ? 1 : -1];
typedef char Layout_cloth_dc_6[(offsetof(ClothObject, node) == 0x2c) ? 1 : -1];
typedef char Layout_cloth_dc_7[(offsetof(ClothObject, owner) == 0x68) ? 1 : -1];
typedef char Layout_cloth_dc_8[(offsetof(ClothObject, link) == 0x6c) ? 1 : -1];
typedef char Layout_cloth_dc_9[(offsetof(ClothObject, previousBindings) == 0x74) ? 1 : -1];
typedef char Layout_cloth_dc_10[(offsetof(ClothObject, vertices) == 0x78) ? 1 : -1];
typedef char Layout_cloth_dc_11[(offsetof(ClothObject, columnLinks) == 0xc0) ? 1 : -1];
typedef char Layout_cloth_dc_12[(sizeof(ClothComponent) == 0x6c) ? 1 : -1];
typedef char Layout_cloth_dc_13[(offsetof(ClothComponent, assignMode) == 0x0b) ? 1 : -1];
typedef char Layout_cloth_dc_14[(offsetof(ClothComponent, model) == 0x20) ? 1 : -1];
typedef char Layout_cloth_dc_15[(offsetof(ClothComponent, boneBuffer) == 0x18) ? 1 : -1];
typedef char Layout_cloth_dc_16[(offsetof(ClothComponent, cloths) == 0x24) ? 1 : -1];
typedef char Layout_cloth_dc_17[(offsetof(ClothComponent, character) == 0x34) ? 1 : -1];
typedef char Layout_cloth_dc_18[(offsetof(ClothComponent, windVelocity) == 0x3c) ? 1 : -1];
typedef char Layout_cloth_dc_19[(offsetof(ClothComponent, windStrength) == 0x48) ? 1 : -1];
typedef char Layout_cloth_dc_20[(offsetof(ClothComponent, oscillation) == 0x4c) ? 1 : -1];
typedef char Layout_cloth_dc_21[(offsetof(ClothComponent, parameters) == 0x54) ? 1 : -1];
typedef char Layout_cloth_dc_22[(offsetof(ClothComponent, source) == 0x58) ? 1 : -1];
typedef char Layout_cloth_dc_23[(offsetof(ClothComponent, windOffset) == 0x5c) ? 1 : -1];
typedef char ClothOwnerOffsetCheck[offsetof(ClothComponent, owner) == 0x10 ? 1 : -1];
typedef char ClothMotionWorkOffsetCheck[offsetof(ClothComponent, motionWork) == 0x14 ? 1 : -1];
typedef char Layout_cloth_dc_24[(offsetof(ClothComponent, savedParameters) == 0x68) ? 1 : -1];
void mtAssignClothFlags(Character *character, std::uint8_t mode);
void ResetClothObjects(Character *character);
void UpdateWindReactiveNode(Character *character, void *payload);
void AllocateClothObjects(ClothComponent *cloth);
void SetupClothBones(ClothComponent *cloth);
void ReadClothCapsules(ClothComponent *cloth, const std::byte *records);
void UpdateClothBoneCapsules(ClothComponent *cloth, const std::byte *records);
void PrepareClothForDraw(Character *character);
void AllocateClothNodeTree(ClothComponent *cloth, MT7Node *node);
std::uint32_t GetClothMeshByteSize(const void *mesh);
void FreeUSE_0(Character *character, void *payload);
void FreeClothObjects(ClothComponent *cloth);
void RestoreClothMeshBindings(Character *character);
void FindCLMD(Character *character);
void CLMD(Character *character, std::uint8_t boneSet);
void clmdUtil(Character *character, std::uint8_t mode);
std::int32_t UpdateClothNodeTree(MT7Node *model);

} // namespace s2
} // namespace shenmue

#endif
