#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

void DestroyScenOwnedSequenceCharacters() {
    if (!g_scenOwnedSequenceTask)
        return;
    for (std::uint32_t &id : g_scenOwnedSequenceCharacters) {
        if (!id)
            continue;
        if (Character *const character = CHID_Find(g_scenOwnedSequenceTask, id)) {
            CHAR_DestroyAndFreeComponents(character);
            SCEN_UnlinkCharacterIndexNode(g_scenOwnedSequenceTask, character);
        }
        id = 0;
    }
    g_scenOwnedSequenceTask = nullptr;
}

const ScenRuntimeContainer *FindScenRuntimeContainer(const std::byte *payload, std::uint32_t payloadSize) {
    if (!payload || payloadSize < sizeof(ScenRuntimeContainer))
        return nullptr;
    for (std::size_t offset = 0; offset + sizeof(ScenRuntimeContainer) <= payloadSize; offset += 4) {
        const auto *const candidate = reinterpret_cast<const ScenRuntimeContainer *>(payload + offset);
        if (candidate->tag == SHEN_NAME(SCNF) && candidate->wholeSize >= sizeof(ScenRuntimeContainer) && candidate->wholeSize <= payloadSize - offset) {
            return candidate;
        }
    }
    return nullptr;
}

bool ReadScenFile(const char *directory, const char *filename, std::vector<std::byte> &bytes) {
    bytes.clear();
    const std::uint64_t size = shenOpenFile(directory, filename);
    if (!size || size > std::numeric_limits<std::uint32_t>::max())
        return false;
    void *const file = ReadFile(directory, filename, static_cast<std::int32_t>(SHEN_NAME(SCNF)));
    if (!file)
        return false;
    bytes.resize(static_cast<std::size_t>(size));
    std::memcpy(bytes.data(), file, bytes.size());
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
    return true;
}

