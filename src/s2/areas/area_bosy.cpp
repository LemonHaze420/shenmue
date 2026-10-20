#include "s2/areas/area_bosy.hpp"
#include "s2/pause.hpp"
#include "s2/npc_package.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/seqp_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, syuei{SHEN_NAME(SYE_)}, base{SHEN_NAME(BASE)}, prop{SHEN_NAME(a043)};
constexpr shen_name names[]{ryo, syuei, shen_name{0xffffffffU}};
struct BoutWork {
    CompiledSequenceWork *sequence;
    HLib::Task *self;
    std::int32_t active, phase;
    std::byte reserved18[8];
    std::int32_t motions[16];
    EventReadCharactersWork *characters;
    std::int32_t argument, reserved6c;
};
struct AuthorWork {
    HLib::Task *self;
    std::uint32_t reserved08;
    shen_name command;
    std::uint16_t phase;
    std::byte reserved12[6];
};
static_assert(sizeof(BoutWork) == 0x70 && offsetof(BoutWork, characters) == 0x60);
static_assert(sizeof(AuthorWork) == 0x18 && offsetof(AuthorWork, phase) == 0x10);
std::uint64_t initializationState[8]{};
void Sequence_Callback(void *);
void Fight_Callback(void *);
void Author_Callback(void *);
std::int32_t ReadCounter(CompiledSequenceWork *work) {
    std::int32_t value;
    std::memcpy(&value, reinterpret_cast<std::byte *>(work) + 0x1d0, 4);
    return value;
}
void WriteCounter(CompiledSequenceWork *work, std::int32_t value) {
    std::memcpy(reinterpret_cast<std::byte *>(work) + 0x1d0, &value, 4);
}
std::uint32_t ReadDialogueId(CompiledSequenceWork *work) {
    std::uint32_t value;
    std::memcpy(&value, reinterpret_cast<std::byte *>(work) + 0x248, 4);
    return value;
}
void WriteDialogueId(CompiledSequenceWork *work, std::uint32_t value) {
    std::memcpy(reinterpret_cast<std::byte *>(work) + 0x248, &value, 4);
}
void Transition(CompiledSequenceWork *work, std::uint16_t mode) {
    work->previousMode = work->mode;
    work->mode = mode;
    work->continueAfterWait = 1;
    work->continueLoop = 1;
}
void Wait(CompiledSequenceWork *work, std::uint16_t mode, std::uint16_t next, std::int32_t delay, std::int32_t continueAfter) {
    work->mode = mode;
    work->nextMode = next;
    work->delay = delay;
    work->continueAfterWait = continueAfter;
}

