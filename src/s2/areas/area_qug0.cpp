#include "s2/areas/area_qug0.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaQUG0Main(std::uint32_t) {
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
}

const GameplayAreaDefinition areaQUG0GameplayDefinition{
    .areaId = AREA_QUG0,
    .padding04 = 0,
    .init = &AreaElevatorLampInit,
    .main = &AreaQUG0Main,
    .cleanup = &AreaElevatorLampCleanup,
};

} // namespace shenmue::s2::areas
