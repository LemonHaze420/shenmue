#pragma once

#include "d3t/ui_runtime.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr std::array<const char *, 8> battleQteAxes{"Up", "Up_Right", "Right", "Down_Right", "Down", "Down_Left", "Left", "Up_Left"};
inline constexpr std::array<const char *, 14> battleQteButtons{
    "DPad",
    "Buttons",
    "Up",
    "Down",
    "Left",
    "Right",
    "Up",
    "Down",
    "Left",
    "Right",
    "X",
    "Y",
    "A",
    "B",
};
inline constexpr std::array<std::int32_t, 14> battleQtePanelInputMasks{
    -1,
    -1,
    0x1000,
    0x2000,
    0x4000,
    0x8000,
    0x1000,
    0x2000,
    0x4000,
    0x8000,
    -1,
    -1,
    -1,
    -1,
};
inline constexpr std::array<std::uint16_t, 12> battleQteSingleMasks{1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800};
inline constexpr std::array<std::int32_t, 12> battleQteSingleFrames{7, 11, 5, 9, 6, 10, 4, 8, 1, 2, 3, 0};
static_assert(offsetof(d3t::UIWindow, layout) + offsetof(d3t::UIWindowLayout, offsetX) == 0x98);
static_assert(offsetof(d3t::UIWindow, layout) + offsetof(d3t::UIWindowLayout, width) + offsetof(d3t::UIWindowLength, scale) == 0xb0);
static_assert(offsetof(d3t::UIWindow, layout) + offsetof(d3t::UIWindowLayout, height) + offsetof(d3t::UIWindowLength, scale) == 0xbc);
static_assert(offsetof(UIButtonIconWidget, action) == 0x84 && offsetof(UIButtonIconWidget, colour) == 0xb0);
void SetupBattleQteWidgets();
void RenderBattleQteWidgets();
void SetBattleQteUIMode(std::int32_t mode);
std::int32_t GetBattleQteUIMode() noexcept;
void RefreshBattleQteUIInputDevice();
void SetupBattleQteIcon(std::int32_t selection, std::uint8_t flags);
void SetBattleQteDirectionIcon(std::int32_t index, float scale, float alpha);
void SetBattleQteActionIcon(std::int32_t index, float scale, float alpha);
void BeginBattleQteDeparture();
bool IsBattleQteTransitionActive();
void ResetBattleQteTransition();

} // namespace shenmue::s2
