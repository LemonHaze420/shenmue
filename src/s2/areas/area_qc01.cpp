#include "s2/areas/area_qc01.hpp"
#include "s2/fog.hpp"
#include "s2/event_queued.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include <cmath>
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
std::int32_t initializing, helpState;
struct SearchItem {
    shen_name prop, search;
    std::uint16_t item, price;
};
struct SearchShop {
    shen_name area, speaker;
    SearchItem items[6];
};
static_assert(sizeof(SearchItem) == 12 && sizeof(SearchShop) == 80);
constexpr SearchShop shops[]{{SHEN_NAME(WTR0), SHEN_NAME(BBT_),
                                 {{SHEN_NAME(M401), {}, 3024, 300}, {SHEN_NAME(M402), {}, 3011, 200}, {SHEN_NAME(M403), {}, 3016, 200},
                                     {SHEN_NAME(M404), {}, 3020, 100}, {SHEN_NAME(M405), {}, 3019, 100}, {}}},
    {SHEN_NAME(QCAC), SHEN_NAME(JI9_),
        {{SHEN_NAME(M401), {}, 3018, 150}, {SHEN_NAME(M402), {}, 3017, 150}, {SHEN_NAME(M403), {}, 3014, 150}, {SHEN_NAME(M404), {}, 3015, 100}, {}, {}}},
    {SHEN_NAME(QCAD), SHEN_NAME(ZYC_), {{SHEN_NAME(M405), {}, 3023, 500}, {SHEN_NAME(M406), {}, 3012, 250}, {SHEN_NAME(M407), {}, 3013, 200}, {}, {}, {}}}, {}};
void EnableItemsForSearch(shen_name area) {
    for (const auto &shop : shops) {
        if (!shop.area.name)
            return;
        if (shop.area.name != area.name)
            continue;
        for (const auto &item : shop.items) {
            if (!item.prop.name)
                continue;
            const std::int32_t enabled = GetItemState(item.item) ? 0 : 1;
            SetDisplayFlagForProp(item.prop, enabled);
            if (item.search.name)
                srchSetTargetEnabled(item.search, enabled);
        }
        return;
    }
}
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
float NormalizeNearZero(float value) {
    return std::fabs(value) < .00000011920928955078125f ? 0 : value;
}
void UpdateHelp(std::int32_t force) {
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const float x = NormalizeNearZero(NormalizeNearZero(position.x - 177) * NormalizeNearZero(position.x - 167));
    const float z = NormalizeNearZero(NormalizeNearZero(position.z - 142) * NormalizeNearZero(position.z - 137));
    const std::int32_t inside = (std::fabs(x) < .00000011920928955078125f || x < 0) && (std::fabs(z) < .00000011920928955078125f || z < 0);
    const std::int32_t previous = helpState ? helpState == 1 ? 1 : inside ^ 1 : 0;
    if (plymIsSearchBlocked())
        helpState = 2;
    else if (force || previous != inside) {
        helpState = inside ? 1 : 0;
        EV_EventSetUpHelpIcon_0(inside ? 0x39 : 0, 0);
    }
}
void MainCallback(void *) {
    UpdateHelp(0);
}
} // namespace
void AreaQC01Init() {
}
void AreaQC01Cleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(3, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    CheckIsArgEquZero(1);
    SetAreaFogParams();
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("QC01");
        EV_LoadAudio("QC01_000", 0);
    }
    if (!afternoon)
        SetDefaultVSByIndex(5);
    EV_AssignLightParams(1, 5);
}
void AreaQC01Main(std::uint32_t) {
    initializing = 1;
    EnableItemsForSearch(SHEN_NAME(QCAC));
    EnableItemsForSearch(SHEN_NAME(QCAD));
    SetMapControlStatusFlag(0, 0);
    AreaQC01Cleanup(EV_GetShenmueFlag(0xff), 0);
    UpdateHelp(1);
    EV_LoadCutsceneFromCurrentScene("QC01");
    if (!GetSoundProgramPhaseFlag(0) && EV_GetShenmueFlag(0x2a0))
        EV_LoadAudio("QAE1_000.sbt", 0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    initializing = 0;
}
const GameplayAreaDefinition areaQC01GameplayDefinition{SHEN_NAME(QC01), 0, AreaQC01Init, AreaQC01Main, AreaQC01Cleanup};
} // namespace shenmue::s2::areas
