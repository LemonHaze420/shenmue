#include "s2/areas/area_wt00.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"

namespace shenmue::s2::areas {

void AreaWT00Init() {
}

void AreaWT00Main(std::uint32_t) {
    std::int32_t sceneIndex;
    if (EV_GetShenmueFlag(0xff)) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(4, 1);
        sceneIndex = 1;
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(4, 0);
        sceneIndex = 0;
    }
    EV_LoadAudio("WT00_000", sceneIndex);
    areaClearAreaEntities();
    flddSetPointFlagOnGrid(WT00_FIELD_POINT, 0);
    SetMainMapMinimumModelHeight(101.0f);
    SetMainMapProjectedShadowDepth(45.0f);
    EV_SetAlignedShadowRange(13.0f);
}

void AreaWT00Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(4, 1);
        EV_LoadAudio("WT00_000", 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(4, 0);
        EV_LoadAudio("WT00_000", 0);
    }
}

const GameplayAreaDefinition areaWT00GameplayDefinition{
    .areaId = SHEN_NAME(WT00),
    .padding04 = 0,
    .init = &AreaWT00Init,
    .main = &AreaWT00Main,
    .cleanup = &AreaWT00Cleanup,
};

} // namespace shenmue::s2::areas
