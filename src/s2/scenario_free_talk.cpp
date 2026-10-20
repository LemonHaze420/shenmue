#include "s2/scenario_free_talk.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {

constexpr shen_name scenarioTalkAllocation{SHEN_NAME(SCTT)};
constexpr shen_name scnfName{SHEN_NAME(SCNF)};
constexpr shen_name extraFreeTalkName{SHEN_NAME(EXTR)};

template <typename Value> bool ReadFreeTalkValue(const std::byte *data, std::size_t size, std::size_t offset, Value &value) {
    if (!data || offset > size || sizeof(Value) > size - offset)
        return false;
    std::memcpy(&value, data + offset, sizeof(value));
    return true;
}

bool OffsetFits(std::uint32_t offset, std::size_t size) {
    return offset < size;
}

bool TableFits(std::uint32_t offset, std::uint8_t count, std::size_t stride, std::size_t size) {
    if (count == 0)
        return true;
    if (offset == 0 || offset >= size)
        return false;
    return static_cast<std::size_t>(count) <= (size - static_cast<std::size_t>(offset)) / stride;
}

const char *BoundedString(const std::byte *record, std::size_t size, std::uint32_t offset) {
    if (!OffsetFits(offset, size))
        return nullptr;
    const char *const value = reinterpret_cast<const char *>(record + offset);
    for (std::size_t index = offset; index < size; ++index) {
        if (record[index] == std::byte{})
            return value;
    }
    return nullptr;
}
} // namespace event_runtime_detail

ScenarioFreeTalkRecord *FindMutableScenarioFreeTalkRecord(shen_name character) {
    if (!character.name)
        return nullptr;
    const bool stateIndex = (character.name & 0xffff0000U) == 0;
    if (!stateIndex) {
        for (std::size_t index = 0; index < scenarioFreeTalkStorage.recordCount; ++index) {
            ScenarioFreeTalkRecord &record = scenarioFreeTalkStorage.records[index];
            if (record.name.name == character.name)
                return &record;
        }
    }
    for (ScenarioFreeTalkCandidate *candidate = scenarioCandidateList; candidate; candidate = candidate->previous) {


        if (stateIndex ? static_cast<std::int16_t>(candidate->characterStateIndex) == static_cast<std::int32_t>(character.name) :
                         candidate->name.name == character.name)
            return candidate->availability > 0 ? candidate->record : nullptr;
    }
    return nullptr;
}
namespace event_runtime_detail {


std::uint16_t FreeTalkCharacterStateIndex(const char *path, std::size_t pathSize) {
    if (!path || pathSize < 6)
        return 0;
    const char *slash = nullptr;
    for (std::size_t index = 0; index < pathSize; ++index) {
        if (path[index] == '/')
            slash = path + index;
    }
    if (!slash || static_cast<std::size_t>(path + pathSize - slash) < 6 || slash[2] != '2') {
        return 0;
    }
    if (slash[1] != 'F' && slash[1] != 'H' && slash[1] != 'K' && slash[1] != 'Q') {
        return 0;
    }
    if (slash[3] < '0' || slash[3] > '9' || slash[4] < '0' || slash[4] > '9' || slash[5] < '0' || slash[5] > '9') {
        return 0;
    }
    const std::uint16_t index = static_cast<std::uint16_t>((slash[3] - '0') * 100 + (slash[4] - '0') * 10 + (slash[5] - '0'));
    return index < GetScenarioTalkPresentationState().characterStates.size() ? index : 0;
}

bool ExpandFreeTalkRecord(const std::byte *source, std::size_t sourceSize, ScenarioFreeTalkRecord &record) {
    ScenarioFreeTalkSourceHeader header{};
    if (!ReadFreeTalkValue(source, sourceSize, 0, header) || header.size < sizeof(header) || header.size > sourceSize || header.participantCount > 32 ||
        header.locationCount > 64 || header.keyValueCount > 64 || !OffsetFits(header.voicePathOffset, header.size) ||
        !OffsetFits(header.scriptOffset, header.size) || !OffsetFits(header.lineTableOffset, header.size) ||
        !OffsetFits(header.participantOffset, header.size) || !OffsetFits(header.vectorDataOffset, header.size) ||
        !OffsetFits(header.locationOffset, header.size) || !OffsetFits(header.keyValueOffset, header.size) ||
        !TableFits(header.participantOffset, header.participantCount, 4, header.size) ||
        !TableFits(header.locationOffset, header.locationCount, 12, header.size) || !TableFits(header.keyValueOffset, header.keyValueCount, 8, header.size)) {
        return false;
    }

    const char *const voicePath = BoundedString(source, header.size, header.voicePathOffset);
    if (!voicePath)
        return false;
    const std::size_t voicePathSize = static_cast<std::size_t>(reinterpret_cast<const std::byte *>(voicePath) - source);
    std::size_t terminatedPathSize = 0;
    while (voicePathSize + terminatedPathSize < header.size && voicePath[terminatedPathSize] != '\0') {
        ++terminatedPathSize;
    }



    record.name = header.name;
    record.locationCount = header.locationCount;
    record.approachPointCount = header.reserved09;
    record.participantCount = header.participantCount;
    record.keyValueCount = header.keyValueCount;
    record.selectionIndex = -1;
    record.field007 = 0;
    record.characterStateIndex = FreeTalkCharacterStateIndex(voicePath, terminatedPathSize);
    record.participants = source + header.participantOffset;
    record.locations = source + header.locationOffset;
    record.vectorData = source + header.vectorDataOffset;
    record.voicePath = voicePath;
    record.keyValues = source + header.keyValueOffset;
    record.script = source + header.scriptOffset;
    record.lineTable = source + header.lineTableOffset;
    record.sourceRecord = source;

    record.localControl.flags |= 4U;
    record.control = static_cast<std::int16_t>(record.characterStateIndex) > 0 ?
        &GetScenarioTalkPresentationState().characterStates[record.characterStateIndex] :
        &record.localControl;
    record.executionState = static_cast<std::uint8_t>(((record.control->flags >> 3) & 2U) | 1U);
    return true;
}

bool InstallFreeTalkRecord(const std::byte *source, std::size_t sourceSize) {
    if (scenarioFreeTalkStorage.recordCount == scenarioFreeTalkStorage.records.size())
        return false;
    ScenarioFreeTalkRecord &record = scenarioFreeTalkStorage.records[scenarioFreeTalkStorage.recordCount];
    if (!ExpandFreeTalkRecord(source, sourceSize, record))
        return false;
    // ReadFreeTalk overwrites the shared control binding and execution byte.
    record.control = &record.localControl;
    record.executionState = 0;
    ++scenarioFreeTalkStorage.recordCount;
    return true;
}

void InstallFreeTalkContainer(const std::byte *data, std::size_t size) {
    ScenarioFreeTalkContainerHeader container{};
    if (!ReadFreeTalkValue(data, size, 0, container) || container.name.name != scnfName.name || container.wholeSize < sizeof(container) ||
        container.wholeSize > size) {
        return;
    }

    std::size_t offset = sizeof(container);
    while (offset < container.wholeSize && scenarioFreeTalkStorage.recordCount < scenarioFreeTalkStorage.records.size()) {
        ScenarioFreeTalkSourceHeader header{};
        if (!ReadFreeTalkValue(data, container.wholeSize, offset, header) || header.name.name == 0 || header.size < sizeof(header) ||
            header.size > container.wholeSize - offset) {
            return;
        }
        if (!InstallFreeTalkRecord(data + offset, header.size))
            return;
        offset += header.size;
    }
}

void SelectExtraFreeTalkRecord(const std::byte *data, std::size_t size) {
    ScenarioFreeTalkContainerHeader container{};
    if (!ReadFreeTalkValue(data, size, 0, container) || container.name.name != scnfName.name || container.wholeSize < sizeof(container) ||
        container.wholeSize > size) {
        return;
    }

    std::size_t offset = sizeof(container);
    while (offset < container.wholeSize) {
        ScenarioFreeTalkSourceHeader header{};
        if (!ReadFreeTalkValue(data, container.wholeSize, offset, header) || header.name.name == 0 || header.size < sizeof(header) ||
            header.size > container.wholeSize - offset) {
            return;
        }
        if (header.name.name == extraFreeTalkName.name) {
            scenarioFreeTalkStorage.extraRecord = data + offset;
            scenarioFreeTalkStorage.extraRecordSize = header.size;
        }
        offset += header.size;
    }
}

void LoadExternalFreeTalkData() {
    std::array<char, 32> path{};
    const char actCharacter = static_cast<char>('0' + g_actNumber);
    int length = std::snprintf(path.data(), path.size(), "/misc/freetlk%c.bin", actCharacter);
    if (length <= 0 || static_cast<std::size_t>(length) >= path.size())
        return;

    std::uint64_t size = ArchiveData_GetFileSize(path.data());
    if (size == 0) {
        length = std::snprintf(path.data(), path.size(), "/misc/freetalk.bin");
        if (length <= 0 || static_cast<std::size_t>(length) >= path.size()) {
            return;
        }
        size = ArchiveData_GetFileSize(path.data());
    }
    if (size == 0 || size > std::numeric_limits<std::size_t>::max())
        return;

    auto *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, scenarioTalkAllocation));
    if (!data || !ArchiveData_Read(path.data(), 0, data, size)) {
        if (data)
            HLib::FreeTaskData(data);
        return;
    }

    scenarioFreeTalkStorage.externalData = data;
    scenarioFreeTalkStorage.externalSize = static_cast<std::size_t>(size);
    SelectExtraFreeTalkRecord(data, scenarioFreeTalkStorage.externalSize);
}

