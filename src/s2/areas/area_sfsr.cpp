#include "s2/areas/area_sfsr.hpp"
#include "s2/author.hpp"
#include "s2/event_call.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/legacy_draw.hpp"
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
    work->package = SFSR_DEBUG_PACKAGE;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFSR_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QLBT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFSR_BASE);
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
    EV_EnableCharacterDisplay(SFSR_FUY);
    EV_EnsureCharacterDraw(SFSR_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFSR_BASE, .parent = STREET_FIGHT_INVALID, .position = SFSR_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFSR_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 0.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFSR_BASE);
    AT_SetLocalBaseChara(SFSR_FUY, SFSR_BASE);
    AT_SetLocalBaseChara(fighter, SFSR_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFSR_BASE, .position = SFSR_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFSR_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFSR_FUY, .parent = SFSR_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFSR_FUY, .parent = SFSR_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFSR_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFSR_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFSR_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFSR_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFSR_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFSR_DEBUG_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
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
        EV_GetCharaPosition(SFSR_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFSR_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFSR_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFSR_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFSR_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFSR_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFSR_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFSR_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfsrStreetFightConversation(StartStreetFightBookieConversation, &work);
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
        ReadBookiePosition(SFSR_FUY);
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
        QueueSfsrStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfsrStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfsrStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFSR_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfsrStreetFightConversation(StreetFightVictoryConversation, &work);
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
        QueueSfsrStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfsrStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfsrStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFSR_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfsrStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfsrStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfsrStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfsrStreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFSR_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFSR_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFSR_FUY, .parent = SFSR_BASE, .position = SFSR_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFSR_FUY, .parent = SFSR_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFSR_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFSR_BASE, .degrees = 0, .delay = 0};
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
    EventSetCamera camera{0, 2001, SFSR_BASE, SFSR_BASE, 0, {}, 0, 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QLBT"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFSR_DEBUG_OPPONENT_ORIGIN.x, SFSR_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFSR_BASE);
    AT_SetLocalBaseChara(fighter, SFSR_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFSR_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, SFSR_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFSR_BASE, SFSR_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFSR_BASE, SFSR_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFSR_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFSR_FUY, SFSR_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFSR_FUY, SFSR_BASE, 0, 0);
    EVT_AT_SetMot(SFSR_FUY, 0x1aed, 0, 1.1f, 0, 0);
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
    EVT_AT_SetCam(2006, SFSR_BASE, SFSR_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFSR_BASE, SFSR_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, SFSR_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFSR_FUY, SFSR_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFSR_FUY, SFSR_BASE, 0, 0);
    EVT_AT_SetMot(SFSR_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFSR_BASE, SFSR_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFSR_BASE, SFSR_BASE, 0, 0, 5);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFSR_FUY, SFSR_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFSR_FUY, SFSR_BASE, 0, 0);
    EVT_AT_SetMot(SFSR_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFSR_BASE, SFSR_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFSR_BASE, SFSR_BASE, 0, 0, 5);
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

HLib::Task *spotlightTask{};
StreetFightStartParameters *ownerEntry{};
std::uint8_t qtePhase{}, ringOutPhase{}, ringOutDelay{60};
Vector4 savedModelColour{};
shen_name spotlightTargets[2]{};
Vector3 spotlightPositions[2]{};
struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name command;
    std::uint16_t state;
    std::uint16_t reserved12;
    std::uint32_t reserved14;
};
struct AuthorLoopWork {
    AuthorWork *owner;
    std::uint64_t reserved08;
};
static_assert(sizeof(AuthorWork) == 24);
static_assert(offsetof(AuthorWork, state) == 16);
static_assert(sizeof(AuthorLoopWork) == 16);
void MainCallback(void *);
void CleanupNormalCallback(void *);
void CleanupWaitCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void AuthorCallback(void *);
void AuthorLoopCallback(void *);
void SpotlightCallback(void *);
void SetupSpotlights();
void InitializeBattle(StreetFightEventWork &);
std::int32_t DefeatConversation(void *);
std::int32_t RoundVictoryConversation(void *);
void LoadAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    EV_EnableCharacterDisplay(SFSR_SHADER_CHARACTER);
    EV_SetCharacterAlphaEnabled(SFSR_SHADER_CHARACTER, 0);
    for (std::uint32_t index = 0; SFSR_AUTHOR_CHARACTERS[index].name != STREET_FIGHT_INVALID.name; ++index) {
        const shen_name character = SFSR_AUTHOR_CHARACTERS[index];
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, 0);
    }
}
void UnloadAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 1);
    EV_DisableCharacterDisplay(SFSR_SHADER_CHARACTER);
    EV_SetCharacterAlphaEnabled(SFSR_SHADER_CHARACTER, 1);
    for (std::uint32_t index = 0; SFSR_AUTHOR_CHARACTERS[index].name != STREET_FIGHT_INVALID.name; ++index) {
        const shen_name character = SFSR_AUTHOR_CHARACTERS[index];
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharacterAlphaEnabled(character, 1);
        EV_EnableCharacterDisplay(character);
        EV_EnsureCharacterDraw(character);
    }
}
void QueueAuthorLoop(AuthorWork *owner) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 0x0b, sizeof(AuthorLoopWork), STREET_FIGHT_EVENT_TASK);
    AuthorLoopWork *const loop = static_cast<AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop->owner = owner;
    EV_SetModelDataDefaultVertexShader();
    switch (owner->command.name) {
    case 0:
        EV_lightLoad(SHEN_NAME(l001));
        break;
    case 1:
        EV_lightLoad(SHEN_NAME(l012));
        break;
    case 2:
        EV_lightLoad(SHEN_NAME(l023));
        break;
    case 3:
        EV_lightLoad(SHEN_NAME(l204));
        break;
    default:
        break;
    }
    PushGlobalLightingProgram();
    AssignLightProgram(0);
    SetDefaultVSByIndex(1);
    if (owner->command.name == SFSR_AUTH_INTRO.name)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    LoadAuthorCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(owner->command.name));
    owner->state = 2;
}
void AuthorCallback(void *parameter) {
    AuthorWork *const work = static_cast<AuthorWork *>(parameter);
    if (work->state == 1)
        QueueAuthorLoop(work);
    else if (work->state == 3)
        HLib::DestroyTaskInternal(work->task);
}
void QueueAuthor(shen_name command) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorCallback, 4, 0x0b, sizeof(AuthorWork), STREET_FIGHT_EVENT_TASK);
    AuthorWork *const work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->state = 1;
    work->task = task;
    work->command = command;
    SetPlayerControlEnabled(0);
}
void AuthorLoopCallback(void *parameter) {
    AuthorLoopWork &loop = *static_cast<AuthorLoopWork *>(parameter);
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (loop.owner->command.name == SFSR_AUTH_INTRO.name) {
        switch (frame) {
        case 1:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SFB), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SFB), 0, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SFB), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            EV_DisableCharacterDisplay(SFSR_CHN);
            EV_ToggleCharacterFlag(SFSR_CHN);
            SetNewNpcBatchCountOverride(NpcFillType::BackFill, 2);
            break;
        case 200:
            SetNewNpcBatchCountOverride(NpcFillType::BackFill, 8);
            break;
        case 351:
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 446:
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 811:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            break;
        case 1036:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 1476:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 1541:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 1, 0);
            break;
        case 1651:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 1, 0);
            break;
        case 2084:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 1, 0);
            break;
        case 2351:
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 2671:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            EV_DisableCharacterDisplay(SFSR_SF3);
            EV_ToggleCharacterFlag(SFSR_SF3);
            break;
        case 2801:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        case 3706:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            EV_EnableCharacterDisplay(SFSR_SF3);
            EV_EnsureCharacterDraw(SFSR_SF3);
            break;
        case 3860:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 4000:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        default:
            break;
        }
    } else if (loop.owner->command.name == SFSR_AUTH_RESULT.name) {
        switch (frame) {
        case 1:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            EV_DisableCharacterDisplay(SFSR_SFB);
            EV_ToggleCharacterFlag(SFSR_SFB);
            break;
        case 63:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        case 119:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 208:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        case 326:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            break;
        case 796:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            break;
        case 1016:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            break;
        case 1160:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        case 1496:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            break;
        case 1549:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 1633:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 1668:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 1701:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
            break;
        case 1814:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            break;
        case 1874:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            break;
        case 1930:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 1989:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
            break;
        case 2029:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 1, 1, 0);
            break;
        case 2131:
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
            break;
        case 2207:
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
            EV_EnableCharacterDisplay(SFSR_CHN);
            EV_EnsureCharacterDraw(SFSR_CHN);
            break;
        case 2329:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            break;
        case 2654:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeReplacement);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 1, 1, 0);
            break;
        case 2717:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            break;
        case 2850:
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_AW5), FacialNodeOriginal);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(SFSR_AW5), 0, 1, 0);
            break;
        default:
            break;
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        UnloadAuthorCharacters();
        loop.owner->state = 3;
        HLib::DestroyCurrentTask();
    }
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
    EVT_AT_SetPos(SFSR_BASE, STREET_FIGHT_INVALID, SFSR_NORMAL_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFSR_BASE, STREET_FIGHT_INVALID, 0, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFSR_BASE);
    AT_SetLocalBaseChara(SFSR_SF3, SFSR_BASE);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.fighters[work.fighterIndex], 2588, 0, -1, 0x40000000U, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, {0, 0, -1.100000023841858f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 180, 0);
    EVT_AT_SetPos(work.fighters[work.fighterIndex], SFSR_BASE, {0, 0, 1.100000023841858f}, 0);
    EVT_AT_SetAngY(work.fighters[work.fighterIndex], SFSR_BASE, 0, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(work.fighters[work.fighterIndex], CharacterTypeStencilShadow);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
    qteBattleUtil(BattleQteMode::Load, 0, 0, 0, 0);
    LoadBlurModelForCharacter(STREET_FIGHT_RYO, "RYO_B.MT7", 1);
    LoadBlurModelForCharacter(SFSR_SF3, "SF3_B.MT7", 1);
    for (std::uint32_t index = 0; SFSR_CHARACTERS[index].name != STREET_FIGHT_INVALID.name; ++index)
        SetLowQualityHands(FindCharacterByScenarioName(SFSR_CHARACTERS[index]));
}
void InitializeBattle(StreetFightEventWork &work) {
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QLBT"), 1);
    work.rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
    std::int32_t level = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{136}, 0, -1));
    if (level > 2)
        level = 2;
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 300, 0);
    const bool replay = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) != 0;
    const std::int32_t health = replay ? 80 * work.rank + 300 : 100 * (6 - level);
    shenInitFightData(FindCharacterByScenarioName(SFSR_SF3), nullptr, 1, 1, health, 25);
    vfUpdateFlagsFor(FindCharacterByScenarioName(SFSR_SF3), 0, 0x8000000, level);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        vfUpdateFlagsFor(FindCharacterByScenarioName(SFSR_SF3), 0, 0x20000, static_cast<std::int32_t>(static_cast<float>(100 * (6 - level)) * .1f));
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
        const std::int64_t count = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{72}, 0, -1);
        vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uint32_t>(count + 1), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1024}, 0);
    } else {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x100}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x4000}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
    }
    vfCopyPosFromEntity(0, nullptr, 230.89999389648438f, 93.5999984741211f, 3.200000047683716f);
}
void UpdateBattleQte() {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        return;
    switch (qtePhase) {
    case 0:
        if ((GetFightCharacterFlag(1, FindCharacterByScenarioName(STREET_FIGHT_RYO), 0) < 15 &&
                GetFightCharacterFlag(1, FindCharacterByScenarioName(SFSR_SF3), 0) < 50 && GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 1) ||
            GetFightCharacterFlag(1, FindCharacterByScenarioName(SFSR_SF3), 0) < 15) {
            if (!(GetFightTaskFlags() & 0x10) && (EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 13) & 1)) {
                qteBattleUtil(BattleQteMode::SetWait, 30, 7, 45, 0);
                qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 512, 0, 0);
                qteBattleUtil(BattleQteMode::ResetStates, 0, 0, 0, 0);
                qteBattleUtil(BattleQteMode::Queue, 0, 0, 0, 0);
                EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
                qtePhase = 1;
            }
        }
        break;
    case 1:
        vfUpdateFlagsFor(FindCharacterByScenarioName(SFSR_SF3), 1, 0x20000, 0);
        qteBattleUtil(BattleQteMode::Start, 0, 0, 0, 0);
        qtePhase = 2;
        break;
    case 2: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult, 0, 0, 0, 0);
        if (!result)
            break;
        if (result != 1) {
            vfmDispatchControl(FightControlOp::QueuedAction, SFSR_SF3, 0x8000a51, 1);
            qtePhase = 3;
        } else {
            EV_vfBattleUtil(FightBattleUtilOp::ForceActorDefeat, SFSR_SF3, 1);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
            check_dead_status(1);
            qtePhase = 4;
        }
        break;
    }
    case 3:
        qtePhase = 4;
        break;
    case 4:
        qteBattleUtil(BattleQteMode::RetireAndFree, 0, 0, 0, 0);
        ++qtePhase;
        break;
    default:
        break;
    }
}

void MainCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    switch (work.state) {
    case 0: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        PlaceNormal(work);
        SetMapControlStatusFlag(0, 0);
        EV_LoadAudio("0259A000", 1);
        spotlightTask = HLib::EnqueueTaskWithoutParameter(SpotlightCallback, 5, 0x0b, static_cast<std::int32_t>(SFSR_SPOTLIGHT_TASK.name));
        if (!work.audienceTask) {
            work.audienceTask = QueueStreetFightAudienceCount(30);
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->soundCentre = SFSR_NORMAL_STAGE_ORIGIN;
                audience->state = 2;
                audience->radius = 5;
                audience->radiusRange = 2;
                audience->state = 2;
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience->fighters[audience->fighterCount++] = SFSR_SF3;
                if (audience)
                    audience->mode |= 1;
            }
        }
        ++work.state;
        break;
    }
    case 1:
        InitializeBattle(work);
        SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFSR_SF3), 0, 0, 0);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work.startParameters->flags));
        work.round = work.wins = work.losses = work.roundResults[0] = work.roundResults[2] = 0;
        ++work.state;
        break;
    case 2:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            SetRankedLightLimit(0);
            EV_DisableCharacterDisplay(SFSR_CHN);
            EV_ToggleCharacterFlag(SFSR_CHN);
            EV_DisableCharacterDisplay(SFSR_SFB);
            EV_ToggleCharacterFlag(SFSR_SFB);
            EV_DisableCharacterDisplay(SFSR_AW5);
            EV_ToggleCharacterFlag(SFSR_AW5);
            EV_DisableCharacterDisplay(SFSR_LIGHT_CENTRE);
            EV_ToggleCharacterFlag(SFSR_LIGHT_CENTRE);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFSR_SF3), FacialNodeOriginal);
            work.state = 15;
        } else if (work.startParameters->scene == 99) {
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            work.state = 21;
        } else
            ++work.state;
        break;
    case 3:
        SetRankedLightLimit(0);
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        QueueAuthoredPluralControl(SFSR_PLURAL_INTRO);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1793))
            EV_authSceneCtrl(AuthSceneCtrl::UnusedAuthorFlag);
        if (IsAuthorStateFlagSet(16))
            EV_SetAuthorActiveTo(1);
        QueueAuthor(SFSR_AUTH_INTRO);
        work.state = 9;
        break;
    case 4:
    case 9:
    case 12:
    case 22:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work.state;
        break;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work.state;
        else if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1550 || EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 2250)
            spotlightTargets[0] = shen_name{};
        break;
    case 6:
        EV_SetCharacterAlphaEnabled(SFSR_AW5, 0);
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        ControlChoice(0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.state = 7;
        break;
    case 7: {
        const std::uint16_t choice = static_cast<std::uint16_t>(ControlChoice(1));
        if (choice) {
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            work.state = choice == 38 ? 8 : 11;
        }
        break;
    }
    case 8:
        QueueAuthor(SFSR_AUTH_ACCEPT);
        ++work.state;
        break;
    case 10:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 15;
        else
            switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
            case 2084:
                spotlightTargets[0] = SFSR_AW5;
                break;
            case 2670:
                spotlightTargets[0] = SFSR_SF3;
                break;
            case 3600:
                spotlightTargets[0] = STREET_FIGHT_RYO;
                break;
            default:
                break;
            }
        break;
    case 11:
        QueueAuthor(SFSR_AUTH_DECLINE);
        ++work.state;
        break;
    case 13:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            work.state = 30;
        }
        break;
    case 14:
        work.state = 30;
        break;
    case 15: {
        if (filmCheckFadeTrackColour())
            break;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            if (static_cast<std::uint8_t>(work.round) < 3) {
                HLib::Task *const task =
                    HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
                SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
                message->frames = 60;
                message->message = SFSR_ROUND_MESSAGES[static_cast<std::uint8_t>(work.round)];
            }
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
        }
        SetupSpotlights();
        spotlightTargets[0] = STREET_FIGHT_RYO;
        spotlightTargets[1] = SFSR_SF3;
        SetNewNpcBatchCountOverride(NpcFillType::BackFill, 8);
        EV_LoadAudio("0259A000", 2);
        EV_spmBeginSampleExecute();
        EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(SFSR_SF3, 2588, 0, -1, 0x40000000U, 0);
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, {0, 0, -1.100000023841858f}, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 180, 0);
        EVT_AT_SetPos(fighter, SFSR_BASE, {0, 0, 1.100000023841858f}, 0);
        EVT_AT_SetAngY(fighter, SFSR_BASE, 0, 0);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SFSR_SF3), FacialExpressionId::Angry, 1, 1);
        camCtrlSetState(Init);
        camCtrlSetTargetPosition(SFSR_CAMERA_TARGET);
        camCtrlSetNextPosition(SFSR_CAMERA_POSITION);
        EV_SetRefBoardParamsFor(STREET_FIGHT_RYO, 0);
        EV_SetRefBoardParamsFor(SFSR_SF3, 0);
        work.fadeFrames = 30;
        EventFilmControl reveal{.cleanup = 0,
            .frames = 30,
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
        EVT_AT_SetCam(1400, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
        SetAudienceState(work, 4, 0);
        EV_DisableCharacterDisplay(SFSR_AW5);
        EV_ToggleCharacterFlag(SFSR_AW5);
        EV_DisableCharacterDisplay(SFSR_LIGHT_LEFT);
        EV_ToggleCharacterFlag(SFSR_LIGHT_LEFT);
        EV_DisableCharacterDisplay(SFSR_LIGHT_RIGHT);
        EV_ToggleCharacterFlag(SFSR_LIGHT_RIGHT);
        EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
        EV_SetCharacterAlphaEnabled(SFSR_SF3, 0);
        ++work.state;
        break;
    }
    case 16:
        if (!camCtrlCheckIsEventCam()) {
            EVT_AT_SetCam(1401, SFSR_BASE, SFSR_BASE, 0, 0, 0);
            EV_PlayDialogue("0259F001");
            SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1);
            SetPluralControlFor(FindCharacterByScenarioName(SFSR_SF3), 1);
            EV_EnableCharacterDisplay(SFSR_LIGHT_LEFT);
            EV_EnsureCharacterDraw(SFSR_LIGHT_LEFT);
            EV_EnableCharacterDisplay(SFSR_LIGHT_RIGHT);
            EV_EnsureCharacterDraw(SFSR_LIGHT_RIGHT);
            ++work.state;
        }
        break;
    case 17:
        if (!camCtrlCheckIsEventCam()) {
            UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
            UnparentAndSetExistingPosRot(fighter);
            EV_PlayDialogue("0259F002");
            HLib::SetupTaskDelay(10);
            ++work.state;
        }
        break;
    case 18: {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            vfmDispatchControl(FightControlOp::RoundCounts, shen_name{static_cast<std::uint32_t>(work.wins)}, static_cast<std::uintptr_t>(work.losses), 1);
        qtePhase = 0;
        work.audienceResultPublished = 0;
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
        std::uint8_t flags = 0;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            ringOutDelay = 60;
            flags = 8;
            ringOutPhase = 0;
        }
        vfmQueueFightStart(2, 0x63, 1, flags, -1);
        EV_SetCharacterAlphaEnabled(SFSR_SF3, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1793, 1);
        SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
        EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
        EV_SetCharacterAlphaEnabled(SFSR_SF3, 0);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            const std::int32_t index = work.rank <= 1 ? 0 :
                work.rank <= 3                        ? 1 :
                work.rank <= 6                        ? 2 :
                work.rank <= 9                        ? 3 :
                work.rank <= 12                       ? 4 :
                work.rank <= 15                       ? 5 :
                                                        0;
            SetFightActorParameterBytes(SFSR_SF3, SFSR_FIGHT_PARAMETERS[index]);
        }
        work.state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 20 : 19;
        break;
    }
    case 19:
        UpdateBattleQte();
        if (IsFightActive())
            break;
        if (GetBtlStatus(FindCharacterByScenarioName(SFSR_SF3)) == 10) {
            RecordStreetFightVictory(work.stage);
            work.battleStatus = 3;
        } else {
            work.battleStatus = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
            if (work.battleStatus != 3) {
                RecordStreetFightDefeat(work.stage);
                work.state = 31;
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                spmFadeOut();
                HLib::SetupTaskDelay(30);
                break;
            }
        }
        work.state = 21;
        break;
    case 20:
        if (IsFightActive()) {
            if (!work.audienceResultPublished) {
                const std::int32_t status = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
                if (status == 3) {
                    SetAudienceState(work, 6, 2);
                    work.battleStatus = 3;
                    ++work.audienceResultPublished;
                } else if (status == 2) {
                    SetAudienceState(work, 7, 1);
                    work.battleStatus = 2;
                    ++work.audienceResultPublished;
                }
            }
            if (ringOutPhase) {
                if (ringOutPhase == 1) {
                    if (ringOutDelay)
                        --ringOutDelay;
                    else {
                        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                        vfmDispatchControl(FightControlOp::SpriteDepth, {}, 0, 1);
                        ++ringOutPhase;
                    }
                } else if (ringOutPhase == 2 && !filmCheckFadeTrackColour()) {
                    check_dead_status(1);
                    ++ringOutPhase;
                }
            } else {
                const std::int32_t playerFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 11));
                const std::int32_t fighterFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFSR_SF3, 11));
                if (fighterFlags != -1 && (fighterFlags & 0x80)) {
                    Vector3 position;
                    EV_GetCharaPosition(SFSR_SF3, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
                    Execute3DSoundCommand(0x7f7809daU, position, 2, 30);
                    SetAudienceState(work, 6, 11);
                    EV_SetCharacterAlphaEnabled(SFSR_SF3, 0);
                    ++ringOutPhase;
                    work.battleStatus = 3;
                } else if (playerFlags != -1 && (playerFlags & 0x80)) {
                    EV_PlayDialogue("0100A080");
                    SetAudienceState(work, 7, 10);
                    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
                    ++ringOutPhase;
                    work.battleStatus = 2;
                }
            }
        } else {
            const std::int64_t result = RecordStreetFightRound(&work);
            if (result) {
                if (ringOutDelay == 60)
                    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                spmFadeOut();
                HLib::SetupTaskDelay(30);
                work.state = result == 1 ? (ringOutDelay != 60 ? 33 : 30) : 31;
            } else {
                InitializeBattle(work);
                work.state = 15;
            }
        }
        break;
    case 21:
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SFSR_SF3), FacialExpressionId::Angry, 1, 1);
        EV_EnableCharacterDisplay(SFSR_AW5);
        EV_EnsureCharacterDraw(SFSR_AW5);
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        QueueAuthoredPluralControl(SFSR_PLURAL_RESULT);
        QueueAuthor(SFSR_AUTH_RESULT);
        ++work.state;
        break;
    case 23:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work.state;
        break;
    case 24:
        if (!filmCheckFadeTrackColour()) {
            EV_ResetFreeTalkFlags();
            EV_NPC_NewLoadSw(0);
            if (EV_GetShenmueFlag(0x128))
                AddCashOrCoinsWithStep(3000, 2, 100);
            AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
            AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
            ++work.state;
        }
        break;
    case 25:
        if (GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            break;
        if (CanPerformForCurrentAreaFlags())
            CommitStreetFightStoryRank(work.stage);
        work.state = 30;
        break;
    case 30:
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? RetryBeginCallback : CleanupNormalCallback);
        break;
    case 31:
        if (!filmCheckFadeTrackColour()) {
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SFSR_SF3), FacialExpressionId::Angry, 1, 1);
            if (IsAuthorStateFlagSet(16)) {
                sfsrConversationSkipEnabled = 1;
                sfsrConversationSkipCallback = nullptr;
            }
            SetAuthorStateFlag(16, 1);
            QueueSfsrStreetFightConversation(DefeatConversation, &work);
            work.state = 32;
        }
        break;
    case 32:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        EVT_AT_ObjectDisp_1(STREET_FIGHT_RYO, 31, 0);
        work.state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 30 : 34;
        HLib::SetupTaskDelay(40);
        break;
    case 33:
        if (!filmCheckFadeTrackColour()) {
            QueueSfsrStreetFightConversation(RoundVictoryConversation, &work);
            work.state = 30;
        }
        break;
    case 34:
        EV_ResetFreeTalkFlags();
        Queue_ST_LoadTitleFadeIn(SFSR_RETRY_TITLE);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            if (audience)
                audience->mode |= 2;
        }
        work.state = 35;
        work.frameCounter = 60;
        break;
    case 35:
        if (work.frameCounter-- == 1) {
            ST_LoadTitleFade();
            work.frameCounter = 60;
            work.state = 36;
        }
        break;
    case 36:
        if (work.frameCounter-- == 1)
            work.state = 30;
        break;
    default:
        break;
    }
}