bool ReadScenI32(const std::vector<std::byte> &bytes, std::size_t offset, std::int32_t &value) {
    if (offset > bytes.size() || sizeof(value) > bytes.size() - offset)
        return false;
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

const ScenRuntimeContainer *LoadScenSCSTFromColdStorage() {
    auto &storage = g_scenOwnedRecordStorage;
    if (!storage.scstBytes.empty()) {
        if (const auto *const cached = FindScenRuntimeContainer(storage.scstBytes.data(), static_cast<std::uint32_t>(storage.scstBytes.size())))
            return cached;
        storage.scstBytes.clear();
    }

    std::vector<std::byte> coldBytes;
    if (!ReadScenFile("misc", "cold.bin", coldBytes))
        return nullptr;

    std::int32_t scstTableByteOffset = 0;
    if (!ReadScenI32(coldBytes, 0x1c, scstTableByteOffset) || scstTableByteOffset < 0)
        return nullptr;

    constexpr std::size_t kMobjBase = 0x10;
    const std::uint64_t tableOffset = static_cast<std::uint64_t>(kMobjBase) + 0x20u + static_cast<std::uint32_t>(scstTableByteOffset);
    if (tableOffset > coldBytes.size() || 8u > coldBytes.size() - static_cast<std::size_t>(tableOffset))
        return nullptr;

    std::int32_t blockRelativeOffset = 0;
    std::int32_t blockSize = 0;
    if (!ReadScenI32(coldBytes, static_cast<std::size_t>(tableOffset), blockRelativeOffset) ||
        !ReadScenI32(coldBytes, static_cast<std::size_t>(tableOffset) + 4, blockSize) || blockRelativeOffset < 0 || blockSize <= 0)
        return nullptr;

    const std::uint64_t blockOffset = static_cast<std::uint64_t>(kMobjBase) + static_cast<std::uint32_t>(blockRelativeOffset);
    if (blockOffset > coldBytes.size() || static_cast<std::uint32_t>(blockSize) > coldBytes.size() - static_cast<std::size_t>(blockOffset))
        return nullptr;

    storage.scstBytes.assign(coldBytes.begin() + static_cast<std::ptrdiff_t>(blockOffset),
        coldBytes.begin() + static_cast<std::ptrdiff_t>(blockOffset + static_cast<std::uint32_t>(blockSize)));
    if (const auto *const container = FindScenRuntimeContainer(storage.scstBytes.data(), static_cast<std::uint32_t>(storage.scstBytes.size())))
        return container;
    storage.scstBytes.clear();
    return nullptr;
}

bool ScenRuntimeRecordHeaderValid(const ScenRuntimeRecord *record) {
    if (!record || record->size < sizeof(*record) || record->participantCount > 32 || record->locationCount > 64 || record->kvPairCount > 64 ||
        record->name[0] == 0)
        return false;
    for (const char character : record->name) {
        const auto value = static_cast<unsigned char>(character);
        if (value < 0x20 || value >= 0x7f)
            return false;
    }
    const auto validOffset = [record](std::uint32_t offset) { return offset == 0 || offset < record->size; };
    return validOffset(record->voicePathOffset) && validOffset(record->scriptOffset) && validOffset(record->lineTableOffset) &&
        validOffset(record->participantBlockOffset) && validOffset(record->data40Offset) && validOffset(record->locationTableOffset) &&
        validOffset(record->kvTableOffset);
}

bool ScenRuntimeRecordFits(const std::byte *begin, const std::byte *end, const ScenRuntimeRecord *record) {
    if (!begin || !end || !record || end <= begin)
        return false;
    const auto *const bytes = reinterpret_cast<const std::byte *>(record);
    if (bytes < begin || bytes + sizeof(*record) > end || record->size > static_cast<std::uint32_t>(end - bytes) || !ScenRuntimeRecordHeaderValid(record))
        return false;
    return true;
}

void IndexScenRuntimeContainer(const ScenRuntimeContainer *container) {
    if (!container)
        return;
    const auto *const begin = reinterpret_cast<const std::byte *>(container);
    const auto *const end = begin + container->wholeSize;
    const auto *cursor = begin + sizeof(*container);
    while (cursor + sizeof(ScenRuntimeRecord) <= end) {
        const auto *const record = reinterpret_cast<const ScenRuntimeRecord *>(cursor);
        if (!ScenRuntimeRecordFits(begin, end, record))
            break;
        const std::uint32_t token = make_shen_name(record->name[0], record->name[1], record->name[2], record->name[3]);
        const auto found =
            std::find_if(g_scenIndexedRecords.begin(), g_scenIndexedRecords.end(), [token](const auto &indexed) { return indexed.first == token; });
        if (token && found == g_scenIndexedRecords.end())
            g_scenIndexedRecords.emplace_back(token, record);
        cursor += record->size;
    }
}

void IndexScenRuntimeRecords() {
    g_scenIndexedRecords.clear();




    IndexScenRuntimeContainer(LoadScenSCSTFromColdStorage());
    for (const std::uint32_t token : {SHEN_NAME(SCST), SHEN_NAME(FNCS)}) {
        std::uint32_t payloadSize = 0;
        const std::byte *const payload = MAPINFO_FindChunkPayload(token, &payloadSize);
        IndexScenRuntimeContainer(FindScenRuntimeContainer(payload, payloadSize));
    }
}

std::uint16_t ResolveScenScriptSlot(const ScenRuntimeObject &object) {
    std::uint16_t slot = 0;
    bool valid = false;
    const auto fixedSlot = [&object]() {
        const auto entry = std::find(kValidScenCharacterNames.begin(), kValidScenCharacterNames.end(), object.name.name);
        return entry == kValidScenCharacterNames.end() ? std::uint16_t{0} : static_cast<std::uint16_t>(entry - kValidScenCharacterNames.begin());
    };

    if ((ScenFlags() & kScenarioFlagUnknown200) != 0) {
        slot = fixedSlot();
        valid = slot != 0;
    } else {
        const char *const slash = object.voicePath ? std::strrchr(object.voicePath, '/') : nullptr;
        if (!slash) {
            slot = 0;
        } else {
            const char *basename = slash + 1;
            if (*basename != 'F') {
                slot = fixedSlot();
                valid = slot != 0;
            } else {
                do {
                    ++basename;
                } while (*basename && static_cast<std::uint8_t>(*basename - '0') > 9U);
                slot = static_cast<std::uint16_t>(std::strtoul(basename, nullptr, 10) % 1000U);
                valid = true;
            }
        }
    }
    if (valid && slot < kValidScenCharacterNames.size())
        return slot;

    const auto existing = std::find(g_scenRuntimeState.dynamicSlotNames.begin(), g_scenRuntimeState.dynamicSlotNames.end(), object.name.name);
    if (existing != g_scenRuntimeState.dynamicSlotNames.end()) {
        return static_cast<std::uint16_t>(301 + existing - g_scenRuntimeState.dynamicSlotNames.begin());
    }
    std::int32_t cursor = g_scenRuntimeState.dynamicSlotCursor + 1;
    if (cursor >= static_cast<std::int32_t>(g_scenRuntimeState.dynamicSlotNames.size())) {
        cursor = 0;
    }
    g_scenRuntimeState.dynamicSlotCursor = cursor;
    g_scenRuntimeState.dynamicSlotNames[static_cast<std::size_t>(cursor)] = object.name.name;
    return static_cast<std::uint16_t>(301 + cursor);
}

void LoadScenRuntimeObject(const ScenRuntimeContainer *container, const ScenRuntimeRecord *record, ScenRuntimeObject &object) {
    object = {};
    object.name = shen_name{make_shen_name(record->name[0], record->name[1], record->name[2], record->name[3])};
    object.participantCount = record->participantCount;
    object.stateByte19 = static_cast<std::int8_t>(record->reserved09);
    object.locationCount = record->locationCount;
    object.kvPairCount = static_cast<std::int8_t>((std::min)(record->kvPairCount, static_cast<std::uint8_t>(127)));
    object.runtimeFlags = 0x8000U;
    const auto *const base = reinterpret_cast<const std::byte *>(record);
    const auto *const end = container ? reinterpret_cast<const std::byte *>(container) + container->wholeSize : base + record->size;
    const auto pointer = [base, end](std::uint32_t offset) {
        return offset && offset < static_cast<std::uint32_t>(end - base) ? const_cast<std::byte *>(base + offset) : nullptr;
    };
    object.participants = pointer(record->participantBlockOffset);
    object.locations = reinterpret_cast<Vector3 *>(pointer(record->locationTableOffset));
    object.data40 = reinterpret_cast<Vector3 *>(pointer(record->data40Offset));
    object.voicePath = reinterpret_cast<char *>(pointer(record->voicePathOffset));
    object.scriptSlot = ResolveScenScriptSlot(object);
    object.kvPairs = reinterpret_cast<ScenRuntimeKVPair *>(pointer(record->kvTableOffset));
    object.scriptStart = pointer(record->scriptOffset);
    object.lineTable = pointer(record->lineTableOffset);
    object.fileHeader = const_cast<ScenRuntimeRecord *>(record);
}

void LoadScenSequenceObjects() {
    g_scenRuntimeState.sequenceObjects.fill(ScenRuntimeObject{});
    g_scenRuntimeState.sequenceObjectCount = 0;
    for (const auto &indexed : g_scenIndexedRecords) {
        if (g_scenRuntimeState.sequenceObjectCount >= static_cast<std::int32_t>(g_scenRuntimeState.sequenceObjects.size()))
            break;
        ScenRuntimeObject &object = g_scenRuntimeState.sequenceObjects[static_cast<std::size_t>(g_scenRuntimeState.sequenceObjectCount)];
        LoadScenRuntimeObject(nullptr, indexed.second, object);
        if (static_cast<std::size_t>(object.scriptSlot) * 3U < g_scenRuntimeState.rangeValues.size())
            g_scenRuntimeState.rangeValues[static_cast<std::size_t>(object.scriptSlot) * 3U] = 9.0f;
        ++g_scenRuntimeState.sequenceObjectCount;
    }
}



bool ScenCycleEntryEligible(const CycleManager::NpcRecord &npc, bool runningPrograms, bool &carry) {
    const float opacity = !npc.cycle->obj || npc.cycle->xm_resume_state || runningPrograms ? 1.0f : npc.cycle->obj->renderOpacity;
    if (!npc.cycle->loadedBinHandle || !(opacity > 0.99000001f) || npc.cycle->contextValue == -999)
        return false;
    if (npc.cycle->currentOpcode == CycleOpcode_UseMnrBEntry) {
        if (!CycleManager::MnrBEntryAllowsTalk(npc))
            return false;
    } else if (npc.cycle->currentOpcode == CycleOpcode_ScheduleMnrBEntry) {
        if (npc.cycle->xm_resume_state) {
            carry = true;
        } else {
            const auto kind = static_cast<std::int16_t>(npc.cycle->morsCommandStatus >> 16);
            if (kind == 0)
                carry = false;
            else if (kind == 1 || kind == 3)
                carry = true;
        }
        if (!carry)
            return false;
    }
    switch (npc.cycle->currentOpcode) {
    case CycleOpcode_InitTimedPath:
        if (npc.cycle->animMoveSpeedScale > 0.0625f)
            return false;
        break;
    case 5:
    case 13:
    case 14:
    case 21:
    case 30:
    case 32:
    case 36:
    case 42:
    case 44:
    case 49:
    case 50:
    case 58:
    case 59:
        return false;
    case CycleOpcode_StartLinkedCycle:
        return npc.cycle->moveObjPathActive;
    default:
        break;
    }
    const Vector3 &center = g_cycleManagerRuntime.center;
    const float dy = npc.cycle->pos.y - center.y;
    const float dx = npc.cycle->pos.x - center.x;
    const float dz = npc.cycle->pos.z - center.z;
    return npc.cycle->xm_resume_state || !(0.1156f > dy * dy + dx * dx + dz * dz);
}

void EnsureScenSequenceObjects() {
    if (g_scenSequenceObjectsInitialized)
        return;
    IndexScenRuntimeRecords();
    LoadScenSequenceObjects();
    g_scenSequenceObjectsInitialized = true;
}


void UpdateScenTaskData() {
    EnsureScenSequenceObjects();
    ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    if (active && (active->runtimeFlags & kRuntimeFlagStateActive) != 0) {
        const std::uint32_t name = active->name.name;
        if ((ScenFlags() & kScenarioFlagUnknown200) != 0 || name == g_cpctLastOperand || name == g_cpctPreviousOperand ||
            name == g_scenarioStepLut.pendingCharacterName)
            return;
        bool present = false;
        ForEachEligibleScenCycleNpc([&](const CycleManager::NpcRecord &npc) {
            present = npc.cycle->moveConfig->obj_id.name == name;
            return !present;
        });
        if (present)
            return;
        for (std::int32_t index = 0; index < g_scenRuntimeState.objectCount; ++index) {
            if (g_scenRuntimeState.objects[static_cast<std::size_t>(index)].name.name == name) {
                g_scenRuntimeState.objects[static_cast<std::size_t>(index)] = ScenRuntimeObject{};
                break;
            }
        }
        ResetScenScriptRuntimeState();
        return;
    }

    const std::array<ScenRuntimeObject, 24> previousObjects = g_scenRuntimeState.objects;
    const std::int32_t previousCount = g_scenRuntimeState.objectCount;
    g_scenRuntimeState.objectCount = 0;
    for (std::int32_t index = 0; index < previousCount; ++index)
        g_scenRuntimeState.objects[static_cast<std::size_t>(index)] = ScenRuntimeObject{};
    std::int32_t built = 0;
    ForEachEligibleScenCycleNpc([&](const CycleManager::NpcRecord &npc) {
        if (built < 24) {
            ScenRuntimeObject &object = g_scenRuntimeState.objects[static_cast<std::size_t>(built++)];
            LoadScenRuntimeObject(
                nullptr, reinterpret_cast<const ScenRuntimeRecord *>(static_cast<const std::byte *>(npc.cycle->loadedBinHandle) + 16), object);
            object.field14 = -1;
            object.runtimeState = 1;
            ++g_scenRuntimeState.objectCount;
        }
        return true;
    });

    // Carry each surviving object's script state across the rebuild; four script pointers follow the record by the low-dword header delta.
    for (std::int32_t previousIndex = 0; previousIndex < previousCount; ++previousIndex) {
        const ScenRuntimeObject &previous = previousObjects[static_cast<std::size_t>(previousIndex)];
        std::int32_t index = 0;
        while (index < g_scenRuntimeState.objectCount && g_scenRuntimeState.objects[static_cast<std::size_t>(index)].name.name != previous.name.name)
            ++index;
        if (index >= g_scenRuntimeState.objectCount)
            continue;
        ScenRuntimeObject &object = g_scenRuntimeState.objects[static_cast<std::size_t>(index)];
        const auto delta = static_cast<std::int32_t>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(object.fileHeader)) -
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(previous.fileHeader)));
        void *const hashNext = object.hashNext;
        object = previous;
        object.hashNext = hashNext;
        if (previous.instructionPtr)
            object.instructionPtr = previous.instructionPtr + delta;
        if (previous.relocatedPtrA8)
            object.relocatedPtrA8 = previous.relocatedPtrA8 + delta;
        if (previous.savedInstructionPtr)
            object.savedInstructionPtr = previous.savedInstructionPtr + delta;
        if (previous.matrix)
            object.matrix = static_cast<std::byte *>(previous.matrix) + delta;
    }
}

