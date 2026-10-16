#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TITLE_WARNING_TASK = SHEN_NAME(TWRN);
struct TitleWarningTaskData {
    std::int32_t textureIndex;
    std::uint32_t reserved04;
};
static_assert(sizeof(TitleWarningTaskData) == 0x08);
static_assert(offsetof(TitleWarningTaskData, textureIndex) == 0x00);
static_assert(offsetof(TitleWarningTaskData, reserved04) == 0x04);
static_assert(alignof(TitleWarningTaskData) == 0x04);

enum class TitleWarningPhase : std::int32_t { Inactive = 0, Begin = 1, Visible = 2, Tail = 3 };
inline constexpr std::array<std::array<std::int16_t, 2>, 47> TITLE_WARNING_LOGOS{{
    {-1,-1},{0,7},{1,4},{1,5},{3,6},{0,7},{0,7},{0,8},{1,9},{3,10},{1,11},{1,12},
    {1,12},{1,13},{3,14},{3,-1},{1,-1},{0,-1},{1,-1},{0,-1},{1,-1},{1,-1},{0,-1},{3,-1},
    {3,-1},{1,-1},{3,-1},{3,-1},{1,15},{1,16},{3,17},{0,18},{0,19},{3,20},{0,21},{0,22},
    {0,23},{0,24},{3,25},{0,26},{0,1},{0,2},{3,27},{0,28},{3,29},{3,30},{0,31}
}};
inline constexpr std::array<std::int16_t, 4> TITLE_WARNING_LOGO_Y{240, 240, 400, 323};

} // namespace shenmue::s2
