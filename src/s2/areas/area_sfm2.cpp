#include "s2/areas/area_sfm2.hpp"
#include "s2/author.hpp"
#include "s2/event_call.hpp"
#include "s2/math.hpp"
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

namespace shenmue::s2 {
namespace {
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
// @verified - p
void ConversationSkipNullsub() {
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
    work->package = SFM2_DEBUG_PACKAGE;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFM2_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFA"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFM2_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
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
    EV_EnableCharacterDisplay(SFM2_FUY);
    EV_EnsureCharacterDraw(SFM2_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFM2_BASE, .parent = STREET_FIGHT_INVALID, .position = SFM2_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFM2_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 315.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFM2_BASE);
    AT_SetLocalBaseChara(SFM2_FUY, SFM2_BASE);
    AT_SetLocalBaseChara(fighter, SFM2_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFM2_BASE, .position = SFM2_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFM2_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFM2_FUY, .parent = SFM2_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFM2_FUY, .parent = SFM2_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFM2_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFM2_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFM2_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFM2_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFM2_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFM2_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
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
        EV_GetCharaPosition(SFM2_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFM2_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFM2_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFM2_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFM2_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFM2_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFM2_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFM2_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfm2StreetFightConversation(StartStreetFightBookieConversation, &work);
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
        ReadBookiePosition(SFM2_FUY);
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
        QueueSfm2StreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfm2StreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfm2StreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFM2_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfm2StreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSfm2StreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfm2StreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfm2StreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFM2_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfm2StreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfm2StreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfm2StreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfm2StreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFM2_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFM2_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFM2_FUY, .parent = SFM2_BASE, .position = SFM2_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFM2_FUY, .parent = SFM2_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFM2_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFM2_BASE, .degrees = 0, .delay = 0};
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
    EventSetCamera camera{0, 2001, SFM2_BASE, SFM2_BASE, 0, {}, 0, 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QSFA"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFM2_DEBUG_OPPONENT_ORIGIN.x, SFM2_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFM2_BASE);
    AT_SetLocalBaseChara(fighter, SFM2_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFM2_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, SFM2_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFM2_BASE, SFM2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFM2_BASE, SFM2_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFM2_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFM2_FUY, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_FUY, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_FUY, 0x1aed, 0, 1.1f, 0, 0);
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
    EVT_AT_SetCam(2006, SFM2_BASE, SFM2_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFM2_BASE, SFM2_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, SFM2_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFM2_FUY, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_FUY, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFM2_BASE, SFM2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFM2_BASE, SFM2_BASE, 0, 0, 5);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFM2_FUY, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_FUY, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFM2_BASE, SFM2_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFM2_BASE, SFM2_BASE, 0, 0, 5);
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

std::uint32_t battleDialogueIds[3]{};
void MainCallback(void *);
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
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t NextAcceptConversation(void *);
std::int32_t StopConversation(void *);
std::int32_t FirstBattleConversation(void *);
std::int32_t NextBattleConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t VictoryAppearanceConversation(void *);
std::int32_t DefeatConversation(void *);
void SetAudienceState(StreetFightEventWork &work, std::uint8_t state, std::int16_t delay) {
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience) {
            audience->state = state;
            audience->loopDelay = delay;
        }
    }
}
void SetFighterIndex(StreetFightEventWork &work, std::int32_t rank) {
    switch (rank) {
    case 0:
        work.fighterIndex = 0;
        break;
    case 1:
    case 3:
        work.fighterIndex = 1;
        break;
    default:
        work.fighterIndex = 2;
        break;
    }
}
void PlaceNormal(StreetFightEventWork &work) {
    EV_EnableCharacterDisplay(SFM2_SHS);
    EV_EnsureCharacterDraw(SFM2_SHS);
    for (std::int32_t index = 0; index < 3; ++index) {
        EV_EnableCharacterDisplay(work.fighters[index]);
        EV_EnsureCharacterDraw(work.fighters[index]);
    }
    work.playerPosition = {-3.3f, -.5f, -5.5f};
    work.bookiePosition = {-1.864f, 0, -1.8f};
    EVT_AT_SetPos(SFM2_BASE, STREET_FIGHT_INVALID, SFM2_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFM2_BASE, STREET_FIGHT_INVALID, 315, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFM2_BASE);
    AT_SetLocalBaseChara(SFM2_SHS, SFM2_BASE);
    for (std::int32_t index = 0; index < 3; ++index)
        AT_SetLocalBaseChara(work.fighters[index], SFM2_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, SFM2_DEBUG_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 225, 0);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 0, 0);
    EVT_AT_SetPos(work.fighters[0], SFM2_BASE, {-.5f, 0, -1}, 0);
    EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 17, 0);
    EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {.5f, 0, -1}, 0);
    EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 17, 0);
    EVT_AT_SetPos(work.fighters[2], SFM2_BASE, {1.5f, 0, -1}, 0);
    EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 17, 0);
    const std::uint32_t type = work.rank >= 4 ? 0 : 4;
    SetScenarioCharacterType(STREET_FIGHT_RYO, type);
    SetScenarioCharacterType(SFM2_SHS, type);
    for (std::int32_t index = 0; index < 3; ++index)
        SetScenarioCharacterType(work.fighters[index], type);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFM2_SHS), FacialNodeOriginal);
    for (std::int32_t index = 0; index < 3; ++index)
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(work.fighters[index]), FacialNodeOriginal);
    adfsSetCameraIdxForChara(SFM2_SHS, SFM2_BOOKIE_VOICE);
    adfsSetCameraIdxForChara(work.fighters[0], SFM2_KZN_VOICE);
    adfsSetCameraIdxForChara(work.fighters[1], SFM2_SSY_VOICE);
    adfsSetCameraIdxForChara(work.fighters[2], SFM2_SF2_VOICE);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1);
}
void MainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    switch (work.state) {
    case 0: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        PlaceNormal(work);
        work.state = 1;
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                if (audience)
                    audience->mode |= 1;
            }
        }
        Vector3 centre, savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(SFM2_BASE, CHARACTER_TRANSLATION_ROOT, &centre, EventTransformFlags::Relative);
        centre.y -= .5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = centre;
            audience->state = 2;
        }
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Relative);
        EV_GetCharaPosition(SFM2_SHS, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Relative);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFM2_SHS, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFM2_SHS, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            audience->corridorLength = GetPlanarDistance(&player, &bookie);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFM2_SHS, EventTransformFlags::AllAxes, savedBookie);
        for (shen_name character : {SFM2_SHS, SFM2_SF2, SFM2_KZN, SFM2_SSY})
            SetLowQualityHands(FindCharacterByScenarioName(character));
        for (shen_name character : {SFM2_SHS, SFM2_SF2, SFM2_KZN, SFM2_SSY}) {
            SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 1, 0);
        }
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.state = 1;
        break;
    }
    case 1: {
        if (work.audienceTask && static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state <= 2)
            break;
        sfm2ConversationSkipEnabled = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 730) != 0;
        sfm2ConversationSkipCallback = sfm2ConversationSkipEnabled ? ConversationSkipNullsub : nullptr;
        const std::int32_t flag = work.rank == 0 ? 730 : work.rank == 1 ? 732 : work.rank == 2 ? 734 : 0;
        QueueSfm2StreetFightConversation(
            flag && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, flag) ? FirstIntroConversation : RepeatIntroConversation, &work);
        work.state = 2;
        break;
    }
    case 2:
        if (work.rank >= 0 && work.rank <= 2)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 730 + 2 * work.rank, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 3;
        break;
    case 3:
    case 19: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        ReadBookiePosition(SFM2_SHS);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
        if (!choice)
            break;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        for (std::int32_t index = 0; index < 3; ++index)
            UpdateMotionWorkWithPosition(scenFindMotionwork(work.fighters[index]), nullptr, 0);
        const bool next = work.state == 19;
        if (choice == 22) {
            if (next)
                ++work.rank;
            if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= SFM2_ENTRY_FEES[work.rank]) {
                work.state = next ? 21 : 4;
                work.paidEntry = 1;
            } else
                work.state = 8;
        } else if (choice == 470) {
            vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, 1);
            work.state = next ? 20 : 9;
        } else
            work.state = next ? 22 : 7;
        if (next)
            SetFighterIndex(work, work.rank);
        break;
    }
    case 4:
        QueueSfm2StreetFightConversation(AcceptConversation, &work);
        work.state = 5;
        break;
    case 5:
    case 6: {
        const bool first = work.state == 5;
        work.hudEnabled = 1;
        shenStartFGST(10, 10, 0, 0);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
        vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QSFA"), 1);
        shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            for (std::int32_t index = 0; index < 4; ++index)
                audience->fighters[index] = shen_name{};
            audience->fighterCount = 0;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = work.fighters[work.fighterIndex];
        }
        if (first && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_spmBeginSampleExecute();
        }
        QueueSfm2StreetFightConversation(first ? FirstBattleConversation : NextBattleConversation, &work);
        work.state = 11;
        break;
    }
    case 7:
        QueueSfm2StreetFightConversation(DeclineConversation, &work);
        work.state = 24;
        break;
    case 8:
        QueueSfm2StreetFightConversation(InsufficientConversation, &work);
        work.state = 24;
        break;
    case 9:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 2;
        break;
    case 10:
        if (!filmCheckFadeTrackColour()) {
            HLib::SetCurrentTaskTo(BattleEntryCallback);
            work.state = 12;
        }
        break;
    case 11:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 10;
        break;
    case 12:
        work.rankHistory[work.rank] = 1;
        work.state = work.battleStatus == 3 || work.battleStatus == 6 ? 13 : 23;
        break;
    case 13:
        QueueSfm2StreetFightConversation(VictoryConversation, &work);
        if (work.rank < 2)
            work.state = 18;
        else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            spmFadeOut();
            SetMotionEnvironmentEnabled(0);
            work.state = 26;
        } else
            work.state = CanPerformForCurrentAreaFlags() && static_cast<std::uint32_t>(CommitStreetFightStoryRank(work.stage) - 1) <= 1 ? 14 : 24;
        break;
    case 14:
        QueueSfm2StreetFightConversation(VictoryAppearanceConversation, &work);
        nullsub();
        work.state = 24;
        break;
    case 18:
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.paidEntry = 0;
        work.state = 19;
        break;
    case 20:
        if (!vfmDispatchControl(FightControlOp::MovesScroll, {}, 0, -1))
            work.state = 18;
        break;
    case 21:
        QueueSfm2StreetFightConversation(NextAcceptConversation, &work);
        work.state = 6;
        break;
    case 22:
        QueueSfm2StreetFightConversation(StopConversation, &work);
        work.state = 24;
        break;
    case 23:
        ResetStreetFightStoryFlag(work.stage);
        QueueSfm2StreetFightConversation(DefeatConversation, &work);
        work.state = 24;
        break;
    case 24:
        if (EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work.state = 25;
        break;
    case 25:
        if (!filmCheckFadeTrackColour()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry &&
                !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{69}, 0, -1) &&
                vfmDispatchControl(FightControlOp::MotionNibble, shen_name{133}, 0, -1) == 2)
                QueueFightAdviceControl(4);
            work.state = 26;
        }
        break;
    case 26:
        if (work.battleStatus == 3 && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            const std::int32_t rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{69}, 0, -1));
            if (rank >= 0 && rank <= 2) {
                if (rank == 2)
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 735, 1);
                if (rank >= 1)
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 733, 1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 731, 1);
            }
        }
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work.paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        break;
    default:
        break;
    }
}

