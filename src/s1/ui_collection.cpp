#include "s1/ui_collection.hpp"
#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
std::int32_t GetIsSame(std::int32_t lhs, std::int32_t rhs);
std::int32_t SelectListEndpointOrMiddleSpriteSlot(std::int32_t index, std::int32_t lastIndex);
void DrawCashSelectorDigits(float x, float y, float scale, float depth, std::int32_t value, std::uint32_t flags, std::int32_t screenMode);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

CollectionGridPhase g_collectionGridPhase{};
const CollectionGridOperations *g_collectionGridOperations{};
const CollectionGridOperations g_collectionOwnedOperations{.drawPage = CollectionGrid_DrawOwnedPage,
    .initPage = CollectionGrid_InitOwnedPage,
    .handleInput = CollectionGrid_HandleOwnedPageInput,
    .refreshSelectionLabel = CollectionGrid_RefreshOwnedLabel,
    .getPreviewId = CollectionGrid_GetOwnedPreviewId};
const CollectionGridOperations g_collectionCassetteOperations{.drawPage = CollectionGrid_DrawCassettePage,
    .initPage = CollectionGrid_InitCassettePage,
    .handleInput = nullptr,
    .refreshSelectionLabel = CollectionGrid_RefreshCassetteLabel,
    .getPreviewId = CollectionGrid_GetCassettePreviewId};
CollectionGridFrame g_collectionGridFrame{};
std::array<float, 4> g_collectionGridPanelMotion{};
float g_collectionGridDepth{};
float g_collectionGridTargetX{};
std::uint16_t g_collectionGridPanelValue{};
std::int8_t g_collectionGridMode{};
std::int8_t g_collectionSavedHelpToken{};
std::uint8_t g_collectionSavedButtonPrompt{};
std::int16_t g_collectionPageEntryCount{};
std::int16_t g_collectionOwnedEntryCount{};
std::int8_t g_collectionOwnedPage{};
std::int8_t g_collectionOwnedPageCount{};
const std::uint8_t *g_collectionOwnedCounts{};
const OwnedItemCatalogEntry *g_collectionOwnedCatalog{};
std::int16_t g_collectionOwnedBaseCatalogId{};
std::uint32_t g_collectionGridDirty{};
std::uint32_t g_collectionCursorPulse{};
std::uint8_t g_collectionCursorPulseRising{};
ScreenSpaceParams g_collectionCursorSprite{};
ScreenSpaceParams g_collectionLabelSprite{};
ScreenSpaceParams g_collectionInfoSprite{};
ScreenSpaceParams g_collectionCassetteSprite{};
float g_itemInfoPopupDepth{};
std::uint16_t g_itemInfoPopupParameter{};
std::array<ScreenSpaceParams, 4> g_collectionBorderSprites{};
OriginalStrip g_collectionTileStrip{};
OriginalStrip g_collectionEdgeStrip{};

