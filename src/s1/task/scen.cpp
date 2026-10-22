#include "s1/task/scen_state.hpp"

namespace shenmue::s1 {

using namespace scen_detail;

extern std::uint8_t g_disableClockAndActionSelector;

std::uint32_t GetDebugLogFlag();

extern float g_newCycleTime;

extern float g_searchTargetDistance;

extern std::uint8_t g_timeValue;

extern std::uint8_t g_shouldTimeAdvance;

extern std::uint8_t g_globalFlags[1];

std::int32_t g_bGlobalFlag = 0;

std::array<float, 2> g_debugTextPosition{};

std::string g_debugText;

extern std::int32_t g_saveScreenFlag;

extern std::uint32_t g_searchLockOnFlags;

extern std::int32_t g_searchAnyInputPressedLatched;

extern std::int32_t g_searchAPressedLatched;

extern std::int32_t g_currentSearchState;

extern std::int32_t g_searchCandidateId;

extern std::uint32_t g_searchTemporaryFocusedCharacter;

extern std::int32_t g_searchFocusedItemState;

void RecalculateDayInTheWeek(GameDateTime *dateTime);

std::int32_t ExecuteAICACommand_Wrapper(std::uint32_t volume);

void ReleaseEnvironmentAicaBank(std::uint32_t bank);

extern std::int32_t g_actNumber;

std::array<std::uint8_t, 128> g_shenmueFlags{};


bool GetFlag(std::uint32_t flag) {
    return flag < 1024U && (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0;
}

ScenarioStepTable g_scenarioStepLut{};

std::uint32_t g_scenarioRuntimeFlags = 0;

std::uint32_t g_scenHelpPromptOverride = 0;

std::int32_t g_pendingSleepResumeLoad = 0;

Vector3 g_ryoHomeSleepPosition{};

std::uint16_t g_ryoHomeSleepYaw = 0;

std::uint8_t g_temporaryRyoEventFlags = 0;

std::int16_t g_defaultKaidanId = 0;

std::int32_t g_temporaryKaidanId = -1;

std::int32_t g_playerKaidanGuideActive = 0;

std::array<std::int32_t, 256> g_globalSceneSlotValues{};

float g_animatedSequencePlayDistance = 99999.0f;

bool g_automatedSequenceSkip = false;

std::uint8_t g_shouldSleep = 0;

std::uint8_t g_sleepSaveTrainingReadiness = 4;

std::int32_t g_sleepPromptWindowStartMinutes = 0;

std::int32_t g_sleepPromptWindowEndMinutes = 0;

std::int32_t g_forcedSleepWindowStartMinutes = 0;

std::int32_t g_forcedSleepWindowEndMinutes = 0;

shen_name g_homeAreaId{};

std::int32_t g_homeSceneId = 0;

std::uint32_t g_requestedEntryPoint = 0;

ScenarioHeader *g_pSCN_INFO_TaskData = nullptr;

void SRCH_Reset();

std::array<std::uint8_t, 16> g_ryoHomeSleepAuxBlock{};

std::array<std::uint8_t, 16> g_ryoHomeSleepAuxBlock1{};

std::uint16_t g_ryoHomeSleepGuideWord = 0;

std::uint8_t DetermineActNum() noexcept {
    std::uint32_t areaId = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        std::uint32_t character = (g_areaId >> shift) & 0xffU;
        if (character >= 'a' && character <= 'z')
            character -= 'a' - 'A';
        areaId |= character << shift;
    }

    g_displayActNumber = 0;
    if (areaId == SHEN_NAME(JOMO))
        g_displayActNumber = 1;
    else if (areaId == SHEN_NAME(JHD0))
        g_displayActNumber = 2;
    else if (areaId == SHEN_NAME(YDB1))
        g_displayActNumber = 3;
    return g_displayActNumber;
}

std::int32_t g_scenarioTaskDeltaTime = 0;



float g_scenarioTaskSubtitleTime = 0.0F;

std::uint32_t g_freeTalkInitialized = 0;

std::int16_t g_debug_talk_status = -1;

ScenRuntimeState g_scenRuntimeState{};

ScenarioFunctionWork *g_scenarioFunctionWork = nullptr;

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

std::uint32_t g_generatedCharacterId = 0;

std::int32_t g_currentHelpToken = 0;

std::int16_t g_currentScenarioStep = 0;

template <typename T> T *ScenarioPointer(std::uint32_t offset);

GameDateTime g_scenarioDateScratch{.year = 86, .month = 12, .day = 3, .dayOfWeek = 3, .hours = 8, .minutes = 30, .seconds = 0, .reserved = 0};

std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntSelectedStateOffsets{};

std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntWaitOffsets{};

std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntResumeMarkOffsets{};

std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntReentryOffsets{};

std::array<std::uint32_t, 9> g_scenSearchCharacters{};

constexpr std::uint32_t kScenarioFlagRestorePadContext = 0x2U;

std::array<std::uint32_t, 12> g_scenOwnedSequenceCharacters{};

HLib::Task *g_scenOwnedSequenceTask = nullptr;

bool g_scenSequenceObjectsInitialized = false;

std::int32_t g_scenarioTaskStartTime = 0;

std::vector<std::pair<std::uint32_t, const ScenRuntimeRecord *>> g_scenIndexedRecords;

ScenOwnedRecordStorage g_scenOwnedRecordStorage{};


CVWKRuntimeSlot *g_pAudio = nullptr;


std::int32_t g_scntFrameTimer = 0;


std::int32_t g_scnfCanSkip = 0;


std::int32_t g_scnfDeltaTime = 0;

std::uint32_t &ScenFlags() {
    return g_scenarioFunctionWork ? g_scenarioFunctionWork->flags : g_scenarioRuntimeFlags;
}

void SetScenActiveObjectName(std::uint32_t name) {
    g_scenRuntimeState.activeObjectName = name;
    if (g_scenarioFunctionWork)
        g_scenarioFunctionWork->activeObjectName = name;
}

void SetScenPreviousObjectName(std::uint32_t name) {
    g_scenRuntimeState.previousObjectName = name;
    if (g_scenarioFunctionWork)
        g_scenarioFunctionWork->previousObjectName = name;
}

bool ScenInteractionBlocked() {
    if (!ShouldRun() || g_isPlayerGameStateActive != 7 || g_scenHelpPromptOverride != 0 || g_tmex_val != 0)
        return false;
    const bool hasRawAnalogInput = useAnalogInput != 0 && static_cast<float>(rawAnalogInputValue) / 245.0F >= 0.5F;
    if (hasRawAnalogInput && g_nAuthorState == 0 && g_searchTemporaryFocusedCharacter == 0)
        return false;
    return g_searchCandidateId < 0 &&
        ((((g_currentSearchState - 2) & ~3) != 0) || g_currentSearchState == static_cast<std::int32_t>(SearchState::AutoCharacterLockOn));
}

void RestoreScenPadContext() {
    if (!g_scenarioFunctionWork || (ScenFlags() & kScenarioFlagRestorePadContext) == 0)
        return;
    ScenFlags() &= ~kScenarioFlagRestorePadContext;
    (void)shenUpdateInputsInto(g_scenarioFunctionWork->restorePadEntry);
}



void CaptureScenPadContext() {
    if (!g_scenarioFunctionWork || (ScenFlags() & (kScenarioFlagRestorePadContext | 0x80U)) != 0)
        return;
    const std::uint32_t name = g_scenarioFunctionWork->activeObjectName;
    if (name == g_cpctLastOperand || name == g_cpctPreviousOperand || name == g_scenarioStepLut.pendingCharacterName)
        return;
    ScenFlags() |= kScenarioFlagRestorePadContext;
    g_scenarioFunctionWork->restorePadEntry = shenUpdateInputsInto(g_scenarioFunctionWork->currentPadEntry);
}

void ClearScenAuthorGameState() {
    if ((ScenFlags() & kScenarioFlagAuthorGameState) == 0)
        return;
    if (g_nAuthorState != 0 && g_isPlayerGameStateActive == 7 && g_scenHelpPromptOverride == 0 && g_tmex_val == 0) {
        SetNewAuthorGameState(0);
    }
    CHAR_SetHandFaceDetails(0);
    g_Conv.cueStartCount = 0;
    ScenFlags() &= ~kScenarioFlagAuthorGameState;
}

void IndexScenRuntimeContainer(const ScenRuntimeContainer *container);

void UpdateScenActiveRuntimeObject();

ScenRuntimeObject *FindScenRuntimeObject(std::uint32_t name);

void ResetScenScriptRuntimeState();



void ResetScenScriptRuntimeState() {
    if (ScenRuntimeObject *const active = g_scenRuntimeState.activeObject)
        FinalizeScenRuntimeObject(active);
    if (g_cpctLastOperand == g_scenRuntimeState.activeObjectName)
        g_cpctLastOperand = 0;
    for (ScenTimelineEntry &entry : g_scenRuntimeState.timelineEntries) {
        const std::int32_t elapsed = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
        entry.payload = nullptr;
        entry.command = 0;
        entry.secondaryActor = 0;
        const std::int32_t relative = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_scenarioTaskDeltaTime) - static_cast<std::uint32_t>(elapsed));
        entry.time = static_cast<float>(relative) / 12500000.0F;
    }
    if (g_scenarioFunctionWork && g_scenarioFunctionWork->scnfRelationHandle >= 0) {
        std::array<std::byte, 8> event{};
        while (DequeueScenarioRelationData(
                   static_cast<std::uint32_t>(g_scenarioFunctionWork->scnfRelationHandle), event.data(), static_cast<std::int32_t>(event.size())) != 0) {
        }
    }
    RestoreScenPadContext();
    ClearScenAuthorGameState();
    if ((ScenFlags() & kScenarioFlagPadBlockOverride) != 0) {
        ScenFlags() &= ~kScenarioFlagPadBlockOverride;
        PCCTL_ResetAuthAndInputStates();
    }
    if ((ScenFlags() & kScenarioFlagConversationMarkers) != 0)
        convEndConversationCamera();
    ScenFlags() &= 0xfffdede6U;
    SetScenActiveObjectName(0);
    g_scenRuntimeState.activeObject = nullptr;
    g_scenarioTaskDeltaTime = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
}

