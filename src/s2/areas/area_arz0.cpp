#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
enum {
    disableAreaWeatherMask = 0x780,
    disableGlobalLightingMask = 0x0c
};
void AreaCleanup(int afternoon);
void AreaARZ0CleanupNullsub(int afternoon);
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaARZ0Main(std::uint32_t) {
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    (void)EV_GetShenmueFlag(nightTimeFlag);
    EV_SetModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaARZ0Cleanup(std::int32_t, std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (afternoon) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.SetObjectFlagByIndex(9, 1);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetObjectFlagByIndex(9, 0);
    }
#else
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.SetDefaultVS();
    FTBL.AssignLightParams(1, 1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.SetupLightConfig();
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaARZ0CleanupNullsub(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaARZ0CleanupNullsub(int afternoon) {
}
#else
const GameplayAreaDefinition areaARZ0GameplayDefinition{
    .areaId = AREA_ARZ0,
    .padding04 = 0,
    .init = AreaARZ0Init,
    .main = AreaARZ0Main,
    .cleanup = AreaARZ0Cleanup,
};

} // namespace shenmue::s2::areas
#endif