// @verified - p
void ICIS_Init() {
    g_iconPresentation.itemInfoSurface = -1;
    g_iconPresentation.itemInfoPopupPosition = {-672.0f, 55.0f, 1.0f, 0.0f};
    g_collectionCursorSprite.depthOrScale = 4.0f;
    g_collectionCursorSprite.scaledHeight = 38;
    g_collectionLabelSprite.pvrSurfaceIndex = -1;
    g_collectionLabelSprite.halfScreenX = kCollectionLabelMotion[0];
    g_collectionLabelSprite.halfScreenY = kCollectionLabelMotion[1];
    g_collectionLabelSprite.positionScaleX = kCollectionLabelMotion[2];
    g_collectionLabelSprite.positionScaleY = kCollectionLabelMotion[3];
    g_itemInfoPopupParameter = 4;
    g_itemInfoPopupDepth = 3.0f;
    g_collectionCursorSprite.positionScaleX = 1.0f;
    g_collectionCursorSprite.positionScaleY = 1.0f;
    g_collectionCursorSprite.alignmentType = 5;
    g_collectionCursorSprite.angle = 0;
    g_collectionCursorSprite.flags14 = 0;
    g_collectionCursorSprite.halfScreenX = 0.0f;
    g_collectionCursorSprite.halfScreenY = 0.0f;
    g_collectionCursorSprite.pvrSurfaceIndex = 252;
    g_collectionCursorSprite.scaledWidth = 38;
    g_collectionLabelSprite.scaledWidth = 512;
    g_collectionLabelSprite.scaledHeight = 32;
    g_collectionLabelSprite.flags14 = 0;
    g_collectionLabelSprite.alignmentType = 5;
    g_collectionLabelSprite.angle = 0;
    g_collectionLabelSprite.depthOrScale = 3.0f;
    for (std::size_t index = 0; index < g_collectionBorderSprites.size(); ++index) {
        ScreenSpaceParams &sprite = g_collectionBorderSprites[index];
        sprite.scaledWidth = 32;
        sprite.scaledHeight = 32;
        sprite.positionScaleX = 1.0f;
        sprite.positionScaleY = 1.0f;
        sprite.depthOrScale = 1.0f;
        sprite.alignmentType = 5;
        sprite.angle = 0;
        sprite.flags14 = 0;
        sprite.halfScreenX = 0.0f;
        sprite.halfScreenY = 0.0f;
        sprite.pvrSurfaceIndex = 0;
        if (kCollectionBorderSurfaces[index] == 254) {
            sprite.scaledWidth = 40;
            sprite.scaledHeight = 16;
        }
        sprite.depthOrScale = 5.0f;
        sprite.pvrSurfaceIndex = kCollectionBorderSurfaces[index];
    }
    g_iconPresentation.itemIconParams.depthOrScale = 5.0f;
    g_collectionInfoSprite.pvrSurfaceIndex = 275;
    g_iconPresentation.itemIconParams.scaledWidth = 32;
    g_iconPresentation.itemIconParams.scaledHeight = 32;
    g_iconPresentation.itemIconParams.positionScaleX = 1.0f;
    g_iconPresentation.itemIconParams.positionScaleY = 1.0f;
    g_iconPresentation.itemIconParams.alignmentType = 5;
    g_iconPresentation.itemIconParams.angle = 0;
    g_iconPresentation.itemIconParams.flags14 = 0;
    g_iconPresentation.itemIconParams.halfScreenX = 0.0f;
    g_iconPresentation.itemIconParams.halfScreenY = 0.0f;
    g_iconPresentation.itemIconParams.pvrSurfaceIndex = 0;
    g_collectionInfoSprite.positionScaleX = 1.0f;
    g_collectionInfoSprite.positionScaleY = 1.0f;
    g_collectionInfoSprite.alignmentType = 5;
    g_collectionInfoSprite.angle = 0;
    g_collectionInfoSprite.flags14 = 0;
    g_collectionInfoSprite.halfScreenX = 0.0f;
    g_collectionInfoSprite.halfScreenY = 0.0f;
    g_collectionInfoSprite.depthOrScale = 3.0f;
    g_collectionInfoSprite.scaledWidth = 80;
    g_collectionInfoSprite.scaledHeight = 40;
    g_collectionTileStrip.corner0 = 0xffffffffU;
    g_collectionTileStrip.vertexCount = 0;
    g_collectionTileStrip.flags = 0x400;
    g_collectionEdgeStrip.flags = 0;
    g_collectionEdgeStrip.corner0 = 0xffffffffU;
    g_collectionEdgeStrip.vertexCount = 0;
    g_collectionTileStrip.normalizedY1 = 0.0f;
    g_collectionTileStrip.normalizedX2 = 0.0f;
    g_collectionTileStrip.normalizedY2 = 0.25f;
    g_collectionTileStrip.normalizedX3 = 0.25f;
    g_collectionTileStrip.normalizedY3 = 0.0f;
    g_collectionEdgeStrip.normalizedY1 = 0.75f;
    g_collectionEdgeStrip.normalizedX2 = 0.625f;
    g_collectionEdgeStrip.normalizedY2 = 1.0f;
    g_collectionEdgeStrip.normalizedX3 = 0.75f;
    g_collectionEdgeStrip.normalizedY3 = 0.0f;
}

// @verified - p
void CollectionGrid_Reset() {
    g_collectionGridPanelMotion = kCollectionPanelMotion;
    g_collectionGridPhase = CollectionGridPhase::Inactive;
    g_collectionGridTargetX = 60.0f;
    g_collectionGridPanelValue = 4;
    g_collectionOwnedCounts = nullptr;
    g_collectionOwnedCatalog = nullptr;
    g_collectionGridDepth = 4.0f;
    g_collectionGridFrame.labelSurface = -1;
    g_collectionGridFrame.depth = 4.0f;
    g_collectionGridFrame.selectedRow = 0;
    g_collectionGridFrame.selectedColumn = 0;
    g_collectionGridFrame.columns = 12;
    g_collectionGridFrame.rows = 7;
    g_collectionGridFrame.selectedStackCount = 0;
    g_collectionGridDirty = 0;
    g_collectionOwnedEntryCount = 0;
    g_collectionCursorPulse = 0;
    g_collectionCursorPulseRising = 1;
    g_collectionGridOperations = nullptr;
    g_collectionCassetteSprite.scaledWidth = 32;
    g_collectionCassetteSprite.scaledHeight = 32;
    g_collectionCassetteSprite.positionScaleX = 1.0f;
    g_collectionCassetteSprite.positionScaleY = 1.0f;
    g_collectionCassetteSprite.alignmentType = 5;
    g_collectionCassetteSprite.angle = 0;
    g_collectionCassetteSprite.flags14 = 0;
    g_collectionCassetteSprite.halfScreenX = 0.0f;
    g_collectionCassetteSprite.halfScreenY = 0.0f;
    g_collectionCassetteSprite.pvrSurfaceIndex = 0;
    g_collectionCassetteSprite.depthOrScale = 4.0f;
}

