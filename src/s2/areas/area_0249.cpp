#include "s2/areas/area_0249.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
void E0249MainCallback(void *);
void E0249LoopCallback(void *);
// @verified - p
void CleanupE0249(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0249_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_FreeAuthorPak(0, "0249");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void E0249Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0249MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0249A.MOT"));
    parent->selection = static_cast<std::int16_t>(g_entryPoint);
    parent->state = 1;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0249_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0249");
    if (parent->selection > 2)
        CleanupE0249(parent);
}
// @verified - p
void SetupE0249Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0249_A297, false);
    EV_SetCharacterAlphaEnabled(AREA_0249_A298, false);
    if (selection == 0) {
        EV_SetCharacterAlphaEnabled(AREA_0249_A352, false);
        EV_SetCharacterAlphaEnabled(AREA_0249_TENT, false);
    } else if (selection == 1) {
        EV_SetCharacterAlphaEnabled(AREA_0249_A401, false);
        EV_DisableCharacterDisplay(AREA_0249_TENT);
        EV_DisableCharacterDisplay(AREA_0249_A352);
    } else {
        EV_SetCharacterAlphaEnabled(AREA_0249_A205, false);
        EV_DisableCharacterDisplay(AREA_0249_TENT);
        EV_DisableCharacterDisplay(AREA_0249_A352);
        EV_DisableCharacterDisplay(AREA_0249_A401);
    }
    LoadBlurModelForCharacter(AREA_AUTHOR_RYO, "RYO_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_0249_SYE, "SYE_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_0249_TGY, "TGY_B.MT7", 1.0f);
    for (const shen_name *character = AREA_0249_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0249Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0249_A205, true);
    EV_SetCharacterAlphaEnabled(AREA_0249_A352, true);
    EV_SetCharacterAlphaEnabled(AREA_0249_A297, true);
    EV_SetCharacterAlphaEnabled(AREA_0249_A298, true);
    EV_SetCharacterAlphaEnabled(AREA_0249_A401, true);
    for (const shen_name *character = AREA_0249_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
// @verified - p
void QueueE0249Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0249LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_LoadAudio("0249A000", 0);
    SetupE0249Characters(parent->selection);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void E0249MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0249Loop(parent);
    else if (parent->state == 3)
        CleanupE0249(parent);
}
// @verified - p
void E0249LoopCallback(void *taskData) {
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(taskData);
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->parent->selection == 1) {
        if (child->currentFrame == 0) {
            EV_EnableCharacterDisplay(AREA_0249_TENT);
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, 48, AREA_AUTHOR_TASK);
            RegisterEventTask(task);
            EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
            dynamics->resource = "TJMP";
            dynamics->parts = AREA_0249_PARTS;
            dynamics->character = AREA_0249_DYNAMICS_CHARACTER;
            dynamics->endFrame = 135.0f;
            dynamics->trackCount = 42;
            dynamics->frameStep = 2.0f;
            dynamics->currentFrame = 2.0f;
            dynamics->delay = 175.0f;
        } else if (child->currentFrame == 140)
            EV_DisableCharacterDisplay(AREA_0249_TENT);
    } else if (child->parent->selection == 0 && child->currentFrame == 22) {
        AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0249_TENT), 1, 1, 1.0f, 0);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0249Characters();
    child->parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0249ScriptDefinition{.areaId = AREA_0249_AREA, .field04 = 0, .init = nullptr, .main = E0249Main};
} // namespace shenmue::s2::areas
