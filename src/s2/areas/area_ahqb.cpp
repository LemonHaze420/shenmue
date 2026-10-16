#include "s2/debug.hpp"
#include "s2/areas/area_ahqb.hpp"
#include "s2/event_runtime.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/weather.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
// @verified - p
void AHQBQueueComboQte(std::int16_t *result, std::int32_t totalFrames, std::span<const std::uint16_t> sequence) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AHQBComboQteCallback, 6, 0x0b, sizeof(AhqbComboQteWork), AHQB_TASK);
    AhqbComboQteWork *const work = static_cast<AhqbComboQteWork *>(HLib::GetTaskParameter(task));
    for (std::size_t index = 0; index < sequence.size(); ++index)
        std::memcpy(&work->sequence[index], &sequence[index], sizeof(std::uint16_t));
    *result = 0;
    work->task = task;
    work->parent = HLib::GetCurrentTask();
    work->frames = 0;
    work->totalFrames = totalFrames;
    work->result = result;
    work->count = static_cast<std::int16_t>(sequence.size());
    HLib::SetupTimedTask(work->parent, 0);
    SetShowQteFlag(static_cast<std::uint16_t>(work->sequence[0]), static_cast<std::int16_t>(totalFrames), 2);
}

// @verified - p
void AHQBComboQteCallback(void *data) {
    AhqbComboQteWork *const work = static_cast<AhqbComboQteWork *>(data);
    ++work->frames;
    if (GetShouldRenderIcons(RenderIconSlot::Qte))
        return;
    const std::uint16_t status = static_cast<std::uint16_t>(EV_GetCurrentQteStatus());
    const std::int16_t index = *work->result;
    if ((status & 0x0fffU) == work->sequence[index]) {
        *work->result = static_cast<std::int16_t>(static_cast<std::uint16_t>(index) + 1U);
        const std::int16_t remaining = static_cast<std::int16_t>(static_cast<std::uint16_t>(work->totalFrames) - static_cast<std::uint16_t>(work->frames));
        if (*work->result < work->count && remaining > 0) {
            SetShowQteFlag(static_cast<std::uint16_t>(work->sequence[*work->result]), remaining, 2);
            return;
        }
    }
    const std::int32_t remaining = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->totalFrames) - work->frames);
    if (remaining > 0)
        HLib::SetupTimedTask(work->parent, remaining);
    else
        HLib::ResolveDeferredCallback(work->parent);
    HLib::DestroyTaskInternal(work->task);
}
// @verified - p
void AreaAHQBInit() {
}
void AreaAHQBMain(std::uintptr_t) {
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), AHQB_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = AHQBQueueMain;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
// @verified - p
void AHQBQueueMain(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AHQBMainCallback, 5, 0x0b, sizeof(AhqbEventWork), AHQB_TASK);
    AhqbEventWork *const work = static_cast<AhqbEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->reserved0c = 0;
    work->state = 0;
    if (g_currentMap.name == AREA_AHQB.name) {
        SetWeatherUpdateEnabledTo(1);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
    }
    work->characters = Queue_EVT_AT_ReadChr(task, AHQB_CHARACTERS);
}
// @verified - p
void AHQBMainCallback(void *data) {
    AhqbEventWork *const work = static_cast<AhqbEventWork *>(data);
    std::int32_t delay = 0;
    switch (work->state) {
    case 0:
        delay = AHQBInitialize(work);
        break;
    case 1:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(AHQB_FIRST_AUTHOR.name), nullptr, nullptr, 0);
        work->state = 2;
        return;
    case 2:
        delay = AHQBCombatPhase(work);
        break;
    case 3:
        EV_LoadAudio("0010A000", 2);
        EV_stepSetFlags(0, 0xee);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(AHQB_WIN_AUTHOR.name), nullptr, nullptr, 0);
        work->state = 5;
        return;
    case 4:
        EV_LoadAudio("0010A000", 2);
        EV_stepSetFlags(1, 0xee);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(AHQB_LOSE_AUTHOR.name), nullptr, nullptr, 0);
        work->state = 5;
        return;
    case 5:
        if (g_currentMap.name == AREA_AHQB.name) {
            work->state = 1;
            work->phase = 0;
        } else {
            delay = 30;
            spmFadeOut();
            HLib::SetCurrentTaskTo(AHQBEndCallback);
        }
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void AHQBEndCallback(void *data) {
    AhqbEventWork *const work = static_cast<AhqbEventWork *>(data);
    SetScenarioCharacterKillMode(AHQB_BASE, 1);
    SetScenarioCharacterKillMode(AHQB_DUMMY, 1);
    if (DoesScenarioCharacterExist(AHQB_DIS1))
        EV_EnableCharacterDisplay(AHQB_DIS1);
    if (DoesScenarioCharacterExist(AHQB_DIS2))
        EV_EnableCharacterDisplay(AHQB_DIS2);
    for (const shen_name character : {AHQB_PLAYER, AHQB_HTK, AHQB_GOI}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 0, 1, 0);
    }
    hndlSetFlags(FindCharacterByScenarioName(AHQB_HTK));
    hndlSetFlags(FindCharacterByScenarioName(AHQB_GOI));
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(AHQB_PLAYER), 0, 0, 1);
    ET_AT_KillWork(work->characters);
    EV_FreeAuthorPak(0, "0010");
    if (g_currentMap.name == AHQB_WN00.name)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AHQBAUT3"), 0);
    else if (g_currentMap.name == AHQB_AR03.name)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AHQBAUT2"), 0);
    else if (g_currentMap.name == AHQB_WS00.name || g_currentMap.name == AREA_AHQB.name)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AHQBAUT1"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AHQB"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_NPC_NewLoadSw(1);
    switch (g_currentMap.name) {
    case AHQB_WN00.name:
        EVT_AT_SetPos(AHQB_PLAYER, AHQB_INVALID, {235.9f, 104.5f, 200.4f}, 0);
        EVT_AT_SetAngY(AHQB_PLAYER, AHQB_INVALID, 45, 0);
        break;
    case AHQB_WS00.name:
        EVT_AT_SetPos(AHQB_PLAYER, AHQB_INVALID, {38.8f, 100, 175.1f}, 0);
        EVT_AT_SetAngY(AHQB_PLAYER, AHQB_INVALID, 180, 0);
        break;
    case AHQB_AR03.name:
        EVT_AT_SetPos(AHQB_PLAYER, AHQB_INVALID, {303.8f, 100, 483.1f}, 0);
        EVT_AT_SetAngY(AHQB_PLAYER, AHQB_INVALID, 180, 0);
        break;
    case AREA_AHQB.name:
        EVT_AT_SetPos(AHQB_PLAYER, AHQB_INVALID, {38.8f, 100.2f, 175.1f}, 0);
        EVT_AT_SetAngY(AHQB_PLAYER, AHQB_INVALID, 180, 0);
        break;
    default:
        break;
    }
    FinishLoading();
}
namespace {

void AHQBInitializationNullsub() {
}
} // namespace
// @verified - p
std::int32_t AHQBInitialize(AhqbEventWork *work) {
    AHQBInitializationNullsub();
    EV_LoadCutsceneFromCurrentScene("0010A000");
    EV_LoadAudio("0010A000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0010");
    switch (g_currentMap.name) {
    case AHQB_WN00.name:
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("AHQBAUT3"));
        break;
    case AHQB_AR03.name:
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("AHQBAUT2"));
        break;
    case AHQB_WS00.name:
    case AREA_AHQB.name:
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("AHQBAUT1"));
        break;
    default:
        break;
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("AHQB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("AHQB"), 0);
    EV_debug_log("AHQB\\eAHQB_et_snakeqte.c", 221, "sssss\n");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, AHQB_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, AHQB_DUMMY);
    if (DoesScenarioCharacterExist(AHQB_DIS1))
        EV_DisableCharacterDisplay(AHQB_DIS1);
    if (DoesScenarioCharacterExist(AHQB_DIS2))
        EV_DisableCharacterDisplay(AHQB_DIS2);
    SetLowQualityHands(FindCharacterByScenarioName(AHQB_HTK));
    SetLowQualityHands(FindCharacterByScenarioName(AHQB_GOI));
    for (const shen_name character : {AHQB_HTK, AHQB_GOI, AHQB_PLAYER}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 1, 0);
    }
    work->state = 1;
    switch (g_currentMap.name) {
    case AHQB_WN00.name:
        EVT_AT_SetPos(AHQB_BASE, AHQB_INVALID, {236.9f, 104.5f, 200.5f}, 0);
        EVT_AT_SetAngY(AHQB_BASE, AHQB_INVALID, 292.5f, 0);
        break;
    case AHQB_AR03.name:
        EVT_AT_SetPos(AHQB_BASE, AHQB_INVALID, {302.5f, 100.2f, 481}, 0);
        EVT_AT_SetAngY(AHQB_BASE, AHQB_INVALID, 135, 0);
        break;
    case AHQB_WS00.name:
    case AREA_AHQB.name:
        EVT_AT_SetPos(AHQB_BASE, AHQB_INVALID, {37.8f, 100.2f, 176.5f}, 0);
        EVT_AT_SetAngY(AHQB_BASE, AHQB_INVALID, 270, 0);
        break;
    default:
        break;
    }
    return 0;
}
const ScriptAreaDefinition areaAHQBScriptDefinition{.areaId = AREA_AHQB, .field04 = 0, .init = AreaAHQBInit, .main = AreaAHQBMain};
} // namespace shenmue::s2::areas
