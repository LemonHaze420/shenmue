#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

void UpdatePartyCompanion(Character *character, void *payload);

float SquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    return x * x + y * y + z * z;
}

// Vector3::ComputeSquaredDistance: a separation under the epsilon on every axis is zero.
// @verified - p
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    if (std::fabs(x) < kCharacterControlEpsilon && std::fabs(y) < kCharacterControlEpsilon && std::fabs(z) < kCharacterControlEpsilon)
        return 0.0f;
    return (y * y + x * x) + z * z;
}

// The XZ form; a separation under the epsilon on both axes is zero.
// @verified - p
float Vector3_PlanarSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    if (std::fabs(x) < kCharacterControlEpsilon && std::fabs(z) < kCharacterControlEpsilon)
        return 0.0f;
    return z * z + x * x;
}

// @verified - p
Vector3 Vector3_ScaleVectorToMagnitude(const Vector3 &vector, float magnitude) {
    const float scale = magnitude / std::sqrt((vector.x * vector.x + vector.y * vector.y) + vector.z * vector.z);
    return {vector.x * scale, scale * vector.y, scale * vector.z};
}

std::int16_t Vector3_GetAngleBetweenPointsXZ(const Vector3 &from, const Vector3 &to) {
    return InverseFixedAtan2(to.x - from.x, to.z - from.z);
}

// The point in the frame at origin turned by yaw, through the prepass-based work matrix.
Vector3 PartyLocalPosition(const Vector3 &position, const Vector3 &origin, std::int16_t yaw) {
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    Matrix4x4::TransformPoint(position, local);
    Matrix4x4::Pop(1);
    return local;
}

Vector3 PartyWorldPosition(const Vector3 &local, const Vector3 &origin, std::int16_t yaw) {
    Vector3 world{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::TransformPoint(local, world);
    Matrix4x4::Pop(1);
    return world;
}

// The translation row after offsetting the turned frame, as the placement offsets read it.
Vector3 PartyOffsetPosition(const Vector3 &origin, std::int16_t yaw, float x, float y, float z) {
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::preTranslate(x, y, z);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return position;
}

float PartyHorizontalDistanceSquared(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float z = left.z - right.z;
    return x * x + z * z;
}

// @verified - p
std::int32_t PartyTrailIndex(const PartyCompanion &party, std::int32_t relative) {
    if (!party.trailCount || !relative)
        return -1;
    const std::int32_t direction = relative > 0 ? 1 : -1;
    std::int32_t distance = (relative > 0 ? relative : -relative) - 1;
    if (distance >= 32)
        distance = 31;
    if (distance >= party.trailCount)
        distance = party.trailCount - 1;
    std::int32_t index = party.trailWriteIndex;
    if (party.trailCount < 32 && direction < 0)
        index = 0;
    if (direction > 0 && --index < 0)
        index = 31;
    index -= distance * direction;
    if (index < 0)
        index += 32;
    if (index > 31)
        index -= 32;
    return index;
}

// @verified - p
void RecordPartyPlayerTrail(PartyCompanion &party, const Vector3 &position) {
    party.trailDistance += std::sqrt(SquaredDistance(position, party.previousPlayerPosition));
    const std::int32_t previous = PartyTrailIndex(party, 1);
    if (!party.trailCount || (party.trailDistance > 1.0f && Vector3_ComputeSquaredDistance(party.trail[previous], position) > 0.80000001f)) {
        party.trail[party.trailWriteIndex] = position;
        if (++party.trailWriteIndex >= 32)
            party.trailWriteIndex = 0;
        if (++party.trailCount > 32)
            party.trailCount = 32;
        party.trailDistance = 0.0f;
    }
    party.previousPlayerPosition = position;
}

std::uint16_t PartyIdleMotion(const PartyCompanion &party) {
    std::uint16_t motion = 0;
    if (party.name.name == kPartyNames[0].name)
        motion = 41476;
    else if (party.name.name == kPartyNames[1].name)
        motion = 38147;
    else if (party.name.name == kPartyNames[2].name)
        motion = 32885;
    if (motion && FindLoadedMotionSequence(motion))
        return motion;
    constexpr std::array<std::uint16_t, 16> idleMotions{
        61503, 61535, 61567, 61527, 61599, 61650, 61537, 61575, 61573, 61561, 61529, 61608, 61573, 61605, 61607, 61674};
    const std::int32_t rig = GetCharacterBoneId(party.character);
    motion = rig >= 0 && rig < static_cast<std::int32_t>(idleMotions.size()) ? idleMotions[rig] : 61573;
    if (rig == 8 && party.name.name == kPartyNames[0].name)
        motion = 41476;
    return FindLoadedMotionSequence(motion) ? motion : 61442;
}

// @verified - p
void AdjustPartyControllerFloor(std::uint32_t, Vector3 *target, MotionWork *, void *) {
    if (!target)
        return;
    const Vector3 probe{target->x, target->y - 0.117f + 0.5f, target->z};
    const float floor = shenFindIntersectionWithScene(probe, probe.z);
    if (floor != -(std::numeric_limits<float>::max)()) {
        const float height = floor + 0.117f;
        if (height > target->y)
            target->y = height;
    }
}

// @verified - p
void ConfigurePartyMotion(PartyCompanion &party) {
    MotionWork *const work = party.motionWork;
    CopyMotionTransition(work, nullptr, 0, 1, 0);
    const std::uint16_t idle = PartyIdleMotion(party);
    mtwkSetRequestedRootMode(work, 1);
    SetMotionFrameTime(work, 20, MotionRegion::LowerBody);
    mtwkRequestMotion(work, idle, MotionRegion::LowerBody);
    SetMotionTimeRelated(work, 1, 0, idle);
    mtwkSetExtraFlag(work, 55, 0);
    AssignMotionTargetCallback(work, AdjustPartyControllerFloor, 25);
    AssignMotionTargetCallback(work, AdjustPartyControllerFloor, 30);
}

// @verified - p
void plymUpdatePlacementNode(PartyPlacementNode &node) {
    node.delta = {node.position.x - node.previousPosition.x, node.position.y - node.previousPosition.y, node.position.z - node.previousPosition.z};
    node.movementBearingYaw = std::fabs(node.delta.x) < kCharacterControlEpsilon || std::fabs(node.delta.z) < kCharacterControlEpsilon ?
        0 :
        static_cast<std::int16_t>(FixedAtan2(node.delta.x, node.delta.z) + 0x8000U);
    node.previousPosition = node.position;
    const Vector3 location = GetCharacterWorldPosition(node.character);
    const std::int16_t yaw = GetAllAngYForNode(&node.character->entity.node);
    Vector3 offset{node.position.x - location.x, node.position.y - location.y, node.position.z - location.z};
    const float distance = std::sqrt(offset.z * offset.z + offset.x * offset.x);
    const auto bearing = static_cast<std::int16_t>(FixedAtan2(offset.x, offset.z) + 0x8000U);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::Transform(offset, offset);
    Matrix4x4::Pop(1);
    node.distance = distance;
    if (node.minimumDistance > distance)
        node.minimumDistance = distance;
    std::uint32_t flags = node.flags;
    if (offset.z <= 0.0f)
        flags &= ~6U;
    else {
        if (!(flags & 2U))
            flags |= 4U;
        flags |= 2U;
    }
    flags &= ~0x4000U;
    if ((node.delta.x * node.delta.x + node.delta.y * node.delta.y) + node.delta.z * node.delta.z >= 0.0001f)
        node.stationaryFrames = 0;
    else {
        if (++node.stationaryFrames >= 4)
            node.stationaryFrames = 4;
        if (node.stationaryFrames == 4)
            flags |= 0x4000U;
    }
    node.distance = distance;
    node.flags = flags;
    node.characterBearingYaw = bearing;
}

// Each linked node takes the head's character; the last node is the one followed.
// @verified - p
PartyPlacementNode *plymUpdatePlacementChain(PartyPlacementNode *node) {
    if (!node)
        return nullptr;
    Character *const character = node->character;
    plymUpdatePlacementNode(*node);
    for (PartyPlacementNode *next = node->next; next; next = next->next) {
        next->character = character;
        node = next;
        plymUpdatePlacementNode(*next);
    }
    return node;
}

// @verified - p
void PreparePartyFollowPlacement(PartyCompanion &party) {
    if (!ctrlGetRootNode())
        return;
    Vector3 player = GetPlayerTargetWorldPosition();
    RecordPartyPlayerTrail(party, player);
    Vector3 position = party.position;
    const PlayerMovementState &movement = *party.movement;
    float height = position.y;
    if (movement.flags & 8U) {
        const Vector3 local = PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw);
        height = std::fmax(0.0f, -local.z * 0.74999994f);
        if (static_cast<float>(movement.targetHeightScale) * 0.22499999f <= height)
            height = static_cast<float>(movement.targetHeightScale) * 0.22499999f;
        height += movement.targetWorldPosition.y;
    }
    player.y += 0.40000001f;
    position.y = height + 0.40000001f;
    Vector3 offset{position.x - player.x, position.y - player.y, position.z - player.z};
    const float distance = Vector3::NormalizeInPlace(offset);
    const float length = distance <= party.followDistance ? distance : party.followDistance;
    offset = {offset.x * length, offset.y * length, offset.z * length};
    Vector3 target{player.x + offset.x, player.y + offset.y, player.z + offset.z};
    target.y += 1.5f;
    const float ground = shenFindIntersectionWithScene(target, target.y);
    target.y = ground == kNoSceneIntersection ? player.y : ground + 0.40000001f;
    party.placement.position = target;
    party.placement.targetYaw = static_cast<std::int16_t>(InverseFixedAtan2(offset.x, offset.z) + 0x8000);
    if (party.flags & 0x20000U) {
        party.placement.position = party.forcedTarget;
        party.placement.targetYaw = party.forcedYaw;
    }
    if (party.flags & 0x80000U) {
        std::int32_t nearestTarget = -1;
        std::int32_t nearestParty = -1;
        float targetDistance = (std::numeric_limits<float>::max)();
        float partyDistance = (std::numeric_limits<float>::max)();
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const float fromTarget = Vector3_PlanarSquaredDistance(party.placement.position, party.trail[index]);
            if (targetDistance > fromTarget) {
                targetDistance = fromTarget;
                nearestTarget = relative - 1;
            }
        }
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const float fromParty = Vector3_PlanarSquaredDistance(position, party.trail[index]);
            if (partyDistance > fromParty) {
                partyDistance = fromParty;
                nearestParty = relative - 1;
            }
        }
        if (party.selectedTrailIndex >= 0) {
            std::int32_t retained = -1;
            if (party.trailCount) {
                retained = party.trailWriteIndex - (party.selectedTrailIndex + 1);
                if (retained < 0)
                    retained += party.trailCount;
                if (retained > party.trailCount - 1)
                    retained -= party.trailCount;
            }
            if (retained < nearestParty)
                nearestParty = retained;
        }
        if (nearestParty > nearestTarget && nearestParty > 0) {
            const float minimumDistance = party.runBlend >= 0.5f ? 1.6500001f : 1.1f;
            for (std::int32_t relative = nearestParty + 1; relative - 1 >= nearestTarget; --relative) {
                const std::int32_t index = PartyTrailIndex(party, relative);
                if (index < 0)
                    break;
                if (minimumDistance * minimumDistance <= Vector3_PlanarSquaredDistance(position, party.trail[index])) {
                    party.placement.next = &party.trailPlacement;
                    party.trailPlacement.position = party.trail[index];
                    break;
                }
            }
        }
        party.selectedTrailIndex = static_cast<std::int8_t>(PartyTrailIndex(party, nearestParty + 1));
        if (party.placement.next)
            return;
    } else {
        party.selectedTrailIndex = -1;
    }
    flddToggleFlag(0x100);
    if (!flddFindIntersections(player, position))
        return;
    Vector3 previous = target;
    for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, relative);
        if (index < 0)
            break;
        Vector3 sample = party.trail[index];
        sample.y += 0.40000001f;
        flddToggleFlag(0x100);
        if (!flddFindIntersections(sample, position)) {
            Vector3 chosen = sample;
            if (Vector3_ComputeSquaredDistance(sample, position) < 4.0f) {
                flddPushIntersectionFlag(0x40);
                if (flddFindIntersections(sample, previous)) {
                    chosen = previous;
                    party.trailPlacement.flags |= 0x40000U;
                }
            }
            party.placement.next = &party.trailPlacement;
            party.trailPlacement.position = chosen;
            break;
        }
        previous = sample;
    }
    if (!party.placement.next)
        party.placement.flags |= 8U;
}

