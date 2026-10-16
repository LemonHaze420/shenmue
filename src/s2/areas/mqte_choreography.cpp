#include "s2/areas/area_mqte.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/map_control.hpp"
#include "s2/maze.hpp"
#include "s2/motion_work.hpp"
#include "s2/screen_film.hpp"
#include "common/math.hpp"

namespace shenmue::s2::areas {
namespace {
void BeginReaction(float distance, std::int32_t camera, const char *voice, std::uint32_t motion, float frame, float speed, std::uint32_t flags) {
    PositionForestQteReaction(distance);
    if (forestLastCommand)
        EVT_AT_SetCam(camera, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetVoice(voice, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, motion, frame, speed, flags, 0);
}
std::int32_t DodgeLeftSuccess() {
    BeginReaction(.6f, 3300, "0517A004", 7503, 9, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 20);
    return -9;
}
std::int32_t DodgeLeftFailure() {
    BeginReaction(.7f, 3310, "0517A013", 7506, 13, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 63);
    return 48;
}
std::int32_t DodgeRightSuccess() {
    BeginReaction(.6f, 3400, "0517A004", 7503, 9, 1, 0x20000000U);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0x20000000U, 20);
    return -9;
}
std::int32_t DodgeRightFailure() {
    BeginReaction(.7f, 3410, "0517A013", 7506, 13, 1, 0x20000000U);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0x20000000U, 63);
    return 48;
}
std::int32_t PitJumpSuccess() {
    BeginReaction(.8f, 3000, "0517A004", 7490, 0, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 30);
    return 1;
}
std::int32_t PitJumpFailure() {
    BeginReaction(.9f, 3010, "0517A005", 7498, 10, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 7497, 0, 1, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0x40000000U, 150);
    return 121;
}
std::int32_t HurdleJumpSuccess() {
    BeginReaction(.9f, 3100, "0517A004", 7495, 0, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 30);
    return 1;
}
void PlaceShenfaAhead() {
    Vector3 ryo, position;
    EV_GetCharaPosition(MQTE_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    position = GetMazeRoads(forestStage)[ForestCharacterRoadIndex(MQTE_RYO) + 5].position;
    EV_AT_SetPos(MQTE_SHENFA, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    const std::uint16_t yaw = static_cast<std::uint16_t>(atan2_fixed16(position.x - ryo.x, position.z - ryo.z));
    EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, position, 0);
    EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, yaw / 182.04445f, 0);
}
std::int32_t HurdleJumpFailure() {
    if (forestShenfaTask)
        HLib::DestroyTaskInternal(forestShenfaTask);
    forestShenfaTask = nullptr;
    if (g_currentMap.name != MQTE_AREA.name)
        PlaceShenfaAhead();
    PositionForestQteReaction(.9f);
    EVT_AT_SetCam(4000, MQTE_CAMERA, MQTE_CAMERA, 0, 0, 0);
    EVT_AT_SetCam(4001, MQTE_DUMMY_SHENFA, MQTE_DUMMY_SHENFA, 0, 0, 50);
    EVT_AT_SetCam(4002, MQTE_DUMMY_RYO, MQTE_DUMMY_RYO, 0, 0, 120);
    EVT_AT_SetMot(MQTE_RYO, 7499, 1, .8f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, 7499, 40, 0, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, 7499, 40, 1, 0, 170);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 180);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0, 0, 0, 0);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0, 1, 0, 50);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30, 0, 0, 80);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30, 1, 0, 150);
    EVT_AT_SetMot(MQTE_SHENFA, 7514, 0, 1.1f, 0, 185);
    EVT_AT_SetVoice("0517A008", 1, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(MQTE_SHENFA), FacialExpressionId::Surprise, 8, .6f);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 0, 0, 1, 185);
    const char *const shenfaVoices[]{"0517B004", "0517B005", "0517B006", "0517B007"}; // @text 0517B004 EN: "Are you okay?" | JA: "大丈夫？" ; 0517B005 EN: "Are you all right?" | JA: "大丈夫っ？" ; 0517B006 EN: "Did you get hurt?" | JA: "ケガしなかった？" ; 0517B007 EN: "All right?" | JA: "平気？"
    const char *const ryoVoices[]{"0517A015", "0517A016", "0517A017", "0517A019"}; // @text 0517A015 EN: "Don't worry." | JA: "いや、大丈夫だ。" ; 0517A016 EN: "I'm okay." | JA: "…大丈夫。" ; 0517A017 EN: "I'm fine." | JA: "俺なら平気だ。" ; 0517A019 EN: "I'm okay.\nLet's move on." | JA: "大丈夫、行こう。"
    EVT_AT_SetVoice(shenfaVoices[shenGetRandomInt32(4)], 1, 70);
    EVT_AT_SetVoice(ryoVoices[shenGetRandomInt32(4)], 1, 115);
    return 185;
}
std::int32_t RootJumpSuccess() {
    BeginReaction(.6f, 3200, "0517A004", 7502, 0, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 7, 1.1f, 0, 20);
    return -4;
}
std::int32_t RootJumpFailure() {
    BeginReaction(.7f, 3210, "0517A013", 7505, 10, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 70);
    return 46;
}
std::int32_t DuckSuccess() {
    BeginReaction(.6f, 3500, "0517A004", 7500, 5, 1.2f, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 27);
    return -2;
}
std::int32_t DuckFailure() {
    BeginReaction(.6f, 3510, "0517A014", 7504, 5, 1, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0, 54);
    return 25;
}
} // namespace
const ForestQteCommand forestQteCommands[7]{{4, 0, DodgeLeftSuccess, DodgeLeftFailure, 4, 1.2f}, {0x40, 0, DodgeRightSuccess, DodgeRightFailure, 4, 1.2f},
    {0x400, 0, HurdleJumpSuccess, HurdleJumpFailure, 4, 1}, {0x100, 0, PitJumpSuccess, PitJumpFailure, 4, 2}, {1, 0, RootJumpSuccess, RootJumpFailure, 4, 1.5f},
    {0x10, 0, DuckSuccess, DuckFailure, 4, 1.5f}, {0x10, 0, DuckSuccess, DuckFailure, 4, 1.5f}};