// @verified - p
void UpdateScenarioFreeTalkCharacter(Character *character, void *) {
    SetFlagForToken(character, SCENARIO_FREE_TALK_COMPONENT, 0);
}

void FreeScenarioFreeTalkCharacter(Character *character, void *payload) {
    auto *const record = static_cast<ScenarioFreeTalkRecord *>(payload);
    ScenarioFreeTalkCandidate *const candidate = record ? record->candidate : nullptr;
    if (!candidate || candidate->character != character || candidate->record != record)
        return;
    if (candidate->next)
        candidate->next->previous = candidate->previous;
    else
        scenarioCandidateList = candidate->previous;
    if (candidate->previous)
        candidate->previous->next = candidate->next;
    *candidate = {};
    candidate->previous = scenarioCandidateFreeList;
    scenarioCandidateFreeList = candidate;
}

std::span<const std::byte> FindFreeTalkCharacterSource(std::span<const std::byte> container, shen_name name, bool matchName) {
    ScenarioFreeTalkContainerHeader header{};
    if (!ReadFreeTalkValue(container.data(), container.size(), 0, header) || header.name.name != scnfName.name || header.wholeSize > container.size() ||
        header.wholeSize < sizeof(header))
        return {};
    std::size_t cursor = sizeof(header);
    while (cursor < header.wholeSize) {
        ScenarioFreeTalkSourceHeader record{};
        if (!ReadFreeTalkValue(container.data(), header.wholeSize, cursor, record) || record.size < sizeof(record) || record.size > header.wholeSize - cursor)
            return {};
        if (!matchName)
            return container.subspan(cursor, record.size);
        if (!record.name.name)
            return {};
        if (record.name.name == name.name)
            return container.subspan(cursor, record.size);
        cursor += record.size;
    }
    return {};
}

