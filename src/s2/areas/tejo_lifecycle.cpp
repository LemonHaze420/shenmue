#include "s2/areas/area_tejo.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {
// @verified - p
void PrepareTejoQteResources() {
    using enum CameraState;
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    tejoQteWork.lives = 4;
    tejoQteIndex = 0;
    SetAllMapFieldDepthSlotsEnabled(1);
    const bool firstQte = tejoProgression.name == TEJO_FIRST_QTE.name;
    SetQronFloor(firstQte ? 5 : 4);
    EventSetPosition position{0, REN_QTE_PLAYER, REN_QTE_INVALID, firstQte ? TEJO_FIRST_QTE_POSITION : TEJO_SECOND_QTE_POSITION, 0};
    EVT_AT_SetPosRot(&position);
    EV_FreeAuthorPak(1, "0209");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("TEJO"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TEJO"), 0);
    EV_LoadCutsceneFromCurrentScene("0209A000");
    EV_LoadAudio("0209A000", 1);
    Loop3DSoundEffect(0x8eb);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, REN_QTE_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, REN_QTE_SECOND_BASE);
    AT_SetLocalBaseChara(REN_QTE_SECOND_BASE, REN_QTE_PLAYER);
    EV_AT_SetPos(REN_QTE_SECOND_BASE, EventTransformFlags::AllAxes, TEJO_SECOND_BASE_POSITION);
    EV_AT_SetAngY(REN_QTE_SECOND_BASE, EventTransformFlags::AllAxes, TEJO_SECOND_BASE_ROTATION);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_PLAYER), 0);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_REN), 0);
    SetQronMapLightingEnabled(0);
    Vector3 doorPosition;
    EV_GetCharaPosition(REN_QTE_DOOR, CHARACTER_TRANSLATION_ROOT, &doorPosition, EventTransformFlags::World);
    Vector3 lightDoorPosition = doorPosition;
    lightDoorPosition.y = 100;
    EV_AT_SetPos(REN_QTE_DOOR, EventTransformFlags::World | EventTransformFlags::AllAxes, lightDoorPosition);
    EV_AT_SetPos(REN_QTE_BROKEN_DOOR, EventTransformFlags::World | EventTransformFlags::AllAxes, lightDoorPosition);
    EV_lightLoad(firstQte ? TEJO_LIGHT_FIRST_QTE : TEJO_LIGHT_SECOND_QTE);
    PushGlobalLightingProgram();
    EV_AT_SetPos(REN_QTE_DOOR, EventTransformFlags::World | EventTransformFlags::AllAxes, doorPosition);
    EV_AT_SetPos(REN_QTE_BROKEN_DOOR, EventTransformFlags::World | EventTransformFlags::AllAxes, doorPosition);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    EV_PlayDialogue(firstQte ? "0209B046" : "0209B047");
    Loop3DSoundEffect(0x8eb);
    if (GetItemState(6001))
        SetNaviDisplayOverride(true);
}
// @verified - p
void BeginTejoEndAuthor() {
    AT_SetLocalBaseChara(REN_QTE_PLAYER, REN_QTE_INVALID);
    AT_SetLocalBaseChara(REN_QTE_REN, REN_QTE_INVALID);
    shen_name light, author;
    if (tejoProgression.name == TEJO_FORCE.name) {
        SetQronFloor(4);
        light = TEJO_LIGHT_FIRST_END;
        author = TEJO_FIRST_END_AUTHOR;
    } else {
        Vector3 position{};
        EV_scenApplyPosOrRotToPartOf(REN_QTE_BROKEN_DOOR, 0xa1, &position, nullptr, EventCharacterPartTransformMode::Set);
        light = TEJO_LIGHT_SECOND_END;
        author = TEJO_SECOND_END_AUTHOR;
    }
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(light);
    PushGlobalLightingProgram();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
    SetQronMapLightingEnabled(0);
    EV_LoadAudio("0209A000", 0);
    SetNaviDisplayOverride(false);
    HLib::SetCurrentTaskTo(TejoEndAuthorCallback);
}
// @verified - p
void HideTejoPursuers() {
    tejoQteWork.hidePursuers &= 0xffffffff00000000ULL;
    AT_SetLocalBaseChara(REN_QTE_KD7, REN_QTE_BASE);
    AT_SetLocalBaseChara(REN_QTE_KK1, REN_QTE_BASE);
    AT_SetLocalBaseChara(REN_QTE_KT1, REN_QTE_BASE);
    AT_SetLocalBaseChara(REN_QTE_KU7, REN_QTE_BASE);
    EV_DisableCharacterDisplay(REN_QTE_KD7);
    EV_DisableCharacterDisplay(REN_QTE_KK1);
    EV_DisableCharacterDisplay(REN_QTE_KT1);
    EV_DisableCharacterDisplay(REN_QTE_KU7);
    EV_ToggleCharacterFlag(REN_QTE_KD7);
    EV_ToggleCharacterFlag(REN_QTE_KK1);
    EV_ToggleCharacterFlag(REN_QTE_KT1);
    EV_ToggleCharacterFlag(REN_QTE_KU7);
    Loop3DSoundEffect(0x8ed);
    SetScreenFilmFade(90, 255, 0, 0, 0, 0, 0, 0, 0);
}
// @verified - p
void CleanupTejoQte() {
    using enum CameraState;
    tejoQteWork.hidePursuers = (tejoQteWork.hidePursuers & 0xffffffff00000000ULL) | 1;
    tejoQteWork.state = REN_QTE_INIT;
    SetNaviDisplayOverride(false);
    EV_FreeAuthorPak(0, "0209");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TEJO"), 0);
    for (const shen_name *character = TEJO_QTE_CHARACTERS; character->name != REN_QTE_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        EV_npcLockUnlock(*character);
        EV_npcFree(*character);
    }
    AT_SetLocalBaseChara(REN_QTE_PLAYER, REN_QTE_INVALID);
    AT_SetLocalBaseChara(REN_QTE_REN, REN_QTE_INVALID);
    mtwkRequestMotion(scenFindMotionwork(REN_QTE_PLAYER), 0xf003, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(REN_QTE_REN), 0xf003, MotionRegion::LowerBody);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    if (DoesScenarioCharacterExist(REN_QTE_BASE))
        SetScenarioCharacterKillMode(REN_QTE_BASE, 1);
    if (DoesScenarioCharacterExist(REN_QTE_SECOND_BASE))
        SetScenarioCharacterKillMode(REN_QTE_SECOND_BASE, 1);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_PLAYER), 1);
    SetPluralControlFor(ctrlGetCharaByName(REN_QTE_REN), 1);
    HLib::ResolveDeferredCallback(tejoMainTask);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(tejoQteTask);
    tejoQteTask = nullptr;
    camCtrlToggleSmoothing(0);
    camCtrlResetMotionscaling();
}
// @verified - p
void PrepareTejoEvent() {
    SelectPostFxBank(1, 1);
    UploadGlowParams(TEJO_GLOW);
    SelectPostFxBank(3, 1);
    UploadBlurStrength(0.9F);
    SetPlayerControlEnabled(0);
    RenQteUnusedResult(&tejoQteWork);
    EV_SetRefBoardParamsFor(REN_QTE_PLAYER, 1);
    EV_SetCharaControllerFlags(REN_QTE_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(REN_QTE_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(REN_QTE_PLAYER, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(REN_QTE_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(REN_QTE_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    mtwkRequestMotion(scenFindMotionwork(REN_QTE_PLAYER), 0xf003, MotionRegion::LowerBody);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Queue);
    tejoProgression = TEJO_TRAP;
    tejoWaitFlag = 0;
    tejoRetryMode = 0;
    tejoExtendedRetry = 0;
    tejoMainTask = nullptr;
    tejoQteTask = nullptr;
    tejoQteWork.base = REN_QTE_BASE;
    tejoQteWork.state = REN_QTE_INIT;
    tejoQteWork.retryCount = 0;
    tejoQteWork.lives = 0;
    tejoQteWork.doorAttempts = 0;
    tejoQteWork.entry = nullptr;
    tejoQteWork.hidePursuers = (tejoQteWork.hidePursuers & 0xffffffff00000000ULL) | 1;
    SetQronMapLightingEnabled(0);
}
// @verified - p
void CleanupTejoEvent() {
    using enum CameraState;
    SelectPostFxBank(1, 0);
    SelectPostFxBank(3, 0);
    UploadBlurStrength(1);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    EV_cuffsSetFlags();
    for (const shen_name *character = TEJO_MAIN_CHARACTERS; character->name != REN_QTE_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        EV_npcLockUnlock(*character);
        EV_npcFree(*character);
    }
    EV_SetRefBoardParamsFor(REN_QTE_PLAYER, 0);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    SetPlayerControlEnabled(1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(tejoMainTask);
    SetConversationCameraCodesEnabled(1);
}
} // namespace shenmue::s2
