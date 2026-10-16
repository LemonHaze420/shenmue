#include "s1/task/char.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"
#include "s1/face.hpp"
#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/effe.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"

#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/motion.hpp"
#include "s1/motion_registry.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/mbst.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/cloth_presets.hpp"
#include "s1/collision.hpp"
#include "s1/scene.hpp"
#include "s1/shadow_volume.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/input.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/wind.hpp"

#include "s1/timing.hpp"
#include <bit>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>
#include <limits>

#include "s1/task/char_components_internal.hpp"

namespace shenmue::s1 {

namespace {

static_assert(sizeof(LineWalkPathNode) == 0x10);
static_assert(sizeof(LineWalkCircleConstraint) == 0x24);
static_assert(sizeof(LineWalkWork) == 0x88);
static_assert(offsetof(LineWalkWork, segmentIndex) == 0x14);
static_assert(offsetof(LineWalkWork, flags) == 0x1c);
static_assert(offsetof(LineWalkWork, yawBase) == 0x20);
static_assert(offsetof(LineWalkWork, motion) == 0x30);
static_assert(offsetof(LineWalkWork, paths) == 0x40);
static_assert(offsetof(LineWalkWork, command) == 0x54);
static_assert(offsetof(LineWalkWork, position) == 0x58);
static_assert(offsetof(LineWalkWork, constraint) == 0x60);

constexpr std::uint32_t kLineWalkComponent = SHEN_NAME(LNWK);

LineWalkWork *FindLineWalkWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent);
    return component ? reinterpret_cast<LineWalkWork *>(component + 1) : nullptr;
}

std::int32_t LineWalkHeading(float fromX, float fromZ, float toX, float toZ) {
    return atan2_fixed16(-(toX - fromX), -(toZ - fromZ));
}

void LineWalkPromoteScenarioCharacter(Character *character) {
    const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
}

void LineWalkMarkDone(LineWalkWork &work) {
    work.state = 5;
    work.command = -1;
}

void lnwkUpdateNextState(Character *character, LineWalkWork &work) {
    const std::int16_t type = work.paths[work.segmentIndex].type;
    if (type == 0) {
        work.state = 1;
    } else if (type == 5) {
        work.state = 3;
    } else {
        if (type != 3)
            LineWalkPromoteScenarioCharacter(character);
        LineWalkMarkDone(work);
    }
}

void lnwkAdvanceCurrentMode(Character *character, LineWalkWork &work, std::int32_t angle) {
    MotionManagerWork *const motion = FindMotionManagerWork(character);
    const bool hasTurnAnimation = motion && work.turnAnimationTable;
    std::uint32_t motionReady = 1;
    if (motion && character && (character->motionFlags & 0x10U) != 0)
        motionReady = motion->motion.motionFlags & 2U;

    if (work.motionMode == 0) {
        work.motionMode = 2;
        return;
    }
    if (work.motionMode == 2) {
        if (motionReady)
            work.motionMode = 1;
        return;
    }
    if (work.motionMode != 1 && work.motionMode != 3)
        LineWalkPromoteScenarioCharacter(character);
    const std::int32_t yaw = character ? character->entity.node.rotation.y : 0;
    work.turnAnimationIndex = static_cast<std::uint8_t>(((-0x1000 - (yaw - work.yawBase - angle)) >> 13) & 7);
    if (work.motionMode != 1 && work.motionMode != 3)
        LineWalkPromoteScenarioCharacter(character);
    work.motionMode = work.turnAnimationIndex == 7 || !hasTurnAnimation ? 3 : 0;
}

void lnwkUpdateWalkAlignment(Character *character) {
    Vector3 delta{};
    if (MotionManagerWork *const motion = FindMotionManagerWork(character))
        delta = motion->motion.rootMotionDelta;
    else
        LineWalkPromoteScenarioCharacter(character);
    if (character) {
        character->rootMotionDelta.x += delta.x;
        character->rootMotionDelta.y += delta.y;
        character->rootMotionDelta.z += delta.z;
    }
}

void lnwkUpdateYaw(Character *character, LineWalkWork &work, std::int32_t angle) {
    if (MotionManagerWork *const motion = FindMotionManagerWork(character)) {
        motion->motion.motionFlags &= ~2U;
        motion->motion.pendingAnimId = static_cast<std::uint16_t>(work.turnAnimationTable[work.turnAnimationIndex]);
    } else {
        LineWalkPromoteScenarioCharacter(character);
    }
    work.yawOffset = angle;
}

void lnwkUpdateSmoothedPath(Character *character, LineWalkWork &work, std::int32_t yawOffset, float scale) {
    Vector3 rotation{};
    std::int32_t yaw = yawOffset;
    const std::int32_t yawDelta = (character ? character->entity.node.rotation.y : 0) - work.yawBase;
    const std::int32_t turn = static_cast<std::int16_t>(yawOffset - yawDelta);
    if ((work.flags & 0x02U) != 0 || std::abs(turn) <= work.yawLimit) {
        if (static_cast<std::uint8_t>(work.state - 3) <= 1U && work.motion.z < 5.0F)
            scale = scale * (work.motion.y / 5.0F);
        const float step = g_bEnableFrameTimeSmoothing ? scale * 30.0F * fFrameTimeMs : scale;
        rotation.x = -sin_fixed16(static_cast<std::int16_t>(yaw)) * step;
        const std::int16_t folded = static_cast<std::uint16_t>(yaw) <= 0x8000U ? static_cast<std::int16_t>(yaw) : static_cast<std::int16_t>(-yaw);
        rotation.z = -sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * step;
    } else {
        yaw = (turn < 0 ? -work.yawLimit : work.yawLimit) + yawDelta;
    }
    if (character)
        character->rootMotionDelta = rotation;
    if ((work.flags & 0x02U) == 0 && character)
        character->entity.node.rotation.y = yaw + work.yawBase;
}

void lnwkUpdate(Character *character, LineWalkWork &work, std::int32_t angle, float scale) {
    lnwkAdvanceCurrentMode(character, work, angle);
    if (work.motionMode == 0) {
        lnwkUpdateYaw(character, work, angle);
    } else if (work.motionMode == 1) {
        if (MotionManagerWork *const motion = FindMotionManagerWork(character)) {
            motion->motion.motionFlags &= ~2U;
            motion->motion.pendingAnimId = work.animationState;
        }
        if ((work.flags & 0x02U) == 0 && character)
            character->entity.node.rotation.y = work.yawBase + work.yawOffset;
    } else if (work.motionMode == 2) {
        lnwkUpdateWalkAlignment(character);
    } else {
        if (work.motionMode != 3)
            LineWalkPromoteScenarioCharacter(character);
        lnwkUpdateSmoothedPath(character, work, angle, scale);
    }
}

float LineWalkDistance(float x0, float z0, float x1, float z1) {
    return std::sqrt((z0 - z1) * (z0 - z1) + (x0 - x1) * (x0 - x1));
}

bool lnwkCanAdvanceCircularSegment(Character *, LineWalkWork &work) {
    if (work.constraint.radius <= 0.0F)
        return true;
    const LineWalkPathNode &next = work.paths[(work.segmentIndex + 1) % work.segmentCount];
    const float x = next.x - work.constraint.end.x;
    const float z = next.z - work.constraint.end.y;
    return std::sqrt(z * z + x * x) + work.motion.x > LineWalkDistance(work.position.x, work.position.y, next.x, next.z);
}

bool lnwkShouldEnterCircularConstraint(Character *character, LineWalkWork &work) {
    const std::int16_t nextType = work.paths[(work.segmentIndex + 1) % work.segmentCount].type;
    if (nextType != 0 && ((static_cast<std::uint16_t>(nextType) - 3U) & 0xfffffffdU) != 0)
        LineWalkPromoteScenarioCharacter(character);
    float radius = work.motion.x;
    const LineWalkPathNode &current = work.paths[work.segmentIndex % work.segmentCount];
    if (radius <= current.radius)
        radius = current.radius;
    return radius > LineWalkDistance(work.position.x, work.position.y, work.constraint.end.x, work.constraint.end.y);
}

void lnwkSetupNextSegmentFromPaths(Character *character, LineWalkWork &work) {
    const std::int16_t type = work.paths[work.segmentIndex % work.segmentCount].type;
    if (type == 5) {
        work.state = 3;
    } else {
        if (type != 0)
            LineWalkPromoteScenarioCharacter(character);
        work.state = 1;
    }
    const std::uint32_t segmentIndex = work.segmentIndex;
    const LineWalkPathNode &start = work.paths[segmentIndex % work.segmentCount];
    work.command = static_cast<std::uint16_t>(start.command);
    work.segmentIndex = segmentIndex + 1;
    work.constraint.start = {start.x, start.z};
    const LineWalkPathNode &end = work.paths[(segmentIndex + 1) % work.segmentCount];
    work.constraint.end = {end.x, end.z};
    work.yawTarget = LineWalkHeading(work.constraint.start.x, work.constraint.start.y, work.constraint.end.x, work.constraint.end.y);
}

