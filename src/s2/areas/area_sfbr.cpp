#include "s2/areas/area_sfbr.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/ui.hpp"
#include "s2/advice.hpp"
namespace shenmue::s2 {
namespace {
std::int32_t bearBattleVoiceIndex{};
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
void BattleEntryCallback(void *);
void BattlePreEventCallback(void *);
void BattleStartCallback(void *);
void BattleWaitCallback(void *);
void BattleWaitFadeCallback(void *);
void BattleEndCallback(void *);
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void RetryResetCallback(void *);
std::int32_t FirstIntroConversation(void *);
std::int32_t RepeatIntroConversation(void *);
std::int32_t AcceptConversation(void *);
std::int32_t DeclineConversation(void *);
std::int32_t InsufficientConversation(void *);
std::int32_t VictoryConversation(void *);
std::int32_t DefeatConversation(void *);
std::int32_t ExpiredConversation(void *);

void QueueHighModel(shen_name character, std::uint32_t selector, std::uint32_t state, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightHighModelCallback, 6, 0x0b, sizeof(StreetFightHighModelWork), STREET_FIGHT_EVENT_TASK);
    StreetFightHighModelWork *const work = static_cast<StreetFightHighModelWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->character = character;
    work->selector = selector;
    work->state = state;
    HLib::SetupTimedTask(task, delay);
}

StreetFightEventWork *InitializeDebug(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = STREET_FIGHT_INVALID;
    work->package = STREET_FIGHT_AB00;
    work->battleFrames = 50;
    work->motions[0] = 0;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->audienceTask = nullptr;
    work->completedOpponents = 0;
    work->hudEnabled = 0;
    work->fighterIndex = 0;
    const StreetFightFighterDefinition *const definition = GetStreetFightDebugFighterDefinition();
    streetFightMaximumOpponents = 5;
    work->fighterDefinitions[0] = definition;
    work->fighters[0] = definition->character;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFBR_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ABSF"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFBR_BASE);
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

void SetupDebugActors(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(STREET_FIGHT_BOOKIE);
    EV_EnsureCharacterDraw(STREET_FIGHT_BOOKIE);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = SFBR_BOOKIE_POSITION;
    EVT_AT_SetPos(SFBR_BASE, STREET_FIGHT_INVALID, SFBR_BASE_POSITION, 0);
    EVT_AT_SetAngY(SFBR_BASE, STREET_FIGHT_INVALID, 90, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFBR_BASE);
    AT_SetLocalBaseChara(STREET_FIGHT_BOOKIE, SFBR_BASE);
    AT_SetLocalBaseChara(fighter, SFBR_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, SFBR_ARRIVAL_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 225, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, SFBR_BASE, 0, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, {1.5f, 0, -1}, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(STREET_FIGHT_BOOKIE, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFBR_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFBR_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFBR_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::uint32_t index = 0; work->motions[index]; ++index)
        DeleteMotion(work->motions[index]);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ENMY"), 0);
    LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(1);
    if (parameters->signal)
        *parameters->signal = 0;
}

void DebugMainCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    const StreetFightFighterDefinition *const definition = work->fighterDefinitions[work->fighterIndex];
    switch (work->state) {
    case 0:
        streetFightLoadCharacters[0] = fighter;
        streetFightLoadCharacters[1] = STREET_FIGHT_INVALID;
        QueueStreetFightCharacterLoad(work->task);
        work->state = 1;
        break;
    case 1: {
        SetupDebugActors(work);
        work->audienceTask = QueueStreetFightAudienceCount(30);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience->fighters[audience->fighterCount++] = fighter;
        }
        Vector3 base;
        EV_GetCharaPosition(SFBR_BASE, CHARACTER_TRANSLATION_ROOT, &base, EventTransformFlags::Local);
        base.y -= 0.5f;
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->soundCentre = base;
            audience->state = 2;
        }
        Vector3 player, bookie, playerWorld, bookieWorld;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::Local);
        EV_GetCharaPosition(STREET_FIGHT_BOOKIE, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work->playerPosition);
        EV_AT_SetPos(STREET_FIGHT_BOOKIE, EventTransformFlags::AllAxes, work->bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerWorld, EventTransformFlags::World);
        EV_GetCharaPosition(STREET_FIGHT_BOOKIE, CHARACTER_TRANSLATION_ROOT, &bookieWorld, EventTransformFlags::World);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->corridorStart.x = playerWorld.x;
            audience->corridorStart.z = playerWorld.z;
            audience->corridorEnd.x = bookieWorld.x;
            audience->corridorEnd.z = bookieWorld.z;
            audience->corridorLength = GetPlanarDistance(&playerWorld, &bookieWorld);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, player);
        EV_AT_SetPos(STREET_FIGHT_BOOKIE, EventTransformFlags::AllAxes, bookie);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->radius = 4.75f;
            audience->radiusRange = 2;
            audience->state = 2;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.0f);
        work->state = 2;
        break;
    }
    case 2:
        EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, {SFBR_DEBUG_STAGE_POSITION.x - 1, SFBR_DEBUG_STAGE_POSITION.y + 0.5f, SFBR_DEBUG_STAGE_POSITION.z}, 0);
        work->state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work->audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask))->state > 2) {
            QueueSfbrStreetFightConversation(StartStreetFightBookieConversation, work);
            work->state = 4;
            break;
        }
        [[fallthrough]];
    case 4:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        ControlStreetFightActionChoice(0);
        work->state = 5;
        break;
    case 5: {
        ReadStreetFightBookiePosition();
        const std::uint16_t choice = static_cast<std::uint16_t>(ControlStreetFightActionChoice(1));
        if (choice) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), 0, 0);
            work->state = choice == 38 ? (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 20 ? 8 : 6) : 7;
        }
        break;
    }
    case 6:
        QueueSfbrStreetFightConversation(AcceptStreetFightConversation, work);
        work->state = 9;
        break;
    case 7:
        QueueSfbrStreetFightConversation(DeclineStreetFightConversation, work);
        work->state = 23;
        break;
    case 8:
        QueueSfbrStreetFightConversation(InsufficientStreetFightFundsConversation, work);
        work->state = 23;
        break;
    case 9:
        HLib::SetCurrentTaskTo(DebugBattleEntryCallback);
        work->state = 12;
        break;
    case 12:
        work->state = work->battleStatus == 3 ? 13 : 17;
        break;
    case 13:
        work->completedOpponents = static_cast<std::uint8_t>(work->completedOpponents + 1);
        AddStreetFightProgress(SFBR_PROGRESS_WEIGHTS[work->completedOpponents]);
        QueueSfbrStreetFightConversation(StreetFightVictoryConversation, work);
        work->state = 14;
        break;
    case 14: {
        nullsub();
        const StreetFightFighterDefinition *const next = GetStreetFightDebugFighterDefinition();
        work->fighterDefinitions[0] = next;
        work->fighters[0] = next->character;
        EV_NPC_NewLoadSw(0);
        if (fighter.name != STREET_FIGHT_INVALID.name) {
            if (work->audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                audience->fighters[0] = {};
                audience->fighters[1] = {};
                audience->fighters[2] = {};
                audience->fighters[3] = {};
                audience->fighterCount = 0;
            }
            ReleaseStreetFightCharacter(fighter);
        }
        if (next->character.name != STREET_FIGHT_INVALID.name) {
            streetFightLoadCharacters[0] = next->character;
            streetFightLoadCharacters[1] = STREET_FIGHT_INVALID;
            QueueStreetFightCharacterLoad(work->task);
        }
        work->state = 15;
        break;
    }
    case 15:
        EV_NPC_NewLoadSw(1);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience->fighters[audience->fighterCount++] = fighter;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.0f);
        QueueSfbrStreetFightConversation(StreetFightNextOpponentConversation, work);
        work->state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        ControlStreetFightActionChoice(0);
        work->state = 19;
        break;
    case 17:
        QueueSfbrStreetFightConversation(StreetFightDefeatConversation, work);
        work->state = 23;
        break;
    case 18:
        QueueSfbrStreetFightConversation(StreetFightWithdrawalConversation, work);
        work->state = 23;
        break;
    case 19: {
        ReadStreetFightBookiePosition();
        const std::uint16_t choice = static_cast<std::uint16_t>(ControlStreetFightActionChoice(1));
        if (choice) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), 0, 0);
            work->state = choice == 38 ? 20 : 21;
        }
        break;
    }
    case 20:
        QueueSfbrStreetFightConversation(StreetFightContinueConversation, work);
        work->state = 22;
        break;
    case 21:
        QueueSfbrStreetFightConversation(StreetFightStopConversation, work);
        work->state = 23;
        break;
    case 22:
        if (work->completedOpponents < streetFightMaximumOpponents) {
            QueueSfbrStreetFightConversation(StreetFightNextMatchConversation, work);
            work->state = 9;
        } else {
            QueueSfbrStreetFightConversation(StreetFightCompletedConversation, work);
            work->state = 23;
        }
        break;
    case 23:
        work->state = 24;
        break;
    case 24:
        if (!filmCheckFadeTrackColour() && !GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            work->state = 25;
        break;
    case 25:
        if (work->audienceTask) {
            HLib::GetTaskParameter(work->audienceTask);
            HLib::DestroyTaskInternal(work->audienceTask);
        }
        CleanupDebug(work);
        ++work->state;
        break;
    default:
        break;
    }
}

void DebugBattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EventSetPosition position{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFBR_BASE, .position = {0, 0, -1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFBR_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = STREET_FIGHT_BOOKIE, .parent = SFBR_BASE, .position = {-3.3f, -0.5f, -5.5f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_BOOKIE, .parent = SFBR_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFBR_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFBR_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}

void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    const EventFilmControl film{.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 5};
    EVT_AT_MenCtrl(film);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1ae0, 0, 1, 0, 50);
    EV_PlayDialogue("0100B072");
    EventSetCamera camera{.cleanup = 0, .camera = 2001, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ABSF"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(FindCharacterByScenarioName(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFBR_DEBUG_STAGE_POSITION.x, SFBR_DEBUG_STAGE_POSITION.z, 2.5f);
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
        EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 0};
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
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFBR_BASE);
    AT_SetLocalBaseChara(fighter, SFBR_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_BOOKIE, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = SFBR_SFX;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->battleFrames = 96;
    work->motions[0] = 0;
    work->reserved138[0] = 0;
    work->audienceTask = nullptr;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFBR_CHARACTERS);
    work->frameCounter = 0;
    work->hudEnabled = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->previousFarClip = camCtrlSetFarDistance(40);
    return work;
}
void QueueNormal(StreetFightStartParameters *parameters) {
    if (parameters->signal)
        *parameters->signal = 1;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = parameters;
    EV_FreeAuthorPak(1, "0100");
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("ABSF"), 0)) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("ABSF"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFBR_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    SetCurrentGameFlag(4, 1);
}
void SetupNormalActors(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(static_cast<FightControlOp>(17), {}, 0, 1);
    const std::int32_t rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{64}, 0, -1));
    vfmDispatchControl(static_cast<FightControlOp>(16), {}, static_cast<std::uintptr_t>(rank + 1), 1);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("ABSF"), 1);
    shenInitFightData(FindCharacterByScenarioName(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(fighter), nullptr, 1, 1, static_cast<std::uint16_t>(11 * rank + 90), 14);
    ControlFightLoadSprites(0, 0);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = SFBR_BOOKIE_POSITION;
    EVT_AT_SetPos(SFBR_BASE, STREET_FIGHT_INVALID, SFBR_BASE_POSITION, 0);
    EVT_AT_SetAngY(SFBR_BASE, STREET_FIGHT_INVALID, 90, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFBR_BASE);
    AT_SetLocalBaseChara(SFBR_SD5, SFBR_BASE);
    AT_SetLocalBaseChara(fighter, SFBR_BASE);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, SFBR_ARRIVAL_POSITION, 0);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, {-0.5f, 0, -1}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 225, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFBR_SD5, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFBR_SD5), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 1);
    SetLowQualityHands(FindCharacterByScenarioName(SFBR_SD5));
    SetLowQualityHands(FindCharacterByScenarioName(fighter));
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    adfsSetCameraIdxForChara(SFBR_SD5, shen_name{SFBR_SD5.name | 0x200U});
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 1);
}
void MainCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    switch (work->state) {
    case 0:
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        if (!work->audienceTask) {
            work->audienceTask = QueueStreetFightAudienceCount(30);
            if (work->audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
                audience->fighters[audience->fighterCount++] = work->fighters[0];
                audience->mode |= 1;
            }
        }
        work->state = 1;
        break;
    case 1: {
        SetupNormalActors(work);
        Vector3 base, player, bookie, playerWorld, bookieWorld;
        EV_GetCharaPosition(SFBR_BASE, CHARACTER_TRANSLATION_ROOT, &base, EventTransformFlags::Local);
        base.y -= 0.5f;
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->soundCentre = base;
            audience->state = 2;
        }
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::Local);
        EV_GetCharaPosition(SFBR_SD5, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work->playerPosition);
        EV_AT_SetPos(SFBR_SD5, EventTransformFlags::AllAxes, work->bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &playerWorld, EventTransformFlags::World);
        EV_GetCharaPosition(SFBR_SD5, CHARACTER_TRANSLATION_ROOT, &bookieWorld, EventTransformFlags::World);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->corridorStart.x = playerWorld.x;
            audience->corridorStart.z = playerWorld.z;
            audience->corridorEnd.x = bookieWorld.x;
            audience->corridorEnd.z = bookieWorld.z;
            audience->corridorLength = GetPlanarDistance(&playerWorld, &bookieWorld);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, player);
        EV_AT_SetPos(SFBR_SD5, EventTransformFlags::AllAxes, bookie);
        EV_AT_SetPos(SFBR_MKZI, EventTransformFlags::AllAxes, SFBR_HIDDEN_MARKER_POSITION);
        EV_NPC_NewLoadSw(1);
        QueueStreetFightNpcWait(static_cast<std::uint8_t>(work->startParameters->flags));
        work->state = 2;
        break;
    }
    case 2:
        if (!work->audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask))->state > 2) {
            const std::int32_t repeat = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 716));
            sfbrConversationSkipCallback = nullptr;
            sfbrConversationSkipEnabled = repeat != 0;
            QueueSfbrStreetFightConversation(repeat ? RepeatIntroConversation : FirstIntroConversation, work);
            work->state = 3;
        }
        break;
    case 3:
        ResetScenarioCharacterAttachment(SFBR_MKZI);
        AT_SetLocalBaseChara(SFBR_MKZI, STREET_FIGHT_INVALID);
        EV_DisableCharacterDisplay(SFBR_MKZI);
        EV_ToggleCharacterFlag(SFBR_MKZI);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 716, 1);
        EVT_AT_KsIcon_Enqueue(-1, 22, 39, 470, 0x7fff, 0xffff, 0, 0);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work->paidEntry = 0;
        work->state = 4;
        break;
    case 4: {
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        const std::uint16_t choice = static_cast<std::uint16_t>(PollStreetFightActionChoice());
        if (choice) {
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
            if (choice == 22) {
                if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= SFBR_BETS[work->rank]) {
                    work->state = 6;
                    work->paidEntry = 1;
                } else
                    work->state = 8;
            } else if (choice == 470) {
                vfmDispatchControl(static_cast<FightControlOp>(21), {}, 0, 1);
                work->state = 5;
            } else
                work->state = 7;
        }
        break;
    }
    case 5:
        if (!vfmDispatchControl(static_cast<FightControlOp>(21), {}, 0, -1))
            work->state = 3;
        break;
    case 6:
        QueueSfbrStreetFightConversation(AcceptConversation, work);
        work->state = 11;
        break;
    case 7:
        QueueSfbrStreetFightConversation(DeclineConversation, work);
        work->state = 16;
        break;
    case 8:
        QueueSfbrStreetFightConversation(InsufficientConversation, work);
        work->state = 16;
        break;
    case 11:
        UpdateMotionWorkWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(work->fighters[0]), nullptr, 0);
        HLib::SetCurrentTaskTo(BattleEntryCallback);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 12;
        break;
    case 12:
        work->state = work->battleStatus == 3 ? 13 : 14;
        break;
    case 13:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueSfbrStreetFightConversation(VictoryConversation, work);
        work->state = 16;
        break;
    case 14:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueSfbrStreetFightConversation(DefeatConversation, work);
        work->state = 18;
        break;
    case 15:
        QueueSfbrStreetFightConversation(ExpiredConversation, work);
        work->state = 16;
        break;
    case 16:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->state = 17;
        break;
    case 17:
        if (!filmCheckFadeTrackColour()) {
            if (work->battleStatus == 3)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 717, 1);
            HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work->paidEntry ? RetryBeginCallback : CleanupNormalCallback);
        }
        break;
    case 18:
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && work->paidEntry &&
            !vfmDispatchControl(FightControlOp::MotionNibble, shen_name{64}, 0, -1) &&
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{128}, 0, -1) == 2)
            QueueFightAdviceControl(11);
        work->state = 16;
        break;
    default:
        break;
    }
}
void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const parameters = work->startParameters;
    hndlSetFlags(FindCharacterByScenarioName(SFBR_SD5));
    hndlSetFlags(FindCharacterByScenarioName(work->fighters[work->fighterIndex]));
    AT_SetLocalBaseChara(SFBR_MKZI, STREET_FIGHT_INVALID);
    if (work->audienceTask) {
        HLib::GetTaskParameter(work->audienceTask);
        HLib::DestroyTaskInternal(work->audienceTask);
    }
    work->audienceTask = nullptr;
    ET_AT_KillWork(work->characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFBR_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFBR_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFBR_EXIT_ROTATION);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    camCtrlSetFarDistance(work->previousFarClip);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ABSF"), 0);
    SetPluralControlFor(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFBR_OMAK, 1);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(1);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (parameters->signal)
        *parameters->signal = 0;
}
void RetryBeginCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 716, 1);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI_Internal(choices, 0x7fff, -1, 2, 0);
    if (work->audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
        if (audience)
            audience->mode |= 2;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}
