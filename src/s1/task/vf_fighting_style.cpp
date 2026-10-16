#include "s1/task/vf_internal.hpp"
#include "s1/task/vf_fighting_style.hpp"

namespace shenmue::s1::vf_detail {

const FightCommandProgram *FightProgramById(std::uint16_t commandId) {
    return commandId < 60 ? &kDummyFightCommand : &kFightCommandPrograms[commandId - 60U];
}


std::uint8_t g_shenFightSubstate = 0;

std::uint8_t g_battleStageId = 0;

std::uint8_t g_battleId = 0;

VFAnimDecisionBits g_vfAnimDecisionBits{};

std::int16_t g_vfComboWindowOpen = 0;

std::int16_t g_vfComboWindowClose = 0;

float g_vfPendingBlendSpeedA = 0.0f;

float g_vfPendingBlendSpeedB = 0.0f;

std::uint8_t g_vfForceEndFlag = 0;

std::int16_t g_vfReactWindowOpen = 0;

std::int16_t g_vfReactWindowClose = 0;

std::uint32_t g_vfCurrentRyoAnimID = 0;

std::uint32_t g_vfCurrentOppoAnimID = 0;

std::int16_t g_vfComboFrameCounter = 0;

std::int16_t g_vfReactFrameCounter = 0;

std::int16_t g_vfBlendTimer = 0;

std::uint16_t g_vfRyoDamageCountdown = 0;

std::int16_t g_vfNextAnimCountdown = 0;

std::int32_t g_vfRyoCharaTaskIdx = 0;

std::int32_t g_vfOppoCharaTaskIdx = 0;

std::uint32_t g_vfPendingRyoAnimID = 0;

std::uint32_t g_vfPendingOppoAnimID = 0;

std::uint32_t g_vfLastResolvedRyoAnimID = 0;

std::uint32_t g_vfLastResolvedOppoAnimID = 0;

std::uint8_t g_vfPendingRyoDirty = 0;

std::uint8_t g_vfPendingOppoDirty = 0;

VFFightScriptResult *g_figpScriptResult = nullptr;

std::uint16_t g_figpRyoBattleCommandId = 0;

std::uint16_t g_figpOpponentBattleCommandId = 0;

std::uint32_t g_figpScriptTicks = 0;

std::int32_t g_figpSetupYaw = 0;

std::int32_t g_figpRyoRequestYaw = 0;

std::int32_t g_figpOppoRequestYaw = 0;

std::uint32_t g_figpRyoRequestedAnim = 0;

std::uint32_t g_figpOppoRequestedAnim = 0;

std::uint8_t g_figpMatcherPhase = 0;

std::uint8_t g_figpRyoRotationPending = 0;

std::uint8_t g_figpOppoRotationPending = 0;

Vector3 g_figpSetupPosition{};

std::int32_t g_figpFacingAngle = 0;

std::array<FightInputHistoryRow, 120> g_figpInputHistory{};

std::uint8_t g_figpInputHistoryCursor = 0;

FightCommandMatcher g_figpRyoMatcher{};

FightCommandMatcher g_figpOpponentMatcher{};

std::int8_t g_figpMoveSelectResult = 0;

std::uint8_t g_figpResolveComplete = 0;

MotionInformationWork *FightScriptMotion(Character *character) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    return manager ? &manager->motion : nullptr;
}

void FightScriptSetPoseActive(std::uint8_t active) {
    if (FightPoseWork *const pose = FIGP_GetWork(FindCharacter(nullptr, g_vfRyoCharaTaskIdx)))
        pose->active = active;
}



void LoadFightMoveFromTable(FightCommandMatcher &matcher) {
    const std::uint16_t commandId = matcher.commandId;
    matcher.matchedOrDisabled = commandId == 0xffffU;
    matcher.bootstrapMaskHigh = 0;
    matcher.initCountdown = 0xf0;
    matcher.retryCountdown = 30;
    matcher.inputWindowEnd = 120;
    matcher.resultKind = 0;
    matcher.matcherPhase = 0;
    matcher.resultCode = 0;
    matcher.classifyCode = 0;
    matcher.programInputIndex = 0;
    matcher.ended = 0;
    matcher.pc = 0;
    matcher.historyCursor = 0;
    matcher.hasDirectionHistory = 0;
    matcher.pendingInput = 0;
    matcher.previousInput = 0;
    matcher.savedInput = 0;
    matcher.lastDirection = 0;
    matcher.selectedAnimation = 0;
    matcher.matchedInputCount = 0;
    if (commandId == 0xffffU)
        return;
    const FightCommandProgram *const program = FightProgramById(commandId);
    matcher.inputOpcodeCount = 0;
    for (std::uint8_t row = 0; row < program->count; ++row) {
        if (program->rows[row].opcode == FightCommandOpcode::InputMask)
            ++matcher.inputOpcodeCount;
        if (program->rows[row].opcode == FightCommandOpcode::End)
            break;
    }
}



bool ShouldMirrorFightingStyleInput() {
    Character *const ryo = FindCharacter(nullptr, g_vfRyoCharaTaskIdx);
    const Vector3 position = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const std::uint16_t yaw = static_cast<std::uint16_t>(g_figpSetupYaw);
    const std::uint16_t folded = yaw > 0x8000U ? static_cast<std::uint16_t>(0U - yaw) : yaw;
    const float negativeSine = -sin_fixed16(static_cast<std::int16_t>(yaw));
    const float negativeCosine = -sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
    g_debugTextPosition = {3.0f, 4.0f};
    return negativeCosine * (g_CameraEyePos.x - position.x) + -negativeSine * (g_CameraEyePos.z - position.z) < 0.0f;
}



std::uint8_t PackFightingStyleInput(std::uint16_t input) {
    std::uint8_t packed = 0;
    if ((input & 0x400U) != 0)
        packed |= 2;
    if ((input & 0x200U) != 0)
        packed |= 8;
    if ((input & 4U) != 0)
        packed |= 1;
    if ((input & 2U) != 0)
        packed |= 4;
    const bool mirror = ShouldMirrorFightingStyleInput();
    if ((input & 0x8000U) != 0)
        packed |= mirror ? 0x40U : 0x80U;
    if ((input & 0x4000U) != 0)
        packed |= mirror ? 0x80U : 0x40U;
    if ((input & 0x1000U) != 0)
        packed |= 0x20U;
    if ((input & 0x2000U) != 0)
        packed |= 0x10U;
    return packed;
}



void RecordFightingStyleInput() {
    FightInputHistoryRow &row = g_figpInputHistory[g_figpInputHistoryCursor];
    const std::uint8_t triggered = PackFightingStyleInput(static_cast<std::uint16_t>(g_inputState.slots[0].leftStick >> 16U));
    row.held = PackFightingStyleInput(g_inputState.slots[0].held);
    row.triggered = triggered;
    g_figpInputHistoryCursor = g_figpInputHistoryCursor == 119 ? 0 : static_cast<std::uint8_t>(g_figpInputHistoryCursor + 1U);
}


std::uint8_t FightDirectionOrdinal(std::uint16_t input) {
    switch (input & 0xf0U) {
    case 0x10:
        return 0x80;
    case 0x40:
        return 0x40;
    case 0x50:
        return 0x60;
    case 0x60:
        return 0x20;
    case 0x80:
        return 0xc0;
    case 0x90:
        return 0xa0;
    case 0xa0:
        return 0xe0;
    default:
        return 0;
    }
}