std::int64_t SelectKznBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFM2_KZN_BATTLE_VOICES[index];
    if (++index >= 2)
        index = 0;
    return voice;
}
std::int64_t SelectSsyBattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFM2_SSY_BATTLE_VOICES[index];
    if (++index >= 2)
        index = 0;
    return voice;
}
std::int64_t SelectSf2BattleVoice(void *parameter) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(parameter);
    const std::uint16_t voice = SFM2_SF2_BATTLE_VOICES[index];
    if (++index >= 2)
        index = 0;
    return voice;
}
void BattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
    for (std::int32_t index = 0; index < 3; ++index)
        UpdateMotionWorkWithPosition(scenFindMotionwork(work.fighters[index]), nullptr, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 180, 0);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, SFM2_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 180, 0);
    if (work.rank >= 0 && work.rank <= 2) {
        EVT_AT_SetPos(work.fighters[work.fighterIndex], SFM2_BASE, {0, 0, 1.1f}, 0);
        EVT_AT_SetAngY(work.fighters[work.fighterIndex], SFM2_BASE, 0, 0);
    } else if (work.rank == 3) {
        EVT_AT_SetPos(work.fighters[0], SFM2_BASE, {0, 0, 1.1f}, 0);
        EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 0, 0);
        EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {1, 0, .8f}, 0);
        EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 0, 0);
    } else if (work.rank == 4) {
        EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {.5f, 0, 1}, 0);
        EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 0, 0);
        EVT_AT_SetPos(work.fighters[2], SFM2_BASE, SFM2_PAIR_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 0, 0);
    } else {
        EVT_AT_SetPos(work.fighters[0], SFM2_BASE, {1, 0, .8f}, 0);
        EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 0, 0);
        EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {-1, 0, .8f}, 0);
        EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 0, 0);
        EVT_AT_SetPos(work.fighters[2], SFM2_BASE, SFM2_TRIO_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 0, 0);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
    HLib::SetupTaskDelay(30);
}
void BattlePreEventCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    std::int32_t visible[3]{1, 1, 1};
    EV_LoadAudio("0100E000", 1);
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
    for (shen_name character : {STREET_FIGHT_RYO, SFM2_SHS, work.fighters[0], work.fighters[1], work.fighters[2]}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 1, 0);
    }
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFM2_SHS), FacialNodeOriginal);
    for (std::int32_t index = 0; index < 3; ++index)
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(work.fighters[index]), FacialNodeOriginal);
    switch (work.rank) {
    case 0:
        visible[1] = 0;
        break;
    case 1:
        visible[2] = visible[0] = 0;
        break;
    case 2:
        visible[0] = 0;
        break;
    case 3:
        visible[2] = 0;
        break;
    case 4:
        visible[0] = 0;
        break;
    default:
        break;
    }
    for (std::int32_t index = 0; index < 3; ++index) {
        if (visible[index]) {
            EV_EnableCharacterDisplay(work.fighters[index]);
            EVT_AT_SetMot(work.fighters[index], 3184, 0, 1, 0, 0);
        } else
            EV_DisableCharacterDisplay(work.fighters[index]);
    }
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6861, 0, .40000001f, 0, 0);
    EVT_AT_SetVoice("0100D020", 1, 0); // @text EN: "All right, take your stance." | JA: "それじゃあ、かまえて。"
    EVT_AT_SetCam(2001, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    EV_DisableCharacterDisplay(SFM2_SHS);
    EV_ToggleCharacterFlag(SFM2_SHS);
    HLib::SetCurrentTaskTo(BattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}
void BattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
    for (std::int32_t index = 0; index < 3; ++index)
        UnparentAndSetExistingPosRot(work.fighters[index]);
    HLib::SetCurrentTaskTo(BattleStartCallback);
    HLib::SetupTaskDelay(10);
}
void BattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    std::int32_t enabled[3]{1, 1, 1};
    std::int32_t guardModes[3]{0, 1, 2};
    const std::uint16_t hitPoints[3]{300, 350, 400};
    const std::uint8_t styles[3]{27, 49, 50};
    switch (work.rank) {
    case 0:
    case 1:
    case 2:
        enabled[0] = enabled[1] = enabled[2] = 0;
        enabled[work.fighterIndex] = 1;
        break;
    case 3:
        enabled[2] = 0;
        break;
    case 4:
        enabled[0] = 0;
        break;
    case 5:
        break;
    case 6:
        guardModes[0] = 1;
        break;
    case 7:
        guardModes[0] = 1;
        guardModes[1] = 2;
        break;
    default:
        guardModes[0] = guardModes[1] = 2;
        break;
    }
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < 3; ++index) {
        if (enabled[index]) {
            shenInitFightData(FindCharacterByScenarioName(work.fighters[index]), nullptr, 1, 1, hitPoints[index], styles[index]);
            vfUpdateFlagsFor(FindCharacterByScenarioName(work.fighters[index]), 0, 0x8000000, guardModes[index]);
            ++count;
        }
    }
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(work.rank + 1), 1);
    vfCopyPosFromEntity(0, nullptr, SFM2_STAGE_ORIGIN.x, SFM2_STAGE_ORIGIN.z, 3.5f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x400}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(count + 1, 99, 1, 0, 30);
    const std::int32_t storyRank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{69}, 0, -1));
    if (storyRank >= 0 && storyRank <= 2) {
        if (storyRank == 2)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1994, 1);
        if (storyRank >= 1)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1993, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1785, 1);
    }
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
    work.hudEnabled = 0;
    const std::uint32_t first = AllocateBattleDialogueCondition12(20, -1, work.fighters[0], shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(first, SelectKznBattleVoice);
    const std::uint32_t second = AllocateBattleDialogueCondition12(20, -1, work.fighters[1], shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(second, SelectSsyBattleVoice);
    const std::uint32_t third = AllocateBattleDialogueCondition12(20, -1, work.fighters[2], shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(third, SelectSf2BattleVoice);
    AllocateMotionBattleDialogue(11, 11, work.fighters[0], shen_name{}, 3146, 1, 100, 1);
    AllocateMotionBattleDialogue(11, 12, work.fighters[1], shen_name{}, 3145, 1, 100, 1);
    AllocateMotionBattleDialogue(11, 13, work.fighters[2], shen_name{}, 3144, 1, 100, 1);
    for (std::int32_t index = 0; index < 3; ++index)
        AllocateBattleDialogueCondition10(10, static_cast<std::int16_t>(109 + index), work.fighters[index], STREET_FIGHT_RYO, 1, 100, 1);
    for (std::int32_t index = 0; index < 3; ++index) {
        if (enabled[index]) {
            battleDialogueIds[index] = AllocateBattleDialogueCondition10(0, -2, work.fighters[index], STREET_FIGHT_RYO, 1, 100, 1);
            EV_SetCharacterAlphaEnabled(work.fighters[index], false);
        } else
            battleDialogueIds[index] = 0;
    }
    HLib::SetCurrentTaskTo(BattleReadyGoCallback);
}
void BattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100D021");
    HLib::SetCurrentTaskTo(BattleStartFightCallback);
    HLib::SetupTaskDelay(65);
}
void BattleStartFightCallback(void *) {
    if (!cvsHasValidConverseData(0)) {
        EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
        HLib::SetCurrentTaskTo(BattleWaitCallback);
    }
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        work.audienceResultPublished = 0;
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
        return;
    }
    for (std::uint32_t index = 0; index < 3; ++index)
        if (battleDialogueIds[index] && GetBattleDialogueCompletionCount(battleDialogueIds[index]))
            break;
    if (work.audienceResultPublished)
        return;
    const std::int32_t status = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
    if (status == 2 || status == 10) {
        EVT_AT_SetVoice("0100D032", 1, 10); // @text EN: "Game!" | JA: "勝負あり！"
        SetAudienceState(work, 7, 1);
        ++work.audienceResultPublished;
        work.battleStatus = 2;
    } else if (status == 3) {
        EVT_AT_SetVoice("0100D032", 1, 10); // @text EN: "Game!" | JA: "勝負あり！"
        SetAudienceState(work, 6, 2);
        ++work.audienceResultPublished;
        work.battleStatus = 3;
    } else if (status > 4 && status <= 7) {
        EVT_AT_SetVoice("0100D033", 1, 10); // @text EN: "Game set!" | JA: "はい、そこまで…"
        SetAudienceState(work, 6, 2);
        ++work.audienceResultPublished;
        work.battleStatus = 6;
    }
}
void BattleWaitFadeCallback(void *) {
    if (!IsFightActive()) {
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(BattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFM2_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_EnableCharacterDisplay(SFM2_SHS);
    EV_EnsureCharacterDraw(SFM2_SHS);
    EV_SetCharacterAlphaEnabled(SFM2_SHS, false);
    for (std::int32_t index = 0; index < 3; ++index) {
        AT_SetLocalBaseChara(work.fighters[index], SFM2_BASE);
        EV_SetCharacterAlphaEnabled(work.fighters[index], false);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(work.fighters[index]), FacialExpressionId::Angry, 1, 1);
    }
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    if (work.battleStatus == 3 || work.battleStatus == 6)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    HLib::SetCurrentTaskTo(MainCallback);
}

std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    for (std::int32_t index = 0; index < 3; ++index)
        EV_EnableCharacterDisplay(work.fighters[index]);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 225, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 225, 200);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6860, 0, 1, 0, 0);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = SFM2_SHS;
    highWork0->selector = 0;
    highWork0->state = 0;
    HLib::SetupTimedTask(highTask0, 120);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFM2_SHS), FacialNodeReplacement);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = SFM2_SHS;
    highWork1->selector = 1;
    highWork1->state = 0;
    HLib::SetupTimedTask(highTask1, 120);
    HLib::Task *const highTask2 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork2 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask2));
    highWork2->task = highTask2;
    highWork2->character = SFM2_SHS;
    highWork2->selector = 1;
    highWork2->state = 1;
    HLib::SetupTimedTask(highTask2, 650);
    EVT_AT_SetPos(work.fighters[0], SFM2_BASE, {-.5f, 0, -1}, 0);
    EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 17, 0);
    EVT_AT_SetMot(work.fighters[0], 6865, 0, .80000001f, 0x10000000U, 0);
    EVT_AT_SetMot(work.fighters[1], 6855, 0, .80000001f, 0x10000000U, 0);
    EVT_AT_SetMot(work.fighters[2], 6851, 0, .80000001f, 0x10000000U, 0);
    EVT_AT_SetMot(SFM2_SHS, 6856, 0, 1, 0, 340);
    EVT_AT_SetMot(work.fighters[0], 6862, 30, 1, 0x50000000U, 280);
    EVT_AT_SetMot(work.fighters[1], 6852, 0, 1, 0x10000000U, 350);
    EVT_AT_SetMot(work.fighters[2], 6849, 30, 1, 0x10000000U, 530);
    HLib::Task *const highTask3 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork3 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask3));
    highWork3->task = highTask3;
    highWork3->character = work.fighters[0];
    highWork3->selector = 1;
    highWork3->state = 1;
    HLib::SetupTimedTask(highTask3, 102);
    HLib::Task *const highTask4 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork4 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask4));
    highWork4->task = highTask4;
    highWork4->character = work.fighters[0];
    highWork4->selector = 0;
    highWork4->state = 1;
    HLib::SetupTimedTask(highTask4, 280);
    Queue_EVT_AT_CtrlHandUp_1(work.fighters[0], 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 30, 280);
    Queue_EVT_AT_CtrlHandUp_1(work.fighters[0], 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 30, 280);
    HLib::Task *const highTask5 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork5 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask5));
    highWork5->task = highTask5;
    highWork5->character = work.fighters[0];
    highWork5->selector = 0;
    highWork5->state = 0;
    HLib::SetupTimedTask(highTask5, 385);
    HLib::Task *const highTask6 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork6 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask6));
    highWork6->task = highTask6;
    highWork6->character = work.fighters[0];
    highWork6->selector = 1;
    highWork6->state = 0;
    HLib::SetupTimedTask(highTask6, 380);
    HLib::Task *const highTask7 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork7 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask7));
    highWork7->task = highTask7;
    highWork7->character = work.fighters[1];
    highWork7->selector = 0;
    highWork7->state = 1;
    HLib::SetupTimedTask(highTask7, 385);
    HLib::Task *const highTask8 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork8 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask8));
    highWork8->task = highTask8;
    highWork8->character = work.fighters[1];
    highWork8->selector = 1;
    highWork8->state = 1;
    HLib::SetupTimedTask(highTask8, 380);
    Queue_EVT_AT_CtrlHandUp_1(work.fighters[1], 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 30, 380);
    Queue_EVT_AT_CtrlHandUp_1(work.fighters[1], 0, reinterpret_cast<std::byte *>(greetingHandPoseCommand), 30, 380);
    HLib::Task *const highTask9 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork9 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask9));
    highWork9->task = highTask9;
    highWork9->character = work.fighters[1];
    highWork9->selector = 0;
    highWork9->state = 0;
    HLib::SetupTimedTask(highTask9, 500);
    HLib::Task *const highTask10 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork10 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask10));
    highWork10->task = highTask10;
    highWork10->character = work.fighters[1];
    highWork10->selector = 1;
    highWork10->state = 0;
    HLib::SetupTimedTask(highTask10, 500);
    EVT_AT_SetMot(work.fighters[1], 6866, 0, 1, 0x40000000U, 280);
    EVT_AT_SetMot(work.fighters[1], 6866, 0, 1, 0x40000000U, 650);
    if (work.rank == 0)
        EVT_AT_SetMot(work.fighters[0], 6862, 30, 1, 0, 820);
    else if (work.rank == 1)
        EVT_AT_SetMot(work.fighters[1], 6852, 0, 1, 0, 820);
    else if (work.rank == 2)
        EVT_AT_SetMot(work.fighters[2], 6849, 30, 1, 0, 820);
    else {
        if (work.rank != 4)
            EVT_AT_SetMot(work.fighters[0], 6862, 30, 1, 0, 820);
        EVT_AT_SetMot(work.fighters[1], 6852, 0, 1, 0, 820);
        if (work.rank != 3)
            EVT_AT_SetMot(work.fighters[2], 6849, 30, 1, 0, 820);
    }
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
    EVT_AT_SetVoice("0100D001", 1, 50); // @text EN: "Come and see this!" | JA: "さあさあ、お立ち会い！"
    EVT_AT_SetVoice("0100D002", 1, 150); // @text EN: "The strongest fighters\nin Mongolian wrestling." | JA: "ここにいるのは\nモンゴル相撲の強者たちだ。"
    EVT_AT_SetVoice("0100D003", 1, 280); // @text EN: "Nadam!" | JA: "ナダム！"
    EVT_AT_SetVoice("0100K017", 1, 305); // @text EN: " " | JA: "おう。"
    EVT_AT_SetVoice("0100D004", 1, 380); // @text EN: "Agon!" | JA: "アゴン！"
    EVT_AT_SetVoice("0100L005", 1, 420); // @text EN: " " | JA: "ふんっ！"
    EVT_AT_SetVoice("0100D005", 1, 500); // @text EN: "And Uzun!" | JA: "そして、ウズン！"
    EVT_AT_SetVoice("0100D006", 1, 590); // @text EN: "The rules are easy.\nStay away from them until time runs out!" | JA: "ルールは簡単、こいつらから\n制限時間まで逃げ切れば勝ち。"
    EVT_AT_SetVoice("0100D007", 1, 750); // @text EN: "If you get caught, you lose." | JA: "捕まったら負けだ。"
    std::int32_t duration;
    if (work.rank == 0) {
        EVT_AT_SetVoice("0100D008", 1, 820); // @text EN: "First, Nadam." | JA: "最初はナダムと勝負。"
        EVT_AT_SetVoice("0100D012", 1, 880); // @text EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D010", 1, 970); // @text EN: "Time limit, thirty seconds." | JA: "制限時間は３０秒。"
        EVT_AT_SetVoice("0100D011", 1, 1030); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        HLib::Task *const highTask11 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork11 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask11));
        highWork11->task = highTask11;
        highWork11->character = SFM2_SHS;
        highWork11->selector = 1;
        highWork11->state = 0;
        HLib::SetupTimedTask(highTask11, 940);
        EVT_AT_SetCam(121, SFM2_BASE, SFM2_BASE, 0, 0, 940);
        EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 940);
        EVT_AT_SetCam(1300, SFM2_BASE, SFM2_BASE, 0, 0, 1050);
        EVT_AT_SetPos(work.fighters[0], SFM2_BASE, {-.5f, 0, -1}, 1050);
        EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 17, 1050);
        EVT_AT_SetMot(work.fighters[0], 6865, 0, .80000001f, 0x50000000U, 1050);
        HLib::Task *const highTask12 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork12 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask12));
        highWork12->task = highTask12;
        highWork12->character = work.fighters[0];
        highWork12->selector = 0;
        highWork12->state = 1;
        HLib::SetupTimedTask(highTask12, 1050);
        Queue_EVT_AT_CtrlHandUp_1(work.fighters[0], 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0, 1050);
        Queue_EVT_AT_CtrlHandUp_1(work.fighters[0], 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0, 1050);
        duration = 1160;
    } else if (work.rank == 1) {
        EVT_AT_SetVoice("0100D016", 1, 820); // @text EN: "Looking interesting.\nThe next opponent, Agon." | JA: "さあ、おもしろくなってきた。\n次はアゴンが相手だ。"
        EVT_AT_SetVoice("0100D013", 1, 980); // @text EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D010", 1, 1080); // @text EN: "Time limit, thirty seconds." | JA: "制限時間は３０秒。"
        EVT_AT_SetVoice("0100D011", 1, 1140); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        EVT_AT_SetCam(121, SFM2_BASE, SFM2_BASE, 0, 0, 1040);
        HLib::Task *const highTask13 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork13 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask13));
        highWork13->task = highTask13;
        highWork13->character = SFM2_SHS;
        highWork13->selector = 1;
        highWork13->state = 0;
        HLib::SetupTimedTask(highTask13, 1040);
        EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 1040);
        duration = 1190;
    } else if (work.rank == 2) {
        EVT_AT_SetVoice("0100D017", 1, 820); // @text EN: "Now, here's the strongest=@\nUzun is next!" | JA: "いよいよ、真打ちの登場だ。\n次の相手はウズン！"
        EVT_AT_SetVoice("0100D014", 1, 980); // @text EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D010", 1, 1080); // @text EN: "Time limit, thirty seconds." | JA: "制限時間は３０秒。"
        EVT_AT_SetVoice("0100D011", 1, 1140); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        EVT_AT_SetCam(121, SFM2_BASE, SFM2_BASE, 0, 0, 1080);
        HLib::Task *const highTask14 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork14 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask14));
        highWork14->task = highTask14;
        highWork14->character = SFM2_SHS;
        highWork14->selector = 1;
        highWork14->state = 0;
        HLib::SetupTimedTask(highTask14, 1080);
        EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 1080);
        duration = 1210;
    } else {
        EVT_AT_SetVoice("0100D018", 1, 820); // @text EN: "An uneven matchup next!\nThey will ALL be your opponents!" | JA: "さあ、次は変則マッチだ！\nこいつらが相手だぜ！"
        EVT_AT_SetVoice(work.rank == 3 ? "0100D015" : "0100D009", 1, 970); // @text 0100D015 EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！" ; 0100D009 EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D010", 1, 1050); // @text EN: "Time limit, thirty seconds." | JA: "制限時間は３０秒。"
        EVT_AT_SetVoice("0100D011", 1, 1130); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        EVT_AT_SetCam(121, SFM2_BASE, SFM2_BASE, 0, 0, 1040);
        HLib::Task *const highTask15 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork15 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask15));
        highWork15->task = highTask15;
        highWork15->character = SFM2_SHS;
        highWork15->selector = 1;
        highWork15->state = 0;
        HLib::SetupTimedTask(highTask15, 1040);
        EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 1040);
        duration = 1210;
    }
    EVT_AT_SetCam(101, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    EVT_AT_SetCam(102, SFM2_BASE, SFM2_BASE, 0, 0, 120);
    EVT_AT_SetCam(2103, SFM2_BASE, SFM2_BASE, 0, 0, 280);
    EVT_AT_SetCam(2107, SFM2_BASE, SFM2_BASE, 0, 0, 650);
    return duration;
}

