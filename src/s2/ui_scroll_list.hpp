#pragma once

#include "d3t/ui_runtime.hpp"

namespace shenmue::s2 {

inline constexpr float UI_IMAGE_CORNER_ANGLE = 1.5707951f;

struct ScrollListScrollData {
    std::int32_t frame;
    std::int32_t unknown04;
    const char *text;
};
static_assert(sizeof(ScrollListScrollData) == 16 && offsetof(ScrollListScrollData, text) == 8);
extern std::array<ScrollListScrollData, 12> g_scrollListScrollData;
ScrollListScrollData GetScrollListScrollData(std::int32_t index);
d3t::UISpriteSheet *UI_GetSpriteSheet(std::uint32_t index);
void UI_DrawImageCorners(nk_context *context, const UILayoutRect &bounds, std::int32_t index, std::uint32_t colour);
void UI_DrawScrollListScrollRow(nk_context &context, std::uint32_t index, std::int32_t row, UILayoutRect &bounds);
struct PawnFlyerSprite {
    std::int32_t unknown00;
    std::int32_t image;
    float u0, v0, u1, v1;
    std::int32_t unknown18;
};
static_assert(sizeof(PawnFlyerSprite) == 28 && offsetof(PawnFlyerSprite, image) == 4 && offsetof(PawnFlyerSprite, u0) == 8);
extern std::array<std::array<PawnFlyerSprite, 64>, 12> g_pawnFlyerSprites;
extern UIScrollList *g_pawnFlyerScrollList;
extern UITexturePage *g_pawnFlyerTexturePage;
PawnFlyerSprite *GetPawnFlyerSprite(std::int32_t row, std::int32_t index);
void RebuildPawnFlyerScrollRows(std::int32_t inspection);
float UI_DrawScrollListPawnshopRow(nk_context &context, std::uint32_t index, std::int32_t row, float y, float &scroll, std::int32_t drawRow);
struct ScrollListMoveData {
    std::int32_t proficiency;
    std::int32_t known;
    const char *text;
    const std::uint8_t *commands;
    std::int32_t category;
    std::int32_t groupCount;
    std::int32_t critical;
    std::int32_t unknown24;
};
static_assert(sizeof(ScrollListMoveData) == 40 && offsetof(ScrollListMoveData, commands) == 16 && offsetof(ScrollListMoveData, critical) == 32);
extern std::array<ScrollListMoveData, 12> g_scrollListMoveData;
ScrollListMoveData GetScrollListMoveData(std::int32_t index);
void FillScrollListMoveData(std::int32_t ordinal, ScrollListMoveData &data);
std::int32_t GetMoveCommandSpriteFrame(std::int32_t command);
std::int32_t GetMoveCommandAction(std::int32_t command);
const d3t::UIViewImage *GetPlatformButtonIconImage(std::int32_t mode);
std::int32_t GetPlatformButtonIconSheet();
void UI_DrawScrollListMoveRow(nk_context &context, std::uint32_t index, std::int32_t row, UILayoutRect &bounds);
extern float g_scrollListPawnshopOffset;
UIStateStyle *UI_GetStateStyle(std::int32_t index);
void UI_SetFontSize(std::uint32_t size);
void UI_DrawScrollList(nk_context &context, std::uint32_t index);
enum class UIScrollListMethod : std::int32_t {
    Move = 1,
    Scroll = 2,
    Save = 3,
    Map = 4,
    Pawnshop = 5,
    Standard = 6,
};

d3t::UIViewSlot<kUIViewImageCapacity> *UI_GetViewSlot(std::uint32_t index);
UIScrollList *UI_GetScrollList(std::uint32_t index);
struct nk_image UI_GetDefaultImage();
void UI_DrawPreparedImage(nk_context &context, const std::int32_t *ready, const void *image, std::uint32_t colour);
void UI_DrawViewImage(nk_context &context, std::int32_t index, std::uint32_t colour);
void UI_DrawScrollListSaveRow(nk_context &context, std::uint32_t index, std::int32_t row, float height);
void UI_DrawScrollListStandardRow(nk_context &context, std::uint32_t index, std::int32_t row);
void UI_DrawScrollListMapRow(nk_context &context, std::uint32_t index, std::int32_t row);

} // namespace shenmue::s2
