#include "s1/task/tmex.hpp"

#include "s1/boot.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/player_control.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/file_loader.hpp"
#include "s1/motion.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/time.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/wzog.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);
extern std::array<std::uint8_t, 128> g_shenmueFlags;
void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);
extern std::uint32_t g_searchLockOnFlags;
extern std::int32_t g_searchAnyInputPressedLatched;
extern std::int32_t g_busy_reset;
extern std::int32_t g_searchAPressedLatched;

HLib::Task *g_tmexTask = nullptr;
std::uint8_t g_TMEX_Flag = 5;
std::uint8_t g_tmex_val = 0;
std::uint32_t g_TMEX_CurrentMode = 0;
TMEXLifecycleState g_TMEX_lifecycleState = TMEXLifecycleState::Inactive;
std::uint8_t g_TMEX_State = 0;
std::uint8_t g_TMEX_displayFlag = 0;
std::uint32_t g_tmexRequestFrames = 0;
std::uint32_t g_tmexTransitionFrames = 0;
std::uint32_t g_tmexAuthorFrames = 0;
std::uint8_t g_tmex_suppressKeepOrRevertPrompt = 0;
std::uint8_t g_TMEX_forceShowWatchNode162 = 0;
Vector3 g_tmpCameraLookAtV3_2{};
Vector3i g_TMEX_watchModelRotation{};
void *WatchModelFilePtr = nullptr;
std::uint32_t g_TMEX_auxHandsPackedTime = 0;
Vector3 g_tmpCameraPosV3{};
Vector3 g_tmpCameraLookAtV3_1{};
std::int32_t g_dCameraPerspective = 0;
std::int32_t g_TMEX_savedFieldOfView = 0;
std::int32_t g_TMEX_savedAuthorState = 0;
std::int32_t g_TMEX_savedTempGameState = 0;
std::uint8_t g_TMEX_savedRyoVisibilityFlag = 0;
std::uint8_t g_TMEX_savedRyoUnknown10Flag = 0;
std::uint16_t g_TMEX_savedUiState = 0;
std::uint8_t g_TMEX_savedSicnTaskActive = 0;
std::uint16_t g_TMEX_savedCurrentSicn = 0;
std::uint8_t g_TMEX_savedDefaultHelpTokenID = 0;
std::uint8_t g_TMEX_savedDButtonID = 0;

namespace {

constexpr std::int32_t kTMEXTaskToken = SHEN_NAME(TMEX);
constexpr std::int16_t kTMEXTaskIndex = 12;
constexpr std::size_t kTMEXOverlayIndex = 0x46;
constexpr std::uint16_t kTMEXInputMask = 0xf1f9U;
constexpr std::uint16_t kCloseInput = 0x0200U;
constexpr std::uint16_t kDateWindowInput = 0x0400U;
constexpr std::uint16_t kAuxiliaryHandsInput = 0xc000U;
constexpr std::uint16_t kEventStopActiveFlag = 0x320;
constexpr std::uint16_t kSleepTransitionModalActiveFlag = 0x0c2;
constexpr std::uint8_t kWristwatchHelpToken = 0x18;

TMEXRuntimeState g_tmexRuntime{};

PadCaptureEntry *CurrentMapPadEntry() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    return static_cast<PadCaptureEntry *>(control->padCaptureEntry);
}

Character *CurrentRyo() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    return static_cast<Character *>(control->ryo);
}

void MaskTMEXInput() {
    InputSlot &input = g_inputState.slots[0];
    input.held &= kTMEXInputMask;
    input.previousHeld &= kTMEXInputMask;
    input.pressedPriority &= kTMEXInputMask;
    input.repeatedPresses &= kTMEXInputMask;

    if (PadCaptureEntry *const pad = CurrentMapPadEntry()) {

        pad->enabled &= kTMEXInputMask;
        pad->unknownMask &= kTMEXInputMask;
        pad->held &= kTMEXInputMask;
        pad->previousHeld &= kTMEXInputMask;
    }
}

