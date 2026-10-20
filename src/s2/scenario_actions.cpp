#include "s2/scenario_actions.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {



// @verified - p
void ResetScenarioFlags() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    talk.ryoAcknowledged = 0;
    talk.previousPlayerTurnTarget = {};
    talk.speakerHeadHeight = 0.0f;
    talk.speakerChestHeight = 0.0f;
    talk.lookActive = 0;
    talk.lookCameraState = 0;
    talk.lookTargetReleased = 0;
    talk.faceForwardPending = 0;
    talk.lookAcknowledged = 0;
    talk.lookReleaseRequested = 0;
    talk.lookLocationPending = 0;
    talk.letterVisible = 0;
    talk.letterHidePending = 0;
    talk.letterShowPending = 0;
    talk.playerTurnPending = 0;
    talk.posesCaptured = 0;
    talk.unused0d = 0;
    talk.lastTurnAction = -1;
    for (std::size_t index = 0; index < 8; ++index) {
        talk.actionParticipants[index] = {};
        talk.actionParticipants[8 + index] = {};
        talk.participantMotionIds[index] = -1;
        talk.participantWorldYaws[index] = 0;
        talk.participantEntityYaws[index] = 0;
        talk.facingApplied[index] = 0;
        talk.lookReleasePending[index] = 0;
        talk.participantTurnIssued[index] = 0;
        talk.turnPending[index] = 0;
        talk.participantFacing[index] = 0;
    }
}


// @verified - p
void CaptureScenarioSpeakerPose() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    for (std::size_t index = 0; index < 8; ++index) {
        Character *const participant = ResolveScenarioTalkCharacter(talk.actionParticipants[index]);
        if (!participant)
            continue;
        MotionWork *const motion = FindMotionWork(participant);
        if (!motion)
            continue;
        talk.participantMotionIds[index] = GetMotionAnimationId(motion, MotionRegion::LowerBody);
        talk.participantWorldYaws[index] = shenGetRotYFromNode(participant);
        talk.participantEntityYaws[index] = static_cast<std::uint32_t>(motion->runtime.entityRotation.y);
    }
    if (Character *const speaker = ResolveScenarioTalkCharacter(talk.actionCharacter))
        GetCharacterMotionWorldPosition(speaker, 5, &talk.speakerPosition);
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (Character *const ryo = static_cast<Character *>(control->ryo))
        GetCharacterMotionWorldPosition(ryo, 5, &talk.playerPosition);
    if (Character *const speaker = ResolveScenarioTalkCharacter(talk.actionCharacter)) {
        Vector3 position{};
        GetCharacterMotionWorldPosition(speaker, 22, &position);
        talk.speakerHeadHeight = position.y;
        GetCharacterMotionWorldPosition(speaker, 27, &position);
        talk.speakerChestHeight = position.y;
    }
}


void LookAtScenarioRecordLocation(std::int32_t action, shen_name character) {
    if (!ResolveScenarioTalkCharacter(character))
        return;
    const std::int32_t location = static_cast<std::uint32_t>(action - 20) <= 9U ? action - 20 :
        static_cast<std::uint32_t>(action - 217) <= 0x1dU                       ? action - 207 :
                                                                                  action - 120;
    const ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(character);
    if (!record)
        return;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    talk.lookLocation =
        location < 0 || location >= record->locationCount ? &scenarioTalkDefaultLocation : reinterpret_cast<const Vector3 *>(record->locations) + location;
    RequestNpcSpecialMotionLookTarget(character, talk.lookLocation);
    SetEyeToChara(ResolveScenarioTalkCharacter(character), talk.lookLocation, 16, 2);
    talk.lookLocationPending = 1;
    talk.lookActive = 1;
    talk.lookTargetActive = 1;
}


