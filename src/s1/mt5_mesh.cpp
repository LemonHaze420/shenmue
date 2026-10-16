#include "s1/matrix_stack.hpp"
#include <shendk/files/model/mt5_reader.h>
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/task/pack.hpp"
#include "s1/scene.hpp"
#include "s1/mt5_types.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>

namespace shenmue::s1 {




constexpr std::size_t kMaxVertices = 262144;
constexpr std::size_t kMaxMaterials = 128;
constexpr std::size_t kMaxStripsPerMaterial = 2000;
thread_local const std::byte *g_mt5VertexDataOverride{};

const void *MT5_SetVertexDataOverride(const void *vertices) {
    const void *const previous = g_mt5VertexDataOverride;
    g_mt5VertexDataOverride = static_cast<const std::byte *>(vertices);
    return previous;
}

#pragma pack(push, 1)
struct Mt5SceneBinding {
    std::uint32_t flags;
    std::uint32_t sceneEntityOffset;
    std::uint32_t auxiliaryOffset;
    std::uint32_t stripCommandOffset;
};

struct Mt5Command {
    std::uint16_t opcode;
    std::uint16_t value;
};
#pragma pack(pop)

using Mt5Node = Mt5RuntimeNode;

struct StripDescription {
    std::int32_t vertexCount;
    std::int32_t flags;
    std::int32_t winding;
};

struct MaterialDescription {
    std::array<StripDescription, kMaxStripsPerMaterial> strips;
    std::int32_t stripCount;
    std::int32_t firstVertex;
    std::int32_t outputReferenceCount;
    std::int32_t textureId;
};

static_assert(sizeof(Mt5Node) == 0x40);
static_assert(sizeof(Mt5SceneBinding) == 0x10);
static_assert(sizeof(Mt5Command) == 4);
static_assert(sizeof(MaterialDescription) == 0x5dd0);

std::array<Matrix4x4, kMaxVertices> g_stagedVertices{};
std::array<std::int32_t, kMaxVertices> g_stagedMaterialIndices{};
std::array<Matrix4x4, kMaxVertices> g_tempMeshData{};
std::array<std::int32_t, kMaxVertices> g_outputMaterialIndices{};
std::array<MaterialDescription, kMaxMaterials> g_materials{};
std::int32_t g_stagedVertexCount = 0;
std::int32_t g_stagedMaterialIndexCount = 0;
std::int32_t g_outputVertexCount = 0;
std::int32_t g_outputMaterialIndexCount = 0;
std::int32_t g_materialCount = 0;

void MT5_RemoveSceneModel(std::int32_t sceneKey);

std::int32_t stripVertexFormat(std::uint32_t opcode) {
#if 0
    if (opcode == 0x10 || opcode == 0x13 || opcode == 0x18 || opcode == 0x1b) {
        return 0;
    }
    if (opcode == 0x11 || opcode == 0x19) {
        return 2;
    }
    if (opcode == 0x12 || opcode == 0x1a) {
        return 1;
    }
    if (opcode == 0x14 || opcode == 0x1c) {
        return 3;
    }
    return 0;
#else
    return opcode <= 0xffffU ? static_cast<std::int32_t>(shendk::Mt5StripVertexFormat(static_cast<std::uint16_t>(opcode))) : 0;
#endif
}

float decodeUv(std::uint16_t encodedValue, bool fixed16, std::uint16_t ratio) {
#if 0
    const float value = static_cast<float>(static_cast<std::int16_t>(encodedValue));
    if (fixed16) {
        return value * 0.000015258789f;
    }
    return value / static_cast<float>(ratio);
#else
    return shendk::DecodeMt5Uv(encodedValue, fixed16, ratio);
#endif
}

void MT5_ExpandTempTriangleStrips() {
    for (std::int32_t materialIndex = 0; materialIndex < g_materialCount; ++materialIndex) {
        MaterialDescription &material = g_materials[materialIndex];
        std::int32_t firstVertex = material.firstVertex;
        for (std::int32_t stripIndex = 0; stripIndex < material.stripCount; ++stripIndex) {
            const StripDescription &strip = material.strips[stripIndex];




            if (strip.winding != 0) {
                firstVertex += strip.vertexCount;
                continue;
            }
            for (std::int32_t triangle = 0; triangle < strip.vertexCount - 2; ++triangle) {
                const std::int32_t order[3] = {
                    triangle,
                    triangle + ((triangle & 1) ? 1 : 2),
                    triangle + ((triangle & 1) ? 2 : 1),
                };
                for (std::int32_t corner : order) {
                    const std::int32_t sourceIndex = firstVertex + corner;
                    if (sourceIndex < 0 || sourceIndex >= g_stagedVertexCount || g_outputVertexCount >= static_cast<std::int32_t>(g_tempMeshData.size()) ||
                        g_outputMaterialIndexCount >= static_cast<std::int32_t>(g_outputMaterialIndices.size())) {
                        continue;
                    }
                    g_tempMeshData[g_outputVertexCount++] = g_stagedVertices[sourceIndex];
                    g_outputMaterialIndices[g_outputMaterialIndexCount++] = g_stagedMaterialIndices[sourceIndex];
                }
            }
            firstVertex += strip.vertexCount;
        }
    }
}

Mt5Node *offsetNode(std::int32_t offset) {
    return offset ? reinterpret_cast<Mt5Node *>(MT5_ModelBase() + offset) : nullptr;
}

Matrix4x4 *offsetMatrix(std::int32_t offset) {
    return offset ? reinterpret_cast<Matrix4x4 *>(MT5_ModelBase() + offset) : nullptr;
}

void MT5_ResolveRelativeVertexTransform(Matrix4x4 *output, std::int32_t vertexIndex, void **nodeReference) {
    auto *const sourceNode = static_cast<Mt5Node *>(*nodeReference);
    Mt5Node *ownerNode = sourceNode;
    const auto offsetBase = [](Mt5Node *node) { return HMDL_NodeOffsetsUseTaskData(node) ? g_pCurrentTaskData : MT5_ModelBaseForNode(node); };
    const auto resolveNodeOffset = [&offsetBase](Mt5Node *node, std::int32_t offset) {
        return offset ? reinterpret_cast<Mt5Node *>(offsetBase(node) + offset) : nullptr;
    };
    const float *vertex = nullptr;
    while (ownerNode->parentOffset) {
        ownerNode = resolveNodeOffset(ownerNode, ownerNode->parentOffset);
        if (!ownerNode)
            return;
        std::byte *bindingBase = nullptr;
        auto *const binding = static_cast<Mt5SceneBinding *>(MT5_SceneBindingForNode(ownerNode, &bindingBase));
        if (!binding)
            continue;
        vertexIndex += static_cast<std::int32_t>(binding->auxiliaryOffset);
        if (!binding->sceneEntityOffset)
            continue;
        const std::byte *const vertexData = bindingBase + binding->sceneEntityOffset;
        vertex = reinterpret_cast<const float *>(vertexData + 24 * vertexIndex);
        if (vertexIndex >= 0) {
            break;
        }
    }
    if (vertexIndex < 0 || !vertex) {
        return;
    }

    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity) {
        ++g_matrixStack.top.stack;
    }
    workTransformMatrix = transformMatrix_prePass;

