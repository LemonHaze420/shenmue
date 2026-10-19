#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_movement.hpp"
#include "s2/qt_nj_obstacles.hpp"
#include <cstddef>
namespace shenmue::s2 {
inline constexpr shen_name QTNJ_AREA{SHEN_NAME(QTNJ)}, QTNJ_TASK{SHEN_NAME(EVNT)}, QTNJ_INVALID{0xffffffffU};
inline constexpr shen_name QTNJ_RYO{SHEN_NAME(RYO_)}, QTNJ_REN{SHEN_NAME(REN_)}, QTNJ_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QTNJ_OBSTACLE_BASE{SHEN_NAME(TNJB)}, QTNJ_DUMMY_RIGHT{SHEN_NAME(DUMR)}, QTNJ_DUMMY_LEFT{SHEN_NAME(DUML)};
inline constexpr shen_name QTNJ_AUTH_START{SHEN_NAME(ATH0)}, QTNJ_AUTH_LAST_FAILURE{SHEN_NAME(ATH5)};
inline constexpr shen_name QTNJ_AUTH_SUCCESS{SHEN_NAME(ATH6)}, QTNJ_AUTH_FAILURE{SHEN_NAME(ATH7)};
struct QtnjTrial {
    shen_name obstacle;
    std::int32_t approachState, escapeState, successState;
    Vector3 position;
    float yawDegrees;
    const shen_name *characters;
    shen_name author;
    std::uint16_t buttons[4];
    std::int16_t maximumFrames, requiredCommands;
};
struct QtnjEventWork {
    EventReadCharactersWork *admission;
    HLib::Task *task;
    HLib::TaskCallback continuation;
    QtnjObstacleWork obstacle;
    std::int32_t success;
    EventChaseWork chase;
    std::int32_t movementSlot, state, trial, cooldown;
    const std::int32_t *selection;
    const QtnjTrial *trialRecord;
    EventNpcReadWork *npcReader;
    float baseHeight;
    std::int32_t restart;
};
static_assert(sizeof(QtnjTrial) == 0x38);
static_assert(offsetof(QtnjTrial, characters) == 0x20);
static_assert(offsetof(QtnjTrial, buttons) == 0x2c);
static_assert(sizeof(QtnjEventWork) == 0x3c8);
static_assert(offsetof(QtnjEventWork, obstacle) == 0x18);
static_assert(offsetof(QtnjEventWork, success) == 0x30);
static_assert(offsetof(QtnjEventWork, chase) == 0x34);
static_assert(offsetof(QtnjEventWork, movementSlot) == 0x398);
static_assert(offsetof(QtnjEventWork, state) == 0x39c);
static_assert(offsetof(QtnjEventWork, trial) == 0x3a0);
static_assert(offsetof(QtnjEventWork, selection) == 0x3a8);
static_assert(offsetof(QtnjEventWork, trialRecord) == 0x3b0);
static_assert(offsetof(QtnjEventWork, npcReader) == 0x3b8);
static_assert(offsetof(QtnjEventWork, baseHeight) == 0x3c0);
void QueueQtnjEvent(const std::int32_t *selection);
void CleanupQtnjEvent(QtnjEventWork *work);
extern const ScriptAreaDefinition qtnjScriptDefinition;
inline constexpr shen_name QTNJ_TRIAL_0_CHARACTERS[]{SHEN_NAME(ANC_), SHEN_NAME(SJN_), SHEN_NAME(KU7_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_TRIAL_1_CHARACTERS[]{SHEN_NAME(VUN_), SHEN_NAME(KK5_), SHEN_NAME(KK1_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_TRIAL_2_CHARACTERS[]{SHEN_NAME(KU7_), SHEN_NAME(MOK_), SHEN_NAME(P06_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_TRIAL_3_CHARACTERS[]{SHEN_NAME(IKO_), SHEN_NAME(P07_), SHEN_NAME(KU6_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_ADMISSION_CHARACTERS[]{QTNJ_RYO, QTNJ_REN, QTNJ_INVALID};
inline constexpr shen_name QTNJ_START_CHARACTERS[]{SHEN_NAME(RNT_), SHEN_NAME(KT9_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_FAILURE_CHARACTERS[]{SHEN_NAME(ANC_), SHEN_NAME(KK1_), SHEN_NAME(KT9_), SHEN_NAME(KU6_), SHEN_NAME(RNT_), QTNJ_INVALID};
inline constexpr shen_name QTNJ_LAST_FAILURE_CHARACTERS[]{SHEN_NAME(IKO_), SHEN_NAME(KK1_), SHEN_NAME(P07_), SHEN_NAME(KU6_), SHEN_NAME(RNT_), QTNJ_INVALID};
inline constexpr std::int32_t QTNJ_DEFAULT_SELECTION[]{0};
inline constexpr QtnjTrial QTNJ_TRIALS[]{
    {SHEN_NAME(TNJ3), 2, 3, 5, {224.10000610351562f, 132.39999389648438f, 151.0500030517578f}, 180.0f, QTNJ_TRIAL_0_CHARACTERS, SHEN_NAME(ATH1), {1, 256, 0, 0},
        16, 2},
    {SHEN_NAME(TNJ0), 8, 9, 11, {198.89999389648438f, 132.39999389648438f, 169.1999969482422f}, 0.0f, QTNJ_TRIAL_1_CHARACTERS, SHEN_NAME(ATH2), {4, 256, 0, 0},
        16, 2},
    {SHEN_NAME(TNJ3), 13, 14, 16, {216.89999389648438f, 132.39999389648438f, 137.10000610351562f}, 0.0f, QTNJ_TRIAL_2_CHARACTERS, SHEN_NAME(ATH3),
        {64, 1, 256, 0}, 24, 3},
    {SHEN_NAME(TNJ0), 19, 20, 21, {247.5f, 132.39999389648438f, 135.60000610351562f}, 180.0f, QTNJ_TRIAL_3_CHARACTERS, SHEN_NAME(ATH4), {16, 1, 256, 0}, 24, 3},
};
inline constexpr EventMovementWaypoint QTNJ_WAYPOINTS[]{
    {{249.6999969482422f, 132.39999389648438f, 167.39999389648438f}, 0.0f},
    {{249.3000030517578f, 132.39999389648438f, 147.8000030517578f}, 90.0f},
    {{236.1999969482422f, 132.39999389648438f, 147.39999389648438f}, 90.0f},
    {{231.8000030517578f, 132.39999389648438f, 147.39999389648438f}, 90.0f},
    {{224.39999389648438f, 132.39999389648438f, 147.0f}, 180.0f},
    {{223.89999389648438f, 132.39999389648438f, 156.0f}, 135.0f},
    {{220.8000030517578f, 132.39999389648438f, 160.89999389648438f}, 180.0f},
    {{220.3000030517578f, 132.39999389648438f, 170.1999969482422f}, 135.0f},
    {{215.5f, 132.39999389648438f, 177.6999969482422f}, 90.0f},
    {{209.1999969482422f, 132.39999389648438f, 178.39999389648438f}, 90.0f},
    {{199.3000030517578f, 132.39999389648438f, 178.0f}, 0.0f},
    {{198.89999389648438f, 132.39999389648438f, 163.0f}, 0.0f},
    {{198.8000030517578f, 132.39999389648438f, 153.0f}, 0.0f},
    {{198.8000030517578f, 132.39999389648438f, 152.0f}, 0.0f},
    {{198.89999389648438f, 132.39999389648438f, 147.39999389648438f}, 270.0f},
    {{216.3000030517578f, 132.39999389648438f, 147.39999389648438f}, 0.0f},
    {{216.6999969482422f, 132.39999389648438f, 132.39999389648438f}, 0.0f},
    {{217.0f, 132.39999389648438f, 126.5f}, 270.0f},
    {{236.60000610351562f, 132.39999389648438f, 126.0f}, 270.0f},
    {{239.0f, 132.39999389648438f, 126.0f}, 270.0f},
    {{240.8000030517578f, 132.39999389648438f, 126.0f}, 270.0f},
    {{244.6999969482422f, 132.39999389648438f, 126.5f}, 180.0f},
    {{247.5f, 132.39999389648438f, 140.0f}, 270.0f},
    {{-1.0f, -1.0f, -1.0f}, 0.0f},
};
inline constexpr EventMovementMotions QTNJ_MOTIONS{61442, 61442, 61442, 61442, 8556, 61442};
inline constexpr Vector3 QTNJ_FAILURE_OFFSETS[]{
    {-17.600006103515625f, 0.0f, 33.20001220703125f},
    {-40.30000305175781f, 0.0f, 10.900009155273438f},
    {9.199996948242188f, 0.0f, -18.99999237060547f},
};
} // namespace shenmue::s2
