#include "s2/hand.hpp"
#include "s2/matrix_stack.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
std::int32_t gripDisabled{};
namespace {
std::int32_t gripMeshWords{};

// @verified - p
std::uint32_t CountGripMeshWords(const std::uint32_t *mesh) {
    if (mesh[0] == SHEN_NAME(XB01)) {
        const std::uint32_t *const data = mesh + static_cast<std::int32_t>(mesh[7]);
        const std::uint32_t count = data[8];
        std::uint64_t bytes = reinterpret_cast<const std::byte *>(data + 7) - reinterpret_cast<const std::byte *>(mesh) + 8ULL * count + 8;
        for (std::uint32_t index = 0; index < count; ++index)
            bytes += data[10 + 3 * index];
        return static_cast<std::uint32_t>((bytes + 8) >> 2);
    }
    if (static_cast<std::int32_t>(mesh[0]) < 0 || !mesh[6])
        return 0;
    const std::byte *cursor = reinterpret_cast<const std::byte *>(mesh + 6);
    while (*reinterpret_cast<const std::uint32_t *>(cursor))
        cursor += 88 + *reinterpret_cast<const std::int32_t *>(cursor + 84);
    return static_cast<std::uint32_t>((cursor - reinterpret_cast<const std::byte *>(mesh) + 8) >> 3);
}

// @verified - p
void TransformGripMesh(const std::uint32_t *mesh, std::uint32_t *destination) {
    if (mesh[0] == SHEN_NAME(XB01)) {
        const std::uint32_t *const data = mesh + static_cast<std::int32_t>(mesh[7]);
        for (std::uint32_t block = 0; block < data[8]; ++block) {
            const std::byte *source = reinterpret_cast<const std::byte *>(data + 11) + 12 * block;
            std::byte *output = reinterpret_cast<std::byte *>(destination + static_cast<std::int32_t>(destination[7]) + 11) + 12 * block;
            for (std::uint32_t vertex = 0; vertex < (data[10 + 3 * block] >> 5); ++vertex, source += 32, output += 32) {
                Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(output));
                *reinterpret_cast<std::uint32_t *>(output) |= 1U;
            }
        }
        return;
    }
    if (static_cast<std::int32_t>(mesh[0]) < 0)
        return;
    const std::uint32_t *source = mesh + 6;
    std::uint32_t *output = destination + 6;
    std::uint32_t stride = 8;
    while (*source) {
        const std::uint32_t command = *source;
        if (static_cast<std::int32_t>(command) < 0) {
            stride = source[11] == 0xfffffffeU ? 14 : 8;
            source += 22;
            output += 22;
            continue;
        }
        const std::uint32_t count = source[1] * ((command & 0x18U) == 8 ? 3U : 1U);
        source += 2;
        output += 2;
        for (std::uint32_t vertex = 0; vertex < count; ++vertex) {
            const std::uint32_t step = (*source & 1U) ? stride : 2;
            if (*source & 1U) {
                Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(output));
                *output |= 1U;
            }
            source += step;
            output += step;
        }
    }
}

// @verified - p
void BuildGripGeometry(const MT7Node *node) {
    while (node) {
        if (node->runtimeSibling)
            Matrix4x4_Push();
        if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(*node))
            Matrix4x4::SetActiveTransform(transform);
        else {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        }
        const MT7Node *const sourceNode = node->borrowedMesh ? node->borrowedMesh : node;
        const std::uint32_t *const mesh = reinterpret_cast<const std::uint32_t *>(sourceNode->runtimeMesh);
        if (mesh) {
            const std::uint32_t words = CountGripMeshWords(mesh);
            if (words && gripMeshWords + words < 24576) {
                std::uint32_t *const destination = static_cast<std::uint32_t *>(handCalculationWork) + gripMeshWords;
                std::memcpy(destination, mesh, 4ULL * words);
                TransformGripMesh(mesh, destination);
                gripMeshWords += words;
            }
        }
        if (node->runtimeChild)
            BuildGripGeometry(node->runtimeChild);
        if (!node->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        node = node->runtimeSibling;
    }
}

