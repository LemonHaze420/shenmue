#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
struct PartyPlacementNode {
    std::uint32_t flags;
    std::uint32_t reserved04;
    Character *character;
    Vector3 position;
    Vector3 previousPosition;
    Vector3 delta;
    std::int16_t targetYaw;
    std::int16_t characterBearingYaw;
    float distance;
    float minimumDistance;
    std::byte reserved40[10];
    std::int16_t movementBearingYaw;
    Vector3 targetPosition;
    Vector3 targetCharacterPosition;
    std::int16_t stationaryFrames;
    std::uint16_t reserved66;
    PartyPlacementNode *next;
};



struct ProcGenInfo {
    bool initialised;
    bool maze;
    std::uint16_t reserved02;
    float parameter;
    shen_name name;
    std::int32_t routeIndex;
    std::int32_t currentParameter;
    std::int32_t low;
    std::int32_t high;
    std::int32_t stride;
    std::int32_t maximum;
    std::int8_t direction;
    std::int8_t partition;
    std::uint8_t reserved26;
    std::int8_t partitionCount;
    std::int32_t targetPartition;
    std::int32_t targetParameter;
    std::uint64_t reserved30;
};
static_assert(sizeof(ProcGenInfo) == 0x38);
static_assert(offsetof(ProcGenInfo, routeIndex) == 0x0c);
static_assert(offsetof(ProcGenInfo, direction) == 0x24);
static_assert(offsetof(ProcGenInfo, targetParameter) == 0x2c);

extern ProcGenInfo g_procGenInfos;
std::int32_t InitializePartyGuide();
std::int32_t PickClosestTargetForPlacement(const Vector3 *point);
void FindClosestTargetForPlacement(const Vector3 *point);
void SetPartyGuideRouteHint(std::int32_t routeIndex);
std::int32_t SetPartyGuideDirection();
std::int32_t PartyGuideEndState();
std::int32_t AdvancePartyGuide(float distance);
void SamplePartyGuide(Vector3 *point, std::int16_t *yaw);
std::int32_t SamplePartyGuideTarget(Vector3 *point);

// PTYC owns its movement state; it is not the player's PLYM payload.
struct PartyCompanion {
    std::uint32_t flags;
    shen_name name;
    Character *character;
    MotionWork *motionWork;
    std::uint8_t behaviorState;
    std::uint8_t motionState;
    std::uint8_t mode;
    std::uint8_t motionPhase;
    std::int16_t yaw;
    std::uint16_t reserved01e;
    Vector3 position;
    std::uint32_t stateFrames;
    std::byte reserved030[8];
    std::uint32_t requestedAction;
    std::int16_t requestedYaw;
    std::uint16_t reserved03e;
    float runBlend;
    float speedScale;
    PlayerMovementState embeddedMovement;
    PlayerMovementState *movement;
    PartyPlacementNode placement;
    PartyPlacementNode trailPlacement;
    PartyPlacementNode avoidancePlacement;
    PartyPlacementNode *selectedPlacement;
    std::int8_t trailWriteIndex;
    std::int8_t trailCount;
    std::uint16_t reserved34a;
    std::array<Vector3, 32> trail;
    Vector3 previousPlayerPosition;
    float trailDistance;
    std::int8_t selectedTrailIndex;
    std::uint8_t reserved4dd;
    std::int16_t idleFrames;
    std::uint32_t doorId;
    std::int8_t doorPhase;
    std::int8_t stairDirection;
    std::uint16_t reserved4e6;
    float followDistance;
    float obstacleDistance;
    Character *obstacleCharacter;
    std::int32_t routeMarker;
    std::int16_t routeYaw;
    std::uint16_t reserved4fe;
    std::int32_t routeIndex;
    float routeLateralOffset;
    float routeWidth;
    Vector3 routeGoal;
    Vector3 separationTarget;
    Vector3 forcedTarget;
    std::int16_t forcedYaw;
    std::uint16_t reserved532;
    std::uint32_t frameCount;
    std::int16_t playerOverlapFrames;
    std::byte reserved53a[6];
};

