#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
enum {
    disableAreaWeatherMask = 0x780,
    disableGlobalLightingMask = 0x0c
};
void AreaCleanup(int afternoon);
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaARM0Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime dateTime;
#endif
    FTBL.EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    FTBL.AssignLightParams(1, 1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.SetupLightConfig();
#if defined(SHENMUE_EVENT_OVERLAY)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
#else
    if (FTBL.EV_GetShenmueFlag(0xff))
#endif
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    (void)EV_GetShenmueFlag(nightTimeFlag);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaARM0Cleanup(std::int32_t, std::uint32_t) {
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaARM0GameplayDefinition{
    .areaId = AREA_ARM0,
    .padding04 = 0,
    .init = AreaARM0Init,
    .main = AreaARM0Main,
    .cleanup = AreaARM0Cleanup,
};

} // namespace shenmue::s2::areas
#endif
