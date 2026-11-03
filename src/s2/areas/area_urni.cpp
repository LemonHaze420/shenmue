#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_urni.hpp"

const UranaiConfiguration uranaiSykConfiguration = {{246.39999389648438f, 104.5f, 167.25999450683594f}, 0.0f, 10, UranaiSykSetup, UranaiSykModeOne,
    UranaiSykIntroduction, UranaiSykSame, UranaiSykDecline, UranaiSykBeginReading, UranaiSykReading, UranaiSykResult, UranaiSykPayment,
    UranaiSykInsufficientCash};
const UranaiConfiguration uranaiKlnConfiguration = {{277.70999145507812f, 100.0f, 135.83999633789062f}, -90.0f, 20, UranaiKlnSetup, UranaiKlnModeOne,
    UranaiKlnIntroduction, UranaiKlnSame, UranaiKlnDecline, UranaiKlnBeginReading, UranaiKlnReading, UranaiKlnResult, UranaiKlnPayment,
    UranaiKlnInsufficientCash};
const UranaiConfiguration uranaiMm5Ar01Configuration = {{177.89999389648438f, 100.34999847412109f, 314.20001220703125f}, 180.0f, 20, UranaiMm5Setup,
    UranaiMm5ModeOne, UranaiMm5Introduction, UranaiMm5Same, UranaiMm5Decline, UranaiMm5BeginReading, UranaiMm5Reading, UranaiMm5Result,
    UranaiMm5Payment, UranaiMm5InsufficientCash};
const UranaiConfiguration uranaiMm5Ws00Configuration = {{71.860000610351562f, 98.199996948242188f, 118.94000244140625f}, 180.0f, 20, UranaiMm5Setup,
    UranaiMm5ModeOne, UranaiMm5Introduction, UranaiMm5Same, UranaiMm5Decline, UranaiMm5BeginReading, UranaiMm5Reading, UranaiMm5Result,
    UranaiMm5Payment, UranaiMm5InsufficientCash};
const UranaiConfiguration uranaiMm5OtherConfiguration = {{293.35000610351562f, 100.0f, 153.72999572753906f}, 90.0f, 20, UranaiMm5Setup,
    UranaiMm5ModeOne, UranaiMm5Introduction, UranaiMm5Same, UranaiMm5Decline, UranaiMm5BeginReading, UranaiMm5Reading, UranaiMm5Result,
    UranaiMm5Payment, UranaiMm5InsufficientCash};
const UranaiConfiguration uranaiSehConfiguration = {{291.73001098632812f, 100.0f, 164.89999389648438f}, 0.0f, 20, UranaiSehSetup, UranaiSehModeOne,
    UranaiSehIntroduction, UranaiSehSame, UranaiSehDecline, UranaiSehBeginReading, UranaiSehReading, UranaiSehResult, UranaiSehPayment,
    UranaiSehInsufficientCash};
const UranaiConfiguration uranaiSriConfiguration = {{122.23000335693359f, 93.019996643066406f, 165.41999816894531f}, 100.0f, 20, UranaiSriSetup,
    UranaiSriModeOne, UranaiSriIntroduction, UranaiSriSame, UranaiSriDecline, UranaiSriBeginReading, UranaiSriReading, UranaiSriResult,
    UranaiSriPayment, UranaiSriInsufficientCash};
const UranaiConfiguration uranaiRcdConfiguration = {{332.67001342773438f, 126.98999786376953f, 290.32000732421875f}, 0.0f, 20, UranaiRcdSetup,
    UranaiRcdModeOne, UranaiRcdIntroduction, UranaiRcdSame, UranaiRcdDecline, UranaiRcdBeginReading, UranaiRcdReading, UranaiRcdResult,
    UranaiRcdPayment, UranaiRcdInsufficientCash};
static shen_name uranaiCharacters[] = {URNI_PLAYER, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID};
static UranaiSelection uranaiSelections[] = {{URNI_SYK, 2}, {URNI_KLN, 2}, {URNI_MM5, 2}, {URNI_SEH, 2}, {URNI_SRI, 2}, {URNI_RCD, 2}};

