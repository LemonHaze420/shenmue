#include "s2/scenario_sequence.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {

constexpr shen_name eventSequenceToken{SHEN_NAME(EVNT)};

template <typename Value> bool ReadSequenceValue(std::span<const std::byte> bytes, std::size_t offset, Value &value) {
    if (offset > bytes.size() || sizeof(Value) > bytes.size() - offset)
        return false;
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

bool IsSequenceArgumentCountValid(ScenarioSequenceMode mode, std::uint8_t count) {
    switch (mode) {
    case ScenarioSequenceMode::Teardown:
        return count == 0;
    case ScenarioSequenceMode::FrameWait:
        return count == 1;
    case ScenarioSequenceMode::CameraWait:
        return count == 12;
    case ScenarioSequenceMode::DialogueWait:
        return count == 7;
    case ScenarioSequenceMode::MotionWait:
    case ScenarioSequenceMode::ScenarioMotionWait:
        return count == 2 || count == 3;
    case ScenarioSequenceMode::UIResultWait:
        return count == 4;
    case ScenarioSequenceMode::ChildResultWait:
    case ScenarioSequenceMode::ChildTaskWait:
        return count == 1;
    case ScenarioSequenceMode::ChildResultBranch:
        return count == 3;
    case ScenarioSequenceMode::FadeWait:
        return count == 3;
    case ScenarioSequenceMode::FightWait:
        return count == 0;
    case ScenarioSequenceMode::AuthorWait:
        return count == 1;
    }
    return false;
}

float SequenceFloat(std::uint32_t bits) {
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return std::isfinite(value) ? value : 0.0f;
}

template <std::size_t Size> void SequenceToken(const ScenarioSequenceRecord &record, std::size_t firstArgument, std::array<char, Size> &token) {
    static_assert(Size >= 13);
    token = {};
    std::memcpy(token.data(), record.arguments.data() + firstArgument, 12);
    token[12] = '\0';
}

std::string_view SequenceTokenView(const std::array<char, 13> &token) {
    std::size_t size = 0;
    while (size < 12 && token[size] != '\0')
        ++size;
    return {token.data(), size};
}

ScenarioSequenceTaskData *SequenceTaskData() {
    return scenarioSequenceRuntime.task ? static_cast<ScenarioSequenceTaskData *>(HLib::GetTaskParameter(scenarioSequenceRuntime.task)) : nullptr;
}

const ScenarioSequenceRecord *CurrentSequenceRecord(ScenarioSequenceTaskData &task) {
    if (scenarioSequenceRuntime.recordIndex >= scenarioSequenceRuntime.records.size()) {
        return nullptr;
    }
    const ScenarioSequenceRecord &record = scenarioSequenceRuntime.records[scenarioSequenceRuntime.recordIndex];
    task.recordCursor = record.offset;
    task.mode = static_cast<std::uint16_t>(record.mode);
    task.recordFlags = record.flags;
    return &record;
}

void ResetSequenceCommandState(ScenarioSequenceTaskData &task) {
    task.dispatchState = 0;
    task.countdown = 0;
    task.waitState = 0;
    scenarioSequenceRuntime.cameraCommandStarted = false;
    scenarioSequenceRuntime.dialogueCommandStarted = false;
    scenarioSequenceRuntime.motionCommandStarted = false;
    scenarioSequenceRuntime.fadeCommandStarted = false;
}

bool AdvanceSequenceRecord(ScenarioSequenceTaskData &task) {
    task.previousMode = task.mode;
    ++scenarioSequenceRuntime.recordIndex;
    ResetSequenceCommandState(task);
    if (scenarioSequenceRuntime.recordIndex >= scenarioSequenceRuntime.records.size()) {
        return false;
    }
    task.nextRecordCursor = scenarioSequenceRuntime.records[scenarioSequenceRuntime.recordIndex].offset;
    return true;
}

bool BranchSequenceRecord(ScenarioSequenceTaskData &task, std::uint32_t targetOffset) {
    const auto found = std::find_if(scenarioSequenceRuntime.records.begin(), scenarioSequenceRuntime.records.end(),
        [targetOffset](const ScenarioSequenceRecord &record) { return record.offset == targetOffset; });
    if (found == scenarioSequenceRuntime.records.end())
        return false;
    scenarioSequenceRuntime.recordIndex = static_cast<std::size_t>(found - scenarioSequenceRuntime.records.begin());
    ResetSequenceCommandState(task);
    task.nextRecordCursor = targetOffset;
    return true;
}

void ClearSequenceChild(std::uint32_t slot) {
    ScenarioSequenceTaskData *const parent = SequenceTaskData();
    if (parent && slot < parent->children.size() && parent->children[slot] == HLib::GetCurrentTask()) {
        parent->children[slot] = nullptr;
    }
}

void FinishSequenceChild(std::uint32_t slot) {
    ClearSequenceChild(slot);
    HLib::DestroyCurrentTask();
}

void ScenarioSequenceCameraChild_Callback(void *data) {
    auto *const child = static_cast<ScenarioSequenceCameraChild *>(data);
    if (!child)
        return;
    if (!scenarioSequenceRuntime.cameraLease.active && !camCtrlAcquireSequenceLease(scenarioSequenceRuntime.cameraLease)) {
        return;
    }
    if (!camCtrlSetSequenceView(scenarioSequenceRuntime.cameraLease, child->cameraId, child->eyeAttachment, child->targetAttachment, child->eye, child->target,
            static_cast<std::int16_t>(child->roll), static_cast<std::uint16_t>(child->fieldOfView), static_cast<std::uint16_t>(child->transitionFrames),
            child->cut != 0)) {
        return;
    }
    scenarioSequenceRuntime.cameraCommandStarted = true;
    FinishSequenceChild(child->header.slot);
}

void ScenarioSequenceDialogueChild_Callback(void *data) {
    auto *const child = static_cast<ScenarioSequenceDialogueChild *>(data);
    if (!child)
        return;
    scenarioSequenceRuntime.voiceToken = child->voice;
    const std::string_view voice = SequenceTokenView(scenarioSequenceRuntime.voiceToken);
    ScenarioSequenceTaskData *const parent = SequenceTaskData();
    const shen_name speaker = parent && parent->scenario ? parent->scenario->activeCharacter : shen_name{};
    scenarioSequenceRuntime.voiceRequested = !voice.empty();
    scenarioSequenceRuntime.voiceBuffer = voice.empty() ? nullptr : shenPlayAudio(scenarioSequenceRuntime.voiceToken.data(), speaker, 4, false);

    const std::string_view subtitle = SequenceTokenView(child->subtitle);
    const std::string_view text = GetLocalizedSceneText(subtitle);
    scenarioSequenceRuntime.subtitleOwned = !subtitle.empty() && !text.empty() && (child->durationFrames != 0 || !voice.empty()) &&
        BeginSubtitlePresentation(subtitle, text, g_sceneTextLanguage, child->durationFrames);
    scenarioSequenceRuntime.dialogueCommandStarted = true;
    FinishSequenceChild(child->header.slot);
}

void ScenarioSequenceMotionChild_Callback(void *data) {
    auto *const child = static_cast<ScenarioSequenceMotionChild *>(data);
    if (!child)
        return;
    ScenarioSequenceTaskData *const parent = SequenceTaskData();
    Character *const character =
        parent && parent->scenario ? FindScenarioCharacter(parent->scenario->task, child->character) : FindCharacterByScenarioName(child->character);
    MotionWork *const motionWork = FindMotionWork(character);
    if (!motionWork)
        return;
    if (child->motion != 0xffffU && !mtwkRequestMotion(motionWork, static_cast<std::uint16_t>(child->motion), static_cast<MotionRegion>(child->region))) {
        return;
    }
    scenarioSequenceRuntime.motionCharacter = child->character;
    scenarioSequenceRuntime.motionRegion = child->region;
    scenarioSequenceRuntime.motionCommandStarted = true;
    FinishSequenceChild(child->header.slot);
}

void ScenarioSequenceUIChild_Callback(void *data) {
    auto *const child = static_cast<ScenarioSequenceUIChild *>(data);
    if (!child)
        return;
    BeginDialogueSelectionPrompt(child->flagIds, child->timeoutFrames, child->defaultFlagId);
    scenarioSequenceRuntime.uiOwned = true;
    FinishSequenceChild(child->header.slot);
}

void ScenarioSequenceFadeChild_Callback(void *data) {
    auto *const child = static_cast<ScenarioSequenceFadeChild *>(data);
    if (!child)
        return;
    StartScreenFilmFade(
        CurrentScreenFilmTask(), child->durationFrames, static_cast<std::uint8_t>(child->startAlpha), static_cast<std::uint8_t>(child->targetAlpha));
    scenarioSequenceRuntime.fadeCommandStarted = true;
    FinishSequenceChild(child->header.slot);
}

template <typename Payload>
Payload *EnqueueSequenceChild(ScenarioSequenceTaskData &parent, HLib::TaskCallback callback, ScenarioSequenceMode mode, std::uint32_t slot) {
    if (slot >= parent.children.size() || parent.children[slot])
        return nullptr;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callback, 6, 11, sizeof(Payload), eventSequenceToken);
    if (!task)
        return nullptr;
    auto *const payload = static_cast<Payload *>(HLib::GetTaskParameter(task));
    if (!payload) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    std::memset(payload, 0, sizeof(*payload));
    payload->header.mode = static_cast<std::uint32_t>(mode);
    payload->header.slot = slot;
    parent.children[slot] = task;
    return payload;
}

