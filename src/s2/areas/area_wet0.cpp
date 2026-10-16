#include "s2/areas/area_wet0.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

} // namespace

// @verified - p
void AreaWET0Init() {
}

// @verified - p
void AreaWET0Main(std::uint32_t) {
    (void)EV_GetShenmueFlag(nightTimeFlag);
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
    (void)CheckIsArgEquZero(1);
    SetNewWindPhaseAndSpeed(0, 0.0f);
}

// @verified - p
void AreaWET0Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaWET0GameplayDefinition{
    .areaId = AREA_WET0,
    .padding04 = 0,
    .init = AreaWET0Init,
    .main = AreaWET0Main,
    .cleanup = AreaWET0Cleanup,
};

} // namespace shenmue::s2::areas