    const Matrix4x4 *const sourceTransform = HMDL_TransformForNode(sourceNode, sourceNode->transformOffset);
    const Matrix4x4 *const ownerTransform = HMDL_TransformForNode(ownerNode, ownerNode->transformOffset);
    if (sourceTransform && ownerTransform) {
        workTransformMatrix = *sourceTransform;
        if (((sourceNode->nodeKind - 5u) <= 1u || (sourceNode->nodeKind - 10u) <= 1u)) {
            Mt5Node *const parent = resolveNodeOffset(sourceNode, sourceNode->parentOffset);
            if (parent && (parent->nodeKind == 6 || parent->nodeKind == 11)) {
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.z));
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.y));
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.x));
            }
        }
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(ownerTransform);
    } else if (sourceTransform && ownerNode->parentOffset) {
        workTransformMatrix = *sourceTransform;
        if (sourceNode->nodeKind == 6 || sourceNode->nodeKind == 11) {
            Mt5Node *const sourceParent = resolveNodeOffset(sourceNode, sourceNode->parentOffset);
            if (sourceParent && (sourceParent->nodeKind == 7 || sourceParent->nodeKind == 12)) {
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.z));
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.y));
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.x));
            }
        }
        Matrix4x4::InvertInPlace(nullptr);
        Mt5Node *const ownerParent = resolveNodeOffset(ownerNode, ownerNode->parentOffset);
        if (ownerParent) {
            if (const Matrix4x4 *const parentTransform = HMDL_TransformForNode(ownerParent, ownerParent->transformOffset)) {
                Matrix4x4::Multiply(parentTransform);
            }
        }
        Matrix4x4::preTranslate(ownerNode->position.x, ownerNode->position.y, ownerNode->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(ownerNode->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(ownerNode->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(ownerNode->rotation.x));
    } else {
        Matrix4x4::preTranslate(sourceNode->position.x, sourceNode->position.y, sourceNode->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(sourceNode->rotation.x));
        Matrix4x4::InvertInPlace(nullptr);
    }

    const float px = vertex[0];
    const float py = vertex[1];
    const float pz = vertex[2];
    const float nx = vertex[3];
    const float ny = vertex[4];
    const float nz = vertex[5];
    output->rows[0].x =
        px * workTransformMatrix.rows[0].x + py * workTransformMatrix.rows[1].x + pz * workTransformMatrix.rows[2].x + workTransformMatrix.rows[3].x;
    output->rows[0].y =
        px * workTransformMatrix.rows[0].y + py * workTransformMatrix.rows[1].y + pz * workTransformMatrix.rows[2].y + workTransformMatrix.rows[3].y;
    output->rows[0].z =
        px * workTransformMatrix.rows[0].z + py * workTransformMatrix.rows[1].z + pz * workTransformMatrix.rows[2].z + workTransformMatrix.rows[3].z;
    output->rows[0].w = nx * workTransformMatrix.rows[0].x + ny * workTransformMatrix.rows[1].x + nz * workTransformMatrix.rows[2].x;
    output->rows[1].x = nx * workTransformMatrix.rows[0].y + ny * workTransformMatrix.rows[1].y + nz * workTransformMatrix.rows[2].y;
    output->rows[1].y = nx * workTransformMatrix.rows[0].z + ny * workTransformMatrix.rows[1].z + nz * workTransformMatrix.rows[2].z;

    std::int16_t popCount = 1;
    if (g_matrixStack.top.info.index < 1) {
        popCount = g_matrixStack.top.info.index;
    }
    std::int16_t pointerCount = popCount;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        pointerCount = static_cast<std::int16_t>(popCount + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index);
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - popCount);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

