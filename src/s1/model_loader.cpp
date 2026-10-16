#include "s1/d3t/namespace.hpp"
#include "s1/task/pack.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "d3t/shader_load.hpp"
#include "d3t/model_paths.hpp"
#include "s1/archive_data.hpp"
#include "s1/face.hpp"
#include "s1/frame_pipeline.hpp"

#include "common/types.hpp"
#include <shendk/files/model/mt5_reader.h>

#include "s1/task/char.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/dynamic_map.hpp"
#include "common/math.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"
#include "s1/file_loader.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/render_array.hpp"
#include "s1/probed_hash.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/scene.hpp"
#include "s1/skinning_cache.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

namespace shenmue::s1 {

namespace {
std::byte *g_mt5ModelBaseOverride = nullptr;
const Matrix4x4 *g_mt5RootTransform = nullptr;
std::unordered_map<const void *, std::byte *> g_mt5ModelBases;
struct Mt5CopiedBinding {
    void *binding;
    std::byte *modelBase;
};
std::unordered_map<const void *, Mt5CopiedBinding> g_mt5CopiedBindings;
std::unordered_set<const void *> g_mt5TemporaryTaskRelativeNodes;

std::uintptr_t MT5_AllocationEnd(const void *payload) {
    if (!payload)
        return 0;
    const auto *const header = reinterpret_cast<const HLib::TaskData *>(payload) - 1;
    if (header->status != 0x20455355 || header->content_size <= static_cast<std::int64_t>(sizeof(HLib::TaskData))) {
        return 0;
    }
    return reinterpret_cast<std::uintptr_t>(header) + static_cast<std::uintptr_t>(header->content_size);
}

void MT5_UnregisterOwnershipRange(const void *first, std::uintptr_t end) {
    const auto begin = reinterpret_cast<std::uintptr_t>(first);
    if (!begin || end <= begin)
        return;
    for (auto entry = g_mt5ModelBases.begin(); entry != g_mt5ModelBases.end();) {
        const auto address = reinterpret_cast<std::uintptr_t>(entry->first);
        if (address >= begin && address < end)
            entry = g_mt5ModelBases.erase(entry);
        else
            ++entry;
    }
    for (auto entry = g_mt5CopiedBindings.begin(); entry != g_mt5CopiedBindings.end();) {
        const auto address = reinterpret_cast<std::uintptr_t>(entry->first);
        if (address >= begin && address < end)
            entry = g_mt5CopiedBindings.erase(entry);
        else
            ++entry;
    }
    for (auto entry = g_mt5TemporaryTaskRelativeNodes.begin(); entry != g_mt5TemporaryTaskRelativeNodes.end();) {
        const auto address = reinterpret_cast<std::uintptr_t>(*entry);
        if (address >= begin && address < end)
            entry = g_mt5TemporaryTaskRelativeNodes.erase(entry);
        else
            ++entry;
    }
}

} // namespace

void MT5_ResetModelOwnershipRegistry() {
    g_mt5ModelBaseOverride = nullptr;
    g_mt5RootTransform = nullptr;
    g_mt5ModelBases.clear();
    g_mt5CopiedBindings.clear();
    g_mt5TemporaryTaskRelativeNodes.clear();
}
bool MT5_IsRegisteredModelNode(const void *node) {
    return node && g_mt5ModelBases.find(node) != g_mt5ModelBases.end();
}

void MT5_UnregisterModelAllocation(const void *payload) {
    MT5_UnregisterOwnershipRange(payload, MT5_AllocationEnd(payload));
}

std::byte *MT5_ModelBase() {
    return g_mt5ModelBaseOverride ? g_mt5ModelBaseOverride : g_pCurrentTaskData;
}

std::byte *MT5_SetModelBaseOverride(std::byte *modelBase) {
    std::byte *const previous = g_mt5ModelBaseOverride;
    g_mt5ModelBaseOverride = modelBase;
    return previous;
}

std::byte *MT5_ModelBaseForNode(const void *node) {
    const auto found = g_mt5ModelBases.find(node);
    return found == g_mt5ModelBases.end() ? g_pCurrentTaskData : found->second;
}

void *MT5_SceneBindingForNode(const void *nodePointer, std::byte **modelBase) {
    if (!nodePointer)
        return nullptr;
    const auto copied = g_mt5CopiedBindings.find(nodePointer);
    if (copied != g_mt5CopiedBindings.end()) {
        if (modelBase)
            *modelBase = copied->second.modelBase;
        return copied->second.binding;
    }
    std::int32_t sceneBindingOffset = 0;
    std::memcpy(&sceneBindingOffset, static_cast<const std::byte *>(nodePointer) + 4, sizeof(sceneBindingOffset));
    std::byte *const base = MT5_ModelBaseForNode(nodePointer);
    if (modelBase)
        *modelBase = base;
    return sceneBindingOffset ? base + sceneBindingOffset : nullptr;
}

void MT5_PrepareStaticCharacterModel(void *model) {
    auto *node = static_cast<Mt5RuntimeNode *>(model);
    while (node) {
        std::byte *bindingBase = nullptr;
        void *const binding = MT5_SceneBindingForNode(node, &bindingBase);
        HMDL_NormalizeCommandHeader(binding, bindingBase);
        std::byte *const nodeBase = MT5_ModelBaseForNode(node);
        if (!nodeBase)
            return;
        if (node->childOffset) {
            MT5_PrepareStaticCharacterModel(nodeBase + node->childOffset);
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(nodeBase + node->siblingOffset) : nullptr;
    }
}

bool HMDL_NodeOffsetsUseTaskData(const void *nodePointer) {
    return g_mt5CopiedBindings.contains(nodePointer);
}

void HMDL_SetTemporaryTaskRelativeOffsets(const void *nodePointer, bool enabled) {
    if (!nodePointer)
        return;
    if (enabled)
        g_mt5TemporaryTaskRelativeNodes.insert(nodePointer);
    else
        g_mt5TemporaryTaskRelativeNodes.erase(nodePointer);
}

void *HMDL_CycleDataForNode(const void *nodePointer) {
    if (!nodePointer)
        return nullptr;
    std::int32_t cycleOffset = 0;
    std::memcpy(&cycleOffset, static_cast<const std::byte *>(nodePointer) + 0x3c, sizeof(cycleOffset));
    const bool taskRelative = g_mt5CopiedBindings.contains(nodePointer) || g_mt5TemporaryTaskRelativeNodes.contains(nodePointer);
    std::byte *const base = taskRelative ? g_pCurrentTaskData : MT5_ModelBaseForNode(nodePointer);
    return cycleOffset && base ? base + cycleOffset : nullptr;
}

bool HMDL_SetCycleDataForNode(void *nodePointer, void *cycleData) {
    if (!nodePointer)
        return false;
    std::int32_t cycleOffset = 0;
    if (cycleData) {
        const bool taskRelative = g_mt5CopiedBindings.contains(nodePointer) || g_mt5TemporaryTaskRelativeNodes.contains(nodePointer);
        std::byte *const base = taskRelative ? g_pCurrentTaskData : MT5_ModelBaseForNode(nodePointer);
        const std::intptr_t offset = static_cast<std::byte *>(cycleData) - base;
        if (offset < INT32_MIN || offset > INT32_MAX)
            return false;
        cycleOffset = static_cast<std::int32_t>(offset);
    }
    std::memcpy(static_cast<std::byte *>(nodePointer) + 0x3c, &cycleOffset, sizeof(cycleOffset));
    return true;
}

const Matrix4x4 *HMDL_TransformForNode(const void *nodePointer, std::int32_t transformOffset) {
    if (!nodePointer || !transformOffset)
        return nullptr;
    // Runtime HMDL nodes and replacement roots temporarily borrowing one of
    // their transforms resolve that offset from the owning task. Static
    // packed nodes retain model-relative transforms.
    const bool taskRelative = g_mt5CopiedBindings.contains(nodePointer) || g_mt5TemporaryTaskRelativeNodes.contains(nodePointer);
    const std::byte *const base = taskRelative ? g_pCurrentTaskData : MT5_ModelBaseForNode(nodePointer);
    return reinterpret_cast<const Matrix4x4 *>(base + transformOffset);
}

// The offset through which HMDL_TransformForNode resolves this node to the transform (0 when it lies out of offset range).
std::int32_t HMDL_TransformOffsetForNode(const void *nodePointer, const Matrix4x4 *transform) {
    if (!nodePointer || !transform)
        return 0;
    const bool taskRelative = g_mt5CopiedBindings.contains(nodePointer) || g_mt5TemporaryTaskRelativeNodes.contains(nodePointer);
    const std::byte *const base = taskRelative ? g_pCurrentTaskData : MT5_ModelBaseForNode(nodePointer);
    const std::ptrdiff_t offset = reinterpret_cast<const std::byte *>(transform) - base;
    return offset >= INT32_MIN && offset <= INT32_MAX ? static_cast<std::int32_t>(offset) : 0;
}

void HMDL_ClearPreparationFlagRecursive(void *nodePointer) {
    if (!nodePointer)
        return;

    std::byte *const base = MT5_ModelBaseForNode(nodePointer);
    const auto clearChain = [&](auto &&self, std::byte *current) -> void {
        while (current) {
            void *binding = nullptr;
            const auto copied = g_mt5CopiedBindings.find(current);
            if (copied != g_mt5CopiedBindings.end()) {
                binding = copied->second.binding;
            } else {
                std::int32_t bindingOffset = 0;
                std::memcpy(&bindingOffset, current + 4, sizeof(bindingOffset));
                if (bindingOffset)
                    binding = base + bindingOffset;
            }
            if (binding) {
                *static_cast<std::uint32_t *>(binding) &= ~0x10U;
            }

            std::int32_t childOffset = 0;
            std::int32_t siblingOffset = 0;
            std::memcpy(&childOffset, current + 0x2c, sizeof(childOffset));
            std::memcpy(&siblingOffset, current + 0x30, sizeof(siblingOffset));
            if (childOffset)
                self(self, base + childOffset);
            current = siblingOffset ? base + siblingOffset : nullptr;
        }
    };
    clearChain(clearChain, static_cast<std::byte *>(nodePointer));
}

void MT5_ReadMeshStrip(void **node, const std::int32_t *textureIds, std::int32_t flags);

char *g_globalEntityName = nullptr;

namespace {

constexpr std::uint32_t kTexd = SHEN_NAME(TEXD);
constexpr std::uint32_t kTexl = SHEN_NAME(TEXL);
constexpr std::uint32_t kChrm = SHEN_NAME(HRCM);
constexpr std::uint32_t kPtrl = SHEN_NAME(PTRL);
constexpr std::int32_t kModelToken = 0x2049414d;
constexpr std::int32_t kTempToken = SHEN_NAME(TEMP);

using Mt5Node = Mt5RuntimeNode;
std::size_t HMDL_CountNodeChain(const Mt5Node *node, const std::byte *sourceBase) {
    std::size_t count = 0;
    while (node) {
        ++count;
        if (node->childOffset) {
            count += HMDL_CountNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + node->childOffset), sourceBase);
        }
        node = node->siblingOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + node->siblingOffset) : nullptr;
    }
    return count;
}

