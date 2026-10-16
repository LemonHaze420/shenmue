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

// @verified - p
void AreaQEDJMainLoop(void *) {
}

// @verified - p
void AreaQEDJMain(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    AreaQEDJCleanup(EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
    HLib::EnqueueTaskWithoutParameter(AreaQEDJMainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQEDJCleanup(std::int32_t, std::uint32_t) {
    SetIsSkySetup(0);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_SetWeatherInstruction(0x0fU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
}

const GameplayAreaDefinition areaQEDJGameplayDefinition{
    .areaId = AREA_QEDJ,
    .padding04 = 0,
    .init = &AreaQEDJInit,
    .main = &AreaQEDJMain,
    .cleanup = &AreaQEDJCleanup,
};

} // namespace shenmue::s2::areas
