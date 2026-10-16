#include "s2/areas/area_ycpa.hpp"
#include "s2/camera.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr Vector4 actorPositions[14]{{.x = 269.20001220703125f, .y = 100, .z = 203.6999969482421875f, .w = 90},
    {.x = 270, .y = 100, .z = 202.8000030517578125f, .w = 135}, {.x = 250.899993896484375f, .y = 100.09999847412109375f, .z = 200, .w = 0},
    {.x = 251.8000030517578125f, .y = 100.09999847412109375f, .z = 200, .w = 22.5f}, {.x = 200.5f, .y = 100, .z = 181.8000030517578125f, .w = 45},
    {.x = 201.1999969482421875f, .y = 100, .z = 180.600006103515625f, .w = 45}, {.x = 182.3000030517578125f, .y = 100, .z = 130.6999969482421875f, .w = 270},
    {.x = 201.1999969482421875f, .y = 100, .z = 180.600006103515625f, .w = 45}, {.x = 221.5f, .y = 100, .z = 113, .w = 247.5f},
    {.x = 201.1999969482421875f, .y = 100, .z = 180.600006103515625f, .w = 45}, {.x = 292.100006103515625f, .y = 100, .z = 91.5f, .w = 337.5f},
    {.x = 201.1999969482421875f, .y = 100, .z = 180.600006103515625f, .w = 45},
    {.x = 295.899993896484375f, .y = 114.40000152587890625f, .z = 100.40000152587890625f, .w = 0},
    {.x = 201.1999969482421875f, .y = 100, .z = 180.600006103515625f, .w = 45}};
constexpr EventMovementWaypoint yuanRoute[21]{{.position = {.x = 261, .y = 100, .z = 204.5f}, .yawDegrees = 90},
    {.position = {.x = 251.8000030517578125f, .y = 100, .z = 202.600006103515625f}, .yawDegrees = 45},
    {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0}, {.position = {.x = 248.3000030517578125f, .y = 100, .z = 192.5f}, .yawDegrees = 90},
    {.position = {.x = 230.899993896484375f, .y = 100, .z = 190.600006103515625f}, .yawDegrees = 90},
    {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0}, {.position = {.x = 192.1999969482421875f, .y = 100, .z = 179.5f}, .yawDegrees = 22.5f},
    {.position = {.x = 187.5f, .y = 100, .z = 173.1999969482421875f}, .yawDegrees = 0}, {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0},
    {.position = {.x = 188.399993896484375f, .y = 100, .z = 127.6999969482421875f}, .yawDegrees = 320},
    {.position = {.x = 195.399993896484375f, .y = 100, .z = 121.3000030517578125f}, .yawDegrees = 340},
    {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0}, {.position = {.x = 230, .y = 100, .z = 114.09999847412109375f}, .yawDegrees = 270},
    {.position = {.x = 240.5f, .y = 100, .z = 114.40000152587890625f}, .yawDegrees = 300}, {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0},
    {.position = {.x = 292.5f, .y = 100, .z = 82.8000030517578125f}, .yawDegrees = 0},
    {.position = {.x = 291.70001220703125f, .y = 100, .z = 73.1999969482421875f}, .yawDegrees = 30}, {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0},
    {.position = {.x = 292.899993896484375f, .y = 114.40000152587890625f, .z = 93.90000152587890625f}, .yawDegrees = 0},
    {.position = {.x = 293, .y = 114.40000152587890625f, .z = 80.40000152587890625f}, .yawDegrees = 0},
    {.position = {.x = -1, .y = -1, .z = -1}, .yawDegrees = 0}};
void IdleMotion(shen_name character, std::uint32_t animation) {
    EventSetMotion cue{.cleanup = 0, .character = character, .animation = animation, .frame = 0, .speed = 1, .flags = YCPA_MOTION_NO_BLEND, .delay = 0};
    EVT_AT_SetMot_Callback(&cue);
    (void)GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
} // namespace

std::int32_t YcpaBriefConversation() {
    const std::int32_t index = ycpaEvent - 100;
    YcpaFilm(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    const Vector4 &player = actorPositions[index * 2];
    const Vector4 &companion = actorPositions[index * 2 + 1];
    EV_AT_SetPos(YCPA_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{.x = player.x, .y = player.y, .z = player.z});
    EV_AT_SetPos(YCPA_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{.x = companion.x, .y = companion.y, .z = companion.z});
    EV_AT_SetPos(YCPA_YUA, EventTransformFlags::World | EventTransformFlags::AllAxes, yuanRoute[index * 3].position);
    EVT_AT_SetAngY(YCPA_RYO, YCPA_INVALID, player.w + 180.0f, 0);
    EVT_AT_SetPos(YCPA_REN, YCPA_RYO, Vector3{.x = -1, .y = 0, .z = -0.5f}, 0);
    EVT_AT_SetAngY(YCPA_REN, YCPA_RYO, 227.5f, 0);
    EVT_AT_SetAngY(YCPA_YUA, YCPA_INVALID, yuanRoute[index * 3].yawDegrees, 0);
    AT_SetLocalBaseChara(YCPA_YUAB, YCPA_YUA);
    EVT_AT_SetPos(YCPA_RYOB, YCPA_RYO, Vector3{.x = 0, .y = 0, .z = 0}, 0);
    EVT_AT_SetAngY(YCPA_RYOB, YCPA_RYO, 0, 0);
    EVT_AT_SetPos(YCPA_RENB, YCPA_REN, Vector3{.x = 0, .y = 0, .z = 0}, 0);
    EVT_AT_SetAngY(YCPA_RENB, YCPA_REN, 0, 0);
    EVT_AT_SetPos(YCPA_YUAB, YCPA_YUA, Vector3{.x = 0, .y = 0, .z = 0}, 0);
    EVT_AT_SetAngY(YCPA_YUAB, YCPA_YUA, 0, 0);
    IdleMotion(YCPA_RYO, 10000);
    IdleMotion(YCPA_REN, 9989);
    IdleMotion(YCPA_YUA, 10048);
    YcpaCamera(3000, YCPA_RENB, YCPA_YUAB, 0, 0, 0);
    const std::int32_t choice = shenGetRandomInt32(3);
    if (!choice) {
        EVT_AT_SetVoice("0235B031", 20); // @text EN: "There she is!\nCome on!" | JA: "あんなとこにいやがる。行くぞ！"
        EVT_AT_SetVoice("0235A005", 100); // @text EN: "Yeah." | JA: "ああ。"
    } else if (choice == 1) {
        EVT_AT_SetVoice("0235B032", 20); // @text EN: "There's Yuan.\nKeep your eyes on her!" | JA: "おい、あれだ。\n見失うんじゃねえぞ！"
        EVT_AT_SetVoice("0235A019", 100); // @text EN: "Yeah.\nLet's go." | JA: "ああ。行こう。"
    } else if (choice == 2) {
        EVT_AT_SetVoice("0235B033", 0); // @text EN: "There she is.\nDon't get too close." | JA: "…いやがった。\n近づきすぎんなよ。"
        EVT_AT_SetVoice("0235A020", 100); // @text EN: "I know.\nLet's go." | JA: "わかってる。行こう。"
    }
    return 140;
}
} // namespace shenmue::s2::areas
