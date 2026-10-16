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
static_assert(sizeof(ClothVertex) == 24);
static_assert(sizeof(ClothNeighbor) == 24);
static_assert(sizeof(ClothSimulationNode) == 0x80);
static_assert(offsetof(ClothSimulationNode, current) == 0x10);
static_assert(offsetof(ClothSimulationNode, above) == 0x20);
static_assert(offsetof(ClothSimulationNode, right) == 0x68);
static_assert(sizeof(ClothCollisionSphere) == 20);
static_assert(sizeof(ClothRenderVertexLink) == 0x10);
static_assert(sizeof(ClothRenderLink) == 0x68);
static_assert(offsetof(ClothRenderLink, binding) == 0x38);
static_assert(offsetof(ClothRenderLink, links) == 0x60);
static_assert(sizeof(ClothChain) == 0xb8);
static_assert(offsetof(ClothChain, nodes) == 0x68);
static_assert(offsetof(ClothChain, rowRestLengths) == 0x84);
static_assert(offsetof(ClothChain, owner) == 0xa8);
namespace {

constexpr std::array<std::int16_t, 9> kClothNodeKinds{-71, -72, -73, -74, -75, -76, -77, -78, -70};
constexpr Matrix4x4 kClothBindTransform{{
    {-1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.999364972114563f, 0.03563399985432625f, 0.0f},
    {0.0f, 0.03563399985432625f, -0.999364972114563f, 0.0f},
    {6.800000190734863f, 1.1124000549316406f, 4.599999904632568f, 1.0f},
}};

float ClothDistanceSquared(const Vector3 &a, const Vector3 &b) {
    const float x = a.x - b.x;
    const float y = a.y - b.y;
    const float z = a.z - b.z;
    return (y * y + x * x) + z * z;
}
float ClothDistance(const Vector3 &a, const Vector3 &b) {
    return std::sqrt(ClothDistanceSquared(a, b));
}

template <typename T> T *AllocateClothBuffer(std::size_t count, std::uint32_t token) {
    auto *const result = static_cast<T *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(sizeof(T) * count), token));
    HLib_EnableTokenCheck(result);
    return result;
}
void FreeClothBuffer(void *data) {
    if (!data)
        return;
    HLib_DisableTokenCheck(data);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(data), 0);
}

std::byte *ClothBinding(Mt5RuntimeNode *model, std::byte **base = nullptr) {
    return static_cast<std::byte *>(MT5_SceneBindingForNode(model, base));
}
ClothVertex *ClothBindingVertices(Mt5RuntimeNode *model) {
    std::byte *base = nullptr;
    std::byte *const binding = ClothBinding(model, &base);
    std::int32_t offset{};
    if (binding)
        std::memcpy(&offset, binding + 4, sizeof(offset));
    return offset ? reinterpret_cast<ClothVertex *>(base + offset) : nullptr;
}
void SetClothBindingVertices(Mt5RuntimeNode *model, std::byte *binding, const ClothVertex *vertices) {
    std::byte *base = nullptr;
    ClothBinding(model, &base);
    const auto offset = vertices ? reinterpret_cast<const std::byte *>(vertices) - base : 0;
    const auto relative = static_cast<std::int32_t>(offset);
    std::memcpy(binding + 4, &relative, sizeof(relative));
}
ClothChain *ClothChainForNode(Mt5RuntimeNode *model) {
    return static_cast<ClothChain *>(HMDL_CycleDataForNode(model));
}

const Matrix4x4 *ClothMotionNodeTransform(const ClothModelWork &work, std::uint8_t nodeId) {
    auto *node = work.motion ? static_cast<MotionNodeRuntime *>(work.motion->previousNodes) : nullptr;
    for (; node && node->nodeId != 0xff; ++node) {
        if (node->nodeId == nodeId)
            return node->transform;
    }
    return nullptr;
}
void ReadClothNodeWorldPosition(const ClothModelWork &work, std::uint8_t nodeId, Vector3 &position) {
    if (const Matrix4x4 *const matrix = ClothMotionNodeTransform(work, nodeId)) {
        position = {matrix->rows[3].x, matrix->rows[3].y, matrix->rows[3].z};
    }
}
void TransformClothVertices(ClothVertex *vertices, std::uint32_t count, const Matrix4x4 &transform) {
    for (std::uint32_t index = 0; index < count; ++index) {
        vertices[index].position = TransformHandPoint(transform, vertices[index].position);
        vertices[index].normal = TransformHandPoint(transform, vertices[index].normal);
    }
}
Matrix4x4 ClothPoseTransform() {
    const Matrix4x4 current = workTransformMatrix;
    PushCharacterMatrix();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::Multiply(&current);
    const Matrix4x4 result = workTransformMatrix;
    PopCharacterMatrix();
    return result;
}