// @verified - p
void UpdateGripTriangleBounds(float *bounds, const Vector3 *first, const Vector3 *second, const Vector3 *third) {
    const HandGripProbe *const probes = static_cast<const HandGripProbe *>(handProbeWork);
    const Vector3 edges[3]{{second->x - first->x, second->y - first->y, second->z - first->z}, {third->x - first->x, third->y - first->y, third->z - first->z},
        {second->x - third->x, second->y - third->y, second->z - third->z}};
    const Vector3 *const origins[3]{first, first, third};
    const auto dot = [](const Vector3 &left, const Vector3 &right) { return (left.x * right.x + left.y * right.y) + left.z * right.z; };
    const float firstSquare = dot(edges[0], edges[0]), secondSquare = dot(edges[1], edges[1]), cross = dot(edges[0], edges[1]);
    const float determinant = cross * cross - firstSquare * secondSquare;
    for (std::int32_t group = 0; group < 5; ++group) {
        if (!(bounds[group] >= 0.0f))
            continue;
        const HandGripProbe *const pair = probes + group * 2;
        float candidates[5];
        for (std::int32_t endpoint = 0; endpoint < 2; ++endpoint) {
            Vector3 offset{pair[endpoint].position.x - first->x, pair[endpoint].position.y - first->y, pair[endpoint].position.z - first->z};
            const float firstDot = dot(offset, edges[0]), secondDot = dot(offset, edges[1]);
            float firstFactor = -1.0f, secondFactor = -1.0f;
            if (determinant >= 0.0000001 || determinant <= -0.0000001) {
                firstFactor = (secondDot * cross - firstDot * secondSquare) / determinant;
                secondFactor = (firstDot * cross - secondDot * firstSquare) / determinant;
            }
            firstFactor = std::fmin(1.0f, std::fmax(0.0f, firstFactor));
            secondFactor = std::fmin(1.0f, std::fmax(0.0f, secondFactor));
            if (firstFactor + secondFactor > 1.0f)
                firstFactor = secondFactor = 0.5f;
            offset.x -= secondFactor * edges[1].x + firstFactor * edges[0].x;
            offset.y -= secondFactor * edges[1].y + firstFactor * edges[0].y;
            offset.z -= secondFactor * edges[1].z + firstFactor * edges[0].z;
            candidates[endpoint] = dot(offset, offset) - pair[endpoint].radius * pair[endpoint].radius;
        }
        const Vector3 segment{pair[1].position.x - pair[0].position.x, pair[1].position.y - pair[0].position.y, pair[1].position.z - pair[0].position.z};
        const float segmentSquare = dot(segment, segment);
        for (std::int32_t edge = 0; edge < 3; ++edge) {
            Vector3 offset{origins[edge]->x - pair[0].position.x, origins[edge]->y - pair[0].position.y, origins[edge]->z - pair[0].position.z};
            const float segmentDot = dot(offset, segment), edgeDot = dot(edges[edge], segment), offsetDot = dot(offset, edges[edge]);
            const float edgeSquare = dot(edges[edge], edges[edge]), denominator = edgeDot * edgeDot - segmentSquare * edgeSquare;
            float edgeFactor = -1.0f, segmentFactor = -1.0f;
            if (denominator >= 0.0000001 || denominator <= -0.0000001) {
                edgeFactor = (offsetDot * segmentSquare - edgeDot * segmentDot) / denominator;
                segmentFactor = (offsetDot * edgeDot - segmentDot * edgeSquare) / denominator;
            }
            edgeFactor = std::fmin(1.0f, std::fmax(0.0f, edgeFactor));
            segmentFactor = std::fmin(1.0f, std::fmax(0.0f, segmentFactor));
            const float radius = (pair[1].radius - pair[0].radius) * segmentFactor + pair[0].radius;
            offset.x += edgeFactor * edges[edge].x - segment.x * segmentFactor;
            offset.y += edgeFactor * edges[edge].y - segment.y * segmentFactor;
            offset.z += edgeFactor * edges[edge].z - segment.z * segmentFactor;
            candidates[edge + 2] = dot(offset, offset) - radius * radius;
        }
        for (float candidate : candidates)
            if (bounds[group] > candidate)
                bounds[group] = candidate;
    }
}

