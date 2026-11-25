#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define OSAGE_COMPONENT SHEN_NAME4('O', 'S', 'A', 'G')
static const std::int16_t osageNodeKinds[] = {120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 141, 131, 65, 132, 133, -1};
struct OsageWork;
enum OsageUpdateFlag {
    OsageUpdateCurrentFrame = 1,
    OsageUpdatePreviousFrames = 7,
    OsageUpdateHistoryMask = 0x0f,
    OsageUpdateWarmupStep = 0x10,
    OsageUpdateWarmupStart = 0x80,
    OsageUpdateWarmupMask = 0xf0
};
struct OsageCollisionPoint {
    std::int8_t x, y, z, radius;
};
struct OsageSpringParameters {
    std::int32_t mode;
    float length, radius, damping, stiffness, gravity;
};
struct OsageParameters {
    shen_name token;
    union {
        std::int32_t collisionOffset;
        OsageCollisionPoint *collisionPoints;
    };
    union {
        std::int32_t primaryOffset;
        OsageSpringParameters *primary;
    };
    union {
        std::int32_t secondaryOffset;
        OsageSpringParameters *secondary;
    };
    std::uint8_t secondaryDepth;
    std::byte retainedDepthState[3];
};
struct OsageNode {
    shen_name token;
    OsageWork *owner;
    MT7Node *node;
    std::int16_t kind;
    std::uint8_t linkFlags, updateFlags;
    Matrix4x4 transform;
    Matrix4x4 parentTransform;
    Vector3 current, previous;
    Vector3 endpoint;
    std::uint32_t facingAngle;
    Vector3i euler;
    float motionMagnitude, accumulatedSway, accumulatedForward, accumulatedVertical;
    float pitchScale, yawScale, pitchPhase, yawPhase;
    Vector2 offsets[10];
    std::int32_t smoothingCountdown;
    OsageNode *parent, *child, *next;
    std::int16_t vertexBindings[32];
    void *meshAllocation, *originalMesh;
};
struct OsageWork {
    shen_name character;
    std::uint8_t flags, mode;
    std::byte retainedModeState[2];
    Character *owner, *collisionOwner;
    std::uint32_t referenceAngles[5];
    OsageCollisionPoint *collisionPoints, *secondaryCollisionPoints;
    Vector4 collisionSurfaces[28];
    float motionScale, distanceLimit;
    OsageNode *nodes;
    OsageParameters *parameters;
    float windSpeedScaled, windSpeedNext;
    Vector3 wind;
};
typedef char Layout_osage_dc_0[(sizeof(OsageCollisionPoint) == 4) ? 1 : -1];
typedef char Layout_osage_dc_1[(sizeof(OsageSpringParameters) == 0x18) ? 1 : -1];
typedef char Layout_osage_dc_2[(sizeof(OsageParameters) == 0x14) ? 1 : -1];
typedef char Layout_osage_dc_3[(offsetof(OsageParameters, secondaryDepth) == 0x10) ? 1 : -1];
typedef char Layout_osage_dc_4[(sizeof(OsageNode) == 0x18c) ? 1 : -1];
typedef char Layout_osage_dc_5[(offsetof(OsageNode, node) == 8) ? 1 : -1];
typedef char Layout_osage_dc_6[(offsetof(OsageNode, linkFlags) == 0x0e) ? 1 : -1];
typedef char Layout_osage_dc_7[(offsetof(OsageNode, transform) == 0x10) ? 1 : -1];
typedef char Layout_osage_dc_8[(offsetof(OsageNode, current) == 0x90) ? 1 : -1];
typedef char Layout_osage_dc_9[(offsetof(OsageNode, previous) == 0x9c) ? 1 : -1];
typedef char Layout_osage_dc_10[(offsetof(OsageNode, facingAngle) == 0xb4) ? 1 : -1];
typedef char Layout_osage_dc_11[(offsetof(OsageNode, euler) == 0xb8) ? 1 : -1];
typedef char Layout_osage_dc_12[(offsetof(OsageNode, pitchPhase) == 0xdc) ? 1 : -1];
typedef char Layout_osage_dc_13[(offsetof(OsageNode, offsets) == 0xe4) ? 1 : -1];
typedef char Layout_osage_dc_14[(offsetof(OsageNode, parent) == 0x138) ? 1 : -1];
typedef char Layout_osage_dc_15[(offsetof(OsageNode, next) == 0x140) ? 1 : -1];
typedef char Layout_osage_dc_16[(offsetof(OsageNode, vertexBindings) == 0x144) ? 1 : -1];
typedef char Layout_osage_dc_17[(offsetof(OsageNode, meshAllocation) == 0x184) ? 1 : -1];
typedef char Layout_osage_dc_18[(offsetof(OsageNode, originalMesh) == 0x188) ? 1 : -1];
typedef char Layout_osage_dc_19[(sizeof(OsageWork) == 0x210) ? 1 : -1];
typedef char Layout_osage_dc_20[(offsetof(OsageWork, owner) == 8) ? 1 : -1];
typedef char Layout_osage_dc_21[(offsetof(OsageWork, collisionPoints) == 0x24) ? 1 : -1];
typedef char Layout_osage_dc_22[(offsetof(OsageWork, secondaryCollisionPoints) == 0x28) ? 1 : -1];
typedef char Layout_osage_dc_23[(offsetof(OsageWork, motionScale) == 0x1ec) ? 1 : -1];
typedef char Layout_osage_dc_24[(offsetof(OsageWork, distanceLimit) == 0x1f0) ? 1 : -1];
typedef char Layout_osage_dc_25[(offsetof(OsageWork, nodes) == 0x1f4) ? 1 : -1];
typedef char Layout_osage_dc_26[(offsetof(OsageWork, parameters) == 0x1f8) ? 1 : -1];
void mtAssignOsage(Character *character);
void FindOSAG(Character *character);
void toggleOsagFlags(Character *character);
void SetOsageMode(Character *character, std::uint8_t mode);
void OsageUpdateNullsub(Character *character, void *payload);
void OsageCleanup(Character *character, void *payload);
void ReadOsageParameters(OsageWork *work);
OsageCollisionPoint *GetDefaultOsageCollisionPoints(shen_name character);
void AllocateOsageNode(OsageWork *work, MT7Node *node);
void AllocateOsageNodeTree(OsageWork *work, MT7Node *node);
void CleanupOsageNodes(OsageWork *work);
void UpdateCharacterOsage(MT7Node *model, Character *character);

float Noise(std::int32_t angle);

} // namespace s2
} // namespace shenmue

#endif
