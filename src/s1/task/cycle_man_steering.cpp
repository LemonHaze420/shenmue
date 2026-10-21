#include "s1/task/cycle_man.hpp"
#include "s1/task/kdan.hpp"
#include "s1/motion.hpp"
#include "s1/collision.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/skinning_cache.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <cctype>
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/wexe.hpp"
#include "s1/mt5_types.hpp"
#include "s1/mt5_render.hpp"
#include "s1/face.hpp"
#include "s1/render_array.hpp"
#include "s1/player_control.hpp"
#include "s1/timing.hpp"
#include "s1/boot.hpp"
#include "s1/subtitles.hpp"
#include "s1/file_loader.hpp"

#include <cfloat>
#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
using namespace CycleManager;

namespace CycleManager {

void ResetPathSteeringState(CycleManager::NpcRecord &npc) {
    npc.cycle->reversePath = 0;
    npc.cycle->pathTurnRequested = 0;
    npc.cycle->steeringBlockedFrames = 0;
    npc.cycle->reversePathFrames = 0;
    npc.cycle->steeringWaitFrames = 0;
    npc.cycle->steeringState = CycleSteerState_Normal;
    npc.cycle->steeringCommand = CycleSteerCmd_None;
}
void RecomputeSteeringExtents(CycleManager::NpcRecord &npc) {
    // The original leaves the character extents alone while a vehicle/move
    // object runtime owns its collision shape.
    if (npc.cycle->moveObjRuntime)
        return;

    if (npc.cycle->doorAnimActive) {
        npc.cycle->steeringHalfWidth = 0.5f;
        npc.cycle->steeringHalfDepth = 0.0f;
        return;
    }

    if (npc.cycle->moveConfig->cycleClassCode >= 4u && npc.cycle->moveConfig->cycleClassCode <= 7u) {
        npc.cycle->steeringHalfWidth = npc.cycle->moveConfig->steeringBodyHalfWidth;
        npc.cycle->steeringHalfDepth = npc.cycle->moveConfig->steeringBodyHalfDepth;
        return;
    }

    if (npc.cycle->steeringProfile == 1) {
        npc.cycle->steeringHalfWidth = npc.cycle->moveConfig->steeringBodyHalfWidth + 0.2f;
        npc.cycle->steeringHalfDepth = npc.cycle->moveConfig->steeringBodyHalfDepth;
        return;
    }

    npc.cycle->steeringHalfWidth = std::max(npc.cycle->moveConfig->steeringBodyHalfWidth, npc.cycle->moveConfig->steeringBodyHalfDepth);
    if (npc.cycle->steeringHalfWidth > 0.40000001f)
        npc.cycle->steeringHalfWidth = 0.36000001f;
    npc.cycle->steeringHalfDepth = 0.0f;
}
} // namespace CycleManager
  // namespace

namespace CycleManager {

void RecomputeSteeringExtentsForNpc(CycleManager::NpcRecord &npc) {
    CycleManager::RecomputeSteeringExtents(npc);
}
} // namespace CycleManager


bool IsTargetPointBlockedByRyoOrActor(const CycleManager::NpcRecord &npc, const Vector3 &target, float distanceSquared) {
    Vector3 playerPos{};
    if (CycleManager::IsReady() && CycleManager::MapRyo()) {
        playerPos = CycleManager::CurrentRyoPosition();
    }
    const float playerDx = target.x - playerPos.x;
    const float playerDy = target.y - playerPos.y;
    const float playerDz = target.z - playerPos.z;
    if (distanceSquared > playerDx * playerDx + playerDy * playerDy + playerDz * playerDz) {
        const float targetDx = target.x - npc.cycle->pos.x;
        const float targetDz = target.z - npc.cycle->pos.z;
        const float playerFromNpcDx = playerPos.x - npc.cycle->pos.x;
        const float playerFromNpcDz = playerPos.z - npc.cycle->pos.z;
        if (targetDx * playerFromNpcDx + targetDz * playerFromNpcDz > 0.0f)
            return true;
    }

    if (!CycleManager::IsReady())
        return false;
    for (const CycleManager::NpcRecord &other : g_cycleManagerRuntime.npcRecords) {
        if (&other == &npc || other.cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name || other.programList != CycleProgramList_SimSet ||
            other.cycle->currentOpcode != CycleOpcode_DoorUse) {
            continue;
        }
        const float dx = target.x - other.cycle->pos.x;
        const float dy = target.y - other.cycle->pos.y;
        const float dz = target.z - other.cycle->pos.z;
        if (distanceSquared > dx * dx + dy * dy + dz * dz)
            return true;
    }
    return false;
}
namespace CycleManager {


float DistanceSquared(const Vector3 &a, const Vector3 &b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

int16_t FoldUnsignedYaw(uint16_t yaw) {
    return yaw <= 0x8000u ? static_cast<int16_t>(yaw) : static_cast<int16_t>(-yaw);
}

size_t CollectAuxiliaryCollisionShapes(AuxiliaryCollisionEntry *outShapes, size_t capacity, uint32_t eventGroup) {
    size_t count = 0;
    capacity = std::min<size_t>(capacity, 32);
    const auto appendShape = [&](float x, float z, uint16_t yaw, float halfWidth, float halfDepth, float forwardOffset) {
        AuxiliaryCollisionEntry &entry = outShapes[count++];
        std::memset(&entry, 0, offsetof(AuxiliaryCollisionEntry, shape));
        QueryShape &shape = entry.shape;
        if (std::fabs(halfDepth) < 0.00000011920929f) {
            shape.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
            shape.packed[0] = {halfWidth, x, z};
            return;
        }
        const float sine = sin_fixed16(yaw);
        const float cosine = sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(yaw));
        shape.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle;
        shape.packed[0].z = (halfDepth + halfDepth) * cosine;
        shape.packed[1].x = (-halfDepth + -halfDepth) * sine;
        shape.packed[1].y = (halfWidth + halfWidth) * sine;
        shape.packed[1].z = (halfWidth + halfWidth) * cosine;
        shape.packed[0].x = (sine * -forwardOffset + x) - (shape.packed[1].y + shape.packed[0].z) * 0.5f;
        shape.packed[0].y = (cosine * -forwardOffset + z) - (shape.packed[1].z + shape.packed[1].x) * 0.5f;
    };

    for (const CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet); npc && count < capacity;
         npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        if (!npc->cycle->motnSteeringActive)
            continue;
        const uint32_t group = npc->cycle->obj ? npc->cycle->obj->eventGroup : 0;
        if (group != eventGroup)
            continue;
        const Vector3 position = npc->cycle->obj ? GetPositionForEntity(npc->cycle->obj->entity.node) : Vector3{};
        float forwardOffset = 0.0f;
        if (npc->cycle->moveObjRuntime) {
            if (npc->cycle->moveObjRuntime->moveObjectKind == 5)
                forwardOffset = 1.0f;
            else if (npc->cycle->moveObjRuntime->moveObjectKind <= 1)
                forwardOffset = -npc->cycle->moveObjRuntime->tokenLocalOffset.z;
        }
        appendShape(position.x, position.z, static_cast<uint16_t>(npc->cycle->desiredPathYaw), npc->cycle->steeringHalfWidth, npc->cycle->steeringHalfDepth,
            forwardOffset);
    }

    for (uint16_t index = 0; index < g_dynamicTurnCollisionVolumeCount && count < capacity; ++index) {
        const CycleManager::IntersectionEntry &entry = g_cycleManagerRuntime.dynamicTurnCollisionVolumes[index];
        appendShape(SCN_FloatFromU32(entry.operand), SCN_FloatFromU32(entry.dwordC), static_cast<uint16_t>(entry.dword10), SCN_FloatFromU32(entry.dword14),
            SCN_FloatFromU32(entry.dword18), -SCN_FloatFromU32(entry.dword1C));
    }
    return count;
}
} // namespace CycleManager

namespace CycleManager {

MobjCollisionObject *MOBJ_CollisionObjects() {
    return reinterpret_cast<MobjCollisionObject *>(g_pMOBJ);
}

const std::uint8_t *MapCollisionMask() {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr;
}


float GetMoveObjectShapeOffset(const CycleManager::NpcRecord &npc) {
    if (const CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime) {
        if (runtime->moveObjectKind == 5u)
            return 1.0f;
        if (runtime->moveObjectKind <= 1u)
            return -runtime->tokenLocalOffset.z;
    }
    return 0.0f;
}

bool IsFixedPositionSteeringActor(const CycleManager::NpcRecord &npc) {
    return npc.cycle->moveObjRuntime || npc.cycle->moveConfig->cycleClassCode - 4u <= 3u;
}

void ApplyRotationMatrix(const OrientedSteeringQuadParams &params) {
    const uint16_t yaw = static_cast<uint16_t>(params.yaw);
    const float negativeSine = -sin_fixed16(static_cast<int16_t>(yaw));
    const float negativeCosine = -cos_fixed16(yaw);
    const float depthZ = params.half_depth * negativeCosine;
    const float offsetZ = params.yaw_offset * negativeCosine;
    const float widthZ = params.half_width * negativeCosine;
    const float offsetX = params.yaw_offset * negativeSine;
    const float widthX = params.half_width * negativeSine;
    const float depthX = params.half_depth * negativeSine;
    const float frontX = depthZ + widthX;
    const float sideX = widthX - depthZ;
    const float frontZ = widthZ - depthX;
    const float sideZ = depthX + widthZ;
    const Vector3 &center = *params.center;
    Vector3 *const corners = params.out_corners;
    corners[0] = {(frontX + center.x) + offsetX, center.y, (frontZ + center.z) + offsetZ};
    corners[1] = {(center.x - sideX) + offsetX, center.y, (center.z - sideZ) + offsetZ};
    corners[2] = {(center.x - frontX) + offsetX, center.y, (center.z - frontZ) + offsetZ};
    corners[3] = {(sideX + center.x) + offsetX, center.y, (sideZ + center.z) + offsetZ};
}

float Vector3_CalculateUnitVectorBetweenPoints(Vector3 &result, const Vector3 &start, int16_t yaw, const Vector3 &end, float radius) {
    Vector3 delta{start.x - end.x, 0.0f, start.z - end.z};
    float length = 0.0f;
    if (delta.x > kSteeringEpsilon || delta.x < -kSteeringEpsilon || delta.z > kSteeringEpsilon || delta.z < -kSteeringEpsilon) {
        length = Vector3::NormalizeInPlace(delta);
        result.x = delta.x;
        result.z = delta.z;
    } else {
        result.x = sin_fixed16(static_cast<int16_t>(static_cast<uint16_t>(yaw)));
        result.z = cos_fixed16(static_cast<uint16_t>(yaw));
    }
    return radius - length;
}

bool IsPointInsideQuadrilateral(const Vector3 &point, const Vector3 *quad) {
    int edge = 0;
    for (; edge < 4; ++edge) {
        const Vector3 &start = quad[edge];
        const Vector3 &end = quad[(edge + 1) & 3];
        if ((point.x - start.x) * (end.z - start.z) - (point.z - start.z) * (end.x - start.x) < 0.0f)
            break;
    }
    return edge == 4;
}


bool CalculatePointVectorRelativeToTwoPoints(Vector3 &push, const SteeringShapeXZ &circle, const SteeringShapeXZ &quad) {
    int8_t firstOutside = 0;
    uint8_t outsideCount = 0;
    const float x = circle.center.x;
    const float z = circle.center.z;
    for (int8_t edge = 0; edge < 4; ++edge) {
        const Vector3 &start = quad.quad_corners[edge];
        const Vector3 &end = quad.quad_corners[(edge + 1) & 3];
        if ((x - start.x) * (end.z - start.z) - (z - start.z) * (end.x - start.x) < 0.0f) {
            if (!outsideCount || edge - 1 == firstOutside)
                firstOutside = edge;
            ++outsideCount;
        }
    }
    if (outsideCount == 1) {
        const uint16_t angle = static_cast<uint16_t>(quad.yaw + (firstOutside + 1) * 0x4000);
        const float normalX = -sin_fixed16(static_cast<int16_t>(angle));
        const float normalZ = -cos_fixed16(angle);
        const Vector3 &corner = quad.quad_corners[firstOutside];
        const float depth = circle.radius - ((z - corner.z) * normalZ + (x - corner.x) * normalX);
        if (depth <= 0.0f)
            return false;
        push.x = depth * normalX;
        push.z = depth * normalZ;
        return true;
    }
    if (outsideCount == 2) {
        Vector3 direction{};
        const float depth = Vector3_CalculateUnitVectorBetweenPoints(direction, quad.quad_corners[firstOutside], quad.yaw, circle.center, circle.radius);
        if (depth <= 0.0f)
            return false;
        push.x = -(depth * direction.x);
        push.z = -(depth * direction.z);
        return true;
    }
    const int edge = ((static_cast<uint16_t>(circle.yaw - quad.yaw) >> 14) - 2) & 3;
    const float deltaX = x - quad.quad_corners[edge].x;
    const float deltaZ = z - quad.quad_corners[edge].z;
    const uint16_t angle = static_cast<uint16_t>(quad.yaw + (edge + 1) * 0x4000);
    const float sine = sin_fixed16(static_cast<int16_t>(angle));
    const float cosine = cos_fixed16(angle);
    float depth = cosine * deltaZ + sine * deltaX;
    if (depth <= 0.0f)
        return false;
    depth += circle.radius;
    push.z = depth * -cosine;
    push.x = depth * -sine;
    return true;
}


bool CalculateNormalVectorForQuadrilateral(Vector3 &push, const SteeringShapeXZ &lhs, const SteeringShapeXZ &rhs) {
    bool found = false;
    float depth = 1000.0f;
    float normalX = 0.0f;
    float normalZ = 0.0f;
    const int lhsQuarter = (static_cast<uint16_t>(lhs.yaw - rhs.yaw) >> 14) & 3;
    for (int corner = 0; corner < 4; ++corner) {
        if (!IsPointInsideQuadrilateral(lhs.quad_corners[corner], rhs.quad_corners))
            continue;
        const int edge = (lhsQuarter - 2 + corner) & 3;
        const float deltaX = lhs.quad_corners[corner].x - rhs.quad_corners[edge].x;
        const float deltaZ = lhs.quad_corners[corner].z - rhs.quad_corners[edge].z;
        const uint16_t angle = static_cast<uint16_t>(rhs.yaw + (edge + 1) * 0x4000);
        const float sine = sin_fixed16(static_cast<int16_t>(angle));
        const float cosine = cos_fixed16(angle);
        const float distance = cosine * deltaZ + sine * deltaX;
        if (depth * depth > distance * distance) {
            depth = distance;
            normalX = -sine;
            normalZ = -cosine;
        }
        found = true;
    }
    const int rhsQuarter = (static_cast<uint16_t>(rhs.yaw - lhs.yaw) >> 14) & 3;
    for (int corner = 0; corner < 4; ++corner) {
        if (!IsPointInsideQuadrilateral(rhs.quad_corners[corner], lhs.quad_corners))
            continue;
        const int edge = (rhsQuarter - 2 + corner) & 3;
        const float deltaX = rhs.quad_corners[corner].x - lhs.quad_corners[edge].x;
        const float deltaZ = rhs.quad_corners[corner].z - lhs.quad_corners[edge].z;
        const uint16_t angle = static_cast<uint16_t>(lhs.yaw + (edge + 1) * 0x4000);
        const float sine = sin_fixed16(static_cast<int16_t>(angle));
        const float cosine = cos_fixed16(angle);
        const float distance = cosine * deltaZ + sine * deltaX;
        if (depth * depth > distance * distance) {
            normalX = sine;
            normalZ = cosine;
            depth = distance;
        }
        found = true;
    }
    if (found) {
        push.x = normalX * depth;
        push.z = normalZ * depth;
    }
    return found;
}
} // namespace CycleManager
  // namespace
