#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/player_movement_dc.hpp"

namespace shenmue {
namespace s2 {

typedef PlayerPlacementNode PartyPlacementNode;
typedef char Layout_party_placement_native[(sizeof(PartyPlacementNode) == 0x64) ? 1 : -1];

struct PartyCompanion {
    std::uint32_t flags;
    shen_name name;
    Character *character;
    MotionWork *motionWork;
    std::uint8_t behaviorState, motionState;
    std::int8_t mode;
    std::uint8_t motionPhase;
    std::int16_t yaw;
    std::uint16_t retained16;
    Vector3 position;
    std::uint32_t stateFrames;
    std::byte retained28[8];
    std::uint32_t requestedAction;
    std::int16_t requestedYaw;
    std::uint16_t retained36;
    float runBlend;
    PlayerMovementState embeddedMovement;
    PlayerMovementState *movement;
    PartyPlacementNode placement, trailPlacement, avoidancePlacement;
    PartyPlacementNode *selectedPlacement;
    std::int8_t trailWriteIndex, trailCount;
    std::uint16_t retained2fa;
    Vector3 trail[32], previousPlayerPosition;
    float trailDistance;
    std::int16_t idleFrames;
    std::int8_t selectedTrailIndex, retained48f;
    shen_name doorId;
    std::int8_t doorPhase, stairDirection;
    std::uint16_t retained496;
    float followDistance, obstacleDistance;
    Character *obstacleCharacter;
    std::int32_t routeMarker;
    std::int16_t routeYaw;
    std::uint16_t retained4aa;
    std::int32_t routeIndex;
    float routeLateralOffset, routeWidth;
    Vector3 separationTarget, requestedPosition;
    std::int16_t requestedPositionYaw;
    std::uint16_t retained4d2;
    std::uint32_t frameCount;
};
typedef char Layout_party_companion_0[(sizeof(PartyCompanion) == 0x4d8) ? 1 : -1];
typedef char Layout_party_companion_1[(offsetof(PartyCompanion, embeddedMovement) == 0x3c) ? 1 : -1];
typedef char Layout_party_companion_2[(offsetof(PartyCompanion, movement) == 0x1c4) ? 1 : -1];
typedef char Layout_party_companion_3[(offsetof(PartyCompanion, placement) == 0x1c8) ? 1 : -1];
typedef char Layout_party_companion_4[(offsetof(PartyCompanion, trailPlacement) == 0x22c) ? 1 : -1];
typedef char Layout_party_companion_5[(offsetof(PartyCompanion, selectedPlacement) == 0x2f4) ? 1 : -1];
typedef char Layout_party_companion_6[(offsetof(PartyCompanion, trail) == 0x2fc) ? 1 : -1];
typedef char Layout_party_companion_7[(offsetof(PartyCompanion, previousPlayerPosition) == 0x47c) ? 1 : -1];
typedef char Layout_party_companion_8[(offsetof(PartyCompanion, followDistance) == 0x498) ? 1 : -1];
typedef char Layout_party_companion_9[(offsetof(PartyCompanion, routeMarker) == 0x4a4) ? 1 : -1];
typedef char Layout_party_companion_10[(offsetof(PartyCompanion, routeIndex) == 0x4ac) ? 1 : -1];
typedef char Layout_party_companion_11[(offsetof(PartyCompanion, frameCount) == 0x4d4) ? 1 : -1];

struct PartyCompanionTransition {
    std::uint32_t present;
    shen_name name;
    Vector3 position;
    std::int16_t yaw;
    std::uint8_t mode, retained17;
    std::int32_t routeIndex, routeMarker;
};
typedef char Layout_party_companion_12[(sizeof(PartyCompanionTransition) == 0x20) ? 1 : -1];
extern PartyCompanionTransition *g_pPartyCompanionTransition;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t AttachPartyCompanion(Character *character);
#else
bool AttachPartyCompanion(Character *character);
#endif
PartyCompanion *FindPartyCompanion(const Character *character);
#define PARTY_COMPANION_COMPONENT SHEN_NAME4('P', 'T', 'Y', 'C')
PartyCompanion *FindPartyCompanionByName(shen_name name);
std::int32_t PausePartyConversation(shen_name name);
std::int32_t ResumePartyConversation(shen_name name);
static const shen_name kNavigationPartyNames[3] = {SHEN_NAME(REN_), SHEN_NAME(SIN_), SHEN_NAME(JOP_)};
shen_name FindNavigationPartyCompanion(shen_name name);
std::int32_t FindPTYCForChara(shen_name name);
std::int32_t SetPartyCompanionMode(shen_name name, std::int32_t mode);
std::int32_t ActivatePartyCompanion(shen_name name);
std::int32_t DeactivatePartyCompanion(shen_name name);
std::int32_t PTYCUtils(shen_name name, std::int32_t command, std::int32_t argument);
std::int32_t ptycCharaUnlock(shen_name name);
std::int32_t FreeInactivePartyCompanion(shen_name name);
std::int32_t SetPartyNpcRouteReload(shen_name name, std::int32_t enabled);
std::int32_t plymGetPartyAuthorStatus(shen_name name);
std::int32_t GetPartyCompanionRouteIndex(shen_name name);
struct ProcGenInfo {
    std::int8_t initialised, maze;
    std::uint16_t retained02;
    float parameter;
    shen_name name;
    std::int32_t routeIndex, currentParameter, low, high, stride, maximum;
    std::int8_t direction, partition;
    std::uint8_t retained26;
    std::int8_t partitionCount;
    std::int32_t targetPartition, targetParameter;
};
typedef char Layout_party_guide_0[(sizeof(ProcGenInfo) == 0x30) ? 1 : -1];
typedef char Layout_party_guide_1[(offsetof(ProcGenInfo, parameter) == 4) ? 1 : -1];
typedef char Layout_party_guide_2[(offsetof(ProcGenInfo, direction) == 0x24) ? 1 : -1];
extern ProcGenInfo *g_pProcGenInfos;
std::int32_t InitializePartyGuide();
std::int32_t PickClosestTargetForPlacement(const Vector3 *point);
void FindClosestTargetForPlacement(const Vector3 *point);
std::int32_t SetPartyGuideRouteHint(std::int32_t routeIndex);
std::int32_t SetPartyGuideDirection();
std::int32_t PartyGuideEndState();
std::int32_t AdvancePartyGuide(float distance);
std::int32_t SamplePartyGuide(Vector3 *point, std::int16_t *yaw);
std::int32_t ResetPartyCompanionTransition(PartyCompanionTransition *transition);
std::uint32_t GetPartyCompanionTransitionPending();
std::int32_t ImportPartyCompanionTransition(const PartyCompanionTransition &state);
std::int32_t ExportPartyCompanionTransition(std::uint8_t *state);

} // namespace s2
} // namespace shenmue

