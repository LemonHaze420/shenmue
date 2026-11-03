#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_view.hpp"

void StartView(void)
{
    HLibTask *task;
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_setHoldStatesFlags(0);
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    task = FTBL.HLib_EnqueueTaskWithoutParameter(UpdateView, 4, 11, TASK_EVNT);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.QueueImmediateNextFadeTask(task);
    FTBL.FinishLoading();
}

void UpdateView(void *taskData)
{
}
#else
#endif
