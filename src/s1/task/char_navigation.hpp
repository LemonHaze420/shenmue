#pragma once
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct Character;
struct LineWalkCircleConstraint;
struct LineWalkPathNode;
struct LineWalkQuery;
struct LineWalkWork;
struct PartyTrailSegment;
struct PartyWork;
struct PatrolMotionParameters;
struct PatrolPathNode;
struct PatrolPoint;
struct PatrolWork;
struct TimedPartyWork;
struct Walk3DArc;
struct Walk3DPathKey;
struct Walk3DSegment;
struct Walk3DWork;

bool PATW_Allocate(Character *character);
std::int32_t PATW_HandleCommand(Character *character, const std::int32_t *arguments);
void PTMV_Allocate(Character *character);
void PTMV_ControlTrailFollow(Character *character, Character *source, std::uint8_t mode, bool stop);
void PTTM_Allocate(Character *character);
void PTTM_AssignSource(Character *character, Character *source, std::uint8_t mode);
void PTTM_Stop(Character *character);

struct Walk3DPathKey {
    Vector3 position;
    float radius;
    std::uint32_t event;
};
static_assert(sizeof(Walk3DPathKey) == 0x14);
bool W3DW_Allocate(Character *character);
void W3DW_SetupPath(Character *character, Walk3DPathKey *path, std::uint32_t scriptFlags);
bool W3DW_Activate(Character *character, float step, std::int32_t &mode);
std::uint32_t W3DW_GetState(Character *character);
void W3DW_EditPathPoint(Walk3DPathKey *path, std::int32_t index, const std::uint32_t *values);
void W3DW_SetDisabled(Character *character, bool disabled);
void W3DW_ResetSegment(Character *character, std::uint16_t index);

bool LNWK_Allocate(Character *character);
void LNWK_BeginPath(Character *character, std::byte *taskData, std::int32_t pathOffset, std::int32_t yawTableOffsetOrLimit, std::uint32_t flags);
bool LNWK_SetMoveScaleAndGetPathCommand(Character *character, float scale, std::int32_t &command);
std::int32_t LNWK_GetStatus(Character *character);
void LNWK_PatchPathNode(std::byte *taskData, std::int32_t pathOffset, std::int32_t nodeIndex, std::uint32_t xBits, std::uint32_t zBits,
    std::uint32_t radiusBits, std::uint32_t typeBits);
void LNWK_SetYawBase(Character *character, std::int32_t yawBase);
#pragma pack(push, 1)
struct LineWalkPathNode {
    std::int16_t command;
    std::int16_t type;
    float x;
    float z;
    float radius;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LineWalkCircleConstraint {
    Vector2 start;
    Vector2 end;
    Vector2 center;
    float radius;
    std::int32_t angleStart;
    std::int32_t angleEnd;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LineWalkWork {
    std::uint8_t state;
    std::uint8_t motionMode;
    std::uint8_t turnAnimationIndex;
    std::uint8_t reserved03;
    std::uint16_t animationState;
    std::int16_t savedAnimationId;
    const std::int16_t *turnAnimationTable;
    float savedAnimationRate;
    std::uint32_t segmentIndex;
    std::int32_t segmentCount;
    std::uint8_t flags;
    std::uint8_t flagByte1;
    std::uint8_t flagByte2;
    std::uint8_t flagByte3;
    std::int32_t yawBase;
    std::int32_t yawTarget;
    std::int32_t yawOffset;
    std::int32_t yawLimit;
    Vector3 motion;
    std::int32_t reserved3c;
    LineWalkPathNode *paths;
    Vector2 obstaclePosition;
    float collisionRadius;
    std::int32_t command;
    Vector2 position;
    LineWalkCircleConstraint constraint;
    std::byte reserved84[4];
};
#pragma pack(pop)

struct LineWalkQuery {
    Character *character;
    LineWalkWork *work;
};

struct Walk3DArc {
    Vector3 previousPosition;
    Vector3 exitPosition;
    Vector3 center;
    Vector3 tangent;
    Vector3 radiusAxis;
    Vector3 binormal;
};

struct Walk3DWork {
    std::uint32_t flags;
    std::int16_t index;
    std::uint16_t count;
    std::int32_t mode;
    std::uint32_t pathEndMode;
    std::int32_t yaw;
    std::int32_t pitch;
    float step;
    std::uint32_t reserved1c;
    Walk3DPathKey *path;
    Vector3 currentPosition;
    Walk3DArc arc;
    std::uint32_t reserved7c;
};

struct Walk3DSegment {
    std::int16_t currentIndex;
    std::int16_t nextIndex;
    Vector3 delta;
    float distance;
};

struct PartyTrailSegment {
    Vector3 source;
    Vector3 offset;
};

struct PartyWork {
    Character *source;
    Character *owner;
    std::uint8_t active;
    std::uint8_t mode;
    std::byte reserved12[2];
    std::uint32_t savedMotionFlags;
    std::uint8_t savedBlendMode;
    std::byte reserved19;
    std::uint16_t savedBlendFrames;
    float savedPlaybackRate;
    float savedInterpolationAmount;
    std::uint32_t savedMotionEnabled;
    std::uint8_t needsPathInitialization;
    std::byte reserved29[3];
    std::uint32_t runLatch;
    float followSpeedScale;
    float walkDistance;
    float runDistance;
    float lateralOffset;
    float forwardOffset;
    float walkStep;
    float runStep;
    std::uint32_t moveState;
    std::uint16_t animationId;
    std::uint16_t reserved52;
    PartyTrailSegment latest;
    PartyTrailSegment trail[50];
    std::int32_t trailWriteIndex;
    std::int32_t trailTargetIndex;
    float sourceSteps[5];
    float sourceStepSum;
    std::int32_t sourceStepCursor;
    float averageSourceStep;
    float currentStep;
    std::int32_t followerYaw;
    std::uint32_t reserved54c;
};

struct TimedPartyWork {
    Character *source;
    Character *owner;
    std::uint8_t active;
    std::uint8_t mode;
    std::byte reserved12[6];
};

enum class PatrolState : std::uint8_t { CheckPathNode, Linger, MoveToPathNode, ResolveTransition, Arc, FollowPoints, Count };

struct PatrolPoint {
    float x;
    float z;
};

struct PatrolPathNode {
    PatrolPoint point;
    std::uint8_t nextIndex;
    std::uint8_t reserved09;
    std::uint16_t reserved0a;
};

struct PatrolMotionParameters {
    std::uint16_t motionIds[3];
    std::uint16_t reserved06;
    float moveSpeed;
    float alternateSpeed;
    float fallbackDistance;
};

struct PatrolWork {
    PatrolState state;
    PatrolState pendingState;
    std::uint8_t recordedPointCount;
    std::uint8_t recordedPointCursor;
    std::uint8_t pathNodeIndex;
    std::uint8_t pathNodeNextIndex;
    std::uint8_t pathNodeCount;
    std::int8_t timer;
    std::uint8_t flags;
    std::uint8_t result;
    std::uint8_t phase;
    std::uint8_t initFlags;
    std::int32_t desiredFacing;
    std::int32_t arcFacing;
    float speed;
    PatrolPoint velocity;
    float distanceToTarget;
    std::uint32_t reserved24;
    const PatrolPathNode *pathNodes;
    const PatrolMotionParameters *motionParameters;
    const std::uint16_t *turnMotions;
    Vector3 arcAnchor;
    PatrolPoint recordedPoints[4];
    std::uint32_t reserved6c;
};
} // namespace shenmue::s1