// @verified - p
bool CollectionGrid_UpdateOpeningSlide(std::array<float, 4> &motion, float target) {
    float speed = target - motion[0];
    if (speed < 0.0f)
        speed = -speed;
    speed *= 0.5f;
    motion[2] = speed <= 64.0f ? speed : motion[2] + motion[2];
    if (motion[2] < 1.0f)
        motion[2] = 1.0f;
    else if (motion[2] > 64.0f)
        motion[2] = 64.0f;
    motion[0] += motion[2];
    if (motion[0] < target)
        return false;
    motion[0] = target;
    return true;
}

// @verified - p
bool CollectionGrid_UpdateClosingSlide(std::array<float, 4> &motion, float target) {
    float speed = target - motion[0];
    if (speed < 0.0f)
        speed = -speed;
    speed *= 0.5f;
    motion[2] = speed <= 64.0f ? speed : motion[2] + motion[2];
    if (motion[2] < 1.0f)
        motion[2] = 1.0f;
    else if (motion[2] > 64.0f)
        motion[2] = 64.0f;
    motion[0] -= motion[2];
    if (motion[0] > target)
        return false;
    motion[0] = target;
    return true;
}

// @verified - p
bool CollectionGrid_Tick() {
    bool closed = false;
    CollectionGridFrame &frame = g_collectionGridFrame;
    switch (g_collectionGridPhase) {
    case CollectionGridPhase::OpenInit: {
        g_collectionGridOperations->initPage();
        g_collectionGridPanelMotion[0] = -660.0f;
        g_collectionGridPhase = CollectionGridPhase::SlidingIn;
        g_collectionGridDirty = 1;
        frame.selectedRow = 0;
        frame.selectedColumn = 0;
        g_collectionGridOperations->refreshSelectionLabel();
        g_collectionSavedHelpToken = static_cast<std::int8_t>(g_defaultHelpTokenId);
        g_collectionSavedButtonPrompt = g_helpButtonId;
        const std::int8_t help = kCollectionHelpTokens[g_collectionGridMode];
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_helpButtonId = 0;
        g_defaultHelpTokenId = static_cast<std::uint8_t>(help < 68 ? help : 0);
        g_currentHelpTokenId = g_defaultHelpTokenId;
        g_currentHelpIconState = 0;
        frame.rows = static_cast<std::int8_t>(g_collectionPageEntryCount / frame.columns);
        if (g_collectionPageEntryCount % frame.columns)
            frame.rows = static_cast<std::int8_t>(frame.rows + 1);
        g_collectionGridPanelMotion[1] = static_cast<float>(214.0 - static_cast<double>(20 * frame.rows));
        g_collectionGridTargetX = g_collectionGridMode ? 160.0f : 60.0f;
        break;
    }
    case CollectionGridPhase::SlidingIn:
        if (!(Shenmue_nPauseState & 2) && CollectionGrid_UpdateOpeningSlide(g_collectionGridPanelMotion, g_collectionGridTargetX))
            g_collectionGridPhase = CollectionGridPhase::Active;
        break;
    case CollectionGridPhase::Active:
        CollectionGrid_HandleInput();
        break;
    case CollectionGridPhase::WaitingPreviewClose:
        if (!g_iconPresentation.sharedItemMenuState) {
            g_collectionGridPhase = CollectionGridPhase::Active;
            g_collectionGridDirty = 1;
            CollectionGrid_Draw();
            return false;
        }
        break;
    case CollectionGridPhase::Closing:
        if (!(Shenmue_nPauseState & 2)) {
            if (g_sfxInputFlags & 0x200) {
                CollectionGrid_Close();
                g_iconPresentation.menuCloseRequested = true;
                closed = true;
            } else if (CollectionGrid_UpdateClosingSlide(g_collectionGridPanelMotion, -660.0f)) {
                CollectionGrid_Close();
                closed = true;
            }
        }
        break;
    case CollectionGridPhase::PageSlideOut:
        if (CollectionGrid_UpdateClosingSlide(g_collectionGridPanelMotion, -660.0f)) {
            g_collectionGridPanelMotion[0] = -660.0f;
            g_collectionGridPhase = CollectionGridPhase::PageSlideIn;
            CollectionGrid_SetOwnedPage(g_collectionOwnedPage);
            frame.selectedRow = 0;
            frame.selectedColumn = 0;
            g_collectionGridOperations->refreshSelectionLabel();
            frame.rows = static_cast<std::int8_t>(g_collectionPageEntryCount / frame.columns);
            if (g_collectionPageEntryCount % frame.columns)
                frame.rows = static_cast<std::int8_t>(frame.rows + 1);
            g_collectionGridPanelMotion[1] = static_cast<float>(214.0 - static_cast<double>(20 * frame.rows));
            UI_SICNSetState(g_collectionOwnedPage ? 0xa7 : 0x67, UISicnPausePolicy::KeepOnPause);
        }
        break;
    case CollectionGridPhase::PageSlideIn:
        if (CollectionGrid_UpdateOpeningSlide(g_collectionGridPanelMotion, g_collectionGridTargetX)) {
            g_collectionGridPanelMotion[0] = 60.0f;
            g_collectionGridPhase = CollectionGridPhase::Active;
        }
        break;
    default:
        break;
    }
    if (g_collectionGridDirty)
        CollectionGrid_Draw();
    return closed;
}