void CircleConstraint_InitFromCorner(LineWalkCircleConstraint &constraint, const Vector4 &corner, float arcRadius) {
    const std::uint16_t incoming = static_cast<std::uint16_t>(LineWalkHeading(constraint.start.x, constraint.start.y, corner.x, corner.y));
    const std::uint16_t outgoing = static_cast<std::uint16_t>(LineWalkHeading(corner.x, corner.y, corner.z, corner.w));
    const float outgoingSin = sin_fixed16(static_cast<std::int16_t>(outgoing));
    const std::uint16_t outgoingFold = outgoing <= 0x8000U ? outgoing : static_cast<std::uint16_t>(-outgoing);
    constraint.end.x = -outgoingSin * arcRadius + corner.x;
    const float outgoingCos = sin_fixed16(static_cast<std::int16_t>(0x4000 - outgoingFold));
    constraint.end.y = -outgoingCos * arcRadius + corner.y;
    const float incomingSin = sin_fixed16(static_cast<std::int16_t>(incoming));
    const std::uint16_t incomingFold = incoming <= 0x8000U ? incoming : static_cast<std::uint16_t>(-incoming);
    const float incomingX = incomingSin * arcRadius;
    const float incomingZ = sin_fixed16(static_cast<std::int16_t>(0x4000 - incomingFold)) * arcRadius;
    const float outgoingX = -(outgoingSin * arcRadius);
    const float outgoingZ = -(outgoingCos * arcRadius);
    const float determinant = outgoingZ * incomingX - outgoingX * incomingZ;
    float centerX = incomingX;
    float centerZ = incomingZ;
    std::int32_t angleEnd = 0;
    if (determinant < -0.00019999999F || determinant > 0.00019999999F) {
        const float factor = arcRadius * arcRadius / determinant;
        centerX = (outgoingZ - incomingZ) * factor;
        centerZ = (incomingX - outgoingX) * factor;
        constraint.radius = std::sqrt((centerX - incomingX) * (centerX - incomingX) + (centerZ - incomingZ) * (centerZ - incomingZ));
        constraint.angleStart = atan2_fixed16(-(incomingX - centerX), -(incomingZ - centerZ));
        angleEnd = atan2_fixed16(-(outgoingX - centerX), -(outgoingZ - centerZ));
    } else {
        constraint.radius = 0.0F;
        constraint.angleStart = 0;
    }
    constraint.angleEnd = angleEnd;
    constraint.center = {centerX + corner.x, centerZ + corner.y};
    if (determinant > 0.00019999999F) {
        while (angleEnd < constraint.angleStart)
            angleEnd += 0x10000;
        constraint.angleEnd = angleEnd;
    } else if (determinant < -0.00019999999F) {
        while (angleEnd > constraint.angleStart)
            angleEnd -= 0x10000;
        constraint.angleEnd = angleEnd;
    } else {
        constraint.angleEnd = constraint.angleStart;
    }
}

void lnwkInitCircleConstraintForCorner(Character *, LineWalkWork &work) {
    const LineWalkPathNode &current = work.paths[work.segmentIndex % work.segmentCount];
    const LineWalkPathNode &next = work.paths[(work.segmentIndex + 1) % work.segmentCount];
    const Vector4 corner{current.x, current.z, next.x, next.z};
    const float x = work.position.x - corner.x;
    const float z = work.position.y - corner.y;
    CircleConstraint_InitFromCorner(work.constraint, corner, std::sqrt(z * z + x * x));
}

void lnwkEnterCircularConstraintState(Character *character, LineWalkWork &work) {
    work.state = 2;
    lnwkInitCircleConstraintForCorner(character, work);
}

void lnwkEnterCircularArcState(Character *character, LineWalkWork &work) {
    work.state = 4;
    lnwkInitCircleConstraintForCorner(character, work);
}

void lnwkAdvanceState(Character *character, LineWalkWork &work) {
    work.command = 0;
    void (*transition)(Character *, LineWalkWork &) = nullptr;
    bool ready = false;
    switch (work.state) {
    case 1:
        transition = lnwkEnterCircularConstraintState;
        ready = lnwkShouldEnterCircularConstraint(character, work);
        break;
    case 2:
        transition = lnwkSetupNextSegmentFromPaths;
        ready = lnwkCanAdvanceCircularSegment(character, work);
        break;
    case 3:
        transition = lnwkEnterCircularArcState;
        ready = lnwkShouldEnterCircularConstraint(character, work);
        break;
    case 4:
        transition = lnwkSetupNextSegmentFromPaths;
        ready = lnwkCanAdvanceCircularSegment(character, work);
        break;
    case 5:
        work.command = -1;
        return;
    default:
        LineWalkPromoteScenarioCharacter(character);
        work.state = 5;
        return;
    }
    if (!ready)
        return;
    if ((work.flags & 0x08U) != 0 || work.segmentIndex + 1 < static_cast<std::uint32_t>(work.segmentCount))
        transition(character, work);
    else
        LineWalkMarkDone(work);
}

void lnwkLinearHeading(Character *character, LineWalkWork &work) {
    const std::int32_t angle = LineWalkHeading(work.position.x, work.position.y, work.constraint.end.x, work.constraint.end.y);
    work.yawTarget = angle;
    lnwkUpdate(character, work, angle, work.motion.x);
}

float ccowComputeDistanceRadiusFromShape(Character *first, Character *second) {
    const CharacterCollisionWork *const firstControl = FindCharacterControlWork(first);
    const CharacterCollisionWork *const secondControl = FindCharacterControlWork(second);
    const float firstX = firstControl ? firstControl->shapeExtents.x : 0.0F;
    const float firstZ = firstControl ? firstControl->shapeExtents.z : 0.0F;
    const float secondX = secondControl ? secondControl->shapeExtents.x : 0.0F;
    const float secondZ = secondControl ? secondControl->shapeExtents.z : 0.0F;
    return std::sqrt((secondZ + firstZ) * (secondZ + firstZ) + (secondX + firstX) * (secondX + firstX));
}

bool lnwkIsValidObstacle(Character *character, const LineWalkWork *work, Character *obstacle) {
    if (!obstacle || !work || !character) {
        LineWalkPromoteScenarioCharacter(character);
        return false;
    }
    return obstacle != character && FindCharacterControlWork(obstacle) && character->entity.node.child == obstacle->entity.node.child &&
        character->eventGroup == obstacle->eventGroup;
}

bool LineWalkObstacleIsForward(const LineWalkWork &work, const Vector3 &owner, const Vector3 &obstacle) {
    const std::uint16_t heading = static_cast<std::uint16_t>(atan2_fixed16(-(obstacle.x - owner.x), -(obstacle.z - owner.z)));
    const std::uint32_t forward = static_cast<std::uint16_t>(heading - static_cast<std::uint16_t>(work.yawTarget));
    const std::int16_t delta =
        forward <= 0x8000U ? static_cast<std::int16_t>(forward) : static_cast<std::int16_t>(static_cast<std::uint16_t>(work.yawTarget) - heading);
    return sin_fixed16(static_cast<std::int16_t>(0x4000 - delta)) > 0.0F;
}

void lnwkEvaluateCircularObstacle(LineWalkWork &work, const Vector3 &owner, const Vector3 &obstacle, float radius) {
    if (!LineWalkObstacleIsForward(work, owner, obstacle))
        return;
    const float obstacleX = (obstacle.x - owner.x) / 40.0F;
    const float obstacleZ = (obstacle.z - owner.z) / 40.0F;
    const float segmentX = (work.constraint.end.x - owner.x) / 40.0F;
    const float segmentZ = (work.constraint.end.y - owner.z) / 40.0F;
    const float segmentLengthSquared = segmentZ * segmentZ + segmentX * segmentX;
    const float dot = obstacleZ * segmentZ + obstacleX * segmentX;
    const float obstacleLengthSquared = obstacleZ * obstacleZ + obstacleX * obstacleX;
    const float twiceDot = dot * -2.0F;
    const float t = dot / segmentLengthSquared;
    const float c = obstacleLengthSquared - (radius / 40.0F) * (radius / 40.0F);
    const bool separated =
        t <= 0.0F || t >= 1.0F ? c > 0.0F && twiceDot + segmentLengthSquared + c > 0.0F : (t * segmentLengthSquared + twiceDot) * t + c > 0.0F;
    if (separated)
        return;
    const float distance = std::sqrt(obstacleLengthSquared);
    if (work.collisionRadius <= 0.0F || work.motion.y > distance) {
        work.obstaclePosition = {obstacle.x, obstacle.z};
        work.collisionRadius = radius;
        work.motion.z = work.motion.y;
        work.motion.y = distance * 40.0F;
    }
}

bool lnwkProcessObstacleCandidate(Character *candidate, LineWalkQuery &query) {
    Character *const character = query.character;
    if (lnwkIsValidObstacle(character, query.work, candidate)) {
        const float radius = ccowComputeDistanceRadiusFromShape(candidate, character);
        const Vector3 owner = character ? character->entity.node.position : Vector3{};
        const Vector3 obstacle = candidate ? candidate->entity.node.position : Vector3{};
        lnwkEvaluateCircularObstacle(*query.work, owner, obstacle, radius);
    }
    return true;
}

void lnwkUpdateLinearDetourHeading(Character *character, LineWalkWork &work) {
    const float radius = work.collisionRadius / 40.0F;
    const float directionX = (work.obstaclePosition.x - work.position.x) / 40.0F;
    const float directionZ = (work.obstaclePosition.y - work.position.y) / 40.0F;
    const float segmentX = (work.constraint.end.x - work.constraint.start.x) / 40.0F;
    const float segmentZ = (work.constraint.end.y - work.constraint.start.y) / 40.0F;
    const float obstacleX = (work.obstaclePosition.x - work.constraint.start.x) / 40.0F;
    const float obstacleZ = (work.obstaclePosition.y - work.constraint.start.y) / 40.0F;
    const std::int32_t direct = atan2_fixed16(-directionX, -directionZ);
    const float distanceSquared = directionZ * directionZ + directionX * directionX;
    const float tangent = distanceSquared <= radius * radius ? 0.0F : std::sqrt(distanceSquared - radius * radius);
    const std::int32_t offset = atan2_fixed16(radius, tangent);
    const std::int32_t heading = obstacleX * segmentZ - obstacleZ * segmentX <= 0.0F ? offset + direct : direct - offset;
    work.yawTarget = heading;
    lnwkUpdate(character, work, heading, work.motion.x);
}

template <typename Visitor> void LineWalkVisitCollisionCharacters(LineWalkQuery &query, Visitor visitor) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    constexpr std::uint32_t kCollisionEntity = SHEN_NAME(CEWP);
    for (void *head : control->characterBuckets) {
        for (auto *link = static_cast<CharacterBucketNode *>(head); link; link = link->next) {
            Character *const candidate = link->character;
            if (!candidate || candidate->entity.name != kCollisionEntity)
                continue;
            if ((candidate->flags.value & 1U) == 0 && ((candidate->flags.value & 2U) == 0 || (candidate->motionFlags & 1U) == 0))
                continue;
            if (!visitor(candidate, query))
                return;
        }
    }
}

void lnwkUpdateLinearObstacleAvoidance(Character *character, LineWalkWork &work) {
    work.collisionRadius = 0.0F;
    work.motion.y = 3.4028235e38F;
    work.motion.z = 3.4028235e38F;
    LineWalkQuery query{.character = character, .work = &work};
    work.yawTarget = LineWalkHeading(work.position.x, work.position.y, work.constraint.end.x, work.constraint.end.y);
    LineWalkVisitCollisionCharacters(query, lnwkProcessObstacleCandidate);
    if (work.collisionRadius > 0.0F) {
        lnwkUpdateLinearDetourHeading(character, work);
    } else {
        const std::int32_t angle = LineWalkHeading(work.position.x, work.position.y, work.constraint.end.x, work.constraint.end.y);
        work.yawTarget = angle;
        lnwkUpdate(character, work, angle, work.motion.x);
    }
}

