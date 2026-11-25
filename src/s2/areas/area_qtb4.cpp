#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#define QTB4_AUDIO_BANK "QTB4_000"
static unsigned int areaQTB4Fire;
void CreateQTB4Fire(void);
void AreaCleanup(int afternoon);
void LoadQTB4Audio(int afternoon);
#else
#include "area_qtb4.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/particle_effects.hpp"
namespace shenmue::s2::areas {
std::uint32_t areaQTB4Fire{};
// @verified - p
void AreaQTB4Init() {
    areaQTB4Fire = 0;
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    CreateQTB4Fire();
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
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
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQTB4Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    if (afternoon) {
        FTBL.ReinitializeLights(1);
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        (void)FTBL.CheckIsArgEquZero(1, 1);
        FTBL.ResumeParticleEffect(4, areaQTB4Fire);
#if !defined(SHENMUE_EVENT_OVERLAY)
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 1);
#endif
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SuspendParticleEffect(4, areaQTB4Fire);
#if !defined(SHENMUE_EVENT_OVERLAY)
        (void)EV_LoadAudio(QTB4_AUDIO_BANK, 0);
#endif
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    LoadQTB4Audio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void LoadQTB4Audio(int afternoon) {
    if (afternoon)
        FTBL.EV_LoadAudio(QTB4_AUDIO_BANK, 1);
    else
        FTBL.EV_LoadAudio(QTB4_AUDIO_BANK, 0);
}
#endif

// @verified - p
void CreateQTB4Fire(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 QTB4_FIRE_POSITIONS[4];
    int instance;
#define QTB4_FIRE_POSITION(index, posX, posY, posZ) \
    QTB4_FIRE_POSITIONS[index].x = posX; \
    QTB4_FIRE_POSITIONS[index].z = posZ;
#include "qtb4_fire_positions.def"
#undef QTB4_FIRE_POSITION
    QTB4_FIRE_POSITIONS[0].y = QTB4_FIRE_POSITIONS[1].y = QTB4_FIRE_POSITIONS[2].y = QTB4_FIRE_POSITIONS[3].y = 100.7f;
    areaQTB4Fire = FTBL.CreateFireEffect(&QTB4_FIRE_POSITIONS[0], 0.8f, 0.9f, 0, 4);
#else
    areaQTB4Fire = static_cast<std::uint32_t>(CreateFireEffect(QTB4_FIRE_POSITIONS[0], 0.8f, 0.9f, 0, 4));
#endif
    FTBL.SetFireFalloffScalar(areaQTB4Fire, -0.15f);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (instance = 1; instance < 4; ++instance)
#else
    for (std::int32_t instance = 1; instance < 4; ++instance)
#endif
        FTBL.SetFirePosition(areaQTB4Fire, instance, &QTB4_FIRE_POSITIONS[instance]);
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQTB4GameplayDefinition{
    .areaId = AREA_QTB4,
    .padding04 = 0,
    .init = &AreaQTB4Init,
    .main = &AreaQTB4Main,
    .cleanup = &AreaQTB4Cleanup,
};
} // namespace shenmue::s2::areas
#endif