void CreateScenSequenceCharacters() {
    if (!g_scenarioFunctionWork || !g_scenarioFunctionWork->scenarioTask) {
        return;
    }
    IndexScenRuntimeRecords();
    LoadScenSequenceObjects();
    g_scenSequenceObjectsInitialized = true;
    g_scenOwnedSequenceTask = g_scenarioFunctionWork->scenarioTask;
    if (!g_scenarioFunctionWork->characterHandler) {
        g_scenarioFunctionWork->characterHandler = AddCharacterHandler(CharacterHandlerPriority::Low);
        Character *const character = g_scenarioFunctionWork->characterHandler;
        if (character) {
            CHID_Init(g_scenOwnedSequenceTask, make_shen_name('$', 'f', 'r', 'e'), character);
            SetCharacterVisibility(character, false);
        }
    }
    g_scenRuntimeState.randomBranchCount = 0;
    g_scenRuntimeState.randomBranchValues.fill(0);
    g_scenHelpPromptOverride = 0;
    g_scenarioFunctionWork->currentPadEntry = AllocateClearedPADCEntry();
    g_freeTalkInitialized = 1;
    g_scenarioFunctionWork->active = 1;
}

ScenRuntimeObject *FindScenRuntimeObject(std::uint32_t name) {
    if (!name)
        return nullptr;
    const std::int32_t sequenceCount = std::clamp(g_scenRuntimeState.sequenceObjectCount, 0, 12);
    for (std::int32_t index = 0; index < sequenceCount; ++index) {
        ScenRuntimeObject &object = g_scenRuntimeState.sequenceObjects[static_cast<std::size_t>(index)];
        if (object.name.name == name)
            return &object;
    }
    const std::int32_t count = std::clamp(g_scenRuntimeState.objectCount, 0, 24);
    for (std::int32_t index = 0; index < count; ++index) {
        ScenRuntimeObject &object = g_scenRuntimeState.objects[static_cast<std::size_t>(index)];
        if (object.name.name == name)
            return &object;
    }
    return nullptr;
}