bool check_if_point_in_quadrilateral(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, const Vector3 &point) {
    const float edgeA = (b.x - a.x) * (point.z - a.z) - (point.x - a.x) * (b.z - a.z);
    const float edgeB = (c.x - b.x) * (point.z - b.z) - (c.z - b.z) * (point.x - b.x);
    const float edgeC = (d.x - c.x) * (point.z - c.z) - (d.z - c.z) * (point.x - c.x);
    const float edgeD = (point.z - d.z) * (a.x - d.x) - (a.z - d.z) * (point.x - d.x);
    if (edgeA > 0.0f)
        return edgeB >= 0.0f && edgeC >= 0.0f && edgeD >= 0.0f;
    if (edgeA >= 0.0f && edgeB > 0.0f)
        return edgeC >= 0.0f && edgeD >= 0.0f;
    if (edgeA < 0.0f && edgeB > 0.0f)
        return false;
    return edgeC <= 0.0f && edgeD <= 0.0f;
}

bool shenResolveSteeringShapeOverlapXZ(SteeringShapeXZ &lhs, SteeringShapeXZ &rhs) {
    Vector3 push{};
    float pushX = 0.0f;
    float pushZ = 0.0f;
    bool overlapped = false;
    if (lhs.radius <= 0.0f) {
        if (rhs.radius > 0.0f) {
            overlapped = CalculatePointVectorRelativeToTwoPoints(push, rhs, lhs);
            pushX = -push.x;
            pushZ = -push.z;
        } else {
            overlapped = CalculateNormalVectorForQuadrilateral(push, lhs, rhs);
            pushX = push.x;
            pushZ = push.z;
        }
    } else if (rhs.radius <= 0.0f) {
        overlapped = CalculatePointVectorRelativeToTwoPoints(push, lhs, rhs);
        pushX = push.x;
        pushZ = push.z;
    } else {
        const float depth = Vector3_CalculateUnitVectorBetweenPoints(push, lhs.center, lhs.yaw, rhs.center, rhs.radius) + lhs.radius;
        pushX = push.x;
        pushZ = push.z;
        if (depth > 0.0f) {
            pushX = push.x * depth;
            overlapped = true;
            pushZ = push.z * depth;
        }
    }
    if (!overlapped)
        return false;

    float share = 0.0f;
    if (lhs.push_resistance == 0)
        share = rhs.push_resistance ? 1.0f : 0.5f;
    else if (lhs.push_resistance == 0xFF)
        share = rhs.push_resistance == 0xFF ? 0.5f : 0.0f;
    else
        share = static_cast<float>(rhs.push_resistance) / (static_cast<float>(lhs.push_resistance) + static_cast<float>(rhs.push_resistance));
    lhs.center.x = pushX * share + lhs.center.x;
    lhs.center.z = pushZ * share + lhs.center.z;
    rhs.center.x = rhs.center.x - pushX * (1.0f - share);
    rhs.center.z = rhs.center.z - pushZ * (1.0f - share);
    return true;
}

bool cmCheckFacingShape(const SteeringShapeXZ &shape, const std::uint8_t *collisionMask) {
    (void)QueryFrameTime();
    QueryShape query{};
    if (shape.radius <= 0.0f) {
        const Vector3 &origin = shape.quad_corners[0];
        query.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle;
        query.packed[0] = {origin.x, origin.z, shape.quad_corners[3].x - origin.x};
        query.packed[1] = {shape.quad_corners[3].z - origin.z, shape.quad_corners[1].x - origin.x, shape.quad_corners[1].z - origin.z};
    } else {
        query.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
        query.packed[0] = {shape.radius, shape.center.x, shape.center.z};
    }
    const bool hit = ExecuteMathFnWrapper(&query, collisionMask) != 0;
    (void)QueryFrameTime();
    return hit;
}


void MoveObj_FindEntryWrapper(CycleManager::NpcRecord &npc) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    uint8_t *const entry = cm.schedule.empty() || cm.bytes.empty() ?
        nullptr :
        CycleManager::MoveObj_FindEntry(cm.schedule.data(), CycleManager::CurrentAreaId(), 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    g_currentMobjRectangleOffset = entry ? static_cast<uint32_t>(entry - cm.bytes.data()) : 0;
    if (npc.cycle->obj && npc.cycle->mot) {
        npc.cycle->obj->eventGroup = npc.cycle->proximityGroup;
        SelectCharacterMapQuery(npc.cycle->obj);
    } else {
        (void)mapQuerySelectActiveGridSection(npc.cycle->proximityGroup);
    }
}
namespace CycleManager {



bool CheckPointInRectangle(const Vector3 &point) {
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (!g_currentMobjRectangleOffset)
        return false;
    bool inside = false;
    for (size_t cursor = g_currentMobjRectangleOffset; cursor <= cm.bytes.size() && 0x20u <= cm.bytes.size() - cursor; cursor += 0x20u) {
        float values[8]{};
        std::memcpy(values, cm.bytes.data() + cursor, sizeof(values));
        if (std::fabs(values[0]) < kSteeringEpsilon && std::fabs(values[1]) < kSteeringEpsilon && std::fabs(values[2]) < kSteeringEpsilon &&
            std::fabs(values[3]) < kSteeringEpsilon)
            break;
        const Vector3 corners[4]{{values[0], 0.0f, values[1]}, {values[2], 0.0f, values[3]}, {values[4], 0.0f, values[5]}, {values[6], 0.0f, values[7]}};
        int edge = 0;
        for (; edge < 4; ++edge) {
            const Vector3 &start = corners[edge];
            const Vector3 &end = corners[(edge + 1) & 3];
            if ((point.x - start.x) * (end.z - start.z) - (point.z - start.z) * (end.x - start.x) < 0.0f)
                break;
        }
        inside |= edge == 4;
    }
    return inside;
}
} // namespace CycleManager


bool CheckIntersectionOnPoint(float positionX, float positionZ, float targetX, float targetZ) {
    QueryShape line{};
    line.mode = CalculateLineIntersection_0;
    line.packed[0] = {positionX, positionZ, targetX};
    line.packed[1].x = targetZ;
    if (ExecuteMathFnWrapper(&line, CycleManager::MapCollisionMask()))
        return true;
    const float startY = MapProbe_UpdateIntersectionStateAtPoint(Vector3{positionX, 1000.0f, positionZ});
    const float probeY = MapProbe_UpdateIntersectionStateAtPoint(
        Vector3{(targetX - positionX) * 0.30000001f + positionX, 1000.0f, (targetZ - positionZ) * 0.30000001f + positionZ});
    return (startY - probeY) * (startY - probeY) > 0.090000004f;
}
namespace CycleManager {



void CollectCollisionObjects() {
    g_mobjCollisionObjectCount = 0;
    MobjCollisionObject *const objects = MOBJ_CollisionObjects();
    if (!objects)
        return;
    std::array<CCOWSteeringShapeRef, 40> shapes{};
    const size_t count = CCOW_CollectMobjSteeringShapes(shapes.data(), shapes.size());
    for (size_t index = 0; index < count; ++index)
        objects[g_mobjCollisionObjectCount++] = {.character = shapes[index].character, .shape = shapes[index].shape};
}
} // namespace CycleManager



bool CalculateAndRotateVectorBetweenPoints(const CycleManager::NpcRecord &npc, uint32_t objectId, CycleProgramList list, SteeringShapeXZ &shape, bool runStart) {
    const float scale = runStart ? g_runStartSpeed : 1.0f;
    for (const CycleManager::NpcRecord *actor = CycleManager::NextProgramInList(list); actor; actor = CycleManager::NextProgramInList(list, actor)) {
        if (!actor->cycle->obj || actor->cycle->moveConfig->obj_id.name == objectId || actor->cycle->trackedMobj == npc.cycle)
            continue;
        SteeringShapeXZ other{};
        other.push_resistance = 127;
        other.center = GetPositionForEntity(actor->cycle->obj->entity.node);
        other.yaw = actor->cycle->desiredPathYaw;
        if (actor->cycle->steeringHalfDepth <= 0.0f) {
            other.radius = actor->cycle->steeringHalfWidth;
        } else {
            other.radius = 0.0f;
            ApplyRotationMatrix({.center = &other.center,
                .yaw = actor->cycle->desiredPathYaw,
                .half_width = scale * actor->cycle->steeringHalfWidth,
                .half_depth = scale * actor->cycle->steeringHalfDepth,
                .yaw_offset = CycleManager::GetMoveObjectShapeOffset(*actor),
                .out_corners = other.quad_corners});
        }
        Vector3 push{};
        bool overlapped = false;
        if (shape.radius <= 0.0f) {
            overlapped =
                other.radius <= 0.0f ? CalculateNormalVectorForQuadrilateral(push, shape, other) : CalculatePointVectorRelativeToTwoPoints(push, other, shape);
        } else if (other.radius > 0.0f) {
            overlapped = Vector3_CalculateUnitVectorBetweenPoints(push, shape.center, shape.yaw, other.center, other.radius) + shape.radius > 0.0f;
        } else {
            overlapped = CalculatePointVectorRelativeToTwoPoints(push, shape, other);
        }
        if (overlapped)
            return true;
    }
    return false;
}
namespace CycleManager {


bool CheckClearanceAgainstRyoAndCycles(const CycleManager::NpcRecord &npc, bool runStart) {
    SteeringShapeXZ shape{};
    shape.center = npc.cycle->pos;
    shape.yaw = npc.cycle->desiredPathYaw;
    if (npc.cycle->steeringHalfDepth > 0.0f) {
        shape.radius = 0.0f;
        ApplyRotationMatrix({.center = &shape.center,
            .yaw = npc.cycle->desiredPathYaw,
            .half_width = npc.cycle->steeringHalfWidth,
            .half_depth = npc.cycle->steeringHalfDepth,
            .yaw_offset = CycleManager::GetMoveObjectShapeOffset(npc),
            .out_corners = shape.quad_corners});
    } else {
        shape.radius = npc.cycle->steeringHalfWidth;
    }
    shape.push_resistance = 0;
    Character *const ryo = CycleManager::MapRyo();
    SteeringShapeXZ player{};
    player.center = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    player.yaw = CycleManager::NodeChainYaw(ryo);
    player.push_resistance = 127;
    player.radius = g_fGlobalTurningRadius;
    Vector3 push{};
    if (shape.radius <= 0.0f) {
        if (CalculatePointVectorRelativeToTwoPoints(push, player, shape))
            return true;
    } else if (Vector3_CalculateUnitVectorBetweenPoints(push, shape.center, shape.yaw, player.center, player.radius) + shape.radius > 0.0f) {
        return true;
    }
    return CalculateAndRotateVectorBetweenPoints(npc, npc.cycle->moveConfig->obj_id.name, CycleProgramList_SimSet, shape, runStart) ||
        CalculateAndRotateVectorBetweenPoints(npc, npc.cycle->moveConfig->obj_id.name, CycleProgramList_Running, shape, runStart);
}


std::array<Vector3, 4> BuildForkliftSteeringQuad(const Vector3 &center, std::int16_t yaw) {
    std::array<Vector3, 4> corners{};
    ApplyRotationMatrix(
        {.center = &center, .yaw = yaw, .half_width = 1.45f, .half_depth = 0.55000001f, .yaw_offset = 0.40000001f, .out_corners = corners.data()});
    return corners;
}
} // namespace CycleManager

