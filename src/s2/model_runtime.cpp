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

std::uint64_t RuntimeHandle(const d3t::ModelRuntimeState &state, std::uint32_t hash) {
    return (static_cast<std::uint64_t>(hash) << 32) | state.cacheHandle;
}


std::uint64_t FindRuntimeSkeleton(d3t::ModelRuntimeState &state, std::uint64_t node) {
    const char *const name = d3t::GetModelCacheNodeName(node);
    if (std::strstr(name, "CSESkeleton")) {
        const char *version = std::strstr(name, "VERSION");
        if (version)
            version += 7;
        else if ((version = std::strchr(name, 'V')))
            ++version;
        const std::uint32_t value = version ? static_cast<std::uint32_t>(std::strtoul(version, nullptr, 10)) : 0;
        if ((value <= 1 || std::strstr(name, state.modelName)) && (value - 1 <= 5 || value == 500)) {
            state.nodeCount = value;
            return node;
        }
    }
    d3t::ModelCacheSlot *const cache = d3t::ResolveModelCache(state.cacheHandle);
    if (!cache)
        return 0;
    const auto index = d3t::LookupModelCacheIndex(cache->indexTable, static_cast<std::uint32_t>(node >> 32));
    for (std::uint32_t child = 0; child < cache->nodes[index].childCount; ++child)
        if (const auto found = FindRuntimeSkeleton(state, d3t::GetModelCacheChild(node, child)))
            return found;
    return 0;
}

bool IsRuntimeNodeEligible(std::uint32_t version, std::int32_t kind) {

    const std::uint32_t id = static_cast<std::uint32_t>(kind);
    if ((version > 2 && id - 81U <= 1U) || (version >= 5 && id + 1339U <= 1U))
        return false;
    if (version != 500)
        return true;
    return (id <= 23 && (15163122U & (1U << id))) || id + 67U <= 2U;
}

void FormatRuntimeHandName(char *output, std::size_t size, const d3t::ModelRuntimeState &state, const d3t::HandMatrixAttachment &hand, std::uint32_t index) {
    const std::uint32_t kind = static_cast<std::uint32_t>(std::abs(hand.nodeKind));
    if (state.nodeCount == 1)
        std::snprintf(output, size, "Node_Hand_%u_%03u_%03u", kind, index, hand.matrixIndices[index]);
    else
        std::snprintf(output, size, "Node_%s_Hand_%02u_%03u_%03u", state.modelName, kind, index, hand.matrixIndices[index]);
}

void CollectHandMatrixIndices(const HandPoseModelJoint *&cursor, d3t::HandMatrixAttachment &hand) {
    const auto &joint = *cursor++;
    if (joint.id <= 19)
        hand.matrixIndices[hand.matrixCount++] = static_cast<std::uint8_t>(joint.id);
    for (std::int32_t child = 0; child < joint.childCount; ++child)
        CollectHandMatrixIndices(cursor, hand);
}

template <class Owner, std::size_t Count> Owner *AllocateRuntimeVertexOwner(Owner (&owners)[Count], MT7Node *node, std::uint32_t &index) {
    for (index = 0; index < Count; ++index)
        if (owners[index].owner.node == node)
            return &owners[index];
    for (index = 0; index < Count; ++index) {
        if (owners[index].owner.node)
            continue;
        owners[index] = {};
        owners[index].owner.node = node;
        owners[index].owner.slot = index;
        return &owners[index];
    }
    return nullptr;
}



void CollectRuntimeHand(Character &character, MT7Node &node, d3t::ModelRuntimeState &state, d3t::ModelRuntimeNode &record, std::uint32_t flags) {
    const auto kind = static_cast<std::int16_t>(node.id);
    if (kind != -66 && kind != -65 && kind != 69 && kind != 68)
        return;
    if (!node.runtimeChild && !(flags & 8U))
        return;
    const auto token = kind == -66 || kind == 69 ? kLeftHandToken : kRightHandToken;
    HandParams *const hand = static_cast<HandParams *>(FindModelComponent(&character, token));
    if (!hand || !hand->poseModelData)
        return;
    std::uint32_t slot = 0;
    while (slot < 2 && state.hands[slot].node != &node)
        ++slot;
    if (slot == 2) {
        slot = 0;
        while (slot < 2 && state.hands[slot].node)
            ++slot;
        if (slot == 2)
            return;
        state.hands[slot] = {};
        state.hands[slot].node = &node;
        state.hands[slot].slot = slot;
    }
    d3t::HandMatrixAttachment &attachment = state.hands[slot];
    attachment.hasChildren = node.runtimeChild != nullptr;
    attachment.matrices = handRelativeMatrices;
    attachment.nodeKind = kind;
    attachment.matrixCount = 0;
    const auto *cursor = reinterpret_cast<const HandPoseModelJoint *>(hand->poseModelData + 8);
    CollectHandMatrixIndices(cursor, attachment);
    attachment.enabled = 1;
    record.type = 3;
    record.attachmentIndex = slot;
}