void AreaURNIMain(void *argument)
{
    QueueClearNpcs(QueueUranaiStart, argument);
}

void QueueUranaiStart(void *parameter)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(UranaiStart, 4, 11, sizeof(UranaiWork), TASK_EVNT);
    UranaiWork *work = (UranaiWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->selection = parameter ? (const UranaiSelection *)parameter : &uranaiSelections[g_entryPoint];
    InitializeUranai(work);
    EventDebugRegister(URNI_DEBUG_ID, UranaiDebugEnd, work, 2000000000);
}

void UranaiDebugEnd(void *data)
{
    UranaiEnd(data);
}

void UranaiEnd(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    CleanupUranai(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(URNI_DEBUG_ID);
}

void UranaiStart(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    const UranaiSelection *selection = work->selection;
    int delay;
    EventDebugCheck(URNI_DEBUG_ID, 4);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(URNI_PLAYER));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(URNI_PLAYER), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(URNI_PLAYER), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 0, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(URNI_PLAYER), 1, (void *)areaLoopHandPoses[URNI_GRIP_HAND_POSE], 0);
    ConfigureUranai(work, selection->reader);
    FTBL.AT_SetLocalBaseChara(URNI_PLAYER, URNI_BASE);
    FTBL.AT_SetLocalBaseChara(URNI_PLAYER_DUMMY, URNI_PLAYER);
    FTBL.AT_SetLocalBaseChara(selection->reader, URNI_BASE);
    FTBL.AT_SetLocalBaseChara(URNI_READER_DUMMY, selection->reader);
    EVT_AT_SetPos(URNI_PLAYER_DUMMY, URNI_PLAYER, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(URNI_PLAYER_DUMMY, URNI_PLAYER, 0.0f, 0);
    EVT_AT_SetPos(URNI_READER_DUMMY, selection->reader, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(URNI_READER_DUMMY, selection->reader, 0.0f, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (selection->mode == 1) {
        EVT_AT_SetSample(0, 0, 0);
        delay = work->configuration->modeOne(selection->reader, 0);
        FTBL.HLib_SetCurrentTaskTo(UranaiFade);
        FTBL.HLib_SetupTaskDelay(delay);
    } else {
        FTBL.EV_LoadAudio("01200000", 0);
        EVT_AT_SetSample(0, 0, 0);
        delay = work->configuration->introduction(selection->reader, 0);
        if (delay) {
            FTBL.FinishLoading();
            FTBL.CheckHelpIcon(1, 0, 0);
            EVT_AT_KsIcon_Enqueue(0xffff, 49, 154, 0xffff, 1800, 154, 2, delay);
            FTBL.HLib_SetCurrentTaskTo(UranaiSelect);
            FTBL.HLib_SetupTaskDelay(delay);
        } else
            FTBL.HLib_SetCurrentTaskTo(UranaiSame);
    }
}

void UranaiSelect(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    const UranaiSelection *selection = work->selection;
    const UranaiConfiguration *configuration = work->configuration;
    int delay;
    if (FTBL.GetShouldRenderIcons(2))
        return;
    FTBL.FinishLoading();
    FTBL.CheckHelpIcon(0, 0, 0);
    if (FTBL.shenGetCtrlSpeakIconFlag() == 49) {
        if (FTBL.GetPlayerStat(2) >= configuration->price) {
            delay = configuration->beginReading(selection->reader, 0);
            delay = configuration->reading(selection->reader, delay);
            FTBL.HLib_SetCurrentTaskTo(UranaiResult);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            delay = configuration->insufficientCash(selection->reader, 0);
            FTBL.HLib_SetCurrentTaskTo(UranaiFade);
            FTBL.HLib_SetupTaskDelay(delay - 60);
        }
    } else {
        delay = configuration->decline(selection->reader, 0);
        FTBL.HLib_SetCurrentTaskTo(UranaiFade);
        FTBL.HLib_SetupTaskDelay(delay - 60);
    }
}

void UranaiResult(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    const UranaiSelection *selection = work->selection;
    int delay = work->configuration->result(selection->reader, 0);
    if (delay < 0)
        FTBL.HLib_SetCurrentTaskTo(UranaiSame);
    else {
        FTBL.HLib_SetCurrentTaskTo(UranaiMoney);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

void UranaiSame(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    const UranaiSelection *selection = work->selection;
    int delay = work->configuration->same(selection->reader, 0);
    FTBL.HLib_SetCurrentTaskTo(UranaiFade);
    FTBL.HLib_SetupTaskDelay(delay - 60);
}

void UranaiMoney(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    const UranaiSelection *selection = work->selection;
    const UranaiConfiguration *configuration = work->configuration;
    int delay = configuration->payment(selection->reader, 0);
    EVT_AT_StartMoneyCount(-configuration->price, delay - 150);
    FTBL.HLib_SetCurrentTaskTo(UranaiFade);
    FTBL.HLib_SetupTaskDelay(delay - 60);
}

void UranaiFade(void *data)
{
    UranaiWork *work = (UranaiWork *)data;
    if (work->selection->mode != 1)
        EVT_AT_SetSample(0, 1, 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(UranaiFadeEnd);
    FTBL.HLib_SetupTaskDelay(60);
}

void UranaiFadeEnd(void *data)
{
    UranaiEnd(data);
}

void InitializeUranai(UranaiWork *work)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    uranaiCharacters[1] = work->selection->reader;
    work->characters = Queue_EVT_AT_ReadChr(work->task, uranaiCharacters);
    work->package = "SUISYO";
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"URNI", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "URNI", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "URNI", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0120");
    SetEventVoicePrefix("0120");
    if (work->selection->mode != 1) {
        FTBL.EV_SetNewSceneIndex();
        FTBL.EV_LoadCutsceneFromCurrentScene("01200000");
    }
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.EV_EventSetUpHelpIcon_0(54, 0);
}

void CleanupUranai(UranaiWork *work)
{
    FTBL.EV_EnableCharacterDisplay(URNI_PLAYER);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->selection->reader));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(URNI_PLAYER));
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    if (work->selection->mode != 1) {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        EVT_AT_SetSample(0, 0, 0);
    }
    FTBL.EV_FreeAuthorPak(0, "0120");
    FTBL.EV_ControlPackageResources(0, 6, "URNI", 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, work->package, 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, work->package, 0, 0);
    ET_AT_KillWork(work->characters);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}

void ConfigureUranai(UranaiWork *work, shen_name reader)
{
    switch (reader) {
    case URNI_SYK:
        work->configuration = &uranaiSykConfiguration;
        break;
    case URNI_KLN:
        work->configuration = &uranaiKlnConfiguration;
        break;
    case URNI_MM5:
        if (g_currentMap == URNI_AR01)
            work->configuration = &uranaiMm5Ar01Configuration;
        else if (g_currentMap == URNI_WS00)
            work->configuration = &uranaiMm5Ws00Configuration;
        else
            work->configuration = &uranaiMm5OtherConfiguration;
        break;
    case URNI_SEH:
        work->configuration = &uranaiSehConfiguration;
        break;
    case URNI_SRI:
        work->configuration = &uranaiSriConfiguration;
        work->package = UranaiStoryPackage();
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)work->package, 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case URNI_RCD:
        work->configuration = &uranaiRcdConfiguration;
        break;
    }
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(reader));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(reader), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(reader), 1, 1, 0);
    EVT_AT_SetPos(URNI_BASE, EVENT_CHARACTER_INVALID, work->configuration->position.x, work->configuration->position.y,
        work->configuration->position.z, 0);
    EVT_AT_SetAngY(URNI_BASE, EVENT_CHARACTER_INVALID, work->configuration->yaw, 0);
    EVT_AT_SetPos(uranaiCharacters[1], URNI_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(uranaiCharacters[1], URNI_BASE, 0.0f, 0);
    work->configuration->setup(reader, 0);
}
#else
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("URNI"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("URNI"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("URNI"), 0);
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
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(work->package));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
        sbtLoadProgram();
        EV_spmBeginSampleExecute();
    }
    EV_FreeAuthorPak(0, "0120");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("URNI"), 0);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
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
#endif