// @verified - p
void CollectionGrid_HandleInput() {
    const std::uint16_t input = static_cast<std::uint16_t>(g_sfxInputFlags);
    if (input & 0x200) {
        g_collectionGridPhase = CollectionGridPhase::Closing;
    } else if (input & 0x402) {
        const std::int16_t preview = g_collectionGridOperations->getPreviewId();
        if (preview != -1) {
            g_collectionGridDirty = 0;
            g_collectionGridPhase = CollectionGridPhase::WaitingPreviewClose;
            Load3DCassetteModelForInventory(preview, g_collectionGridMode != 0);
        }
    } else if ((!g_collectionGridOperations->handleInput || !g_collectionGridOperations->handleInput(input)) && CollectionGrid_MoveCursor(input)) {
        g_collectionGridOperations->refreshSelectionLabel();
    }
}

// @verified - p
void CollectionGrid_Draw() {
    if (!g_updateRenderFrameMode || !g_collectionGridOperations->drawPage)
        return;
    g_collectionGridOperations->drawPage();
    const CollectionGridFrame &frame = g_collectionGridFrame;
    g_collectionCursorSprite.halfScreenX = ((40 * frame.selectedColumn + g_collectionGridPanelMotion[0]) + 11.0f) + 24.0f;
    g_collectionCursorSprite.halfScreenY = (40 * frame.selectedRow + g_collectionGridPanelMotion[1]) + 24.0f;
    g_collectionCursorSprite.depthOrScale = g_collectionGridDepth + 2.0f;
    if (g_updateRenderFrameMode)
        AdjustScreenSpaceWithParams(&g_collectionCursorSprite, g_collectionCursorPulse | 0xff000000U, 0);
    if (!(Shenmue_nPauseState & 2)) {
        if (g_collectionCursorPulseRising) {
            g_collectionCursorPulse += 0x05050505;
            if (g_collectionCursorPulse >= 0x4f4f4f4f) {
                g_collectionCursorPulseRising = 0;
                g_collectionCursorPulse = 0x4f4f4f4f;
            }
        } else {
            g_collectionCursorPulse -= 0x05050505;
            if (g_collectionCursorPulse <= 0x05050505) {
                g_collectionCursorPulseRising = 1;
                g_collectionCursorPulse = 0;
            }
        }
    }
    g_collectionGridFrame.x = g_collectionGridPanelMotion[0];
    g_collectionGridFrame.y = g_collectionGridPanelMotion[1];
    CollectionGrid_DrawFrame();
}

// @verified - p
void CollectionGrid_Close() {
    g_collectionGridPhase = CollectionGridPhase::Inactive;
    if (g_collectionGridFrame.labelSurface != -1) {
        PVR_FreeAllocatedSurfaceByIndex(g_collectionGridFrame.labelSurface);
        g_collectionGridFrame.labelSurface = -1;
    }
    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t help = static_cast<std::uint8_t>(g_collectionSavedHelpToken < 68 ? g_collectionSavedHelpToken : 0);
    g_collectionGridPanelMotion[0] = -660.0f;
    g_defaultHelpTokenId = help;
    g_currentHelpTokenId = help;
    g_helpButtonId = g_collectionSavedButtonPrompt;
    std::uint16_t state = 0x27;
    g_collectionGridDirty = 0;
    g_collectionGridOperations = nullptr;
    g_currentHelpIconState = 0;
    g_currentSicn = UISicnPausePolicy::KeepOnPause;
    if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20))
        state = 0x26;
    if ((g_uiState & 0x200) && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive)
        state |= 0x200;
    g_uiState = state;
}