void CollectRuntimeNodes(Character *character, MT7Node *node, d3t::ModelRuntimeState &state, std::uint32_t flags) {
    for (; node && state.recordCount < 300; node = node->runtimeSibling) {
        const auto kind = static_cast<std::int16_t>(node->id);
        if (IsRuntimeNodeEligible(state.nodeCount, kind)) {
            d3t::ModelRuntimeNode &record = state.nodes[state.recordCount++];
            record.node = node;
            record.kind = kind;
            record.type = 1;
            auto *const cloth = static_cast<ClothObject *>(node->taskData);
            if (cloth && cloth->token.name == SHEN_NAME(CLTH)) {
                auto &owners = *reinterpret_cast<RuntimeClothOwner(*)[8]>(state.clothRuntime);
                if (auto *const owner = AllocateRuntimeVertexOwner(owners, node, record.attachmentIndex)) {
                    if (!owner->owner.enabled) {
                        owner->owner.enabled = 1;
                        owner->owner.vertexCount = cloth->vertexCount;
                    }
                    if (owner->owner.enabled)
                        record.type = 2;
                }
            }
            if (static_cast<std::uint16_t>(kind - 5000) <= 9U) {
                auto &owners = *reinterpret_cast<RuntimeDynamicOwner(*)[32]>(state.dynamicModelRuntime);
                if (auto *const owner = AllocateRuntimeVertexOwner(owners, node, record.attachmentIndex)) {
                    if (!owner->owner.enabled) {
                        if (const FlagWindModel *const wind = FindFlagWindModel(node)) {
                            owner->owner.enabled = 1;
                            owner->owner.vertexCount = wind->vertexCount;
                        }
                    }
                    record.type = 5;
                }
            }
            if (character) {
                CollectRuntimeHand(*character, *node, state, record, flags);
                if (kind == -67) {
                    record.type = 4;
                    auto &event = *reinterpret_cast<RuntimeMatrixEvent *>(state.cachedMatrixEventState);
                    std::memset(&event, 0, 0x54);
                    event.enabled = 1;
                }
            }
        }
        CollectRuntimeNodes(character, node->runtimeChild, state, flags);
    }
}

const char *RuntimeNodeSuffix(std::uint32_t version, std::int32_t kind) {
    if (version < 5)
        return "";
    if (kind == -1337)
        return "_Door";
    if (kind >= 5000 && kind <= 5009)
        return "_Flag";
    if (kind == -1338)
        return "_Search";
    if (kind == -1339)
        return "_SearchRange";
    if (kind == -1340)
        return "_Lockon";
    if (kind == -1341)
        return "_LockonHandle";
    return "";
}

Matrix4x4 MultiplyRuntimeMatrices(const Matrix4x4 &left, const Matrix4x4 &right) {
    Matrix4x4 output;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            output.rows[row][column] = left.rows[row][0] * right.rows[0][column] + left.rows[row][1] * right.rows[1][column] +
                left.rows[row][2] * right.rows[2][column] + left.rows[row][3] * right.rows[3][column];
    return output;
}

