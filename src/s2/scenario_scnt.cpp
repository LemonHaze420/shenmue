#include "s2/scenario_scnt.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


float ScenarioLineClock() {
    return static_cast<float>(scenarioLineStartTicks - static_cast<std::int32_t>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta))) /
        12500000.0f;
}

std::int32_t ScenarioFrameTicks() {
    return static_cast<std::int32_t>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
}

Character *ResolveScenarioTalkCharacter(shen_name name) {
    if (!activeScenarioEventRuntime)
        return nullptr;
    if (name.name == SHEN_NAME(AKIR) || name.name == SHEN_NAME(RYO_)) {
        auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        return control ? static_cast<Character *>(control->ryo) : nullptr;
    }
    return FindScenarioCharacter(activeScenarioEventRuntime->task, name);
}

std::uint8_t ScntByte(std::span<const std::byte> bytes, std::size_t offset) {
    return std::to_integer<std::uint8_t>(bytes[offset]);
}

bool FreeTalkProgram(const ScenarioFreeTalkRecord &record, std::span<const std::byte> &program) {
    if (!record.sourceRecord || !record.script)
        return false;
    ScenarioFreeTalkSourceHeader header{};
    std::memcpy(&header, record.sourceRecord, sizeof(header));
    const std::ptrdiff_t scriptOffset = record.script - record.sourceRecord;
    if (header.size < sizeof(header) || scriptOffset < 0 || static_cast<std::size_t>(scriptOffset) >= header.size) {
        return false;
    }
    program = {record.script, header.size - static_cast<std::size_t>(scriptOffset)};
    return true;
}

bool ResolveScntGameDataOperand(std::int32_t index, std::int32_t &value) {
    DateTime dateTime{};
    if (index >= 0 && index <= 5) {
        GetCurrentDateTime(dateTime);
        switch (index) {
        case 0:
            value = dateTime.year;
            break;
        case 1:
            value = dateTime.month;
            break;
        case 2:
            value = dateTime.day;
            break;
        case 3:
            value = static_cast<std::int8_t>(dateTime.week);
            break;
        case 4:
            value = dateTime.hour;
            break;
        default:
            value = dateTime.minute;
            break;
        }
        return true;
    }
    if (index == 6) {
        value = GetCurrentCash();
        return true;
    }
    if (index == 10) {
        value = QueryNpcSceneCheck(GetActiveScenarioCharacter()) ? 1 : 0;
        return true;
    }
    if (index == 7) {
        value = static_cast<std::int32_t>(GetCurrentStep());
        return true;
    }
    if (index == 8 || index == 9) {
        // ResolveScenarioOperand resolves the ambient SCEN name, not the record whose F0
        // expression is currently being evaluated (including FRTK probes).
        const auto *const active = FindMutableScenarioFreeTalkRecord(GetActiveScenarioCharacter());
        value = 0;
        if (active) {
            const ScenarioFreeTalkControl *const state = active->control;
            value = static_cast<std::int8_t>(index == 8 ? state->completedCount : state->activationCount);
        }
        return true;
    }
    value = 0;
    return true;
}

bool AssignScntGameDataOperand(std::int32_t index, std::int32_t value) {
    DateTime dateTime{};
    if (index >= 0 && index <= 5) {
        GetCurrentDateTime(dateTime);
        switch (index) {
        case 0:
            dateTime.year = static_cast<std::int8_t>(value);
            break;
        case 1:
            dateTime.month = static_cast<std::int8_t>(value);
            break;
        case 2:
            dateTime.day = static_cast<std::int8_t>(value);
            break;
        case 3:
            dateTime.week = static_cast<std::uint8_t>(value);
            break;
        case 4:
            dateTime.hour = static_cast<std::int8_t>(value);
            break;
        default:
            dateTime.minute = static_cast<std::int8_t>(value);
            break;
        }
        SetCurrentDateTimeTo(dateTime);
        return true;
    }
    if (index == 6) {
        const auto difference = static_cast<std::int32_t>(static_cast<std::uint32_t>(value) - static_cast<std::uint32_t>(GetCurrentCash()));
        AddThisToCashOrSegaCoins(difference, ECashOrCoins::Cash);
        return true;
    }
    if (index == 10) {
        GetScenarioTalkPresentationState().followRequest = -1;
        SetScenarioFollowCharacter(value != 0 ? GetActiveScenarioCharacter() : shen_name{});
        return true;
    }
    if (index == 7) {
        SetSTEP(value);
        return true;
    }
    if (index == 8 || index == 9) {
        auto *const active = FindMutableScenarioFreeTalkRecord(GetActiveScenarioCharacter());
        if (active) {
            ScenarioFreeTalkControl &state = *active->control;
            auto &counter = index == 8 ? state.completedCount : state.activationCount;
            counter = static_cast<std::uint8_t>(std::clamp(value, 0, 100));
        }
        return true;
    }
    return true;
}

bool ResolveScenarioExpressionOperand(const ScenarioExpressionOperand &operand, std::int32_t &value) {
    switch (operand.mode) {
    case 1:
        value = operand.index;
        return true;
    case 2:
        value = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(operand.index)));
        return true;
    case 3:
        value = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Story, static_cast<std::int16_t>(operand.index)));
        return true;
    case 4:
        value = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(operand.index)));
        return true;
    case 5:
        return ResolveScntGameDataOperand(operand.index, value);
    case 6:
        return (value = QueryNpcRouteLiveState(shen_name{static_cast<std::uint32_t>(operand.index)})), true;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        return ResolveScenarioEventStateOperand(operand.mode, static_cast<std::uint16_t>(operand.index), value);
    case 15: {
        shen_name operandName{};
        if (!GetCurrentMapOperandName(static_cast<std::uint16_t>(operand.index), operandName)) {
            value = 0;
            return true;
        }
        if (operandName.name == g_currentMap.name || operandName.name == GetCurrentDoorAuthor().name ||
            std::any_of(scenarioEventObjectMapNames.begin(), scenarioEventObjectMapNames.end(),
                [operandName](shen_name name) { return name.name == operandName.name; })) {
            value = 1;
            return true;
        }
        value = 0;
        return true;
    }
    default:
        return false;
    }
}

bool AssignScenarioExpressionOperand(const ScenarioExpressionOperand &operand, std::int32_t value) {
    switch (operand.mode) {
    case 2:
        ClearOrSetShenmueFlags(operand.index, value);
        return true;
    case 3:
        WriteBit(reinterpret_cast<std::uint8_t *>(&scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, story), storyFlags.size(), operand.index,
            value != 0);
        return true;
    case 4:
        SetScenarioByteStat(operand.index, value);
        return true;
    case 5:
        return AssignScntGameDataOperand(operand.index, value);
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        return AssignScenarioEventStateOperand(operand.mode, static_cast<std::uint16_t>(operand.index), value);
    default:
        return true;
    }
}

bool DecodeScntExpression(
    std::span<const std::byte> program, std::size_t offset, std::size_t &nextOffset, ScenarioFreeTalkRecord &record, std::int32_t &result) {
    std::array<ScenarioExpressionOperand, 64> stack{};
    std::size_t stackSize = 0;
    std::size_t cursor = offset;
    auto push = [&](ScenarioExpressionOperand operand) {
        if (stackSize == stack.size())
            return false;
        stack[stackSize++] = operand;
        return true;
    };
    auto pop = [&](ScenarioExpressionOperand &operand) {
        if (stackSize == 0)
            return false;
        operand = stack[--stackSize];
        return true;
    };
    auto immediate = [](std::int32_t value) {
        ScenarioExpressionOperand operand{};
        operand.index = value;
        return operand;
    };

    while (cursor < program.size()) {
        const std::uint8_t opcode = ScntByte(program, cursor++);
        if (opcode <= 0x6f) {
            if (cursor >= program.size())
                return false;
            const std::int32_t index = ((opcode & 0x0f) << 8) | ScntByte(program, cursor++);
            if (opcode <= 0x4f) {
                ScenarioExpressionOperand operand{};
                operand.index = index;
                operand.mode = opcode <= 0x1f ? 2 : opcode <= 0x3f ? 3 : 4;
                if (!push(operand))
                    return false;
            } else if (!push(immediate(opcode <= 0x5f ? -index : index))) {
                return false;
            }
            continue;
        }
        if (opcode <= 0x77) {
            if (program.size() - cursor < 2)
                return false;
            const std::int32_t value = ((opcode & 7) << 16) | (ScntByte(program, cursor) << 8) | ScntByte(program, cursor + 1);
            cursor += 2;
            if (!push(immediate(value)))
                return false;
            continue;
        }
        if (opcode <= 0x7f) {
            if (program.size() - cursor < 3)
                return false;
            std::int32_t value =
                ((opcode & 3) << 24) | (ScntByte(program, cursor) << 16) | (ScntByte(program, cursor + 1) << 8) | ScntByte(program, cursor + 2);
            cursor += 3;
            if (opcode >= 0x7c)
                value = -value;
            if (!push(immediate(value)))
                return false;
            continue;
        }
        if (opcode == 0x80) {
            ScenarioExpressionOperand operand{};
            // ResolveScenarioExpressionOperand returns the top operand; preceding expression values
            // need not have been reduced to a single stack entry.
            if (!pop(operand) || !ResolveScenarioExpressionOperand(operand, result)) {
                return false;
            }
            nextOffset = cursor;
            scntExpressionResult = result;
            return true;
        }
        if (opcode >= 0x81 && opcode <= 0x83) {
            ScenarioExpressionOperand target{};
            ScenarioExpressionOperand valueOperand{};
            std::int32_t value{};
            if (opcode == 0x81) {
                if (!pop(valueOperand) || !pop(target) || !ResolveScenarioExpressionOperand(valueOperand, value) ||
                    !AssignScenarioExpressionOperand(target, value) || !push(target)) {
                    return false;
                }
            } else {
                if (!pop(target) || !ResolveScenarioExpressionOperand(target, value)) {
                    return false;
                }
                value = static_cast<std::int32_t>(static_cast<std::uint32_t>(value) + (opcode == 0x82 ? 1U : 0xffffffffU));
                if (!AssignScenarioExpressionOperand(target, value) || !push(target)) {
                    return false;
                }
            }
            continue;
        }
        if (opcode == 0x84) {
            ScenarioExpressionOperand operand{};
            std::int32_t value{};
            if (!pop(operand) || !ResolveScenarioExpressionOperand(operand, value) || !push(immediate(value == 0 ? 1 : 0))) {
                return false;
            }
            continue;
        }
        if (opcode >= 0x85 && opcode <= 0x91) {
            ScenarioExpressionOperand rightOperand{};
            ScenarioExpressionOperand leftOperand{};
            std::int32_t left{};
            std::int32_t right{};
            if (!pop(rightOperand) || !pop(leftOperand) || !ResolveScenarioExpressionOperand(leftOperand, left)) {
                return false;
            }

            const bool shortCircuit = (opcode == 0x85 && left == 0) || (opcode == 0x86 && left != 0);
            if (!shortCircuit && !ResolveScenarioExpressionOperand(rightOperand, right)) {
                return false;
            }
            std::int32_t value{};
            switch (opcode) {
            case 0x85:
                value = left && right ? 1 : 0;
                break;
            case 0x86:
                value = left || right ? 1 : 0;
                break;
            case 0x87:
                value = left == right ? 1 : 0;
                break;
            case 0x88:
                value = left != right ? 1 : 0;
                break;
            case 0x89:
                value = left < right ? 1 : 0;
                break;
            case 0x8a:
                value = left <= right ? 1 : 0;
                break;
            case 0x8b:
                value = left > right ? 1 : 0;
                break;
            case 0x8c:
                value = left >= right ? 1 : 0;
                break;
            case 0x8d:
                value = static_cast<std::int32_t>(static_cast<std::uint32_t>(left) + static_cast<std::uint32_t>(right));
                break;
            case 0x8e:
                value = static_cast<std::int32_t>(static_cast<std::uint32_t>(left) - static_cast<std::uint32_t>(right));
                break;
            case 0x8f:
                value = static_cast<std::int32_t>(static_cast<std::uint32_t>(left) * static_cast<std::uint32_t>(right));
                break;
            case 0x90:
            case 0x91:
                if (right == 0)
                    return false;
                value = opcode == 0x90 ? static_cast<std::int32_t>(static_cast<std::int64_t>(left) / right) :
                                         static_cast<std::int32_t>(static_cast<std::int64_t>(left) % right);
                break;
            }
            if (!push(immediate(value)))
                return false;
            continue;
        }
        if (opcode <= 0x9f)
            continue;
        if (cursor >= program.size())
            return false;
        const std::int32_t encoded = ScntByte(program, cursor++) + ((opcode & 3) << 8);
        ScenarioExpressionOperand operand{};
        if (opcode < 0xc0) {
            operand.mode = 5;
            operand.index = ScntByte(program, cursor - 1);
        } else if (opcode < 0xe0) {
            operand.index = encoded == 0 ? record.characterStateIndex : encoded;
            switch (opcode & 0x1c) {
            case 0x04:
                operand.mode = 7;
                break;
            case 0x08:
                operand.mode = 8;
                break;
            case 0x0c:
                operand.mode = 9;
                break;
            case 0x10:
                operand.mode = 10;
                break;
            case 0x14:
                operand.mode = 11;
                break;
            case 0x18:
                operand.mode = 12;
                break;
            case 0x1c:
                operand.mode = 13;
                break;
            default:
                operand.mode = 14;
                break;
            }
        } else if (opcode < 0xe4) {
            operand.mode = 15;
            operand.index = encoded;
        } else if (opcode < 0xe8) {
            operand.mode = 6;
            operand.index = encoded == 0 ? static_cast<std::int32_t>(record.name.name) : 0;
        } else {
            operand.index = encoded;
        }
        if (!push(operand))
            return false;
    }
    return false;
}

