#include "s1/ui_internal.hpp"

#include "s1/ui_collection.hpp"
#include "s1/steam.hpp"
#include "s1/modern_material.hpp"
#include "s1/render_array.hpp"
namespace shenmue::s1::ui_runtime_detail {
void ItemsMenu_PopulateEntries();

void Items_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *elements,
    std::int32_t index);

void SharedItemsMenu_DestroyAndResetPreviewState();

std::int32_t GetAssignedFightMoveIndex();

void Notebook_DrawPageText(std::int32_t side, std::uint8_t page, std::uint32_t renderTarget);

void UI_SetOrbitLightDirection(Vector3 &position, const Vector3 &cameraEye, std::int16_t yaw);

void UI_AimObjectLight(const UIDrawObject &object);

void UI_DrawObjectModel(UIDrawObject &object);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void CreateNotebookTexture2DForPages();

void CreateNotebookTexture2D(std::int32_t surface, std::byte *memoTexture);

void FormatNotebookText(std::uint32_t pageIndex, std::uint32_t textBuffer, std::uint32_t pageSlot, std::uint32_t line, char messageType);

void Notebook_BindModelPageTextures();

void InitNotebookTexture2D();

void DrawObjectForUI(UIDrawObject *object);

bool UI_ICONStartItemInfoPopup(std::uint16_t itemId);

bool UI_ICONAdvanceItemInfoPopup();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void IconSystemMenu_AimCameraAtRyo(IconSystemMenuCameraTransition &transition, Character &ryo);

void IconSystemMenu_ApplyCamera(const IconSystemMenuCameraTransition &transition);

void shenBeginIconSystemMenuCameraTransition(IconSystemMenuCameraTransition &transition);

bool HandleRyoCameraAlpha(IconSystemMenuCameraTransition &transition);

std::int32_t IconSystemMenu_FindSelectedInventoryItem();

void icsClampSelectionPage();

void icsmPrepareInfoSurface();

void HandleCassetteItemEvents();

void IconSystemMenu_UpdateMenuCloseState();

void IconSystemMenu_FinishClose();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void UI_ICSSCallback(HLib::TaskData *);

void UI_ICSSMainCallback(HLib::TaskData *);

bool UI_ICLISharedItemMenuIsActive();

bool UI_ICLILoadSharedItemPreview(std::int16_t itemId);

bool UI_ICLISetSharedItemPreview(std::int16_t itemId);

void UI_ICLICallback(HLib::TaskData *);
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
const char *ItemLook_ExamineRemark(std::int16_t itemId, std::uint32_t &move);

bool LoadItem();

void ItemLook_Place();

void DrawBatteryIcons();

void CassetteMenu_HandleInput();

void ItemLook_Open();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void UI_ICLIMainCallback(HLib::TaskData *);

void DestroySharedItemsMenu();

void DestroySharedItemsMenuIfTrueResponse();

bool Load3DCassetteModelForInventory(std::int16_t id, bool cassette);
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void ReleaseCassettePlayerResources();

void ResetCassettePlayer();

void BatteriesAreDead();

void PlayCassette();

void Cassette_RequestPlay(const std::array<float, 4> &cell);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t::ui_runtime_detail {
void ShowDescriptionForItem(float *rect, void *data);
}

namespace shenmue::s1::ui_runtime_detail {
void Cassette_ShowDescription(float *rect, void *data);

void Cassette_SetSelectedIndex(float *rect, void *data);

void cbHandleAllItemsWithInteraction(float *, void *data);

const char *UIGridEntry_NameToken(const UIGridEntry &entry);

std::int32_t Cassettes_PopulateEntries(std::array<UIGridEntry, 32> &elements);

void Cassettes_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    void *elements, std::int32_t index);

void CassetteMusicNote(const std::array<float, 4> &cell);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
std::int64_t CassettesMenu_GetPromptIconSet();
}

namespace shenmue::s1::ui_runtime_detail {
void Gacha_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *elements,
    std::int32_t index);

void Gacha_PopulateEntries();

void Collection_ShowSelectedTab();

void SubMenu_Collection_OnTabHovered(float *, void *data);

void SubMenu_Collection_OnTabHoverLeave(float *, void *data);

void SubMenu_Collection_OnTabClicked(float *, void *data);

void Scrolls_PopulateEntries();

void Scrolls_DrawCell(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *elements, std::int32_t index);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
bool shenPrepareCassetteUI();

bool UI_ICMCBegin();

bool UI_ICMCWaitForClosed(std::int32_t &result);
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
char *shenGetMoveNameFromID(std::int32_t move);

void _0001_UpdateRyoMove(std::int32_t move);

bool AssignFightMove(std::int32_t move);

void MovesScreen_RemapAnswered(UIRetry70ManBattlePromptResult result);

void CreateMessageBoxForFightMoveRemapping(std::int32_t move);

void MovesScreen_ClampStatBars();

void UpdateSelectedMoveInMovesScreen(float *rect, void *data);

void MovesScreen_ConfirmMove(float *rect, void *data);

void *MovesScreen_GetElement(void *elements, std::int32_t index);

void MovesScreen_SetListCount(std::int32_t count);

void MovesScreen_StartButtonFrame(std::int32_t slotIndex);

void MovesScreen_DrawCell(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *, std::int32_t index);

bool UI_SetViewImageTexture(d3t::UIViewImage &image, std::uint32_t texture);

void MovesScreen_SetTextureAlphas(float front, float game);



void ItemsMenu_PopulateEntries() {
    const std::int32_t showAll = g_debugForceShowAllItems;
    std::int32_t count = 0;
    for (std::int32_t item = 0; item < 256; ++item) {
        const std::uint8_t owned = g_playerStats.inventoryItemCounts0To66[static_cast<std::size_t>(item)];
        if ((owned || (showAll && item < 67)) && static_cast<std::uint32_t>(item - 17) > 17U) {
            UIGridEntry &entry = g_itemsGridElements[static_cast<std::size_t>(count)];
            entry.count = owned;
            entry.type = 0;
            entry.id = item;
            if (showAll)
                entry.count = owned + 1;
            ++count;
        }
    }
    UIGrid &grid = g_itemsGrid;
    grid.elements = g_itemsGridElements.data();
    g_itemsGridElementCount = count;
    grid.elementCount = count;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (count > grid.columns * (lastRow + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0 ? 1 : 0);
}


void Items_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *elements,
    std::int32_t index) {
    static_cast<void>(UIGrid_PlaceCellWidgets(g_itemsGridTemplate, index, *holder, *cell));
    UIGrid_AfterDraw(g_itemsGridTemplate, grid, index, 1, -1, *holder, panel);
    UIGrid_DrawItemCell(grid, panel, holder, cell, spacing, element, elements, index);
}


std::int32_t UI_FindViewSpacePush(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U || !id)
        return -1;
    const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 13)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) > 0x3ffU)
            continue;
        const char *const name = g_uiLayoutItems[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}







std::int64_t ItemsMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuItems].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_itemsGridWindows.size(); ++window)
        g_itemsGridWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kItemsGridWindowIds[window]));
    g_itemsGridPlaceholderLabel = UI_LabelRecord(UI_FindViewLabel(slot, "Label_Placeholder"));
    for (std::size_t item = 0; item < g_itemsGridLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kItemsGridLayoutItemIds[item]);
        g_itemsGridLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    const std::int32_t image = UI_FindViewImageWidget(slot, "Image_Placeholder");
    g_itemsGridPlaceholderImage = static_cast<std::uint32_t>(image) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(image)] : nullptr;
    UIGrid &grid = g_itemsGrid;
    grid.columns = 7;
    grid.visibleRows = 3;
    grid.rows = 36;
    grid.showEmptyCells = 1;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_itemsGridWindows[0];
    grid.rect = {};
    grid.drawCell = Items_DrawCell;
    grid.mouseCallbacks = {
        .entered = d3t::ShowDescriptionForItem, .hovered = nullptr, .left = nullptr, .clicked = cbHandleAllItemsWithInteraction, .isHovered = 0};
    grid.getElement = d3t::cbGetMenuElementPointer;
    grid.rowHeight = nullptr;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    UIGridTemplate &cellTemplate = g_itemsGridTemplate;
    cellTemplate.initialized = 0;
    grid.scrollSpeed = 0.25f;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.layoutFirst = 0;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 0;
    cellTemplate.labels = nullptr;
    cellTemplate.layoutItems = g_itemsGridLayoutItems.data();
    cellTemplate.first = 1;
    cellTemplate.end = 5;
    cellTemplate.clipWindows[0] = 3;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.windows = g_itemsGridWindows.data();
    cellTemplate.ids = kItemsGridWindowIds.data();
    cellTemplate.layoutEnd = 2;
    return 1;
}



void ItemsMenu_Render() {
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    ItemsMenu_PopulateEntries();
    UIGrid_ResetRuntime(g_itemsGrid);
    g_uiGridInputEnabled = 1;
    g_uiFocusedList = nullptr;
    UI_RestartCurrentGridSelection();
}


std::int64_t ItemsMenu_Tick() {
    ItemsMenu_PopulateEntries();
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuItems];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_itemsGrid, element.index);
    return 0;
}


std::int64_t ItemsMenu_IsPressed() {
    return UIGrid_IsPressed(g_itemsGrid);
}




void SharedItemsMenu_DestroyAndResetPreviewState() {
    if (g_sharedItemRequest.requestActive && g_sharedItemsRequested) {
        if (g_sharedItemRequest.requestType == 0) {
            if (g_sharedItemRequest.itemId == 3)
                DestroyTMEX();
            else
                UI_ICLIResetSharedItemMenu();
        } else if (g_sharedItemRequest.requestType == 1) {
            UI_ICLIResetSharedItemMenu();
        } else if (g_sharedItemRequest.itemId == 3) {
            DestroyTMEX();
        }
    }
    g_sharedItemRequest.requestActive = 0;
    g_sharedItemRequest.requestType = 0;
    g_sharedItemRequest.itemId = 0;
    g_sharedItemsRequested = 0;
    if (g_currUIState == -1 || g_currUIState == kUiSharedItems) {
        UIBaseRuntime &sharedItems = g_uiElements[kUiSharedItems];
        if (!sharedItems.isVisible || !sharedItems.destroy || sharedItems.destroy() == 1 || g_busy_reset == 1)
            sharedItems.isVisible = 0;
    }
}


std::int64_t ItemsMenu_Destroy() {
    SharedItemsMenu_DestroyAndResetPreviewState();
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    g_uiGridInputEnabled = 0;
    g_uiFocusedList = nullptr;
    return 1;
}



std::int32_t GetAssignedFightMoveIndex() {
    const auto fightMove = static_cast<std::int8_t>(g_playerStats.currentMove >> 8U);
    if (fightMove < 0)
        return -1;
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::size_t set = fightMove >= 64 ? 2 : fightMove >= 32 ? 1 : 0;
    const std::int32_t moveId = fightMove - static_cast<std::int32_t>(set) * 32;
    std::int32_t offset = 0;
    for (std::size_t preceding = 0; preceding < set; ++preceding)
        offset += static_cast<std::int8_t>(sets[preceding].count);
    for (std::size_t slot = 0; slot < sets[set].moveValue.size(); ++slot) {
        if (static_cast<std::int16_t>(sets[set].moveValue[slot]) == moveId)
            return static_cast<std::int32_t>(slot) + offset;
    }
    return -1;
}





void Notebook_DrawPageText(std::int32_t side, std::uint8_t page, std::uint32_t renderTarget) {
    const char *const text = side == 1 ? g_memoText[page].data() : g_memoTextPages[page];
    if (!text)
        return;
    const float fontScale = 1.0f;
    const float lineHeight = (side == 1 ? 565.0f : 1200.0f) / 10000.0f * 512.0f;
    const float left = 30.0f;
    const float top = side == 1 ? 58.0f : 0.0f;
    if (g_uiLoadedLanguage == -1)
        ReadSubtitleGlyphsAndSubs(0);
    const SubtitleFontLayer &font = g_memoFonts[static_cast<std::size_t>(g_uiLoadedLanguage)];
    const std::uint32_t texture = font.texture.load(std::memory_order_acquire);
    if (!texture)
        return;
    const float fontHeight = font.lineHeight * fontScale;
    constexpr std::uint32_t kMemoTextColour = 0xff081219U;
    std::array<NuklearQuad, 512> quads{};
    std::size_t quadCount = 0;
    const char *cursor = text;
    std::int32_t line = 0;
    while (*cursor) {
        float x = left;
        const float y = static_cast<float>(line) * lineHeight + top + (lineHeight - fontHeight) * 0.5f;
        while (*cursor && *cursor != '\n') {
            const std::uint32_t codepoint = DecodeSubtitleCodepoint(cursor);
            const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, codepoint);
            if (!glyph)
                continue;
            if (glyph->width > 0.0f && glyph->height > 0.0f && quadCount < quads.size()) {
                const float height = glyph->height * fontScale;
                quads[quadCount++] = {.x = x + glyph->x0 * fontScale,
                    .y = 512.0f - (y + glyph->y0 * fontScale) - height,
                    .width = glyph->width * fontScale,
                    .height = height,
                    .u0 = glyph->u0,
                    .v0 = glyph->v1,
                    .u1 = glyph->u1,
                    .v1 = glyph->v0,
                    .colour = kMemoTextColour};
            }
            x += glyph->advance * fontScale;
        }
        if (*cursor == '\n')
            ++cursor;
        ++line;
    }
    if (quadCount)
        DrawNuklearQuads(g_RenderDeviceContext, renderTarget, texture, std::span<const NuklearQuad>{quads.data(), quadCount});
}


std::uint32_t NotebookPages_ParseDefinition() {
    return 3;
}


std::int64_t PrepareNotebookBuffers() {
    g_memoPadDebugMode = g_notebookDebugMode;
    if (g_debugNotebookOpenRequested) {
        g_notebookRequestedEntryPage = g_debugNotebookOpenPage;
        MMTK_BeginManualSession();
        g_hudPromptJinguruIconOverrideActive = 0;
        g_debugNotebookOpenRequested = 0;
    }
    if (g_debugNotebookRedraw) {
        g_notebookRightPageFlags.fill(1);
        g_notebookLeftPageFlags.fill(1);
    }
    for (std::int32_t side = 0; side < 2; ++side) {
        std::array<std::uint32_t, 4> &textures = side == 1 ? g_notebookTextureLeft : g_notebookTextureRight;
        std::array<std::uint8_t, 4> &flags = side == 1 ? g_notebookLeftPageFlags : g_notebookRightPageFlags;
        for (std::uint8_t page = 0; page < 4; ++page) {
            if (flags[page] != 1)
                continue;
            if (BeginNuklearFrame(g_RenderDeviceContext, textures[page]))
                Notebook_DrawPageText(side, page, textures[page]);
            flags[page] = 0;
        }
    }
    return 0;
}


void NotebookPages_Initialize() {
    CreateNotebookTexture2DForPages();
}


std::int64_t InitNotebookTextures() {
    for (std::uint32_t &texture : g_notebookTextureLeft) {
        InitTexture2DResource(texture);
        texture = 0;
    }
    return 1;
}



void UI_SetOrbitLightDirection(Vector3 &position, const Vector3 &cameraEye, std::int16_t yaw) {
    const Vector3 &target = g_CameraTargetPos;
    const float radius = std::sqrt((target.x - cameraEye.x) * (target.x - cameraEye.x) + (target.z - cameraEye.z) * (target.z - cameraEye.z));
    const auto angle = static_cast<std::uint16_t>(g_fCamAccumYaw + yaw);
    const float sourceX = target.x - sin_fixed16(static_cast<std::int16_t>(angle)) * radius;
    const std::uint16_t folded = angle <= 0x8000U ? angle : static_cast<std::uint16_t>(-angle);
    const float sourceZ = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * radius + target.z;
    if (std::fabs(position.x) < 0.00001f)
        position.x = 0.0f;
    if (std::fabs(position.y) < 0.00001f)
        position.y = 0.0f;
    if (std::fabs(position.z) < 0.00001f)
        position.z = 0.0f;
    Vector3 direction{position.x - sourceX, position.y - target.y, position.z - sourceZ};
    const float length = std::sqrt(direction.y * direction.y + direction.x * direction.x + direction.z * direction.z);
    if (length > 0.0f) {
        direction.x *= 1.0f / length;
        direction.y *= 1.0f / length;
        direction.z *= 1.0f / length;
    }
    SetEffectTransformByIndex(31, &direction);
}


void UI_AimObjectLight(const UIDrawObject &object) {
    const ShenmueLightSlot &slot = *LGHT_GetDataSlot(31);
    g_refBoardSavedDirection = slot.direction;
    g_refBoardSavedColour = {slot.colourScale, slot.colourR, slot.colourG, slot.colourB};
    Vector3 position = object.position;
    UI_SetOrbitLightDirection(position, g_CameraEyePos, static_cast<std::int16_t>(object.lightYaw));
    SetRGBStruct(31, &g_effeObjectLightColour);
    Shenmue_InitEffectByIndexExact();
}






