#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

void SCNF_LogStateChange(const ScenRuntimeObject &object, std::int32_t eventCode) {
    if (!g_scenarioFunctionWork || g_scenarioFunctionWork->scnaRelationHandle < 0)
        return;
    const std::array<std::uint32_t, 2> event{
        object.name.name,
        static_cast<std::uint32_t>(eventCode),
    };
    (void)EnqueueScenarioRelationData(
        static_cast<std::uint32_t>(g_scenarioFunctionWork->scnaRelationHandle), event.data(), static_cast<std::int32_t>(sizeof(event)));
}




void SCNF_EnqueueLocationEvent(const ScenRuntimeObject &object, std::uint32_t location) {
    if (!g_scenarioFunctionWork || static_cast<std::uint32_t>(g_scenarioFunctionWork->scnaRelationHandle) > 1)
        return;
    const std::array<std::uint32_t, 3> event{object.name.name, 20, location};
    (void)EnqueueScenarioRelationData(static_cast<std::uint32_t>(g_scenarioFunctionWork->scnaRelationHandle), event.data(), sizeof(event));
}

void SCNF_UpdateObjectActivation(ScenRuntimeObject *object, std::int32_t stateHint) {
    if (!object)
        return;
    ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    if (active && active != object) {
        object->runtimeFlags &= ~0x400U;
        return;
    }
    if (stateHint == 2)
        stateHint = 0;
    if (stateHint < 0)
        return;

    std::uint32_t runtimeFlags = object->runtimeFlags;
    std::int32_t shouldActivate = 1;
    if ((runtimeFlags & kRuntimeFlagStateTransitionLatch) != 0)
        stateHint = 1;
    if ((runtimeFlags & 2U) == 0)
        shouldActivate = stateHint;

    if (shouldActivate == 0) {
        if (object->walkAwayReply) {
            if (ScenInteractionBlocked() && (g_ryoIdleTurnInPlaceCounter > 0 || (g_pControlPadController && g_pControlPadController->numActiveFrames > 0))) {
                ScenFlags() |= kScenarioFlagInteractionModal;
                shouldActivate = 1;
                object->runtimeFlags |= 2U;
                object->savedInstructionPtr = object->instructionPtr;
                object->instructionPtr = object->walkAwayReply;
            }
            object->walkAwayReply = nullptr;
        } else {
            if ((runtimeFlags & kRuntimeFlagStateEntered) != 0) {
                if (g_scenRuntimeState.activeObjectName == object->name.name) {
                    ScenFlags() &= ~kScenarioFlagInteractionModal;
                    SetScenActiveObjectName(0);
                    g_scenRuntimeState.activeObject = nullptr;
                }
                object->runtimeFlags &= ~kRuntimeFlagStateEntered;
                SCNF_LogStateChange(*object, -7);
                FinalizeScenRuntimeObject(object);
            }
            object->runtimeFlags &= ~kRuntimeFlagStateActive;
        }
        if (shouldActivate == 0)
            return;
    }

    ScenRuntimeObject *const current = g_scenRuntimeState.activeObject;
    if (current && g_scenRuntimeState.activeObjectName != object->name.name && (current->runtimeFlags & kRuntimeFlagStateActive) != 0)
        return;
    if ((object->runtimeFlags & kRuntimeFlagStateEntered) == 0) {
        SCNF_LogStateChange(*object, -6);
        object->runtimeFlags |= kRuntimeFlagStateEntered;
    }
    object->runtimeFlags |= kRuntimeFlagStateActive;
    ScenFlags() |= kScenarioFlagInteractiveFocused;
    SetScenActiveObjectName(object->name.name);
    g_scenRuntimeState.activeObject = object;
}

HLib::Task *CurrentScenarioTask() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->scenarioTask;
}

Character *CurrentScenarioRyo() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return static_cast<Character *>(reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->ryo);
}

