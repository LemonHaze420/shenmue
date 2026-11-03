#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_1000.hpp"

void StartMotionViewer(void)
{
    FTBL.HLib_EnqueueTaskWithoutParameter(UpdateMotionViewer, 4, 11, TASK_EVNT);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.FinishLoading();
}

void UpdateMotionViewer(void *taskData)
{
    EventDebugPrintf(1, 1, "This is Motion Veiwer!!");
}
#else
#endif
