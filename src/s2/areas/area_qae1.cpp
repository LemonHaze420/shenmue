#include "s2/areas/area_qae1.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaQAE1Init() {
}

// @verified - p
void AreaQAE1MainLoop(void *) {
}

// @verified - p
void AreaQAE1Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    if (EV_GetShenmueFlag(0xffU) != 0) {
        EV_SetWeatherInstruction(0x0cU, 0);
        EV_SetWeatherInstruction(0x780U, 0);
        EV_SetWeatherInstruction(0x1800U, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)SetAreaFogParams();
        EV_LoadCutsceneFromCurrentScene("QAE1");
        (void)EV_LoadAudio("QAE1_000", 0);
        EV_AssignLightParams(1, 8);
    } else {
        AreaQAE1Cleanup(0, 0);
    }
    HLib::EnqueueTaskWithoutParameter(AreaQAE1MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQAE1Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(0x0cU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    EV_LoadCutsceneFromCurrentScene("QAE1");
    (void)EV_LoadAudio("QAE1_000", 0);
    if (afternoon == 0)
        SetDefaultVSByIndex(8);
    EV_AssignLightParams(1, 8);
}

const GameplayAreaDefinition areaQAE1GameplayDefinition{
    .areaId = AREA_QAE1,
    .padding04 = 0,
    .init = &AreaQAE1Init,
    .main = &AreaQAE1Main,
    .cleanup = &AreaQAE1Cleanup,
};

} // namespace shenmue::s2::areas
