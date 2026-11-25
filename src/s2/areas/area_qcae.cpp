#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/areas/area_jukebox.hpp"
typedef struct QCAEMainloopData {
    unsigned short state[4];
    HLibTaskCallback callback;
    unsigned int field0c, field10, field14, field18, field1c, field20;
} QCAEMainloopData;
#define QCAE_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
void AreaCleanup(int afternoon);
void QCAEMainloopNullsub(void *taskData);
#else
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
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    HLibTask *task;
    QCAEMainloopData *data;
    EventDateTime dateTime;
#else
void AreaQCAEMain(std::uint32_t) {
    ResetSoundProgramPhaseFlags(0);
#endif
    FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(QCAEMainloopNullsub, 4, 0x0b, sizeof(QCAEMainloopData), TASK_EVNT);
    data = FTBL.HLib_GetTaskParameter(task);
    data->state[0] = 0;
    data->state[1] = 0;
    data->state[2] = 0;
    data->state[3] = 0;
    data->callback = QCAEMainloopNullsub;
    data->field0c = 0;
    data->field10 = 0;
    data->field14 = 0;
    data->field18 = 0;
    data->field1c = 0;
    data->field20 = 0;
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 6) > 12)
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QCAEMainloopNullsub, 4, 0x0b, sizeof(QCAEMainloopData), QCAE_EVENT_TASK);
    QCAEMainloopData *const data = static_cast<QCAEMainloopData *>(HLib::GetTaskParameter(task));
    data->state = 0;
    data->field10 = 0;
    data->field18 = 0;
    data->field20 = 0;
    data->callback = QCAEMainloopNullsub;
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
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
#endif
    if (FTBL.flagRelated(g_currentMap))
        FTBL.shenUpdatePlayerDataFlags(1);
    if (!FTBL.shenUpdateFlagEvaluation(1))
        FTBL.SetPlayerControlEnabled(1);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.QueueImmediateNextFadeTask(task);
    FTBL.FinishLoading();
    FTBL.HLib_EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, TASK_EVNT);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQCAECleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_SetWeatherInstruction(0x0c, 0);
    FTBL.AssignLightParams(1, 1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetScenarioCharacterDrawControl(QCAE_PLAYER, 2, 0, 0.0f);
#else
    SetScenarioCharacterDrawControl(QCAE_PLAYER, 2, 0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void QCAEMainloopNullsub(void *taskData) {
}
#else
const GameplayAreaDefinition areaQCAEGameplayDefinition{
    .areaId = SHEN_NAME(QCAE),
    .padding04 = 0,
    .init = &AreaQCAEInit,
    .main = &AreaQCAEMain,
    .cleanup = &AreaQCAECleanup,
};
} // namespace shenmue::s2::areas
#endif
