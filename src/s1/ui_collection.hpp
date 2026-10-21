#pragma once

#include "s1/item_catalog.hpp"
#include "s1/screen_space.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class CollectionGridPhase : std::int8_t {
    Inactive = 0,
    OpenInit = 1,
    SlidingIn = 2,
    Active = 3,
    WaitingPreviewClose = 4,
    Closing = 6,
    PageSlideOut = 7,
    PageSlideIn = 8,
};

struct CollectionGridFrame {
    float depth;
    float x;
    float y;
    std::int8_t selectedRow;
    std::int8_t selectedColumn;
    std::int8_t columns;
    std::int8_t rows;
    std::int16_t labelSurface;
    std::uint16_t selectedStackCount;
};
static_assert(sizeof(CollectionGridFrame) == 20 && offsetof(CollectionGridFrame, selectedRow) == 12 && offsetof(CollectionGridFrame, labelSurface) == 16 &&
    offsetof(CollectionGridFrame, selectedStackCount) == 18);

struct CollectionGridOperations {
    void (*drawPage)();
    void (*initPage)();
    bool (*handleInput)(std::uint16_t);
    void (*refreshSelectionLabel)();
    std::int16_t (*getPreviewId)();
};
static_assert(sizeof(CollectionGridOperations) == 40 && offsetof(CollectionGridOperations, getPreviewId) == 32);

inline constexpr std::array<std::int8_t, 2> kCollectionHelpTokens{61, 62};
inline constexpr std::array<std::int16_t, 4> kCollectionBorderSurfaces{253, 254, 255, 256};
inline constexpr std::array<float, 4> kCollectionPanelMotion{-672.0f, 214.0f, 1.0f, 0.0f};
inline constexpr std::array<float, 4> kCollectionLabelMotion{-672.0f, 55.0f, 1.0f, 1.0f};

extern CollectionGridPhase g_collectionGridPhase;
extern const CollectionGridOperations *g_collectionGridOperations;
extern const CollectionGridOperations g_collectionOwnedOperations;
extern const CollectionGridOperations g_collectionCassetteOperations;
extern CollectionGridFrame g_collectionGridFrame;
extern std::array<float, 4> g_collectionGridPanelMotion;
extern float g_collectionGridDepth;
extern float g_collectionGridTargetX;
extern std::uint16_t g_collectionGridPanelValue;
extern std::int8_t g_collectionGridMode;
extern std::int8_t g_collectionSavedHelpToken;
extern std::uint8_t g_collectionSavedButtonPrompt;
extern std::int16_t g_collectionPageEntryCount;
extern std::int16_t g_collectionOwnedEntryCount;
extern std::int8_t g_collectionOwnedPage;
extern std::int8_t g_collectionOwnedPageCount;
extern const std::uint8_t *g_collectionOwnedCounts;
extern const OwnedItemCatalogEntry *g_collectionOwnedCatalog;
extern std::int16_t g_collectionOwnedBaseCatalogId;
extern std::uint32_t g_collectionGridDirty;
extern std::uint32_t g_collectionCursorPulse;
extern std::uint8_t g_collectionCursorPulseRising;
extern ScreenSpaceParams g_collectionCursorSprite;
extern ScreenSpaceParams g_collectionLabelSprite;
extern ScreenSpaceParams g_collectionInfoSprite;
extern ScreenSpaceParams g_collectionCassetteSprite;
extern float g_itemInfoPopupDepth;
extern std::uint16_t g_itemInfoPopupParameter;
extern std::array<ScreenSpaceParams, 4> g_collectionBorderSprites;
extern OriginalStrip g_collectionTileStrip;
extern OriginalStrip g_collectionEdgeStrip;

void ICIS_Init();
void CollectionGrid_Reset();
bool CollectionGrid_Tick();
void CollectionGrid_HandleInput();
void CollectionGrid_Draw();
void CollectionGrid_Close();
bool CollectionGrid_MoveCursor(std::uint16_t input);
void CollectionGrid_RefreshOwnedLabel();
void CollectionGrid_RefreshCassetteLabel();
void CollectionGrid_InitOwnedPage();
void CollectionGrid_InitCassettePage();
bool CollectionGrid_HandleOwnedPageInput(std::uint16_t input);
void CollectionGrid_DrawOwnedPage();
void CollectionGrid_DrawCassettePage();
std::int16_t CollectionGrid_GetOwnedPreviewId();
std::int16_t CollectionGrid_GetCassettePreviewId();
void CollectionGrid_SetOwnedPage(std::int32_t page);
void CollectionGrid_DrawFrame();
void CollectionGrid_RefreshLabelSurface(std::int16_t &surface, const char *text);
void CollectionGrid_DrawEdges(float x, float y, std::int32_t rows, std::int32_t columns);
void CollectionGrid_DrawTiles(float x, float y, std::int32_t columns, std::int32_t rows);
void CollectionGrid_DrawFrameFade(float x, float y, float width, float height, float depth);
void CollectionGrid_DrawOwnedIcons(const std::uint8_t *counts, std::int32_t columns, const OwnedItemCatalogEntry *catalog, std::int32_t count, float x, float y,
    float depth, std::int32_t maxEntries);
void CollectionGrid_DrawStackCount(float x, float y, std::uint16_t count);
bool CollectionGrid_UpdateOpeningSlide(std::array<float, 4> &motion, float target);
bool CollectionGrid_UpdateClosingSlide(std::array<float, 4> &motion, float target);

bool Load3DCassetteModelForInventory(std::int16_t id, bool cassette);

} // namespace shenmue::s1