void UI_DrawObjectModel(UIDrawObject &object) {
    const std::int32_t *const textures = object.textureIds[0] >= 0 ? object.textureIds.data() : nullptr;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(object.position.x, object.position.y, object.position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(object.rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(object.rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(object.rotation.x));
    Matrix4x4::Scale(object.scale, object.scale, object.scale);
    g_sceneObjectMaxScale = object.scale;
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(object.position.x, object.position.y, object.position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(object.rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(object.rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(object.rotation.x));
    Matrix4x4::Scale(object.scale, object.scale, object.scale);
    Mt5RuntimeNode *const root = static_cast<Mt5RuntimeNode *>(object.model);
    Matrix4x4::preTranslate(root->position.x, root->position.y, root->position.z);
    if (root->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(root->rotation.z));
    if (root->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(root->rotation.y));
    if (root->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(root->rotation.x));
    if (root->scale.x != 1.0f || root->scale.y != 1.0f || root->scale.z != 1.0f)
        Matrix4x4::Scale(root->scale.x, root->scale.y, root->scale.z);
    if (CachedAttachmentOwnsSubmission(object.model, 1.0f, workTransformMatrix)) {
        SubmitRiggedSceneObjectForDrawing(object.model, nullptr, 0, 1);
        Matrix4x4::Pop(1);
        g_effectLightRenderSlots[31].transformedDirection.x *= -1.0f;
        PrepareSceneLightingForRendering();
        ArmSingleLightOverrideForCachedMapModel(root);
    } else {
        Matrix4x4::Pop(1);
        d3t::MT5_PushAndReadNodeTree(object.model, textures, 1.0f);
    }
    g_sceneObjectMaxScale = 1.0f;
    Matrix4x4::Pop(1);
    EFFE_ReplayQueuedEffects(0);
    ComposeEffectABuffer();
    g_bufferPassNotBusy = 1;
    if (g_viewportInterpolationProgress != 0.0f) {
        g_depthPassProgress = g_viewportInterpolationProgress;
        g_renderPassProgress = 0.0f;
    }
    DrawMiddleOfScreenStretchedAdjusted(0x76000000U, -2.0f);
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {


void CreateNotebookTexture2DForPages() {
    const Texture2DDescription description{.width = 512, .height = 512, .arraySize = 1, .mipLevels = 1, .format = 2, .bindFlags = 4, .sampleCount = 1};
    for (std::size_t index = 0; index < 4; ++index) {
        if (!g_notebookTextureLeft[index])
            g_notebookTextureLeft[index] = CreateTexture2D(&description);
        g_notebookLeftPageFlags[index] = 1;
        if (!g_notebookTextureRight[index])
            g_notebookTextureRight[index] = CreateTexture2D(&description);
        g_notebookRightPageFlags[index] = 1;
    }
}



void CreateNotebookTexture2D(std::int32_t surface, std::byte *memoTexture) {
    std::size_t index = 0;
    while (g_notebookMemoTextureData[index] != memoTexture + 0x2c) {
        if (++index >= 4)
            return;
    }
    if (!g_notebookTextureRight[index]) {
        const Texture2DDescription description{.width = 512, .height = 512, .arraySize = 1, .mipLevels = 1, .format = 2, .bindFlags = 4, .sampleCount = 1};
        g_notebookTextureRight[index] = CreateTexture2D(&description);
    }
    g_notebookRightPageFlags[index] = 1;
    IncrementTextureById(g_notebookTextureRight[index]);
    InitTexture2DResource(UI_SetPVRSurfaceTexture(surface, g_notebookTextureRight[index]));
}



void FormatNotebookText(std::uint32_t pageIndex, std::uint32_t textBuffer, std::uint32_t pageSlot, std::uint32_t line, char messageType) {
    char *const stringId = g_notebookTextIds[line + 16U * pageIndex].data();
    const std::uint32_t tens = pageIndex % 100U / 10U;
    std::snprintf(stringId, 11, "MEMP%i%i%iL%i%c", pageIndex / 100U, tens, pageIndex % 100U - 10U * tens, line, messageType);
    char *const memo = g_memoText[textBuffer].data();
    const auto append = [memo](const char *text) {
        const std::size_t length = std::strlen(memo);
        std::snprintf(memo + length, 1280 - length, "%s\n", text);
    };
    switch (g_memoPadDebugMode) {
    case 2:
        append("NOTUSED");
        break;
    case 3:
        append(stringId);
        g_memoTextPages[pageSlot] = memo;
        return;
    case 4: {
        char page[100]{};
        std::snprintf(page, sizeof(page), "TEST: PAGE %d", textBuffer + 1);
        append(page);
        break;
    }
    default: {
        const char *const text = InitAndRetrieveStringFromID(stringId);
        char shortId[11]{};
        std::snprintf(shortId, sizeof(shortId), "%s", text + 2);
        if (messageType == '0' || !std::strcmp("[EMPTY LINE]", text))
            append(" ");
        else if (!std::strcmp(stringId, shortId))
            append(g_memoPadDebugMode == 1 ? "NOTUSED" : " ");
        else
            append(text);
        break;
    }
    }
    g_memoTextPages[pageSlot] = memo;
}

// @verified - p
void CreateNotebookTextures(std::uint64_t, RenderArrayOwner *object) {
    if (object->state != 1)
        return;
    if (!g_notebookTextureLeft[0]) {
        const Texture2DDescription description{.width = 512, .height = 512, .arraySize = 1, .mipLevels = 1, .format = 2, .bindFlags = 4, .sampleCount = 1};
        for (std::uint32_t page = 0; page < 4; ++page) {
            if (!g_notebookTextureLeft[page])
                g_notebookTextureLeft[page] = CreateTexture2D(&description);
            g_notebookLeftPageFlags[page] = 1;
        }
    }
    g_notebookModelHandle = object->renderHandle;
    ModelMaterialDescriptor areas[4]{};
    if (!ResolveModelWritingAreas(g_notebookModelHandle, areas))
        return;
    for (std::uint32_t page = 0; page < 4; ++page) {
        ModelMaterialDescriptor &area = areas[page];
        if (!area.name)
            continue;
        for (std::uint32_t parameter = 0; parameter < area.material->parameterCount; ++parameter) {
            if (area.parameters[parameter].type == kNotebookPageTextureParameter) {
                SetModelMaterialTexture(area, parameter, g_notebookTextureLeft[page]);
                break;
            }
        }
    }
}

// @verified - p
void
// @verified - p
Notebook_BindModelPageTextures() {
    ModelMaterialDescriptor areas[4]{};
    if (!ResolveModelWritingAreas(g_notebookModelHandle, areas))
        return;
    for (std::uint32_t page = 0; page < 4; ++page) {
        ModelMaterialDescriptor &area = areas[page];
        if (!area.name)
            continue;
        for (std::uint32_t parameter = 0; parameter < area.material->parameterCount; ++parameter) {
            if (area.parameters[parameter].type == kNotebookPageTextureParameter) {
                SetModelMaterialTexture(area, parameter, g_notebookTextureLeft[page]);
                break;
            }
        }
    }
}

void InitNotebookTexture2D() {
    for (std::uint32_t &texture : g_notebookTextureLeft) {
        InitTexture2DResource(texture);
        texture = 0;
    }
}


void DrawObjectForUI(UIDrawObject *object) {
    if (!g_updateRenderFrameMode)
        return;
    const std::int32_t lightingMode = g_cachedGlobalLightingAdjustFlag;
    const Vector4 savedLightColour = g_effeObjectLightColour;
    g_effeObjectLightColour = object->lightColour;
    if (lightingMode == 1) {
        g_cachedGlobalLightingAdjustFlag = 2;
        g_effectsNeedInitialization = 1;
    }
    if (LGHT_GetDataSlot(31)->enabled) {
        UI_DrawObjectModel(*object);
    } else {
        lightSetSlotEnabled(31, 0);
        SetLightTypeOrFlagsByIndex(31, 0);
        const Vector4 colour{1.0f, 1.0f, 1.0f, 1.0f};
        const Vector3 position{0.0f, 2.0f, 0.0f};
        const Vector3 direction{0.0f, -1.0f, 0.0f};
        SetRGBStruct(31, &colour);
        SetLightIntensity(31, 1.0f);
        SetLightFalloffRangeByIndex(31, 2.0f);
        SetLightFalloff(31, 0.25f);
        SetLightDirectionVectorByIndex(31, &position);
        SetEffectTransformByIndex(31, &direction);
        SetLightAngleByIndex(31, 0x71c);
        SetLightTypeOrFlagsByIndex(31, (LGHT_GetDataSlot(31)->modeFlags & 8U) != 0 ? 10U : 2U);
        lightSetSlotEnabled(31, 1);
        UI_AimObjectLight(*object);
        UI_DrawObjectModel(*object);
        lightSetSlotEnabled(31, 0);
        SetRGBStruct(31, &g_refBoardSavedColour);
        SetEffectTransformByIndex(31, &g_refBoardSavedDirection);
        Shenmue_InitEffectByIndexExact();
    }
    if (lightingMode == 1) {
        g_cachedGlobalLightingAdjustFlag = 1;
        g_effectsNeedInitialization = 0;
    }
    g_effeObjectLightColour = savedLightColour;
}

void UI_ICONRecomputeInventoryGrid() {
    IconPresentationOwner &icon = g_iconPresentation;
    std::int16_t ownedEntries = 0;
    std::uint16_t ownedItems = 0;
    for (std::uint16_t itemId = 0; itemId < 256U; ++itemId) {
        const std::uint8_t count = itemId <= 234U ? GetInventoryItemCount(itemId) : 0;
        ownedItems = static_cast<std::uint16_t>(ownedItems + count);
        if (count != 0)
            ++ownedEntries;
    }
    icon.inventoryOwnedEntryCount = ownedEntries;
    icon.inventoryOwnedItemCount = ownedItems;

    std::int8_t pages = 1;
    std::int8_t slotsPerPage = 8;
    while (static_cast<std::int16_t>(pages) * slotsPerPage < ownedEntries) {
        ++pages;
        if (pages > 5) {
            pages = 1;
            ++slotsPerPage;
        }
    }
    icon.inventoryPageCount = pages;
    icon.inventorySlotsPerPage = slotsPerPage;

    std::int8_t page = icon.inventorySelectionPage;
    if (page + 1 > pages && static_cast<std::int16_t>(slotsPerPage) * page > ownedEntries) {
        do {
            --page;
        } while (page >= 0 && static_cast<std::int16_t>(slotsPerPage) * page > ownedEntries);
        if (page < 0)
            page = 0;
        icon.inventorySelectionPage = page;
    }

    std::int8_t slot = icon.inventorySelectionSlot;
    while (slot + 1 + static_cast<std::int16_t>(slotsPerPage) * page > ownedEntries) {
        --slot;
        if (slot < 0) {
            slot = static_cast<std::int8_t>(slotsPerPage - 1);
            --page;
            if (page < 0) {
                page = 0;
                slot = 0;
                break;
            }
        }
        if (slot + static_cast<std::int16_t>(slotsPerPage) * page < ownedEntries)
            break;
    }
    icon.inventorySelectionPage = page;
    icon.inventorySelectionSlot = slot;

    if (ownedEntries <= 0 && icon.menuSelection == UIIconSystemMenuSelection::Items) {
        icon.menuSelection = UIIconSystemMenuSelection::Resume;
        icon.inventorySelectionSlot = 0;
    }
    if (icon.inventorySelectionPage + 1 > pages ||
        icon.inventorySelectionSlot + 1 + static_cast<std::int16_t>(slotsPerPage) * icon.inventorySelectionPage > ownedEntries ||
        icon.inventorySelectionSlot < 0 || icon.inventorySelectionPage < 0) {
        icon.inventorySelectionPage = 0;
        icon.inventorySelectionSlot = 0;
    }
    ++icon.inventoryGridRevision;
}

// scnfStartIconSystemMenu (SCNF 28): announce an obtained item (0-234) or cassette (255-282) through the GameMessage popup.
bool UI_ICONStartItemInfoPopup(std::uint16_t itemId) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.itemIconParams.halfScreenY = 55.0f;
    icon.itemInfoPopupPosition[1] = 340.0f;
    icon.itemIconParams.halfScreenX = -672.0f;
    icon.itemInfoPopupNameWidthClass = 6;
    icon.itemInfoPopupPosition[0] = -672.0f;
    const OwnedItemCatalogEntry *entry = nullptr;
    std::array<char, 64> surfaceText{};
    if (static_cast<std::int16_t>(itemId) < static_cast<std::int16_t>(kCassetteCatalogFirstItem)) {
        if (itemId >= kOwnedItemCatalogCount)
            return false;
        icon.itemInfoPopupJinguruType = itemId == 5;
        entry = &g_ownedItemCatalog[itemId];
        std::snprintf(surfaceText.data(), surfaceText.size(), "%s", entry->itemNameId ? entry->itemNameId : "ERROR");
        if (std::strlen(surfaceText.data()) >= 14)
            ++icon.itemInfoPopupNameWidthClass;
    } else {
        const std::uint32_t cassette = static_cast<std::uint32_t>(static_cast<std::int16_t>(itemId) - kCassetteCatalogFirstItem);
        if (cassette >= kCassetteCatalogCount)
            return false;
        entry = &g_cassetteCatalog[cassette].entry;
        std::snprintf(surfaceText.data(), surfaceText.size(), "%s", InitAndRetrieveStringFromID(entry->cassetteNameId));
        icon.itemInfoPopupJinguruType = 0;
    }
    const bool itemName = entry->itemNameId != nullptr;
    const char *const name = InitAndRetrieveStringFromID(itemName ? entry->itemNameId : entry->cassetteNameId);
    GameMessage_SetContent(itemName ? UIGameMessageContentType::Item : UIGameMessageContentType::Cassette, entry->iconIndex, name);
    icon.itemIconParams.pvrSurfaceIndex = entry->iconIndex;
    if (icon.itemInfoSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
    icon.itemInfoSurface = -1;
    icon.itemInfoSurface = AllocatePVRSurface(surfaceText.data(), 512, 0xcccc, 0xf000, 0, true);
    icon.itemInfoPopupPhase = 2;
    icon.itemInfoPopupVisible = 1;
    g_isPlayerGameStateActive = 6;
    icon.overlayMode = 7;

    if (static_cast<std::uint16_t>(itemId - 67U) <= 167U && GetInventoryItemCount(itemId) == 1)
        RefreshSteamStatForItem(static_cast<std::int16_t>(itemId));
    return true;
}



bool UI_ICONAdvanceItemInfoPopup() {
    IconPresentationOwner &icon = g_iconPresentation;
    switch (icon.itemInfoPopupPhase) {
    case 0:
        break;
    case 1:
        if (GameMessageAnimIdle())
            icon.itemInfoPopupPhase = 3;
        break;
    case 2:
        ActivateOwnedElement(kUiGameMessage, "/ui/gamemessage/GameMessage.ui");
        icon.itemInfoPopupPhase = 1;
        icon.itemInfoVoice = ProcessSubtitleFile(icon.itemInfoPopupJinguruType ? "JINGURU02" : "JINGURU01", 0, 1, nullptr);
        break;
    case 3:
        ++icon.itemInfoPopupFrames;
        if (static_cast<std::int8_t>(icon.itemInfoPopupFrames) >= 30 && !CVWK_IsVoiceSlotActive(icon.itemInfoVoice)) {
            icon.itemInfoPopupPhase = 4;
            icon.itemInfoPopupFrames = 0;
            static_cast<void>(ResetOwnedElement(kUiGameMessage));
        }
        break;
    case 4:
        if (GameMessageAnimIdle())
            icon.itemInfoPopupPhase = 5;
        break;
    case 5:
        icon.itemInfoPopupPosition[0] = -672.0f;
        icon.itemInfoPopupPosition[1] = 55.0f;
        icon.itemInfoPopupPhase = 0;
        if (icon.itemInfoSurface != -1)
            PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
        icon.itemInfoPopupVisible = 0;
        icon.itemInfoSurface = -1;
        icon.overlayMode = 0;
        g_isPlayerGameStateActive = 7;
        return true;
    default:
        icon.itemInfoPopupPhase = 5;
        break;
    }
    if (icon.itemInfoPopupVisible && icon.itemInfoSurface != -1)
        g_uiElements[kUiGameMessage].isActive = 1;
    return icon.itemInfoPopupPhase == 0;
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

// Ryo's yaw (the summed node yaws, negated) and his position, as the system menu's camera move reads them each time: the eye goes to his
// head, the target 0.2 in front of it.
void IconSystemMenu_AimCameraAtRyo(IconSystemMenuCameraTransition &transition, Character &ryo) {
    std::int16_t yawSum = 0;
    for (CharacterNode *node = &ryo.entity.node; node; node = node->child)
        yawSum = static_cast<std::int16_t>(yawSum + static_cast<std::int16_t>(node->rotation.y));
    const std::int32_t yaw = -yawSum;
    const Vector3 position = GetPositionForEntity(ryo.entity.node);
    const float eyeY = static_cast<float>(static_cast<double>(position.y) + 1.6);
    transition.destinationEye = {position.x, eyeY, position.z};
    transition.destinationTarget.y = eyeY;
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const std::int16_t magnitude = static_cast<std::uint32_t>(yaw) <= 0x8000U ? static_cast<std::int16_t>(yaw) : static_cast<std::int16_t>(-yaw);
    transition.destinationTarget.x = static_cast<float>(sine * 0.2 + position.x);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude));
    transition.destinationTarget.z = static_cast<float>(cosine * -0.2 + static_cast<double>(position.z));
}

void IconSystemMenu_ApplyCamera(const IconSystemMenuCameraTransition &transition) {
    TranslateCameraRelative(transition.eye);
    if (!bLockCamera) {
        g_CameraTargetPos = transition.target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
}


void shenBeginIconSystemMenuCameraTransition(IconSystemMenuCameraTransition &transition) {
    Character *const ryo = MapRyo();
    if (!ryo)
        return;
    transition.frameCount = 8;
    transition.frame = 0;
    transition.mode = 2;
    transition.savedEye = g_CameraEyePos;
    transition.savedTarget = g_CameraTargetPos;
    transition.savedRyoOpacity = ryo->renderOpacity;
    transition.eye = transition.savedEye;
    transition.target = transition.savedTarget;
    IconSystemMenu_AimCameraAtRyo(transition, *ryo);
    const float frames = static_cast<float>(transition.frameCount);
    transition.eyeStep = {(transition.destinationEye.x - transition.eye.x) / frames, (transition.destinationEye.y - transition.eye.y) / frames,
        (transition.destinationEye.z - transition.eye.z) / frames};
    transition.targetStep = {(transition.destinationTarget.x - transition.target.x) / frames, (transition.destinationTarget.y - transition.target.y) / frames,
        (transition.destinationTarget.z - transition.target.z) / frames};
    IconSystemMenu_ApplyCamera(transition);
    transition.ryoOpacity = transition.savedRyoOpacity;
    transition.ryoOpacityStep = transition.savedRyoOpacity / static_cast<float>(transition.frameCount / 2) * -1.0f;
    ryo->renderOpacity = transition.savedRyoOpacity;
}


void RestoreIconSystemMenuCameraTransition(IconSystemMenuCameraTransition &transition) {
    Character *const ryo = MapRyo();
    if (!ryo)
        return;
    transition.destinationEye = transition.savedEye;
    transition.destinationTarget = transition.savedTarget;
    transition.frameCount = 8;
    transition.frame = 0;
    transition.mode = 3;
    transition.eyeStep = {(transition.destinationEye.x - transition.eye.x) * 0.125f, (transition.destinationEye.y - transition.eye.y) * 0.125f,
        (transition.destinationEye.z - transition.eye.z) * 0.125f};
    transition.targetStep = {(transition.destinationTarget.x - transition.target.x) * 0.125f, (transition.destinationTarget.y - transition.target.y) * 0.125f,
        (transition.destinationTarget.z - transition.target.z) * 0.125f};
    IconSystemMenu_ApplyCamera(transition);
    transition.ryoOpacityStep = transition.savedRyoOpacity / static_cast<float>(transition.frameCount / 2);
    ryo->renderOpacity = transition.ryoOpacity;
}


bool HandleRyoCameraAlpha(IconSystemMenuCameraTransition &transition) {
    Character *const ryo = MapRyo();
    if (!ryo || g_searchCandidateId >= 0 || (((g_currentSearchState - 2) & ~3) == 0 && g_currentSearchState != 4 && g_searchCandidateId < 0))
        return true;
    bool finished = false;
    if (++transition.frame <= transition.frameCount) {
        if (transition.mode == 2) {
            IconSystemMenu_AimCameraAtRyo(transition, *ryo);
            std::int16_t remaining = static_cast<std::int16_t>(transition.frameCount - transition.frame);
            if (remaining <= 0)
                remaining = 1;
            const float frames = remaining;
            transition.eyeStep = {(transition.destinationEye.x - transition.eye.x) / frames, (transition.destinationEye.y - transition.eye.y) / frames,
                (transition.destinationEye.z - transition.eye.z) / frames};
            transition.targetStep = {(transition.destinationTarget.x - transition.target.x) / frames,
                (transition.destinationTarget.y - transition.target.y) / frames, (transition.destinationTarget.z - transition.target.z) / frames};
        }
        transition.eye = {transition.eye.x + transition.eyeStep.x, transition.eye.y + transition.eyeStep.y, transition.eye.z + transition.eyeStep.z};
        transition.target = {
            transition.target.x + transition.targetStep.x, transition.target.y + transition.targetStep.y, transition.target.z + transition.targetStep.z};
        const float opacity = transition.ryoOpacity + transition.ryoOpacityStep;
        transition.ryoOpacity = opacity < 0.0f ? 0.0f : opacity > 1.0f ? 1.0f : opacity;
    } else {
        finished = true;
        transition.eye = transition.destinationEye;
        transition.target = transition.destinationTarget;
        transition.ryoOpacity = transition.mode ? 0.0f : transition.savedRyoOpacity;
    }
    IconSystemMenu_ApplyCamera(transition);
    ryo->renderOpacity = transition.ryoOpacity;
    return finished;
}


std::int32_t IconSystemMenu_FindSelectedInventoryItem() {
    const IconPresentationOwner &icon = g_iconPresentation;
    const std::int16_t selected = static_cast<std::int16_t>(icon.inventorySelectionSlot + icon.inventorySelectionPage * icon.inventorySlotsPerPage);
    std::int16_t owned = 0;
    for (std::int32_t item = 0; item < 256; ++item) {
        if (!GetInventoryItemCount(static_cast<std::uint16_t>(item)))
            continue;
        if (owned == selected)
            return item;
        ++owned;
    }
    return -1;
}


void icsClampSelectionPage() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (icon.inventoryOwnedEntryCount <= 0 && icon.menuSelection == UIIconSystemMenuSelection::Items) {
        icon.menuSelection = UIIconSystemMenuSelection::Resume;
        icon.inventorySelectionSlot = 0;
    }
    if (icon.inventorySelectionPage + 1 > icon.inventoryPageCount) {
        icon.inventorySelectionPage = 0;
        icon.inventorySelectionSlot = 0;
    }
    if (icon.inventorySelectionSlot + 1 + icon.inventorySelectionPage * icon.inventorySlotsPerPage > icon.inventoryOwnedEntryCount ||
        icon.inventorySelectionSlot < 0 || icon.inventorySelectionPage < 0) {
        icon.inventorySelectionPage = 0;
        icon.inventorySelectionSlot = 0;
    }
}


void icsmPrepareInfoSurface() {
    IconPresentationOwner &icon = g_iconPresentation;
    const std::int32_t item = IconSystemMenu_FindSelectedInventoryItem();
    if (icon.itemInfoSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
    icon.itemInfoSurface = -1;
    if (icon.menuSelection == UIIconSystemMenuSelection::Items) {
        if (item >= 0 && item < 235) {
            const char *const token = g_ownedItemCatalog[static_cast<std::size_t>(item)].itemNameId;
            icon.itemInfoSurface = AllocatePVRSurface(token ? token : "ERROR", 512, 0xcccc, 0xf000, 0, true);
        }
        return;
    }
    static constexpr std::array<const char *, 7> kMenuItems{"Resume", "Settings", "Moves Scroll", "Notebook", "Collection", "Cassette Tapes", "Tokens"};
    static constexpr std::array<const char *, 10> kDateNumbers{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    static constexpr std::array<const char *, 12> kDateMonths{"Jan", "Feb", "Mar", "Apr", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Nov", "Dec"};
    static constexpr std::array<const char *, 7> kDayNames{"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
    std::array<char, 40> message{};
    if (icon.menuSelection == UIIconSystemMenuSelection::Date) {
        std::array<char, 8> dateNumber{};
        const std::int8_t day = static_cast<std::int8_t>(g_currentDateTime.day);
        if (day >= 10) {
            std::snprintf(dateNumber.data(), dateNumber.size(), "%s%s", kDateNumbers[day / 10], kDateNumbers[day % 10]);
        } else {
            std::snprintf(dateNumber.data(), dateNumber.size(), "%s", kDateNumbers[day]);
        }
        const char *const month = kDateMonths[static_cast<std::int8_t>(g_currentDateTime.month) - 1];
        const char *const weekday = kDayNames[static_cast<std::int8_t>(g_currentDateTime.dayOfWeek)];
        if (month)
            std::snprintf(message.data(), message.size(), "%s %s,(%s)", month, dateNumber.data(), weekday);
        else
            std::snprintf(message.data(), message.size(), "%s,%s (%s)", "ERR", dateNumber.data(), weekday);
    } else {
        std::snprintf(message.data(), message.size(), "%s", kMenuItems[static_cast<std::uint8_t>(icon.menuSelection)]);
    }
    icon.itemInfoSurface = AllocatePVRSurface(message.data(), 512, 0xcccc, 0xf000, 0, true);
}


void HandleCassetteItemEvents() {
    IconPresentationOwner &icon = g_iconPresentation;
    const std::int16_t item = icon.sharedItemPreviewValue;
    if (icon.menuSelection == UIIconSystemMenuSelection::Items && item == -1) {
        icon.cassetteItemState = 0;
        shenExecuteAICACommand(kUiRejectSound, 0, 0);
        return;
    }
    switch (icon.cassetteItemState) {
    case 2:
        g_playerStats.cassetteUtilityResult = static_cast<std::int32_t>((static_cast<std::uint32_t>(static_cast<std::uint16_t>(item)) << 16U) | 2U);
        if (item == 51) {
            g_playerStats.runtimeFlags ^= 4U;
        } else if (static_cast<std::uint16_t>(item - 52) <= 1U) {
            // Shenmue_LoadAudioFile, the ReadAudioFromTable_0 wait and PlayDialogueByVoiceCode, through the wrapper's modes 1 and 0.
            LoadAudioFile_Wrapper(1, "E1079");
            ProcessSubtitleFile("E1079A001", 0, 0, nullptr);
            LoadAudioFile_Wrapper(0, "E1079");
            g_playerStats.batteryLives[item == 52 ? 0 : 1] = 28800;
            RemoveInventoryItemCount(item == 52 ? 52 : 53, 1);
        }
        icon.menuState = UIIconSystemMenuState::CloseRequested;
        break;
    case 4:
        UI_ICLILoadSharedItemPreview(item);
        icon.cassetteItemState = 0;
        return;
    case 8:
        icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
        icon.cassetteItemState = 0;
        return;
    default:
        break;
    }
    icon.cassetteItemState = 0;
}



void IconSystemMenu_UpdateMenuCloseState() {
    IconPresentationOwner &icon = g_iconPresentation;
    const std::uint8_t state = static_cast<std::uint8_t>(icon.menuState);
    bool keep = static_cast<std::uint8_t>(state - 5U) <= 2U;
    if (!keep && (g_currUIState == -1 || g_currUIState == kUiGameMenu)) {
        UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
        if (gameMenu.tick && g_uiDebugElementDispatch == 1) {
            g_uiRegisteredCallbackRequests[0] |= 1U << kUiGameMenu;
            keep = true;
        } else if (gameMenu.unknown28) {
            g_uiRegisteredCallbackRequests[0] |= 1U << kUiGameMenu;
            keep = gameMenu.unknown28() != 0;
        }
    }
    if (!keep && !g_uiDebugElementDispatch)
        icon.menuState = UIIconSystemMenuState::CloseRequested;
    if (icon.cassetteItemState == 2)
        HandleCassetteItemEvents();
}

// The ICSM_ClosingTeardown/ClosingTransition end: the saved author and temporary game states return and ICON's stage 5 follows.
void IconSystemMenu_FinishClose() {
    IconPresentationOwner &icon = g_iconPresentation;
    g_temporaryGameState = icon.cameraTransition.savedTemporaryGameState;

    ApplyPendingGameState(g_previousAuthorState);
    SetNewAuthorGameState(icon.cameraTransition.savedAuthorState);
    icon.cameraTransition.mode = 0;
    icon.menuState = UIIconSystemMenuState::Closed;
    icon.menuCloseRequested = false;
    icon.overlayTransitionStage = 5;
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {


void UI_ICISCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.menuTask = CurrentTask;
    const std::int32_t queueIndex = CurrentTask ? CurrentTask->taskIndex : 15;
    icon.itemLookTask = HLib_EnqueueTaskWithoutParameter(UI_ICLICallback, queueIndex, 0, SHEN_NAME(ICLI));
    icon.systemStateTask = HLib_EnqueueTaskWithoutParameter(UI_ICSSCallback, queueIndex, 0, SHEN_NAME(ICSS));
    // ICIS_Init's item-info fields.
    ICIS_Init();
    CollectionGrid_Reset();
    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
    icon.inventorySlotsPerPage = 8;
    icon.menuState = UIIconSystemMenuState::Closed;
    g_playerStats.cassetteUtilityResult = 0;
    icon.inventoryOwnedEntryCount = 0;
    icon.inventorySelectionPage = 0;
    icon.inventorySelectionSlot = 0;
    icon.inventoryPageCount = 1;
    icon.menuSelection = UIIconSystemMenuSelection::Items;
    icon.menuCloseRequested = false;
    icon.pendingState = 0;
    icon.itemInfoPopupPhase = 0;
    icon.bufferedInitialPressMask = 0;
    icon.bufferedFollowupPressMask = 0;
    icon.cameraTransition.mode = 0;
    InstallOwnedTaskMain(UI_ICISMainCallback);
}


void UI_ICISMainCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    Character *const ryo = MapRyo();
    if (ryo && icon.menuState != UIIconSystemMenuState::Closed && g_pControlPadController && g_pControlPadController->controlMode == 2)
        ryo->skipRootMotionDelta = 1;
    if ((Shenmue_nPauseState & 2) == 0) {
        if (icon.menuState == UIIconSystemMenuState::OpeningTransition) {
            const std::uint16_t pressed = static_cast<std::uint16_t>(g_sfxInputFlags & 0x202U);
            if ((pressed & 0x200U) != 0) {
                if (icon.bufferedFollowupPressMask)
                    icon.bufferedFollowupPressMask = 0;
                else if (icon.bufferedInitialPressMask)
                    icon.bufferedInitialPressMask = 0;
            } else if ((pressed & 2U) != 0) {
                if (!icon.bufferedInitialPressMask)
                    icon.bufferedInitialPressMask = pressed;
                else if (!icon.bufferedFollowupPressMask)
                    icon.bufferedFollowupPressMask = pressed;
            }
        }
        switch (icon.menuState) {
        case UIIconSystemMenuState::OpenRequested:
            icon.menuState = UIIconSystemMenuState::OpeningTransition;
            icon.overlayMode = 5;
            icon.menuCloseRequested = false;
            if (ryo) {
                icon.savedRyoMotionVisible = (ryo->motionFlags >> 2U) & 1U;
                icon.savedRyoFlag10 = static_cast<std::int32_t>((ryo->flags.value >> 4U) & 1U);
            }
            icon.cameraTransition.savedAuthorState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
            icon.itemInfoPopupPosition[0] = -672.0f;
            icon.itemInfoPopupPosition[1] = 55.0f;
            icon.cameraTransition.mode = 1;
            icon.cameraTransition.savedTemporaryGameState = g_temporaryGameState;
            SetNewAuthorGameState(9);
            g_temporaryGameState = 0;
            ApplyPendingGameState(g_previousAuthorState);
            shenBeginIconSystemMenuCameraTransition(icon.cameraTransition);
            icsClampSelectionPage();
            icsmPrepareInfoSurface();
            icon.pendingState = 1;
            break;
        case UIIconSystemMenuState::OpeningTransition:
            if (!HandleRyoCameraAlpha(icon.cameraTransition))
                break;
            icon.iconFlags |= 0x20U;
            icon.savedHelpToken = g_defaultHelpTokenId;
            icon.savedHelpButton = g_helpButtonId;
            icon.savedSicnPausePolicy = g_currentSicn;
            icon.savedSicnState = g_uiState;
            icon.savedSicnTaskActive = UI_SICNIsTaskActive();
            g_previousHelpTokenId = g_currentHelpTokenId;
            icon.itemInfoPopupPosition[0] = -55.0f;
            icon.menuState = UIIconSystemMenuState::Visible;
            g_defaultHelpTokenId = 17;
            g_currentHelpTokenId = 17;
            g_helpButtonId = 0;
            g_currentHelpIconState = 0;
            if (UI_SICNIsTaskActive())
                UI_SICNSetState(0x27, UISicnPausePolicy::KeepOnPause);
            else
                UI_SICNEnqueue(39, UISicnPausePolicy::KeepOnPause);
            d3t::ActivateElementByIdx(kUiGameMenu);
            if (icon.bufferedInitialPressMask) {
                const std::uint16_t selection = static_cast<std::uint8_t>(icon.menuSelection);
                if (icon.menuSelection == UIIconSystemMenuSelection::Items || IconSystemMenu_FindSelectedInventoryItem() == 3) {
                    const std::int32_t item = IconSystemMenu_FindSelectedInventoryItem();
                    if (item >= 0 && item < kOwnedItemCatalogCount && g_ownedItemCatalog[static_cast<std::size_t>(item)].modelName)
                        icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
                } else if (static_cast<std::uint16_t>(selection - 6U) > 1U) {
                    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
                }
            }
            break;
        case UIIconSystemMenuState::Visible:
            if (icon.overlayMode == 5 && icon.systemGameState == UIIconSystemGameState::SystemMenuIdle && g_cassetteCollectionUiState == 0) {
                if (icon.menuCloseRequested) {
                    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
                    icon.menuState = UIIconSystemMenuState::CloseRequested;
                } else {
                    IconSystemMenu_UpdateMenuCloseState();
                }
            }
            break;
        case UIIconSystemMenuState::ClosingTeardown:
            icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
            icon.pendingState = 0;
            if (icon.cameraTransition.savedAuthorState) {
                RestoreIconSystemMenuCameraTransition(icon.cameraTransition);
                icon.menuState = UIIconSystemMenuState::ClosingTransition;
            } else {
                g_bIsConversationCamInactive = 0;
                IconSystemMenu_FinishClose();
            }
            if (icon.itemInfoSurface != -1)
                PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
            icon.itemInfoSurface = -1;
            g_previousHelpTokenId = g_currentHelpTokenId;
            g_currentHelpIconState = 0;
            g_defaultHelpTokenId = icon.savedHelpToken < 68 ? icon.savedHelpToken : 0;
            g_currentHelpTokenId = g_defaultHelpTokenId;
            g_helpButtonId = icon.savedHelpButton;
            break;
        case UIIconSystemMenuState::CloseRequested:
            icon.iconFlags |= 1U;
            icon.menuState = UIIconSystemMenuState::ClosingTeardown;
            if (icon.savedSicnTaskActive)
                UI_SICNSetState(icon.savedSicnState, icon.savedSicnPausePolicy);
            else
                UI_SICNClearState(static_cast<std::uint16_t>(((~g_uiState) & 0x200U) | 0x0dffU));
            break;
        case UIIconSystemMenuState::ClosingTransition:
            if (icon.cameraTransition.mode && HandleRyoCameraAlpha(icon.cameraTransition))
                IconSystemMenu_FinishClose();
            break;
        case UIIconSystemMenuState::Closed:
        case UIIconSystemMenuState::Unused1:
            break;
        }
    }
    if ((icon.iconFlags & 0x20U) != 0) {
        UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
        gameMenu.isActive = g_cassetteCollectionUiState == 0 ? 1 : gameMenu.isActive;
    }
}


void UI_ICSSCallback(HLib::TaskData *) {
    g_iconPresentation.systemStateTask = CurrentTask;
    g_iconPresentation.systemGameState = UIIconSystemGameState::SystemMenuIdle;
    InstallOwnedTaskMain(UI_ICSSMainCallback);
}




void UI_ICSSMainCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    switch (icon.systemGameState) {
    case UIIconSystemGameState::Timex:
        TMEX_Callback(nullptr);
        break;
    case UIIconSystemGameState::Collection:
        if (CollectionGrid_Tick())
            icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
        break;
    case UIIconSystemGameState::MovesScreen:
        if (static_cast<std::uint8_t>(HandleMovesScreenUi()))
            icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
        break;
    case UIIconSystemGameState::SaveDisplay:
        if (g_saveDisplayModuleState == 0) {
            icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
            if (g_timeValue)
                g_shouldTimeAdvance = 1;
        }
        break;
    case UIIconSystemGameState::Notebook:
        if (ProcessNotebookStates()) {
            icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
            icon.savedSicnState &= static_cast<std::uint16_t>(~0x200U);
        }
        break;
    case UIIconSystemGameState::EasyOptions:
        if (g_easyOptionState.state == 0)
            icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
        break;
    case UIIconSystemGameState::SystemMenuIdle:
        return;
    case UIIconSystemGameState::SystemMenuRefresh:
        icon.iconFlags |= 0x20U;
        icon.systemGameState = UIIconSystemGameState::SystemMenuIdle;
        icsmPrepareInfoSurface();
        break;
    default:
        icon.systemGameState = UIIconSystemGameState::SystemMenuRefresh;
        break;
    }
}

bool UI_ICLISharedItemMenuIsActive() {
    const IconPresentationOwner &icon = g_iconPresentation;
    return icon.sharedItemMenuState != 0;
}




void UI_ICLIResetSharedItemMenu() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (icon.itemLookModel) {
        FreePVR(icon.itemLookModel);
        icon.itemLookModel = nullptr;
        icon.itemLookModelRoot = nullptr;
    }
    icon.itemLookEntry = nullptr;
    icon.cassetteItemState = 0;
    if (!icon.sharedItemMenuState)
        return;

    if (g_collectionGridPhase == CollectionGridPhase::Inactive && g_isPlayerGameStateActive == 0) {
        icon.iconFlags |= 0x20U;
        icon.overlayMode = 5;
    }
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_currentHelpIconState = 0;
    g_defaultHelpTokenId = icon.itemLookSavedHelpToken < 68 ? icon.itemLookSavedHelpToken : 0;
    g_currentHelpTokenId = g_defaultHelpTokenId;
    g_helpButtonId = icon.itemLookSavedHelpButton;
    if (icon.itemLookSavedSicnTaskActive)
        UI_SICNSetState(icon.itemLookSavedSicnState, icon.itemLookSavedSicnPausePolicy);
    else
        UI_SICNClearState(static_cast<std::uint16_t>(((~g_uiState) & 0x200U) | 0x0dffU));
    icon.itemLookScriptOpened = false;
    CAME_SetFieldOfView(icon.itemLookSavedFov);
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = icon.itemLookSavedAuthorState;
    ApplyPendingGameState(g_previousAuthorState);
    g_temporaryGameState = icon.itemLookSavedTemporaryGameState;
    ApplyPendingGameState(g_previousAuthorState);
    TranslateCameraRelative(icon.itemLookSavedEye);
    if (!bLockCamera) {
        g_CameraTargetPos = icon.itemLookSavedTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    if (Character *const ryo = MapRyo()) {
        ryo->motionFlags = static_cast<std::uint8_t>(icon.itemLookSavedRyoVisible ? ryo->motionFlags | 4U : ryo->motionFlags & ~4U);
        ryo->flags.value = icon.itemLookSavedRyoRenderFlag ? ryo->flags.value | 0x10U : ryo->flags.value & ~0x10U;
    }
    if (icon.itemLookInput && (icon.itemLookInput->flags & 2U) != 0)
        shenUpdateInputsInto(icon.itemLookPreviousInput);
    icon.sharedItemMenuState = 0;
    EnqueueEWZO(1, 0, 0);
    shenExecuteAICACommand(kUiNavigateSound, 0, 0);
    if (g_userResponse) {
        UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
        gameMenu.isHandledExternally = 0;
        if (gameMenu.elementDraw)
            gameMenu.elementDraw();
        DestroySharedItemsMenu();
        g_inputState.slots[0].pressedPriority = 0;
    }
    g_userResponse = 0;
    UIBaseRuntime &battery = g_uiElements[kUiGameHudBattery];
    if (!g_uiElements[kUiSubMenuCassettes].isVisible && (g_currUIState == -1 || g_currUIState == kUiGameHudBattery) &&
        (!battery.isVisible || !battery.destroy || battery.destroy() == 1 || g_busy_reset == 1)) {
        battery.isVisible = 0;
    }
}


bool UI_ICLILoadSharedItemPreview(std::int16_t itemId) {
    return Load3DCassetteModelForInventory(itemId, false);
}


bool UI_ICLISetSharedItemPreview(std::int16_t itemId) {
    IconPresentationOwner &icon = g_iconPresentation;
    if (g_isPlayerGameStateActive != 7 || icon.sharedItemMenuState)
        return false;
    const bool loaded = Load3DCassetteModelForInventory(itemId, false);
    icon.itemLookScriptOpened = icon.itemLookScriptOpened || loaded;
    return loaded;
}


void UI_ICLICallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.itemLookTask = CurrentTask;
    icon.sharedItemMenuState = 0;
    icon.itemLookInput = AllocateClearedPADCEntry();
    icon.sharedItemPreviewValue = -1;
    icon.itemLookModelRoot = nullptr;
    icon.itemLookModel = nullptr;
    icon.itemLookEntry = nullptr;
    icon.sharedItemPreviewIsCassette = false;
    icon.itemLookLightYaw = 0;
    icon.itemLookScriptOpened = false;
    InstallOwnedTaskMain(UI_ICLIMainCallback);
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {




const char *ItemLook_ExamineRemark(std::int16_t itemId, std::uint32_t &move) {
    const ItemRemarkEntry *remark = nullptr;
    for (const ItemRemarkEntry &entry : kItemRemarks) {
        if (entry.itemId == itemId) {
            remark = &entry;
            break;
        }
    }
    if (!remark)
        return nullptr;
    std::uint16_t set = 0;
    std::int16_t moveId = -1;
    if (remark->move != -1) {
        set = remark->move >= 64 ? 3 : remark->move >= 32 ? 2 : 1;
        moveId = static_cast<std::int16_t>(remark->move - (set == 3 ? 64 : set == 2 ? 32 : 0));
        if (moveId < kMoveSetMoveCounts[static_cast<std::size_t>(set - 1)] && SYGH_GetMoveSkill(set, static_cast<std::uint16_t>(moveId)) >= 12)
            return nullptr;
    }
    std::int32_t flag = -1;
    if (remark->itemId == 35)
        flag = 0x223;
    else if (remark->itemId == 21)
        flag = 0x225;
    else if (remark->itemId == 36)
        flag = 0x104;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const float chance = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
    bool teach = true;
    std::uint8_t row = remark->clearRow;
    if (flag != -1 && (g_shenmueFlags[static_cast<std::size_t>(flag >> 3)] & (1U << (flag & 7))) != 0)
        row = remark->setRow;
    else if (flag != -1)
        teach = false;
    const std::array<const char *, 3> &lines = kItemRemarkLines[row];
    const char *const line = chance > 0.33f ? (chance > 0.66f ? lines[2] : lines[1]) : lines[0];
    if (!teach || remark->move == -1) {
        move = 0xffffffffU;
    } else {
        move = set | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(moveId)) << 16U);
        SYGH_SetMoveSkill(set, static_cast<std::uint16_t>(moveId), 12);
    }
    return line;
}


bool LoadItem() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (!icon.itemLookEntry || !icon.itemLookEntry->modelName)
        return false;
    std::array<char, 32> filename{};
    std::snprintf(filename.data(), filename.size(), "%sG.MT5", icon.itemLookEntry->modelName);
    void *const file = ReadFile("model/item", filename.data(), SHEN_NAME(ITEM));
    if (!file)
        return false;
    std::array<char, 260> path{};
    std::snprintf(path.data(), path.size(), "/%s/%sG/%s", "model/item", icon.itemLookEntry->modelName, filename.data());
    PackModel *const model = MT5_InitRawModelAndTextures(file, filename.data(), path.data());
    if (!model) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return false;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), path.data(), 0, 34);
    icon.itemLookModel = model;
    SetupEngineObject(reinterpret_cast<std::uintptr_t>(model->model), reinterpret_cast<std::uintptr_t>(model->model), 8);
    icon.itemLookModelRoot = static_cast<Mt5RuntimeNode *>(model->model);
    return icon.itemLookModelRoot != nullptr;
}




void ItemLook_Place() {
    IconPresentationOwner &icon = g_iconPresentation;
    Character *const ryo = MapRyo();
    if (!ryo)
        return;
    if (!icon.itemLookEntry) {
        UI_ICLIResetSharedItemMenu();
        return;
    }
    icon.itemLookZoomMin = static_cast<float>(icon.itemLookEntry->previewZoomMin) / 1000.0f;
    icon.itemLookZoomMax = static_cast<float>(icon.itemLookEntry->previewZoomMax) / 1000.0f;
    icon.itemLookZoom = (icon.itemLookZoomMax + icon.itemLookZoomMin) * 0.5f;
    const std::int16_t facing = static_cast<std::int16_t>(-charaGetWorldYaw(ryo));
    Vector3 eye = GetPositionForEntity(ryo->entity.node);
    icon.itemLookPosition = eye;
    Mt5RuntimeNode &root = *icon.itemLookModelRoot;
    bool tipped = true;
    if (!icon.sharedItemPreviewIsCassette) {
        const std::int16_t item = icon.sharedItemPreviewValue;
        if (item >= 43 && item <= 50) {
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
            root.rotation.z = static_cast<std::int32_t>(control->direction) + g_fCamAccumYaw;
            tipped = false;
        } else if (item >= 67) {
            root.rotation.y -= 5461;
            icon.itemLookLightYaw = 10922;
            tipped = false;
        } else if (static_cast<std::uint16_t>(item - 35) <= 1U) {
            icon.itemLookLightYaw = 0;
            tipped = false;
        }
    }
    if (tipped) {
        root.rotation.y += 5461;
        root.rotation.x -= 3640;
    }
    root.rotation.x &= 0xffff;
    root.rotation.y &= 0xffff;
    root.rotation.z &= 0xffff;
    icon.itemLookYaw = static_cast<std::uint16_t>(-facing);
    icon.itemLookPosition.y += 1.6f;
    const std::int16_t behind = static_cast<std::int16_t>(facing + 0x8000);
    const float sine = sin_fixed16(behind);
    const std::uint16_t angle = static_cast<std::uint16_t>(facing + 0x8000);
    const std::int16_t magnitude = angle <= 0x8000U ? behind : static_cast<std::int16_t>(-static_cast<std::int16_t>(angle));
    icon.itemLookPosition.x += sine * 0.15f;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - magnitude));
    icon.itemLookPosition.z -= cosine * 0.15f;
    eye.y += 1.6f;
    eye.x += sine * 0.35f;
    eye.z -= cosine * 0.35f;
    TranslateCameraRelative(eye);
}

// @verified - p
void DrawBatteryIcon(const std::int32_t *batteryId) {
    if (!g_updateRenderFrameMode)
        return;
    ScreenSpaceParams sprite;
    sprite.angle = 0;
    sprite.flags14 = 0;
    sprite.pvrSurfaceIndex = 311;
    sprite.halfScreenX = 32.0f;
    sprite.halfScreenY = 418.0f;
    sprite.positionScaleX = *batteryId == 0 ? 1.0f : 0.75f;
    sprite.positionScaleY = 1.0f;
    sprite.alignmentType = 5;
    sprite.scaledWidth = 64;
    sprite.scaledHeight = 64;
    sprite.depthOrScale = 1.0f;
    const std::int8_t index = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(*batteryId));
    if (g_playerStats.batteryLives[index] >= 28801U)
        g_playerStats.batteryLives[index] = 28800;
    AdjustScreenSpaceWithParams(&sprite, 0xff000000U, 0);
}

void DrawBatteryIcons() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (!icon.itemLookModelRoot)
        return;
    UIDrawObject object{.model = icon.itemLookModelRoot,
        .position = icon.itemLookPosition,
        .rotation = {0, icon.itemLookYaw, 0},
        .scale = icon.itemLookZoom,
        .lightColour = {2.0f, 0.5f, 0.5f, 0.5f},
        .lightYaw = icon.itemLookLightYaw,
        .textureIds = {}};
    object.textureIds.fill(-1);
    DrawObjectForUI(&object);
    if (!icon.sharedItemPreviewIsCassette) {
        std::int32_t batteryId;
        if (icon.sharedItemPreviewValue == 51)
            batteryId = 0;
        else if (icon.sharedItemPreviewValue == 1)
            batteryId = 1;
        else
            return;
        DrawBatteryIcon(&batteryId);
    }
}