Mt5Node *HMDL_AppendNodeChain(const Mt5Node *source, const std::byte *sourceBase, Mt5Node *&destinationCursor, Mt5Node *destinationParent) {
    Mt5Node *first = nullptr;
    Mt5Node *previous = nullptr;
    while (source) {
        Mt5Node *const destination = destinationCursor++;
        std::memcpy(destination, source, sizeof(*destination));
        HMDL_SetCycleDataForNode(destination, HMDL_CycleDataForNode(source));
        g_mt5CopiedBindings[destination] = {
            source->sceneBindingOffset ? const_cast<std::byte *>(sourceBase) + source->sceneBindingOffset : nullptr, const_cast<std::byte *>(sourceBase)};
        if (!first)
            first = destination;
        if (previous) {
            previous->siblingOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(destination) - g_pCurrentTaskData);
        }
        destination->parentOffset = destinationParent ? static_cast<std::int32_t>(reinterpret_cast<std::byte *>(destinationParent) - g_pCurrentTaskData) : 0;
        destination->siblingOffset = 0;
        if (source->childOffset) {
            Mt5Node *const child =
                HMDL_AppendNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + source->childOffset), sourceBase, destinationCursor, destination);
            destination->childOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(child) - g_pCurrentTaskData);
        } else {
            destination->childOffset = 0;
        }
        previous = destination;
        source = source->siblingOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + source->siblingOffset) : nullptr;
    }
    return first;
}

void MT5_ClearNodeTransformOffsets(Mt5Node *node) {
    if (node->childOffset) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(MT5_ModelBase() + node->childOffset));
    }
    if (node->siblingOffset) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(MT5_ModelBase() + node->siblingOffset));
    }
    node->transformOffset = 0;
}

} // namespace

void *hmdlAssignFor(PackModel *sourceModel, std::int32_t childIndex, std::int32_t nodeKind, std::int32_t occurrence) {
    if (!sourceModel || !sourceModel->model)
        return nullptr;
    const auto *source = static_cast<const Mt5Node *>(sourceModel->model);
    const std::byte *const sourceBase = MT5_ModelBaseForNode(source);



    if (childIndex != 0) {
        if (!source->childOffset)
            return nullptr;
        source = reinterpret_cast<const Mt5Node *>(sourceBase + source->childOffset);
        while (childIndex > 1) {
            if (!source->siblingOffset)
                return nullptr;
            source = reinterpret_cast<const Mt5Node *>(sourceBase + source->siblingOffset);
            --childIndex;
        }
    }
    if (nodeKind != -1 && (source->nodeKind == 62 || static_cast<std::int16_t>(source->nodeKind) > 0x7000)) {
        const Mt5Node *container = source->childOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + source->childOffset) : nullptr;
        while (container && static_cast<std::int16_t>(container->nodeKind) != nodeKind) {
            container = container->siblingOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + container->siblingOffset) : nullptr;
        }
        const Mt5Node *selected = container && container->childOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + container->childOffset) : nullptr;
        while (selected && occurrence > 0) {
            selected = selected->siblingOffset ? reinterpret_cast<const Mt5Node *>(sourceBase + selected->siblingOffset) : nullptr;
            --occurrence;
        }
        if (selected)
            source = selected;
    }



    const std::size_t nodeCount =
        1 + (source->childOffset ? HMDL_CountNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + source->childOffset), sourceBase) : 0);
    auto *const allocation = static_cast<Mt5Node *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(nodeCount * sizeof(Mt5Node)), SHEN_NAME(HMDL)));
    if (!allocation)
        return nullptr;

    Mt5Node *cursor = allocation;
    std::memcpy(cursor, source, sizeof(*cursor));
    Mt5Node *const root = cursor++;
    HMDL_SetCycleDataForNode(root, HMDL_CycleDataForNode(source));
    g_mt5CopiedBindings[root] = {
        source->sceneBindingOffset ? const_cast<std::byte *>(sourceBase) + source->sceneBindingOffset : nullptr, const_cast<std::byte *>(sourceBase)};
    root->parentOffset = 0;
    root->siblingOffset = 0;




    root->transformOffset = 0;
    if (source->childOffset) {
        Mt5Node *const child = HMDL_AppendNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + source->childOffset), sourceBase, cursor, root);
        root->childOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(child) - g_pCurrentTaskData);
    } else {
        root->childOffset = 0;
    }
    SetupEngineObject(reinterpret_cast<std::uintptr_t>(sourceModel->model), reinterpret_cast<std::uintptr_t>(root), 0);
    return root;
}


std::size_t HMDL_CountModelNodes(const Mt5Node *model, const std::byte *sourceBase) {
    if (!model)
        return 0;
    std::size_t count = 1;
    if (model->childOffset)
        count += HMDL_CountNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + model->childOffset), sourceBase);
    if (model->siblingOffset)
        count += HMDL_CountNodeChain(reinterpret_cast<const Mt5Node *>(sourceBase + model->siblingOffset), sourceBase);
    return count;
}



void *HMDL_CloneNodeTree(const void *sourceNode) {
    const auto *const source = static_cast<const Mt5Node *>(sourceNode);
    if (!source)
        return nullptr;
    const std::byte *const sourceBase = MT5_ModelBaseForNode(source);
    if (!sourceBase)
        return nullptr;

    const std::size_t nodeCount = HMDL_CountModelNodes(source, sourceBase);
    auto *const allocation = static_cast<Mt5Node *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(nodeCount * sizeof(Mt5Node)), SHEN_NAME(HMDL)));
    if (!allocation)
        return nullptr;

    Mt5Node *cursor = allocation;
    Mt5Node *const root = HMDL_AppendNodeChain(source, sourceBase, cursor, nullptr);
    g_mt5ModelBases[root] = g_pCurrentTaskData;
    return root;
}