constexpr std::uint32_t StepModelBit(ScenarioStepModelAsset asset) {
    return 1U << static_cast<std::uint8_t>(asset);
}

bool SelectScenarioStepHighModel(std::int32_t step, ScenarioStepHighModelState &state) {
    if (step >= 370000)
        return false;

    state = {};
    state.modelLoadMask = StepModelBit(ScenarioStepModelAsset::TMEMH00G);
    if (step < 50000) {
        state.modelLoadMask |= StepModelBit(ScenarioStepModelAsset::LMALH00G);
        state.nativeMode = 0;
        state.selectedAsset = ScenarioStepModelAsset::LMALH00G;
    } else if (step < 110000) {
        state.nativeMode = 1;
        state.selectedAsset = ScenarioStepModelAsset::TMEMH00G;
    } else if (step < 120000) {
        state.nativeMode = 2;
        state.selectedAsset = ScenarioStepModelAsset::ABR0HMPG;
    } else if (step < 350000) {
        state.nativeMode = 1;
        state.selectedAsset = ScenarioStepModelAsset::TMEMH00G;
    } else if (step < 360000) {


        switch (GetScenarioTalkPresentationState().selectedFollowFlag) {
        case 347:
            state.nativeMode = 3;
            state.selectedAsset = ScenarioStepModelAsset::PSF72AWG;
            break;
        case 348:
            state.nativeMode = 4;
            state.selectedAsset = ScenarioStepModelAsset::PSF6200G;
            break;
        case 349:
            state.nativeMode = 5;
            state.selectedAsset = ScenarioStepModelAsset::PSF3200G;
            break;
        default:
            state.nativeMode = 1;
            state.selectedAsset = ScenarioStepModelAsset::TMEMH00G;
            break;
        }
    } else {
        state.nativeMode = 6;
        state.selectedAsset = ScenarioStepModelAsset::HKSMH01G;
    }

    if (step < 120000) {
        state.modelLoadMask |= StepModelBit(ScenarioStepModelAsset::ABR0HMPG);
    } else if (step < 360000) {
        state.modelLoadMask |=
            StepModelBit(ScenarioStepModelAsset::PSF72AWG) | StepModelBit(ScenarioStepModelAsset::PSF6200G) | StepModelBit(ScenarioStepModelAsset::PSF3200G);
    }
    if (step >= 120000) {
        state.modelLoadMask |= StepModelBit(ScenarioStepModelAsset::HKSMH01G);
    }
    return true;
}

void CreateCharactersDependingOnCurrentStep(ScenarioEventRuntime &scenario) {
    ScenarioStepCharacterState &owner = scenarioFreeTalkStorage.stepCharacter;
    if (owner.character)
        return;

    ScenarioStepHighModelState pending{};
    const std::int32_t step = static_cast<std::int32_t>(GetCurrentStep());
    if (!SelectScenarioStepHighModel(step, pending))
        return;

    HLib::Task *handlerTask = nullptr;
    Character *const character = AddHighModelCharacter(stepVisibleCharacterName, handlerTask);
    if (!character)
        return;
    if (!AssignScenarioCharacter(scenario.task, stepVisibleCharacterName, character)) {
        FreeCharacter(character);
        return;
    }
    if (!ConfigureScenarioStepHighModelCharacter(character, pending)) {
        UnassignScenarioCharacter(scenario.task, character);
        FreeCharacter(character);
        return;
    }

    owner.id = stepVisibleCharacterName;
    owner.record = FindScenarioFreeTalkRecord(owner.id);
    owner.character = character;
    owner.handlerTask = handlerTask;
    ScenarioStepHighModelState configured{};
    GetScenarioStepHighModelState(character, configured);
}

void ReleaseScenarioStepCharacter(ScenarioEventRuntime &scenario) {
    ScenarioStepCharacterState &owner = scenarioFreeTalkStorage.stepCharacter;
    Character *const character = owner.character;
    owner = {};
    if (!character)
        return;

    UnassignScenarioCharacter(scenario.task, character);
    FreeCharacter(character);
}

void CreateHiddenFreeTalkCharacter(ScenarioEventRuntime &scenario) {
    if (scenario.hiddenFreeTalkCharacter)
        return;

    HLib::Task *handlerTask = nullptr;
    Character *const character = AddHighModelCharacter(hiddenFreeTalkCharacterName, handlerTask);
    scenario.hiddenFreeTalkCharacter = character;
    if (!character)
        return;

    if (!AssignScenarioCharacter(scenario.task, hiddenFreeTalkCharacterName, character)) {
        scenario.hiddenFreeTalkCharacter = nullptr;
        FreeCharacter(character);
        return;
    }

    character->flags &= ~CF_Active;
    HLib::SetupTimedTask(handlerTask, 0);
}

void ReleaseHiddenFreeTalkCharacter(ScenarioEventRuntime &scenario) {
    Character *const character = scenario.hiddenFreeTalkCharacter;
    scenario.hiddenFreeTalkCharacter = nullptr;
    if (!character)
        return;

    UnassignScenarioCharacter(scenario.task, character);
    FreeCharacter(character);
}