void CassetteMenu_HandleInput() {
    IconPresentationOwner &icon = g_iconPresentation;
    Mt5RuntimeNode &root = *icon.itemLookModelRoot;
    std::int32_t step = 256;
    float *slide = nullptr;
    std::int32_t *turn = &root.rotation.y;
    const float zoomStep =
        (static_cast<float>(icon.itemLookEntry->previewZoomMax) / 1000.0f - static_cast<float>(icon.itemLookEntry->previewZoomMin) / 1000.0f) / 50.0f;
    const std::uint16_t pressed = static_cast<std::uint16_t>(icon.bufferedFollowupPressMask | (icon.itemLookInput ? icon.itemLookInput->pressedPriority : 0U));
    std::uint16_t held = icon.itemLookInput ? icon.itemLookInput->held : 0U;
    icon.bufferedFollowupPressMask = 0;
    const std::int16_t item = icon.sharedItemPreviewValue;
    if (static_cast<std::uint16_t>(item) <= 66U && (g_playerStats.ownedItemMenuFlags0To66[static_cast<std::size_t>(item)] & 2U) != 0 &&
        !icon.itemLookScriptOpened && (pressed & 0x400U) != 0) {
        if (!icon.sharedItemPreviewIsCassette && item == 1 && CountOwnedCassetteTapes()) {
            if ((g_shenmueFlags[63] & 0x40U) == 0 || (g_shenmueFlags[65] & 0x20U) != 0) {
                const std::int32_t response = g_userResponse;
                g_userResponse = 0;
                shenPrepareCassetteUI();
                UI_ICLIResetSharedItemMenu();
                g_userResponse = response;
            } else {
                // Shenmue_LoadAudioFile, the ReadAudioFromTable_0 wait and PlayDialogueByVoiceCode, through the wrapper's modes 1 and 0.
                LoadAudioFile_Wrapper(1, "E1060");
                icon.itemLookRemark = ProcessSubtitleFile("E1060A020", 0, 0, nullptr);
                icon.sharedItemMenuState = 4;
                LoadAudioFile_Wrapper(0, "E1060");
            }
        } else {
            if (g_currentSearchState == static_cast<std::int32_t>(SearchState::ScriptedLockOnActive))
                g_currentSearchState = static_cast<std::int32_t>(SearchState::ExitAndRestoreState);
            UI_ICLIResetSharedItemMenu();
            UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
            gameMenu.isHandledExternally = 1;
            icon.cassetteItemState = 2;
            if (gameMenu.elementDraw)
                gameMenu.elementDraw();
            static_cast<void>(d3t::ResetElementByIdx(kUiGameMenu));
        }
        return;
    }
    if (item >= 43 && item <= 50) {
        held &= static_cast<std::uint16_t>(~0x3000U);
        step = -256;
        turn = &root.rotation.z;
    } else if (!icon.sharedItemPreviewIsCassette && static_cast<std::uint16_t>(item - 35) <= 1U) {
        slide = &root.position.x;
    }
    if ((pressed & 0x200U) != 0 || (icon.itemLookScriptOpened && (pressed & 4U) != 0)) {
        UI_ICLIResetSharedItemMenu();
        return;
    }
    if ((held & 0xf000U) != 0) {
        if ((held & 0x1000U) != 0)
            root.rotation.x -= step;
        if ((held & 0x2000U) != 0)
            root.rotation.x += step;
        if (slide) {
            if ((held & 0x8000U) != 0) {
                *slide -= 0.01f;
                if (*slide <= -0.14f)
                    *slide = -0.14f;
            }
            if ((held & 0x4000U) != 0) {
                *slide += 0.01f;
                if (*slide >= 0.21f)
                    *slide = 0.20999999f;
            }
        } else {
            if ((held & 0x8000U) != 0)
                *turn += step;
            if ((held & 0x4000U) != 0)
                *turn -= step;
        }
    }
    root.rotation.x &= 0xffff;
    root.rotation.y &= 0xffff;
    root.rotation.z &= 0xffff;
    if ((held & 0x80U) != 0) {
        icon.itemLookZoom += zoomStep;
        if (icon.itemLookZoom >= icon.itemLookZoomMax)
            icon.itemLookZoom = icon.itemLookZoomMax;
    } else if ((held & 0x40U) != 0) {
        icon.itemLookZoom -= zoomStep;
        if (icon.itemLookZoom <= icon.itemLookZoomMin)
            icon.itemLookZoom = icon.itemLookZoomMin;
    }
}




