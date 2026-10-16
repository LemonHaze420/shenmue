#include "s2/areas/gabt_authored.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
// @verified - p
void GabtPrepareAuthorCharacter(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 0, 8, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 1, 8, 16);
}
// @verified - p
void GabtPrepareAuthorCharacters() {
    GabtPrepareAuthorCharacter(GABT_AUTHOR_RYO);
    for (shen_name scenery : GABT_AUTHOR_SCENERY) {
        EV_EnableCharacterDisplay(scenery);
        EV_SetCharaControllerFlags(scenery, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(scenery, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(scenery, false);
    }
    SetScenarioCharacterDrawControl(GABT_AUTHOR_RYO, 1, 0);
    GabtPrepareAuthorCharacter(GABT_AUTHOR_GAI);
}
// @verified - p
void GabtRestoreAuthorCharacters() {
    EV_EnableCharacterDisplay(GABT_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(GABT_AUTHOR_RYO), 6, 0);
    for (shen_name scenery : GABT_AUTHOR_SCENERY)
        EV_EnableCharacterDisplay(scenery);
    EV_EnableCharacterDisplay(GABT_AUTHOR_GAI);
    mtwkSetExtraFlag(scenFindMotionwork(GABT_AUTHOR_GAI), 6, 0);
}
// @verified - p
void GabtPrepareAuthorLighting(GabtAuthorWork *work) {
    SetPlayerControlEnabled(0);
    SetWeatherUpdateEnabledTo(0);
    EV_SetModelDataDefaultVertexShader();
    if (work->author.name == GABT_AUTHOR_INTRO.name || work->author.name == GABT_AUTHOR_PRACTICE.name) {
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
        EV_lightLoad(GABT_AUTHOR_LIGHT_INTRO);
    } else if (work->author.name == GABT_AUTHOR_RESULT.name) {
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
        EV_lightLoad(GABT_AUTHOR_LIGHT_RESULT);
    }
    PushGlobalLightingProgram();
}
// @verified - p
void GabtAuthorMainCallback(void *data) {
    GabtAuthorWork &work = *static_cast<GabtAuthorWork *>(data);
    if (work.state == 1) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(GabtAuthorLoopCallback, 4, 0x0b, 0x10, GABT_AUTHOR_TASK);
        GabtAuthorLoopWork &loop = *static_cast<GabtAuthorLoopWork *>(HLib::GetTaskParameter(task));
        loop.frame = 0;
        loop.owner = &work;
        EV_LoadAudio("0232A000", 0);
        GabtPrepareAuthorCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
        work.state = 2;
    } else if (work.state == 3)
        HLib::DestroyTask(work.task);
}
// @verified - p
void GabtAuthorLoopCallback(void *data) {
    GabtAuthorLoopWork &loop = *static_cast<GabtAuthorLoopWork *>(data);
    GabtAuthorWork &work = *loop.owner;
    loop.frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.author.name == GABT_AUTHOR_INTRO.name) {
        if (loop.frame == 1)
            snapSetDesc(1);
        else if (loop.frame == 590)
            snapSetDesc(0);
    }
    if (loop.frame == 1 && work.author.name == GABT_AUTHOR_PRACTICE.name)
        SetScreenFilmFade(5, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work.author.name == GABT_AUTHOR_PRACTICE.name && loop.frame == 1355)
        SetIsSkySetup(0);
    if (work.author.name == GABT_AUTHOR_RESULT.name) {
        if (loop.frame == 5520)
            AddInventoryItem(0x1f40, 1, 1);
    } else if (work.author.name == GABT_AUTHOR_PRACTICE.name) {
        if (loop.frame == 1300)
            for (std::size_t i = 0; i < 4; ++i)
                EV_DisableCharacterDisplay(GABT_AUTHOR_SCENERY[i]);
    } else if (work.author.name == GABT_AUTHOR_INTRO.name) {
        if (loop.frame == 1)
            SetScenarioCharacterDrawControl(GABT_AUTHOR_RYO, 0, 0);
        else if (loop.frame == 1100) {
            EV_SetRefBoardParamsFor(GABT_AUTHOR_SCENERY[3], 1);
            SetScenarioReflectorBoardValues(GABT_AUTHOR_SCENERY[3], &GABT_AUTHOR_REFLECTOR);
            SetScenarioCharacterDrawControl(GABT_AUTHOR_RYO, 1, 0);
        }
    }
    if (work.author.name == GABT_AUTHOR_PRACTICE.name && loop.frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (work.author.name == GABT_AUTHOR_INTRO.name && loop.frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 15)
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (work.author.name == GABT_AUTHOR_PRACTICE.name)
            SetIsSkySetup(1);
        work.state = 3;
        GabtRestoreAuthorCharacters();
        HLib::DestroyCurrentTask();
    }
}
} // namespace shenmue::s2::areas
