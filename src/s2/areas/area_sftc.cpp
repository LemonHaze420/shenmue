#include "s2/areas/area_sftc.hpp"
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
#include "s2/math.hpp"
#include "s2/particle_effects.hpp"
#include "s2/model_component.hpp"
#include "s2/battle_qte.hpp"
#include "s2/step_event.hpp"
#include "s2/advice.hpp"

namespace shenmue::s2 {
namespace {
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
void MainCallback(void *);
void ReadBookiePosition(shen_name character) {
    Vector3 position;
    EV_GetCharaPosition(character, 5, &position, EventTransformFlags::World);
}
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
    work->package = SFTC_DEBUG_PACKAGE;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFTC_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFD"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFTC_BASE);
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
    EV_EnableCharacterDisplay(SFTC_FUY);
    EV_EnsureCharacterDraw(SFTC_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFTC_BASE, .parent = STREET_FIGHT_INVALID, .position = SFTC_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFTC_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 180.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTC_BASE);
    AT_SetLocalBaseChara(SFTC_FUY, SFTC_BASE);
    AT_SetLocalBaseChara(fighter, SFTC_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTC_BASE, .position = SFTC_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTC_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFTC_FUY, .parent = SFTC_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFTC_FUY, .parent = SFTC_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFTC_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFTC_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFTC_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFTC_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTC_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTC_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
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
        EV_GetCharaPosition(SFTC_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFTC_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFTC_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFTC_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFTC_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFTC_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFTC_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFTC_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSftcStreetFightConversation(StartStreetFightBookieConversation, &work);
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
        ReadBookiePosition(SFTC_FUY);
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
        QueueSftcStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSftcStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSftcStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFTC_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSftcStreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSftcStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSftcStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSftcStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFTC_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSftcStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSftcStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSftcStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSftcStreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFTC_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFTC_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFTC_FUY, .parent = SFTC_BASE, .position = SFTC_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFTC_FUY, .parent = SFTC_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFTC_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFTC_BASE, .degrees = 0, .delay = 0};
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
    EventSetCamera camera{0, 2001, SFTC_BASE, SFTC_BASE, 0, {}, 0, 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFD"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFTC_DEBUG_OPPONENT_ORIGIN.x, SFTC_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTC_BASE);
    AT_SetLocalBaseChara(fighter, SFTC_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFTC_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, SFTC_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFTC_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFTC_FUY, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_FUY, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_FUY, 0x1aed, 0, 1.1f, 0, 0);
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
    EVT_AT_SetCam(2006, SFTC_BASE, SFTC_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFTC_BASE, SFTC_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, SFTC_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTC_FUY, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_FUY, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFTC_BASE, SFTC_BASE, 0, 0, 5);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTC_FUY, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_FUY, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFTC_BASE, SFTC_BASE, 0, 0, 5);
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

std::int32_t forcedVoicePhase{};
std::uint32_t playerDialogue{}, fighterDialogue{};
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattlePreInitCallback(void *);
void BattleStartCallback(void *);
void BattleReadyGoCallback(void *);
void BattleReleaseCallback(void *);
void BattleWaitCallback(void *);
void BattleRingOutCallback(void *);
void BattleWaitFadeCallback(void *);
void BattleEndCallback(void *);
void SmokeControlCallback(void *);
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t VictoryAppearanceConversation(void *);
std::int32_t DefeatConversation(void *);
std::int32_t TimeoutConversation(void *);
void SetAudienceState(StreetFightEventWork &work, std::uint8_t state, std::int16_t delay) {
    if (!work.audienceTask)
        return;
    StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
    if (audience) {
        audience->state = state;
        audience->loopDelay = delay;
    }
}
void PlaceNormal(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_EnableCharacterDisplay(SFTC_WD2);
    EV_EnsureCharacterDraw(SFTC_WD2);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work.playerPosition = {-3.3f, -.5f, -5.5f};
    work.bookiePosition = {-1.864f, 0, -1.8f};
    EVT_AT_SetPos(SFTC_BASE, STREET_FIGHT_INVALID, SFTC_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFTC_BASE, STREET_FIGHT_INVALID, 180, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTC_BASE);
    AT_SetLocalBaseChara(SFTC_WD2, SFTC_BASE);
    AT_SetLocalBaseChara(fighter, SFTC_BASE);
    AT_SetLocalBaseChara(SFTC_STAG, SFTC_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, SFTC_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 225, 0);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, {1.5f, 0, -1}, 0);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFTC_WD2, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFTC_WD2), FacialNodeReplacement);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetLowQualityHands(FindCharacterByScenarioName(SFTC_WD2));
    SetLowQualityHands(FindCharacterByScenarioName(fighter));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
}
void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        PlaceNormal(work);
        work.state = 1;
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            }
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[audience->fighterCount++] = fighter;
            }
            if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) && work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                if (audience)
                    audience->mode |= 1U;
            }
        }
        Vector3 origin, playerPosition, bookiePosition, firstPosition, secondPosition;
        EV_GetCharaPosition(SFTC_BASE, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::Local);
        origin.y -= .5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = origin;
            audience->state = 2;
        }
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::Local);
        EV_GetCharaPosition(SFTC_WD2, CHARACTER_TRANSLATION_ROOT, &bookiePosition, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFTC_WD2, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &firstPosition, EventTransformFlags::World);
        EV_GetCharaPosition(SFTC_WD2, CHARACTER_TRANSLATION_ROOT, &secondPosition, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = firstPosition.x;
            audience->corridorStart.z = firstPosition.z;
            audience->corridorEnd.x = secondPosition.x;
            audience->corridorEnd.z = secondPosition.z;
            audience->corridorLength = GetPlanarDistance(&firstPosition, &secondPosition);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, playerPosition);
        EV_AT_SetPos(SFTC_WD2, EventTransformFlags::AllAxes, bookiePosition);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->radius = 5.4f;
            audience->radiusRange = 2;
            audience->state = 2;
        }
        LoadBlurModelForCharacter(fighter, nullptr, 0.20000000298023224f);
        work.hudEnabled = 1;
        shenStartFGST(10, 10, 0, 0);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
        vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFD"), 1);
        shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
        shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, 500, 29);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.state = 1;
        break;
    }
    case 1:
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            sftcConversationSkipEnabled = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 718) != 0;
            sftcConversationSkipCallback = nullptr;
            QueueSftcStreetFightConversation(sftcConversationSkipEnabled ? RepeatIntroConversation : FirstIntroConversation, &work);
            work.state = 2;
        }
        break;
    case 2:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 718, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 3;
        break;
    case 3: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFTC_WD2);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
        if (!choice)
            return;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 22) {
            if (GetPlayerStat(PlayerStatsId::Cash) >= static_cast<std::uint64_t>(SFTC_ENTRY_FEES[work.rank])) {
                work.state = 5;
                work.paidEntry = 1;
            } else
                work.state = 7;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = 4;
        } else
            work.state = 6;
        break;
    }
    case 4:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 2;
        break;
    case 5:
        QueueSftcStreetFightConversation(AcceptConversation, &work);
        work.state = 9;
        break;
    case 6:
        QueueSftcStreetFightConversation(DeclineConversation, &work);
        work.state = 18;
        break;
    case 7:
        QueueSftcStreetFightConversation(InsufficientConversation, &work);
        work.state = 18;
        break;
    case 8:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetCurrentTaskTo(BattleEntryCallback);
            work.state = 10;
        }
        break;
    case 9:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 8;
        break;
    case 10:
        switch (work.battleStatus) {
        case 2:
            work.state = 16;
            break;
        case 3:
            work.state = 11;
            break;
        case 7:
            work.state = 17;
            break;
        default:
            break;
        }
        break;
    case 11:
        QueueSftcStreetFightConversation(VictoryConversation, &work);
        work.state = !CanPerformForCurrentAreaFlags() || static_cast<std::uint32_t>(CommitStreetFightStoryRank(work.stage) - 1) > 1 ? 18 : 12;
        break;
    case 12:
        QueueSftcStreetFightConversation(VictoryAppearanceConversation, &work);
        nullsub();
        work.state = 18;
        break;
    case 16:
        ResetStreetFightStoryFlag(work.stage);
        QueueSftcStreetFightConversation(DefeatConversation, &work);
        work.state = 18;
        break;
    case 17:
        QueueSftcStreetFightConversation(TimeoutConversation, &work);
        work.state = 18;
        break;
    case 18:
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        work.state = 19;
        break;
    case 19:
        if (!filmCheckFadeTrackColour()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry &&
                !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{74}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{138}, 0, -1) == 2)
                QueueFightAdviceControl(3);
            work.state = 20;
        }
        break;
    case 20:
        if (!work.battleStatus || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            if (work.battleStatus == 3 && work.fadeFrames < 2)
                ++work.fadeFrames;
        } else if (work.battleStatus == 3) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 719, 1);
            if (work.fadeFrames < 2)
                ++work.fadeFrames;
        }
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        break;
    default:
        break;
    }
}
std::int64_t SelectFighterBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    std::int32_t voice = -1;
    if (GetBattleDialogueCompletionCount(playerDialogue) >= 20) {
        voice = SFTC_FIRST_BATTLE_VOICES[index];
        ++index;
        if (index)
            index = 0;
        SetBattleDialogueFlags(fighterDialogue, 0);
    }
    return static_cast<std::uint32_t>(voice);
}
std::int64_t SelectMotionBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFTC_SECOND_BATTLE_VOICES[index];
    if (++index >= 2)
        index = 0;
    return voice;
}
std::int64_t SelectForcedBattleVoice(void *) {
    if (!(EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFTC_SF5, 8) & 4))
        return 0xffff;
    if (!forcedVoicePhase) {
        forcedVoicePhase = 1;
        return 148;
    }
    if (forcedVoicePhase != 1)
        return 0xffff;
    forcedVoicePhase = 2;
    return 149;
}
void AllocateBattleVoices(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    forcedVoicePhase = 0;
    if (work.fadeFrames == 2) {
        const std::uint32_t forced = AllocateBattleDialogueCondition17(7, -1, fighter, shen_name{}, 1, 100, 1);
        AssignBattleDialogueCallback(forced, SelectForcedBattleVoice);
    }
    playerDialogue = AllocateBattleDialogueCondition12(9, -2, STREET_FIGHT_RYO, fighter, 1, 100, 1);
    fighterDialogue = AllocateBattleDialogueCondition17(9, -1, fighter, shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(fighterDialogue, SelectFighterBattleVoice);
    const std::uint32_t motion = AllocateBattleDialogueCondition17(10, -1, fighter, shen_name{}, 2, 85, 1);
    AssignBattleDialogueCallback(motion, SelectMotionBattleVoice);
}
void BattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, SFTC_BATTLE_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 0);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, SFTC_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 180, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_BATTLE_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 0, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(fighter), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetMot(fighter, 3130, 0, 1, 0, 0);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
}
void BattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    EV_spmBeginSampleExecute();
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
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6856, 0, 1, 0, 50);
    EVT_AT_SetVoice("0100F013", 1, 0); // @text EN: "Okay, take your stance!" | JA: "それじゃあ、かまえて。"
    EVT_AT_SetCam(2001, SFTC_BASE, SFTC_BASE, 0, 0, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    HLib::SetCurrentTaskTo(BattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}
void BattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    Vector3 position;
    Vector3i rotation;
    const shen_name fighter = work.fighters[work.fighterIndex];
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::SetCurrentTaskTo(BattleStartCallback);
}
void BattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x8000000U, work.fadeFrames ? work.fadeFrames == 1 ? 1 : 2 : 0);
    const std::intptr_t rank = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{74}, 0, -1);
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(rank + 1), 1);
    vfCopyPosFromEntity(0, nullptr, SFTC_DEBUG_OPPONENT_ORIGIN.x, SFTC_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(2, 99, 1, 8, 60);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1788, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    work.hudEnabled = 0;
    AllocateBattleVoices(work);
    HLib::SetCurrentTaskTo(BattleReadyGoCallback);
}
void BattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100F014");
    HLib::SetCurrentTaskTo(BattleReleaseCallback);
    HLib::SetupTaskDelay(40);
}
void BattleReleaseCallback(void *) {
    if (!cvsHasValidConverseData(0)) {
        EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
        HLib::SetCurrentTaskTo(BattleWaitCallback);
    }
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (IsFightActive()) {
        if (work.audienceResultPublished)
            return;
        const std::int32_t playerFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 11));
        const std::int32_t fighterFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFTC_SF5, 11));
        if (fighterFlags != -1 && (fighterFlags & 0x80) && playerFlags != -1 && !(playerFlags & 0x80)) {
            if (GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 1) {
                check_dead_status(1);
                ++work.audienceResultPublished;
                work.battleStatus = 3;
                HLib::SetCurrentTaskTo(BattleRingOutCallback);
                return;
            }
        }
        const std::uint32_t status = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
        if (status == 2) {
            EVT_AT_SetVoice("0100F016", 1, 0); // @text EN: "A winner!" | JA: "勝負あり！"
            SetAudienceState(work, 7, 10);
            work.battleStatus = 2;
        } else if (status == 3) {
            EVT_AT_SetVoice("0100F016", 1, 0); // @text EN: "A winner!" | JA: "勝負あり！"
            SetAudienceState(work, 6, 11);
            work.battleStatus = 3;
        } else {
            if (status <= 4)
                return;
            if (status <= 7) {
                EV_PlayDialogue("0100F017");
                work.battleStatus = 7;
            } else if (status != 10)
                return;
        }
        ++work.audienceResultPublished;
    } else
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
}
std::uint32_t CreateRingOutSmoke() {
    const Vector3 zero{};
    const Vector3 velocity{0, .009999999776482582f, 0};
    const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 100, .0010000000474974513f, shen_name{}, 3));
    SetSmokeEffectFalloff(effect, 10);
    SetSmokeEffectFadeFactor(effect, 255);
    SetParticleEffectColour(0, effect, 0, 142, 125, 110);
    SetParticleEffectInitialScale(0, effect, .43529412150382996f);
    SetParticleEffectSize(0, effect, 3.1372549533843994f, 3.1764705181121826f);
    SetParticleEffectBirthRate(0, effect, 0);
    SetParticleEffectVelocity(0, effect, velocity, zero);
    SetSmokeEffectSpread(effect, 55.86399841308594f, 1.058823585510254f);
    SetSmokeEffectRadialMotion(effect, .772549033164978f, .46666666865348816f);
    SetParticleEffectBirthRate(0, effect, .10000000149011612f);
    return effect;
}
void SmokeControlCallback(void *parameter) {
    SftcSmokeWork &work = *static_cast<SftcSmokeWork *>(parameter);
    StreetFightEventWork &event = *work.event;
    const shen_name fighter = event.fighters[event.fighterIndex];
    const Vector3 zero{};
    switch (work.frame) {
    case 0:
        if (event.reservedbc == 0xffffffffU)
            event.reservedbc = CreateRingOutSmoke();
        break;
    case 80: {
        GetNpcBatchCountOverride(5);
        Vector3 position;
        EV_GetCharaPosition(fighter, 5, &position, EventTransformFlags::World);
        position.y = 100.2249984741211f;
        SetParticleEffectPosition(0, event.reservedbc, &position, &zero);
        SetParticleEffectBirthRate(0, event.reservedbc, .050196077674627304f);
        ResumeParticleEffect(0, event.reservedbc);
        break;
    }
    case 100:
        SetParticleEffectBirthRate(0, event.reservedbc, 0);
        break;
    case 210:
        GetNpcBatchCountOverride(40);
        if (event.reservedbc != 0xffffffffU) {
            SuspendParticleEffect(0, event.reservedbc);
            FreeParticleEffect(0, event.reservedbc);
            event.reservedbc = 0xffffffffU;
        }
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
    ++work.frame;
}
void BattleRingOutCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    if (!IsFightActive()) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(SmokeControlCallback, 6, 0x0b, sizeof(SftcSmokeWork), STREET_FIGHT_EVENT_TASK);
        SftcSmokeWork *const smoke = static_cast<SftcSmokeWork *>(HLib::GetTaskParameter(task));
        smoke->event = &work;
        smoke->frame = 0;
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, SFTC_RING_OUT_PLAYER_POSITION, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 340, 0);
        EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_RING_OUT_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(fighter, SFTC_BASE, 0, 0);
        EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, 1, 0, 0);
        EVT_AT_SetMot(fighter, 6863, 0, .5f, 0x40000000U, 0);
        EVT_AT_SetCam(2002, SFTC_BASE, SFTC_BASE, 0, 0, 0);
        EVT_AT_SetVoice("0100P011", 1, 10);
        EVT_AT_SetVoice("0100P012", 1, 60);
        EVT_AT_SetPoint(STREET_FIGHT_RYO, fighter, 5, 3, 0, 0, 210, 0);
        QueueStreetFightMotionSpeed(fighter, .2f, 85);
        SetAudienceState(work, 6, 81);
        EVT_AT_SetVoice("0100F016", 1, 120); // @text EN: "A winner!" | JA: "勝負あり！"
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
        HLib::SetupTaskDelay(180);
    }
    work.frameCounter = 0;
}
void BattleWaitFadeCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        HLib::SetCurrentTaskTo(BattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
    work.frameCounter = 0;
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFTC_BASE);
    AT_SetLocalBaseChara(fighter, SFTC_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFTC_WD2, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), FacialExpressionId::Angry, 1, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    HLib::SetCurrentTaskTo(MainCallback);
    work.frameCounter = 0;
}

