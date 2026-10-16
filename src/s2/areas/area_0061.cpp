#include "s2/areas/area_0061.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include <bit>
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
void E0061MainCallback(void *);
void E0061LoopCallback(void *);
// @verified - p: E0061Init.
void E0061Init() {
}
// @verified - p: E0061Main.
void E0061Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0061MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const work = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0061A.MOT"));
    work->selection = 0;
    work->state = 0;
    SetPlayerControlEnabled(0);
    EV_SetObjectFlagByIndex(15, 0);
    EV_SetObjectFlagByIndex(16, 1);
    EV_FreeAuthorPak(1, "0061");
    HLib::EnqueueTaskWithoutParameter(E0061SpringCallback, 3, 2, static_cast<std::int32_t>(AREA_0061_SPRING.name));
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0061_CHARACTERS, work->self);
}
// @verified - p: SetE0061Characters.
void SetE0061Characters(bool restore) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, restore);
    EV_SetCharacterAlphaEnabled(AREA_0061_A014, restore);
    EV_SetCharacterAlphaEnabled(AREA_0061_A127, restore);
    for (const shen_name *character = AREA_0061_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(*character, restore);
    }
}
// @verified - p: CleanupE0061.
void CleanupE0061(AreaAuthorTaskParameter *work) {
    for (const shen_name *character = AREA_0061_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0061");
    DeleteMotion(std::bit_cast<std::int32_t>(work->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(work->self);
    EV_scenQueueLoad(g_actNumber, AREA_0061_AR02, 25);
}
// @verified - p: QueueE0061Loop.
void QueueE0061Loop(AreaAuthorTaskParameter *work) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0061LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = work;
    if (work->selection != 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetE0061Characters(false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->selection));
    work->state = 2;
}
// @verified - p: E0061MainCallback.
void E0061MainCallback(void *parameter) {
    AreaAuthorTaskParameter *const work = static_cast<AreaAuthorTaskParameter *>(parameter);
    if (work->state == 1)
        QueueE0061Loop(work);
    else if (work->state == 3)
        CleanupE0061(work);
    else if (work->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            work->state = 1;
        else if (input == 64) {
            if (--work->selection < 0)
                work->selection = 1;
        } else if (input == 128 && ++work->selection > 1)
            work->selection = 0;
        if (work->selection == 0 || work->selection == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(work->selection == 0 ? "0061A000.c\n" : "0061A005.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", work->selection);
    }
}
// @verified - p: E0061LoopCallback.
void E0061LoopCallback(void *parameter) {
    AreaAuthorTaskParameter *const work = static_cast<AreaAuthorLoopTaskParameter *>(parameter)->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        SetE0061Characters(true);
        work->state = 0;
        HLib::DestroyCurrentTask();
    }
}
} // namespace
const ScriptAreaDefinition area0061ScriptDefinition{AREA_0061_AREA, 0, E0061Init, E0061Main};
// @verified - p: E0061SpringCallback.
void E0061SpringCallback(void *) {
    SetTextureAnimationUV(0.0f, 0.055f);
    ApplyTextureAnimationResource(20);
    SetTextureAnimationUV(0.0f, 0.027000001f);
    ApplyTextureAnimationResource(21);
}
} // namespace shenmue::s2::areas
