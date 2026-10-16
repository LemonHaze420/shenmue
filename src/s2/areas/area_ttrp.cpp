#include "s2/areas/area_ttrp.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2 {

// @verified - p
void TtrpInit() {
}

// @verified - p
void TtrpDeveloperNullsub() {
}

// @verified - p
void TtrpMain(std::uintptr_t) {
    TtrpDeveloperNullsub();
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    (void)mtwkRequestMotion(scenFindMotionwork(TTRP_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(TtrpPlayAuthorCallback, 6, 0x0b, static_cast<std::int32_t>(TTRP_TASK.name));
    (void)Queue_EVT_UTL_LoadNPCChara_3(TTRP_CHARACTERS, task);
}

// @verified - p
void PrepareTtrpCharacters() {
    EV_EnableCharacterDisplay(TTRP_RYO);
    EV_SetCharaControllerFlags(TTRP_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(TTRP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(TTRP_RYO, false);
    EV_SetRefBoardParamsFor(TTRP_RYO, 1);
    (void)SetScenarioReflectorBoardValues(TTRP_RYO, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_REN, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_CHR, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_SCX, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_TGY, &TTRP_REFLECTOR);
    (void)SetScenarioCharacterFlag40(TTRP_RYO, 0);
    (void)SetScenarioCharacterFlag40(TTRP_REN, 0);
}

// @verified - p
void TtrpPlayAuthorCallback(void *) {
    PrepareTtrpCharacters();
    (void)EV_FreeAuthorPak(1, "0207");
    (void)EV_FreeAuthorPak(1, "0208");
    (void)EV_LoadCutsceneFromCurrentScene("0207A000");
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(TTRP_LIGHT);
    PushGlobalLightingProgram();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TTRP_AUT1.name));
    HLib::SetCurrentTaskTo(TtrpWaitFirstAuthorCallback);
}

// @verified - p
void TtrpWaitFirstAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_DisableCharacterDisplay(TTRP_A202);
    EV_DisableCharacterDisplay(TTRP_A203);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TTRP_AUT2.name));
    HLib::SetCurrentTaskTo(TtrpWaitSecondAuthorCallback);
}

// @verified - p
void TtrpWaitSecondAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetCurrentTaskTo(TtrpEndCallback);
}

// @verified - p
void TtrpEndCallback(void *) {
    for (const shen_name *character = TTRP_CHARACTERS + 1; character->name != TTRP_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        (void)EV_npcLockUnlock(*character);
        (void)EV_npcFree(*character);
    }
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TTRP"), 0);
    (void)EV_FreeAuthorPak(0, "0207");
    (void)EV_FreeAuthorPak(0, "0208");
    EV_stepSetFlags(0, 0x137);
    (void)HLib::DestroyCurrentTask();
}

const ScriptAreaDefinition ttrpScriptDefinition{.areaId = AREA_TTRP, .field04 = 0, .init = TtrpInit, .main = TtrpMain};

} // namespace shenmue::s2
