#include "s2/areas/area_kmz1.hpp"
#include "s2/author.hpp"
#include "s2/areas/area_maze_events.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/system_modules.hpp"
#include "s2/maze.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/search.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo = MAZE_EVENT_RYO, shenhua = MAZE_EVENT_SHENHUA;
constexpr shen_name loadCharacters[]{shenhua, shen_name{0xffffffffU}};
struct MazeEventParameters {
    std::int32_t state;
    Vector3 position;
    std::uint32_t yaw;
};
static_assert(sizeof(MazeEventParameters) == 20);
static_assert(offsetof(MazeEventParameters, position) == 4);
std::int32_t playerMarker, companionMarker, entryVariant, hasRunIntro, debugEnabled;
MazeEventParameters eventParameters;
void EventBombCallback(void *);
void IntroSetupCallback(void *);
void IntroExitCallback(void *);
void FollowSetupCallback(void *);
void FollowExitCallback(void *);
void CollapseSetupCallback(void *);
void CollapseExitCallback(void *);
void ChangePositionCallback(void *);
void TimeOverLockCallback(void *);
void TimeOverSetupCallback(void *);
void TimeOverExitCallback(void *);
void ResetActorMotion(shen_name actor, std::uint16_t animation) {
    mtwkSetExtraFlag(scenFindMotionwork(actor), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(actor), animation, MotionRegion::LowerBody);
}
void LoadCompanion(HLib::Task *continuation) {
    AssignAreaNpcLoadContext(loadCharacters, continuation);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (continuation)
        HLib::SetupTimedTask(continuation, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0xb, static_cast<std::int32_t>(SHEN_NAME(load)));
}
void EnableSkipCallback(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    HLib::DestroyCurrentTask();
}
void AssignMazeEvent(std::int32_t state, Vector3 position, std::uint32_t yaw, std::uint32_t unit) {
    shenToggleAreaFlags(0);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    EV_PTYCUtils(shenhua, 3, 1);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
    ResetActorMotion(shenhua, 0x9502);
    eventParameters.state = state;
    eventParameters.position = position;
    eventParameters.yaw = yaw;
    Vector3 unitPosition;
    GetMazeUnitPosition(static_cast<std::int32_t>(unit), &unitPosition);
    const Vector3i unitRotation{0, static_cast<std::int32_t>(GetMazeUnitYaw(static_cast<std::int32_t>(unit))), 0};
    ApplyScenarioTranslationAndRotation(&unitPosition, &unitRotation, &eventParameters.position);
    eventParameters.yaw += unitRotation.y;
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EVT_AT_MenCtrl(EventFilmControl{1, 0, 0, 0, 0, 0, 255, 0, 0, 0, 2});
    EVT_AT_MenCtrl(EventFilmControl{1, 60, 255, 0, 0, 0, 0, 0, 0, 0, 15});
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(EnableSkipCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(GakS)));
    HLib::SetupTimedTask(task, 70);
}
void DispatchMazeEvent() {
    ExecuteCurrAreaMainFunc_Wrapper(2, reinterpret_cast<std::uintptr_t>(&eventParameters));
}
void IntroSetupCallback(void *) {
    AssignMazeEvent(13, {2.1475000381469727f, 0, -.9405999779701233f}, 16980, 1);
    DispatchMazeEvent();
    if (entryVariant == 1)
        QueueImmediateNextFadeTask(nullptr);
    HLib::SetCurrentTaskTo(IntroExitCallback);
}
void IntroExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-194.68080139160156f, 500, -175.88699340820312f};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, {0, 14330, 0});
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-196.44000244140625f, 500, -176.38600158691406f};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 19076, 0});
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 45);
    EV_stepSetFlags(0, 0x3a5);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void FollowSetupCallback(void *) {
    AssignMazeEvent(16, {-1.0240999460220337f, 0, 2.190500020980835f}, 61427, 1);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(FollowExitCallback);
}
void FollowExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-183.88999938964844f, 500, -174.78399658203125f};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, {0, 49122, 0});
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-186.91000366210938f, 500, -174.63999938964844f};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 49516, 0});
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 0x34d);
    EV_PTYCUtils(shenhua, 1, 46);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void CollapseSetupCallback(void *) {
    AssignMazeEvent(17, {0, 0, 8.461899757385254f}, 0, 0);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(CollapseExitCallback);
}
void CollapseExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-112.89839935302734f, 500, -219.9427947998047f};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, {0, 28708, 0});
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-113.80000305175781f, 500, -218.1199951171875f};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 32942, 0});
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 11);
    EV_stepSetFlags(0, 0x1b9);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void LeaveForestCallback(void *) {
    shenToggleAreaFlags(1);
    EV_stepSetFlags(0, 0xde);
    EV_scenQueueLoad(g_actNumber, KMZ1_NEXT_MAP, 0);
}
void CompanionPatchCallback(void *) {
    if (!FindCharacterByScenarioName(shenhua))
        return;
    EV_PTYCUtils(shenhua, 3, 1);
    EV_PTYCUtils(shenhua, 1, 45);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    HLib::DestroyCurrentTask();
}
void UpdatePlayerFlags() {
    constexpr std::int32_t resetMarkers[]{4, 5, 6}, admitMarkers[]{13, 22, 24};
    for (std::int32_t marker : resetMarkers) {
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 10) && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 0);
        }
    }
    for (std::int32_t marker : admitMarkers) {
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 20) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
        }
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 7)) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
            }
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1440) == 1)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 1);
        }
    }
    if (debugEnabled) {
        EV_set_debug_text_pos(32, 9);
        const char *state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1440) ? "ON" : "OFF";
        EV_debug_log_to_screen("NOW %s", state);
        EV_set_debug_text_pos(32, 10);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441))
            state = "ON";
        EV_debug_log_to_screen("NOW %s", state);
    }
}
bool TryChangePosition() {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1495))
        return false;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    constexpr std::int32_t stories[]{0x34d, 0x1b9};
    for (std::int32_t i = 0; i < 2; ++i) {
        if (!EV_GetShenmueFlag(stories[i]))
            continue;
        companionMarker = 45 + i;
        playerMarker = 45 + i;
        shenToggleAreaFlags(2);
        SetPlayerControlEnabled(0);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ChangePositionCallback);
        HLib::SetupTaskDelay(60);
        return true;
    }
    return false;
}
bool TryAuthoredEvent() {
    struct EventRow {
        std::int32_t marker;
        float radius;
        std::int32_t story;
        HLib::TaskCallback callback;
    };
    constexpr EventRow rows[]{
        {45, 2, 0x34d, FollowSetupCallback}, {46, 2, 0x1b9, CollapseSetupCallback}, {10, 3.5f, 0x3a5, IntroSetupCallback}, {1, 2, 0xde, LeaveForestCallback}};
    for (std::int32_t i = 0; i < 4; ++i) {
        const auto &row = rows[i];
        if (!IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), row.marker, row.radius) || !EV_GetShenmueFlag(row.story))
            continue;
        shenToggleAreaFlags(2);
        SetPlayerControlEnabled(0);
        HLib::SetCurrentTaskTo(row.callback);
        if (!hasRunIntro) {
            std::int32_t delay;
            if (i == 3) {
                spmFadeOut();
                SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
                delay = 60;
            } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902) && EV_PTYCUtils(shenhua, 4, 0)) {
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                delay = 60;
            } else {
                SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
                delay = 15;
            }
            HLib::SetupTaskDelay(delay);
        }
        hasRunIntro = 0;
        return true;
    }
    return false;
}
bool TryTimeOver() {
    if (!EV_GetShenmueFlag(0x459))
        return false;
    DateTime date;
    GetCurrentDateTime(date);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        if ((date.hour < 16 || date.minute < 30) && date.hour < 17)
            return false;
    } else if (date.hour < 17)
        return false;
    if (!shenGetIsPlayerInMenu())
        return false;
    srchSetLookMode();
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(0);
    HLib::SetCurrentTaskTo(TimeOverLockCallback);
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(30);
    return true;
}
void EventBombCallback(void *) {
    if (hasRunIntro || shenGetIsPlayerInMenu()) {
        UpdatePlayerFlags();
        if (!TryChangePosition() && !TryAuthoredEvent())
            TryTimeOver();
    }
}
void ChangePositionCallback(void *) {
    EV_PTYCUtils(shenhua, 3, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 1, companionMarker);
    Vector3 position;
    mazeGetMarkerPosition(companionMarker, &position);
    position.y = 100;
    position.y = shenFindIntersectionWithScene(position, position.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, position);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mazeGetMarkerPosition(playerMarker, &position);
    position.y = 100;
    position.y = shenFindIntersectionWithScene(position, position.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    hasRunIntro = 1;
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void TimeOverLockCallback(void *) {
    HLib::SetCurrentTaskTo(TimeOverSetupCallback);
    if (!DoesScenarioCharacterExist(shenhua)) {
        HLib::SetupTaskDelay(0);
        LoadCompanion(HLib::GetCurrentTask());
    }
}
void TimeOverSetupCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(MazeTimeOverToForest4Callback);
        return;
    }
    shenToggleAreaFlags(0);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    EV_PTYCUtils(shenhua, 3, 1);
    ResetActorMotion(shenhua, 0x9502);
    eventParameters.position = {-236.60000610351562f, 17.100000381469727f, -197.5f};
    eventParameters.state = 32;
    const float height = shenFindIntersectionWithScene(eventParameters.position, eventParameters.position.y);
    if (height > -3.4028234663852886e38f)
        eventParameters.position.y = shenFindIntersectionWithScene(eventParameters.position, height);
    else
        eventParameters.position.y = 16.100000381469727f;
    eventParameters.yaw = 6371;
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(TimeOverExitCallback);
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
    HLib::DestroyCurrentTask();
}
void StartEntry(std::int32_t variant) {
    entryVariant = variant;
    hasRunIntro = 0;
    debugEnabled = 0;
    if (variant != 1) {
        if (EV_GetShenmueFlag(0x1c9)) {
            EV_stepSetFlags(0, 0x1c9);
            shenToggleAreaFlags(1);
        } else if (!EV_GetShenmueFlag(0x34d)) {
            SetPlayerControlEnabled(1);
            HLib::EnqueueTaskWithoutParameter(EventBombCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(bomS)));
            return;
        }
        SetPlayerControlEnabled(1);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(CompanionPatchCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(Stat)));
        HLib::SetupTimedTask(task, 2);
        HLib::EnqueueTaskWithoutParameter(EventBombCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(bomS)));
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(IntroSetupCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(bomS)));
    shenToggleAreaFlags(1);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(shenhua, 3, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    LoadCompanion(task);
}
} // namespace
void AreaKMZ1Init() {
    eventParameters = {};
    hasRunIntro = 0;
    debugEnabled = 0;
    companionMarker = 0;
    entryVariant = 0;
    playerMarker = 0;
}
void AreaKMZ1Cleanup(std::int32_t, std::uint32_t) {
}
void AreaKMZ1Main(std::uint32_t) {
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_FreeAuthorPak(1, "0527");
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    DateTime date;
    GetCurrentDateTime(date);
    if (static_cast<std::uint32_t>(date.hour - 8) > 14U) {
        date.hour = 8;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
    std::int32_t variant = 0;
    if (g_entryPoint == 99) {
        areaFlags |= 3U;
        SetSceneInfos(g_actNumber, g_currentMap, 0);
        variant = 1;
    }
    StartEntry(variant);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(StartNavigationCallback, 4, 0xb, static_cast<std::int32_t>(TASK_NAVI_START.name));
    HLib::SetupTimedTask(task, 3);
    ConfigureScenarioNodeMotion(SHEN_NAME(FISH), 40963, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(SHEN_NAME(FISH), 0, -1, -1, -1, 32767, 4);
    SetMazeUnitDrawDistance(65);
}
const GameplayAreaDefinition areaKMZ1GameplayDefinition{SHEN_NAME(KMZ1), 0, AreaKMZ1Init, AreaKMZ1Main, AreaKMZ1Cleanup};
} // namespace shenmue::s2::areas