std::uint32_t AccumulateFightInput(FightCommandMatcher &matcher, const FightInputHistoryRow &row, std::uint32_t required) {
    const std::uint32_t previous = matcher.pendingInput;
    std::uint32_t combined;
    if (!matcher.hasDirectionHistory || (row.triggered & required & 0xf0U) != 0) {
        combined = previous | row.triggered;
        if ((row.triggered & 0xf0U) != 0)
            matcher.hasDirectionHistory = 1;
    } else {
        combined = previous | (row.triggered & 0x0fU);
        const std::uint8_t currentOrdinal = FightDirectionOrdinal(row.held);
        const std::uint8_t previousOrdinal = FightDirectionOrdinal(matcher.lastDirection);
        const std::uint8_t delta = currentOrdinal <= previousOrdinal ? static_cast<std::uint8_t>(previousOrdinal - currentOrdinal) :
                                                                       static_cast<std::uint8_t>(currentOrdinal - previousOrdinal);
        if ((row.held & 0xf0U) != 0 && (matcher.lastDirection & 0xf0U) != 0 && delta >= 0x40U)
            combined |= row.held & 0xf0U;
        else if ((row.held & 0xf0U) == 0)
            matcher.hasDirectionHistory = 0;
    }
    if ((combined & 0xf0U) != 0)
        matcher.lastDirection = static_cast<std::uint16_t>(combined & 0xf0U);
    matcher.previousInput = previous;
    matcher.pendingInput = 0;
    if ((required & 0xf0U) != 0 && (combined & required & 0xf0U) != 0)
        combined = (combined & 0x0fU) | (required & 0xf0U);
    return combined;
}


void SelectFightAnimation(std::uint32_t animation) {
    g_vfPendingRyoAnimID = animation;
    g_vfCurrentRyoAnimID = animation;
    const std::uint8_t *const cue = shenLookupAnimationIndex(static_cast<std::uint16_t>(animation), 2);
    if (g_battleId >= 2 && static_cast<std::uint8_t>(g_battleStageId - 3U) <= 3U) {
        g_vfComboWindowOpen = 0;
        g_vfComboWindowClose = 1000;
    } else if (cue) {
        std::memcpy(&g_vfComboWindowOpen, cue + 2, sizeof(g_vfComboWindowOpen));
        std::memcpy(&g_vfComboWindowClose, cue + 4, sizeof(g_vfComboWindowClose));
    } else {
        g_vfComboWindowOpen = 0;
        g_vfComboWindowClose = 0;
    }
    g_vfComboFrameCounter = 0;
}



std::uint8_t FightSpecialCondition(const FightCommandMatcher &matcher) {
    if (g_battleId < 2 || matcher.matchedInputCount != 0 || static_cast<std::uint8_t>(g_battleStageId - 3U) > 3U)
        return 0;
    if (g_vfAnimDecisionBits.two != 1)
        return 1;
    return g_vfReactFrameCounter <= g_vfReactWindowOpen ? 0 : 3;
}


void LookupFightComboWindow() {
    if (const std::uint8_t *const cue = shenLookupAnimationIndex(static_cast<std::uint16_t>(g_vfCurrentRyoAnimID), 2)) {
        std::uint16_t open = 0;
        std::memcpy(&open, cue + 2, sizeof(open));
        g_vfComboWindowOpen = static_cast<std::int16_t>(open - 1U);
        std::memcpy(&g_vfComboWindowClose, cue + 4, sizeof(g_vfComboWindowClose));
        g_vfComboFrameCounter = 0;
    } else if (g_vfPendingRyoAnimID == 792) {
        g_vfComboWindowOpen = 3;
        g_vfComboWindowClose = 6;
        g_vfComboFrameCounter = 0;
    } else if (g_vfPendingRyoAnimID == 870) {
        g_vfComboWindowOpen = 6;
        g_vfComboWindowClose = 9;
        g_vfComboFrameCounter = 0;
    }
}



void SelectFallbackFightAnimation(std::uint8_t held, std::uint8_t triggered, FightCommandMatcher &matcher) {
    g_vfAnimDecisionBits.one = 2;
    g_vfAnimDecisionBits.two = 2;
    if (matcher.matchedInputCount)
        return;
    const MotionInformationWork *const motion = FightScriptMotion(FindCharacter(nullptr, g_vfRyoCharaTaskIdx));
    if (!motion)
        return;
    const std::uint16_t current = motion->currentAnimId;
    if (current != 208 && current != 215 && current != 841 && current != 850 && current != 861)
        return;
    const bool mirror = ShouldMirrorFightingStyleInput();
    std::uint32_t animation = g_vfPendingRyoAnimID;
    if ((triggered & 4U) != 0) {
        if ((held & 0x80U) != 0)
            animation = 1059;
        else if ((held & 0x40U) != 0)
            animation = 1246;
        else if ((!mirror && (held & 0x20U) != 0) || (mirror && (held & 0x10U) != 0))
            animation = 1272;
        else if ((!mirror && (held & 0x10U) != 0) || (mirror && (held & 0x20U) != 0))
            animation = 1271;
        else
            animation = mirror ? 1271 : 1272;
    } else {
        if ((held & 0x80U) != 0)
            animation = 208;
        else if ((held & 0x40U) != 0)
            animation = 215;
        else if ((!mirror && (held & 0x20U) != 0) || (mirror && (held & 0x10U) != 0))
            animation = 861;
        else if ((!mirror && (held & 0x10U) != 0) || (mirror && (held & 0x20U) != 0))
            animation = 841;
    }
    if ((held & 8U) != 0)
        animation = 583;
    if ((triggered & 1U) != 0)
        animation = 792;
    if ((triggered & 2U) != 0)
        animation = 870;
    g_vfPendingRyoAnimID = animation;
    if (animation == 792 || animation == 870)
        LookupFightComboWindow();
}



void TrySelectFallbackFightAnimation(std::uint8_t held, std::uint8_t triggered, const FightCommandMatcher &opponent) {
    const FightCommandMatcher &ryo = g_figpRyoMatcher;
    if (ryo.matchedInputCount || opponent.matchedInputCount || ((ryo.matcherPhase - 1U) & 0xfdU) == 0 || ((opponent.matcherPhase - 1U) & 0xfdU) == 0)
        return;
    const MotionInformationWork *const motion = FightScriptMotion(FindCharacter(nullptr, g_vfRyoCharaTaskIdx));
    if (!motion)
        return;
    const std::uint16_t current = motion->currentAnimId;
    if (current != 208 && current != 215 && current != 841 && current != 850 && current != 861)
        return;
    const bool mirror = ShouldMirrorFightingStyleInput();
    std::uint8_t direction = 0;
    if ((!mirror && (held & 0x10U) != 0) || (mirror && (held & 0x20U) != 0))
        direction = 3;
    if ((!mirror && (held & 0x20U) != 0) || (mirror && (held & 0x10U) != 0))
        direction = 7;
    if ((held & 0x40U) != 0)
        direction = 5;
    if ((held & 0x80U) != 0)
        direction = 1;
    std::uint16_t animation = 0;
    if ((triggered & 4U) != 0) {
        animation = direction == 1 ? 1059 : direction == 3 ? 1271 : direction == 5 ? 1246 : direction == 7 ? 1272 : mirror ? 1271 : 1272;
    } else if ((triggered & 0xf0U) != 0) {
        animation = direction == 1 ? 208 : direction == 3 ? 841 : direction == 5 ? 215 : direction == 7 ? 861 : 0;
    }
    if ((triggered & 8U) != 0)
        animation = 583;
    if ((triggered & 1U) != 0)
        animation = 792;
    if ((triggered & 2U) != 0)
        animation = 870;
    if (!animation)
        return;


    Character *const ryoCharacter = FindCharacter(nullptr, g_vfRyoCharaTaskIdx);
    const Vector3 position = ryoCharacter ? ryoCharacter->entity.node.position : Vector3{};
    const std::uint16_t yaw = ryoCharacter ? static_cast<std::uint16_t>(ryoCharacter->entity.node.rotation.y) : 0;
    Vector3 root{};
    MOTN_ReadRootPositionAtFrame(animation, static_cast<std::uint16_t>(motion->basePoseTemplateIndex), -1.0f, root, true);
    const std::uint16_t folded = yaw > 0x8000U ? static_cast<std::uint16_t>(0U - yaw) : yaw;
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
    const float deltaX = root.x * cosine + root.z * sine + position.x - g_figpSetupPosition.x;
    const float deltaZ = -(root.x * sine) + root.z * cosine + position.z - g_figpSetupPosition.z;
    if (deltaX * deltaX + deltaZ * deltaZ >= 1.0f)
        return;
    g_vfPendingRyoAnimID = animation;
    g_vfPendingRyoDirty = 1;
}