void TurnScenarioPlayerTowardSpeaker() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    if (!talk.posesCaptured) {
        CaptureScenarioSpeakerPose();
        talk.posesCaptured = 1;
    }
    if (talk.playerTurnFrameLimit == -1)
        talk.playerTurnFrameLimit = 20;
    shen_name target = talk.playerTurnTarget;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (target.name != talk.previousPlayerTurnTarget.name && talk.previousPlayerTurnTarget.name) {
        if (Character *const ryo = static_cast<Character *>(control->ryo)) {
            Vector3 current{}, previous{};
            SolveRyoMatrix(ryoName, target, static_cast<std::uint16_t>(shenGetRotYFromNode(ryo)), &current);
            SolveRyoMatrix(ryoName, talk.previousPlayerTurnTarget, static_cast<std::uint16_t>(shenGetRotYFromNode(ryo)), &previous);
            target = talk.playerTurnTarget;
            if (previous.z * current.z >= 0.0f) {
                talk.playerTurnAround = 0;
                talk.previousPlayerTurnTarget = {};
            } else {
                talk.playerTurnAround = 1;
            }
        }
    }
    if (talk.playerTurnFrames == static_cast<std::uint8_t>(talk.playerTurnFrameLimit)) {
        talk.playerTurnPending = 0;
        talk.playerTurnFrames = 0;
        talk.playerTurnFrameLimit = -1;
        talk.previousPlayerTurnTarget = target;
        return;
    }
    if (talk.playerTurnAround) {
        if (Character *const speaker = ResolveScenarioTalkCharacter(target)) {
            Vector3 position{};
            GetCharacterMotionWorldPosition(speaker, 5, &position);
            plymSetSpecialModeTargetPosition(&position, talk.playerFacingMode ? 2 : 0);
            talk.playerFacingMode = 0;
            SetEyeToChara(static_cast<Character *>(control->ryo), &position, 16, 2);
            talk.playerTurnPending = 0;
            talk.playerTurnFrames = 0;
            talk.playerTurnAround = 0;
            return;
        }
        target = talk.playerTurnTarget;
    }
    ++talk.playerTurnFrames;
    if (Character *const speaker = ResolveScenarioTalkCharacter(target)) {
        Vector3 position{};
        GetCharacterMotionWorldPosition(speaker, 5, &position);
        talk.speakerPosition = position;
        talk.playerTurnPending = 0;
        talk.playerTurnFrames = 0;
        talk.playerTurnFrameLimit = -1;
        plymSetSpecialModeTargetPosition(&position, talk.playerFacingMode ? 2 : 0);
        talk.playerFacingMode = 0;
        SetEyeToChara(static_cast<Character *>(control->ryo), &position, 16, 2);
    }
}


void FaceScenarioParticipantsForward() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    for (std::size_t index = 0; index < 8; ++index) {
        if (talk.facingApplied[index] || ScenarioCharacterIsRyo(talk.actionParticipants[index]))
            continue;
        Character *const character = ResolveScenarioTalkCharacter(talk.actionParticipants[index]);
        if (!character)
            continue;
        Vector3 position{};
        GetCharacterMotionWorldPosition(character, 5, &position);
        const auto yaw = static_cast<std::uint16_t>(shenGetRotYFromNode(character));
        position.x -= FixedAngleSin(yaw);
        position.z -= FixedAngleCos(yaw);
        RequestNpcSpecialMotionTarget(talk.actionParticipants[index], &position);
        talk.facingApplied[index] = 1;
    }
    talk.faceForwardPending = 0;
}


void ReleaseScenarioParticipantLook(shen_name character) {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    if (talk.lookActive) {
        talk.lookReleaseRequested = 1;
        talk.lookReleaseCharacter = character;
        talk.lookActive = 0;
    }
    const Vector3 zero{};
    for (std::size_t index = 0; index < 8; ++index) {
        if (talk.actionParticipants[index].name != character.name)
            continue;
        if (!talk.facingApplied[index])
            return;
        talk.lookReleaseNames[index] = character;
        talk.lookReleasePending[index] = 1;
        ResetNpcSpecialMotionTarget(character);
        SetEyeToChara(ResolveScenarioTalkCharacter(character), &zero, 16, 0);
        talk.facingApplied[index] = 0;
    }
}


