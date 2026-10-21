#include "s2/areas/area_qtb1.hpp"
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
constexpr shen_name fighter{SHEN_NAME(KFT_)}, partner{SHEN_NAME(SFB_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QSFE",
    .centre = {203.16969299316406f, 100.0f, 246.56700134277344f},
    .approachCentre = {240.5f, 100.0f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 30,
    .partnerStyle = 17,
    .formationAssignments = 2,
    .approachRadiusSquared = 36.0f,
    .freeAnnouncerPackage = true};
constexpr Vector3 firePositions[]{
    {206.87f, 101.0f, 240.25f},
    {208.14f, 101.0f, 250.74001f},
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
    for (std::int32_t index = 1; index < 2; ++index)
        SetFirePosition(fire, index, &firePositions[index]);
}
} // namespace
void AreaQTB1Init() {
    fire = 0;
}
void AreaQTB1Cleanup(std::int32_t night, std::uint32_t) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(1, 1);
        ResumeParticleEffect(4, fire);
        SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        SuspendParticleEffect(4, fire);
    }
    SetNewNpcBatchCountOverride(NpcFillType::FrontFill, night ? 10 : 15);
    EV_LoadCutsceneFromCurrentScene("QTB1");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("QTB1_000", 0);
}
void AreaQTB1Main(std::uint32_t) {
    CreateFire();
    HLib::EnqueueTaskWithoutParameter(BusyCallback, 4, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    AreaQTB1Cleanup(EV_GetShenmueFlag(0xff) != 0, 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQTB1GameplayDefinition{SHEN_NAME(QTB1), 0, AreaQTB1Init, AreaQTB1Main, AreaQTB1Cleanup};
} // namespace shenmue::s2::areas
