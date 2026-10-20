#include "s2/ren_qte_choreography.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
std::int32_t renQteRetryDifficulty{};
void SetRenQtePose(shen_name character, shen_name base, const Vector3 &position, float degrees) {
    EventSetPosition translation{0, character, base, position, 0};
    EVT_AT_SetPosRot(&translation);
    EventSetAngle angle{0, character, base, degrees, 0};
    EVT_AT_SetAngY_Callback(&angle);
}
std::int32_t PlayRenQteMotion(shen_name character, std::uint16_t animation, float frame, float speed, std::uint32_t flags) {
    EventSetMotion motion{0, character, animation, frame, speed, flags, 0};
    EVT_AT_SetMot_Callback(&motion);
    return static_cast<std::int32_t>(GetMotionFrameCount(animation));
}
// @verified - p
void PlayRenQteCameras(const RenQteCamera *cameras) {
    using enum CameraState;
    if (!cameras)
        return;
    if (!cameras->camera) {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    for (const RenQteCamera *camera = cameras; camera->camera; ++camera)
        EVT_AT_SetCam(camera->camera, camera->attachment, camera->attachment, 0, 0, camera->delay);
}
// @verified - p
void PlayRenQteVoices(const RenQteVoice *voices) {
    if (!voices)
        return;
    for (const RenQteVoice *voice = voices; voice->voice; ++voice)
        EVT_AT_SetVoice(voice->voice, 1, voice->delay);
}
// @verified - p
void RenQteFadeOutSoundCallback(void *) {
    spmFadeOut();
    HLib::DestroyCurrentTask();
}
// @verified - p
void RenQteUnusedResult(RenQteWork *) {
}
// @verified - p
bool UpdateRenQteEvent(RenQteWork *work) {
    if (work->timer)
        --work->timer;
    switch (work->state.name) {
    case SHEN_NAME(INIT):
        InitializeRenQteEntry(work);
        PlayRenQteCameras(work->entry->initialCameras);
        PlayRenQteVoices(work->entry->initialVoices);
        return false;
    case SHEN_NAME(STRT):
        WaitRenQteApproach(work);
        return false;
    case SHEN_NAME(QTE_):
        PollRenQteInput(work);
        return false;
    case SHEN_NAME(SUCC):
        renQteSuccessCallbacks[work->entry->type](work);

        PlayRenQteCameras(work->entry->successCameras);
        PlayRenQteVoices(work->entry->successVoices);
        camCtrlToggleSmoothing(0);
        return false;
    case SHEN_NAME(FAIL):
        --work->lives;
        renQteFailureCallbacks[work->entry->type](work);
        PlayRenQteCameras(work->entry->failureCameras);
        PlayRenQteVoices(work->entry->failureVoices);
        camCtrlToggleSmoothing(0);
        if (work->lives <= 0)
            work->state = REN_QTE_CAPTURE;
        return false;
    case SHEN_NAME(FINI):
        if (!work->timer) {
            work->state = REN_QTE_INIT;
            return true;
        }
        return false;
    case SHEN_NAME(CAPT):
        if (!work->timer) {
            work->state = REN_QTE_INIT;
            CaptureRenQtePursuers(work);
        }
        return false;
    default:
        return false;
    }
}
// @verified - p
void InitializeRenQteEntry(RenQteWork *work) {
    std::int32_t difficulty = work->retryCount;
    work->state = REN_QTE_START;
    work->doorAttempts = 0;
    if (difficulty > 4)
        difficulty = 4;
    renQteRetryDifficulty = difficulty;
    work->triggerDistance = static_cast<float>(difficulty) + 6.5F;
    Vector3 position;
    EV_GetCharaPosition(REN_QTE_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = work->entry->startX;
    position.z = work->entry->startZ;
    EV_AT_SetPos(work->base, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(work->base, EventTransformFlags::AllAxes, Vector3i{0, work->entry->startYaw, 0});
    AT_SetLocalBaseChara(REN_QTE_PLAYER, work->base);
    AT_SetLocalBaseChara(REN_QTE_REN, work->base);
    mtwkSetRequestedRootMode(scenFindMotionwork(REN_QTE_PLAYER), 3);
    mtwkSetRequestedRootMode(scenFindMotionwork(REN_QTE_REN), 3);
    SetMotionFrameTime(scenFindMotionwork(REN_QTE_PLAYER), 5, MotionRegion::LowerBody);
    SetMotionFrameTime(scenFindMotionwork(REN_QTE_REN), 5, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(REN_QTE_DOOR);
    EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    switch (work->entry->type) {
    case 7:
        BeginRenQteDoorApproach(work);
        break;
    case 8:
        BeginRenQteExitApproach(work);
        break;
    case 9:
        BeginRenQteRightExitApproach(work);
        break;
    default:
        BeginRenQteRunApproach(work);
        break;
    }
    camCtrlSetFovTransitionTo(true);
}
// @verified - p
void BeginRenQteDoorApproach(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, {0.4F, 0, work->entry->approachDistance});
    SetRenQtePose(REN_QTE_REN, work->base, {-0.4F, 0, work->entry->approachDistance + 0.25F});
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_DOOR_APPROACH);
    work->timer = frames;
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_DOOR_APPROACH, 0, 1, 0);
    hmdlSetFlagsFor(ctrlGetCharaByName(REN_QTE_PLAYER), 0, 0, 16);
    EV_EnableCharacterDisplay(REN_QTE_DOOR);
    EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    renQteDoorMotionPending = 1;
    work->lives = 3;
}
// @verified - p
void BeginRenQteExitApproach(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, {0.4F, 0, work->entry->approachDistance});
    SetRenQtePose(REN_QTE_REN, work->base, {-0.4F, 0, work->entry->approachDistance});
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_RUN);
    work->state = REN_QTE_START;
    work->triggerDistance = 0;
}
// @verified - p
void BeginRenQteRightExitApproach(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, {0.4F, 0, work->entry->approachDistance});
    SetRenQtePose(REN_QTE_REN, work->base, {-0.4F, 0, work->entry->approachDistance});
    work->state = REN_QTE_START;
    work->triggerDistance = 3;
}
// @verified - p
void BeginRenQteRunApproach(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, {0.4F, 0, work->entry->approachDistance});
    SetRenQtePose(REN_QTE_REN, work->base, {-0.4F, 0, work->entry->approachDistance});
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_RUN);
    work->timer = 0;
}
// @verified - p
void WaitRenQteApproach(RenQteWork *work) {
    if (work->timer)
        return;
    Vector3 position;
    EV_GetCharaPosition(REN_QTE_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (position.z > work->triggerDistance)
        return;
    work->timer = 4 * renQteRetryDifficulty + 12;
    switch (work->entry->type) {
    case 7:
        qteBattleUtil(BattleQteMode::SetWait, 1, 6, work->timer);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256);
        qteBattleUtil(BattleQteMode::Start);
        ++work->doorAttempts;
        break;
    case 8:
        FinishRenQteExit(work);
        return;
    case 9:
        FinishRenQteRightExit(work);
        return;
    default:
        SetShowQteFlag(work->entry->command, static_cast<std::int16_t>(work->timer), 2);
        break;
    }
    work->state = REN_QTE_INPUT;
}
// @verified - p
void FinishRenQteExit(RenQteWork *work) {
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, 0xf008);
    PlayRenQteMotion(REN_QTE_REN, 0xf008);
    EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, {0, -0.9F, 0}, frames - 1);
    EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, {0, -0.9F, 0}, frames - 1);
    EventFilmControl film{0, 20, 0, 0, 0, 0, 255, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    work->timer = 20;
    work->state = REN_QTE_FINISH;
}
// @verified - p
void FinishRenQteRightExit(RenQteWork *work) {
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_RIGHT);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_RIGHT);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), REN_QTE_EVENT_TASK);
    RegisterEventTask(task);
    EventFilmControl *const film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
    *film = {1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 30};
    work->timer = 60;
    work->state = REN_QTE_FINISH;
}
// @verified - p
void PollRenQteInput(RenQteWork *work) {
    Vector3 position;
    EV_GetCharaPosition(REN_QTE_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (GetShouldRenderIcons(RenderIconSlot::Qte))
        return;
    if (work->entry->type != 7) {
        work->state = (GetCurrentQteStatus() & 0xfff) == work->entry->command ? REN_QTE_SUCCESS : REN_QTE_FAILURE;
    } else {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (result == 1)
            work->state = REN_QTE_SUCCESS;
        else if (result == -1)
            work->state = REN_QTE_FAILURE;
    }
}
} // namespace shenmue::s2