#elif !SHENMUE_IS_DC

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
struct PartyPlacementNode {
    std::uint32_t flags; // +0x00
    std::uint32_t reserved04;
    Character *character;     // +0x08
    Vector3 position;         // +0x10
    Vector3 previousPosition; // +0x1c
    Vector3 delta;            // +0x28
    std::int16_t targetYaw;   // +0x34
    std::int16_t characterBearingYaw;
    float distance; // +0x38
    float minimumDistance;
    std::byte reserved40[10];
    std::int16_t movementBearingYaw; // +0x4a
    Vector3 targetPosition;          // +0x4c
    Vector3 targetCharacterPosition; // +0x58
    std::int16_t stationaryFrames;   // +0x64
    std::uint16_t reserved66;
    PartyPlacementNode *next; // +0x68
};

// Native party guide walk (g_pProcGenInfos): the KMZ maze partition walk or the M01_ route walk that PTYC's guide placement and the
// PLYM placement builders share.
struct ProcGenInfo {
    bool initialised; // +0x00
    bool maze;        // +0x01
    std::uint16_t reserved02;
    float parameter;               // +0x04
    shen_name name;                // +0x08
    std::int32_t routeIndex;       // +0x0c
    std::int32_t currentParameter; // +0x10
    std::int32_t low;              // +0x14
    std::int32_t high;             // +0x18
    std::int32_t stride;           // +0x1c
    std::int32_t maximum;          // +0x20
    std::int8_t direction;         // +0x24
    std::int8_t partition;         // +0x25
    std::uint8_t reserved26;
    std::int8_t partitionCount;   // +0x27
    std::int32_t targetPartition; // +0x28
    std::int32_t targetParameter; // +0x2c
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
    std::uint32_t flags;        // +0x000
    shen_name name;             // +0x004
    Character *character;       // +0x008
    MotionWork *motionWork;     // +0x010
    std::uint8_t behaviorState; // +0x018
    std::uint8_t motionState;   // +0x019
    std::uint8_t mode;          // +0x01a
    std::uint8_t motionPhase;   // +0x01b
    std::int16_t yaw;           // +0x01c
    std::uint16_t reserved01e;
    Vector3 position;          // +0x020
    std::uint32_t stateFrames; // +0x02c
    std::byte reserved030[8];
    std::uint32_t requestedAction; // +0x038
    std::int16_t requestedYaw;     // +0x03c
    std::uint16_t reserved03e;
    float runBlend;                        // +0x040
    float speedScale;                      // +0x044
    PlayerMovementState embeddedMovement;  // +0x048
    PlayerMovementState *movement;         // +0x1e8
    PartyPlacementNode placement;          // +0x1f0
    PartyPlacementNode trailPlacement;     // +0x260
    PartyPlacementNode avoidancePlacement; // +0x2d0
    PartyPlacementNode *selectedPlacement; // +0x340
    std::int8_t trailWriteIndex;           // +0x348
    std::int8_t trailCount;                // +0x349
    std::uint16_t reserved34a;
    std::array<Vector3, 32> trail;  // +0x34c
    Vector3 previousPlayerPosition; // +0x4cc
    float trailDistance;            // +0x4d8
    std::int8_t selectedTrailIndex; // +0x4dc
    std::uint8_t reserved4dd;
    std::int16_t idleFrames; // +0x4de
    std::uint32_t doorId;    // +0x4e0
    std::int8_t doorPhase;   // +0x4e4
    std::int8_t stairDirection;
    std::uint16_t reserved4e6;
    float followDistance;         // +0x4e8
    float obstacleDistance;       // +0x4ec
    Character *obstacleCharacter; // +0x4f0
    std::int32_t routeMarker;     // +0x4f8
    std::int16_t routeYaw;        // +0x4fc
    std::uint16_t reserved4fe;
    std::int32_t routeIndex;  // +0x500
    float routeLateralOffset; // +0x504
    float routeWidth;         // +0x508
    Vector3 routeGoal;        // +0x50c
    Vector3 separationTarget; // +0x518
    Vector3 forcedTarget;     // +0x524
    std::int16_t forcedYaw;   // +0x530
    std::uint16_t reserved532;
    std::uint32_t frameCount;         // +0x534
    std::int16_t playerOverlapFrames; // +0x538
    std::byte reserved53a[6];
};