// @verified - p
void IntersectGripMesh(float *bounds, const std::uint32_t *mesh) {
    if (mesh[0] == SHEN_NAME(XB01)) {
        const std::byte *const bytes = reinterpret_cast<const std::byte *>(mesh);
        const std::byte *const data = bytes + 4LL * static_cast<std::int32_t>(mesh[7]);
        const std::uint32_t *block = mesh + static_cast<std::int32_t>(mesh[6]) + 6;
        const std::byte *command = reinterpret_cast<const std::byte *>(block + 5);
        std::uint32_t selected = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data));
        while (*block) {
            if (std::to_integer<std::uint8_t>(*command) == 1)
                selected = std::to_integer<std::uint8_t>(command[8]);
            else if (std::to_integer<std::uint8_t>(*command) == 16) {
                const std::byte *const vertices = data + 4ULL * (3ULL * selected + 11);
                const std::uint16_t *const indices = reinterpret_cast<const std::uint16_t *>(command + 12);
                const std::uint32_t count = *reinterpret_cast<const std::uint16_t *>(command + 2) / 3U;
                for (std::uint32_t triangle = 0; triangle < count; ++triangle) {
                    const std::uint16_t *const index = indices + 3 * triangle;
                    UpdateGripTriangleBounds(bounds, reinterpret_cast<const Vector3 *>(vertices + 32ULL * index[0]),
                        reinterpret_cast<const Vector3 *>(vertices + 32ULL * index[1]), reinterpret_cast<const Vector3 *>(vertices + 32ULL * index[2]));
                }
            }
            command += 4ULL * (*reinterpret_cast<const std::uint32_t *>(command) >> 8);
            if (*command == std::byte{}) {
                block += *block >> 8;
                command = reinterpret_cast<const std::byte *>(block + 5);
            }
        }
        return;
    }
    if (static_cast<std::int32_t>(mesh[0]) < 0)
        return;
    const std::uint32_t *cursor = mesh + 6;
    std::uint32_t stride = 8;
    while (*cursor) {
        const std::uint32_t command = *cursor;
        if (static_cast<std::int32_t>(command) < 0) {
            stride = cursor[11] == 0xfffffffeU ? 14 : 8;
            cursor += 22;
            continue;
        }
        const std::uint32_t count = cursor[1];
        cursor += 2;
        const Vector3 *vertices[3];
        const bool triangles = (command & 0x18U) == 8;
        const std::uint32_t entries = triangles ? count * 3 : count;
        for (std::uint32_t index = 0; index < entries; ++index) {
            const Vector3 *vertex = reinterpret_cast<const Vector3 *>(cursor);
            if (*cursor & 1U)
                cursor += stride;
            else {
                vertex = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor + 2) + cursor[1]);
                cursor += 2;
            }
            if (triangles) {
                vertices[index % 3] = vertex;
                if (index % 3 == 2)
                    UpdateGripTriangleBounds(bounds, vertices[0], vertices[1], vertices[2]);
            } else {
                if (index < 2)
                    vertices[index] = vertex;
                else {
                    UpdateGripTriangleBounds(bounds, vertices[0], vertices[1], vertex);
                    vertices[0] = vertices[1];
                    vertices[1] = vertex;
                }
            }
        }
    }
}

// @verified - p
void UpdateGripBounds(HandParams &hand) {
    for (std::int32_t group = 0; group < 5; ++group)
        if (hand.boundsWork[group] >= 0.0f)
            hand.boundsWork[group] = std::numeric_limits<float>::max();
    gripMeshWords = 0;
    const std::uint32_t *const meshes = static_cast<const std::uint32_t *>(handCalculationWork);
    for (;;) {
        const std::uint32_t *const mesh = meshes + gripMeshWords;
        const std::uint32_t words = CountGripMeshWords(mesh);
        if (!words)
            break;
        if (gripMeshWords + words >= 24576)
            continue;
        IntersectGripMesh(hand.boundsWork, mesh);
        gripMeshWords += words;
    }
    for (std::int32_t group = 0; group < 5; ++group)
        if (hand.boundsWork[group] <= 0.00001)
            hand.boundsWork[group] = -1.0f;
}

} // namespace

// @verified - p
void AdvanceGripAngles(HandParams &hand, std::int32_t collide, std::int32_t flags) {
    const std::int32_t divisor = hand.poseTicks < 1 ? 1 : hand.poseTicks;
    hand.poseTicks = static_cast<std::int16_t>(hand.poseTicks - 1);
    bool finished = hand.poseTicks < 1;
    if (finished)
        hand.poseTicks = 1;
    bool unblocked = false;
    for (std::int32_t joint = 0; joint < 72; ++joint) {
        std::int16_t steps[3];
        for (std::int32_t axis = 0; axis < 3; ++axis)
            steps[axis] = static_cast<std::int16_t>(hand.targetJointAngles[joint][axis] - hand.currentJointAngles[joint][axis]) / divisor;
        const std::int16_t group = handGripJointGroups[joint];
        if (collide && group >= 0) {
            if (!(hand.boundsWork[group] < 0.0f))
                unblocked = true;
            else {
                steps[0] = steps[1] = steps[2] = 0;
                if (flags & 1)
                    finished = true;
            }
        }
        for (std::int32_t axis = 0; axis < 3; ++axis)
            hand.currentJointAngles[joint][axis] += steps[axis];
        if ((steps[0] || steps[1] || steps[2]) && !flags)
            finished = false;
    }
    if ((!flags && !unblocked && collide) || finished) {
        hand.poseMode &= 9U;
        hand.poseTicks = 0;
    }
}

