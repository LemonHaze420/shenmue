#include "s2/event_runtime.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

DateTime eventTemporaryDateTime{};
DateTime eventSavedCurrentDateTime{};
Vector3 eventPigeonTestPosition{};
std::array<char, 13> eventSavedBgmName{}, eventSavedSoundBankName{};
std::uint64_t eventSavedSoundScene{};
std::int32_t eventEscapeRoute{};
HLib::Task *eventWeatherControlTask{};
std::int16_t g_eventQteRetryFrames{-1};
namespace event_runtime_detail {


std::int32_t eventDormantWord1458{-1};
std::uint8_t eventDormantByteff77{}, eventDormantByteffef{};
std::uint16_t eventDormantWordff78{}, eventDormantWordfff0{};
std::uint64_t eventDormantQword14a0{}, eventDormantQword12e8{}, eventDormantQword12f0{}, eventDormantQword12f8{};
std::uint64_t eventDormantQword12dc{};
std::int32_t eventDormantWord12e4{}, eventDormantWord14a8{}, eventDormantWord12d8{}, eventDormantWord12d4{};
std::array<std::byte, 0x88> eventDormantBank13d0{};
std::array<std::uint64_t, 8> eventDormantBank1460{};
std::array<std::byte, 0xd0> eventDormantBank1300{};

HLib::Task *CurrentScreenFilmTask() {
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? control->screenFilmTask : nullptr;
}



ScenarioPersistentFlagBanks scenarioPersistentFlags{};
std::array<std::uint8_t, 8> &storyFlags = scenarioPersistentFlags.story;
std::array<std::uint8_t, 512> &internalFlags = scenarioPersistentFlags.internal;
std::array<std::uint8_t, 64> &stats = scenarioPersistentFlags.stats;
std::array<std::byte, 0x80> &scenarioPersistentScratch = scenarioPersistentFlags.scratch;

EventAuvwRuntime eventAuvwRuntime{};
std::int32_t butterflyRegionIndex = -1;
std::int32_t butterflySlotIndex = 0;

EventDartsState eventDartsState{};
shen_name eventDartsTemporaryCharacter{};

EventQtePresentationState eventQtePresentation{};
EventQtePresentationSink eventQtePresentationSink{};
std::uint32_t eventQteFailureCount{};
} // namespace event_runtime_detail

// @verified - p
void ResetCompiledEventState() {
    std::memset(eventVoicePrefix, 0, 8);
    g_eventQteRetryFrames = -1;
    eventDormantWord1458 = -1;
    std::memset(eventVoicePrefix + 8, 0, 24);
    eventQteFailureCount = 0;
    eventTemporaryDateTime = {};
    eventDormantByteff77 = 0;
    eventDormantWordff78 = 0;
    eventDormantQword14a0 = 0;
    eventDormantQword12e8 = eventDormantQword12f0 = eventDormantQword12f8 = 0;
    eventDormantWord12e4 = 0;
    eventDormantWord14a8 = 0;
    areas::dartsTemporaryCharacter = {};
    eventPigeonTestPosition = {};
    eventDormantQword12dc = 0;
    eventDormantWord12d8 = eventDormantWord12d4 = 0;
    eventDormantBank13d0.fill(std::byte{});
    ResetEventMovementTasks();
    eventDormantBank1460.fill(0);
    g_patiPocketModelCache = nullptr;
    eventDormantBank1300.fill(std::byte{});
    std::memset(&sapEventData, 0, sizeof(sapEventData));
    g_areaBoardCharacterIndex = 0;
    eventSavedBgmName.fill(0);
    eventSavedSoundBankName.fill(0);
    eventEscapeRoute = 0;
    eventSavedSoundScene = 0;
    std::memset(&slEventData, 0, sizeof(slEventData));
    eventSavedCurrentDateTime = {};
    eventDormantByteffef = 0;
    eventDormantWordfff0 = 0;
    ResetCompiledEventNpcLoadState();
    eventWeatherControlTask = nullptr;
}
namespace event_runtime_detail {


bool IsPackageLoaded(const char *packageId) {
    if (!packageId || !*packageId)
        return false;
    for (const LoadedMapPackage &package : GetLoadedMapPackages()) {
        if (std::strncmp(package.name.data(), packageId, package.name.size()) == 0) {
            return true;
        }
    }
    return false;
}

bool EnsureEventPackageLoaded(const char *sceneFolder, const char *packageId) {
    if (IsPackageLoaded(packageId))
        return true;
    if (!sceneFolder || !*sceneFolder || !LoadMapPackage(sceneFolder, packageId)) {
        return false;
    }
    LoadPak();
    return true;
}

void CopyEventString(char *destination, std::size_t capacity, const char *source) {
    if (!destination || capacity == 0)
        return;
    std::snprintf(destination, capacity, "%s", source ? source : "");
}

ScenarioAuthorContext scenarioAuthorContext{};
ScenarioEventRuntime scenarioEventRuntimeStorage{};
ScenarioEventRuntime *activeScenarioEventRuntime{};
std::int32_t scenarioFreeStateState{};

ScenarioFreeTalkStorage scenarioFreeTalkStorage{};
std::array<shen_name, 32> scenarioCharacterAdmissionNames{};
std::array<ScenarioFreeTalkCandidate, 64> scenarioCandidatePool{};
ScenarioFreeTalkCandidate *scenarioCandidateFreeList{};
ScenarioFreeTalkCandidate *scenarioCandidateList{};
std::int32_t scenarioCandidateMeshIndex{};
std::int32_t scenarioCandidateTaggedMesh{};
std::array<shen_name, 6> scenarioEventObjectMapNames{};

// SCEN free-talk VM state. The cue list and the conversation-camera packets are rebuilt for every line block; the choice
// tables hold the prompt the record is waiting on.

std::array<SCENDialogueCue, 101> scenarioDialogueCues{};
std::array<CameraDialogueRecord, 24> scenarioCameraRecords{};
std::array<std::uint16_t, 4> scenarioChoiceIds{};
std::uint16_t scenarioChoiceIdSpare{};
std::array<std::int8_t, 5> scenarioChoiceRanks{};
std::array<std::uint8_t, 5> scenarioChoiceSources{};
std::array<const std::byte *, 6> scenarioChoiceTargets{};
std::array<ScenarioChoiceCandidate, 100> scenarioChoiceCandidates{};
std::array<const std::byte *, 100> scenarioBranchTargets{};
std::int32_t scenarioBranchCount{};
std::int32_t scenarioBranchLoop{};
std::int32_t scenarioPromptFrames{};
std::int32_t scenarioLineHoldFrames{};
std::int32_t scenarioLineSkipped{};
float scenarioLineTimeOffset{};
std::int32_t scenarioLinePauseTicks{};
std::int32_t scenarioLineStartTicks{};
// The line-clock offset held while the pause menu or a snapshot save suspends the game.
std::uint32_t scenarioSuspendedLineTicks{};
std::int32_t scenarioVoiceWaitState{};
std::int32_t scenarioVoiceWaitArmed{};

ConversationBuffer *scenarioCurrentDialogue{};
std::int32_t scenarioHardBusy{};
// NotifyScenarioMemoFlag's pending memo jingle and the shown state TickScenarioFlagNotification raises when it plays.
std::int32_t scenarioFlagNotificationPending{};
std::int32_t scenarioFlagNotificationFrames{};
ConversationBuffer *scenarioFlagNotificationJingle{};
std::int32_t scenarioFlagNotificationShown{};

ScenarioSequenceRuntime scenarioSequenceRuntime{};
bool scenarioSequenceFightActive{};
} // namespace event_runtime_detail