bool IsRyoModel(const char *name) {
    if (!g_globalEntityName || !name) {
        return false;
    }
    static constexpr std::array<const char *, 14> names{
        "YKA_M",
        "YKA_F",
        "YKB_M",
        "YKB_F",
        "YKC_M",
        "YKC_F",
        "YKD_M",
        "YKD_F",
        "YKE_M",
        "YKE_F",
        "YKF_M",
        "YKF_F",
        "YKG_M",
        "YKG_F",
    };
    for (const char *candidate : names) {
        if (std::memcmp(name, candidate, 6) == 0) {
            return true;
        }
    }
    return false;
}

void AssignShenmueModelData(void *nodePointer, std::int32_t bufferHandle, bool transparent) {
    auto *const node = static_cast<Mt5Node *>(nodePointer);
    std::byte *const modelBase = MT5_ModelBaseForNode(node);
    auto *const binding = node->sceneBindingOffset && modelBase ? reinterpret_cast<Mt5SceneBinding *>(modelBase + node->sceneBindingOffset) : nullptr;
    const std::uint64_t bindingKeyAddress = reinterpret_cast<std::uintptr_t>(binding) + 4;
    const std::int32_t sceneKey = static_cast<std::int32_t>(bindingKeyAddress % 0xffffffffULL);
    MT5_RemoveSceneModel(sceneKey);

    constexpr std::int32_t kPackModelToken = SHEN_NAME(PAKM);
    auto *const gameNode = static_cast<GameNode *>(HLib_AllocHeapMemBlock(sizeof(GameNode), kPackModelToken));
    std::memset(gameNode, 0, sizeof(GameNode));
    Matrix4x4 worldTransform = transformMatrix_prePass;
    if (node->transformOffset && modelBase) {
        worldTransform = *reinterpret_cast<const Matrix4x4 *>(modelBase + node->transformOffset);
    }
    D3T_SetSceneNodeWorldTransform(gameNode, worldTransform);
    gameNode->abc = g_materialCount;
    gameNode->bufferHandle = bufferHandle;
    if (transparent) {
        gameNode->renderFlags |= 1;
    }
    if ((binding->flags & 2) != 0) {
        gameNode->renderFlags |= 2;
    }
    if ((binding->flags & 0x20) != 0) {
        gameNode->renderFlags |= 4;
    }

    if (g_globalEntityName) {
        std::strncpy(gameNode->entityName, g_globalEntityName, sizeof(gameNode->entityName) - 1);
    }
    if (IsRyoModel(gameNode->entityName)) {
        gameNode->renderFlags |= 8;
    }

    auto *material = &gameNode->vertexCount;
    for (std::int32_t index = 0; index < g_materialCount; ++index) {
        material[0] = static_cast<std::uint32_t>(g_materials[index].outputReferenceCount);
        material[1] = static_cast<std::uint32_t>(g_materials[index].textureId);




        material[2] = 0;
        material += 3;
    }
    *AddEntryToSceneTable(sceneKey) = gameNode;
}

