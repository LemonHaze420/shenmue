#include "s1/task/scen_state.hpp"

namespace shenmue::s1 {



std::uint32_t scntCheckHasParticipant(std::uint32_t name) {
    if (!name)
        return 0;
    Character *const chara = CHID_Find(g_scenarioFunctionWork ? g_scenarioFunctionWork->scenarioTask : nullptr, name);
    const MapControlData *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return (control && control->ryo == chara) || name == g_cpctLastOperand || name == g_cpctPreviousOperand || name == SHEN_NAME(null) ||
        name == SHEN_NAME(AKIR) || name == SHEN_NAME(AKID) || name == SHEN_NAME(JAKR) || name == SHEN_NAME(AKI_);
}

void EnqueueSCNT(std::uint32_t name, std::int16_t stateValue) {
    if (g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0)
                return;
        }
    }
    if ((ScenFlags() & kScenarioFlagInteractionModal) != 0)
        return;

    SCNT_EnsureFreeTalkRuntime();
    if (g_scenarioFunctionWork && !g_scenarioFunctionWork->active)
        UpdateScenTaskTimer();
    if (g_scenRuntimeState.activeObject)
        FinalizeScenRuntimeObject(g_scenRuntimeState.activeObject);

    ScenFlags() |= kScenarioFlagActiveRuntimeObject;
    SetScenActiveObjectName(name);
    ScenRuntimeObject *const object = FindScenRuntimeObject(name);
    if (!object)
        return;

    ScenFlags() |= kScenarioFlagUnknown200 | kScenarioFlagInteractionModal;
    g_scenRuntimeState.activeObject = object;
    object->runtimeState = 0;
    object->field28 = stateValue;
    object->runtimeFlags = (object->runtimeFlags & 0xfffc7fffU) | kRuntimeFlagStateActive;
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

void SCNT_InitWrapper() {
    ScenFlags() &= ~kScenarioFlagInteractionModal;
    if (g_scenarioFunctionWork) {
        g_scenarioFunctionWork->active = 0;
        g_scenarioFunctionWork->activeObjectName = 0;
    }
    SetScenActiveObjectName(0);
    ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
    if (active && (active->runtimeFlags & kRuntimeFlagConversationCamera) != 0) {
        (void)d3t::ResetElementByIdx(0);
        g_scenarioTaskStartTime = FrameTimeTicks(g_LastFrameTime - QueryFrameTime()) - g_scenarioTaskDeltaTime;
        g_scenarioFunctionWork->pendingDeltaReset = 1;
    }
    if (active) {
        SCNT_ResetConversationState();
        active->name.name = 0;
        g_scenRuntimeState.activeObject = nullptr;
    }
    if (g_notebookJingleSubtitle) {
        if (g_notebookJingleSubtitle->callback)
            shenForceClearCurrentSubtitles(g_notebookJingleSubtitle);
        g_notebookJingleSubtitle = nullptr;
    }
    ClearScenAuthorGameState();
    RestoreScenPadContext();
    if ((ScenFlags() & kScenarioFlagOneShotPrompt) != 0) {
        ScenFlags() &= ~kScenarioFlagOneShotPrompt;
        (void)UI_ICONRequestNotification(0x100U, 0, 0, 2);
    }
}




bool SCNT_IsParticipantTurnSettled(std::uint32_t name) {
    bool voiceActive = false;
    if (g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0) {
                voiceActive = true;
                break;
            }
        }
    }
    if (!voiceActive && (ScenFlags() & 0x20000U) == 0)
        return true;
    if (name == g_cpctLastOperand || name == g_cpctPreviousOperand || name == g_scenarioStepLut.pendingCharacterName)
        return true;
    Character *const chara = CHID_Find(g_scenarioFunctionWork ? g_scenarioFunctionWork->scenarioTask : nullptr, name);
    return !chara || !charaHasBlockingTPMRequest(chara);
}

