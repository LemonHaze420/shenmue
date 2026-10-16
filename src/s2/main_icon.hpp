#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_MAIN_ICON = SHEN_NAME(ICON);
inline constexpr shen_name BUTTON_MAIN_ICON_SHORTCUT = SHEN_NAME(BTMP);
inline constexpr shen_name BUTTON_MAIN_ICON_MEMO = SHEN_NAME(BTME);
inline constexpr shen_name BUTTON_MAIN_ICON_ITEM_HELP = SHEN_NAME(BTIL);
inline constexpr shen_name BUTTON_MAIN_ICON_ITEM_SELECT = SHEN_NAME(BTSW);

struct MainIconShortcut {
    std::int32_t flag;
    std::int16_t item;
    std::uint16_t reserved;
};
static_assert(sizeof(MainIconShortcut) == 8);
static_assert(offsetof(MainIconShortcut, flag) == 0);
static_assert(offsetof(MainIconShortcut, item) == 4);
static_assert(offsetof(MainIconShortcut, reserved) == 6);

inline constexpr std::array<MainIconShortcut, 4> MAIN_ICON_SHORTCUTS{{
    {.flag = 532, .item = 6025, .reserved = 0},
    {.flag = 533, .item = 6026, .reserved = 0},
    {.flag = 534, .item = 6018, .reserved = 0},
    {.flag = 535, .item = 6017, .reserved = 0},
}};

} // namespace shenmue::s2