std::int32_t LineWalkUnwrapAngle(std::int32_t angleStart, std::int32_t angleEnd, std::int16_t angle) {
    const std::int32_t merged = angleEnd ^ static_cast<std::uint16_t>(angle ^ angleEnd);
    if (angleStart >= angleEnd) {
        const std::int32_t lowered = merged - 0x10000;
        if (lowered > angleEnd)
            return lowered;
        return merged + static_cast<std::int32_t>(static_cast<std::uint32_t>(angleEnd - lowered) & 0xffff0000U);
    }
    std::int32_t raised = merged + 0x10000;
    if (raised >= angleEnd)
        raised += static_cast<std::int32_t>(0xffff0000U - (static_cast<std::uint32_t>(raised - angleEnd) & 0xffff0000U));
    return raised;
}

bool AngleIsBetween(std::int32_t start, std::int32_t end, std::uint16_t angle) {
    if (start >= end) {
        std::int32_t value = angle + 0x10000;
        if (start < value)
            value = angle - static_cast<std::int32_t>(static_cast<std::uint32_t>(angle + 0x10000 - start - 1) & 0xffff0000U);
        return start > value && value > end;
    }
    std::int32_t value = angle - 0x10000;
    if (start > value)
        value += static_cast<std::int32_t>(static_cast<std::uint32_t>(start - value - 1) & 0xffff0000U) + 0x10000;
    return start < value && value < end;
}

void lnwkUpdateClosestObstacleInCone(Character *, LineWalkWork &work, const Vector3 &owner, const Vector3 &obstacle, float radius) {
    if (!LineWalkObstacleIsForward(work, owner, obstacle))
        return;
    const LineWalkCircleConstraint &constraint = work.constraint;
    const float ownerX = owner.x - constraint.center.x;
    const float ownerZ = owner.z - constraint.center.y;
    const float obstacleZ = obstacle.z - constraint.center.y;
    const float obstacleX = obstacle.x - constraint.center.x;
    const float obstacleRadius = std::sqrt(obstacleZ * obstacleZ + obstacleX * obstacleX);
    const std::uint16_t obstacleAngle = static_cast<std::uint16_t>(atan2_fixed16(-obstacleX, -obstacleZ));
    const std::int16_t ownerAngle = static_cast<std::int16_t>(atan2_fixed16(-ownerX, -ownerZ));
    const std::int32_t end = constraint.angleEnd;
    const std::int32_t start = LineWalkUnwrapAngle(constraint.angleStart, end, ownerAngle);
    if (obstacleRadius > constraint.radius - radius && constraint.radius + radius > obstacleRadius && AngleIsBetween(start, end, obstacleAngle) &&
        AngleIsBetween(constraint.angleStart, end, obstacleAngle)) {
        const float x = obstacle.x - owner.x;
        const float z = obstacle.z - owner.z;
        const float distance = std::sqrt(z * z + x * x);
        if (work.collisionRadius <= 0.0F || work.motion.y > distance) {
            work.obstaclePosition = {obstacle.x, obstacle.z};
            work.collisionRadius = radius;
            work.motion.z = work.motion.y;
            work.motion.y = distance;
        }
    }
}

bool lnwkCheckObstacleInConeRadius(Character *candidate, LineWalkQuery &query) {
    Character *const character = query.character;
    if (lnwkIsValidObstacle(character, query.work, candidate)) {
        const float radius = ccowComputeDistanceRadiusFromShape(candidate, character);
        const Vector3 owner = character ? character->entity.node.position : Vector3{};
        const Vector3 obstacle = candidate ? candidate->entity.node.position : Vector3{};
        lnwkUpdateClosestObstacleInCone(candidate, *query.work, owner, obstacle, radius);
    }
    return true;
}

void lnwkUpdateCircularDetourHeading(Character *character, LineWalkWork &work) {
    const float centerZ = work.obstaclePosition.y - work.constraint.center.y;
    const float centerX = work.obstaclePosition.x - work.constraint.center.x;
    const float radialDistance = std::sqrt(centerZ * centerZ + centerX * centerX);
    const float radius = work.collisionRadius / 40.0F;
    const float directionX = (work.obstaclePosition.x - work.position.x) / 40.0F;
    const float directionZ = (work.obstaclePosition.y - work.position.y) / 40.0F;
    const std::int32_t heading = atan2_fixed16(-directionX, -directionZ);
    const float distanceSquared = directionZ * directionZ + directionX * directionX;
    const float tangent = distanceSquared <= radius * radius ? 0.0F : std::sqrt(distanceSquared - radius * radius);
    const std::int32_t offset = atan2_fixed16(radius, tangent);
    std::int32_t detour;
    bool replace;
    if ((work.constraint.angleStart < work.constraint.angleEnd) == (work.constraint.radius > radialDistance)) {
        detour = offset + heading;
        replace = detour > work.yawTarget;
    } else {
        detour = heading - offset;
        replace = detour < work.yawTarget;
    }
    if (sin_fixed16(static_cast<std::int16_t>(replace)) != 0.0F)
        work.yawTarget = detour;
    lnwkUpdate(character, work, work.yawTarget, work.motion.x);
}

std::int32_t CircleConstraint_ComputeHeading(const LineWalkCircleConstraint &constraint, const Vector2 &from, const Vector2 &to) {
    const float radius = constraint.radius;
    if (radius <= 0.0F)
        return atan2_fixed16(-(to.x - from.x), -(to.y - from.y));
    const float z = from.y - constraint.center.y;
    const float x = from.x - constraint.center.x;
    const float distance = std::sqrt(z * z + x * x);
    const std::int16_t rawAngle = static_cast<std::int16_t>(atan2_fixed16(-x, -z));
    const std::int32_t end = constraint.angleEnd;
    const std::int32_t angle = LineWalkUnwrapAngle(constraint.angleStart, end, rawAngle);
    return angle - atan2_fixed16(radius - distance, static_cast<float>(end - angle) * distance * 0.000095873802F) + 0x4000;
}

void lnwkUpdateCircularObstacleAvoidance(Character *character, LineWalkWork &work) {
    LineWalkQuery query{.character = character, .work = &work};
    work.collisionRadius = 0.0F;
    work.motion.y = 3.4028235e38F;
    work.motion.z = 3.4028235e38F;
    const LineWalkPathNode &next = work.paths[(work.segmentIndex + 1) % work.segmentCount];
    work.yawTarget = CircleConstraint_ComputeHeading(work.constraint, work.position, Vector2{next.x, next.z});
    LineWalkVisitCollisionCharacters(query, lnwkCheckObstacleInConeRadius);
    if (work.collisionRadius > 0.0F)
        lnwkUpdateCircularDetourHeading(character, work);
    else
        lnwkUpdate(character, work, work.yawTarget, work.motion.x);
}

void lnwkUpdateMotion(Character *character, LineWalkWork &work) {
    switch (work.state) {
    case 1:
        lnwkLinearHeading(character, work);
        break;
    case 2: {
        const LineWalkPathNode &next = work.paths[(work.segmentIndex + 1) % work.segmentCount];
        lnwkUpdate(character, work, CircleConstraint_ComputeHeading(work.constraint, work.position, Vector2{next.x, next.z}), work.motion.x);
        break;
    }
    case 3:
        lnwkUpdateLinearObstacleAvoidance(character, work);
        break;
    case 4:
        lnwkUpdateCircularObstacleAvoidance(character, work);
        break;
    default:
        if (work.state != 5) {
            LineWalkPromoteScenarioCharacter(character);
            work.state = 5;
        }
        break;
    }
}

void LNWK_Callback(Character *character, void *payload) {
    auto *const work = static_cast<LineWalkWork *>(payload);
    if (work->state == 0) {
        LineWalkPromoteScenarioCharacter(character);
        return;
    }
    if ((work->flags & 0x04U) == 0) {
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent))
            component->flags &= ~1U;
    }
    work->position = character ? Vector2{character->entity.node.position.x, character->entity.node.position.z} : Vector2{};
    lnwkAdvanceState(character, *work);
    lnwkUpdateMotion(character, *work);
    if ((work->flags & 0x80U) != 0) {
        const Vector3 position = character ? character->entity.node.position : Vector3{};
        const float height = mapUpdateIntersectionStateAtLocalPoint(position);
        if (character)
            character->entity.node.position = {position.x, height, position.z};
    }
}

static_assert(sizeof(Walk3DArc) == 0x48);
static_assert(sizeof(Walk3DWork) == 0x80);
static_assert(offsetof(Walk3DWork, path) == 0x20);
static_assert(offsetof(Walk3DWork, arc) == 0x34);
static_assert(sizeof(Walk3DSegment) == 0x14);

constexpr std::uint32_t kWalk3DComponent = SHEN_NAME(W3DW);

void PromoteWalk3DCharacter(Character *character) {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control)
        SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
}

Walk3DWork *FindWalk3DWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kWalk3DComponent);
    return component ? reinterpret_cast<Walk3DWork *>(component + 1) : nullptr;
}

void ScheduleWalk3D(Character *character, bool enabled) {
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kWalk3DComponent)) {
        component->flags = enabled ? component->flags | 1U : component->flags & ~1U;
    }
}

float Walk3DLength(const Vector3 &value) {
    return std::sqrt(value.y * value.y + value.x * value.x + value.z * value.z);
}

Vector3 ScaleWalk3DVector(Vector3 value, float scale) {
    const float length = Walk3DLength(value);
    if (length != 0.0f) {
        const float factor = scale / length;
        value.x *= factor;
        value.y *= factor;
        value.z *= factor;
    }
    return value;
}

