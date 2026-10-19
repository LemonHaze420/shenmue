#include "s2/areas/area_qa11.hpp"
#include "s2/areas/area_pigeons.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

void AreaQA11Init() {
}

void AreaQA11Main(std::uint32_t) {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (EV_GetShenmueFlag(0xff)) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    SetMapControlStatusFlag(0, 1);
    QueueAreaPigeons(&QA11_FIRST_PIGEON_ORIGIN, 4.5f, areaPigeons3.data(), QA11_FIRST_PIGEON);
    QueueAreaPigeons(&QA11_SECOND_PIGEON_ORIGIN, 3.5f, areaPigeons2.data(), QA11_SECOND_PIGEON);
}

void AreaQA11Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
}

const GameplayAreaDefinition areaQA11GameplayDefinition{
    .areaId = SHEN_NAME(QA11),
    .padding04 = 0,
    .init = &AreaQA11Init,
    .main = &AreaQA11Main,
    .cleanup = &AreaQA11Cleanup,
};

} // namespace shenmue::s2::areas