namespace CycleManager {


float CalculateSquaredDistance3DPoints(const CycleManager::NpcRecord &npc) {
    Vector3 position = npc.cycle->pos;
    if (!CycleManager::IsFixedPositionSteeringActor(npc) && npc.cycle->obj) {
        if (const MotionInformationWork *const motion = npc.cycle->mot)
            position = motion->rootWorldPosition;
        else
            position = GetPositionForEntity(npc.cycle->obj->entity.node);
    }
    return (position.z - npc.cycle->steeringTarget.z) * (position.z - npc.cycle->steeringTarget.z) +
        (position.x - npc.cycle->steeringTarget.x) * (position.x - npc.cycle->steeringTarget.x);
}

bool SteerTowardActorWithGate(CycleManager::NpcRecord &npc, SteeringShapeXZ &shape, const CycleManager::NpcRecord &actor) {
    if (actor.cycle->moveObjRuntime) {
        if ((npc.cycle->flags & 0x40u) == 0)
            return false;
    } else if (((npc.cycle->flags & 0x10u) == 0 && actor.cycle->moveConfig->cycleClassCode <= 3u) ||
        ((npc.cycle->flags & 0x20u) == 0 && actor.cycle->moveConfig->cycleClassCode - 4u <= 3u)) {
        return false;
    }
    SteeringShapeXZ other{};
    other.center = actor.cycle->pos;
    if (!CycleManager::IsFixedPositionSteeringActor(actor) && actor.cycle->obj) {
        if (const MotionInformationWork *const motion = actor.cycle->mot)
            other.center = motion->rootWorldPosition;
        else
            other.center = GetPositionForEntity(actor.cycle->obj->entity.node);
    }
    other.yaw = actor.cycle->desiredPathYaw;
    if (actor.cycle->steeringHalfDepth <= 0.0f) {
        other.radius = actor.cycle->steeringHalfWidth;
    } else {
        other.radius = 0.0f;
        ApplyRotationMatrix({.center = &other.center,
            .yaw = actor.cycle->desiredPathYaw,
            .half_width = actor.cycle->steeringHalfWidth,
            .half_depth = actor.cycle->steeringHalfDepth,
            .yaw_offset = CycleManager::GetMoveObjectShapeOffset(actor),
            .out_corners = other.quad_corners});
    }
    other.push_resistance = 0xFF;
    const bool overlapped = shenResolveSteeringShapeOverlapXZ(shape, other);
    if (overlapped) {
        if (actor.cycle->moveObjRuntime) {
            npc.steeringCollisionResult |= 0x40u;
        } else {
            if (actor.cycle->moveConfig->cycleClassCode <= 3u)
                npc.steeringCollisionResult |= 0x10u;
            if (actor.cycle->moveConfig->cycleClassCode - 4u <= 3u)
                npc.steeringCollisionResult |= 0x20u;
        }
    }
    return overlapped;
}

bool SteerTowardBlockerWithGate(SteeringShapeXZ &shape, const CycleManager::IntersectionEntry &volume) {
    SteeringShapeXZ other{};
    other.center = {SCN_FloatFromU32(volume.operand), SCN_FloatFromU32(volume.offset), SCN_FloatFromU32(volume.dwordC)};
    other.yaw = static_cast<int16_t>(volume.dword10);
    const float halfWidth = SCN_FloatFromU32(volume.dword14);
    const float halfDepth = SCN_FloatFromU32(volume.dword18);
    if (halfDepth <= 0.0f) {
        other.radius = halfWidth;
    } else {
        other.radius = 0.0f;
        ApplyRotationMatrix({.center = &other.center,
            .yaw = other.yaw,
            .half_width = halfWidth,
            .half_depth = halfDepth,
            .yaw_offset = -SCN_FloatFromU32(volume.dword1C),
            .out_corners = other.quad_corners});
    }
    other.push_resistance = 0xFF;
    return shenResolveSteeringShapeOverlapXZ(shape, other);
}



bool SteerAroundRyoCorridor(CycleManager::NpcRecord &npc, SteeringShapeXZ &shape, CycleManager::NpcRecord &actor) {
    if ((npc.cycle->flags & 0x100u) == 0 || npc.cycle->trackedMobj == actor.cycle || actor.cycle->trackedMobj == npc.cycle || npc.cycle->xm_resume_state)
        return false;
    const float extentSquared =
        actor.cycle->steeringHalfDepth * actor.cycle->steeringHalfDepth + actor.cycle->steeringHalfWidth * actor.cycle->steeringHalfWidth;
    Character *const ryo = CycleManager::MapRyo();
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const Vector3 actorPosition = actor.cycle->obj ? GetPositionForEntity(actor.cycle->obj->entity.node) : Vector3{};
    Vector3 direction{actorPosition.x - ryoPosition.x, 0.0f, actorPosition.z - ryoPosition.z};
    SteeringShapeXZ corridor{};
    corridor.yaw = static_cast<int16_t>(atan2_fixed16(-direction.x, -direction.z));
    corridor.center = {(ryoPosition.x + actorPosition.x) * 0.5f, 0.0f, (ryoPosition.z + actorPosition.z) * 0.5f};
    corridor.radius = 0.0f;
    const float halfLength = Vector3::NormalizeInPlace(direction) * 0.5f;
    ApplyRotationMatrix({.center = &corridor.center,
        .yaw = corridor.yaw,
        .half_width = halfLength,
        .half_depth = std::sqrt(extentSquared),
        .yaw_offset = 0.0f,
        .out_corners = corridor.quad_corners});
    const Vector3 savedCenter = shape.center;
    const int16_t savedYaw = shape.yaw;
    corridor.push_resistance = 0xFF;
    bool overlapped = shenResolveSteeringShapeOverlapXZ(shape, corridor);
    if (overlapped) {
        SteeringShapeXZ previous{};
        previous.center = npc.cycle->steeringTarget;
        previous.yaw = npc.cycle->steeringTargetYaw;
        const float halfWidth = npc.cycle->steeringHalfWidth * 0.89999998f;
        if (npc.cycle->steeringHalfDepth <= 0.0f) {
            previous.radius = halfWidth;
        } else {
            previous.radius = 0.0f;
            ApplyRotationMatrix({.center = &previous.center,
                .yaw = previous.yaw,
                .half_width = halfWidth,
                .half_depth = npc.cycle->steeringHalfDepth * 0.89999998f,
                .yaw_offset = CycleManager::GetMoveObjectShapeOffset(npc),
                .out_corners = previous.quad_corners});
        }
        if (shenResolveSteeringShapeOverlapXZ(previous, corridor)) {
            shape.center = savedCenter;
            shape.yaw = savedYaw;
            overlapped = false;
        }
    }
    return overlapped;
}

void ComputeRotatedProjectionBounds(SteeringProjectionBounds &bounds, const ObjectProjectionData &basis, const ObjectProjectionData &shape) {
    const float deltaX = shape.center.x - basis.center.x;
    const float deltaZ = shape.center.z - basis.center.z;
    if (deltaZ * deltaZ + deltaX * deltaX > basis.halfBoxExtent.x * basis.halfBoxExtent.x)
        return;
    if (0.0f > deltaZ * basis.forward.z + deltaX * basis.forward.x)
        return;

    const float sweepX = shape.halfBoxExtent.x * shape.forward.x;
    const float sweepZ = shape.halfBoxExtent.x * shape.forward.z;
    Vector3 corners[4]{};
    if (shape.halfBoxExtent.y > 0.0f) {
        const float halfDepth = shape.halfBoxExtent.z > 0.0f ? shape.halfBoxExtent.z : shape.halfBoxExtent.y;
        ApplyRotationMatrix({.center = &shape.center,
            .yaw = shape.facingAngle16,
            .half_width = shape.halfBoxExtent.y * 1.1f,
            .half_depth = halfDepth * 1.1f,
            .yaw_offset = shape.angYoffset,
            .out_corners = corners});
        corners[0].x += sweepX;
        corners[0].z += sweepZ;
        corners[3].x += sweepX;
        corners[3].z += sweepZ;
    } else {
        for (int corner = 0; corner < 4; ++corner) {
            corners[corner].x = shape.corners[corner].x;
            corners[corner].z = shape.corners[corner].z;
        }
    }

    const float one = sin_fixed16(static_cast<int16_t>(0x4000));
    const float zero = sin_fixed16(static_cast<int16_t>(0));
    float leftX = basis.forward.x * zero - basis.forward.z * one;
    float leftZ = basis.forward.x * one + basis.forward.z * zero;
    float rightX = basis.forward.z * one + basis.forward.x * zero;
    float rightZ = basis.forward.z * zero - basis.forward.x * one;
    for (int pass = 0; pass < 2; ++pass) {
        const Vector3 &anchor = basis.corners[pass > 0 ? 2 : 1];
        for (const Vector3 &corner : corners) {
            const float x = corner.x - anchor.x;
            const float z = corner.z - anchor.z;
            if (x <= kSteeringEpsilon && x >= -kSteeringEpsilon && z <= kSteeringEpsilon && z >= -kSteeringEpsilon)
                continue;
            if (leftZ * x - leftX * z > 0.0f) {
                leftX = x;
                leftZ = z;
            }
            if (0.0f > rightZ * x - rightX * z) {
                rightX = x;
                rightZ = z;
            }
            const float distanceSquared = (x * x + 0.0f) + z * z;
            if (bounds.minDistanceSquared > distanceSquared)
                bounds.minDistanceSquared = distanceSquared;
        }
    }

    Vector3 *const directions = bounds.directions;
    if (leftX * directions[0].z - leftZ * directions[0].x > 0.0f) {
        directions[0].x = leftX;
        directions[0].z = leftZ;
    }
    if (0.0f > rightX * directions[1].z - rightZ * directions[1].x) {
        directions[1].x = rightX;
        directions[1].z = rightZ;
    }
    if (rightX * basis.forward.z - rightZ * basis.forward.x > 0.0f) {
        if (0.0f > rightX * directions[2].z - rightZ * directions[2].x) {
            directions[2].x = rightX;
            directions[2].z = rightZ;
        }
        return;
    }
    if (0.0f > leftX * basis.forward.z - leftZ * basis.forward.x) {
        if (leftX * directions[3].z - leftZ * directions[3].x > 0.0f) {
            directions[3].x = leftX;
            directions[3].z = leftZ;
        }
        return;
    }
    const float leftLength = std::sqrt((leftX * leftX + 0.0f) + leftZ * leftZ);
    if (leftLength > 0.0f) {
        leftX = leftX * (1.0f / leftLength);
        leftZ = leftZ * (1.0f / leftLength);
    }
    const float rightLength = std::sqrt((rightX * rightX + 0.0f) + rightZ * rightZ);
    if (rightLength > 0.0f) {
        rightX = rightX * (1.0f / rightLength);
        rightZ = rightZ * (1.0f / rightLength);
    }
    if (rightZ * basis.heading.z + rightX * basis.heading.x > leftZ * basis.heading.z + leftX * basis.heading.x) {
        directions[3].x = leftX;
        directions[3].z = leftZ;
    } else {
        directions[2].z = rightZ;
        directions[2].x = rightX;
    }
}

void SetProjectionFacing(ObjectProjectionData &query, int16_t yaw) {
    query.facingAngle16 = yaw;
    const uint16_t angle = static_cast<uint16_t>(yaw);
    query.forward = {-sin_fixed16(static_cast<int16_t>(angle)), 0.0f, -cos_fixed16(angle)};
    query.heading = {-sin_fixed16(static_cast<int16_t>(angle)), 0.0f, -cos_fixed16(angle)};
}


void ProjectSteeringVolume(SteeringProjectionBounds &bounds, const ObjectProjectionData &basis, const CycleManager::IntersectionEntry &volume) {
    ObjectProjectionData query{};
    query.center = {SCN_FloatFromU32(volume.operand), SCN_FloatFromU32(volume.offset), SCN_FloatFromU32(volume.dwordC)};
    CycleManager::SetProjectionFacing(query, static_cast<int16_t>(volume.dword10));
    query.projHeight = 0.0f;
    query.halfBoxExtent = {0.0f, SCN_FloatFromU32(volume.dword14), SCN_FloatFromU32(volume.dword18)};
    query.angYoffset = -SCN_FloatFromU32(volume.dword1C);
    ComputeRotatedProjectionBounds(bounds, basis, query);
}


bool BuildCollisionShapeCorners(const MobjCollisionObject &object, Vector3 *corners) {
    const Vector3 position = object.character ? GetPositionForEntity(object.character->entity.node) : Vector3{};
    const QueryShape &shape = *object.shape;
    if (shape.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange) {
        ApplyRotationMatrix({.center = &position,
            .yaw = CycleManager::NodeChainYaw(object.character),
            .half_width = shape.packed[0].x,
            .half_depth = shape.packed[0].x,
            .yaw_offset = 0.0f,
            .out_corners = corners});
        return true;
    }
    if (shape.mode == OrientedBox_OrientedRectXZ_IsPointInTriangle) {
        corners[0] = {shape.packed[0].x, position.y, shape.packed[0].y};
        corners[3] = {corners[0].x + shape.packed[0].z, position.y, corners[0].z + shape.packed[1].x};
        corners[1] = {corners[0].x + shape.packed[1].y, position.y, corners[0].z + shape.packed[1].z};
        corners[2] = {shape.packed[1].y + corners[3].x, position.y, shape.packed[1].z + corners[3].z};
        return true;
    }
    for (int corner = 0; corner < 4; ++corner)
        corners[corner] = position;
    return false;
}


void ProjectActorSteeringBounds(
    SteeringProjectionBounds &bounds, const CycleManager::NpcRecord &npc, const ObjectProjectionData &basis, CycleManager::NpcRecord &actor) {
    if (!actor.cycle->obj || actor.cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name || (npc.cycle->flags & 0x100u) == 0)
        return;
    ObjectProjectionData query{};
    if (actor.cycle->xm_resume_state) {
        Character *const ryo = CycleManager::MapRyo();
        const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
        const Vector3 actorPosition = GetPositionForEntity(actor.cycle->obj->entity.node);
        Vector3 direction{actorPosition.x - ryoPosition.x, 0.0f, actorPosition.z - ryoPosition.z};
        query.center = {(ryoPosition.x + actorPosition.x) * 0.5f, 0.0f, (ryoPosition.z + actorPosition.z) * 0.5f};
        const float halfLength = Vector3::NormalizeInPlace(direction) * 0.5f;
        query.forward = direction;
        query.heading = direction;
        query.angYoffset = 0.0f;
        query.halfBoxExtent = {0.0f, halfLength, actor.cycle->steeringHalfWidth};
        query.facingAngle16 = static_cast<int16_t>(atan2_fixed16(-direction.x, -direction.z));
        query.projHeight = 0.0f;
        ComputeRotatedProjectionBounds(bounds, basis, query);
    }
    const CycleManager::NpcRecord *const tracked = CycleManager::GetTrackedMobj(actor);
    if (tracked == &npc &&
        (tracked->cycle->pos.z - actor.cycle->pos.z) * (tracked->cycle->pos.z - actor.cycle->pos.z) +
                (tracked->cycle->pos.x - actor.cycle->pos.x) * (tracked->cycle->pos.x - actor.cycle->pos.x) >
            2.25f)
        return;
    query.center = GetPositionForEntity(actor.cycle->obj->entity.node);
    CycleManager::SetProjectionFacing(query, actor.cycle->desiredPathYaw);
    const float step = npcCalculateMovementDistanceStep(actor);
    query.projHeight = step;
    query.halfBoxExtent = {step * 30.0f, actor.cycle->steeringHalfWidth, actor.cycle->steeringHalfDepth};
    query.angYoffset = CycleManager::GetMoveObjectShapeOffset(actor);
    ComputeRotatedProjectionBounds(bounds, basis, query);
}
} // namespace CycleManager
  // namespace