void SCNT_Core(ScenRuntimeObject *object) {
    if (!object)
        return;
    if (object->relocatedPtrA8 && g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0) {
                object->relocatedPtrA8 = nullptr;
                break;
            }
        }
    }
    ScenFlags() |= kScenarioFlagCoreRanThisFrame;
    if (!ScenInteractionBlocked() || (object->runtimeFlags & 0x800000U) != 0)
        ScenFlags() &= ~kScenarioFlagQueueConfirmPrompt;
    if ((ScenFlags() & kScenarioFlagQueueConfirmPrompt) != 0) {
        ScenFlags() &= ~kScenarioFlagQueueConfirmPrompt;
        QueueScenConfirmPrompt();
    } else {
        ClearScenConfirmPrompt();
    }

    if ((object->runtimeFlags & 8U) != 0) {
        std::int16_t selectedHelp = -1;
        if (!UI_ICSpPeekSelectedHelpToken(selectedHelp))
            return;
        object->field24 = selectedHelp == -1 ? 0U : static_cast<std::uint16_t>(selectedHelp);
        object->runtimeFlags &= ~0x10U;
    }
    if ((object->runtimeFlags & 4U) != 0) {
        std::int16_t selectedHelp = -1;
        if (!UI_ICSpPeekSelectedHelpToken(selectedHelp))
            return;
        const std::uint8_t selected = static_cast<std::uint8_t>(selectedHelp);
        object->field24 = selected == 0 ? 1U : selected == 1 ? 0U : selected;
        object->runtimeFlags &= ~0x10U;
    }
    if (SCEN_HasActiveScript() && SCNT_IsFreeTalkPairSetupPending(0) && !SCNT_IsParticipantTurnSettled(object->name.name))
        return;

    ScenarioFunctionWork *const scnf = g_scenarioFunctionWork;
    if ((scnf->flags & (kScenarioFlagConversationMarkers | kScenarioFlagTimelineActive)) == (kScenarioFlagConversationMarkers | kScenarioFlagTimelineActive))
        convUpdateCameraPos();
    if ((object->runtimeFlags & 0xf0000U) != 0) {
        (void)UpdateScenPendingRuntimeEvent(*object);
        return;
    }

    // The conversation timeline. The clock is the seconds since the first line started (g_evScenarioTaskDeltaTime) plus
    // g_evScenarioTaskSubtitleTime; an entry runs once the clock reaches its time and the previous line's voice has finished.
    const auto timelineClock = []() {
        const std::uint32_t now = static_cast<std::uint32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
        return static_cast<float>(static_cast<std::uint32_t>(g_scenarioTaskDeltaTime) - now) / 12500000.0F + g_scenarioTaskSubtitleTime;
    };
    const auto beginTimelineWait = [&]() {
        scnf->flags &= ~kScenarioFlagConversationCamera;
        object->runtimeFlags |= kRuntimeFlagPendingWait;
        object->waitCount = 0;
        g_scnfDeltaTime =
            static_cast<std::int32_t>(static_cast<std::uint32_t>(FrameTimeUtil(TimeMode::ReturnNewTime)) - static_cast<std::uint32_t>(g_scenarioTaskDeltaTime));
        ++scnf->timelineCursor;
    };
    const std::uint32_t dialogueMode = (S1SettingsFlags_Get() >> 4U) & 3U;
    bool lineEnded = false;
    if (((scnf->currentPadEntry && (scnf->currentPadEntry->pressedPriority & 0x200U) != 0) && dialogueMode != 0 && g_scntFrameTimer <= 0 &&
            (scnf->flags & kScenarioFlagSuppressAutoSubtitleStart) == 0) ||
        g_scnfCanSkip) {
        if (dialogueMode != 1 || (scnf->flags & kScenarioFlagSubtitleLineActive) != 0 || g_scnfCanSkip) {
            shenForceClearCurrentSubtitles(nullptr);
            const float lineEnd = scnf->timelineCursor->time;
            g_scnfCanSkip = 0;
            g_scenarioTaskSubtitleTime += lineEnd - timelineClock();
            lineEnded = true;
        } else {
            AssignReadSubtitleByID();
            *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(CurrentScenarioTask()->task_data_ptr) + kScenarioDialogueFlagsOffset) |= 0x40U;
            if (const ScenLineEntry *const line = static_cast<const ScenLineEntry *>(object->field78)) {
                if (g_pAudio) {
                    ReadSubtitleByID(scnf->scenarioTask, *g_pAudio);
                } else {
                    const char *const lineId = static_cast<const char *>(object->fileHeader) + static_cast<std::int32_t>(line->lineIdOffset);
                    SCNT_ShowSubtitleText(scnf->scenarioTask, InitAndRetrieveStringFromID(lineId), static_cast<std::uint32_t>(object->subtitleSpeakerClass));
                }
                object->runtimeFlags |= kRuntimeFlagSubtitleCleanupPending;
            }
            scnf->flags |= kScenarioFlagSubtitleLineActive;
        }
    } else if (g_scntFrameTimer > 0) {
        --g_scntFrameTimer;
    }
    if (!lineEnded && (object->runtimeFlags & kRuntimeFlagConversationCamera) != 0) {
        if ((object->runtimeFlags & kRuntimeFlagTimelineAdvancePending) != 0) {
            object->runtimeFlags &= ~kRuntimeFlagTimelineAdvancePending;
            for (;;) {
                ScenTimelineEntry *const entry = scnf->timelineCursor;
                if (entry->payload) {
                    object->activeLineOrMarker = entry->payload;
                    object->field78 = entry->payload;
                    if ((object->runtimeFlags & kRuntimeFlagSilentTimeline) == 0) {
                        if (SCNT_IsLineVoicePlaying())
                            return;
                        const ScenLineEntry *const line = static_cast<const ScenLineEntry *>(object->activeLineOrMarker);
                        const char *const lineId = static_cast<const char *>(object->fileHeader) + static_cast<std::int32_t>(line->lineIdOffset);
                        g_pAudio = nullptr;
                        if (line->lineIdOffset) {
                            std::uint32_t aliasState = 0;
                            g_pAudio = ProcessSubtitleFile(lineId, scnf->timelineCursor->command, 0, &aliasState);
                            if (aliasState == 1) {
                                g_scnfCanSkip = 1;
                                scnf->timelineCursor->time = 0.0F;
                                ++scnf->timelineCursor;
                                return;
                            }
                        }
                        const std::uint32_t speakerClass = scntCheckHasParticipant(scnf->timelineCursor->command);
                        object->subtitleSpeakerClass = static_cast<std::int32_t>(speakerClass);
                        if (!g_pAudio) {
                            SCNT_ShowSubtitleText(scnf->scenarioTask, InitAndRetrieveStringFromID(lineId), speakerClass);
                            object->runtimeFlags |= kRuntimeFlagSubtitleCleanupPending;
                        }
                        if ((scnf->flags & kScenarioFlagTimelineActive) == 0) {
                            scnf->flags |= kScenarioFlagTimelineActive;
                            g_scenarioTaskDeltaTime = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
                            if ((scnf->flags & kScenarioFlagConversationMarkers) != 0) {
                                scnf->flags |= kScenarioFlagConversationCamera;
                                if (g_searchTemporaryFocusedCharacter)
                                    SRCH_Reset();
                                CHAR_SetHandFaceDetails(1);
                                scntStartConversationCamera();
                            }
                        }
                        if (!g_pAudio && scnf->timelineCursor->command) {
                            if (Character *const speaker = CHID_Find(scnf->scenarioTask, scnf->timelineCursor->command)) {
                                if (FaceClipControllerState *const clip = CLIP_GetWork(speaker)) {
                                    scnfGenerateLipCues(*object);
                                    clip->enabled = 1;
                                    clip->generatedLipCues = g_scenRuntimeState.generatedLipCues.data();
                                }
                            }
                        }
                    }
                } else {
                    const std::uint32_t command = entry->command;
                    if (!command)
                        return;
                    if ((command & 0xf000U) == 0x3000U) {
                        if ((command & 0xfffU) != 0x10U) {
                            SCNF_LogStateChange(*object, static_cast<std::int32_t>(command & 0xfffU));
                            beginTimelineWait();
                            return;
                        }
                        g_bGlobalFlag = 1;
                    } else if ((command & 0xf000U) == 0x6000U) {
                        if ((command & 0xfffU) < 0x400U)
                            SetStatOrNotebookFlag(static_cast<std::uint16_t>(command & 0xfffU), true);
                    } else if (command == 0xfc00U) {
                        beginTimelineWait();
                        return;
                    }
                }
                ++scnf->timelineCursor;
                if (timelineClock() < scnf->timelineCursor->time)
                    return;
            }
        }
        if (scnf->timelineCursor->time > timelineClock() || SCNT_IsLineVoicePlaying())
            return;
        if (g_pAudio && CVWK_IsSlotActive(g_pAudio))
            shenForceClearCurrentSubtitles(g_pAudio);
        lineEnded = true;
    }
    if (lineEnded) {
        if ((scnf->flags & kScenarioFlagSubtitleLineActive) != 0) {
            scnf->flags &= ~kScenarioFlagSubtitleLineActive;
            object->field78 = nullptr;
        }
        if ((object->runtimeFlags & kRuntimeFlagSubtitleCleanupPending) != 0) {
            DisableSubtitles();
            object->runtimeFlags &= ~kRuntimeFlagSubtitleCleanupPending;
        }
        if (scnf->timelineCursor->command) {
            object->runtimeFlags |= kRuntimeFlagTimelineAdvancePending;
            g_scntFrameTimer = 15;
            return;
        }
        object->runtimeFlags &= ~(kRuntimeFlagConversationCameraActive | kRuntimeFlagConversationCamera);
        if ((scnf->flags & kScenarioFlagTimelineActive) != 0) {
            if ((scnf->flags & kScenarioFlagConversationMarkers) != 0)
                convEndConversationCamera();
            scnf->flags &= ~(kScenarioFlagTimelineActive | kScenarioFlagConversationCamera);
        }
    }

    constexpr std::uint32_t decodeStates = 0x1d1U;
    if (object->runtimeState == 3 && (object->runtimeFlags & 0x8000U) == 0)
        return;
    const bool shouldDecode =
        (object->runtimeFlags & kRuntimeFlagStateDecode) == 0 && (object->runtimeState > 8 || (decodeStates & (1U << object->runtimeState)) == 0);
    if (shouldDecode && object->scriptSlot >= 301U) {
        if (object->runtimeState != 5)
            object->runtimeState = 0;
        const std::size_t slot = ScenWordSlotIndex(*object);
        if (slot < g_scntWord22.size())
            g_scntWord22[slot] = 0;
    } else if (shouldDecode) {
        std::byte *const decoded = ScanScenConditionalFlow(*object, object->instructionPtr);
        const std::size_t slot = ScenWordSlotIndex(*object);
        if (decoded && slot < g_scntWord20.size()) {
            const auto offset = decoded - object->scriptStart;
            if (offset >= 0 && offset <= std::numeric_limits<std::uint16_t>::max() && static_cast<std::uint16_t>(offset) != g_scntWord20[slot]) {
                object->instructionPtr = object->scriptStart;
                g_scntWord26[slot] = 0;
                g_scntWord20[slot] = static_cast<std::uint16_t>(offset);
                g_scntWord24[slot] = 0;
                g_scntWord22[slot] = 0;
                object->runtimeFlags |= kRuntimeFlagStateDecode;
                object->runtimeState = 0;
                (void)RunScenBytecode(*object);
                return;
            }
        } else if (!decoded) {
            object->runtimeState = 0;
        }
    }
    if (object->runtimeState != 5) {
        object->runtimeFlags &= ~kRuntimeFlagExitLatched;
        object->runtimeFlags |= kRuntimeFlagStateDecode;
        g_scenarioTaskSubtitleTime = 0.0F;
        g_scenRuntimeState.randomBranchCount = 0;
        g_scntFrameTimer = 0;
        (void)RunScenBytecode(*object);
        return;
    }

    if ((object->runtimeFlags & 2U) == 0) {
        object->runtimeFlags &= ~0x80U;
        ScenFlags() |= kScenarioFlagQueueConfirmPrompt;
        const MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        const auto *const pad = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
        const bool pressedInteract = pad && (pad->pressedPriority & 0x0400U) != 0;
        if (!filmGetTaskFlag() && (!g_searchTemporaryFocusedCharacter || !pressedInteract)) {
            (void)UpdateScenStateFiveRecovery(*object, pad);
            return;
        }
        if (!ShouldRun() || g_scenHelpPromptOverride != 0)
            return;
        if (g_areaId != AREA_MFSY.name || SCEN_HasActiveScript())
            g_bGlobalFlag = 1;
        BeginScenInteraction(*object);
        if (object->scriptSlot < kScntScriptSlotCount) {
            const std::size_t slot = ScenWordSlotIndex(*object);
            if ((object->runtimeFlags & 0x8000U) == 0 || (object->runtimeFlags & 0x200U) != 0) {
                if (!ScenCanRead(*object, object->instructionPtr, 3))
                    return;
                object->instructionPtr += 3;
            } else if (slot < g_scntWord22.size()) {
                std::byte *const branch = object->scriptStart + g_scntWord22[slot];
                if (!ScenCanRead(*object, branch, 3))
                    return;
                const std::uint32_t raw = std::to_integer<std::uint8_t>(branch[2]) |
                    ((std::to_integer<std::uint8_t>(branch[1]) + (std::to_integer<std::uint8_t>(branch[0]) << 8U)) << 8U);
                std::byte *const next = branch + 3;
                const std::int32_t offset = ScenSignExtend(raw, 0x800000U);
                object->runtimeFlags = (object->runtimeFlags | 0x100U) & ~0x8000U;
                if (offset) {
                    object->instructionPtr = ScenAddRelative(*object, next, offset);
                    if (!object->instructionPtr)
                        return;
                    object->savedInstructionPtr = next;
                } else {
                    object->savedInstructionPtr = nullptr;
                    object->instructionPtr = next;
                }
            }
        }
    }
    object->runtimeFlags &= ~kRuntimeFlagExitLatched;
    object->runtimeFlags |= kRuntimeFlagStateDecode;
    g_scenarioTaskSubtitleTime = 0.0F;
    g_scenRuntimeState.randomBranchCount = 0;
    g_scntFrameTimer = 0;
    (void)RunScenBytecode(*object);
}






