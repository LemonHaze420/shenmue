#include "s2/areas/area_wrsq.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, base{SHEN_NAME(BASE)}, iii{SHEN_NAME(III_)}, see{SHEN_NAME(SEE_)}, sjn{SHEN_NAME(SJN_)};
constexpr shen_name say{SHEN_NAME(SAY_)}, gnb{SHEN_NAME(GNB_)}, myu{SHEN_NAME(MYU_)}, a244{SHEN_NAME(a244)}, a245{SHEN_NAME(a245)};
constexpr shen_name event{SHEN_NAME(EVNT)};
constexpr shen_name characterNames[]{ryo, say, iii, see, sjn, gnb, myu, shen_name{0xffffffffU}};
std::int16_t sequence{};
void Hand(std::int32_t side, std::int32_t *command, std::int32_t ticks, std::int32_t delay) {
    Queue_EVT_AT_CtrlHandUp_1(ryo, static_cast<std::uint32_t>(side), reinterpret_cast<std::byte *>(command), static_cast<std::uint32_t>(ticks), delay);
}
void Fade(std::int32_t frames, std::int32_t start, std::int32_t target) {
    SetScreenFilmFade(frames, start, 0, 0, 0, target, 0, 0, 0);
}
void Mot(shen_name name, std::uint32_t motion, float frame, float speed, std::int32_t delay) {
    (void)EVT_AT_SetMot(name, motion, frame, speed, 0x44000000U, delay);
}
void Cam(std::int32_t id, std::int32_t delay) {
    EVT_AT_SetCam(id, base, base, 0, 0, delay);
}
void Voice(const char *name, std::int32_t delay) {
    EVT_AT_SetVoice(name, 1, delay);
}
void TimedFade(std::int32_t delay) {
    EventFilmControl work{};
    work.frames = 30;
    work.targetAlpha = 255;
    work.delay = static_cast<float>(delay);
    EVT_AT_MenCtrl(work);
}
void SetPose(shen_name name, shen_name parent, const Vector3 &position, float yaw, std::int32_t delay) {
    EVT_AT_SetPos(name, parent, position, delay);
    EVT_AT_SetAngY(name, parent, yaw, delay);
}
std::int32_t SuccessA() {
    SetPose(ryo, base, {0, 0, 0}, 180, 0);
    SetPose(iii, ryo, {.1f, 0, -1.45f}, 0, 0);
    Mot(ryo, 0x1b4a, 20, 1, 0);
    Mot(iii, 0x1b44, 20, 1, 0);
    Mot(see, 0x1b46, 0, 0, 0);
    Mot(sjn, 0x1b42, 0, 0, 0);
    Cam(1000, 0);
    EVT_AT_SetFaceLooks(iii, FacialExpressionId::Suffer, 10, 1, 20);
    Voice("0045B012", 20);
    return 25;
}
std::int32_t SuccessB(std::int32_t delay) {
    SetPose(iii, base, {.1f, 0, 1.8f}, 180, delay);
    SetPose(see, ryo, {-1.1f, 0, -.3f}, 0, delay);
    Mot(ryo, 0x1b4b, 50, 1, delay);
    Mot(see, 0x1b46, 50, 1, delay);
    Cam(1001, delay);
    Voice("0045C004", delay);
    Voice("0045C007", delay + 40);
    return 60;
}
std::int32_t SuccessC(std::int32_t delay) {
    SetPose(see, base, {1.1f, 0, 1}, 180, delay);
    SetPose(sjn, ryo, {1.3f, 0, -.9f}, 0, delay);
    Mot(ryo, 0x1b4c, 130, 1, delay);
    Mot(sjn, 0x1b42, 130, 1, delay);
    Cam(1002, delay);
    Cam(1007, delay + 40);
    EVT_AT_SetFaceLooks(sjn, FacialExpressionId::Suffer, 10, 1, 20);
    Voice("0045E012", delay + 10);
    Voice("0045E013", delay + 35);
    return 80;
}
std::int32_t FailureA(std::int32_t delay) {
    SetPose(iii, ryo, {0, 0, -1.1f}, 0, delay);
    Mot(ryo, 0x1b47, 40, 1, delay);
    Mot(iii, 0x1b43, 40, 1, delay);
    Cam(1003, delay);
    Cam(1008, delay + 40);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, 10);
    Hand(0, ryoQteReactionHandPose, 8, 0);
    Hand(1, ryoQteReactionHandPose, 8, 0);
    Voice("0045B008", delay);
    Voice("0045B010", delay + 40);
    TimedFade(delay + 140);
    return 170;
}
std::int32_t FailureB(std::int32_t delay) {
    EVT_AT_SetPos(ryo, base, {.2f, 0, -.1f}, delay);
    SetPose(see, ryo, {-1.1f, 0, -.52999997f}, 0, delay);
    Mot(ryo, 0x1b48, 90, 1, delay);
    Mot(see, 0x1b45, 90, 1, delay);
    Cam(1004, delay);
    Cam(1005, delay + 40);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, delay + 40);
    Hand(0, ryoQteReactionHandPose, 8, delay);
    Hand(1, ryoQteReactionHandPose, 8, delay);
    Voice("0045C004", delay + 10);
    Voice("0045C003", delay + 40); // @text EN: "Take this!" | JA: "くらえっ！"
    TimedFade(delay + 120);
    return 150;
}
std::int32_t FailureC(std::int32_t delay) {
    SetPose(see, base, {1.1f, 0, 1}, 180, delay);
    SetPose(sjn, ryo, {.9f, 0, -.98000002f}, 0, delay);
    Mot(ryo, 0x1b49, 70, 1, delay);
    Mot(sjn, 0x1b41, 70, 1, delay);
    Cam(1006, delay);
    Cam(1009, delay + 35);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, delay + 10);
    Hand(0, ryoQteReactionHandPose, 8, delay);
    Hand(1, ryoQteReactionHandPose, 8, delay);
    Voice("0045E010", delay);
    Voice("0045E011", delay + 50);
    TimedFade(delay + 120);
    return 150;
}
std::int32_t Initialize(WrsqWork &work) {
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("WRSQ"));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    (void)EV_FreeAuthorPak(1, "0045");
    SetPose(base, shen_name{0xffffffffU}, {181.3f, 102.25f, 196.60001f}, 270, 0);
    (void)EV_LoadCutsceneFromCurrentScene("0045A000");
    (void)EV_LoadAudio("0045A000", 0);
    (void)EV_spmBeginSampleExecute();
    (void)qteBattleUtil(BattleQteMode::SetWait, 10, 6, 24);
    (void)qteBattleUtil(BattleQteMode::SetFirstCommands, 256, 1024, 512);
    (void)qteBattleUtil(BattleQteMode::SetLastCommands);
    (void)qteBattleUtil(BattleQteMode::ResetStates, 1);
    (void)qteBattleUtil(BattleQteMode::Load);
    (void)qteBattleUtil(BattleQteMode::Queue);
    work.phase = 1;
    (void)SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 1, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 481, 1);
    return 0;
}
std::int32_t Intro(WrsqWork &work) {
    if (work.step) {
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            (void)qteBattleUtil(BattleQteMode::Start);
            work.phase = 5;
        }
    } else {
        Fade(30, 255, 0);
        for (auto name : {a244, a245, gnb, myu})
            EV_EnableCharacterDisplay(name);
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(FIST)));
        ++work.step;
    }
    return 0;
}
std::int32_t Win(WrsqWork &work) {
    if (work.step)
        work.phase = 4;
    else {
        Fade(30, 255, 0);
        EV_EnableCharacterDisplay(gnb);
        EV_EnableCharacterDisplay(myu);
        QueueSkAuthorStart(work.self, static_cast<std::int32_t>(SHEN_NAME(WIN_)), nullptr, nullptr, 0);
        ++work.step;
        work.won = 1;
    }
    return 0;
}
std::int32_t AdvanceTime(WrsqWork &work) {
    std::int32_t delay = 0;
    switch (work.step) {
    case 0: {
        DateTime now{};
        GetCurrentDateTime(now);
        now.hour = static_cast<std::int8_t>(now.hour + 2);
        SetCurrentDateTimeTo(now);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        delay = 120;
        break;
    }
    case 1:
        ST_LoadTitleFade();
        delay = 30;
        break;
    case 2:
        Fade(30, 255, 0);
        (void)EV_LoadAudio("0045A000", 0);
        (void)EV_spmBeginSampleExecute();
        Cam(125, 0);
        (void)EVT_AT_SetMot(ryo, 0x1b4e, 200, 1, 0x40000000U, 0);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 6, 10, 1);
        EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Normal, 10, 1, 220);
        delay = 220;
        break;
    case 3:
        Fade(30, 0, 255);
        delay = 30;
        work.phase = 6;
        break;
    default:
        return 0;
    }
    ++work.step;
    return delay;
}
std::int32_t Battle(WrsqWork &work) {
    const auto result = qteBattleUtil(BattleQteMode::GetResult);
    if (!result)
        return 0;
    (void)EV_LoadAudio("0045A000", 1);
    for (auto name : {ryo, iii, see, sjn})
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(name), 1, 0, 1);
    for (auto name : {a244, a245, gnb, myu})
        EV_DisableCharacterDisplay(name);
    Hand(0, ryoQteHandPose, 0, 0);
    Hand(1, ryoQteHandPose, 0, 0);
    SetPose(ryo, base, {0, 0, 0}, 180, 0);
    SetPose(say, base, {-.2f, 0, -2.3f}, 180, 0);
    SetPose(iii, ryo, {.1f, 0, -1.4f}, 0, 0);
    SetPose(see, ryo, {-1.1f, 0, -.6f}, 0, 0);
    SetPose(sjn, ryo, {1.3f, 0, -.9f}, 0, 0);
    const auto count = qteBattleUtil(BattleQteMode::GetCompletedCount);
    std::int32_t delay = 0;
    if (count > 0)
        delay = SuccessA();
    if (count > 1)
        delay += SuccessB(delay);
    if (count > 2)
        delay += SuccessC(delay);
    if (count == 0)
        delay += FailureA(delay);
    else if (count == 1)
        delay += FailureB(delay);
    else if (count == 2)
        delay += FailureC(delay);
    sequence = static_cast<std::int16_t>((sequence + 1) % 4);
    work.phase = count == 3 ? 2 : 3;
    return delay;
}
} // namespace
// @verified - p
void WrsqInit() {
    sequence = 0;
}
void WrsqMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    if (g_currentMap.name == AREA_WRSQ.name)
        Fade(1, 0, 255);
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), event);
    RegisterEventTask(task);
    auto *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueWrsq;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
