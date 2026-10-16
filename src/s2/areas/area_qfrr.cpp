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

// @verified - p
void AreaQFRRMainLoop(void *) {
}

// @verified - p
void AreaQFRRMain(std::uint32_t) {
    SetWeatherUpdateEnabledTo(1);
    (void)RefreshWeatherForCurrentTime(0);
    UpdateWeather();
    SetIsSkySetup(0);
    UpdateWeatherForMinute(0, 0, 0, 0.0f);
    UpdateWeatherForMinute(1, 0, 0, 0.0f);
    if (EV_GetShenmueFlag(0xffU) != 0) {
        EV_SetModelDataDefaultVertexShader();
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    HLib::EnqueueTaskWithoutParameter(AreaQFRRMainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
}

// @verified - p
void AreaQFRRCleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon != 0) {
        EV_SetModelDataDefaultVertexShader();
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
}

const GameplayAreaDefinition areaQFRRGameplayDefinition{
    .areaId = AREA_QFRR,
    .padding04 = 0,
    .init = &AreaQFRRInit,
    .main = &AreaQFRRMain,
    .cleanup = &AreaQFRRCleanup,
};

} // namespace shenmue::s2::areas