const std::byte *ScenScriptEnd(const ScenRuntimeObject &object);

void QueueScenConfirmPrompt();

void ClearScenConfirmPrompt();

bool UpdateScenPendingRuntimeEvent(ScenRuntimeObject &object);

void BeginScenInteraction(ScenRuntimeObject &object);

bool UpdateScenStateFiveRecovery(ScenRuntimeObject &object, const PadCaptureEntry *pad);

bool RunScenBytecode(ScenRuntimeObject &object);

std::byte *ScanScenConditionalFlow(ScenRuntimeObject &object, std::byte *ip);

bool ScenCanRead(const ScenRuntimeObject &object, const std::byte *pointer, std::size_t size);

std::byte *ScenAddRelative(ScenRuntimeObject &object, std::byte *base, std::int32_t delta);

std::int32_t ScenSignExtend(std::uint32_t value, std::uint32_t signBit);

std::size_t ScenWordSlotIndex(const ScenRuntimeObject &object);

std::byte *BuildScenConversationMarkers(ScenRuntimeObject &object, std::byte *ip);

void scnfGenerateLipCues(const ScenRuntimeObject &object);

bool SCNT_IsLineVoicePlaying();

void IncrementInventoryItemCountAndTrackStep(std::uint16_t itemId);

void SCEN_ParseRecursive_Impl() {
    ScenFlags() &= ~kScenarioFlagCoreRanThisFrame;
    UpdateScenTaskData();
    const std::uint32_t flags = ScenFlags();
    if ((flags & kScenarioFlagActiveRuntimeObject) != 0) {
        if (ScenRuntimeObject *const active = g_scenRuntimeState.activeObject) {
            SCNT_Core(active);
            ScenRuntimeObject *const current = g_scenRuntimeState.activeObject;
            if (!current || (static_cast<std::uint8_t>(current->runtimeState - 1U) <= 2U && (current->runtimeFlags & 0xe0000U) == 0)) {
                ScenFlags() &= 0xfffdfdfeU;
                SetScenActiveObjectName(0);
                g_scenRuntimeState.activeObject = nullptr;
            }
        }
        return;
    }

    bool cvwkActive = false;
    if (g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0) {
                cvwkActive = true;
                break;
            }
        }
    }
    const bool searchAllowsSelection = g_searchCandidateId < 0 &&
        (((g_currentSearchState - 2) & ~3) != 0 || g_currentSearchState == static_cast<std::int32_t>(SearchState::AutoCharacterLockOn) ||
            g_searchCandidateId >= 0);
    ScenRuntimeObject *const activeBeforeSelection = g_scenRuntimeState.activeObject;
    const bool activeAllowsSelection = !activeBeforeSelection || (activeBeforeSelection->runtimeFlags & 0xe0000U) == 0;
    if (!cvwkActive && (flags & kScenarioFlagInteractionModal) == 0 && searchAllowsSelection && activeAllowsSelection) {
        ScenFlags() &= ~kScenarioFlagInteractiveFocused;
        std::array<float, 24> distances{};
        std::array<std::int32_t, 24> stateHints{};
        const std::int32_t selected = SCEN_InteractiveObjects(distances, stateHints);
        const std::int32_t count = std::clamp(g_scenRuntimeState.objectCount, 0, 24);
        for (std::int32_t index = 0; index < count; ++index) {
            if (index == selected)
                continue;
            std::int32_t stateHint = stateHints[static_cast<std::size_t>(index)];
            if (stateHint == 1)
                stateHint = 2;
            SCNF_UpdateObjectActivation(&g_scenRuntimeState.objects[static_cast<std::size_t>(index)], stateHint);
        }
        if (selected >= 0 && selected < count) {
            SCNF_UpdateObjectActivation(&g_scenRuntimeState.objects[static_cast<std::size_t>(selected)], stateHints[static_cast<std::size_t>(selected)]);
        }
    }
    const std::uint32_t previous = g_scenRuntimeState.previousObjectName;
    const std::uint32_t active = g_scenRuntimeState.activeObjectName;
    if (active == previous && previous && g_scenRuntimeState.activeObject) {
        SCNT_Core(g_scenRuntimeState.activeObject);
    } else {
        SetScenPreviousObjectName(active);
        g_scntWaitOffsets[0] = 0;
    }
}