void CloseTMEXOverlay() {
    UIBaseRuntime &overlay = g_uiElements[kTMEXOverlayIndex];
    if ((g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kTMEXOverlayIndex)) &&
        (!overlay.isVisible || !overlay.destroy || overlay.destroy() == 1 || g_busy_reset == 1)) {
        overlay.isVisible = 0;
    }
}

bool IsTMEXOverlayReady() {
    return g_uiElements[kTMEXOverlayIndex].isVisible != 0;
}

Mt5RuntimeNode *FindWatchNode(std::uint16_t nodeKind) {
    auto *const root = static_cast<Mt5RuntimeNode *>(WatchModelFilePtr);
    std::byte *const base = root ? MT5_ModelBaseForNode(root) : nullptr;
    if (!root || !base)
        return nullptr;

    std::size_t visited = 0;
    const auto visit = [&](auto &&self, Mt5RuntimeNode *node) -> Mt5RuntimeNode * {
        while (node && visited++ < 4096) {
            if (node->nodeKind == nodeKind)
                return node;
            if (node->childOffset) {
                if (Mt5RuntimeNode *const found = self(self, reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset))) {
                    return found;
                }
            }
            node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
        }
        return nullptr;
    };
    return visit(visit, root);
}

bool LoadRyoWatch() {
    if (g_tmexRuntime.watchModelOwner) {
        WatchModelFilePtr = g_tmexRuntime.watchModelOwner->model;
        return true;
    }

    void *const file = ReadFile("model/item", "YKBT2U2G.MT5", SHEN_NAME(WATC));
    if (!file)
        return false;
    char fbxPath[] = "Scene/Misc/YKBT2U2G/YKBT2U2G.fbx";
    PackModel *const owner = MT5_InitRawModelAndTextures(file, "YKBT2U2G.MT5", fbxPath);
    if (!owner) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return false;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(owner->model), fbxPath, 0, 2);
    g_tmexRuntime.watchModelOwner = owner;
    WatchModelFilePtr = owner->model;

    return FindWatchNode(150) && FindWatchNode(151) && FindWatchNode(162) && FindWatchNode(152) && FindWatchNode(153);
}

// The world height TMEX_Process reads for one of Ryo's motion rows (its matrix's row 3), or his entity's when the row is missing.
float WatchRowHeight(Character *ryo, std::uint8_t nodeId) {
    MotionManagerWork *const manager = MOTM_GetWork(ryo);
    for (const MotionNodeRuntime *row = manager ? static_cast<const MotionNodeRuntime *>(manager->motion.previousNodes) : nullptr; row && row->nodeId != 0xffU;
         ++row) {
        if (row->nodeId == nodeId && row->transform)
            return row->transform->rows[3].y;
    }
    return GetPositionForEntity(ryo->entity.node).y;
}

void CaptureTMEXPresentation() {
    const UIPresentationSnapshot snapshot = UI_GetPresentationSnapshot();
    g_TMEX_savedUiState = snapshot.sicnState;
    g_TMEX_savedCurrentSicn = static_cast<std::uint8_t>(snapshot.sicnPausePolicy);
    g_TMEX_savedDefaultHelpTokenID = g_defaultHelpTokenId;
    g_TMEX_savedDButtonID = g_helpButtonId;
    g_TMEX_savedSicnTaskActive = static_cast<std::uint8_t>(UI_SICNIsTaskActive());

    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = kWristwatchHelpToken;
    g_currentHelpTokenId = kWristwatchHelpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    if (g_TMEX_savedSicnTaskActive) {
        UI_SICNSetState(3, UISicnPausePolicy::KeepOnPause);
    } else {
        UI_SICNEnqueue(3, UISicnPausePolicy::KeepOnPause);
    }
}

