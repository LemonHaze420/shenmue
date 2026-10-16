#include "s2/areas/area_9kan.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>

namespace shenmue::s2::areas {
HLib::Task *nineKanOpenDoorTask, *nineKanBurstDoorTask, *nineKanEntryDoorTask, *nineKanCloseDoorTask;
HLib::Task *nineKanAuxiliaryTask, *nineKanOpenBoxTask, *nineKanCloseBoxTask;
std::int32_t nineKanSoundCounter;
namespace {
void NineKanAuthoredControlNullsub() {
}
void RestoreClosedDoors() {
    EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 0, 0);
    EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 180, 0);
}
bool AuthoredFinished(NineKanWork &work, std::int32_t retry) {
    if (authGetFinished() || (retry > 0 && HasPressedCancel())) {
        work.countdown = 0;
        EV_CleanupMapControlTask();
        NineKanCleanupTasks();
    }
    if (work.countdown) {
        --work.countdown;
        return false;
    }
    return true;
}
} // namespace
void Area9KANInit() {
    nineKanSoundCounter = 0;
}
void Area9KANMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("9KAN"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("9KAN"), 0);
    EV_FreeAuthorPak(1, "0237");
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NineKanLoop, 4, 0x0b, sizeof(NineKanWork), NINE_KAN_TASK);
    NineKanWork *const work = static_cast<NineKanWork *>(HLib::GetTaskParameter(task));
    nineKanOpenDoorTask = nullptr;
    nineKanBurstDoorTask = nullptr;
    nineKanEntryDoorTask = nullptr;
    nineKanCloseDoorTask = nullptr;
    std::memset(reinterpret_cast<std::byte *>(work) + 8, 0, sizeof(NineKanWork) - 8);
    work->task = task;
    nineKanAuxiliaryTask = nullptr;
    nineKanOpenBoxTask = nullptr;
    nineKanCloseBoxTask = nullptr;
    const std::uint32_t entry = static_cast<std::uint32_t>(argument);
    if (entry == 0) {
        Queue_EVT_UTL_LoadNPCChara_3(NINE_KAN_FIRST_CHARACTERS, task);
        work->state = 1;
    } else if (entry == 1) {
        Queue_EVT_UTL_LoadNPCChara_3(NINE_KAN_REPEAT_CHARACTERS, task);
        work->state = 4;
    }
    EV_AT_SetPos(NINE_KAN_DUMMY, EventTransformFlags::World | EventTransformFlags::AllAxes, NINE_KAN_ORIGIN);
    EV_AT_SetAngY(NINE_KAN_DUMMY, EventTransformFlags::World | EventTransformFlags::AllAxes, NINE_KAN_ROTATION);
}
void NineKanSetAuthoredControl(std::int32_t enabled) {
    if (enabled)
        NineKanAuthoredControlNullsub();
    else
        FinishLoading();
    SetRenderClockAndCompassTo(enabled ? 1 : 0);
    const NpcCharacterControllerState state = enabled ? NpcCharacterControllerState::Disabled : NpcCharacterControllerState::Enabled;
    EV_SetCharaControllerFlags(NINE_KAN_RYO, ApplyGravity, state);
    EV_SetCharaControllerFlags(NINE_KAN_RYO, UpdateHeight | ApplyGravity, state);
    EV_SetRefBoardParamsFor(NINE_KAN_RYO, enabled ? 1 : 0);
    EV_SetCharacterAlphaEnabled(NINE_KAN_RYO, !enabled);
    for (const shen_name *character = NINE_KAN_FIRST_CHARACTERS; character->name != NINE_KAN_INVALID.name; ++character) {
        EV_SetCharaControllerFlags(*character, ApplyGravity, state);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, state);
        EV_SetCharacterAlphaEnabled(*character, !enabled);
    }
}
bool NineKanAtIntroductionPoint() {
    Vector3 point{.x = 2.9f, .y = 0, .z = 7.07f}, player;
    EV_GetCharaPosition(NINE_KAN_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&NINE_KAN_ORIGIN, &NINE_KAN_ROTATION, &point);
    const float x = point.x - player.x, z = point.z - player.z;
    return x * x + z * z < 6.25f;
}
std::int32_t NineKanChooseHidingPlace() {
    Vector3 player;
    EV_GetCharaPosition(NINE_KAN_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    for (const NineKanHidingPlace *place = NINE_KAN_HIDING_PLACES; place->state != 17; ++place) {
        Vector3 point = place->position;
        ApplyScenarioTranslationAndRotation(&NINE_KAN_ORIGIN, &NINE_KAN_ROTATION, &point);
        const float x = point.x - player.x, z = point.z - player.z;
        if (place->radius * place->radius > x * x + z * z)
            return place->state;
    }
    return 17;
}
void NineKanPlayTinyMotion(void *data) {
    const NineKanTinyMotion &work = *static_cast<NineKanTinyMotion *>(data);
    ConfigureScenarioNodeMotion(work.character, work.motion, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(work.character, 0, -1, -1, -1, 32767, 4);
    HLib::DestroyCurrentTask();
}
void NineKanOpenDoor(void *data) {
    std::int32_t &frames = *static_cast<std::int32_t *>(data);
    if (frames-- <= 1) {
        nineKanOpenDoorTask = nullptr;
        HLib::DestroyCurrentTask();
    } else {
        EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_LEFT_DOOR, -5, 0);
        EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_RIGHT_DOOR, 5, 0);
    }
}
void NineKanQueueOpenDoor() {
    nineKanOpenDoorTask = HLib::EnqueueTaskWithParameter(NineKanOpenDoor, 4, 0x0b, 4, NINE_KAN_TASK);
    EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 0, 661);
    EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 180, 661);
    HLib::SetupTimedTask(nineKanOpenDoorTask, 662);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(nineKanOpenDoorTask)) = 19;
}
void NineKanCloseDoor(void *data) {
    std::int32_t &frames = *static_cast<std::int32_t *>(data);
    if (frames-- <= 1) {
        EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 0, 0);
        EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 180, 0);
        nineKanCloseDoorTask = nullptr;
        HLib::DestroyCurrentTask();
    } else {
        EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_LEFT_DOOR, 3, 0);
        EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_RIGHT_DOOR, -3, 0);
    }
}
void NineKanQueueCloseDoor() {
    nineKanCloseDoorTask = HLib::EnqueueTaskWithParameter(NineKanCloseDoor, 4, 0x0b, 4, NINE_KAN_TASK);
    EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, 315, 75);
    EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, 215, 75);
    HLib::SetupTimedTask(nineKanCloseDoorTask, 76);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(nineKanCloseDoorTask)) = 15;
}
void NineKanBurstDoor(void *data) {
    std::int32_t &frame = *static_cast<std::int32_t *>(data);
    if (frame >= 21) {
        EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_DUMMY, NINE_KAN_LEFT_FINAL, 0);
        EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_DUMMY, NINE_KAN_RIGHT_FINAL, 0);
        nineKanBurstDoorTask = nullptr;
        HLib::DestroyCurrentTask();
    } else {
        EVT_AT_SetAngY(NINE_KAN_LEFT_DOOR, NINE_KAN_LEFT_DOOR, -NINE_KAN_DOOR_STEPS[frame], 0);
        EVT_AT_SetAngY(NINE_KAN_RIGHT_DOOR, NINE_KAN_RIGHT_DOOR, NINE_KAN_DOOR_STEPS[frame], 0);
        ++frame;
    }
}
void NineKanEntryDoor(void *) {
    doorPlayRigidMotion(NINE_KAN_DOOR, 0xf0ab, 0, 1);
    nineKanEntryDoorTask = nullptr;
    HLib::DestroyCurrentTask();
}
void NineKanTogyuSound(void *) {
    if (nineKanSoundCounter >= 1)
        --nineKanSoundCounter;
    else {
        Loop3DSoundEffect(0x776);
        nineKanSoundCounter = 48;
    }
}
void NineKanOpenBox(void *data) {
    Vector3 position{};
    Vector3i rotation{.x = -1820, .y = 0, .z = 0};
    EV_scenApplyPosOrRotToPartOf(NINE_KAN_BOX, 152, &position, &rotation, EventCharacterPartTransformMode::Relative);
    if (--*static_cast<std::int32_t *>(data) < 0) {
        nineKanOpenBoxTask = nullptr;
        HLib::DestroyCurrentTask();
    }
}
void NineKanCloseBox(void *data) {
    Vector3 position{};
    Vector3i rotation{.x = 1820, .y = 0, .z = 0};
    EV_scenApplyPosOrRotToPartOf(NINE_KAN_BOX, 152, &position, &rotation, EventCharacterPartTransformMode::Relative);
    if (--*static_cast<std::int32_t *>(data) < 0) {
        nineKanCloseBoxTask = nullptr;
        HLib::DestroyCurrentTask();
    }
}
void NineKanCleanupTasks() {
    if (nineKanOpenDoorTask)
        HLib::DestroyTaskInternal(nineKanOpenDoorTask);
    if (nineKanBurstDoorTask)
        HLib::DestroyTaskInternal(nineKanBurstDoorTask);
    if (nineKanEntryDoorTask)
        HLib::DestroyTaskInternal(nineKanEntryDoorTask);
    if (nineKanCloseDoorTask)
        HLib::DestroyTaskInternal(nineKanCloseDoorTask);
    if (nineKanAuxiliaryTask)
        HLib::DestroyTaskInternal(nineKanAuxiliaryTask);
    if (nineKanOpenBoxTask)
        HLib::DestroyTaskInternal(nineKanOpenBoxTask);
    if (nineKanCloseBoxTask)
        HLib::DestroyTaskInternal(nineKanCloseBoxTask);
    nineKanOpenDoorTask = nullptr;
    nineKanBurstDoorTask = nullptr;
    nineKanEntryDoorTask = nullptr;
    nineKanCloseDoorTask = nullptr;
    nineKanAuxiliaryTask = nullptr;
    nineKanOpenBoxTask = nullptr;
    nineKanCloseBoxTask = nullptr;
}
void NineKanLoop(void *data) {
    NineKanWork &work = *static_cast<NineKanWork *>(data);
    DateTime now;
    CameraUtilityParameters camera;
    switch (work.state) {
    case 1:
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        NineKanInitialPlacement();
        NineKanSetAuthoredControl(0);
        camera = {.first = 1, .second = 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&camera));
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        ClaimDoor(NINE_KAN_DOOR, NINE_KAN_RYO);
        ClaimDoor(NINE_KAN_DOOR, NINE_KAN_TGY);
        work.state = 2;
        return;
    case 2:
        SetPlayerControlEnabled(1);
        EV_setHoldStatesFlags(0);
        EV_SetCanDoAreaJumpTo(0);
        EV_npcUnknownFunc(NINE_KAN_CHO);
        EV_npcUnknownFunc(NINE_KAN_YUA);
        EV_SetCharaControllerFlags(NINE_KAN_REN, 12, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(NINE_KAN_YUA, 12, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(NINE_KAN_CHO, 12, NpcCharacterControllerState::Enabled);
        GetCurrentDateTime(now);
        work.deadline = static_cast<std::uint32_t>(ComputeSecondsSince1980(now)) + 3600;
        work.state = 3;
        return;
    case 3:
        GetCurrentDateTime(now);
        if (NineKanAtIntroductionPoint() || work.deadline < static_cast<std::uint32_t>(ComputeSecondsSince1980(now))) {
            if (srchHasActiveLockonTarget())
                srchReset();
            if (ryoNotBusy()) {
                SetPlayerControlEnabled(0);
                EV_stepSetFlags(0, 0x117);
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                spmFadeOut();
                work.state = 8;
            }
        }
        return;
    case 4:
        NineKanSetAuthoredControl(1);
        work.countdown = NineKanRepeatBox();
        work.state = 5;
        [[fallthrough]];
    case 5:
        if (work.countdown)
            --work.countdown;
        else
            work.state = 6;
        return;
    case 6:
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        NineKanRepeatFinish();
        NineKanSetAuthoredControl(0);
        camera = {.first = 1, .second = 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&camera));
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        work.state = 7;
        return;
    case 7:
        SetPlayerControlEnabled(1);
        EV_SetCanDoAreaJumpTo(0);
        EV_npcUnknownFunc(NINE_KAN_CHO);
        EV_npcUnknownFunc(NINE_KAN_YUA);
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        return;
    case 8:
        if (filmCheckFadeTrackColour())
            return;
        for (const shen_name *name = NINE_KAN_REPEAT_CHARACTERS; name->name != NINE_KAN_INVALID.name; ++name)
            if (DoesScenarioCharacterExist(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
        EV_CheckWarmStartNpcRoute(NINE_KAN_REN);
        EV_ForceLoadNpc(NINE_KAN_REN);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EnableAuthoredCharacterFlagsAndDisplay(NINE_KAN_REN);
        srchSetLookMode();
        SetDisplayFlagForProp(NINE_KAN_PROP, 0);
        NineKanSetAuthoredControl(1);
        EV_EnableCharacterDisplay(NINE_KAN_RYO);
        EV_DisableCharacterDisplay(NINE_KAN_TGY);
        EV_DisableCharacterDisplay(NINE_KAN_BIRD);
        EV_EnableCharacterDisplay(NINE_KAN_AUTHOR_BIRD);
        GetCurrentDateTime(work.date);
        EV_lightLoad(NINE_KAN_LIGHT);
        PushGlobalLightingProgram();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>((work.introRetry ? NINE_KAN_RETRY_INTRO : NINE_KAN_INTRO).name));
        EV_LoadCutsceneFromCurrentScene("0237A000.smp");
        EV_LoadAudio("0237A000", 1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work.state = 9;
        return;
    case 9:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_EnableCharacterDisplay(NINE_KAN_BIRD);
            EV_DisableCharacterDisplay(NINE_KAN_AUTHOR_BIRD);
            ExecuteCleanupFuncForAreaWithCachedScene();
            work.state = 10;
        }
        return;
    case 10:
        work.soundTask = HLib::EnqueueTaskWithoutParameter(NineKanTogyuSound, 4, 0x0b, static_cast<std::int32_t>(NINE_KAN_SOUND_TASK.name));
        nineKanSoundCounter = 48;
        Loop3DSoundEffect(0x776);
        work.countdown = NineKanBeginSearch();
        work.state = 11;
        return;
    case 11:
        if (AuthoredFinished(work, work.introRetry))
            work.state = 12;
        return;
    case 12:
        ClaimDoor(NINE_KAN_DOOR, NINE_KAN_RYO);
        ClaimDoor(NINE_KAN_DOOR, NINE_KAN_TGY);
        SetPlayerControlEnabled(1);
        EV_SetCanDoAreaJumpTo(0);
        NineKanSetAuthoredControl(0);
        camera = {.first = 0, .second = 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&camera));
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        for (const shen_name *name = NINE_KAN_SEARCH_TARGETS; name->name; ++name)
            srchSetTargetEnabled(*name, 0);
        srchClearTarget(2);
        EV_npcUnknownFunc(NINE_KAN_REN);
        EV_npcUnknownFunc(NINE_KAN_CHO);
        EV_npcUnknownFunc(NINE_KAN_YUA);
        EVT_AT_SetPos(NINE_KAN_TGY, NINE_KAN_INVALID, {}, 0);
        EVT_AT_SetAngY(NINE_KAN_TGY, NINE_KAN_INVALID, 0, 0);
        GetCurrentDateTime(now);
        work.deadline = static_cast<std::uint32_t>(ComputeSecondsSince1980(now)) + 200;
        work.state = 13;
        return;
    case 13: {
        const std::int32_t state = NineKanChooseHidingPlace();
        GetCurrentDateTime(now);
        if (state != 17 || work.deadline < static_cast<std::uint32_t>(ComputeSecondsSince1980(now))) {
            if (srchHasActiveLockonTarget())
                srchReset();
            if (ryoNotBusy()) {
                SetPlayerControlEnabled(0);
                NineKanSetAuthoredControl(1);
                work.state = state;
            }
        }
        return;
    }
    case 14:
    case 15:
    case 16:
        srchSetLookMode();
        SetDisplayFlagForProp(NINE_KAN_PROP, 0);
        NineKanEnterHidingPlace(work.state);
        work.state = 19;
        return;
    case 17:
        srchSetLookMode();
        SetDisplayFlagForProp(NINE_KAN_PROP, 0);
        EV_LoadAudio("0237A000", 1);
        EV_spmBeginSampleExecute();
        SetCurrentDateTimeTo(work.date);
        if (work.soundTask) {
            HLib::DestroyTask(work.soundTask);
            work.soundTask = nullptr;
        }
        work.countdown = NineKanSearchFailure();
        work.state = 18;
        return;
    case 18:
        if (AuthoredFinished(work, work.introRetry)) {
            work.introRetry = 1;
            work.state = 8;
        }
        return;
    case 19:
        EV_LoadAudio("0237A000", 1);
        EV_spmBeginSampleExecute();
        GetCurrentDateTime(work.date);
        if (work.soundTask) {
            HLib::DestroyTask(work.soundTask);
            work.soundTask = nullptr;
        }
        work.countdown = NineKanTogyuSearch();
        work.state = 20;
        return;
    case 20:
        if (work.countdown)
            --work.countdown;
        else
            work.state = 21;
        return;
    case 21:
        qteBattleUtil(BattleQteMode::SetWait, 8, 6, 4 * (work.comboRetry > 2 ? 2 : work.comboRetry) + 24);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256);
        qteBattleUtil(BattleQteMode::SetLastCommands);
        qteBattleUtil(BattleQteMode::ResetStates);
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::Start);
        work.state = 22;
        return;
    case 22:
        if (qteBattleUtil(BattleQteMode::GetResult)) {
            qteBattleUtil(BattleQteMode::RetireAndFree);
            work.state = qteBattleUtil(BattleQteMode::GetCompletedCount) != 2 ? 24 : 23;
        }
        return;
    case 23:
        NineKanComboSuccess();
        work.state = 26;
        return;
    case 24:
        work.countdown = NineKanComboFailure();
        work.state = 25;
        return;
    case 25:
        if (AuthoredFinished(work, work.comboRetry)) {
            SetCurrentDateTimeTo(work.date);
            work.state = 19;
            RestoreClosedDoors();
            ++work.comboRetry;
        }
        return;
    case 26:
        EVT_AT_SetCam(7010, NINE_KAN_DUMMY, NINE_KAN_DUMMY, 0, 0, 0);
        SetShowQteFlag(4, static_cast<std::int16_t>(4 * ((work.promptRetry > 2 ? 2 : work.promptRetry) + 4)), 0);
        work.state = 27;
        return;
    case 27:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            work.state = (EV_GetCurrentQteStatus() & 0xfff) != 4 ? 29 : 28;
        return;
    case 28:
        NineKanShelfSuccess();
        work.state = 31;
        return;
    case 29:
        work.countdown = NineKanShelfFailure();
        work.state = 30;
        return;
    case 30:
        if (AuthoredFinished(work, work.promptRetry)) {
            RestoreClosedDoors();
            SetCurrentDateTimeTo(work.date);
            ++work.promptRetry;
            work.state = 19;
        }
        return;
    case 31:
        NineKanEscape();
        work.state = 32;
        return;
    case 32:
        GetCurrentDateTime(work.date);
        if (work.date.hour > 7)
            ++work.date.day;
        work.date.hour = 8;
        work.date.minute = 0;
        work.date.second = 0;
        SetCurrentDateTimeTo(work.date);
        Queue_ST_LoadTitleFadeIn(NINE_KAN_NEXT_DAY);
        EV_stepSetFlags(0, 0x143);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("9KAN"), 0);
        scenFreeEvent("9KAN.EV3");
        EV_scenQueueLoad(g_actNumber, NINE_KAN_DESTINATION, 0);
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition area9KANScriptDefinition{.areaId = AREA_9KAN, .field04 = 0, .init = Area9KANInit, .main = Area9KANMain};
} // namespace shenmue::s2::areas