void AdvanceFightCommandMatcher(FightCommandMatcher &matcher) {
    const FightCommandProgram *const program = FightProgramById(matcher.commandId);
    const std::uint8_t initialHistory = matcher.historyCursor;
    std::uint8_t pc = matcher.pc;
    std::uint8_t cursor = initialHistory;
    bool stop = false;
    if (!initialHistory) {
        matcher.previousInput = 0;
        matcher.savedInput = 0;
        matcher.pendingInput = 0;
        matcher.hasDirectionHistory = 0;
    }
    for (;;) {
        const FightCommandRow &row = program->rows[pc];
        switch (row.opcode) {
        case FightCommandOpcode::SelectAltA:
        case FightCommandOpcode::SelectAltB:
            if ((matcher.matcherPhase == 1 || matcher.matcherPhase == 3) && !matcher.pendingInput && !matcher.previousInput) {
                ++pc;
                matcher.pc = pc;
                matcher.selectedAnimation = row.argument;
                matcher.historyCursor = cursor;
                continue;
            }
            matcher.matcherPhase = 2;
            if (!matcher.classifyCode)
                matcher.classifyCode = 13;
            matcher.matchedOrDisabled = 1;
            break;
        case FightCommandOpcode::SelectContinue: {
            if ((matcher.matcherPhase & 0xfcU) == 0 && matcher.matcherPhase != 2) {
                matcher.selectedAnimation = row.argument;
                bool failed = false;
                if (g_vfAnimDecisionBits.one == 2) {
                    matcher.resultCode = 19;
                    failed = true;
                } else if (const std::uint8_t condition = FightSpecialCondition(matcher)) {
                    if (condition <= 3) {
                        g_vfPendingRyoAnimID = 583;
                        if (condition == 3) {
                            const std::uint32_t saved = matcher.savedInput;
                            SelectFallbackFightAnimation(static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].held),
                                static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].triggered), matcher);
                        }
                        matcher.resultCode = condition == 1 ? 18 : condition == 2 ? 21 : 19;
                        failed = true;
                    }
                } else {
                    SelectFightAnimation(row.argument);
                    g_vfNextAnimCountdown = 1;
                    g_vfAnimDecisionBits.four = 1;
                    std::uint8_t lookahead = static_cast<std::uint8_t>(pc + 1U);
                    while (program->rows[lookahead].opcode != FightCommandOpcode::InputMask && program->rows[lookahead].opcode != FightCommandOpcode::End)
                        ++lookahead;
                    const std::uint32_t nextMask = program->rows[lookahead].opcode == FightCommandOpcode::InputMask ? program->rows[lookahead].argument : 0U;
                    if (const std::uint32_t overlap = nextMask & matcher.pendingInput) {
                        matcher.resultCode = 14;
                        matcher.classifyMask = overlap;
                        failed = true;
                    }
                }
                if (failed) {
                    matcher.resultKind = 2;
                    g_figpMatcherPhase = 3;
                }
                ++matcher.matchedInputCount;
            }
            ++pc;
            matcher.historyCursor = cursor;
            matcher.pc = pc;
            continue;
        }
        case FightCommandOpcode::End:
            matcher.ended = 1;
            if (matcher.matcherPhase == 3) {
                matcher.matcherPhase = 1;
                matcher.classifyCode = 0;
            }
            matcher.matchedOrDisabled = 1;
            break;
        case FightCommandOpcode::InputMask: {
            if (static_cast<std::int32_t>(cursor) > static_cast<std::int32_t>(matcher.inputWindowEnd)) {
                if (!matcher.matcherPhase) {
                    matcher.classifyCode = 20;
                    matcher.matcherPhase = 2;
                } else if (matcher.matcherPhase == 3) {
                    matcher.classifyCode = 0;
                    matcher.matcherPhase = 2;
                }
                stop = true;
            }
            if (cursor >= g_figpInputHistoryCursor)
                break;
            matcher.programInputIndex = pc;
            const std::uint32_t combined = AccumulateFightInput(matcher, g_figpInputHistory[cursor], row.argument);
            if (combined == row.argument) {
                ++pc;
                matcher.matcherPhase = 3;
                matcher.pendingInput = 0;
                matcher.previousInput = 0;
            } else if (matcher.previousInput) {
                matcher.matcherPhase = 2;
                stop = true;
                matcher.matchedOrDisabled = 1;
                matcher.savedInput = combined;
            } else if ((combined & row.argument) == row.argument) {
                ++pc;
                matcher.pendingInput = combined ^ row.argument;
            } else {
                matcher.pendingInput = combined;
            }
            ++cursor;
            if (!stop)
                continue;
            break;
        }
        case FightCommandOpcode::Window:
            matcher.inputWindowEnd =
                matcher.inputWindowEnd == 120 ? static_cast<std::int8_t>(row.argument) : static_cast<std::int8_t>(cursor + row.argument - 1U);
            ++pc;
            matcher.pc = pc;
            matcher.inputWindowLength = static_cast<std::uint8_t>(row.argument + 1U);
            matcher.historyCursor = cursor;
            continue;
        case FightCommandOpcode::BlendSpeedA:
            ++pc;
            g_vfPendingBlendSpeedA = static_cast<float>(static_cast<std::int32_t>(row.argument));
            matcher.pc = pc;
            continue;
        case FightCommandOpcode::BlendSpeedB:
            g_vfPendingBlendSpeedB = static_cast<float>(static_cast<std::int32_t>(row.argument));
            if (static_cast<double>(g_vfPendingBlendSpeedB) < 0.1)
                g_vfPendingBlendSpeedB = 0.001f;
            ++pc;
            matcher.pc = pc;
            continue;
        case FightCommandOpcode::ForceEnd:
            ++pc;
            g_vfForceEndFlag = static_cast<std::uint8_t>(row.argument);
            matcher.pc = pc;
            continue;
        }
        if (initialHistory == matcher.historyCursor) {
            matcher.pendingInput = 0;
            matcher.previousInput = 0;
        }
        return;
    }
}

std::uint8_t ClassifyButtonMask(std::uint32_t mask, std::uint8_t base) {
    if ((mask & 1U) != 0)
        return base;
    if ((mask & 2U) != 0)
        return static_cast<std::uint8_t>(base + 1U);
    if ((mask & 8U) != 0)
        return static_cast<std::uint8_t>(base + 2U);
    if ((mask & 4U) != 0)
        return static_cast<std::uint8_t>(base + 3U);
    return 0;
}