std::byte *BuildScenConversationMarkers(ScenRuntimeObject &object, std::byte *ip) {
    ScenarioFunctionWork *const scnf = g_scenarioFunctionWork;
    const std::uint32_t *const participants = static_cast<const std::uint32_t *>(object.participants);
    std::uint8_t cueFlags = 0;
    std::byte *cursor = ip;
    std::uint32_t previousName = 0;
    std::uint32_t name = 0;
    bool matrixCueSet = false;
    std::uint8_t savedCueFlags = 0;
    std::uint8_t savedMatrixCue = 0;
    float currentTime = 0.0F;
    float terminalDuration = 0.0F;
    std::uint32_t otherName = 0;
    float leadingDuration = 0.0F;
    if (participants) {
        if (scnf->scntVariable18)
            name = participants[scnf->scntVariable18 - 'A'];
        if (scnf->scntVariable19)
            otherName = participants[scnf->scntVariable19 - 'A'];
    }
    scnf->flags &= ~kScenarioFlagConversationMarkers;
    ScenTimelineEntry *entry = g_scenRuntimeState.timelineEntries.data();
    ScntConversationCue *cue = g_scntConversationCueWorkbuf.data();
    ScntConversationCue *const cueEnd = g_scntConversationCueWorkbuf.data() + g_scntConversationCueWorkbuf.size() - 1;
    scnf->timelineCursor = entry;
    std::uint32_t opcode = std::to_integer<std::uint8_t>(*ip);
    std::array<std::uint8_t, 24> referencedSlots{};
    g_scenRuntimeState.referencedObjectCount = 0;
    g_scenRuntimeState.referencedCharacterCount = 0;
    Character *const ryo = CurrentScenarioRyo();
    std::byte *result = nullptr;
    const auto readByte = [&]() { return static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(*cursor)); };
    while (!result) {
        ++cursor;
        switch (opcode & 0xf0U) {
        case 0x00:
        case 0x70:
            ++cursor;
            opcode = readByte();
            break;
        case 0x20: {
            const std::uint32_t lineIndex = readByte() | ((opcode & 0xfU) << 8U);
            ++cursor;
            ScenLineEntry *const line = static_cast<ScenLineEntry *>(object.lineTable) + lineIndex;
            double seconds = 0.0;
            if (line->delaySeconds == 0.0F) {
                const char *const text = static_cast<const char *>(object.fileHeader) + static_cast<std::int32_t>(line->textOffset);
                std::int32_t length = 0;
                while (text[length])
                    ++length;
                if (length > 12) {
                    if (length > 18)
                        seconds = static_cast<double>(length > 24 ? 3 * (length - 6) : 4 * length - 24) / 30.0;
                    else
                        seconds = static_cast<double>(5 * (length - 6)) / 30.0;
                } else {
                    seconds = static_cast<double>(10 * (length - 6)) / 30.0;
                }
            } else {
                seconds = static_cast<double>(line->delaySeconds) + 0.5;
            }
            const float duration = static_cast<float>(seconds);
            cue->flags = cueFlags;
            cue->size = 4;
            cue->kind = 1;
            cue->variant = 1;
            cue->durationSeconds = duration;
            entry->time = currentTime;
            entry->secondaryActor = SHEN_NAME(null);
            entry->command = SHEN_NAME(null);
            entry->payload = line;
            char lineCode = 0;
            if (object.matrix && !matrixCueSet) {
                name = make_shen_name('$', 'f', 'r', 'e');
                savedMatrixCue = 1;
                otherName = shenGetNextCharaFromTable(scnf->scenarioTask, ryo);
                if (scnf->characterHandler)
                    shenApplyTranslationToAll(scnf->characterHandler->entity.node, *static_cast<const Vector3 *>(object.matrix));
            } else if (!name) {
                lineCode = line->code[0];
                if (participants)
                    name = participants[lineCode - 'A'];
            }
            if (!name)
                name = lineCode == 'A' ? shenGetNextCharaFromTable(scnf->scenarioTask, ryo) : object.name.name;
            if (name == previousName) {
                entry->command = name;

                if (cue != g_scntConversationCueWorkbuf.data()) {
                    cue[-1].flags |= savedCueFlags;
                    cue[-1].durationSeconds = duration + cue[-1].durationSeconds;
                }
            } else {
                if (!otherName) {
                    if (previousName) {
                        otherName = previousName;
                    } else {
                        otherName = object.name.name;
                        if (name == otherName) {
                            otherName = shenGetNextCharaFromTable(scnf->scenarioTask, ryo);
                        } else if (!CHID_Find(scnf->scenarioTask, otherName) && participants) {
                            for (std::int32_t index = 0; index < static_cast<std::int8_t>(object.participantCount); ++index) {
                                if (participants[index] && participants[index] != name) {
                                    otherName = participants[index];
                                    break;
                                }
                            }
                        }
                    }
                }
                Character *const actor0 = CHID_Find(scnf->scenarioTask, name);
                cue->actor0TaskDataOffset = actor0 ? static_cast<std::int32_t>(HLib_GetTaskDataOffset(actor0)) : 0;
                if (!actor0)
                    cue->actor1TaskDataOffset = 0;
                if (Character *const actor1 = CHID_Find(scnf->scenarioTask, otherName)) {
                    cue->actor1TaskDataOffset = static_cast<std::int32_t>(HLib_GetTaskDataOffset(actor1));
                } else {

                    cue->actor1TaskDataOffset = 0;
                    if (cue + 1 < g_scntConversationCueWorkbuf.data() + g_scntConversationCueWorkbuf.size())
                        cue[1].size = cue[1].kind = cue[1].variant = cue[1].flags = 0;
                }
                if (cue->actor0TaskDataOffset && cue->actor1TaskDataOffset) {
                    if (cue == g_scntConversationCueWorkbuf.data()) {
                        cue->flags |= savedCueFlags;
                        cue->durationSeconds = leadingDuration + cue->durationSeconds;
                    }
                    if (cue < cueEnd) {
                        ++cue;
                        entry->command = name;
                        entry->secondaryActor = otherName;
                    } else {
                        cue[-1].flags |= savedCueFlags;
                        cue[-1].durationSeconds = duration + cue[-1].durationSeconds;
                    }
                } else if (cue != g_scntConversationCueWorkbuf.data()) {
                    cue[-1].flags |= savedCueFlags;
                    cue[-1].durationSeconds = duration + cue[-1].durationSeconds;
                } else {
                    leadingDuration = leadingDuration + duration;
                }
                if (lineCode && object.participantCount) {
                    const std::int32_t slot = lineCode - 'A';
                    if (!referencedSlots[static_cast<std::size_t>(slot)]) {
                        const std::int32_t index = g_scenRuntimeState.referencedCharacterCount++;
                        referencedSlots[static_cast<std::size_t>(slot)] = 1;
                        g_scenRuntimeState.referencedCharacterIds[static_cast<std::size_t>(index)] = participants[slot];
                        if (actor0)
                            g_scenRuntimeState.referencedObjects[static_cast<std::size_t>(g_scenRuntimeState.referencedObjectCount++)] = actor0;
                    }
                }
            }
            matrixCueSet = savedMatrixCue != 0;
            currentTime = currentTime + duration;
            opcode = readByte();
            ++entry;
            previousName = name;
            cueFlags = 0;
            otherName = 0;
            savedCueFlags = 0;
            terminalDuration = currentTime;
            name = 0;
            break;
        }
        case 0x30: {
            entry->time = currentTime;
            const std::uint32_t command = readByte() | (opcode << 8U);
            ++cursor;
            entry->payload = nullptr;
            entry->command = command;
            const std::uint32_t event = command & 0xfffU;
            if (event - 20U <= 9U || event - 120U <= 9U) {
                scnf->flags |= kScenarioFlagInteractionResume;
                cueFlags |= 2U;
                savedCueFlags = cueFlags;
                g_Conv.interactionCueLocation = object.locations[entry->command - (entry->command < 0x3078U ? 20U : 120U) - 0x3000U];
            }
            ++entry;
            currentTime = static_cast<float>(static_cast<double>(currentTime) + 1.0 / 30.0);

            if (cue != g_scntConversationCueWorkbuf.data())
                cue[-1].durationSeconds = static_cast<float>(static_cast<double>(cue[-1].durationSeconds) + 1.0 / 30.0);
            opcode = readByte();
            terminalDuration = currentTime;
            break;
        }
        case 0x40:
            if (g_scenRuntimeState.conditionResult) {
                opcode = readByte();
                break;
            }
            [[fallthrough]];
        case 0x50: {
            const std::uint32_t value = readByte() | ((opcode & 0xfU) << 8U);
            cursor += ScenSignExtend(value, 0x800U) + 1;
            opcode = readByte();
            break;
        }
        case 0x60:
            entry->time = currentTime;
            entry->payload = nullptr;
            entry->command = readByte() | (opcode << 8U);
            ++cursor;
            ++entry;
            opcode = readByte();
            break;
        case 0x90:
            ++cursor;
            if ((object.runtimeFlags & 0x302U) == 0 && object.scriptSlot < kScntScriptSlotCount) {
                g_scntWord24[ScenWordSlotIndex(object)] = static_cast<std::uint16_t>(cursor - object.scriptStart);
                object.relocatedPtrA8 = nullptr;
            }
            object.runtimeFlags |= 0x800000U;
            opcode = readByte();
            break;
        case 0xc0:
            switch (opcode & 0xfU) {
            case 1:
                scnf->scntVariable18 = static_cast<std::uint8_t>(readByte());
                break;
            case 2:
                scnf->scntVariable19 = static_cast<std::uint8_t>(readByte());
                break;
            case 3:
                object.byte0f = static_cast<std::uint8_t>(readByte());
                break;
            case 4:
                object.byte10 = static_cast<std::uint8_t>(readByte());
                break;
            case 5:
                object.byte11 = static_cast<std::uint8_t>(readByte());
                break;
            case 6:
                object.byte12 = static_cast<std::uint8_t>(readByte());
                break;
            case 7:
                object.field14 = static_cast<std::int8_t>(readByte());
                break;
            default:
                break;
            }
            ++cursor;
            opcode = readByte();
            break;
        case 0xd0: {
            const std::uint32_t value = ((opcode & 0xfU) << 16U) + std::to_integer<std::uint8_t>(cursor[1]) + (static_cast<std::uint32_t>(readByte()) << 8U);
            cursor += ScenSignExtend(value, 0x80000U) + 2;
            opcode = readByte();
            break;
        }
        case 0xe0: {
            const std::int32_t branchCount = static_cast<std::int32_t>(readByte());
            ++cursor;
            if (opcode != 0xe0U) {
                result = cursor - 2;
                break;
            }
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            std::int32_t branch = static_cast<std::int32_t>(
                static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125) * static_cast<float>(branchCount));
            const std::int32_t randomIndex = g_scenRuntimeState.randomBranchCount;
            if (randomIndex < 3) {
                if (g_scenRuntimeState.randomBranchValues[static_cast<std::size_t>(randomIndex)] == branch)
                    branch = branch ? branch - 1 : branchCount - 1;
                g_scenRuntimeState.randomBranchValues[static_cast<std::size_t>(randomIndex)] = branch;
                ++g_scenRuntimeState.randomBranchCount;
            }
            for (; branch; --branch)
                cursor += (std::to_integer<std::uint8_t>(cursor[1]) | ((readByte() & 0xfU) << 8U)) + 2;
            opcode = readByte();
            break;
        }
        case 0xf0:
            if (opcode == 0xfcU) {
                entry->time = currentTime;
                entry->command = 0xfc00U;
                entry->payload = nullptr;
                ++entry;
                opcode = readByte();
                break;
            }
            if (opcode == 0xf6U || opcode == 0xf7U || opcode == 0xf8U) {
                cueFlags |= 4U;
            } else if (opcode == 0xf0U) {
                cursor = DecodeScenExpression(cursor, object);
                cueFlags = savedCueFlags;
                matrixCueSet = savedMatrixCue != 0;
                opcode = readByte();
                break;
            } else if (opcode == 0xf1U || opcode == 0xfdU) {
                opcode = readByte();
                break;
            } else if (opcode == 0xf9U) {
                if ((object.runtimeFlags & 0x100U) != 0) {
                    cursor = object.savedInstructionPtr;
                    object.runtimeFlags &= ~0x100U;
                    object.savedInstructionPtr = nullptr;
                    opcode = readByte();
                    break;
                }
                if (std::byte *const alternate = object.alternateInstructionPtr) {
                    object.alternateInstructionPtr = nullptr;
                    cursor = alternate;
                    opcode = readByte();
                    break;
                }
            }
            result = cursor - 1;
            break;
        default:
            result = cursor - 1;
            break;
        }
    }
    scnf->scntVariable18 = 0;
    scnf->scntVariable19 = 0;
    cue->flags = cueFlags | 1U;
    cue->size = 2;
    cue->kind = 0;
    cue->variant = 0;
    cue->durationSeconds = terminalDuration;
    cue->actor0TaskDataOffset = 0;
    cue->actor1TaskDataOffset = 0;
    entry->time = currentTime;
    entry->payload = nullptr;
    entry->command = 0;
    entry->secondaryActor = 0;
    g_scenRuntimeState.referencedCharacterIds[static_cast<std::size_t>(g_scenRuntimeState.referencedCharacterCount)] = 0;
    g_scenRuntimeState.referencedObjects[static_cast<std::size_t>(g_scenRuntimeState.referencedObjectCount)] = nullptr;
    if ((g_scntConversationCueWorkbuf.front().flags & 1U) == 0 && (object.runtimeFlags & 2U) == 0)
        scnf->flags |= kScenarioFlagConversationMarkers;
    return result;
}