void ReadFreeTalk(ScenarioEventRuntime &scenario) {
    scenarioFreeTalkStorage.records = {};
    scenarioFreeTalkStorage.recordCount = 0;
    scenarioFreeTalkStorage.extraRecord = nullptr;
    scenarioFreeTalkStorage.extraRecordSize = 0;

    const std::uint64_t systemTalkSize = ArchiveData_GetFileSize("/misc/systalk.bin");
    if (g_systemTalkData && systemTalkSize > 0 && systemTalkSize <= std::numeric_limits<std::size_t>::max()) {
        InstallFreeTalkContainer(g_systemTalkData, static_cast<std::size_t>(systemTalkSize));
    }

    LoadExternalFreeTalkData();

    if (scenario.freeTalkData && scenarioFreeTalkStorage.recordCount < scenarioFreeTalkStorage.records.size()) {
        const std::byte *const eventData = scenario.freeTalkData;
        ScenarioFreeTalkContainerHeader container{};
        if (ReadFreeTalkValue(eventData, sizeof(container), 0, container) && container.wholeSize >= sizeof(container) &&
            container.wholeSize <= static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())) {
            InstallFreeTalkContainer(eventData, container.wholeSize);
        }
    }

    scenario.freeTalkRecordCount = static_cast<std::uint32_t>(scenarioFreeTalkStorage.recordCount);
    CreateHiddenFreeTalkCharacter(scenario);
    scenario.randomSelectionCount = 0;
    scenario.randomSelections = {};
    scenario.padControlData = AllocatePadControlEntry();
    scenario.inputState = nullptr;
}

void ReleaseFreeTalk() {
    auto &scenario = scenarioEventRuntimeStorage;
    if (scenario.flags & 2U) {
        scenario.flags &= ~2U;
        BindPadControlEntry(scenario.inputState);
    }
    if (PadControlEntry *const owned = scenario.padControlData) {
        owned->flags = 0;
    }
    scenario.padControlData = nullptr;
    scenario.inputState = nullptr;
    if (scenarioFreeTalkStorage.externalData) {
        HLib::FreeTaskData(scenarioFreeTalkStorage.externalData);
    }
    scenarioFreeTalkStorage.externalData = nullptr;
    scenarioFreeTalkStorage.externalSize = 0;
    scenarioFreeTalkStorage.extraRecord = nullptr;
    scenarioFreeTalkStorage.extraRecordSize = 0;
    scenarioFreeTalkStorage.records = {};
    scenarioFreeTalkStorage.recordCount = 0;
    scenarioPersistentFlags.initialized = false;
}
} // namespace event_runtime_detail


bool ScenarioCharacterIsRyo(shen_name name) {
    // scenCheckIsRyo rejects the zero token, but accepts the
    // literal four-character token "null". Pointer aliases precede names.
    if (!name.name)
        return false;
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (activeScenarioEventRuntime && control && FindScenarioCharacter(activeScenarioEventRuntime->task, name) == control->ryo)
        return true;
    return name.name == SHEN_NAME(RYO_) || name.name == SHEN_NAME(null) || name.name == SHEN_NAME(AKIR) || name.name == SHEN_NAME(AKID) ||
        name.name == SHEN_NAME(JAKR) || name.name == SHEN_NAME(AKI_);
}
bool IsScenarioCharacterForcedVisible(shen_name character) {
    const ScenarioEventRuntime *const scenario = activeScenarioEventRuntime;
    if (!scenario || (scenario->flags & ScenarioEventFlagFreeState) == 0)
        return false;
    if (ScenarioCharacterIsRyo(character) || scenario->activeCharacter.name == character.name)
        return true;
    for (const shen_name participant : GetScenarioParticipants())
        if (participant.name == character.name)
            return true;
    return false;
}

bool HasScenarioCharacterControl() {
    const ScenarioEventRuntime *const scenario = activeScenarioEventRuntime;
    if (!scenario)
        return false;
    const ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(scenario->activeCharacter);
    if (record && (record->flags & ScenarioTalkInterrupted) != 0)
        return false;
    return (scenario->flags & ScenarioEventFlagFreeState) != 0;
}


// @verified - p
bool scenCheckActiveRecordBusy() {
    const ScenarioEventRuntime *const scenario = activeScenarioEventRuntime;
    return FindMutableScenarioFreeTalkRecord(scenario->activeCharacter) && !(scenario->flags & 0x20U);
}


void ResetScenarioFreeTalkResumeFlags() {
    for (ScenarioFreeTalkControl &control : GetScenarioTalkPresentationState().characterStates)
        control.flags = (control.flags & ~0xeU) | 4U;
}


bool IsScenarioBusy() {
    return (activeScenarioEventRuntime && (activeScenarioEventRuntime->flags & ScenarioEventFlagFreeState)) || cvsGet();
}


bool IsScenarioBusyForMainIcon() {
    if (cvsGet())
        return true;
    return activeScenarioEventRuntime && (activeScenarioEventRuntime->flags & ScenarioEventFlagFreeState) && !scenarioHardBusy;
}

std::span<const shen_name> GetScenarioParticipants() {
    const ScenarioTalkPresentationState &state = GetScenarioTalkPresentationState();
    return {state.participants.data(), static_cast<std::size_t>(state.participantCount)};
}

std::span<shen_name, 32> GetScenarioCharacterAdmissionNames() {
    return scenarioCharacterAdmissionNames;
}

std::int32_t GetScenarioFollowEligibility(shen_name character) {
    if (!character.name || !HasNpcRuntimeEntry(character))
        return 0;
    if (QueryNpcRouteLiveState(character) == 10)
        return 1;
    switch (character.name) {
    case SHEN_NAME(REN_):
        if (EV_GetShenmueFlag(0x305) || EV_GetShenmueFlag(0x342))
            return 1;
        return 0;
    case SHEN_NAME(WON_):
        return EV_GetShenmueFlag(0x2de);
    case SHEN_NAME(LLY_):
        return EV_GetShenmueFlag(0x377);
    case SHEN_NAME(YUA_):
        return EV_GetShenmueFlag(0x115);
    case SHEN_NAME(SYE_):
        return EV_GetShenmueFlag(0x105);
    case SHEN_NAME(SAM_):
        return EV_GetShenmueFlag(0x375);
    default:
        return 0;
    }
}