void SetupSpotlights() {
    AssignLightProgram(0);
    SetDefaultVSByIndex(1);
    SetLightEntryIndex(21, 0);
    SetLightEntryIndex(22, 0);
    ResetLightRuntimeEntry(13);
    ResetLightRuntimeEntry(14);
    SetLightFlagForIndex(13, 4);
    SetLightFlagForIndex(14, 4);
    SetLightRuntimeColour(13, SFSR_LEFT_LIGHT_COLOUR);
    SetLightRuntimeColour(14, SFSR_RIGHT_LIGHT_COLOUR);
    SetLightSceneIntensity(13, 1);
    SetLightSceneIntensity(14, 1);
    SetLightRange(13, 12);
    SetLightRange(14, 12);
    const float leftFalloff[]{1.5f, 8.0f}, rightFalloff[]{1.0f, 8.0f};
    SetLightFalloff(13, leftFalloff);
    SetLightFalloff(14, rightFalloff);
    SetLightEntryIndex(13, 1);
    SetLightEntryIndex(14, 1);
    spotlightPositions[0] = SFSR_SPOTLIGHT_ORIGIN;
    spotlightPositions[1] = SFSR_SPOTLIGHT_ORIGIN;
    streetFightSpotlightPositioned = 0;
    spotlightTargets[0] = shen_name{};
    spotlightTargets[1] = shen_name{};
}
void SpotlightCallback(void *) {
    if (!DoesScenarioCharacterExist(SFSR_LIGHT_LEFT))
        return;
    Vector3 targets[2];
    for (std::uint32_t index = 0; index < 2; ++index) {
        const shen_name target = spotlightTargets[index];
        if (target.name == STREET_FIGHT_INVALID.name)
            EV_GetCharaPosition(SFSR_SF3, 0, &targets[index], EventTransformFlags::World);
        else if (DoesScenarioCharacterExist(target))
            EV_GetCharaPosition(target, 0, &targets[index], EventTransformFlags::World);
        else
            targets[index] = SFSR_SPOTLIGHT_ORIGIN;
    }
    const shen_name lights[]{SFSR_LIGHT_LEFT, SFSR_LIGHT_RIGHT};
    for (std::uint32_t index = 0; index < 2; ++index) {
        Vector3 &smooth = spotlightPositions[index];
        smooth.x += (targets[index].x - smooth.x) * .5f;
        smooth.z += (targets[index].z - smooth.z) * .5f;
        Vector3 position;
        EV_GetCharaPosition(lights[index], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        Vector3 direction{smooth.x - position.x, smooth.y - position.y, smooth.z - position.z};
        const Vector3i rotation{InverseFixedAtan2(direction.y, std::sqrt(direction.z * direction.z + direction.x * direction.x)) - 0x4000,
            InverseFixedAtan2(direction.x, direction.z), 0};
        EV_AT_SetAngY(lights[index], EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        const float scale = (134.6300048828125f - position.y) / direction.y;
        position.x += scale * direction.x;
        position.y += scale * direction.y;
        position.z += scale * direction.z;
        SetLightRuntimePosition(index + 13, position);
        direction.x = -direction.x;
        direction.y = -direction.y;
        direction.z = -direction.z;
        SetLightSunDirection(index + 13, direction);
    }
    if (!streetFightSpotlightPositioned) {
        SetScenarioCharacterRadius(SFSR_LIGHT_LEFT, 100);
        SetScenarioCharacterRadius(SFSR_LIGHT_CENTRE, 100);
        SetScenarioCharacterRadius(SFSR_LIGHT_RIGHT, 100);
        streetFightSpotlightPositioned = 1;
    }
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_DisableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharacterAlphaEnabled(fighter, 0);
    spotlightTargets[0] = SFSR_SF3;
    spotlightTargets[1] = SFSR_SF3;
    EVT_AT_SetPos(fighter, SFSR_BASE, {0, -.05000000074505806f, 0}, 0);
    EVT_AT_SetAngY(fighter, SFSR_BASE, 90, 0);
    EVT_AT_SetMot(fighter, 10053, 0, 1, 0x40000000U, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    const char *const voices[]{"0259E010", "0259E011", "0259E012"}; // @text 0259E010 EN: " " | JA: "フッ！" ; 0259E011 EN: " " | JA: "ハッ！" ; 0259E012 EN: " " | JA: "ハァッ！"
    EVT_AT_SetVoice(voices[shenGetRandomInt32(3)], 1, 86);
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
    EVT_AT_SetCam(8099, SFSR_BASE, SFSR_BASE, 0, 0, 1);
    return 130;
}
std::int32_t RoundVictoryConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_DisableCharacterDisplay(fighter);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    spotlightTargets[0] = STREET_FIGHT_RYO;
    spotlightTargets[1] = STREET_FIGHT_RYO;
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFSR_BASE, {0, 0, 0}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFSR_BASE, 90, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 395, 0, 1, 0x40000000U, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1, 0, 0);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
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
    EVT_AT_SetCam(8100, SFSR_BASE, SFSR_BASE, 0, 0, 1);
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
        .delay = 120});
    EVT_AT_ObjectDisp_1(fighter, 155, 0);
    return 160;
}