// @verified - p
bool PartySegmentHitsCylinder(const Vector3 &start, const Vector3 &end, const Vector3 &center, float radius) {
    const float radiusSquared = radius * radius;
    const float maximumY = center.y + 3.25f;
    if (PartyHorizontalDistanceSquared(start, center) <= radiusSquared && start.y >= center.y && start.y <= maximumY)
        return true;
    const Vector3 delta{end.x - start.x, end.y - start.y, end.z - start.z};
    if (!(std::abs(delta.x) >= kCharacterControlEpsilon || std::abs(delta.z) >= kCharacterControlEpsilon || std::abs(delta.z) >= kCharacterControlEpsilon))
        return false;
    if (PartyHorizontalDistanceSquared(end, center) <= radiusSquared && end.y >= center.y && end.y <= maximumY)
        return true;
    const float fraction = ((center.x - start.x) * delta.x + (center.z - start.z) * delta.z) / (delta.z * delta.z + delta.x * delta.x);
    if (!(fraction >= 0.0f && fraction <= 1.0f))
        return false;
    const Vector3 nearest{start.x + fraction * delta.x, start.y + fraction * delta.y, start.z + fraction * delta.z};
    return PartyHorizontalDistanceSquared(nearest, center) <= radiusSquared && nearest.y >= center.y && nearest.y <= maximumY;
}

// Three probes from the companion toward the followed node, offset 0.4 to either side; the nearest blocker is kept.
// @verified - p
void FindPartyObstacles(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 sideways{0.40000001f, 0.0f, 0.0f};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(placement.characterBearingYaw));
    Matrix4x4::TransformPoint(sideways, sideways);
    Matrix4x4::Pop(1);
    float plusDistance = 10.0f;
    float minusDistance = 10.0f;
    float centreDistance = 10.0f;
    party.obstacleDistance = 10.0f;
    party.obstacleCharacter = nullptr;
    const Vector3 start{party.position.x, party.position.y + 0.40000001f, party.position.z};
    const Vector3 end{placement.position.x, placement.position.y + 0.40000001f, placement.position.z};
    const Vector3 plusStart{start.x + sideways.x, start.y + sideways.y, start.z + sideways.z};
    const Vector3 plusEnd{end.x + sideways.x, end.y + sideways.y, end.z + sideways.z};
    const Vector3 minusStart{start.x - sideways.x, start.y - sideways.y, start.z - sideways.z};
    const Vector3 minusEnd{end.x - sideways.x, end.y - sideways.y, end.z - sideways.z};
    ccmUpdateEntityFlag(party.character);
    Character *const centre = CheckPointInCylinder_Wrapper(start, end);
    if (centre)
        centreDistance = Vector3_PlanarSquaredDistance(start, GetCharacterWorldPosition(centre));
    ccmUpdateEntityFlag(party.character);
    Character *const minus = CheckPointInCylinder_Wrapper(minusStart, minusEnd);
    if (minus)
        minusDistance = Vector3_PlanarSquaredDistance(minusStart, GetCharacterWorldPosition(minus));
    ccmUpdateEntityFlag(party.character);
    Character *const plus = CheckPointInCylinder_Wrapper(plusStart, plusEnd);
    if (plus)
        plusDistance = Vector3_PlanarSquaredDistance(plusStart, GetCharacterWorldPosition(plus));
    if (minusDistance <= centreDistance || plusDistance <= centreDistance) {
        if (plusDistance <= minusDistance) {
            party.obstacleDistance = plusDistance;
            party.obstacleCharacter = plus;
        } else {
            party.obstacleDistance = minusDistance;
            party.obstacleCharacter = minus;
        }
    } else {
        party.obstacleDistance = centreDistance;
        party.obstacleCharacter = centre;
    }
    if (party.obstacleCharacter)
        party.obstacleDistance = std::sqrt(party.obstacleDistance);
}

// A blocker within 3 units appends the avoidance node one unit to its free side.
// @verified - p
void SelectPartyAvoidancePlacement(PartyCompanion &party) {
    PartyPlacementNode &placement = *party.selectedPlacement;
    if (!party.obstacleCharacter) {
        party.flags &= 0xffff3fffU;
        return;
    }
    const Vector3 local = PartyLocalPosition(GetCharacterWorldPosition(party.obstacleCharacter), party.position, placement.characterBearingYaw);
    std::int16_t direction = 0;
    if (!(party.flags & 0xc000U))
        direction = local.x < 0.0f ? 0x4000 : static_cast<std::int16_t>(0x8000);
    if (party.obstacleDistance > 3.0f)
        return;
    const float side = direction < 0 ? -1.0f : 1.0f;
    PartyPlacementNode &avoidance = party.avoidancePlacement;
    avoidance.position = PartyOffsetPosition(party.position, placement.characterBearingYaw, side, 0.0f, -party.obstacleDistance);
    Vector3 hit{};
    if (RaycastForCollision(&hit, &avoidance.position, &party.position))
        avoidance.position = PartyOffsetPosition(party.position, placement.characterBearingYaw, -side, 0.0f, -party.obstacleDistance);
    avoidance.targetYaw = placement.targetYaw;
    placement.next = &avoidance;
    party.selectedPlacement = &avoidance;
}

} // namespace model_component_detail



// @verified - p
std::int32_t InitializePartyGuide() {
    ProcGenInfo &guide = g_procGenInfos;
    guide.initialised = false;
    guide.maze = false;
    guide.routeIndex = -1;
    guide.parameter = 0.0f;
    guide.name = {};
    guide.currentParameter = 0;
    guide.low = 0;
    guide.stride = 1;
    guide.maximum = 0;
    guide.direction = 1;
    guide.partition = -1;
    guide.partitionCount = 0;
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_MAZE)) {
        guide.maze = true;
        guide.partitionCount = static_cast<std::int8_t>(mazeGetNumPartitions());
    } else {
        guide.maze = false;
        guide.name = SHEN_NAME(M01_);
        guide.currentParameter = GetNpcRouteCurrentParameter(guide.name);
        guide.stride = GetNpcRouteStride(guide.name);
        guide.low = GetNpcRouteLowParameter(guide.name);
        guide.high = GetNpcRouteHighParameter(guide.name);
        guide.maximum = GetNpcRouteMaximumParameter(guide.name);
        guide.parameter = static_cast<float>(guide.currentParameter);
        if (!IsNpcRouteReady(guide.name))
            return 0;
    }
    guide.initialised = true;
    return 1;
}