void ApplyScenarioFaceLook(std::uint32_t command) {
    if (!activeScenarioEventRuntime || (activeScenarioEventRuntime->flags & ScenarioEventFlagFreeState) == 0)
        return;
    ScenarioTalkPresentationState &state = GetScenarioTalkPresentationState();
    const std::size_t participant = (command >> 12) & 15U;
    const std::size_t index = (command >> 8) & 15U;
    const std::uint32_t lookIndex = static_cast<std::uint32_t>(index);
    std::memcpy(reinterpret_cast<std::byte *>(&state) + offsetof(ScenarioTalkPresentationState, faceLooks) + sizeof(lookIndex) * participant, &lookIndex,
        sizeof(lookIndex));
    shen_name participantName{};
    std::memcpy(&participantName, reinterpret_cast<const std::byte *>(&state) + sizeof(participantName) * participant, sizeof(participantName));
    Character *const character = ResolveScenarioTalkCharacter(participantName);
    if (!character)
        return;
    struct FaceLook {
        std::int16_t upper, lower;
        float upperScale, lowerScale;
    };

    static constexpr std::array<FaceLook, 16> looks{{
        {0, 0, 1.0f, 1.0f}, {3, 3, 0.5f, 0.7f},
        {1, 1, 1.0f, 1.0f}, {5, 5, 1.0f, 1.0f},
        {4, 4, 1.0f, 1.0f}, {21, 21, 1.0f, 1.0f},
        {22, 22, 0.9f, 1.0f}, {2, 2, 0.4f, 1.0f},
        {4, 2, 0.6f, 1.0f}, {0, 0, 1.0f, 1.0f},
        {0, 0, 1.0f, 1.0f}, {0, 0, 1.0f, 1.0f},
        {0, 0, 1.0f, 1.0f}, {0, 0, 1.0f, 1.0f},
        {0, 0, 1.0f, 1.0f}, {0, 0, 1.0f, 1.0f}
    }};
    const FaceLook &look = looks[index];
    const float frames = static_cast<float>(static_cast<std::uint8_t>(command));
    SetUpperFacialExpression(character, look.upper, frames, look.upperScale);
    SetLowerFacialExpression(character, look.lower, frames, look.lowerScale);
}

shen_name GetActiveScenarioCharacter() {
    return activeScenarioEventRuntime ? activeScenarioEventRuntime->activeCharacter : shen_name{};
}

bool IsActiveScenarioCameraSuppressed() {
    const ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(GetActiveScenarioCharacter());
    return record && (record->flags & ScenarioTalkSuppressCamera) != 0;
}

bool IsScenarioButtonAdmissionEnabled() {
    return activeScenarioEventRuntime && (activeScenarioEventRuntime->flags & 2U) != 0;
}

ScenarioFreeTalkCandidate *GetScenarioCandidateList() {
    return scenarioCandidateList;
}

std::span<const std::byte> GetScenarioFreeTalkSource(const ScenarioFreeTalkRecord &record) {
    ScenarioFreeTalkSourceHeader header{};
    if (!record.sourceRecord)
        return {};
    std::memcpy(&header, record.sourceRecord, sizeof(header));
    return {record.sourceRecord, header.size};
}

ScenarioFreeTalkRecord *AllocateScenarioFreeTalkCharacter(Character *character) {
    ScenarioFreeTalkRecord *const record = static_cast<ScenarioFreeTalkRecord *>(AllocateModelComponent(
        character, SCENARIO_FREE_TALK_COMPONENT, sizeof(ScenarioFreeTalkRecord), UpdateScenarioFreeTalkCharacter, FreeScenarioFreeTalkCharacter));
    if (record) {
        std::memset(record, 0, sizeof(*record));
        record->control = &record->localControl;
    }
    return record;
}

// @verified - p
void CollectScenarioCandidateMeshIndex(const MT7Node *node) {
    for (; node; node = mtGetSibling(node)) {
        if (mt7GetXb01Offset(node)) {
            if (static_cast<std::uint16_t>(node->id) == 0xffbd)
                scenarioCandidateTaggedMesh = scenarioCandidateMeshIndex;
            ++scenarioCandidateMeshIndex;
        }
        if (const MT7Node *const child = mtGetTransform(node))
            CollectScenarioCandidateMeshIndex(child);
    }
}