void MT5_AdjustManagerModel(void *nodePointer) {
    auto *const node = static_cast<Mt5Node *>(nodePointer);
    std::array<Mt5Node *, 4> children{};
    Mt5Node *child = offsetNode(node->childOffset);
    for (Mt5Node *&entry : children) {
        entry = child;
        if (!child) {
            break;
        }
        child = offsetNode(child->childOffset);
    }
    if (node->nodeKind != 0xffff) {
        return;
    }
    for (Mt5Node *entry : children) {
        if (!entry || entry->nodeKind != 120) {
            return;
        }
    }

    const std::array<Mt5Node *, 5> nodes{node, children[0], children[1], children[2], children[3]};
    for (Mt5Node *entry : nodes) {
        auto *const binding = entry->sceneBindingOffset ? reinterpret_cast<Mt5SceneBinding *>(MT5_ModelBase() + entry->sceneBindingOffset) : nullptr;
        const std::int32_t sceneKey = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4) % 0xffffffffULL);
        FindSceneEntityByID(sceneKey)->renderFlags |= 0x10;
    }
}

Mt5SceneBinding *AddCharaToSceneEntryTable(void *nodePointer, std::int64_t, bool transparent, const std::int32_t *textureIds) {
    auto *const node = static_cast<Mt5Node *>(nodePointer);
    std::byte *const modelBase = MT5_ModelBaseForNode(node);
    auto *const binding = modelBase ? reinterpret_cast<Mt5SceneBinding *>(modelBase + node->sceneBindingOffset) : nullptr;
    if (!binding)
        return nullptr;
    const std::int32_t sceneKey = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4) % 0xffffffffULL);
    std::uint16_t current = static_cast<std::uint16_t>(sceneKey);
    if (g_Scene.keys[current] != sceneKey) {
        while (g_Scene.chainOffsets[current] != 0) {
            current = static_cast<std::uint16_t>(current + g_Scene.chainOffsets[current]);
            if (g_Scene.keys[current] == sceneKey) {
                break;
            }
        }
    }
    GameNode *const gameNode = g_Scene.keys[current] == sceneKey ? g_Scene.entries[current] : nullptr;
    if (!gameNode) {
        return binding;
    }

    gameNode->abc = g_materialCount;
    if (transparent) {
        gameNode->renderFlags |= 1;
    }
    if ((binding->flags & 2) != 0) {
        gameNode->renderFlags |= 2;
    }
    if (IsRyoModel(gameNode->entityName)) {
        gameNode->renderFlags |= 8;
    }
    if (std::memcmp(gameNode->entityName, "MGR_M", 6) == 0) {
        MT5_AdjustManagerModel(nodePointer);
    }

    auto *material = &gameNode->vertexCount;
    for (std::int32_t index = 0; index < g_materialCount; ++index) {
        material[0] = static_cast<std::uint32_t>(g_materials[index].outputReferenceCount);
        if (textureIds) {
            material[1] = static_cast<std::uint32_t>(g_materials[index].textureId);
        }
        material[2] = 0;
        if ((std::strstr(gameNode->entityName, "ZZB_L") && node->nodeKind == 0xffff) ||
            (std::strstr(gameNode->entityName, "BCA_L") && node->nodeKind == 0xffbd)) {
            if (material[0] == 96) {
                material[2] = 16;
            }
        }
        material += 3;
    }
    *AddEntryToSceneTable(sceneKey) = gameNode;
    return binding;
}