// @verified - p
std::int32_t PickClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 0;
    if (!guide.maze) {
        guide.currentParameter = FindNearestNpcRouteParameter(guide.name, point);
        guide.parameter = static_cast<float>(guide.currentParameter);
    } else {
        std::int32_t partition = -1;
        std::int32_t element = -1;
        if (guide.routeIndex != -1) {
            partition = static_cast<std::uint16_t>(guide.routeIndex >> 16);
            element = static_cast<std::uint16_t>(guide.routeIndex);
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.partition = static_cast<std::int8_t>(partition);
        guide.currentParameter = element;
        guide.low = 0;
        guide.parameter = static_cast<float>(element);
        guide.high = GetMZNRDataForID(guide.partition) - 1;
        guide.maximum = guide.high;
    }
    guide.routeIndex = guide.maze ? static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(guide.partition)) << 16U) |
            static_cast<std::uint16_t>(guide.currentParameter) :
                                    guide.currentParameter;
    return guide.routeIndex;
}



// @verified - p
void FindClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return;
    if (!point) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = guide.high;
    } else if (!guide.maze) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = FindNearestNpcRouteParameter(guide.name, point);
    } else {
        std::int32_t partition = -1;
        std::int32_t element = -1;
        if (guide.routeIndex != -1) {
            partition = static_cast<std::uint16_t>(guide.routeIndex >> 16);
            element = static_cast<std::uint16_t>(guide.routeIndex);
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.targetPartition = partition;
        guide.targetParameter = element;
    }
}


// @verified - p
void SetPartyGuideRouteHint(std::int32_t routeIndex) {
    g_procGenInfos.routeIndex = routeIndex;
}


// @verified - p
std::int32_t SetPartyGuideDirection() {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 0;
    std::int32_t direction = 0;
    if (guide.maze) {
        std::int32_t partition, element;
        direction = mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element);
    } else {
        const float target = static_cast<float>(guide.targetParameter);
        direction = target > guide.parameter;
        if (guide.parameter > target)
            direction = -1;
    }
    guide.direction = static_cast<std::int8_t>(direction);
    return direction;
}



// @verified - p
std::int32_t PartyGuideEndState() {
    const ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 99;
    if (!guide.direction)
        return 1;
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition) {
            if (guide.parameter >= static_cast<float>(guide.targetParameter))
                return 1;
        } else if (guide.parameter >= static_cast<float>(guide.high))
            return -1;
    } else {
        if (guide.partition == guide.targetPartition) {
            if (static_cast<float>(guide.targetParameter) >= guide.parameter)
                return 1;
        } else if (static_cast<float>(guide.low) >= guide.parameter)
            return -1;
    }
    return 0;
}


// @verified - p
std::int32_t AdvancePartyGuidePartition() {
    ProcGenInfo &guide = g_procGenInfos;
    if (guide.maze) {
        std::int32_t partition = guide.partition;
        std::int32_t element = static_cast<std::int32_t>(guide.parameter);
        if (!mazeFindPartitionDirection(guide.partition, element, guide.targetPartition, guide.targetParameter, &partition, &element))
            return 0;
        guide.partition = static_cast<std::int8_t>(partition);
        guide.low = 0;
        guide.parameter = static_cast<float>(element);
        guide.high = GetMZNRDataForID(partition) - 1;
        guide.direction = static_cast<std::int8_t>(mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element));
    }
    return 1;
}



// @verified - p
std::int32_t AdvancePartyGuide(float distance) {
    ProcGenInfo &guide = g_procGenInfos;
    std::int32_t steps = 0;
    if (guide.maze) {
        steps = static_cast<std::int32_t>((distance + 1.99999f) * 0.5f);
    } else {
        float length = 0.0f;
        std::int32_t parameter = static_cast<std::int32_t>(guide.parameter);
        if (guide.direction > 0) {
            while (static_cast<float>(guide.maximum) > static_cast<float>(parameter)) {
                const float segment = GetNpcRouteSegmentLength(guide.name, parameter++);
                if (segment < 0.0f)
                    break;
                length += segment;
                ++steps;
                if (length > distance)
                    break;
            }
        } else {
            while (parameter > 0) {
                const float segment = GetNpcRouteSegmentLength(guide.name, --parameter);
                if (segment < 0.0f)
                    break;
                length += segment;
                ++steps;
                if (length > distance)
                    break;
            }
        }
    }
    guide.parameter += static_cast<float>(steps * guide.direction);
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition && guide.parameter > static_cast<float>(guide.targetParameter))
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter > static_cast<float>(guide.maximum))
            guide.parameter = static_cast<float>(guide.maximum);
    } else if (guide.direction < 0) {
        if (guide.partition == guide.targetPartition && static_cast<float>(guide.targetParameter) > guide.parameter)
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter < 0.0f)
            guide.parameter = 0.0f;
    }
    const std::int32_t end = PartyGuideEndState();
    if (end > 0)
        return 1;
    if (end < 0)
        AdvancePartyGuidePartition();
    return 0;
}

std::int32_t PartyGuideParameter() {
    const ProcGenInfo &guide = g_procGenInfos;
    return std::min(std::max(static_cast<std::int32_t>(guide.parameter), guide.low), guide.high);
}


// @verified - p
void SamplePartyGuide(Vector3 *point, std::int16_t *yaw) {
    const ProcGenInfo &guide = g_procGenInfos;
    const std::int32_t parameter = PartyGuideParameter();
    if (!guide.maze) {
        std::int32_t angle = 0;
        SampleNpcRoute(guide.name, parameter, point, &angle);
        *yaw = static_cast<std::int16_t>(angle);
        return;
    }
    mazeSamplePartition(guide.partition, parameter, point);
    Vector3 adjacent{};
    const bool forward = guide.direction >= 0;
    if ((forward && parameter < guide.high) || (!forward && parameter > guide.low)) {
        mazeSamplePartition(guide.partition, parameter + (forward ? 1 : -1), &adjacent);
        *yaw = Vector3_GetAngleBetweenPointsXZ(*point, adjacent);
    } else {
        mazeSamplePartition(guide.partition, parameter + (forward ? -1 : 1), &adjacent);
        *yaw = Vector3_GetAngleBetweenPointsXZ(adjacent, *point);
    }
}


// @verified - p
std::int32_t SamplePartyGuideTarget(Vector3 *point) {
    const ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised || !point)
        return -1;
    if (guide.maze) {
        mazeSamplePartition(guide.targetPartition, guide.targetParameter, point);
    } else {
        std::int32_t angle = 0;
        SampleNpcRoute(guide.name, guide.targetParameter, point, &angle);
    }
    return 0;
}

