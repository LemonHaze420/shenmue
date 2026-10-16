#pragma once
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr std::array<const char *, 3> UI_PROMPT_WINDOWS{
    "YesNoSelector::SelectorContainer_Left", "YesNoSelector::SelectorContainer_Right", "YesNoSelector::QuestionContainer"};
inline constexpr std::array<const char *, 2> UI_PROMPT_LABEL_IMAGES{"YesNoSelector::LeftText", "YesNoSelector::RightText"};
inline constexpr std::array<const char *, 4> UI_PROMPT_SPRITES{
    "YesNoSelector::SelectedLeft", "YesNoSelector::UnselectedLeft", "YesNoSelector::SelectedRight", "YesNoSelector::UnselectedRight"};
inline constexpr std::array<const char *, 16> UI_PROMPT_IMAGES{"YesNo_Off", "YesNo_On", "Yes_EN", "Yes_JP", "Yes_FR", "Yes_DE", "Yes_KR", "Yes_CT", "Yes_CS",
    "No_EN", "No_JP", "No_FR", "No_DE", "No_KR", "No_CT", "No_CS"};
inline constexpr std::array<std::int16_t, 4> UI_PROMPT_HINTS{45, 44, -1, -1};
}
