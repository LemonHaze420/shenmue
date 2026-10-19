#include "s2/areas/area_ar03.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/pati.hpp"
#include "s2/lighting.hpp"

#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t alternateAudioFlag = 0x84;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr std::int32_t waveTaskToken = static_cast<std::int32_t>(SHEN_NAME(wave));

std::int32_t initFlag{};

void AR03WaveCallback(void *) {
    SetTextureAnimationUV(0.0f, -0.00079999998f);
    ApplyTextureAnimationResource(4);
}

void ApplyAR03LightingAndAudio(std::int32_t afternoon) {
    if (afternoon) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        SetupLightConfig();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(5, 1);
        EV_SetObjectFlagByIndex(6, 0);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(5, 0);
        EV_SetObjectFlagByIndex(6, 1);
    }

    if (initFlag != 0)
        return;
    if (EV_GetShenmueFlag(alternateAudioFlag)) {
        EV_LoadAudio("AR03_000", 5);
    } else if (afternoon) {
        EV_LoadAudio("AR03_000", 1);
    } else {
        EV_LoadAudio("AR03_000", 0);
    }
}

} // namespace

void AreaAR03Init() {
    initFlag = 0;
}

void AreaAR03Main(std::uint32_t) {
    initFlag = 1;
    ApplyAR03LightingAndAudio(EV_GetShenmueFlag(nightTimeFlag));
    InitPati();
    areaClearAreaEntities();
    HLib::EnqueueTaskWithoutParameter(AR03WaveCallback, 3, 2, waveTaskToken);
    initFlag = 0;
}

void AreaAR03Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyAR03LightingAndAudio(afternoon);
}
const GameplayAreaDefinition areaAR03GameplayDefinition{SHEN_NAME(AR03), 0, AreaAR03Init, AreaAR03Main, AreaAR03Cleanup};

} // namespace shenmue::s2::areas