namespace model_component_detail {


void ReleaseCompletedPartyGuideRoute() {
    constexpr shen_name guideName{SHEN_NAME(M01_)};
    if (g_currentMap.name == SHEN_NAME(KSH1) || !IsNpcRouteReady(guideName) || !InitializePartyGuide())
        return;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    PickClosestTargetForPlacement(&player);
    FindClosestTargetForPlacement(nullptr);
    if (SetPartyGuideDirection() <= 0) {
        ReleaseNpcRouteControl(nullptr, guideName, 0);
        EV_npcLockUnlock(guideName);
    }
}

void PartyGuideLaneBounds(float &low, float &high) {
    low = -1.5f;
    high = 1.5f;
    if (!g_procGenInfos.maze)
        GetNpcRouteLaneBounds(g_procGenInfos.name, PartyGuideParameter(), &low, &high);
}

void PreparePartyGuidePlacement(PartyCompanion &party) {
    if (!InitializePartyGuide()) {
        party.placement.flags |= 8U;
        return;
    }
    g_procGenInfos.routeIndex = party.routeIndex;
    party.routeIndex = PickClosestTargetForPlacement(&party.position);
    Vector3 goal{-111.4f, 23.511999f, -132.3f};
    if ((party.flags & 0x40000000U) && party.routeMarker)
        mazeGetMarkerPosition(party.routeMarker, &goal);
    FindClosestTargetForPlacement((party.flags & 0x40000000U) ? &goal : nullptr);
    SamplePartyGuideTarget(&party.routeGoal);
    SetPartyGuideDirection();
    if (PartyGuideEndState() > 0) {
        party.placement.flags |= 0x20U;
        return;
    }
    Vector3 sample{};
    std::int16_t yaw = 0;
    SamplePartyGuide(&sample, &yaw);
    party.routeYaw = yaw;
    float low = 0.0f, high = 0.0f;
    PartyGuideLaneBounds(low, high);
    party.routeWidth = std::abs(low) + std::abs(high);
    const Vector3 local = PartyLocalPosition(sample, party.position, party.yaw);
    if ((local.z > -1.0f || Vector3_PlanarSquaredDistance(party.position, sample) < 2.25f) && PartyGuideEndState() <= 0)
        AdvancePartyGuide(1.0f);
    if (PartyGuideEndState() <= 0)
        AdvancePartyGuide(2.0f);
    SamplePartyGuide(&sample, &yaw);
    if (Vector3_ComputeSquaredDistance(party.position, sample) < 1.0f && PartyGuideEndState() <= 0) {
        AdvancePartyGuide(2.0f);
        SamplePartyGuide(&sample, &yaw);
    }
    party.placement.position = sample;
    party.placement.targetYaw = yaw;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    player = PartyLocalPosition(player, party.position, party.routeYaw);
    PartyGuideLaneBounds(low, high);
    const float lateral = std::min(std::fmax(-1.0f, player.x >= 0.0f ? low + 0.75f : high - 0.75f), 1.0f);
    party.routeLateralOffset += (lateral - party.routeLateralOffset) * 0.079999998f;
    party.placement.position = PartyOffsetPosition(party.placement.position, party.placement.targetYaw, party.routeLateralOffset, 0.0f, 0.0f);
    flddToggleFlag(1064);
    if (!GlobalRayIntersectionTest(party.position, party.placement.position, 0.34999999f))
        return;
    InitializePartyGuide();
    PickClosestTargetForPlacement(&party.placement.position);
    g_procGenInfos.routeIndex = party.routeIndex;
    FindClosestTargetForPlacement(&party.position);
    SetPartyGuideDirection();
    while (PartyGuideEndState() <= 0) {
        SamplePartyGuide(&sample, &yaw);
        sample.y += 0.40000001f;
        flddToggleFlag(1064);
        if (!GlobalRayIntersectionTest(party.position, sample, 0.34999999f)) {
            party.placement.next = &party.trailPlacement;
            party.trailPlacement.position = sample;
            return;
        }
        AdvancePartyGuide(5.0f);
    }
}

// @verified - p
void PreparePartyPlacement(PartyCompanion &party) {
    party.placement.character = party.character;
    party.placement.next = nullptr;
    party.trailPlacement.next = nullptr;
    party.avoidancePlacement.next = nullptr;
    party.placement.flags &= 0x7ffbffd7U;
    party.trailPlacement.flags &= 0x7ffbf7d7U;
    party.trailPlacement.flags |= 0x800U;
    party.avoidancePlacement.flags &= 0x7ff9ffd7U;
    party.avoidancePlacement.flags |= 0x20000U;
    if (party.mode == 1)
        PreparePartyFollowPlacement(party);
    else if (party.mode == 2)
        PreparePartyGuidePlacement(party);
    party.selectedPlacement = plymUpdatePlacementChain(&party.placement);
}

void PublishPartyRelocation(PartyCompanion &party, const Vector3 &position) {
    shenApplyTranslationToAll(&party.character->entity.node, position);
    party.position = position;
    party.character->alpha = 0.0099999998f;
    if (party.motionWork)
        SetMotionRegionFlag(party.motionWork, 71, 0, MotionRegion::LowerBody);
}

// Out of sight and out of reach, the companion reappears behind the player (guide: ahead of them). Returns -1 when
// already close, -2 when a trail sample still sees it, -3 when nothing separates the two bodies.
// @verified - p
std::int32_t ResetPartyPlacementNearPlayer(PartyCompanion &party) {
    Vector3 offset{0.0f, 0.0f, -1.0f};
    Vector3 position = GetCharacterWorldPosition(party.character);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    auto yaw = static_cast<std::int16_t>(playerGetRotY());
    player.y = player.y + 0.40000001f;
    position.y = position.y + 0.40000001f;
    Vector3 hit{};
    const bool blocked = RaycastForCollision(&hit, &player, &position);
    player.y = player.y + -0.40000001f;
    position.y = position.y + -0.40000001f;
    const float distance = Vector3_PlanarSquaredDistance(position, player);
    if (blocked) {
        if (std::fabs(position.y - player.y) < 2.0f && distance < 100.0f) {
            Vector3 playerBone{}, partyBone{};
            GetCharacterMotionWorldPosition(ctrlGetRootNode(), 6, &playerBone);
            GetCharacterMotionWorldPosition(party.character, 6, &partyBone);
            if (!flddFindIntersections(playerBone, partyBone))
                return -3;
        }
    } else if (distance < 900.0f && std::fabs(position.y - player.y) < 5.0f)
        return -1;
    position.y = position.y + 0.40000001f;
    if (party.mode == 1) {
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const Vector3 sample{party.trail[index].x, party.trail[index].y + 0.40000001f, party.trail[index].z};
            if (!flddFindIntersections(sample, position))
                return -2;
        }
    }
    offset.z = party.mode == 2 ? -1.5f : -1.25f;
    position = PartyWorldPosition(offset, player, static_cast<std::int16_t>(yaw + (party.mode == 2 ? -8192 : -24576)));
    if (RaycastForCollision(&hit, &position, &player)) {
        const Vector3 back = Vector3_ScaleVectorToMagnitude({player.x - hit.x, player.y - hit.y, player.z - hit.z}, 0.0099999998f);
        position = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
    }
    position.y = position.y + 0.40000001f;
    RaycastInWorldGrid(position, position, GetCharacterCollisionRadius(party.character));
    position.y = position.y + -0.40000001f;
    if (party.mode != 2)
        yaw = Vector3_GetAngleBetweenPointsXZ(position, player);
    shenApplyTranslationToAll(&party.character->entity.node, position);
    shenDecrementRotYAllNodes(&party.character->entity.node, yaw);
    party.position = position;
    party.yaw = yaw;
    charaSetFlagFor(party.character, 1);
    toggleCCMflagsByAmt(party.character, CharacterControlResetMotion);
    if (party.motionWork)
        ConfigurePartyMotion(party);
    party.character->alpha = 0.0099999998f;
    if (party.motionWork)
        SetMotionRegionFlag(party.motionWork, 71, 0, MotionRegion::LowerBody);
    return 1;
}

// A follower left behind jumps to the newest trail sample ahead of the player that the camera cannot see.
// @verified - p
bool RelocatePartyAlongTrail(PartyCompanion &party) {
    if ((party.flags & 0x100U) || party.motionState == 5)
        return false;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const float maximumDistance = Vector3_PlanarSquaredDistance(party.position, player);
    const float minimumDistance = maximumDistance < 100.0f ? 0.0f : 25.0f;
    bool relocated = false;
    Vector3 sample{}, local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-Vector3_GetAngleBetweenPointsXZ(party.position, player)));
    Matrix4x4::preTranslate(-player.x, -player.y, -player.z);
    for (std::int32_t relative = 0; relative < party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, ~relative);
        if (index < 0)
            break;
        sample = party.trail[index];
        Matrix4x4::TransformPoint(sample, local);
        if (local.z < 0.0f)
            continue;
        const float distance = Vector3_PlanarSquaredDistance(player, sample);
        if (minimumDistance <= distance && distance <= maximumDistance) {
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
            Matrix4x4::TransformPoint(sample, local);
            Matrix4x4::Pop(1);
            if (local.z >= 0.0f || std::fabs(-(camCtrlGetHalfVerticalFovTangent() * local.z)) + 0.44999999f <= std::fabs(local.x)) {
                relocated = true;
                break;
            }
        }
    }
    Matrix4x4::Pop(1);
    if (relocated)
        PublishPartyRelocation(party, sample);
    return relocated;
}

// Kowloon's stacked floors: gravity is withheld while no floor is near, and a companion stranded above the player
// drops back to the last trail sample that still had ground under it.
// @verified - p
bool RelocatePartyAcrossQronFloor(PartyCompanion &party) {
    if (!MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) || !(GetQronCurrentFlags() & 1U))
        return false;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    Vector3 probe = party.position;
    probe.y += 0.5f;
    const float floor = shenFindIntersectionWithScene(probe, probe.y);
    if (floor == kNoSceneIntersection || party.position.y - floor >= 2.699999928474426)
        ccmSetFlagsFor(party.character, CharacterControlApplyGravity);
    else if (!(party.movement->flags & 8U))
        toggleCCMflagsByAmt(party.character, CharacterControlApplyGravity);
    if (party.motionState == 5)
        return false;
    if (floor != kNoSceneIntersection && (party.position.y - floor < 2.6999998f || party.position.y - player.y < 1.8f))
        return false;
    Vector3 previous = player;
    for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, relative);
        if (index < 0)
            break;
        Vector3 sample = party.trail[index];
        sample.y += 0.5f;
        const float ground = shenFindIntersectionWithScene(sample, sample.y);
        sample.y = party.trail[index].y;
        if (ground == kNoSceneIntersection || party.trail[index].y - ground > 1.8f) {
            PublishPartyRelocation(party, previous);
            return true;
        }
        sample.y = ground;
        previous = sample;
    }
    return false;
}

