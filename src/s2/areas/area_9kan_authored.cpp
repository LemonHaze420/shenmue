#include "s2/areas/area_9kan.hpp"
#include "s2/camera.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>

namespace shenmue::s2::areas {
namespace {
void NineKanVoice(const char *voice, std::int32_t delay) {
    if (!delay) {
        EV_PlayDialogue(voice);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, sizeof(EventSetVoice), NINE_KAN_TASK);
    RegisterEventTask(task);
    EventSetVoice *const work = static_cast<EventSetVoice *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->mode = 1;
    work->delay = static_cast<float>(delay);
    std::strcpy(work->voice, voice);
}
void NineKanFilm(std::int32_t startAlpha, std::int32_t targetAlpha, std::int32_t delay) {
    EventFilmControl request{.cleanup = delay != 0,
        .frames = 30,
        .startAlpha = startAlpha,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = targetAlpha,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)};
    if (delay)
        EVT_AT_MenCtrl(request);
    else
        EVT_AT_MenCtrl_Callback(&request);
}
void NineKanQueueTinyMotion(std::uint32_t motion, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NineKanPlayTinyMotion, 4, 0x0b, sizeof(NineKanTinyMotion), NINE_KAN_TASK);
    HLib::SetupTimedTask(task, delay);
    NineKanTinyMotion *const work = static_cast<NineKanTinyMotion *>(HLib::GetTaskParameter(task));
    work->character = NINE_KAN_BIRD;
    work->motion = motion;
}
} // namespace
void NineKanInitialPlacement() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_CHO);
    EV_EnableCharacterDisplay(NINE_KAN_YUA);
    EV_DisableCharacterDisplay(NINE_KAN_TGY);
    EV_EnableCharacterDisplay(NINE_KAN_BIRD);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_INVALID, {.x = 250.35f, .y = 114.4f, .z = 62.2f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_INVALID, 90, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 13.8f, .y = 0, .z = 5.2f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 310, 0);
    EVT_AT_SetPos(NINE_KAN_CHO, NINE_KAN_DUMMY, {.x = 4.34f, .y = 0, .z = 10.1f}, 0);
    EVT_AT_SetAngY(NINE_KAN_CHO, NINE_KAN_DUMMY, 145, 0);
    EVT_AT_SetMot(NINE_KAN_CHO, 0x7f06, 0, 0, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_YUA, NINE_KAN_DUMMY, {.x = 12, .y = 0, .z = 6.72f}, 0);
    EVT_AT_SetAngY(NINE_KAN_YUA, NINE_KAN_DUMMY, 90, 0);
    EVT_AT_SetMot(NINE_KAN_YUA, 0x1faa, 0, 1, 0x40000000U, 0);
    ConfigureScenarioNodeMotion(NINE_KAN_BIRD, 0x1fb7, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(NINE_KAN_BIRD, 0, -1, -1, -1, 32767, 4);
}
std::int32_t NineKanBeginSearch() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_DisableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 11.15f, .y = 0, .z = 5.42f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 125, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 13.8f, .y = 0, .z = 10.38f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 170, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f88, 39, 1, 0x40000000U, 0);
    EVT_AT_SetCam(340, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetCam(350, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 40);
    EVT_AT_SetCam(360, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 80);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    return 110;
}
void NineKanEnterHidingPlace(std::int32_t state) {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_DisableCharacterDisplay(NINE_KAN_TGY);
    switch (state) {
    case 14:
        EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 4.14f, .y = 0, .z = 5.45f}, 0);
        EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 240, 0);
        EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa0, 50, 0, 0x40000000U, 0);
        EVT_AT_SetCam(2000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
        EVT_AT_SetCam(2010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 60);
        NineKanFilm(0, 255, 150);
        HLib::SetupTaskDelay(180);
        return;
    case 15:
        EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 4.57f, .y = 0, .z = 8.2f}, 0);
        EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 170, 0);
        EVT_AT_SetMot(NINE_KAN_RYO, 0x1f9e, 0, 1, 0x40000000U, 0);
        EVT_AT_SetCam(5500, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
        NineKanFilm(0, 255, 75);
        HLib::SetupTaskDelay(105);
        return;
    case 16:
        EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 11.42f, .y = 0, .z = 10.3f}, 0);
        EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 270, 0);
        EVT_AT_SetMot(NINE_KAN_RYO, 0x1f9e, 0, 1, 0, 0);
        EVT_AT_SetCam(5000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
        EVT_AT_SetCam(5010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 45);
        NineKanFilm(0, 255, 107);
        HLib::SetupTaskDelay(137);
        return;
    default:
        return;
    }
}
std::int32_t NineKanTogyuSearch() {
    EV_DisableCharacterDisplay(NINE_KAN_RYO);
    EV_DisableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    NineKanFilm(255, 0, 0);
    EVT_AT_SetCam(1000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 16.11f, .y = 0, .z = 6.97f}, 30);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 90, 30);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1faf, 0, 1, 0x40000000U, 30);
    nineKanEntryDoorTask = HLib::EnqueueTaskWithoutParameter(NineKanEntryDoor, 4, 0x0b, static_cast<std::int32_t>(NINE_KAN_TASK.name));
    HLib::SetupTimedTask(nineKanEntryDoorTask, 10);
    EVT_AT_SetCam(1015, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 100);
    NineKanVoice("0237E014", 140);
    EVT_AT_SetCam(80, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 316);
    NineKanQueueTinyMotion(0x1fb8, 316);
    NineKanVoice("0237B005", 317);
    EVT_AT_SetCam(3005, NINE_KAN_INVALID, NINE_KAN_INVALID, 0, 0, 422);
    NineKanQueueTinyMotion(0x1fb7, 422);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 11.88f, .y = 0, .z = 6.16f}, 422);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 0, 422);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fb5, 0, 1, 0, 422);
    NineKanVoice("0237E015", 422);
    EVT_AT_SetCam(3010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 589);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fb0, 30, 1, 0, 589);
    EVT_AT_SetCam(3020, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 661);
    NineKanVoice("0237E016", 661);
    NineKanQueueOpenDoor();
    EVT_AT_SetCam(3030, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 696);
    NineKanVoice("0237E017", 716);
    EVT_AT_SetCam(70, NINE_KAN_LEFT_DOOR, NINE_KAN_LEFT_DOOR, 0, 0, 723);
    EVT_AT_SetCam(3040, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 783);
    EVT_AT_ObjectDisp_1(NINE_KAN_REN, 783, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 10.71f, .y = 0, .z = 6.83f}, 783);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 0, 783);
    EVT_AT_SetMot(NINE_KAN_REN, 0xf003, 0, 1, 0x40000000U, 783);
    NineKanVoice("0237C053", 783);
    return 801;
}
std::int32_t NineKanSearchFailure() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetCam(1000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 10.1f, .y = 0, .z = 5.56f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 270, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa5, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 16.11f, .y = 0, .z = 6.97f}, 30);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 90, 30);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1faf, 0, 1, 0x40000000U, 30);
    nineKanEntryDoorTask = HLib::EnqueueTaskWithoutParameter(NineKanEntryDoor, 4, 0x0b, static_cast<std::int32_t>(NINE_KAN_TASK.name));
    HLib::SetupTimedTask(nineKanEntryDoorTask, 10);
    EVT_AT_SetCam(1010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 100);
    NineKanVoice("0237E014", 140);
    NineKanVoice("0237E018", 316);
    EVT_AT_SetCam(1020, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 413);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 11.82f, .y = 0, .z = 5.66f}, 413);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 90, 413);
    NineKanVoice("0237E019", 413);
    EVT_AT_SetCam(1030, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 519);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 90, 519);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 0, .3f, 0x40000000U, 519);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 0, .3f, 0x40000000U, 519);
    EVT_AT_SetCam(1040, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 594);
    QueueCameraShake(0, .3f, .7f, 624);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 50, 1, 0x40000000U, 594);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 50, 1, 0x40000000U, 594);
    NineKanVoice("0237A036", 594);
    NineKanVoice("0237E023", 612);
    NineKanVoice("0237A037", 642);
    NineKanFilm(0, 255, 702);
    EVT_AT_FadeBGM(0, 1, 702);
    return 732;
}
void NineKanComboSuccess() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetCam(4500, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 11.23f, .y = 0, .z = 5.73f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 0, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa6, 5, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 12.21f, .y = 0, .z = 5.98f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 0, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f8f, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 11.84f, .y = 0, .z = 4.2f}, 0);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 0, 0);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fb2, 0, 1, 0, 30);
    NineKanVoice("0237E024", 30);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 10.45f, .y = 0, .z = 5.3f}, 75);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 0, 75);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa5, 0, 1, 0x40000000U, 75);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 11.6f, .y = 0, .z = 5.14f}, 75);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 0, 75);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f95, 45, 1, 0x40000000U, 75);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f96, 0, 1, 0, 129);
    NineKanQueueCloseDoor();
    EVT_AT_SetCam(7000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 75);
    NineKanVoice("0237E026", 75);
    EVT_AT_SetSound(0x77c, 75);
    NineKanVoice("0237C056", 172);
    HLib::SetupTaskDelay(252);
}
std::int32_t NineKanComboFailure() {
    EV_DisableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetCam(4000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 11.71f, .y = 0, .z = 8.23f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 90, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f99, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 11.68f, .y = 0, .z = 5.15f}, 0);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 90, 0);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fb4, 0, 1, 0x40000000U, 0);
    NineKanVoice("0237C054", 0);
    NineKanVoice("0237E020", 35);
    NineKanVoice("0237C055", 125);
    EVT_AT_SetCam(4010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 130);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 12.18f, .y = 0, .z = 8.17f}, 130);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 0, 130);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa8, 0, 1, 0x40000000U, 130);
    NineKanVoice("0237E021", 130);
    EVT_AT_ObjectDisp_1(NINE_KAN_RYO, 130, 0);
    EVT_AT_ObjectDisp_1(NINE_KAN_TGY, 130, 1);
    EVT_AT_SetCam(4020, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 183);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 12.18f, .y = 0, .z = 5.57f}, 183);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 180, 183);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 0, .3f, 0x40000000U, 183);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 12.144f, .y = 0, .z = 3.815f}, 183);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 180, 183);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 0, .3f, 0x40000000U, 183);
    NineKanVoice("0237E022", 183);
    EVT_AT_ObjectDisp_1(NINE_KAN_TGY, 183, 0);
    EVT_AT_SetCam(4030, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 258);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 50, 1, 0x40000000U, 258);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 50, 1, 0x40000000U, 258);
    QueueCameraShake(0, .3f, .7f, 287);
    NineKanVoice("0237A036", 287);
    NineKanVoice("0237E023", 305);
    NineKanVoice("0237A037", 335);
    NineKanFilm(0, 255, 395);
    EVT_AT_FadeBGM(0, 1, 395);
    EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 0, 425);
    EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 180, 425);
    return 425;
}
void NineKanShelfSuccess() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetCam(7200, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 7.7f, .y = 0, .z = 4.2f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 270, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa9, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f95, 102, 1, 0x40000000U, 0);
    EVT_AT_SetCam(7210, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 60);
    QueueCameraShake(0, .3f, .7f, 92);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f97, 40, 2, 0x40000000U, 60);
    NineKanVoice("0237A037", 40);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetFlags, 1, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), NINE_KAN_TASK);
    RegisterEventTask(task);
    EventPlayDynamics *const work = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    work->resource = "TANA";
    work->parts = NINE_KAN_SHELF_PARTS;
    work->character = NINE_KAN_SHELF;
    work->endFrame = 109;
    work->trackCount = 1;
    work->frameStep = 2;
    work->currentFrame = 1;
    work->delay = 60;
    EVT_AT_SetSound(0x564, 60);
    EVT_AT_SetSound(0x690, 90);
    EVT_AT_SetCam(7300, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 105);
    NineKanFilm(0, 255, 120);
    HLib::SetupTaskDelay(150);
}
std::int32_t NineKanShelfFailure() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EVT_AT_SetCam(7100, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_REN, 180, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f98, 0, 1, 0x40000000U, 0);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_TGY, 180, 0);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fb1, 10, 1, 0x40000000U, 0);
    NineKanVoice("0237C055", 0);
    nineKanBurstDoorTask = HLib::EnqueueTaskWithParameter(NineKanBurstDoor, 4, 0x0b, 4, NINE_KAN_TASK);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(nineKanBurstDoorTask)) = 0;
    NineKanVoice("0237E021", 18);
    EVT_AT_ObjectDisp_1(NINE_KAN_REN, 30, 1);
    EVT_AT_SetCam(4020, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 30);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 12.18f, .y = 0, .z = 5.57f}, 30);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 180, 30);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 0, .3f, 0x40000000U, 30);
    EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_DUMMY, {.x = 12.144f, .y = 0, .z = 3.815f}, 30);
    EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_DUMMY, 180, 30);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 0, .3f, 0x40000000U, 30);
    NineKanVoice("0237E022", 71);
    EVT_AT_ObjectDisp_1(NINE_KAN_TGY, 30, 0);
    EVT_AT_SetCam(4030, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 105);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa3, 50, 1, 0x40000000U, 105);
    EVT_AT_SetMot(NINE_KAN_TGY, 0x1fae, 50, 1, 0x40000000U, 105);
    QueueCameraShake(0, .3f, .7f, 134);
    NineKanVoice("0237A036", 134);
    NineKanVoice("0237E023", 152);
    NineKanVoice("0237A037", 182);
    NineKanFilm(0, 255, 242);
    EVT_AT_FadeBGM(0, 1, 242);
    EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 0, 272);
    EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 180, 272);
    return 272;
}
void NineKanEscape() {
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnableCharacterDisplay(NINE_KAN_TGY);
    EV_EnableCharacterDisplay(NINE_KAN_CHO);
    NineKanFilm(255, 0, 0);
    EVT_AT_SetCam(6000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 6.16f, .y = 0, .z = 5.28f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 35, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa4, 0, 0, 0x40000000U, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1fa4, 30, 1, 0x40000000U, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 15, 0);
    EVT_AT_SetPos(NINE_KAN_CHO, NINE_KAN_RYO, {.x = .25f, .y = 0, .z = -.28f}, 1);
    EVT_AT_SetAngY(NINE_KAN_CHO, NINE_KAN_RYO, 0, 1);
    EVT_AT_SetMot(NINE_KAN_CHO, 0x1fab, 0, 0, 0x40000000U, 0);
    EVT_AT_SetMot(NINE_KAN_CHO, 0x1fab, 30, 1, 0x40000000U, 0);
    NineKanVoice("0237C057", 30);
    EVT_AT_SetCam(6010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 185);
    EVT_AT_ObjectDisp_1(NINE_KAN_RYO, 185, 1);
    EVT_AT_ObjectDisp_1(NINE_KAN_CHO, 185, 1);
    NineKanVoice("0237C058", 185);
    NineKanVoice("0237B006", 247);
    NineKanVoice("0237E027", 317);
    NineKanFilm(0, 255, 317);
    NineKanVoice("0237C059", 405);
    EVT_AT_FadeBGM(0, 1, 449);
    HLib::SetupTaskDelay(479);
}
std::int32_t NineKanRepeatBox() {
    srchSetLookMode();
    SetDisplayFlagForProp(NINE_KAN_PROP, 0);
    SetLowQualityHands(ctrlGetCharaByName(NINE_KAN_RYO));
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_RYO), 1, 1, 0);
    SetLowQualityHands(ctrlGetCharaByName(NINE_KAN_REN));
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_REN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(NINE_KAN_REN), 0, reinterpret_cast<std::byte *>(nineKanRightHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(NINE_KAN_REN), 1, reinterpret_cast<std::byte *>(nineKanLeftHandPose), 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(NINE_KAN_REN), 1, 21, &NINE_KAN_HAND_ROTATION);
    EV_EnableCharacterDisplay(NINE_KAN_RYO);
    EV_EnableCharacterDisplay(NINE_KAN_REN);
    EV_EnsureCharacterDraw(NINE_KAN_BOX);
    EV_EnableCharacterDisplay(NINE_KAN_BOX);
    EVT_AT_SetCam(8000, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
    EVT_AT_SetPos(NINE_KAN_RYO, NINE_KAN_DUMMY, {.x = 6.162f, .y = 0, .z = 6.52f}, 0);
    EVT_AT_SetAngY(NINE_KAN_RYO, NINE_KAN_DUMMY, 200, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0x1f9c, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 8.53f, .y = 0, .z = 4.84f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 180, 0);
    EVT_AT_SetMot(NINE_KAN_REN, 0x1f86, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_BOX, NINE_KAN_INVALID, {.x = 244, .y = 115.147f, .z = 63.7f}, 0);
    EVT_AT_SetAngY(NINE_KAN_BOX, NINE_KAN_INVALID, 315, 0);
    NineKanVoice("0237C060", 30);
    NineKanVoice("0237A038", 75);
    EVT_AT_SetCam(8001, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 136);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), NINE_KAN_TASK);
    RegisterEventTask(task);
    EventCling *cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    *cling = {.cleanup = 1,
        .target = NINE_KAN_BOX,
        .character = NINE_KAN_REN,
        .bone = 14,
        .position = {.x = .126f, .y = .035f, .z = -.054f},
        .rotation = {.x = 20206, .y = 2184, .z = 34224},
        .mode = 0,
        .delay = 136};
    EVT_AT_SetCam(8002, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 150);
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), NINE_KAN_TASK);
    RegisterEventTask(task);
    cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    *cling = {.cleanup = 1,
        .target = NINE_KAN_BOX,
        .character = NINE_KAN_REN,
        .bone = 21,
        .position = {.x = .15f, .y = .0301f, .z = .053f},
        .rotation = {.x = 16014, .y = 0, .z = 42585},
        .mode = 0,
        .delay = 150};
    nineKanOpenBoxTask = HLib::EnqueueTaskWithParameter(NineKanOpenBox, 4, 0x0b, 4, NINE_KAN_TASK);
    HLib::SetupTimedTask(nineKanOpenBoxTask, 151);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(nineKanOpenBoxTask)) = 15;
    NineKanVoice("0237C061", 193);
    nineKanCloseBoxTask = HLib::EnqueueTaskWithParameter(NineKanCloseBox, 4, 0x0b, 4, NINE_KAN_TASK);
    HLib::SetupTimedTask(nineKanCloseBoxTask, 207);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(nineKanCloseBoxTask)) = 15;
    EVT_AT_SetCam(8003, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 250);
    NineKanVoice("0237A039", 380);
    NineKanFilm(0, 255, 450);
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), NINE_KAN_TASK);
    RegisterEventTask(task);
    cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = NINE_KAN_BOX;
    cling->character = shen_name{};
    cling->mode = 1;
    cling->delay = 480;
    EVT_AT_ObjectDisp_1(NINE_KAN_BOX, 480, 1);
    return 480;
}
void NineKanRepeatFinish() {
    srchSetTargetEnabled(NINE_KAN_PROP, 0);
    EVT_AT_SetMot(NINE_KAN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(NINE_KAN_REN, NINE_KAN_DUMMY, {.x = 13.8f, .y = 0, .z = 5.2f}, 0);
    EVT_AT_SetAngY(NINE_KAN_REN, NINE_KAN_DUMMY, 310, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_RYO), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_REN), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(NINE_KAN_REN), 0, 1, 0);
}
} // namespace shenmue::s2::areas