void RetryChoiceCallback(void *parameter) {
    const std::uint16_t choice = static_cast<std::uint16_t>(PollStreetFightActionChoice());
    if (choice) {
        ctrlStopSubtitles();
        if (choice == 82)
            RetryResetCallback(parameter);
        else
            HLib::SetCurrentTaskTo(CleanupNormalCallback);
    }
}
void RetryResetCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    EV_ResetFreeTalkFlags();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->state = 1;
    work->battleStatus = 0;
    work->auxiliaryState = 0;
    HLib::SetCurrentTaskTo(MainCallback);
    if (work->audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
}

void BattleEntryCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_DisableCharacterDisplay(SFBR_SD5);
    EV_ToggleCharacterFlag(SFBR_SD5);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 180, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, {0, 0, 1.1f}, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 0, 0);
    EventSetCamera camera{.cleanup = 0, .camera = 1400, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    SetCurrentGameFlag(4, 0);
    HLib::SetCurrentTaskTo(BattlePreEventCallback);
    HLib::SetupTaskDelay(30);
}
void BattlePreEventCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetMot(STREET_FIGHT_RYO, 13, 0, 0.5f, 0x20000000U, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0, 0);
    EVT_AT_SetVoice("0100W019", 1, 0); // @text EN: "You can start hitting him anywhere!" | JA: "さあ、どっからでも\n殴っていいぜ！"
    EVT_AT_SetVoice("0100W020", 1, 120); // @text EN: "Ready=@go!" | JA: "レディ・ゴー！"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::SetCurrentTaskTo(BattleStartCallback);
    HLib::SetupTaskDelay(180);
}
void BattleStartCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_LoadAudio("0100E000", 1);
    SetMotionRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 69, 2, MotionRegion::LowerBody);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFBR_DEBUG_STAGE_POSITION.x, SFBR_DEBUG_STAGE_POSITION.z, 2.75f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(2, 5, 1, 10, 180);
    EV_SetCharacterAlphaEnabled(fighter, false);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1784, 1);
    SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work->stage)), 1);
    work->hudEnabled = 0;
    bearBattleVoiceIndex = 0;
    vfmDispatchControl(static_cast<FightControlOp>(3), {}, 50, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(BattleWaitCallback);
}
void BattleWaitCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    if (IsFightActive()) {
        work->frameCounter = static_cast<std::int32_t>(vfmDispatchControl(static_cast<FightControlOp>(18), {}, 0, -1));
        if (EV_vfBattleUtil(FightBattleUtilOp::ActorField, fighter, 0) & 0x20000U) {
            if (!bearBattleVoiceIndex) {
                QueueBattleDialogue(10, 199);
                ++bearBattleVoiceIndex;
            } else if (bearBattleVoiceIndex == 1) {
                QueueBattleDialogue(10, 200);
                ++bearBattleVoiceIndex;
            } else if (bearBattleVoiceIndex == 2) {
                QueueBattleDialogue(10, 201);
                bearBattleVoiceIndex = 0;
            }
        }
        if (!work->audienceResultPublished) {
            const std::int32_t status = GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO));
            if (status == 2 || status == 3) {
                if (work->audienceTask) {
                    StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                    if (audience) {
                        audience->state = status == 2 ? 7 : 6;
                        audience->loopDelay = status == 2 ? 0 : 2;
                    }
                }
                ++work->audienceResultPublished;
            }
        }
        return;
    }
    if (GetBtlStatus(FindCharacterByScenarioName(STREET_FIGHT_RYO)) == 3)
        work->battleStatus = 3;
    else if (!work->frameCounter) {
        work->battleStatus = 2;
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), Normal, 0, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), Provo1R, 30, 3);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
        EventSetCamera camera{.cleanup = 0, .camera = 1162, .eye = SFBR_SFX, .target = SFBR_SFX, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetVoice("0100J002", 1, 10);
        EV_DisableCharacterDisplay(STREET_FIGHT_RYO);
        EV_ToggleCharacterFlag(STREET_FIGHT_RYO);
        HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
        HLib::SetupTaskDelay(60);
        return;
    } else
        work->battleStatus = 7;
    HLib::SetCurrentTaskTo(BattleWaitFadeCallback);
}
void BattleWaitFadeCallback(void *) {
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    HLib::SetCurrentTaskTo(BattleEndCallback);
    HLib::SetupTaskDelay(30);
}
void BattleEndCallback(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    EV_EnsureCharacterDraw(STREET_FIGHT_RYO);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(STREET_FIGHT_RYO), Angry, 1, 1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), Angry, 1, 1);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFBR_BASE);
    AT_SetLocalBaseChara(fighter, SFBR_BASE);
    EV_EnableCharacterDisplay(SFBR_SD5);
    EV_EnsureCharacterDraw(SFBR_SD5);
    SetScenarioCharacterTransparency(SFBR_SD5, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(fighter), Normal, 0, 1);
    EV_LoadAudio("0100E000", 0);
    EV_spmBeginSampleExecute();
    if (work->battleStatus == 3)
        CommitStreetFightStoryRank(work->stage);
    else
        ResetStreetFightStoryFlag(work->stage);
    HLib::SetCurrentTaskTo(MainCallback);
}