void ItemLook_Open() {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.itemLookRemark = nullptr;
    if (!LoadItem()) {
        icon.sharedItemMenuState = 0;
        DestroySharedItemsMenuIfTrueResponse();
        shenExecuteAICACommand(kUiRejectSound, 0, 0);
        icon.cassetteItemState = 0;
        return;
    }
    if (icon.itemLookInput && (icon.itemLookInput->flags & 2U) == 0)
        icon.itemLookPreviousInput = shenUpdateInputsInto(icon.itemLookInput);
    const char *const remark = icon.sharedItemPreviewIsCassette ? nullptr : ItemLook_ExamineRemark(icon.sharedItemPreviewValue, icon.itemLookRemarkMove);
    if (!remark) {
        icon.sharedItemMenuState = 1;
    } else {
        // Shenmue_LoadAudioFile, the ReadAudioFromTable_0 wait and PlayDialogueByVoiceCode, through the wrapper's modes 1 and 0.
        LoadAudioFile_Wrapper(1, "E1060");
        icon.itemLookRemark = ProcessSubtitleFile(remark, 0, 0, nullptr);
        icon.sharedItemMenuState = 3;
        LoadAudioFile_Wrapper(0, "E1060");
    }
    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
    icon.itemLookSavedTemporaryGameState = g_temporaryGameState;
    icon.itemLookSavedFov = currentPerspectiveFov;
    icon.itemLookSavedHelpToken = g_defaultHelpTokenId;
    icon.itemLookSavedHelpButton = g_helpButtonId;
    icon.itemLookSavedEye = g_CameraEyePos;
    icon.itemLookSavedSicnPausePolicy = g_currentSicn;
    icon.itemLookSavedTarget = g_CameraTargetPos;
    icon.itemLookSavedSicnState = g_uiState;
    icon.itemLookSavedAuthorState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
    if (Character *const ryo = MapRyo()) {
        icon.itemLookSavedRyoVisible = (ryo->motionFlags >> 2U) & 1U;
        icon.itemLookSavedRyoRenderFlag = static_cast<std::int32_t>((ryo->flags.value >> 4U) & 1U);
        ryo->motionFlags = static_cast<std::uint8_t>(ryo->motionFlags & ~4U);
        ryo->flags.value &= ~0x10U;
    }
    g_temporaryGameState = 0;
    ApplyPendingGameState(g_previousAuthorState);
    SetNewAuthorGameState(9);
    CAME_SetFieldOfView(8192);
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = 18;
    g_currentHelpTokenId = 18;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    const std::int16_t item = icon.sharedItemPreviewValue;
    std::uint16_t sicn = 35;
    if (static_cast<std::uint16_t>(item) <= 66U && (g_playerStats.ownedItemMenuFlags0To66[static_cast<std::size_t>(item)] & 2U) != 0 &&
        !icon.itemLookScriptOpened) {
        sicn = 39;
    }
    icon.itemLookSavedSicnTaskActive = UI_SICNIsTaskActive();
    if (icon.itemLookSavedSicnTaskActive)
        UI_SICNSetState(sicn, UISicnPausePolicy::KeepOnPause);
    else
        UI_SICNEnqueue(sicn, UISicnPausePolicy::KeepOnPause);
    ItemLook_Place();
    if (!bLockCamera) {
        g_CameraTargetPos = icon.itemLookPosition;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {



void UI_ICLIMainCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    if ((Shenmue_nPauseState & 2) == 0) {
        switch (icon.sharedItemMenuState) {
        case 1:
            CassetteMenu_HandleInput();
            break;
        case 2:
            ItemLook_Open();
            return;
        case 3:
            if (icon.itemLookRemark && !CVWK_IsVoiceSlotActive(icon.itemLookRemark)) {
                if ((icon.itemLookRemarkMove & 0x8000U) != 0) {
                    icon.sharedItemMenuState = 1;
                } else {
                    EnqueueEWZO(0, static_cast<std::uint16_t>(icon.itemLookRemarkMove), static_cast<std::uint16_t>(icon.itemLookRemarkMove >> 16U));
                    icon.sharedItemMenuState = 5;
                }
                icon.itemLookTeachFrames = 0;
            }
            break;
        case 4:
            if (icon.itemLookRemark && !CVWK_IsVoiceSlotActive(icon.itemLookRemark))
                icon.sharedItemMenuState = 1;
            break;
        case 5:
            if (++icon.itemLookTeachFrames >= 90) {
                EnqueueEWZO(1, 0, 0);
                icon.sharedItemMenuState = 1;
            }
            break;
        default:
            break;
        }
    }
    DrawBatteryIcons();
}


void DestroySharedItemsMenu() {
    g_sharedItemRequest.requestActive = 0;
    g_sharedItemRequest.requestType = 0;
    g_sharedItemRequest.itemId = 0;
    g_sharedItemsRequested = 0;
    if (g_currUIState == -1 || g_currUIState == kUiSharedItems) {
        UIBaseRuntime &sharedItems = g_uiElements[kUiSharedItems];
        if (!sharedItems.isVisible || !sharedItems.destroy || sharedItems.destroy() == 1 || g_busy_reset == 1)
            sharedItems.isVisible = 0;
    }
}


void DestroySharedItemsMenuIfTrueResponse() {
    if (g_userResponse) {
        UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
        gameMenu.isHandledExternally = 0;
        if (gameMenu.elementDraw)
            gameMenu.elementDraw();
        DestroySharedItemsMenu();
        g_inputState.slots[0].pressedPriority = 0;
    }
    g_userResponse = 0;
}
} // namespace shenmue::s1

namespace shenmue::s1::d3t {

void Shared_Items_Tick(std::int32_t type, std::int32_t id) {
    g_sharedItemRequest.requestActive = 1;
    g_sharedItemsRequested = 1;
    g_sharedItemRequest.itemId = id;
    g_sharedItemRequest.requestType = type;
    if (g_currUIState == -1 || g_currUIState == kUiSharedItems)
        UI_ActivateElement(g_uiElements[kUiSharedItems]);
}
} // namespace shenmue::s1::d3t

namespace shenmue::s1 {



bool Load3DCassetteModelForInventory(std::int16_t id, bool cassette) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.sharedItemPreviewIsCassette = cassette;
    if ((icon.sharedItemMenuState && g_notebookHelpOverrideActive) || id < 0)
        return false;
    if (cassette) {
        if (id >= 32)
            return false;
        bool owned = false;
        for (const CassetteTapeState &tape : g_playerStats.cassetteTapes)
            owned = owned || tape.id == static_cast<std::uint8_t>(id);
        if (!owned)
            return false;
    } else if (id == 3 || id > 234 || (g_debugForceShowAllItems != 1 && !GetInventoryItemCount(static_cast<std::uint16_t>(id)))) {
        return false;
    }
    icon.itemLookEntry = cassette ? &g_cassetteCatalog[static_cast<std::size_t>(id)].entry : &g_ownedItemCatalog[static_cast<std::size_t>(id)];
    if (CountOwnedCassetteTapes() && GetInventoryItemCount(1))
        g_playerStats.ownedItemMenuFlags0To66[1] |= 2U;
    else
        g_playerStats.ownedItemMenuFlags0To66[1] &= static_cast<std::uint8_t>(~2U);
    icon.sharedItemPreviewValue = id;
    icon.sharedItemMenuState = 2;
    if (g_uiElements[kUiSubMenuItems].isVisible && id == 1) {
        d3t::ActivateElementByIdx(kUiGameHudBattery);
        g_batteryHudIndex = 1;
    } else if (g_uiElements[kUiSubMenuItems].isVisible && id == 51) {
        d3t::ActivateElementByIdx(kUiGameHudBattery);
        g_batteryHudIndex = 0;
    }
    return true;
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



void ReleaseCassettePlayerResources() {
    for (std::int16_t &surface : g_cassetteSurfaces) {
        if (surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(surface);
        surface = -1;
    }
    if (g_icmcInput && (g_icmcInput->flags & 2U) != 0 && g_cassetteUiCachedInput)
        shenUpdateInputsInto(g_cassetteUiCachedInput);
}



void ResetCassettePlayer() {
    ReleaseCassettePlayerResources();
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_cassetteCollectionUiState = 0;
    g_icmcCassetteIsPlayerActive = 0;
    const std::uint8_t helpToken = g_cassetteSavedHelpToken < 68 ? static_cast<std::uint8_t>(g_cassetteSavedHelpToken) : 0;
    g_cassettePlaying = 0;
    g_defaultHelpTokenId = helpToken;
    g_currentHelpTokenId = helpToken;
    g_helpButtonId = static_cast<std::uint8_t>(g_cassetteSavedHelpButton);
    g_cassettePlayFlag = 0;
    g_currentHelpIconState = 0;
    DestroySharedItemsMenuIfTrueResponse();
}


void BatteriesAreDead() {
    if (g_cassettePlaying) {
        ExecuteCassetteSND(g_cassetteSound);
        g_playerStats.batteryDecrementMask &= static_cast<std::uint8_t>(~2U);
        g_cassettePlaying = 0;
    }
    // Shenmue_LoadAudioFile, the ReadAudioFromTable_0 wait and PlayDialogueByVoiceCode, through the wrapper's modes 1 and 0.
    LoadAudioFile_Wrapper(1, "E1079");
    g_currentCassetteDialogue = ProcessSubtitleFile("E1079A005", 0, 0, nullptr);
    LoadAudioFile_Wrapper(0, "E1079");
    g_cassetteCollectionUiState = 4;
    g_cassetteMusicNoteVisible = 0;
    if (g_cassetteSpeakerAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_cassetteSpeakerAnimSlot)];
        slot.status = 0;
        slot.elapsed = 0.0f;
        slot.allocated = 1;
    }
    if (g_cassettesSpeakers)
        g_cassettesSpeakers->sprite = 0;
}



void PlayCassette() {
    if (((g_icmcInput ? g_icmcInput->pressedPriority : 0U) & 0x200U) != 0) {
        g_cassetteCollectionUiState = 5;
        if (g_cassettePlaying) {
            ExecuteCassetteSND(g_cassetteSound);
            g_playerStats.batteryDecrementMask &= static_cast<std::uint8_t>(~2U);
            g_cassettePlaying = 0;
        }
        return;
    }
    if (!g_cassettePlayRequested)
        return;
    g_cassettePlayRequested = 0;
    const CassetteTapeState &tape = g_playerStats.cassetteTapes[static_cast<std::size_t>(g_icmcCassetteSelectedIndex + g_cassetteBaseIndex)];
    if (tape.id == 0xffU) {
        shenExecuteAICACommand(kUiRejectSound, 0, 0);
        return;
    }
    if (!AccessBatteryLife(1, 1, 0) && !g_cassettePlayFlag) {
        BatteriesAreDead();
        return;
    }
    if (!g_cassettePlaying)
        ResetAICA(g_cassetteSound, 0.0f);
    if (!g_cassettePlayFlag)
        g_playerStats.batteryDecrementMask |= 2U;
    const CassetteCatalogEntry &cassette = g_cassetteCatalog[tape.id];
    std::array<char, 64> filename{};
    std::snprintf(filename.data(), filename.size(), "%s.snd", cassette.streamName);
    const std::array<const char *, 8> banks{nullptr, nullptr, filename.data()};
    SND_LoadSND(banks);
    shenExecuteAICACommand(cassette.soundCommand, 0, 0);
    g_cassetteIndex = static_cast<std::int8_t>(tape.id);
    g_cassettePlaying = 1;
}



void Cassette_RequestPlay(const std::array<float, 4> &cell) {
    g_cassettePlayRequested = 1;
    if (!AccessBatteryLife(1, 1, 0))
        return;
    g_cassetteMusicNoteVisible = 1;
    if (g_cassetteSpeakerAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_cassetteSpeakerAnimSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 3;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = std::bit_cast<float>(5);
        slot.delay = 0.0f;
        slot.duration = 2.0f;
        slot.reserved40 = -1;
    }
    g_cassetteMusicNoteRect = cell;
    if (g_cassetteMusicNoteAnimSlot != -1) {
        g_uiAnimSlots[static_cast<std::size_t>(g_cassetteMusicNoteAnimSlot)].elapsed = 0.0f;
        return;
    }
    g_cassetteMusicNoteAnimSlot = UIAnimAllocSlot();
    if (g_cassetteMusicNoteAnimSlot == -1)
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_cassetteMusicNoteAnimSlot)];
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 2;
    slot.start[0] = 0.0f;
    slot.current[0] = 0.0f;
    slot.end[0] = 1.0f;
    slot.delay = 0.0f;
    slot.duration = 0.75f;
    slot.reserved40 = -1;
    slot.loopMode = 2;
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t::ui_runtime_detail {



void ShowDescriptionForItem(float *rect, void *data) {
    InformationBarMessage message{.iconAction = -1, .outlinePanel = -1, .outline = {rect[0], rect[1], rect[2], rect[3]}};
    std::array<char, 32> count{};
    if (const UIGridEntry *const entry = static_cast<const UIGridEntry *>(data)) {
        struct nk_image icon {};
        if (entry->type == 0 && static_cast<std::uint32_t>(entry->id) < kOwnedItemCatalogCount)
            RenderingRelatedInventory(icon, g_ownedItemCatalog[static_cast<std::size_t>(entry->id)].iconIndex, 0);
        else if (entry->type == 1 && static_cast<std::uint32_t>(entry->id) < kCassetteCatalogCount)
            RenderingRelatedInventory(icon, g_cassetteCatalog[static_cast<std::size_t>(entry->id)].entry.iconIndex, 0);
        std::memcpy(message.iconImage.data(), &icon, sizeof(icon));
        const char *token = "";
        if (entry->type == 0)
            token = g_ownedItemCatalog[static_cast<std::size_t>(entry->id)].itemNameId;
        else if (entry->type == 1 && entry->id >= 0)
            token = g_cassetteCatalog[static_cast<std::size_t>(entry->id)].entry.cassetteNameId;
        message.text = entry->count ? token : "? ? ?";
        message.textLiteral = entry->count ? 0 : 1;
        if (static_cast<std::uint32_t>(entry->count) > 1U) {
            std::snprintf(count.data(), count.size(), "(%u)", static_cast<unsigned>(entry->count));
            message.rightText = count.data();
            message.rightTextLiteral = 1;
        }
    }
    d3t::CopyStringToUILabel(&message);
}
} // namespace shenmue::s1::d3t::ui_runtime_detail