void InitializeClothGrid(ClothChain &chain) {
    std::array<float, 182> heights;
    std::array<std::int16_t, 184> indices;
    const auto count = static_cast<std::uint8_t>(chain.vertexCount);
    for (std::uint8_t index = 0; index < count; ++index) {
        heights[index] = chain.current[index].position.y;
        indices[index] = index;
    }
    for (std::uint8_t first = 0; first < count - 1; ++first) {
        for (std::uint8_t index = count - 1; index > first; --index) {
            if (heights[index] > heights[index - 1]) {
                std::swap(heights[index], heights[index - 1]);
                std::swap(indices[index], indices[index - 1]);
            }
        }
    }
    std::uint8_t lastRow = 0;
    std::array<std::uint8_t, 9> rowCounts;
    float tolerance = 0.05f;
    for (;;) {
        rowCounts.fill(0);
        lastRow = 0;
        float previous = heights[0];
        for (std::uint8_t index = 0; index < count; ++index) {
            if (std::fabs(heights[index] - previous) > tolerance)
                ++lastRow;
            previous = heights[index];
            ++rowCounts[lastRow];
        }
        bool irregular = false;
        for (std::uint8_t row = 0; row <= lastRow; ++row) {
            if (rowCounts[row] > 20 || (row && rowCounts[row] != rowCounts[row - 1]))
                irregular = true;
        }
        if (!irregular || tolerance != 0.05f)
            break;
        tolerance = 0.03f;
    }
    chain.rows = lastRow + 1;
    chain.columns = rowCounts[0];
    std::uint8_t cursor = 0;
    bool useZ = false;
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        float maxX = -10000.0f, maxZ = -10000.0f;
        float minX = 10000.0f, minZ = 10000.0f;
        std::uint8_t minXIndex{}, minZIndex{};
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            const Vector3 &point = chain.current[indices[cursor]].position;
            if (point.x > maxX)
                maxX = point.x;
            if (minX > point.x) {
                minX = point.x;
                minXIndex = cursor;
            }
            if (point.z > maxZ)
                maxZ = point.z;
            if (minZ > point.z) {
                minZ = point.z;
                minZIndex = cursor;
            }
            ++cursor;
        }
        if (!row)
            useZ = (maxX - minX) <= (maxZ - minZ);
        const std::uint8_t first = useZ ? minZIndex : minXIndex;
        chain.nodes[row * chain.columns].vertexIndex = indices[first];
        chain.nodes[row * chain.columns + 1].left.vertexIndex = indices[first];
        indices[first] = -1;
    }
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        for (std::uint8_t column = 1; column < chain.columns; ++column) {
            ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
            ClothSimulationNode &left = chain.nodes[row * chain.columns + column - 1];
            const Vector3 &from = chain.current[left.vertexIndex].position;
            float nearest = 10.0f;
            std::uint8_t selected = 0xff;
            for (std::uint8_t candidate = 0; candidate < chain.columns; ++candidate) {
                const std::uint8_t index = row * chain.columns + candidate;
                if (indices[index] == -1)
                    continue;
                const Vector3 &to = chain.current[indices[index]].position;
                const float distance = ClothDistanceSquared(from, to);
                if (nearest <= distance)
                    continue;
                if (row) {
                    const Vector3 &aboveLeft = chain.current[chain.nodes[(row - 1) * chain.columns + column - 1].vertexIndex].position;
                    const Vector3 &above = chain.current[chain.nodes[(row - 1) * chain.columns + column].vertexIndex].position;
                    const float dot =
                        ((above.x - aboveLeft.x) * (to.x - from.x) + (above.y - aboveLeft.y) * (to.y - from.y)) + (above.z - aboveLeft.z) * (to.z - from.z);
                    if (dot < 0.0f)
                        continue;
                }
                nearest = distance;
                selected = index;
            }
            const float distance = std::sqrt(nearest);
            node.vertexIndex = indices[selected];
            node.left.restLength = distance;
            left.right.vertexIndex = indices[selected];
            left.right.restLength = distance;
            if (column < chain.columns - 1)
                chain.nodes[row * chain.columns + column + 1].left.vertexIndex = indices[selected];
            indices[selected] = -1;
        }
    }
    if (chain.nodeKind == -75 || chain.nodeKind == -76 || chain.nodeKind == -77 || chain.nodeKind == -78 || chain.nodeKind == -70) {
        for (std::uint8_t row = 0; row < chain.rows; ++row) {
            ClothSimulationNode &first = chain.nodes[row * chain.columns];
            ClothSimulationNode &last = chain.nodes[row * chain.columns + chain.columns - 1];
            const float distance = ClothDistance(chain.current[first.vertexIndex].position, chain.current[last.vertexIndex].position);
            first.left.vertexIndex = last.vertexIndex;
            first.left.restLength = distance;
            last.right.vertexIndex = first.vertexIndex;
            last.right.restLength = distance;
        }
    }
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
            const Vector3 &point = chain.current[node.vertexIndex].position;
            if (row) {
                ClothSimulationNode &above = chain.nodes[(row - 1) * chain.columns + column];
                above.below.vertexIndex = node.vertexIndex;
                above.below.restLength = ClothDistance(point, chain.current[above.vertexIndex].position);
            }
            if (row < chain.rows - 1) {
                ClothSimulationNode &below = chain.nodes[(row + 1) * chain.columns + column];
                below.above.vertexIndex = node.vertexIndex;
                below.above.restLength = ClothDistance(point, chain.current[below.vertexIndex].position);
            }
        }
    }
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
            node.current = chain.current + node.vertexIndex;
            node.previous = chain.previous + node.vertexIndex;
            for (ClothNeighbor *neighbor : {&node.above, &node.below, &node.left, &node.right}) {
                neighbor->current = chain.current + neighbor->vertexIndex;
                neighbor->previous = chain.previous + neighbor->vertexIndex;
            }
        }
    }
}

void ConfigureClothPins(ClothChain &chain) {
    for (std::uint8_t column = 0; column < chain.columns; ++column)
        chain.nodes[column].pinFlags = 0x10;
    const ClothPreset &preset = kClothPresets[chain.owner->presetIndex];
    switch (preset.options[0]) {
    case 1:
        for (std::uint8_t row = 1; row < chain.rows; ++row)
            chain.nodes[row * chain.columns].pinFlags = (row - 1) | 0x20;
        break;
    case 2:
        for (std::uint8_t row = 1; row < chain.rows; ++row)
            chain.nodes[(row + 1) * chain.columns - 1].pinFlags = (row - 1) | 0x20;
        break;
    case 3:
        for (std::uint8_t row = 1, seam = 0; row <= 4; ++row) {
            chain.nodes[row * chain.columns].pinFlags = seam++ | 0x20;
            chain.nodes[(row + 1) * chain.columns - 1].pinFlags = seam++ | 0x20;
        }
        break;
    case 4:
        for (std::uint8_t row = 1; row < chain.rows - 1; ++row)
            chain.nodes[(row + 1) * chain.columns - 1].pinFlags = (row - 1) | 0x20;
        break;
    case 5:
        for (std::uint8_t row = 1; row < chain.rows; ++row) {
            if (chain.nodeKind == -74)
                chain.nodes[row * chain.columns + 3].pinFlags = row | 0x20;
            else if (chain.nodeKind == -73)
                chain.nodes[row * chain.columns].pinFlags = row | 0x20;
        }
        break;
    case 6:
        for (std::uint8_t row = 1; row < chain.rows; ++row) {
            if (chain.nodeKind == -74)
                chain.nodes[(row + 1) * chain.columns - 1].pinFlags = row | 0x20;
            else if (chain.nodeKind == -73)
                chain.nodes[row * chain.columns].pinFlags = row | 0x20;
        }
        break;
    case 7:
        if (chain.nodeKind != -70) {
            for (std::uint8_t row = 1; row < chain.rows; ++row)
                chain.nodes[(row + 1) * chain.columns - 1].pinFlags = (row - 1) | 0x20;
        }
        break;
    case 8:
        if (chain.nodeKind == -74 || chain.nodeKind == -73) {
            chain.nodes[chain.columns].pinFlags = 0x20;
            chain.nodes[2 * chain.columns - 1].pinFlags = 0x21;
            chain.nodes[2 * chain.columns].pinFlags = 0x22;
            chain.nodes[3 * chain.columns - 1].pinFlags = 0x23;
        } else {
            for (std::uint8_t row = 1; row < chain.rows - 1; ++row)
                chain.nodes[(row + 1) * chain.columns - 1].pinFlags = (row + 4) | 0x20;
        }
        break;
    }
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        std::int16_t pivot = preset.options[1];
        switch (pivot) {
        case -5:
            if (chain.nodeKind == -73)
                pivot = 1;
            else if (chain.nodeKind == -74)
                pivot = chain.columns - 1;
            break;
        case -4:
            pivot = chain.nodeKind == -70 ? 6 : chain.columns >> 1;
            break;
        case -3:
            if (chain.nodeKind == -73)
                pivot = 0;
            else if (chain.nodeKind == -74)
                pivot = 3;
            break;
        case -2:
            pivot = chain.columns >> 1;
            break;
        case -1:
            pivot = chain.columns - 1;
            break;
        }
        chain.pivotColumns[row] = pivot;
    }
    switch (chain.nodeKind) {
    case -78:
        chain.collisionMask = 0xc000;
        break;
    case -77:
        chain.collisionMask = 0x1800;
        break;
    case -76:
        chain.collisionMask = 0x80;
        break;
    case -75:
        chain.collisionMask = 0x10;
        break;
    default:
        chain.collisionMask = 0xffff;
        break;
    }
}

