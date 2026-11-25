#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQEDJMainLoop(void *taskData);
void AreaQEDJLoadAudio(int afternoon);
#else
#include "s2/areas/area_qedj.hpp"

#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {



// @verified - p
void AreaQEDJInit() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQEDJMain(std::uint32_t) {
#endif
    (void)FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    AreaQEDJCleanup(FTBL.EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
#endif
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQEDJMainLoop, 4, 0x0b, TASK_EVNT);
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQEDJCleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.SetIsSkySetup(0);

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.FreeWAIR();
#else
    EV_FreeWAIR();
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
#else
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeather();
#else
    EV_UpdateWeather();
#endif
    FTBL.EV_SetWeatherInstruction(0x0fU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQEDJLoadAudio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQEDJLoadAudio(int afternoon) {
}
#endif

// @verified - p
void AreaQEDJMainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQEDJGameplayDefinition{
    .areaId = AREA_QEDJ,
    .padding04 = 0,
    .init = &AreaQEDJInit,
    .main = &AreaQEDJMain,
    .cleanup = &AreaQEDJCleanup,
};

} // namespace shenmue::s2::areas

#endif