struct PartyCompanionTransition {
    std::uint32_t present;
    shen_name name;
    Vector3 position;
    std::int16_t yaw;
    std::uint8_t mode;
    std::uint8_t reserved17;
    std::int32_t routeIndex;
    std::int32_t routeMarker;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PartyCompanionNativeLayout {
    std::uint32_t flags;
    shen_name name;
    Character *character;
    MotionWork *motionWork;
    std::byte reserved018[0x04];
    std::int16_t yaw;
    std::uint16_t reserved01e;
    Vector3 translation;
    std::byte reserved02c[0x0c];
    std::uint32_t state;
    std::byte reserved03c[0x04];
    float blend40;
    float blend44;
    std::byte embedded048[0x164];
    std::byte reserved1ac[0x3c];
    PlayerMovementState *movement;
    std::uint8_t reserved1f0;
    std::byte reserved1f1[0x33];
    float resolvedY;
    std::byte reserved228[0xa8];
    std::byte region2d0[0x70];
    std::uint64_t reserved340;
    std::uint16_t reserved348;
    std::array<Vector3, 32> trail;
    std::uint16_t reserved4ca;
    std::uint64_t reserved4cc;
    std::uint32_t reserved4d4;
    std::uint32_t reserved4d8;
    std::uint32_t reserved4dc;
    shen_name name4e0;
    std::uint8_t collisionState;
    std::uint8_t stepState;
    std::byte reserved4e6[0x02];
    std::byte reserved4e8[0x08];
    void *reserved4f0;
    std::byte reserved4f8[0x0c];
    std::byte reserved504[0x08];
    std::uint8_t reserved50c;
    std::byte reserved50d[0x0b];
    Vector3 position;
    std::byte reserved524[0x10];
    std::uint32_t frameCount;
    std::byte reserved538[0x07];
};

struct PartyCompanionNativeAllocation {
    PartyCompanionNativeLayout data;
    std::byte reserved53f[0x01];
};
#pragma pack(pop)

static_assert(sizeof(PartyPlacementNode) == 0x70);
static_assert(offsetof(PartyPlacementNode, next) == 0x68);
static_assert(sizeof(PartyCompanion) == 0x540);
static_assert(offsetof(PartyCompanion, embeddedMovement) == 0x48);
static_assert(offsetof(PartyCompanion, movement) == 0x1e8);
static_assert(offsetof(PartyCompanion, placement) == 0x1f0);
static_assert(offsetof(PartyCompanion, selectedPlacement) == 0x340);
static_assert(offsetof(PartyCompanion, trail) == 0x34c);
static_assert(offsetof(PartyCompanion, doorId) == 0x4e0);
static_assert(offsetof(PartyCompanion, routeMarker) == 0x4f8);
static_assert(offsetof(PartyCompanion, frameCount) == 0x534);
static_assert(sizeof(PartyCompanionTransition) == 0x20);
static_assert(sizeof(PartyCompanionNativeLayout) == 0x53f);
static_assert(offsetof(PartyCompanionNativeLayout, flags) == 0x000);
static_assert(offsetof(PartyCompanionNativeLayout, name) == 0x004);
static_assert(offsetof(PartyCompanionNativeLayout, character) == 0x008);
static_assert(offsetof(PartyCompanionNativeLayout, motionWork) == 0x010);
static_assert(offsetof(PartyCompanionNativeLayout, reserved018) == 0x018);
static_assert(offsetof(PartyCompanionNativeLayout, yaw) == 0x01c);
static_assert(offsetof(PartyCompanionNativeLayout, reserved01e) == 0x01e);
static_assert(offsetof(PartyCompanionNativeLayout, translation) == 0x020);
static_assert(offsetof(PartyCompanionNativeLayout, reserved02c) == 0x02c);
static_assert(offsetof(PartyCompanionNativeLayout, state) == 0x038);
static_assert(offsetof(PartyCompanionNativeLayout, reserved03c) == 0x03c);
static_assert(offsetof(PartyCompanionNativeLayout, blend40) == 0x040);
static_assert(offsetof(PartyCompanionNativeLayout, blend44) == 0x044);
static_assert(offsetof(PartyCompanionNativeLayout, embedded048) == 0x048);
static_assert(offsetof(PartyCompanionNativeLayout, reserved1ac) == 0x1ac);
static_assert(offsetof(PartyCompanionNativeLayout, movement) == 0x1e8);
static_assert(offsetof(PartyCompanionNativeLayout, reserved1f0) == 0x1f0);
static_assert(offsetof(PartyCompanionNativeLayout, reserved1f1) == 0x1f1);
static_assert(offsetof(PartyCompanionNativeLayout, resolvedY) == 0x224);
static_assert(offsetof(PartyCompanionNativeLayout, reserved228) == 0x228);
static_assert(offsetof(PartyCompanionNativeLayout, region2d0) == 0x2d0);
static_assert(offsetof(PartyCompanionNativeLayout, reserved340) == 0x340);
static_assert(offsetof(PartyCompanionNativeLayout, reserved348) == 0x348);
static_assert(offsetof(PartyCompanionNativeLayout, trail) == 0x34a);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4ca) == 0x4ca);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4cc) == 0x4cc);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4d4) == 0x4d4);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4d8) == 0x4d8);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4dc) == 0x4dc);
static_assert(offsetof(PartyCompanionNativeLayout, name4e0) == 0x4e0);
static_assert(offsetof(PartyCompanionNativeLayout, collisionState) == 0x4e4);
static_assert(offsetof(PartyCompanionNativeLayout, stepState) == 0x4e5);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4e6) == 0x4e6);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4e8) == 0x4e8);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4f0) == 0x4f0);
static_assert(offsetof(PartyCompanionNativeLayout, reserved4f8) == 0x4f8);
static_assert(offsetof(PartyCompanionNativeLayout, reserved504) == 0x504);
static_assert(offsetof(PartyCompanionNativeLayout, reserved50c) == 0x50c);
static_assert(offsetof(PartyCompanionNativeLayout, reserved50d) == 0x50d);
static_assert(offsetof(PartyCompanionNativeLayout, position) == 0x518);
static_assert(offsetof(PartyCompanionNativeLayout, reserved524) == 0x524);
static_assert(offsetof(PartyCompanionNativeLayout, frameCount) == 0x534);
static_assert(offsetof(PartyCompanionNativeLayout, reserved538) == 0x538);
static_assert(sizeof(PartyCompanionNativeAllocation) == 0x540);
static_assert(offsetof(PartyCompanionNativeAllocation, data) == 0x000);
static_assert(offsetof(PartyCompanionNativeAllocation, reserved53f) == 0x53f);

PartyCompanion *FindPartyCompanion(const Character *character);
std::int64_t ReleaseAbsentPartyCharacter(shen_name name);
bool AttachPartyCompanion(Character *character);
std::int32_t ActivatePartyCompanion(shen_name name);
std::int32_t DeactivatePartyCompanion(shen_name name);
std::int32_t UnlockPartyCompanion(shen_name name);
shen_name FindNavigationPartyCompanion(shen_name name);
bool IsPartyCompanionPlaced(shen_name name);
bool IsPartyCompanionOnStairs(shen_name name);
std::int32_t SetPartyConversationPaused(shen_name name, bool paused);
std::int32_t PTYCUtils(shen_name name, std::int32_t command, std::int32_t argument);
std::int32_t FindPTYCForChara(shen_name name);

std::int32_t plymGetPartyAuthorStatus(shen_name name);
std::int32_t ptycCharaUnlock(shen_name name);
void ResetPartyCompanionTransition();
std::uint32_t GetPartyCompanionTransitionPending();
std::int32_t ImportPartyCompanionTransition(const PartyCompanionTransition &state);
std::int32_t ExportPartyCompanionTransition(PartyCompanionTransition &state);

} // namespace shenmue::s2
