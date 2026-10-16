#pragma once
#include "s2/area_event.hpp"
#include "s2/legacy_hud_text.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name QDMR_AREA{SHEN_NAME(QDMR)}, QDMR_PLAYER{SHEN_NAME(RYO_)}, QDMR_REN{SHEN_NAME(REN_)};
inline constexpr shen_name QDMR_R101{SHEN_NAME(R101)}, QDMR_R102{SHEN_NAME(R102)}, QDMR_R103{SHEN_NAME(R103)}, QDMR_R104{SHEN_NAME(R104)};
inline constexpr shen_name QDMR_R105{SHEN_NAME(R105)}, QDMR_R106{SHEN_NAME(R106)}, QDMR_R107{SHEN_NAME(R107)}, QDMR_R108{SHEN_NAME(R108)};
inline constexpr shen_name QDMR_R109{SHEN_NAME(R109)}, QDMR_R110{SHEN_NAME(R110)}, QDMR_R111{SHEN_NAME(R111)}, QDMR_R112{SHEN_NAME(R112)};
inline constexpr shen_name QDMR_R113{SHEN_NAME(R113)}, QDMR_R114{SHEN_NAME(R114)}, QDMR_R115{SHEN_NAME(R115)}, QDMR_R116{SHEN_NAME(R116)};
inline constexpr shen_name QDMR_R117{SHEN_NAME(R117)}, QDMR_R118{SHEN_NAME(R118)}, QDMR_R119{SHEN_NAME(R119)}, QDMR_R120{SHEN_NAME(R120)};
inline constexpr shen_name QDMR_R121{SHEN_NAME(R121)}, QDMR_R122{SHEN_NAME(R122)}, QDMR_R123{SHEN_NAME(R123)}, QDMR_R124{SHEN_NAME(R124)};
inline constexpr shen_name QDMR_QF1B{SHEN_NAME(QF1B)}, QDMR_QF2B{SHEN_NAME(QF2B)}, QDMR_QF3B{SHEN_NAME(QF3B)}, QDMR_QFIX{SHEN_NAME(QFIX)};
inline constexpr shen_name QDMR_P02{SHEN_NAME(P02_)}, QDMR_P04{SHEN_NAME(P04_)}, QDMR_P05{SHEN_NAME(P05_)}, QDMR_P07{SHEN_NAME(P07_)};
inline constexpr shen_name QDMR_P08{SHEN_NAME(P08_)}, QDMR_P09{SHEN_NAME(P09_)};
inline constexpr shen_name QDMR_TRANSITION_TARGET{SHEN_NAME(trns)};
inline constexpr shen_name QDMR_DEFAULT_LIGHT{SHEN_NAME(0000)}, QDMR_INIT_LIGHT{SHEN_NAME(INIT)};
inline constexpr shen_name QDMR_AD1{SHEN_NAME(AD1_)}, QDMR_KT2{SHEN_NAME(KT2_)}, QDMR_KTA{SHEN_NAME(KTA_)}, QDMR_YUAN_PROP{SHEN_NAME(a452)};
inline constexpr shen_name QDMR_KF4{SHEN_NAME(KF4_)};
inline constexpr shen_name QDMR_AA99{SHEN_NAME(aa99)}, QDMR_A020{SHEN_NAME(a020)};
inline constexpr shen_name QDMR_DUMMY{SHEN_NAME(qdmr)}, QDMR_QF0X{SHEN_NAME(QF0X)}, QDMR_PROP{SHEN_NAME(PROP)}, QDMR_QFBA{SHEN_NAME(QFBA)};
inline constexpr shen_name QDMR_QFB1{SHEN_NAME(QFB1)}, QDMR_QFB2{SHEN_NAME(QFB2)}, QDMR_QFB3{SHEN_NAME(QFB3)};
inline constexpr shen_name QDMR_BTKK{SHEN_NAME(BTKK)}, QDMR_BTPP{SHEN_NAME(BTPP)}, QDMR_BTAA{SHEN_NAME(BTAA)};
inline constexpr shen_name QDMR_TIMEOUT_AUTHOR{SHEN_NAME(TOVR)}, QDMR_YUAN_AUTHOR{SHEN_NAME(END_)};
inline constexpr shen_name QDMR_BINGO_OPEN_AUTHOR{SHEN_NAME(BBGN)}, QDMR_BINGO_WIN_AUTHOR{SHEN_NAME(BQIT)}, QDMR_RETRY_TITLE{SHEN_NAME(RTR2)};
inline constexpr shen_name QDMR_TIMEOUT_TITLE{SHEN_NAME(TENT) | 0x02010800U};
inline constexpr shen_name QDMR_SBK{SHEN_NAME(SBK_)}, QDMR_ANC{SHEN_NAME(ANC_)}, QDMR_KU8{SHEN_NAME(KU8_)};
inline constexpr shen_name QDMR_KHO{SHEN_NAME(KHO_)}, QDMR_UYD{SHEN_NAME(UYD_)}, QDMR_KT7{SHEN_NAME(KT7_)};
struct QdmrRoomPair {
    shen_name door, package;
};
struct QdmrDoorRoute {
    shen_name door;
    std::int32_t variant;
};
struct QdmrDoorPlacement {
    shen_name door;
    Vector2 position;
    std::int32_t yaw;
};
inline constexpr QdmrRoomPair QDMR_ROOMS[]{{.door = QDMR_R101, .package = QDMR_R101}, {.door = QDMR_R102, .package = QDMR_R102},
    {.door = QDMR_R103, .package = QDMR_R103}, {.door = QDMR_R104, .package = QDMR_QF1B}, {.door = QDMR_R105, .package = QDMR_R105},
    {.door = QDMR_R106, .package = QDMR_R106}, {.door = QDMR_R107, .package = QDMR_R107}, {.door = QDMR_R108, .package = QDMR_R108},
    {.door = QDMR_R109, .package = QDMR_R109}, {.door = QDMR_R110, .package = QDMR_R110}, {.door = QDMR_R111, .package = QDMR_R111},
    {.door = QDMR_R112, .package = QDMR_QF2B}, {.door = QDMR_R113, .package = QDMR_R113}, {.door = QDMR_R114, .package = QDMR_R114},
    {.door = QDMR_R115, .package = QDMR_R115}, {.door = QDMR_R116, .package = QDMR_R116}, {.door = QDMR_R117, .package = QDMR_R117},
    {.door = QDMR_R118, .package = QDMR_R118}, {.door = QDMR_R119, .package = QDMR_R119}, {.door = QDMR_R120, .package = QDMR_QF3B},
    {.door = QDMR_R121, .package = QDMR_R121}, {.door = QDMR_R122, .package = QDMR_R122}, {.door = QDMR_R123, .package = QDMR_R123},
    {.door = QDMR_R124, .package = QDMR_QFIX}, {}};
