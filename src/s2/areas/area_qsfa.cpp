#include "s2/areas/area_qsfa.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
std::uint32_t fire;
constexpr shen_name fighter{SHEN_NAME(KZN_)}, partner{SHEN_NAME(SFA_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QSFA",
    .centre = {174.39999389648438f, 100.0f, 233.57240295410156f},
    .approachCentre = {240.5f, 100.0f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 27,
    .partnerStyle = 17,
    .formationAssignments = 2,
    .approachRadiusSquared = 42.25f,
    .freeAnnouncerPackage = true};
constexpr Vector3 firePositions[]{
    {164.3f, 101.0f, 225.74001f},
    {182.67f, 101.0f, 233.96001f},
    {174.91f, 101.0f, 243.83f},
    {164.64f, 101.0f, 243.99001f},
};
void BusyCallback(void *) {
    (void)ryoNotBusy();
}
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void CreateFire() {
    fire = static_cast<std::uint32_t>(CreateFireEffect(firePositions[0], 0.9f, 0.9f, 0, 4));
    SetFireFalloffScalar(fire, -0.15000000596046448f);
    for (std::int32_t index = 1; index < 4; ++index)
        SetFirePosition(fire, index, &firePositions[index]);
}
} // namespace
void AreaQSFAInit() {
    fire = 0;
}
void AreaQSFACleanup(std::int32_t night, std::uint32_t) {
    if (night) {
        SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        ResumeParticleEffect(4, fire);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        SuspendParticleEffect(4, fire);
    }
    EV_LoadCutsceneFromCurrentScene("QSFA");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("QSFA_000", 0);
}
void AreaQSFAMain(std::uint32_t) {
    CreateFire();
    HLib::EnqueueTaskWithoutParameter(BusyCallback, 4, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    AreaQSFACleanup(EV_GetShenmueFlag(0xff) != 0, 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQSFAGameplayDefinition{SHEN_NAME(QSFA), 0, AreaQSFAInit, AreaQSFAMain, AreaQSFACleanup};
} // namespace shenmue::s2::areas
