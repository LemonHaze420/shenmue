#include "s2/areas/area_qcae.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
void QCAEMainloopNullsub(void *) {
}
} // namespace

void AreaQCAEInit() {
}

void AreaQCAECleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetScenarioCharacterDrawControl(QCAE_PLAYER, 2, 0);
}

void AreaQCAEMain(std::uint32_t) {
    ResetSoundProgramPhaseFlags(0);
    SetMapControlStatusFlag(0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QCAEMainloopNullsub, 4, 0x0b, sizeof(QCAEMainloopData), QCAE_EVENT_TASK);
    QCAEMainloopData *const data = static_cast<QCAEMainloopData *>(HLib::GetTaskParameter(task));
    data->state = 0;
    data->field10 = 0;
    data->field18 = 0;
    data->field20 = 0;
    data->callback = QCAEMainloopNullsub;
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (static_cast<std::uint8_t>(dateTime.hour - 6) > 12U) {
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_AssignLightParams(1, 1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        AssignLightProgram(0);
        SetScenarioCharacterDrawControl(QCAE_PLAYER, 2, 0);
    } else {
        AreaQCAECleanup(0, 0);
    }
    if (flagRelated(g_currentMap))
        shenUpdatePlayerDataFlags(1);
    if (!shenUpdateFlagEvaluation(1))
        SetPlayerControlEnabled(1);
    SetPlayerControlEnabled(1);
    QueueImmediateNextFadeTask(task);
    FinishLoading();
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(QCAE_EVENT_TASK.name));
}

const GameplayAreaDefinition areaQCAEGameplayDefinition{
    .areaId = SHEN_NAME(QCAE),
    .padding04 = 0,
    .init = &AreaQCAEInit,
    .main = &AreaQCAEMain,
    .cleanup = &AreaQCAECleanup,
};

} // namespace shenmue::s2::areas