void SolveWalk3DArc(Walk3DArc &arc, const Vector3 &corner, const Vector3 &next, float radius) {
    const Vector3 incoming =
        ScaleWalk3DVector({arc.previousPosition.x - corner.x, arc.previousPosition.y - corner.y, arc.previousPosition.z - corner.z}, radius);
    const Vector3 outgoing = ScaleWalk3DVector({next.x - corner.x, next.y - corner.y, next.z - corner.z}, radius);
    arc.exitPosition = {corner.x + outgoing.x, corner.y + outgoing.y, corner.z + outgoing.z};
    const float squaredRadius = radius * radius;
    const float denominator = outgoing.y * incoming.y + outgoing.x * incoming.x + outgoing.z * incoming.z + squaredRadius;
    Vector3 centerOffset{};
    if (denominator <= 0.0f) {
        arc.tangent = {};
        arc.radiusAxis = {};
    } else {
        const float factor = squaredRadius / denominator;
        centerOffset = {factor * outgoing.x + factor * incoming.x, factor * outgoing.y + factor * incoming.y, factor * outgoing.z + factor * incoming.z};
        arc.tangent = ScaleWalk3DVector({incoming.x - centerOffset.x, incoming.y - centerOffset.y, incoming.z - centerOffset.z}, 1.0f);
        arc.radiusAxis = ScaleWalk3DVector(incoming, -1.0f);
    }
    arc.center = {corner.x + centerOffset.x, corner.y + centerOffset.y, corner.z + centerOffset.z};
    arc.binormal = ScaleWalk3DVector(
        {arc.tangent.y * arc.radiusAxis.z - arc.tangent.z * arc.radiusAxis.y, arc.tangent.z * arc.radiusAxis.x - arc.tangent.x * arc.radiusAxis.z,
            arc.tangent.x * arc.radiusAxis.y - arc.tangent.y * arc.radiusAxis.x},
        1.0f);
}

void SetWalk3DHeading(Walk3DWork &work, const Vector3 &delta) {
    const float horizontal = std::sqrt(delta.z * delta.z + delta.x * delta.x);
    work.pitch = FixedAtan2ForEuler(delta.y, horizontal);
    work.yaw = FixedAtan2ForEuler(-delta.x, -delta.z);
}

bool HasWalk3DArc(const Walk3DArc &arc) {
    return arc.tangent.x != 0.0f || arc.tangent.y != 0.0f || arc.tangent.z != 0.0f;
}

void ComputeWalk3DArcHeading(Walk3DWork &work, const Vector3 &target) {
    const Walk3DArc &arc = work.arc;
    Vector3 delta{};
    if (!HasWalk3DArc(arc)) {
        delta = {target.x - work.currentPosition.x, target.y - work.currentPosition.y, target.z - work.currentPosition.z};
    } else {
        const Vector3 centerDelta = {work.currentPosition.x - arc.center.x, work.currentPosition.y - arc.center.y, work.currentPosition.z - arc.center.z};
        const float z = -(arc.radiusAxis.x * centerDelta.x + arc.radiusAxis.y * centerDelta.y + arc.radiusAxis.z * centerDelta.z);
        const float x = arc.tangent.y * centerDelta.y + arc.tangent.x * centerDelta.x + arc.tangent.z * centerDelta.z;
        const float y = -(arc.binormal.x * centerDelta.x + arc.binormal.y * centerDelta.y + arc.binormal.z * centerDelta.z);
        delta = {arc.radiusAxis.x * x + arc.tangent.x * z + arc.binormal.x * y, arc.tangent.y * z + arc.radiusAxis.y * x + arc.binormal.y * y,
            arc.tangent.z * z + arc.radiusAxis.z * x + arc.binormal.z * y};
    }
    SetWalk3DHeading(work, delta);
}

void ResetWalk3DArc(Walk3DWork &work, Character *character, std::int16_t index) {
    work.flags |= 0x180U;
    work.index = index;
    const Vector3 &current = work.path[index].position;
    if (character)
        character->entity.node.position = current;
    const Vector3 &next = work.path[index + 1].position;
    SetWalk3DHeading(work, {next.x - current.x, next.y - current.y, next.z - current.z});
}

void AdvanceWalk3DSegment(Walk3DWork &work, Character *character, Walk3DSegment &segment) {
    segment.currentIndex = static_cast<std::int16_t>(work.index + 1);
    if (segment.currentIndex >= work.count)
        segment.currentIndex = 0;
    if ((work.flags & 0x80U) != 0) {
        work.flags = (work.flags & ~0xe80U) | 0x200U;
        work.arc.previousPosition = work.path[work.index].position;
        work.arc.exitPosition = work.path[segment.currentIndex].position;
    }
    if ((work.flags & 0x100U) != 0) {
        if (character)
            character->entity.node.position = work.path[work.index].position;
        work.flags &= ~0x100U;
    }
    segment.delta = {
        work.arc.exitPosition.x - work.currentPosition.x, work.arc.exitPosition.y - work.currentPosition.y, work.arc.exitPosition.z - work.currentPosition.z};
    segment.distance = Walk3DLength(segment.delta);
    segment.nextIndex = static_cast<std::int16_t>(segment.currentIndex + 1);
    if (segment.nextIndex >= work.count)
        segment.nextIndex = 0;
}

void HandleWalk3DPhase(Walk3DWork &work, Character *character, Walk3DSegment &segment) {
    const std::uint32_t phase = (work.flags >> 9) & 7U;
    const Walk3DPathKey &current = work.path[segment.currentIndex];
    const Vector3 &next = work.path[segment.nextIndex].position;
    if (phase == 1) {
        SetWalk3DHeading(work, segment.delta);
        if (std::max(current.radius, work.step + work.step) <= segment.distance)
            return;
        work.mode = std::bit_cast<std::int32_t>(current.event);
        if (segment.currentIndex + 1 >= work.count) {
            if (work.pathEndMode == 0xff573300U) {
                work.flags |= 0x40U;
                return;
            }
            if (work.pathEndMode == 0xff573302U) {
                ResetWalk3DArc(work, character, 0);
                return;
            }
            if (work.pathEndMode != 0xff573301U)
                return;
        }
        work.flags = (work.flags & ~0xa00U) | 0x400U;
        SolveWalk3DArc(work.arc, current.position, next, segment.distance);
    } else if (phase == 2) {
        ComputeWalk3DArcHeading(work, next);
        bool advance = !HasWalk3DArc(work.arc) || current.radius == 0.0f;
        if (!advance) {
            const float exitDistance = Walk3DLength({next.x - work.arc.exitPosition.x, next.y - work.arc.exitPosition.y, next.z - work.arc.exitPosition.z});
            const float currentDistance = Walk3DLength({next.x - work.currentPosition.x, next.y - work.currentPosition.y, next.z - work.currentPosition.z});
            advance = exitDistance + work.step > currentDistance;
        }
        if (advance) {
            work.index = segment.currentIndex;
            if (++segment.currentIndex >= work.count)
                segment.currentIndex = 0;
            work.flags |= 0x80U;
        }
    } else {
        PromoteWalk3DCharacter(character);
    }
}

void W3DW_Callback(Character *character, void *payload) {
    auto &work = *static_cast<Walk3DWork *>(payload);
    if ((work.flags & 1U) == 0) {
        PromoteWalk3DCharacter(character);
        return;
    }
    if ((work.flags & 0x40U) != 0) {
        work.mode = -1;
        if (character)
            character->rootMotionDelta = {};
    } else {
        work.mode = 0;
        work.currentPosition = character ? character->entity.node.position : Vector3{};
        Walk3DSegment segment{};
        AdvanceWalk3DSegment(work, character, segment);
        HandleWalk3DPhase(work, character, segment);
        const float step = (work.flags & 0x100U) != 0 ? 0.0f : work.step;
        const auto yaw = static_cast<std::uint16_t>(work.yaw);
        const auto pitch = static_cast<std::uint16_t>(work.pitch);
        const float pitchCos = sin_fixed16(static_cast<std::int16_t>(0x4000 - static_cast<std::int16_t>(pitch <= 0x8000U ? pitch : -pitch)));
        const float yawCos = sin_fixed16(static_cast<std::int16_t>(0x4000 - static_cast<std::int16_t>(yaw <= 0x8000U ? yaw : -yaw)));
        if (character) {
            character->rootMotionDelta = {-(pitchCos * step * sin_fixed16(static_cast<std::int16_t>(yaw))),
                sin_fixed16(static_cast<std::int16_t>(pitch)) * step, -(yawCos * (pitchCos * step))};
            if ((work.flags & 0x10U) == 0)
                character->entity.node.rotation.x = work.pitch;
            if ((work.flags & 8U) == 0)
                character->entity.node.rotation.y = work.yaw;
        }
    }
    ScheduleWalk3D(character, (work.flags & 4U) != 0 && (work.flags & 0x60U) == 0);
}
} // namespace

bool LNWK_Allocate(Character *character) {
    if (!character)
        return false;
    if (FindLineWalkWork(character))
        return true;
    auto *const work = static_cast<LineWalkWork *>(AllocateHeapMemoryMODL(character, kLineWalkComponent, sizeof(LineWalkWork), LNWK_Callback));
    if (!work)
        return false;
    work->state = 0;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent)) {
        component->flags &= ~1U;
    }
    return true;
}

