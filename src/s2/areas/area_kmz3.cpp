#include "s2/areas/area_kmz3.hpp"
#include "s2/areas/area_maze_events.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/system_modules.hpp"
#include "s2/maze.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/search.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
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
std::int32_t startParameters, entryFlags, companionMarker, playerMarker;
std::uint64_t forceEvent;
MazeEventParameters eventParameters;
void EventBombCallback(void *);
void CollapseExitCallback(void *);
void BranchChoiceCallback(void *);
void BranchExitCallback(void *);
void BranchExit2Callback(void *);
void SpringCollapseSetupCallback(void *);
void SpringContinueSetupCallback(void *);
void TimeOverSetupCallback(void *);
void TimeOverExitCallback(void *);
void ChangePositionCallback(void *);
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
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, SHEN_NAME(load));
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
    if (unit == 0xffffffffU)
        unit = static_cast<std::uint32_t>(GetCharacterMazeUnitIndex(FindCharacterByScenarioName(ryo)));
    Vector3 unitPosition;
    GetMazeUnitPosition(static_cast<std::int32_t>(unit), &unitPosition);
    const Vector3i unitRotation{0, static_cast<std::int32_t>(GetMazeUnitYaw(static_cast<std::int32_t>(unit))), 0};
    ApplyScenarioTranslationAndRotation(&unitPosition, &unitRotation, &eventParameters.position);
    eventParameters.yaw += unitRotation.y;
}
void DispatchMazeEvent() {
    ExecuteCurrAreaMainFunc_Wrapper(2, reinterpret_cast<std::uintptr_t>(&eventParameters));
}
void FollowExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-93.269501f, 500.0f, -486.302f};
    const Vector3i companionRotation{0, 1583, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-92.795303f, 500.0f, -482.72f};
    const Vector3i playerRotation{0, 1583, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 718);
    EV_PTYCUtils(shenhua, 1, 95);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void BranchExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-133.2755f, 500.0f, -590.0f};
    const Vector3i companionRotation{0, 56397, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-135.06371f, 500.0f, -588.02197f};
    const Vector3i playerRotation{0, 57622, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 74);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    shenSetClockAndCompassRenderTo(0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void BranchExit2Callback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-143.106f, 500.0f, -581.84589f};
    const Vector3i companionRotation{0, 27628, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-139.698f, 500.0f, -584.12f};
    const Vector3i playerRotation{0, 19759, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 97);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    shenSetClockAndCompassRenderTo(0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void FlowerExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-153.8350067138672f, 500.0f, -645.3858f};
    const Vector3i companionRotation{0, 16493, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-151.03551f, 500.0f, -647.69019f};
    const Vector3i playerRotation{0, 20416, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 458);
    EV_PTYCUtils(shenhua, 1, 96);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void SpiderExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-219.0f, 500.0f, -557.15002f};
    const Vector3i companionRotation{0, 10849, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-213.89999f, 500.0f, -554.03998f};
    const Vector3i playerRotation{0, 9229, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 448);
    EV_PTYCUtils(shenhua, 1, 99);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void SpringBranchExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-357.92001f, 500.0f, -628.56f};
    const Vector3i companionRotation{0, 15420, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-359.31f, 500.0f, -626.34998f};
    const Vector3i playerRotation{0, 60431, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 849);
    EV_PTYCUtils(shenhua, 1, 75);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    ResetMazeHourAnimation(4);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void Herb1ExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-403.54001f, 500.0f, -487.67001f};
    const Vector3i companionRotation{0, 15473, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-406.44f, 500.0f, -488.04501f};
    const Vector3i playerRotation{0, 16384, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    DisableScenarioCharacterDisplay(SHEN_NAME(a980));
    DisableScenarioCharacterDisplay(SHEN_NAME(a984));
    DisableScenarioCharacterDisplay(SHEN_NAME(a982));
    DisableScenarioCharacterDisplay(SHEN_NAME(a986));
    EV_stepSetFlags(0, 722);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    clmdUtil(FindCharacterByScenarioName(shenhua), 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void Herb2ExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-498.06280517578125f, 500.0f, -543.87231f};
    const Vector3i companionRotation{0, 60827, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-497.20999f, 500.0f, -545.5f};
    const Vector3i playerRotation{0, 0, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    DisableScenarioCharacterDisplay(SHEN_NAME(a016));
    DisableScenarioCharacterDisplay(SHEN_NAME(a980));
    DisableScenarioCharacterDisplay(SHEN_NAME(a984));
    DisableScenarioCharacterDisplay(SHEN_NAME(a982));
    DisableScenarioCharacterDisplay(SHEN_NAME(a986));
    EV_stepSetFlags(0, 723);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void FollowSetupCallback(void *) {
    AssignMazeEvent(20, {-1.5224f, 0.0f, 11.4321f}, 44676, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(FollowExitCallback);
}
void BranchSetupCallback(void *) {
    shenSetClockAndCompassRenderTo(1);
    AssignMazeEvent(2, {0.0f, 0.0f, 0.0f}, 13768, 7);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(BranchChoiceCallback);
}
void BranchChoiceCallback(void *) {
    if (eventParameters.state != -1 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 398) {
        EV_stepSetFlags(0, 719);
        AssignMazeEvent(14, {0.0f, 0.0f, 0.0f}, 9241, 7);
        HLib::SetCurrentTaskTo(BranchExitCallback);
    } else {
        EV_stepSetFlags(1, 719);
        AssignMazeEvent(15, {0.0458f, 0.0f, 1.2312f}, 17533, 7);
        HLib::SetCurrentTaskTo(BranchExit2Callback);
    }
    DispatchMazeEvent();
}
void FlowerSetupCallback(void *) {
    AssignMazeEvent(3, {-0.34290001f, -0.0020999999f, 20.745899f}, 64118, 6);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(FlowerExitCallback);
}
void SpiderLeftSetupCallback(void *) {
    AssignMazeEvent(18, {-0.27880001f, -0.36899999f, 7.6160002f}, 2545, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(SpiderExitCallback);
}
void SpiderRightSetupCallback(void *) {
    AssignMazeEvent(19, {-13.4542f, -0.36899999f, -0.1796f}, 50621, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(SpiderExitCallback);
}
void SpringBranchSetupCallback(void *) {
    AssignMazeEvent(8, {-1.4541f, 0.0f, 3.6214001f}, 61990, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(SpringBranchExitCallback);
}
void SpringExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    EV_PTYCUtils(shenhua, 3, 1);
    EV_stepSetFlags(0, 448);
    EV_PTYCUtils(shenhua, 1, 37);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_stepSetFlags(0, 721);
    HLib::SetCurrentTaskTo(SpringCollapseSetupCallback);
    HLib::SetupTaskDelay(90);
}
void SpringSetupCallback(void *) {
    shenSetClockAndCompassRenderTo(1);
    AssignMazeEvent(6, {-7.0001001f, -0.14920001f, 8.4889002f}, 6925, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(SpringExitCallback);
}
void SpringCollapseSetupCallback(void *) {
    AssignMazeEvent(7, {-3.3440001f, 0.1007f, -18.2855f}, 1689, 11);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(SpringContinueSetupCallback);
}
void SpringContinueSetupCallback(void *) {
    if (eventParameters.state != -1)
        return;
    AssignMazeEvent(26, {2.0943f, 0.0f, -0.58890003f}, 19358, 12);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(CollapseExitCallback);
}
void Herb1SetupCallback(void *) {
    AssignMazeEvent(0, {-0.54790002f, -0.0099999998f, -0.3831f}, 0, 14);
    clmdUtil(FindCharacterByScenarioName(shenhua), 22);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(Herb1ExitCallback);
}
void Herb2SetupCallback(void *) {
    AssignMazeEvent(1, {-0.23029999f, 0.0f, 1.0896f}, 0, 19);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(Herb2ExitCallback);
    EnableScenarioCharacterDisplay(SHEN_NAME(a980));
    EnableScenarioCharacterDisplay(SHEN_NAME(a984));
    EnableScenarioCharacterDisplay(SHEN_NAME(a982));
    EnableScenarioCharacterDisplay(SHEN_NAME(a986));
}
void AdmitCollapseCallback(void *) {
    EV_stepSetFlags(0, 720);
    forceEvent = (forceEvent & 0xffffffff00000000ULL) | 1;
}
void AdmitRiverLeftCallback(void *) {
    EV_stepSetFlags(0, 859);
    forceEvent = (forceEvent & 0xffffffff00000000ULL) | 1;
}
void AdmitRiverRightCallback(void *) {
    EV_stepSetFlags(0, 860);
    forceEvent = (forceEvent & 0xffffffff00000000ULL) | 1;
}
void EnterForest6Callback(void *) {
    shenToggleAreaFlags(1);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(KRK6), 0);
}
struct MazeTrigger {
    std::int32_t marker;
    float radius;
    std::int32_t requiredFlag;
    std::int32_t reserved;
    HLib::TaskCallback prepare;
    HLib::TaskCallback callback;
};
static_assert(sizeof(MazeTrigger) == 32);
static_assert(offsetof(MazeTrigger, prepare) == 16);
constexpr MazeTrigger triggers[]{
    {27, 7.0f, 718, 0, nullptr, FollowSetupCallback},
    {95, 2.0f, 719, 0, nullptr, BranchSetupCallback},
    {74, 2.0f, 458, 0, nullptr, FlowerSetupCallback},
    {96, 2.0f, 448, 0, nullptr, SpiderLeftSetupCallback},
    {97, 2.0f, 448, 0, nullptr, SpiderRightSetupCallback},
    {37, 2.0f, 720, 0, AdmitCollapseCallback, EventBombCallback},
    {99, 2.0f, 849, 0, nullptr, SpringBranchSetupCallback},
    {75, 3.0f, 721, 0, nullptr, SpringSetupCallback},
    {100, 2.0f, 722, 0, nullptr, Herb1SetupCallback},
    {101, 2.0f, 723, 0, nullptr, Herb2SetupCallback},
    {14, 5.0f, 859, 0, AdmitRiverLeftCallback, EventBombCallback},
    {13, 5.0f, 860, 0, AdmitRiverRightCallback, EventBombCallback},
    {1, 2.0f, 846, 0, nullptr, EnterForest6Callback},
};
constexpr std::int32_t dangerEntryMarkers[]{18, 49, 50, 63};
constexpr std::int32_t dangerExitMarkers[]{3, 11, 12, 15};
void UpdateDangerAreaFlags() {
    for (std::int32_t marker : dangerExitMarkers) {
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 10.0f) && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 0);
        }
    }
    for (std::int32_t marker : dangerEntryMarkers) {
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 20.0f) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
        }
        if (IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), marker, 7.0f)) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
            }
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1440) == 1)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 1);
        }
    }
}
struct StoryRelocation {
    char name[16];
    std::int32_t flag, advanceStep, prerequisite, companionMarker;
    Vector3 companionPosition;
    std::int32_t reserved2c;
    std::int32_t playerMarkerTrue, playerMarkerFalse;
    Vector3 playerPosition;
    std::int32_t reserved44;
};
static_assert(sizeof(StoryRelocation) == 72);
static_assert(offsetof(StoryRelocation, flag) == 16);
static_assert(offsetof(StoryRelocation, playerMarkerTrue) == 48);
constexpr StoryRelocation relocations[]{
    {"Y_JIRO1", 719, 0, 0, 7, {}, 0, 95, 0, {}, 0},
    {"TUKIMISOU", 458, 0, 0, 74, {}, 0, 74, 0, {}, 0},
    {"GOURYU1", 448, 0, 767, 8, {}, 0, 96, 97, {}, 0},
    {"IZUMI_BUNKI", 849, 0, 0, 10, {}, 0, 99, 0, {}, 0},
    {"IZUMI", 721, 0, 0, 75, {}, 0, 75, 0, {}, 0},
};
bool CheckRelocation() {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1495))
        return false;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    for (const StoryRelocation &relocation : relocations) {
        if (!EV_GetShenmueFlag(relocation.flag))
            continue;
        companionMarker = relocation.companionMarker;
        playerMarker = !relocation.prerequisite || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, relocation.prerequisite) ? relocation.playerMarkerTrue :
                                                                                                                             relocation.playerMarkerFalse;
        shenToggleAreaFlags(2);
        SetPlayerControlEnabled(0);
        if (relocation.advanceStep)
            EV_stepSetFlags(0, relocation.flag);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ChangePositionCallback);
        HLib::SetupTaskDelay(60);
        return true;
    }
    return false;
}
void ChangePositionCallback(void *) {
    EV_PTYCUtils(shenhua, 3, 1);
    EV_PTYCUtils(shenhua, 1, companionMarker);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    Vector3 position;
    mazeGetMarkerPosition(companionMarker, &position);
    position.y = 100.0f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, position);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mazeGetMarkerPosition(playerMarker, &position);
    position.y = 100.0f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    forceEvent = (forceEvent & 0xffffffff00000000ULL) | 1;
    HLib::SetCurrentTaskTo(EventBombCallback);
}
bool CheckMazeEvent() {
    for (std::size_t index = 0; index < std::size(triggers); ++index) {
        const MazeTrigger &trigger = triggers[index];
        if (!IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), trigger.marker, trigger.radius) || !EV_GetShenmueFlag(trigger.requiredFlag))
            continue;
        if (trigger.prepare)
            trigger.prepare(nullptr);
        else {
            shenToggleAreaFlags(2);
            SetPlayerControlEnabled(0);
        }
        HLib::SetCurrentTaskTo(trigger.callback);
        if (!forceEvent) {
            std::int32_t delay;
            if (index == 12) {
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
        forceEvent &= 0xffffffff00000000ULL;
        return true;
    }
    return false;
}
void TimeOverLockcheckCallback(void *) {
    HLib::SetCurrentTaskTo(TimeOverSetupCallback);
    if (!DoesScenarioCharacterExist(shenhua)) {
        HLib::SetupTaskDelay(0);
        LoadCompanion(HLib::GetCurrentTask());
    }
}
bool CheckTimeOver() {
    if (!EV_GetShenmueFlag(0x455) && !EV_GetShenmueFlag(0x456))
        return false;
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        if ((dateTime.hour < 16 || dateTime.minute < 30) && dateTime.hour < 17)
            return false;
    } else if (dateTime.hour < 17)
        return false;
    if (!shenGetIsPlayerInMenu())
        return false;
    srchSetLookMode();
    shenToggleAreaFlags(3);
    SetPlayerControlEnabled(0);
    HLib::SetCurrentTaskTo(TimeOverLockcheckCallback);
    SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(15);
    return true;
}
void EventBombCallback(void *) {
    if (forceEvent || ryoNotBusy()) {
        UpdateDangerAreaFlags();
        if (!CheckRelocation() && !CheckMazeEvent())
            CheckTimeOver();
    }
}
void TimeOverSetupCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(MazeTimeOverToForest4Callback);
    } else {
        shenToggleAreaFlags(0);
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
        SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
        EV_PTYCUtils(shenhua, 3, 1);
        ResetActorMotion(shenhua, 0x9502);
        eventParameters.position = {-87.400002f, 100.0f, -448.29999f};
        eventParameters.state = 32;
        eventParameters.position.y = shenFindIntersectionWithScene(eventParameters.position, eventParameters.position.y);
        eventParameters.yaw = 26396;
        DispatchMazeEvent();
        HLib::SetCurrentTaskTo(TimeOverExitCallback);
    }
}
void TimeOverExitCallback(void *) {
    if (eventParameters.state == -1) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(MazeTimeOverTo552ECallback);
        shenSetClockAndCompassRenderTo(1);
        HLib::SetupTaskDelay(90);
    }
}
void CollapseExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-367.15491f, 500.0f, -619.06293f};
    const Vector3i companionRotation{0, 22507, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    EV_PTYCUtils(shenhua, 3, 0);
    Vector3 playerPosition{-369.06f, 500.0f, -617.96002f};
    const Vector3i playerRotation{0, 26385, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    shenSetClockAndCompassRenderTo(0);
    RestoreMazeCompanionControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void StartWaitCallback(void *) {
    if (!mtwkSetExtraFlag(scenFindMotionwork(ryo), 58, 1)) {
        EV_PTYCUtils(shenhua, 3, 0);
        SetPlayerControlEnabled(1);
        HLib::SetCurrentTaskTo(EventBombCallback);
    }
}
void StartCallback(void *) {
    if (!DoesScenarioCharacterExist(ryo))
        return;
    SetPlayerControlEnabled(0);
    const Vector3 playerPosition{-395.20001f, 3.5985999f, -398.70001f};
    CopyMotionTransition(scenFindMotionwork(ryo), &playerPosition, 51427, 5, 0);
    Vector3 companionPosition{-401.92999f, 500.0f, -399.9313f};
    const Vector3i rotation{0, 51427, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, rotation);
    ResetActorMotion(shenhua, 0x9502);
    HLib::SetCurrentTaskTo(StartWaitCallback);
}
void StartNavigationCallback(void *) {
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    navPlaceMarker(position, 4);
    if (!startParameters)
        InitializeMazeNavigationTrail();
    HLib::DestroyCurrentTask();
}
} // namespace
void AreaKMZ3Init() {
    startParameters = 0;
    forceEvent = 0;
    eventParameters = {};
    entryFlags = 0;
    playerMarker = 0;
    companionMarker = 0;
}
void AreaKMZ3Main(std::uint32_t parameters) {
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_FreeAuthorPak(1, "0527");
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (static_cast<std::uint8_t>(dateTime.hour - 8) > 14U) {
        dateTime.hour = 8;
        dateTime.minute = 0;
        SetCurrentDateTimeTo(dateTime);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
    ResetMazeHourAnimation(4);
    startParameters = static_cast<std::int32_t>(parameters);
    entryFlags = g_entryPoint != 0 ? 1 : 0;
    if (parameters)
        entryFlags |= 2;
    if (!entryFlags)
        areaFlags |= AreaFlag_KeepPlayerControlLocked;
    SetSceneInfos(g_actNumber, g_currentMap, 0);
    HLib::Task *task;
    if (entryFlags & 1) {
        task = HLib::EnqueueTaskWithoutParameter(CollapseExitCallback, 4, 0x0b, SHEN_NAME(bomS));
        shenToggleAreaFlags(1);
        SetPlayerControlEnabled(0);
        eventParameters.state = -1;
        LoadCompanion(task);
    } else if (entryFlags) {
        HLib::EnqueueTaskWithoutParameter(EventBombCallback, 4, 0x0b, SHEN_NAME(bomS));
    } else {
        task = HLib::EnqueueTaskWithoutParameter(StartCallback, 4, 0x0b, SHEN_NAME(bomS));
        HLib::SetupTimedTask(task, 0);
        LoadCompanion(task);
    }
    forceEvent = 0;
    task = HLib::EnqueueTaskWithoutParameter(StartNavigationCallback, 4, 0x0b, TASK_NAVI_START.name);
    HLib::SetupTimedTask(task, 3);
    ConfigureScenarioNodeMotion(SHEN_NAME(FISH), 0xa003, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    ConfigureScenarioNodeMotion(SHEN_NAME(FISH), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    SetMazeUnitDrawDistance(65.0f);
}
void AreaKMZ3Cleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaKMZ3GameplayDefinition{SHEN_NAME(KMZ3), 0, AreaKMZ3Init, AreaKMZ3Main, AreaKMZ3Cleanup};
} // namespace shenmue::s2::areas