void AssignRuntimeVertexNodes(RuntimeVertexOwner &owner, std::uint32_t *hashes, std::uint32_t maximum, const d3t::ModelRuntimeState &state, bool cloth) {
    if (owner.ready)
        return;
    owner.ready = 1;
    if (!owner.cacheHandle || !owner.rootHash)
        return;
    auto *const cache = d3t::ResolveModelCache(owner.cacheHandle);
    const auto index = d3t::LookupModelCacheIndex(cache->indexTable, owner.rootHash);
    const auto count = std::min<std::uint32_t>(maximum, cache->nodes[index].childCount);
    const auto root = (static_cast<std::uint64_t>(owner.rootHash) << 32) | owner.cacheHandle;
    for (std::uint32_t vertex = 0; vertex < count; ++vertex) {
        char name[256];
        if (!cloth)
            std::snprintf(name, sizeof(name), "Node_%s_Flag_%03u_%03u", state.modelName, owner.slot, vertex);
        else if (state.nodeCount == 1)
            std::snprintf(name, sizeof(name), "Node_Cloth_%03u_%03u", owner.slot, vertex);
        else
            std::snprintf(name, sizeof(name), "Node_%s_Cloth_%03u_%03u", state.modelName, owner.slot, vertex);
        const auto mapped = d3t::FindModelCacheNode(root, name);
        if (static_cast<std::uint32_t>(mapped) && static_cast<std::uint32_t>(mapped >> 32))
            hashes[vertex] = static_cast<std::uint32_t>(mapped >> 32);
    }
}

void AssignRuntimeMatrixEvent(RuntimeMatrixEvent &event, const d3t::ModelRuntimeState &state) {
    if (event.ready)
        return;
    event.ready = 1;
    if (!event.cacheHandle || !state.rootHash)
        return;
    const auto head = d3t::FindModelCacheNode(RuntimeHandle(state, state.rootHash), "HeadRig:c_kao_bn");
    if (!static_cast<std::uint32_t>(head) || !static_cast<std::uint32_t>(head >> 32))
        return;
    event.headHash = static_cast<std::uint32_t>(head >> 32);
    if (!event.nodeHash)
        return;
    const Matrix4x4 node = d3t::ComputeModelCacheGlobalTransform(RuntimeHandle(state, event.nodeHash));
    const Matrix4x4 headMatrix = d3t::ComputeModelCacheGlobalTransform(head);
    Matrix4x4 inverse;
    Matrix4x4::invertMatrix(&node, &inverse);
    event.relative = MultiplyRuntimeMatrices(headMatrix, inverse);
}


void AssignRuntimeNodes(const void *model, d3t::ModelRuntimeState &state) {
    if (!state.nodeTableAvailable || state.nodesAssigned || !state.cacheHandle || !state.rootHash)
        return;
    state.nodesAssigned = 1;
    if (!state.nodeCount)
        return;
    char name[256];
    for (std::uint32_t index = 0; index < state.recordCount; ++index) {
        d3t::ModelRuntimeNode &record = state.nodes[index];
        if (!record.node)
            continue;
        if (record.type == 3 && record.attachmentIndex < 2) {
            auto &hand = state.hands[record.attachmentIndex];
            FormatRuntimeHandName(name, sizeof(name), state, hand, 0);
            const auto handRoot = d3t::ResolveModelRuntimeNode(model, name, record.kind);
            if (handRoot) {
                record.nodeHash = static_cast<std::uint32_t>(handRoot >> 32);
                hand.cacheHandle = state.cacheHandle;
                hand.nodeHash = record.nodeHash;
                hand.ready = 1;
                auto *const cache = d3t::ResolveModelCache(state.cacheHandle);
                const auto handIndex = d3t::LookupModelCacheIndex(cache->indexTable, hand.nodeHash);
                const std::uint32_t count = std::min<std::uint32_t>(20, cache->nodes[handIndex].subtreeSize);
                for (std::uint32_t joint = 0; joint < count; ++joint) {
                    FormatRuntimeHandName(name, sizeof(name), state, hand, joint);
                    hand.boneHashes[joint] = static_cast<std::uint32_t>(d3t::FindModelCacheNode(handRoot, name) >> 32);
                }
            }
        }
        const char sign = record.kind < 0 ? 'M' : 'P';
        const auto kind = static_cast<std::uint32_t>(std::abs(static_cast<std::int32_t>(record.kind)));
        if (state.nodeCount == 1)
            std::snprintf(name, sizeof(name), "Node_%03u_K%c%05u", index, sign, kind);
        else
            std::snprintf(name, sizeof(name), "Node_%s_%03u_K%c%05u%s", state.modelName, index, sign, kind, RuntimeNodeSuffix(state.nodeCount, record.kind));
        if (record.type == 4) {
            const auto face = d3t::ResolveModelRuntimeNode(model, name, record.kind);
            if (face) {
                record.nodeHash = static_cast<std::uint32_t>(face >> 32);
                auto &event = *reinterpret_cast<RuntimeMatrixEvent *>(state.cachedMatrixEventState);
                event.cacheHandle = state.cacheHandle;
                event.nodeHash = record.nodeHash;
                AssignRuntimeMatrixEvent(event, state);
            }
        }
        const auto mapped = d3t::ResolveModelRuntimeNode(model, name, record.kind);
        if (mapped)
            record.nodeHash = static_cast<std::uint32_t>(mapped >> 32);
        else if (record.type == 1)
            record.node = nullptr;
        if (!record.node)
            continue;
        if (record.type == 5 && record.attachmentIndex < 32) {
            auto &dynamic = reinterpret_cast<RuntimeDynamicOwner *>(state.dynamicModelRuntime)[record.attachmentIndex];
            dynamic.owner.cacheHandle = state.cacheHandle;
            dynamic.owner.rootHash = record.nodeHash;
            AssignRuntimeVertexNodes(dynamic.owner, dynamic.hashes, 128, state, false);
        } else if (record.type == 2 && record.attachmentIndex < 8) {
            auto &cloth = reinterpret_cast<RuntimeClothOwner *>(state.clothRuntime)[record.attachmentIndex];
            cloth.owner.cacheHandle = state.cacheHandle;
            cloth.owner.rootHash = record.nodeHash;
            AssignRuntimeVertexNodes(cloth.owner, cloth.hashes, 180, state, true);
        }
    }
}

