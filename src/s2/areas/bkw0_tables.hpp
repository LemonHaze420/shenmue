#pragma once
#include "s2/areas/area_bkw0.hpp"
namespace shenmue::s2::areas {
inline constexpr Vector3 BKW0_RYO_POSITION{109.86f, 100.0f, 203.23f}, BKW0_LKN_POSITION{109.93f, 100.0f, 205.12f};
inline constexpr Vector3 BKW0_BKN_POSITION{112.1f, 100.0f, 204.6f}, BKW0_RYO_INITIAL_POSITION{109.86f, 100.0f, 203.73f};
inline constexpr Vector3 BKW0_BOW_MYS_POSITION{0.3119f, 0.13f, -0.9995f}, BKW0_BOW_BASE_POSITION{97.17f, 99.9f, 204.23f};
inline constexpr Vector3i BKW0_BOW_BASE_ROTATION{0, 46239, 0};
inline constexpr Vector3 BKW0_CROWD_POSITIONS[]{{110, 100, 199.5f}, {108, 100, 199.5f}, {106, 100, 199.5f}, {104, 100, 199.5f}, {102, 100, 199.5f},
    {109, 100, 198.1f}, {107, 100, 198.1f}, {105, 100, 198.1f}, {103, 100, 198.1f}, {110.4f, 100, 209.3f}, {108.4f, 100, 209.3f}, {106.4f, 100, 209.3f},
    {104.4f, 100, 209.3f}, {102.4f, 100, 209.3f}, {109.4f, 100, 210.7f}, {107.4f, 100, 210.7f}, {105.4f, 100, 210.7f}, {103.4f, 100, 210.7f}};
inline constexpr Vector3 BKW0_SMOKE_POSITIONS[]{{330.64f, 128.05f, 268}, {341.21f, 128.05f, 268}};
inline constexpr shen_name BKW0_S01{SHEN_NAME(S01_)}, BKW0_S02{SHEN_NAME(S02_)}, BKW0_K04{SHEN_NAME(K04_)};
inline constexpr shen_name BKW0_AUTHOR_CHARACTERS[]{BKW0_LKN, BKW0_BKN, BKW0_MYS, BKW0_S01, BKW0_S02, EVENT_CHARACTER_INVALID};
inline Vector3 bkw0BasePosition{}, bkw0BowRyoPosition{};
inline Vector3i bkw0BaseRotation{}, bkw0BowRyoRotation{}, bkw0BowMysRotation{};
inline constexpr std::uint8_t BKW0_PRACTICE_LCD[200]{0x06, 0x20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xc0, 0x03, 0xf8, 0, 0, 0x80,
    0x20, 0x02, 0x08, 0, 0, 0x80, 0x10, 0x02, 0x08, 0, 0, 0x80, 0x08, 0x02, 0x08, 0, 0, 0x8f, 0x88, 0x02, 0x08, 0, 0, 0x88, 0x88, 0x3e, 0x0f, 0x80, 0, 0x8f,
    0x88, 0x20, 0xe0, 0x80, 0, 0x80, 0x10, 0x21, 0x10, 0x80, 0, 0x80, 0x20, 0x21, 0x10, 0x80, 0, 0x80, 0x10, 0x21, 0x10, 0x80, 0, 0x8f, 0x88, 0x20, 0xe0, 0x80,
    0, 0x88, 0x88, 0x3e, 0x0f, 0x80, 0, 0x8f, 0x88, 0x02, 0x08, 0, 0, 0x80, 0x08, 0x02, 0x08, 0, 0, 0x80, 0x10, 0x02, 0x08, 0, 0, 0x80, 0x20, 0x02, 0x08, 0, 0,
    0xff, 0xc0, 0x03, 0xf8, 0};
} // namespace shenmue::s2::areas
