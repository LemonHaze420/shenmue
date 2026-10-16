#include "s2/areas/area_ycpa.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include <bit>
namespace shenmue::s2::areas {
std::int32_t ycpaEvent{}, ycpaSavedEvent{}, ycpaPhase{}, ycpaPursuitSucceeded{};
const shen_name *ycpaCharacters{};
void *YcpaQueueRecord(HLib::TaskCallback callback, std::uint32_t size) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callback, 6, 0x0b, size, YCPA_TASK);
    RegisterEventTask(task);
    return HLib::GetTaskParameter(task);
}
void YcpaCamera(std::int32_t camera, shen_name eye, shen_name target, std::uint8_t context, float angle, std::int32_t delay) {
    if (delay) {
        EventSetCamera &cue = *static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue.cleanup = 1;
        cue.camera = camera;
        cue.eye = eye;
        cue.target = target;
        cue.context = context;
        cue.delay = static_cast<float>(delay);
        cue.angle = angle;
    } else {
        EventSetCamera cue;
        cue.cleanup = 0;
        cue.delay = 0;
        cue.camera = camera;
        cue.eye = eye;
        cue.target = target;
        cue.context = context;
        cue.angle = angle;
        EVT_AT_SetCam_Callback(&cue);
        camCtrlDisableFilmBars();
    }
}
void YcpaFilm(std::int32_t frames, std::int32_t startAlpha, std::int32_t startRed, std::int32_t startGreen, std::int32_t startBlue, std::int32_t targetAlpha,
    std::int32_t targetRed, std::int32_t targetGreen, std::int32_t targetBlue, std::int32_t delay) {
    const EventFilmControl cue{.cleanup = delay ? 1 : 0,
        .frames = frames,
        .startAlpha = startAlpha,
        .startRed = startRed,
        .startGreen = startGreen,
        .startBlue = startBlue,
        .targetAlpha = targetAlpha,
        .targetRed = targetRed,
        .targetGreen = targetGreen,
        .targetBlue = targetBlue,
        .delay = static_cast<float>(delay)};
    EVT_AT_MenCtrl(cue);
}
namespace {
constexpr shen_name departureCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, SHEN_NAME(BA6_), YCPA_INVALID};
constexpr shen_name cornerCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, SHEN_NAME(TB3_), YCPA_INVALID};
constexpr shen_name marketCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, SHEN_NAME(AM1_), YCPA_INVALID};
constexpr shen_name laneCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, SHEN_NAME(RYW_), YCPA_INVALID};
constexpr shen_name doorCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, SHEN_NAME(OB4_), YCPA_INVALID};
constexpr shen_name defaultCharacters[]{YCPA_RYO, YCPA_REN, YCPA_YUA, YCPA_INVALID};
void YcpaRootCallback(void *data);
void YcpaQueueMain(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(YcpaRootCallback, 4, 0x0b, 0x30, YCPA_TASK);
    YcpaWork &work = *static_cast<YcpaWork *>(HLib::GetTaskParameter(task));
    work.state = 0;
    work.reserved0a = 0;
    work.qteElapsed = 0;
    work.reserved10 = 0;
    work.finished = 0;
    work.reserved16 = 0;
    work.qteSucceeded = 0;
    work.reserved1c = 0;
    work.characters = nullptr;
    work.countdown = 0;
    work.reserved2c = 0;
    work.finished = 1;
    ycpaPhase = 0;
    work.state = 0;
    work.task = task;
    switch (ycpaEvent) {
    case 3:
        ycpaCharacters = departureCharacters;
        break;
    case 4:
        ycpaCharacters = cornerCharacters;
        break;
    case 6:
        ycpaCharacters = marketCharacters;
        break;
    case 7:
    case 8:
        ycpaCharacters = laneCharacters;
        break;
    case 9:
        ycpaCharacters = doorCharacters;
        break;
    default:
        ycpaCharacters = defaultCharacters;
        break;
    }
    work.characters = Queue_EVT_AT_ReadChr(task, ycpaCharacters);
}
void YcpaWaitStartupCallback(void *) {
    EV_debug_log("YCPA\\eYCPA_et_yuan_pa.c", 204, ".");
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    EV_debug_log("YCPA\\eYCPA_et_yuan_pa.c", 206, "\n");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, 0x20, YCPA_TASK);
    RegisterEventTask(task);
    StepNpcClearWork &work = *static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work.callback = YcpaQueueMain;
    work.callbackData = nullptr;
    work.reserved10 = 0;
    work.state = 0;
    work.delay = 3;
    HLib::DestroyCurrentTask();
}
void YcpaSetup() {
    eventMovementStates.fill(0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("YCPAMAIN"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YCPA"), 0);
    EV_FreeAuthorPak(1, "0235");
    if (ycpaEvent >= 100)
        EV_LoadCutsceneFromCurrentScene("0235E000");
    else {
        EV_LoadCutsceneFromCurrentScene("0235E999");
        EV_LoadAudio("0235E000", 0);
        EV_spmBeginSampleExecute();
    }
    qteBattleUtil(BattleQteMode::SetWait, 0, 6, 16);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 2048);
    qteBattleUtil(BattleQteMode::SetLastCommands, 0);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    if (DoesScenarioCharacterExist(YCPA_REN))
        SetLowQualityHands(FindCharacterByScenarioName(YCPA_REN));
    if (DoesScenarioCharacterExist(YCPA_YUA))
        SetLowQualityHands(FindCharacterByScenarioName(YCPA_YUA));
    for (shen_name character : {YCPA_RYO, YCPA_REN, YCPA_YUA}) {
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 1, 0);
    }
    for (shen_name character : {YCPA_RYO, YCPA_REN, YCPA_YUA}) {
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    }
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(YCPA_RYO), FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(YCPA_RYO), FacialExpressionId::Angry, 16, 0.800000011920928955f);
    for (shen_name character : {YCPA_BASE, YCPA_RYOB, YCPA_YUAB, YCPA_RENB, YCPA_RYOD})
        CreateEventCharacter(CharacterCreationHighModel, nullptr, character);
    ycpaPhase = 1;
}
std::int32_t YcpaDispatch(YcpaWork &work) {
    if (!EV_GetScreenFilmIsNotShowing())
        SetScreenFilmFade(45, 255, 0, 0, 0, 0, 0, 0, 0);
    ycpaPhase = 2;
    switch (ycpaEvent) {
    case 3:
        ycpaPhase = 15;
        work.countdown = YcpaElevatorDeparture();
        return 0;
    case 4:
        return YcpaStreetCorner(work);
    case 6:
        ycpaPhase = 15;
        work.countdown = YcpaMarketConversation();
        return 0;
    case 7:
    case 8:
        ycpaPhase = 15;
        work.countdown = YcpaLaneConversation();
        return 0;
    case 9:
        return YcpaDoorInterruption();
    case 10:
        YcpaQueuePursuit(work);
        return 0;
    case 11:
        ycpaPhase = 15;
        work.countdown = YcpaElevatorArrival();
        return 0;
    case 12:
        if (IsAuthorStateFlagSet(3))
            EV_SetAuthorActiveTo(true);
        YcpaQueueReturn();
        return 0;
    case 13:
        ycpaPhase = 15;
        work.countdown = YcpaDiscovery();
        return 0;
    case 14:
        ycpaPhase = 15;
        work.countdown = YcpaDiscoveryAgain();
        return 0;
    default:
        ycpaPhase = 15;
        work.countdown = YcpaBriefConversation();
        return 0;
    }
}
std::int32_t YcpaSkipFlag() {
    switch (ycpaEvent) {
    case 3:
        return 1;
    case 4:
        return 2;
    case 6:
        return 4;
    case 7:
    case 8:
        return 5;
    case 11:
        return 8;
    case 13:
        return 9;
    case 14:
        return 10;
    case 100:
        return 39;
    case 101:
        return 40;
    case 102:
        return 41;
    case 103:
        return 42;
    case 104:
        return 43;
    case 105:
        return 44;
    case 106:
        return 45;
    default:
        return -1;
    }
}
std::int32_t YcpaFinish(YcpaWork &work) {
    if (!work.finished) {
        EVT_AT_SetAngY(YCPA_YUA, YCPA_YUA, 180, 0);
        ycpaEvent = 14;
        work.finished = 1;
        ycpaPhase = 1;
        ycpaSavedEvent = 14;
        return 0;
    }
    std::int32_t delay = 0;
    if (EV_GetScreenFilmIsNotShowing()) {
        delay = 60;
        if (ycpaEvent != 13)
            SetScreenFilmFade(ycpaEvent < 100 ? 60 : 30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (ycpaEvent < 100)
            spmFadeOut();
    } else if (ycpaEvent == 10) {
        spmFadeOut();
        delay = 20;
    }
    HLib::SetCurrentTaskTo(YcpaStopChaseCallback);
    return delay;
}
void YcpaRootCallback(void *data) {
    YcpaWork &work = *static_cast<YcpaWork *>(data);
    std::int32_t delay = 0;
    switch (ycpaPhase) {
    case 0:
        YcpaSetup();
        [[fallthrough]];
    case 1:
        delay = YcpaDispatch(work);
        break;
    case 2:
        delay = YcpaFinish(work);
        break;
    case 15: {
        const std::int32_t flag = YcpaSkipFlag();
        if ((flag >= 0 && IsAuthorStateFlagSet(flag) && HasPressedCancel()) || authGetFinished()) {
            EV_CleanupMapControlTask();
            work.countdown = 0;
        }
        const std::int32_t previous = work.countdown;
        work.countdown = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(previous) - 1U);
        if (!previous)
            ycpaPhase = 2;
        break;
    }
    default:
        break;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void InitializeYcpa() {
    ycpaCharacters = nullptr;
    ycpaEvent = 0;
    ycpaSavedEvent = 0;
    ycpaPhase = 0;
    ycpaPursuitSucceeded = 0;
}
void RunYcpa(std::uintptr_t entry) {
    ycpaEvent = static_cast<std::int32_t>(entry);
    ycpaSavedEvent = ycpaEvent;
    SetPlayerControlEnabled(0);
    SetSoundListenerUsesCamera(1);
    SetScenarioFollowCharacter(shen_name{});
    if (ycpaEvent == 20 || ycpaEvent == 30)
        YcpaPrepareAdvice();
    else {
        HLib::EnqueueTaskWithoutParameter(YcpaWaitStartupCallback, 4, 0x0b, static_cast<std::int32_t>(YCPA_TASK.name));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 7U));
    }
}
} // namespace
const ScriptAreaDefinition ycpaScriptDefinition{.areaId = YCPA_AREA, .field04 = 0, .init = InitializeYcpa, .main = RunYcpa};
} // namespace shenmue::s2::areas
