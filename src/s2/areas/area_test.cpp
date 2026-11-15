#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

void UpdateTestArea(void *taskData);

void StartTestArea(void)
{
    HLibTask *task;
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    task = FTBL.HLib_EnqueueTaskWithoutParameter(UpdateTestArea, 4, 11, TASK_EVNT);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.BeginPrimarySampleExecution(0);
    FTBL.BeginSecondarySampleExecution(0);
    if (FTBL.flagRelated(g_currentMap))
        FTBL.shenUpdatePlayerDataFlags(1);
    if (!FTBL.shenUpdateFlagEvaluation(1))
        FTBL.SetPlayerControlEnabled(1);
    FTBL.QueueImmediateNextFadeTask(task);
    FTBL.FinishLoading();
}

void UpdateTestArea(void *taskData)
{
    FTBL.shenUpdateFlagEvaluation(0);
}
#endif
