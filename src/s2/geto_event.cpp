#include "s2/geto_event.hpp"
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

// @verified - p
void GetoMain(std::uintptr_t argument) {
    const std::uint32_t *const parameters = reinterpret_cast<const std::uint32_t *>(argument);
    if (!parameters)
        return;
    if (parameters[0] || parameters[1] || parameters[2]) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(GetoLoop, 4, 0x0b, 0x10, EVENT_CHARACTER_TASK);
        if (task) {
            GetoEventWork *const work = static_cast<GetoEventWork *>(HLib::GetTaskParameter(task));
            if (work) {
                work->prop = parameters[0];
                work->searchTarget = parameters[1];
                work->enabled = parameters[2];
                work->caller = HLib::GetCurrentTask();
                SetPlayerControlEnabled(0);
                HLib::SetupTaskDelay(-1);
            } else {
                HLib::DestroyTask(task);
            }
        }
    } else {
        SetPlayerControlEnabled(1);
    }
}

// @verified - p
void GetoLoop(void *taskData) {
    using enum CameraState;
    GetoEventWork *const work = static_cast<GetoEventWork *>(taskData);
    if (work && !filmCheckFadeTrackColour()) {
        SetDisplayFlagForProp(shen_name{static_cast<std::uint32_t>(work->prop)}, static_cast<std::uint32_t>(work->enabled));
        if (work->searchTarget) {
            srchSetTargetEnabled(shen_name{static_cast<std::uint32_t>(work->searchTarget)}, static_cast<std::int32_t>(work->enabled));
        }
        SetScreenFilmFade(8, 0xff, 0, 0, 0, 0, 0, 0, 0);
        HLib::ResolveDeferredCallback(work->caller);
        HLib::SetupTimedTask(work->caller, 1);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        HLib::MarkCurrentTaskForCleanup();
    }
}

const ScriptAreaDefinition getoScriptDefinition{
    .areaId = GETO_AREA,
    .field04 = 0,
    .init = &GetoInit,
    .main = &GetoMain,
};

} // namespace shenmue::s2
