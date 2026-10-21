#include "s2/mt7.hpp"

#include <shendk/files/model/mt7_reader.h>
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
#include "s2/enhancements/map_seams.hpp"
#endif
#include "d3t/buffer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/dx11_renderer.hpp"

#include "s2/pak.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/scene_data.hpp"
#include "s2/hlib.hpp"
#include "s2/math.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <unordered_set>

namespace shenmue::s2 {
namespace {

#if 0
constexpr std::array<std::uint32_t, 8> modelSignatures{0x5843444dU, 0x3743444dU, 0x5850444dU, 0x3750444dU, 0x584f444dU, 0x374f444dU, 0x584c444dU, 0x374c444dU};
#endif
constexpr std::uint32_t xb01Signature = 0x31304258U;

template <typename Value> bool Read(std::span<const std::byte> bytes, std::size_t offset, Value &value) {
    if (offset > bytes.size() || sizeof(Value) > bytes.size() - offset) {
        return false;
    }
    std::memcpy(&value, bytes.data() + offset, sizeof(Value));
    return true;
}

bool Contains(std::span<const std::byte> bytes, std::size_t offset, std::size_t size) {
    return offset <= bytes.size() && size <= bytes.size() - offset;
}

struct MeshRange {
    std::uint32_t vertexOffset{};
    std::uint32_t vertexCount{};
    std::int32_t parentNode{-1};
};

class MT7Reader {
  public:
    MT7Reader(std::span<const std::byte> source, const char *label, MT7Model &destination) : bytes(source), container(source), model(destination) {
        model = {};
        model.label = label ? label : "MT7";
        model.source = source;
    }

    bool Run() {
        std::size_t offset = 0;
        for (;;) {
            std::array<std::uint32_t, 2> chunk{};
            if (!Read(container, offset, chunk))
                return false;
#if 0
            if (std::find(modelSignatures.begin(), modelSignatures.end(), chunk[0]) != modelSignatures.end())
#else
            if (shendk::IsMt7Signature(chunk[0]))
#endif
            break;
            if (chunk[0] != SHEN_NAME(FACE) && chunk[0] != SHEN_NAME(FACX) && chunk[0] != SHEN_NAME(LIGT) && chunk[0] != SHEN_NAME(CLSG) &&
                chunk[0] != SHEN_NAME(CLSX))
                return false;
            if (chunk[1] < 8 || chunk[1] > container.size() - offset)
                return false;
            if ((chunk[0] == SHEN_NAME(CLSG) || chunk[0] == SHEN_NAME(CLSX)) && !clothChunk) {
                clothChunk = container.data() + offset;
            }
            offset += chunk[1];
        }
        bytes = container.subspan(offset);
        model.source = bytes;
#if 0
        MT7Header header{};
        if (!Read(bytes, 0, header) || std::find(modelSignatures.begin(), modelSignatures.end(), header.signature) == modelSignatures.end()) {
            return false;
        }
#else
        shendk::Mt7Header header{};
        if (!shendk::ReadMt7Header(bytes, header)) {
            return false;
        }
#endif
        model.signature = header.signature;
        if (header.size >= sizeof(MT7Header) && header.size <= container.size() - offset)
            ReadTxt7(offset + header.size);
        if (!ReadTextures(header.textureCount)) {
            return false;
        }
        if (!header.firstNodeOffset || !ReadNode(header.firstNodeOffset, -1, 0)) {
            return false;
        }
        if (model.nodes.empty())
            return false;
        if (clothChunk)
            model.nodes.front().taskData = const_cast<std::byte *>(clothChunk);
        const std::uint32_t rootId = static_cast<std::uint32_t>(model.nodes.front().id) & 0xffffU;
        if (rootId >= 0x7001U && rootId <= 0x7061U) {
            model.entityType = static_cast<std::int32_t>(rootId - 0x7001U);
        }
        std::uint32_t vertexMemorySize = 0;
        if (!ParseNode(vertexMemorySize))
            return false;
        vertexMemorySize += 32;
        model.vertexMemoryOwner = bytes.data() + model.nodes.front().sourceOffset;
        void *vertexMemory = nullptr;
        (void)AllocateModelVertexMemory(std::bit_cast<std::int32_t>(vertexMemorySize), model.vertexMemoryOwner, &vertexMemory);
        for (std::uint32_t nodeIndex = 0; nodeIndex < model.nodes.size(); ++nodeIndex) {
            MT7Node &node = model.nodes[nodeIndex];
            if (node.xb01Offset) {
                for (std::uint32_t previous = 0; previous < nodeIndex; ++previous) {
                    if (model.nodes[previous].xb01Offset == node.xb01Offset) {
                        node.borrowedMesh = &model.nodes[previous];
                        break;
                    }
                }
            }
            if (!ReadModelVertexLinks(nodeIndex) || !ReadWbtr(nodeIndex) ||
                (node.xb01Offset && !node.borrowedMesh && !PrepareLegacyDrawResources(bytes, node.xb01Offset, node.drawResources))) {
                FreeModelVertexMemory(model.vertexMemoryOwner);
                model.vertexMemoryOwner = nullptr;
                for (MT7Node &prepared : model.nodes) {
                    ReleaseLegacyDrawResources(prepared.drawResources);
                }
                return false;
            }
            for (LegacyTextureBinding &binding : node.drawResources.textureBindings) {
                if (binding.textureTableIndex >= 0 && static_cast<std::size_t>(binding.textureTableIndex) < model.textures.size()) {
                    binding.textureHandle = model.textures[static_cast<std::size_t>(binding.textureTableIndex)].textureIndex;
                }
            }
        }
        return ReadFacialModelResource(model, container);
    }

  private:



