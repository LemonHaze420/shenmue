#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "s2/npc_package_internal.hpp"

namespace shenmue::s2 {
using namespace npc_package_detail;

namespace npc_package_detail {

void SetNpcYaw(NpcRouteLiveEntry &entry, const std::byte *operand) {
    const float yaw = ReadF32(operand);
    WriteNpcLiveField(entry, npcYawOffset, yaw);
    if (entry.node) {
        entry.node->entity.node.rotation[1] = static_cast<std::int32_t>(ReadU32(operand));
    }
}

const std::byte *GetNpcAreaPoint(std::uint16_t point) {
    if (point >= npcSchedule.areaPointCount)
        return nullptr;
    const std::size_t offset = npcSchedule.areaPointTableOffset + static_cast<std::size_t>(point) * 0x44;
    return Contains(npcSchedule.bytes, offset, 0x44) ? npcSchedule.bytes.data() + offset : nullptr;
}

const std::byte *GetNpcRouteNode(std::uint16_t node) {
    if (node >= npcSchedule.routeNodeCount)
        return nullptr;
    const std::size_t offset = npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(node) * 0x4c;
    return Contains(npcSchedule.bytes, offset, 0x4c) ? npcSchedule.bytes.data() + offset : nullptr;
}

bool GetNpcRouteNodePoint(std::uint16_t node, std::uint32_t point, Vector3 &position) {
    const std::byte *const routeNode = GetNpcRouteNode(node);
    if (!routeNode || point >= ReadU32(routeNode + 4))
        return false;
    const std::uint64_t offset = static_cast<std::uint64_t>(npcSchedule.routeNodeTableOffset) + ReadU32(routeNode) + static_cast<std::uint64_t>(point) * 12;
    if (!Contains(npcSchedule.bytes, offset, sizeof(position)))
        return false;
    std::memcpy(&position, npcSchedule.bytes.data() + offset, sizeof(position));
    return true;
}

// @verified - p
std::uint64_t SampleNpcSplineWindow(const Vector3 *points, std::int32_t pointCount, float parameter, Vector3 *position) {
    std::uint32_t valid = 1;
    if (parameter < 0.0f)
        parameter = -parameter - static_cast<float>(static_cast<std::int32_t>(-parameter));
    std::int32_t index = static_cast<std::int32_t>(parameter);
    float fraction = parameter - static_cast<float>(index);
    if (parameter >= 0.0f) {
        if (parameter >= static_cast<float>(pointCount - 3)) {
            fraction = 1.0f;
            index = pointCount - 4;
            valid = 0;
        }
    } else {
        index = 0;
        fraction = 0.0f;
    }
    Matrix4x4 control{.rows = {
                          {points[index].x, points[index + 1].x, points[index + 2].x, points[index + 3].x},
                          {points[index].y, points[index + 1].y, points[index + 2].y, points[index + 3].y},
                          {points[index].z, points[index + 1].z, points[index + 2].z, points[index + 3].z},
                          {1.0f, 1.0f, 1.0f, 1.0f},
                      }};
    Matrix4x4 interpolation{};
    interpolation.rows[0].z = (fraction * fraction) * fraction;
    interpolation.rows[0].y = interpolation.rows[0].z * 6.0f;
    interpolation.rows[0].x = interpolation.rows[0].z * 6.0f;
    interpolation.rows[1].z = fraction * fraction;
    interpolation.rows[1].y = (fraction * fraction) + (fraction * fraction);
    interpolation.rows[2].z = fraction;
    interpolation.rows[3].w = 1.0f;
    Matrix4x4_Push(&interpolation);
    Matrix4x4::MultiplyWithPrePassMatrix(&npcSplineBasis);
    Matrix4x4::MultiplyWithPrePassMatrix(&control);
    Matrix4x4::GetTransformMatrix(&control);
    Matrix4x4::Pop(0);
    position->x = control.rows[0].z + control.rows[0].w;
    position->y = control.rows[1].z + control.rows[1].w;
    position->z = control.rows[2].z + control.rows[2].w;
    return valid;
}

bool SampleNpcRouteNode(std::uint16_t node, float parameter, Vector3 &position) {
    const std::byte *const routeNode = GetNpcRouteNode(node);
    if (!routeNode)
        return false;
    const std::uint32_t pointCount = ReadU32(routeNode + 4);
    if (pointCount < 4)
        return false;
    const std::size_t offset = npcSchedule.routeNodeTableOffset + ReadU32(routeNode);
    if (!Contains(npcSchedule.bytes, offset, pointCount * sizeof(Vector3)))
        return false;
    return SampleNpcSplineWindow(
               reinterpret_cast<const Vector3 *>(npcSchedule.bytes.data() + offset), static_cast<std::int32_t>(pointCount), parameter, &position) != 0;
}

std::int32_t GetNpcRouteNodePointCount(std::int32_t route) {
    const std::byte *const node = npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(route) * 0x4c;
    return static_cast<std::int32_t>(ReadU32(node + 4));
}

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route) {
    const std::byte *const node = npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + static_cast<std::size_t>(route) * 0x4c;
    const std::uint32_t offset = ReadU32(node);
    return offset ? reinterpret_cast<const Vector3 *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset + offset) : nullptr;
}
} // namespace npc_package_detail

std::uint64_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position) {
    return SampleNpcSplineWindow(points, pointCount, static_cast<float>(parameter) * 0.015625f, position);
}
namespace npc_package_detail {

void InitializeNpcRouteResetGroups(NpcRouteResetGroupHeader *groups) {
    npcRouteResetGroup = groups;
    for (NpcRouteResetGroupHeader *group = groups; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name != npcLoadedArea.name)
                continue;
            entry->nodeOffset = 0;
            const std::int32_t pointCount = GetNpcRouteNodePointCount(entry->routeNode);
            const Vector3 *const points = GetNpcRouteNodeSplinePoints(entry->routeNode);
            InterpolateNpcSplineNormalized(points, pointCount, entry->parameter, &entry->position);
            entry->previousPosition = entry->position;
            entry->parameter = static_cast<std::int32_t>(static_cast<std::uint32_t>(entry->parameter) + entry->parameterStep);
            const std::int32_t nextPointCount = GetNpcRouteNodePointCount(entry->routeNode);
            const Vector3 *const nextPoints = GetNpcRouteNodeSplinePoints(entry->routeNode);
            InterpolateNpcSplineNormalized(nextPoints, nextPointCount, entry->parameter, &entry->position);
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
}

void ResetNpcTransitions() {
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        slot.character = {};
        slot.motionCursor = 0;
        slot.node = nullptr;
    }
    std::int8_t motionCacheIndex = 0;
    NpcTransitionRuntimeSlot *slot = npcTransitionRuntimeSlots.data();
    for (const NpcTransitionEntry &entry : npcTransitionEntries) {
        if (!entry.area.name)
            break;
        if (entry.area.name != g_currentMap.name)
            continue;
        slot->character = entry.character;
        slot->position = entry.position;
        slot->yaw = entry.yaw;
        slot->activationMode = entry.activationMode;
        slot->controllerToggle = entry.controllerToggle;
        slot->motions = entry.motions;
        if (entry.motions)
            slot->motionCacheIndex = motionCacheIndex++;
        ++slot;
    }
}
// @verified - p
void ResetNpcRoutes(NpcRouteResetGroupHeader *groups) {
    npcDrawActive = false;
    InitializeNpcRouteResetGroups(groups);
    ResetNpcTransitions();
}

void MeasureNpcRouteClearance(const Vector3 &start, const Vector3 &end, float &left, float &right, std::uint32_t moveFlags, std::int32_t step) {
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    if (std::abs(dx) < 0.001f && std::abs(dz) < 0.001f)
        return;
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-dx, -dz) - 0x4000);
    const float sine = sin_fixed16(yaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw))));
    std::int32_t leftSteps = static_cast<std::int32_t>(std::abs(left * 10.0f));
    std::int32_t rightSteps = static_cast<std::int32_t>(std::abs(right * 10.0f));
    for (std::int32_t index = step; index <= leftSteps || index <= rightSteps; index += step) {
        const float distance = static_cast<float>(index) * 0.1f;
        const float x = distance * sine;
        const float z = distance * cosine;
        Vector3 resolved;
        if (index <= rightSteps) {
            Vector3 point{start.x - x - npcSchedule.areaOrigin.x, start.y, start.z - z - npcSchedule.areaOrigin.z};
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.30000001f;
            if (RaycastInWorldGrid(resolved, point, 0.40000001f))
                rightSteps = index - step;
        }
        if (index <= leftSteps) {
            Vector3 point{start.x + x - npcSchedule.areaOrigin.x, start.y, start.z + z - npcSchedule.areaOrigin.z};
            FindNpcIntersectionInScene(point, moveFlags);
            point.y += 0.30000001f;
            if (RaycastInWorldGrid(resolved, point, 0.40000001f))
                leftSteps = index - step;
        }
    }
    left = static_cast<float>(leftSteps) / 10.0f;
    right = static_cast<float>(rightSteps) / 10.0f;
}

void RebuildNpcRouteGround() {
    auto *const table = reinterpret_cast<NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.routeNodeCount; ++index) {
        NpcRouteNode &node = table[index];
        auto *const points = reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(table) + node.pointsOffset);
        const std::uint32_t flags = floorFlags | (node.flags & 0xffU);
        for (std::uint32_t point = 0; point < node.pointCount - 2; ++point) {
            Vector3 sampled;
            SampleNpcRouteNode(static_cast<std::uint16_t>(index), static_cast<float>(point), sampled);
            sampled.x -= npcSchedule.areaOrigin.x;
            sampled.z -= npcSchedule.areaOrigin.z;
            FindNpcIntersectionInScene(sampled, flags);
            points[point + 1].y = sampled.y;
        }
        points[0].y = points[1].y;
        points[node.pointCount - 1].y = points[node.pointCount - 2].y;
    }
}

std::int32_t CheckNpcRouteIntersections(
    const Vector3 *points, std::int32_t pointCount, float *bounds, std::int32_t sampleStep, std::uint32_t moveFlags, std::uint8_t *auxiliary) {
    const std::int32_t clearanceStep = sampleStep == 1 ? 1 : 4;
    std::uint32_t presentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    Vector3 previous{};
    SampleNpcSplineWindow(points, pointCount, 0.0f, &previous);
    const std::int32_t segments = pointCount - 3;
    std::memset(auxiliary, 0, segments);
    std::int32_t parameter = 0;
    if (segments <= 0)
        return 1;
    do {
        float left = 3.0f, right = 3.0f;
        std::int32_t steps = 0;
        for (;;) {
            Vector3 next{};
            const std::int32_t nextParameter = parameter + sampleStep;
            if (!SampleNpcSplineWindow(points, pointCount, static_cast<float>(nextParameter) * 0.015625f, &next)) {
                previous = next;
                parameter = nextParameter;
                break;
            }
            const std::int32_t segment = parameter / 64;
            if (!auxiliary[segment]) {
                Vector3 ground = previous;
                ground.x -= npcSchedule.areaOrigin.x;
                ground.z -= npcSchedule.areaOrigin.z;
                FindNpcIntersectionInScene(ground, moveFlags);
                ground.y -= 0.22499999f;
                std::int32_t yaw{};
                if (flddQueryStairOrigin(ground, ground, yaw))
                    auxiliary[segment] = 1;
            }
            MeasureNpcRouteClearance(previous, next, left, right, moveFlags, clearanceStep);
            previous = next;
            const std::uint32_t currentTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
            if (IsLoadingDrawActive()) {
                const float elapsed = static_cast<float>(std::bit_cast<std::int32_t>(presentedTime - currentTime));
                if (static_cast<double>(elapsed) / 12.5 > 15000.0) {
                    DrawLoadingModes();
                    FlushQueuedStripType3Batches();
                    (void)ProfileFramePerformance();
                    presentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
                }
            }
            steps += sampleStep;
            parameter = nextParameter;
            if (steps >= 64)
                break;
        }
        *bounds++ = left;
        *bounds++ = right;
    } while (parameter / 64 < segments);
    return 1;
}

void RebuildNpcRouteLanes(std::int32_t sampleStep) {
    auto *const table = reinterpret_cast<NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.routeNodeCount; ++index) {
        NpcRouteNode &node = table[index];
        const auto *const points = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(table) + node.pointsOffset);
        auto *const bounds = reinterpret_cast<float *>(reinterpret_cast<std::byte *>(table) + node.laneBoundsOffset);
        auto *const auxiliary = reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::byte *>(table) + node.auxiliaryOffset);
        const std::uint32_t flags = floorFlags | (node.flags & 0xffU);
        Vector3 point = points[1];
        point.x -= npcSchedule.areaOrigin.x;
        point.z -= npcSchedule.areaOrigin.z;
        bool hasGround = FindNpcIntersectionInScene(point, flags);
        if (!hasGround) {
            point = points[node.pointCount - 2];
            point.x -= npcSchedule.areaOrigin.x;
            point.z -= npcSchedule.areaOrigin.z;
            hasGround = FindNpcIntersectionInScene(point, flags);
        }
        const std::int32_t segments = static_cast<std::int32_t>(node.pointCount) - 3;
        if (!hasGround) {
            std::fill_n(bounds, segments * 2, 0.40000001f);
            continue;
        }
        CheckNpcRouteIntersections(points, static_cast<std::int32_t>(node.pointCount), bounds, sampleStep, flags, auxiliary);
    }
}

void RebuildNpcAreaPointGround() {
    auto *const table = reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    const std::uint32_t floorFlags = npcSchedule.floor == -1000 ? 0U : static_cast<std::uint32_t>(npcSchedule.floor) << 8;
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        std::byte *const record = npcSchedule.bytes.data() + npcSchedule.areaPointTableOffset + index * 0x44;
        const std::int16_t node = static_cast<std::int16_t>(ReadU16(record + 4));
        if (node < 0)
            continue;
        Vector3 point;
        std::memcpy(&point, record + 0x34, sizeof(point));
        point.x -= npcSchedule.areaOrigin.x;
        point.z -= npcSchedule.areaOrigin.z;
        FindNpcIntersectionInScene(point, floorFlags | (table[node].flags & 0xffU));
        std::memcpy(record + 0x38, &point.y, sizeof(point.y));
    }
}

std::int32_t NpcRouteYaw(float deltaX, float deltaZ) {
    if (std::abs(deltaX) < 0.001f && std::abs(deltaZ) < 0.001f)
        return 0;
    return static_cast<std::int16_t>(FixedAtan2(-deltaX, -deltaZ));
}

void SetNpcYawBits(NpcRouteLiveEntry &entry, std::int32_t yaw) {
    float packed{};
    std::memcpy(&packed, &yaw, sizeof(packed));
    WriteNpcLiveField(entry, npcYawOffset, packed);
}

bool SetNpcAreaPointTangentYaw(NpcRouteLiveEntry &entry, std::uint16_t point) {
    const std::byte *const areaPoint = GetNpcAreaPoint(point);
    if (!areaPoint)
        return false;
    const auto routeNode = static_cast<std::int16_t>(ReadU16(areaPoint + 4));
    if (routeNode < 0)
        return false;
    Vector3 sample;
    SampleNpcSplineWindow(GetNpcRouteNodeSplinePoints(routeNode), GetNpcRouteNodePointCount(routeNode),
        static_cast<float>(static_cast<std::int16_t>(ReadU16(areaPoint + 8))) * 0.015625f, &sample);
    const Vector3 pointPosition{ReadF32(areaPoint + 0x34), ReadF32(areaPoint + 0x38), ReadF32(areaPoint + 0x3c)};
    SetNpcYawBits(entry, NpcRouteYaw(sample.x - pointPosition.x, sample.z - pointPosition.z));
    return true;
}

const std::byte *ResolveNpcRouteData(const NpcRouteStateContext &context, std::int32_t offset) {
    return reinterpret_cast<const std::byte *>(context.stateTable) + offset;
}

bool SearchNpcRouteNodes(
    NpcRouteStateContext &context, NpcRouteSearch &search, std::int16_t state, std::int16_t target, const Vector3 &targetPosition, std::int32_t depth) {
    if (depth == 71)
        return false;
    const std::uint16_t nodeIndex = state & 0x7fff;
    const NpcRouteNode &node = context.stateTable[nodeIndex];
    if (nodeIndex == (target & 0x7fff)) {
        search.nodes[depth] = state;
        search.nodes[depth + 1] = -1;
        search.count = depth + 1;
        search.pointCount = static_cast<std::int32_t>(node.pointCount);
        search.length = ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
        return true;
    }
    npcSchedule.routeVisited[nodeIndex] = 1;
    std::array<std::int16_t, 4> candidates{};
    std::array<std::int8_t, 4> slots{};
    std::array<float, 4> distances{};
    std::int32_t count = 0;
    const std::int32_t first = state < 0 ? 4 : 0;
    for (std::int32_t slot = first; slot < first + 4; ++slot) {
        const std::int16_t next = node.connections[slot];
        const std::uint16_t nextIndex = next & 0x7fff;
        if (next == -1 || nextIndex >= npcSchedule.routeNodeCount)
            continue;
        const std::uint32_t flags = context.stateTable[nextIndex].flags;
        if ((flags & 0x200) || ((flags & 0x100) && !(context.flags & 0x300)) || npcSchedule.routeVisited[nextIndex])
            continue;
        candidates[count] = next;
        slots[count++] = static_cast<std::int8_t>(slot);
    }
    for (std::int32_t index = 0; index < count; ++index) {
        if ((candidates[index] & 0x7fff) != (target & 0x7fff))
            continue;
        if (SearchNpcRouteNodes(context, search, candidates[index], target, targetPosition, depth + 1)) {
            search.nodes[depth] = state;
            search.connections[depth] = slots[index];
            search.pointCount += static_cast<std::int32_t>(node.pointCount) - 2;
            search.length += ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
            return true;
        }
    }
    for (std::int32_t index = 0; index < count; ++index) {
        const NpcRouteNode &next = context.stateTable[candidates[index] & 0x7fff];
        const std::int32_t point = candidates[index] < 0 ? 1 : static_cast<std::int32_t>(next.pointCount) - 2;
        Vector3 position;
        std::memcpy(&position, ResolveNpcRouteData(context, next.pointsOffset) + point * sizeof(Vector3), sizeof(position));
        const float dx = position.x - targetPosition.x;
        const float dz = position.z - targetPosition.z;
        distances[index] = dz * dz + dx * dx;
    }
    std::array<std::int16_t, 4> ordered{};
    std::array<std::int8_t, 4> orderedSlots{};
    for (std::int32_t index = 0; index < count; ++index) {
        std::int32_t rank = 0;
        for (std::int32_t other = 0; other < count; ++other) {
            if (distances[index] > distances[other] || (distances[index] == distances[other] && other < index))
                ++rank;
        }
        ordered[rank] = candidates[index];
        orderedSlots[rank] = slots[index];
    }
    for (std::int32_t index = 0; index < count; ++index) {
        if (npcSchedule.routeVisited[ordered[index] & 0x7fff])
            continue;
        if (!SearchNpcRouteNodes(context, search, ordered[index], target, targetPosition, depth + 1))
            continue;
        search.nodes[depth] = state;
        search.connections[depth] = orderedSlots[index];
        search.pointCount += static_cast<std::int32_t>(node.pointCount) - 2;
        search.length += ReadF32(ResolveNpcRouteData(context, node.lengthsOffset));
        return true;
    }
    return false;
}

