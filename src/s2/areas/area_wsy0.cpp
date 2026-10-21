#include "s2/areas/area_wsy0.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sleep_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name bike{SHEN_NAME(bike)};
constexpr shen_name sleepTask{SHEN_NAME(inn_)};
constexpr Vector4 nightLighting{.2f, .1f, .1f, .14f};
constexpr Vector4 storyWakeLighting{.2f, .6f, .6f, .6f};
constexpr Vector4 wakeLighting{.2f, .1f, .1f, .1f};
constexpr shen_name sleepActors[]{SHEN_NAME(JOP_), shen_name{}};
struct SearchTargetMask {
    std::uint32_t name, mask;
};
constexpr SearchTargetMask excludedSearchTargets[]{{0x00425645, 0x00ffffff}, {}};
constexpr std::uint32_t searchTargets[]{0x00425645, 0x3730414d, 0x3830414d, 0x3930414d, 0x3031414d, 0x3131414d, 0x3330344d, 0x3231394d, 0x3230394d, 0x3130384d,
    0x3530314d, 0x3630324d, 0x3531364d, 0x3430344d, 0};
constexpr SleepTriggerBox authoredSleepBox{{60.76530075073242f, 101.0f, 118.20600128173828f}, 63.95090103149414f,
    {101.0f, 118.20600128173828f, 60.76530075073242f, 103.25830078125f}, 123.14399719238281f, {63.95090103149414f, 103.25830078125f, 123.14399719238281f},
    {63.099998474121094f, 101.80999755859375f, 118.63999938964844f}, 0};
SleepTriggerBox sleepBox;
std::int32_t insideBedroom;
std::int32_t initializing;
std::int32_t lightFrames;
std::int32_t sleepYaw;
Vector3 switchPreviousPosition;
struct MainWork {
    std::uint64_t state;
    HLib::TaskCallback callback;
    std::uint64_t reserved[3];
};
static_assert(sizeof(MainWork) == 0x28);
void SleepPackageCallback(void *);
void SleepAuthorCallback(void *);
void SleepAfterAuthorCallback(void *);
void WakeAuthorCallback(void *);
void WakeAfterAuthorCallback(void *);
void SleepCompleteCallback(void *);
void SwitchBeginCallback(void *);
void SwitchToggleCallback(void *);
void SwitchRestoreCallback(void *);