std::int32_t FirstIntroConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    BeginSecondarySampleExecution(0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetMot(SFBR_SD5, 6857, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, {-0.5f, 0, 0.5f}, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x10000000U, 0);
    EVT_AT_SetMot(fighter, 6851, 0, 1, 0, 220);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x10000000U, 438);
    EVT_AT_SetMot(fighter, 6854, 0, 1, 0x40000000U, 540);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x50000000U, 780);
    EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 2});
    EVT_AT_SetVoice("0100W001", 1, 32); // @text EN: "Yan Ren was found in the mountains of\nYunnan. He's the last wild man on earth!" | JA: "この厳仁は雲南省の山奥で\n発見された今世紀最後の野人だ！"
    EVT_AT_SetVoice("0100J001", 1, 230);
    EVT_AT_SetVoice("0100W002", 1, 320); // @text EN: "He can't speak, but living in the wild\nhas given him the toughest body ever!" | JA: "言葉は話せねえが、大自然に\nきたえられた肉体はまさに驚異！"
    EVT_AT_SetVoice("0100W003", 1, 510); // @text EN: "He's hard as a rock.\nHe can't feel pain!" | JA: "岩よりも硬く、\n痛みなんて感じやしねえ！"
    EVT_AT_SetVoice("0100J002", 1, 620);
    EVT_AT_SetVoice("0100W004", 1, 670); // @text EN: "Anyone want to challenge this man?" | JA: "さあ、誰かこの男と\n勝負するヤツはいねえか？"
    EVT_AT_SetVoice("0100W005", 1, 800); // @text EN: "He won't fight back.\nYou can hit him as hard as you want." | JA: "と言っても闘うわけじゃねえ。\n一方的に殴っていいんだ。"
    EVT_AT_SetVoice("0100W006", 1, 970); // @text EN: "Five tries and if he feels pain\neven once, you win." | JA: "コイツを５発殴って、\n１回でも痛がらせたら勝ちだ。"
    switch (SFBR_BETS[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100W010", 1, 1100); // @text EN: "If you win,\nyou get $10." | JA: "そのときは１０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W011", 1, 1200); // @text EN: "But if you fail,\nyou'll owe me $10." | JA: "ただし、失敗したら１０ドルを\n払ってもらう。"
        break;
    case 20:
        EVT_AT_SetVoice("0100W012", 1, 1100); // @text EN: "If you win,\nyou get $20." | JA: "そのときは２０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W013", 1, 1200); // @text EN: "But if you fail,\nyou'll owe me $20." | JA: "ただし、失敗したら２０ドルを\n払ってもらう。"
        break;
    case 30:
        EVT_AT_SetVoice("0100W014", 1, 1100); // @text EN: "If you win,\nyou get $30." | JA: "そのときは３０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W015", 1, 1200); // @text EN: "But if you fail,\nyou'll owe me $30." | JA: "ただし、失敗したら３０ドルを\n払ってもらう。"
        break;
    case 40:
        EVT_AT_SetVoice("0100W016", 1, 1100); // @text EN: "If you win,\nyou get $40." | JA: "そのときは４０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W017", 1, 1200); // @text EN: "But if you fail,\nyou'll owe me $40." | JA: "ただし、失敗したら４０ドルを\n払ってもらう。"
        break;
    default:
        EVT_AT_SetVoice("0100W007", 1, 1100); // @text EN: "If you win,\nyou get $50." | JA: "そのときは５０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W008", 1, 1200); // @text EN: "But if you fail,\nyou'll owe me $50." | JA: "ただし、失敗したら\n５０ドルを払ってくれ。"
        break;
    }
    EVT_AT_SetVoice("0100W009", 1, 1340); // @text EN: "Is there anyone strong enough\nto challenge this man?" | JA: "さあ、誰か勝負しようって\n腕自慢はいないか？"
    QueueHighModel(SFBR_SD5, 0, 1, 120);
    QueueHighModel(SFBR_SD5, 1, 1, 120);
    QueueHighModel(SFBR_SD5, 0, 0, 220);
    QueueHighModel(SFBR_SD5, 1, 0, 220);
    QueueHighModel(fighter, 0, 1, 220);
    QueueHighModel(fighter, 1, 1, 220);
    QueueHighModel(fighter, 0, 0, 320);
    QueueHighModel(fighter, 1, 0, 320);
    QueueHighModel(SFBR_SD5, 1, 1, 320);
    QueueHighModel(STREET_FIGHT_RYO, 0, 1, 320);
    QueueHighModel(STREET_FIGHT_RYO, 0, 0, 540);
    QueueHighModel(SFBR_SD5, 1, 1, 540);
    QueueHighModel(SFBR_SD5, 0, 1, 540);
    QueueHighModel(fighter, 1, 1, 540);
    QueueHighModel(fighter, 0, 1, 540);
    QueueHighModel(SFBR_SD5, 1, 0, 780);
    QueueHighModel(SFBR_SD5, 0, 0, 780);
    QueueHighModel(fighter, 1, 0, 780);
    QueueHighModel(fighter, 0, 0, 780);
    QueueHighModel(SFBR_SD5, 1, 1, 970);
    QueueHighModel(SFBR_SD5, 0, 1, 970);
    QueueHighModel(fighter, 0, 1, 970);
    QueueHighModel(SFBR_SD5, 0, 0, 1200);
    QueueHighModel(SFBR_SD5, 1, 0, 1200);
    QueueHighModel(fighter, 0, 0, 1200);
    QueueHighModel(SFBR_SD5, 0, 0, 1300);
    QueueHighModel(SFBR_SD5, 1, 0, 1300);
    QueueHighModel(fighter, 1, 1, 1300);
    QueueHighModel(fighter, 0, 1, 1300);
    EventSetCamera camera{.cleanup = 0, .camera = 1100, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1110, SFBR_BASE, SFBR_BASE, 0, 0, 120);
    EVT_AT_SetCam(1115, SFBR_BASE, SFBR_BASE, 0, 0, 220);
    EVT_AT_SetCam(1120, SFBR_BASE, SFBR_BASE, 0, 0, 320);
    EVT_AT_SetCam(1130, SFBR_BASE, SFBR_BASE, 0, 0, 540);
    EVT_AT_SetCam(1160, SFBR_BASE, SFBR_BASE, 0, 0, 780);
    EVT_AT_SetCam(2100, SFBR_BASE, SFBR_BASE, 0, 0, 970);
    EVT_AT_SetCam(2150, SFBR_BASE, SFBR_BASE, 0, 0, 1200);
    EVT_AT_SetCam(1161, SFBR_BASE, SFBR_BASE, 0, 0, 1300);
    return 1450;
}
std::int32_t RepeatIntroConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    BeginSecondarySampleExecution(0);
    EV_EnableCharacterDisplay(SFBR_MKZI);
    EV_EnsureCharacterDraw(SFBR_MKZI);
    EVT_AT_SetPos(SFBR_MKZI, SFBR_BASE, {1000, 0, 0}, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 225, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetMot(SFBR_SD5, 6852, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, {-0.5f, 0, -1}, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x50000000U, 0);
    EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 2});
    EVT_AT_SetVoice("0100W006", 1, 0); // @text EN: "Five tries and if he feels pain\neven once, you win." | JA: "コイツを５発殴って、\n１回でも痛がらせたら勝ちだ。"
    switch (SFBR_BETS[work->rank]) {
    case 10:
        EVT_AT_SetVoice("0100W010", 1, 130); // @text EN: "If you win,\nyou get $10." | JA: "そのときは１０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W011", 1, 230); // @text EN: "But if you fail,\nyou'll owe me $10." | JA: "ただし、失敗したら１０ドルを\n払ってもらう。"
        break;
    case 20:
        EVT_AT_SetVoice("0100W012", 1, 130); // @text EN: "If you win,\nyou get $20." | JA: "そのときは２０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W013", 1, 230); // @text EN: "But if you fail,\nyou'll owe me $20." | JA: "ただし、失敗したら２０ドルを\n払ってもらう。"
        break;
    case 30:
        EVT_AT_SetVoice("0100W014", 1, 130); // @text EN: "If you win,\nyou get $30." | JA: "そのときは３０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W015", 1, 230); // @text EN: "But if you fail,\nyou'll owe me $30." | JA: "ただし、失敗したら３０ドルを\n払ってもらう。"
        break;
    case 40:
        EVT_AT_SetVoice("0100W016", 1, 130); // @text EN: "If you win,\nyou get $40." | JA: "そのときは４０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W017", 1, 230); // @text EN: "But if you fail,\nyou'll owe me $40." | JA: "ただし、失敗したら４０ドルを\n払ってもらう。"
        break;
    default:
        EVT_AT_SetVoice("0100W007", 1, 130); // @text EN: "If you win,\nyou get $50." | JA: "そのときは５０ドルを進呈するぜ。"
        EVT_AT_SetVoice("0100W008", 1, 230); // @text EN: "But if you fail,\nyou'll owe me $50." | JA: "ただし、失敗したら\n５０ドルを払ってくれ。"
        break;
    }
    EVT_AT_SetVoice("0100W009", 1, 370); // @text EN: "Is there anyone strong enough\nto challenge this man?" | JA: "さあ、誰か勝負しようって\n腕自慢はいないか？"
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFBR_SD5), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    QueueHighModel(SFBR_SD5, 0, 0, 230);
    QueueHighModel(SFBR_SD5, 1, 0, 230);
    QueueHighModel(fighter, 0, 1, 230);
    QueueHighModel(fighter, 1, 1, 230);
    EVT_AT_SetCam(2140, SFBR_BASE, SFBR_BASE, 0, 0, 1);
    EVT_AT_SetCam(2145, SFBR_BASE, SFBR_BASE, 0, 0, 230);
    return 500;
}
std::int32_t AcceptConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    QueueStreetFightLook(SFBR_SD5, STREET_FIGHT_RYO);
    QueueStreetFightLook(fighter, STREET_FIGHT_RYO);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
    QueueHighModel(STREET_FIGHT_RYO, 1, 0, 28);
    QueueHighModel(STREET_FIGHT_RYO, 0, 1, 28);
    QueueHighModel(fighter, 1, 1, 28);
    QueueHighModel(fighter, 0, 0, 28);
    QueueHighModel(SFBR_SD5, 1, 1, 28);
    QueueHighModel(SFBR_SD5, 0, 0, 28);
    QueueHighModel(STREET_FIGHT_RYO, 0, 0, 176);
    QueueHighModel(fighter, 1, 0, 176);
    QueueHighModel(SFBR_SD5, 1, 0, 176);
    EVT_AT_SetCam(1200, SFBR_BASE, SFBR_BASE, 0, 0, 1);
    EVT_AT_SetCam(1210, SFBR_BASE, SFBR_BASE, 0, 0, 28);
    EVT_AT_SetVoice("0100A022", 1, 1); // @text EN: "I'll do it." | JA: "俺がやろう。"
    if (work->audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
        if (audience) {
            audience->state = 4;
            audience->loopDelay = 31;
        }
    }
    EVT_AT_SetVoice("0100W018", 1, 30); // @text EN: "You look strong enough.\nGood luck." | JA: "ほう、腕っぷし強そうだな。\nがんばんな。"
    EVT_AT_SetVoice("0100J003", 1, 180);
    return 176;
}
std::int32_t DeclineConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
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
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFBR_SD5), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    EventSetCamera camera{.cleanup = 0, .camera = 2110, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 31;
}
std::int32_t InsufficientConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const money = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    money->cleanup = 1;
    money->delay = 60;
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(STREET_FIGHT_RYO), FacialNodeReplacement);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(STREET_FIGHT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFBR_SD5), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 0, 1, 0);
    EventSetCamera camera{.cleanup = 0, .camera = 2110, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 120;
}