void TurnScenarioParticipantToward(std::size_t slot) {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    if (!talk.posesCaptured) {
        CaptureScenarioSpeakerPose();
        talk.posesCaptured = 1;
    }
    std::size_t turned = 0;
    for (std::size_t index = 0; index < 8; ++index) {
        if (talk.turnSources[slot].name == talk.actionParticipants[index].name)
            turned = index;
    }
    Vector3 position{};
    if (Character *const target = ResolveScenarioTalkCharacter(talk.turnTargets[slot]))
        GetCharacterMotionWorldPosition(target, 5, &position);
    talk.participantTurnIssued[slot] = 1;
    RequestNpcSpecialMotionTarget(talk.turnSources[slot], &position);
    SetEyeToChara(ResolveScenarioTalkCharacter(talk.turnSources[slot]), &position, 16, 2);
    talk.facingApplied[turned] = 1;
    if (talk.lookActive) {
        talk.lookReleaseCharacter = talk.turnSources[slot];
        talk.lookReleaseRequested = 1;
        talk.lookActive = 0;
    }
    talk.turnPending[slot] = 0;
}


void RetireScenarioTalkParticipants(shen_name character) {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    const Vector3 zero{};
    for (std::int32_t index = 0; index < talk.participantCount; ++index) {
        const auto slot = static_cast<std::size_t>(index);
        shen_name participant{};
        std::memcpy(&participant, reinterpret_cast<const std::byte *>(&talk) + slot * sizeof(participant), sizeof(participant));
        if (participant.name) {
            if (!ScenarioCharacterIsRyo(participant)) {
                const std::uint8_t facing = reinterpret_cast<const std::uint8_t *>(&talk)[offsetof(ScenarioTalkPresentationState, facingApplied) + slot];
                if (facing)
                    ResetNpcSpecialMotionTarget(participant);
                EndNpcSpecialMotion(participant);
            }
            Character *const node = ResolveScenarioTalkCharacter(participant);
            const bool actionMode = (activeScenarioEventRuntime->flags & 0x4000U) != 0;
            if (!actionMode || node) {
                if (actionMode)
                    UpdateMotionWorkWithPosition(FindMotionWork(node), nullptr, 0);
                if (node) {
                    SetEyeToChara(node, &zero, 16, 0);
                    if (ScenarioCharacterIsRyo(participant))
                        SetFaceFlagsForChara_0(node, FacialNodeOriginal);
                    SetHighModelAndFaceFlagsForChara(node, Normal, 10.0f, 1.0f);
                    SetFaceFlagsForChara(node, 1);
                    AssignFacialClip(node, SCEN_DEFAULT_FACE_SEQUENCE.data());
                    SetCharacterHandRegionEnabled(node, 0, false, -1);
                    SetCharacterHandRegionEnabled(node, 1, false, -1);
                }
            }
        }
        const std::uint32_t cleared = 0;
        std::memcpy(reinterpret_cast<std::byte *>(&talk) + slot * sizeof(cleared), &cleared, sizeof(cleared));
        std::memcpy(
            reinterpret_cast<std::byte *>(&talk) + offsetof(ScenarioTalkPresentationState, faceLooks) + slot * sizeof(cleared), &cleared, sizeof(cleared));
#if SHENMUE_IS_PC
        if (slot >= 8) {
#if defined(_MSC_VER)
            __fastfail(8);
#else
            std::abort();
#endif
        }
#endif
        talk.participantFacing[slot] = 0;
        talk.facingApplied[slot] = 0;
    }
    talk.participantCount = 0;
    plymEndSpecialMode(0);
    talk.suppressNpcFacing = 0;
    talk.suppressPlayerFacing = 0;
    talk.playerFacingMode = 0;
    talk.npcFacingIssued = 0;
    talk.playerFacingIssued = 0;
    talk.facingCommandsIssued = 0;
    talk.unfocusedFacingRequested = 0;
    if (talk.letterVisible) {
        if (Character *const letter = FindScenarioCharacter(activeScenarioEventRuntime->task, stepVisibleCharacterName)) {
            HLib::Task *const task = GetCharacterHandlerTask(letter);
            charaSetFlags(letter, 0);
            if (task)
                HLib::SetupTimedTask(task, 0);
            EV_SetCharDisplay(letter, CVM_DisplayEnabled, 0);
        }
        talk.letterVisible = 0;
    }
    ResetScenarioFlags();
    AcknowledgeScenarioCharacterAction(character, -8);
}