void UpdateBike() {
    if (!EV_GetShenmueFlag(0x11) || EV_GetShenmueFlag(0x430)) {
        if (DoesScenarioCharacterExist(bike)) {
            DisableScenarioCharacterDisplay(bike);
            ToggleScenarioCharacterFlag(bike);
            ToggleFlagOnFieldEntity(bike, 0);
        }
    } else if (DoesScenarioCharacterExist(bike)) {
        EnsureScenarioCharacterDraw(bike);
        EnableScenarioCharacterDisplay(bike);
        ToggleFlagOnFieldEntity(bike, 1);
    }
}
void ApplyStreetLighting(std::int32_t night) {
    SetDefaultVSByIndex(1);
    if (night) {
        EV_lightLoad(SHEN_NAME(0001));
        UpdateGlobalLighting(nightLighting);
        AssignLightProgram(1);
    } else {
        EV_AssignLightParams(1, 1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    }
    EV_SetObjectFlagByIndex(3, night ? 1 : 0);
    UpdateBike();
}
void ApplyCurrentStreetLighting(std::uint32_t *) {
    ApplyStreetLighting(EV_GetShenmueFlag(0xff) != 0);
}
// @verified - p
void SetBedroomSearchTargets(std::int32_t enabled) {
    for (const std::uint32_t *target = searchTargets; *target; ++target) {
        const SearchTargetMask *excluded = excludedSearchTargets;
        while (excluded->name && excluded->name != (*target & excluded->mask))
            ++excluded;
        if (!excluded->name)
            srchSetTargetEnabled(shen_name{*target}, enabled);
    }
}
void CheckBedroom() {
    const Vector3 first{64.377602f, 101.0f, 118.2098f};
    const Vector3 second{60.815498f, 102.0f, 123.1519f};
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (TestCharaPositionWithinRange(ryo, &first, &second, shen_name{}) && position.y > first.y && second.y > position.y) {
        if (insideBedroom != 1) {
            CameraUtilityParameters parameters{1, 0};
            camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
            SetBedroomSearchTargets(1);
            insideBedroom = 1;
        }
    } else if (insideBedroom) {
        CameraUtilityParameters parameters{0, 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
        SetBedroomSearchTargets(0);
        EnableScenarioCharacterDisplay(ryo);
        insideBedroom = 0;
    }
}
void StreetLightCallback(void *) {
    Vector4 colour = nightLighting;
    if (lightFrames < 100 && (lightFrames & 3) == 0) {
        const float fraction = static_cast<float>(lightFrames) / 100.0f;
        const float green = (1.0f - fraction) * .16f + (1.0f - fraction) * .16f;
        colour.z = ((1.0f - fraction) * .14f + (1.0f - fraction) * .14f) + fraction * .1f;
        colour.y = fraction * .1f + green;
        colour.w = fraction * .14f + green;
        SetDefaultVSByIndex(1);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        EV_SetIndexForLightEntry(12, 1 - static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1120)));
        UpdateGlobalLighting(colour);
        CheckIsArgEquZero(1);
        AssignLightProgram(1);
    }
    if (lightFrames == 100) {
        EV_lightLoad(SHEN_NAME(0001));
        UpdateGlobalLighting(colour);
        AssignLightProgram(1);
        CheckIsArgEquZero(1);
        EV_SetIndexForLightEntry(12, 1 - static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1120)));
        HLib::DestroyCurrentTask();
    }
    ++lightFrames;
}
void SleepCancelCallback(void *) {
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
void SleepRunCallback(void *) {
    EV_NPC_NewLoadSw(0);
    PrepareNpcAllFromArray();
    CameraUtilityParameters parameters{1, 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
    QueueSleepCallback(SleepPackageCallback, SleepCancelCallback, 502);
}
void SleepPackageCallback(void *) {
    QueueSleepPackageActors("SLEP", sleepActors, SleepAuthorCallback);
}


void SleepAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(SLP0), SHEN_NAME(SLP1), SHEN_NAME(SLP2)};
    EV_LoadCutsceneFromCurrentScene("0016A000");
    EV_spmBeginSampleExecute();
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
    SetDefaultVSByIndex(1);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    EV_SetObjectFlagByIndex(3, 0);
    UpdateBike();
    SelectBadEndingState(WakeAuthorCallback);
}

void WakeAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(WUP0), SHEN_NAME(WUP1), SHEN_NAME(WUP2)};
    if (EV_GetShenmueFlag(0x430)) {
        EV_FreeAuthorPak(1, "0016");
        SetDisplayFlagForProp(SHEN_NAME(M912), 0);
        SetNpcColdResetMode(0);
        UpdateGlobalLighting(storyWakeLighting);
        sleepEventData.flags = 0x80;
        sleepEventData.continuation = WakeAfterAuthorCallback;
        constexpr shen_name special[]{SHEN_NAME(1ST_)};
        sleepCurrentAuthor = special[shenGetRandomInt32(1)];
    } else {
        sleepEventData.flags = 0;
        sleepEventData.continuation = WakeAfterAuthorCallback;
        sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    }
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
    UpdateGlobalLighting(wakeLighting);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {62.599998f, 101.35f, 121.5f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 57344, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    FreeSleepPackageActors();
    if (EV_GetShenmueFlag(0x11)) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(SleepCompleteCallback);
        HLib::SetupTaskDelay(32);
    } else {
        sleepEventData.flags = 1;
        sleepEventData.continuation = SleepCompleteCallback;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        RefreshPadTask(2, 1);
        HLib::SetCurrentTaskTo(SleepMemoWaitCallback);
    }
}
void SleepCompleteCallback(void *) {
    EV_LoadCutsceneFromCurrentScene("WSY0");
    EV_spmBeginSampleExecute();
    if (EV_GetShenmueFlag(0x430)) {
        EV_stepSetFlags(0, 0x430);
        EV_FreeAuthorPak(0, "0016");
    }
    SetDisplayFlagForProp(SHEN_NAME(M912), 1);
    const std::int32_t nights = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 9));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 9, nights + 1);
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    HLib::DestroyCurrentTask();
}
void MoveCompleteCallback(void *) {
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void MoveRyoCallback(void *) {
    using enum CameraState;
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {64.900002f, 98.199997f, 120.2f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 14563, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 17, 0);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
    InitiateCleanupForSceneChara(SHEN_NAME(TNZ_));
    SetPlayerGameFlags(PlayerGameFlagMode::DisableAreaControl, nullptr);
    HLib::SetCurrentTaskTo(MoveCompleteCallback);
    HLib::SetupTaskDelay(20);
}
void QueueMoveRyo() {
    if (!ryoNotBusy())
        return;
    std::int32_t move = 0;
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (EV_GetShenmueFlag(0x48b) && position.x > 63.5f && position.x < 66.3f && position.z > 121.5f && static_cast<double>(position.z) < 123.1 &&
        static_cast<double>(position.y) > 98.1 && static_cast<double>(position.y) < 98.3) {
        EV_stepSetFlags(0, 0x48b);
        move = 1;
    }
    if (EV_GetShenmueFlag(0x48c))
        EV_stepSetFlags(0, 0x48c);
    else if (move != 1) {
        if (EV_GetShenmueFlag(0x48a)) {
            EV_stepSetFlags(0, 0x48a);
            InitiateCleanupForSceneChara(SHEN_NAME(TNZ_));
        }
        return;
    }
    SetPlayerControlEnabled(0);
    SetPlayerGameFlags(PlayerGameFlagMode::EnableAreaControl, nullptr);
    SetScreenFilmFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(MoveRyoCallback, 6, 0xd, static_cast<std::int32_t>(SHEN_NAME(tnrm)));
    HLib::SetupTimedTask(task, 20);
    if (EV_GetShenmueFlag(0x48a)) {
        EV_stepSetFlags(0, 0x48a);
        InitiateCleanupForSceneChara(SHEN_NAME(TNZ_));
    }
}
void SwitchBeginCallback(void *) {
    EVS_LoopCamera(1300, ryo, ryo);
    EnableScenarioCharacterDisplay(ryo);
    HLib::SetCurrentTaskTo(SwitchToggleCallback);
    HLib::SetupTaskDelay(30);
}
void SwitchToggleCallback(void *) {
    const std::int32_t light = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1120));
    SetDefaultVSByIndex(2);
    SetDefaultVS();
    EV_SetIndexForLightEntry(12, light == 1 ? 1 : 0);
    AssignLightProgram(2);
    SetupLightConfig();
    Loop3DSoundEffect(0xa45);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1120, light ^ 1);
    HLib::SetCurrentTaskTo(SwitchRestoreCallback);
    HLib::SetupTaskDelay(60);
}
void SwitchRestoreCallback(void *) {
    using enum CameraState;
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y = switchPreviousPosition.y;
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(FindMotionWork(FindCharacterByScenarioName(ryo)), 0);
    SetMotionFrameTime(FindMotionWork(FindCharacterByScenarioName(ryo)), 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 17, 0);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    UpdateMotionWorkWithPosition(FindMotionWork(FindCharacterByScenarioName(ryo)), nullptr, 0);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void SwitchCallback(std::uint32_t *) {
    SetPlayerControlEnabled(0);
    Vector3 location;
    location.z = .68924999f;
    Vector3 offset{-.35218000411987305f, -1.2051199674606323f, .68924999f};
    Vector3i rotation{0, 50973, 0};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &switchPreviousPosition, EventTransformFlags::Local);
    RetrieveSearchFlagFromPtr(location);
    UpdateMotionWorkWithPosition(FindMotionWork(FindCharacterByScenarioName(ryo)), &location, 1);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    ApplyScenarioTranslationAndRotation(&location, &rotation, &offset);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    SetMotionFrameTime(FindMotionWork(FindCharacterByScenarioName(ryo)), 0, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(FindMotionWork(FindCharacterByScenarioName(ryo)), 0);
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0x2d03, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(FindMotionWork(FindCharacterByScenarioName(ryo)), 60, MotionRegion::LowerBody);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(FindMotionWork(FindCharacterByScenarioName(ryo)), 0);
    SetMotionFrameTime(FindMotionWork(FindCharacterByScenarioName(ryo)), 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 17, 0);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, offset);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, rotation);
    DisableScenarioCharacterDisplay(ryo);
    HLib::EnqueueTaskWithoutParameter(SwitchBeginCallback, 4, 0xd, static_cast<std::int32_t>(SHEN_NAME(wsys)));
    HLib::SetupTaskDelay(1);
}
void MainCallback(void *) {
    if (ryoNotBusy()) {
        if (shenTrySleepTrigger(&sleepBox)) {
            SetPlayerControlEnabled(0);
            EV_setHoldStatesFlags(0);
            mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
            HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
        }
        CheckBedroom();
    }
    const Vector3i rotation{57344, 1092, 0};
    EV_AT_SetAngY(SHEN_NAME(fun0), EventTransformFlags::Relative | EventTransformFlags::Y, rotation);
    EV_AT_SetAngY(SHEN_NAME(fun2), EventTransformFlags::Relative | EventTransformFlags::X, rotation);
    QueueMoveRyo();
    SetScenarioCharacterDrawControl(SHEN_NAME(RCKO), 2, 0);
}
} // namespace

