#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/model_clone_dc.hpp"

#include "s2/hlib.hpp"
#include <cstring>
#include "s2/model_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "shenmue.h"
#include "s2/search_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_info.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/hand_draw_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/texture_dc.hpp"
#include "s2/event_matrix_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
std::uint32_t PrepareMT7Mesh(void *mesh, const void *textures);
}
} // namespace dc
} // namespace shenmue
#include "s2/cold_archive_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/event_overlay.hpp"
#include "s2/frame_dc.hpp"
#include "s2/initialization_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "s2/scene_data.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/stream_audio_dc.hpp"
#include <cstdio>
#include "platform/dc/mt7.hpp"
#include "s2/prop_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/face_data.hpp"
#include "common/types.hpp"
#include "s2/scenario_flags.hpp"
#include "s2/memo_dc.hpp"
#include <cmath>
#include <float.h>
#else
#include "s2/model_clone.hpp"

#include "d3t/object_store.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"
#include "s2/legacy_draw.hpp"

#include <bit>
#include <cstring>
#include <new>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t g_modelCloneNodeCount;

std::uint32_t g_modelCloneMeshCount{};

std::uint32_t g_modelCloneNonXb01Count{};

std::uint32_t g_modelCloneMdcBytes;

std::uint32_t g_modelCloneMdpBytes;

std::vector<std::vector<MT7Node>> *g_componentTeardownNodes{};

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetModelMdcCloneBlockSize(const ModelCloneNode *node);
std::int64_t CountModelNodeTreeForClone(const ModelCloneNode *node);
std::int64_t AccumulateModelMdcCloneBytes(const ModelCloneNode *node);
std::int64_t AccumulateModelMdpCloneBytes(const ModelCloneNode *node);
std::uint64_t MeasureModelNodeTreeClone(const ModelCloneNode *node);
std::byte *CopyModelNodeTree(ModelCloneNode *parent, const ModelCloneNode *source, std::byte *destination);
ModelCloneNode *CloneModelNodeTree(const void *sourceKey, const ModelCloneNode *node, std::uint8_t flags);
void ReleaseMT7ModelNodeTreeClone(MT7Model *model);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint32_t GetModelMdpCloneBlockSize(const MT7Node *node);

void ReleaseNamedTexture_Wrapper(std::int32_t index);

MT7Model *FindPreloadedCharacterImageNullsub(const char *filename);

MT7Model *ReadMT7Model(void *source, std::int32_t mode);

MT7Model *ReadMT7Model(void *source, std::int32_t mode);

MT7Model *ReadMT7Model(void *source, std::int32_t mode);

void CaptureModelNodeTransformBase();

void RestoreModelNodeTransformBase();

std::uint32_t g_modelCloneNodeCount, g_modelCloneMdcBytes = 0, g_modelCloneMdpBytes = 0;

void PropagateCharacterLinkedVertices(const MT7Node *node, const MT7Node *parent, const Matrix4x4 *matrix, const Matrix4x4 *parentMatrix);

Matrix4x4 g_characterModelMatrix;

std::int32_t g_characterLinkedVerticesEnabled = 1;

bool IsMT7ClothNode(const MT7Node *node);

std::int32_t g_characterClothDrawDisabled = 0;

