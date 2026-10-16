#pragma once
#include "s2/area_event.hpp"
#include "s2/conversation.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name QRDK_TASK{SHEN_NAME(EVNT)}, QRDK_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name QRDK_REN{SHEN_NAME(REN_)}, QRDK_INVALID{0xffffffffU};
inline constexpr shen_name QRDK_R102{SHEN_NAME(R102)}, QRDK_R104{SHEN_NAME(R104)}, QRDK_R117{SHEN_NAME(R117)};
inline constexpr shen_name QRDK_R122{SHEN_NAME(R122)};
inline constexpr std::uint16_t QRDK_MOTION_KT6_TALK_D = 0x1c44, QRDK_MOTION_DOOR_KICK_PUSH = 0x1c42;
inline constexpr std::uint16_t QRDK_MOTION_BREAK_DOOR = 0x1c41, QRDK_MOTION_ANC_TALK_C = 0x1c43;
inline constexpr std::array<shen_name, 8> QRDK_CHARACTERS{
    QRDK_REN, SHEN_NAME(P05Q), SHEN_NAME(P04Q), SHEN_NAME(P07Q), SHEN_NAME(P01Q), SHEN_NAME(P08Q), SHEN_NAME(P03Q), QRDK_INVALID};
struct QrdkRoutePoint {
    float x, z;
    std::int32_t marker;
};
struct QrdkDoorPoint {
    shen_name door;
    float x, z;
    std::uint32_t yaw;
};
struct QrdkEnemy {
    shen_name character, door;
    float x, z;
    std::uint32_t yaw, style;
};
inline constexpr std::array<QrdkRoutePoint, 16> QRDK_ROUTE{{{.x = 209, .z = 177.6999969482422f, .marker = 0}, {.x = 200, .z = 179, .marker = 0},
    {.x = 199.1999969482422f, .z = 176, .marker = 1}, {.x = 199.1999969482422f, .z = 150, .marker = 0}, {.x = 200.5f, .z = 147.3000030517578f, .marker = 0},
    {.x = 214.1999969482422f, .z = 148.1999969482422f, .marker = 0}, {.x = 216.1999969482422f, .z = 145.1999969482422f, .marker = 0},
    {.x = 217.5f, .z = 126, .marker = 0}, {.x = 236.39999389648438f, .z = 126, .marker = 0}, {.x = 232.60000610351562f, .z = 125.80000305175781f, .marker = 0},
    {.x = 217, .z = 126.4000015258789f, .marker = 0}, {.x = 216.8000030517578f, .z = 131.60000610351562f, .marker = 0},
    {.x = 216.8000030517578f, .z = 144.8000030517578f, .marker = 0}, {.x = 214.83999633789062f, .z = 147.1999969482422f, .marker = 0},
    {.x = 198.89999389648438f, .z = 147.8000030517578f, .marker = 0}, {.x = 199, .z = 150.60000610351562f, .marker = 2}}};
inline constexpr std::array<QrdkDoorPoint, 14> QRDK_DOORS{
    {{.door = SHEN_NAME(R101), .x = 200, .z = 152, .yaw = 0xc000}, {.door = QRDK_R102, .x = 200, .z = 157.5f, .yaw = 0xc000},
        {.door = SHEN_NAME(R103), .x = 200, .z = 163, .yaw = 0xc000}, {.door = QRDK_R104, .x = 200, .z = 168.39999389648438f, .yaw = 0xc000},
        {.door = SHEN_NAME(R105), .x = 200, .z = 173.6999969482422f, .yaw = 0xc000}, {.door = SHEN_NAME(R116), .x = 215, .z = 143, .yaw = 0x4000},
        {.door = QRDK_R117, .x = 215, .z = 138, .yaw = 0x4000}, {.door = SHEN_NAME(R118), .x = 215, .z = 132, .yaw = 0x4000},
        {.door = SHEN_NAME(R119), .x = 215, .z = 127, .yaw = 0x4000}, {.door = SHEN_NAME(R120), .x = 218, .z = 124, .yaw = 0},
        {.door = SHEN_NAME(R121), .x = 223.3000030517578f, .z = 124, .yaw = 0}, {.door = QRDK_R122, .x = 228.60000610351562f, .z = 124, .yaw = 0},
        {.door = SHEN_NAME(R123), .x = 234, .z = 124, .yaw = 0}, {.door = QRDK_INVALID, .x = 0, .z = 0, .yaw = 0}}};