inline constexpr QdmrDoorRoute QDMR_ROUTES[]{{.door = QDMR_R101, .variant = 0}, {.door = QDMR_R102, .variant = 0}, {.door = QDMR_R103, .variant = 0},
    {.door = QDMR_R104, .variant = 1}, {.door = QDMR_R105, .variant = 0}, {.door = QDMR_R106, .variant = 0}, {.door = QDMR_R107, .variant = 0},
    {.door = QDMR_R108, .variant = 0}, {.door = QDMR_R109, .variant = 0}, {.door = QDMR_R110, .variant = 0}, {.door = QDMR_R111, .variant = 0},
    {.door = QDMR_R112, .variant = 2}, {.door = QDMR_R113, .variant = 0}, {.door = QDMR_R114, .variant = 0}, {.door = QDMR_R115, .variant = 0},
    {.door = QDMR_R116, .variant = 0}, {.door = QDMR_R117, .variant = 0}, {.door = QDMR_R118, .variant = 0}, {.door = QDMR_R119, .variant = 0},
    {.door = QDMR_R120, .variant = 3}, {.door = QDMR_R121, .variant = 0}, {.door = QDMR_R122, .variant = 0}, {.door = QDMR_R123, .variant = 0},
    {.door = QDMR_R124, .variant = 7}, {}};
