#include "s2/areas/area_yuku.hpp"
#include "s2/scene_data.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2 {

std::uint64_t yukuFrameCounter{};
std::uint32_t yukuState{};
shen_name yukuEntry{};

// @verified - p
void YukuInit() {
    yukuFrameCounter = 0;
    yukuState = 0;
}

// @verified - p
void YukuMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    (void)HLib::EnqueueTaskWithoutParameter(YukuInitializeTask, 4, 0x0d, static_cast<std::int32_t>(YUKU_TASK.name));
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    yukuState = 0;
    yukuFrameCounter &= 0xffffffff00000000ULL;
    yukuEntry = *reinterpret_cast<const shen_name *>(argument);
    (void)EV_FreeAuthorPak(1, "0503");
}

// @verified - p
void YukuInitializeTask(void *) {
    const std::uint32_t frame = static_cast<std::uint32_t>(yukuFrameCounter) + 1U;
    yukuFrameCounter = (yukuFrameCounter & 0xffffffff00000000ULL) | frame;
    EV_set_debug_text_pos(2, 10);
    EV_debug_log_to_screen("%d", std::bit_cast<std::int32_t>(frame));
    if (yukuState != 0) {
        if (yukuState == 1 && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0)) {
            yukuFrameCounter &= 0xffffffff00000000ULL;
            yukuState = 0;
            HLib::SetCurrentTaskTo(yukuEntry.name == YUKU_STOP.name ? YukuStopCallback : YukuEventCallback);
        }
    } else {
        const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
        (void)EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("YUKU"));
        ++yukuState;
    }
}

// @verified - p
void YukuDormantNullsub(void *) {
}

// @verified - p
void YukuEventCallback(void *) {
    const std::uint32_t frame = static_cast<std::uint32_t>(yukuFrameCounter) + 1U;
    yukuFrameCounter = (yukuFrameCounter & 0xffffffff00000000ULL) | frame;
    EV_set_debug_text_pos(2, 10);
    EV_debug_log_to_screen("%d", std::bit_cast<std::int32_t>(frame));
    switch (yukuState) {
    case 0:
        yukuState = 1;
        break;
    case 1:
        (void)mtwkSetRequestedRootMode(scenFindMotionwork(YUKU_RYO), 0);
        (void)SetMotionFrameTime(scenFindMotionwork(YUKU_RYO), 0, MotionRegion::LowerBody);
        (void)SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 71, 0, MotionRegion::LowerBody);
        (void)mtwkSetExtraFlag(scenFindMotionwork(YUKU_RYO), 17, 0);
        EV_AT_SetPos(YUKU_RYO, EventTransformFlags::AllAxes, YUKU_LOOK_POSITION);
        (void)EV_AT_SetAngY(YUKU_RYO, EventTransformFlags::AllAxes, YUKU_LOOK_ROTATION, nullptr);
        (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YUKU"), 0);
        (void)EVS_LoopCamera(1000, YUKU_INVALID, YUKU_INVALID);
        (void)CopyMotionTransition(scenFindMotionwork(YUKU_RYO), &YUKU_WALK_POSITION, 10922, 0, 1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        ++yukuState;
        break;
    case 2:
        if (!mtwkSetExtraFlag(scenFindMotionwork(YUKU_RYO), 58, 1)) {
            (void)EVS_LoopCamera(1001, YUKU_RYO, YUKU_RYO);
            (void)EV_PlayDialogue("0503A001");
            ++yukuState;
            HLib::SetupTaskDelay(90);
        }
        break;
    case 3:
        (void)camCtrlSetFovTransitionTo(true);
        (void)camCtrlSetState(CameraState::Init);
        (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YUKU"), 0);
        (void)EV_FreeAuthorPak(0, "0503");
        SetPlayerControlEnabled(1);
        (void)HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}

// @verified - p
void YukuStopCallback(void *) {
    const std::uint32_t frame = static_cast<std::uint32_t>(yukuFrameCounter) + 1U;
    yukuFrameCounter = (yukuFrameCounter & 0xffffffff00000000ULL) | frame;
    EV_set_debug_text_pos(2, 10);
    EV_debug_log_to_screen("%d", std::bit_cast<std::int32_t>(frame));
    switch (yukuState) {
    case 0:
        (void)SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 71, 0, MotionRegion::LowerBody);
        (void)mtwkSetExtraFlag(scenFindMotionwork(YUKU_RYO), 17, 0);
        EV_AT_SetPos(YUKU_RYO, EventTransformFlags::AllAxes, YUKU_STOP_POSITION);
        (void)EV_AT_SetAngY(YUKU_RYO, EventTransformFlags::AllAxes, YUKU_STOP_ROTATION, nullptr);
        (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YUKU"), 0);
        (void)EVS_LoopCamera(1000, YUKU_INVALID, YUKU_INVALID);
        (void)mtwkRequestMotion(scenFindMotionwork(YUKU_RYO), 0xf005, MotionRegion::LowerBody);
        (void)SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 66, 2, MotionRegion::LowerBody);
        (void)mtwkSetRequestedRootMode(scenFindMotionwork(YUKU_RYO), 1);
        (void)SetMotionFrameTime(scenFindMotionwork(YUKU_RYO), 15, MotionRegion::LowerBody);
        ++yukuState;
        break;
    case 1:
        if (SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 65, 1, MotionRegion::LowerBody)) {
            (void)mtwkRequestMotion(scenFindMotionwork(YUKU_RYO), 0xf003, MotionRegion::LowerBody);
            (void)EV_PlayDialogue("0503A003");
            (void)SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 66, 2, MotionRegion::LowerBody);
            HLib::SetupTaskDelay(70);
            ++yukuState;
        }
        break;
    case 2:
        if (SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 65, 1, MotionRegion::LowerBody)) {
            (void)mtwkRequestMotion(scenFindMotionwork(YUKU_RYO), 0xf024, MotionRegion::LowerBody);
            ++yukuState;
        }
        break;
    case 3:
        if (SetMotionRegionFlag(scenFindMotionwork(YUKU_RYO), 65, 1, MotionRegion::LowerBody)) {
            (void)camCtrlSetFovTransitionTo(false);
            (void)camCtrlSetState(CameraState::Init);
            (void)CopyMotionTransition(scenFindMotionwork(YUKU_RYO), &YUKU_RETURN_POSITION, 10686, 5, 1);
            ++yukuState;
        }
        break;
    case 4:
        if (!mtwkSetExtraFlag(scenFindMotionwork(YUKU_RYO), 58, 1))
            ++yukuState;
        break;
    case 5:
        (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("YUKU"), 0);
        (void)EV_FreeAuthorPak(0, "0503");
        SetPlayerControlEnabled(1);
        (void)HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}

const ScriptAreaDefinition yukuScriptDefinition{.areaId = AREA_YUKU, .field04 = 0, .init = YukuInit, .main = YukuMain};

} // namespace shenmue::s2
