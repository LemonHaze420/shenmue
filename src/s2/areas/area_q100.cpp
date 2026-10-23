#include "s2/debug.hpp"
#include "s2/areas/area_q100.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/npc.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pati.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(OXO_)}, partner{SHEN_NAME(SFA_)}, secondFighter{SHEN_NAME(KZN_)}, secondPartner{SHEN_NAME(SFB_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, secondFighter, secondPartner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, secondFighter, secondPartner, shen_name{0xffffffffU}};
constexpr Vector3 secondCentre{174.39999389648438f, 100, 233.57240295410156f};
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
    .approachRadiusSquared = 49,
    .freeAnnouncerPackage = true};
std::int32_t initializing;
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void PrepareActors(StreetWatchWork &work) {
    const shen_name invalid{0xffffffffU};
    if (CanPerformForCurrentAreaFlags() == 1) {
        PositionStreetWatchAnnouncer();
        EVT_AT_SetMot(STREET_WATCH_ANNOUNCER, 0x25c3, 0, 1, 0, 0);
        EnableScenarioCharacterDisplay(STREET_WATCH_ANNOUNCER);
        EnsureScenarioCharacterDraw(STREET_WATCH_ANNOUNCER);
    } else if (DoesScenarioCharacterExist(STREET_WATCH_ANNOUNCER)) {
        DisableScenarioCharacterDisplay(STREET_WATCH_ANNOUNCER);
        ToggleScenarioCharacterFlag(STREET_WATCH_ANNOUNCER);
    }
    EnableScenarioCharacterDisplay(fighter);
    EnsureScenarioCharacterDraw(fighter);
    EnableScenarioCharacterDisplay(partner);
    EnsureScenarioCharacterDraw(partner);
    EVT_AT_SetPos(fighter, invalid, {street.centre.x - 1, street.centre.y + .5f, street.centre.z}, 0);
    EVT_AT_SetPos(partner, invalid, {street.centre.x + 1, street.centre.y + .5f, street.centre.z}, 0);
    EnableScenarioCharacterDisplay(secondFighter);
    EnsureScenarioCharacterDraw(secondFighter);
    EnableScenarioCharacterDisplay(secondPartner);
    EnsureScenarioCharacterDraw(secondPartner);
    EVT_AT_SetPos(secondFighter, invalid, {secondCentre.x - 1, secondCentre.y + .5f, secondCentre.z}, 0);
    EVT_AT_SetPos(secondPartner, invalid, {secondCentre.x + 1, secondCentre.y + .5f, secondCentre.z}, 0);
    const std::int32_t type = g_currentMap.name == SHEN_NAME(QLBT) ? 0 : 4;
    SetScenarioCharacterType(fighter, type);
    SetScenarioCharacterType(partner, type);
    SetScenarioCharacterDrawControl(STREET_WATCH_RYO, 2, 1);
    SetScenarioCharacterDrawControl(fighter, 2, 1);
    SetScenarioCharacterDrawControl(partner, 2, 1);
    EV_SetCharacterAlphaEnabled(STREET_WATCH_RYO, true);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EV_SetCharacterAlphaEnabled(partner, false);
    SetScenarioCharacterType(secondFighter, CharacterTypeStencilShadow);
    SetScenarioCharacterType(secondPartner, CharacterTypeStencilShadow);
    SetScenarioCharacterDrawControl(secondFighter, 2, 1);
    SetScenarioCharacterDrawControl(secondPartner, 2, 1);
    EV_SetCharacterAlphaEnabled(secondFighter, false);
    EV_SetCharacterAlphaEnabled(secondPartner, false);
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        if (!DoesScenarioCharacterExist(STREET_FIGHT_CNT2))
            CreateEventCharacter(CharacterCreationHighModel, nullptr, STREET_FIGHT_CNT2);
    } else if (g_currentMap.name != SHEN_NAME(AB00) && g_currentMap.name != SHEN_NAME(QC01) && g_currentMap.name != SHEN_NAME(QD01)) {
        work.state = 3;
        return;
    }
    if (!DoesScenarioCharacterExist(STREET_FIGHT_CNT1))
        CreateEventCharacter(CharacterCreationHighModel, nullptr, STREET_FIGHT_CNT1);
    work.state = 2;
}
void ParentActors(StreetWatchWork &work) {
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        const shen_name invalid{0xffffffffU};
        EVT_AT_SetPos(STREET_FIGHT_CNT1, invalid, {197, 100.5f, 129}, 0);
        EVT_AT_SetAngY(STREET_FIGHT_CNT1, invalid, 0, 0);
        EVT_AT_SetPos(fighter, STREET_FIGHT_CNT1, {0, 0, -.28999999165534973f}, 0);
        EVT_AT_SetPos(partner, STREET_FIGHT_CNT1, {.04650000110268593f, 0, -1.13264000415802f}, 0);
        EVT_AT_SetAngY(fighter, STREET_FIGHT_CNT1, 0, 0);
        EVT_AT_SetAngY(partner, STREET_FIGHT_CNT1, 0, 0);
        EVT_AT_SetMot(fighter, 0x1acd, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(partner, 0x1ac9, 0, 1, 0x40000000U, 0);
        EVT_AT_SetPos(STREET_FIGHT_CNT2, invalid, {174.39999389648438f, 100.5f, 233.57240295410156f}, 0);
        EVT_AT_SetAngY(STREET_FIGHT_CNT2, invalid, 0, 0);
        EVT_AT_SetPos(secondFighter, STREET_FIGHT_CNT2, {.004600000102072954f, 0, -1.4149999618530273f}, 0);
        EVT_AT_SetPos(secondPartner, STREET_FIGHT_CNT2, {-.4632999897003174f, 0, -1.2020000219345093f}, 0);
        EVT_AT_SetAngY(secondFighter, STREET_FIGHT_CNT2, 0, 0);
        EVT_AT_SetAngY(secondPartner, STREET_FIGHT_CNT2, 0, 0);
        EVT_AT_SetMot(secondFighter, 0x1acc, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(secondPartner, 0x1ac8, 0, 1, 0x40000000U, 0);
    }
    work.state = 4;
}
void PrepareFight(StreetWatchWork &work) {
    shenStartFGST(10, 10, 0, 0);
    if (g_currentMap == AREA_QJBT || g_currentMap.name == SHEN_NAME(QKBT))
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
    else if (g_currentMap.name == SHEN_NAME(QLBT)) {
        EV_SetRefBoardParamsFor(fighter, 0);
        EV_SetRefBoardParamsFor(partner, 0);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x200}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
    }
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{2}, 1);
    DestroyElementByIndex(UIElement::StreetFighterHud);
    vfmDispatchControl(FightControlOp::Package, shen_name{0}, reinterpret_cast<std::uintptr_t>(street.package), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 0, 500, 28);
    shenInitFightData(FindCharacterByScenarioName(partner), nullptr, 1, 1, 500, 47);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x8000000, 2);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x8000000, 1);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_WATCH_RYO), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(partner), FacialNodeOriginal);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x10000, 0);
    vfUpdateFlagsFor(FindCharacterByScenarioName(partner), 0, 0x10000, 0);
    vfCopyPosFromEntity(0, nullptr, street.centre.x, street.centre.z, 2.75f);
    shenInitFightData(FindCharacterByScenarioName(secondFighter), nullptr, 1, 0, 500, 27);
    shenInitFightData(FindCharacterByScenarioName(secondPartner), nullptr, 1, 1, 500, 2);
    vfUpdateFlagsFor(FindCharacterByScenarioName(secondFighter), 0, 0x10000, 0);
    vfUpdateFlagsFor(FindCharacterByScenarioName(secondPartner), 0, 0x10000, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x400}, 1);
    vfCopyPosFromEntity(0, nullptr, secondCentre.x, secondCentre.z, 2.75f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    vfmQueueFightStart(4, 99, 1, 0, -1);
    work.state = 4;
}
void StreetCallback(void *payload) {
    auto &work = *static_cast<StreetWatchWork *>(payload);
    if (work.state == 1)
        PrepareActors(work);
    else if (work.state == 2)
        ParentActors(work);
    else if (work.state == 3)
        PrepareFight(work);
    else
        StreetWatchMainCallback(payload, street);
}
void UpdateBike() {

    if (EV_GetShenmueFlag(0x3c1)) {
        if (DoesScenarioCharacterExist(CHARACTER_BIKE_NAME)) {
            EnsureScenarioCharacterDraw(CHARACTER_BIKE_NAME);
            EnableScenarioCharacterDisplay(CHARACTER_BIKE_NAME);
            ToggleFlagOnFieldEntity(CHARACTER_BIKE_NAME, 1);
        }
    } else if (DoesScenarioCharacterExist(CHARACTER_BIKE_NAME)) {
        DisableScenarioCharacterDisplay(CHARACTER_BIKE_NAME);
        ToggleScenarioCharacterFlag(CHARACTER_BIKE_NAME);
        ToggleFlagOnFieldEntity(CHARACTER_BIKE_NAME, 0);
    }
}
void SpringCallback(void *) {
    SetTextureAnimationUV(0, .054999999701976776f);
    ApplyTextureAnimationResource(4);
}
} // namespace
void AreaQ100Init() {
}
void AreaQ100Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    EV_SetObjectFlagByIndex(5, afternoon ? 1 : 0);
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("Q100");
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
            EV_LoadAudio("Q100_000", afternoon ? 1 : 0);
    }
    if (g_currentMap.name != SHEN_NAME(Q109))
        UpdateBike();
}
void AreaQ100Main(std::uint32_t) {
    initializing = 1;
    SetSoundProgramPhaseFlags(0);
    AreaQ100Cleanup(EV_GetShenmueFlag(0xff), 0);
    HLib::EnqueueTaskWithoutParameter(SpringCallback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(fntn)));
    InitPati();
    if (g_currentMap.name != SHEN_NAME(Q109)) {
        QueueStreetWatch(street, MinigameCallback, StreetCallback);
        SetGamble();
        areaClearAreaEntities();
    }
    initializing = 0;
}
const GameplayAreaDefinition areaQ100GameplayDefinition{SHEN_NAME(Q100), 0, AreaQ100Init, AreaQ100Main, AreaQ100Cleanup};
} // namespace shenmue::s2::areas