    void ReadTxt7(std::size_t offset) {
        for (;;) {
            std::array<std::uint32_t, 2> chunk{};
            if (!Read(container, offset, chunk))
                return;
            if (chunk[0] == SHEN_NAME(TXT7))
                break;
            if (chunk[0] != SHEN_NAME(FACE) && chunk[0] != SHEN_NAME(FACX) && chunk[0] != SHEN_NAME(LIGT) && chunk[0] != SHEN_NAME(CLSG) &&
                chunk[0] != SHEN_NAME(CLSX))
                return;
            if (chunk[1] < 8 || chunk[1] > container.size() - offset)
                return;
            if ((chunk[0] == SHEN_NAME(CLSG) || chunk[0] == SHEN_NAME(CLSX)) && !clothChunk) {
                clothChunk = container.data() + offset;
            }
            offset += chunk[1];
        }
        std::uint32_t count{};
        if (!Read(container, offset + 8, count) || count > (container.size() - offset) / 12U)
            return;
        const std::size_t idTable = offset + 12 + static_cast<std::size_t>(count) * 4;
        for (std::uint32_t index = 0; index < count; ++index) {
            std::uint32_t entryOffset{};
            std::array<std::uint32_t, 2> id{};
            if (!Read(container, offset + 12 + static_cast<std::size_t>(index) * 4, entryOffset) ||
                !Read(container, idTable + static_cast<std::size_t>(index) * 8, id) || entryOffset > container.size() - offset) {
                return;
            }
            const std::size_t entry = offset + entryOffset;
            std::uint32_t signature{}, gbixSize{}, pvrSize{};
            std::size_t image = entry;
            if (Read(container, entry, signature) && signature == SHEN_NAME(GBIX) && Read(container, entry + 4, gbixSize))
                image += 8ULL + gbixSize;

            if (!Read(container, image + 4, pvrSize) || image + 16ULL + pvrSize > container.size())
                return;
            if (AcquireModelTexture(id[0], id[1], container.subspan(entry, image + 16 + pvrSize - entry)))
                model.embeddedTextureIds.push_back(id);
        }
        const std::uintptr_t source = reinterpret_cast<std::uintptr_t>(container.data());
        const std::uintptr_t heap = reinterpret_cast<std::uintptr_t>(HLib::DynamicHeapBase);

        if (heap && source >= heap + sizeof(HLib::TaskData) && source - heap < HLib::DYNAMIC_HEAP_SIZE && HLib::GetTaskDataName(container.data()) &&
            HLib::ShrinkTaskData(const_cast<std::byte *>(container.data()), offset)) {
            const std::size_t modelOffset = static_cast<std::size_t>(bytes.data() - container.data());
            container = container.first(offset);
            bytes = container.subspan(modelOffset);
            model.source = bytes;
        }
    }

    bool ParseNode(std::uint32_t &size) const {
        for (const MT7Node &node : model.nodes) {
            if (!node.xb01Offset)
                continue;
            std::uint32_t commandSizeWords{};
            if (!Read(bytes, node.xb01Offset + 28, commandSizeWords))
                return false;
            const std::size_t resourceCommand = node.xb01Offset + 28ULL + 4ULL * commandSizeWords;
            std::uint32_t count{};
            if (!Read(bytes, resourceCommand + 4, count))
                return false;
            std::size_t record = resourceCommand + 8;
            for (std::uint32_t index = 0; index < count; ++index) {
                std::uint32_t byteSize{};
                if (!Read(bytes, record + 4, byteSize) || !Contains(bytes, record + 8, byteSize))
                    return false;
                size += byteSize;
                record += 8ULL + byteSize;
            }
        }
        return true;
    }

    bool ReadTextures(std::uint32_t count) {
        const std::uint64_t tableSize = static_cast<std::uint64_t>(count) * 16;
        const std::uint64_t idsSize = static_cast<std::uint64_t>(count) * 8;
        if (tableSize + idsSize > bytes.size() - 16) {
            return false;
        }
        model.textures.resize(count);
        for (std::uint32_t index = 0; index < count; ++index) {
            MT7TextureEntry &entry = model.textures[index];
#if 0
            const std::size_t row = 16 + static_cast<std::size_t>(index) * 16;
            if (!Read(bytes, row, entry.width) || !Read(bytes, row + 2, entry.height) || !Read(bytes, row + 4, entry.unknown04) ||
                !Read(bytes, row + 8, entry.unknown08) || !Read(bytes, row + 12, entry.textureIndex)) {
                return false;
            }
            const std::size_t idOffset = 16 + static_cast<std::size_t>(count) * 16 + static_cast<std::size_t>(index) * 8;
            std::memcpy(entry.id, bytes.data() + idOffset, sizeof(entry.id));
#else
            const shendk::Mt7Header header{.textureCount = count};
            shendk::Mt7Texture source{};
            if (!shendk::ReadMt7Texture(bytes, header, index, source)) {
                return false;
            }
            entry = std::bit_cast<MT7TextureEntry>(source);
#endif
            std::uint32_t idLow{};
            std::uint32_t idHigh{};
            std::memcpy(&idLow, entry.id, sizeof(idLow));
            std::memcpy(&idHigh, entry.id + sizeof(idLow), sizeof(idHigh));
            entry.unknown08 = 0xffffffffU;
            entry.textureIndex = ResolveLoadedMapTexture(idLow, idHigh);
        }
        return true;
    }

