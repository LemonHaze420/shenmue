#include "s2/areas/area_sfjf.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/npc.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/sk_event.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/ui.hpp"
#include <cmath>

namespace shenmue::s2 {
namespace {
std::int32_t choicePhase{};
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
void MainCallback(void *);
void DebugBattleEntryCallback(void *);
void DebugBattlePreEventCallback(void *);
void DebugBattlePreInitCallback(void *);
void DebugBattleStartCallback(void *);
void DebugBattleReadyGoCallback(void *);
void DebugBattleStartFightCallback(void *);
void DebugBattleWaitCallback(void *);
void DebugBattleWaitFadeCallback(void *);
void DebugBattleEndCallback(void *);
std::int32_t DebugContinueConversation(void *);
std::int32_t DebugCompletedConversation(void *);
std::int32_t DebugStopConversation(void *);

// @verified - p
std::uint32_t ControlChoice(std::int32_t enabled) {
    if (!enabled) {
        choicePhase = 0;
        return 0;
    }
    switch (choicePhase) {
    case 0:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 2: {
        const std::uint32_t result = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return result;
    }
    default:
        return 0;
    }
}

// @verified - p
std::int64_t QueueDebugCharacterLoad(HLib::Task *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(StreetFightCharacterLoadCallback, 6, 0x0b, sizeof(StreetFightCharacterLoadWork), STREET_FIGHT_EVENT_TASK);
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(parent, 0);
    work->task = task;
    work->characters = debugLoadCharacters;
    work->parent = parent;
    work->index = 0;
    work->state = 0;
    return 0;
}

StreetFightEventWork *InitializeDebug(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = STREET_FIGHT_INVALID;
    work->package = STREET_FIGHT_QJBT;
    work->battleFrames = 50;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->audienceTask = nullptr;
    work->completedOpponents = 0;
    work->hudEnabled = 0;
    work->fighterIndex = 0;
    const StreetFightFighterDefinition *const definition = GetStreetFightDebugFighterDefinition();
    debugOpponentCount = 5;
    work->fighterDefinitions[0] = definition;
    work->fighters[0] = definition->character;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFJF_DEBUG_CHARACTERS);
    return work;
}

// @verified - p
void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QJBT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFJF_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}

// @verified - p
void QueueDebug(StreetFightStartParameters *parameters) {
    if (parameters->signal)
        *parameters->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DebugMainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeDebug(task);
    work->startParameters = parameters;
    LoadDebugResources(work);
}

