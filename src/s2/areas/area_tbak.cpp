#include "s2/debug.hpp"
#include "s2/areas/area_tbak.hpp"
#include "s2/areas/area_akt0.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/lighting.hpp"
#include "s2/sound_program.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
namespace {
void TBAKMainLoopNullsub(void *) {
}
bool UsesQTBLighting() {
    return g_currentMap == AREA_QTB1 || g_currentMap.name == SHEN_NAME(QTB3);
}
} // namespace
void AreaTBAKInit() {
}
void AreaTBAKMain(std::uint32_t) {
    if (!UsesQTBLighting()) {
        SetMapControlStatusFlag(0, false);
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x1800, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    }
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(TBAKMainLoopNullsub, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    AreaAKTSharedCleanup(EV_GetShenmueFlag(0xff));
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
}
void AreaTBAKCleanup(std::int32_t afternoon, std::uint32_t) {
    AreaAKTSharedCleanup(afternoon);
}
const GameplayAreaDefinition areaTBAKGameplayDefinition{SHEN_NAME(TBAK), 0, AreaTBAKInit, AreaTBAKMain, AreaTBAKCleanup};
} // namespace shenmue::s2::areas
