#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQAW1MainLoop(void *taskData);
void AreaQAW1LoadAudio(int afternoon);
#else
#include "s2/areas/area_qaw1.hpp"
#include "s2/event_characters.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {



// @verified - p
void AreaQAW1Init() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQAW1Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
    FTBL.SetMapControlStatusFlag(0, 0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQAW1MainLoop, 4, 0x0b, TASK_EVNT);
#else
    (void)FTBL.SetMapControlStatusFlag(0, 0);
    DateTime time;
    shenGetCurrentDateTime(time);
    if (FTBL.EV_GetShenmueFlag(0xffU) != 0) {
        FTBL.EV_SetWeatherInstruction(0x0cU, 0);
        FTBL.EV_SetWeatherInstruction(0x780U, 0);
        FTBL.EV_SetWeatherInstruction(0x1800U, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        (void)FTBL.SetAreaFogParams();
    } else {
        FTBL.EV_SetWeatherInstruction(0x0cU, 0);
        FTBL.EV_SetWeatherInstruction(0x780U, 0);
        FTBL.EV_SetWeatherInstruction(0x1800U, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        (void)FTBL.SetAreaFogParams();
        FTBL.SetDefaultVSByIndex(9);
    }
    FTBL.AssignLightParams(1, 9);
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQAW1MainLoop, 4, 0x0b, TASK_EVNT);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQAW1Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0cU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQAW1LoadAudio(afternoon);
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
    if (afternoon == 0)
#endif
        FTBL.SetDefaultVSByIndex(9);
    FTBL.AssignLightParams(1, 9);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQAW1LoadAudio(int afternoon) {
}
#endif

// @verified - p
void AreaQAW1MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQAW1GameplayDefinition{
    .areaId = AREA_QAW1,
    .padding04 = 0,
    .init = &AreaQAW1Init,
    .main = &AreaQAW1Main,
    .cleanup = &AreaQAW1Cleanup,
};

} // namespace shenmue::s2::areas

#endif
