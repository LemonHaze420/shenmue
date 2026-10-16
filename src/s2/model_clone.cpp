#include "s2/model_clone.hpp"

#include "d3t/object_store.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"
#include "s2/legacy_draw.hpp"

#include <bit>
#include <cstring>
#include <new>

namespace shenmue::s2 {

std::uint32_t g_modelCloneNodeCount{};
std::uint32_t g_modelCloneMeshCount{};
std::uint32_t g_modelCloneNonXb01Count{};
std::uint32_t g_modelCloneMdcBytes{};
std::uint32_t g_modelCloneMdpBytes{};

// @verified - p
std::uint32_t GetModelMdcCloneBlockSize(const ModelCloneNode *node) {
    const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(node);
    const std::byte *const bytes = reinterpret_cast<const std::byte *>(node);
    const std::uint16_t kind = static_cast<std::uint16_t>(node->id);
    if (static_cast<std::uint16_t>(kind - 68U) <= 1U) {
        const std::uint32_t prefixSize = words[17];
        const std::uint32_t *const records = reinterpret_cast<const std::uint32_t *>(bytes + prefixSize + 72);
        const std::uint32_t recordBytes = 48U * records[1];
        const std::uint32_t *const tail = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(records) + recordBytes);
        if (words[16] == MODEL_MDHX || words[16] == MODEL_MDH7) {
            const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(tail) + tail[3] + 36);
            return tail[3] + prefixSize + recordBytes + 8U * count + 48U;
        }
        if (words[16] != MODEL_MDCX && words[16] != MODEL_MDC7)
            return 0;
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(tail) + tail[3] + 24);
        return prefixSize + recordBytes + 8U * count + tail[3] + 36U;
    }
    if (kind != 5 && kind != 10) {
        const std::uint32_t count = words[33];
        const std::uint32_t prefixWords = count ? 2U * count + 1U : 2U;
        const std::uint32_t records = words[count ? 34U + 2U * count : 35U];
        return 4U * (prefixWords + 8U * records + 1U) + 68U;
    }
    if (words[16] != MODEL_MDCX && words[16] != MODEL_MDC7)
        return 0;
    const std::uint32_t count = static_cast<std::uint16_t>(words[33]);
    const std::uint32_t prefixWords = count ? 2U * count + 1U : 2U;
    const std::uint32_t *const tail = words + (count ? 33U + 2U * count : 34U);
    if (tail[1] != MODEL_WBTR)
        return 4U * (prefixWords + 8U * tail[1] + 1U) + 68U;
    const std::uint32_t bindWords = 17U * tail[2];
    const std::uint32_t *const weights = tail + 4;
    const std::uint32_t vertices = weights[bindWords];
    const std::uint32_t mappingBytes = weights[bindWords + 2U + 12U * vertices];
    const std::uint32_t mappingCount = weights[bindWords + 5U + 12U * vertices + (mappingBytes >> 2U)];
    return mappingBytes + 4U * (prefixWords + bindWords + 2U * (mappingCount + 6U * vertices + 13U));
}

// @verified - p
std::int64_t CountModelNodeTreeForClone(const ModelCloneNode *node) {
    std::int32_t siblingOffset{};
    do {
        ++g_modelCloneNodeCount;
        if (node->meshOffset != 0) {
            const std::uint32_t *const mesh = static_cast<const std::uint32_t *>(HLib::GetPointerFromDynamicHeapOffsetBase(node->meshOffset));
            if (mesh) {
                ++g_modelCloneMeshCount;
                if (*mesh != MODEL_XB01)
                    ++g_modelCloneNonXb01Count;
            }
        }
        if (node->childOffset != 0) {
            const ModelCloneNode *const child = static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(node->childOffset));
            if (child)
                CountModelNodeTreeForClone(child);
        }
        siblingOffset = node->siblingOffset;
        node = siblingOffset ? static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(siblingOffset)) : nullptr;
    } while (node);
    return siblingOffset;
}

