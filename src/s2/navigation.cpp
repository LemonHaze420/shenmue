#include "s2/navigation.hpp"
#include "s2/navi_window.hpp"
#include "s2/legacy_sprite.hpp"
#include "d3t/texture.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/help.hpp"
#include "s2/gauge.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/navi.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

namespace shenmue::s2 {
namespace {

constexpr shen_name naviTaskName{SHEN_NAME(NAVI)};

HLib::Task *navigationTask{};

NaviWindowState naviWindow{};
Vector4 naviWindowPosition{};
float naviWindowVelocity{};
LegacyIconSprite naviWindowHeadingSprite{}, naviWindowSelectionSprite{};
std::array<std::int32_t, 6> naviWindowLabelTextures{}, naviWindowSecondaryTextures{}, naviWindowLabels{}, naviWindowSecondaryLabels{};
std::array<std::int32_t, 2> naviWindowPreviewTextures{};
std::int32_t naviWindowPreviewTexture{};

std::uint32_t NaviFolderEntryMask(std::int32_t page) {
    if (page >= 3)
        return 0;
    const NaviFolderPage &folder = naviFolderPages[page];
    if (!folder.entries)
        return 0;
    std::uint32_t mask = 0, bit = 1;
    for (std::int32_t index = 0; index < NWIN_FOLDER_COUNTS[page]; ++index) {
        const NaviFolderEntry &entry = folder.entries[index];
        if (!(entry.flags & 2U) && (entry.itemId < 0 || GetItemState(static_cast<std::uint16_t>(entry.itemId))))
            mask |= bit;
        bit = (bit << 1U) | (bit >> 31U);
    }
    return mask;
}

std::int32_t NaviFolderEntryCount(std::int32_t page) {
    if (page >= 3)
        return 0;
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < NWIN_FOLDER_COUNTS[page]; ++index) {
        if (!(naviFolderPages[page].entries[index].flags & 2U))
            ++count;
    }
    return count;
}

void ReleaseNaviWindowStrips(std::uint8_t flags) {
    if (!(flags & 3U))
        return;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (flags & 1U) {
            if (naviWindowLabelTextures[index] >= 0)
                ReleaseNamedTexture(naviWindowLabelTextures[index]);
            if (naviWindowSecondaryTextures[index] >= 0)
                ReleaseNamedTexture(naviWindowSecondaryTextures[index]);
            naviWindowSecondaryTextures[index] = naviWindowLabelTextures[index] = -1;
        }
        if (flags & 2U)
            naviWindowSecondaryLabels[index] = naviWindowLabels[index] = -1;
    }
}

void PublishNaviWindowLabel(NaviWindowState &work, const char *label, const char *secondary, std::int32_t slot) {
    if (label) {
        (void)RasterizeItemLabelNullsub(static_cast<std::byte *>(work.labelScratch), 256, label, 0, 0);
        (void)ReplacePtxTexture(naviWindowLabelTextures[slot], work.labelScratch);
        naviWindowLabels[slot] = naviWindowLabelTextures[slot];
    }
    if (secondary) {
        (void)RasterizeItemLabelNullsub(static_cast<std::byte *>(work.labelScratch), 256, secondary, 0, 0);
        (void)ReplacePtxTexture(naviWindowSecondaryTextures[slot], work.labelScratch);
        naviWindowSecondaryLabels[slot] = naviWindowSecondaryTextures[slot];
    }
}

std::uint32_t BuildNaviWindowHeading(std::int32_t slot, std::int32_t category) {
    const bool available = NaviFolderEntryMask(category) != 0;
    const NaviFolderPage &folder = naviFolderPages[category];
    PublishNaviWindowLabel(naviWindow, available ? folder.label : "? ? ? ", available ? folder.secondaryLabel : "", slot);
    return available;
}

void BuildNaviWindowRootLabels(NaviWindowState &work) {
    ReleaseNaviWindowStrips(2);
    std::uint32_t bit = 1;
    for (std::int32_t category = 0; category < 3; ++category) {
        if (BuildNaviWindowHeading(category, category))
            work.availablePages |= bit;
        bit = (bit << 1U) | (bit >> 31U);
    }
}

const NaviFolderEntry *GetNaviWindowFolderEntries(std::int32_t category) {
    return category >= 3 ? nullptr : naviFolderPages[category].entries;
}