void RestoreTMEXPresentation() {
    g_previousHelpTokenId = g_currentHelpTokenId;
    std::uint8_t helpToken = g_TMEX_savedDefaultHelpTokenID;
    if (static_cast<std::int8_t>(helpToken) >= 68)
        helpToken = 0;
    g_currentHelpIconState = 0;
    g_defaultHelpTokenId = helpToken;
    g_currentHelpTokenId = helpToken;
    g_helpButtonId = g_TMEX_savedDButtonID;
    if (g_TMEX_savedSicnTaskActive) {
        UI_SICNSetState(g_TMEX_savedUiState, static_cast<UISicnPausePolicy>(static_cast<std::uint8_t>(g_TMEX_savedCurrentSicn)));
    } else {
        const std::uint16_t currentState = UI_GetPresentationSnapshot().sicnState;
        const std::uint16_t clearMask = static_cast<std::uint16_t>(((~currentState) & 0x0200U) | 0x0dffU);
        UI_SICNClearState(clearMask);
    }
}
void UpdateWatchModelNodes() {
    Mt5RuntimeNode *const hourHand = FindWatchNode(150);
    Mt5RuntimeNode *const minuteHand = FindWatchNode(151);
    Mt5RuntimeNode *const dateWindow = FindWatchNode(162);
    Mt5RuntimeNode *const auxiliaryMinute = FindWatchNode(152);
    Mt5RuntimeNode *const auxiliaryHour = FindWatchNode(153);
    if (hourHand) {
        hourHand->rotation.y = g_tmexRuntime.hourHandAngle;
    }
    if (minuteHand) {
        minuteHand->rotation.y = g_tmexRuntime.minuteHandAngle;
    }
    if (dateWindow) {
        if (g_tmexRuntime.showDateWindow) {
            dateWindow->scale.z = 1.0f;
            dateWindow->position.y = 0.0f;
            dateWindow->scale.y = 1.0f;
            dateWindow->scale.x = 1.0f;
        } else {
            dateWindow->position.y = -0.001f;
            dateWindow->scale.y = 0.0f;
            dateWindow->scale.z = 0.0f;
            dateWindow->scale.x = 0.0f;
        }
    }
    if (g_tmexRuntime.showAuxiliaryHands) {
        if (auxiliaryMinute) {
            auxiliaryMinute->rotation.y = g_tmexRuntime.auxiliaryMinuteHandAngle;
        }
        if (auxiliaryHour) {
            auxiliaryHour->rotation.y = g_tmexRuntime.auxiliaryHourHandAngle;
        }
    }
}

void UpdateWatchHands() {
    const std::int32_t hours = static_cast<std::int8_t>(g_currentDateTime.hours);
    const std::int32_t minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
    const std::int32_t seconds = static_cast<std::int8_t>(g_currentDateTime.seconds);
    g_tmexRuntime.hourHandAngle = static_cast<std::uint16_t>(-91 * minutes - 5461 * hours - seconds);
    g_tmexRuntime.minuteHandAngle = static_cast<std::uint16_t>(-18 * seconds - 1092 * minutes);

    const std::uint16_t held = g_tmpPadcHeld;
    g_tmexRuntime.showDateWindow = g_TMEX_forceShowWatchNode162 || (held & kDateWindowInput) != 0;
    g_tmexRuntime.showAuxiliaryHands = (held & kAuxiliaryHandsInput) != 0;
    if (g_tmexRuntime.showAuxiliaryHands) {
        const std::int32_t auxMinutes = static_cast<std::int16_t>(g_TMEX_auxHandsPackedTime & 0xffffU);
        const std::int32_t auxHours = static_cast<std::int16_t>(g_TMEX_auxHandsPackedTime >> 16U);
        g_tmexRuntime.auxiliaryMinuteHandAngle = -1092 * auxMinutes;
        g_tmexRuntime.auxiliaryHourHandAngle = -91 * auxMinutes - 5461 * auxHours;
    }
    UpdateWatchModelNodes();
    ++g_tmexRuntime.handUpdateCount;
    TIME_UpdateClockUI();
}