bool BuildNpcRouteNodePath(NpcRouteStateContext &context) {
    if ((context.startNode & 0x7fff) == (context.endNode & 0x7fff)) {
        context.routeNodes[0] = context.startNode;
        context.routeNodes[1] = -1;
        context.routeCount = 1;
        context.pointCount = static_cast<std::int16_t>(context.stateTable[context.startNode & 0x7fff].pointCount);
        return true;
    }
    const std::byte *const target = GetNpcAreaPoint(context.owner->pendingAreaPoint);
    Vector3 targetPosition;
    std::memcpy(&targetPosition, target + 0x34, sizeof(targetPosition));
    NpcRouteSearch forward;
    NpcRouteSearch backward;
    std::fill(npcSchedule.routeVisited.begin(), npcSchedule.routeVisited.end(), 0);
    const bool hasForward = SearchNpcRouteNodes(context, forward, context.startNode, context.endNode, targetPosition, 0);
    std::fill(npcSchedule.routeVisited.begin(), npcSchedule.routeVisited.end(), 0);
    const bool hasBackward = SearchNpcRouteNodes(context, backward, static_cast<std::int16_t>(context.startNode ^ 0x8000), context.endNode, targetPosition, 0);
    if (!hasForward && !hasBackward)
        return false;
    const NpcRouteSearch &selected = hasForward && (!hasBackward || forward.length <= backward.length) ? forward : backward;
    std::memcpy(context.routeNodes.data(), selected.nodes.data(), (selected.count + 1) * sizeof(std::int16_t));
    std::memcpy(context.routeConnections.data(), selected.connections.data(), selected.count + 1);
    context.routeCount = static_cast<std::uint8_t>(selected.count);
    context.pointCount = static_cast<std::int16_t>(selected.pointCount);
    return true;
}

std::uint16_t GetNpcRouteConnectionParameter(const std::byte *areaPoint, std::uint16_t state, std::uint16_t adjacentState, bool target) {
    const bool reversed = (state & 0x8000U) != 0;
    const std::byte *const routeNode = GetNpcRouteNode(state & 0x7fffU);
    if (!areaPoint || !routeNode)
        return 0;
    const std::size_t groupBase = target ? (reversed ? 15 : 10) : (reversed ? 5 : 0);
    const std::size_t connectionBase = target ? (reversed ? 0 : 4) : (reversed ? 4 : 0);
    const std::uint16_t sought = target ? static_cast<std::uint16_t>(adjacentState ^ 0x8000U) : adjacentState;
    std::size_t slot = 0;
    if (adjacentState != 0xffff) {
        do {
            ++slot;
            if (ReadU16(routeNode + 0x0c + (connectionBase + slot - 1) * 2) == sought)
                break;
        } while (slot < 4);
    }
    return ReadU16(areaPoint + 8 + (groupBase + slot) * 2);
}

// @verified - p
std::int32_t SampleNpcRouteWindow(NpcRouteStateContext &context, Vector3 *position, bool maintainWindow) {
    const std::uint16_t parameter = context.parameter;
    float amount = static_cast<float>(parameter) * 0.015625f;
    std::int32_t window = static_cast<std::int32_t>(amount);
    std::int32_t valid = 1;
    if (window > context.pointCount - 4) {
        amount = 0.99998999f;
        window = context.pointCount - 4;
        valid = 0;
    }
    if (maintainWindow && context.sampledWindow == window) {
        SampleNpcSplineWindow(context.spline.data(), 4, -amount, position);
        return valid;
    }
    const std::int16_t *route = context.routeNodes.data();
    if (maintainWindow && parameter >= context.sampledParameter) {
        route += context.sampledRouteIndex;
    } else {
        context.sampledPointBase = 0;
        context.sampledRouteIndex = 0;
    }
    std::int32_t base = context.sampledPointBase;
    std::int32_t length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
    context.sampledParameter = parameter;
    context.sampledWindow = static_cast<std::int16_t>(window);
    std::array<std::int16_t, 4> nodes{};
    std::array<std::int16_t, 4> points{};
    std::int32_t count = 0;
    for (; count < 4; ++count) {
        while (count + window > length + base) {
            if (*++route == -1)
                break;
            base += length;
            if (count < 2) {
                ++context.sampledRouteIndex;
                context.sampledPointBase = static_cast<std::int16_t>(base);
            }
            length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
        }
        if (*route == -1)
            break;
        nodes[count] = *route;
        points[count] = static_cast<std::int16_t>(window + count - base);
    }
    if (count == 2) {
        const NpcRouteNode &node = context.stateTable[nodes[1] & 0x7fff];
        const std::int32_t index = nodes[1] < 0 ? 1 : static_cast<std::int32_t>(node.pointCount) - 2;
        std::memcpy(position, ResolveNpcRouteData(context, node.pointsOffset) + index * sizeof(Vector3), sizeof(*position));
        return 0;
    }
    if (count == 3) {
        nodes[3] = nodes[2];
        points[3] = points[2] + 1;
    }
    for (std::int32_t index = 0; index < 4; ++index) {
        const NpcRouteNode &node = context.stateTable[nodes[index] & 0x7fff];
        const std::int32_t point = nodes[index] < 0 ? static_cast<std::int32_t>(node.pointCount) - points[index] - 1 : points[index];
        std::memcpy(&context.spline[index], ResolveNpcRouteData(context, node.pointsOffset) + point * sizeof(Vector3), sizeof(Vector3));
    }
    SampleNpcSplineWindow(context.spline.data(), 4, -amount, position);
    return valid;
}

void UpdateNpcRouteWindow(NpcRouteStateContext &context, std::int32_t parameter, bool maintainWindow) {
    std::int32_t window = parameter / 64;
    if (window > context.pointCount - 4)
        window = context.pointCount - 4;
    const std::int16_t *route = context.routeNodes.data();
    if (maintainWindow) {
        if (window == context.activeWindow)
            return;

        route += context.activeRouteIndex;
    } else {
        context.crossingPoint = 0;
        context.activeWindow = context.activePointBase = 0;
        context.activeRouteIndex = 0;
        context.activeNode = route[0];
        context.nextNode = route[1];
    }
    std::int32_t base = context.activePointBase;
    std::int32_t length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
    for (std::int32_t point = 0; point < 4; ++point) {
        while (point + window > length + base) {
            if (*++route == -1)
                break;
            base += length;
            if (point < 2) {
                ++context.activeRouteIndex;
                context.activePointBase = static_cast<std::int16_t>(base);
            }
            context.crossingPoint = static_cast<std::uint8_t>(point);
            length = static_cast<std::int32_t>(context.stateTable[*route & 0x7fff].pointCount) - 2;
        }
        if (*route == -1)
            break;
        if (point == 1) {
            context.activeNode = *route;
            context.nextNode = route[1];
            context.activeSegment = static_cast<std::int16_t>(*route < 0 ? length - window + base - 2 : window - base);
        }
    }
    context.activeWindow = static_cast<std::int16_t>(window);
    context.moveFlags = (context.moveFlags & 0x1ff00U) | (context.stateTable[context.activeNode & 0x7fff].flags & 0xffU);
}

float ReadNpcRouteSegmentLength(const NpcRouteStateContext &context) {
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    if (context.crossingPoint == 2)
        return node.connectionLengths[context.routeConnections[context.activeRouteIndex]];

    return ReadF32(ResolveNpcRouteData(context, node.lengthsOffset) + (context.activeSegment + 1) * sizeof(float));
}

std::int32_t ReadNpcEulerYaw(const Vector3 &euler) {
    std::int32_t yaw;
    std::memcpy(&yaw, &euler.y, sizeof(yaw));
    return yaw;
}

void StoreNpcEulerYaw(Vector3 &euler, std::int32_t yaw) {
    std::memcpy(&euler.y, &yaw, sizeof(yaw));
}

Vector3 TransformNpcPoint(const Vector3 &point) {
    const Matrix4x4 &matrix = workTransformMatrix;
    return {point.x * matrix.rows[0].x + point.y * matrix.rows[1].x + point.z * matrix.rows[2].x + matrix.rows[3].x,
        point.x * matrix.rows[0].y + point.y * matrix.rows[1].y + point.z * matrix.rows[2].y + matrix.rows[3].y,
        point.x * matrix.rows[0].z + point.y * matrix.rows[1].z + point.z * matrix.rows[2].z + matrix.rows[3].z};
}

Vector3 NpcPositionInFrame(const Vector3 &point, const Vector3 &origin, std::int32_t yaw) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(yaw), 0, 0);
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    const Vector3 result = TransformNpcPoint(point);
    Matrix4x4::Pop(1);
    return result;
}

Vector3 OffsetNpcRoutePosition(const Vector3 &origin, std::int32_t yaw, float lateral) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::SetViewMatrixFromEuler(yaw, 0, 0);
    Matrix4x4::preTranslate(lateral, 0.0f, 0.0f);
    const Vector3 result{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return result;
}

Vector3 NpcAreaPosition(Vector3 position) {
    position.x += npcSchedule.areaOrigin.x;
    position.z += npcSchedule.areaOrigin.z;
    return position;
}

std::uint16_t SelectNpcLocomotion(const NpcRouteStateContext &context, std::uint32_t selector) {
    constexpr std::array<std::array<std::uint16_t, 15>, 4> motions{
        {{0xf03e, 0xf0ff, 0xf07e, 0xf084, 0xf09e, 0xf0d1, 0xf060, 0xf086, 0xf005, 0xf078, 0xf058, 0xf084, 0xf084, 0xf0a4, 0xf0a6},
            {0xf006, 0xf006, 0xf006, 0xf0e6, 0xf0e6, 0xf0e6, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006, 0xf006},
            {0xf056, 0xf056, 0xf083, 0xf056, 0xf0a3, 0xf0e9, 0xf077, 0xf09d, 0x8024, 0xf07d, 0xf05d, 0x8024, 0x8024, 0xf056, 0xf0f4},
            {0xf040, 0xf040, 0xf080, 0xf040, 0xf0a0, 0xf0d3, 0xf062, 0xf088, 0x8020, 0xf07a, 0xf05a, 0x8020, 0x8020, 0xf040, 0xf0f3}}};
    constexpr std::array<std::uint16_t, 4> defaults{0xf084, 0xf006, 0x8024, 0x8020};
    const std::int32_t rig = GetMotionRigBoneId(context.motionWork);
    return rig >= 0 && rig < 15 ? motions[selector][rig] : defaults[selector];
}

std::uint16_t ComputeNpcRouteStride(NpcRouteStateContext &context) {
    if (context.routeFlags & 0x400000U)
        return 1;

    if (!context.owner->node)
        return 16;

    context.segmentLength = ReadNpcRouteSegmentLength(context);
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(std::ceil(context.motionStep * 64.0f / context.segmentLength)));
}

void InitializeNpcLocomotionProfile(NpcRouteStateContext &context) {
    if (context.flags & 0x40U)
        return;

    const bool running = (context.flags & 0x10U) != 0;
    context.maximumYawStep = running ? 1820 : 546;
    context.motionStep = running ? 0.1f : 0.039999999f;
    if (context.owner->node) {
        const std::uint16_t motion = SelectNpcLocomotion(context, running ? 1 : 0);
        const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
        if (sequence) {
            Vector3 displacement{};
            SampleNativeMotionRoot(motion, GetCharacterBoneId(context.owner->node), -1.0f, displacement);
            if (-displacement.z > 0.00000011920929f)
                context.motionStep = -(displacement.z / static_cast<float>(sequence->frameCount));
        }
        context.flags |= 0x40U;
    }
    context.motionStep *= context.motionScale;
    if (context.flags & 2U)
        context.stride = ComputeNpcRouteStride(context);
}

void SetNpcLocomotionScale(NpcRouteStateContext &context, float scale) {
    constexpr std::array<std::uint8_t, 16> scales{11, 11, 11, 10, 10, 11, 12, 12, 12, 12, 11, 10, 10, 11, 11, 10};
    float multiplier = 1.0f;
    if (context.owner->node) {
        const std::int32_t rig = GetMotionRigBoneId(context.motionWork);
        if (rig >= 0 && rig <= 15)
            multiplier = static_cast<float>(scales[rig]) / 10.0f;
    }
    context.motionStep /= context.motionScale;
    const float requested = scale * multiplier;
    context.motionScale = requested <= 3.99f ? std::fmax(0.5f, requested) : 3.99f;
    context.motionStep *= context.motionScale;
    if (context.flags & 2U)
        context.stride = ComputeNpcRouteStride(context);
    if (context.owner->node)
        SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
}

void InterpolateNpcRouteLanes(NpcRouteStateContext &context, bool advanced) {
    float fraction = static_cast<float>(context.parameter & 63U) * 0.015625f;
    if (advanced && std::abs(fraction) < 0.00000011920929f && context.parameter)
        fraction = 1.0f;

    const float highDelta = context.nextLaneHigh - context.segmentLaneHigh;
    context.interpolatedLaneHigh = -context.segmentLength > highDelta ? context.nextLaneHigh :
        highDelta >= 0.0f                                             ? context.segmentLaneHigh :
                                                                        highDelta * fraction + context.segmentLaneHigh;
    const float lowDelta = context.nextLaneLow - context.segmentLaneLow;
    context.interpolatedLaneLow = lowDelta > context.segmentLength ? context.nextLaneLow :
        lowDelta <= 0.0f                                           ? context.segmentLaneLow :
                                                                     lowDelta * fraction + context.segmentLaneLow;
    context.laneHigh = context.interpolatedLaneHigh - 0.1f;
    context.laneLow = context.interpolatedLaneLow + 0.1f;
    if (context.laneLow > context.laneHigh) {
        if (context.laneHigh < 0.0f && context.laneLow < 0.0f)
            context.laneHigh = context.laneLow;
        else if (context.laneLow > 0.0f && context.laneHigh > 0.0f)
            context.laneLow = context.laneHigh;
        else
            context.laneHigh = context.laneLow = 0.0f;
    }
    if (context.laneOffset > context.laneHigh)
        context.laneOffset = context.laneHigh;
    else if (context.laneLow > context.laneOffset)
        context.laneOffset = context.laneLow;
}

void LoadNpcRouteLanes(NpcRouteStateContext &context) {
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    const bool reverse = context.activeNode < 0;
    std::int32_t segment = context.crossingPoint == 2 ? (reverse ? 0 : static_cast<std::int32_t>(node.pointCount) - 4) : context.activeSegment;
    context.stairFlags = static_cast<std::uint8_t>(ResolveNpcRouteData(context, node.auxiliaryOffset)[segment]);
    const std::byte *lanes = ResolveNpcRouteData(context, node.laneBoundsOffset) + segment * 8;
    context.segmentLaneHigh = ReadF32(lanes + (reverse ? 0 : 4));
    context.segmentLaneLow = -ReadF32(lanes + (reverse ? 4 : 0));
    const NpcRouteNode *next = &node;
    segment += reverse ? -1 : 1;
    bool nextReverse = reverse;
    if (context.crossingPoint == 2 || context.crossingPoint == 3) {
        next = &context.stateTable[context.nextNode & 0x7fff];
        nextReverse = context.nextNode < 0;
        segment = nextReverse ? static_cast<std::int32_t>(next->pointCount) - 4 : 0;
    } else if (segment < 0 || segment >= static_cast<std::int32_t>(node.pointCount) - 3) {
        context.nextLaneHigh = context.segmentLaneHigh;
        context.nextLaneLow = context.segmentLaneLow;
        InterpolateNpcRouteLanes(context, false);
        return;
    }
    context.stairFlags |= static_cast<std::uint8_t>(ResolveNpcRouteData(context, next->auxiliaryOffset)[segment]);
    lanes = ResolveNpcRouteData(context, next->laneBoundsOffset) + segment * 8;
    context.nextLaneHigh = ReadF32(lanes + (nextReverse ? 0 : 4));
    context.nextLaneLow = -ReadF32(lanes + (nextReverse ? 4 : 0));
    InterpolateNpcRouteLanes(context, false);
}

bool FitNpcRouteLane(NpcRouteStateContext &context, float clearance) {
    const Vector3 offset = NpcPositionInFrame(context.position, context.localPosition, context.tangentYaw);
    const float lateral = -offset.x;
    bool clipped = false;
    float result;
    if (offset.x <= 0.0f) {
        result = context.laneHigh - clearance;
        if (lateral <= result)
            result = lateral;
        else
            clipped = true;

        if (context.laneLow > result) {
            context.laneOffset = context.laneLow;
            return true;
        }
    } else {
        result = context.laneLow + clearance;
        if (result <= lateral)
            result = lateral;
        else
            clipped = true;
        if (result > context.laneHigh) {
            result = context.laneHigh;
            clipped = true;
        }
    }
    context.laneOffset = result;
    return clipped;
}