void SetModePerCurrentStep(std::int32_t mode) {
    Character *const letter = FindScenarioCharacter(activeScenarioEventRuntime->task, stepVisibleCharacterName);
    if (!letter)
        return;
    if (mode < 0) {
        const std::uint32_t step = GetCurrentStep();
        const std::int32_t selected = GetScenarioTalkPresentationState().selectedFollowFlag;
        if (step < 50000)
            mode = 0;
        else if (step >= 110000 && step < 120000)
            mode = 2;
        else if (step >= 360000)
            mode = step < 370000 ? 6 : 1;
        else if (step >= 350000)
            mode = selected == 347 ? 3 : selected == 348 ? 4 : selected == 349 ? 5 : 1;
        else
            mode = 1;
    }
    SetScenarioStepModel(letter, mode);
}



void scenProcessCharacterActionQueue() {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::size_t index = 0; index < 8; ++index) {
        if (talk.lookReleasePending[index] && ResolveScenarioTalkCharacter(talk.lookReleaseNames[index]))
            talk.lookReleasePending[index] = 0;
    }
    if (talk.lookLocationPending && ++talk.lookLocationFrames >= 20) {
        talk.lookLocationPending = 0;
        talk.lookLocationFrames = 0;
        if (talk.lookTargetReleased) {
            talk.lookCameraState = -1;
            talk.lookTargetReleased = 0;
        } else {
            talk.lookCameraState = 1;
            camCtrlSetConversationFlag4000(1);
        }
        const Vector3 *const location = talk.lookLocation;
        plymSetSpecialModeTargetPosition(location, 1);
        talk.playerFacingMode = 0;
        SetEyeToChara(static_cast<Character *>(control->ryo), location, 16, 2);
    }
    if ((scenario.flags & 0x4000U) && talk.participants[0].name && talk.participants[1].name) {
        Character *const first = ResolveScenarioTalkCharacter(talk.participants[0]);
        Character *const second = ResolveScenarioTalkCharacter(talk.participants[1]);
        if (first && second) {
            if (MotionWork *const motion = FindMotionWork(first)) {
                Vector3 position{};
                if (talk.participantFacing[0] > 0) {
                    GetCharacterMotionWorldPosition(second, 5, &position);
                    UpdateMotionWorkWithPosition(motion, &position, 770);
                }
                if (talk.participantFacing[0] < 0) {
                    UpdateMotionWorkWithPosition(motion, nullptr, 0);
                    talk.participantFacing[0] = 0;
                }
            }
            if (MotionWork *const motion = FindMotionWork(second)) {
                Vector3 position{};
                if (talk.participantFacing[1] > 0) {
                    GetCharacterMotionWorldPosition(first, 5, &position);
                    UpdateMotionWorkWithPosition(motion, &position, 770);
                }
                if (talk.participantFacing[1] < 0) {
                    UpdateMotionWorkWithPosition(motion, nullptr, 0);
                    talk.participantFacing[1] = 0;
                }
            }
        }
    }
    if (talk.playerTurnPending)
        TurnScenarioPlayerTowardSpeaker();
    if (talk.letterShowPending) {
        if (++talk.letterFrames >= 25) {
            talk.letterShowPending = 0;
            talk.letterFrames = 0;
            Character *const ryo = static_cast<Character *>(control->ryo);
            Character *const letter = FindScenarioCharacter(scenario.task, stepVisibleCharacterName);
            if (letter && ryo) {
                HLib::Task *const task = GetCharacterHandlerTask(letter);
                charaSetFlags(letter, 1);
                if (task)
                    HLib::ResolveDeferredCallback(task);
                EV_SetCharDisplay(letter, CVM_DisplayEnabled, 1);
                talk.letterVisible = 1;
                if (IsOfThisVisibilityType(letter, 3))
                    SetFixoDataForMotionWork(letter, ryo, 14, {0.17f, 0.050000001f, -0.029999999f}, {16384, 25486, 16384});
                SetModePerCurrentStep(-1);
            }
        }
    } else if (talk.letterHidePending && ++talk.letterFrames >= 25) {
        talk.letterHidePending = 0;
        talk.letterFrames = 0;
        if (Character *const letter = FindScenarioCharacter(scenario.task, stepVisibleCharacterName)) {
            HLib::Task *const task = GetCharacterHandlerTask(letter);
            charaSetFlags(letter, 0);
            if (task)
                HLib::SetupTimedTask(task, 0);
            EV_SetCharDisplay(letter, CVM_DisplayEnabled, 0);
            talk.letterVisible = 0;
        }
    }
    if (talk.lookReleaseRequested && ++talk.lookReleaseFrames >= 30) {
        talk.lookReleaseRequested = 0;
        talk.lookReleaseFrames = 0;
        if (ResolveScenarioTalkCharacter(talk.lookReleaseCharacter)) {
            ResetNpcSpecialMotionLookTarget(talk.lookReleaseCharacter);
            const Vector3 zero{};
            SetEyeToChara(ResolveScenarioTalkCharacter(talk.lookReleaseCharacter), &zero, 16, 0);
            talk.lookActive = 0;
        }
    }
    for (std::size_t slot = 0; slot < 10; ++slot) {
        if (talk.turnPending[slot])
            TurnScenarioParticipantToward(slot);
    }
    if (talk.faceForwardPending)
        FaceScenarioParticipantsForward();
    ScenarioAction queued{};
    while (ReadScenarioCharacterAction(queued) >= 1) {
        const shen_name character = queued.name;
        const auto action = static_cast<std::uint32_t>(queued.action);
        talk.actionCharacter = character;
        if (FindMutableScenarioFreeTalkRecord(character)) {
            for (std::size_t index = 0; index < 8 && talk.participants[index].name; ++index)
                talk.actionParticipants[index] = talk.participants[index];
        }
        switch (queued.action) {
        case -16:
            if (!(scenario.flags & 0x4000U) && talk.unfocusedFacingRequested)
                talk.faceForwardPending = 1;
            talk.unfocusedFacingRequested = 0;
            break;
        case -8:
            RetireScenarioTalkParticipants(character);
            break;
        case -2:
            talk.lookAcknowledged = 0;
            if (talk.lookActive) {
                talk.lookReleaseRequested = 1;
                talk.lookReleaseCharacter = character;
                talk.lookActive = 0;
            }
            AcknowledgeScenarioCharacterAction(character, -2);
            break;
        case -1:
            if (control->ryo) {
                if (!talk.ryoAcknowledged)
                    talk.ryoAcknowledged = 1;
                AcknowledgeScenarioCharacterAction(character, -1);
            }
            break;
        default:
            break;
        }
        if (action - 40U <= 9U) {
            talk.npcFacingIssued = 1;
            talk.facingCommandsIssued = 1;
            if (scenario.flags & 0x4000U) {
                if (action == 40U)
                    talk.participantFacing[1] = 1;
            } else {
                const std::int32_t pair = static_cast<std::int32_t>(action) - 41;
                std::int32_t source = 1, target = 0;
                if (pair >= 0) {
                    source = pair / 3 + 1;
                    target = pair % 3;
                    if (source <= target)
                        ++target;
                }
                talk.lastTurnAction = static_cast<std::int32_t>(action);
                talk.turnSource = talk.actionParticipants[static_cast<std::size_t>(source)];
                talk.turnTarget = talk.actionParticipants[static_cast<std::size_t>(target)];
                talk.turnPending[action - 40U] = 1;
                talk.turnSources[action - 40U] = talk.turnSource;
                talk.turnTargets[action - 40U] = talk.turnTarget;
            }
        } else if (action - 50U <= 9U) {
            const std::size_t participant = action - 49U;
            if (scenario.flags & 0x4000U) {
                // Actions 57-59 write past the eight pin states into the first look-release name.
                const std::int8_t released = -1;
                std::memcpy(reinterpret_cast<std::byte *>(&talk) + offsetof(ScenarioTalkPresentationState, participantFacing) + participant, &released,
                    sizeof(released));
            } else {
                ReleaseScenarioParticipantLook(talk.actionParticipants[participant]);
            }
        } else if (action - 140U <= 9U) {
            const auto participant = static_cast<std::int8_t>(action - 139U);
            talk.playerFacingIssued = 1;
            talk.facingCommandsIssued = 1;
            if (scenario.flags & 0x4000U) {
                talk.participantFacing[0] = participant;
            } else {
                talk.playerTurnPending = 1;
                talk.facingApplied[0] = 1;
                talk.playerTurnTarget = talk.actionParticipants[static_cast<std::size_t>(participant)];
            }
        } else if (action == 203U) {
            if (scenario.flags & 0x4000U) {
                talk.participantFacing[0] = -1;
            } else {
                UpdateMotionWorkWithPosition(FindMotionWork(static_cast<Character *>(control->ryo)), nullptr, 0);
                talk.facingApplied[0] = 0;
            }
        } else if (action - 20U <= 9U || action - 217U <= 0x1dU || action - 120U <= 9U) {
            LookAtScenarioRecordLocation(static_cast<std::int32_t>(action), talk.actionCharacter);
        } else {
            switch (action) {
            case 13U:
                talk.lookReleaseCharacter = talk.actionCharacter;
                talk.lookReleaseRequested = 1;
                talk.lookActive = 0;
                talk.lookCameraState = 0;
                camCtrlSetConversationFlag4000(0);
                break;
            case 10U:
                if (!talk.letterVisible) {
                    talk.letterShowPending = 1;
                    if (talk.actionCharacter.name)
                        plymBeginSpecialTargetMotionSequence(stepVisibleCharacterName);
                }
                break;
            case 15U:
                if (talk.letterVisible) {
                    talk.letterHidePending = 1;
                    if (talk.actionCharacter.name)
                        plymContinueSpecialTargetMotionSequence();
                }
                break;
            case 200U:
                talk.suppressNpcFacing = 1;
                break;
            case 201U:
                talk.suppressPlayerFacing = 1;
                break;
            case 202U:
                talk.lookTargetReleased = 1;
                break;
            case 205U:
                ExecuteAicaCommand(0x800ab, 0, 0);
                break;
            default:
                if (action - 207U > 9U) {
                    if (action == 16U)
                        talk.unfocusedFacingRequested = 1;
                } else {
                    plymRequestSpecialPoseSequence();
                }
                break;
            }
        }
        AcknowledgeScenarioCharacterAction(talk.actionCharacter, -10);
    }
}