void RenderTMEXWatch() {
    g_tmexRuntime.overlayReady = IsTMEXOverlayReady();
    g_tmexRuntime.renderRequested = 0;
    if (!g_updateRenderFrameMode || !WatchModelFilePtr)
        return;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(g_tmpCameraLookAtV3_2.x, g_tmpCameraLookAtV3_2.y, g_tmpCameraLookAtV3_2.z);
    const std::int16_t rotation = static_cast<std::int16_t>(g_TMEX_watchModelRotation.y);
    const float sine = sin_fixed16(rotation);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - rotation));
    const float alpha = workTransformMatrix.rows[2].w * cosine;
    Matrix4x4::RotateAroundY(sine, cosine);
    const bool cached = CachedAttachmentOwnsSubmission(WatchModelFilePtr, alpha, workTransformMatrix);
    ::shenmue::Matrix4x4::Pop(1);
    if (cached) {
        static_cast<void>(SubmitRiggedSceneObjectForDrawing(WatchModelFilePtr, nullptr, 0, 0));
    } else {
        ::shenmue::matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(g_tmpCameraLookAtV3_2.x, g_tmpCameraLookAtV3_2.y, g_tmpCameraLookAtV3_2.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_TMEX_watchModelRotation.y));
        d3t::MT5_SubmitObjectNode(WatchModelFilePtr, 1.0f);
        ::shenmue::Matrix4x4::Pop(1);
    }
    g_tmexRuntime.renderRequested = 1;
    ++g_tmexRuntime.renderCount;
}

} // namespace

TMEXRuntimeState &TMEX_GetRuntimeState() {
    return g_tmexRuntime;
}
void TMEX_ResetForceCancelRequest() {
    SetStatOrNotebookFlag(kEventStopActiveFlag, false);
    SetStatOrNotebookFlag(kSleepTransitionModalActiveFlag, false);
    g_TMEX_Flag = 5;
    g_tmex_val = 0;
}
void TMEX_SetAuxiliaryTime(std::uint32_t packedTime, bool forceDateWindow) {
    g_TMEX_auxHandsPackedTime = packedTime;
    g_TMEX_forceShowWatchNode162 = forceDateWindow ? 1 : 0;
}
bool TMEX_Process() {
    Character *const ryo = CurrentRyo();
    Mt5RuntimeNode *const dateWindow = FindWatchNode(162);
    Mt5RuntimeNode *const root = static_cast<Mt5RuntimeNode *>(WatchModelFilePtr);
    if (!ryo || !root || !dateWindow || !FindWatchNode(150) || !FindWatchNode(151) || !FindWatchNode(152) || !FindWatchNode(153)) {
        return false;
    }

    const bool wasVisible = (ryo->motionFlags & 4U) != 0;
    g_TMEX_savedRyoVisibilityFlag = wasVisible;
    g_TMEX_savedRyoUnknown10Flag = (ryo->flags.value & 0x10U) != 0;
    ryo->motionFlags &= static_cast<std::uint8_t>(~4U);
    if (g_bDebugLogVisibility && (ryo->flags.value & CharacterFlagHasCpct) != 0 && wasVisible)
        CHAR_LogNPCVisibility(ryo, false);
    ryo->flags.value &= ~0x10U;

    g_TMEX_savedFieldOfView = FieldOfView;
    g_tmpCameraPosV3 = g_CameraEyePos;
    g_dCameraPerspective = currentPerspectiveFov;
    g_TMEX_savedAuthorState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
    g_tmpCameraLookAtV3_1 = g_CameraTargetPos;
    g_TMEX_savedTempGameState = g_temporaryGameState;
    dateWindow->rotation.y = 0;
    dateWindow->scale.x = 0.0f;
    dateWindow->scale.y = 0.0f;
    dateWindow->scale.z = 0.0f;
    dateWindow->position.y = -0.001f;

    g_temporaryGameState = 0;
    ApplyPendingGameState(g_previousAuthorState);
    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_Reset();
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);

    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    TranslateCameraRelative({ryoPosition.x, WatchRowHeight(ryo, 47) + 0.12f, ryoPosition.z});

    std::int16_t yawSum = 0;
    for (const CharacterNode *node = &ryo->entity.node; node; node = node->child)
        yawSum = static_cast<std::int16_t>(yawSum + node->rotation.y);
    const std::int32_t yaw = -static_cast<std::int32_t>(yawSum);
    const std::int32_t foldedYaw = static_cast<std::uint32_t>(yaw) <= 0x8000U ? yaw : -yaw;
    g_tmpCameraLookAtV3_2 = {ryoPosition.x + sin_fixed16(static_cast<std::int16_t>(yaw)) * 0.2f, WatchRowHeight(ryo, 1),
        ryoPosition.z + sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedYaw)) * -0.2f};
    g_TMEX_watchModelRotation = {0, -yaw, 0};
    const std::int32_t auxiliaryMinutes = static_cast<std::int16_t>(g_TMEX_auxHandsPackedTime & 0xffffU);
    const std::int32_t auxiliaryHours = static_cast<std::int16_t>(g_TMEX_auxHandsPackedTime >> 16U);
    const std::int32_t hours = static_cast<std::int8_t>(g_currentDateTime.hours);
    const std::int32_t minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
    const std::int32_t seconds = static_cast<std::int8_t>(g_currentDateTime.seconds);
    FindWatchNode(150)->rotation.y = static_cast<std::uint16_t>(-91 * minutes - 5461 * hours - seconds);
    FindWatchNode(151)->rotation.y = static_cast<std::uint16_t>(-18 * seconds - 1092 * minutes);
    FindWatchNode(152)->rotation.y = -1092 * auxiliaryMinutes;
    FindWatchNode(153)->rotation.y = -91 * auxiliaryMinutes - 5461 * auxiliaryHours;
    root->rotation.y = 0x8000;

    if (!bLockCamera) {
        g_CameraTargetPos = g_tmpCameraLookAtV3_2;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    CAME_SetFieldOfView(1274);
    return true;
}