void ResetScenRuntimeState() {
    g_scenOwnedSequenceCharacters.fill(0);
    g_scenOwnedSequenceTask = nullptr;
    g_scenRuntimeState = {};
    for (std::size_t index = 0; index < g_scenRuntimeState.rangeValues.size(); index += 3)
        g_scenRuntimeState.rangeValues[index] = 9.0f;
    g_scenSearchCharacters.fill(0);
    g_scntWord20.fill(0);
    g_scntWord22.fill(0);
    g_scntWord24.fill(0);
    g_scntWord26.fill(0);
    g_scenSequenceObjectsInitialized = false;
    g_cpctPreviousOperand = 0;
    g_cpctSleepPromptReady = true;
    g_cpctInputUpdatePending = false;
    g_scenIndexedRecords.clear();
    g_scenOwnedRecordStorage = {};
}

void ResetScenMapEpochRecords() {
    DestroyScenOwnedSequenceCharacters();
    g_scenRuntimeState = {};
    for (std::size_t index = 0; index < g_scenRuntimeState.rangeValues.size(); index += 3)
        g_scenRuntimeState.rangeValues[index] = 9.0f;
    g_scenSearchCharacters.fill(0);
    g_scntWord20.fill(0);
    g_scntWord22.fill(0);
    g_scntWord24.fill(0);
    g_scntWord26.fill(0);
    g_cpctPreviousOperand = 0;
    g_cpctSleepPromptReady = true;
    g_cpctInputUpdatePending = false;
    g_scenSequenceObjectsInitialized = false;
    g_scenIndexedRecords.clear();
}

