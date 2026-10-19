#include "s2/areas/area_qlgt.hpp"
namespace shenmue::s2::areas::qlgt {
const Entry entries[4]{{0, 391}, {1, 392}, {2, 393}, {3, 394}};
shen_name actors[9]{ryo, ren, invalid, invalid, invalid, invalid, invalid, invalid, invalid};
const shen_name doors[25]{SHEN_NAME(R111), SHEN_NAME(R112), SHEN_NAME(R113), SHEN_NAME(R103), SHEN_NAME(R110), SHEN_NAME(R109), SHEN_NAME(R115),
    SHEN_NAME(R114), SHEN_NAME(R101), SHEN_NAME(R102), SHEN_NAME(R104), SHEN_NAME(R105), SHEN_NAME(R106), SHEN_NAME(R107), SHEN_NAME(R108), SHEN_NAME(R116),
    SHEN_NAME(R117), SHEN_NAME(R118), SHEN_NAME(R119), SHEN_NAME(R123), SHEN_NAME(R122), SHEN_NAME(R121), SHEN_NAME(R120), SHEN_NAME(R124), SHEN_NAME(R140)};
const shen_name hiddenDoors[4]{SHEN_NAME(DR00), SHEN_NAME(DR01), SHEN_NAME(DR02), SHEN_NAME(DR03)};
const shen_name lightDefinitions[4]{SHEN_NAME(0001), SHEN_NAME(0002), SHEN_NAME(0003), SHEN_NAME(0004)};
const std::int32_t lightSlots[4]{1, 4, 6, 8};
const std::uint16_t idleMotions[8]{0x1d9c, 0x1dac, 0x1d9e, 0x1dac, 0x1dad, 0x1db0, 0x1daf, 0x1d9d};
const std::uint16_t battleMotions[10]{0x1d99, 0x1db1, 0x1d9a, 0x1d99, 0x1db1, 0x1d9a, 0x1db3, 0x1db4, 0x1d9b, 0x1db2};
const std::int32_t ordinaryStyles[6]{5, 4, 3, 2, 2, 2}, ordinaryHealth[6]{120, 50, 50, 50, 50, 50};
const std::int32_t advancedStyles[4]{37, 36, 35, 38}, advancedHealth[4]{120, 90, 80, 100};
const EventMovementMotions routeMotions{0xf002, 0xf002, 0xf002, 0xf002, 0x1d92, 0xf002};
const EventChaseWork chaseDefaults{.source = ren,
    .target = renDummy,
    .delay = 0,
    .positionGain = {.08f, .08f, .08f},
    .rotationGain = {0, 0, 0},
    .positionGainAmplitude = .01f,
    .rotationGainAmplitude = .01f,
    .positionGainFrequency = 0,
    .rotationGainFrequency = 0,
    .positionDeadzone = {.1f, .1f, .1f},
    .rotationDeadzone = {0, 0, 0},
    .positionDeadzoneAmplitude = .1f,
    .rotationDeadzoneAmplitude = 0,
    .positionDeadzoneFrequency = 6,
    .rotationDeadzoneFrequency = 1,
    .ticks = 0,
    .readIndex = 0,
    .writeIndex = 0};
namespace {
const FlashPattern patterns[]{{2, {1, 2, 0, 1, 2, 0, 0, 1, 2, 2, 0, 0, 1, 2, 2, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {3, {1, 2, 1, 2, 0, 0, 1, 2, 2, 0, 1, 2, 0, 0, 1, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {3, {1, 2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 2, 1, 0, 1, 2}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
    {2, {1, 2, 2, 0, 1, 1, 2, 0, 0, 1, 2, 0, 1, 2, 2, 1}, {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}}};
const shen_name enemiesFirst[]{SHEN_NAME(KD6_), SHEN_NAME(THG_), SHEN_NAME(KT3_)};
const shen_name enemiesSecond[]{SHEN_NAME(CHR_), SHEN_NAME(SOY_), SHEN_NAME(KT5_), SHEN_NAME(CJU_)};
const shen_name enemiesThird[]{SHEN_NAME(KU2_), SHEN_NAME(AD3_), SHEN_NAME(WTC_), SHEN_NAME(KN3_)};
const shen_name enemiesFourth[]{SHEN_NAME(MON_), SHEN_NAME(SD7_), SHEN_NAME(WD4_), SHEN_NAME(AD4_)};
const SafeZone safeFirst[]{{1, {237, 121.6f, 147.7f}, 2.7f}, {1, {229, 121.6f, 147.9f}, 2.5f}, {0, {0, 0, 0}, 0}};
const SafeZone safeSecond[]{{1, {210, 121.6f, 171.5f}, 3.2f}, {0, {0, 0, 0}, 0}};
const SafeZone safeThird[]{{1, {199.4f, 121.6f, 156.8f}, 1.8f}, {1, {199.3f, 121.6f, 151.2f}, 1.8f}, {0, {0, 0, 0}, 0}};
const SafeZone safeFourth[]{{1, {245, 121.6f, 125.5f}, 2.2f}, {0, {0, 0, 0}, 0}};
const EventMovementWaypoint routeFirst[]{
    {{239.6f, 121.6f, 148.2f}, 90}, {{235.5f, 121.6f, 148.2f}, 90}, {{224.6f, 121.6f, 148.2f}, 180}, {{224.6f, 121.6f, 150.2f}, 90}, {{-1, -1, -1}, 0}};
const EventMovementWaypoint routeSecond[]{{{219.6f, 121.6f, 170.8f}, 90}, {{210, 121.6f, 170.8f}, 180}, {{209.9f, 121.6f, 172}, 180},
    {{208.9f, 121.6f, 177.8f}, 90}, {{206.7f, 121.6f, 177.8f}, 180}, {{-1, -1, -1}, 0}};
const EventMovementWaypoint routeThird[]{{{199.4f, 121.6f, 159.7f}, 0}, {{199.4f, 121.6f, 156.4f}, 0}, {{199.9f, 121.6f, 152.9f}, 0},
    {{200.2f, 121.6f, 150.5f}, 0}, {{200.3f, 121.6f, 148.5f}, 270}, {{202, 121.6f, 148.2f}, 0}, {{-1, -1, -1}, 0}};
const EventMovementWaypoint routeFourth[]{
    {{240.9f, 121.6f, 124.7f}, 225}, {{242.7f, 121.6f, 126.4f}, 315}, {{247.5f, 121.6f, 125.1f}, 180}, {{247.6f, 121.6f, 128.6f}, 270}, {{-1, -1, -1}, 0}};
const std::uint8_t actionsFirst[]{0, 0, 1, 0, 2}, actionsSecond[]{0, 0, 0, 1, 0, 2}, actionsThird[]{0, 0, 1, 0, 1, 0, 2}, actionsFourth[]{0, 0, 1, 0, 2};
} // namespace
const Course courses[4]{
    {&patterns[0], {222.2f, 121.6f, 134.9f}, {251.7f, 121.6f, 154.4f}, {{244.3f, 121.6f, 146.3f}, 180}, 1.5f, {{225.5f, 121.6f, 150}, 90}, 3.3f,
        {224.2f, 121.6f, 151.5f}, 2.2f, {{234, 121.6f, 142}, 0}, 11, {{233.1f, 121.6f, 146.7f}, 0}, {239.5f, 121.6f, 139}, 5, 3, 50, {233.5f, 121.6f, 140.5f},
        5, {{222.3f, 121.6f, 155}, 165}, {{223.2f, 121.6f, 154.5f}, 115}, {{249, 121.6f, 152}, 0}, 0, enemiesFirst, safeFirst, routeFirst, actionsFirst},
    {&patterns[1], {201, 121.6f, 164.1f}, {226, 121.6f, 188.1f}, {{221.8f, 121.6f, 168.7f}, 90}, 1.5f, {{207.5f, 121.6f, 176.8f}, 180}, 3.3f,
        {208, 121.6f, 179}, 2.8f, {{218, 121.6f, 179.5f}, 0}, 10.5f, {{214.2f, 121.6f, 170.9f}, 0}, {223.3f, 121.6f, 176.8f}, 5, 4, 50, {218, 121.6f, 174.4f},
        5, {{201.9f, 121.6f, 178.4f}, 70}, {{201.9f, 121.6f, 176.9f}, 175}, {{220.5f, 121.6f, 166.7f}, 180}, 0, enemiesSecond, safeSecond, routeSecond,
        actionsSecond},
    {&patterns[2], {185, 121.6f, 144.6f}, {207.5f, 121.6f, 165.4f}, {{197.5f, 121.6f, 160.6f}, 270}, 1.5f, {{201, 121.6f, 148.9f}, 0}, 3.3f,
        {202.5f, 121.6f, 147.5f}, 2.2f, {{194, 121.6f, 150.5f}, 90}, 10, {{198, 121.6f, 154.3f}, 90}, {190, 121.6f, 157}, 4, 4, 50, {192, 121.6f, 151.8f}, 5,
        {{206.4f, 121.6f, 147.5f}, 270}, {{207.1f, 121.6f, 148.7f}, 320}, {{198.5f, 121.6f, 162.5f}, 0}, 0, enemiesThird, safeThird, routeThird, actionsThird},
    {&patterns[3], {231.4f, 121.6f, 115.6f}, {249.4f, 121.6f, 131.9f}, {{236.2f, 121.6f, 124.9f}, 180}, 1.5f, {{246.2f, 121.6f, 128.4f}, 270}, 3.3f,
        {247.5f, 121.6f, 128.4f}, 2.2f, {{243, 121.6f, 122.4f}, 0}, 7, {{239.4f, 121.6f, 124.3f}, 0}, {240, 121.6f, 120}, 4, 4, 50, {245, 121.6f, 120}, 4,
        {{247.8f, 121.6f, 132}, 180}, {{246.4f, 121.6f, 132.2f}, 230}, {{235, 121.6f, 126}, 270}, 0, enemiesFourth, safeFourth, routeFourth, actionsFourth}};
} // namespace shenmue::s2::areas::qlgt
