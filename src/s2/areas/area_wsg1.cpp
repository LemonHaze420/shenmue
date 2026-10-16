#include "s2/areas/area_wsg1.hpp"

#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"

#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t nightTimeFlag = 0xff;

} // namespace

// @verified - p
void AreaWSG1Init() {
}

// @verified - p
void AreaWSG1Main(std::uint32_t) {
    if (EV_GetShenmueFlag(nightTimeFlag)) {
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_LoadAudio("WSG1_000", 1);
    } else {
        EV_SetModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_LoadAudio("WSG1_000", 0);
    }
}

// @verified - p
void AreaWSG1Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetModelDataDefaultVertexShader();
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_LoadAudio("WSG1_000", 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_LoadAudio("WSG1_000", 0);
    }
}

const GameplayAreaDefinition areaWSG1GameplayDefinition{
    .areaId = AREA_WSG1,
    .padding04 = 0,
    .init = AreaWSG1Init,
    .main = AreaWSG1Main,
    .cleanup = AreaWSG1Cleanup,
};

} // namespace shenmue::s2::areas
