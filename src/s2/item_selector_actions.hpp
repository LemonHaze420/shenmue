#pragma once

#include "s2/item_selector.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/navigation.hpp"
#include "s2/snapshot_viewer.hpp"

namespace shenmue::s2 {
namespace ui_detail {

std::int64_t BeginSelectorMemo();
std::int64_t UpdateSelectorMemo();
void CancelSelectorMemo(std::int32_t force);
std::int64_t BeginSelectorWatch();
std::int64_t UpdateSelectorWatch();
void CancelSelectorFullMap(std::int32_t force);
std::int64_t BeginSelectorItemLook();
std::int64_t UpdateSelectorItemLook();
void CancelSelectorItemLook(std::int32_t force);
std::int64_t BeginSelectorSave();
std::int64_t UpdateSelectorSave();
void CompleteSelectorSave();
std::int64_t BeginSelectorEasyOptions();
std::int64_t UpdateSelectorEasyOptions();
void CancelSelectorEasyOptions(std::int32_t force);
std::int64_t BeginSnapshotDebug();
std::int64_t UpdateSelectorMoves();
std::int64_t BeginSelectorCollection();
std::int64_t BeginSelectorPhotos();
std::int64_t UpdateSelectorCollection();
std::int64_t UpdateSelectorPhotos();
void CancelSelectorCollection(std::int32_t force);
std::int64_t UpdateSelectorNavigation();
void CancelSelectorNavigation(std::int32_t force);
std::int64_t BeginSelectorPawn();
std::int64_t UpdateSelectorPawn();
void CancelSelectorPawn(std::int32_t force);
std::int64_t BeginSelectorMoveHandbook();
std::int64_t UpdateSelectorMoveHandbook();
void CancelSelectorMoveHandbook(std::int32_t force);

inline constexpr std::array<ItemSelectorActionCallbacks, 13> ITEM_SELECTOR_CALLBACKS{
    {{.begin = BeginSelectorMemo, .update = UpdateSelectorMemo, .complete = nullptr, .cancel = CancelSelectorMemo},
        {.begin = BeginSelectorWatch, .update = UpdateSelectorWatch, .complete = nullptr, .cancel = nullptr},
        {.begin = OpenFullMapMenu, .update = UpdateFullMapMenu, .complete = CloseFullMapMenu, .cancel = CancelSelectorFullMap},
        {.begin = BeginSelectorItemLook, .update = UpdateSelectorItemLook, .complete = nullptr, .cancel = CancelSelectorItemLook},
        {.begin = BeginSelectorSave, .update = UpdateSelectorSave, .complete = CompleteSelectorSave, .cancel = nullptr},
        {.begin = BeginSelectorEasyOptions, .update = UpdateSelectorEasyOptions, .complete = nullptr, .cancel = CancelSelectorEasyOptions},
        {.begin = BeginSnapshotDebug, .update = CheckTextureData, .complete = nullptr, .cancel = nullptr},
        {.begin = nullptr, .update = UpdateSelectorMoves, .complete = nullptr, .cancel = CancelWazamakiMenu},
        {.begin = BeginSelectorCollection, .update = UpdateSelectorCollection, .complete = nullptr, .cancel = CancelSelectorCollection},
        {.begin = BeginSelectorPhotos, .update = UpdateSelectorPhotos, .complete = nullptr, .cancel = CancelSelectorCollection},
        {.begin = OpenNaviWindow, .update = UpdateSelectorNavigation, .complete = nullptr, .cancel = CancelSelectorNavigation},
        {.begin = BeginSelectorPawn, .update = UpdateSelectorPawn, .complete = nullptr, .cancel = CancelSelectorPawn},
        {.begin = BeginSelectorMoveHandbook, .update = UpdateSelectorMoveHandbook, .complete = nullptr, .cancel = CancelSelectorMoveHandbook}}};

} // namespace ui_detail
} // namespace shenmue::s2
