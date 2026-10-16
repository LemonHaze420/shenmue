#pragma once
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct Character;
struct ClothAuthoredSphere;
struct ClothChain;
struct ClothCollisionSphere;
struct ClothModelWork;
struct ClothNeighbor;
struct ClothRenderLink;
struct ClothRenderVertexLink;
struct ClothSimulationNode;
struct ClothVertex;
struct MotionInformationWork;
struct Mt5RuntimeNode;

#pragma pack(push, 1)
struct ClothModelWork {
    std::uint8_t flags;
    std::uint8_t previousFlags;
    std::uint8_t nodeCount;
    std::uint8_t basePoseMode;
    std::uint8_t chainCount;
    std::uint8_t updateFlags;
    std::uint8_t setupMode;
    std::uint8_t setupCountdown;
    Character *character;
    MotionInformationWork *motion;
    ClothCollisionSphere *clothData;
    const ClothAuthoredSphere *clothPresetTable;
    Mt5RuntimeNode *model;
    ClothChain *chains[4];
    std::uint32_t imageName;
    float maximumRowLength;
    Vector3 wind;
    std::uint16_t presetIndex;
    std::byte reserved066[2];
};
#pragma pack(pop)

static_assert(sizeof(ClothModelWork) == 0x68);
static_assert(offsetof(ClothModelWork, clothData) == 0x18);
static_assert(offsetof(ClothModelWork, clothPresetTable) == 0x20);
static_assert(offsetof(ClothModelWork, model) == 0x28);
static_assert(offsetof(ClothModelWork, imageName) == 0x50);
static_assert(offsetof(ClothModelWork, presetIndex) == 0x64);
void CLMD_Callback(Character *character, void *payload);
void CLMD_Cleanup(Character *character, void *payload);
void CLMD_Alloc(Character *character, std::uint8_t setupMode = 0);
void CLMD_Setup(Character *character, std::uint8_t setupMode);
ClothModelWork *CLMD_Flag(Character *character);
void CLMD_RestoreModelBindings(Character *character);
void CLMD_SetFlags(Character *character, std::uint8_t flags, std::int32_t enable);

inline constexpr float kClothMode4WindProfile[]{
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.30000001192092896f,
    0.4000000059604645f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.30000001192092896f,
    0.10000000149011612f,
    0.30000001192092896f,
    0.30000001192092896f,
    0.4000000059604645f,
    0.4000000059604645f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.20000000298023224f,
    0.10000000149011612f,
    0.10000000149011612f,
    0.0f,
    0.0f,
    0.10000000149011612f,
    0.10000000149011612f,
    0.20000000298023224f,
    0.30000001192092896f,
    0.5f,
    0.5f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.20000000298023224f,
    0.30000001192092896f,
    0.30000001192092896f,
    0.5f,
    0.5f,
    0.5f,
    0.30000001192092896f,
    0.20000000298023224f,
    0.20000000298023224f,
    0.10000000149011612f,
    0.0f,
    0.0f,
    0.10000000149011612f,
    0.20000000298023224f,
    0.20000000298023224f,
    0.4000000059604645f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.4000000059604645f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.4000000059604645f,
    0.4000000059604645f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.20000000298023224f,
    0.10000000149011612f,
    0.0f,
    0.0f,
    0.10000000149011612f,
    0.20000000298023224f,
    0.30000001192092896f,
    0.5f,
    0.699999988079071f,
    0.699999988079071f,
    0.5f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.6000000238418579f,
    0.699999988079071f,
    0.699999988079071f,
    0.699999988079071f,
    0.5f,
    0.4000000059604645f,
    0.30000001192092896f,
    0.20000000298023224f,
    0.0f,
    0.0f,
    0.20000000298023224f,
    0.30000001192092896f,
    0.4000000059604645f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.8999999761581421f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.699999988079071f,
    0.6000000238418579f,
    0.5f,
    0.0f,
    0.0f,
    0.5f,
    0.6000000238418579f,
    0.699999988079071f,
    0.800000011920929f,
    0.8999999761581421f,
    0.8999999761581421f,
    0.800000011920929f,
    0.8999999761581421f,
    1.0f,
    0.8999999761581421f,
    0.800000011920929f,
    0.8999999761581421f,
    0.8999999761581421f,
    0.8999999761581421f,
    0.800000011920929f,
    0.800000011920929f,
    0.800000011920929f,
    0.6000000238418579f,
    0.10000000149011612f,
    0.10000000149011612f,
    0.6000000238418579f,
    0.800000011920929f,
    0.800000011920929f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.2000000476837158f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.8999999761581421f,
    0.8999999761581421f,
    0.800000011920929f,
    0.20000000298023224f,
    0.20000000298023224f,
    0.800000011920929f,
    0.8999999761581421f,
    0.8999999761581421f,
};

