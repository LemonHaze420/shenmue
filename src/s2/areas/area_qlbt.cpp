#include "s2/areas/area_qlbt.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include "s2/event_queued.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/world_render.hpp"
#include <cmath>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(SF3_)}, partner{SHEN_NAME(SFA_)}, lostPartner{SHEN_NAME(AW5_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name lostActors[]{fighter, lostPartner, shen_name{0xffffffffU}};
void QueueLostSyunen();
StreetWatchState street{.package = "QLBT",
    .centre = {230.89999389648438f, 128.6999969482422f, 93.80000305175781f},
    .approachCentre = {240.5f, 81.5f, 92.5f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 25,
    .partnerStyle = 17,
    .approachRadiusSquared = 36.0f,
    .wideResult = true,
    .queueLostSyunen = QueueLostSyunen};
HLib::Task *streetTask;
StreetWatchWork *streetWork;
std::int32_t wonBusy;
StreetFightStartParameters wonAuthor;
Vector3 trackedPositions[2];
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void WonCompleteCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void WonWaitCallback(void *) {
    if (wonBusy)
        return;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    street.restart = 1;
    HLib::SetCurrentTaskTo(WonCompleteCallback);
}
void WonBeginCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234))
        return;
    wonBusy = 1;
    wonAuthor.area = SHEN_NAME(SFSR);
    wonAuthor.reserved08 = 0;
    wonAuthor.scene = 99;
    SetPlayerControlEnabled(0);
    wonAuthor.reserved18 = 0;
    wonAuthor.flags = 0;
    wonAuthor.signal = &wonBusy;
    if (GetFightModeFlags() & 8)
        wonAuthor.flags |= 0x10U;
    wonAuthor.flags |= 1U;
    LoadAreaFunc_Wrapper_0("SFSR.EV3", 0);
    ExecuteCurrAreaMainFunc("SFSR.EV3", reinterpret_cast<std::uintptr_t>(&wonAuthor));
    HLib::SetCurrentTaskTo(WonWaitCallback);
}
void WonAdmissionCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    HLib::SetCurrentTaskTo(WonBeginCallback);
}
void LostSyunenCallback(void *payload) {
    auto &work = *static_cast<StreetWatchWork *>(payload);
    if (work.state == 0) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        work.audience = QueueStreetFightAudience();
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QLBT"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
        work.characters = Queue_EVT_AT_ReadChr(work.task, lostActors);
        work.state = 1;
    } else if (work.state == 1) {
        EnableScenarioCharacterDisplay(fighter);
        EnsureScenarioCharacterDraw(fighter);
        EnableScenarioCharacterDisplay(lostPartner);
        EnsureScenarioCharacterDraw(lostPartner);
        EVT_AT_SetPos(fighter, shen_name{0xffffffffU}, {232.0f, 128.8000030517578f, 94.5999984741211f}, 0);
        EVT_AT_SetPos(lostPartner, shen_name{0xffffffffU}, {233.10000610351562f, 128.6999969482422f, 95.19999694824219f}, 0);
        EVT_AT_SetAngY(fighter, shen_name{0xffffffffU}, 0, 0);
        EVT_AT_SetAngY(lostPartner, shen_name{0xffffffffU}, 0, 0);
        EVT_AT_SetMot(fighter, 10052, 0, 1, 0, 0);
        GetMotionFrameCount(10052);
        EVT_AT_SetMot(lostPartner, 10049, 0, 1, 0, 0);
        GetMotionFrameCount(10049);
        SetScenarioCharacterType(STREET_WATCH_RYO, CharacterTypeDefault);
        SetScenarioCharacterType(fighter, CharacterTypeDefault);
        SetScenarioCharacterType(lostPartner, CharacterTypeDefault);
        SetScenarioCharacterDrawControl(STREET_WATCH_RYO, 2, 1);
        SetScenarioCharacterDrawControl(fighter, 2, 1);
        SetScenarioCharacterDrawControl(lostPartner, 2, 1);
        EV_SetCharacterAlphaEnabled(STREET_WATCH_RYO, true);
        EV_SetCharacterAlphaEnabled(fighter, false);
        EV_SetCharacterAlphaEnabled(lostPartner, false);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
        SetClothFlag(FindCharacterByScenarioName(fighter), 4, 1);
        SetClothFlag(FindCharacterByScenarioName(lostPartner), 4, 1);
        work.state = 2;
    } else if (work.state == 2 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234)) {
        SetPlayerControlEnabled(0);
        ET_AT_KillWork(work.characters);
        SetPlayerControlEnabled(0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
        if (work.audience) {
            HLib::GetTaskParameter(work.audience);
            HLib::DestroyTaskInternal(work.audience);
        }
        HLib::DestroyTaskInternal(work.task);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 0);
    }
}
void QueueLostSyunen() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(LostSyunenCallback, 5, 0xb, sizeof(StreetWatchWork), STREET_WATCH_TASK);
    auto *work = static_cast<StreetWatchWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->audience = nullptr;
}
void TrackLights() {
    constexpr shen_name lamps[]{SHEN_NAME(LGT1), SHEN_NAME(LGT3)};
    if (!DoesScenarioCharacterExist(lamps[0]))
        return;
    if (DoesScenarioCharacterExist(SHEN_NAME(LGT2))) {
        DisableScenarioCharacterDisplay(SHEN_NAME(LGT2));
        ToggleScenarioCharacterFlag(SHEN_NAME(LGT2));
    }
    const shen_name other = DoesScenarioCharacterExist(lostPartner) ? lostPartner : partner;
    Vector3 actors[2]{{230.89999389648438f, 130.1999969482422f, 93.80000305175781f}, {230.89999389648438f, 130.1999969482422f, 93.80000305175781f}};
    if (DoesScenarioCharacterExist(fighter) && DoesScenarioCharacterExist(other)) {
        EV_GetCharaPosition(other, 0, &actors[0], EventTransformFlags::World);
        EV_GetCharaPosition(fighter, 0, &actors[1], EventTransformFlags::World);
    }
    for (std::uint32_t i = 0; i < 2; ++i) {
        trackedPositions[i].x += (actors[i].x - trackedPositions[i].x) * .5f;
        trackedPositions[i].z += (actors[i].z - trackedPositions[i].z) * .5f;
        Vector3 position;
        EV_GetCharaPosition(lamps[i], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        Vector3 direction{trackedPositions[i].x - position.x, trackedPositions[i].y - position.y, trackedPositions[i].z - position.z};
        const Vector3i rotation{InverseFixedAtan2(direction.y, std::sqrt(direction.z * direction.z + direction.x * direction.x)) - 0x4000,
            InverseFixedAtan2(direction.x, direction.z), 0};
        EV_AT_SetAngY(lamps[i], EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        const float scale = (134.6300048828125f - position.y) / direction.y;
        position.x += scale * direction.x;
        position.y += scale * direction.y;
        position.z += scale * direction.z;
        SetLightRuntimePosition(i + 13, position);
        SetLightSunDirection(i + 13, {-direction.x, -direction.y, -direction.z});
    }
    if (!streetFightSpotlightPositioned) {
        SetScenarioCharacterRadius(lamps[0], 100);
        SetScenarioCharacterRadius(SHEN_NAME(LGT2), 100);
        SetScenarioCharacterRadius(lamps[1], 100);
        streetFightSpotlightPositioned = 1;
    }
}
void MainCallback(void *) {
    ryoNotBusy();
    if (streetWork->state < 25 || !street.restart)
        TrackLights();
    const Vector3 first{240.0f, 0, 90.0f}, second{230.0f, 0, 85.5f};
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const Vector3i rotation = GetCharacterWorldRotation(FindCharacterByScenarioName(STREET_WATCH_RYO));
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 531) || !TestCharaPositionWithinRange(STREET_WATCH_RYO, &first, &second, shen_name{0}) ||
        !(position.y > 139) || static_cast<std::uint32_t>(rotation.y) - 10378U > 0x5552U)
        return;
    if (vfmDispatchControl(FightControlOp::CameraMode, shen_name{0}, 0, -1) == 5 || !shenGetMinutesUntilNextFrameMinute())
        return;
    if (GetBattleDataConfig(2, SHEN_NAME(BTLD)))
        vfmDispatchControl(FightControlOp::MotionUpdateCamera, fighter, 0, 1);
    else
        GetBattleDataConfig(0, SHEN_NAME(BTLD));
}
void ApplyLighting() {
    SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetDefaultVSByIndex(1);
    CheckIsArgEquZero(1);
}
void SetupLights() {
    AssignLightProgram(0);
    SetDefaultVSByIndex(1);
    EV_SetIndexForLightEntry(21, 0);
    EV_SetIndexForLightEntry(22, 0);
    ResetLightRuntimeEntry(13);
    ResetLightRuntimeEntry(14);
    EV_SetLightFlagForIndex(13, 4);
    EV_SetLightFlagForIndex(14, 4);
    SetLightRuntimeColour(13, {1, 1, .9300000071525574f, .6399999856948853f});
    SetLightRuntimeColour(14, {1, .9800000190734863f, .800000011920929f, .4000000059604645f});
    SetLightSceneIntensity(13, 1);
    SetLightSceneIntensity(14, 1);
    SetLightRange(13, 12);
    SetLightRange(14, 12);
    const float first[]{1.5f, 8}, second[]{1, 8};
    SetLightFalloff(13, first);
    SetLightFalloff(14, second);
    EV_SetIndexForLightEntry(13, 1);
    EV_SetIndexForLightEntry(14, 1);
    trackedPositions[0] = {230.89999389648438f, 130.1999969482422f, 93.80000305175781f};
    trackedPositions[1] = {230.89999389648438f, 130.1999969482422f, 93.80000305175781f};
    streetFightSpotlightPositioned = 0;
}
} // namespace
void AreaQLBTInit() {
    streetTask = nullptr;
    streetWork = nullptr;
    wonBusy = 0;
    wonAuthor = {};
    streetFightSpotlightPositioned = 0;
    trackedPositions[0] = {};
    trackedPositions[1] = {};
}
void AreaQLBTCleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
void AreaQLBTMain(std::uint32_t) {
    SetMapControlStatusFlag(0, 0);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_GetShenmueFlag(0xff);
    ApplyLighting();
    SetupLights();
    SetRankedLightLimit(0);
    EV_LoadAudio("QLBT_000", 0);
    SetIsSkySetup(0);
    SetNewNpcBatchCountOverride(NpcFillType::BackFill, 5);
    streetTask = QueueStreetWatch(street, MinigameCallback, StreetCallback);
    streetWork = static_cast<StreetWatchWork *>(HLib::GetTaskParameter(streetTask));
    if (g_entryPoint == 99) {
        SetPlayerControlEnabled(0);
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 2U));
        HLib::EnqueueTaskWithoutParameter(WonAdmissionCallback, 5, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    }
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
}
const GameplayAreaDefinition areaQLBTGameplayDefinition{SHEN_NAME(QLBT), 0, AreaQLBTInit, AreaQLBTMain, AreaQLBTCleanup};
} // namespace shenmue::s2::areas
