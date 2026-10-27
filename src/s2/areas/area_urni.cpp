#include "s2/areas/area_urni.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
std::int32_t UranaiStoryPackageIndex() {
    if (EV_GetShenmueFlag(1526))
        return 1;
    std::int32_t index = 27;
    while (index > 0) {
        if (EV_GetShenmueFlag(URNI_SRI_STORY_FLAGS[index]))
            break;
        --index;
    }
    return index;
}
namespace {
shen_name uranaiCharacters[]{URNI_PLAYER, URNI_INVALID, URNI_INVALID};
void UranaiStart(void *);
void UranaiFade(void *);
void UranaiEnd(void *);
void UranaiSelect(void *);
void UranaiSame(void *);
void UranaiResult(void *);
void UranaiMoney(void *);
void UranaiAdmissionNoop() {
}
void UranaiDiagnosticNoop() {
}
void InitializeUranai(UranaiWork *work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    uranaiCharacters[1] = work->selection->reader;
    work->characters = Queue_EVT_AT_ReadChr(work->task, uranaiCharacters);
    work->package = "SUISYO";
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("URNI"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("URNI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("URNI"), 0);
    EV_FreeAuthorPak(1, "0120");
    std::strcpy(eventVoicePrefix, "0120");
    if (work->selection->mode != 1) {
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0120");
    }
    shenSetClockAndCompassRenderTo(1);
    UranaiDiagnosticNoop();
    EV_EventSetUpHelpIcon_0(0x36, 0);
}
void QueueUranaiStart(void *parameter) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(UranaiStart, 4, 0x0b, sizeof(UranaiWork), URNI_TASK);
    UranaiWork *const work = static_cast<UranaiWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->selection = parameter ? static_cast<const UranaiSelection *>(parameter) : &URNI_SELECTIONS[g_entryPoint];
    InitializeUranai(work);
}
void UranaiMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), URNI_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueUranaiStart;
    work->callbackData = reinterpret_cast<void *>(argument);
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
void ConfigureUranai(UranaiWork *work, shen_name reader) {
    switch (reader.name) {
    case SHEN_NAME(SYK_):
        work->configuration = &URNI_SYK_CONFIGURATION;
        break;
    case SHEN_NAME(KLN_):
        work->configuration = &URNI_KLN_CONFIGURATION;
        break;
    case SHEN_NAME(MM5_):
        work->configuration = g_currentMap.name == URNI_AR01.name ? &URNI_MM5_AR01_CONFIGURATION :
            g_currentMap.name == URNI_WS00.name                   ? &URNI_MM5_WS00_CONFIGURATION :
                                                                    &URNI_MM5_OTHER_CONFIGURATION;
        break;
    case SHEN_NAME(SEH_):
        work->configuration = &URNI_SEH_CONFIGURATION;
        break;
    case SHEN_NAME(SRI_):
        work->configuration = &URNI_SRI_CONFIGURATION;
        work->package = URNI_SRI_PACKAGES[UranaiStoryPackageIndex()];
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(work->package));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        break;
    case SHEN_NAME(RCD_):
        work->configuration = &URNI_RCD_CONFIGURATION;
        break;
    }
    SetLowQualityHands(ctrlGetCharaByName(reader));
    SetHighModelXYZ(ctrlGetCharaByName(reader), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(reader), 1, 1, 0);
    EventSetPosition position{.cleanup = 0, .character = URNI_BASE, .parent = URNI_INVALID, .position = work->configuration->position, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle rotation{.cleanup = 0, .character = URNI_BASE, .parent = URNI_INVALID, .degrees = work->configuration->yaw, .delay = 0};
    EVT_AT_SetAngY_Callback(&rotation);
    position.character = uranaiCharacters[1];
    position.parent = URNI_BASE;
    position.position = {};
    EVT_AT_SetPosRot(&position);
    rotation.character = uranaiCharacters[1];
    rotation.parent = URNI_BASE;
    rotation.degrees = 0;
    EVT_AT_SetAngY_Callback(&rotation);
    work->configuration->setup(reader, 0);
}
void UranaiStart(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    SetLowQualityHands(ctrlGetCharaByName(URNI_PLAYER));
    SetHighModelXYZ(ctrlGetCharaByName(URNI_PLAYER), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(URNI_PLAYER), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(URNI_PLAYER), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    const shen_name reader = work->selection->reader;
    ConfigureUranai(work, reader);
    AT_SetLocalBaseChara(URNI_PLAYER, URNI_BASE);
    AT_SetLocalBaseChara(URNI_PLAYER_DUMMY, URNI_PLAYER);
    AT_SetLocalBaseChara(reader, URNI_BASE);
    AT_SetLocalBaseChara(URNI_READER_DUMMY, reader);
    EventSetPosition position{.cleanup = 0, .character = URNI_PLAYER_DUMMY, .parent = URNI_PLAYER, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle rotation{.cleanup = 0, .character = URNI_PLAYER_DUMMY, .parent = URNI_PLAYER, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&rotation);
    position.character = URNI_READER_DUMMY;
    position.parent = reader;
    EVT_AT_SetPosRot(&position);
    rotation.character = URNI_READER_DUMMY;
    rotation.parent = reader;
    EVT_AT_SetAngY_Callback(&rotation);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work->selection->mode == 1) {
        EV_spmBeginSampleExecute();
        const std::int32_t delay = static_cast<std::int32_t>(work->configuration->modeOne(reader, 0));
        HLib::SetCurrentTaskTo(UranaiFade);
        HLib::SetupTaskDelay(delay);
    } else {
        EV_LoadAudio("0120", 0);
        EV_spmBeginSampleExecute();
        const std::int32_t delay = static_cast<std::int32_t>(work->configuration->introduction(reader, 0));
        if (delay) {
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            EVT_AT_KsIcon_Enqueue(-1, 25, 26, -1, 1800, 26, 0, delay);
            HLib::SetCurrentTaskTo(UranaiSelect);
            HLib::SetupTaskDelay(delay);
        } else
            HLib::SetCurrentTaskTo(UranaiSame);
    }
}
void UranaiSelect(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    FinishLoading();
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    std::int32_t delay;
    if (shenGetCtrlSpeakIconFlag() == 25) {
        const std::int32_t cash = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)));
        if (cash >= work->configuration->price) {
            const std::int32_t start = static_cast<std::int32_t>(work->configuration->beginReading(work->selection->reader, 0));
            const std::int32_t end = static_cast<std::int32_t>(work->configuration->reading(work->selection->reader, start));
            HLib::SetCurrentTaskTo(UranaiResult);
            HLib::SetupTaskDelay(end);
            return;
        }
        delay = static_cast<std::int32_t>(work->configuration->insufficientCash(work->selection->reader, 0));
    } else
        delay = static_cast<std::int32_t>(work->configuration->decline(work->selection->reader, 0));
    HLib::SetCurrentTaskTo(UranaiFade);
    HLib::SetupTaskDelay(delay - 60);
}
void UranaiSame(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    const std::int32_t delay = static_cast<std::int32_t>(work->configuration->same(work->selection->reader, 0));
    HLib::SetCurrentTaskTo(UranaiFade);
    HLib::SetupTaskDelay(delay - 60);
}
void UranaiResult(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    const std::int32_t delay = static_cast<std::int32_t>(work->configuration->result(work->selection->reader, 0));
    if (delay >= 0) {
        HLib::SetCurrentTaskTo(UranaiMoney);
        HLib::SetupTaskDelay(delay);
    } else
        HLib::SetCurrentTaskTo(UranaiSame);
}
void UranaiMoney(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    const std::int32_t delay = static_cast<std::int32_t>(work->configuration->payment(work->selection->reader, 0));
    PATI_StartMoneyCount(0U - static_cast<std::uint32_t>(work->configuration->price), delay - 150);
    HLib::SetCurrentTaskTo(UranaiFade);
    HLib::SetupTaskDelay(delay - 60);
}
void UranaiFade(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    if (work->selection->mode != 1)
        spmFadeOut();
    EventFilmControl fade{.cleanup = 0,
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
    EVT_AT_MenCtrl_Callback(&fade);
    HLib::SetCurrentTaskTo(UranaiEnd);
    HLib::SetupTaskDelay(60);
}
void CleanupUranai(UranaiWork *work) {
    EV_EnableCharacterDisplay(URNI_PLAYER);
    hndlSetFlags(ctrlGetCharaByName(work->selection->reader));
    hndlSetFlags(ctrlGetCharaByName(URNI_PLAYER));
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    if (work->selection->mode != 1) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    }
    EV_FreeAuthorPak(0, "0120");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("URNI"), 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
    ET_AT_KillWork(work->characters);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_NPC_NewLoadSw(1);
}
void UranaiEnd(void *parameter) {
    UranaiWork *const work = static_cast<UranaiWork *>(parameter);
    CleanupUranai(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
}
} // namespace
const ScriptAreaDefinition urniDefinition{.areaId = URNI_AREA, .field04 = 0, .init = UranaiAdmissionNoop, .main = UranaiMain};
} // namespace shenmue::s2