FightCommandClassification ClassifyRecentFightInput() {
    const FightCommandProgram *const program = FightProgramById(g_figpRyoMatcher.commandId);
    FightCommandMatcher replay{};
    std::int8_t windowEnd = 120;
    std::int32_t cursor = 0;
    std::int32_t lastMatchedHistory = 0;
    std::uint8_t pc = 0;
    std::uint8_t selectCount = 0;
    std::uint8_t matchPhase = 0;
    std::uint8_t lastMatchedMask = 0;
    std::uint8_t code = 0;
    std::uint8_t length = 0;
    std::uint8_t count = 0;
    std::uint32_t diagnostic = 0;
    std::uint32_t mask = 0;
    for (bool done = false; !done;) {
        const FightCommandRow &row = program->rows[pc];
        const std::uint32_t argument = row.argument;
        switch (row.opcode) {
        case FightCommandOpcode::SelectAltA:
        case FightCommandOpcode::SelectAltB: {
            if (!(replay.pendingInput | replay.previousInput) || (code && code != 13)) {
                ++pc;
                break;
            }
            const std::uint32_t residual = replay.pendingInput ? (replay.previousInput & ~0xffU) | (replay.pendingInput & 0xffU) : replay.previousInput;
            if (const std::uint8_t button = ClassifyButtonMask(residual, 8))
                code = button;
            else if ((residual & 0xf0U) != 0)
                code = (lastMatchedMask & 0xf0U) != 0 ? 1 : 12;
            done = true;
            break;
        }
        case FightCommandOpcode::SelectContinue:
            ++selectCount;
            ++pc;
            break;
        case FightCommandOpcode::End:
            if (lastMatchedHistory > windowEnd) {
                code = 16;
                length = static_cast<std::uint8_t>(lastMatchedHistory - windowEnd);
                count = selectCount;
            }
            done = true;
            break;
        case FightCommandOpcode::InputMask: {
            g_figpRyoMatcher.programInputIndex = pc;
            if (cursor >= g_figpInputHistoryCursor) {
                if (!matchPhase) {
                    code = 20;
                } else if (matchPhase == 3) {
                    if ((argument & 0xf0U) != 0)
                        code = 7;
                    else if (const std::uint8_t button = ClassifyButtonMask(argument, 3))
                        code = button;
                }
                done = true;
                break;
            }
            const std::uint32_t combined = AccumulateFightInput(replay, g_figpInputHistory[cursor], argument);
            if (combined == argument) {
                replay.previousInput = 0;
                matchPhase = 3;
                if (cursor > windowEnd) {
                    count = selectCount;
                    length = static_cast<std::uint8_t>(cursor - windowEnd);
                    code = 16;
                    done = true;
                    break;
                }
                lastMatchedHistory = cursor;
                ++pc;
                lastMatchedMask = static_cast<std::uint8_t>(argument);
            } else if (!replay.previousInput) {
                if ((combined & argument) == argument) {
                    replay.pendingInput = combined ^ argument;
                    ++pc;
                } else {
                    replay.pendingInput = combined;
                }
            } else {
                matchPhase = 2;
                done = true;
                if (!code) {
                    if ((combined & argument) != 0) {
                        diagnostic = combined & (combined ^ argument);
                        if (const std::uint8_t button = ClassifyButtonMask(diagnostic, 8))
                            code = button;
                        else if ((diagnostic & 0xf0U) != 0)
                            code = (argument & 0xf0U) != 0 ? 1 : 12;
                    } else if ((combined & 0x0fU) != 0) {
                        diagnostic = combined;
                        code = ClassifyButtonMask(combined, 8);
                    } else if ((combined & 0xf0U) != 0) {
                        diagnostic = combined;
                        code = (argument & 0xf0U) != 0 ? 1 : 12;
                    }
                }
                if (!code) {
                    std::uint32_t future = 0;
                    for (std::int32_t ahead = 1; ahead <= 6; ++ahead) {
                        const std::int32_t index = cursor + ahead;
                        future |= index < static_cast<std::int32_t>(g_figpInputHistory.size()) ? g_figpInputHistory[index].triggered : 0U;
                        if ((argument & (future | combined)) == argument) {
                            code = 15;
                            mask = future;
                            length = static_cast<std::uint8_t>(ahead + 1);
                            break;
                        }
                    }
                }
                if (!code && (combined & argument) != 0) {
                    diagnostic = combined;
                    const std::uint32_t missing = argument & (combined ^ argument);
                    code = ClassifyButtonMask(missing, 3);
                    if (!code && (missing & 0xf0U) != 0)
                        code = 7;
                }
                if (!code) {
                    if ((argument & 0xf0U) == 0)
                        code = 13;
                    else if ((argument & 0xf0U) != (combined & 0xf0U) && (combined & 0xf0U) != 0)
                        code = 1;
                    else if ((argument & 0x0fU) == 0 && (combined & 0x0fU) != 0)
                        code = 2;
                    else
                        code = 13;
                }
            }
            ++cursor;
            break;
        }
        case FightCommandOpcode::Window:
            windowEnd = windowEnd == 120 ? static_cast<std::int8_t>(argument) : static_cast<std::int8_t>(cursor + static_cast<std::int32_t>(argument) - 1);
            ++pc;
            break;
        default:
            ++pc;
            break;
        }
    }
    if (code == 1 || static_cast<std::uint8_t>(code - 3U) <= 9U)
        mask = diagnostic;
    FightCommandClassification result{};
    result.code = code;
    result.mask = mask;
    result.length = length;
    result.referenceCount = count;
    return result;
}