bool AdvanceUnstreamedNpc(NpcRouteStateContext &context) {
    if ((context.flags & 0x41000U) == 0x41000U)
        return false;
    float step = context.motionStep;
    if (context.routeFlags & 0x10000U)
        step *= 8.0f;
    if (context.flags & 0x80000U) {
        context.flags &= ~0x80000U;
        context.position = context.savedSplinePosition;
        context.moveTarget = context.savedMoveTarget;
    }
    const float x = context.moveTarget.x - context.localPosition.x;
    const float z = context.moveTarget.z - context.localPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    if (distance > 100.0f)
        context.localPosition = context.moveTarget;
    const bool arrived = step > distance;
    if (arrived)
        context.localPosition = context.moveTarget;
    else if (std::abs(distance) > 0.00000011920929f) {
        context.localPosition.x += x * step / distance;
        context.localPosition.y = context.moveTarget.y;
        context.localPosition.z += z * step / distance;
    }
    if (context.routeFlags & 0x800U) {
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
    }
    return arrived;
}

bool RequestNpcRouteTurn(NpcRouteStateContext &context, const Vector3 *target, std::uint16_t yaw, std::int32_t copyType) {
    SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
    CopyMotionTransition(context.motionWork, target, yaw, 5, copyType);
    const bool result = CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
    return (context.flags & 0x10U) ? CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0) : result;
}

void ApplyNpcRouteEuler(NpcRouteStateContext &context) {
    Vector3i rotation;
    std::memcpy(&rotation, &context.savedEuler, sizeof(rotation));
    SetCharacterWorldRotation(context.owner->node, rotation);
}

void RestoreNpcGroundControl(NpcRouteStateContext &context) {
    if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node)) {
        std::uint32_t flags = control->flags;
        if (npcSchedule.floor == -1000 || !(context.owner->packedMoveFlags & 0xff00U))
            flags |= 2U;
        flags = (flags & ~0x804U) | 4U;
        if (context.owner->flags & 0x8000000U)
            flags |= 0x800U;
        control->flags = flags;
    }
    context.flags &= ~0x200000U;
}

std::int32_t DetectNpcRouteStair(NpcRouteStateContext &context) {
    if (!(context.routeFlags & 4U) || !context.owner->node)
        return 0;

    if (context.avoidancePhase)
        return 2;

    if (!context.stairFlags) {
        if (context.flags & 0x200000U)
            RestoreNpcGroundControl(context);
        context.flags &= 0xffbeffffU;
        return 0;
    }

    if (context.routeFlags & 0x4040U) {
        context.localPosition = NpcRouteAreaToScene(context.localPosition);
        FindNpcIntersectionInScene(context.localPosition, context.moveFlags);
        context.scenePosition = context.localPosition;
        context.localPosition = NpcAreaPosition(context.localPosition);
        context.routeFlags &= ~0x4040U;
    }

    Vector3 position = NpcRouteAreaToScene(context.localPosition);
    position.y -= 0.22499999f;
    Vector3 origin;
    std::int32_t yaw;
    const std::int32_t stairs = flddQueryStairOrigin(position, origin, yaw);
    if (!stairs) {
        if (context.flags & 0x200000U)
            RestoreNpcGroundControl(context);

        context.flags &= 0xffbeffffU;
        return 0;
    }

    const std::int32_t stairFrameYaw = yaw;
    if (context.flags & 0x10000U)
        return 0;

    position = NpcPositionInFrame(NpcRouteAreaToScene(context.localPosition), origin, yaw);
    const std::int16_t difference = static_cast<std::int16_t>(context.tangentYaw - yaw);
    const std::int32_t magnitude = difference < 0 ? -static_cast<std::int32_t>(difference) : difference;
    context.stairCount = static_cast<std::uint16_t>(stairs);
    context.stairTarget.x = context.stairTarget.y = 0.0f;
    if (magnitude >= 0x4000) {
        if (position.z > 0.0f) {
            context.flags |= 0x10000U;
            return 0;
        }

        context.stairTarget.z = 0.1f;
        std::int32_t remaining = static_cast<std::int8_t>(1 - static_cast<std::int32_t>(position.z / 0.30000001f));
        if (remaining > stairs)
            remaining = stairs;

        context.stairRemaining = static_cast<std::int8_t>(remaining);
        yaw += 0x8000;
        position.y = static_cast<float>(context.stairRemaining) * 0.22499999f;
        position.z = 0.1f - static_cast<float>(context.stairRemaining) * 0.30000001f;
        context.motion = SelectNpcLocomotion(context, 3);
        context.flags &= ~0x800U;
    } else {
        std::int32_t entered = static_cast<std::int32_t>(position.z / -0.30000001f);
        context.stairRemaining = static_cast<std::int8_t>(entered);
        if (entered & 0x80)
            context.stairRemaining = 0;
        if (position.z < 0.0f)
            ++context.stairRemaining;

        entered = context.stairRemaining;
        if (entered >= stairs) {
            context.flags |= 0x10000U;
            return 0;
        }
        context.stairRemaining = static_cast<std::int8_t>(stairs - entered);
        position.y = static_cast<float>(entered) * 0.22499999f;
        position.z = static_cast<float>(entered - 1) * -0.30000001f - 0.1f;
        context.stairTarget.z = static_cast<float>(stairs - 1) * -0.30000001f - 0.1f;
        context.motion = SelectNpcLocomotion(context, 2);
        context.flags |= 0x800U;
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::SetViewMatrixFromEuler(stairFrameYaw, 0, 0);
    context.doorPosition = TransformNpcPoint(position);
    context.stairTarget = TransformNpcPoint(context.stairTarget);
    Matrix4x4::Pop(1);
    context.stairYaw = static_cast<std::int16_t>(yaw);
    StoreNpcEulerYaw(context.savedEuler, context.stairYaw);
    context.flags |= 0x400U;
    return 1;
}

std::int32_t TickNpcRouteStair(NpcRouteStateContext &context) {
    Character *const character = context.owner->node;
    if (!(context.routeFlags & 4U) || !character)
        return 0;

    switch (context.avoidancePhase) {
    case 0:
        if (context.flags & 0x400U) {
            if ((context.flags & 0x1000U) || (context.routeFlags & 0x800U)) {
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.motion = GetMotionAnimationId(context.motionWork, MotionRegion::LowerBody);
                context.avoidancePhase = 3;
            } else {
                context.stairTarget = NpcAreaPosition(context.stairTarget);
                const MotnSequence *const sequence = FindLoadedMotionSequence(context.motion);
                context.stairMotionFrames = sequence ? static_cast<float>(sequence->frameCount) : 0.0f;
                context.avoidancePhase = context.stairRemaining == 1 ? 2 : 1;
            }
            mtwkRequestMotion(context.motionWork, context.motion, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(context.motionWork, 1);
            SetMotionFrameTime(context.motionWork, 10, MotionRegion::LowerBody);
            if (context.flags & 0x1000U)
                SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            else {
                SetMotionSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
                SetMotionRegionFlag(context.motionWork, 69, HasMotionDependentTrack(context.motionWork) ? 0 : 2, MotionRegion::LowerBody);
            }

            shenApplyTranslationToAll(&character->entity.node, context.doorPosition);
            ApplyNpcRouteEuler(context);
            if (CharacterControlModel *const control = FindCharacterControlModel(character))
                control->flags = (control->flags & ~0x806U) | 0x800U;

            context.flags |= 0x200000U;
        }
        return context.avoidancePhase;
    case 1:
        if (SetMotionRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody)) {
            Vector3 position = NpcRouteAreaToScene(context.localPosition);
            position.y += (context.flags & 0x800U) ? 0.44999999f : -0.44999999f;
            const std::int32_t remaining = context.stairRemaining - 2;
            context.stairRemaining = static_cast<std::int8_t>(remaining < 0 ? 0 : remaining);
            if (context.stairRemaining == 1)
                context.avoidancePhase = 2;
            else if (!context.stairRemaining)
                context.avoidancePhase = 4;

            shenApplyTranslationToAll(&character->entity.node, position);
            if ((context.routeFlags & 0x800U) && context.stairRemaining) {
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.avoidancePhase = 3;
                CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            }
        } else if (context.routeFlags & 0x800U) {
            const float middleDistance = std::abs(GetMotionPoseFrame(context.motionWork, MotionRegion::LowerBody) - context.stairMotionFrames * 0.5f);
            const bool nearMiddle = context.motionScale * 1.1f > middleDistance;
            if (nearMiddle || SetMotionRegionFlag(context.motionWork, 68, 1, MotionRegion::LowerBody)) {
                if (nearMiddle) {
                    Vector3 position = NpcRouteAreaToScene(context.localPosition);
                    position.y += (context.flags & 0x800U) ? 0.22499999f : -0.22499999f;
                    shenApplyTranslationToAll(&character->entity.node, position);
                }
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
                context.avoidancePhase = 3;
                CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            }
        }
        return context.avoidancePhase;
    case 2: {
        const Vector3 local = NpcPositionInFrame(context.localPosition, context.stairTarget, context.stairYaw);
        if (local.z > 0.0f)
            return context.avoidancePhase;
        Vector3 position = NpcRouteAreaToScene(context.localPosition);
        context.stairRemaining = 0;
        position.y += (context.flags & 0x800U) ? 0.22499999f : -0.22499999f;
        shenApplyTranslationToAll(&character->entity.node, position);
        break;
    }
    case 3:
        if (context.routeFlags & 0x800U) {
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
        }
        if (!(context.flags & 0x101000U))
            context.avoidancePhase = 0;
        return context.avoidancePhase;
    case 4:
        break;
    default:
        return context.avoidancePhase;
    }
    if (context.laneLow > context.lateralPosition || context.lateralPosition > context.laneHigh)
        context.routeFlags |= 0x800000U;

    if (context.routeFlags & 0x800U) {
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
        CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
    } else {
        SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
        context.motion = SelectNpcLocomotion(context, (context.flags & 0x10U) ? 1 : 0);
        mtwkRequestMotion(context.motionWork, context.motion, MotionRegion::LowerBody);
        SetMotionRegionFlag(context.motionWork, 69, HasMotionDependentTrack(context.motionWork) ? 2 : 0, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(context.motionWork, 1);
        SetMotionFrameTime(context.motionWork, 6, MotionRegion::LowerBody);
    }
    context.flags = (context.flags & ~0x400U) | 0x10000U;
    context.avoidancePhase = 0;
    return 0;
}

bool RequestNpcRouteMotion(NpcRouteStateContext &context, bool explicitRequest) {
    Character *const character = context.owner->node;
    if (!character || !(context.flags & 3U))
        return false;
    SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
    if (context.flags & 0x1000U) {
        CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
        return true;
    }
    switch (context.phase) {
    case 5: {
        CharacterControlModel *const control = FindCharacterControlModel(character);
        std::uint32_t flags = control ? control->flags | 0x800U : 0x800U;
        if (context.moveState == 1 || (!context.moveState && explicitRequest)) {
            CopyMotionTransition(context.motionWork, &context.sceneTarget, static_cast<std::uint16_t>(context.targetYaw), 0, 0);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
            SetMotionTimeRelated(context.motionWork, 1, 5, context.doorMotion);
            SetMotionRegionEvalTime(context.motionWork, 14.0f, MotionRegion::LowerBody);
            flags &= ~4U;
        } else if (context.moveState == 3) {
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.targetYaw), 13, 0);
            SetMotionTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (control)
            control->flags = flags;
        return false;
    }
    case 6:
        if (context.moveState != 1 && (context.moveState || !explicitRequest))
            return false;
        CopyMotionTransition(context.motionWork, &context.sceneTarget, static_cast<std::uint16_t>(context.targetYaw), 5, 0);
        CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        if (context.flags & 0x10U)
            CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0);
        return false;
    case 7: {
        const std::int32_t stair = DetectNpcRouteStair(context);
        if (!stair) {
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 5, 2);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
            if (context.flags & 0x10U)
                CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0);
        } else if (stair == 1 && !explicitRequest) {
            UpdateNpcRouteAvoidance(context);
            TickNpcRouteStair(context);
        }
        return !explicitRequest;
    }
    case 8: {
        if (context.moveState != 1 && (context.moveState || !explicitRequest)) {
            if (context.moveState == 3) {
                CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.targetYaw), 13, 0);
                SetMotionTimeRelated(context.motionWork, 1, 5, 0);
            }
            return false;
        }
        Vector3 target = context.sceneTarget;
        if (context.otherName.name) {
            context.doorMotion =
                static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), character, &context.scenePosition));
            if (context.otherName.name != static_cast<std::uint32_t>(GetSelectedNpcDoorId())) {
                context.doorMotion = 0;
                context.otherName = {};
            } else if (context.doorMotion) {
                Vector3 track{};
                SampleNativeMotionRoot(context.doorMotion, GetCharacterBoneId(character), 15.0f, track);
                Matrix4x4_Push(&transformMatrix_prePass);
                Matrix4x4::SetViewMatrixFromEuler(context.targetYaw, 0, 0);
                track = TransformNpcPoint(track);
                Matrix4x4::Pop(1);
                target.x += track.x;
                target.z += track.z;
            }
        }
        CopyMotionTransition(context.motionWork, &target, static_cast<std::uint16_t>(context.targetYaw), 6, 0);
        CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        if (context.flags & 0x10U)
            CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0);
        if (context.otherName.name && context.doorMotion) {
            SetMotionTimeRelated(context.motionWork, 1, 5, context.doorMotion);
            SetMotionRegionEvalTime(context.motionWork, 14.0f, MotionRegion::LowerBody);
        }
        return false;
    }
    default:
        return false;
    }
}

void InitializeNpcRouteRequest(NpcRouteStateContext &context, std::uint16_t current, std::uint16_t target) {
    context.phase = 0;
    context.flags = (context.flags & 0x20002U) | 1U;
    context.previousSegment = -10;
    context.otherName = {};
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.doorWaitFrames = -1;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.routeFlags = (context.owner->flags & 0x400000U) ? 0x100206U : 0x206U;
    context.scriptOwner = context.owner->name;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.currentAreaPoint = current;
    context.pendingAreaPoint = target;
    context.motionScale = 1.0f;
    context.motionWork = context.owner->motionWork;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node)) {
        std::uint32_t flags = (context.owner->flags & 0x8000000U) ? control->flags | 0x800U : control->flags & ~0x800U;
        if (npcSchedule.floor == -1000 || !(context.owner->packedMoveFlags & 0xff00U))
            flags |= 2U;
        SetCharacterControlModelFlags(context.owner->node, flags | 4U);
    }
}

std::int32_t AdvanceNpcRouteByDistance(const NpcRouteStateContext &context, float distance) {
    const std::int32_t direction = distance >= 0.0f ? 1 : -1;
    if (distance < 0.0f)
        distance = -distance;
    std::int32_t routeIndex = context.activeRouteIndex;
    std::int32_t segment = context.activeSegment;
    std::int32_t available = direction == 1 ? 64 - (context.parameter & 63) : context.parameter & 63;
    std::int32_t advanced = 0;
    bool connection = context.crossingPoint == 2;
    std::int32_t count = static_cast<std::int32_t>(context.stateTable[context.routeNodes[routeIndex] & 0x7fff].pointCount) - 3;
    for (;;) {
        const NpcRouteNode &node = context.stateTable[context.routeNodes[routeIndex] & 0x7fff];
        const float length = connection ? node.connectionLengths[context.routeConnections[routeIndex]] :
                                          ReadF32(ResolveNpcRouteData(context, node.lengthsOffset) + (segment + 1) * sizeof(float));
        const float remaining = distance - static_cast<float>(available) * length * 0.015625f;
        if (remaining <= 0.0f) {
            advanced += static_cast<std::int32_t>(distance * 64.0f / length);
            break;
        }
        advanced += available;
        segment += context.routeNodes[routeIndex] < 0 ? -direction : direction;
        connection = false;
        available = 64;
        distance = remaining;
        if (segment > count || segment < -1) {
            routeIndex += direction;
            if (routeIndex < 0 || routeIndex >= context.routeCount)
                break;
            count = static_cast<std::int32_t>(context.stateTable[context.routeNodes[routeIndex] & 0x7fff].pointCount) - 3;
            segment = (context.routeNodes[routeIndex] < 0) == (direction == 1) ? count - 1 : 0;
        } else if (segment == count || segment == -1) {
            connection = true;
        }
    }
    const std::int32_t result = context.parameter + direction * advanced;
    return result > context.highParameter ? context.highParameter : result < context.lowParameter ? context.lowParameter : result;
}

bool MeasureNpcAvoidanceCandidate(Vector3 &center, float radius, std::int32_t kind, float &distance) {
    center = TransformNpcPoint(center);
    if (center.z - radius - npcAvoidanceRadius > 0.0f)
        return false;
    const float lateral = center.x + npcAvoidanceLateral;
    if (npcAvoidanceLow - npcAvoidanceRadius > lateral + radius || lateral - radius > npcAvoidanceHigh + npcAvoidanceRadius)
        return false;
    const float forward = -(center.z + radius + npcAvoidanceRadius);
    if ((kind == 2 && forward < -1.0f) || forward > npcAvoidanceDistance)
        return false;
    if (forward < 0.0f) {
        const float combined = npcAvoidanceRadius + radius;
        if (std::abs(center.x) > combined || center.z > 0.0f) {
            if (center.x >= 0.0f) {
                const float high = lateral - combined;
                if (npcAvoidanceHigh > high)
                    npcAvoidanceHigh = high;
            } else {
                const float low = lateral + combined;
                if (low > npcAvoidanceLow)
                    npcAvoidanceLow = low;
            }
            if (npcAvoidanceLow > npcAvoidanceHigh)
                npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
            return false;
        }
    }
    distance = forward;
    return true;
}

void InsertNpcAvoidanceCandidate(std::uint16_t kind, NpcRouteLiveEntry *source, const Vector3 &local, const Vector3 &position, float radius, float distance) {
    NpcAvoidanceCandidate *insertion = npcAvoidanceFirst;
    while (insertion && distance > insertion->distance)
        insertion = insertion->next;
    if (!insertion)
        return;
    NpcAvoidanceCandidate *const candidate = npcAvoidanceLast;
    candidate->distance = distance;
    candidate->source = source;
    candidate->kind = kind;
    candidate->localX = local.x;
    candidate->localZ = local.z;
    candidate->position = position;
    candidate->lateralRadius = candidate->forwardRadius = radius;
    if (insertion == candidate)
        return;
    npcAvoidanceLast = candidate->previous;
    npcAvoidanceLast->next = nullptr;
    candidate->previous = insertion->previous;
    candidate->next = insertion;
    if (insertion->previous)
        insertion->previous->next = candidate;
    else
        npcAvoidanceFirst = candidate;
    insertion->previous = candidate;
}