void CollectModelRuntimeTable(MT7Model *model, MT7Node *source, Character *character, d3t::ModelRuntimeState &state, std::uint32_t flags) {
    if (!source || !model || state.nodeTableAvailable)
        return;
    state.nodeTableAvailable = 1;
    state.nodeCount = 0;
    const auto skeleton = FindRuntimeSkeleton(state, RuntimeHandle(state, state.rootHash));
    state.skeletonHash = static_cast<std::uint32_t>(skeleton >> 32);
    if (!static_cast<std::uint32_t>(skeleton) || !state.skeletonHash)
        state.nodeCount = 0;
    if (state.nodeCount)
        CollectRuntimeNodes(character, source, state, flags);
}

void EnableUnmappedRuntimeSubtree(d3t::ModelRuntimeState &state, std::uint64_t handle) {
    auto *const cache = d3t::ResolveModelCache(static_cast<std::uint32_t>(handle));
    const auto index = d3t::LookupModelCacheIndex(cache->indexTable, static_cast<std::uint32_t>(handle >> 32));
    cache->nodes[index].flags |= 1U;
    const auto count = cache->nodes[index].childCount;
    for (std::uint32_t child = 0; child < count; ++child) {
        const auto next = d3t::GetModelCacheChild(handle, child);
        bool mapped = false;
        for (std::uint32_t record = 0; record < state.recordCount; ++record)
            if (state.nodes[record].nodeHash == static_cast<std::uint32_t>(next >> 32)) {
                mapped = true;
                break;
            }
        if (!mapped)
            EnableUnmappedRuntimeSubtree(state, next);
    }
}

void PublishRuntimeNodeTransforms(MT7Node *node, d3t::ModelRuntimeState &state, std::uint32_t flags) {
    for (; node; node = node->runtimeSibling) {
        Matrix4x4_Push();
        if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(*node))
            Matrix4x4::SetActiveTransform(transform);
        else {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
            if (flags & 1U)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        }
        for (std::uint32_t record = 0; record < state.recordCount; ++record) {
            const auto &mapped = state.nodes[record];
            if (mapped.node != node)
                continue;
            if (mapped.nodeHash) {
                const auto handle = RuntimeHandle(state, mapped.nodeHash);
                if (node->id >> 16)
                    EnableUnmappedRuntimeSubtree(state, handle);
                Matrix4x4 transposed;
                for (std::size_t row = 0; row < 4; ++row)
                    for (std::size_t column = 0; column < 4; ++column)
                        transposed.rows[row][column] = workTransformMatrix.rows[column][row];
                d3t::SetModelCacheTransform(handle, transposed);
            }
            break;
        }
        PublishRuntimeNodeTransforms(node->runtimeChild, state, flags);
        Matrix4x4::Pop(1);
    }
}