std::int8_t UpdateFightCommandMatchers() {
    FightCommandMatcher &ryo = g_figpRyoMatcher;
    FightCommandMatcher &opponent = g_figpOpponentMatcher;
    if (g_figpMatcherPhase == 0) {
        g_figpMatcherPhase = 1;
        return -1;
    }
    if (g_figpMatcherPhase == 4)
        return 0;
    if (g_figpMatcherPhase == 3) {
        RecordFightingStyleInput();
        if (!--ryo.retryCountdown) {
            ryo.bootstrapMaskHigh = 1;
            if (ryo.matcherPhase != 1 && (!ryo.classifyCode || ryo.classifyCode == 13)) {
                const FightCommandClassification classification = ClassifyRecentFightInput();
                ryo.matcherPhase = 2;
                ryo.classifyCode = classification.code;
                ryo.classifyMask = classification.mask;
                ryo.classifyLength = classification.length;
                ryo.classifyReferenceCount = ryo.matchedInputCount;
                opponent.classifyReferenceCount = opponent.matchedInputCount;
            }
            g_figpMatcherPhase = 4;
        }

        const FightInputHistoryRow previous = g_figpInputHistoryCursor ? g_figpInputHistory[g_figpInputHistoryCursor - 1U] : FightInputHistoryRow{};
        TrySelectFallbackFightAnimation(static_cast<std::uint8_t>(previous.held), static_cast<std::uint8_t>(previous.triggered), opponent);
        return -1;
    }
    if (g_figpMatcherPhase == 1) {
        if (g_vfAnimDecisionBits.one) {
            g_figpMatcherPhase = 3;
            return -1;
        }
        if (!--ryo.initCountdown) {
            ryo.resultKind = 2;
            ryo.resultCode = 20;
            g_figpMatcherPhase = 4;
            return -1;
        }
        if ((static_cast<std::uint16_t>(g_inputState.slots[0].leftStick >> 16U) & 0xf606U) == 0)
            return -1;
        ryo.inputWindowLength = 120;
        ryo.inputWindowEnd = 120;
        opponent.inputWindowLength = 120;
        opponent.inputWindowEnd = 120;
        g_figpMatcherPhase = 2;
    }
    RecordFightingStyleInput();
    if (g_vfAnimDecisionBits.one) {
        g_figpMatcherPhase = 3;
        return -1;
    }
    if (!--ryo.inputWindowLength) {
        if (!opponent.ended && !ryo.ended) {
            if (opponent.matchedInputCount && opponent.matchedInputCount > ryo.matchedInputCount && !FightSpecialCondition(opponent)) {
                g_vfAnimDecisionBits.three = 1;
                opponent.classifyCode = 23;
                opponent.resultKind = 1;
                ryo.matchedOrDisabled = 1;
                g_vfAnimDecisionBits.four = 1;
                if (!g_vfAnimDecisionBits.one)
                    g_vfAnimDecisionBits.one = 1;
                g_figpMatcherPhase = 4;
                return -1;
            }
            ryo.resultKind = 2;
            ryo.bootstrapMaskHigh = 1;
            if (!opponent.matchedInputCount && !ryo.matchedInputCount) {
                const std::uint32_t saved = ryo.savedInput;
                SelectFallbackFightAnimation(static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].held),
                    static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].triggered), ryo);
            }
            g_figpMatcherPhase = 3;
            return -1;
        }
        ryo.matchedOrDisabled = 1;
        opponent.matchedOrDisabled = 1;
    }
    if (opponent.commandId != 0xffffU && !opponent.matchedOrDisabled)
        AdvanceFightCommandMatcher(opponent);
    if (!ryo.matchedOrDisabled)
        AdvanceFightCommandMatcher(ryo);
    if (!opponent.matchedOrDisabled || !ryo.matchedOrDisabled)
        return -1;
    if (opponent.commandId != 0xffffU && opponent.ended) {
        if (g_vfAnimDecisionBits.one != 2 && !FightSpecialCondition(opponent)) {
            SelectFightAnimation(opponent.selectedAnimation);
            g_vfAnimDecisionBits.three = 1;
            opponent.classifyCode = 22;
            g_vfAnimDecisionBits.four = 1;
            if (!g_vfAnimDecisionBits.one)
                g_vfAnimDecisionBits.one = 1;
            opponent.resultKind = 1;
            g_figpMatcherPhase = 4;
        }
        return -1;
    }
    if (ryo.ended) {
        if (ryo.bootstrapMaskHigh) {
            if (!opponent.matchedInputCount && !ryo.matchedInputCount) {
                const std::uint32_t saved = ryo.savedInput;
                SelectFallbackFightAnimation(static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].held),
                    static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].triggered), ryo);
            }
            ryo.resultCode = 17;
            ryo.resultKind = 2;
            g_figpMatcherPhase = 3;
            return -1;
        }
        if (g_vfAnimDecisionBits.one == 2) {
            ryo.resultKind = 2;
            ryo.resultCode = 19;
            g_figpMatcherPhase = 3;
            return -1;
        }
        const std::uint8_t condition = FightSpecialCondition(ryo);
        if (condition) {
            if (!ryo.matchedInputCount)
                g_vfPendingRyoAnimID = 583;
            if (condition == 3 && !opponent.matchedInputCount && !ryo.matchedInputCount) {
                const std::uint32_t saved = ryo.savedInput;
                SelectFallbackFightAnimation(static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].held),
                    static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].triggered), ryo);
            }
            ryo.resultKind = 2;
            ryo.resultCode = condition == 1 ? 18 : condition == 2 ? 21 : 19;
            g_figpMatcherPhase = 3;
            return -1;
        }
        g_vfAnimDecisionBits.three = 1;
        if (!g_vfAnimDecisionBits.one)
            g_vfAnimDecisionBits.one = 1;
        if (opponent.matchedInputCount > ryo.matchedInputCount) {
            opponent.classifyCode = 23;
            opponent.resultKind = 1;
        } else {
            ryo.resultKind = 1;
            SelectFightAnimation(ryo.selectedAnimation);
        }
        g_vfAnimDecisionBits.four = 1;
        g_figpMatcherPhase = 4;
        return -1;
    }
    if (ryo.matcherPhase == 2) {
        if (!opponent.matchedInputCount && !ryo.matchedInputCount) {
            const std::uint32_t saved = ryo.savedInput;
            SelectFallbackFightAnimation(static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].held),
                static_cast<std::uint8_t>(saved ? saved : g_figpInputHistory[0].triggered), ryo);
        }
        ryo.resultKind = 2;
        g_figpMatcherPhase = 3;
    }
    return -1;
}



void InitializeFightingStyleMatcher() {
    g_vfLastResolvedRyoAnimID = 0;
    g_vfLastResolvedOppoAnimID = 0;
    for (FightInputHistoryRow &row : g_figpInputHistory)
        row = {};
    g_vfAnimDecisionBits.mirror = g_battleId != 0;
    g_figpInputHistoryCursor = 0;
    g_figpMatcherPhase = 0;
    g_figpRyoRotationPending = 0;
    g_vfAnimDecisionBits.one = 0;
    g_vfAnimDecisionBits.two = 0;
    g_vfCurrentRyoAnimID = 0;
    g_vfCurrentOppoAnimID = 0;
    g_vfPendingRyoDirty = 0;
    g_vfPendingOppoDirty = 0;
    g_vfAnimDecisionBits.four = 0;
    g_vfAnimDecisionBits.extra = 0;
    g_vfBlendTimer = 20;
    g_vfRyoDamageCountdown = 0;
    g_vfAnimDecisionBits.three = 0;
    g_vfNextAnimCountdown = 0;
    g_figpScriptTicks = 0;
    g_figpOppoRotationPending = 0;
    g_vfPendingBlendSpeedA = -1.0f;
    g_vfPendingBlendSpeedB = -1.0f;
    g_vfForceEndFlag = 0;
    g_figpRyoRequestYaw = 0;
    g_figpOppoRequestYaw = 0;
    g_figpRyoRequestedAnim = 0;
    g_figpOppoRequestedAnim = 0;
    FightScriptSetPoseActive(0);
    g_vfBattleForceEndRequested = 0;
    LoadFightMoveFromTable(g_figpRyoMatcher);
    LoadFightMoveFromTable(g_figpOpponentMatcher);
}


void RequestFightAnimation(Character *character, std::uint32_t animation, bool opponent) {
    const std::int32_t yaw = character ? static_cast<std::int16_t>(cpctGetAccumYaw(character)) : 0;
    if (opponent) {
        g_figpOppoRequestedAnim = animation;
        g_figpOppoRequestYaw = yaw;
    } else {
        g_figpRyoRequestedAnim = animation;
        g_figpRyoRequestYaw = yaw;
    }
    MotionInformationWork *const motion = FightScriptMotion(character);
    if (!motion)
        return;
    motion->motionFlags &= ~2U;
    motion->pendingAnimId = static_cast<std::uint16_t>(animation);
}



void RestoreFightAnimationRotation(Character *character, bool opponent) {
    const std::uint32_t requested = opponent ? g_figpOppoRequestedAnim : g_figpRyoRequestedAnim;
    std::int32_t yaw;
    if (requested == 1177 || requested == 1178) {
        yaw = static_cast<std::uint16_t>(g_figpRyoRequestYaw - 0x6000);
    } else if (requested == 1213 || requested == 1214) {
        yaw = static_cast<std::uint16_t>(g_figpRyoRequestYaw);
    } else if (requested == 1539) {
        yaw = static_cast<std::uint16_t>(g_figpRyoRequestYaw + 0x8000);
    } else {
        std::uint8_t &pending = opponent ? g_figpOppoRotationPending : g_figpRyoRotationPending;
        if (!pending)
            return;
        pending = 0;
        yaw = character ? static_cast<std::uint16_t>(cpctGetAccumYaw(character)) : 0;
    }
    if (character)
        character->entity.node.rotation = Vector3i{.x = 0, .y = yaw, .z = 0};
}

bool FightAnimationEnded(Character *character) {
    const MotionInformationWork *const motion = FightScriptMotion(character);
    return !motion || (motion->motionFlags & 2U) != 0;
}

void ApplyRelativeFightTransform(Character *target, Character *reference, const Vector3 &offset, std::uint16_t yawOffset) {
    if (!target || !reference)
        return;
    const Vector3 referencePosition = GetPositionForEntity(reference->entity.node);
    Vector3i rotation = GetEulerAnglesForEntity(reference->entity.node);
    rotation.y = static_cast<std::uint16_t>(rotation.y + yawOffset);
    ApplyEulerAnglesToNode(target->entity.node, rotation);
    const std::uint16_t yaw = static_cast<std::uint16_t>(rotation.y);
    const std::uint16_t folded = yaw > 0x8000U ? static_cast<std::uint16_t>(0U - yaw) : yaw;
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
    const Vector3 position{
        referencePosition.x + cosine * offset.x + sine * offset.z,
        referencePosition.y,
        referencePosition.z + cosine * offset.z - sine * offset.x,
    };
    shenApplyTranslationToAll(target->entity.node, position);
}

