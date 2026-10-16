#include "s2/areas/area_qes2.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>
#include <xmmintrin.h>
namespace shenmue::s2 {
namespace {
void Qes2Callback(void *);
void Qes2Enqueue(void *);
std::int32_t Qes2FloorboardSuccess() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 0}, .degrees = 1.5f, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8167, 10, 1, 0x40000000U, 0);
    EVT_AT_DispBlur(QES_RYO, 1, 7, 1, 40, 0);
    EV_DisableCharacterDisplay(QES_REN);
    EVT_AT_SetCam(2003, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0295A002", 1, 0); // @text EN: " " | JA: "はっ！"
    return 20;
}
std::int32_t Qes2FloorboardFailure() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 0}, .degrees = 1.5f, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetPos(QES_RYO, QES_RYO, {0.5f, 0, 0}, 45);
    EVT_AT_SetPos(QES_REN, QES_BASE, {-0.5f, 0, -3.5f}, 0);
    EVT_AT_SetPos(QES_REN, QES_RYO, {-0.56f, 0, -0.95999998f}, 45);
    EVT_AT_SetPos(QES_DUMMY, QES_FLOORBOARD, {0, -0.6f, 0}, 0);
    EVT_AT_SetMot(QES_RYO, 8147, 0, 1, 0, 0);
    EVT_AT_SetMot(QES_RYO, 8139, 60, 1, 0x44000000U, 45);
    EVT_AT_SetMot(QES_REN, 8134, 50, 1, 0, 0);
    EVT_AT_SetMot(QES_REN, 8131, 60, 1, 0x44000000U, 45);
    Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 206, 10, nullptr, 2, 10);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), QES_TASK);
    HLib::SetupTimedTask(task, 20);
    EventSmokeWork *const smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    smoke->character = QES_DUMMY;
    smoke->bone = 0xffffffffU;
    smoke->startDelay = 0;
    smoke->remaining = 2;
    smoke->finishDelay = 108;
    smoke->profile = 0;
    smoke->state = 0;
    smoke->birthFraction = 0.1f;
    smoke->birthStep = 0.1f;
    EVT_AT_SetCam(2001, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(2002, QES_BASE, QES_BASE, 0, 0, 45);
    EVT_AT_SetVoice("0295A004", 1, 0); // @text EN: " " | JA: "うわっ！"
    EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0295C006" : "0295C009", 1, 45); // @text 0295C006 EN: "Oh man=@" | JA: "…ったく。しゃあねぇな。" ; 0295C009 EN: "You're so slow=@" | JA: "トロくせぇな…ったくよ。"
    return 130;
}
void Qes2QueueFloorboardQte(Qes2EventWork *work) {
    EVT_AT_SetPos(QES_RYO, QES_BASE, {0, 0, 5}, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 0, 0);
    EVT_AT_SetPos(QES_REN, QES_BASE, {0, 0, 0}, 0);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 0, 0);
    EVT_AT_SetCam(2000, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_REN, 8135, 40, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0, 20);
    Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 3, 10, nullptr, 1, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_SK_WaitQTE_Callback, 6, 0x0b, sizeof(StepWaitQteWork), QES_TASK);
    StepWaitQteWork *const wait = static_cast<StepWaitQteWork *>(HLib::GetTaskParameter(task));
    EV_SetAuthorActiveTo(0);
    work->qteElapsed = 0;
    work->qteSucceeded = 0;
    wait->task = task;
    wait->parent = HLib::GetCurrentTask();
    wait->success = Qes2FloorboardSuccess;
    wait->failure = Qes2FloorboardFailure;
    wait->referenceFrames = 24;
    wait->elapsed = &work->qteElapsed;
    wait->maximumFrames = scenarioQteRetryCount * 4 + 16;
    if (wait->maximumFrames > 20)
        wait->maximumFrames = 20;
    wait->button = 256;
    wait->status = &work->qteStatus;
    wait->phase = 0;
    wait->succeeded = &work->qteSucceeded;
    HLib::SetupTimedTask(wait->parent, 0);
}
std::int32_t Qes2FirstDrawerSuccess() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 2.5f}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8155, 0, 1, 0, 0);
    EVT_AT_SetCam(3008, QES_BASE, QES_BASE, 0, 0, 0);
    return 30;
}
std::int32_t Qes2FirstDrawerFailure() {
    EventSetPositionAngle position{
        .cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = QES2_DRAWER_FAILURE_POSITIONS[0], .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8141, 50, 1, 0x60000000U, 0);
    EVT_AT_SetCam(3005, QES_BASE, QES_BASE, 0, 0, 0);
    return 20;
}
std::int32_t Qes2SecondDrawerSuccess() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 2.5f}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8155, 0, 1, 0x20000000U, 0);
    EVT_AT_SetCam(3004, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0295A002", 1, 0); // @text EN: " " | JA: "はっ！"
    return 30;
}
std::int32_t Qes2SecondDrawerFailure() {
    EventSetPositionAngle position{
        .cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = QES2_DRAWER_FAILURE_POSITIONS[1], .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8141, 50, 1, 0x40000000U, 0);
    EVT_AT_SetCam(3010, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0295A008", 1, 0); // @text EN: " " | JA: "おわっ…"
    return 20;
}
void Qes2ExtinguisherSmokeCallback(void *data) {
    Qes2SmokeWork *const work = static_cast<Qes2SmokeWork *>(data);
    if (work->phase) {
        FreeParticleEffect(0, work->effect);
        HLib::DestroyCurrentTask();
    } else {
        Vector3 position{119.0999984741211f, 98.95600128173828f, 183.98f};
        const Vector3 volume{0.005f, 0, 0.005f}, velocity{0, 0, 0.1f}, variance{0, 0.01568627543747425f, 0};
        EV_GetCharaPosition(QES_RYO, 14, &position, EventTransformFlags::World);
        position.z += 3;
        work->effect = static_cast<std::uint32_t>(CreateSmokeEffect(position, volume, 80, 0.0001f, {}, 0));
        SetParticleEffectPosition(0, work->effect, &position, &volume);
        SetSmokeEffectFadeFactor(work->effect, 112);
        SetSmokeEffectFalloff(work->effect, 210);
        SetParticleEffectColour(0, work->effect, 0, 255, 255, 255);
        SetParticleEffectInitialScale(0, work->effect, 0.1882352977991104f);
        SetParticleEffectSize(0, work->effect, 2.627450942993164f, 3.725490093231201f);
        SetParticleEffectBirthRate(0, work->effect, 0.029803920537233353f);
        SetParticleEffectVelocity(0, work->effect, velocity, variance);
        ResumeParticleEffect(0, work->effect);
        HLib::SetupTaskDelay(work->frames);
        ++work->phase;
    }
}
std::int32_t Qes2TinSuccess() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), QES_TASK);
    HLib::SetupTimedTask(task, 25);
    EventSmokeWork *const smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    smoke->character = QES2_TIN;
    smoke->bone = 0xffffffffU;
    smoke->startDelay = 80;
    smoke->remaining = 0;
    smoke->finishDelay = 0;
    smoke->profile = 0;
    smoke->state = 0;
    smoke->birthFraction = 0.1f;
    smoke->birthStep = 0.1f;
    Queue_EVT_AT_PlayDynamix(QES2_TIN, "ITTO", 2, 90, 47, QES2_TIN_PARTS, 1, 10);
    EVT_AT_SetSound(1207, 15);
    EVT_AT_SetPos(QES_RYO, QES_BASE, {3.1f, 0, -3.6f}, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, {8.6f, 0, -3.3f}, 40);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 40);
    EVT_AT_SetAngY(QES_REN, QES_REN, -90, 0);
    EVT_AT_SetMot(QES_RYO, 8196, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_YUA, 8195, 0, 1, 0x40000000U, 40);
    EVT_AT_SetMot(QES_REN, 8137, 70, 1, 0x40000000U, 0);
    EVT_AT_DispBlur(QES_RYO, 6, 15, 2, 30, 10);
    EVT_AT_SetVoice("0295A011", 1, 0); // @text EN: " " | JA: "おりゃっ！"
    EVT_AT_SetVoice("0295B003", 1, 40); // @text EN: " " | JA: "ああっ！?"
    EVT_AT_SetVoice("0295B004", 1, 60); // @text EN: "No! Filthy!\nWhat is this!?" | JA: "イヤッ！　汚いっ！\n何これっ！　いやあっ！"
    EVT_AT_SetCam(5006, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(5002, QES_BASE, QES_BASE, 0, 0, 40);
    if (qes2SoundHandle) {
        task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        HLib::SetupTimedTask(task, 40);
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 10, .target = 10, .frames = 0, .phase = 0, .step = 0};
    }
    return 105;
}
std::int32_t Qes2TinFailure() {
    EV_DisableCharacterDisplay(QES_YUA);
    EV_DisableCharacterDisplay(QES2_CHAINSAW);
    EV_DisableCharacterDisplay(QES_RYO);
    EVT_AT_ObjectDisp_1(QES_RYO, 60, 0);
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_TIN_FAILURE_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 0);
    EVT_AT_SetPos(QES_REN, QES_BASE, QES2_TIN_REN_POSITION, 0);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 30, 0);
    EVT_AT_SetPos(QES_REN, QES_BASE, {1.2f, 0, -2.4f}, 50);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 120, 50);
    EVT_AT_SetMot(QES_RYO, 8136, 0, 1, 0x40000000U, 60);
    EVT_AT_SetMot(QES_REN, 8136, 5, 1, 0x40000000U, 50);
    QueueQesAngle(QES_RYO, 6, 15, 65);
    QueueQesAngle(QES_REN, 5, 12, 50);
    EVT_AT_SetVoice("0295C014", 1, 0); // @text EN: "Fool!" | JA: "…バカが…！"
    EVT_AT_SetCam(5008, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(5009, QES_BASE, QES_BASE, 0, 0, 50);
    EVT_AT_SetCam(5010, QES_BASE, QES_BASE, 0, 0, 90);
    if (qes2SoundHandle) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 70, .target = 70, .frames = 0, .phase = 0, .step = 0};
        if (qes2SoundHandle) {
            task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 50);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 70, .target = 50, .frames = 40, .phase = 0, .step = -0.5f};
        }
    }
    return 120;
}
std::int32_t Qes2ShopSuccess() {
    EVT_AT_ObjectDisp_1(QES2_SHOK, 180, 1);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = QES2_SHOK,
        .character = QES_RYO,
        .bone = 21,
        .position = {0.1f, 0.2f, 0},
        .rotation = {42780, 60074, 27306},
        .mode = 0,
        .delay = 60};
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1, .target = QES2_SHOK, .mode = 1, .delay = 200};
    EVT_AT_SetSound(1213, 120);
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_SHOP_SUCCESS_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, QES2_SHOP_SUCCESS_YUA, 0);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 180, 0);
    EVT_AT_SetMot(QES_RYO, 8149, 0, 1, 0x20000000U, 0);
    EVT_AT_SetMot(QES_YUA, 8193, 0, 0.7f, 0, 0);
    EVT_AT_SetMot(QES_YUA, 8192, 350, 1, 0, 80);
    EVT_AT_SetCam(5004, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(5007, QES_BASE, QES_BASE, 0, 0, 80);
    EVT_AT_SetVoice("0295B009", 1, 80); // @text EN: "No!\nNooo!" | JA: "イヤッ！　イヤアァッ！"
    EVT_AT_SetVoice("0295B010", 1, 140); // @text EN: "=@my eyes!" | JA: "ゲホッ！　ゲホッ！\nアアッ、目があっ！"
    if (qes2SoundHandle) {
        task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
            .initial = initial, .target = 40, .frames = 80, .phase = 0, .step = initial == 40 ? 0 : static_cast<float>(40 - initial) / 80};
        if (qes2SoundHandle) {
            task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 80);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 20, .target = 20, .frames = 0, .phase = 0, .step = 0};
        }
    }
    task = HLib::EnqueueTaskWithParameter(Qes2ExtinguisherSmokeCallback, 6, 0x0b, sizeof(Qes2SmokeWork), QES_TASK);
    HLib::SetupTimedTask(task, 120);
    static_cast<Qes2SmokeWork *>(HLib::GetTaskParameter(task))->frames = 80;
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 170});
    EV_DisableCharacterDisplay(QES2_SHOH);
    EV_EnableCharacterDisplay(QES2_SHOK);
    return 260;
}
std::int32_t Qes2ShopFailure() {
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_SHOP_SUCCESS_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, {2.5f, 0, -3.2f}, 0);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, {-0.4f, 0, 11.5f}, 60);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 170, 60);
    EVT_AT_SetAngY(QES_YUA, QES_YUA, 90, 40);
    EVT_AT_SetMot(QES_RYO, 8149, 0, 1, 0x20000000U, 0);
    EVT_AT_SetMot(QES_RYO, 8149, 70, 0, 0x20000000U, 70);
    EVT_AT_SetMot(QES_YUA, 8194, 0, 1, 0, 0);
    EVT_AT_SetMot(QES_YUA, 8193, 0, 1, 0, 40);
    EVT_AT_SetMot(QES_YUA, 8191, 0, 1, 0, 60);
    EVT_AT_SetVoice("0295B012", 1, 65); // @text EN: " " | JA: "ハァーッ！"
    EVT_AT_SetVoice("0295A012", 1, 85); // @text EN: " " | JA: "…くっ！"
    EVT_AT_SetVoice("0295C017", 1, 160); // @text EN: "Too damn slow!" | JA: "…このグズ…！"
    EVT_AT_SetCam(5012, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(5013, QES_BASE, QES_BASE, 0, 0, 60);
    if (qes2SoundHandle) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
            .initial = initial, .target = 20, .frames = 60, .phase = 0, .step = initial == 20 ? 0 : static_cast<float>(20 - initial) / 60};
        if (qes2SoundHandle) {
            task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 60);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 10, .target = 2, .frames = 10, .phase = 0, .step = -0.8f};
        }
    }
    EV_DisableCharacterDisplay(QES2_SHOH);
    EV_EnableCharacterDisplay(QES2_SHOK);
    QueueQesRetryFade(160);
    return 250;
}
std::int32_t Qes2Initialize(Qes2EventWork *work) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QES2"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QES2"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES_DUMMY);
    eventSavedSoundScene = GetSelectedSoundSceneIndex();
    std::strcpy(eventSavedSoundBankName.data(), shenGetCurrentSbt());
    std::strcpy(eventSavedBgmName.data(), GetCurrentBgm());
    EV_LoadCutsceneFromCurrentScene("0291A000");
    EV_LoadAudio("0291A000", 0);
    EV_spmBeginSampleExecute();
    scenarioQteRetryCount = 0;
    qes2SoundHandle = AllocateMaterialSoundHandle(1951);
    qes2SoundIntensity = 0;
    ExecuteMaterialSoundHandleCommand(qes2SoundHandle, 0xa6, 0);
    SetScenarioCharacterFlag40(QES_RYO, 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0295");
    SetLowQualityHands(ctrlGetCharaByName(QES_RYO));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_RYO), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 0);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 0);
    SetAreaNpcLowQualityHands(false);
    for (std::int32_t index = 0; index < 20; ++index)
        eventMovementStates[index] = 0;
    if (g_currentMap.name == QES2_AREA.name)
        qes2FloorHeight = 100;
    else {
        Vector3 position;
        EV_GetCharaPosition(QES_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        qes2FloorHeight = position.y;
    }
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EventCling cling{.cleanup = 0,
        .target = QES2_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = {0.20200001f, 0.1f, -0.044f},
        .rotation = {3731, 56251, 35862},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    work->phase = 1;
    GetCurrentDateTime(eventTemporaryDateTime);
    return 0;
}
std::int32_t Qes2FirstChase(Qes2EventWork *work) {
    if (!work->subphase) {
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        const shen_name *lists[]{QES2_SECOND_PAIR_CHARACTERS, QES2_SURROUND_CHARACTERS};
        for (const shen_name *list : lists)
            for (std::int32_t index = 0; list[index].name != QES_INVALID.name; ++index) {
                if (DoesScenarioCharacterExist(list[index])) {
                    EV_npcLockUnlock(list[index]);
                    EV_npcFree(list[index]);
                }
            }
        Queue_EVT_UTL_LoadNPCChara_3(QES2_FIRST_PAIR_CHARACTERS, work->task);
        ++work->subphase;
    } else {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        for (const shen_name character : {QES_REN, QES_YUA}) {
            EV_EnsureCharacterDraw(character);
            SetScenarioCharacterFlag40(character, 0);
            EV_EnableCharacterDisplay(character);
            if (character.name == QES_YUA.name) {
                EV_ToggleCharacterFlag(QES2_CHAINSAW);
                EV_EnableCharacterDisplay(QES2_CHAINSAW);
            }
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EV_EnsureCharacterDraw(QES2_KU5);
        SetScenarioCharacterFlag40(QES2_KU5, 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES2_KU5), 3);
        EVT_AT_ObjectDisp_1(QES2_KU5, 105, 0);
        Queue_EVT_AT_PlayDynamix(QES2_TIN, "ITTO", 2, 3, 47, QES2_TIN_PARTS, 1, 0);
        EVT_AT_SetMot(QES_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_RYO, 8136, 0, 1, 0, 50);
        EVT_AT_SetMot(QES_REN, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 8136, 0, 1, 0, 45);
        EVT_AT_SetMot(QES_REN, 8165, 0, 1, 0, 147);
        EVT_AT_SetMot(QES_REN, 8148, 0, 0.9f, 0, 165);
        EVT_AT_SetMot(QES2_KU5, 8136, 0, 1, 0, 105);
        EVT_AT_SetMot(QES2_KU5, 8175, 0, 1, 0, 127);
        EVT_AT_SetMot(QES2_KU5, 8130, 0, 0.9f, 0, 165);
        EVT_AT_SetMot(QES_YUA, 8194, 0, 1, 0x60000000U, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {249.2f, qes2FloorHeight, 167.2f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_INITIAL_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES2_INITIAL_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_YUA, QES_BASE, {1.5f, 0, 0.1f}, 0);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 0);
        EVT_AT_SetPos(QES2_KU5, QES_BASE, {-26.8f, 0, -20.5f}, 105);
        EVT_AT_SetAngY(QES2_KU5, QES_BASE, 270, 105);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_RYO), FacialExpressionId::Angry, 0, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_REN), FacialExpressionId::Angry, 0, 1);
        EVT_AT_SetAngY(QES_REN, QES2_KU5, 180, 165);
        EVT_AT_SetAngY(QES2_KU5, QES2_KU5, 180, 127);
        EVT_AT_SetAngY(QES2_KU5, QES2_KU5, 180, 165);
        EVT_AT_SetVoice("0295B001", 1, 0); // @text EN: "You're not getting away." | JA: "逃がさないわよ…"
        EVT_AT_SetVoice("0295C002", 1, 65); // @text EN: "Let's get outta here!" | JA: "おいっ、逃げるぞ！"
        EVT_AT_SetVoice("0295C003", 1, 120); // @text EN: " " | JA: "ちっ…"
        EVT_AT_SetCam(1001, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 45);
        EVT_AT_SetCam(1002, QES_BASE, QES_BASE, 0, 0, 105);
        QueueQesAngle(QES_RYO, 6, 15, 85);
        QueueQesAngle(QES_REN, 3, 30, 60);
        if (qes2SoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 20, .target = 80, .frames = 150, .phase = 0, .step = 0.4f};
        }
        work->phase = 2;
        work->subphase = 0;
    }
    return 150;
}
std::int32_t Qes2FirstPairedFight(Qes2EventWork *work) {
    if (!work->subphase) {
        for (const shen_name character : {QES_REN, QES2_KU5, QES2_SCX, QES2_SBO}) {
            EV_EnsureCharacterDraw(character);
            SetScenarioCharacterFlag40(character, 0);
            EV_EnableCharacterDisplay(character);
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        InitializeQes2FirstPairedSequence(224.89999f, 147, 90);
        qes2FirstPairedCharacters[0] = QES_RYO;
        qes2FirstPairedCharacters[1] = QES2_SCX;
        qes2FirstPairedCharacters[2] = QES2_SBO;
        adfsSetCameraIdxForChara(QES2_SCX, QES_CAMERA_WTC);
        adfsSetCameraIdxForChara(QES2_SBO, QES_CAMERA_THG);
        for (const shen_name character : {QES_RYO, QES2_KU5, QES2_SCX, QES2_SBO}) {
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 0, 0, 16);
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 1, 0, 16);
        }
        ++work->subphase;
        return 0;
    }
    const std::int32_t result = UpdateQes2FirstPairedSequence(&work->qteElapsed);
    if (result == -1)
        return 0;
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 1, 2, 16);
    std::int32_t delay = 0;
    if (result == 1) {
        EV_DisableCharacterDisplay(QES2_SCX);
        EV_DisableCharacterDisplay(QES2_SBO);
        for (std::int32_t index = 0; QES2_FIRST_PAIR_CHARACTERS[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(QES2_FIRST_PAIR_CHARACTERS[index])) {
                EV_npcLockUnlock(QES2_FIRST_PAIR_CHARACTERS[index]);
                EV_npcFree(QES2_FIRST_PAIR_CHARACTERS[index]);
            }
        }
        Queue_EVT_UTL_LoadNPCChara_3(QES2_SECOND_PAIR_CHARACTERS, nullptr);
        work->phase = 3;
    } else {
        work->phase = 13;
        if (scenarioQteRetryCount < 2)
            ++scenarioQteRetryCount;
        for (std::int32_t index = 0; index < 20; ++index)
            CancelEventMovement(index);
        delay = 30;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    work->subphase = 0;
    return delay;
}
std::int32_t Qes2FirstFloorboard(Qes2EventWork *work) {
    if (!work->subphase) {
        EV_EnsureCharacterDraw(QES_REN);
        SetScenarioCharacterFlag40(QES_REN, 0);
        EV_EnableCharacterDisplay(QES_REN);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
        EventSetPositionAngle position{.cleanup = 0,
            .character = QES_RYO,
            .parent = QES_INVALID,
            .position = QES2_FLOORBOARD_POSES[0].position,
            .degrees = QES2_FLOORBOARD_POSES[0].yawDegrees,
            .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        position.character = QES_REN;
        position.position = QES2_FLOORBOARD_POSES[1].position;
        position.degrees = QES2_FLOORBOARD_POSES[1].yawDegrees;
        EVT_AT_SetPosAng_Callback(&position);
        EVT_AT_SetMot(QES_RYO, 8136, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 8136, 0, 1.5f, 0x40000000U, 0);
        QueueQesAngle(QES_REN, -3, 5, 15);
        EVT_AT_SetVoice("0295C004", 1, 0); // @text EN: "Hey, jump!" | JA: "おい、飛べ！"
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {220.89999f, qes2FloorHeight, 167}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 150, 0);
        EVT_AT_SetPos(QES_FLOORBOARD, QES_INVALID, {220.3f, qes2FloorHeight, 167.3f}, 0);
        EVT_AT_SetAngY(QES_FLOORBOARD, QES_INVALID, 180, 0);
        Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 3, 10, nullptr, 1, 0);
        ++work->subphase;
        return 30;
    }
    if (work->subphase == 1) {
        EV_EnsureCharacterDraw(QES_REN);
        SetScenarioCharacterFlag40(QES_REN, 0);
        EV_EnableCharacterDisplay(QES_REN);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {220.89999f, qes2FloorHeight, 167}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 150, 0);
        EVT_AT_SetPos(QES_FLOORBOARD, QES_INVALID, {220.3f, qes2FloorHeight, 167.3f}, 0);
        EVT_AT_SetAngY(QES_FLOORBOARD, QES_INVALID, 180, 0);
        Qes2QueueFloorboardQte(work);
        ++work->subphase;
        return 0;
    }
    if (work->subphase != 2)
        return 0;
    EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
    EV_EnableCharacterDisplay(QES_REN);
    EventSetPositionAngle position{.cleanup = 0,
        .character = QES_RYO,
        .parent = QES_INVALID,
        .position = QES2_FLOORBOARD_POSES[2].position,
        .degrees = QES2_FLOORBOARD_POSES[2].yawDegrees,
        .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    position.character = QES_REN;
    position.position = QES2_FLOORBOARD_POSES[3].position;
    position.degrees = QES2_FLOORBOARD_POSES[3].yawDegrees;
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 8136, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_REN, 8136, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0295C007", 1, 0); // @text EN: "There's another one!" | JA: "ほれっ、もういっちょ！"
    QueueQesAngle(QES_RYO, -1, 20, 10);
    QueueQesAngle(QES_REN, -1, 10, 30);
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {208.2f, qes2FloorHeight, 178.2f}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, 90, 0);
    EVT_AT_SetPos(QES_FLOORBOARD, QES_INVALID, {206.3f, qes2FloorHeight, 178.4f}, 0);
    EVT_AT_SetAngY(QES_FLOORBOARD, QES_INVALID, 90, 0);
    Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 3, 10, nullptr, 1, 0);
    work->phase = 4;
    work->subphase = 0;
    return 30;
}
std::int32_t Qes2SecondFloorboard(Qes2EventWork *work) {
    if (work->subphase) {
        if (work->subphase == 1) {
            work->phase = 7;
            work->subphase = 0;
        }
        return 0;
    }
    for (std::int32_t index = 0; index < 20; ++index)
        CancelEventMovement(index);
    EVT_AT_SetVoice("0295C008", 1, 10); // @text EN: " " | JA: "ほいっ！"
    EV_EnsureCharacterDraw(QES_REN);
    SetScenarioCharacterFlag40(QES_REN, 0);
    EV_EnableCharacterDisplay(QES_REN);
    mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {208.2f, qes2FloorHeight, 178.2f}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, 90, 0);
    EVT_AT_SetPos(QES_FLOORBOARD, QES_INVALID, {206.3f, qes2FloorHeight, 178.39999f}, 0);
    EVT_AT_SetAngY(QES_FLOORBOARD, QES_INVALID, 90, 0);
    Qes2QueueFloorboardQte(work);
    ++work->subphase;
    return 0;
}
std::int32_t Qes2Drawers(Qes2EventWork *work) {
    if (!work->subphase) {
        if (qes2SoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
                .initial = initial, .target = 60, .frames = 120, .phase = 0, .step = initial == 60 ? 0 : static_cast<float>(60 - initial) / 120};
        }
        EV_EnsureCharacterDraw(QES_REN);
        SetScenarioCharacterFlag40(QES_REN, 0);
        EV_EnableCharacterDisplay(QES_REN);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {198.2f, qes2FloorHeight, 173.7f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_INVALID, {205.8f, qes2FloorHeight, 178.6f}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_INVALID, 90, 0);
        EVT_AT_SetPos(QES_REN, QES_INVALID, {204.1f, qes2FloorHeight, 179}, 0);
        EVT_AT_SetAngY(QES_REN, QES_INVALID, 90, 0);
        EVT_AT_SetPos(QES2_DRAWERS[0], QES_INVALID, {198.1f, qes2FloorHeight, 172.5f}, 0);
        EVT_AT_SetPos(QES2_DRAWERS[1], QES_INVALID, {199.2f, qes2FloorHeight, 166.1f}, 0);
        EVT_AT_SetPos(QES2_DRAWERS[2], QES_INVALID, {197.7f, qes2FloorHeight + 0.9f, 172.1f}, 0);
        EVT_AT_SetPos(QES2_DRAWERS[3], QES_INVALID, {199.4f, qes2FloorHeight + 0.9f, 165.3f}, 0);
        EVT_AT_SetPos(QES2_DRAWERS[4], QES_INVALID, {199.5f, qes2FloorHeight, 165}, 0);
        EVT_AT_SetPos(QES_BASE, QES2_DRAWERS[0], {0, 0, 0}, 0);
        EVT_AT_SetAngY(QES_BASE, QES2_DRAWERS[0], 0, 0);
        EVT_AT_SetMot(QES_RYO, 8150, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 8150, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 8155, 0, 1, 0x20000000U, 40);
        EVT_AT_SetMot(QES_REN, 8150, 0, 1, 0, 70);
        QueueQesAngle(QES_RYO, -5, 15, 20);
        QueueQesAngle(QES_REN, -5, 15, 10);
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
        QueueSkQteWait(&work->qteElapsed, 4, 24, 16, Qes2FirstDrawerSuccess, Qes2FirstDrawerFailure, 10);
        EVT_AT_SetVoice("0295C010", 1, 20); // @text EN: "=@that was close." | JA: "…っと！　あぶね。"
        ++work->subphase;
        return 0;
    }
    if (work->subphase == 1) {
        EVT_AT_SetPos(QES_BASE, QES2_DRAWERS[1], {0, 0, 0}, 0);
        EVT_AT_SetAngY(QES_BASE, QES2_DRAWERS[1], 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_INVALID, {199.3f, qes2FloorHeight, 175}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_INVALID, 0, 0);
        EVT_AT_SetMot(QES_RYO, 8150, 0, 1, 0x40000000U, 0);
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
        QueueSkQteWait(&work->qteElapsed, 64, 24, 16, Qes2SecondDrawerSuccess, Qes2SecondDrawerFailure, 0);
        ++work->subphase;
        return 0;
    }
    if (work->subphase != 2 || !g_usingCustomAuthorCharacterList)
        return 0;
    EV_EnsureCharacterDraw(QES2_RKN);
    SetScenarioCharacterFlag40(QES2_RKN, 0);
    EV_EnableCharacterDisplay(QES2_RKN);
    mtwkSetRequestedRootMode(scenFindMotionwork(QES2_RKN), 3);
    EVT_AT_SetCam(3100, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(3101, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 40);
    EV_DisableCharacterDisplay(QES_RYO);
    EVT_AT_ObjectDisp_1(QES_RYO, 40, 0);
    EVT_AT_SetPos(QES_RYO, QES_INVALID, {198.7f, qes2FloorHeight, 163.8f}, 0);
    EVT_AT_SetAngY(QES_RYO, QES_INVALID, 0, 0);
    EVT_AT_SetPos(QES_RYO, QES_INVALID, {199.2f, qes2FloorHeight, 159.8f}, 40);
    EVT_AT_SetAngY(QES_RYO, QES_INVALID, 0, 40);
    EVT_AT_SetPos(QES_REN, QES_INVALID, {198.5f, qes2FloorHeight, 158}, 0);
    EVT_AT_SetAngY(QES_REN, QES_INVALID, 0, 0);
    EVT_AT_SetPos(QES_REN, QES_INVALID, {196.2f, qes2FloorHeight, 151.8f}, 40);
    EVT_AT_SetAngY(QES_REN, QES_INVALID, 315, 40);
    EVT_AT_SetPos(QES2_KK4, QES_INVALID, {201.7f, qes2FloorHeight, 147.9f}, 0);
    EVT_AT_SetAngY(QES2_KK4, QES_INVALID, 90, 0);
    EVT_AT_SetPos(QES2_KK4, QES_INVALID, {198.8f, qes2FloorHeight, 149.2f}, 40);
    EVT_AT_SetAngY(QES2_KK4, QES_INVALID, 135, 40);
    EVT_AT_SetAngY(QES_REN, QES_REN, -45, 30);
    EVT_AT_SetAngY(QES2_KK4, QES2_KK4, -45, 30);
    EVT_AT_SetMot(QES_RYO, 8150, 0, 1, 0x40000000U, 40);
    EVT_AT_SetMot(QES_REN, 8133, 0, 1.5f, 0x40000000U, 30);
    EVT_AT_SetMot(QES_REN, 8133, 30, 1.5f, 0, 40);
    EVT_AT_SetMot(QES_REN, 8140, 0, 1, 0, 60);
    EVT_AT_SetMot(QES2_KK4, 8189, 15, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES2_KK4, 0xf006, 0, 0.7f, 0, 40);
    EVT_AT_SetMot(QES2_KK4, 8164, 0, 1, 0, 70);
    EVT_AT_SetMot(QES2_KK4, 8129, 0, 1, 0x20000000U, 85);
    EVT_AT_SetVoice("0295C011", 1, 30); // @text EN: "Damn!" | JA: "…くっそ！"
    work->phase = 8;
    work->subphase = 0;
    return 70;
}
std::int32_t Qes2SecondPairedFight(Qes2EventWork *work) {
    if (!work->subphase) {
        if (qes2SoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
                .initial = initial, .target = 30, .frames = 90, .phase = 0, .step = initial == 30 ? 0 : static_cast<float>(30 - initial) / 90};
        }
        for (const shen_name character : {QES2_RKN, QES2_MRC}) {
            EV_EnsureCharacterDraw(character);
            SetScenarioCharacterFlag40(character, 0);
            EV_EnableCharacterDisplay(character);
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        InitializeQes2SecondPairedSequence(198.39999f, 147.7f, 180);
        qes2SecondPairedCharacters[0] = QES_RYO;
        qes2SecondPairedCharacters[1] = QES2_RKN;
        qes2SecondPairedCharacters[2] = QES2_MRC;
        adfsSetCameraIdxForChara(QES2_RKN, QES_CAMERA_WTC);
        adfsSetCameraIdxForChara(QES2_MRC, QES_CAMERA_THG);
        for (const shen_name character : {QES_RYO, QES2_RKN, QES2_MRC}) {
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 0, 0, 16);
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 1, 0, 16);
        }
        ++work->subphase;
        return 0;
    }
    const std::int32_t result = UpdateQes2SecondPairedSequence(&work->qteElapsed);
    if (result == -1)
        return 0;
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 1, 2, 16);
    std::int32_t delay = 0;
    if (result == 1) {
        for (std::int32_t index = 0; QES2_SECOND_PAIR_CHARACTERS[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(QES2_SECOND_PAIR_CHARACTERS[index])) {
                EV_npcLockUnlock(QES2_SECOND_PAIR_CHARACTERS[index]);
                EV_npcFree(QES2_SECOND_PAIR_CHARACTERS[index]);
            }
        }
        work->phase = 9;
        work->subphase = 0;
    } else {
        work->phase = 13;
        if (scenarioQteRetryCount < 2)
            ++scenarioQteRetryCount;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        delay = 30;
        work->subphase = 0;
    }
    return delay;
}
std::int32_t Qes2TinEncounter(Qes2EventWork *work) {
    switch (work->subphase) {
    case 0:
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {217, qes2FloorHeight, 144}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 180, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, {21.5f, 0, -3.8f}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 0);
        Queue_EVT_AT_PlayDynamix(QES2_TIN, "ITTO", 2, 3, 47, QES2_TIN_PARTS, 1, 0);
        EVT_AT_SetPos(QES2_TIN, QES_BASE, QES2_TIN_POSITION, 0);
        EVT_AT_SetAngY(QES2_TIN, QES_BASE, 270, 0);
        EVT_AT_SetMot(QES_RYO, 8150, 0, 1, 0x40000000U, 0);
        EVT_AT_SetCam(5014, QES_BASE, QES_BASE, 0, 0, 0);
        ++work->subphase;
        return 45;
    case 1:
        for (const shen_name character : {QES_REN, QES_YUA}) {
            EV_EnsureCharacterDraw(character);
            SetScenarioCharacterFlag40(character, 0);
            EV_EnableCharacterDisplay(character);
            if (character.name == QES_YUA.name) {
                EV_ToggleCharacterFlag(QES2_CHAINSAW);
                EV_EnableCharacterDisplay(QES2_CHAINSAW);
            }
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EVT_AT_SetVoice("0295C012", 1, 0); // @text EN: "Hey!\nThis way!" | JA: "おい！こっちだ！"
        EVT_AT_SetVoice("0295C013", 1, 50); // @text EN: "Kick these down=@that'll stop them." | JA: "こいつを蹴っとばして、\n足止めすっか。"
        EV_DisableCharacterDisplay(QES_RYO);
        EV_DisableCharacterDisplay(QES_YUA);
        EV_DisableCharacterDisplay(QES2_CHAINSAW);
        EV_EnableCharacterDisplay(QES_REN);
        EVT_AT_ObjectDisp_1(QES_RYO, 60, 0);
        EVT_AT_ObjectDisp_1(QES_YUA, 100, 0);
        EVT_AT_ObjectDisp_1(QES2_CHAINSAW, 100, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, {6.8f, 0, -3.3f}, 60);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 60);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES2_TIN_REN_POSITION, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 30, 0);
        EVT_AT_SetPos(QES_YUA, QES_BASE, {18, 0, -5.3f}, 100);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 180, 100);
        EVT_AT_SetAngY(QES_YUA, QES_YUA, -90, 140);
        EVT_AT_SetMot(QES_RYO, 8168, 0, 1, 0x40000000U, 60);
        EVT_AT_SetMot(QES_REN, 8132, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_YUA, 8194, 0, 1, 0x20000000U, 100);
        EVT_AT_SetMot(QES_YUA, 8193, 0, 1, 0x20000000U, 140);
        EVT_AT_SetCam(5005, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetCam(5000, QES_BASE, QES_BASE, 0, 0, 60);
        if (qes2SoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 60);
            const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
                .initial = initial, .target = 20, .frames = 60, .phase = 0, .step = initial == 20 ? 0 : static_cast<float>(20 - initial) / 60};
        }
        ++work->subphase;
        return 120;
    case 2:
        qteBattleUtil(BattleQteMode::SetWait, 0, 6, scenarioQteRetryCount * 4 + 16);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 4, 256);
        qteBattleUtil(BattleQteMode::Start);
        ++work->subphase;
        return 0;
    case 3:
        if (!qteBattleUtil(BattleQteMode::GetResult))
            return 0;
        if (qteBattleUtil(BattleQteMode::GetResult) == 1) {
            const std::int32_t delay = Qes2TinSuccess();
            work->subphase = 0;
            work->phase = 10;
            return delay;
        } else {
            Queue_EVT_UTL_LoadNPCChara_3(QES2_SURROUND_CHARACTERS, nullptr);
            const std::int32_t delay = Qes2TinFailure();
            work->subphase = 0;
            work->phase = 12;
            return delay;
        }
    default:
        return 0;
    }
}
std::int32_t Qes2TinClear(Qes2EventWork *work) {
    for (const shen_name character : {QES_REN, QES_YUA}) {
        EV_EnsureCharacterDraw(character);
        SetScenarioCharacterFlag40(character, 0);
        EV_EnableCharacterDisplay(character);
        if (character.name == QES_YUA.name) {
            EV_ToggleCharacterFlag(QES2_CHAINSAW);
            EV_EnableCharacterDisplay(QES2_CHAINSAW);
        }
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    }
    EV_DisableCharacterDisplay(QES_RYO);
    EVT_AT_ObjectDisp_1(QES_RYO, 20, 0);
    EV_EnableCharacterDisplay(QES2_SHOH);
    EV_DisableCharacterDisplay(QES2_SHOK);
    EVT_AT_SetPos(QES_RYO, QES_BASE, {4.8f, 0, -3}, 20);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 20);
    EVT_AT_SetPos(QES_REN, QES_BASE, QES2_TIN_CLEAR_REN, 0);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 180, 0);
    EVT_AT_SetPos(QES2_SHOH, QES_BASE, QES2_EXTINGUISHER_POSITION, 0);
    EVT_AT_SetAngY(QES2_SHOH, QES_BASE, 0, 0);
    EVT_AT_SetPos(QES2_SHOK, QES_BASE, QES2_EXTINGUISHER_POSITION, 0);
    EVT_AT_SetAngY(QES2_SHOK, QES_BASE, 0, 0);
    EVT_AT_SetMot(QES_RYO, 8150, 0, 1, 0x40000000U, 20);
    EVT_AT_SetMot(QES_REN, 8136, 5, 1, 0x40000000U, 0);
    EVT_AT_SetCam(5001, QES_BASE, QES_BASE, 0, 0, 0);
    QueueQesAngle(QES_RYO, 3, 30, 23);
    QueueQesAngle(QES_REN, -5, 15, 60);
    if (qes2SoundHandle) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        HLib::SetupTimedTask(task, 50);
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 60, .target = 60, .frames = 0, .phase = 0, .step = 0};
    }
    work->phase = 11;
    return 50;
}
std::int32_t Qes2ShopEscape(Qes2EventWork *work) {
    switch (work->subphase) {
    case 0:
        EV_EnableCharacterDisplay(QES2_SHOH);
        EV_DisableCharacterDisplay(QES2_SHOK);
        EVT_AT_SetPos(QES2_SHOH, QES_BASE, QES2_EXTINGUISHER_POSITION, 0);
        EVT_AT_SetAngY(QES2_SHOH, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES2_SHOK, QES_BASE, QES2_EXTINGUISHER_POSITION, 0);
        EVT_AT_SetAngY(QES2_SHOK, QES_BASE, 0, 0);
        for (const shen_name character : {QES_REN, QES_YUA}) {
            EV_EnsureCharacterDraw(character);
            SetScenarioCharacterFlag40(character, 0);
            EV_EnableCharacterDisplay(character);
            if (character.name == QES_YUA.name) {
                EV_ToggleCharacterFlag(QES2_CHAINSAW);
                EV_EnableCharacterDisplay(QES2_CHAINSAW);
            }
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EV_DisableCharacterDisplay(QES_RYO);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {217, qes2FloorHeight, 144}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 180, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES2_SHOP_REN_POSITION, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 90, 0);
        EVT_AT_SetMot(QES_REN, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetPoint(QES_REN, QES2_SHOK, -1, 4, 1, 0, 45, 0);
        EVT_AT_SetVoice("0295C015", 1, 0); // @text EN: "Hey!" | JA: "おいっ！"
        EVT_AT_SetVoice("0295C016", 1, 10); // @text EN: "We can use that!" | JA: "そいつ使えるぞ！"
        EVT_AT_SetCam(5003, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetCam(5011, QES_BASE, QES_BASE, 0, 0, 35);
        ++work->subphase;
        return 45;
    case 1:
        EV_EnableCharacterDisplay(QES_RYO);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_SHOP_RYO_POSITION, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
        EVT_AT_SetMot(QES_RYO, 8136, 0, 1, 0x40000000U, 0);
        ++work->subphase;
        return 45;
    case 2:
        qteBattleUtil(BattleQteMode::SetWait, 0, 6, scenarioQteRetryCount * 4 + 16);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 1024);
        qteBattleUtil(BattleQteMode::Start);
        ++work->subphase;
        return 5;
    case 3: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetCompletedCount);
        if (result == 2) {
            const std::int32_t delay = Qes2ShopSuccess();
            ++work->subphase;
            return delay;
        }
        if (qteBattleUtil(BattleQteMode::GetCompletedCount) != 1) {
            Queue_EVT_UTL_LoadNPCChara_3(QES2_SURROUND_CHARACTERS, nullptr);
            work->phase = 12;
            work->subphase = 0;
            return 0;
        }
        if (scenarioQteRetryCount < 2)
            ++scenarioQteRetryCount;
        const std::int32_t delay = Qes2ShopFailure();
        work->phase = 1;
        work->subphase = 0;
        return delay;
    }
    case 4:
        if (qes2SoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
                .initial = initial, .target = 0, .frames = 30, .phase = 0, .step = initial ? static_cast<float>(-initial) / 30 : 0};
        }
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(QES2_END_AUTHOR.name), nullptr, nullptr, 30);
        work->phase = 14;
        work->subphase = 0;
        return 0;
    default:
        return 0;
    }
}
std::int32_t Qes2Surrounded(Qes2EventWork *work) {
    if (!g_usingCustomAuthorCharacterList)
        return 0;
    for (std::int32_t index = 0; index < 20; ++index)
        CancelEventMovement(index);
    for (const shen_name character : {QES_REN, QES_YUA, QES2_TB2, QES2_SD7, QES2_WD4, QES2_AD4, QES2_KD7}) {
        EV_EnsureCharacterDraw(character);
        SetScenarioCharacterFlag40(character, 0);
        EV_EnableCharacterDisplay(character);
        if (character.name == QES_YUA.name) {
            EV_ToggleCharacterFlag(QES2_CHAINSAW);
            EV_EnableCharacterDisplay(QES2_CHAINSAW);
        }
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    }
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {217, qes2FloorHeight, 144}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, 180, 0);
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES2_SURROUND_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
    EVT_AT_SetPos(QES_REN, QES_BASE, QES2_SURROUND_REN, 0);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 180, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, {-6.2f, 0, 18.1f}, 110);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 110);
    EVT_AT_SetPos(QES2_TB2, QES_BASE, {-17.6f, 0, 18.8f}, 70);
    EVT_AT_SetAngY(QES2_TB2, QES_BASE, 270, 70);
    EVT_AT_SetPos(QES2_SD7, QES_BASE, {-18.2f, 0, 17.7f}, 70);
    EVT_AT_SetAngY(QES2_SD7, QES_BASE, 270, 70);
    EVT_AT_SetPos(QES2_WD4, QES_BASE, QES2_SURROUND_WD4, 0);
    EVT_AT_SetAngY(QES2_WD4, QES_BASE, 270, 0);
    EVT_AT_SetPos(QES2_AD4, QES_BASE, QES2_SURROUND_AD4, 0);
    EVT_AT_SetAngY(QES2_AD4, QES_BASE, 270, 0);
    EVT_AT_SetPos(QES2_KD7, QES_BASE, QES2_SURROUND_KD7, 0);
    EVT_AT_SetAngY(QES2_KD7, QES_BASE, 270, 0);
    EVT_AT_SetMot(QES_RYO, 0xf006, 5, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_RYO, 0xf003, 5, 1, 0, 75);
    EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_REN, 0xf003, 0, 1, 0, 75);
    EVT_AT_SetMot(QES2_TB2, 8198, 0, 1, 0x40000000U, 70);
    EVT_AT_SetMot(QES2_SD7, 8199, 0, 1, 0x40000000U, 70);
    EVT_AT_SetMot(QES2_WD4, 8198, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES2_AD4, 8199, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES2_KD7, 8200, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_YUA, 8191, 0, 1, 0, 110);
    EVT_AT_SetCam(6002, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(6000, QES_BASE, QES_BASE, 0, 0, 70);
    EVT_AT_SetCam(6001, QES_BASE, QES_BASE, 0, 0, 110);
    EVT_AT_SetCam(6003, QES_BASE, QES_BASE, 0, 0, 135);
    EVT_AT_ObjectDisp_1(QES_RYO, 110, 1);
    EVT_AT_ObjectDisp_1(QES_REN, 110, 1);
    EVT_AT_ObjectDisp_1(QES_RYO, 290, 0);
    EVT_AT_ObjectDisp_1(QES_REN, 290, 0);
    EVT_AT_SetVoice("0295C003", 1, 60); // @text EN: " " | JA: "ちっ…"
    EVT_AT_SetVoice("0295B006", 1, 100); // @text EN: "Are you ready for this?" | JA: "んッふっふっ…\n覚悟をしなさい…"
    EVT_AT_SetVoice("0295B007", 1, 200); // @text EN: " " | JA: "イヤアァァーッ！"
    EVT_AT_SetVoice("0295A013", 1, 230); // @text EN: " " | JA: "うわぁーっ！"
    if (qes2SoundHandle) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        const std::int32_t initial = _mm_cvttss_si32(_mm_set_ss(qes2SoundIntensity));
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {
            .initial = initial, .target = 20, .frames = 30, .phase = 0, .step = initial == 20 ? 0 : static_cast<float>(20 - initial) / 30};
        if (qes2SoundHandle) {
            task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 30);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 40, .target = 30, .frames = 50, .phase = 0, .step = -0.2f};
            if (qes2SoundHandle) {
                task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
                HLib::SetupTimedTask(task, 80);
                *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 30, .target = 10, .frames = 25, .phase = 0, .step = -0.8f};
                if (qes2SoundHandle) {
                    task = HLib::EnqueueTaskWithParameter(Qes2SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
                    HLib::SetupTimedTask(task, 105);
                    *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 10, .target = 2, .frames = 10, .phase = 0, .step = -0.8f};
                }
            }
        }
    }
    QueueQesAngle(QES_RYO, -3, 30, 17);
    QueueQesAngle(QES_REN, -3, 30, 0);
    QueueQesRetryFade(210);
    if (scenarioQteRetryCount < 2)
        ++scenarioQteRetryCount;
    work->phase = 1;
    return 300;
}
std::int32_t Qes2DefeatAudio(Qes2EventWork *work) {
    if (GetRandomNumberInRange(100) >= 50) {
        EVT_AT_SetVoice("S2BVC007", 1, 21);
        EVT_AT_SetVoice("S2BVC009", 1, 57);
        EVT_AT_SetVoice("S2BVA012", 1, 105);
        EVT_AT_SetSound(0x289, 62);
        EVT_AT_SetSound(0x28a, 76);
        EVT_AT_SetSound(0x84, 87);
        EVT_AT_SetSound(0x28d, 98);
        EVT_AT_SetSound(0x28b, 114);
        EVT_AT_SetSound(0x28c, 130);
        EVT_AT_SetSound(0x289, 140);
        EVT_AT_SetSound(0x274, 155);
        EVT_AT_SetSound(0x28c, 160);
        EVT_AT_SetSound(0x28a, 187);
        EVT_AT_SetSound(0x274, 52);
        EVT_AT_SetSound(0x275, 58);
        EVT_AT_SetSound(0x28d, 91);
        EVT_AT_SetSound(0x84, 127);
        EVT_AT_SetSound(0x85, 165);
        EVT_AT_SetSound(0x289, 194);
        EVT_AT_SetSound(0x28d, 103);
        EVT_AT_SetSound(0x274, 160);
        EVT_AT_SetSound(0x275, 166);
        work->phase = 1;
        return 260;
    }
    EVT_AT_SetVoice("S2BVB010", 1, 23);
    EVT_AT_SetVoice("S2BVA010", 1, 92);
    EVT_AT_SetSound(0x28d, 53);
    EVT_AT_SetSound(0x28b, 85);
    EVT_AT_SetSound(0x28c, 115);
    EVT_AT_SetSound(0x28c, 131);
    EVT_AT_SetSound(0x28a, 142);
    EVT_AT_SetSound(0x289, 158);
    EVT_AT_SetSound(0x274, 45);
    EVT_AT_SetSound(0x275, 51);
    EVT_AT_SetSound(0x289, 58);
    EVT_AT_SetSound(0x28a, 70);
    EVT_AT_SetSound(0x28d, 47);
    work->phase = 1;
    return 225;
}
std::int32_t Qes2Finish(Qes2EventWork *work) {
    if (g_currentMap.name == QES2_AREA.name) {
        EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
            .frames = 30,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 60});
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        for (std::int32_t index = 0; index < 20; ++index)
            CancelEventMovement(index);
        work->phase = 1;
        work->subphase = 0;
    } else {
        spmFadeOut();
        HLib::SetCurrentTaskTo(Qes2Cleanup);
    }
    return 60;
}
void Qes2HideCharacters() {
    for (std::int32_t index = 1; QES2_READ_CHARACTERS[index].name != QES_INVALID.name; ++index) {
        EV_DisableCharacterDisplay(QES2_READ_CHARACTERS[index]);
        EV_ToggleCharacterFlag(QES2_READ_CHARACTERS[index]);
    }
    EV_DisableCharacterDisplay(QES2_CHAINSAW);
    EV_ToggleCharacterFlag(QES2_CHAINSAW);
}
void Qes2Callback(void *data) {
    Qes2EventWork *const work = static_cast<Qes2EventWork *>(data);
    if (!work->subphase && work->phase != 0 && work->phase != 14)
        Qes2HideCharacters();
    std::int32_t delay = 0;
    switch (work->phase) {
    case 0:
        delay = Qes2Initialize(work);
        break;
    case 1:
        delay = Qes2FirstChase(work);
        break;
    case 2:
        delay = Qes2FirstPairedFight(work);
        break;
    case 3:
        delay = Qes2FirstFloorboard(work);
        break;
    case 4:
        delay = Qes2SecondFloorboard(work);
        break;
    case 7:
        delay = Qes2Drawers(work);
        break;
    case 8:
        delay = Qes2SecondPairedFight(work);
        break;
    case 9:
        delay = Qes2TinEncounter(work);
        break;
    case 10:
        delay = Qes2TinClear(work);
        break;
    case 11:
        delay = Qes2ShopEscape(work);
        break;
    case 12:
        delay = Qes2Surrounded(work);
        break;
    case 13:
        delay = Qes2DefeatAudio(work);
        break;
    case 14:
        delay = Qes2Finish(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void Qes2Enqueue(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes2Callback, 4, 0x0b, sizeof(Qes2EventWork), QES_TASK);
    Qes2EventWork *const work = static_cast<Qes2EventWork *>(HLib::GetTaskParameter(task));
    std::memset(reinterpret_cast<std::byte *>(work) + 8, 0, 0x20);
    work->task = task;
    work->characters = Queue_EVT_AT_ReadChr(task, QES2_READ_CHARACTERS);
    work->phase = 0;
}
void Qes2AdmissionNoop() {
}
} // namespace
void Qes2Cleanup(void *data) {
    Qes2EventWork *const work = static_cast<Qes2EventWork *>(data);
    const shen_name *lists[]{QES2_FIRST_PAIR_CHARACTERS, QES2_SECOND_PAIR_CHARACTERS, QES2_SURROUND_CHARACTERS};
    for (const shen_name *list : lists)
        for (std::int32_t index = 0; list[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(list[index])) {
                EV_npcLockUnlock(list[index]);
                EV_npcFree(list[index]);
            }
        }
    qteBattleUtil(BattleQteMode::RetireAndFree);
    EV_FreeAuthorPak(0, "0295");
    EV_FreeAuthorPak(0, "S2BV");
    if (qes2SoundHandle) {
        FreeMaterialSoundHandle(qes2SoundHandle);
        qes2SoundHandle = nullptr;
    }
    SetScenarioCharacterFlag40(QES_RYO, 1);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 90);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 90);
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    EV_stepSetFlags(0, 354);
    EV_stepSetFlags(0, 395);
    EventCling cling{.cleanup = 0, .target = QES2_CHAINSAW, .mode = 1};
    EVT_AT_Cling(&cling);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScenarioCharacterKillMode(QES_BASE, 1);
    SetScenarioCharacterKillMode(QES_DUMMY, 1);
    ET_AT_KillWork(work->characters);
    EVT_AT_SetPos(QES_RYO, QES_INVALID, QES2_CLEANUP_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_INVALID, 270, 0);
    EVT_AT_SetMot(QES_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EV_LoadCutsceneFromCurrentScene(eventSavedBgmName.data());
    EV_LoadAudio(eventSavedSoundBankName.data(), eventSavedSoundScene);
    EV_spmBeginSampleExecute();
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES2"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES2"), 0);
    scenFreeEvent("QES2.EV3");
    EV_NPC_NewLoadSw(1);
    FinishLoading();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void Qes2Init() {
    qes2EntryMode = 0;
    qes2FloorHeight = 0;
    qes2SoundHandle = nullptr;
    for (std::int32_t index = 0; index < 3; ++index)
        qes2FirstPairedCharacters[index] = shen_name{};
    qes2FirstPairedPhase = 0;
    for (std::int32_t index = 0; index < 3; ++index)
        qes2SecondPairedCharacters[index] = shen_name{};
    qes2SoundIntensity = 0;
    qes2SecondPairedPhase = 0;
}
void Qes2Main(std::uintptr_t argument) {
    Qes2AdmissionNoop();
    if (g_currentMap.name == QES2_AREA.name) {
        qes2EntryMode = static_cast<std::int16_t>(g_entryPoint & 1);
        EV_lightLoad({});
        PushGlobalLightingProgram();
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    } else
        qes2EntryMode = static_cast<std::int16_t>(argument);
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), QES_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = Qes2Enqueue;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
const ScriptAreaDefinition qes2Definition{.areaId = QES2_AREA, .field04 = 0, .init = Qes2Init, .main = Qes2Main};
} // namespace shenmue::s2
