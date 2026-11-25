#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
enum {
    disableGlobalLightingMask = 0x0c,
    disableAreaWeatherMask = 0x780,
    AREA_AKA3_GRID_BTC1 = SHEN_NAME4('B','T','C','1')
};
void AreaCleanup(int afternoon);
void AreaAKA3LoadAudio(int afternoon);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static int initializing;
#endif
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaAKA3Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime dateTime;
#else
    initializing = 1;
#endif
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    FTBL.EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    FTBL.AssignLightParams(1, 2);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.AssignLightProgram(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.CheckIsArgEquZero(1, 1);
#else
    (void)CheckIsArgEquZero(1);
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    (void)FTBL.flddSetPointFlagOnGrid(AREA_AKA3_GRID_BTC1, 0);
#endif
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
    if (initializing == 0)
        EV_LoadAudio("AKA3_000", 0);
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    initializing = 0;
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaAKA3Cleanup(std::int32_t, std::uint32_t) {
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (initializing == 0)
#if defined(SHENMUE_EVENT_OVERLAY)
        AreaAKA3LoadAudio(afternoon);
#else
        EV_LoadAudio("AKA3_000", 0);
#endif
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaAKA3LoadAudio(int afternoon) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (afternoon)
        FTBL.EV_LoadAudio("WE00_000", 1);
    else
        FTBL.EV_LoadAudio("WE00_000", 0);
#else
    FTBL.EV_LoadAudio("AKA3_000", 0);
#endif
}
#else
const GameplayAreaDefinition areaAKA3GameplayDefinition{
    .areaId = AREA_AKA3,
    .padding04 = 0,
    .init = AreaAKA3Init,
    .main = AreaAKA3Main,
    .cleanup = AreaAKA3Cleanup,
};

} // namespace shenmue::s2::areas
#endif
