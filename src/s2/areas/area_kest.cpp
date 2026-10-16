#include "s2/areas/area_kest.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::int32_t setupState, setupPhase, framesRemaining, frameTail, lastEntry;
HLib::Task *setupTask;
std::uint64_t reservedWork[3];
const shen_name characters[2]{KEST_SHENHUA, KEST_INVALID};
void LoadOrdinaryLights() {
    EV_lightLoad(KEST_LIGHT_DEFAULT);
    AssignLightProgram(2);
    AssignLightProgram(3);
    AssignLightProgram(9);
    AssignLightProgram(10);
}
void LoadSpecialLight() {
    EV_lightLoad(KEST_LIGHT_SPECIAL);
    AssignLightProgram(5);
}
void SetSequenceFog() {
    SetFogRed(20.0f);
    SetFogGreen(100.0f);
    SetFogBlue(.5f);
    SetFogDistance(0xffb9d3dfU);
    ReinitFogTasks(1);
}
void RevealScene() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void ResetCamera() {
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(static_cast<CameraUtilityMode>(0), 0);
    camCtrlSetState(CameraState::Init);
}
void SetSceneObjects(bool later) {
    if (later) {
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 1);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 1);
        EV_SetObjectFlagByIndex(4, 0);
        EV_SetObjectFlagByIndex(4, 0);
        EV_SetObjectFlagByIndex(6, 0);
        EV_SetObjectFlagByIndex(7, 1);
        EV_SetObjectFlagByIndex(8, 1);
        EV_SetObjectFlagByIndex(9, 1);
        EV_SetObjectFlagByIndex(10, 0);
    } else {
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 0);
        EV_SetObjectFlagByIndex(4, 1);
        EV_SetObjectFlagByIndex(5, 1);
        EV_SetObjectFlagByIndex(6, 1);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 0);
        EV_SetObjectFlagByIndex(9, 0);
        EV_SetObjectFlagByIndex(10, 1);
    }
}
void PositionPlayer(const Vector3 &position, const Vector3i &rotation) {
    EV_AT_SetPos(KEST_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(KEST_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(KEST_PLAYER), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(KEST_PLAYER), 71, 0, MotionRegion::LowerBody);
}
void QueueCamera(std::int32_t camera, float delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), KEST_EVENT_TASK);
    RegisterEventTask(task);
    EventSetCamera *const work = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    *work = EventSetCamera{1, camera, KEST_DUMMY, KEST_DUMMY, 0, {}, 0, delay};
}
} // namespace
// @verified - p
void AreaKESTInit() {
    setupState = 0;
    setupPhase = 0;
    framesRemaining = 0;
    frameTail = 0;
    setupTask = nullptr;
    for (std::uint64_t &work : reservedWork)
        work = 0;
    lastEntry = 0;
}
// @verified - p
void AreaKESTMain(std::uintptr_t parameter) {
    const std::int32_t entry = static_cast<std::int32_t>(parameter);
    if (entry == 0) {
        lastEntry = 0;
        return;
    }
    if (lastEntry == entry)
        return;
    if (entry == 1) {
        LoadOrdinaryLights();
        LoadSpecialLight();
        EV_AssignLightParams(1, 6);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_SetWeatherInstruction(0x0c, 1);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 0);
        EV_SetObjectFlagByIndex(4, 1);
        EV_SetObjectFlagByIndex(4, 1);
        EV_SetObjectFlagByIndex(6, 1);
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(8, 0);
        EV_SetObjectFlagByIndex(9, 0);
        EV_SetObjectFlagByIndex(10, 0);
    } else if (entry == 2) {
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_LoadAudio("KES1_000", 0);
        EV_AssignLightParams(1, 6);
        LoadOrdinaryLights();
        LoadSpecialLight();
        SetSceneObjects(false);
        SetSequenceFog();
        if (lastEntry != 0) {
            PositionPlayer({67.730003f, 98.730003f, 71.5f}, {0, 14836, 0});
            ResetCamera();
        }
        RevealScene();
        SetPlayerControlEnabled(1);
    } else {
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_LoadAudio("KES1_000", 1);
        EV_AssignLightParams(1, 6);
        LoadSpecialLight();
        LoadOrdinaryLights();
        SetSceneObjects(true);
        SetSequenceFog();
        if (entry == 4) {
            RevealScene();
            SetPlayerControlEnabled(1);
        } else {
            mtwkRequestMotion(scenFindMotionwork(KEST_PLAYER), 0xf085, MotionRegion::LowerBody);
            PositionPlayer({59.900002f, 100.59f, 38.580002f}, {0, 54497, 0});
            setupTask = HLib::EnqueueTaskWithoutParameter(KESTSetupCallback, 4, 0x0b, static_cast<std::int32_t>(KEST_TASK.name));
            setupPhase = 0;
            framesRemaining = 600;
            ptycCharaUnlock(KEST_SHENHUA);
            AssignAreaNpcLoadContext(characters, setupTask);
            g_usingCustomAuthorCharacterList = 0;
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            if (setupTask)
                HLib::SetupTimedTask(setupTask, 0);
            HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(KEST_LOAD_TASK.name));
            RevealScene();
        }
    }
    lastEntry = entry;
}
// @verified - p
void KESTSetupCallback(void *) {
    Vector3 playerPosition;
    EV_GetCharaPosition(KEST_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::Local);
    EV_AT_SetPos(KEST_DUMMY, EventTransformFlags::AllAxes, playerPosition);
    EV_AT_SetPos(KEST_SHENHUA, EventTransformFlags::AllAxes, Vector3{59.169998f, 100.6f, 38.220001f});
    EV_AT_SetAngY(KEST_SHENHUA, EventTransformFlags::AllAxes, Vector3i{0, 54497, 0});
    const Vector3 cameraScale{60.031502f, 102.2162f, 38.464401f};
    camCtrlSetScaleToAndRecalcFov(&cameraScale);
    EventSetCamera camera{0, 110, KEST_DUMMY, KEST_DUMMY, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::Task *const depthTask = HLib::EnqueueTaskWithParameter(KESTQuitCameraDepthCallback, 6, 0x0b, 4, KEST_EVENT_TASK);
    HLib::SetupTimedTask(depthTask, 120);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(depthTask)) = 1;
    QueueCamera(120, std::bit_cast<float>(1123024896U));
    QueueCamera(130, std::bit_cast<float>(1139802112U));
    --framesRemaining;
    HLib::SetCurrentTaskTo(KESTWaitCallback);
}
// @verified - p
void KESTQuitCameraDepthCallback(void *data) {
    camCtrlSetFlags();
    if (data != nullptr)
        HLib::DestroyCurrentTask();
}
// @verified - p
void KESTWaitCallback(void *) {
    --framesRemaining;
    if (framesRemaining <= 0) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(KESTReleaseCallback);
        HLib::SetupTaskDelay(30);
    }
}
// @verified - p
void KESTReleaseCallback(void *) {
    for (const shen_name *character = characters; character->name != KEST_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    RevealScene();
    ResetCamera();
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
const ScriptAreaDefinition areaKESTScriptDefinition{.areaId = AREA_KEST, .field04 = 0, .init = AreaKESTInit, .main = AreaKESTMain};
} // namespace shenmue::s2::areas
