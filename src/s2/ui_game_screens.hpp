#pragma once
#include "d3t/ui_runtime.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr std::array<std::array<char, 64>, 4> GAME_COLLECTION_CONTENT_PANELS{
    {"GameCollection_ContentParent", "GameCollection_ContentPanel", "GameCollection_CollectionPanel", "GameCollection_CollectionNamePanel"}};
inline constexpr std::array<std::int32_t, 2> GAME_COLLECTION_TAB_CATEGORIES{0, 1};
inline constexpr std::array<std::int32_t, 4> GAME_COLLECTION_MAP_FOLDER_SIZES{6, 8, 11, -1};
inline constexpr std::array<std::int32_t, 12> GAME_HOME_SUBMENUS{44, 45, -1, 46, 47, 30, 48, 57, 49, -1, -1, -1};
inline constexpr std::int32_t GAME_COLLECTION_ITEM_CAPACITY = 275;
inline constexpr std::int32_t GAME_COLLECTION_PAWNSHOP_COUNT = 11;

void RegisterLegacyGameScreens();

std::uint32_t LoadGameCollection();
void BindGameCollection();
std::int64_t RenderGameCollection();
std::int64_t GameCollectionInput();
void OpenGameCollection();
std::int64_t CloseGameCollection();
std::int64_t ReloadGameCollection();
std::int64_t GetGameCollectionInitState();
void SetGameCollectionElement(std::int32_t record, std::int32_t slot);
void SetGameCollectionContentVisible(std::int32_t visible);
void SelectGameCollectionItem();
void GetGameCollectionIconRect(std::int32_t tab, std::uint32_t index, float *u0, float *u1, float *v0, float *v1);
std::int32_t GetGameCollectionIconImage(std::int32_t tab, std::uint32_t index);
void UpdateGameCollectionPanelAlpha();
void SetGameCollectionButtonPanel(std::int32_t select);
void RefreshGameCollectionPage();
void SetGameCollectionInputEnabled(std::int32_t enabled);

std::uint32_t LoadCollectionMaps();
void BindCollectionMaps();
std::int64_t RenderCollectionMaps();
std::int64_t GetCollectionMapsInitState();
void SetCollectionMapsSheet(std::int32_t sheet);
std::int64_t CloseCollectionMaps();
void BuildCollectionMapFolders();
void OpenCollectionMaps();
std::int32_t CollectionMapsInput();
void RefreshCollectionMapRows();

std::uint32_t LoadCollectionPawnshop();
void BindCollectionPawnshop();
std::int64_t RenderCollectionPawnshop();
std::int64_t GetCollectionPawnshopInitState();
void OpenCollectionPawnshop();
std::int32_t CollectionPawnshopInput();
void RefreshCollectionPawnshopRows();

std::uint32_t LoadGameWatch();
void BindGameWatch();
std::int64_t RenderGameWatch();
std::int64_t GameWatchInputNullsub();
void OpenGameWatch();
std::int64_t CloseGameWatch();
std::int64_t GetGameWatchInitState();

std::uint32_t LoadGameQuit();
void BindGameQuit();
std::int64_t RenderGameQuit();
std::int64_t GameQuitInput();
void OpenGameQuit();
std::int64_t CloseGameQuit();
std::int64_t ReloadGameQuit();
std::int64_t GetGameQuitInitState();
void ConfirmGameQuit();
void RequestGameHomeQuit();
std::int64_t CloseGameHomeSubMenu();
} // namespace shenmue::s2
