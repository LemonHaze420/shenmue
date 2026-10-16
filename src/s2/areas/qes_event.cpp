#include "s2/areas/qes_event.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include <xmmintrin.h>
namespace shenmue::s2 {
SoundEffectHandle *qesSoundHandle;
float qesSoundIntensity, qesFloorHeight;
std::uint16_t qesMissedInputs;
shen_name qesPairedCharacters[3];
std::int16_t qesPairedPhase;
void QesAngleCallback(void *data) {
    QesAngleWork *const work = static_cast<QesAngleWork *>(data);
    const std::int32_t frames = work->frames--;
    if (frames <= 0)
        HLib::DestroyCurrentTask();
    else {
        EventSetAngle angle{.cleanup = 0, .character = work->character, .parent = work->character, .degrees = work->degrees, .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
    }
}
QesAngleWork *QueueQesAngle(shen_name character, float degrees, std::int32_t frames, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesAngleCallback, 6, 0x0b, sizeof(QesAngleWork), QES_TASK);
    HLib::SetupTimedTask(task, delay);
    QesAngleWork *const work = static_cast<QesAngleWork *>(HLib::GetTaskParameter(task));
    work->character = character;
    work->degrees = degrees;
    work->frames = frames;
    return work;
}
void QesSoundIntensityCallback(void *data) {
    QesSoundIntensityWork *const work = static_cast<QesSoundIntensityWork *>(data);
    if (work->phase) {
        qesSoundIntensity += work->step;
        if (--work->frames <= 0) {
            qesSoundIntensity = static_cast<float>(work->target);
            HLib::DestroyCurrentTask();
        }
    } else {
        work->phase = 1;
        qesSoundIntensity = static_cast<float>(work->initial);
        if (work->initial == work->target)
            HLib::DestroyCurrentTask();
    }
    ExecuteMaterialSoundHandleCommand(qesSoundHandle, 0xa6, _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity)));
}
void QesCameraCallback(void *data) {
    QesCameraWork *const work = static_cast<QesCameraWork *>(data);
    camCtrlSetSplineShouldRebuildRotTo(true);
    const bool unnamed = work->eye.name == 0xffffffffU && work->target.name == 0xffffffffU;
    EVS_LoopCamera(work->camera, unnamed ? shen_name{} : work->eye, unnamed ? shen_name{} : work->target);
    if (work->cleanup)
        HLib::DestroyCurrentTask();
}
void PrepareQesCharacter(shen_name character) {
    HasCharacterMotionWork(ctrlGetCharaByName(character));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterDrawControl(character, 1, 1);
    AT_SetLocalBaseChara(character, QES_INVALID);
    figpSetFlagsBlendForBlur(character, 2, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
    mtwkSetExtraFlag(scenFindMotionwork(character), 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(character), 3, 2);
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    SetMotionFrameTime(scenFindMotionwork(character), 7, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 66, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 69, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 71, 2, MotionRegion::LowerBody);
}
void QueueQesRetryFade(std::int32_t delay) {
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 255,
        .targetGreen = 255,
        .targetBlue = 255,
        .delay = static_cast<float>(delay)});
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 255,
        .startGreen = 255,
        .startBlue = 255,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay + 60)});
}
namespace {
std::int32_t QesPairFirstSuccess() {
    EVT_AT_DispBlur(qesPairedCharacters[0], 8, 10, 1, 20, 0);
    EV_EnableCharacterDisplay(qesPairedCharacters[2]);
    EVT_AT_SetPos(qesPairedCharacters[2], QES_BASE, QES_PAIR_SECOND_POSITION, 0);
    EVT_AT_SetAngY(qesPairedCharacters[2], QES_BASE, 180, 0);
    EVT_AT_SetMot(qesPairedCharacters[2], 7914, 0, 1, 0x60000000U, 0);
    EVT_AT_SetVoice("S2BVA005", 1, 20);
    return 0;
}
std::int32_t QesPairFirstFailure() {
    EVT_AT_SetPos(qesPairedCharacters[0], QES_BASE, {0, 0, 2.5f}, 0);
    EVT_AT_SetAngY(qesPairedCharacters[0], QES_BASE, 0, 0);
    EVT_AT_SetPos(qesPairedCharacters[1], qesPairedCharacters[0], {0, 0, 0}, 0);
    EVT_AT_SetAngY(qesPairedCharacters[1], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qesPairedCharacters[0], 7896, 40, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[1], 7905, 40, 1, 0x64000000U, 0);
    EVT_AT_SetFaceLooks(qesPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 10);
    EVT_AT_SetFaceLooks(qesPairedCharacters[0], FacialExpressionId::Angry, 8, 1, 30);
    EVT_AT_SetVoice("S2BVB008", 1, 0);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7013, QES_BASE, QES_BASE);
    return 30;
}
std::int32_t QesPairDoubleFailure() {
    EVT_AT_SetPos(qesPairedCharacters[1], qesPairedCharacters[0], QES_PAIR_DOUBLE_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qesPairedCharacters[1], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qesPairedCharacters[0], 7899, 30, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[1], 7907, 30, 1, 0x64000000U, 0);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7002, QES_BASE, QES_BASE);
    EVT_AT_SetVoice("S2BVB009", 1, 0);
    EVT_AT_SetFaceLooks(qesPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 5);
    return 60;
}
std::int32_t QesPairSecondSuccess() {
    EVT_AT_SetPos(qesPairedCharacters[1], qesPairedCharacters[0], QES_PAIR_SPLIT_FAILURE_POSITIONS[0], 0);
    EVT_AT_SetAngY(qesPairedCharacters[1], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetPos(qesPairedCharacters[2], qesPairedCharacters[0], QES_PAIR_SPLIT_FAILURE_POSITIONS[1], 0);
    EVT_AT_SetAngY(qesPairedCharacters[2], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qesPairedCharacters[0], 7898, 0, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[1], 7906, 0, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[2], 7912, 0, 1, 0x64000000U, 0);
    EVT_AT_DispBlur(qesPairedCharacters[0], 7, 10, 2, 5, 0);
    EVT_AT_DispBlur(qesPairedCharacters[0], 7, 10, 2, 10, 30);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7003, QES_BASE, QES_BASE);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(QesCameraCallback, 6, 0x0b, sizeof(QesCameraWork), QES_TASK);
    HLib::SetupTimedTask(task, 30);
    QesCameraWork *work = static_cast<QesCameraWork *>(HLib::GetTaskParameter(task));
    *work = {.cleanup = 1, .camera = 7004, .eye = QES_BASE, .target = QES_BASE};
    task = HLib::EnqueueTaskWithParameter(QesCameraCallback, 6, 0x0b, sizeof(QesCameraWork), QES_TASK);
    HLib::SetupTimedTask(task, 60);
    work = static_cast<QesCameraWork *>(HLib::GetTaskParameter(task));
    *work = {.cleanup = 1, .camera = 7015, .eye = QES_BASE, .target = QES_BASE};
    EVT_AT_SetVoice("S2BVC020", 1, 40);
    EVT_AT_SetVoice("S2BVB022", 1, 50);
    return 100;
}
std::int32_t QesPairSecondFailure() {
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7006, QES_BASE, QES_BASE);
    EVT_AT_SetPos(qesPairedCharacters[2], qesPairedCharacters[0], QES_PAIR_SECOND_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qesPairedCharacters[2], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qesPairedCharacters[0], 7897, 10, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[2], 7911, 10, 1, 0x64000000U, 0);
    EVT_AT_SetVoice("S2BVA009", 1, 15);
    return 45;
}
std::int32_t QesPairSecondRecovery() {
    EVT_AT_DispBlur(qesPairedCharacters[0], 1, 10, 2, 15, 0);
    EVT_AT_DispBlur(qesPairedCharacters[0], 5, 10, 2, 20, 20);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7011, QES_BASE, QES_BASE);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesCameraCallback, 6, 0x0b, sizeof(QesCameraWork), QES_TASK);
    HLib::SetupTimedTask(task, 40);
    QesCameraWork *const work = static_cast<QesCameraWork *>(HLib::GetTaskParameter(task));
    *work = {.cleanup = 1, .camera = 7014, .eye = QES_BASE, .target = QES_BASE};
    EVT_AT_SetFaceLooks(qesPairedCharacters[2], FacialExpressionId::Suffer, 8, 1, 5);
    EV_PlayDialogue("S2BVA002");
    EVT_AT_SetVoice("S2BVC023", 1, 35);
    return 60;
}
std::int32_t QesPairCounterFailure() {
    EVT_AT_SetAngY(qesPairedCharacters[0], qesPairedCharacters[0], -45, 0);
    EVT_AT_SetPos(qesPairedCharacters[2], qesPairedCharacters[0], QES_PAIR_COUNTER_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(qesPairedCharacters[2], qesPairedCharacters[0], 0, 0);
    EVT_AT_SetMot(qesPairedCharacters[0], 7884, 20, 1, 0x64000000U, 0);
    EVT_AT_SetMot(qesPairedCharacters[2], 7909, 20, 1, 0x64000000U, 0);
    EVT_AT_SetFaceLooks(qesPairedCharacters[0], FacialExpressionId::Suffer, 8, 1, 5);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7010, QES_BASE, QES_BASE);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesCameraCallback, 6, 0x0b, sizeof(QesCameraWork), QES_TASK);
    HLib::SetupTimedTask(task, 30);
    QesCameraWork *const work = static_cast<QesCameraWork *>(HLib::GetTaskParameter(task));
    *work = {.cleanup = 1, .camera = 7012, .eye = QES_BASE, .target = QES_BASE};
    EVT_AT_SetVoice("S2BVC013", 1, 0);
    EVT_AT_SetVoice("S2BVA012", 1, 30);
    return 60;
}
std::int32_t QesPairCounterSuccess() {
    EVT_AT_DispBlur(qesPairedCharacters[0], 1, 10, 2, 20, 0);
    camCtrlSetSplineShouldRebuildRotTo(true);
    EVS_LoopCamera(7008, QES_BASE, QES_BASE);
    EV_PlayDialogue("S2BVC004");
    EVT_AT_SetVoice("S2BVA003", 1, 10);
    return 35;
}
} // namespace
void InitializeQesPairedSequence(float x, float z, float degrees) {
    qesPairedPhase = 0;
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {x, qesFloorHeight, z}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, degrees, 0);
}
std::int32_t UpdateQesPairedSequence(std::int32_t *result) {
    switch (qesPairedPhase) {
    case 0:
        EV_EnableCharacterDisplay(qesPairedCharacters[1]);
        EV_DisableCharacterDisplay(qesPairedCharacters[2]);
        EVT_AT_SetPos(qesPairedCharacters[0], QES_BASE, {0, 0, 2.5f}, 0);
        EVT_AT_SetAngY(qesPairedCharacters[0], QES_BASE, 0, 0);
        EVT_AT_SetPos(qesPairedCharacters[1], qesPairedCharacters[0], QES_PAIR_FIRST_POSITION, 0);
        EVT_AT_SetAngY(qesPairedCharacters[1], qesPairedCharacters[0], 0, 0);
        EVT_AT_SetMot(qesPairedCharacters[0], 7893, 0, 1, 0x60000000U, 0);
        EVT_AT_SetMot(qesPairedCharacters[1], 7904, 0, 1, 0x60000000U, 0);
        QueueSkQteWait(result, 256, 24, 20, QesPairFirstSuccess, QesPairFirstFailure, 10);
        camCtrlSetSplineShouldRebuildRotTo(true);
        EVS_LoopCamera(7000, QES_BASE, QES_BASE);
        qesPairedPhase = 1;
        break;
    case 1:
        qesPairedPhase = result[3] ? 4 : 2;
        break;
    case 2:
        EV_EnableCharacterDisplay(qesPairedCharacters[2]);
        EVT_AT_SetPos(qesPairedCharacters[2], QES_BASE, QES_PAIR_SECOND_POSITION, 0);
        EVT_AT_SetAngY(qesPairedCharacters[2], QES_BASE, 180, 0);
        EVT_AT_SetMot(qesPairedCharacters[2], 7914, 0, 1, 0x60000000U, 0);
        QueueSkQteWait(result, 512, 24, 20, QesPairSecondSuccess, QesPairDoubleFailure, 0);
        camCtrlSetSplineShouldRebuildRotTo(true);
        EVS_LoopCamera(7001, QES_BASE, QES_BASE);
        qesPairedPhase = 3;
        break;
    case 3:
    case 7:
        qesPairedPhase = result[3] ? 8 : 9;
        break;
    case 4: {
        EVT_AT_SetPos(qesPairedCharacters[2], qesPairedCharacters[0], {-0.5f, 0, -2}, 30);
        EVT_AT_SetAngY(qesPairedCharacters[2], qesPairedCharacters[0], 0, 30);
        EVT_AT_SetMot(qesPairedCharacters[0], 7894, 0, 1, 0x60000000U, 30);
        EVT_AT_SetMot(qesPairedCharacters[2], 7910, 0, 1, 0x60000000U, 30);
        QueueSkQteWait(result, 4, 24, 20, QesPairSecondRecovery, QesPairSecondFailure, 20);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesCameraCallback, 6, 0x0b, sizeof(QesCameraWork), QES_TASK);
        HLib::SetupTimedTask(task, 30);
        QesCameraWork *const work = static_cast<QesCameraWork *>(HLib::GetTaskParameter(task));
        *work = {.cleanup = 1, .camera = 7005, .eye = QES_BASE, .target = QES_BASE};
        qesPairedPhase = 5;
        break;
    }
    case 5:
        qesPairedPhase = result[3] ? 8 : 6;
        break;
    case 6:
        EVT_AT_SetPos(qesPairedCharacters[2], qesPairedCharacters[0], QES_PAIR_COUNTER_POSITION, 0);
        EVT_AT_SetAngY(qesPairedCharacters[2], qesPairedCharacters[0], 0, 0);
        EVT_AT_SetMot(qesPairedCharacters[0], 7883, 5, 1, 0x60000000U, 0);
        EVT_AT_SetMot(qesPairedCharacters[2], 7908, 5, 1, 0x60000000U, 0);
        camCtrlSetSplineShouldRebuildRotTo(true);
        EVS_LoopCamera(7007, QES_BASE, QES_BASE);
        QueueSkQteWait(result, 512, 24, 20, QesPairCounterSuccess, QesPairCounterFailure, 0);
        qesPairedPhase = 7;
        break;
    case 8:
    case 9:
        RemoveConversationSpeakerAlias(qesPairedCharacters[1]);
        RemoveConversationSpeakerAlias(qesPairedCharacters[2]);
        return qesPairedPhase == 8 ? 1 : 0;
    default:
        break;
    }
    return -1;
}
} // namespace shenmue::s2
