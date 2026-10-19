#include "s2/areas/area_kick.hpp"
#include "s2/pause.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, yua{SHEN_NAME(YUA_)}, event{SHEN_NAME(EVNT)};
constexpr shen_name characters[]{SHEN_NAME(REN_), yua, shen_name{0xffffffffU}};
std::uint32_t state{};
std::int32_t delay{}, qteTicks{}, inputTicks{}, failedBefore{}, backBefore{}, frontBefore{}, timeoutBefore{};
DateTime savedTime{};
void QueueChoice() {
    QueueSkChoiceWait(HLib::GetCurrentTask(), 0, 231, 232, -1, -1, 0, 0);
}
void QueueAuthor(std::uint32_t command, HLib::TaskCallback callback = nullptr) {
    QueueSkAuthorStart(HLib::GetCurrentTask(), static_cast<std::int32_t>(command), callback, nullptr, 0);
}
void ResetPosition(shen_name character) {
    EventSetPositionAngle position{0, character, shen_name{0xffffffffU}, {}, 0.0f, 0.0f};
    EVT_AT_SetPosAng_Callback(&position);
}
bool InputPressed() {
    const shen_name input{SHEN_NAME(BTD2)};
    GetBattleDataConfig(0, input);
    return GetBattleDataConfig(1, input) && GetBattleDataConfig(2, input);
}

void ChoiceAuthor_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) != EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 1)
        return;
    if (qteTicks) {
        state = SHEN_NAME(COIN);
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        qteTicks = 0;
    } else {
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        QueueChoice();
        qteTicks = 1;
    }
}

void WaitInputAuthor_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) != EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 1)
        return;
    if (!qteTicks)
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
    EV_SetAuthorActiveTo(0);
    CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
    std::int32_t previous = qteTicks;
    if (InputPressed()) {
        state = SHEN_NAME(QTE_);
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        previous = 0;
    }
    qteTicks = previous + 1;
    if (previous > 1800) {
        state = SHEN_NAME(QTE_);
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        qteTicks = 0;
    }
}

void BattleAuthor_Callback(void *) {
    const auto remaining = EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    const auto result = qteBattleUtil(BattleQteMode::GetResult);
    if (result) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        state = result == 1 ? SHEN_NAME(OK__) : SHEN_NAME(NG__);
    }
    if (remaining - 5 == frame)
        qteBattleUtil(BattleQteMode::Start);
    if (remaining - 3 == frame)
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    if (GetCurrentAuthorId().name == SHEN_NAME(___2)) {
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 325)
            snapSetDesc(0);
    }
}

void Exit() {
    FinishLoading();
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    npcSetCycleTimeLimitExpiry(SHEN_NAME(cylf), 0);
    SetDoorSubjectEnabled(SHEN_NAME(R141), 1);
    EV_SetObjectFlagByIndex(31, 1);
    EventSetMotion motion{0, ryo, 0xf003, 0.0f, 1.0f, 0, 0.0f};
    EVT_AT_SetMot_Callback(&motion);
    EV_FreeAuthorPak(0, "0236");
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
    for (const shen_name *name = characters; name->name != 0xffffffffU; ++name) {
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KICKDOOR"), 0);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(QEDJ), 1);
}
} // namespace

void AreaKICKInit() {
    state = 0;
    delay = 0;
    qteTicks = 0;
    inputTicks = 0;
    failedBefore = 0;
    timeoutBefore = 0;
    backBefore = 0;
    frontBefore = 0;
}

void AreaKICKMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_SetRefBoardParamsFor(ryo, 1);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    qteBattleUtil(BattleQteMode::SetWait, 0, 6, 16);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 4, 256);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KICKDOOR"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0236A000");
    EV_LoadAudio("0236A000", 1);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0236");
    SetDoorSubjectEnabled(SHEN_NAME(R141), 0);
    EV_SetObjectFlagByIndex(31, 0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    auto *task = HLib::EnqueueTaskWithoutParameter(KICK_KickDoor_Callback, 6, 0x0b, SHEN_NAME(kick));
    state = SHEN_NAME(INIT);
    GetCurrentDateTime(savedTime);
    AssignGenericNpcLoadContext(characters, task);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, SHEN_NAME(load));
}

