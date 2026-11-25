#include "s2/krht_event.hpp"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/sound_program.hpp"
#endif

#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace shenmue::s2 {

// @verified - p
void KRHTInit() {}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
// @verified - p
void KRHTMain(KrhtStartParameters argument)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    struct KrhtPlacement {
        Vector3 position;
        Vector3i rotation;
    };
    struct KrhtPlacement first = {{1401.1f, 100.0f, 790.6f}, {0, 0x1000, 0}};
    struct KrhtPlacement second = {{1402.2f, 100.0f, 793.2f}, {0, 0x9000, 0}};
#endif
    if ((unsigned int)argument == 2) {
        FTBL.HLib_EnqueueTaskWithoutParameter(KRHTStartAuthor, 3, 2, KRHT_TASK_TOKEN);
#if defined(SHENMUE_EVENT_OVERLAY)
        if (FTBL.drGetCurrentAuthorId() == KRHT_SOURCE_AUTHOR) {
#else
        if (GetCurrentAuthorId().name == KRHT_SOURCE_AUTHOR.name) {
#endif
            (void)FTBL.EV_LoadCutsceneFromCurrentScene(KRHT_MEDIA);
#if defined(SHENMUE_EVENT_OVERLAY)
            (void)FTBL.EV_LoadAudio(FTBL.shenGetCurrentSbt(), 0);
#else
            (void)EV_LoadAudio(shenGetCurrentSbt(), 0);
#endif
            (void)FTBL.EV_spmBeginSampleExecute(30);
        }
#if defined(SHENMUE_EVENT_OVERLAY)
    } else {
        FTBL.DebugLogToFileRemoved("誰じゃい！！\n");
#endif
    }
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_EVENT_ENTRY)
// @verified - p
void KRHTStartAuthor(void *taskData)
{
    (void)FTBL.EV_authSceneCtrl(-8);
    (void)FTBL.EV_authSceneCtrl(KRHT_TARGET_AUTHOR_COMMAND);
    FTBL.HLib_SetCurrentTaskTo(KRHTWaitAuthor);
}

// @verified - p
void KRHTWaitAuthor(void *taskData)
{
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) == 0) {
        (void)FTBL.EV_authSceneCtrl(-7);
        FTBL.HLib_DestroyCurrentEventTask();
        (void)FTBL.EV_scenQueueLoad(4, KRHT_TARGET_AREA, 0);
    }
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition krhtScriptDefinition{.areaId = KRHT_AREA, .field04 = 0, .init = &KRHTInit, .main = &KRHTMain};

} // namespace shenmue::s2
#endif
