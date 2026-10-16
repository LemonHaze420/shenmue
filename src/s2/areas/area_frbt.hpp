#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include <cstddef>
namespace shenmue::s2::areas {
inline constexpr shen_name FRBT_AREA{SHEN_NAME(FRBT)}, FRBT_TASK{SHEN_NAME(frbt)}, FRBT_WAIT_TASK{SHEN_NAME(wait)};
inline constexpr shen_name FRBT_RYO{SHEN_NAME(RYO_)}, FRBT_BASE{SHEN_NAME(BASE)}, FRBT_MAP_PROP{SHEN_NAME(map_)};
inline constexpr shen_name FRBT_AB00{SHEN_NAME(AB00)}, FRBT_BATTLE_GRID{SHEN_NAME(BTLF)}, FRBT_DEFAULT_GRID{SHEN_NAME(0000)};
inline constexpr shen_name FRBT_INVALID{0xffffffffU}, FRBT_EMPTY{0};
inline constexpr shen_name FRBT_JUK_{SHEN_NAME(JUK_)};
inline constexpr shen_name FRBT_H08_{SHEN_NAME(H08_)};
inline constexpr shen_name FRBT_H10Q{SHEN_NAME(H10Q)};
inline constexpr shen_name FRBT_H16Q{SHEN_NAME(H16Q)};
inline constexpr shen_name FRBT_H19Q{SHEN_NAME(H19Q)};
inline constexpr shen_name FRBT_UNH_{SHEN_NAME(UNH_)};
inline constexpr shen_name FRBT_H09Q{SHEN_NAME(H09Q)};
inline constexpr shen_name FRBT_H15Q{SHEN_NAME(H15Q)};
inline constexpr shen_name FRBT_SYJ_{SHEN_NAME(SYJ_)};
inline constexpr shen_name FRBT_H03Q{SHEN_NAME(H03Q)};
inline constexpr shen_name FRBT_H04_{SHEN_NAME(H04_)};
inline constexpr shen_name FRBT_H07Q{SHEN_NAME(H07Q)};
inline constexpr shen_name FRBT_H19_{SHEN_NAME(H19_)};
inline constexpr shen_name FRBT_ENF_{SHEN_NAME(ENF_)};
inline constexpr shen_name FRBT_H11Q{SHEN_NAME(H11Q)};
inline constexpr shen_name FRBT_H17Q{SHEN_NAME(H17Q)};
inline constexpr shen_name FRBT_H20Q{SHEN_NAME(H20Q)};
inline constexpr shen_name FRBT_AGO_{SHEN_NAME(AGO_)};
inline constexpr shen_name FRBT_H01_{SHEN_NAME(H01_)};
inline constexpr shen_name FRBT_H10_{SHEN_NAME(H10_)};
inline constexpr shen_name FRBT_H13_{SHEN_NAME(H13_)};
inline constexpr shen_name FRBT_H14_{SHEN_NAME(H14_)};
inline constexpr shen_name FRBT_TYZ_{SHEN_NAME(TYZ_)};
inline constexpr shen_name FRBT_H05Q{SHEN_NAME(H05Q)};
inline constexpr shen_name FRBT_H06Q{SHEN_NAME(H06Q)};
inline constexpr shen_name FRBT_H12_{SHEN_NAME(H12_)};
inline constexpr shen_name FRBT_SNC_{SHEN_NAME(SNC_)};
inline constexpr shen_name FRBT_H04Q{SHEN_NAME(H04Q)};
inline constexpr shen_name FRBT_H11_{SHEN_NAME(H11_)};
inline constexpr shen_name FRBT_HV1_{SHEN_NAME(HV1_)};
inline constexpr shen_name FRBT_HV2_{SHEN_NAME(HV2_)};
inline constexpr shen_name FRBT_HV3_{SHEN_NAME(HV3_)};
inline constexpr shen_name FRBT_HV4_{SHEN_NAME(HV4_)};
inline constexpr shen_name FRBT_HV8_{SHEN_NAME(HV8_)};
inline constexpr shen_name FRBT_HV5_{SHEN_NAME(HV5_)};
inline constexpr shen_name FRBT_HV9_{SHEN_NAME(HV9_)};
inline constexpr shen_name FRBT_HV6_{SHEN_NAME(HV6_)};
inline constexpr shen_name FRBT_HV7_{SHEN_NAME(HV7_)};
inline constexpr shen_name FRBT_KEI_{SHEN_NAME(KEI_)};
inline constexpr shen_name FRBT_HVA_{SHEN_NAME(HVA_)};
inline constexpr shen_name FRBT_YOH_{SHEN_NAME(YOH_)};
inline constexpr shen_name FRBT_RIK_{SHEN_NAME(RIK_)};
inline constexpr shen_name FRBT_KHO_{SHEN_NAME(KHO_)};
inline constexpr shen_name FRBT_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name FRBT_SNN_{SHEN_NAME(SNN_)};
inline constexpr shen_name FRBT_FUY_{SHEN_NAME(FUY_)};
struct FrbtWork {
    HLib::Task *task;
    EventReadCharactersWork *characters;
    std::uint32_t reserved10, reserved14;
    shen_name enemies[6];
    std::int32_t motion, phase, enemyCount, outcome;
};
static_assert(sizeof(FrbtWork) == 0x40 && offsetof(FrbtWork, enemies) == 0x18 && offsetof(FrbtWork, motion) == 0x30);
struct FrbtVoiceWork {
    HLib::Task *parent;
    std::int32_t delay, index;
    std::uint32_t reserved10, elapsed, flags, reserved1c;
};
static_assert(sizeof(FrbtVoiceWork) == 0x20 && offsetof(FrbtVoiceWork, elapsed) == 0x14 && offsetof(FrbtVoiceWork, flags) == 0x18);
struct FrbtOutcomeRecord {
    std::int32_t flag, won, lost, advanceTime;
    const char *voicePrefix;
};
static_assert(sizeof(FrbtOutcomeRecord) == 0x18);
struct FrbtPlacement {
    Vector3 position;
    float yaw;
};
static_assert(sizeof(FrbtPlacement) == 0x10);
inline constexpr FrbtOutcomeRecord FRBT_OUTCOMES[]{
    {.flag = 171, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052F"},
    {.flag = 172, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052B"},
    {.flag = 173, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052L"},
    {.flag = 192, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052D"},
    {.flag = 193, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 194, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052J"},
    {.flag = 195, .won = 0, .lost = 1, .advanceTime = 1, .voicePrefix = "0052H"},
    {.flag = 464, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 465, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 466, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 467, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 468, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 469, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 470, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 471, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 164, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 166, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
    {.flag = 165, .won = 0, .lost = 0, .advanceTime = 1, .voicePrefix = "0052A"},
};
inline constexpr shen_name FRBT_ENEMIES[18][6]{
    {FRBT_JUK_, FRBT_H08_, FRBT_H10Q, FRBT_H16Q, FRBT_H19Q, FRBT_INVALID},
    {FRBT_UNH_, FRBT_H09Q, FRBT_H15Q, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_SYJ_, FRBT_H03Q, FRBT_H04_, FRBT_H07Q, FRBT_H19_, FRBT_INVALID},
    {FRBT_ENF_, FRBT_H11Q, FRBT_H17Q, FRBT_H20Q, FRBT_INVALID, FRBT_EMPTY},
    {FRBT_AGO_, FRBT_H01_, FRBT_H10_, FRBT_H13_, FRBT_H14_, FRBT_INVALID},
    {FRBT_TYZ_, FRBT_H05Q, FRBT_H06Q, FRBT_H12_, FRBT_INVALID, FRBT_EMPTY},
    {FRBT_SNC_, FRBT_H04Q, FRBT_H11_, FRBT_H14_, FRBT_INVALID, FRBT_EMPTY},
    {FRBT_HV1_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV2_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV3_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV4_, FRBT_HV8_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV5_, FRBT_HV9_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV6_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HV7_, FRBT_KEI_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_HVA_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_YOH_, FRBT_RIK_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_KHO_, FRBT_SEE_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
    {FRBT_SNN_, FRBT_FUY_, FRBT_INVALID, FRBT_EMPTY, FRBT_EMPTY, FRBT_EMPTY},
};
inline constexpr FrbtPlacement FRBT_POSITIONS[]{
    {.position = {.x = 243.5f, .y = 96.8499984741211f, .z = 492.20001220703125f}, .yaw = 90.0f},
    {.position = {.x = 242.39999389648438f, .y = 96.8499984741211f, .z = 491.0f}, .yaw = 235.0f},
    {.position = {.x = 244.39999389648438f, .y = 96.8499984741211f, .z = 491.0f}, .yaw = 135.0f},
    {.position = {.x = 243.6999969482422f, .y = 96.8499984741211f, .z = 490.29998779296875f}, .yaw = 135.0f},
    {.position = {.x = 242.60000610351562f, .y = 96.8499984741211f, .z = 490.29998779296875f}, .yaw = 190.0f},
    {.position = {.x = 276.8999938964844f, .y = 96.8499984741211f, .z = 496.20001220703125f}, .yaw = 305.0f},
    {.position = {.x = 278.70001220703125f, .y = 96.8499984741211f, .z = 496.29998779296875f}, .yaw = 105.0f},
    {.position = {.x = 278.3999938964844f, .y = 96.8499984741211f, .z = 495.1000061035156f}, .yaw = 125.0f},
    {.position = {.x = 277.0f, .y = 96.8499984741211f, .z = 494.6000061035156f}, .yaw = 155.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 242.8000030517578f, .y = 96.8499984741211f, .z = 526.2999877929688f}, .yaw = 270.0f},
    {.position = {.x = 244.1999969482422f, .y = 96.8499984741211f, .z = 527.7000122070312f}, .yaw = 65.0f},
    {.position = {.x = 243.6999969482422f, .y = 96.8499984741211f, .z = 507.70001220703125f}, .yaw = 50.0f},
    {.position = {.x = 244.39999389648438f, .y = 96.8499984741211f, .z = 525.5999755859375f}, .yaw = 120.0f},
    {.position = {.x = 243.6999969482422f, .y = 96.8499984741211f, .z = 525.2999877929688f}, .yaw = 120.0f},
    {.position = {.x = 250.5f, .y = 96.8499984741211f, .z = 577.0f}, .yaw = 210.0f},
    {.position = {.x = 251.89999389648438f, .y = 96.8499984741211f, .z = 578.7999877929688f}, .yaw = 30.0f},
    {.position = {.x = 252.10000610351562f, .y = 96.8499984741211f, .z = 578.0999755859375f}, .yaw = 45.0f},
    {.position = {.x = 250.6999969482422f, .y = 96.8499984741211f, .z = 579.5f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 250.6999969482422f, .y = 96.8499984741211f, .z = 579.5f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 195.3000030517578f, .y = 96.8499984741211f, .z = 526.5999755859375f}, .yaw = 90.0f},
    {.position = {.x = 194.0f, .y = 96.8499984741211f, .z = 527.2999877929688f}, .yaw = 290.0f},
    {.position = {.x = 193.6999969482422f, .y = 96.8499984741211f, .z = 525.4000244140625f}, .yaw = 250.0f},
    {.position = {.x = 194.5f, .y = 96.8499984741211f, .z = 524.7000122070312f}, .yaw = 210.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 197.60000610351562f, .y = 96.8499984741211f, .z = 569.7000122070312f}, .yaw = 225.0f},
    {.position = {.x = 198.89999389648438f, .y = 96.8499984741211f, .z = 570.0999755859375f}, .yaw = 65.0f},
    {.position = {.x = 198.39999389648438f, .y = 96.8499984741211f, .z = 571.0f}, .yaw = 45.0f},
    {.position = {.x = 196.39999389648438f, .y = 96.8499984741211f, .z = 571.2000122070312f}, .yaw = 350.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
};
inline constexpr FrbtPlacement FRBT_BASES[]{
    {.position = {.x = 245.1999969482422f, .y = 96.8499984741211f, .z = 492.8999938964844f}, .yaw = 90.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 275.6000061035156f, .y = 96.8499984741211f, .z = 496.79998779296875f}, .yaw = 180.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 240.60000610351562f, .y = 96.8499984741211f, .z = 524.0f}, .yaw = 180.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 250.0f, .y = 96.8499984741211f, .z = 574.7000122070312f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 221.0f, .y = 96.8499984741211f, .z = 549.9000244140625f}, .yaw = 270.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 196.1999969482422f, .y = 96.8499984741211f, .z = 532.9000244140625f}, .yaw = 180.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 197.1999969482422f, .y = 96.8499984741211f, .z = 568.7999877929688f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 303.20001220703125f, .y = 96.8499984741211f, .z = 505.29998779296875f}, .yaw = 247.5f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 288.70001220703125f, .y = 95.30000305175781f, .z = 555.2999877929688f}, .yaw = 180.0f},
    {.position = {.x = 288.8999938964844f, .y = 95.30000305175781f, .z = 561.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 284.6000061035156f, .y = 95.30000305175781f, .z = 550.0f}, .yaw = 90.0f},
    {.position = {.x = 280.70001220703125f, .y = 95.30000305175781f, .z = 550.4000244140625f}, .yaw = 270.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 279.3999938964844f, .y = 95.30000305175781f, .z = 569.9000244140625f}, .yaw = 90.0f},
    {.position = {.x = 264.70001220703125f, .y = 95.30000305175781f, .z = 568.5999755859375f}, .yaw = 270.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 250.6999969482422f, .y = 96.8499984741211f, .z = 566.5999755859375f}, .yaw = 0.0f},
    {.position = {.x = 253.39999389648438f, .y = 96.8499984741211f, .z = 552.0f}, .yaw = 160.0f},
    {.position = {.x = 247.5f, .y = 96.8499984741211f, .z = 551.7999877929688f}, .yaw = 200.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 200.6999969482422f, .y = 96.8499984741211f, .z = 492.5f}, .yaw = 67.5f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 201.6999969482422f, .y = 96.8499984741211f, .z = 548.2999877929688f}, .yaw = 225.0f},
    {.position = {.x = 201.0f, .y = 96.8499984741211f, .z = 553.5f}, .yaw = 292.5f},
    {.position = {.x = 207.5f, .y = 96.8499984741211f, .z = 548.5999755859375f}, .yaw = 112.5f},
    {.position = {.x = 206.8000030517578f, .y = 96.8499984741211f, .z = 554.5999755859375f}, .yaw = 45.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 178.6999969482422f, .y = 96.8499984741211f, .z = 572.2999877929688f}, .yaw = 180.0f},
    {.position = {.x = 0.0f, .y = -100.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
    {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .yaw = 0.0f},
};
inline constexpr const char *FRBT_LOSE_VOICES[]{"S2BVE005", "S2BVE011", "S2BVE012", "S2BVE014", "S2BVE018", "S2BVF018", "S2BVF005", "S2BVF014"}; // @text S2BVE005 EN: "Die!" | JA: "死ねっ！" ; S2BVE011 EN: "Acting so tough!" | JA: "調子にのりやがって！" ; S2BVE012 EN: "You're not going any further!" | JA: "これ以上、行かせるか！" ; S2BVE014 EN: "You cocky kid!" | JA: "ふざけやがって！" ; S2BVE018 EN: "Don't try to make a fool out of me!" | JA: "ナメてんじゃねえぞ！" ; S2BVF018 EN: "Don't try to make a fool out of me!" | JA: "ナメてんじゃねえぞ！" ; S2BVF005 EN: "Die!" | JA: "死ねっ！" ; S2BVF014 EN: "You cocky kid!" | JA: "ふざけやがって！"
inline constexpr const char *FRBT_START_VOICES[]{"S2BVE010", "S2BVE012", "S2BVF017", "S2BVE014", "S2BVE017", "S2BVE019", "S2BVF010", "S2BVF012"}; // @text S2BVE010 EN: "You!" | JA: "この野郎！" ; S2BVE012 EN: "You're not going any further!" | JA: "これ以上、行かせるか！" ; S2BVF017 EN: "Damn kid!" | JA: "このガキぃっ！" ; S2BVE014 EN: "You cocky kid!" | JA: "ふざけやがって！" ; S2BVE017 EN: "Damn kid!" | JA: "このガキぃっ！" ; S2BVE019 EN: "Drop dead!" | JA: "くたばりやがれっ！" ; S2BVF010 EN: "You!" | JA: "この野郎！" ; S2BVF012 EN: "You're not going any further!" | JA: "これ以上、行かせるか！"
inline constexpr const char *FRBT_EXIT_VOICES[]{"S2BVE011", "S2BVE014", "S2BVF018", "S2BVE018", "S2BVE019", "S2BVF010", "S2BVF011", "S2BVF014"}; // @text S2BVE011 EN: "Acting so tough!" | JA: "調子にのりやがって！" ; S2BVE014 EN: "You cocky kid!" | JA: "ふざけやがって！" ; S2BVF018 EN: "Don't try to make a fool out of me!" | JA: "ナメてんじゃねえぞ！" ; S2BVE018 EN: "Don't try to make a fool out of me!" | JA: "ナメてんじゃねえぞ！" ; S2BVE019 EN: "Drop dead!" | JA: "くたばりやがれっ！" ; S2BVF010 EN: "You!" | JA: "この野郎！" ; S2BVF011 EN: "Acting so tough!" | JA: "調子にのりやがって！" ; S2BVF014 EN: "You cocky kid!" | JA: "ふざけやがって！"
inline constexpr Vector3 FRBT_START_ENEMY_POSITIONS[]{{.x = .1f, .y = 0, .z = -5}, {.x = 1.5f, .y = 0, .z = -5.4f}, {.x = -.6f, .y = 0, .z = -6.5f}};
inline std::int32_t frbtMapHandPose[57]{0, 0, 60074, 0, 546, 0, 0, 64444, 2364, 0, 64990, 55344, 0, 0, 59893, 0, 0, 56435, 0, 65354, 726, 0, 0, 56435, 0, 0,
    53158, 65354, 65172, 51157, 0, 65354, 1456, 0, 727, 54069, 65172, 182, 51522, 65354, 64445, 51339, 0, 182, 1273, 1273, 729, 51522, 908, 64809, 51522, 0,
    64263, 50427, 182, 182, 726};
inline constexpr Vector3 FRBT_WIN_ENEMY_POSITION{.x = .1f, .y = 0, .z = -.88f}, FRBT_MAP_PLAYER_POSITION{.x = -.06f, .y = 0, .z = 2.6f};
struct FrbtEnemyDefinition {
    shen_name character;
    std::int32_t health, style;
};
inline constexpr shen_name FRBT_TGI{SHEN_NAME(TGI_)};
inline constexpr FrbtEnemyDefinition FRBT_ENEMY_DEFINITIONS[]{{.character = FRBT_UNH_, .health = 100, .style = 2},
    {.character = FRBT_SYJ_, .health = 100, .style = 2}, {.character = FRBT_AGO_, .health = 100, .style = 2},
    {.character = FRBT_JUK_, .health = 100, .style = 2}, {.character = FRBT_ENF_, .health = 100, .style = 2},
    {.character = FRBT_KEI_, .health = 100, .style = 2}, {.character = FRBT_TYZ_, .health = 100, .style = 2},
    {.character = FRBT_TGI, .health = 100, .style = 2}, {.character = FRBT_FUY_, .health = 100, .style = 2},
    {.character = FRBT_SEE_, .health = 100, .style = 2}, {.character = FRBT_YOH_, .health = 100, .style = 2},
    {.character = FRBT_KHO_, .health = 100, .style = 2}, {.character = FRBT_RIK_, .health = 100, .style = 2},
    {.character = FRBT_SNN_, .health = 100, .style = 2}, {.character = FRBT_EMPTY, .health = 100, .style = 2}};
extern std::uint32_t frbtVariant;
extern std::int32_t frbtResponse, frbtVoiceCount;
extern char frbtVoicePrefixes[64], frbtVoiceSequence[16][12];
void FrbtPositionBase();
void FrbtFacePlayer(shen_name character);
void FrbtResetEnemyMotion(shen_name character);
std::int32_t FrbtStartAuthor(FrbtWork &work);
std::int32_t FrbtWinMessage(FrbtWork &work);
std::int32_t FrbtRunAway(FrbtWork &work);
void FrbtPlayPrefixVoice(std::int32_t prefix, const char *suffix, std::int32_t delay);
void FrbtVictoryAuthorCallback(void *data);
void FrbtMapGetCallback(void *data);
void FrbtVoiceWaitCallback(void *data);
void FrbtStopCallback(void *data);
void InitializeFrbtEvent();
void RunFrbtEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition frbtScriptDefinition;
} // namespace shenmue::s2::areas