void FinalizeScenRuntimeObject(ScenRuntimeObject *object) {
    if (!object)
        return;
    if ((object->runtimeFlags & kRuntimeFlagInteractionResume) != 0) {
        object->runtimeFlags &= ~kRuntimeFlagInteractionResume;
        SetScenActiveObjectName(0);
        SetScenPreviousObjectName(0);
        object->runtimeFlags &= ~kRuntimeFlagConversationCameraActive;
        if ((ScenFlags() & kScenarioFlagPadBlockOverride) != 0) {
            ScenFlags() &= ~kScenarioFlagPadBlockOverride;
            PCCTL_ResetAuthAndInputStates();
        }
        ClearScenAuthorGameState();
    }
    if ((ScenFlags() & kScenarioFlagOneShotPrompt) != 0) {
        ScenFlags() &= ~kScenarioFlagOneShotPrompt;
        (void)UI_ICONRequestNotification(0x100U, 0, 0, 2);
    }
    object->runtimeFlags &= ~kRuntimeFlagPendingFailure2;
    object->runtimeFlags |= kRuntimeFlagExitLatched;
    if (object->runtimeState == 7) {
        object->runtimeFlags &= ~0x10100U;
        object->runtimeState = 1;
        object->savedInstructionPtr = nullptr;
    }
    object->runtimeFlags &= ~kRuntimeFlagPendingFailure1;
    if (g_scenarioFunctionWork && g_scenarioFunctionWork->scnfRelationHandle >= 0) {
        std::array<std::byte, 8> event{};
        while (DequeueScenarioRelationData(
                   static_cast<std::uint32_t>(g_scenarioFunctionWork->scnfRelationHandle), event.data(), static_cast<std::int32_t>(event.size())) != 0) {
        }
    }
}

void UpdateScenActiveRuntimeObject() {
    ScenFlags() &= ~(kScenarioFlagActiveRuntimeObject | kScenarioFlagUnknown200 | kScenarioFlagInteractionModal);
    ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    const std::uint32_t flags = ScenFlags();
    SetScenActiveObjectName(0);
    if (!active)
        return;

    g_scenRuntimeState.activeObject = nullptr;
    if (!ShouldRun() && (flags & kScenarioFlagPadBlockOverride) != 0) {
        ScenFlags() &= ~kScenarioFlagPadBlockOverride;
        PCCTL_ResetAuthAndInputStates();
    }
    RestoreScenPadContext();
    FinalizeScenRuntimeObject(active);
    active->name.name = 0;
}

bool ScenRuntimeObjectBusy() {
    const ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    return active && (static_cast<std::uint8_t>(active->runtimeState - 1U) > 2U || (active->runtimeFlags & 0xe0000U) != 0);
}

void UpdateScenTaskTimer() {
    g_scenarioFunctionWork->active = 1;
    if (g_scenarioFunctionWork->pendingDeltaReset) {
        const std::int32_t elapsed = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
        g_scenarioTaskDeltaTime = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(elapsed) - static_cast<std::uint32_t>(g_scenarioTaskStartTime));
        g_scenarioFunctionWork->pendingDeltaReset = 0;
    }
}

