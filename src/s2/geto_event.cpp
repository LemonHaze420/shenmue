#include "s2/geto_event.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/camera.hpp"
#include "s2/event_characters.hpp"
#include "s2/map_assets.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
namespace shenmue::s2 {

// @verified - p
void GetoInit() {
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void GetoMain(const shen_name *parameters)
#else
void GetoMain(std::uintptr_t argument)
#endif
{
#if !defined(SHENMUE_EVENT_OVERLAY)
    const std::uint32_t *const parameters = reinterpret_cast<const std::uint32_t *>(argument);
#endif
    if (!parameters)
        return;
    if (!parameters[0] && !parameters[1] && !parameters[2]) {
        FTBL.SetPlayerControlEnabled(1);
        return;
    }
    {
#if defined(SHENMUE_EVENT_OVERLAY)
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(GetoLoop, 4, 11, sizeof(GetoEventWork), TASK_EVNT);
#else
        HLib::Task *const task = FTBL.HLib_EnqueueTaskWithParameter(GetoLoop, 4, 0x0b, 0x10, EVENT_CHARACTER_TASK);
#endif
        if (task) {
#if defined(SHENMUE_EVENT_OVERLAY)
            shen_name *work = (shen_name *)FTBL.HLib_GetTaskParameter(task);
#else
            GetoEventWork *const work = static_cast<GetoEventWork *>(FTBL.HLib_GetTaskParameter(task));
#endif
            if (work) {
#if defined(SHENMUE_EVENT_OVERLAY)
                *work++ = parameters[0];
                *work++ = parameters[1];
                *work++ = parameters[2];
                *(HLibTask **)work = FTBL.HLib_GetCurrentTask();
#else
                work->prop = parameters[0];
                work->searchTarget = parameters[1];
                work->enabled = parameters[2];
                work->caller = FTBL.HLib_GetCurrentTask();
#endif
                FTBL.SetPlayerControlEnabled(0);
                FTBL.HLib_SetupTaskDelay(-1);
            } else {
                FTBL.HLib_DestroyTask(task);
            }
        }
    }
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void GetoLoop(void *taskData) {
    GetoEventWork *const work = (GetoEventWork *)taskData;
    if (work && !FTBL.filmCheckFadeTrackColour()) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetDisplayFlagForProp(work->prop, work->enabled);
        if (work->searchTarget)
            FTBL.srchSetTargetEnabled(work->searchTarget, work->enabled);
#else
        FTBL.SetDisplayFlagForProp(shen_name{static_cast<std::uint32_t>(work->prop)}, static_cast<std::uint32_t>(work->enabled));
        if (work->searchTarget)
            FTBL.srchSetTargetEnabled(shen_name{static_cast<std::uint32_t>(work->searchTarget)}, static_cast<std::int32_t>(work->enabled));
#endif
        FTBL.SetScreenFilmFade(8, 0xff, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_ResolveDeferredCallback(work->caller);
        FTBL.HLib_SetupTimedTask(work->caller, 1);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.HLib_MarkCurrentTaskForCleanup();
    }
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition getoScriptDefinition{
    .areaId = GETO_AREA,
    .field04 = 0,
    .init = &GetoInit,
    .main = &GetoMain,
};
} // namespace shenmue::s2
#endif