void InitializeClothRenderLink(ClothChain &chain) {
    ClothRenderLink *const link = chain.renderLink;
    if (!link)
        return;
    TransformClothVertices(link->current, link->vertexCount, kClothBindTransform);
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            const std::uint32_t index = row * chain.columns + column;
            const Vector3 &source = chain.nodes[index].current->position;
            float nearest = 10000.0f;
            std::uint8_t selected;
            ClothVertex *vertex = nullptr;
            for (std::uint8_t candidate = 0; candidate < static_cast<std::uint8_t>(chain.vertexCount); ++candidate) {
                const float distance = ClothDistanceSquared(source, link->current[candidate].position);
                if (nearest > distance) {
                    nearest = distance;
                    selected = candidate;
                    vertex = link->current + candidate;
                }
            }
            link->links[index].vertexIndex = selected;
            link->links[index].vertex = vertex;
        }
    }
}
void UpdateClothRenderLink(ClothChain &chain) {
    ClothRenderLink *const link = chain.renderLink;
    if (!link)
        return;
    for (std::uint8_t row = 0; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            const std::uint32_t index = row * chain.columns + column;
            const ClothVertex &source = *chain.nodes[index].current;
            ClothVertex &destination = *link->links[index].vertex;
            destination.position = source.position;
            destination.normal = {-source.normal.x, -source.normal.y, -source.normal.z};
        }
    }
}
void UpdateClothNormals(ClothChain &chain) {
    for (std::uint8_t row = 1; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
            const bool useRight = node.left.vertexIndex == -1;
            const Vector3 &side = (useRight ? node.right.current : node.left.current)->position;
            ClothVertex &current = *node.current;
            const ClothVertex &above = *node.above.current;
            Vector3 normal{
                (current.position.y - side.y) * (above.position.z - current.position.z) -
                    (current.position.z - side.z) * (above.position.y - current.position.y),
                (current.position.z - side.z) * (above.position.x - current.position.x) -
                    (current.position.x - side.x) * (above.position.z - current.position.z),
                (current.position.x - side.x) * (above.position.y - current.position.y) -
                    (current.position.y - side.y) * (above.position.x - current.position.x),
            };
            if (useRight)
                normal = {-normal.x, -normal.y, -normal.z};
            if (((above.normal.x - above.position.x) * normal.x + (above.normal.y - above.position.y) * normal.y) +
                    (above.normal.z - above.position.z) * normal.z <
                0.0f)
                normal = {-normal.x, -normal.y, -normal.z};
            const float length = std::sqrt((normal.x * normal.x + normal.y * normal.y) + normal.z * normal.z);
            current.normal = {current.position.x + normal.x / length, current.position.y + normal.y / length, current.position.z + normal.z / length};
        }
    }
}

constexpr float kClothMode4RowEnvelope[]{
    0.0f,
    0.20000000298023224f,
    0.5f,
    0.800000011920929f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
};

constexpr float kClothMode9RowEnvelope[]{
    0.0f,
    0.20000000298023224f,
    0.4000000059604645f,
    0.800000011920929f,
    0.8999999761581421f,
    1.0f,
};

std::uint32_t g_clothNoisePhase{};
std::uint32_t g_clothWavePhase{};
std::uint32_t g_clothWavePhaseStep = 3082;
float g_clothWaveAmplitude = 0.064f;
float g_clothWindNoise{};
float g_clothNoisePhaseSine{};
std::uint32_t g_clothWindYaw{};

// @verified - p
float CalculateClothWindGust(std::uint16_t phase) {
    float gust = sin_fixed16(static_cast<std::int16_t>(22 * phase)) * 0.3f;
    gust += sin_fixed16(static_cast<std::int16_t>(31 * phase)) * 0.37f;
    gust += sin_fixed16(static_cast<std::int16_t>(103 * phase)) * 0.15f;
    return gust + sin_fixed16(static_cast<std::int16_t>(147 * phase)) * 0.18f;
}