void SetScenRuntimeObjectRange(std::uint32_t name, std::uint32_t mode, std::uint32_t center, std::uint32_t radius) {
    ScenRuntimeObject *const object = FindScenRuntimeObject(name);
    if (!object)
        return;
    object->rangeMode = static_cast<std::uint8_t>(mode);
    if ((mode == 1 || mode == 2) && static_cast<std::uint16_t>(radius) < 0x8000U) {
        const std::uint16_t midpoint = static_cast<std::uint16_t>(center);
        const std::uint16_t width = static_cast<std::uint16_t>(radius);
        object->rangeMin = static_cast<std::int16_t>(midpoint - width);
        object->rangeMax = static_cast<std::int16_t>(midpoint + width);
    }
}

std::uint32_t QueryScenRuntimeObject(ScenarioContext *context, std::uint32_t name, std::uint32_t query, std::uint32_t auxiliary) {
    ScenRuntimeObject *const object = name ? FindScenRuntimeObject(name) : g_scenRuntimeState.activeObject;
    if (!object)
        return 0;
    switch (query) {
    case 0:
        if ((object->runtimeFlags & 0x40U) == 0 || !object->activeLineOrMarker)
            return 0;
        return static_cast<const std::uint32_t *>(object->activeLineOrMarker)[2];
    case 3:
        return object->runtimeState == 5 ? static_cast<std::uint32_t>(static_cast<std::int32_t>(object->runtimeStateValue)) : 0;
    case 4:
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(object->field28));
    case 5:
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(object->runtimeStateValue));
    case 6:
        return static_cast<std::uint32_t>(
            reinterpret_cast<std::uintptr_t>(object->locations) + static_cast<std::int64_t>(static_cast<std::int32_t>(auxiliary)) * sizeof(Vector3));
    case 7:
    case 9:
        if (object->kvPairs) {
            for (std::int32_t index = 0; index < object->kvPairCount; ++index) {
                if (object->kvPairs[index].key == static_cast<std::int32_t>(auxiliary))
                    return query == 9 ? 1U : static_cast<std::uint32_t>(object->kvPairs[index].value);
            }
        }
        return query == 7 ? 0xffffffffU : 0U;
    case 8:
        if ((object->runtimeFlags & 0x40U) == 0)
            return 0;
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(g_scenRuntimeState.referencedCharacterIds.data()));
    default:
        return 0;
    }
}

std::uint32_t StoreScenRuntimeObjectText(std::uint32_t name) {
    const ScenRuntimeObject *const object = FindScenRuntimeObject(name);
    if (!object) {
        g_scenRuntimeObjectTextScratch[0] = 0;
    } else {
        const std::int32_t count = static_cast<std::int8_t>(object->participantCount);

        if (count >= 0 && count < static_cast<std::int32_t>(g_scenRuntimeObjectTextScratch.size()) && (!count || object->participants)) {
            if (count)
                std::memcpy(g_scenRuntimeObjectTextScratch.data(), object->participants, static_cast<std::size_t>(count) * sizeof(std::uint32_t));
            g_scenRuntimeObjectTextScratch[count] = 0;
        }
    }
    return HLib_GetTaskDataOffset(g_scenRuntimeObjectTextScratch.data());
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

bool SCEN_TryFinalizeReadyRuntimeObject() noexcept {
    if (SCEN_IsRuntimeObjectBusy())
        return false;
    UpdateScenActiveRuntimeObject();
    return true;
}

void SCEN_FinalizeReadyPlayerControlRuntimeObjects() noexcept {
    if (g_cpctInputUpdatePending && g_cpctPreviousOperand && SCEN_TryFinalizeReadyRuntimeObject()) {
        g_cpctInputUpdatePending = false;
    }
}

Character *SCEN_FindCharacterByFunctionId(ScenarioContext *context, std::uint32_t identifier) {
    if (!context || !context->task || !context->task->task || !context->task->task->task_data_ptr || !identifier) {
        return nullptr;
    }
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(context->task->task->task_data_ptr);
    CharacterIdLookupNode *node = scenario->characterIdRoot;
    while (node) {
        if (node->id == identifier)
            return node->character;
        node = static_cast<std::int32_t>(identifier) > static_cast<std::int32_t>(node->id) ? node->left : node->right;
    }
    return nullptr;
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

void *FindPackedAuthorTrackByIndex(PackSlot *slot, std::int32_t requestedIndex) {
    if (!slot || !slot->pakData || requestedIndex < 0)
        return nullptr;
    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(slot->pakData) + slot->pakData->directoryOffset);
    auto *const entries = reinterpret_cast<PakDirectoryEntry *>(reinterpret_cast<std::byte *>(directory) + directory->entriesOffset);
    for (std::int32_t entryIndex = 0; entryIndex < directory->entryCount; ++entryIndex) {
        const PakDirectoryEntry &entry = entries[entryIndex];
        if (entry.tag != SHEN_NAME(AUTH))
            continue;
        if (requestedIndex-- == 0) {
            return reinterpret_cast<std::byte *>(directory) + static_cast<std::int32_t>(entry.dataOffset);
        }
    }
    return nullptr;
}

bool AuthorTrackBlockBelongsToPack(const void *block) {
    for (const PackSlot &slot : g_packSlots) {
        if (!slot.tag || !slot.pakData || (g_packTaskData && &slot == g_packTaskData->loadingSlot))
            continue;
        const std::byte *const directoryBytes = reinterpret_cast<const std::byte *>(slot.pakData) + slot.pakData->directoryOffset;
        const PakDirectory *const directory = reinterpret_cast<const PakDirectory *>(directoryBytes);
        const PakDirectoryEntry *const entries = reinterpret_cast<const PakDirectoryEntry *>(directoryBytes + directory->entriesOffset);
        for (std::int32_t index = 0; index < directory->entryCount; ++index) {
            if (directoryBytes + static_cast<std::int32_t>(entries[index].dataOffset) == block)
                return true;
        }
    }
    return false;
}

void FreeAuthorSequenceTrack(std::int32_t index) {
    ScenarioTrackSlot &slot = g_SCNCTrackSlots[static_cast<std::size_t>(index)];
    if (!slot.loaded)
        return;
    if (g_pSCNC && g_pSCNC->header.currentTrackIndex == index && g_pSCNC->header.active == 1) {
        return;
    }
    if (slot.fileBlock && !AuthorTrackBlockBelongsToPack(slot.fileBlock)) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(slot.fileBlock), 0);
    }
    slot = {};
}

