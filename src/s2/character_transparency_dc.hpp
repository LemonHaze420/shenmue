#pragma once

#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2


namespace shenmue { namespace s2 {

typedef void (*CharacterTransparencyNodeCallback)(const MT7Node *);
typedef void (*CharacterTransparencyMeshCallback)(void *);
static const float kCharacterVisibleAlpha = 9.999999747378752e-05f;
extern std::int32_t g_characterSearchMeshSuppressed;
extern std::uint32_t g_motionDrawOverrideFlags;
extern float g_characterTransparencyAlpha, g_searchHandBlend;
extern CharacterTransparencyMeshCallback g_characterTransparencyMeshCallback, g_characterHandMeshCallback;
void DrawCharacterAlphaNode(const MT7Node *node);
void DrawCharacterSearchNode(const MT7Node *node);
void DrawCharacterSkinnedSearchNode(const MT7Node *node);
void DrawCharacterHandHierarchy(const MT7Node *node);
void DrawCharacterTransparencyHierarchy(const MT7Node *node);
void DrawSingleLightCharacterMeshAlpha(void *mesh);
void DrawLitCharacterMeshAlpha(void *mesh);
void mtUpdateTransparencyCallback(const MT7Node *node, float alpha);
float SearchIK_GetMaxBlend();
void setSearchIKFlag(float blend);
static const CharacterTransparencyNodeCallback kCharacterTransparencyNodeCallbacks[3]= {
    DrawCharacterAlphaNode, DrawCharacterSearchNode, DrawCharacterSkinnedSearchNode};

} } // namespace shenmue::s2

#endif