void RestoreAuthorCharacters() {
    for (auto name : {ryo, syuei}) {
        EV_EnableCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
        EV_SetCharacterAlphaEnabled(name, true);
    }
    EV_EnableCharacterDisplay(prop);
    EV_SetCharaControllerFlags(prop, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(prop, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(prop, 3, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(prop, true);
    figpAssignModeFor(ryo, 0);
    figpAssignModeFor(syuei, 0);
}

void AuthorFrame_Callback(void *data) {
    auto *work = *static_cast<AuthorWork **>(data);
    const auto command = work->command.name;
    const auto frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (command == SHEN_NAME(AU_2)) {
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 15)
            spmFadeOut();
    } else if (frame == 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (command == SHEN_NAME(AU_2)) {
        AddInventoryItem(0x1782, 1, 1);
        HLib::SetupTimedTask(work->self, 130);
        RestoreAuthorCharacters();
        work->phase = 3;
    } else {
        RestoreAuthorCharacters();
        work->phase = 3;
        if (command == SHEN_NAME(AU_0)) {
            HLib::SetupTimedTask(work->self, 30);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        }
    }
    HLib::DestroyCurrentTask();
}

void Author_Callback(void *data) {
    auto *work = static_cast<AuthorWork *>(data);
    if (work->phase == 1) {
        auto *task = HLib::EnqueueTaskWithParameter(AuthorFrame_Callback, 4, 0x0b, sizeof(AuthorWork *), event);
        *static_cast<AuthorWork **>(HLib::GetTaskParameter(task)) = work;
        ResetAuthorCharacterForSequence(ryo);
        ResetAuthorCharacterForSequence(syuei);
        EV_EnableCharacterDisplay(prop);
        EV_SetCharaControllerFlags(prop, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(prop, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(prop, false);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->command.name));
        work->phase = 2;
    } else if (work->phase == 3)
        HLib::DestroyTask(work->self);
}
void QueueAuthor(shen_name command) {
    auto *task = HLib::EnqueueTaskWithParameter(Author_Callback, 4, 0x0b, sizeof(AuthorWork), event);
    auto *work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->phase = 1;
    work->command = command;
    SetPlayerControlEnabled(0);
}

std::int64_t NextBattleDialogue(void *data) {
    auto *index = static_cast<std::uint16_t *>(data);
    constexpr std::int16_t ids[]{174, 175, 176};
    const auto result = ids[*index];
    if (++*index >= 3)
        *index = 0;
    return result;
}

void Fight_Callback(void *data) {
    auto *work = static_cast<CompiledSequenceWork *>(data);
    work->callbackFlag = 0;
    do {
        work->continueLoop = 0;
        switch (work->mode) {
        case 0:
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_EnableCharacterDisplay(ryo);
            EV_EnableCharacterDisplay(syuei);
            Transition(work, 100);
            break;
        case 12:
            if (!IsFightActive())
                Transition(work, 104);
            break;
        case 100: {
            WriteCounter(work, 0);
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("BOSY"), 1);
            shenInitFightData(FindCharacterByScenarioName(ryo), nullptr, 0, 0, 0, 0);
            shenInitFightData(FindCharacterByScenarioName(syuei), nullptr, 1, 1, 20000, 15);
            vfCopyPosFromEntity(0, nullptr, 336, 269.7f, 5);
            vfmQueueFightStart(2, 99, 1, 0, 180);
            EV_SetCharacterAlphaEnabled(syuei, false);
            AllocateBattleDialogue(11, 64, ryo, shen_name{}, 10, 100, 0);
            AllocateBattleDialogue(10, 70, ryo, shen_name{}, 20, 100, 0);
            const auto id = AllocateBattleDialogue(12, -1, ryo, shen_name{}, 240, 100, 1);
            AssignBattleDialogueCallback(id, NextBattleDialogue);
            Transition(work, 101);
            break;
        }
        case 101:
            if (!IsFightActive()) {
                Transition(work, 104);
                break;
            }
            if (ReadCounter(work) > 900 && GetFightCounterHighWord() > 30) {
                check_dead_status(1);
                Transition(work, 12);
                break;
            }
            if (ReadCounter(work) > 1350) {
                WriteDialogueId(work, AllocateBattleDialogue(0, 59, shen_name{}, shen_name{}, 1, 100, 0));
                Transition(work, 102);
                break;
            }
            WriteCounter(work, ReadCounter(work) + 1);
            break;
        case 102:
            if (!GetBattleDialogueCompletionCount(ReadDialogueId(work)))
                break;
            BeginFightDialogueRetirement();
            EV_SetCinemaBarVisibility(FilmControlFlag::Show, 5);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
            SetFightFlag400000(1);
            SetQteCameraAtTop(1);
            Transition(work, 103);
            break;
        case 103:
            if (IsFightActive()) {
                if (ReadCounter(work) <= 2100) {
                    WriteCounter(work, ReadCounter(work) + 1);
                    break;
                }
                EV_SetCinemaBarVisibility(FilmControlFlag::Hide, 5);
                check_dead_status(1);
                Transition(work, 12);
            } else {
                EV_SetCinemaBarVisibility(FilmControlFlag::Hide, 5);
                Transition(work, 104);
            }
            break;
        case 104:
            SetQteCameraAtTop(0);
            SetScreenFilmFade(5, 0, 0, 0, 0, 255, 0, 0, 0);
            Wait(work, 11, 105, 2, 1);
            break;
        case 105:
            figpAssignModeFor(ryo, 0);
            figpAssignModeFor(syuei, 0);
            RetireCompiledSequenceLinks(work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(work);
            break;
        }
    } while (work->continueLoop);
}

void Sequence_Callback(void *data) {
    auto *work = static_cast<CompiledSequenceWork *>(data);
    auto *owner = static_cast<BoutWork *>(work->event);
    work->callbackFlag = 0;
    do {
        work->continueLoop = 0;
        switch (work->mode) {
        case 0:
            Transition(work, 102);
            break;
        case 100:
            return;
        case 101:
            Wait(work, 2, 102, 32, 1);
            return;
        case 102:
            QueueAuthor(SHEN_NAME(AU_0));
            Wait(work, 13, 103, 5, 0);
            SetQteCameraAtTop(1);
            break;
        case 103:
            Wait(work, 2, 104, 32, 1);
            SetQteCameraAtTop(0);
            break;
        case 104: {
            auto *child = QueueCompiledSequence(Fight_Callback, SHEN_NAME(BSBT), owner);
            if (!work->children[0])
                work->children[0] = child;
            child->parent = work;
            child->childSlot = 0;
            child->reference = work->reference + 1;
            work->timer = 0;
            std::uint32_t zero = 0;
            std::memcpy(work->parameters, &zero, 4);
            Wait(work, 10, 105, work->delay, 1);
            work->waitChildIndex = 0;
            break;
        }
        case 105:
            Transition(work, 106);
            break;
        case 106:
            QueueAuthor(SHEN_NAME(AU_1));
            Wait(work, 13, 107, 5, 0);
            break;
        case 107:
            Wait(work, 2, 108, 32, 1);
            return;
        case 108:
            QueueAuthor(SHEN_NAME(AU_2));
            Wait(work, 13, 109, 5, 0);
            break;
        case 109:
            Wait(work, 2, 110, 122, 1);
            return;
        case 110:
            Transition(work, 111);
            break;
        case 111:
            owner->active = 0;
            RetireCompiledSequenceLinks(work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(work);
            break;
        }
    } while (work->continueLoop);
}

void Exit(BoutWork *work) {
    using enum CameraState;
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(base, 1);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    for (auto *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0032");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BOSY"), 0);
    ExecuteCleanupFuncForAreaWithCachedScene();
    if (EV_GetShenmueFlag(255))
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 1);
    SetRenderClockAndCompassTo(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->self);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
}

void Bout_Callback(void *data) {
    auto *work = static_cast<BoutWork *>(data);
    switch (work->phase) {
    case 0:
        SetPlayerControlEnabled(0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BOSY"), 0);
        LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(syuei, "SYE_B.MT7", 1);
        work->sequence = QueueCompiledSequence(Sequence_Callback, SHEN_NAME(SEQP), work);
        work->active = 1;
        work->phase = 1;
        break;
    case 1:
        work->phase = 2;
        break;
    case 2:
        if (!work->active)
            work->phase = 3;
        break;
    case 3:
        work->phase = 4;
        break;
    case 4:
        for (auto *motion = work->motions; *motion; ++motion)
            DeleteMotion(*motion);
        FinishLoading();
        work->phase = 5;
        break;
    case 5:
        EV_stepSetFlags(0, 38);
        Exit(work);
        break;
    default:
        break;
    }
}
} // namespace
void AreaBOSYInit() {
    for (auto &value : initializationState)
        value = 0;
}

void AreaBOSYMain(std::uintptr_t parameter) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    auto *task = HLib::EnqueueTaskWithParameter(Bout_Callback, 4, 0x0b, sizeof(BoutWork), event);
    auto *work = static_cast<BoutWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->argument = static_cast<std::int32_t>(parameter);
    work->phase = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, names);
    if (IsFightActive())
        check_dead_status(1);
    work->motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BOSY"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BOSY"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    EV_FreeAuthorPak(1, "0032");
    EV_LoadCutsceneFromCurrentScene("0032A000");
    EV_spmBeginSampleExecute();
    EV_LoadAudio("0032A000", 1);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    UpdateWeather();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
    EV_EventSetUpHelpIcon_0(20, 0);
}
const ScriptAreaDefinition areaBOSYScriptDefinition{.areaId = SHEN_NAME(BOSY), .field04 = 0, .init = AreaBOSYInit, .main = AreaBOSYMain};
} // namespace shenmue::s2::areas