void UpdateClothWind(ClothModelWork &work) {
    if ((work.flags & 6U) != 0U || (work.setupMode != 4 && work.setupMode != 7 && work.setupMode != 9))
        return;
    g_clothNoisePhaseSine = sin_fixed16(static_cast<std::int16_t>(g_clothNoisePhase));
    if (work.setupMode == 9) {
        g_clothWavePhaseStep = 5226;
        g_clothWaveAmplitude = 0.044f;
    }
    g_clothNoisePhase += 23;
    g_clothWavePhase += g_clothWavePhaseStep;
    g_clothWindNoise = 0.053f * CalculateClothWindGust(static_cast<std::uint16_t>(g_clothNoisePhase));
    g_clothWindYaw = work.character ? static_cast<std::uint32_t>(work.character->entity.node.rotation.y) : 0;
    const auto yaw = static_cast<std::uint16_t>(g_clothWindYaw);
    const auto folded = static_cast<std::uint16_t>(yaw <= 0x8000U ? yaw : -yaw);
    work.wind = {
        sin_fixed16(static_cast<std::int16_t>(yaw)) * (0.04f + g_clothWindNoise),
        0.0f,
        sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * (0.04f + g_clothWindNoise),
    };
}
Vector3 ConstrainClothDistance(const Vector3 &point, const Vector3 &anchor, float length) {
    const float distance = ClothDistance(anchor, point);
    if (std::fabs(distance) < 0.00000011920929f)
        return anchor;
    return {(length / distance) * (point.x - anchor.x) + anchor.x, (length / distance) * (point.y - anchor.y) + anchor.y,
        (length / distance) * (point.z - anchor.z) + anchor.z};
}
Vector3 ClothGravityDirection(const Vector3 &target, const Vector3 &root) {
    const float distance = ClothDistance(target, root);
    if (std::fabs(distance) < 0.00000011920929f)
        return {};
    return {(0.054444447f / distance) * (target.x - root.x), (0.054444447f / distance) * (target.y - root.y), (0.054444447f / distance) * (target.z - root.z)};
}
void SimulateClothChain(ClothChain &chain) {
    ClothModelWork &work = *chain.owner;
    const ClothPreset &preset = kClothPresets[work.presetIndex];
    Vector3 gravity{}, secondaryGravity{};
    if (work.setupMode == 1 || work.setupMode == 2 || work.setupMode == 5 || preset.options[2] == 4 ||
        (work.imageName == make_shen_name('N', 'A', 'T', ' ') && chain.nodeKind != -70)) {
        gravity = {0.0f, -0.054444447f, 0.0f};
    } else {
        Vector3 target{}, root{}, other{};
        switch (chain.nodeKind) {
        case -78:
            ReadClothNodeWorldPosition(work, 27, target);
            ReadClothNodeWorldPosition(work, 0, root);
            break;
        case -77:
            ReadClothNodeWorldPosition(work, 34, target);
            ReadClothNodeWorldPosition(work, 0, root);
            break;
        case -76:
            ReadClothNodeWorldPosition(work, 12, target);
            ReadClothNodeWorldPosition(work, 10, root);
            break;
        case -75:
            ReadClothNodeWorldPosition(work, 18, target);
            ReadClothNodeWorldPosition(work, 16, root);
            break;
        default:
            switch (preset.options[2]) {
            case 2:
                ReadClothNodeWorldPosition(work, 31, target);
                ReadClothNodeWorldPosition(work, 24, other);
                break;
            case 3:
                ReadClothNodeWorldPosition(work, 32, target);
                ReadClothNodeWorldPosition(work, 25, other);
                break;
            case 4:
                ReadClothNodeWorldPosition(work, 0, target);
                ReadClothNodeWorldPosition(work, 0, other);
                target.y -= 2.0f;
                other.y -= 2.0f;
                break;
            default:
                ReadClothNodeWorldPosition(work, 34, target);
                ReadClothNodeWorldPosition(work, 27, other);
                break;
            }
            target = {(other.x - target.x) * 0.5f + target.x, (other.y - target.y) * 0.5f + target.y, (other.z - target.z) * 0.5f + target.z};
            ReadClothNodeWorldPosition(work, 0, root);
            break;
        }
        gravity = ClothGravityDirection(target, root);
        switch (preset.options[3]) {
        case 1:
            ReadClothNodeWorldPosition(work, 34, target);
            ReadClothNodeWorldPosition(work, 27, other);
            break;
        case 2:
            ReadClothNodeWorldPosition(work, 31, target);
            ReadClothNodeWorldPosition(work, 24, other);
            break;
        case 3:
            ReadClothNodeWorldPosition(work, 32, target);
            ReadClothNodeWorldPosition(work, 25, other);
            break;
        }
        target = {(other.x - target.x) * 0.5f + target.x, (other.y - target.y) * 0.5f + target.y, (other.z - target.z) * 0.5f + target.z};
        secondaryGravity = ClothGravityDirection(target, root);
    }
    if (ClothDistance(chain.nodes[0].current->position, chain.nodes[0].previous->position) > 1.8f)
        chain.flags |= 0x40;
    const bool closed = chain.nodeKind == -75 || chain.nodeKind == -76 || chain.nodeKind == -77 || chain.nodeKind == -78 || chain.nodeKind == -70;
    for (std::uint8_t row = 1; row < chain.rows; ++row) {
        for (std::uint8_t column = 0; column < chain.columns; ++column) {
            ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
            if ((node.pinFlags & 0xf0U) == 0x10U)
                continue;
            const Vector3 &above = node.above.current->position;
            const Vector3 &priorAbove = node.above.previous->position;
            Vector3 inertia{above.x - priorAbove.x, above.y - priorAbove.y, above.z - priorAbove.z};
            if (work.setupMode == 4) {
                const float scale = kClothMode4WindProfile[20 * row + column];
                inertia.x += work.wind.x * scale;
                inertia.y += work.wind.y * scale;
                inertia.z += work.wind.z * scale;
            } else if (work.setupMode == 9 && (chain.nodeKind == -71 || chain.nodeKind == -72)) {
                const float scale = chain.nodeKind == -71 ? kClothMode9RowEnvelope[row] * 0.2f : kClothMode9RowEnvelope[row];
                inertia.x += work.wind.x * scale;
                inertia.y += work.wind.y * scale;
                inertia.z += work.wind.z * scale;
            }
            if (work.setupMode != 8 && work.setupMode != 7 && work.setupMode != 9 && work.imageName != make_shen_name('S', 'A', 'K', ' ') &&
                work.imageName != make_shen_name('K', 'H', 'I', ' ') && node.collisionIndex == -1 && !node.groundContact) {
                if (chain.nodeKind == -70) {
                    inertia.y *= 0.5f;
                } else {
                    switch (preset.options[5]) {
                    case 1:
                        inertia.x *= 0.8f;
                        inertia.y *= 0.65f;
                        inertia.z *= 0.8f;
                        break;
                    case 2:
                        inertia.x *= 0.2f;
                        inertia.y *= 0.1f;
                        inertia.z *= 0.2f;
                        break;
                    case 3:
                        break;
                    default:
                        inertia.x *= 0.5f;
                        inertia.y *= 0.5f;
                        inertia.z *= 0.5f;
                        break;
                    }
                }
            }
            const Vector3 *origin = &node.previous->position;
            if ((chain.flags & 0x40U) != 0U) {
                inertia = {};
                origin = &above;
            }
            const Vector3 &force = preset.options[3] && row >= static_cast<std::uint8_t>(preset.options[4]) &&
                    (work.imageName != make_shen_name('N', 'A', 'T', ' ') || chain.nodeKind == -70) ?
                secondaryGravity :
                gravity;
            Vector3 predicted{(origin->x + inertia.x) + force.x, (origin->y + inertia.y) + force.y, (origin->z + inertia.z) + force.z};
            predicted = ConstrainClothDistance(predicted, above, node.above.restLength);
            node.collisionIndex = -1;
            for (std::uint8_t index = 0; index < work.nodeCount; ++index) {
                const ClothCollisionSphere &sphere = chain.collisionSpheres[index];
                if ((sphere.collisionMask & chain.collisionMask) == 0)
                    continue;
                const float radiusSquared = sphere.radius * sphere.radius;
                if (radiusSquared < ClothDistanceSquared(predicted, sphere.center))
                    continue;
                node.collisionIndex = static_cast<std::int8_t>(index);
                const Vector3 ray{node.current->normal.x - predicted.x, node.current->normal.y - predicted.y, node.current->normal.z - predicted.z};
                const Vector3 relative{predicted.x - sphere.center.x, predicted.y - sphere.center.y, predicted.z - sphere.center.z};
                const float quadratic = (ray.x * ray.x + ray.y * ray.y) + ray.z * ray.z;
                const float linear = ((ray.x * relative.x + ray.y * relative.y) + ray.z * relative.z) * 2.0f;
                const float discriminant =
                    linear * linear - (4.0f * quadratic) * (((relative.x * relative.x + relative.y * relative.y) + relative.z * relative.z) - radiusSquared);
                if (discriminant >= 0.0f) {
                    const float root = std::sqrt(discriminant);
                    float distance = (root - linear) / (2.0f * quadratic);
                    if (distance < 0.0f)
                        distance = (-linear - root) / (2.0f * quadratic);
                    predicted.x += distance * ray.x;
                    predicted.y += distance * ray.y;
                    predicted.z += distance * ray.z;
                }
                predicted = ConstrainClothDistance(predicted, above, node.above.restLength);
            }
            node.current->position = predicted;
            if (row >= 2 && !node.groundContact) {
                const Vector3 &twiceAbove = chain.current[chain.nodes[(row - 2) * chain.columns + column].vertexIndex].position;
                const Vector3 midpoint{(predicted.x - twiceAbove.x) * 0.5f + twiceAbove.x, (predicted.y - twiceAbove.y) * 0.5f + twiceAbove.y,
                    (predicted.z - twiceAbove.z) * 0.5f + twiceAbove.z};
                node.above.current->position = {
                    (above.x - midpoint.x) * 0.5f + midpoint.x, (above.y - midpoint.y) * 0.5f + midpoint.y, (above.z - midpoint.z) * 0.5f + midpoint.z};
            }
        }
        if (closed) {
            float length = 0.0f;
            for (std::int16_t column = 0; column < chain.columns; ++column) {
                const ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
                length += ClothDistance(node.current->position, node.right.current->position);
            }
            length /= chain.columns;
            if (length > work.maximumRowLength * 2.0f)
                length = work.maximumRowLength * 2.0f;
            chain.rowRestLengths[row] = length;
            for (std::uint8_t column = 0; column < chain.columns; ++column) {
                ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
                if (node.groundContact)
                    continue;
                const float leftLength = work.setupMode ? node.left.restLength :
                    preset.options[5] == 3              ? node.left.restLength * 0.3f :
                                                          chain.rowRestLengths[row];
                const Vector3 left = ConstrainClothDistance(node.current->position, node.left.current->position, leftLength);
                const Vector3 right =
                    ConstrainClothDistance(node.current->position, node.right.current->position, work.setupMode ? node.right.restLength : leftLength);
                node.current->position = {(left.x + right.x) * 0.5f, (left.y + right.y) * 0.5f, (left.z + right.z) * 0.5f};
            }
        } else {
            for (std::int16_t column = chain.pivotColumns[row] - 1; column >= 0; --column) {
                ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
                if (node.right.vertexIndex != -1 && (node.pinFlags & 0xf0U) != 0x10U)
                    node.current->position = ConstrainClothDistance(node.current->position, node.right.current->position, node.right.restLength);
            }
            for (std::int16_t column = chain.pivotColumns[row] + 1; column < chain.columns; ++column) {
                ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
                if (node.left.vertexIndex != -1 && (node.pinFlags & 0xf0U) != 0x10U)
                    node.current->position = ConstrainClothDistance(node.current->position, node.left.current->position, node.left.restLength);
            }
        }
    }
    if (work.setupMode == 4) {
        for (std::uint8_t row = 1; row < chain.rows; ++row) {
            for (std::uint8_t sweep = 0; sweep < 2; ++sweep) {
                std::uint8_t column = sweep ? 13 : 15;
                for (std::uint32_t step = 0; step < static_cast<std::uint32_t>(chain.columns >> 1); ++step) {
                    ClothVertex &vertex = *chain.nodes[row * chain.columns + column].current;
                    const float wave = sin_fixed16(static_cast<std::int16_t>(g_clothWavePhase - step * 16544));
                    const float y = ((g_clothWaveAmplitude * 0.5f) * (vertex.normal.y - vertex.position.y)) * wave;
                    const float z = (g_clothWaveAmplitude * (vertex.normal.z - vertex.position.z)) * wave;
                    vertex.position.x += (((g_clothWaveAmplitude * (vertex.normal.x - vertex.position.x)) * wave) * kClothMode4RowEnvelope[row]) *
                        kClothMode4ColumnEnvelope[column];
                    vertex.position.y += (y * kClothMode4RowEnvelope[row]) * kClothMode4ColumnEnvelope[column];
                    vertex.position.z += (z * kClothMode4RowEnvelope[row]) * kClothMode4ColumnEnvelope[column];
                    if (sweep)
                        --column;
                    else if (++column >= chain.columns)
                        column = 0;
                }
            }
        }
    } else if (work.setupMode == 7 || work.setupMode == 9) {
        for (std::uint8_t row = 1; row < chain.rows; ++row) {
            float rowScale = 0.0f;
            if (work.setupMode == 9) {
                rowScale = kClothMode9RowEnvelope[row];
                if (chain.nodeKind == -71)
                    rowScale *= 0.3f;
            }
            for (std::uint8_t column = 0; column < chain.columns; ++column) {
                ClothVertex &vertex = *chain.nodes[row * chain.columns + column].current;
                const float wave = sin_fixed16(static_cast<std::int16_t>(g_clothWavePhase - column * 16544));
                const float scale = work.setupMode == 7 ? kClothMode7WindProfile[row * 10 + column] : rowScale;
                const float z = (g_clothWaveAmplitude * (vertex.normal.z - vertex.position.z)) * wave;
                vertex.position.x += ((g_clothWaveAmplitude * (vertex.normal.x - vertex.position.x)) * wave) * scale;
                vertex.position.z += z * scale;
            }
        }
    }
    chain.flags &= static_cast<std::uint8_t>(~0x40U);
    if ((work.flags & 8U) == 0U) {
        for (std::uint8_t row = 1; row < chain.rows; ++row) {
            for (std::uint8_t column = 0; column < chain.columns; ++column) {
                ClothSimulationNode &node = chain.nodes[row * chain.columns + column];
                const float height = mapUpdateIntersectionStateAtLocalPoint(node.current->position);
                const float y = chain.nodeKind == -70 ? height - 0.02f : height + 0.02f;
                if (y > node.current->position.y) {
                    node.current->position.y = y;
                    node.groundContact = 1;
                }
            }
        }
    }
}

