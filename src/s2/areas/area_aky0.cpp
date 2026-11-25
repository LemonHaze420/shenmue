#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
enum {
    disableGlobalLightingMask = 0x0c,
    disableAreaWeatherMask = 0x780
};
void AreaCleanup(int afternoon);
void AreaAKY0LoadAudio(int afternoon);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static int initFlag;
#endif
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaAKY0Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime dateTime;
#endif
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    FTBL.EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    FTBL.AssignLightParams(1, 3);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.AssignLightProgram(1);
    FTBL.AssignLightProgram(2);
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
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    initFlag = 0;
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    AreaAKY0LoadAudio(afternoon);
#else
    if (initFlag == 0)
        AreaAKY0LoadAudio(afternoon);
#endif
#else
void AreaAKY0Cleanup(std::int32_t, std::uint32_t) {
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaAKY0LoadAudio(int afternoon) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (afternoon)
        FTBL.EV_LoadAudio("WE00_000", 1);
    else
        FTBL.EV_LoadAudio("WE00_000", 0);
#else
    FTBL.EV_LoadCutsceneFromCurrentScene("AKY0");
    FTBL.EV_LoadAudio("AKY0_000", 0);
#endif
}
#else
const GameplayAreaDefinition areaAKY0GameplayDefinition{
    .areaId = AREA_AKY0,
    .padding04 = 0,
    .init = AreaAKY0Init,
    .main = AreaAKY0Main,
    .cleanup = AreaAKY0Cleanup,
};

} // namespace shenmue::s2::areas
#endif