bool ScenCharacterOnExcludedStairs(std::uint32_t name) {
    constexpr std::array<std::uint32_t, 6> kStairAreas{{SHEN_NAME(D000), SHEN_NAME(JU00), SHEN_NAME(JD00), SHEN_NAME(JHD0), SHEN_NAME(DYKZ), SHEN_NAME(DMAJ)}};
    const std::uint32_t area = CycleManager::CurrentAreaId();
    for (const std::uint32_t stairArea : kStairAreas) {
        if (area == stairArea)
            return KDAN_IsCharacterOccupyingStairs(name);
    }
    return false;
}


bool scenIsOutsideOfScreenRange(const Character *character) {
    if (!character)
        return false;
    const Vector3 world = GetPositionForEntity(const_cast<CharacterNode &>(character->entity.node));
    Matrix4x4_RotateAndPush();
    Vector2 screen{};
    WorldToScreen(world, screen);
    const auto width = static_cast<float>(g_originalScreenWidth);
    return screen.x > 0.0f - width * 0.0625f && width + width * 0.0625f > screen.x;
}

std::int32_t CalculateScenCharacterInRange(ScenRuntimeObject &object, float &distanceSquared, std::uint32_t &angle) {
    distanceSquared = 99999.0F;
    angle = 0xffffU;
    Character *const ryo = CurrentScenarioRyo();
    HLib::Task *const scenarioTask = CurrentScenarioTask();
    if (!ryo || !scenarioTask)
        return -1;

    Character *const target = CHID_Find(scenarioTask, object.name.name);
    if (!target && object.stateByte19 == 0)
        return -1;

    if (g_playerMotion && g_pControlPadController->controlMode == 2 && ShouldRun()) {
        const std::uint32_t playerState = static_cast<std::uint32_t>(g_ryoCurrentAnimationState);
        if (((playerState - 4U) & ~4U) == 0U)
            return 0;
        if (playerState == 10U && (g_playerMotion->currentAnimId == 0x42fU || g_playerMotion->currentAnimId == 0x434U))
            return 0;
    }

    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    Vector3 targetPosition{};
    float dx = 0.0F;
    float dz = 0.0F;
    if (object.stateByte19 != 0) {


        object.matrix = nullptr;
        float closestDistance = 99999.0F;
        for (std::int32_t index = 0; index < object.stateByte19; ++index) {
            Vector3 &point = object.data40[index];
            dx = point.x - ryoPosition.x;
            float dy = point.y - ryoPosition.y;
            dz = point.z - ryoPosition.z;
            if (std::fabs(dx) < 0.00000011920929F)
                dx = 0.0F;
            if (std::fabs(dy) < 0.00000011920929F)
                dy = 0.0F;
            if (std::fabs(dz) < 0.00000011920929F)
                dz = 0.0F;
            float candidate = dy * dy + dx * dx + dz * dz;
            if (std::fabs(dy) > g_animatedSequencePlayDistance)
                candidate = 99999.0F;
            if (closestDistance > candidate) {
                closestDistance = candidate;
                object.matrix = &point;
            }
        }
        distanceSquared = closestDistance;
    } else {
        targetPosition = target ? GetPositionForEntity(target->entity.node) : Vector3{};
        dx = targetPosition.x - ryoPosition.x;
        float dy = targetPosition.y - ryoPosition.y;
        dz = targetPosition.z - ryoPosition.z;
        if (std::fabs(dx) < 0.00000011920929F)
            dx = 0.0F;
        if (std::fabs(dy) < 0.00000011920929F)
            dy = 0.0F;
        if (std::fabs(dz) < 0.00000011920929F)
            dz = 0.0F;
        distanceSquared = dy * dy + dx * dx + dz * dz;
        if (std::fabs(dy) > g_animatedSequencePlayDistance)
            distanceSquared = 99999.0F;
    }

    if (object.runtimeState != 3 && g_searchTemporaryFocusedCharacter == 0 && !SCEN_HasActiveScript() && !scenIsOutsideOfScreenRange(target))
        distanceSquared = 99999.0F;

    const std::size_t rangeIndex = static_cast<std::size_t>(object.scriptSlot) * 3U;
    if (rangeIndex >= g_scenRuntimeState.rangeValues.size())
        return 0;
    const float authoredRange = g_scenRuntimeState.rangeValues[rangeIndex];
    if (authoredRange <= distanceSquared) {
        if (distanceSquared >= authoredRange + 5.0F) {
            object.runtimeFlags &= ~0x20U;
            object.runtimeFlags |= kRuntimeFlagPlayerLeftTalkRange;
        }
        return 0;
    }

    const std::uint16_t calculatedAngle = atan2_fixed16(-dx, -dz);
    const auto ryoYaw = static_cast<std::uint16_t>(ryo->entity.node.rotation.y);
    const std::uint16_t rawDifference =
        calculatedAngle <= ryoYaw ? static_cast<std::uint16_t>(ryoYaw - calculatedAngle) : static_cast<std::uint16_t>(calculatedAngle - ryoYaw);
    const std::int16_t finalDifference =
        rawDifference < 0x8000U ? static_cast<std::int16_t>(rawDifference) : static_cast<std::int16_t>(-static_cast<std::int32_t>(rawDifference));
    angle = static_cast<std::uint16_t>(finalDifference);

    if (object.runtimeState != 3 && (object.rangeMode == 1 || (object.rangeMode == 2 && target))) {
        std::uint16_t adjusted = static_cast<std::uint16_t>(calculatedAngle + 0x8000U);
        if (object.rangeMode == 2)
            adjusted = static_cast<std::uint16_t>(adjusted - static_cast<std::uint16_t>(target->entity.node.rotation.y));
        const std::uint16_t offset = static_cast<std::uint16_t>(adjusted - static_cast<std::uint16_t>(object.rangeMin));
        const std::uint16_t span = static_cast<std::uint16_t>(object.rangeMax - object.rangeMin);
        if (span < offset)
            return 0;
    }

    const std::uint16_t facing = static_cast<std::uint16_t>(calculatedAngle - ryoYaw + 0x4000U);
    if (facing > 0x8000U || coneCheck(targetPosition, ryoPosition) || CheckVectorIntersectionWithMaxAngleAndIterate(targetPosition, ryoPosition) ||
        !checkCharacterDistanceFromCoords(object.name.name, 0.3F) || ScenCharacterOnExcludedStairs(object.name.name)) {
        return 0;
    }
    return 1;
}