namespace shenmue::s1::ui_runtime_detail {


void Cassette_ShowDescription(float *rect, void *data) {
    if (const UIGridEntry *const entry = static_cast<const UIGridEntry *>(data))
        g_icmcCassetteSelectedIndex = static_cast<std::uint8_t>(entry->drawnCell);
    d3t::ShowDescriptionForItem(rect, data);
}


void Cassette_SetSelectedIndex(float *rect, void *data) {
    const UIGridEntry *const entry = static_cast<const UIGridEntry *>(data);
    if (!entry)
        return;
    g_icmcCassetteSelectedIndex = static_cast<std::uint8_t>(entry->drawnCell);
    Cassette_RequestPlay({rect[0], rect[1], rect[2], rect[3]});
}



void cbHandleAllItemsWithInteraction(float *, void *data) {
    UIGridEntry *const entry = static_cast<UIGridEntry *>(data);
    const bool watch = entry && entry->type == 0 && entry->id == 3;
    if (entry && !watch && (entry->type == 0 || entry->type == 1)) {
        if (entry->count || g_debugForceShowAllItems) {
            g_userResponse = 1;
            d3t::Shared_Items_Tick(entry->type, entry->id);
            if (Load3DCassetteModelForInventory(static_cast<std::int16_t>(entry->id), entry->type == 1)) {
                g_uiElements[kUiGameMenu].isHandledExternally = 1;
                if (g_uiElements[kUiGameMenu].elementDraw)
                    g_uiElements[kUiGameMenu].elementDraw();
                PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
                return;
            }
        }
        PlayGatedUiSound(g_uiRejectSoundFrame, kUiRejectSound);
        return;
    }
    if (!entry) {
        PlayGatedUiSound(g_uiRejectSoundFrame, kUiRejectSound);
        return;
    }
    WriteInput_Callback(*entry);
    PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
}


const char *UIGridEntry_NameToken(const UIGridEntry &entry) {
    if (entry.type == 0)
        return g_ownedItemCatalog[static_cast<std::size_t>(entry.id)].itemNameId;
    if (entry.type == 1 && entry.id >= 0)
        return g_cassetteCatalog[static_cast<std::size_t>(entry.id)].entry.cassetteNameId;
    return "";
}

// The tail of the Collection children's Render (inlined in each): the Information Bar shows `text` (already localized) outlined by the grid's
// selected cell (none in the frontend), with the element's view slot in the message's first word; the rest of the current message stays.
void Collection_ShowSelectedCell(UIGrid &grid, std::int32_t element, const char *text) {
    const UILayoutRect cell = UIGrid_GetSelectedCellRect(&grid);
    InformationBarMessage message = g_informationBarLabels.message;
    message.noOutline = g_isInGame == 0;
    message.iconAction = g_uiElements[static_cast<std::size_t>(element)].config.uiHandleIndex;
    message.outline = {cell.x, cell.y, cell.width, cell.height};
    message.outlinePanel = -1;
    message.textLiteral = 1;
    message.text = text;
    d3t::CopyStringToUILabel(&message);
}


std::int32_t Cassettes_PopulateEntries(std::array<UIGridEntry, 32> &elements) {
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < 32; ++index) {
        CassetteTapeState &tape = g_playerStats.cassetteTapes[static_cast<std::size_t>(index)];
        UIGridEntry &entry = elements[static_cast<std::size_t>(count)];
        if (g_debugForceShowAllItems) {
            tape.id = static_cast<std::uint8_t>(index);
            tape.count = 1;
            entry = {.type = 1, .id = index, .count = 2};
        } else {
            if (!tape.count)
                continue;
            entry.id = static_cast<std::int8_t>(tape.id);
            entry.type = 1;
            entry.count = tape.count;
        }
        ++count;
    }
    return count;
}


void Cassettes_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    void *elements, std::int32_t index) {
    static_cast<void>(UIGrid_PlaceCellWidgets(g_cassettesGridTemplate, index, *holder, *cell));
    UIGrid_AfterDraw(g_cassettesGridTemplate, grid, index, 2, -1, *holder, panel);
    UIGrid_DrawItemCell(grid, panel, holder, cell, spacing, element, elements, index);
}



void CassetteMusicNote(const std::array<float, 4> &cell) {
    nk_context &context = GNuklearWindow;
    nk_begin_titled(
        &context, &g_cassetteMusicNotePanel, "Cassette_Music_Note", "Cassette_Music_Note", nk_rect(cell[0], cell[1], cell[2], cell[3]), NK_WINDOW_NO_SCROLLBAR);
    if (context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    float bob = 0.0f;
    if (g_cassetteMusicNoteAnimSlot != -1)
        bob = std::sin(g_uiAnimSlots[static_cast<std::size_t>(g_cassetteMusicNoteAnimSlot)].current[0]);
    const struct nk_rect note { .x = cell[2] - 40.0f - 8.0f, .y = cell[3] - 40.0f - 16.0f + bob * 5.0f, .w = 40.0f, .h = 40.0f };
    if (context.current && context.current->layout)
        context.current->layout->row.item = note;
    struct nk_image image {};
    RenderingRelatedInventory(image, 251, 0);
    nk_image_color(&context, image, nk_color{.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff});
    if (!context.current)
        return;
    if (context.current->layout)
        nk_layout_space_end(&context);
    nk_end(&context);
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {






std::int64_t CassettesMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuCassettes].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_cassettesWindows.size(); ++window)
        g_cassettesWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kCassettesWindowIds[window]));
    for (std::size_t item = 0; item < g_cassettesLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kCassettesLayoutItemIds[item]);
        g_cassettesLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    g_cassettesSpeakers = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "Submenu_Collection_Cassettes::Speakers_anim"));
    g_cassetteSpeakerAnimSlot = UIAnimAllocSlot();
    if (g_cassetteSpeakerAnimSlot != -1)
        g_uiAnimSlots[static_cast<std::size_t>(g_cassetteSpeakerAnimSlot)].loopMode = 1;
    UIGrid &grid = g_cassettesGrid;
    grid.columns = 8;
    grid.visibleRows = 4;
    grid.rows = 4;
    grid.showEmptyCells = 1;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_cassettesWindows[0];
    grid.rect = {};
    grid.drawCell = Cassettes_DrawCell;
    grid.mouseCallbacks = {
        .entered = d3t::ShowDescriptionForItem, .hovered = nullptr, .left = nullptr, .clicked = cbHandleAllItemsWithInteraction, .isHovered = 0};
    grid.getElement = d3t::cbGetMenuElementPointer;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.elements = g_cassettesGridElements.data();
    grid.scrollSpeed = 0.25f;
    grid.elementCount = g_cassettesGridElementCount;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (g_cassettesGridElementCount > grid.columns * (lastRow + 1) - 1)
        grid.rows = g_cassettesGridElementCount / grid.columns + (g_cassettesGridElementCount % grid.columns > 0 ? 1 : 0);
    UIGridTemplate &cellTemplate = g_cassettesGridTemplate;
    cellTemplate.initialized = 0;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.layoutItems = g_cassettesLayoutItems.data();
    cellTemplate.first = 2;
    cellTemplate.end = 6;
    cellTemplate.clipWindows[0] = 4;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.windows = g_cassettesWindows.data();
    cellTemplate.ids = kCassettesWindowIds.data();
    cellTemplate.layoutFirst = 0;
    cellTemplate.layoutEnd = 2;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 0;
    cellTemplate.labels = nullptr;
    if (d3t::UIWindow *const left = g_cassettesWindows[1])
        left->alpha = 0.0f;
    return 1;
}


std::int64_t CassettesMenu_Reset() {
    g_cassettesWindows = {};
    return 0;
}


std::int64_t CassettesMenu_IsPressed() {
    return UIGrid_IsPressed(g_cassettesGrid);
}



std::int64_t CassettesMenu_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuCassettes];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_cassettesGrid, element.index);
    if (!g_cassetteMusicNoteVisible)
        return 0;
    CassetteMusicNote(g_cassetteMusicNoteRect);
    if (g_cassettesSpeakers && g_cassetteSpeakerAnimSlot != -1) {
        const std::int32_t frame = std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(g_cassetteSpeakerAnimSlot)].current[0]);
        g_cassettesSpeakers->sprite = frame > 4 ? 4 : frame;
    }
    return 0;
}




