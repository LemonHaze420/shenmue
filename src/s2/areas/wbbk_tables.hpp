#pragma once
#include "s2/areas/wbbk_motion.hpp"
#include <cstddef>

namespace shenmue::s2::areas {
inline constexpr shen_name BOOK_BASE{SHEN_NAME(base)}, BOOK_DYNAMIC{SHEN_NAME(DYNB)}, BOOK_FIRST{SHEN_NAME(AZOU)};
struct BookStackPlacement {
    Vector3 position;
    float label;
    std::int32_t yaw;
};
static_assert(sizeof(BookStackPlacement) == 0x14);
inline constexpr BookStackPlacement bookStackPlacements[]{{.position = {.x = 350.10001f, .y = 127.87000f, .z = 230.36f}, .label = 5, .yaw = 0},
    {.position = {.x = 349.51999f, .y = 127.87000f, .z = 230.36f}, .label = 4, .yaw = 0},
    {.position = {.x = 348.92001f, .y = 127.87000f, .z = 230.36f}, .label = 3, .yaw = 0},
    {.position = {.x = 348.32001f, .y = 127.87000f, .z = 230.36f}, .label = 2, .yaw = 0},
    {.position = {.x = 347.67001f, .y = 127.87000f, .z = 230.36f}, .label = 1, .yaw = 0},
    {.position = {.x = 345.45001f, .y = 127.87000f, .z = 230.36f}, .label = 0, .yaw = 0},
    {.position = {.x = 344.78000f, .y = 127.87000f, .z = 230.36f}, .label = 0, .yaw = 0},
    {.position = {.x = 350.10001f, .y = 127.87000f, .z = 233.36f}, .label = 5, .yaw = 32768},
    {.position = {.x = 349.51999f, .y = 127.87000f, .z = 233.36f}, .label = 4, .yaw = 32768},
    {.position = {.x = 348.92001f, .y = 127.87000f, .z = 233.36f}, .label = 3, .yaw = 32768},
    {.position = {.x = 348.32001f, .y = 127.87000f, .z = 233.36f}, .label = 2, .yaw = 32768},
    {.position = {.x = 347.67001f, .y = 127.87000f, .z = 233.36f}, .label = 1, .yaw = 32768},
    {.position = {.x = 345.45001f, .y = 127.87000f, .z = 233.36f}, .label = 0, .yaw = 32768},
    {.position = {.x = 344.78000f, .y = 127.87000f, .z = 233.36f}, .label = 0, .yaw = 32768}, {.position = {.x = -1, .y = -1, .z = -1}, .label = 0, .yaw = 0}};
struct BookPartPlacement {
    Vector3 position;
    Vector3i rotation;
    std::int32_t category;
};
static_assert(sizeof(BookPartPlacement) == 0x1c);
inline constexpr BookPartPlacement bookPutPartPlacements[]{
    {.position = {.x = -1.3f, .y = .02f, .z = -.52f}, .rotation = {.x = 0, .y = 396, .z = 0}, .category = 1},
    {.position = {.x = -1, .y = .04f, .z = -.53f}, .rotation = {.x = 0, .y = 64932, .z = 0}, .category = 1},
    {.position = {.x = -.72f, .y = .01f, .z = -.55f}, .rotation = {.x = 0, .y = 479, .z = 0}, .category = 1},
    {.position = {.x = -1.37f, .y = .01f, .z = -.13f}, .rotation = {.x = 0, .y = 65182, .z = 0}, .category = 1},
    {.position = {.x = -1.1f, .y = .01f, .z = -.12f}, .rotation = {.x = 0, .y = 604, .z = 0}, .category = 1},
    {.position = {.x = -.81f, .y = .01f, .z = -.18f}, .rotation = {.x = 0, .y = 64639, .z = 0}, .category = 1},
    {.position = {.x = -1.38f, .y = .02f, .z = .16f}, .rotation = {.x = 0, .y = 250, .z = 0}, .category = 1},
    {.position = {.x = -1.1f, .y = .01f, .z = .23f}, .rotation = {.x = 0, .y = 16751, .z = 0}, .category = 1},
    {.position = {.x = -.71f, .y = .01f, .z = .2f}, .rotation = {.x = 0, .y = 396, .z = 0}, .category = 1},
    {.position = {.x = -1.36f, .y = .04f, .z = .58f}, .rotation = {.x = 0, .y = 64932, .z = 0}, .category = 1},
    {.position = {.x = -.99f, .y = .02f, .z = .55f}, .rotation = {.x = 0, .y = 48744, .z = 0}, .category = 2},
    {.position = {.x = -.63f, .y = .02f, .z = .57f}, .rotation = {.x = 0, .y = 65078, .z = 0}, .category = 2},
    {.position = {.x = -.37f, .y = .01f, .z = -.55f}, .rotation = {.x = 0, .y = 65265, .z = 0}, .category = 2},
    {.position = {.x = -.04f, .y = .01f, .z = -.55f}, .rotation = {.x = 0, .y = 65182, .z = 0}, .category = 2},
    {.position = {.x = .25f, .y = .04f, .z = -.57f}, .rotation = {.x = 0, .y = 980, .z = 0}, .category = 2},
    {.position = {.x = -.39f, .y = .02f, .z = -.17f}, .rotation = {.x = 0, .y = 15186, .z = 0}, .category = 2},
    {.position = {.x = .02f, .y = .02f, .z = -.15f}, .rotation = {.x = 0, .y = 396, .z = 0}, .category = 2},
    {.position = {.x = .34f, .y = .01f, .z = -.18f}, .rotation = {.x = 0, .y = 855, .z = 0}, .category = 2},
    {.position = {.x = -.31f, .y = .01f, .z = .21f}, .rotation = {.x = 0, .y = 50496, .z = 0}, .category = 2},
    {.position = {.x = .03f, .y = .02f, .z = .22f}, .rotation = {.x = 0, .y = 250, .z = 0}, .category = 2},
    {.position = {.x = .29f, .y = .04f, .z = .2f}, .rotation = {.x = 0, .y = 65328, .z = 0}, .category = 2},
    {.position = {.x = -.3f, .y = .01f, .z = .53f}, .rotation = {.x = 0, .y = 166, .z = 0}, .category = 2},
    {.position = {.x = .04f, .y = .01f, .z = .53f}, .rotation = {.x = 0, .y = 65182, .z = 0}, .category = 3},
    {.position = {.x = .33f, .y = .01f, .z = .52f}, .rotation = {.x = 0, .y = 64639, .z = 0}, .category = 3},
    {.position = {.x = .63f, .y = .02f, .z = -.51f}, .rotation = {.x = 0, .y = 64952, .z = 0}, .category = 3},
    {.position = {.x = .96f, .y = .04f, .z = -.6f}, .rotation = {.x = 0, .y = 16646, .z = 0}, .category = 3},
    {.position = {.x = 1.29f, .y = .02f, .z = -.48f}, .rotation = {.x = 0, .y = 64994, .z = 0}, .category = 3},
    {.position = {.x = .63f, .y = .01f, .z = -.11f}, .rotation = {.x = 0, .y = 354, .z = 0}, .category = 3},
    {.position = {.x = .93f, .y = .01f, .z = -.3f}, .rotation = {.x = 0, .y = 64827, .z = 0}, .category = 3},
    {.position = {.x = 1.28f, .y = .01f, .z = -.07f}, .rotation = {.x = 0, .y = 1126, .z = 0}, .category = 3},
    {.position = {.x = .62f, .y = .01f, .z = .27f}, .rotation = {.x = 0, .y = 64765, .z = 0}, .category = 3},
    {.position = {.x = .94f, .y = .04f, .z = .05f}, .rotation = {.x = 0, .y = 64932, .z = 0}, .category = 3},
    {.position = {.x = 1.26f, .y = .02f, .z = .24f}, .rotation = {.x = 0, .y = 16125, .z = 0}, .category = 3},
    {.position = {.x = .61f, .y = .02f, .z = .58f}, .rotation = {.x = 0, .y = 813, .z = 0}, .category = 3},
    {.position = {.x = .93f, .y = .02f, .z = .46f}, .rotation = {.x = 0, .y = 458, .z = 0}, .category = 3},
    {.position = {.x = 1.28f, .y = .01f, .z = .54f}, .rotation = {.x = 0, .y = 354, .z = 0}, .category = 127}};
inline constexpr shen_name bookPutCharacters[]{SHEN_NAME(BoK0), SHEN_NAME(BoK1), SHEN_NAME(BoK2), SHEN_NAME(BoK3), SHEN_NAME(BoK4), shen_name{}};
inline constexpr Vector3 bookPutPositions[]{{.x = 341.17001f, .y = 127.449996f, .z = 237.38f}, {.x = 335.59f, .y = 127.449996f, .z = 228.3f},
    {.x = 337.18f, .y = 127.449996f, .z = 237.48f}, {.x = 330.88f, .y = 127.449996f, .z = 228.29f}, {.x = 333.18f, .y = 127.449996f, .z = 237.5f}};
struct BookPutGroup {
    std::uint8_t group, count;
};
inline constexpr BookPutGroup bookPutGroups[]{{.group = 0, .count = 0}, {.group = 0, .count = 1}, {.group = 0, .count = 2}, {.group = 1, .count = 0},
    {.group = 1, .count = 1}, {.group = 1, .count = 2}, {.group = 2, .count = 0}, {.group = 2, .count = 1}, {.group = 2, .count = 2}, {.group = 3, .count = 0},
    {.group = 3, .count = 1}, {.group = 3, .count = 2}, {.group = 4, .count = 0}, {.group = 4, .count = 1}};
struct BookHeldPlacement {
    Vector3 position;
    shen_name character;
    std::uint32_t type;
};
static_assert(sizeof(BookHeldPlacement) == 0x14);
inline constexpr BookHeldPlacement bookHeldPlacements[]{{.position = {.x = -.01f, .y = .04f, .z = -.01f}, .character = BOOK_BASE, .type = 0},
    {.position = {.x = 0, .y = .04f, .z = 0}, .character = SHEN_NAME(AZOU), .type = 5},
    {.position = {.x = .04f, .y = .04f, .z = .01f}, .character = SHEN_NAME(BZOU), .type = 0},
    {.position = {.x = -0.0f, .y = .02f, .z = .02f}, .character = SHEN_NAME(CZOU), .type = 2},
    {.position = {.x = -.03f, .y = .08f, .z = .03f}, .character = SHEN_NAME(DZOU), .type = 4},
    {.position = {.x = -0.0f, .y = .02f, .z = -.04f}, .character = SHEN_NAME(EZOU), .type = 1},
    {.position = {.x = .02f, .y = .04f, .z = -.02f}, .character = SHEN_NAME(FZOU), .type = 5},
    {.position = {.x = .01f, .y = .04f, .z = .03f}, .character = SHEN_NAME(GZOU), .type = 5},
    {.position = {.x = -.01f, .y = .04f, .z = 0}, .character = SHEN_NAME(HZOU), .type = 0},
    {.position = {.x = -.04f, .y = .02f, .z = .01f}, .character = SHEN_NAME(IZOU), .type = 2},
    {.position = {.x = -.03f, .y = .02f, .z = -.01f}, .character = SHEN_NAME(JZOU), .type = 3},
    {.position = {.x = .02f, .y = .02f, .z = .02f}, .character = SHEN_NAME(KZOU), .type = 1},
    {.position = {.x = .02f, .y = .02f, .z = -.04f}, .character = SHEN_NAME(LZOU), .type = 1},
    {.position = {.x = -.01f, .y = .04f, .z = .01f}, .character = SHEN_NAME(MZOU), .type = 0},
    {.position = {.x = .02f, .y = .02f, .z = -.02f}, .character = SHEN_NAME(NZOU), .type = 3},
    {.position = {.x = -.04f, .y = .02f, .z = .03f}, .character = SHEN_NAME(OZOU), .type = 3},
    {.position = {.x = .03f, .y = .02f, .z = .04f}, .character = SHEN_NAME(PZOU), .type = 1},
    {.position = {.x = -.02f, .y = .02f, .z = -.02f}, .character = SHEN_NAME(QZOU), .type = 2},
    {.position = {.x = .02f, .y = .02f, .z = -.04f}, .character = SHEN_NAME(RZOU), .type = 2},
    {.position = {.x = .02f, .y = .02f, .z = -.04f}, .character = SHEN_NAME(SZOU), .type = 1}};
struct BookModelType {
    const char *model;
    float width;
    std::uint32_t reserved0c;
};
static_assert(sizeof(BookModelType) == 0x10);
inline constexpr BookModelType bookModelTypes[]{{.model = "A3BR4", .width = .04f, .reserved0c = 0}, {.model = "B4RD2", .width = .02f, .reserved0c = 0},
    {.model = "A4BL2", .width = .02f, .reserved0c = 0}, {.model = "A4BR2", .width = .02f, .reserved0c = 0}, {.model = "B5RD8", .width = .08f, .reserved0c = 0},
    {.model = "A5GR4", .width = .04f, .reserved0c = 0}};
struct BookCompanionPlacement {
    Vector3 position;
    std::int32_t yaw;
    std::byte reserved10[16];
};
static_assert(sizeof(BookCompanionPlacement) == 0x20);
inline constexpr BookCompanionPlacement bookCompanionPlacements[]{{.position = {.x = 343, .y = 126.97f, .z = 236.3f}, .yaw = 7281, .reserved10 = {}},
    {.position = {.x = 342.1f, .y = 126.97f, .z = 236.13f}, .yaw = 32768, .reserved10 = {}},
    {.position = {.x = 341.5f, .y = 126.97f, .z = 236.13f}, .yaw = 32768, .reserved10 = {}},
    {.position = {.x = 340.1f, .y = 126.97f, .z = 236.13f}, .yaw = 32768, .reserved10 = {}},
    {.position = {.x = 337.61f, .y = 126.97f, .z = 228.43f}, .yaw = 16384, .reserved10 = {}},
    {.position = {.x = 335.95f, .y = 126.97f, .z = 229.6f}, .yaw = 1274, .reserved10 = {}},
    {.position = {.x = 333.7f, .y = 126.97f, .z = 228.7f}, .yaw = 52429, .reserved10 = {}},
    {.position = {.x = 339.18f, .y = 126.97f, .z = 236.99f}, .yaw = 19114, .reserved10 = {}},
    {.position = {.x = 337.46f, .y = 126.97f, .z = 236.2f}, .yaw = 33860, .reserved10 = {}},
    {.position = {.x = 336.84f, .y = 126.97f, .z = 236.23f}, .yaw = 30219, .reserved10 = {}},
    {.position = {.x = 332.82f, .y = 126.97f, .z = 228.6f}, .yaw = 12743, .reserved10 = {}},
    {.position = {.x = 331.44f, .y = 126.97f, .z = 229.5f}, .yaw = 4369, .reserved10 = {}},
    {.position = {.x = 330, .y = 126.97f, .z = 229.5f}, .yaw = 0, .reserved10 = {}},
    {.position = {.x = 333.87f, .y = 126.97f, .z = 236.33f}, .yaw = 32768, .reserved10 = {}}};
inline std::int32_t bookHandCommand[19][3]{{1999, 65354, 62444}, {1638, 180, 63900}, {546, 64719, 3366}, {0, 64809, 60257}, {0, 64809, 59528}, {0, 0, 62623},
    {0, 0, 64628}, {0, 0, 58983}, {0, 0, 58073}, {0, 65172, 61897}, {0, 0, 65354}, {0, 729, 60074}, {0, 65173, 57163}, {0, 65172, 63171}, {0, 0, 0},
    {0, 547, 56252}, {0, 1639, 60074}, {0, 64809, 61715}, {64080, 182, 65535}};
} // namespace shenmue::s2::areas