void UpdateNpcStairAvoidance(NpcRouteStateContext &context, const NpcAvoidanceCandidate &candidate) {
    std::int32_t response = 0;
    if (candidate.kind != 3) {
        if (candidate.distance < 0.5f) {
            context.flags |= 0x400000U;
            response = 1;
        }
    } else {
        const NpcRouteLiveEntry &other = *candidate.source;
        const NpcRouteStateContext &route = other.stateContext;
        if (!(route.flags & 0x400U)) {
            if (candidate.distance < 0.5f)
                response = 2;
        } else {
            const std::int16_t delta = static_cast<std::int16_t>(route.stairYaw - context.stairYaw);
            const std::int32_t difference = delta < 0 ? -static_cast<std::int32_t>(delta) : delta;
            if (difference < 10) {
                if (std::abs(context.stairRemaining - route.stairRemaining) <= 4)
                    response = 2;
            } else if (difference > 32758) {
                if ((other.flags & 0x10000U) || (route.flags & 0x400000U) || (route.routeFlags & 0x10U)) {
                    context.flags |= 0x400000U;
                    response = 3;
                } else if (context.flags & 0x400000U)
                    response = 1;
                else if (context.stairRemaining >= route.stairRemaining)
                    response = 3;
                else if (std::abs(context.stairCount - context.stairRemaining - route.stairRemaining) <= 4)
                    response = 2;
            }
        }
    }
    if (!response) {
        context.flags &= 0xffafffffU;
        context.avoidanceResult = 0;
        SetNpcRouteMotionState(context, 3);
        context.avoidanceWaitFrames = 0;
        return;
    }
    if ((context.owner->flags & 0x10000U) || (context.routeFlags & 0x10U))
        response = 2;
    if (response == 2)
        context.avoidanceWaitFrames = 0;
    else if (response == 3)
        context.avoidanceWaitFrames = 90;
    context.avoidanceResult = 1;
    if (!(context.flags & 0x1000U)) {
        context.routeFlags |= 0x800U;
        context.flags |= 0x100000U;
    }
}

std::uint8_t UpdateNpcRouteAvoidance(NpcRouteStateContext &context) {
    const std::uint8_t previousResult = context.avoidanceResult;
    context.other = nullptr;
    if (previousResult == 4)
        context.field0a1 |= 0x20;
    else
        context.field0a1 &= ~0x20;
    context.avoidanceResult = context.field0a0 = 0;
    for (std::size_t index = 0; index < npcAvoidanceCandidates.size(); ++index) {
        NpcAvoidanceCandidate &candidate = npcAvoidanceCandidates[index];
        candidate.previous = index ? &npcAvoidanceCandidates[index - 1] : nullptr;
        candidate.next = index + 1 < npcAvoidanceCandidates.size() ? &npcAvoidanceCandidates[index + 1] : nullptr;
        candidate.distance = 10.0f;
        candidate.kind = candidate.flags = 0;
    }
    npcAvoidanceFirst = npcAvoidanceCandidates.data();
    npcAvoidanceLast = &npcAvoidanceCandidates.back();
    npcAvoidanceLow = context.laneLow;
    npcAvoidanceHigh = context.laneHigh;
    npcAvoidanceRadius = context.owner->streamRadius;
    npcAvoidanceLateral = context.laneOffset - context.laneStep;
    npcAvoidanceDistance = 10.0f;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(context.tangentYaw), 0, 0);
    const Vector3 &center = context.owner->streamCenter;
    Matrix4x4::preTranslate(-center.x, -center.y, -center.z);
    if (npcRegistryTask.areaCount) {
        for (NpcNoSpawnArea &area : npcRegistryTask.areas) {
            if (!area.name.name || std::abs(area.center.y - context.scenePosition.y) > 1.8f)
                continue;
            Vector3 local = area.center;
            float distance;
            if (MeasureNpcAvoidanceCandidate(local, area.radius, 2, distance) && context.motionStep * 30.0f + 3.0f > distance) {
                InsertNpcAvoidanceCandidate(2, reinterpret_cast<NpcRouteLiveEntry *>(&area), local, area.center, area.radius, distance);
            }
            npcAvoidanceDistance = npcAvoidanceLast->distance;
        }
    }
    if (npcPlayerPresent && std::abs(npcPlayerPosition.y - context.scenePosition.y) < 1.8f) {
        Vector3 local = npcPlayerCenter;
        float distance;
        if (MeasureNpcAvoidanceCandidate(local, npcPlayerRadius, 1, distance) &&
            std::fmax(context.motionStep, npcPlayerDisplacement) * 30.0f + 3.0f > distance) {
            InsertNpcAvoidanceCandidate(1, reinterpret_cast<NpcRouteLiveEntry *>(&npcPlayerPosition), local, npcPlayerCenter, npcPlayerRadius, distance);
        }
        npcAvoidanceDistance = npcAvoidanceLast->distance;
    }
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry && entry != npcCrowdBookmark; entry = entry->next) {
        if (!entry->node || entry->name.name == context.scriptOwner.name || (entry->flags & 0x80000040U) ||
            std::abs(entry->localPosition.y - context.scenePosition.y) > 1.8f)
            continue;
        Vector3 local = entry->streamCenter;
        float distance;
        if (MeasureNpcAvoidanceCandidate(local, entry->streamRadius, 3, distance) && context.motionStep * 30.0f + 3.0f > distance)
            InsertNpcAvoidanceCandidate(3, entry, local, entry->streamCenter, entry->streamRadius, distance);
    }
    Matrix4x4::Pop(1);
    NpcAvoidanceCandidate *obstacle = npcAvoidanceFirst;
    float obstacleLateral = 0.0f;
    float combinedRadius = 0.0f;
    while (obstacle && obstacle->kind) {
        obstacleLateral = obstacle->localX + npcAvoidanceLateral;
        combinedRadius = npcAvoidanceRadius + obstacle->lateralRadius;
        if (std::abs(obstacle->localX) <= combinedRadius)
            break;
        if (obstacle->distance < 0.0f) {
            if (obstacle->localX >= 0.0f)
                npcAvoidanceHigh = std::fmin(npcAvoidanceHigh, obstacleLateral - combinedRadius);
            else
                npcAvoidanceLow = std::fmax(npcAvoidanceLow, obstacleLateral + combinedRadius);
            if (npcAvoidanceLow > npcAvoidanceHigh)
                npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
        }
        obstacle = obstacle->next;
    }
    if (!obstacle || !obstacle->kind) {
        context.field0a1 &= ~8;
        return context.avoidanceResult;
    }
    float nearestZ = obstacle->localZ;
    const float forwardRadius = npcAvoidanceRadius + obstacle->forwardRadius;
    const float separation = npcAvoidanceRadius * 2.0f + 0.1f;
    for (NpcAvoidanceCandidate *candidate = npcAvoidanceFirst; candidate && candidate->kind;) {
        if (candidate != obstacle && candidate->kind != 4) {
            if (candidate->lateralRadius + obstacle->forwardRadius + separation > std::abs(candidate->localZ - obstacle->localZ) &&
                candidate->lateralRadius + obstacle->lateralRadius + separation > std::abs(candidate->localX - obstacle->localX)) {
                const float radius = (obstacle->lateralRadius + std::abs(candidate->localX - obstacle->localX) + candidate->lateralRadius) * 0.5f;
                if (radius > obstacle->lateralRadius) {
                    obstacle->localX = std::fmin(obstacle->localX - obstacle->lateralRadius, candidate->localX - candidate->lateralRadius) + radius;
                    obstacle->lateralRadius = radius;
                    obstacleLateral = npcAvoidanceLateral + obstacle->localX;
                    combinedRadius = npcAvoidanceRadius + radius;
                    nearestZ = std::fmin(candidate->localZ, nearestZ);
                    candidate->kind = 4;
                    candidate = npcAvoidanceFirst;
                    continue;
                }
            }
            if (candidate->distance < 0.0f) {
                const float lateral = npcAvoidanceLateral + candidate->localX;
                const float radius = candidate->lateralRadius + npcAvoidanceRadius;
                if (obstacle->localX >= 0.0f)
                    npcAvoidanceHigh = std::fmin(npcAvoidanceHigh, lateral - radius);
                else
                    npcAvoidanceLow = std::fmax(npcAvoidanceLow, lateral + radius);
                if (npcAvoidanceLow > npcAvoidanceHigh)
                    npcAvoidanceLow = npcAvoidanceHigh = (npcAvoidanceLow + npcAvoidanceHigh) * 0.5f;
            }
        }
        candidate = candidate->next;
    }
    if (nearestZ > 0.0f || (context.flags & 0x10400U) == 0x400U) {
        if (nearestZ <= 0.0f)
            UpdateNpcStairAvoidance(context, *obstacle);
        context.field0a1 &= ~8;
        return context.avoidanceResult;
    }
    std::int32_t side = context.avoidanceAuxiliary;
    if (obstacle->kind == 3 && obstacle->source->stateContext.phase == 7 && obstacle->source->stateContext.other == context.owner)
        side = obstacle->source->stateContext.avoidanceAuxiliary;
    else if (previousResult < 2 && !(context.field0a1 & 0x0a)) {
        const Vector3 local = NpcPositionInFrame(obstacle->position, center, ReadNpcEulerYaw(context.savedEuler));
        side = local.x < 0.0f ? 1 : -1;
    }
    std::uint8_t flags = static_cast<std::uint8_t>(context.field0a1) & ~8U;
    const std::int32_t sign = side == -1 ? -1 : 1;
    const float localX = obstacle->localX * static_cast<float>(sign);
    const float high = sign == -1 ? -npcAvoidanceLow : npcAvoidanceHigh;
    const float low = sign == -1 ? -npcAvoidanceHigh : npcAvoidanceLow;
    obstacleLateral *= static_cast<float>(sign);
    if (sign == -1)
        obstacle->flags |= 1U;
    float targetLane = obstacleLateral + combinedRadius + 0.1f;
    float retreatDistance = 0.0f;
    std::uint8_t result = 4;
    bool moveAside = localX <= combinedRadius * 0.5f && targetLane <= high && (localX <= 0.0f || obstacle->distance >= 0.15000001f);
    if (!moveAside) {
        if (low <= obstacleLateral - combinedRadius - 0.1f)
            side = -side;
        else if (targetLane > high) {
            if (forwardRadius + 0.5f <= -obstacle->localZ) {
                targetLane = localX <= combinedRadius * 0.5f ? high : low;
                flags |= 8U;
                moveAside = true;
            } else
                result = 1;
        } else if (forwardRadius + 0.15000001f <= -obstacle->localZ)
            moveAside = true;
        else if (!(context.flags & 0x10000U)) {
            flags &= ~4U;
            retreatDistance = forwardRadius + 0.15000001f + obstacle->localZ;
            result = 3;
        } else
            result = 1;
    }
    if (moveAside) {
        targetLane *= static_cast<float>(sign);
        const float distance = std::abs(targetLane - npcAvoidanceLateral);
        if (forwardRadius + 0.80000001f <= -obstacle->localZ || distance < 0.40000001f) {
            result = 2;
            context.runtime0fc = 0;
            if (std::abs(distance) >= 0.001f || std::abs(obstacle->distance) >= 0.001f) {
                const std::int16_t angle = static_cast<std::int16_t>(FixedAtan2(distance, obstacle->distance));
                context.runtime0fc = static_cast<std::uint32_t>(2 * std::abs(static_cast<std::int32_t>(angle)));
                if (context.runtime0fc > 0x4000U)
                    context.runtime0fc = 0x4000U;
            }
        } else if (distance >= 0.80000001f) {
            result = 6;
            flags &= ~2U;
        } else
            result = 5;
    } else if (result == 1 && obstacle->distance <= 0.5f) {
        const std::uint16_t parameter = context.parameter;
        context.parameter = static_cast<std::uint16_t>(AdvanceNpcRouteByDistance(context, 2.0f));
        if (context.parameter != context.highParameter) {
            Vector3 sample;
            SampleNpcRouteWindow(context, &sample, false);
            sample = NpcPositionInFrame(NpcRouteAreaToScene(sample), center, context.tangentYaw);
            const Vector3 local = NpcPositionInFrame(obstacle->position, center, context.tangentYaw);
            if (sample.z > local.z + forwardRadius)
                result = 0;
        }
        context.parameter = parameter;
    }
    context.avoidanceAuxiliary = static_cast<std::int8_t>(side);
    context.avoidanceResult = result;
    context.field0a1 = static_cast<std::int8_t>(flags);
    context.field0a0 = static_cast<std::uint8_t>(obstacle->kind);
    context.other = obstacle->source;
    switch (result) {
    case 0:
        context.other = nullptr;
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
        }
        break;
    case 1:
        context.other = nullptr;
        if (!(context.flags & 0x100000U)) {
            context.routeFlags |= 0x800U;
            context.flags |= 0x100000U;
        }
        break;
    case 2:
    case 5:
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
        }
        if (context.field0a1 & 2) {
            context.field0a1 &= ~2;
            RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
        }
        context.laneOffset = targetLane;
        context.laneStep = targetLane - context.lateralPosition;
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, targetLane);
        break;
    case 3:
        if (!(flags & 4U)) {
            const Vector3 behind = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw - 0x4000, retreatDistance);
            Vector3 target = NpcRouteAreaToScene(behind);
            target.y = 0.0f;
            context.flags |= 0x101000U;
            context.field0a1 = static_cast<std::int8_t>((flags & ~6U) | 4U);
            SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            const Vector3 &root = context.motionWork->runtime.rootPosition;
            const float x = target.x - root.x;
            const float z = target.z - root.z;
            const float maximum = GetMotionRigScale(static_cast<std::uint32_t>(GetMotionRigBoneId(context.motionWork)), 8, 0) * 0.55000001f * 0.89999998f;
            const float distance = std::sqrt(x * x + z * z);
            if (distance > maximum)
                target = {root.x + x * (maximum / distance), root.y, root.z + z * (maximum / distance)};
            CopyMotionTransition(context.motionWork, &target, static_cast<std::uint16_t>(context.tangentYaw), 6, 3);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
            if (context.flags & 0x10U)
                CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0);
            context.routeFlags |= 0x800000U;
        }
        break;
    case 4:
        context.other = nullptr;
        context.field0a1 &= ~0x20;
        break;
    case 6:
        if (!(flags & 2U)) {
            if (context.flags & 0x100000U) {
                context.routeFlags &= ~0x800U;
                context.flags &= ~0x101000U;
            }
            if (context.movementProgress <= 0.0f) {
                if (!(context.flags & 0x80000U)) {
                    context.flags |= 0x80000U;
                    context.savedSplinePosition = context.position;
                    context.savedMoveTarget = context.moveTarget;
                    context.field13c = context.laneOffset;
                }
                context.position = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw, -npcAvoidanceLateral);
                context.moveTarget = OffsetNpcRoutePosition(context.localPosition, context.tangentYaw, targetLane - npcAvoidanceLateral);
            } else
                context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, targetLane);
            const Vector3 target = NpcRouteAreaToScene(context.moveTarget);
            context.laneOffset = targetLane;
            context.field0a1 = static_cast<std::int8_t>((flags & ~6U) | 2U);
            RequestNpcRouteTurn(context, &target, static_cast<std::uint16_t>(context.tangentYaw), 3);
        }
        break;
    }
    return result;
}

bool TestNpcForwardCylinder(const Vector3 &position, std::int16_t angle, float radius, float length) {
    const Vector3 local = TransformNpcPoint(position);
    const std::int16_t yaw = static_cast<std::int16_t>(FixedAtan2(-local.x, -local.z));
    return static_cast<float>(angle) > std::abs(static_cast<float>(yaw)) && local.z > -length && radius > std::abs(local.x);
}

bool UpdateNpcObstructionState(NpcRouteStateContext &context, bool blocked) {
    if (!blocked) {
        if (context.flags & 0x100000U) {
            context.flags &= ~0x100000U;
            SetNpcRouteMotionState(context, 3);
            return true;
        }
    } else if (context.flags & 0x100000U) {
        if (!(context.owner->flags & 0x4010000U)) {
            context.owner->streamingCountdown = static_cast<std::uint16_t>((context.owner->streamingCountdown & 0xff00U) | 10U);
            if (++context.obstructionFrames > 300) {
                RequestNpcStreamOut(*context.owner, 2);
                context.obstructionFrames = 0;
                return true;
            }
        }
    } else {
        context.routeFlags |= 0x800U;
        context.obstructionFrames = 0;
        context.flags |= 0x100000U;
    }
    return false;
}

void UpdateNpcAmbientLook(NpcRouteStateContext &context) {
    if (!context.owner->node)
        return;

    if (context.doorWaitFrames < 0) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        }

        const std::int32_t interval = 120 * static_cast<std::int8_t>(context.owner->recordBase[5]);
        context.doorWaitFrames = static_cast<std::int16_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(interval)));
        if (context.doorWaitFrames < interval / 2)
            context.doorWaitFrames = static_cast<std::int16_t>(interval / 2);
    }

    if (--context.doorWaitFrames != 120)
        return;

    std::int32_t point = static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(npcSchedule.areaPointCount));
    if (npcSchedule.areaPointCount && point >= static_cast<std::int32_t>(npcSchedule.areaPointCount))
        point = static_cast<std::int32_t>(npcSchedule.areaPointCount) - 1;
    const std::byte *const areaPoint = GetNpcAreaPoint(static_cast<std::uint16_t>(point));
    if (!areaPoint)
        return;

    Vector3 target;
    std::memcpy(&target, areaPoint + 0x34, sizeof(target));
    Vector3 head{};
    GetMotionBonePosition(context.motionWork, MB_Eyes, &head);
    if (!std::isnormal(head.y) && head.y != 0.0f)
        head.y = 0.0f;
    target.y = static_cast<float>(point % 25) * 0.0099999998f + head.y - static_cast<float>(static_cast<std::int8_t>(context.owner->recordBase[8]) - 1) * 0.1f;
    target = NpcRouteAreaToScene(target);
    UpdateMotionWorkWithPosition(context.motionWork, &target, 0);
    context.flags |= 0x20U;
}

