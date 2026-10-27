#include "s2/areas/maki_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/search.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <cstring>

namespace shenmue::s2::areas {
MakiRuntime makiRuntime;
SearchTarget makiFirewoodTargets[30];
std::int32_t makiSelectedFirewood, makiSavedCameraContext, makiPreviousDeposit, makiSavedRootMode, makiSavedBlendFrames;
Vector3 makiSavedPosition;
Vector3i makiSavedRotation;
namespace {
void InitializeMakiEvent() {
    std::memset(&makiRuntime, 0, sizeof(makiRuntime));
    makiSavedCameraContext = 0;
    std::memset(makiFirewoodTargets, 0, sizeof(makiFirewoodTargets));
    makiPreviousDeposit = -1;
    makiSavedPosition = {.x = 0, .y = 0, .z = 0};
    makiSavedRotation = {.x = 0, .y = 0, .z = 0};
    makiLighterColour = {.x = 0, .y = 0, .z = 0, .w = 0};
    makiLighterParameters = {.x = 0, .y = 0, .z = 0};
    makiLighterOffset = {.x = 0, .y = 0, .z = 0};
    makiSelectedFirewood = 0;
    makiSavedRootMode = 0;
    makiSavedBlendFrames = 0;
    makiLighterPhase = 0;
}
void MakiExit() {
    using enum CameraState;
    *makiRuntime.result = -1;
    if (mtwkSetExtraFlag(scenFindMotionwork(MAKI_RYO), 59, 1))
        UpdateMotionWorkWithPosition(scenFindMotionwork(MAKI_RYO), nullptr, 0);
    if (mtwkSetExtraFlag(scenFindMotionwork(MAKI_SHENHUA), 59, 1))
        UpdateMotionWorkWithPosition(scenFindMotionwork(MAKI_SHENHUA), nullptr, 0);
    clmdUtil(FindCharacterByScenarioName(MAKI_SHENHUA), 0);
    EV_EventSetUpHelpIcon_0(0, 0);
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::LeftArm);
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::RightArm);
    for (std::int32_t i = 0; makiFirewoodPlacements[i].type != 3; ++i)
        SetCharaKill(makiFirewoodTargets[i].name, 1);
    EV_npcUpdateFlags(MAKI_SHENHUA);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_RYO), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_SHENHUA), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_SHENHUA), 0, 1, 0);
    ResetScenarioCharacterAttachment(MAKI_LIGHTER);
    EV_DisableCharacterDisplay(MAKI_LIGHTER);
    SetLightEntryIndex(14, 0);
    ET_AT_KillWork(makiRuntime.characters);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(0, "0518");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("MAKI"), 0);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    const CameraUtilityParameters parameters{.first = 0, .second = 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(makiRuntime.lightTask);
    HLib::EV_HLKillOtherHierarchyTask(makiRuntime.mainTask);
    FinishLoading();
    EV_SetCanDoAreaJumpTo(1);
}
void MakiBeginWalk() {
    using enum CameraState;
    switch (makiRuntime.phase) {
    case 0:
        EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 157.1f, .y = 100, .z = 389.4f}, 0);
        EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 315, 0);
        EVT_AT_SetPos(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, {.x = 154.9f, .y = 100, .z = 393.5f}, 0);
        EVT_AT_SetAngY(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, 0, 0);
        EVT_AT_SetMot(MAKI_RYO, 0xf005, 0, 1, 0, 0);
        EVT_AT_SetMot(MAKI_SHENHUA, 0x4008, 0, 1, 0x40000000U, 0);
        mtwkRequestMotion(scenFindMotionwork(MAKI_RYO), 0x4004, MotionRegion::RightArm);
        MakiAttachLighter(0);
        SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::LeftArm);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        ++makiRuntime.phase;
        HLib::SetupTaskDelay(30);
        break;
    case 1: {
        const CameraUtilityParameters parameters{.first = 1, .second = 1};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
        EVT_AT_SetMot(MAKI_RYO, 0xf002, 0, 1, 0, 30);
        ++makiRuntime.phase;
        HLib::SetupTaskDelay(30);
        break;
    }
    case 2:
        makiRuntime.state = 1;
        makiRuntime.phase = 0;
        SetPlayerControlEnabled(1);
        HLib::SetCurrentTaskTo(MakiMainCallback);
        break;
    default:
        break;
    }
}
void MakiFindFirewood() {
    using enum CameraState;
    if (makiRuntime.phase == 1) {
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        makiRuntime.phase = 0;
    }
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour >= 21 && (time.minute >= 30 || time.hour > 21))
        makiRuntime.state = 5;
    Vector3 position;
    EV_GetCharaPosition(MAKI_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (ComputeAngle(position, 2.8f, 0, 0x8000, {.x = 154.8f, .y = 100, .z = 392.2f})) {
        if (time.hour != 21)
            makiRuntime.state = 3;
        else {
            makiRuntime.state = 5;
            if (time.minute < 20)
                makiRuntime.state = 3;
        }
    }
    const shen_name focus = srchGetTargetName(0);
    if (focus.name == 0x4b4c) {
        const shen_name button{SHEN_NAME(BTEV)};
        GetBattleDataConfig(0, button);
        if (GetBattleDataConfig(1, button) && GetBattleDataConfig(2, button)) {
            if (makiRuntime.heldCount >= 7) {
                const char *first[]{"0518A040", "0518A041"}; // @text 0518A040 EN: "There's some here." | JA: "ここにもあるな…" ; 0518A041 EN: "Firewood=@" | JA: "…たき木だ。"
                const char *second[]{"0518A042", "0518A043"}; // @text 0518A042 EN: "I can't hold any more." | JA: "でも、これ以上は持ちきれないな。" ; 0518A043 EN: "I should go and put these down\nbefore I gather more." | JA: "いったん木を置いてこないと、\n拾えないな。"
                srchReset();
                EVT_AT_SetVoice(first[shenGetRandomInt32(2)], 0);
                EVT_AT_SetVoice(second[shenGetRandomInt32(2)], 40);
                makiRuntime.phase = 1;
                SetPlayerControlEnabled(0);
                HLib::SetupTaskDelay(120);
            } else
                makiRuntime.state = 2;
        }
    }
    SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::RightArm);
    SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::LeftArm);
}
void RunMakiEvent(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    EV_SetCanDoAreaJumpTo(0);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    EV_EventSetUpHelpIcon_0(5, 0);
    EV_npcUnknownFunc(MAKI_SHENHUA);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_RYO), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_SHENHUA), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAKI_SHENHUA), 0, 1, 0);
    makiRuntime.result = reinterpret_cast<std::int32_t *>(argument);
    makiRuntime.state = 0;
    makiRuntime.heldCount = 0;
    makiRuntime.depositedCount = 0;
    makiRuntime.phase = 0;
    const char *models[]{"KRC1IK5G", "KRC1IK6G", "KRC1IK7G"};
    std::int32_t index = 0;
    shen_name character{SHEN_NAME(LK00)};
    for (; makiFirewoodPlacements[index].type != 3; ++index, character.name += 0x01000000U) {
        const MakiFirewoodPlacement &placement = makiFirewoodPlacements[index];
        CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(models[placement.type]), character);
        EV_AT_SetPos(character, EventTransformFlags::AllAxes, placement.position);
        EV_AT_SetAngY(character, EventTransformFlags::AllAxes, placement.rotation);
        const Vector3 scale{.x = 1, .y = 1, .z = 1};
        mtwkSetMotionworkScale3(character, &scale);
        EV_EnableCharacterDisplay(character);
        SearchTarget &target = makiFirewoodTargets[index];
        target.position.x = placement.position.x;
        target.position.y = placement.position.y;
        target.position.z = placement.position.z;
        target.name = character;
        target.flags = 8;
        target.lockonAngle = .5f;
    }
    makiFirewoodTargets[index].name = shen_name{};
    EventCling cling{.cleanup = 0,
        .target = MAKI_CLING,
        .character = MAKI_RYO,
        .bone = 21,
        .position = {.x = 0, .y = 0, .z = 0},
        .rotation = {.x = 0, .y = 0, .z = 0},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    for (std::int32_t i = 0; i < 16; ++i) {
        makiRuntime.heldCharacters[i] = makiHeldLogs[i];
        EVT_AT_SetPos(makiRuntime.heldCharacters[i], MAKI_CLING, {.x = 0, .y = .08f, .z = .2f}, 0);
        EventSetRawAngle angle{
            .cleanup = 0, .character = makiRuntime.heldCharacters[i], .parent = MAKI_CLING, .rotation = {.x = 0, .y = 24576, .z = 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&angle);
    }
    AT_SetLocalBaseChara(MAKI_SMALL_LOG, MAKI_CLING);
    AT_SetLocalBaseChara(MAKI_MIDDLE_LOG, MAKI_CLING);
    AT_SetLocalBaseChara(MAKI_LARGE_LOG, MAKI_CLING);
    const Vector3 scale{.x = 1, .y = 1, .z = 1};
    mtwkSetMotionworkScale3(MAKI_SMALL_LOG, &scale);
    mtwkSetMotionworkScale3(MAKI_MIDDLE_LOG, &scale);
    mtwkSetMotionworkScale3(MAKI_LARGE_LOG, &scale);
    srchSetTarget(2, makiFirewoodTargets);
    EV_FreeAuthorPak(1, "0518");
    makiRuntime.mainTask = HLib::EnqueueTaskWithoutParameter(MakiMainCallback, 4, 0x0b, static_cast<std::int32_t>(MAKI_TASK.name));
    makiRuntime.lightTask = QueueMakiLighterLight();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MAKI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("MAKI"), 0);
    clmdUtil(FindCharacterByScenarioName(MAKI_SHENHUA), 15);
    makiRuntime.characters = Queue_EVT_AT_ReadChr(makiRuntime.mainTask, makiReadCharacters);
}
} // namespace

