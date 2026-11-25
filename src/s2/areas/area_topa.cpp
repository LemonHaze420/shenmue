#include "s2/areas/area_topa.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void TopaMain(int argument) {
    HLibTask *task;
    topaMode = argument;
#else
void TopaMain(std::uintptr_t argument) {
    topaMode = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
#endif
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(TOPA_RYO), 0xf003, 0);
    task = FTBL.HLib_EnqueueTaskWithoutParameter(TopaPlayAuthorCallback, 6, 0x0b, TOPA_TASK);
#else
    (void)mtwkRequestMotion(scenFindMotionwork(TOPA_RYO), 0xf003, MotionRegion::LowerBody);
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithoutParameter(TopaPlayAuthorCallback, 6, 0x0b, static_cast<std::int32_t>(TOPA_TASK.name));
#endif
    (void)Queue_EVT_UTL_LoadNPCChara_3(TOPA_CHARACTERS, task);
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void TopaPlayAuthorCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    int mapSlot;
#endif
    PrepareTopaCharacters();
    (void)FTBL.EV_FreeAuthorPak(1, "0213");
    (void)FTBL.EV_LoadCutsceneFromCurrentScene("0213A000");
    (void)FTBL.EV_LoadAudio("0213A000", 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (topaMode == 0) {
        FTBL.EV_authSceneCtrl(TOPA_AUT1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(TopaWaitAuthorCallback);
    } else {
        FTBL.HLib_SetCurrentTaskTo(TopaPlaySecondAuthorCallback);
        FTBL.Queue_ST_LoadTitleFadeIn(0);
        FTBL.HLib_SetupTaskDelay(90);
    }
#else
    if (topaMode != 0) {
        FTBL.HLib_SetCurrentTaskTo(TopaPlaySecondAuthorCallback);
        FTBL.Queue_ST_LoadTitleFadeIn(shen_name{});
        FTBL.HLib_SetupTaskDelay(90);
    } else {
        (void)FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(TOPA_AUT1.name));
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(TopaWaitAuthorCallback);
    }
#endif
    (void)FTBL.mapSetupLighting(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    mapSlot = 0;
    while (FTBL.HasMapModelReader(mapSlot))
        ++mapSlot;
    FTBL.AssignLightProgram(mapSlot - 1);
#else
    std::uint32_t mapSlot = 0;
    while (FTBL.HasMapModelReader(static_cast<std::int32_t>(mapSlot)))
        ++mapSlot;
    FTBL.AssignLightProgram(mapSlot - 1U);
#endif
}

// @verified - pd
void TopaPlaySecondAuthorCallback(void *taskData) {
    FTBL.ST_LoadTitleFade();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(TOPA_AUT2);
#else
    (void)FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(TOPA_AUT2.name));
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(TopaWaitAuthorCallback);
}

// @verified - pd
void TopaWaitAuthorCallback(void *taskData) {
    if (FTBL.EV_authSceneCtrl(-1))
        return;
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    EndTopaEvent();
}

// @verified - pd
void EndTopaEvent() {
#if defined(SHENMUE_EVENT_OVERLAY)
    FreeTopaCharacters();
    FTBL.EV_ControlPackageResources(0, 6, "TOPA", 0, 0);
#else
    for (const shen_name *character = TOPA_CHARACTERS + 1; character->name != TOPA_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        (void)EV_npcLockUnlock(*character);
        (void)EV_npcFree(*character);
    }
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOPA"), 0);
#endif
    (void)FTBL.EV_FreeAuthorPak(0, "0213");
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_stepSetFlags(0x272, 0);
#else
    EV_stepSetFlags(0, 0x272);
#endif
    (void)FTBL.HLib_DestroyCurrentTask();
}

// @verified - pd
void PrepareTopaCharacters() {
#if defined(SHENMUE_EVENT_OVERLAY)
    const float values[4] = {1.0f, 0.3f, 0.3f, 0.3f};
#endif
    FTBL.EV_EnableCharacterDisplay(TOPA_RYO);
#if defined(SHENMUE_EVENT_OVERLAY)
    do {
        FTBL.EV_SetCharaControllerFlags(TOPA_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(TOPA_RYO, 3, 0);
    } while (0);
#else
    EV_SetCharaControllerFlags(TOPA_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(TOPA_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(TOPA_RYO, 0);
    FTBL.SetRefBoardParamsForCharaById(TOPA_RYO, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetScenarioReflectorBoardValues(TOPA_RYO, values);
    FTBL.SetScenarioReflectorBoardValues(TOPA_REN, values);
    FTBL.SetScenarioReflectorBoardValues(TOPA_CHR, values);
    FTBL.SetScenarioReflectorBoardValues(TOPA_SOY, values);
#else
    (void)SetScenarioReflectorBoardValues(TOPA_RYO, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_REN, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_CHR, &TOPA_REFLECTOR);
    (void)SetScenarioReflectorBoardValues(TOPA_SOY, &TOPA_REFLECTOR);
#endif
    (void)FTBL.SetScenarioCharacterFlag40(TOPA_RYO, 0);
    (void)FTBL.SetScenarioCharacterFlag40(TOPA_REN, 0);
    (void)FTBL.SetDoorSubjectEnabled(TOPA_DOOR, 0);
}

#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - d
void FreeTopaCharacters(void) {
    EVT_UTL_FreeNPCChara(TOPA_FREE_CHARACTERS);
}
const shen_name TOPA_CHARACTERS[] = {TOPA_REN, TOPA_CHR, TOPA_SOY, TOPA_INVALID};
const shen_name TOPA_FREE_CHARACTERS[] = {TOPA_CHR, TOPA_SOY, TOPA_INVALID};
int topaMode __attribute__((section(".bss")));
#else
const ScriptAreaDefinition topaScriptDefinition{.areaId = AREA_TOPA, .field04 = 0, .init = TopaInit, .main = TopaMain};
} // namespace shenmue::s2
#endif
#endif