// @verified - p
bool CollectionGrid_MoveCursor(std::uint16_t input) {
    CollectionGridFrame &frame = g_collectionGridFrame;
    std::int8_t row = frame.selectedRow;
    std::int8_t column = frame.selectedColumn;
    const std::int8_t previousRow = row;
    const std::int8_t previousColumn = column;
    if (g_collectionPageEntryCount <= 1)
        input = 0;
    if (g_collectionPageEntryCount <= frame.columns)
        input &= 0xcfff;
    const std::int32_t start = frame.columns * row;
    if (start + column == g_collectionPageEntryCount - 1 && column == 0)
        input &= 0x3fff;
    if (!(input & 0xf000))
        return false;
    if (input & 0x4000) {
        column = static_cast<std::int8_t>(column - 1);
        frame.selectedColumn = column;
        if (column < 0)
            column = static_cast<std::int8_t>(frame.columns - 1);
        frame.selectedColumn = column;
    } else if (input & 0x8000) {
        column = static_cast<std::int8_t>(column + 1);
        if (start + column >= g_collectionPageEntryCount)
            column = 0;
        if (column >= frame.columns)
            column = 0;
        frame.selectedColumn = column;
    }
    if (input & 0x1000) {
        row = static_cast<std::int8_t>(row - 1);
        frame.selectedRow = row;
        if (row < 0) {
            std::int8_t rows = 0;
            if (column < g_collectionPageEntryCount) {
                do {
                    rows = static_cast<std::int8_t>(rows + 1);
                } while (column + frame.columns * rows < g_collectionPageEntryCount);
            }
            row = static_cast<std::int8_t>(rows - 1);
        }
        frame.selectedRow = row;
    } else if (input & 0x2000) {
        if (g_collectionPageEntryCount - start - column > frame.columns) {
            row = static_cast<std::int8_t>(row + 1);
            if (row >= frame.rows)
                row = 0;
            if (column + frame.columns * row >= g_collectionPageEntryCount)
                row = 0;
        } else {
            row = 0;
        }
        frame.selectedRow = row;
    }
    const std::int32_t offset = frame.columns * row;
    if (offset + column >= g_collectionPageEntryCount) {
        if (offset > g_collectionPageEntryCount) {
            do {
                row = static_cast<std::int8_t>(row - 1);
                frame.selectedRow = row;
                if (row <= 0) {
                    row = 0;
                    frame.selectedRow = 0;
                    break;
                }
            } while (frame.columns * row > g_collectionPageEntryCount);
        }
        column = static_cast<std::int8_t>(g_collectionPageEntryCount - row * frame.columns - 1);
        frame.selectedColumn = column;
    }
    return previousColumn != column || previousRow != row;
}

// @verified - p
void CollectionGrid_RefreshOwnedLabel() {
    const std::uint8_t *counts = g_collectionOwnedCounts;
    std::int16_t id = -1;
    std::int32_t ordinal = 0;
    for (std::int32_t offset = 0; offset < 168 && offset + g_collectionOwnedBaseCatalogId < 235; ++offset, ++counts) {
        if (*counts) {
            if (ordinal == g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns) {
                id = static_cast<std::int16_t>(offset + g_collectionOwnedBaseCatalogId);
                break;
            }
            ++ordinal;
        }
    }
    const char *text = nullptr;
    if (id < 235) {
        if (id != -1)
            text = g_ownedItemCatalog[id].itemNameId ? g_ownedItemCatalog[id].itemNameId : "ERROR";
        CollectionGrid_RefreshLabelSurface(g_collectionGridFrame.labelSurface, text);
    }
}

// @verified - p
void CollectionGrid_RefreshCassetteLabel() {
    const std::uint16_t index =
        static_cast<std::uint16_t>(g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns);
    if (index <= 31) {
        const std::int16_t id = static_cast<std::int8_t>(g_playerStats.cassetteTapes[index].id);
        if (id != -1) {
            const char *const text = InitAndRetrieveStringFromID(g_cassetteCatalog[id].entry.cassetteNameId);
            CollectionGrid_RefreshLabelSurface(g_collectionGridFrame.labelSurface, text);
        }
    }
}

// @verified - p
void CollectionGrid_InitOwnedPage() {
    std::int16_t count = 0;
    for (std::uint8_t value : g_playerStats.inventoryItemCounts67To234) {
        if (value)
            ++count;
    }
    g_collectionOwnedEntryCount = count;
    g_collectionGridFrame.columns = 12;
    g_collectionGridFrame.rows = 7;
    g_collectionOwnedPage = 0;
    std::int8_t pages = static_cast<std::int8_t>(count / 84);
    g_collectionOwnedPageCount = pages;
    if (count % 84) {
        pages = static_cast<std::int8_t>(pages + 1);
        g_collectionOwnedPageCount = pages;
    }
    CollectionGrid_SetOwnedPage(0);
    g_currentSicn = UISicnPausePolicy::KeepOnPause;
    std::uint16_t state = pages >= 2 ? 0x67 : 0x27;
    if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20))
        state &= ~1U;
    if ((g_uiState & 0x200) && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive)
        state |= 0x200;
    g_uiState = state;
    if (!state)
        UI_SICNClearState(4095);
}

// @verified - p
void CollectionGrid_InitCassettePage() {
    std::int16_t count = 0;
    g_collectionGridFrame.columns = 7;
    g_collectionGridFrame.rows = 4;
    for (const CassetteTapeState &tape : g_playerStats.cassetteTapes) {
        if (tape.id != 0xff)
            ++count;
    }
    g_collectionPageEntryCount = count;
    std::uint16_t state = 0x27;
    g_currentSicn = UISicnPausePolicy::KeepOnPause;
    if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20))
        state = 0x26;
    if ((g_uiState & 0x200) && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive)
        state |= 0x200;
    g_uiState = state;
}

// @verified - p
bool CollectionGrid_HandleOwnedPageInput(std::uint16_t input) {
    const std::int32_t previous = g_collectionOwnedPage;
    if (input & 0x40) {
        g_collectionOwnedPage = static_cast<std::int8_t>(g_collectionOwnedPage - 1);
        if (g_collectionOwnedPage < 0)
            g_collectionOwnedPage = 0;
    } else if (input & 0x80) {
        g_collectionOwnedPage = static_cast<std::int8_t>(g_collectionOwnedPage + 1);
        if (g_collectionOwnedPage >= g_collectionOwnedPageCount)
            g_collectionOwnedPage = static_cast<std::int8_t>(g_collectionOwnedPageCount - 1);
    }
    if (previous == g_collectionOwnedPage)
        return false;
    g_collectionGridPhase = CollectionGridPhase::PageSlideOut;
    return true;
}

