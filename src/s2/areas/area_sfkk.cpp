#include "s2/areas/area_sfkk.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/game_runtime.hpp"
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
#include "s2/battle_qte.hpp"
#include "s2/step_event.hpp"
#include "s2/advice.hpp"

namespace shenmue::s2 {
namespace {
std::int32_t choicePhase{};
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
void MainCallback(void *);
void ReadBookiePosition(shen_name character) {
    Vector3 position;
    EV_GetCharaPosition(character, 5, &position, EventTransformFlags::World);
}
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
    work->package = STREET_FIGHT_Q100;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFKK_DEBUG_CHARACTERS);
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
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFF"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFKK_BASE);
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
    EV_EnableCharacterDisplay(SFKK_FUY);
    EV_EnsureCharacterDraw(SFKK_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{0, SFKK_BASE, STREET_FIGHT_INVALID, SFKK_STAGE_ORIGIN, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, SFKK_BASE, STREET_FIGHT_INVALID, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKK_BASE);
    AT_SetLocalBaseChara(SFKK_FUY, SFKK_BASE);
    AT_SetLocalBaseChara(fighter, SFKK_BASE);
    position = {0, STREET_FIGHT_RYO, SFKK_BASE, SFKK_DEBUG_PLAYER_POSITION, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, STREET_FIGHT_RYO, SFKK_BASE, 225, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, SFKK_FUY, SFKK_BASE, work->bookiePosition, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, SFKK_FUY, SFKK_BASE, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, fighter, SFKK_BASE, {1.5f, 0, -1}, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, fighter, SFKK_BASE, 17, 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFKK_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

// @verified - p
void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFKK_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKK_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKK_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
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
        work.audienceTask = QueueStreetFightAudienceCount(30);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        Vector3 stage;
        EV_GetCharaPosition(SFKK_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFKK_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFKK_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFKK_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFKK_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFKK_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFKK_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFKK_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfkkStreetFightConversation(StartStreetFightBookieConversation, &work);
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
        ReadBookiePosition(SFKK_FUY);
        const std::uint32_t choice = ControlChoice(1);
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
        QueueSfkkStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfkkStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfkkStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFKK_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfkkStreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSfkkStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfkkStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfkkStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFKK_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfkkStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfkkStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfkkStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfkkStreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFKK_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, STREET_FIGHT_RYO, SFKK_BASE, 180, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, SFKK_FUY, SFKK_BASE, SFKK_DEBUG_BATTLE_BOOKIE_POSITION, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, SFKK_FUY, SFKK_BASE, 180, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {0, fighter, SFKK_BASE, {0, 0, 1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    angle = {0, fighter, SFKK_BASE, 0, 0};
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
    EventSetCamera camera{0, 2001, SFKK_BASE, SFKK_BASE, 0, {}, 0, 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFF"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFKK_DEBUG_OPPONENT_ORIGIN.x, SFKK_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKK_BASE);
    AT_SetLocalBaseChara(fighter, SFKK_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFKK_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

// @verified - p
std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, SFKK_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFKK_BASE, SFKK_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFKK_BASE, SFKK_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFKK_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFKK_FUY, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_FUY, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_FUY, 0x1aed, 0, 1.1f, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = SFKK_DEBUG_PLAYER_AGREE;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = SFKK_DEBUG_BOOKIE_WAIT;
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = SFKK_DEBUG_BOOKIE_INTRODUCE;
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = SFKK_DEBUG_BOOKIE_CHALLENGER;
    fourth->frames = 60;
    EVT_AT_SetCam(2006, SFKK_BASE, SFKK_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFKK_BASE, SFKK_BASE, 0, 0, 150);
    return 210;
}

// @verified - p
std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, SFKK_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKK_FUY, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_FUY, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, SFKK_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = SFKK_DEBUG_BOOKIE_ANYONE;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = SFKK_DEBUG_BOOKIE_NO_CHALLENGER;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = SFKK_DEBUG_BOOKIE_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFKK_BASE, SFKK_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180});
    return 240;
}

// @verified - p
std::int32_t DebugStopConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKK_FUY, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_FUY, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, SFKK_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = SFKK_DEBUG_PLAYER_STOP;
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = SFKK_DEBUG_BOOKIE_ACCEPT_STOP;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = SFKK_DEBUG_BOOKIE_STOP_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFKK_BASE, SFKK_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180});
    return 240;
}

std::int32_t qtePhase{}, qteFrames{}, qteFallbackEnabled{}, scmanTalkPhase{};
std::uint16_t motionVoiceIndex{};
Vector3 storyPlayerPosition{};
std::int32_t storyPlayerAngle{};
inline constexpr std::int32_t entryFees[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};

void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattlePreInitCallback(void *);
void BattleStartCallback(void *);
void BattleReadyGoCallback(void *);
void BattleStartFightCallback(void *);
void BattleWaitCallback(void *);
void BattleWaitFadeCallback(void *);
void BattleEndCallback(void *);

void SetAudienceState(StreetFightEventWork &work, std::uint8_t state, std::int16_t delay) {
    if (!work.audienceTask)
        return;
    auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
    if (audience) {
        audience->state = state;
        audience->loopDelay = delay;
    }
}
void QueueObjectDisplay(shen_name character, std::int32_t mode, float delay) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    auto *work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->character = character;
    work->mode = mode;
    work->delay = delay;
}
void SetCamera(std::int32_t index) {
    EVT_AT_SetCam(index, SFKK_BASE, SFKK_BASE, 0, 0, 0);
    camCtrlDisableFilmBars();
}
const char *FeeVoice(std::int32_t fee) {
    switch (fee) {
    case 10:
        return "0100E007"; // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
    case 20:
        return "0100E008"; // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
    case 30:
        return "0100E009"; // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
    case 40:
        return "0100E010"; // @text EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！"
    default:
        return "0100E004"; // @text EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
    }
}
// @verified - p
void PlaceNormal(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_EnableCharacterDisplay(SFKK_SD6);
    EV_EnsureCharacterDraw(SFKK_SD6);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work.playerPosition = {-3.3f, -0.5f, -5.5f};
    work.bookiePosition = {-1.864f, 0, -1.8f};
    EVT_AT_SetPos(SFKK_BASE, STREET_FIGHT_INVALID, SFKK_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFKK_BASE, STREET_FIGHT_INVALID, 0, 0);
    for (shen_name name : {STREET_FIGHT_RYO, SFKK_SD6, fighter}) {
        AT_SetLocalBaseChara(name, SFKK_BASE);
        SetScenarioCharacterType(name, CharacterTypeStencilShadow);
    }
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, SFKK_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 225, 0);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 0, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, {1.5f, 0, -1}, 0);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFKK_SD6), FacialNodeReplacement);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    adfsSetCameraIdxForChara(SFKK_SD6, SHEN_NAME(dumA));
    adfsSetCameraIdxForChara(fighter, SHEN_NAME(dumM));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
}
// @verified - p
std::int32_t FirstIntroConversation(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_SD6, 6849, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, {0.5f, -0.1f, -0.5f}, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, {-0.98799998f, 0, -2.2f}, 370);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 0);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 370);
    EVT_AT_SetMot(fighter, 6850, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(fighter, 6853, 0, 0.8f, 0x50000000U, 370);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2});
    EVT_AT_SetVoice("0100E001", 1, 30); // @text EN: "Now, Enjiang Zhou here is one of the\nbest masters of Acrobat Style martial arts." | JA: "さあさあ、この周恩将こそは\n当代きっての軽功の達人だ。"
    EVT_AT_SetVoice("0100E002", 1, 220); // @text EN: "Swift as a flying swallow,\nleaping as high as a monkey." | JA: "機敏なること飛燕のごとく。\n跳ねること猿のごとく。"
    EVT_AT_SetVoice("0100E003", 1, 390); // @text EN: "Catch this man here,\nand be the winner!" | JA: "この周恩将を見事\n捕まえたら勝ちだ。"
    EVT_AT_SetVoice(FeeVoice(entryFees[work.rank]), 1, 510);
    if (work.rank > 2)
        EVT_AT_SetVoice("0100E006", 1, 600); // @text EN: "Anyone confident enough to try?\nWhere's the challenger?" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    else {
        EVT_AT_SetVoice("0100E005", 1, 600); // @text EN: "Time limit, thirty seconds." | JA: "制限時間は３０秒。"
        EVT_AT_SetVoice("0100E006", 1, 660); // @text EN: "Anyone confident enough to try?\nWhere's the challenger?" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    }
    SetCamera(8020);
    EVT_AT_SetCam(2201, SFKK_BASE, SFKK_BASE, 0, 0, 120);
    EVT_AT_SetCam(106, SFKK_BASE, SFKK_BASE, 0, 0, 370);
    const std::int32_t delay = work.rank > 2 ? 600 : 700;
    QueueObjectDisplay(SFKK_SD6, 1, static_cast<float>(delay));
    EVT_AT_SetCam(2303, SFKK_BASE, SFKK_BASE, 0, 0, delay);
    return work.rank > 2 ? 660 : 720;
}
// @verified - p
std::int32_t RepeatIntroConversation(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFKK_SD6);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_SD6, 6849, 0, 1.1f, 0x40000000U, 0);
    QueueStreetFightLook(SFKK_SD6, STREET_FIGHT_RYO)->reserved08 = 15;
    EVT_AT_SetPos(fighter, SFKK_BASE, {-0.98799998f, 0, -2.2f}, 0);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6853, 0, 0.8f, 0x50000000U, 0);
    QueueStreetFightLook(fighter, STREET_FIGHT_RYO);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2});
    EVT_AT_SetVoice(FeeVoice(entryFees[work.rank]), 1, 0);
    EVT_AT_SetVoice("0100E006", 1, 91); // @text EN: "Anyone confident enough to try?\nWhere's the challenger?" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    EV_DisableCharacterDisplay(SFKK_SD6);
    SetCamera(2303);
    return 160;
}
// @verified - p
std::int32_t AcceptConversation(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6854, 0, 1, 0x40000000U, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    SetAudienceState(work, 4, 60);
    if (vfmDispatchControl(FightControlOp::MotionNibble, shen_name{130}, 0, -1) || !work.fadeFrames) {
        EVT_AT_SetVoice("0100E011", 1, 60); // @text EN: "Get up here." | JA: "よし。上がりな。"
        EVT_AT_SetVoice("0100N030", 1, 100); // @text EN: "Can you keep up with my speed?" | JA: "俺の動きについてこれるかな。"
        EVT_AT_SetCam(141, SFKK_BASE, SFKK_BASE, 0, 0, 50);
    } else {
        EVT_AT_SetVoice("0100E011", 1, 75); // @text EN: "Get up here." | JA: "よし。上がりな。"
        EVT_AT_SetVoice("0100N023", 1, 120); // @text EN: "I will show you\nwhat the Acrobat Style can do." | JA: "軽功の真髄を見せてやろう。"
        EVT_AT_SetCam(2200, SFKK_BASE, SFKK_BASE, 0, 0, 65);
    }
    EVT_AT_SetPoint(SFKK_SD6, STREET_FIGHT_RYO, 5, 3, 0, 0, 210, 0);
    EVT_AT_SetPoint(fighter, STREET_FIGHT_RYO, 5, 3, 0, 30, 210, 0);
    SetCamera(130);
    return 210;
}
// @verified - p
std::int32_t DeclineConversation(void *) {
    SetCamera(108);
    switch (shenGetRandomInt32(3)) {
    case 0:
        EVT_AT_SetVoice("0100A038", 1, 0); // @text EN: "No, I don't think so." | JA: "やめておこう…"
        break;
    case 1:
        EVT_AT_SetVoice("0100A039", 1, 0); // @text EN: "Maybe next time." | JA: "またにするか…"
        break;
    case 2:
        EVT_AT_SetVoice("0100A040", 1, 0); // @text EN: "I might try next time." | JA: "また今度にするか…"
        break;
    }
    return 70;
}
// @verified - p
std::int32_t InsufficientConversation(void *) {
    SetCamera(108);
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    auto *work = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->cash = 0;
    work->delay = 60;
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    return 120;
}
// @verified - p
std::int64_t SelectFirstBattleVoice(void *parameter) {
    constexpr std::uint16_t voices[]{91, 92, 93, 94, 95, 96, 97, 98, 99};
    auto &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = voices[index];
    if (++index >= 9)
        index = 0;
    return voice;
}
// @verified - p
std::int64_t SelectSecondBattleVoice(void *) {
    constexpr std::uint16_t voices[]{100, 101, 102, 103, 104, 194, 195, 196, 197, 198};
    const std::uint16_t voice = voices[motionVoiceIndex];
    if (++motionVoiceIndex >= 10)
        motionVoiceIndex = 0;
    return voice;
}
// @verified - p
void ControlQteFallback(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (qtePhase) {
    case 0:
        ++qteFrames;
        if (qteFrames > 600 && !(GetFightTaskFlags() & 0x10U) && (EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 13) & 1U))
            ++qtePhase;
        break;
    case 1:
        EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
        qteBattleUtil(BattleQteMode::RetireAndFree);
        ++qtePhase;
        break;
    case 2:
        if (const auto result = qteBattleUtil(BattleQteMode::GetResult)) {
            if (result == 1) {
                vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000103U, 1);
                EV_vfBattleUtil(FightBattleUtilOp::ForceActorDefeat, fighter, 1);
            } else {
                vfmDispatchControl(FightControlOp::QueuedAction, fighter, 0x8000c35U, 1);
                EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
            }
            qtePhase = 3;
            qteFrames = 0;
        }
        break;
    case 3:
        qteBattleUtil(BattleQteMode::Start);
        ++qtePhase;
        break;
    default:
        break;
    }
}
// @verified - p
void BattleEntryCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 180, 0);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, {-3.3f, -0.5f, -5.5f}, 0);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 180, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, {0, 0, 1.1f}, 0);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 0, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(fighter), nullptr, 0);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
    HLib::SetupTaskDelay(30);
}
void BattlePreEventCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 1);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5});
    EV_spmBeginSampleExecute();
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6855, 0, 1, 0, 50);
    EVT_AT_SetMot(fighter, 6855, 0, 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    EVT_AT_SetVoice("0100E012", 1, 0); // @text EN: "Take your stance." | JA: "それじゃあ、かまえて。"
    SetCamera(2001);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, 500, 28);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    if (work.fadeFrames <= 2)
        vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x8000000U, work.fadeFrames);
    qteFallbackEnabled = 0;
    if (!vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1)) {
        qteFallbackEnabled = 1;
        if (vfmDispatchControl(FightControlOp::MotionNibble, shen_name{130}, 0, -1) < 3)
            qteFallbackEnabled = 0;
    }
    const auto level = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1);
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(level + 1), 1);
    vfCopyPosFromEntity(0, nullptr, 197, 129, 2.5f);
    HLib::SetCurrentTaskTo(BattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}