void CassettesMenu_Render() {
    if (g_cassetteSpeakerAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_cassetteSpeakerAnimSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 3;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = 0.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.duration = 2.0f;
        slot.reserved40 = -1;
    }
    if (g_cassettesSpeakers)
        g_cassettesSpeakers->sprite = 0;
    if (d3t::UIWindow *const left = g_cassettesWindows[1])
        left->alpha = 0.0f;
    g_cassetteMusicNoteVisible = 0;
    UIGrid &grid = g_cassettesGrid;
    grid.mouseCallbacks.clicked = cbHandleAllItemsWithInteraction;
    grid.mouseCallbacks.entered = d3t::ShowDescriptionForItem;
    g_uiElements[kUiSubMenuCassettes].getSearchType = nullptr;
    g_cassetteBatteryHudOwned = 0;
    const std::int32_t count = Cassettes_PopulateEntries(g_cassettesGridElements);
    g_cassettesGridElementCount = count;
    grid.elementCount = count;
    grid.elements = g_cassettesGridElements.data();
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (count > grid.columns * (lastRow + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0 ? 1 : 0);
    UIGrid_ResetRuntime(grid);
    const UIGridEntry &first = g_cassettesGridElements[0];
    const char *const text = first.count ? InitAndRetrieveStringFromID(UIGridEntry_NameToken(first)) : nullptr;
    Collection_ShowSelectedCell(grid, kUiSubMenuCassettes, text);
}


std::int64_t CassettesMenu_Destroy() {
    if (g_cassetteBatteryHudOwned == 1)
        static_cast<void>(d3t::ResetElementByIdx(kUiGameHudBattery));
    g_cassetteBatteryHudOwned = 0;
    g_cassetteMusicNoteVisible = 0;
    return 1;
}



std::int64_t CassettesMenu_GetPromptIconSet() {
    if (g_cassetteBatteryHudOwned != 1)
        return 0;
    constexpr std::array<std::int16_t, 4> kCassettePromptIcons{7, 44, -1, -1};
    for (std::size_t slot = 0; slot < kCassettePromptIcons.size(); ++slot)
        g_searchPromptIconSlots[slot] = kCassettePromptIcons[slot];
    for (std::size_t slot = 0; slot < g_cassettePromptIconSet.size(); ++slot)
        g_cassettePromptIconSet[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(g_cassettePromptIconSet.data());
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


void Gacha_DrawCell(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *elements,
    std::int32_t index) {
    static_cast<void>(UIGrid_PlaceCellWidgets(g_gachaGridTemplate, index, *holder, *cell));
    UIGrid_AfterDraw(g_gachaGridTemplate, grid, index, 1, -1, *holder, panel);
    UIGrid_DrawItemCell(grid, panel, holder, cell, spacing, element, elements, index);
}



void Gacha_PopulateEntries() {
    for (std::int32_t item = 67; item < 235; ++item) {
        UIGridEntry &entry = g_gachaGridElements[static_cast<std::size_t>(item - 67)];
        std::int32_t count = GetInventoryItemCount(static_cast<std::uint16_t>(item));
        entry.count = count;
        entry.type = 0;
        entry.id = item;
        if (g_debugForceShowAllItems) {
            if (!count)
                count = 1;
            entry.count = count;
        }
    }
    UIGrid &grid = g_gachaGrid;
    grid.elements = g_gachaGridElements.data();
    g_gachaGridElementCount = 168;
    grid.elementCount = 168;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (grid.columns * (lastRow + 1) - 1 < 168)
        grid.rows = 168 / grid.columns + (168 % grid.columns > 0 ? 1 : 0);
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {






std::int64_t GachaMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuGacha].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_gachaWindows.size(); ++window)
        g_gachaWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kGachaWindowIds[window]));
    for (std::size_t item = 0; item < g_gachaLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kGachaLayoutItemIds[item]);
        g_gachaLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    g_gachaPlaceholderLabel = UI_LabelRecord(UI_FindViewLabel(slot, "Label_Placeholder"));
    const std::int32_t image = UI_FindViewImage(slot, "Image_Placeholder");
    g_gachaPlaceholderImage = static_cast<std::uint32_t>(image) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(image)] : nullptr;
    UIGrid &grid = g_gachaGrid;
    grid.columns = 8;
    grid.visibleRows = 4;
    grid.rows = 64;
    grid.showEmptyCells = 0;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_gachaWindows[0];
    grid.rect = {};
    grid.drawCell = Gacha_DrawCell;
    grid.mouseCallbacks = {
        .entered = d3t::ShowDescriptionForItem, .hovered = nullptr, .left = nullptr, .clicked = cbHandleAllItemsWithInteraction, .isHovered = 0};
    grid.getElement = d3t::cbGetMenuElementPointer;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.elements = g_gachaGridElements.data();
    grid.scrollSpeed = 0.25f;
    grid.elementCount = g_gachaGridElementCount;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (g_gachaGridElementCount > grid.columns * (lastRow + 1) - 1)
        grid.rows = g_gachaGridElementCount / grid.columns + (g_gachaGridElementCount % grid.columns > 0 ? 1 : 0);
    UIGridTemplate &cellTemplate = g_gachaGridTemplate;
    cellTemplate.initialized = 0;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.layoutFirst = 0;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 0;
    cellTemplate.labels = nullptr;
    cellTemplate.layoutItems = g_gachaLayoutItems.data();
    cellTemplate.first = 1;
    cellTemplate.end = 5;
    cellTemplate.clipWindows[0] = 3;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.windows = g_gachaWindows.data();
    cellTemplate.ids = kGachaWindowIds.data();
    cellTemplate.layoutEnd = 2;
    return 1;
}


std::int64_t GachaMenu_Reset() {
    g_gachaPlaceholderImage = nullptr;
    g_gachaWindows = {};
    g_gachaPlaceholderLabel = nullptr;
    return 0;
}


std::int64_t GachaMenu_IsPressed() {
    return UIGrid_IsPressed(g_gachaGrid);
}


std::int64_t GachaMenu_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuGacha];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_gachaGrid, element.index);
    return 0;
}



void GachaMenu_Render() {
    UIGrid &grid = g_gachaGrid;
    Gacha_PopulateEntries();
    UIGrid_ResetRuntime(grid);
    const UIGridEntry &first = g_gachaGridElements[0];
    const char *const name = InitAndRetrieveStringFromID(UIGridEntry_NameToken(first));
    Collection_ShowSelectedCell(grid, kUiSubMenuGacha, first.count ? name : "? ? ?");
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



void Collection_ShowSelectedTab() {
    UIFocusList &list = g_collectionTabList;
    if (list.shownIndex == list.selected)
        return;
    if (static_cast<std::uint32_t>(list.shownIndex) < kCollectionChildren.size())
        d3t::ResetElementByIdx(kCollectionChildren[static_cast<std::size_t>(list.shownIndex)]);
    list.shownIndex = list.selected;
    if (static_cast<std::uint32_t>(list.selected) < kCollectionChildren.size())
        d3t::ActivateElementByIdx(kCollectionChildren[static_cast<std::size_t>(list.selected)]);
    for (std::size_t tab = 0; tab < kCollectionChildren.size(); ++tab) {
        const bool shown = list.shownIndex == static_cast<std::int32_t>(tab);
        if (UIColourImageWidget *const icon = g_collectionColourImages[tab]) {
            const std::uint32_t alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>((shown ? 1.0f : 0.6f) * 255.0f));
            icon->colour = (icon->colour & 0x00ffffffU) | (alpha << 24U);
        }
        if (UIButtonWidget *const button = g_collectionButtons[tab])
            button->colour = shown ? kCollectionTabButtonSelectedColour : kCollectionTabButtonColour;
        if (UIColourImageWidget *const background = g_collectionColourImages[3 + tab])
            background->colour = shown ? kCollectionTabBackgroundSelectedColour : kCollectionTabBackgroundColour;
    }
}


void SubMenu_Collection_OnTabHovered(float *, void *data) {
    UIFocusList &list = g_collectionTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data != list.windows[tab])
            continue;
        list.lastFocused = -1;
        list.focused = tab;
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
    }
}


void SubMenu_Collection_OnTabHoverLeave(float *, void *data) {
    UIFocusList &list = g_collectionTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data == list.windows[tab] && list.focused == tab) {
            list.focused = -1;
            UIFocusList_ColourLabels(list);
        }
    }
}


void SubMenu_Collection_OnTabClicked(float *, void *data) {
    UIFocusList &list = g_collectionTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data != list.windows[tab])
            continue;
        list.selected = tab;
        PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
    }
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {



std::int64_t CollectionMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuCollection].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_collectionWindows.size(); ++window) {
        d3t::UIWindow *const record = UI_GetWindow(UI_FindViewWindow(slot, kCollectionWindowIds[window]));
        g_collectionWindows[window] = record;
        if (record) {
            record->mouseCallbacks.entered = SubMenu_Collection_OnTabHovered;
            record->mouseCallbacks.left = SubMenu_Collection_OnTabHoverLeave;
            record->mouseCallbacks.clicked = SubMenu_Collection_OnTabClicked;
        }
    }
    for (std::size_t label = 0; label < g_collectionLabels.size(); ++label)
        g_collectionLabels[label] = UI_LabelRecord(UI_FindViewLabel(slot, kCollectionLabelIds[label]));
    for (std::size_t image = 0; image < g_collectionColourImages.size(); ++image)
        g_collectionColourImages[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kCollectionColourImageIds[image]));
    for (std::size_t button = 0; button < g_collectionButtons.size(); ++button) {
        const std::uint32_t index = static_cast<std::uint32_t>(UI_FindViewButton(slot, kCollectionButtonIds[button]));
        g_collectionButtons[button] = index < g_uiButtonWidgets.size() ? &g_uiButtonWidgets[index] : nullptr;
    }
    UIFocusList &list = g_collectionTabList;
    list.labels = g_collectionLabels.data();
    list.windows = g_collectionWindows.data();
    list.first = 0;
    list.last = 2;
    return 1;
}


std::int64_t CollectionMenu_Reset() {
    g_collectionButtons = {};
    g_collectionWindows = {};
    g_collectionLabels = {};
    g_collectionColourImages = {};
    return 0;
}


std::int64_t CollectionMenu_IsPressed() {
    UIFocusList_HandleInput(g_collectionTabList);
    Collection_ShowSelectedTab();
    return 1;
}


std::int64_t CollectionMenu_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuCollection];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}




void CollectionMenu_Render() {
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    UI_ActivateElement(informationBar);
    informationBar.isHandledExternally = 0;
    UIFocusList &list = g_collectionTabList;
    g_uiFocusedList = &list;
    list.focused = 0;
    list.lastFocused = 0;
    list.initialized = 0;
    list.hasFocus = 0;
    g_uiGridInputEnabled = 1;
    UI_RestartCurrentGridSelection();
    list.shownIndex = -1;
    Collection_ShowSelectedTab();
    constexpr std::int32_t kPlatformCountry = 2;
    for (OwnedItemCatalogEntry &entry : g_ownedItemCatalog) {
        if (entry.iconIndex == 0xbe)
            entry.itemNameId = kPlatformCountry == 0 || kPlatformCountry == 1 ?
                "UIGACHGENE" :
                "UIGACHMEGA"; // @text UIGACHGENE EN: "Genesis" | JA: "ジェネシス" ; UIGACHMEGA EN: "Mega Drive" | JA: "メガドライブ"
    }
}


std::int64_t CollectionMenu_Destroy() {
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    d3t::ResetElementByIdx(kCollectionChildren[static_cast<std::size_t>(g_collectionTabList.selected)]);
    return 1;
}



std::int64_t CollectionMenu_ElementDraw() {
    UIBaseRuntime &child = g_uiElements[static_cast<std::size_t>(kCollectionChildren[static_cast<std::size_t>(g_collectionTabList.selected)])];
    if (g_uiElements[kUiSubMenuCollection].isHandledExternally) {
        child.isHandledExternally = 1;
    } else {
        g_uiGridInputEnabled = 1;
        g_uiFocusedList = &g_collectionTabList;
        child.isHandledExternally = 0;
    }
    return child.elementDraw ? child.elementDraw() : 0;
}


std::int64_t CollectionMenu_GetPromptIconSet() {
    constexpr std::array<std::int16_t, 4> kCollectionPromptIcons{4, 44, -1, -1};
    for (std::size_t slot = 0; slot < kCollectionPromptIcons.size(); ++slot)
        g_searchPromptIconSlots[slot] = kCollectionPromptIcons[slot];
    for (std::size_t slot = 0; slot < g_collectionPromptIconSet.size(); ++slot)
        g_collectionPromptIconSet[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(g_collectionPromptIconSet.data());
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {




void Scrolls_PopulateEntries() {
    std::size_t slot = 0;
    for (std::int32_t item = 17; item < 35; ++item) {
        const std::uint8_t owned = GetInventoryItemCount(static_cast<std::uint16_t>(item));
        if ((!owned && !g_debugForceShowAllItems) || slot >= g_scrollsGridElements.size())
            continue;
        UIGridEntry &entry = g_scrollsGridElements[slot++];
        entry.count = owned;
        entry.type = 0;
        entry.id = item;
        if (g_debugForceShowAllItems)
            entry.count = owned + 1;
    }
    UIGrid &grid = g_scrollsGrid;
    grid.elements = g_scrollsGridElements.data();
    g_scrollsGridElementCount = 9;
    grid.elementCount = 9;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (grid.columns * (lastRow + 1) - 1 < 9)
        grid.rows = 9 / grid.columns + (9 % grid.columns > 0 ? 1 : 0);
}





void Scrolls_DrawCell(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *elements, std::int32_t index) {
    nk_context &context = GNuklearWindow;
    const UILayoutRect rect = *cell;
    static_cast<void>(UIGrid_PlaceCellWidgets(g_scrollsGridTemplate, index, *holder, rect));
    UIGrid_AfterDraw(g_scrollsGridTemplate, grid, index, 1, -1, *holder, panel);
    const float alpha = index == UIGrid_SelectedIndex(*grid) ? 1.0f : 0.6f;
    const auto place = [&context](const UILayoutRect &at) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(at.x, at.y, at.width, at.height);
    };
    UIGridEntry *const entry = elements ? &static_cast<UIGridEntry *>(elements)[static_cast<std::uint32_t>(index)] : nullptr;
    const bool owned = entry && entry->count != 0;
    UI_WindowMouseCallbacks(grid->mouseCallbacks, element, rect, owned ? entry : &g_scrollsEmptyElement);
    struct nk_image image {};
    if (static_cast<std::uint32_t>(g_uiElements[kUiSharedItems].config.uiHandleIndex) <= 75U) {
        if (!owned)
            RenderingRelatedInventory(image, g_ownedItemCatalog[29].iconIndex, 0);
        else if (entry->type == 0)
            RenderingRelatedInventory(image, g_ownedItemCatalog[static_cast<std::size_t>(entry->id)].iconIndex, 0);
        else if (entry->type == 1)
            RenderingRelatedInventory(image, g_cassetteCatalog[static_cast<std::size_t>(entry->id)].entry.iconIndex, 0);
    }
    const nk_color colour{.r = 0xff, .g = 0xff, .b = 0xff, .a = static_cast<nk_byte>(static_cast<std::int32_t>(alpha * 255.0f))};
    place({.x = rect.x + 30.0f + rect.height * 0.1f, .y = rect.y + rect.height * 0.1f, .width = rect.height * 0.8f, .height = rect.height * 0.8f});
    if (g_waitingForNextTick == 1)
        nk_image_color(&context, image, owned ? colour : nk_color{.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff});
    const float iconSpan = rect.height * 0.8f + 30.0f;
    place({.x = rect.x + 30.0f + iconSpan, .y = rect.y, .width = rect.width - 30.0f - iconSpan, .height = rect.height});
    context.style.text.color = colour;
    const char *const text = InitAndRetrieveStringFromID(owned ? UIGridEntry_NameToken(*entry) : "UIGNRL???"); // @text EN: "???" | JA: "???"
    if (g_waitingForNextTick == 1)
        nk_text_colored(&context, text, text ? static_cast<int>(std::strlen(text)) : 0, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, colour);
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {





std::int64_t ScrollsMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuScrolls].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_scrollsWindows.size(); ++window)
        g_scrollsWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kScrollsWindowIds[window]));
    for (std::size_t item = 0; item < g_scrollsLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kScrollsLayoutItemIds[item]);
        g_scrollsLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    const std::int32_t image = UI_FindViewImageWidget(slot, "Image_Placeholder");
    g_scrollsPlaceholderImage = static_cast<std::uint32_t>(image) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(image)] : nullptr;
    UIGrid &grid = g_scrollsGrid;
    grid.columns = 1;
    grid.visibleRows = 8;
    grid.rows = 9;
    grid.showEmptyCells = 1;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_scrollsWindows[0];
    grid.rect = {};
    grid.drawCell = Scrolls_DrawCell;
    grid.mouseCallbacks = {
        .entered = d3t::ShowDescriptionForItem, .hovered = nullptr, .left = nullptr, .clicked = cbHandleAllItemsWithInteraction, .isHovered = 0};
    grid.getElement = d3t::cbGetMenuElementPointer;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.elements = g_scrollsGridElements.data();
    grid.scrollSpeed = 0.25f;
    grid.elementCount = g_scrollsGridElementCount;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (g_scrollsGridElementCount > grid.columns * (lastRow + 1) - 1)
        grid.rows = g_scrollsGridElementCount / grid.columns + (g_scrollsGridElementCount % grid.columns > 0 ? 1 : 0);
    UIGridTemplate &cellTemplate = g_scrollsGridTemplate;
    cellTemplate.initialized = 0;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.layoutFirst = 0;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 0;
    cellTemplate.labels = nullptr;
    cellTemplate.layoutItems = g_scrollsLayoutItems.data();
    cellTemplate.first = 1;
    cellTemplate.end = 5;
    cellTemplate.clipWindows[0] = 3;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.windows = g_scrollsWindows.data();
    cellTemplate.ids = kScrollsWindowIds.data();
    cellTemplate.layoutEnd = 2;
    return 1;
}


std::int64_t ScrollsMenu_Reset() {
    g_scrollsWindows = {};
    g_scrollsPlaceholderImage = nullptr;
    return 0;
}


std::int64_t ScrollsMenu_IsPressed() {
    return UIGrid_IsPressed(g_scrollsGrid);
}


std::int64_t ScrollsMenu_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuScrolls];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_scrollsGrid, element.index);
    return 0;
}



void ScrollsMenu_Render() {
    UIGrid &grid = g_scrollsGrid;
    Scrolls_PopulateEntries();
    UIGrid_ResetRuntime(grid);
    const UIGridEntry &first = g_scrollsGridElements[0];
    const char *const name = InitAndRetrieveStringFromID(UIGridEntry_NameToken(first));
    Collection_ShowSelectedCell(grid, kUiSubMenuScrolls, first.count ? name : "? ? ?");
}


std::int64_t BatteryHud_IsSelected() {
    g_batteryHudIndex = 1;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudBattery].config.uiHandleIndex);
    g_batteryHudSprite = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "Battery::Sprite"));
    g_batteryHudAnimSlot = UIAnimAllocSlot();
    return 1;
}


std::int64_t BatteryHud_IsPressed() {
    if ((g_inputState.slots[0].pressedPriority & 2U) == 0 || g_batteryLifeFractions[static_cast<std::size_t>(g_batteryHudIndex)] > 0.2f)
        return 1;
    const std::uint16_t spare = g_batteryHudIndex ? 53 : 52;
    if (GetInventoryItemCount(spare) && RemoveInventoryItemCount(spare, 1) != 2)
        g_playerStats.batteryLives[static_cast<std::size_t>(g_batteryHudIndex)] = 28800;
    return 1;
}




std::int64_t BatteryHud_Tick() {
    if (UIButtonIconWidget *const sprite = g_batteryHudSprite) {
        const float life = g_batteryLifeFractions[static_cast<std::size_t>(g_batteryHudIndex)];
        std::int32_t bars = static_cast<std::int32_t>(life * 100.0f / 20.0f);
        if (life < 1.0f && life > 0.0f)
            ++bars;
        const std::int32_t slotIndex = g_batteryHudAnimSlot;
        sprite->sprite = 5 - bars;
        if (slotIndex != -1) {
            UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
            if (life <= 0.2f && !slot.status) {
                slot.allocated = 1;
                slot.status = 1;
                slot.interpolation = 2;
                slot.start[0] = 2.0f;
                slot.current[0] = 2.0f;
                slot.end[0] = -1.0f;
                slot.delay = 0.0f;
                slot.elapsed = 0.0f;
                slot.duration = 0.5f;
                slot.reserved40 = -1;
                slot.loopMode = 2;
            }
            if (slot.status) {
                std::uint32_t alpha = 0xffU;
                if (life <= 0.2f) {
                    alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>((slot.current[0] >= 0.0f ? std::fminf(1.0f, slot.current[0]) : 0.0f) * 255.0f));
                } else {
                    slot.status = 0;
                    slot.elapsed = 0.0f;
                    slot.allocated = 1;
                }
                sprite->colour = (sprite->colour & 0x00ffffffU) | (alpha << 24U);
            }
        }
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudBattery].config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}




std::int64_t BatteryHud_GetPromptIconSet() {
    std::array<std::int32_t, 5> &set = g_batteryPromptIconSet;
    if (g_batteryHudIndex == 1 && g_cassetteBatteryHudOwned == 1)
        set = {7, 44, 0xffff, 0xffff, static_cast<std::uint16_t>(SRCH_GetPromptIconSlots()[4])};
    else if (g_batteryHudIndex == 1)
        set = {6, 44, 58, 0xffff, 0xffff};
    else
        set = {0xffff, 44, 58, 0xffff, 0xffff};
    if (g_batteryLifeFractions[static_cast<std::size_t>(g_batteryHudIndex)] > 0.2f)
        set[3] = 0xffff;
    else
        set[3] = GetInventoryItemCount(g_batteryHudIndex ? 53 : 52) ? 11 : 12;
    return reinterpret_cast<std::int64_t>(set.data());
}