void LNWK_BeginPath(Character *character, std::byte *taskData, std::int32_t pathOffset, std::int32_t yawTableOffsetOrLimit, std::uint32_t flags) {
    if (!character)
        return;
    SelectCharacterMapQuery(character);
    LineWalkWork *const work = FindLineWalkWork(character);
    if (!work)
        return;
    work->command = 0;
    if ((flags & 0x10U) != 0) {
        if ((work->flags & 0x04U) == 0)
            LineWalkPromoteScenarioCharacter(character);
        if (MotionManagerWork *const motion = FindMotionManagerWork(character)) {
            work->savedAnimationId = static_cast<std::int16_t>(motion->motion.currentAnimId);
            work->savedAnimationRate = MOTI_GetPlayRateScaledRemain(motion->motion);
        }
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent))
            component->flags &= ~1U;
        return;
    }
    if ((flags & 0x20U) != 0) {
        if ((work->flags & 0x04U) == 0)
            LineWalkPromoteScenarioCharacter(character);
        if (MotionManagerWork *const motion = FindMotionManagerWork(character)) {
            motion->motion.motionFlags &= ~2U;
            motion->motion.pendingAnimId = static_cast<std::uint16_t>(work->savedAnimationId);
            motion->motion.scriptStartFrame = work->savedAnimationRate;
        }
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent))
            component->flags |= 1U;
        return;
    }

    if (MotionManagerWork *const motion = FindMotionManagerWork(character)) {
        work->animationState = motion->motion.pendingAnimId ? motion->motion.pendingAnimId : motion->motion.currentAnimId;
    } else {
        work->animationState = 0;
    }
    work->flags = static_cast<std::uint8_t>(flags);
    work->flagByte1 = static_cast<std::uint8_t>(flags >> 8);
    work->flagByte2 = static_cast<std::uint8_t>(flags >> 16);
    work->flagByte3 = static_cast<std::uint8_t>(flags >> 24);
    work->paths = pathOffset ? reinterpret_cast<LineWalkPathNode *>(taskData + pathOffset) : nullptr;
    if (!work->paths) {
        LineWalkPromoteScenarioCharacter(character);
        work->state = 0;
        return;
    }
    work->yawBase = 0;
    if ((flags & 0x40U) != 0) {
        work->turnAnimationTable = nullptr;
        work->yawLimit = yawTableOffsetOrLimit;
    } else {
        work->turnAnimationTable = yawTableOffsetOrLimit ? reinterpret_cast<const std::int16_t *>(taskData + yawTableOffsetOrLimit) : nullptr;
        work->yawLimit = 0xffff;
    }
    if ((flags & 0x02U) != 0)
        work->turnAnimationTable = nullptr;
    work->motionMode = 3;
    std::int16_t segmentCount = 0;
    while (work->paths[segmentCount].type == 0 || work->paths[segmentCount].type == 5) {
        ++segmentCount;
    }
    work->segmentCount = segmentCount;
    if (segmentCount <= 0) {
        LineWalkMarkDone(*work);
        return;
    }

    if ((flags & 0x01U) != 0) {
        work->segmentIndex = 1;
        work->constraint.start = {work->paths[0].x, work->paths[0].z};
        character->entity.node.position = {work->constraint.start.x, 0.0f, work->constraint.start.y};
        character->entity.node.position.y = mapUpdateIntersectionStateAtLocalPoint({work->constraint.start.x, 0.0f, work->constraint.start.y});
    } else {
        work->segmentIndex = 0;
        work->constraint.start = {
            character->entity.node.position.x,
            character->entity.node.position.z,
        };
    }
    lnwkUpdateNextState(character, *work);
    const LineWalkPathNode &target = work->paths[work->segmentIndex % work->segmentCount];
    work->constraint.end = {target.x, target.z};
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent)) {
        component->flags &= ~1U;
    }
}

bool LNWK_SetMoveScaleAndGetPathCommand(Character *character, float scale, std::int32_t &command) {
    LineWalkWork *const work = FindLineWalkWork(character);
    if (!work || work->state == 0)
        return false;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent))
        component->flags |= 1U;
    work->motion.x = scale;
    command = work->command;
    return true;
}

std::int32_t LNWK_GetStatus(Character *character) {
    LineWalkWork *const work = FindLineWalkWork(character);
    if (!work)
        return 6;
    bool callbackEnabled = false;
    if ((work->flags & 0x04U) != 0) {
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLineWalkComponent)) {
            callbackEnabled = (component->flags & 1U) != 0;
        }
    }
    switch (work->state) {
    case 0:
        return 0;
    case 1:
    case 2:
        return callbackEnabled ? 1 : 2;
    case 3:
    case 4:
        return callbackEnabled ? 3 : 4;
    case 5:
        return 5;
    default:
        LineWalkPromoteScenarioCharacter(character);
        work->state = 5;
        return 6;
    }
}

void LNWK_PatchPathNode(std::byte *taskData, std::int32_t pathOffset, std::int32_t nodeIndex, std::uint32_t xBits, std::uint32_t zBits,
    std::uint32_t radiusBits, std::uint32_t typeBits) {
    if (!taskData || !pathOffset)
        return;
    constexpr std::uint32_t kKeepValue = 0xffff4c57U;
    auto *const node = reinterpret_cast<LineWalkPathNode *>(taskData + pathOffset) + nodeIndex;
    if (xBits != kKeepValue)
        std::memcpy(&node->x, &xBits, sizeof(xBits));
    if (zBits != kKeepValue)
        std::memcpy(&node->z, &zBits, sizeof(zBits));
    if (radiusBits != kKeepValue)
        std::memcpy(&node->radius, &radiusBits, sizeof(radiusBits));
    if (typeBits != kKeepValue)
        node->type = static_cast<std::int16_t>(typeBits);
}

void LNWK_SetYawBase(Character *character, std::int32_t yawBase) {
    if (LineWalkWork *const work = FindLineWalkWork(character); work && work->state != 0) {
        work->yawBase = yawBase;
    }
}
namespace {

static_assert(offsetof(PartyWork, latest) == 0x54);
static_assert(offsetof(PartyWork, trail) == 0x6c);
static_assert(offsetof(PartyWork, sourceSteps) == 0x524);
static_assert(offsetof(PartyWork, followerYaw) == 0x548);
static_assert(sizeof(PartyWork) == 0x550);
static_assert(sizeof(TimedPartyWork) == 24);
static_assert(sizeof(PatrolPathNode) == 0x0c);
static_assert(sizeof(PatrolMotionParameters) == 0x14);
static_assert(sizeof(PatrolWork) == 0x70);
static_assert(offsetof(PatrolWork, pathNodes) == 0x28);
static_assert(offsetof(PatrolWork, arcAnchor) == 0x40);
static_assert(offsetof(PatrolWork, recordedPoints) == 0x4c);

constexpr auto kPatrolComponent = SHEN_NAME(PATW);
constexpr std::uint8_t kPatrolSkipTurn = 1;
constexpr std::uint8_t kPatrolTargetBlocked = 2;
constexpr std::uint8_t kPatrolAllowNearReposition = 4;
constexpr std::uint8_t kPatrolRejoinOnObstruction = 8;
constexpr std::uint8_t kPatrolRejoinLock = 16;

MapControlData *PatrolControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}



bool PatrolQuery(PatrolPoint start, PatrolPoint end, IntersectionTestType mode, float width) {
    QueryShape query{};
    query.mode = mode;
    query.packed[0] = {start.x, start.z, end.x};
    query.packed[1] = {end.z, width, 0.0f};
    const MapControlData *const control = PatrolControl();
    return ExecuteMathFnWrapper(&query, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr) != 0;
}

PatrolPoint PatrolForward(std::int32_t facing, float speed) {
    const std::int16_t folded = static_cast<std::uint16_t>(facing) <= 0x8000U ? static_cast<std::int16_t>(facing) : static_cast<std::int16_t>(-facing);
    return {-sin_fixed16(facing) * speed, -sin_fixed16(0x4000 - folded) * speed};
}


void PatrolRequestMotion(Character *character, PatrolWork &work, std::uint32_t slot) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    std::uint16_t motion = 0;
    if (slot <= 2)
        motion = work.motionParameters->motionIds[slot];
    else if (slot <= 9 && work.turnMotions)
        motion = work.turnMotions[slot - 3];
    if (!motion)
        return;
    manager->motion.motionFlags &= ~2U;
    manager->motion.pendingAnimId = motion;
    manager->motion.playbackRate = slot > 2 && work.pendingState == PatrolState::Linger ? 2.0f : 1.0f;
}


void PatrolQueueMove(Character *character, PatrolWork &work, PatrolPoint target) {
    const Vector3 &position = character->entity.node.position;
    work.desiredFacing = LineWalkHeading(position.x, position.z, target.x, target.z);
    if (!work.turnMotions || (work.flags & kPatrolSkipTurn)) {
        work.state = work.pendingState;
        character->entity.node.rotation.y = work.desiredFacing;
        return;
    }
    work.state = PatrolState::ResolveTransition;
    const std::uint32_t turn = ((work.desiredFacing - character->entity.node.rotation.y + 4096) >> 13) & 7;
    if (turn) {
        PatrolRequestMotion(character, work, turn + 2);
    } else {
        work.state = work.pendingState;
        character->entity.node.rotation.y = work.desiredFacing;
        PatrolRequestMotion(character, work, 0);
    }
}


void PatrolForceLinger(Character *character, PatrolWork &work) {
    if (work.flags & kPatrolRejoinLock)
        return;
    if (work.state != PatrolState::Linger) {
        work.state = PatrolState::Linger;
        work.speed = work.motionParameters->alternateSpeed;
        PatrolRequestMotion(character, work, 1);
    }
    work.recordedPointCount = 0;
    work.timer = 0;
}


void PatrolProbeForward(Character *character, PatrolWork &work) {
    Vector3 &position = character->entity.node.position;
    const std::int32_t facing = character->entity.node.rotation.y;
    const PatrolPoint forward = PatrolForward(facing, 1.0f);
    const PatrolPoint probe{static_cast<float>(static_cast<double>(forward.x) * 1.5 + static_cast<double>(position.x)),
        static_cast<float>(static_cast<double>(forward.z) * 1.5 + static_cast<double>(position.z))};
    if (PatrolQuery({position.x, position.z}, probe, CalculateLineIntersection_0, 0.0f)) {
        character->entity.node.rotation.y = facing + 4187;
        return;
    }
    work.velocity = PatrolForward(facing, work.speed);
    position.x += work.velocity.x;
    position.z += work.velocity.z;
}


void PatrolCheckPathNode(Character *character, PatrolWork &work) {
    const Vector3 &position = character->entity.node.position;
    if (PatrolQuery({position.x, position.z}, work.pathNodes[work.pathNodeIndex].point, IsPointInTriangle_2, 0.2f)) {
        if (++work.pathNodeIndex >= work.pathNodeCount)
            work.pathNodeIndex = 0;
        work.pathNodeNextIndex = work.pathNodes[work.pathNodeIndex].nextIndex;
        PatrolProbeForward(character, work);
    } else {
        work.pendingState = PatrolState::MoveToPathNode;
        PatrolQueueMove(character, work, work.pathNodes[work.pathNodeIndex].point);
    }
}


void PatrolLinger(Character *character, PatrolWork &work) {
    if (work.timer && work.timer != -1) {
        work.timer = static_cast<std::int8_t>(work.timer - 1);
        PatrolRequestMotion(character, work, 2);
        return;
    }
    if (!work.timer) {
        work.timer = -1;
        PatrolRequestMotion(character, work, 1);
    }
    if (work.flags & kPatrolTargetBlocked) {
        work.recordedPointCount = 1;
        work.recordedPointCursor = 0;
        work.state = PatrolState::FollowPoints;
        work.recordedPoints[1] = work.recordedPoints[0];
    } else if (work.distanceToTarget >= 1.5f) {
        character->entity.node.rotation.y = work.desiredFacing;
        work.velocity = PatrolForward(work.desiredFacing, work.speed);
        character->entity.node.position.x += work.velocity.x;
        character->entity.node.position.z += work.velocity.z;
    } else {
        work.result = 1;
    }
}


