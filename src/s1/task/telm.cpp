#include "s1/task/telm.hpp"

#include "s1/boot.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/initialization.hpp"
#include "s1/input.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/motion_registry.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/player_control.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/srch.hpp"
#include "s1/timing.hpp"
#include "s1/steam.hpp"
#include "s1/ui_runtime.hpp"

#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s1 {

extern void *g_cameraTargetEntity;
extern void *g_shenmueCamera0;
extern std::uint32_t g_searchLockOnFlags;
extern std::int32_t g_searchAnyInputPressedLatched;
extern std::int32_t g_searchAPressedLatched;

std::array<char, 25> g_telephoneDisplayedNumber{};
std::int32_t g_telmIsActive = 0;
Character *g_telmRetailProbeCharacter = nullptr;
TelephoneWork *g_telmRetailProbeWork = nullptr;

namespace {

constexpr std::uint32_t kTelmComponent = SHEN_NAME(TELM);
constexpr std::int32_t kTelmReceiverTransformToken = static_cast<std::int32_t>(SHEN_NAME(TELT));
constexpr std::int32_t kTelmCameraPoolToken = static_cast<std::int32_t>(SHEN_NAME(TELR));
constexpr std::int32_t kTelmOwnerPoolToken = static_cast<std::int32_t>(SHEN_NAME(TELL));
constexpr std::int32_t kSplineToken = static_cast<std::int32_t>(SHEN_NAME(SPLI));
constexpr std::uint32_t kTelmAuthSequenceFlags = 0x606660U;
constexpr std::size_t kSubtitleUiIndex = 0x3a;
constexpr std::size_t kScenarioWindowOffset = 0x98;
constexpr std::size_t kScenarioDialogueFlagsOffset = 0xd8;
constexpr std::size_t kSplineCoefficientStride = 76;
constexpr float kFixedAnglePerDegree = 182.04445f;
constexpr double kPrngUnit = 3.0517578125e-05;
constexpr char kDialDigits[] = "0987654321";

constexpr std::int32_t kHandCueTable[] = {587, 15, -1, -1, -1, 584, 24, -1, -1, -1};

// Shared Script_NoOp nullsub installed as the TELM destroy callback.
void Script_NoOp(Character *, void *) {
}

std::uint32_t NextEnvironmentSeed(std::uint32_t seed) {
    return LcgStep(seed);
}

float EnvironmentRandom(std::uint32_t seed) {
    return static_cast<float>(static_cast<double>((seed >> 16U) & 0x7fffU) * kPrngUnit);
}

std::uint16_t TelmPressed(const TelephoneWork &phone) {
    return phone.padInput ? phone.padInput->pressedPriority : 0;
}

std::uint16_t TelmHeld(const TelephoneWork &phone) {
    return phone.padInput ? phone.padInput->held : 0;
}

std::uint32_t &ScenarioDialogueFlags(const TelephoneWork &phone) {
    auto *const data = reinterpret_cast<std::byte *>(phone.scenarioTask->task_data_ptr);
    return *reinterpret_cast<std::uint32_t *>(data + kScenarioDialogueFlagsOffset);
}

std::byte *NodeOffsetBase(const Mt5RuntimeNode *node) {
    return HMDL_NodeOffsetsUseTaskData(node) ? g_pCurrentTaskData : MT5_ModelBaseForNode(node);
}

void CloseSubtitleElement() {
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
    if ((g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) &&
        (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
        subtitles.isVisible = 0;
    }
}

void ShowDialPrompt() {
    if (UI_SICNIsTaskActive())
        UI_SICNSetState(0x24U, UISicnPausePolicy::KeepOnPause);
    else
        UI_SICNEnqueue(0x24U, UISicnPausePolicy::KeepOnPause);
}

std::int32_t TelmDialAngle(std::int32_t index) {
    return static_cast<std::int32_t>(static_cast<double>(index) * 24.0 * 8192.0 / 45.0);
}

bool SettleTelmDial(std::int32_t &yaw, std::int32_t angle, std::int32_t &velocity) {
    std::int32_t distance = static_cast<std::int16_t>(yaw) - static_cast<std::int16_t>(angle);
    if (distance < 0)
        distance = -distance;
    std::int32_t speed = static_cast<std::int16_t>(velocity);
    if ((speed & 0x8000) != 0)
        speed = -speed;
    if (distance >= speed)
        return false;
    yaw = static_cast<std::uint16_t>(angle);
    velocity = 0;
    return true;
}

void UpdateRingCadence(TelephoneWork &phone) {
    if (phone.ringAudioPhase) {
        if (phone.ringAudioTimer == 1.0f) {
            shenExecuteAICACommand(0x605abU, 0, 0);
            phone.audioResetLatched = 1;
        } else if (phone.ringAudioTimer > 18.0f) {
            phone.ringAudioPhase = 0;
            phone.ringAudioTimer = 0.0f;
        }
    } else if (phone.ringAudioTimer == 1.0f) {
        shenExecuteAICACommand(0x705abU, 0, 0);
        phone.audioResetLatched = 0;
    } else if (phone.ringAudioTimer > 15.0f) {
        phone.ringAudioPhase = 1;
        phone.ringAudioTimer = 0.0f;
    }
    phone.ringAudioTimer = phone.ringAudioTimer + 1.0f;
}

Vector3 WorkTranslation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}



Vector3 TelephoneAnchorPosition(Mt5RuntimeNode *root, const Mt5RuntimeNode *node, const Vector3 &offset, const Vector3 *secondOffset, std::int32_t *eulerX) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4 chain;
    shenChainToTargetEval(&chain, root, node);
    workTransformMatrix = chain;
    if (eulerX)
        *eulerX = CHAR_EulerAnglesFromMatrix(chain).x;
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    if (secondOffset)
        Matrix4x4::preTranslate(secondOffset->x, secondOffset->y, secondOffset->z);
    const Vector3 position = WorkTranslation();
    Matrix4x4::Pop(1);
    return position;
}

void StartSequenceHandMotion(TelmHandSequence &sequence, float blendFrames) {
    HandModelWork &hand = *sequence.hand;
    const std::uint8_t control = sequence.reverse ? 2U : 0U;
    const auto sequenceId = static_cast<std::int16_t>(sequence.sequenceId);
    if (hand.primaryRoot) {
        hand.playback.status = 1;
        hand.playback.playbackMode = 1;
        HND_SetNewData(hand, sequenceId, 0, 0, 0, 1, 1.0f, 0);
    }
    if (hand.primaryRoot)
        hand.primaryRoot->rotation = {};
    const auto startFrame = static_cast<std::int16_t>(static_cast<std::int32_t>(sequence.startFrame));
    HND_SetNewData(hand, sequenceId, startFrame, static_cast<std::int16_t>(static_cast<std::int32_t>(sequence.endFrame)), startFrame, control,
        sequence.frameStep, static_cast<std::int16_t>(static_cast<std::int32_t>(blendFrames)));
    sequence.frame = 1.0f;
}


void HandMot_ReadSequenceFCV(TelephoneWork &phone, TelmHandSequence &sequence) {
    if (sequence.state == 2) {
        if (!g_isPlayerGameStateActive)
            return;
        StartSequenceHandMotion(sequence, sequence.blendFrames);
        sequence.state = 3;
        sequence.frameCounter = 0.0f;
        sequence.firstCuePlayed = 0;
        sequence.secondCuePlayed = 0;
        return;
    }
    if (sequence.state != 3)
        return;

    HandModelWork &hand = *sequence.hand;
    if (sequence.blendPending || (hand.control & 8U) == 0) {
        if (sequence.cuesEnabled && !sequence.blendPending) {
            for (std::size_t row = 0; kHandCueTable[row] != -1; row += 4) {
                if (!sequence.firstCuePlayed && kHandCueTable[row] == sequence.sequenceId &&
                    kHandCueTable[row + 1] <= static_cast<std::int32_t>(sequence.frame)) {
                    shenExecuteAICACommand(static_cast<std::uint32_t>(kHandCueTable[row + 2]), 0, 0);
                    phone.audioResetLatched = kHandCueTable[row + 2] == 0x605ab;
                    sequence.firstCuePlayed = 1;
                }
                if (!sequence.secondCuePlayed && kHandCueTable[row] == sequence.sequenceId &&
                    kHandCueTable[row + 3] <= static_cast<std::int32_t>(sequence.frame)) {
                    shenExecuteAICACommand(static_cast<std::uint32_t>(kHandCueTable[row + 4]), 0, 0);
                    phone.audioResetLatched = kHandCueTable[row + 4] == 0x605ab;
                    sequence.secondCuePlayed = 1;
                }
            }
        }
    } else {
        if (sequence.cuesEnabled) {
            for (std::size_t row = 0; kHandCueTable[row] != -1; row += 4) {
                if (!sequence.secondCuePlayed && kHandCueTable[row] == sequence.sequenceId && kHandCueTable[row + 3] == 0) {
                    shenExecuteAICACommand(static_cast<std::uint32_t>(kHandCueTable[row + 4]), 0, 0);
                    phone.audioResetLatched = kHandCueTable[row + 4] == 0x605ab;
                    sequence.secondCuePlayed = 1;
                }
            }
        }
        if (sequence.loop) {
            StartSequenceHandMotion(sequence, sequence.restartBlendFrames);
            sequence.frameCounter = 0.0f;
            sequence.firstCuePlayed = 0;
            sequence.secondCuePlayed = 0;
        } else {
            sequence.finished = 1;
        }
    }

    Character *const target = sequence.target;
    if (!sequence.positionDone[0] || !sequence.positionDone[1] || !sequence.positionDone[2]) {
        float values[3]{};
        if (target) {
            values[0] = target->entity.node.position.x;
            values[1] = target->entity.node.position.y;
            values[2] = target->entity.node.position.z;
        }
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (!sequence.positionDone[axis] && !Sequence_ReadFCVKey(&sequence.positionReaders[axis], sequence.frameCounter, &values[axis]))
                sequence.positionDone[axis] = 1;
        }
        if (target)
            target->entity.node.position = {values[0], values[1], values[2]};
    }
    if (!sequence.rotationDone[0] || !sequence.rotationDone[1] || !sequence.rotationDone[2]) {
        float values[3]{};
        if (target) {
            values[0] = static_cast<float>(target->entity.node.rotation.x) / kFixedAnglePerDegree;
            values[1] = static_cast<float>(target->entity.node.rotation.y) / kFixedAnglePerDegree;
            values[2] = static_cast<float>(target->entity.node.rotation.z) / kFixedAnglePerDegree;
        }
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (!sequence.rotationDone[axis] && !Sequence_ReadFCVKey(&sequence.rotationReaders[axis], sequence.frameCounter, &values[axis]))
                sequence.rotationDone[axis] = 1;
        }
        if (target) {
            target->entity.node.rotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>(values[1] * 8192.0f / 45.0f));
            target->entity.node.rotation.z = static_cast<std::uint16_t>(static_cast<std::int32_t>(values[2] * 8192.0f / 45.0f));
            target->entity.node.rotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>(values[0] * 8192.0f / 45.0f));
        }
    }

    const std::int32_t frame = hand.animationFlags ? -hand.blendFrames : hand.playback.frameIndex + 1;
    sequence.frame = static_cast<float>(frame);
    if (sequence.blendPending && sequence.frame > 0.0f)
        sequence.blendPending = 0;
    sequence.frameCounter = sequence.frameCounter + 1.0f;
    if (sequence.finished && sequence.positionDone[0] && sequence.positionDone[1] && sequence.positionDone[2] && sequence.rotationDone[0] &&
        sequence.rotationDone[1] && sequence.rotationDone[2]) {
        sequence.state = 0;
    }
}

