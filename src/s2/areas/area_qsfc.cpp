#include "s2/areas/area_qsfc.hpp"
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
constexpr shen_name fighter{SHEN_NAME(AW1_)}, partner{SHEN_NAME(SFA_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QSFC",
    .centre = {268.20001220703125f, 94.83000183105469f, 42.06999969482422f},
    .approachCentre = {240.5f, 94.83000183105469f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 31,
    .partnerStyle = 17,
    .formationAssignments = 1,
    .approachRadiusSquared = 56.25f,
    .freeAnnouncerPackage = true};
constexpr Vector3 firePositions[]{
    {267.0899963378906f, 97.08000183105469f, 59.279999f},
    {267.0899963378906f, 97.08000183105469f, 63.84f},
    {267.0899963378906f, 97.08000183105469f, 68.449997f},
    {270.9599914550781f, 96.5999984741211f, 50.86000061035156f},
    {260.04998779296875f, 96.41999816894531f, 44.38999938964844f},
    {265.510009765625f, 95.94999694824219f, 32.599998474121094f},
    {271.55999755859375f, 96.19999694824219f, 33.70000076293945f},
    {276.78f, 96.269997f, 39.369999f},
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
    fire = static_cast<std::uint32_t>(CreateFireEffect(firePositions[0], 0.3f, 0.6f, 0, 4));
    SetFireFalloffScalar(fire, -0.15000000596046448f);
    SetFirePosition(fire, 0, &firePositions[1]);
    SetFirePosition(fire, 1, &firePositions[2]);
    for (std::int32_t index = 3; index < 8; ++index) {
        SetFirePosition(fire, index, &firePositions[index]);
        SetFireInstanceSize(fire, index, 0.8999999761581421f, 0.8999999761581421f);
    }
}
} // namespace
void AreaQSFCInit() {
    fire = 0;
}
void AreaQSFCCleanup(std::int32_t night, std::uint32_t) {
    if (night) {
        SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        SuspendParticleEffect(4, fire);
        EV_SetObjectFlagByIndex(1, 1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        ResumeParticleEffect(4, fire);
        EV_SetObjectFlagByIndex(1, 0);
    }
    SetNewNpcBatchCountOverride(NpcFillType::FrontFill, night ? 10 : 15);
    EV_LoadCutsceneFromCurrentScene("QSFC");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("QSFC_000", 0);
}
void AreaQSFCMain(std::uint32_t) {
    CreateFire();
    HLib::EnqueueTaskWithoutParameter(BusyCallback, 4, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    AreaQSFCCleanup(EV_GetShenmueFlag(0xff) != 0, 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQSFCGameplayDefinition{SHEN_NAME(QSFC), 0, AreaQSFCInit, AreaQSFCMain, AreaQSFCCleanup};
} // namespace shenmue::s2::areas