// @verified - p
void CollectionGrid_DrawOwnedPage() {
    CollectionGrid_DrawOwnedIcons(g_collectionOwnedCounts, 12, g_collectionOwnedCatalog, 256 - 84 * g_collectionOwnedPage,
        (g_collectionGridPanelMotion[0] + 11.0f) + 24.0f, g_collectionGridPanelMotion[1] + 24.0f, g_collectionGridDepth, g_collectionPageEntryCount);
    std::int32_t ordinal = 0;
    const std::uint8_t *counts = g_collectionOwnedCounts;
    for (std::int32_t offset = 0; offset < 168 && offset + g_collectionOwnedBaseCatalogId < 235; ++offset, ++counts) {
        if (*counts) {
            if (ordinal == g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns) {
                const std::int16_t id = static_cast<std::int16_t>(offset + g_collectionOwnedBaseCatalogId);
                if (id >= 0)
                    g_collectionGridFrame.selectedStackCount = g_playerStats.inventoryItemCounts67To234[id - 67];
                return;
            }
            ++ordinal;
        }
    }
}

// @verified - p
void CollectionGrid_DrawCassettePage() {
    const CassetteTapeState *tape = g_playerStats.cassetteTapes.data();
    std::int32_t column = 0;
    std::int32_t y = 0;
    for (std::int32_t remaining = 32; remaining != 0; --remaining) {
        if (tape->id != 0xff) {
            const std::int8_t id = static_cast<std::int8_t>(tape->id);
            g_collectionCassetteSprite.pvrSurfaceIndex = g_cassetteCatalog[id].entry.iconIndex;
            g_collectionCassetteSprite.halfScreenX = ((40 * column + g_collectionGridPanelMotion[0]) + 11.0f) + 24.0f;
            g_collectionCassetteSprite.halfScreenY = (y + g_collectionGridPanelMotion[1]) + 24.0f;
            if (g_updateRenderFrameMode)
                AdjustScreenSpaceWithParams(&g_collectionCassetteSprite, 0xff000000U, 4);
            if (++column >= 7) {
                column = 0;
                y += 40;
            }
            ++tape;
        }
    }
    const std::uint16_t index =
        static_cast<std::uint16_t>(g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns);
    if (index <= 31) {
        const CassetteTapeState &selected = g_playerStats.cassetteTapes[index];
        const std::int8_t id = static_cast<std::int8_t>(selected.id);
        if (id >= 0) {
            const CassetteTapeState *found = nullptr;
            if (id < 32) {
                for (const CassetteTapeState &entry : g_playerStats.cassetteTapes) {
                    if (entry.id == id) {
                        found = &entry;
                        break;
                    }
                }
            }
            g_collectionGridFrame.selectedStackCount = found->count;
        }
    }
}

// @verified - p
std::int16_t CollectionGrid_GetOwnedPreviewId() {
    std::int32_t ordinal = 0;
    const std::uint8_t *counts = g_collectionOwnedCounts;
    for (std::int32_t offset = 0; offset < 168 && offset + g_collectionOwnedBaseCatalogId < 235; ++offset, ++counts) {
        if (*counts) {
            if (ordinal == g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns)
                return static_cast<std::int16_t>(g_collectionOwnedBaseCatalogId + offset);
            ++ordinal;
        }
    }
    return -1;
}

// @verified - p
std::int16_t CollectionGrid_GetCassettePreviewId() {
    const std::uint16_t index =
        static_cast<std::uint16_t>(g_collectionGridFrame.selectedColumn + g_collectionGridFrame.selectedRow * g_collectionGridFrame.columns);
    return index > 31 ? -1 : static_cast<std::int8_t>(g_playerStats.cassetteTapes[index].id);
}

// @verified - p
void CollectionGrid_SetOwnedPage(std::int32_t page) {
    const std::uint8_t *counts = g_playerStats.inventoryItemCounts67To234.data();
    g_collectionPageEntryCount = static_cast<std::int16_t>(
        84 * (g_collectionOwnedPage + 1) < g_collectionOwnedEntryCount ? 84 : g_collectionOwnedEntryCount - 84 * g_collectionOwnedPage);
    std::int32_t ordinal = 0;
    for (std::int32_t offset = 0; offset < 256; ++offset, ++counts) {
        if (*counts) {
            if (84 * page == ordinal) {
                g_collectionOwnedCounts = counts;
                g_collectionOwnedBaseCatalogId = static_cast<std::int16_t>(offset + 67);
                g_collectionOwnedCatalog = g_ownedItemCatalog.data() + 67 + offset;
                return;
            }
            ++ordinal;
        }
    }
}