    bool ReadNode(std::uint32_t offset, std::int32_t parent, std::size_t depth) {
        if (depth >= 512 || !visited.insert(offset).second || !Contains(bytes, offset, sizeof(MT7NodeSource))) {
            return false;
        }
#if 0
        MT7NodeSource source{};
        if (!Read(bytes, offset, source)) {
            return false;
        }
#else
        shendk::Mt7Node record{};
        if (!shendk::ReadMt7Node(bytes, offset, record)) {
            return false;
        }
        const MT7NodeSource source = std::bit_cast<MT7NodeSource>(record);
#endif
        const std::uint32_t nodeIndex = static_cast<std::uint32_t>(model.nodes.size());
        model.nodes.push_back({offset, source.id, parent, source.position, source.rotation, source.scale, 0, 0, 0, source.xb01Offset, {}, {}});
        meshRanges.push_back({0, 0, parent});
        std::uint32_t meshType{};
        if (Read(bytes, offset + 0x40, meshType))
            model.nodes.back().meshType = meshType;
        if (meshType == SHEN_NAME(MDP7) || meshType == SHEN_NAME(MDPX))
            Read(bytes, offset + 0x48, model.nodes.back().modelMarker);
        if (source.xb01Offset && !ReadMesh(source.xb01Offset, nodeIndex)) {
            return false;
        }
        if (source.childOffset)
            model.nodes[nodeIndex].firstChild = static_cast<std::int32_t>(model.nodes.size());
        if (source.childOffset && !ReadNode(source.childOffset, static_cast<std::int32_t>(nodeIndex), depth + 1)) {
            return false;
        }
        if (source.siblingOffset) {
            model.nodes[nodeIndex].nextSibling = static_cast<std::int32_t>(model.nodes.size());
            if (!ReadNode(source.siblingOffset, parent, depth)) {
                return false;
            }
        }
        return true;
    }

    bool ReadMesh(std::uint32_t offset, std::uint32_t nodeIndex) {
        std::uint32_t signature{};
        if (!Read(bytes, offset, signature) || signature != xb01Signature || !Contains(bytes, offset, 0x30)) {
            return false;
        }
        if (!InspectLegacyDrawCommandStream(bytes, offset, model.nodes[nodeIndex].commandStream)) {
            return false;
        }
#if 0
        std::uint32_t firstEntryOffset{};
        std::uint32_t commandSizeWords{};
        if (!Read(bytes, offset + 24, firstEntryOffset) || !Read(bytes, offset + 28, commandSizeWords)) {
            return false;
        }
        const std::uint64_t vertexHeaderOffset64 = static_cast<std::uint64_t>(offset) + 28 + static_cast<std::uint64_t>(commandSizeWords) * 4;
        if (vertexHeaderOffset64 > (std::numeric_limits<std::size_t>::max)()) {
            return false;
        }
        const std::size_t vertexHeaderOffset = static_cast<std::size_t>(vertexHeaderOffset64);
        std::uint16_t stride{};
        std::uint32_t verticesSize{};
        if (!Read(bytes, vertexHeaderOffset + 8, stride) || !Read(bytes, vertexHeaderOffset + 12, verticesSize) || stride < 12 || verticesSize % stride != 0 ||
            !Contains(bytes, vertexHeaderOffset + 16, verticesSize)) {
            return false;
        }
#else
        shendk::Mt7MeshView mesh{};
        std::uint32_t firstEntryOffset{};
        if (!shendk::ReadMt7Mesh(bytes, offset, mesh) || !Read(bytes, offset + 24, firstEntryOffset)) {
            return false;
        }
        const std::size_t vertexHeaderOffset = mesh.vertexOffset - 16U;
        const std::uint16_t stride = mesh.vertexStride;
        const std::uint32_t verticesSize = static_cast<std::uint32_t>(mesh.vertices.size());
#endif
        MT7Node &node = model.nodes[nodeIndex];
        node.meshVertexOffset = static_cast<std::uint32_t>(model.vertices.size());
        node.meshVertexCount = verticesSize / stride;
        node.meshVertexStride = stride;
        meshRanges[nodeIndex] = {node.meshVertexOffset, node.meshVertexCount, node.parent};
        for (std::uint32_t index = 0; index < node.meshVertexCount; ++index) {
            const std::size_t vertexOffset = vertexHeaderOffset + 16 + static_cast<std::size_t>(index) * stride;
            MT7Vertex vertex{};
            if (!Read(bytes, vertexOffset, vertex.position)) {
                return false;
            }
            vertex.normal = {0.0f, 1.0f, 0.0f};
            if (stride >= 24) {
                Read(bytes, vertexOffset + 12, vertex.normal);
                const float length = std::sqrt(vertex.normal.x * vertex.normal.x + vertex.normal.y * vertex.normal.y + vertex.normal.z * vertex.normal.z);
                if (length > 0.0f) {
                    vertex.normal.x /= length;
                    vertex.normal.y /= length;
                    vertex.normal.z /= length;
                }
            }
            vertex.uv = {};
            if (stride == 32) {
                Read(bytes, vertexOffset + 24, vertex.uv);
            } else if (stride >= 48) {
                Read(bytes, vertexOffset + 40, vertex.uv);
            }
            vertex.colour = {1.0f, 1.0f, 1.0f, 1.0f};
            if (stride == 40 || stride >= 48) {
                Read(bytes, vertexOffset + 24, vertex.colour);
            }
            vertex.ownerNode = nodeIndex;
            model.vertices.push_back(vertex);
        }
        return ReadCommands(offset, firstEntryOffset, vertexHeaderOffset, nodeIndex);
    }