std::array<std::uint32_t, 17> g_scenRuntimeObjectTextScratch{};

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

static_assert(sizeof(ScenarioRelationQueue) == 0x88);

ScenarioRelationQueue *g_scenarioRelationQueues = nullptr;

std::int32_t AcquireScenarioRelationQueue(std::uint32_t tag) {
    if (!g_scenarioRelationQueues)
        return -1;
    for (std::int32_t index = 0; index < 2; ++index) {
        if (g_scenarioRelationQueues[index].tag == tag)
            return index;
    }
    for (std::int32_t index = 0; index < 2; ++index) {
        ScenarioRelationQueue &queue = g_scenarioRelationQueues[index];
        if (queue.tag)
            continue;
        queue.tag = tag;
        queue.count = 0;
        queue.read = queue.buffer.data();
        queue.write = queue.buffer.data();
        std::uint32_t empty = 0;
        std::memcpy(queue.buffer.data(), &empty, sizeof(empty));
        return index;
    }
    return -1;
}

std::int32_t EnqueueScenarioRelationData(std::uint32_t handle, const void *source, std::int32_t size) {
    if (!g_scenarioRelationQueues || handle > 1 || !source || size < 0)
        return -1;
    ScenarioRelationQueue &queue = g_scenarioRelationQueues[handle];
    if (!queue.write || !queue.read)
        return -1;

    std::int32_t previousSize = 0;
    std::memcpy(&previousSize, queue.write, sizeof(previousSize));
    if (previousSize) {
        queue.write += 4 + ((static_cast<std::size_t>(previousSize) + 3U) & ~3U);
    }
    if (queue.write + size + 4 > queue.buffer.data() + 108) {
        const std::int32_t wrap = 0x88;
        std::memcpy(queue.write, &wrap, sizeof(wrap));
        queue.write = queue.buffer.data();
    }
    if (queue.write < queue.read && queue.write + size + 4 > queue.read) {
        return -1;
    }
    ++queue.count;
    std::memcpy(queue.write, &size, sizeof(size));
    std::memcpy(queue.write + 4, source, static_cast<std::size_t>(size));
    return 0;
}

