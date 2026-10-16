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

// @verified - p
void AreaQAW1MainLoop(void *) {
}

// @verified - p
void AreaQAW1Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    DateTime time;
    GetCurrentDateTime(time);
    if (EV_GetShenmueFlag(0xffU) != 0) {
        EV_SetWeatherInstruction(0x0cU, 0);
        EV_SetWeatherInstruction(0x780U, 0);
        EV_SetWeatherInstruction(0x1800U, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)SetAreaFogParams();
    } else {
        EV_SetWeatherInstruction(0x0cU, 0);
        EV_SetWeatherInstruction(0x780U, 0);
        EV_SetWeatherInstruction(0x1800U, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)SetAreaFogParams();
        SetDefaultVSByIndex(9);
    }
    EV_AssignLightParams(1, 9);
    HLib::EnqueueTaskWithoutParameter(AreaQAW1MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQAW1Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(0x0cU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    if (afternoon == 0)
        SetDefaultVSByIndex(9);
    EV_AssignLightParams(1, 9);
}

const GameplayAreaDefinition areaQAW1GameplayDefinition{
    .areaId = AREA_QAW1,
    .padding04 = 0,
    .init = &AreaQAW1Init,
    .main = &AreaQAW1Main,
    .cleanup = &AreaQAW1Cleanup,
};

} // namespace shenmue::s2::areas
