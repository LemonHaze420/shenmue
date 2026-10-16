#pragma once
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_ESOP = SHEN_NAME(ESOP);
inline constexpr shen_name EASY_OPTION_LABEL_TEXTURE = SHEN_NAME(EZ00);
inline constexpr shen_name EASY_OPTION_DESCRIPTION_TEXTURE = SHEN_NAME(EZEX);

struct EasyOptionCaption {
    std::int8_t first, second;
    std::int16_t size, x, y, condition, value;
};
static_assert(sizeof(EasyOptionCaption) == 12);
static_assert(offsetof(EasyOptionCaption, size) == 2);
static_assert(offsetof(EasyOptionCaption, x) == 4);
static_assert(offsetof(EasyOptionCaption, condition) == 8);
inline constexpr std::array<const char *, 15> EASY_OPTION_LABELS{
    "Dialog/Text",
    "Thumbsticks",
    "Toggle",
    "Controls",
    "Shenmue Mode",
    "Cinema Mode",
    "Text Mode",
    "Game Mode",
    "On ",
    "Off",
    " : Run ",
    " : Zoom",
    "Look",
    "Move",
    "Thumb Pad",
};
inline constexpr std::array<std::int8_t, 15> EASY_OPTION_LABEL_WIDTHS{4, 4, 2, 2, 4, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4};
inline constexpr std::array<const char *, 8> EASY_OPTION_DESCRIPTIONS{
    "Play using dialog only.",
    "Play using both dialog and text.",
    "Text is displayed when skip is used.",
    "Play using text only.",
    "Toggle the setting of",
    "the thumbsticks.",
    "Toggle the Run and Zoom ",
    "control buttons.",
};
inline constexpr std::array<std::array<std::int8_t, 2>, 4> EASY_OPTION_DIALOGUE_DESCRIPTIONS{{{0, -1}, {1, 2}, {1, -1}, {3, -1}}};
inline constexpr std::array<std::uint8_t, 4> EASY_OPTION_DIALOGUE_LABELS{5, 4, 7, 6};
inline constexpr std::array<std::array<std::int16_t, 2>, 8> EASY_OPTION_BOX_SIZES{{
    {568, 80},
    {248, 80},
    {248, 40},
    {200, 80},
    {200, 40},
    {136, 80},
    {136, 40},
    {0, 0},
}};
inline constexpr std::array<EasyOptionCaption, 7> EASY_OPTION_CAPTIONS{{
    {.first = 0, .second = -1, .size = 4, .x = 0, .y = 0, .condition = 2, .value = 0},
    {.first = 1, .second = -1, .size = 3, .x = 0, .y = 64, .condition = 2, .value = 1},
    {.first = 2, .second = 3, .size = 3, .x = 0, .y = 148, .condition = 2, .value = 2},
    {.first = 12, .second = -1, .size = 6, .x = 200, .y = 64, .condition = 4, .value = 0},
    {.first = 13, .second = -1, .size = 6, .x = 400, .y = 64, .condition = 4, .value = 1},
    {.first = 11, .second = 10, .size = 5, .x = 200, .y = 148, .condition = 5, .value = 0},
    {.first = 10, .second = 11, .size = 5, .x = 400, .y = 148, .condition = 5, .value = 1},
}};

} // namespace shenmue::s2
