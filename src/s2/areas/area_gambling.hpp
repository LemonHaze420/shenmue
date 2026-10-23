#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/area_event.hpp"
#include "s2/debug.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
struct GamblingFreeBoardPlacement {
    shen_name area;
    Vector3 position;
    float angleX, angleY, angleZ;
    std::int32_t firstDigits, secondDigits;
};
struct GamblingBoardPlacement {
    shen_name area;
    Vector3 position;
    float angleY;
    std::int32_t firstDigits, secondDigits;
};
struct GamblingTablePlacement {
    shen_name area;
    Vector3 position;
    float angleY;
    std::int32_t firstDigits, secondDigits;
    std::int16_t objectKinds[2];
};
static_assert(sizeof(GamblingFreeBoardPlacement) == 36);
static_assert(sizeof(GamblingBoardPlacement) == 28);
static_assert(sizeof(GamblingTablePlacement) == 32);

inline constexpr std::array<GamblingFreeBoardPlacement, 5> aberdeenBS_0{{
    {SHEN_NAME(AKT0), {246.1999969482422f, 107.19999694824219f, 232.5f}, 10.0f, 180.0f, 0.0f, 10, 50},
    {SHEN_NAME(AK00), {271.70001220703125f, 98.0f, 253.6999969482422f}, 10.0f, 90.0f, 0.0f, 100, 500},
    {SHEN_NAME(AKT2), {297.6000061035156f, 107.19999694824219f, 230.3000030517578f}, 355.0f, 90.0f, 345.0f, 50, 200},
    {SHEN_NAME(AK00), {285.6000061035156f, 100.0f, 224.10000610351562f}, 0.0f, 90.0f, 0.0f, 5, 20},
    {shen_name{0x00000000U}, {0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 10, 10},
}};

inline constexpr std::array<GamblingTablePlacement, 15> aberdeenBS{{
    {SHEN_NAME(AKT0), {253.39999389648438f, 106.0999984741211f, 228.6999969482422f}, 270.0f, 10, 50, {1, 0}},
    {SHEN_NAME(AKT1), {267.5f, 96.8499984741211f, 265.20001220703125f}, 90.0f, 100, 500, {5, 0}},
    {SHEN_NAME(AKT2), {301.70001220703125f, 106.0999984741211f, 238.10000610351562f}, 180.0f, 50, 200, {0, 0}},
    {SHEN_NAME(QR00), {118.19999694824219f, 93.0250015258789f, 189.5f}, 160.0f, 10, 100, {6, 0}},
    {SHEN_NAME(QTB2), {251.60000610351562f, 94.9000015258789f, 248.10000610351562f}, 210.0f, 20, 100, {5, 0}},
    {SHEN_NAME(QTB2), {245.1999969482422f, 94.9000015258789f, 238.8000030517578f}, 45.0f, 100, 1000, {2, 0}},
    {SHEN_NAME(QTB2), {244.89999389648438f, 94.9000015258789f, 244.6999969482422f}, 105.0f, 50, 300, {4, 0}},
    {SHEN_NAME(BGSM), {100.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {0, 0}},
    {SHEN_NAME(BGSM), {150.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {1, 0}},
    {SHEN_NAME(BGSM), {200.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {2, 0}},
    {SHEN_NAME(BGSM), {250.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {3, 0}},
    {SHEN_NAME(BGSM), {300.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {4, 0}},
    {SHEN_NAME(BGSM), {350.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {5, 0}},
    {SHEN_NAME(QR09), {118.19999694824219f, 93.0250015258789f, 189.5f}, 160.0f, 10, 100, {6, 0}},
    {shen_name{0x00000000U}, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0, {0, 0}},
}};

inline constexpr std::array<GamblingBoardPlacement, 12> wanChaiBS{{
    {shen_name{0xffffffffU}, {280.739990234375f, 100.02999877929688f, 224.22000122070312f}, 90.0f, 5, 20},
    {SHEN_NAME(WN00), {240.60000610351562f, 104.5f, 160.6999969482422f}, 180.0f, 10, 100},
    {SHEN_NAME(WR00), {178.60000610351562f, 101.30000305175781f, 149.60000610351562f}, 0.0f, 10, 50},
    {SHEN_NAME(WR00), {151.0f, 100.0f, 172.8000030517578f}, 270.0f, 20, 100},
    {SHEN_NAME(WS00), {47.0f, 98.19999694824219f, 145.8000030517578f}, 180.0f, 50, 300},
    {SHEN_NAME(WS00), {52.5f, 98.19999694824219f, 140.8000030517578f}, 270.0f, 100, 500},
    {SHEN_NAME(Q300), {229.60000610351562f, 100.0f, 166.1999969482422f}, 10.0f, 100, 500},
    {SHEN_NAME(QR00), {154.60000610351562f, 100.0f, 193.8000030517578f}, 0.0f, 10, 100},
    {SHEN_NAME(UEDE), {0.0f, 0.0f, 0.0f}, 0.0f, 10, 50},
    {SHEN_NAME(UEDE), {0.0f, 0.0f, 0.0f}, 0.0f, 10, 50},
    {SHEN_NAME(QR09), {154.60000610351562f, 100.0f, 193.8000030517578f}, 0.0f, 10, 100},
    {shen_name{0x00000000U}, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0},
}};

inline constexpr std::array<GamblingBoardPlacement, 8> kowloonBS{{
    {SHEN_NAME(QR00), {124.0f, 93.0f, 191.0f}, 167.5f, 5, 20},
    {SHEN_NAME(QR00), {151.3000030517578f, 100.0f, 193.8000030517578f}, 350.0f, 10, 50},
    {SHEN_NAME(Q300), {225.89999389648438f, 100.0f, 168.8000030517578f}, 94.5999984741211f, 20, 100},
    {SHEN_NAME(QA00), {291.8999938964844f, 110.80000305175781f, 172.39999389648438f}, 0.0f, 10, 100},
    {SHEN_NAME(QA00), {292.0f, 125.19999694824219f, 172.1999969482422f}, 345.0f, 100, 500},
    {SHEN_NAME(QR09), {124.0f, 93.0f, 191.0f}, 167.5f, 5, 20},
    {SHEN_NAME(QR09), {151.3000030517578f, 100.0f, 193.8000030517578f}, 350.0f, 10, 50},
    {shen_name{0x00000000U}, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0},
}};

extern std::uint32_t g_areaBoardCharacterIndex;
void GambleInit();

// @verified - p
inline void SetGamble() {
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
}
void LoadBoardSet(shen_name parent, float x, float y, float z, float angleX, float angleY, float angleZ, std::int32_t firstDigits, std::int32_t secondDigits);
void SetupObjects(std::int16_t index);
bool IsNotInGamblingHouse();
void PlaceGamblingBoardDigits(shen_name parent, std::int32_t digits, std::int16_t axis);
} // namespace shenmue::s2
