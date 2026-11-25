#include "s2/character_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_clone_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cstring>
#include "s2/character_transparency_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "platform/dc/mt7.hpp"

namespace shenmue {
namespace s2 {

void ApplyMT7BlendedVertices(const MT7Node *node, const void *table);
void BlendMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight);
void BlendNormalizedMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight);

std::int32_t g_characterVertexBlendEnabled = 1;
std::int32_t g_characterVertexBlendMode = 2;
Vector3 g_characterVertexAnchor = {};
__attribute__((aligned(32))) Matrix4x4 g_characterNodeRelativeTransform, g_characterParentRelativeTransform, g_characterVertexBlendTransform;
const Matrix4x4 *g_characterSkinBoneSources[7] = {};
__attribute__((aligned(32))) Matrix4x4 g_characterSkinBoneMatrices[7];

void PropagateCharacterLinkedVertices(const MT7Node *node, const MT7Node *parent, const Matrix4x4 *matrix, const Matrix4x4 *parentMatrix) {
    const MT7NodeDeformationData *const deformation = reinterpret_cast<const MT7NodeDeformationData *>(node + 1);
    const std::uint32_t chunkSize = deformation->nodeChunkSize;
    const shen_name type = deformation->nodeChunk;
    const std::byte *bytes = reinterpret_cast<const std::byte *>(node);
    if (type != MODEL_DEFORMATION_CHUNK)
        return;
    const shen_name *linksChunk = reinterpret_cast<const shen_name *>(bytes + sizeof(MT7Node) + offsetof(MT7NodeDeformationData, linksChunk));
    if (chunkSize <= 20)
        return;
    const std::int32_t *linkCount = reinterpret_cast<const std::int32_t *>(bytes + sizeof(MT7Node) + offsetof(MT7NodeDeformationData, linkCount));
    if (*linksChunk == type && g_characterLinkedVerticesEnabled && node && parent && node->mesh && parent->mesh && matrix && parentMatrix) {
        const std::int32_t count = *linkCount;
        const MT7VertexLink *link = reinterpret_cast<const MT7VertexLink *>(deformation + 1);
        if (count <= 0)
            return;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(matrix);
        Matrix4x4::NormalizeAndSetTransformMatrix();
        Matrix4x4::MultiplyWithPrePassMatrix(parentMatrix);
        const std::byte *const sourceMesh = reinterpret_cast<const std::byte *>(parent->mesh);
        std::byte *const destinationMesh = static_cast<std::byte *>(node->mesh);
        for (std::int32_t index = 0; index < count; ++index, ++link) {
            Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(sourceMesh + link->sourceOffset / 4 * 4),
                *reinterpret_cast<Vector3 *>(destinationMesh + link->destinationOffset / 4 * 4));
            Matrix4x4::Transform(*reinterpret_cast<const Vector3 *>(sourceMesh + link->sourceOffset / 4 * 4 + sizeof(Vector3)),
                *reinterpret_cast<Vector3 *>(destinationMesh + link->destinationOffset / 4 * 4 + sizeof(Vector3)));
            *reinterpret_cast<std::uint32_t *>(destinationMesh + link->destinationOffset / 4 * 4) |= 1U;
        }
        PopCurrentMatrix(1);
    }
}