std::int32_t ScntSignedOffset(std::uint32_t value, std::uint32_t signBit) {
    return static_cast<std::int32_t>((value ^ signBit) - signBit);
}

bool ScntBranch(std::size_t &cursor, std::int32_t displacement, std::size_t size) {
    const std::int64_t destination = static_cast<std::int64_t>(cursor) + displacement;
    if (destination < 0 || static_cast<std::uint64_t>(destination) >= size)
        return false;
    cursor = static_cast<std::size_t>(destination);
    return true;
}



const std::byte *FindScenarioConversationRoot(ScenarioFreeTalkRecord &record, std::int32_t step) {
    std::span<const std::byte> program{};
    if (!FreeTalkProgram(record, program))
        return nullptr;
    struct StepLease {
        std::int32_t previous;
        ~StepLease() {
            SetSTEP(previous);
        }
    } lease{static_cast<std::int32_t>(GetCurrentStep())};
    SetSTEP(step);
    const bool hasDefault = record.defaultRoot != nullptr;
    std::size_t cursor = hasDefault ? static_cast<std::size_t>(record.defaultRoot - program.data()) : 0;
    const std::byte *result{};
    std::size_t returnOffset{};
    bool hasReturn{};
    std::int32_t condition = scntExpressionResult;
    ScntBranchHistory branches{};
    while (cursor < program.size()) {
        const std::size_t instruction = cursor;
        const std::uint8_t opcode = ScntByte(program, cursor++);
        if (opcode >= 0xe0) {
            switch (opcode) {
            case 0xe0:
            case 0xf5:
                if (program.size() - cursor < 3)
                    return nullptr;
                cursor += 3;
                break;
            case 0xe1:
            case 0xe2:
            case 0xe5:
                return hasDefault ? program.data() + instruction : result;
            case 0xe6:
                if (!hasDefault)
                    return result;
                break;
            case 0xf0:
                if (!DecodeScntExpression(program, cursor, cursor, record, condition))
                    return nullptr;
                break;
            case 0xf1:
            case 0xf3:
            case 0xf4:
            case 0xfa:
            case 0xfb:
            case 0xfc:
            case 0xfd:
                break;
            case 0xf2:
                if (program.size() - cursor < 3)
                    return nullptr;
                cursor += 3;
                if (!hasDefault)
                    return result;
                break;
            case 0xf9:
                if (hasReturn)
                    cursor = returnOffset;
                break;
            default:
                return nullptr;
            }
        } else if (opcode >= 0x80) {
            const std::uint8_t kind = opcode & 0xf0;
            if (kind == 0x80 || kind == 0xd0) {
                if (program.size() - cursor < 2)
                    return nullptr;
                const std::uint32_t encoded = ((opcode & 0x0fU) << 16) | (ScntByte(program, cursor) << 8) | ScntByte(program, cursor + 1);
                cursor += 2;
                if (kind == 0x80) {
                    returnOffset = cursor;
                    hasReturn = true;
                }
                if (!ScntBranch(cursor, ScntSignedOffset(encoded, 0x80000), program.size()) || (kind == 0xd0 && branches.Visit(cursor)))
                    return nullptr;
            } else if (kind == 0x90) {
                if (cursor == program.size())
                    return nullptr;
                ++cursor;
                result = program.data() + instruction;
                if (!hasDefault)
                    return result;
            } else if (kind == 0xa0) {
                if (program.size() - cursor < 2)
                    return nullptr;
                cursor += 2;
            } else if (kind == 0xb0) {
                if (opcode == 0xb0) {
                    if (program.size() - cursor < 4)
                        return nullptr;
                    cursor += 4;
                } else if (opcode == 0xb1) {
                    if (cursor == program.size())
                        return nullptr;
                    record.field007 = ScntByte(program, cursor++);
                } else if (opcode == 0xb2 && result) {
                    record.control->flags |= 0x20U;
                }
            } else if (kind == 0xc0) {
                if (cursor == program.size())
                    return nullptr;
                ++cursor;
            }
        } else {
            if (cursor == program.size())
                return nullptr;
            const std::uint32_t encoded = ((opcode & 0x0fU) << 8) | ScntByte(program, cursor++);
            const std::uint8_t kind = opcode & 0xf0;
            if (kind == 0x10 || kind == 0x20)
                result = program.data() + cursor;
            else if ((kind == 0x40 && condition == 0) || kind == 0x50) {
                if (!ScntBranch(cursor, ScntSignedOffset(encoded, 0x800), program.size()) || (kind == 0x50 && branches.Visit(cursor)))
                    return nullptr;
            }
        }
    }
    return result;
}


void SolveRyoMatrix(shen_name origin, shen_name other, std::uint16_t yaw, Vector3 *result) {
    Character *const originCharacter = ResolveScenarioTalkCharacter(origin);
    if (!originCharacter)
        return;
    Character *const otherCharacter = ResolveScenarioTalkCharacter(other);
    if (!otherCharacter)
        return;
    const Vector3 originPosition = originCharacter->entity.node.position;
    const Vector3 position = otherCharacter->entity.node.position;
    Matrix4x4 frame{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(originPosition.x, originPosition.y, originPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::GetTransformMatrix(&frame);
    Matrix4x4::InvertInPlace(&frame);
    Matrix4x4::Pop(1);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::SetActiveTransform(&frame);
    Matrix4x4::TransformPoint(position, *result);
    Matrix4x4::Pop(1);
}
void EndScenarioPlayerPresentation(std::int32_t mode) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    ScenarioTalkPresentationState &presentation = GetScenarioTalkPresentationState();
    if (scenario.flags & 0x200U) {
        presentation.playerFacingIssued = 0;
        scenario.flags &= ~0x200U;
        plymEndSpecialMode(mode);
    }
    if (!mode && presentation.participantCount) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const player = static_cast<Character *>(control->ryo);
        if (player) {
            const Vector3 position{};
            SetEyeToChara(player, &position, 16, 0);
            SetFaceFlagsForChara_0(player, FacialNodeOriginal);
            SetHighModelAndFaceFlagsForChara(player, Normal, 10, 1);
            SetFaceFlagsForChara(player, 1);
            AssignFacialClip(player, SCEN_DEFAULT_FACE_SEQUENCE.data());
            SetCharacterHandRegionEnabled(player, 0, false, -1);
            SetCharacterHandRegionEnabled(player, 1, false, -1);
        }
        presentation.participantCount = 0;
    }
}
void ConsumeScenarioCameraRestoreRequest() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (!(scenario.flags & 0x400U))
        return;
    if (GetParentMenu() == static_cast<std::uint8_t>(UIElement::GameBackground) && !IsItemSelectable() && !IsAnyItemSelectActive() && !IsWristwatchOpenable())
        RetireConversationCamera();
    scenario.flags &= ~0x400U;
}


bool ryoCheckBusyFlags() {
    const std::uint32_t flags = activeScenarioEventRuntime->flags;
    return (!plymIsSearchBlocked() || (flags & 0x40U)) && (GetParentMenu() == static_cast<std::uint8_t>(UIElement::GameBackground) || (flags & 0x40U)) &&
        !IsItemSelectable() && !IsAnyItemSelectActive() && (!IsWristwatchOpenable() || (flags & 0x40U)) && !srchHasActiveLockonTarget();
}


void scenClearSpeakerPresentation() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (scenario.flags & 0x1000U) {
        scenDestroySubtitles(scenario.task);
        scenario.flags &= ~0x1000U;
    }
    if (!scenario.faceResetCharacter.name)
        return;
    if (Character *const character = ResolveScenarioTalkCharacter(scenario.faceResetCharacter))
        AssignFacialClip(character, SCEN_DEFAULT_FACE_SEQUENCE.data());
    scenario.faceResetCharacter = {};
}


void SetScenarioActivationCount(ScenarioFreeTalkRecord &record, std::int32_t count) {
    record.control->activationCount = static_cast<std::uint8_t>(count < 0 ? 0 : count > 100 ? 100 : count);
}

void SetScenarioCompletionCount(ScenarioFreeTalkRecord &record, std::int32_t count) {
    record.control->completedCount = static_cast<std::uint8_t>(count < 0 ? 0 : count > 100 ? 100 : count);
}


void CaptureScenarioTalkInput() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (scenario.flags & 2U)
        return;
    const std::uint32_t name = scenario.activeCharacter.name;
    if ((name & 0xffffU) != 0x5953U || (name & 0xff000000U) == 0x5f000000U) {
        scenario.flags |= 2U;
        scenario.inputState = BindPadControlEntry(scenario.padControlData);
    }
}

void ReleaseScenarioTalkInput() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (!(scenario.flags & 2U))
        return;
    scenario.flags &= ~2U;
    BindPadControlEntry(scenario.inputState);
}


void BuildScenarioLipSequence(const ScenarioFreeTalkRecord &record) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const auto *const line = reinterpret_cast<const std::int32_t *>(record.firstDialogueLine);
    float duration{};
    std::memcpy(&duration, line + 3, sizeof(duration));
    std::int32_t steps{};
    if (duration == 0.0f) {
        const char *const text = reinterpret_cast<const char *>(record.sourceRecord) + line[0];
        std::int64_t length = -1;
        do
            ++length;
        while (text[length]);
        steps = static_cast<std::int32_t>(length) / 2 + 1;
    } else {
        steps = static_cast<std::int32_t>((duration * 60.0f) * 0.125f);
    }
    if (steps > 37)
        steps = 37;
    scenario.faceSequence[0] = 0;
    scenario.faceSequence[1] = 10;
    std::size_t cursor = 2;
    for (std::int64_t index = 2; index < 2LL * steps; index += 2) {
        const float random = IncrementSharedPrngFloat();
        scenario.faceSequence[cursor + 1] = 8;
        scenario.faceSequence[cursor] = static_cast<std::int16_t>(random * 6.0f);
        cursor += 2;
    }
    scenario.faceSequence[cursor] = 0;
    scenario.faceSequence[cursor + 1] = 10;
    scenario.faceSequence[cursor + 2] = -1;
}



bool IsScenarioFollowPromptAvailable() {
    return !EV_GetShenmueFlag(0x305) && !EV_GetShenmueFlag(0x342) && !EV_GetShenmueFlag(0x375) && !EV_GetShenmueFlag(0x377) && !EV_GetShenmueFlag(0x115) &&
        EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1348) && static_cast<std::int32_t>(GetCurrentStep()) >= 40000 &&
        static_cast<std::int32_t>(GetCurrentStep()) < 370000;
}


bool IsScenarioActiveRecordIdle() {
    const ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(activeScenarioEventRuntime->activeCharacter);
    if (!record)
        return true;
    if (static_cast<std::uint8_t>(record->executionState - 1) > 2U)
        return false;
    return (record->flags & (ScenarioTalkWaitStart | ScenarioTalkWaitFollow | ScenarioTalkWaitEnd)) == 0;
}

const shen_name *ScenarioParticipants(const ScenarioFreeTalkRecord &record) {
    return reinterpret_cast<const shen_name *>(record.participants);
}



shen_name ResolveScenarioParticipant(shen_name participant, const ScenarioFreeTalkRecord &record) {
    shen_name name = participant;
    if (name.name == SHEN_NAME(EXTR))
        name = record.name;
    if (static_cast<std::int16_t>(record.characterStateIndex) > 0 || ScenarioCharacterIsRyo(name))
        return name;
    return record.name;
}