void MakiMainCallback(void *) {
    std::int32_t delay = 0;
    switch (makiRuntime.state) {
    case 0:
        MakiBeginWalk();
        return;
    case 1:
        MakiFindFirewood();
        return;
    case 2:
        delay = MakiCollectFirewood();
        break;
    case 3:
        delay = MakiReturnToCampfire();
        break;
    case 4:
        delay = MakiFinishCollecting();
        break;
    case 5:
        delay = MakiNightfall();
        break;
    case 6:
        MakiExit();
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void MakiLockPlayerForSequence() {
    SetPlayerControlEnabled(0);
    RefreshPadControlData(0, 1);
    RequestMemoClose();
    EV_SetCharaControllerFlags(MAKI_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(MAKI_RYO, 3, NpcCharacterControllerState::Disabled);
    makiSavedCameraContext = static_cast<std::int32_t>(camCtrlUtil(CameraUtilityMode::GetContext, 0));
    EV_EnableCharacterDisplay(MAKI_RYO);
    makiSavedRootMode = static_cast<std::int32_t>(GetMotionTrackedRootMode(scenFindMotionwork(MAKI_RYO)));
    makiSavedBlendFrames = static_cast<std::int32_t>(GetMotionTransitionFramesRemaining(scenFindMotionwork(MAKI_RYO), MotionRegion::LowerBody));
    mtwkSetRequestedRootMode(scenFindMotionwork(MAKI_RYO), 3);
    SetMotionFrameTime(scenFindMotionwork(MAKI_RYO), 7, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::LeftArm);
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::RightArm);
}
void MakiRestorePlayerAfterSequence() {
    using enum CameraState;
    const CameraUtilityParameters parameters{.first = 1, .second = static_cast<std::uint32_t>(makiSavedCameraContext)};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    mtwkSetRequestedRootMode(scenFindMotionwork(MAKI_RYO), makiSavedRootMode);
    SetMotionFrameTime(scenFindMotionwork(MAKI_RYO), makiSavedBlendFrames, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::RightArm);
    SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::LeftArm);
    EV_SetCharaControllerFlags(MAKI_RYO, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(MAKI_RYO, 3, NpcCharacterControllerState::Enabled);
    SetPlayerControlEnabled(1);
}
const ScriptAreaDefinition makiScriptDefinition{.areaId = MAKI_AREA, .field04 = 0, .init = InitializeMakiEvent, .main = RunMakiEvent};
} // namespace shenmue::s2::areas