void ApplyFightAnimationRotation(Character *character, std::uint16_t animation, bool opponent) {
    if (!character)
        return;
    if (opponent) {
        if (animation == 311 || animation == 845) {
            const Vector3i rotation =
                FindCharacter(nullptr, g_vfRyoCharaTaskIdx) ? GetEulerAnglesForEntity(FindCharacter(nullptr, g_vfRyoCharaTaskIdx)->entity.node) : Vector3i{};
            ApplyEulerAnglesToNode(character->entity.node, rotation);
        } else if (animation == 1179) {
            ApplyRelativeFightTransform(character, character, {0.0241870005f, 0.0f, 0.689467013f}, 0);
            g_figpOppoRotationPending = 1;
        } else if (animation == 1180) {
            ApplyRelativeFightTransform(character, character, {0.0948299989f, 0.0f, 1.35087800f}, 0x8000);
            g_figpOppoRotationPending = 1;
        }
        return;
    }
    if (g_battleId) {
        if (animation == 1167) {
            ApplyRelativeFightTransform(character, character, {0.0f, 0.0f, 1.29999995f}, 0x8000);
        } else if (animation == 1177) {
            ApplyRelativeFightTransform(character, FindCharacter(nullptr, g_vfOppoCharaTaskIdx), {0.0241870005f, 0.0f, 0.689467013f}, 0);
            g_figpRyoRotationPending = 1;
        } else if (animation == 1178) {
            ApplyRelativeFightTransform(character, FindCharacter(nullptr, g_vfOppoCharaTaskIdx), {0.0948299989f, 0.0f, 1.35087800f}, 0x8000);
            g_figpRyoRotationPending = 1;
        }
        return;
    }
    if (animation == 850) {
        FightScriptSetPoseActive(0);
    } else if (animation == 1167) {
        ApplyRelativeFightTransform(character, character, {0.0f, 0.0f, 1.29999995f}, 0x8000);
    } else if (animation == 1177) {
        ApplyRelativeFightTransform(character, character, {0.648925006f, 0.0f, 0.542532027f}, 0);
        g_figpRyoRotationPending = 1;
    } else if (animation == 1178) {
        ApplyRelativeFightTransform(character, character, {}, 0);
        g_figpRyoRotationPending = 1;
    } else if (animation == 1213) {
        ApplyRelativeFightTransform(character, character, {}, 0);
        g_figpRyoRotationPending = 1;
    } else if (animation == 1214) {
        ApplyRelativeFightTransform(character, character, {-0.0910760015f, 0.0f, 0.695838988f}, 0);
        g_figpRyoRotationPending = 1;
    } else if (animation == 1539) {
        g_figpRyoRotationPending = 1;
    }
}


bool CanResolveFightAnimation(Character *character) {
    if (character == FindCharacter(nullptr, g_vfRyoCharaTaskIdx)) {
        const bool outsideReactionStage = static_cast<std::uint8_t>(g_battleStageId - 3U) > 3U;
        if ((g_vfAnimDecisionBits.two != 1 || outsideReactionStage || !g_vfAnimDecisionBits.four) &&
            (g_vfAnimDecisionBits.extra ||
                (g_battleId >= 2 && !outsideReactionStage && (g_vfAnimDecisionBits.two != 1 || g_vfReactFrameCounter > g_vfReactWindowOpen)))) {
            return false;
        }
    } else if (g_vfAnimDecisionBits.four) {
        return false;
    }
    return true;
}

bool ShouldDelayRyoFightAnimation() {
    const MotionInformationWork *const ryoMotion = FightScriptMotion(FindCharacter(nullptr, g_vfRyoCharaTaskIdx));
    const std::uint16_t current = ryoMotion ? ryoMotion->currentAnimId : 0;
    if (current == 1178) {
        const MotionInformationWork *const opponentMotion = FightScriptMotion(FindCharacter(nullptr, g_vfOppoCharaTaskIdx));
        return g_vfPendingRyoAnimID == 1177 && opponentMotion && MOTI_GetPlayRateScaledRemain(*opponentMotion) < 12.0f;
    }
    return current == 1213 && g_vfPendingRyoAnimID == 1214 && MOTI_GetPlayRateScaledRemain(*ryoMotion) < 24.0f;
}



void TurnFightCharacterToward(Character *character, Character *target) {
    if (!character || !target)
        return;
    const Vector3 &position = character->entity.node.position;
    const Vector3 &targetPosition = target->entity.node.position;
    const std::int32_t yaw = character->entity.node.rotation.y;
    const std::uint16_t desired = static_cast<std::uint16_t>(atan2_fixed16(targetPosition.x - position.x, targetPosition.z - position.z) + 0x8000);
    const std::uint16_t forward = static_cast<std::uint16_t>(desired - yaw);
    const std::uint16_t backward = static_cast<std::uint16_t>(yaw - desired);
    const std::int32_t delta = forward >= backward ? backward : (desired <= yaw ? -static_cast<std::int32_t>(forward) : yaw - desired);
    if ((delta < 0 ? -delta : delta) > 1820)
        character->entity.node.rotation.y = delta < 0 ? yaw + 1820 : yaw - 1820;
}



void SeparateFightCharacters() {
    Character *const ryo = FindCharacter(nullptr, g_vfRyoCharaTaskIdx);
    Character *const opponent = FindCharacter(nullptr, g_vfOppoCharaTaskIdx);
    if (!ryo || !opponent)
        return;
    const Vector3 &ryoPosition = ryo->entity.node.position;
    Vector3 &opponentPosition = opponent->entity.node.position;
    const float deltaX = opponentPosition.x - ryoPosition.x;
    const float deltaZ = opponentPosition.z - ryoPosition.z;
    const float distanceSquared = deltaZ * deltaZ + deltaX * deltaX;
    if (static_cast<double>(distanceSquared) >= 0.9)
        return;
    const std::uint16_t yaw = static_cast<std::uint16_t>(atan2_fixed16(deltaX, deltaZ) + 0x8000);
    const std::uint16_t folded = yaw > 0x8000U ? static_cast<std::uint16_t>(0x10000U - yaw) : yaw;
    const double correction = 0.9 - static_cast<double>(distanceSquared);
    opponentPosition.x += static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(yaw))) * correction);
    opponentPosition.z += static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(0x4000U - folded))) * correction);
}