void KICK_KickDoor_Callback(void *) {
    if (delay > 0)
        --delay;
    switch (state) {
    case SHEN_NAME(INIT):
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        state = SHEN_NAME(AUT0);
        break;
    case SHEN_NAME(AUT0):
        SetCurrentDateTimeTo(savedTime);
        ResetPosition(SHEN_NAME(ab18));
        ResetPosition(SHEN_NAME(ab19));
        ResetPosition(yua);
        if (failedBefore)
            EV_SetAuthorActiveTo(1);
        QueueAuthor(SHEN_NAME(___0), ChoiceAuthor_Callback);
        state = SHEN_NAME(CINI);
        qteTicks = 0;
        break;
    case SHEN_NAME(CINI):
        EV_LoadAudio("0236A000", 0);
        EV_DisableCharacterDisplay(yua);
        QueueChoice();
        state = SHEN_NAME(COIN);
        break;
    case SHEN_NAME(COIN): {
        const auto choice = shenGetCtrlSpeakIconFlag();
        auto *previous = choice == 231 ? &frontBefore : choice == 232 ? &backBefore : &timeoutBefore;
        if (*previous)
            EV_SetAuthorActiveTo(1);
        QueueAuthor(choice == 231 ? SHEN_NAME(OMOT) : choice == 232 ? SHEN_NAME(URA_) : SHEN_NAME(TOUT));
        *previous = 1;
        state = SHEN_NAME(STRT);
        break;
    }
    case SHEN_NAME(STRT):
        if (failedBefore)
            EV_SetAuthorActiveTo(1);
        QueueAuthor(SHEN_NAME(___1), WaitInputAuthor_Callback);
        ResetPosition(yua);
        state = SHEN_NAME(WAIT);
        qteTicks = 0;
        break;
    case SHEN_NAME(WAIT): {
        std::int32_t previous = inputTicks;
        if (InputPressed()) {
            state = SHEN_NAME(QTE_);
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            previous = 0;
        }
        inputTicks = previous + 1;
        if (previous > 1800) {
            state = SHEN_NAME(QTE_);
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            inputTicks = 0;
        }
        break;
    }
    case SHEN_NAME(QTE_):
        EV_EnableCharacterDisplay(yua);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueAuthor(SHEN_NAME(___2), BattleAuthor_Callback);
        break;
    case SHEN_NAME(REQ_):
        state = SHEN_NAME(RES_);
        break;
    case SHEN_NAME(RES_): {
        const auto result = qteBattleUtil(BattleQteMode::GetResult);
        if (!result)
            return;
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (result == 1) {
            QueueAuthor(SHEN_NAME(OK__));
            state = SHEN_NAME(FADE);
        } else {
            if (failedBefore)
                EV_SetAuthorActiveTo(1);
            failedBefore = 1;
            QueueAuthor(SHEN_NAME(NG__));
            state = SHEN_NAME(AUT0);
        }
        break;
    }
    case SHEN_NAME(NG__):
        if (failedBefore)
            EV_SetAuthorActiveTo(1);
        QueueAuthor(SHEN_NAME(NG__));
        failedBefore = 1;
        state = SHEN_NAME(AUT0);
        break;
    case SHEN_NAME(OK__):
        QueueAuthor(SHEN_NAME(OK__));
        state = SHEN_NAME(FADE);
        break;
    case SHEN_NAME(FADE):
        delay = 30;
        spmFadeOut();
        state = SHEN_NAME(END_);
        break;
    case SHEN_NAME(END_):
        if (!delay)
            Exit();
        break;
    default:
        break;
    }
}
const ScriptAreaDefinition areaKICKScriptDefinition{.areaId = SHEN_NAME(KICK), .field04 = 0, .init = AreaKICKInit, .main = AreaKICKMain};
} // namespace shenmue::s2::areas
