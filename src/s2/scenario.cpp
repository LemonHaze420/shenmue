#include "s2/scenario.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


ScenarioTaskData *ScenarioData(HLib::Task *task) {
    return task ? static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task)) : nullptr;
}

Character *ScenarioRyo() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}



ScenarioEventRuntime *AllocateScenarioEventRuntime(HLib::Task *task) {
    scenarioEventRuntimeStorage = {};
    scenarioEventRuntimeStorage.task = task;
    for (ScenarioFreeTalkControl &state : GetScenarioTalkPresentationState().characterStates) {
        state.flags = (state.flags & 0xffffffe1U) | 4U;
    }
    scenarioCharacterAdmissionNames = {};
    scenarioCandidatePool = {};
    scenarioCandidateList = nullptr;
    scenarioCandidateFreeList = scenarioCandidatePool.data();
    for (std::size_t index = 1; index < scenarioCandidatePool.size(); ++index)
        scenarioCandidatePool[index - 1].previous = &scenarioCandidatePool[index];
    scenarioPersistentFlags.initialized = false;
    activeScenarioEventRuntime = &scenarioEventRuntimeStorage;
    InitializeScenarioConversation();
    for (SCENDialogueCue &cue : scenarioDialogueCues) {
        cue.triggerTime = ScenarioLineClock();
        cue.payload = nullptr;
        cue.speaker = {};
        cue.reserved14 = 0;
    }
    scenarioLineHoldFrames = 0;
    scenarioLineTimeOffset = 0.0f;
    scenarioCurrentDialogue = nullptr;
    scenarioFlagNotificationJingle = nullptr;
    scenarioFreeStateState = 0;
    ResetScenarioEventObjectMapNames();
    return activeScenarioEventRuntime;
}

void ReleaseScenarioEventRuntime(ScenarioTaskData &scenario) {
    ScenarioEventRuntime *const runtime = scenario.scenarioEventRuntime;
    if (!runtime || runtime != activeScenarioEventRuntime) {
        scenario.scenarioEventRuntime = nullptr;
        return;
    }
    StopScenarioSequence();
    ReleaseFreeTalk();
    ReleaseScenarioStepCharacter(*runtime);
    ReleaseHiddenFreeTalkCharacter(*runtime);
    scenario.scenarioEventRuntime = nullptr;

    if (scenarioFreeStateState != 0) {
        Vector3 center{};
        npcSetAreaName(talkArea, &center, 0.0f, 0);
    }
    scenarioFreeStateState = 0;
    ResetScenarioEventObjectMapNames();
    activeScenarioEventRuntime = nullptr;
    scenarioEventRuntimeStorage = {};
    FreeNpcRegistryTask();
}

AuthorController *AllocateScenarioControl(HLib::Task *task) {
    auto *const control = static_cast<AuthorController *>(HLib::AllocateTaskData(sizeof(AuthorController), SHEN_NAME(SCNC)));
    if (!control)
        return nullptr;

    std::memset(control, 0, sizeof(*control));
    control->task = task;
    control->track = -1;
    control->sequenceCount = 1;
    Author_BindController(control);
    scenarioAuthorContext.current = control;
    return control;
}

void FreeScenarioControl(ScenarioTaskData &scenario) {
    AuthorController *const control = scenario.scenarioControl;
    if (!control)
        return;
    Author_BindController(nullptr);
    if (scenarioAuthorContext.current == control) {
        scenarioAuthorContext = {};
    }
    scenario.scenarioControl = nullptr;
    HLib::FreeTaskData(control);
}

void UpdateScenarioAuthorContext(AuthorController &control) {
    const std::int8_t previousTrack = scenarioAuthorContext.track;
    scenarioAuthorContext.current = &control;
    scenarioAuthorContext.areaId = g_currentMap;
    scenarioAuthorContext.active = control.loaded == 1;
    scenarioAuthorContext.sceneId = g_actNumber;
    scenarioAuthorContext.track = control.track;
    if (previousTrack != control.track) {
        scenarioAuthorContext.trackLatch = false;
    }
    scenarioAuthorContext.currentFrame = static_cast<std::int32_t>(static_cast<float>(control.currentFrame));
    scenarioAuthorContext.framesRemaining = control.framesRemaining;
}

