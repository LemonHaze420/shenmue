#include "s2/areas/area_qae6.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {

std::int32_t areaQAE6Initializing{};

} // namespace

// @verified - p
void AreaQAE6Init() {
}

// @verified - p
void AreaQAE6MainLoop(void *) {
}

// @verified - p
void AreaQAE6Main(std::uint32_t) {
    areaQAE6Initializing = 1;
    (void)SetMapControlStatusFlag(0, 0);
    AreaQAE6Cleanup(EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
    HLib::EnqueueTaskWithoutParameter(AreaQAE6MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    areaQAE6Initializing = 0;
}

// @verified - p
void AreaQAE6Cleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0fU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetWeatherInstruction(0x1800U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    if (areaQAE6Initializing == 0) {
        EV_LoadCutsceneFromCurrentScene("QAE6");
        (void)EV_LoadAudio("QAE1_000", 0);
    }
}

const GameplayAreaDefinition areaQAE6GameplayDefinition{
    .areaId = AREA_QAE6,
    .padding04 = 0,
    .init = &AreaQAE6Init,
    .main = &AreaQAE6Main,
    .cleanup = &AreaQAE6Cleanup,
};

} // namespace shenmue::s2::areas
