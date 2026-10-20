#include "s2/areas/area_ycpa.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t EveningDestination() {
    if (EV_GetShenmueFlag(0x3fd))
        return 0;
    return 2 - (EV_GetShenmueFlag(0x3fe) != 0);
}
void QueueEveningArea(bool rewindMorning) {
    constexpr shen_name areas[]{SHEN_NAME(QA11), SHEN_NAME(Q100), SHEN_NAME(QE01)};
    constexpr std::int32_t times[]{19, 0, 19, 30, 20, 0};
    const std::int32_t destination = EveningDestination();
    DateTime time;
    GetCurrentDateTime(time);
    if (rewindMorning && time.hour < 8)
        --time.day;
    time.hour = static_cast<std::int8_t>(times[destination * 2]);
    time.minute = static_cast<std::int8_t>(times[destination * 2 + 1]);
    SetCurrentDateTimeTo(time);
    EV_scenQueueLoad(g_actNumber, areas[destination], 0);
}
void RestorePlayer(shen_name parent, const Vector3 &position, float yaw) {
    EVT_AT_SetPos(YCPA_RYO, parent, position, 0);
    EVT_AT_SetAngY(YCPA_RYO, parent, yaw, 0);
}
void CommitChaseOutcome() {
    std::int32_t flag = -1;
    switch (ycpaEvent) {
    case 3:
        SetCurrentElevatorFloor(SHEN_NAME(R598), 1);
        SetElevatorIndicatorFlag(SHEN_NAME(R598), 1, 0);
        doorLoopRigidMotion(SHEN_NAME(R598), 0xf0cf, 0, 100);
        flag = 1;
        break;
    case 4:
        flag = 2;
        break;
    case 6:
        flag = 4;
        break;
    case 7:
    case 8:
        flag = 5;
        break;
    case 11:
        SetCurrentElevatorFloor(SHEN_NAME(R594), 4);
        SetElevatorIndicatorFlag(SHEN_NAME(R594), 0, 0);
        doorLoopRigidMotion(SHEN_NAME(R594), 0xf0cf, 0, 100);
        flag = 8;
        break;
    case 13:
        flag = 9;
        break;
    case 14:
        flag = 10;
        break;
    case 100:
        flag = 39;
        break;
    case 101:
        flag = 40;
        break;
    case 102:
        flag = 41;
        break;
    case 103:
        flag = 42;
        break;
    case 104:
        flag = 43;
        break;
    case 105:
        flag = 44;
        break;
    case 106:
        flag = 45;
        break;
    default:
        break;
    }
    if (flag >= 0)
        SetAuthorStateFlag(flag, 1);
}
} // namespace

void YcpaStopChaseCallback(void *) {
    CommitChaseOutcome();
    HLib::SetupTaskDelay(30);
    HLib::SetCurrentTaskTo(YcpaEndChaseCallback);
}