std::int32_t DequeueScenarioRelationData(std::uint32_t handle, void *destination, std::int32_t capacity) {
    if (!g_scenarioRelationQueues || handle > 1)
        return -1;
    ScenarioRelationQueue &queue = g_scenarioRelationQueues[handle];
    if (!queue.count)
        return 0;
    if (!queue.read || !destination)
        return -1;

    std::int32_t size = 0;
    std::memcpy(&size, queue.read, sizeof(size));
    if (size < 0 || capacity < size)
        return -1;
    --queue.count;
    std::memcpy(destination, queue.read + 4, static_cast<std::size_t>(size));
    if (queue.read == queue.write) {
        const std::uint32_t empty = 0;
        std::memcpy(queue.read, &empty, sizeof(empty));
        return size;
    }
    queue.read += 4 + ((static_cast<std::size_t>(size) + 3U) & ~3U);
    const std::byte *const end = reinterpret_cast<const std::byte *>(&queue + 1);
    if (queue.read >= end)
        queue.read = queue.buffer.data();
    std::int32_t nextSize = 0;
    std::memcpy(&nextSize, queue.read, sizeof(nextSize));
    if (queue.read + nextSize + 4 > end)
        queue.read = queue.buffer.data();
    return size;
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

static_assert(sizeof(AddCharacterHandlerParameters) == 0x0c);

static_assert(sizeof(CharacterBoundsParameters) == 0x10);

static_assert(sizeof(TargetAngleRangeParameters) == 0x14);

static_assert(sizeof(FixedObjectAttachmentOffsetParameters) == 0x10);

static_assert(sizeof(EventCameraParameters) == 0x0c);

static_assert(sizeof(SequencePlayRootParameters) == 0x08);

static_assert(sizeof(HomeAreaParameters) == 0x1c);

static_assert(sizeof(CycleManager::UtilityParameters) == 0x20);

static_assert(sizeof(GlobalSceneSlotValueParameters) == 0x08);

static_assert(sizeof(MapEventUtilsParameters) == 0x14);

static_assert(sizeof(AssignCharacterEventParameters) == 0x0c);

static_assert(sizeof(CharacterControlValueParameters) == 0x08);

static_assert(sizeof(PlayCharacterMotionParameters) == 0x18);

static_assert(sizeof(RotateNodeParameters) == 0x0c);

static_assert(sizeof(CharacterInteractionLimitsParameters) == 0x10);

static_assert(sizeof(InputDataParameters) == 0x08);

static_assert(offsetof(InventoryRemoveParameters, isCassette) == 8);

static_assert(sizeof(OwnedItemMenuParameters) == 0x0c);

static_assert(sizeof(InventoryChangeParameters) == 0x10);

static_assert(sizeof(InventoryQueryParameters) == 0x08);

static_assert(sizeof(MotionPlaybackRateParameters) == 0x08);

static_assert(sizeof(FloatUtilityParameters) == 0x0c);

static_assert(sizeof(LineWalkParameters) == 0x1c);

static_assert(sizeof(HandNamedParameters) == 0x10);

static_assert(sizeof(HandMotionResetParameters) == 0x10);

static_assert(sizeof(HandMotionSetParameters) == 0x24);

static_assert(sizeof(HandFloatsParameters) == 0x0e);

static_assert(sizeof(HandControlParameters) == 0x14);

static_assert(sizeof(HandRootRotationParameters) == 0x10);

static_assert(sizeof(OsageFlagsParameters) == 8);

static_assert(sizeof(AicaCommandParameters) == 8);

static_assert(sizeof(DirectAicaCommandParameters) == 0x0c);

static_assert(sizeof(MotnTargetParameters) == 0x10);

static_assert(sizeof(LoadSoundBankParameters) == 0x0c);

static_assert(sizeof(CameraShakeParameters) == 0x0c);

static_assert(sizeof(AmbientSoundParameters) == 0x20);

static_assert(sizeof(AmbientSoundCommandParameters) == 8);

static_assert(sizeof(ScenarioRuntimeUtilityParameters) == 0x14);

static_assert(sizeof(MotionTransformParameters) == 0x18);

static_assert(sizeof(MotionScaleParameters) == 0x0c);

static_assert(sizeof(LookPointParameters) == 0x10);

static_assert(sizeof(RyoSleepParameters) == 0x10);

static_assert(sizeof(MotStepParameters) == 0x8);

static_assert(sizeof(FixedObjectAttachParameters) == 0x14);

shen_name NextGeneratedCharacterId(HLib::Task *scenarioTask) {
    for (;;) {
        const std::uint32_t index = g_generatedCharacterId;
        g_generatedCharacterId = g_generatedCharacterId + 1U < 1000U ? g_generatedCharacterId + 1U : 0U;
        const shen_name id{
            make_shen_name(' ', static_cast<char>('0' + index / 100U), static_cast<char>('0' + index / 10U % 10U), static_cast<char>('0' + index % 10U))};
        if (!CHID_Find(scenarioTask, id.name))
            return id;
    }
}

Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t identifier);

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

