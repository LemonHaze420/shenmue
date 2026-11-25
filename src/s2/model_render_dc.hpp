#pragma once

#include "s2/model_dc.hpp"
#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace dc {
namespace sdk {

static const std::uint32_t MT7_DRAW_MIRRORED = 0x80000000U;
void SetMirroredModelDrawMode(std::int32_t enabled);
void DrawSingleLightMT7Mesh(void *mesh);
void DrawLitMT7Mesh(void *mesh);
void DrawSingleLightMT7MeshAlpha(void *mesh, float alpha);
void DrawLitMT7MeshAlpha(void *mesh, float alpha);

} // namespace sdk
} // namespace dc
} // namespace shenmue

namespace shenmue {
namespace s2 {

struct Character;
extern std::uint32_t g_characterModelDrawFlags;
extern std::int32_t g_characterClothDrawDisabled;
void ResetModelDrawState();
void UpdateMT7MeshTextureCoordinates(void *mesh);
void nlSetTexanimUV(float u, float v);
void ApplyTexAnimUVs(void *mesh);
void RecursiveApplyTexAnimUVs(MT7Node *node);
void UpdateModelTextureCoordinates(MT7ModelChunk *model);
void DrawImageModelSelection(Character *character, std::uint32_t flags);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawCharacterModelBranch(MT7Node *node);
#else
void DrawCharacterModelBranch(MT7Node *node, float alpha);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawMT7ProceduralBranch(MT7Node *node, std::int16_t stop, float alpha);
#endif
bool IsMT7ClothNode(const MT7Node *node);
void ApplyMT7ParentLinkedVertices(const MT7Node *node, const MT7Node *parent, const Matrix4x4 *matrix, const Matrix4x4 *parentMatrix);
void ApplyMT7ClothDeformation(const MT7Node *node);

} // namespace s2
} // namespace shenmue

#endif