void ProcessScenario(AuthorController *control) {
    if (!control)
        return;
    UpdateScenarioAuthorContext(*control);
    if (control->loaded != 1)
        return;
    control->remainingFrames = 0;
    if (!control->field06c && control->skipToNext) {
        if (--control->skipToNext == 0) {
            control->flag2 = 0;
            control->needsInitAuthorMask = 1;
            control->field06c = 1;
        }
    } else if (control->sequenceCount <= 1) {
        Author_Update();
    } else {
        const std::int8_t track = control->track;
        for (std::int32_t iteration = 0; iteration < static_cast<std::int32_t>(25U * (control->sequenceCount - 1U)); ++iteration) {
            if (track != control->track)
                break;
            Author_Update();
        }
    }
    Author_UpdateDynamic();
    const std::uint16_t input = g_inputBuffer[0].pressedPriority;
    const bool skipExcluded = g_currentMap.name == SHEN_NAME(AB00) && g_actNumber == 2 && static_cast<std::uint8_t>(control->track - 67) <= 1U;
    if (Author_IsSkipActive() && !skipExcluded) {
        if (control->skipTimer <= 0.0f || control->finished) {
            if ((input & 0xfff7U) != 0)
                control->skipTimer = 90.0f;
        } else {
            EnableElement(UIElement::CutsceneSkip);
            control->skipTimer -= HLib::GetScaledDeltaTick();
            if ((input & 0x0200U) != 0 && (control->currentInput & 0x0200U) == 0) {
                Author_RequestAbort();
            }
        }
    }
    control->currentInput = input;
}

void ScenarioTaskEntry_Callback(void *taskData) {
    auto *const scenario = static_cast<ScenarioTaskData *>(taskData);
    if (ReturnZero() != 0)
        return;
    scenUpdateFreeTalkRuntime(scenario->scenarioEventRuntime);
    ProcessScenario(scenario->scenarioControl);
    EnableElement(UIElement::Subtitles);
}
} // namespace event_runtime_detail


ScenarioEventRuntime *GetScenarioEventRuntime() {
    return activeScenarioEventRuntime;
}


void SetNewScenarioTask(const std::byte *scnf) {
    activeScenarioEventRuntime->freeTalkData = scnf;
}



void ResetScenarioGlobals() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    scenarioFlagNotificationPending = 0;
    scenarioFlagNotificationShown = 0;
    activeScenarioEventRuntime = nullptr;
    scenarioPersistentFlags.initialized = false;
    GetScenarioTalkPresentationState().characterStates = {};
    talk.followRouteWait = 0;
    talk.followRequest = 0;
    talk.lookTargetActive = 0;
    talk.talkEndRequest = -1;
}

HLib::Task *ScenarioTaskEntry_Enqueue(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ScenarioTaskEntry_Callback, nextFunctionIndex, 2, sizeof(ScenarioTaskData), SHEN_NAME(NECS));
    ScenarioTaskData *const scenario = ScenarioData(task);
    if (!scenario) {
        FreeNpcRegistryTask();
        return task;
    }

    std::memset(scenario, 0, sizeof(*scenario));
    scenarioActionQueueStorage = scenario->actionQueues.data();
    scenario->name = SHEN_NAME(NECS);
    scenario->actionQueues[0].name = {};
    scenario->actionQueues[1].name = {};
    scenario->scenarioWindow = AllocateSCNW(task);
    scenario->scenarioEventRuntime = AllocateScenarioEventRuntime(task);
    scenario->scenarioControl = AllocateScenarioControl(task);
    if (!scenario->scenarioControl) {
        ReleaseScenarioEventRuntime(*scenario);
        HLib::DestroyTask(task);
        return nullptr;
    }
    constexpr WindowSpecification specification{.flags = 0,
        .reserved01 = 0,
        .columns = 46,
        .rows = 2,
        .field06 = 0,
        .field08 = 384,
        .x = 21,
        .y = 1,
        .horizontalGap = -1,
        .verticalGap = -1,
        .width = 640,
        .height = 96,
        .lineWidth = -1,
        .lineHeight = 30};
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    WindowSlot *const window = AllocateWindowSlot(control->windowTask, specification);
    scenario->windowDialog = window;
    scenario->windowTextState.columnCount = 46;
    scenario->windowTextState.surfaceRecoveryFrames = 0;
    scenario->windowTextState.surfaceReady = 1;
    scenario->windowTextState.callbackEnabled = 1;
    scenario->windowTextState.lastWindowEvent = static_cast<std::int32_t>(WindowCallbackEvent::Tick);
    scenario->windowTextState.rowCount = 2;
    scenario->windowTextState.scenarioStateFlags = 2;
    scenario->field0e4 = 0;
    AttachWindowCallback(window, SCNF_Handler, &scenario->windowTextState);
    return task;
}

void EV_SetScenarioTaskFlags(std::uint32_t flags) {
    scenarioEventRuntimeStorage.flags |= flags;
}

} // namespace shenmue::s2