// @verified - p
void PlaceDebugCharacters(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(SFJF_FUY);
    EV_EnsureCharacterDraw(SFJF_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{0, SFJF_BASE, STREET_FIGHT_INVALID, SFJF_STAGE_ORIGIN, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, SFJF_BASE, STREET_FIGHT_INVALID, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFJF_BASE);
    AT_SetLocalBaseChara(SFJF_FUY, SFJF_BASE);
    AT_SetLocalBaseChara(fighter, SFJF_BASE);
    position = {0, STREET_FIGHT_RYO, SFJF_BASE, SFJF_DEBUG_PLAYER_POSITION, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, STREET_FIGHT_RYO, SFJF_BASE, 225, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, SFJF_FUY, SFJF_BASE, work->bookiePosition, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, SFJF_FUY, SFJF_BASE, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, fighter, SFJF_BASE, {1.5f, 0, -1}, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, fighter, SFJF_BASE, 17, 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFJF_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

// @verified - p
void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFJF_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFJF_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFJF_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ENMY"), 0);
    LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    if (parameters->signal)
        *parameters->signal = 0;
}

void DebugMainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    switch (work.state) {
    case 0:
        debugLoadCharacters[0] = fighter;
        debugLoadCharacters[1] = STREET_FIGHT_INVALID;
        QueueDebugCharacterLoad(work.task);
        work.state = 1;
        break;
    case 1: {
        PlaceDebugCharacters(&work);
        work.audienceTask = QueueStreetFightAudience();
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        Vector3 stage;
        EV_GetCharaPosition(SFJF_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFJF_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFJF_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFJF_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            const float x = player.x - bookie.x, z = player.z - bookie.z;
            audience->corridorLength = std::sqrt(x * x + z * z);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFJF_FUY, EventTransformFlags::AllAxes, savedBookie);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->radius = 5.4f;
            audience->radiusRange = 2.0f;
            audience->state = 2;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        work.state = 2;
        break;
    }
    case 2:
        EVT_AT_SetPos(
            fighter, STREET_FIGHT_INVALID, {SFJF_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFJF_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFJF_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfjfStreetFightConversation(StartStreetFightBookieConversation, &work);
            work.state = 4;
            break;
        }
        [[fallthrough]];
    case 4:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 5;
        break;
    case 5: {
        ReadStreetFightBookiePosition();
        const std::uint32_t choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 38)
            work.state = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= 20 ? 6 : 8;
        else
            work.state = 7;
        break;
    }
    case 6:
        QueueSfjfStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfjfStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfjfStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
        work.state = 23;
        break;
    case 9:
        HLib::SetCurrentTaskTo(DebugBattleEntryCallback);
        work.state = 12;
        break;
    case 12:
        work.state = work.battleStatus == 3 ? 13 : 17;
        break;
    case 13:
        ++work.completedOpponents;
        AddStreetFightProgress(SFJF_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfjfStreetFightConversation(StreetFightVictoryConversation, &work);
        work.state = 14;
        break;
    case 14: {
        nullsub();
        const StreetFightFighterDefinition *const next = GetStreetFightDebugFighterDefinition();
        work.fighterDefinitions[0] = next;
        const shen_name nextFighter = next->character;
        work.fighters[0] = nextFighter;
        EV_NPC_NewLoadSw(0);
        if (fighter.name != STREET_FIGHT_INVALID.name) {
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[0] = audience->fighters[1] = audience->fighters[2] = audience->fighters[3] = shen_name{};
                audience->fighterCount = 0;
            }
            ReleaseStreetFightCharacter(fighter);
        }
        if (nextFighter.name != STREET_FIGHT_INVALID.name) {
            debugLoadCharacters[0] = nextFighter;
            debugLoadCharacters[1] = STREET_FIGHT_INVALID;
            QueueDebugCharacterLoad(work.task);
        }
        work.state = 15;
        break;
    }
    case 15:
        EV_NPC_NewLoadSw(1);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        QueueSfjfStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfjfStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfjfStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadStreetFightBookiePosition();
        const std::uint32_t choice = ControlStreetFightActionChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfjfStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfjfStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfjfStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfjfStreetFightConversation(DebugCompletedConversation, &work);
            work.state = 23;
        }
        break;
    case 23:
        work.state = 24;
        break;
    case 24:
        if (!filmCheckFadeTrackColour() && !GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            work.state = 25;
        break;
    case 25:
        if (work.audienceTask) {
            (void)HLib::GetTaskParameter(work.audienceTask);
            HLib::DestroyTaskInternal(work.audienceTask);
        }
        CleanupDebug(&work);
        ++work.state;
        break;
    default:
        break;
    }
}

// @verified - p
void DebugBattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EventSetPosition position{0, STREET_FIGHT_RYO, SFJF_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, STREET_FIGHT_RYO, SFJF_BASE, 180, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, SFJF_FUY, SFJF_BASE, SFJF_DEBUG_BATTLE_BOOKIE_POSITION, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, SFJF_FUY, SFJF_BASE, 180, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, fighter, SFJF_BASE, {0, 0, 1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, fighter, SFJF_BASE, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}

// @verified - p
void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    const EventFilmControl film{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5};
    EVT_AT_MenCtrl(film);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1ae0, 0, 1, 0, 50);
    EV_PlayDialogue("0100B072");
    EventSetCamera camera{0, 2001, SFJF_BASE, SFJF_BASE, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::SetCurrentTaskTo(DebugBattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}

// @verified - p
void DebugBattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    const shen_name fighter = work.fighters[work.fighterIndex];
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::SetCurrentTaskTo(DebugBattleStartCallback);
    HLib::SetupTaskDelay(10);
}

// @verified - p
void DebugBattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    work.hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QJBT"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFJF_DEBUG_OPPONENT_ORIGIN.x, SFJF_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    vfmQueueFightStart(2, 0x63, 1, 8, 30);
    work.hudEnabled = 0;
    AccessFightActorFlag411(1, FindCharacterByScenarioName(fighter), definition->guardMode);
    AccessFightActorGroupFloat(1, FindCharacterByScenarioName(fighter), definition->strikeScale);
    AccessFightActorGroupFloat(3, FindCharacterByScenarioName(fighter), definition->guardScale);
    AccessFightActorGroupFloat(5, FindCharacterByScenarioName(fighter), definition->guardScale);
    HLib::SetCurrentTaskTo(DebugBattleReadyGoCallback);
}