namespace {

std::int32_t modelSlotFlag;

void *modelSlots[256];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t unusedReloadWord0, unusedReloadWord1, unusedReloadWord2;
#else
std::uint32_t unusedReloadWords[3];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const Matrix4x4 *characterModelFrameStream = 0;
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetModelSoundMaterial(const MT7Node *node) {
    const MT7NodeSoundData *const data = reinterpret_cast<const MT7NodeSoundData *>(node + 1);
    if (data->header.type == SHEN_NAME4('M', 'D', 'P', '7') && static_cast<std::int32_t>(data->header.size) > 12)
        return data->material;
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetModelSoundEnvironment(const MT7Node *node) {
    const MT7NodeSoundData *const data = reinterpret_cast<const MT7NodeSoundData *>(node + 1);
    if (data->header.type == SHEN_NAME4('M', 'D', 'P', '7') && static_cast<std::int32_t>(data->header.size) > 12)
        return data->environment;
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t UpdateMT7NodeTextureCoordinates(const MT7Node *node) {
    void *mesh = node->mesh;
    if (mesh) {
        if (!(reinterpret_cast<const std::uint32_t *>(mesh)[1] & 4U))
            return 0;
        UpdateMT7MeshTextureCoordinates(mesh);
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void BindAirMeshPackets(void *mesh, void *output) {
    std::uint32_t **const binding = static_cast<std::uint32_t **>(output);
    std::byte *const bytes = static_cast<std::byte *>(mesh);
    binding[0] = reinterpret_cast<std::uint32_t *>(bytes + 0x6c);
    std::byte *const packets = bytes + 0x64;
    binding[1] = reinterpret_cast<std::uint32_t *>(packets + (*reinterpret_cast<const std::uint32_t *>(packets) & ~3U));
    binding[2] = reinterpret_cast<std::uint32_t *>(bytes + 0x18);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FindAirMeshPackets(MT7Node *node, void *output) {
    if (node->mesh)
        BindAirMeshPackets(node->mesh, output);
    if (node->child)
        FindAirMeshPackets(node->child, output);
    if (node->sibling)
        FindAirMeshPackets(node->sibling, output);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMT7ClothNodeKinds(MT7Node *node) {
    switch (node->id) {
    default:
        node->kind = 0;
        break;
    case -0x41:
    case -0x42:
        node->kind = 1;
        break;
    case -0x5a:
    case -0x5b:
        node->kind = 2;
        break;
    case 5:
    case 10:
        node->kind = 3;
        break;
    }
    if (node->child)
        SetMT7ClothNodeKinds(node->child);
    if (node->sibling)
        SetMT7ClothNodeKinds(node->sibling);
}
#else
// @verified - d
void SetMT7ClothNodeKinds(MT7Node *node) {
    std::int16_t kind = 0;
    if (node->id < -0x40) {
        if (node->id > -0x43)
            kind = 1;
        else if (node->id < -0x59 && node->id > -0x5c)
            kind = 2;
    } else if (node->id == 5 || node->id == 10) {
        kind = 3;
    }
    node->kind = kind;
    if (node->child)
        SetMT7ClothNodeKinds(node->child);
    if (node->sibling)
        SetMT7ClothNodeKinds(node->sibling);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetMT7Parents(MT7Node *node, MT7Node *parent) {
    node->parent = parent;
    if (node->child)
        SetMT7Parents(node->child, node);
    if (node->sibling)
        SetMT7Parents(node->sibling, parent);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RelocateMT7Nodes(std::intptr_t base, MT7Node *node) {
    if (node->mesh && reinterpret_cast<std::intptr_t>(node->mesh) < base)
        node->mesh = reinterpret_cast<void *>(reinterpret_cast<std::intptr_t>(node->mesh) + base);
    if (node->child && reinterpret_cast<std::intptr_t>(node->child) < base)
        node->child = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->child) + base);
    if (node->sibling && reinterpret_cast<std::intptr_t>(node->sibling) < base)
        node->sibling = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->sibling) + base);
    if (node->parent && reinterpret_cast<std::intptr_t>(node->parent) < base)
        node->parent = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->parent) + base);
    node->transformOverride = 0;
    if (node->child)
        RelocateMT7Nodes(base, node->child);
    if (node->sibling)
        RelocateMT7Nodes(base, node->sibling);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CountModelNodeTreeForClone(const MT7Node *node) {
    ++g_modelCloneNodeCount;
    if (node->child)
        CountModelNodeTreeForClone(node->child);
    if (node->sibling)
        CountModelNodeTreeForClone(node->sibling);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetModelNodeCountForClone(const MT7Node *node) {
    g_modelCloneNodeCount = 0;
    if (node)
        CountModelNodeTreeForClone(node);
    return g_modelCloneNodeCount;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetModelMdcCloneBlockSize(const MT7Node *node) {
    std::uint32_t size = 0, count, words, block;
    const std::byte *chunk;
    const std::uint32_t *cursor;
    if (static_cast<std::uint16_t>(node->id - 0x44U) < 2U) {
        chunk = reinterpret_cast<const std::byte *>(node + 1);
        if (*reinterpret_cast<const shen_name *>(chunk) == MODEL_HAND_CHUNK) {
            count = reinterpret_cast<const MT7NodeDataHeader *>(chunk)->size;
            chunk += count + 8;
            size = count;
            size += 8;
            count = reinterpret_cast<const std::uint32_t *>(chunk)[1] * 48;
            chunk += count + 8;
            size += 8;
            size += count;
            count = reinterpret_cast<const MT7NodeDataHeader *>(chunk)->size;
            chunk += count + 12;
            size += 12;
            size += count;
            count = reinterpret_cast<const std::uint32_t *>(chunk)[4];
            count *= 8;
            size += 20;
            size += count;
        } else if (*reinterpret_cast<const shen_name *>(chunk) == MODEL_CLOTH_CHUNK) {
            size += 8;
            count = reinterpret_cast<const MT7NodeDataHeader *>(chunk)->size;
            chunk += count + 8;
            size += count;
            count = reinterpret_cast<const std::uint32_t *>(chunk)[1] * 48;
            chunk += count + 8;
            size += 8;
            size += count;
            count = reinterpret_cast<const MT7NodeDataHeader *>(chunk)->size;
            chunk += count + 12;
            size += 12;
            size += count;
            count = reinterpret_cast<const std::uint32_t *>(chunk)[1];
            count *= 8;
            size += 8;
            size += count;
        } else {
            return 0;
        }
        return size;
    }
    if (node->id == 5 || node->id == 10) {
        cursor = reinterpret_cast<const std::uint32_t *>(node + 1);
        if (*cursor++ != MODEL_CLOTH_CHUNK)
            return size;
        cursor += 16;
        count = *cursor++;
        if (!count) {
            ++cursor;
            words = 2;
        } else {
            cursor += count * 2;
            words = count * 2 + 1;
        }
        size = (words + 17) * 4;
        if (*cursor == MODEL_WEIGHT_CHUNK) {
            ++cursor;
            count = *cursor++;
            block = count * 17;
            size += 8;
            cursor += block;
            size += block * 4;
            ++cursor;
            count = *cursor++;
            size += 8;
            block = count * 48;
            cursor = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(cursor) + block);
            size += block;
            ++cursor;
            count = *cursor++;
            ++cursor;
            size += 12;
            cursor += count / 4;
            size += count;
            count = cursor[1];
            count *= 8;
            size += 8;
            size += count;
            return size;
        }
        count = *cursor;
    } else {
        cursor = reinterpret_cast<const std::uint32_t *>(node) + 17;
        cursor += 16;
        count = *cursor++;
        if (!count) {
            ++cursor;
            words = 2;
        } else {
            cursor += count * 2;
            words = count * 2 + 1;
        }
        count = *cursor;
    }
    if (count)
        size = count * 8 + 1;
    else
        size = 1;
    return (words + size + 17) * 4;
}
#else
// @verified - d
std::uint32_t GetModelMdcCloneBlockSize(const MT7Node *node) {
    const MT7ClothNodeData *const data = reinterpret_cast<const MT7ClothNodeData *>(node + 1);
    const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(data);
    if (static_cast<std::uint16_t>(node->id - 0x44U) < 2U) {
        if (data->header.type != MODEL_HAND_CHUNK && data->header.type != MODEL_CLOTH_CHUNK)
            return 0;
        const std::byte *const bytes = reinterpret_cast<const std::byte *>(data);
        const std::uint32_t prefixBytes = data->header.size;
        const std::uint32_t vertices = *reinterpret_cast<const std::uint32_t *>(bytes + prefixBytes + 0x0c);
        const std::uint32_t vertexBytes = vertices * 0x30U;
        const std::uint32_t meshOffset = prefixBytes + vertexBytes + 0x10U;
        const std::uint32_t meshBytes = *reinterpret_cast<const std::uint32_t *>(bytes + meshOffset + 4);
        if (data->header.type == MODEL_HAND_CHUNK) {
            const std::uint32_t links = *reinterpret_cast<const std::uint32_t *>(bytes + meshOffset + meshBytes + 0x1c);
            return prefixBytes + vertexBytes + meshBytes + 0x30U + links * 8U;
        }
        const std::uint32_t links = *reinterpret_cast<const std::uint32_t *>(bytes + meshOffset + meshBytes + 0x10);
        return prefixBytes + vertexBytes + meshBytes + 0x24U + links * 8U;
    }
    const bool weighted = node->id == 5 || node->id == 10;
    if (weighted && data->header.type != MODEL_CLOTH_CHUNK)
        return 0;
    const std::uint32_t count = data->linkCount;
    const std::uint32_t prefixWords = count ? count * 2U + 1U : 2U;
    const std::uint32_t *const table = words + (count ? 18U + count * 2U : 19U);
    if (weighted && table[0] == MODEL_WEIGHT_CHUNK) {
        const std::uint32_t weights = table[1];
        const std::uint32_t vertices = table[weights * 17U + 3U];
        const std::uint32_t *const mesh = table + weights * 17U + vertices * 12U + 5U;
        const std::uint32_t meshBytes = *mesh;
        const std::uint32_t links = *reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(mesh) + (meshBytes & ~3U) + 0x0c);
        return (prefixWords + 17U) * 4U + weights * 0x44U + vertices * 0x30U + meshBytes + 0x24U + links * 8U;
    }
    const std::uint32_t links = table[0] ? table[0] * 8U + 1U : 1U;
    return (prefixWords + links + 17U) * 4U;
}
#endif

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AccumulateModelMdcCloneBytes(const MT7Node *node) {
    const MT7NodeDataHeader *const header = reinterpret_cast<const MT7NodeDataHeader *>(node + 1);
    const std::uint32_t bytes = header->type == MODEL_CLOTH_CHUNK || header->type == MODEL_HAND_CHUNK ? GetModelMdcCloneBlockSize(node) : 0;
    g_modelCloneMdcBytes += bytes;
    if (node->child)
        AccumulateModelMdcCloneBytes(node->child);
    if (node->sibling)
        AccumulateModelMdcCloneBytes(node->sibling);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetModelMdcCloneBytes(const MT7Node *node) {
    g_modelCloneMdcBytes = 0;
    if (node)
        AccumulateModelMdcCloneBytes(node);
    return g_modelCloneMdcBytes;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::byte *CopyModelNodeTree(MT7Node *parent, const MT7Node *source, std::byte *destination) {
    *reinterpret_cast<MT7Node *>(destination) = *source;
    MT7Node *const copy = reinterpret_cast<MT7Node *>(destination);
    destination += sizeof(MT7Node);
    const MT7NodeDataHeader *const header = reinterpret_cast<const MT7NodeDataHeader *>(source + 1);
    std::uint32_t bytes;
    if (header->type == MODEL_CLOTH_CHUNK || header->type == MODEL_HAND_CHUNK) {
        bytes = GetModelMdcCloneBlockSize(source);
        std::memcpy(destination, source + 1, bytes);
        destination += bytes;
    } else if (header->type == MODEL_DEFORMATION_CHUNK) {
        bytes = GetModelMdpCloneBlockSize(source);
        std::memcpy(destination, source + 1, bytes);
        destination += bytes;
    }
    copy->parent = parent;
    if (source->child) {
        copy->child = reinterpret_cast<MT7Node *>(destination);
        destination = CopyModelNodeTree(copy, source->child, destination);
    }
    if (source->sibling) {
        copy->sibling = reinterpret_cast<MT7Node *>(destination);
        destination = CopyModelNodeTree(parent, source->sibling, destination);
    }
    return destination;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AccumulateModelMdpCloneBytes(const MT7Node *node) {
    const MT7NodeDataHeader *const header = reinterpret_cast<const MT7NodeDataHeader *>(node + 1);
    const std::uint32_t bytes = header->type == MODEL_DEFORMATION_CHUNK ? GetModelMdpCloneBlockSize(node) : 0;
    g_modelCloneMdpBytes += bytes;
    if (node->child)
        AccumulateModelMdpCloneBytes(node->child);
    if (node->sibling)
        AccumulateModelMdpCloneBytes(node->sibling);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetModelMdpCloneBytes(const MT7Node *node) {
    g_modelCloneMdpBytes = 0;
    if (node) {
        AccumulateModelMdpCloneBytes(node);
        g_modelCloneMdpBytes += 4;
    }
    return g_modelCloneMdpBytes;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetModelMdpCloneBlockSize(const MT7Node *node) {
    return reinterpret_cast<const MT7NodeDataHeader *>(node + 1)->size;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t MeasureModelNodeTreeClone(const MT7Node *node) {
    const MT7NodeDataHeader *const header = reinterpret_cast<const MT7NodeDataHeader *>(node + 1);
    std::uint32_t bytes = GetModelNodeCountForClone(node) * sizeof(MT7Node);
    if (header->type == MODEL_CLOTH_CHUNK || header->type == MODEL_HAND_CHUNK)
        bytes += GetModelMdcCloneBytes(node);
    else if (header->type == MODEL_DEFORMATION_CHUNK)
        bytes += GetModelMdpCloneBytes(node);
    return bytes;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CopyModelNodeTreeClone(const MT7Node *source, MT7Node *destination) {
    std::byte *const end = CopyModelNodeTree(0, source, reinterpret_cast<std::byte *>(destination));
    if (reinterpret_cast<const MT7NodeDataHeader *>(source + 1)->type == MODEL_DEFORMATION_CHUNK)
        *reinterpret_cast<std::uint32_t *>(end) = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *CloneModelNodeTree(const MT7Node *node) {
    if (node) {
        const std::uint32_t bytes = MeasureModelNodeTreeClone(node);
        if (bytes) {
            MT7Node *const clone = static_cast<MT7Node *>(HLib::AllocateTaskData(bytes, MEMORY_MODEL_CLONE));
            if (clone)
                CopyModelNodeTreeClone(node, clone);
            return clone;
        }
    }
    return 0;
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Model *InitializeMT7Model(void *source) {
    return ReadMT7Model(source, 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Model *ReadMT7ModelTextureNames(void *source, MemoTextureName *names, std::int32_t *textures) {
    MT7Model *const model = ReadMT7Model(source, 0);
    if (model) {
        MT7ModelChunk *const chunk = static_cast<MT7ModelChunk *>(source);
        const MT7TextureBinding *const bindings = reinterpret_cast<const MT7TextureBinding *>(chunk + 1);
        const std::uint32_t *pairs = reinterpret_cast<const std::uint32_t *>(bindings + chunk->textureCount);
        std::int32_t *const retained = reinterpret_cast<std::int32_t *>(model + 1);
        for (std::int32_t index = 0; index < chunk->textureCount; ++index) {
            names[index].texture = bindings[index].texture;
            names[index].nameLow = *pairs++;
            names[index].nameHigh = *pairs++;
            textures[index] = names[index].texture;
            retained[index] = names[index].texture;
        }
        model->retainedTextureCount = chunk->textureCount;
    }
    return model;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Model *ReadAirCloudModel(void *source, std::int32_t *textures) {
    MT7Model *const model = ReadMT7Model(source, 1);
    if (model) {
        const MT7ModelChunk *const chunk = reinterpret_cast<const MT7ModelChunk *>(source);
        const MT7TextureBinding *const bindings = reinterpret_cast<const MT7TextureBinding *>(chunk + 1);
        for (std::int32_t index = 0; index < chunk->textureCount; ++index)
            textures[index] = bindings[index].texture;
        model->retainedTextureCount = chunk->textureCount;
    }
    return model;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RebindMT7ModelTextures(MT7Model *model, const MemoTextureName *names, std::int32_t count, const std::int32_t *) {
#else
void RebindMT7ModelTextures(MT7Model *model, const MemoTextureName *names, std::int32_t count) {
#endif
    if (!model || !model->source)
        return;
    MT7ModelChunk *const chunk = static_cast<MT7ModelChunk *>(model->source);
    MT7TextureBinding *const textures = reinterpret_cast<MT7TextureBinding *>(chunk + 1);
    for (std::int32_t index = 0; index < count; ++index)
        textures[index].texture = names[index].texture;
    BindMT7NodeTextures(chunk->root, textures);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ScrollAirCloudPackets(AirCloudLayer *layer, std::int32_t mode, float u, float v) {
    if (mode)
        return;
    float deltaU = layer->scroll.x;
    float deltaV = layer->scroll.y;
    layer->scroll.x += u;
    if (layer->scroll.x > 1.0f)
        layer->scroll.x -= 1.0f;
    if (layer->scroll.x < -1.0f)
        layer->scroll.x += 1.0f;
    layer->scroll.y += v;
    if (layer->scroll.y > 1.0f)
        layer->scroll.y -= 1.0f;
    if (layer->scroll.y < -1.0f)
        layer->scroll.y += 1.0f;
    deltaU = layer->scroll.x - deltaU;
    deltaV = layer->scroll.y - deltaV;
    std::uint32_t *packet = layer->packets;
    std::uint32_t *const end = layer->packetEnd;
    std::uint16_t (*const colours)[2] = layer->colours;
    std::int32_t vertex = 0;
    do {
        const std::int32_t count = static_cast<std::int32_t>(*packet++);
        for (std::int32_t index = 0; index < count; ++index) {
            if (*packet & 1U) {
                const std::uint8_t level = airCloudVertexLevels[vertex];
                float *uv = reinterpret_cast<float *>(packet + 6);
                *uv += deltaU;
                ++uv;
                *uv += deltaV;
                packet[7] |= 1U;
                packet[4] = static_cast<std::uint32_t>(colours[level][0]) << 16U | colours[level][1];
                ++vertex;
                packet += 8;
            } else
                packet += 2;
        }
        ++packet;
    } while (end > packet);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RecolourAirDomePackets(AirDomeLayer *layer) {
    std::uint32_t *packet = layer->packets;
    std::uint32_t *const end = layer->packetEnd;
    std::uint16_t (*const colours)[2] = layer->colours;
    std::int32_t vertex = 0;
    do {
        const std::int32_t count = static_cast<std::int32_t>(*packet++);
        for (std::int32_t index = 0; index < count; ++index) {
            if (*packet & 1U) {
                const std::uint8_t level = airDomeVertexLevels[vertex];
                packet[4] = static_cast<std::uint32_t>(colours[level][0]) << 16U | colours[level][1];
                ++vertex;
                packet += 8;
            } else
                packet += 2;
        }
        ++packet;
    } while (end > packet);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void UpdateAirMeshPackets() {
    RecolourAirDomePackets(&g_airDomeLayers[0]);
    AirCloudLayer *layer = g_airCloudLayers;
    for (std::int32_t index = 0; index < 2; ++index, ++layer)
        ScrollAirCloudPackets(layer, 0, layer->scrollRate.x, layer->scrollRate.y);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseMT7Model(MT7Model *model) {
    if (!HLib::GetTaskDataName(model))
        return;
    if (model->retainedTextureCount) {
        const std::int32_t *const textures = reinterpret_cast<const std::int32_t *>(model + 1);
        for (std::int32_t index = 0; index < model->retainedTextureCount; ++index)
            ReleaseNamedTexture_Wrapper(textures[index]);
    } else {
        MT7ModelChunk *const chunk = static_cast<MT7ModelChunk *>(model->source);
        MT7TextureBinding *const textures = reinterpret_cast<MT7TextureBinding *>(chunk + 1);
        for (std::int32_t index = 0; index < chunk->textureCount; ++index) {
            if (textures[index].texture != -1) {
                ReleaseNamedTexture_Wrapper(textures[index].texture);
                textures[index].texture = -1;
            }
        }
    }
    HLib::FreeTaskData(model->source);
    HLib::FreeTaskData(model);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *ParseMT7ModelRoot(void *source, MT7Model **output) {
    MT7Model *const model = InitializeMT7Model(source);
    if (output)
        *output = model;
    if (model)
        return model->root;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *CloneMT7ModelRoot(MT7Model *model) {
    return CloneModelNodeTree(model->root);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseMT7ModelNodeTreeClone(MT7Node *root) {
    HLib::FreeTaskData(root);
}

#else

void ReleaseMT7ModelNodeTreeClone(MT7Model *model) {
    if (!model)
        return;
    FlushQueuedLegacyDrawPasses();
    d3t::RetireObjectPlacement(GetMT7ReaderData(model));
    while (d3t::ProcessObjectStore(true, -1)) {
    }
    if (g_componentTeardownNodes)
        g_componentTeardownNodes->push_back(std::move(model->nodes));
    model->~MT7Model();
    HLib::FreeTaskData(model);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetMT7ModelEntityType(MT7Model *model, const char *) {
    if (!model)
        return;
    if (model->root && *reinterpret_cast<const std::int16_t *>(model->root) > 0x7000)
        model->entityType = *reinterpret_cast<const std::int16_t *>(model->root) - 0x7001;
    else
        model->entityType = 0;
}

MT7Model *InitializeTypedMT7Model(void *source, const char *filename) {
    MT7Model *const model = InitializeMT7Model(source);
    SetMT7ModelEntityType(model, filename);
    return model;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void *LoadPackageCharacterModel(void *data, const char *filename, PakGlobalData *slot) {
    const std::uint32_t type = *reinterpret_cast<const std::uint32_t *>(data);
    if (((type ^ MT7_MODEL_FILE_TYPE) & 0xff00ffffU) != 0)
        return 0;
    MT7Model *const model = InitializeMT7Model(data);
    if (slot->flags & PakKeepRetained)
        HLib::SetTaskDataMotionFlag(model);
    SetMT7ModelEntityType(model, filename);
    return model;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Model *LoadMT7Model(const char *directory, const char *filename) {
    MT7Model *model = FindPreloadedCharacterImageNullsub(filename);
    if (model)
        SetMT7ModelEntityType(model, filename);
    else
        model = InitializeTypedMT7Model(shenLoadFile(directory, filename, MEMORY_MODEL_FILE), filename);
    return model;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *FindImageGroupNode(MT7Node *node, std::int32_t group, std::int32_t variant) {
    while (node->id != group) {
        node = node->sibling;
        if (!node)
            return 0;
    }
    node = node->child;
    for (std::int32_t index = 0; index < variant; ++index) {
        node = node->sibling;
        if (!node)
            return 0;
    }
    return node;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *CloneImageSelection(MT7Model *source, std::uint32_t child, std::int32_t group, std::uint32_t variant) {
    MT7Node *node = source->root;
    if (child) {
        node = node->child;
        while (node) {
            if (static_cast<std::int32_t>(child) < 2)
                break;
            node = node->sibling;
            --child;
        }
    }
    if (!node)
        return 0;
    if (group != -1 && (node->id == 0x3e || node->id > 0x7000)) {
        MT7Node *const selected = FindImageGroupNode(node->child, group, static_cast<std::int32_t>(variant));
        if (selected)
            node = selected;
        if (!node)
            return 0;
    }
    MT7Node *const sibling = node->sibling;
    node->sibling = 0;
    MT7Node *const clone = CloneModelNodeTree(node);
    node->sibling = sibling;
    return clone;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *FindMT7Node(MT7Node *node, std::int16_t id) {
    if (!node)
        return 0;
    if (node->id == id)
        return node;
    if (node->child) {
        MT7Node *const found = FindMT7Node(node->child, id);
        if (found)
            return found;
    }
    if (node->sibling) {
        MT7Node *const found = FindMT7Node(node->sibling, id);
        if (found)
            return found;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ClearShadowNodeTransforms(MT7Node *node) {
    if (node) {
        node->transformOverride = 0;
        if (node->child)
            ClearShadowNodeTransforms(node->child);
        if (node->sibling)
            ClearShadowNodeTransforms(node->sibling);
    }
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BindShadowNodeTransforms(MT7Node *source, MT7Node *destination) {
    destination->transformOverride = 0;
    if (destination->id != -1) {
        MT7Node *const found = FindMT7Node(source, destination->id);
        if (found)
            destination->transformOverride = found->transformOverride;
    }
    if (destination->child)
        BindShadowNodeTransforms(source, destination->child);
    if (destination->sibling)
        BindShadowNodeTransforms(source, destination->sibling);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void PublishSharedShadowNodeTransforms(MT7Node *source, MT7Node *destination) {
    if (source && destination && source != destination)
        BindShadowNodeTransforms(source, destination);
    else if (destination)
        ClearShadowNodeTransforms(destination);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SetMT7NodeTransformOverrideById(MT7Node *node, std::int16_t id, const Matrix4x4 *transform) {
    if (!node)
        return 0;
    if (node->id == id) {
        node->transformOverride = transform;
        return 1;
    }
    if (node->child && SetMT7NodeTransformOverrideById(node->child, id, transform))
        return 1;
    if (node->sibling && SetMT7NodeTransformOverrideById(node->sibling, id, transform))
        return 1;
    return 0;
}

#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrepareCharacterModelNodes(const MT7Node *node, const Matrix4x4 *parentMatrix) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::UnitCurrentMatrix();
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    PropagateCharacterLinkedVertices(node, node->parent, &matrix, parentMatrix);
    if (node->child)
        PrepareCharacterModelNodes(node->child, &matrix);
    if (node->sibling) {
        PopCurrentMatrix(1);
        PrepareCharacterModelNodes(node->sibling, parentMatrix);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawCharacterModelNodes(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.y);
        }
    }
    if (node->mesh) {
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        else
            dc::sdk::DrawLitMT7Mesh(node->mesh);
    }
    if (node->child)
        DrawCharacterModelNodes(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawCharacterModelNodes(node->sibling);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void imgmDrawModelNode(const MT7Node *node, float) {
    matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.y);
        }
    }
    if (node->mesh) {
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        else
            dc::sdk::DrawLitMT7Mesh(node->mesh);
    }
    PopCurrentMatrix(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawModelNodeTree(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.y);
        }
    }
    if (node->mesh) {
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        else
            dc::sdk::DrawLitMT7Mesh(node->mesh);
    }
    if (node->child)
        DrawModelNodeTree(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawModelNodeTree(node->sibling);
    }
}

// @verified - d
void DrawCharacterModelTree(const MT7Node *node) {
    if (node) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::GetMatrix(&g_characterModelMatrix);
        DrawModelNodeTree(node);
        PopCurrentMatrix(1);
    }
}

// @verified - d
void DrawModelNodeTreePushed(MT7Node *node) {
    if (node) {
        MT7Node *const sibling = node->sibling;
        node->sibling = 0;
        matrix_stack::Matrix4x4_Push(0);
        DrawModelNodeTree(node);
        PopCurrentMatrix(1);
        node->sibling = sibling;
    }
}

// @verified - d
void DrawModelNodeLocal(const MT7Node *node) {
    matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.y);
        }
    }
    if (node->mesh) {
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        else
            dc::sdk::DrawLitMT7Mesh(node->mesh);
    }
    PopCurrentMatrix(1);
}

// @verified - d
void DrawModelMesh(void *mesh) {
    if (static_cast<std::uint32_t *>(mesh)[1] & 4U)
        UpdateMT7MeshTextureCoordinates(mesh);
    if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
        dc::sdk::DrawSingleLightMT7Mesh(mesh);
    else
        dc::sdk::DrawLitMT7Mesh(mesh);
}

// @verified - d
void DrawModelMeshAlpha(void *mesh, float alpha) {
    if (static_cast<std::uint32_t *>(mesh)[1] & 4U)
        UpdateMT7MeshTextureCoordinates(mesh);
    if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
        dc::sdk::DrawSingleLightMT7MeshAlpha(mesh, alpha);
    else
        dc::sdk::DrawLitMT7MeshAlpha(mesh, alpha);
}

// @verified - d
void DrawCharacterModelNodeById(MT7Node *root, std::int16_t id) {
    if (!root)
        return;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::GetMatrix(&g_characterModelMatrix);
    MT7Node *const rootSibling = root->sibling;
    root->sibling = 0;
    MT7Node *node;
    if (id == -1)
        node = 0;
    else
        node = FindMT7Node(root, id);
    if (node) {
        MT7Node *const child = node->child;
        MT7Node *const sibling = node->sibling;
        node->child = 0;
        node->sibling = 0;
        DrawCharacterModelNodes(root);
        node->child = child;
        node->sibling = sibling;
    } else {
        DrawCharacterModelNodes(root);
    }
    root->sibling = rootSibling;
    PopCurrentMatrix(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawAlphaCharacterModelNodes(const MT7Node *node, float alpha) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.y);
        }
    }
    if (node->mesh) {
        if (!g_characterClothDrawDisabled) {
            if (IsMT7ClothNode(node)) {
                ApplyMT7ClothDeformation(node);
            } else if (node->transformOverride && node->parent && node->parent->mesh && node->parent->transformOverride) {
                ApplyMT7ParentLinkedVertices(node, node->parent, node->transformOverride, node->parent->transformOverride);
            }
        }
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1)
            dc::sdk::DrawSingleLightMT7MeshAlpha(node->mesh, alpha);
        else
            dc::sdk::DrawLitMT7MeshAlpha(node->mesh, alpha);
    }
    if (node->child)
        DrawAlphaCharacterModelNodes(node->child, alpha);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawAlphaCharacterModelNodes(node->sibling, alpha);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawCharacterModelBranch(MT7Node *node) {
#else
void DrawCharacterModelBranch(MT7Node *node, float) {
#endif
    if (!node)
        return;
    MT7Node *const sibling = node->sibling;
    node->sibling = 0;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    PrepareCharacterModelNodes(node, 0);
    PopCurrentMatrix(1);
    DrawCharacterModelNodes(node);
    PopCurrentMatrix(1);
    node->sibling = sibling;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void charaApplyScale(const MT7Node *node, float) {
    if (node) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::GetMatrix(&g_characterModelMatrix);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        PrepareCharacterModelNodes(node, 0);
        PopCurrentMatrix(1);
        DrawCharacterModelNodes(node);
        PopCurrentMatrix(1);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawSingleCharacterModel(MT7Node *node) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    DrawCharacterModelBranch(node);
#else
    reinterpret_cast<void (*)(MT7Node *)>(DrawCharacterModelBranch)(node);
#endif
}

void DrawSingleAlphaCharacterModel(MT7Node *node, float alpha) {
    DrawAlphaCharacterModelBranch(node, alpha);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void charaPrepareNodesForDraw(const MT7Node *node, float alpha) {
    if (node) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::GetMatrix(&g_characterModelMatrix);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        PrepareCharacterModelNodes(node, 0);
        PopCurrentMatrix(1);
        DrawAlphaCharacterModelNodes(node, alpha);
        PopCurrentMatrix(1);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ModelNodeTraversalNullsub(const MT7Node *) {
}

// @verified - d
void TraverseModelNodeTransforms(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    if (node->mesh)
        ModelNodeTraversalNullsub(node);
    if (node->child)
        TraverseModelNodeTransforms(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        TraverseModelNodeTransforms(node->sibling);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t ClearModelSlotFlag() {
    modelSlotFlag = 0;
    return 0;
}

std::int32_t GetModelSlotFlag() {
    return modelSlotFlag;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t FindMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *node, const MT7Node *target) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push();
    if (node->transformOverride) {
        RestoreModelNodeTransformBase();
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    matrix_stack::GetMatrix(output);
    if (node == target) {
        if (node->sibling)
            PopCurrentMatrix(1);
        return 1;
    }
    if (node->child && FindMT7NodeTransformMatrix(output, node->child, target) == 1) {
        if (node->sibling)
            PopCurrentMatrix(1);
        return 1;
    }
    if (node->sibling) {
        PopCurrentMatrix(1);
        if (FindMT7NodeTransformMatrix(output, node->sibling, target) == 1)
            return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void GetMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *root, const MT7Node *target) {
    matrix_stack::Matrix4x4_Push();
    CaptureModelNodeTransformBase();
    FindMT7NodeTransformMatrix(output, root, target);
    PopCurrentMatrix(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ClearModelSlots() {
    for (std::int32_t i = 0; i < 256; ++i)
        modelSlots[i] = 0;
}

std::int32_t AddModelSlot(void *entry) {
    for (std::int32_t i = 0; i < 256; ++i) {
        if (!modelSlots[i]) {
            modelSlots[i] = entry;
            return 1;
        }
    }
    return 0;
}

std::int32_t RemoveModelSlot(void *entry) {
    for (std::int32_t i = 0; i < 256; ++i) {
        if (modelSlots[i] == entry) {
            modelSlots[i] = 0;
            return 1;
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RebaseMT7NodeTree(MT7Node *node, std::intptr_t delta) {
    if (node->parent)
        node->parent = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->parent) + delta);
    if (node->mesh)
        node->mesh = reinterpret_cast<void *>(reinterpret_cast<std::intptr_t>(node->mesh) - delta);
    if (node->child) {
        node->child = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->child) + delta);
        RebaseMT7NodeTree(node->child, delta);
    }
    if (node->sibling) {
        node->sibling = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(node->sibling) + delta);
        RebaseMT7NodeTree(node->sibling, delta);
    }
}

// @verified - d
void RebaseMT7Model(MT7Node *previous, MT7Node *current) {
    RebaseMT7NodeTree(current, reinterpret_cast<std::intptr_t>(current) - reinterpret_cast<std::intptr_t>(previous));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ResetUnusedReloadState() {
    unusedReloadWord1 = 0;
    unusedReloadWord0 = 0;
    unusedReloadWord2 = 0;
    return 0;
}
#else
// @verified - d
std::int64_t ResetUnusedReloadState() {
    unusedReloadWords[1] = 0;
    unusedReloadWords[0] = 0;
    unusedReloadWords[2] = 0;
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReloadModelResourcesNullsub() {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Model *FindPreloadedCharacterImageNullsub(const char *) {
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetGeometryLightTreeTarget(const Matrix4x4 *transform) {
    g_geometryLightTransform = transform;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ModelHasFaceNode(const MT7ModelChunk *chunk) {
    if (chunk && chunk->root && FindMT7Node(chunk->root, 70))
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GraftFacialModelNode(void *face, MT7Node *root) {
    FacialModelData *data = static_cast<FacialModelData *>(face);
    if (data->reserved10 || data->reserved14)
        return;
    MT7Node *const replacement = FindMT7Node(root, 0x46);
    MT7Node *original = 0;
    if (replacement) {
        original = FindMT7Node(root, -0x43);
        if (!original)
            original = FindMT7Node(root, -0x43);
    }
    if (replacement && original) {
        replacement->id = original->id;
        if (replacement == replacement->parent->child) {
            replacement->parent->child = replacement->sibling;
        } else {
            MT7Node *previous = replacement->parent->child;
            while (previous && previous->sibling != replacement)
                previous = previous->sibling;
            if (previous && previous->sibling == replacement)
                previous->sibling = replacement->sibling;
        }
        replacement->sibling = original->sibling;
        MT7Node *last = replacement;
        if (replacement->child) {
            last = replacement->child;
            while (last->sibling)
                last = last->sibling;
            last->sibling = original->child;
        } else {
            last->child = original->child;
        }
        if (data) {
            data->reserved10 = reinterpret_cast<std::intptr_t>(original);
            data->reserved14 = reinterpret_cast<std::intptr_t>(replacement);
        }
        if (original == original->parent->child) {
            original->parent->child = replacement;
            if (data)
                data->modelOffset = reinterpret_cast<std::intptr_t>(&original->parent->child);
        } else {
            MT7Node *previous = original->parent->child;
            while (previous && previous->sibling != original)
                previous = previous->sibling;
            if (previous) {
                previous->sibling = replacement;
                if (data)
                    data->modelOffset = reinterpret_cast<std::intptr_t>(&previous->sibling);
            }
        }
    } else {
        if (replacement)
            replacement->id = -0x43;
    }
    SetMT7Parents(root, 0);
}
#else
// @verified - d
void GraftFacialModelNode(void *face, MT7Node *root) {
    FacialModelData &data = *static_cast<FacialModelData *>(face);
    if (data.reserved10 || data.reserved14)
        return;
    MT7Node *const replacement = FindMT7Node(root, 0x46);
    MT7Node *original = 0;
    if (replacement) {
        original = FindMT7Node(root, -0x43);
        if (!original)
            original = FindMT7Node(root, -0x43);
    }
    if (!replacement || !original) {
        if (replacement)
            replacement->id = -0x43;
    } else {
        replacement->id = original->id;
        if (replacement->parent->child == replacement) {
            replacement->parent->child = replacement->sibling;
        } else {
            MT7Node *previous = replacement->parent->child;
            while (previous && previous->sibling != replacement)
                previous = previous->sibling;
            if (previous && previous->sibling == replacement)
                previous->sibling = replacement->sibling;
        }
        replacement->sibling = original->sibling;
        if (!replacement->child) {
            replacement->child = original->child;
        } else {
            MT7Node *last = replacement->child;
            while (last->sibling)
                last = last->sibling;
            last->sibling = original->child;
        }
        if (face) {
            data.reserved10 = reinterpret_cast<std::intptr_t>(original);
            data.reserved14 = reinterpret_cast<std::intptr_t>(replacement);
        }
        if (original->parent->child == original) {
            original->parent->child = replacement;
            if (face)
                data.modelOffset = reinterpret_cast<std::intptr_t>(&original->parent->child);
        } else {
            MT7Node *previous = original->parent->child;
            while (previous && previous->sibling != original)
                previous = previous->sibling;
            if (previous) {
                previous->sibling = replacement;
                if (face)
                    data.modelOffset = reinterpret_cast<std::intptr_t>(&previous->sibling);
            }
        }
    }
    SetMT7Parents(root, 0);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawCharacterModelFrameNodes(MT7Node *node, const Matrix4x4 *parentFrame) {
    const Matrix4x4 *const frame = characterModelFrameStream;
    if (node->mesh) {
        am2LoadXmtrxPairs(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(characterModelFrameStream);
        if (!g_characterClothDrawDisabled) {
            if ((node->id == 5 || node->id == 10) && IsMT7ClothNode(node))
                ApplyMT7ClothDeformation(node);
            else if (node->parent && node->parent->mesh)
                ApplyMT7ParentLinkedVertices(node, node->parent, characterModelFrameStream, parentFrame);
        }
        g_characterTransparencyMeshCallback(node->mesh);
        ++characterModelFrameStream;
    }
    if (node->child)
        DrawCharacterModelFrameNodes(node->child, frame);
    if (node->sibling)
        DrawCharacterModelFrameNodes(node->sibling, parentFrame);
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawCharacterMatrixFrames(MT7Node *model, float alpha, const Matrix4x4 *frames) {
    matrix_stack::Matrix4x4_Push(0);
    am2StoreXmtrx(&g_characterModelMatrix);
    characterModelFrameStream = frames;
    g_characterTransparencyAlpha = alpha;
    if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1) {
        if (alpha < 1.0f)
            g_characterTransparencyMeshCallback = DrawSingleLightCharacterMeshAlpha;
        else
            g_characterTransparencyMeshCallback = dc::sdk::DrawSingleLightMT7Mesh;
    } else if (alpha < 1.0f) {
        g_characterTransparencyMeshCallback = DrawLitCharacterMeshAlpha;
    } else {
        g_characterTransparencyMeshCallback = dc::sdk::DrawLitMT7Mesh;
    }
    DrawCharacterModelFrameNodes(model, frames);
    PopCurrentMatrix(1);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Matrix4x4 *modelNodeMatrixCursor = 0;
std::int32_t modelNodeVisitIndex = 0;

} // namespace

// @verified - d
void StoreMT7NodeMatrices(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    Matrix4x4 *const output = modelNodeMatrixCursor++;
    matrix_stack::GetMatrix(output);
    if (node->child)
        StoreMT7NodeMatrices(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        StoreMT7NodeMatrices(node->sibling);
    }
}

// @verified - d
void StoreMT7ModelMatrices(const MT7Node *root, Matrix4x4 *output) {
    if (!root || !output)
        return;
    modelNodeMatrixCursor = output;
    matrix_stack::PushUnitMatrix();
    StoreMT7NodeMatrices(root);
    PopCurrentMatrix(1);
}

// @verified - d
std::int32_t FindMT7NodeIndexByIdRecursive(const MT7Node *node, std::int32_t id) {
    if (!node)
        return 0;
    ++modelNodeVisitIndex;
    if (node->id == id)
        return 1;
    if (node->child) {
        const std::int32_t found = FindMT7NodeIndexByIdRecursive(node->child, id);
        if (found)
            return found;
    }
    if (node->sibling) {
        const std::int32_t found = FindMT7NodeIndexByIdRecursive(node->sibling, id);
        if (found)
            return found;
    }
    return 0;
}

// @verified - d
std::int32_t FindMT7NodeIndexById(const MT7Node *root, std::int32_t id) {
    modelNodeVisitIndex = -1;
    FindMT7NodeIndexByIdRecursive(root, id);
    return modelNodeVisitIndex;
}

// @verified - d
Matrix4x4 *FindMT7NodeMatrixByIdRecursive(const MT7Node *node, std::int32_t id, Matrix4x4 *output) {
    if (!output)
        return 0;
    if (!node)
        return output;
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    if (node->id == id) {
        matrix_stack::GetMatrix(output);
        output = 0;
    }
    if (node->child)
        output = FindMT7NodeMatrixByIdRecursive(node->child, id, output);
    if (node->sibling) {
        PopCurrentMatrix(1);
        output = FindMT7NodeMatrixByIdRecursive(node->sibling, id, output);
    }
    return output;
}

// @verified - d
std::int32_t GetMT7NodeMatrixById(const MT7Node *root, std::int32_t id, Matrix4x4 *output) {
    if (output) {
        matrix_stack::PushUnitMatrix();
        Matrix4x4 *const remaining = FindMT7NodeMatrixByIdRecursive(root, id, output);
        PopCurrentMatrix(1);
        if (!remaining)
            return 0;
    }
    return -1;
}

// @verified - d
void GetMT7NodeWorldMatrixById(const MT7Node *root, std::int32_t id) {
    Matrix4x4 matrix;
    if (!GetMT7NodeMatrixById(root, id, &matrix))
        Matrix4x4::MultiplyWithPrePassMatrix(&matrix);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetModelMarker(const MT7Node *node) {
    const MT7NodeDataHeader *const data = reinterpret_cast<const MT7NodeDataHeader *>(node + 1);
    return data->type == MODEL_DEFORMATION_CHUNK ? reinterpret_cast<const MT7PropNode *>(node)->marker : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
MT7Node *FindModelCharacterRecursive(MT7Node *node, shen_name name) {
    if (GetModelMarker(node) == name)
        return node;
    if (node->child) {
        MT7Node *const found = FindModelCharacterRecursive(node->child, name);
        if (found)
            return found;
    }
    if (node->sibling) {
        MT7Node *const found = FindModelCharacterRecursive(node->sibling, name);
        if (found)
            return found;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RelocateMT7Nodes(std::intptr_t base, MT7Node *node);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMT7Parents(MT7Node *node, MT7Node *parent);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Node *FindMT7Node(MT7Node *node, std::int16_t id);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMT7ClothNodeKinds(MT7Node *node);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ParseFaceResource(void *face, void *base);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeFaceResource(void *face);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

// @verified - d
void RelocateMT7ClothHeader(MT7ModelChunk *chunk) {
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(chunk);
    chunk->root = reinterpret_cast<MT7Node *>(base + reinterpret_cast<std::uintptr_t>(chunk->root));
    const std::int32_t textureCount = chunk->textureCount;
    MT7TextureBinding *textures = reinterpret_cast<MT7TextureBinding *>(chunk + 1);
    std::uint32_t *header = reinterpret_cast<std::uint32_t *>(textures + textureCount) + textureCount * 2;
    const std::int32_t count = static_cast<std::int32_t>(*header++);
    std::uint32_t *entry = header;
    for (std::int32_t index = 0; index < count; ++index, ++entry) {
        if (*entry)
            *entry += static_cast<std::uint32_t>(base);
    }
}

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

// @verified - d
void LoadMT7EmbeddedTextures(std::uint32_t *chunk) {
    if (chunk[0] == SHEN_NAME4('T', 'X', 'T', '7')) {
        const std::int32_t count = chunk[2];
        const std::uint32_t *offset = chunk + 3;
        const std::uint32_t *name = offset + count;
        for (std::int32_t index = 0; index < count; ++index) {
            std::intptr_t base = reinterpret_cast<std::intptr_t>(chunk);
            if (base < 0)
                base += 3;
            base &= ~std::intptr_t(3);
            const std::byte *const image = reinterpret_cast<const std::byte *>(base + *offset++);
            NamedTextureRequest texture;
            texture.image.width = *reinterpret_cast<const std::int16_t *>(image + 0x18);
            texture.image.height = *reinterpret_cast<const std::int16_t *>(image + 0x1a);
            texture.image.type = *reinterpret_cast<const std::uint32_t *>(image + 0x14);
            texture.image.pixels = image + 0x1c;
            texture.index = LoadNamedTexture(&texture.image, name);
            name += 2;
        }
    }
}

} // namespace

void LoadMT7TexnTextures(std::uint32_t *chunk) {
    while (chunk[0] == SHEN_NAME(TEXN)) {
        const std::int32_t size = static_cast<std::int32_t>(chunk[1]);
        const std::uint32_t *const name = chunk + 2;
        const std::byte *const image = reinterpret_cast<const std::byte *>(chunk + 4);
        NamedTextureRequest texture;
        do {
            texture.image.width = *reinterpret_cast<const std::int16_t *>(image + 0x18);
            texture.image.height = *reinterpret_cast<const std::int16_t *>(image + 0x1a);
            texture.image.type = *reinterpret_cast<const std::uint32_t *>(image + 0x14);
            texture.image.pixels = image + 0x1c;
        } while (0);
        texture.index = LoadNamedTexture(&texture.image, name);
        chunk += size / 4;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BindMT7NodeTextures(MT7Node *node, MT7TextureBinding *textures) {
    if (node->mesh)
        dc::sdk::PrepareMT7Mesh(node->mesh, textures);
    if (node->child)
        BindMT7NodeTextures(node->child, textures);
    if (node->sibling)
        BindMT7NodeTextures(node->sibling, textures);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RelocateMT7Model(MT7ModelChunk *chunk) {
    const std::intptr_t base = reinterpret_cast<std::intptr_t>(chunk);
    const shen_name tag = chunk->name;
    if (tag == SHEN_NAME4('M', 'D', 'C', '7'))
        RelocateMT7ClothHeader(chunk);
    else if (tag != SHEN_NAME4('M', 'D', 'L', '7') && tag != SHEN_NAME4('M', 'D', 'O', '7') && tag != SHEN_NAME4('M', 'D', 'P', '7'))
        return;
    MT7TextureBinding *const textures = reinterpret_cast<MT7TextureBinding *>(chunk + 1);
    if (base > reinterpret_cast<std::intptr_t>(chunk->root))
        chunk->root = reinterpret_cast<MT7Node *>(reinterpret_cast<std::intptr_t>(chunk->root) + base);
    RelocateMT7Nodes(base, chunk->root);
    const std::uint32_t *name = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<MT7TextureBinding *>(chunk + 1) + chunk->textureCount);
    const std::int32_t count = chunk->textureCount;
    for (std::int32_t index = 0; index < count; ++index) {
        textures[index].pixels = 0;
        const std::int32_t texture = FindNamedTextureIndex(name[0], name[1]);
        textures[index].texture = texture;
        if (texture < 0)
            textures[index].texture = 0;
        name += 2;
    }
    BindMT7NodeTextures(chunk->root, textures);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
MT7Model *ReadMT7Model(void *source, std::int32_t mode) {
    const shen_name first = *reinterpret_cast<const shen_name *>(source);
    if (first != SHEN_NAME4('M', 'D', 'L', '7') && first != SHEN_NAME4('M', 'D', 'O', '7') && first != SHEN_NAME4('M', 'D', 'C', '7') &&
        first != SHEN_NAME4('M', 'D', 'P', '7'))
        return 0;
    MT7ModelChunk *modelChunk = 0;
    PakChunkHeader *textureChunk = 0;
    PakChunkHeader *texture = 0;
    void *face = 0;
    std::int32_t textureCount;
    void *cloth = 0;
    std::int32_t end = 0;
    textureCount = 0;
    PakChunkHeader *chunk = static_cast<PakChunkHeader *>(source);
    for (;;) {
        switch (chunk->name) {
        case SHEN_NAME4('M', 'D', 'L', '7'):
        case SHEN_NAME4('M', 'D', 'O', '7'):
        case SHEN_NAME4('M', 'D', 'C', '7'):
        case SHEN_NAME4('M', 'D', 'P', '7'):
            if (modelChunk)
                end = 1;
            else
                modelChunk = reinterpret_cast<MT7ModelChunk *>(chunk);
            break;
        case SHEN_NAME4('F', 'A', 'C', 'E'):
            if (face)
                end = 1;
            else
                face = chunk;
            break;
        case SHEN_NAME4('C', 'L', 'S', 'G'):
            if (cloth)
                end = 1;
            else
                cloth = chunk;
            break;
        case SHEN_NAME4('L', 'I', 'G', 'T'):
            break;
        case SHEN_NAME4('T', 'X', 'T', '7'):
            if (textureChunk)
                end = 1;
            else {
                texture = chunk;
                textureChunk = texture;
                textureCount = reinterpret_cast<const std::int32_t *>(texture)[2];
                LoadMT7EmbeddedTextures(reinterpret_cast<std::uint32_t *>(texture));
            }
            break;
        default:
            end = 1;
            break;
        }
        if (textureChunk || end)
            break;
        chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
    }
    if (mode && texture && HLib::GetTaskDataName(source))
        HLib::ShrinkTaskData(source, reinterpret_cast<std::byte *>(texture) - static_cast<std::byte *>(source));
    std::int32_t bytes = sizeof(MT7Model);
    if (!mode)
        bytes += textureCount * 4;
    MT7Model *model;
    if (modelChunk) {
        RelocateMT7Model(modelChunk);
        if (face && ParseFaceResource(face, source) == -1)
            InitializeFaceResource(face);
        if (cloth)
            modelChunk->root->taskData = cloth;
        if (!face && ModelHasFaceNode(modelChunk)) {
            model = static_cast<MT7Model *>(HLib::AllocateTaskData(bytes + 0x148, MODEL_INSTANCE_MEMORY));
            face = reinterpret_cast<std::byte *>(model) + bytes;
            InitializeFaceResource(face);
        } else {
            model = static_cast<MT7Model *>(HLib::AllocateTaskData(bytes, MODEL_INSTANCE_MEMORY));
        }
        SetMT7Parents(modelChunk->root, 0);
    } else {
        model = static_cast<MT7Model *>(HLib::AllocateTaskData(bytes, MODEL_INSTANCE_MEMORY));
    }
    if (model) {
        model->entityType = 0;
        model->source = source;
        model->root = modelChunk->root;
        model->retainedTextureCount = 0;
        model->face = face;
        if (reinterpret_cast<const MT7NodeDataHeader *>(model->root + 1)->type == SHEN_NAME4('M', 'D', 'C', '7'))
            SetMT7ClothNodeKinds(model->root);
        return model;
    }
    if (modelChunk->root)
        modelChunk->root->taskData = 0;
    return model;
}
#endif

} // namespace s2
} // namespace shenmue