void BuildNaviWindowFolderLabels(NaviWindowState &work, std::int32_t category) {
    const std::int32_t count = NaviFolderEntryCount(category);
    const std::uint32_t available = NaviFolderEntryMask(category);
    const NaviFolderEntry *const entries = GetNaviWindowFolderEntries(category);
    ReleaseNaviWindowStrips(2);
    (void)BuildNaviWindowHeading(0, category);
    if (!entries)
        return;
    for (std::int32_t row = 0; row < 5; ++row) {
        const std::int32_t entry = row + work.firstVisibleEntry;
        if (entry >= count)
            break;
        const bool unlocked = (available & (1U << (entry & 31))) != 0;
        PublishNaviWindowLabel(work, unlocked ? entries[entry].label : "? ? ? ", unlocked ? entries[entry].secondaryLabel : "  ", row + 1);
    }
}

void ReleaseNaviWindowAllocations() {
    if (naviWindow.mapImageData)
        HLib::FreeTaskData(naviWindow.mapImageData);
    if (naviWindow.labelScratch)
        HLib::FreeTaskData(naviWindow.labelScratch);
    if (naviWindow.dummySpriteData)
        HLib::FreeTaskData(naviWindow.dummySpriteData);
    naviWindow.dummySpriteData = naviWindow.mapImageData = naviWindow.labelScratch = nullptr;
}

void LoadNaviWindowPageSource() {
    if (naviWindow.mapImageData) {
        HLib::FreeTaskData(naviWindow.mapImageData);
        naviWindow.mapImageData = nullptr;
    }
    std::size_t bytes = 0;
    if (!naviWindow.page)
        (void)LoadLegacySprite("sprite", "NAVI_IMG.SPR", NWIN_ROOT_FILE, naviWindow.mapImageData, bytes);
    else if (naviWindow.categorySelection < 3)
        (void)LoadLegacySprite("sprite", naviFolderPages[naviWindow.categorySelection].sprite, NWIN_FOLDER_FILE, naviWindow.mapImageData, bytes);
}

void RefreshNaviWindowPreview() {
    const std::byte *cursor = static_cast<const std::byte *>(naviWindow.mapImageData);
    std::uint32_t selected = naviWindow.page ? naviWindow.entrySelection + naviWindow.firstVisibleEntry : naviWindow.categorySelection;
    const std::uint32_t available = naviWindow.page ? naviWindow.availableEntries : naviWindow.availablePages;
    naviWindowPreviewTexture = -1;
    std::int32_t dummySize = 0;
    if (!(available & (1U << (selected & 31U)))) {
        if (naviWindow.page)
            dummySize = 256;
        cursor = static_cast<const std::byte *>(naviWindow.dummySpriteData);
        selected = 0;
    }
    if (!cursor)
        return;
    std::int32_t texture = 0;
    for (;;) {
        std::uint32_t tag = 0, size = 0;
        std::memcpy(&tag, cursor, 4);
        if (!tag)
            return;
        if (tag == SHEN_NAME(TEXN) && texture++ == selected) {
            std::int16_t width = 0, height = 0;
            std::memcpy(&width, cursor + 40, 2);
            std::memcpy(&height, cursor + 42, 2);
            const std::int32_t slot = width != 128;
            (void)ReplacePtxTexture(naviWindowPreviewTextures[slot], cursor);
            naviWindowPreviewTexture = naviWindowPreviewTextures[slot];
            naviWindow.mapWidth = dummySize ? dummySize : width;
            naviWindow.mapHeight = dummySize ? dummySize : height;
            return;
        }
        std::memcpy(&size, cursor + 4, 4);
        cursor += size;
    }
}

std::uint64_t AllocateNaviWindowLabelTextures() {
    naviWindow.labelScratch = HLib::AllocateTaskData(GetTextureStorageSize(0x8089, 256, 16) + 48ULL, NWIN_LABEL_SCRATCH);
    if (!naviWindow.labelScratch)
        return 0;
    for (std::uint32_t index = 0; index < 6; ++index) {
        const std::uint32_t suffix = (index + 48U) << 24U;
        ParseGuiPtx(static_cast<std::byte *>(naviWindow.labelScratch), 256, 16, NWIN_FOLDER_FILE.name, suffix | make_shen_name('N', 'A', '0', '\0'));
        naviWindowLabelTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(naviWindow.labelScratch));
        ParseGuiPtx(static_cast<std::byte *>(naviWindow.labelScratch), 256, 16, NWIN_FOLDER_FILE.name, suffix | make_shen_name('R', 'B', '0', '\0'));
        naviWindowSecondaryTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(naviWindow.labelScratch));
    }
    return static_cast<std::uint32_t>(naviWindowSecondaryTextures[5]);
}