// @verified - p
std::int64_t AccumulateModelMdcCloneBytes(const ModelCloneNode *node) {
    std::int32_t siblingOffset{};
    do {
        const std::uint32_t tag = reinterpret_cast<const std::uint32_t *>(node)[16];
        if (tag == MODEL_MDC7 || tag == MODEL_MDH7 || tag == MODEL_MDCX || tag == MODEL_MDHX)
            g_modelCloneMdcBytes += GetModelMdcCloneBlockSize(node);
        if (node->childOffset != 0) {
            const ModelCloneNode *const child = static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(node->childOffset));
            if (child)
                AccumulateModelMdcCloneBytes(child);
        }
        siblingOffset = node->siblingOffset;
        node = siblingOffset ? static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(siblingOffset)) : nullptr;
    } while (node);
    return siblingOffset;
}

// @verified - p
std::int64_t AccumulateModelMdpCloneBytes(const ModelCloneNode *node) {
    std::int32_t siblingOffset{};
    do {
        const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(node);
        if (words[16] == MODEL_MDP7 || words[16] == MODEL_MDPX)
            g_modelCloneMdpBytes += words[17];
        if (node->childOffset != 0) {
            const ModelCloneNode *const child = static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(node->childOffset));
            if (child)
                AccumulateModelMdpCloneBytes(child);
        }
        siblingOffset = node->siblingOffset;
        node = siblingOffset ? static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(siblingOffset)) : nullptr;
    } while (node);
    return siblingOffset;
}

// @verified - p
std::uint64_t MeasureModelNodeTreeClone(const ModelCloneNode *node) {
    g_modelCloneNodeCount = 0;
    if (node)
        CountModelNodeTreeForClone(node);
    const std::uint64_t nodeBytes = static_cast<std::uint64_t>(std::bit_cast<std::int32_t>(g_modelCloneNodeCount)) << 6U;
    const std::uint32_t tag = reinterpret_cast<const std::uint32_t *>(node)[16];
    if (tag == MODEL_MDC7 || tag == MODEL_MDH7 || tag == MODEL_MDCX || tag == MODEL_MDHX) {
        g_modelCloneMdcBytes = 0;
        if (node)
            AccumulateModelMdcCloneBytes(node);
        return nodeBytes + static_cast<std::uint64_t>(std::bit_cast<std::int32_t>(g_modelCloneMdcBytes));
    }
    if (tag == MODEL_MDP7 || tag == MODEL_MDPX) {
        g_modelCloneMdpBytes = 0;
        if (node)
            AccumulateModelMdpCloneBytes(node);
        g_modelCloneMdpBytes += 4;
        return nodeBytes + static_cast<std::uint64_t>(std::bit_cast<std::int32_t>(g_modelCloneMdpBytes));
    }
    return nodeBytes;
}

// @verified - p
std::byte *CopyModelNodeTree(ModelCloneNode *parent, const ModelCloneNode *source, std::byte *destination) {
    do {
        ModelCloneNode *const copy = reinterpret_cast<ModelCloneNode *>(destination);
        std::memcpy(copy, source, sizeof(ModelCloneNode));
        destination += sizeof(ModelCloneNode);
        const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(source);
        const std::uint32_t tag = words[16];
        std::uint64_t meshBytes = 0;
        if (tag == MODEL_MDC7 || tag == MODEL_MDH7 || tag == MODEL_MDCX || tag == MODEL_MDHX)
            meshBytes = static_cast<std::uint64_t>(std::bit_cast<std::int32_t>(GetModelMdcCloneBlockSize(source)));
        else if (tag == MODEL_MDP7 || tag == MODEL_MDPX)
            meshBytes = static_cast<std::uint64_t>(std::bit_cast<std::int32_t>(words[17]));
        if (meshBytes) {
            std::memmove(destination, words + 16, static_cast<std::size_t>(meshBytes));
            destination += meshBytes;
        }
        copy->parentOffset = parent ? HLib::GetDynamicHeapOffset(parent) : 0;
        if (source->childOffset != 0) {
            const ModelCloneNode *const child = static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(source->childOffset));
            if (child) {
                copy->childOffset = HLib::GetDynamicHeapOffset(destination);
                destination = CopyModelNodeTree(copy, child, destination);
            }
        }
        const ModelCloneNode *const sibling =
            source->siblingOffset ? static_cast<const ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(source->siblingOffset)) : nullptr;
        if (!sibling)
            return destination;
        copy->siblingOffset = HLib::GetDynamicHeapOffset(destination);
        source = sibling;
    } while (true);
}