void RestoreTMEXWorldState() {

    if (!bLockCamera) {
        g_CameraTargetPos = g_tmpCameraLookAtV3_1;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    TranslateCameraRelative(g_tmpCameraPosV3);
    CAME_SetFieldOfView(g_dCameraPerspective);
    if (!bLockCamera)
        FieldOfView = g_TMEX_savedFieldOfView;

    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_Reset();
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = g_TMEX_savedAuthorState;
    ApplyPendingGameState(g_TMEX_savedAuthorState);
    g_temporaryGameState = g_TMEX_savedTempGameState;
    ApplyPendingGameState(g_TMEX_savedTempGameState);

    if (Character *const ryo = CurrentRyo()) {
        const bool wasVisible = (ryo->motionFlags & 4U) != 0;
        if (g_TMEX_savedRyoVisibilityFlag)
            ryo->motionFlags |= 4U;
        else
            ryo->motionFlags &= static_cast<std::uint8_t>(~4U);
        const bool visible = (ryo->motionFlags & 4U) != 0;
        if (g_bDebugLogVisibility && (ryo->flags.value & CharacterFlagHasCpct) != 0 && visible != wasVisible)
            CHAR_LogNPCVisibility(ryo, visible);
        if (g_TMEX_savedRyoUnknown10Flag)
            ryo->flags.value |= 0x10U;
        else
            ryo->flags.value &= ~0x10U;
    }
}

void ClearTMEXSharedItemResponse() {
    DestroySharedItemsMenuIfTrueResponse();
}

void TMEX_Callback(HLib::TaskData *taskData) {
    (void)taskData;
    const std::uint16_t inputFlags = static_cast<std::uint16_t>(g_sfxInputFlags);
    g_tmexRuntime.inputFlags = inputFlags;
    g_tmexRuntime.maskedInputFlags = g_TMEX_CurrentMode > 0 ? static_cast<std::uint16_t>(inputFlags & ~kCloseInput) : inputFlags;

    if ((Shenmue_nPauseState & 2) == 0) {
        switch (g_TMEX_lifecycleState) {
        case TMEXLifecycleState::Inactive:
            g_TMEX_lifecycleState = TMEXLifecycleState::Starting;
            g_TMEX_State = 1;
            break;
        case TMEXLifecycleState::Starting:
            if (LoadRyoWatch() && TMEX_Process()) {
                CaptureTMEXPresentation();
                g_TMEX_lifecycleState = TMEXLifecycleState::Active;
            }
            break;
        case TMEXLifecycleState::Active:
            if (g_uiElements[kUiHelpNew].isVisible || (g_tmexRuntime.maskedInputFlags & kCloseInput) == 0)
                UpdateWatchHands();
            else
                g_TMEX_lifecycleState = TMEXLifecycleState::DestroyRequested;
            break;
        case TMEXLifecycleState::DestroyRequested:
            DestroyTMEX();
            break;
        }
    }



    RenderTMEXWatch();
}

void EnqueueTMEX() {
    if (g_tmexTask || !g_isPlayerGameStateActive)
        return;

    g_tmexTask = HLib_EnqueueTaskWithoutParameter(TMEX_Callback, kTMEXTaskIndex, 0, kTMEXTaskToken);
    g_tmex_suppressKeepOrRevertPrompt = 1;
    g_TMEX_lifecycleState = TMEXLifecycleState::Starting;
    g_TMEX_displayFlag = 1;
    g_TMEX_State = 1;
    d3t::ActivateElementByIdx(static_cast<std::int32_t>(kTMEXOverlayIndex));
}

void DestroyTMEX() {
    HLib::Task *const task = g_tmexTask;
    if (task) {
        HLib_DestroyTask(task);
        g_tmexTask = nullptr;
        if (g_tmexRuntime.watchModelOwner) {
            RestoreTMEXWorldState();
            RestoreTMEXPresentation();
        }
        g_TMEX_State = 0;
        g_TMEX_forceShowWatchNode162 = 0;
        g_TMEX_lifecycleState = TMEXLifecycleState::Inactive;
        CloseTMEXOverlay();
        g_TMEX_displayFlag = 0;
        g_tmex_suppressKeepOrRevertPrompt = 0;
        CloseTMEXOverlay();
        shenExecuteAICACommand(0x200abU, 0, 0);
    }
    ClearTMEXSharedItemResponse();
}
std::uint32_t TMEXUtilities(std::uint32_t mode) {
    if (mode == static_cast<std::uint32_t>(TMEXUtilityMode::CloseSetResult0) || mode == static_cast<std::uint32_t>(TMEXUtilityMode::CloseSetResult1)) {
        DestroyTMEX();
        shenRefreshUI();
        UI_ResetConversationDetailsState();
        shenExecuteAICACommand(0x30babU, 0, 0);
        SetStatOrNotebookFlag(kEventStopActiveFlag, false);
        SetStatOrNotebookFlag(kSleepTransitionModalActiveFlag, false);
        g_TMEX_Flag = 5;
        g_tmex_val = mode == static_cast<std::uint32_t>(TMEXUtilityMode::CloseSetResult1) ? 1 : 0;
        g_TMEX_CurrentMode = 0;
        g_tmexValHigh = 0;
        return 1;
    }

    if (g_TMEX_Flag != 5 || g_tmex_val > 1U)
        return 0;

    SetStatOrNotebookFlag(kEventStopActiveFlag, true);
    g_TMEX_Flag = static_cast<std::uint8_t>(mode);
    g_tmex_val = 2;
    g_tmexRequestFrames = 0;
    g_tmexTransitionFrames = 0;
    g_tmexAuthorFrames = 0;
    MaskTMEXInput();
    g_TMEX_CurrentMode = mode != 0 ? 1U : 0U;
    (void)shenForceCancel(g_pControlPadController);
    return 1;
}

void Script_TMEXUtilities(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack)
        return;

    std::uint32_t result = 0;
    const auto *const params = static_cast<const TMEXUtilitiesParameters *>(rawParams);
    if (params) {
        if (params->operation == 0) {
            result = TMEXUtilities(params->mode);
        } else if (params->operation == 1) {
            result = g_tmex_val;
        }
    }
    context->stack->accumulator = result;
}

} // namespace shenmue::s1