std::int64_t QueueScenarioSpeakerFacing(ScenarioFreeTalkRecord &record) {
    if (record.requestedRoot)
        return 0;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    std::int64_t queued = 0;
    if (!talk.suppressPlayerFacing && !talk.playerFacingIssued) {
        QueueScenarioCharacterAction(record.name, 140);
        queued = 1;
    }
    if (!talk.suppressNpcFacing && !talk.npcFacingIssued) {
        QueueScenarioCharacterAction(record.name, 40);
        queued = 1;
    }
    if (talk.unfocusedFacingRequested) {
        QueueScenarioCharacterAction(record.name, -16);
        queued = 1;
    } else if (!queued) {
        return 0;
    }
    record.flags |= ScenarioTalkWaitAction;
    record.timers[3] = 0;
    activeScenarioEventRuntime->flags &= ~0x10U;
    record.executionState = 7;
    return queued;
}


void EndScenarioTalkSession(ScenarioFreeTalkRecord &record) {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    record.flags &= ~ScenarioTalkPresentationActive;
    record.flags |= ScenarioTalkWaitFollow;
    record.timers[1] = 0;
    talk.talkEndRequest = -1;
    QueueScenarioCharacterAction(record.name, -8);
    ConsumeScenarioCameraRestoreRequest();
    ReleaseScenarioTalkInput();
    if (talk.followRequest < 0) {
        record.control->flags |= 4U;
        record.control->flags |= 1U;
        record.control->flags &= ~2U;
        record.control->flags &= ~8U;
        record.control->flags &= ~0x10U;
    }
}