void HMDL_FreeAssignedModel(void *model) {
    if (!model)
        return;
    auto *const header = reinterpret_cast<HLib::TaskData *>(model) - 1;
    if (header->status != 0x20455355 || header->taskToken != static_cast<std::int32_t>(SHEN_NAME(HMDL)) ||
        header->content_size <= static_cast<std::int64_t>(sizeof(HLib::TaskData))) {
        return;
    }
    if (RenderArrayEntry *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model)))) {
        if (entry->retire != 2)
            entry->retire = 1;
        g_hmdlEntryDirty = 1;
    }
    MT5_UnregisterOwnershipRange(model, MT5_AllocationEnd(model));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(model), 0);
}

namespace {



void HMDL_AliasModelTree(const Mt5Node *node, std::uintptr_t modelHandle, std::uintptr_t leg, const std::byte *base) {
    while (node) {
        InsertLargeHmdlMapping(g_renderIndexStorageA, reinterpret_cast<std::uintptr_t>(node) + leg, modelHandle + leg);
        if (node->childOffset)
            HMDL_AliasModelTree(reinterpret_cast<const Mt5Node *>(base + node->childOffset), modelHandle, leg, base);
        node = node->siblingOffset ? reinterpret_cast<const Mt5Node *>(base + node->siblingOffset) : nullptr;
    }
}

} // namespace



void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags) {
    const std::uintptr_t handle = static_cast<std::uintptr_t>(model);
    const Mt5Node *const root = reinterpret_cast<const Mt5Node *>(handle);
    const std::byte *const base = MT5_ModelBaseForNode(root);
    if (d3t::FileRead_ALL(handle, formattedPath, readMode, static_cast<std::uint16_t>(flags)))
        HMDL_AliasModelTree(root, handle, 0, base);
    if (d3t::FileRead_ALL(handle + 1, formattedPath, readMode, static_cast<std::uint16_t>(flags | 0x110)))
        HMDL_AliasModelTree(root, handle, 1, base);
}

void MT5_PrepassNodeTreeTexturesAndStrips(void **node, std::int32_t *textureIds, std::uint32_t *depth) {
    auto *current = static_cast<Mt5Node *>(*node);
    // Packed HMDL offsets are relative to the owning model allocation for
    // every node, not just the root. Runtime clip records retain arbitrary
    // child-node pointers, so preserve that ownership for those pointers too.
    g_mt5ModelBases[current] = MT5_ModelBase();
    Matrix4x4 *temporaryMatrix = nullptr;

    if (*depth != 0) {
        temporaryMatrix = static_cast<Matrix4x4 *>(HLib_AllocHeapMemBlock(sizeof(Matrix4x4), kTempToken));

        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(current->position.x, current->position.y, current->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(current->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(current->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(current->rotation.x));

        if (temporaryMatrix) {
            *temporaryMatrix = workTransformMatrix;
            current->transformOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(temporaryMatrix) - MT5_ModelBase());
        } else {
            current->transformOffset = 0;
        }
    }

    if (current->sceneBindingOffset) {
        auto *const binding = reinterpret_cast<std::uint32_t *>(MT5_ModelBase() + current->sceneBindingOffset);
        if (*binding != 1024) {
            MT5_ReadMeshStrip(node, textureIds, 0);
        }
    }

    if (current->childOffset) {
        void *child = MT5_ModelBase() + current->childOffset;
        ++*depth;
        MT5_PrepassNodeTreeTexturesAndStrips(&child, textureIds, depth);
        --*depth;
    }
    if (current->siblingOffset) {
        void *sibling = MT5_ModelBase() + current->siblingOffset;
        MT5_PrepassNodeTreeTexturesAndStrips(&sibling, textureIds, depth);
    }

    if (temporaryMatrix) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(temporaryMatrix), kTempToken);
        current->transformOffset = 0;
    }
}

PackModel *MT5_InitRawModelAndTextures(void *fileData, const char *entityName, const char *remapContext) {
    if (!fileData)
        return nullptr;
    constexpr std::uint32_t kHrcm = SHEN_NAME(HRCM);
    constexpr std::uint32_t kTexdChunk = SHEN_NAME(TEXD);
    constexpr std::uint32_t kTexnChunk = SHEN_NAME(TEXN);
    constexpr std::uint32_t kPtrlChunk = SHEN_NAME(PTRL);
    auto *const fileBase = static_cast<std::byte *>(fileData);
    const std::uintptr_t allocationEnd = MT5_AllocationEnd(fileData);
    const std::uintptr_t allocationBegin = reinterpret_cast<std::uintptr_t>(fileData);
    if (allocationEnd <= allocationBegin)
        return nullptr;
    const shendk::Bytes fileBytes{fileBase, static_cast<std::size_t>(allocationEnd - allocationBegin)};
    std::byte *const taskBase = g_pCurrentTaskData;
    Mt5Node *root = nullptr;
    std::uint32_t textureCount = 0;
    std::array<std::int32_t, 640> textureIds{};
    textureIds.fill(-1);

    auto *chunk = reinterpret_cast<PakChunkHeader *>(fileBase);
    bool relocated = false;
    for (std::uint32_t guard = 0; guard < 4096; ++guard) {
        if (!chunk || chunk->size < sizeof(PakChunkHeader))
            break;
        auto *const chunkBytes = reinterpret_cast<std::byte *>(chunk);
        if (chunk->tag == kHrcm) {
#if 0
            const std::uint32_t rootOffset = *reinterpret_cast<const std::uint32_t *>(chunkBytes + 8);
            if (rootOffset)
                root = reinterpret_cast<Mt5Node *>(fileBase + rootOffset);
#else
            shendk::Mt5Header header{};
            if (!shendk::ReadMt5Header(fileBytes, header, static_cast<std::uint32_t>(chunkBytes - fileBase)))
                return nullptr;
            if (header.firstNodeOffset)
                root = reinterpret_cast<Mt5Node *>(fileBase + header.firstNodeOffset);
#endif
        } else if (chunk->tag == kTexdChunk) {
            textureCount = std::min<std::uint32_t>(*reinterpret_cast<const std::uint32_t *>(chunkBytes + 8), static_cast<std::uint32_t>(textureIds.size()));
        } else if (chunk->tag == kTexnChunk) {
            auto *texture = chunk;
            for (std::uint32_t index = 0; index < textureCount; ++index) {
                textureIds[index] = LoadTEXN(texture);
                if (remapContext) {
                    if (const std::uint32_t textureHandle = GetPVRSurfaceTextureHandle(textureIds[index]))
                        d3t::CheckAndRemapIfNeeded(remapContext, static_cast<std::int32_t>(index), textureHandle);
                }
                if (texture->size < sizeof(PakChunkHeader))
                    break;
                texture = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(texture) + texture->size);
            }
            if (textureCount < textureIds.size())
                textureIds[textureCount] = -1;
            chunk = texture;
            continue;
        } else if (chunk->tag == kPtrlChunk) {
            const std::int32_t relocationCount = *reinterpret_cast<const std::int32_t *>(chunkBytes + 8);
            auto *const relocations = reinterpret_cast<const std::uint32_t *>(chunkBytes + 12);
            if (relocationCount > 0 && relocationCount < 0x100000) {
                for (std::int32_t index = 0; index < relocationCount; ++index) {
                    auto *const field = reinterpret_cast<std::uint32_t *>(fileBase + relocations[index]);
                    if (*field) {
                        *field = static_cast<std::uint32_t>((fileBase + *field) - taskBase);
                    }
                }
            }
            relocated = true;
            break;
        }
        chunk = reinterpret_cast<PakChunkHeader *>(chunkBytes + chunk->size);
    }
    if (!root || !relocated)
        return nullptr;

    std::byte *const previousBase = MT5_SetModelBaseOverride(taskBase);
    if (root->childOffset) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(taskBase + root->childOffset));
    }
    if (root->siblingOffset) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(taskBase + root->siblingOffset));
    }
    root->attachmentOffset = 0;

    const std::size_t allocationSize = sizeof(PackModel) + textureCount * sizeof(std::int32_t);
    auto *const model = static_cast<PackModel *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(allocationSize), kModelToken));
    if (!model) {
        MT5_SetModelBaseOverride(previousBase);
        return nullptr;
    }
    model->pakf = static_cast<PakChunkHeader *>(fileData);
    model->model = root;
    model->textureCount = textureCount;
    model->field_14 = 0;
    if (textureCount) {
        std::memcpy(model + 1, textureIds.data(), textureCount * sizeof(std::int32_t));
    }
    g_globalEntityName = const_cast<char *>(entityName);
    std::uint32_t depth = 0;
    void *rootPointer = root;
    MT5_PrepassNodeTreeTexturesAndStrips(&rootPointer, textureCount ? textureIds.data() : nullptr, &depth);
    g_globalEntityName = nullptr;
    g_mt5ModelBases[root] = taskBase;
    MT5_SetModelBaseOverride(previousBase);
    return model;
}