namespace CycleManager {



bool AdjustDesiredYawForSteeringCollision(CycleManager::NpcRecord &npc) {
    (void)QueryFrameTime();
    ObjectProjectionData subject{};
    subject.center = npc.cycle->pos;
    const int16_t yaw = npc.cycle->obj ? CycleManager::NodeChainYaw(npc.cycle->obj) : npc.cycle->desiredPathYaw;
    subject.facingAngle16 = yaw;
    const uint16_t facing = static_cast<uint16_t>(yaw);
    const uint16_t desired = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
    subject.forward = {-sin_fixed16(static_cast<int16_t>(facing)), 0.0f, -cos_fixed16(facing)};
    subject.heading = {-sin_fixed16(static_cast<int16_t>(desired)), 0.0f, -cos_fixed16(desired)};
    const float speed = npcCalculateMovementDistanceStep(npc);
    subject.projHeight = speed;
    subject.halfBoxExtent = {speed * 150.0f, npc.cycle->steeringHalfWidth, npc.cycle->steeringHalfDepth};
    subject.angYoffset = CycleManager::GetMoveObjectShapeOffset(npc);
    ApplyRotationMatrix({.center = &subject.center,
        .yaw = yaw,
        .half_width = npc.cycle->steeringHalfWidth,
        .half_depth = npc.cycle->steeringHalfDepth > 0.0f ? npc.cycle->steeringHalfDepth : npc.cycle->steeringHalfWidth,
        .yaw_offset = subject.angYoffset,
        .out_corners = subject.corners});

    const float one = sin_fixed16(static_cast<int16_t>(0x4000));
    const float zero = sin_fixed16(static_cast<int16_t>(0));
    const Vector3 &forward = subject.forward;
    SteeringProjectionBounds bounds{};
    bounds.directions[0] = {zero * forward.x - one * forward.z, 0.0f, one * forward.x + zero * forward.z};
    bounds.directions[1] = {one * forward.z + zero * forward.x, 0.0f, zero * forward.z - one * forward.x};
    bounds.directions[2] = bounds.directions[1];
    bounds.directions[3] = bounds.directions[0];
    bounds.minDistanceSquared = subject.halfBoxExtent.x * subject.halfBoxExtent.x;

    ObjectProjectionData query{};
    if ((npc.cycle->flags & 8u) != 0) {
        MobjCollisionObject *const objects = MOBJ_CollisionObjects();
        g_mobjCollisionObjectCursor = 0;
        for (uint16_t index = 0; objects && index < g_mobjCollisionObjectCount; ++index) {
            g_mobjCollisionObjectCursor = static_cast<uint16_t>(index + 1);
            const MobjCollisionObject &object = objects[index];
            if (object.character == npc.cycle->obj || !CycleManager::BuildCollisionShapeCorners(object, query.corners))
                continue;
            query.center = object.character ? GetPositionForEntity(object.character->entity.node) : Vector3{};
            CycleManager::SetProjectionFacing(query, CycleManager::NodeChainYaw(object.character));
            query.projHeight = 0.0f;
            query.halfBoxExtent = {};
            query.angYoffset = 0.0f;
            ComputeRotatedProjectionBounds(bounds, subject, query);
        }
    }
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if ((npc.cycle->flags & 0x80u) != 0) {
        g_dynamicTurnCollisionVolumeCursor = 0;
        for (uint16_t index = 0; index < g_dynamicTurnCollisionVolumeCount; ++index) {
            g_dynamicTurnCollisionVolumeCursor = static_cast<uint16_t>(index + 1);
            CycleManager::ProjectSteeringVolume(bounds, subject, cm.dynamicTurnCollisionVolumes[index]);
        }
        g_steeringIntersectionVolumeCursor = 0;
        for (uint16_t index = 0; index < g_steeringIntersectionVolumeCount && index < cm.intersectionEntries.size(); ++index) {
            g_steeringIntersectionVolumeCursor = static_cast<uint16_t>(index + 1);
            CycleManager::ProjectSteeringVolume(bounds, subject, cm.intersectionEntries[index]);
        }
    }
    if ((npc.cycle->flags & 4u) != 0) {
        Character *const ryo = CycleManager::MapRyo();
        query.center = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
        CycleManager::SetProjectionFacing(query, CycleManager::NodeChainYaw(ryo));
        const float ryoSpeed = g_pControlPadController ? g_pControlPadController->resolvedMovementSpeed : 0.0f;
        query.projHeight = ryoSpeed;
        query.halfBoxExtent = {ryoSpeed * 150.0f * 0.5f, g_fGlobalTurningRadius, g_default_turning_box_half_depth_z};
        query.angYoffset = g_angYoffset;
        ComputeRotatedProjectionBounds(bounds, subject, query);
    }
    const uint16_t opcode = static_cast<uint16_t>(npc.cycle->currentOpcode);
    const bool maskedStep = (opcode <= CycleOpcode_WalkPath3D && ((0x200800802ull >> opcode) & 1u) != 0) ||
        (opcode == CycleOpcode_StartLinkedCycle && npc.cycle->moveObjSubOpcode == CycleOpcode_WalkPath3D);
    if ((!CycleManager::IsFixedPositionSteeringActor(npc) && !maskedStep) || 0.079999998f > npcCalculateMovementDistanceStep(npc)) {
        for (CycleManager::NpcRecord *actor = CycleManager::NextProgramInList(CycleProgramList_SimSet); actor;
             actor = CycleManager::NextProgramInList(CycleProgramList_SimSet, actor))
            CycleManager::ProjectActorSteeringBounds(bounds, npc, subject, *actor);
        for (CycleManager::NpcRecord *actor = CycleManager::NextProgramInList(CycleProgramList_Running); actor;
             actor = CycleManager::NextProgramInList(CycleProgramList_Running, actor))
            CycleManager::ProjectActorSteeringBounds(bounds, npc, subject, *actor);
    }

    float probeLength = 2.0f;
    if (4.0f > bounds.minDistanceSquared)
        probeLength = std::sqrt(bounds.minDistanceSquared) * 2.0f;
    const Vector3 &heading = subject.heading;
    bool mapBlocked = false;
    bool search = false;
    if ((npc.cycle->flags & 1u) != 0 && !CycleManager::CheckPointInRectangle(subject.center)) {
        mapBlocked = CheckIntersectionOnPoint(
            subject.center.x, subject.center.z, probeLength * heading.x + subject.center.x, probeLength * heading.z + subject.center.z);
        search = mapBlocked;
    }
    const Vector3 *directions = bounds.directions;
    if (!search) {
        if (0.0f > directions[0].x * heading.z - directions[0].z * heading.x || directions[1].x * heading.z - directions[1].z * heading.x > 0.0f)
            return true;
        if (directions[2].x * heading.z - directions[2].z * heading.x > 0.0f && 0.0f > directions[3].x * heading.z - directions[3].z * heading.x)
            return true;
    }

    Vector3 *candidates[4]{&bounds.directions[0], &bounds.directions[1], &bounds.directions[2], &bounds.directions[3]};
    float alignment[4]{};
    for (int index = 0; index < 4; ++index) {
        (void)Vector3::NormalizeInPlace(*candidates[index]);
        alignment[index] = heading.z * candidates[index]->z + heading.x * candidates[index]->x;
    }
    for (int index = 1; index - 1 < 3; ++index) {
        for (int other = index; other < 4; ++other) {
            if (alignment[other] > alignment[index - 1]) {
                Vector3 *const swapped = candidates[index - 1];
                candidates[index - 1] = candidates[other];
                candidates[other] = swapped;
                const float swappedAlignment = alignment[index - 1];
                alignment[index - 1] = alignment[other];
                alignment[other] = swappedAlignment;
            }
        }
    }
    const float facingSide = heading.z * forward.x - heading.x * forward.z;
    int sameSide = 0;
    for (int index = 0; index < 4; ++index) {
        const Vector3 &candidate = *candidates[index];
        const float side = heading.z * candidate.x - heading.x * candidate.z;
        if (facingSide > 0.0f ? side > 0.0f : 0.0f > side) {
            Vector3 *const swapped = candidates[sameSide];
            candidates[sameSide] = candidates[index];
            candidates[index] = swapped;
            ++sameSide;
        }
    }

    const float limit = sin_fixed16(static_cast<int16_t>(0x2000));
    const float extentSquared = subject.halfBoxExtent.z * subject.halfBoxExtent.z + subject.halfBoxExtent.y * subject.halfBoxExtent.y;
    for (int index = 0; index < 4; ++index) {
        const Vector3 &candidate = *candidates[index];
        if ((extentSquared > bounds.minDistanceSquared || heading.z * forward.z + heading.x * forward.x > 0.0f) &&
            limit > forward.z * candidate.z + forward.x * candidate.x)
            continue;
        if ((npc.cycle->flags & 2u) != 0) {
            const Vector3 &corner = subject.corners[0.0f > heading.z * candidate.x - heading.x * candidate.z ? 3 : 0];
            if (CheckIntersectionOnPoint(corner.x, corner.z, probeLength * candidate.x + corner.x, probeLength * candidate.z + corner.z))
                continue;
        }
        npc.cycle->desiredPathYaw = static_cast<int16_t>(atan2_fixed16(-candidate.x, -candidate.z));
        (void)QueryFrameTime();
        return true;
    }
    (void)QueryFrameTime();
    return !mapBlocked && !(extentSquared > bounds.minDistanceSquared);
}
} // namespace CycleManager