void QueueWrsq(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WrsqMainCallback, 5, 0x0b, sizeof(WrsqWork), event);
    auto *work = static_cast<WrsqWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->self = task;
    work->characters = Queue_EVT_AT_ReadChr(task, characterNames);
}
// @verified - p
void WrsqLoseAuthorCallback(void *) {
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 1)
        hmdlSetFlagsFor(FindCharacterByScenarioName(sjn), 0, 1, 0);
    else if (frame == 180)
        hmdlSetFlagsFor(FindCharacterByScenarioName(sjn), 0, 8, 16);
}
// @verified - p
void WrsqMainCallback(void *data) {
    auto &work = *static_cast<WrsqWork *>(data);
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        delay = Initialize(work);
        break;
    case 1:
        delay = Intro(work);
        break;
    case 2:
        EV_EnableCharacterDisplay(gnb);
        EV_EnableCharacterDisplay(myu);
        QueueSkAuthorStart(work.self, static_cast<std::int32_t>(SHEN_NAME(LOSE)), WrsqLoseAuthorCallback, nullptr, 0);
        work.won = 0;
        work.phase = 6;
        return;
    case 3:
        delay = Win(work);
        break;
    case 4:
        delay = AdvanceTime(work);
        break;
    case 5:
        delay = Battle(work);
        break;
    case 6:
        if (g_currentMap.name == AREA_WRSQ.name)
            work.phase = 1;
        else {
            spmFadeOut();
            HLib::SetCurrentTaskTo(WrsqEndCallback);
        }
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void WrsqEndCallback(void *data) {
    auto &work = *static_cast<WrsqWork *>(data);
    EV_stepSetFlags(work.won != 0, 0x4f);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
    SetCharaKill(base, 1);
    ET_AT_KillWork(work.characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    FinishLoading();
    (void)EV_FreeAuthorPak(0, "0045");
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)LoadCutsceneAndSoundProgramForArea();
    (void)EV_spmBeginSampleExecute();
    (void)EV_NPC_NewLoadSw(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 481, 0);
    scenFreeEvent("WRSQ.EV2");
    SetPose(ryo, shen_name{0xffffffffU}, {180.399994f, 102.25f, 195}, 180, 0);
    Fade(30, 255, 0);
}
const ScriptAreaDefinition wrsqScriptDefinition{.areaId = AREA_WRSQ, .field04 = 0, .init = WrsqInit, .main = WrsqMain};
} // namespace shenmue::s2
