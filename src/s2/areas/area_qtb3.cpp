#include "s2/areas/area_qtb3.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/pati.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t initializing{};
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        SetupLightConfig();
        EV_ReinitLights(1);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
    }
    if (!initializing)
        EV_LoadAudio("QTB3_000", night ? 1 : 0);
}
} // namespace
void AreaQTB3Init() {
    initializing = 0;
}
void AreaQTB3Main(std::uint32_t) {
    initializing = 1;
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    InitPati();
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    initializing = 0;
}
void AreaQTB3Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaQTB3GameplayDefinition{SHEN_NAME(QTB3), 0, AreaQTB3Init, AreaQTB3Main, AreaQTB3Cleanup};
} // namespace shenmue::s2::areas
