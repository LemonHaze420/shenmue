#include "s2/areas/area_qtb4.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/particle_effects.hpp"

namespace shenmue::s2::areas {

std::uint32_t areaQTB4Fire{};

// @verified - p
void AreaQTB4Init() {
    areaQTB4Fire = 0;
}

// @verified - p
void CreateQTB4Fire() {
    areaQTB4Fire = static_cast<std::uint32_t>(CreateFireEffect(QTB4_FIRE_POSITIONS[0], 0.8f, 0.9f, 0, 4));
    SetFireFalloffScalar(areaQTB4Fire, -0.15f);
    for (std::int32_t instance = 1; instance < 4; ++instance)
        SetFirePosition(areaQTB4Fire, instance, &QTB4_FIRE_POSITIONS[instance]);
}

// @verified - p
void AreaQTB4Main(std::uint32_t) {
    CreateQTB4Fire();
    if (EV_GetShenmueFlag(0xff)) {
        EV_ReinitLights(1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        ResumeParticleEffect(4, areaQTB4Fire);
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        SuspendParticleEffect(4, areaQTB4Fire);
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 0);
    }
}

// @verified - p
void AreaQTB4Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_ReinitLights(1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        ResumeParticleEffect(4, areaQTB4Fire);
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        SuspendParticleEffect(4, areaQTB4Fire);
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 0);
    }
}

const GameplayAreaDefinition areaQTB4GameplayDefinition{
    .areaId = AREA_QTB4,
    .padding04 = 0,
    .init = &AreaQTB4Init,
    .main = &AreaQTB4Main,
    .cleanup = &AreaQTB4Cleanup,
};

} // namespace shenmue::s2::areas