bool shenPrepareCassetteUI() {
    std::int32_t tape = 0;
    for (; tape < 28; ++tape) {
        if (GetCassetteTapeCount(static_cast<std::uint8_t>(tape)))
            break;
    }
    if (tape >= 28 || g_cassetteCollectionUiState)
        return false;
    g_cassetteCollectionUiState = 1;
    if (g_currUIState == -1 || g_currUIState == kUiSubMenuCassettes)
        UI_ActivateElement(g_uiElements[kUiSubMenuCassettes]);
    d3t::ActivateElementByIdx(kUiGameHudBattery);
    if (d3t::UIWindow *const left = g_cassettesWindows[1])
        left->alpha = 1.0f;
    g_uiGridInputEnabled = 1;
    g_cassettesGrid.mouseCallbacks.clicked = Cassette_SetSelectedIndex;
    g_uiFocusedList = nullptr;
    g_cassettesGrid.mouseCallbacks.entered = Cassette_ShowDescription;
    g_uiElements[kUiSubMenuCassettes].getSearchType = CassettesMenu_GetPromptIconSet;
    g_cassetteBatteryHudOwned = 1;
    if (g_currUIState == -1 || g_currUIState == kUiGameMenuBackground)
        UI_ActivateElement(g_uiElements[kUiGameMenuBackground]);
    g_inputState.slots[0].pressedPriority = 0;
    return true;
}


void UI_StopPlayingCassettes() {
    if (!g_cassetteCollectionUiState)
        return;
    ReleaseCassettePlayerResources();
    if (g_cassettePlaying) {
        ExecuteCassetteSND(g_cassetteSound);
        g_cassettePlaying = 0;
    }
    g_playerStats.batteryDecrementMask &= static_cast<std::uint8_t>(~2U);
    g_cassetteCollectionUiState = 0;
    g_icmcCassetteIsPlayerActive = 0;
    g_cassetteVisibleCount = 1;
    g_currentCassetteDialogue = nullptr;
}

// SCNF_PrepareCassetteUI's test: open the player from free roam when no item look or notebook help is up; a script-started player
// neither drains the battery nor checks it.
bool UI_ICMCBegin() {
    if (g_isPlayerGameStateActive != 7 || g_iconPresentation.sharedItemMenuState || g_notebookHelpOverrideActive || !shenPrepareCassetteUI())
        return false;
    g_cassettePlayFlag = 1;
    return true;
}

// SCNF_WaitForCassetteUI's test: the player has closed.
bool UI_ICMCWaitForClosed(std::int32_t &result) {
    if (g_cassetteCollectionUiState)
        return false;
    result = -1;
    return true;
}


void UI_ICMCCallback(HLib::TaskData *) {
    g_iconPresentation.cassetteTask = CurrentTask;
    g_icmcCassetteSelectedIndex = 0;
    g_cassetteSurfaces.fill(-1);
    g_cassetteCollectionUiState = 0;
    g_cassetteIndex = -1;
    g_cassettePlaying = 0;
    g_cassetteOwnedCount = 0;
    g_currentCassetteDialogue = nullptr;
    g_cassettePlayFlag = 0;
    g_cassetteVisibleCount = 1;
    g_icmcInput = AllocateClearedPADCEntry();
    if (g_icmcInput)
        g_icmcInput->flags |= 1U;
    InstallOwnedTaskMain(UI_ICMCMainCallback);
}



void UI_ICMCMainCallback(HLib::TaskData *) {
    switch (g_cassetteCollectionUiState) {
    case 1: {
        g_cassetteSavedHelpToken = g_defaultHelpTokenId;
        g_cassetteSavedHelpButton = static_cast<std::int8_t>(g_helpButtonId);
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_icmcCassetteIsPlayerActive = 1;
        g_icmcCassetteSelectedIndex = 0;
        g_cassetteBaseIndex = 0;
        g_currentCassetteDialogue = nullptr;
        g_cassettePlaying = 0;
        g_defaultHelpTokenId = 64;
        g_currentHelpTokenId = 64;
        g_helpButtonId = 0;
        g_currentHelpIconState = 0;
        g_cassetteCollectionUiState = 2;
        std::uint16_t owned = 0;
        for (const CassetteTapeState &tape : g_playerStats.cassetteTapes) {
            if (tape.id != 0xffU)
                ++owned;
        }
        g_cassetteOwnedCount = owned;
        g_cassetteVisibleCount = static_cast<std::uint8_t>(owned >= 9 ? 9 : owned);
        if (g_icmcInput && (g_icmcInput->flags & 2U) == 0)
            g_cassetteUiCachedInput = shenUpdateInputsInto(g_icmcInput);
        break;
    }
    case 2:
        g_cassetteCollectionUiState = 3;
        break;
    case 3:
        if (g_cassettePlayFlag || !g_cassettePlaying || AccessBatteryLife(1, 1, 0))
            PlayCassette();
        else
            BatteriesAreDead();
        break;
    case 4:
        if (g_currentCassetteDialogue && !CVWK_IsVoiceSlotActive(g_currentCassetteDialogue)) {
            g_cassetteCollectionUiState = 3;
            g_currentCassetteDialogue = nullptr;
        }
        break;
    case 5: {
        const std::int32_t state = g_currUIState;
        if (state == -1 || state == kUiSubMenuCassettes) {
            UIBaseRuntime &cassettes = g_uiElements[kUiSubMenuCassettes];
            if (!cassettes.isVisible || !cassettes.destroy || cassettes.destroy() == 1 || g_busy_reset == 1)
                cassettes.isVisible = 0;
        }
        if (g_currUIState == -1 || g_currUIState == kUiGameMenuBackground) {
            UIBaseRuntime &background = g_uiElements[kUiGameMenuBackground];
            if (!background.isVisible || !background.destroy || background.destroy() == 1 || g_busy_reset == 1)
                background.isVisible = 0;
        }
        if (g_userResponse)
            UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
        const bool closeMenu = !g_cassettePlayFlag && ((g_icmcInput ? g_icmcInput->pressedPriority : 0U) & 0x200U) != 0;
        ResetCassettePlayer();
        if (closeMenu)
            g_iconPresentation.menuCloseRequested = true;
        break;
    }
    default:
        break;
    }
    if (g_icmcCassetteIsPlayerActive)
        g_uiElements[kUiSubMenuCassettes].isActive = 1;
}

std::uint16_t RefreshBatteryLifeForCassettes() {
    if ((g_playerStats.batteryDecrementMask & 1U) != 0 && g_isPlayerGameStateActive != 0) {
        g_playerStats.batteryLives[0] = g_playerStats.batteryLives[0] > 1U ? g_playerStats.batteryLives[0] - 1U : 0U;
    }
    if ((g_playerStats.batteryDecrementMask & 2U) != 0) {
        g_playerStats.batteryLives[1] = g_playerStats.batteryLives[1] > 1U ? g_playerStats.batteryLives[1] - 1U : 0U;
    }
    g_batteryLifeFractions[0] = static_cast<float>(g_playerStats.batteryLives[0]) / 28800.0f;
    g_batteryLifeFractions[1] = static_cast<float>(g_playerStats.batteryLives[1]) / 28800.0f;
    return g_playerStats.batteryLives[1];
}



std::int32_t GetFightAnimInfos(std::int32_t move, char *name, FightMoveInfo *info) {
    info->reserved08 = 0;
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    std::size_t set = 0;
    std::int32_t index = move;
    if (move >= handCount) {
        if (move >= legCount + handCount) {
            if (move >= handCount + legCount + throwCount)
                return 0;
            set = 2;
            index = move - legCount - handCount;
        } else {
            set = 1;
            index = move - handCount;
        }
    }
    const auto moveId = static_cast<std::size_t>(static_cast<std::int16_t>(sets[set].moveValue[static_cast<std::size_t>(index)]));
    const MoveListRecord &record = set == 0 ? g_handMoveList[moveId] : set == 1 ? g_legMoveList[moveId] : g_throwMoveList[moveId];
    if (name)
        std::sprintf(name, "%s", InitAndRetrieveStringFromID(record.nameToken));
    info->progress = static_cast<float>(static_cast<std::int16_t>(sets[set].learnValue[static_cast<std::size_t>(index)])) / 100.0f;
    info->newExperience = static_cast<float>(static_cast<std::int16_t>(g_playerStats.moveUsageCounts[set][static_cast<std::size_t>(index)])) / 100.0f;
    const auto command = static_cast<std::size_t>(record.commandIndex);
    const MoveCommandRecord &commands = set == 0 ? g_handMoveCommands[command] : set == 1 ? g_legMoveCommands[command] : g_throwMoveCommands[command];
    info->commandCount = commands[0];
    std::memcpy(info->commands.data(), commands.data() + 1, info->commands.size());
    return 1;
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


char *shenGetMoveNameFromID(std::int32_t move) {
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    static char empty[1]{};
    const char *id = nullptr;
    if (move < handCount)
        id = g_handMoveNames[static_cast<std::size_t>(static_cast<std::int16_t>(sets[0].moveValue[static_cast<std::size_t>(move)]))];
    else if (move < legCount + handCount)
        id = g_legMoveNames[static_cast<std::size_t>(static_cast<std::int16_t>(sets[1].moveValue[static_cast<std::size_t>(move - handCount)]))];
    else if (move < handCount + legCount + throwCount)
        id = g_throwMoveNames[static_cast<std::size_t>(static_cast<std::int16_t>(sets[2].moveValue[static_cast<std::size_t>(move - legCount - handCount)]))];
    return id ? const_cast<char *>(InitAndRetrieveStringFromID(id)) : empty;
}


void _0001_UpdateRyoMove(std::int32_t move) {
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    if (move < handCount)
        RequestAnimation(1, static_cast<std::uint8_t>(sets[0].moveValue[static_cast<std::size_t>(move)]));
    else if (move < legCount + handCount)
        RequestAnimation(2, static_cast<std::uint8_t>(sets[1].moveValue[static_cast<std::size_t>(move - handCount)]));
    else if (move < handCount + legCount + throwCount)
        RequestAnimation(3, static_cast<std::uint8_t>(sets[2].moveValue[static_cast<std::size_t>(move - legCount - handCount)]));
}



bool AssignFightMove(std::int32_t move) {
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    std::uint8_t animSet = 1;
    std::int16_t moveId = 0;
    if (move < handCount) {
        moveId = static_cast<std::int16_t>(sets[0].moveValue[static_cast<std::size_t>(move)]);
    } else if (move < legCount + handCount) {
        animSet = 2;
        moveId = static_cast<std::int16_t>(sets[1].moveValue[static_cast<std::size_t>(move - handCount)]);
    } else if (move < handCount + legCount + throwCount) {
        animSet = 3;
        moveId = static_cast<std::int16_t>(sets[2].moveValue[static_cast<std::size_t>(move - legCount - handCount)]);
    } else {
        return false;
    }
    FightMoveInfo info{};
    if (GetFightAnimInfos(move, nullptr, &info) != 1 || info.progress * 100.0f < 12.0f || static_cast<std::uint16_t>(moveId) >= 20U)
        return false;
    constexpr std::array<std::uint8_t, 4> kAnimSetFightBase{0, 0, 32, 64};
    const auto fightMove = static_cast<std::uint8_t>(moveId + kAnimSetFightBase[animSet]);
    if (fightMove != 0xffU)
        g_playerStats.currentMove = static_cast<std::uint16_t>((g_playerStats.currentMove & 0x00ffU) | (fightMove << 8));
    return fightMove != 0xffU;
}


void MovesScreen_RemapAnswered(UIRetry70ManBattlePromptResult result) {
    if (result == UIRetry70ManBattlePromptResult::Yes && AssignFightMove(g_movesScreen.remapMove))
        PlayGatedUiSound(g_movesRemapAssignSoundFrame, 0x208abU);
}



void CreateMessageBoxForFightMoveRemapping(std::int32_t move) {
    g_movesScreen.remapMove = move;
    std::array<char, 512> message{};
    std::snprintf(message.data(), message.size(), "%s",
        InitAndRetrieveStringFromID(
            "UIMOVEMAPRT")); // @text EN: "Do you want to assign [MOVE_NAME] to [TRIGGER_ICON]?" | JA: "[MOVE_NAME]を [TRIGGER_ICON]に指定しますか？"
    std::array<char, 512> name{};
    FightMoveInfo info{};
    GetFightAnimInfos(move, name.data(), &info);
    if (info.progress * 100.0f < 12.0f) {
        PlayGatedUiSound(g_uiRejectSoundFrame, kUiRejectSound);
        return;
    }
    TextFormat(message.data(), static_cast<std::int32_t>(message.size()), "[MOVE_NAME]", name.data());
    TextFormat(message.data(), static_cast<std::int32_t>(message.size()), "[TRIGGER_ICON]", "<FightAssignedMove>");
    d3t::MessageBox(message.data(), true, 1, MovesScreen_RemapAnswered, 0.0f);
}



void MovesScreen_ClampStatBars() {
    d3t::UIWindow *const power = g_movesWindows[2];
    d3t::UIWindow *const speed = g_movesWindows[3];
    d3t::UIWindow *const rigid = g_movesWindows[4];
    if (!power || !speed || !rigid)
        return;
    power->layout.width.scale = 0.0f;
    speed->layout.width.scale = 0.0f;
    rigid->layout.width.scale = 0.0f;
    for (d3t::UIWindow *const window : {power, speed, rigid}) {
        float &scale = window->layout.width.scale;
        if (scale < 0.12f)
            scale = scale > 0.0f ? 0.12f : 0.0f;
    }
}




void UpdateSelectedMoveInMovesScreen(float *rect, void *data) {
    InformationBarMessage message = g_informationBarLabels.message;
    message.iconAction = -1;
    message.outlinePanel = -1;
    message.noOutline = 0;
    if (rect)
        message.outline = {rect[0], rect[1], rect[2], rect[3]};
    message.textLiteral = 0;
    if (const std::int32_t *const move = static_cast<const std::int32_t *>(data)) {
        _0001_UpdateRyoMove(*move);
        MovesScreen_ClampStatBars();
        g_movesScreen.selectedMove = *move;
        FightMoveInfo info{};
        GetFightAnimInfos(*move, nullptr, &info);
        if (info.progress * 100.0f < 12.0f) {
            message.text = kUnknownMoveText;
            message.textLiteral = 1;
        } else if (char *const name = shenGetMoveNameFromID(*move); name) {
            const auto size = static_cast<std::int32_t>(std::strlen(name) + 1);
            while (TextFormat(name, size, " [[HN]]", " ") == 1) {
            }
            while (TextFormat(name, size, "[[HN]] ", " ") == 1) {
            }
            while (TextFormat(name, size, "[[HN]]", " ") == 1) {
            }
            message.text = name;
            message.textLiteral = 1;
        }
    }
    d3t::CopyStringToUILabel(&message);
}



void MovesScreen_ConfirmMove(float *rect, void *data) {
    if (!data)
        return;
    UpdateSelectedMoveInMovesScreen(rect, data);
    if (g_movesScreen.adjustFightMove)
        g_movesScreen.adjustFightMove(g_movesScreen.fightMoves[static_cast<std::size_t>(*static_cast<const std::int32_t *>(data))]);
}


void *MovesScreen_GetElement(void *elements, std::int32_t index) {
    return elements ? &g_movesScreen.fightMoves[static_cast<std::size_t>(index)] : nullptr;
}


void MovesScreen_SetListCount(std::int32_t count) {
    UIGrid &grid = g_movesGrid;
    grid.elementCount = count;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    grid.elements = g_movesScreen.fightMoves.data();
    if (count > grid.columns * (lastRow + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0 ? 1 : 0);
}


void UpdateWidget() {
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    MovesScreenElement &moves = g_movesScreen;
    moves.fightMoveCount = 0;
    for (std::int32_t move = 0; move < 51; ++move) {
        FightMoveInfo info{};
        if (GetFightAnimInfos(move, nullptr, &info) == 1)
            moves.fightMoves[static_cast<std::size_t>(moves.fightMoveCount++)] = move;
    }
    MovesScreen_SetListCount(moves.fightMoveCount);
}


void UpdateState(std::int32_t animSet) {
    MovesScreenElement &moves = g_movesScreen;
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    moves.fightMoveCount = 0;
    for (std::int32_t move = 0; move < 51; ++move) {
        FightMoveInfo info{};
        if (GetFightAnimInfos(move, nullptr, &info) != 1)
            continue;
        std::int32_t set = 1;
        if (move >= handCount)
            set = move >= legCount + handCount ? (move < handCount + legCount + throwCount ? 3 : -1) : 2;
        if (set == animSet)
            moves.fightMoves[static_cast<std::size_t>(moves.fightMoveCount++)] = move;
    }
    MovesScreen_SetListCount(moves.fightMoveCount);
}

// The button-sprite frame slot: its integer lane steps 0 -> 1 after 0.05 s, when the Tick advances the frame and restarts it.
void MovesScreen_StartButtonFrame(std::int32_t slotIndex) {
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 3;
    slot.start[0] = 0.0f;
    slot.current[0] = 0.0f;
    slot.end[0] = std::bit_cast<float>(1);
    slot.duration = 0.001f;
    slot.delay = 0.05f;
    slot.elapsed = 0.0f;
    slot.reserved40 = -1;
}









void MovesScreen_DrawCell(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *, std::int32_t index) {
    nk_context &context = GNuklearWindow;
    context.style.window.fixed_background.data.color.a = 0;
    std::int32_t *const move = &g_movesScreen.fightMoves[static_cast<std::size_t>(index)];
    UI_WindowMouseCallbacks(grid->mouseCallbacks, element, *cell, move);
    static_cast<void>(UIGrid_PlaceCellWidgets(g_movesGridTemplate, index, *holder, *cell));
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuMoves].config.uiHandleIndex);
    std::array<char, 256> name{};
    FightMoveInfo info{};
    if (slot <= 0x4bU && GetFightAnimInfos(*move, name.data(), &info) == 1) {
        float alpha = index == UIGrid_SelectedIndex(*grid) ? 1.0f : 0.6f;
        const float total = info.newExperience + info.progress;
        const float progressPercent = info.progress * 100.0f;
        const float totalPercent = total * 100.0f;
        if (12.0f > totalPercent)
            alpha = alpha * 0.35f;
        const auto withAlpha = [](std::uint32_t colour, std::int32_t value) {
            return (colour & 0x00ffffffU) | static_cast<std::uint32_t>(static_cast<std::uint8_t>(value)) << 24;
        };
        for (std::size_t label = g_movesLabels.size(); label-- > 0;) {
            if (d3t::UILabelWidget *const record = g_movesLabels[label]) {
                std::snprintf(g_movesLabelTexts[label].data(), g_movesLabelTexts[label].size(), "%s", name.data());
                record->text = g_movesLabelTexts[label].data();
                record->colour = withAlpha(record->colour, static_cast<std::int32_t>(alpha * 255.0f));
            }
        }
        d3t::UIWindow *const row = g_movesWindows[5];
        if (d3t::UIWindow *const bar = g_movesWindows[8]) {
            bar->layout.height.value = row->layout.height.value;
            bar->layout.width.value.fraction = info.progress * row->layout.width.value.fraction;
        }
        if (d3t::UIWindow *const newExperience = g_movesWindows[7]) {
            newExperience->layout.height.value = row->layout.height.value;
            newExperience->layout.width.value.fraction = total * row->layout.width.value.fraction;
            newExperience->alpha = 0.0f < info.newExperience ? 1.0f : 0.0f;
        }
        d3t::UIWindow *const level = g_movesWindows[6];
        level->layout.height.value = row->layout.height.value;
        float levelWidth = row->layout.width.value.fraction;
        std::uint32_t levelColour = kMovesLevelColours[3];
        if (totalPercent < 100.0f) {
            if (totalPercent >= 56.0f) {
                levelColour = kMovesLevelColours[2];
            } else if (totalPercent >= 12.0f) {
                levelColour = kMovesLevelColours[1];
                levelWidth = levelWidth * 0.56f;
            } else {
                levelColour = kMovesLevelColours[0];
                levelWidth = levelWidth * 0.12f;
            }
        }
        const std::uint32_t barColour = withAlpha(levelColour, static_cast<std::int32_t>(static_cast<float>(levelColour >> 24) * alpha));
        if (UIButtonWidget *const button = g_movesButtons[4])
            button->colour = barColour;
        if (UIButtonWidget *const button = g_movesButtons[2])
            button->colour = barColour;
        if (UIButtonWidget *const button = g_movesButtons[3]) {
            std::uint32_t lighter = 0;
            for (std::uint32_t shift = 0; shift < 24; shift += 8)
                lighter |= static_cast<std::uint32_t>(static_cast<std::uint8_t>((levelColour >> shift) + 20U)) << shift;
            button->colour = withAlpha(lighter, static_cast<std::int32_t>(alpha * 255.0f));
        }
        level->layout.width.value.fraction = levelWidth;
        if (d3t::UIWindow *const assign = g_movesWindows[24]) {
            if (*move == GetAssignedFightMoveIndex()) {
                const d3t::UILabelWidget *const label = g_movesLabels[1];
                if (g_uiFontSize != 2) {
                    g_uiFontSize = 2;
                    UI_SelectFont();
                }
                g_uiFontScale = g_uiViewSlots[slot].variables[static_cast<std::size_t>(label->scaleVar)].value.floatValue;
                UI_SelectFont();
                const std::int32_t length = static_cast<std::int32_t>(strnlen(label->text, 0x40));
                const nk_user_font *const font = context.style.font;
                const float width = font && font->width ? font->width(font->userdata, font->height, label->text, length) : 0.0f;
                const float aspectWidth = static_cast<float>(UI_GetAspectWidth());
                assign->layout.offsetX = 128.0f / aspectWidth + width / aspectWidth;
                assign->alpha = 1.0f;
            } else {
                assign->alpha = 0.0f;
            }
            assign->alpha = 0.0f;
        }
        const auto hideButtons = [] {
            for (std::size_t window = 13; window < 24; ++window) {
                if (d3t::UIWindow *const record = g_movesWindows[window])
                    record->alpha = 0.0f;
            }
        };
        hideButtons();
        if (progressPercent < 12.0f) {
            hideButtons();
        } else if (*move == GetAssignedFightMoveIndex()) {
            if (UIButtonIconWidget *const icon = g_movesButtonIcons[1])
                icon->action = 30;
            if (d3t::UIWindow *const window = g_movesWindows[13])
                window->alpha = alpha;
        } else {
            std::size_t button = 0;
            for (std::int32_t command = static_cast<std::int8_t>(info.commandCount) - 1; command >= 0; --command) {
                const std::int32_t code = static_cast<std::int8_t>(info.commands[static_cast<std::size_t>(command)]);
                if (static_cast<std::uint32_t>(code) > 7U)
                    continue;
                if (code == 7) {
                    if (d3t::UIWindow *const plus = g_movesWindows[18 + button])
                        plus->alpha = alpha;
                    continue;
                }
                if (UIButtonIconWidget *const icon = g_movesButtonIcons[1 + button])
                    icon->action = kMovesCommandActions[static_cast<std::size_t>(code)];
                if (d3t::UIWindow *const window = g_movesWindows[13 + button])
                    window->alpha = alpha;
                ++button;
            }
        }
        d3t::UIWindow *const left = g_movesWindows[10];
        d3t::UIWindow *const right = g_movesWindows[11];
        if (left)
            left->alpha = 0.0f;
        if (right)
            right->alpha = 0.0f;
        std::int32_t leftAlpha = 0x92;
        std::int32_t rightAlpha = 0x92;
        if (index == g_movesScreen.selectedMove) {
            const float pulse = g_uiAnimSlots[static_cast<std::size_t>(g_movesScreen.highlightAnimSlot)].current[0];
            leftAlpha = static_cast<std::int32_t>(pulse * 255.0f);
            rightAlpha = static_cast<std::int32_t>((1.0f - pulse + 0.15f) * 255.0f);
        }
        if (left)
            left->colour = withAlpha(left->colour, leftAlpha);
        if (right)
            right->colour = withAlpha(right->colour, rightAlpha);
    }
    UIGrid_AfterDraw(g_movesGridTemplate, grid, index, 5, -1, *holder, panel);
}


