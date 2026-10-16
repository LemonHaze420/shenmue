#include "s2/areas/area_aky0.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"

#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

std::int32_t initFlag{};

} // namespace

// @verified - p
void AreaAKY0Init() {
    initFlag = 0;
}

// @verified - p
void AreaAKY0Main(std::uint32_t) {
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_AssignLightParams(1, 3);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(1);
    AssignLightProgram(2);
    (void)EV_GetShenmueFlag(nightTimeFlag);
    initFlag = 0;
}

// @verified - p
void AreaAKY0Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaAKY0GameplayDefinition{
    .areaId = AREA_AKY0,
    .padding04 = 0,
    .init = AreaAKY0Init,
    .main = AreaAKY0Main,
    .cleanup = AreaAKY0Cleanup,
};

} // namespace shenmue::s2::areas