struct PartyCompanionTransition {
    std::uint32_t present;
    shen_name name;
    Vector3 position;  // +0x08
    std::int16_t yaw;  // +0x14
    std::uint8_t mode; // +0x16
    std::uint8_t reserved17;
    std::int32_t routeIndex;  // +0x18
    std::int32_t routeMarker; // +0x1c
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PartyCompanionNativeLayout {
    std::uint32_t flags;    // +0x000
    shen_name name;         // +0x004
    Character *character;   // +0x008
    MotionWork *motionWork; // +0x010
    std::byte reserved018[0x04];
    std::int16_t yaw; // +0x01c
    std::uint16_t reserved01e;
    Vector3 translation; // +0x020
    std::byte reserved02c[0x0c];
    std::uint32_t state; // +0x038
    std::byte reserved03c[0x04];
    float blend40; // +0x040
    float blend44; // +0x044
    std::byte embedded048[0x164];
    std::byte reserved1ac[0x3c];
    PlayerMovementState *movement; // +0x1e8
    std::uint8_t reserved1f0;      // +0x1f0
    std::byte reserved1f1[0x33];
    float resolvedY; // +0x224
    std::byte reserved228[0xa8];
    std::byte region2d0[0x70];
    std::uint64_t reserved340;
    std::uint16_t reserved348;
    std::array<Vector3, 32> trail; // +0x34a
    std::uint16_t reserved4ca;
    std::uint64_t reserved4cc;
    std::uint32_t reserved4d4;
    std::uint32_t reserved4d8;
    std::uint32_t reserved4dc;
    shen_name name4e0;           // +0x4e0
    std::uint8_t collisionState; // +0x4e4
    std::uint8_t stepState;      // +0x4e5
    std::byte reserved4e6[0x02];
    std::byte reserved4e8[0x08];
    void *reserved4f0;
    std::byte reserved4f8[0x0c];
    std::byte reserved504[0x08];
    std::uint8_t reserved50c; // +0x50c
    std::byte reserved50d[0x0b];
    Vector3 position; // +0x518
    std::byte reserved524[0x10];
    std::uint32_t frameCount; // +0x534
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
// Native plymGetPartyAuthorStatus: the named companion's author-target status, 0 settled, 1 busy, 2 following or arrived.
std::int32_t plymGetPartyAuthorStatus(shen_name name);
std::int32_t ptycCharaUnlock(shen_name name);
void ResetPartyCompanionTransition();
std::uint32_t GetPartyCompanionTransitionPending();
std::int32_t ImportPartyCompanionTransition(const PartyCompanionTransition &state);
std::int32_t ExportPartyCompanionTransition(PartyCompanionTransition &state);

} // namespace shenmue::s2

#endif
