#include "s2/areas/area_wonc.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
const WoncAuthorDescriptor woncAuthors[]{{WONC_ALL_CHARACTERS, WONC_AUT1, 1, "0012", "0012E011", "WONCHSE1", 0, 16384},
    {WONC_SECOND_CHARACTERS, WONC_AUT2, 1, "0012", "0012E011", "WONCHSE1", 0, 0}, {WONC_SECOND_CHARACTERS, WONC_AUT3, 1, "0012", "0012E011", "WONCHSE1", 0, 0},
    {WONC_LAST_CHARACTERS, WONC_AUT4, -1, "0013", "0013A000", "WONCHSE2", 0, 0}};
const WoncAuthorDescriptor *woncCurrentAuthor{};
std::int32_t woncPauseFrames{};
Vector3i woncPlayerRotation{};
// @verified - p
void WoncInit() {
    woncCurrentAuthor = nullptr;
    woncPauseFrames = 0;
    woncPlayerRotation = {};
}
// @verified - p
void WoncMain(std::uintptr_t argument) {
    woncPauseFrames = 32;
    woncCurrentAuthor = woncAuthors + static_cast<std::int32_t>(argument);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    (void)mtwkRequestMotion(scenFindMotionwork(WONC_PLAYER), 0xf003, MotionRegion::LowerBody);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(WoncPauseCallback, 6, 0x0b, static_cast<std::int32_t>(WONC_TASK.name));
    (void)Queue_EVT_UTL_LoadNPCChara_3(woncCurrentAuthor->characters, task);
}
// @verified - p
void WoncPauseCallback(void *) {
    if (--woncPauseFrames <= 0 && g_usingCustomAuthorCharacterList) {
        (void)ProcessAicaCommand(0x1200a0, 0);
        FinishLoading();
        SetHelpIconState();
        HLib::SetCurrentTaskTo(WoncPlayAuthorCallback);
    }
}
// @verified - p
void WoncPlayAuthorCallback(void *) {
    PrepareWoncCharacters();
    (void)EV_FreeAuthorPak(1, woncCurrentAuthor->authorBank);
    (void)EV_SetNewSceneIndex();
    (void)EV_LoadCutsceneFromCurrentScene(woncCurrentAuthor->soundProgram);
    if (woncCurrentAuthor->audioScene > 0) {
        (void)EV_LoadAudio(woncCurrentAuthor->soundProgram, woncCurrentAuthor->audioScene);
        (void)EV_spmBeginSampleExecute();
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
    EV_SetAuthorActiveTo(1);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(woncCurrentAuthor->author.name));
    (void)EV_NPC_NewLoadSw(woncCurrentAuthor->npcLoad != 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(WoncWaitAuthorCallback);
}
// @verified - p
void PrepareWoncCharacters() {
    constexpr Vector4 reflector{1.0F, 0.3F, 0.3F, 0.3F};
    EV_EnableCharacterDisplay(WONC_PLAYER);
    EV_SetCharaControllerFlags(WONC_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(WONC_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(WONC_PLAYER, false);
    EV_SetRefBoardParamsFor(WONC_PLAYER, 1);
    (void)SetScenarioReflectorBoardValues(WONC_PLAYER, &reflector);
    (void)SetScenarioCharacterFlag40(WONC_PLAYER, 0);
    for (const shen_name *character = woncCurrentAuthor->characters; character->name != WONC_INVALID.name; ++character) {
        (void)SetScenarioReflectorBoardValues(*character, &reflector);
        (void)SetScenarioCharacterFlag40(*character, 0);
    }
    EV_DisableCharacterDisplay(WONC_HIDDEN);
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, WONC_TEMPORARY);
}
// @verified - p
void WoncWaitAuthorCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        WoncCompleteAuthorCallback(nullptr);
        return;
    }
    if (((woncCurrentAuthor->author.name - WONC_AUT2.name) & 0xfeffffffU) != 0)
        return;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 1: {
        EV_DisableCharacterDisplay(WONC_WONG);
        DateTime date;
        GetCurrentDateTime(date);
        if (date.hour < 19)
            return;
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_PLAYER), 0, 0, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_PLAYER), 0, 1, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_WONG), 0, 0, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_WONG), 0, 1, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_SAM), 0, 0, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_SAM), 0, 1, 0);
        break;
    }
    case 75:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_LILLY), 0, 0, 0);
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_LILLY), 0, 1, 0);
        break;
    case 190:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_SAM), 0, 0, 0);
        break;
    case 211:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_PLAYER), 0, 1, 0);
        EV_DisableCharacterDisplay(WONC_LILLY);
        break;
    case 215:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_SAM), 0, 1, 0);
        break;
    case 225:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_PLAYER), 0, 0, 0);
        break;
    case 310:
        EV_EnableCharacterDisplay(WONC_WONG);
        break;
    case 636:
        EV_EnableCharacterDisplay(WONC_LILLY);
        break;
    case 674:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_WONG), 0, 0, 0);
        break;
    case 688:
        (void)SetHighModelXYZ(FindCharacterByScenarioName(WONC_WONG), 0, 1, 0);
        break;
    default:
        break;
    }
}
// @verified - p
void WoncDormantNullsub() {
}
// @verified - p
void WoncCompleteAuthorCallback(void *) {
    using enum CameraState;
    WoncDormantNullsub();
    (void)EV_ResetFreeTalkFlags();
    (void)EV_NPC_NewLoadSw(0);
    SetScenarioFollowCharacter(WONC_WONG);
    RestoreWoncCharacters();
    (void)camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(Init);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(woncCurrentAuthor->package), 0);
    (void)EV_FreeAuthorPak(0, woncCurrentAuthor->authorBank);
    (void)EV_ForceLoadNpc(WONC_WONG);
    HLib::SetCurrentTaskTo(WoncEndCallback);
}
// @verified - p
void RestoreWoncCharacters() {
    for (const shen_name *character = WONC_ALL_CHARACTERS; character->name != WONC_INVALID.name; ++character) {
        if (DoesScenarioCharacterExist(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    woncPlayerRotation.y = woncCurrentAuthor->playerYaw;
    (void)EV_AT_SetAngY(WONC_PLAYER, EventTransformFlags::AllAxes, woncPlayerRotation, nullptr);
    (void)mtwkRequestMotion(scenFindMotionwork(WONC_PLAYER), 0xf003, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(WONC_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(WONC_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(WONC_PLAYER, true);
    EV_SetRefBoardParamsFor(WONC_PLAYER, 0);
    (void)SetScenarioCharacterFlag40(WONC_PLAYER, 1);
    EV_EnableCharacterDisplay(WONC_HIDDEN);
    SetScenarioCharacterKillMode(WONC_TEMPORARY, 1);
}
// @verified - p
void WoncEndCallback(void *) {
    if (EV_ForceLoadNpc(WONC_WONG)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 0);
        (void)LoadCutsceneAndSoundProgramForArea();
        (void)EV_spmBeginSampleExecute();
        (void)EV_NPC_NewLoadSw(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(WoncEndFadeCallback);
    }
}
// @verified - p
void WoncEndFadeCallback(void *) {
    if (!filmCheckFadeTrackColour()) {
        FinishLoading();
        SetPlayerControlEnabled(1);
        (void)HLib::DestroyCurrentTask();
    }
}
const ScriptAreaDefinition woncScriptDefinition{.areaId = AREA_WONC, .field04 = 0, .init = WoncInit, .main = WoncMain};
} // namespace shenmue::s2
