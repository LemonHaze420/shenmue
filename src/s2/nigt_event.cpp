#include "s2/nigt_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {

// @verified - p
void NIGTInit() {
}

// @verified - p
void NIGTMain(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NIGTMainCallback, 5, 0x0b, sizeof(NIGTTaskParameter), NIGT_TASK);
    NIGTTaskParameter *const params = static_cast<NIGTTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->authorCommand = *reinterpret_cast<const std::uint32_t *>(argument);
    params->callingParent = HLib::GetCurrentTask();
    params->state = 0;
    EV_FreeAuthorPak(1, NIGT_AUTHOR_PACKAGE);
    EV_NPC_NewLoadSw(0);
    (void)EV_SetNewSceneIndex();
    SetPlayerControlEnabled(0);
    HLib::SetupTaskDelay(-1);
}

// @verified - p
void NIGTMainCallback(void *taskData) {
    using enum CameraState;
    NIGTTaskParameter *const params = static_cast<NIGTTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        (void)mtwkRequestMotion(scenFindMotionwork(NIGT_RYO), 0xf003, MotionRegion::LowerBody);
        DateTime dateTime;
        GetCurrentDateTime(dateTime);
        (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, NIGT_YORU);
        params->state = dateTime.hour == 19 && dateTime.minute == 0 ? 1 : 3;
        break;
    }
    case 1:
        if (!filmCheckFadeTrackColour()) {
            EV_lightLoad(NIGT_LIGHT);
            PushGlobalLightingProgram();
            (void)CheckIsArgEquZero(1);
            (void)EV_SetObjectFlagByIndex(7, 1);
            (void)EV_SetObjectFlagByIndex(6, 0);
            (void)EV_LoadAudio(NIGT_AUDIO_BANK, 1);
            EV_DisableCharacterDisplay(NIGT_RYO);
            EV_ToggleCharacterFlag(NIGT_RYO);
            (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(params->authorCommand));
            // Preserve SetFadeIn's request bytes: ff 00 00 00 -> 00 00 00 00, duration 30.
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            (void)EV_spmBeginSampleExecute();
            params->state = 2;
        }
        break;
    case 2:
        if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
            EV_EnsureCharacterDraw(NIGT_RYO);
            EV_EnableCharacterDisplay(NIGT_RYO);
            (void)EVS_LoopCamera(0, shen_name{}, shen_name{});
            (void)camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            HLib::SetupTaskDelay(50);
            params->state = 4;
        }
        break;
    case 3:
        if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
            EV_EnsureCharacterDraw(NIGT_RYO);
            EV_EnableCharacterDisplay(NIGT_RYO);
            camCtrlSetState(Init);
            HLib::SetupTaskDelay(50);
            params->state = 4;
        }
        break;
    case 4:
        (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(NIGT_FINAL_AUTHOR.name));
        params->state = 5;
        break;
    case 5: {
        const std::uint32_t currentFrame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        const std::uint32_t framesRemaining = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining));
        if (framesRemaining - currentFrame == 30U)
            spmFadeOut();
        if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
            (void)EV_spmBeginSampleExecute();
            SetCharaKill(NIGT_YORU, 1);
            HLib::SetCurrentTaskTo(NIGTExitCallback);
        }
        break;
    }
    default:
        return;
    }
}

// @verified - p
void NIGTExitCallback(void *taskData) {
    NIGTTaskParameter *const params = static_cast<NIGTTaskParameter *>(taskData);
    EV_FreeAuthorPak(0, NIGT_AUTHOR_PACKAGE);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(NIGT_PACKAGE), 0, 0);
    HLib::EV_HLKillOtherHierarchyTask(params->self);
    EV_scenQueueLoad(g_actNumber, NIGT_RETURN_AREA, 1);
}

const ScriptAreaDefinition nigtScriptDefinition{.areaId = NIGT_AREA, .field04 = 0, .init = &NIGTInit, .main = &NIGTMain};

} // namespace shenmue::s2