PackModel *PAKF_Mallocs(PakChunkHeader *pakf, std::int32_t *textureIds, void *, char *entityName, char *formattedAssetPath) {
    if (!pakf)
        return nullptr;
    PakChunkHeader *chunk = pakf;
    std::uint32_t textureCount = 0;
    std::byte *textureList = nullptr;
    std::int32_t *model = nullptr;
    bool relocated = false;

    for (std::uint32_t guard = 0; guard < 4096; ++guard) {
        if (!chunk || chunk->size < sizeof(PakChunkHeader))
            break;
        auto *const chunkBytes = reinterpret_cast<std::byte *>(chunk);
        if (chunk->tag == kTexd) {
            textureCount = std::min<std::uint32_t>(*reinterpret_cast<std::uint32_t *>(chunkBytes + 8), 640);
        } else if (chunk->tag == kTexl) {
            const std::uint32_t offset = *reinterpret_cast<std::uint32_t *>(chunkBytes + 8);
            textureList = reinterpret_cast<std::byte *>(pakf) + offset;
        } else if (chunk->tag == kChrm) {
            const std::uint32_t offset = *reinterpret_cast<std::uint32_t *>(chunkBytes + 8);
            model = reinterpret_cast<std::int32_t *>(reinterpret_cast<std::byte *>(pakf) + offset);
        } else if (chunk->tag == kPtrl) {
            const std::int32_t relocationCount = *reinterpret_cast<std::int32_t *>(chunkBytes + 8);
            if (relocationCount < 0 || relocationCount >= 0x100000)
                return nullptr;
            auto *relocation = reinterpret_cast<std::uint32_t *>(chunkBytes + 12);
            for (std::int32_t index = 0; index < relocationCount; ++index) {
                const std::uint32_t fieldOffset = relocation[index];
                auto *const field = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(pakf) + fieldOffset);
                if (*field) {
                    const auto *const pointer = reinterpret_cast<std::byte *>(pakf) + *field;
                    *field = static_cast<std::uint32_t>(pointer - g_pCurrentTaskData);
                }
            }
            relocated = true;
            break;
        }
        chunk = reinterpret_cast<PakChunkHeader *>(chunkBytes + chunk->size);
    }
    if (!relocated || !model || (textureCount && !textureList))
        return nullptr;

    std::array<std::int32_t, 640> resolvedTextures{};
    std::memset(resolvedTextures.data(), 0xff, static_cast<std::size_t>(textureCount) * sizeof(std::int32_t));
    for (std::uint32_t index = 0; index < textureCount; ++index) {
        const std::int32_t textureOffset = *reinterpret_cast<std::int32_t *>(textureList + index * 12);
        auto *const texture = reinterpret_cast<std::int32_t *>(g_pCurrentTaskData + textureOffset);
        const std::int32_t surface = PVR_FindSurface(texture[0], texture[1]);
        resolvedTextures[index] = surface;
        if (formattedAssetPath) {
            if (const std::uint32_t textureHandle = GetPVRSurfaceTextureHandle(surface))
                d3t::CheckAndRemapIfNeeded(formattedAssetPath, static_cast<std::int32_t>(index), textureHandle);
        }
    }

    if (model[11]) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(g_pCurrentTaskData + model[11]));
    }
    if (model[12]) {
        MT5_ClearNodeTransformOffsets(reinterpret_cast<Mt5Node *>(g_pCurrentTaskData + model[12]));
    }
    model[15] = 0;

    const std::size_t allocationSize = textureIds ? sizeof(PackModel) + textureCount * sizeof(std::int32_t) : sizeof(PackModel);
    auto *const result = static_cast<PackModel *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(allocationSize), kModelToken));
    if (!result)
        return nullptr;
    if (textureIds && textureCount) {
        std::memcpy(result + 1, resolvedTextures.data(), textureCount * sizeof(std::int32_t));
    }
    result->pakf = pakf;
    result->model = model;
    result->textureCount = textureCount;
    result->field_14 = 0;
    // Every pointer stored by the packed MT5 is an offset from the task-data
    // allocation which owned the PAKS while PAKF_Mallocs relocated it.  The
    // model is submitted later from a different HLib task, so retain that base
    // with the root instead of depending on the current task-data global.
    g_mt5ModelBases[model] = g_pCurrentTaskData;

    g_globalEntityName = entityName;
    std::uint32_t depth = 0;
    void *modelPointer = model;
    MT5_PrepassNodeTreeTexturesAndStrips(&modelPointer, textureIds ? resolvedTextures.data() : nullptr, &depth);
    g_globalEntityName = nullptr;
    return result;
}

bool MT5_LoadRawModel(const char *rootDirectory, const char *filename, const std::int32_t *textureIds) {
    return MT5_LoadRawModelPlaced(rootDirectory, filename, textureIds, 0.0f, 0.0f, 0.0f, 0);
}

bool MT5_LoadRawModelPlaced(const char *rootDirectory, const char *filename, const std::int32_t *textureIds, float x, float y, float z, std::int16_t yaw) {
#if 0
    struct RawHeader {
        std::uint32_t signature;
        std::uint32_t nodesSize;
        std::uint32_t firstNodeOffset;
    };
#endif
    void *const bytes = ReadFile(rootDirectory, filename, kModelToken);
    if (!bytes)
        return false;
    std::uint32_t firstNodeOffset = 0;
#if 0
    const auto *const header = static_cast<const RawHeader *>(bytes);
    if ((header->signature != SHEN_NAME(HRCM) && header->signature != SHEN_NAME(HM5M)) || !header->firstNodeOffset) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
        return false;
    }
    firstNodeOffset = header->firstNodeOffset;
#else
    const std::uintptr_t allocationEnd = MT5_AllocationEnd(bytes);
    const std::uintptr_t allocationBegin = reinterpret_cast<std::uintptr_t>(bytes);
    shendk::Mt5Header header{};
    if (allocationEnd <= allocationBegin ||
        !shendk::ReadMt5Header({static_cast<const std::byte *>(bytes), static_cast<std::size_t>(allocationEnd - allocationBegin)}, header) ||
        !header.firstNodeOffset) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
        return false;
    }
    firstNodeOffset = header.firstNodeOffset;
#endif
    std::byte *const previousBase = g_mt5ModelBaseOverride;
    const Matrix4x4 *const previousRoot = g_mt5RootTransform;
    g_mt5ModelBaseOverride = static_cast<std::byte *>(bytes);
    constexpr float kTwoPi = 6.2831853071795864769f;
    const float angle = static_cast<float>(static_cast<std::uint16_t>(yaw)) * (kTwoPi / 65536.0f);
    const float sine = std::sin(angle);
    const float cosine = std::cos(angle);
    const Matrix4x4 rootTransform{{
        {cosine, 0.0f, -sine, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {sine, 0.0f, cosine, 0.0f},
        {x, y, z, 1.0f},
    }};
    g_mt5RootTransform = &rootTransform;
    char entityName[6]{};
    for (std::size_t index = 0; index < 5 && filename[index] && filename[index] != '.'; ++index) {
        const unsigned char character = static_cast<unsigned char>(filename[index]);
        entityName[index] = static_cast<char>(character >= 'a' && character <= 'z' ? character - 32 : character);
    }
    g_globalEntityName = entityName;
    std::uint32_t depth = 0;
    void *rootNode = g_mt5ModelBaseOverride + firstNodeOffset;
    MT5_PrepassNodeTreeTexturesAndStrips(&rootNode, const_cast<std::int32_t *>(textureIds), &depth);
    g_globalEntityName = nullptr;
    g_mt5RootTransform = previousRoot;
    g_mt5ModelBaseOverride = previousBase;
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(rootNode);
    ReleaseHmdlRuntimeObject(handle);
    ReleaseHmdlRuntimeObject(handle + 1);
    MT5_UnregisterOwnershipRange(bytes, MT5_AllocationEnd(bytes));
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
    return true;
}