void TickNpcRouteMovement(NpcRouteStateContext &context) {
    Character *const character = context.owner->node;
    if (!character) {
        if (AdvanceUnstreamedNpc(context)) {
            context.moveState = 2;
            StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
        }
        return;
    }
    if ((context.flags & 0x41000U) == 0x41000U)
        return;
    Vector3 offset = NpcPositionInFrame(context.moveTarget, context.localPosition, context.tangentYaw);
    context.lateralPosition = context.laneOffset - offset.x;
    context.laneStep = offset.x;
    context.movementProgress = offset.z;
    if (offset.z > -context.motionStep) {
        if (context.flags & 0x80000U) {
            context.flags &= ~0x80000U;
            context.laneOffset = context.field13c;
            context.position = context.savedSplinePosition;
            context.moveTarget = context.savedMoveTarget;
            offset = NpcPositionInFrame(context.moveTarget, context.localPosition, context.tangentYaw);
            context.lateralPosition = context.laneOffset - offset.x;
            context.laneStep = offset.x;
            context.movementProgress = offset.z;
        } else {
            context.extraStride =
                offset.z <= context.motionStep ? 0 : static_cast<std::uint16_t>(static_cast<std::int32_t>(offset.z * 64.0f / context.segmentLength));
            context.moveState = 2;
        }
    }
    if (context.owner->flags & 0x40U) {
        StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
        ApplyNpcRouteEuler(context);
        return;
    }
    DetectNpcRouteStair(context);
    bool largeCorrection = false;
    if (!(context.flags & 0x400U)) {
        const float fromLow = context.interpolatedLaneLow - context.lateralPosition;
        const float fromHigh = context.lateralPosition - context.interpolatedLaneHigh;
        if (fromLow > 0.0f || fromHigh > 0.0f) {
            if ((context.routeFlags & 0x800000U) && !(context.routeFlags & 0x80U)) {
                if (context.movementProgress < 0.0f && context.interpolatedLaneHigh - context.interpolatedLaneLow < 0.30000001f)
                    context.routeFlags &= ~0x800000U;
            } else {
                const float boundary = fromLow > 0.0f ? context.interpolatedLaneLow : context.interpolatedLaneHigh;
                offset.x -= boundary - context.lateralPosition;
                context.lateralPosition = boundary;
                context.laneStep = offset.x;
                offset = {-offset.x, -offset.y, -offset.z};
                Matrix4x4_Push(&transformMatrix_prePass);
                Matrix4x4::preTranslate(context.moveTarget.x, context.moveTarget.y, context.moveTarget.z);
                Matrix4x4::SetViewMatrixFromEuler(context.tangentYaw, 0, 0);
                const Vector3 position = TransformNpcPoint(offset);
                Matrix4x4::Pop(1);
                if (!(context.routeFlags & 0x80U) && (fromLow > 0.1f || fromHigh > 0.1f)) {
                    largeCorrection = true;
                    context.routeFlags |= 0x800000U;
                } else {
                    context.routeFlags = (context.routeFlags & ~0x880000U) | 0x80000U;
                    context.localPosition = position;
                    if (context.interpolatedLaneHigh - context.interpolatedLaneLow <= 0.2f)
                        context.laneCorrectionFrames = 0;
                    else if (++context.laneCorrectionFrames > 128)
                        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
                }
            }
        } else if (context.movementProgress < 0.0f) {
            context.routeFlags &= ~0x800000U;
            context.laneCorrectionFrames = 0;
        }
    }
    std::int32_t yawDelta = static_cast<std::int16_t>(context.tangentYaw - ReadNpcEulerYaw(context.savedEuler));
    if ((context.routeFlags & 2U) && !(context.owner->flags & 0x10U) && !(context.flags & 0x40000U)) {
        if (!UpdateNpcRouteAvoidance(context)) {
            if (context.flags & 0x100000U) {
                context.flags &= ~0x100000U;
                SetNpcRouteMotionState(context, 3);
            }
            if (context.field0a1 & 2) {
                context.field0a1 &= ~2;
                RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 2);
            }
            context.avoidanceWaitFrames = 0;
            if (context.routeFlags & 0x200U)
                UpdateNpcAmbientLook(context);
        } else if ((context.flags & 0x1000U) && !(context.owner->flags & 0x10000U) && !(context.routeFlags & 0x10U) && ++context.avoidanceWaitFrames > 90) {
            context.avoidanceWaitFrames = 0;
            if (context.routeFlags & 0x1000000U) {
                context.owner->streamingCountdown = static_cast<std::uint16_t>((context.owner->streamingCountdown & 0xff00U) | 10U);
                RequestNpcStreamOut(*context.owner, 2);
                return;
            }
            const std::uint32_t flags = context.flags;
            const bool returning = (context.routeFlags & 0x4000000U) != 0;
            InitializeNpcRouteRequest(context, context.pendingAreaPoint, context.currentAreaPoint);
            SetNpcLocomotionScale(context, context.owner->motionSpeed);
            if (flags & 0x10U)
                SetNpcRouteMotionState(context, 1);
            if (!returning)
                context.routeFlags |= 0x4000000U;
            if (flags & 0x400000U)
                context.flags |= 0x400000U;
            if (flags & 0x200000U) {
                if (CharacterControlModel *const control = FindCharacterControlModel(character))
                    control->flags = (control->flags & ~0x806U) | 0x800U;
                context.flags |= 0x200000U;
            }
            return;
        }
    }
    if (!TickNpcRouteStair(context) && (context.routeFlags & 0x800U)) {
        CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 13, 0);
        CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        context.flags |= 0x1000U;
        context.routeFlags &= ~0x800U;
        return;
    }
    if (context.flags & 0x400U) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        }
        return;
    }
    if (mtwkSetExtraFlag(context.motionWork, 58, 1)) {
        if (context.flags & 0x20U) {
            context.flags &= ~0x20U;
            UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        }
        context.field0a1 |= 1;
        StoreNpcEulerYaw(context.savedEuler, context.motionWork->runtime.entityRotation.y);
        if (CopyMotionTransition(context.motionWork, nullptr, 0, 3, 0))
            RequestNpcRouteMotion(context, true);
    } else if (context.field0a1 & 1) {
        context.field0f4 = 0;
        const Vector3i rotation = GetCharacterWorldRotation(character);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        context.field0a1 &= ~7;
        if (context.moveState == 2)
            return;
        context.laneOffset = context.lateralPosition;
        if (context.laneOffset > context.laneHigh)
            context.laneOffset = context.laneHigh;
        else if (context.laneLow > context.laneOffset)
            context.laneOffset = context.laneLow;
        context.laneStep = context.laneOffset - context.lateralPosition;
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, context.laneOffset);
    }
    const std::uint8_t avoidance = context.avoidanceResult;
    if (avoidance == 6 || avoidance == 3 || (context.flags & 0x100000U))
        return;
    if (avoidance == 5) {
        RequestNpcRouteTurn(context, nullptr,
            static_cast<std::uint16_t>(NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z)), 2);
        return;
    }
    std::int32_t correction = avoidance == 2 ? static_cast<std::int32_t>(context.runtime0fc) :
                                               static_cast<std::int32_t>(std::abs(context.laneStep) * 16384.0f * (largeCorrection ? 3.0f : 1.0f));
    if (correction > 0x4000)
        correction = 0x4000;
    if (avoidance)
        context.field0a1 |= 0x10;
    else if (context.field0a1 & 0x10) {
        context.field0f4 = 0;
        context.field0a1 &= ~0x10;
    }
    const std::int32_t side = context.laneStep < 0.0f ? -1 : context.laneStep > 0.0f ? 1 : 0;
    const std::int32_t corrected = static_cast<std::int16_t>(yawDelta - correction * side);
    if (yawDelta * side >= 0 || largeCorrection)
        yawDelta = corrected;
    const std::int32_t magnitude = std::abs(yawDelta);
    if (magnitude > 10922 && (!avoidance || (context.field0a1 & 0x20))) {
        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + (avoidance == 2 ? corrected : yawDelta)), 2);
        return;
    }
    const std::int32_t direction = corrected < 0 ? -1 : corrected > 0 ? 1 : 0;
    context.field0f4 += static_cast<std::int32_t>(context.motionStep * 1000.0f);
    if (magnitude < static_cast<std::int32_t>(context.field0f4))
        context.field0f4 = magnitude;
    if (static_cast<std::int32_t>(context.field0f4) > context.maximumYawStep)
        context.field0f4 = context.maximumYawStep;
    if (avoidance == 2 && mtwkSetExtraFlag(context.motionWork, 58, 1) && static_cast<std::int32_t>(context.field0f4) == context.maximumYawStep) {
        RequestNpcRouteTurn(context, nullptr, static_cast<std::uint16_t>(corrected + ReadNpcEulerYaw(context.savedEuler)), 2);
    } else if (!mtwkSetExtraFlag(context.motionWork, 58, 1)) {
        StoreNpcEulerYaw(context.savedEuler, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + direction * context.field0f4));
        ApplyNpcRouteEuler(context);
    }
}

std::int32_t FindNearestNpcParameter(NpcRouteStateContext &context, const Vector3 &position) {
    Vector3 sampled;
    context.parameter = 0;
    SampleNpcRouteWindow(context, &sampled, false);
    std::int32_t nearest = 0;
    float x = position.x - sampled.x;
    float z = position.z - sampled.z;
    float distance = x * x + z * z;
    context.parameter += 32;
    std::int32_t valid;
    do {
        valid = SampleNpcRouteWindow(context, &sampled, true);
        x = position.x - sampled.x;
        z = position.z - sampled.z;
        const float candidate = x * x + z * z;
        if (distance > candidate) {
            nearest = context.parameter;
            distance = candidate;
        }
        context.parameter += 32;
    } while (valid == 1);
    context.parameter = static_cast<std::uint16_t>(nearest < 64 ? 0 : nearest - 64);
    const std::int32_t end = nearest + 64;
    SampleNpcRouteWindow(context, &sampled, false);
    x = position.x - sampled.x;
    z = position.z - sampled.z;
    if (distance > x * x + z * z) {
        nearest = context.parameter;
        distance = x * x + z * z;
    }
    ++context.parameter;
    valid = 1;
    while (context.parameter < end && valid == 1) {
        valid = SampleNpcRouteWindow(context, &sampled, true);
        x = position.x - sampled.x;
        z = position.z - sampled.z;
        if (distance > x * x + z * z) {
            nearest = context.parameter;
            distance = x * x + z * z;
        }
        ++context.parameter;
    }
    return nearest;
}

void EnterNpcRoute(NpcRouteStateContext &context) {
    if (context.flags & 2U) {
        context.startNode = context.crossingPoint == 2 && (context.parameter & 63U) > 32U ? context.nextNode : context.activeNode;
        context.routeFlags |= 0x804000U;
        context.endNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.pendingAreaPoint) + 4));
        context.flags = (context.flags & ~2U) | 4U;
        context.moveState = 0;
        context.phase = 1;
    } else if (context.currentAreaPoint != context.pendingAreaPoint || (context.flags & 0x300U)) {
        if (context.routeFlags & 0x20U)
            context.startNode = context.endNode = 0;
        else {
            context.startNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.currentAreaPoint) + 4));
            context.endNode = static_cast<std::int16_t>(ReadU16(GetNpcAreaPoint(context.pendingAreaPoint) + 4));
        }
        context.flags &= ~4U;
        context.phase = 1;
    } else {
        context.phase = 9;
        context.flags &= ~4U;
    }
}

void BuildNpcRoute(NpcRouteStateContext &context) {
    if (npcRouteBuildCount >= 8 && !(context.flags & 0x100U))
        return;
    ++npcRouteBuildCount;
    if (!BuildNpcRouteNodePath(context)) {
        context.flags |= 0x80U;
        context.phase = 9;
    } else
        context.phase = 2;
}

void BindNpcRouteWindow(NpcRouteStateContext &context) {
    if ((context.flags & 4U) || (context.routeFlags & 0x20U)) {
        NpcRouteStateContext copy = context;
        context.lowParameter = static_cast<std::uint16_t>(FindNearestNpcParameter(copy, context.localPosition));
    } else {
        context.lowParameter = GetNpcRouteConnectionParameter(
            GetNpcAreaPoint(context.currentAreaPoint), context.routeNodes[0], context.routeCount > 1 ? context.routeNodes[1] : 0xffff, false);
    }
    if (context.routeFlags & 0x20U) {
        NpcRouteStateContext copy = context;
        context.highParameter = static_cast<std::uint16_t>(FindNearestNpcParameter(copy, context.targetPosition));
    } else {
        context.highParameter = static_cast<std::uint16_t>((context.pointCount - 3) * 64 -
            GetNpcRouteConnectionParameter(GetNpcAreaPoint(context.pendingAreaPoint), context.routeNodes[context.routeCount - 1],
                context.routeCount > 1 ? context.routeNodes[context.routeCount - 2] : 0xffff, true));
    }
    if (context.lowParameter > context.highParameter) {
        if (!(context.flags & 0x2000U)) {
            context.startNode ^= 0x8000;
            context.flags |= 0x2000U;
            context.phase = 1;
            return;
        }
        context.lowParameter = context.highParameter;
    }
    context.startNode = context.routeNodes[0];
    context.endNode = context.routeNodes[context.routeCount - 1];
    context.stride = 1;
    context.extraStride = 0;
    context.parameter = context.lowParameter;
    UpdateNpcRouteWindow(context, context.parameter);
    SampleNpcRouteWindow(context, &context.position, false);
    if (context.stateTable[context.activeNode & 0x7fff].flags & 0x400U)
        context.owner->flags |= 0x100000U;
    else {
        context.owner->flags &= ~0x100000U;
        context.owner->motionAttachments[0].flags &= ~0x40U;
    }
    context.phase = 6;
    if (!(context.flags & 0x204U) && !(context.routeFlags & 0x20U) && (context.owner->flags & 0x800U)) {
        const std::byte *const point = GetNpcAreaPoint(context.currentAreaPoint);
        if (!ReadU32(point + 0x40)) {
            context.otherName = shen_name{ReadU32(point + 0x30)};
            if (context.otherName.name)
                context.phase = 5;
        }
    }
    if (context.routeFlags & 1U) {
        context.flags |= 8U;
        context.savedPhase = context.phase;
        context.phase = 4;
        if ((context.flags & 0x8000U) && context.owner->node)
            InitiateCleanupForSceneChara(context.scriptOwner);
    }
    context.flags |= 2U;
    context.moveState = 0;
}

void TickNpcPausedRoute(NpcRouteStateContext &context) {
    if (!(context.routeFlags & 1U)) {
        if (context.flags & 0x8000U) {
            if (context.owner->node)
                InitiateCleanupForSceneChara(context.scriptOwner);
            context.flags &= ~0x8000U;
        }

        context.flags &= ~0x28U;
        context.phase = context.savedPhase;
        return;
    }

    if (!(context.flags & 0x8000U) || (context.routeFlags & 0x40000U))
        return;

    if (!(context.flags & 0x20U) && context.owner->node) {
        bool wait = false;
        if (mtwkSetExtraFlag(context.motionWork, 58, 1))
            wait = ++context.owner->counter416 <= 8;
        if (!wait && g_playerRuntime.character) {
            Vector3 head{};
            GetMotionBonePosition(FindMotionWork(g_playerRuntime.character), MB_Eyes, &head);
            UpdateMotionWorkWithPosition(context.motionWork, &head, 1);
            context.flags |= 0x20U;
        }
    }
    DateTime date;
    GetCurrentDateTime(date);
    if (120 * (static_cast<std::int32_t>(date.minute) + 60 * date.hour) - context.waitStartTime > 1800) {
        if (context.owner->node)
            UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        context.flags &= ~0x20U;
        context.routeFlags &= ~1U;
        if (!(context.flags & 8U))
            context.flags &= ~0x8000U;
        NpcRouteLiveEntry &owner = *context.owner;
        if (context.flags & 0x100U) {
            if (owner.eyeControlMode) {
                owner.field338 = static_cast<std::uint8_t>(-owner.eyeControlMode);
                owner.eyeControlMode = 10;
            } else {
                owner.field338 = static_cast<std::uint8_t>(owner.routeDirectionMode);
                owner.routeDirectionMode = 10;
            }
        } else if (!(owner.flags & 0x10U)) {
            if (!(owner.field338 & 0x80U))
                owner.routeDirectionMode = 12;
            else
                owner.eyeControlMode = 12;
        }
    }
}

void CompleteNpcDoorEgress(NpcRouteStateContext &context, bool snap) {
    if (snap) {
        context.localPosition = context.doorPosition;
        StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
    } else if (context.owner->node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(context.owner->node))
            control->flags = (control->flags & ~0x804U) | 4U;
        SetCharacterType(context.owner->node, CharacterTypeDefault);
    }
    if (context.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner);
        context.otherName = {};
        context.owner->flags &= ~0x100U;
    }
    context.phase = 6;
    context.moveState = 0;
    if (!(context.stateTable[context.activeNode & 0x7fff].flags & 0x400U)) {
        context.owner->flags &= ~0x100000U;
        context.owner->motionAttachments[0].flags &= ~0x40U;
    }
}

void BeginNpcDoorEgressMotion(NpcRouteStateContext &context) {
    context.moveState = 1;
    context.doorMotion =
        static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.owner->node, &context.scenePosition));
    Vector3 track{};
    SampleNativeMotionRoot(context.doorMotion, GetCharacterBoneId(context.owner->node), 15.0f, track);

    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(context.targetYaw, 0, 0);
    track = TransformNpcPoint(track);
    Matrix4x4::Pop(1);
    context.sceneTarget.x += track.x;
    context.sceneTarget.z += track.z;
    context.sceneTarget = NpcRouteAreaToScene(context.sceneTarget);
    if (!FindNpcIntersectionInScene(context.sceneTarget, context.moveFlags)) {
        Vector3 floor = NpcRouteAreaToScene(context.doorPosition);
        FindNpcIntersectionInScene(floor, context.moveFlags);
        context.scenePosition.y = floor.y;
        shenApplyTranslationToAll(&context.owner->node->entity.node, context.scenePosition);
        context.localPosition = NpcAreaPosition(context.scenePosition);
    }
    RequestNpcRouteMotion(context, true);
}