void PositionForestQteReaction(float distance) {
    Vector3 offset{0, 0, distance}, local, world;
    Vector3i rotation{};
    AT_SetLocalBaseChara(MQTE_RYO, MQTE_BASE);
    EV_GetCharaPosition(MQTE_RYO, CHARACTER_TRANSLATION_ROOT, &local, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(MQTE_BASE, &rotation, EventTransformFlags::Local);
    EV_GetCharaPosition(MQTE_RYO, CHARACTER_TRANSLATION_ROOT, &world, EventTransformFlags::World);
    const float x = local.x;
    ApplyScenarioTranslationAndRotation(&forestObstaclePosition, &rotation, &offset);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, {offset.x, world.y, offset.z}, 0);
    EV_GetCharaPosition(MQTE_RYO, CHARACTER_TRANSLATION_ROOT, &local, EventTransformFlags::Local);
    local.x = x;
    EV_AT_SetPos(MQTE_RYO, EventTransformFlags::AllAxes, local);
    AT_SetLocalBaseChara(MQTE_RYO, MQTE_INVALID);
}
std::int32_t ForestFinalFailure(ForestQteWork &work) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 120, 0, 0, 0);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, {-550.3f, 0, -346.5f}, 0);
    EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, 270, 0);
    EVT_AT_SetPos(MQTE_CAMERA, MQTE_RYO, {0, 10, 0}, 0);
    EVT_AT_SetAngY(MQTE_CAMERA, MQTE_RYO, 0, 0);
    PlaceShenfaAhead();
    EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, {0, 0, 0}, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0, 0);
    EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, {0, 0, 0}, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(MQTE_RYO), FacialExpressionId::Suffer, 8, 1);
    EVT_AT_SetFaceLooks(MQTE_RYO, 0, 0, 1, 380);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(MQTE_SHENFA), FacialExpressionId::Scheme, 8, 1);
    EVT_AT_SetFaceLooks(MQTE_SHENFA, 0, 0, 1, 380);
    EVT_AT_SetCam(7003, MQTE_INVALID, MQTE_INVALID, 0, 0, 0);
    EVT_AT_SetCam(7004, MQTE_INVALID, MQTE_INVALID, 0, 0, 50);
    EVT_AT_SetCam(7000, MQTE_INVALID, MQTE_INVALID, 0, 0, 90);
    EVT_AT_SetCam(7001, MQTE_INVALID, MQTE_INVALID, 0, 0, 170);
    EVT_AT_SetCam(7002, MQTE_INVALID, MQTE_INVALID, 0, 0, 290);
    EVT_AT_SetMot(MQTE_RYO, 7499, 1, .8f, 0, 0);
    EVT_AT_SetMot(MQTE_RYO, 7493, 0, .8f, 0, 50);
    EVT_AT_SetMot(MQTE_RYO, 7493, 0, .8f, 0x40000000U, 90);
    EVT_AT_SetMot(MQTE_RYO, 7493, 280, .8f, 0x40000000U, 290);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0, 0, 0, 0);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 0, 1, 0, 50);
    EVT_AT_SetMot(MQTE_SHENFA, 7509, 30, 0, 0, 80);
    EVT_AT_SetPoint(MQTE_SHENFA, MQTE_CAMERA, 5, 3, 1, 170, 150, 0);
    EVT_AT_SetVoice("0517A005", 1, 0);
    EVT_AT_SetVoice("0517B034", 1, 50); // @text EN: "Ryo!\nAre you all right?" | JA: "リョウ！　大丈夫？"
    EVT_AT_SetVoice("0517A043", 1, 90); // @text EN: "No=@\nit's too dark=@" | JA: "…こんなにまっ暗じゃ…"
    EVT_AT_SetVoice("0517B035", 1, 170); // @text EN: "We can't go on.\nIt's raining too hard." | JA: "ダメ…この雨じゃ、\nもう先に進めない…"
    EVT_AT_SetVoice("0517A044", 1, 290); // @text EN: "This is as far as we can go=@" | JA: "くそっ…ここまでか…"
    EVT_AT_MenCtrl({1, 30, 120, 0, 0, 0, 255, 0, 0, 0, 320});
    work.phase = 9;
    return 380;
}
} // namespace shenmue::s2::areas
