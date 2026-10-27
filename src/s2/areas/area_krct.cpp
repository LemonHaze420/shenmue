#include "s2/areas/area_krct.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/pak.hpp"
#include "s2/party_companion.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <cstring>
namespace shenmue::s2::areas {
KrctGlobals krct;
float krctFireHeight;
std::int32_t krctFire, krctHandFire, krctFirePhase, krctFireTicks, krctFirePhaseStep, krctAlternateSound;
namespace {
inline constexpr shen_name house{SHEN_NAME(a016)}, photo{SHEN_NAME(a436)}, fhTitle{SHEN_NAME(FHL1)}, morningTitle{SHEN_NAME(NDAY)};
void ResetCamera() {
    using enum CameraState;
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
}
void RestoreMotion(shen_name character) {
    mtwkSetExtraFlag(scenFindMotionwork(character), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(character), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(character), 13, 0);
}
void LoadCompanion() {
    HLib::Task *const task = krct.task;
    AssignGenericNpcLoadContext(krctCharacters, task);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(KRCT_LOAD_TASK.name));
}
void LoadHouseSound(std::int32_t variant) {
    EV_LoadCutsceneFromCurrentScene("KRC1");
    EV_LoadAudio("KRC1_000", variant);
    EV_spmBeginSampleExecute();
}
void StartAuthor(std::int32_t selection, std::int32_t next, std::int32_t flag = -1) {
    krct.authorSelection = selection;
    krct.state = next;
    if (flag != -1)
        EV_stepSetFlags(0, flag);
    QueueKrctAuthor(&krct.authorSelection);
}
} // namespace
// @verified - p
void KrctNullsub() {
}
// @verified - p
void AreaKRCTInit() {
    std::memset(&krct, 0, sizeof(krct));
    krctFireHeight = .3f;
    krctFire = 0;
    krctHandFire = 0;
    krctFirePhase = 0;
    krctFireTicks = 0;
    krctFirePhaseStep = 0;
}
// @verified - p
void AreaKRCTMain(std::uintptr_t argument) {
    const KrctRequest *const request = reinterpret_cast<const KrctRequest *>(argument);
    if (request->state) {
        krct.state = request->state;
        krct.authorSelection = -1;
        return;
    }
    krct.task = HLib::EnqueueTaskWithoutParameter(KrctMainLoop, 4, 0x0b, static_cast<std::int32_t>(KRCT_TASK.name));
    krct.scene = request->scene;
    krct.objects = request->objects;
    krct.character = request->character;
    krct.lighting = request->lighting;
    krct.authorSelection = 0;
    krct.reserved1c = 0;
    krct.reserved20 = 0;
    krct.trigger = 0;
    krct.state = 1;
    krct.admitted = 1;
}
void KrctMainLoop(void *) {
    using enum CameraState;
    if (krct.authorSelection != -1)
        return;
    DateTime time{};
    switch (krct.state) {
    case 2:
        EV_PTYCUtils(KRCT_SHENHUA, 3, 1);
        KrctNullsub();
        shenSetClockAndCompassRenderTo(1);
        krct.state = 3;
        EV_NPC_NewLoadSw(0);
        EV_npcLockUnlock(KRCT_NPC);
        ptycCharaUnlock(KRCT_SHENHUA);
        EV_npcLockUnlock(KRCT_SHENHUA);
        EV_npcFree(KRCT_SHENHUA);
        HLib::SetupTaskDelay(0);
        LoadCompanion();
        break;
    case 3:
        StartAuthor(0, 4);
        break;
    case 4:
        krct.character(0);
        StartAuthor(1, 5);
        break;
    case 5:
        EV_AT_SetPos(house, EventTransformFlags::AllAxes, {156.8f, 100.25f, 392});
        EV_AT_SetAngY(house, EventTransformFlags::AllAxes, {0, 18204, 0});
        EV_UpdateWeatherForMinute(0, 0, 150, 1);
        EV_UpdateWeatherForMinute(1, 0, 200, 0.7f);
        krct.objects(1);
        krct.lighting(1);
        krct.scene(1);
        LoadHouseSound(1);
        EV_stepSetFlags(0, 236);
        EV_stepSetFlags(0, 308);
        krct.state = 6;
        krct.authorSelection = 0;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("MAKI"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        shenSetClockAndCompassRenderTo(0);
        FinishLoading();
        ExecuteCurrAreaMainFunc_Wrapper(3, reinterpret_cast<std::uintptr_t>(&krct.authorSelection));
        break;
    case 6:
        shenSetClockAndCompassRenderTo(1);
        KrctNullsub();
        GetCurrentDateTime(time);
        time.minute += 20;
        SetCurrentDateTimeTo(time);
        krct.scene(1);
        krct.objects(1);
        krct.lighting(1);
        StartAuthor(2, 32, 414);
        break;
    case 7:
        StartAuthor(4, 32, 415);
        break;
    case 8:
        StartAuthor(5, 32, 419);
        krctAlternateSound = 1;
        break;
    case 9:
        StartAuthor(3, 32, 416);
        break;
    case 10:
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("SHASHIN"), 0))
            break;
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
            break;
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>("SHASHIN")) != 1)
            break;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        krct.authorSelection = 6;
        krct.state = 34;
        EV_stepSetFlags(0, 420);
        if (GetItemState(8007))
            SetScenarioImageModule(photo, "SHAS506I");
        else if (GetItemState(8008))
            SetScenarioImageModule(photo, "SHAS504I");
        else if (GetItemState(8009))
            SetScenarioImageModule(photo, "SHAS505I");
        else if (GetItemState(8010))
            SetScenarioImageModule(photo, "SHAS507I");
        else if (GetItemState(8011))
            SetScenarioImageModule(photo, "SHAS508I");
        QueueKrctAuthor(&krct.authorSelection);
        break;
    case 11:
        StartAuthor(3, 32, 410);
        break;
    case 12:
        krct.scene(1);
        krct.character(1);
        krct.objects(1);
        krct.lighting(0);
        StartAuthor(14, 13, 421);
        break;
    case 13:
        krct.state = 23;
        HLib::SetupTaskDelay(60);
        break;
    case 14:
        EV_EnableCharacterDisplay(house);
        UpdateTempHourMinTimeScale(time.hour, time.minute, -1);
        SetWeatherUpdateEnabledTo(1);
        SetIsSkySetup(1);
        EV_UpdateWeatherForMinute(0, 0, 0, 0.2f);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        krct.scene(1);
        krct.character(1);
        krct.objects(1);
        krct.lighting(0);
        StartAuthor(7, 32, 363);
        krctAlternateSound = 1;
        break;
    case 15:
        StartAuthor(8, 16);
        break;
    case 16:
        krct.state = 24;
        HLib::SetupTaskDelay(60);
        break;
    case 17:
        EV_EnableCharacterDisplay(house);
        UpdateTempHourMinTimeScale(time.hour, time.minute, -1);
        GetCurrentDateTime(time);
        time.hour = 1;
        time.minute = 40;
        SetCurrentDateTimeTo(time);
        SetWeatherUpdateEnabledTo(1);
        SetIsSkySetup(1);
        EV_UpdateWeatherForMinute(0, 0, 0, 0.1f);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_EnableCharacterDisplay(house);
        krct.scene(1);
        krct.character(1);
        krct.objects(1);
        krct.lighting(0);
        EV_stepSetFlags(0, 398);
        StartAuthor(9, 18, 716);
        break;
    case 18:
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        krct.state = 19;
        EV_stepSetFlags(0, 399);
        HLib::SetupTaskDelay(60);
        break;
    case 19:
        EV_LoadCutsceneFromCurrentScene("0530A000");
        EV_LoadAudio("0530A000", 0);
        EV_spmBeginSampleExecute();
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
        GetCurrentDateTime(time);
        if (time.hour >= 19)
            ++time.day;
        time.hour = 8;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        krct.scene(0);
        krct.character(1);
        krct.objects(0);
        EV_stepSetFlags(0, 100);
        Queue_ST_LoadTitleFadeIn(morningTitle);
        krct.state = 20;
        HLib::SetupTaskDelay(60);
        break;
    case 20:
        StartAuthor(13, 25);
        break;
    case 21:
        KrctNullsub();
        shenSetClockAndCompassRenderTo(1);
        krct.state = 22;
        krctAlternateSound = 0;
        HLib::SetupTaskDelay(0);
        EV_NPC_NewLoadSw(0);
        ptycCharaUnlock(KRCT_SHENHUA);
        EV_npcLockUnlock(KRCT_SHENHUA);
        EV_npcFree(KRCT_SHENHUA);
        LoadCompanion();
        break;
    case 22:
        StartAuthor(15, 29);
        break;
    case 23:
        krct.state = 14;
        GetCurrentDateTime(time);
        time.hour += 2;
        SetCurrentDateTimeTo(time);
        Queue_ST_LoadTitleFadeIn(fhTitle);
        krct.objects(0);
        krct.lighting(0);
        ResetCamera();
        HLib::SetupTaskDelay(60);
        break;
    case 24:
        krct.state = 17;
        GetCurrentDateTime(time);
        time.hour += 2;
        SetCurrentDateTimeTo(time);
        Queue_ST_LoadTitleFadeIn(fhTitle);
        krct.objects(0);
        ResetCamera();
        HLib::SetupTaskDelay(60);
        break;
    case 25:
        krct.authorSelection = -1;
        krct.objects(0);
        krct.character(1);
        krct.scene(0);
        LoadHouseSound(0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
        EV_npcLockUnlock(KRCT_NPC);
        EV_npcFree(KRCT_NPC);
        EV_NPC_NewLoadSw(1);
        EV_PTYCUtils(KRCT_SHENHUA, 3, 1);
        KrctConfigureRestoredParty();
        ActivatePartyCompanion(KRCT_SHENHUA);
        EV_PTYCUtils(KRCT_SHENHUA, 3, 0);
        ResetCamera();
        krct.state = 27;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        InitiateCleanupForSceneChara(KRCT_SHENHUA);
        shenSetClockAndCompassRenderTo(0);
        HLib::SetupTaskDelay(30);
        break;
    case 27:
        krct.state = 28;
        SetPlayerControlEnabled(1);
        HLib::SetupTaskDelay(2);
        break;
    case 28:
        if (IsScenarioBusy()) {
            shenToggleAreaFlags(0);
            FinishLoading();
            HLib::DestroyCurrentTask();
        }
        break;
    case 29:
        krct.authorSelection = -1;
        LoadHouseSound(1);
        EV_AT_SetPos(KRCT_RYO, EventTransformFlags::AllAxes, krctPlayerReturn);
        EV_AT_SetAngY(KRCT_RYO, EventTransformFlags::AllAxes, krctReturnAngle);
        RestoreMotion(KRCT_RYO);
        ResetCamera();
        for (const shen_name *name = krctCharacters; name->name != KRCT_INVALID.name; ++name)
            if (DoesScenarioCharacterExist(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
        EV_npcLockUnlock(KRCT_NPC);
        LoadCompanion();
        HLib::SetupTaskDelay(0);
        krct.state = 30;
        break;
    case 30: {
        krct.state = 1;
        krct.authorSelection = 0;
        Vector3 position = krctShenhuaReturn;
        position.y = shenFindIntersectionWithScene(position, 0) + .2f;
        EV_AT_SetPos(KRCT_SHENHUA, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(KRCT_SHENHUA, EventTransformFlags::AllAxes, krctReturnAngle);
        RestoreMotion(KRCT_SHENHUA);
        EV_SetCharaControllerFlags(KRCT_SHENHUA, 1, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(KRCT_SHENHUA, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(KRCT_SHENHUA, 3, NpcCharacterControllerState::Enabled);
        EV_PTYCUtils(KRCT_SHENHUA, 3, 0);
        InitiateCleanupForSceneChara(KRCT_SHENHUA);
        EV_NPC_NewLoadSw(1);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        shenSetClockAndCompassRenderTo(0);
        FinishLoading();
        break;
    }
    case 32:
        if (EV_GetShenmueFlag(715)) {
            krct.state = 12;
            for (std::int32_t flag : {415, 419, 416, 420, 410})
                EV_stepSetFlags(0, flag);
            break;
        }
        scenCvsCtrlFree(GetConverseDataForChara(KRCT_SHENHUA));
        scenCvsCtrlFree(GetConverseDataForChara(KRCT_RYO));
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 680))
            SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_CheckWarmStartNpcRoute(KRCT_SHENHUA);
        EV_CheckWarmStartNpcRoute(KRCT_NPC);
        krct.objects(1);
        krct.character(0);
        if (EV_GetShenmueFlag(398))
            krct.scene(1);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 680))
            Loop3DSoundEffect(0xc49);
        if (!krctAlternateSound)
            LoadHouseSound(1);
        KrctConfigureSeatedParty();
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 680)) {
            EVS_LoopCamera(2000, shen_name{0}, shen_name{0});
            HLib::SetupTaskDelay(106);
        } else {
            EVS_LoopCamera(1000, shen_name{0}, shen_name{0});
            HLib::SetupTaskDelay(60);
        }
        krct.authorSelection = -1;
        krct.state = 33;
        break;
    case 33:
        camCtrlSetState(SnapYawTarget);
        shenSetClockAndCompassRenderTo(0);
        FinishLoading();
        if (StartScenarioFreeTalkRecord(KRCT_SHENHUA, -1) == 1) {
            krct.authorSelection = -1;
            krct.state = 35;
            HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(KrctRestoreHighModels, 4, 0x0b, static_cast<std::int32_t>(KRCT_HIGH_MODEL_TASK.name));
            HLib::SetupTimedTask(task, 2);
            EV_spmBeginSampleExecute();
        }
        break;
    case 34:
        krct.state = 32;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SHASHIN"), 0);
        break;
    case 35: {
        krct.trigger = 0;
        constexpr std::int32_t flags[]{415, 419, 420, 421, 363, 398, 416, 410, 399};
        constexpr std::int32_t triggers[]{1, 3, 4, 6, 7, 8, 9, 10, 11}, states[]{7, 8, 10, 12, 13, 15, 9, 11, 18};
        for (std::size_t i = 0; i < 9; ++i)
            if (EV_GetShenmueFlag(flags[i])) {
                krct.trigger = triggers[i];
                krct.state = 36;
                krct.resumeState = states[i];
            }
        if (!krct.trigger)
            break;
        EVS_LoopCamera(2000, shen_name{0}, shen_name{0});
        StopScenarioFreeTalkRecord();
        EV_SetCharacterAlphaEnabled(KRCT_SHENHUA, false);
        break;
    }
    case 36:
        KrctNullsub();
        shenSetClockAndCompassRenderTo(1);
        krctAlternateSound = 0;
        krct.state = 37;
        if (krct.trigger != 8)
            SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        if (krct.trigger != 3 && krct.trigger != 8)
            spmFadeOut();
        HLib::SetupTaskDelay(15);
        break;
    case 37:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KRCT_RYO), nullptr, 3);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KRCT_RYO), nullptr, 0);
        clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 0);
        EV_CheckWarmStartNpcRoute(KRCT_SHENHUA);
        EV_CheckWarmStartNpcRoute(KRCT_NPC);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1004, 0);
        krct.trigger = 0;
        krct.state = krct.resumeState;
        break;
    default:
        break;
    }
}
const ScriptAreaDefinition areaKRCTScriptDefinition{.areaId = AREA_KRCT, .field04 = 0, .init = AreaKRCTInit, .main = AreaKRCTMain};
} // namespace shenmue::s2::areas
