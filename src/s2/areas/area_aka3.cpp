#include "s2/areas/area_aka3.hpp"

#include "s2/field_collision.hpp"
#include "s2/lighting.hpp"
#include "s2/event_runtime.hpp"

#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;

std::int32_t initializing{};

} // namespace

// @verified - p
void AreaAKA3Init() {
    initializing = 0;
}

// @verified - p
void AreaAKA3Main(std::uint32_t) {
    initializing = 1;
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_AssignLightParams(1, 2);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(1);
    (void)CheckIsArgEquZero(1);
    (void)flddSetPointFlagOnGrid(AREA_AKA3_GRID_BTC1, 0);
    (void)EV_GetShenmueFlag(nightTimeFlag);
    if (initializing == 0)
        EV_LoadAudio("AKA3_000", 0);
    initializing = 0;
}

// @verified - p
void AreaAKA3Cleanup(std::int32_t, std::uint32_t) {
    if (initializing == 0)
        EV_LoadAudio("AKA3_000", 0);
}

const GameplayAreaDefinition areaAKA3GameplayDefinition{
    .areaId = AREA_AKA3,
    .padding04 = 0,
    .init = AreaAKA3Init,
    .main = AreaAKA3Main,
    .cleanup = AreaAKA3Cleanup,
};

} // namespace shenmue::s2::areas