void AreaWSY0Init() {
    insideBedroom = 0;
    initializing = 0;
    lightFrames = 0;
    switchPreviousPosition = {};
}
void AreaWSY0Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        HLib::EnqueueTaskWithoutParameter(StreetLightCallback, 4, 0xd, static_cast<std::int32_t>(SHEN_NAME(wsys)));
        lightFrames = 0;
    } else
        ApplyStreetLighting(0);
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("WSY0");
        EV_LoadAudio("WSY0_000", 0);
    }
}
void AreaWSY0Main(std::uint32_t startParameters) {
    initializing = 1;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0xb, sizeof(MainWork), SHEN_NAME(EVNT));
    auto *work = static_cast<MainWork *>(HLib::GetTaskParameter(task));
    insideBedroom = -1;
    work->state = 0;
    work->callback = MainCallback;
    work->reserved[0] = work->reserved[1] = work->reserved[2] = 0;
    areaBaseCallbacks[0] = SwitchCallback;
    areaBaseCallbacks[1] = ApplyCurrentStreetLighting;
    ApplyCurrentStreetLighting(nullptr);
    EV_SetWeatherInstruction(0x780, 1);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(2);
    SetupLightConfig();
    SetScenarioCharacterDrawControl(ryo, 2, 0);
    const bool light = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1120) != 0;
    SetDefaultVSByIndex(2);
    SetDefaultVS();
    EV_SetIndexForLightEntry(12, light ? 0 : 1);
    AssignLightProgram(2);
    SetupLightConfig();
    AddInventoryItem(0x45, 1, 0);
    sleepBox = authoredSleepBox;
    sleepYaw = 49152;
    CheckBedroom();
    if (!startParameters && g_entryPoint == 79) {
        areaFlags |= 1U;
        SetSceneInfos(g_actNumber, g_currentMap, 1);
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::Task *sleep = HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
        HLib::SetupTimedTask(sleep, 10);
    }
    initializing = 0;
}
const GameplayAreaDefinition areaWSY0GameplayDefinition{SHEN_NAME(WSY0), 0, AreaWSY0Init, AreaWSY0Main, AreaWSY0Cleanup};
} // namespace shenmue::s2::areas
