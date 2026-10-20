#include "s2/ren_qte_choreography.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include <cstring>

namespace shenmue::s2 {
// @verified - p
void CaptureRenQtePursuers(RenQteWork *work) {
    using enum CameraState;
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    EV_EnableCharacterDisplay(REN_QTE_KD7);
    EV_EnableCharacterDisplay(REN_QTE_KK1);
    EV_EnableCharacterDisplay(REN_QTE_KT1);
    EV_EnableCharacterDisplay(REN_QTE_KU7);
    EV_EnsureCharacterDraw(REN_QTE_KD7);
    EV_EnsureCharacterDraw(REN_QTE_KK1);
    EV_EnsureCharacterDraw(REN_QTE_KT1);
    EV_EnsureCharacterDraw(REN_QTE_KU7);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_KD7), 0);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_KK1), 0);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_KT1), 0);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_KU7), 0);
    const Vector3 basePosition{work->entry->endX, GetQronTemporaryFloorHeight(), work->entry->endZ};
    EV_AT_SetPos(REN_QTE_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, basePosition);
    EV_AT_SetAngY(REN_QTE_BASE, EventTransformFlags::AllAxes, Vector3i{0, work->entry->endYaw, 0});
    std::int32_t firstFrame;
    if (work->entry->type == 7) {
        SetRenQtePose(REN_QTE_PLAYER, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_PLAYER_POSITION);
        SetRenQtePose(REN_QTE_REN, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_REN_POSITION, 180);
        SetRenQtePose(REN_QTE_KD7, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_KD7_POSITION);
        SetRenQtePose(REN_QTE_KK1, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_KK1_POSITION);
        SetRenQtePose(REN_QTE_KT1, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_KT1_POSITION);
        SetRenQtePose(REN_QTE_KU7, REN_QTE_BASE, REN_QTE_DOOR_CAPTURE_KU7_POSITION);
        EVT_AT_SetCam(2001, REN_QTE_BASE, REN_QTE_BASE, 0, 0, 0);
        firstFrame = 80;
    } else {
        SetRenQtePose(REN_QTE_PLAYER, REN_QTE_BASE, REN_QTE_CAPTURE_PLAYER_POSITION);
        SetRenQtePose(REN_QTE_REN, REN_QTE_BASE, REN_QTE_CAPTURE_REN_POSITION);
        SetRenQtePose(REN_QTE_KD7, REN_QTE_BASE, REN_QTE_CAPTURE_KD7_POSITION);
        SetRenQtePose(REN_QTE_KK1, REN_QTE_BASE, REN_QTE_CAPTURE_KK1_POSITION);
        SetRenQtePose(REN_QTE_KT1, REN_QTE_BASE, REN_QTE_CAPTURE_KT1_POSITION, 180);
        SetRenQtePose(REN_QTE_KU7, REN_QTE_BASE, REN_QTE_CAPTURE_KU7_POSITION, 180);
        EVT_AT_SetCam(2000, REN_QTE_BASE, REN_QTE_BASE, 0, 0, 0);
        firstFrame = 0;
    }
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_CAPTURE, static_cast<float>(firstFrame));
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_CAPTURE, static_cast<float>(firstFrame));
    PlayRenQteMotion(REN_QTE_KD7, REN_QTE_PURSUER_CAPTURE_FIRST, 0, 1, 0);
    PlayRenQteMotion(REN_QTE_KK1, REN_QTE_PURSUER_CAPTURE_SECOND, 30, 1, 0);
    PlayRenQteMotion(REN_QTE_KT1, REN_QTE_PURSUER_CAPTURE_THIRD, 0, 1, 0);
    PlayRenQteMotion(REN_QTE_KU7, REN_QTE_PURSUER_CAPTURE_SECOND, 0, 1, 0);
    HLib::Task *const voiceTask = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, sizeof(EventSetVoice), REN_QTE_EVENT_TASK);
    RegisterEventTask(voiceTask);
    EventSetVoice *const voice = static_cast<EventSetVoice *>(HLib::GetTaskParameter(voiceTask));
    voice->cleanup = 1;
    voice->mode = 1;
    voice->delay = 10;
    std::strcpy(voice->voice, "0209B041");
    EVT_AT_SetVoice("0209B038", 1, 100 - (firstFrame >> 1)); // @text EN: "Came all the way for nothing." | JA: "くそったれが！\nここまで来て…"
    EVT_AT_SetVoice("0209F001", 1, 240 - firstFrame); // @text EN: "Giving us trouble!!" | JA: "てこずらせやがって。"
    if (work->entry->type == 7) {
        EVT_AT_SetVoice("0209G001", 1, 280 - firstFrame); // @text EN: "You won't be getting away now!" | JA: "もう逃がさねぇぞ！"
        EVT_AT_SetVoice("0209G002", 1, 320 - firstFrame); // @text EN: " " | JA: "オラオラオラッ！！"
    } else {
        EVT_AT_SetVoice("0209H001", 1, 280 - firstFrame); // @text EN: "Beat 'em!" | JA: "やっちまえっ！！"
        EVT_AT_SetVoice("0209H002", 1, 310 - firstFrame); // @text EN: "Damn kid!" | JA: "このガキ！"
    }
    EVT_AT_SetSound(0xb0c, 300 - firstFrame);
    EVT_AT_SetVoice("0209A023", 1, 345 - firstFrame);
    EVT_AT_MenCtrl({0, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(210 - firstFrame)});
    HLib::Task *const soundTask =
        HLib::EnqueueTaskWithoutParameter(RenQteFadeOutSoundCallback, 6, 0x0b, static_cast<std::int32_t>(REN_QTE_SOUND_FADE_TASK.name));
    HLib::SetupTimedTask(soundTask, 210 - firstFrame);
    work->state = REN_QTE_FINISH;
    work->timer = 435 - firstFrame;
}
} // namespace shenmue::s2
