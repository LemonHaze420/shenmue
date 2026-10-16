#include "s1/scene.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/task/scen.hpp"

#include "s1/hlib.hpp"

#include <cstring>
#include <unordered_map>

namespace shenmue::s1 {

SceneEntityTable g_Scene{};
std::int32_t g_UploadedSceneRenderHandles[32768]{};
std::uint32_t g_UploadedSceneRenderHandleCount = 0;
Vector3 g_sceneBoundsMin{3.402823466e+38F, 3.402823466e+38F, 3.402823466e+38F};
Vector3 g_sceneBoundsMax{-3.402823466e+38F, -3.402823466e+38F, -3.402823466e+38F};
Vector3 g_scriptTransformScale{};
namespace {
std::unordered_map<const GameNode *, Matrix4x4> g_sceneNodeWorldTransforms;
}

void D3T_SetSceneNodeWorldTransform(const GameNode *node, const Matrix4x4 &transform) {
    if (node)
        g_sceneNodeWorldTransforms[node] = transform;
}

bool D3T_GetSceneNodeWorldTransform(const GameNode *node, Matrix4x4 *transform) {
    if (!node || !transform)
        return false;

    const auto found = g_sceneNodeWorldTransforms.find(node);
    if (found == g_sceneNodeWorldTransforms.end())
        return false;

    *transform = found->second;
    return true;
}

void D3T_RemoveSceneNodeWorldTransform(const GameNode *node) {
    if (node)
        g_sceneNodeWorldTransforms.erase(node);
}

void InitSceneDataWrapper() {
    for (GameNode *node : g_Scene.entries) {
        if (node) {
            ValidateAndCommitBuffer(node->bufferHandle);
        }
    }

    for (std::uint32_t index = 0; index < g_UploadedSceneRenderHandleCount; ++index) {
        const std::int32_t handle = g_UploadedSceneRenderHandles[index];
        if (handle) {
            ValidateAndCommitBuffer(handle);
        }
    }
    g_UploadedSceneRenderHandleCount = 0;
}

void FreeSceneSpace(std::uint64_t index) {
    const std::int32_t key = g_Scene.keys[index];
    if (!key) {
        return;
    }

    std::uint16_t current = static_cast<std::uint16_t>(key);
    if (g_Scene.keys[current] == key) {
        g_Scene.keys[current] = 0;
        g_Scene.entries[current] = nullptr;
        --g_Scene.entityCount;
        return;
    }

    std::uint16_t previous = current;
    if (g_Scene.chainOffsets[current] == 0) {
        return;
    }
    for (;;) {
        current = static_cast<std::uint16_t>(current + g_Scene.chainOffsets[previous]);
        if (g_Scene.keys[current] == key) {
            g_Scene.keys[current] = 0;
            g_Scene.entries[current] = nullptr;
            if (g_Scene.chainOffsets[current] == 0) {
                g_Scene.chainOffsets[previous] = 0;
            }
            --g_Scene.entityCount;
            return;
        }
        previous = current;
        if (g_Scene.chainOffsets[current] == 0) {
            return;
        }
    }
}

GameNode **AddEntryToSceneTable(std::int32_t key) {
    if (g_Scene.entityCount >= 65536) {
        return g_Scene.entries;
    }

    std::uint16_t current = static_cast<std::uint16_t>(key);
    std::int32_t firstFree = -1;
    if (g_Scene.keys[current] == 0 && g_Scene.chainOffsets[current] == 0) {
        g_Scene.keys[current] = key;
        ++g_Scene.entityCount;
        return &g_Scene.entries[current];
    }

    for (;;) {
        if (g_Scene.keys[current] == key) {
            return &g_Scene.entries[current];
        }
        if (g_Scene.chainOffsets[current] == 0) {
            if (firstFree != -1) {
                g_Scene.keys[firstFree] = key;
                ++g_Scene.entityCount;
                return &g_Scene.entries[firstFree];
            }

            std::uint16_t freeIndex = static_cast<std::uint16_t>(current + 1);
            while (freeIndex != current && g_Scene.keys[freeIndex] != 0) {
                ++freeIndex;
            }
            if (freeIndex == current) {
                return &g_Scene.entries[current];
            }
            ++g_Scene.entityCount;
            g_Scene.keys[freeIndex] = key;
            g_Scene.chainOffsets[current] = static_cast<std::int16_t>(freeIndex - current);
            return &g_Scene.entries[freeIndex];
        }

        current = static_cast<std::uint16_t>(current + g_Scene.chainOffsets[current]);
        if (g_Scene.keys[current] == 0) {
            if (firstFree == -1) {
                firstFree = current;
            }
            if (g_Scene.chainOffsets[current] == 0) {
                g_Scene.keys[current] = key;
                ++g_Scene.entityCount;
                return &g_Scene.entries[current];
            }
        }
    }
}

GameNode *FindSceneEntityByID(std::int32_t key) {
    std::uint16_t current = static_cast<std::uint16_t>(key);
    if (g_Scene.keys[current] == key) {
        return g_Scene.entries[current];
    }
    while (g_Scene.chainOffsets[current] != 0) {
        current = static_cast<std::uint16_t>(current + g_Scene.chainOffsets[current]);
        if (g_Scene.keys[current] == key) {
            return g_Scene.entries[current];
        }
    }
    return nullptr;
}

void MT5_RemoveSceneModel(std::int32_t key) {
    const std::uint16_t base = static_cast<std::uint16_t>(key);
    std::uint16_t current = base;
    if (g_Scene.keys[current] != key) {
        while (g_Scene.chainOffsets[current] != 0) {
            current = static_cast<std::uint16_t>(current + g_Scene.chainOffsets[current]);
            if (g_Scene.keys[current] == key) {
                break;
            }
        }
        if (g_Scene.keys[current] != key) {
            return;
        }
    }

    GameNode *const node = g_Scene.entries[current];
    if (!node) {
        return;
    }
    ValidateAndCommitBuffer(node->bufferHandle);
    D3T_RemoveSceneNodeWorldTransform(node);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(node), SHEN_NAME(PAKM));
    FreeSceneSpace(current);
}

void Script_TranslateRotateScale_Wrapper(ScenarioContext *, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    if (!params)
        return;

    Matrix4x4_Push();
    const auto taskPointer = [](std::uint32_t offset) -> const std::byte * {
        return offset && g_pCurrentTaskData ? g_pCurrentTaskData + static_cast<std::int32_t>(offset) : nullptr;
    };
    const std::byte *const positionBytes = taskPointer(params[2]);
    const std::byte *const rotationBytes = taskPointer(params[3]);
    const std::byte *const scaleBytes = taskPointer(params[4]);

    if (positionBytes) {
        Vector3 position{};
        std::memcpy(&position, positionBytes, sizeof(position));
        Matrix4x4::preTranslate(position.x, position.y, position.z);
    }
    if (rotationBytes) {
        Vector3i rotation{};
        std::memcpy(&rotation, rotationBytes, sizeof(rotation));
        if (rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
        if (rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        if (rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    }
    if (scaleBytes) {
        Vector3 scale{};
        std::memcpy(&scale, scaleBytes, sizeof(scale));
        g_scriptTransformScale = scale;
        Matrix4x4::Scale(scale.x, scale.y, scale.z);
    }
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s1
