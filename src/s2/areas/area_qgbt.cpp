#include "s2/areas/area_qgbt.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/lighting.hpp"
#include "s2/math.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(SF6_)}, partner{SHEN_NAME(SFA_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QGBT",
    .centre = {371.7799987792969f, 76.05000305175781f, 120.77999877929688f},
    .approachCentre = {240.5f, 76.05000305175781f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 24,
    .partnerStyle = 2,
    .approachRadiusSquared = 45.5625f};
std::int32_t preAuthorAdmitted;
std::int32_t preAuthorBusy;
StreetFightStartParameters preAuthor;
constexpr Vector3 preAuthorPosition{339.0400085449219f, 82.27999877929688f, 120.70999908447266f};
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void PreAuthorCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234))
        return;
    preAuthor.area = SHEN_NAME(SFWF);
    preAuthor.scene = 99;
    SetPlayerControlEnabled(0);
    preAuthor.reserved18 = 0;
    preAuthor.flags = 0;
    preAuthor.signal = &preAuthorBusy;
    if (GetFightModeFlags() & 8)
        preAuthor.flags |= 0x10U;
    preAuthor.flags |= 1U;
    LoadAreaFunc_Wrapper_0("SFWF.EV3", 0);
    ExecuteCurrAreaMainFunc("SFWF.EV3", reinterpret_cast<std::uintptr_t>(&preAuthor));
    HLib::DestroyCurrentTask();
}
void TryPreAuthor() {
    if (preAuthorAdmitted || !EV_GetShenmueFlag(294) || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2220))
        return;
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (!(GetSquaredDistanceXZ(&preAuthorPosition, &position) < 9.0f))
        return;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    preAuthorBusy = 1;
    preAuthorAdmitted = 1;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2220, 1);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(PreAuthorCallback, 5, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    HLib::SetupTimedTask(task, 30);
}
void MainCallback(void *) {
    if (ryoNotBusy())
        TryPreAuthor();
}
} // namespace
void AreaQGBTInit() {
    preAuthorAdmitted = 0;
    preAuthor = {};
    preAuthorBusy = 0;
}
void AreaQGBTCleanup(std::int32_t night, std::uint32_t) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
        SetPluralControlFor(FindCharacterByScenarioName(fighter), 1);
        SetPluralControlFor(FindCharacterByScenarioName(partner), 1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
}
void AreaQGBTMain(std::uint32_t) {
    preAuthorAdmitted = 0;
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    AreaQGBTCleanup(EV_GetShenmueFlag(0xff) != 0, 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQGBTGameplayDefinition{SHEN_NAME(QGBT), 0, AreaQGBTInit, AreaQGBTMain, AreaQGBTCleanup};
} // namespace shenmue::s2::areas
