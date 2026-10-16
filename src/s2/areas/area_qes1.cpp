#include "s2/areas/area_qes1.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/game_runtime.hpp"
#include <cstring>
#include <xmmintrin.h>
namespace shenmue::s2 {
namespace {
void Qes1Callback(void *);
std::int32_t Qes1RubbleSuccess() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = QES1_RUBBLE_SUCCESS, .degrees = 90, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 7895, 0, 1, 0x60000000U, 0);
    EVT_AT_DispBlur(QES_RYO, 1, 8, 2, 30, 0);
    EVT_AT_SetCam(1024, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0291A008", 1, 0);
    return 30;
}
std::int32_t Qes1RubbleFailure() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = QES1_RUBBLE_FAILURE, .degrees = 90, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 7882, 50, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1025, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0291A009", 1, 0);
    return 20;
}
std::int32_t Qes1KickSuccess() {
    EVT_AT_SetSound(650, 10);
    EVT_AT_SetSound(642, 30);
    EVT_AT_SetPos(QES_KU1, QES_RYO, QES1_KICK_SUCCESS_KU1, 0);
    EVT_AT_SetAngY(QES_KU1, QES_RYO, 180, 0);
    EVT_AT_SetAngY(QES_KU1, QES_RYO, 0, 15);
    EVT_AT_SetMot(QES_RYO, 7885, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_KU1, 7873, 0, 1, 0, 15);
    EVT_AT_SetFaceLooks(QES_KU1, FacialExpressionId::Suffer, 8, 1, 15);
    EVT_AT_DispBlur(QES_RYO, 6, 10, 2, 30, 0);
    EVT_AT_SetCam(1018, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0291A010", 1, 0);
    return 30;
}
std::int32_t Qes1KickFailure() {
    EVT_AT_SetSound(649, 15);
    EVT_AT_SetPos(QES_KU1, QES_RYO, QES1_KICK_FAILURE_KU1, 0);
    EVT_AT_SetAngY(QES_KU1, QES_RYO, 0, 0);
    EVT_AT_SetAngY(QES_KK6, QES_KK6, 45, 0);
    EVT_AT_SetMot(QES_RYO, 7887, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_KU1, 7875, 0, 1, 0x40000000U, 0);
    EVT_AT_DispBlur(QES_RYO, 1, 12, 1, 15, 20);
    EVT_AT_SetCam(1020, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0291A009", 1, 0);
    return 15;
}
std::int32_t Qes1CounterSuccess() {
    EVT_AT_SetCam(1021, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_DispBlur(QES_RYO, 6, 8, 1, 20, 10);
    EVT_AT_SetSound(649, 20);
    EVT_AT_SetSound(642, 45);
    EVT_AT_SetVoice("0291A012", 1, 0);
    return 50;
}
std::int32_t Qes1CounterFailure() {
    EVT_AT_SetCam(1029, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetPos(QES_KU1, QES_RYO, QES1_KICK_FAILURE_POSITION, 0);
    EVT_AT_SetAngY(QES_KU1, QES_RYO, 0, 0);
    EVT_AT_SetFaceLooks(QES_RYO, FacialExpressionId::Suffer, 8, 1, 5);
    EVT_AT_SetMot(QES_RYO, 7899, 25, 1, 0x24000000U, 0);
    EVT_AT_SetMot(QES_KU1, 7907, 25, 1, 0x24000000U, 0);
    EVT_AT_SetVoice("0291A011", 1, 15);
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
        .delay = 70});
    return 100;
}
std::int32_t Qes1JumpSuccess() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 0}, .degrees = 1.5f, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetMot(QES_RYO, 7900, 10, 1, 0x40000000U, 0);
    EVT_AT_DispBlur(QES_RYO, 1, 7, 1, 40, 0);
    EVT_AT_SetCam(1016, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0291A013", 1, 0);
    return 45;
}
std::int32_t Qes1JumpFailure() {
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_BASE, .position = {0, 0, 0}, .degrees = 1.5f, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetPos(QES_RYO, QES_RYO, {0.5f, 0, 0}, 45);
    EVT_AT_SetPos(QES_REN, QES_BASE, QES1_JUMP_FAILURE_REN, 0);
    EVT_AT_SetPos(QES_REN, QES_RYO, {-0.56f, 0, -0.95999998f}, 45);
    EVT_AT_SetPos(QES_DUMMY, QES_FLOORBOARD, QES1_FLOORBOARD_DUMMY, 0);
    EVT_AT_SetMot(QES_RYO, 7888, 0, 1, 0, 0);
    EVT_AT_SetMot(QES_RYO, 7880, 60, 1, 0x44000000U, 45);
    EVT_AT_SetMot(QES_REN, 7877, 50, 1, 0, 0);
    EVT_AT_SetMot(QES_REN, 7876, 60, 1, 0x44000000U, 45);
    Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 206, 10, nullptr, 2, 10);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), QES_TASK);
    HLib::SetupTimedTask(task, 20);
    EventSmokeWork *const smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    smoke->character = QES_DUMMY;
    smoke->bone = 0xffffffffU;
    smoke->startDelay = 2;
    smoke->remaining = 0;
    smoke->finishDelay = 108;
    smoke->profile = 0;
    smoke->state = 0;
    smoke->birthFraction = 0.1f;
    smoke->birthStep = 0.1f;
    EVT_AT_SetCam(1015, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(1032, QES_BASE, QES_BASE, 0, 0, 45);
    EVT_AT_SetVoice("0291A014", 1, 0); // @text EN: " " | JA: "うわっ！"
    EVT_AT_SetVoice("0291B009", 1, 45); // @text EN: "Fool!\nGet up here quick!" | JA: "バーカ！　さっさと上がれ！"
    EVT_AT_SetVoice("0291A015", 1, 110); // @text EN: "Damn." | JA: "くそ…"
    return 150;
}
void Qes1HideCharacters() {
    for (std::int32_t index = 1; QES1_READ_CHARACTERS[index].name != QES_INVALID.name; ++index) {
        EV_DisableCharacterDisplay(QES1_READ_CHARACTERS[index]);
        EV_ToggleCharacterFlag(QES1_READ_CHARACTERS[index]);
    }
    EV_DisableCharacterDisplay(QES_CHAINSAW);
    EV_ToggleCharacterFlag(QES_CHAINSAW);
}
std::int32_t Qes1Initialize(Qes1EventWork *work) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QES1"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QES1"), 0);
    eventSavedSoundScene = GetSelectedSoundSceneIndex();
    std::strcpy(eventSavedSoundBankName.data(), shenGetCurrentSbt());
    std::strcpy(eventSavedBgmName.data(), GetCurrentBgm());
    EV_LoadCutsceneFromCurrentScene("0291A000");
    EV_LoadAudio("0291A000", 0);
    EV_spmBeginSampleExecute();
    qesSoundHandle = AllocateMaterialSoundHandle(1951);
    qesSoundIntensity = 0;
    ExecuteMaterialSoundHandleCommand(qesSoundHandle, 0xa6, 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0291");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES_DUMMY);
    if (g_currentMap.name == QES1_AREA.name)
        qesFloorHeight = 100;
    else {
        Vector3 position;
        EV_GetCharaPosition(QES_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        qesFloorHeight = position.y;
    }
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    scenarioQteRetryCount = 0;
    SetAreaNpcLowQualityHands(false);
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_RYO), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 0);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_RYO), FacialExpressionId::Angry, 0, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_REN), FacialExpressionId::Angry, 0, 1);
    for (std::int32_t index = 0; index < 20; ++index)
        eventMovementStates[index] = 0;
    work->phase = 1;
    GetCurrentDateTime(eventTemporaryDateTime);
    work->retryCanSkip = 0;
    return 0;
}
std::int32_t Qes1ResetActors(Qes1EventWork *work) {
    const shen_name *lists[]{QES1_KICK_CHARACTERS, QES1_PAIR_CHARACTERS, QES1_PURSUERS};
    for (const shen_name *list : lists)
        for (std::int32_t index = 0; list[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(list[index])) {
                EV_npcLockUnlock(list[index]);
                EV_npcFree(list[index]);
            }
        }
    SetCurrentDateTimeTo(eventTemporaryDateTime);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QES1AUTH"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work->phase = 2;
    return 0;
}
std::int32_t Qes1Intro(Qes1EventWork *work) {
    switch (work->subphase) {
    case 0: {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        for (const shen_name character : {QES_REN, QES_YUA, QES_CHAINSAW}) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
            HasCharacterMotionWork(ctrlGetCharaByName(character));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
            if (character.name != QES_CHAINSAW.name)
                mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EventCling cling{.cleanup = 0, .target = QES_CHAINSAW, .mode = 1, .delay = 0};
        EVT_AT_Cling(&cling);
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 0;
            intensity->frames = 0;
            intensity->step = -static_cast<float>(intensity->initial);
            intensity->phase = 0;
        }
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->retryCanSkip > 0)
            EV_SetAuthorActiveTo(1);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(QES_START_AUTHOR.name), nullptr, nullptr, 0);
        ++work->subphase;
        break;
    }
    case 1: {
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES1AUTH"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES1AUTH"), 0);
        Queue_EVT_UTL_LoadNPCChara_3(QES1_KICK_CHARACTERS, nullptr);
        EV_DisableCharacterDisplay(QES_YUA);
        EV_DisableCharacterDisplay(QES_CHAINSAW);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_RYO), FacialExpressionId::Angry, 0, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_REN), FacialExpressionId::Angry, 0, 1);
        EventCling cling{.cleanup = 0,
            .target = QES_CHAINSAW,
            .character = QES_YUA,
            .bone = 14,
            .position = QES1_CHAINSAW_ATTACH_POSITION,
            .rotation = {3731, 56251, 35862},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&cling);
        for (std::int32_t index = 0; index < 20; ++index)
            CancelEventMovement(index);
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {249.39999f, qesFloorHeight, 156.3f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_BASE, QES_BOX, {0, 0, 0}, 10);
        EVT_AT_SetAngY(QES_BASE, QES_BOX, 0, 10);
        EVT_AT_SetPos(QES_BOX, QES_INVALID, {244.7f, qesFloorHeight, 146.5f}, 0);
        EVT_AT_SetAngY(QES_BOX, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_INITIAL_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES1_INITIAL_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_CEILING, QES_INVALID, {247.6f, qesFloorHeight, 133.89999f}, 0);
        EVT_AT_SetAngY(QES_CEILING, QES_INVALID, 270, 0);
        Queue_EVT_AT_PlayDynamix(QES_CEILING, "CEIL", 2, 3, 45, QES1_CEILING_PARTS, 1, 0);
        EVT_AT_SetMot(QES_RYO, 7889, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 7879, 0, 1, 0x40000000U, 0);
        QueueQesAngle(QES_RYO, 3, 20, 52);
        QueueQesAngle(QES_REN, 3, 30, 35);
        QueueQesAngle(QES_REN, -3, 2, 65);
        Loop3DSoundEffect(0xb9d, 270);
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 10, .target = 50, .frames = 60, .phase = 0, .step = 2.0f / 3.0f};
        }
        QueueSkQteWait(&work->qteElapsed, 64, 24, 16, Qes1RubbleSuccess, Qes1RubbleFailure, 45);
        qesMissedInputs = 0;
        ++work->subphase;
        break;
    }
    case 2:
        EVT_AT_SetCam(-1, {}, {}, 0, 0xfb, 0);
        if (!work->qteSucceeded)
            ++qesMissedInputs;
        work->phase = 3;
        work->subphase = 0;
        break;
    default:
        break;
    }
    return 0;
}
std::int32_t Qes1FirstKick(Qes1EventWork *work) {
    if (!work->subphase) {
        if (!g_usingCustomAuthorCharacterList)
            return 0;
        for (const shen_name character : {QES_REN, QES_KU1, QES_KK6}) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
            HasCharacterMotionWork(ctrlGetCharaByName(character));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        PrepareQesCharacter(QES_KU1);
        PrepareQesCharacter(QES_KK6);
        EVT_AT_SetSound(652, 25);
        EVT_AT_SetSound(642, 40);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {221.5f, qesFloorHeight, 143.89999f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_KICK_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 75, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES1_KICK_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 80, 0);
        EVT_AT_SetPos(QES_KU1, QES_BASE, QES1_KICK_KU1, 0);
        EVT_AT_SetAngY(QES_KU1, QES_BASE, 265, 0);
        EVT_AT_SetPos(QES_KK6, QES_BASE, QES1_KICK_KK6, 0);
        EVT_AT_SetAngY(QES_KK6, QES_BASE, 255, 0);
        EVT_AT_SetAngY(QES_KK6, QES_REN, 0, 20);
        EVT_AT_SetMot(QES_RYO, 7889, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 7889, 5, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 7881, 0, 1, 0, 20);
        EVT_AT_SetMot(QES_REN, 7889, 5, 1, 0, 40);
        EVT_AT_SetMot(QES_KU1, 7889, 15, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KK6, 7889, 10, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KK6, 7874, 0, 1, 0, 20);
        EVT_AT_SetCam(1017, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(&work->qteElapsed, 256, 24, 16, Qes1KickSuccess, Qes1KickFailure, 0);
        for (const shen_name character : {QES_KU1, QES_KK6}) {
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 0, 0, 16);
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 1, 0, 16);
        }
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 100;
            intensity->frames = 50;
            intensity->phase = 0;
            intensity->step = static_cast<float>(100 - intensity->initial) / 50;
        }
        ++work->subphase;
    } else if (work->subphase == 1) {
        work->subphase = 0;
        if (work->qteSucceeded)
            work->phase = 5;
        else {
            ++qesMissedInputs;
            work->phase = 4;
        }
    }
    return 0;
}
std::int32_t Qes1CounterKick(Qes1EventWork *work) {
    if (!work->subphase) {
        EV_EnsureCharacterDraw(QES_KU1);
        EV_EnableCharacterDisplay(QES_KU1);
        HasCharacterMotionWork(ctrlGetCharaByName(QES_KU1));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_KU1), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_KU1), 3);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_SK_WaitQTE_Callback, 6, 0x0b, sizeof(StepWaitQteWork), QES_TASK);
        StepWaitQteWork *const qte = static_cast<StepWaitQteWork *>(HLib::GetTaskParameter(task));
        EV_SetAuthorActiveTo(0);
        work->qteElapsed = 0;
        work->qteSucceeded = 0;
        qte->success = Qes1CounterSuccess;
        qte->failure = Qes1CounterFailure;
        qte->task = task;
        qte->parent = HLib::GetCurrentTask();
        qte->status = &work->qteStatus;
        qte->button = 512;
        qte->referenceFrames = 24;
        const std::int32_t maximum = 4 * scenarioQteRetryCount + 16;
        qte->maximumFrames = maximum < 20 ? maximum : 20;
        qte->phase = 0;
        qte->elapsed = &work->qteElapsed;
        qte->succeeded = &work->qteSucceeded;
        HLib::SetupTimedTask(qte->parent, 0);
        ++work->subphase;
    } else {
        work->subphase = 0;
        if (work->qteSucceeded)
            work->phase = 5;
        else {
            if (scenarioQteRetryCount < 2)
                ++scenarioQteRetryCount;
            work->phase = 15;
        }
    }
    return 0;
}
std::int32_t Qes1ChooseRoute(Qes1EventWork *work) {
    if (!work->subphase) {
        for (std::int32_t index = 0; QES1_KICK_CHARACTERS[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(QES1_KICK_CHARACTERS[index])) {
                EV_npcLockUnlock(QES1_KICK_CHARACTERS[index]);
                EV_npcFree(QES1_KICK_CHARACTERS[index]);
            }
        }
        EV_EnsureCharacterDraw(QES_REN);
        EV_EnableCharacterDisplay(QES_REN);
        HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {230.8f, qesFloorHeight, 144.2f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_ROUTE_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES1_ROUTE_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 80, 0);
        EVT_AT_SetMot(QES_RYO, 7889, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 7889, 0, 1, 0x40000000U, 0);
        QueueQesAngle(QES_REN, -3, 30, 30);
        EVT_AT_SetCam(1000, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(&work->qteElapsed, 0xa0000000U, 40, 40, nullptr, nullptr, 4);
        if (qesMissedInputs == 2) {
            EVT_AT_SetPos(QES_REN, QES_BASE, QES1_OFFSCREEN_REN, 0);
            EVT_AT_SetAngY(QES_REN, QES_BASE, 80, 0);
            EV_DisableCharacterDisplay(QES_REN);
        } else if (!qesMissedInputs) {
            EVT_AT_SetPoint(QES_REN, QES_RYO, 5, 3, 1, 20, 60, 0);
            EVT_AT_SetVoice("0291B005", 1, 0); // @text EN: "Right!" | JA: "右だ！"
        }
        ++work->subphase;
    } else {
        if (work->qteSucceeded) {
            Queue_EVT_UTL_LoadNPCChara_3(QES1_PAIR_CHARACTERS, nullptr);
            work->phase = 7;
        } else {
            Queue_EVT_UTL_LoadNPCChara_3(QES1_PURSUERS, nullptr);
            work->phase = 6;
        }
        work->subphase = 0;
    }
    return 0;
}
std::int32_t Qes1DeadEnd(Qes1EventWork *work) {
    switch (work->subphase) {
    case 0:
        if (qesMissedInputs != 2) {
            EV_EnsureCharacterDraw(QES_REN);
            EV_EnableCharacterDisplay(QES_REN);
            HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
            mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        }
        EVT_AT_SetCam(-1, {}, {}, 0, 0xfb, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {199.2f, qesFloorHeight, 148.6f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        QueueQesAngle(QES_RYO, 3, 10, 14);
        QueueQesAngle(QES_RYO, -3, 10, 35);
        ++work->subphase;
        return 60;
    case 1: {
        if (!g_usingCustomAuthorCharacterList)
            return 0;
        EVT_AT_SetVoice("0291A005", 1, 0); // @text EN: "Oh, man!" | JA: "うっ…しまった！"
        EV_EnsureCharacterDraw(QES_SOY);
        EV_EnableCharacterDisplay(QES_SOY);
        HasCharacterMotionWork(ctrlGetCharaByName(QES_SOY));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_SOY), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_SOY), 3);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_DEADEND_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 90, 0);
        EVT_AT_SetPos(QES_SOY, QES_BASE, QES1_DEADEND_SOY, 0);
        EVT_AT_SetAngY(QES_SOY, QES_BASE, 0, 0);
        EVT_AT_SetMot(QES_RYO, 7890, 40, 1, 0x40000000U, 20);
        EVT_AT_SetMot(QES_REN, 0xf002, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_SOY, 7913, 20, 1, 0x60000000U, 0);
        EVT_AT_SetCam(1001, QES_BASE, QES_BASE, 0, 0, 0);
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 70;
            intensity->frames = 30;
            intensity->phase = 0;
            intensity->step = static_cast<float>(70 - intensity->initial) / 30;
        }
        ++work->subphase;
        return 30;
    }
    case 2: {
        EventCling cling{.cleanup = 0,
            .target = QES_CHAINSAW,
            .character = QES_YUA,
            .bone = 14,
            .position = QES1_RETRY_ATTACH_POSITION,
            .rotation = {55579, 25007, 57088},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&cling);
        EVT_AT_SetVoice("0291C003", 1, 50); // @text EN: "Too bad, isn't it?" | JA: "残念だったわねぇ…"
        EVT_AT_SetVoice("0291C004", 1, 110); // @text EN: " " | JA: "キエェェェーッ！"
        for (const shen_name character : {QES_YUA, QES_CHAINSAW, QES_KT3, QES_KT5, QES_CJU, QES_CJU}) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
            HasCharacterMotionWork(ctrlGetCharaByName(character));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
            if (character.name != QES_CHAINSAW.name)
                mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EV_DisableCharacterDisplay(QES_YUA);
        EV_DisableCharacterDisplay(QES_CHAINSAW);
        EVT_AT_ObjectDisp_1(QES_YUA, 50, 0);
        EVT_AT_ObjectDisp_1(QES_CHAINSAW, 50, 0);
        EVT_AT_SetPos(QES_YUA, QES_BASE, QES1_CHAINSAW_APPROACH, 0);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 0);
        for (std::int32_t index = 0; index < 4; ++index) {
            const shen_name character = QES1_PURSUERS[index + 1];
            EVT_AT_SetPos(character, QES_BASE, QES1_PURSUER_POSITIONS[index], 0);
            EVT_AT_SetAngY(character, QES_BASE, index == 2 ? 292.5f : 270, 0);
        }
        EVT_AT_SetMot(QES_YUA, 7931, 0, 1, 0x40000000U, 50);
        EVT_AT_SetMot(QES_KT3, 0xf005, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KT3, 7902, 0, 1, 0, 10);
        EVT_AT_SetMot(QES_KT5, 0xf005, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KT5, 7903, 0, 1, 0, 20);
        EVT_AT_SetMot(QES_CJU, 0xf005, 30, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_CJU, 7901, 30, 1, 0, 20);
        EVT_AT_SetMot(QES_CJU, 0xf005, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_CJU, 7901, 0, 1, 0, 15);
        EVT_AT_SetMot(QES_KT5, 0xf005, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KT5, 7903, 0, 1, 0, 10);
        EVT_AT_SetCam(1002, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetCam(1022, QES_BASE, QES_BASE, 0, 0, 50);
        EVT_AT_SetCam(1026, QES_BASE, QES_BASE, 0, 0, 110);
        if (qesSoundHandle) {
            HLib::Task *task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 40;
            intensity->frames = 50;
            intensity->phase = 0;
            intensity->step = static_cast<float>(40 - intensity->initial) / 50;
            task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 50);
            *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 20, .target = 2, .frames = 30, .phase = 0, .step = -0.6f};
        }
        if (scenarioQteRetryCount < 2)
            ++scenarioQteRetryCount;
        work->phase = 16;
        work->subphase = 0;
        QueueQesRetryFade(160);
        return 250;
    }
    default:
        return 0;
    }
}
std::int32_t Qes1RenRun(Qes1EventWork *work) {
    EV_EnsureCharacterDraw(QES_REN);
    EV_EnableCharacterDisplay(QES_REN);
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
    mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
    EVT_AT_SetCam(-1, {}, {}, 0, 0xfb, 0);
    QueueQesAngle(QES_RYO, -5, 15, 22);
    QueueQesAngle(QES_RYO, -5, 15, 83);
    QueueEventMovement(nullptr, QES_REN, QES1_REN_ROUTE, &QES1_MOVEMENT_MOTIONS, 1, 0xc0400000U);
    EVT_AT_SetVoice("0291B006", 1, 60); // @text EN: "The floor!" | JA: "おっ、床が…！"
    EVT_AT_SetPos(QES_FLOORBOARD, QES_INVALID, {220.7f, qesFloorHeight, 124.4f}, 0);
    EVT_AT_SetAngY(QES_FLOORBOARD, QES_INVALID, 2700, 0);
    Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 3, 10, nullptr, 1, 0);
    work->phase = 8;
    return 100;
}
std::int32_t Qes1FloorboardJump(Qes1EventWork *work) {
    if (!work->subphase) {
        for (std::int32_t index = 0; index < 20; ++index)
            CancelEventMovement(index);
        EV_EnsureCharacterDraw(QES_REN);
        EV_EnableCharacterDisplay(QES_REN);
        HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(QES_REN), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(QES_REN), 3);
        EVT_AT_SetVoice("0291B007", 1, 0);
        EVT_AT_SetCam(1014, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {220.7f, qesFloorHeight, 125.9f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 270, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_FLOORBOARD_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, {0, 0, 0}, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES_FLOORBOARD, QES_BASE, QES1_FLOORBOARD, 0);
        EVT_AT_SetAngY(QES_FLOORBOARD, QES_BASE, 0, 0);
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 7878, 40, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0, 20);
        Queue_EVT_AT_PlayDynamix(QES_FLOORBOARD, "FLBD", 2, 3, 10, nullptr, 1, 0);
        QueueSkQteWait(&work->qteElapsed, 256, 24, 16, Qes1JumpSuccess, Qes1JumpFailure, 0);
        ++work->subphase;
    } else if (work->subphase == 1) {
        work->phase = 9;
        work->subphase = 0;
    }
    return 0;
}
std::int32_t Qes1ThirdPursuer(Qes1EventWork *work) {
    if (!g_usingCustomAuthorCharacterList)
        return 0;
    PrepareQesCharacter(QES_KN3);
    for (const shen_name character : {QES_REN, QES_KN3}) {
        EV_EnsureCharacterDraw(character);
        EV_EnableCharacterDisplay(character);
        HasCharacterMotionWork(ctrlGetCharaByName(character));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    }
    EVT_AT_SetPos(QES_BASE, QES_INVALID, {240.4f, qesFloorHeight, 125.9f}, 0);
    EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_THIRD_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 267, 0);
    EVT_AT_SetPos(QES_REN, QES_BASE, QES1_THIRD_REN, 0);
    EVT_AT_SetAngY(QES_REN, QES_BASE, 270, 0);
    EVT_AT_SetPos(QES_KN3, QES_BASE, QES1_THIRD_KN3, 0);
    EVT_AT_SetAngY(QES_KN3, QES_BASE, 90, 0);
    EVT_AT_SetAngY(QES_REN, QES_REN, 180, 30);
    EVT_AT_SetAngY(QES_KN3, QES_REN, 0, 30);
    EVT_AT_SetSound(649, 40);
    EVT_AT_SetSound(649, 60);
    EVT_AT_SetSound(649, 80);
    EVT_AT_SetSound(642, 110);
    EVT_AT_SetMot(QES_RYO, 7889, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_REN, 7891, 0, 1, 0, 30);
    EVT_AT_SetMot(QES_REN, 7889, 5, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_KN3, 7889, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_KN3, 7892, 0, 1, 0, 30);
    EVT_AT_SetVoice("0291B010", 1, 10); // @text EN: "What a pest=@" | JA: "ジャマくせぇ…"
    EVT_AT_SetCam(1031, QES_BASE, QES_BASE, 0, 0, 0);
    work->phase = 10;
    return 60;
}
std::int32_t Qes1PairedFight(Qes1EventWork *work) {
    if (!work->subphase) {
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 40;
            intensity->frames = 90;
            intensity->phase = 0;
            intensity->step = static_cast<float>(40 - intensity->initial) / 90;
        }
        SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 0);
        InitializeQesPairedSequence(246.89999f, 125.5f, 270);
        qesPairedCharacters[0] = QES_RYO;
        qesPairedCharacters[1] = QES_WTC;
        qesPairedCharacters[2] = QES_THG;
        adfsSetCameraIdxForChara(QES_WTC, QES_CAMERA_WTC);
        adfsSetCameraIdxForChara(QES_THG, QES_CAMERA_THG);
        for (const shen_name character : {QES_RYO, QES_WTC, QES_THG}) {
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 0, 0, 16);
            hmdlSetFlagsFor(ctrlGetCharaByName(character), 1, 0, 16);
        }
        ++work->subphase;
        return 0;
    }
    PrepareQesCharacter(QES_WTC);
    PrepareQesCharacter(QES_THG);
    for (const shen_name character : {QES_WTC, QES_THG}) {
        EV_EnsureCharacterDraw(character);
        EV_EnableCharacterDisplay(character);
        HasCharacterMotionWork(ctrlGetCharaByName(character));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    }
    const std::int32_t result = UpdateQesPairedSequence(&work->qteElapsed);
    if (result == -1)
        return 0;
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 90);
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 0, 2, 16);
    hmdlSetFlagsFor(ctrlGetCharaByName(QES_RYO), 1, 2, 16);
    work->subphase = 0;
    if (result == 1) {
        work->phase = 12;
        return 0;
    }
    work->phase = 15;
    if (scenarioQteRetryCount < 2)
        ++scenarioQteRetryCount;
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    return 30;
}
std::int32_t Qes1RyoApproach(Qes1EventWork *work) {
    EVT_AT_SetCam(-1, {}, {}, 0, 0xfb, 0);
    QueueEventMovement(work->task, QES_RYO, QES1_RYO_PAIR_ROUTE, &QES1_MOVEMENT_MOTIONS, 1, 0xc04c0000U);
    work->phase = 12;
    return 0;
}
std::int32_t Qes1StickOutcome() {
    EVT_AT_SetVoice("0291A002", 1, 0);
    std::int32_t delay;
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 2) {
        EVT_AT_ObjectDisp_1(QES_YUA, 30, 0);
        EVT_AT_ObjectDisp_1(QES_CHAINSAW, 30, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_STICK_SUCCESS_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, {2.7f, 0, 0.3f}, 150);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 150);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES1_STICK_SUCCESS_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 270, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, {2, 0, -0.5f}, 150);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 270, 150);
        EVT_AT_SetPos(QES_YUA, QES_BASE, {-6.9f, 0, 2}, 30);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 0, 30);
        EVT_AT_SetAngY(QES_YUA, QES_YUA, 270, 70);
        EVT_AT_SetMot(QES_RYO, 7886, 0, 1, 0x60000000U, 0);
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0, 60);
        EVT_AT_SetMot(QES_RYO, 7920, 0, 1, 0, 150);
        EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0, 0);
        EVT_AT_SetMot(QES_REN, 7919, 0, 1, 0, 150);
        EVT_AT_SetMot(QES_YUA, 7918, 0, 1, 0x20000000U, 30);
        EVT_AT_SetMot(QES_YUA, 7915, 0, 1, 0, 70);
        EVT_AT_SetSound(1211, 30);
        EVT_AT_SetSound(648, 45);
        EVT_AT_SetSound(648, 100);
        EVT_AT_SetSound(647, 70);
        EVT_AT_SetSound(647, 90);
        EVT_AT_DispBlur(QES_RYO, 7, 12, 1, 45, 10);
        EVT_AT_SetCam(1003, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetCam(1004, QES_BASE, QES_BASE, 0, 0, 60);
        EVT_AT_SetCam(1023, QES_BASE, QES_BASE, 0, 0, 150);
        QueueCameraShake(0.4f, 0.7f, 0.7f, 90);
        Queue_EVT_AT_PlayDynamix(QES_CEILING, "CEIL", 2, 255, 45, QES1_CEILING_PARTS, 2, 30);
        EVT_AT_SetVoice("0291C006", 1, 40); // @text EN: "You rats!\nYou won't get away!" | JA: "このネズミども！\n許さないわよーっ！"
        EVT_AT_SetVoice("0291A003", 1, 150); // @text EN: "Good!" | JA: "よしっ！"
        EVT_AT_SetVoice("0291B002", 1, 170);
        EVT_AT_SetVoice("0291B003", 1, 200); // @text EN: "This way." | JA: "こっちだ。"
        EVT_AT_SetVoice("0291A004", 1, 220); // @text EN: "Yeah." | JA: "ああ。"
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), QES_TASK);
        HLib::SetupTimedTask(task, 90);
        EventSmokeWork *const smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
        smoke->character = QES_BASE;
        smoke->bone = 0xffffffffU;
        smoke->startDelay = 60;
        smoke->remaining = 40;
        smoke->finishDelay = 0;
        smoke->profile = 1;
        smoke->state = 0;
        smoke->birthFraction = 0.05f;
        smoke->birthStep = 0.00125f;
        if (qesSoundHandle) {
            task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 20;
            intensity->frames = 40;
            intensity->phase = 0;
            intensity->step = static_cast<float>(20 - intensity->initial) / 40;
            task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            HLib::SetupTimedTask(task, 150);
            QesSoundIntensityWork *const final = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            final->initial = 80;
            final->target = 80;
            final->phase = 0;
        }
        delay = 220;
    } else {
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 20;
            intensity->frames = 60;
            intensity->phase = 0;
            intensity->step = static_cast<float>(20 - intensity->initial) / 60;
        }
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_STICK_FAILURE_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 0);
        EVT_AT_SetMot(QES_RYO, 7886, 0, 1, 0x60000000U, 0);
        EVT_AT_SetCam(1027, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetSound(653, 20);
        delay = 60;
    }
    snapAssignFrmCntFlags(45, 131);
    return delay;
}
std::int32_t Qes1ChainsawQte(Qes1EventWork *work) {
    switch (work->subphase) {
    case 0:
        for (std::int32_t index = 0; QES1_PAIR_CHARACTERS[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(QES1_PAIR_CHARACTERS[index])) {
                EV_npcLockUnlock(QES1_PAIR_CHARACTERS[index]);
                EV_npcFree(QES1_PAIR_CHARACTERS[index]);
            }
        }
        Queue_EVT_UTL_LoadNPCChara_3(QES1_PURSUERS, nullptr);
        for (const shen_name character : {QES_REN, QES_YUA, QES_CHAINSAW}) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
            HasCharacterMotionWork(ctrlGetCharaByName(character));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
            if (character.name != QES_CHAINSAW.name)
                mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EV_DisableCharacterDisplay(QES_YUA);
        EV_DisableCharacterDisplay(QES_CHAINSAW);
        EVT_AT_SetCam(1028, QES_BASE, QES_BASE, 0, 0, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {247.6f, qesFloorHeight, 150.09999f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 270, 0);
        Queue_EVT_AT_PlayDynamix(QES_CEILING, "CEIL", 2, 3, 45, QES1_CEILING_PARTS, 1, 0);
        EVT_AT_SetPos(QES_CEILING, QES_BASE, {0, 0, 0}, 0);
        EVT_AT_SetAngY(QES_CEILING, QES_BASE, 270, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_CHAINSAW_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 0);
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0x40000000U, 0);
        if (qesSoundHandle) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
            QesSoundIntensityWork *const intensity = static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task));
            intensity->initial = _mm_cvttss_si32(_mm_set_ss(qesSoundIntensity));
            intensity->target = 30;
            intensity->frames = 40;
            intensity->phase = 0;
            intensity->step = static_cast<float>(30 - intensity->initial) / 40;
        }
        ++work->subphase;
        return 30;
    case 1:
        qteBattleUtil(BattleQteMode::SetWait, 0, 6, 4 * scenarioQteRetryCount + 16, 0);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256, 0, 0);
        qteBattleUtil(BattleQteMode::Start);
        ++work->subphase;
        return 0;
    case 2:
        if (qteBattleUtil(BattleQteMode::GetResult)) {
            const std::int32_t delay = qteBattleUtil(BattleQteMode::GetCompletedCount) != 0 ? Qes1StickOutcome() : 0;
            work->phase = qteBattleUtil(BattleQteMode::GetCompletedCount) == 2 ? 17 : 13;
            work->subphase = 0;
            return delay;
        }
        return 0;
    default:
        return 0;
    }
}
std::int32_t Qes1ChaseRetry(Qes1EventWork *work) {
    if (!work->subphase) {
        if (!g_usingCustomAuthorCharacterList)
            return 0;
        EVT_AT_SetCam(-2, {}, {}, 0, 0xfb, 0);
        for (std::int32_t index = 0; QES1_PURSUERS[index].name != QES_INVALID.name; ++index) {
            const shen_name character = QES1_PURSUERS[index];
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
            HasCharacterMotionWork(ctrlGetCharaByName(character));
            SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
        }
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {227.10001f, qesFloorHeight, 125.9f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_RETRY_POSITIONS[0], 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
        for (std::int32_t index = 0; QES1_PURSUERS[index].name != QES_INVALID.name; ++index) {
            EVT_AT_SetPos(QES1_PURSUERS[index], QES_BASE, QES1_RETRY_POSITIONS[index + 1], 0);
            EVT_AT_SetAngY(QES1_PURSUERS[index], QES_BASE, index == 0 ? 70 : 90, 0);
        }
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_SOY, 7902, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KT3, 7903, 60, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_KT5, 7903, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_CJU, 7901, 30, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_CJU, 7901, 0, 1, 0x40000000U, 0);
        QueueQesAngle(QES_RYO, 3, 15, 30);
        ++work->subphase;
        return 45;
    }
    if (work->subphase != 1)
        return 0;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = QES_CHAINSAW,
        .character = QES_YUA,
        .bone = 14,
        .position = QES1_RETRY_ATTACH_POSITION,
        .rotation = {55579, 25007, 57088},
        .mode = 0,
        .delay = 100};
    for (const shen_name character : {QES_YUA, QES_CHAINSAW}) {
        EV_EnsureCharacterDraw(character);
        EV_EnableCharacterDisplay(character);
        HasCharacterMotionWork(ctrlGetCharaByName(character));
        SetFaceFlagsForChara_0(ctrlGetCharaByName(character), FacialNodeReplacement);
        if (character.name != QES_CHAINSAW.name)
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    }
    EV_DisableCharacterDisplay(QES_YUA);
    EV_DisableCharacterDisplay(QES_CHAINSAW);
    EVT_AT_ObjectDisp_1(QES_YUA, 10, 0);
    EVT_AT_ObjectDisp_1(QES_CHAINSAW, 10, 0);
    EVT_AT_SetPos(QES_RYO, QES_BASE, QES1_FINAL_RYO, 0);
    EVT_AT_SetAngY(QES_RYO, QES_BASE, 270, 0);
    EVT_AT_SetPos(QES_YUA, QES_BASE, {20.5f, 0, 7.8f}, 40);
    EVT_AT_SetAngY(QES_YUA, QES_BASE, 180, 40);
    EVT_AT_SetPos(QES_YUA, QES_INVALID, {247.6f, qesFloorHeight, 139.6f}, 100);
    EVT_AT_SetAngY(QES_YUA, QES_YUA, 90, 100);
    EVT_AT_SetMot(QES_RYO, 7890, 40, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QES_YUA, 7923, 0, 1, 0x40000000U, 40);
    EVT_AT_SetMot(QES_YUA, 7918, 0, 1, 0, 70);
    EVT_AT_SetMot(QES_YUA, 7931, 60, 1, 0x40000000U, 100);
    EVT_AT_SetCam(1007, QES_BASE, QES_BASE, 0, 0, 0);
    EVT_AT_SetCam(1030, QES_BASE, QES_BASE, 0, 0, 45);
    EVT_AT_SetCam(1010, QES_BASE, QES_BASE, 0, 0, 100);
    if (qesSoundHandle) {
        task = HLib::EnqueueTaskWithParameter(QesSoundIntensityCallback, 6, 0x0b, sizeof(QesSoundIntensityWork), QES_TASK);
        HLib::SetupTimedTask(task, 70);
        *static_cast<QesSoundIntensityWork *>(HLib::GetTaskParameter(task)) = {.initial = 20, .target = 2, .frames = 20, .phase = 0, .step = -0.9f};
    }
    EVT_AT_SetVoice("0291A005", 1, 0); // @text EN: "Oh, man!" | JA: "うっ…しまった！"
    EVT_AT_SetVoice("0291C007", 1, 70); // @text EN: "Now be ready for this=@" | JA: "さあ…覚悟なさい…"
    EVT_AT_SetVoice("0291C008", 1, 130); // @text EN: " " | JA: "イヤアアァァーッ！"
    EVT_AT_SetVoice("0291A006", 1, 160);
    if (scenarioQteRetryCount < 2)
        ++scenarioQteRetryCount;
    work->phase = 16;
    work->subphase = 0;
    QueueQesRetryFade(155);
    return 245;
}
std::int32_t Qes1RyoExit(Qes1EventWork *work) {
    EVT_AT_SetCam(-1, {}, {}, 0, 0xfb, 0);
    EventSetPositionAngle position{.cleanup = 0, .character = QES_RYO, .parent = QES_INVALID, .position = {247.89999f, 0, 140.3f}, .degrees = 270, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    QueueEventMovement(work->task, QES_RYO, QES1_RYO_EXIT_ROUTE, &QES1_MOVEMENT_MOTIONS, 1, 0xc0480000U);
    work->phase = 17;
    return 0;
}
std::int32_t Qes1DefeatAudio(Qes1EventWork *work) {
    if (GetRandomNumberInRange(100) >= 50) {
        EVT_AT_SetVoice("S2BVC007", 1, 21);
        EVT_AT_SetVoice("S2BVC009", 1, 57);
        EVT_AT_SetVoice("S2BVA012", 1, 105);
        for (const QesDefeatSound &sound : QES1_DEFEAT_SOUNDS_A)
            EVT_AT_SetSound(sound.sound, sound.delay);
        work->phase = 16;
        return 260;
    }
    EVT_AT_SetVoice("S2BVB010", 1, 23);
    EVT_AT_SetVoice("S2BVA010", 1, 92);
    for (const QesDefeatSound &sound : QES1_DEFEAT_SOUNDS_B)
        EVT_AT_SetSound(sound.sound, sound.delay);
    work->phase = 16;
    return 225;
}
void Qes1Cleanup(Qes1EventWork *work) {
    const shen_name *lists[]{QES1_KICK_CHARACTERS, QES1_PAIR_CHARACTERS, QES1_PURSUERS};
    for (const shen_name *list : lists)
        for (std::int32_t index = 0; list[index].name != QES_INVALID.name; ++index) {
            if (DoesScenarioCharacterExist(list[index])) {
                EV_npcLockUnlock(list[index]);
                EV_npcFree(list[index]);
            }
        }
    qteBattleUtil(BattleQteMode::RetireAndFree);
    if (qesSoundHandle) {
        FreeMaterialSoundHandle(qesSoundHandle);
        qesSoundHandle = nullptr;
    }
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 90);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 90);
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    hndlSetFlags(ctrlGetCharaByName(QES_RYO));
    hndlSetFlags(ctrlGetCharaByName(QES_REN));
    ExecuteCleanupFuncForAreaWithCachedScene();
    for (std::int32_t index = 0; index < 20; ++index)
        CancelEventMovement(index);
    EV_FreeAuthorPak(0, "0291");
    EV_FreeAuthorPak(0, "S2BV");
    ET_AT_KillWork(work->characters);
    SetScenarioCharacterKillMode(QES_BASE, 1);
    SetScenarioCharacterKillMode(QES_DUMMY, 1);
    EV_LoadCutsceneFromCurrentScene(eventSavedBgmName.data());
    EV_LoadAudio(eventSavedSoundBankName.data(), eventSavedSoundScene);
    EV_spmBeginSampleExecute();
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES1"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES1"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_NPC_NewLoadSw(1);
    EVT_AT_SetPos(QES_RYO, QES_INVALID, {252, qesFloorHeight, 140.39999f}, 0);
    EVT_AT_SetAngY(QES_RYO, QES_INVALID, 270, 0);
    FinishLoading();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
