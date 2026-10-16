#include "s2/krht_event.hpp"
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {

// @verified - p
void KRHTInit() {
}

// @verified - p
void KRHTMain(std::uintptr_t argument) {
    if (static_cast<std::uint32_t>(argument) == 2) {
        HLib::EnqueueTaskWithoutParameter(KRHTStartAuthor, 3, 2, static_cast<std::int32_t>(KRHT_TASK.name));
        if (GetCurrentAuthorId().name == KRHT_SOURCE_AUTHOR.name) {
            (void)EV_LoadCutsceneFromCurrentScene(KRHT_MEDIA);
            (void)EV_LoadAudio(shenGetCurrentSbt(), 0);
            (void)EV_spmBeginSampleExecute();
        }
    }
}

// @verified - p
void KRHTStartAuthor(void *) {
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KRHT_TARGET_AUTHOR.name));
    HLib::SetCurrentTaskTo(KRHTWaitAuthor);
}

// @verified - p
void KRHTWaitAuthor(void *) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        (void)EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        HLib::DestroyCurrentTask();
        (void)EV_scenQueueLoad(4, KRHT_TARGET_AREA, 0);
    }
}

const ScriptAreaDefinition krhtScriptDefinition{.areaId = KRHT_AREA, .field04 = 0, .init = &KRHTInit, .main = &KRHTMain};

} // namespace shenmue::s2