void TickNpcDoorEgress(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    switch (context.moveState) {
    case 0: {
        owner.flags &= ~0x800U;
        if (!ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.moveTarget)) {
            CompleteNpcDoorEgress(context, false);
            return;
        }
        context.moveTarget = NpcAreaPosition(context.moveTarget);
        context.targetYaw = g_doorIngressRotation.y;
        context.savedSplinePosition = GetDoorIngressOrigin();
        context.sceneTarget = context.moveTarget;
        ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.doorPosition);
        context.doorPosition = NpcAreaPosition(context.doorPosition);
        context.savedSplinePosition = NpcAreaPosition(context.savedSplinePosition);
        const float x = context.position.x - context.moveTarget.x;
        const float z = context.position.z - context.moveTarget.z;
        if (std::abs(x) >= 0.001f || std::abs(z) >= 0.001f) {
            StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(x, z));
            if (NpcPositionInFrame(context.doorPosition, context.localPosition, ReadNpcEulerYaw(context.savedEuler)).z > 0.0f) {
                CompleteNpcDoorEgress(context, false);
                return;
            }
        }
        owner.flags |= 0x100000U;
        if (!ClaimDoor(context.otherName, context.scriptOwner)) {
            context.moveState = 3;
            owner.field342 = 0;
        } else if (owner.node)
            BeginNpcDoorEgressMotion(context);
        else {
            owner.flags |= 0x1000000U;
            context.doorWaitFrames = 150;
            context.moveState = 7;
        }
        break;
    }
    case 1:
        if (!owner.node) {
            CompleteNpcDoorEgress(context, true);
            return;
        }
        if (UpdateNpcObstructionState(context, QueryNpcMoveObstruction(owner, context.moveTarget, true, 0x4000, 0.44999999f, 1.5f) != 0))
            return;
        if (context.routeFlags & 0x800U) {
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
            SetMotionTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (!(context.flags & 0x1000U) && !mtwkSetExtraFlag(context.motionWork, 58, 1)) {
            StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
            owner.flags |= 0x100U;
            context.moveState = 4;
            BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner, context.doorMotion, 14.0f, context.motionScale * 1.1f);
            SetMotionSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
        }
        break;
    case 3:
        if (ClaimDoor(context.otherName, context.scriptOwner)) {
            if (owner.node)
                context.moveState = 6;
            else {
                owner.flags |= 0x1000000U;
                context.doorWaitFrames = 150;
                context.moveState = 7;
            }
        } else if (static_cast<std::int16_t>(++owner.field342) > 1800) {
            if (!owner.node) {
                owner.field342 = 0;
                CompleteNpcDoorEgress(context, true);
            } else {
                RequestNpcStreamOut(owner, 2);
                owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
            }
        }
        break;
    case 4:
        if (!owner.node)
            CompleteNpcDoorEgress(context, true);
        else if (SetMotionRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody))
            CompleteNpcDoorEgress(context, false);
        break;
    case 6:
        if (owner.node)
            BeginNpcDoorEgressMotion(context);
        else
            CompleteNpcDoorEgress(context, true);
        break;
    case 7:
        if (owner.node) {
            context.doorWaitFrames = 0;
            context.moveState = 6;
        } else if (context.doorWaitFrames-- == 1) {
            owner.flags &= ~0x1000000U;
            CompleteNpcDoorEgress(context, true);
        }
        break;
    }
}

void TickNpcRouteApproach(NpcRouteStateContext &context) {
    if (!context.moveState) {
        const std::uint16_t parameter = context.parameter;
        ++context.parameter;
        Vector3 tangent;
        SampleNpcRouteWindow(context, &tangent, false);
        context.parameter = parameter;
        context.tangentYaw = NpcRouteYaw(tangent.x - context.position.x, tangent.z - context.position.z);
        LoadNpcRouteLanes(context);
        const bool outside = FitNpcRouteLane(context, 0.0f);
        context.moveTarget = OffsetNpcRoutePosition(context.position, context.tangentYaw, context.laneOffset);
        StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z));
        context.sceneTarget = NpcRouteAreaToScene(context.moveTarget);
        context.targetYaw = context.tangentYaw;
        if ((outside || (context.owner->flags & 0x8000000U)) && !(context.flags & 0x20000U)) {
            RequestNpcRouteMotion(context, true);
            context.moveState = 1;
            return;
        }
        if (context.owner->node) {
            SetMotionSpeed(context.motionWork, context.motionScale, MotionRegion::LowerBody);
            Vector3 position = NpcRouteAreaToScene(context.localPosition);
            if (context.stairFlags) {
                if (context.routeFlags & 0x4040U)
                    FindNpcIntersectionInScene(position, context.moveFlags);
                position.y -= 0.22499999f;
                Vector3 origin;
                std::int32_t yaw;
                if (flddQueryStairOrigin(position, origin, yaw)) {
                    context.moveState = 5;
                    CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 13, 0);
                    CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
                    return;
                }
            }
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(context.tangentYaw), 5, 2);
            if (context.flags & 0x10U)
                CopyMotionTransition(context.motionWork, nullptr, 0, 8, 0);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        }
    } else if (context.moveState == 5) {
        if (context.owner->node && mtwkSetExtraFlag(context.motionWork, 58, 1))
            return;
    } else if (context.moveState == 1) {
        if (!context.owner->node) {
            if (!AdvanceUnstreamedNpc(context))
                return;
        } else {
            const bool blocked = !(context.owner->flags & 0x8000000U) && !(context.routeFlags & 0x10U) &&
                (QueryNpcMoveObstruction(*context.owner, context.moveTarget, false, 0x4000, context.owner->streamRadius, -1.0f) & 1U);
            if (UpdateNpcObstructionState(context, blocked))
                return;
            if (context.routeFlags & 0x800U) {
                CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
                CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
                context.flags |= 0x1000U;
                context.routeFlags &= ~0x800U;
            }
            if (context.flags & 0x1000U)
                return;
            if (CopyMotionTransition(context.motionWork, nullptr, 0, 3, 0)) {
                RequestNpcRouteMotion(context, true);
                return;
            }
            if (mtwkSetExtraFlag(context.motionWork, 58, 1))
                return;
        }
    } else
        return;
    context.phase = 7;
    context.moveState = 2;
    StoreNpcEulerYaw(context.savedEuler, context.tangentYaw);
    context.movementProgress = 0.0f;
    if (context.flags & 0x20000U)
        RequestNpcRouteMotion(context, true);
    context.routeFlags |= 0x800000U;
    context.flags |= 0x20000U;
}

void TickNpcRouteTraversal(NpcRouteStateContext &context) {
    if (context.moveState == 1) {
        TickNpcRouteMovement(context);
        return;
    }
    if (context.moveState != 2)
        return;
    UpdateNpcRouteWindow(context, context.parameter, true);
    if (context.activeSegment != context.previousSegment) {
        context.stride = ComputeNpcRouteStride(context);
        LoadNpcRouteLanes(context);
        context.previousSegment = context.activeSegment;
        if (context.crossingPoint == 3) {
            if (context.stateTable[context.nextNode & 0x7fff].flags & 0x400U)
                context.owner->flags |= 0x100000U;
            else {
                context.owner->flags &= ~0x100000U;
                context.owner->motionAttachments[0].flags &= ~0x40U;
            }
        }
    }
    context.parameter = static_cast<std::uint16_t>(context.parameter + context.stride + context.extraStride);
    if (!context.avoidanceResult) {
        context.laneOffset -= context.laneStep;
        if (context.laneOffset > context.laneHigh)
            context.laneOffset = context.laneHigh;
        else if (context.laneLow > context.laneOffset)
            context.laneOffset = context.laneLow;
    }
    InterpolateNpcRouteLanes(context, true);
    if (context.highParameter >= context.parameter) {
        const Vector3 previous = context.position;
        Vector3 sample;
        const bool valid = SampleNpcRouteWindow(context, &sample, true) != 0;
        context.position = sample;
        context.tangentYaw = NpcRouteYaw(sample.x - previous.x, sample.z - previous.z);
        context.moveTarget = OffsetNpcRoutePosition(sample, context.tangentYaw, context.laneOffset);
        if (valid) {
            context.moveState = 1;
            TickNpcRouteMovement(context);
            return;
        }
    }
    if (context.flags & 0x400U) {
        context.parameter = context.highParameter;
        context.moveState = 1;
        TickNpcRouteMovement(context);
        return;
    }
    if (!(context.flags & 0x40000U)) {
        context.routeFlags &= ~0x2000800U;
        context.flags &= ~0x101000U;
    }
    if (context.owner->node && (context.flags & 0x200000U))
        RestoreNpcGroundControl(context);
    context.flags &= ~0x20000U;
    context.phase = 8;
    context.moveState = 0;
    if (context.flags & 0x100U) {
        context.phase = 9;
        const std::int32_t yaw = NpcRouteYaw(npcPlayerPosition.x - context.scenePosition.x, npcPlayerPosition.z - context.scenePosition.z);
        const std::int32_t difference = static_cast<std::int16_t>(yaw - ReadNpcEulerYaw(context.savedEuler));
        const std::int32_t correction = difference < 0 ? std::min(difference + 20024, 0) : std::max(difference - 20024, 0);
        const std::uint16_t transitionYaw = static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler) + correction);
        StoreNpcEulerYaw(context.savedEuler, yaw);
        context.owner->counter416 = 0;
        if (context.owner->node) {
            CopyMotionTransition(context.motionWork, nullptr, transitionYaw, 13, 0);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        }
    } else if (context.owner->node && (context.flags & 0x20U)) {
        UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        context.flags &= ~0x20U;
    }
}

void CompleteNpcDoorIngress(NpcRouteStateContext &context, bool snap) {
    if (snap)
        context.localPosition = context.doorPosition;
    if (context.otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner);
        context.otherName = {};
        context.owner->flags = (context.owner->flags & ~0x900U) | 0x800U;
        if (context.owner->node)
            SetCharacterType(context.owner->node, 3);
    }
    context.phase = 9;
}

void TickNpcDoorIngress(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    switch (context.moveState) {
    case 0: {
        context.moveState = 1;
        if (!(context.routeFlags & 0x20U)) {
            const std::byte *const point = GetNpcAreaPoint(context.pendingAreaPoint);
            if (ReadU32(point + 0x40)) {
                CompleteNpcDoorIngress(context, false);
                return;
            }
            context.otherName = shen_name{ReadU32(point + 0x30)};
            if (context.otherName.name && ComputeDoorIngress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.moveTarget)) {
                context.targetYaw = g_doorIngressRotation.y;
                ComputeDoorEgress(static_cast<std::int32_t>(context.otherName.name), &context.scenePosition, &context.doorPosition);
                if (owner.node)
                    UpdateMotionWorkWithPosition(context.motionWork, &context.doorPosition, 0);
                context.doorPosition = NpcAreaPosition(context.doorPosition);
                if (ClaimDoor(context.otherName, context.scriptOwner))
                    owner.flags |= 0x100U;
                else {
                    context.moveState = 3;
                    owner.field342 = 0;
                }
                context.sceneTarget = context.moveTarget;
                context.moveTarget = NpcAreaPosition(context.moveTarget);
                owner.flags |= 0x100000U;
            } else {
                context.otherName = {};
                const std::int16_t route = static_cast<std::int16_t>(ReadU16(point + 4));
                Vector3 tangent;
                SampleNpcRouteNode(static_cast<std::uint16_t>(route), static_cast<float>(static_cast<std::int16_t>(ReadU16(point + 8))) * 0.015625f, tangent);
                Vector3 target;
                std::memcpy(&target, point + 0x34, sizeof(target));
                context.targetYaw = NpcRouteYaw(tangent.x - target.x, tangent.z - target.z) + 0x8000;
                context.moveTarget = context.doorPosition = target;
                context.sceneTarget = NpcRouteAreaToScene(target);
            }
        } else {
            context.targetYaw = static_cast<std::uint32_t>(context.arrivalYaw) | (static_cast<std::uint32_t>(context.field10a) << 16);
            context.moveTarget = context.doorPosition = context.targetPosition;
            context.otherName = {};
            context.sceneTarget = NpcRouteAreaToScene(context.targetPosition);
        }
        StoreNpcEulerYaw(context.savedEuler, NpcRouteYaw(context.moveTarget.x - context.localPosition.x, context.moveTarget.z - context.localPosition.z));
        if (context.routeFlags & 0x20000U) {
            context.routeFlags &= ~0x20000U;
            context.targetYaw = static_cast<std::uint32_t>(context.arrivalYaw) | (static_cast<std::uint32_t>(context.field10a) << 16);
        }
        RequestNpcRouteMotion(context, true);
        return;
    }
    case 1:
        if (!owner.node) {
            if (AdvanceUnstreamedNpc(context))
                CompleteNpcDoorIngress(context, context.otherName.name != 0);
            return;
        }
        if (UpdateNpcObstructionState(context,
                !(owner.flags & 0x8000000U) && !(context.routeFlags & 0x10U) &&
                    QueryNpcMoveObstruction(owner, context.doorPosition, false, 0x4000, owner.streamRadius, -1.0f) != 0))
            return;
        if (context.routeFlags & 0x800U) {
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            context.flags |= 0x1000U;
            context.routeFlags &= ~0x800U;
            if (context.otherName.name)
                SetMotionTimeRelated(context.motionWork, 1, 5, 0);
        }
        if (context.flags & 0x1000U)
            return;
        if (CopyMotionTransition(context.motionWork, nullptr, 0, 3, 0)) {
            RequestNpcRouteMotion(context, true);
            return;
        }
        if (mtwkSetExtraFlag(context.motionWork, 58, 1))
            return;
        if (!context.otherName.name) {
            CompleteNpcDoorIngress(context, false);
            return;
        }
        StoreNpcEulerYaw(context.savedEuler, context.targetYaw);
        context.moveState = 4;
        if (CharacterControlModel *const control = FindCharacterControlModel(owner.node))
            control->flags = (control->flags | 4U) & ~0x800U;
        BeginDoorAnimation(static_cast<std::int32_t>(context.otherName.name), context.scriptOwner, context.doorMotion, 14.0f, context.motionScale * 1.1f);
        SetMotionSpeed(context.motionWork, context.motionScale * 1.1f, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        return;
    case 3:
        if (ClaimDoor(context.otherName, context.scriptOwner)) {
            context.moveState = 1;
            RequestNpcRouteMotion(context, true);
            owner.flags |= 0x100U;
            owner.field342 = 0;
        } else if (static_cast<std::int16_t>(++owner.field342) > 1800) {
            if (owner.node) {
                RequestNpcStreamOut(owner, 2);
                owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
            } else {
                owner.field342 = 0;
                CompleteNpcDoorIngress(context, true);
            }
        }
        return;
    case 4:
        if (!owner.node) {
            CompleteNpcDoorIngress(context, true);
            return;
        }
        if (SetMotionRegionFlag(context.motionWork, 65, 1, MotionRegion::LowerBody)) {
            Vector3 floor = NpcRouteAreaToScene(context.localPosition);
            if (FindNpcIntersectionInScene(floor, context.moveFlags) || (owner.flags & 0x200U)) {
                if (CharacterControlModel *const control = FindCharacterControlModel(owner.node))
                    control->flags = (control->flags & ~4U) | 0x800U;
            } else
                RequestNpcStreamOut(owner, 0);
            CompleteNpcDoorIngress(context, false);
        }
        return;
    }
}

bool FinishNpcRoute(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &owner = *context.owner;
    if (context.routeFlags & 0x10U) {
        const std::uint32_t area = ReadU32(GetNpcAreaPoint(context.pendingAreaPoint)) & 0xffffffU;
        if (area == 0x4c4f47U || area == 0x4d4f44U)
            owner.flags |= 0x100000U;
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != context.scriptOwner.name)
                continue;
            entry.name = SHEN_NAME(MOVE);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            break;
        }
    }
    const std::uint32_t flags = context.flags;
    context.currentAreaPoint = context.pendingAreaPoint;
    context.phase = 0;
    if (flags & 0x100U) {
        context.flags = context.routeFlags = 0;
        InitializeNpcRouteRequest(context, owner.stateContext.currentAreaPoint, owner.pendingAreaPoint);
        SetNpcLocomotionScale(context, owner.motionSpeed);
        context.flags |= 0x200U;
        if (flags & 0x4000U) {
            context.flags |= 0x4000U;
            context.routeFlags |= 0x20000U;
        }
        if (!(owner.field338 & 0x80U))
            owner.routeDirectionMode = 11;
        else
            owner.eyeControlMode = 11;
        context.routeFlags |= 1U;
        context.flags |= 0x8000U;
        DateTime date;
        GetCurrentDateTime(date);
        context.waitStartTime = 120 * (static_cast<std::int32_t>(date.minute) + 60 * date.hour);
        return false;
    }
    if (context.routeFlags & 0x4000000U) {
        context.flags = 0;
        context.routeFlags = 0;
        InitializeNpcRouteRequest(context, owner.stateContext.currentAreaPoint, owner.pendingAreaPoint);
        SetNpcLocomotionScale(context, owner.motionSpeed);
        if (flags & 0x10U)
            SetNpcRouteMotionState(context, 1);
        return false;
    }
    context.routeFlags = 0;
    if (flags & 0x200U) {
        context.flags &= ~0x200U;
        context.routeFlags = 0x400U;
        owner.scheduleProgress = owner.field32f;
        owner.liveState = owner.field331;
        if (!(owner.field338 & 0x80U))
            owner.routeDirectionMode = static_cast<std::int8_t>(owner.field338);
        else
            owner.eyeControlMode = static_cast<std::int8_t>(-static_cast<std::int8_t>(owner.field338));
        if (context.flags & 0x4000U) {
            context.flags &= ~0x4000U;
            owner.currentCommand = owner.branchCommandStart;
            owner.scheduleProgress = 1;
        }
        context.moveFlags = (owner.packedMoveFlags & 0x100ffU) | (static_cast<std::uint32_t>(context.floor) << 8);
    }
    context.flags = 0;
    return true;
}