std::int32_t SCEN_InteractiveObjects(std::array<float, 24> &distances, std::array<std::int32_t, 24> &stateHints) {
    distances.fill(99999.0F);
    stateHints.fill(-1);
    ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    if (active && g_searchTemporaryFocusedCharacter) {
        if (g_searchTemporaryFocusedCharacter != active->name.name) {
            active->walkAwayReply = nullptr;
            const std::int32_t count = std::clamp(g_scenRuntimeState.objectCount, 0, 24);
            for (std::int32_t index = 0; index < count; ++index) {
                ScenRuntimeObject &candidate = g_scenRuntimeState.objects[static_cast<std::size_t>(index)];
                if (candidate.name.name != g_searchTemporaryFocusedCharacter) {
                    continue;
                }
                g_scenRuntimeState.activeObject = &candidate;
                SetScenActiveObjectName(candidate.name.name);
                break;
            }
        }
        const std::int32_t count = std::clamp(g_scenRuntimeState.objectCount, 0, 24);
        for (std::int32_t index = 0; index < count; ++index) {
            ScenRuntimeObject &candidate = g_scenRuntimeState.objects[static_cast<std::size_t>(index)];
            if (&candidate != g_scenRuntimeState.activeObject)
                continue;
            std::uint32_t angle = 0;
            const std::int32_t state = CalculateScenCharacterInRange(candidate, distances[static_cast<std::size_t>(index)], angle);
            stateHints[static_cast<std::size_t>(index)] = state;
            if (state > 0)
                return index;
        }
    }

    g_scenSearchCharacters.fill(0);
    std::size_t searchCount = 0;
    std::int32_t selected = -1;
    float bestDistance = 1000.0F;
    std::uint32_t bestAngle = 0;
    const std::int32_t count = std::clamp(g_scenRuntimeState.objectCount, 0, 24);
    for (std::int32_t index = 0; index < count; ++index) {
        ScenRuntimeObject &object = g_scenRuntimeState.objects[static_cast<std::size_t>(index)];
        std::uint32_t angle = 0;
        const std::int32_t state = CalculateScenCharacterInRange(object, distances[static_cast<std::size_t>(index)], angle);
        stateHints[static_cast<std::size_t>(index)] = state;
        if (state <= 0)
            continue;
        if (searchCount < 8U)
            g_scenSearchCharacters[searchCount++] = object.name.name;
        if (distances[static_cast<std::size_t>(index)] < bestDistance) {
            bestDistance = distances[static_cast<std::size_t>(index)];
            bestAngle = angle;
            selected = index;
        }
    }
    g_scenSearchCharacters[searchCount] = 0;
    g_scenRuntimeState.focusBestAngle = bestAngle;
    g_scenRuntimeState.focusBestDistance = bestDistance;
    return selected;
}