void LocalizeRuntimeNodeTransforms(d3t::ModelRuntimeState &state, std::int32_t mode) {
    std::uint64_t root = 0;
    Matrix4x4 retained = d3t::kModelCacheIdentity;
    for (std::uint32_t index = 0; index < state.recordCount; ++index) {
        const auto &record = state.nodes[index];
        if (!record.node || !record.nodeHash)
            continue;
        const auto handle = RuntimeHandle(state, record.nodeHash);
        const Matrix4x4 parent = d3t::ComputeModelCacheGlobalTransform(d3t::GetModelCacheParent(handle));
        Matrix4x4 inverse;
        Matrix4x4::invertMatrix(&parent, &inverse);
        const Matrix4x4 current = d3t::GetModelCacheTransform(handle);
        d3t::SetModelCacheTransform(handle, MultiplyRuntimeMatrices(inverse, current));
        if (!index && !mode) {
            root = handle;
            retained = current;
        }
    }
    if (root)
        d3t::SetModelCacheTransform(root, retained);
}

// @verified - p
Vector3 NormalizeRuntimeVector(Vector3 value) {
    const float length = std::sqrt((value.x * value.x + value.y * value.y) + value.z * value.z);
    if (!(length > 0.0f))
        return value;
    const float reciprocal = 1.0f / length;
    return {value.x * reciprocal, value.y * reciprocal, value.z * reciprocal};
}

// @verified - p
Matrix4x4 *BuildRuntimeAxisAngle(Matrix4x4 *output, const Vector3 *input, float angle) {
    const Vector3 axis = NormalizeRuntimeVector(*input);
    const float c = std::cos(angle), sine = std::sin(angle), t = 1.0f - c;
    const float x = axis.x, y = axis.y, z = axis.z;
    output->rows[0] = {.x = x * x * t + c, .y = y * x * t - z * sine, .z = y * sine + z * x * t, .w = 0};
    output->rows[1] = {.x = z * sine + y * x * t, .y = y * y * t + c, .z = z * y * t - x * sine, .w = 0};
    output->rows[2] = {.x = z * x * t - y * sine, .y = x * sine + z * y * t, .z = z * z * t + c, .w = 0};
    output->rows[3] = {.x = 0, .y = 0, .z = 0, .w = 1};
    return output;
}

// @verified - p
Matrix4x4 *BuildRuntimeVectorRotation(Matrix4x4 *output, const Vector3 *from, const Vector3 *to) {
    const Vector3 a = NormalizeRuntimeVector(*from), b = NormalizeRuntimeVector(*to);
    const float dot = (a.x * b.x + a.y * b.y) + a.z * b.z;
    if (dot > 0.99999988f) {
        *output = d3t::kModelCacheIdentity;
        return output;
    }
    const Vector3 axis = dot < -0.99999988f ? Vector3{.x = from->y, .y = from->x, .z = from->z} :
                                              Vector3{.x = a.y * b.z - a.z * b.y, .y = b.x * a.z - a.x * b.z, .z = a.x * b.y - b.x * a.y};
    return BuildRuntimeAxisAngle(output, &axis, std::acos(dot));
}

// @verified - p
void PublishRuntimeVertex(
    RuntimeVertexOwner &owner, const std::uint32_t *hashes, std::uint32_t maximum, std::uint32_t index, const Vector3 &position, const Vector3 *previous) {
    if (!owner.ready || index > maximum || !owner.cacheHandle || !hashes[index])
        return;
    const auto handle = (static_cast<std::uint64_t>(hashes[index]) << 32) | owner.cacheHandle;
    const auto parent = d3t::GetModelCacheParent(handle);
    if (!static_cast<std::uint32_t>(parent) || !static_cast<std::uint32_t>(parent >> 32))
        return;
    Matrix4x4 matrix = d3t::GetModelCacheTransform(handle);
    if (previous && g_shouldHashRotationMatrix) {
        const Vector3 from{.x = 0.0f, .y = 1.0f, .z = 0.0f};
        const Vector3 direction{.x = previous->x - position.x, .y = previous->y - position.y, .z = previous->z - position.z};
        BuildRuntimeVectorRotation(&matrix, &from, &direction);
    }
    matrix.rows[0].w = position.x;
    matrix.rows[1].w = position.y;
    matrix.rows[2].w = position.z;
    d3t::SetModelCacheTransform(handle, matrix);
}