void JoinClothSeams(ClothChain &first, ClothChain &second) {
    for (std::uint8_t row = 1; row < first.rows; ++row) {
        for (std::uint8_t column = 0; column < first.columns; ++column) {
            ClothSimulationNode &node = first.nodes[row * first.columns + column];
            if ((node.pinFlags & 0xf0U) != 0x20U)
                continue;
            for (std::uint8_t otherRow = 1; otherRow < second.rows; ++otherRow) {
                for (std::uint8_t otherColumn = 0; otherColumn < second.columns; ++otherColumn) {
                    ClothSimulationNode &other = second.nodes[otherRow * second.columns + otherColumn];
                    if (other.pinFlags != node.pinFlags)
                        continue;
                    ClothVertex &left = *node.current;
                    ClothVertex &right = *other.current;
                    left.position = {(right.position.x - left.position.x) * 0.5f + left.position.x,
                        (right.position.y - left.position.y) * 0.5f + left.position.y, (right.position.z - left.position.z) * 0.5f + left.position.z};
                    right.position = left.position;
                    const Vector3 normal{(right.normal.x - left.normal.x) * 0.5f + left.normal.x, (right.normal.y - left.normal.y) * 0.5f + left.normal.y,
                        (right.normal.z - left.normal.z) * 0.5f + left.normal.z};
                    left.normal = ConstrainClothDistance(normal, left.position, 1.0f);
                    right.normal = left.normal;
                    if (first.renderLink) {
                        ClothVertex &target = first.renderLink->current[first.renderLink->links[row * first.columns + column].vertexIndex];
                        target.position = right.position;
                        target.normal = {-right.normal.x, -right.normal.y, -right.normal.z};
                    }
                    if (second.renderLink) {
                        ClothVertex &target = second.renderLink->current[second.renderLink->links[otherRow * second.columns + otherColumn].vertexIndex];
                        target.position = right.position;
                        target.normal = {-right.normal.x, -right.normal.y, -right.normal.z};
                    }
                    second.previous[other.vertexIndex].position = TransformHandPoint(ClothPoseTransform(), right.position);
                }
            }
        }
    }
}

bool HasClothNodes(Mt5RuntimeNode *model) {
    if (!model)
        return false;
    for (const std::int16_t kind : kClothNodeKinds)
        if (shenGetNodeByChildIdx(model, kind))
            return true;
    return false;
}

