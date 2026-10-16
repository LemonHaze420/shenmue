#include "s2/areas/area_kbrg.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <algorithm>

namespace shenmue::s2::areas {
namespace {
std::int32_t bridgeState, bridgeSucceeded, bridgeFailures;
DateTime savedDate;
std::byte dateTail[5];
const shen_name bridgeCharacters[2]{KBRG_SHENHUA, KBRG_INVALID};
inline constexpr shen_name bridgeNextMap{SHEN_NAME(KMZ4)};
void ResetBridgeWork() {
    bridgeState = 0;
    bridgeSucceeded = 0;
    bridgeFailures = 0;
    savedDate = {};
    for (std::byte &byte : dateTail)
        byte = std::byte{};
}
void RevealBridgeScene() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
} // namespace
// @verified - p
void AreaKBRGInit() {
    ResetBridgeWork();
}
// @verified - p
void AreaKBRGMain(std::uintptr_t parameter) {
    const std::int32_t entry = static_cast<std::int32_t>(parameter);
    HLib::Task *task = nullptr;
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(KBRG_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(KBRG_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0540A000");
    EV_FreeAuthorPak(1, "0540");
    LoadBlurModelForCharacter(KBRG_PLAYER, "RYO_B.MT7", 1.0f);
    ResetBridgeWork();
    if (entry == 0)
        task = HLib::EnqueueTaskWithoutParameter(KBRGStoryCallback, 5, 0x0b, static_cast<std::int32_t>(KBRG_TASK.name));
    else if (entry == 1)
        task = HLib::EnqueueTaskWithoutParameter(KBRGQteCallback, 5, 0x0b, static_cast<std::int32_t>(KBRG_TASK.name));
    AssignAreaNpcLoadContext(bridgeCharacters, task);
    g_usingCustomAuthorCharacterList = 0;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(KBRG_LOAD_TASK.name));
    SetRenderClockAndCompassTo(1);
    GetCurrentDateTime(savedDate);
}
// @verified - p
void KBRGPrepareCharacter(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
// @verified - p
void KBRGStoryCallback(void *) {
    switch (bridgeState) {
    case 0:
        KBRGPrepareCharacter(KBRG_PLAYER);
        KBRGPrepareCharacter(KBRG_SHENHUA);
        EV_PTYCUtils(KBRG_SHENHUA, 3, 1);
        bridgeState = 1;
        break;
    case 1:
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KBRG_STORY_AUTHOR.name));
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1662)) {
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(-16));
            EV_SetAuthorActiveTo(1);
        }
        RevealBridgeScene();
        EV_spmBeginSampleExecute();
        bridgeState = 2;
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_AT_SetPos(KBRG_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{-18.620001f, -1.0f, -34.27f});
            EV_AT_SetAngY(KBRG_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 54613, 0});
            mtwkRequestMotion(scenFindMotionwork(KBRG_PLAYER), 0xf003, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(KBRG_PLAYER), 71, 0, MotionRegion::LowerBody);
            EV_SetCharaControllerFlags(KBRG_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(KBRG_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(CameraState::Init);
            EV_DisableCharacterDisplay(KBRG_SHENHUA);
            RevealBridgeScene();
            HLib::SetupTaskDelay(30);
            bridgeState = 6;
        }
        break;
    case 6:
        KBRGRestoreGameplay();
        break;
    default:
        break;
    }
}
// @verified - p
void KBRGQteCallback(void *) {
    switch (bridgeState) {
    case 0:
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        KBRGPrepareCharacter(KBRG_PLAYER);
        KBRGPrepareCharacter(KBRG_SHENHUA);
        EV_PTYCUtils(KBRG_SHENHUA, 3, 1);
        bridgeState = 1;
        break;
    case 1:
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KBRG_QTE_AUTHORS[0].name));
        RevealBridgeScene();
        EV_spmBeginSampleExecute();
        bridgeState = 5;
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (bridgeSucceeded == 1)
                EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KBRG_QTE_AUTHORS[1].name));
            else {
                if (bridgeFailures > 0)
                    EV_SetAuthorActiveTo(1);
                ++bridgeFailures;
                SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
                EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KBRG_QTE_AUTHORS[2].name));
            }
            bridgeState = 6;
        }
        break;
    case 3:
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::SetWait, 8, 6, 6 * (std::min(bridgeFailures, 2) + 4));
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 1, 256);
        qteBattleUtil(BattleQteMode::SetLastCommands);
        qteBattleUtil(BattleQteMode::ResetStates);
        qteBattleUtil(BattleQteMode::Start);
        bridgeState = 4;
        break;
    case 4:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (qteBattleUtil(BattleQteMode::GetResult)) {
            bridgeSucceeded = qteBattleUtil(BattleQteMode::GetCompletedCount) == 3;
            qteBattleUtil(BattleQteMode::RetireAndFree);
            bridgeState = 2;
        }
        break;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            bridgeState = 3;
        break;
    case 6:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (bridgeSucceeded == 1) {
                shenSetTimeSimulationTo(1);
                DateTime date;
                GetCurrentDateTime(date);
                const std::uint32_t seconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(date)) + 1800U;
                IncrementDateTimeBySeconds(seconds, date);
                SetCurrentDateTimeTo(date);
                EV_scenQueueLoad(g_actNumber, bridgeNextMap, 0);
                EV_stepSetFlags(0, 577);
                EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            } else
                bridgeState = 0;
        }
        break;
    default:
        break;
    }
}
// @verified - p
void KBRGRestoreGameplay() {
    LoadCutsceneAndSoundProgramForArea();
    EnableCharDisplayAndSetMotionworkData(KBRG_PLAYER);
    EnableCharDisplayAndSetMotionworkData(KBRG_SHENHUA);
    SetPlayerControlEnabled(1);
    EV_SetCanDoAreaJumpTo(0);
    EV_SetCharaControllerFlags(KBRG_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(KBRG_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_EventSetUpHelpIcon_0(0x3f, 0);
    HLib::DestroyCurrentTask();
    scenFreeEvent("KBRG");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KBRG"), 0);
    ActivatePartyCompanion(KBRG_SHENHUA);
    EV_PTYCUtils(KBRG_SHENHUA, 3, 1);
    EV_AT_SetPos(KBRG_SHENHUA, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{-21.620001f, -1.0f, -33.599998f});
    EV_AT_SetAngY(KBRG_SHENHUA, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 54613, 0});
    EV_EnableCharacterDisplay(KBRG_SHENHUA);
    SetCurrentDateTimeTo(savedDate);
    shenSetTimeSimulationTo(0);
}
const ScriptAreaDefinition areaKBRGScriptDefinition{.areaId = AREA_KBRG, .field04 = 0, .init = AreaKBRGInit, .main = AreaKBRGMain};
} // namespace shenmue::s2::areas
