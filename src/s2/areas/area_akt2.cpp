#include "s2/debug.hpp"
#include "s2/areas/area_akt2.hpp"
#include "s2/areas/area_akt0.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/sound_program.hpp"
#include "s2/lighting.hpp"

#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

namespace {

constexpr std::int32_t disableGlobalLightingMask = 0x0f;
constexpr std::int32_t disableFogEffectMask = 0x780;
constexpr std::int32_t nightTimeFlag = 0xff;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name qtb1{SHEN_NAME(QTB1)};
constexpr shen_name qtb3{SHEN_NAME(QTB3)};
constexpr std::int32_t eventTaskToken = static_cast<std::int32_t>(SHEN_NAME(EVNT));

void AKT2PostUpdateCallback(void *) {
}

bool UsesQTBLightOverride() {
    return g_currentMap.name == qtb1.name || g_currentMap.name == qtb3.name;
}

} // namespace

void AreaAKT2Init() {
}

void AreaAKT2Main(std::uint32_t) {
    SetMapControlStatusFlag(0, false);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_SetWeatherInstruction(disableFogEffectMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(defaultLight);
    AssignLightProgram(0);
    (void)CheckIsArgEquZero(1);
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(AKT2PostUpdateCallback, 4, 0x0b, eventTaskToken);

    if (EV_GetShenmueFlag(nightTimeFlag)) {
        if (UsesQTBLightOverride()) {
            EV_lightLoad(defaultLight);
            PushGlobalLightingProgram();
            (void)CheckIsArgEquZero(1);
        }
    } else if (UsesQTBLightOverride()) {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }

    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
}

void AreaAKT2Cleanup(std::int32_t afternoon, std::uint32_t) {
    AreaAKTSharedCleanup(afternoon);
}

const GameplayAreaDefinition areaAKT2GameplayDefinition{SHEN_NAME(AKT2), 0, AreaAKT2Init, AreaAKT2Main, AreaAKT2Cleanup};

} // namespace shenmue::s2::areas