// @verified - p
std::int64_t AssignPreviousFrameTimeDelta(std::int32_t paused) {
    PauseConversationCameraClock(paused != 0);
    const std::uint32_t now = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    if (paused) {
        scenarioSuspendedLineTicks = now - static_cast<std::uint32_t>(scenarioLineStartTicks);
        return scenarioSuspendedLineTicks;
    }
    scenarioLineStartTicks = static_cast<std::int32_t>(now - scenarioSuspendedLineTicks);
    return static_cast<std::uint32_t>(scenarioLineStartTicks);
}
namespace event_runtime_detail {


std::int32_t scntExpressionResult{};


void *scenarioWindowRecord{};
void **g_pSCNW{};
} // namespace event_runtime_detail


void ReleaseEventGameplayModeOwners() noexcept {
    eventQtePresentation = {};
    eventQtePresentation.currentStatus = 0xffff;
    eventQtePresentation.phase = EventQtePhase::Hidden;
    (void)ResetQuickIconState(true);
    eventDartsState = {};
    eventDartsTemporaryCharacter = {};
    PublishEventQteState();
}
namespace event_runtime_detail {



std::int32_t scenarioTalkHandPoseCommand[19][3]{
    {1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335}, {0, 0, 60075}, {0, 0, 64811},
    {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256},
    {0, 0, 0}, {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};
} // namespace event_runtime_detail

std::int32_t scenarioTalkPromptResumed{};
std::int32_t scenarioFollowPromptShown{};

void EV_CleanupMapControlTask() {
    scenCvsCtrlFree(nullptr);
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    scenDestroySubtitles(control ? control->scenarioTask : nullptr);
    snapStopControl();
    CleanupEventTaskQueue();
}

void EV_scenQueueLoad(std::int32_t act, shen_name scene, std::int32_t entry) {
    SetPlayerControlEnabled(0);
    const auto game = static_cast<std::int32_t>(SystemModule::Game);
    const auto module = static_cast<SystemModule>(((act >> 31) & game) + game);
    shenQueueChangeModuleAndScene(module, act, scene, entry);
}

// @verified - p
std::int64_t EV_LoadAudio(const char *bank, std::int64_t sceneIndex) {
    return LoadSoundProgram(bank, sceneIndex, static_cast<std::uint32_t>(g_actNumber));
}
std::int64_t EV_SetNewSceneIndex() {
    return SaveCurrentSoundProgramContext();
}
void EV_GetMapEventTask(const MapEventCallbackEntry *callbacks) {
    SetMapEventCallbackTable(callbacks);
}


void EV_SetCinemaBarVisibility(FilmControlFlag mode, std::int32_t toggle) {
    if (!toggle)
        mode = static_cast<FilmControlFlag>(static_cast<std::uint32_t>(mode) | 2U);
    auto *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    filmSetCinemaBarVisibility(static_cast<ScreenFilmState *>(HLib::GetTaskParameter(control->screenFilmTask)), mode);
}

void GakeEnableSkip_Callback(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    HLib::DestroyCurrentTask();
}

void QueueGakeEnableSkipCallback() {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(GakeEnableSkip_Callback, 4, 0x0b, SHEN_NAME(GakS));
    HLib::SetupTimedTask(task, 70);
}

} // namespace shenmue::s2