// @verified - p
void DebugBattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100B073");
    HLib::SetCurrentTaskTo(DebugBattleStartFightCallback);
    HLib::SetupTaskDelay(40);
}

// @verified - p
void DebugBattleStartFightCallback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(DebugBattleWaitCallback);
}

// @verified - p
void DebugBattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (IsFightActive())
        return;
    if (GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 7) {
        work.battleStatus = 7;
        EV_PlayDialogue("0100B075");
    } else {
        work.battleStatus = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 3 ? 3 : 2;
        EVT_AT_SetVoice("0100B074", 1, 0); // @text EN: "勝負あり！" | JA: "勝負あり！"
    }
    HLib::SetCurrentTaskTo(DebugBattleWaitFadeCallback);
}

// @verified - p
void DebugBattleWaitFadeCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        ProcessAicaCommand(0x0aa0, 30);
        EventFilmControl film{0, 30, 0, 0, 0, 0, 255, 0, 0, 0, 0};
        EVT_AT_MenCtrl_Callback(&film);
        HLib::SetCurrentTaskTo(DebugBattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
    work.frameCounter = 0;
}

void DebugBattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 0);
    BeginSecondarySampleExecution(0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFJF_BASE);
    AT_SetLocalBaseChara(fighter, SFJF_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFJF_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

// @verified - p
std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFJF_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFJF_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFJF_BASE, SFJF_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFJF_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFJF_BASE, SFJF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFJF_BASE, SFJF_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFJF_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFJF_FUY, SFJF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFJF_FUY, SFJF_BASE, 0, 0);
    EVT_AT_SetMot(SFJF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = "涼「ああ。」";
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = "胴元「よし待ってな。」";
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = "胴元「さあさあ、この兄ちゃん、＆まだ若えが、けっこうなツワモノだ。」";
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = "胴元「さあ誰か、こいつと勝負するやつはいないか？」";
    fourth->frames = 60;
    EVT_AT_SetCam(2006, SFJF_BASE, SFJF_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFJF_BASE, SFJF_BASE, 0, 0, 150);
    return 210;
}

// @verified - p
std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFJF_BASE, SFJF_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFJF_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFJF_FUY, SFJF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFJF_FUY, SFJF_BASE, 0, 0);
    EVT_AT_SetMot(SFJF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFJF_BASE, SFJF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = "胴元「さあ誰か！！」";
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = "胴元「・・・っと、いねえようだな。」";
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = "胴元「ほら、兄ちゃん、賞金だ。」";
    third->frames = 60;
    EVT_AT_SetCam(2007, SFJF_BASE, SFJF_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180});
    return 240;
}

// @verified - p
std::int32_t DebugStopConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFJF_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFJF_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFJF_FUY, SFJF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFJF_FUY, SFJF_BASE, 0, 0);
    EVT_AT_SetMot(SFJF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFJF_BASE, SFJF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = "涼「いや、もうよしとくよ。」";
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = "胴元「そうか・・・。」";
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = "胴元「しゃあねえ、ほら、賞金だ。」";
    third->frames = 60;
    EVT_AT_SetCam(2007, SFJF_BASE, SFJF_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180});
    return 240;
}

struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state;
    std::byte reserved12[6];
};
struct AuthorLoopWork {
    AuthorWork *parent;
    std::uint32_t reserved08, reserved0c;
};
static_assert(sizeof(AuthorWork) == 0x18);
static_assert(sizeof(AuthorLoopWork) == 0x10);
void AuthorLoopCallback(void *);
void AuthorMainCallback(void *);
void CleanupNormalCallback(void *);
void RetryChoiceCallback(void *);
void RetryBeginCallback(void *);

void SetAuthorCharacterState(shen_name name, bool enabled, bool motionFlag) {
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, 1, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
    if (enabled)
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 3, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
    if (motionFlag)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, enabled ? 0 : 2);
    EV_SetCharacterAlphaEnabled(name, enabled);
}

void PrepareAuthorCharacters(AuthorWork *work) {
    SetAuthorCharacterState(STREET_FIGHT_RYO, false, true);
    SetAuthorCharacterState(SHEN_NAME(a428), false, false);
    SetAuthorCharacterState(SHEN_NAME(a534), false, false);
    for (const shen_name name : SFJF_AUTHOR_CHARACTERS) {
        if (name.name == STREET_FIGHT_INVALID.name)
            break;
        if (work->author.name != SHEN_NAME(AU_1) || name.name != SFJF_SFA.name)
            SetAuthorCharacterState(name, false, true);
    }
}

void RestoreAuthorCharacters() {
    areas::EnableCharDisplayAndSetMotionworkData(STREET_FIGHT_RYO);
    SetAuthorCharacterState(SHEN_NAME(a428), true, false);
    SetAuthorCharacterState(SHEN_NAME(a534), true, false);
    for (const shen_name name : SFJF_AUTHOR_CHARACTERS) {
        if (name.name == STREET_FIGHT_INVALID.name)
            break;
        SetAuthorCharacterState(name, true, true);
    }
}

// @verified - p
void QueueAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorMainCallback, 4, 0x0b, sizeof(AuthorWork), STREET_FIGHT_EVENT_TASK);
    AuthorWork *const work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->author = author;
    work->state = 1;
    SetPlayerControlEnabled(0);
}

// @verified - p
void AuthorMainCallback(void *parameter) {
    AuthorWork &work = *static_cast<AuthorWork *>(parameter);
    if (work.state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 0x0b, sizeof(AuthorLoopWork), STREET_FIGHT_EVENT_TASK);
        AuthorLoopWork *const loop = static_cast<AuthorLoopWork *>(HLib::GetTaskParameter(task));
        loop->parent = &work;
        loop->reserved08 = 0;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadAudio("0256A000", 0);
        PrepareAuthorCharacters(&work);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
        work.state = 2;
    } else if (work.state == 3)
        HLib::DestroyTaskInternal(work.task);
}

void AuthorLoopCallback(void *parameter) {
    AuthorWork &parent = *static_cast<AuthorLoopWork *>(parameter)->parent;
    if (parent.author.name == SHEN_NAME(AU_0)) {
        const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        const shen_name guest{SHEN_NAME(P01Q)};
        if (frame == 506 && DoesScenarioCharacterExist(guest)) {
            EV_SetCharacterAlphaEnabled(guest, false);
            SetScenarioCharacterTransparency(guest, 0);
        } else if (frame == 626 && DoesScenarioCharacterExist(guest)) {
            SetScenarioCharacterTransparency(guest, 1);
            EV_SetCharacterAlphaEnabled(guest, true);
        } else if (frame == 1729 || frame == 1859) {
            const bool enabled = frame == 1729;
            for (shen_name name : {STREET_FIGHT_RYO, SFJF_AW3, SFJF_RE1})
                SetPluralControlFor(FindCharacterByScenarioName(name), enabled);
            for (shen_name name : {STREET_FIGHT_RYO, SFJF_AW3, SFJF_RE1})
                SetFaceFlagsForChara_0(FindCharacterByScenarioName(name), !enabled);
            for (shen_name name : {STREET_FIGHT_RYO, SFJF_AW3, SFJF_RE1}) {
                SetHighModelXYZ(FindCharacterByScenarioName(name), !enabled, 1, 0);
                SetHighModelXYZ(FindCharacterByScenarioName(name), !enabled, 0, 0);
            }
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreAuthorCharacters();
        parent.state = 3;
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
std::int32_t DefeatPoseConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_DisableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharacterAlphaEnabled(fighter, false);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    EVT_AT_SetPos(fighter, SFJF_BASE, {0, -0.05f, 0}, 0);
    EVT_AT_SetAngY(fighter, SFJF_BASE, 90, 0);
    EVT_AT_SetMot(fighter, 10881, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0256B020", 1, 25); // @text EN: " " | JA: "おおおおおぉお！"
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2});
    EVT_AT_SetCam(8099, SFJF_BASE, SFJF_BASE, 0, 0, 1);
    return 120;
}

void SetAudienceResult(StreetFightEventWork &work, std::uint8_t state, std::int16_t result) {
    if (!work.audienceTask)
        return;
    StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
    if (!audience)
        return;
    audience->state = state;
    audience->loopDelay = result;
}

void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI_Internal(choices, 0x7fff, 0xffff, 2, 0);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}

void RetryChoiceCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const std::uint16_t choice = static_cast<std::uint16_t>(ControlChoice(1));
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice != 82) {
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
        return;
    }
    EV_ResetFreeTalkFlags();
    work.state = work.battleStatus = work.auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0256A000", 1);
    EV_spmBeginSampleExecute();
}

void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    const std::int32_t result = work.battleStatus;
    const std::int32_t returnToQron = work.reserved178;
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    for (shen_name name : SFJF_CHARACTERS) {
        if (name.name == STREET_FIGHT_INVALID.name)
            break;
        hndlSetFlags(FindCharacterByScenarioName(name));
    }
    for (shen_name name : {STREET_FIGHT_RYO, SFJF_AW3}) {
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
    }
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFJF_BASE, 1);
    if (result == 3) {
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFJF_DEBUG_EXIT_POSITION);
        EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFJF_DEBUG_EXIT_ROTATION);
    } else {
        EV_AT_SetPos(
            STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {279.54998779296875f, 85.5999984741211f, 235.02999877929688f});
        EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 14672, 0});
        RepositionPlayerForQronFloor(-3);
    }
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    if (!(start->flags & 1))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    EV_FreeAuthorPak(0, "0256");
    ExecuteCleanupFuncForAreaWithCachedScene();
    camCtrlSetFarDistance(work.previousFarClip);
    camCtrlSetNearDistance(work.previousNearClip);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SHEN_NAME(OMAK), 1);
    else if (returnToQron)
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(QR00), 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    EV_EventSetUpHelpIcon_0(STREET_FIGHT_HELP_FREE, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}