bool UpdateNpcSteeringFromMOTN(CycleManager::NpcRecord &npc, bool applyPosition) {
    (void)QueryFrameTime();
    const uint16_t opcode = static_cast<uint16_t>(npc.cycle->currentOpcode);
    const bool dcbnArea = opcode <= 0x22u && ((0x4000C1084ull >> opcode) & 1u) != 0 && CycleManager::CurrentAreaId() == SHEN_NAME(DCBN);
    const bool fixedActor = CycleManager::IsFixedPositionSteeringActor(npc);
    MotionInformationWork *const motion = npc.cycle->mot;
    npc.steeringCollisionResult = 0;
    if (!dcbnArea && applyPosition) {
        Vector3 current = npc.cycle->pos;
        if (!fixedActor && npc.cycle->obj)
            current = motion ? motion->rootWorldPosition : GetPositionForEntity(npc.cycle->obj->entity.node);
        const float moved = std::fabs((current.z - npc.cycle->steeringTarget.z) * (current.z - npc.cycle->steeringTarget.z) +
            (current.x - npc.cycle->steeringTarget.x) * (current.x - npc.cycle->steeringTarget.x));
        if (FLT_EPSILON > moved && opcode != CycleOpcode_RequestNearestDoorTransition) {
            npc.cycle->flags = npc.cycle->previousFlags | 2u;
            return false;
        }
    }
    if (opcode == CycleOpcode_InitTimedPath || opcode == CycleOpcode_BeginTalkMoveToTaggedPosition)
        npc.cycle->flags |= 3u;

    Vector3 start = npc.cycle->pos;
    if (!fixedActor && npc.cycle->motnSteeringActive && npc.cycle->obj)
        start = motion ? motion->rootWorldPosition : GetPositionForEntity(npc.cycle->obj->entity.node);
    const int8_t group = static_cast<int8_t>(npc.cycle->proximityGroup);
    SteeringShapeXZ shape{};
    shape.center = start;
    shape.yaw = npc.cycle->desiredPathYaw;
    if (npc.cycle->steeringHalfDepth > 0.0f) {
        shape.radius = 0.0f;
        ApplyRotationMatrix({.center = &shape.center,
            .yaw = npc.cycle->desiredPathYaw,
            .half_width = npc.cycle->steeringHalfWidth,
            .half_depth = npc.cycle->steeringHalfDepth,
            .yaw_offset = CycleManager::GetMoveObjectShapeOffset(npc),
            .out_corners = shape.quad_corners});
    } else {
        shape.radius = npc.cycle->steeringHalfWidth;
    }
    shape.push_resistance = 0;

    bool adjusted = false;
    for (CycleManager::NpcRecord *actor = CycleManager::NextProgramInList(CycleProgramList_SimSet); actor;
         actor = CycleManager::NextProgramInList(CycleProgramList_SimSet, actor)) {
        if (!actor->cycle->motnSteeringActive || !actor->cycle->obj || actor->cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name ||
            static_cast<int8_t>(actor->cycle->proximityGroup) != group)
            continue;
        if (actor->cycle->xm_resume_state)
            adjusted |= CycleManager::SteerAroundRyoCorridor(npc, shape, *actor);
        const float ownDistance = CalculateSquaredDistance3DPoints(npc);
        if (CalculateSquaredDistance3DPoints(*actor) <= ownDistance || !applyPosition)
            adjusted |= SteerTowardActorWithGate(npc, shape, *actor);
    }
    for (CycleManager::NpcRecord *actor = CycleManager::NextProgramInList(CycleProgramList_Running); actor;
         actor = CycleManager::NextProgramInList(CycleProgramList_Running, actor)) {
        if (!actor->cycle->obj || actor->cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name ||
            static_cast<int32_t>(actor->cycle->obj->eventGroup) != group)
            continue;
        if (actor->cycle->xm_resume_state)
            adjusted |= CycleManager::SteerAroundRyoCorridor(npc, shape, *actor);
        adjusted |= SteerTowardActorWithGate(npc, shape, *actor);
    }

    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if ((npc.cycle->flags & 0x80u) != 0) {
        const Vector3 savedCenter = shape.center;
        const int16_t savedYaw = shape.yaw;
        bool blocked = false;
        g_dynamicTurnCollisionVolumeCursor = 0;
        for (uint16_t index = 0; index < g_dynamicTurnCollisionVolumeCount; ++index) {
            g_dynamicTurnCollisionVolumeCursor = static_cast<uint16_t>(index + 1);
            blocked |= SteerTowardBlockerWithGate(shape, cm.dynamicTurnCollisionVolumes[index]);
        }
        g_steeringIntersectionVolumeCursor = 0;
        for (uint16_t index = 0; index < g_steeringIntersectionVolumeCount && index < cm.intersectionEntries.size(); ++index) {
            g_steeringIntersectionVolumeCursor = static_cast<uint16_t>(index + 1);
            blocked |= SteerTowardBlockerWithGate(shape, cm.intersectionEntries[index]);
        }
        if (blocked) {
            if ((npc.cycle->previousFlags & 0x80u) != 0) {
                npc.steeringCollisionResult |= 0x80u;
            } else {
                npc.cycle->flags &= ~0x80u;
                blocked = false;
                shape.center = savedCenter;
                shape.yaw = savedYaw;
            }
        }
        adjusted |= blocked;
    }

    SteeringShapeXZ other{};
    if ((npc.cycle->flags & 8u) != 0) {
        bool hit = false;
        MobjCollisionObject *const objects = MOBJ_CollisionObjects();
        g_mobjCollisionObjectCursor = 0;
        for (uint16_t index = 0; objects && index < g_mobjCollisionObjectCount; ++index) {
            g_mobjCollisionObjectCursor = static_cast<uint16_t>(index + 1);
            const MobjCollisionObject &object = objects[index];
            if (object.character == npc.cycle->obj)
                continue;
            other.center = object.character ? GetPositionForEntity(object.character->entity.node) : Vector3{};
            const QueryShape &collision = *object.shape;
            if (collision.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange) {
                other.radius = collision.packed[0].x;
            } else if (collision.mode == OrientedBox_OrientedRectXZ_IsPointInTriangle) {
                const float y = other.center.y;
                other.radius = 0.0f;
                other.quad_corners[0] = {collision.packed[0].x, y, collision.packed[0].y};
                other.quad_corners[3] = {other.quad_corners[0].x + collision.packed[0].z, y, other.quad_corners[0].z + collision.packed[1].x};
                other.quad_corners[1] = {other.quad_corners[0].x + collision.packed[1].y, y, other.quad_corners[0].z + collision.packed[1].z};
                other.quad_corners[2] = {other.quad_corners[3].x + collision.packed[1].y, y, other.quad_corners[3].z + collision.packed[1].z};
            } else {
                continue;
            }
            other.yaw = CycleManager::NodeChainYaw(object.character);
            other.push_resistance = 0xFF;
            hit |= shenResolveSteeringShapeOverlapXZ(shape, other);
        }
        if (hit)
            npc.steeringCollisionResult |= 8u;
        adjusted |= hit;
    }

    if ((npc.cycle->flags & 4u) != 0) {
        Character *const ryo = CycleManager::MapRyo();
        if ((ryo ? static_cast<int32_t>(ryo->eventGroup) : 0) == group) {
            other.center = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
            other.yaw = CycleManager::NodeChainYaw(ryo);
            if (g_default_turning_box_half_depth_z > 0.0f) {
                other.radius = 0.0f;
                const float widen = dcbnArea ? 0.30000001f : 0.0f;
                ApplyRotationMatrix({.center = &other.center,
                    .yaw = other.yaw,
                    .half_width = dcbnArea ? g_fGlobalTurningRadius + widen : g_fGlobalTurningRadius,
                    .half_depth = dcbnArea ? g_default_turning_box_half_depth_z + widen : g_default_turning_box_half_depth_z,
                    .yaw_offset = g_angYoffset,
                    .out_corners = other.quad_corners});
            } else {
                other.radius = g_fGlobalTurningRadius;
                if (dcbnArea)
                    other.radius = other.radius + 0.30000001f;
            }
            other.push_resistance = 0xFF;
            const bool hit = shenResolveSteeringShapeOverlapXZ(shape, other);
            if (hit)
                npc.steeringCollisionResult |= 4u;
            adjusted |= hit;
        }
    }

    if (npc.steeringCollisionResult && applyPosition && !dcbnArea && !fixedActor && motion && opcode <= 0x22u && ((0x4040C1094ull >> opcode) & 1u) != 0 &&
        motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[8]) {
        const uint16_t templateIndex = static_cast<uint16_t>(motion->basePoseTemplateIndex);
        Vector3 origin{};
        Vector3 travelled{};
        (void)MOTN_ReadRootPositionAtFrame(motion->currentAnimId, templateIndex, 0.0f, origin, false);
        const float frame = 0.0f > motion->frameTimerRef ? motion->frameTimer : motion->frameTimerRef;
        (void)MOTN_ReadRootPositionAtFrame(motion->currentAnimId, templateIndex, frame, travelled, false);
        const float allowed = (travelled.z - origin.z) * (travelled.z - origin.z) + (travelled.x - origin.x) * (travelled.x - origin.x);
        const float deltaX = shape.center.x - npc.cycle->xmptResumePos.x;
        const float deltaZ = shape.center.z - npc.cycle->xmptResumePos.z;
        const float moved = deltaZ * deltaZ + deltaX * deltaX;
        if (moved > allowed) {
            float scale = 1.0f;
            if (!(FLT_EPSILON > std::fabs(moved)))
                scale = std::sqrt(allowed / moved);
            adjusted = true;
            shape.center.x = npc.cycle->xmptResumePos.x + deltaX * scale;
            shape.center.z = npc.cycle->xmptResumePos.z + deltaZ * scale;
        }
    }

    if ((npc.cycle->flags & 3u) == 3u && !CycleManager::CheckPointInRectangle(shape.center)) {
        MoveObj_FindEntryWrapper(npc);
        if (cmCheckFacingShape(shape, CycleManager::MapCollisionMask())) {
            adjusted = true;
            npc.steeringCollisionResult |= 2u;
            shape.center = npc.cycle->steeringTarget;
            shape.yaw = npc.cycle->steeringTargetYaw;
        }
    }

    if (applyPosition && adjusted) {
        npc.cycle->pos.x = shape.center.x;
        npc.cycle->pos.z = shape.center.z;
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
        const float offsetX = shape.center.x - start.x;
        const float offsetZ = shape.center.z - start.z;
        if (fixedActor && npc.cycle->moveObjRuntime)
            npc.cycle->moveObjRuntime->pos = npc.cycle->pos;
        if (npc.cycle->obj)
            shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
        if (motion)
            MOTI_ApplyPositionOffset(*motion, Vector3{offsetX, 0.0f, offsetZ});
    }
    (void)QueryFrameTime();
    return adjusted;
}
namespace CycleManager {


std::int32_t GetAreaIdx() {
    std::int32_t count = 0;
    for (const CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.cycle->areaID.name == CycleManager::CurrentAreaId())
            ++count;
    }
    return count;
}

bool AreaCapacityExceeded(std::uint32_t areaId) {
    bool allowed = true;
    for (const CycleAreaCapacity *entry = kCycleAreaCapacities; entry->areaId; ++entry) {
        if (entry->areaId == areaId) {
            if (entry->capacity >= GetAreaIdx())
                break;
            allowed = false;
        }
    }
    return !allowed;
}

std::uint32_t CommandWord(std::uint32_t offset) {
    std::uint32_t value = 0;
    ReadValue(g_cycleManagerRuntime.bytes, offset, value);
    return value;
}


float getFloatValueFromLUT_Wrapper(const MotionInformationWork *motion, std::uint16_t animId) {
    if (!motion)
        return 0.041666668f;
    const float frames = static_cast<float>(MOTN_getFrameCountFromAnimId(animId) - motnGetRootMotionEndMarker(animId));
    if (1.0f > frames)
        return 0.041666668f;
    Vector3 end{};
    (void)MOTN_ReadRootPositionAtFrame(animId, static_cast<std::uint16_t>(motion->basePoseTemplateIndex), frames, end, true);
    if (FLT_EPSILON > std::fabs(end.z))
        return 0.041666668f;
    return -(end.z / frames);
}


float MOBJ_OnlyReadMOTNPositionValues(Vector3 &travel, const CycleManager::NpcRecord &npc) {
    const MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion || !npc.cycle->obj || !npc.cycle->motnSteeringActive)
        return 0.0017361112f;
    const std::uint16_t templateIndex = static_cast<std::uint16_t>(motion->basePoseTemplateIndex);
    Vector3 start{};
    float end = motion->frameTimer;
    if (motion->frameTimer == motion->frameStart) {
        (void)MOTN_ReadRootPositionAtFrame(motion->currentAnimId, templateIndex, motion->frameTimer, start, false);
        end = motion->playbackRate + motion->frameTimer;
    } else {
        (void)MOTN_ReadRootPositionAtFrame(motion->currentAnimId, templateIndex, motion->frameTimerRef, start, false);
    }
    (void)MOTN_ReadRootPositionAtFrame(motion->currentAnimId, templateIndex, end, travel, false);
    travel.z = travel.z - start.z;
    travel.x = travel.x - start.x;
    return travel.z * travel.z + travel.x * travel.x;
}

bool PathAtEnd(const CycleManager::NpcRecord &npc) {
    return npc.cycle->reversePath ? npc.cycle->pathPointIndex <= 0 : static_cast<std::uint16_t>(npc.cycle->pathPointIndex + 1) >= npc.cycle->pathPointCount;
}

bool npcSetCurrentPathTarget(CycleManager::NpcRecord &npc, Vector3 &target) {
    target = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
    npc.cycle->currentWaypointPos = target;
    return CycleManager::PathAtEnd(npc);
}


bool CheckIfWithinRange(CycleManager::NpcRecord &npc) {
    const Vector3 waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
    npc.cycle->currentWaypointPos = waypoint;
    const float distance =
        (waypoint.z - npc.cycle->pos.z) * (waypoint.z - npc.cycle->pos.z) + (waypoint.x - npc.cycle->pos.x) * (waypoint.x - npc.cycle->pos.x);
    Character *const ryo = CycleManager::MapRyo();
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    if (distance > (waypoint.z - ryoPosition.z) * (waypoint.z - ryoPosition.z) + (waypoint.x - ryoPosition.x) * (waypoint.x - ryoPosition.x))
        return true;
    for (const CycleManager::NpcRecord *other = CycleManager::NextProgramInList(CycleProgramList_SimSet); other;
         other = CycleManager::NextProgramInList(CycleProgramList_SimSet, other)) {
        if (distance >
            (waypoint.z - other->cycle->pos.z) * (waypoint.z - other->cycle->pos.z) + (waypoint.x - other->cycle->pos.x) * (waypoint.x - other->cycle->pos.x))
            return true;
    }
    return false;
}


bool cyclIsRyoBlockingDobuitaWaypoint(CycleManager::NpcRecord &npc) {
    if (!g_cycleManagerRuntime.isWithinDobuita)
        return false;
    Character *const ryo = CycleManager::MapRyo();
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const Vector3 waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
    npc.cycle->currentWaypointPos = waypoint;
    if ((waypoint.z - ryoPosition.z) * (waypoint.z - ryoPosition.z) + (waypoint.x - ryoPosition.x) * (waypoint.x - ryoPosition.x) > 1.0f)
        return false;
    Vector3 heading{waypoint.x - npc.cycle->pos.x, 0.0f, waypoint.z - npc.cycle->pos.z};
    if (heading.x > kSteeringEpsilon || heading.x < -kSteeringEpsilon || heading.z > kSteeringEpsilon || heading.z < -kSteeringEpsilon) {
        (void)Vector3::NormalizeInPlace(heading);
        Vector3 toRyo{ryoPosition.x - npc.cycle->pos.x, 0.0f, ryoPosition.z - npc.cycle->pos.z};
        if (toRyo.x > kSteeringEpsilon || toRyo.x < -kSteeringEpsilon || toRyo.z > kSteeringEpsilon || toRyo.z < -kSteeringEpsilon) {
            (void)Vector3::NormalizeInPlace(toRyo);
            if (sin_fixed16(static_cast<int16_t>(5462)) <= toRyo.z * heading.z + toRyo.x * heading.x)
                return true;
        }
    }
    return false;
}

RadialStepResult npcClassifySegmentVsCircleXZ(const Vector3 &start, const Vector3 &end, const Vector3 &center, float radius) {
    const float startX = start.x - center.x;
    const float startZ = start.z - center.z;
    const float radiusSquared = radius * radius;
    const float startDistance = startZ * startZ + startX * startX;
    const float endDistance = (end.z - center.z) * (end.z - center.z) + (end.x - center.x) * (end.x - center.x);
    if (radiusSquared > startDistance && radiusSquared > endDistance)
        return InsideBoth;
    const float segmentZ = start.z - end.z;
    const float segmentX = start.x - end.x;
    const float segmentLength = segmentZ * segmentZ + segmentX * segmentX;
    if (radiusSquared <= startDistance) {
        if (radiusSquared > endDistance)
            return CrossesCircle;
    } else if (startDistance > segmentLength) {
        return StayInside;
    }
    const std::int16_t segmentYaw = static_cast<std::int16_t>(atan2_fixed16(segmentX, segmentZ));
    const std::int16_t angle = static_cast<std::int16_t>(static_cast<std::int16_t>(atan2_fixed16(startX, startZ)) - segmentYaw);
    if (static_cast<std::uint32_t>(angle + 0x4000) > 0x8000u)
        return NoHit;
    const float sine = sin_fixed16(angle);
    if (sine * sine * segmentLength > radiusSquared)
        return NoHit;
    return startDistance > segmentLength ? NoHit : CrossesCircle;
}

bool IntersectSegmentsXZ(Vector3 &out, const Vector3 &start, const Vector3 &end, const Vector3 &edgeStart, const Vector3 &edgeEnd) {
    const float edgeX = edgeEnd.x - edgeStart.x;
    const float edgeZ = edgeEnd.z - edgeStart.z;
    const float segmentZ = end.z - start.z;
    const float segmentX = end.x - start.x;
    const float denominator = edgeX * segmentZ - segmentX * edgeZ;
    if (!(std::fabs(denominator) >= kSteeringEpsilon)) {
        out = {};
        return false;
    }
    out.y = 0.0f;
    const float edgeParam = ((edgeStart.z - start.z) * segmentX - (edgeStart.x - start.x) * segmentZ) / denominator;
    const float segmentParam = ((start.x - edgeStart.x) * edgeZ - (start.z - edgeStart.z) * edgeX) / denominator;
    out.x = segmentX * segmentParam + start.x;
    out.z = segmentZ * segmentParam + start.z;
    return segmentParam >= 0.0f && segmentParam <= 1.0f && edgeParam >= 0.0f && edgeParam <= 1.0f;
}