bool DispatchCameraRecord(ScenarioSequenceTaskData &task, const ScenarioSequenceRecord &record) {
    auto *const payload = EnqueueSequenceChild<ScenarioSequenceCameraChild>(task, ScenarioSequenceCameraChild_Callback, record.mode, 0);
    if (!payload)
        return false;
    payload->cameraId = static_cast<std::int32_t>(record.arguments[0]);
    payload->eyeAttachment = shen_name{record.arguments[1]};
    payload->targetAttachment = shen_name{record.arguments[2]};
    payload->eye = {SequenceFloat(record.arguments[3]), SequenceFloat(record.arguments[4]), SequenceFloat(record.arguments[5])};
    payload->target = {SequenceFloat(record.arguments[6]), SequenceFloat(record.arguments[7]), SequenceFloat(record.arguments[8])};
    payload->roll = static_cast<std::int32_t>(record.arguments[9]);
    payload->fieldOfView = record.arguments[10];
    payload->transitionFrames = record.arguments[11];
    payload->cut = record.flags & 1U;
    return true;
}

bool DispatchDialogueRecord(ScenarioSequenceTaskData &task, const ScenarioSequenceRecord &record) {
    auto *const payload = EnqueueSequenceChild<ScenarioSequenceDialogueChild>(task, ScenarioSequenceDialogueChild_Callback, record.mode, 0);
    if (!payload)
        return false;
    SequenceToken(record, 0, payload->voice);
    SequenceToken(record, 3, payload->subtitle);
    payload->durationFrames = record.arguments[6];
    return true;
}