void PrepareSequenceChannel(FcvSplineChannel &channel, FcvSplineReader &reader, std::byte *pool, std::size_t poolIndex, float duration, float from, float to) {
    channel.keyCount = 2;
    channel.times[0] = 0.0f;
    channel.times[1] = duration;
    channel.values[0] = from;
    channel.values[1] = to;
    channel.slopes[0] = 0.0f;
    channel.slopes[1] = 0.0f;
    reader.times = channel.times;
    reader.keyCount = 2;
    reader.values = channel.values;
    reader.slopes = channel.slopes;
    auto *coefficients = reinterpret_cast<float *>(reinterpret_cast<std::uintptr_t>(pool) + poolIndex * kSplineCoefficientStride);
    if (!coefficients) {
        coefficients = static_cast<float *>(HLib_AllocHeapMemBlock(16, kSplineToken));
        if (!coefficients)
            return;
    }
    reader.coefficients = coefficients;
    *reinterpret_cast<std::int32_t *>(coefficients) = 0;
    reader.keyCount = 2;
    reader.keyOffset = 0;
    reader.segmentKeyCount = AllocSPLI(coefficients, 2, channel.times, channel.values, channel.slopes);
}


void TELM_StartHandSequence(TelmHandSequence &sequence, Character *target, const Vector3 *start, const Vector3 *end, float duration,
    const Vector3i *endRotation, float rotationDuration, std::int32_t sequenceId, float startFrame, float endFrame, float frameStep, float blendFrames,
    float restartBlendFrames, std::int32_t loop, std::int32_t cuesEnabled) {
    sequence.target = target;
    sequence.state = 2;
    sequence.finished = 0;
    if (sequenceId >= 0) {
        sequence.reverse = 0;
    } else {
        sequenceId = -sequenceId;
        sequence.reverse = 1;
    }
    sequence.sequenceId = sequenceId;
    sequence.loop = loop;
    sequence.startFrame = startFrame;
    sequence.endFrame = endFrame;
    sequence.frameStep = frameStep;
    sequence.blendFrames = blendFrames;
    sequence.restartBlendFrames = restartBlendFrames;
    sequence.blendPending = static_cast<std::int32_t>(blendFrames);
    sequence.cuesEnabled = cuesEnabled;

    Vector3 startPosition{};
    Vector3i startRotation{};
    if (!start) {
        if (target) {
            startPosition = target->entity.node.position;
            startRotation = target->entity.node.rotation;
        }
    } else {
        if (target) {
            target->entity.node.position = *start;
            startRotation = target->entity.node.rotation;
        }
        startPosition = *start;
    }

    if (end) {
        const float from[3]{startPosition.x, startPosition.y, startPosition.z};
        const float to[3]{end->x, end->y, end->z};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            sequence.positionDone[axis] = 0;
            PrepareSequenceChannel(
                sequence.positionChannels[axis], sequence.positionReaders[axis], sequence.coefficientPool, axis, duration, from[axis], to[axis]);
        }
    } else {
        sequence.positionDone[2] = 1;
        sequence.positionDone[1] = 1;
        sequence.positionDone[0] = 1;
    }

    if (endRotation) {
        const float from[3]{static_cast<float>(startRotation.x) / kFixedAnglePerDegree, static_cast<float>(startRotation.y) / kFixedAnglePerDegree,
            static_cast<float>(startRotation.z) / kFixedAnglePerDegree};
        const float to[3]{static_cast<float>(endRotation->x) / kFixedAnglePerDegree, static_cast<float>(endRotation->y) / kFixedAnglePerDegree,
            static_cast<float>(endRotation->z) / kFixedAnglePerDegree};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            sequence.rotationDone[axis] = 0;
            PrepareSequenceChannel(
                sequence.rotationChannels[axis], sequence.rotationReaders[axis], sequence.coefficientPool, axis + 3, rotationDuration, from[axis], to[axis]);
        }
    } else {
        sequence.rotationDone[2] = 1;
        sequence.rotationDone[1] = 1;
        sequence.rotationDone[0] = 1;
    }
}

// Re-arms the camera hand in place with a new HMOT_TEL sequence, as telmMain's dial-idle branches write the sequence fields directly.
void RearmCameraSequence(
    TelephoneWork &phone, std::int32_t sequenceId, std::int32_t reverse, float endFrame, float frameStep, float blendFrames, std::int32_t blendPending) {
    TelmHandSequence &sequence = phone.cameraSequence;
    sequence.target = phone.cameraNode;
    sequence.state = 2;
    sequence.reverse = reverse;
    sequence.finished = 0;
    sequence.sequenceId = sequenceId;
    sequence.startFrame = 0.0f;
    sequence.endFrame = endFrame;
    sequence.frameStep = frameStep;
    sequence.blendFrames = blendFrames;
    sequence.restartBlendFrames = blendFrames;
    sequence.blendPending = blendPending;
    sequence.loop = 0;
    sequence.cuesEnabled = 1;
    sequence.positionDone[0] = 1;
    sequence.positionDone[1] = 1;
    sequence.positionDone[2] = 1;
    sequence.rotationDone[0] = 1;
    sequence.rotationDone[1] = 1;
    sequence.rotationDone[2] = 1;
}


void StepCameraDialSequence(TelephoneWork &phone) {
    if (phone.cameraSequence.sequenceId == 586) {
        if (phone.cameraSequence.state)
            return;
        g_EnvPrngSeed = NextEnvironmentSeed(g_EnvPrngSeed);
        RearmCameraSequence(phone, 584, 0, EnvironmentRandom(g_EnvPrngSeed) * 6.0f + 4.0f, 0.89999998f, 30.0f, 30);
    } else {
        RearmCameraSequence(phone, 586, 0, 0.0f, 0.89999998f, 30.0f, 30);
    }
}

void authInitAndStartSequence(TelephoneWork &phone, const Vector3 &eye, const Vector3 &target, float duration) {
    TelephoneAuthSequence &sequence = phone.authSequence;
    sequence.flags = kTelmAuthSequenceFlags;
    const float eyeValues[3]{eye.x, eye.y, eye.z};
    const float targetValues[3]{target.x, target.y, target.z};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        sequence.channels[axis].values[1] = eyeValues[axis];
        sequence.channels[axis + 3].values[1] = targetValues[axis];
    }
    for (std::size_t channel = 0; channel < 6; ++channel) {
        sequence.channels[channel].keyCount = 2;
        sequence.channels[channel].times[0] = 0.0f;
        sequence.channels[channel].times[1] = duration;
        sequence.channels[channel].slopes[0] = 0.0f;
        sequence.channels[channel].slopes[1] = 0.0f;
    }
    sequence.channels[6] = {.keyCount = 2, .times = {0.0f, duration}, .values = {0.0f, 0.0f}, .slopes = {0.0f, 0.0f}};
    sequence.channels[7] = {
        .keyCount = 2, .times = {0.0f, duration}, .values = {static_cast<float>(currentPerspectiveFov) / kFixedAnglePerDegree, 27.5f}, .slopes = {0.0f, 0.0f}};
    phone.authEyePosition = eye;
    phone.authTargetPosition = target;
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 14;
    ApplyPendingGameState(14);
    g_animatedCameraSequence.eyeAnchorOwner = phone.ownerTaskData;
    g_animatedCameraSequence.targetAnchorOwner = phone.ownerTaskData;
    g_animatedCameraSequence.chunk = reinterpret_cast<std::uint32_t *>(&sequence);
    StartAnimatedCamera();
}

bool check_cam_pos_rot_are_same(TelephoneWork &phone) {
    const Vector3 &cameraOrigin = g_CameraEyePos;
    const Vector3 &lookAt = g_CameraTargetPos;
    if (phone.authEyePosition.x == cameraOrigin.x && phone.authEyePosition.y == cameraOrigin.y && phone.authEyePosition.z == cameraOrigin.z &&
        phone.authTargetPosition.x == lookAt.x && phone.authTargetPosition.y == lookAt.y && phone.authTargetPosition.z == lookAt.z) {
        return true;
    }
    phone.authEyePosition = cameraOrigin;
    phone.authTargetPosition = lookAt;
    return false;
}

void TELM_UpdateDialDisplay(TelephoneWork &phone) {
    char display[25]{};
    std::size_t length = 0;
    while (length < sizeof(phone.dialedDigits) && phone.dialedDigits[length])
        ++length;
    std::memcpy(display, phone.dialedDigits, length);
    if (phone.state == 12 && phone.digitDisplaySideToggle)
        display[length] = kDialDigits[phone.dialDigitIndex];
    CloseSubtitleElement();
    if (!phone.pauseCountdown) {
        ScenarioDialogueFlags(phone) |= 0x40U;
        if (display[0] != '\n')
            std::memcpy(g_telephoneDisplayedNumber.data(), display, sizeof(display));
        g_uiElements[kSubtitleUiIndex].isActive = 1;
    }
}

void TELM_RequestDialDisplayRefresh(TelephoneWork &phone) {
    ScenarioDialogueFlags(phone) |= 0x40U;
}

std::int32_t TELM_FindDialMatch(TelephoneWork &phone) {
    const std::size_t dialedLength = std::strlen(phone.dialedDigits);
    std::int32_t entry = 0;
    bool matched = false;
    while (!matched) {
        const char *const number = phone.phoneBookEntries[entry];
        const std::size_t length = std::strlen(number);
        if (length == dialedLength) {
            matched = true;
            for (std::size_t digit = length; digit-- > 0;) {
                if (number[digit] != phone.dialedDigits[digit] && number[digit] != '?') {
                    matched = false;
                    break;
                }
            }
        }
        ++entry;
        if (entry >= 32) {
            if (!matched)
                return -1;
            break;
        }
    }
    UI_SICNClearState(0x0dffU);
    if (phone.dialedDigits[0] == '1' && phone.dialedDigits[1] == '1' && phone.dialedDigits[2] == '0') {
        SteamStatDefinition &stat = g_steamTemporaryStats[Stat_Call_Police];
        if (stat.type == SS_Set)
            stat.value = 1;
        else if (stat.type == SS_Add)
            stat.value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(stat.value) + 1U);
        UpdateSteamStat(Stat_Call_Police);
        UpdateSteamAchievement(Stat_Call_Police);
    }
    return entry;
}