std::int64_t InitializeNaviWindowResources() {
    std::size_t bytes = 0;
    (void)LoadLegacySprite("sprite", "NV_DUMMY.SPR", NWIN_DUMMY_FILE, naviWindow.dummySpriteData, bytes);
    if (!naviWindow.dummySpriteData)
        return 0;
    std::byte *const ptx = static_cast<std::byte *>(HLib::AllocateTaskData(GetTextureStorageSize(0x8081, 256, 256) + 48ULL, NWIN_TEXTURE_SCRATCH));
    if (!ptx)
        return 0;
    for (std::uint32_t index = 0; index < 2; ++index) {
        const std::uint16_t size = static_cast<std::uint16_t>(128 + 128 * index);
        ParseGuiPtx(ptx, size, size, NWIN_PREVIEW_TEXTURE.name, ((index + 48U) << 24U) | make_shen_name('N', 'V', '0', '\0'), 0x8081);
        naviWindowPreviewTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(ptx));
    }
    HLib::FreeTaskData(ptx);
    AllocateNaviWindowLabelTextures();
    BuildNaviWindowRootLabels(naviWindow);
    LoadNaviWindowPageSource();
    RefreshNaviWindowPreview();
    return 1;
}

void InitializeNaviWindowSprites() {
    naviWindowPosition = NWIN_POSITION_INITIAL;
    naviWindowVelocity = 0.1f;
    naviWindowHeadingSprite.x = naviWindowHeadingSprite.y = 0;
    naviWindowHeadingSprite.scaleX = naviWindowHeadingSprite.scaleY = 1;
    naviWindowHeadingSprite.depth = 0;
    naviWindowHeadingSprite.flags = 0;
    naviWindowHeadingSprite.rotation = 0;
    naviWindowHeadingSprite.icon = 1553;
    naviWindowHeadingSprite.width = 160;
    naviWindowHeadingSprite.height = 3;
    naviWindowHeadingSprite.alignment = 4;
    naviWindowSelectionSprite.scaleX = naviWindowSelectionSprite.scaleY = 1;
    naviWindowSelectionSprite.depth = 3.1f;
    naviWindowSelectionSprite.flags = 1024;
    naviWindowSelectionSprite.rotation = 0;
    naviWindowSelectionSprite.icon = 771;
    naviWindowSelectionSprite.width = naviWindowSelectionSprite.height = 32;
    naviWindowSelectionSprite.alignment = 5;
    for (std::int32_t index = 0; index < 6; ++index)
        naviWindowLabelTextures[index] = naviWindowSecondaryTextures[index] = naviWindowLabels[index] = naviWindowSecondaryLabels[index] = -1;
    naviWindowPreviewTexture = -1;
}

void DrawNaviWindowLabel(std::int32_t row, const Vector2 &position, std::uint32_t colour) {
    LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .secondaryColour = colour | 0xff000000U, .uv = {0, 0, 1, 1}};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::VerticalStretch, TruncateSpriteCoordinate(position.x), TruncateSpriteCoordinate(position.y + 10), 256,
        32, ScreenMode::Centered);
    if (naviWindowLabels[row] >= 0)
        LegacySpriteNullsub(packet, naviWindowLabels[row], 0.6f);
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::VerticalStretch, TruncateSpriteCoordinate(position.x), TruncateSpriteCoordinate(position.y - 18), 256,
        16, ScreenMode::Centered);
    if (naviWindowSecondaryLabels[row] >= 0)
        LegacySpriteNullsub(packet, naviWindowSecondaryLabels[row], 0.6f);
}

