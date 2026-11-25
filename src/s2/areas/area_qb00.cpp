#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/areas/area_jukebox.hpp"
enum {
    QB00_FOG_WEATHER_MASK = 0x780,
    QB00_GLOBAL_LIGHTING_MASK = 0x0c,
    QB00_NIGHT_FLAG = 0xff,
    QB00_FOG_COLOUR = 0xff000000,
    QB00_RYO = SHEN_NAME4('R', 'Y', 'O', '_')
};
void AreaCleanup(int afternoon);
void AreaQB00CleanupNullsub(int afternoon);
void AreaQB00MainLoop(void *taskData);
#else
#include "s2/areas/area_qb00.hpp"
#include "s2/areas/area_wecf.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
namespace shenmue::s2::areas {
// @verified - p
void AreaQB00Init() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    EventDateTime dateTime;
#else
void AreaQB00Main(std::uint32_t) {
#endif
    (void)FTBL.SetMapControlStatusFlag(0, 0);
    FTBL.EV_SetWeatherInstruction(QB00_FOG_WEATHER_MASK, 0);
    FTBL.EV_SetWeatherInstruction(QB00_GLOBAL_LIGHTING_MASK, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    do {
        float red = 35.0f;
        float green = 40.0f;
        float blue = 1.0f;
        FTBL.SetFogRed(red);
        FTBL.SetFogGreen(green);
        FTBL.SetFogBlue(blue);
        FTBL.SetFogDistance(QB00_FOG_COLOUR);
        FTBL.ReinitFogTasks(1);
    } while (0);
#else
    (void)FTBL.SetFogRed(35.0f);
    (void)FTBL.SetFogGreen(40.0f);
    (void)FTBL.SetFogBlue(1.0f);
    (void)FTBL.SetFogDistance(QB00_FOG_COLOUR);
    FTBL.ReinitFogTasks(1);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if (FTBL.EV_GetShenmueFlag(QB00_NIGHT_FLAG))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
    (void)EV_GetShenmueFlag(QB00_NIGHT_FLAG);
#endif
    FTBL.AssignLightParams(1, 4);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.AssignLightProgram(1);
    FTBL.AssignLightProgram(2);
    FTBL.AssignLightProgram(3);
    FTBL.SetupLightConfig();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetScenarioCharacterDrawControl(QB00_RYO, 2, 0, 0.0f);
#else
    SetScenarioCharacterDrawControl(QB00_RYO, 2, 0);
#endif
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQB00MainLoop, 4, 0x0b, TASK_EVNT);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, TASK_EVNT);
#else
    FTBL.HLib_EnqueueTaskWithoutParameter(JukeboxInitCallback, 5, 0x0b, TASK_EVNT);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
    AreaQB00CleanupNullsub(afternoon);
}
void AreaQB00CleanupNullsub(int afternoon) {
#else
void AreaQB00Cleanup(std::int32_t, std::uint32_t) {
#endif
}

// @verified - p
void AreaQB00MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQB00GameplayDefinition{
    .areaId = AREA_QB00,
    .padding04 = 0,
    .init = &AreaQB00Init,
    .main = &AreaQB00Main,
    .cleanup = &AreaQB00Cleanup,
};
} // namespace shenmue::s2::areas
#endif