bool AreScenarioParticipantsPresent(const ScenarioFreeTalkRecord &record) {
    if (!record.participants || !(record.control->flags & 0x20U))
        return true;
    for (std::int32_t index = 0; index < static_cast<std::int8_t>(record.participantCount); ++index) {
        const shen_name name = ResolveScenarioParticipant(ScenarioParticipants(record)[index], record);
        if (!name.name || ScenarioCharacterIsRyo(name))
            continue;
        bool admitted = false;
        for (const shen_name admittedName : scenarioCharacterAdmissionNames) {
            if (!admittedName.name)
                break;
            if (admittedName.name == name.name) {
                admitted = true;
                break;
            }
        }
        if (!admitted)
            return false;
    }
    return true;
}


std::int32_t SCEN_TrackVisitedBranchTarget(const std::byte *target) {
    std::int32_t count = scenarioBranchCount;
    for (std::int32_t index = 0; index < scenarioBranchCount; ++index) {
        if (scenarioBranchTargets[static_cast<std::size_t>(index)] == target) {
            scenarioBranchLoop = 1;
            return 1;
        }
    }
    if (scenarioBranchCount >= 100) {
        for (std::int32_t index = 0; index < scenarioBranchCount - 1; ++index)
            scenarioBranchTargets[static_cast<std::size_t>(index)] = scenarioBranchTargets[static_cast<std::size_t>(index) + 1];
        --count;
    }
    scenarioBranchCount = count + 1;
    scenarioBranchTargets[static_cast<std::size_t>(count)] = target;
    return scenarioBranchLoop;
}

std::int32_t ScntBe12(const std::byte *cursor, std::uint8_t opcode) {
    const std::int32_t value = static_cast<std::uint8_t>(cursor[0]) | ((opcode & 0x0f) << 8);
    return (value & 0x800) ? value | static_cast<std::int32_t>(0xfffff000U) : value;
}

std::int32_t ScntBe20(const std::byte *cursor, std::uint8_t opcode) {
    const std::int32_t value = static_cast<std::uint8_t>(cursor[1]) + ((((opcode & 0x0f) << 8) + static_cast<std::uint8_t>(cursor[0])) << 8);
    return (value & 0x80000) ? value | static_cast<std::int32_t>(0xfff00000U) : value;
}


const std::byte *Interpreter(const std::byte *cursor, ScenarioFreeTalkRecord &record) {
    std::span<const std::byte> program{};
    std::size_t next{};
    std::int32_t result{};
    if (!FreeTalkProgram(record, program) || cursor < program.data() || cursor > program.data() + program.size() ||
        !DecodeScntExpression(program, static_cast<std::size_t>(cursor - program.data()), next, record, result))
        return nullptr;
    return program.data() + next;
}



const std::byte *SCEN_AdvanceCharacterCommandStream(ScenarioFreeTalkRecord &record) {
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    const std::byte *cursor = record.script;
    std::int32_t queued = 0;
    scenarioBranchCount = 0;
    scenarioBranchLoop = 0;
    talk.queuedFaceLookCount = 0;
    for (;;) {
        const std::uint8_t opcode = static_cast<std::uint8_t>(*cursor++);
        if (opcode < 0x80) {
            const std::int32_t operand = ScntBe12(cursor, opcode);
            ++cursor;
            switch (opcode & 0xf0) {
            case 0x00:
                continue;
            case 0x10:
                return cursor;
            case 0x40:
                if (scntExpressionResult == 0)
                    cursor += operand;
                continue;
            case 0x50:
                cursor += operand;
                if (SCEN_TrackVisitedBranchTarget(cursor))
                    return nullptr;
                continue;
            default:
                return nullptr;
            }
        }
        if (opcode >= 0xe0) {
            if (opcode == 0xf0) {
                cursor = Interpreter(cursor, record);
                if (!cursor)
                    return nullptr;
                queued = talk.queuedFaceLookCount;
                continue;
            }
            if (opcode == 0xf1)
                continue;
            return nullptr;
        }
        switch (opcode & 0xf0) {
        case 0xa0:
            talk.queuedFaceLooks[static_cast<std::size_t>(queued++)] = (static_cast<std::uint32_t>(opcode) << 16) |
                (static_cast<std::uint32_t>(static_cast<std::uint8_t>(cursor[0])) << 8) | static_cast<std::uint8_t>(cursor[1]);
            cursor += 2;
            talk.queuedFaceLookCount = queued;
            continue;
        case 0xb0:
            if (opcode == 0xb1) {
                record.field007 = static_cast<std::uint8_t>(*cursor++);
                continue;
            }
            if (opcode == 0xb2) {
                record.control->flags |= 0x20U;
                continue;
            }
            return nullptr;
        case 0xd0:
            cursor += ScntBe20(cursor, opcode) + 2;
            if (SCEN_TrackVisitedBranchTarget(cursor))
                return nullptr;
            continue;
        default:
            return nullptr;
        }
    }
}



const std::byte *scenBuildDialogueCues(ScenarioFreeTalkRecord &record, const std::byte *cursor) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    const shen_name *const participants = ScenarioParticipants(record);
    std::uint8_t cameraFlags = 0;
    std::uint8_t pendingFlags = 0;
    std::int32_t speakerLetter = 0;
    std::int32_t approachLetter = 0;
    shen_name speaker{}, previousSpeaker{}, listener{};
    float time = 0.0f;
    float unattachedTime = 0.0f;
    std::int8_t pitch = static_cast<std::int8_t>(record.field007);
    if (participants) {
        if (const std::uint8_t letter = scenario.dialogueSpeaker)
            speaker = ResolveScenarioParticipant(participants[letter - 'A'], record);
        if (const std::uint8_t letter = scenario.dialogueListener)
            listener = ResolveScenarioParticipant(participants[letter - 'A'], record);
    }
    std::size_t cueIndex = 0;
    std::size_t packet = 0;
    scenario.flags &= ~4U;
    scenario.dialogueCursor = scenarioDialogueCues.data();
    scenarioBranchCount = 0;
    scenarioBranchLoop = 0;
    const auto addCue = [&](std::uint32_t code) -> SCENDialogueCue & {
        SCENDialogueCue &cue = scenarioDialogueCues[cueIndex++];
        cue.triggerTime = time;
        cue.payload = nullptr;
        cue.speaker = shen_name{code};
        return cue;
    };

    std::ptrdiff_t rewind = 1;
    for (;;) {
        const std::uint8_t opcode = static_cast<std::uint8_t>(*cursor++);
        switch (opcode & 0xf0) {
        case 0x00:
            ++cursor;
            continue;
        case 0x20: {
            const std::int32_t lineIndex = static_cast<std::uint8_t>(*cursor++) | ((opcode & 0x0f) << 8);
            const std::byte *const line = record.lineTable + 16 * lineIndex;
            if (!record.firstDialogueLine)
                record.firstDialogueLine = line;
            bool resolved = false;
            if (!record.selectedApproachPoint || approachLetter) {
                if (speaker.name) {
                    speakerLetter = 0;
                    resolved = true;
                } else {
                    speakerLetter = static_cast<std::uint8_t>(line[8]);
                    if (participants) {
                        speaker = ResolveScenarioParticipant(participants[speakerLetter - 'A'], record);
                        resolved = speaker.name != 0;
                    }
                }
            } else {
                speaker = hiddenFreeTalkCharacterName;
                approachLetter = 1;
                listener = scenGetCharaName(scenario.task, ScenarioRyo());
                if (scenario.hiddenFreeTalkCharacter)
                    shenApplyTranslationToAll(&scenario.hiddenFreeTalkCharacter->entity.node, *record.selectedApproachPoint);
                resolved = true;
            }
            if (!resolved)
                speaker = speakerLetter == 'A' ? scenGetCharaName(scenario.task, ScenarioRyo()) : record.name;
            float lineDuration{};
            std::memcpy(&lineDuration, line + 12, sizeof(lineDuration));
            std::int32_t textOffset{};
            std::memcpy(&textOffset, line, sizeof(textOffset));
            float duration{};
            if (lineDuration == 0.0f) {
                const char *const text = reinterpret_cast<const char *>(record.sourceRecord) + textOffset;
                std::uint64_t length = 0;
                while (text[length])
                    ++length;
                duration = ((static_cast<float>(length) * 4.0f) * 0.5f + 15.0f) / 30.0f;
            } else {
                duration = lineDuration + 0.5f;
            }
            if (pitch && !ScenarioCharacterIsRyo(speaker))
                duration *= fast_powf_approx(2.0f, static_cast<float>(pitch) / -12.0f);
            std::uint8_t lineFlags = pendingFlags;
            CameraDialogueRecord &current = scenarioCameraRecords[packet];
            current.flags = pendingFlags;
            current.duration = duration;
            current.header = 260;
            current.type = 1;
            SCENDialogueCue &cue = scenarioDialogueCues[cueIndex];
            cue.triggerTime = time;
            cue.speaker = shen_name{SHEN_NAME(null)};
            cue.reserved14 = SHEN_NAME(null);
            cue.payload = reinterpret_cast<const std::int32_t *>(line);
            bool merge = speaker.name == previousSpeaker.name;
            if (merge) {
                cue.speaker = speaker;
            } else {
                shen_name addressed = listener;
                if (!addressed.name) {
                    if (previousSpeaker.name) {
                        addressed = previousSpeaker;
                    } else {
                        addressed = record.name;
                        if (speaker.name == record.name) {
                            addressed = scenGetCharaName(scenario.task, ScenarioRyo());
                        } else if (!ResolveScenarioTalkCharacter(addressed) && participants) {
                            for (std::int32_t index = 0; index < static_cast<std::int8_t>(record.participantCount); ++index) {
                                const shen_name candidate = participants[index];
                                if (candidate.name && ResolveScenarioParticipant(candidate, record).name != speaker.name) {
                                    addressed = ResolveScenarioParticipant(candidate, record);
                                    break;
                                }
                            }
                        }
                    }
                }
                current.speaker = ResolveScenarioTalkCharacter(speaker);
                current.listener = ResolveScenarioTalkCharacter(addressed);
                lineFlags = pendingFlags;
                if (!current.speaker || !current.listener) {
                    if (packet == 0) {
                        unattachedTime += duration;
                        goto nextLine;
                    }
                } else {
                    if (packet == 0) {
                        current.flags |= pendingFlags;
                        current.duration = unattachedTime + current.duration;
                    }
                    if (packet < 23) {
                        ++packet;
                        cue.speaker = speaker;
                        cue.reserved14 = addressed.name;
                        goto nextLine;
                    }
                }
                merge = true;
            }

            if (merge && packet) {
                scenarioCameraRecords[packet - 1].flags |= lineFlags;
                scenarioCameraRecords[packet - 1].duration += duration;
            }
        nextLine:
            speakerLetter = approachLetter;
            time += duration;
            ++cueIndex;
            cameraFlags = 0;
            listener = {};
            previousSpeaker = speaker;
            pendingFlags = 0;
            speaker = {};
            continue;
        }
        case 0x30: {
            const std::uint32_t code = static_cast<std::uint8_t>(*cursor++) | (static_cast<std::uint32_t>(opcode) << 8);
            addCue(code);
            const std::uint32_t action = code & 0xfffU;
            if (action - 20U <= 9U || action - 217U <= 0x1dU || action - 120U <= 9U) {
                cameraFlags |= 2U;
                pendingFlags = cameraFlags;
                scenario.flags |= 0x100U;
                const std::int32_t value = static_cast<std::int32_t>(action);
                const std::int32_t location = action > 0x1dU ? (action < 0xd9U ? value - 120 : value - 207) : value - 20;
                camCtrlSetConversationLookTarget(location < 0 || location >= static_cast<std::int8_t>(record.locationCount) ?
                        &scenarioTalkDefaultLocation :
                        reinterpret_cast<const Vector3 *>(record.locations) + location);
            } else if (action == 10U) {
                cameraFlags |= 8U;
                pendingFlags = cameraFlags;
            } else if (action == 207U) {
                talk.playerFacingMode = 1;
            }
            if (packet != 0) {
                time += 0.033333335f;
                scenarioCameraRecords[packet - 1].duration += 0.033333335f;
            }
            continue;
        }
        case 0x40: {
            const std::int32_t offset = ScntBe12(cursor, opcode);
            ++cursor;
            if (!scntExpressionResult)
                cursor += offset;
            continue;
        }
        case 0x50:
            cursor += ScntBe12(cursor, opcode) + 1;
            if (!SCEN_TrackVisitedBranchTarget(cursor))
                continue;
            rewind = 0;
            break;
        case 0x60:
        case 0x70:
            addCue(static_cast<std::uint8_t>(*cursor++) | (static_cast<std::uint32_t>(opcode) << 8));
            continue;
        case 0x80: {
            const std::int32_t offset = ScntBe20(cursor, opcode);
            record.returnScriptCursor = cursor + 2;
            cursor += 2 + offset;
            continue;
        }
        case 0x90:
            ++cursor;
            if (!(record.flags & (ScenarioTalkInterrupted | ScenarioTalkSavedScript)) && !record.requestedRoot) {
                record.control->resumeOffsets[TalkResumeMark] = static_cast<std::uint16_t>(cursor - record.script);
                record.flags |= ScenarioTalkResumeReached;
                record.pendingScriptCursor = nullptr;
            }
            continue;
        case 0xa0:
            addCue((static_cast<std::uint32_t>(opcode) << 16) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(cursor[0])) << 8) |
                static_cast<std::uint8_t>(cursor[1]));
            cursor += 2;
            continue;
        case 0xb0:
            if ((opcode & 0x0f) == 1) {
                const std::int8_t value = static_cast<std::int8_t>(*cursor++);
                addCue(opcode).reserved14 = static_cast<std::uint32_t>(static_cast<std::int32_t>(value));
                pitch = value;
                speakerLetter = approachLetter;
            } else if ((opcode & 0x0f) == 2) {
                addCue(opcode).reserved14 = 1;
            } else if ((opcode & 0x0f) == 0) {
                std::uint32_t value{};
                std::memcpy(&value, cursor, sizeof(value));
                cursor += 4;
                addCue(opcode).reserved14 = value;
            }
            continue;
        case 0xc0:
            switch (opcode & 0x0f) {
            case 1:
                scenario.dialogueSpeaker = static_cast<std::uint8_t>(*cursor++);
                continue;
            case 2:
                scenario.dialogueListener = static_cast<std::uint8_t>(*cursor++);
                continue;
            case 3:
            case 4:
            case 5:
            case 6:
                reinterpret_cast<std::uint8_t *>(&record.reserved008)[(opcode & 0x0f) - 3] = static_cast<std::uint8_t>(*cursor++);
                continue;
            case 7:
                record.selectionIndex = static_cast<std::int8_t>(*cursor++);
                continue;
            default:
                ++cursor;
                continue;
            }
        case 0xd0:
            cursor += ScntBe20(cursor, opcode) + 2;
            if (!SCEN_TrackVisitedBranchTarget(cursor))
                continue;
            rewind = 0;
            break;
        case 0xe0: {
            const std::uint8_t count = static_cast<std::uint8_t>(*cursor++);
            if (opcode != 0xe0) {
                rewind = 2;
                break;
            }
            const float random = IncrementSharedPrngFloat();
            const std::int32_t slot = static_cast<std::int32_t>(scenario.randomSelectionCount);
            std::int32_t skip = static_cast<std::int32_t>(random * static_cast<float>(count));
            if (slot < 3) {
                if (scenario.randomSelections[static_cast<std::size_t>(slot)] == skip)
                    skip = skip ? skip - 1 : count - 1;
                scenario.randomSelections[static_cast<std::size_t>(slot)] = skip;
                ++scenario.randomSelectionCount;
            }
            for (; skip; --skip)
                cursor += (static_cast<std::uint8_t>(cursor[1]) | ((static_cast<std::uint8_t>(cursor[0]) & 0x0f) << 8)) + 2;
            speakerLetter = approachLetter;
            continue;
        }
        case 0xf0:
            if (opcode == 0xfc) {
                addCue(0xfc00U);
                continue;
            }
            if (static_cast<std::uint8_t>(opcode + 10) > 1U && opcode != 0xf8) {
                if (opcode == 0xf0) {
                    cursor = Interpreter(cursor, record);
                    if (!cursor)
                        break;
                    continue;
                }
                if (opcode == 0xf1 || opcode == 0xfd)
                    continue;
                if (opcode == 0xf9) {
                    if (record.flags & ScenarioTalkSavedScript) {
                        cursor = record.savedScriptCursor;
                        record.flags &= ~ScenarioTalkSavedScript;
                        record.savedScriptCursor = nullptr;
                        continue;
                    }
                    if (record.returnScriptCursor) {
                        cursor = record.returnScriptCursor;
                        record.returnScriptCursor = nullptr;
                        continue;
                    }
                }
                break;
            }
            cameraFlags |= 4U;
            break;
        default:
            break;
        }
        break;
    }
    if (cursor)
        cursor -= rewind;
    scenario.dialogueSpeaker = 0;
    scenario.dialogueListener = 0;
    CameraDialogueRecord &terminator = scenarioCameraRecords[packet];
    terminator.flags = static_cast<std::uint8_t>(cameraFlags | 1U);
    terminator.header = 2;
    terminator.type = 0;
    terminator.duration = time;
    terminator.reserved0c = 0;
    terminator.speaker = nullptr;
    terminator.listener = nullptr;
    SCENDialogueCue &end = scenarioDialogueCues[cueIndex];
    end.triggerTime = time;
    end.payload = nullptr;
    end.speaker = {};
    end.reserved14 = 0;
    if (!(scenarioCameraRecords[0].flags & 1U) && !(record.flags & ScenarioTalkInterrupted) && static_cast<std::uint32_t>(record.requestedRoot - 3) > 1U)
        scenario.flags |= 4U;
    return cursor;
}