    bool ReadWbtr(std::uint32_t nodeIndex) {
        const MT7Node &node = model.nodes[nodeIndex];
        constexpr std::uint32_t mdcx = 0x5843444dU;
        constexpr std::uint32_t wbtr = 0x52544257U;
        constexpr std::uint32_t wvtx = 0x58545657U;
        std::uint32_t signature{};
        std::uint32_t packedCount{};
        if (!Read(bytes, node.sourceOffset + 64, signature) || signature != mdcx || !Read(bytes, node.sourceOffset + 132, packedCount)) {
            return true;
        }
        const std::uint32_t mdcCount = packedCount & 0xffffU;
        const std::uint64_t wbtrOffset64 = static_cast<std::uint64_t>(node.sourceOffset) + (mdcCount ? 136U + static_cast<std::uint64_t>(mdcCount) * 8U : 140U);
        if (wbtrOffset64 > (std::numeric_limits<std::size_t>::max)()) {
            return false;
        }
        std::size_t cursor = static_cast<std::size_t>(wbtrOffset64);
        std::uint32_t tag{}, boneCount{};
        if (!Read(bytes, cursor, tag) || tag != wbtr)
            return true;
        if (!Read(bytes, cursor + 4, boneCount) || boneCount > 32 || !Contains(bytes, cursor + 8, static_cast<std::size_t>(boneCount) * 68U)) {
            return false;
        }
        MT7WbtrDeformer deformer{};
        deformer.nodeIndex = nodeIndex;
        cursor += 8;
        for (std::uint32_t index = 0; index < boneCount; ++index) {
            std::uint32_t controller{};
            Matrix4x4 matrix{};
            if (!Read(bytes, cursor, controller) || !Read(bytes, cursor + 4, matrix))
                return false;
            deformer.bindMatrices.emplace_back(controller, matrix);
            cursor += 68;
        }
        std::uint32_t vertexCount{};
        if (!Read(bytes, cursor, tag) || tag != wvtx || !Read(bytes, cursor + 4, vertexCount) || vertexCount > 0x10000U ||
            !Contains(bytes, cursor + 8, static_cast<std::size_t>(vertexCount) * 48U)) {
            return false;
        }
        cursor += 8;
        deformer.vertices.resize(vertexCount);
        deformer.evaluatedVertices.resize(vertexCount);
        for (MT7WeightedVertex &vertex : deformer.vertices) {
            if (!Read(bytes, cursor, vertex.position) || !Read(bytes, cursor + 12, vertex.normal))
                return false;
            cursor += 48;
        }
        std::uint32_t groupBytes{}, groupCount{};
        if (!Read(bytes, cursor + 4, groupBytes) || !Read(bytes, cursor + 8, groupCount) || groupCount > 0x10000U || !Contains(bytes, cursor + 12, groupBytes))
            return false;
        const std::size_t groupStart = cursor + 12;
        const std::size_t groupEnd = groupStart + groupBytes;
        cursor = groupStart;
        for (std::uint32_t group = 0; group < groupCount; ++group) {
            std::uint32_t controller{}, count{};
            if (!Read(bytes, cursor, controller) || !Read(bytes, cursor + 4, count) || count > 0x10000U ||
                !Contains(bytes, cursor + 8, static_cast<std::size_t>(count) * 8U) || cursor + 8 + static_cast<std::size_t>(count) * 8U > groupEnd) {
                return false;
            }
            cursor += 8;
            for (std::uint32_t influence = 0; influence < count; ++influence) {
                std::uint32_t vertexIndex{};
                float weight{};
                if (!Read(bytes, cursor, vertexIndex) || !Read(bytes, cursor + 4, weight))
                    return false;
                if (vertexIndex < deformer.vertices.size()) {
                    deformer.vertices[vertexIndex].influences.push_back({controller, weight});
                }
                cursor += 8;
            }
        }
        cursor = groupEnd;
        std::uint32_t mappingCount{};
        if (!Read(bytes, cursor + 4, mappingCount) || mappingCount > 0x10000U || !Contains(bytes, cursor + 8, static_cast<std::size_t>(mappingCount) * 8U)) {
            return false;
        }
        cursor += 8;
        for (std::uint32_t mapping = 0; mapping < mappingCount; ++mapping) {
            MT7VertexMapping row{};
            if (!Read(bytes, cursor, row.sourceVertex) || !Read(bytes, cursor + 4, row.destinationByteOffset)) {
                return false;
            }
            if (row.sourceVertex < deformer.vertices.size() && node.meshVertexStride && row.destinationByteOffset % node.meshVertexStride == 0 &&
                row.destinationByteOffset / node.meshVertexStride < node.meshVertexCount) {
                deformer.mappings.push_back(row);
            }
            cursor += 8;
        }
        model.wbtrDeformers.push_back(std::move(deformer));
        return true;
    }

    bool ReadModelVertexLinks(std::uint32_t nodeIndex) {
        MT7Node &node = model.nodes[nodeIndex];
        constexpr std::uint32_t mdcx = SHEN_NAME(MDCX);
        constexpr std::uint32_t mdc7 = SHEN_NAME(MDC7);
        constexpr std::uint32_t mdpx = SHEN_NAME(MDPX);
        constexpr std::uint32_t mdp7 = SHEN_NAME(MDP7);
        std::uint32_t signature{}, packedCount{};
        if (!Read(bytes, node.sourceOffset + 64, signature))
            return true;
        const bool mdp = signature == mdpx || signature == mdp7;
        if (!mdp && signature != mdcx && signature != mdc7)
            return true;
        std::size_t cursor{};
        if (mdp) {
            std::uint32_t size{}, repeated{};
            if (!Read(bytes, node.sourceOffset + 68, size) || size <= 0x14 || !Read(bytes, node.sourceOffset + 84, repeated) || repeated != signature)
                return true;
            if (!Read(bytes, node.sourceOffset + 88, packedCount))
                return false;
            cursor = node.sourceOffset + 92;
        } else {
            if (!Read(bytes, node.sourceOffset + 132, packedCount))
                return false;
            node.mdcVertexLinkFlags = packedCount;
            const std::uint32_t id = static_cast<std::uint32_t>(node.id) & 0xffffU;
            if (id == 5 || id == 10)
                packedCount &= 0xffffU;
            cursor = node.sourceOffset + 136;
        }
        const std::uint32_t count = packedCount;
        if (count > 0x10000U || !Contains(bytes, cursor, static_cast<std::size_t>(count) * 8U))
            return false;
        for (std::uint32_t index = 0; index < count; ++index) {
            MT7MdcVertexLink link{nodeIndex, 0, 0};
            if (!Read(bytes, cursor, link.sourceByteOffset) || !Read(bytes, cursor + 4, link.destinationByteOffset)) {
                return false;
            }
            (mdp ? model.mdpVertexLinks : model.mdcVertexLinks).push_back(link);
            cursor += 8;
        }
        if (!mdp && count) {
            std::uint32_t weightedCount;
            if (!Read(bytes, cursor, weightedCount))
                return false;
            if (weightedCount != SHEN_NAME(WBTR)) {
                cursor += 4;
                if (!Contains(bytes, cursor, static_cast<std::size_t>(weightedCount) * sizeof(MT7MdcWeightedVertex)) ||
                    !Read(bytes, node.sourceOffset + 68, node.mdcWeightMatrix))
                    return false;
                node.mdcWeightedVertices = {reinterpret_cast<const MT7MdcWeightedVertex *>(bytes.data() + cursor), weightedCount};
            }
        }
        return true;
    }

