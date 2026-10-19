#include "s2/debug.hpp"
#include "s2/areas/area_qr00.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/npc_package.hpp"
#include "s2/pati.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t initializing{};
void QR00NativeNullsub(void *) {
}
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    if (!initializing)
        EV_LoadAudio("QR00_000", night ? 1 : 0);
}
} // namespace
void AreaQR00Init() {
    initializing = 0;
}
void AreaQR00Main(std::uint32_t) {
    initializing = 1;
    HLib::EnqueueTaskWithoutParameter(QR00NativeNullsub, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    InitPati();
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    if (EV_GetShenmueFlag(0x488))
        SetNpcColdResetMode(0);
    SetScenarioCharacterRadius(SHEN_NAME(gat1), 10.0f);
    SetScenarioCharacterRadius(SHEN_NAME(gat2), 10.0f);
    initializing = 0;
}
void AreaQR00Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaQR00GameplayDefinition{SHEN_NAME(QR00), 0, AreaQR00Init, AreaQR00Main, AreaQR00Cleanup};
} // namespace shenmue::s2::areas