const std::byte *scenExecuteLineOpcode(ScenarioFreeTalkRecord &record, const std::byte *cursor, std::int32_t &stop) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const std::uint8_t opcode = static_cast<std::uint8_t>(cursor[0]);
    const std::byte *next = cursor + 2;
    const std::int32_t operand = static_cast<std::uint8_t>(cursor[1]) + ((opcode & 0x0f) << 8);
    switch (opcode & 0xf0) {
    case 0x10:
        if (!record.control->resumeOffsets[TalkSelectedSection] && !(record.flags & (ScenarioTalkInterrupted | ScenarioTalkSavedScript)) &&
            !record.requestedRoot)
            record.control->resumeOffsets[TalkSelectedSection] = static_cast<std::uint16_t>(next - record.script);
        break;
    case 0x20:
        goto lineBlock;
    case 0x30: {
        std::int32_t pairs = 0;
        if ((static_cast<std::uint8_t>(*next) & 0xf0) == 0x30) {
            const std::byte *scan = next;
            std::uint8_t value{};
            do {
                value = static_cast<std::uint8_t>(scan[2]);
                scan += 2;
                pairs += 2;
            } while ((value & 0xf0) == 0x30);
        }
        const std::uint8_t following = static_cast<std::uint8_t>(next[pairs]);
        if ((following & 0xf0) == 0x20 || (following == 0xe0 && (static_cast<std::uint8_t>(next[pairs + 4]) & 0xf0) == 0x20))
            goto lineBlock;
        QueueScenarioRecordAction(record, operand);
        for (std::uint8_t value = static_cast<std::uint8_t>(*next); (value & 0xf0) == 0x30; next += 2) {
            QueueScenarioRecordAction(record, ((value & 0x0f) << 8) + static_cast<std::uint8_t>(next[1]));
            value = static_cast<std::uint8_t>(next[2]);
        }
        record.flags |= ScenarioTalkWaitAction;
        record.timers[3] = 0;
        scenario.flags &= ~0x10U;
        record.executionState = 7;
        stop = 1;
        break;
    }
    case 0x40:
        if (!scntExpressionResult)
            next += (operand & 0x800) ? operand | static_cast<std::int32_t>(0xfffff000U) : operand;
        break;
    case 0x50:
        next += (operand & 0x800) ? operand | static_cast<std::int32_t>(0xfffff000U) : operand;
        if (SCEN_TrackVisitedBranchTarget(next))
            stop = 1;
        break;
    case 0x60:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 1);
        break;
    case 0x70:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 0);
        break;
    default:
        break;
    }
    return next;
lineBlock:
    record.firstDialogueLine = nullptr;
    next = scenBuildDialogueCues(record, next - 2);
    record.flags |= (ScenarioTalkLineBlock | ScenarioTalkNextCue | ScenarioTalkPresentationActive);
    record.executionState = 4;
    record.dialoguePreparation = nullptr;
    scenarioLineHoldFrames = 15;
    scenario.flags &= ~0x80U;
    scenario.faceResetCharacter = {};
    stop = 1;
    return next;
}


const std::byte *scenExecuteStreamOpcode(ScenarioFreeTalkRecord &record, const std::byte *cursor, std::int32_t &stop) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const std::uint8_t opcode = static_cast<std::uint8_t>(cursor[0]);
    const std::byte *next = cursor + 1;
    switch (opcode & 0xf0) {
    case 0x80: {
        const std::int32_t offset = ScntBe20(next, opcode);
        record.returnScriptCursor = next + 2;
        return next + 2 + offset;
    }
    case 0x90:
        ++next;
        if ((record.flags & (ScenarioTalkInterrupted | ScenarioTalkSavedScript)) || record.requestedRoot)
            return next;
        record.flags |= ScenarioTalkResumeReached;
        record.control->resumeOffsets[TalkResumeMark] = static_cast<std::uint16_t>(next - record.script);
        record.pendingScriptCursor = nullptr;
        return next;
    case 0xa0:
        ApplyScenarioFaceLook((static_cast<std::uint32_t>(opcode) << 16) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(next[0])) << 8) |
            static_cast<std::uint8_t>(next[1]));
        return next + 2;
    case 0xb0:
        switch (opcode & 0x0f) {
        case 0: {
            std::uint32_t area{};
            std::memcpy(&area, next, sizeof(area));
            GetScenarioTalkPresentationState().followRequest = -1;
            if (RequestNpcAreaTransfer(record.name, area)) {
                npcReleaseSpecialAnchor(record.name);
                SetScenarioFollowCharacter(record.name);
            }
            return next + 4;
        }
        case 1:
            record.field007 = static_cast<std::uint8_t>(*next);
            return next + 1;
        case 2:
            record.control->flags |= 0x20U;
            return next;
        default:
            return next;
        }
    case 0xc0:
        switch (opcode & 0x0f) {
        case 1:
            scenario.dialogueSpeaker = static_cast<std::uint8_t>(*next);
            return next + 1;
        case 2:
            scenario.dialogueListener = static_cast<std::uint8_t>(*next);
            return next + 1;
        case 3:
        case 4:
        case 5:
        case 6:
            reinterpret_cast<std::uint8_t *>(&record.reserved008)[(opcode & 0x0f) - 3] = static_cast<std::uint8_t>(*next);
            return next + 1;
        case 7:
            record.selectionIndex = static_cast<std::int8_t>(*next);
            return next + 1;
        default:
            return next + 1;
        }
    case 0xd0:
        next += ScntBe20(next, opcode) + 2;
        if (SCEN_TrackVisitedBranchTarget(next))
            stop = 1;
        return next;
    default:
        return next;
    }
}

// The E1/E2/E5 prompts settle the same way once the speak icon is up.
const std::byte *SettleScenarioPrompt(ScenarioFreeTalkRecord &record, const std::byte *next, std::int32_t &stop) {
    record.executionState = 6;
    stop = 1;
    if (activeScenarioEventRuntime->flags & 0x4000U)
        ResumeConversationCamera();
    if (record.requestedRoot == 2)
        record.requestedRoot = 5;
    if (record.requestedRoot == 3) {
        record.requestedRoot = 4;
        return record.defaultRoot;
    }
    return next;
}