// A follower standing in the player's way steps to the side; on stairs the step stays on the companion's tread.
// @verified - p
bool FindPartySeparationTarget(PartyCompanion &party) {
    if (!ctrlGetRootNode())
        return false;
    const PlayerMovementState &movement = *party.movement;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const auto playerYaw = static_cast<std::uint16_t>(playerGetRotY());
    const Vector3 origin{party.position.x, party.position.y + 0.40000001f, party.position.z};
    Matrix4x4 playerFrame{};
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-playerYaw));
    Matrix4x4::preTranslate(-player.x, -player.y, -player.z);
    Matrix4x4::GetTransformMatrix(&playerFrame);
    Matrix4x4::TransformPoint(origin, local);
    Matrix4x4::Pop(1);
    const float radius = GetCharacterCollisionCapsuleMargin(ctrlGetRootNode());
    float width = radius * 2.0f;
    const float lateral = std::fabs(local.x);
    if (lateral > width * 0.94999999f)
        return false;
    bool separate = local.z >= 0.0f;
    if (!separate && std::fabs(local.z) < 1.5f) {
        if (lateral > 0.25f)
            separate = true;
        else if (((movement.flags & 8U) || plymIsLookAllowed(32)) && party.playerOverlapFrames % 150 == 0)
            separate = true;
    }
    if ((width * 0.94999999f) * (width * 0.94999999f) > Vector3_PlanarSquaredDistance(player, party.position))
        separate = true;
    if (!separate)
        return false;
    Vector3 first{width, local.y, local.z};
    Vector3 second{-width, local.y, local.z};
    Vector3 firstTarget{}, secondTarget{};
    std::int32_t firstStairs = 0;
    std::int32_t secondStairs = 0;
    Vector3 hit{};
    if (movement.flags & 8U) {
        auto stairYaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
        if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(stairYaw - playerYaw))) >= 0x4000)
            stairYaw = static_cast<std::uint16_t>(stairYaw + 0x8000U);
        width = 0.30000001f;
        first = {0.30000001f, 0.0f, 0.0f};
        second = {-0.30000001f, 0.0f, 0.0f};
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
        Matrix4x4::RotateAroundY_Wrapper(stairYaw);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        Matrix4x4::Pop(1);
        if (RaycastForCollision(&hit, &first, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({first.x - origin.x, first.y - origin.y, first.z - origin.z}, -0.0099999998f);
            first = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(firstTarget, first, radius);
        if (RaycastForCollision(&hit, &second, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({second.x - origin.x, second.y - origin.y, second.z - origin.z}, -0.0099999998f);
            second = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(secondTarget, second, radius);
        const Vector3 &stair = movement.targetWorldPosition;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
        Matrix4x4::preTranslate(-stair.x, -stair.y, -stair.z);
        Matrix4x4::TransformPoint(firstTarget, firstTarget);
        Matrix4x4::TransformPoint(secondTarget, secondTarget);
        Matrix4x4::Pop(1);
        firstTarget.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        firstTarget.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
        secondTarget.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        secondTarget.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(stair.x, stair.y, stair.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(movement.targetWorldYaw));
        Matrix4x4::TransformPoint(firstTarget, firstTarget);
        Matrix4x4::TransformPoint(secondTarget, secondTarget);
        Matrix4x4::Pop(1);
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(player.x, player.y, player.z);
        Matrix4x4::RotateAroundY_Wrapper(playerYaw);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        Matrix4x4::Pop(1);
        Vector3 stairOrigin{};
        std::int32_t stairYaw = 0;
        if (RaycastForCollision(&hit, &first, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({first.x - origin.x, first.y - origin.y, first.z - origin.z}, -0.0099999998f);
            first = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(firstTarget, first, radius);
        Vector3 probe{firstTarget.x, firstTarget.y + 0.5f, firstTarget.z};
        probe.y = shenFindIntersectionWithScene(probe, firstTarget.y + 0.5f);
        firstStairs = flddQueryStairOrigin(probe, stairOrigin, stairYaw);
        if (RaycastForCollision(&hit, &second, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({second.x - origin.x, second.y - origin.y, second.z - origin.z}, -0.0099999998f);
            second = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(secondTarget, second, radius);
        probe = {secondTarget.x, secondTarget.y + 0.5f, secondTarget.z};
        probe.y = shenFindIntersectionWithScene(probe, secondTarget.y + 0.5f);
        secondStairs = flddQueryStairOrigin(probe, stairOrigin, stairYaw);
    }
    Vector3 firstLocal{}, secondLocal{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&playerFrame);
    Matrix4x4::TransformPoint(firstTarget, firstLocal);
    Matrix4x4::TransformPoint(secondTarget, secondLocal);
    Matrix4x4::Pop(1);
    const bool preferFirst = local.z >= 0.0f ? local.x > 0.0f : local.x >= 0.0f;
    const double strict = static_cast<double>(width) * 0.85;
    const double loose = static_cast<double>(width) * 0.5;
    const float firstX = std::fabs(firstLocal.x);
    const float secondX = std::fabs(secondLocal.x);
    const Vector3 *target = nullptr;
    if (preferFirst) {
        if (firstX > strict && !firstStairs)
            target = &firstTarget;
        else if (secondX > strict && !secondStairs)
            target = &secondTarget;
        else if (firstX > loose && !firstStairs)
            target = &firstTarget;
        else if (secondX > loose && !secondStairs)
            target = &secondTarget;
    } else {
        if (secondX > strict && !secondStairs)
            target = &secondTarget;
        else if (firstX > strict && !firstStairs)
            target = &firstTarget;
        else if (secondX > loose && !secondStairs)
            target = &secondTarget;
        else if (firstX > loose && !firstStairs)
            target = &firstTarget;
    }
    if (!target)
        return false;
    party.separationTarget = *target;
    return true;
}

// Stair state: the first probe (feet, then 0.6 ahead along the facing, then along the requested yaw) that lands on a
// stair polygon latches the flight; the lower foot picks the tread and the requested yaw picks the climb direction.
// @verified - p
void UpdatePartyStairController(PartyCompanion &party) {
    PlayerMovementState &movement = *party.movement;
    FieldRadiusMetadata metadata{};
    std::int32_t steps = 0;
    movement.stepDirection = 0;
    const Vector3 position = GetCharacterWorldPosition(party.character);
    for (std::int32_t probeIndex = 0; probeIndex < 3 && !steps; ++probeIndex) {
        Vector3 probe =
            probeIndex == 0 ? position : PartyOffsetPosition(party.position, probeIndex == 1 ? party.yaw : party.requestedYaw, 0.0f, 0.0f, -0.60000002f);
        probe.y += 1.5f;
        const float floor = shenFindIntersectionWithScene(probe, probe.y);
        if (probe.y - floor >= 3.41999990940094)
            continue;
        probe.y = floor;
        steps = flddQueryRadiusMetadata(probe, metadata) ? static_cast<std::int32_t>(metadata.stepCount) : 0;
    }
    if (steps > 0) {
        const std::uint32_t controlFlags = GetCharacterControlModelFlags(party.character);
        const std::uint32_t flags = movement.flags;
        if (!(flags & 8U) || movement.targetWorldPosition.y != metadata.anchor.y) {
            movement.targetHeightScale = static_cast<std::int8_t>(steps);
            movement.targetWorldPosition = metadata.anchor;
            movement.targetWorldYaw = static_cast<std::int16_t>(metadata.wallYaw);
            if (!(flags & 8U))
                movement.cachedCharacterControlFlags = controlFlags;
            movement.targetStairEdgeLength = metadata.maximumEdgeLength;
        }
        movement.flags = flags | 8U;
        SetCharacterControlModelFlags(
            party.character, (controlFlags & 0xfffff6bdU) | (party.motionState == 0 && party.requestedAction <= 1U ? 0x942U : 0x900U));
    } else {
        if (movement.flags & 8U)
            SetCharacterControlModelFlags(party.character, movement.cachedCharacterControlFlags);
        movement.flags &= ~8U;
    }
    if (movement.flags & 8U) {
        const float margin = GetCharacterCollisionCapsuleMargin(party.character);
        if (!HasScenarioCharacterControl() && margin >= 0.32499999f)
            SetCharacterCollisionCapsuleMargin(party.character, 0.32499999f);
    }
    Vector3 right{}, left{};
    GetMotionControllerTargetPosition(party.motionWork, 6, right);
    GetMotionControllerTargetPosition(party.motionWork, 5, left);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
    Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
    Matrix4x4::TransformPoint(right, right);
    Matrix4x4::TransformPoint(left, left);
    Matrix4x4::Pop(1);
    auto step = static_cast<std::int32_t>(left.y / 0.22499999f);
    if (right.y / 0.22499999f < static_cast<float>(step))
        step = static_cast<std::int32_t>(right.y / 0.22499999f);
    const std::int32_t lowest = step < 0 ? 0 : step;
    movement.groundedStepIndex = static_cast<std::int8_t>(lowest >= movement.targetHeightScale ? movement.targetHeightScale : lowest);
    const float along = -PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw).z;
    movement.stepDirection = 0;
    if (steps <= 0)
        return;
    const std::int32_t yawDifference = std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(party.requestedYaw - movement.targetWorldYaw)));
    if (yawDifference > 0x4000) {
        if (along > -0.075000003f && static_cast<float>(steps) * 0.30000001f > along && movement.groundedStepIndex > 0)
            movement.stepDirection = -1;
    } else if (along > -0.30000001f && static_cast<float>(steps - 1) * 0.30000001f > along && movement.groundedStepIndex < steps) {
        movement.stepDirection = 1;
    }
}

// @verified - p
void UpdatePartyFollowBehavior(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    float distance = Vector3_ComputeSquaredDistance(player, party.position);
    const float heightDifference = std::abs(party.position.y - player.y);
    if (heightDifference > 3.239999914169311)
        distance += 36.0f;
    party.requestedAction = 0;
    if ((party.flags & 0x40U) && party.behaviorState != 7) {
        party.behaviorState = 7;
        party.requestedAction = 1;
    }
    bool relocated = false;
    if ((placement.flags & 8U) && RelocatePartyAlongTrail(party)) {
        party.behaviorState = party.motionState = 0;
        CopyMotionTransition(party.motionWork, nullptr, party.yaw, 13, 0);
        relocated = true;
    }
    if (RelocatePartyAcrossQronFloor(party)) {
        party.behaviorState = party.motionState = 0;
        CopyMotionTransition(party.motionWork, nullptr, party.yaw, 13, 0);
        relocated = true;
    }
    switch (party.behaviorState) {
    case 0:
        party.runBlend = 0.0f;
        if (relocated)
            break;
        if (party.motionState == 1) {
            party.behaviorState = 1;
            break;
        }
        ++party.idleFrames;
        party.requestedAction = 1;
        {
            const float limit = (party.flags & 0x20000U) ? 0.30250001f : (party.followDistance + 0.55000001f) * (party.followDistance + 0.55000001f);
            if (distance > limit) {
                party.requestedAction = 3;
                break;
            }
            if (distance > 4.0f) {
                const std::int16_t yaw = Vector3_GetAngleBetweenPointsXZ(party.position, player);
                if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(yaw - party.yaw))) > 0x4000) {
                    party.requestedAction = 2;
                    party.requestedYaw = yaw;
                    break;
                }
            }
            bool sameFloor = !MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) || !(GetQronCurrentFlags() & 1U);
            if (!sameFloor) {
                Vector3 search{};
                plymUpdatePlayerSearchLocation(&search);
                const std::int32_t floor = GetQronFloorAtPosition(party.position);
                sameFloor = GetQronFloorAtPosition(search) == floor;
            }
            if (sameFloor && party.frameCount > 10U && FindPartySeparationTarget(party))
                party.requestedAction = 13;
        }
        break;
    case 1: {
        const std::uint32_t door = flddQueryDoorIntersection(party.position);
        Vector3 hit{};
        if ((door & 0xff00U) != 0x3500U && door && (placement.flags & 0x40000U) && RaycastForCollision(&hit, &placement.position, &party.position)) {
            party.requestedAction = 11;
            party.behaviorState = 8;
            party.doorPhase = 0;
            party.doorId = door;
            break;
        }
        if (party.flags & 0x800U) {
            if (party.runBlend >= 0.5f) {
                if (distance < 100.0f)
                    party.runBlend = 0.0f;
            } else if (distance > 225.0f)
                party.runBlend = 1.0f;
        }
        const float limit = (party.flags & 0x20000U) ? 0.0f : party.followDistance * party.followDistance;
        if (limit > distance)
            party.requestedAction = 1;
        if (party.placement.distance < 0.55000001f && heightDifference < 1.8f)
            party.requestedAction = 1;
        if (plymIsInterpolationActive() && !(party.flags & 0x100U))
            party.speedScale = distance > 225.0f ? 1.5925951f : distance > 100.0f ? 1.4651874f : distance > 25.0f ? 1.3377799f : 1.0f;
        break;
    }
    case 6:
    case 7:
        party.requestedAction = 1;
        if (!(party.flags & 0x40U))
            party.behaviorState = 0;
        break;
    case 8:
        if (party.doorPhase == 4)
            party.behaviorState = 0;
        else
            party.requestedAction = 11;
        break;
    default:
        break;
    }
    if (party.behaviorState)
        party.idleFrames = 0;
}