std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    std::int32_t visible[3]{1, 1, 1};
    if (work.rank == 4)
        visible[0] = 0;
    else
        visible[2] = work.rank != 3;
    for (std::int32_t index = 0; index < 3; ++index) {
        if (visible[index])
            EV_EnableCharacterDisplay(work.fighters[index]);
        else
            EV_DisableCharacterDisplay(work.fighters[index]);
    }
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6860, 0, 1, 0x40000000U, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFM2_SHS);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0x40000000U, 0);
    StreetFightLookWork *const bookieLook = QueueStreetFightLook(SFM2_SHS, STREET_FIGHT_RYO);
    bookieLook->reserved08 = 15;
    EVT_AT_SetPos(work.fighters[0], SFM2_BASE, SFM2_REPEAT_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(work.fighters[0], SFM2_BASE, 17, 0);
    EVT_AT_SetMot(work.fighters[0], 6865, 0, .80000001f, 0x50000000U, 0);
    EVT_AT_SetMot(work.fighters[1], 6855, 0, .80000001f, 0x50000000U, 0);
    EVT_AT_SetMot(work.fighters[2], 6851, 0, .80000001f, 0x50000000U, 0);
    QueueStreetFightLook(work.fighters[0], STREET_FIGHT_RYO);
    SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(work.fighters[0]), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(work.fighters[0]), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    const std::int32_t fee = SFM2_ENTRY_FEES[work.rank];
    std::int32_t duration;
    if (fee == 10 || fee == 20 || fee == 30) {
        EVT_AT_SetVoice(fee == 10 ? "0100D012" : fee == 20 ? "0100D013" : "0100D014", 1, 0); // @text 0100D012 EN: "One match for a bet of $10!" | JA: "賭け金１０ドルの１本勝負だ！" ; 0100D013 EN: "One match for a bet of $20!" | JA: "賭け金２０ドルの１本勝負だ！" ; 0100D014 EN: "One match for a bet of $30!" | JA: "賭け金３０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D011", 1, 80); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        duration = 180;
    } else {
        EVT_AT_SetVoice("0100D018", 1, 0); // @text EN: "An uneven matchup next!\nThey will ALL be your opponents!" | JA: "さあ、次は変則マッチだ！\nこいつらが相手だぜ！"
        EVT_AT_SetVoice(fee == 40 ? "0100D015" : "0100D009", 1, 141); // @text 0100D015 EN: "One match for a bet of $40!" | JA: "賭け金４０ドルの１本勝負だ！" ; 0100D009 EN: "One match for a bet of $50!" | JA: "賭け金５０ドルの１本勝負だ！"
        EVT_AT_SetVoice("0100D011", 1, fee == 40 ? 215 : 225); // @text EN: "Anyone confident enough to try?\nLet's see some hands up!" | JA: "さあ、我と思わん者は\n手をあげてくれ！"
        duration = fee == 40 ? 330 : 340;
    }
    const std::uint16_t camera = work.rank == 0 ? 1300 : work.rank == 1 ? 1301 : work.rank == 2 ? 1302 : work.rank == 3 ? 1310 : work.rank == 4 ? 1311 : 1312;
    EVT_AT_SetCam(camera, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    if (work.rank != 0 && work.rank != 1 && work.rank != 2)
        EVT_AT_SetCam(work.rank == 3 ? 1303 : work.rank == 4 ? 1304 : 1305, SFM2_BASE, SFM2_BASE, 0, 0, 120);
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
        .delay = 2});
    return duration;
}
std::int32_t NextAcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EVT_AT_SetPoint(SFM2_SHS, STREET_FIGHT_RYO, 5, 3, 0, 0, 110, 0);
    EVT_AT_SetVoice("0100A001", 1, 30); // @text EN: "I'll do it." | JA: "いいぜ。"
    EVT_AT_SetVoice("0100D030", 1, 60); // @text EN: "All right then." | JA: "よし決まりだ。"
    SetAudienceState(work, 4, 60);
    return 110;
}
std::int32_t StopConversation(void *) {
    EVT_AT_SetVoice("0100A079", 1, 30); // @text EN: "No, I won't." | JA: "いや、やめとく。"
    EVT_AT_SetVoice("0100D031", 1, 90); // @text EN: "Really?\nThat's too bad." | JA: "そうか、残念だな。"
    EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    return 200;
}

