#include "s2/areas/area_qf06.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fog.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/particle_effects.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {

std::uint32_t areaQF06Fire{};

// @verified - p
void AreaQF06Init() {
    areaQF06Fire = 0;
}

// @verified - p
void CreateQF06Fire() {
    areaQF06Fire = static_cast<std::uint32_t>(CreateFireEffect(QF06_FIRE_POSITIONS[0], 0.2f, 0.5f, 0, 4));
    SetFirePalette(areaQF06Fire, QF06_FIRE_PALETTE.data());
    SetParticleEffectPosition(4, areaQF06Fire, &QF06_FIRE_POSITIONS[0], &QF06_FIRE_POSITIONS[0]);
    SetFireFalloff(areaQF06Fire, 3.1764705f, 6.7843137f, 0.70588237f);
    SetFireFalloffScalar(areaQF06Fire, -0.29803923f);
    SetParticleEffectSize(4, areaQF06Fire, 0.98039216f, 1.5294118f);
    for (std::int32_t instance = 1; instance < 4; ++instance) {
        SetFirePosition(areaQF06Fire, instance, &QF06_FIRE_POSITIONS[instance]);
        SetFireInstanceSize(areaQF06Fire, instance, 0.98039216f, 1.5294118f);
    }
    SetFireInstanceCameraOffset(areaQF06Fire, 3, 0.15f);
    SetFireInstanceSize(areaQF06Fire, 3, 0.98039216f, 0.9019608f);
    ResumeParticleEffect(4, areaQF06Fire);
}

// @verified - p
void AreaQF06Main(std::uint32_t) {
    CreateQF06Fire();
    SetMapControlStatusFlag(0, 0);
    (void)EV_GetShenmueFlag(0xff);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    SetAreaFogParams();
    ToggleFlagOnFieldEntity(QF06_DOOR, false);
    (void)SetDoorSubjectEnabled(QF06_DOOR, 0);
}

// @verified - p
void AreaQF01Cleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    SetAreaFogParams();
}

const GameplayAreaDefinition areaQF06GameplayDefinition{
    .areaId = AREA_QF06,
    .padding04 = 0,
    .init = &AreaQF06Init,
    .main = &AreaQF06Main,
    .cleanup = &AreaQF01Cleanup,
};

} // namespace shenmue::s2::areas