// @verified - p
void CollectionGrid_RefreshLabelSurface(std::int16_t &surface, const char *text) {
    if (surface != -1)
        PVR_FreeAllocatedSurfaceByIndex(surface);
    surface = -1;
    char buffer[64];
    std::sprintf(buffer, "%s", text);
    surface = AllocatePVRSurface(buffer, 512, 0xcccc, 0xf000, 0, true);
}

// @verified - p
void CollectionGrid_DrawEdges(float x, float y, std::int32_t rows, std::int32_t columns) {
    if (!g_updateRenderFrameMode)
        return;
    float height = 20.0f;
    if (rows) {
        const std::int32_t right = static_cast<std::int32_t>(static_cast<float>(40 * columns + 8) + x);
        const std::int32_t left = static_cast<std::int32_t>(x);
        do {
            g_collectionEdgeStrip.flags |= 0x400;
            UILayout_BuildQuad(&g_collectionEdgeStrip, 5, left, static_cast<std::int32_t>(height + y), 40, 16, 1.0f, 1.0f, 0x4000, 0);
            g_collectionEdgeStrip.flags |= 0x700;
            UILayout_BuildQuad(&g_collectionEdgeStrip, 5, right, static_cast<std::int32_t>(height + y), 40, 16, 1.0f, 1.0f, 0x4000, 0);
            height += 40.0f;
        } while (--rows);
    }
}

// @verified - p
void CollectionGrid_DrawTiles(float x, float y, std::int32_t columns, std::int32_t rows) {
    if (g_updateRenderFrameMode && columns) {
        do {
            float nextY = y;
            for (std::int32_t remaining = rows; remaining != 0; --remaining) {
                AdjustAspectRatioMatrix(&g_collectionTileStrip, 5, static_cast<std::int32_t>(x), static_cast<std::int32_t>(nextY), 40, 40, 0);
                nextY += 40.0f;
            }
            x += 40.0f;
        } while (--columns);
    }
}

// @verified - p
void CollectionGrid_DrawFrameFade(float x, float y, float width, float height, float depth) {
    if (g_updateRenderFrameMode) {
        OriginalStrip strip;
        strip.normalizedX3 = 1.0f;
        strip.normalizedY1 = 0.0f;
        strip.normalizedX2 = 0.0f;
        strip.normalizedY2 = 1.0f;
        strip.normalizedY3 = 0.0f;
        strip.vertexCount = 0;
        strip.flags = 8;
        strip.corner0 = 0x05000020;
        strip.corner1 = 0x05000020;
        strip.corner2 = 0xbf000000;
        strip.corner3 = 0xbf000000;
        AdjustAspectRatioMatrix(
            &strip, 0, static_cast<std::int32_t>(x), static_cast<std::int32_t>(y), static_cast<std::int32_t>(width), static_cast<std::int32_t>(height), 0);
        strip.flags |= 0x400;
        ApplyFadeToPacket(&strip, depth);
    }
}

// @verified - p
void CollectionGrid_DrawOwnedIcons(const std::uint8_t *counts, std::int32_t columns, const OwnedItemCatalogEntry *catalog, std::int32_t count, float x, float y,
    float depth, std::int32_t maxEntries) {
    std::int32_t column = 0;
    std::int32_t drawn = 0;
    g_iconPresentation.itemIconParams.depthOrScale = depth;
    g_iconPresentation.itemIconParams.halfScreenX = x;
    g_iconPresentation.itemIconParams.halfScreenY = y;
    for (; count != 0; --count, ++counts, ++catalog) {
        if (*counts) {
            g_iconPresentation.itemIconParams.pvrSurfaceIndex = catalog->iconIndex;
            if (g_updateRenderFrameMode)
                AdjustScreenSpaceWithParams(&g_iconPresentation.itemIconParams, 0xff000000U, 0);
            g_iconPresentation.itemIconParams.halfScreenX += 40.0f;
            if (++column >= columns) {
                g_iconPresentation.itemIconParams.halfScreenY += 40.0f;
                g_iconPresentation.itemIconParams.halfScreenX = x;
                column = 0;
            }
            if (++drawn >= maxEntries)
                break;
        }
    }
}

// @verified - p
void CollectionGrid_DrawStackCount(float x, float y, std::uint16_t count) {
    ui_runtime_detail::CashOverlayOwner &cash = ui_runtime_detail::g_cashOverlayPresentation;
    float left = (x - 128.0f) - 16.0f;
    for (std::int32_t denomination : ui_runtime_detail::kCashDenominations) {
        if (count >= denomination)
            break;
        left += 16.0f;
    }
    cash.selectorChoiceMarkers[0].halfScreenX = x;
    cash.selectorChoiceMarkers[1].halfScreenX = left;
    cash.selectorChoiceMarkers[0].halfScreenY = y;
    cash.selectorChoiceMarkers[1].halfScreenY = y;
    ui_runtime_detail::DrawCashSelectorDigits(x - 16.0f, y, 1.0f, 5.0f, count, 0xff303030U, 4);
    AdjustScreenSpaceWithParams(&cash.selectorChoiceMarkers[0], 0xff303030U, 4);
    AdjustScreenSpaceWithParams(&cash.selectorChoiceMarkers[1], 0xff303030U, 4);
}