std::uint32_t MovesScreen_Load() {
    UIBaseRuntime &element = g_uiElements[kUiSubMenuMoves];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/gamemenu_New/SubMenu_Moves.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}


std::int32_t UI_FindViewButton(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U || !id)
        return -1;
    const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 17)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiButtonWidgets.size())
            continue;
        const char *const name = g_uiButtonWidgets[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}



bool UI_SetViewImageTexture(d3t::UIViewImage &image, std::uint32_t texture) {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    const bool described = GetTexture2DDimensions(texture, &width, &height);
    if (!described) {
        texture = d3t::g_defaultWhiteTexture;
        GetTexture2DDimensions(texture, &width, &height);
    }
    image.textureHandle = static_cast<std::int32_t>(texture);
    image.width = static_cast<std::uint16_t>(width);
    image.height = static_cast<std::uint16_t>(height);
    image.region = {0, 0, image.width, image.height};
    return described;
}








std::int64_t MovesScreen_IsSelected() {
    MovesScreenElement &moves = g_movesScreen;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuMoves].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_movesWindows.size(); ++window)
        g_movesWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kMovesWindowIds[window]));
    for (std::size_t label = 0; label < g_movesLabels.size(); ++label) {
        d3t::UILabelWidget *const record = UI_LabelRecord(UI_FindViewLabel(slot, kMovesLabelIds[label]));
        g_movesLabels[label] = record;
        if (record) {
            std::snprintf(g_movesLabelTexts[label].data(), g_movesLabelTexts[label].size(), "%s", record->text ? record->text : "");
            record->text = g_movesLabelTexts[label].data();
        }
    }
    for (std::size_t image = 0; image < g_movesColourImages.size(); ++image)
        g_movesColourImages[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kMovesColourImageIds[image]));
    for (std::size_t item = 0; item < g_movesLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kMovesLayoutItemIds[item]);
        g_movesLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    for (std::size_t icon = 0; icon < g_movesButtonIcons.size(); ++icon)
        g_movesButtonIcons[icon] = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, kMovesButtonIconIds[icon]));
    for (std::size_t button = 0; button < g_movesButtons.size(); ++button) {
        const std::int32_t index = UI_FindViewButton(slot, kMovesButtonIds[button]);
        g_movesButtons[button] = static_cast<std::uint32_t>(index) < g_uiButtonWidgets.size() ? &g_uiButtonWidgets[static_cast<std::size_t>(index)] : nullptr;
    }
    moves.buttonAnimSlot = UIAnimAllocSlot();
    moves.buttonAnimFrame = 0;
    if (moves.buttonAnimSlot != -1)
        MovesScreen_StartButtonFrame(moves.buttonAnimSlot);
    moves.highlightAnimSlot = UIAnimAllocSlot();
    if (moves.highlightAnimSlot != -1) {
        UIAnimSlot &highlight = g_uiAnimSlots[static_cast<std::size_t>(moves.highlightAnimSlot)];
        highlight.loopMode = 2;
        highlight.allocated = 1;
        highlight.status = 1;
        highlight.interpolation = 2;
        highlight.start[0] = 1.0f;
        highlight.current[0] = 1.0f;
        highlight.end[0] = 0.15f;
        highlight.delay = 0.0f;
        highlight.elapsed = 0.0f;
        highlight.duration = 0.3f;
        highlight.reserved40 = -1;
    }
    if (const UIColourImageWidget *const ryo = g_movesColourImages[3]; ryo && slot <= 0x4bU) {
        d3t::UIViewImage &image = g_uiViewSlots[slot].images[static_cast<std::size_t>(ryo->image)];
        moves.sceneTextureWidth = image.width;
        moves.sceneTextureHeight = image.height;
        const Texture2DDescription description{.width = moves.sceneTextureWidth,
            .height = moves.sceneTextureHeight,
            .arraySize = 1,
            .mipLevels = 1,
            .format = 2,
            .bindFlags = 4,
            .sampleCount = 1};
        moves.sceneTexture = CreateTexture2D(&description);
        if (image.textureHandle != -1) {
            moves.sceneTextureBound = true;
            static_cast<void>(UI_SetViewImageTexture(image, moves.sceneTexture));
        }
    }
    UIGrid &grid = g_movesGrid;
    grid.columns = 1;
    grid.visibleRows = 8;
    grid.rows = 51;
    grid.showEmptyCells = 0;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_movesWindows[1];
    grid.rect = {};
    grid.drawCell = MovesScreen_DrawCell;
    grid.mouseCallbacks = {.entered = UpdateSelectedMoveInMovesScreen, .hovered = nullptr, .left = nullptr, .clicked = MovesScreen_ConfirmMove, .isHovered = 0};
    grid.getElement = MovesScreen_GetElement;
    grid.rowHeight = nullptr;
    grid.navigateSound = 6;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.scrollSpeed = 0.25f;
    MovesScreen_SetListCount(51);
    UIGridTemplate &cellTemplate = g_movesGridTemplate;
    cellTemplate.initialized = 0;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.windows = g_movesWindows.data();
    cellTemplate.layoutItems = g_movesLayoutItems.data();
    cellTemplate.labels = g_movesLabels.data();
    cellTemplate.first = 5;
    cellTemplate.end = 25;
    cellTemplate.clipWindows[0] = 9;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.ids = kMovesWindowIds.data();
    cellTemplate.layoutFirst = 0;
    cellTemplate.layoutEnd = 28;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 2;
    return 1;
}


std::int64_t MovesScreen_Reset() {
    g_movesWindows = {};
    g_movesLabels = {};
    g_movesColourImages = {};
    return 0;
}





void MovesScreen_Render() {
    MovesScreenElement &moves = g_movesScreen;
    BeginMovesScreenViewTransition();
    moves.adjustFightMove = CreateMessageBoxForFightMoveRemapping;
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    UpdateWidget();
    g_uiGridInputEnabled = 1;
    g_uiFocusedList = nullptr;
    UI_RestartCurrentGridSelection();
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuMoves].config.uiHandleIndex);
    if (const UIColourImageWidget *const ryo = g_movesColourImages[3]) {
        if (slot <= 0x4bU) {
            d3t::UIViewImage &image = g_uiViewSlots[slot].images[static_cast<std::size_t>(ryo->image)];
            if (image.textureHandle != -1) {
                moves.sceneTextureBound = true;
                static_cast<void>(UI_SetViewImageTexture(image, moves.sceneTexture));
            }
        }
        g_pMovesScreenSceneBuffer = MovesScreen_RenderScene;
    } else {
        g_pMovesScreenSceneBuffer = nullptr;
    }
    if (g_movesWindows[2]) {
        for (std::size_t window = 2; window < 5; ++window) {
            if (g_movesWindows[window])
                g_movesWindows[window]->layout.width.scale = 0.0f;
        }
    }
    UIGrid_ResetRuntime(g_movesGrid);
    g_uiGridInputEnabled = 1;
    g_uiFocusedList = nullptr;
    UI_RestartCurrentGridSelection();
}




std::int64_t MovesScreen_Update() {
    MovesScreenElement &moves = g_movesScreen;
    if (!moves.initialized && !DX11::g_ppD3DDeviceContext) {
        moves.initialized = 1;
        InitMovesScreenPipelineContext();
        LoadRyoForMovesScreen(0, 0, false);
        RenderMovesScreenSceneToTexture();
    }
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuMoves];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_movesGrid, element.index);
    if (moves.buttonAnimSlot != -1 && std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(moves.buttonAnimSlot)].current[0]) == 1) {
        MovesScreen_StartButtonFrame(moves.buttonAnimSlot);
        moves.buttonAnimFrame = moves.buttonAnimFrame + 1 > 7 ? 0 : moves.buttonAnimFrame + 1;
    }
    return 0;
}




std::int64_t MovesScreen_IsPressed() {
    MovesScreenElement &moves = g_movesScreen;
    const std::int32_t selected = UIGrid_SelectedIndex(g_movesGrid);
    std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & 0x400U) != 0 && moves.adjustFightMove) {
        moves.adjustFightMove(selected < 0 ? moves.fightMoveCount : moves.fightMoves[static_cast<std::size_t>(selected)]);
        pressed = g_inputState.slots[0].pressedPriority;
    }
    if (g_isInGame || (pressed & 0x200U) == 0) {
        if ((pressed & 4U) != 0)
            SetHelpDetailsByIndex(51);
        return UIGrid_IsPressed(g_movesGrid);
    }
    UIBaseRuntime &element = g_uiElements[kUiSubMenuMoves];
    if ((g_currUIState == -1 || g_currUIState == kUiSubMenuMoves) && (!element.isVisible || !element.destroy || element.destroy() == 1 || g_busy_reset == 1))
        element.isVisible = 0;
    UIBaseRuntime &owner = g_uiElements[30];
    owner.isHandledExternally = 0;
    if (owner.elementDraw)
        owner.elementDraw();
    return 0;
}



std::int64_t MovesScreen_Destroy() {
    if (g_movesScreenViewActive == 1) {
        (void)VFMA_RequestMove(VFMoveRequestFlags::ReturnAbsolute);
        g_movesScreenTransitionFlags = 0;
        g_pendingMovePopupMoveId = 0;
        g_pendingMovePopupAnimSetId = 0;
        g_movesScreenViewActive = 0;
    }
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    g_pMovesScreenSceneBuffer = nullptr;
    g_movesScreen.destroyRyoPending = 1;
    g_movesScreen.initialized = 0;
    return 1;
}


std::int64_t MovesScreen_GetPromptIconSet() {
    constexpr std::array<std::int16_t, 5> kPrompts{45, 44, 58, -1, -1};
    g_searchPromptIconSlots = kPrompts;
    for (std::size_t slot = 0; slot < kPrompts.size(); ++slot)
        g_movesPromptIconSet[slot] = static_cast<std::uint16_t>(kPrompts[slot]);
    return reinterpret_cast<std::int64_t>(g_movesPromptIconSet.data());
}



void MovesScreen_SetTextureAlphas(float front, float game) {
    if (UIColourImageWidget *const image = g_movesColourImages[1])
        image->alpha = front;
    for (const std::size_t index : {std::size_t{0}, std::size_t{2}}) {
        if (UIColourImageWidget *const image = g_movesColourImages[index])
            image->alpha = game;
    }
}




std::int64_t MovesScreen_GetContentPanel() {
    const bool inGame = g_isInGame != 0;
    if (d3t::UIWindow *const root = g_movesWindows[0])
        std::snprintf(root->layout.parentId.data(), root->layout.parentId.size(), "%s", inGame ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    if (UIButtonWidget *const panel = g_movesButtons[0])
        panel->colour = inGame ? 0xff322b1cU : 0xffa2997cU;
    const std::uint32_t background = inGame ? 0xdc272415U : 0x4628414bU;
    for (const std::size_t index : {std::size_t{1}, std::size_t{5}, std::size_t{6}, std::size_t{7}}) {
        if (UIButtonWidget *const button = g_movesButtons[index])
            button->colour = background;
    }
    if (UIColourImageWidget *const xBase = g_movesColourImages[4])
        xBase->colour = inGame ? 0x37ffffffU : 0xe13c3778U;
    if (inGame)
        MovesScreen_SetTextureAlphas(0.0f, 1.0f);
    else
        MovesScreen_SetTextureAlphas(1.0f, 0.0f);
    return 0;
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {




void RenderingRelatedInventory(struct nk_image &image, std::uint16_t item, std::int32_t mask) {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSharedItems].config.uiHandleIndex);
    if (slot > 75U || item >= g_inventoryIconAtlasRects.size())
        return;
    const InventoryIconAtlasRect &cell = g_inventoryIconAtlasRects[item];
    const std::size_t sheet = static_cast<std::size_t>(cell.sheet) + (mask == 1 ? kInventoryIconMaskSheetOffset : 0);
    const std::int32_t found = sheet < g_inventoryIconSheetImageIndices.size() ? g_inventoryIconSheetImageIndices[sheet] : -1;
    if (static_cast<std::uint32_t>(found) >= d3t::kS1UIViewImageCapacity)
        return;
    const d3t::UIViewImage &source = g_uiViewSlots[slot].images[static_cast<std::size_t>(found)];
    image.handle.id = source.textureHandle;
    image.w = source.width;
    image.h = source.height;
    const float width = static_cast<float>(source.width);
    const float height = static_cast<float>(source.height);
    const float left = static_cast<float>(cell.left) * 0.125f * width;
    const float top = height - static_cast<float>(cell.top) * 0.125f * height;
    image.region[0] = static_cast<unsigned short>(static_cast<std::int32_t>(left));
    image.region[1] = static_cast<unsigned short>(static_cast<std::int32_t>(top));
    image.region[2] = static_cast<unsigned short>(static_cast<std::int32_t>(static_cast<float>(cell.right) * 0.125f * width - left));
    image.region[3] = static_cast<unsigned short>(static_cast<std::int32_t>(height - static_cast<float>(cell.bottom) * 0.125f * height - top));
}


std::int64_t GetIconSet() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSharedItems].config.uiHandleIndex);
    if (slot <= 75U) {
        for (std::size_t sheet = 0; sheet < g_inventoryIconSheetImageIndices.size(); ++sheet)
            g_inventoryIconSheetImageIndices[sheet] = UI_FindViewImage(slot, g_inventoryIconSheetNames[sheet]);
    }
    return 1;
}



std::int64_t SharedItems_IsPressed() {
    const IconPresentationOwner &icon = g_iconPresentation;
    const bool items = g_uiElements[kUiSubMenuItems].isVisible != 0;
    const bool walkman = items && icon.sharedItemPreviewValue == 1;
    if (g_uiElements[kUiHelpNew].isVisible || ((!icon.sharedItemMenuState || walkman) && (!walkman || g_uiElements[kUiSubMenuCassettes].isVisible)) ||
        (g_inputState.slots[0].pressedPriority & 4U) == 0) {
        return 1;
    }
    g_pauseInputRequest = 1;
    return 0;
}

} // namespace shenmue::s1
