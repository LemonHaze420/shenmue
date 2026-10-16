#pragma once

#include <array>
#include <cstdint>
#include "d3t/ui_runtime.hpp"

namespace shenmue::s2 {
inline constexpr std::array<std::uint32_t, 2> COLLECTION_TAB_BUTTON_COLOURS{0x7f322b1cU, 0xff322b1cU};
inline constexpr std::array<std::uint32_t, 2> COLLECTION_TAB_TEXTURE_COLOURS{0x00ffffffU, 0xa0ffffffU};
inline constexpr std::array<std::uint32_t, 2> PHOTOS_FRONT_TAB_BUTTON_COLOURS{0x7f34595eU, 0xc1163f4cU};
inline constexpr std::array<std::uint32_t, 2> PHOTOS_FRONT_TAB_TEXTURE_COLOURS{0x00ffffffU, 0x1ea5e6ffU};
inline constexpr std::uint32_t PHOTOS_FRONT_MAIN_BUTTON_COLOUR = 0xff3f6a70U, PHOTOS_FRONT_BACKGROUND_COLOUR = 0xa5ffffffU;
inline constexpr std::array<const char *, 10> PHOTOS_TAB_WINDOWS{"SubMenu_Photos::Tab1Button", "SubMenu_Photos::Tab2Button", "SubMenu_Photos::Tab3Button",
    "SubMenu_Photos::TabIcon1", "SubMenu_Photos::TabIcon2", "SubMenu_Photos::TabIcon3", "SubMenu_Photos::TabLabel1", "SubMenu_Photos::TabLabel2",
    "SubMenu_Photos::TabLabel3", "SubMenu_Photos::Panel_Root"};
inline constexpr std::array<const char *, 11> PHOTOS_TAB_IMAGES{"SubMenu_Photos::Image_Tab_1", "SubMenu_Photos::Image_Tab_2", "SubMenu_Photos::Image_Tab_3",
    "SubMenu_Photos::Button_TextureFront_1", "SubMenu_Photos::Button_TextureFront_2", "SubMenu_Photos::Button_TextureFront_3",
    "SubMenu_Photos::Button_TextureGame_1", "SubMenu_Photos::Button_TextureGame_2", "SubMenu_Photos::Button_TextureGame_3", "SubMenu_Photos::TextureGame",
    "SubMenu_Photos::TextureFront"};
inline constexpr std::array<const char *, 7> PHOTOS_GRID_IMAGE_SUFFIXES{"GridElementTexture_Game", "GridElementTexture_Game2", "GridElementTexture_Front2",
    "GridElementTexture_Front", "GridElementBackground_Game", "GridElementBackground_Front", "GridElementBackground_Selected"};
inline constexpr std::array<std::uint32_t, 2> PHOTOS_GRID_BUTTON_COLOURS{0xbe0c222fU, 0xdc272415U};
struct CollectionAlbum {
    std::uint32_t request;
    std::uint32_t thumbnail;
    struct nk_image image;
    std::array<std::int32_t, 126> slots;
    std::int32_t count;
    std::array<std::byte, 4> reserved224;
};
static_assert(sizeof(CollectionAlbum) == 0x228);
static_assert(offsetof(CollectionAlbum, image) == 8 && offsetof(CollectionAlbum, slots) == 40 && offsetof(CollectionAlbum, count) == 544);
}