void shenChainToTargetFor(TelephoneWork &phone, Vector3 &output, const Vector3 &input, std::int32_t yaw) {
    Mt5RuntimeNode *const dial = shenGetNodeByChildIdx(phone.hmdl, 1);
    const Mt5RuntimeNode *const parent = dial->parentOffset ? reinterpret_cast<const Mt5RuntimeNode *>(NodeOffsetBase(dial) + dial->parentOffset) : nullptr;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4 chain;
    shenChainToTargetEval(&chain, phone.hmdl, parent);
    Matrix4x4::preTranslate(input.x, input.y, input.z);
    const auto angle = static_cast<std::uint16_t>(yaw + 0x4444);
    constexpr double kSwingRadius = 0.032999999999999995;
    Vector3 position;
    position.y = workTransformMatrix.rows[3].y;
    position.x = static_cast<float>(static_cast<double>(cos_fixed16(angle)) * kSwingRadius + static_cast<double>(workTransformMatrix.rows[3].x));
    position.z = static_cast<float>(
        static_cast<double>(sin_fixed16(static_cast<std::int16_t>(angle))) * kSwingRadius + static_cast<double>(workTransformMatrix.rows[3].z));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(CHAR_EulerAnglesFromMatrix(chain).x));
    Matrix4x4::TransformPoint(position, output);
    Matrix4x4::Pop(1);
}