void DrawNaviWindow(void *) {
    if (!(naviWindow.drawFlags & 7U))
        return;
    (void)GetNaviWindowFolderEntries(naviWindow.categorySelection);
    Vector2 position{naviWindowPosition.x, naviWindowPosition.y};
    DrawLegacyItemWindow(position, 0.11f, 11, 9, ScreenMode::Centered);
    position.x += 100;
    if (naviWindowPreviewTexture >= 0) {
        LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xff000000U, .uv = {0, 0, 1, 1}};
        const std::int32_t x = TruncateSpriteCoordinate(position.x - 8), y = TruncateSpriteCoordinate(position.y - 8);
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, 256, 256, ScreenMode::Centered);
        packet.quad.flags |= 1024;
        (void)shenSubmitQuad(LegacyColourQuad{.quad = packet.quad, .colour = packet.colour}, 0.045f);
        if (naviWindow.drawFlags & 2U) {
            packet.colour = 0xa0ffffffU;
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, 256, 256, ScreenMode::Centered);
            packet.quad.flags |= 1024;
            LegacySpriteNullsub(packet, naviWindowPreviewTexture, 0.6f);
        }
        position.x = naviWindowPosition.x + 204;
        position.y = naviWindowPosition.y + 164;
    }
    Vector2 labelPosition{naviWindowPosition.x - 200, naviWindowPosition.y - 28};
    if (naviWindow.page) {
        const std::int32_t rows = naviWindow.entryCount + 1 > 6 ? 6 : naviWindow.entryCount + 1;
        labelPosition.y = naviWindowPosition.y - rows * 28.0f + 28;
        DrawNaviWindowLabel(0, labelPosition, 0xff00ff00U);
        labelPosition.y += 56;
        for (std::int32_t row = 0; row < 5; ++row) {
            DrawNaviWindowLabel(row + 1, labelPosition, row == naviWindow.entrySelection ? 0xffff0000U : 0xff000000U);
            labelPosition.y += 56;
        }
    } else {
        for (std::int32_t row = 0; row < 3; ++row) {
            DrawNaviWindowLabel(row, labelPosition, row == naviWindow.categorySelection ? 0xffff0000U : 0xff000000U);
            labelPosition.y += 56;
        }
    }
}

void ReleaseNaviWindowPreviewTextures() {
    for (std::int32_t &texture : naviWindowPreviewTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
}

} // namespace

bool NaviIsDisplayable(bool skipMapItemRequirement) {
    if (IsNaviDisplayOverridden())
        return true;
    if (GetCurrentAuthorId().name != 0 || g_currentMap.name == SHEN_NAME(ARK0) || g_currentMap.name == SHEN_NAME(KESS) ||
        GetShouldRenderIcons(RenderIconSlot::ItemSelect) || IsScenarioBusyForMainIcon() || IsItemSelectable() || g_fightUiActive != 0 ||
        !plymIsSearchStartRequested()) {
        return false;
    }
    const std::int32_t configIndex = navGetNumEntriesFor(g_currentMap);
    const std::span<const NaviMapConfig> configs = GetNaviMapConfigs();
    if (!skipMapItemRequirement && configIndex >= 0 && static_cast<std::size_t>(configIndex) < configs.size()) {
        const std::int16_t itemId = configs[static_cast<std::size_t>(configIndex)].itemId;
        if (itemId != -1 && GetItemState(static_cast<std::uint16_t>(itemId)) == 0) {
            return false;
        }
    }
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control && IsScreenFilmNotShowing(control->screenFilmTask) && authGetLoaded() == 0 && g_moveUiClockAndNavigationHold == 0;
}

