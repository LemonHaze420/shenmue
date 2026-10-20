#include "s2/areas/area_sfts.hpp"
#include "s2/author.hpp"
#include "s2/event_call.hpp"
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
#include "s2/game_runtime.hpp"
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
#include "s2/model_component.hpp"
#include "s2/battle_qte.hpp"
#include "s2/step_event.hpp"
#include "s2/advice.hpp"
#include "s2/math.hpp"

namespace shenmue::s2 {
namespace {
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
std::int32_t choicePhase{};
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
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    default:
        return 0;
    }
}
std::uint32_t PollChoice() {
    if (!choicePhase) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 1) {
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 2) {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    return 0;
}
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
    work->package = SFTS_RESOURCE_PACKAGE;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFTS_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFE"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFTS_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}

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

void PlaceDebugCharacters(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(SFTS_FUY);
    EV_EnsureCharacterDraw(SFTS_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFTS_BASE, .parent = STREET_FIGHT_INVALID, .position = SFTS_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFTS_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 315.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTS_BASE);
    AT_SetLocalBaseChara(SFTS_FUY, SFTS_BASE);
    AT_SetLocalBaseChara(fighter, SFTS_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTS_BASE, .position = SFTS_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTS_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFTS_FUY, .parent = SFTS_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFTS_FUY, .parent = SFTS_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFTS_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFTS_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFTS_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFTS_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTS_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTS_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ENMY"), 0);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
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
        EV_GetCharaPosition(SFTS_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFTS_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFTS_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFTS_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFTS_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFTS_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFTS_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFTS_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSftsStreetFightConversation(StartStreetFightBookieConversation, &work);
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
        ReadBookiePosition(SFTS_FUY);
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
        QueueSftsStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSftsStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSftsStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFTS_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSftsStreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSftsStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSftsStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSftsStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFTS_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSftsStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSftsStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSftsStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSftsStreetFightConversation(DebugCompletedConversation, &work);
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

void DebugBattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EventSetPosition position{0, STREET_FIGHT_RYO, SFTS_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTS_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFTS_FUY, .parent = SFTS_BASE, .position = SFTS_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFTS_FUY, .parent = SFTS_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFTS_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFTS_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}

void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    const EventFilmControl film{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5};
    EVT_AT_MenCtrl(film);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1ae0, 0, 1, 0, 50);
    EV_PlayDialogue("0100B072");
    EventSetCamera camera{0, 2001, SFTS_BASE, SFTS_BASE, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::SetCurrentTaskTo(DebugBattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}

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

void DebugBattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    work.hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFE"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFTS_DEBUG_OPPONENT_ORIGIN.x, SFTS_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    vfmQueueFightStart(2, 0x63, 1, 8, 30);
    work.hudEnabled = 0;
    AccessFightActorFlag411(1, FindCharacterByScenarioName(fighter), definition->guardMode);
    AccessFightActorGroupFloat(1, FindCharacterByScenarioName(fighter), definition->strikeScale);
    AccessFightActorGroupFloat(3, FindCharacterByScenarioName(fighter), definition->guardScale);
    AccessFightActorGroupFloat(5, FindCharacterByScenarioName(fighter), definition->guardScale);
    HLib::SetCurrentTaskTo(DebugBattleReadyGoCallback);
}

void DebugBattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100B073");
    HLib::SetCurrentTaskTo(DebugBattleStartFightCallback);
    HLib::SetupTaskDelay(40);
}

void DebugBattleStartFightCallback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(DebugBattleWaitCallback);
}

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

void DebugBattleWaitFadeCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        ProcessAicaCommand(0x0aa0, 30);
        EventFilmControl film{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTS_BASE);
    AT_SetLocalBaseChara(fighter, SFTS_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFTS_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFTS_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFTS_FUY, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_FUY, SFTS_BASE, 0, 0);
    EVT_AT_SetMot(SFTS_FUY, 0x1aed, 0, 1.1f, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_ACCEPT;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_WAIT;
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_INVITATION;
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = STREET_FIGHT_DEBUG_CHALLENGE;
    fourth->frames = 60;
    EVT_AT_SetCam(2006, SFTS_BASE, SFTS_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFTS_BASE, SFTS_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTS_FUY, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_FUY, SFTS_BASE, 0, 0);
    EVT_AT_SetMot(SFTS_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_CALL;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_NO_OPPONENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFTS_BASE, SFTS_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}

std::int32_t DebugStopConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTS_FUY, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_FUY, SFTS_BASE, 0, 0);
    EVT_AT_SetMot(SFTS_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_STOP;
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_AGREEMENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_STOP_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFTS_BASE, SFTS_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}