// @verified - p
void BattlePreInitCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    UnparentAndSetExistingPosRot(work.fighters[work.fighterIndex]);
    HLib::SetCurrentTaskTo(BattleStartCallback);
    HLib::SetupTaskDelay(10);
}
// @verified - p
void BattleStartCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFF"), 1);
    const auto rank = static_cast<std::uint16_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1));
    const auto time = rank > 2 ? static_cast<std::uint16_t>(32 - rank) : std::uint16_t{30};
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1);
    for (shen_name name : {STREET_FIGHT_RYO, fighter, SFKK_SD6})
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(name), FacialNodeOriginal);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeDefault);
    SetScenarioCharacterType(fighter, CharacterTypeDefault);
    for (std::uint32_t flag : {0x800U, 0x2000U, 0x4000U, 0x80000U})
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{flag}, 1);
    vfmQueueFightStart(2, 99, 1, 0, time);
    EV_SetCharacterAlphaEnabled(fighter, false);
    vfmDispatchControl(static_cast<FightControlOp>(3), {}, 50, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1790, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    EV_NPC_NewLoadSw(1);
    work.reserved138[1] = AllocateBattleDialogueCondition10(0, -2, STREET_FIGHT_RYO, fighter, 1, 100, 1);
    auto id = AllocateMotionBattleDialogue(8, -1, SFKK_OXO, {}, 3125, 1, 100, 1);
    AssignBattleDialogueCallback(id, SelectFirstBattleVoice);
    id = AllocateBattleDialogueCondition13(9, -1, SFKK_OXO, STREET_FIGHT_RYO, 2, 100, 1);
    AssignBattleDialogueCallback(id, SelectSecondBattleVoice);
    for (std::uint16_t animation : {std::uint16_t{150}, std::uint16_t{151}, std::uint16_t{3123}, std::uint16_t{3126}}) {
        id = AllocateMotionBattleDialogue(9, -1, SFKK_OXO, {}, animation, 2, 100, 1);
        AssignBattleDialogueCallback(id, SelectSecondBattleVoice);
    }
    qtePhase = qteFrames = 0;
    HLib::SetCurrentTaskTo(BattleReadyGoCallback);
}
// @verified - p
void BattleReadyGoCallback(void *) {
    EVT_AT_SetVoice("0100E013", 1, 0); // @text EN: "Ready=@go!" | JA: "レディ・ゴー！"
    HLib::SetCurrentTaskTo(BattleStartFightCallback);
    HLib::SetupTaskDelay(60);
}
// @verified - p
void BattleStartFightCallback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(BattleWaitCallback);
}
// @verified - p
void BattleWaitCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const auto level = static_cast<std::uint8_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1));
    if (!IsFightActive()) {
        work.battleStatus = (GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 3) + 2;
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
        return;
    }
    if (GetBattleDialogueCompletionCount(work.reserved138[1])) {
        GetFightCharacterFlag(2, FindCharacterByScenarioName(fighter), 1);
        EVT_AT_SetVoice("0100A026", 1, 0); // @text EN: "I caught you!" | JA: "捕まえたぞ！"
        EVT_AT_SetVoice("0100N019", 1, 30); // @text EN: "Damn!" | JA: "しまった！"
        EVT_AT_SetVoice("0100E014", 1, 65); // @text EN: "We got a winner!" | JA: "勝負あり！"
        SetAudienceState(work, 6, 31);
        ++work.audienceResultPublished;
        work.battleStatus = 3;
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
    }
    if ((EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 13) & 0x6000009U) &&
        (level <= 2 || !(EV_vfBattleUtil(FightBattleUtilOp::ActorField, fighter, 1) & 0x20U)))
        ResetStreetFighterHUD();
    if (work.audienceResultPublished)
        return;
    const std::uint32_t result = static_cast<std::uint32_t>(GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)));
    if (result == 3) {
        SetAudienceState(work, 6, 2);
        ++work.audienceResultPublished;
    } else if (result - 5U <= 2U) {
        EVT_AT_SetVoice("0100E019", 1, 0); // @text EN: "Game set." | JA: "はい、そこまで…"
        SetAudienceState(work, 7, 10);
        ++work.audienceResultPublished;
    } else if (qteFallbackEnabled)
        ControlQteFallback(work);
}
// @verified - p
void BattleWaitFadeCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        spmFadeOut();
        EVT_AT_MenCtrl({0, 30, 0, 0, 0, 0, 255, 0, 0, 0, 0});
        HLib::SetCurrentTaskTo(BattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
    work.frameCounter = 0;
}
void BattleEndCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), Angry, 1.0f, 1.0f);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), Angry, 1.0f, 1.0f);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFKK_BASE);
    AT_SetLocalBaseChara(fighter, SFKK_BASE);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFKK_SD6), FacialNodeReplacement);
    for (shen_name name : {STREET_FIGHT_RYO, SFKK_SD6, fighter})
        EV_SetCharacterAlphaEnabled(name, false);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFKK_SD6), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFKK_SD6), 1, 1, 0);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    HLib::SetCurrentTaskTo(MainCallback);
    work.frameCounter = 0;
}

