#include "s2/areas/area_qc06.hpp"
#include "s2/event_characters.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaQC06Init() {
}

// @verified - p
void AreaQC06MainLoop(void *) {
}

// @verified - p
void AreaQC06Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    DateTime time;
    GetCurrentDateTime(time);
    (void)EV_GetShenmueFlag(0xffU);
    EV_SetWeatherInstruction(0x0fU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    HLib::EnqueueTaskWithoutParameter(AreaQC06MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQC06Cleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0fU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
}

const GameplayAreaDefinition areaQC06GameplayDefinition{
    .areaId = AREA_QC06,
    .padding04 = 0,
    .init = &AreaQC06Init,
    .main = &AreaQC06Main,
    .cleanup = &AreaQC06Cleanup,
};

} // namespace shenmue::s2::areas
