#include "s2/areas/area_qsfd.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
std::uint32_t fire;
constexpr shen_name fighter{SHEN_NAME(SF5_)}, partner{SHEN_NAME(SFA_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QSFD",
    .centre = {137.5f, 100.2249984741211f, 111.56999969482422f},
    .approachCentre = {240.5f, 100.2249984741211f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 29,
    .partnerStyle = 17,
    .formationAssignments = 2,
    .approachRadiusSquared = 30.25f,
    .freeAnnouncerPackage = true};
constexpr Vector3 firePositions[]{
    {130.97999572753906f, 101.0f, 116.84f},
    {131.2100067138672f, 101.0f, 110.35f},
    {133.59f, 101.0f, 105.12999725341797f},
    {142.1300048828125f, 101.0f, 105.23999786376953f},
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
void AreaQSFDInit() {
    fire = 0;
}
void AreaQSFDCleanup(std::int32_t night, std::uint32_t) {
    if (night) {
        SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(1, 1);
        ResumeParticleEffect(4, fire);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        SuspendParticleEffect(4, fire);
    }
    SetNewNpcBatchCountOverride(NpcFillType::FrontFill, night ? 12 : 18);
    EV_LoadCutsceneFromCurrentScene("QSFD");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("QSFD_000", 0);
}
void AreaQSFDMain(std::uint32_t) {
    CreateFire();
    HLib::EnqueueTaskWithoutParameter(BusyCallback, 4, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    const bool night = EV_GetShenmueFlag(0xff) != 0;
    AreaQSFDCleanup(night, 0);
    if (night)
        SetMainMapProjectionDirection(Vector3{-2.0f, 1.0f, 1.7300000190734863f});
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQSFDGameplayDefinition{SHEN_NAME(QSFD), 0, AreaQSFDInit, AreaQSFDMain, AreaQSFDCleanup};
} // namespace shenmue::s2::areas
