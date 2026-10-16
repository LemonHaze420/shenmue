#include "s2/areas/area_qes3.hpp"
#include "s2/areas/qes3_event.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/elevator.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
void Qes3Callback(void *);
void Qes3Enqueue(void *);
std::int32_t Qes3Initialize(Qes3EventWork *work) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QES3"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QES3"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QES3_DUMMY);
    eventSavedSoundScene = GetSelectedSoundSceneIndex();
    std::strcpy(eventSavedSoundBankName.data(), shenGetCurrentSbt());
    std::strcpy(eventSavedBgmName.data(), GetCurrentBgm());
    EV_LoadCutsceneFromCurrentScene("0296E000");
    EV_LoadAudio("0296E000", 0);
    EV_spmBeginSampleExecute();
    qes3SoundHandle = AllocateMaterialSoundHandle(1951);
    qes3SoundIntensity = 0;
    ExecuteMaterialSoundHandleCommand(qes3SoundHandle, 0xa6, 0);
    scenarioQteRetryCount = 0;
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0296");
    EV_FreeAuthorPak(1, "0271");
    EV_FreeAuthorPak(1, "0364");
    qteBattleUtil(BattleQteMode::SetWait, 0, 6, 16);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 256);
    qteBattleUtil(BattleQteMode::SetLastCommands, 0, 0, 0);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    SetLowQualityHands(ctrlGetCharaByName(QES_RYO));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    SetLowQualityHands(ctrlGetCharaByName(QES_REN));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    SetLowQualityHands(ctrlGetCharaByName(QES_YUA));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_YUA));
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 0);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 0);
    if (g_currentMap.name == QES3_AREA.name)
        qes3FloorHeight = 100;
    else {
        Vector3 position;
        EV_GetCharaPosition(QES_RYO, 0xffffffffU, &position, EventTransformFlags::World);
        qes3FloorHeight = position.y;
    }
    work->phase = 1;
    SetCurrentElevatorFloor(QES3_ELEVATOR, 17);
    for (std::int32_t index = 0; index < 4; ++index) {
        qes3Sparks.primary[index] = -1;
        qes3Sparks.secondary[index] = -1;
    }
    return 0;
}
std::int32_t Qes3Chase(Qes3EventWork *work) {
    switch (work->subphase) {
    case 0: {
        for (std::int32_t index = 0; QES3_CLEANUP_CHARACTERS[index].name != QES_INVALID.name; ++index) {
            const shen_name character = QES3_CLEANUP_CHARACTERS[index];
            if (DoesScenarioCharacterExist(character)) {
                EV_npcLockUnlock(character);
                EV_npcFree(character);
            }
        }
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES3BTL"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES3BTL"), 0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QES3QTE"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadAudio("0296E000", 0);
        EV_EnableCharacterDisplay(QES_CHAINSAW);
        EV_DisableCharacterDisplay(QES3_POLE);
        EV_DisableCharacterDisplay(QES3_KT8);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {224.5f, qes3FloorHeight, 146.39999389648438f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_YUA, QES_BASE, {24.5f, 0, 3}, 0);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 0, 0);
        EVT_AT_SetPos(QES3_KT8, QES_BASE, QES3_CHASE_KT8, 0);
        EVT_AT_SetAngY(QES3_KT8, QES_BASE, 0, 0);
        EVT_AT_SetAngY(QES_YUA, QES_YUA, 90, 40);
        EVT_AT_SetMot(QES3_KT8, 8449, 0, 0, 0, 0);
        EVT_AT_SetMot(QES_YUA, 8463, 0, 1, 0, 0);
        EVT_AT_SetMot(QES_YUA, 8484, 0, 1, 0, 40);
        EventCling cling{.cleanup = 0,
            .target = QES_CHAINSAW,
            .character = QES_YUA,
            .bone = 14,
            .position = {0.1f, 0, 0.1f},
            .rotation = {24576, 49152, 8192},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&cling);
        EVT_AT_SetVoice("0296B001", 1, 10); // @text EN: "I found you=@" | JA: "見つけたわよぉ…"
        EVT_AT_SetCam(1000, QES_BASE, QES_BASE, 0, 0, 0);
        QueueQes3SoundIntensity(40, 30, 60, 0);
        work->subphase = 4;
        work->nextSubphase = 1;
        work->timer = 60;
        break;
    }
    case 1:
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_RYO), FacialExpressionId::Angry, 10, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QES_REN), FacialExpressionId::Normal, 10, 1);
        EVT_AT_SetFaceLooks(QES_REN, FacialExpressionId::Angry, 10, 1, 10);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES3_CHASE_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 247.5f, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES3_CHASE_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 270, 0);
        EVT_AT_SetMot(QES_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_REN, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetVoice("0296C001", 1, 0); // @text EN: "Doesn't she ever give up?!" | JA: "ちっ…しつけぇ野郎だ！"
        EVT_AT_SetCam(1001, QES_BASE, QES_BASE, 0, 0, 0);
        QueueQes3SoundIntensity(70, 50, 50, 0);
        work->timer = 80;
        work->subphase = 4;
        work->nextSubphase = 2;
        break;
    case 2:
        EVT_AT_SetVoice("0296C002", 1, 0); // @text EN: "Hey, this way!" | JA: "おいっ、こっちだ！"
        EVT_AT_SetPos(QES_YUA, QES_BASE, QES3_CHASE_YUA, 0);
        EVT_AT_SetAngY(QES_YUA, QES_BASE, 90, 0);
        EVT_AT_SetMot(QES_RYO, 8481, 0, 1, 0, 10);
        EVT_AT_SetMot(QES_REN, 8479, 0, 1, 0, 0);
        QueueQesAngle(QES_RYO, -4, 20, 10);
        QueueQesAngle(QES_REN, -5, 20, 0);
        EVT_AT_SetCam(1002, QES_BASE, QES_BASE, 0, 0, 0);
        QueueQes3SoundIntensity(40, 30, 60, 0);
        work->subphase = 4;
        work->nextSubphase = 5;
        work->timer = 60;
        break;
    case 4:
        if (work->timer) {
            if (qes3RetryCanSkip && HasPressedCancel()) {
                EV_CleanupMapControlTask();
                work->phase = 2;
                work->subphase = 0;
                work->timer = 0;
            } else
                --work->timer;
        } else
            work->subphase = work->nextSubphase;
        break;
    case 5:
        work->phase = 2;
        work->subphase = 0;
        break;
    default:
        break;
    }
    return 0;
}
std::int32_t Qes3PoleQte(Qes3EventWork *work) {
    switch (work->subphase) {
    case 0: {
        QueueQes3SoundIntensity(70, 70, 0, 0);
        AT_SetLocalBaseChara(QES3_POLE, QES3_DUMMY);
        EV_EnableCharacterDisplay(QES3_KT8);
        EV_EnableCharacterDisplay(QES3_POLE);
        EventCling cling{.cleanup = 0,
            .target = QES3_POLE,
            .character = QES3_KT8,
            .bone = 14,
            .position = {0.12f, 0.02f, -0.04f},
            .rotation = {45056, 45056, 20480},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&cling);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {220.39999389648438f, qes3FloorHeight, 168.39999389648438f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, QES3_POLE_RYO, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 0);
        EVT_AT_SetPos(QES_REN, QES_BASE, QES3_POLE_REN, 0);
        EVT_AT_SetAngY(QES_REN, QES_BASE, 180, 0);
        EVT_AT_SetPos(QES3_KT8, QES_BASE, QES3_POLE_KT8, 0);
        EVT_AT_SetAngY(QES3_KT8, QES_BASE, 270, 0);
        EVT_AT_SetAngY(QES3_KT8, QES_BASE, 0, 40);
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0, 0);
        EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0, 0);
        EVT_AT_SetMot(QES_REN, 8454, 0, 1, 0, 45);
        EVT_AT_SetMot(QES_REN, 0xf006, 0, 1, 0, 75);
        EVT_AT_SetMot(QES3_KT8, 8461, 25, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES3_KT8, 8449, 0, 1, 0, 40);
        EVT_AT_SetCam(2000, QES_BASE, QES_BASE, 0, 0, 0);
        QueueSkQteWait(&work->qteElapsed, 16, 24, 20, Qes3PoleSuccess, Qes3PoleFailure, 40);
        EVT_AT_SetVoice("S2BVB008", 1, 10);
        QueueQesAngle(QES_REN, -3, 10, 70);
        QueueQesAngle(QES_REN, -3, 20, 120);
        ++work->subphase;
        break;
    }
    case 1:
        work->subphase = work->qteSucceeded ? 10 : 11;
        break;
    case 10:
        work->phase = 3;
        work->subphase = 0;
        break;
    case 11:
        qes3RetryCanSkip = 1;
        work->phase = 7;
        work->subphase = 0;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        return 90;
    default:
        break;
    }
    return 0;
}
std::int32_t Qes3ChainsawQtes(Qes3EventWork *work) {
    switch (work->subphase) {
    case 0: {
        EV_DisableCharacterDisplay(QES_YUA);
        EV_DisableCharacterDisplay(QES_CHAINSAW);
        EVT_AT_ObjectDisp_1(QES_YUA, 50, 0);
        EVT_AT_ObjectDisp_1(QES_CHAINSAW, 50, 0);
        EV_EnableCharacterDisplay(QES3_POLE);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {219.89999389648438f, qes3FloorHeight, 171.3000030517578f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, {1, 0, -4.800000190734863f}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 175, 0);
        EVT_AT_SetPos(QES_RYO, QES_BASE, {-0.3f, 0, -1.4f}, 20);
        EVT_AT_SetAngY(QES_RYO, QES_BASE, 180, 20);
        EVT_AT_SetPos(QES_YUA, QES_RYO, {0, 0, 2}, 50);
        EVT_AT_SetAngY(QES_YUA, QES_RYO, 0, 50);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
        RegisterEventTask(task);
        EventCling *const cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        *cling = {.cleanup = 1,
            .target = QES3_POLE,
            .character = QES_RYO,
            .bone = 14,
            .position = {0.115f, -0.043f, -0.021f},
            .rotation = {31572, 0, 31253},
            .mode = 0,
            .delay = 30};
        EVT_AT_SetMot(QES_RYO, 0xf006, 0, 1, 0, 0);
        EVT_AT_SetMot(QES_RYO, 8458, 0, 0.9f, 0x40000000U, 20);
        EVT_AT_SetMot(QES_YUA, 8470, 0, 0.9f, 0x40000000U, 50);
        QueueSkQteWait(&work->qteElapsed, 1, 24, 20, Qes3FirstChainsawSuccess, Qes3FirstChainsawFailure, 30);
        EVT_AT_SetCam(-1, QES_INVALID, QES_INVALID, 0, 0, 0);
        EVT_AT_SetCam(3000, QES_BASE, QES_BASE, 0, 0, 20);
        EVT_AT_SetCam(3008, QES_BASE, QES_BASE, 0, 0, 50);
        work->subphase = 1;
        break;
    }
    case 1:
        work->subphase = work->qteSucceeded ? 4 : 11;
        break;
    case 4:
        EVT_AT_SetMot(QES_RYO, 8459, 0, 0.9f, 0, 15);
        EVT_AT_SetMot(QES_YUA, 8471, 0, 0.9f, 0, 15);
        QueueSkQteWait(&work->qteElapsed, 4, 24, 20, Qes3SecondChainsawSuccess, Qes3SecondChainsawFailure, 0);
        work->subphase = 5;
        break;
    case 5:
        work->subphase = work->qteSucceeded ? 8 : 11;
        break;
    case 8:
        EVT_AT_SetCam(3010, QES_BASE, QES_BASE, 0, 0, 35);
        EVT_AT_SetPos(QES_YUA, QES_RYO, QES3_SECOND_CHAIN_SUCCESS, 30);
        EVT_AT_SetAngY(QES_YUA, QES_RYO, 0, 30);
        EVT_AT_SetMot(QES_RYO, 8460, 0, 0.9f, 0, 15);
        EVT_AT_SetMot(QES_RYO, 8457, 0, 0.9f, 0, 30);
        EVT_AT_SetMot(QES_YUA, 8472, 0, 0.9f, 0, 15);
        EVT_AT_SetMot(QES_YUA, 8466, 0, 0.9f, 0, 30);
        EVT_AT_QTE(0, 16, 4, 42);
        work->subphase = 9;
        return 42;
    case 9: {
        const std::int32_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (result == 1) {
            work->subphase = 10;
            return Qes3ThirdChainsawSuccess();
        }
        if (result) {
            work->subphase = 11;
            return Qes3ThirdChainsawFailure();
        }
        break;
    }
    case 10: {
        EventCling cling{.cleanup = 0, .target = QES_CHAINSAW, .mode = 1};
        EVT_AT_Cling(&cling);
        cling.target = QES3_POLE;
        EVT_AT_Cling(&cling);
        AT_SetLocalBaseChara(QES3_POLE, QES_INVALID);
        EV_LoadAudio("0296E000", 1);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        QueueSkAuthorStart(work->task, QES3_QTE_AUTHOR.name, Qes3AuthorAudioCallback, nullptr, 0);
        work->phase = 4;
        work->subphase = 0;
        break;
    }
    case 11: {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
        RegisterEventTask(task);
        EventCling *const cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        cling->cleanup = 1;
        cling->target = QES3_POLE;
        cling->character = {};
        cling->mode = 1;
        cling->delay = 30;
        EVT_AT_ObjectDisp_1(QES_RYO, 30, 0);
        QueueQesRetryFade(0);
        qes3RetryCanSkip = 1;
        work->phase = 7;
        work->subphase = 0;
        return 90;
    }
    default:
        break;
    }
    return 0;
}
std::int32_t Qes3CommandOutcome(Qes3EventWork *work) {
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 4) {
        work->phase = 4;
        return 0;
    }
    const std::int32_t duration = Qes3FirstChainsawFailure();
    if (duration + 30) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QES_TASK);
        RegisterEventTask(task);
        EventCling *const cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        cling->cleanup = 1;
        cling->target = QES3_POLE;
        cling->character = {};
        cling->mode = 1;
        cling->delay = static_cast<float>(duration + 30);
    } else {
        EventCling cling{.cleanup = 0, .target = QES3_POLE, .mode = 1};
        EVT_AT_Cling(&cling);
    }
    QueueQesRetryFade(duration);
    work->phase = 7;
    qes3RetryCanSkip = 1;
    return duration + 90;
}
std::int32_t Qes3Fight(Qes3EventWork *work) {
    switch (work->subphase) {
    case 0:
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES3QTE"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES3QTE"), 0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QES3BTL"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        qes3MotionHandle = ReadMotiFromDirectory(g_actNumber, g_currentMap, "TOG_YUA.MOT");
        EV_LoadAudio("0296E000", 2);
        EV_spmBeginSampleExecute();
        QueueQes3SoundIntensity(0, 0, 0, 0);
        FreeAllQes3Sparks();
        EVT_AT_SetCam(4000, QES_INVALID, QES_INVALID, 0, 0, 0);
        EVT_AT_SetPos(QES_BASE, QES_INVALID, {224.5f, qes3FloorHeight, 146.39999389648438f}, 0);
        EVT_AT_SetAngY(QES_BASE, QES_INVALID, 0, 0);
        Loop3DSoundEffect(2973);
        EVT_AT_SetMot(QES_REN, 32519, 400, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_RYO, 306, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(QES_YUA, 2091, 0, 1, 0x60000000U, 0);
        EVT_AT_SetAng_Enqueue(QES_RYO, QES_INVALID, 0, 26.1749267578125f, 0, 0);
        EVT_AT_SetAng_Enqueue(QES_YUA, QES_INVALID, 0, 206.1749267578125f, 0, 0);
        ++work->subphase;
        break;
    case 1: {
        qteBattleUtil(BattleQteMode::RetireAndFree);
        shenStartFGST(10, 10, 0, 0);
        const std::int32_t hp = 100 * (static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{141}, 0, -1)) + 2);
        shenInitFightData(ctrlGetCharaByName(QES_RYO), nullptr, 0, 0, static_cast<std::uint16_t>(hp), 0);
        shenInitFightData(ctrlGetCharaByName(QES_YUA), nullptr, 1, 1, 400, 10);
        vfCopyPosFromEntity(0, nullptr, 220.8000030517578f, 174.6999969482422f, 3.5f);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(2, 99, 1, 0, -1);
        vfmDispatchControl(FightControlOp::CameraParameter, {}, 13, 1);
        ++work->subphase;
        break;
    }
    case 2: {
        const std::int32_t hp = GetFightCharacterFlag(1, ctrlGetCharaByName(QES_YUA), 0);
        if (IsFightActive() && hp > 20)
            break;
        RestoreQes3FightCharacter(QES_RYO);
        RestoreQes3FightCharacter(QES_YUA);
        EV_EnableCharacterDisplay(QES_REN);
        if (hp > 20) {
            std::int32_t flag = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{77}, 0, -1));
            if (flag > 0)
                --flag;
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{77}, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(flag)), 1);
            flag = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{141}, 0, -1));
            if (flag < 15)
                ++flag;
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{141}, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(flag)), 1);
            qteBattleUtil(BattleQteMode::Load);
            qteBattleUtil(BattleQteMode::Queue);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            Queue_ST_LoadTitleFadeIn(QES3_RETRY_TITLE);
            qes3RetryCanSkip = 1;
            work->phase = 6;
            work->subphase = 0;
            return 60;
        }
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        EV_lightLoad(QES3_FIGHT_LIGHT);
        ++work->subphase;
        return 30;
    }
    case 3:
        if (IsFightActive())
            check_dead_status(1);
        QueueSkAuthorStart(work->task, QES3_FIGHT_AUTHOR.name, Qes3AfterFightAuthorCallback, nullptr, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        ++work->subphase;
        break;
    case 4:
        cuffsSetFlags();
        DeleteMotion(qes3MotionHandle);
        qes3MotionHandle = -1;
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QES3BTL"), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES3BTL"), 0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QES3END"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        Queue_EVT_UTL_LoadNPCChara_3(QES3_CLEANUP_CHARACTERS, work->task);
        ++work->subphase;
        break;
    case 5:
        EV_LoadCutsceneFromCurrentScene("0364A000");
        EV_LoadAudio("0364A000", 0);
        QueueSkAuthorStart(work->task, QES3_END_AUTHOR.name, nullptr, nullptr, 0);
        work->phase = 7;
        work->subphase = 0;
        break;
    default:
        break;
    }
    return 0;
}
std::int32_t Qes3Finish(Qes3EventWork *work) {
    if (g_currentMap.name == QES3_AREA.name) {
        EventFilmControl film{.cleanup = 1,
            .frames = 30,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 60};
        EVT_AT_MenCtrl(film);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        work->phase = 1;
        work->subphase = 0;
        return 60;
    }
    if (EV_GetScreenFilmIsNotShowing())
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    HLib::SetCurrentTaskTo(Qes3Cleanup);
    return 30;
}
void Qes3Callback(void *data) {
    Qes3EventWork *const work = static_cast<Qes3EventWork *>(data);
    std::int32_t delay;
    switch (work->phase) {
    case 0:
        delay = Qes3Initialize(work);
        break;
    case 1:
        delay = Qes3Chase(work);
        break;
    case 2:
        delay = Qes3PoleQte(work);
        break;
    case 3:
        delay = Qes3ChainsawQtes(work);
        break;
    case 4:
        delay = Qes3Fight(work);
        break;
    case 5:
        delay = Qes3CommandOutcome(work);
        break;
    case 6:
        ST_LoadTitleFade();
        work->phase = 7;
        work->subphase = 0;
        HLib::SetupTaskDelay(30);
        return;
    case 7:
        delay = Qes3Finish(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void Qes3Enqueue(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qes3Callback, 5, 0x0b, sizeof(Qes3EventWork), QES_TASK);
    Qes3EventWork *const work = static_cast<Qes3EventWork *>(HLib::GetTaskParameter(task));
    std::memset(reinterpret_cast<std::byte *>(work) + 8, 0, 0x28);
    work->task = task;
    work->characters = Queue_EVT_AT_ReadChr(task, QES3_READ_CHARACTERS);
    work->phase = 0;
    work->timer = 0;
}
void Qes3AdmissionNoop() {
}
} // namespace
void Qes3Cleanup(void *data) {
    Qes3EventWork *const work = static_cast<Qes3EventWork *>(data);
    if (qes3MotionHandle != -1)
        DeleteMotion(qes3MotionHandle);
    SetCurrentElevatorFloor(QES3_ELEVATOR, 8);
    for (std::int32_t index = 0; QES3_CLEANUP_CHARACTERS[index].name != QES_INVALID.name; ++index) {
        const shen_name character = QES3_CLEANUP_CHARACTERS[index];
        if (DoesScenarioCharacterExist(character)) {
            EV_npcLockUnlock(character);
            EV_npcFree(character);
        }
    }
    if (qes3SoundHandle) {
        FreeMaterialSoundHandle(qes3SoundHandle);
        qes3SoundHandle = nullptr;
    }
    FreeAllQes3Sparks();
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_RYO), 90);
    SetFacialBlinkDelay(ctrlGetCharaByName(QES_REN), 90);
    HasCharacterMotionWork(ctrlGetCharaByName(QES_RYO));
    HasCharacterMotionWork(ctrlGetCharaByName(QES_REN));
    hndlSetFlags(ctrlGetCharaByName(QES_RYO));
    hndlSetFlags(ctrlGetCharaByName(QES_REN));
    EventCling cling{.cleanup = 0, .target = QES_CHAINSAW, .mode = 1};
    EVT_AT_Cling(&cling);
    cling.target = QES3_POLE;
    EVT_AT_Cling(&cling);
    AT_SetLocalBaseChara(QES3_POLE, QES_INVALID);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScenarioCharacterKillMode(QES_BASE, 1);
    SetScenarioCharacterKillMode(QES3_DUMMY, 1);
    DeactivatePartyCompanion(QES_REN);
    ET_AT_KillWork(work->characters);
    for (const char *package : {"QES3QTE", "QES3BTL", "QES3END"})
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QES3"), 0);
    scenFreeEvent("QES3.EV3");
    EV_FreeAuthorPak(0, "0296");
    EV_FreeAuthorPak(0, "0271");
    EV_FreeAuthorPak(0, "0364");
    EV_FreeAuthorPak(0, "S2BV");
    EVT_AT_SetMot(QES_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    FinishLoading();
    if (qes3RetryCanSkip) {
        EV_LoadCutsceneFromCurrentScene(eventSavedBgmName.data());
        EV_LoadAudio(eventSavedSoundBankName.data(), eventSavedSoundScene);
        EV_spmBeginSampleExecute();
        EVT_AT_SetPos(QES_RYO, QES_INVALID, {254, qes3FloorHeight, 140.8000030517578f}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_INVALID, 90, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_NPC_NewLoadSw(1);
    } else {
        EV_NPC_NewLoadSw(0);
        EVT_AT_SetPos(QES_RYO, QES_INVALID, {223.1999969482422f, qes3FloorHeight, 179.39999389648438f}, 0);
        EVT_AT_SetAngY(QES_RYO, QES_INVALID, 0, 0);
        SetPlayerControlEnabled(0);
        EV_stepSetFlags(0, 299);
        SetQronFloor(18);
        RepositionPlayerForQronFloor(18);
    }
}
void Qes3Init() {
    qes3MotionHandle = 0;
    qes3FloorHeight = 0;
    qes3RetryCanSkip = 0;
    qes3Sparks = {};
    qes3SoundIntensity = 0;
    qes3SoundHandle = nullptr;
}
void Qes3Main(std::uintptr_t) {
    Qes3AdmissionNoop();
    if (g_currentMap.name == QES3_AREA.name) {
        SetMapControlStatusFlag(0, 0);
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x1800, 0);
        EV_lightLoad({});
        PushGlobalLightingProgram();
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), QES_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = Qes3Enqueue;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
const ScriptAreaDefinition qes3Definition{.areaId = QES3_AREA, .field04 = 0, .init = Qes3Init, .main = Qes3Main};
} // namespace shenmue::s2
