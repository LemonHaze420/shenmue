#include "s2/areas/area_tes2.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/area_jump.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {

// @verified - p
void Tes2Init() {
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void Tes2Main(void)
#else
void Tes2Main(std::uintptr_t)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    HLibTask *task;
#endif
    FTBL.EV_SetWeatherInstruction(15, 0);
    (void)FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithoutParameter(Tes2MainLoopCallback, 4, 11, TASK_EVNT);
#else
    HLib::Task *const task = FTBL.HLib_EnqueueTaskWithoutParameter(Tes2MainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(TES2_TASK.name));
#endif
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    (void)FTBL.BeginPrimarySampleExecution(0);
    (void)FTBL.BeginSecondarySampleExecution(0);
    if (FTBL.flagRelated(g_currentMap))
        (void)FTBL.shenUpdatePlayerDataFlags(1);
    if (!FTBL.shenUpdateFlagEvaluation(1))
        FTBL.SetPlayerControlEnabled(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.HasSavedAreaJumpLocation()) {
        FTBL.DebugLogToFileRemoved("ON\n");
        FTBL.EV_ApplyTranslationClockVisibility();
    } else {
        FTBL.DebugLogToFileRemoved("OFF\n");
    }
#else
    if (FTBL.HasSavedAreaJumpLocation())
        EV_ApplyTranslationClockVisibility(0);
#endif
    FTBL.QueueImmediateNextFadeTask(task);
    FTBL.FinishLoading();
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void Tes2MainLoopCallback(void *taskData) {
    (void)FTBL.shenUpdateFlagEvaluation(0);
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition tes2ScriptDefinition{.areaId = AREA_TES2, .field04 = 0, .init = Tes2Init, .main = Tes2Main};
} // namespace shenmue::s2
#endif