bool ResolveFightCommandAnimations(ScenarioContext *context) {
    if (!g_vfLastResolvedRyoAnimID)
        g_vfPendingRyoAnimID = 850;
    if (!g_vfLastResolvedOppoAnimID && g_figpOppoRequestedAnim != 1179)
        g_vfPendingOppoAnimID = 850;
    Character *const ryo = FindCharacter(nullptr, g_vfRyoCharaTaskIdx);
    Character *const opponent = FindCharacter(nullptr, g_vfOppoCharaTaskIdx);
    bool requestedRyo = false;
    bool requestedOpponent = false;
    if (g_battleId >= 1) {
        if (g_vfAnimDecisionBits.one) {
            ++g_vfComboFrameCounter;
            if (g_vfAnimDecisionBits.one == 1 && g_vfComboFrameCounter >= g_vfComboWindowOpen && g_vfComboFrameCounter <= g_vfComboWindowClose &&
                g_vfLastResolvedRyoAnimID != 583 && CanResolveFightAnimation(ryo)) {
                g_vfAnimDecisionBits.one = 2;
                g_vfAnimDecisionBits.two = 2;
                g_vfAnimDecisionBits.three = 1;
                g_vfAnimDecisionBits.four = 1;
                g_vfNextAnimCountdown = g_vfCurrentRyoAnimID == 1177 ? 12 : (g_vfCurrentRyoAnimID == 1167 || g_vfCurrentRyoAnimID == 1178 ? 1 : 3);
            }
        }
        bool reacted = false;
        if (g_vfAnimDecisionBits.two) {
            ++g_vfReactFrameCounter;
            if (g_vfAnimDecisionBits.two == 1 && g_vfReactFrameCounter >= g_vfReactWindowOpen && g_vfReactFrameCounter <= g_vfReactWindowClose &&
                CanResolveFightAnimation(opponent)) {
                g_vfAnimDecisionBits.one = 2;
                g_vfAnimDecisionBits.two = 2;
                g_vfAnimDecisionBits.extra = 1;
                if (!g_figpRyoMatcher.resultCode) {
                    g_figpRyoMatcher.resultKind = 2;
                    g_figpRyoMatcher.resultCode = 19;
                }
                reacted = true;
            }
        } else if (g_battleStageId >= 3 && g_battleStageId <= 6 && g_vfBlendTimer) {
            if (!--g_vfBlendTimer && g_battleId == 2) {
                g_vfPendingOppoAnimID = 795;
                g_vfCurrentOppoAnimID = 795;
                g_vfReactFrameCounter = 0;
                g_vfAnimDecisionBits.two = 1;
                const std::uint8_t *const cue = shenLookupAnimationIndex(795, 2);
                if (cue) {
                    std::memcpy(&g_vfReactWindowOpen, cue + 2, sizeof(g_vfReactWindowOpen));
                    --g_vfReactWindowOpen;
                    std::memcpy(&g_vfReactWindowClose, cue + 4, sizeof(g_vfReactWindowClose));
                }
            }
        }
        // A reaction inside the opponent's window starts Ryo's damage countdown at 3, counted down on the same frame.
        if (reacted) {
            g_vfRyoDamageCountdown = 2;
        } else if (g_vfRyoDamageCountdown && !--g_vfRyoDamageCountdown) {
            const std::uint8_t *const cue = shenLookupAnimationIndex(static_cast<std::uint16_t>(g_vfCurrentOppoAnimID), 5);
            std::uint16_t damage = 845;
            if (cue)
                std::memcpy(&damage, cue + 2, sizeof(damage));
            g_vfPendingRyoAnimID = damage;
        }
        if (g_vfNextAnimCountdown && !--g_vfNextAnimCountdown) {
            const std::uint8_t *const cue = shenLookupAnimationIndex(static_cast<std::uint16_t>(g_vfCurrentRyoAnimID), 5);
            std::uint16_t response = g_vfCurrentRyoAnimID == 1178 ? 1180 : 845;
            if (cue)
                std::memcpy(&response, cue + 2, sizeof(response));
            g_vfPendingOppoAnimID = response;
        }
    }
    const bool commandEnded = (g_figpRyoMatcher.resultKind == 1 || g_figpOpponentMatcher.resultKind == 1) && g_vfAnimDecisionBits.three == 1;
    if (g_vfPendingRyoAnimID && (g_vfPendingRyoAnimID != g_vfLastResolvedRyoAnimID || g_vfPendingRyoDirty) && !ShouldDelayRyoFightAnimation()) {
        MotionInformationWork *const motion = FightScriptMotion(ryo);
        if (!commandEnded || g_vfForceEndFlag != 1) {
            if (commandEnded) {
                if (FightPoseWork *const pose = FIGP_GetWork(ryo)) {
                    pose->poseCacheColumns = 2;
                    pose->poseCacheRows = 0;
                    pose->active = 1;
                }
                g_vfBattleForceEndRequested = 1;
            }
            if (!motion || motion->currentAnimId != 850) {
                if (g_vfPendingRyoAnimID == 850)
                    RestoreFightAnimationRotation(ryo, false);
                RequestFightAnimation(ryo, g_vfPendingRyoAnimID, false);
            } else if (g_vfPendingRyoAnimID != 850) {
                RequestFightAnimation(ryo, g_vfPendingRyoAnimID, false);
            }
        }
        requestedRyo = true;
        g_vfPendingRyoDirty = 0;
        if (motion && g_vfPendingBlendSpeedA > 0.0f)
            motion->forcedFrameTime = g_vfPendingBlendSpeedA;
        if (motion && g_vfPendingBlendSpeedB > 0.0f)
            motion->scriptEndFrame = g_vfPendingBlendSpeedB;
        g_vfPendingBlendSpeedA = -1.0f;
        g_vfPendingBlendSpeedB = -1.0f;
        ApplyFightAnimationRotation(ryo, static_cast<std::uint16_t>(g_vfPendingRyoAnimID), false);
        if (g_vfPendingRyoAnimID == 188)
            motmDisableFlags(ryo, 1);
        g_vfLastResolvedRyoAnimID = g_vfPendingRyoAnimID;
    }
    if (g_vfPendingOppoAnimID && (g_vfPendingOppoAnimID != g_vfLastResolvedOppoAnimID || g_vfPendingOppoDirty)) {
        if (g_battleId) {
            if (g_vfPendingOppoAnimID == 850)
                RestoreFightAnimationRotation(opponent, true);
            RequestFightAnimation(opponent, g_vfPendingOppoAnimID, true);
            requestedOpponent = true;
            g_vfPendingOppoDirty = 0;
            ApplyFightAnimationRotation(opponent, static_cast<std::uint16_t>(g_vfPendingOppoAnimID), true);
        }
        g_vfLastResolvedOppoAnimID = g_vfPendingOppoAnimID;
    }
    if (g_battleId) {
        const MotionInformationWork *const ryoMotion = FightScriptMotion(ryo);
        if (ryoMotion && ryoMotion->currentAnimId == 850 && !requestedRyo)
            TurnFightCharacterToward(ryo, opponent);
        const MotionInformationWork *const opponentMotion = FightScriptMotion(opponent);
        if (opponentMotion && opponentMotion->currentAnimId == 850 && !requestedOpponent)
            TurnFightCharacterToward(opponent, ryo);
    }
    if (g_vfAnimDecisionBits.mirror && ryo &&
        (g_vfPendingRyoAnimID == 184 || g_vfPendingRyoAnimID == 226 || g_vfPendingRyoAnimID == 255 || g_vfPendingRyoAnimID == 259)) {
        Vector3i rotation = GetEulerAnglesForEntity(ryo->entity.node);
        rotation.y += 0x8000;
        ApplyEulerAnglesToNode(ryo->entity.node, rotation);
    }
    if (g_vfLastResolvedRyoAnimID == 188 && FightAnimationEnded(ryo))
        g_vfPendingRyoAnimID = 850;
    if (g_battleId >= 1 && g_vfCurrentRyoAnimID != 1167 && g_vfCurrentRyoAnimID - 1177U > 1U && g_vfAnimDecisionBits.one == 2 &&
        g_vfAnimDecisionBits.two == 2) {
        MotionInformationWork *const opponentMotion = FightScriptMotion(opponent);
        if (opponentMotion && opponentMotion->currentAnimId == 795 && MOTI_GetPlayRateScaledRemain(*opponentMotion) == 12.0f)
            opponent->entity.node.position = Vector3{65.953003f, 0.0f, 31.417999f};
        SeparateFightCharacters();
        shenPushCharaAwayFromOtherChara(
            context, shen_name{static_cast<std::uint32_t>(g_vfOppoCharaTaskIdx)}, shen_name{static_cast<std::uint32_t>(g_vfRyoCharaTaskIdx)});
        shenPushCharaAwayFromOtherChara(
            context, shen_name{static_cast<std::uint32_t>(g_vfRyoCharaTaskIdx)}, shen_name{static_cast<std::uint32_t>(g_vfOppoCharaTaskIdx)});
    }
    bool complete = false;
    if (commandEnded && g_vfForceEndFlag == 1) {
        g_vfLastResolvedRyoAnimID = 0;
        FightScriptSetPoseActive(0);
        complete = true;
    } else if (g_vfLastResolvedRyoAnimID && g_vfLastResolvedRyoAnimID != 850 && g_vfLastResolvedRyoAnimID != 188 && FightAnimationEnded(ryo)) {
        g_vfLastResolvedRyoAnimID = 0;
        FightScriptSetPoseActive(0);
    }
    if (g_vfLastResolvedOppoAnimID && g_vfLastResolvedOppoAnimID != 850 && g_vfLastResolvedOppoAnimID != 188 && FightAnimationEnded(opponent))
        g_vfLastResolvedOppoAnimID = 0;
    if (!g_vfLastResolvedRyoAnimID && !g_vfLastResolvedOppoAnimID)
        complete = false;
    if (g_figpOppoRequestedAnim == 1179 && FightAnimationEnded(opponent))
        complete = true;
    if (g_vfPendingRyoAnimID == g_vfLastResolvedRyoAnimID)
        g_vfPendingRyoAnimID = 0;
    if (g_vfPendingOppoAnimID == g_vfLastResolvedOppoAnimID)
        g_vfPendingOppoAnimID = 0;
    if ((g_vfLastResolvedRyoAnimID == 850 || g_vfLastResolvedRyoAnimID == 188) && (g_vfLastResolvedOppoAnimID == 850 || g_vfLastResolvedOppoAnimID == 188))
        complete = true;
    return (commandEnded && g_vfForceEndFlag == 1) || complete;
}



