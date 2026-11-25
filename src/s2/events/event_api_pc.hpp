#pragma once

#include "s2/area_jump.hpp"
#include "s2/camera.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_weather.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {

struct EventFunctionBindings {
    static constexpr auto HLib_DestroyCurrentTask = HLib::DestroyCurrentTask;
    static constexpr auto HLib_DestroyTask = HLib::DestroyTask;
    static constexpr auto HLib_EnqueueTaskWithParameter = HLib::EnqueueTaskWithParameter;
    static constexpr auto HLib_EnqueueTaskWithoutParameter = HLib::EnqueueTaskWithoutParameter;
    static constexpr auto HLib_GetTaskParameter = HLib::GetTaskParameter;
    static constexpr auto QueueImmediateNextFadeTask = s2::QueueImmediateNextFadeTask;
    static constexpr auto SetPlayerControlEnabled = s2::SetPlayerControlEnabled;
    static constexpr auto SetScreenFilmFade = s2::SetScreenFilmFade;
    static constexpr auto camCtrlSetFovTransitionTo = s2::camCtrlSetFovTransitionTo;
    static constexpr auto BeginPrimarySampleExecution = s2::BeginPrimarySampleExecution;
    static constexpr auto BeginSecondarySampleExecution = s2::BeginSecondarySampleExecution;
    static constexpr auto EV_SetWeatherInstruction = s2::EV_SetWeatherInstruction;
    static constexpr auto EV_lightLoad = s2::EV_lightLoad;
    static constexpr auto FinishLoading = s2::FinishLoading;
    static constexpr auto HLib_GetCurrentTask = HLib::GetCurrentTask;
    static constexpr auto HLib_MarkCurrentTaskForCleanup = HLib::MarkCurrentTaskForCleanup;
    static constexpr auto HLib_ResolveDeferredCallback = HLib::ResolveDeferredCallback;
    static constexpr auto HLib_SetupTaskDelay = HLib::SetupTaskDelay;
    static constexpr auto HLib_SetupTimedTask = HLib::SetupTimedTask;
    static constexpr auto HasSavedAreaJumpLocation = s2::HasSavedAreaJumpLocation;
    static constexpr auto NpcWait = s2::NpcWait;
    static constexpr auto PushGlobalLightingProgram = s2::PushGlobalLightingProgram;
    static constexpr auto SetDisplayFlagForProp = s2::SetDisplayFlagForProp;
    static constexpr auto SetMapControlStatusFlag = s2::SetMapControlStatusFlag;
    static constexpr auto filmCheckFadeTrackColour = s2::filmCheckFadeTrackColour;
    static constexpr auto flagRelated = s2::flagRelated;
    static constexpr auto shenUpdateFlagEvaluation = s2::shenUpdateFlagEvaluation;
    static constexpr auto shenUpdatePlayerDataFlags = s2::shenUpdatePlayerDataFlags;
    static constexpr auto srchSetTargetEnabled = s2::srchSetTargetEnabled;
    static inline std::uint64_t camCtrlSetState(std::int32_t state) {
        return s2::camCtrlSetState(static_cast<CameraState>(state));
    }
    static inline void EV_ApplyTranslationClockVisibility() {
        s2::EV_ApplyTranslationClockVisibility(0);
    }
};

inline constexpr EventFunctionBindings FTBL{};
using HLibTask = HLib::Task;
inline constexpr std::int32_t TASK_EVNT = static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name);

} // namespace shenmue::s2