// @verified - p
void UpdatePartyGuideBehavior(PartyCompanion &party) {
    const bool conversation = IsConversationCameraCharacter(party.character);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    if (party.flags & 0x40000U) {
        if (party.placement.distance > 0.55000001f)
            party.flags &= ~0x40000U;
    } else if (party.placement.distance <= 0.55000001f &&
        ((party.placement.flags & 0x20U) || (!party.behaviorState && Vector3_PlanarSquaredDistance(party.position, party.routeGoal) < 0.30250001f))) {
        party.flags |= 0x40000U;
    }
    party.requestedAction = 0;
    if (party.behaviorState == 1) {
        party.requestedAction = party.placement.distance < 0.55000001f ? 14 : 0;
        if ((party.selectedPlacement->flags & 8U) || (party.flags & 0x80U))
            party.requestedAction = 1;
    } else if (party.behaviorState) {
        party.runBlend = 0.0f;
        if (party.behaviorState == 6 || party.behaviorState == 7) {
            if (!conversation)
                party.behaviorState = 0;
            else if (party.flags & 0x40000U) {
                Vector3 target{};
                if (ctrlGetRootNode())
                    GetCharacterMotionWorldPosition(ctrlGetRootNode(), 5, &target);
                RequestNpcSpecialMotionTarget(party.name, &target);
            }
        }
    } else {
        party.runBlend = 0.0f;
        if (conversation && (party.flags & 0x40000U)) {
            party.behaviorState = 6;
            return;
        }
        if (party.placement.distance < 0.55000001f && (party.placement.flags & 0x20U)) {
            const Vector3 local = PartyLocalPosition(player, party.position, party.placement.targetYaw);
            const auto yaw = static_cast<std::int16_t>(party.placement.targetYaw + (local.x >= 0.0f ? -24576 : 24576));
            if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(yaw - party.yaw))) > 0x2000) {
                party.requestedYaw = yaw;
                party.requestedAction = 2;
                return;
            }
        }
        if (!(party.selectedPlacement->flags & 8U) && !(party.flags & 0x80U) && party.placement.distance >= 0.55000001f) {
            party.requestedAction = 3;
        }
    }
}

// The requested action drives the motion state: 0 idle, 1 following, 3 settling, 4 turning in place, 5 passing a door
// (claim, ingress, door animation, release), 6 climbing a stair tread. A settled stair position is re-snapped to the tread.
// @verified - p
void UpdatePartyMotionController(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    PlayerMovementState &movement = *party.movement;
    MotionWork *const work = party.motionWork;
    const Vector3 position = GetCharacterWorldPosition(party.character);
    UpdatePartyStairController(party);
    const std::uint8_t previousState = party.motionState;
    switch (party.motionState) {
    case 0:
        switch (party.requestedAction) {
        case 1:
        case 14:
            if (party.idleFrames > 150) {
                const std::uint16_t idle = PartyIdleMotion(party);
                if (GetMotionAnimationId(movement.motionWork, MotionRegion::LowerBody) != idle) {
                    mtwkSetRequestedRootMode(movement.motionWork, 1);
                    SetMotionFrameTime(movement.motionWork, 20, MotionRegion::LowerBody);
                    mtwkRequestMotion(movement.motionWork, idle, MotionRegion::LowerBody);
                }
            }
            break;
        case 2:
            CopyMotionTransition(work, nullptr, party.requestedYaw, 13, 0);
            party.motionState = 4;
            break;
        case 3:
            if (movement.stepDirection) {
                party.motionState = 6;
                party.motionPhase = 0;
            } else {
                CopyMotionTransition(work, &placement.position, placement.targetYaw, 0, 1);
                CopyMotionTransition(work, nullptr, 0, 16, 0);
                party.behaviorState = party.motionState = 1;
            }
            break;
        case 13: {
            const std::int16_t bearing = Vector3_GetAngleBetweenPointsXZ(party.separationTarget, *plymUpdatePlayerSearchLocation(nullptr));
            const std::int16_t yaw = (movement.flags & 8U) ? party.yaw : bearing;
            CopyMotionTransition(work, &party.separationTarget, yaw, 0, 0);
            CopyMotionTransition(work, nullptr, 0, 2, 0);
            party.motionState = 3;
            break;
        }
        default:
            break;
        }
        if (!party.motionState && (movement.flags & 8U) && movement.stepDirection)
            movement.flags |= 0x20000U;
        break;
    case 1:
        if (movement.stepDirection) {
            party.motionState = 6;
            party.motionPhase = 0;
            break;
        }
        CopyMotionTransition(work, &placement.position, placement.targetYaw, 0, 0);
        CopyMotionTransition(work, nullptr, 0, 16, 0);
        if (party.runBlend >= 0.5f) {
            if (!CopyMotionTransition(work, nullptr, 0, 15, 0))
                CopyMotionTransition(work, nullptr, 0, 8, 0);
            movement.field15c = party.speedScale;
        } else if (CopyMotionTransition(work, nullptr, 0, 15, 0)) {
            CopyMotionTransition(work, nullptr, 0, 14, 0);
        }
        if (party.requestedAction == 1 || party.requestedAction == 13) {
            CopyMotionTransition(work, nullptr, party.yaw, 13, 0);
            party.motionState = 3;
        } else if (party.requestedAction == 11) {
            CopyMotionTransition(work, nullptr, 0, 2, 0);
            party.motionState = 5;
        } else if (party.requestedAction == 14) {
            CopyMotionTransition(work, &placement.position, placement.targetYaw, 6, 0);
            party.motionState = 3;
        }
        if (!mtwkSetExtraFlag(work, 58, 1) && party.motionState == 1)
            party.motionState = 3;
        break;
    case 3:
        if (party.requestedAction == 3) {
            CopyMotionTransition(work, &placement.position, placement.targetYaw, 0, 0);
            party.motionState = 1;
        } else if (party.requestedAction == 11) {
            CopyMotionTransition(work, nullptr, party.yaw, 13, 0);
            party.motionState = 5;
        }
        if (party.motionState == 3) {
            if (CopyMotionTransition(work, nullptr, 0, 15, 0))
                CopyMotionTransition(work, nullptr, 0, 14, 0);
            if (CopyMotionTransition(work, nullptr, 0, 3, 0))
                CopyMotionTransition(work, nullptr, 0, 2, 0);
            if (party.stateFrames > 120U) {
                CopyMotionTransition(work, nullptr, party.yaw, 13, 0);
                party.behaviorState = party.motionState = 0;
            } else if (!mtwkSetExtraFlag(work, 58, 1)) {
                party.behaviorState = party.motionState = 0;
            }
        }
        break;
    case 4:
        if (!mtwkSetExtraFlag(work, 58, 1))
            party.motionState = 0;
        break;
    case 5:
        if (party.doorPhase == 0) {
            if (!ClaimDoor(shen_name{party.doorId}, GetCharacterScenarioName(party.character)))
                break;
            ++party.doorPhase;
        }
        switch (party.doorPhase) {
        case 1: {
            const Vector3 current = GetCharacterWorldPosition(party.character);
            Vector3 target{};
            ComputeDoorIngress(static_cast<std::int32_t>(party.doorId), &current, &target);
            if (CopyMotionTransition(work, &target, static_cast<std::uint16_t>(g_doorIngressRotation.y), 0, 0)) {
                const auto animation = static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(party.doorId), party.character, &current));
                SetMotionTimeRelated(work, 1, 5, animation);
                ++party.doorPhase;
                toggleCCMflagsByAmt(party.character, CharacterControlStairMode);
            }
            break;
        }
        case 2:
            if (!mtwkSetExtraFlag(work, 58, 1)) {
                const Vector3 current = GetCharacterWorldPosition(party.character);
                const auto animation = GetNpcDoorAnimation(static_cast<std::int32_t>(party.doorId), party.character, &current);
                BeginDoorAnimation(static_cast<std::int32_t>(party.doorId), GetCharacterScenarioName(party.character), static_cast<std::int32_t>(animation));
                ccmSetFlagsFor(party.character, CharacterControlFineRaycast);
                ++party.doorPhase;
            }
            break;
        case 3:
            if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
                toggleCCMflagsByAmt(party.character, CharacterControlFineRaycast);
                ccmSetFlagsFor(party.character, CharacterControlStairMode);
                ++party.doorPhase;
            }
            break;
        case 4:
            party.motionState = 0;
            party.doorPhase = 0;
            break;
        default:
            break;
        }
        break;
    case 6:
        switch (party.motionPhase) {
        case 0:
            if (!movement.stepDirection) {
                party.motionState = 1;
                break;
            }
            party.stairDirection = movement.stepDirection;
            {
                Vector3 step = PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw);
                step.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
                step.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
                if (step.x > 0.0f)
                    step.x = -GetCharacterCollisionRadius(party.character);
                if (-movement.targetStairEdgeLength > step.x)
                    step.x = GetCharacterCollisionRadius(party.character) - movement.targetStairEdgeLength;
                const Vector3 target = PartyWorldPosition(step, movement.targetWorldPosition, movement.targetWorldYaw);
                auto yaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
                if (party.stairDirection == -1)
                    yaw = static_cast<std::uint16_t>(yaw + 0x8000U);
                CopyMotionTransition(work, &target, yaw, 0, 0);
                CopyMotionTransition(work, nullptr, 0, 2, 0);
                ++party.motionPhase;
            }
            break;
        case 1:
            if (party.requestedAction == 1)
                party.motionState = 3;
            else if (CopyMotionTransition(work, nullptr, 0, 3, 0))
                party.motionPhase = 0;
            else if (!mtwkSetExtraFlag(work, 58, 1)) {
                ++party.motionPhase;
                mtwkSetRequestedRootMode(work, 3);
                SetMotionFrameTime(work, 1, MotionRegion::LowerBody);
                if (party.stairDirection == 1)
                    RequestPlayerMotion(movement, 61483);
                else if (party.stairDirection == -1)
                    RequestPlayerMotion(movement, 61449);
            }
            break;
        case 2:
            if (!movement.stepDirection || party.requestedAction == 1) {
                if ((movement.playerMotionFlags & 2U) && (movement.playerMotionFlags & 1U)) {
                    mtwkSetRequestedRootMode(movement.motionWork, 1);
                    SetMotionFrameTime(movement.motionWork, 20, MotionRegion::LowerBody);
                    mtwkRequestMotion(movement.motionWork, PartyIdleMotion(party), MotionRegion::LowerBody);
                    movement.flags |= 0x20000U;
                    party.motionState = party.requestedAction == 1 ? 3 : 1;
                }
            } else if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody))
                movement.flags |= 0x20000U;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    if (movement.flags & 0x20000U) {
        movement.flags &= ~0x20000U;
        Vector3 step = PartyLocalPosition(party.position, movement.targetWorldPosition, movement.targetWorldYaw);
        const float depth = static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f;
        step.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        step.z = -depth;
        shenApplyTranslationToAll(&party.character->entity.node, PartyWorldPosition(step, movement.targetWorldPosition, movement.targetWorldYaw));
    }
    ++party.stateFrames;
    if (party.motionState != previousState)
        party.stateFrames = 0;
    SetMotionSpeed(work, movement.field160 * movement.field15c, MotionRegion::LowerBody);
    movement.previousField15c = movement.field15c;
}