void YcpaEndChaseCallback(void *data) {
    YcpaWork &work = *static_cast<YcpaWork *>(data);
    for (std::int32_t index = 0; index < 20; ++index)
        CancelEventMovement(index);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(YCPA_RYO), FacialNodeOriginal);
    if (DoesScenarioCharacterExist(YCPA_REN))
        hndlSetFlags(FindCharacterByScenarioName(YCPA_REN));
    if (DoesScenarioCharacterExist(YCPA_YUA))
        hndlSetFlags(FindCharacterByScenarioName(YCPA_YUA));
    std::int32_t timeLimit = 5;
    qteBattleUtil(BattleQteMode::RetireAndFree);
    ET_AT_KillWork(work.characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    for (shen_name character : {YCPA_BASE, YCPA_RYOB, YCPA_YUAB, YCPA_RENB, YCPA_RYOD})
        SetCharaKill(character, 1);
    scenFreeEvent("YCPA.EV3");
    EV_FreeAuthorPak(0, "0235");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YCPA"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YCPAMAIN"), 0);
    switch (ycpaSavedEvent) {
    case 3:
    case 4:
    case 11:
        timeLimit = 15;
        break;
    case 6:
    case 100:
        timeLimit = 10;
        break;
    case 7:
    case 8:
    case 105:
        break;
    case 9:
        timeLimit = 11;
        break;
    case 101:
        timeLimit = 17;
        break;
    case 103:
        timeLimit = 12;
        break;
    default:
        timeLimit = 20;
        break;
    }
    npcSetCycleTimeLimitExpiry(SHEN_NAME(cylf), static_cast<std::int16_t>(timeLimit));
    EV_debug_log("YCPA\\eYCPA_et_yuan_pa.c", 407, "=========================\n");
    EV_debug_log("YCPA\\eYCPA_et_yuan_pa.c", 408, "    SetTimeLimit %d\n", timeLimit);
    EV_debug_log("YCPA\\eYCPA_et_yuan_pa.c", 409, "=========================\n");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1900, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2068, 0);
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
        SetFightModeFlags(128);
    }
    if (ycpaSavedEvent == 106)
        EV_stepSetFlags(0, 0x3d0);
    if (ycpaSavedEvent == 13 || ycpaSavedEvent == 14) {
        SetPlayerControlEnabled(0);
        EV_stepSetFlags(0, 0x2a8);
        QueueEveningArea(true);
        SetSoundListenerUsesCamera(0);
        return;
    }
    if (ycpaSavedEvent == 4 && work.finished) {
        SetPlayerControlEnabled(0);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(Q100), 8);
        SetSoundListenerUsesCamera(0);
        return;
    }
    bool resumeSamples = false;
    if (ycpaSavedEvent == 9 && work.finished) {
        RestorePlayer(YCPA_INVALID, Vector3{.x = 293, .y = 100, .z = 76.59999847412109375f}, 180);
        LoadCutsceneAndSoundProgramForArea();
        EV_LoadAudio("QAE1_000.sbt", 0);
        resumeSamples = true;
    } else {
        switch (ycpaSavedEvent) {
        case 3:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 269.100006103515625f, .y = 121.59999847412109375f, .z = 203.5f}, 270);
            break;
        case 4:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 177.899993896484375f, .y = 100, .z = 135.8000030517578125f}, 67.5f);
            break;
        case 6:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 194.6999969482421875f, .y = 100, .z = 99.40000152587890625f}, 320);
            break;
        case 7:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 278.29998779296875f, .y = 100, .z = 90.5f}, 247.5f);
            break;
        case 8:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 282.600006103515625f, .y = 100, .z = 107.8000030517578125f}, 0);
            break;
        case 9:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 291.5f, .y = 100, .z = 76.1999969482421875f}, 0);
            break;
        case 11:
            RestorePlayer(YCPA_INVALID, Vector3{.x = 296.100006103515625f, .y = 100, .z = 102}, 180);
            break;
        default:
            break;
        }
        ExecuteCleanupFuncForAreaWithCachedScene();
        if (ycpaEvent >= 100)
            EVT_AT_SetAngY(YCPA_RYO, YCPA_RYO, 180, 0);
        else
            resumeSamples = true;
    }
    if (resumeSamples)
        EV_spmBeginSampleExecute();
    EV_NPC_NewLoadSw(1);
    if (g_currentMap.name != SHEN_NAME(Q100) && !EV_GetScreenFilmIsNotShowing())
        SetScreenFilmFade(60, 255, 0, 0, 0, 0, 0, 0, 0);
    SetSoundListenerUsesCamera(0);
}

void YcpaEndAdviceCallback(void *) {
    using enum CameraState;
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    for (shen_name character : {YCPA_BASE, YCPA_RYOD, YCPA_YUAB, YCPA_RENB})
        SetCharaKill(character, 1);
    npcSetAreaName(SHEN_NAME(aera), nullptr, 0, 0);
    EV_npcFree(YCPA_REN);
    EV_npcLockUnlock(YCPA_REN);
    HLib::MarkCurrentTaskForCleanup();
    scenFreeEvent("YCPA.EV3");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YCPA"), 0);
    EV_FreeAuthorPak(0, "0235");
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
}

void YcpaChangeAreaAfterAdvice(void *) {
    using enum CameraState;
    camCtrlSetState(Init);
    spmFadeOut();
    npcSetAreaName(SHEN_NAME(aera), nullptr, 0, 0);
    for (shen_name character : {YCPA_BASE, YCPA_RYOD, YCPA_YUAB, YCPA_RENB})
        SetCharaKill(character, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1900, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2068, 0);
    EV_npcFree(YCPA_REN);
    EV_npcLockUnlock(YCPA_REN);
    HLib::MarkCurrentTaskForCleanup();
    scenFreeEvent("YCPA.EV3");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YCPA"), 0);
    EV_FreeAuthorPak(0, "0235");
    EV_NPC_NewLoadSw(1);
    EV_stepSetFlags(0, 0x2a8);
    QueueEveningArea(false);
}
} // namespace shenmue::s2::areas