std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6859, 0, 1, 0x40000000U, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(greetingHandPoseCommand), 30);
    SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 0, 1, 0);
    EVT_AT_SetPoint(SFM2_SHS, STREET_FIGHT_RYO, 5, 3, 0, 30, 156, 0);
    if (work.rank == 0)
        QueueStreetFightLook(work.fighters[0], STREET_FIGHT_RYO);
    else if (work.rank == 1)
        QueueStreetFightLook(work.fighters[1], STREET_FIGHT_RYO);
    else if (work.rank == 2)
        QueueStreetFightLook(work.fighters[2], STREET_FIGHT_RYO);
    else if (work.rank == 3) {
        QueueStreetFightLook(work.fighters[0], STREET_FIGHT_RYO);
        QueueStreetFightLook(work.fighters[1], STREET_FIGHT_RYO);
    } else {
        if (work.rank != 4)
            QueueStreetFightLook(work.fighters[0], STREET_FIGHT_RYO);
        QueueStreetFightLook(work.fighters[1], STREET_FIGHT_RYO);
        QueueStreetFightLook(work.fighters[2], STREET_FIGHT_RYO);
    }
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    EVT_AT_SetVoice("0100D019", 1, 60); // @text EN: "All right." | JA: "よし。決まりだ。"
    EVT_AT_SetCam(130, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    EVT_AT_SetCam(141, SFM2_BASE, SFM2_BASE, 0, 0, 90);
    SetAudienceState(work, 4, 60);
    return 156;
}
std::int32_t FirstBattleConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = fighter;
    highWork0->selector = 0;
    highWork0->state = 0;
    HLib::SetupTimedTask(highTask0, 150);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = fighter;
    highWork1->selector = 1;
    highWork1->state = 0;
    HLib::SetupTimedTask(highTask1, 150);
    switch (work.fighterIndex) {
    case 0:
        EVT_AT_SetCam(2104, SFM2_BASE, SFM2_BASE, 0, 0, 0);
        EVT_AT_SetVoice(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1785) ? "0100K018" : "0100K001", 1, 0); // @text 0100K018 EN: "Won't be easy." | JA: "手かげんしねえぜ。" ; 0100K001 EN: "He's got some skill." | JA: "少しは骨がありそうだな。"
        return 150;
    case 1:
        EVT_AT_SetCam(2105, SFM2_BASE, SFM2_BASE, 0, 0, 0);
        EVT_AT_SetVoice(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1993) ? "0100L011" : "0100L010", 1, 0); // @text 0100L011 EN: "Come and get me, kid." | JA: "かかってこい。小僧。" ; 0100L010 EN: "I'll give you some training." | JA: "いっちょう、もんでやるか。"
        return 150;
    case 2:
        EVT_AT_SetCam(2106, SFM2_BASE, SFM2_BASE, 0, 0, 0);
        EVT_AT_SetVoice("0100M005", 1, 0); // @text EN: "We already know the result=@" | JA: "勝負はもう見えている…"
        return 150;
    default:
        return 0;
    }
}
std::int32_t NextBattleConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = fighter;
    highWork0->selector = 0;
    highWork0->state = 0;
    HLib::SetupTimedTask(highTask0, 70);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    HLib::Task *const highTask1 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
    highWork1->task = highTask1;
    highWork1->character = fighter;
    highWork1->selector = 1;
    highWork1->state = 0;
    HLib::SetupTimedTask(highTask1, 70);
    if (work.fighterIndex == 1)
        EV_PlayDialogue(work.rankHistory[work.rank] ? "0100L011" : "0100L010");
    else if (work.fighterIndex == 2)
        EV_PlayDialogue("0100M005");
    return 70;
}
std::int32_t DeclineConversation(void *) {
    const char *const voices[]{"0100A038", "0100A039", "0100A020", "0100A040"}; // @text 0100A038 EN: "No, I don't think so." | JA: "やめておこう…" ; 0100A039 EN: "Maybe next time." | JA: "またにするか…" ; 0100A020 EN: "I won't." | JA: "やめておくか…" ; 0100A040 EN: "I might try next time." | JA: "また今度にするか…"
    EVT_AT_SetVoice(voices[shenGetRandomInt32(4)], 1, 10);
    EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    return 60;
}
std::int32_t InsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const count = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    count->cleanup = 1;
    count->delay = 60;
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    return 150;
}