std::int32_t firstStoryMatch{1}, qtePhase{}, qteDelay{};
void MainCallback(void *);
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattlePreInitCallback(void *);
void BattleStartCallback(void *);
void BattleWaitCallback(void *);
void BattleEndCallback(void *);
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t SecondAcceptConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t VictoryAppearanceConversation(void *);
std::int32_t DefeatConversation(void *);

void ConfigureBattleQte(std::int32_t rank) {
    const std::int16_t *const definition = SFTS_QTE_COMMANDS[rank];
    qteBattleUtil(BattleQteMode::SetWait, definition[0], definition[1], definition[2], 0);
    std::int32_t commands[8]{};
    for (std::int32_t index = 0; index < 8; ++index) {
        commands[index] =
            static_cast<std::uint16_t>(definition[4 + index * 2]) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(definition[5 + index * 2])) << 16);
        if (!commands[index])
            break;
    }
    qteBattleUtil(BattleQteMode::SetFirstCommands, commands[0], commands[1], commands[2], commands[3]);
    qteBattleUtil(BattleQteMode::SetLastCommands, commands[4], commands[5], commands[6], commands[7]);
}
void ResetQtePositions(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, SFTS_STAGE_ORIGIN, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, SFTS_STAGE_ORIGIN, 0);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(fighter), 71, 0, MotionRegion::LowerBody);
}
void PrepareLowRankQte(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x10}, 1);
    ResetQtePositions(work);
    vfmDispatchControl(FightControlOp::QueuedAction, fighter, 0x8000c51, 1);
    SetShowQteFlagForButtonType(512, SFTS_QTE_COMMANDS[work.rank][2], 0);
    SetQteButtonState(512);
}
void PrepareHighRankQte(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x10}, 1);
    ResetQtePositions(work);
    vfmDispatchControl(FightControlOp::QueuedAction, fighter, 0x8000c50, 1);
    SetShowQteFlagForButtonType(512, SFTS_QTE_COMMANDS[work.rank][2], 0);
    SetQteButtonState(512);
}
std::int64_t PollBattlePrompt() {
    const std::int16_t status = static_cast<std::int16_t>(GetCurrentQteStatus());
    if (status != -1) {
        if (status == -24576)
            return 1;
        if (status & 512) {
            vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000c5a, 1);
            return 1;
        }
    }
    return 0;
}
ConversationBuffer *RandomQteVoice() {
    const char *const voices[]{"0100Q009", "0100Q011", "0100Q013", "0100Q014"};
    return EV_PlayDialogue(voices[shenGetRandomInt32(4)]);
}
std::int64_t ControlBattleQte(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    const std::uint32_t guard = static_cast<std::uint32_t>(GetFightPlayerGuardEffectTimerSeconds());
    Vector3 playerPosition, fighterPosition;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &fighterPosition, EventTransformFlags::World);
    const float x = playerPosition.x - fighterPosition.x;
    const float z = playerPosition.z - fighterPosition.z;
    const float distanceSquared = z * z + x * x;
    EV_SetCharacterAlphaEnabled(fighter, false);
    switch (qtePhase) {
    case 0:
        if (guard <= 25 && !(GetFightTaskFlags() & 0x10U)) {
            const std::uint64_t fighterFlags = EV_vfBattleUtil(FightBattleUtilOp::ActorField, fighter, 0);
            if (!((EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) | fighterFlags) & 0xdc010U) &&
                static_cast<double>(distanceSquared) < 5.289999999999999) {
                if (firstStoryMatch)
                    EVT_AT_SetVoice("0100Q009", 1, 0); // @text EN: "Take this!" | JA: "食らえッ！"
                else
                    RandomQteVoice();
                if (static_cast<std::uint32_t>(work.rank) > 2)
                    PrepareHighRankQte(work);
                else
                    PrepareLowRankQte(work);
                ++qtePhase;
            }
        }
        break;
    case 1:
    case 4:
        if (PollBattlePrompt())
            ++qtePhase;
        break;
    case 2:
    case 5:
        if (SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 65, 1, MotionRegion::LowerBody)) {
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 0);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x10}, 0);
            qteDelay = qtePhase == 2 ? 180 : 90;
            ++qtePhase;
        }
        break;
    case 3:
        if (guard > 15 && qteDelay)
            break;
        if (guard <= 15)
            qteDelay = 0;
        if (guard > 17)
            return 1;
        if (!(GetFightTaskFlags() & 0x10U)) {
            const std::uint64_t fighterFlags = EV_vfBattleUtil(FightBattleUtilOp::ActorField, fighter, 0);
            if (!((EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) | fighterFlags) & 0xdc010U) &&
                static_cast<double>(distanceSquared) < 5.289999999999999) {
                RandomQteVoice();
                if (static_cast<std::uint32_t>(work.rank) > 2)
                    PrepareHighRankQte(work);
                else
                    PrepareLowRankQte(work);
                ++qtePhase;
            }
        }
        break;
    case 6:
        if (guard > 5 && qteDelay)
            break;
        if (guard <= 5)
            qteDelay = 0;
        if (guard > 7)
            return 1;
        if (!(GetFightTaskFlags() & 0x10U)) {
            const std::uint64_t fighterFlags = EV_vfBattleUtil(FightBattleUtilOp::ActorField, fighter, 0);
            if (!((EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 0) | fighterFlags) & 0xdc010U) &&
                static_cast<double>(distanceSquared) < 5.289999999999999) {
                ConfigureBattleQte(work.rank);
                qteBattleUtil(BattleQteMode::ResetStates);
                qteBattleUtil(BattleQteMode::Queue);
                RandomQteVoice();
                EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
                EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x10}, 1);
                ResetQtePositions(work);
                vfmDispatchControl(FightControlOp::QueuedAction, fighter, 0x8000c50, 1);
                ++qtePhase;
                qteDelay = 30;
            }
        }
        break;
    case 7:
        if (!qteDelay) {
            qteBattleUtil(BattleQteMode::Start);
            ++qtePhase;
        }
        break;
    case 8: {
        const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
        if (!result)
            break;
        if (result == 1) {
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
            ResetQtePositions(work);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, 90, 0);
            EVT_AT_SetAngY(fighter, STREET_FIGHT_INVALID, 90, 0);
            vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000c89, 1);
            vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 1, 0x20000, 0);
            EVT_AT_SetVoice("0100A083", 1, 0); // @text EN: "I see your move!" | JA: "見切ったぞ！"
            EVT_AT_SetVoice("0100H018", 1, 150); // @text EN: "Game!" | JA: "勝負あり！"
        }
        ++qtePhase;
        break;
    }
    default:
        break;
    }
    if (qteDelay)
        --qteDelay;
    return 1;
}