const std::byte *ExecuteScenarioLinkedChoice(ScenarioFreeTalkRecord &record, const std::byte *header, std::int32_t &stop) {
    record.dialogueTimeout = 10800;
    for (ScenarioChoiceCandidate &candidate : scenarioChoiceCandidates)
        candidate.target = nullptr;
    for (std::size_t index = 0; index < 5; ++index) {
        if (index < 4)
            scenarioChoiceIds[index] = 0xffff;
        else
            scenarioChoiceIdSpare = 0xffff;
        scenarioChoiceRanks[index] = -1;
        scenarioChoiceSources[index] = 0;
    }
    const std::byte *link = header + 2;
    std::uint32_t flagId = static_cast<std::uint8_t>(header[1]) | (static_cast<std::uint8_t>(header[0]) << 8);
    for (std::size_t index = 0; index < 6; ++index)
        scenarioChoiceTargets[index] = nullptr;
    while (!(flagId & 0x8000U)) {
        const std::int32_t offset = ScntBe12(link + 1, static_cast<std::uint8_t>(link[0]));
        const std::byte *const body = link + 2;
        const std::byte *const following = body + offset;
        if (flagId) {
            const std::int8_t rank = static_cast<std::int8_t>(body[0]);
            const std::byte *const descriptor = body + 1;
            if (rank >= 0) {
                const std::uint8_t bucket = static_cast<std::uint8_t>(descriptor[0]);
                const std::int8_t priority = static_cast<std::int8_t>(descriptor[1]);
                if (static_cast<std::uint8_t>(descriptor[2]) == 0xf0) {
                    const std::byte *const target = Interpreter(descriptor + 3, record);
                    ScenarioChoiceCandidate &candidate = scenarioChoiceCandidates[bucket];
                    if (target && scntExpressionResult && (!candidate.target || candidate.priority < priority)) {
                        candidate.flagId = static_cast<std::uint16_t>(flagId);
                        candidate.rank = rank;
                        candidate.priority = priority;
                        candidate.target = target;
                    }
                }
            } else {
                scenarioChoiceIds[0] = static_cast<std::uint16_t>(flagId);
                scenarioChoiceRanks[0] = -1;
                scenarioChoiceSources[0] = 0;
                scenarioChoiceTargets[1] = descriptor;
            }
        } else {
            scenarioChoiceTargets[0] = body;
        }
        flagId = static_cast<std::uint8_t>(following[1]) | (static_cast<std::uint8_t>(following[0]) << 8);
        link = following + 2;
    }
    if (!scenarioChoiceTargets[0])
        scenarioChoiceTargets[0] = link;
    for (std::size_t bucket = 0; bucket < scenarioChoiceCandidates.size(); ++bucket) {
        const ScenarioChoiceCandidate &candidate = scenarioChoiceCandidates[bucket];
        if (!candidate.target)
            continue;
        std::size_t slot = 0;
        while (scenarioChoiceRanks[slot] >= candidate.rank) {
            if (++slot >= 4)
                break;
        }
        if (slot >= 4)
            continue;
        for (std::size_t move = 3; move > slot; --move) {
            scenarioChoiceIds[move] = scenarioChoiceIds[move - 1];
            scenarioChoiceRanks[move] = scenarioChoiceRanks[move - 1];
            scenarioChoiceSources[move] = scenarioChoiceSources[move - 1];
            scenarioChoiceTargets[move + 1] = scenarioChoiceTargets[move];
        }
        scenarioChoiceIds[slot] = candidate.flagId;
        scenarioChoiceTargets[slot + 1] = candidate.target;
        scenarioChoiceRanks[slot] = candidate.rank;
        scenarioChoiceSources[slot] = static_cast<std::uint8_t>(bucket);
    }
    if (scenarioChoiceIds[1] != 0xffff && scenarioChoiceIds[2] == 0xffff) {
        for (std::size_t slot = 2; slot > 0; --slot) {
            scenarioChoiceRanks[slot] = scenarioChoiceRanks[slot - 1];
            scenarioChoiceSources[slot] = scenarioChoiceSources[slot - 1];
            scenarioChoiceTargets[slot + 1] = scenarioChoiceTargets[slot];
        }
        const std::uint16_t first = scenarioChoiceIds[0];
        const std::uint16_t second = scenarioChoiceIds[1];
        scenarioChoiceIds[0] = scenarioChoiceIds[3];
        scenarioChoiceRanks[0] = scenarioChoiceRanks[3];
        scenarioChoiceSources[0] = scenarioChoiceSources[3];
        scenarioChoiceTargets[1] = scenarioChoiceTargets[4];
        scenarioChoiceIds[1] = first;
        scenarioChoiceIds[2] = second;
    }
    std::int32_t pinned = -1;
    std::int32_t offered = 0;
    for (std::int32_t slot = 0; slot < 4; ++slot) {
        if (scenarioChoiceIds[static_cast<std::size_t>(slot)] == 0xffff)
            continue;
        if (scenarioChoiceSources[static_cast<std::size_t>(slot)] == 98)
            pinned = slot;
        ++offered;
    }
    if (pinned >= 0) {
        scenarioChoiceIdSpare = scenarioChoiceIds[static_cast<std::size_t>(pinned)];
        scenarioChoiceRanks[4] = scenarioChoiceRanks[static_cast<std::size_t>(pinned)];
        scenarioChoiceSources[4] = scenarioChoiceSources[static_cast<std::size_t>(pinned)];
        scenarioChoiceTargets[5] = scenarioChoiceTargets[static_cast<std::size_t>(pinned) + 1];
        for (std::int32_t slot = pinned; slot > 0; --slot) {
            const std::size_t index = static_cast<std::size_t>(slot);
            scenarioChoiceIds[index] = scenarioChoiceIds[index - 1];
            scenarioChoiceRanks[index] = scenarioChoiceRanks[index - 1];
            scenarioChoiceSources[index] = scenarioChoiceSources[index - 1];
            scenarioChoiceTargets[index + 1] = scenarioChoiceTargets[index];
        }
        scenarioChoiceIds[0] = 0xffff;
        scenarioChoiceRanks[0] = -1;
        scenarioChoiceSources[0] = 0;
        scenarioChoiceTargets[1] = nullptr;
        const std::size_t placed = offered == 2 ? 1 : 0;
        scenarioChoiceIds[placed] = scenarioChoiceIdSpare;
        scenarioChoiceRanks[placed] = scenarioChoiceRanks[4];
        scenarioChoiceSources[placed] = scenarioChoiceSources[4];
        scenarioChoiceTargets[placed + 1] = scenarioChoiceTargets[5];
    }
    pinned = -1;
    offered = 0;
    for (std::int32_t slot = 0; slot < 4; ++slot) {
        if (scenarioChoiceIds[static_cast<std::size_t>(slot)] == 0xffff)
            continue;
        if (scenarioChoiceSources[static_cast<std::size_t>(slot)] == 99)
            pinned = slot;
        ++offered;
    }
    if (pinned >= 0) {
        const std::size_t from = static_cast<std::size_t>(pinned);
        scenarioChoiceIdSpare = scenarioChoiceIds[from];
        scenarioChoiceRanks[4] = scenarioChoiceRanks[from];
        scenarioChoiceSources[4] = scenarioChoiceSources[from];
        scenarioChoiceTargets[5] = scenarioChoiceTargets[from + 1];
        std::size_t emptied = from;
        if (from < 3) {
            for (std::size_t slot = from; slot < 3; ++slot) {
                scenarioChoiceIds[slot] = scenarioChoiceIds[slot + 1];
                scenarioChoiceRanks[slot] = scenarioChoiceRanks[slot + 1];
                scenarioChoiceSources[slot] = scenarioChoiceSources[slot + 1];
                scenarioChoiceTargets[slot + 1] = scenarioChoiceTargets[slot + 2];
            }
            emptied = 3;
        }
        if (offered == 3) {
            for (std::size_t slot = 3; slot > 0; --slot) {
                scenarioChoiceIds[slot] = scenarioChoiceIds[slot - 1];
                scenarioChoiceRanks[slot] = scenarioChoiceRanks[slot - 1];
                scenarioChoiceSources[slot] = scenarioChoiceSources[slot - 1];
                scenarioChoiceTargets[slot + 1] = scenarioChoiceTargets[slot];
            }
            emptied = 0;
        }
        scenarioChoiceIds[emptied] = 0xffff;
        scenarioChoiceRanks[emptied] = -1;
        scenarioChoiceSources[emptied] = 0;
        scenarioChoiceTargets[emptied + 1] = nullptr;
        const std::size_t placed = offered > 3 ? 3 : offered < 2 ? 0 : static_cast<std::size_t>(offered);
        scenarioChoiceIds[placed] = scenarioChoiceIdSpare;
        scenarioChoiceRanks[placed] = scenarioChoiceRanks[4];
        scenarioChoiceSources[placed] = scenarioChoiceSources[4];
        scenarioChoiceTargets[placed + 1] = scenarioChoiceTargets[5];
    }
    std::array<std::int16_t, 4> choices{};
    for (std::size_t index = 0; index < 4; ++index)
        choices[index] = static_cast<std::int16_t>(scenarioChoiceIds[index]);
    const std::uint16_t timeout = record.dialogueTimeout;
    SetupControlSpeakAndUI_Internal(choices.data(), timeout ? timeout : 0x7fff, -1, timeout ? 2 : 0, record.requestedRoot ? 3 : 1);
    if (record.requestedRoot || (record.flags & ScenarioTalkMoneyIconOpen)) {
        AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash);
        KeepMoneyIconOpen();
        record.flags |= ScenarioTalkMoneyIconOpen;
    }
    record.flags |= ScenarioTalkLinkedPrompt;
    return SettleScenarioPrompt(record, header, stop);
}


const std::byte *scenExecuteChoiceOpcode(ScenarioFreeTalkRecord &record, const std::byte *cursor, std::int32_t &stop) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const std::uint8_t opcode = static_cast<std::uint8_t>(cursor[0]);
    const std::uint8_t length = static_cast<std::uint8_t>(cursor[1]);
    const std::byte *next = cursor + 2;
    if (opcode == 0xe1 || opcode == 0xe2 || opcode == 0xe5) {
        if (record.requestedRoot == 4) {
            record.executionState = 6;
            stop = 1;
            return cursor;
        }
        if (QueueScenarioSpeakerFacing(record)) {
            stop = 1;
            return cursor;
        }
    }
    switch (opcode) {
    case 0xe0: {
        const float random = IncrementSharedPrngFloat();
        const std::int32_t slot = static_cast<std::int32_t>(scenario.randomSelectionCount);
        std::int32_t skip = static_cast<std::int32_t>(random * static_cast<float>(length));
        if (slot < 3) {
            if (scenario.randomSelections[static_cast<std::size_t>(slot)] == skip)
                skip = skip ? skip - 1 : length - 1;
            scenario.randomSelections[static_cast<std::size_t>(slot)] = skip;
            ++scenario.randomSelectionCount;
        }
        for (; skip; --skip)
            next += (static_cast<std::uint8_t>(next[1]) | ((static_cast<std::uint8_t>(next[0]) & 0x0f) << 8)) + 2;
        return next;
    }
    case 0xe1: {
        record.dialogueTimeout = 10800;
        ++next;
        for (std::size_t index = 0; index < 4; ++index, next += 2)
            scenarioChoiceIds[index] = static_cast<std::uint16_t>(static_cast<std::uint8_t>(next[1]) | (static_cast<std::int8_t>(next[0]) << 8));
        std::array<std::int16_t, 4> choices{};
        for (std::size_t index = 0; index < 4; ++index)
            choices[index] = static_cast<std::int16_t>(scenarioChoiceIds[index]);
        SetupControlSpeakAndUI_Internal(choices.data(), 10800, -1, 2, record.requestedRoot ? 3 : 1);
        if (record.requestedRoot || (record.flags & ScenarioTalkMoneyIconOpen)) {
            AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash);
            KeepMoneyIconOpen();
            record.flags |= ScenarioTalkMoneyIconOpen;
        }
        record.flags |= ScenarioTalkChoicePrompt;
        return SettleScenarioPrompt(record, next, stop);
    }
    case 0xe2: {
        std::array<std::int16_t, 4> choices{-1, 0, 1, -1};
        SetupControlSpeakAndUI_Internal(choices.data(), 0x7fff, -1, 3, 1);
        record.flags |= ScenarioTalkBinaryPrompt;
        return SettleScenarioPrompt(record, next, stop);
    }
    case 0xe3:
        return next;
    case 0xe5:
        return ExecuteScenarioLinkedChoice(record, next, stop);
    case 0xe6:
        if (record.requestedRoot != 4) {
            record.flags |= ScenarioTalkContinuePrompt;
            scenarioPromptFrames = 0;
            record.executionState = 6;
            CaptureScenarioTalkInput();
            stop = 1;
            if (scenario.flags & 0x4000U)
                ResumeConversationCamera();
            if (record.requestedRoot == 2)
                record.requestedRoot = 5;
        }
        return cursor + 1;
    default:
        return next + length;
    }
}