bool DispatchMotionRecord(ScenarioSequenceTaskData &task, const ScenarioSequenceRecord &record) {
    auto *const payload = EnqueueSequenceChild<ScenarioSequenceMotionChild>(task, ScenarioSequenceMotionChild_Callback, record.mode, 0);
    if (!payload)
        return false;
    payload->character = shen_name{record.arguments[0]};
    payload->motion = record.argumentCount == 3 ? record.arguments[1] : 0xffffU;
    payload->region = record.argumentCount == 3 ? record.arguments[2] : record.arguments[1];
    return true;
}

bool DispatchUIRecord(ScenarioSequenceTaskData &task, const ScenarioSequenceRecord &record) {
    auto *const payload = EnqueueSequenceChild<ScenarioSequenceUIChild>(task, ScenarioSequenceUIChild_Callback, record.mode, 0);
    if (!payload)
        return false;
    payload->flagIds = {static_cast<std::uint16_t>(record.arguments[0]), static_cast<std::uint16_t>(record.arguments[0] >> 16U),
        static_cast<std::uint16_t>(record.arguments[1]), static_cast<std::uint16_t>(record.arguments[1] >> 16U)};
    payload->timeoutFrames = static_cast<std::uint16_t>(record.arguments[2]);
    payload->defaultFlagId = static_cast<std::uint16_t>(record.arguments[2] >> 16U);
    payload->resultSlot = record.arguments[3];
    return true;
}

bool DispatchFadeRecord(ScenarioSequenceTaskData &task, const ScenarioSequenceRecord &record) {
    auto *const payload = EnqueueSequenceChild<ScenarioSequenceFadeChild>(task, ScenarioSequenceFadeChild_Callback, record.mode, 0);
    if (!payload)
        return false;
    payload->durationFrames = record.arguments[0];
    payload->startAlpha = record.arguments[1];
    payload->targetAlpha = record.arguments[2];
    return true;
}

