#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQFRRMainLoop(void *taskData);
void AreaQFRRLoadAudio(int afternoon);
#else
#include "s2/areas/area_qfrr.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {



// @verified - p
void AreaQFRRInit() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQFRRMain(std::uint32_t) {
#endif
    FTBL.SetWeatherUpdateEnabledTo(1);
    (void)FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    FTBL.SetIsSkySetup(0);
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    if (FTBL.EV_GetShenmueFlag(0xffU) != 0) {
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
#endif
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQFRRMainLoop, 4, 0x0b, TASK_EVNT);
    (void)FTBL.BeginPrimarySampleExecution(0);
    (void)FTBL.BeginSecondarySampleExecution(0);
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQFRRCleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    if (afternoon != 0) {
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQFRRLoadAudio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQFRRLoadAudio(int afternoon) {
}
#endif

// @verified - p
void AreaQFRRMainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQFRRGameplayDefinition{
    .areaId = AREA_QFRR,
    .padding04 = 0,
    .init = &AreaQFRRInit,
    .main = &AreaQFRRMain,
    .cleanup = &AreaQFRRCleanup,
};

} // namespace shenmue::s2::areas

#endif