void charaGetPosForNode(Character *character, std::int32_t nodeId, Vector3 *output) {
    if (!character) {
        *output = {};
        return;
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const row = manager ? MOTN_FindNode(static_cast<MotionNodeRuntime *>(manager->motion.previousNodes), nodeId) : nullptr;
    if (!row) {
        *output = character->entity.node.position;
        return;
    }
    *output = {row->transform->rows[3].x, row->transform->rows[3].y, row->transform->rows[3].z};
    if (character->entity.node.child)
        *output = WorldToEntityLocalPoint(*character->entity.node.child, *output);
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

std::int32_t ApplyScriptEulerFlags(Character *character, std::int32_t signedFlags, const Vector3i *source) {
    constexpr std::uint32_t kRecursive = 0x40000000U;
    constexpr std::uint32_t kRotationX = 0x20000000U;
    constexpr std::uint32_t kRotationY = 0x10000000U;
    constexpr std::uint32_t kRotationZ = 0x08000000U;
    constexpr std::uint32_t kTurnAnimation = 0x04200000U;

    const auto flags = static_cast<std::uint32_t>(signedFlags);
    const bool recursive = (flags & kRecursive) != 0;
    Vector3i rotation{};
    if (character) {
        rotation = recursive ? GetEulerAnglesForEntity(character->entity.node) : character->entity.node.rotation;
    }
    const bool relative = signedFlags < 0;
    const auto apply = [relative](std::int32_t current, std::int32_t value) {
        return relative ? static_cast<std::int32_t>(static_cast<std::uint32_t>(current) + static_cast<std::uint32_t>(value)) : value;
    };
    if ((flags & kRotationX) != 0)
        rotation.x = apply(rotation.x, source->x);
    if ((flags & kRotationY) != 0)
        rotation.y = apply(rotation.y, source->y);
    if ((flags & kRotationZ) != 0)
        rotation.z = apply(rotation.z, source->z);

    if ((flags & kTurnAnimation) != 0) {
        (void)ryoPickTurnAnimByYawDelta(character, rotation.y);
        return source->y;
    }
    if (!character)
        return 0;
    if (recursive)
        ApplyEulerAnglesToNode(character->entity.node, rotation);
    else
        character->entity.node.rotation = rotation;
    if (MotionManagerWork *const motion = MOTM_GetWork(character))
        motion->motion.entityEuler = rotation;
    return 0;
}

HandModelWork *FindScenarioHand(ScenarioContext *context, shen_name characterId, std::int32_t side) {
    if (!context || !characterId.name)
        return nullptr;
    Character *const character = CHID_Find(context->task->task, characterId.name);
    return character ? HND_GetWork(character, side == 0) : nullptr;
}

static_assert(sizeof(ScnfUiUtilsParameters) == 0x18);

static_assert(offsetof(ScnfUiUtilsParameters, expectedMask) == 0x00);

static_assert(offsetof(ScnfUiUtilsParameters, timeoutFrames) == 0x04);

static_assert(offsetof(ScnfUiUtilsParameters, promptMode) == 0x08);

static_assert(offsetof(ScnfUiUtilsParameters, bonusFrames) == 0x0c);

static_assert(offsetof(ScnfUiUtilsParameters, command) == 0x14);

void DoorPreserveAccumulator(ScenarioContext *, void *) {
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

// keeping a count its readers use.
std::span<const std::uint32_t> SCEN_ReferencedCharacterIds() noexcept {
    return {g_scenRuntimeState.referencedCharacterIds.data(), g_scenRuntimeState.referencedCharacterIds.size()};
}



std::uint32_t scenGetFirstNonRyoChara() noexcept {
    const std::array<std::uint32_t, 32> &ids = g_scenRuntimeState.referencedCharacterIds;
    constexpr std::uint32_t kRyo = SHEN_NAME(AKIR);
    if (!g_scenRuntimeState.activeObject || (ScenFlags() & 0x20000U) == 0)
        return 0;
    for (std::size_t i = 0; i < 0x18U && ids[i]; ++i) {
        if (ids[i] != kRyo)
            continue;
        for (std::size_t j = 0; j < 4U; ++j) {
            if (ids[j] != kRyo)
                return ids[j];
            if (!ids[j + 1])
                return 0;
        }
        return 0;
    }
    return 0;
}

bool SCEN_IsConversationCameraOverrideActive() noexcept {
    return g_Conv.conversationCameraOverrideActive != 0;
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t identifier) {
    return context && context->task ? CHID_Find(context->task->task, identifier) : nullptr;
}

float CameraDistance(const Vector3 &target, const Vector3 &eye) {
    const float x = target.x - eye.x;
    const float y = target.y - eye.y;
    const float z = target.z - eye.z;
    return std::sqrt(x * x + y * y + z * z);
}

float CameraFixedHalfAngleTangent(std::int32_t halfAngle) {
    float radians = static_cast<float>(static_cast<std::uint16_t>(halfAngle));
    radians *= 6.2831855f;
    radians *= 0.000015258789f;
    return std::tan(radians);
}

bool g_scenarioConversationPromptEnabled{true};

Mt5RuntimeNode *FindScenarioModelNode(Mt5RuntimeNode *node, std::uint16_t kind);

ScenarioSelectorState g_scenarioSelector{};

constexpr std::array<float, 5> kFastSelectionCurve{{
    0.0526320003f,
    0.2631579936f,
    0.7368419766f,
    0.9473680258f,
    1.0f,
}};

constexpr std::array<float, 7> kSlowSelectionCurve{{
    0.0227269996f,
    0.1136360019f,
    0.3181819916f,
    0.6818180084f,
    0.8863639832f,
    0.9772729874f,
    1.0f,
}};

constexpr std::array<std::int32_t, 8> kSelectionSlots{{
    0,
    1,
    2,
    3,
    4,
    5,
    10,
    11,
}};

void SubmitScenarioMove(ScenarioContext *context, void *rawParams, bool payload, bool playbackRate, std::uint32_t mode) {
    const ScriptArguments arguments(context, rawParams);
    Character *const character = ScenarioCharacter(context, arguments.word(0));
    if (!character)
        return;
    const auto *const locomotionPayload = payload ? arguments.pointer<std::uint32_t>(4) : nullptr;
    const float rate = playbackRate ? arguments.scalar(payload ? 5 : 4) : -1.0f;
    charaSubmitMoveRequest(character, arguments.pointer<Vector3>(1), arguments.signedWord(2), arguments.signedWord(3), locomotionPayload, rate, mode);
}

Mt5RuntimeNode *FindScenarioModelNode(Mt5RuntimeNode *node, std::uint16_t kind) {
    while (node) {
        if (node->nodeKind == kind)
            return node;
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->childOffset) {
            if (Mt5RuntimeNode *const child = FindScenarioModelNode(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), kind)) {
                return child;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    }
    return nullptr;
}

static_assert(sizeof(TimedTransitionParameters) == 0x1c);

std::array<TimedTransitionEntry, 8> g_scenarioTimedTransitions{};

std::uint8_t g_scenarioTimedTransitionCount{};

TimedTransitionEntry *g_activeTimedTransition{};

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

std::uint8_t g_timedTransitionUpcoming{0xff};

GameDateTime g_timedTransitionUpcomingTime{};

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

bool g_scenarioTimedTransitionEnabled{true};

constexpr std::array<std::size_t, 55> kNativeAccumulatorPreservingScriptSlots{{
#define SCENARIO_PRESERVE_SLOT(index) index,
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_PRESERVE_SLOT
}};

constexpr std::array<std::size_t, 6> kNativeZeroScriptSlots{{
#define SCENARIO_ZERO_SLOT(index) index,
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_ZERO_SLOT
}};

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {


void SCEN_ParseRecursive() {
    SCEN_ParseRecursive_Impl();
}

std::int16_t SCEN_GetCurrentScenarioStep() noexcept {
    return g_currentScenarioStep;
}

void SCEN_SetCurrentScenarioStep(std::int16_t step) noexcept {
    g_currentScenarioStep = step;
}

std::uint8_t SCEN_GetCurrentHelpToken() noexcept {
    return static_cast<std::uint8_t>(g_currentHelpToken);
}

void SCEN_SetHelpToken(std::uint8_t token, std::uint8_t button) noexcept {
    g_currentHelpToken = static_cast<std::int8_t>(token) < 68 ? token : 0;
    UI_HELPDispatch(0, token, button);
}

std::array<LegacyScenarioFunction, kLegacyScenarioFunctionCount> LegacyFunctions{{
#define SCENARIO_LEGACY_SLOT(index, handler) handler
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_LEGACY_SLOT
}};

std::array<GeneralScenarioFunction, kGeneralScenarioFunctionCount> GeneralPurposeFuncs{{
#define SCENARIO_GENERAL_SLOT(index, handler) handler,
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_GENERAL_SLOT
}};

std::array<ShenmueScenarioFunction, kShenmueScenarioFunctionCount> ShenmueScriptFuncs = [] {
    std::array<ShenmueScenarioFunction, kShenmueScenarioFunctionCount> functions{};
    for (const std::size_t slot : kNativeAccumulatorPreservingScriptSlots)
        functions[slot] = Script_PreserveAccumulator;
    for (const std::size_t slot : kNativeZeroScriptSlots)
        functions[slot] = Script_ClearAccumulator;
#define SCENARIO_SHENMUE_SLOT(index, handler) functions[index] = handler;
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_SHENMUE_SLOT
    return functions;
}();

std::array<SCNFScenarioFunction, kSCNFScenarioFunctionCount> SCNF_Functions = [] {
    std::array<SCNFScenarioFunction, kSCNFScenarioFunctionCount> functions{};
#define SCENARIO_SCNF_SLOT(index, handler) functions[index] = handler;
#define SCENARIO_SCNF_FILL(first, handler) for (std::size_t slot = first; slot < functions.size(); ++slot) functions[slot] = handler;
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_SCNF_SLOT
#undef SCENARIO_SCNF_FILL
    return functions;
}();

std::array<DoorScenarioFunction, kDoorScenarioFunctionCount> DSWKFunctions = [] {
    std::array<DoorScenarioFunction, kDoorScenarioFunctionCount> functions{};
#define SCENARIO_DOOR_SLOT(index, handler) functions[index] = handler;
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_DOOR_SLOT
    return functions;
}();

void SetStrForScnArg(ScenarioContext *context, std::int64_t pointer, std::uint32_t, ScenarioRegisters *) {
    if (!context || !context->stack)
        return;
    auto *const parameters = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(context->stack) + 0x2c);
    parameters[0] = pointer && g_pCurrentTaskData ? static_cast<std::uint32_t>(reinterpret_cast<const std::byte *>(pointer) - g_pCurrentTaskData) : 0;
    parameters[1] = 0xffffffffU;
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(context->stack) + 0x28) = 1;
    context->stack->accumulator = 0;
    if (SCNF_Functions[2])
        SCNF_Functions[2](context, parameters);
}

void ExecuteRemovedScriptFn(std::int32_t index, ScenarioContext *context, std::int64_t, void *) {
    if (index < 0 || static_cast<std::size_t>(index) >= LegacyFunctions.size() || !LegacyFunctions[static_cast<std::size_t>(index)]) {
        return;
    }
    LegacyFunctions[static_cast<std::size_t>(index)](reinterpret_cast<ScenarioRegisters *>(context));
}

std::int64_t ExecuteMemScriptFn(std::int32_t index, void *rawParams) {
    if (index < 0 || static_cast<std::size_t>(index) >= GeneralPurposeFuncs.size() || !GeneralPurposeFuncs[static_cast<std::size_t>(index)] || !rawParams) {
        return 0;
    }
    const auto *const params = static_cast<const ScenarioRegisters *>(rawParams);
    return GeneralPurposeFuncs[static_cast<std::size_t>(index)](params->r0, params->r1, params->r2, params->r3);
}

std::int64_t ExecuteShenmueScriptFn(ScenarioContext *context, std::int32_t index, void *params) {
    if (!context || !context->stack || index < 0 || static_cast<std::size_t>(index) >= ShenmueScriptFuncs.size() ||
        !ShenmueScriptFuncs[static_cast<std::size_t>(index)]) {
        return 0;
    }
    context->stack->accumulator = 0;
    ShenmueScriptFuncs[static_cast<std::size_t>(index)](context, params);
    return context->stack->accumulator;
}

std::uint32_t g_tmpScnfReturnValue = 0;

std::int64_t ExecuteSCNFScriptFn(ScenarioContext *context, std::uint32_t index, std::int64_t stackPointer, std::int32_t argumentCount) {
    if (!context || !context->stack)
        return 0;
    auto *const parameters = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(context->stack) + 0x2c);
    const auto *const source = reinterpret_cast<const std::uint32_t *>(stackPointer);
    for (std::int32_t argument = 0; argument < argumentCount; ++argument)
        parameters[argument] = source[argument];
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(context->stack) + 0x28) = static_cast<std::uint16_t>(index);
    context->stack->accumulator = 0;
    const SCNFScenarioFunction begin = SCNF_Functions[static_cast<std::size_t>(index) * 2U];
    const std::int64_t result = begin ? begin(context, parameters) : 0;
    g_tmpScnfReturnValue = context->stack->accumulator;
    return result;
}

