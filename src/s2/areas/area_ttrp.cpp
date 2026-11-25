#include "s2/areas/area_ttrp.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void TtrpMain(unsigned int argument) {
    HLibTask *task;
    FTBL.EventSystemNullsub();
#else
void TtrpMain(std::uintptr_t) {
    TtrpDeveloperNullsub();
#endif
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TTRP_RYO), 0xf003, 0);
    task = FTBL.HLib_EnqueueTaskWithoutParameter(TtrpPlayAuthorCallback, 6, 0x0b, TTRP_TASK);
#else
    (void)mtwkRequestMotion(scenFindMotionwork(TTRP_RYO), 0xf003, MotionRegion::LowerBody);
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithoutParameter(TtrpPlayAuthorCallback, 6, 0x0b, static_cast<std::int32_t>(TTRP_TASK.name));
#endif
    (void)Queue_EVT_UTL_LoadNPCChara_3(TTRP_CHARACTERS, task);
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void TtrpPlayAuthorCallback(void *taskData) {
    PrepareTtrpCharacters();
    (void)FTBL.EV_FreeAuthorPak(1, "0207");
    (void)FTBL.EV_FreeAuthorPak(1, "0208");
    (void)FTBL.EV_LoadCutsceneFromCurrentScene("0207A000");
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(TTRP_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(TTRP_AUT1);
#else
    (void)FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(TTRP_AUT1.name));
#endif
    FTBL.HLib_SetCurrentTaskTo(TtrpWaitFirstAuthorCallback);
}

// @verified - pd
void TtrpWaitFirstAuthorCallback(void *taskData) {
    if (FTBL.EV_authSceneCtrl(-1))
        return;
    FTBL.EV_DisableCharacterDisplay(TTRP_A202);
    FTBL.EV_DisableCharacterDisplay(TTRP_A203);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(TTRP_AUT2);
#else
    (void)FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(TTRP_AUT2.name));
#endif
    FTBL.HLib_SetCurrentTaskTo(TtrpWaitSecondAuthorCallback);
}

// @verified - pd
void TtrpWaitSecondAuthorCallback(void *taskData) {
    if (FTBL.EV_authSceneCtrl(-1))
        return;
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(TtrpEndCallback);
}

// @verified - pd
void TtrpEndCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FreeTtrpCharacters();
    FTBL.EV_ControlPackageResources(0, 6, "TTRP", 0, 0);
#else
    for (const shen_name *character = TTRP_CHARACTERS + 1; character->name != TTRP_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        (void)EV_npcLockUnlock(*character);
        (void)EV_npcFree(*character);
    }
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TTRP"), 0);
#endif
    (void)FTBL.EV_FreeAuthorPak(0, "0207");
    (void)FTBL.EV_FreeAuthorPak(0, "0208");
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_stepSetFlags(0x137, 0);
#else
    EV_stepSetFlags(0, 0x137);
#endif
    (void)FTBL.HLib_DestroyCurrentTask();
}

// @verified - pd
void PrepareTtrpCharacters() {
#if defined(SHENMUE_EVENT_OVERLAY)
    const float values[4] = {1.0f, 0.3f, 0.3f, 0.3f};
#endif
    FTBL.EV_EnableCharacterDisplay(TTRP_RYO);
#if defined(SHENMUE_EVENT_OVERLAY)
    do {
        FTBL.EV_SetCharaControllerFlags(TTRP_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(TTRP_RYO, 3, 0);
    } while (0);
#else
    EV_SetCharaControllerFlags(TTRP_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(TTRP_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(TTRP_RYO, 0);
    FTBL.SetRefBoardParamsForCharaById(TTRP_RYO, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetScenarioReflectorBoardValues(TTRP_RYO, values);
    FTBL.SetScenarioReflectorBoardValues(TTRP_REN, values);
    FTBL.SetScenarioReflectorBoardValues(TTRP_CHR, values);
    FTBL.SetScenarioReflectorBoardValues(TTRP_SCX, values);
    FTBL.SetScenarioReflectorBoardValues(TTRP_TGY, values);
#else
    (void)SetScenarioReflectorBoardValues(TTRP_RYO, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_REN, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_CHR, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_SCX, &TTRP_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TTRP_TGY, &TTRP_REFLECTOR);
#endif
    (void)FTBL.SetScenarioCharacterFlag40(TTRP_RYO, 0);
    (void)FTBL.SetScenarioCharacterFlag40(TTRP_REN, 0);
}

#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - d
void FreeTtrpCharacters(void) {
    EVT_UTL_FreeNPCChara(TTRP_FREE_CHARACTERS);
}
const shen_name TTRP_CHARACTERS[] = {TTRP_REN, TTRP_CHR, TTRP_SCX, TTRP_TGY, TTRP_INVALID};
const shen_name TTRP_FREE_CHARACTERS[] = {TTRP_CHR, TTRP_SCX, TTRP_TGY, TTRP_INVALID};
#else
const ScriptAreaDefinition ttrpScriptDefinition{.areaId = AREA_TTRP, .field04 = 0, .init = TtrpInit, .main = TtrpMain};
} // namespace shenmue::s2
#endif
#endif