// @verified - p
ModelCloneNode *CloneModelNodeTree(const void *sourceKey, const ModelCloneNode *node, std::uint8_t flags) {
    if (!node)
        return nullptr;
    const std::uint64_t size = MeasureModelNodeTreeClone(node);
    if (!size)
        return nullptr;
    ModelCloneNode *const copy = static_cast<ModelCloneNode *>(HLib::AllocateTaskData(size, MEMORY_MODEL_CLONE));
    if (copy) {
        std::byte *const end = CopyModelNodeTree(nullptr, node, reinterpret_cast<std::byte *>(copy));
        const std::uint32_t tag = reinterpret_cast<const std::uint32_t *>(node)[16];
        if (tag == MODEL_MDP7 || tag == MODEL_MDPX)
            *reinterpret_cast<std::uint32_t *>(end) = 0;
        if (sourceKey)
            (void)d3t::RegisterObjectPlacement(sourceKey, copy, flags);
    }
    return copy;
}

MT7Model *CloneMT7ModelNodeTree(const void *sourceKey, const MT7Model &source, std::uint8_t flags) {
    if (source.nodes.empty())
        return nullptr;
    void *const allocation = HLib::AllocateTaskData(sizeof(MT7Model), MEMORY_MODEL_CLONE);
    if (!allocation)
        return nullptr;
    MT7Model *const copy = new (allocation) MT7Model{};
    try {
        copy->signature = source.signature;
        copy->entityType = source.entityType;
        copy->source = source.source;
        copy->nodes.reserve(source.nodes.size());
        for (const MT7Node &node : source.nodes) {
            copy->nodes.push_back(MT7Node{.sourceOffset = node.sourceOffset,
                .id = node.id,
                .parent = node.parent,
                .position = node.position,
                .rotation = node.rotation,
                .scale = node.scale,
                .meshVertexOffset = node.meshVertexOffset,
                .meshVertexCount = node.meshVertexCount,
                .meshVertexStride = node.meshVertexStride,
                .xb01Offset = node.xb01Offset,
                .commandStream = node.commandStream,
                .drawResources = {},
                .visible = node.visible,
                .transformOverride = node.transformOverride,
                .meshType = node.meshType,
                .taskData = node.taskData,
                .firstChild = node.firstChild,
                .nextSibling = node.nextSibling,
                .borrowedMesh = node.borrowedMesh ? node.borrowedMesh : &node,
                .facialData = node.facialData,
                .runtimeMesh = node.runtimeMesh,
                .modelMarker = node.modelMarker,
                .mdcVertexLinkFlags = node.mdcVertexLinkFlags,
                .mdcWeightMatrix = node.mdcWeightMatrix,
                .mdcWeightedVertices = node.mdcWeightedVertices});
        }
        InitializeMT7RuntimeLinks(*copy);
        copy->wbtrDeformers = source.wbtrDeformers;
        copy->mdcVertexLinks = source.mdcVertexLinks;
        copy->mdpVertexLinks = source.mdpVertexLinks;
        if (sourceKey)
            (void)d3t::RegisterObjectPlacement(sourceKey, GetMT7ReaderData(copy), flags);
    } catch (...) {
        copy->~MT7Model();
        HLib::FreeTaskData(allocation);
        throw;
    }
    return copy;
}
namespace {
std::int32_t CopyBorrowedModelBranch(
    MT7Model &destination, const MT7Model &source, std::uint32_t index, std::int32_t parent, std::vector<std::int32_t> &indices) {
    const MT7Node &node = source.nodes[index];
    const std::int32_t copied = static_cast<std::int32_t>(destination.nodes.size());
    indices[index] = copied;
    destination.nodes.push_back(MT7Node{.sourceOffset = node.sourceOffset,
        .id = node.id,
        .parent = parent,
        .position = node.position,
        .rotation = node.rotation,
        .scale = node.scale,
        .meshVertexOffset = node.meshVertexOffset,
        .meshVertexCount = node.meshVertexCount,
        .meshVertexStride = node.meshVertexStride,
        .xb01Offset = node.xb01Offset,
        .commandStream = node.commandStream,
        .drawResources = {},
        .visible = node.visible,
        .transformOverride = node.transformOverride,
        .meshType = node.meshType,
        .taskData = node.taskData,
        .firstChild = -1,
        .nextSibling = -1,
        .borrowedMesh = node.borrowedMesh ? node.borrowedMesh : &node,
        .facialData = node.facialData,
        .runtimeMesh = node.runtimeMesh,
        .modelMarker = node.modelMarker,
        .mdcVertexLinkFlags = node.mdcVertexLinkFlags,
        .mdcWeightMatrix = node.mdcWeightMatrix,
        .mdcWeightedVertices = node.mdcWeightedVertices});
    std::int32_t previous = -1;
    for (std::int32_t child = node.firstChild; child >= 0; child = source.nodes[static_cast<std::size_t>(child)].nextSibling) {
        const std::int32_t next = CopyBorrowedModelBranch(destination, source, static_cast<std::uint32_t>(child), copied, indices);
        if (previous == -1)
            destination.nodes[static_cast<std::size_t>(copied)].firstChild = next;
        else
            destination.nodes[static_cast<std::size_t>(previous)].nextSibling = next;
        previous = next;
    }
    return copied;
}
}