void LoadAuthorSequenceTrack(std::int32_t index, const char *identifier, const char *packageName) {
    ScenarioTrackSlot &slot = g_SCNCTrackSlots[static_cast<std::size_t>(index)];
    if (slot.track) {
        if (g_pSCNC && g_pSCNC->header.active == 1) {
            SEQ_EndActiveScenarioSubtitles();
            ASEQ_ApplyCameraKeyframes(g_pSCNC);
        }
        FreeAuthorSequenceTrack(index);
    }

    char directory[64];
    std::sprintf(directory, "%s", packageName);
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    void *block = nullptr;
    if (pack && pack->usePackCache) {
        if (packageName && *packageName) {
            block = LoadFileIntoHeapMem(PACK_FindSlotByName(packageName), identifier, SHEN_NAME(AUTH));
        } else {
            block = FindPackedAuthorTrackByIndex(PACK_FindSlotByName(identifier), index);
        }
    } else {
        block = ReadFile(directory, identifier, static_cast<std::int32_t>(SHEN_NAME(SCIF)));
    }

    slot.fileBlock = block;
    slot.track = block;
    slot.loaded = 1;
    slot.enabled = 0;
    const auto *const bytes = static_cast<const std::byte *>(block);
    std::uint32_t trackSize = 0;
    std::memcpy(&trackSize, bytes + 4, sizeof(trackSize));
    std::uint32_t followingToken = 0;
    std::memcpy(&followingToken, bytes + trackSize, sizeof(followingToken));
    if (followingToken == SHEN_NAME(MLWD))
        slot.movementPayload = const_cast<std::byte *>(bytes + 8 + trackSize);
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

void SCEN_CreateCharactersForSequence() {
    CreateScenSequenceCharacters();
}

bool SCEN_IsRuntimeObjectBusy() noexcept {
    return ScenRuntimeObjectBusy();
}


bool SCEN_HasActiveRuntimeObject() noexcept {
    return g_scenRuntimeState.activeObject != nullptr;
}

std::uint32_t SCEN_GetActiveRuntimeObjectFlags() noexcept {
    return g_scenRuntimeState.activeObject ? g_scenRuntimeState.activeObject->runtimeFlags : 0U;
}

std::uint32_t SCEN_GetActiveRuntimeObjectName() noexcept {
    return g_scenRuntimeState.activeObjectName;
}

bool SCEN_HasRuntimeObject(std::uint32_t characterName) noexcept {
    return FindScenRuntimeObject(characterName) != nullptr;
}

bool SCEN_LoadCurrentMapScript(ScenarioWindow *window) {
    if (!window)
        return false;

    while (window->stacks) {
        ScenarioStack *const stack = window->stacks;
        window->stacks = *reinterpret_cast<ScenarioStack **>(stack->frame + 4);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stack), 0);
    }
    if (g_scenOwnedRecordStorage.scenarioFile) {
        HLib_FreeTaskData(g_scenOwnedRecordStorage.scenarioFile, 0);
        g_scenOwnedRecordStorage.scenarioFile = nullptr;
    }
    ResetScenMapEpochRecords();

    char filename[32]{};
    std::snprintf(filename, sizeof(filename), "act%02d_%c%c%c%c.scn", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8U),
        static_cast<char>(g_areaId >> 16U), static_cast<char>(g_areaId >> 24U));

    auto &file = g_scenOwnedRecordStorage.scenarioFile;
    file = static_cast<HLib::TaskData *>(ReadFile("scenario", filename, static_cast<std::int32_t>(SHEN_NAME(INFO))));
    const auto *const info = reinterpret_cast<const ScenarioHeader *>(file);
    const auto offsetWithinFile = [info](std::int32_t offset) {
        return info && offset >= static_cast<std::int32_t>(sizeof(ScenarioHeader)) && offset < info->wholeSize;
    };
    if (!info || info->wholeSize < static_cast<std::int32_t>(sizeof(ScenarioHeader)) || !offsetWithinFile(info->functionOffset) ||
        !offsetWithinFile(info->dataSegmentOffset) || !offsetWithinFile(info->entrypoint)) {
        if (file)
            HLib_FreeTaskData(file, 0);
        file = nullptr;
        window->info = nullptr;
        window->activeInfo = nullptr;
        window->functionInfo = nullptr;
        window->dataSegment = nullptr;
        window->entrypoint = nullptr;
        return false;
    }
    Startup_EventBase(window, reinterpret_cast<ScenarioHeader *>(file));
    return true;
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

