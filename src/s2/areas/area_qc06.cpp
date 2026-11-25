#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQC06MainLoop(void *taskData);
void AreaQC06LoadAudio(int afternoon);
#else
#include "s2/areas/area_qc06.hpp"
#include "s2/event_characters.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {



// @verified - p
void AreaQC06Init() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQC06Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
    FTBL.SetMapControlStatusFlag(0, 0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQC06MainLoop, 4, 0x0b, TASK_EVNT);
#else
    (void)FTBL.SetMapControlStatusFlag(0, 0);
    DateTime time;
    shenGetCurrentDateTime(time);
    (void)FTBL.EV_GetShenmueFlag(0xffU);
    FTBL.EV_SetWeatherInstruction(0x0fU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQC06MainLoop, 4, 0x0b, TASK_EVNT);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQC06Cleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0fU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQC06LoadAudio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQC06LoadAudio(int afternoon) {
}
#endif

// @verified - p
void AreaQC06MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQC06GameplayDefinition{
    .areaId = AREA_QC06,
    .padding04 = 0,
    .init = &AreaQC06Init,
    .main = &AreaQC06Main,
    .cleanup = &AreaQC06Cleanup,
};

} // namespace shenmue::s2::areas

#endif
