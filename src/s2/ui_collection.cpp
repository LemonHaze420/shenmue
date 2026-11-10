#include "s2/ui.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/game/arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/scene_data.hpp"
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#include "s2/date_time.hpp"
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"

namespace shenmue::s2 {
using namespace ui_detail;

namespace ui_detail {

void BindCollectionLayout(const d3t::UIDefinition &definition, CollectionAuthoredLayout &layout) {
    BindAuthoredMenuLayout(definition, layout);
    layout.overrides.resize(definition.panels.size() + 1);
}
void ResolveCollectionNativeLayout(const d3t::UIDefinition &definition, CollectionAuthoredLayout &layout, const d3t::UIPanelBoundsOverride *explicitOverride) {
    std::size_t count{};
    for (std::size_t i = 0; i < definition.panels.size(); ++i) {
        const auto &panel = definition.panels[i];
        if (layout.parents[i] >= 0 || panel.parentId.empty() || (explicitOverride && explicitOverride->panel == i))
            continue;
        d3t::UIRect parent{};
        const d3t::UIPanelDefinition *source{};
        if (FindUIParentBounds(panel.parentId, parent, &source))
            layout.overrides[count++] = {i, d3t::ResolveUIPanelBounds(panel, parent, source->width, source->height)};
    }
    if (explicitOverride)
        layout.overrides[count++] = *explicitOverride;
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels,
        std::span{layout.overrides.data(), count}, GetUILayoutPaddingScale());
}

void ResolveCollectionLayout(std::size_t index) {
    auto &definition = uiDefinitions[static_cast<std::size_t>(collectionChildren[index])];
    auto &layout = collectionChild[index].layout;
    ResolveCollectionNativeLayout(definition, layout);
}
void DrawCollectionDefinition(UIElement element, AuthoredMenuLayout &layout, std::span<const std::uint8_t> excluded) {
    auto &definition = uiDefinitions[static_cast<std::size_t>(element)];
    if (!drawingUI)
        return;
    for (std::size_t i = 0; i < definition.textureObjects.size(); ++i) {
        const auto panel = layout.objectPanels[i];
        if (panel < 0 || (!excluded.empty() && excluded[static_cast<std::size_t>(panel)]))
            continue;
        const auto &object = definition.textureObjects[i];
        if (element == collectionChildren[1] && object.id == "UI_SubMenu::Collection_Maps::Image")
            continue;
        if (element == UIElement::SecretViewer && object.imageName == "Image_Placeholder")
            continue;
        if (element == UIElement::SubMenuPhotos) {
            const bool game = IsCameraIconHidden() != 0;
            if (object.id.find("Button_TextureFront_") != std::string::npos && game)
                continue;
            if (object.id.find("Button_TextureGame_") != std::string::npos && !game)
                continue;
            if (object.id == "SubMenu_Photos::TextureFront" && game)
                continue;
            if (object.id == "SubMenu_Photos::TextureGame" && !game)
                continue;
        }
        DrawNativeMenuObject(definition, definition.textureObjects[i], layout.objects[i], 1.0f);
    }
    for (std::size_t i = 0; i < definition.labels.size(); ++i) {
        const auto panel = layout.labelPanels[i];
        if (panel >= 0 && !excluded.empty() && excluded[static_cast<std::size_t>(panel)])
            continue;
        const auto &label = definition.labels[i];
        DrawMovesLabel(label, layout.labels[i], label.token.empty() ? std::string_view(label.text) : GetLocalizedSceneText(label.token), 1.0f, &definition);
    }
}
const d3t::UIImageDefinition *CollectionMapImage(std::int32_t image) {
    return CollectionLocalizedMapImage(image);
}
bool CollectionSpriteImage(std::uint16_t icon, d3t::UISharedSpriteImage &image) {
    return ResolveSharedItemImage(icon, image);
}
void CollectionMessageImage(UIMessageBox &message, const d3t::UISharedSpriteImage &image) {
    std::uint32_t width{}, height{};
    d3t::GetTexture2DDimensions(image.textureHandle, &width, &height);
    message.texture = image.textureHandle;
    message.textureWidth = static_cast<std::uint16_t>(width);
    message.textureHeight = static_cast<std::uint16_t>(height);
    message.cropX = static_cast<std::uint16_t>(image.source.x);
    message.cropY = static_cast<std::uint16_t>(image.source.y);
    message.cropWidth = static_cast<std::uint16_t>(image.source.width);
    message.cropHeight = static_cast<std::uint16_t>(image.source.height);
}
std::int32_t CollectionBioIndex(shen_name name) {
    for (std::size_t i = 0; i < collectionBioNames.size(); ++i)
        if (collectionBioNames[i].name == name.name)
            return static_cast<std::int32_t>(i);
    return 48;
}
void CollectionSnapshotBuilding(const SnapshotMetadataRecord &record, std::span<char> output) {
    const auto area = static_cast<std::uint32_t>(record.sortKeys[2]);
    const char *label{};
    switch (area) {
    case 0x1000e:
        label = "UIHELPL026";
        break; // @text EN: "Lucky Plaza" | JA: "好運中心"
    case 0x1001d:
        label = "UIHELPL022";
        break; // @text EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ"
    case 0x2000d:
    case 0x2000e:
        label = "UIHELPL103";
        break; // @text EN: "South Carmain Qr." | JA: "紅南街"
    case 0x20022:
        label = "UIHELPL069";
        break; // @text EN: "Dou Jiang Diner" | JA: "豆将軒"
    case 0x2004b:
    case 0x2004c:
    case 0x2004d:
        label = "UIHELPL091";
        break; // @text EN: "Man Mo Temple" | JA: "文武廟"
    case 0x20053:
        label = "UIHELPL113";
        break; // @text EN: "White Dynasty Qr." | JA: "白秦街"
    case 0x30044:
        label = "UIHELPL179";
        break; // @text EN: "Yellow Head Bldg." | JA: "黄天楼"
    case 0x40003:
        label = "UIHELPL182";
        break; // @text EN: "Yingshuihe" | JA: "緑野"
    case 0x30000:
    case 0x40000:
    case 0x40010:
        break;
    case 0x80000:
        std::snprintf(output.data(), output.size(), "???");
        return;
    default:
        label = GetLocationLabelId(record.area.name);
        const char *region = GetLocationLabelId(record.subArea.name);
        if (const char *shop = GetLocationLabelId(record.shop.name))
            label = shop;
        if (!label || (region && std::strcmp(region, label) < 0))
            label = region;
        break;
    }
    const auto location = label ? GetLocalizedSceneText(label) : std::string_view{};
    constexpr std::array<const char *, 4> regions{"UIMAREABER", "UIMAREWACH", "UIMAREKOWL",
        "UIMAREGUIL"}; // @text UIMAREABER EN: "Aberdeen" | JA: "香港仔" ; UIMAREWACH EN: "Wan Chai" | JA: "湾仔" ; UIMAREKOWL EN: "Kowloon" | JA: "九龍城" ; UIMAREGUIL EN: "Guilin" | JA: "桂林"
    const auto regionIndex = (area >> 16) - 1;
    const auto region = regionIndex < regions.size() ? GetLocalizedSceneText(regions[regionIndex]) : std::string_view{};
    std::snprintf(output.data(), output.size(), region.empty() ? "%.*s" : "%.*s, %.*s", static_cast<int>(location.size()), location.data(),
        static_cast<int>(region.size()), region.data());
}
void CollectionSnapshotDate(const SnapshotMetadataRecord &record, std::span<char> output) {
    std::array<char, 128> date{};
    FormatLoadingScreenDate(date, record.year, record.month, record.day, -1, 1);
    std::snprintf(output.data(), output.size(), "%s - %02d:%02d", date.data(), record.hour, record.minute);
}
void RefreshCollectionViewerText() {
    for (auto &line : collectionViewerText)
        line.fill(0);
    if (!collectionViewerCount)
        return;
    const auto &record = GetSnapshotMetadataOwner()->records[collectionViewerOrder[collectionViewerIndex]];
    CollectionSnapshotDate(record, collectionViewerText[0]);
    CollectionSnapshotBuilding(record, collectionViewerText[1]);
    const auto bio = CollectionBioIndex(record.lockOnId);
    if (bio >= 48)
        return;
    constexpr std::array<const char *, 5> suffixes{"NAME", "AGE", "BIRTH", "BLOOD", "DESC"};
    constexpr std::array<const char *, 4> prefixes{"UIBIONAME", "UIBIOAGE", "UIBIOBIRTH",
        "UIBIOBLOOD"}; // @text UIBIONAME EN: "Name:" | JA: "名前：" ; UIBIOAGE EN: "Age:" | JA: "年齢：" ; UIBIOBIRTH EN: "Birth:" | JA: "生年月日：" ; UIBIOBLOOD EN: "Blood:" | JA: "血液型："
    for (std::size_t i = 0; i < 5; ++i) {
        std::array<char, 32> token{};
        std::snprintf(token.data(), token.size(), "UIBIO%02d%s", bio, suffixes[i]);
        const auto value = GetLocalizedSceneText(token.data());
        const auto prefix = i < 4 ? GetLocalizedSceneText(prefixes[i]) : std::string_view{};
        std::snprintf(collectionViewerText[i + 2].data(), collectionViewerText[i + 2].size(), i < 4 ? "%.*s %.*s" : "%.*s",
            i < 4 ? static_cast<int>(prefix.size()) : static_cast<int>(value.size()), i < 4 ? prefix.data() : value.data(), static_cast<int>(value.size()),
            value.data());
    }
}
void SetCollectionViewerBack(bool back) {
    collectionViewerBack = back;
    const float alpha = back ? 1.0f : 0.0f;
    for (std::size_t window = 6; window < collectionViewerWindows.size(); ++window)
        if (collectionViewerWindows[window])
            collectionViewerWindows[window]->alpha = alpha;
}
void OpenCollectionSnapshotViewer(const std::uint32_t *order, std::uint32_t count, std::uint32_t selected, UIElement returning, bool wrap) {
    collectionViewerOrder = order;
    collectionViewerWrap = wrap;
    collectionViewerCount = count;
    collectionViewerIndex = selected;
    collectionViewerReturn = returning;
    RefreshCollectionViewerText();
    SetCollectionViewerBack(false);
    ShowElementByIndex(UIElement::SnapshotViewer);
}
void RefreshCollectionAlbums() {
    const auto *metadata = GetSnapshotMetadataOwner();
    for (std::size_t i = 0; i < collectionAlbums.size(); ++i) {
        auto &album = collectionAlbums[i];
        if (album.thumbnail)
            d3t::InitTexture2DResource(album.thumbnail);
        album.thumbnail = 0;
        album.image = {};
        album.slots.fill(-1);
        album.count = 0;
        std::array<char, 128> path{};

        std::snprintf(path.data(), path.size(), "/Snapshots/Thumbnail/CharThumb%02u.png?usage=0", static_cast<unsigned>(i + i / 8));
        album.request = SetupUsageForTexture(path.data(), 0);
        for (std::size_t slot = 0; slot < metadata->records.size(); ++slot)
            if (metadata->records[slot].use == 1 && metadata->records[slot].lockOnId.name == collectionBioNames[i].name)
                album.slots[static_cast<std::size_t>(album.count++)] = static_cast<std::int32_t>(slot);
    }
    for (std::int32_t group = 0; group < 6; ++group)
        for (std::int32_t column = 0; column < 9; ++column)
            collectionSecretRows[static_cast<std::size_t>(group * 9 + column)] = {group, column};
}
bool CollectionPrizeUnlocked(std::int32_t group) {
    for (std::int32_t i = 0; i < 8; ++i)
        if (!collectionAlbums[static_cast<std::size_t>(group * 8 + i)].count)
            return false;
    return true;
}

template <std::size_t Child> const std::int32_t *CollectionSelectionData(UIGrid *, std::int32_t row) {
    if constexpr (Child == 0)
        return reinterpret_cast<const std::int32_t *>(&collectionGacha[row]);
    if constexpr (Child == 1)
        return reinterpret_cast<const std::int32_t *>(&collectionMaps[row]);
    if constexpr (Child == 2)
        return reinterpret_cast<const std::int32_t *>(&collectionPawn[row]);
    if constexpr (Child == 3)
        return reinterpret_cast<const std::int32_t *>(&collectionScrolls[row]);
    if constexpr (Child == 4)
        return reinterpret_cast<const std::int32_t *>(&collectionEventPhotos[row]);
    if constexpr (Child == 5)
        return reinterpret_cast<const std::int32_t *>(&snapshotOrder[row]);
    if constexpr (Child == 6)
        return collectionSecretRows[row].data();
}
template <std::size_t Child> void SelectCollectionRow(float *bounds, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    UIMessageBox message{};
    message.imageIndex = message.widgetIndex = -1;
    if (bounds)
        std::memcpy(&message.bounds, bounds, sizeof(message.bounds));
    if (row) {
        if constexpr (Child == 0 || Child == 2 || Child == 4) {
            message.primaryText = row[2] ? GetShenmueItem(static_cast<std::uint32_t>(row[0]), static_cast<std::uint32_t>(row[1])) : "? ? ?";
            message.primaryLocalized = row[2] == 0;
            if constexpr (Child == 2)
                message.primaryText = reinterpret_cast<const CollectionItemRow *>(row)->label;
            if (row[2] > 1) {
                std::snprintf(collectionChild[Child].quantity.data(), collectionChild[Child].quantity.size(), "(%u)", static_cast<unsigned>(row[2]));
                message.tertiaryText = collectionChild[Child].quantity.data();
                message.tertiaryLocalized = 1;
            }
            d3t::UISharedSpriteImage image{};
            const auto icon =
                Child == 2 ? GetInventoryItemIcon(0, 27) : GetInventoryItemIcon(static_cast<std::uint32_t>(row[0]), static_cast<std::uint32_t>(row[1]));
            if (CollectionSpriteImage(icon, image))
                CollectionMessageImage(message, image);
        } else if constexpr (Child == 1) {
            const CollectionMapRow &map = *reinterpret_cast<const CollectionMapRow *>(row);
            message.primaryText = map.label;
            if (collectionMapPreview) {
                collectionMapPreview->frame = g_uiElements[static_cast<std::size_t>(collectionChildren[Child])].config.uiHandleIndex;
                if (map.heading || map.image < 0)
                    collectionMapPreview->image = collectionMapImageIndices[map.area + 26];
                else {
                    const std::int32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::CollectionMaps)].config.uiHandleIndex;
                    if (GetNuklearElementByIdx(slot)) {
                        collectionMapPreview->frame = slot;
                        collectionMapPreview->image = map.image;
                    }
                }
            }
        } else if constexpr (Child == 3)
            message.primaryText = reinterpret_cast<const WfMenuEntry *>(row)->label;
        else if constexpr (Child == 5) {
            const auto &record = GetSnapshotMetadataOwner()->records[static_cast<std::uint32_t>(*row)];
            CollectionSnapshotDate(record, collectionViewerText[0]);
            CollectionSnapshotBuilding(record, collectionViewerText[1]);
            std::snprintf(
                collectionViewerText[2].data(), collectionViewerText[2].size(), "%s %s", collectionViewerText[0].data(), collectionViewerText[1].data());
            message.primaryText = collectionViewerText[2].data();
            message.primaryLocalized = 1;
        } else {
            std::array<char, 32> token{};
            std::snprintf(token.data(), token.size(), "UIBIO%02dNAME", row[0] * 8 + row[1]);
            if (row[1] >= 8)
                message.primaryText = CollectionPrizeUnlocked(row[0]) ?
                    "UIBONUSUNL" :
                    "UIBONUSLOC"; // @text UIBONUSUNL EN: "Bonus Prize" | JA: "秘密の贈りもの" ; UIBONUSLOC EN: "Locked" | JA: "ロック中"
            else {
                const auto value = GetLocalizedSceneText(token.data());
                std::snprintf(collectionViewerText[2].data(), collectionViewerText[2].size(), "%.*s", static_cast<int>(value.size()), value.data());
                message.primaryText = collectionViewerText[2].data();
                message.primaryLocalized = 1;
            }
        }
    }
    CopyUIMessageBox(&message);
}
template <std::size_t Child> void ActivateCollectionRow(float *bounds, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (!row) {
        AicaExecuteWrapper(8);
        return;
    }
    if constexpr (Child == 0 || Child == 4)
        ActivateItemsRow(bounds, data);
    else if constexpr (Child == 2) {
        if (row[2]) {
            PreparePawnSetFlyer(reinterpret_cast<const CollectionItemRow *>(row)->flyer, 1);
            ShowElementByIndex(UIElement::PawnshopFlyer2);
            AicaExecuteWrapper(2);
        } else
            AicaExecuteWrapper(8);
    } else if constexpr (Child == 3) {
        const auto index = *row;
        const auto bit = 1U << index;
        if (!(GetWfOwnedScrolls() & bit)) {
            AicaExecuteWrapper(8);
            return;
        }
        if (index != 24 && !(GetWfLearnedScrolls() & bit))
            LearnWfScroll(index);
        else {
            AicaExecuteWrapper(2);
            InspectWfScroll(index);
            ShowSharedItemsWidget(1, static_cast<std::uint32_t>(index));
            auto &menu = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)];
            if (!menu.isRenderable) {
                menu.isRenderable = 1;
                if (menu.resetRender)
                    menu.resetRender();
            }
        }
    } else if constexpr (Child == 5) {
        if (snapshotImages[static_cast<std::uint32_t>(*row)].state.load(std::memory_order_acquire) != 2)
            return;
        if (collectionSnapshotDeleteClick == static_cast<std::uint32_t>(*row)) {
            RequestCollectionSnapshotDelete(static_cast<std::uint32_t>(*row), EnableSnapshotViewer);
            return;
        }
        const auto selected = GetUIGridSelection(collectionChild[Child].grid);
        OpenCollectionSnapshotViewer(snapshotOrder.data(), snapshotOrderCount, static_cast<std::uint32_t>(selected), static_cast<UIElement>(0), true);
    } else if constexpr (Child == 6) {
        if (row[1] >= 8) {
            if (CollectionPrizeUnlocked(row[0])) {
                OpenCollectionPresent(static_cast<std::uint32_t>(row[0]));
                AicaExecuteWrapper(0);
            } else
                AicaExecuteWrapper(4);
        } else {
            const auto &album = collectionAlbums[static_cast<std::size_t>(row[0] * 8 + row[1])];
            if (!album.count) {
                AicaExecuteWrapper(8);
                return;
            }
            OpenCollectionSnapshotViewer(
                reinterpret_cast<const std::uint32_t *>(album.slots.data()), static_cast<std::uint32_t>(album.count), 0, static_cast<UIElement>(0), false);
            AicaExecuteWrapper(0);
        }
    }
}
void DrawCollectionRowImage(const d3t::UISharedSpriteImage &image, const d3t::UIRect &bounds, float opacity, const d3t::UIRect &clip) {
    d3t::UITextureObjectDefinition object;
    object.kind = d3t::UITextureObjectKind::Image;
    d3t::DrawUITextureObject(uiDefinitions[static_cast<std::size_t>(UIElement::SharedItems)], object, {bounds, clip}, opacity, &image);
}
void DrawNativeCollectionPostcard(const UILayoutRect &rect, const struct nk_image *image, float border, float opacity) {
    nk_context &context = *GetNativeUiContext();
    const float inset = border < 0.0f ? 4.0f : border;
    UI_GatedLayoutSpacePush(context, nk_rect(rect.x + 3.0f, rect.y + 3.0f, rect.width, rect.height));
    UI_GatedButtonColour(context, nk_rgba(0, 0, 0, 150));
    UI_GatedLayoutSpacePush(context, nk_rect(rect.x, rect.y, rect.width, rect.height));
    nk_color background{};
    const UIGridStyle &style = IsCameraIconHidden() ? UI_GRID_GAME_STYLE : UI_GRID_FRONTEND_STYLE;
    std::memcpy(&background, &style.borderColour, sizeof(background));
    UI_GatedButtonColour(context, background);
    const float rounding = context.style.button.rounding;
    context.style.button.rounding = ScaleTo1080WithSafeArea(nk_rect(rect.x, rect.y, rect.width, rect.height), inset * 0.25f);
    UI_GatedLayoutSpacePush(context, nk_rect(rect.x, rect.y, rect.width, rect.height));
    UI_GatedButtonColour(context, nk_rgba(216, 216, 216, 255));
    context.style.button.rounding = rounding;
    if (image) {
        UI_GatedLayoutSpacePush(context, nk_rect(rect.x + inset, rect.y + inset, rect.width - inset * 2.0f, rect.height - inset * 2.0f));
        UI_GatedImageColour(context, *image, nk_rgba(255, 255, 255, static_cast<std::int32_t>(opacity * 255.0f)));
    }
}
void FillCollectionTextureImage(struct nk_image &image, std::uint32_t texture) {
    std::uint32_t width{}, height{};
    image = {};
    if (!d3t::GetTexture2DDimensions(texture, &width, &height))
        return;
    image.handle.id = static_cast<std::int32_t>(texture);
    image.w = static_cast<unsigned short>(width);
    image.h = static_cast<unsigned short>(height);
    image.region[2] = image.w;
    image.region[3] = image.h;
}
void DeleteCollectionSnapshotCell(float *, void *data) {
    const std::uint32_t *const slot = static_cast<const std::uint32_t *>(data);
    if (slot && *slot != std::numeric_limits<std::uint32_t>::max() && !IsSnapshotDeletePending())
        RequestCollectionSnapshotDelete(*slot, EnableSnapshotViewer);
}
template <std::size_t Child>
void DrawCollectionRow(
    UIGrid *grid, void *panel, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    if constexpr (Child <= 4) {
        CollectionChildRuntime &runtime = collectionChild[Child];
        const UILayoutRect holder{.x = outer->x, .y = outer->y, .width = outer->z, .height = outer->w};
        const UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
        const std::int32_t *const entry = dictionary ? CollectionSelectionData<Child>(grid, ordinal) : nullptr;
        const bool heading = Child == 1 && entry && reinterpret_cast<const CollectionMapRow *>(entry)->heading;
        if (!heading) {
            if (runtime.windows[3])
                runtime.windows[3]->alpha = 1.0f;
            PlaceUIGridCellWidgets(runtime.cellTemplate, ordinal, holder, cell);
            AfterDrawUIGridCell(runtime.cellTemplate, *grid, ordinal, 1, -1, holder, static_cast<nk_panel *>(panel));
        }
        if constexpr (Child == 0 || Child == 4) {
            DrawInventoryGridCell(grid, bounds, dictionary, ordinal);
        } else if (entry) {
            nk_context &context = *GetNativeUiContext();
            const float opacity = ordinal == GetUIGridSelection(*grid) ? 1.0f : 0.60000002f;
            UILayoutRect textRect = cell;
            textRect.x += 30.0f;
            textRect.width -= 30.0f;
            const char *text = nullptr;
            if constexpr (Child == 1)
                text = reinterpret_cast<const CollectionMapRow *>(entry)->label;
            else if constexpr (Child == 2)
                text = reinterpret_cast<const CollectionItemRow *>(entry)->label;
            else
                text = reinterpret_cast<const WfMenuEntry *>(entry)->label;
            if (!heading) {
                struct nk_image image{};
                if constexpr (Child == 1)
                    RenderingRelatedInventory(image, GetInventoryItemIcon(0, 0), 0);
                else if constexpr (Child == 2)
                    RenderingRelatedInventory(image, GetInventoryItemIcon(0, entry[3]), 0);
                else {
                    const WfMenuEntry &move = *reinterpret_cast<const WfMenuEntry *>(entry);
                    RenderingRelatedInventory(image, GetWfPromptIcon(move.prompt, ordinal), 0);
                }
                const float side = cell.height * 0.80000001f;
                if (g_waitingForNextTick == 1) {
                    if (context.current && context.current->layout)
                        context.current->layout->row.item = nk_rect(textRect.x + cell.height * 0.1f, textRect.y + cell.height * 0.1f, side, side);
                    nk_image_color(&context, image, nk_rgba(255, 255, 255, static_cast<std::int32_t>(opacity * 255.0f)));
                }
                textRect.x += side + 30.0f;
                textRect.width -= side + 30.0f;
                context.style.text.color = nk_rgba(255, 255, 255, static_cast<std::int32_t>(opacity * 255.0f));
            } else {
                if (g_uiFontFace != 1) {
                    g_uiFontFace = 1;
                    UI_SelectFont();
                }
                context.style.text.color = nk_rgba(0, 0, 0, static_cast<std::int32_t>(200.0f * opacity));
                if (g_waitingForNextTick == 1) {
                    if (context.current && context.current->layout)
                        context.current->layout->row.item = nk_rect(textRect.x, textRect.y, textRect.width, textRect.height);
                    nk_label(&context, GetLocalizedSceneText(text).data(), NK_TEXT_LEFT);
                }
                g_uiFontFace = 0;
                UI_SelectFont();
                context.style.text.color = nk_rgba(111, 144, 61, static_cast<std::int32_t>(255.0f * opacity));
            }
            if (g_waitingForNextTick == 1) {
                if (context.current && context.current->layout)
                    context.current->layout->row.item = nk_rect(textRect.x, textRect.y, textRect.width, textRect.height);
                nk_label(&context, Child == 2 ? text : GetLocalizedSceneText(text).data(), NK_TEXT_LEFT);
            }
        }
        return;
    } else {
        CollectionChildRuntime &runtime = collectionChild[Child];
        nk_context &context = *GetNativeUiContext();
        const UILayoutRect holder{.x = outer->x, .y = outer->y, .width = outer->z, .height = outer->w};
        UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
        const std::int32_t *const entry = dictionary ? CollectionSelectionData<Child>(grid, ordinal) : nullptr;
        const bool selected = ordinal == GetUIGridSelection(*grid), game = IsCameraIconHidden() != 0;
        if constexpr (Child == 5)
            PlaceUIGridCellWidgets(runtime.cellTemplate, ordinal, holder, cell);
        else if (ordinal % 9 < 8)
            PlaceUIGridCellWidgets(runtime.cellTemplate, ordinal, holder, cell);
        if (runtime.photoImages[4])
            runtime.photoImages[4]->alpha = game ? 1.0f : 0.0f;
        if (runtime.photoImages[5])
            runtime.photoImages[5]->alpha = !game && !selected ? 1.0f : 0.0f;
        if (runtime.photoImages[6])
            runtime.photoImages[6]->alpha = !game && selected ? 1.0f : 0.0f;
        if constexpr (Child == 5)
            AfterDrawUIGridCell(runtime.cellTemplate, *grid, ordinal, 1, -1, holder, static_cast<nk_panel *>(panel));
        else if (ordinal % 9 < 8)
            AfterDrawUIGridCell(runtime.cellTemplate, *grid, ordinal, 1, -1, holder, static_cast<nk_panel *>(panel));
        if (!entry)
            return;
        struct nk_image image{};
        bool postcard = true;
        float opacity = 1.0f;
        if constexpr (Child == 6) {
            if (entry[1] < 8) {
                CollectionAlbum &album = collectionAlbums[entry[0] * 8 + entry[1]];
                if (!album.thumbnail) {
                    album.thumbnail = GetShaderDataByIndex(album.request);
                    if (album.thumbnail)
                        FillCollectionTextureImage(album.image, album.thumbnail);
                }
                image = album.image;
                opacity = album.count ? 1.0f : 0.60000002f;
            } else {
                image = UI_ReadyViewImage(runtime.photoViewImages[CollectionPrizeUnlocked(entry[0]) ? entry[0] + 1 : 0]);
                postcard = false;
            }
        } else {
            const std::uint32_t slot = static_cast<std::uint32_t>(*entry);
            collectionSnapshotPriority[slot] = 1;
            const std::uint32_t state = snapshotImages[slot].state.load(std::memory_order_acquire);
            if (state == 2)
                FillCollectionTextureImage(image, snapshotImages[slot].texture.load(std::memory_order_acquire));
            else
                image = UI_ReadyViewImage(runtime.photoViewImages[state == 3 ? 1 : 0]);
            if (state != 2 && state != 3) {
                opacity =
                    collectionSnapshotLoadingTime > 0.75f ? std::sin(collectionSnapshotLoadingTime - 0.75f) * 0.25f + 0.75f : collectionSnapshotLoadingTime;
                postcard = false;
            }
        }
        UILayoutRect fitted = cell;
        const bool prize = Child == 6 && entry[1] >= 8;
        if (!prize && postcard)
            AdjustForSafeArea(cell, 22.0f, 22.0f);
        fitted = cell;
        UI_ScaleSize(fitted.width, fitted.height, 2, cell.width, cell.height, image.region[2], image.region[3], nullptr);
        UI_AlignInArea(1, 1, fitted.width, fitted.height, fitted.x, fitted.y, cell.width, cell.height);
        if (prize)
            AdjustForSafeArea(fitted, selected ? 5.5f : 22.0f, selected ? 5.5f : 22.0f);
        if (postcard)
            DrawNativeCollectionPostcard(fitted, &image, -1.0f, opacity);
        else if (g_waitingForNextTick == 1) {
            if (context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(fitted.x, fitted.y, fitted.width, fitted.height);
            nk_image_color(&context, image, nk_rgba(255, 255, 255, static_cast<std::int32_t>(opacity * 255.0f)));
        }
        if constexpr (Child == 5) {
            const std::uint32_t slot = static_cast<std::uint32_t>(*entry);
            if (selected && GetIsKeyboardActive() == 1 && snapshotImages[slot].state.load(std::memory_order_acquire) == 2) {
                image = UI_ReadyViewImage(runtime.photoViewImages[2]);
                const UILayoutRect button{.x = cell.x + cell.width - image.region[2] * 0.5f - 8.0f,
                    .y = cell.y + cell.height - image.region[3] * 0.5f - 8.0f,
                    .width = image.region[2] * 0.5f,
                    .height = image.region[3] * 0.5f};
                d3t::UIMouseCallbacks callbacks{.clicked = DeleteCollectionSnapshotCell};
                UI_WindowMouseCallbacks(callbacks, g_uiElements[static_cast<std::size_t>(collectionChildren[Child])].index,
                    {.x = holder.x + button.x, .y = holder.y + button.y, .width = button.width, .height = button.height}, const_cast<std::int32_t *>(entry));
                if (g_waitingForNextTick == 1) {
                    if (context.current && context.current->layout)
                        context.current->layout->row.item = nk_rect(button.x, button.y, button.width, button.height);
                    nk_image_color(&context, image, nk_rgba(255, 255, 255, static_cast<std::int32_t>(collectionSnapshotDeleteAlpha * 255.0f)));
                }
                const float delta = (context.last_widget_state & NK_WIDGET_STATE_HOVERED) ? GetFrameTime() : -GetFrameTime();
                collectionSnapshotDeleteAlpha = std::fmax(0.2f, std::fmin(1.0f, collectionSnapshotDeleteAlpha + delta));
            }
        }
    }
}
template <std::size_t Child> std::uint32_t LoadCollectionChild() {
    return ReadUIDefinitionFile(collectionChildren[Child], collectionFiles[Child], Child >= 5 ? 1 : 0);
}
template <std::size_t Child> std::int64_t BindCollectionChild() {
    auto &runtime = collectionChild[Child];
    constexpr std::array<std::int32_t, 7> columns{8, 1, 1, 1, 4, 3, 3}, visible{4, 8, 8, 8, 3, 3, 3}, rows{64, 28, 12, 25, 16, 42, 18};
    InitializeUIGrid(runtime.grid,
        {.columns = columns[Child],
            .visibleRows = visible[Child],
            .rows = rows[Child],
            .drawEmpty = Child == 0 ? std::uint8_t{0} : std::uint8_t{1},
            .wrap = Child == 6 ? std::uint8_t{1} : std::uint8_t{0},
            .pageScroll = Child == 6 ? std::uint8_t{1} : std::uint8_t{0},
            .drawRow = DrawCollectionRow<Child>,
            .actions = {SelectCollectionRow<Child>, nullptr, nullptr, ActivateCollectionRow<Child>},
            .selectionData = CollectionSelectionData<Child>,
            .sound = 3});
    {
        const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(collectionChildren[Child])].config.uiHandleIndex;
        for (std::size_t index = 0; index < runtime.windows.size(); ++index)
            runtime.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, COLLECTION_GRID_WINDOWS[Child][index]));
        d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
        for (std::int32_t item = 0; item < (Child >= 5 ? 4 : 2); ++item) {
            runtime.layoutItems[item] = nullptr;
            if (!view)
                continue;
            for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
                if (view->widgetKinds[widget] != 13)
                    continue;
                d3t::UILayoutItem &record = g_uiLayoutItems[view->widgetPoolIndices[widget]];
                if (!std::strcmp(record.id.data(), COLLECTION_GRID_LAYOUT_ITEMS[Child][item])) {
                    runtime.layoutItems[item] = &record;
                    break;
                }
            }
        }
        runtime.grid.panel = runtime.windows[0];
        runtime.cellTemplate = {.viewSlot = static_cast<std::int32_t>(slot),
            .first = 1,
            .end = 5,
            .clipWindows = {3, -1},
            .windows = runtime.windows.data(),
            .ids = COLLECTION_GRID_WINDOWS[Child].data(),
            .layoutFirst = 0,
            .layoutEnd = Child >= 5 ? 4 : 2,
            .layoutItems = runtime.layoutItems.data()};
        if constexpr (Child == 1) {
            const std::int32_t image = UI_FindViewImageWidget(slot, "UI_SubMenu::Collection_Maps::Image");
            collectionMapPreview = image >= 0 ? &g_uiImageWidgets[image] : nullptr;
            for (std::size_t index = 0; index < collectionMapImages.size(); ++index)
                collectionMapImageIndices[index] = UI_FindViewImage(slot, collectionMapImages[index]);
        }
        if constexpr (Child >= 5) {
            const char *const prefix = Child == 5 ? "SubMenu_Photos_SnapShot" : "SubMenu_Photos_Secret";
            std::array<char, 96> id{};
            for (std::size_t image = 0; image < runtime.photoImages.size(); ++image) {
                std::snprintf(id.data(), id.size(), "%s::%s", prefix, PHOTOS_GRID_IMAGE_SUFFIXES[image]);
                const std::int32_t index = UI_FindViewColourImage(slot, id.data());
                runtime.photoImages[image] = index >= 0 ? &g_uiColourImageWidgets[index] : nullptr;
                if constexpr (Child == 5) {
                    constexpr std::array<const char *, 3> names{"Icon_Loading", "Icon_LoadError", "Icon_Delete"};
                    if (image < 3)
                        runtime.photoViewImages[image] = UI_FindViewImage(slot, names[image]);
                } else {
                    std::snprintf(id.data(), id.size(), image ? "Image_PrizeUnlocked%u" : "Image_PrizeLocked", static_cast<unsigned>(image - 1));
                    runtime.photoViewImages[image] = UI_FindViewImage(slot, id.data());
                }
            }
            std::snprintf(id.data(), id.size(), "%s::GridElement_Background", prefix);
            runtime.photoButton = nullptr;
            if (view)
                for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
                    if (view->widgetKinds[widget] != 17)
                        continue;
                    UIButtonWidget &button = g_uiButtonWidgets[view->widgetPoolIndices[widget]];
                    if (!std::strcmp(button.id.data(), id.data())) {
                        runtime.photoButton = &button;
                        break;
                    }
                }
        }
    }
    runtime.bound = true;
    if (g_uiElements[static_cast<std::size_t>(collectionChildren[Child])].isVisible)
        TickCollectionChild<Child>();
    return 1;
}
template <std::size_t Child> std::int64_t TickCollectionChild() {
    auto &runtime = collectionChild[Child];
    if (!runtime.bound)
        return 0;
    std::int32_t count{};
    const std::int32_t *dictionary{};
    if constexpr (Child == 0 || Child == 4) {
        constexpr std::uint32_t bank = Child == 0 ? 1 : 2;
        auto &rows = []() -> auto & {
            if constexpr (Child == 0)
                return collectionGacha;
            else
                return collectionEventPhotos;
        }();
        const auto stock = S2Save_GetInventoryBank(static_cast<S2InventoryBank>(bank));
        const auto maximum = Child == 0 ? GetMaxItemsForItemType(bank) : 64U;
        for (std::uint32_t i = 0; i < maximum; ++i) {
            std::int32_t quantity = std::to_integer<std::uint8_t>(stock[i]);
            if constexpr (Child == 4)
                if (!quantity && !g_inventoryStockBiasEnabled)
                    continue;
            if (g_inventoryStockBiasEnabled) {
                if constexpr (Child == 4)
                    ++quantity;
                else if (!quantity)
                    quantity = 1;
            }
            CollectionItemRow &row = rows[static_cast<std::size_t>(count++)];
            row.type = static_cast<std::int32_t>(bank);
            row.index = static_cast<std::int32_t>(i);
            row.quantity = quantity;
        }
        dictionary = reinterpret_cast<const std::int32_t *>(rows.data());
    } else if constexpr (Child == 1) {
        for (std::int32_t area = 0; area < 3; ++area) {
            const auto configs = GetFullMapConfigs();
            collectionMaps[static_cast<std::size_t>(count++)] = {area, -1, configs[area].areaLabel, 1};
            const auto floors = GetFullMapFloors(area);
            const auto mask = GetFullMapFloorMask(area, 2);
            for (std::size_t i = 0; i < floors.size(); ++i) {
                if (!floors[i].label)
                    continue;
                const auto ordinal = count - area - 1;
                const bool owned = (mask & (1U << i)) != 0;
                collectionMaps[static_cast<std::size_t>(count++)] = {
                    area, owned ? ordinal : -1, owned ? floors[i].label : "UIGNRLQSTN", 0}; // @text EN: "? ? ?" | JA: "？？？"
            }
        }
        dictionary = reinterpret_cast<const std::int32_t *>(collectionMaps.data());
    } else if constexpr (Child == 2) {
        for (std::size_t i = 0; i < collectionPawn.size(); ++i) {
            std::array<char, 16> token{};
            std::snprintf(token.data(), token.size(), "UIPWNSHP%02u", static_cast<unsigned>(i));
            const bool owned = GetItemState(static_cast<std::uint32_t>(PAWN_WINDOW_SHOPS[i].item)) != 0;
            collectionPawn[i] = {5, owned ? static_cast<std::int32_t>(i) : -1, static_cast<std::int32_t>(owned), 27,
                GetLocalizedSceneText(owned ? token.data() : "UIGNRLQSTN").data(), static_cast<std::int32_t>(i)}; // @text EN: "? ? ?" | JA: "？？？"
        }
        count = 12;
        dictionary = reinterpret_cast<const std::int32_t *>(collectionPawn.data());
    } else if constexpr (Child == 3) {
        BuildWfMenuEntries(collectionScrolls);
        count = 25;
        dictionary = reinterpret_cast<const std::int32_t *>(collectionScrolls.data());
    } else if constexpr (Child == 5) {
        RefreshSnapshotOrder();
        count = static_cast<std::int32_t>(snapshotOrderCount);
        dictionary = reinterpret_cast<const std::int32_t *>(snapshotOrder.data());
    } else {
        RefreshCollectionAlbums();
        count = 54;
        dictionary = collectionSecretRows[0].data();
    }
    SetUIGridDictionary(runtime.grid, count, dictionary);
    if constexpr (Child == 3)
        OpenWfScrollWidget();
    ResetUIGrid(runtime.grid);
    Vector4 outer{}, bounds{};
    GetUIGridRowBounds(runtime.grid, &outer, &bounds);
    SelectCollectionRow<Child>(&bounds.x, count ? const_cast<std::int32_t *>(CollectionSelectionData<Child>(&runtime.grid, 0)) : nullptr);
    return 0;
}
template <std::size_t Child> std::int64_t InputCollectionChild() {
    auto &runtime = collectionChild[Child];
    if (nativePrompt.active)
        return 0;
    if constexpr (Child == 5) {
        const auto pressed = GetPadControlData(1);
        if (pressed & 4U) {
            const auto option = (GetSnapshotDataOption() + 1) % 3;
            SetSnapshotDataOption(option);
            constexpr std::array<const char *, 3> sortLabels{"UISORTNEW", "UISORTOLD",
                "UISORTPLC"}; // @text UISORTNEW EN: "Sorted by new" | JA: "並べ替え（新しい順）" ; UISORTOLD EN: "Sorted by old" | JA: "並べ替え（古い順）" ; UISORTPLC EN: "Sorted by place" | JA: "並べ替え（場所）"
            auto message = informationBar.current;
            message.primaryText = sortLabels[static_cast<std::size_t>(option)];
            message.primaryLocalized = false;
            ShowTemporaryUIMessageBox(&message, 4.0f);
            RefreshSnapshotOrder();
            SetUIGridDictionary(runtime.grid, static_cast<std::int32_t>(snapshotOrderCount), reinterpret_cast<const std::int32_t *>(snapshotOrder.data()));
        }
        const std::int32_t selected = GetUIGridSelection(runtime.grid);
        if ((pressed & 2U) && !IsSnapshotDeletePending() && selected >= 0 && static_cast<std::uint32_t>(selected) < snapshotOrderCount)
            RequestCollectionSnapshotDelete(snapshotOrder[static_cast<std::size_t>(selected)], EnableSnapshotViewer);
    }
    ProcessUIGridInput(runtime.grid, uiPointer, GetFrameTime());
    return 1;
}
template <std::size_t Child> std::int64_t RenderCollectionChild() {
    CollectionChildRuntime &runtime = collectionChild[Child];
    if (!runtime.bound)
        return 0;
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(collectionChildren[Child])];
    if constexpr (Child == 1)
        uiUpdateWithColour(*GetNativeUiContext(), element.config.uiHandleIndex);
    else
        DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    if constexpr (Child == 5)
        collectionSnapshotLoadingTime += GetFrameTime();
    RenderNativeUIGrid(runtime.grid, element.index, uiPointer);
    return 0;
}
template <std::size_t Child> std::int64_t SetPhotoGridContext() {
    CollectionChildRuntime &runtime = collectionChild[Child];
    const bool game = IsCameraIconHidden() != 0;
    if (runtime.photoButton)
        runtime.photoButton->colour = PHOTOS_GRID_BUTTON_COLOURS[game];
    for (std::size_t index = 0; index < runtime.photoImages.size(); ++index) {
        if (!runtime.photoImages[index])
            continue;
        const bool gameImage = index == 0 || index == 1 || index == 4;
        runtime.photoImages[index]->alpha = gameImage == game ? 1.0f : 0.0f;
    }
    return 0;
}
template <std::size_t Child> std::int64_t ResetCollectionChild() {
    collectionChild[Child].bound = false;
    return 0;
}
void CollectionTabClicked(float *, void *data) {
    ClickUIGridTab(collectionTabs.nativeTabs, data);
}
void CollectionTabEntered(float *, void *data) {
    EnterUIGridTab(collectionTabs.nativeTabs, data);
}
void CollectionTabLeft(float *, void *data) {
    LeaveUIGridTab(collectionTabs.nativeTabs, data);
}
void PhotosTabClicked(float *, void *data) {
    ClickUIGridTab(photosTabs.nativeTabs, data);
}
void PhotosTabEntered(float *, void *data) {
    EnterUIGridTab(photosTabs.nativeTabs, data);
}
void PhotosTabLeft(float *, void *data) {
    LeaveUIGridTab(photosTabs.nativeTabs, data);
}
template <bool Photos> constexpr UIElement CollectionRoot() {
    return Photos ? UIElement::SubMenuPhotos : UIElement::SubMenuCollection;
}
template <bool Photos> constexpr UIElement CollectionTabChild(std::int32_t tab) {
    if constexpr (Photos) {
        constexpr std::array<UIElement, 3> children{UIElement::PhotosSnapshot, static_cast<UIElement>(23), static_cast<UIElement>(21)};
        return children[tab];
    } else
        return collectionChildren[static_cast<std::size_t>(tab)];
}
template <bool Photos> void SwitchCollectionTab() {
    CollectionTabsRuntime &runtime = Photos ? photosTabs : collectionTabs;
    const std::int32_t selected = runtime.nativeTabs.selected;
    if (selected == runtime.shown)
        return;
    if (runtime.shown >= 0)
        DestroyElementByIndex(CollectionTabChild<Photos>(runtime.shown));
    runtime.shown = selected;
    ShowElementByIndex(CollectionTabChild<Photos>(selected));
    const bool game = !Photos || IsCameraIconHidden() != 0;
    const std::array<std::uint32_t, 2> &buttons = game ? COLLECTION_TAB_BUTTON_COLOURS : PHOTOS_FRONT_TAB_BUTTON_COLOURS;
    const std::array<std::uint32_t, 2> &textures = game ? COLLECTION_TAB_TEXTURE_COLOURS : PHOTOS_FRONT_TAB_TEXTURE_COLOURS;
    const std::int32_t textureBase = Photos ? (game ? 6 : 3) : 4;
    for (std::int32_t tab = 0; tab < (Photos ? 3 : 4); ++tab) {
        if (runtime.nativeImages[tab])
            runtime.nativeImages[tab]->colour = (runtime.nativeImages[tab]->colour & 0xffffffU) | ((tab == selected ? 255U : 153U) << 24);
        if (runtime.nativeButtons[tab])
            runtime.nativeButtons[tab]->colour = buttons[tab == selected];
        if (runtime.nativeImages[textureBase + tab])
            runtime.nativeImages[textureBase + tab]->colour = textures[tab == selected];
    }
    if constexpr (Photos)
        if (!game && runtime.nativeButtons[2])
            runtime.nativeButtons[2]->colour &= 0xffffffU;
}
template <bool Photos> std::uint32_t LoadCollectionRoot() {
    return ReadUIDefinitionFile(
        CollectionRoot<Photos>(), Photos ? "/ui/gamemenu_New/SubMenu_Photos.ui" : "/ui/gamemenu_New/SubMenu_Collection.ui", Photos ? 1 : 0);
}
template <bool Photos> std::int64_t BindCollectionRoot() {
    CollectionTabsRuntime &runtime = Photos ? photosTabs : collectionTabs;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(CollectionRoot<Photos>())].config.uiHandleIndex;
    const char *const prefix = Photos ? "SubMenu_Photos" : "SubMenu_Collection";
    std::array<char, 80> id{};
    for (std::int32_t index = 0; index < (Photos ? 10 : 4); ++index) {
        std::snprintf(id.data(), id.size(), "%s::Tab%dButton", prefix, index + 1);
        runtime.nativeWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, Photos ? PHOTOS_TAB_WINDOWS[index] : id.data()));
    }
    for (std::int32_t tab = 0; tab < (Photos ? 3 : 4); ++tab) {
        d3t::UIWindow *const window = static_cast<d3t::UIWindow *>(runtime.nativeWindows[tab]);
        if (window) {
            window->mouseCallbacks.entered = Photos ? PhotosTabEntered : CollectionTabEntered;
            window->mouseCallbacks.left = Photos ? PhotosTabLeft : CollectionTabLeft;
            window->mouseCallbacks.clicked = Photos ? PhotosTabClicked : CollectionTabClicked;
        }
        std::snprintf(id.data(), id.size(), "%s::Label_Tab_%d", prefix, tab + 1);
        const std::int32_t label = UI_FindViewLabel(slot, id.data());
        runtime.nativeLabels[tab] = label >= 0 ? &g_uiLabelWidgets[label] : nullptr;
    }
    for (std::int32_t image = 0; image < (Photos ? 11 : 8); ++image) {
        std::snprintf(id.data(), id.size(), image < 4 ? "%s::Image_Tab_%d" : "%s::Texture_Alpha_%d", prefix, image % 4 + 1);
        const std::int32_t index = UI_FindViewColourImage(slot, Photos ? PHOTOS_TAB_IMAGES[image] : id.data());
        runtime.nativeImages[image] = index >= 0 ? &g_uiColourImageWidgets[index] : nullptr;
    }
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    for (std::int32_t tab = 0; tab < 4; ++tab) {
        std::snprintf(id.data(), id.size(), "%s::Button_Tab_%d", prefix, tab + 1);
        if constexpr (Photos)
            if (tab == 3)
                std::snprintf(id.data(), id.size(), "SubMenu_Photos::MainPanelButton");
        runtime.nativeButtons[tab] = nullptr;
        if (view)
            for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
                if (view->widgetKinds[widget] != 17)
                    continue;
                UIButtonWidget &button = g_uiButtonWidgets[view->widgetPoolIndices[widget]];
                if (!std::strcmp(button.id.data(), id.data())) {
                    runtime.nativeButtons[tab] = &button;
                    break;
                }
            }
    }
    runtime.nativeTabs.first = 0;
    runtime.nativeTabs.last = Photos ? 2 : 3;
    runtime.nativeTabs.buttons = runtime.nativeWindows.data();
    runtime.nativeTabs.labels = runtime.nativeLabels.data();
    runtime.bound = true;
    return 1;
}
std::int64_t SetPhotosMenuContext() {
    const bool game = IsCameraIconHidden() != 0;
    d3t::UIWindow *const root = static_cast<d3t::UIWindow *>(photosTabs.nativeWindows[9]);
    if (root)
        std::snprintf(root->layout.parentId.data(), root->layout.parentId.size(), "%s", game ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    if (photosTabs.nativeButtons[3])
        photosTabs.nativeButtons[3]->colour = game ? COLLECTION_TAB_BUTTON_COLOURS[1] : PHOTOS_FRONT_MAIN_BUTTON_COLOUR;
    for (std::int32_t tab = 0; tab < 3; ++tab) {
        UIColourImageWidget *const hidden = photosTabs.nativeImages[(game ? 3 : 6) + tab];
        if (hidden)
            hidden->colour &= 0xffffffU;
        d3t::UIWindow *const window = static_cast<d3t::UIWindow *>(photosTabs.nativeWindows[2 + 3 * tab]);
        if (window)
            window->alpha = game ? 1.0f : 0.0f;
    }
    if (photosTabs.nativeImages[game ? 10 : 9])
        photosTabs.nativeImages[game ? 10 : 9]->colour &= 0xffffffU;
    if (photosTabs.nativeImages[game ? 9 : 10])
        photosTabs.nativeImages[game ? 9 : 10]->colour = game ? COLLECTION_TAB_TEXTURE_COLOURS[1] : PHOTOS_FRONT_BACKGROUND_COLOUR;
    if (photosTabs.nativeButtons[2])
        photosTabs.nativeButtons[2]->colour = (photosTabs.nativeButtons[2]->colour & 0xffffffU) | (game ? 0x7f000000U : 0U);
    photosTabs.nativeTabs.last = game ? 2 : 1;
    if (!game && photosTabs.nativeTabs.selected > 1)
        photosTabs.nativeTabs.selected = 1;
    return 0;
}
template <bool Photos> std::int64_t TickCollectionRoot() {
    ResetUIGridTabSelector();
    CollectionTabsRuntime &runtime = Photos ? photosTabs : collectionTabs;
    runtime.shown = -1;
    runtime.nativeTabs.selected = runtime.nativeTabs.hovered = 0;
    runtime.nativeTabs.active = runtime.nativeTabs.previousActive = 0;
    SetUIGridTabSelector(&runtime.nativeTabs, 1);
    if constexpr (Photos) {
        PrepareSnapshotImages();
        ShowElementByIndex(UIElement::GameInfoBar);
    }
    SwitchCollectionTab<Photos>();
    return 0;
}
template <bool Photos> std::int64_t RenderCollectionRoot() {
    const CollectionTabsRuntime &runtime = Photos ? photosTabs : collectionTabs;
    if (!runtime.bound)
        return 0;
    if constexpr (Photos)
        PumpCollectionSnapshotImages();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(CollectionRoot<Photos>())];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    return 0;
}
template <bool Photos> std::int64_t InputCollectionRoot() {
    CollectionTabsRuntime &runtime = Photos ? photosTabs : collectionTabs;
    ProcessUIGridTabInput(runtime.nativeTabs);
    SwitchCollectionTab<Photos>();
    return 1;
}
template <bool Photos> std::int64_t DestroyCollectionRoot() {
    auto &runtime = Photos ? photosTabs : collectionTabs;
    if constexpr (Photos)
        ReleaseSnapshotImages();
    if (runtime.shown >= 0)
        DestroyElementByIndex(CollectionTabChild<Photos>(runtime.shown));
    return 1;
}
template <bool Photos> std::int64_t ResetCollectionRoot() {
    (Photos ? photosTabs : collectionTabs).bound = false;
    return 0;
}
template <bool Photos> std::int64_t ResetCollectionRootRender() {
    auto &runtime = Photos ? photosTabs : collectionTabs;
    if (runtime.shown >= 0)
        g_uiElements[static_cast<std::size_t>(CollectionTabChild<Photos>(runtime.shown))].isRenderable =
            g_uiElements[static_cast<std::size_t>(CollectionRoot<Photos>())].isRenderable;
    return 0;
}
void CopyCollectionMapImages(const UIElementState &source) {
    const UIElementState &target = g_uiElements[static_cast<std::size_t>(UIElement::CollectionMaps)];
    if (static_cast<std::uint32_t>(source.config.uiHandleIndex) >= d3t::kUIViewSlotCount ||
        static_cast<std::uint32_t>(target.config.uiHandleIndex) >= d3t::kUIViewSlotCount)
        return;
    d3t::UIViewSlot<80> &targetView = g_uiViewSlots[static_cast<std::size_t>(target.config.uiHandleIndex)];
    const d3t::UIViewSlot<80> &sourceView = g_uiViewSlots[static_cast<std::size_t>(source.config.uiHandleIndex)];
    std::memcpy(targetView.images.data(), sourceView.images.data(), 25 * sizeof(d3t::UIViewImage));
    const d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(source.index)];
    for (std::size_t image = 0; image < collectionLocalizedMaps.size(); ++image)
        collectionLocalizedMaps[image] = &definition.images[image];
}
std::int64_t PumpCollectionMapLanguage() {
    if (collectionMapLanguagePending) {
        const std::int32_t language = pendingCollectionMapLanguage;
        const UIElementState &source = *collectionMapLanguageElements[static_cast<std::size_t>(language)];
        if (source.config.init == 3) {
            CopyCollectionMapImages(source);
            collectionMapLanguage = language;
            collectionMapLanguagePending = false;
        } else
            collectionMapLanguagePending = true;
    }
    return 1;
}
std::uint32_t LoadCollectionMapLocalization() {
    for (std::size_t language = 0; language < collectionMapLanguageElements.size(); ++language)
        ReadUIDefinitionFile(static_cast<UIElement>(collectionMapLanguageElements[language]->index), COLLECTION_MAP_LANGUAGE_FILES[language], 0);
    return ReadUIDefinitionFile(UIElement::CollectionMaps, "/ui/gamemenu_New/maps/Collection_Maps_Localisation.ui", 0);
}
std::int64_t BindCollectionMapLocalization() {
    const std::int32_t language = GetLoadingScreenTextType(ResetSceneTextLanguage());
    const UIElementState &source = *collectionMapLanguageElements[static_cast<std::size_t>(language)];
    if (source.config.init == 3) {
        CopyCollectionMapImages(source);
        collectionMapLanguage = language;
        collectionMapLanguagePending = false;
    } else {
        collectionMapLanguagePending = true;
        pendingCollectionMapLanguage = language;
    }
    return 1;
}
const d3t::UIImageDefinition *CollectionLocalizedMapImage(std::int32_t index) {
    if (index < 0 || index >= 29)
        return nullptr;
    if (index < 25)
        return collectionLocalizedMaps[static_cast<std::size_t>(index)];
    const d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::CollectionMaps)];
    for (const d3t::UIImageDefinition &image : definition.images)
        if (image.name == collectionMapImages[index])
            return &image;
    return nullptr;
}
void DrawCollectionColour(const d3t::UIRect &rectangle, std::uint32_t colour, const d3t::UIRect &clip) {
    d3t::UITextureObjectDefinition object;
    object.colour = colour;
    const d3t::UISharedSpriteImage white{.textureHandle = d3t::g_defaultWhiteTexture, .source = {0, 0, 1, 1}};
    d3t::DrawUITextureObject(uiDefinitions[24], object, {rectangle, clip}, 1.0f, &white);
}
void SetCollectionUnderlayAlpha(bool restore) {
    constexpr std::array<std::string_view, 3> names{"MainMenu::ContentBackground", "Texture_Alpha_Front", "Texture_Alpha_Front2"};
    auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::MainMenu)];
    if (!restore && !collectionUnderlayHidden) {
        for (std::size_t slot = 0; slot < names.size(); ++slot) {
            auto &saved = collectionUnderlayAlpha[slot];
            saved.object = -1;
            for (std::size_t i = 0; i < definition.textureObjects.size(); ++i)
                if (definition.textureObjects[i].id == names[slot]) {
                    saved.object = static_cast<std::int32_t>(i);
                    saved.alpha = static_cast<std::uint8_t>(definition.textureObjects[i].colour >> 24);
                    break;
                }
        }
        collectionUnderlayHidden = true;
    }
    if (!collectionUnderlayHidden)
        return;
    for (const auto &saved : collectionUnderlayAlpha)
        if (saved.object >= 0) {
            auto &object = definition.textureObjects[static_cast<std::size_t>(saved.object)];
            object.colour = (object.colour & 0xffffffU) | (static_cast<std::uint32_t>(restore ? saved.alpha : 0) << 24);
        }
    if (restore)
        collectionUnderlayHidden = false;
}
std::int64_t TickCollectionViewer() {
    AicaExecuteWrapper(2);
    EnableRenderableByIndex(UIElement::SubMenuPhotos);
    EnableRenderableByIndex(UIElement::PhotosSecrets);
    EnableRenderableByIndex(UIElement::GameMenu);
    EnableRenderableByIndex(UIElement::GameInfoBar);
    SetCollectionUnderlayAlpha(false);
    return 0;
}
void RetireCollectionViewerUnderlay(UIElement returning) {
    AicaExecuteWrapper(1);
    DisableElement(UIElement::SubMenuPhotos);
    DisableElement(UIElement::PhotosSecrets);
    DisableElement(UIElement::GameMenu);
    DisableElement(UIElement::GameInfoBar);
    SetCollectionUnderlayAlpha(true);
    ShowElementByIndex(returning);
}
void FitCollectionViewerImage(UILayoutRect &fitted, const UILayoutRect &area, const struct nk_image &image) {
    fitted = {.x = 0.0f, .y = 0.0f, .width = area.width, .height = area.height};
    UI_ScaleSize(fitted.width, fitted.height, 2, area.width, area.height, image.region[2], image.region[3], nullptr);
    UI_AlignInArea(1, 1, fitted.width, fitted.height, fitted.x, fitted.y, area.width, area.height);
}
void ShowNextCollectionViewerImage(float *, void *) {
    const std::uint32_t previous = collectionViewerIndex;
    if (++collectionViewerIndex >= collectionViewerCount)
        collectionViewerIndex = collectionViewerWrap ? 0 : collectionViewerCount - 1;
    if (previous != collectionViewerIndex) {
        RefreshCollectionViewerText();
        AicaExecuteWrapper(3);
    }
}
void ShowPreviousCollectionViewerImage(float *, void *) {
    const std::uint32_t previous = collectionViewerIndex;
    if (--collectionViewerIndex >= collectionViewerCount)
        collectionViewerIndex = collectionViewerWrap ? collectionViewerCount - 1 : 0;
    if (previous != collectionViewerIndex) {
        RefreshCollectionViewerText();
        AicaExecuteWrapper(3);
    }
}
void FlipCollectionViewerImage(float *, void *) {
    SetCollectionViewerBack(!collectionViewerBack);
    AicaExecuteWrapper(3);
}
void UpdateCollectionViewerArrows() {
    const bool left = collectionViewerCount > 1 && (collectionViewerWrap || collectionViewerIndex);
    const bool right = collectionViewerCount > 1 && (collectionViewerWrap || collectionViewerIndex < collectionViewerCount - 1);
    if (collectionViewerWindows[4])
        collectionViewerWindows[4]->alpha = left ? 1.0f : 0.0f;
    if (collectionViewerWindows[5])
        collectionViewerWindows[5]->alpha = right ? 1.0f : 0.0f;
    const bool pointerLeft = GetIsKeyboardActive() == 0 && left, pointerRight = GetIsKeyboardActive() == 0 && right;
    if (collectionViewerWindows[2])
        collectionViewerWindows[2]->alpha = pointerLeft ? 1.0f : 0.0f;
    if (collectionViewerWindows[3])
        collectionViewerWindows[3]->alpha = pointerRight ? 1.0f : 0.0f;
}
std::uint32_t LoadCollectionViewer() {
    return ReadUIDefinitionFile(UIElement::SnapshotViewer, "/ui/gamemenu_New/SubMenu_Photos_Snapshot_Viewer.ui", 1);
}
std::int64_t BindCollectionViewer() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::SnapshotViewer)].config.uiHandleIndex;
    for (std::size_t window = 0; window < collectionViewerWindows.size(); ++window)
        collectionViewerWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, COLLECTION_VIEWER_WINDOWS[window]));
    if (collectionViewerWindows[0])
        collectionViewerWindows[0]->mouseCallbacks.clicked = FlipCollectionViewerImage;
    if (collectionViewerWindows[2])
        collectionViewerWindows[2]->mouseCallbacks.clicked = ShowPreviousCollectionViewerImage;
    if (collectionViewerWindows[3])
        collectionViewerWindows[3]->mouseCallbacks.clicked = ShowNextCollectionViewerImage;
    if (collectionViewerWindows[4])
        collectionViewerWindows[4]->mouseCallbacks.clicked = ShowPreviousCollectionViewerImage;
    if (collectionViewerWindows[5])
        collectionViewerWindows[5]->mouseCallbacks.clicked = ShowNextCollectionViewerImage;
    for (std::size_t item = 0; item < collectionViewerLayoutItems.size(); ++item)
        collectionViewerLayoutItems[item] = UI_FindViewLayoutItem(slot, COLLECTION_VIEWER_LAYOUT_ITEMS[item]);
    for (std::size_t label = 0; label < collectionViewerLabels.size(); ++label) {
        d3t::UILabelWidget *const widget = UI_GetLabel(static_cast<std::uint32_t>(UI_FindViewLabel(slot, COLLECTION_VIEWER_LABELS[label])));
        collectionViewerLabels[label] = widget;
        if (widget) {
            widget->text = collectionViewerText[label].data();
            widget->token.alreadyLocalized = 1;
        }
    }
    collectionViewerPlaceholder = UI_FindViewImage(slot, "Image_Placeholder");
    return 1;
}
std::int64_t ResetCollectionViewer() {
    collectionViewerWindows.fill(nullptr);
    collectionViewerLabels.fill(nullptr);
    collectionViewerLayoutItems.fill(nullptr);
    collectionViewerPlaceholder = -1;
    return 0;
}
std::int64_t InputCollectionViewer() {
    if (nativePrompt.active)
        return 0;
    const auto pressed = GetPadControlData(1);
    if (pressed & 0x400U) {
        SetCollectionViewerBack(!collectionViewerBack);
        AicaExecuteWrapper(3);
    }
    if ((pressed & 2U) && !IsSnapshotDeletePending())
        RequestCollectionSnapshotDelete(collectionViewerOrder[collectionViewerIndex], EnableSnapshotViewer);
    if (pressed & 0x200U) {
        DestroyElementByIndex(UIElement::SnapshotViewer);
        return 0;
    }
    if (pressed & 0x8088U)
        ShowNextCollectionViewerImage(nullptr, nullptr);
    if (pressed & 0x4050U)
        ShowPreviousCollectionViewerImage(nullptr, nullptr);
    return 0;
}
std::int64_t RenderCollectionViewer() {
    const float aspect = static_cast<float>(static_cast<std::int32_t>(GetUIViewportWidth()));
    for (std::size_t item = 0; item < collectionViewerLayoutItems.size(); ++item) {
        d3t::UIWindow *const window = collectionViewerWindows[item + 7];
        d3t::UILayoutItem *const layout = collectionViewerLayoutItems[item];
        if (!window || !layout)
            continue;
        UILayoutRect rect{};
        UI_GetWindowRect(window, rect);
        layout->offsetX = rect.x / aspect;
        layout->width = rect.width / aspect;
        layout->offsetY = rect.y / 1080.0f;
        layout->height = rect.height / 1080.0f;
    }
    UpdateCollectionViewerArrows();
    nk_context &context = *GetNativeUiContext();
    const std::uint32_t slot = collectionViewerOrder ? collectionViewerOrder[collectionViewerIndex] : std::numeric_limits<std::uint32_t>::max();
    if (slot < collectionSnapshotPriority.size())
        collectionSnapshotPriority[slot] = 1;
    PumpCollectionSnapshotImages();
    context.style.window.fixed_background.data.color.a = 200;
    UI_GatedBegin(context, collectionViewerBackgroundPanel, "SnapshotViewBackground", nk_rect(0.0f, 0.0f, static_cast<float>(g_uiScreenWidth), 1080.0f),
        NK_WINDOW_NO_SCROLLBAR);
    UI_GatedEnd(context);
    if (slot < snapshotImages.size() && GetSnapshotMetadataOwner()->records[slot].use == 1 && snapshotImages[slot].state.load(std::memory_order_acquire) == 2 &&
        collectionViewerWindows[0]) {
        UILayoutRect area{};
        UI_GetWindowRect(collectionViewerWindows[0], area);
        context.style.window.fixed_background.data.color.a = 0;
        UI_GatedBegin(context, collectionViewerPanel, "SnapshotView", nk_rect(area.x, area.y, area.width, area.height), NK_WINDOW_NO_SCROLLBAR);
        UI_GatedLayoutSpaceBegin(context, NK_STATIC, 0.0f, 1);
        struct nk_image image{};
        FillCollectionTextureImage(image, snapshotImages[slot].texture.load(std::memory_order_acquire));
        UILayoutRect fitted{};
        FitCollectionViewerImage(fitted, area, image);
        DrawNativeCollectionPostcard(fitted, &image, 16.0f, collectionViewerBack ? 0.34999999f : 1.0f);
        if (d3t::UIWindow *const back = collectionViewerWindows[7])
            back->layout.offsetX = fitted.x / aspect + back->layout.offsetY;
        UI_GatedLayoutSpaceEnd(context);
        UI_GatedEnd(context);
    }
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::SnapshotViewer)];
    DrawUIEntity(context, element.config.uiHandleIndex, element.index);
    return 0;
}
std::int64_t DestroyCollectionViewer() {
    collectionViewerOrder = nullptr;
    collectionViewerCount = 0;
    collectionViewerIndex = 0;
    RetireCollectionViewerUnderlay(collectionViewerReturn);
    return 1;
}
void ConfirmCollectionSnapshotDelete(std::int32_t response) {
    const std::uint32_t slot = collectionPendingDelete;
    if (response == 2 && slot < snapshotImages.size()) {
        DeleteSnapshotSlot(slot);
        if (collectionPendingDeleteCompletion)
            collectionPendingDeleteCompletion();
    }
    collectionPendingDelete = std::numeric_limits<std::uint32_t>::max();
    collectionPendingDeleteCompletion = nullptr;
}
void RequestCollectionSnapshotDelete(std::uint32_t slot, std::int64_t (*completion)()) {
    if (nativePrompt.active || IsSnapshotDeletePending())
        return;
    collectionPendingDelete = std::numeric_limits<std::uint32_t>::max();
    collectionPendingDeleteCompletion = nullptr;
    if (slot >= snapshotImages.size() || GetSnapshotMetadataOwner()->records[slot].use != 1 || snapshotImages[slot].state.load(std::memory_order_acquire) < 2)
        return;
    collectionPendingDelete = slot;
    collectionPendingDeleteCompletion = completion;
    NativeMessageBox("UISNAPDELE", false, 0,
        ConfirmCollectionSnapshotDelete); // @text EN: "Are you sure you want to delete this image?" | JA: "このスナップショットを削除してもよろしいですか？"
}
std::uint32_t LoadCollectionPresentViewer() {
    return ReadUIDefinitionFile(UIElement::SecretViewer, "/ui/gamemenu_New/SubMenu_Photos_Secret_Viewer.ui", 1);
}
void ShowNextCollectionPresentPage(float *, void *) {
    if (collectionPresentIndex < collectionPresentCount - collectionPresentPageStep) {
        ++collectionPresentIndex;
        AicaExecuteWrapper(3);
    }
}
void ShowPreviousCollectionPresentPage(float *, void *) {
    if (collectionPresentIndex) {
        --collectionPresentIndex;
        AicaExecuteWrapper(3);
    }
}
std::int64_t BindCollectionPresentViewer() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::SecretViewer)].config.uiHandleIndex;
    for (std::size_t window = 0; window < collectionPresentWindows.size(); ++window)
        collectionPresentWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, COLLECTION_PRESENT_VIEWER_WINDOWS[window]));
    if (collectionPresentWindows[1])
        collectionPresentWindows[1]->mouseCallbacks.clicked = ShowPreviousCollectionPresentPage;
    if (collectionPresentWindows[2])
        collectionPresentWindows[2]->mouseCallbacks.clicked = ShowNextCollectionPresentPage;
    if (collectionPresentWindows[3])
        collectionPresentWindows[3]->mouseCallbacks.clicked = ShowPreviousCollectionPresentPage;
    if (collectionPresentWindows[4])
        collectionPresentWindows[4]->mouseCallbacks.clicked = ShowNextCollectionPresentPage;
    if (collectionPresentWindows[0])
        collectionPresentWindows[0]->mouseCallbacks.clicked = ShowNextCollectionPresentPage;
    collectionPresentPlaceholder = UI_FindViewImage(slot, "Image_Placeholder");
    return 1;
}
std::int64_t ResetCollectionPresentViewer() {
    collectionPresentWindows.fill(nullptr);
    collectionPresentPlaceholder = -1;
    return 0;
}
std::int64_t DestroyCollectionPresentViewer() {
    for (std::uint32_t page = 0; page < collectionPresentCount; ++page) {
        d3t::InitTexture2DResource(collectionPresentPages[page].texture);
        collectionPresentPages[page].texture = 0;
    }
    collectionPresentIndex = 0;
    collectionPresentCount = 0;
    RetireCollectionViewerUnderlay(collectionPresentReturn);
    return 1;
}
void OpenCollectionPresent(std::uint32_t group) {
    constexpr std::array<const char *, 7> suffixes{"", "_jp", "_fr", "_de", "_kr", "_tc", "_sc"};
    const auto language = GetLoadingScreenTextType(g_s2SelectedSubtitleLanguage);

    constexpr std::uint8_t japaneseVersion = 0xff;
    const char *suffix = japaneseVersion ? "_jp" : suffixes[static_cast<std::size_t>(language)];
    collectionPresentReturn = static_cast<UIElement>(0);
    collectionPresentIndex = 0;
    collectionPresentCount = 0;
    collectionPresentPageStep = 1;
    collectionPresentFramed = group >= 4;
    while (collectionPresentCount < collectionPresentPages.size()) {
        std::array<char, 272> path{};
        std::snprintf(path.data(), path.size(), "/Snapshots/PresentPic/Present%02u_%02u%s.png", group, collectionPresentCount, suffix);
        if (!ArchiveData_GetFileSize(path.data())) {
            std::snprintf(path.data(), path.size(), "/Snapshots/PresentPic/Present%02u_%02u.png", group, collectionPresentCount);
            if (!ArchiveData_GetFileSize(path.data()))
                break;
        }
        std::strcat(path.data(), "?usage=0");
        collectionPresentPages[collectionPresentCount++].request = static_cast<std::int32_t>(SetupUsageForTexture(path.data(), 0));
    }
    ShowElementByIndex(UIElement::SecretViewer);
}
std::int64_t InputCollectionPresentViewer() {
    const auto pressed = GetPadControlData(1);
    if (pressed & 0x200U)
        DestroyElementByIndex(UIElement::SecretViewer);
    if (pressed & 0x8088U)
        ShowNextCollectionPresentPage(nullptr, nullptr);
    if (pressed & 0x4050U)
        ShowPreviousCollectionPresentPage(nullptr, nullptr);
    return 0;
}
void UpdateCollectionPresentArrows() {
    const bool left = collectionPresentCount > 1 && collectionPresentIndex;
    const bool right = collectionPresentCount > 1 && collectionPresentIndex < collectionPresentCount - collectionPresentPageStep;
    if (collectionPresentWindows[3])
        collectionPresentWindows[3]->alpha = left ? 1.0f : 0.0f;
    if (collectionPresentWindows[4])
        collectionPresentWindows[4]->alpha = right ? 1.0f : 0.0f;
    const bool pointerLeft = GetIsKeyboardActive() == 0 && left, pointerRight = GetIsKeyboardActive() == 0 && right;
    if (collectionPresentWindows[1])
        collectionPresentWindows[1]->alpha = pointerLeft ? 1.0f : 0.0f;
    if (collectionPresentWindows[2])
        collectionPresentWindows[2]->alpha = pointerRight ? 1.0f : 0.0f;
}
void ResolveCollectionPresentPage(CollectionPresentPage &page) {
    if (page.texture)
        return;
    page.texture = GetShaderDataByIndex(static_cast<std::uint32_t>(page.request));
    if (page.texture)
        FillCollectionTextureImage(page.image, page.texture);
}
void DrawCollectionPresentPage(const CollectionPresentPage &page, const UILayoutRect &area) {
    nk_context &context = *GetNativeUiContext();
    UILayoutRect fitted{};
    FitCollectionViewerImage(fitted, area, page.image);
    if (collectionPresentFramed) {
        DrawNativeCollectionPostcard(fitted, &page.image, -1.0f, 1.0f);
        return;
    }
    UI_GatedLayoutSpacePush(context, nk_rect(fitted.x + 3.0f, fitted.y + 3.0f, fitted.width, fitted.height));
    UI_GatedButtonColour(context, nk_rgba(0, 0, 0, 150));
    UI_GatedLayoutSpacePush(context, nk_rect(fitted.x, fitted.y, fitted.width, fitted.height));
    UI_GatedImageColour(context, page.image, nk_rgba(255, 255, 255, 255));
}
std::int64_t RenderCollectionPresentViewer() {
    UpdateCollectionPresentArrows();
    nk_context &context = *GetNativeUiContext();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::SecretViewer)];
    DrawUIEntity(context, element.config.uiHandleIndex, element.index);
    if (collectionPresentIndex >= collectionPresentCount)
        return 0;
    CollectionPresentPage *page = &collectionPresentPages[collectionPresentIndex];
    ResolveCollectionPresentPage(*page);
    if (!collectionPresentWindows[0])
        return 0;
    UILayoutRect area{};
    UI_GetWindowRect(collectionPresentWindows[0], area);
    UI_GatedBegin(context, collectionPresentPanel, "SnapshotView", nk_rect(area.x, area.y, area.width, area.height), NK_WINDOW_NO_SCROLLBAR);
    UI_GatedLayoutSpaceBegin(context, NK_STATIC, 0.0f, 1);
    if (collectionPresentPageStep != 1) {
        DrawCollectionPresentPage(*page, area);
        if (collectionPresentIndex < collectionPresentCount - 1) {
            page = &collectionPresentPages[collectionPresentIndex + 1];
            ResolveCollectionPresentPage(*page);
            DrawCollectionPresentPage(*page, area);
        }
    } else
        DrawCollectionPresentPage(*page, area);
    UI_GatedLayoutSpaceEnd(context);
    UI_GatedEnd(context);
    return 0;
}
template <std::size_t Child> std::int64_t DestroyCollectionChild() {
    if constexpr (Child == 3)
        StopWfScrollMenu();
    if constexpr (Child == 6)
        for (auto &album : collectionAlbums) {
            if (album.thumbnail)
                d3t::InitTexture2DResource(album.thumbnail);
            album.thumbnail = 0;
        }
    return Child == 0 || Child == 1 || Child == 2 || Child == 4 || Child == 5 ? GetDebugForceUIEnabled(collectionChildren[Child]) : 1;
}
std::int64_t ResetPhotoSecretRender() {
    if (!g_uiElements[static_cast<std::size_t>(UIElement::PhotosSecrets)].isRenderable)
        RefreshCollectionAlbums();
    return 0;
}
std::int64_t ResetPhotoSnapshotRender() {
    if (g_uiElements[static_cast<std::size_t>(UIElement::PhotosSnapshot)].isRenderable)
        collectionChild[5].grid.notifySelection = 1;
    return 0;
}
std::int64_t CollectionRootHints() {
    const std::array<std::int16_t, 5> ids{static_cast<std::int16_t>(collectionTabs.shown == 1 ? -1 : 3844), 44, -1, -1, -1};
    return CopyHudHintIds(ids);
}
std::int64_t CollectionPhotoHints() {
    constexpr std::array<std::int16_t, 5> ids{3844, 44, -1, -1, -1};
    return CopyHudHintIds(ids);
}
std::int64_t CollectionPresentHints() {
    constexpr std::array<std::int16_t, 5> ids{-1, 44, -1, -1, -1};
    return CopyHudHintIds(ids);
}
std::int64_t CollectionSnapshotHints() {
    std::array<std::int16_t, 4> ids{3844, 44, 3841, -1};
    const auto selected = GetUIGridSelection(collectionChild[5].grid);
    if (selected >= 0 && static_cast<std::uint32_t>(selected) < snapshotOrderCount &&
        snapshotImages[snapshotOrder[static_cast<std::size_t>(selected)]].state.load(std::memory_order_acquire) >= 2 && !IsSnapshotDeletePending())
        ids[3] = 3840;
    return CopyHudHintIds(ids);
}
std::int64_t CollectionSnapshotViewerHints() {
    const std::array<std::int16_t, 4> ids{3844, 44, -1, static_cast<std::int16_t>(IsSnapshotDeletePending() ? -1 : 3840)};
    return CopyHudHintIds(ids);
}
template <std::size_t Child> void RegisterCollectionChild() {
    auto &element = g_uiElements[static_cast<std::size_t>(collectionChildren[Child])];
    element.name = collectionFiles[Child];
    element.parseDefinitionFromFile = LoadCollectionChild<Child>;
    element.isSelected = BindCollectionChild<Child>;
    element.isPressed = InputCollectionChild<Child>;
    element.render = RenderCollectionChild<Child>;
    element.reset = ResetCollectionChild<Child>;
    element.tick = TickCollectionChild<Child>;
    element.destroy = DestroyCollectionChild<Child>;
    element.fieldAc = Child >= 5 ? 1 : 0;
    if constexpr (Child >= 5)
        element.unknown88 = SetPhotoGridContext<Child>;
    if constexpr (Child == 4 || Child == 6)
        element.unknown98 = CollectionPhotoHints;
    if constexpr (Child == 5) {
        element.unknown98 = CollectionSnapshotHints;
        element.resetRender = ResetPhotoSnapshotRender;
    }
    if constexpr (Child == 6)
        element.resetRender = ResetPhotoSecretRender;
}
template <bool Photos> void RegisterCollectionRoot() {
    auto &root = g_uiElements[static_cast<std::size_t>(CollectionRoot<Photos>())];
    root.name = Photos ? "SubMenu: Photos" : "SubMenu: Shenmue Collection";
    root.parseDefinitionFromFile = LoadCollectionRoot<Photos>;
    root.isSelected = BindCollectionRoot<Photos>;
    root.isPressed = InputCollectionRoot<Photos>;
    root.render = RenderCollectionRoot<Photos>;
    root.reset = ResetCollectionRoot<Photos>;
    root.tick = TickCollectionRoot<Photos>;
    root.destroy = DestroyCollectionRoot<Photos>;
    root.resetRender = ResetCollectionRootRender<Photos>;
    root.fieldAc = Photos ? 1 : 0;
    if constexpr (Photos)
        root.unknown88 = SetPhotosMenuContext;
    if constexpr (!Photos)
        root.unknown98 = CollectionRootHints;
}
std::int64_t CollectionMapsLocalizationNullsub() {
    return 0;
}
void RegisterCollectionAndPhotos() {
    RegisterCollectionRoot<false>();
    RegisterCollectionRoot<true>();
    RegisterCollectionChild<0>();
    RegisterCollectionChild<1>();
    RegisterCollectionChild<2>();
    RegisterCollectionChild<3>();
    RegisterCollectionChild<4>();
    RegisterCollectionChild<5>();
    RegisterCollectionChild<6>();
    UIElementState &maps = g_uiElements[static_cast<std::size_t>(UIElement::CollectionMaps)];
    maps.name = "SubMenu: Collection-Maps-Localisation";
    maps.parseDefinitionFromFile = LoadCollectionMapLocalization;
    maps.isSelected = BindCollectionMapLocalization;
    maps.reset = CollectionMapsLocalizationNullsub;
    for (std::size_t language = 0; language < collectionMapLanguageElements.size(); ++language) {
        UIElementState &element = g_uiElements[static_cast<std::size_t>(COLLECTION_MAP_LANGUAGE_ELEMENTS[language])];
        collectionMapLanguageElements[language] = &element;
        element.isSelected = PumpCollectionMapLanguage;
    }
    auto &viewer = g_uiElements[24];
    viewer.name = "Snapshot Viewer";
    viewer.parseDefinitionFromFile = LoadCollectionViewer;
    viewer.isSelected = BindCollectionViewer;
    viewer.isPressed = InputCollectionViewer;
    viewer.render = RenderCollectionViewer;
    viewer.reset = ResetCollectionViewer;
    viewer.tick = TickCollectionViewer;
    viewer.destroy = DestroyCollectionViewer;
    viewer.fieldAc = 1;
    viewer.unknown98 = CollectionSnapshotViewerHints;
    auto &present = g_uiElements[25];
    present.name = "Secret Viewer";
    present.parseDefinitionFromFile = LoadCollectionPresentViewer;
    present.isSelected = BindCollectionPresentViewer;
    present.isPressed = InputCollectionPresentViewer;
    present.render = RenderCollectionPresentViewer;
    present.reset = ResetCollectionPresentViewer;
    present.tick = TickCollectionViewer;
    present.destroy = DestroyCollectionPresentViewer;
    present.fieldAc = 1;
    present.unknown98 = CollectionPresentHints;
}
void PumpCollectionSnapshotImages() {
    if (!snapshotImagesReady)
        return;
    if (!d3t::d3t_IsTaskActive(snapshotLoadTask)) {
        bool queued = false;
        for (const std::uint32_t slot : snapshotOrder)
            if (slot < collectionSnapshotPriority.size() && collectionSnapshotPriority[slot] && (queued = QueueSnapshotImageLoad(slot)))
                break;
        if (!queued)
            for (const std::uint32_t slot : snapshotOrder)
                if (QueueSnapshotImageLoad(slot))
                    break;
    }
    collectionSnapshotPriority.fill(0);
}
void DrawCollectionSnapshotStatus(std::uint32_t slot, const d3t::UIRect &rectangle, const d3t::UIRect &clip, bool selected) {
    const auto state = snapshotImages[slot].state.load(std::memory_order_acquire);
    auto &definition = uiDefinitions[22];
    const char *id = state == 3 ? "Icon_LoadError" : state == 2 ? "Icon_Delete" : "Icon_Loading";
    if (state == 2 && (!selected || GetIsKeyboardActive() != 1))
        return;
    for (auto &object : definition.textureObjects) {
        if (object.id != id && object.imageName != id)
            continue;
        d3t::UIRect target{rectangle.x + 22, rectangle.y + 22, rectangle.width - 44, rectangle.height - 44};
        float opacity = 1;
        if (state == 2) {
            target = {rectangle.x + rectangle.width - object.width * 0.5f - 8, rectangle.y + rectangle.height - object.height * 0.5f - 8, object.width * 0.5f,
                object.height * 0.5f};
            const float x = uiPointer.position.x * 1920 / GetUIViewportWidth();
            if (uiPointer.leftClicks && x >= target.x && x < target.x + target.width && uiPointer.position.y >= target.y &&
                uiPointer.position.y < target.y + target.height)
                collectionSnapshotDeleteClick = slot;
        } else if (state != 3)
            opacity = collectionSnapshotLoadingTime > 0.75f ? std::sin(collectionSnapshotLoadingTime - 0.75f) * 0.25f + 0.75f : collectionSnapshotLoadingTime;
        const bool hidden = object.hidden;
        object.hidden = false;
        d3t::DrawUITextureObject(definition, object, {target, clip}, opacity);
        object.hidden = hidden;
        break;
    }
}
void ReleaseCollectionMenuResources() {
    SetCollectionUnderlayAlpha(true);
    ReleaseSnapshotImages();
    for (auto &album : collectionAlbums) {
        if (album.thumbnail)
            d3t::InitTexture2DResource(album.thumbnail);
        album = {};
    }
    for (CollectionPresentPage &page : collectionPresentPages) {
        if (page.texture)
            d3t::InitTexture2DResource(page.texture);
        page = {};
    }
    collectionPresentCount = 0;
    collectionViewerCount = 0;
}
} // namespace ui_detail

void UpdateCollectionMapLanguage(std::int32_t language) {
    const UIElementState &source = *collectionMapLanguageElements[static_cast<std::size_t>(language)];
    if (source.config.init == 3) {
        CopyCollectionMapImages(source);
        collectionMapLanguage = language;
        collectionMapLanguagePending = false;
    } else {
        collectionMapLanguagePending = true;
        pendingCollectionMapLanguage = language;
    }
}
void RetireCollectionMenuResources() {
    ReleaseCollectionMenuResources();
}
} // namespace shenmue::s2
