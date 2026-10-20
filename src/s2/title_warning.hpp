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

inline constexpr std::array<std::array<std::uint16_t, 4>, 14> TITLE_WARNING_DECORATION_RECTANGLES{
    {{0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32}, {96, 0, 32, 32}, {0, 32, 32, 32}, {32, 32, 32, 32}, {64, 32, 32, 32}, {96, 32, 32, 32}, {0, 64, 32, 32},
        {32, 64, 32, 32}, {0, 96, 32, 32}, {32, 96, 32, 32}, {64, 96, 32, 32}, {96, 96, 32, 32}}};
inline constexpr std::array<std::array<float, 4>, 10> TITLE_WARNING_DIGIT_UV{{{0.0f, 0.625f, 0.140625f, 1.0f}, {0.140625f, 0.625f, 0.28125f, 1.0f},
    {0.28125f, 0.625f, 0.421875f, 1.0f}, {0.421875f, 0.625f, 0.5625f, 1.0f}, {0.5625f, 0.625f, 0.703125f, 1.0f}, {0.703125f, 0.625f, 0.84375f, 1.0f},
    {0.84375f, 0.625f, 0.984375f, 1.0f}, {0.0f, 0.25f, 0.140625f, 0.625f}, {0.140625f, 0.25f, 0.28125f, 0.625f}, {0.28125f, 0.25f, 0.421875f, 0.625f}}};
inline constexpr std::array<std::int32_t, 3> TITLE_WARNING_DIGIT_X{142, 142, 146};
void LoadTitleWarningLogo(TitleWarningTaskData *data, std::int32_t texture, std::int32_t x, std::int32_t y);
void DrawTitleWarningDecorations(std::int32_t logo, const std::byte *parameters);
void FreeTitleWarningResources();
enum class TitleWarningPhase : std::int32_t { Inactive = 0, Begin = 1, Visible = 2, Tail = 3 };
inline constexpr std::array<std::array<std::int16_t, 2>, 47> TITLE_WARNING_LOGOS{{
    {-1,-1},{0,7},{1,4},{1,5},{3,6},{0,7},{0,7},{0,8},{1,9},{3,10},{1,11},{1,12},
    {1,12},{1,13},{3,14},{3,-1},{1,-1},{0,-1},{1,-1},{0,-1},{1,-1},{1,-1},{0,-1},{3,-1},
    {3,-1},{1,-1},{3,-1},{3,-1},{1,15},{1,16},{3,17},{0,18},{0,19},{3,20},{0,21},{0,22},
    {0,23},{0,24},{3,25},{0,26},{0,1},{0,2},{3,27},{0,28},{3,29},{3,30},{0,31}
}};
inline constexpr std::array<std::int16_t, 4> TITLE_WARNING_LOGO_Y{240, 240, 400, 323};

} // namespace shenmue::s2