std::int64_t ExecuteDoorSupportFunc(ScenarioContext *context, std::uint32_t index, void *params) {
    if (!context || !context->stack)
        return 0;
    context->stack->accumulator = 0;
    DSWKFunctions[index](context, params);
    return context->stack->accumulator;
}

void SCEN_Callback(HLib::TaskData *taskData) {
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(taskData);
    if (g_bDebugSkipCyclesForCharacters || static_cast<std::uint32_t>(g_NumFramesProcessed - 2) <= 1U)
        return;

    scenParseStackRecursively(scenario->scenarioWindow);
    if (scenario->scenarioFunctions) {
        g_scenarioFunctionWork = scenario->scenarioFunctions;
        if (g_scenarioFunctionWork->active)
            SCEN_ParseRecursive();

        if (g_debug_talk_status >= 0) {
            std::array<char, 32> message{};
            std::snprintf(message.data(), message.size(), "DEBUG TALK STATUS : %d", g_debug_talk_status);
            g_debugTextPosition = {8.0f, 3.0f};
            g_debugText = message.data();
            (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], g_debugText);
        }
    }

    SEQ_ParseFrame(scenario->scenarioConstants);
    SEQ_Cleanup();
    g_uiElements[0x3a].isActive = 1;
}

ScenarioFunctionWork *ASEQ_SCNF_Init(HLib::Task *scenarioTask) {
    auto *const work = static_cast<ScenarioFunctionWork *>(HLib_AllocHeapMemBlock(sizeof(ScenarioFunctionWork), static_cast<std::int32_t>(SHEN_NAME(SCNF))));
    g_scenarioFunctionWork = work;
    std::memset(work, 0, sizeof(*work));
    work->scenarioTask = scenarioTask;
    work->active = 0;
    work->scnaRelationHandle = AcquireScenarioRelationQueue(SHEN_NAME(SCNA));
    work->scnfRelationHandle = AcquireScenarioRelationQueue(SHEN_NAME(SCNF));
    ResetScenRuntimeState();
    g_animatedSequencePlayDistance = 2.0f;
    g_bGlobalFlag = 0;
    g_scenarioStepLut.pendingCharacterName = 0;
    g_notebookJingleSubtitle = nullptr;
    ScenFlags() = 0;
    g_scenHelpPromptOverride = 0;
    g_freeTalkInitialized = 0;

    for (ScenTimelineEntry &entry : g_scenRuntimeState.timelineEntries) {
        const std::int32_t elapsed = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
        entry.payload = nullptr;
        entry.command = 0;
        entry.secondaryActor = 0;
        const std::int32_t relative = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_scenarioTaskDeltaTime) - static_cast<std::uint32_t>(elapsed));
        entry.time = static_cast<float>(relative) / 12500000.0F;
    }
    g_debug_talk_status = -1;
    return work;
}