    bool ReadCommands(std::uint32_t meshOffset, std::uint32_t firstEntryOffset, std::size_t vertexHeaderOffset, std::uint32_t nodeIndex) {
        if (vertexHeaderOffset < 8) {
            return false;
        }
        const std::size_t end = vertexHeaderOffset - 8;
        const std::uint64_t commandOffset64 = static_cast<std::uint64_t>(meshOffset) + static_cast<std::uint64_t>(firstEntryOffset + 6) * 4;
        if (commandOffset64 > end || end - commandOffset64 < 20) {
            return true;
        }
        std::size_t cursor = static_cast<std::size_t>(commandOffset64) + 20;
        std::size_t groupEnd = static_cast<std::size_t>(commandOffset64);
        std::uint16_t groupSize{};
        Read(bytes, static_cast<std::size_t>(commandOffset64) + 1, groupSize);
        groupEnd += static_cast<std::size_t>(groupSize) * 4U;
        std::uint32_t textureIndex = 0;
        std::uint16_t wrap = 0;
        bool transparent = false;
        bool unlit = false;

        for (std::size_t guard = 0; cursor + 4 <= end && guard < 100000; ++guard) {
            std::uint32_t command{};
            Read(bytes, cursor, command);
            if (!command) {
                cursor += 4;
                if (cursor + 20 <= end && cursor >= groupEnd) {
                    Read(bytes, cursor + 1, groupSize);
                    groupEnd = cursor + static_cast<std::size_t>(groupSize) * 4U;
                    cursor += 20;
                }
                continue;
            }
            const auto type = static_cast<std::uint8_t>(command);
            const auto sizeWords = static_cast<std::uint8_t>(command >> 8U);
            if (type == 0x0b) {
                const std::size_t count = sizeWords > 3 ? sizeWords - 3 : 0;
                if (count && Contains(bytes, cursor + 8, 4)) {
                    Read(bytes, cursor + 8, textureIndex);
                }
            } else if (type == 0x0d) {
                std::uint32_t count{};
                if (!Read(bytes, cursor + 4, count) || count > (end - std::min(end, cursor + 8)) / 4) {
                    return false;
                }
                for (std::uint32_t index = 0; index < count; ++index) {
                    std::uint16_t key{};
                    std::uint16_t value{};
                    Read(bytes, cursor + 8 + index * 4, key);
                    Read(bytes, cursor + 10 + index * 4, value);
                    if (key == 0x10) {
                        transparent = true;
                        if (value == 0x0400) {
                            unlit = true;
                        }
                    } else if (key == 0) {
                        wrap = value;
                    }
                }
            } else if (type == 0x10) {
                std::uint16_t fullSizeWords{};
                std::uint32_t primitive{};
                std::uint32_t indexCount{};
                if (!Read(bytes, cursor + 1, fullSizeWords) || !Read(bytes, cursor + 4, primitive) || !Read(bytes, cursor + 8, indexCount) ||
                    !Contains(bytes, cursor + 12, static_cast<std::size_t>(indexCount) * 2)) {
                    return false;
                }
                const std::uint32_t firstIndex = static_cast<std::uint32_t>(model.indices.size());
                if (!EmitIndices(cursor + 12, indexCount, primitive, nodeIndex)) {
                    return false;
                }
                model.surfaces.push_back(
                    {nodeIndex, textureIndex, firstIndex, static_cast<std::uint32_t>(model.indices.size()) - firstIndex, primitive, wrap, transparent, unlit});
                cursor += std::max<std::size_t>(1, fullSizeWords) * 4;
                continue;
            }
            cursor += std::max<std::size_t>(1, sizeWords & 0x0fU) * 4;
        }
        return true;
    }

    std::int64_t ResolveIndex(std::int32_t nodeIndex, std::int16_t index) const {
        if (nodeIndex < 0 || static_cast<std::size_t>(nodeIndex) >= meshRanges.size()) {
            return -1;
        }
        const MeshRange &mesh = meshRanges[static_cast<std::size_t>(nodeIndex)];
        if (index >= 0) {
            return static_cast<std::int64_t>(mesh.vertexOffset) + index;
        }
        if (mesh.parentNode < 0) {
            return mesh.vertexOffset;
        }
        const MeshRange &parent = meshRanges[static_cast<std::size_t>(mesh.parentNode)];
        const std::int64_t resolved = static_cast<std::int64_t>(parent.vertexOffset) + parent.vertexCount + index;
        if (resolved >= static_cast<std::int64_t>(parent.vertexOffset)) {
            return resolved;
        }
        const std::int64_t relative = resolved - static_cast<std::int64_t>(parent.vertexOffset);
        if (relative < (std::numeric_limits<std::int16_t>::min)()) {
            return -1;
        }
        return ResolveIndex(mesh.parentNode, static_cast<std::int16_t>(relative));
    }