bool AllocateClothRenderLink(ClothChain &chain, Mt5RuntimeNode *model, std::int16_t nodeKind) {
    std::byte *const binding = ClothBinding(model);
    if (!binding)
        return false;
    ClothRenderLink *const link = AllocateClothBuffer<ClothRenderLink>(1, SHEN_NAME(CLUR));
    chain.renderLink = link;
    link->current = nullptr;
    link->sourceVertices = nullptr;
    link->links = nullptr;
    link->nodeKind = nodeKind;
    link->model = model;
    link->originalBinding = binding;
    std::memcpy(&link->vertexCount, binding + 8, sizeof(link->vertexCount));
    link->current = AllocateClothBuffer<ClothVertex>(link->vertexCount, SHEN_NAME(CLVB));
    link->originalVertices = ClothBindingVertices(model);
    link->sourceVertices = AllocateClothBuffer<ClothVertex>(link->vertexCount, SHEN_NAME(CLVB));
    std::memcpy(link->current, link->originalVertices, sizeof(ClothVertex) * link->vertexCount);
    std::memcpy(link->sourceVertices, link->originalVertices, sizeof(ClothVertex) * link->vertexCount);
    std::memcpy(link->binding, binding, sizeof(link->binding));
    link->links = AllocateClothBuffer<ClothRenderVertexLink>(link->vertexCount, SHEN_NAME(CLUI));
    for (std::uint32_t index = 0; index < link->vertexCount; ++index) {
        std::memset(&link->links[index], 0xff, 8);
        link->links[index].vertex = nullptr;
    }
    return true;
}

bool AllocateClothChain(ClothModelWork &work, Mt5RuntimeNode *model, std::int16_t kind) {
    auto *const chain = AllocateClothBuffer<ClothChain>(1, SHEN_NAME(CLTH));
    work.chains[work.chainCount++] = chain;
    chain->flags = 1;
    chain->rows = chain->columns = 0;
    chain->vertexCount = 0;
    chain->current = chain->previous = chain->sourceVertices = nullptr;
    chain->nodes = nullptr;
    chain->renderLink = nullptr;
    chain->owner = &work;
    chain->nodeKind = kind;
    chain->collisionMask = 0xffff;
    chain->model = model;
    HMDL_SetCycleDataForNode(model, chain);
    std::byte *const binding = ClothBinding(model);
    chain->originalBinding = binding;
    std::memcpy(chain->binding, binding, sizeof(chain->binding));
    std::memcpy(&chain->vertexCount, binding + 8, sizeof(chain->vertexCount));
    chain->nodes = AllocateClothBuffer<ClothSimulationNode>(chain->vertexCount, SHEN_NAME(CLID));
    std::fill(std::begin(chain->pivotColumns), std::end(chain->pivotColumns), -1);
    for (std::uint32_t index = 0; index < chain->vertexCount; ++index) {
        ClothSimulationNode &node = chain->nodes[index];
        node.vertexIndex = -1;
        node.collisionIndex = -1;
        node.reserved03 = 0xff;
        node.groundContact = 0;
        node.pinFlags = 0;
        node.current = node.previous = nullptr;
        for (ClothNeighbor *neighbor : {&node.above, &node.below, &node.left, &node.right}) {
            neighbor->vertexIndex = -1;
            neighbor->restLength = 0.0f;
            neighbor->current = neighbor->previous = nullptr;
        }
    }
    chain->current = AllocateClothBuffer<ClothVertex>(chain->vertexCount, SHEN_NAME(CLVB));
    chain->previous = AllocateClothBuffer<ClothVertex>(chain->vertexCount, SHEN_NAME(CLVO));
    chain->originalVertices = ClothBindingVertices(model);
    chain->sourceVertices = AllocateClothBuffer<ClothVertex>(chain->vertexCount, SHEN_NAME(CLVZ));
    std::memcpy(chain->sourceVertices, chain->originalVertices, sizeof(ClothVertex) * chain->vertexCount);
    std::int16_t renderKind = 90;
    switch (kind) {
    case -71:
        renderKind = 86;
        break;
    case -72:
        renderKind = 87;
        break;
    case -73:
        renderKind = 88;
        break;
    case -74:
        renderKind = 89;
        break;
    case -75:
        renderKind = 146;
        break;
    case -76:
        renderKind = 147;
        break;
    case -77:
        renderKind = 148;
        break;
    case -78:
        renderKind = 149;
        break;
    case -70:
        break;
    default:
        return false;
    }
    Mt5RuntimeNode *const renderModel = shenGetNodeByChildIdx(work.model, renderKind);
    return !renderModel || AllocateClothRenderLink(*chain, renderModel, renderKind);
}

void DestroyClothChains(ClothModelWork &work) {
    for (ClothChain *&chain : work.chains) {
        if (!chain)
            continue;
        FreeClothBuffer(chain->current);
        FreeClothBuffer(chain->previous);
        FreeClothBuffer(chain->sourceVertices);
        FreeClothBuffer(chain->nodes);
        HMDL_SetCycleDataForNode(chain->model, nullptr);
        if (ClothRenderLink *const link = chain->renderLink) {
            FreeClothBuffer(link->current);
            FreeClothBuffer(link->links);
            FreeClothBuffer(link->sourceVertices);
            FreeClothBuffer(link);
        }
        FreeClothBuffer(chain);
        chain = nullptr;
    }
}

bool SetupClothCollisionData(ClothModelWork &work) {
    work.presetIndex = 0;
    while (kClothPresets[work.presetIndex].imageName != 0xffffffffU && kClothPresets[work.presetIndex].imageName != work.imageName) {
        ++work.presetIndex;
    }
    switch (work.setupMode) {
    case 1:
        work.clothPresetTable = kClothMode1Spheres;
        break;
    case 2:
        work.clothPresetTable = kClothMode2Spheres;
        break;
    case 4:
        work.clothPresetTable = kClothMode4Spheres;
        break;
    case 6:
        work.clothPresetTable = kClothMode6Spheres;
        break;
    case 7:
        work.clothPresetTable = kClothMode7Spheres;
        break;
    default:
        work.clothPresetTable = kClothPresets[work.presetIndex].spheres;
        break;
    }
    work.nodeCount = 0;
    do {
        ++work.nodeCount;
    } while (work.clothPresetTable[work.nodeCount].collisionGroup != 0xff);
    FreeClothBuffer(work.clothData);
    work.clothData = AllocateClothBuffer<ClothCollisionSphere>(work.nodeCount, SHEN_NAME(CLCB));
    return true;
}


bool SetupClothChains(ClothModelWork &work) {
    work.flags = 0;
    work.chainCount = 0;
    work.wind = {};
    work.maximumRowLength = 0.0f;
    for (std::size_t index = 0; index < std::size(work.chains); ++index)
        work.chains[index] = nullptr;
    if (work.imageName != make_shen_name('C', 'H', 'A', ' ') && work.imageName != make_shen_name('S', 'Y', 'U', ' ') &&
        work.imageName != make_shen_name('K', 'O', 'K', ' '))
        work.flags |= 8;
    SetupClothCollisionData(work);
    for (const std::int16_t kind : kClothNodeKinds) {
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(work.model, kind))
            static_cast<void>(AllocateClothChain(work, node, kind));
    }
    return true;
}




