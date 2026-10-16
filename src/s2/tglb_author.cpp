#include "s2/tglb_author.hpp"

#include "s2/author.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {

TglbAuthorContext *QueueTglbAuthorMain(shen_name command) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TglbAuthorMain_Callback, 4, 0x0b, sizeof(TglbAuthorContext), EVENT_CHARACTER_TASK);
    TglbAuthorContext *const context = static_cast<TglbAuthorContext *>(HLib::GetTaskParameter(task));
    context->task = task;
    context->state = 1;
    context->authorCmd = command;
    QueueTglbAuthorLoop(context);
    return context;
}

// @verified - p
void TglbAuthorMain_Callback(void *taskData) {
    TglbAuthorContext *const context = static_cast<TglbAuthorContext *>(taskData);
    if (context->state == 1)
        QueueTglbAuthorLoop(context);
    else if (context->state == 3)
        HLib::DestroyTask(context->task);
}

// @verified - p
std::int64_t QueueTglbAuthorLoop(TglbAuthorContext *context) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TglbAuthorLoop_Callback, 4, 0x0b, sizeof(TglbAuthorLoopWork), EVENT_CHARACTER_TASK);
    TglbAuthorLoopWork *const work = static_cast<TglbAuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->context = context;
    switch (context->authorCmd.name) {
    case TGLB_AUTHOR_AU_0.name:
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case TGLB_AUTHOR_AU_7.name:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case TGLB_AUTHOR_AU_8.name:
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        break;
    default:
        break;
    }
    SetMapModelCommandRoutingEnabled(0, 0);
    SetMapModelCommandRoutingEnabled(0, 1);
    SetMapModelCommandRoutingEnabled(0, 2);
    SetMapModelCommandRoutingEnabled(0, 3);
    SetupTglbAuthorCharacters(context);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(context->authorCmd.name));
    context->state = 2;
    return result;
}

// @verified - p
void TglbAuthorLoop_Callback(void *taskData) {
    TglbAuthorLoopWork *const work = static_cast<TglbAuthorLoopWork *>(taskData);
    TglbAuthorContext *const context = work->context;
    const std::uint32_t frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (context->authorCmd.name == TGLB_AUTHOR_AU_1.name) {
        if (frame == 350)
            QueueExtraTelop("AAAAAAAA", static_cast<std::int32_t>(0xc0000000U));
        if (frame == static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 15U) {
            spmFadeOut();
            SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        }
    }
    if (context->authorCmd.name == TGLB_AUTHOR_AU_2.name && frame == static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 30U)
        spmFadeOut();
    if (context->authorCmd.name == TGLB_AUTHOR_AU_8.name) {
        if (frame == 242) {
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TGLB"), 0);
            EV_lightLoad(TGLB_AUTHOR_LIGHT);
        } else if (frame == 510) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        }
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        context->state = 3;
        RestoreTglbAuthorCharacters(context);
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
void SetupTglbAuthorCharacters(TglbAuthorContext *context) {
    EV_EnableCharacterDisplay(TGLB_AUTHOR_RYO);
    EV_EnsureCharacterDraw(TGLB_AUTHOR_RYO);
    mtwkSetExtraFlag(FindMotionWork(ctrlGetCharaByName(TGLB_AUTHOR_RYO)), 6, 2);
    EV_SetCharacterAlphaEnabled(TGLB_AUTHOR_RYO, false);
    for (shen_name name : tglbAuthorProps) {
        EV_EnableCharacterDisplay(name);
        EV_EnsureCharacterDraw(name);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    const shen_name *names = tglbAuthorDefaultCharacters.data();
    if (context->authorCmd.name == TGLB_AUTHOR_AU_7.name)
        names = tglbAuthorVictoryCharacters.data();
    else if (context->authorCmd.name == TGLB_AUTHOR_AU_8.name)
        names = tglbAuthorFinalCharacters.data();
    for (std::size_t index = 0; names[index].name != EVENT_CHARACTER_INVALID.name; ++index) {
        const shen_name name = names[index];
        EV_EnableCharacterDisplay(name);
        EV_EnsureCharacterDraw(name);
        mtwkSetExtraFlag(FindMotionWork(ctrlGetCharaByName(name)), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
    }
}

// @verified - p
std::int64_t RestoreTglbAuthorCharacters(TglbAuthorContext *context) {
    mtwkSetExtraFlag(FindMotionWork(ctrlGetCharaByName(TGLB_AUTHOR_RYO)), 6, 0);
    for (shen_name name : tglbAuthorProps)
        EV_SetCharacterAlphaEnabled(name, false);
    const shen_name *names = tglbAuthorDefaultCharacters.data();
    if (context->authorCmd.name == TGLB_AUTHOR_AU_7.name)
        names = tglbAuthorVictoryCharacters.data();
    else if (context->authorCmd.name == TGLB_AUTHOR_AU_8.name)
        names = tglbAuthorFinalCharacters.data();
    std::size_t index = 0;
    do {
        mtwkSetExtraFlag(FindMotionWork(ctrlGetCharaByName(names[index])), 6, 0);
        ++index;
    } while (names[index].name != EVENT_CHARACTER_INVALID.name);
    return names[index].name;
}

} // namespace shenmue::s2
