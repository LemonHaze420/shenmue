#include "s2/areas/area_khin.hpp"
#include "s2/areas/khin_tables.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/save.hpp"

namespace shenmue::s2 {
std::int32_t KhinXhgDeposit(KhinWork &work) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
    EVT_AT_SetCam(4050, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {117.34f, 100.71f, 112.78f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 90, 0);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {115.79f, 100.74f, 112.7f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 0);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][0], 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][2], 0, 1, 0x80000000U, 100);
    EVT_AT_SetMot(KHIN_RYO, 0xf019, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 60);
    return 180;
}
std::int32_t KhinGstDeposit(KhinWork &work) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
    EVT_AT_SetCam(4150, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {210.35f, 104, 86.6f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 90, 0);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {208.7f, 103.95f, 86.4f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 0);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][0], 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][2], 0, 1, 0x80000000U, 100);
    EVT_AT_SetMot(KHIN_RYO, 0xf019, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 60);
    return 180;
}
std::int32_t KhinAgnDeposit(KhinWork &work) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
    EVT_AT_SetCam(4250, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {287.6f, 100.1f, 192.7f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 270, 0);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {289, 100, 192.8f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 0);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][0], 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][2], 0, 1, 0x80000000U, 100);
    EVT_AT_SetMot(KHIN_RYO, 0xf019, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 60);
    return 180;
}
std::int32_t KhinXhgCancel(KhinWork &work) {
    EVT_AT_SetCam(4500, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {117.25f, 100.75f, 116.8f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 5, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0xf023, 0, 1, 0x80000000U, 30);
    EVT_AT_SetVoice(work.row->voices[0], 1, 30);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 65);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 45, 65);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 74});
    return 90;
}
std::int32_t KhinGstCancel(KhinWork &work) {
    EVT_AT_SetCam(4600, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {205.7f, 103.95f, 90.4f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 5, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0xf028, 0, 1, 0x80000000U, 30);
    EVT_AT_SetVoice(work.row->voices[0], 1, 30);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 65);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 315, 65);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 74});
    return 90;
}
std::int32_t KhinAgnCancel(KhinWork &work) {
    EVT_AT_SetCam(4700, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 44});
    return 60;
}
std::int32_t KhinXhgLeave(KhinWork &work) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_SetCam(4510, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {116.25f, 100.75f, 116.8f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 34});
    return 50;
}
std::int32_t KhinGstLeave(KhinWork &work) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_SetCam(4610, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {205.6f, 103.95f, 90.4f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 0);
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 14});
    return 30;
}
std::int32_t KhinAgnLeave(KhinWork &) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 0);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 34});
    return 50;
}
std::int32_t KhinTokensGreeting(KhinWork &work) {
    EVT_AT_SetVoice(work.row->voices[0], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 0);
    return 40;
}
} // namespace shenmue::s2