const char *FeeVoice(std::int32_t fee) {
    switch (fee) {
    case 10:
        return "0100F008"; // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
    case 20:
        return "0100F009"; // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
    case 30:
        return "0100F010"; // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
    case 40:
        return "0100F011"; // @text EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！"
    default:
        return "0100F005"; // @text EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
    }
}
std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 225, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 225, 200);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0, 1, 0, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 199);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_WD2, 6854, 0, 1.1f, 0, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTC_WD2)->reserved08 = 16;
    EVT_AT_SetPos(fighter, SFTC_BASE, {0, -.050000001f, -1.3f}, 0);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(fighter, 6849, 0, .80000001f, 0x40000000U, 383);
    QueueStreetFightMotionSpeed(fighter, .2f, 490);
    EVT_AT_SetMot(fighter, 6850, 0, 1, 0x40000000U, 490);
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
    EVT_AT_SetVoice("0100F001", 1, 50); // @text EN: "Come and see this!" | JA: "さあさあ、お立ち会い。"
    EVT_AT_SetVoice("0100F002", 1, 142); // @text EN: "Dongshan Su here, has acquired one of\nthe skills of Shaolin Shi." | JA: "この蘇東山こそは少林寺七十二芸の\nひとつ、鉄頭功を極めた達人だ。"
    EVT_AT_SetVoice("0100F003", 1, 383); // @text EN: "He has defeated many with a single\nblow using his head." | JA: "あらゆる相手を頭突きの\n一撃で倒してきた、"
    EVT_AT_SetVoice("0100F004", 1, 510); // @text EN: "Is there anyone strong enough\nto challenge this man?" | JA: "この男と勝負する\n腕自慢はいないか？"
    EVT_AT_SetVoice(FeeVoice(SFTC_ENTRY_FEES[work.rank]), 1, 637);
    EVT_AT_SetVoice("0100F006", 1, 747); // @text EN: "The time limit is sixty seconds." | JA: "制限時間は６０秒。"
    EVT_AT_SetVoice("0100F007", 1, 839); // @text EN: "Raise your hand if you want to\ngive it a shot!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
    EVT_AT_SetCam(100, SFTC_BASE, SFTC_BASE, 0, 0, 0);
    EVT_AT_SetCam(2004, SFTC_BASE, SFTC_BASE, 0, 0, 120);
    EVT_AT_SetCam(2005, SFTC_BASE, SFTC_BASE, 0, 0, 383);
    EVT_AT_SetCam(2006, SFTC_BASE, SFTC_BASE, 0, 0, 490);
    EVT_AT_SetCam(120, SFTC_BASE, SFTC_BASE, 0, 0, 700);
    EVT_AT_SetCam(128, SFTC_BASE, SFTC_BASE, 0, 0, 839);
    return 990;
}
std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTC_WD2)->reserved08 = 16;
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_WD2, 6854, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_FIGHTER_REPEAT_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6850, 0, 1, 0x40000000U, 0);
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
    EVT_AT_SetVoice(FeeVoice(SFTC_ENTRY_FEES[work.rank]), 1, 0);
    EVT_AT_SetVoice("0100F007", 1, 120); // @text EN: "Raise your hand if you want to\ngive it a shot!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
    EVT_AT_SetCam(128, SFTC_BASE, SFTC_BASE, 0, 0, 1);
    return 240;
}
std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6855, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTC_WD2)->reserved08 = 6;
    QueueStreetFightLook(SFTC_WD2, STREET_FIGHT_RYO)->reserved08 = 6;
    QueueStreetFightLook(fighter, STREET_FIGHT_RYO)->reserved08 = 6;
    SetAudienceState(work, 4, 60);
    const std::int32_t frames = work.fadeFrames ? 280 : 190;
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    EVT_AT_SetVoice(work.fadeFrames ? "0100F015" : "0100F012", 1, 100); // @text 0100F015 EN: "Not you again!\nAll right." | JA: "また兄ちゃんか…\nしかたがねえ。決まりだ。" ; 0100F012 EN: "Good.\nIt's decided." | JA: "よし、決まりだ。"
    EVT_AT_SetPoint(SFTC_WD2, STREET_FIGHT_RYO, 5, 3, 0, 50, frames, 0);
    EVT_AT_SetPoint(fighter, STREET_FIGHT_RYO, 5, 1, 0, 70, frames, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(greetingHandPoseCommand), 30);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const display = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(task));
    display->task = task;
    display->character = STREET_FIGHT_RYO;
    display->selector = 0;
    display->state = 0;
    HLib::SetupTimedTask(task, frames);
    EVT_AT_SetCam(130, SFTC_BASE, SFTC_BASE, 0, 0, 1);
    EVT_AT_SetCam(141, SFTC_BASE, SFTC_BASE, 0, 0, 90);
    return frames;
}
std::int32_t DeclineConversation(void *) {
    switch (shenGetRandomInt32(3)) {
    case 0:
        EV_PlayDialogue("0100A038");
        break;
    case 1:
        EV_PlayDialogue("0100A039");
        break;
    case 2:
        EV_PlayDialogue("0100A040");
        break;
    default:
        break;
    }
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTC_WD2)->reserved08 = 6;
    EVT_AT_SetCam(3000, SFTC_BASE, SFTC_BASE, 0, 0, 1);
    return 70;
}
std::int32_t InsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const work = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->delay = 60;
    EV_PlayDialogue("0100A037");
    QueueStreetFightLook(STREET_FIGHT_RYO, SFTC_WD2)->reserved08 = 6;
    EVT_AT_SetCam(3001, SFTC_BASE, SFTC_BASE, 0, 0, 1);
    return 150;
}
std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 0);
    EVT_AT_SetMot(SFTC_WD2, 6854, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, SFTC_FIGHTER_VICTORY_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6851, 0, 1, 0x50000000U, 0);
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
    EVT_AT_SetVoice("0100F018", 1, 35); // @text EN: "You're good.\nHere you go." | JA: "兄ちゃんやるなあ、\nほらよ。"
    EVT_AT_SetVoice("0100P009", 1, 150); // @text EN: "Damn! I was off guard!" | JA: "くそっ、油断した！"
    PATI_StartMoneyCount(SFTC_ENTRY_FEES[work.rank], 180);
    EVT_AT_SetCam(2007, SFTC_BASE, SFTC_BASE, 0, 0, 5);
    EVT_AT_SetCam(210, SFTC_BASE, SFTC_BASE, 0, 0, 195);
    return 280;
}
std::int32_t PlayVictoryAppearance() {
    EV_EnableCharacterDisplay(SFTC_CHN);
    EV_EnsureCharacterDraw(SFTC_CHN);
    EV_SetCharacterAlphaEnabled(SFTC_CHN, false);
    EVT_AT_SetPos(SFTC_CHN, STREET_FIGHT_INVALID, SFTC_VICTORY_APPEARANCE_POSITION, 0);
    EVT_AT_SetAngY(SFTC_CHN, STREET_FIGHT_INVALID, 230, 0);
    EVT_AT_SetMot(SFTC_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    EVT_AT_SetCam(9901, SFTC_CHN, SFTC_CHN, 0, 0, 0);
    return 60;
}
std::int32_t VictoryAppearanceConversation(void *) {
    return PlayVictoryAppearance();
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 1);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 1);
    EVT_AT_SetMot(SFTC_WD2, 6854, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, {-.98799998f, 0, -1.2f}, 1);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 1);
    EVT_AT_SetMot(fighter, 6853, 0, 1, 0x40000000U, 0);
    SetMotionLayerEndFrame(scenFindMotionwork(fighter), 1, MotionRegion::LowerBody);
    EVT_AT_SetMot(fighter, 6853, 1, 1, 0x40000000U, 60);
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
    EVT_AT_SetVoice("0100F019", 1, 35); // @text EN: "That's too bad.\nTry harder next time." | JA: "いやあ、惜しかったなあ、\n次はがんばんな。"
    EVT_AT_SetVoice("0100P010", 1, 180); // @text EN: "Train yourself more!" | JA: "もっときたえて出直してこい！"
    EVT_AT_SetCam(2008, SFTC_BASE, SFTC_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, SFTC_BASE, SFTC_BASE, 0, 0, 300);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFTC_ENTRY_FEES[work.rank]), 180);
    return 290;
}
std::int32_t TimeoutConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFTC_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFTC_BASE, 180, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFTC_WD2, SFTC_BASE, work.bookiePosition, 1);
    EVT_AT_SetAngY(SFTC_WD2, SFTC_BASE, 0, 1);
    EVT_AT_SetMot(SFTC_WD2, 6854, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFTC_BASE, {-.98799998f, 0, -1.2f}, 1);
    EVT_AT_SetAngY(fighter, SFTC_BASE, 17, 1);
    EVT_AT_SetMot(fighter, 6853, 0, 1, 0x40000000U, 0);
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
    EVT_AT_SetVoice("0100F020", 1, 35); // @text EN: "It's a draw, so I can't\nask for your money." | JA: "引き分けじゃ金は取れねえな。"
    EVT_AT_SetVoice("0100F021", 1, 140); // @text EN: "Come and try again." | JA: "また今度挑戦してくれ。"
    EVT_AT_SetVoice("0100P003", 1, 230);
    EVT_AT_SetCam(107, SFTC_BASE, SFTC_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, SFTC_BASE, SFTC_BASE, 0, 0, 300);
    return 320;
}

