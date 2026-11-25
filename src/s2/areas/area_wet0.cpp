#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
enum {
    disableAreaWeatherMask = 0x780,
    disableGlobalLightingMask = 0x0c
};
void AreaCleanup(int afternoon);
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaWET0Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    (void)EV_GetShenmueFlag(nightTimeFlag);
#endif
    FTBL.EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    FTBL.AssignLightParams(1, 1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.SetupLightConfig();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.CheckIsArgEquZero(1, 1);
#else
    (void)CheckIsArgEquZero(1);
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.SetNewWindPhaseAndSpeed(0, 0.0f);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaWET0Cleanup(std::int32_t, std::uint32_t) {
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaWET0GameplayDefinition{
    .areaId = AREA_WET0,
    .padding04 = 0,
    .init = AreaWET0Init,
    .main = AreaWET0Main,
    .cleanup = AreaWET0Cleanup,
};

} // namespace shenmue::s2::areas
#endif