namespace {

void NAVI_CtrlNavi_Callback(void *taskData) {
    NaviTaskData *const data = static_cast<NaviTaskData *>(taskData);
    if (!data)
        return;

    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(ctrlGetPadData());
    const bool admitted = input && navigationTask && HLib::GetTaskParameter(navigationTask) && NaviIsDisplayable(false) && plymIsSearchable() &&
        !plymIsSearchBlocked() && !IsScenarioBusy() && g_fightUiActive == 0 && srchCheckFlags(6) == 0 && srchCheckFlags(3) < 0;
    if (admitted)
        UpdateNaviInteraction(*input);
    else
        ResetNaviInputPhase();

    UpdateNaviInputAnimation(*data, *input);
    HLib::ExecuteCallbackNow(PrepareNaviFrame, data);
}

void NAVI_DummyWait_Callback(void *taskData) {
    NaviTaskData *const data = static_cast<NaviTaskData *>(taskData);
    if (!data)
        return;
    InitializeNaviInputState(*data);
    HLib::SetCurrentTaskTo(NAVI_CtrlNavi_Callback);
}

void NVW_CtrlNaviWindow_Callback(void *) {
    std::uint16_t input = GetPadControlData(1);
    (void)GetPadControlData(2);
    switch (naviWindow.phase) {
    case 1:
        naviWindow.phase = 2;
        break;
    case 2:
        naviWindow.phase = 3;
        break;
    case 3:
        InitializeNaviWindowResources();
        naviWindow.drawFlags |= 7U;
        naviWindow.phase = 4;
        break;
    case 4:
        if (AdvanceIconPosition(naviWindowPosition.x, naviWindowVelocity, 320.0f))
            naviWindow.phase = 7;
        break;
    case 5:
        RefreshNaviWindowPreview();
        set_icon_select_flags_to(naviWindow.page == 0 ? 0x600U : 0x200U);
        naviWindow.phase = 7;
        break;
    case 6:
        if (RetreatIconPosition(naviWindowPosition.x, naviWindowVelocity, -300.0f - GetScreenAlignmentOffset())) {
            naviWindow.page = naviWindow.requestedPage;
            ReleaseNaviWindowStrips(2);
            if (naviWindow.page)
                BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
            else
                BuildNaviWindowRootLabels(naviWindow);
            LoadNaviWindowPageSource();
            RefreshNaviWindowPreview();
            naviWindow.mapScaleX = naviFolderPages[naviWindow.categorySelection].scaleX;
            naviWindow.mapScaleY = naviFolderPages[naviWindow.categorySelection].scaleY;
            naviWindow.phase = 4;
        }
        break;
    case 7: {
        naviWindow.drawFlags |= 2U;
        const std::uint32_t iconFlags = naviWindow.page == 0 ? 0x600U : 0x200U;
        if (naviWindow.page != 0)
            input &= ~0x400U;
        if (input & 0x400U) {
            if (naviWindow.availablePages & (1U << (naviWindow.categorySelection & 31))) {
                naviWindow.requestedPage = naviWindow.page + 1;
                naviWindow.entrySelection = naviWindow.firstVisibleEntry = 0;
                naviWindow.phase = 6;
                input = 0;
            } else {
                (void)ExecuteAicaCommand(0x800abU, 0, 0);
            }
        }
        if (input & 0x200U) {
            if (naviWindow.page <= 0)
                naviWindow.phase = 8;
            else {
                naviWindow.phase = 6;
                naviWindow.requestedPage = naviWindow.page - 1;
            }
            input = 0;
        }
        if (input & 0xf000U) {
            const std::int32_t total = naviWindow.page ? NaviFolderEntryCount(naviWindow.categorySelection) : 3;
            const std::int32_t visible = naviWindow.page ? (total < 5 ? total : 5) : 3;
            std::int32_t selection = naviWindow.page ? naviWindow.entrySelection : naviWindow.categorySelection;
            const std::int32_t previous = selection + naviWindow.firstVisibleEntry;
            if ((input & 0x1000U) && --selection < 0) {
                selection = 0;
                if (naviWindow.page == 1 && naviWindow.firstVisibleEntry > 0) {
                    --naviWindow.firstVisibleEntry;
                    ReleaseNaviWindowStrips(2);
                    BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
                }
            }
            if ((input & 0x2000U) && ++selection >= visible) {
                selection = visible - 1;
                if (naviWindow.page == 1 && visible + naviWindow.firstVisibleEntry < total) {
                    ++naviWindow.firstVisibleEntry;
                    ReleaseNaviWindowStrips(2);
                    BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
                }
            }
            if (naviWindow.page)
                naviWindow.entrySelection = selection;
            else
                naviWindow.categorySelection = selection;
            naviWindow.entryCount = NaviFolderEntryCount(naviWindow.categorySelection);
            if (selection + naviWindow.firstVisibleEntry != previous) {
                naviWindow.drawFlags &= ~2U;
                naviWindow.phase = 5;
            }
        }
        naviWindow.entryCount = NaviFolderEntryCount(naviWindow.categorySelection);
        naviWindow.availableEntries = NaviFolderEntryMask(naviWindow.categorySelection);
        set_icon_select_flags_to(iconFlags);
        break;
    }
    case 8:
        if (RetreatIconPosition(naviWindowPosition.x, naviWindowVelocity, -300.0f - GetScreenAlignmentOffset())) {
            ReleaseNaviWindowPreviewTextures();
            icsToggleIconState(3, 0);
            HLib::SetupTimedTask(naviWindow.task, 0);
            ReleaseNaviWindowStrips(3);
            naviWindowPreviewTexture = -1;
            ReleaseNaviWindowAllocations();
            naviWindow.phase = 0;
            EV_EventSetUpHelpIcon_0(0x17, 0);
            EV_NPC_NewLoadSw(naviWindow.savedNpcLoadState);
        }
        break;
    default:
        break;
    }
    HLib::ExecuteCallbackNow(DrawNaviWindow, nullptr);
}

} // namespace

HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex) {
    navigationTask = nullptr;

    const std::int32_t configIndex = navGetNumEntriesFor(g_currentMap);
    if (configIndex < 0) {
        SelectNaviMapMarkers(g_currentMap);
        return nullptr;
    }

    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NAVI_DummyWait_Callback, nextFunctionIndex, 0, sizeof(NaviTaskData), naviTaskName);
    navigationTask = task;
    if (!task) {
        SelectNaviMapMarkers(g_currentMap);
        return nullptr;
    }
    InitializeNaviResources(task, configIndex);
    return task;
}

HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex) {
    naviWindow.phase = naviWindow.page = 0;
    naviWindow.requestedPage = -1;
    naviWindow.task = nullptr;
    naviWindow.drawFlags = 0;
    naviWindow.mapImageData = naviWindow.labelScratch = naviWindow.dummySpriteData = nullptr;
    naviWindow.availablePages = naviWindow.availableEntries = 0;
    InitializeNaviWindowSprites();
    naviWindow.task = HLib::EnqueueTaskWithoutParameter(NVW_CtrlNaviWindow_Callback, nextFunctionIndex, 0, TASK_NWIN.name);
    if (naviWindow.task)
        HLib::SetupTimedTask(naviWindow.task, 0);
    return naviWindow.task;
}

std::int64_t OpenNaviWindow() {
    naviWindow.dummySpriteData = naviWindow.mapImageData = naviWindow.labelScratch = nullptr;
    if (!naviWindow.task)
        return 0;
    naviWindowPreviewTexture = -1;

    naviWindow.availablePages = 0;
    for (std::size_t page = 0; page < naviFolderPages.size(); ++page) {
        if (NaviFolderEntryMask(static_cast<std::int32_t>(page)) != 0)
            naviWindow.availablePages |= 1U << page;
    }
    naviWindow.availableEntries = NaviFolderEntryMask(0);
    naviWindow.entryCount = NaviFolderEntryCount(0);
    if (!naviWindow.availablePages)
        return 0;

    naviWindow.savedNpcLoadState = static_cast<std::int32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    naviWindowPosition.x = -300.0f - GetScreenAlignmentOffset();
    naviWindow.phase = 1;
    naviWindow.page = 0;
    naviWindow.drawFlags = 0;
    naviWindow.categorySelection = 0;
    naviWindow.entrySelection = 0;
    EV_EventSetUpHelpIcon_0(0x74, 0);
    icsToggleIconState(3, 1);
    HLib::ResolveDeferredCallback(naviWindow.task);
    return 1;
}

std::int64_t CloseNaviWindow(std::int32_t force) {
    if (!naviWindow.task || !force || !naviWindow.phase)
        return 0;
    ReleaseNaviWindowPreviewTextures();
    ReleaseNaviWindowStrips(3);
    ReleaseNaviWindowAllocations();
    naviWindow.phase = 0;
    icsToggleIconState(3, 0);
    HLib::SetupTimedTask(naviWindow.task, 0);
    EV_NPC_NewLoadSw(naviWindow.savedNpcLoadState);
    return 1;
}

void ReleaseNavigationTaskOwners() {
    (void)CloseNaviWindow(1);
    ReleaseNaviWindowAllocations();
    if (naviWindow.task)
        HLib::DestroyTask(naviWindow.task);
    if (navigationTask)
        HLib::DestroyTask(navigationTask);
    naviWindow.task = nullptr;
    navigationTask = nullptr;
    ReleaseNaviResources();
}

} // namespace shenmue::s2