const std::byte *scenExecuteControlOpcode(ScenarioFreeTalkRecord &record, const std::byte *cursor, std::int32_t &stop) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    const std::uint8_t opcode = static_cast<std::uint8_t>(cursor[0]);
    const std::byte *next = cursor + 1;
    switch (opcode) {
    case 0xf0:
        return Interpreter(next, record);
    case 0xf2:
        if ((static_cast<std::uint8_t>(cursor[4]) & 0xf0) == 0x90)
            return cursor + 4;
        if (!(record.flags & (ScenarioTalkInterrupted | ScenarioTalkSavedScript)) && !record.requestedRoot)
            record.control->resumeOffsets[TalkWaitPoint] = static_cast<std::uint16_t>(next - record.script);
        record.control->requestedRootOffset = static_cast<std::uint16_t>(next - record.script);
        record.executionState = 5;
        stop = 1;
        return next;
    case 0xf3:
    case 0xfa:
        if (!(scenario.flags & 0x800U)) {
            if (!ryoCheckBusyFlags())
                record.flags |= ScenarioTalkPlayerUnavailable;
            if (record.control->flags & 2U)
                record.flags |= ScenarioTalkAlreadyActivated;
            if (!AreScenarioParticipantsPresent(record))
                record.flags |= ScenarioTalkParticipantsAbsent;
            if (!(scenario.flags & 0x40U) && opcode == 0xf3 &&
                (EV_GetShenmueFlag(0x191) || plymIsInterpolationActive() || !(record.flags & ScenarioTalkAllowUnforcedActivation)))
                record.flags |= ScenarioTalkActivationBlocked;
            if (record.flags &
                (ScenarioTalkPlayerUnavailable | ScenarioTalkAlreadyActivated | ScenarioTalkParticipantsAbsent | ScenarioTalkActivationBlocked)) {
                record.executionState = 3;
                stop = 1;
                return next - 1;
            }
        }
        SetFaceXFlags(record);
        record.flags |= ScenarioTalkSuppressCamera;
        record.control->flags &= ~4U;
        if (record.name.name != SHEN_NAME(SIN_))
            record.control->flags |= 2U;
        SetScenarioActivationCount(record, static_cast<std::int8_t>(record.control->activationCount) + 1);
        return next;
    case 0xf5:
        return cursor + 4;
    case 0xf6: {
        if (record.requestedRoot) {
            record.flags |= ScenarioTalkCharacterActive;
            QueueScenarioCharacterAction(record.name, -3);
            record.flags &= ~ScenarioTalkAllowVoiceOverlap;
            record.executionState = 1;
            stop = 1;
            return next;
        }
        bool completed = true;
        if (!(record.flags & ScenarioTalkCharacterActive)) {
            completed = false;
            record.flags = (record.flags & ~(ScenarioTalkCharacterActive | ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection)) |
                (ScenarioTalkCharacterActive | ScenarioTalkRestartSection);
        }
        ScenarioFreeTalkControl &control = *record.control;
        if (control.resumeOffsets[TalkResumeMark] > control.resumeOffsets[TalkWaitPoint]) {
            record.flags &= ~ScenarioTalkRestartSection;
            completed = false;
            record.flags |= ScenarioTalkResumeAfterSection;
        }
        if (control.resumeOffsets[TalkResumeMark] <= control.resumeOffsets[TalkSelectedSection]) {
            record.flags |= (ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection);
            completed = false;
        }
        if (!completed) {
            QueueScenarioCharacterAction(record.name, -5);
            record.executionState = 3;
            record.scriptCursor = record.script;
            control.resumeOffsets[TalkResumeMark] = 0;
            control.resumeOffsets[TalkWaitPoint] = 0;
            record.flags &= ~ScenarioTalkResumeReached;
            record.flags &= ~ScenarioTalkCompletedResume;
            record.flags &= ~ScenarioTalkAllowVoiceOverlap;
            stop = 1;
            return record.script;
        }
        QueueScenarioCharacterAction(record.name, -3);
        record.flags |= ScenarioTalkResumeReached;
        record.executionState = 1;
        SetScenarioCompletionCount(record, static_cast<std::int8_t>(control.completedCount) + 1);
        record.flags &= ~ScenarioTalkCompletedResume;
        record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        stop = 1;
        return next;
    }
    case 0xf7:
        QueueScenarioCharacterAction(record.name, -4);
        record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        record.executionState = 2;
        stop = 1;
        return next;
    case 0xf8: {
        if (record.requestedRoot) {
            record.flags |= ScenarioTalkCharacterActive;
            QueueScenarioCharacterAction(record.name, -3);
            record.flags &= ~ScenarioTalkAllowVoiceOverlap;
            record.executionState = 1;
            stop = 1;
            return next;
        }
        ScenarioFreeTalkControl &control = *record.control;
        const std::uint16_t resume = control.resumeOffsets[TalkResumeMark];
        if (resume > control.resumeOffsets[TalkSelectedSection] && (record.flags & ScenarioTalkCharacterActive) &&
            resume < control.resumeOffsets[TalkWaitPoint]) {
            QueueScenarioCharacterAction(record.name, -3);
            record.flags |= (ScenarioTalkResumeReached | ScenarioTalkCompletedResume);
            record.executionState = 1;
            SetScenarioCompletionCount(record, static_cast<std::int8_t>(control.completedCount) + 1);
            record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        } else {
            QueueScenarioCharacterAction(record.name, -5);
            record.executionState = 3;
            next = record.script;
            record.scriptCursor = record.script;
            if (!(record.flags & ScenarioTalkCharacterActive))
                record.flags = (record.flags & ~(ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection)) | ScenarioTalkRestartSection;
            if (control.resumeOffsets[TalkResumeMark])
                record.flags |= (ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection);
            record.flags |= ScenarioTalkCharacterActive;
            control.resumeOffsets[TalkResumeMark] = 0;
            control.resumeOffsets[TalkWaitPoint] = 0;
            record.flags &= ~(ScenarioTalkResumeReached | ScenarioTalkCompletedResume);
            record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        }
        stop = 1;
        return next;
    }
    case 0xf9:
        if (record.flags & ScenarioTalkInterrupted) {
            scenario.flags &= ~0x800U;
            next = record.savedScriptCursor;
            record.flags &= ~(ScenarioTalkInterrupted | ScenarioTalkAllowVoiceOverlap | ScenarioTalkPromptClockStarted);
            record.savedScriptCursor = nullptr;
            record.executionState = 5;
            stop = 1;
            return next;
        }
        if (record.flags & ScenarioTalkSavedScript) {
            next = record.savedScriptCursor;
            record.flags &= ~ScenarioTalkSavedScript;
            record.savedScriptCursor = nullptr;
            return next;
        }
        if (record.returnScriptCursor) {
            next = record.returnScriptCursor;
            record.returnScriptCursor = nullptr;
            return next;
        }
        QueueScenarioCharacterAction(record.name, -3);
        record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        record.executionState = 1;
        stop = 1;
        return next;
    case 0xfc:
        scenario.flags &= ~0x10U;
        record.flags |= ScenarioTalkWaitAction;
        record.executionState = 6;
        record.timers[3] = 0;
        stop = 1;
        return next;
    case 0xfd:
        record.flags |= ScenarioTalkPresentationActive;
        record.dialoguePreparation = nullptr;
        scenario.flags &= ~0x80U;
        return next;
    case 0xff:
        record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        record.executionState = 3;
        stop = 1;
        return next;
    default:
        return next;
    }
}

// The answered prompt's flag selects its one-based slot, or zero when it was not offered.
std::uint16_t ResolveScenarioChoiceSlot(ScenarioFreeTalkRecord &record, std::uint16_t answer) {
    std::size_t slot = 0;
    while (scenarioChoiceIds[slot] != answer) {
        if (++slot >= 4)
            break;
    }
    if (slot >= 4) {
        record.dialogueFlagId = 0;
    } else {
        GetScenarioTalkPresentationState().selectedFollowFlag = answer;
        record.dialogueFlagId = static_cast<std::uint16_t>(slot + 1);
    }
    return record.dialogueFlagId;
}


const std::byte *scenSelectRecordStart(ScenarioFreeTalkRecord &record) {
    const std::uint32_t flags = record.flags;
    const std::byte *cursor{};
    const std::uint8_t state = record.executionState;
    if ((flags & ScenarioTalkInterrupted) || state == 6 || record.requestedRoot) {
        cursor = record.scriptCursor;
    } else if (state == 0) {
        cursor = record.script;
        const std::int16_t section = static_cast<std::int16_t>(record.dialogueChoiceIndex);
        if ((activeScenarioEventRuntime->flags & 0x1U) && section != -1) {
            const std::byte *scan = record.script;
            for (std::int32_t index = 0;; ++index) {
                std::uint8_t opcode{};
                do
                    opcode = static_cast<std::uint8_t>(*scan++);
                while ((opcode & 0xf0) != 0x10);
                const std::int32_t length = static_cast<std::uint8_t>(*scan) | ((opcode & 0x0f) << 8);
                const std::byte *const body = scan + 1;
                if (index == section) {
                    cursor = body - 2;
                    break;
                }
                scan = body + length;
            }
        }
    } else if (state >= 4) {
        cursor = record.scriptCursor;
    } else if (state == 1) {
        const ScenarioFreeTalkControl &control = *record.control;
        if (control.resumeOffsets[TalkResumeMark] > control.resumeOffsets[TalkSelectedSection])
            cursor = record.script + control.resumeOffsets[TalkResumeMark] - 2;
        else if (control.resumeOffsets[TalkWaitPoint] > control.resumeOffsets[TalkSelectedSection])
            cursor = record.script + control.resumeOffsets[TalkWaitPoint] - 1;
        else
            cursor = record.script + control.resumeOffsets[TalkSelectedSection];
    } else {
        cursor = record.script;
    }
    std::uint16_t answer = record.dialogueFlagId;
    if (answer == 0xffff || !(flags & (ScenarioTalkBinaryPrompt | ScenarioTalkChoicePrompt | ScenarioTalkLinkedPrompt | ScenarioTalkContinuePrompt)))
        return cursor;
    if (flags & ScenarioTalkChoicePrompt) {
        if (record.requestedRoot == 4)
            cursor = record.alternateRoot + 11;
        answer = ResolveScenarioChoiceSlot(record, answer);
        cursor += 2 * answer;
    }
    if (flags & ScenarioTalkLinkedPrompt) {
        answer = ResolveScenarioChoiceSlot(record, answer);
        cursor = scenarioChoiceTargets[answer] ? scenarioChoiceTargets[answer] : scenarioChoiceTargets[0];
    }
    if (flags & ScenarioTalkBinaryPrompt) {
        if (record.requestedRoot == 4)
            cursor = record.alternateRoot + 2;
        if (answer)
            record.dialogueFlagId = 1;
        answer = record.dialogueFlagId;
        cursor += 2 * answer;
    }
    if ((flags & ScenarioTalkContinuePrompt) && answer)
        cursor += 2;
    if ((flags & (ScenarioTalkChoicePrompt | ScenarioTalkLinkedPrompt)) && (flags & ScenarioTalkMoneyIconOpen)) {
        CloseMoneyIcon();
        record.flags &= ~ScenarioTalkMoneyIconOpen;
    }
    record.flags &= ~(ScenarioTalkBinaryPrompt | ScenarioTalkChoicePrompt | ScenarioTalkLinkedPrompt | ScenarioTalkContinuePrompt);
    if (record.requestedRoot == 4)
        record.requestedRoot = 5;
    record.flags &= ~(ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection);
    return cursor;
}


void scenStepRecordProgram(ScenarioFreeTalkRecord &record) {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    if (!record.name.name) {
        scenario.activeCharacter = {};
        scenario.previousCharacter = {};
        return;
    }
    const std::byte *cursor = scenSelectRecordStart(record);
    if (!cursor)
        return;
    scenarioBranchCount = 0;
    scenarioBranchLoop = 0;
    std::int32_t stop = 0;
    do {
        const std::uint8_t opcode = static_cast<std::uint8_t>(*cursor);
        if (opcode < 0x80)
            cursor = scenExecuteLineOpcode(record, cursor, stop);
        else if (opcode < 0xe0)
            cursor = scenExecuteStreamOpcode(record, cursor, stop);
        else if (opcode < 0xf0)
            cursor = scenExecuteChoiceOpcode(record, cursor, stop);
        else
            cursor = scenExecuteControlOpcode(record, cursor, stop);
        if (!cursor)
            return;
    } while (!stop);
    record.scriptCursor = cursor;
    const std::uint8_t state = record.executionState;
    if (state == 1)
        return;
    if (state == 4) {
        if (record.flags & ScenarioTalkCharacterActive)
            return;
        record.flags |= ScenarioTalkCharacterActive;
        if (!(record.flags & ScenarioTalkInterrupted) && record.requestedRoot != 4) {
            record.timers[0] = 0;
            record.flags |= ScenarioTalkWaitStart;
            QueueScenarioCharacterAction(record.name, -1);
            scenario.flags |= 0x400U;
            CaptureScenarioTalkInput();
        }
        std::memset(scenario.reserved00f + 1, 0, 16);
        return;
    }
    if (state == 5) {
        if (!(record.flags & ScenarioTalkCharacterActive))
            return;
        record.timers[2] = 0;
        record.flags = (record.flags & ~(ScenarioTalkCharacterActive | ScenarioTalkWaitEnd)) | ScenarioTalkWaitEnd;
        QueueScenarioCharacterAction(record.name, -2);
        ReleaseScenarioTalkInput();
        return;
    }
    if (static_cast<std::uint8_t>(state - 6) <= 2U || !(record.flags & ScenarioTalkCharacterActive))
        return;
    record.timers[2] = 0;
    record.flags = (record.flags & ~(ScenarioTalkCharacterActive | ScenarioTalkWaitEnd)) | ScenarioTalkWaitEnd;
    QueueScenarioCharacterAction(record.name, -2);
    EndScenarioTalkSession(record);
}