bool AttachScenarioFreeTalkCharacter(Character *character, std::span<const std::byte> scnf) {
    if (ReturnZero() != 0)
        return false;
    if (!character || !activeScenarioEventRuntime)
        return false;
    ScenarioTaskData *const scenario = ScenarioData(activeScenarioEventRuntime->task);
    if (!scenario)
        return false;
    const shen_name name = scenGetCharaName(activeScenarioEventRuntime->task, character);
    if (!name.name)
        return false;
    ScenarioFreeTalkRecord *const record = static_cast<ScenarioFreeTalkRecord *>(FindModelComponent(character, SCENARIO_FREE_TALK_COMPONENT));
    if (!record)
        return false;
    std::span<const std::byte> source{};
    if (scnf.empty()) {
        source = {scenarioFreeTalkStorage.extraRecord, scenarioFreeTalkStorage.extraRecordSize};
    } else {
        source = FindFreeTalkCharacterSource(scnf, name, false);
        const auto external = FindFreeTalkCharacterSource({scenarioFreeTalkStorage.externalData, scenarioFreeTalkStorage.externalSize}, name, true);
        if (!external.empty())
            source = external;
    }
    if (source.empty())
        return false;
    if (!ExpandFreeTalkRecord(source.data(), source.size(), *record))
        return false;
    if (scnf.empty())
        record->characterStateIndex = 0xffff;
    if (static_cast<std::int16_t>(record->characterStateIndex) <= 0) {
        record->name = name;
        record->control = &record->localControl;
    } else if (record->name.name != name.name) {
        return false;
    }
    ScenarioFreeTalkCandidate *const candidate = scenarioCandidateFreeList;
    if (!candidate)
        return false;
    scenarioCandidateFreeList = candidate->previous;
    *candidate = {};
    candidate->name = record->name;
    candidate->characterStateIndex = record->characterStateIndex;
    scenarioCandidateMeshIndex = 0;
    scenarioCandidateTaggedMesh = 0;
    if (const MT7Model *const model = static_cast<const MT7Model *>(character->model))
        CollectScenarioCandidateMeshIndex(GetMT7ReaderData(model));
    candidate->flags = scenarioCandidateTaggedMesh;
    candidate->character = character;
    candidate->motionWork = FindMotionWork(character);
    candidate->record = record;
    candidate->previous = scenarioCandidateList;
    if (scenarioCandidateList)
        scenarioCandidateList->next = candidate;
    scenarioCandidateList = candidate;
    record->candidate = candidate;
    record->alternateRoot = nullptr;
    record->defaultRoot = nullptr;
    const std::byte *const maximumRoot = FindScenarioConversationRoot(*record, 999999);
    record->defaultRoot = FindScenarioConversationRoot(*record, 0);
    if (record->defaultRoot == maximumRoot)
        record->defaultRoot = nullptr;
    std::int32_t step{};
    if (!record->defaultRoot) {
        step = 101000;
        record->defaultRoot = FindScenarioConversationRoot(*record, step);
        if (record->defaultRoot == maximumRoot)
            record->defaultRoot = nullptr;
    }
    if (record->defaultRoot)
        record->alternateRoot = FindScenarioConversationRoot(*record, step);
    record->requestedRoot = 0;
    return true;
}

void InitiateCleanupForSceneChara(shen_name name) {
    if (!scenarioPersistentFlags.initialized || !activeScenarioEventRuntime ||
        ((activeScenarioEventRuntime->flags & ScenarioEventFlagFreeState) != 0 && activeScenarioEventRuntime->activeCharacter.name == name.name))
        return;
    if (name.name == 0xffffffffU) {
        for (ScenarioFreeTalkControl &control : GetScenarioTalkPresentationState().characterStates) {
            control.resumeOffsets = {};
            control.requestedRootOffset = 0;
            control.flags = (control.flags & 0xffffffc0U) | 5U;
        }
    }
    for (ScenarioFreeTalkCandidate *candidate = scenarioCandidateList; candidate; candidate = candidate->previous) {
        if (name.name != 0xffffffffU && candidate->name.name != name.name)
            continue;
        ScenarioFreeTalkRecord &record = *candidate->record;
        ScenarioFreeTalkControl &control = *record.control;
        record.executionState = 0;
        control.flags = (control.flags & 0xffffffc0U) | 5U;
        control.resumeOffsets = {};
        control.requestedRootOffset = 0;
        control.completedCount = 0;
        control.activationCount = 0;
        record.scriptCursor = record.script;
        record.flags &= ~(ScenarioTalkWaitAction | ScenarioTalkWaitStart | ScenarioTalkResumeReached);
        if (name.name != 0xffffffffU)
            break;
    }
}

std::span<const ScenarioFreeTalkRecord> GetScenarioFreeTalkRecords() {
    return {scenarioFreeTalkStorage.records.data(), scenarioFreeTalkStorage.recordCount};
}

std::span<const std::byte> GetScenarioFreeTalkExtraRecord() {
    if (!scenarioFreeTalkStorage.extraRecord || scenarioFreeTalkStorage.extraRecordSize == 0) {
        return {};
    }
    return {scenarioFreeTalkStorage.extraRecord, scenarioFreeTalkStorage.extraRecordSize};
}

const ScenarioFreeTalkRecord *FindScenarioFreeTalkRecord(shen_name character) {
    return FindMutableScenarioFreeTalkRecord(character);
}

bool IsScenarioFreeTalkInitialized() {
    return scenarioPersistentFlags.initialized;
}
namespace event_runtime_detail {

void RetireScenarioFreeTalkRecord(ScenarioFreeTalkRecord *record) {
    if (!record)
        return;
    if (record->flags & ScenarioTalkPresentationActive) {
        record->flags &= ~ScenarioTalkPresentationActive;
        activeScenarioEventRuntime->activeCharacter = {};
        activeScenarioEventRuntime->previousCharacter = {};
        record->flags &= ~ScenarioTalkSuppressCamera;
        EndScenarioPlayerPresentation(0);
        ConsumeScenarioCameraRestoreRequest();
    }
    record->flags &= ~ScenarioTalkWaitFollow;
    if (record->executionState == 7) {
        record->flags &= ~(ScenarioTalkSavedScript | ScenarioTalkWaitAction);
        record->executionState = 1;
        record->savedScriptCursor = nullptr;
    }
    record->flags &= ~ScenarioTalkWaitStart;
    record->flags &= ~ScenarioTalkPromptClockStarted;
}
} // namespace event_runtime_detail