void ReleaseSequenceResources() {
    ScenarioSequenceTaskData *const data = SequenceTaskData();
    if (data) {
        for (HLib::Task *&child : data->children) {
            if (child)
                HLib::DestroyTask(child);
            child = nullptr;
        }
    }
    if (scenarioSequenceRuntime.voiceBuffer) {
        scenCvsCtrlFree(scenarioSequenceRuntime.voiceBuffer);
    }
    scenarioSequenceRuntime.voiceBuffer = nullptr;
    scenarioSequenceRuntime.voiceRequested = false;
    scenarioSequenceRuntime.voiceToken = {};
    if (scenarioSequenceRuntime.subtitleOwned)
        EndSubtitlePresentation();
    scenarioSequenceRuntime.subtitleOwned = false;
    if (scenarioSequenceRuntime.uiOwned)
        EndDialogueSelectionPrompt();
    scenarioSequenceRuntime.uiOwned = false;
    camCtrlReleaseSequenceLease(scenarioSequenceRuntime.cameraLease);
}

void CompleteSequenceTask(ScenarioSequenceTaskData &task, ScenarioSequenceStatus status) {
    task.completed = 1;
    task.finalResult = scenarioSequenceRuntime.result;
    if (task.scenario) {
        task.scenario->field11c = scenarioSequenceRuntime.result;
    }
    ReleaseSequenceResources();
    scenarioSequenceRuntime.status = status;
    scenarioSequenceRuntime.task = nullptr;
    HLib::DestroyCurrentTask();
}