void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 718, 1);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI_Internal(choices, 0x7fff, -1, 2, 0);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2U;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
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
    work.state = work.battleStatus = 0;
    work.audienceResultPublished = 0;
    work.auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
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
    hndlSetFlags(FindCharacterByScenarioName(SFTC_WD2));
    hndlSetFlags(FindCharacterByScenarioName(work.fighters[0]));
    AT_SetLocalBaseChara(SFTC_STAG, STREET_FIGHT_INVALID);
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFTC_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTC_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFTC_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work.hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFTC_RETRY_AREA, 1);
    else if (work.reserved178)
        EV_scenQueueLoad(g_actNumber, SFTC_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED))
        SetNewNpcBatchCountOverride(NpcFillType::FrontFill, 12);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = SFTC_SF5;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->package = SFTC_RESOURCE_PACKAGE;
    work->battleFrames = 50;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFTC_CHARACTERS);
    work->audienceTask = nullptr;
    work->hudEnabled = work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->reserved178 = 0;
    return work;
}
void LoadNormalResources(StreetFightEventWork &work) {
    work.motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFD"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFD"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFD"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFTC_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED))
        SetNewNpcBatchCountOverride(NpcFillType::BackFill, 12);
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
    work->fadeFrames = static_cast<std::uint8_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{74}, 0, -1));
    work->reservedbc = 0xffffffffU;
}
std::uint64_t resetOnlyDialogueWords[3]{};
void Init() {
    for (std::uint64_t &word : resetOnlyDialogueWords)
        word = 0;
    playerDialogue = fighterDialogue = 0;
    forcedVoicePhase = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

} // namespace
const ScriptAreaDefinition areaSftcScriptDefinition{.areaId = STREET_FIGHT_SFTC, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
