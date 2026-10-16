#include "s2/areas/area_wka0.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t weatherInstruction = 0x0f;
constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

std::int32_t initializing{};

} // namespace

// @verified - p
void AreaWKA0Init() {
    initializing = 0;
}

// @verified - p
void AreaWKA0Main(std::uint32_t) {
    initializing = 1;
    EV_SetWeatherInstruction(weatherInstruction, 0);
    if (EV_GetShenmueFlag(nightTimeFlag) != 0) {
        AreaWKA0Cleanup(1, 0);
    } else {
        AreaWKA0Cleanup(0, 0);
    }
    initializing = 0;
}

// @verified - p
void AreaWKA0Cleanup(std::int32_t, std::uint32_t) {
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(1);
    AssignLightProgram(2);
    SetupLightConfig();
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_AssignLightParams(1, 3);
    if (initializing == 0) {
        EV_LoadCutsceneFromCurrentScene("WKA0");
        EV_LoadAudio("WKA0_000", 0);
    }
}

const GameplayAreaDefinition areaWKA0GameplayDefinition{
    .areaId = AREA_WKA0,
    .padding04 = 0,
    .init = AreaWKA0Init,
    .main = AreaWKA0Main,
    .cleanup = AreaWKA0Cleanup,
};

} // namespace shenmue::s2::areas