void scenEndConversation() {
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    RetireScenarioFreeTalkRecord(FindMutableScenarioFreeTalkRecord(scenario.activeCharacter));
    for (SCENDialogueCue &cue : scenarioDialogueCues) {
        cue.triggerTime = ScenarioLineClock();
        cue.payload = nullptr;
        cue.speaker = {};
        cue.reserved14 = 0;
    }
    ScenarioAction acknowledgement{};
    while (ReadScenarioAcknowledgement(acknowledgement)) {
    }
    ReleaseScenarioTalkInput();
    ConsumeScenarioCameraRestoreRequest();
    EndScenarioPlayerPresentation(0);
    if (scenario.flags & 4U)
        EndConversationCameraStream();
    scenario.flags &= 0xfffff726U;
    scenario.activeCharacter = {};
    scenarioLineStartTicks = ScenarioFrameTicks();
}

const char *ScenarioLineText(const ScenarioFreeTalkRecord &record, std::int32_t offset) {
    return GetLocalizedSceneText(reinterpret_cast<const char *>(record.sourceRecord) + offset).data();
}




void scenExecuteRecord(ScenarioFreeTalkRecord *recordPointer) {
    if (!recordPointer)
        return;
    ScenarioFreeTalkRecord &record = *recordPointer;
    ScenarioEventRuntime &scenario = *activeScenarioEventRuntime;
    ScenarioTalkPresentationState &talk = GetScenarioTalkPresentationState();
    if (record.pendingScriptCursor && cvsGet())
        record.pendingScriptCursor = nullptr;
    scenario.flags |= 0x20U;
    scenario.flags = (scenario.flags & 0xffffdfffU) | (record.requestedRoot ? 0x2000U : 0U);
    if ((record.flags & (ScenarioTalkBinaryPrompt | ScenarioTalkChoicePrompt | ScenarioTalkLinkedPrompt)) && plymGetPartyAuthorStatus(record.name) > 0)
        speakSetFlagTo(300);
    record.flags &= ~(ScenarioTalkPlayerUnavailable | ScenarioTalkAlreadyActivated | ScenarioTalkParticipantsAbsent | ScenarioTalkActivationBlocked);
    record.dialogueFlagId = 0xffff;
    if (record.flags & ScenarioTalkChoicePrompt) {
        if (IsCtrlSpeakIconInactive()) {
            record.dialogueFlagId = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
            if (record.dialogueFlagId == 0xffff)
                record.dialogueFlagId = 0;
        } else if (record.requestedRoot != 4) {
            return;
        }
    }
    if (record.flags & ScenarioTalkLinkedPrompt) {
        if (IsCtrlSpeakIconInactive()) {
            record.dialogueFlagId = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
            if (record.dialogueFlagId == 0xffff)
                record.dialogueFlagId = 0;
        } else if (record.requestedRoot != 4) {
            return;
        }
    }
    if (record.flags & ScenarioTalkBinaryPrompt) {
        if (IsCtrlSpeakIconInactive()) {
            const std::uint8_t answer = static_cast<std::uint8_t>(shenGetCtrlSpeakIconFlag());
            record.dialogueFlagId = answer ? (answer == 1 ? 0 : answer) : 1;
            if (record.dialogueFlagId == 0xffff)
                return;
        } else if (record.requestedRoot != 4) {
            return;
        }
    }
    if (record.flags & ScenarioTalkContinuePrompt) {
        if (g_currentMap.name == SHEN_NAME(KWW4) && ++scenarioPromptFrames < 15)
            return;
        if (g_currentMap.name != SHEN_NAME(KWW4))
            ++scenarioPromptFrames;
        const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(scenario.padControlData);
        if (scenarioPromptFrames <= 180) {
            if (!input) {
                record.dialogueFlagId = 0;
            } else if (!(input->pressedPriority & 0x400U)) {
                if (!(input->pressedPriority & 0x200U) && !(input->held & 0xf0c0U) && input->analog0e == 0x80 && input->analog0f == 0x80) {
                    Vector3 position{};
                    GetCharacterMotionWorldPosition(FindScenarioCharacter(scenario.task, record.name), 5, &position);
                    RegisterButtonMediation(SHEN_NAME(BTFT), &position, (ButtonMediationShowButtonHint | ButtonMediationBindX | 0x1U));
                    return;
                }
                record.dialogueFlagId = 0;
            } else {
                record.dialogueFlagId = 1;
            }
        } else {
            record.dialogueFlagId = 1;
        }
    }
    if ((scenario.flags & 8U) && (scenario.flags & 4U))
        camCtrlHoldConversationShot((scenario.flags >> 4) & 1U);
    if (record.flags & (ScenarioTalkWaitAction | ScenarioTalkWaitStart | ScenarioTalkWaitFollow | ScenarioTalkWaitEnd))
        goto acknowledgements;
lineFrame : {
    const std::int32_t hold = scenarioLineHoldFrames;
    if ((record.flags & ScenarioTalkLineBlock) && (ctrlGetPadInputBuffer0(scenario.padControlData)->pressedPriority & 0x200U) &&
        static_cast<std::uint8_t>(GetS2DialogueMode()) && hold <= 0)
        goto skipLine;
    if (!scenarioLineSkipped) {
        if (hold > 0)
            scenarioLineHoldFrames = hold - 1;
        goto answered;
    }
}
skipLine:
    scenarioVoiceWaitState = 0;
    scenarioVoiceWaitArmed = 0;
    GetScenarioTalkPresentationState().voiceWaitDeadline = 0.0f;
    GetScenarioTalkPresentationState().voiceWaitElapsed = 0.0f;
    if (static_cast<std::uint8_t>(GetS2DialogueMode()) != 1 || (scenario.flags & 0x80U) || scenarioLineSkipped) {
        scenCvsCtrlFree(nullptr);
        scenarioLineSkipped = 0;
        scenarioLineTimeOffset += scenario.dialogueCursor->triggerTime - (ScenarioLineClock() + scenarioLineTimeOffset);
        goto lineEnded;
    }
    scenCvsCtrlFree(nullptr);
    ScenarioData(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask)->windowTextState.scenarioStateFlags |= 0x40U;
    if (record.dialoguePreparation) {
        std::int32_t textOffset{};
        std::memcpy(&textOffset, record.dialoguePreparation + 4, sizeof(textOffset));
        shenPrepareDialogue(scenario.task, 1, ScenarioLineText(record, textOffset), static_cast<std::int32_t>(record.ryoSpeaking));
        scenario.flags |= 0x1000U;
    }
    scenario.flags |= 0x80U;
answered:
    if (record.requestedRoot == 4 && record.dialogueFlagId != 0xffff) {
        record.flags &= ~ScenarioTalkCharacterActive;
        if (record.flags & ScenarioTalkLineBlock) {
            scenCvsCtrlFree(nullptr);
            scenarioLineTimeOffset += scenario.dialogueCursor->triggerTime - (ScenarioLineClock() + scenarioLineTimeOffset);
            scenClearSpeakerPresentation();
            goto blockEnded;
        }
    }
    if (!(record.flags & ScenarioTalkLineBlock))
        goto stepProgram;
    if (record.flags & ScenarioTalkNextCue) {
        record.flags &= ~ScenarioTalkNextCue;
        for (;;) {
            SCENDialogueCue &cue = *scenario.dialogueCursor;
            if (cue.payload) {
                if (record.flags & ScenarioTalkWaitAction)
                    return;
                record.control->flags &= ~4U;
                if (scenario.lastSpeakingRecord.name != record.name.name) {
                    if (ScenarioFreeTalkRecord *const other = FindMutableScenarioFreeTalkRecord(scenario.lastSpeakingRecord))
                        other->control->flags |= 4U;
                    scenario.lastSpeakingRecord = record.name;
                }
                std::int32_t participants = talk.participantCount;
                if (participants <= 2 || talk.suppressNpcFacing) {
                    bool facing = false;
                    if (!talk.suppressPlayerFacing && (!talk.playerFacingIssued || (!talk.facingCommandsIssued && !talk.lookActive))) {
                        QueueScenarioCharacterAction(record.name, 140);
                        facing = true;
                    }
                    if (!talk.suppressNpcFacing && (!talk.npcFacingIssued || (!talk.facingCommandsIssued && !talk.lookActive))) {
                        QueueScenarioCharacterAction(record.name, 40);
                        facing = true;
                    }
                    if (facing)
                        goto facingQueued;
                } else if (!talk.facingCommandsIssued && !talk.lookActive) {
                    if (participants > 4)
                        participants = 4;
                    std::int32_t speaker = 0, listener = 0;
                    for (std::int32_t index = 0; index < participants; ++index) {
                        if (talk.participants[static_cast<std::size_t>(index)].name == cue.speaker.name)
                            speaker = index;
                        if (talk.participants[static_cast<std::size_t>(index)].name == cue.reserved14)
                            listener = index;
                    }
                    if (speaker != listener) {
                        if (speaker) {
                            QueueScenarioRecordAction(record, (speaker >= listener ? listener : listener - 1) + 38 + 3 * speaker);
                            QueueScenarioRecordAction(record, speaker + 139);
                        } else {
                            QueueScenarioRecordAction(record, listener + 139);
                        }
                        for (std::int32_t other = 1, base = 3; other < participants; ++other, base += 3) {
                            if (other != speaker)
                                QueueScenarioRecordAction(record, base + (speaker <= other ? speaker : speaker - 1) + 38);
                        }
                        goto facingQueued;
                    }
                }
                if (talk.unfocusedFacingRequested)
                    goto unfocused;
                const std::int32_t *const payload = cue.payload;
                record.firstDialogueLine = reinterpret_cast<const std::byte *>(payload);
                record.dialoguePreparation = reinterpret_cast<const std::byte *>(payload);
                if (!(record.flags & ScenarioTalkAllowVoiceOverlap)) {
                    if (IsScenarioDialoguePlaying(scenarioCurrentDialogue)) {
                        if (!scenarioVoiceWaitState)
                            scenarioVoiceWaitState = 1;
                        return;
                    }
                    if (scenarioVoiceWaitState == 1) {
                        if (!scenarioVoiceWaitArmed) {
                            scenarioVoiceWaitArmed = 1;
                            GetScenarioTalkPresentationState().voiceWaitDeadline = ScenarioLineClock() + 1.0f;
                        }
                        GetScenarioTalkPresentationState().voiceWaitElapsed = ScenarioLineClock();
                        if (GetScenarioTalkPresentationState().voiceWaitDeadline > GetScenarioTalkPresentationState().voiceWaitElapsed)
                            return;
                    }
                    scenarioVoiceWaitState = 0;
                    scenarioVoiceWaitArmed = 0;
                    GetScenarioTalkPresentationState().voiceWaitDeadline = 0.0f;
                    GetScenarioTalkPresentationState().voiceWaitElapsed = 0.0f;
                    scenarioCurrentDialogue = nullptr;
                    if (payload[1]) {
                        std::int32_t remapped = 0;
                        const char *const code = reinterpret_cast<const char *>(record.sourceRecord) + payload[1];
                        scenarioCurrentDialogue = shenPlayAudio(code, cue.speaker, 4, true, &remapped);
                        if (remapped == 1) {
                            scenarioLineSkipped = 1;
                            cue.triggerTime = 0.0f;
                            ++scenario.dialogueCursor;
                            return;
                        }
                        if (scenarioCurrentDialogue && record.field007 && !ScenarioCharacterIsRyo(cue.speaker))
                            SetScenarioDialoguePitch(scenarioCurrentDialogue, static_cast<std::int8_t>(record.field007));
                        bool achievement = false;
                        switch (record.name.name) {
                        case SHEN_NAME(KEK_):
                            achievement = payload[1] == 0x21ae;
                            break;
                        case SHEN_NAME(KEM_):
                            achievement = payload[1] == 0x382e;
                            break;
                        case SHEN_NAME(BKN_):
                            achievement = payload[1] == 0x334c;
                            break;
                        case SHEN_NAME(CHY_):
                            achievement = payload[1] == 0x326c;
                            break;
                        default:
                            break;
                        }
                        if (achievement)
                            IncrementSteamStat(16);
                    }
                    const bool ryoSpeaks = ScenarioCharacterIsRyo(cue.speaker);
                    record.ryoSpeaking = ryoSpeaks ? 1U : 0U;
                    if (!scenarioCurrentDialogue) {
                        shenPrepareDialogue(scenario.task, 1, ScenarioLineText(record, payload[1]), ryoSpeaks ? 1 : 0);
                        scenario.flags |= 0x1000U;
                    }
                    if (!(scenario.flags & 8U)) {
                        scenario.flags |= 8U;
                        scenarioLineStartTicks = ScenarioFrameTicks();
                        if (scenario.flags & 4U) {
                            scenario.flags |= 0x10U;
                            ActivateConversationCameraStream(scenarioCameraRecords.data());
                        }
                    }
                    if (!scenarioCurrentDialogue && cue.speaker.name) {
                        if (Character *const speaker = ResolveScenarioTalkCharacter(cue.speaker)) {
                            BuildScenarioLipSequence(record);
                            AssignFacialClip(speaker, scenario.faceSequence.data());
                        }
                    }
                    scenario.faceResetCharacter = cue.speaker;
                }
            } else {
                const std::uint32_t code = cue.speaker.name;
                switch (code) {
                case 0:
                    return;
                case 0xb0:
                    talk.followRequest = -1;
                    if (RequestNpcAreaTransfer(record.name, cue.reserved14)) {
                        npcReleaseSpecialAnchor(record.name);
                        SetScenarioFollowCharacter(record.name);
                    }
                    break;
                case 0xb1:
                    record.field007 = static_cast<std::uint8_t>(cue.reserved14);
                    break;
                case 0xb2:
                    record.control->flags |= 0x20U;
                    break;
                default:
                    if ((code & 0xf00000U) == 0xa00000U) {
                        ApplyScenarioFaceLook(code);
                    } else if ((code & 0xf000U) == 0x3000U) {
                        QueueScenarioRecordAction(record, static_cast<std::int32_t>(code & 0xfffU));
                        scenario.flags &= ~0x10U;
                        record.flags |= ScenarioTalkWaitAction;
                        record.timers[3] = 0;
                        scenarioLinePauseTicks = ScenarioFrameTicks() - scenarioLineStartTicks;
                    } else if ((code & 0xf000U) == 0x6000U) {
                        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(code & 0xfffU), 1);
                    } else if ((code & 0xf000U) == 0x7000U) {
                        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(code & 0xfffU), 0);
                    } else if (code == 0xfc00U) {
                        scenario.flags &= ~0x10U;
                        record.flags |= ScenarioTalkWaitAction;
                        record.timers[3] = 0;
                        scenarioLinePauseTicks = ScenarioFrameTicks() - scenarioLineStartTicks;
                        ++scenario.dialogueCursor;
                        return;
                    }
                    break;
                }
            }
            ++scenario.dialogueCursor;
            if (ScenarioLineClock() + scenarioLineTimeOffset < scenario.dialogueCursor->triggerTime) {
                talk.facingCommandsIssued = 0;
                return;
            }
            continue;
        facingQueued:
            talk.facingCommandsIssued = 1;
            if (talk.unfocusedFacingRequested) {
            unfocused:
                QueueScenarioCharacterAction(record.name, -16);
            }
            scenario.flags &= ~0x10U;
            record.flags |= ScenarioTalkWaitAction;
            record.timers[3] = 0;
            scenarioLinePauseTicks = ScenarioFrameTicks() - scenarioLineStartTicks;
            return;
        }
    }
    if (scenario.dialogueCursor->triggerTime > ScenarioLineClock() + scenarioLineTimeOffset || IsScenarioDialoguePlaying(scenarioCurrentDialogue))
        return;
