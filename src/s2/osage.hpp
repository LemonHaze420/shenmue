#pragma once

#include "common/types.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct Character;
struct MT7Node;
struct OsageNode;
inline constexpr shen_name OSAGE_COMPONENT{SHEN_NAME(OSAG)};
inline constexpr std::array<std::int16_t, 17> osageNodeKinds{120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 141, 131, 65, 132, 133, -1};
enum OsageUpdateFlag : std::uint8_t {
    OsageUpdateCurrentFrame = 0x1U,
    OsageUpdatePreviousFrames = 0x7U,
    OsageUpdateHistoryMask = 0xfU,
    OsageUpdateWarmupStep = 0x10U,
    OsageUpdateWarmupStart = 0x80U,
    OsageUpdateWarmupMask = 0xf0U,
};

#pragma pack(push, 4)
struct OsageCollisionPoint {
    std::int8_t x, y, z, radius;
};
struct OsageParameters {
    shen_name token;
    std::int32_t collisionOffset;
    std::int32_t primaryOffset;
    std::int32_t secondaryOffset;
    std::int32_t secondaryDepth;
    std::uint32_t reserved14;
};
struct OsageSpringParameters {
    std::int32_t mode;
    float length, radius, damping, stiffness, gravity;
};
struct OsageWork {
    shen_name character;
    std::uint8_t flags;
    std::uint8_t mode;
    std::uint16_t reserved06;
    Character *owner;
    Character *collisionOwner;
    std::array<std::uint32_t, 6> referenceAngles;
    OsageCollisionPoint *collisionPoints;
    OsageCollisionPoint *secondaryCollisionPoints;
    std::array<Vector4, 28> collisionSurfaces;
    float motionScale;
    float distanceLimit;
    OsageNode *nodes;
    OsageParameters *parameters;
    float windSpeedScaled;
    float windSpeedNext;
    Vector3 wind;
    std::uint32_t reserved22c;
};
struct OsageNode {
    shen_name token;
    std::uint32_t reserved04;
    OsageWork *owner;
    MT7Node *node;
    std::int16_t kind;
    std::uint8_t linkFlags;
    std::uint8_t updateFlags;
    Matrix4x4 transform;
    Matrix4x4 parentTransform;
    Vector3 current;
    Vector3 previous;
    Vector3 endpoint;
    std::uint32_t facingAngle;
    Vector3i euler;
    float motionMagnitude;
    float accumulatedSway;
    float accumulatedForward;
    float accumulatedVertical;
    float pitchScale;
    float yawScale;
    float pitchPhase;
    float yawPhase;
    std::array<Vector2, 10> offsets;
    std::int32_t smoothingCountdown;
    std::uint32_t reserved144;
    OsageNode *parent;
    OsageNode *child;
    OsageNode *next;
    std::array<std::int16_t, 32> vertexBindings;
    void *meshAllocation;
    std::byte *originalMesh;
    std::uint64_t reserved1b0;
};
#pragma pack(pop)
static_assert(sizeof(OsageParameters) == 0x18);
static_assert(sizeof(OsageSpringParameters) == 0x18);
static_assert(sizeof(OsageWork) == 0x230);
static_assert(offsetof(OsageWork, mode) == 5);
static_assert(offsetof(OsageWork, collisionSurfaces) == 0x40);
static_assert(offsetof(OsageWork, nodes) == 0x208);
static_assert(offsetof(OsageWork, parameters) == 0x210);
static_assert(sizeof(OsageNode) == 0x1b8);
static_assert(offsetof(OsageNode, transform) == 0x1c);
static_assert(offsetof(OsageNode, parentTransform) == 0x5c);
static_assert(offsetof(OsageNode, current) == 0x9c);
static_assert(offsetof(OsageNode, facingAngle) == 0xc0);
static_assert(offsetof(OsageNode, offsets) == 0xf0);
static_assert(offsetof(OsageNode, parent) == 0x148);
static_assert(offsetof(OsageNode, next) == 0x158);
static_assert(offsetof(OsageNode, meshAllocation) == 0x1a0);
void SetCharacterOsageMode(Character *character, std::uint8_t mode);
void mtAssignOsage(Character *character);
void FindOSAG(Character *character);
void UpdateCharacterOsage(MT7Node *model, Character *character);
void toggleOsagFlags(Character *character);
}
