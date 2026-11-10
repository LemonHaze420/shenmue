#pragma once
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr std::array<const char *, 2> UI_LEGACY_HUD_PANELS{"GameHUDLegacy_MainHudPanel", "GameHUDLegacy_ExtraHudPanel"};
inline constexpr std::array<const char *, 5> UI_LEGACY_HUD_BUTTONS{"A", "B", "X", "Y", "RB"};
inline constexpr std::array<const char *, 4> UI_LEGACY_HUD_BUTTON_PARTS{"Base", "Off", "On", "Icon"};
inline constexpr std::array<const char *, 5> UI_LEGACY_HUD_ICONS{"Confirm_Icon", "Cancel_Icon", "Event_Icon", "System_Icon", "Snapshot_Icon"};
inline constexpr std::array<const char *, 2> UI_INFO_BAR_PANELS{"InfoBar_LabelPanel", "InfoBar_LabelPanelBackground"};

std::int64_t LegacyUICallbackReturnZero();
std::int64_t LegacyUICallbackReturnOne();
void LegacyUICallbackNullsub();
void RefreshNotebookAndUpdateView(std::uint32_t slot);

std::uint32_t LoadOptionsGamma();
std::int64_t RenderOptionsGamma();
std::int64_t OptionsGammaInput();
std::int64_t GetOptionsGammaInitState();
std::uint32_t LoadOptionsBackground();
std::int64_t RenderOptionsBackground();
std::int64_t GetOptionsBackgroundInitState();

std::uint32_t LoadLegacyGameHud();
std::int64_t ReloadLegacyGameHud();
std::int64_t GetLegacyGameHudInitState();
std::int64_t GetGameQteInitState();
std::int64_t GetGameHelpLoadState();

std::int64_t GetMainLoadRowCount();
void SetMainLoadListMode(std::int32_t mode);
void SetMainLoadButtonLabels();
void InitializeMainLoad();
void BindMainLoad();
std::uint32_t LoadMainLoad();
std::int64_t RenderMainLoad();
std::int64_t LoadGame();
std::int64_t GetMainLoadMode();

void SetLegacyMainMenuSubmenuActive(std::int32_t active);
std::uint32_t LoadLegacyMainMenu();
std::int64_t RenderLegacyMainMenu();
std::uint8_t UpdateLegacyMainMenuS1Prompt();
std::int64_t LegacyMainMenuInput();
std::int64_t GetLegacyMainMenuInitState();
void ResetLegacyMainMenuWheel();

std::uint32_t LoadInfoBar();
std::int64_t BindInfoBar();
std::int64_t RenderInfoBar();
std::int64_t ShowInfoBar();
std::int64_t HideInfoBar();
std::int64_t IsInfoBarAnimating();
} // namespace shenmue::s2