inline constexpr std::array<QrdkEnemy, 7> QRDK_ENEMIES{{{.character = QRDK_CHARACTERS[1], .door = QRDK_R102, .x = 1, .z = -1, .yaw = 0, .style = 2},
    {.character = QRDK_CHARACTERS[2], .door = QRDK_R102, .x = -1, .z = -1, .yaw = 0, .style = 3},
    {.character = QRDK_CHARACTERS[3], .door = QRDK_R104, .x = 1, .z = -1, .yaw = 0, .style = 2},
    {.character = QRDK_CHARACTERS[4], .door = QRDK_R104, .x = -1, .z = -1, .yaw = 0, .style = 3},
    {.character = QRDK_CHARACTERS[5], .door = QRDK_R117, .x = 1, .z = -1, .yaw = 0, .style = 2},
    {.character = QRDK_CHARACTERS[6], .door = QRDK_R117, .x = -1, .z = -1, .yaw = 0, .style = 3},
    {.character = QRDK_INVALID, .door = QRDK_INVALID, .x = 0, .z = 0, .yaw = 0, .style = 0}}};
#pragma pack(push, 4)
struct QrdkWork {
    HLib::Task *parent;
    HLib::Task *task;
    std::uint32_t flags, action;
    std::int32_t routeCounter;
    shen_name door;
    Vector3 savedPosition;
    Vector3i savedRotation;
    std::byte reserved38[0x14];
    std::uint32_t loadIndex, enemyCount;
    std::array<std::uint32_t, 6> defeated;
};
using QrdkSerifCallback = void (*)(std::int32_t event, std::uintptr_t argument);
struct QrdkSerifWork {
    HLib::Task *parent;
    ConversationBuffer *conversation;
    std::int32_t startTicks, endTicks;
    QrdkSerifCallback callback;
    std::int32_t voice;
};
#pragma pack(pop)
static_assert(sizeof(QrdkRoutePoint) == 12 && sizeof(QrdkDoorPoint) == 16 && sizeof(QrdkEnemy) == 24);
static_assert(sizeof(QrdkWork) == 108 && alignof(QrdkWork) == 4 && offsetof(QrdkWork, task) == 8);
static_assert(offsetof(QrdkWork, flags) == 16 && offsetof(QrdkWork, action) == 20 && offsetof(QrdkWork, routeCounter) == 24);
static_assert(offsetof(QrdkWork, door) == 28 && offsetof(QrdkWork, savedPosition) == 32 && offsetof(QrdkWork, savedRotation) == 44);
static_assert(offsetof(QrdkWork, loadIndex) == 76 && offsetof(QrdkWork, enemyCount) == 80 && offsetof(QrdkWork, defeated) == 84);
static_assert(sizeof(QrdkSerifWork) == 36 && offsetof(QrdkSerifWork, startTicks) == 16 && offsetof(QrdkSerifWork, callback) == 24);
void QrdkInitNullsub();
void QrdkMain(std::uintptr_t argument);
void QrdkHideCharacter(shen_name character);
const QrdkRoutePoint *QrdkGetRoutePoint(std::int32_t counter);
void QrdkPlaceOnRoute(QrdkWork *work);
bool QrdkPassedRoutePoint(std::int32_t counter);
std::uint32_t QrdkGetDoorPoint(shen_name door, Vector3 *position, std::uint32_t *yaw);
shen_name QrdkFindClosestDoor(float *distance);
void QrdkQueueSerif(const char *text, std::int32_t startTicks, std::int32_t endTicks, QrdkSerifCallback callback);
void QrdkResumeOnSerifFinished(std::int32_t event, std::uintptr_t argument);
void QrdkInitializeCallback(void *data);
void QrdkWaitPackageCallback(void *data);
void QrdkLoadCharactersCallback(void *data);
void QrdkBeginGameCallback(void *data);
void QrdkStartRouteCallback(void *data);
void QrdkRunCallback(void *data);
void QrdkBeginDoorCallback(void *data);
void QrdkGetDoorCallback(void *data);
void QrdkWrongRoomCallback(void *data);
void QrdkReturnToRouteCallback(void *data);
void QrdkFoundCallback(void *data);
void QrdkBattleMessageCallback(void *data);
void QrdkPlaceEnemiesCallback(void *data);
void QrdkStartBattleCallback(void *data);
void QrdkBattleCallback(void *data);
void QrdkRetryCallback(void *data);
void QrdkExitCallback(void *data);
void QrdkSerifCallbackLoop(void *data);
extern const ScriptAreaDefinition qrdkScriptDefinition;
} // namespace shenmue::s2::areas