bool WantsTurnTowardEntityXZ(CycleManager::NpcRecord &npc) {
    Character *const ryo = CycleManager::MapRyo();
    const Vector3 waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
    npc.cycle->currentWaypointPos = waypoint;
    if ((ryo ? static_cast<std::int32_t>(ryo->eventGroup) : 0) != static_cast<std::int8_t>(npc.cycle->proximityGroup))
        return false;
    const Vector3 center = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    if ((center.z - npc.cycle->pos.z) * (center.z - npc.cycle->pos.z) + (center.x - npc.cycle->pos.x) * (center.x - npc.cycle->pos.x) > 2.25f)
        return false;
    if (g_default_turning_box_half_depth_z <= 0.0f)
        return npcClassifySegmentVsCircleXZ(npc.cycle->pos, waypoint, center, g_fGlobalTurningRadius) != NoHit;
    Vector3 box[4]{};
    ApplyRotationMatrix({.center = &center,
        .yaw = CycleManager::NodeChainYaw(ryo),
        .half_width = g_fGlobalTurningRadius,
        .half_depth = g_default_turning_box_half_depth_z,
        .yaw_offset = g_angYoffset,
        .out_corners = box});
    Vector3 hit{};
    for (int edge = 0; edge < 4; ++edge) {
        if (IntersectSegmentsXZ(hit, npc.cycle->pos, waypoint, box[edge], box[(edge + 1) & 3]))
            return true;
    }
    return false;
}


bool npcCheckLinewalkEndpointBusy(CycleManager::NpcRecord &npc) {
    std::uint32_t cursor = CycleManager::FileOffset(npc.cycle->cycleCommandPtr);
    bool scannedPosition = false;
    float x = g_cycleCommandScanPosition.x;
    float z = g_cycleCommandScanPosition.z;
    g_cycleCommandScanHasPosition = 0;
    bool reachedEnd = false;
    for (;;) {
        const std::uint32_t opcode = CycleManager::CommandWord(cursor) & 0xffffu;
        if (!opcode || static_cast<std::int16_t>(opcode) >= 0x3e)
            return false;
        if (opcode == 0 || opcode == 0xA) {
            cursor += 4;
        } else if (opcode == 3) {
            g_cycleCommandScanHasPosition = 1;
            scannedPosition = true;
            ReadValue(g_cycleManagerRuntime.bytes, cursor + 4, g_cycleCommandScanPosition);
            x = g_cycleCommandScanPosition.x;
            z = g_cycleCommandScanPosition.z;
            cursor += 20;
        } else if (opcode == 0x10 || opcode == 0x2D) {
            cursor += 48;
        } else if (opcode == 0x19) {
            cursor += 24;
        } else if (opcode == 0x1A || opcode == 0x30) {
            cursor += 16;
        } else if (opcode == 2 || opcode == 7 || opcode == 9 || opcode == 0xF || opcode == 0x11 || opcode == 0x28 || opcode == 0x2E || opcode == 0x37 ||
            opcode == 0x38) {
            cursor += 8;
        } else {
            reachedEnd = true;
            break;
        }
    }
    if (!reachedEnd || CycleManager::CommandWord(cursor) != CycleOpcode_RequestNearestDoorTransition)
        return false;
    std::uint32_t next = cursor + 4;
    if (!scannedPosition) {
        const Vector3 &endpoint =
            npc.cycle->reversePath ? npc.cycle->pathPoints[0] : npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointCount - 1)];
        x = endpoint.x;
        z = endpoint.z;
    }
    Character *const ryo = CycleManager::MapRyo();
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    if (!((ryoPosition.z - z) * (ryoPosition.z - z) + (ryoPosition.x - x) * (ryoPosition.x - x) >= 4.0f))
        return true;
    g_cycleCommandScanHasPosition = 0;
    for (;;) {
        const std::uint32_t opcode = CycleManager::CommandWord(next) & 0xffffu;
        if (!opcode || static_cast<std::int16_t>(opcode) >= 0x3e)
            return false;
        if (opcode == 0xA) {
            next += 4;
        } else if (opcode == 1 || opcode == 0x1A || opcode == 0x30 || opcode == 0x36) {
            next += 16;
        } else if (opcode == 2 || opcode == 7 || opcode == 9 || opcode == 0xF || opcode == 0x11 || opcode == 0x28 || opcode == 0x2E || opcode == 0x37 ||
            opcode == 0x38) {
            next += 8;
        } else if (opcode == 3) {
            g_cycleCommandScanHasPosition = 1;
            ReadValue(g_cycleManagerRuntime.bytes, next + 4, g_cycleCommandScanPosition);
            next += 20;
        } else if (opcode == 0x10 || opcode == 0x2D) {
            next += 48;
        } else if (opcode == 0x19) {
            next += 24;
        } else if (opcode == 0x21 || opcode == 0x39) {
            next += 12;
        } else {
            break;
        }
    }
    if (CycleManager::CommandWord(next) != CycleOpcode_SetAreaTag)
        return false;
    const std::uint32_t areaId = CycleManager::CommandWord(next + 4);
    return areaId == CycleManager::CurrentAreaId() && CycleManager::AreaCapacityExceeded(areaId);
}



bool npcCheckPathEndpointOccupiedWithin10Units(CycleManager::NpcRecord &npc) {
    std::uint32_t cursor = CycleManager::FileOffset(npc.cycle->cycleCommandPtr);
    g_cycleCommandScanHasPosition = 0;
    for (;;) {
        const std::uint32_t opcode = CycleManager::CommandWord(cursor) & 0xffffu;
        if (!opcode || static_cast<std::int16_t>(opcode) >= 0x3e)
            return false;
        if (opcode == 0xA) {
            cursor += 4;
        } else if (opcode == 3) {
            g_cycleCommandScanHasPosition = 1;
            ReadValue(g_cycleManagerRuntime.bytes, cursor + 4, g_cycleCommandScanPosition);
            cursor += 20;
        } else if (opcode == 0x10 || opcode == 0x2D) {
            cursor += 48;
        } else if (opcode == 0x19) {
            cursor += 24;
        } else if (opcode == 0x1A || opcode == 0x30) {
            cursor += 16;
        } else if (opcode == 2 || opcode == 7 || opcode == 9 || opcode == 0xF || opcode == 0x11 || opcode == 0x28 || opcode == 0x2E || opcode == 0x37 ||
            opcode == 0x38) {
            cursor += 8;
        } else {
            break;
        }
    }
    if (CycleManager::CommandWord(cursor) != CycleOpcode_FollowStairPath2D)
        return false;
    const Vector3 &endpoint =
        npc.cycle->reversePath ? npc.cycle->pathPoints[0] : npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointCount - 1)];
    if (g_playerKaidanGuideActive) {
        Character *const ryo = CycleManager::MapRyo();
        const std::int32_t ryoGroup = ryo ? static_cast<std::int32_t>(ryo->eventGroup) : 0;
        const bool sameStairGroup =
            g_playerInStairPrivateVolume && npc.cycle->proximityGroup && ryoGroup == static_cast<std::int8_t>(npc.cycle->proximityGroup);
        if (!sameStairGroup) {
            const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
            if (100.0f > (ryoPosition.z - endpoint.z) * (ryoPosition.z - endpoint.z) + (ryoPosition.x - endpoint.x) * (ryoPosition.x - endpoint.x))
                return true;
        }
    }
    for (KaidanOccupancyEntry *entry = g_kaidanOccupancyHead;; entry = entry->activeNext) {
        g_kaidanIterator = entry;
        if (!entry)
            return false;
        if (entry->stairGuideEntryId == -1) {
            g_kaidanIterator = nullptr;
            return false;
        }
        const Vector3 &occupant = entry->npc->cycle->pos;
        if (100.0f > (occupant.z - endpoint.z) * (occupant.z - endpoint.z) + (occupant.x - endpoint.x) * (occupant.x - endpoint.x))
            return true;
    }
}

void SetMoveTargetFromYaw(CycleManager::NpcRecord &npc) {
    const std::uint16_t yaw = static_cast<std::uint16_t>(npc.cycle->desiredPathYaw);
    npc.cycle->moveTargetPos = {-sin_fixed16(static_cast<int16_t>(yaw)), 0.0f, -cos_fixed16(yaw)};
}

std::int16_t HeadingYaw(const Vector3 &direction) {
    return static_cast<std::int16_t>(atan2_fixed16(-direction.x, -direction.z));
}


bool CommitPathYaw(CycleManager::NpcRecord &npc) {
    const std::int16_t delta = static_cast<std::int16_t>(npc.cycle->desiredPathYaw - CycleManager::NodeChainYaw(npc.cycle->obj));
    if (std::abs(static_cast<std::int32_t>(delta)) > 364)
        return false;
    CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
    return true;
}



bool SegmentHitsTurnGateXZ(const Vector3 &start, const Vector3 &end, const CycleManager::IntersectionEntry &volume) {
    const Vector3 center{SCN_FloatFromU32(volume.operand), SCN_FloatFromU32(volume.offset), SCN_FloatFromU32(volume.dwordC)};
    const float halfWidth = SCN_FloatFromU32(volume.dword14);
    const float halfDepth = SCN_FloatFromU32(volume.dword18);
    if (halfDepth <= 0.0f)
        return npcClassifySegmentVsCircleXZ(start, end, center, halfWidth) != NoHit;
    Vector3 corners[4]{};
    ApplyRotationMatrix({.center = &center,
        .yaw = static_cast<int16_t>(volume.dword10),
        .half_width = halfWidth,
        .half_depth = halfDepth,
        .yaw_offset = -SCN_FloatFromU32(volume.dword1C),
        .out_corners = corners});
    Vector3 hit{};
    for (int edge = 0; edge < 4; ++edge) {
        if (IntersectSegmentsXZ(hit, start, end, corners[edge], corners[(edge + 1) & 3]))
            return true;
    }
    return false;
}
} // namespace CycleManager
  // namespace



std::int32_t checkCharacterDistanceFromCoords(std::uint32_t charaId, float distance) {
    std::int32_t result = 1;
    std::size_t count = static_cast<std::size_t>(g_MOBJ_num_entries > 0 ? g_MOBJ_num_entries : 0);
    if (count > g_cycleManagerRuntime.npcRecords.size())
        count = g_cycleManagerRuntime.npcRecords.size();
    for (std::size_t index = 0; index < count; ++index) {
        const CycleManager::NpcRecord &cycle = g_cycleManagerRuntime.npcRecords[index];
        if (cycle.cycle->moveConfig->obj_id.name != charaId)
            continue;
        const std::uint16_t opcode = static_cast<std::uint16_t>(cycle.cycle->currentOpcode);
        const bool resumable = opcode <= 60U && ((0x10000004058c1094ULL >> opcode) & 1U) != 0U;
        if (!(opcode == CycleOpcode_StartLinkedCycle && static_cast<std::uint32_t>(cycle.cycle->moveObjPhase - 4U) <= 2U) && !resumable)
            continue;
        Vector3 position = cycle.cycle->pos;
        if (cycle.cycle->obj && cycle.cycle->mot && cycle.cycle->motnSteeringActive)
            position = GetPositionForEntity(cycle.cycle->obj->entity.node);
        const float dz = cycle.cycle->xmptResumePos.z - position.z;
        const float dx = cycle.cycle->xmptResumePos.x - position.x;
        if (dz * dz + dx * dx > distance * distance)
            result = 0;
    }
    return result;
}



bool CheckVectorIntersectionWithMaxAngleAndIterate(const Vector3 &start, const Vector3 &end) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    g_dynamicTurnCollisionVolumeCursor = 0;
    for (uint16_t index = 0; index < g_dynamicTurnCollisionVolumeCount; ++index) {
        g_dynamicTurnCollisionVolumeCursor = static_cast<uint16_t>(index + 1);
        if (SegmentHitsTurnGateXZ(start, end, cm.dynamicTurnCollisionVolumes[index]))
            return true;
    }
    g_steeringIntersectionVolumeCursor = 0;
    for (uint16_t index = 0; index < g_steeringIntersectionVolumeCount; ++index) {
        g_steeringIntersectionVolumeCursor = static_cast<uint16_t>(index + 1);
        if (SegmentHitsTurnGateXZ(start, end, cm.intersectionEntries[index]))
            return true;
    }
    return false;
}

static bool shenStepPathPointIndex(CycleManager::NpcRecord &npc) {
    if (npc.cycle->reversePath) {
        if (npc.cycle->pathPointIndex <= 0) {
            npc.cycle->pathPointIndex = 0;
            return true;
        }
        --npc.cycle->pathPointIndex;
        return false;
    }
    const std::int16_t next = static_cast<std::int16_t>(npc.cycle->pathPointIndex + 1);
    if (static_cast<std::uint16_t>(next) >= npc.cycle->pathPointCount) {
        npc.cycle->pathPointIndex = static_cast<std::int16_t>(npc.cycle->pathPointCount - 1);
        return true;
    }
    npc.cycle->pathPointIndex = next;
    return false;
}
namespace CycleManager {


bool ApproachSteeringTargetXZ(CycleManager::NpcRecord &npc, float factor) {
    const float deltaZ = npc.cycle->pos.z - npc.cycle->steeringTarget.z;
    const float deltaX = npc.cycle->pos.x - npc.cycle->steeringTarget.x;
    const float distanceSquared = deltaZ * deltaZ + deltaX * deltaX;
    const float reach = npcCalculateMovementDistanceStep(npc) * factor;
    const bool stationary = std::fabs(distanceSquared) < kSteeringEpsilon;
    if (reach * reach <= distanceSquared) {
        if (stationary) {
            npc.cycle->pos.x = npc.cycle->steeringTarget.x;
            npc.cycle->pos.z = npc.cycle->steeringTarget.z;
        }
        return stationary;
    }
    npc.cycle->pos.x = npc.cycle->steeringTarget.x;
    npc.cycle->pos.z = npc.cycle->steeringTarget.z;
    return true;
}
} // namespace CycleManager


