#pragma once
#include "s2/areas/qes_event.hpp"
#include "s2/event_characters.hpp"
namespace shenmue::s2 {

struct Qes1EventWork {
    HLib::Task *task;
    std::int16_t phase, subphase;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed;
    std::uint32_t reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded, retryCanSkip, retryFrames;
};
static_assert(sizeof(Qes1EventWork) == 0x30 && offsetof(Qes1EventWork, phase) == 8 && offsetof(Qes1EventWork, subphase) == 0x0a);
static_assert(offsetof(Qes1EventWork, characters) == 0x10 && offsetof(Qes1EventWork, qteElapsed) == 0x18);
static_assert(offsetof(Qes1EventWork, qteStatus) == 0x20 && offsetof(Qes1EventWork, qteSucceeded) == 0x24);
static_assert(offsetof(Qes1EventWork, retryCanSkip) == 0x28 && offsetof(Qes1EventWork, retryFrames) == 0x2c);
inline constexpr Vector3 QES1_INITIAL_RYO{0.600000024f, 0, 9}, QES1_INITIAL_REN{0, 0, 5.5999999f};
inline constexpr Vector3 QES1_KICK_RYO{17, 0, 2.799999952f}, QES1_KICK_REN{13.699999809f, 0, 3.200000048f};
inline constexpr Vector3 QES1_KICK_KU1{-3.0999999f, 0, -0.400000006f}, QES1_KICK_KK6{1.20000005f, 0, 0.300000012f};
inline constexpr Vector3 QES1_ROUTE_RYO{6.80000019f, 0, -0.100000001f}, QES1_ROUTE_REN{-1.70000005f, 0, 2};
inline constexpr Vector3 QES1_CHAINSAW_APPROACH{9.5f, 0, -1.29999995f};
inline constexpr Vector3 QES1_PURSUER_POSITIONS[]{
    {-1.89999998f, 0, -1}, {-2.79999995f, 0, 0.400000006f}, {-0.800000012f, 0, 0.800000012f}, {-1.10000002f, 0, -2}};
inline constexpr Vector3 QES1_DEADEND_RYO{9.69999981f, 0, -0.899999976f}, QES1_DEADEND_SOY{0.400000006f, 0, 1.5f};
inline constexpr Vector3 QES1_THIRD_RYO{-13.6000004f, 0, 0.200000003f}, QES1_THIRD_REN{-11.8000002f, 0, -0.600000024f};
inline constexpr Vector3 QES1_THIRD_KN3{5, 0, -0.600000024f};
inline constexpr Vector3 QES1_CHAINSAW_RYO{-8.60000038f, 0, 0.200000003f};
inline constexpr Vector3 QES1_RETRY_POSITIONS[]{{20.2999992f, 0, 6.5f}, {24.8999996f, 0, 15.3000002f}, {26.2000008f, 0, 15.3000002f},
    {26.1000004f, 0, 13.8999996f}, {27.3999996f, 0, 14.3000002f}, {24.7999992f, 0, 13.6999998f}};
inline constexpr Vector3 QES1_FINAL_RYO{20.6000004f, 0, 14.1999998f};
inline constexpr Vector3 QES1_STICK_SUCCESS_RYO{-1.60000002f, 0, 0.100000001f}, QES1_STICK_SUCCESS_REN{-7.69999981f, 0, -0.800000012f};
inline constexpr Vector3 QES1_STICK_FAILURE_RYO{-1.60000002f, 0, -0.5f};
inline constexpr Vector3 QES1_RUBBLE_SUCCESS{2.0999999f, 0, 0}, QES1_RUBBLE_FAILURE{1.79999995f, 0, 0.699999988f};
inline constexpr Vector3 QES1_KICK_FAILURE_POSITION{-0.200000003f, 0, -1.89999998f};
inline constexpr Vector3 QES1_CHAINSAW_ATTACH_POSITION{0.202000007f, 0.100000001f, -0.04399999976f};
inline constexpr Vector3 QES1_RETRY_ATTACH_POSITION{0.319999993f, 0.0350000001f, -0.12999999523f};
inline constexpr Vector3 QES1_FLOORBOARD_RYO{0, 0, 5}, QES1_FLOORBOARD_DUMMY{0, -0.600000024f, 0}, QES1_FLOORBOARD{0, 0, -1.5f};
inline constexpr Vector3 QES1_JUMP_FAILURE_REN{-0.5f, 0, -3.5f};
inline constexpr Vector3 QES1_KICK_FAILURE_KU1{0, 0, -5.30000019f}, QES1_KICK_SUCCESS_KU1{0, 0, -6.5f};
inline constexpr Vector3 QES1_OFFSCREEN_REN{-100, 0, 1};
struct QesDefeatSound {
    std::uint32_t sound;
    std::int32_t delay;
};
inline constexpr QesDefeatSound QES1_DEFEAT_SOUNDS_A[]{{.sound = 649, .delay = 62}, {.sound = 650, .delay = 76}, {.sound = 132, .delay = 87},
    {.sound = 653, .delay = 98}, {.sound = 651, .delay = 114}, {.sound = 652, .delay = 130}, {.sound = 649, .delay = 140}, {.sound = 628, .delay = 155},
    {.sound = 652, .delay = 160}, {.sound = 650, .delay = 187}, {.sound = 628, .delay = 52}, {.sound = 629, .delay = 58}, {.sound = 653, .delay = 91},
    {.sound = 132, .delay = 127}, {.sound = 133, .delay = 165}, {.sound = 649, .delay = 194}, {.sound = 653, .delay = 103}, {.sound = 628, .delay = 160},
    {.sound = 629, .delay = 166}};
inline constexpr QesDefeatSound QES1_DEFEAT_SOUNDS_B[]{{.sound = 653, .delay = 53}, {.sound = 651, .delay = 85}, {.sound = 652, .delay = 115},
    {.sound = 652, .delay = 131}, {.sound = 650, .delay = 142}, {.sound = 649, .delay = 158}, {.sound = 628, .delay = 45}, {.sound = 629, .delay = 51},
    {.sound = 649, .delay = 58}, {.sound = 650, .delay = 70}, {.sound = 653, .delay = 47}};
extern const ScriptAreaDefinition qes1ScriptDefinition;
} // namespace shenmue::s2
