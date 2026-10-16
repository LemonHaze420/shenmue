#include "s2/areas/area_qdtg.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2 {
namespace {
const std::int32_t qdtgDefaultEntry = 0;
void StartTogyuCallback(void *);
void AuthorWaitCallback(void *);
void RunRyoRenCallback(void *);
void Qte2Callback(void *);
void Qte0Callback(void *);
void Qte0ResultCallback(void *);
void ComQteCallback(void *);
void ComQteResultCallback(void *);
void Author1Callback(void *);
void FloorAuthorCallback(void *);
void StartFloorBattleCallback(void *);
void FloorBattleCallback(void *);
void WinAuthorCallback(void *);
void RetryTogyuCallback(void *);
void Author2Callback(void *);
void FinishTogyuCallback(void *);
void FinishAuthorThunk(void *data) {
    FinishTogyuCallback(data);
}
void QdtgAdmissionNullsub() {
}

std::int32_t QueueBoxCollapse(shen_name character, shen_name hidden, std::int32_t delay) {
    if (character.name != EVENT_CHARACTER_INVALID.name)
        EVT_AT_ObjectDisp_1(character, delay, 0);
    if (hidden.name != EVENT_CHARACTER_INVALID.name)
        EVT_AT_ObjectDisp_1(hidden, delay, 1);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), QDTG_TASK);
    RegisterEventTask(task);
    EventPlayDynamics *const work = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    work->resource = "BOX0";
    work->character = character;
    work->endFrame = 498;
    work->trackCount = 10;
    work->delay = static_cast<float>(delay);
    work->parts = QDTG_BOX_PARTS;
    work->frameStep = 1;
    work->currentFrame = 2;
    return delay + 498;
}
std::int32_t QueueFloorCollapse(shen_name character, shen_name hidden, std::int32_t delay) {
    if (character.name != EVENT_CHARACTER_INVALID.name)
        EVT_AT_ObjectDisp_1(character, delay, 0);
    if (hidden.name != EVENT_CHARACTER_INVALID.name)
        EVT_AT_ObjectDisp_1(hidden, delay, 1);
    EV_SetObjectFlagByIndex(1, 0);
    Queue_EVT_AT_PlayDynamix(character, "YUKA", 2, 56, 76, nullptr, 2, delay);
    Queue_EVT_AT_PlayDynamix(character, "YUKA", 12, 250, 76, nullptr, 2, delay + 28);
    EVT_AT_SetPos(QDTG_YUKA, QDTG_YUKA, {0, -1, 0}, delay + 59);
    EVT_AT_SetAngY(QDTG_YUKA, QDTG_YUKA, 0, delay + 59);
    EVT_AT_SetPos(QDTG_YUKB, QDTG_YUKA, {0, -3.5999999f, 0}, delay);
    EVT_AT_SetAngY(QDTG_YUKB, QDTG_YUKA, 0, delay);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), QDTG_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    EventSmokeWork *const work = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->profile = 1;
    work->character = QDTG_YUKB;
    work->bone = 0xffffffffU;
    work->startDelay = 2;
    work->remaining = 0;
    work->finishDelay = 140;
    work->birthFraction = .05f;
    work->birthStep = .05f;
    return delay + 146;
}
std::int32_t DoRyoRenSequence() {
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-17, 0, 23}, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, 0, 0);
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-17, 0, 12.8f}, 37);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, 0, 37);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-18.200001f, 0, 18.5f}, 0);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, 0, 0);
    EVT_AT_SetCam(7638, QDTG_BASE, QDTG_BASE, 0, 0, 0);
    EVT_AT_SetCam(7639, QDTG_BASE, QDTG_BASE, 0, 0, 37);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 0);
    const std::int32_t renTime = EVT_AT_SetMot(QDTG_REN, QDTG_ARM_D, 0, 1, 0x20000000U, 25);
    EVT_AT_SetAngY(QDTG_REN, QDTG_REN, -90, renTime);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0, renTime);
    EVT_AT_SetMot(QDTG_RYO, QDTG_RYO_RUN, 0, 1, 0x40000000U, 0);
    const std::int32_t time = EVT_AT_SetMot(QDTG_RYO, QDTG_ARM_D, 0, 1, 0x60000000U, 37);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QDTG_RYO), FacialExpressionId::Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QDTG_REN), FacialExpressionId::Angry, 1, 1);
    return time;
}
std::int32_t Qte2MotionSuccess() {
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-12.2f, 0, 10.7f}, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, -90, 0);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-12.5f, 0, 9.6000004f}, 0);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, -90, 0);
    EVT_AT_SetPos(QDTG_RYO, QDTG_RYO, {0, 0, 3}, 40);
    EVT_AT_SetCam(7640, QDTG_BASE, QDTG_BASE, 0, 0, 0);
    EVT_AT_SetCam(7641, QDTG_BASE, QDTG_BASE, 0, 0, 40);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QDTG_REN, QDTG_ARM_D, 0, 1, 0, 37);
    const std::int32_t time = EVT_AT_SetMot(QDTG_RYO, QDTG_ARM_E, 0, 1, 0x60000000U, 0);
    EVT_AT_SetMot(QDTG_RYO, QDTG_RYO_RUN, 0, 1, 0, time);
    EVT_AT_SetVoice("0282A005", 1, 0); // @text EN: " " | JA: "ふっ…！"
    return 73;
}
std::int32_t Qte2MotionFailure() {
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-14.2f, 0, 10.7f}, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, -90, 0);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-12.5f, 0, 9.6000004f}, 0);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, -90, 0);
    EVT_AT_SetCam(7640, QDTG_BASE, QDTG_BASE, 0, 0, 0);
    EVT_AT_SetCam(7641, QDTG_BASE, QDTG_BASE, 0, 0, 40);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QDTG_REN, QDTG_ARM_D, 0, 1, 0, 37);
    const std::int32_t time = EVT_AT_SetMot(QDTG_RYO, QDTG_CRUSH, 30, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QDTG_RYO, QDTG_RYO_RUN, 0, 1, 0, time);
    EVT_AT_SetVoice("0282A014", 1, 0); // @text EN: " " | JA: "…くっ！"
    return 73;
}
std::int32_t Qte0MotionSuccess() {
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-2, 0, 9.5f}, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, -90, 0);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-.80000001f, 0, 7.8000002f}, 0);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, 0, 0);
    EVT_AT_SetCam(7642, QDTG_BASE, QDTG_BASE, 0, 0, 0);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 0);
    const std::int32_t time = EVT_AT_SetMot(QDTG_RYO, QDTG_ARM_D, 0, 1, 0x40000000U, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_RYO, 90, time);
    EVT_AT_SetMot(QDTG_RYO, QDTG_RYO_RUN, 0, 1, 0, time);
    EVT_AT_SetVoice("0282A005", 1, 0); // @text EN: " " | JA: "ふっ…！"
    return 70;
}
std::int32_t Qte0MotionFailure() {
    EVT_AT_SetPos(QDTG_RYO, QDTG_BASE, {-1.9f, 0, 9.5f}, 0);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_BASE, -90, 0);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-.80000001f, 0, 7.8000002f}, 0);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, 0, 0);
    EVT_AT_SetPos(QDTG_REN, QDTG_BASE, {-.80000001f, 0, 7.8000002f}, 100);
    EVT_AT_SetAngY(QDTG_REN, QDTG_BASE, 0, 100);
    EVT_AT_SetCam(7643, QDTG_BASE, QDTG_BASE, 0, 0, 0);
    EVT_AT_SetCam(7644, QDTG_BASE, QDTG_BASE, 0, 0, 120);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QDTG_REN, QDTG_REN_RUN, 0, 1, 0x40000000U, 120);
    EVT_AT_SetMot(QDTG_RYO, QDTG_COIN_TOSS, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QDTG_RYO, QDTG_COIN_TOSS, 30, 1.2f, 0, 30);
    EVT_AT_SetAngY(QDTG_RYO, QDTG_RYO, 90, 120);
    EVT_AT_SetMot(QDTG_RYO, QDTG_RYO_RUN, 0, 1, 0x40000000U, 120);
    QueueBoxCollapse(QDTG_BOX0, EVENT_CHARACTER_INVALID, 30);
    EVT_AT_SetVoice("0282A013", 1, 10); // @text EN: " " | JA: "うわっ！?"
    return 150;
}
void RunRyoRenCallback(void *) {
    for (shen_name name : {QDTG_RYO, QDTG_REN}) {
        Character *const character = ctrlGetCharaByName(name);
        SetHighModelXYZ(character, 1, 0, 0);
        SetHighModelXYZ(character, 1, 1, 0);
        ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
        ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
    }
    const std::int32_t time = DoRyoRenSequence();
    EVT_AT_QTE(64, 8, 0, time - 29);
    HLib::SetCurrentTaskTo(Qte2Callback);
    HLib::SetupTaskDelay(time);
}
void Qte2Callback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    std::int32_t time;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x40) {
        work->state = 1;
        time = Qte2MotionSuccess();
    } else {
        work->state = 0;
        time = Qte2MotionFailure();
    }
    EVT_AT_QTE(64, 8, 0, time - 29);
    HLib::SetCurrentTaskTo(Qte0Callback);
    HLib::SetupTaskDelay(time);
}
void Qte0Callback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    std::int32_t time;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x40) {
        ++work->state;
        time = Qte0MotionSuccess();
    } else
        time = Qte0MotionFailure();
    HLib::SetCurrentTaskTo(Qte0ResultCallback);
    HLib::SetupTaskDelay(time);
}
void Qte0ResultCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (work->state >= 2) {
        EV_stepSetFlags(0, 0x15f);
        work->state = 1;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_QTES.name));
        work->continuation = FinishAuthorThunk;
        HLib::SetCurrentTaskTo(Author1Callback);
    } else {
        work->state = 0;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_QTEF.name));
        work->continuation = ComQteResultCallback;
        HLib::SetCurrentTaskTo(ComQteCallback);
    }
}
void ComQteCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        for (shen_name name : {QDTG_RYO, QDTG_REN, QDTG_SOY, QDTG_SEE})
            SetPluralControlFor(ctrlGetCharaByName(name), 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 2048);
        EVT_AT_QTE(0, 16, 4, 0);
        HLib::SetCurrentTaskTo(work->continuation);
    }
}
void ComQteResultCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (!qteBattleUtil(BattleQteMode::GetResult))
        return;
    work->retrySkip = 1;
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 2) {
        EV_stepSetFlags(1, 0x15f);
        work->state = 1;
        qteBattleUtil(BattleQteMode::RetireAndFree);
        EVT_AT_ObjectDisp_1(QDTG_HIBI, 0, 0);
        QueueFloorCollapse(QDTG_YUKA, QDTG_HIBI, 64);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_TOGS.name));
        work->continuation = FinishAuthorThunk;
        HLib::SetCurrentTaskTo(Author1Callback);
    } else {
        work->state = 0;
        qteBattleUtil(BattleQteMode::RetireAndFree);
        EVT_AT_ObjectDisp_1(QDTG_YUKA, 193, 1);
        EVT_AT_ObjectDisp_1(QDTG_YUKB, 193, 0);
        EVT_AT_ObjectDisp_1(QDTG_HIBI, 0, 0);
        QueueFloorCollapse(QDTG_YUKA, QDTG_HIBI, 64);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        if (work->loss || work->authorState)
            EV_SetAuthorActiveTo(1);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_TOGF.name));
        work->continuation = StartFloorBattleCallback;
        HLib::SetCurrentTaskTo(FloorAuthorCallback);
    }
}
void Author1Callback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        for (shen_name name : {QDTG_RYO, QDTG_REN, QDTG_SOY, QDTG_SEE})
            SetPluralControlFor(ctrlGetCharaByName(name), 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->continuation);
        HLib::SetupTaskDelay(60);
    }
}
void FloorAuthorCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        for (shen_name name : {QDTG_RYO, QDTG_REN, QDTG_SOY, QDTG_SEE})
            SetPluralControlFor(ctrlGetCharaByName(name), 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EV_CleanupMapControlTask();
        if (GetAuthorFlags()) {
            EV_DisableCharacterDisplay(QDTG_YUKA);
            EV_EnableCharacterDisplay(QDTG_YUKB);
        }
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->continuation);
        HLib::SetupTaskDelay(60);
    }
}
void StartFloorBattleCallback(void *) {
    EventSetPosition position{.cleanup = 0, .character = QDTG_A896, .parent = QDTG_YUKB, .position = {0, 0, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = QDTG_A896, .parent = QDTG_YUKB, .rotation = {0, 0, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    EV_EnableCharacterDisplay(QDTG_A896);
    EV_DisableCharacterDisplay(QDTG_YUKB);
    EventSetMotion motion{.cleanup = 0, .character = QDTG_TGY, .animation = QDTG_TOGYU_STUCK, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(QDTG_TOGYU_STUCK);
    motion.character = QDTG_SEE;
    motion.animation = QDTG_SEE_STANCE;
    motion.flags = 0x20000000U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(QDTG_SEE_STANCE);
    angle.parent = EVENT_CHARACTER_INVALID;
    angle.character = QDTG_RYO;
    angle.rotation = {0, 32768, 0};
    EVT_AT_SetAng_Callback(&angle);
    angle.character = QDTG_SOY;
    angle.rotation = {0, 2912, 0};
    EVT_AT_SetAng_Callback(&angle);
    angle.character = QDTG_SEE;
    angle.rotation = {0, 65141, 0};
    EVT_AT_SetAng_Callback(&angle);
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(ctrlGetCharaByName(QDTG_RYO), nullptr, 0, 0, 0, 0);
    for (shen_name name : {QDTG_SOY, QDTG_SEE})
        shenInitFightData(ctrlGetCharaByName(name), nullptr, 1, 1, 200, 2);
    vfCopyPosFromEntity(0, nullptr, 195.8f, 173.8f, 6.5f);
    vfmQueueFightStart(3, 2, 1, 1, -1);
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{2}, 1);
    EV_LoadAudio("0282E000", 0);
    EV_spmBeginSampleExecute();
    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
    EVT_AT_MenCtrl_Callback(&film);
    HLib::SetCurrentTaskTo(FloorBattleCallback);
}
void FloorBattleCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (IsFightActive()) {
        Vector3 ryo, togyu;
        EV_GetCharaPosition(QDTG_RYO, 0xffffffffU, &ryo, EventTransformFlags::World);
        EV_GetCharaPosition(QDTG_TGY, 0xffffffffU, &togyu, EventTransformFlags::World);
        if (GetHorizontalDistance(&ryo, &togyu) < 1) {
            check_dead_status(1);
            if (EVT_GetQteFailureCount() < 2)
                EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
            SetCurrentDateTimeTo(eventTemporaryDateTime);
            shenSetTimeSimulationTo(1);
            EVT_AT_SetPos(QDTG_SOY, QDTG_BASE, {1000, 0, 0}, 1);
            EVT_AT_SetPos(QDTG_SEE, QDTG_BASE, {1000, 0, 0}, 1);
            if (work->authorState == 1)
                EV_SetAuthorActiveTo(1);
            work->authorState = 1;
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_TOG0.name));
            work->continuation = RetryTogyuCallback;
            HLib::SetCurrentTaskTo(Author2Callback);
        }
    } else {
        if (GetBtlStatus(ctrlGetCharaByName(QDTG_RYO)) == 3) {
            EV_stepSetFlags(2, 0x15f);
            HLib::SetCurrentTaskTo(WinAuthorCallback);
        } else {
            work->loss = 1;
            if (EVT_GetQteFailureCount() < 2)
                EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
            SetCurrentDateTimeTo(eventTemporaryDateTime);
            shenSetTimeSimulationTo(1);
            Queue_ST_LoadTitleFadeIn(QDTG_RETRY_TITLE);
            HLib::SetCurrentTaskTo(RetryTogyuCallback);
        }
        EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 0, .targetAlpha = 255};
        EVT_AT_MenCtrl_Callback(&film);
        spmFadeOut();
        HLib::SetupTaskDelay(60);
    }
}
void WinAuthorCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    EV_LoadCutsceneFromCurrentScene("0282E000");
    EV_LoadAudio("0282E000", 0);
    EV_spmBeginSampleExecute();
    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
    EVT_AT_MenCtrl_Callback(&film);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_WIN0.name));
    work->continuation = FinishAuthorThunk;
    HLib::SetCurrentTaskTo(Author1Callback);
}
void RetryTogyuCallback(void *) {
    ST_LoadTitleFadeIn();
    HLib::SetCurrentTaskTo(StartTogyuCallback);
    HLib::SetupTaskDelay(30);
}
void Author2Callback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Queue_ST_LoadTitleFadeIn(QDTG_RETRY_TITLE);
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->continuation);
        HLib::SetupTaskDelay(60);
    }
}
void CleanupTogyu(QdtgEventWork *work) {
    for (std::size_t i = 0; i < 5; ++i)
        SetPluralControlFor(ctrlGetCharaByName(QDTG_CHARACTERS[i]), 0);
    hndlSetFlags(ctrlGetCharaByName(QDTG_TGY));
    hndlSetFlags(ctrlGetCharaByName(QDTG_REN));
    hndlSetFlags(ctrlGetCharaByName(QDTG_RYO));
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0282");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDTG"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0282"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("282E"), 0);
    SetNaviDisplayOverride(work->savedNavi != 0);
    DeleteMotion(work->motion);
    ET_AT_KillWork(work->characters);
    EV_NPC_NewLoadSw(1);
}
void FinishTogyuCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    const std::int32_t state = work->state;
    CleanupTogyu(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    if (g_currentMap.name == AREA_QDTG.name) {
        scenSetTaskFlags(8);
        EV_scenQueueLoad(g_actNumber, QDTG_RETURN_AREA, state ? 56 : 57);
    }
}
void AuthorWaitCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EVT_AT_SetPos(QDTG_TGY, EVENT_CHARACTER_INVALID, {1000, 0, 0}, 0);
        EVT_AT_SetAngY(QDTG_TGY, EVENT_CHARACTER_INVALID, 0, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(QDTG_LIGHT);
        PushGlobalLightingProgram();
        EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
        EVT_AT_MenCtrl_Callback(&film);
        HLib::SetCurrentTaskTo(work->continuation);
    }
}
void StartTogyuCallback(void *data) {
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(data);
    work->continuation = RunRyoRenCallback;
    SetLowQualityHands(ctrlGetCharaByName(QDTG_RYO));
    SetLowQualityHands(ctrlGetCharaByName(QDTG_REN));
    SetLowQualityHands(ctrlGetCharaByName(QDTG_TGY));
    EV_DisableCharacterDisplay(QDTG_YUKB);
    EV_DisableCharacterDisplay(QDTG_A896);
    EVT_AT_SetPos(QDTG_BASE, EVENT_CHARACTER_INVALID, QDTG_BASE_POSITION, 0);
    EVT_AT_SetAngY(QDTG_BASE, EVENT_CHARACTER_INVALID, 180, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), QDTG_TASK);
    RegisterEventTask(task);
    EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    dynamics->resource = "BOX0";
    dynamics->parts = QDTG_BOX_PARTS;
    dynamics->character = QDTG_BOX0;
    dynamics->endFrame = 4;
    dynamics->trackCount = 10;
    dynamics->frameStep = 2;
    dynamics->currentFrame = 2;
    dynamics->delay = 0;
    EVT_AT_SetPos(QDTG_YUKA, EVENT_CHARACTER_INVALID, QDTG_YUKA_POSITION, 0);
    EVT_AT_SetAngY(QDTG_YUKA, EVENT_CHARACTER_INVALID, 0, 0);
    EVT_AT_SetPos(QDTG_YUKB, EVENT_CHARACTER_INVALID, QDTG_YUKB_POSITION, 0);
    EVT_AT_SetAngY(QDTG_YUKB, EVENT_CHARACTER_INVALID, 0, 0);
    EV_SetObjectFlagByIndex(1, 1);
    EV_DisableCharacterDisplay(QDTG_YUKA);
    Queue_EVT_AT_PlayDynamix(QDTG_YUKA, "YUKA", 2, 4, 76, nullptr, 2, 0);
    EVT_AT_SetPos(QDTG_HIBI, EVENT_CHARACTER_INVALID, QDTG_HIBI_POSITION, 0);
    EVT_AT_SetAngY(QDTG_HIBI, EVENT_CHARACTER_INVALID, 180, 0);
    EV_DisableCharacterDisplay(QDTG_HIBI);
    AT_SetLocalBaseChara(QDTG_DUMR, QDTG_RYO);
    AT_SetLocalBaseChara(QDTG_DUML, QDTG_REN);
    AT_SetLocalBaseChara(QDTG_DUMT, QDTG_TGY);
    EVT_AT_SetPos(QDTG_DUMR, QDTG_RYO, {0, 0, 0}, 0);
    EVT_AT_SetAngY(QDTG_DUMR, QDTG_RYO, 0, 0);
    EVT_AT_SetPos(QDTG_DUML, QDTG_REN, {0, 0, 0}, 0);
    EVT_AT_SetAngY(QDTG_DUML, QDTG_REN, 0, 0);
    EVT_AT_SetPos(QDTG_DUMT, QDTG_TGY, {0, 0, 0}, 0);
    EVT_AT_SetAngY(QDTG_DUMT, QDTG_TGY, 0, 0);
    EVT_AT_SetPos(QDTG_SOY, QDTG_BASE, {1000, 0, 0}, 0);
    EVT_AT_SetPos(QDTG_SEE, QDTG_BASE, {1000, 0, 0}, 0);
    for (std::size_t i = 0; i < 5; ++i)
        SetPluralControlFor(ctrlGetCharaByName(QDTG_CHARACTERS[i]), 0);
    EV_LoadCutsceneFromCurrentScene("0282E000");
    EV_LoadAudio("0282E000", 0);
    EV_spmBeginSampleExecute();
    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
    EVT_AT_MenCtrl_Callback(&film);
    if (work->retrySkip == 1)
        EV_SetAuthorActiveTo(1);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDTG_ATH0.name));
    work->continuation = RunRyoRenCallback;
    HLib::SetCurrentTaskTo(AuthorWaitCallback);
}
void InitializeTogyu(QdtgEventWork *work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    GetCurrentDateTime(eventTemporaryDateTime);
    work->state = 1;
    work->characters = Queue_EVT_AT_ReadChr(work->task, QDTG_CHARACTERS);
    work->motion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    work->savedNavi = SetNaviDisplayOverride(true);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("282E"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0282"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QDTG"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QDTG"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDTG"), 0);
    EV_FreeAuthorPak(1, "0282");
    EV_SetNewSceneIndex();
    SetWeatherUpdateEnabledTo(0);
    SetIsSkySetup(0);
    EV_lightLoad(QDTG_LIGHT);
    PushGlobalLightingProgram();
    EVT_AT_SetPos(QDTG_YUKA, EVENT_CHARACTER_INVALID, QDTG_YUKA_POSITION, 0);
    EVT_AT_SetAngY(QDTG_YUKA, EVENT_CHARACTER_INVALID, 0, 0);
    EVT_AT_SetPos(QDTG_HIBI, EVENT_CHARACTER_INVALID, QDTG_HIBI_POSITION, 0);
    EVT_AT_SetAngY(QDTG_HIBI, EVENT_CHARACTER_INVALID, 180, 0);
    ApplyCharacterGeometryLighting(ctrlGetCharaByName(QDTG_HIBI), 0);
    ApplyCharacterGeometryLighting(ctrlGetCharaByName(QDTG_YUKA), 0);
    SetRenderClockAndCompassTo(1);
    QdtgAdmissionNullsub();
}
} // namespace
void QdtgInit() {
}
void QdtgMain(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StartTogyuCallback, 4, 0x0b, sizeof(QdtgEventWork), QDTG_TASK);
    QdtgEventWork *const work = static_cast<QdtgEventWork *>(HLib::GetTaskParameter(task));
    work->unused08 = 0;
    work->retrySkip = 0;
    work->authorState = 0;
    work->loss = 0;
    work->task = task;
    work->entry = argument ? reinterpret_cast<const std::int32_t *>(argument) : &qdtgDefaultEntry;
    InitializeTogyu(work);
}
const ScriptAreaDefinition qdtgScriptDefinition{.areaId = AREA_QDTG, .field04 = 0, .init = QdtgInit, .main = QdtgMain};
} // namespace shenmue::s2
