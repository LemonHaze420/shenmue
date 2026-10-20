#include "s2/areas/area_qlgt.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {

void AreaQLGTInit() {
}
void AreaQLGTMain(std::uintptr_t parameter) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(qlgt::Start, 4, 0x0b, sizeof(qlgt::Work), qlgt::taskName);
    qlgt::Work &work = *static_cast<qlgt::Work *>(HLib::GetTaskParameter(task));
    work.reserved08 = 0;
    work.task = task;
    work.retry = 0;
    work.entry = parameter ? reinterpret_cast<const qlgt::Entry *>(parameter) : &qlgt::entries[g_entryPoint];
    qlgt::Prepare(work);
}
const ScriptAreaDefinition areaQLGTScriptDefinition{.areaId = qlgt::area, .field04 = 0, .init = AreaQLGTInit, .main = AreaQLGTMain};
namespace qlgt {
namespace {
void EventNull() {
}
void Gravity(bool enabled) {
    const NpcCharacterControllerState state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(ryo, 2, state);
    EV_SetCharaControllerFlags(ryo, 3, state);
}
void ReattachRenCamera() {
    AT_SetLocalBaseChara(renDummy, ren);
    EVT_AT_SetPos(renDummy, ren, {0, 0, 0}, 0);
    EVT_AT_SetAngY(renDummy, ren, 0, 0);
}
void FinishLecture(Work &work) {
    ReattachRenCamera();
    const std::int32_t frames = LectureFinish(work.endSide);
    Film(10, 0, 255, frames - 10);
    Film(10, 255, 0, frames);
    QueueSearchPose(work.startSide, frames);
    HLib::SetCurrentTaskTo(Search);
    HLib::SetupTaskDelay(frames + 10);
}
} // namespace
void Start(void *data) {
    Work &work = *static_cast<Work *>(data);
    const Course &course = *work.course;
    EV_npcUnknownFunc(ren);
    SetNaviDisplayOverride(false);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    FinishLoading();
    EVT_AT_SetPos(fail, invalid, {course.fail.x, work.height, course.fail.z}, 0);
    EVT_AT_SetAngY(fail, invalid, 0, 0);
    EVT_AT_SetPos(enemy, invalid, {course.failBattle.x, work.height, course.failBattle.z}, 0);
    EVT_AT_SetAngY(enemy, invalid, 0, 0);
    EVT_AT_SetPos(light, invalid, {course.battle.position.x, work.height, course.battle.position.z}, 0);
    EVT_AT_SetAngY(light, invalid, course.battle.yaw, 0);
    EVT_AT_SetPos(start, invalid, {course.start.position.x, work.height, course.start.position.z}, 0);
    EVT_AT_SetAngY(start, invalid, course.start.yaw, 0);
    EVT_AT_SetPos(end, invalid, {course.end.position.x, work.height, course.end.position.z}, 0);
    EVT_AT_SetAngY(end, invalid, course.end.yaw, 0);
    EVT_AT_SetPos(drum, enemy, {0, 0, 0}, 0);
    EVT_AT_SetAngY(drum, enemy, 0, 0);
    Vector3 position;
    AT_SetLocalBaseChara(enemy, start);
    EV_GetCharaPosition(enemy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.startSide = position.x < 0;
    AT_SetLocalBaseChara(enemy, end);
    EV_GetCharaPosition(enemy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.endSide = position.x < 0;
    AT_SetLocalBaseChara(enemy, invalid);
    work.waypoint = 0;
    work.grace = 10;
    PlaceEnemies(work, 1.5f);
    AT_SetLocalBaseChara(ryoDummy, ryo);
    AT_SetLocalBaseChara(renDummy, ren);
    EVT_AT_SetPos(ryoDummy, ryo, {0, 0, 0}, 0);
    EVT_AT_SetAngY(ryoDummy, ryo, 0, 0);
    EVT_AT_SetPos(renDummy, ren, {0, 0, 0}, 0);
    EVT_AT_SetAngY(renDummy, ren, 0, 0);
    EVT_AT_SetPos(ryo, start, {0, 0, 0}, 0);
    EVT_AT_SetPos(ren, start, {0, 0, 0}, 0);
    SetPluralControlFor(FindCharacterByScenarioName(ryo), 1);
    SetPluralControlFor(FindCharacterByScenarioName(ren), 1);
    EV_LoadAudio("0263A000", 0);
    EV_spmBeginSampleExecute();
    Film(30, 255, 0);
    const std::int32_t frames = work.entry->course ? Introduction(work.startSide, work.entry->course) : FirstIntroduction(work.startSide);
    ResetFlash(*work.flash);
    TrackDrumTransparency(true);
    work.remaining = frames;
    HLib::SetCurrentTaskTo(StartLectureWait);
}
void StartLectureWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (authGetFinished() || (work.retry && HasPressedCancel())) {
        work.remaining = 0;
        EV_CleanupMapControlTask();
        FlashWork &flash = *static_cast<FlashWork *>(HLib::GetTaskParameter(flashTask));
        if (work.entry->course) {
            SkipIntroduction(work.startSide);
            flash.index = 9;
            flash.remaining = 15;
        } else {
            SkipFirstIntroduction(work.startSide);
            flash.index = 15;
            flash.remaining = 20;
        }
    }
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(StartLecture);
}
void StartLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    SetNaviDisplayOverride(true);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    GetRandomNumberInRange(100);
    Camera(7011, renDummy);
    AT_SetLocalBaseChara(renDummy, invalid);
    // Only the chase prefix is initialized; the other 31 history samples remain allocator-owned bytes.
    std::memcpy(&work.chase, &chaseDefaults, offsetof(EventChaseWork, positions));
    EV_GetCharaPosition(ren, CHARACTER_TRANSLATION_ROOT, &work.chase.positions[0], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ren, &work.chase.rotations[work.chase.writeIndex], EventTransformFlags::World);
    EV_AT_SetPos(renDummy, EventTransformFlags::World | EventTransformFlags::AllAxes, work.chase.positions[work.chase.writeIndex]);
    EV_AT_SetAngY(renDummy, EventTransformFlags::World | EventTransformFlags::AllAxes, work.chase.rotations[work.chase.writeIndex]);
    EVT_AT_SetAngY(renDummy, start, work.startSide ? 90.0f : -90.0f, 0);
    HLib::SetCurrentTaskTo(RunLecture);
    HLib::SetupTaskDelay(1);
}
void RunLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.flash->transition) {
        if (authGetFinished() || (work.retry && HasPressedCancel()))
            FinishLecture(work);
    } else {
        SetMotionRegionFlag(scenFindMotionwork(ren), 71, 2, MotionRegion::LowerBody);
        work.movement = static_cast<std::int32_t>(QueueEventMovement(nullptr, ren, work.course->route, &routeMotions, 1, 0x80420000U));
        if (eventMovementStates[work.movement]) {
            EventMovementWork *movement = static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work.movement]));
            movement->index = work.waypoint;
            StartEventMovementSegment(movement);
            ConfigureEventMovementMotions(movement);
            movement->active = 1;
        }
        HLib::SetCurrentTaskTo(WaitLecture);
        HLib::SetupTaskDelay(10);
    }
}
void WaitLecture(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (!eventMovementStates[work.movement] || authGetFinished() || (work.retry && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        CancelEventMovement(work.movement);
        FinishLecture(work);
        return;
    }
    const std::int32_t waypoint =
        eventMovementStates[work.movement] ? static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work.movement]))->index : -1;
    if (work.waypoint == waypoint) {
        UpdateEventChase(&work.chase);
        return;
    }
    work.waypoint = waypoint;
    if (work.course->routeActions[waypoint] == 1) {
        CancelEventMovement(work.movement);
        const std::int32_t frames = EVT_AT_SetMot(ren, 0xf002, 0, 1, 0, 0);
        HLib::SetCurrentTaskTo(RunLecture);
        HLib::SetupTaskDelay(frames - 30);
    } else if (work.course->routeActions[waypoint + 1] == 1) {
        CancelEventMovement(work.movement);
        SetMotionRegionFlag(scenFindMotionwork(ren), 71, 2, MotionRegion::LowerBody);
        work.movement = static_cast<std::int32_t>(QueueEventMovement(nullptr, ren, work.course->route, &routeMotions, 1, 0x80220000U));
        SetEventMovementSegment(work.movement, work.waypoint);
    }
}
void EndLecture(void *) {
    using enum CameraState;
    EV_LoadAudio("0263A000", 1);
    Film(10, 255, 0);
    FinishLoading();
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EVT_AT_SetMot(ryo, 0xf002, 2, 1, 0x40000000U, 0);
    Gravity(true);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(Play);
}
void Play(void *data) {
    Work &work = *static_cast<Work *>(data);
    const Course &course = *work.course;
    SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const bool inside = TestCharaPositionWithinRange(ryo, &course.boundsFirst, &course.boundsSecond, shen_name{});
    const bool exposed = ComputeAngle(position, course.enemyRadius, 0, 65353, course.enemy.position);
    const bool failed = ComputeAngle(position, course.failRadius, 0, 65353, course.fail);
    const bool atStart = ComputeAngle(position, course.startRadius, 0, 65353, course.start.position);
    const bool atGoal = ComputeAngle(position, course.goalRadius, 0, 65353, course.goal);
    if (work.flash->state == 2 && !IsInSafeZone(course.safeZones) && exposed) {
        const std::int32_t grace = work.grace--;
        if (grace < 0)
            HLib::SetCurrentTaskTo(Fail);
        return;
    }
    if (failed)
        HLib::SetCurrentTaskTo(Fail);
    else if (atStart) {
        GetBattleDataConfig(0, battleData);
        GetBattleDataConfig(1, battleData);
        if (GetBattleDataConfig(2, battleData)) {
            SetPlayerControlEnabled(0);
            Gravity(false);
            Film(10, 0, 255);
            Film(10, 255, 0, 10);
            QueueSearchPose(work.startSide, 10);
            HLib::SetCurrentTaskTo(Search);
            HLib::SetupTaskDelay(10);
        }
    } else if (atGoal) {
        SetPlayerControlEnabled(0);
        EventNull();
        Film(10, 0, 255);
        HLib::SetCurrentTaskTo(Success);
        HLib::SetupTaskDelay(40);
    } else if (inside)
        work.grace = 10;
    else {
        SetPlayerControlEnabled(0);
        EventNull();
        const std::int32_t frames = ReturnDialogue();
        EVT_AT_SetPosAng(ryo, invalid, course.returnPosition.position.x, course.returnPosition.position.z, course.returnPosition.yaw, frames);
        Film(10, 0, 255);
        HLib::SetCurrentTaskTo(EndLecture);
        HLib::SetupTaskDelay(frames);
    }
}
void Search(void *data) {
    Work &work = *static_cast<Work *>(data);
    GetBattleDataConfig(0, battleData);
    GetBattleDataConfig(1, battleData);
    if (GetBattleDataConfig(2, battleData)) {
        Film(10, 0, 255);
        EVT_AT_SetPosAng(ryo, start, 0, -.7f, work.startSide ? 90.0f : -90.0f, 10);
        HLib::SetCurrentTaskTo(EndLecture);
        HLib::SetupTaskDelay(10);
    }
}
void Success(void *data) {
    Work &work = *static_cast<Work *>(data);
    Gravity(false);
    SetNaviDisplayOverride(false);
    EV_LoadAudio("0263A000", 0);
    Film(30, 255, 0);
    const std::int32_t frames = SuccessDialogue(work.endSide, work.entry->course);
    Film(30, 0, 255, frames - 30);
    if (frames == 60)
        spmFadeOut();
    else
        EVT_AT_FadeBGM(0, 1, frames - 60);
    work.remaining = frames;
    HLib::SetCurrentTaskTo(SuccessWait);
}
void SuccessWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (authGetFinished()) {
        work.remaining = 0;
        EV_CleanupMapControlTask();
    }
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(End);
}
void Fail(void *data) {
    Work &work = *static_cast<Work *>(data);
    SetPlayerControlEnabled(0);
    EventNull();
    Gravity(false);
    SetNaviDisplayOverride(false);
    EV_LoadAudio("0263A000", 2);
    ResetFlash(*work.flash);
    const std::int32_t frames = FailDialogue(work.endSide, work.entry->course);
    PlaceBattleEnemies(work, 3);
    HLib::SetCurrentTaskTo(FailWait);
    work.remaining = frames;
}
void FailWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(StartBattle);
}
void End(void *data) {
    Work &work = *static_cast<Work *>(data);
    Cleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == area.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
} // namespace qlgt
} // namespace shenmue::s2::areas