void PatrolMoveToPathNode(Character *character, PatrolWork &work) {
    Vector3 &position = character->entity.node.position;
    const PatrolPoint target = work.pathNodes[work.pathNodeIndex].point;
    const PatrolPoint delta{target.x - position.x, target.z - position.z};
    if (work.speed <= std::sqrt(delta.z * delta.z + delta.x * delta.x)) {
        character->entity.node.rotation.y = LineWalkHeading(position.x, position.z, target.x, target.z);
        work.velocity = PatrolForward(character->entity.node.rotation.y, work.speed);
    } else {
        work.velocity = delta;
        work.flags &= ~kPatrolRejoinLock;
        work.pathNodeIndex = work.pathNodes[work.pathNodeIndex].nextIndex;
        work.state = PatrolState::Arc;
        work.arcFacing = character->entity.node.rotation.y;
        work.timer = 25;
        work.phase = 0;
        PatrolRequestMotion(character, work, 2);
    }
    position.x += work.velocity.x;
    position.z += work.velocity.z;
}


void PatrolResolveTransition(Character *character, PatrolWork &work) {
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager || !(manager->motion.motionFlags & 2U))
        return;
    if (work.pendingState == PatrolState::Linger) {
        PatrolForceLinger(character, work);
    } else {
        work.state = work.pendingState;
        character->entity.node.rotation.y = work.desiredFacing;
        work.speed = work.motionParameters->moveSpeed;
        PatrolRequestMotion(character, work, 0);
    }
}



void PatrolArc(Character *character, PatrolWork &work) {
    Matrix4x4 transform;
    SetupTransformationMatrix(character, 5, &transform);
    work.arcAnchor = {transform.rows[3].x, transform.rows[3].y, transform.rows[3].z};
    work.timer = static_cast<std::int8_t>(work.timer - 1);
    switch (work.phase) {
    case 0:
        work.arcFacing += 364;
        if (!work.timer) {
            work.timer = 50;
            ++work.phase;
        }
        break;
    case 1:
        work.arcFacing -= 364;
        if (!work.timer) {
            work.timer = 25;
            ++work.phase;
        }
        break;
    case 2:
        work.arcFacing += 364;
        if (!work.timer) {
            work.pendingState = PatrolState::MoveToPathNode;
            PatrolQueueMove(character, work, work.pathNodes[work.pathNodeIndex].point);
        }
        break;
    }
    const PatrolPoint direction = PatrolForward(work.arcFacing, 1.0f);
    work.arcAnchor.x = character->entity.node.position.x + direction.x;
    work.arcAnchor.z = character->entity.node.position.z + direction.z;
    if (MotionManagerWork *const manager = MOTM_GetWork(character))
        manager->motion.worldTargets[2] = &work.arcAnchor;
}

void PatrolRejoinPath(Character *character, PatrolWork &work) {
    work.timer = static_cast<std::int8_t>(0x80);
    work.speed = work.motionParameters->moveSpeed;
    PatrolRequestMotion(character, work, 0);
    work.state = PatrolState::CheckPathNode;
}




void PatrolFollowPoints(Character *character, PatrolWork &work) {
    const std::int32_t count = static_cast<std::int8_t>(work.recordedPointCount);
    if (count < 4) {
        const PatrolPoint previous = count ? work.recordedPoints[count - 1] : PatrolPoint{work.arcAnchor.y, work.arcAnchor.z};
        if (PatrolQuery(work.recordedPoints[count], previous, IsPointInTriangle_2, 0.3f))
            ++work.recordedPointCount;
    }
    Vector3 &position = character->entity.node.position;
    const PatrolPoint target = work.recordedPoints[work.recordedPointCursor];
    const PatrolPoint delta{target.x - position.x, target.z - position.z};
    character->entity.node.rotation.y = LineWalkHeading(position.x, position.z, target.x, target.z);
    if (work.distanceToTarget < 5.0f &&
        (work.flags & (kPatrolTargetBlocked | kPatrolRejoinOnObstruction)) == (kPatrolTargetBlocked | kPatrolRejoinOnObstruction)) {
        PatrolRejoinPath(character, work);
        work.flags |= kPatrolRejoinLock;
        return;
    }
    if (work.speed <= std::sqrt(delta.z * delta.z + delta.x * delta.x)) {
        work.velocity = PatrolForward(character->entity.node.rotation.y, work.speed);
        position.x += work.velocity.x;
        position.z += work.velocity.z;
        return;
    }
    if (work.recordedPointCount - work.recordedPointCursor > 2) {
        PatrolRejoinPath(character, work);
        return;
    }
    work.velocity = delta;
    position.x += delta.x;
    position.z += delta.z;
    if (!(work.flags & (kPatrolTargetBlocked | kPatrolRejoinLock)))
        PatrolForceLinger(character, work);
    if (++work.recordedPointCursor == work.recordedPointCount) {
        if (work.motionParameters->fallbackDistance > work.distanceToTarget && !(work.flags & kPatrolTargetBlocked))
            PatrolForceLinger(character, work);
        else
            PatrolRejoinPath(character, work);
    }
}



void PATW_Callback(Character *character, void *payload) {
    auto &work = *static_cast<PatrolWork *>(payload);
    const Vector3 position = character->entity.node.position;
    const MapControlData *const control = PatrolControl();
    Character *const target = control ? static_cast<Character *>(control->ryo) : nullptr;
    const Vector3 targetPosition = target ? target->entity.node.position : Vector3{};
    const PatrolPoint targetPoint{targetPosition.x, targetPosition.z};
    const float dx = targetPosition.x - position.x;
    const float dz = targetPosition.z - position.z;
    work.distanceToTarget = std::sqrt(dz * dz + dx * dx);
    const std::int32_t count = static_cast<std::int8_t>(work.recordedPointCount);
    if (count < 4)
        work.recordedPoints[count] = targetPoint;
    if (PatrolQuery(targetPoint, {position.x, position.z}, IsPointInTriangle_2, 0.3f)) {
        work.flags |= kPatrolTargetBlocked;
    } else {
        work.flags &= ~kPatrolTargetBlocked;
        work.desiredFacing = LineWalkHeading(position.x, position.z, targetPoint.x, targetPoint.z);
        const std::int32_t delta = work.desiredFacing - character->entity.node.rotation.y;
        const auto facingGate = static_cast<std::uint16_t>((delta < 0 ? -delta : delta) + 10012);
        if (work.motionParameters->fallbackDistance > work.distanceToTarget && work.state != PatrolState::Linger) {
            if (work.distanceToTarget < 2.0f) {
                if (work.state != PatrolState::ResolveTransition && !(work.flags & kPatrolRejoinLock)) {
                    work.pendingState = PatrolState::Linger;
                    PatrolQueueMove(character, work, targetPoint);
                }
            } else if (facingGate < 20024U) {
                PatrolForceLinger(character, work);
                work.timer = 20;
            } else if ((work.flags & kPatrolAllowNearReposition) && work.distanceToTarget < 5.0f) {
                const std::int32_t targetFacing = LineWalkHeading(targetPoint.x, targetPoint.z, position.x, position.z);
                const std::int32_t targetDelta = targetFacing - (target ? target->entity.node.rotation.y : 0);
                const auto targetGate = static_cast<std::uint16_t>((targetDelta < 0 ? -targetDelta : targetDelta) + DEG_TO_ANG(27.498779296875) /*5006*/); //
                if (targetGate < DEG_TO_ANG(54.99755859375) /*10012U*/ &&
                    (work.state != PatrolState::ResolveTransition || work.pendingState != PatrolState::Linger)) {
                    work.pendingState = PatrolState::Linger;
                    PatrolQueueMove(character, work, targetPoint);
                    work.timer = 20;
                }
            }
        }
    }
    using Handler = void (*)(Character *, PatrolWork &);
    constexpr Handler handlers[]{PatrolCheckPathNode, PatrolLinger, PatrolMoveToPathNode, PatrolResolveTransition, PatrolArc, PatrolFollowPoints};
    if (work.state < PatrolState::Count)
        handlers[static_cast<std::uint8_t>(work.state)](character, work);
}


void PatrolInitialize(Character *character, PatrolWork &work, const std::int32_t *arguments) {
    work.motionParameters = arguments[3] ? reinterpret_cast<const PatrolMotionParameters *>(g_pCurrentTaskData + arguments[3]) : nullptr;
    work.turnMotions = arguments[4] ? reinterpret_cast<const std::uint16_t *>(g_pCurrentTaskData + arguments[4]) : nullptr;
    work.recordedPointCount = 0;
    work.recordedPointCursor = 0;
    work.state = PatrolState::CheckPathNode;
    work.pathNodeCount = *reinterpret_cast<const std::uint8_t *>(g_pCurrentTaskData + arguments[2]);
    work.pathNodes = arguments[2] == -4 ? nullptr : reinterpret_cast<const PatrolPathNode *>(g_pCurrentTaskData + arguments[2] + 4);
    work.pathNodeIndex = static_cast<std::uint8_t>(arguments[6]);
    if (work.pathNodeIndex >= work.pathNodeCount)
        work.pathNodeIndex = 0;
    work.pathNodeNextIndex = work.pathNodes[work.pathNodeIndex].nextIndex;
    work.flags = 0;
    work.initFlags = static_cast<std::uint8_t>(arguments[5]);
    work.speed = work.motionParameters->moveSpeed;
    work.velocity = PatrolForward(character->entity.node.rotation.y, work.speed);
    if (!(arguments[5] & 1))
        PatrolRequestMotion(character, work, 0);
}
} // namespace

bool PATW_Allocate(Character *character) {
    auto *const work = static_cast<PatrolWork *>(AllocateHeapMemoryMODL(character, kPatrolComponent, sizeof(PatrolWork), PATW_Callback));
    if (!work) {
        if (MapControlData *const control = PatrolControl())
            SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
        return false;
    }
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kPatrolComponent))
        component->flags &= ~1U;
    work->state = PatrolState::Count;
    return true;
}