void telmMain(Character *, TelephoneWork &phone) {
    const std::int32_t ranLastFrame = phone.mainRanLastFrame;
    phone.mainRanLastFrame = 0;
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive) {
        CloseSubtitleElement();
        return;
    }
    if (!ranLastFrame && phone.audioResetLatched) {
        shenExecuteAICACommand(0x605abU, 0, 0);
        phone.audioResetLatched = 1;
    }

    std::int32_t state = phone.state;
    std::int32_t stateFrame = phone.stateFrame;
    if (state != phone.previousFrameState) {
        stateFrame = 1;
        phone.previousState = phone.previousFrameState;
        phone.previousFrameState = state;
        phone.stateTimer = 0;
    }
    HandMot_ReadSequenceFCV(phone, phone.ownerSequence);
    const bool ownerMoving = !phone.ownerSequence.positionDone[0] || !phone.ownerSequence.positionDone[1] || !phone.ownerSequence.positionDone[2];
    HandMot_ReadSequenceFCV(phone, phone.cameraSequence);
    const bool cameraMoving = !phone.cameraSequence.positionDone[0] || !phone.cameraSequence.positionDone[1] || !phone.cameraSequence.positionDone[2];

    Character *const ownerNode = phone.ownerNode;
    std::int32_t x = ownerNode ? ownerNode->entity.node.rotation.x : 0;
    const std::int32_t y = ownerNode ? ownerNode->entity.node.rotation.y : 0;
    std::int32_t z = ownerNode ? ownerNode->entity.node.rotation.z : 0;
    Character *const cameraNode = phone.cameraNode;
    Vector3i cameraRotation = cameraNode ? cameraNode->entity.node.rotation : Vector3i{};

    const std::int32_t dialYawVelocity = phone.dialYawVelocity;
    phone.dialCurrentYaw += dialYawVelocity;
    const std::int32_t dialTargetYawVelocity = phone.dialTargetYawVelocity;
    phone.dialTargetYaw += dialTargetYawVelocity;
    phone.dialCurrentYaw &= 0xffff;
    phone.dialTargetYaw &= 0xffff;
    phone.mainRanLastFrame = 1;

    std::int32_t handFollow = 1;
    bool cancelEnabled = false;
    Vector3 point{};
    const Vector3 cameraOffscreen{0.34900001f, 0.30000001f, 0.32499999f};
    const Vector3 ownerOffscreen{-0.34900001f, phone.phoneVariant == 0 ? 0.30000001f : 0.63f, 0.32499999f};

    switch (state) {
    case 0:
        if (check_cam_pos_rot_are_same(phone)) {
            NPCLogger(phone.ownerNode, 0, 1);
            const Vector3 to = phone.ownerSplinePosition;
            TELM_StartHandSequence(phone.ownerSequence, phone.ownerNode, &ownerOffscreen, &to, 30.0f, nullptr, 0.0f, 588, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 0, 1);
            state = 1;
        }
        handFollow = 0;
        break;
    case 1:
        if (!phone.ownerSequence.state) {
            phone.currentPeerNode = ownerNode;
            phone.currentPeerMode = -1;
            NPCLogger(cameraNode, 0, 1);
            if (phone.phoneVariant == 0) {
                shenChainToTargetFor(phone, point, phone.dialFollowPosition, phone.dialCurrentYaw);
                TELM_StartHandSequence(
                    phone.cameraSequence, phone.cameraNode, &cameraOffscreen, &point, 30.0f, nullptr, 0.0f, 582, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 1, 1);
                shenExecuteAICACommand(0x705abU, 0, 0);
                phone.audioResetLatched = 0;
            } else {
                Character *const camera = phone.cameraNode;
                point = phone.alternateReceiverPosition;
                if (camera) {
                    cameraRotation.y = camera->entity.node.rotation.y;
                    cameraRotation.z = camera->entity.node.rotation.z;
                }
                const Vector3i aim{0x4000, cameraRotation.y, cameraRotation.z};
                TELM_StartHandSequence(
                    phone.cameraSequence, camera, &cameraOffscreen, &point, 30.0f, &aim, kSeqFramesPerSecond, 587, 0.0f, 24.0f, 0.5f, 30.0f, 30.0f, 0, 1);
                authInitAndStartSequence(phone, phone.dialAuthStartPosition, phone.dialAuthEndPosition, 1.25f);
            }
            shenExecuteAICACommand(0x6abU, 0, 0);
            phone.audioResetLatched = 0;
            state = 2;
            if (stateFrame >= 43 && phone.ownerNode)
                phone.ownerNode->entity.node.rotation = {x - 36, y, z - 36};
        }
        handFollow = 0;
        break;
    case 2:
        if (phone.phoneVariant != 2) {
            x -= 546;
            z += 819;
        }
        if (ownerNode)
            ownerNode->entity.node.rotation = {x, y, z};
        if (!phone.ownerSequence.state) {
            const Vector3 to{-0.34900001f, phone.phoneVariant ? 0.63f : 0.30000001f, 0.32499999f};
            authInitAndStartSequence(phone, phone.callAuthStartPosition, phone.callAuthEndPosition, 1.25f);
            const Vector3 from = phone.ownerSplinePosition;
            TELM_StartHandSequence(phone.ownerSequence, phone.ownerNode, &from, &to, 30.0f, nullptr, 0.0f, 588, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 0, 1);
        }
        if (stateFrame < 13 && phone.ownerNode)
            phone.ownerNode->entity.node.rotation = {x + 36, y, z + 36};
        if (stateFrame == 30) {
            ShowDialPrompt();
            if (phone.phoneVariant) {
                state = 18;
            } else {
                state = 12;
                shenExecuteAICACommand(0x605abU, 0, 0);
                phone.audioResetLatched = 1;
            }
        }
        handFollow = 0;
        break;
    case 3:
        if (phone.dialDisplayDirty) {
            TELM_RequestDialDisplayRefresh(phone);
            TELM_UpdateDialDisplay(phone);
            phone.dialDisplayDirty = 0;
        }
        if (!ownerMoving) {
            Vector3 from;
            shenChainToTargetFor(phone, from, phone.dialFollowPosition, phone.dialCurrentYaw);
            TELM_StartHandSequence(
                phone.cameraSequence, phone.cameraNode, &from, &cameraOffscreen, 30.0f, nullptr, 0.0f, 582, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 0, 1);
            phone.currentPeerNode = nullptr;
            phone.currentPeerMode = -1;
            shenExecuteAICACommand(0x705abU, 0, 0);
            phone.audioResetLatched = 0;
            shenExecuteAICACommand(0x106abU, 0, 0);
            state = 4;
            phone.audioResetLatched = 0;
        }
        if (stateFrame >= 43 && phone.ownerNode)
            phone.ownerNode->entity.node.rotation = {x - 36, y, z - 36};
        handFollow = 0;
        break;
    case 4:
        if (!phone.ownerSequence.state) {
            Vector3 to{-0.34900001f, 0.30000001f, 0.32499999f};
            if (phone.phoneVariant) {
                to.y = 0.63f;
                if (phone.coinChargeApplied == 1)
                    UI_ICMoRequest(10, true);
            }
            const Vector3 from = phone.ownerSplinePosition;
            TELM_StartHandSequence(phone.ownerSequence, phone.ownerNode, &from, &to, 30.0f, nullptr, 0.0f, 582, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 0, 1);
        }
        if (stateFrame < 13 && phone.ownerNode)
            phone.ownerNode->entity.node.rotation = {x + 36, y, z + 36};
        if (stateFrame == 60)
            phone.cleanup = 1;
        handFollow = 0;
        break;
    case 5:
        if (phone.previousState == 8 && stateFrame < 38)
            UpdateRingCadence(phone);
        if (!cameraMoving && !phone.cameraSequence.state) {
            shenExecuteAICACommand(0x705abU, 0, 0);
            phone.audioResetLatched = 0;
            const Vector3 *authStart = &phone.callAuthStartPosition;
            const Vector3 *authEnd = &phone.callAuthEndPosition;
            if (phone.phoneVariant) {
                Character *const camera = phone.cameraNode;
                point = {0.097999997f, 0.044f, 0.26899999f};
                if (camera) {
                    cameraRotation.y = camera->entity.node.rotation.y;
                    cameraRotation.z = camera->entity.node.rotation.z;
                }
                const Vector3i aim{0x4000, cameraRotation.y, cameraRotation.z};
                TELM_StartHandSequence(
                    phone.cameraSequence, camera, nullptr, &point, 30.0f, &aim, kSeqFramesPerSecond, 587, 0.0f, 24.0f, 1.0f, 30.0f, 30.0f, 0, 1);
                authStart = &phone.dialAuthStartPosition;
                authEnd = &phone.dialAuthEndPosition;
                state = 20;
            } else {
                shenChainToTargetFor(phone, point, phone.dialFollowPosition, phone.dialCurrentYaw);
                Character *const camera = phone.cameraNode;
                if (camera) {
                    cameraRotation.y = camera->entity.node.rotation.y;
                    cameraRotation.z = camera->entity.node.rotation.z;
                }
                const Vector3i aim{0x4000, cameraRotation.y, cameraRotation.z};
                TELM_StartHandSequence(
                    phone.cameraSequence, camera, nullptr, &point, 30.0f, &aim, kSeqFramesPerSecond, 586, 0.0f, 0.0f, 1.0f, 15.0f, 15.0f, 0, 1);
                state = 7;
            }
            authInitAndStartSequence(phone, *authStart, *authEnd, 1.25f);
        }
        if (stateFrame == 38) {
            shenExecuteAICACommand(0x705abU, 0, 0);
            phone.audioResetLatched = 0;
            shenExecuteAICACommand(0x106abU, 0, 0);
            phone.audioResetLatched = 0;
            shenGetNodeByChildIdx(phone.hmdl, 0)->position.y = phone.receiverBaseY;
            std::memset(phone.dialedDigits, 0, sizeof(phone.dialedDigits));
            phone.dialedDigitsDirty = 1;
        }
        handFollow = 0;
        break;
    case 6:
    case 9:
    case 10:
    case 11:
    case 19:
        break;
    case 7:
        if (!cameraMoving && !phone.cameraSequence.state) {
            phone.receiverAnimTimer = 100.0f;
            shenExecuteAICACommand(0x605abU, 0, 0);
            phone.audioResetLatched = 1;
            state = 12;
        }
        handFollow = 0;
        break;
    case 8:
        UI_SICNClearState(0x0dffU);
        TELM_RequestDialDisplayRefresh(phone);
        SettleTelmDial(phone.dialCurrentYaw, TelmDialAngle(phone.dialDigitIndex), phone.dialYawVelocity);
        handFollow = 0;
        UpdateRingCadence(phone);
        if ((TelmPressed(phone) & 0x200U) != 0 || static_cast<double>(stateFrame) > 60.0) {
            if (phone.phoneVariant == 2) {
                shenExecuteAICACommand(0x705abU, 0, 0);
                phone.audioResetLatched = 0;
            }
            authInitAndStartSequence(phone, phone.dialAuthStartPosition, phone.dialAuthEndPosition, 1.25f);
            phone.dialDigitIndex = 5;
            state = 21;
            phone.dialCurrentYaw = 21845;
        }
        break;
    case 12: {
        if (!dialYawVelocity && !cameraMoving && !g_notebookHelpOverrideActive && (TelmPressed(phone) & 4U) != 0)
            shenBeginNotebook();
        ShowDialPrompt();
        const std::int32_t angle = TelmDialAngle(phone.dialDigitIndex);
        if (SettleTelmDial(phone.dialCurrentYaw, angle, phone.dialYawVelocity)) {
            ++phone.digitDisplayBlinkCounter;
        } else {
            ++phone.digitDisplayBlinkCounter;
            if (phone.digitDisplayBlinkCounter > 3) {
                phone.digitDisplaySideToggle = phone.digitDisplaySideToggle ? 0 : 1;
                phone.digitDisplayBlinkCounter = 0;
                TELM_RequestDialDisplayRefresh(phone);
                TELM_UpdateDialDisplay(phone);
            }
        }
        if (!phone.dialYawVelocity && !cameraMoving) {
            bool dialled = false;
            if ((TelmPressed(phone) & 0x400U) != 0) {
                UI_SICNClearState(0x0dffU);
                const std::size_t length = std::strlen(phone.dialedDigits);
                phone.dialedDigits[length] = kDialDigits[phone.dialDigitIndex];
                phone.dialedDigits[length + 1] = '\0';
                phone.dialedDigitsDirty = 1;
                shenChainToTargetFor(phone, point, phone.numberDisplayPosition, phone.dialCurrentYaw);
                TELM_StartHandSequence(
                    phone.cameraSequence, phone.cameraNode, nullptr, &point, 10.0f, nullptr, 0.0f, -581, 9.0f, 90.0f, 1.2f, 10.0f, 10.0f, 0, 0);
                const std::int32_t digitIndex = phone.dialDigitIndex;
                state = 13;
                phone.savedDialDigitIndex = digitIndex;
                phone.dialDigitIndex = 13;
                phone.dialTargetDigitDelta = 13 - digitIndex;
                phone.dialTargetYawVelocity = 0;
                phone.selectionIdleTimer = 0;
                phone.dialYawVelocity = 0;
                dialled = true;
            }
            if (!dialled) {
                if ((TelmHeld(phone) & 0x4000U) != 0) {
                    --phone.dialDigitIndex;
                    phone.dialYawVelocity = 64808;
                    StepCameraDialSequence(phone);
                    phone.selectionIdleTimer = 0;
                }
                if ((TelmHeld(phone) & 0x8000U) != 0) {
                    ++phone.dialDigitIndex;
                    phone.dialYawVelocity = 728;
                    StepCameraDialSequence(phone);
                    phone.selectionIdleTimer = 0;
                }
                if (phone.dialDigitIndex < 0) {
                    phone.dialYawVelocity = 0;
                    phone.dialDigitIndex = 0;
                }
                if (phone.dialDigitIndex > 9) {
                    phone.dialDigitIndex = 9;
                    phone.dialYawVelocity = 0;
                }
            }
            if (!phone.dialYawVelocity && static_cast<double>(++phone.selectionIdleTimer) > 90.0 && !phone.cameraSequence.state) {
                const bool idleTurn = phone.cameraSequence.sequenceId == 587;
                phone.selectionIdleTimer = 90;
                if (idleTurn) {
                    const std::uint32_t speedSeed = NextEnvironmentSeed(g_EnvPrngSeed);
                    const float speedRandom = EnvironmentRandom(speedSeed);
                    g_EnvPrngSeed = NextEnvironmentSeed(speedSeed);
                    RearmCameraSequence(phone, 587, 0, 0.0f, static_cast<float>(static_cast<double>(speedRandom) * 0.25 + 0.35), 0.0f, 0);
                    if (!static_cast<std::int32_t>(static_cast<double>(EnvironmentRandom(g_EnvPrngSeed)) * 6.0))
                        phone.selectionIdleTimer = 0;
                } else {
                    g_EnvPrngSeed = NextEnvironmentSeed(g_EnvPrngSeed);
                    const float speedRandom = EnvironmentRandom(g_EnvPrngSeed);
                    RearmCameraSequence(phone, 587, 0, 0.0f, static_cast<float>(static_cast<double>(speedRandom) * 0.25 + 0.35), 30.0f, 30);
                }
            }
        }
        ++phone.stateTimer;
        handFollow = cameraMoving ? 0 : 1;
        if (static_cast<double>(phone.stateTimer) >= 600.0) {
            phone.stateTimer = 0;
            phone.ringAudioPhase = 1;
            phone.ringAudioTimer = 0.0f;
            shenExecuteAICACommand(0x705abU, 0, 0);
            phone.audioResetLatched = 0;
            state = 8;
        }
        cancelEnabled = !cameraMoving;
        break;
    }
    case 13:
        if (!phone.digitDisplaySideToggle) {
            phone.digitDisplaySideToggle = 1;
            TELM_RequestDialDisplayRefresh(phone);
            TELM_UpdateDialDisplay(phone);
        }
        if (!phone.cameraSequence.state) {
            shenChainToTargetFor(phone, point, phone.digitPopupPosition, phone.dialCurrentYaw);
            TELM_StartHandSequence(phone.cameraSequence, phone.cameraNode, nullptr, &point, 7.0f, nullptr, 0.0f, -587, 1.0f, 8.0f, 1.0f, 10.0f, 10.0f, 0, 0);
            shenExecuteAICACommand(0x705abU, 0, 0);
            phone.audioResetLatched = 0;
            state = 14;
        }
        handFollow = 0;
        break;
    case 14:
        if (!dialTargetYawVelocity) {
            if (!cameraMoving) {
                phone.dialTargetYawVelocity = 2548;
                phone.dialYawVelocity = 2548;
                shenExecuteAICACommand(0x5abU, 0, 0);
                phone.audioResetLatched = 0;
            }
        } else {
            const std::int32_t angle = TelmDialAngle(phone.dialDigitIndex);
            if (SettleTelmDial(phone.dialCurrentYaw, angle, phone.dialYawVelocity)) {
                phone.dialTargetYawVelocity = 364;
                phone.dialTargetYaw = static_cast<std::uint16_t>(TelmDialAngle(phone.dialTargetDigitDelta));
                shenChainToTargetFor(phone, point, phone.numberDisplayPosition, static_cast<std::int16_t>(angle));
                TELM_StartHandSequence(
                    phone.cameraSequence, phone.cameraNode, nullptr, &point, 15.0f, nullptr, 0.0f, 587, 15.0f, 23.0f, 1.2f, 0.0f, 0.0f, 0, 0);
                shenExecuteAICACommand(0x105abU, 0, 0);
                state = 15;
                phone.audioResetLatched = 0;
            }
        }
        handFollow = cameraMoving ? 0 : 2;
        break;
    case 15:
        if (stateFrame >= 0 && stateFrame <= 2) {
            phone.dialTargetYawVelocity = 0;
        } else if (stateFrame >= 0 && stateFrame <= 4) {
            phone.dialTargetYawVelocity = 64808;
        } else if (stateFrame == 5) {
            phone.dialTargetYawVelocity = 129616;
        } else {
            phone.dialTargetYawVelocity = 194424;
            phone.dialTargetDigitDelta = 0;
            phone.dialDigitIndex = 5;
            phone.dialCurrentYaw = 21845;
            shenExecuteAICACommand(0x205abU, 0, 0);
            phone.audioResetLatched = 0;
            state = 16;
        }
        handFollow = 0;
        break;
    case 16: {
        if (SettleTelmDial(phone.dialTargetYaw, TelmDialAngle(phone.dialTargetDigitDelta), phone.dialTargetYawVelocity)) {
            phone.selectionIdleTimer = 0;
            shenExecuteAICACommand(0x305abU, 0, 0);
            phone.audioResetLatched = 0;
            const std::int32_t match = TELM_FindDialMatch(phone);
            phone.active = match;
            if (match == -1) {
                if (std::strlen(phone.dialedDigits) < 10) {
                    shenChainToTargetFor(phone, point, phone.dialFollowPosition, 21845);
                    TELM_StartHandSequence(
                        phone.cameraSequence, phone.cameraNode, nullptr, &point, 15.0f, nullptr, 0.0f, 586, 0.0f, 0.0f, 1.0f, 15.0f, 15.0f, 0, 1);
                    state = 12;
                } else {
                    point = phone.dialHomePosition;
                    TELM_StartHandSequence(
                        phone.cameraSequence, phone.cameraNode, nullptr, &point, 30.0f, nullptr, 0.0f, 585, 0.0f, 0.0f, 1.0f, 30.0f, 30.0f, 0, 1);
                    const Vector3 eye{0.023800001f, 0.60759997f, 0.1305f};
                    g_EnvPrngSeed = NextEnvironmentSeed(g_EnvPrngSeed);
                    const Vector3 target{
                        0.015900001f, static_cast<float>(static_cast<double>(EnvironmentRandom(g_EnvPrngSeed) / 40.0f) + 0.1999 - 0.0125), -0.0298f};
                    authInitAndStartSequence(phone, eye, target, 1.25f);
                    phone.dialDigitIndex = 5;
                    phone.dialYawVelocity = 0;
                    phone.dialCurrentYaw = 21845;
                    state = 5;
                }
            } else {
                if (phone.phoneVariant)
                    phone.coinChargeApplied = 0;
                shenChainToTargetFor(phone, point, phone.dialFollowPosition, 21845);
                TELM_StartHandSequence(
                    phone.cameraSequence, phone.cameraNode, nullptr, &point, 15.0f, nullptr, 0.0f, 586, 0.0f, 0.0f, 1.0f, 15.0f, 15.0f, 0, 1);
                phone.dialDigitIndex = 5;
                phone.dialYawVelocity = 0;
                phone.dialCurrentYaw = 21845;
                state = 17;
            }
        }
        const std::int32_t digitIndex = phone.dialDigitIndex;
        SettleTelmDial(phone.dialCurrentYaw, TelmDialAngle(digitIndex), phone.dialYawVelocity);
        if ((state != 16 || stateFrame > 30) && digitIndex == 13) {
            phone.dialDigitIndex = 5;
            phone.dialYawVelocity = 728;
        }
        handFollow = 0;
        break;
    }
    case 17:
        if (phone.active == -1) {
            if (phone.phoneVariant == 2) {
                state = 21;
            } else {
                point = phone.dialHomePosition;
                if (cameraNode) {
                    cameraRotation.y = cameraNode->entity.node.rotation.y;
                    cameraRotation.z = cameraNode->entity.node.rotation.z;
                }
                const Vector3i aim{0x4000, cameraRotation.y, cameraRotation.z};
                TELM_StartHandSequence(
                    phone.cameraSequence, cameraNode, nullptr, &point, 30.0f, &aim, kSeqFramesPerSecond, 585, 0.0f, 0.0f, 1.0f, 30.0f, 30.0f, 0, 1);
                const Vector3 eye{0.023800001f, 0.60759997f, 0.1305f};
                g_EnvPrngSeed = NextEnvironmentSeed(g_EnvPrngSeed);
                const Vector3 target{
                    0.015900001f, static_cast<float>(static_cast<double>(EnvironmentRandom(g_EnvPrngSeed) / 40.0f) + 0.1999 - 0.0125), -0.0298f};
                authInitAndStartSequence(phone, eye, target, 1.25f);
                phone.dialYawVelocity = 0;
                phone.dialDigitIndex = 5;
                state = 5;
                phone.dialCurrentYaw = 21845;
            }
        }
        handFollow = 0;
        break;
    case 18:
        if (!phone.cameraSequence.state) {
            const Vector3 from = phone.alternateReceiverPosition;
            shenChainToTargetFor(phone, point, phone.dialFollowPosition, phone.dialCurrentYaw);
            Character *const camera = phone.cameraNode;
            if (camera) {
                cameraRotation.y = camera->entity.node.rotation.y;
                cameraRotation.z = camera->entity.node.rotation.z;
            }
            const Vector3i aim{phone.savedCameraRot.x, cameraRotation.y, cameraRotation.z};
            TELM_StartHandSequence(phone.cameraSequence, camera, &from, &point, 30.0f, &aim, kSeqFramesPerSecond, 582, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 1, 1);
            state = 12;
            shenExecuteAICACommand(0x206abU, 0, 0);
            phone.audioResetLatched = 0;
            shenExecuteAICACommand(0x605abU, 0, 0);
            phone.audioResetLatched = 1;
            authInitAndStartSequence(phone, phone.callAuthStartPosition, phone.callAuthEndPosition, 1.25f);
            if (!phone.coinChargeApplied) {
                UI_ICMoRequest(-10, true);
                phone.coinChargeApplied = 1;
            }
        }
        handFollow = 0;
        break;
    case 20:
        if (!phone.cameraSequence.state) {
            point = phone.alternateReceiverPosition;
            TELM_StartHandSequence(phone.cameraSequence, cameraNode, nullptr, &point, 30.0f, nullptr, 0.0f, 587, 0.0f, 24.0f, 0.5f, 30.0f, 30.0f, 0, 1);
            phone.dialDigitIndex = 5;
            phone.dialCurrentYaw = 21845;
            state = 18;
            shenExecuteAICACommand(0x306abU, 0, 0);
            phone.audioResetLatched = 0;
        }
        handFollow = 0;
        break;
    case 21: {
        UI_SICNClearState(0x0dffU);
        if (phone.ownerNode)
            phone.ownerNode->entity.node.rotation = phone.savedOwnerRot;
        if (phone.cameraNode)
            phone.cameraNode->entity.node.rotation = phone.savedCameraRot;
        authInitAndStartSequence(phone, phone.dialAuthStartPosition, phone.dialAuthEndPosition, 1.0f);
        const Vector3 to = phone.ownerSplinePosition;
        TELM_StartHandSequence(phone.ownerSequence, phone.ownerNode, &ownerOffscreen, &to, 30.0f, nullptr, 0.0f, -588, 0.0f, 0.0f, 0.5f, 30.0f, 30.0f, 0, 1);
        handFollow = 0;
        state = 3;
        break;
    }
    default:
        cancelEnabled = true;
        break;
    }

    if (handFollow == 1 || handFollow == 2) {
        shenChainToTargetFor(phone, point, handFollow == 1 ? phone.dialFollowPosition : phone.digitPopupPosition, phone.dialCurrentYaw);
        if (phone.cameraNode)
            phone.cameraNode->entity.node.position = point;
        if (phone.cameraNode) {
            phone.cameraNode->entity.node.rotation.y = cameraRotation.y;
            phone.cameraNode->entity.node.rotation.z = cameraRotation.z;
            phone.cameraNode->entity.node.rotation.x = cameraRotation.x;
        }
    }
    if ((state == 8 || state == 12 || state == 17) && !phone.cameraSequence.state) {
        const std::uint32_t speedSeed = NextEnvironmentSeed(g_EnvPrngSeed);
        const float speedRandom = EnvironmentRandom(speedSeed);
        g_EnvPrngSeed = NextEnvironmentSeed(speedSeed);
        std::int32_t sequenceId = 582 - static_cast<std::int32_t>(EnvironmentRandom(g_EnvPrngSeed) * -2.0f);
        std::int32_t reverse = 0;
        if (sequenceId < 0) {
            sequenceId = -sequenceId;
            reverse = 1;
        }
        RearmCameraSequence(phone, sequenceId, reverse, 0.0f, static_cast<float>(static_cast<double>(speedRandom) * 0.25 + 0.6), 30.0f, 30);
    }
    if (cancelEnabled && (TelmPressed(phone) & 0x200U) != 0) {
        const std::size_t length = std::strlen(phone.dialedDigits);
        if (length) {
            phone.dialedDigits[length - 1] = '\0';
            phone.dialedDigitsDirty = 1;
        } else {
            phone.dialDisplayDirty = 1;
            state = 21;
        }
    }
    if (static_cast<std::uint32_t>(state - 13) > 3U) {
        if (phone.active == -4)
            phone.active = -1;
    } else {
        phone.active = -4;
    }
    phone.state = state;
    phone.stateFrame = stateFrame + 1;
}