bool MT5_LoadCharacterModel(Character *character, const char *rootDirectory, const char *filename, const std::int32_t *textureIds) {
    (void)textureIds;
    if (!character || !filename)
        return false;
    void *const bytes = ReadFile(rootDirectory, filename, kModelToken);
    if (!bytes)
        return false;
    char entityName[6]{};
    for (std::size_t index = 0; index < 5 && filename[index] && filename[index] != '.'; ++index) {
        const unsigned char value = static_cast<unsigned char>(filename[index]);
        entityName[index] = static_cast<char>(value >= 'a' && value <= 'z' ? value - 32 : value);
    }
    PackModel *const model = MT5_InitRawModelAndTextures(bytes, entityName, nullptr);
    if (!model) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
        return false;
    }

    char registryName[9]{};
    std::memcpy(registryName, entityName, sizeof(entityName));
    if (!MDLM_RegisterImage(registryName, filename, model, 0, nullptr, true)) {
        FreePVR(model);
        return false;
    }
    return MDLM_ActivateCharacterImage(character, registryName, 0);
}

namespace d3t {


PackModel *ModelLoad_FBX_Replace(const char *rootDirectory, const char *filename) {
    void *const file = ReadFile(rootDirectory, filename, 0x204c444d);
    char path[256];
    const int length = std::sprintf(path, "/%s/%s_/%s", rootDirectory, filename, filename);
    std::memcpy(path + length - 3, "fbx", 4);
    PackModel *const model = MT5_InitRawModelAndTextures(file, filename, path);
    if (!model) {
        if (file)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return nullptr;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), path, 0, 34);
    const std::int16_t kind = model->model ? static_cast<std::int16_t>(static_cast<Mt5RuntimeNode *>(model->model)->nodeKind) : 0;
    model->field_14 = kind > 28672 ? kind - 0x7001 : 0;
    return model;
}
} // namespace d3t

PackModel *MT5_LoadPackModel(const char *rootDirectory, const char *filename) {
    if (!rootDirectory || !filename)
        return nullptr;
    void *const bytes = ReadFile(rootDirectory, filename, kModelToken);
    if (!bytes)
        return nullptr;
    PackModel *const model = MT5_InitRawModelAndTextures(bytes, filename, nullptr);
    if (!model)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
    return model;
}

RenderArrayEntry *CheckRenderArrayEntryGeneration(std::uint32_t value) {
    if (!g_renderPacketStorage)
        return nullptr;
    constexpr std::uint32_t kIndexMask = 0x3ff;
    constexpr std::uint32_t kGenerationMask = 0xfffffc00;
    constexpr std::size_t kEntryStride = 0x85f8;
    constexpr std::size_t kGenerationOffset = 0x217e000;

    auto *const storage = static_cast<std::byte *>(g_renderPacketStorage);
    const std::uint32_t index = value & kIndexMask;
    std::uint32_t generation{};
    std::memcpy(&generation, storage + kGenerationOffset + index * sizeof(generation), sizeof(generation));
    if (generation != (value & kGenerationMask))
        return nullptr;
    return reinterpret_cast<RenderArrayEntry *>(storage + static_cast<std::size_t>(index) * kEntryStride);
}

RenderArrayEntry *GetRenderArrayEntry(std::uint32_t index) {
    if (index >= 1024 || !g_renderLookupStorage)
        return nullptr;

    const auto *const storage = static_cast<const std::byte *>(g_renderLookupStorage);
    const auto *const values = reinterpret_cast<const std::uint32_t *>(storage);
    const auto *const hashes = reinterpret_cast<const std::uint32_t *>(storage + 0x1000);
    const auto *const probes = reinterpret_cast<const std::uint16_t *>(storage + 0x2000);

    const std::uint32_t hash = hashes[index];
    if (hash == 0)
        return nullptr;

    std::uint32_t slot = hash & 0x3ffU;
    while (hashes[slot] != hash) {
        const std::uint16_t distance = probes[slot];
        if (distance == 0)
            return CheckRenderArrayEntryGeneration(0);
        slot = (slot + distance) & 0x3ffU;
    }
    return CheckRenderArrayEntryGeneration(values[slot]);
}

