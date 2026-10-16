#include "s2/debug.hpp"
#include "s2/areas/area_qtb2.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include <cstring>

namespace shenmue::s2::areas {
namespace {
std::int32_t qtb2Initializing{};
void ApplyQTB2Lighting(std::int32_t afternoon) {
    std::int32_t enabled;
    if (afternoon) {
        EV_ReinitLights(1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        enabled = 1;
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        enabled = 0;
    }
    EV_SetObjectFlagByIndex(1, enabled);
    if (!qtb2Initializing) {
        char audio[16]{};
        std::memcpy(audio, &g_currentMap.name, 4);
        std::memcpy(audio + 4, "_000", 4);
        EV_LoadAudio(audio, enabled);
    }
}
} // namespace
void AreaQTB2Init() {
    qtb2Initializing = 0;
}
void AreaQTB2Main(std::uint32_t) {
    qtb2Initializing = 1;
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    ApplyQTB2Lighting(EV_GetShenmueFlag(0xff));
    qtb2Initializing = 0;
}
void AreaQTB2Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyQTB2Lighting(afternoon);
}
const GameplayAreaDefinition areaQTB2GameplayDefinition{SHEN_NAME(QTB2), 0, AreaQTB2Init, AreaQTB2Main, AreaQTB2Cleanup};
} // namespace shenmue::s2::areas
