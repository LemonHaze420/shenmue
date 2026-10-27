#include "s2/areas/area_qlgt.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
namespace shenmue::s2::areas::qlgt {
HLib::Task *flashTask;
namespace {
void EventNull() {
}
void SetFlashLights(FlashWork &work, std::int32_t enabled) {
    for (std::int32_t i = 1; i <= work.pattern->lights; ++i)
        SetLightSceneIntensity(i, static_cast<float>(enabled));
    constexpr shen_name first{SHEN_NAME(F060)};
    for (std::uint32_t i = 0; i < 10; ++i)
        SetDisplayFlagForProp(shen_name{first.name + (i << 24)}, enabled == 0);
}
void Flash(void *data) {
    FlashWork &work = *static_cast<FlashWork *>(data);
    if (work.delay > 0) {
        --work.delay;
        return;
    }
    const std::int32_t remaining = work.remaining--;
    if (remaining > 0) {
        work.transition = 4;
        if (work.state == 1)
            SetFlashLights(work, ((remaining - 1) & 1) ? 0 : 1);
        return;
    }
    work.index = (work.index + 1) % 16;
    work.remaining = work.pattern->frames[work.index];
    work.state = work.pattern->states[work.index];
    work.transition = work.state;
    const float random = GetRandomNumberInRange(100);
    switch (work.state) {
    case 0:
        SetFlashLights(work, 0);
        Loop3DSoundEffect(0x570, .0001f);
        break;
    case 1:
        Loop3DSoundEffect(random < 30 ? 0x579 : random < 60 ? 0x57a : 0x57b, random < 30 ? 30.0f : 60.0f);
        break;
    case 2:
        SetFlashLights(work, 1);
        Loop3DSoundEffect(random >= 50 ? 0x571 : 0x56f, 50.0f);
        break;
    case 3:
        ResetFlash(work);
        break;
    default:
        break;
    }
}
} // namespace
FlashWork *QueueFlash(const FlashPattern *pattern) {
    if (!pattern)
        return nullptr;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Flash, 4, 0x0b, sizeof(FlashWork), taskName);
    FlashWork *work = static_cast<FlashWork *>(HLib::GetTaskParameter(task));
    flashTask = task;
    work->task = task;
    work->delay = 60;
    work->pattern = pattern;
    work->index = 0;
    work->remaining = pattern->frames[0];
    work->state = pattern->states[0];
    work->transition = work->state;
    work->reserved20 = 0;
    return work;
}
void ResetFlash(FlashWork &work) {
    work.index = 0;
    work.remaining = work.pattern->frames[0];
    work.state = work.pattern->states[0];
}
void Prepare(Work &work) {
    EV_DisableCharacterDisplay(hiddenDoors[work.entry->course]);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    GetCurrentDateTime(eventTemporaryDateTime);

    eventMovementStates.fill(0);
    for (shen_name door : doors)
        ClaimDoor(door, doorOwner);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work.height = position.y;
    work.course = &courses[work.entry->course];
    for (std::int32_t i = 0; i < work.course->enemyCount; ++i)
        actors[2 + i] = work.course->enemies[i];
    work.characters = Queue_EVT_AT_ReadChr(work.task, actors);
    work.flash = QueueFlash(work.course->pattern);
    work.motionBank = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    work.previousNavigation = SetNaviDisplayOverride(false);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QLGT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QLGT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QLGT"), 0);
    EV_FreeAuthorPak(1, "0263");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0263A000");
    shenSetClockAndCompassRenderTo(1);
    EventNull();
    for (std::int32_t i = 1; i < 10; ++i)
        EV_SetObjectFlagByIndex(i, 0);
    const std::int32_t slot = lightSlots[work.entry->course];
    EV_SetObjectFlagByIndex(slot, 1);
    EV_SetObjectFlagByIndex(slot + 1, 1);
    SetDefaultVSByIndex(slot);
    SetDefaultVSByIndex(slot + 1);
    EV_lightLoad(lightDefinitions[work.entry->course]);
    EV_EventSetUpHelpIcon_0(0x58, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 679, 1);
}
void Cleanup(Work &work) {
    const Course &course = *work.course;
    SetPluralControlFor(FindCharacterByScenarioName(ryo), 0);
    SetPluralControlFor(FindCharacterByScenarioName(ren), 0);
    EventSetPositionAngle position{0, ryo, invalid, {course.ryoExit.position.x, 0, course.ryoExit.position.z}, course.ryoExit.yaw, 0};
    EVT_AT_SetPosAng_Callback(&position);
    position.character = ren;
    position.position = {course.renExit.position.x, 0, course.renExit.position.z};
    position.degrees = course.renExit.yaw;
    EVT_AT_SetPosAng_Callback(&position);
    EV_npcUpdateFlags(ren);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 679, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    for (std::int32_t i = 1; i < 10; ++i)
        EV_SetObjectFlagByIndex(i, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(lightSlots[work.entry->course]);
    AssignLightProgram(lightSlots[work.entry->course] + 1);
    SetQronFloor(static_cast<std::int32_t>(GetQronNextStream()));
    ExecuteCleanupFuncForAreaWithCachedScene();
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0263");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QLGT"), 0);
    SetNaviDisplayOverride(work.previousNavigation != 0);
    DeleteMotion(work.motionBank);
    for (shen_name door : doors)
        ReleaseDoor(static_cast<std::int32_t>(door.name), doorOwner);
    for (std::int32_t i = 0; i < 20; ++i)
        CancelEventMovement(i);
    if (work.flash)
        HLib::EV_HLKillOtherHierarchyTask(work.flash->task);
    ET_AT_KillWork(work.characters);
    EV_NPC_NewLoadSw(1);
    Film(30, 255, 0);
    EV_EnableCharacterDisplay(hiddenDoors[work.entry->course]);
    EV_stepSetFlags(1, work.entry->step);
}
void TrackDrumTransparency(bool opaque) {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    if (!FindCharacterByScenarioName(drum))
        return;
    float alpha = 1;
    if (!opaque) {
        Vector3 position;
        EV_GetCharaPosition(drum, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float x = camera.x - position.x, y = camera.y - position.y, z = camera.z - position.z;
        alpha = static_cast<float>((static_cast<double>(std::sqrt(x * x + y * y + z * z)) - .6) / 2.4);
        if (alpha < 0)
            alpha = 0;
        else if (alpha > .7f)
            alpha = 1;
    }
    SetScenarioCharacterTransparency(drum, alpha);
    EV_SetCharacterAlphaEnabled(drum, alpha < 1);
}
void PlaceEnemies(Work &work, float radius) {
    const float step = 360.0f / static_cast<float>(work.course->enemyCount);
    for (std::int32_t i = 0; i < work.course->enemyCount; ++i) {
        const float degrees = static_cast<float>(i) * step;
        const std::int16_t angle = static_cast<std::int16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
        const float x = cos_fixed16(static_cast<std::uint16_t>(angle)) * radius, z = -(sin_fixed16(angle) * radius);
        EVT_AT_SetPos(actors[2 + i], enemy, {x, 0, z}, 0);
        EVT_AT_SetAngY(actors[2 + i], enemy, degrees + 90, 0);
        EVT_AT_SetMot(actors[2 + i], idleMotions[i], 0, 1, 0x40000000U, 0);
        SetScenarioCharacterDrawControl(actors[2 + i], 0, 0);
        SetPluralControlFor(FindCharacterByScenarioName(actors[2 + i]), 1);
    }
}
void PlaceBattleEnemies(Work &work, float radius) {
    const float step = 360.0f / static_cast<float>(work.course->enemyCount);
    for (std::int32_t i = 0; i < work.course->enemyCount; ++i) {
        const float degrees = static_cast<float>(i) * step;
        const std::int16_t angle = static_cast<std::int16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
        const float x = cos_fixed16(static_cast<std::uint16_t>(angle)) * radius, z = -(sin_fixed16(angle) * radius);
        EVT_AT_SetPos(actors[2 + i], battle, {x, 0, z}, 230);
        EVT_AT_SetAngY(actors[2 + i], battle, degrees + 150, 230);
        EVT_AT_SetMot(actors[2 + i], battleMotions[i], 0, 1, 0x40000000U, 230);
    }
}
bool IsInSafeZone(const SafeZone *zones) {
    if (!zones)
        return false;
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    for (std::int32_t i = 0; i < 4 && zones[i].enabled; ++i)
        if (ComputeAngle(position, zones[i].radius, 0, 65353, zones[i].centre))
            return true;
    return false;
}
} // namespace shenmue::s2::areas::qlgt