lineEnded:
    if (scenario.flags & 0x80U) {
        scenario.flags &= ~0x80U;
        record.dialoguePreparation = nullptr;
    }
    scenClearSpeakerPresentation();
    if (scenario.dialogueCursor->speaker.name) {
        record.flags |= ScenarioTalkNextCue;
        scenarioLineHoldFrames = 15;
        return;
    }
blockEnded:
    record.flags &= ~(ScenarioTalkLineBlock | ScenarioTalkSuppressCamera);
    if (scenario.flags & 8U) {
        if (scenario.flags & 4U)
            EndConversationCameraStream();
        scenario.flags &= 0xffffffe7U;
    }
stepProgram : {
    ScenarioFreeTalkControl &control = *record.control;
    const std::uint32_t controlFlags = control.flags;
    if (controlFlags & 4U) {
        if (controlFlags & 1U) {
            control.resumeOffsets[TalkSelectedSection] = 0;
            control.flags &= ~1U;
        }
        control.flags &= ~0x20U;
        const std::byte *const section = SCEN_AdvanceCharacterCommandStream(record);
        if (section) {
            if (section - record.script != control.resumeOffsets[TalkSelectedSection]) {
                record.scriptCursor = record.script;
                control.resumeOffsets[TalkSelectedSection] = static_cast<std::uint16_t>(section - record.script);
                control.resumeOffsets[TalkResumeMark] = 0;
                control.resumeOffsets[TalkWaitPoint] = 0;
                control.completedCount = 0;
                control.activationCount = 0;
                record.flags &= ~ScenarioTalkResumeReached;
                record.executionState = 0;
                scenStepRecordProgram(record);
                return;
            }
            if (controlFlags & 0x20U)
                control.flags |= 0x20U;
        } else {
            record.executionState = 0;
            control.resumeOffsets[TalkSelectedSection] = 0;
            control.completedCount = 0;
            control.activationCount = 0;
        }
    }
    if (record.executionState == 3 && !(control.flags & 4U)) {
        if (record.name.name == SHEN_NAME(SIN_)) {
            control.flags |= 4U;
            control.flags |= 1U;
        }
        return;
    }
    if (record.executionState == 5 && !(record.flags & ScenarioTalkInterrupted)) {
        if (IsItemSelectable() || IsAnyItemSelectActive() || (plymIsSearchBlocked() && !(scenario.flags & 0x40U)) || !AreScenarioParticipantsPresent(record))
            return;
        record.flags &= ~ScenarioTalkAllowVoiceOverlap;
        std::int32_t frames = 0;
        if (record.flags & ScenarioTalkPromptClockStarted) {
            frames = scenarioPromptFrames;
        } else {
            record.flags |= ScenarioTalkPromptClockStarted;
            scenarioPromptFrames = 0;
        }
        if ((scenario.flags & 0x800U) || frames < 30)
            scenarioPromptFrames = ++frames;
        if (frames < 180 && ((record.requestedRoot - 2) & 0xfffffffd) != 0 && !btnCheck(SHEN_NAME(BTFT)) && !btnCheck(SHEN_NAME(BTMT))) {
            if (record.flags & ScenarioTalkPresentationActive) {
                if (((scenario.flags & 0x100U) || (record.flags & ScenarioTalkResumeReached)) &&
                    GetParentMenu() == static_cast<std::uint8_t>(UIElement::GameBackground)) {
                    scenario.flags &= ~0x100U;
                    EndScenarioTalkSession(record);
                    return;
                }
                const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(ctrlGetPadData());
                if ((!input || (input->pressedPriority & 0x200U) || (input->held & 0xf0c0U) || input->analog0e != 0x80 || input->analog0f != 0x80) &&
                    GetParentMenu() == static_cast<std::uint8_t>(UIElement::GameBackground)) {
                    EndScenarioTalkSession(record);
                    return;
                }
            }
            if (record.name.name && scenarioPromptFrames > 4) {
                Vector3 position{};
                GetCharacterMotionWorldPosition(FindScenarioCharacter(scenario.task, record.name), 5, &position);
                const std::uint32_t busy = ryoCheckBusyFlags() ? ButtonMediationShowButtonHint : 0U;
                scenarioHardBusy = 1;
                if (record.flags & ScenarioTalkResumeReached)
                    scenarioTalkPromptResumed = 1;
                RegisterButtonMediation(SHEN_NAME(BTFT), &position, busy | ButtonMediationBindX | 1U);
                if (!record.requestedRoot && (!(scenario.flags & 0x800U) || (record.control->flags & 2U)) && IsScenarioFollowPromptAvailable()) {
                    if (record.control->flags & 8U)
                        scenarioFollowPromptShown = 1;
                    RegisterButtonMediation(SHEN_NAME(BTMT), &position, busy | ButtonMediationBindY | 1U);
                }
            }
            return;
        }
        if (btnCheck(SHEN_NAME(BTMT))) {
            if (!record.defaultRoot) {
                ExecuteAicaCommand(0x800ab, 0, 0);
                return;
            }
            record.requestedRoot = 1;
        }
        record.flags &= ~(ScenarioTalkResumeAfterSection | ScenarioTalkRestartSection);
        SetFaceXFlags(record);
        if (record.requestedRoot == 1) {
            const std::byte *root = record.alternateRoot;
            if (root) {
                record.requestedRoot = 3;
            } else {
                root = record.defaultRoot;
                record.requestedRoot = 2;
            }
            record.scriptCursor = root;
            record.control->flags |= 8U;
            record.flags &= ~ScenarioTalkSavedScript;
            record.savedScriptCursor = nullptr;
            record.pendingScriptCursor = nullptr;
        } else {
            ScenarioFreeTalkControl &owner = *record.control;
            if (owner.flags & 4U) {
                const std::byte *const entry = record.script + (record.requestedRoot ? owner.requestedRootOffset : owner.resumeOffsets[TalkWaitPoint]);
                const std::int32_t value =
                    static_cast<std::uint8_t>(entry[2]) + ((static_cast<std::uint8_t>(entry[1]) + (static_cast<std::uint8_t>(entry[0]) << 8)) << 8);
                const std::int32_t offset = (value & 0x800000) ? value | static_cast<std::int32_t>(0xff000000U) : value;
                const std::byte *const body = entry + 3;
                if (offset) {
                    record.flags |= ScenarioTalkSavedScript;
                    record.savedScriptCursor = body;
                    record.scriptCursor = body + offset;
                } else {
                    record.savedScriptCursor = nullptr;
                    record.scriptCursor = body;
                }
                owner.flags &= ~4U;
            } else {
                record.scriptCursor += 3;
            }
            if (record.requestedRoot == 2)
                record.requestedRoot = 5;
        }
    }
    scenarioLineTimeOffset = 0.0f;
    scenarioLineHoldFrames = 0;
    scenario.randomSelectionCount = 0;
    scenStepRecordProgram(record);
    return;
}
acknowledgements:
    for (;;) {
        ScenarioAction acknowledgement{};
        if (ReadScenarioAcknowledgement(acknowledgement) < 1) {
            const std::uint32_t flags = record.flags;
            if (flags & 0x1000U)
                ++record.timers[3];
            if (flags & 0x2000U)
                ++record.timers[0];
            if (flags & 0x4000U)
                ++record.timers[1];
            if (flags & 0x8000U)
                ++record.timers[2];
            if (static_cast<std::int16_t>(record.timers[3]) >= 30)
                acknowledgement = {record.name, -10};
            else if (static_cast<std::int16_t>(record.timers[0]) >= 30)
                acknowledgement = {record.name, -1};
            else if (static_cast<std::int16_t>(record.timers[2]) >= 30)
                acknowledgement = {record.name, -2};
            else if (static_cast<std::int16_t>(record.timers[1]) >= 30)
                acknowledgement = {record.name, -8};
            else
                return;
        }
        for (;;) {
            if (acknowledgement.name.name != record.name.name)
                return;
            if (acknowledgement.action == -8) {
                scenario.flags &= ~0x800U;
                record.flags &= ~ScenarioTalkWaitFollow;
                record.timers[1] = 0;
                if (record.requestedRoot) {
                    record.executionState = 1;
                    record.requestedRoot = 0;
                }
                record.flags &= ~ScenarioTalkPromptClockStarted;
                return;
            }
            if (acknowledgement.action == -10) {
                record.flags &= ~ScenarioTalkWaitAction;
                record.timers[3] = 0;
                if (record.flags & ScenarioTalkLineBlock) {
                    scenario.flags |= 0x10U;
                    scenarioLineStartTicks = ScenarioFrameTicks() - scenarioLinePauseTicks;
                }
            } else if (acknowledgement.action == -2) {
                record.flags &= ~ScenarioTalkWaitEnd;
                record.timers[2] = 0;
                UpdateMotionWorkWithPosition(FindMotionWork(ScenarioRyo()), nullptr, 0);
                EndScenarioPlayerPresentation(3);
            } else if (acknowledgement.action == -1) {
                record.flags &= ~ScenarioTalkWaitStart;
                record.timers[0] = 0;
            }
            if (ReadScenarioAcknowledgement(acknowledgement) <= 0)
                break;
        }
        if (!(record.flags & (ScenarioTalkWaitAction | ScenarioTalkWaitStart | ScenarioTalkWaitFollow | ScenarioTalkWaitEnd)))
            goto lineFrame;
    }
}
} // namespace event_runtime_detail


} // namespace shenmue::s2
