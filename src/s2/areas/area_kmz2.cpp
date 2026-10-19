#include "s2/areas/area_kmz2.hpp"
#include "s2/areas/area_maze_events.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/maze.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, shenhua{SHEN_NAME(SIN_)};
constexpr shen_name navigationTask{TASK_NAVI_START}, eventTask{SHEN_NAME(bomS)}, loadTask{SHEN_NAME(load)};
constexpr shen_name loadCharacters[]{shenhua, shen_name{0xffffffffU}};
struct MazeEventParameters {
    std::int32_t state;
    Vector3 position;
    std::uint32_t yaw;
};
static_assert(sizeof(MazeEventParameters) == 20);
static_assert(offsetof(MazeEventParameters, position) == 4);
std::int32_t resumed;
std::uint64_t forceEvent;
MazeEventParameters eventParameters;

void EventBombCallback(void *);
void AfterQteExitCallback(void *);
void TimeOverSetupCallback(void *);
void TimeOverExitCallback(void *);

void ResetActorMotion(shen_name actor, std::uint16_t motion) {
    mtwkSetExtraFlag(scenFindMotionwork(actor), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(actor), motion, MotionRegion::LowerBody);
}
void AssignEventParameters(Vector3 position, std::int32_t state, std::uint32_t yaw) {
    shenToggleAreaFlags(0);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    EV_PTYCUtils(shenhua, 3, 1);
    ResetActorMotion(shenhua, 0x9502);
    eventParameters.state = state;
    eventParameters.position = position;
    const float height = shenFindIntersectionWithScene(eventParameters.position, eventParameters.position.y);
    eventParameters.yaw = yaw;
    eventParameters.position.y = height;
}
void DispatchMazeEvent() {
    ExecuteCurrAreaMainFunc_Wrapper(2, reinterpret_cast<std::uintptr_t>(&eventParameters));
}
void LoadCompanion(HLib::Task *continuation) {
    AssignAreaNpcLoadContext(loadCharacters, continuation);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (continuation)
        HLib::SetupTimedTask(continuation, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, loadTask.name);
}
void AfterQteSetupCallback(void *) {
    AssignEventParameters({-500.1000061035156f, 100.0f, -297.7250061035156f}, 21, 0x71c);
    DispatchMazeEvent();
    QueueImmediateNextFadeTask(nullptr);
    HLib::SetCurrentTaskTo(AfterQteExitCallback);
}
void AfterQteExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-501.8299865722656f, 500.0f, -302.9551086425781f};
    const Vector3i companionRotation{0, 63351, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-501.2130126953125f, 500.0f, -305.20001220703125f};
    const Vector3i playerRotation{0, 1491, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 4);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(shenhua), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(shenhua), 0, 1, 0);
    camCtrlUtil(static_cast<CameraUtilityMode>(0), 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    InitiateCleanupForSceneChara(shenhua);
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void EnterForest3Callback(void *) {
    shenToggleAreaFlags(1);
    EV_stepSetFlags(0, 576);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(KMZ3), 0);
}
std::int64_t CheckAreaEvent() {
    if (!IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), 1, 2.0f) || !EV_GetShenmueFlag(576))
        return 0;
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(0);
    HLib::SetCurrentTaskTo(EnterForest3Callback);
    if (!forceEvent) {
        spmFadeOut();
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(60);
    }
    forceEvent = (forceEvent & 0xffffffff00000000ULL);
    return 1;
}
void TimeOverLockcheckCallback(void *) {
    HLib::SetCurrentTaskTo(TimeOverSetupCallback);
    if (!DoesScenarioCharacterExist(shenhua)) {
        HLib::SetupTaskDelay(0);
        LoadCompanion(HLib::GetCurrentTask());
    }
}
std::int64_t CheckTimeOver() {
    if (!EV_GetShenmueFlag(0x454))
        return 0;
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        if (dateTime.hour < 16 || dateTime.minute < 30)
            return 0;
    } else if (dateTime.hour < 17)
        return 0;
    if (!shenGetIsPlayerInMenu())
        return 0;
    srchSetLookMode();
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(0);
    HLib::SetCurrentTaskTo(TimeOverLockcheckCallback);
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(30);
    return 1;
}
void EventBombCallback(void *) {
    if ((forceEvent || ryoNotBusy()) && !CheckAreaEvent())
        CheckTimeOver();
}
void TimeOverSetupCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(MazeTimeOverToForest4Callback);
    } else {
        AssignEventParameters({-502.0f, 100.0f, -401.0f}, 32, 43690);
        DispatchMazeEvent();
        HLib::SetCurrentTaskTo(TimeOverExitCallback);
    }
}
void TimeOverExitCallback(void *) {
    if (eventParameters.state == -1) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(MazeTimeOverTo552ECallback);
        HLib::SetupTaskDelay(90);
    }
}
void StartNavigationCallback(void *) {
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    PlaceNaviMarker(position, 4);
    if (!resumed)
        InitializeMazeNavigationTrail();
    HLib::DestroyCurrentTask();
}
} // namespace
void AreaKMZ2Init() {
    resumed = 0;
    forceEvent = 0;
    eventParameters = {};
}
void AreaKMZ2Main(std::uint32_t startParameters) {
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_FreeAuthorPak(1, "0527");
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (static_cast<std::uint8_t>(dateTime.hour - 8) > 14U) {
        dateTime.hour = 8;
        dateTime.minute = 22;
        SetCurrentDateTimeTo(dateTime);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
    resumed = startParameters ? 1 : 0;
    if (!resumed)
        areaFlags |= 3;
    if (resumed)
        HLib::EnqueueTaskWithoutParameter(EventBombCallback, 4, 0x0b, eventTask.name);
    else {
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(AfterQteSetupCallback, 4, 0x0b, eventTask.name);
        SetPlayerControlEnabled(0);
        LoadCompanion(task);
    }
    forceEvent = 0;
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(StartNavigationCallback, 4, 0x0b, navigationTask.name);
    HLib::SetupTimedTask(task, 3);
    SetMazeUnitDrawDistance(65.0f);
}
void AreaKMZ2Cleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaKMZ2GameplayDefinition{SHEN_NAME(KMZ2), 0, AreaKMZ2Init, AreaKMZ2Main, AreaKMZ2Cleanup};
} // namespace shenmue::s2::areas
