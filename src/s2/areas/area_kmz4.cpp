#include "s2/areas/area_kmz4.hpp"
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
#include "s2/world_render.hpp"
#include "s2/math.hpp"
#include "d3t/dx11_renderer.hpp"
#include <cmath>
#include <bit>
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
std::int32_t startParameters, entryFlags, companionMarker, playerMarker, drawLimit;
HLib::Task *dandelionTask, *wakeTask, *killTask;
std::uint64_t forceEvent;
MazeEventParameters eventParameters;
void EventBombCallback(void *);
void FollowExitCallback(void *);
void Branch1ChoiceCallback(void *);
void Branch1ExitCallback(void *);
void Branch1Exit2Callback(void *);
void DandelionSetupCallback(void *);
void DandelionExitCallback(void *);
void CottonExitCallback(void *);
void Branch2ChoiceCallback(void *);
void Branch2ExitCallback(void *);
void Branch2Exit2Callback(void *);
void WaterfallSetupCallback(void *);
void WaterfallExitCallback(void *);
void LastAuthorSetupCallback(void *);
void LastAuthorExitCallback(void *);
void StartExitCallback(void *);
void TimeOverSetupCallback(void *);
void TimeOverExitCallback(void *);
void TimeOverLockCallback(void *);
void ChangePositionCallback(void *);
void DandelionLoopCallback(void *);
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
void AssignAbsoluteEvent(std::int32_t state, Vector3 position, std::uint32_t yaw) {
    shenToggleAreaFlags(0);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 13, 0);
    EV_PTYCUtils(shenhua, 3, 1);
    ResetActorMotion(shenhua, 0x9502);
    eventParameters.position = position;
    eventParameters.state = state;
    eventParameters.position.y = shenFindIntersectionWithScene(eventParameters.position, position.y);
    eventParameters.yaw = yaw;
}
void ResumeMazeControl() {
    camCtrlUtil(static_cast<CameraUtilityMode>(0), 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    InitiateCleanupForSceneChara(shenhua);
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(1);
}
void FollowExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-268.17999f, 500.0f, -810.40002f};
    const Vector3i companionRotation{0, 57737, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-270.60999f, 500.0f, -807.41998f};
    const Vector3i playerRotation{0, 57526, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    EV_stepSetFlags(0, 724);
    EV_PTYCUtils(shenhua, 1, 52);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void Branch1Exit2Callback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-205.56f, 500.0f, -743.12f};
    const Vector3i companionRotation{0, 57737, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-207.32001f, 500.0f, -746.03998f};
    const Vector3i playerRotation{0, 36845, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    SetRenderClockAndCompassTo(0);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void DandelionExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-169.42999f, 500.0f, -785.46997f};
    const Vector3i companionRotation{0, 60396, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-170.41f, 500.0f, -782.79999f};
    const Vector3i playerRotation{0, 62841, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_stepSetFlags(0, 459);
    EV_PTYCUtils(shenhua, 1, 54);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    SetRenderClockAndCompassTo(0);
    ResumeMazeControl();
    if (wakeTask)
        HLib::DestroyTaskInternal(wakeTask);
    if (killTask) {
        HLib::DestroyTask(dandelionTask);
        HLib::DestroyTask(killTask);
    }
    HLib::SetCurrentTaskTo(EventBombCallback);
    for (std::uint32_t i = 0; i != 100; ++i) {
        const shen_name name{14177U | ((i / 10 + 48U) << 16) | ((i % 10 + 48U) << 24)};
        if (FindCharacterByScenarioName(name)) {
            ToggleScenarioCharacterFlag(name);
            DisableScenarioCharacterDisplay(name);
            EV_SetCharacterAlphaEnabled(name, true);
        }
    }
}
void CottonExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-107.35f, 500.0f, -805.15002f};
    const Vector3i companionRotation{0, 2331, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-106.67f, 500.0f, -801.91998f};
    const Vector3i playerRotation{0, 505, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_PTYCUtils(shenhua, 1, 56);
    EV_stepSetFlags(0, 449);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void WaterfallExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-203.85001f, 500.0f, -938.28497f};
    const Vector3i companionRotation{0, 12994, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-198.315f, 500.0f, -936.78003f};
    const Vector3i playerRotation{0, 12404, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    camCtrlUtil(CameraUtilityMode::SetConversationPosition, 0);
    EV_stepSetFlags(0, 727);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    EV_PTYCUtils(shenhua, 1, 57);
    camCtrlSetFarDistance(50.0f);
    SetRenderClockAndCompassTo(0);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void Branch2ExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-164.39f, 500.0f, -863.28998f};
    const Vector3i companionRotation{0, 36467, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-165.75999f, 500.0f, -865.45001f};
    const Vector3i playerRotation{0, 36845, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 0);
    EV_PTYCUtils(shenhua, 3, 0);
    SetRenderClockAndCompassTo(0);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void StartExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    Vector3 companionPosition{-399.16f, 500.0f, -802.96997f};
    const Vector3i companionRotation{0, 55159, 0};
    companionPosition.y = shenFindIntersectionWithScene(companionPosition, companionPosition.y);
    EV_AT_SetPos(shenhua, EventTransformFlags::AllAxes, companionPosition);
    EV_AT_SetAngY(shenhua, EventTransformFlags::AllAxes, companionRotation);
    ResetActorMotion(shenhua, 0x9502);
    Vector3 playerPosition{-397.2825f, 500.0f, -803.34497f};
    const Vector3i playerRotation{0, 57440, 0};
    playerPosition.y = shenFindIntersectionWithScene(playerPosition, playerPosition.y);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, playerRotation);
    ResetActorMotion(ryo, 0xf003);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(shenhua, 3, 0);
    ResumeMazeControl();
    HLib::SetCurrentTaskTo(EventBombCallback);
}
void DandelionWakeCallback(void *) {
    HLib::ResolveDeferredCallback(dandelionTask);
    HLib::DestroyCurrentTask();
    wakeTask = nullptr;
}
void DandelionKillCallback(void *) {
    drawLimit -= 2;
    if (drawLimit <= 0) {
        HLib::DestroyTask(dandelionTask);
        HLib::DestroyCurrentTask();
        killTask = nullptr;
    }
}
void DandelionSetupCallback(void *) {
    AssignMazeEvent(10, {0.0f, 0.0f, 23.9202f}, 0, 10);
    DispatchMazeEvent();
    drawLimit = 300;
    wakeTask = HLib::EnqueueTaskWithoutParameter(DandelionWakeCallback, 4, 0x0b, SHEN_NAME(bomW));
    HLib::SetupTimedTask(wakeTask, 1100);
    killTask = HLib::EnqueueTaskWithoutParameter(DandelionKillCallback, 4, 0x0b, SHEN_NAME(bomW));
    HLib::SetupTimedTask(killTask, 2200);
    HLib::SetCurrentTaskTo(DandelionExitCallback);
}
void Branch1ExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    HLib::SetCurrentTaskTo(DandelionSetupCallback);
    HLib::SetupTaskDelay(60);
}
void Branch2Exit2Callback(void *) {
    if (eventParameters.state != -1)
        return;
    HLib::SetCurrentTaskTo(WaterfallSetupCallback);
    HLib::SetupTaskDelay(60);
}
void Branch1ChoiceCallback(void *) {
    if (eventParameters.state != -1 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 69) {
        EV_stepSetFlags(0, 725);
        EV_PTYCUtils(shenhua, 1, 41);
        AssignAbsoluteEvent(11, {-210.6039f, 100.0f, -750.61621f}, 0xa000);
        HLib::SetCurrentTaskTo(Branch1ExitCallback);
    } else {
        EV_stepSetFlags(1, 725);
        EV_PTYCUtils(shenhua, 1, 55);
        AssignAbsoluteEvent(12, {-210.6039f, 100.0f, -750.61621f}, 0xa71c);
        HLib::SetCurrentTaskTo(Branch1Exit2Callback);
    }
    DispatchMazeEvent();
}
void Branch2ChoiceCallback(void *) {
    if (eventParameters.state != -1 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 398) {
        EV_stepSetFlags(1, 726);
        EV_PTYCUtils(shenhua, 1, 42);
        AssignMazeEvent(24, {0.78079998f, 0.48429999f, 12.6735f}, 63688, 0xffffffffU);
        HLib::SetCurrentTaskTo(Branch2Exit2Callback);
    } else {
        EV_stepSetFlags(0, 726);
        EV_PTYCUtils(shenhua, 1, 58);
        AssignMazeEvent(25, {0.78079998f, 0.48429999f, 12.6735f}, 63688, 0xffffffffU);
        HLib::SetCurrentTaskTo(Branch2ExitCallback);
    }
    DispatchMazeEvent();
}
void FollowSetupCallback(void *) {
    AssignMazeEvent(27, {-0.56849998f, 0.0f, 11.8687f}, 44676, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(FollowExitCallback);
}
void Branch1SetupCallback(void *) {
    SetRenderClockAndCompassTo(1);
    AssignAbsoluteEvent(9, {-210.6039f, 100.0f, -750.61621f}, 0xa71c);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(Branch1ChoiceCallback);
}
void CottonLeftSetupCallback(void *) {
    AssignMazeEvent(28, {-1.7697999f, -0.0307f, 2.8343999f}, 62074, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(CottonExitCallback);
}
void CottonRightSetupCallback(void *) {
    AssignMazeEvent(29, {2.5081999f, -0.0307f, -0.035f}, 19901, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(CottonExitCallback);
}
void Branch2SetupCallback(void *) {
    SetRenderClockAndCompassTo(1);
    AssignMazeEvent(23, {3.6257f, 0.48429999f, 12.6213f}, 0x4000, 0xffffffffU);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(Branch2ChoiceCallback);
}
void WaterfallSetupCallback(void *) {
    AssignMazeEvent(22, {0.0052999998f, 0.0f, 15.7692f}, 0, 8);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(WaterfallExitCallback);
}
void LastAuthorSetupCallback(void *) {
    AssignMazeEvent(4, {-0.81959999f, 0.0f, -0.16689999f}, 9178, 7);
    DispatchMazeEvent();
    if (entryFlags & 2)
        QueueImmediateNextFadeTask(nullptr);
    HLib::SetCurrentTaskTo(LastAuthorExitCallback);
}
void LastAuthorRouteSetupCallback(void *) {
    AssignMazeEvent(31, {-0.81959999f, 0.0f, -0.16689999f}, 9178, 7);
    DispatchMazeEvent();
    HLib::SetCurrentTaskTo(LastAuthorExitCallback);
}
void FinishLastAuthorCallback(void *) {
    SetRenderClockAndCompassTo(1);
    DateTime date;
    GetCurrentDateTime(date);
    if (date.hour < 17) {
        date.hour = 17;
        date.minute = 2;
    }
    SetCurrentDateTimeTo(date);
    shenToggleAreaFlags(1);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(KSH1), 0);
}
void LastAuthorExitCallback(void *) {
    camCtrlSetFarDistance(50.0f);
    if (eventParameters.state == -1)
        FinishLastAuthorCallback(nullptr);
}
void StartSetupCallback(void *) {
    AssignAbsoluteEvent(30, {-400.60001f, 100.0f, -801.29999f}, 0xe000);
    DispatchMazeEvent();
    QueueImmediateNextFadeTask(nullptr);
    HLib::SetCurrentTaskTo(StartExitCallback);
}
struct ForestTrigger {
    std::int32_t marker;
    float radius;
    std::int32_t story;
    std::uint32_t reserved;
    void (*prepare)();
    HLib::TaskCallback callback;
};
static_assert(sizeof(ForestTrigger) == 32);
static_assert(offsetof(ForestTrigger, prepare) == 16);
constexpr ForestTrigger triggers[]{
    {14, 5.0f, 724, 0, nullptr, FollowSetupCallback},
    {52, 2.0f, 725, 0, nullptr, Branch1SetupCallback},
    {41, 2.0f, 459, 0, nullptr, DandelionSetupCallback},
    {54, 2.0f, 449, 0, nullptr, CottonLeftSetupCallback},
    {55, 2.0f, 449, 0, nullptr, CottonRightSetupCallback},
    {56, 1.7f, 726, 0, nullptr, Branch2SetupCallback},
    {42, 20.0f, 727, 0, nullptr, WaterfallSetupCallback},
    {57, 2.0f, 792, 0, nullptr, LastAuthorRouteSetupCallback},
    {58, 2.0f, 792, 0, nullptr, LastAuthorSetupCallback},
    {1, 2.0f, 220, 0, nullptr, FinishLastAuthorCallback},
};
struct ForestRelocation {
    char label[16];
    std::int32_t story, clearStory, currentCompanion, companion;
    std::int32_t reserved[4];
    std::int32_t player;
    std::int32_t tail[5];
};
static_assert(sizeof(ForestRelocation) == 72);
static_assert(offsetof(ForestRelocation, story) == 16);
static_assert(offsetof(ForestRelocation, player) == 48);
constexpr ForestRelocation relocations[]{
    {"Y_JIRO2", 725, 0, 0, 5, {}, 52, {}},
    {"TAMPOPO", 459, 0, 0, 41, {}, 41, {}},
    {"GOURYU2", 449, 0, 1, 6, {}, 0, {}},
    {"Y_JIRO3", 726, 0, 0, 7, {}, 56, {}},
    {"IWAGESIKI", 727, 0, 0, 42, {}, 42, {}},
    {"GOURYU3", 792, 0, 1, 8, {}, 0, {}},
};
void UpdateDangerFlags() {
    Character *player = FindCharacterByScenarioName(ryo);
    if (IsCharacterWithinMazeMarkerRadius(player, 3, 10.0f) && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 0);
    }
    player = FindCharacterByScenarioName(ryo);
    if (IsCharacterWithinMazeMarkerRadius(player, 11, 15.0f) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
    }
    player = FindCharacterByScenarioName(ryo);
    if (IsCharacterWithinMazeMarkerRadius(player, 11, 7.0f)) {
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441)) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1440, 1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1441, 1);
        }
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1441) == 1 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1440) == 1)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1675, 1);
    }
}
bool CheckRelocation() {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1495))
        return false;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    for (const ForestRelocation &row : relocations) {
        if (!EV_GetShenmueFlag(row.story))
            continue;
        companionMarker = row.companion;
        playerMarker = row.currentCompanion ? static_cast<std::int32_t>(EV_PTYCUtils(shenhua, 1, -1)) : row.player;
        shenToggleAreaFlags(2);
        SetPlayerControlEnabled(0);
        if (row.clearStory)
            EV_stepSetFlags(0, row.story);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ChangePositionCallback);
        HLib::SetupTaskDelay(60);
        return true;
    }
    return false;
}
bool CheckTriggers() {
    for (std::uint32_t i = 0; i != 10; ++i) {
        const ForestTrigger &row = triggers[i];
        if (!IsCharacterWithinMazeMarkerRadius(FindCharacterByScenarioName(ryo), row.marker, row.radius) || !EV_GetShenmueFlag(row.story))
            continue;
        shenToggleAreaFlags(2);
        SetPlayerControlEnabled(0);
        if (row.prepare)
            row.prepare();
        HLib::SetCurrentTaskTo(row.callback);
        if (!forceEvent) {
            std::uint32_t delay;
            if (i == 9) {
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
bool CheckTimeOver() {
    if (!EV_GetShenmueFlag(1111))
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
void TimeOverLockCallback(void *) {
    HLib::SetCurrentTaskTo(TimeOverSetupCallback);
    if (!FindCharacterByScenarioName(shenhua)) {
        HLib::SetupTaskDelay(0);
        LoadCompanion(HLib::GetCurrentTask());
    }
}
void TimeOverSetupCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 902)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(LastAuthorSetupCallback);
    } else {
        AssignAbsoluteEvent(32, {-282.20001f, 100.0f, -781.29999f}, 0x7000);
        DispatchMazeEvent();
        HLib::SetCurrentTaskTo(TimeOverExitCallback);
    }
}
void TimeOverExitCallback(void *) {
    if (eventParameters.state != -1)
        return;
    spmFadeOut();
    HLib::SetCurrentTaskTo(MazeTimeOverTo552ECallback);
    HLib::SetupTaskDelay(90);
}
constexpr Vector3 cameraBounds[]{
    {-150.0f, 0.0f, -858.0f},
    {-176.5f, 0.0f, -847.5f},
    {-165.0f, 0.0f, -872.0f},
    {-179.0f, 0.0f, -857.200012207031f},
    {-169.0f, 0.0f, -855.0f},
    {-161.199996948242f, 0.0f, -870.5f},
    {-166.5f, 0.0f, -859.5f},
    {-186.0f, 0.0f, -853.700012207031f},
};
constexpr Vector3 cameraPositions[]{
    {-165.365493774414f, 27.3034992218018f, -844.325012207031f},
    {-178.828903198242f, 29.0624008178711f, -867.55078125f},
    {-156.587203979492f, 29.2964992523193f, -859.380126953125f},
    {-164.462600708008f, 34.7047996520996f, -846.694580078125f},
};
void EventBombCallback(void *) {
    if (GetCharacterMazeUnitIndex(FindCharacterByScenarioName(ryo)) == 5) {
        Vector3 playerPosition;
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
        const Vector3 *target = nullptr;
        if (EV_GetShenmueFlag(726)) {
            if (TestCharaPositionWithinRange(ryo, &cameraBounds[0], &cameraBounds[1], shen_name{0}) && playerPosition.y < 25.3f)
                target = &cameraPositions[0];
            else if (TestCharaPositionWithinRange(ryo, &cameraBounds[2], &cameraBounds[3], shen_name{0}))
                target = &cameraPositions[1];
        } else {
            if (TestCharaPositionWithinRange(ryo, &cameraBounds[4], &cameraBounds[5], shen_name{0}))
                target = &cameraPositions[2];
            else if (TestCharaPositionWithinRange(ryo, &cameraBounds[6], &cameraBounds[7], shen_name{0}))
                target = &cameraPositions[3];
        }
        camCtrlUtil(CameraUtilityMode::SetConversationPosition, reinterpret_cast<std::intptr_t>(target));
    }
    if (forceEvent || ryoNotBusy()) {
        UpdateDangerFlags();
        if (!CheckRelocation() && !CheckTriggers())
            CheckTimeOver();
    }
}
struct DandelionParticle {
    Vector3 position;
    Vector3i rotation;
    Vector3 velocity;
    std::int32_t reserved36, rotationIncrement, reserved44;
    Vector3 acceleration;
    std::int32_t field60, reserved64, angleZ, angleX, angleY, state;
};
static_assert(sizeof(DandelionParticle) == 84);
static_assert(offsetof(DandelionParticle, velocity) == 24);
static_assert(offsetof(DandelionParticle, acceleration) == 48);
static_assert(offsetof(DandelionParticle, angleX) == 72);
static_assert(offsetof(DandelionParticle, state) == 80);
void DandelionInitCallback(void *payload) {
    DandelionParticle *particles = static_cast<DandelionParticle *>(payload);
    for (std::int32_t i = 0; i != 500; ++i) {
        const std::int32_t angle = shenGetRandomInt32(90);
        particles[i].angleY = angle;
        particles[i].angleX = angle;
        particles[i].state = 0;
    }
    HLib::SetCurrentTaskTo(DandelionLoopCallback);
    HLib::SetupTaskDelay(0);
}
constexpr float dandelionAccelerationY[]{0.0003000000142492354f, 0.00019999999494757503f, 0.00009999999747378752f};
constexpr float dandelionAccelerationZ[]{-0.00009999999747378752f, -0.00004999999873689376f, -0.00002499999936844688f, -0.000011250000170548446f};
void DandelionLoopCallback(void *payload) {
    DandelionParticle *particles = static_cast<DandelionParticle *>(payload);
    Vector3 cameraTarget, cameraEye, groundPosition;
    camCtrlGetTargetPosition(&cameraTarget);
    camCtrlGetNextCamPos(&cameraEye);
    camCtrlGetTargetPosition(&groundPosition);
    groundPosition.y = 100.0f;
    groundPosition.y = shenFindIntersectionWithScene(groundPosition, groundPosition.y);
    const Vector3 zero{};
    EV_AT_SetPos(SHEN_NAME(dumy), EventTransformFlags::AllAxes, zero);
    Vector3 playerPosition;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    const Vector3 scale{3.0f, 3.0f, 3.0f};
    std::int32_t drawn = 0;
    for (std::int32_t i = 0; i != 500; ++i) {
        DandelionParticle &particle = particles[i];
        if (particle.state == 1) {
            particle.rotation.y += particle.rotationIncrement;
            particle.angleX += 455;
            particle.angleY += 655;
            particle.angleZ = 0;
            const std::int32_t angle = particle.angleX;
            particle.angleY = 0;
            particle.rotation.x = static_cast<std::int32_t>(((sin_fixed16(static_cast<std::int16_t>(angle)) * 36.0f) * 8192.0f) / 45.0f);
            particle.rotation.z = static_cast<std::int32_t>(((cos_fixed16(static_cast<std::uint16_t>(particle.angleY)) * 25.0f) * 8192.0f) / 45.0f);
            particle.velocity.x += particle.acceleration.x;
            particle.velocity.y += particle.acceleration.y;
            particle.velocity.z += particle.acceleration.z;
            particle.position.x += particle.velocity.x;
            particle.position.y += particle.velocity.y;
            particle.position.z += particle.velocity.z;
            Vector2 screen;
            PushProjectionPrePass(static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
            PushTransform(particle.position, screen);
            const float distance = GetEuclideanDistance(&cameraEye, &particle.position);
            if (distance >= 400.0f || particle.position.y >= ((cameraTarget.y + cameraEye.y) * 0.5f) + 5.0f) {
                const std::int32_t random = shenGetRandomInt32(90);
                particle.angleY = random;
                particle.angleX = random;
                particle.state = 0;
            } else if (screen.x >= 0.0f && screen.x <= 639.0f && screen.y >= 0.0f && screen.y <= 479.0f && distance < 225.0f && drawn < drawLimit) {
                shen_name name;
                if (distance < 27.0f)
                    name = SHEN_NAME(tmph);
                else if (distance < 64.0f)
                    name = SHEN_NAME(tmpm);
                else
                    name = SHEN_NAME(tmpl);
                ApplyTranslationRotationScaleXYZToCharaByID(name, 1.0f, &particle.position, &particle.rotation, &scale);
                ++drawn;
            }
        } else if (particle.state == 0) {
            if (particle.angleX <= 0) {
                particle.state = 1;
                const std::uint16_t yaw = static_cast<std::uint16_t>(shenGetRandomInt32(65536));
                const float radius = GetRandomNumberInRange(20.0f);
                particle.position.y = groundPosition.y;
                particle.position.x = ((sin_fixed16(static_cast<std::int16_t>(yaw)) * radius) + cameraEye.x) + 0.69999999f;
                particle.position.z = ((cos_fixed16(yaw) * radius) + cameraEye.z) + 1.0f;
                particle.velocity.x = 0.0f;
                particle.velocity.y = std::bit_cast<float>(0x3a83126fU);
                particle.acceleration.x = 0.0f;
                particle.acceleration.y = dandelionAccelerationY[shenGetRandomInt32(3)];
                particle.acceleration.z = dandelionAccelerationZ[shenGetRandomInt32(4)];
                particle.reserved36 = 0;
                particle.field60 = 728;
                particle.rotation.x = 0;
                particle.rotation.z = 0;
                particle.angleY = 0;
            } else
                --particle.angleX;
        }
    }
}
void NavigationStartCallback(void *) {
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    PlaceNaviMarker(position, 4);
    if (!startParameters)
        InitializeMazeNavigationTrail();
    HLib::DestroyCurrentTask();
}
void AdmitLastAuthor(std::int32_t variant) {
    entryFlags = variant;
    if (variant & 2) {
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(LastAuthorSetupCallback, 4, 0x0b, SHEN_NAME(bomS));
        SetPlayerControlEnabled(0);
        LoadCompanion(task);
        return;
    }
    if (variant)
        HLib::EnqueueTaskWithoutParameter(EventBombCallback, 4, 0x0b, SHEN_NAME(bomS));
    else {
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(StartSetupCallback, 4, 0x0b, SHEN_NAME(bomS));
        SetPlayerControlEnabled(0);
        LoadCompanion(task);
    }
    dandelionTask = HLib::EnqueueTaskWithParameter(DandelionInitCallback, 4, 0x0b, 0x14820, KMZ4_DANDELION_TASK);
    forceEvent = 0;
    drawLimit = 0;
}
} // namespace
void AreaKMZ4Init() {
    startParameters = 0;
    forceEvent = 0;
    drawLimit = 0;
    dandelionTask = nullptr;
    wakeTask = nullptr;
    killTask = nullptr;
    eventParameters = {};
    entryFlags = 0;
    companionMarker = 0;
    playerMarker = 0;
}
void AreaKMZ4Main(std::uint32_t parameters) {
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    EV_FreeAuthorPak(1, "0527");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    DateTime date;
    GetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 8) > 14U) {
        date.hour = 8;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
    std::int32_t variant = 1;
    if (!parameters) {
        areaFlags |= 3U;
        variant = 0;
    }
    startParameters = variant;
    if (g_entryPoint) {
        variant += 2;
        SetSceneInfos(g_actNumber, g_currentMap, 0);
    }
    AdmitLastAuthor(variant);
    HLib::Task *navigation = HLib::EnqueueTaskWithoutParameter(NavigationStartCallback, 4, 0x0b, TASK_NAVI_START.name);
    HLib::SetupTimedTask(navigation, 3);
    SetMazeUnitDrawDistance(65.0f);
}
void AreaKMZ4Cleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaKMZ4GameplayDefinition{SHEN_NAME(KMZ4), 0, AreaKMZ4Init, AreaKMZ4Main, AreaKMZ4Cleanup};
} // namespace shenmue::s2::areas