// Sets 0x100 while the companion projects inside the view; returns 1 when it is off camera.
// @verified - p
std::int32_t UpdatePartyCameraVisibility(PartyCompanion &party) {
    party.flags &= ~0x100U;
    Vector3 projected{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
    Matrix4x4::TransformPoint(party.position, projected);
    Matrix4x4::Pop(1);
    if (projected.z >= 0.0f || std::fabs(-(camCtrlGetHalfVerticalFovTangent() * projected.z)) + 0.44999999f <= std::fabs(projected.x))
        return 1;
    party.flags |= 0x100U;
    return 0;
}

// Binds a late motion work, steps the embedded movement state, and refreshes the companion's pose, route and guide gates.
// @verified - p
void UpdatePartyCharacterState(PartyCompanion &party) {
    MotionWork *const work = FindMotionWork(party.character);
    if (work && !party.motionWork) {
        party.motionWork = work;
        party.movement->motionWork = work;
        ConfigurePartyMotion(party);
    }
    if (party.motionWork && SetMotionRegionFlag(party.motionWork, 71, 1, MotionRegion::LowerBody))
        SetMotionRegionFlag(party.motionWork, 71, 2, MotionRegion::LowerBody);
    UpdatePlayerMovementState(*party.movement);
    party.position = GetCharacterWorldPosition(party.character);
    party.yaw = GetAllAngYForNode(&party.character->entity.node);
    party.speedScale = 1.0f;
    if (IsNpcLoadedOrVisible(party.name))
        SetNpcRouteYaw(party.name, party.yaw);
    party.flags &= 0xfff5ffffU;
    if (party.flags & 0x10000U)
        party.flags = (party.flags & 0xfffcffffU) | 0x20000U;
    if (!IsNpcLoadedOrVisible(party.name))
        EV_CheckWarmStartNpcRoute(party.name);
    party.flags &= ~0x80U;
    if (IsPlayerPartyGuideBlocked())
        party.flags |= 0x80U;
    if (IsNaviActive())
        party.flags |= 0x80U;
    constexpr shen_name guideName{SHEN_NAME(M01_)};
    if (IsNpcRouteReady(guideName)) {
        EV_CheckWarmStartNpcRoute(guideName);
        PauseNpcRouteMotion(nullptr, guideName);
    }
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) && (GetQronCurrentFlags() & 1U)) {
        Vector3 player{};
        plymUpdatePlayerSearchLocation(&player);
        const std::int32_t floor = GetQronFloorAtPosition(party.position);
        if (GetQronFloorAtPosition(player) != floor)
            party.flags |= 0x80000U;
    }
}

// Requested resets, camera visibility, and the count of frames spent close behind the player.
// @verified - p
void UpdatePartyPlayerProximity(PartyCompanion &party) {
    if (party.flags & 0x600U) {
        (void)ResetPartyPlacementNearPlayer(party);
        party.flags &= 0xfffff9ffU;
    }
    (void)UpdatePartyCameraVisibility(party);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const auto yaw = static_cast<std::int16_t>(playerGetRotY());
    const Vector3 local = PartyLocalPosition({party.position.x, party.position.y + 0.40000001f, party.position.z}, player, yaw);
    if (local.z >= 0.0f || std::fabs(local.z) >= 1.5f || std::fabs(local.x) > 0.25f)
        party.playerOverlapFrames = 0;
    else
        ++party.playerOverlapFrames;
}

// @verified - p
void UpdatePartyControl(PartyCompanion &party) {
    PreparePartyPlacement(party);
    if (party.flags & 0x2000U)
        return;
    FindPartyObstacles(party);
    SelectPartyAvoidancePlacement(party);
    party.requestedAction = 0;
    party.requestedYaw = party.selectedPlacement->characterBearingYaw;
    if (party.frameCount >= 2U) {
        if (party.mode == 1)
            UpdatePartyFollowBehavior(party);
        else if (party.mode == 2)
            UpdatePartyGuideBehavior(party);
        else if (party.mode == 4)
            party.flags &= ~0x10U;
    }
    UpdatePartyMotionController(party);
}

// PTYC update: an inactive follower still records the player's trail.
// @verified - p
void UpdatePartyCompanion(Character *character, void *payload) {
    PartyCompanion &party = *static_cast<PartyCompanion *>(payload);
    const bool inactive = (party.flags & 2U) == 0;
    party.character = character;
    if (inactive && party.mode == 1)
        RecordPartyPlayerTrail(party, GetPlayerTargetWorldPosition());
    if (!(party.flags & 2U))
        return;
    ++party.frameCount;
    UpdatePartyCharacterState(party);
    UpdatePartyPlayerProximity(party);
    UpdatePartyControl(party);
}

} // namespace model_component_detail

PartyCompanion *FindPartyCompanion(const Character *character) {
    return static_cast<PartyCompanion *>(FindModelComponent(character, kPartyToken));
}

shen_name FindNavigationPartyCompanion(shen_name name) {
    PartyCompanion *party = nullptr;
    if (name.name) {
        party = FindPartyCompanion(FindCharacterByScenarioName(name));
    } else {
        for (const shen_name candidate : kPartyNames) {
            party = FindPartyCompanion(FindCharacterByScenarioName(candidate));
            if (party)
                break;
        }
    }
    return party && (party->flags & 0x10000002U) == 0x10000002U ? party->name : shen_name{};
}

bool IsPartyCompanionPlaced(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    return party && (party->flags & 0x202U) == 2U;
}

bool IsPartyCompanionOnStairs(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    return party && (party->movement->flags & 8U) != 0;
}

std::int32_t SetPartyConversationPaused(shen_name name, bool paused) {
    PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    if (!party)
        return -1;
    if (paused)
        party->flags |= 0x40U;
    else
        party->flags &= ~0x40U;
    return 0;
}

