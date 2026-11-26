#include "s2/events/event_api.h"
#include "s2/areas/area_qf06.hpp"
#if defined(SHENMUE_EVENT_OVERLAY)
int areaQF06Reserved;
static unsigned int areaQF06Fire;
void CreateQF06Fire(void);
void AreaCleanup(int afternoon);
void AreaQF01LoadAudio(int afternoon);
#else
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

#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQF06Main(std::uint32_t) {
#endif
    CreateQF06Fire();
    FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    (void)EV_GetShenmueFlag(0xff);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    SetAreaFogParams();
#endif
    FTBL.ToggleFlagOnFieldEntity(QF06_DOOR, 0);
    (void)FTBL.SetDoorSubjectEnabled(QF06_DOOR, 0);
}

#include "qf_lighting.inc"

// @verified - p
void CreateQF06Fire(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    int instance;
    areaQF06Fire = FTBL.CreateFireEffect(&QF06_FIRE_POSITIONS[0], 0.2f, 0.5f, 0, 4);
#else
    areaQF06Fire = static_cast<std::uint32_t>(FTBL.CreateFireEffect(&QF06_FIRE_POSITIONS[0], 0.2f, 0.5f, 0, 4));
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetFirePalette(areaQF06Fire, QF06_FIRE_PALETTE);
#else
    FTBL.SetFirePalette(areaQF06Fire, QF06_FIRE_PALETTE.data());
#endif
    FTBL.SetParticleEffectPosition(4, areaQF06Fire, &QF06_FIRE_POSITIONS[0], &QF06_FIRE_POSITIONS[0]);
    FTBL.SetFireFalloff(areaQF06Fire, 3.1764705f, 6.7843137f, 0.70588237f);
    FTBL.SetFireFalloffScalar(areaQF06Fire, -0.29803923f);
    FTBL.SetParticleEffectSize(4, areaQF06Fire, 0.98039216f, 1.5294118f);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (instance = 1; instance < 4; ++instance) {
#else
    for (std::int32_t instance = 1; instance < 4; ++instance) {
#endif
        FTBL.SetFirePosition(areaQF06Fire, instance, &QF06_FIRE_POSITIONS[instance]);
        FTBL.SetFireInstanceSize(areaQF06Fire, instance, 0.98039216f, 1.5294118f);
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    SetFireInstanceCameraOffset(areaQF06Fire, 3, 0.15f);
    FTBL.SetFireInstanceSize(areaQF06Fire, 3, 0.98039216f, 0.9019608f);
#endif
    FTBL.ResumeParticleEffect(4, areaQF06Fire);
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQF06GameplayDefinition{
    .areaId = AREA_QF06,
    .padding04 = 0,
    .init = &AreaQF06Init,
    .main = &AreaQF06Main,
    .cleanup = &AreaQF01Cleanup,
};

} // namespace shenmue::s2::areas
#endif
