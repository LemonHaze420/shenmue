#include "s2/areas/area_wkta.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>

namespace shenmue::s2 {
// @verified - p
void WktaInit() {
}
void WktaMain(std::uintptr_t argument) {
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(WKTA_RYO)), 0xf002, MotionRegion::LowerBody);
    spmFadeOut();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WktaOpeningCallback, 4, 0x0b, sizeof(WktaWork), WKTA_EVENT);
    auto *work = static_cast<WktaWork *>(HLib::GetTaskParameter(task));
    SetPlayerControlEnabled(0);
    std::memset(reinterpret_cast<std::byte *>(work) + 8, 0, sizeof(WktaWork) - 8);
    work->self = task;
    work->phase = 1;
    work->response = static_cast<std::int32_t>(argument);
    EV_SetNewSceneIndex();
    (void)EV_LoadCutsceneFromCurrentScene("0024E000.smp");
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WKTA"), 0);
    (void)EV_LoadAudio("0024E000", 0);
    (void)EV_spmBeginSampleExecute();
    (void)EV_FreeAuthorPak(1, "0024");
}
// @verified - p
void FinishWkta(WktaWork *work) {
    using enum CameraState;
    (void)camCtrlSetState(Init);
    (void)LoadCutsceneAndSoundProgramForArea();
    SetPlayerControlEnabled(1);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WKTA"), 0);
    scenFreeEvent("WKTA.EV3");
    HLib::DestroyTask(work->self);
    (void)EV_FreeAuthorPak(0, "0024");
}
// @verified - p
void WktaOpeningCallback(void *data) {
    using enum CameraState;
    auto &work = *static_cast<WktaWork *>(data);
    switch (work.phase) {
    case 1:
        if (EV_ForceLoadNpc(WKTA_KKI) == 1) {
            work.savedAnimation = GetMotionAnimationId(FindMotionWork(FindCharacterByScenarioName(WKTA_KKI)), MotionRegion::LowerBody);
            EV_DisableCharacterDisplay(WKTA_KKI);
            EV_SetCharaControllerFlags(WKTA_KKI, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(WKTA_KKI, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            (void)EV_CheckWarmStartNpcRoute(WKTA_KKI);
            (void)EV_npcUnknownFunc(WKTA_KKI);
            EV_SetCharacterAlphaEnabled(WKTA_KKI, false);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 955, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1167, 1);
            work.phase = 2;
        }
        break;
    case 2:
        EV_GetCharaPosition(WKTA_KKI, 0xffffffffU, &work.originalPosition, EventTransformFlags::World);
        EVT_AT_SetCam(100, WKTA_DOOR_BASE, WKTA_DOOR_BASE, 0, 0, 0);
        EVT_AT_SetPos(WKTA_RYO, WKTA_PLAYER_BASE, {0.08f, 0, 1.76f}, 0);
        EVT_AT_SetPos(WKTA_KKI, WKTA_DOOR_BASE, {0.80949003f, 0, -0.3f}, 0);
        EVT_AT_SetAngY(WKTA_KKI, WKTA_DOOR_BASE, -138.4445f, 0);
        EVT_AT_SetAngY(WKTA_RYO, WKTA_PLAYER_BASE, 0, 0);
        (void)EVT_AT_SetMot(WKTA_RYO, 0xf002, 0, 1, 0x40000000U, 0);
        work.phase = 3;
        break;
    case 3:
        EV_EnableCharacterDisplay(WKTA_KKI);
        EVT_AT_SetPoint(WKTA_KKI, WKTA_RYO, 2, 1, 0, 0, 0, 0x20000000U);
        (void)doorLoopRigidMotion(WKTA_DOOR, 0x1309, 0, 1);
        (void)EVT_AT_SetMot(WKTA_KKI, 0x1304, 0, 1, 0x40000000U, 0);
        if (work.response)
            EVT_AT_SetVoice("0024B002", 1, 60); // @text EN: "=@what is it?" | JA: "ふああ…うるせえな…"
        else {
            EVT_AT_SetVoice("0024B001", 1, 100); // @text EN: "What do you want?" | JA: "なんだ、うるせえな！"
            EVT_AT_SetVoice("0024A001", 1, 195); // @text EN: "Excuse me." | JA: "すいません。"
        }
        InitiateCleanupForSceneChara(WKTA_KKI);
        work.phase = 4;
        HLib::SetupTaskDelay(232);
        break;
    case 4:
        (void)EVT_AT_SetMot(WKTA_KKI, 0x1303, 0, 1, 0, 0);
        (void)camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        (void)EV_npcUpdateFlags(WKTA_KKI);
        HLib::SetCurrentTaskTo(WktaClosingCallback);
        work.phase = 5;
        break;
    case 5:
        work.phase = 6;
        break;
    case 6:
        work.phase = 8;
        break;
    case 8:
        (void)EVT_AT_SetMot(WKTA_KKI, work.savedAnimation, 0, 1, 0, 0);
        FinishWkta(&work);
        break;
    default:
        break;
    }
}
// @verified - p
void WktaClosingCallback(void *data) {
    auto &work = *static_cast<WktaWork *>(data);
    switch (work.phase) {
    case 2:
        work.phase = 3;
        break;
    case 3:
        work.phase = 4;
        break;
    case 4:
        work.phase = 5;
        break;
    case 5:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 955) == 1) {
            (void)EV_CheckWarmStartNpcRoute(WKTA_KKI);
            (void)EV_npcUnknownFunc(WKTA_KKI);
            SetPlayerControlEnabled(0);
            work.phase = 6;
            HLib::SetupTaskDelay(1);
        }
        break;
    case 6: {
        EVT_AT_SetCam(200, WKTA_DOOR_BASE, WKTA_DOOR_BASE, 0, 0, 0);
        EVT_AT_SetPos(WKTA_KKI, WKTA_PLAYER_BASE, {-0.2281f, 0, 0.8251f}, 0);
        EVT_AT_SetAngY(WKTA_KKI, WKTA_PLAYER_BASE, 172.55679f, 0);
        (void)doorPlayRigidMotion(WKTA_DOOR, 0x1308, 0, 1);
        (void)EVT_AT_SetMot(WKTA_KKI, 0x1302, 0, 1, 0x40000000U, 0);
        (void)EVT_AT_SetSound(0x831, 55);
        (void)EVT_AT_SetSound(0x263, 102);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), WKTA_EVENT);
        RegisterEventTask(task);
        *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, WKTA_KKI, 1, 115.0f};
        work.phase = 7;
        HLib::SetupTaskDelay(115);
        break;
    }
    case 7:
        (void)ProcessAicaCommand(0xaa0, 30);
        work.phase = 8;
        HLib::SetupTaskDelay(30);
        break;
    case 8:
        EV_EnableCharacterDisplay(WKTA_KKI);
        EV_SetCharacterAlphaEnabled(WKTA_KKI, true);
        EV_SetCharaControllerFlags(WKTA_KKI, UpdateHeight, NpcCharacterControllerState::Enabled);
        (void)EV_npcFree(WKTA_KKI);
        (void)EV_npcLockUnlock(WKTA_KKI);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 955, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1167, 0);
        FinishWkta(&work);
        break;
    default:
        break;
    }
}
const ScriptAreaDefinition wktaScriptDefinition{.areaId = AREA_WKTA, .field04 = 0, .init = WktaInit, .main = WktaMain};
} // namespace shenmue::s2
