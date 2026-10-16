#include "s2/areas/area_khin.hpp"
#include "s2/areas/khin_tables.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
void KhinDisplayTokensCallback(void *) {
    AddThisToCashOrSegaCoins(0, ECashOrCoins::Coins);
    HLib::DestroyCurrentTask();
}
std::int32_t KhinXhgWithdrawal(KhinWork &work) {
    EVT_AT_MenCtrl({.frames = 16, .startAlpha = 255});
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {117.96f, 100.74f, 116.81f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 0);
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {117.34f, 100.71f, 112.78f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 90, 0);
    EVT_AT_SetCam(4000, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(4005, KHIN_RYO, KHIN_RYO, 0, 0, 40);
    EVT_AT_SetCam(4010, KHIN_RYO, KHIN_RYO, 0, 0, 100);
    EVT_AT_SetCam(4020, KHIN_RYO, KHIN_RYO, 0, 0, 180);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 40);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 5, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetVoice(work.row->voices[0], 1, 100);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][1], 0, 1, 0x80000000U, 100);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 100, 1);
    HLib::Task *tokens = HLib::EnqueueTaskWithoutParameter(KhinDisplayTokensCallback, 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(tokens, 180);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {115.79f, 100.74f, 112.7f}, 180);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 180);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 180, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0x2b84, 100, 1, 0x40000000U, 180);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 180, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 180);
    EVT_AT_SetVoice(work.row->voices[2], 1, 240);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 324});
    EVT_AT_SetPoint(KHIN_RYO, KHIN_RYO, 5, 1, 0, 340, 0, 0x40000000U);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {115.79f, 100.74f, 112.5f}, 340);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 180, 340);
    return 340;
}
std::int32_t KhinGstWithdrawal(KhinWork &work) {
    EVT_AT_MenCtrl({.frames = 16, .startAlpha = 255});
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {205, 103.95f, 90.5f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 300, 0);
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {210.35f, 104, 86.6f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 90, 0);
    EVT_AT_SetCam(4100, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(4110, KHIN_RYO, KHIN_RYO, 0, 0, 80);
    EVT_AT_SetCam(4120, KHIN_RYO, KHIN_RYO, 0, 0, 160);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 40);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 5, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetVoice(work.row->voices[0], 1, 80);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][1], 0, 1, 0x80000000U, 80);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 80, 1);
    HLib::Task *tokens = HLib::EnqueueTaskWithoutParameter(KhinDisplayTokensCallback, 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(tokens, 160);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {208.5f, 103.95f, 86.4f}, 160);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 160);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 160, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0x2b84, 100, 1, 0x40000000U, 160);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 160, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 160);
    EVT_AT_SetVoice(work.row->voices[2], 1, 220);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 289});
    EVT_AT_SetPoint(KHIN_RYO, KHIN_RYO, 5, 1, 0, 305, 0, 0x40000000U);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {208.1f, 103.95f, 86.4f}, 305);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 270, 305);
    return 305;
}
std::int32_t KhinAgnWithdrawal(KhinWork &work) {
    EVT_AT_MenCtrl({.frames = 16, .startAlpha = 255});
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {287.8f, 100, 196.2f}, 0);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 320, 0);
    EVT_AT_SetPos(work.row->character, KHIN_INVALID, {287.4f, 100, 192.59f}, 0);
    EVT_AT_SetAngY(work.row->character, KHIN_INVALID, 270, 0);
    EVT_AT_SetCam(4200, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(4210, KHIN_RYO, KHIN_RYO, 0, 0, 80);
    EVT_AT_SetCam(4220, KHIN_RYO, KHIN_RYO, 0, 0, 160);
    EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPoint(work.row->character, KHIN_RYO, 22, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 40);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 5, 1, 0, 0, 0, 0x20000000U);
    EVT_AT_SetPoint(work.row->character, KHIN_RYO, 5, 1, 0, 80, 0, 0x40000000U);
    EVT_AT_SetVoice(work.row->voices[0], 1, 80);
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][1], 0, 1, 0x40000000U, 80);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 80, 1);
    HLib::Task *tokens = HLib::EnqueueTaskWithoutParameter(KhinDisplayTokensCallback, 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(tokens, 160);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {289, 100, 192.8f}, 160);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 160);
    EVT_AT_SetPoint(KHIN_RYO, work.row->character, 22, 1, 0, 160, 0, 0x20000000U);
    EVT_AT_SetMot(KHIN_RYO, 0x2b84, 100, 1, 0x40000000U, 160);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 160, 0);
    EVT_AT_SetVoice(work.row->voices[1], 1, 190);
    EVT_AT_SetVoice(work.row->voices[2], 1, 280);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 304});
    EVT_AT_SetPoint(KHIN_RYO, KHIN_RYO, 5, 1, 0, 320, 0, 0x40000000U);
    EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {289, 100, 192.8f}, 320);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 90, 320);
    return 320;
}
} // namespace shenmue::s2