inline constexpr float kClothMode4ColumnEnvelope[]{
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.800000011920929f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.800000011920929f,
    0.6000000238418579f,
    0.4000000059604645f,
    0.20000000298023224f,
    0.0f,
    0.20000000298023224f,
    0.4000000059604645f,
    0.6000000238418579f,
    0.800000011920929f,
    1.0f,
};

inline constexpr float kClothMode7WindProfile[]{
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.30000001192092896f,
    0.10000000149011612f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.20000000298023224f,
    0.30000001192092896f,
    0.5f,
    0.6000000238418579f,
    0.4000000059604645f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.4000000059604645f,
    0.6000000238418579f,
    0.800000011920929f,
    0.800000011920929f,
    0.5f,
    0.10000000149011612f,
    0.0f,
    0.0f,
    0.0f,
    0.10000000149011612f,
    0.5f,
    0.800000011920929f,
    1.0f,
};
#pragma pack(push, 1)
struct ClothVertex {
    Vector3 position;
    Vector3 normal;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothNeighbor {
    std::int16_t vertexIndex;
    std::uint16_t reserved02;
    float restLength;
    ClothVertex *current;
    ClothVertex *previous;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothSimulationNode {
    std::int16_t vertexIndex;
    std::int8_t collisionIndex;
    std::uint8_t reserved03;
    std::uint32_t groundContact;
    std::uint8_t pinFlags;
    std::byte reserved09[7];
    ClothVertex *current;
    ClothVertex *previous;
    ClothNeighbor above, below, left, right;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothCollisionSphere {
    Vector3 center;
    float radius;
    std::uint8_t motionNodeId;
    std::uint8_t reserved11;
    std::uint16_t collisionMask;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothRenderVertexLink {
    std::int16_t vertexIndex;
    std::byte reserved02[6];
    ClothVertex *vertex;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothRenderLink {
    std::byte reserved00[8];
    std::uint32_t vertexCount;
    std::uint32_t reserved0c;
    ClothVertex *current;
    ClothVertex *originalVertices;
    ClothVertex *sourceVertices;
    Mt5RuntimeNode *model;
    void *originalBinding;
    std::byte binding[0x20];
    std::int16_t nodeKind;
    std::byte reserved5a[6];
    ClothRenderVertexLink *links;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothChain {
    std::uint8_t flags, rows, columns, reserved03;
    std::int16_t nodeKind;
    std::uint16_t collisionMask;
    std::uint32_t vertexCount;
    std::uint32_t reserved0c;
    ClothVertex *current;
    ClothVertex *previous;
    ClothVertex *originalVertices;
    ClothVertex *sourceVertices;
    ClothCollisionSphere *collisionSpheres;
    void *originalBinding;
    std::byte binding[0x20];
    Mt5RuntimeNode *model;
    ClothSimulationNode *nodes;
    std::int16_t pivotColumns[9];
    std::byte reserved082[2];
    float rowRestLengths[9];
    ClothModelWork *owner;
    ClothRenderLink *renderLink;
};
#pragma pack(pop)

} // namespace shenmue::s1