void MT5_ReadMeshStrip(void **node, const std::int32_t *textureIds, std::int32_t bufferHandle) {
    auto *const current = static_cast<Mt5Node *>(*node);
    std::byte *modelBase = nullptr;
    auto *const binding = static_cast<Mt5SceneBinding *>(MT5_SceneBindingForNode(current, &modelBase));
    if (!binding || !modelBase)
        return;
    const std::byte *const vertexData = g_mt5VertexDataOverride ? g_mt5VertexDataOverride : modelBase + static_cast<std::int32_t>(binding->sceneEntityOffset);
    const auto readU16 = [](const std::byte *address) {
        std::uint16_t value{};
        std::memcpy(&value, address, sizeof(value));
        return value;
    };
    auto *cursor = modelBase + binding->stripCommandOffset;

    g_stagedVertexCount = 0;
    g_stagedMaterialIndexCount = 0;
    g_outputVertexCount = 0;
    g_outputMaterialIndexCount = 0;
    g_materialCount = 0;

    std::uint16_t uvRatio = 256;
    bool transparent = false;
    bool fixed16Uv = false;
    std::int32_t textureId = 0;
    std::int32_t materialIndex = -1;
    std::int32_t alphaMode = 0;

    bool streamTerminated = false;
    for (std::int32_t instructionGuard = 0; instructionGuard < 500000 && !streamTerminated; ++instructionGuard) {
        const std::uint16_t opcode = readU16(cursor);
        cursor += sizeof(std::uint16_t);
        if (opcode == 0x8000)
            break;
        switch (opcode) {
        case 0:
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: {
            const std::uint16_t size = readU16(cursor);
            cursor += sizeof(std::uint16_t);
            std::uint32_t flags = 0;
            if (size >= sizeof(flags))
                std::memcpy(&flags, cursor, sizeof(flags));
            cursor += size;
            fixed16Uv = (flags & 1) != 0;
            alphaMode = static_cast<std::int32_t>((flags >> 24) & 7);
            if (alphaMode == 2 || alphaMode == 3) {
                transparent = true;
            }
            break;
        }
        case 8:
        case 10:
            cursor += sizeof(std::uint16_t);
            break;
        case 9:
            if (textureIds) {
                const std::uint16_t textureIndex = readU16(cursor);
                if (textureIndex < 640)
                    textureId = textureIds[textureIndex];
            }
            cursor += sizeof(std::uint16_t);
            break;
        case 11:
            uvRatio = readU16(cursor);
            cursor += sizeof(std::uint16_t);
            break;
        case 12:
        case 13:
        case 21:
        case 29:


            break;
        case 14:
        case 15:
            cursor += 10;
            break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28: {
            const std::int32_t format = stripVertexFormat(opcode);
            ++materialIndex;
            if (materialIndex >= static_cast<std::int32_t>(kMaxMaterials)) {
                return;
            }
            MaterialDescription &material = g_materials[materialIndex];
            material = {};
            material.firstVertex = g_stagedVertexCount;
            material.textureId = textureId;
            g_materialCount = materialIndex + 1;

            cursor += sizeof(std::uint16_t);
            const std::uint16_t stripCount = readU16(cursor);
            cursor += sizeof(std::uint16_t);
            if (stripCount > kMaxStripsPerMaterial)
                return;
            for (std::uint16_t stripIndex = 0; stripIndex < stripCount; ++stripIndex) {
                StripDescription &strip = material.strips[material.stripCount++];
                std::int16_t signedCount{};
                std::memcpy(&signedCount, cursor, sizeof(signedCount));
                cursor += sizeof(signedCount);
                strip.flags = 0x70 | (signedCount >= 0 ? 3 : 2);
                if (signedCount < 0) {
                    signedCount = static_cast<std::int16_t>(-signedCount);
                }
                if (signedCount <= 0 || g_stagedVertexCount + signedCount > static_cast<std::int32_t>(g_stagedVertices.size())) {
                    return;
                }
                strip.vertexCount = signedCount;
                strip.winding = (strip.flags & 1) ? 1 : 0;

                for (std::int32_t vertex = 0; vertex < strip.vertexCount; ++vertex) {
                    std::int16_t vertexIndex{};
                    std::memcpy(&vertexIndex, cursor, sizeof(vertexIndex));
                    cursor += sizeof(vertexIndex);
                    Matrix4x4 decoded{};
                    if (vertexIndex >= 0) {
                        const auto *const vertices = reinterpret_cast<const float *>(vertexData);
                        std::memcpy(&decoded.rows[0], vertices + 6 * vertexIndex, 6 * sizeof(float));
                    } else {
                        MT5_ResolveRelativeVertexTransform(&decoded, vertexIndex, node);
                    }

                    decoded.rows[1].z = 0.0f;
                    decoded.rows[1].w = 0.0f;
                    if (format == 2 || format == 3) {
                        decoded.rows[1].z = decodeUv(readU16(cursor), fixed16Uv, uvRatio);
                        decoded.rows[1].w = decodeUv(readU16(cursor + 2), fixed16Uv, uvRatio);
                        cursor += 4;
                    }
                    decoded.rows[2] = {1.0f, 1.0f, 1.0f, 1.0f};
                    decoded.rows[3] = {};
                    if (format == 1 || format == 3) {
                        std::uint32_t bgra{};
                        std::memcpy(&bgra, cursor, sizeof(bgra));
                        cursor += sizeof(bgra);
                        decoded.rows[2].x = static_cast<float>((bgra >> 16) & 0xff) / 255.0f;
                        decoded.rows[2].y = static_cast<float>((bgra >> 8) & 0xff) / 255.0f;
                        decoded.rows[2].z = static_cast<float>(bgra & 0xff) / 255.0f;
                        decoded.rows[2].w = static_cast<float>((bgra >> 24) & 0xff) / 255.0f;
                    }
                    if (g_stagedVertexCount >= static_cast<std::int32_t>(g_stagedVertices.size())) {
                        continue;
                    }
                    g_stagedVertices[g_stagedVertexCount++] = decoded;
                    const float px = decoded.rows[0].x;
                    const float py = decoded.rows[0].y;
                    const float pz = decoded.rows[0].z;
                    if (std::isfinite(px) && std::isfinite(py) && std::isfinite(pz) && std::abs(px) < 10000000.0f && std::abs(py) < 10000000.0f &&
                        std::abs(pz) < 10000000.0f) {
                        g_sceneBoundsMin[0] = (std::min)(g_sceneBoundsMin[0], px);
                        g_sceneBoundsMin[1] = (std::min)(g_sceneBoundsMin[1], py);
                        g_sceneBoundsMin[2] = (std::min)(g_sceneBoundsMin[2], pz);
                        g_sceneBoundsMax[0] = (std::max)(g_sceneBoundsMax[0], px);
                        g_sceneBoundsMax[1] = (std::max)(g_sceneBoundsMax[1], py);
                        g_sceneBoundsMax[2] = (std::max)(g_sceneBoundsMax[2], pz);
                    }
                    if (g_stagedMaterialIndexCount < static_cast<std::int32_t>(g_stagedMaterialIndices.size())) {
                        g_stagedMaterialIndices[g_stagedMaterialIndexCount++] = materialIndex;
                    }
                }
            }
            break;
        }
        default:





            streamTerminated = true;
            break;
        }
    }

    MT5_ExpandTempTriangleStrips();
    for (std::int32_t index = 0; index < g_outputMaterialIndexCount; ++index)
        ++g_materials[g_outputMaterialIndices[index]].outputReferenceCount;
    if (bufferHandle) {
        const std::int32_t byteSize = g_outputVertexCount * static_cast<std::int32_t>(sizeof(Matrix4x4));
        const std::int32_t bufferSize = GetSizeForModelBuffer(bufferHandle);
        void *const mapped = GetWriteMappedBufferPointer(DX11::g_ppD3DDeviceContext, bufferHandle, 1, 0);
        if (mapped && byteSize == bufferSize) {
            std::memcpy(mapped, g_tempMeshData.data(), static_cast<std::size_t>(byteSize));
        }
        DX11_CopySceneNodeToGPU(DX11::g_ppD3DDeviceContext, bufferHandle);
        AddCharaToSceneEntryTable(*node, 0, transparent, textureIds);
        return;
    }

    const BufferDescription description{
        1,
        g_outputVertexCount * static_cast<std::int32_t>(sizeof(Matrix4x4)),
        1,
        3,
        g_tempMeshData.data(),
        nullptr,
    };
    const std::int32_t newBuffer = CreateBuffer(&description);
    AssignShenmueModelData(*node, newBuffer, transparent);
    binding->flags |= 0x400;
}

} // namespace shenmue::s1
