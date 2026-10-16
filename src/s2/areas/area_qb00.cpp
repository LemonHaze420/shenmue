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

// @verified - p
void AreaQB00Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    EV_SetWeatherInstruction(QB00_FOG_WEATHER_MASK, 0);
    EV_SetWeatherInstruction(QB00_GLOBAL_LIGHTING_MASK, 0);
    (void)SetFogRed(35.0f);
    (void)SetFogGreen(40.0f);
    (void)SetFogBlue(1.0f);
    (void)SetFogDistance(QB00_FOG_COLOUR);
    ReinitFogTasks(1);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    (void)EV_GetShenmueFlag(QB00_NIGHT_FLAG);
    EV_AssignLightParams(1, 4);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(1);
    AssignLightProgram(2);
    AssignLightProgram(3);
    SetupLightConfig();
    SetScenarioCharacterDrawControl(QB00_RYO, 2, 0);
    HLib::EnqueueTaskWithoutParameter(AreaQB00MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    HLib::EnqueueTaskWithoutParameter(JukeboxInitCallback, 5, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQB00Cleanup(std::int32_t, std::uint32_t) {
}

// @verified - p
void AreaQB00MainLoop(void *) {
}

const GameplayAreaDefinition areaQB00GameplayDefinition{
    .areaId = AREA_QB00,
    .padding04 = 0,
    .init = &AreaQB00Init,
    .main = &AreaQB00Main,
    .cleanup = &AreaQB00Cleanup,
};

} // namespace shenmue::s2::areas