void Transform3DObject(TelephoneUiData &ui, std::byte *modelBase) {
    if (!g_updateRenderFrameMode || !ui.cordBinding)
        return;
    const Vector3 &receiver = ui.receiverRotatedPosition;
    const Vector3 &handset = ui.characterRotatedPosition;
    const float deltaZ = handset.z - receiver.z;
    const float deltaY = handset.y - receiver.y;
    const float deltaX = handset.x - receiver.x;
    const float midX = handset.x * 0.5f + receiver.x * 0.5f;
    const float midY = handset.y * 0.5f + receiver.y * 0.5f;
    const float midZ = handset.z * 0.5f + receiver.z * 0.5f;
    Vector3 side{deltaY * 0.0f - deltaZ * -1.0f, deltaZ * 0.0f - deltaX * 0.0f, deltaX * -1.0f - deltaY * 0.0f};
    Vector3 along{deltaX, deltaY, deltaZ};
    Vector3 up{side.y * deltaZ - side.z * deltaY, side.z * deltaX - side.x * deltaZ, side.x * deltaY - side.y * deltaX};
    const float spanX = handset.x - receiver.x;
    const float spanY = handset.y - receiver.y;
    const float spanZ = handset.z - receiver.z;
    const float length = std::sqrt((spanY * spanY + spanX * spanX) + spanZ * spanZ);

    float radius = 0.0f;
    std::int32_t cosineAngle = 0;
    if (static_cast<double>(length) < 0.2) {
        const float root = std::sqrt(static_cast<float>(24.0 - static_cast<double>(length) * 120.0));
        radius = static_cast<float>(0.2 / static_cast<double>(root));
        const std::int32_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(root) * 10430.378350470464));
        ui.arcAngle = angle;
        cosineAngle = 0x4000 - (angle >> 1);
    } else {
        const float scaled = 85.0f - length / (static_cast<float>(ui.segmentCount) * ui.segmentLength * 8.0f) * 80.0f;
        float degrees = 85.0f;
        if (!(scaled > 85.0f))
            degrees = 5.0f > scaled ? 5.0f : scaled;
        const std::int32_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
        ui.arcAngle = angle;
        const std::int32_t half = angle >> 1;
        radius = length / (sin_fixed16(static_cast<std::int16_t>(half)) * 2.0f);
        cosineAngle = 0x4000 - half;
    }
    ui.arcRadius = radius;
    ui.arcOffset = -(sin_fixed16(static_cast<std::int16_t>(cosineAngle)) * radius);
    Vector3::NormalizeInPlace(along);
    Vector3::NormalizeInPlace(up);
    Vector3::NormalizeInPlace(side);

    const float biasZ = (midZ - ui.previousMidpoint.z) + ui.midpointBias.z;
    const float biasX = (midX - ui.previousMidpoint.x) + ui.midpointBias.x;
    const float biasY = (midY - ui.previousMidpoint.y) + ui.midpointBias.y;
    const float sway = ((biasY * side.y + biasX * side.x) + biasZ * side.z) + ui.swayAccumulator;
    ui.previousMidpoint = {midX, midY, midZ};
    ui.swayAccumulator = sway;
    Matrix4x4 frame{};
    frame.rows[0] = {along.x, along.y, along.z, 0.0f};
    frame.rows[1] = {up.x, up.y, up.z, 0.0f};
    frame.rows[2] = {side.x, side.y, side.z, 0.0f};
    frame.rows[3] = {ui.previousMidpoint.x, ui.previousMidpoint.y, ui.previousMidpoint.z, 1.0f};

    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(0.0f, ui.arcRadius, 0.0f);
    Matrix4x4 arc = workTransformMatrix;
    const Matrix4x4 arcBase = workTransformMatrix;
    workTransformMatrix = frame;
    Matrix4x4::preTranslate(0.0f, ui.arcOffset, 0.0f);
    frame = workTransformMatrix;
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(ui.arcAngle / ui.segmentCount - ui.arcAngle / 2));
    Matrix4x4::Multiply(&arc);
    float stretch = std::sqrt(((workTransformMatrix.rows[3].y - handset.y) * (workTransformMatrix.rows[3].y - handset.y) +
                                  (workTransformMatrix.rows[3].x - handset.x) * (workTransformMatrix.rows[3].x - handset.x)) +
                        (workTransformMatrix.rows[3].z - handset.z) * (workTransformMatrix.rows[3].z - handset.z)) /
        ui.segmentLength;
    if (!(stretch > 0.4f))
        stretch = 0.4f;
    workTransformMatrix = arcBase;
    Matrix4x4::Scale(stretch, 1.0f, 1.0f);
    arc = workTransformMatrix;
    workTransformMatrix = *(g_matrixStack.top.stack - 1);
    Matrix4x4::Multiply(&frame);
    Matrix4x4_Push();
    for (std::int32_t segment = 0; segment < ui.segmentCount; ++segment) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(ui.arcAngle * segment / ui.segmentCount - ui.arcAngle / 2));
        Matrix4x4::Multiply(&arc);
        SubmitSceneObjectForDrawing(ui.cordBinding, modelBase);
        workTransformMatrix = *(g_matrixStack.top.stack - 1);
    }
    Matrix4x4::Pop(1);
    Matrix4x4::Pop(1);
}