void QueueScenConfirmPrompt() {
    if ((ScenFlags() & kScenarioFlagOneShotPrompt) != 0)
        return;
    ScenFlags() |= kScenarioFlagOneShotPrompt;
    (void)UI_ICONRequestNotification(0x100U, 0, 1, 1);
}

void ClearScenConfirmPrompt() {
    if ((ScenFlags() & kScenarioFlagOneShotPrompt) == 0)
        return;
    ScenFlags() &= ~kScenarioFlagOneShotPrompt;
    (void)UI_ICONRequestNotification(0x100U, 0, 0, 2);
}

bool UpdateScenPendingRuntimeEvent(ScenRuntimeObject &object) {
    const std::uint32_t pending = object.runtimeFlags & 0xf0000U;
    if (!pending)
        return false;
    if ((pending & kRuntimeFlagPendingWait) != 0)
        ++object.waitCount;
    if ((pending & kRuntimeFlagPendingFailure1) != 0)
        ++object.failCount1;
    if ((pending & kRuntimeFlagPendingFailure2) != 0)
        ++object.failCount2;
    if ((pending & kRuntimeFlagPendingFailure3) != 0)
        ++object.failCount3;

    struct RelationEvent {
        std::uint32_t name;
        std::int32_t code;
    } event{};
    bool haveEvent = false;
    if (g_scenarioFunctionWork && g_scenarioFunctionWork->scnfRelationHandle >= 0) {
        haveEvent = DequeueScenarioRelationData(static_cast<std::uint32_t>(g_scenarioFunctionWork->scnfRelationHandle), &event, sizeof(event)) >= 1;
    }
    if (!haveEvent) {
        event.name = object.name.name;
        if (static_cast<std::int16_t>(object.waitCount) >= 30)
            event.code = -10;
        else if (static_cast<std::int16_t>(object.failCount1) >= 30)
            event.code = -1;
        else if (static_cast<std::int16_t>(object.failCount3) >= 30)
            event.code = -2;
        else if (static_cast<std::int16_t>(object.failCount2) >= 30)
            event.code = -8;
        else
            return true;
    }
    if (event.name != object.name.name)
        return true;
    switch (event.code) {
    case -10:
        object.runtimeFlags &= ~kRuntimeFlagPendingWait;
        object.waitCount = 0;
        if ((object.runtimeFlags & kRuntimeFlagConversationCamera) != 0) {
            ScenFlags() |= kScenarioFlagConversationCamera;
            g_scenarioTaskDeltaTime =
                static_cast<std::int32_t>(static_cast<std::uint32_t>(FrameTimeUtil(TimeMode::ReturnNewTime)) - static_cast<std::uint32_t>(g_scnfDeltaTime));
        }
        break;
    case -8:
        ScenFlags() &= ~kScenarioFlagInteractionModal;
        object.runtimeFlags &= ~kRuntimeFlagPendingFailure2;
        object.runtimeFlags |= kRuntimeFlagExitLatched;
        object.failCount2 = 0;
        break;
    case -2:
        object.runtimeFlags &= ~kRuntimeFlagPendingFailure3;
        object.failCount3 = 0;
        if (!ShouldRun() && (ScenFlags() & kScenarioFlagPadBlockOverride) != 0) {
            ScenFlags() &= ~kScenarioFlagPadBlockOverride;
            navResetAuthAndInputStates();
        }
        break;
    case -1:
        object.runtimeFlags &= ~kRuntimeFlagPendingFailure1;
        g_pAudio = nullptr;
        object.failCount1 = 0;
        break;
    default:
        break;
    }
    return true;
}