namespace {


constexpr std::array<const char *, 16> kHeadRigNodeNames{
    "L_Eye_Hi_Blink_Bn",
    "HeadRig:L_Eye_Hi_Blink_Bn",
    "L_Eye_Bn",
    "HeadRig:L_Eye_Bn",
    "L_Eye_Lo_Blink_Bn",
    "HeadRig:L_Eye_Lo_Blink_Bn",
    "R_Eye_Hi_Blink_Bn",
    "HeadRig:R_Eye_Hi_Blink_Bn",
    "R_Eye_Bn",
    "HeadRig:R_Eye_Bn",
    "R_Eye_Lo_Blink_Bn",
    "HeadRig:R_Eye_Lo_Blink_Bn",
    "head_geo",
    nullptr,
    "eyelashes_geo",
    nullptr,
};


constexpr std::array<const char *, 8> kEyeModelNodes{
    "Node_LeftEyeLidUpper",
    "Node_LeftEye",
    "Node_LeftEyeLidLower",
    "Node_RightEyeLidUpper",
    "Node_RightEye",
    "Node_RightEyeLidLower",
    "Mesh_Head",
    "Mesh_Eyelashes",
};


bool D3T_ReadModelSetting(std::uint32_t modelHandle, const char *key, Json::Value &value) {
    const char *const settings = d3t::GetModelSettings(modelHandle);
    if (!settings)
        return false;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    if (!reader->parse(settings, settings + std::strlen(settings), &root, nullptr) || !root.isObject())
        return false;
    for (const std::string &name : root.getMemberNames()) {
        if (D3T_SettingsKeyMatches(name.c_str(), key)) {
            value = root[name];
            return true;
        }
    }
    return false;
}

std::uint32_t NormalizeHmdlHandle(std::uintptr_t handle) {
    return static_cast<std::uint32_t>(handle + handle / std::uint64_t{0xffffffffU});
}

std::uintptr_t LookupLargeHmdlMapping(const void *mappingStorage, std::uintptr_t handle) {
    if (!mappingStorage || handle == 0)
        return handle;
    const HmdlHandleRemap &remap = *static_cast<const HmdlHandleRemap *>(mappingStorage);
    const std::uint32_t hash = NormalizeHmdlHandle(handle);
    std::uint32_t slot = hash & 0x3fffU;
    while (remap.hashes[slot] != hash) {
        const std::uint16_t distance = remap.probes[slot];
        if (distance == 0)
            return handle;
        slot = (slot + distance) & 0x3fffU;
    }
    return remap.values[slot];
}

bool RemoveLargeHmdlMapping(void *mappingStorage, std::uint32_t hash) {
    HmdlHandleRemap &remap = *static_cast<HmdlHandleRemap *>(mappingStorage);
    return RemoveProbedHash(remap, 0x3fffU, hash, [&remap](std::uint32_t slot) { remap.values[slot] = 0; });
}


void RemoveLargeHmdlMappingsTo(void *mappingStorage, std::uintptr_t handle) {
    HmdlHandleRemap &remap = *static_cast<HmdlHandleRemap *>(mappingStorage);
    for (std::uint32_t slot = 0; slot < 0x4000; ++slot) {
        if (remap.values[slot] == handle)
            RemoveLargeHmdlMapping(mappingStorage, remap.hashes[slot]);
    }
}

bool InsertHmdlEntryLookup(std::uint32_t id, std::uint32_t hash) {
    HmdlEntryLookup &lookup = *static_cast<HmdlEntryLookup *>(g_renderLookupStorage);
    return InsertProbedHash(lookup, 0x3ffU, 1024, hash, [&lookup, id](std::uint32_t slot) { lookup.values[slot] = id; });
}

bool RemoveHmdlEntryLookup(std::uint32_t hash) {
    HmdlEntryLookup &lookup = *static_cast<HmdlEntryLookup *>(g_renderLookupStorage);
    return RemoveProbedHash(lookup, 0x3ffU, hash, [&lookup](std::uint32_t slot) { lookup.values[slot] = 0; });
}

bool InsertHmdlRuntimeObject(const RenderArrayOwner &record, std::uint32_t hash) {
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    return InsertProbedHash(objects, 0x3ffU, 1024, hash, [&objects, &record](std::uint32_t slot) { objects.objects[slot] = record; });
}



void RemoveSkinningTexSortRecords(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    SkinningTexSortBatch &batch = *reinterpret_cast<SkinningTexSortBatch *>(reinterpret_cast<std::byte *>(slot) + 0xb8);
    const std::uint32_t nodeHash = static_cast<std::uint32_t>(handle >> 32U);
    SkinningTexSortRecord *const records = reinterpret_cast<SkinningTexSortRecord *>(batch.records);
    for (std::uint32_t index = 0; index < batch.recordCount; ++index) {
        if (records[index].baseBoneHash != nodeHash)
            continue;
        ReleaseSkinningTexSortRecord(&records[index]);
        std::memmove(&records[index], &records[batch.recordCount - 1], sizeof(SkinningTexSortRecord));
        --batch.recordCount;
    }
}



void InitializeHmdlHeadRigNodes(RenderArrayEntry &entry) {
    entry.headRigNodes = static_cast<std::uint64_t *>(std::malloc(64));
    for (std::size_t pair = 0; pair < 8; ++pair) {
        entry.headRigNodes[pair] = 0;
        for (std::size_t variant = 0; variant < 2; ++variant) {
            const char *const name = kHeadRigNodeNames[pair * 2 + variant];
            if (!entry.headRigNodes[pair] && name)
                entry.headRigNodes[pair] = AssignNode(entry.handle, name);
        }
    }
    Json::Value markers;
    if (D3T_ReadModelSetting(entry.owner->renderHandle, "ModelMarkers", markers) && markers.isObject()) {
        const std::vector<std::string> names = markers.getMemberNames();
        for (std::size_t pair = 0; pair < kEyeModelNodes.size(); ++pair) {
            for (const std::string &name : names) {
                if (!D3T_SettingsKeyMatches(name.c_str(), kEyeModelNodes[pair]))
                    continue;
                const Json::Value &marker = markers[name];
                if (const std::uint64_t node = marker.isString() ? AssignNode(entry.handle, marker.asCString()) : 0)
                    entry.headRigNodes[pair] = node;
                break;
            }
        }
    }
    for (const std::size_t pair : {std::size_t{7}, std::size_t{6}}) {
        const std::uint64_t node = entry.headRigNodes[pair];
        if (node && (node >> 32U))
            ModifyArrayElementsByMurmurHash(node, 0x20, 0);
    }
}

void CollectHmdlNodesByName(std::uint64_t node, const char *suffix, std::vector<std::uint64_t> &nodes) {
    const char *const name = GetSkinningNodeName(node);
    if (name && std::strstr(name, suffix))
        nodes.push_back(node);
    const std::uint32_t childCount = GetSkinningNodeChildCount(node);
    for (std::uint32_t child = 0; child < childCount; ++child) {
        const std::uint64_t childHandle = GetSkinningNodeChild(node, child);
        if (childHandle != node)
            CollectHmdlNodesByName(childHandle, suffix, nodes);
    }
}



void MarkHmdlDayNightNodes(std::uint64_t handle) {
    constexpr const char *kSuffixes[2] = {"_day", "_night"};
    constexpr std::uint16_t kFlags[2] = {0x40, 0x80};
    for (std::uint32_t pass = 0; pass < 2; ++pass) {
        std::vector<std::uint64_t> nodes;
        CollectHmdlNodesByName(handle, kSuffixes[pass], nodes);
        for (const std::uint64_t node : nodes)
            ModifyArrayElementsByMurmurHash(node, kFlags[pass], 0);
    }
}


void RemoveCubeProbeSlotsForEntry(RenderArrayEntry &entry) {
    std::int32_t slot = 0;
    while (slot < g_FramePacingFactor) {
        if (g_effectProbeSlots[static_cast<std::size_t>(slot)].owner != &entry) {
            ++slot;
            continue;
        }
        const EffectProbeSlot removed = g_effectProbeSlots[static_cast<std::size_t>(slot)];
        for (std::int32_t next = slot; next < g_FramePacingFactor - 1; ++next)
            g_effectProbeSlots[static_cast<std::size_t>(next)] = g_effectProbeSlots[static_cast<std::size_t>(next + 1)];
        g_effectProbeSlots[static_cast<std::size_t>(g_FramePacingFactor - 1)] = removed;
        --g_FramePacingFactor;
        g_effectProbeSlots[static_cast<std::size_t>(g_FramePacingFactor)].owner = nullptr;
    }
}



void SetupHmdlOwnerProbes(RenderArrayEntry &entry) {
    const D3TModelRecord *const model = entry.owner ? d3t::ResolveModel(entry.owner->renderHandle) : nullptr;
    const std::int32_t probeCount = model ? static_cast<std::int32_t>(model->probeCount) : 0;
    if (!g_FramePacingFactor) {
        EffectProbeSlot &slot = g_effectProbeSlots[0];
        slot.node = 0;
        slot.parameter08 = 0;
        slot.parameter10 = 0;
        slot.parameter14 = 0;
        slot.parameter1c = 0;
        slot.parameter20[0] = 1.0f;
        slot.parameter20[1] = 1.0f;
        slot.parameter00 = 99999.0f;
        slot.parameter04 = 99999.0f;
        slot.parameter2c = 0;
        slot.parameter28 = 1.0f;
        slot.parameter30 = 0;
        ++g_FramePacingFactor;
    }
    const std::int32_t first = g_FramePacingFactor;
    if (first + probeCount >= 32)
        return;
    for (std::int32_t probe = 0; probe < probeCount; ++probe) {
        char name[256]{};
        std::snprintf(name, sizeof(name), "Probe %d", probe);
        const D3TModelProbe &source = model->probes[probe];
        EffectProbeSlot &slot = g_effectProbeSlots[static_cast<std::size_t>(first + probe)];
        slot.node = AssignNode(entry.handle, name);
        slot.cacheHandle = entry.handle;
        slot.owner = &entry;
        slot.parameter00 = source.parameter00;
        slot.parameter04 = source.parameter04;
        slot.parameter08 = source.parameter08;
        slot.parameter10 = source.parameter10;
        slot.parameter14 = source.parameter14;
        slot.parameter1c = source.parameter1c;
        slot.parameter2c = source.parameter2c;
        slot.parameter20[0] = source.parameter20[0];
        slot.parameter20[1] = source.parameter20[1];
        slot.parameter28 = source.parameter28;
        slot.parameter30 = source.parameter30;
    }
    g_FramePacingFactor = first + probeCount;
}




void RebindHmdlRenderEntry(std::uintptr_t modelData, RenderArrayEntry &entry) {
    if (entry.modelData) {
        if (entry.state == 1) {

            RemoveCubeProbeSlotsForEntry(entry);
            if (RenderArrayEntry *const bound = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(entry.modelData)))
                std::memset(bound->rigState, 0, sizeof(bound->rigState));


        }
        RemoveLargeHmdlMappingsTo(g_renderIndexStorageA, entry.modelData);
        RemoveHmdlEntryLookup(NormalizeHmdlHandle(entry.modelData));
    }
    entry.modelData = modelData;
    if (modelData) {
        if (entry.state == 1) {
            MarkHmdlDayNightNodes(entry.handle);

            SetupHmdlOwnerProbes(entry);
            if (entry.owner->remapType == 1)
                HMDL_InitCEWP(entry);

            ModifyArrayElementsByMurmurHash(entry.handle, 1, 1);
        }
        InsertHmdlEntryLookup(entry.id, NormalizeHmdlHandle(modelData));
    } else if (entry.state == 1) {
        ModifyArrayElementsByMurmurHash(entry.handle, 1, 0);
    }
}