void telmUpdateForReceiver(Character *character, TelephoneWork &phone) {
    Mt5RuntimeNode *const handset = shenGetNodeByChildIdx(phone.hmdl, 3);
    auto *const handsetTransform = const_cast<Matrix4x4 *>(HMDL_TransformForNode(handset, handset->transformOffset));
    if (handsetTransform) {
        Matrix4x4_Push(handsetTransform);
        Matrix4x4::preTranslate(phone.customReceiverOffset.x, phone.customReceiverOffset.y, phone.customReceiverOffset.z);
        if (phone.customReceiverRotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(phone.customReceiverRotation.z));
        if (phone.customReceiverRotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(phone.customReceiverRotation.y));
        if (phone.customReceiverRotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(phone.customReceiverRotation.x));
        *handsetTransform = workTransformMatrix;
        Matrix4x4::Pop(1);
    }
    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(character ? &character->entity.node : nullptr);
    Matrix4x4_Push();
    const Vector3 receiverLocal = phone.receiverCordNode ? phone.receiverCordNode->position : Vector3{-0.07f, 0.07f, -0.07f};
    Matrix4x4::preTranslate(receiverLocal.x, receiverLocal.y, receiverLocal.z);
    const Vector3 receiverPosition = WorkTranslation();
    workTransformMatrix = *(g_matrixStack.top.stack - 1);
    Vector3 handsetLocal;
    if (handsetTransform) {
        Matrix4x4::Multiply(handsetTransform);
        handsetLocal = {-0.079999998f, -0.0099999998f, 0.0f};
    } else if (phone.handsetCordNode) {
        handsetLocal = phone.handsetCordNode->position;
    } else {
        handsetLocal = {static_cast<float>(static_cast<double>(handset->position.x) - 0.1), static_cast<float>(static_cast<double>(handset->position.y) - 0.02),
            static_cast<float>(static_cast<double>(handset->position.z) - 0.16)};
    }
    Matrix4x4::preTranslate(handsetLocal.x, handsetLocal.y, handsetLocal.z);
    const Vector3 handsetPosition = WorkTranslation();
    Matrix4x4::Pop(1);
    Matrix4x4::Pop(1);
    phone.phoneUi.receiverRotatedPosition = receiverPosition;
    phone.phoneUi.characterRotatedPosition = handsetPosition;
    Transform3DObject(phone.phoneUi, MT5_ModelBaseForNode(phone.hmdl));
}

TelephoneWork *FindTelephoneWork(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kTelmComponent) : nullptr;
    return component ? reinterpret_cast<TelephoneWork *>(component + 1) : nullptr;
}

Character *ScriptCharacter(const TelephoneWork &phone, std::uint32_t id) {
    return id && phone.scriptContext && phone.scriptContext->task ? CHID_Find(phone.scriptContext->task->task, id) : nullptr;
}


std::int32_t cpctGetCurrentMode() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    CharacterComponentNode *const component = ryo ? HLib_GetCharacterComponent(ryo, SHEN_NAME(CPCT)) : nullptr;
    return component ? static_cast<std::int32_t>(reinterpret_cast<PlayerControlWork *>(component + 1)->currentMode) : 0;
}

} // namespace

std::int32_t AllocSPLI(float *coefficients, std::int32_t keyCount, const float *times, const float *values, const float *slopes) {
    std::int32_t count = keyCount;
    for (std::int32_t index = 0; index < keyCount - 1; ++index) {
        if (times[index] == times[index + 1]) {
            count = index + 1;
            break;
        }
    }
    if (count < 2)
        return count == 1;

    auto *const scratch = static_cast<float *>(HLib_AllocHeapMemBlock(28LL * count, kSplineToken));
    float *const spans = scratch;
    float *const doubledSpans = spans + count;
    float *const diagonal = doubledSpans + count;
    float *const factors = diagonal + count;
    float *const deltas = factors + count;
    float *const rightHandSide = deltas + count;
    float *const derivatives = rightHandSide + count;
    const std::int32_t interior = count - 2;
    std::int32_t index = 0;
    for (; index < interior; ++index) {
        spans[index] = times[index + 1] - times[index];
        doubledSpans[index] = (times[index + 2] - times[index]) * 2.0f;
        deltas[index] = values[index + 1] - values[index];
    }
    spans[index] = times[index + 1] - times[index];
    deltas[index] = values[index + 1] - values[index];
    if (slopes) {
        derivatives[0] = slopes[0];
        derivatives[count - 1] = slopes[count - 1];
    } else {
        derivatives[0] = 0.0f;
        derivatives[count - 1] = 0.0f;
    }
    if (count >= 3) {
        diagonal[0] = doubledSpans[0];
        for (std::int32_t row = 1; row < interior; ++row) {
            factors[row] = spans[row + 1] / diagonal[row - 1];
            diagonal[row] = doubledSpans[row] - factors[row] * spans[row - 1];
        }
        rightHandSide[0] = ((spans[0] * deltas[1]) / spans[1] + (spans[1] * deltas[0]) / spans[0]) * 3.0f;
        for (std::int32_t row = 1; row < interior; ++row) {
            rightHandSide[row] =
                ((spans[row + 1] * deltas[row]) / spans[row] + (spans[row] * deltas[row + 1]) / spans[row + 1]) * 3.0f - factors[row] * rightHandSide[row - 1];
        }
        derivatives[interior] = rightHandSide[interior - 1] / diagonal[interior - 1];
        for (std::int32_t row = interior - 1; row-- > 0;)
            derivatives[row + 1] = (rightHandSide[row] - derivatives[row + 2] * spans[row]) / diagonal[row];
    }
    for (std::int32_t key = 1; key < count; ++key) {
        coefficients[2 * key] = derivatives[key - 1] * spans[key - 1] + values[key - 1] * 3.0f;
        coefficients[2 * key + 1] = values[key] * 3.0f - derivatives[key] * spans[key - 1];
    }
    *reinterpret_cast<std::int32_t *>(coefficients) = 0;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(scratch), 0);
    return count;
}

std::int32_t Sequence_ReadFCVKey(FcvSplineReader *reader, float time, float *output) {
    float sampleTime = time;
    if (g_updateRenderFrameMode > 0)
        sampleTime = static_cast<float>(static_cast<double>(static_cast<float>(g_updateRenderFrameMode)) * 0.00625 + static_cast<double>(time));
    while (true) {
        const std::int32_t result = HermiteSpline_InterpolateFCVKey(
            output, sampleTime, reader->segmentKeyCount, reader->times + reader->keyOffset, reader->values + reader->keyOffset, reader->coefficients);
        if (result)
            return result;
        if (reader->keyOffset + reader->segmentKeyCount >= reader->keyCount)
            return 0;
        reader->keyOffset += reader->segmentKeyCount;
        reader->segmentKeyCount = AllocSPLI(reader->coefficients, reader->keyCount - reader->keyOffset, reader->times + reader->keyOffset,
            reader->values + reader->keyOffset, reader->slopes ? reader->slopes + reader->keyOffset : nullptr);
    }
}

void TELM_Callback(Character *character, void *payload) {
    TelephoneWork &phone = *static_cast<TelephoneWork *>(payload);
    if (phone.active) {
        auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        auto *const pad = control ? static_cast<PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
        const bool resumed = phone.pauseCountdown == 11;
        phone.padInput = pad ? reinterpret_cast<InputSlot *>(&pad->enabled) : nullptr;
        if (resumed && phone.audioResetLatched && !g_notebookHelpOverrideActive) {
            shenExecuteAICACommand(0x605abU, 0, 0);
            phone.audioResetLatched = 1;
        }
        telmMain(character, phone);
    }
    if (phone.cleanup) {
        TELM_Cleanup(&phone);
        phone.cleanup = 0;
    }
    if (phone.pauseCountdown)
        --phone.pauseCountdown;
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive)
        phone.pauseCountdown = 10;
    if ((Shenmue_nPauseState & 2) != 0)
        phone.pauseCountdown = 11;
}

