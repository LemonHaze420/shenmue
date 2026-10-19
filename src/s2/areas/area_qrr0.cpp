#include "s2/areas/area_qrr0.hpp"
#include "s2/load_title.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sleep_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name sleepTask{SHEN_NAME(inn_)};
constexpr shen_name actors[]{SHEN_NAME(CLJ_), SHEN_NAME(REN_), SHEN_NAME(JOP_), SHEN_NAME(a358), shen_name{}};
std::int32_t waking;
std::int32_t sleepYaw;
SleepTriggerBox sleepBox;
constexpr SleepTriggerBox authoredSleepBox{{103.44999694824219f, 89.0f, 150.8699951171875f}, 110.62999725341797f,
    {89.0f, 150.8699951171875f, 103.44999694824219f, 90.0f}, 155.52999877929688f, {110.62999725341797f, 90.0f, 155.52999877929688f},
    {107.12000274658203f, 90.41000366210938f, 155.36000061035156f}, 0};
struct MainWork {
    std::uint64_t state;
    HLib::TaskCallback callback;
    std::uint64_t reserved[3];
};
static_assert(sizeof(MainWork) == 0x28);
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
    if (waking) {
        ST_LoadTitleFade();
        HLib::SetCurrentTaskTo(SleepBeginCallback);
    } else
        QueueSleepCallback(SleepBeginCallback, SleepCancelCallback, 505);
}
void SleepBeginCallback(void *) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    spmFadeOut();
    EV_FreeAuthorPak(1, "0204");
    if (waking)
        EV_LoadCutsceneFromCurrentScene("0204A000");
    QueueSleepPackageActors("SLEP", actors, waking ? WakeAuthorCallback : SleepAuthorCallback);
}


void SleepAuthorCallback(void *) {
    shen_name authors[]{SHEN_NAME(SLP0), SHEN_NAME(SLP1)};
    shen_name selected = authors[shenGetRandomInt32(2)];
    if (EV_GetShenmueFlag(0x45b))
        authors[0] = SHEN_NAME(SLPJ);
    else {
        if (EV_GetShenmueFlag(0x45c))
            selected = SHEN_NAME(SLPR);
        authors[0] = selected;
    }
    EV_LoadCutsceneFromCurrentScene("0204A000");
    sleepEventData.flags = 0;
    sleepEventData.continuation = SleepAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(1)];
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
    EV_AT_SetPos(SHEN_NAME(a358), EventTransformFlags::AllAxes, {-1000, -1000, -1000});
    SelectBadEndingState(WakeAuthorCallback);
}


void WakeAuthorCallback(void *) {
    shen_name authors[]{SHEN_NAME(WUP0), SHEN_NAME(WUP1)};
    shen_name selected = authors[shenGetRandomInt32(2)];
    if (EV_GetShenmueFlag(0x45d))
        authors[0] = SHEN_NAME(WUJ0);
    else if (EV_GetShenmueFlag(0x45e))
        authors[0] = SHEN_NAME(WUJ2);
    else {
        if (EV_GetShenmueFlag(0x45f))
            selected = SHEN_NAME(WUPR);
        authors[0] = selected;
    }
    if (EV_GetShenmueFlag(0x43e)) {
        const std::uint32_t nights = static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 9));
        EV_stepSetFlags(0, 0x43e);
        EV_EventSetUpHelpIcon_0(0x21, 0);
        const std::int32_t remaining = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)) - 38U * nights);
        SetPlayerStat(PlayerStatsId::Cash, remaining >= 0 ? static_cast<std::uint32_t>(remaining) : 0U);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 9, 0);
        SetHelpIconState();
    }
    sleepEventData.flags = 0x101;
    sleepEventData.continuation = WakeAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(1)];
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
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_npcFree(actors[0]);
    EV_npcFree(actors[1]);
    EV_npcFree(actors[2]);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {108.0199966430664f, 89.27999877929688f, 153.3f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 5880, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    sleepEventData.flags = 1;
    sleepEventData.continuation = SleepCompleteCallback;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    RefreshPadTask(2, 1);
    HLib::SetCurrentTaskTo(SleepMemoWaitCallback);
}
void SleepCompleteCallback(void *) {
    FreeSleepPackageActors();
    EV_LoadCutsceneFromCurrentScene("QRR0");
    BeginPrimarySampleExecution(0);
    BeginSecondarySampleExecution(0);
    EV_LoadAudio("QRR0_000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0204");
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
void MainCallback(void *) {
    if (ryoNotBusy() && shenTrySleepTrigger(&sleepBox)) {
        waking = 0;
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
    }
}
void PlaceReceptionActor(std::int32_t selection) {
    const shen_name marker{selection ? SHEN_NAME(rece) : SHEN_NAME(recw)};
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(marker, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(marker, &rotation, EventTransformFlags::World);
    EV_AT_SetPos(SHEN_NAME(rec_), EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(SHEN_NAME(rec_), EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EnableScenarioCharacterDisplay(SHEN_NAME(rec_));
    if (selection)
        EnableScenarioCharacterDisplay(SHEN_NAME(tap0));
    else
        DisableScenarioCharacterDisplay(SHEN_NAME(tap0));
}
} // namespace

void AreaQRR0Init() {
    waking = 0;
}
void AreaQRR0Cleanup(std::int32_t, std::uint32_t) {
    SetDefaultVS();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    CheckIsArgEquZero(1);
}
void AreaQRR0Main(std::uint32_t startParameters) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0xb, sizeof(MainWork), SHEN_NAME(EVNT));
    auto *work = static_cast<MainWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->callback = MainCallback;
    work->reserved[0] = work->reserved[1] = work->reserved[2] = 0;
    EV_GetShenmueFlag(0xff);
    AreaQRR0Cleanup(0, 0);
    sleepBox = authoredSleepBox;
    sleepYaw = 49152;
    BeginPrimarySampleExecution(0);
    BeginSecondarySampleExecution(0);
    if (!startParameters && (g_entryPoint == 70 || g_entryPoint == 80)) {
        areaFlags |= 1U;
        if (g_entryPoint == 70) {
            waking = 1;
            SetLoadingPhaseFlags(GetLoadingPhaseFlags() | 1U);
        } else
            waking = 0;
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::Task *sleep = HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
        HLib::SetupTimedTask(sleep, 5);
    }
    PlaceReceptionActor(EV_GetShenmueFlag(0x18d));
}
const GameplayAreaDefinition areaQRR0GameplayDefinition{SHEN_NAME(QRR0), 0, AreaQRR0Init, AreaQRR0Main, AreaQRR0Cleanup};
} // namespace shenmue::s2::areas