// @verified - p
std::int32_t VictoryConversation(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 0, 0);
    EVT_AT_SetMot(SFKK_SD6, 6849, 0, 1.1f, 0, 0);
    EV_EnableCharacterDisplay(SFKK_SD6);
    EVT_AT_SetPos(fighter, SFKK_BASE, {-1.288f, 0, -2.2f}, 0);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6852, 0, 1, 0x50000000U, 0);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5});
    PATI_StartMoneyCount(static_cast<std::uint32_t>(entryFees[work.rank]), 205);
    EVT_AT_SetVoice(work.fadeFrames ? "0100N020" : "0100N024", 1, 35); // @text 0100N020 EN: "I'm beaten.\nTerrific move." | JA: "まいった。\nいい動きだったぜ。" ; 0100N024 EN: "Damn, you read my moves." | JA: "くそっ、俺の動きを\n読まれるとは…"
    EVT_AT_SetVoice(work.fadeFrames ? "0100E016" : "0100E015", 1, work.fadeFrames ? 110 : 120); // @text 0100E016 EN: "I'm impressed.\nHere." | JA: "たいしたもんだ。\nほらよ。" ; 0100E015 EN: "Amazing.\nHere you go." | JA: "やるなぁ、兄ちゃん。\nほらよ。"
    EVT_AT_SetCam(616, SFKK_BASE, SFKK_BASE, 0, 0, 5);
    EVT_AT_SetCam(210, SFKK_BASE, SFKK_BASE, 0, 0, 210);
    return 350;
}
// @verified - p
std::int32_t PostVictoryScmanConversation(void *) {
    EV_EnableCharacterDisplay(SFKK_CHN);
    EV_EnsureCharacterDraw(SFKK_CHN);
    EV_SetCharacterAlphaEnabled(SFKK_CHN, false);
    EVT_AT_SetPos(SFKK_CHN, STREET_FIGHT_INVALID, {205.97999572753906f, 100, 124.97000122070312f}, 0);
    EVT_AT_SetAngY(SFKK_CHN, STREET_FIGHT_INVALID, 110, 0);
    EVT_AT_SetMot(SFKK_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    EventSetCamera camera{.cleanup = 0, .camera = 9901, .eye = SFKK_CHN, .target = SFKK_CHN, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 60;
}
// @verified - p
void ScmanLookCallback(void *) {
    if (scmanTalkPhase == 45) {
        Vector3 position;
        EV_GetCharaPosition(SFKK_CHN, 7, &position, EventTransformFlags::World);
        SetEyeToChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), &position, 15, 2);
    } else if (scmanTalkPhase == 299) {
        SetEyeToChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 16, 0);
        HLib::DestroyCurrentTask();
    }
    ++scmanTalkPhase;
}
// @verified - p
std::int32_t StoryScmanConversation(void *) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    ProcessAicaCommand(0x1200a0U, 0);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2});
    EV_SetCharacterAlphaEnabled(SFKK_CHN, false);
    EVT_AT_SetPos(SFKK_BASE, STREET_FIGHT_INVALID, {191.8000030517578f, 100, 127.69999694824219f}, 0);
    EVT_AT_SetAngY(SFKK_BASE, STREET_FIGHT_INVALID, 270, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, storyPlayerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, static_cast<float>(storyPlayerAngle), 0);
    EVT_AT_SetPos(SFKK_CHN, SFKK_BASE, {0.10000002384185791f, 0, 0.40000009536743164f}, 0);
    EVT_AT_SetAngY(SFKK_CHN, SFKK_BASE, 270, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 9673, 0, 1, 0x60000000U, 0);
    EVT_AT_SetMot(SFKK_CHN, 9669, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(SFKK_CHN, 9670, 0, 1, 0x40000000U, 100);
    EV_DisableCharacterDisplay(SHEN_NAME(OWO_));
    EV_DisableCharacterDisplay(SFKK_SD6);
    QueueObjectDisplay(SFKK_CHN, 1, 180);
    SetCamera(9910);
    EVT_AT_SetCam(9911, SFKK_BASE, SFKK_BASE, 0, 0, 230);
    EVT_AT_SetVoice("0255B001", 1, 30); // @text EN: "Come to Dragon St at sundown." | JA: "陽が落ちてから龍影街に来な…"
    EVT_AT_SetVoice("0255A004", 1, 160); // @text EN: "Dragon St=@" | JA: "龍影街…"
    scmanTalkPhase = 0;
    HLib::EnqueueTaskWithoutParameter(ScmanLookCallback, 5, 0x0b, static_cast<std::int32_t>(STREET_FIGHT_EVENT_TASK.name));
    return 300;
}
// @verified - p
void SkipScmanConversation() {
    EV_DisableCharacterDisplay(SFKK_CHN);
    SetCamera(9911);
}
// @verified - p
std::int32_t DefeatConversation(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFKK_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFKK_BASE, 180, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EV_EnableCharacterDisplay(SFKK_SD6);
    EVT_AT_SetPos(SFKK_SD6, SFKK_BASE, work.bookiePosition, 1);
    EVT_AT_SetAngY(SFKK_SD6, SFKK_BASE, 0, 1);
    EVT_AT_SetMot(SFKK_SD6, 6849, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFKK_BASE, {-0.98799998f, 0, -1.2f}, 1);
    EVT_AT_SetAngY(fighter, SFKK_BASE, 17, 1);
    EVT_AT_SetMot(fighter, 6851, 0, 1, 0x40000000U, 0);
    EVT_AT_MenCtrl({1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5});
    if (work.fadeFrames) {
        EVT_AT_SetVoice("0100E018", 1, 35); // @text EN: "Too bad.\nBut you came close." | JA: "おしかったな、兄ちゃん。\nでも、いいセンいってたぜ。"
        EVT_AT_SetVoice("0100N025", 1, 140); // @text EN: "That's the difference in our kung fu." | JA: "これが功夫の差ってやつだ。"
        EVT_AT_SetVoice("0100N026", 1, 210); // @text EN: "Train yourself more before you\ncome back." | JA: "もっと修練して出直すんだな。"
    } else {
        EVT_AT_SetVoice("0100E017", 1, 35); // @text EN: "Too bad. Show us something more\nnext time." | JA: "残念だったな、兄ちゃん。\n今度は期待してるぜ。"
        EVT_AT_SetVoice("0100N021", 1, 140); // @text EN: "Your movements are still slow." | JA: "まだまだ動きがニブいな。"
        EVT_AT_SetVoice("0100N022", 1, 210); // @text EN: "You need more kung fu." | JA: "もっと功夫を積むことだ。"
    }
    SetCamera(107);
    EVT_AT_SetCam(310, SFKK_BASE, SFKK_BASE, 0, 0, 300);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-entryFees[work.rank]), 300);
    return 420;
}
void RestartNormal(StreetFightEventWork &work) {
    EV_ResetFreeTalkFlags();
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    work.state = 1;
    work.audienceResultPublished = 0;
    work.auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
    if (work.audienceTask) {
        auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
}
// @verified - p
void RetryBeginCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 724, 1);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI_Internal(choices, 0x7fff, -1, 2, 0);
    if (work.audienceTask) {
        auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}
