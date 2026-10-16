#include "s2/areas/area_topa.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2 {

std::int32_t topaMode{};

// @verified - p
void TopaInit() {
    topaMode = 0;
}

// @verified - p
void TopaMain(std::uintptr_t argument) {
    topaMode = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    (void)mtwkRequestMotion(scenFindMotionwork(TOPA_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(TopaPlayAuthorCallback, 6, 0x0b, static_cast<std::int32_t>(TOPA_TASK.name));
    (void)Queue_EVT_UTL_LoadNPCChara_3(TOPA_CHARACTERS, task);
}

// @verified - p
void PrepareTopaCharacters() {
    EV_EnableCharacterDisplay(TOPA_RYO);
    EV_SetCharaControllerFlags(TOPA_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(TOPA_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(TOPA_RYO, false);
    EV_SetRefBoardParamsFor(TOPA_RYO, 1);
    (void)SetScenarioReflectorBoardValues(TOPA_RYO, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_REN, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_CHR, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_SOY, &TOPA_REFLECTOR);
    (void)SetScenarioCharacterFlag40(TOPA_RYO, 0);
    (void)SetScenarioCharacterFlag40(TOPA_REN, 0);
    (void)SetDoorSubjectEnabled(TOPA_DOOR, 0);
}

// @verified - p
void TopaPlayAuthorCallback(void *) {
    PrepareTopaCharacters();
    (void)EV_FreeAuthorPak(1, "0213");
    (void)EV_LoadCutsceneFromCurrentScene("0213A000");
    (void)EV_LoadAudio("0213A000", 0);
    if (topaMode != 0) {
        HLib::SetCurrentTaskTo(TopaPlaySecondAuthorCallback);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        HLib::SetupTaskDelay(90);
    } else {
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOPA_AUT1.name));
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(TopaWaitAuthorCallback);
    }
    (void)SetQronMapLightingEnabled(0);
    std::uint32_t mapSlot = 0;
    while (HasMapModelReader(static_cast<std::int32_t>(mapSlot)))
        ++mapSlot;
    AssignLightProgram(mapSlot - 1U);
}

// @verified - p
void TopaWaitAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    EndTopaEvent();
}

// @verified - p
void TopaPlaySecondAuthorCallback(void *) {
    ST_LoadTitleFade();
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOPA_AUT2.name));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(TopaWaitAuthorCallback);
}

// @verified - p
void EndTopaEvent() {
    for (const shen_name *character = TOPA_CHARACTERS + 1; character->name != TOPA_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        (void)EV_npcLockUnlock(*character);
        (void)EV_npcFree(*character);
    }
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOPA"), 0);
    (void)EV_FreeAuthorPak(0, "0213");
    EV_stepSetFlags(0, 0x272);
    (void)HLib::DestroyCurrentTask();
}

const ScriptAreaDefinition topaScriptDefinition{.areaId = AREA_TOPA, .field04 = 0, .init = TopaInit, .main = TopaMain};

} // namespace shenmue::s2
