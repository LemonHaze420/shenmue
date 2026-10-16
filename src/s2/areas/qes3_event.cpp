#include "s2/areas/qes3_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/handcuffs.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/sound_program.hpp"
#include <bit>
#include <xmmintrin.h>
namespace shenmue::s2 {
std::int32_t qes3MotionHandle, qes3RetryCanSkip;
float qes3FloorHeight, qes3SoundIntensity;
SoundEffectHandle *qes3SoundHandle;
Qes3SparkBank qes3Sparks;
void Qes3SoundIntensityCallback(void *data) {
    QesSoundIntensityWork *const work = static_cast<QesSoundIntensityWork *>(data);
    if (work->phase) {
        qes3SoundIntensity += work->step;
        if (--work->frames <= 0) {
            qes3SoundIntensity = static_cast<float>(work->target);
            HLib::DestroyCurrentTask();
        }
    } else {
        work->phase = 1;
        qes3SoundIntensity = static_cast<float>(work->initial);
        if (work->initial == work->target)
            HLib::DestroyCurrentTask();
    }
    ExecuteMaterialSoundHandleCommand(qes3SoundHandle, 0xa6, _mm_cvttss_si32(_mm_set_ss(qes3SoundIntensity)));
}
void QueueQes3SoundIntensity(std::int32_t initial, std::int32_t target, std::int32_t frames, std::int32_t delay) {
    if (!qes3SoundHandle)
        return;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes3SoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    QesSoundIntensityWork *const work = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
    *work = {.initial = initial,
        .target = target,
        .frames = frames,
        .phase = 0,
        .step = initial == target ? 0 : static_cast<float>(target - initial) / static_cast<float>(frames)};
}
void CreateQes3Sparks(std::int16_t index, const Vector3 &position, float floorHeight, float rate) {
    if (index > 4 || qes3Sparks.primary[index] != -1)
        return;
    const Vector3 primaryVolume{0.1f, 0, 0.1f}, secondaryVolume{0.05f, 0, 0.05f}, primaryVariance{0.5f, 0.5f, 0.5f}, secondaryVariance{0.02f, 0.02f, 0.02f};

    Vector3 primaryVelocity;
    primaryVelocity.x = 0;
    primaryVelocity.y = 0.1f;
    const Vector3 secondaryVelocity{0.05f, 0.03f, 0};
    qes3Sparks.primary[index] =
        static_cast<std::int16_t>(CreateSplashEffect(position, primaryVolume, 300, 0, {}, std::bit_cast<std::int32_t>(floorHeight), nullptr, nullptr));
    SetParticleEffectColour(1, qes3Sparks.primary[index], 255, 255, 112, 16);
    SetParticleEffectVelocity(1, qes3Sparks.primary[index], primaryVelocity, primaryVariance);
    SetParticleEffectSize(1, qes3Sparks.primary[index], 0.03f, 0.03f);
    ResumeParticleEffect(1, qes3Sparks.primary[index]);
    qes3Sparks.secondary[index] =
        static_cast<std::int16_t>(CreateSplashEffect(position, secondaryVolume, 1000, 0, {}, std::bit_cast<std::int32_t>(floorHeight), nullptr, nullptr));
    SetParticleEffectColour(1, qes3Sparks.secondary[index], 255, 255, 160, 32);
    SetParticleEffectVelocity(1, qes3Sparks.secondary[index], secondaryVelocity, secondaryVariance);
    SetParticleEffectSize(1, qes3Sparks.secondary[index], 0.03f, 0.03f);
    ResumeParticleEffect(1, qes3Sparks.secondary[index]);
    qes3Sparks.rates[index] = rate;
    qes3Sparks.previous[index] = position;
}
void UpdateQes3Sparks(std::int16_t index, const Vector3 &position) {
    if (index > 4 || qes3Sparks.primary[index] == -1)
        return;
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const float cameraDistance = GetEuclideanDistance(&position, &camera), distance = GetPlanarDistance(&qes3Sparks.previous[index], &position);
    const float distanceSquared = distance * distance;
    const Vector3 primaryVolume{0.1f, 0, 0.1f}, secondaryVolume{0.05f, 0, 0.05f}, variance{0.02f, 0.02f, 0.02f};
    SetParticleEffectPosition(1, qes3Sparks.primary[index], &position, &primaryVolume);
    SetParticleEffectBirthRate(1, qes3Sparks.primary[index], 0.1f);
    SetParticleEffectBirthRate(1, qes3Sparks.primary[index], (distanceSquared * 0.3f) * qes3Sparks.rates[index]);
    SetParticleEffectSize(1, qes3Sparks.primary[index], cameraDistance * 0.002f, cameraDistance * 0.002f);
    const Vector3 velocity{(position.x - qes3Sparks.previous[index].x) * 0.5f, 0.03f, (position.z - qes3Sparks.previous[index].z) * 0.5f};
    SetParticleEffectPosition(1, qes3Sparks.secondary[index], &position, &secondaryVolume);
    SetParticleEffectVelocity(1, qes3Sparks.secondary[index], velocity, variance);
    SetParticleEffectBirthRate(1, qes3Sparks.secondary[index], (distanceSquared * 0.4f) * qes3Sparks.rates[index]);
    SetParticleEffectBirthRate(1, qes3Sparks.secondary[index], 0.1f);
    SetParticleEffectSize(1, qes3Sparks.secondary[index], cameraDistance * 0.004f, cameraDistance * 0.004f);
    qes3Sparks.previous[index] = position;
}
void FreeQes3Sparks(std::int16_t index) {
    if (index > 4 || qes3Sparks.primary[index] == -1)
        return;
    FreeParticleEffect(1, qes3Sparks.primary[index]);
    FreeParticleEffect(1, qes3Sparks.secondary[index]);
    qes3Sparks.primary[index] = -1;
    qes3Sparks.secondary[index] = -1;
}
void FreeAllQes3Sparks() {
    for (std::int32_t index = 0; index < 4; ++index)
        if (qes3Sparks.primary[index] != -1) {
            FreeParticleEffect(1, qes3Sparks.primary[index]);
            FreeParticleEffect(1, qes3Sparks.secondary[index]);
            qes3Sparks.primary[index] = -1;
            qes3Sparks.secondary[index] = -1;
        }
}
void Qes3SparkCallback(void *data) {
    Qes3SparkWork *const work = static_cast<Qes3SparkWork *>(data);
    if (work->frames-- <= 0) {
        if (work->index <= 4 && qes3Sparks.primary[work->index] != -1) {
            SetParticleEffectBirthRate(1, qes3Sparks.primary[work->index], 0);
            SetParticleEffectBirthRate(1, qes3Sparks.secondary[work->index], 0);
        }
        if (work->retireFrames-- < 0) {
            FreeQes3Sparks(work->index);
            HLib::DestroyCurrentTask();
        }
    } else {
        Vector3 position, point = work->offset;
        Vector3i rotation;
        EV_GetCharaPosition(QES3_POLE, 0xffffffffU, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(QES3_POLE, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
        UpdateQes3Sparks(work->index, point);
    }
}
std::int32_t Qes3PoleFailure() {
    EVT_AT_SetPos(QES_RYO, QES3_KT8, {-0.2f, 0, -2.2f}, 0);
    EVT_AT_SetAngY(QES_RYO, QES3_KT8, 180, 0);
    EVT_AT_SetAngY(QES_RYO, QES3_KT8, 180, 5);
    EVT_AT_SetAngY(QES3_KT8, QES3_KT8, 45, 0);
    EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_RYO, 8451, 10, 1, 0, 5);
    EVT_AT_SetMot(QES3_KT8, 8452, 10, 1, 0x40000000U, 0);
    EVT_AT_SetCam(2002, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0296A003", 1, 10); // @text EN: " " | JA: "うぐっ！"
    return 60;
}
std::int32_t Qes3PoleSuccess() {
    QueueQes3SoundIntensity(70, 40, 40, 0);
    EVT_AT_SetPos(QES_RYO, QES3_KT8, {0, 0, -1.5f}, 0);
    EVT_AT_SetAngY(QES_RYO, QES3_KT8, 180, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
    RegisterEventTask(task);
    EventCling *const cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = QES3_POLE;
    cling->character = {};
    cling->mode = 1;
    cling->delay = 20;
    EVT_AT_SetPos(QES3_DUMMY, QES_INVALID, {219.89999389648438f, qes3FloorHeight, 169}, 0);
    EventSetRawAngle angle{.cleanup = 0, .character = QES3_DUMMY, .parent = QES_INVALID, .rotation = {0, 0, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    EVT_AT_SetMot(QES_RYO, 8450, 10, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES3_KT8, 8449, 15, 1, 0x40000000U, 0);
    Queue_EVT_AT_PlayDynamix(QES3_POLE, "PIPE", 65, 200, 0, nullptr, 1.4f, 20);
    EVT_AT_SetSound(535, 40);
    EVT_AT_SetCam(2001, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0296A004", 1, 10); // @text EN: " " | JA: "ふんっ！"
    EVT_AT_SetVoice("S2BVB031", 1, 30);
    return 40;
}
std::int32_t Qes3FirstChainsawFailure() {
    EventCling cling{.cleanup = 0,
        .target = QES_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = {0.193f, 0.197f, 0.098f},
        .rotation = {1396, 56023, 40636},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EVT_AT_SetPos(QES_YUA, QES_BASE, QES3_CHAIN_FAILURES[0], 0);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 150, 0);
    EVT_AT_SetMot(QES_YUA, 8467, 0, 0.5f, 0, 0);
    EVT_AT_SetCam(3005, QES_BASE, QES_BASE, 0, 0, 0);
    QueueCameraShake(0.4f, 0.4f, 0.7f, 10);
    EVT_AT_SetSound(1387, 15);
    EVT_AT_SetVoice("0296A006", 1, 10); // @text EN: " " | JA: "うわあぁーっ！"
    return 20;
}
std::int32_t Qes3FirstChainsawSuccess() {
    EVT_AT_SetPos(QES_YUA, QES_RYO, QES3_FIRST_CHAIN_SUCCESS, 0);
    EVT_AT_SetAngY(QES_YUA, QES_RYO, 0, 0);
    EventCling cling{.cleanup = 0,
        .target = QES_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = {0.323f, 0.094f, 0.016f},
        .rotation = {-3453, -13721, -27505},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EVT_AT_SetMot(QES_RYO, 8455, 0, 0.9f, 0, 0);
    EVT_AT_SetMot(QES_YUA, 8464, 0, 0.9f, 0, 0);
    EVT_AT_SetCam(3001, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetSound(1386, 10);
    EVT_AT_SetVoice("0296A002", 1, 10); // @text EN: " " | JA: "はっ！"
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes3SparkCallback, 6, 0x0b, sizeof(Qes3SparkWork), QES_TASK);
    *static_cast<Qes3SparkWork *>(HLib::GetTaskParameter(task)) = {.index = 0, .reserved02 = 0, .offset = {0, 0.3f, 0}, .frames = 20, .retireFrames = 0};
    CreateQes3Sparks(0, {0, 0, 0}, qes3FloorHeight, 4);
    return 0;
}
std::int32_t Qes3SecondChainsawFailure() {
    EVT_AT_SetPos(QES_YUA, QES_BASE, QES3_CHAIN_FAILURES[1], 0);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 180, 0);
    EventCling cling{.cleanup = 0,
        .target = QES_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = {0.254f, 0.167f, -0.192f},
        .rotation = {48046, 61921, 56769},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EVT_AT_SetMot(QES_YUA, 8468, 0, 1, 0, 0);
    EVT_AT_SetCam(3006, QES_BASE, QES_BASE, 0, 0, 0);
    QueueCameraShake(0.6f, 0.2f, 0.7f, 10);
    EVT_AT_SetSound(1387, 10);
    EVT_AT_SetVoice("0296A006", 1, 10); // @text EN: " " | JA: "うわあぁーっ！"
    return 20;
}
std::int32_t Qes3SecondChainsawSuccess() {
    QueueQes3SoundIntensity(1, 1, 0, 5);
    QueueQes3SoundIntensity(2, 2, 0, 50);
    EVT_AT_SetPos(QES_YUA, QES_RYO, QES3_SECOND_CHAIN_SUCCESS, 0);
    EVT_AT_SetAngY(QES_YUA, QES_RYO, 0, 0);
    EVT_AT_SetMot(QES_RYO, 8456, 0, 0.9f, 0, 0);
    EVT_AT_SetMot(QES_YUA, 8465, 0, 0.9f, 0, 0);
    EVT_AT_SetCam(3002, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetSound(1386, 5);
    EVT_AT_SetVoice("0296A004", 1, 0); // @text EN: " " | JA: "ふんっ！"
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes3SparkCallback, 6, 0x0b, sizeof(Qes3SparkWork), QES_TASK);
    HLib::SetupTimedTask(task, 5);
    *static_cast<Qes3SparkWork *>(HLib::GetTaskParameter(task)) = {.index = 1, .reserved02 = 0, .offset = {0, 0.3f, 0}, .frames = 45, .retireFrames = 25};
    CreateQes3Sparks(1, {0, 0, 0}, qes3FloorHeight, 4);
    return 40;
}
std::int32_t Qes3ThirdChainsawSuccess() {
    EVT_AT_SetPos(QES_YUA, QES_RYO, QES3_SECOND_CHAIN_SUCCESS, 0);
    EVT_AT_SetAngY(QES_YUA, QES_RYO, 0, 0);
    EVT_AT_SetMot(QES_RYO, 8457, 0, 0.9f, 0x40000000U, 0);
    EVT_AT_SetMot(QES_YUA, 8466, 0, 0.9f, 0x40000000U, 0);
    EVT_AT_SetCam(3003, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(3009, QES_BASE, QES_BASE, 0, 0, 30);
    EVT_AT_SetCam(3004, QES_BASE, QES_BASE, 0, 0, 90);
    EVT_AT_SetSound(1386, 15);
    QueueQes3SoundIntensity(1, 1, 0, 5);
    QueueQes3SoundIntensity(2, 2, 0, 110);
    EVT_AT_SetVoice("0296A005", 1, 120); // @text EN: " " | JA: "やっ！"
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes3SparkCallback, 6, 0x0b, sizeof(Qes3SparkWork), QES_TASK);
    HLib::SetupTimedTask(task, 15);
    *static_cast<Qes3SparkWork *>(HLib::GetTaskParameter(task)) = {.index = 2, .reserved02 = 0, .offset = {0, 0.3f, 0}, .frames = 80, .retireFrames = -35};
    CreateQes3Sparks(2, {0, 0, 0}, qes3FloorHeight, 4);
    return 140;
}
std::int32_t Qes3ThirdChainsawFailure() {
    EV_DisableCharacterDisplay(QES_RYO);
    EVT_AT_SetPos(QES_YUA, QES_BASE, QES3_CHAIN_FAILURES[2], 0);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 0, 0);
    EventCling cling{.cleanup = 0,
        .target = QES_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = {0.335f, 0.058f, -0.055f},
        .rotation = {20264, 50791, 11494},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EVT_AT_SetMot(QES_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_YUA, 8469, 0, 1, 0, 0);
    EVT_AT_SetCam(3007, QES_BASE, QES_BASE, 0, 0, 0);
    QueueCameraShake(0.2f, 0.4f, 0.7f, 10);
    EVT_AT_SetSound(1387, 10);
    EVT_AT_SetVoice("0296A006", 1, 10); // @text EN: " " | JA: "うわあぁーっ！"
    return 20;
}
void Qes3AuthorAudioCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 70)
        QueueQes3SoundIntensity(0, 0, 0, 0);
}
void Qes3AfterFightAuthorCallback(void *) {
    switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
    case 0:
        EV_SetObjectFlagByIndex(3, 0);
        break;
    case 581:
    case 1001:
    case 1311:
    case 1511:
    case 1711:
        Handcuffs_Attach(QES_RYO, 0, QES_YUA, 1, 0.3f);
        break;
    case 881:
    case 1050:
    case 1341:
    case 1631:
    case 1926:
    case 2221:
        cuffsSetFlags();
        break;
    case 2030:
        Handcuffs_Attach(QES_RYO, 0, QES_YUA, 1, 0.3f);
        EV_SetObjectFlagByIndex(3, 1);
        break;
    default:
        break;
    }
}
void RestoreQes3FightCharacter(shen_name character) {
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    SetMotionFrameTime(scenFindMotionwork(character), 7, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 69, 2, MotionRegion::LowerBody);
    EV_SetCharacterAlphaEnabled(character, false);
    mtwkSetExtraFlag(scenFindMotionwork(character), 55, 2);
}
} // namespace shenmue::s2