    bool EmitIndices(std::size_t offset, std::uint32_t count, std::uint32_t primitive, std::uint32_t nodeIndex) {
        std::vector<std::int16_t> source(count);
        std::memcpy(source.data(), bytes.data() + offset, static_cast<std::size_t>(count) * sizeof(std::int16_t));
        const auto emit = [&](std::int16_t a, std::int16_t b, std::int16_t c) {
            const std::array<std::int64_t, 3> resolved{ResolveIndex(static_cast<std::int32_t>(nodeIndex), a),
                ResolveIndex(static_cast<std::int32_t>(nodeIndex), b), ResolveIndex(static_cast<std::int32_t>(nodeIndex), c)};
            if (resolved[0] < 0 || resolved[1] < 0 || resolved[2] < 0 || resolved[0] >= static_cast<std::int64_t>(model.vertices.size()) ||
                resolved[1] >= static_cast<std::int64_t>(model.vertices.size()) || resolved[2] >= static_cast<std::int64_t>(model.vertices.size()) ||
                resolved[0] == resolved[1] || resolved[1] == resolved[2] || resolved[0] == resolved[2]) {
                return;
            }
            model.indices.push_back(static_cast<std::uint32_t>(resolved[0]));
            model.indices.push_back(static_cast<std::uint32_t>(resolved[1]));
            model.indices.push_back(static_cast<std::uint32_t>(resolved[2]));
        };
        if (primitive == 5) {
            for (std::size_t index = 0; index + 2 < source.size(); index += 3) {
                emit(source[index], source[index + 1], source[index + 2]);
            }
        } else {
            for (std::size_t index = 0; index + 2 < source.size(); ++index) {
                if ((index & 1U) != 0) {
                    emit(source[index], source[index + 1], source[index + 2]);
                } else {
                    emit(source[index], source[index + 2], source[index + 1]);
                }
            }
        }
        return true;
    }

    std::span<const std::byte> bytes;
    std::span<const std::byte> container;
    const std::byte *clothChunk{};
    MT7Model &model;
    std::unordered_set<std::uint32_t> visited;
    std::vector<MeshRange> meshRanges;
};

} // namespace

// @verified - p
void ClampPointToSegmentRange(Vector3 *point, const Vector3 *start, const Vector3 *end, float startRadius, float endRadius) {
    const float dx = start->x - end->x, dy = start->y - end->y, dz = start->z - end->z;
    const float t =
        std::fmin(1.0f, std::fmax(0.0f, (((point->y - end->y) * dy + (point->x - end->x) * dx) + (point->z - end->z) * dz) / ((dy * dy + dx * dx) + dz * dz)));
    const float radius = (startRadius - endRadius) * t + endRadius;
    const Vector3 projection{t * dx + end->x, t * dy + end->y, t * dz + end->z};
    const Vector3 delta{point->x - projection.x, point->y - projection.y, point->z - projection.z};
    const float distance = std::sqrt((delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z);
    if (radius > distance) {
        const float scale = radius / distance;
        *point = {delta.x * scale + projection.x, delta.y * scale + projection.y, delta.z * scale + projection.z};
    }
}

const Matrix4x4 *SetMT7NodeTransformOverride(MT7Node &node, const Matrix4x4 *transform) {
    node.transformOverride = transform;
    return transform;
}

const Matrix4x4 *GetMT7NodeTransformOverride(const MT7Node &node) {
    return node.transformOverride;
}

// @verified - p
void ClearMT7NodeTransformOverrides(MT7Node *root) {
    for (MT7Node *node = root; node; node = node->runtimeSibling) {
        node->transformOverride = nullptr;
        if (node->runtimeChild)
            ClearMT7NodeTransformOverrides(node->runtimeChild);
    }
}

Matrix4x4 g_modelNodeTransformBase{};

// @verified - p
void CaptureModelNodeTransformBase() {
    Matrix4x4::GetTransformMatrix(&g_modelNodeTransformBase);
}

// @verified - p
void RestoreModelNodeTransformBase() {
    Matrix4x4::SetActiveTransform(&g_modelNodeTransformBase);
}

// @verified - p
std::int64_t FindMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *node, const MT7Node *target) {
    if (node->runtimeSibling)
        Matrix4x4_Push();
    if (node->transformOverride) {
        RestoreModelNodeTransformBase();
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    Matrix4x4::GetTransformMatrix(output);
    if (node == target) {
        if (node->runtimeSibling)
            Matrix4x4::Pop(1);
        return 1;
    }
    if (node->runtimeChild && FindMT7NodeTransformMatrix(output, node->runtimeChild, target) == 1) {
        if (node->runtimeSibling)
            Matrix4x4::Pop(1);
        return 1;
    }
    if (node->runtimeSibling) {
        Matrix4x4::Pop(1);
        if (FindMT7NodeTransformMatrix(output, node->runtimeSibling, target) == 1)
            return 1;
    }
    return 0;
}

// @verified - p
void GetMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *root, const MT7Node *target) {
    Matrix4x4_Push();
    CaptureModelNodeTransformBase();
    FindMT7NodeTransformMatrix(output, root, target);
    Matrix4x4::Pop(1);
}

void ApplyMT7NodeTransform(const MT7Node &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z != 0.0f) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.z)));
    }
    if (node.rotation.y != 0.0f) {
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.y)));
    }
    if (node.rotation.x != 0.0f) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.x)));
    }
    if (node.scale.x != 1.0f || node.scale.y != 1.0f || node.scale.z != 1.0f) {
        Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
    }
}

namespace {
std::int32_t g_mtTypeFlag{};
} // namespace

void mtSetUseCustomReadOverride() {
    g_mtTypeFlag = 1;
}