// @verified - p
void CollectionGrid_DrawFrame() {
    const CollectionGridFrame &frame = g_collectionGridFrame;
    const float x = frame.x;
    const float y = frame.y;
    const float depth = frame.depth;
    const std::int32_t columns = frame.columns < 1 ? 1 : frame.columns;
    const std::int32_t rows = frame.rows < 1 ? 1 : frame.rows;
    const std::int32_t height = 40 * rows;
    const float width = static_cast<float>(40 * columns);
    const float left = x + 11.0f;
    CollectionGrid_DrawFrameFade(left + 4.0f, y + 4.0f, width, static_cast<float>(height + 48), depth - 1.0f);
    CollectionGrid_DrawEdges(left, y + 27.0f, rows, columns);
    CollectionGrid_DrawTiles(left + 24.0f, y + 24.0f, columns, rows);
    float nextX = x + 3.0f;
    for (std::int32_t column = 0; column <= columns; ++column) {
        const std::int32_t index = ui_runtime_detail::GetIsSame(column, columns);
        ScreenSpaceParams &sprite = g_collectionBorderSprites[index];
        const std::int32_t halfWidth = sprite.scaledWidth >> 1;
        sprite.depthOrScale = depth;
        sprite.halfScreenY = y;
        sprite.flags14 = 0;
        const float middle = nextX + static_cast<float>(halfWidth);
        sprite.halfScreenX = middle;
        if ((index & ~2) == 0)
            sprite.halfScreenY = y + 11.0f;
        if (column == 0 || column == columns) {
            sprite.depthOrScale = depth + 1.0f;
            sprite.halfScreenX = column ? middle - 3.0f : middle + 3.0f;
        }
        if (g_updateRenderFrameMode)
            AdjustScreenSpaceWithParams(&sprite, 0xff000000U, 0);
        nextX = middle + static_cast<float>(sprite.scaledWidth >> 1);
    }
    nextX = x + 3.0f;
    const float separatorY = y + static_cast<float>(height + 8);
    for (std::int32_t column = 0; column <= columns; ++column) {
        ScreenSpaceParams &sprite = g_collectionBorderSprites[ui_runtime_detail::SelectListEndpointOrMiddleSpriteSlot(column, columns)];
        sprite.depthOrScale = depth;
        sprite.halfScreenY = separatorY;
        sprite.flags14 = 0;
        const float middle = nextX + static_cast<float>(sprite.scaledWidth >> 1);
        sprite.halfScreenX = middle;
        if (column == 0)
            sprite.flags14 = 768;
        if (column == 0 || column == columns) {
            sprite.depthOrScale = depth + 1.0f;
            sprite.halfScreenX = column ? middle - 3.0f : middle + 3.0f;
        }
        if (g_updateRenderFrameMode)
            AdjustScreenSpaceWithParams(&sprite, 0xff000000U, 0);
        nextX = middle + static_cast<float>(sprite.scaledWidth >> 1);
    }
    const float bottomY = separatorY + 48.0f;
    nextX = x + 3.0f;
    for (std::int32_t column = 0; column <= columns; ++column) {
        const std::int32_t index = ui_runtime_detail::GetIsSame(columns - column, columns);
        ScreenSpaceParams &sprite = g_collectionBorderSprites[index];
        sprite.depthOrScale = depth;
        sprite.halfScreenY = bottomY;
        sprite.flags14 = 768;
        const float middle = nextX + static_cast<float>(sprite.scaledWidth >> 1);
        sprite.halfScreenX = middle;
        if ((index & ~2) == 0)
            sprite.halfScreenY = bottomY - 11.0f;
        if (column == 0 || column == columns) {
            sprite.depthOrScale = depth + 1.0f;
            sprite.halfScreenX = column ? middle - 3.0f : middle + 3.0f;
        }
        if (g_updateRenderFrameMode)
            AdjustScreenSpaceWithParams(&sprite, 0xff000000U, 0);
        nextX = middle + static_cast<float>(sprite.scaledWidth >> 1);
    }
    CollectionGrid_DrawTiles(left + 24.0f, bottomY - 24.0f, columns, 1);
    CollectionGrid_DrawEdges(left, bottomY - 44.0f, 1, columns);
    if (frame.labelSurface != -1) {
        g_collectionLabelSprite.pvrSurfaceIndex = frame.labelSurface;
        g_collectionLabelSprite.halfScreenY = bottomY - 24.0f;
        g_collectionLabelSprite.depthOrScale = depth;
        g_collectionLabelSprite.halfScreenX = static_cast<float>(20 * columns - 20) + (left + 24.0f);
        AdjustDefaultScreenSpace(&g_collectionLabelSprite);
    }
    if (frame.selectedStackCount > 1)
        CollectionGrid_DrawStackCount((((width + left) + 8.0f) - 4.0f) - 8.0f, (bottomY - 24.0f) + 4.0f, frame.selectedStackCount);
}

} // namespace shenmue::s1