// @verified - p
std::int64_t ReleaseAbsentPartyCharacter(shen_name name) {
    if (FindCharacterByScenarioName(name))
        return 0xffffffffLL;
    EV_npcFree(name);
    return 0;
}

// @verified - p
bool AttachPartyCompanion(Character *character) {
    if (!character)
        return false;
    PartyCompanion *const party = static_cast<PartyCompanion *>(AllocateModelComponent(character, kPartyToken, sizeof(PartyCompanion), UpdatePartyCompanion));
    if (!party)
        return false;
    party->mode = 2;
    party->movement = &party->embeddedMovement;
    InitializePlayerMovementState(party->embeddedMovement);
    party->selectedTrailIndex = -1;
    party->followDistance = 3.5f;
    party->routeIndex = -1;
    party->flags |= 0xa12U;
    party->name = GetCharacterScenarioName(character);
    party->character = character;
    party->movement->character = character;
    party->movement->flags |= 0x24U;
    EV_mtwkSetEnabled(FindMotionWork(character), true);
    cpamSetFlag(party->character, 1);
    toggleCCMflagsByAmt(character, 0x300000U);
    if (party->name.name == kPartyNames[1].name) {
        SetCharacterType(character, CharacterTypeStencilShadow);
        party->flags |= 0x800U;
    }
    party->routeMarker = g_currentMap.name == SHEN_NAME(KMZ1) ? 9 : 0;
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_MAZE)) {
        party->flags |= 0x40000000U;
    }
    if (MotionWork *const motion = FindMotionWork(character)) {
        party->motionWork = motion;
        party->movement->motionWork = motion;
        ConfigurePartyMotion(*party);
    }
    if (partyTransition.present && partyTransition.name.name == party->name.name) {
        party->flags &= ~0x200U;
        shenApplyTranslationToAll(&character->entity.node, partyTransition.position);
        shenDecrementRotYAllNodes(&character->entity.node, partyTransition.yaw);
        party->routeIndex = partyTransition.routeIndex;
        party->routeMarker = partyTransition.routeMarker;
        party->mode = partyTransition.mode;
        partyTransition.present = 0;
        party->character->alpha = 0.0f;
        charaSetFlagFor(party->character, 1);
        toggleCCMflagsByAmt(character, CharacterControlResetMotion);
        if (party->mode == 1)
            party->flags |= 0x200U;
        if (party->motionWork)
            SetMotionRegionFlag(party->motionWork, 71, 0, MotionRegion::LowerBody);
    }
    return true;
}

// An existing PTYC restarts from its idle states with full control and a fresh motion setup.
// @verified - p
void ReactivatePartyCompanion(PartyCompanion &party) {
    party.flags |= 0x402U;
    party.behaviorState = 0;
    party.motionState = 0;
    party.motionPhase = 0;
    party.runBlend = 0.0f;
    party.frameCount = 0;
    const std::int16_t yaw = GetAllAngYForNode(&party.character->entity.node);
    CopyMotionTransition(FindMotionWork(party.character), nullptr, yaw, 13, 0);
    toggleCCMflagsByAmt(party.character, 0x300000U);
    cpamSetFlag(party.character, 1);
    if (MotionWork *const motion = FindMotionWork(party.character)) {
        party.motionWork = motion;
        party.movement->motionWork = motion;
        ConfigurePartyMotion(party);
    }
}

// @verified - p
std::int32_t ActivatePartyCompanion(shen_name name) {
    EV_CheckWarmStartNpcRoute(name);
    if (!EV_ForceLoadNpc(name))
        return -1;
    EV_CheckWarmStartNpcRoute(name);
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return -1;
    if (PartyCompanion *const party = FindPartyCompanion(character)) {
        party->character = character;
        ReactivatePartyCompanion(*party);
    } else if (!AttachPartyCompanion(character))
        return -1;
    MarkNpcRoutePartyControl(name);
    ReleaseCompletedPartyGuideRoute();
    return 0;
}

// @verified - p
std::int32_t DeactivatePartyCompanion(shen_name name) {
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return -1;
    PartyCompanion *const party = FindPartyCompanion(character);
    if (!party)
        return -1;
    party->flags &= ~2U;
    EV_CheckWarmStartNpcRoute(name);
    EV_npcFree(name);
    if (party->motionState == 5 && party->doorId != 0 && party->doorPhase > 0) {
        party->doorPhase = 0;
        ReleaseDoor(static_cast<std::int32_t>(party->doorId), GetCharacterScenarioName(party->character));
    }
    return 0;
}

std::int32_t UnlockPartyCompanion(shen_name name) {
    PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    if (!party)
        return -1;
    if (!(party->flags & 0x40000000U) && name.name != kPartyNames[1].name) {
        if (!(party->flags & 0x2000U))
            EV_npcLockUnlock(name);
        party->flags &= ~2U;
        if (!(party->flags & 0x2000U) && party->motionState == 5 && party->doorId && party->doorPhase > 0) {
            party->doorPhase = 0;
            ReleaseDoor(static_cast<std::int32_t>(party->doorId), GetCharacterScenarioName(party->character));
        }
    }
    return 0;
}

// @verified - p
std::int32_t plymGetPartyAuthorStatus(shen_name name) {
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return 0;
    const PartyCompanion *const party = FindPartyCompanion(character);
    if (!party)
        return 0;
    if (party->mode != 2)
        return party->behaviorState == 0 || party->motionState == 0;
    if (!(party->placement.flags & 0x20U) && !(party->flags & 0x40000U))
        return 0;
    return party->behaviorState ? 2 - (party->motionState != 0) : 2;
}

std::int32_t FindPTYCForChara(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    return party && (party->flags & 2U) != 0;
}

std::int32_t ptycCharaUnlock(shen_name name) {
    PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    if (!party)
        return -1;
    if ((party->flags & 0x40000000U) != 0 || name.name == SHEN_NAME(SIN_))
        return 0;
    if ((party->flags & 0x2000U) == 0)
        EV_npcLockUnlock(name);
    party->flags &= ~2U;
    if ((party->flags & 0x2000U) == 0 && party->motionState == 5 && party->doorId != 0 && party->doorPhase > 0) {
        party->doorPhase = 0;
        ReleaseDoor(static_cast<std::int32_t>(party->doorId), GetCharacterScenarioName(party->character));
    }
    return 0;
}

// @verified - p
std::int32_t PTYCUtils(shen_name name, std::int32_t command, std::int32_t argument) {
    PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
    if (!party)
        return 0;
    switch (command) {
    case 0:
        party->mode = argument == 1 || argument == 2 || argument == 4 ? static_cast<std::uint8_t>(argument) : 0;
        if (argument == 4)
            party->behaviorState = 10;
        return party->mode;
    case 1:
        if (argument < 0)
            return party->routeMarker;
        party->routeMarker = argument;
        return 0;
    case 2: {
        const std::uint32_t previous = (party->flags >> 1) & 1U;
        if (argument && !previous) {
            party->behaviorState = 0;
            party->motionState = 0;
            party->motionPhase = 0;
            party->flags |= 0x1002U;
        } else if (!argument && previous) {
            party->flags &= ~0x1002U;
            if (mtwkSetExtraFlag(party->motionWork, 58, 1)) {
                CopyMotionTransition(party->motionWork, nullptr, party->yaw, 13, 0);
            }
        }
        return static_cast<std::int32_t>(previous);
    }
    case 3: {
        const std::uint32_t previous = (party->flags >> 13) & 1U;
        if (argument && !previous) {
            party->flags |= 0x2000U;
            if (mtwkSetExtraFlag(party->motionWork, 58, 1)) {
                CopyMotionTransition(party->motionWork, nullptr, party->yaw, 13, 0);
            }
            if (party->motionState == 5 && party->doorId && party->doorPhase > 0) {
                party->doorPhase = 0;
                ReleaseDoor(static_cast<std::int32_t>(party->doorId), GetCharacterScenarioName(party->character));
            }
        } else if (!argument && previous) {
            party->behaviorState = 0;
            party->motionState = 0;
            party->motionPhase = 0;
            party->flags &= ~0x2000U;
        }
        return static_cast<std::int32_t>(previous);
    }
    case 4:
        return plymGetPartyAuthorStatus(name);
    case 6:
        return 1;
    case 7:
        return static_cast<std::int32_t>((party->flags >> 1) & 1U);
    case 8:
        return DeactivatePartyCompanion(name);
    default:
        return 0;
    }
}

void ResetPartyCompanionTransition() {
    partyTransition = {};
}

std::uint32_t GetPartyCompanionTransitionPending() {
    return partyTransition.present;
}

std::int32_t ImportPartyCompanionTransition(const PartyCompanionTransition &state) {
    if (!state.present)
        return -1;
    partyTransition = state;
    return 0;
}

// @verified - p
std::int32_t ExportPartyCompanionTransition(PartyCompanionTransition &state) {
    std::int32_t present = 0;
    for (const shen_name name : kPartyNames) {
        const PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(name));
        if (!party)
            continue;
        partyTransition.name = name;
        present = 1;
        partyTransition.present = 1;
        partyTransition.position = GetCharacterWorldPosition(party->character);
        partyTransition.yaw = GetAllAngYForNode(&party->character->entity.node);
        partyTransition.routeIndex = party->routeIndex;
        partyTransition.routeMarker = party->routeMarker;
        partyTransition.mode = party->mode;
        break;
    }
    if (!present)
        partyTransition = {};
    state = partyTransition;
    partyTransition = {};
    return present;
}

} // namespace shenmue::s2