void ApplyMT7ParentLinkedVertices(const MT7Node *node, const MT7Node *parent, const Matrix4x4 *matrix, const Matrix4x4 *parentMatrix) {
    std::int32_t index, count;
    const std::int32_t *header;
    const MT7VertexLink *link;
    const std::byte *sourceMesh;
    std::byte *destinationMesh;
    const Vector3 *source;
    Vector3 *destination;
    std::uint32_t offset;
    if (!g_characterLinkedVerticesEnabled)
        return;
    if (node->id == 14) {
        g_characterVertexAnchor.x = matrix->rows[3].x;
        g_characterVertexAnchor.y = matrix->rows[3].y;
        g_characterVertexAnchor.z = matrix->rows[3].z;
    }
    if (reinterpret_cast<const MT7LinkedVertexData *>(node + 1)->type != MODEL_CLOTH_CHUNK)
        return;
    header = &reinterpret_cast<const MT7LinkedVertexData *>(node + 1)->linkCount;
    count = *header;
    link = reinterpret_cast<const MT7VertexLink *>(header + 1);
    if (count <= 0)
        return;
    for (index = 0; index < 12; ++index)
        g_characterNodeRelativeTransform.rows[0].data()[index] = matrix->rows[0].data()[index];
    g_characterNodeRelativeTransform.rows[3].x = matrix->rows[3].x - g_characterVertexAnchor.x;
    g_characterNodeRelativeTransform.rows[3].y = matrix->rows[3].y - g_characterVertexAnchor.y;
    g_characterNodeRelativeTransform.rows[3].z = matrix->rows[3].z - g_characterVertexAnchor.z;
    g_characterNodeRelativeTransform.rows[3].w = matrix->rows[3].w;
    for (index = 0; index < 12; ++index)
        g_characterParentRelativeTransform.rows[0].data()[index] = parentMatrix->rows[0].data()[index];
    g_characterParentRelativeTransform.rows[3].x = parentMatrix->rows[3].x - g_characterVertexAnchor.x;
    g_characterParentRelativeTransform.rows[3].y = parentMatrix->rows[3].y - g_characterVertexAnchor.y;
    g_characterParentRelativeTransform.rows[3].z = parentMatrix->rows[3].z - g_characterVertexAnchor.z;
    g_characterParentRelativeTransform.rows[3].w = parentMatrix->rows[3].w;
    matrix_stack::Matrix4x4_Push(&g_characterNodeRelativeTransform);
    Matrix4x4::NormalizeAndSetTransformMatrix();
    Matrix4x4::MultiplyWithPrePassMatrix(&g_characterParentRelativeTransform);
    destinationMesh = static_cast<std::byte *>(node->mesh);
    sourceMesh = reinterpret_cast<const std::byte *>(parent->mesh);
    for (index = 0; index < count; ++index, ++link) {
        source = reinterpret_cast<const Vector3 *>(sourceMesh + link->sourceOffset / 4 * 4);
        offset = link->destinationOffset / 4 * 4;
        destination = reinterpret_cast<Vector3 *>(destinationMesh + offset);
        Matrix4x4::TransformPoint(source[0], destination[0]);
        Matrix4x4::Transform(source[1], destination[1]);
        *reinterpret_cast<std::uint32_t *>(destinationMesh + offset) |= 1U;
    }
    if (g_characterVertexBlendEnabled && *reinterpret_cast<const std::uint32_t *>(link))
        ApplyMT7BlendedVertices(node, link);
    PopCurrentMatrix(1);
}

// @verified - d
void ApplyMT7BlendedVertices(const MT7Node *node, const void *table) {
    const std::int32_t *cursor = static_cast<const std::int32_t *>(table);
    const std::int32_t count = *cursor++;
    std::int32_t index, offset;
    std::uint32_t *mesh;
    float weight;
    am2StoreXmtrx(&g_characterNodeRelativeTransform);
    const Matrix4x4 *const bind = &reinterpret_cast<const MT7LinkedVertexData *>(node + 1)->bindTransform;
    if (node->id == 7 || node->id == 12)
        g_characterVertexBlendMode = 3;
    else
        g_characterVertexBlendMode = 2;
    for (index = 0; index < count; ++index, cursor += 6) {
        mesh = static_cast<std::uint32_t *>(node->mesh);
        offset = *cursor++;
        offset /= 4;
        weight = *reinterpret_cast<const float *>(cursor++);
        if (!(weight < 99.99f))
            continue;
        switch (g_characterVertexBlendMode) {
        case 0:
            am2LoadXmtrxPairs(&g_characterNodeRelativeTransform);
            break;
        case 2:
            BlendMT7VertexMatrices(&g_characterNodeRelativeTransform, bind, &g_characterVertexBlendTransform, weight);
            am2LoadXmtrxPairs(&g_characterVertexBlendTransform);
            break;
        case 3:
            BlendNormalizedMT7VertexMatrices(&g_characterNodeRelativeTransform, bind, &g_characterVertexBlendTransform, weight);
            am2LoadXmtrxPairs(&g_characterVertexBlendTransform);
            break;
        case 1:
        default:
            return;
        }
        Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(cursor), *reinterpret_cast<Vector3 *>(mesh + offset));
        Matrix4x4::Transform(*reinterpret_cast<const Vector3 *>(cursor + 3), *reinterpret_cast<Vector3 *>(mesh + offset + 3));
        am2Vector3NormalizeInPlace(reinterpret_cast<Vector3 *>(mesh + offset + 3));
        mesh[offset] |= 1U;
    }
}

// @verified - d
void BlendMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight) {
    for (std::int32_t index = 0; index < 16; ++index)
        output->rows[0].data()[index] = (1.0f - weight) * first->rows[0].data()[index] + second->rows[0].data()[index] * weight;
}