void mtResetOverrideFlag() {
    g_mtTypeFlag = 0;
}

bool mtIsMeshTypeOverridden(const MT7Model &model, const MT7Node &node) {
    if (g_mtTypeFlag == 0 || node.xb01Offset == 0)
        return false;
    std::uint32_t type{};
    std::memcpy(&type, model.source.data() + node.xb01Offset + 8, sizeof(type));
    return type == 0x3c000510U;
}

// @verified - p
MT7Node *GetChildObjectByIndex(MT7Model &model, MT7Node *node, std::uint16_t id) {
    if (!node)
        return nullptr;
    if (static_cast<std::uint16_t>(node->id) == id)
        return node;
    if (node->runtimeChild)
        if (MT7Node *const found = GetChildObjectByIndex(model, node->runtimeChild, id))
            return found;
    return node->runtimeSibling ? GetChildObjectByIndex(model, node->runtimeSibling, id) : nullptr;
}

// @verified - p
void InterpolateMdcWeightMatrix(float weight, const Matrix4x4 *base, const Matrix4x4 *authored, Matrix4x4 *output) {
    const float baseWeight = 1.0f - weight;
    for (unsigned row = 0; row < 4; ++row)
        for (unsigned column = 0; column < 4; ++column)
            output->rows[row][column] = baseWeight * base->rows[row][column] + weight * authored->rows[row][column];
}

// @verified - p
void InterpolateNormalizedMdcWeightMatrix(float weight, const Matrix4x4 *base, const Matrix4x4 *authored, Matrix4x4 *output) {
    const float baseWeight = 1.0f - weight;
    for (unsigned row = 0; row < 4; ++row)
        for (unsigned column = 0; column < 4; ++column)
            output->rows[row][column] = baseWeight * base->rows[row][column] + weight * authored->rows[row][column];
    for (unsigned row = 0; row < 3; ++row) {
        Vector3 axis{output->rows[row].x, output->rows[row].y, output->rows[row].z};
        NormalizeVector3(&axis);
        output->rows[row].x = axis.x;
        output->rows[row].y = axis.y;
        output->rows[row].z = axis.z;
    }
}

void InitializeMT7RuntimeLinks(MT7Model &model) {
    for (MT7Node &node : model.nodes) {
        node.runtimeChild = node.firstChild >= 0 ? &model.nodes[static_cast<std::size_t>(node.firstChild)] : nullptr;
        node.runtimeSibling = node.nextSibling >= 0 ? &model.nodes[static_cast<std::size_t>(node.nextSibling)] : nullptr;
        node.runtimeParent = node.parent >= 0 ? &model.nodes[static_cast<std::size_t>(node.parent)] : nullptr;
    }
}

void BindMT7WritableRuntimeMeshes(MT7Model &model, std::span<std::byte> sourceAllocation) {
    std::byte *const source = sourceAllocation.data() + (model.source.data() - sourceAllocation.data());
    for (MT7Node &node : model.nodes) {
        node.runtimeMesh = node.xb01Offset ? source + node.xb01Offset : nullptr;
        if (!node.runtimeMesh || !IsModelXB01(node.runtimeMesh))
            continue;
        const MT7Node *const owner = node.borrowedMesh ? node.borrowedMesh : &node;
        std::uint32_t commandWords;
        std::memcpy(&commandWords, node.runtimeMesh + 28, sizeof(commandWords));
        auto *command = reinterpret_cast<std::uint32_t *>(node.runtimeMesh + 28 + 4 * commandWords);
        std::size_t buffer = 0;
        while (*command) {
            if (static_cast<std::uint8_t>(*command) == 1) {
                for (std::uint32_t slot = 0; slot < command[1]; ++slot) {
                    const auto handle = owner->drawResources.vertexBuffers[buffer++].handle;
                    std::memcpy(node.runtimeMesh + 40 + 4 * slot, &handle, sizeof(handle));
                }
            }
            command += *command >> 8;
        }
    }
}

bool IsModelXB01(const void *mesh) {
    std::uint32_t signature;
    std::memcpy(&signature, mesh, sizeof(signature));
    return signature == xb01Signature;
}

void *CopyForDirectX(const void *mesh, std::uint32_t slot, std::uint32_t byteOffset, std::int64_t, std::int32_t preserveContents) {
    std::int32_t handle;
    std::memcpy(&handle, static_cast<const std::byte *>(mesh) + 40 + 4 * slot, sizeof(handle));
    return d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, handle, (preserveContents == 1) + 1, byteOffset);
}

std::int64_t CopyBufferToDirectX_Wrapper(const void *mesh, std::uint32_t slot) {
    std::int32_t handle;
    std::memcpy(&handle, static_cast<const std::byte *>(mesh) + 40 + 4 * slot, sizeof(handle));
    return d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, handle);
}

std::uint64_t IsMDPXorMDP7(const MT7Node *node) {
    return node->meshType == SHEN_NAME(MDP7) || node->meshType == SHEN_NAME(MDPX) ? node->modelMarker : 0;
}
// @verified - p
MT7Node *mtGetTransform(const MT7Node *node) {
    return node->runtimeChild;
}

// @verified - p
MT7Node *mtGetSibling(const MT7Node *node) {
    return node->runtimeSibling;
}

// @verified - p
std::uint32_t mt7GetXb01Offset(const MT7Node *node) {
    return node->xb01Offset;
}

// @verified - p
void mt7_UpdateChildOffset(MT7Node *node, MT7Node *child) {
    node->runtimeChild = child;
}

// @verified - p
void mt7_UpdateParentOffset(MT7Node *node, MT7Node *parent) {
    node->runtimeParent = parent;
}

// @verified - p
void mt7_UpdateSiblingOffset(MT7Node *node, MT7Node *sibling) {
    node->runtimeSibling = sibling;
}