inline constexpr QdmrDoorPlacement QDMR_DOOR_PLACEMENTS[]{{.door = QDMR_R101, .position = {200.7100067138672f, 152.5500030517578f}, .yaw = 49152},
    {.door = QDMR_R102, .position = {200.7100067138672f, 157.9499969482422f}, .yaw = 49152},
    {.door = QDMR_R103, .position = {200.7100067138672f, 163.35000610351562f}, .yaw = 49152},
    {.door = QDMR_R104, .position = {200.7100067138672f, 168.75f}, .yaw = 49152},
    {.door = QDMR_R105, .position = {200.7100067138672f, 174.14999389648438f}, .yaw = 49152},
    {.door = QDMR_R106, .position = {218.6999969482422f, 166.0500030517578f}, .yaw = 16384},
    {.door = QDMR_R107, .position = {218.6999969482422f, 160.64999389648438f}, .yaw = 16384},
    {.door = QDMR_R108, .position = {218.6999969482422f, 155.25f}, .yaw = 16384},
    {.door = QDMR_R109, .position = {230.85000610351562f, 149.39999389648438f}, .yaw = 32768},
    {.door = QDMR_R110, .position = {241.64999389648438f, 149.39999389648438f}, .yaw = 32768},
    {.door = QDMR_R111, .position = {251.11000061035156f, 152.5500030517578f}, .yaw = 49152},
    {.door = QDMR_R112, .position = {251.11000061035156f, 157.9499969482422f}, .yaw = 49152},
    {.door = QDMR_R113, .position = {251.11000061035156f, 163.35000610351562f}, .yaw = 49152},
    {.door = QDMR_R114, .position = {244.35000610351562f, 169.1999969482422f}, .yaw = 32768},
    {.door = QDMR_R115, .position = {255.14999389648438f, 169.1999969482422f}, .yaw = 32768},
    {.door = QDMR_R116, .position = {215.10000610351562f, 142.64999389648438f}, .yaw = 16384},
    {.door = QDMR_R117, .position = {215.10000610351562f, 137.25f}, .yaw = 16384},
    {.door = QDMR_R118, .position = {215.10000610351562f, 131.85000610351562f}, .yaw = 16384},
    {.door = QDMR_R119, .position = {215.08999633789062f, 126.44999694824219f}, .yaw = 16384},
    {.door = QDMR_R120, .position = {218.25f, 124.19999694824219f}, .yaw = 0},
    {.door = QDMR_R121, .position = {223.64999389648438f, 124.19999694824219f}, .yaw = 0},
    {.door = QDMR_R122, .position = {229.0500030517578f, 124.19999694824219f}, .yaw = 0},
    {.door = QDMR_R123, .position = {234.4499969482422f, 124.19999694824219f}, .yaw = 0},
    {.door = QDMR_R124, .position = {249.30999755859375f, 133.64999389648438f}, .yaw = 49152}, {}};
inline constexpr shen_name QDMR_NPCS[]{QDMR_PLAYER, QDMR_P02, QDMR_P04, QDMR_P05, QDMR_P07, QDMR_P08, QDMR_P09, {}};
inline constexpr shen_name QDMR_YUAN_NPCS[]{QDMR_PLAYER, QDMR_REN, QDMR_AD1, QDMR_KT2, QDMR_KTA, QDMR_YUAN_PROP, {}};
inline constexpr shen_name QDMR_BINGO_NPCS[]{QDMR_KF4, QDMR_P02, QDMR_P07, QDMR_P09, {}};
inline constexpr shen_name QDMR_FIRST_ROOM_NPCS[]{QDMR_SBK, QDMR_ANC, {}};
inline constexpr shen_name QDMR_SECOND_ROOM_NPCS[]{QDMR_KU8, QDMR_KHO, {}};
inline constexpr shen_name QDMR_THIRD_ROOM_NPCS[]{QDMR_UYD, QDMR_KT7, {}};
inline constexpr std::int32_t QDMR_FIRST_ROOM_HEALTH[]{100, 100}, QDMR_FIRST_ROOM_STYLES[]{2, 3};
inline constexpr std::int32_t QDMR_SECOND_ROOM_HEALTH[]{100, 100}, QDMR_SECOND_ROOM_STYLES[]{4, 2};
inline constexpr std::int32_t QDMR_THIRD_ROOM_HEALTH[]{100, 100}, QDMR_THIRD_ROOM_STYLES[]{3, 4};
inline constexpr std::int32_t QDMR_BINGO_ROOM_HEALTH[]{200, 80, 80, 80}, QDMR_BINGO_ROOM_STYLES[]{38, 3, 2, 2};
inline constexpr Vector3 QDMR_LADDER_POSITION{224.1699981689453f, 164.8000030517578f, 178.94000244140625f};
inline constexpr Vector4 QDMR_INIT_LIGHT_OFFSET{0, 64.80000305175781f, 0, 0};
inline std::array<std::int32_t, 57> QDMR_HAND_POSE{0, 0, 46606, 0, 2182, 64444, 0, 0, 1456, 0, 64080, 49154, 0, 908, 44239, 0, 0, 53340, 0, 0, 0, 0, 1456,
    54796, 0, 0, 43692, 0, 727, 50974, 0, 0, 0, 0, 2912, 54797, 0, 182, 44421, 0, 1456, 50245, 0, 0, 0, 2912, 910, 51336, 726, 726, 47517, 0, 2003, 50974,
    65518, 65427, 0};
