#include "s2/areas/area_qd01.hpp"
#include "s2/fog.hpp"
#include "s2/event_queued.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(OXO_)}, partner{SHEN_NAME(SFA_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
void PrepareParentedActors();
StreetWatchState street{.package = "QSFF",
    .centre = {197, 100, 129},
    .approachCentre = {197, 100, 129},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 28,
    .partnerStyle = 47,
    .formationAssignments = 2,
    .approachRadiusSquared = 36,
    .prepareParentedActors = PrepareParentedActors};
constexpr MagicRoomDescriptor roomDescriptors[]{
    {{{172.10000610351562f, 86.4000015258789f}, {172.10000610351562f, 81.0f}, {162.89999389648438f, 81.0f}, {162.89999389648438f, 86.4000015258789f}},
        {166.5f, 83.69999694824219f}, {5.400000095367432f, 7.199999809265137f}, 16385, 0, 0},
    {{{172.10000610351562f, 81.0f}, {172.10000610351562f, 75.5999984741211f}, {162.89999389648438f, 75.5999984741211f}, {162.89999389648438f, 81.0f}},
        {166.5f, 78.30000305175781f}, {5.400000095367432f, 7.199999809265137f}, 16385, 0, 0},
    {{{171.6999969482422f, 75.5999984741211f}, {171.6999969482422f, 81.0f}, {180.89999389648438f, 81.0f}, {180.89999389648438f, 75.5999984741211f}},
        {177.3000030517578f, 78.30000305175781f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0},
    {{{171.6999969482422f, 81.0f}, {171.6999969482422f, 86.4000015258789f}, {180.89999389648438f, 86.4000015258789f}, {180.89999389648438f, 81.0f}},
        {177.3000030517578f, 83.69999694824219f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0}};
constexpr MagicRoomFloorGroup roomGroups[]{{4, 63, {100.0f, 103.30000305175781f}, roomDescriptors}, {}};
void PrepareParentedActors() {
    if (((g_currentMap.name - SHEN_NAME(QC01)) & 0xfffffeffU) != 0)
        return;
    const shen_name centre{SHEN_NAME(CNT1)}, invalid{0xffffffffU};
    EVT_AT_SetPos(centre, invalid, {197, 100.5f, 129}, 0);
    EVT_AT_SetAngY(centre, invalid, 0, 0);
    EVT_AT_SetPos(fighter, centre, {0, 0, -.28999999165534973f}, 0);
    EVT_AT_SetPos(partner, centre, {.04650000110268593f, 0, -1.13264000415802f}, 0);
    EVT_AT_SetAngY(fighter, centre, 0, 0);
    EVT_AT_SetAngY(partner, centre, 0, 0);
    EVT_AT_SetMot(fighter, 0x1acd, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(partner, 0x1ac9, 0, 1, 0x40000000U, 0);
}
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void MainCallback(void *) {
}
} // namespace
void AreaQD01Init() {
}
void AreaQD01Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(3, 0);
    EV_SetWeatherInstruction(0x780, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    CheckIsArgEquZero(1);
    SetAreaFogParams();
    if (!afternoon)
        SetDefaultVSByIndex(5);
    EV_AssignLightParams(1, 5);
}
void AreaQD01Main(std::uint32_t) {
    SetMapControlStatusFlag(0, 0);
    DateTime date;
    GetCurrentDateTime(date);
    AreaQD01Cleanup(EV_GetShenmueFlag(0xff), 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    QueueMagicRoom("scene/03/stream", "ROOM.afs", roomGroups, 0xd000);
}
const GameplayAreaDefinition areaQD01GameplayDefinition{SHEN_NAME(QD01), 0, AreaQD01Init, AreaQD01Main, AreaQD01Cleanup};
} // namespace shenmue::s2::areas