// @verified - p
void UpdateRuntimeCloth(d3t::ModelRuntimeState &state) {
    auto *const owners = reinterpret_cast<RuntimeClothOwner *>(state.clothRuntime);
    for (std::uint32_t slot = 0; slot < 8; ++slot) {
        auto &owner = owners[slot];
        if (!owner.owner.enabled || !owner.owner.ready || !owner.owner.node)
            continue;
        auto *const cloth = static_cast<ClothObject *>(owner.owner.node->taskData);
        if (!cloth || cloth->token.name != SHEN_NAME(CLTH))
            continue;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&cloth->transform);
        for (std::int32_t index = 0; index < cloth->vertexCount; ++index) {
            const Vector3 &position = cloth->bindings[index].position;
            const Vector3 *neighbor = nullptr;
            if (index >= cloth->columns) {
                neighbor = &cloth->bindings[index - cloth->columns].position;
            }
            PublishRuntimeVertex(owner.owner, owner.hashes, 180, index, position, neighbor);
        }
        Matrix4x4::Pop(1);
    }
}

void UpdateRuntimeDynamicModels(d3t::ModelRuntimeState &state) {
    auto *const owners = reinterpret_cast<RuntimeDynamicOwner *>(state.dynamicModelRuntime);
    for (std::uint32_t slot = 0; slot < 32; ++slot) {
        auto &owner = owners[slot];
        if (!owner.owner.enabled || !owner.owner.ready)
            continue;
        const auto *const wind = FindFlagWindModel(owner.owner.node);
        if (!wind || !wind->vertices)
            continue;
        for (std::int32_t index = 0; index < wind->vertexCount; ++index)
            PublishRuntimeVertex(owner.owner, owner.hashes, 128, index, *wind->vertices[index].position, nullptr);
    }
}

void UpdateRuntimeMatrixEvent(d3t::ModelRuntimeState &state) {
    auto &event = *reinterpret_cast<RuntimeMatrixEvent *>(state.cachedMatrixEventState);
    if (!event.enabled || !event.ready || !event.cacheHandle || !event.nodeHash || !event.headHash)
        return;
    const auto head = RuntimeHandle(state, event.headHash);
    const Matrix4x4 current = d3t::ComputeModelCacheGlobalTransform(RuntimeHandle(state, event.nodeHash));
    const Matrix4x4 parent = d3t::ComputeModelCacheGlobalTransform(d3t::GetModelCacheParent(head));
    Matrix4x4 inverse;
    Matrix4x4::invertMatrix(&parent, &inverse);
    d3t::SetModelCacheTransform(head, MultiplyRuntimeMatrices(MultiplyRuntimeMatrices(event.relative, current), inverse));
}

} // namespace model_component_detail

std::uint32_t GetModelBodyKind(const void *model) {
    const auto *const state = d3t::GetModelRuntimeState(GetMT7ReaderData(static_cast<const MT7Model *>(model)));
    return state ? state->nodeCount : 0;
}

namespace model_component_detail {

d3t::ModelRuntimeNode *FindPlacementRuntimeNode(d3t::ModelRuntimeState &state, std::int32_t kind) {
    if (!state.nodeTableAvailable || !state.nodesAssigned)
        return nullptr;
    for (std::uint32_t index = 0; index < state.recordCount; ++index)
        if (state.nodes[index].node && state.nodes[index].kind == kind)
            return &state.nodes[index];
    return nullptr;
}

void SetUnmappedRuntimeChildVisibility(d3t::ModelRuntimeState &state, std::uint64_t handle, std::uint32_t visible) {
    d3t::ModelCacheSlot *const cache = d3t::ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!cache)
        return;
    const std::uint32_t index = d3t::LookupModelCacheIndex(cache->indexTable, static_cast<std::uint32_t>(handle >> 32));
    if (visible)
        cache->nodes[index].flags &= ~1U;
    else
        cache->nodes[index].flags |= 1U;
    const std::uint32_t count = cache->nodes[index].childCount;
    for (std::uint32_t child = 0; child < count; ++child) {
        const std::uint64_t next = d3t::GetModelCacheChild(handle, child);
        bool mapped = false;
        for (std::uint32_t record = 0; record < state.recordCount; ++record)
            if (state.nodes[record].nodeHash == static_cast<std::uint32_t>(next >> 32)) {
                mapped = true;
                break;
            }
        if (!mapped)
            SetUnmappedRuntimeChildVisibility(state, next, visible);
    }
}

} // namespace model_component_detail

