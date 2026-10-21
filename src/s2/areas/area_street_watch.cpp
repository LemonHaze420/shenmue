#include "s2/debug.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/advice.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name invalid{0xffffffffU};
void LoadStreetWatchPackage(const char *package, bool waitFirst) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
        return;
    if (waitFirst)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    if (waitFirst)
        EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_street_watch.c", 305, "PACK LOAD START\n");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    if (waitFirst)
        EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_street_watch.c", 308, "PACK LOAD END\n");
}
void RestoreStreetWatchActors(StreetWatchWork &work, const StreetWatchState &owner) {
    ET_AT_KillWork(work.characters);
    if (DoesScenarioCharacterExist(STREET_FIGHT_CNT1))
        SetScenarioCharacterKillMode(STREET_FIGHT_CNT1, 1);
    if (DoesScenarioCharacterExist(STREET_FIGHT_CNT2))
        SetScenarioCharacterKillMode(STREET_FIGHT_CNT2, 1);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(0, "0100");
    if (owner.freeAnnouncerPackage)
        EV_FreeAuthorPak(0, "0255");
    if (work.audience) {
        HLib::GetTaskParameter(work.audience);
        HLib::DestroyTaskInternal(work.audience);
    }
}
void PrepareStreetWatchPlayerAngle(const StreetWatchState &owner) {
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const float x = owner.centre.x - position.x, z = owner.centre.z - position.z;
    const float distance = std::sqrt(z * z + x * x);
    EventSetAngle request{0, STREET_WATCH_RYO, invalid, static_cast<float>(InverseFixedAtan2(x / distance, z / distance)) / 182.04445f - 20.0f, 0.0f};
    EVT_AT_SetAngY_Callback(&request);
}
} // namespace
void PositionStreetWatchAnnouncer() {
    Vector3 position{0.0f, 10000.0f, 0.0f};
    float angle = 0.0f;
    switch (g_currentMap.name) {
    case SHEN_NAME(QSFC):
        position = {270.60001f, 94.809998f, 35.5f};
        angle = 165.0f;
        break;
    case SHEN_NAME(QSFD):
        position = {136.2f, 100.0f, 106.2f};
        angle = 200.0f;
        break;
    case SHEN_NAME(QSFB):
        position = {333.5f, 84.25f, 77.400002f};
        angle = 220.0f;
        break;
    case SHEN_NAME(Q100):
        position = {199.10001f, 100.0f, 135.3f};
        angle = 12.0f;
        break;
    case SHEN_NAME(QTB1):
        position = {207.7f, 100.0f, 247.10001f};
        angle = 75.0f;
        break;
    case SHEN_NAME(QSFA):
        position = {176.2f, 100.0f, 239.3f};
        angle = 22.0f;
        break;
    default:
        break;
    }
    EventSetPosition request{0, STREET_WATCH_ANNOUNCER, invalid, position, 0.0f};
    EVT_AT_SetPosRot(&request);
    EventSetAngle rotation{0, STREET_WATCH_ANNOUNCER, invalid, angle, 0.0f};
    EVT_AT_SetAngY_Callback(&rotation);
}
std::uint8_t GetStreetWatchHelpTopic() {
    const bool minigame = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) != 0;
    switch (g_currentMap.name) {
    case SHEN_NAME(QSFC):
        return 100;
    case SHEN_NAME(QSFD):
        return 93;
    case SHEN_NAME(Q100):
        return 91;
    case SHEN_NAME(AB00):
        return 109;
    case SHEN_NAME(QTB1):
        return 89;
    case SHEN_NAME(QSFA):
        return 95;
    case SHEN_NAME(QSFB):
        return 97;
    case SHEN_NAME(QGBT):
        return minigame ? 150 : 102;
    case SHEN_NAME(QJBT):
        return minigame ? 149 : 99;
    case SHEN_NAME(QKBT):
        return minigame ? 152 : 106;
    case SHEN_NAME(QLBT):
        return minigame ? 151 : 103;
    default:
        return 20;
    }
}
void StartStreetWatchFight(StreetWatchState &owner) {
    if (!owner.selection.area.name)
        return;
    SetPlayerControlEnabled(0);
    owner.selection.signal = &owner.busy;
    owner.selection.reserved18 = 0;
    owner.selection.flags = 0;
    if (GetFightModeFlags() & 8)
        owner.selection.flags |= 0x10U;
    owner.selection.flags |= 1U;
    char eventName[16];
    std::memcpy(eventName, &owner.selection.area.name, 4);
    std::memcpy(eventName + 4, ".EV3", 5);
    LoadAreaFunc_Wrapper_0(eventName, 0);
    ExecuteCurrAreaMainFunc(eventName, reinterpret_cast<std::uintptr_t>(&owner.selection));
}
HLib::Task *QueueStreetWatch(StreetWatchState &owner, HLib::TaskCallback minigame, HLib::TaskCallback street) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    const std::int32_t isMinigame = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014);
    areaFlags |= 3U;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(isMinigame ? minigame : street, 5, 0x0b, sizeof(StreetWatchWork), STREET_WATCH_TASK);
    StreetWatchWork *work = static_cast<StreetWatchWork *>(HLib::GetTaskParameter(task));
    owner.iconPrepared = 0;
    work->task = task;
    work->state = 0;
    if (isMinigame)
        return task;
    owner.speechStarted = 0;
    owner.battleCount = 0;
    work->audience = nullptr;
    owner.initialized = 0;
    owner.announcerLoaded = 0;
    owner.restart = 0;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    return work->task;
}
void StreetWatchMinigameCallback(void *payload, StreetWatchState &owner) {
    StreetWatchWork &work = *static_cast<StreetWatchWork *>(payload);
    if (work.state) {
        if (work.state == 1 && !owner.busy) {
            HLib::EV_HLKillOtherHierarchyTask(work.task);
            EV_scenQueueLoad(g_actNumber, invalid, 0);
        }
        return;
    }
    work.state = 1;
    owner.busy = 1;
    ST_LoadTitleFade();
    LoadStreetWatchPackage(owner.package, true);
    SetSFCharaForCurrentArea(nullptr, reinterpret_cast<StreetFightAreaSelection *>(&owner.selection));
    StartStreetWatchFight(owner);
}
void StreetWatchMainCallback(void *payload, StreetWatchState &owner) {
    StreetWatchWork &work = *static_cast<StreetWatchWork *>(payload);
    switch (work.state) {
    case 0: {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        if (g_currentMap.name != SHEN_NAME(QKBT))
            work.audience = QueueStreetFightAudience();
        if (work.audience) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audience));
            audience->fighters[audience->fighterCount++] = owner.fighter;
        }
        if (work.audience) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audience));
            audience->fighters[audience->fighterCount++] = owner.partner;
        }
        if (work.audience) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audience));
            audience->soundCentre = owner.centre;
            audience->state = 2;
        }
        LoadStreetWatchPackage(owner.package, false);
        EV_FreeAuthorPak(1, "0100");
        if (owner.freeAnnouncerPackage)
            EV_FreeAuthorPak(1, "0255");
        work.characters = Queue_EVT_AT_ReadChr(work.task, CanPerformForCurrentAreaFlags() == 1 ? owner.withAnnouncer : owner.withoutAnnouncer);
        work.state = 1;
        return;
    }
    case 1: {
        if (CanPerformForCurrentAreaFlags() == 1) {
            PositionStreetWatchAnnouncer();
            EVT_AT_SetMot(STREET_WATCH_ANNOUNCER, 0x25c3, 0.0f, 1.0f, 0, 0);
            EnableScenarioCharacterDisplay(STREET_WATCH_ANNOUNCER);
            EnsureScenarioCharacterDraw(STREET_WATCH_ANNOUNCER);
        } else if (DoesScenarioCharacterExist(STREET_WATCH_ANNOUNCER)) {
            DisableScenarioCharacterDisplay(STREET_WATCH_ANNOUNCER);
            ToggleScenarioCharacterFlag(STREET_WATCH_ANNOUNCER);
        }
        EnableScenarioCharacterDisplay(owner.fighter);
        EnsureScenarioCharacterDraw(owner.fighter);
        EnableScenarioCharacterDisplay(owner.partner);
        EnsureScenarioCharacterDraw(owner.partner);
        EVT_AT_SetPos(owner.fighter, invalid, {owner.centre.x - 1.0f, owner.centre.y + 0.5f, owner.centre.z}, 0);
        EVT_AT_SetPos(owner.partner, invalid, {owner.centre.x + 1.0f, owner.centre.y + 0.5f, owner.centre.z}, 0);
        const std::int32_t type = g_currentMap.name == SHEN_NAME(QLBT) ? 0 : 4;
        SetScenarioCharacterType(owner.fighter, type);
        SetScenarioCharacterType(owner.partner, type);
        SetScenarioCharacterDrawControl(STREET_WATCH_RYO, 2, 1);
        SetScenarioCharacterDrawControl(owner.fighter, 2, 1);
        SetScenarioCharacterDrawControl(owner.partner, 2, 1);
        EV_SetCharacterAlphaEnabled(STREET_WATCH_RYO, true);
        EV_SetCharacterAlphaEnabled(owner.fighter, false);
        EV_SetCharacterAlphaEnabled(owner.partner, false);
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
        return;
    }
    case 2:
        if (owner.prepareParentedActors)
            owner.prepareParentedActors();
        work.state = 4;
        return;
    case 3:
        shenStartFGST(10, 10, 0, 0);
        if (g_currentMap == AREA_QJBT || g_currentMap.name == SHEN_NAME(QKBT)) {
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
        } else if (g_currentMap.name == SHEN_NAME(QLBT)) {
            EV_SetRefBoardParamsFor(owner.fighter, 0);
            EV_SetRefBoardParamsFor(owner.partner, 0);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x200}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
        }
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{2}, 1);
        DestroyElementByIndex(UIElement::StreetFighterHud);
        vfmDispatchControl(FightControlOp::Package, shen_name{0}, reinterpret_cast<std::uintptr_t>(owner.package), 1);
        shenInitFightData(FindCharacterByScenarioName(owner.fighter), nullptr, 1, 0, 500, owner.fighterStyle);
        shenInitFightData(FindCharacterByScenarioName(owner.partner), nullptr, 1, 1, 500, owner.partnerStyle);
        if (owner.formationAssignments == 2)
            vfUpdateFlagsFor(FindCharacterByScenarioName(owner.fighter), 0, 0x8000000, 2);
        if (owner.formationAssignments)
            vfUpdateFlagsFor(FindCharacterByScenarioName(owner.fighter), 0, 0x8000000, 1);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_WATCH_RYO), FacialNodeOriginal);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(owner.fighter), FacialNodeOriginal);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(owner.partner), FacialNodeOriginal);
        vfUpdateFlagsFor(FindCharacterByScenarioName(owner.fighter), 0, 0x10000, 0);
        vfUpdateFlagsFor(FindCharacterByScenarioName(owner.partner), 0, 0x10000, 0);
        vfCopyPosFromEntity(0, nullptr, owner.centre.x, owner.centre.z, 2.75f);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        work.state = 4;
        return;
    case 4:
        if (!(GetFightModeFlags() & 4)) {
            EV_NPC_NewLoadSw(1);
            if ((GetFightModeFlags() & 8) && owner.initialized)
                QueueStreetFightNpcWait(0x10);
        }
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
        work.state = 5;
        return;
    case 5:
        if (owner.initialized) {
            if (!(GetFightModeFlags() & 2)) {
                SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
                EV_spmBeginSampleExecute();
            }
        } else {
            if (owner.queueInitialSleep) {
                areaFlags |= 1U;
                owner.queueInitialSleep();
                SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
            }
            if (GetFightModeFlags() & 2) {
                if (g_currentMap.name != SHEN_NAME(QKBT)) {
                    shenSetTimeSimulationTo(1);
                    ST_LoadTitleFade();
                }
                EV_setHoldStatesFlags(1);
            } else
                QueueImmediateNextFadeTask(work.task);
        }
        SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(22), nullptr);
        work.state = 6;
        return;
    case 6:
        if (g_areaHeldItemRestorePending)
            EV_setHoldStatesFlags(0);
        if (!(GetFightModeFlags() & 1))
            SetPlayerControlEnabled(1);
        SetFightModeFlags(128);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
        owner.initialized = 1;
        work.state = 7;
        return;
    case 7: {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234)) {
            SetPlayerControlEnabled(0);
            work.state = 27;
            return;
        }
        if (!ryoNotBusy())
            return;
        owner.help = 0;
        owner.advice = 0;
        owner.motion = 0;
        std::int32_t admitted = 0;
        switch (g_currentMap.name) {
        case SHEN_NAME(QSFC):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 8;
            owner.motion = 3;
            break;
        case SHEN_NAME(Q100):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 2;
            owner.motion = 2;
            break;
        case SHEN_NAME(AB00):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 11;
            owner.motion = 0;
            break;
        case SHEN_NAME(QTB1):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 1;
            owner.motion = 11;
            break;
        case SHEN_NAME(QSFA):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 4;
            owner.motion = 5;
            break;
        case SHEN_NAME(QSFB):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004}, -1) ? 5 : 20;
            owner.motion = 7;
            break;
        case SHEN_NAME(QSFD):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 3;
            owner.motion = 10;
            break;
        case SHEN_NAME(QGBT):
            admitted = EV_GetShenmueFlag(294);
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 7;
            owner.motion = 12;
            break;
        case SHEN_NAME(QJBT):
            if (static_cast<std::int32_t>(GetQronNextStream()) != -3 || !EV_GetShenmueFlag(295))
                break;
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 6;
            owner.motion = 1;
            break;
        case SHEN_NAME(QKBT):
            admitted = 1;
            owner.help = GetStreetWatchHelpTopic();
            owner.advice = 10;
            owner.motion = 4;
            break;
        case SHEN_NAME(QLBT): {
            Vector3 position;
            EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
            if (position.y > 125.5f && position.y < 130.0f) {
                admitted = EV_GetShenmueFlag(296);
                owner.help = GetStreetWatchHelpTopic();
                owner.advice = 9;
                owner.motion = 8;
            }
            break;
        }
        default:
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const Vector3i rotation = GetCharacterWorldRotation(FindCharacterByScenarioName(STREET_WATCH_RYO));
        if (g_currentMap.name == SHEN_NAME(QKBT)) {
            if (GetSquaredDistanceXZ(&owner.rooftopCentre, &position) < 4.0f && position.y > 83.5f && position.y < 84.5f) {
                if (!owner.iconPrepared)
                    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(owner.help), 0);
                owner.iconPrepared = 1;
                work.state = 20;
            }
            return;
        }
        if (!admitted)
            return;
        const std::int32_t facing = InverseFixedAtan2(owner.centre.x - position.x, owner.centre.z - position.z);
        const std::uint32_t difference = static_cast<std::uint16_t>(facing - rotation.y);
        const std::uint32_t reverse = static_cast<std::uint16_t>(rotation.y - facing);
        std::int32_t signedDifference;
        if (difference >= reverse)
            signedDifference = static_cast<std::int32_t>(reverse);
        else
            signedDifference = -(facing > rotation.y ? facing - rotation.y : static_cast<std::int32_t>(difference));
        const Vector3 *approach = g_currentMap.name == SHEN_NAME(QLBT) ? &owner.approachCentre : &owner.centre;
        const float degrees = static_cast<float>(signedDifference) / 182.04445f;
        if (GetSquaredDistanceXZ(approach, &position) >= owner.approachRadiusSquared || degrees <= -90.0f || degrees >= 90.0f) {
            if (owner.iconPrepared)
                EV_EventSetUpHelpIcon_0(0, 0);
            owner.iconPrepared = 0;
            return;
        }
        if (!owner.iconPrepared)
            EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(owner.help), 0);
        owner.iconPrepared = 1;
        if (!vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(owner.motion) | 0x40U}, 0, -1) &&
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(owner.motion) | 0x80U}, 0, -1) && IsMainIconInputReady()) {
            if (GetBattleDataConfig(2, SHEN_NAME(BTAD)))
                QueueFightAdviceControl(owner.advice);
            else
                GetBattleDataConfig(0, SHEN_NAME(BTAD));
        }
        if (!vfmDispatchControl(FightControlOp::MovesScroll, shen_name{0}, 0, -1) && IsMainIconInputReady()) {
            if (GetBattleDataConfig(2, SHEN_NAME(BTBT))) {
                if (!owner.speechStarted) {
                    const std::int64_t performance = CanPerformForCurrentAreaFlags();
                    if (performance == 1) {
                        work.state = 13;
                        return;
                    }
                    if (performance == -1) {
                        work.state = 17;
                        return;
                    }
                }
                work.state = 20;
            } else
                GetBattleDataConfig(0, SHEN_NAME(BTBT));
            if (GetBattleDataConfig(2, SHEN_NAME(BTMM)))
                vfmDispatchControl(FightControlOp::MovesScroll, shen_name{0}, 0, 1);
            else
                GetBattleDataConfig(0, SHEN_NAME(BTMM));
        }
        return;
    }
    case 13:
        SetPlayerControlEnabled(0);
        shenToggleAreaFlags(1);
        owner.speechStarted = 1;
        EVT_AT_SetMot(STREET_WATCH_ANNOUNCER, 0x25c3, 0.0f, 1.0f, 0, 0);
        EVT_AT_SetVoice("0255A001", 90); // @text EN: "That's him!" | JA: "…あいつだ！"
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(owner.package), 0);
        work.state = 14;
        HLib::SetupTaskDelay(30);
        return;
    case 14: {
        vfmDispatchControl(FightControlOp::ResetCameraWaypoints, shen_name{0}, 0, 1);
        Vector3 position;
        EV_GetCharaPosition(STREET_WATCH_ANNOUNCER, 5, &position, EventTransformFlags::World);
        vfmDispatchControl(FightControlOp::AppendCameraWaypoint, shen_name{0}, reinterpret_cast<std::uintptr_t>(&position), 1);
        vfmDispatchControl(FightControlOp::MotionInterpolationCamera, STREET_WATCH_ANNOUNCER, 0, 1);
        work.state = 15;
        HLib::SetupTaskDelay(36);
        return;
    }
    case 15:
        if (vfmDispatchControl(FightControlOp::CameraMode, shen_name{0}, 0, -1) != 6) {
            Vector3 player, announcer;
            EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
            EV_GetCharaPosition(STREET_WATCH_ANNOUNCER, CHARACTER_TRANSLATION_ROOT, &announcer, EventTransformFlags::World);
            EventSetAngle request{
                0, STREET_WATCH_RYO, invalid, static_cast<float>(InverseFixedAtan2(announcer.x - player.x, announcer.z - player.z)) / 182.04445f, 0.0f};
            EVT_AT_SetAngY_Callback(&request);
            work.state = 16;
            HLib::SetupTaskDelay(1);
        }
        return;
    case 16:
        work.state = 20;
        HLib::SetupTaskDelay(10);
        return;
    case 17: {
        SetPlayerControlEnabled(0);
        owner.speechStarted = 1;
        PrepareStreetWatchPlayerAngle(owner);
        EVT_AT_SetMot(STREET_WATCH_RYO, 0x25c4, 0.0f, 1.0f, 0, 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(owner.package), 0);
        EventSetCamera camera{0, 0x26ac, STREET_WATCH_RYO, STREET_WATCH_RYO, 0, {}, 0.0f, 0.0f};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlSetFilmBarEnabled(0);
        EVT_AT_SetVoice("0255A002", 180);
        work.state = 18;
        HLib::SetupTaskDelay(60);
        return;
    }
    case 18: {
        Vector3 position;
        EV_GetCharaPosition(STREET_WATCH_RYO, 5, &position, EventTransformFlags::World);
        const float x = owner.centre.x - position.x, z = owner.centre.z - position.z;
        const float distance = std::sqrt(z * z + x * x);
        const float dx = x * 3.5f / distance, dz = z * 3.5f / distance;
        Vector3 first{(cos_fixed16(0xc000) * dx - sin_fixed16(static_cast<std::int16_t>(0xc000)) * dz) + owner.centre.x, owner.centre.y + 1.2f,
            (sin_fixed16(static_cast<std::int16_t>(0xc000)) * dx + cos_fixed16(0xc000) * dz) + owner.centre.z};
        Vector3 second{(cos_fixed16(0x4000) * dx - sin_fixed16(0x4000) * dz) + owner.centre.x, owner.centre.y + 1.2f,
            (sin_fixed16(0x4000) * dx + cos_fixed16(0x4000) * dz) + owner.centre.z};
        Vector3 centre{owner.centre.x, owner.centre.y + 1.2f, owner.centre.z};
        vfmDispatchControl(FightControlOp::ResetCameraWaypoints, shen_name{0}, 0, 1);
        vfmDispatchControl(FightControlOp::AppendCameraWaypoint, shen_name{0}, reinterpret_cast<std::uintptr_t>(&centre), 1);
        vfmDispatchControl(FightControlOp::AppendCameraWaypoint, shen_name{0}, reinterpret_cast<std::uintptr_t>(&first), 1);
        vfmDispatchControl(FightControlOp::AppendCameraWaypoint, shen_name{0}, reinterpret_cast<std::uintptr_t>(&second), 1);
        vfmDispatchControl(FightControlOp::AppendCameraWaypoint, shen_name{0}, reinterpret_cast<std::uintptr_t>(&centre), 1);
        vfmDispatchControl(FightControlOp::MotionInterpolationCamera, shen_name{0}, 0, 1);
        work.state = 19;
        HLib::SetupTaskDelay(270);
        return;
    }
    case 19:
        SetPlayerControlEnabled(1);
        work.state = 7;
        return;
    case 20:
        SetPlayerControlEnabled(0);
        shenToggleAreaFlags(1);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 21;
        return;
    case 21:
        if (!filmCheckFadeTrackColour()) {
            if (IsFightActive())
                check_dead_status(1);
            if (owner.announcerLoaded) {
                EV_npcFree(STREET_WATCH_ANNOUNCER);
                EV_npcLockUnlock(STREET_WATCH_ANNOUNCER);
                owner.announcerLoaded = 0;
            }
            HLib::SetupTaskDelay(30);
            work.state = 22;
        }
        return;
    case 22:
        if (IsFightActive())
            check_dead_status(1);
        work.state = 23;
        return;
    case 23:
        if (!IsFightActive()) {
            RestoreStreetWatchActors(work, owner);
            work.state = 24;
        }
        return;
    case 24:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
        work.state = 25;
        return;
    case 25:
        ++owner.battleCount;
        owner.busy = 1;
        SetSFCharaForCurrentArea(&work, reinterpret_cast<StreetFightAreaSelection *>(&owner.selection));
        StartStreetWatchFight(owner);
        work.state = 26;
        return;
    case 26:
        if (owner.busy)
            return;
        if (g_currentMap.name == SHEN_NAME(QLBT) && owner.selection.reserved18 == 99) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
            if (owner.queueLostSyunen)
                owner.queueLostSyunen();
            owner.iconPrepared = 0;
            SetFightModeFlags(128);
            work.state = 31;
        } else
            work.state = 30;
        return;
    case 27:
        if (IsFightActive())
            check_dead_status(1);
        work.state = 28;
        return;
    case 28:
        if (!IsFightActive()) {
            RestoreStreetWatchActors(work, owner);
            work.state = 29;
        }
        return;
    case 29:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
        work.state = 30;
        return;
    case 30:
        if (owner.restart) {
            owner.restart = 0;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
            if (owner.queueLostSyunen)
                owner.queueLostSyunen();
            SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
            SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 2U));
            SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 4U));
            work.state = 31;
        } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
            work.state = 0;
        return;
    case 31:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
            return;
        if (!(GetFightModeFlags() & 4))
            EV_NPC_NewLoadSw(1);
        if (!(GetFightModeFlags() & 2)) {
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_spmBeginSampleExecute();
        }
        if (!(GetFightModeFlags() & 1))
            SetPlayerControlEnabled(1);
        work.state = 30;
        return;
    default:
        return;
    }
}
} // namespace shenmue::s2::areas
