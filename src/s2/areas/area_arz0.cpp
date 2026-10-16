#include "s2/areas/area_arz0.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

} // namespace

// @verified - p
void AreaARZ0Init() {
}

// @verified - p
void AreaARZ0Main(std::uint32_t) {
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    (void)EV_GetShenmueFlag(nightTimeFlag);
    EV_SetModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
}

// @verified - p
void AreaARZ0Cleanup(std::int32_t, std::uint32_t) {
    EV_SetModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
}

const GameplayAreaDefinition areaARZ0GameplayDefinition{
    .areaId = AREA_ARZ0,
    .padding04 = 0,
    .init = AreaARZ0Init,
    .main = AreaARZ0Main,
    .cleanup = AreaARZ0Cleanup,
};

} // namespace shenmue::s2::areas