// @verified - p
void SetModelPlacementVisible(const void *placement, std::int32_t visible) {
    d3t::ModelRuntimeState *const state = d3t::GetModelRuntimeState(placement);
    if (state)
        d3t::UpdateModelCacheFlags(RuntimeHandle(*state, state->rootHash), 1, visible ? d3t::ModelCacheFlagMode::Clear : d3t::ModelCacheFlagMode::Enable);
}

// @verified - p
void SetModelPlacementChildVisible(const void *placement, std::int32_t kind, std::uint32_t visible) {
    if (!placement)
        return;
    d3t::ModelRuntimeState *const state = d3t::GetModelRuntimeState(placement);
    if (!state)
        return;
    const d3t::ModelRuntimeNode *const node = FindPlacementRuntimeNode(*state, kind);
    if (node && state->cacheHandle && node->nodeHash)
        SetUnmappedRuntimeChildVisibility(*state, RuntimeHandle(*state, node->nodeHash), visible);
}

// @verified - p
void SetModelPlacementChildTransform(const void *placement, std::int32_t kind, const Matrix4x4 *transform) {
    if (!placement)
        return;
    d3t::ModelRuntimeState *const state = d3t::GetModelRuntimeState(placement);
    if (!state)
        return;
    const d3t::ModelRuntimeNode *const node = FindPlacementRuntimeNode(*state, kind);
    if (!node || !state->cacheHandle || !node->nodeHash)
        return;
    const std::uint64_t handle = RuntimeHandle(*state, node->nodeHash);
    const Matrix4x4 &source = transform ? *transform : workTransformMatrix;
    Matrix4x4 matrix;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            matrix.rows[row][column] = source.rows[column][row];
    const std::uint64_t parent = d3t::GetModelCacheParent(handle);
    if (static_cast<std::uint32_t>(parent) && parent >> 32) {
        const Matrix4x4 global = d3t::ComputeModelCacheGlobalTransform(parent);
        Matrix4x4 inverse;
        Matrix4x4::invertMatrix(&global, &inverse);
        matrix = MultiplyRuntimeMatrices(inverse, matrix);
    }
    d3t::SetModelCacheTransform(handle, matrix);
}

bool UpdateModelFrame(MT7Model *model, Character *character, std::int32_t mode, std::uint32_t flags, const void *placement) {
    if (!placement)
        placement = GetMT7ReaderData(model);
    d3t::ModelRuntimeState *const state = d3t::GetModelRuntimeState(placement);
    if (!state)
        return false;
    MT7Node *const source = const_cast<MT7Node *>(static_cast<const MT7Node *>(d3t::GetObjectModelTraversalSource(placement)));
    InsertGameModelNodes(source, character);
    CollectModelRuntimeTable(model, source, character, *state, flags);
    bool updated = false;
    if (state->nodeTableAvailable) {
        AssignRuntimeNodes(placement, *state);
        if (state->nodesAssigned && state->nodeCount && !(flags & 4U)) {
            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            PublishRuntimeNodeTransforms(source, *state, flags);
            updated = true;
            Matrix4x4::Pop(1);
            LocalizeRuntimeNodeTransforms(*state, mode);
            UpdateRuntimeCloth(*state);
            UpdateRuntimeDynamicModels(*state);
            UpdateRuntimeMatrixEvent(*state);
            d3t::UpdateModelExpressions(placement);
        }
    }
    RestoreGameModelNodes(source, character);
    return updated;
}

} // namespace shenmue::s2
