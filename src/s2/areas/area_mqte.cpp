#include "s2/areas/area_mqte.hpp"
#include "s2/author.hpp"
#include "s2/audio_data.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/maze.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
std::int32_t forestStage, forestFailures, forestRetries, forestRainStarted, forestTestObstacle, forestTestSelection;
std::int32_t forestLastCommand, forestShenfaTicks, forestSelectedCamera, forestFireflyCount;
float forestObstacleDistance, forestFireflyAmplitude, forestFireflyScale;
Vector3 forestObstaclePosition;
HLib::Task *forestDummyTask, *forestShenfaTask, *forestFireflyTask;
namespace {

void ForestRemovedLogger() {
}
void ClearForestWeather() {
    Weather_FreeActiveFIXC();
    EnsureWeatherFixcRegistered();
    EV_FreeWAIR();
    ResetWeatherOverrides();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
}
void ReleaseForestScenePackages() {
    for (const char *name : {"KWW1", "KWA1", "KWA2", "KWA3"})
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
}
void AlignForestDummies() {
    EVT_AT_SetPos(MQTE_DUMMY_RYO, MQTE_RYO, {0, 0, 0}, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0, 0);
    EVT_AT_SetPos(MQTE_DUMMY_SHENFA, MQTE_SHENFA, {0, 0, 0}, 0);
    EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0, 0);
}
void RestoreForestLighting() {
    EV_SetWeatherInstruction(12, 1);
    EV_SetWeatherInstruction(0x780, 1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}
std::int32_t SetupForest(ForestQteWork &work) {
    EV_SetNewSceneIndex();
    if (g_currentMap.name == MQTE_AREA.name) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("MQTETEST"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("MQTE"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("MQTE"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MQTE"), 0);
    EV_LoadCutsceneFromCurrentScene("0517A000");
    EV_FreeAuthorPak(1, "0517");
    ForestRemovedLogger();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, MQTE_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, MQTE_CAMERA);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, MQTE_DUMMY_RYO);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, MQTE_DUMMY_SHENFA);
    AlignForestDummies();
    LoadWeatherConditionData();
    for (shen_name name : {MQTE_RYO, MQTE_SHENFA}) {
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 0, reinterpret_cast<std::byte *>(forestHandPose), 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 1, reinterpret_cast<std::byte *>(forestHandPose), 0);
    }
    work.phase = g_currentMap.name == MQTE_AREA.name ? 3 : 1;
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    return 0;
}
void ForestEnd(void *data) {
    ForestQteWork &work = *static_cast<ForestQteWork *>(data);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    DateTime date;
    GetCurrentDateTime(date);
    if (date.hour < 19) {
        date.hour = 19;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
    }
    FinishLoading();
    EV_EventSetUpHelpIcon_0(0, 0);
    SetCharaKill(MQTE_DUMMY_RYO, 1);
    SetCharaKill(MQTE_DUMMY_SHENFA, 1);
    HLib::DestroyTaskInternal(forestDummyTask);
    if (forestShenfaTask)
        HLib::DestroyTaskInternal(forestShenfaTask);
    ET_AT_KillWork(work.characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    SetCharaKill(MQTE_BASE, 1);
    SetCharaKill(MQTE_CAMERA, 1);
    EV_FreeAuthorPak(0, "0517");
    for (const char *name : {"MQTETEST", "MQTE"})
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
    scenFreeEvent("MQTE.EV3");
    SetPlayerControlEnabled(0);
    EV_stepSetFlags(0, 0xeb);
    EV_scenQueueLoad(g_actNumber, MQTE_DESTINATION, 0);
}
void PositionForestRunners() {
    const std::span<const MazeRoad> roads = GetMazeRoads(forestStage);
    EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, roads[14].position, 0);
    EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, roads[14].yaw / 182.04445f, 0);
    EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, roads[18].position, 0);
    EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, roads[18].yaw / 182.04445f, 0);
    EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0x40000000U, 0);
    EVT_AT_SetMot(MQTE_SHENFA, 7514, 0, 1.1f, 0x40000000U, 0);
}
void SelectForestObstacle(ForestQteWork &work) {
    switch (GetMazeUnitAuthoredName(work.obstacle).name) {
    case SHEN_NAME(1RJ1):
        work.obstacle = 0;
        break;
    case SHEN_NAME(2RA1):
        work.obstacle = 1;
        break;
    case SHEN_NAME(13T1):
        work.obstacle = 2;
        break;
    case SHEN_NAME(2OB0):
        work.obstacle = 3;
        break;
    case SHEN_NAME(1OH0):
        work.obstacle = 4;
        break;
    case SHEN_NAME(1OH1):
        work.obstacle = 5;
        break;
    case SHEN_NAME(2OD0):
        work.obstacle = 6;
        break;
    case SHEN_NAME(1IK1):
        work.obstacle = 7;
        break;
    case SHEN_NAME(1OL0):
        work.obstacle = 8;
        break;
    case SHEN_NAME(23C1):
        work.obstacle = 9;
        break;
    case SHEN_NAME(1OR0):
        work.obstacle = 10;
        break;
    case SHEN_NAME(3OB1):
        work.obstacle = 11;
        break;
    case SHEN_NAME(3OD1):
        work.obstacle = 12;
        break;
    case SHEN_NAME(3OC1):
        work.obstacle = 13;
        break;
    case SHEN_NAME(3OF1):
        work.obstacle = 14;
        break;
    case SHEN_NAME(1IK2):
        work.obstacle = 15;
        break;
    case SHEN_NAME(33A1):
        work.obstacle = 16;
        break;
    case SHEN_NAME(33E1):
        work.obstacle = 17;
        break;
    default:
        break;
    }
}
std::int32_t BeginForestObstacle(ForestQteWork &work) {
    if (g_currentMap.name == MQTE_AREA.name) {
        forestTestObstacle = forestTestSelection;
        work.obstacle = static_cast<std::int16_t>(forestTestSelection);
        for (shen_name name : MQTE_TEST_OBJECTS)
            EV_DisableCharacterDisplay(name);
        EV_EnableCharacterDisplay(MQTE_TEST_OBJECTS[forestTestObstacle]);
        EVT_AT_SetPos(MQTE_BASE, MQTE_INVALID, {0, 10, 0}, 0);
        EVT_AT_SetAngY(MQTE_BASE, MQTE_INVALID, 0, 0);
        EVT_AT_SetPos(MQTE_TEST_OBJECTS[forestTestObstacle], MQTE_BASE, {0, 0, 0}, 0);
        EVT_AT_SetAngY(MQTE_TEST_OBJECTS[forestTestObstacle], MQTE_BASE, 0, 0);
    }
    const ForestObstacle &obstacle = forestObstacles[work.obstacle];
    EVT_AT_SetPos(work.character, MQTE_BASE, {obstacle.x, 0, obstacle.z + 8}, 0);
    EVT_AT_SetAngY(work.character, MQTE_BASE, 0, 0);
    const bool shenfa = work.character.name == MQTE_SHENFA.name;
    const shen_name dummy = shenfa ? MQTE_DUMMY_SHENFA : MQTE_DUMMY_RYO;
    EVT_AT_SetPos(dummy, work.character, {0, 0, 0}, 0);
    EVT_AT_SetAngY(dummy, work.character, 0, 0);
    SetForestObstacleCamera(dummy, work.obstacle);
    if (shenfa)
        EV_DisableCharacterDisplay(MQTE_RYO);
    else
        EV_EnableCharacterDisplay(MQTE_RYO);
    EVT_AT_SetMot(work.character, shenfa ? 7514 : 61446, 0, 1, 0x40000000U, 0);
    forestLastCommand = 0;
    work.command = 0;
    work.phase = 4;
    if (shenfa && shenGetRandomInt32(3) == 0) {
        const std::uint16_t kind = obstacle.commands[0].kind;
        if (kind == 2 || kind == 3)
            EVT_AT_SetVoice("0517B009", 1, 0); // @text EN: "Watch your step." | JA: "足元に気をつけて。"
        else if (kind == 4 || kind == 5)
            EVT_AT_SetVoice("0517B008", 1, 0); // @text EN: "Be careful." | JA: "気をつけて。"
    }
    return 0;
}
std::int32_t RunForestRoute(ForestQteWork &work) {
    if (work.state == 0) {
        forestFailures = 0;
        forestRainStarted = 0;
        work.completedUnits = 0;
        clmdUtil(ctrlGetCharaByName(MQTE_SHENFA), 20);
        if (!EV_GetScreenFilmIsNotShowing()) {
            EventFilmControl film{.cleanup = 0,
                .frames = 60,
                .startAlpha = 255,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 0,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = 0};
            EVT_AT_MenCtrl_Callback(&film);
        }
        PositionForestRunners();
        AlignForestDummies();
        QueueForestRoadCameras(work.completedUnits, 0);
        EV_LoadAudio("0517A000", 1);
        EV_spmBeginSampleExecute();
        ++work.state;
        return 0;
    }
    if (work.state != 1)
        return 0;
    UpdateForestCharacterRoute(MQTE_RYO);
    UpdateForestCharacterRoute(MQTE_SHENFA);
    UpdateForestShenfaSpacing();
    work.obstacle = static_cast<std::int16_t>(GetCharacterMazeUnitIndex(ctrlGetCharaByName(MQTE_SHENFA)));
    const std::int32_t end = static_cast<std::int32_t>(GetMazeRoads(forestStage).size()) - 30;
    if (ForestCharacterRoadIndex(MQTE_SHENFA) <= end) {
        if (work.obstacle != -1 && work.previousUnit != work.obstacle && GetMotionAnimationId(scenFindMotionwork(MQTE_RYO), MotionRegion::LowerBody) == 61446) {
            work.previousUnit = work.obstacle;
            Vector3 position;
            GetMazeUnitPosition(work.obstacle, &position);
            const std::int32_t yaw = static_cast<std::int32_t>(GetMazeUnitYaw(work.obstacle));
            EVT_AT_SetPos(MQTE_BASE, MQTE_INVALID, position, 0);
            EVT_AT_SetAngY(MQTE_BASE, MQTE_INVALID, yaw / 182.04445f, 0);
            SelectForestObstacle(work);
            BeginForestObstacle(work);
            work.state = 0;
        }
        return 0;
    }
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
    if ((forestFailures >= 5 && forestRetries <= 10) || g_entryPoint >= 90) {
        work.phase = 8;
        work.state = 0;
    } else {
        work.phase = forestStage == 2 ? 11 : 1;
        forestFailures = 0;
        ++forestStage;
        forestRetries = 0;
        work.state = 0;
    }
    return 60;
}
void UpdateForestObstacleTarget(ForestQteWork &work) {
    const ForestObstacle &obstacle = forestObstacles[work.obstacle];
    Vector3 position, base;
    Vector3i rotation;
    EV_GetCharaPosition(work.character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_GetCharaPosition(MQTE_BASE, CHARACTER_TRANSLATION_ROOT, &base, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(MQTE_BASE, &rotation, EventTransformFlags::World);
    float z = obstacle.z;
    for (std::int32_t i = 1; i <= work.command; ++i)
        z = static_cast<float>(static_cast<double>(z) - (obstacle.commands[i].kind > 1 || obstacle.commands[i - 1].kind > 1 ? 4.0 : 3.0));
    const ForestObstacleCommand &command = obstacle.commands[work.command];
    z += forestQteCommands[command.kind].offset * .5f;
    Vector3 target{0, 0, z};
    ApplyScenarioTranslationAndRotation(&base, &rotation, &target);
    forestObstaclePosition = target;
    const float x = position.x - target.x, dz = position.z - target.z;
    forestObstacleDistance = std::sqrt(x * x + dz * dz);
    const float cameraX = command.cameraOffset + obstacle.x;
    EVT_AT_SetPos(MQTE_CAMERA, MQTE_BASE, {cameraX, 0, z}, 0);
    EVT_AT_SetAngY(MQTE_CAMERA, MQTE_BASE, 0, 0);
}
std::int32_t ApproachForestObstacle(ForestQteWork &work) {
    UpdateForestObstacleTarget(work);
    if (work.character.name == MQTE_RYO.name) {
        if ((work.command ? 5.0f : 6.5f) > forestObstacleDistance)
            work.phase = 5;
        return 0;
    }
    const ForestShenfaMotion &motion = forestShenfaMotions[forestObstacles[work.obstacle].commands[work.command].kind];
    if (motion.distance <= forestObstacleDistance)
        return 0;
    const std::uint32_t flags = motion.mirrored ? 0x20000000U : 0;
    const std::int32_t delay = static_cast<std::int32_t>((motion.end - motion.start) / motion.speed);
    EVT_AT_SetMot(work.character, motion.animation, motion.start, motion.speed, flags, 0);
    EVT_AT_SetMot(work.character, 7514, 0, 1.1f, flags, delay);
    ++work.command;
    if (forestObstacles[work.obstacle].commands[work.command].kind == 7 || work.command == 3)
        work.phase = 7;
    return motion.end;
}
void WaitForestQte(void *data) {
    StepWaitQteWork &work = *static_cast<StepWaitQteWork *>(data);
    if (!work.phase) {
        SetShowQteFlag(static_cast<std::uint16_t>(work.button), static_cast<std::int16_t>(work.maximumFrames), 10);
        ++work.phase;
    } else if (work.phase == 1) {
        ++*work.elapsed;
        if (GetShouldRenderIcons(RenderIconSlot::Qte))
            return;
        *work.status = static_cast<std::uint16_t>(EV_GetCurrentQteStatus() & 0xfff);
        *work.succeeded = *work.status == work.button;
        const std::int32_t delay = work.referenceFrames - *work.elapsed;
        if (delay > 0)
            HLib::SetupTimedTask(work.task, delay);
        ++work.phase;
    } else if (work.phase == 2) {
        StepQteCallback callback = work.success;
        if (!*work.succeeded) {
            if (++forestFailures == 5 && !forestRainStarted) {
                forestRainStarted = 1;
                EV_UpdateWeatherForMinute(1, 0, 200, 1.5f);
            }
            callback = work.failure;
        }
        const std::int32_t delay = callback ? callback() : 0;
        if (delay > 0)
            HLib::SetupTimedTask(work.parent, delay);
        else
            HLib::ResolveDeferredCallback(work.parent);
        HLib::DestroyTask(work.task);
    }
}
std::int32_t BeginForestQte(ForestQteWork &work) {
    const ForestObstacle &obstacle = forestObstacles[work.obstacle];
    const std::uint16_t kind = obstacle.commands[work.command].kind;
    const bool adjacent = work.command > 0 && kind <= 1 && obstacle.commands[work.command - 1].kind <= 1;
    Loop3DSoundEffect(0x973);
    const ForestQteCommand &command = forestQteCommands[kind];
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(WaitForestQte, 6, 0x0b, sizeof(StepWaitQteWork), MQTE_EVENT);
    StepWaitQteWork &wait = *static_cast<StepWaitQteWork *>(HLib::GetTaskParameter(task));
    work.result.elapsed = 0;
    work.result.succeeded = 0;
    wait.success = command.success;
    wait.failure = command.failure;
    wait.task = task;
    wait.button = command.button;
    wait.maximumFrames = 4 * forestRetries + 16;
    wait.referenceFrames = adjacent ? 24 : 28;
    wait.elapsed = &work.result.elapsed;
    wait.status = &work.result.status;
    wait.succeeded = &work.result.succeeded;
    wait.phase = 0;
    wait.parent = HLib::GetCurrentTask();
    if (wait.maximumFrames > wait.referenceFrames - 4)
        wait.maximumFrames = wait.referenceFrames - 4;
    HLib::SetupTimedTask(wait.parent, 0);
    if (!adjacent)
        HLib::SetupTimedTask(task, 1);
    ++work.command;
    if (obstacle.commands[work.command].kind == 7 || work.command == 3) {
        work.phase = 7;
        forestLastCommand = 1;
    } else
        work.phase = 6;
    return 0;
}
std::int32_t FinishForestObstacle(ForestQteWork &work) {
    std::int32_t delay = 0;
    if (g_currentMap.name == MQTE_AREA.name) {
        delay = 60;
        work.phase = 3;
    } else if (work.character.name == MQTE_RYO.name) {
        if (++work.completedUnits == static_cast<std::int32_t>(GetMazeUnits().size())) {
            const std::span<const MazeRoad> roads = GetMazeRoads(forestStage);
            const MazeRoad &ryo = roads[roads.size() - 48];
            const MazeRoad &shenfa = roads[roads.size() - 45];
            EVT_AT_SetPos(MQTE_RYO, MQTE_INVALID, ryo.position, 30);
            EVT_AT_SetAngY(MQTE_RYO, MQTE_INVALID, ryo.yaw / 182.04445f, 30);
            EVT_AT_SetPos(MQTE_SHENFA, MQTE_INVALID, shenfa.position, 30);
            EVT_AT_SetAngY(MQTE_SHENFA, MQTE_INVALID, shenfa.yaw / 182.04445f, 30);
            EVT_AT_SetMot(MQTE_RYO, 61446, 0, 1.1f, 0x40000000U, 30);
            EVT_AT_SetMot(MQTE_SHENFA, 7514, 0, 1.1f, 0x40000000U, 30);
            EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0, 30);
            EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0, 30);
        }
        QueueForestRoadCameras(work.completedUnits, 30);
        if (forestShenfaTask)
            HLib::DestroyTaskInternal(forestShenfaTask);
        forestShenfaTask = nullptr;
        work.phase = 2;
        work.state = 1;
        EVT_AT_SetAngY(MQTE_DUMMY_RYO, MQTE_RYO, 0, 0);
        EVT_AT_SetAngY(MQTE_DUMMY_SHENFA, MQTE_SHENFA, 0, 0);
    } else {
        delay = 30;
        forestShenfaTask = HLib::EnqueueTaskWithoutParameter(ForestQteRunShenfa, 4, 0x0b, static_cast<std::int32_t>(MQTE_RUN_TASK.name));
        work.phase = 3;
        forestShenfaTicks = 30;
    }
    work.character = work.character.name == MQTE_RYO.name ? MQTE_SHENFA : MQTE_RYO;
    return delay;
}
std::int32_t RunForestAuthor(ForestQteWork &work, bool failure) {
    if (forestShenfaTask)
        HLib::DestroyTaskInternal(forestShenfaTask);
    forestShenfaTask = nullptr;
    ReleaseForestScenePackages();
    if (work.state) {
        if (work.state == 1) {
            UpdateTempHourMinTimeScale(23, 30, 35);
            if (DoesScenarioCharacterExist(MQTE_FIRE_DUMMY))
                SetCharaKill(MQTE_FIRE_DUMMY, 1);
            if (forestFireflyTask)
                HLib::DestroyTask(forestFireflyTask);
            forestFireflyTask = nullptr;
            SetMazeTerrainDrawEnabled(1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 1);
            EV_SetModelDataDefaultVertexShader();
            RestoreForestLighting();
            work.phase = work.phase == 1 ? 2 : 9;
            ReleaseForestScenePackages();
            SetMazeUnitDrawDistance(forestStage == 0 ? 50 : 60);
            work.state = 0;
            ForestQteMain(&work);
        }
        return 0;
    }
    clmdUtil(ctrlGetCharaByName(MQTE_SHENFA), 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work.retried[forestStage] == 1)
        EV_SetAuthorActiveTo(1);
    if (!failure) {
        DateTime date;
        GetCurrentDateTime(date);
        if (forestStage >= 0 && forestStage <= 2) {
            date.hour = static_cast<std::uint8_t>(16 + forestStage);
            date.minute = 30;
        }
        SetCurrentDateTimeTo(date);
        EV_FreeWAIR();
        EV_UpdateWeather();
    }
    UpdateTempHourMinTimeScale(23, 30, .001f);
    std::int32_t delay = 0;
    const char *package = nullptr;
    if (!failure) {
        ClearForestWeather();
        if (forestStage == 0)
            package = "KWW1";
        else if (forestStage == 1)
            package = "KWA1";
        else if (forestStage == 2)
            package = "KWA2";
        else {
            work.phase = 2;
            work.state = 0;
        }
    } else {
        EV_LoadAudio("0517A000", 2);
        if (forestStage == 0)
            package = "KWA1";
        else if (forestStage == 1)
            package = "KWA2";
        else {
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            work.phase = forestStage == 2 ? 10 : 9;
            work.state = 0;
            delay = 30;
        }
    }
    if (package)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetModelDataDefaultVertexShader();
    if ((forestStage == 1 && failure) || (forestStage == 2 && !failure)) {
        EV_SetWeatherInstruction(12, 0);
        EV_ReinitLights(2);
        EV_lightLoad(failure ? MQTE_FAILURE_LIGHT : MQTE_SUCCESS_LIGHT);
        PushGlobalLightingProgram();
    } else if (forestStage == 0 && !failure)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 0);
    else {
        RestoreForestLighting();
        EV_FreeWAIR();
        EV_UpdateWeather();
    }
    HLib::TaskCallback callback = nullptr;
    if (forestStage == 1 && !failure)
        EV_SetObjectFlagByIndex(3, 0);
    if (forestStage == 2 && !failure) {
        forestFireflyTask = HLib::EnqueueTaskWithParameter(ForestFireflyLoop, 6, 0x0b, sizeof(ForestFireflyWork), MQTE_FIRE_TASK);
        std::memset(HLib::GetTaskParameter(forestFireflyTask), 0, sizeof(ForestFireflyWork));
        CreateEventCharacter(CharacterCreationHighModel, nullptr, MQTE_FIRE_DUMMY);
        InitializeForestFireflies(0);
        callback = ForestFireflyAuthor;
    }
    if (work.phase == 1 || work.phase == 8) {
        SetMazeTerrainDrawEnabled(0);
        QueueSkAuthorStart(work.task, static_cast<std::int32_t>((failure ? MQTE_FAILURE : MQTE_SUCCESS).name), callback, nullptr, 0);
        ++work.state;
    }
    return delay;
}
std::int32_t ForestRetry(ForestQteWork &work) {
    const std::int16_t frame = work.state++;
    if (frame >= 90) {
        ++forestRetries;
        work.retried[forestStage] = 1;
        work.phase = 1;
        work.state = 0;
        ST_LoadTitleFade();
        ForestRemovedLogger();
        return 40;
    }
    if (!frame) {
        ClearForestWeather();
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        Queue_ST_LoadTitleFadeIn(MQTE_RETRY);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        FinishLoading();
    }
    return 0;
}
} // namespace
void ForestQteMain(void *data) {
    ForestQteWork &work = *static_cast<ForestQteWork *>(data);
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        delay = SetupForest(work);
        break;
    case 1:
        delay = RunForestAuthor(work, false);
        break;
    case 2:
        delay = RunForestRoute(work);
        break;
    case 3:
        delay = BeginForestObstacle(work);
        break;
    case 4:
        delay = ApproachForestObstacle(work);
        break;
    case 5:
        delay = BeginForestQte(work);
        break;
    case 6:
        UpdateForestObstacleTarget(work);
        work.phase = 4;
        return;
    case 7:
        delay = FinishForestObstacle(work);
        break;
    case 8:
        delay = RunForestAuthor(work, true);
        break;
    case 9:
        delay = ForestRetry(work);
        break;
    case 10:
        delay = ForestFinalFailure(work);
        break;
    case 11:
        spmFadeOut();
        HLib::SetCurrentTaskTo(ForestEnd);
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void QueueForestQte(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ForestQteMain, 4, 0x0b, sizeof(ForestQteWork), MQTE_TASK);
    forestDummyTask = HLib::EnqueueTaskWithoutParameter(ForestQteDummyControl, 5, 0x0b, static_cast<std::int32_t>(MQTE_DUMMY_TASK.name));
    ForestQteWork &work = *static_cast<ForestQteWork *>(HLib::GetTaskParameter(task));
    work = {};
    work.previousUnit = -1;
    work.task = task;
    work.character = MQTE_SHENFA;
    work.characters = Queue_EVT_AT_ReadChr(task, MQTE_CHARACTERS);
    forestFailures = forestRetries = forestRainStarted = forestStage = 0;
    forestObstacleDistance = 0;
}
void InitializeMqte() {
    forestFireflyAmplitude = .4f;
    forestStage = forestTestObstacle = forestTestSelection = 0;
    forestObstacleDistance = 0;
    forestObstaclePosition = {};
    forestShenfaTicks = forestLastCommand = 0;
    forestShenfaTask = forestDummyTask = nullptr;
    forestFailures = forestRetries = forestRainStarted = 0;
    forestFireflyTask = nullptr;
    forestSelectedCamera = 0;
    forestFireflyScale = .4f;
    forestFireflyCount = 150;
}
void RunMqte(std::uintptr_t) {
    forestTestObstacle = forestTestSelection = 0;
    SetPlayerControlEnabled(0);
    EV_EventSetUpHelpIcon_0(0x23, 0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), MQTE_EVENT);
    RegisterEventTask(task);
    StepNpcClearWork &work = *static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work.callback = QueueForestQte;
    work.callbackData = nullptr;
    work.reserved10 = 0;
    work.state = 0;
    work.delay = 3;
}
const ScriptAreaDefinition mqteScriptDefinition{.areaId = MQTE_AREA, .field04 = 0, .init = InitializeMqte, .main = RunMqte};
} // namespace shenmue::s2::areas