void BlockScenPlayerControl() {
    if (!ShouldRun())
        return;
    g_suppressDefaultHudPromptIcons = 1;
    ScenFlags() |= kScenarioFlagPadBlockOverride;
    g_pControlPadController->blockedCanControl = 0;
    g_pControlPadController->state = 2;
    g_tmexValHigh = 0;
}




void BeginScenInteraction(ScenRuntimeObject &object) {
    BlockScenPlayerControl();
    ScenFlags() = (ScenFlags() & ~(kScenarioFlagInteractionResume | kScenarioFlagInteractionModal)) | kScenarioFlagInteractionModal;
    object.runtimeFlags &= 0xfcffbf7fU;
    object.field14 = -1;
    object.walkAwayReply = nullptr;
    const std::uint32_t *const participants = static_cast<const std::uint32_t *>(object.participants);
    const std::int64_t count = (reinterpret_cast<const std::byte *>(object.voicePath) - static_cast<const std::byte *>(object.participants)) / 4;
    std::int32_t index = 0;
    for (; index < count && index + 1 < static_cast<std::int32_t>(g_scenRuntimeState.referencedCharacterIds.size()); ++index)
        g_scenRuntimeState.referencedCharacterIds[static_cast<std::size_t>(index)] = participants[index];
    g_scenRuntimeState.referencedCharacterIds[static_cast<std::size_t>(index)] = 0;
}

std::int32_t g_scenarioInteractionStartTicks = 0;

// @verified - p
std::int64_t scenarioUpdateDeltaTimeTicks() {
    const std::int64_t ticks = static_cast<std::int64_t>(g_LastFrameTime - QueryFrameTime());
    g_scenarioInteractionStartTicks = static_cast<std::int32_t>(ticks);
    return ticks;
}

// @verified - p
float scenarioGetCurrentFrameTimeNormalized() {
    const std::int32_t current = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
    const std::uint32_t elapsed = static_cast<std::uint32_t>(g_scenarioInteractionStartTicks) - static_cast<std::uint32_t>(current);
    return static_cast<float>(static_cast<double>(static_cast<float>(elapsed)) * 30.0 / 12500000.0);
}
bool UpdateScenStateFiveRecovery(ScenRuntimeObject &object, const PadCaptureEntry *pad) {
    if ((object.runtimeFlags & kRuntimeFlagInteractionResume) == 0)
        return false;
    if (((ScenFlags() & kScenarioFlagInteractionResume) != 0 || (object.runtimeFlags & 0x800000U) != 0) && g_isPlayerGameStateActive == 7) {
        object.runtimeFlags &= ~kRuntimeFlagInteractionResume;
        ScenFlags() &= ~kScenarioFlagInteractionResume;
        object.runtimeFlags |= 0x3040000U;
        object.failCount2 = 0;
        SCNF_LogStateChange(object, -8);
        ClearScenAuthorGameState();
        RestoreScenPadContext();
    }
    if ((object.runtimeFlags & 0x1000000U) == 0) {
        object.runtimeFlags |= 0x1000000U;
        scenarioUpdateDeltaTimeTicks();
        return true;
    }
    if ((object.runtimeFlags & 0x2000000U) != 0)
        return true;

    const bool cancelInput = pad &&
        (((pad->pressedPriority & 0x0200U) != 0) || (pad->held & 0xf0c0U) != 0 || static_cast<std::uint8_t>(pad->rightStick) != 0x80U ||
            static_cast<std::uint8_t>(pad->rightStick >> 8U) != 0x80U);
    if ((scenarioGetCurrentFrameTimeNormalized() >= 150.0f || cancelInput) && g_isPlayerGameStateActive == 7) {
        object.runtimeFlags |= kRuntimeFlagPendingFailure2;
        object.failCount2 = 0;
        SCNF_LogStateChange(object, -8);
        ClearScenAuthorGameState();
        RestoreScenPadContext();
        object.runtimeFlags |= 0x2000000U;
    }
    return true;
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

bool SCEN_IsPlayerAllowed() {
    const auto *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const film = control && control->filmTask && control->filmTask->task_data_ptr ?
        reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4) :
        nullptr;
    const bool blockedSearchState = ((g_currentSearchState - 2) & ~3) == 0 &&
        g_currentSearchState != static_cast<std::int32_t>(SearchState::AutoCharacterLockOn) && g_searchCandidateId < 0;
    return g_NumFramesProcessed == 0 && control && control->ryo && g_pControlPadController && g_tmex_val <= 1U &&
        g_pControlPadController->blockedCanControl == 1 && g_searchCandidateId < 0 && !blockedSearchState && g_searchTemporaryFocusedCharacter == 0 &&
        camCtrlGetState() <= 0 && g_isPlayerGameStateActive != 0 && !g_notebookHelpOverrideActive && UI_ICSpIsIdle() &&
        g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_NONE && film && film->current[0] == 0 && !SCEN_HasActiveScript();
}