// @verified - p
std::int64_t StartScenarioFreeTalkRecord(shen_name character, std::int16_t root) {
    if (!scenarioPersistentFlags.initialized || (activeScenarioEventRuntime->flags & 0x800U) || cvsGet())
        return 0;
    RetireScenarioFreeTalkRecord(FindMutableScenarioFreeTalkRecord(activeScenarioEventRuntime->activeCharacter));
    DrainScenarioAcknowledgements();
    ScenarioFreeTalkRecord *record = FindMutableScenarioFreeTalkRecord(character);
    if (!record) {
        ScenarioFreeTalkCandidate *candidate = scenarioCandidateList;
        while (candidate && candidate->name.name != character.name)
            candidate = candidate->previous;
        if (!candidate || !candidate->record)
            return 1;
        record = candidate->record;
        candidate->availability = 1;
    }
    ScenarioTalkPresentationState &presentation = GetScenarioTalkPresentationState();
    presentation.playerFacingMode = 0;
    presentation.npcFacingIssued = 0;
    presentation.playerFacingIssued = 0;
    activeScenarioEventRuntime->flags |= 0x841U;
    activeScenarioEventRuntime->activeCharacter = character;
    record->executionState = 0;
    presentation.facingCommandsIssued = 0;
    presentation.unfocusedFacingRequested = 0;
    presentation.queuedFaceLookCount = 0;
    presentation.participantCount = 0;
    presentation.suppressNpcFacing = 1;
    presentation.suppressPlayerFacing = 1;
    record->control->flags &= ~4U;
    record->control->flags &= ~2U;
    const std::uint32_t flags = record->flags & ~(ScenarioTalkWaitAction | ScenarioTalkWaitStart | ScenarioTalkPromptClockStarted);
    record->dialogueChoiceIndex = static_cast<std::uint16_t>(root);
    record->flags = flags | ScenarioTalkSelected;
    return 1;
}

// @verified - p
std::int64_t StopScenarioFreeTalkRecord() {
    ScenarioEventRuntime *scenario = activeScenarioEventRuntime;
    const shen_name character = scenario->activeCharacter;
    scenario->flags &= ~0x841U;
    ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(character);
    if (record) {
        EndScenarioPlayerPresentation(0);
        if (activeScenarioEventRuntime->flags & 2U) {
            activeScenarioEventRuntime->flags &= ~2U;
            BindPadControlEntry(activeScenarioEventRuntime->padControlData);
        }
        RetireScenarioFreeTalkRecord(record);
        scenario = activeScenarioEventRuntime;
    }
    scenario->activeCharacter = {};
    return DrainScenarioAcknowledgements();
}
namespace event_runtime_detail {




void scenUpdateCandidateAvailability() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const shen_name active = scenario.activeCharacter;
    const ScenarioFreeTalkRecord *const activeRecord = FindMutableScenarioFreeTalkRecord(active);
    if (!activeRecord || !(activeRecord->flags & 0x10000U)) {
        ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
        talk.suppressNpcFacing = 0;
        talk.suppressPlayerFacing = 0;
        talk.playerFacingMode = 0;
        talk.npcFacingIssued = 0;
        talk.playerFacingIssued = 0;
        talk.facingCommandsIssued = 0;
        talk.unfocusedFacingRequested = 0;
        if (active.name && !(scenario.flags & 0x40U))
            scenEndConversation();
    }
    for (ScenarioFreeTalkCandidate *candidate = scenarioCandidateList; candidate; candidate = candidate->previous) {
        Character *const character = candidate->character;
        const ScenarioFreeTalkRecord *const record = candidate->record;
        candidate->availability = 0;
        if (!character || !record || authGetLoaded() || character->entity.name.name != CHARACTER_ENTITY.name || !(character->flags & CF_Valid) ||
            !IsOfThisVisibilityType(character, 3) || !record->name.name || !record->sourceRecord || !record->script)
            continue;
        if (HasNpcRuntimeEntry(record->name))
            candidate->availability = CanNpcEnterScenario(record->name) ? 1 : -1;
        else if (!(record->characterStateIndex & 0x8000U))
            candidate->availability = 1;
    }
}


void scenActivateCandidate(ScenarioFreeTalkCandidate &candidate) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    ScenarioFreeTalkRecord &record = *candidate.record;
    const shen_name active = scenario.activeCharacter;
    if (active.name && active.name != record.name.name)
        return;
    std::int8_t searchState = candidate.searchState;
    if (searchState == 2)
        searchState = 0;
    else if (searchState < 0)
        return;
    if (candidate.proximity < -100.0f)
        return;
    const std::uint32_t flags = record.flags;
    if (flags & ScenarioTalkCharacterActive)
        searchState = 1;
    bool activate = (flags & ScenarioTalkInterrupted) || searchState;
    if (!activate) {
        if (record.pendingScriptCursor) {
            if (ryoCheckBusyFlags() && (plymGetMovingFrames() > 0 || plymGetTurningFrames() > 0)) {
                ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
                activate = true;
                talk.playerFacingMode = 0;
                talk.npcFacingIssued = 0;
                talk.playerFacingIssued = 0;
                scenario.flags |= 0x800U;
                record.flags |= ScenarioTalkInterrupted;
                record.savedScriptCursor = record.scriptCursor;
                record.scriptCursor = record.pendingScriptCursor;
                talk.facingCommandsIssued = 0;
                talk.unfocusedFacingRequested = 0;
                talk.suppressNpcFacing = 1;
                talk.suppressPlayerFacing = 1;
            }
            record.pendingScriptCursor = nullptr;
        } else {
            if (flags & ScenarioTalkPreviouslySelected) {
                scenario.pendingCharacter = record.name;
                if (active.name == record.name.name) {
                    scenario.flags &= ~0x800U;
                    scenario.activeCharacter = {};
                    record.flags &= ~ScenarioTalkPromptClockStarted;
                }
                record.flags &= ~ScenarioTalkPreviouslySelected;
                RetireScenarioFreeTalkRecord(&record);
            }
            record.flags &= ~ScenarioTalkSelected;
            if ((record.flags & ScenarioTalkCompletedResume) && (record.control->flags & ScenarioTalkBinaryPrompt)) {
                record.executionState = 3;
                record.flags &= ~(ScenarioTalkResumeReached | ScenarioTalkCompletedResume);
                record.control->resumeOffsets[1] = 0;
                record.control->resumeOffsets[2] = 0;
            }
        }
    }
    if (!activate)
        return;
    const shen_name current = scenario.activeCharacter;
    const ScenarioFreeTalkRecord *const currentRecord = FindMutableScenarioFreeTalkRecord(current);
    if (currentRecord && current.name != record.name.name && (currentRecord->flags & ScenarioTalkSelected))
        return;
    record.flags |= (ScenarioTalkSelected | ScenarioTalkPreviouslySelected);
    scenario.activeCharacter = record.name;
}



