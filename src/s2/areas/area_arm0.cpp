#include "s2/areas/area_arm0.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

} // namespace

// @verified - p
void AreaARM0Init() {
}

// @verified - p
void AreaARM0Main(std::uint32_t) {
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
    (void)EV_GetShenmueFlag(nightTimeFlag);
}

// @verified - p
void AreaARM0Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaARM0GameplayDefinition{
    .areaId = AREA_ARM0,
    .padding04 = 0,
    .init = AreaARM0Init,
    .main = AreaARM0Main,
    .cleanup = AreaARM0Cleanup,
};

} // namespace shenmue::s2::areas
