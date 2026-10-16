#include "s2/areas/area_qe01.hpp"
#include "s2/areas/area_chime.hpp"
#include "s2/areas/area_qe06.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

#include <cstdint>

namespace shenmue::s2::areas {

// @verified - p
void AreaQE01Init() {
}

// @verified - p
void AreaQE01MainLoop(void *) {
}

// @verified - p
void AreaQE01Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    AreaQE01Cleanup(EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);

    if (g_currentMap.name == AREA_QE00.name || g_currentMap.name == AREA_QE01.name || g_currentMap.name == AREA_QE06.name) {
        HLib::EnqueueTaskWithoutParameter(AreaChimeCallback, 4, 0x0b, static_cast<std::int32_t>(CHIME_TASK.name));
        areaChimeLastSound = 100;
    }
    HLib::EnqueueTaskWithoutParameter(AreaQE01MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    EV_SetAlignedShadowRange(7.0f);
}

// @verified - p
void AreaQE01Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(0x0cU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    (void)EV_LoadCutsceneFromCurrentScene("QE01");
    (void)EV_LoadAudio("QAE1_000", 0);
    if (afternoon != 0) {
        Vector4 savedGlobalLighting;
        GetGlobalLighting(&savedGlobalLighting);
        UpdateGlobalLighting(qe01NightGlobalLighting);
        AssignLightProgram(7);
        UpdateGlobalLighting(savedGlobalLighting);
    } else {
        SetDefaultVSByIndex(7);
    }
    EV_AssignLightParams(1, 7);
}

const GameplayAreaDefinition areaQE01GameplayDefinition{
    .areaId = AREA_QE01,
    .padding04 = 0,
    .init = &AreaQE01Init,
    .main = &AreaQE01Main,
    .cleanup = &AreaQE01Cleanup,
};

} // namespace shenmue::s2::areas