void SetAudienceState(StreetFightEventWork &work, std::uint8_t state, std::int16_t delay) {
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience) {
            audience->state = state;
            audience->loopDelay = delay;
        }
    }
}
void PlaceNormal(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_EnableCharacterDisplay(SFTS_VCN);
    EV_EnsureCharacterDraw(SFTS_VCN);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work.playerPosition = {-3.3f, -.5f, -5.5f};
    work.bookiePosition = {-1.564f, 0, -1.1f};
    EVT_AT_SetPos(SFTS_BASE, STREET_FIGHT_INVALID, SFTS_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFTS_BASE, STREET_FIGHT_INVALID, 315, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTS_BASE);
    AT_SetLocalBaseChara(SFTS_VCN, SFTS_BASE);
    AT_SetLocalBaseChara(fighter, SFTS_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_FIGHTER_INTRO_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 225, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 0, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFTS_VCN, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
    mtwkSetRequestedRootMode(scenFindMotionwork(STREET_FIGHT_RYO), 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(SFTS_VCN), 0);
    SetFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0);
    LoadBlurModelForCharacter(fighter, nullptr, .2f);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeReplacement);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    adfsSetCameraIdxForChara(SFTS_VCN, SFTS_BOOKIE_VOICE);
    adfsSetCameraIdxForChara(fighter, SFTS_FIGHTER_VOICE);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
}
void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0:
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = fighter;
                if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED)) {
                    audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                    if (audience)
                        audience->mode |= 1;
                }
            }
        }
        firstStoryMatch = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 736) == 0;
        work.state = 1;
        break;
    case 1: {
        PlaceNormal(work);
        Vector3 centre, savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(SFTS_BASE, CHARACTER_TRANSLATION_ROOT, &centre, EventTransformFlags::Relative);
        centre.y -= .5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = centre;
            audience->state = 2;
        }
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Relative);
        EV_GetCharaPosition(SFTS_VCN, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Relative);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFTS_VCN, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFTS_VCN, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            audience->corridorLength = GetPlanarDistance(&player, &bookie);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFTS_VCN, EventTransformFlags::AllAxes, savedBookie);
        SetLowQualityHands(FindCharacterByScenarioName(SFTS_VCN));
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 0, 0);
        SetLowQualityHands(FindCharacterByScenarioName(SFTS_KFT));
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_KFT), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_KFT), 1, 0, 0);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.state = 2;
        break;
    }
    case 2:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            sftsConversationSkipCallback = nullptr;
            sftsConversationSkipEnabled = !firstStoryMatch;
            QueueSftsStreetFightConversation(firstStoryMatch ? FirstIntroConversation : RepeatIntroConversation, &work);
            work.state = 3;
        }
        break;
    case 3:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 736, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 4;
        break;
    case 4: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFTS_VCN);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
        if (!choice)
            break;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 22) {
            if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= SFTS_ENTRY_FEES[work.rank]) {
                work.state = 6;
                work.paidEntry = 1;
            } else
                work.state = 9;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = 5;
        } else
            work.state = 8;
        break;
    }
    case 5:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 3;
        break;
    case 6:
        QueueSftsStreetFightConversation(AcceptConversation, &work);
        work.state = 7;
        break;
    case 7:
        QueueSftsStreetFightConversation(SecondAcceptConversation, &work);
        work.state = 10;
        break;
    case 8:
        QueueSftsStreetFightConversation(DeclineConversation, &work);
        work.state = 19;
        break;
    case 9:
        QueueSftsStreetFightConversation(InsufficientConversation, &work);
        work.state = 19;
        break;
    case 10:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 11;
        break;
    case 11:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetCurrentTaskTo(BattleEntryCallback);
            work.state = 12;
        }
        break;
    case 12:
        work.state = work.battleStatus == 3 ? 13 : 18;
        break;
    case 13:
        QueueSftsStreetFightConversation(VictoryConversation, &work);
        work.state = CanPerformForCurrentAreaFlags() && static_cast<std::uint32_t>(CommitStreetFightStoryRank(work.stage) - 1) <= 1 ? 14 : 19;
        break;
    case 14:
        QueueSftsStreetFightConversation(VictoryAppearanceConversation, &work);
        nullsub();
        work.state = 19;
        break;
    case 18:
        ResetStreetFightStoryFlag(work.stage);
        QueueSftsStreetFightConversation(DefeatConversation, &work);
        work.state = 19;
        break;
    case 19:
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        work.state = 20;
        break;
    case 20:
        if (!filmCheckFadeTrackColour()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry &&
                !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{75}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{139}, 0, -1) == 2)
                QueueFightAdviceControl(1);
            work.state = 21;
        }
        break;
    case 21:
        if (work.battleStatus && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.battleStatus == 3)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 737, 1);
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        break;
    default:
        break;
    }
}
void BattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, SFTS_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 180, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, {0, 0, 1.1f}, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 0, 0);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
    HLib::SetupTaskDelay(30);
}
void BattlePreEventCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 1);
    work.hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFE"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, 500, 30);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x20000, 250);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x400}, 1);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EV_spmBeginSampleExecute();
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, 1, 0, 0);
    EVT_AT_SetMot(fighter, 3130, 0, 1, 0, 0);
    EVT_AT_SetVoice("0100H015", 1, 10); // @text EN: "Take your stance!" | JA: "それじゃあ、かまえて。"
    EVT_AT_SetVoice("0100H016", 1, 110); // @text EN: "Ready=@go!" | JA: "レディ・ゴー！"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    EVT_AT_SetCam(2306, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    HLib::SetCurrentTaskTo(BattlePreInitCallback);
    HLib::SetupTaskDelay(150);
}
void BattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    UnparentAndSetExistingPosRot(work.fighters[work.fighterIndex]);
    HLib::SetCurrentTaskTo(BattleStartCallback);
    HLib::SetupTaskDelay(10);
}
void BattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const std::int32_t level = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{75}, 0, -1));
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(level + 1), 1);
    vfCopyPosFromEntity(0, nullptr, SFTS_STAGE_ORIGIN.x, SFTS_STAGE_ORIGIN.z, 2.5f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x4000}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(2, 99, 1, 8, 30);
    vfmDispatchControl(static_cast<FightControlOp>(3), {}, 50, 1);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1789, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    work.hudEnabled = 0;
    const std::int16_t voice = firstStoryMatch ? 154 : static_cast<std::int16_t>(202 + 2 * shenGetRandomInt32(3));
    AllocateMotionBattleDialogue(10, voice, STREET_FIGHT_RYO, fighter, 3162, 1, 100, 0);
    AllocateMotionBattleDialogue(10, 152, STREET_FIGHT_RYO, fighter, 617, 1, 100, 1);
    qtePhase = 0;
    HLib::SetCurrentTaskTo(BattleWaitCallback);
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (IsFightActive()) {
        ControlBattleQte(work);
        if (!work.audienceResultPublished) {
            const std::int32_t status = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
            if (status == 2 || status == 3) {
                SetAudienceState(work, status == 2 ? 7 : 6, status == 2 ? 1 : 2);
                ++work.audienceResultPublished;
            }
        }
    } else {
        qteBattleUtil(BattleQteMode::RetireAndFree);
        if (GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 3)
            work.battleStatus = 3;
        else {
            EV_PlayDialogue("0100H018");
            work.battleStatus = 2;
        }
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(BattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), FacialExpressionId::Angry, 1, 1);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTS_BASE);
    AT_SetLocalBaseChara(fighter, SFTS_BASE);
    SetScenarioCharacterTransparency(SFTS_VCN, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_KFT), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_KFT), 1, 0, 0);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    HLib::SetCurrentTaskTo(MainCallback);
}

