#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
int ConfigureAreaElevatorLamp(int enabled);
void AreaCleanup(int afternoon);
void LoadElevatorAreaAudio(int afternoon);
void AreaElevatorLampMainLoop(void *taskData);
#else
#include "s2/areas/area_qf39.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    FTBL.SetMapControlStatusFlag(0, 0);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaElevatorLampMainLoop, 4, 0x0b, TASK_EVNT);
#else
void AreaQF39Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    (void)EV_GetShenmueFlag(0xff);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    (void)ConfigureAreaElevatorLamp();
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    HLib::EnqueueTaskWithoutParameter(AreaElevatorLampMainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
    FTBL.EV_SetWeatherInstruction(0x0f, 0);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_SetWeatherInstruction(0x1800, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    ConfigureAreaElevatorLamp(1);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    FTBL.SetAreaFogParams();
    LoadElevatorAreaAudio(afternoon);
}
void LoadElevatorAreaAudio(int afternoon) {
}
void AreaElevatorLampMainLoop(void *taskData) {
}
#else
const GameplayAreaDefinition areaQF39GameplayDefinition{
    .areaId = AREA_QF39,
    .padding04 = 0,
    .init = &AreaElevatorLampInit,
    .main = &AreaQF39Main,
    .cleanup = &AreaElevatorLampCleanup,
};

} // namespace shenmue::s2::areas

#endif