std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6868, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 0, 0);
    EVT_AT_SetMot(SFM2_SHS, 6858, 0, 1.1f, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 1, 0);
    EVT_AT_SetPos(fighter, SFM2_BASE, SFM2_VICTORY_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFM2_BASE, 17, 0);
    if (!work.fighterIndex) {
        for (std::int32_t index = 0; index < 3; ++index)
            EV_EnableCharacterDisplay(work.fighters[index]);
        EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {4, -.5f, .2f}, 0);
        EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 71, 0);
        EVT_AT_SetPos(work.fighters[2], SFM2_BASE, SFM2_VICTORY_SPECTATOR_POSITION, 0);
        EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 72, 0);
        EVT_AT_SetPos(work.fighters[1], SFM2_BASE, {2.8f, -.5f, -.6f}, 380);
        EVT_AT_SetAngY(work.fighters[1], SFM2_BASE, 71, 380);
        EVT_AT_SetPos(work.fighters[2], SFM2_BASE, {2.9f, -.5f, .6f}, 380);
        EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 72, 380);
        EVT_AT_SetMot(work.fighters[1], 6852, 0, 1, 0x20000000U, 270);
    } else {
        EV_DisableCharacterDisplay(work.fighters[0]);
        if (work.fighterIndex == 1) {
            EV_EnableCharacterDisplay(work.fighters[1]);
            EV_EnableCharacterDisplay(work.fighters[2]);
            EVT_AT_SetPos(work.fighters[2], SFM2_BASE, {4, -.5f, .2f}, 0);
            EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 72, 0);
            EVT_AT_SetPos(work.fighters[2], SFM2_BASE, {2.8f, -.5f, -.6f}, 430);
            EVT_AT_SetAngY(work.fighters[2], SFM2_BASE, 72, 430);
            EVT_AT_SetMot(work.fighters[2], 6849, 0, 1, 0x20000000U, 320);
        } else {
            EV_DisableCharacterDisplay(work.fighters[1]);
            EV_EnableCharacterDisplay(work.fighters[2]);
        }
    }
    EVT_AT_SetCam(215, SFM2_BASE, SFM2_BASE, 0, 0, 0);
    if (work.fighterIndex >= 0 && work.fighterIndex <= 2)
        EVT_AT_SetMot(fighter, work.fighterIndex == 0 ? 6864 : work.fighterIndex == 1 ? 6854 : 6850, 0, .80000001f, 0x50000000U, 0);
    std::int32_t paymentFrame, duration = 380;
    if (work.rank == 0) {
        EVT_AT_SetVoice("0100K019", 1, 25); // @text EN: "You're good.\nNice moves." | JA: "やるな。いい動きしてたぜ。"
        EVT_AT_SetVoice("0100D022", 1, 130); // @text EN: "Terrific!\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
        EVT_AT_SetVoice("0100D023", 1, 270); // @text EN: "Why don't you challenge him next?" | JA: "兄ちゃん、次はアイツと\n勝負しねえか？"
        EVT_AT_SetVoice("0100D027", 1, 390); // @text EN: "The bet is $20.\nHow about it?" | JA: "賭け金は２０ドル。\nどうだ？"
        EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 170);
        EVT_AT_SetCam(2100, SFM2_BASE, SFM2_BASE, 0, 0, 270);
        EVT_AT_SetCam(2101, SFM2_BASE, SFM2_BASE, 0, 0, 380);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(work.fighters[0]), FacialNodeReplacement);
        SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[0]), 1, 1, 0);
        HLib::Task *const highTask0 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
        highWork0->task = highTask0;
        highWork0->character = SFM2_SHS;
        highWork0->selector = 1;
        highWork0->state = 1;
        HLib::SetupTimedTask(highTask0, 270);
        HLib::Task *const highTask1 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
        highWork1->task = highTask1;
        highWork1->character = work.fighters[1];
        highWork1->selector = 0;
        highWork1->state = 1;
        HLib::SetupTimedTask(highTask1, 270);
        HLib::Task *const highTask2 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork2 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask2));
        highWork2->task = highTask2;
        highWork2->character = SFM2_SHS;
        highWork2->selector = 0;
        highWork2->state = 0;
        HLib::SetupTimedTask(highTask2, 380);
        HLib::Task *const highTask3 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork3 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask3));
        highWork3->task = highTask3;
        highWork3->character = SFM2_SHS;
        highWork3->selector = 1;
        highWork3->state = 0;
        HLib::SetupTimedTask(highTask3, 380);
        HLib::Task *const highTask4 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork4 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask4));
        highWork4->task = highTask4;
        highWork4->character = work.fighters[1];
        highWork4->selector = 1;
        highWork4->state = 1;
        HLib::SetupTimedTask(highTask4, 380);
        paymentFrame = 180;
        duration = 445;
    } else if (work.rank == 1) {
        EVT_AT_SetVoice("0100L008", 1, 35); // @text EN: "I didn't think he could escape.\nI underestimated him=@" | JA: "まさか、逃げ切られるとは…\n少し、見くびっていたようだ…"
        EVT_AT_SetVoice("0100D022", 1, 190); // @text EN: "Terrific!\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
        EVT_AT_SetVoice("0100D023", 1, 330); // @text EN: "Why don't you challenge him next?" | JA: "兄ちゃん、次はアイツと\n勝負しねえか？"
        EVT_AT_SetVoice("0100D026", 1, 440); // @text EN: "The bet is $30.\nHow about it?" | JA: "賭け金は３０ドル。\nどうだ？"
        EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 230);
        EVT_AT_SetCam(2100, SFM2_BASE, SFM2_BASE, 0, 0, 330);
        EVT_AT_SetCam(2101, SFM2_BASE, SFM2_BASE, 0, 0, 430);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(work.fighters[1]), FacialNodeReplacement);
        SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[1]), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(work.fighters[1]), 1, 1, 0);
        HLib::Task *const highTask0 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
        highWork0->task = highTask0;
        highWork0->character = SFM2_SHS;
        highWork0->selector = 1;
        highWork0->state = 1;
        HLib::SetupTimedTask(highTask0, 330);
        HLib::Task *const highTask1 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork1 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask1));
        highWork1->task = highTask1;
        highWork1->character = work.fighters[2];
        highWork1->selector = 0;
        highWork1->state = 1;
        HLib::SetupTimedTask(highTask1, 330);
        HLib::Task *const highTask2 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork2 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask2));
        highWork2->task = highTask2;
        highWork2->character = SFM2_SHS;
        highWork2->selector = 0;
        highWork2->state = 0;
        HLib::SetupTimedTask(highTask2, 430);
        HLib::Task *const highTask3 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork3 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask3));
        highWork3->task = highTask3;
        highWork3->character = SFM2_SHS;
        highWork3->selector = 1;
        highWork3->state = 0;
        HLib::SetupTimedTask(highTask3, 430);
        HLib::Task *const highTask4 =
            HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
        StreetFightHighModelWork *const highWork4 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask4));
        highWork4->task = highTask4;
        highWork4->character = work.fighters[2];
        highWork4->selector = 1;
        highWork4->state = 1;
        HLib::SetupTimedTask(highTask4, 430);
        paymentFrame = 250;
        duration = 460;
    } else {
        EVT_AT_SetVoice(work.rank == 3 ? "0100L012" : work.rank == 4 ? "0100M010" : "0100M002", 1, 35); // @text 0100L012 EN: "Damn!\nWe were defeated=@" | JA: "くそっ、不覚をとった…" ; 0100M010 EN: "I was too overconfident=@" | JA: "心に油断があったか…" ; 0100M002 EN: "He beat me.\nI lost=@" | JA: "まいった。完敗だ…"
        EVT_AT_SetVoice("0100D022", 1, 150); // @text EN: "Terrific!\nHere you go." | JA: "スゲエなぁ、兄ちゃん。\nほらよ。"
        EVT_AT_SetCam(210, SFM2_BASE, SFM2_BASE, 0, 0, 210);
        const shen_name speaker = work.rank == 3 ? work.fighters[1] : work.fighters[2];
        SetHighModelXYZ(FindCharacterByScenarioName(speaker), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(speaker), 1, 1, 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(speaker), FacialNodeReplacement);
        if (work.rank == 2) {
            paymentFrame = 205;
            if (work.state != 14) {
                duration = 340;
                EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
                    .frames = 30,
                    .startAlpha = 0,
                    .startRed = 0,
                    .startGreen = 0,
                    .startBlue = 0,
                    .targetAlpha = 255,
                    .targetRed = 0,
                    .targetGreen = 0,
                    .targetBlue = 0,
                    .delay = 310});
            } else
                duration = 300;
        } else if (work.rank == 3 || work.rank == 4) {
            paymentFrame = 205;
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
                    .frames = 30,
                    .startAlpha = 0,
                    .startRed = 0,
                    .startGreen = 0,
                    .startBlue = 0,
                    .targetAlpha = 255,
                    .targetRed = 0,
                    .targetGreen = 0,
                    .targetBlue = 0,
                    .delay = 350});
        } else {
            paymentFrame = 210;
            duration = 340;
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
                    .frames = 30,
                    .startAlpha = 0,
                    .startRed = 0,
                    .startGreen = 0,
                    .startBlue = 0,
                    .targetAlpha = 255,
                    .targetRed = 0,
                    .targetGreen = 0,
                    .targetBlue = 0,
                    .delay = 310});
        }
    }
    PATI_StartMoneyCount(static_cast<std::uint32_t>(SFM2_ENTRY_FEES[work.rank]), paymentFrame);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    return duration;
}
std::int32_t VictoryAppearanceConversation(void *) {
    EV_EnableCharacterDisplay(SFM2_CHN);
    EV_EnsureCharacterDraw(SFM2_CHN);
    EV_SetCharacterAlphaEnabled(SFM2_CHN, false);
    EVT_AT_SetPos(SFM2_CHN, STREET_FIGHT_INVALID, SFM2_VICTORY_APPEARANCE_POSITION, 0);
    EVT_AT_SetAngY(SFM2_CHN, STREET_FIGHT_INVALID, 120, 0);
    EVT_AT_SetMot(SFM2_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    EVT_AT_SetCam(9901, SFM2_CHN, SFM2_CHN, 0, 0, 0);
    return 60;
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFM2_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFM2_BASE, 180, 1);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6867, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(SFM2_SHS, SFM2_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFM2_SHS, SFM2_BASE, 0, 1);
    EVT_AT_SetMot(SFM2_SHS, 6857, 0, 1.1f, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFM2_SHS), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFM2_SHS), 1, 1, 0);
    HLib::Task *const highTask0 =
        HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const highWork0 = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(highTask0));
    highWork0->task = highTask0;
    highWork0->character = SFM2_SHS;
    highWork0->selector = 1;
    highWork0->state = 0;
    HLib::SetupTimedTask(highTask0, 190);
    EVT_AT_SetPos(fighter, SFM2_BASE, {-.788f, 0, -2.2f}, 1);
    EVT_AT_SetAngY(fighter, SFM2_BASE, 17, 1);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    const std::int32_t visible = work.rank == 0 ? 0 : work.rank == 1 || work.rank == 3 ? 1 : 2;
    for (std::int32_t index = 0; index < 3; ++index) {
        if (index == visible)
            EV_EnableCharacterDisplay(work.fighters[index]);
        else
            EV_DisableCharacterDisplay(work.fighters[index]);
    }
    EVT_AT_SetCam(static_cast<std::uint16_t>(125 + visible), SFM2_BASE, SFM2_BASE, 0, 0, 0);
    EVT_AT_SetCam(310, SFM2_BASE, SFM2_BASE, 0, 0, work.rank == 2 ? 400 : 370);
    if (work.rank == 0 || work.rank == 1 || work.rank == 3) {
        const std::int32_t motion = work.rank == 0 ? 6863 : 6853;
        EVT_AT_SetMot(fighter, motion, 0, 0, 0, 0);
        EVT_AT_SetMot(fighter, motion, 0, 1, 0, 130);
    } else
        EVT_AT_SetMot(fighter, 6849, 0, .80000001f, 0x10000000U, 0);
    EVT_AT_SetVoice("0100D029", 1, 25); // @text EN: "Too bad!\nBut you came close." | JA: "おしかったな、兄ちゃん。\nでも、いいセンいってたぜ。"
    EVT_AT_SetVoice(work.rank == 0 ? "0100K020" : // @text EN: "You can't get away from me,\nbeing that slow." | JA: "あの程度じゃ\nオレからは逃げきれん。"
            work.rank == 1         ? "0100L003" : // @text EN: "We know the results even before\nthe match." | JA: "結果は最初からわかっていた。"
            work.rank == 2         ? "0100M007" : // @text EN: "I used to fight wolves\nin Mongolia." | JA: "モンゴルじゃ、\nオオカミが相手だった。"
            work.rank == 3         ? "0100L019" : // @text EN: "Two against one was definitely tough." | JA: "さすがに２人相手だと\n苦しかったか。"
            work.rank == 4         ? "0100M011" : // @text EN: "You don't have to feel ashamed.\nYou had a good fighting spirit." | JA: "恥じることはない。\nよく闘った。"
                                     "0100M012", // @text EN: "You shouldn't put too much confidence\nin your abilities." | JA: "あまり自分の腕を\n過信せぬことだ。"
        1, 190);
    if (work.rank == 0)
        EVT_AT_SetVoice("0100K021", 1, 310); // @text EN: "Need more training." | JA: "出直してくるんだな。"
    else if (work.rank == 1)
        EVT_AT_SetVoice("0100L004", 1, 280); // @text EN: "Get more training." | JA: "もっときたえることだ…"
    else if (work.rank == 2)
        EVT_AT_SetVoice("0100M008", 1, 310); // @text EN: "Compared to wolves,\na man's movement is nothing=@" | JA: "それに比べたら、\n人間の動きなど…"
    const std::int32_t paymentFrame = work.rank == 1 ? 360 : work.rank == 2 ? 420 : 390;
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFM2_ENTRY_FEES[work.rank]), paymentFrame);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    return paymentFrame + 100;
}