void telmUpdateMotion(Character *character) {
    if (g_updateRenderFrameMode == 0)
        return;
    TelephoneWork *const phone = FindTelephoneWork(character);
    if (!phone)
        return;
    Matrix4x4 *receiverTransform = nullptr;
    Mt5RuntimeNode *const root = phone->hmdl;
    Mt5RuntimeNode *const receiver = shenGetNodeByChildIdx(root, 0);
    Mt5RuntimeNode *const handset = shenGetNodeByChildIdx(root, 3);
    if (phone->currentPeerNode) {
        receiverTransform = static_cast<Matrix4x4 *>(HLib_AllocHeapMemBlock(sizeof(Matrix4x4), kTelmReceiverTransformToken));
        if (phone->currentPeerMode < 0) {
            Matrix4x4::SetActiveTransformToPrePass();
            TransformCharacterHierarchyZYX(&phone->currentPeerNode->entity.node);
            if (phone->phoneVariant == 2) {
                Matrix4x4::preTranslate(0.035f, -0.1f, 0.026799999f);
                Matrix4x4::RotateAroundZ_Wrapper(59164);
                Matrix4x4::RotateAroundY_Wrapper(0);
                Matrix4x4::RotateAroundZ_Wrapper(0);
            } else {
                Matrix4x4::preTranslate(0.0f, -0.1f, 0.026799999f);
                Matrix4x4::RotateAroundZ_Wrapper(62625);
                Matrix4x4::RotateAroundY_Wrapper(1091);
                Matrix4x4::RotateAroundX_Wrapper(49885);
            }
            *(receiverTransform ? receiverTransform : g_matrixStack.top.stack) = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::SetActiveTransformToPrePass();
            TransformCharacterHierarchyZYX(&character->entity.node);
            Matrix4x4::InvertInPlace(nullptr);
            if (receiverTransform)
                Matrix4x4::Multiply(receiverTransform);
        } else {
            auto *const peerModel = static_cast<Mt5RuntimeNode *>(phone->currentPeerNode->model);
            Mt5RuntimeNode *const hand = shenGetNodeByChildIdx(peerModel, phone->currentPeerMode == 3 ? -66 : -65);
            const Matrix4x4 *const handTransform = hand ? HMDL_TransformForNode(hand, hand->transformOffset) : nullptr;
            Matrix4x4::SetActiveTransformToPrePass();
            TransformCharacterHierarchyZYX(&character->entity.node);
            Matrix4x4::InvertInPlace(nullptr);
            if (handTransform)
                Matrix4x4::Multiply(handTransform);
            Matrix4x4::preTranslate(0.075000003f, 0.025f, 0.0f);
            Matrix4x4::RotateAroundZ_Wrapper(0x4000);
        }
        *(receiverTransform ? receiverTransform : g_matrixStack.top.stack) = workTransformMatrix;
        Matrix4x4::Pop(1);
        if (phone->receiverLiftedY <= receiver->position.y) {
            receiver->position.y = phone->receiverLiftedY;
        } else {
            receiver->position.y = static_cast<float>(static_cast<double>(receiver->position.y) + 0.002);
            float distance = receiver->position.y - phone->receiverLiftedY;
            if (distance < 0.0f)
                distance = -distance;
            if (static_cast<double>(distance) < 0.0005)
                receiver->position.y = phone->receiverLiftedY;
        }
        HMDL_SetTemporaryTaskRelativeOffsets(handset, true);
        handset->transformOffset = receiverTransform ? static_cast<std::int32_t>(reinterpret_cast<std::byte *>(receiverTransform) - g_pCurrentTaskData) : 0;
    } else {
        handset->transformOffset = 0;
        HMDL_SetTemporaryTaskRelativeOffsets(handset, false);
        if (receiver->position.y <= phone->receiverBaseY) {
            receiver->position.y = phone->receiverBaseY;
        } else {
            receiver->position.y = static_cast<float>(static_cast<double>(receiver->position.y) - 0.002);
            float distance = receiver->position.y - phone->receiverBaseY;
            if (distance < 0.0f)
                distance = -distance;
            if (static_cast<double>(distance) < 0.0005)
                receiver->position.y = phone->receiverBaseY;
        }
    }
    shenGetNodeByChildIdx(root, 1)->rotation.y = -phone->dialTargetYaw;

    g_sceneObjectMaxScale = 10.0f;
    telmUpdateForReceiver(character, *phone);
    g_sceneObjectMaxScale = 1.0f;
    Matrix4x4_Push();
    g_drawingCharacter = character;
    TransformCharacterHierarchyZYX(&character->entity.node);
    CharacterHandler_DrawSubmitAndScale(phone->hmdl, 1.0f);
    g_drawingCharacter = nullptr;
    Matrix4x4::Pop(1);
    phone->hmdl->rotation.y = 0;

    auto *const scenario = reinterpret_cast<std::byte *>(phone->scenarioTask->task_data_ptr);
    auto &dialogueFlags = *reinterpret_cast<std::uint32_t *>(scenario + kScenarioDialogueFlagsOffset);
    if ((dialogueFlags & 0x12U) == 0) {
        if ((dialogueFlags & 4U) != 0) {
            **reinterpret_cast<std::uint32_t **>(scenario + kScenarioWindowOffset) |= 0x30U;
            dialogueFlags ^= 4U;
        } else {
            dialogueFlags |= 8U;
        }
    }
    if (receiverTransform)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(receiverTransform), 0);
}

void EnqueueTELM(Character *character, std::int32_t variant) {
    auto *const phone = static_cast<TelephoneWork *>(AllocateHeapMemoryMODL(character, kTelmComponent, sizeof(TelephoneWork), TELM_Callback));
    if (!phone)
        return;
    if (character) {
        character->figurePoseCallback = telmUpdateMotion;
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kTelmComponent))
            component->destroyCallback = Script_NoOp;
    }
    g_telmRetailProbeCharacter = character;
    g_telmRetailProbeWork = phone;
    phone->phoneVariant = variant;
    auto *const root = static_cast<Mt5RuntimeNode *>(character->model);
    phone->hmdl = root;
    Mt5RuntimeNode *dialNode = nullptr;
    Mt5RuntimeNode *receiverNode = nullptr;
    Mt5RuntimeNode *ownerNode = nullptr;
    Mt5RuntimeNode *alternateNode = nullptr;
    if (root) {
        dialNode = shenGetNodeByChildIdx(root, 1);
        receiverNode = shenGetNodeByChildIdx(root, 0);
        ownerNode = shenGetNodeByChildIdx(root, 3);
        if (variant)
            alternateNode = shenGetNodeByChildIdx(root, 85);
    }
    phone->customReceiverPoseEnabled = 0;
    phone->customReceiverOffset = {};
    phone->customReceiverRotation = {};

    void *cordBinding = nullptr;
    const auto detachCordBinding = [&cordBinding](Mt5RuntimeNode *node) {
        if (node && node->sceneBindingOffset) {
            if (std::byte *const base = MT5_ModelBaseForNode(node)) {
                cordBinding = base + node->sceneBindingOffset;
                node->sceneBindingOffset = 0;
            }
        }
    };
    phone->receiverCordNode = shenGetNodeByChildIdx(root, 252);
    detachCordBinding(phone->receiverCordNode);
    phone->handsetCordNode = shenGetNodeByChildIdx(phone->hmdl, 253);
    detachCordBinding(phone->handsetCordNode);
    phone->phoneUi = {};
    phone->phoneUi.segmentCount = 30;
    phone->phoneUi.cordBinding = cordBinding;
    phone->phoneUi.segmentLength = 0.02f;
    phone->phoneUi.midpointBias.y = 100.0f;

    phone->currentPeerNode = nullptr;
    phone->currentPeerMode = -1;
    phone->initialDialYaw = dialNode ? dialNode->rotation.y : 0;
    const float receiverBaseY = receiverNode ? receiverNode->position.y : 0.0f;
    phone->active = 0;
    phone->motionFlag0Enabled = 1;
    phone->internalFlag8 = 1;
    phone->receiverBaseY = receiverBaseY;
    phone->receiverLiftedY = static_cast<float>(static_cast<double>(receiverBaseY) + 0.011247);
    for (char(&entry)[11] : phone->phoneBookEntries)
        std::memset(entry, 0, 10);
    g_telephoneDisplayedNumber[0] = '\0';
    phone->ownerNode = nullptr;
    phone->cameraNode = nullptr;
    phone->scriptContext = nullptr;
    phone->scenarioTask = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->scenarioTask : nullptr;
    phone->padInput = nullptr;
    phone->cleanup = 0;
    phone->digitDisplaySideToggle = 1;
    phone->digitDisplayBlinkCounter = 0;

    const Vector3 ownerOffsets[3]{
        {0.059f, 0.021500001f, -0.097499996f},
        {-0.059f, 0.021500001f, 0.097499996f},
        {-0.081f, 0.083499998f, -0.0145f},
    };
    const Vector3 dialOffsets[3]{
        {0.037599999f, 0.019200001f, 0.052499998f},
        {0.042599998f, -0.0077999998f, 0.051600002f},
        {0.0126f, -0.046799999f, 0.090499997f},
    };
    const Vector3 numberOffset = variant == 2 ? Vector3{0.003f, -0.0080000004f, -0.027000001f} : Vector3{0.0020000001f, -0.001f, -0.016000001f};
    const Vector3 digitOffset = variant == 2 ? Vector3{0.0f, -0.022f, -0.015f} : Vector3{-0.003f, -0.0080000004f, -0.024f};
    const std::size_t dialIndex = variant == 0 ? 0U : (variant == 1 ? 1U : 2U);
    std::int32_t ownerEulerX = 0;
    std::int32_t dialEulerX = 0;
    const Vector3 ownerOffset = variant >= 0 && variant <= 2 ? ownerOffsets[static_cast<std::size_t>(variant)] : Vector3{};
    phone->ownerSplinePosition = TelephoneAnchorPosition(root, ownerNode, ownerOffset, nullptr, &ownerEulerX);
    phone->savedOwnerRot = {16930, 60984, ownerEulerX + 910};
    phone->numberDisplayPosition = TelephoneAnchorPosition(root, dialNode, dialOffsets[dialIndex], &numberOffset, nullptr);
    phone->digitPopupPosition = TelephoneAnchorPosition(root, dialNode, dialOffsets[dialIndex], &digitOffset, nullptr);
    phone->dialFollowPosition = TelephoneAnchorPosition(root, dialNode, dialOffsets[dialIndex], nullptr, &dialEulerX);
    phone->savedCameraRot = {21846 - dialEulerX, 62805, 0x8000};
    const Vector3 homeOffset = variant ? Vector3{0.097999997f, 0.114f, 0.175f} : Vector3{-0.097999997f, 0.114f, -0.175f};
    phone->dialHomePosition = TelephoneAnchorPosition(root, receiverNode, homeOffset, nullptr, nullptr);
    if (variant && alternateNode)
        phone->alternateReceiverPosition = TelephoneAnchorPosition(root, alternateNode, {0.078000002f, 0.083999999f, 0.133f}, nullptr, nullptr);
    else
        phone->alternateReceiverPosition = {};
    phone->pauseCountdown = 0;
}

void TELM_Cleanup(TelephoneWork *phone) {
    phone->active = 0;
    if (HandModelWork *const cameraHand = phone->cameraSequence.hand; cameraHand && (cameraHand->primaryRoot || cameraHand->secondaryRoot))
        HND_ResetMotion(*cameraHand, 0, 0);
    if (HandModelWork *const ownerHand = phone->ownerSequence.hand; ownerHand && (ownerHand->primaryRoot || ownerHand->secondaryRoot))
        HND_ResetMotion(*ownerHand, 0, 0);
    HND_UnregisterMotion(phone->handMotionTaskData);
    if (phone->ownerNode) {
        NPCLogger(phone->ownerNode, 3, 0);
        phone->ownerNode = nullptr;
    }
    if (phone->cameraNode) {
        NPCLogger(phone->cameraNode, 3, 0);
        phone->cameraNode = nullptr;
    }
    if (Character *const receiver = phone->receiverNode) {
        NPCLogger(receiver, 0, 0);
        receiver->flags.value = (receiver->flags.value & ~3U) | 2U;
        ++reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->pendingCharacterRetirements;
        phone->receiverNode = nullptr;
    }
    phone->currentPeerNode = nullptr;
    phone->currentPeerMode = -1;
    PCCTL_RequestNewMode(phone->savedCpctAnimState);
    if (phone->cameraSequence.coefficientPool) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(phone->cameraSequence.coefficientPool), kTelmCameraPoolToken);
        phone->cameraSequence.coefficientPool = nullptr;
    }
    if (phone->ownerSequence.coefficientPool) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(phone->ownerSequence.coefficientPool), kTelmOwnerPoolToken);
        phone->ownerSequence.coefficientPool = nullptr;
    }
    CloseSubtitleElement();
    g_telephoneDisplayedNumber[0] = '\0';
    SCEN_SetHelpToken(phone->savedHelpTokenId, phone->savedButtonId);
}