std::int32_t PATW_HandleCommand(Character *character, const std::int32_t *arguments) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kPatrolComponent);
    if (!component)
        return 255;
    auto &work = *reinterpret_cast<PatrolWork *>(component + 1);
    work.result = 0;
    switch (arguments[1]) {
    case 0:
        PatrolInitialize(character, work, arguments);
        break;
    case 1:
        PATW_Callback(character, &work);
        break;
    case 2:
        work.result = static_cast<std::uint8_t>(work.state);
        if (work.state == PatrolState::ResolveTransition && work.pendingState == PatrolState::Linger)
            work.result = 1;
        break;
    case 3:
        if (arguments[2])
            work.flags |= kPatrolAllowNearReposition;
        else
            work.flags &= ~kPatrolAllowNearReposition;
        break;
    case 4:
        if (arguments[2])
            work.flags |= kPatrolRejoinOnObstruction;
        else
            work.flags &= ~(kPatrolRejoinOnObstruction | kPatrolRejoinLock);
        break;
    case 5:
        return static_cast<std::int8_t>(work.recordedPointCount) - static_cast<std::int8_t>(work.recordedPointCursor);
    }
    return static_cast<std::int8_t>(work.result);
}
namespace {

constexpr std::uint32_t kPartyComponent = SHEN_NAME(PTMV);
constexpr std::uint16_t kPartyStandAnimation = 89;
constexpr std::uint16_t kPartyWalkAnimation = 735;
constexpr std::uint16_t kPartyRunAnimation = 724;

float PartyDistance(const Vector3 &a, const Vector3 &b) {
    const float x = a.x - b.x;
    const float z = a.z - b.z;
    return std::sqrt(x * x + z * z);
}

float PartyCosine(std::uint16_t yaw) {
    const std::uint16_t folded = yaw <= 0x8000U ? yaw : static_cast<std::uint16_t>(-yaw);
    return sin_fixed16(0x4000 - folded);
}

bool PartyPathBlocked(PartyWork &party, const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result) {
    COLS_TraceCharacterVector(start, end, radius, result, party.owner, party.source);
    const std::uint32_t hit = result.flags & 7U;
    return hit != 0 && (hit != 1 || result.fraction <= 1.0f);
}

void PartyUpdateCameraSpacing(PartyWork &party) {
    if (party.followSpeedScale != 1.0f)
        return;
    const float distance = (CameraDistanceFromPlayer - 2.2f) / 0.79999995f;
    party.runDistance = distance * 0.5999999f + 2.0f;
    party.walkDistance = distance * 0.60000014f + 1.8f;
}

void PartyResetTrail(PartyWork &party) {
    std::fill(std::begin(party.sourceSteps), std::end(party.sourceSteps), 0.0f);
    party.sourceStepSum = 0.0f;
    party.sourceStepCursor = 0;
    const Vector3 position = party.source ? party.source->entity.node.position : Vector3{};
    for (PartyTrailSegment &segment : party.trail)
        segment = {position, position};
    party.latest = {position, position};
    party.trailWriteIndex = 0;
    party.trailTargetIndex = 0;
}

Vector3 PartyOffsetTrailPoint(PartyWork &party, const Vector3 &position, std::uint16_t yaw) {
    const float sine = sin_fixed16(yaw);
    const float cosine = PartyCosine(yaw);
    const float forwardX = sine * party.forwardOffset;
    Vector3 offset{
        (cosine * party.lateralOffset + position.x) + forwardX, position.y, cosine * party.forwardOffset + (position.z - sine * party.lateralOffset)};
    CharacterVectorCollisionResult result{};
    PartyPathBlocked(party, position, offset, 0.30000001f, result);
    if ((result.flags & 7U) == 2U) {
        result.flags = (result.flags & ~7U) | 1U;
        result.fraction = 0.0f;
    }
    if ((result.flags & 7U) != 1U || result.fraction > 1.0f)
        return offset;
    if (!(std::fabs(party.lateralOffset) * result.fraction > 0.2f)) {
        const float lateral = -party.lateralOffset;
        offset.x = (cosine * lateral + position.x) + forwardX;
        offset.z = cosine * party.forwardOffset + (position.z - sine * lateral);
        PartyPathBlocked(party, position, offset, 0.30000001f, result);
        if ((result.flags & 7U) == 2U) {
            result.flags = (result.flags & ~7U) | 1U;
            result.fraction = 0.0f;
        }
        if ((result.flags & 7U) != 1U || result.fraction > 1.0f)
            return offset;
    }
    offset.x = (offset.x - position.x) * result.fraction + position.x;
    offset.z = (offset.z - position.z) * result.fraction + position.z;
    return offset;
}

void PartyUpdateSourceTrail(PartyWork &party) {
    party.latest.source = party.latest.offset;
    party.latest.offset = party.source ? party.source->entity.node.position : Vector3{};
    const float step = PartyDistance(party.latest.offset, party.latest.source);
    party.sourceStepSum -= party.sourceSteps[party.sourceStepCursor];
    party.sourceSteps[party.sourceStepCursor] = step;
    if (++party.sourceStepCursor >= 5)
        party.sourceStepCursor = 0;
    party.sourceStepSum += step;
    party.averageSourceStep = static_cast<float>(static_cast<double>(party.sourceStepSum) * 0.2);
    const std::int32_t previous = party.trailWriteIndex ? party.trailWriteIndex - 1 : 49;
    const std::uint16_t yaw = static_cast<std::uint16_t>(party.source ? charaGetWorldYaw(party.source) : 0);
    SelectCharacterMapQuery(party.source);
    const Vector3 offset = PartyOffsetTrailPoint(party, party.latest.offset, yaw);
    if (party.source) {
        const Vector3 saved = party.source->entity.node.position;
        party.source->entity.node.position = party.trail[previous].offset;
        SelectCharacterMapQuery(party.source);
        party.source->entity.node.position = saved;
    } else {
        SelectCharacterMapQuery(nullptr);
    }
    CharacterVectorCollisionResult result{};
    if (PartyPathBlocked(party, party.trail[previous].offset, offset, 0.28999999f, result)) {
        if (++party.trailWriteIndex >= 50)
            party.trailWriteIndex = 0;
        if (party.trailTargetIndex == party.trailWriteIndex && ++party.trailTargetIndex >= 50)
            party.trailTargetIndex = 0;
    }
    party.trail[party.trailWriteIndex] = {party.latest.offset, offset};
}

void PartySelectReachableTrail(Character *character, PartyWork &party, std::int32_t stopIndex) {
    const Vector3 position = character->entity.node.position;
    SelectCharacterMapQuery(character);
    std::int32_t index = party.trailWriteIndex;
    CharacterVectorCollisionResult result{};
    while (index != stopIndex) {
        if (!PartyPathBlocked(party, position, party.trail[index].offset, 0.28999999f, result))
            break;
        if (--index < 0)
            index = 49;
    }
    party.trailTargetIndex = index;
}

void PartyRequestIdle(PartyWork &party, MotionInformationWork &motion) {
    if (party.animationId != kPartyStandAnimation) {
        motion.motionFlags &= ~2U;
        motion.pendingAnimId = kPartyStandAnimation;
        party.animationId = kPartyStandAnimation;
    }
}

bool PartyUpdateFollowerMotion(Character &character, PartyWork &party, MotionManagerWork &manager) {
    MotionInformationWork &motion = manager.motion;
    Vector3 position = character.entity.node.position;
    const float distance = PartyDistance(party.latest.source, position);
    float scale = 1.0f;
    float sourceStep = party.averageSourceStep;
    bool stopped = false;
    if (party.followSpeedScale == 1.0f) {
        if (party.walkDistance - 0.5f > distance && party.averageSourceStep < 0.01f) {
            stopped = true;
        } else if (party.walkDistance > distance) {
            scale = 0.80000001f;
        } else if (party.runDistance <= distance) {
            scale = 1.2f;
            if (party.runDistance + 1.0f <= distance) {
                sourceStep = party.runStep;
                if (party.runDistance + 5.0f <= distance)
                    scale = 2.0f;
            }
        }
    } else if (party.walkDistance > distance && party.averageSourceStep < 0.01f) {
        party.runLatch = 0;
        stopped = true;
    } else {
        if (party.runDistance <= distance || party.runLatch) {
            sourceStep = party.runStep;
            party.runLatch = party.walkDistance <= distance;
        }
        scale = party.followSpeedScale;
    }
    if (stopped) {
        party.currentStep = 0.0f;
        motion.interpolationAmount = 0.0f;
        PartyRequestIdle(party, motion);
        return true;
    }
    float step = std::fmax(party.walkStep, sourceStep) * scale;
    while (party.trailWriteIndex != party.trailTargetIndex) {
        if (PartyDistance(party.trail[party.trailTargetIndex].offset, position) > 0.2)
            break;
        if (++party.trailTargetIndex >= 50)
            party.trailTargetIndex = 0;
    }
    const Vector3 &target = party.trail[party.trailTargetIndex].offset;
    if (step > party.currentStep)
        step = static_cast<float>(std::fmin(static_cast<double>(party.currentStep) + 0.02, static_cast<double>(step)));
    party.currentStep = step;
    const std::int32_t yawDelta = static_cast<std::uint16_t>(Shenmue_Atan2Fixed(target.x - position.x, target.z - position.z) - party.followerYaw) - 0x8000;
    party.followerYaw += std::clamp(yawDelta, -2730, 2730);
    if (static_cast<std::uint32_t>(yawDelta + 2730) > 5460U)
        party.currentStep = step = party.walkStep;
    character.entity.node.rotation.y = party.followerYaw;
    manager.flags &= ~1U;
    const std::uint16_t yaw = static_cast<std::uint16_t>(party.followerYaw + 0x8000);
    position.x += sin_fixed16(yaw) * step;
    position.z += PartyCosine(yaw) * step;
    character.entity.node.position = position;
    const float amount = std::fmax(0.0f, (1.0f / (party.runStep - party.walkStep)) * (step - party.walkStep));
    const double current = motion.interpolationAmount;
    motion.interpolationAmount =
        static_cast<float>(amount <= current ? std::fmax(current - 0.05, static_cast<double>(amount)) : std::fmin(current + 0.05, static_cast<double>(amount)));
    motion.motionFlags |= 0x100U;
    if (party.animationId != kPartyWalkAnimation) {
        motion.pendingAnimId = kPartyWalkAnimation;
        motion.motionFlags &= ~2U;
        party.animationId = kPartyWalkAnimation;
    }
    return false;
}

void PartyUpdateOpacity(Character &character, const PartyWork &party) {
    if (party.followSpeedScale != 1.0f)
        return;
    const Vector3 source = party.source ? party.source->entity.node.position : Vector3{};
    const Vector3 &camera = g_CameraEyePos;
    const float sourceX = source.x - camera.x;
    const float sourceZ = source.z - camera.z;
    const float followerX = character.entity.node.position.x - camera.x;
    const float followerZ = character.entity.node.position.z - camera.z;
    const std::uint16_t yaw = Shenmue_Atan2Fixed(sourceX, sourceZ);
    const float sine = sin_fixed16(yaw);
    const float cosine = PartyCosine(yaw);
    float opacity = 1.0f;
    if (cosine * sourceZ + sine * sourceX > cosine * followerZ + sine * followerX) {
        const float sideways = std::fabs(cosine * followerX - sine * followerZ);
        if (sideways < 0.30000001f)
            opacity = (sideways * sideways * 0.69999999f) / 0.090000004f + 0.30000001f;
    }
    character.renderOpacity = opacity;
}

void PTMV_Callback(Character *character, void *payload) {
    auto &party = *static_cast<PartyWork *>(payload);
    if (!party.needsPathInitialization)
        PartyUpdateSourceTrail(party);
    if (party.active == 1) {
        const Vector3 original = character->entity.node.position;
        PartyUpdateCameraSpacing(party);
        party.lateralOffset = -0.60000002f;
        MotionManagerWork &manager = *MOTM_GetWork(character);
        if (party.moveState == 1) {
            if (PartyUpdateFollowerMotion(*character, party, manager))
                party.moveState = 0;
            PartySelectReachableTrail(character, party, party.trailTargetIndex);
        } else if (party.moveState == 0) {
            PartyRequestIdle(party, manager.motion);
            const float spacing = party.followSpeedScale == 1.0f ? party.runDistance : party.walkDistance;
            if (PartyDistance(party.latest.source, character->entity.node.position) > spacing + 0.5f) {
                party.moveState = 1;
                PartySelectReachableTrail(character, party, party.trailWriteIndex + 1 < 50 ? party.trailWriteIndex + 1 : 0);
            }
        }
        Vector3 &position = character->entity.node.position;
        const float x = position.x - party.latest.offset.x;
        const float z = position.z - party.latest.offset.z;
        const float distance = std::sqrt(z * z + x * x);
        if (distance < 1.0f) {
            const double inverse = 1.0 / distance;
            position.x = static_cast<float>(x * inverse + party.latest.offset.x);
            position.z = static_cast<float>(z * inverse + party.latest.offset.z);
        }
        character->rootMotionDelta.x += position.x - original.x;
        character->rootMotionDelta.y += position.y - original.y;
        character->rootMotionDelta.z += position.z - original.z;
        position = original;
    }
    PartyUpdateOpacity(*character, party);
}
} // namespace