void SetFaceXFlags(ScenarioFreeTalkRecord &record) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    if (!(scenario.flags & 0x800U)) {
        talk.suppressNpcFacing = 0;
        talk.suppressPlayerFacing = 0;
        talk.playerFacingMode = 0;
        talk.npcFacingIssued = 0;
        talk.playerFacingIssued = 0;
        talk.facingCommandsIssued = 0;
        talk.unfocusedFacingRequested = 0;
    }
    talk.participants = {};
    talk.faceLooks = {};
    if (!plymIsSearchBlocked()) {
        scenario.flags |= 0x200U;
        plymBeginSpecialMode();
    }
    scenario.flags = (scenario.flags & 0xfffff6ffU) | 0x800U;
    std::int64_t participantCount = (reinterpret_cast<const std::byte *>(record.voicePath) - record.participants) / 4;
    record.flags &= ~(ScenarioTalkAllowVoiceOverlap | ScenarioTalkPresentationActive | ScenarioTalkPromptClockStarted);
    record.selectionIndex = -1;
    record.pendingScriptCursor = nullptr;
    std::int32_t index = 0;
    for (; index < participantCount; ++index) {
        const shen_name name = ResolveScenarioParticipant(ScenarioParticipants(record)[index], record);
        std::memcpy(reinterpret_cast<std::byte *>(&talk) + sizeof(shen_name) * index, &name, sizeof(name));
        if (!name.name)
            continue;
        if (Character *const character = ResolveScenarioTalkCharacter(name)) {
            SetFaceFlagsForChara_0(character, FacialNodeReplacement);
            SetFaceFlagsForChara(character, 0);
            SetHighModelAndFaceFlagsForChara(character, Normal, 10.0f, 1.0f);
            AssignFacialClip(character, SCEN_DEFAULT_FACE_SEQUENCE.data());
            SetCharacterHandRegionEnabled(character, 0, true, -1);
            HandParams *const left = GetCharacterHand(character, 1);
            SetCharacterHandRegionEnabled(character, 1, true, -1);
            if (ScenarioCharacterIsRyo(name) && left && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 123))
                ParseHandPoseCommand(left, reinterpret_cast<std::byte *>(scenarioTalkHandPoseCommand), 0);
        }
        if (!ScenarioCharacterIsRyo(name) && !(scenario.flags & 0x4000U))
            BeginNpcSpecialMotion(name);
    }
    talk.participantCount = index;
    for (std::int32_t queued = 0; queued < talk.queuedFaceLookCount; ++queued)
        ApplyScenarioFaceLook(talk.queuedFaceLooks[static_cast<std::size_t>(queued)]);
    talk.queuedFaceLookCount = 0;
    talk.followRequest = 0;
    talk.lookTargetActive = 0;
    talk.selectedFollowFlag = -1;
    if (talk.followRouteWait) {
        SetScenarioFollowCharacter({});
        talk.followRouteWait = 0;
    }
    if (!GetScenarioFollowEligibility(record.name))
        SetScenarioFollowCharacter({});
}
} // namespace event_runtime_detail


} // namespace shenmue::s2