void UpdateLinkedNpcRoute(NpcRouteStateContext &context) {
    if (context.phase != 7)
        return;
    NpcRouteLiveEntry &leader = *context.linkedRoute.character;
    std::int32_t request = 0;
    float distance = 0.0f;
    if (leader.stateContext.phase == 6)
        request = 1;
    else if (leader.stateContext.phase == 7) {
        const float x = leader.stateContext.localPosition.x - context.localPosition.x;
        const float z = leader.stateContext.localPosition.z - context.localPosition.z;
        distance = std::sqrt(x * x + z * z);
        if (distance > context.linkedRoute.leashDistance)
            request = leader.stateContext.parameter > context.parameter ? 2 : 1;
    }
    if (request == 1) {
        context.routeFlags |= 0x2000800U;
        if (context.owner->node && leader.node) {
            Vector3 head{};
            GetMotionBonePosition(leader.motionWork, MB_Eyes, &head);
            UpdateMotionWorkWithPosition(context.motionWork, &head, 0);
        }
    } else if (request == 2) {
        leader.stateContext.routeFlags |= 0x2000800U;
        if (context.owner->node && leader.node) {
            Vector3 head{};
            GetMotionBonePosition(context.motionWork, MB_Eyes, &head);
            UpdateMotionWorkWithPosition(leader.motionWork, &head, 0);
        }
    } else if (context.linkedRoute.leashDistance * 0.5f <= distance)
        return;
    if (request != 1 && (context.routeFlags & 0x2000000U) && !(context.owner->flags & 0x10U)) {
        context.routeFlags &= ~0x2000000U;
        if (context.owner->node)
            UpdateMotionWorkWithPosition(context.motionWork, nullptr, 0);
        SetNpcRouteMotionState(context, 3);
    }
    if (request != 2 && (leader.stateContext.routeFlags & 0x2000000U) && !(leader.flags & 0x10U)) {
        leader.stateContext.routeFlags &= ~0x2000000U;
        if (leader.node)
            UpdateMotionWorkWithPosition(leader.motionWork, nullptr, 0);
        SetNpcRouteMotionState(leader.stateContext, 3);
    }
}

// Restarts avoidance, the motion profile and stride, then takes a deferred script run-mode change.
void RestartNpcAreaRouteContext(NpcRouteStateContext &context) {
    context.routeFlags |= 0x40U;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    const bool profileWasReady = (context.flags & 0x40U) != 0;
    InitializeNpcLocomotionProfile(context);
    if (profileWasReady && (context.flags & 2U))
        context.stride = ComputeNpcRouteStride(context);
    if (context.routeFlags & 0x1000U)
        RefreshNpcScriptRouteMotion(context);
    context.flags &= 0xff9efbffU;
}

void InitializeStreamedNpcRoute(NpcRouteLiveEntry &entry, bool warm) {
    entry.flags = (entry.flags & ~3U) | NpcRouteStreamLive;
    entry.node = FindCharacterByScenarioName(entry.name);
    entry.motionWork = FindMotionWork(entry.node);
    npcSlotFlags[entry.poolIndex] |= 1U;
    entry.streamRadius = 0.30000001f;
    NpcRouteStateContext &context = entry.stateContext;
    context.motionWork = entry.motionWork;
    entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
    if (entry.eyeControlProfile)
        HandleNpcEyeControl(entry);
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        if (!(entry.flags & 0x200U)) {
            if ((npcSchedule.floor != -1000 && (entry.packedMoveFlags & 0xff00U)) || (entry.flags & 0x2000U))
                control->flags &= ~CharacterControlApplyGravity;
            if (entry.flags & 0x8000000U)
                control->flags = (control->flags & ~0x804U) | 0x800U;
        }
    }
    if (entry.flags & 0x800U)
        SetCharacterType(entry.node, 3);
    if (warm)
        return;
    if (entry.flags & 0xa000U)
        entry.localPosition.y = entry.forcedY;
    else if (!(entry.flags & 0x1000000U))
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
    SetCharacterWorldRotation(entry.node, rotation);
    EV_mtwkSetEnabled(entry.motionWork, true);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    if (entry.name.name == SHEN_NAME(REN_))
        SetMotionTimeRelated(entry.motionWork, 1, 0, 0xa204);
    entry.flags &= ~0x1000000U;
    if (entry.flags & 8U) {
        if (entry.liveState == 1) {
            entry.streamRadius = 0.34999999f;
            if (context.routeFlags & 0x10U) {
                RestartNpcAreaRouteContext(context);
                RequestNpcRouteMotion(context, false);
                return;
            }
        }
        mtwkRequestMotion(entry.motionWork, SelectNpcIdleMotion(entry), MotionRegion::LowerBody);
        return;
    }
    switch (entry.liveState) {
    case 1:
        entry.streamRadius = 0.34999999f;
        RestartNpcAreaRouteContext(context);
        RequestNpcRouteMotion(context, false);
        return;
    case 3:
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        SetCharacterWorldRotation(entry.node, rotation);
        if (!(entry.flags & 0x400U))
            break;
        mtwkSetRequestedRootMode(entry.motionWork, 0);
        ResumeNpcTimedMotion(entry);
        return;
    case 4:
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CharacterControlApplyGravity | CharacterControlEvaluateMotion);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        SetCharacterType(entry.node, 3);
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        return;
    case 6:
        entry.currentMotionId = SelectNpcSeatedIdleMotion(entry);
        break;
    case 7:
        SetMotionDrawOverrideData(entry.motionWork, nullptr);
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CharacterControlEvaluateMotion | CharacterControlVoxelSweep);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        if (entry.flags & 0x40000U) {
            const std::uint32_t frames = npcMotionClips[entry.clipOverrideIndex].frameCount;
            if (frames)
                entry.animationFrame =
                    static_cast<float>((npcFrameCount + 2 * static_cast<std::int8_t>(entry.recordBase[8])) % static_cast<std::int32_t>(frames));
        }
        SetNpcHumanVisibilityChannels(entry, false);
        return;
    case 8: {
        if (entry.streamCost == 1) {
            SetMotionDrawOverrideData(entry.motionWork, nullptr);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~(CharacterControlEvaluateMotion | CharacterControlVoxelSweep);
            return;
        }
        NpcAreaRouteState &state = *GetNpcAreaPointRouteState(entry.currentAreaPoint);
        if (entry.specialAnimationContext)
            entry.currentMotionId = ResolveNpcAreaAnimation(state, entry, false);
        if (!entry.specialAnimationContext)
            entry.currentMotionId = ResolveNpcAreaAnimation(state, entry, false);
        if (!entry.currentMotionId)
            entry.currentMotionId = SelectNpcIdleMotion(entry);
        ApplyNpcAreaMotion(entry, entry.currentMotionId);
        if (((entry.routeMotionState - 16) & 0xfb) == 0) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags |= CharacterControlStairMode;
            entry.commandFlags |= 0x800U;
        }
        return;
    }
    default:
        entry.currentMotionId = SelectNpcIdleMotion(entry);
        break;
    }
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
}

std::int64_t NpcRouteScheduleCallback(NpcRouteLiveEntry *entry) {
    NpcRouteStateContext &context = entry->stateContext;
    if (!(context.flags & 1U))
        return 0;
    if ((context.flags & 0x300U) || context.otherName.name)
        return 1;
    const std::uint32_t opcode = ReadU32(entry->currentCommand);
    switch (opcode) {
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 18:
    case 19:
    case 32:
    case 40:
        if (context.flags & 0x400U) {
            if (context.flags & 0x1000U)
                return 0;
            SetNpcRouteMotionState(context, 2);
            return 1;
        }
        if (opcode != 4 && opcode != 18)
            ReleaseNpcRouteControl(&context, {}, ReadNpcEulerYaw(entry->euler));
        return 0;
    default:
        return -1;
    }
}

bool BeginNpcAreaRoute(NpcRouteLiveEntry &entry, std::uint16_t targetPoint, float authoredScale) {
    if (!GetNpcAreaPoint(entry.currentAreaPoint) || !GetNpcAreaPoint(targetPoint))
        return false;
    InitializeNpcRouteRequest(entry.stateContext, entry.currentAreaPoint, targetPoint);
    if (entry.flags & 0x40U)
        entry.stateContext.routeFlags &= ~6U;
    entry.stateContext.scenePosition = entry.localPosition;
    entry.stateContext.localPosition = NpcAreaPosition(entry.localPosition);
    entry.stateContext.savedEuler = entry.euler;
    entry.stateContext.moveFlags = entry.packedMoveFlags;
    entry.motionSpeed = authoredScale;
    entry.flags &= ~0x8000U;
    SetNpcLocomotionScale(entry.stateContext, authoredScale);
    entry.pendingAreaPoint = targetPoint;
    entry.stateCallback = NpcRouteScheduleCallback;
    entry.liveState = 1;
    entry.scheduleProgress = 3;
    const std::uint32_t initial = ReadU32(entry.recordBase + 0x0c);
    entry.routeDirectionMode = static_cast<std::int8_t>(targetPoint == initial ? 4 : 2);
    return true;
}
} // namespace npc_package_detail

void TickNpcMotionClip(NpcRouteLiveEntry &entry) {
    const NpcMotionClip &clip = npcMotionClips[entry.clipOverrideIndex];
    if (!clip.frames)
        return;
    entry.animationFrame += HLib::GetScaledDeltaTick();
    if (entry.animationFrame >= static_cast<float>(clip.frameCount))
        entry.animationFrame -= static_cast<float>(clip.frameCount);
    if (!entry.node)
        return;
    const bool visible = camCtrlIsInsideDepthScaledRect(entry.localPosition, entry.node->radius);
    entry.node->renderFlags = static_cast<std::uint8_t>((entry.node->renderFlags & ~CRF_InRange) | (visible ? 0x10U : 0U));
    const Vector3 viewPosition = TransformNpcPoint(entry.localPosition);
    entry.node->viewPosition = viewPosition;
    const float magnitude = std::sqrt(viewPosition.x * viewPosition.x + viewPosition.y * viewPosition.y + viewPosition.z * viewPosition.z);
    entry.node->drawDistance = magnitude < 0.0000099999997f ? 0.0f : magnitude;
    SetMotionDrawOverrideData(entry.motionWork, clip.frames + static_cast<std::int32_t>(entry.animationFrame) * clip.boneCount);
}

bool SetNpcMotionAttachment(NpcRouteLiveEntry &entry, std::uint16_t motion, std::uint8_t flags, std::int32_t slot) {
    if (!motion)
        return false;
    NpcMotionAttachment &attachment = entry.motionAttachments[slot];
    attachment.motion = motion;
    attachment.flags = (attachment.flags & 0xffc1U) | (flags & 0x3eU);
    std::uint8_t remaining = flags & 0x1eU;
    for (std::int32_t index = 0; index < slot; ++index) {
        const NpcMotionAttachment &prior = entry.motionAttachments[index];
        if (prior.motion)
            remaining &= ~(prior.flags & 0x1eU);
    }
    return remaining != 0;
}

void ClearNpcMotionAttachment(NpcRouteLiveEntry &entry, std::int32_t slot) {
    if (slot < 3)
        entry.motionAttachments[slot].flags = entry.motionAttachments[slot].motion = 0;
}
bool FindNpcIntersectionInScene(Vector3 &position, std::uint32_t moveFlags) {
    constexpr float maximum = (std::numeric_limits<float>::max)();
    constexpr float epsilon = (std::numeric_limits<float>::epsilon)();
    const float savedY = position.y;
    std::uint8_t count = static_cast<std::uint8_t>(moveFlags);
    if (npcSchedule.floor == -1000) {
        const float bound = TransformClippedPoint(position, position.y) + 1.8f;
        if (bound != maximum)
            position.y = bound;
        position.y = shenFindIntersectionWithScene(position, bound);
    } else {
        std::int32_t floor = static_cast<std::int16_t>(moveFlags) >> 8;
        if (!floor)
            floor = count ? count : 1;
        if (npcSchedule.floor == floor || (moveFlags & 0x10000U)) {
            const float bound = (GetQronFloorHeight(floor) + 3.5999999f) - 0.1f;
            position.y = bound;
            position.y = FindQronIntersectionHeight(position, bound, floor);
        } else {
            position.y = maximum;
        }
        count = 0;
    }
    if (position.y == maximum || position.y == -maximum || std::abs(maximum - position.y) < epsilon || std::abs(position.y + maximum) < epsilon) {
        position.y = savedY;
        return false;
    }
    if (count >= 2 && count <= 10) {
        float height = position.y;
        for (std::int32_t floor = 1; floor < count; ++floor) {
            const float previous = position.y;
            const float ceiling = previous + 10.0f;
            while (ceiling > position.y) {
                position.y += 1.0f;
                const float candidate = shenFindIntersectionWithScene(position, position.y);
                if (candidate > previous) {
                    height = candidate;
                    break;
                }
            }
        }
        position.y = height;
    }
    return true;
}
std::uint32_t QueryNpcMoveObstruction(NpcRouteLiveEntry &entry, const Vector3 &target, bool door, std::int16_t angle, float probeRadius, float probeLength) {
    Vector3 origin = entry.localPosition;
    std::int32_t yaw;
    float radius;
    float length;
    if (door) {
        origin = NpcRouteAreaToScene(entry.stateContext.savedSplinePosition);
        yaw = entry.stateContext.targetYaw;
        radius = 0.44999999f;
        length = 1.5f;
    } else {
        const float x = target.x - entry.worldPosition.x;
        const float z = target.z - entry.worldPosition.z;
        if (entry.streamRadius > std::abs(x) && entry.streamRadius > std::abs(z))
            return 0;
        yaw = static_cast<std::int16_t>(FixedAtan2(-x, -z));
        radius = probeRadius;
        length = (probeLength < 0.0f ? std::sqrt(x * x + z * z) : probeLength) + entry.streamRadius;
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetViewMatrixFromEuler(-static_cast<std::int16_t>(yaw), 0, 0);
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    std::uint32_t result = 0;
    if (TestNpcForwardCylinder(npcPlayerPosition, angle, npcPlayerRadius + radius, npcPlayerRadius + length))
        result = 1;
    if (!result && npcRegistryTask.areaCount) {
        for (const NpcNoSpawnArea &area : npcRegistryTask.areas) {
            if (area.name.name && TestNpcForwardCylinder(area.center, angle, area.radius + radius, area.radius + length)) {
                result = 1;
                break;
            }
        }
    }
    if (!result) {
        for (NpcRouteLiveEntry *other = npcCrowdHead; other && other != npcCrowdBookmark; other = other->next) {
            if (!other->node || other == &entry || (other->flags & 0x40U) || std::abs(other->localPosition.y - entry.localPosition.y) > 1.8f ||
                (door && (other->areaPointPrevious || other->areaPointNext)))
                continue;
            if (TestNpcForwardCylinder(other->streamCenter, angle, other->streamRadius + radius, other->streamRadius + length)) {
                result = (other->stateContext.flags & 1U) | 2U;
                break;
            }
        }
    }
    Matrix4x4::Pop(1);
    return result;
}
Vector3 NpcRouteAreaToScene(Vector3 position) {
    position.x -= npcSchedule.areaOrigin.x;
    position.z -= npcSchedule.areaOrigin.z;
    return position;
}
void SetNpcRouteMotionState(NpcRouteStateContext &context, std::int32_t operation, bool explicitRequest) {
    if (!(context.flags & 1U))
        return;
    switch (operation) {
    case 0:
        if (!(context.flags & 0x10U))
            return;
        context.flags &= ~0x50U;
        InitializeNpcLocomotionProfile(context);
        break;
    case 1:
        if ((static_cast<std::uint8_t>(context.owner->recordBase[0x0b]) & 0x20U) || (context.flags & 0x10U))
            return;
        context.flags = (context.flags & ~0x50U) | 0x10U;
        InitializeNpcLocomotionProfile(context);
        break;
    case 2:
        if (!(context.flags & 0x1000U))
            context.routeFlags |= 0x800U;
        if ((context.owner->flags & 0x40U) && context.owner->node) {
            CopyMotionTransition(context.motionWork, nullptr, 0, 1, 0);
            CopyMotionTransition(context.motionWork, nullptr, static_cast<std::uint16_t>(ReadNpcEulerYaw(context.savedEuler)), 13, 0);
            CopyMotionTransition(context.motionWork, nullptr, 0, 2, 0);
        }
        return;
    case 3:
        if (context.routeFlags & 0x2000000U)
            return;
        if (!(context.flags & 0x100000U)) {
            context.flags &= ~0x1000U;
            context.routeFlags &= ~0x800U;
        }
        break;
    default:
        return;
    }
    RequestNpcRouteMotion(context, explicitRequest);
}

void TickNpcSpecialRouteContext(NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    if ((context.flags & 0x1400U) != 0x400U)
        return;
    if (npcSlotFlags[entry.poolIndex])
        context.routeFlags &= ~0x10000U;
    else
        context.routeFlags |= 0x10000U;
    TickNpcRouteStateContext(&context);
}

void ResumeNpcScheduleMotion(NpcRouteLiveEntry &entry) {
    std::int32_t phase = static_cast<std::int32_t>(npcFrameCount) + 2 * static_cast<std::int8_t>(entry.recordBase[8]);
    if (GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody))
        phase += 20;
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
    const MotnSequence &sequence = *FindLoadedMotionSequence(entry.currentMotionId);
    const float frame = static_cast<float>(phase % (sequence.frameCount - 1));
    SetMotionRegionEvalTime(entry.motionWork, frame, MotionRegion::LowerBody);
    Vector3 track;
    SampleNativeMotionRoot(entry.currentMotionId, GetCharacterBoneId(entry.node), frame, track);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(entry.localPosition.x, entry.localPosition.y, entry.localPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::preTranslate(track.x, 0.0f, track.z);
    track = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    shenApplyTranslationToAll(&entry.node->entity.node, track);
    SetCharacterWorldRotation(entry.node, rotation);
    if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
        control->flags &= ~(CharacterControlApplyGravity | CharacterControlEvaluateMotion);
    }
}