std::int32_t VictoryConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    SetCurrentGameFlag(4, 1);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6856, 0, 1, 0, 0);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetMot(SFBR_SD5, 6852, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, SFBR_CONVERSATION_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x10000000U, 0);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(SFBR_BETS[work->rank]), 200);
    EventSetCamera camera{.cleanup = 0, .camera = 1500, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    std::int32_t duration;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 717)) {
        EVT_AT_SetVoice("0100J004", 1, 35);
        EVT_AT_SetVoice("0100W027", 1, 90); // @text EN: "Hey, are you all right, brother?" | JA: "おい、大丈夫か、兄弟。"
        EVT_AT_SetVoice("0100W028", 1, 160); // @text EN: "You definitely hurt him=@Here." | JA: "やられたぜ、兄ちゃん。\nほらよ。"
        duration = 300;
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(1510, SFBR_BASE, SFBR_BASE, 0, 0, 90);
        EVT_AT_SetCam(1530, SFBR_BASE, SFBR_BASE, 0, 0, 160);
        SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
        QueueHighModel(SFBR_SD5, 0, 0, 90);
        QueueHighModel(fighter, 0, 1, 90);
        QueueHighModel(fighter, 1, 1, 90);
    } else {
        EVT_AT_SetVoice("0100J004", 1, 35);
        EVT_AT_SetVoice("0100W022", 1, 90); // @text EN: "Steady, brother." | JA: "おい、しっかりしろ、兄弟。"
        EVT_AT_SetVoice("0100W023", 1, 170); // @text EN: "You're something, boy.\nHere." | JA: "やるなぁ、兄ちゃん。\nほらよ。"
        EVT_AT_SetVoice("0100J005", 1, 240);
        EVT_AT_SetVoice("0100W024", 1, 340); // @text EN: "What? Young man, he wants you\nto come to his village=@" | JA: "なになに…兄ちゃん、\nあんたコイツの村に来ないかってよ。"
        EVT_AT_SetVoice("0100W025", 1, 500); // @text EN: "to meet his younger sister." | JA: "妹を紹介したいんだと。"
        EVT_AT_SetVoice("0100A021", 1, 560); // @text EN: "No, I think I'll pass." | JA: "…いえ、遠慮しときます。"
        EVT_AT_SetVoice("0100W026", 1, 620); // @text EN: "Oh well.\nSee you again." | JA: "そうか。じゃあ、またな。"
        duration = 700;
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(1510, SFBR_BASE, SFBR_BASE, 0, 0, 90);
        EVT_AT_SetCam(1520, SFBR_BASE, SFBR_BASE, 0, 0, 240);
        EVT_AT_SetCam(1530, SFBR_BASE, SFBR_BASE, 0, 0, 560);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(SFBR_SD5), FacialNodeReplacement);
        SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
        QueueHighModel(SFBR_SD5, 1, 0, 90);
        QueueHighModel(SFBR_SD5, 0, 0, 90);
        QueueHighModel(fighter, 0, 0, 90);
        QueueHighModel(fighter, 1, 0, 90);
        QueueHighModel(SFBR_SD5, 1, 1, 240);
        QueueHighModel(SFBR_SD5, 0, 1, 240);
        QueueHighModel(fighter, 1, 1, 240);
        QueueHighModel(SFBR_SD5, 1, 0, 560);
        QueueHighModel(SFBR_SD5, 0, 0, 560);
        QueueHighModel(fighter, 1, 0, 560);
        QueueHighModel(STREET_FIGHT_RYO, 1, 1, 560);
    }
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    return duration;
}
std::int32_t DefeatConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    QueueStreetFightLook(STREET_FIGHT_RYO, SFBR_SD5);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6855, 0, 1, 0, 120);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetMot(SFBR_SD5, 6852, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, SFBR_CONVERSATION_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x10000000U, 0);
    EventSetCamera camera{.cleanup = 0, .camera = 1600, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1610, SFBR_BASE, SFBR_BASE, 0, 0, 120);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    QueueHighModel(SFBR_SD5, 0, 0, 120);
    QueueHighModel(fighter, 0, 0, 120);
    QueueHighModel(fighter, 1, 0, 120);
    QueueHighModel(STREET_FIGHT_RYO, 0, 1, 120);
    const std::int32_t playerFlag = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1784));
    EVT_AT_SetVoice("0100W029", 1, 0); // @text EN: "Time is up!" | JA: "はい、そこまでー！"
    if (playerFlag) {
        EVT_AT_SetVoice("0100J007", 1, 60);
        EVT_AT_SetVoice("0100W033", 1, 150); // @text EN: "Uh=@what? His blows felt like a fly\nlanding on you?" | JA: "なになに…ハエが止まったような\n攻撃だったって？"
        EVT_AT_SetVoice("0100W031", 1, 310); // @text EN: "Oh well.\nToo bad." | JA: "残念だったな、兄ちゃん。"
        EVT_AT_SetVoice("0100W032", 1, 360); // @text EN: "Come and try again." | JA: "また今度挑戦してくれ。"
        EVT_AT_SetCam(2130, SFBR_BASE, SFBR_BASE, 0, 0, 290);
    } else {
        EVT_AT_SetVoice("0100J006", 1, 60);
        EVT_AT_SetVoice("0100W030", 1, 150); // @text EN: "=@What? Those strikes didn't\neven make you flinch?" | JA: "なになに…そんな攻撃くらいじゃ、\nびくともしない？"
        EVT_AT_SetVoice("0100W031", 1, 310); // @text EN: "Oh well.\nToo bad." | JA: "残念だったな、兄ちゃん。"
        EVT_AT_SetVoice("0100W032", 1, 360); // @text EN: "Come and try again." | JA: "また今度挑戦してくれ。"
        EVT_AT_SetCam(2130, SFBR_BASE, SFBR_BASE, 0, 0, 280);
    }
    QueueHighModel(SFBR_SD5, 0, 1, 280);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-SFBR_BETS[work->rank]), 320);
    SetCurrentGameFlag(4, 1);
    return 430;
}
std::int32_t ExpiredConversation(void *parameter) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFBR_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFBR_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 6855, 0, 1, 0, 0);
    EVT_AT_SetPos(SFBR_SD5, SFBR_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(SFBR_SD5, SFBR_BASE, 0, 0);
    EVT_AT_SetMot(SFBR_SD5, 6852, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFBR_BASE, SFBR_CONVERSATION_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFBR_BASE, 17, 0);
    EVT_AT_SetMot(fighter, 6849, 0, 1, 0x10000000U, 0);
    EventSetCamera camera{.cleanup = 0, .camera = 1600, .eye = SFBR_BASE, .target = SFBR_BASE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1610, SFBR_BASE, SFBR_BASE, 0, 0, 120);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SFBR_SD5), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(fighter), 1, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(fighter), FacialNodeReplacement);
    QueueHighModel(SFBR_SD5, 0, 0, 120);
    QueueHighModel(fighter, 0, 0, 120);
    QueueHighModel(fighter, 1, 0, 120);
    QueueHighModel(STREET_FIGHT_RYO, 0, 1, 120);
    EVT_AT_SetVoice("0100W029", 1, 0); // @text EN: "Time is up!" | JA: "はい、そこまでー！"
    EVT_AT_SetVoice("0100W034", 1, 60); // @text EN: "Young man, you can't win just by\nstanding there." | JA: "おいおい、兄ちゃん、\nボーッとしてちゃ勝てねえぜ。"
    EVT_AT_SetVoice("0100W035", 1, 160); // @text EN: "It's a draw, so I can't\nask for your money." | JA: "ま、引き分けじゃ金は\n取れねえな。"
    EVT_AT_SetVoice("0100W032", 1, 250); // @text EN: "Come and try again." | JA: "また今度挑戦してくれ。"
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const money = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    money->cleanup = 1;
    money->delay = 275;
    SetCurrentGameFlag(4, 1);
    return 380;
}
void InitializeArea() {
    bearBattleVoiceIndex = 0;
}
void RunArea(std::uintptr_t parameter) {
    StreetFightStartParameters *const parameters = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (parameters->scene == 99)
        QueueDebug(parameters);
    else
        QueueNormal(parameters);
}
} // namespace
const ScriptAreaDefinition areaSfbrScriptDefinition{.areaId = STREET_FIGHT_SFBR, .field04 = 0, .init = InitializeArea, .main = RunArea};
} // namespace shenmue::s2