bool UpdateClothMesh(ClothChain &chain) {
    ClothModelWork &work = *chain.owner;
    if ((Shenmue_nPauseState & 2) != 0 || (work.flags & 4U) != 0U)
        return true;
    const std::size_t vertexBytes = sizeof(ClothVertex) * chain.vertexCount;
    std::memcpy(chain.current, chain.originalVertices, vertexBytes);
    if ((work.flags & 2U) != 0U) {
        SetClothBindingVertices(chain.model, chain.binding, chain.originalVertices);
        std::memcpy(chain.originalVertices, chain.sourceVertices, vertexBytes);
        if (ClothRenderLink *const link = chain.renderLink) {
            SetClothBindingVertices(link->model, link->binding, link->originalVertices);
            std::memcpy(link->originalVertices, link->sourceVertices, sizeof(ClothVertex) * link->vertexCount);
        }
        return false;
    }
    if ((chain.flags & 1U) != 0U) {
        TransformClothVertices(chain.current, chain.vertexCount, kClothBindTransform);
        std::memcpy(chain.previous, chain.current, vertexBytes);
        InitializeClothGrid(chain);
        InitializeClothRenderLink(chain);
        ConfigureClothPins(chain);
        chain.flags &= static_cast<std::uint8_t>(~1U);
        chain.collisionSpheres = work.clothData;
        work.maximumRowLength = 10.0f;
        for (std::uint8_t index = 0; index < work.nodeCount; ++index)
            if (work.maximumRowLength > work.clothData[index].radius)
                work.maximumRowLength = work.clothData[index].radius;
        return false;
    }
    const Matrix4x4 transform = ClothPoseTransform();
    TransformClothVertices(chain.current, chain.vertexCount, transform);
    SimulateClothChain(chain);
    if (IMGM_GetImageName(work.character) == SHEN_NAME(MGRM))
        UpdateClothNormals(chain);
    std::memcpy(chain.previous, chain.current, vertexBytes);
    Matrix4x4 inverse = transform;
    Matrix4x4::InvertInPlace(&inverse);
    TransformClothVertices(chain.current, chain.vertexCount, inverse);
    UpdateClothRenderLink(chain);
    SetClothBindingVertices(chain.model, ClothBinding(chain.model), chain.current);
    if (ClothRenderLink *const link = chain.renderLink) {
        std::byte *base = nullptr;
        ClothBinding(link->model, &base);
        std::int32_t vertexOffset{};
        std::uint32_t vertexCount{};
        std::memcpy(&vertexOffset, link->binding + 4, sizeof(vertexOffset));
        std::memcpy(&vertexCount, link->binding + 8, sizeof(vertexCount));
        std::memcpy(base + vertexOffset, link->current, sizeof(ClothVertex) * vertexCount);
        if (std::byte *const binding = ClothBinding(link->model))
            std::memcpy(binding + 4, &vertexOffset, sizeof(vertexOffset));
    }
    chain.flags |= 0x20;
    ++work.updateFlags;
    return true;
}

void ApplyClothNodeTransform(const Mt5RuntimeNode &node) {
    if (const Matrix4x4 *const transform = HMDL_TransformForNode(&node, node.transformOffset)) {
        workTransformMatrix = *transform;
        return;
    }
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
}
void UpdateClothNodeTree(Mt5RuntimeNode *node) {
    ClothChain *const chain = ClothChainForNode(node);
    if (chain) {
        std::uint32_t identifier{};
        std::memcpy(&identifier, chain, sizeof(identifier));
        if (identifier == SHEN_NAME(OSAG))
            return;
    }
    const bool active = chain && (chain->flags & 0x20U) == 0U && chain->owner && (chain->owner->flags & 0x20U) == 0U;
    std::byte *const base = MT5_ModelBaseForNode(node);
    Mt5RuntimeNode *const sibling = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    if (sibling)
        PushCharacterMatrix();
    ApplyClothNodeTransform(*node);
    if (active) {
        ClothVertex *const originalVertices = ClothBindingVertices(node);
        SelectCharacterMapQuery(chain->owner->character);
        UpdateClothMesh(*chain);
        std::int16_t pairKind = -1;
        switch (chain->nodeKind) {
        case -74:
            pairKind = -73;
            break;
        case -73:
            pairKind = -74;
            break;
        case -72:
            pairKind = -71;
            break;
        case -71:
            pairKind = -72;
            break;
        }
        if (pairKind != -1) {
            if (Mt5RuntimeNode *const pairedModel = shenGetNodeByChildIdx(chain->owner->model, pairKind)) {
                ClothVertex *const pairedVertices = ClothBindingVertices(pairedModel);
                ClothChain *const paired = ClothChainForNode(pairedModel);
                if (paired && UpdateClothMesh(*paired)) {
                    JoinClothSeams(*chain, *paired);
                    if ((chain->owner->flags & 2U) == 0U || (chain->flags & 1U) == 0U) {
                        SetClothBindingVertices(node, ClothBinding(node), originalVertices);
                        SetClothBindingVertices(pairedModel, ClothBinding(pairedModel), pairedVertices);
                        std::uint32_t count{};
                        std::memcpy(&count, ClothBinding(node) + 8, sizeof(count));
                        if (originalVertices != chain->current)
                            std::memcpy(originalVertices, chain->current, sizeof(ClothVertex) * count);
                        std::memcpy(&count, ClothBinding(pairedModel) + 8, sizeof(count));
                        if (pairedVertices != paired->current)
                            std::memcpy(pairedVertices, paired->current, sizeof(ClothVertex) * count);
                    }
                }
            }
        }
        chain->owner->previousFlags = chain->owner->flags;
    }
    if (node->childOffset)
        UpdateClothNodeTree(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset));
    if (sibling) {
        PopCharacterMatrix();
        UpdateClothNodeTree(sibling);
    }
}
} // namespace


void UpdateCharacterClothPose(Character &character) {
    auto *const model = static_cast<Mt5RuntimeNode *>(character.model);
    MotionManagerWork *const manager = MOTM_GetWork(&character);
    if (!model || !manager)
        return;
    auto *const work = static_cast<ClothModelWork *>(HMDL_CycleDataForNode(model));
    if (work) {
        work->character = &character;
        work->motion = &manager->motion;
        work->basePoseMode = static_cast<std::uint8_t>(manager->motion.basePoseTemplateIndex);
        for (std::uint8_t index = 0; index < work->nodeCount; ++index) {
            const ClothAuthoredSphere &source = work->clothPresetTable[index];
            PushCharacterMatrix();
            Matrix4x4::SetActiveTransform(ClothMotionNodeTransform(*work, source.motionNodeId));
            ClothCollisionSphere &sphere = work->clothData[index];
            sphere.center = TransformHandPoint(workTransformMatrix, {source.x * 0.001f, source.y * 0.001f, source.z * 0.001f});
            sphere.radius = source.radius * 0.001f;
            sphere.motionNodeId = source.motionNodeId;
            sphere.collisionMask = source.collisionGroup == 0xff ? 0xffffU : static_cast<std::uint16_t>(1U << source.collisionGroup);
            PopCharacterMatrix();
        }
        manager->motion.rootAngleFlags |= 0x800U;
    }
    PushCharacterMatrix();
    workTransformMatrix = transformMatrix_prePass;
    ApplyClothNodeTransform(*model);
    if (model->childOffset) {
        std::byte *const base = MT5_ModelBaseForNode(model);
        UpdateClothNodeTree(reinterpret_cast<Mt5RuntimeNode *>(base + model->childOffset));
    }
    PopCharacterMatrix();
}
namespace {

std::uint8_t g_ryoClothMovementMode{};
bool g_ryoClothPreviouslyRunning = true;

void RebuildClothModel(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CLMD));
    auto *work = component ? reinterpret_cast<ClothModelWork *>(component + 1) : nullptr;
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!work && !HasClothNodes(model))
        return;
    if (!work) {
        work = static_cast<ClothModelWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CLMD), sizeof(ClothModelWork), CLMD_Callback));
        HLib_GetCharacterComponent(character, SHEN_NAME(CLMD))->destroyCallback = CLMD_Cleanup;
    }
    CLMD_Cleanup(character, work);
    work->imageName = (IMGM_GetImageName(character) & 0x00ffffffU) | 0x20000000U;
    work->model = model;
    HMDL_SetCycleDataForNode(model, work);
    static_cast<void>(SetupClothChains(*work));
}
} // namespace