std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0, 1, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 398, 0, 1, 0, 270);
    ReadBookiePosition(SFTS_VCN);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 200, 200);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_REPEAT_PLAYER_POSITION, 270);
    hmdlSetFlagsFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 2, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 2, 16);
    EV_SetCharacterAlphaEnabled(SFTS_VCN, false);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 40, 0);
    EVT_AT_SetMot(SFTS_VCN, 6857, 0, 1.1f, 0, 0);
    EVT_AT_SetMot(SFTS_VCN, 6855, 0, 1.1f, 0, 730);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_INTRO_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, -15, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1.2f, 0, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1.2f, 0, 310);
    EVT_AT_SetMot(fighter, 6850, 0, 1.2f, 0, 490);
    hmdlSetFlagsFor(FindCharacterByScenarioName(fighter), 0, 2, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(fighter), 1, 2, 16);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(fighter), 1, reinterpret_cast<std::byte *>(SFTS_SUMO_HAND), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(fighter), 0, reinterpret_cast<std::byte *>(SFTS_SUMO_HAND), 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = SFTS_VCN;
    highWork0->selector = 1;
    highWork0->state = 1;
    HLib::SetupTimedTask(highTask0, 220);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = SFTS_VCN;
    highWork1->selector = 0;
    highWork1->state = 1;
    HLib::SetupTimedTask(highTask1, 220);
    HLib::Task *const highTask2 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork2 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask2));
    highWork2->task = highTask2;
    highWork2->character = fighter;
    highWork2->selector = 1;
    highWork2->state = 1;
    HLib::SetupTimedTask(highTask2, 220);
    HLib::Task *const highTask3 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork3 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask3));
    highWork3->task = highTask3;
    highWork3->character = fighter;
    highWork3->selector = 0;
    highWork3->state = 1;
    HLib::SetupTimedTask(highTask3, 220);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 1, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 0, 220);
    Queue_EVT_AT_CtrlHandUp_1(fighter, 0, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 0, 220);
    HLib::Task *const highTask4 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork4 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask4));
    highWork4->task = highTask4;
    highWork4->character = SFTS_VCN;
    highWork4->selector = 1;
    highWork4->state = 0;
    HLib::SetupTimedTask(highTask4, 340);
    HLib::Task *const highTask5 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork5 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask5));
    highWork5->task = highTask5;
    highWork5->character = SFTS_VCN;
    highWork5->selector = 0;
    highWork5->state = 0;
    HLib::SetupTimedTask(highTask5, 340);
    HLib::Task *const highTask6 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork6 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask6));
    highWork6->task = highTask6;
    highWork6->character = SFTS_VCN;
    highWork6->selector = 1;
    highWork6->state = 1;
    HLib::SetupTimedTask(highTask6, 550);
    HLib::Task *const highTask7 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork7 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask7));
    highWork7->task = highTask7;
    highWork7->character = SFTS_VCN;
    highWork7->selector = 0;
    highWork7->state = 1;
    HLib::SetupTimedTask(highTask7, 550);
    HLib::Task *const highTask8 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork8 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask8));
    highWork8->task = highTask8;
    highWork8->character = fighter;
    highWork8->selector = 1;
    highWork8->state = 0;
    HLib::SetupTimedTask(highTask8, 550);
    HLib::Task *const highTask9 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork9 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask9));
    highWork9->task = highTask9;
    highWork9->character = fighter;
    highWork9->selector = 0;
    highWork9->state = 0;
    HLib::SetupTimedTask(highTask9, 550);
    HLib::Task *const highTask10 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork10 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask10));
    highWork10->task = highTask10;
    highWork10->character = SFTS_VCN;
    highWork10->selector = 1;
    highWork10->state = 0;
    HLib::SetupTimedTask(highTask10, 730);
    HLib::Task *const highTask11 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork11 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask11));
    highWork11->task = highTask11;
    highWork11->character = SFTS_VCN;
    highWork11->selector = 0;
    highWork11->state = 0;
    HLib::SetupTimedTask(highTask11, 730);
    HLib::Task *const highTask12 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork12 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask12));
    highWork12->task = highTask12;
    highWork12->character = fighter;
    highWork12->selector = 1;
    highWork12->state = 1;
    HLib::SetupTimedTask(highTask12, 730);
    HLib::Task *const highTask13 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork13 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask13));
    highWork13->task = highTask13;
    highWork13->character = fighter;
    highWork13->selector = 0;
    highWork13->state = 1;
    HLib::SetupTimedTask(highTask13, 730);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 2});
    EVT_AT_SetVoice("0100H001", 1, 30); // @text EN: "Come and see this!" | JA: "さあさあ、お立ち会い。"
    EVT_AT_SetVoice("0100H002", 1, 90); // @text EN: "Hanqing Qin here, is an expert of the\nIron Thrust Style." | JA: "ここにいる秦漢清こそは\n鉄砂掌の達人だぜ。"
    EVT_AT_SetVoice("0100H003", 1, 240); // @text EN: "The long years of training show in his\nsingle thrust that could knock down a bull." | JA: "長年きたえたその掌打は\n一撃で牛をも倒すってシロモノだ。"
    EVT_AT_SetVoice("0100H004", 1, 440); // @text EN: "Is there anyone strong enough\nto challenge this man?" | JA: "この男と勝負する\n腕自慢はいないか？"
    const std::int32_t fee = SFTS_ENTRY_FEES[work.rank];
    EVT_AT_SetVoice(fee == 10 ? "0100H008" : fee == 20 ? "0100H009" : fee == 30 ? "0100H010" : fee == 40 ? "0100H011" : "0100H005", 1, 560); // @text 0100H008 EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！" ; 0100H009 EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！" ; 0100H010 EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！" ; 0100H011 EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！" ; 0100H005 EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
    EVT_AT_SetVoice("0100H006", 1, 670); // @text EN: "The time limit is thirty seconds." | JA: "制限時間は３０秒。"
    EVT_AT_SetVoice("0100H007", 1, 730); // @text EN: "Anyone brave enough to challenge this man?" | JA: "さあ、誰か勇気ある\n挑戦者はいねえか？"
    EVT_AT_SetSound(1078955, 330);
    QueueCameraShake(.015f, 0, .80000001f, 334);
    EVT_AT_SetSound(1078955, 360);
    QueueCameraShake(.015f, 0, .80000001f, 364);
    EVT_AT_SetSound(1078955, 398);
    EVT_AT_SetSound(1078955, 400);
    QueueCameraShake(.015f, 0, .80000001f, 400);
    EVT_AT_SetCam(8098, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    EVT_AT_SetCam(2301, SFTS_BASE, SFTS_BASE, 0, 0, 220);
    EVT_AT_SetCam(2302, SFTS_BASE, SFTS_BASE, 0, 0, 550);
    EVT_AT_SetCam(2303, SFTS_BASE, SFTS_BASE, 0, 0, 730);
    return 820;
}
std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 398, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTS_VCN);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 200, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_REPEAT_PLAYER_POSITION, 0);
    hmdlSetFlagsFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 2, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 2, 16);
    EV_SetCharacterAlphaEnabled(SFTS_VCN, false);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 40, 0);
    EVT_AT_SetMot(SFTS_VCN, 6855, 0, 1.1f, 0x40000000U, 0);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_INTRO_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, -15, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1.2f, 0x40000000U, 0);
    hmdlSetFlagsFor(FindCharacterByScenarioName(fighter), 0, 2, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(fighter), 1, 2, 16);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 2});
    const std::int32_t fee = SFTS_ENTRY_FEES[work.rank];
    EVT_AT_SetVoice(fee == 10 ? "0100H008" : fee == 20 ? "0100H009" : fee == 30 ? "0100H010" : fee == 40 ? "0100H011" : "0100H005", 1, 0); // @text 0100H008 EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！" ; 0100H009 EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！" ; 0100H010 EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！" ; 0100H011 EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！" ; 0100H005 EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
    EVT_AT_SetVoice("0100H007", 1, 90); // @text EN: "Anyone brave enough to challenge this man?" | JA: "さあ、誰か勇気ある\n挑戦者はいねえか？"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    EVT_AT_SetCam(2303, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    return 200;
}