void TELM_SetTargetTaskAndMode(TelephoneWork *phone, std::uint32_t ownerId, std::uint32_t targetId, std::int32_t mode) {
    Character *const owner = ScriptCharacter(*phone, ownerId);
    Character *const target = ScriptCharacter(*phone, targetId);
    TelephoneWork *const ownerPhone = FindTelephoneWork(owner);
    if (!ownerPhone)
        return;
    ownerPhone->currentPeerNode = target;
    ownerPhone->currentPeerMode = mode;
}

void TELM_LoadHandMotionAndSetup(TelephoneWork *phone, std::uint32_t ownerId) {
    phone->active = -1;
    phone->handMotionTaskData = LoadHandMot(g_sceneId, g_areaId, "HMOT_TEL.BIN");
    Character *const owner = ScriptCharacter(*phone, ownerId);
    CharacterNode *const ownerFrame = owner ? &owner->entity.node : nullptr;
    phone->ownerNode = static_cast<Character *>(g_cameraTargetEntity);
    NPCLogger(phone->ownerNode, 3, 0);
    phone->ownerSequence.hand = phone->ownerNode ? HND_GetWork(phone->ownerNode, true) : nullptr;
    phone->cameraNode = static_cast<Character *>(g_shenmueCamera0);
    NPCLogger(phone->cameraNode, 3, 0);
    phone->cameraSequence.hand = phone->cameraNode ? HND_GetWork(phone->cameraNode, false) : nullptr;
    if (phone->ownerNode)
        phone->ownerNode->entity.node.child = ownerFrame;
    if (phone->cameraNode)
        phone->cameraNode->entity.node.child = ownerFrame;
    if (phone->receiverNode)
        phone->receiverNode->entity.node.child = ownerFrame;
    phone->ownerSequence.sequenceId = 582;
    phone->cameraSequence.sequenceId = 582;
    phone->ownerSequence.state = 0;
    phone->ownerSequence.reserved14 = 0;
    phone->cameraSequence.state = 0;
    phone->cameraSequence.reserved14 = 0;
    phone->cameraSequence.coefficientPool = static_cast<std::byte *>(HLib_AllocHeapMemBlock(0x1e0, kTelmCameraPoolToken));
    phone->ownerSequence.coefficientPool = static_cast<std::byte *>(HLib_AllocHeapMemBlock(0x1e0, kTelmOwnerPoolToken));
    HLib_EnableTokenCheck(phone->cameraSequence.coefficientPool);
    HLib_EnableTokenCheck(phone->ownerSequence.coefficientPool);

    if (phone->ownerNode)
        phone->ownerNode->entity.node.position = {0.0f, -0.1f, 0.026799999f};
    if (phone->ownerNode)
        phone->ownerNode->entity.node.rotation = phone->savedOwnerRot;
    if (phone->cameraNode)
        phone->cameraNode->entity.node.position = {0.34900001f, 0.30000001f, 0.32499999f};
    if (phone->cameraNode)
        phone->cameraNode->entity.node.rotation = phone->savedCameraRot;
    Mt5RuntimeNode *const dial = shenGetNodeByChildIdx(phone->hmdl, 1);
    phone->dialTargetYaw = phone->initialDialYaw;
    phone->dialTargetYawVelocity = 0;
    phone->dialTargetDigitDelta = 0;
    phone->dialDigitIndex = 5;
    phone->dialCurrentYaw = 21845;
    phone->dialYawVelocity = 0;
    phone->selectionIdleTimer = 0;
    phone->stateTimer = 0;
    phone->state = 0;
    phone->previousFrameState = 0;
    phone->previousState = 0;
    phone->stateFrame = 1;
    std::memset(phone->dialedDigits, 0, sizeof(phone->dialedDigits));
    phone->dialedDigitsDirty = 1;
    ScenarioDialogueFlags(*phone) |= 0x40U;
    if (Character *const receiver = phone->receiverNode) {
        receiver->entity.node.position = {-0.13f, 0.0555f, 0.0287f};
        receiver->entity.node.rotation = {33123, 31130, 32957};
    }

    const Mt5RuntimeNode *const dialParent = dial->parentOffset ? reinterpret_cast<const Mt5RuntimeNode *>(NodeOffsetBase(dial) + dial->parentOffset) : nullptr;
    Matrix4x4 chain;
    Matrix4x4::SetActiveTransformToPrePass();
    shenChainToTargetEval(&chain, phone->hmdl, dialParent);
    workTransformMatrix = chain;
    phone->callAuthEndPosition = WorkTranslation();
    Matrix4x4::Pop(1);
    Matrix4x4::SetActiveTransformToPrePass();
    shenChainToTargetEval(&chain, phone->hmdl, dialParent);
    workTransformMatrix = chain;
    Matrix4x4::preTranslate(-0.1053f, 0.65179998f, phone->phoneVariant == 2 ? -0.26499999f : 0.26499999f);
    phone->callAuthStartPosition = WorkTranslation();
    Matrix4x4::Pop(1);
    Mt5RuntimeNode *const handset = shenGetNodeByChildIdx(phone->hmdl, 3);
    Matrix4x4::SetActiveTransformToPrePass();
    shenChainToTargetEval(&chain, phone->hmdl, handset);
    workTransformMatrix = chain;
    Matrix4x4::preTranslate(0.0f, 0.55239999f, phone->phoneVariant ? 0.5f : -0.5f);
    const Vector3 dialAuthStart = WorkTranslation();
    Matrix4x4::Pop(1);
    Matrix4x4::SetActiveTransformToPrePass();
    shenChainToTargetEval(&chain, phone->hmdl, handset);
    workTransformMatrix = chain;
    const Vector3 dialAuthEnd{chain.rows[3].x, chain.rows[3].y, chain.rows[3].z};
    Matrix4x4::Pop(1);
    phone->dialAuthStartPosition = dialAuthStart;
    phone->dialAuthEndPosition = dialAuthEnd;
    authInitAndStartSequence(*phone, dialAuthStart, dialAuthEnd, 1.0f);

    const UIPresentationSnapshot presentation = UI_GetPresentationSnapshot();
    phone->savedHelpTokenId = presentation.helpToken;
    std::memset(phone->reserved099, 0, sizeof(phone->reserved099));
    phone->savedButtonId = presentation.helpButton;
    std::memset(phone->reserved09d, 0, sizeof(phone->reserved09d));
    phone->receiverAnimTimer = 0.0f;
    phone->defaultReceiverPosition = {0.0228f, 0.55239999f, 0.2324f};
    phone->defaultReceiverRotation = {-0.0131f, 0.2484f, 0.0086000003f};
    phone->cleanup = 0;
    phone->coinChargeApplied = 0;
    phone->digitDisplaySideToggle = 1;
    phone->digitDisplayBlinkCounter = 0;
    phone->helpTokenId = phone->phoneVariant ? 9 : 8;
    SCEN_SetHelpToken(static_cast<std::uint8_t>(phone->helpTokenId), 0);

    phone->playerOwnedFrame = ShouldRun() ? 1 : 0;
    phone->savedCpctAnimState = cpctGetCurrentMode();
    PCCTL_RequestNewMode(0x10002);
    phone->mainRanLastFrame = 0;
}

bool telmGetIsActive(Character *character) {
    TelephoneWork *const phone = FindTelephoneWork(character);
    if (!phone)
        return true;
    g_telmIsActive = phone->active;
    return !g_telmIsActive;
}

void Script_TELMUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const TelmScriptParameters *>(rawParams);
    Character *const character = params->owner && context && context->task ? CHID_Find(context->task->task, params->owner) : nullptr;
    TelephoneWork *const phone = FindTelephoneWork(character);
    if (!phone)
        return;
    phone->ownerTaskData = character;
    phone->scriptContext = context;
    const auto taskDataAt = [](std::int32_t offset) { return offset ? g_pCurrentTaskData + offset : nullptr; };
    switch (params->command) {
    case 0:
        TELM_SetTargetTaskAndMode(phone, params->owner, static_cast<std::uint32_t>(params->argument2), params->argument3);
        break;
    case 1:
        if (TelephoneWork *const ownerPhone = FindTelephoneWork(ScriptCharacter(*phone, params->owner))) {
            ownerPhone->currentPeerNode = nullptr;
            ownerPhone->currentPeerMode = -1;
        }
        break;
    case 2:
        TELM_LoadHandMotionAndSetup(phone, params->owner);
        break;
    case 3:
        TELM_Cleanup(phone);
        break;
    case 4:
        context->stack->accumulator = static_cast<std::uint32_t>(phone->active);
        break;
    case 5: {
        const auto *const number = reinterpret_cast<const char *>(taskDataAt(params->argument2));
        std::int32_t entry = 0;
        while (entry < 32 && phone->phoneBookEntries[entry][0])
            ++entry;
        std::strcpy(phone->phoneBookEntries[entry], number);
        context->stack->accumulator = static_cast<std::uint32_t>(entry + 1);
        break;
    }
    case 6:
        for (char(&entry)[11] : phone->phoneBookEntries)
            std::memset(entry, 0, 10);
        g_telephoneDisplayedNumber[0] = '\0';
        break;
    case 7:
        phone->active = -1;
        break;
    case 8:
        context->stack->accumulator = phone->motionFlag0Enabled ? UINT32_MAX : 0U;
        phone->motionFlag0Enabled = params->argument2;
        NPCLogger(phone->ownerNode, 0, params->argument2);
        NPCLogger(phone->cameraNode, 0, phone->motionFlag0Enabled);
        break;
    case 9:
        context->stack->accumulator = phone->internalFlag8 ? UINT32_MAX : 0U;
        phone->internalFlag8 = params->argument2;
        break;
    case 10:
        phone->active = -1;
        phone->state = 21;
        g_telephoneDisplayedNumber[0] = '\0';
        break;
    case 11:
        std::memcpy(&phone->customReceiverOffset, taskDataAt(params->argument2), sizeof(phone->customReceiverOffset));
        break;
    case 12:
        std::memcpy(&phone->customReceiverRotation, taskDataAt(params->argument2), sizeof(phone->customReceiverRotation));
        break;
    case 13:
        phone->customReceiverPoseEnabled = 1;
        break;
    case 14:
        phone->helpTokenId = params->argument2;
        SCEN_SetHelpToken(static_cast<std::uint8_t>(params->argument2), 0);
        break;
    case 15:
        phone->savedCpctAnimState = cpctGetCurrentMode();
        PCCTL_RequestNewMode(0x10002);
        break;
    case 16:
        PCCTL_RequestNewMode(phone->savedCpctAnimState);
        break;
    case 17:
        if (cpctGetCurrentMode() != 0x10002)
            PCCTL_RequestNewMode(0x10002);
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
