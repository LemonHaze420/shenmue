#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"

namespace shenmue { namespace s2 {

extern Matrix4x4 g_characterModelMatrix;
extern std::int32_t g_characterLinkedVerticesEnabled;
extern std::int32_t g_characterVertexBlendEnabled, g_characterVertexBlendMode;
extern Vector3 g_characterVertexAnchor;
extern Matrix4x4 g_characterNodeRelativeTransform, g_characterParentRelativeTransform, g_characterVertexBlendTransform;
extern const Matrix4x4 *g_characterSkinBoneSources[7];
extern Matrix4x4 g_characterSkinBoneMatrices[7];
struct MT7LinkedVertexData {
    shen_name type;
    Matrix4x4 bindTransform;
    std::int32_t linkCount;
};
struct MT7BlendedVertex {
    std::int32_t destinationOffset;
    float weight;
    Vector3 position, normal;
};
struct MT7WeightBinding {
    std::uint32_t boneId;
    Matrix4x4 transform;
};
struct MT7WeightedVertex {
    Vector3 position, normal, evaluatedPosition, evaluatedNormal;
};
struct MT7WeightGroup {
    std::uint32_t boneId;
    std::int32_t count;
};
struct MT7VertexInfluence {
    std::uint32_t vertexIndex;
    float weight;
};
struct MT7WeightedVertexMapping {
    std::uint32_t vertexIndex;
    std::int32_t destinationOffset;
};
struct MT7WeightGroupHeader {
    shen_name type;
    std::uint32_t bytes;
    std::int32_t count;
};
typedef char Layout_character_model_dc_0[(sizeof(MT7LinkedVertexData) == 0x48) ? 1 : -1];
typedef char Layout_character_model_dc_1[(offsetof(MT7LinkedVertexData, bindTransform) == 4) ? 1 : -1];
typedef char Layout_character_model_dc_2[(offsetof(MT7LinkedVertexData, linkCount) == 0x44) ? 1 : -1];
typedef char Layout_character_model_dc_3[(sizeof(MT7BlendedVertex) == 0x20) ? 1 : -1];
typedef char Layout_character_model_dc_4[(sizeof(MT7WeightBinding) == 0x44) ? 1 : -1];
typedef char Layout_character_model_dc_5[(offsetof(MT7WeightBinding, transform) == 4) ? 1 : -1];
typedef char Layout_character_model_dc_6[(sizeof(MT7WeightedVertex) == 0x30) ? 1 : -1];
typedef char Layout_character_model_dc_7[(offsetof(MT7WeightedVertex, evaluatedPosition) == 0x18) ? 1 : -1];
typedef char Layout_character_model_dc_8[(sizeof(MT7WeightGroup) == 8) ? 1 : -1];
typedef char Layout_character_model_dc_9[(sizeof(MT7VertexInfluence) == 8) ? 1 : -1];
typedef char Layout_character_model_dc_10[(sizeof(MT7WeightedVertexMapping) == 8) ? 1 : -1];
typedef char Layout_character_model_dc_11[(sizeof(MT7WeightGroupHeader) == 12) ? 1 : -1];
void BlendMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight);
void BlendNormalizedMT7VertexMatrices(const Matrix4x4 *first, const Matrix4x4 *second, Matrix4x4 *output, float weight);
void ApplyMT7BlendedVertices(const MT7Node *node, const void *table);
void PropagateCharacterLinkedVertices(const MT7Node *node, const MT7Node *parent, const Matrix4x4 *matrix, const Matrix4x4 *parentMatrix);
void PrepareCharacterModelNodes(const MT7Node *node, const Matrix4x4 *parentMatrix);
void DrawSingleCharacterModel(MT7Node *node);
void DrawCharacterModelNodes(const MT7Node *node);
void DrawAlphaCharacterModelNodes(const MT7Node *node, float alpha);

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM || SHENMUE_TARGET_S2_DC_JP_1001
void DrawKnifeModel(const MT7Node *node);
void DrawKnifeCharacter(Character *character);
#endif

} } // namespace shenmue::s2

#endif
