#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/areas/area_pigeons.hpp"
static AreaPigeon qa11FirstPigeons[10];
static AreaPigeon qa11SecondPigeons[10];
void AreaCleanup(int afternoon);
#else
#include "s2/areas/area_qa11.hpp"
#include "s2/areas/area_pigeons.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

void AreaQA11Init() {
}

#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    EventDateTime dateTime;
    Vector3 firstOrigin = {232.0f, 139.70001f, 221.0f};
    Vector3 secondOrigin = {251.0f, 139.70001f, 200.0f};
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
void AreaQA11Main(std::uint32_t) {
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
    if (EV_GetShenmueFlag(0xff)) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
#endif
    FTBL.SetMapControlStatusFlag(0, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    QueueAreaPigeons(&firstOrigin, 4.5f, qa11FirstPigeons, SHEN_NAME4('h', 'a', 't', '0'), 10);
    QueueAreaPigeons(&secondOrigin, 3.5f, qa11SecondPigeons, SHEN_NAME4('h', 'a', 't', 'A'), 10);
#else
    QueueAreaPigeons(&QA11_FIRST_PIGEON_ORIGIN, 4.5f, areaPigeons3.data(), QA11_FIRST_PIGEON);
    QueueAreaPigeons(&QA11_SECOND_PIGEON_ORIGIN, 3.5f, areaPigeons2.data(), QA11_SECOND_PIGEON);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQA11Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    if (afternoon) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        (void)FTBL.CheckIsArgEquZero(1, 1);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQA11GameplayDefinition{
    .areaId = SHEN_NAME(QA11),
    .padding04 = 0,
    .init = &AreaQA11Init,
    .main = &AreaQA11Main,
    .cleanup = &AreaQA11Cleanup,
};

} // namespace shenmue::s2::areas

#endif
