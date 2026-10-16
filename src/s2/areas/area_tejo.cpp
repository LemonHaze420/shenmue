#include "s2/areas/area_tejo.hpp"
#include "s2/areas/tejo_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/step_event.hpp"
#include <cmath>
#include <limits>

namespace shenmue::s2 {
HLib::Task *tejoMainTask{};
HLib::Task *tejoQteTask{};
shen_name tejoProgression{};
std::int32_t tejoQteIndex{}, tejoWaitFlag{}, tejoExtendedRetry{};
std::intptr_t tejoRetryMode{};
RenQteWork tejoQteWork{};
RenQteEntry *tejoQteEntries{};
TejoDateCheckpoint tejoForceCheckpoint{};
// @verified - p
void TejoInit() {
    tejoMainTask = nullptr;
    tejoQteTask = nullptr;
    tejoProgression = shen_name{};
    tejoQteIndex = 0;
    tejoWaitFlag = 0;
    tejoRetryMode = 0;
    tejoExtendedRetry = 0;
    tejoForceCheckpoint = {};
    tejoQteWork = {};
    tejoQteEntries = nullptr;
}
// @verified - p
void TejoMain(std::uintptr_t) {
    PrepareTejoEvent();
    tejoMainTask = HLib::EnqueueTaskWithoutParameter(TejoProgressionCallback, 4, 0x0b, static_cast<std::int32_t>(TEJO_MAIN_TASK.name));
    Queue_EVT_UTL_LoadNPCChara(TEJO_MAIN_CHARACTERS, tejoMainTask);
}
// @verified - p
void TejoProgressionCallback(void *) {
    switch (tejoProgression.name) {
    case SHEN_NAME(TRAP):
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TTRP"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        ExecuteCurrAreaMainFunc("TTRP.EV3", 0);
        tejoProgression = TEJO_RATI;
        tejoWaitFlag = 311;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(RATI): {
        UploadBlurStrength(0.9F);
        SetConversationCameraCodesEnabled(0);
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        EV_stepSetFlags(0, 311);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("RATI"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        tejoProgression = TEJO_FIRST_QTE;
        tejoQteEntries = tejoLowerQteEntries;
        tejoWaitFlag = 312;
        if (!tejoRetryMode || tejoRetryMode == 3) {
            DateTime date;
            GetCurrentDateTime(date);
            if (date.hour >= 23)
                date.minute = 45;
            else
                ++date.hour;
            SetCurrentDateTimeTo(date);
            GetCurrentDateTime(eventSavedCurrentDateTime);
        } else if (tejoRetryMode < 4)
            SetCurrentDateTimeTo(eventSavedCurrentDateTime);
        SetQronFloor(5);
        ExecuteCurrAreaMainFunc("RATI.EV3", static_cast<std::uintptr_t>(tejoRetryMode));
        tejoRetryMode = 0;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    }
    case SHEN_NAME(TEJ1):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 312);
        QueueTejoQte();
        tejoProgression = TEJO_FORCE;
        return;
    case SHEN_NAME(FORC):
        if (tejoRetryMode)
            EV_SetAuthorActiveTo(1);
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 314);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 952, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TOPA"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        SetQronFloor(4);
        SetQronMapLightingEnabled(0);
        if (tejoRetryMode)
            SetCurrentDateTimeTo(tejoForceCheckpoint.date);
        else
            GetCurrentDateTime(tejoForceCheckpoint.date);
        ExecuteCurrAreaMainFunc("TOPA.EV3", static_cast<std::uintptr_t>(tejoRetryMode));
        tejoProgression = TEJO_SECOND_QTE;
        tejoQteEntries = tejoUpperQteEntries;
        tejoWaitFlag = 626;
        tejoRetryMode = 0;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(TEJ2):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 626);
        QueueTejoQte();
        tejoProgression = TEJO_HIDE;
        return;
    case SHEN_NAME(HIDE):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 627);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 954, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        for (std::int32_t index = 1; index < 32; ++index)
            if (GetMagicRoomFlag(index))
                SetMapLightingPlacementEnabled(0, index);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("HIDE"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        ExecuteCurrAreaMainFunc("HIDE.EV3", 0);
        tejoProgression = TEJO_ROOF;
        tejoWaitFlag = 628;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(ROOF): {
        EV_stepSetFlags(0, 628);
        DateTime date;
        GetCurrentDateTime(date);
        date.hour = 21;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 1);
        return;
    }
    case SHEN_NAME(YUAN):
        if (!filmCheckFadeTrackColour()) {
            CleanupTejoEvent();
            EV_SetScenarioTaskFlags(6);
            EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 30);
        }
        return;
    default:
        return;
    }
}
// @verified - p
void TejoWaitEventCallback(void *) {
    if (!EV_GetShenmueFlag(tejoWaitFlag))
        HLib::SetCurrentTaskTo(TejoProgressionCallback);
}
// @verified - p
void QueueTejoQte() {
    tejoQteTask = HLib::EnqueueTaskWithoutParameter(TejoQteCallback, 5, 0x0b, static_cast<std::int32_t>(TEJO_QTE_TASK.name));
    Queue_EVT_UTL_LoadNPCChara(TEJO_QTE_CHARACTERS, tejoQteTask);
    HLib::SetupTaskDelay(0);
    PrepareTejoQteResources();
}
// @verified - p
void TejoQteCallback(void *) {
    if (!tejoQteEntries[tejoQteIndex].type) {
        UploadBlurStrength(0.9F);
        BeginTejoEndAuthor();
        return;
    }
    if (tejoQteWork.hidePursuers)
        HideTejoPursuers();
    if (tejoQteWork.state.name == REN_QTE_INIT.name) {
        tejoQteWork.entry = tejoQteEntries + tejoQteIndex;
        if (std::fabs(tejoQteWork.entry->cameraMotionScale) <= std::numeric_limits<float>::min()) {
            UploadBlurStrength(0.9F);
            camCtrlToggleSmoothing(0);
            camCtrlSetYOffset(0);
        } else {
            UploadBlurStrength(0.45F);
            camCtrlToggleSmoothing(1);
            camCtrlSetMotionScaleFactor(tejoQteWork.entry->cameraMotionScale);
            camCtrlSetYOffset(tejoQteWork.entry->cameraYOffset);
        }
        if (tejoProgression.name == TEJO_FORCE.name && tejoQteIndex == 6)
            UploadBlurStrength(0.45F);
    }
    if (tejoQteWork.entry->type == 7 && (tejoQteWork.state.name == REN_QTE_SUCCESS.name || tejoQteWork.state.name == REN_QTE_FAILURE.name)) {
        switch (tejoQteWork.doorAttempts % 3) {
        case 1:
            tejoQteWork.entry->failureCameras = tejoDoorFirstRetryCameras;
            tejoQteWork.entry->failureVoices = tejoDoorFirstRetryFailureVoices;
            break;
        case 2:
            tejoQteWork.entry->failureCameras = tejoDoorSecondRetryCameras;
            tejoQteWork.entry->successVoices = tejoDoorSecondRetrySuccessVoices;
            tejoQteWork.entry->failureVoices = tejoDoorSecondRetryFailureVoices;
            break;
        default:
            tejoQteWork.entry->failureCameras = tejoDoorThirdRetryCameras;
            tejoQteWork.entry->failureVoices = tejoDoorThirdRetryFailureVoices;
            break;
        }
    }
    if (!UpdateRenQteEvent(&tejoQteWork))
        return;
    ++tejoQteIndex;
    if (tejoQteWork.lives > 0)
        return;
    CleanupTejoQte();
    if (tejoQteWork.retryCount < 5)
        ++tejoQteWork.retryCount;
    if (tejoProgression.name == TEJO_HIDE.name) {
        if (tejoQteWork.retryCount <= 3) {
            tejoProgression = TEJO_FORCE;
            tejoRetryMode = 1;
            return;
        }
    } else if (tejoProgression.name == TEJO_FORCE.name) {
        tejoRetryMode = tejoQteWork.retryCount != 1 ? 2 : 1;
        tejoProgression = TEJO_RATI;
        return;
    }
    const bool previousRetry = tejoExtendedRetry != 0;
    tejoExtendedRetry = 1;
    tejoRetryMode = previousRetry ? 4 : 3;
    tejoProgression = TEJO_RATI;
}
// @verified - p
void TejoEndAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    CleanupTejoQte();
    tejoQteWork.retryCount = 0;
}
const ScriptAreaDefinition tejoScriptDefinition{.areaId = AREA_TEJO, .field04 = 0, .init = TejoInit, .main = TejoMain};
} // namespace shenmue::s2