int32_t shenUpdateSteeringState(CycleManager::NpcRecord &npc) {
    std::uint8_t state = npc.cycle->steeringState;
    std::uint32_t reversePath = npc.cycle->reversePath;
    bool resume = false;
    bool blockedOther = false;
    bool countReverse = true;
    npc.cycle->pathTurnRequested = 0;
    switch (npc.cycle->steeringCommand) {
    case CycleSteerCmd_Resume:
        resume = true;
        break;
    case CycleSteerCmd_ToggleReversePath: {
        bool toggle = reversePath != 0;
        if (!toggle && npc.cycle->currentOpcode != CycleOpcode_InitTimedPath && npc.cycle->pathPointIndex == 0) {
            resume = true;
            countReverse = false;
            break;
        }
        if (!toggle && npc.cycle->pathPointIndex == 1) {
            const Vector3 &endpoint = npc.cycle->pathPoints[0];
            if (1.0f > (endpoint.z - npc.cycle->pos.z) * (endpoint.z - npc.cycle->pos.z) + (endpoint.x - npc.cycle->pos.x) * (endpoint.x - npc.cycle->pos.x)) {
                resume = true;
                break;
            }
        }
        npc.cycle->reversePathFrames = 0;
        reversePath = reversePath == 0;
        break;
    }
    case CycleSteerCmd_ForceForwardPath:
        reversePath = 0;
        countReverse = false;
        break;
    case CycleSteerCmd_BlockedOther:
        blockedOther = true;
        break;
    case CycleSteerCmd_YawCorrect:
        state = CycleSteerState_YawCorrecting;
        break;
    default:
        break;
    }
    if (countReverse && reversePath && static_cast<std::int32_t>(++npc.cycle->reversePathFrames) > 600)
        reversePath = 0;

    int32_t result = CycleManager::ApproachSteeringTargetXZ(npc, 0.5f) ? 1 : 0;
    switch (state) {
    case CycleSteerState_Invalid:
        state = CycleSteerState_Normal;
        break;
    case CycleSteerState_Normal:
        if (result || resume) {
            npc.cycle->steeringBlockedFrames = 0;
            state = CycleSteerState_TargetReached;
        } else if (blockedOther) {
            state = CycleSteerState_BlockedOther;
        }
        break;
    case CycleSteerState_TargetReached:
        if (!result && !resume) {
            state = CycleSteerState_Normal;
        } else if (static_cast<std::int32_t>(++npc.cycle->steeringBlockedFrames) > 5) {
            state = CycleSteerState_PreTurnWait;
            npc.cycle->steeringWaitFrames = 0;
        }
        break;
    case CycleSteerState_ForceWait:
        state = CycleSteerState_PreTurnWait;
        break;
    case CycleSteerState_PreTurnWait:
        if (resume)
            state = CycleSteerState_BlockedForward;
        else if (static_cast<std::int32_t>(++npc.cycle->steeringWaitFrames) > 10)
            state = CycleSteerState_RequestTurn;
        break;
    case CycleSteerState_RequestTurn:
        state = CycleSteerState_Normal;
        if (resume)
            state = CycleSteerState_BlockedForward;
        else if (result)
            npc.cycle->pathTurnRequested = 1;
        break;
    case CycleSteerState_BlockedForward:
        if (!resume)
            state = CycleSteerState_Normal;
        break;
    case CycleSteerState_BlockedOther:
        if (!blockedOther)
            state = CycleSteerState_Normal;
        break;
    case CycleSteerState_YawCorrecting: {
        state = CycleSteerState_Normal;
        if (!npc.cycle->motnSteeringActive || !npc.cycle->obj)
            break;
        CycleManager::EnableRootMotion(npc.cycle->obj, false);
        const std::int16_t actualYaw = CycleManager::NodeChainYaw(npc.cycle->obj);
        const std::int16_t delta = static_cast<std::int16_t>(npc.cycle->desiredPathYaw - actualYaw);
        std::int16_t applied = npc.cycle->desiredPathYaw;
        if (delta < 0) {
            if (delta <= -364)
                applied = static_cast<std::int16_t>(actualYaw - 364);
        } else if (delta >= 364) {
            applied = static_cast<std::int16_t>(actualYaw + 364);
        }
        CycleManager::SetNodeChainYaw(*npc.cycle->obj, applied);
        result = static_cast<std::int16_t>(npc.cycle->desiredPathYaw - applied);
        if (!(182.0f > std::fabs(static_cast<float>(result))))
            state = CycleSteerState_YawCorrecting;
        break;
    }
    case CycleSteerState_ReversePause:
        result = static_cast<std::int32_t>(npc.cycle->steeringWaitFrames);
        if (result <= 0)
            state = CycleSteerState_Normal;
        else
            npc.cycle->steeringWaitFrames = static_cast<std::uint32_t>(--result);
        break;
    default:
        npc.cycle->pathTurnRequested = 0;
        npc.cycle->steeringBlockedFrames = 0;
        npc.cycle->reversePathFrames = 0;
        npc.cycle->steeringWaitFrames = 0;
        break;
    }
    npc.cycle->steeringState = static_cast<CycleSteeringState>(state);
    npc.cycle->reversePath = reversePath;
    npc.cycle->steeringCommand = CycleSteerCmd_None;
    return result;
}


bool npcUpdateMovementPath(CycleManager::NpcRecord &npc) {
    if (npc.cycle->currentOpcode == CycleOpcode_RemoveCharacter || static_cast<std::int16_t>(npc.cycle->currentOpcode) >= 0x3e)
        return false;
    const std::uint32_t reversePath = npc.cycle->reversePath;
    shenUpdateSteeringState(npc);
    if (npc.cycle->reversePath != reversePath)
        shenStepPathPointIndex(npc);
    const std::uint8_t state = npc.cycle->steeringState;
    if (((state - 4u) & 0xfcu) == 0 && state != CycleSteerState_RequestTurn)
        return true;
    npc.cycle->baseMoveSpeed = npc.cycle->pathMoveSpeed;
    npc.cycle->animMoveSpeedScale = 0.041666668f;
    const float step = npcCalculateMovementDistanceStep(npc);
    Vector3 waypoint{};
    for (;;) {
        waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->currentWaypointPos = waypoint;
        const bool atEnd = CycleManager::PathAtEnd(npc);
        const float deltaX = npc.cycle->pos.x - waypoint.x;
        const float deltaZ = npc.cycle->pos.z - waypoint.z;
        npc.cycle->moveTargetPos = {deltaX, 0.0f, deltaZ};
        float distance = 0.0f;
        if (deltaX > kSteeringEpsilon || deltaX < -kSteeringEpsilon || deltaZ > kSteeringEpsilon || deltaZ < -kSteeringEpsilon)
            distance = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
        npc.cycle->desiredPathYaw = static_cast<std::int16_t>(atan2_fixed16(npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z));
        if (!npc.cycle->reversePath && atEnd && (npcCheckLinewalkEndpointBusy(npc) || npcCheckPathEndpointOccupiedWithin10Units(npc))) {
            npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
            return true;
        }
        if (step <= distance) {
            npc.cycle->pos.x = npc.cycle->pos.x - step * npc.cycle->moveTargetPos.x;
            npc.cycle->pos.z = npc.cycle->pos.z - step * npc.cycle->moveTargetPos.z;
            return true;
        }
        if (atEnd)
            break;
        shenStepPathPointIndex(npc);
    }
    npc.cycle->pos = waypoint;
    if (cyclIsRyoBlockingDobuitaWaypoint(npc)) {
        npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
        return true;
    }
    if (npc.cycle->reversePath) {
        npc.cycle->steeringCommand = CycleSteerCmd_ForceForwardPath;
        return true;
    }
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
    return false;
}
namespace CycleManager {





bool UpdateMoveObjectPathing(CycleManager::NpcRecord &npc, bool moveObjMode) {
    const bool turnInPlace = npc.cycle->moveConfig->cycleClassCode - 2u <= 1u;
    const std::int16_t opcode = static_cast<std::int16_t>(npc.cycle->currentOpcode);
    if ((!opcode || opcode >= 0x3e) && !moveObjMode)
        return false;
    Character *const obj = npc.cycle->obj;
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!obj || !motion)
        return false;
    npc.cycle->selectedMotionToken = npc.cycle->forcedMotionToken ? npc.cycle->forcedMotionToken : CycleManager::SelectRandomMotionToken(npc);
    if (charaHasBlockingTPMRequest(obj))
        return true;
    CycleManager::EnableRootMotion(obj, true);
    const std::uint32_t reversePath = npc.cycle->reversePath;
    shenUpdateSteeringState(npc);
    if (npc.cycle->steeringState == CycleSteerState_YawCorrecting)
        return true;
    if (npc.cycle->pathTurnRequested) {
        const Vector3 waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->currentWaypointPos = waypoint;
        const bool atEnd = CycleManager::PathAtEnd(npc);
        const float deltaX = waypoint.x - npc.cycle->pos.x;
        const float deltaZ = waypoint.z - npc.cycle->pos.z;
        npc.cycle->desiredPathYaw = static_cast<std::int16_t>(npc.cycle->desiredPathYaw + (atEnd ? 0x8000 : 0x2000));
        if (turnInPlace)
            npc.cycle->steeringState = CycleSteerState_YawCorrecting;
        else
            cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        CycleManager::SetMoveTargetFromYaw(npc);
        if (atEnd || 0.0f > deltaX * npc.cycle->moveTargetPos.x + npc.cycle->moveTargetPos.z * deltaZ)
            npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
        return true;
    }
    bool stepped = false;
    if (npc.cycle->reversePath != reversePath) {
        shenStepPathPointIndex(npc);
        stepped = true;
    }
    const bool steppedOnEntry = stepped;
    const std::uint8_t state = npc.cycle->steeringState;
    if (((state - 4u) & 0xfcu) == 0 && state != CycleSteerState_RequestTurn) {
        if (turnInPlace)
            motion->playbackRate = 0.0f;
        else if (motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[0])
            cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        return true;
    }
    npc.cycle->baseMoveSpeed = npc.cycle->pathMoveSpeed;
    npc.cycle->animMoveSpeedScale = getFloatValueFromLUT_Wrapper(motion, motion->currentAnimId);
    const float step = npcCalculateMovementDistanceStep(npc);
    Vector3 rootTravel{};
    const float rootTravelSquared = MOBJ_OnlyReadMOTNPositionValues(rootTravel, npc);
    float travel = step;
    if (!(rootTravelSquared > step * step) && !(FLT_EPSILON > std::fabs(rootTravelSquared)))
        travel = std::sqrt(rootTravelSquared);
    const float turnDistance = travel * 0.5f / sin_fixed16(static_cast<int16_t>(182));
    Vector3 waypoint{};
    bool atEnd = false;
    float distance = 0.0f;
    for (;;) {
        waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->currentWaypointPos = waypoint;
        atEnd = CycleManager::PathAtEnd(npc);
        npc.cycle->moveTargetPos = {waypoint.x - npc.cycle->pos.x, 0.0f, waypoint.z - npc.cycle->pos.z};
        distance = 0.0f;
        if (npc.cycle->moveTargetPos.x > FLT_EPSILON || -kSteeringEpsilon > npc.cycle->moveTargetPos.x || npc.cycle->moveTargetPos.z > FLT_EPSILON ||
            -kSteeringEpsilon > npc.cycle->moveTargetPos.z)
            distance = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
        if (!npc.cycle->reversePath && atEnd && (npcCheckLinewalkEndpointBusy(npc) || npcCheckPathEndpointOccupiedWithin10Units(npc))) {
            npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
            return true;
        }
        if (!(1.5f > distance) && !(turnDistance > distance))
            break;
        if (!atEnd) {
            shenStepPathPointIndex(npc);
            stepped = true;
            continue;
        }
        if (step > distance || rootTravel.x * rootTravel.x > 0.0f) {
            if (cyclIsRyoBlockingDobuitaWaypoint(npc)) {
                npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
                return true;
            }
            if (npc.cycle->reversePath) {
                npc.cycle->steeringCommand = CycleSteerCmd_ForceForwardPath;
                return true;
            }
            if (step > distance)
                npcSetCurrentPathTarget(npc, npc.cycle->pos);
            if (moveObjMode) {
                npc.cycle->activeAnimId = static_cast<std::uint16_t>(npc.cycle->selectedMotionToken);
                g_shouldComputeBetweenTwoEntities = 0;
                cyclFreeTalkConvSetMoveAndFacingConstraint(npc, static_cast<std::uint16_t>(npc.cycle->selectedMotionToken));
                npc.cycle->currentOpcode = CycleOpcode_SetActiveAnimAndMotionVariant;
                return true;
            }
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            cyclFreeTalkConvSetMoveAndFacingConstraint(npc, npc.cycle->moveConfig->motionParamIds[0]);
            return false;
        }
        if (!npc.cycle->reversePath && CycleManager::FileOffset(npc.cycle->cycleCommandPtr)) {
            std::uint32_t cursor = CycleManager::FileOffset(npc.cycle->cycleCommandPtr);
            for (;;) {
                const std::uint32_t next = CycleManager::CommandWord(cursor);
                if (next == CycleOpcode_InitTimedPath || next == CycleOpcode_SetPositionAndHeading || next == CycleOpcode_UseMnrBEntry)
                    return false;
                if (next != CycleOpcode_SetActionMode && next != CycleOpcode_SetScriptVisibleState)
                    break;
                cursor += 8;
            }
        }
        break;
    }
    if ((atEnd || steppedOnEntry) && stepped) {
        if (turnInPlace) {
            npc.cycle->desiredPathYaw = CycleManager::HeadingYaw(npc.cycle->moveTargetPos);
            npc.cycle->steeringCommand = CycleSteerCmd_YawCorrect;
        } else {
            npc.cycle->steeringCommand = CycleSteerCmd_BlockedOther;
        }
        return true;
    }
    if (npc.cycle->steeringState == CycleSteerState_Normal) {
        CycleManager::SetMoveTargetFromYaw(npc);
        const float deltaX = waypoint.x - npc.cycle->pos.x;
        const float deltaZ = waypoint.z - npc.cycle->pos.z;
        npc.cycle->desiredPathYaw = static_cast<std::int16_t>(atan2_fixed16(-deltaX, -deltaZ));
        if (atEnd) {
            npc.cycle->flags &= ~0x80u;
        } else if (motion->currentAnimId == npc.cycle->activeAnimId || !(0.0f > deltaZ * npc.cycle->moveTargetPos.z + deltaX * npc.cycle->moveTargetPos.x)) {
            (void)CycleManager::AdjustDesiredYawForSteeringCollision(npc);
            const std::int16_t actualYaw = CycleManager::NodeChainYaw(obj);
            std::int16_t turn = static_cast<std::int16_t>(npc.cycle->desiredPathYaw - actualYaw);
            const std::int16_t previous = npc.cycle->moveTargetYaw;
            bool reset = false;
            if (previous < turn) {
                if (turn > 0 && previous < 0)
                    reset = true;
                else if (static_cast<std::int16_t>(previous + 182) < turn)
                    turn = static_cast<std::int16_t>(previous + 182);
            } else if (previous > turn) {
                if (turn < 0 && previous > 0)
                    reset = true;
                else if (static_cast<std::int16_t>(previous - 182) > turn)
                    turn = static_cast<std::int16_t>(previous - 182);
            }
            if (reset) {
                npc.cycle->moveTargetYaw = 0;
                npc.cycle->desiredPathYaw = actualYaw;
            } else {
                if (turn > 364)
                    turn = 364;
                else if (turn < -364)
                    turn = -364;
                npc.cycle->moveTargetYaw = turn;
                npc.cycle->desiredPathYaw = static_cast<std::int16_t>(actualYaw + turn);
            }
        }
    }
    CycleManager::SetMoveTargetFromYaw(npc);
    if (((npc.cycle->steeringState - 1u) & 0xfbu) == 0) {
        npc.cycle->pos.x = step * npc.cycle->moveTargetPos.x + npc.cycle->pos.x;
        npc.cycle->pos.z = step * npc.cycle->moveTargetPos.z + npc.cycle->pos.z;
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
        motion->playbackRate = npc.cycle->pathMoveSpeed;
        if (motion->currentAnimId != npc.cycle->activeAnimId)
            cyclSubmitMoveRequest(npc);
        else if (!CycleManager::CommitPathYaw(npc)) {
            if (turnInPlace)
                npc.cycle->steeringState = CycleSteerState_YawCorrecting;
            else
                cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        }
    }
    return true;
}