// @verified - p
void RetryChoiceCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const auto choice = static_cast<std::uint16_t>(ControlChoice(1));
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82)
        RestartNormal(work);
    else
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
}
void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    auto *const start = work.startParameters;
    const auto returnToQron = work.reserved178;
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0);
    hndlSetFlags(FindCharacterByScenarioName(SFKK_SD6));
    SetNpcColdResetMode(1);
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFKK_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKK_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFKK_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (auto *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
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
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
void MainCallback(void *parameter) {
    auto &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0:
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(20);
            if (work.audienceTask) {
                auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience->fighters[audience->fighterCount++] = fighter;
            }
        }
        work.state = 1;
        return;
    case 1: {
        PlaceNormal(work);
        Vector3 centre;
        EV_GetCharaPosition(SFKK_BASE, CHARACTER_TRANSLATION_ROOT, &centre, EventTransformFlags::Relative);
        centre.y -= 0.5f;
        if (work.audienceTask) {
            auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = centre;
            audience->state = 2;
        }
        Vector3 player, bookie, first, second;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::Relative);
        EV_GetCharaPosition(SFKK_SD6, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::Relative);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFKK_SD6, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &first, EventTransformFlags::World);
        EV_GetCharaPosition(SFKK_SD6, CHARACTER_TRANSLATION_ROOT, &second, EventTransformFlags::World);
        if (work.audienceTask) {
            auto *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = first.x;
            audience->corridorStart.z = first.z;
            audience->corridorEnd.x = second.x;
            audience->corridorEnd.z = second.z;
            const float x = first.x - second.x, z = first.z - second.z;
            audience->corridorLength = std::sqrt(x * x + z * z);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, player);
        EV_AT_SetPos(SFKK_SD6, EventTransformFlags::AllAxes, bookie);
        SetLowQualityHands(FindCharacterByScenarioName(SFKK_SD6));
        SetHighModelXYZ(FindCharacterByScenarioName(SFKK_SD6), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFKK_SD6), 1, 1, 0);
        LoadBlurModelForCharacter(fighter, nullptr, 0.2f);
        shenStartFGST(10, 10, 0, 0);
        vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFF"), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
        shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::SetWait, 30, 7, 30);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 512);
        qteBattleUtil(BattleQteMode::ResetStates);
        qteBattleUtil(BattleQteMode::Queue);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.state = 2;
        return;
    }
    case 2:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            sfkkConversationSkipCallback = nullptr;
            sfkkConversationSkipEnabled = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 724) != 0;
            QueueSfkkStreetFightConversation(sfkkConversationSkipEnabled ? RepeatIntroConversation : FirstIntroConversation, &work);
            work.state = 3;
        }
        return;
    case 3:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 724, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 4;
        return;
    case 4: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFKK_SD6);
        const auto choice = static_cast<std::uint16_t>(ControlChoice(1));
        if (!choice)
            return;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        EV_EnableCharacterDisplay(SFKK_SD6);
        if (choice == 22) {
            if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= entryFees[work.rank]) {
                work.state = 6;
                work.paidEntry = 1;
            } else
                work.state = 8;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = 5;
        } else
            work.state = 7;
        return;
    }
    case 5:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 3;
        return;
    case 6:
        QueueSfkkStreetFightConversation(AcceptConversation, &work);
        work.state = 9;
        return;
    case 7:
        QueueSfkkStreetFightConversation(DeclineConversation, &work);
        ToggleStreetFighterUI();
        work.state = 20;
        return;
    case 8:
        QueueSfkkStreetFightConversation(InsufficientConversation, &work);
        ToggleStreetFighterUI();
        work.state = 20;
        return;
    case 9:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 10;
        return;
    case 10:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetCurrentTaskTo(BattleEntryCallback);
            work.state = 11;
        }
        return;
    case 11:
        work.state = work.battleStatus == 3 ? 12 : 19;
        return;
    case 12:
        QueueSfkkStreetFightConversation(VictoryConversation, &work);
        if (!CanPerformForCurrentAreaFlags())
            work.state = 20;
        else {
            const auto result = CommitStreetFightStoryRank(work.stage);
            work.state = result == 1 ? 13 : result == 2 ? 14 : 20;
        }
        return;
    case 13:
        QueueSfkkStreetFightConversation(PostVictoryScmanConversation, &work);
        work.state = EV_GetShenmueFlag(349) ? 14 : 20;
        return;
    case 14:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ProcessAicaCommand(0xaa0U, 30);
        work.state = 15;
        return;
    case 15:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetupTaskDelay(60);
            work.state = 16;
        }
        return;
    case 16:
        EV_FreeAuthorPak(1, "0255");
        EV_stepSetFlags(0, 349);
        if (GetCompletedGameCount()) {
            sfkkConversationSkipEnabled = 1;
            sfkkConversationSkipCallback = SkipScmanConversation;
        }
        QueueSfkkStreetFightConversation(StoryScmanConversation, &work);
        work.state = 17;
        return;
    case 17:
        EVT_AT_SetVoice("0255A006", 1, 0); // @text EN: "I'll go there." | JA: "行ってみるか…"
        EVT_AT_KsIcon_Enqueue(-1, 251, 253, -1, 10800, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 18;
        return;
    case 18:
        if (const auto choice = static_cast<std::uint16_t>(ControlChoice(1))) {
            if (choice == 251) {
                DateTime time;
                GetCurrentDateTime(time);
                time.minute = static_cast<std::int8_t>(static_cast<std::uint8_t>(time.minute) + 30U);
                SetCurrentDateTimeTo(time);
                work.reserved178 = 1;
            }
            work.state = 20;
            EV_FreeAuthorPak(0, "0255");
        }
        return;
    case 19:
        ResetStreetFightStoryFlag(work.stage);
        QueueSfkkStreetFightConversation(DefeatConversation, &work);
        work.state = 20;
        return;
    case 20:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 21;
        return;
    case 21:
        if (!filmCheckFadeTrackColour()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry &&
                !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{130}, 0, -1) == 2)
                QueueFightAdviceControl(static_cast<std::uint8_t>(AdviceControlMode::AcrobatMaster));
            work.state = 22;
        }
        return;
    case 22:
        if (!work.battleStatus || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            if (work.battleStatus == 3 && work.fadeFrames < 2)
                ++work.fadeFrames;
        } else if (work.battleStatus == 3) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 725, 1);
            if (work.fadeFrames < 2)
                ++work.fadeFrames;
        }
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        return;
    default:
        return;
    }
}
// @verified - p
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    auto *work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = SFKK_OXO;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->package = SHEN_NAME(Q200);
    work->battleFrames = 96;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFKK_CHARACTERS);
    work->audienceTask = nullptr;
    work->audienceResultPublished = 0;
    work->reserved178 = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    return work;
}
void LoadNormalResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFF"), 0)) {
        const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFF"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFF"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFKK_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    shenSetClockAndCompassRenderTo(1);
    FinishLoading();
}
// @verified - p
std::int64_t QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    EV_NPC_NewLoadSw(0);
    SetNpcColdResetMode(0);
    EV_ResetFreeTalkFlags();
    auto *task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    auto *work = InitializeNormal(task);
    work->startParameters = start;
    LoadNormalResources(work);
    const auto tier = static_cast<std::uint8_t>(start->scene);
    work->fadeFrames = tier > 3 ? 2 : tier;
    return 2;
}
// @verified - p
void Init() {
    qtePhase = qteFrames = qteFallbackEnabled = scmanTalkPhase = 0;
}
// @verified - p
void Main(std::uintptr_t parameter) {
    auto *start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

} // namespace
const ScriptAreaDefinition areaSfkkScriptDefinition{.areaId = STREET_FIGHT_SFKK, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
