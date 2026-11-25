#pragma once

#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define MEMORY_MODEL_CLONE SHEN_NAME4('H', 'M', 'D', 'L')
#define MODEL_CLOTH_CHUNK SHEN_NAME4('M', 'D', 'C', '7')
#define MODEL_HAND_CHUNK SHEN_NAME4('M', 'D', 'H', '7')
#define MODEL_WEIGHT_CHUNK SHEN_NAME4('W', 'B', 'T', 'R')
struct MT7ClothNodeData {
    MT7NodeDataHeader header;
    std::byte retainedState[0x3c];
    std::uint32_t linkCount;
};
typedef char Layout_model_clone_dc_0[(sizeof(MT7ClothNodeData) == 0x48) ? 1 : -1];
typedef char Layout_model_clone_dc_1[(offsetof(MT7ClothNodeData, linkCount) == 0x44) ? 1 : -1];

extern std::uint32_t g_modelCloneNodeCount, g_modelCloneMdcBytes, g_modelCloneMdpBytes;
void CountModelNodeTreeForClone(const MT7Node *node);
std::uint32_t GetModelNodeCountForClone(const MT7Node *node);
std::uint32_t GetModelMdcCloneBlockSize(const MT7Node *node);
void AccumulateModelMdcCloneBytes(const MT7Node *node);
void AccumulateModelMdpCloneBytes(const MT7Node *node);
std::uint32_t GetModelMdcCloneBytes(const MT7Node *node);
std::uint32_t GetModelMdpCloneBytes(const MT7Node *node);
std::uint32_t GetModelMdpCloneBlockSize(const MT7Node *node);
std::uint32_t MeasureModelNodeTreeClone(const MT7Node *node);
std::byte *CopyModelNodeTree(MT7Node *parent, const MT7Node *source, std::byte *destination);
void CopyModelNodeTreeClone(const MT7Node *source, MT7Node *destination);
MT7Node *CloneModelNodeTree(const MT7Node *node);
MT7Node *CloneMT7ModelRoot(MT7Model *model);
MT7Node *FindImageGroupNode(MT7Node *node, std::int32_t group, std::int32_t variant);
MT7Node *CloneImageSelection(MT7Model *source, std::uint32_t child, std::int32_t group, std::uint32_t variant);
void ReleaseMT7ModelNodeTreeClone(MT7Node *root);
struct MemoTextureName;
MT7Model *ReadMT7ModelTextureNames(void *source, MemoTextureName *names, std::int32_t *textures);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RebindMT7ModelTextures(MT7Model *model, const MemoTextureName *names, std::int32_t count, const std::int32_t *originalTextures);
#else
void RebindMT7ModelTextures(MT7Model *model, const MemoTextureName *names, std::int32_t count);
#endif
std::int32_t SetMT7NodeTransformOverrideById(MT7Node *node, std::int16_t id, const Matrix4x4 *transform);
void DrawModelNodeTree(const MT7Node *node);
void DrawCharacterModelTree(const MT7Node *node);
void DrawModelNodeTreePushed(MT7Node *node);
void DrawCharacterModelNodeById(MT7Node *root, std::int16_t id);
void RebaseMT7Model(MT7Node *previous, MT7Node *current);
void SetGeometryLightTreeTarget(const Matrix4x4 *transform);
std::int32_t ModelHasFaceNode(const MT7ModelChunk *chunk);
void StoreMT7ModelMatrices(const MT7Node *root, Matrix4x4 *output);
std::int32_t FindMT7NodeIndexById(const MT7Node *root, std::int32_t id);
std::int32_t GetMT7NodeMatrixById(const MT7Node *root, std::int32_t id, Matrix4x4 *output);
void GetMT7NodeWorldMatrixById(const MT7Node *root, std::int32_t id);

} // namespace s2
} // namespace shenmue

#endif