// @verified - d
void BlendNormalizedMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight) {
    for (std::int32_t index = 0; index < 16; ++index)
        output->rows[0].data()[index] = (1.0f - weight) * first->rows[0].data()[index] + second->rows[0].data()[index] * weight;
    am2Vector3NormalizeInPlace(reinterpret_cast<Vector3 *>(&output->rows[0]));
    am2Vector3NormalizeInPlace(reinterpret_cast<Vector3 *>(&output->rows[1]));
    am2Vector3NormalizeInPlace(reinterpret_cast<Vector3 *>(&output->rows[2]));
}

void ApplyMT7ClothDeformation(const MT7Node *node) {
    std::uint32_t *cursor = reinterpret_cast<std::uint32_t *>(const_cast<MT7Node *>(node + 1));
    const MT7VertexLink *links = reinterpret_cast<const MT7VertexLink *>(cursor);
    std::int32_t linkCount = 0;
    std::int32_t bindingCount, vertexCount, groupCount, mappingCount, skip;
    std::int32_t i, j, k;
    const MT7WeightBinding *bindings, *binding;
    MT7WeightedVertex *vertices;
    const MT7WeightGroup *groups;
    const MT7WeightedVertexMapping *mappings;
    const MT7WeightGroup *group;
    const MT7WeightedVertexMapping *mapping;
    MT7WeightedVertex *vertex;
    const MT7VertexInfluence *influence, *influences;
    std::uint32_t boneId, groupBytes, offset;
    const Matrix4x4 *base, *source, *transform;
    Matrix4x4 *matrix;
    std::byte *mesh, *destinationMesh;
    const std::byte *sourceMesh;
    std::uint32_t *words;
    const std::uint32_t *sourceWords;
    const Vector3 *sourceVector;
    Vector3 *destinationVector;
    Vector3 position, normal;
    if (*cursor == MODEL_CLOTH_CHUNK) {
        cursor = reinterpret_cast<std::uint32_t *>(const_cast<MT7Node *>(node));
        cursor += offsetof(MT7LinkedVertexData, linkCount) / sizeof(std::uint32_t);
        cursor += sizeof(MT7Node) / sizeof(std::uint32_t);
        linkCount = static_cast<std::int32_t>(*cursor++);
        links = reinterpret_cast<const MT7VertexLink *>(cursor);
        if (!linkCount)
            ++cursor;
        else
            cursor += linkCount * static_cast<std::ptrdiff_t>(sizeof(MT7VertexLink) / sizeof(std::uint32_t));
    }
    if (*cursor != MODEL_WEIGHT_CHUNK)
        return;
    cursor++;
    bindingCount = *cursor++;
    skip = bindingCount * 17;
    bindings = reinterpret_cast<const MT7WeightBinding *>(cursor);
    cursor += skip;
    cursor++;
    vertexCount = *cursor++;
    vertices = reinterpret_cast<MT7WeightedVertex *>(cursor);
    cursor = reinterpret_cast<std::uint32_t *>(vertices + vertexCount);
    cursor++;
    groupBytes = *cursor++;
    groupCount = *cursor++;
    groups = reinterpret_cast<const MT7WeightGroup *>(cursor);
    cursor += groupBytes / 4;
    cursor++;
    mappingCount = *cursor++;
    mappings = reinterpret_cast<const MT7WeightedVertexMapping *>(cursor);
    matrix_stack::Matrix4x4_Push(0);
    std::uint32_t *values = reinterpret_cast<std::uint32_t *>(&vertices->evaluatedPosition);
    for (i = 0; i < vertexCount; ++i, values += 12) {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
        values[3] = 0;
        values[4] = 0;
        values[5] = 0;
    }
    base = node->id == 5 ? g_characterSkinBoneSources[2] : g_characterSkinBoneSources[4];
    binding = bindings;
    for (j = 0; j < bindingCount; ++j) {
        boneId = binding->boneId;
        transform = &binding->transform;
        ++binding;
        source = g_characterSkinBoneSources[boneId - 1];
        matrix = &g_characterSkinBoneMatrices[boneId - 1];
        am2LoadXmtrxPairs(base);
        Matrix4x4::NormalizeAndSetTransformMatrix();
        dc::sdk::MultiplyMT7ProjectionRows(source);
        Matrix4x4::MultiplyWithPrePassMatrix(transform);
        am2StoreXmtrx(matrix);
    }
    group = groups;
    for (i = 0; i < groupCount; ++i) {
        am2LoadXmtrxPairs(&g_characterSkinBoneMatrices[group->boneId - 1]);
        influence = reinterpret_cast<const MT7VertexInfluence *>(group + 1);
        for (j = 0; j < group->count; ++j, ++influence) {
            vertex = &vertices[influence->vertexIndex];
            Matrix4x4::TransformPoint(vertex->position, position);
            Matrix4x4::Transform(vertex->normal, normal);
            am2Vector3Scale(&position, &position, influence->weight);
            am2Vector3Scale(&normal, &normal, influence->weight);
            am2Vector3AddInPlace(&vertex->evaluatedPosition, &position);
            am2Vector3AddInPlace(&vertex->evaluatedNormal, &normal);
        }
        influences = reinterpret_cast<const MT7VertexInfluence *>(group + 1);
        group = reinterpret_cast<const MT7WeightGroup *>(influences + group->count);
    }
    vertex = vertices;
    for (j = 0; j < vertexCount; ++j, ++vertex) {
        *reinterpret_cast<std::uint32_t *>(&vertex->evaluatedPosition.x) |= 1U;
        am2Vector3NormalizeInPlace(&vertex->evaluatedNormal);
    }
    mapping = mappings;
    mesh = static_cast<std::byte *>(node->mesh);
    for (k = 0; k < mappingCount; ++k, ++mapping) {
        words = reinterpret_cast<std::uint32_t *>(mesh + mapping->destinationOffset);
        sourceWords = reinterpret_cast<const std::uint32_t *>(&vertices[mapping->vertexIndex].evaluatedPosition);
        words[0] = sourceWords[0];
        words[1] = sourceWords[1];
        words[2] = sourceWords[2];
        words[3] = sourceWords[3];
        words[4] = sourceWords[4];
        words[5] = sourceWords[5];
    }
    PopCurrentMatrix(1);
    if (!node->transformOverride || !node->parent || !node->parent->transformOverride || !node->parent->mesh || linkCount <= 0)
        return;
    for (i = 0; i < 12; ++i)
        g_characterNodeRelativeTransform.rows[0].data()[i] = node->transformOverride->rows[0].data()[i];
    g_characterNodeRelativeTransform.rows[3].x = node->transformOverride->rows[3].x - g_characterVertexAnchor.x;
    g_characterNodeRelativeTransform.rows[3].y = node->transformOverride->rows[3].y - g_characterVertexAnchor.y;
    g_characterNodeRelativeTransform.rows[3].z = node->transformOverride->rows[3].z - g_characterVertexAnchor.z;
    g_characterNodeRelativeTransform.rows[3].w = node->transformOverride->rows[3].w;
    for (i = 0; i < 12; ++i)
        g_characterParentRelativeTransform.rows[0].data()[i] = node->parent->transformOverride->rows[0].data()[i];
    g_characterParentRelativeTransform.rows[3].x = node->parent->transformOverride->rows[3].x - g_characterVertexAnchor.x;
    g_characterParentRelativeTransform.rows[3].y = node->parent->transformOverride->rows[3].y - g_characterVertexAnchor.y;
    g_characterParentRelativeTransform.rows[3].z = node->parent->transformOverride->rows[3].z - g_characterVertexAnchor.z;
    g_characterParentRelativeTransform.rows[3].w = node->parent->transformOverride->rows[3].w;
    matrix_stack::Matrix4x4_Push(&g_characterNodeRelativeTransform);
    Matrix4x4::NormalizeAndSetTransformMatrix();
    Matrix4x4::MultiplyWithPrePassMatrix(&g_characterParentRelativeTransform);
    destinationMesh = static_cast<std::byte *>(node->mesh);
    sourceMesh = reinterpret_cast<const std::byte *>(node->parent->mesh);
    for (i = 0; i < linkCount; ++i, ++links) {
        sourceVector = reinterpret_cast<const Vector3 *>(sourceMesh + links->sourceOffset / 4 * 4);
        offset = links->destinationOffset / 4 * 4;
        destinationVector = reinterpret_cast<Vector3 *>(destinationMesh + offset);
        Matrix4x4::TransformPoint(sourceVector[0], destinationVector[0]);
        Matrix4x4::Transform(sourceVector[1], destinationVector[1]);
        *reinterpret_cast<std::uint32_t *>(destinationMesh + offset) |= 1U;
    }
    PopCurrentMatrix(1);
}

bool IsMT7ClothNode(const MT7Node *node) {
    if (reinterpret_cast<const MT7ClothNodeData *>(node + 1)->header.type != MODEL_CLOTH_CHUNK)
        return false;
    const std::uint32_t *cursor = reinterpret_cast<const std::uint32_t *>(node);
    cursor += offsetof(MT7ClothNodeData, linkCount) / sizeof(std::uint32_t);
    cursor += sizeof(MT7Node) / sizeof(std::uint32_t);
    const std::uint32_t count = *cursor++;
    if (!count)
        cursor += 1;
    else
        cursor += count * 2U;
    if (*cursor == MODEL_WEIGHT_CHUNK)
        return true;
    return false;
}

} // namespace s2
} // namespace shenmue

#endif