MT7Model *CloneMT7ModelBranch(const void *sourceKey, const MT7Model &source, std::uint32_t rootIndex, std::uint8_t flags) {
    if (rootIndex >= source.nodes.size())
        return nullptr;
    void *const allocation = HLib::AllocateTaskData(sizeof(MT7Model), MEMORY_MODEL_CLONE);
    if (!allocation)
        return nullptr;
    MT7Model *const copy = new (allocation) MT7Model{};
    try {
        copy->signature = source.signature;
        copy->entityType = source.entityType;
        copy->source = source.source;
        copy->nodes.reserve(source.nodes.size());
        std::vector<std::int32_t> indices(source.nodes.size(), -1);
        CopyBorrowedModelBranch(*copy, source, rootIndex, -1, indices);
        for (const MT7WbtrDeformer &deformer : source.wbtrDeformers) {
            if (indices[deformer.nodeIndex] == -1)
                continue;
            copy->wbtrDeformers.push_back(deformer);
            copy->wbtrDeformers.back().nodeIndex = static_cast<std::uint32_t>(indices[deformer.nodeIndex]);
        }
        for (const MT7MdcVertexLink &link : source.mdcVertexLinks)
            if (indices[link.nodeIndex] != -1)
                copy->mdcVertexLinks.push_back({.nodeIndex = static_cast<std::uint32_t>(indices[link.nodeIndex]),
                    .sourceByteOffset = link.sourceByteOffset,
                    .destinationByteOffset = link.destinationByteOffset});
        for (const MT7MdcVertexLink &link : source.mdpVertexLinks)
            if (indices[link.nodeIndex] != -1)
                copy->mdpVertexLinks.push_back({.nodeIndex = static_cast<std::uint32_t>(indices[link.nodeIndex]),
                    .sourceByteOffset = link.sourceByteOffset,
                    .destinationByteOffset = link.destinationByteOffset});
        InitializeMT7RuntimeLinks(*copy);
        if (sourceKey)
            (void)d3t::RegisterObjectPlacement(sourceKey, GetMT7ReaderData(copy), flags);
    } catch (...) {
        copy->~MT7Model();
        HLib::FreeTaskData(allocation);
        throw;
    }
    return copy;
}

std::vector<std::vector<MT7Node>> *g_componentTeardownNodes{};

void ReleaseMT7ModelNodeTreeClone(MT7Model *model) {
    if (!model)
        return;
    FlushQueuedLegacyDrawPasses();
    d3t::RetireObjectPlacement(GetMT7ReaderData(model));
    while (d3t::ProcessObjectStore()) {
    }
    if (g_componentTeardownNodes)
        g_componentTeardownNodes->push_back(std::move(model->nodes));
    model->~MT7Model();
    HLib::FreeTaskData(model);
}

} // namespace shenmue::s2