bool RealizeHmdlRenderEntry(RenderArrayEntry &entry) {
    if (entry.state)
        return entry.state == 1;
    RenderArrayOwner *const object = entry.owner;
    if (!object || object->state == 2) {
        entry.state = 2;
        return false;
    }
    if (object->state != 1)
        return entry.state == 1;
    if (!entry.handle || !(entry.handle >> 32U)) {
        std::uint64_t cache = g_mainSkinningRoot;
        if ((object->flags & 0x10U) != 0U)
            cache = g_sdOverrideSkinningRoot;
        else if ((entry.flags & 2U) != 0U)
            cache = g_skySkinningRoot;
        else if ((entry.flags & 4U) != 0U)
            cache = g_skyProbeSkinningRoot;
        if (!static_cast<std::uint32_t>(cache) ||
            !(entry.handle = AppendModelToCachedSkinningBuffer(static_cast<std::uint32_t>(cache), object->renderHandle)) || !(entry.handle >> 32U)) {
            entry.state = 2;
            return false;
        }
        entry.state = 1;
    }
    if ((object->flags & 2U) != 0U)
        ModifyArrayElementsByMurmurHash(entry.handle, 2, 0);
    if (object->remapType == 1)
        ModifyArrayElementsByMurmurHash(entry.handle, 16, 0);
    if ((object->flags & 0x1000U) != 0U)
        ModifyArrayElementsByMurmurHash(entry.handle, 256, 0);
    InitializeHmdlHeadRigNodes(entry);
    const std::uintptr_t modelData = entry.modelData;
    entry.modelData = 0;
    RebindHmdlRenderEntry(modelData, entry);
    return entry.state == 1;
}



bool RetireHmdlRenderEntry(RenderArrayEntry &entry) {
    RenderArrayOwner *const object = entry.owner;
    const bool keep = object && (object->childCount <= 1 || (object->flags & 0x4000U) != 0U) && entry.retire != 2;
    if (object && !keep && object->childCount) {
        std::uint32_t index = 0;
        do {
            if (object->children[index] == &entry) {
                object->children[index] = object->children[object->childCount - 1];
                --object->childCount;
            }
            ++index;
        } while (index < object->childCount);
    }
    RebindHmdlRenderEntry(0, entry);
    if (entry.state == 1) {
        if (keep) {
            entry.retire = 0;
            return true;
        }
        std::free(entry.headRigNodes);
        entry.headRigNodes = nullptr;
        RemoveSkinningTexSortRecords(entry.handle);
        RemoveSkinningNode(entry.handle);
    } else if (keep) {
        entry.retire = 0;
        return true;
    }
    HmdlEntryStorage &storage = *static_cast<HmdlEntryStorage *>(g_renderPacketStorage);
    std::uint32_t &generation = storage.generations[entry.id & 0x3ffU];
    if (generation == (entry.id & 0xfffffc00U))
        generation &= 0x7fffffffU;
    std::memset(&entry, 0, sizeof(entry));
    return true;
}






bool ResolveHmdlRuntimeAsset(RenderArrayOwner &object, bool setupShaders) {
    if (object.state)
        return object.state == 1;
    if (d3t_IsRenderHandlePending(object.instanceHandle))
        return false;
    const D3TModelLoad *const load = static_cast<const D3TModelLoad *>(d3t::d3t_GetAsyncLoadRecord(object.loadJob));
    object.renderHandle = load && d3t::ResolveModel(load->modelHandle) ? load->modelHandle : 0U;
    if (setupShaders && !d3t::SetupShaderReferences(object.renderHandle))
        return object.state == 1;
    d3t_CloseRenderHandle(object.instanceHandle);
    object.loadJob = 0;
    if (!object.renderHandle) {
        object.state = 2;
        if (object.resolveCallback)
            object.resolveCallback(object.resolveArgument, &object);
        return false;
    }
    object.state = 1;
    if (object.resolveCallback)
        object.resolveCallback(object.resolveArgument, &object);
    Json::Value dedicatedShadow;
    if (D3T_ReadModelSetting(object.renderHandle, "DedicatedShadow", dedicatedShadow) && dedicatedShadow.isInt() && dedicatedShadow.asInt() == 1)
        object.flags |= 4U;
    return object.state == 1;
}



bool RetireHmdlRuntimeObject(RenderArrayOwner &object) {
    if (object.childCount) {
        for (std::uint32_t index = 0; index < object.childCount; ++index) {
            if (RenderArrayEntry *const child = object.children[index])
                child->retire = 2;
        }
        return false;
    }
    d3t::ReleaseModelHandle(object.renderHandle);
    const std::uintptr_t handle = object.handle;
    RemoveLargeHmdlMappingsTo(g_renderIndexStorageA, handle);
    RemoveLargeHmdlMappingsTo(g_renderIndexStorageB, handle);
    RemoveHmdlRuntimeObject(static_cast<std::uint32_t>(handle % 0xffffffffULL));
    return true;
}

} // namespace


bool InsertLargeHmdlMapping(void *mappingStorage, std::uintptr_t handle, std::uintptr_t target) {
    HmdlHandleRemap &remap = *static_cast<HmdlHandleRemap *>(mappingStorage);
    return InsertProbedHash(remap, 0x3fffU, 0x4000U, NormalizeHmdlHandle(handle), [&remap, target](std::uint32_t slot) { remap.values[slot] = target; });
}


bool RemoveHmdlRuntimeObject(std::uint32_t hash) {
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    return RemoveProbedHash(objects, 0x3ffU, hash, [&objects](std::uint32_t slot) { objects.objects[slot] = {}; });
}



RenderArrayOwner BuildHmdlRuntimeObjectRecord(std::uintptr_t handle, const char *path, std::uint32_t instanceHandle, std::uint16_t flags) {
    RenderArrayOwner record{};
    std::strcpy(record.path, path);
    record.handle = handle;
    record.flags = flags;
    record.state = 2;
    record.referenceCount = 1;
    record.pathHash = d3t_GenerateHash(record.path);
    if ((flags & 0x10U) != 0U) {
        if (!instanceHandle)
            instanceHandle = d3t_AllocateRenderHandle();
        record.instanceHandle = instanceHandle;
        record.loadJob = d3t::DispatchModelLoad(path, instanceHandle);
        record.state = 0;
    }
    return record;
}

namespace {

// The flag-0x20 tail of d3t_FileRead_ALL: set up an unbound spare entry once every entry of the object is bound.
bool SetupHmdlSpareEntry(std::uintptr_t handle) {
    const RenderArrayOwner *const object = reinterpret_cast<const RenderArrayOwner *>(ResolveHmdlRuntimeObject(handle));
    if (!object)
        return true;
    for (std::uint32_t index = 0; index < object->childCount; ++index) {
        if (!object->children[index]->modelData)
            return true;
    }
    SetupEngineObject(handle, 0, 0);
    return true;
}

} // namespace

namespace d3t {






bool FileRead_ALL(std::uintptr_t handle, const char *path, std::uint32_t instanceHandle, std::uint16_t flags,
    void (*resolveCallback)(std::uint64_t argument, RenderArrayOwner *object)) {
    const std::uint32_t normalized = NormalizeHmdlHandle(handle);
    if (RenderArrayOwner *const object = reinterpret_cast<RenderArrayOwner *>(ResolveHmdlRuntimeObject(handle))) {
        if (!object->retire)
            return false;
        object->retire = 0;
        if ((flags & 0x20U) != 0U)
            return SetupHmdlSpareEntry(handle);
        SetupEngineObject(handle, handle, 0);
        return true;
    }
    char resolved[0x200]{'/'};
    std::uint32_t remapType = 4;
    if ((flags & 0x80U) != 0U) {
        std::strcat(resolved, path);
    } else {
        char remapped[272]{};
        remapType = static_cast<std::uint32_t>(shenmue::d3t::RemapModelPath(path, remapped));
        std::strcpy(resolved, remapped);
    }
    FilepathCleaner(resolved);
    if ((flags & 0x10U) != 0U) {
        std::size_t end = std::strlen(resolved);
        const char *name = resolved;
        if (end > 1) {
            while (true) {
                if (resolved[end - 1] == '/' || resolved[end - 1] == '\\') {
                    name = resolved + end;
                    break;
                }
                if (end-- == 1 || end == 1)
                    break;
            }
        }
        char stem[64]{};
        std::snprintf(stem, sizeof(stem), "%s", name);
        if (char *const dot = std::strstr(stem, "."))
            *dot = '\0';
        char overridePath[272]{};
        std::sprintf(overridePath, "/modelOverride/%s/%s.fbx", stem, stem);
        FilepathCleaner(overridePath);
        if (!d3t::CheckForFile(overridePath))
            return false;
        std::strcpy(resolved, overridePath);
    }

    const std::uint32_t pathHash = d3t_GenerateHash(resolved);
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    RenderArrayOwner *shared = nullptr;
    for (std::uint32_t slot = 0; slot < 1024; ++slot) {
        if (objects.hashes[slot] && objects.objects[slot].pathHash == pathHash) {
            shared = &objects.objects[slot];
            break;
        }
    }
    if ((flags & 0x800U) != 0U || !shared) {
        if (objects.count >= 1024)
            return false;
        RenderArrayOwner record = BuildHmdlRuntimeObjectRecord(handle, resolved, instanceHandle, flags);
        record.remapType = remapType;
        record.resolveArgument = 0;
        record.resolveCallback = resolveCallback;
        InsertHmdlRuntimeObject(record, normalized);
        g_hmdlEntryDirty = 1;
    } else {
        ++shared->referenceCount;
        shared->retire = 0;
        InsertLargeHmdlMapping(g_renderIndexStorageB, handle, shared->handle);
    }
    if ((flags & 0x30U) == 0x20U)
        return SetupHmdlSpareEntry(handle);
    SetupEngineObject(handle, handle, 0);
    return true;
}
} // namespace d3t