void SetupFightingStyleRuntime() {
    Character *const ryo = FindCharacter(nullptr, g_vfRyoCharaTaskIdx);
    Character *const opponent = FindCharacter(nullptr, g_vfOppoCharaTaskIdx);
    g_figpSetupPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const Vector3i rotation = ryo ? GetEulerAnglesForEntity(ryo->entity.node) : Vector3i{};
    g_figpSetupYaw = rotation.y;
    g_figpFacingAngle = g_battleStageId == 2 ?
        rotation.y :
        static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(static_cast<float>(rotation.y) / 182.04445f) + 180.0) * 8192.0 / 45.0));
    if (g_battleId == 2 && opponent)
        ApplyEulerAnglesToNode(opponent->entity.node, Vector3i{.x = rotation.x, .y = g_figpFacingAngle, .z = rotation.z});
    const std::uint16_t yaw = static_cast<std::uint16_t>(rotation.y);
    const std::uint16_t folded = yaw > 0x8000U ? static_cast<std::uint16_t>(0U - yaw) : yaw;
    const Vector3 position{
        .x = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(yaw))) * 1.4 + static_cast<double>(g_figpSetupPosition.x)),
        .y = g_figpSetupPosition.y,
        .z = static_cast<float>(
            static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(0x4000U - folded))) * 1.4 + static_cast<double>(g_figpSetupPosition.z))};
    if (g_battleId == 2 && opponent)
        shenApplyTranslationToAll(opponent->entity.node, position);
    motmEnableFlags(ryo, 1);
    if (g_battleId == 2)
        motmEnableFlags(opponent, 1);
    if (MotionInformationWork *const motion = FightScriptMotion(ryo)) {
        motion->motionFlags &= ~0x4000U;
        motion->activeBasePoseAnimIndex = 0;
        motion->headLookAngleBase = 0;
        motion->lookAtMode = 0;
        motion->currentTurnAngle = 0;
    }
    InitializeFightingStyleMatcher();
    g_shenFightSubstate = 9;
}

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

void VFMA_ReinitializeFightScriptState(ScenarioContext *, void *rawParams) {
    const VFFightScriptParameters *const params = static_cast<const VFFightScriptParameters *>(rawParams);
    if (!params)
        return;
    VFFightScriptResult *const result = ScenarioPointer<VFFightScriptResult>(params->resultOffset);
    if (!result)
        return;
    g_vfRyoCharaTaskIdx = params->ryoCharacterId;
    g_vfOppoCharaTaskIdx = params->opponentCharacterId;
    g_figpRyoBattleCommandId = params->ryoCommandId;
    g_figpOpponentBattleCommandId = params->opponentCommandId;
    g_figpScriptResult = result;
    g_shenFightState = 0;
    g_vfPendingRyoAnimID = 0;
    g_vfPendingOppoAnimID = 0;
    g_vfLastResolvedRyoAnimID = 0;
    g_vfLastResolvedOppoAnimID = 0;
    result->phase = 0;
    result->commandState = 0;
    result->resultCode = 0;
    result->inputIndex = 0xff;
    result->classificationMask = 0;
    g_suppressDefaultHudPromptIcons = 1;
    if (g_pControlPadController) {
        g_pControlPadController->blockedCanControl = 0;
        g_pControlPadController->state = 2;
    }
    g_tmexValHigh = 0;
}


void Script_fightingStyle(ScenarioContext *context, void *) {
    VFFightScriptResult *const result = g_figpScriptResult;
    if (!result)
        return;
    switch (g_shenFightState) {
    case 0:
        if (g_figpRyoBattleCommandId) {
            g_figpRyoMatcher.commandId = g_figpRyoBattleCommandId;
            g_figpOpponentMatcher.commandId = g_figpOpponentBattleCommandId;
            g_shenFightState = 3;
        } else {
            g_shenFightState = 2;
        }
        break;
    case 3: {
        const FightCommandProgram *const program = FightProgramById(g_figpRyoMatcher.commandId);
        g_shenFightState = 4;
        g_shenFightSubstate = 6;
        g_battleId = program->battleId;
        g_battleStageId = program->stageId;
        break;
    }
    case 4: {
        bool finished = false;
        if (g_shenFightSubstate == 6) {
            SetupFightingStyleRuntime();
        } else if (g_shenFightSubstate == 8) {
            InitializeFightingStyleMatcher();
            g_shenFightSubstate = 9;
        } else if (g_shenFightSubstate == 9) {
            g_figpMoveSelectResult = UpdateFightCommandMatchers();
            if (g_figpMoveSelectResult != -1)
                g_shenFightSubstate = 11;
        } else if (g_shenFightSubstate == 11 && g_figpResolveComplete) {
            FightScriptSetPoseActive(0);
            g_shenFightSubstate = 8;
            finished = true;
        }
        g_figpResolveComplete = ResolveFightCommandAnimations(context);
        if (finished)
            g_shenFightState = 5;
        break;
    }
    case 5: {
        g_vfBattleForceEndRequested = 1;
        FightScriptSetPoseActive(0);
        FightCommandMatcher &ryo = g_figpRyoMatcher;
        FightCommandMatcher &opponent = g_figpOpponentMatcher;
        result->phase = 2;
        result->inputIndex = ryo.programInputIndex;
        if ((ryo.resultKind == 1 || opponent.resultKind == 1) && g_vfAnimDecisionBits.three == 1) {
            result->commandState = 1;
            if (opponent.resultKind == 1) {
                if (opponent.classifyCode == 22) {
                    result->resultCode = 22;
                } else if (opponent.classifyCode == 23) {
                    result->resultCode = 23;
                    result->classificationReferenceCount = opponent.classifyReferenceCount;
                }
            }
        } else {
            result->commandState = 2;
            if (ryo.classifyCode != 0) {
                result->resultCode = ryo.classifyCode;
                result->classificationMask = ryo.classifyMask;
                result->classificationLength = ryo.classifyLength;
                result->classificationReferenceCount = ryo.classifyReferenceCount;
            } else {
                result->resultCode = ryo.resultCode;
            }
        }
        break;
    }
    default:
        break;
    }
    ++result->frameCounter;
    ++g_figpScriptTicks;
}

} // namespace shenmue::s1