void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
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
    const std::uint32_t choice = ControlChoice(1);
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82) {
        EV_ResetFreeTalkFlags();
        work.state = 1;
        work.battleStatus = 0;
        work.auxiliaryState = 0;
        HLib::SetCurrentTaskTo(MainCallback);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            if (audience)
                audience->mode &= ~2U;
        }
        EV_LoadAudio("0259A000", 1);
        EV_spmBeginSampleExecute();
    } else
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
}
void CleanupNormalCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    const std::int32_t status = work.battleStatus;
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    for (std::uint32_t index = 0; SFSR_CHARACTERS[index].name != STREET_FIGHT_INVALID.name; ++index)
        hndlSetFlags(FindCharacterByScenarioName(SFSR_CHARACTERS[index]));
    if (spotlightTask)
        HLib::DestroyTaskInternal(spotlightTask);
    if (work.audienceTask) {
        (void)HLib::GetTaskParameter(work.audienceTask);
        HLib::DestroyTaskInternal(work.audienceTask);
    }
    work.audienceTask = nullptr;
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFSR_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, status == 3 ? SFSR_WIN_EXIT_POSITION : SFSR_LOSS_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, status == 3 ? SFSR_WIN_EXIT_ROTATION : SFSR_LOSS_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    SetNewNpcBatchCountOverride(NpcFillType::BackFill, 5);
    for (std::int32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(static_cast<std::uint32_t>(*motion));
    EV_FreeAuthorPak(0, "0259");
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_FreeAuthorPak(0, "0100");
    if (!(start->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    ExchangeLegacyModelColour(&savedModelColour, nullptr);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_NPC_NewLoadSw(1);
    HLib::SetCurrentTaskTo(CleanupWaitCallback);
}
void CleanupWaitCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    const std::int32_t status = work.battleStatus, returnToArea = work.reserved178;
    const std::uint32_t entry = start->reserved08;
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFSR_RETRY_AREA, 1);
    else if (returnToArea)
        EV_scenQueueLoad(g_actNumber, SFSR_RETURN_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    if (status == 3 || entry == 99)
        ownerEntry->reserved18 = 99;
    else
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(40);
    work->fighterIndex = 0;
    work->fighters[0] = SFSR_SF3;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFSR_CHARACTERS);
    work->audienceTask = nullptr;
    work->fadeFrames = 0;
    work->attempts = 0;
    work->reserved178 = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->frameCounter = 60;
    SetNewNpcBatchCountOverride(NpcFillType::BackFill, 8);
    return work;
}
void LoadNormalResources(StreetFightEventWork &work) {
    if (IsFightActive())
        check_dead_status(1);
    work.motions[0] = 0;
    RepositionPlayerForQronFloor(8);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_FreeAuthorPak(1, "0100");
    EV_FreeAuthorPak(1, "0259");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QLBT"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QLBT"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QLBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFSR_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0259A000");
    EV_spmBeginSampleExecute();
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
    const Vector4 colour{};
    ExchangeLegacyModelColour(&colour, &savedModelColour);
    SetupSpotlights();
    spotlightTargets[0] = SFSR_SFB;
    spotlightTargets[1] = SFSR_SF3;
}
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
    LoadNormalResources(*work);
}
void Init() {
    ringOutDelay = 60;
    savedModelColour = {};
    qtePhase = 0;
    ownerEntry = nullptr;
    spotlightTask = nullptr;
    ringOutPhase = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
    ownerEntry = start;
}
} // namespace
const ScriptAreaDefinition areaSfsrScriptDefinition{.areaId = STREET_FIGHT_SFSR, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