std::byte *ResolveHmdlEntry(std::uintptr_t handle) {
    if (!g_renderLookupStorage || handle == 0)
        return nullptr;
    handle = LookupLargeHmdlMapping(g_renderIndexStorageA, handle);
    const HmdlEntryLookup &lookup = *static_cast<const HmdlEntryLookup *>(g_renderLookupStorage);
    const std::uint32_t hash = NormalizeHmdlHandle(handle);
    std::uint32_t slot = hash & 0x3ffU;
    while (lookup.hashes[slot] != hash) {
        const std::uint16_t distance = lookup.probes[slot];
        if (distance == 0)
            return nullptr;
        slot = (slot + distance) & 0x3ffU;
    }
    return reinterpret_cast<std::byte *>(CheckRenderArrayEntryGeneration(lookup.values[slot]));
}

std::byte *ResolveHmdlRuntimeObject(std::uintptr_t handle) {
    if (!g_renderObjectStorage || handle == 0)
        return nullptr;
    const std::uintptr_t mapped = LookupLargeHmdlMapping(g_renderIndexStorageB, handle);
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    const std::uint32_t hash = NormalizeHmdlHandle(mapped);
    if (hash != 0) {
        std::uint32_t slot = hash & 0x3ffU;
        while (objects.hashes[slot] != hash) {
            const std::uint16_t distance = objects.probes[slot];
            if (distance == 0)
                break;
            slot = (slot + distance) & 0x3ffU;
        }
        if (objects.hashes[slot] == hash)
            return reinterpret_cast<std::byte *>(&objects.objects[slot]);
    }
    RenderArrayEntry *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(mapped));
    return entry ? reinterpret_cast<std::byte *>(entry->owner) : nullptr;
}


bool ReleaseHmdlRuntimeObject(std::uintptr_t handle) {
    RenderArrayOwner *const object = reinterpret_cast<RenderArrayOwner *>(ResolveHmdlRuntimeObject(handle));
    if (!object)
        return false;
    if (object->referenceCount)
        --object->referenceCount;
    if (object->referenceCount)
        return RemoveLargeHmdlMapping(g_renderIndexStorageB, NormalizeHmdlHandle(handle));
    object->retire = 1;
    for (std::uint32_t index = 0; index < object->childCount; ++index) {
        if (RenderArrayEntry *const child = object->children[index])
            child->retire = 2;
    }
    g_hmdlEntryDirty = 1;
    return true;
}



bool SetupEngineObject(std::uintptr_t modelFileData, std::uintptr_t modelData, std::uint8_t mode) {
    RenderArrayOwner *const object = reinterpret_cast<RenderArrayOwner *>(ResolveHmdlRuntimeObject(modelFileData));
    if (!object)
        return false;
    if (RenderArrayEntry *const existing = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(modelData))) {
        if (!existing->retire)
            return false;
        if (existing->owner == object) {
            existing->retire = 0;
            return true;
        }
        RetireHmdlRenderEntry(*existing);
    }
    if (object->retire)
        return false;
    RenderArrayEntry *entry = nullptr;
    for (std::uint32_t index = 0; index < object->childCount; ++index) {
        RenderArrayEntry *const child = object->children[index];
        if (child && !child->modelData) {
            entry = child;
            break;
        }
    }
    if (!entry) {
        if (static_cast<const HmdlEntryLookup *>(g_renderLookupStorage)->count >= 1024)
            return false;
        HmdlEntryStorage &storage = *static_cast<HmdlEntryStorage *>(g_renderPacketStorage);
        std::uint32_t slot = storage.cursor;
        std::uint32_t counter = 0;
        while (true) {
            counter = storage.generations[slot] >> 10U;
            if (counter < 0x200000U)
                break;
            slot = (slot + 1) & 0x3ffU;
            if (slot == storage.cursor)
                return false;
        }
        const std::uint32_t next = counter + 1;
        const std::uint32_t generation = static_cast<std::uint32_t>((((next & 0xffe00000U) != 0x200000U ? next : 1U) | 0xffe00000U) << 10U);
        storage.generations[slot] = generation;
        storage.cursor = (slot + 1) & 0x3ffU;
        const std::uint32_t id = slot | generation;
        if (!id)
            return false;
        entry = CheckRenderArrayEntryGeneration(id);
        std::memset(entry, 0, sizeof(*entry));
        entry->id = id;
        if (object->childCount >= object->childCapacity) {
            const std::uint32_t doubled = object->childCapacity * 2U;
            const std::uint32_t capacity = doubled >= 16U ? doubled : 16U;
            if (capacity > object->childCapacity) {
                RenderArrayEntry **const children = static_cast<RenderArrayEntry **>(std::realloc(object->children, sizeof(RenderArrayEntry *) * capacity));
                if (!children)
                    return false;
                object->children = children;
                object->childCapacity = capacity;
            }
        }
        object->children[object->childCount++] = entry;
        entry->owner = object;
    }
    entry->flags = mode;
    RebindHmdlRenderEntry(modelData, *entry);
    g_hmdlEntryDirty = 1;
    return true;
}


void *InitChara(void *modelFileData, void *sourceModel) {
    if (!sourceModel)
        return nullptr;
    void *const model = HMDL_CloneNodeTree(sourceModel);
    if (!model)
        return nullptr;
    SetupEngineObject(reinterpret_cast<std::uintptr_t>(modelFileData), reinterpret_cast<std::uintptr_t>(model), 0);
    return model;
}



std::uint8_t ProcessHmdlResourceTransitions(std::int32_t setupShaders) {
    if (!g_hmdlEntryDirty || !g_renderPacketStorage || !g_renderObjectStorage)
        return g_hmdlEntryDirty;
    bool settled = true;
    HmdlEntryStorage &storage = *static_cast<HmdlEntryStorage *>(g_renderPacketStorage);
    for (std::uint32_t slot = 0; slot < 1024; ++slot) {
        if (storage.generations[slot] < 0x80000000U)
            continue;
        RenderArrayEntry &entry = storage.entries[slot];
        std::uint32_t state = entry.state;
        if (!state) {
            RenderArrayOwner *const object = entry.owner;
            if (!object || object->state == 2)
                entry.state = 2;
            else if (object->state == 1)
                RealizeHmdlRenderEntry(entry);
            state = entry.state;
            settled &= state != 0;
        }
        if (entry.retire) {
            if (state)
                RetireHmdlRenderEntry(entry);
            settled &= entry.retire == 0;
        }
    }
    HmdlObjectStorage &objects = *static_cast<HmdlObjectStorage *>(g_renderObjectStorage);
    for (std::uint32_t slot = 0; slot < 1024; ++slot) {
        if (!objects.hashes[slot])
            continue;
        RenderArrayOwner &object = objects.objects[slot];
        std::uint32_t state = object.state;
        if (!state) {
            ResolveHmdlRuntimeAsset(object, setupShaders != 0);
            state = object.state;
            settled &= state != 0;
        }
        if (object.retire) {
            if (state)
                RetireHmdlRuntimeObject(object);
            settled &= object.retire == 0;
        }
    }
    if (settled)
        g_hmdlEntryDirty = 0;
    return g_hmdlEntryDirty;
}

} // namespace shenmue::s1
