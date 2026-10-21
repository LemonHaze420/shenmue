#include "s2/areas/area_khin.hpp"
#include "s2/areas/khin_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
std::uint32_t khinSelector;
namespace {
void SelectKhinCallback(void *) {
    if (khinSelector <= 22) {
        if (khinSelector <= 5)
            EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(KhinPurchaseCallback);
    } else if (khinSelector == 38)
        HLib::SetCurrentTaskTo(KhinReturnToMenuCallback);
    else if (khinSelector <= 41) {
        EV_EventSetUpHelpIcon_0(26, 0);
        HLib::SetCurrentTaskTo(KhinSlotCallback);
    }
}
} // namespace
void InitializeKhinEvent() {
    khinSelector = 0;
}
void RunKhinEvent(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(SelectKhinCallback, 5, 0x0b, sizeof(KhinWork), KHIN_TASK);
    KhinWork *work = static_cast<KhinWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    khinSelector = reinterpret_cast<const KhinInput *>(argument)->selector;
    work->caller = HLib::GetCurrentTask();
    EV_setHoldStatesFlags(0);
    EV_SetNewSceneIndex();
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(KHIN_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(KHIN_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
        EV_FreeAuthorPak(1, "0185");
        EV_FreeAuthorPak(1, "0112");
    }
}
void KhinSlotCallback(void *data) {
    KhinWork &work = *static_cast<KhinWork *>(data);
    switch (work.state) {
    case 0:
        work.row = &khinCharacters[khinSelector];
        EV_CheckWarmStartNpcRoute(work.row->character);
        mtwkRequestMotion(scenFindMotionwork(KHIN_RYO), 0xf003, MotionRegion::LowerBody);
        work.state = 1;
        break;
    case 1:
        if (!EV_ForceLoadNpc(work.row->character))
            break;
        srchReset();
        SetScenarioReflectorBoardValues(KHIN_RYO, &khinReflector);
        EV_SetRefBoardParamsFor(KHIN_RYO, 1);
        SetScenarioReflectorBoardValues(work.row->character, &khinReflector);
        EV_SetRefBoardParamsFor(work.row->character, 1);
        EV_SetRefBoardParamsFor(KHIN_RYO, 1);
        EV_SetRefBoardParamsFor(work.row->character, 1);
        EV_SetCharacterAlphaEnabled(KHIN_RYO, false);
        EV_SetCharacterAlphaEnabled(work.row->character, false);
        EV_SetCharaControllerFlags(work.row->character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(work.row->character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        work.state = 2;
        break;
    case 2: {
        std::int32_t delay = 0;
        switch (khinSelector) {
        case 23:
            delay = KhinXhgDeposit(work);
            break;
        case 24:
            delay = KhinXhgWithdrawal(work);
            break;
        case 25:
            delay = KhinXhgLeave(work);
            break;
        case 26:
            delay = KhinXhgCancel(work);
            break;
        case 27:
        case 32:
        case 37:
            delay = KhinTokensGreeting(work);
            break;
        case 28:
            delay = KhinGstDeposit(work);
            work.nextSelector = 40;
            break;
        case 29:
            delay = KhinGstWithdrawal(work);
            break;
        case 30:
            delay = KhinGstLeave(work);
            break;
        case 31:
            delay = KhinGstCancel(work);
            break;
        case 33:
            delay = KhinAgnDeposit(work);
            break;
        case 34:
            delay = KhinAgnWithdrawal(work);
            break;
        case 35:
            delay = KhinAgnLeave(work);
            break;
        case 36:
            delay = KhinAgnCancel(work);
            break;
        case 39:
            delay = KhinXhgAward(work);
            break;
        case 40:
            delay = KhinGstAward(work);
            break;
        case 41:
            delay = KhinAgnAward(work);
            break;
        default:
            break;
        }
        switch (khinSelector) {
        case 23:
            work.remaining = delay;
            work.nextSelector = 39;
            work.state = 3;
            return;
        case 28:
            work.remaining = delay;
            work.nextSelector = 40;
            work.state = 3;
            return;
        case 33:
            work.remaining = delay;
            work.nextSelector = 41;
            work.state = 3;
            return;
        case 27:
        case 32:
        case 37:
            khinSelector -= EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1809) ? 4 : 2;
            work.row = &khinCharacters[khinSelector];
            break;
        default:
            work.state = 4;
            break;
        }
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 3: {
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            work.remaining = 0;
        }
        const std::int32_t previous = work.remaining--;
        if (previous <= 0) {
            khinSelector = work.nextSelector;
            work.state = 2;
        }
        break;
    }
    case 4:
        HLib::SetCurrentTaskTo(KhinSlotExitCallback);
        break;
    default:
        break;
    }
}
void KhinPurchaseExitCallback(void *data) {
    using enum CameraState;
    KhinWork &work = *static_cast<KhinWork *>(data);
    if (g_currentMap.name == KHIN_QJBT.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    switch (khinSelector) {
    case 0:
    case 3:
    case 4:
        EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {-.15f, 0, 0}, 0);
        EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, khinSelector == 0 ? -50.0f : -60.0f, 0);
        break;
    case 11:
    case 15:
    case 19:
        EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 0);
        EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, -20.0f, 0);
        break;
    default:
        if (khinSelector <= 22) {
            EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 0);
            EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, -50.0f, 0);
        }
        break;
    }
    EV_setHoldStatesFlags(1);
    EV_setHoldStatesFlags(1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(work.row->character), Normal, 10, 1);
    EVT_AT_SetPoint(KHIN_RYO, KHIN_NONE, 0, 1, 0, 0, 0, 0x50000000U);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KHIN_RYO), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(work.row->character), nullptr, 0);
    (void)FindMotionWork(FindCharacterByScenarioName(KHIN_RYO));
    (void)FindMotionWork(FindCharacterByScenarioName(work.row->character));
    EV_SetCharacterAlphaEnabled(KHIN_RYO, true);
    EV_SetCharacterAlphaEnabled(work.row->character, true);
    EV_EnableCharacterDisplay(KHIN_RYO);
    AT_SetLocalBaseChara(KHIN_HAND, KHIN_INVALID);
    EV_SetRefBoardParamsFor(KHIN_RYO, 0);
    EV_SetRefBoardParamsFor(KHIN_HAND, 0);
    EV_SetRefBoardParamsFor(work.row->character, 0);
    EV_npcFree(work.row->character);
    EV_npcLockUnlock(work.row->character);
    LoadCutsceneAndSoundProgramForArea();
    EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
    EV_SetRefBoardParamsFor(KHIN_RYO, 0);
    EV_SetCharaControllerFlags(KHIN_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(KHIN_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0185");
    EV_FreeAuthorPak(0, "0112");
    EVS_LoopCamera(0, KHIN_NONE, KHIN_NONE);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
}
void KhinSlotExitCallback(void *data) {
    using enum CameraState;
    KhinWork &work = *static_cast<KhinWork *>(data);
    EV_EnableCharacterDisplay(KHIN_RYO);
    EV_SetCharaControllerFlags(KHIN_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(KHIN_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetRefBoardParamsFor(KHIN_RYO, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KHIN_RYO), nullptr, 0);
    EVT_AT_SetPoint(KHIN_RYO, KHIN_NONE, 0, 1, 0, 0, 0, 0x50000000U);
    EV_SetCharacterAlphaEnabled(KHIN_RYO, true);
    EV_SetCharacterAlphaEnabled(work.row->character, true);
    EV_SetCharaControllerFlags(work.row->character, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(work.row->character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetRefBoardParamsFor(work.row->character, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(work.row->character), nullptr, 0);
    EV_npcFree(work.row->character);
    EV_npcLockUnlock(work.row->character);
    LoadCutsceneAndSoundProgramForArea();
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0185");
    EVS_LoopCamera(0, KHIN_NONE, KHIN_NONE);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    switch (khinSelector) {
    case 23:
    case 39:
        EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
        [[fallthrough]];
    case 25:
        EV_setHoldStatesFlags(1);
        EV_scenQueueLoad(g_actNumber, KHIN_WE00, 79);
        break;
    case 24:
    case 26:
    case 29:
    case 31:
    case 34:
    case 36:
        SetScreenFilmFade(8, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 28:
    case 40:
        EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
        [[fallthrough]];
    case 30:
        EV_setHoldStatesFlags(1);
        EVT_AT_SetPos(KHIN_RYO, KHIN_INVALID, {204.2f, 103.95f, 90.34f}, 0);
        EVT_AT_SetAngY(KHIN_RYO, KHIN_INVALID, 30, 0);
        EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EVS_LoopCamera(0, KHIN_NONE, KHIN_NONE);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        break;
    case 33:
    case 41:
        EVT_AT_MenCtrl({.frames = 8, .startAlpha = 255});
        [[fallthrough]];
    case 35:
        EV_setHoldStatesFlags(1);
        EV_scenQueueLoad(g_actNumber, KHIN_QAW1, 99);
        break;
    default:
        break;
    }
    SetPlayerControlEnabled(1);
    HLib::ResolveDeferredCallback(work.caller);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
    HLib::EV_HLKillOtherHierarchyTask(work.task);
}
void KhinReturnToMenuCallback(void *data) {
    using enum CameraState;
    KhinWork &work = *static_cast<KhinWork *>(data);
    if (work.state == 0) {
        EVT_AT_SetCam(5000, KHIN_INVALID, KHIN_INVALID, 0, 0, 0);
        camCtrlDisableFilmBars();
        EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x80000000U, 0);
        dbgPrintEventMessage(GetLocalizedSceneText("UISMRETMENU").data()); // @text EN: "Return to menu?" | JA: "メニューに戻りますか？"
        std::int16_t choices[]{-1, 82, 83, -1};
        SetupControlSpeakAndUI(choices, 200, 0, 2);
        work.state = 1;
    } else if (work.state == 1) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return;
        if (shenGetCtrlSpeakIconFlag() == 82) {
            EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255});
            work.state = 3;
            HLib::SetupTaskDelay(16);
        } else {
            EVT_AT_SetMot(KHIN_RYO, 0xf024, 0, 1, 0x80000000U, 0);
            EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 40);
            EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 180, 40);
            EVT_AT_SetMot(KHIN_RYO, 0xf005, 0, 1, 0x40000000U, 40);
            work.state = 2;
            HLib::SetupTaskDelay(70);
        }
    } else if (work.state == 2 || work.state == 3) {
        const bool leave = work.state == 3;
        ctrlStopSubtitles();
        EVT_AT_SetPoint(KHIN_RYO, KHIN_NONE, 0, 3, 0, 0, 0, 0x50000000U);
        LoadCutsceneAndSoundProgramForArea();
        if (!leave) {
            EV_SetCharaControllerFlags(KHIN_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(KHIN_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        }
        EV_NPC_NewLoadSw(1);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work.task);
        if (leave)
            EV_scenQueueLoad(g_actNumber, KHIN_OMAK, 1);
    }
}
} // namespace shenmue::s2