void SCEN_SetConversationPromptEnabled(bool enabled) noexcept {
    g_scenarioConversationPromptEnabled = enabled;
}

bool SCEN_IsConversationPromptEnabled() noexcept {
    return g_scenarioConversationPromptEnabled;
}

bool SCEN_HasActiveScript() noexcept {
    return TimedTransitionScenarioBusy();
}

const std::array<std::uint32_t, 9> &SCEN_GetSearchCharacterList() noexcept {
    return g_scenSearchCharacters;
}

std::size_t SCEN_CopyFreeTalkParticipants(std::uint32_t characterName, std::uint32_t *participants, std::size_t capacity) noexcept {
    if (!participants || capacity == 0)
        return 0;
    const ScenRuntimeObject *const object = FindScenRuntimeObject(characterName);
    if (!object || !object->participants)
        return 0;
    const std::uint32_t *const source = static_cast<const std::uint32_t *>(object->participants);
    std::size_t count = 0;
    while (count < capacity && count < 8 && source[count] != 0) {
        participants[count] = source[count];
        ++count;
    }
    return count;
}

const Vector3 *SCEN_GetFreeTalkLocation(std::uint32_t characterName, std::size_t locationIndex) noexcept {
    const ScenRuntimeObject *const object = FindScenRuntimeObject(characterName);
    return object && object->locations ? object->locations + locationIndex : nullptr;
}

bool SCEN_IsFreeTalkCharacterContextOwned(std::uint32_t characterName) noexcept {
    ScenarioStack stack{};
    ScenarioContext context{nullptr, &stack};
    const std::array<std::uint32_t, 2> params{3, characterName};
    Script_CPCT_related_0(&context, const_cast<std::uint32_t *>(params.data()));
    return stack.accumulator != 0 || g_scenarioStepLut.pendingCharacterName == characterName;
}

bool SCEN_AreFreeTalkConstraintsDisabled() noexcept {
    return g_scenRuntimeState.activeObject && (g_scenRuntimeState.activeObject->runtimeFlags & 0x400000U) != 0;
}

void SCEN_ClearFreeTalkRuntimeFlag() noexcept {
    ScenFlags() &= ~0x80U;
}

void SCEN_ActivateFreeTalkRuntime() {
    CreateScenSequenceCharacters();
}

bool SCEN_IsInteractionModalActive() noexcept {
    return (ScenFlags() & kScenarioFlagInteractionModal) != 0;
}

bool SCEN_IsConversationCameraActive() noexcept {
    return (ScenFlags() & kScenarioFlagConversationCamera) != 0;
}

bool SCEN_BlocksDoorCandidate() noexcept {
    return g_scenRuntimeState.activeObject && g_scenRuntimeState.focusBestAngle <= 10922U;
}

} // namespace shenmue::s1
