#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
enum { nightTimeFlag = 0xff };
void AreaCleanup(int afternoon);
void LoadWSG1Audio(int afternoon);
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    if (FTBL.EV_GetShenmueFlag(nightTimeFlag))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
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
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaWSG1Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    FTBL.SetMapModelDataDefaultVertexShader();
    if (afternoon) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        (void)FTBL.CheckIsArgEquZero(1, 1);
#if !defined(SHENMUE_EVENT_OVERLAY)
        EV_LoadAudio("WSG1_000", 1);
#endif
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
#if !defined(SHENMUE_EVENT_OVERLAY)
        EV_LoadAudio("WSG1_000", 0);
#endif
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    LoadWSG1Audio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void LoadWSG1Audio(int afternoon) {
    if (afternoon)
        FTBL.EV_LoadAudio("WSG1_000", 1);
    else
        FTBL.EV_LoadAudio("WSG1_000", 0);
}
#else
const GameplayAreaDefinition areaWSG1GameplayDefinition{
    .areaId = AREA_WSG1,
    .padding04 = 0,
    .init = AreaWSG1Init,
    .main = AreaWSG1Main,
    .cleanup = AreaWSG1Cleanup,
};
} // namespace shenmue::s2::areas
#endif