std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6858, 0, 1, 0, 0);
    ReadBookiePosition(STREET_FIGHT_RYO);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 20, 0);
    EVT_AT_SetMot(SFTS_VCN, 6856, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_ACCEPT_FIGHTER_POSITION, 0);
    EVT_AT_SetMot(fighter, 6852, 0, 1, 130, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 40, 0);
    EVT_AT_SetCam(2304, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    SetAudienceState(work, 4, 100);
    EVT_AT_SetVoice("0100A022", 1, 60); // @text EN: "I'll do it." | JA: "俺がやろう。"
    if (vfmDispatchControl(FightControlOp::MotionNibble, shen_name{75}, 0, -1) || vfmDispatchControl(FightControlOp::MotionNibble, shen_name{139}, 0, -1)) {
        EVT_AT_SetVoice("0100H017", 1, 100); // @text EN: "Not you again!\nAll right." | JA: "また、兄ちゃんか…\nま、いいや。上がりな。"
        EVT_AT_SetVoice("0100Q015", 1, 230); // @text EN: "You'll love my Iron Thrust blows." | JA: "わしの鉄砂掌を\n味わわせてやろう。"
        EVT_AT_SetMot(fighter, 6851, 0, 1, 130, 205);
        EVT_AT_SetCam(2312, SFTS_BASE, SFTS_BASE, 0, 0, 215);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
        SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
        HLib::Task *const highTask0 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
        highWork0->task = highTask0;
        highWork0->character = STREET_FIGHT_RYO;
        highWork0->selector = 0;
        highWork0->state = 0;
        HLib::SetupTimedTask(highTask0, 215);
        HLib::Task *const highTask1 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
        highWork1->task = highTask1;
        highWork1->character = fighter;
        highWork1->selector = 1;
        highWork1->state = 1;
        HLib::SetupTimedTask(highTask1, 215);
        HLib::Task *const highTask2 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork2 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask2));
        highWork2->task = highTask2;
        highWork2->character = fighter;
        highWork2->selector = 0;
        highWork2->state = 1;
        HLib::SetupTimedTask(highTask2, 215);
        return 350;
    }
    EVT_AT_SetVoice("0100H012", 1, 100); // @text EN: "Good.\nIt's decided." | JA: "よし。決まりだ。"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 20,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 165});
    return 185;
}
std::int32_t SecondAcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    if (vfmDispatchControl(FightControlOp::MotionNibble, shen_name{75}, 0, -1) || vfmDispatchControl(FightControlOp::MotionNibble, shen_name{139}, 0, -1))
        return 0;
    EventFilmControl reveal{.cleanup = 0,
        .frames = 20,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&reveal);
    EVT_AT_SetVoice("0100H013", 1, 15); // @text EN: "You'll be severely injured by\nhis thrust." | JA: "ヤツの鉄砂掌を食らったら\nイチコロだぜ。"
    EVT_AT_SetVoice("0100H014", 1, 125); // @text EN: "You'd better be careful." | JA: "せいぜい気をつけな、\n兄ちゃん。"
    EVT_AT_SetVoice("0100Q001", 1, 195); // @text EN: "Ready for this, boy?" | JA: "覚悟はいいか、小僧。"
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, SFTS_CHEST_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6854, 0, .75f, 0x20000000U, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0, 1, 0, 145);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, SFTS_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_CHEST_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 0, 0);
    EVT_AT_SetMot(fighter, 6851, 0, 1, 130, 170);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    EVT_AT_SetCam(2305, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    return 305;
}
std::int32_t DeclineConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const char *const voices[]{"0100A038", "0100A039", "0100A040"}; // @text 0100A038 EN: "No, I don't think so." | JA: "やめておこう…" ; 0100A039 EN: "Maybe next time." | JA: "またにするか…" ; 0100A040 EN: "I might try next time." | JA: "また今度にするか…"
    EVT_AT_SetVoice(voices[shenGetRandomInt32(3)], 1, 10);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 398, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetCam(211, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    return 60;
}
std::int32_t InsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const count = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    count->cleanup = 1;
    count->delay = 60;
    EVT_AT_SetVoice("0100A037", 1, 60); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    return 210;
}
std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTS_VCN);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, 0, 0);
    EVT_AT_SetMot(SFTS_VCN, 6857, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_VICTORY_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6853, 0, 1.2f, 0x50000000U, 0);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EVT_AT_SetVoice(work.fadeFrames ? "0100Q007" : "0100Q004", 1, 50); // @text 0100Q007 EN: "I lost. I was too confident\nin my Iron Thrust." | JA: "鉄砂掌に頼りすぎた\nわしの負けか…" ; 0100Q004 EN: "He avoided my thrust=@" | JA: "わしの鉄砂掌がかわされるとは…"
    EVT_AT_SetVoice("0100H019", 1, 170); // @text EN: "Amazing!\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = STREET_FIGHT_RYO;
    highWork0->selector = 1;
    highWork0->state = 1;
    HLib::SetupTimedTask(highTask0, 220);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = STREET_FIGHT_RYO;
    highWork1->selector = 0;
    highWork1->state = 1;
    HLib::SetupTimedTask(highTask1, 220);
    EVT_AT_SetCam(2307, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    EVT_AT_SetCam(210, SFTS_BASE, SFTS_BASE, 0, 0, 220);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(SFTS_ENTRY_FEES[work.rank]), 220);
    return 320;
}
std::int32_t VictoryAppearanceConversation(void *) {
    EV_EnableCharacterDisplay(SFTS_CHN);
    EV_EnsureCharacterDraw(SFTS_CHN);
    EV_SetCharacterAlphaEnabled(SFTS_CHN, false);
    EVT_AT_SetPos(SFTS_CHN, STREET_FIGHT_INVALID, SFTS_VICTORY_APPEARANCE_POSITION, 0);
    EVT_AT_SetAngY(SFTS_CHN, STREET_FIGHT_INVALID, 315, 0);
    EVT_AT_SetMot(SFTS_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    EventSetCamera camera{.cleanup = 0, .camera = 9901, .eye = SFTS_CHN, .target = SFTS_CHN, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 60;
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTS_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTS_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTS_VCN, SFTS_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTS_VCN, SFTS_BASE, -10, 0);
    EVT_AT_SetMot(SFTS_VCN, 6857, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTS_BASE, SFTS_DEFEAT_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTS_BASE, 30, 0);
    EVT_AT_SetMot(fighter, 6852, 0, 1.2f, 0x50000000U, 0);
    EVT_AT_SetMot(fighter, 6851, 0, 1.2f, 0, 180);
    mtwkSetRequestedRootMode(scenFindMotionwork(fighter), 1);
    SetMotionFrameTime(scenFindMotionwork(fighter), 20, MotionRegion::LowerBody);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EVT_AT_SetVoice("0100H020", 1, 30); // @text EN: "Too bad.\nBut it was a good match." | JA: "おしかったな、兄ちゃん。\nでも、いいセンいってたぜ。"
    EVT_AT_SetVoice(work.fadeFrames ? "0100Q008" : "0100Q005", 1, 190); // @text 0100Q008 EN: "That's the difference between\nour kung fu, boy!" | JA: "これが功夫の差だ！\nわかったか、小僧！" ; 0100Q005 EN: "You can't overcome my Iron Thrust Style\nwith your skill." | JA: "その程度じゃ鉄砂掌は破れん。\n出直してこい。"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTS_VCN), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFTS_VCN), 1, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = STREET_FIGHT_RYO;
    highWork0->selector = 1;
    highWork0->state = 1;
    HLib::SetupTimedTask(highTask0, 275);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = STREET_FIGHT_RYO;
    highWork1->selector = 0;
    highWork1->state = 1;
    HLib::SetupTimedTask(highTask1, 275);
    EVT_AT_SetCam(2308, SFTS_BASE, SFTS_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, SFTS_BASE, SFTS_BASE, 0, 0, 275);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFTS_ENTRY_FEES[work.rank]), 330);
    return 430;
}

