#pragma once
#include "s2/areas/area_omak.hpp"

namespace shenmue::s2::areas {
struct OmakeSpriteSetup {
    shen_name name;
    std::int32_t alignment, x, y, width, height, texture;
    float depth;
    std::uint32_t flags;
};
inline constexpr OmakeSpriteSetup OMAKE_SPRITE_SETUP[]{
    {.name = SHEN_NAME(BG00), .alignment = 5, .x = 320, .y = 240, .width = 640, .height = 640, .texture = 23, .depth = 0, .flags = 0x400},
    {.name = SHEN_NAME(TTBG), .alignment = 5, .x = 320, .y = 55, .width = 512, .height = 512, .texture = 10, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(TITL), .alignment = 5, .x = 320, .y = 55, .width = 512, .height = 512, .texture = 8, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BG13), .alignment = 5, .x = 320, .y = 129, .width = 256, .height = 256, .texture = 0, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(IC13), .alignment = 5, .x = 320, .y = 129, .width = 128, .height = 128, .texture = 17, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BG12), .alignment = 5, .x = 214, .y = 127, .width = 128, .height = 128, .texture = 0, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(IC12), .alignment = 5, .x = 214, .y = 127, .width = 64, .height = 64, .texture = 16, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BG14), .alignment = 5, .x = 426, .y = 127, .width = 128, .height = 128, .texture = 0, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(IC14), .alignment = 5, .x = 426, .y = 127, .width = 64, .height = 64, .texture = 2, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BG10), .alignment = 5, .x = 155, .y = 129, .width = 128, .height = 128, .texture = 1, .depth = 1, .flags = 0x500},
    {.name = SHEN_NAME(BG16), .alignment = 5, .x = 485, .y = 129, .width = 128, .height = 128, .texture = 1, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(MNBX), .alignment = 5, .x = 320, .y = 272, .width = 1024, .height = 1024, .texture = 11, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(SCBX), .alignment = 5, .x = 562, .y = 272, .width = 256, .height = 256, .texture = 3, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(SCBA), .alignment = 5, .x = 570, .y = 220, .width = 128, .height = 128, .texture = 4, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(BGM0), .alignment = 5, .x = 376, .y = 210, .width = 512, .height = 512, .texture = 5, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BGM1), .alignment = 5, .x = 376, .y = 252, .width = 512, .height = 512, .texture = 5, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BGM2), .alignment = 5, .x = 376, .y = 294, .width = 512, .height = 512, .texture = 5, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(BGM3), .alignment = 5, .x = 376, .y = 336, .width = 512, .height = 512, .texture = 5, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(MNU0), .alignment = 5, .x = 376, .y = 210, .width = 512, .height = 32, .texture = -1, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(MNU1), .alignment = 5, .x = 376, .y = 252, .width = 512, .height = 32, .texture = -1, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(MNU2), .alignment = 5, .x = 376, .y = 294, .width = 512, .height = 32, .texture = -1, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(MNU3), .alignment = 5, .x = 376, .y = 336, .width = 512, .height = 32, .texture = -1, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(PHOT), .alignment = 5, .x = 126, .y = 272, .width = 256, .height = 256, .texture = 9, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(PFRM), .alignment = 5, .x = 133, .y = 272, .width = 256, .height = 256, .texture = 6, .depth = 3, .flags = 0x400},
    {.name = SHEN_NAME(ISBG), .alignment = 5, .x = 323, .y = 412, .width = 512, .height = 512, .texture = 12, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(INS1), .alignment = 5, .x = 320, .y = 395, .width = 512, .height = 32, .texture = -1, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(INS2), .alignment = 5, .x = 320, .y = 433, .width = 512, .height = 32, .texture = -1, .depth = 2, .flags = 0x400},
    {.name = SHEN_NAME(KEYS), .alignment = 0, .x = 6, .y = 66, .width = 128, .height = 128, .texture = 15, .depth = 1, .flags = 0x400},
    {.name = SHEN_NAME(BTNB), .alignment = 0, .x = 511, .y = 66, .width = 128, .height = 128, .texture = 13, .depth = 1, .flags = 0x400},
};
} // namespace shenmue::s2::areas
