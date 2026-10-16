#include "s2/areas/area_tes2.hpp"
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

// @verified - p
void Tes2Main(std::uintptr_t) {
    EV_SetWeatherInstruction(15, 0);
    (void)SetMapControlStatusFlag(0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(Tes2MainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(TES2_TASK.name));
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    if (flagRelated(g_currentMap))
        (void)shenUpdatePlayerDataFlags(1);
    if (!shenUpdateFlagEvaluation(1))
        SetPlayerControlEnabled(1);
    if (HasSavedAreaJumpLocation())
        EV_ApplyTranslationClockVisibility(0);
    QueueImmediateNextFadeTask(task);
    FinishLoading();
}

// @verified - p
void Tes2MainLoopCallback(void *) {
    (void)shenUpdateFlagEvaluation(0);
}

const ScriptAreaDefinition tes2ScriptDefinition{.areaId = AREA_TES2, .field04 = 0, .init = Tes2Init, .main = Tes2Main};

} // namespace shenmue::s2