void ResetToMain(StreetFightEventWork &work) {
    EV_ResetFreeTalkFlags();
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    work.state = 0;
    work.auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
}
void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 736, 1);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    EVT_AT_KsIcon_Enqueue(-1, 82, 83, -1, 0x7fff, 0xffff, 0, 0);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}
void RetryChoiceCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82)
        ResetToMain(work);
    else
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
}
void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    hndlSetFlags(FindCharacterByScenarioName(SFTS_VCN));
    hndlSetFlags(FindCharacterByScenarioName(SFTS_KFT));
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFTS_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTS_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTS_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work.hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFTS_RETRY_AREA, 1);
    else if (work.reserved178)
        EV_scenQueueLoad(g_actNumber, SFTS_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    SetNewNpcBatchCountOverride(NpcFillType::FrontFill, EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) ? 12 : 15);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFTS_CHARACTERS);
    work->fighterIndex = 0;
    work->fighters[0] = SFTS_KFT;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->package = SFTS_NORMAL_PACKAGE;
    work->battleFrames = 51;
    work->audienceTask = nullptr;
    work->hudEnabled = work->audienceResultPublished = 0;
    work->reserved178 = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    return work;
}
void LoadNormalResources(StreetFightEventWork &work) {
    work.motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFE"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFE"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFE"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFTS_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = start;
    LoadNormalResources(*work);
    SetNewNpcBatchCountOverride(NpcFillType::BackFill, EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) ? 8 : 15);
    const std::uint32_t level = static_cast<std::uint32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{75}, 0, -1));
    work->fadeFrames = static_cast<std::uint8_t>(level > 2 ? 1 : level);
}
void Init() {
    firstStoryMatch = 1;
    qtePhase = 0;
    qteDelay = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
} // namespace
const ScriptAreaDefinition areaSftsScriptDefinition{.areaId = STREET_FIGHT_SFTS, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