void ResetToMain(StreetFightEventWork &work) {
    EV_ResetFreeTalkFlags();
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    SetFighterIndex(work, work.rank);
    work.state = 0;
    work.auxiliaryState = 0;
    work.audienceResultPublished = 0;
    EV_EnableCharacterDisplay(work.fighters[0]);
    EV_EnableCharacterDisplay(work.fighters[1]);
    EV_EnableCharacterDisplay(work.fighters[2]);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    HLib::SetCurrentTaskTo(MainCallback);
}
void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 730, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 732, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 734, 1);
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
    hndlSetFlags(FindCharacterByScenarioName(SFM2_SHS));
    hndlSetFlags(FindCharacterByScenarioName(SFM2_SF2));
    hndlSetFlags(FindCharacterByScenarioName(SFM2_KZN));
    hndlSetFlags(FindCharacterByScenarioName(SFM2_SSY));
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0);
    SetCharaKill(SFM2_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFM2_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFM2_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    if (work.hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(static_cast<std::uint32_t>(*motion));
    EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFM2_RETRY_AREA, 1);
    else if (work.reserved178)
        EV_scenQueueLoad(g_actNumber, SFM2_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->motions[0] = 0;
    work->fighters[0] = SFM2_KZN;
    work->fighters[1] = SFM2_SSY;
    work->fighters[2] = SFM2_SF2;
    work->fighters[3] = STREET_FIGHT_INVALID;
    work->package = SFM2_NORMAL_PACKAGE;
    work->battleFrames = 54;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFM2_CHARACTERS);
    work->audienceTask = nullptr;
    work->hudEnabled = 0;
    work->reserved178 = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    SetFighterIndex(*work, work->rank);
    switch (work->stage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
        for (std::uint32_t index = 0; index < 15; ++index)
            work->rankHistory[index] = 0;
        break;
    default:
        break;
    }
    return work;
}
void LoadNormalResources(StreetFightEventWork &work) {
    work.motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QSFA"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QSFA"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QSFA"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFM2_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    shenSetClockAndCompassRenderTo(1);
    FinishLoading();
}
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = start;
    work->audienceResultPublished = 0;
    LoadNormalResources(*work);
    work->fadeFrames = static_cast<std::uint8_t>(start->scene);
}
void Init() {
    battleDialogueIds[0] = battleDialogueIds[1] = battleDialogueIds[2] = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
} // namespace
const ScriptAreaDefinition areaSfm2ScriptDefinition{.areaId = STREET_FIGHT_SFM2, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