inline std::array<std::int32_t, 57> QDMR_TRANSITION_HAND_POSE{1999, 65354, 59167, 1638, 543, 64810, 546, 63262, 4821, 0, 64810, 56252, 0, 64081, 58800, 0, 727,
    363, 0, 0, 64628, 0, 0, 57709, 0, 0, 57344, 0, 65172, 65354, 0, 0, 65354, 0, 729, 60074, 0, 65355, 57526, 0, 64809, 182, 0, 0, 0, 0, 910, 55705, 0, 1456,
    59164, 0, 63354, 1092, 64080, 182, 0};
struct QdmrEventWork {
    HLib::Task *task;
    HLib::Task *timerTask;
    HLib::Task *parent;
    std::uint32_t *roomFlags;
    std::uint32_t flags;
    std::uint16_t doorQteButtons, reserved26;
    shen_name door;
    std::int32_t battleRetries, timer, timerFlags;
    std::uint32_t timerRequests;
    std::int32_t timerFadeFrames, timerWarningThreshold, reserved44;
    LegacyHudText *timerText;
    Vector3 savedPosition;
    std::int32_t savedYaw;
    std::int32_t canSkipIntro, reserved64;
};
struct QdmrDoorMotionWork {
    shen_name door;
    std::int32_t frames;
    float startFrame, speed;
    std::uint32_t motion;
};
static_assert(sizeof(QdmrDoorMotionWork) == 0x14);
static_assert(offsetof(QdmrDoorMotionWork, startFrame) == 8);
static_assert(offsetof(QdmrDoorMotionWork, motion) == 0x10);
static_assert(sizeof(QdmrRoomPair) == 8);
static_assert(sizeof(QdmrDoorRoute) == 8);
static_assert(sizeof(QdmrDoorPlacement) == 16);
static_assert(sizeof(QdmrEventWork) == 0x68);
static_assert(offsetof(QdmrEventWork, flags) == 0x20);
static_assert(offsetof(QdmrEventWork, door) == 0x28);
static_assert(offsetof(QdmrEventWork, timerText) == 0x48);
static_assert(offsetof(QdmrEventWork, savedPosition) == 0x50);
static_assert(offsetof(QdmrEventWork, canSkipIntro) == 0x60);
void QdmrTimerCallback(void *parameter);
shen_name FindNearestQdmrDoor(const std::uint32_t *roomFlags);
std::int32_t GetQdmrDoorVariant(shen_name door, const std::uint32_t *roomFlags);
void MarkQdmrRoomVisited(shen_name door, std::uint32_t *roomFlags);
void SetQdmrPlayerHandState(QdmrEventWork *work, bool scripted);
std::int64_t QdmrStoryAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
std::int64_t QdmrInitialAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
std::int64_t QdmrYuanAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
std::int64_t QdmrBingoWinAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
std::int64_t QdmrBingoOpeningAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
void QueueQdmrTransitionAttachment(const Vector3 *position, const Vector3i *rotation);
void QueueQdmrTransitionLookAt();
bool FindClearRotationPathForAngY(std::int32_t *yaw);
void QdmrDoorMotionCallback(void *parameter);
bool GetQdmrDoorPlacement(shen_name door, Vector3 *position, std::int32_t *yaw);
void QdmrLoopSoundCallback(void *parameter);
void QueueQdmrLoopSound(std::uint32_t command, std::int32_t delay);
void QdmrFadeOutSoundCallback(void *parameter);
void QdmrDoneCallback(void *parameter);
extern const ScriptAreaDefinition qdmrDefinition;
} // namespace shenmue::s2