void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const bool threeRound = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) != 0;
    switch (work.state) {
    case 0:
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        work.round = work.wins = work.losses = 0;
        work.roundResults[0] = work.roundResults[1] = work.roundResults[2] = 0;
        for (shen_name name : SFJF_CHARACTERS) {
            if (name.name == STREET_FIGHT_INVALID.name)
                break;
            SetLowQualityHands(FindCharacterByScenarioName(name));
        }
        SetHighModelXYZ(FindCharacterByScenarioName(SFJF_AW3), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFJF_AW3), 0, 1, 0);
        for (shen_name name : {STREET_FIGHT_RYO, SFJF_AW3}) {
            EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        }
        vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
        LoadBlurModelForCharacter(STREET_FIGHT_RYO, "RYO_B.MT7", 1.0f);
        LoadBlurModelForCharacter(SFJF_AW3, nullptr, 0.2f);
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudience();
            if (work.audienceTask) {
                StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->soundCentre = SFJF_STAGE_ORIGIN;
                audience->state = 2;
                audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = SFJF_AW3;
            }
        }
        work.state = threeRound ? 4 : work.startParameters->scene == 99 ? 5 : 1;
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        return;
    case 1:
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        EV_EnableCharacterDisplay(SFJF_SFA);
        EV_EnsureCharacterDraw(SFJF_SFA);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1791))
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(-16));
        if (IsAuthorStateFlagSet(14))
            EV_SetAuthorActiveTo(1);
        QueueAuthor(SHEN_NAME(AU_0));
        work.state = 2;
        return;
    case 2:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            CheckIsArgEquZero(1);
            work.state = 3;
        }
        return;
    case 3:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_DisableCharacterDisplay(SFJF_SFA);
            ToggleScenarioCharacterFlag(SFJF_SFA);
            work.state = 4;
        }
        return;
    case 4: {
        SetMapControlStatusFlag(0, 0);
        EV_LoadAudio("0256A000", 1);
        EV_AT_SetPos(SFJF_RE1, EventTransformFlags::World | EventTransformFlags::AllAxes, {261.79998779296875f, 85.5999984741211f, 233.22999572753906f});
        EV_AT_SetAngY(SFJF_RE1, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 49825, 0});
        EVT_AT_SetMot(SFJF_RE1, 0xf003, 0, 1, 0x40000000U, 0);
        EventSetPosition position{0, SFJF_BASE, STREET_FIGHT_INVALID, SFJF_STAGE_ORIGIN, 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{0, SFJF_BASE, STREET_FIGHT_INVALID, 0, 0};
        EVT_AT_SetAngY_Callback(&angle);
        AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFJF_BASE);
        AT_SetLocalBaseChara(SFJF_AW3, SFJF_BASE);
        EVT_AT_SetMot(STREET_FIGHT_RYO, 0x132, 0, 1, 0x40000000U, 0);
        position = {0, STREET_FIGHT_RYO, SFJF_BASE, {0, 0, -1.1f}, 0};
        EVT_AT_SetPosRot(&position);
        angle = {0, STREET_FIGHT_RYO, SFJF_BASE, 180, 0};
        EVT_AT_SetAngY_Callback(&angle);
        EVT_AT_SetMot(fighter, 0xba7, 0, -1, 0x40000000U, 0);
        position = {0, fighter, SFJF_BASE, {0, 0, 1.1f}, 0};
        EVT_AT_SetPosRot(&position);
        angle = {0, fighter, SFJF_BASE, 0, 0};
        EVT_AT_SetAngY_Callback(&angle);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SFJF_AW3), 1, 1, 1);
        if (DoesScenarioCharacterExist(SFJF_CHN)) {
            if (!threeRound) {
                EV_AT_SetPos(SFJF_CHN, EventTransformFlags::World | EventTransformFlags::AllAxes, {265.5f, 85.5999984741211f, 228.89999389648438f});
                work.state = 5;
                return;
            }
            EV_DisableCharacterDisplay(SFJF_CHN);
            ToggleScenarioCharacterFlag(SFJF_CHN);
        }
        work.state = 5;
        return;
    }
    case 5:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            work.fadeFrames = 30;
            if (!threeRound) {
                EventFilmControl film{0, work.fadeFrames, 255, 0, 0, 0, 0, 0, 0, 0, 0};
                EVT_AT_MenCtrl_Callback(&film);
                if (work.startParameters->scene == 99) {
                    work.state = 11;
                    return;
                }
            }
            EventSetCamera camera{0, 1401, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, {}, 0, 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
            work.state = 6;
        }
        return;
    case 6: {
        const char *const messages[]{"｡ﾖ｣ﾒ｣ﾏ｣ﾕ｣ﾎ｣ﾄ ｣ｱ｡ﾗ", "｡ﾖ｣ﾒ｣ﾏ｣ﾕ｣ﾎ｣ﾄ ｣ｲ｡ﾗ", "｡ﾖ｣ﾆ｣ﾉ｣ﾎ｣ﾁ｣ﾌ ｣ﾒ｣ﾏ｣ﾕ｣ﾎ｣ﾄ｡ﾗ"};
        if (work.round >= 0 && work.round <= 2) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
            SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
            message->frames = 60;
            message->message = messages[work.round];
        }
        EVT_AT_SetVoice("0256F001", 1, 30);
        EventFilmControl film{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
        EVT_AT_MenCtrl_Callback(&film);
        work.state = 7;
        return;
    }
    case 7: {
        SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
        SetScenarioCharacterType(SFJF_AW3, CharacterTypeStencilShadow);
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        HLib::SetupTaskDelay(10);
        SetAudienceResult(work, 4, 0);
        shenStartFGST(10, 10, 0, 0);
        vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QJBT"), 1);
        work.rank =
            static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
        std::int32_t difficulty = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{129}, 0, -1));
        if (difficulty > 2)
            difficulty = 2;
        shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 300, 0);
        const std::uint16_t hp = static_cast<std::uint16_t>(threeRound ? 80 * work.rank + 300 : 100 * (5 - difficulty));
        shenInitFightData(FindCharacterByScenarioName(SFJF_AW3), nullptr, 1, 1, hp, 0x17);
        vfUpdateFlagsFor(FindCharacterByScenarioName(SFJF_AW3), 0, 0x8000000U, difficulty);
        if (threeRound) {
            const std::int32_t level = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{65}, 0, -1));
            vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(level + 1), 1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
        }
        vfCopyPosFromEntity(0, nullptr, SFJF_STAGE_ORIGIN.x, SFJF_STAGE_ORIGIN.z, 2.5f);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
        vfmDispatchControl(FightControlOp::RoundCounts, shen_name{static_cast<std::uint32_t>(static_cast<std::int32_t>(work.wins))},
            static_cast<std::uintptr_t>(static_cast<std::int32_t>(work.losses)), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x8000}, 1);
        work.audienceResultPublished = 0;
        work.state = 8;
        return;
    }
    case 8:
        if (!camCtrlCheckIsEventCam()) {
            EV_PlayDialogue("0256F002");
            HLib::SetupTaskDelay(10);
            work.state = 9;
        }
        return;
    case 9: {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
        vfmQueueFightStart(2, 0x63, 1, 8, -1);
        EV_SetCharacterAlphaEnabled(SFJF_AW3, false);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1791, 1);
        SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
        if (threeRound) {
            constexpr std::uint8_t parameters[6][12]{{0x55, 0x6f, 0x6f, 0x6f, 0, 0x20, 0, 0x20, 0x70, 0, 0, 0},
                {0x45, 0x78, 0x78, 0x78, 0, 0x30, 0, 0x28, 0x74, 0, 0, 0}, {0x35, 0x8f, 0x8f, 0x8f, 8, 0x38, 0, 0x30, 0x78, 0, 0, 0},
                {0x25, 0x98, 0x98, 0x98, 0x20, 0x40, 0, 0x3f, 0x7b, 0, 0, 0}, {0x20, 0xaf, 0xaf, 0xaf, 0x28, 0x50, 0, 0x40, 0x7f, 0, 0, 0},
                {0x10, 0xbf, 0xbf, 0xbf, 0x30, 0x60, 0, 0x50, 0x88, 0, 0, 0}};
            const int rank = work.rank;
            const int index = rank <= 1 ? 0 : rank <= 3 ? 1 : rank <= 6 ? 2 : rank <= 9 ? 3 : rank <= 12 ? 4 : rank <= 15 ? 5 : 0;
            SetFightActorParameterBytes(SFJF_AW3, parameters[index]);
        }
        work.state = 10;
        return;
    }
    case 10:
        if (!IsFightActive()) {
            work.battleStatus = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
            const std::int64_t result = RecordStreetFightRound(&work);
            if (threeRound && result) {
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                spmFadeOut();
                HLib::SetupTaskDelay(30);
                work.state = work.battleStatus == 3 ? 25 : 12;
                return;
            }
            if (!threeRound && result) {
                work.state = 11;
                if (result == 1)
                    spmFadeOut();
            } else
                work.state = 4;
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(30);
            EV_NPC_NewLoadSw(0);
        } else if (!work.audienceResultPublished) {
            const std::int32_t result = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
            if (result == 3) {
                SetAudienceResult(work, 6, 2);
                ++work.audienceResultPublished;
            } else if (result == 2) {
                SetAudienceResult(work, 7, 1);
                ++work.audienceResultPublished;
            }
        }
        return;
    case 11:
        if (!filmCheckFadeTrackColour()) {
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 1);
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SFJF_AW3), 1, 1, 1);
            if (work.battleStatus == 3) {
                EV_DisableCharacterDisplay(SFJF_SFA);
                RecordStreetFightVictory(work.stage);
                UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
                UnparentAndSetExistingPosRot(fighter);
                EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
                QueueAuthor(SHEN_NAME(AU_1));
            } else if (work.startParameters->scene == 99)
                QueueAuthor(SHEN_NAME(AU_1));
            else
                RecordStreetFightDefeat(work.stage);
            work.state = work.battleStatus == 3 || work.startParameters->scene == 99 ? 18 : 12;
        }
        return;
    case 12:
        if (IsAuthorStateFlagSet(14)) {
            sfjfConversationSkipEnabled = 1;
            sfjfConversationSkipCallback = nullptr;
        }
        QueueSfjfStreetFightConversation(DefeatPoseConversation, &work);
        ++work.attempts;
        SetAuthorStateFlag(14, 1);
        work.state = 13;
        return;
    case 13:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            if (audience)
                audience->mode |= 2;
        }
        work.state = 14;
        return;
    case 14:
        if (!filmCheckFadeTrackColour()) {
            EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
            work.state = threeRound ? 25 : 15;
        }
        return;
    case 15:
        EV_ResetFreeTalkFlags();
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
        work.state = 16;
        work.frameCounter = 60;
        return;
    case 16:
        if (work.frameCounter-- == 1) {
            ST_LoadTitleFade();
            work.frameCounter = 60;
            work.state = 17;
        }
        return;
    case 17:
        if (work.frameCounter-- == 1)
            work.state = 25;
        return;
    case 18:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 19;
        return;
    case 19:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
            if (frame == 960) {
                for (shen_name name : {SHEN_NAME(P03Q), SHEN_NAME(Y01Q)})
                    if (DoesScenarioCharacterExist(name))
                        EV_DisableCharacterDisplay(name);
            } else if (frame == 1000)
                AddCashOrCoinsWithStep(1000, 2, 100);
        } else {
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            work.state = 20;
        }
        return;
    case 20:
        if (CanPerformForCurrentAreaFlags())
            CommitStreetFightStoryRank(work.stage);
        work.state = 25;
        return;
    case 25:
        EV_debug_log("SFJF\\eSFJF_et_vs_jefry.c", 681, "SF_BTM\n");
        HLib::SetCurrentTaskTo(threeRound ? RetryBeginCallback : CleanupNormalCallback);
        return;
    default:
        return;
    }
}

// @verified - p
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->fighterIndex = 0;
    work->fighters[0] = SFJF_AW3;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->motions[0] = 0;
    work->fadeFrames = 0;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFJF_CHARACTERS);
    work->audienceTask = nullptr;
    work->attempts = 0;
    work->reserved178 = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->frameCounter = 60;
    return work;
}

// @verified - p
void LoadNormalResources(StreetFightEventWork *work) {
    if (IsFightActive())
        check_dead_status(1);
    work->motions[0] = 0;
    RepositionPlayerForQronFloor(-3);
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QJBT"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QJBT"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QJBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFJF_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0256A000");
    EV_FreeAuthorPak(1, "0256");
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}

// @verified - p
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = start;
    LoadNormalResources(work);
    work->previousFarClip = camCtrlGetFarDistance();
    work->previousNearClip = camCtrlGetNearDistance();
}

void SfjfInit() {
    nullsub();
}
// @verified - p
void SfjfMain(std::uintptr_t argument) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(argument);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

} // namespace
const ScriptAreaDefinition areaSfjfScriptDefinition{.areaId = STREET_FIGHT_SFJF, .field04 = 0, .init = SfjfInit, .main = SfjfMain};
} // namespace shenmue::s2
