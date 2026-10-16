#include "s2/debug.hpp"
#include "s2/areas/area_wr00.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"

namespace shenmue::s2::areas {
namespace {
void ApplyWR00Lighting(std::int32_t afternoon) {
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(7, 1);
        EV_LoadAudio("WR00_000", 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(7, 0);
        EV_LoadAudio("WR00_000", 0);
    }
}
} // namespace
void AreaWR00Init() {
}
void AreaWR00Main(std::uint32_t) {
    ApplyWR00Lighting(EV_GetShenmueFlag(0xff));
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    areaClearAreaEntities();
    SetMainMapProjectedShadowDepth(45.0f);
    EV_SetAlignedShadowRange(7.5f);
}
void AreaWR00Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyWR00Lighting(afternoon);
}
const GameplayAreaDefinition areaWR00GameplayDefinition{
    .areaId = SHEN_NAME(WR00),
    .padding04 = 0,
    .init = AreaWR00Init,
    .main = AreaWR00Main,
    .cleanup = AreaWR00Cleanup,
};
} // namespace shenmue::s2::areas
