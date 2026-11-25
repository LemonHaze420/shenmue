#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hlib.hpp"
#include "s2/model_dc.hpp"
#include "s2/texture_dc.hpp"
namespace shenmue {
namespace s2 {
struct MemoTexture {
    TextureChunk header;
    std::uint16_t pixels[256 * 128];
};
struct MemoTextureTaskData {
    MemoTexture textures[2];
    std::uint32_t terminator;
};
struct MemoTextureName {
    std::int32_t texture;
    std::uint32_t nameLow, nameHigh;
};
struct MemoCameraSave {
    std::uint8_t selectedHelp, retained01;
    std::uint16_t visibleHelp;
    std::int32_t playerDisplay, playerVisibility, state, perspective;
    Vector3 position, target;
};
struct MemoModelDrawContext {
    MT7Node *root;
    Vector3 position;
    Vector3i rotation;
    float scale;
    Vector4 colour;
    std::int32_t style;
};
typedef char MemoTextureSizeCheck[sizeof(MemoTexture) == 0x1002c ? 1 : -1];
typedef char MemoTextureTaskSizeCheck[sizeof(MemoTextureTaskData) == 0x2005c ? 1 : -1];
typedef char MemoCameraSizeCheck[sizeof(MemoCameraSave) == 0x2c ? 1 : -1];
typedef char MemoModelDrawContextSizeCheck[sizeof(MemoModelDrawContext) == 0x34 ? 1 : -1];
void MM_MemoCtrl(std::int8_t queue);
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
void MEMO_Callback(void *data);
void MEMO_RefreshControllerInput();
std::int32_t CancelSelectorMemo(std::int32_t force);
void CloseActiveMemo();
void OpenMemoFromNotebookMenu();
void RequestMemoClose();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RenderMemoPageText(std::int32_t page, MemoTexture **textures);
#else
void RenderMemoPageText(std::int16_t page, MemoTexture **textures);
#endif
void DrawMemoModelContext(void *context);
} // namespace s2
} // namespace shenmue
#endif