void scnfGenerateLipCues(const ScenRuntimeObject &object) {
    const ScenLineEntry *const line = static_cast<const ScenLineEntry *>(object.activeLineOrMarker);
    std::int32_t count = 0;
    if (line->delaySeconds == 0.0F) {
        const char *const text = static_cast<const char *>(object.fileHeader) + static_cast<std::int32_t>(line->textOffset);
        std::int32_t length = 0;
        while (text[length])
            ++length;
        count = length / 4;
    } else {
        count = static_cast<std::int32_t>(line->delaySeconds * 60.0F * 0.125F);
    }
    if (count > 38)
        count = 38;
    std::array<FaceLipCue, 40> &cues = g_scenRuntimeState.generatedLipCues;
    std::int32_t terminal = 1;
    if (count > 1) {
        std::uint32_t seed = g_EnvPrngSeed;
        for (terminal = 1; terminal < count; ++terminal) {
            cues[static_cast<std::size_t>(terminal)].field02 = 2;
            cues[static_cast<std::size_t>(terminal)].duration = 8;
            seed = LcgStep(seed);
            cues[static_cast<std::size_t>(terminal)].viseme =
                static_cast<std::int16_t>(static_cast<float>(static_cast<double>((seed >> 16U) & 0x7fffU) * 0.000030517578125) * 6.0F);
        }
        g_EnvPrngSeed = seed;
    }
    cues[0].viseme = 0;
    cues[0].duration = 10;
    cues[static_cast<std::size_t>(terminal)].viseme = -1;
}


bool SCNT_IsLineVoicePlaying() {
    return g_pAudio && DialogueVoiceIsPlaying(g_pAudio->audioContext);
}

} // namespace shenmue::s1::scen_detail
