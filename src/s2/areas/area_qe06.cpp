#include "s2/areas/area_qe06.hpp"
#include "s2/areas/area_chime.hpp"
#include "s2/areas/area_qe01.hpp"
#include "s2/areas/area_qc06.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaQE06Init() {
}

// @verified - p
void AreaQE06MainLoop(void *) {
}

// @verified - p
void AreaQE06Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    (void)EV_GetShenmueFlag(0xffU);
    EV_SetWeatherInstruction(0x0fU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();

    if (g_currentMap.name == AREA_QE00.name || g_currentMap.name == AREA_QE01.name || g_currentMap.name == AREA_QE06.name) {
        HLib::EnqueueTaskWithoutParameter(AreaChimeCallback, 4, 0x0b, static_cast<std::int32_t>(CHIME_TASK.name));
        areaChimeLastSound = 100;
    }
    HLib::EnqueueTaskWithoutParameter(AreaQE06MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    EV_SetAlignedShadowRange(7.0f);
}

const GameplayAreaDefinition areaQE06GameplayDefinition{
    .areaId = AREA_QE06,
    .padding04 = 0,
    .init = &AreaQE06Init,
    .main = &AreaQE06Main,
    .cleanup = &AreaQC06Cleanup,
};

} // namespace shenmue::s2::areas