bool TickPathFollowMotion(CycleManager::NpcRecord &npc, bool steeringActive) {
    Character *const obj = npc.cycle->obj;
    MotionInformationWork *const motion = npc.cycle->mot;
    const bool turnInPlace = npc.cycle->moveConfig->cycleClassCode - 2u <= 1u;
    const bool drive = steeringActive && obj && motion;
    bool stepped = false;
    bool result = true;
    if (steeringActive)
        npc.cycle->selectedMotionToken = npc.cycle->forcedMotionToken ? npc.cycle->forcedMotionToken : CycleManager::SelectRandomMotionToken(npc);
    if (obj) {
        if (charaHasBlockingTPMRequest(obj))
            return true;
        CycleManager::EnableRootMotion(obj, true);
    }
    const bool lineWalk = npc.cycle->currentOpcode == CycleOpcode_WalkPath3D;
    if (lineWalk && !npc.cycle->reversePath && !npc.cycle->pathPointIndex && CheckIfWithinRange(npc))
        npc.cycle->steeringCommand = CycleSteerCmd_Resume;
    if (WantsTurnTowardEntityXZ(npc))
        npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
    const std::uint32_t reversePath = npc.cycle->reversePath;
    shenUpdateSteeringState(npc);
    if (static_cast<std::uint8_t>(npc.cycle->steeringState - CycleSteerState_YawCorrecting) <= 1u)
        return true;
    if (npc.cycle->pathTurnRequested) {
        npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
        return true;
    }
    if (npc.cycle->reversePath != reversePath) {
        const bool boundary = npc.cycle->reversePath ? npc.cycle->pathPointIndex == npc.cycle->pathPointCount - 1 : npc.cycle->pathPointIndex == 0;
        if (boundary) {
            npc.cycle->steeringState = CycleSteerState_ReversePause;
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            npc.cycle->steeringWaitFrames = 30u * (((g_EnvPrngSeed >> 16) & 1u) + 1u);
        }
        shenStepPathPointIndex(npc);
        stepped = true;
    }
    const std::uint8_t state = npc.cycle->steeringState;
    if (((state - 4u) & 0xfcu) == 0 && state != CycleSteerState_RequestTurn) {
        if (!turnInPlace) {
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            if (drive && motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[0])
                cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        } else if (drive) {
            motion->playbackRate = 0.0f;
        }
        return true;
    }
    npc.cycle->baseMoveSpeed = npc.cycle->pathMoveSpeed;
    npc.cycle->animMoveSpeedScale = getFloatValueFromLUT_Wrapper(motion, npc.cycle->activeAnimId);
    const float step = npcCalculateMovementDistanceStep(npc);
    bool reachedEnd = false;
    for (;;) {
        const Vector3 waypoint = npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->currentWaypointPos = waypoint;
        const bool atEnd = CycleManager::PathAtEnd(npc);
        npc.cycle->moveTargetPos = {waypoint.x - npc.cycle->pos.x, 0.0f, waypoint.z - npc.cycle->pos.z};
        float distance = 0.0f;
        if (npc.cycle->moveTargetPos.x > FLT_EPSILON || -kSteeringEpsilon > npc.cycle->moveTargetPos.x || npc.cycle->moveTargetPos.z > FLT_EPSILON ||
            -kSteeringEpsilon > npc.cycle->moveTargetPos.z)
            distance = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
        if (lineWalk && !npc.cycle->reversePath && atEnd && (npcCheckLinewalkEndpointBusy(npc) || npcCheckPathEndpointOccupiedWithin10Units(npc))) {
            npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
            return true;
        }
        if (step <= distance)
            break;
        npc.cycle->pos = waypoint;
        if (atEnd) {
            reachedEnd = true;
            break;
        }
        shenStepPathPointIndex(npc);
        stepped = true;
    }
    if (reachedEnd) {
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        if (!turnInPlace) {
            if (drive && motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[0])
                cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        } else if (drive) {
            motion->playbackRate = 0.0f;
        }
        if (lineWalk && cyclIsRyoBlockingDobuitaWaypoint(npc)) {
            npc.cycle->steeringCommand = CycleSteerCmd_ToggleReversePath;
            return true;
        }
        if (npc.cycle->reversePath) {
            npc.cycle->steeringCommand = CycleSteerCmd_ForceForwardPath;
            return true;
        }
        npcSetCurrentPathTarget(npc, npc.cycle->pos);
        result = false;
    }
    if (npc.cycle->steeringState == CycleSteerState_ReversePause) {
        if (!turnInPlace) {
            npc.cycle->desiredPathYaw = CycleManager::HeadingYaw(npc.cycle->moveTargetPos);
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            if (drive)
                cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
        } else if (drive) {
            motion->playbackRate = 0.0f;
        }
        return true;
    }
    if (stepped) {
        if (!turnInPlace) {
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            if (drive && motion->currentAnimId != npc.cycle->moveConfig->motionParamIds[0])
                cycleRequestAnim(npc, npc.cycle->moveConfig->motionParamIds[0]);
            return result;
        }
        if (drive) {
            npc.cycle->desiredPathYaw = CycleManager::HeadingYaw(npc.cycle->moveTargetPos);
            npc.cycle->steeringCommand = CycleSteerCmd_YawCorrect;
        }
        return true;
    }
    if (static_cast<std::uint8_t>(npc.cycle->steeringState - 1u) > 1u)
        return result;
    npc.cycle->pos.x = step * npc.cycle->moveTargetPos.x + npc.cycle->pos.x;
    npc.cycle->pos.z = step * npc.cycle->moveTargetPos.z + npc.cycle->pos.z;
    npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
    npc.cycle->desiredPathYaw = CycleManager::HeadingYaw(npc.cycle->moveTargetPos);
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
    if (!drive)
        return result;
    motion->playbackRate = npc.cycle->pathMoveSpeed;
    if (motion->currentAnimId != npc.cycle->activeAnimId) {
        cyclSubmitMoveRequest(npc);
        return result;
    }
    if (!CycleManager::CommitPathYaw(npc))
        npc.cycle->steeringCommand = turnInPlace ? CycleSteerCmd_YawCorrect : CycleSteerCmd_ToggleReversePath;
    return result;
}



bool InitTimedPathMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive) {
    uint16_t anim = 0;
    uint32_t count = 0;
    uint32_t offset = 0;
    if (!ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 4u, anim) || !ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 8u, count) ||
        !ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 12u, offset))
        return false;
    npc.cycle->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(offset));
    npc.cycle->pathPointCount = static_cast<uint16_t>(count);
    npc.cycle->pathPointIndex = 0;
    npc.cycle->activeAnimId = anim;
    npc.cycle->selectedMotionToken = npc.cycle->forcedMotionToken ? npc.cycle->forcedMotionToken : CycleManager::SelectRandomMotionToken(npc);
    npc.cycle->pathSegmentProgress = 0.0f;
    npc.cycle->animMoveSpeedScale = steeringActive ? getFloatValueFromLUT_Wrapper(npc.cycle->mot, npc.cycle->activeAnimId) : 0.0f;
    if (npc.cycle->animMoveSpeedScale == 0.0f)
        npc.cycle->animMoveSpeedScale = 0.041666668f;
    npc.cycle->pathMoveSpeed = npc.cycle->moveConfig->motionRadiusTimingScalar;
    npc.cycle->baseMoveSpeed = npc.cycle->moveConfig->motionRadiusTimingScalar;
    const Vector3 first = npc.cycle->pathPoints[0];
    const float distance = CycleManager::Distance(npc.cycle->pos, first);
    if (npc.cycle->snapToPathStart || distance > 16.0f || 0.30000001f > distance) {
        npc.cycle->pathPointIndex = 1;
        npc.cycle->pos = first;
        if (npc.cycle->pathPointCount <= 1) {
            npc.cycle->desiredPathYaw = 0;
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        } else {
            const Vector3 &next = npc.cycle->pathPoints[1];
            npc.cycle->moveTargetPos = {npc.cycle->pos.x - next.x, npc.cycle->pos.y - next.y, npc.cycle->pos.z - next.z};
            npc.cycle->pathSegmentDistanceOrWork = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
            npc.cycle->desiredPathYaw = static_cast<int16_t>(atan2_fixed16(npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z));
        }
    } else {
        npc.cycle->pathPointIndex = distance <= 0.5f ? 1 : 0;
        const Vector3 &target = npc.cycle->pathPoints[static_cast<size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->moveTargetPos = {npc.cycle->pos.x - target.x, npc.cycle->pos.y - target.y, npc.cycle->pos.z - target.z};
        npc.cycle->pathSegmentDistanceOrWork = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(atan2_fixed16(npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z));
        const MotionInformationWork *const motion = npc.cycle->mot;
        if (npc.cycle->obj && motion && npc.cycle->motnSteeringActive && !charaHasBlockingTPMRequest(npc.cycle->obj) &&
            motion->currentAnimId != npc.cycle->activeAnimId)
            cyclSubmitMoveRequest(npc);
    }
    CycleManager::ResetPathSteeringState(npc);
    return true;
}


bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t recordOffset, bool steeringActive) {
    uint32_t count = 0;
    uint32_t offset = 0;
    if (!ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 4u, count) || !ReadValue(g_cycleManagerRuntime.bytes, recordOffset + 8u, offset))
        return false;
    return CycleManager::InitPathFollowMotion(npc, offset, count, steeringActive);
}

bool InitPathFollowMotion(CycleManager::NpcRecord &npc, uint32_t offset, uint32_t count, bool steeringActive) {
    npc.cycle->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(offset));
    npc.cycle->pathPointCount = static_cast<uint16_t>(count);
    npc.cycle->pathPointIndex = 0;
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
    npc.cycle->selectedMotionToken = npc.cycle->forcedMotionToken ? npc.cycle->forcedMotionToken : CycleManager::SelectRandomMotionToken(npc);
    npc.cycle->pathSegmentProgress = 0.0f;
    npc.cycle->animMoveSpeedScale = steeringActive ? getFloatValueFromLUT_Wrapper(npc.cycle->mot, npc.cycle->activeAnimId) : 0.0f;
    if (npc.cycle->animMoveSpeedScale == 0.0f)
        npc.cycle->animMoveSpeedScale = 0.041666668f;
    npc.cycle->pathMoveSpeed = npc.cycle->moveConfig->motionRadiusTimingScalar;
    npc.cycle->baseMoveSpeed = npc.cycle->moveConfig->motionRadiusTimingScalar;
    const Vector3 first = npc.cycle->pathPoints[0];
    const float distance = CycleManager::Distance(npc.cycle->pos, first);
    if (npc.cycle->snapToPathStart || distance > 16.0f || 0.30000001f > distance) {
        npc.cycle->pathPointIndex = 1;
        npc.cycle->pos = first;
        npc.cycle->moveTargetPos = npc.cycle->pathPoints[1];
        npc.cycle->desiredPathYaw =
            static_cast<int16_t>(atan2_fixed16(npc.cycle->pos.x - npc.cycle->moveTargetPos.x, npc.cycle->pos.z - npc.cycle->moveTargetPos.z));
    } else {
        npc.cycle->pathPointIndex = distance <= 0.5f ? 1 : 0;
        npc.cycle->moveTargetPos = npc.cycle->pathPoints[static_cast<size_t>(npc.cycle->pathPointIndex)];
        npc.cycle->desiredPathYaw =
            static_cast<int16_t>(atan2_fixed16(npc.cycle->pos.x - npc.cycle->moveTargetPos.x, npc.cycle->pos.z - npc.cycle->moveTargetPos.z));
        const MotionInformationWork *const motion = npc.cycle->mot;
        if (npc.cycle->obj && motion && npc.cycle->motnSteeringActive && !charaHasBlockingTPMRequest(npc.cycle->obj) &&
            motion->currentAnimId != npc.cycle->activeAnimId)
            cyclSubmitMoveRequest(npc);
    }
    CycleManager::ResetPathSteeringState(npc);
    return true;
}
} // namespace CycleManager

} // namespace shenmue::s1