void ScenarioSequenceTask_Callback(void *data) {
    auto *const task = static_cast<ScenarioSequenceTaskData *>(data);
    if (!task || task->task != HLib::GetCurrentTask())
        return;




    for (std::size_t iteration = 0; iteration < 64; ++iteration) {
        const ScenarioSequenceRecord *const record = CurrentSequenceRecord(*task);
        if (!record) {
            CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
            return;
        }
        switch (record->mode) {
        case ScenarioSequenceMode::Teardown:
            CompleteSequenceTask(*task, ScenarioSequenceStatus::Complete);
            return;

        case ScenarioSequenceMode::FrameWait:
            if (!task->dispatchState) {
                task->countdown = record->arguments[0];
                task->dispatchState = 1;
                return;
            }
            if (task->countdown && --task->countdown)
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;

        case ScenarioSequenceMode::CameraWait:
            if (!task->dispatchState) {
                if (!DispatchCameraRecord(*task, *record))
                    return;
                task->dispatchState = 1;
                return;
            }
            if (task->children[0] || !scenarioSequenceRuntime.cameraCommandStarted) {
                return;
            }
            if (camCtrlTickSequenceView(scenarioSequenceRuntime.cameraLease)) {
                return;
            }
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;

        case ScenarioSequenceMode::DialogueWait: {
            if (!task->dispatchState) {
                if (!DispatchDialogueRecord(*task, *record))
                    return;
                task->dispatchState = 1;
                return;
            }
            if (task->children[0] || !scenarioSequenceRuntime.dialogueCommandStarted) {
                return;
            }
            if (!scenarioSequenceRuntime.voiceRequested && scenarioSequenceRuntime.subtitleOwned && !TickSubtitlePresentation()) {
                scenarioSequenceRuntime.subtitleOwned = false;
            }
            if (scenarioSequenceRuntime.voiceRequested) {
                ConversationBuffer *const buffer = scenarioSequenceRuntime.voiceBuffer;
                if (!buffer) {
                    CompleteSequenceTask(*task, ScenarioSequenceStatus::VoiceFailed);
                    return;
                }
                if (cvsHasValidConverseData(buffer))
                    return;
                scenarioSequenceRuntime.voiceBuffer = nullptr;
                scenarioSequenceRuntime.voiceRequested = false;
                if (scenarioSequenceRuntime.subtitleOwned) {
                    EndSubtitlePresentation();
                    scenarioSequenceRuntime.subtitleOwned = false;
                }
            }
            if (scenarioSequenceRuntime.subtitleOwned)
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::MotionWait:
        case ScenarioSequenceMode::ScenarioMotionWait: {
            if (!task->dispatchState) {
                if (!DispatchMotionRecord(*task, *record))
                    return;
                task->dispatchState = 1;
                return;
            }
            if (task->children[0] || !scenarioSequenceRuntime.motionCommandStarted) {
                return;
            }
            Character *const character = task->scenario ? FindScenarioCharacter(task->scenario->task, scenarioSequenceRuntime.motionCharacter) :
                                                          FindCharacterByScenarioName(scenarioSequenceRuntime.motionCharacter);
            MotionWork *const motionWork = FindMotionWork(character);
            if (!motionWork || !SetMotionRegionFlag(motionWork, 65, 1, static_cast<MotionRegion>(scenarioSequenceRuntime.motionRegion))) {
                return;
            }
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::UIResultWait: {
            if (!task->dispatchState) {
                if (!DispatchUIRecord(*task, *record))
                    return;
                task->resultIndex = record->arguments[3];
                task->dispatchState = 1;
                return;
            }
            if (task->children[0])
                return;
            std::uint16_t flagId = 0xffff;
            const DialogueSelectionResult selection = PollDialogueSelectionPrompt(flagId);
            if (selection == DialogueSelectionResult::Pending)
                return;
            EndDialogueSelectionPrompt();
            scenarioSequenceRuntime.uiOwned = false;
            const std::uint32_t value = flagId == 0xffff ? 0U : flagId;
            const std::size_t slot = task->resultIndex;
            if (slot >= task->results.size()) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            task->results[slot] = value ? value : 1U;
            task->branchValues[slot] = value;
            task->uiResult = value;
            scenarioSequenceRuntime.result = value;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::ChildResultWait: {
            const std::size_t slot = record->arguments[0];
            if (slot >= task->results.size()) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            if (!task->results[slot])
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::ChildResultBranch: {
            const std::size_t slot = record->arguments[0];
            if (slot >= task->results.size()) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            if (!task->results[slot])
                return;
            const bool takeBranch = task->results[slot] == record->arguments[1] || task->branchValues[slot] == record->arguments[1];
            if (takeBranch) {
                if (!BranchSequenceRecord(*task, record->arguments[2])) {
                    CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                    return;
                }
            } else if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::ChildTaskWait: {
            const std::size_t slot = record->arguments[0];
            if (slot >= task->children.size()) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            if (task->children[slot])
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }

        case ScenarioSequenceMode::FadeWait:
            if (!task->dispatchState) {
                if (!DispatchFadeRecord(*task, *record))
                    return;
                task->dispatchState = 1;
                return;
            }
            if (task->children[0] || !scenarioSequenceRuntime.fadeCommandStarted || DoesScreenFilmColourExist(CurrentScreenFilmTask())) {
                return;
            }
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;

        case ScenarioSequenceMode::FightWait:
            if (scenarioSequenceFightActive)
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;

        case ScenarioSequenceMode::AuthorWait:
            if (!task->dispatchState) {
                task->countdown = record->arguments[0];
                task->dispatchState = 1;
                if (task->countdown)
                    return;
            }
            if (task->countdown) {
                --task->countdown;
                return;
            }
            if (authGetLoaded())
                return;
            if (!AdvanceSequenceRecord(*task)) {
                CompleteSequenceTask(*task, ScenarioSequenceStatus::InvalidRecords);
                return;
            }
            continue;
        }
    }
}
} // namespace event_runtime_detail


bool RetainScenarioSequenceRecords(std::span<const std::byte> records) {
    StopScenarioSequence();
    scenarioSequenceRuntime.retainedBytes.clear();
    scenarioSequenceRuntime.records.clear();
    scenarioSequenceRuntime.unsupportedModes.clear();
    scenarioSequenceRuntime.recordIndex = 0;
    scenarioSequenceRuntime.result = 0;

    std::vector<ScenarioSequenceRecord> parsed;
    bool terminated = false;
    for (std::size_t cursor = 0; cursor + 4 <= records.size();) {
        std::uint32_t header{};
        if (!ReadSequenceValue(records, cursor, header))
            break;
        if (header == 0) {
            if (cursor + 4 != records.size())
                break;
            ScenarioSequenceRecord teardown{};
            teardown.offset = static_cast<std::uint32_t>(cursor);
            teardown.mode = ScenarioSequenceMode::Teardown;
            parsed.push_back(teardown);
            terminated = true;
            break;
        }
        const std::uint8_t rawMode = static_cast<std::uint8_t>(header);
        const std::uint8_t argumentCount = static_cast<std::uint8_t>(header >> 8U);
        const std::size_t recordSize = 4U + static_cast<std::size_t>(argumentCount) * 4U;
        if (recordSize > records.size() - cursor)
            break;
        if (rawMode < 1 || rawMode > 13) {
            if (std::find(scenarioSequenceRuntime.unsupportedModes.begin(), scenarioSequenceRuntime.unsupportedModes.end(), rawMode) ==
                scenarioSequenceRuntime.unsupportedModes.end()) {
                scenarioSequenceRuntime.unsupportedModes.push_back(rawMode);
            }
            cursor += recordSize;
            continue;
        }
        const auto mode = static_cast<ScenarioSequenceMode>(rawMode);
        if (!IsSequenceArgumentCountValid(mode, argumentCount))
            break;
        ScenarioSequenceRecord record{};
        record.offset = static_cast<std::uint32_t>(cursor);
        record.mode = mode;
        record.flags = static_cast<std::uint16_t>(header >> 16U);
        record.argumentCount = argumentCount;
        bool valid = true;
        for (std::size_t argument = 0; argument < argumentCount; ++argument) {
            valid &= ReadSequenceValue(records, cursor + 4U + argument * 4U, record.arguments[argument]);
        }
        if (!valid)
            break;
        parsed.push_back(record);
        cursor += recordSize;
    }

    if (!terminated || !scenarioSequenceRuntime.unsupportedModes.empty() || parsed.empty()) {
        scenarioSequenceRuntime.status = ScenarioSequenceStatus::InvalidRecords;
        return false;
    }
    for (const ScenarioSequenceRecord &record : parsed) {
        if (record.mode != ScenarioSequenceMode::ChildResultBranch)
            continue;
        const std::uint32_t target = record.arguments[2];
        if (std::none_of(parsed.begin(), parsed.end(), [target](const ScenarioSequenceRecord &candidate) { return candidate.offset == target; })) {
            scenarioSequenceRuntime.status = ScenarioSequenceStatus::InvalidRecords;
            return false;
        }
    }
    scenarioSequenceRuntime.retainedBytes.assign(records.begin(), records.end());
    scenarioSequenceRuntime.records = std::move(parsed);
    scenarioSequenceRuntime.status = ScenarioSequenceStatus::Ready;
    return true;
}

HLib::Task *StartRetainedScenarioSequence(shen_name sequenceName) {
    if (scenarioSequenceRuntime.records.empty() || scenarioSequenceRuntime.status == ScenarioSequenceStatus::InvalidRecords) {
        return nullptr;
    }
    StopScenarioSequence();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ScenarioSequenceTask_Callback, 4, 11, sizeof(ScenarioSequenceTaskData), eventSequenceToken);
    auto *const data = task ? static_cast<ScenarioSequenceTaskData *>(HLib::GetTaskParameter(task)) : nullptr;
    if (!data) {
        if (task)
            HLib::DestroyTask(task);
        scenarioSequenceRuntime.status = ScenarioSequenceStatus::Ready;
        return nullptr;
    }
    std::memset(data, 0, sizeof(*data));
    data->sequenceName = sequenceName;
    data->task = task;
    data->scenario = activeScenarioEventRuntime;
    data->order = 1;
    data->recordCursor = scenarioSequenceRuntime.records.front().offset;
    data->nextRecordCursor = data->recordCursor;
    scenarioSequenceRuntime.task = task;
    scenarioSequenceRuntime.recordIndex = 0;
    scenarioSequenceRuntime.result = 0;
    scenarioSequenceRuntime.status = ScenarioSequenceStatus::Running;
    return task;
}

void StopScenarioSequence() {
    HLib::Task *const task = scenarioSequenceRuntime.task;
    ReleaseSequenceResources();
    scenarioSequenceRuntime.task = nullptr;
    if (task)
        HLib::DestroyTask(task);
    scenarioSequenceRuntime.status = scenarioSequenceRuntime.records.empty() ? ScenarioSequenceStatus::Empty : ScenarioSequenceStatus::Ready;
}

ScenarioSequenceStatus GetScenarioSequenceStatus() {
    return scenarioSequenceRuntime.status;
}

std::uint32_t GetScenarioSequenceResult() {
    return scenarioSequenceRuntime.result;
}

std::span<const std::uint8_t> GetScenarioSequenceUnsupportedModes() {
    return scenarioSequenceRuntime.unsupportedModes;
}

void SetScenarioSequenceFightActive(bool active) {
    scenarioSequenceFightActive = active;
}

} // namespace shenmue::s2
