#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct VFMAData;
struct VFActor;
struct VFFightStage;

#pragma pack(push, 1)

struct VFCLTaskData {
    std::uint32_t name;
    std::uint32_t reserved04;
    VFMAData *owner;
    VFActor *defender;
    VFActor *attacker;
    std::uint32_t flags;
    std::uint32_t contactType;
    std::uint32_t maskA;
    std::uint32_t maskB;
    float slideScale;
    float stickScale;
    std::uint32_t attackerMask;
    std::uint32_t defenderMask;
    float penetration;
    float specialPenetration;
    float separationLength;
    Vector3 separation;
    Vector3 previousContact[6];
    Vector3 currentContact[6];
    std::uint32_t terrainOverlapFlags[6];
    std::uint32_t ringContactFlags[6];
    Vector2 contactDisplacement[6];
};
enum class VFParticipant : std::int8_t {
    Main = 1,
    Sub = 2,
};

struct VFCLParticipantTmp {
    VFParticipant type;
    std::int8_t index;
    std::uint8_t link;
    std::uint8_t reserved03;
    std::int32_t cba;
    union {
        void *fcmg;
        VFActor *actor;
    };
    float x;
    float z;
};
#pragma pack(pop)

static_assert(sizeof(VFCLTaskData) == 0x148);
static_assert(offsetof(VFCLTaskData, owner) == 0x08);
static_assert(offsetof(VFCLTaskData, defender) == 0x10);
static_assert(offsetof(VFCLTaskData, flags) == 0x20);
static_assert(offsetof(VFCLTaskData, contactType) == 0x24);
static_assert(offsetof(VFCLTaskData, separation) == 0x4c);
static_assert(offsetof(VFCLTaskData, previousContact) == 0x58);
static_assert(offsetof(VFCLTaskData, currentContact) == 0xa0);
static_assert(offsetof(VFCLTaskData, terrainOverlapFlags) == 0xe8);
static_assert(offsetof(VFCLTaskData, ringContactFlags) == 0x100);
static_assert(offsetof(VFCLTaskData, contactDisplacement) == 0x118);
static_assert(sizeof(VFCLParticipantTmp) == 0x18);


constexpr float kVFContactRadius = 0.89999998f;
constexpr Vector3 kVFContactRootOffset{0.0f, -0.265f, 0.0f};
constexpr std::uint8_t kVFContactRootNode = 0;

enum class VFCLPhase : std::int32_t {
    BroadPhase = 0,
    AttackDefense = 1,
};

void vfclPrepareContact(VFCLTaskData *context, std::uint8_t aIndex, std::uint8_t bIndex);
std::int64_t vfResolveAttackOrPush(VFCLParticipantTmp *participants, int self, int other);
void vfResolveMutualPush(VFCLParticipantTmp *participants, int entityA, int entityB);
void vfclFindContactType(VFCLTaskData *context);
void vfclComputePenetrationAndOverlap(VFCLTaskData *context, VFActor *defender, VFActor *attacker);
void vfclResolveHitVsHitOutcome(VFCLTaskData *context, std::uint8_t entityA, std::uint8_t entityB);
void vfBuildContactsAndResolvePushOrHit(VFCLTaskData *context, VFCLParticipantTmp *participants, int participantLen, VFCLPhase phase);
void vfclEnforceRingBoundary(VFCLTaskData *context);
void vfclAdjustToGroundOrSlope(VFCLTaskData *context);
void vfResolveTerrainColi(VFCLTaskData *context);

void vfUpdateStageObjects(VFMAData *game);
VFMAData *vfclResolvePairedContactDrift(VFMAData *game);
void check_radius_intersections();
VFActor *barelyUsed(VFMAData *game);

HLib::Task *EnqueueVFCL(VFMAData *game);
void GetTransformAndTransformVector3(VFActor &actor);
void VFCL_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