void PTMV_Allocate(Character *character) {
    auto *const party = static_cast<PartyWork *>(AllocateHeapMemoryMODL(character, kPartyComponent, sizeof(PartyWork), PTMV_Callback));
    if (!party)
        return;
    party->active = 0;
    party->needsPathInitialization = 1;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kPartyComponent))
        component->flags &= ~1U;
}

void PTMV_ControlTrailFollow(Character *character, Character *source, std::uint8_t mode, bool stop) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kPartyComponent);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!component || !manager)
        return;
    auto &party = *reinterpret_cast<PartyWork *>(component + 1);
    MotionInformationWork &motion = manager->motion;
    if (stop) {
        if (party.active != 1)
            return;
        motion.motionFlags |= party.savedMotionFlags;
        motion.blendMode = party.savedBlendMode;
        motion.blendFrameCount = party.savedBlendFrames;
        motion.playbackRate = party.savedPlaybackRate;
        motion.interpolationAmount = party.savedInterpolationAmount;
        if (party.savedMotionEnabled)
            manager->flags |= 1U;
        else
            manager->flags &= ~1U;
        motion.motionFlags &= ~2U;
        g_playerStats.runtimeFlags &= ~1U;
        motion.pendingAnimId = 236;
        party.active = 0;
        return;
    }
    party.savedMotionFlags = motion.motionFlags & 0x140U;
    party.savedBlendMode = motion.blendMode;
    party.savedBlendFrames = motion.blendFrameCount;
    party.savedPlaybackRate = motion.playbackRate;
    party.savedInterpolationAmount = motion.interpolationAmount;
    party.savedMotionEnabled = manager->flags & 1U;
    motion.motionFlags |= 0x40U;
    motion.blendMode = 1;
    motion.blendFrameCount = 5;
    motion.playbackRate = 1.0f;
    manager->flags &= ~1U;
    motion.interpolationAmount = 0.0f;
    party.source = source;
    party.owner = character;
    party.mode = mode;
    party.animationId = motion.currentAnimId;
    party.moveState = 0;
    party.currentStep = 0.0f;
    party.runLatch = 0;
    if (!party.needsPathInitialization) {
        PartySelectReachableTrail(character, party, party.trailWriteIndex + 1 < 50 ? party.trailWriteIndex + 1 : 0);
    } else {
        PartyResetTrail(party);
        party.followSpeedScale = 1.0f;
        PartyUpdateCameraSpacing(party);
        party.lateralOffset = -0.60000002f;
        party.forwardOffset = 0.0f;
        Vector3 sample{};
        g_motnHasYValue = 1;
        MOTN_ReadRootPositionAtFrame(kPartyWalkAnimation, 0, 0.0f, sample);
        party.walkStep = -sample.z;
        g_motnHasYValue = 1;
        MOTN_ReadRootPositionAtFrame(kPartyRunAnimation, 0, 0.0f, sample);
        party.runStep = -sample.z;
        party.needsPathInitialization = 0;
    }
    party.followerYaw = character->entity.node.rotation.y;
    g_playerStats.runtimeFlags |= 1U;
    party.active = 1;
    component->flags |= 1U;
}

void PTTM_Allocate(Character *character) {
    constexpr auto token = SHEN_NAME(PTTM);
    if (AllocateHeapMemoryMODL(character, token, sizeof(TimedPartyWork), nullptr)) {
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, token))
            component->flags &= ~1U;
    }
}

void PTTM_AssignSource(Character *character, Character *source, std::uint8_t mode) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(PTTM));
    if (!component)
        return;
    auto *const work = reinterpret_cast<TimedPartyWork *>(component + 1);
    work->source = source;
    work->owner = character;
    work->mode = mode;
    work->active = 1;
    if (MotionManagerWork *const motion = MOTM_GetWork(character))
        motion->flags &= ~1U;
    component->flags |= 1U;
}

void PTTM_Stop(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(PTTM));
    if (!component)
        return;
    if (MotionManagerWork *const motion = MOTM_GetWork(character)) {
        motion->motion.motionFlags &= ~2U;
        motion->motion.pendingAnimId = 236;
    }
    reinterpret_cast<TimedPartyWork *>(component + 1)->active = 0;
}

bool W3DW_Allocate(Character *character) {
    if (!character)
        return false;
    Walk3DWork *work = FindWalk3DWork(character);
    if (!work) {
        work = static_cast<Walk3DWork *>(AllocateHeapMemoryMODL(character, kWalk3DComponent, sizeof(Walk3DWork), W3DW_Callback));
    }
    if (!work) {
        PromoteWalk3DCharacter(character);
        return false;
    }
    ScheduleWalk3D(character, false);
    work->flags &= ~1U;
    return true;
}

void W3DW_SetupPath(Character *character, Walk3DPathKey *path, std::uint32_t scriptFlags) {
    Walk3DWork *const work = FindWalk3DWork(character);
    if (!work) {
        PromoteWalk3DCharacter(character);
        return;
    }
    work->mode = 0;
    std::uint32_t flags = work->flags & ~0x1eU;
    flags |= ((~scriptFlags << 1) & 2U) | ((scriptFlags << 1) & 4U) | (scriptFlags & 0x18U);
    work->flags = (flags & 0xfffff19eU) | 0x201U;
    work->path = path;
    if (!path) {
        PromoteWalk3DCharacter(character);
        return;
    }
    std::int16_t count = 0;
    for (;;) {
        const std::uint32_t marker = std::bit_cast<std::uint32_t>(path[count].position.x);
        if (marker - 0xff573300U <= 2U) {
            work->pathEndMode = marker;
            break;
        }
        ++count;
    }
    work->count = static_cast<std::uint16_t>(count);
    if ((work->flags & 2U) != 0) {
        work->index = -1;
        work->arc.previousPosition = character ? character->entity.node.position : Vector3{};
        work->arc.exitPosition = path[0].position;
        work->flags &= ~0x180U;
    } else {
        work->index = 0;
        work->flags |= 0x180U;
    }
    ScheduleWalk3D(character, false);
}

bool W3DW_Activate(Character *character, float step, std::int32_t &mode) {
    Walk3DWork *const work = FindWalk3DWork(character);
    if (!work || (work->flags & 1U) == 0) {
        PromoteWalk3DCharacter(character);
        return false;
    }
    ScheduleWalk3D(character, true);
    work->step = step;
    mode = work->mode;
    return true;
}

std::uint32_t W3DW_GetState(Character *character) {
    Walk3DWork *const work = FindWalk3DWork(character);
    if (!work)
        return 5;
    if ((work->flags & 1U) == 0)
        return 0;
    if ((work->flags & 0x40U) != 0)
        return 3;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kWalk3DComponent);
    if ((work->flags & 4U) == 0 || !component || (component->flags & 1U) == 0) {
        return 2;
    }
    return (work->flags & 0x20U) != 0 ? 4 : 1;
}

void W3DW_EditPathPoint(Walk3DPathKey *path, std::int32_t index, const std::uint32_t *values) {
    if (!path || !values)
        return;
    auto *const destination = reinterpret_cast<std::byte *>(path + index);
    for (std::size_t field = 0; field < 5; ++field) {
        if (values[field] != 0xffff5733U) {
            std::memcpy(destination + field * 4, values + field, 4);
        }
    }
}

void W3DW_SetDisabled(Character *character, bool disabled) {
    Walk3DWork *const work = FindWalk3DWork(character);
    if (!work || (work->flags & 4U) == 0) {
        PromoteWalk3DCharacter(character);
        return;
    }
    work->flags = disabled ? work->flags | 0x20U : work->flags & ~0x20U;
    ScheduleWalk3D(character, !disabled);
}

void W3DW_ResetSegment(Character *character, std::uint16_t index) {
    Walk3DWork *const work = FindWalk3DWork(character);
    if (!work) {
        PromoteWalk3DCharacter(character);
        return;
    }
    ResetWalk3DArc(*work, character, index);
}
} // namespace shenmue::s1