void scenUpdateFreeTalkInteraction() {
    using enum CameraState;
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (!GetParentMenu() && !(scenario.flags & 0x40U))
        return;
    if (IsItemSelectable() || IsAnyItemSelectActive() || IsSaveAllowed())
        return;
    if (!IsScreenFilmNotShowing(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->screenFilmTask))
        return;
    scenario.flags &= ~0x2020U;
    const bool sinForced = (scenario.flags & 0x40U) && scenario.activeCharacter.name == SHEN_NAME(SIN_);
    const bool authorTarget = plymCheckAuthorTargetRequested();
    scenario.flags = (scenario.flags & ~0x4000U) | ((sinForced || authorTarget) ? 0x4000U : 0U);
    scenUpdateCandidateAvailability();
    if (scenario.flags & 1U) {
        if (ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(scenario.activeCharacter)) {
            scenExecuteRecord(record);
            record->control->flags = (record->control->flags & ~0x10U) | (record->executionState == 3 ? 0x10U : 0U);
            if (IsScenarioActiveRecordIdle()) {
                scenario.flags &= ~0x841U;
                scenario.activeCharacter = {};
                record->flags &= ~ScenarioTalkPromptClockStarted;
            }
        }
        return;
    }
    ScenarioFreeTalkCandidate *selected = nullptr;
    if (!(scenario.flags & 0x800U)) {
        const ScenarioFreeTalkRecord *const active = FindMutableScenarioFreeTalkRecord(scenario.activeCharacter);
        if (!active || !(active->flags & (ScenarioTalkWaitStart | ScenarioTalkWaitFollow | ScenarioTalkWaitEnd))) {
            const bool lockon = srchHasActiveLockonTarget();
            selected = srchEvaluateSearchTarget();
            for (ScenarioFreeTalkCandidate *candidate = scenarioCandidateList; candidate; candidate = candidate->previous) {
                if (ryoCheckBusyFlags() && candidate->availability) {
                    SetFaceFlagsForChara_0(candidate->character, candidate == selected && candidate->searchState);
                    SetFaceFlagsForChara(candidate->character, 1);
                }
                if (lockon)
                    candidate->availability = -3;
                if (candidate->availability > 0 && candidate != selected) {
                    if (candidate->searchState == 1)
                        candidate->searchState = 2;
                    scenActivateCandidate(*candidate);
                }
            }
            if (selected && selected->availability > 0)
                scenActivateCandidate(*selected);
        }
    }
    const shen_name name = scenario.activeCharacter;
    if (name.name == scenario.previousCharacter.name && name.name) {
        if (ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(name)) {
            scenExecuteRecord(record);
            record->control->flags = (record->control->flags & ~0x10U) | (record->executionState == 3 ? 0x10U : 0U);
        } else {
            scenario.pendingCharacter = name;
        }
    } else {
        scenario.previousCharacter = name;
    }
    if (!selected)
        return;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    const shen_name selectedName = selected->record->name;
    if (selectedName.name != srchGetCurrentFocus().name) {
        talk.followRouteWait = 0;
        return;
    }
    if (talk.followRouteWait) {
        if (QueryNpcRouteLiveState(selectedName) == 11)
            return;
        SetScenarioFollowCharacter({});
        camCtrlSetState(Init);
        talk.followRouteWait = 0;
        return;
    }
    if (QueryNpcRouteLiveState(selectedName) == 11)
        talk.followRouteWait = -1;
}
} // namespace event_runtime_detail




void scenUpdateFreeTalkRuntime(ScenarioEventRuntime *scenario) {
    if (!scenario || scenario != activeScenarioEventRuntime)
        return;
    scenarioTalkPromptResumed = 0;
    scenarioFollowPromptShown = 0;
    scenarioHardBusy = 0;
    if (!scenarioPersistentFlags.initialized) {
        ResetScenarioFlags();
        ReadFreeTalk(*scenario);
        CreateCharactersDependingOnCurrentStep(*scenario);
        scenarioPersistentFlags.initialized = true;
    } else {
        scenario->pendingCharacter = {};
        RebuildScenarioEventObjectMapNames();
        scenUpdateFreeTalkInteraction();
        scenProcessCharacterActionQueue();
        UpdateGameplayFlags();
        if (scenario->pendingCharacter.name) {
            scenClearSpeakerPresentation();
            EndScenarioPlayerPresentation(0);
            ConsumeScenarioCameraRestoreRequest();
            ReleaseScenarioTalkInput();
        }
    }
    Vector3 center{};
    if (scenarioFreeStateState)
        scenarioFreeStateState = npcSetAreaName(talkArea, &center, 0.0f, 0);
    if (!(scenario->flags & ScenarioEventFlagFreeState))
        return;
    Character *const character = FindScenarioCharacter(scenario->task, scenario->activeCharacter);
    plymUpdatePlayerSearchLocation(&center);
    float radius = 1.0f;
    if (character) {
        const Vector3 target = character->entity.node.position;
        const float deltaX = target.x - center.x;
        const float deltaY = target.y - center.y;
        const float deltaZ = target.z - center.z;
        center.x += deltaX * 0.5f;
        center.y += deltaY * 0.5f;
        center.z += deltaZ * 0.5f;
        radius = std::sqrt((deltaY * deltaY + deltaX * deltaX) + deltaZ * deltaZ) * 0.5f + 0.5f;
    }
    scenarioFreeStateState = npcSetAreaName(talkArea, &center, radius, 1);
}

} // namespace shenmue::s2