bool TickNpcRouteStateContext(NpcRouteStateContext *state) {
    npcActiveRouteContext = state;
    NpcRouteStateContext &context = *state;
    NpcRouteLiveEntry &owner = *context.owner;
    if (!(context.flags & 1U))
        return true;
    Character *const character = owner.node;
    context.motionWork = owner.motionWork;
    if (character) {
        context.scenePosition = GetCharacterWorldPosition(character);
        if (context.routeFlags & 0x40U) {
            if (owner.flags & 0xa000U)
                context.scenePosition.y = owner.forcedY;
            else
                FindNpcIntersectionInScene(context.scenePosition, context.moveFlags);
            shenApplyTranslationToAll(&character->entity.node, context.scenePosition);
            context.routeFlags &= ~0x40U;
        }
        context.localPosition = NpcAreaPosition(context.scenePosition);
        if ((context.flags & 3U) && !(context.flags & 0x101008U) && context.moveState != 3) {
            if (!context.stuckCounter++)
                context.stuckPosition = context.localPosition;
            else {
                const float x = context.localPosition.x - context.stuckPosition.x;
                const float z = context.localPosition.z - context.stuckPosition.z;
                if (std::sqrt(x * x + z * z) <= 0.2f) {
                    if (static_cast<std::int16_t>(++context.stuckCounter) > 300) {
                        context.stuckCounter = 0;
                        RequestNpcStreamOut(owner, 2);
                        owner.streamingCountdown = static_cast<std::uint16_t>((owner.streamingCountdown & 0xff00U) | 10U);
                    }
                } else
                    context.stuckCounter = 0;
            }
        } else
            context.stuckCounter = 0;
    }
    if ((context.routeFlags & 0x100000U) && !(context.routeFlags & 0x400000U)) {
        const bool searching = plymIsSearchStartRequested();
        if (context.field036 != searching) {
            if (context.flags & 0x40000U) {
                context.flags &= ~0x40000U;
                SetNpcRouteMotionState(context, 3);
            } else {
                context.routeFlags |= 0x800U;
                context.flags |= 0x40000U;
            }
        }
        context.field036 = searching;
    }
    if ((context.flags & 0x100000U) && !(context.flags & 0x1000U) && !(context.routeFlags & 0x800U)) {
        context.flags &= ~0x100000U;
        SetNpcRouteMotionState(context, 3);
    }
    if (context.routeFlags & 0x1000000U)
        UpdateLinkedNpcRoute(context);
    switch (context.phase) {
    case 0:
        EnterNpcRoute(context);
        break;
    case 1:
        BuildNpcRoute(context);
        break;
    case 2:
        BindNpcRouteWindow(context);
        break;
    case 4:
        TickNpcPausedRoute(context);
        break;
    case 5:
        TickNpcDoorEgress(context);
        break;
    case 6:
        TickNpcRouteApproach(context);
        break;
    case 7:
        TickNpcRouteTraversal(context);
        break;
    case 8:
        TickNpcDoorIngress(context);
        break;
    case 9:
        return FinishNpcRoute(context);
    default:
        break;
    }
    context.scenePosition = NpcRouteAreaToScene(context.localPosition);
    if (context.routeFlags & 0x80000U) {
        if (character)
            shenApplyTranslationToAll(&character->entity.node, context.scenePosition);
        context.routeFlags &= ~0x80000U;
    }
    owner.localPosition = context.scenePosition;
    owner.worldPosition = context.localPosition;
    owner.euler.y = context.savedEuler.y;
    owner.packedMoveFlags = context.moveFlags;
    return false;
}

bool ReleaseNpcRouteControl(NpcRouteStateContext *context, shen_name character, std::int32_t yaw) {
    if (!context)
        context = FindNpcRouteStateContext(character);
    if (!context)
        return false;
    NpcRouteLiveEntry &owner = *context->owner;
    if (context->otherName.name) {
        ReleaseDoor(static_cast<std::int32_t>(context->otherName.name), context->scriptOwner);
        context->otherName = {};
    }
    if (yaw >= 0)
        std::memcpy(&context->savedEuler.y, &yaw, sizeof(yaw));
    if (owner.node) {
        if (CharacterControlModel *const control = FindCharacterControlModel(owner.node)) {
            std::uint32_t flags = (owner.flags & 0x8000000U) ? control->flags | 0x800U : control->flags & ~0x800U;
            if (context->flags & 0x200000U) {
                if (npcSchedule.floor == -1000 || !(context->moveFlags & 0xff00U))
                    flags |= 2U;
                flags |= 4U;
            }
            control->flags = flags;
        }
        std::int32_t savedYaw;
        std::memcpy(&savedYaw, &context->savedEuler.y, sizeof(savedYaw));
        CopyMotionTransition(context->motionWork, nullptr, static_cast<std::uint16_t>(savedYaw), 13, 0);
        CopyMotionTransition(context->motionWork, nullptr, 0, 2, 0);
        if (context->flags & 0x20U) {
            UpdateMotionWorkWithPosition(context->motionWork, nullptr, 0);
            context->flags &= ~0x20U;
        }
    }
    if (context->routeFlags & 0x10U) {
        const std::byte *const point = GetNpcAreaPoint(owner.currentAreaPoint);
        if (point) {
            const std::uint32_t area = ReadU32(point) & 0xffffffU;
            if (area == 0x4c4f47U || area == 0x4d4f44U)
                owner.flags |= 0x100000U;
        }
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != owner.name.name)
                continue;
            entry.name = SHEN_NAME(MOVE);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            break;
        }
    }
    context->flags = 0;
    context->routeFlags = 0;
    context->previousSegment = -10;
    context->doorWaitFrames = -1;
    context->phase = 0;
    context->otherName = {};
    context->laneStep = 0.0f;
    context->field0f4 = 0;
    context->avoidanceWaitFrames = 0;
    context->other = nullptr;
    context->field0a1 = 0;
    context->avoidancePhase = 0;
    context->stuckCounter = 0;
    context->avoidanceResult = 0;
    context->avoidanceAuxiliary = -1;
    return true;
}

void RebuildNpcRouteCollision(bool reloading) {
    if (npcSchedule.bytes.empty())
        return;
    npcPositionQueryMode = 0;
    npcSchedule.floor = static_cast<std::int32_t>(GetQronNextStream());
    npcSchedule.areaOrigin = {};
    const auto areaPrefix = static_cast<unsigned char>(g_currentMap.name & 0xffU);
    for (std::size_t index = 0; index < 8; ++index) {
        if (static_cast<unsigned char>(npcSchedule.bytes[0x20 + index]) != areaPrefix)
            continue;
        npcSchedule.areaOrigin.x = ReadF32(npcSchedule.bytes.data() + 0x28 + index * 8);
        npcSchedule.areaOrigin.z = ReadF32(npcSchedule.bytes.data() + 0x2c + index * 8);
        break;
    }
    if (!(npcSchedule.routeFlags & 1U)) {
        RebuildNpcRouteGround();
        RebuildNpcRouteLanes(reloading ? 8 : 2);
    }
    RebuildNpcAreaPointGround();
    SetNpcAreaRouteOrigin(npcSchedule.areaOrigin);
    ActivateNpcAreaRoutes(true);
}

std::int32_t SampleNpcRoute(shen_name character, std::int32_t parameter, Vector3 *position, std::int32_t *yaw) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source)
        return 0;
    NpcRouteStateContext context = *source;
    if (parameter < context.lowParameter)
        parameter = context.lowParameter;
    if (parameter > context.highParameter)
        parameter = context.highParameter;
    context.parameter = static_cast<std::uint16_t>(parameter);
    Vector3 current;
    Vector3 next;
    SampleNpcRouteWindow(context, &current, false);
    context.parameter = static_cast<std::uint16_t>(context.parameter + context.stride);
    const std::int32_t result = SampleNpcRouteWindow(context, &next, true);
    *yaw = NpcRouteYaw(next.x - current.x, next.z - current.z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(current.x, current.y, current.z);
    Matrix4x4::SetViewMatrixFromEuler(*yaw, 0, 0);
    *position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    position->x -= npcSchedule.areaOrigin.x;
    position->z -= npcSchedule.areaOrigin.z;
    return result;
}

std::int32_t FindNearestNpcRouteParameter(shen_name character, const Vector3 *position) {
    NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    if (!context || (context->flags & 3U) == 0)
        return -1;
    return FindNearestNpcParameter(*context, NpcAreaPosition(*position));
}

float GetNpcRouteSegmentLength(shen_name character, std::int32_t parameter) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source || (source->flags & 3U) != 3U)
        return -1.0f;
    NpcRouteStateContext context = *source;
    UpdateNpcRouteWindow(context, parameter);
    return ReadNpcRouteSegmentLength(context) * 0.015625f;
}

bool GetNpcRouteLaneBounds(shen_name character, std::int32_t parameter, float *low, float *high) {
    const NpcRouteStateContext *const source = FindNpcRouteStateContext(character);
    if (!source || (source->flags & 3U) != 3U)
        return false;
    NpcRouteStateContext context = *source;
    UpdateNpcRouteWindow(context, parameter);
    const NpcRouteNode &node = context.stateTable[context.activeNode & 0x7fff];
    const std::byte *bounds = ResolveNpcRouteData(context, node.laneBoundsOffset) + context.activeSegment * 8;
    float left = ReadF32(bounds + (context.activeNode < 0 ? 4 : 0));
    float right = ReadF32(bounds + (context.activeNode < 0 ? 0 : 4));
    left = -left;
    if (context.crossingPoint == 2 || context.crossingPoint == 3) {
        const NpcRouteNode &next = context.stateTable[context.nextNode & 0x7fff];
        const std::int32_t segment = context.nextNode < 0 ? static_cast<std::int32_t>(next.pointCount) - 4 : 0;
        bounds = ResolveNpcRouteData(context, next.laneBoundsOffset) + segment * 8;
        right = std::fmin(right, ReadF32(bounds + (context.nextNode < 0 ? 0 : 4)));
        left = std::fmax(left, -ReadF32(bounds + (context.nextNode < 0 ? 4 : 0)));
    }
    *low = left;
    *high = right;
    return true;
}

// Caller-owned scripted route admission; the six MOVE entries belong to NPCC.
bool BindNpcScriptRoute(shen_name character, const NpcRouteNode *route, const Vector3 *origin, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindActiveNpcEntry(character);
    Vector3 position{};
    if (entry) {
        position = entry->localPosition;
    } else {
        for (std::size_t i = npcAuthoredEntryCount; i < npcRouteEntries.size(); ++i) {
            NpcRouteLiveEntry &candidate = npcRouteEntries[i];
            if (candidate.name.name != character.name)
                continue;
            candidate.name = NPC_SCRIPT_ROUTE_SLOT;
            candidate.node = nullptr;
            candidate.motionWork = nullptr;
            break;
        }
        for (std::size_t i = npcAuthoredEntryCount; i < npcRouteEntries.size(); ++i) {
            if (npcRouteEntries[i].name.name != NPC_SCRIPT_ROUTE_SLOT.name)
                continue;
            entry = &npcRouteEntries[i];
            entry->name = character;
            break;
        }
        if (!entry)
            return false;
    }
    NpcRouteStateContext &context = entry->stateContext;
    context.currentAreaPoint = 0;
    context.pendingAreaPoint = 1;
    context.otherName = {};
    context.routeFlags = 0;
    context.phase = 0;
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.previousSegment = -10;
    context.doorWaitFrames = -1;
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.stateTable = route;
    context.targetPosition = *origin;
    context.arrivalYaw = static_cast<std::uint16_t>(yaw);
    context.field10a = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) >> 16U);
    context.scriptOwner = character;
    context.flags = 1;
    context.routeFlags = 32950;
    context.moveFlags = route->flags | (entry->packedMoveFlags & 0x1ff00U);
    Character *const node = FindCharacterByScenarioName(character);
    if (node) {
        const Vector3i rotation = GetCharacterWorldRotation(node);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        position = GetCharacterWorldPosition(node);
        entry->motionWork = FindMotionWork(node);
        context.motionWork = entry->motionWork;
    }
    if (std::fabs(position.x + 1300.0f) < 0.00000011920929f && std::fabs(position.z - 1000.0f) < 0.00000011920929f)
        position = *origin;
    context.scenePosition = position;
    context.localPosition = NpcAreaPosition(position);
    entry->node = node;
    context.motionScale = 1.0f;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    return true;
}

bool StartNpcScriptAreaRoute(shen_name character, shen_name oldPoint, shen_name newPoint, std::int32_t yaw) {
    NpcRouteLiveEntry *entry = FindActiveNpcEntry(character);
    Vector3 position{};
    if (entry)
        position = entry->localPosition;
    else {
        NpcRouteLiveEntry *slot = nullptr;
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &candidate = npcRouteEntries[index];
            if (candidate.name.name != character.name)
                continue;
            candidate.name = NPC_SCRIPT_ROUTE_SLOT;
            candidate.node = nullptr;
            candidate.motionWork = nullptr;
            break;
        }
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            if (npcRouteEntries[index].name.name != NPC_SCRIPT_ROUTE_SLOT.name)
                continue;
            slot = &npcRouteEntries[index];
            slot->name = character;
            break;
        }
        if (!slot)
            return false;
        entry = FindActiveNpcEntry(character);
    }
    NpcRouteStateContext &context = entry->stateContext;
    entry->liveState = 1;
    entry->scheduleProgress = 3;
    context.previousSegment = -10;
    context.doorWaitFrames = -1;
    context.routeFlags = 0;
    context.phase = 0;
    context.otherName = {};
    context.laneStep = 0.0f;
    context.field0f4 = 0;
    context.avoidanceWaitFrames = 0;
    context.other = nullptr;
    context.field0a1 = 0;
    context.avoidancePhase = 0;
    context.stuckCounter = 0;
    context.stateTable = reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    context.avoidanceResult = 0;
    context.avoidanceAuxiliary = -1;
    context.currentAreaPoint = 0;
    context.pendingAreaPoint = 0;
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        const std::uint32_t pointName = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(index)));
        if (!pointName)
            break;
        if (pointName == oldPoint.name) {
            context.currentAreaPoint = static_cast<std::uint16_t>(index);
            break;
        }
    }
    for (std::size_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        const std::uint32_t pointName = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(index)));
        if (!pointName)
            break;
        if (pointName == newPoint.name) {
            context.pendingAreaPoint = static_cast<std::uint16_t>(index);
            break;
        }
    }
    context.flags = (context.flags & 0x20002U) | 1U;
    context.scriptOwner = character;
    context.routeFlags |= 0x16U;
    if (yaw >= 0) {
        context.arrivalYaw = static_cast<std::uint16_t>(yaw);
        context.field10a = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) >> 16U);
        context.routeFlags |= 0x20000U;
    }
    Character *const node = FindCharacterByScenarioName(character);
    if (node) {
        const Vector3i rotation = GetCharacterWorldRotation(node);
        std::memcpy(&context.savedEuler, &rotation, sizeof(rotation));
        position = GetCharacterWorldPosition(node);
        entry->motionWork = FindMotionWork(node);
        context.motionWork = entry->motionWork;
    }
    if (std::fabs(position.x + 1300.0f) < 0.00000011920929f && std::fabs(position.z - 1000.0f) < 0.00000011920929f) {
        std::memcpy(&position, GetNpcAreaPoint(context.currentAreaPoint) + 0x34, sizeof(position));
        position.x -= npcSchedule.areaOrigin.x;
        position.z -= npcSchedule.areaOrigin.z;
    }
    context.scenePosition = position;
    context.localPosition = NpcAreaPosition(position);
    entry->node = node;
    context.motionScale = 1.0f;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.field036 = 1;
    entry->currentAreaPoint = context.currentAreaPoint;
    entry->pendingAreaPoint = context.pendingAreaPoint;
    return true;
}

void SetNpcScriptRouteFlags(NpcRouteStateContext *context, shen_name character, std::int32_t operation, std::uint32_t flags) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    if (!(context->flags & 1U))
        return;
    if (operation)
        context->routeFlags |= flags;
    else
        context->routeFlags &= ~flags;
}

void SetNpcScriptRouteSpeed(NpcRouteStateContext *context, shen_name character, float speed) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    SetNpcLocomotionScale(*context, speed);
}

std::int64_t SetNpcRouteYaw(shen_name character, std::int32_t yaw) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        std::memcpy(&entry.euler.y, &yaw, sizeof(yaw));
        return 1;
    }
    return 0;
}
std::int64_t PlaceNpcAtAreaPoint(shen_name character, shen_name areaPoint, const Vector3 *position, std::int32_t yaw) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (areaPoint.name) {
            std::size_t point = 0;
            while (point < npcSchedule.areaPointCount) {
                const std::uint32_t name = ReadU32(GetNpcAreaPoint(static_cast<std::uint16_t>(point)));
                if (name == areaPoint.name)
                    break;
                if (!name)
                    return 0;
                ++point;
            }
            if (point == npcSchedule.areaPointCount)
                return 0;
            entry.currentAreaPoint = static_cast<std::uint16_t>(point);
        }
        std::memcpy(&entry.euler.y, &yaw, sizeof(yaw));
        entry.scheduleProgress = 3;
        entry.liveState = 0;
        RestoreNpcAreaPointSeedPosition(entry);
        FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
        entry.worldPosition = NpcAreaPosition(entry.localPosition);
        if (entry.stateContext.flags & 1U)
            ReleaseNpcRouteControl(&entry.stateContext, {}, yaw);
        if (!areaPoint.name) {
            entry.localPosition = position ? *position : Vector3{};
            entry.worldPosition = NpcAreaPosition(entry.localPosition);
        }
        if (entry.node) {
            CopyMotionTransition(entry.motionWork, nullptr, 0, 1, 0);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            SetMotionRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
            mtwkSetExtraFlag(entry.motionWork, 14, 0);
            entry.currentMotionId = SelectNpcIdleMotion(entry);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
        }
        return 1;
    }
    return 0;
}
} // namespace shenmue::s2