void CLMD_Callback(Character *character, void *payload) {
    auto *const work = static_cast<ClothModelWork *>(payload);
    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    if (control->ryo == character) {
        const bool running = ShouldRun();
        if (running) {
            MotionManagerWork *const manager = MOTM_GetWork(character);
            const std::uint16_t animation = manager->motion.currentAnimId;
            constexpr std::uint16_t kClothIdleLeanAnimation = 903;
            constexpr std::uint16_t kClothIdleStandAnimation = 89;
            const bool idle = animation == kClothIdleLeanAnimation || animation == kClothIdleStandAnimation;
            if (g_ryoClothMovementMode == 0) {
                if (idle)
                    ++g_ryoClothMovementMode;
            } else if (g_ryoClothMovementMode == 1) {
                if ((manager->motion.motionFlags & 2U) != 0U) {
                    if (idle)
                        ++g_ryoClothMovementMode;
                    else
                        g_ryoClothMovementMode = 0;
                }
            } else if (g_ryoClothMovementMode == 2 && !idle) {
                g_ryoClothMovementMode = 0;
            }
            constexpr std::array<std::uint16_t, 7> kClothWalkAnimations{1374, 603, 604, 605, 607, 608, 609};
            const bool disable =
                g_ryoClothMovementMode == 2 || std::find(kClothWalkAnimations.begin(), kClothWalkAnimations.end(), animation) != kClothWalkAnimations.end();
            if (disable)
                work->flags |= 4;
            else
                work->flags &= static_cast<std::uint8_t>(~4U);
        } else if (g_ryoClothPreviouslyRunning) {
            work->flags &= static_cast<std::uint8_t>(~4U);
        }
        g_ryoClothPreviouslyRunning = running;
    }
    if ((work->flags & 4U) == 0U && ((work->previousFlags ^ work->flags) & 4U) != 0U)
        work->flags |= 0x10;
    for (ClothChain *const chain : work->chains) {
        if (!chain)
            continue;
        chain->flags &= static_cast<std::uint8_t>(~0x20U);
        if ((work->flags & 0x10U) != 0U)
            chain->flags |= 0x40;
    }
    work->flags &= static_cast<std::uint8_t>(~0x10U);
    if ((work->flags & 0x20U) != 0U) {
        --work->setupCountdown;
        if (work->setupCountdown) {
            for (ClothChain *const chain : work->chains)
                if (chain)
                    chain->flags |= 0x40;
        } else {
            RebuildClothModel(character);
            work->flags &= static_cast<std::uint8_t>(~0x20U);
        }
    }
    work->updateFlags = 0;
    UpdateClothWind(*work);
}

void CLMD_Cleanup(Character *character, void *payload) {
    auto *const work = static_cast<ClothModelWork *>(payload);
    FreeClothBuffer(work->clothData);
    work->clothData = nullptr;
    DestroyClothChains(*work);
    HMDL_SetCycleDataForNode(character->model, nullptr);
}

ClothModelWork *CLMD_Flag(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CLMD));
    if (!component)
        return nullptr;
    auto *const work = reinterpret_cast<ClothModelWork *>(component + 1);
    work->flags = static_cast<std::uint8_t>((work->flags & ~0x40U) | 0x20U);
    work->setupCountdown = 2;
    return work;
}

void CLMD_RestoreModelBindings(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CLMD));
    if (!component)
        return;
    auto *const work = reinterpret_cast<ClothModelWork *>(component + 1);
    for (ClothChain *const chain : work->chains) {
        if (!chain)
            continue;
        const auto offset = static_cast<std::int32_t>(static_cast<std::byte *>(chain->originalBinding) - MT5_ModelBaseForNode(chain->model));
        chain->model->sceneBindingOffset = offset;
        if (chain->sourceVertices) {
            if (ClothVertex *const vertices = ClothBindingVertices(chain->model))
                std::memcpy(vertices, chain->sourceVertices, sizeof(ClothVertex) * chain->vertexCount);
        }
        if (ClothRenderLink *const link = chain->renderLink) {
            const auto linkOffset = static_cast<std::int32_t>(static_cast<std::byte *>(link->originalBinding) - MT5_ModelBaseForNode(link->model));
            link->model->sceneBindingOffset = linkOffset;
            if (link->sourceVertices) {
                if (ClothVertex *const vertices = ClothBindingVertices(link->model))
                    std::memcpy(vertices, link->sourceVertices, sizeof(ClothVertex) * link->vertexCount);
            }
        }
    }
    work->flags |= 0x40;
}

void CLMD_SetFlags(Character *character, std::uint8_t flags, std::int32_t enable) {
    auto *const work = static_cast<ClothModelWork *>(HMDL_CycleDataForNode(character->model));
    if (!work)
        return;
    if (enable)
        work->flags |= flags;
    else
        work->flags &= static_cast<std::uint8_t>(~flags);
}

void CLMD_Alloc(Character *character, std::uint8_t setupMode) {
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!HasClothNodes(model))
        return;
    auto *const work = static_cast<ClothModelWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CLMD), sizeof(ClothModelWork), CLMD_Callback));
    work->imageName = (IMGM_GetImageName(character) & 0x00ffffffU) | 0x20000000U;
    work->model = model;
    work->setupMode = setupMode;
    HMDL_SetCycleDataForNode(model, work);
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control && control->ryo == character) {
        g_ryoClothMovementMode = 0;
        g_ryoClothPreviouslyRunning = true;
    }
    static_cast<void>(SetupClothChains(*work));
    SetHeapMemoryFreeCallback(character, SHEN_NAME(CLMD), CLMD_Cleanup);
}

void CLMD_Setup(Character *character, std::uint8_t setupMode) {
    if (!character || !character->model)
        return;
    auto *const work = static_cast<ClothModelWork *>(HMDL_CycleDataForNode(character->model));
    if (!work)
        return;
    work->setupMode = setupMode;
    SetupClothCollisionData(*work);
}
} // namespace shenmue::s1