const std::byte *FindMapInfoChunkNative(const void *mapInfo, std::uint32_t token) {
    return MAPINFO_FindChunkIn(mapInfo, g_mapInfoSize, token);
}

bool TrimMapInfoAllocationNative(void *allocation, std::uint64_t requestedSize) {
    if (!allocation)
        return false;
    if (requestedSize <= 32)
        return true;

    constexpr std::int32_t kUse = static_cast<std::int32_t>(make_shen_name('U', 'S', 'E', ' '));
    const std::uint64_t blockSize = (requestedSize + sizeof(HLib::TaskData) + 31U) & ~std::uint64_t{31U};
    auto *const header = reinterpret_cast<HLib::TaskData *>(allocation) - 1;
    if (header->status != kUse || static_cast<std::uint64_t>(header->content_size) < blockSize) {
        return false;
    }
    if (static_cast<std::uint64_t>(header->content_size) <= blockSize + 64U)
        return true;

    HLib::TaskData *next = header->next_ptr;
    auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(header) + blockSize);
    std::uint64_t remainderSize = static_cast<std::uint64_t>(header->content_size) - blockSize;
    remainder->taskToken = HLIB_FREE_TOKEN.name;
    remainder->status = HLIB_FREE_TOKEN.name;
    remainder->prev_ptr = header;
    remainder->next_ptr = next;
    header->next_ptr = remainder;
    header->content_size = static_cast<std::int64_t>(blockSize);
    if (next->status != kUse) {
        remainderSize += static_cast<std::uint64_t>(next->content_size);
        next = next->next_ptr;
        remainder->next_ptr = next;
    }
    next->prev_ptr = remainder;
    remainder->content_size = static_cast<std::int64_t>(remainderSize);
    return true;
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

void SCN3_Process(const void *mapInfo) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr);
    g_scenSearchCharacters.fill(0);

    if (const std::byte *const trid = FindMapInfoChunkNative(mapInfo, SHEN_NAME(DIRT))) {
        std::int32_t direction = 0;
        std::memcpy(&direction, trid + 8, sizeof(direction));
        control->direction = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(direction << 13) / 45.0f));
    }

    EFFE_ResetSceneStateNative();

    if (g_pSCNC)
        g_pSCNC->header.active = 0;

    ASEQ_InitCameraTrackPointers(FindMapInfoChunkNative(mapInfo, SHEN_NAME(SCEX)));

    g_scenarioFunctionWork = scenario->scenarioFunctions;
    g_scenarioFunctionWork->scenarioChunk = reinterpret_cast<ScenarioHeader *>(const_cast<std::byte *>(FindMapInfoChunkNative(mapInfo, SHEN_NAME(SCNF))));
    g_scenarioFunctionWork->active = 0;

    SCEN_LoadCurrentMapScript(scenario->scenarioWindow);

    if (const std::byte *const dger = FindMapInfoChunkNative(mapInfo, SHEN_NAME(REGD))) {
        const auto size = static_cast<std::uint64_t>(dger - static_cast<const std::byte *>(mapInfo)) + 8U;
        TrimMapInfoAllocationNative(const_cast<void *>(mapInfo), size);

        const std::uint32_t terminator[2]{make_shen_name('E', 'N', 'D', '\0'), 0};
        std::memcpy(const_cast<std::byte *>(dger), terminator, sizeof(terminator));
    }
}

} // namespace shenmue::s1