std::int32_t Qes1Finish(Qes1EventWork *work) {
    if (work->subphase) {
        Qes1Cleanup(work);
        return 0;
    }
    if (g_currentMap.name == QES1_AREA.name) {
        work->phase = 1;
        work->subphase = 0;
        return 0;
    }
    work->subphase = 1;
    spmFadeOut();
    if (EV_GetScreenFilmIsNotShowing())
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    return 30;
}
void Qes1Callback(void *data) {
    Qes1EventWork *const work = static_cast<Qes1EventWork *>(data);
    if (!work->subphase && ((static_cast<std::uint16_t>(work->phase) & 0xffeeU) != 0 || work->phase == 1))
        Qes1HideCharacters();
    std::int32_t delay = 0;
    switch (work->phase) {
    case 0:
        delay = Qes1Initialize(work);
        break;
    case 1:
        delay = Qes1ResetActors(work);
        break;
    case 2:
        delay = Qes1Intro(work);
        break;
    case 3:
        delay = Qes1FirstKick(work);
        break;
    case 4:
        delay = Qes1CounterKick(work);
        break;
    case 5:
        delay = Qes1ChooseRoute(work);
        break;
    case 6:
        delay = Qes1DeadEnd(work);
        if (work->phase != 6) {
            work->retryFrames = delay;
            return;
        }
        break;
    case 7:
        delay = Qes1RenRun(work);
        break;
    case 8:
        delay = Qes1FloorboardJump(work);
        break;
    case 9:
        delay = Qes1ThirdPursuer(work);
        break;
    case 10:
        delay = Qes1PairedFight(work);
        break;
    case 11:
        delay = Qes1RyoApproach(work);
        break;
    case 12:
        delay = Qes1ChainsawQte(work);
        break;
    case 13:
        delay = Qes1ChaseRetry(work);
        if (work->phase != 13) {
            work->retryFrames = delay;
            return;
        }
        break;
    case 14:
        delay = Qes1RyoExit(work);
        break;
    case 15:
        work->retryFrames = Qes1DefeatAudio(work);
        return;
    case 16:
        if (work->retryCanSkip && HasPressedCancel())
            work->retryFrames = 0;
        if (work->retryFrames > 0)
            --work->retryFrames;
        else {
            EV_CleanupMapControlTask();
            qesSoundIntensity = 0;
            work->retryCanSkip = 1;
            work->phase = 1;
        }
        return;
    case 17:
        delay = Qes1Finish(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void Qes1AdmissionNoop() {
}
EventReadCharactersWork *QueueQes1Escape() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes1Callback, 5, 0x0b, sizeof(Qes1EventWork), QES_TASK);
    Qes1EventWork *const work = static_cast<Qes1EventWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->characters = Queue_EVT_AT_ReadChr(task, QES1_READ_CHARACTERS);
    return work->characters;
}
} // namespace
void Qes1Init() {
    qesSoundHandle = nullptr;
}
void Qes1Main(std::uintptr_t) {
    if (g_currentMap.name == QES1_AREA.name) {
        SetMapControlStatusFlag(0, 0);
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x1800, 0);
        EV_lightLoad({});
        PushGlobalLightingProgram();
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    SetPlayerControlEnabled(0);
    QueueQes1Escape();
    Qes1AdmissionNoop();
}
const ScriptAreaDefinition qes1ScriptDefinition{.areaId = QES1_AREA, .field04 = 0, .init = Qes1Init, .main = Qes1Main};
} // namespace shenmue::s2