HLib::Task *SCEN_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(SCEN_Callback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(SCEN)));
    if (!task || task->task_data_ptr) {
        if (task) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
        }
        return nullptr;
    }
    auto *const data = static_cast<ScenarioTaskData *>(HLib_AllocHeapMemBlock(sizeof(ScenarioTaskData), static_cast<std::int32_t>(SHEN_NAME(TASK))));
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }
    HLib_EnableTokenCheck(data);
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);

    data->name = SHEN_NAME(SCEN);
    data->reserved_004 = 0;
    data->characterIdRoot = nullptr;
    data->characterBuckets.fill(nullptr);
    g_scenarioRelationQueues = reinterpret_cast<ScenarioRelationQueue *>(reinterpret_cast<std::byte *>(data) + 0x1a8);
    g_scenarioRelationQueues[1].tag = 0;
    g_scenarioRelationQueues[0].tag = 0;

    data->scenarioWindow = ASEQ_SCNW_Init(task);
    data->scenarioFunctions = ASEQ_SCNF_Init(task);
    data->scenarioConstants = ASEQ_SCNC_Init(task);
    data->voiceWork = CVWK_Initialize();
    const MapControlData *const map = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    SCNT_InitializeScenarioDialogue(*data, map ? map->windTask : nullptr);
    return task;
}

