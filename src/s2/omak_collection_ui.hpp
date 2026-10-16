#pragma once
#include <cstdint>
#include <array>
#include <cstddef>

namespace shenmue::s2 {
struct OmakeCollectionRow {
    std::array<char, 256> name;
    std::array<char, 512> description;
    std::uint8_t unlocked, reserved[3];
    std::int32_t category, index;
};
static_assert(sizeof(OmakeCollectionRow) == 780 && offsetof(OmakeCollectionRow, category) == 772);
inline constexpr std::int32_t OMAKE_COLLECTION_PREVIEWS[3][12]{
    {24, 25, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0}, {14, 15, 16, 0, 17, 18, 20, 21, 1, 19, 0, 0}, {13, 2, 3, 7, 8, 6, 5, 4, 10, 9, 11, 12}};
inline constexpr std::uint32_t OMAKE_MAIN_MENU_PAGES[]{26, 57, 16, 20, 29, 15};
inline constexpr std::int16_t OMAKE_COLLECTION_HINTS[]{45, 44, -1, -1};
inline constexpr const char *OMAKE_MAIN_MENU_ROOTS[]{
    "MainMenu_WheelContainer", "LoadScreen::Load_Screen_Panel", "Options_MainPanel", "Snapshots_BackgroundPanel", nullptr, nullptr};
inline constexpr const char *OMAKE_MAIN_MENU_BACKGROUNDS[]{"MainMenu::ContentBackground", "Texture_Alpha_Front", "Texture_Alpha_Front2"};
extern std::int32_t omakeCollectionSuppressed;
void SetOmakeCollectionSuppressed(std::int32_t suppressed);
bool SelectMainMenuPage(std::uint32_t page);
std::int64_t BindOmakeCollectionUI();
std::int64_t TickOmakeCollectionUI();
std::int64_t RenderOmakeCollectionUI();
std::int64_t DestroyOmakeCollectionUI();
std::int64_t InputOmakeCollectionUI();
std::int64_t ResetOmakeCollectionUI();
std::int64_t ResetOmakeCollectionRender();
std::int64_t DestroyMainMenuPage();
std::int64_t OmakeCollectionHints();
} // namespace shenmue::s2
