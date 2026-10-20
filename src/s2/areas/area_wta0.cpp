#include "s2/areas/area_wta0.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sleep_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name room{SHEN_NAME(0001)};
constexpr shen_name sleepTask{SHEN_NAME(inn_)};
std::int32_t initializing;
std::int32_t sleepYaw;
SleepTriggerBox sleepBox;
constexpr SleepTriggerBox authoredSleepBox{{229.47000122070312f, 111.0f, 225.99000549316406f}, 233.5f,
    {111.0f, 225.99000549316406f, 229.47000122070312f, 112.0f}, 230.97999572753906f, {233.5f, 112.0f, 230.97999572753906f},
    {229.2100067138672f, 112.0f, 227.5500030517578f}, 2};
constexpr shen_name sleepActors[]{SHEN_NAME(KUN_), SHEN_NAME(SYP_), SHEN_NAME(SYX_), SHEN_NAME(SYE_), shen_name{}};
void SleepBeginCallback(void *);
void SleepAuthorCallback(void *);
void SleepAfterAuthorCallback(void *);
void WakeAuthorCallback(void *);
void WakeAfterAuthorCallback(void *);
void SleepCompleteCallback(void *);

void SleepCancelCallback(void *) {
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
void SleepRunCallback(void *) {
    QueueSleepCallback(SleepBeginCallback, SleepCancelCallback, 503);
}
void SleepAdmissionCallback(void *) {
    if (GetNpcNewLoadState())
        HLib::SetCurrentTaskTo(SleepRunCallback);
}
void SleepBeginCallback(void *) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    QueueSleepPackageActors("SLEP", sleepActors, SleepAuthorCallback);
}


void SleepAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(SLP0), SHEN_NAME(SLP1), SHEN_NAME(SLP2)};
    EV_LoadCutsceneFromCurrentScene("0033A010");
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0033");
    EnsureScenarioCharacterDraw(SHEN_NAME(HOUK));
    EnableScenarioCharacterDisplay(SHEN_NAME(HOUK));
    sleepEventData.flags = 0;
    sleepEventData.continuation = SleepAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if ((sleepEventData.flags & 1U) != 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if ((sleepEventData.flags & 0x40U) == 0) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      static_cast<AuthSceneCtrl>(-16));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}
void SleepAfterAuthorCallback(void *) {
    SelectBadEndingState(WakeAuthorCallback);
}


void WakeAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(WUP0), SHEN_NAME(WUP1), SHEN_NAME(WUP2)};
    AreaWTA0Cleanup(0, 0);
    sleepEventData.flags = 0;
    sleepEventData.continuation = WakeAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if ((sleepEventData.flags & 1U) != 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if ((sleepEventData.flags & 0x40U) == 0) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      static_cast<AuthSceneCtrl>(-16));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}
void WakeAfterAuthorCallback(void *) {
    using enum CameraState;
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {230.75999450683594f, 111.69999694824219f, 226.75f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 63192, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    EV_FreeAuthorPak(0, "0033");
    if (EV_GetShenmueFlag(0x2a)) {
        DateTime date;
        GetCurrentDateTime(date);
        date.hour = 8;
        date.minute = 45;
        SetCurrentDateTimeTo(date);
        EV_stepSetFlags(0, 0x2a);
        QueueSleepSceneChange(SHEN_NAME(0034), 1);
        HLib::DestroyCurrentTask();
    } else if (EV_GetShenmueFlag(0x29)) {
        DateTime date;
        GetCurrentDateTime(date);
        date.hour = 9;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        QueueSleepSceneChange(SHEN_NAME(WBBK), 0);
        HLib::DestroyCurrentTask();
    } else {
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        sleepEventData.flags = 1;
        sleepEventData.continuation = SleepCompleteCallback;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        RefreshPadTask(2, 1);
        HLib::SetCurrentTaskTo(SleepMemoWaitCallback);
    }
}
void SleepCompleteCallback(void *) {
    FreeSleepPackageActors();
    EV_LoadCutsceneFromCurrentScene("WTA0");
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
void MainCallback(void *) {
    if (ryoNotBusy() && shenTrySleepTrigger(&sleepBox)) {
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
    }
    Vector3i rotation{0, 65172, 0};
    EV_scenApplyPosOrRotToPartOf(room, 152, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
    EV_scenApplyPosOrRotToPartOf(room, 153, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
}
} // namespace

void AreaWTA0Init() {
    initializing = 0;
}
void AreaWTA0Cleanup(std::int32_t afternoon, std::uint32_t) {
    SetDefaultVS();
    EV_SetObjectFlagByIndex(1, afternoon ? 0 : 1);
    EV_SetObjectFlagByIndex(2, afternoon ? 1 : 0);
    EV_lightLoad(shen_name{afternoon ? SHEN_NAME(0001) : SHEN_NAME(0000)});
    AssignLightProgram(0);
    AssignLightProgram(1);
    AssignLightProgram(2);
    AssignLightProgram(3);
    AssignLightProgram(5);
    CheckIsArgEquZero(1);
    EV_AssignLightParams(1, 4);
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("WTA0");
        EV_LoadAudio("WTA0_000", 0);
    }
    Vector3 position{};
    EV_scenApplyPosOrRotToPartOf(room, 155, &position, nullptr, EventCharacterPartTransformMode::Set);
}
void AreaWTA0Main(std::uint32_t startParameters) {
    initializing = 1;
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(main)));
    AreaWTA0Cleanup(EV_GetShenmueFlag(0xff) != 0, 0);
    BeginPrimarySampleExecution(0);
    BeginSecondarySampleExecution(0);
    sleepBox = authoredSleepBox;
    sleepYaw = 49152;
    if (!startParameters && g_entryPoint == 80) {
        EV_setHoldStatesFlags(0);
        areaFlags |= 1U;
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(SleepAdmissionCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
        HLib::SetupTimedTask(task, 10);
    }
    initializing = 0;
}
const GameplayAreaDefinition areaWTA0GameplayDefinition{SHEN_NAME(WTA0), 0, AreaWTA0Init, AreaWTA0Main, AreaWTA0Cleanup};
} // namespace shenmue::s2::areas