namespace {
// @verified - p
void UpdateGripPose(HandParams &hand, std::int32_t flags) {
    StageHandGripProbes(hand);
    UpdateGripBounds(hand);
    hand.poseFlags &= ~8U;
    if (hand.poseTicks > 0)
        AdvanceGripAngles(hand, 1, flags);
    if (hand.poseTicks < 1)
        hand.poseFlags |= 8U;
}
} // namespace

// @verified - p
void UpdateHandGripBounds(HandParams &hand) {
    for (std::int32_t group = 0; group < 5; ++group)
        if (!(hand.boundsWork[group] < 0.0f))
            hand.boundsWork[group] = std::numeric_limits<float>::max();
    Character *const target = static_cast<Character *>(hand.secondaryPoseWork);
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3 position = GetCharacterWorldPosition(target);
    const Vector3i rotation = GetCharacterWorldRotation(target);
    Vector3 scale;
    GetCharacterScale(target, &scale);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    const MT7Model *const model = static_cast<const MT7Model *>(target->model);
    if (model) {
        gripMeshWords = 0;
        BuildGripGeometry(model->nodes.data());
        std::uint32_t *const meshes = static_cast<std::uint32_t *>(handCalculationWork);
        meshes[gripMeshWords] = 0xffffffffU;
        for (std::int32_t index = 1; index < 8; ++index)
            meshes[gripMeshWords + index] = 0;
        UpdateGripBounds(hand);
    } else {
        const Vector3 origin{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        const HandGripProbe *const probes = static_cast<const HandGripProbe *>(handProbeWork);
        for (std::int32_t group = 0; group < 5; ++group) {
            const HandGripProbe &probe = probes[group * 2];
            const Vector3 difference{origin.x - probe.position.x, origin.y - probe.position.y, origin.z - probe.position.z};
            const float radius = probe.radius + 0.0020000001f;
            const float bound = (difference.x * difference.x + difference.y * difference.y) + difference.z * difference.z - radius * radius;
            if (bound < 0.0f)
                hand.boundsWork[group] = bound;
        }
    }
    Matrix4x4::Pop(1);
}

// @verified - p
void SolveHandGripPose(
    HandParams *hand, const Matrix4x4 *base, const MT7Node *geometry, const Matrix4x4 *, std::int32_t flags, std::byte *beginCommand, std::byte *endCommand) {
    if (!hand->poseModelData || gripDisabled)
        return;
    std::int16_t target[72][3], current[72][3];
    float bounds[16];
    std::memcpy(target, hand->targetJointAngles, sizeof(target));
    std::memcpy(current, hand->currentJointAngles, sizeof(current));
    std::memcpy(bounds, hand->boundsWork, sizeof(bounds));
    const std::uint8_t poseMode = hand->poseMode, poseFlags = hand->poseFlags;
    const std::int16_t poseTicks = hand->poseTicks, currentFrame = hand->currentPoseFrame, previousFrame = hand->previousPoseFrame;
    ParseHandPoseCommand(hand, beginCommand, 1);
    std::memcpy(hand->currentJointAngles, hand->targetJointAngles, sizeof(hand->currentJointAngles));
    ParseHandPoseCommand(hand, endCommand, 100);
    std::memset(hand->wristAngles, 0, 8);
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    gripMeshWords = 0;
    BuildGripGeometry(geometry);
    std::uint32_t *const meshes = static_cast<std::uint32_t *>(handCalculationWork);
    meshes[gripMeshWords] = 0xffffffffU;
    for (std::int32_t index = 1; index < 8; ++index)
        meshes[gripMeshWords + index] = 0;
    Matrix4x4::SetActiveTransform(base);
    do
        UpdateGripPose(*hand, flags);
    while (!(hand->poseFlags & 8U));
    Matrix4x4::Pop(1);
    WriteHandPoseCommand(*hand, endCommand);
    hand->poseFlags = poseFlags;
    hand->poseMode = poseMode;
    hand->poseTicks = poseTicks;
    hand->currentPoseFrame = currentFrame;
    hand->previousPoseFrame = previousFrame;
    std::memcpy(hand->targetJointAngles, target, sizeof(target));
    std::memcpy(hand->currentJointAngles, current, sizeof(current));
    std::memcpy(hand->boundsWork, bounds, sizeof(bounds));
}
} // namespace shenmue::s2