using namespace scen_detail;

using namespace scen_detail;

ScenarioSequence *g_pSCNC = nullptr;

std::array<ScenarioTrackSlot, 70> g_SCNCTrackSlots{};

std::int32_t g_SCNCSelectedTrackIndex = -1;

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

static_assert(sizeof(MotionFunctionData) == 0x11);

static_assert(sizeof(SoundEffectFunctionData) == 0x24);

static_assert(sizeof(VoiceFunctionData) == 0x14);

static_assert(sizeof(EffectFunctionData) == 0x0e);

static_assert(offsetof(EffectFunctionData, channel) == 0x04);

static_assert(offsetof(EffectFunctionData, morph) == 0x08);

static_assert(offsetof(EffectFunctionData, timing) == 0x0c);

static_assert(offsetof(MotionRuntimeState, saved018) == 0x18);

static_assert(offsetof(MotionRuntimeState, savedMotion) == 0x1c);

static_assert(offsetof(MotionRuntimeState, savedInterpolationFlags) == 0x3c);


std::int32_t g_seqRemainingKeyframes = 0;

const float *g_seqFcvKeyframePtr = nullptr;

constexpr std::array<AnimatedSequenceFunction, 7> kAnimatedSequenceFunctions{
    AnimatedSequenceNull,
    ASEQ_ANIMATED_CAM,
    ASEQ_ANIMATED_MOV,
    ASEQ_MOTION,
    ASEQ_EFFECT,
    ASEQ_VOICE,
    ASEQ_SOUND_EFFECT,
};

} // namespace shenmue::s1::scen_detail