// @verified - p
void mt7_CopyXB01Offset(MT7Node *node, const MT7Node *source) {
    node->xb01Offset = source->xb01Offset;
    node->borrowedMesh = source->xb01Offset ? (source->borrowedMesh ? source->borrowedMesh : source) : nullptr;
    node->runtimeMesh = source->xb01Offset ? source->runtimeMesh : nullptr;
}

bool ReadModelFormat7(std::span<const std::byte> bytes, const char *label, MT7Model &model) {
    if (MT7Reader(bytes, label, model).Run()) {
        InitializeMT7RuntimeLinks(model);
        return true;
    }
    ReleaseModelFormat7(model);
    return false;
}

const MT7Node *GetMT7ReaderData(const MT7Model *model) {
    return model && !model->nodes.empty() ? model->nodes.data() : nullptr;
}

namespace {

void RegisterModelNodeAliases(const MT7Node *node, const MT7Node *root, std::uintptr_t flag) {
    for (; node; node = mtGetSibling(node)) {
        d3t::RegisterObjectAlias(reinterpret_cast<const std::byte *>(node) + flag, reinterpret_cast<const std::byte *>(root) + flag);
        if (const MT7Node *const child = mtGetTransform(node))
            RegisterModelNodeAliases(child, root, flag);
    }
}
}

// @verified - p
void FilepathBuilder(const MT7Node *data, const char *directory, const char *subdirectory, const char *stem, std::uint16_t flags, std::uint32_t threadHandle) {
    char path[512];
    std::snprintf(path, sizeof(path), "/%s/%s/%s.XXX_/%s.fbx", *directory == '/' ? directory + 1 : directory, subdirectory ? subdirectory : "", stem, stem);
    std::uintptr_t flag = 1;
    if (!d3t::RegisterObjectSource(data, path, static_cast<std::uint16_t>(flags | 0x110U), threadHandle)) {
        if (!d3t::RegisterObjectSource(data, path, flags, threadHandle))
            return;
        flag = 0;
    }
    RegisterModelNodeAliases(data, data, flag);
}

bool ReadModelFormat7(std::span<const std::byte> bytes, const char *label, MT7Model &model, std::uint16_t flags) {
    if (!ReadModelFormat7(bytes, label, model)) {
        ReleaseModelFormat7(model);
        return false;
    }
    if (!label)
        return true;
    switch (model.signature) {
    case SHEN_NAME(MDL7):
    case SHEN_NAME(MDO7):
    case SHEN_NAME(MDC7):
    case SHEN_NAME(MDP7):
    case SHEN_NAME(MDLX):
    case SHEN_NAME(MDOX):
    case SHEN_NAME(MDCX):
    case SHEN_NAME(MDPX):
        break;
    default:
        return true;
    }
    const char *filename = label + std::strlen(label);
    while (filename > label && filename[-1] != '/' && filename[-1] != '\\')
        --filename;
    const std::size_t nameLength = std::strlen(filename);
    if (filename == label || nameLength < 4)
        return true;
    char directory[272]{};
    char stem[64]{};
    const std::size_t directoryLength = static_cast<std::size_t>(filename - label - 1);
    std::memcpy(directory, label, directoryLength < sizeof(directory) ? directoryLength : sizeof(directory) - 1);
    std::memcpy(stem, filename, nameLength - 4 < sizeof(stem) ? nameLength - 4 : sizeof(stem) - 1);
    try {
        if (const MT7Node *const data = GetMT7ReaderData(&model))
            FilepathBuilder(data, directory, nullptr, stem, flags);
    } catch (...) {
        ReleaseModelFormat7(model);
        throw;
    }
    return true;
}

// AssignFlags: a model whose first node id is above 0x7000 records that id less 0x7001 as its
// character type; any other model records zero.
// @verified - p
void AssignFlags(MT7Model *model) {
    if (!model)
        return;
    const std::int16_t id = model->nodes.empty() ? 0 : static_cast<std::int16_t>(model->nodes.front().id);
    model->entityType = !model->nodes.empty() && id > 28672 ? id - 28673 : 0;
}

void ReleaseModelFormat7(MT7Model &model) {
    FlushQueuedLegacyDrawPasses();
    if (const MT7Node *const data = GetMT7ReaderData(&model)) {
        d3t::ReleaseObjectSource(data);
        d3t::ReleaseObjectSource(data);
    }
    ReleaseModelFormat7Storage(model);
}

void ReleaseModelFormat7Storage(MT7Model &model) {
    FlushQueuedLegacyDrawPasses();
    while (d3t::ProcessObjectStore(true, -1)) {
    }
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
    ReleaseMapSeamResources(model);
#endif
    if (model.vertexMemoryOwner)
        FreeModelVertexMemory(model.vertexMemoryOwner);
    for (MT7Node &node : model.nodes)
        ReleaseLegacyDrawResources(node.drawResources);
    for (const std::array<std::uint32_t, 2> &id : model.embeddedTextureIds)
        ReleaseModelTexture(id[0], id[1]);
    model = {};
}

const LegacyVertexBufferBinding *FindMT7MutableVertexBuffer(const MT7Node &node) {
    if (node.borrowedMesh)
        return FindMT7MutableVertexBuffer(*node.borrowedMesh);
    if (!node.meshVertexStride || !node.meshVertexCount)
        return nullptr;
    const std::uint64_t required = static_cast<std::uint64_t>(node.meshVertexStride) * node.meshVertexCount;
    for (const LegacyVertexBufferBinding &binding : node.drawResources.vertexBuffers) {
        const std::int32_t size = d3t::GetSizeForModelBuffer(binding.handle);
        if (size > 0 && required <= static_cast<std::uint32_t>(size)) {
            return &binding;
        }
    }
    return nullptr;
}

} // namespace shenmue::s2
