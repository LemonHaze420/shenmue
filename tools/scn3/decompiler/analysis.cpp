#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/bytecode/value.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"

namespace shenmue::scenario {
namespace {

constexpr VmValue unknown{.kind = VmValueKind::Unknown};

VmValue join(VmValue left, VmValue right) {
    if (left == right || right.kind == VmValueKind::Undefined)
        return left;
    if (left.kind == VmValueKind::Undefined)
        return right;
    return unknown;
}

bool known(VmValue value) {
    return value.kind != VmValueKind::Undefined && value.kind != VmValueKind::Unknown;
}

struct State {
    std::vector<VmValue> stack;
    std::map<uint32_t, VmValue> frame;
    VmValue accumulator = unknown;
    VmValue operand = unknown;
    VmValue parameterCount = unknown;
    bool operator==(const State &) const = default;
};

VmValue pop(State &state) {
    if (state.stack.empty())
        return unknown;
    const VmValue value = state.stack.back();
    state.stack.pop_back();
    return value;
}

State join(const State &left, const State &right) {
    State result;
    const size_t count = std::min(left.stack.size(), right.stack.size());
    result.stack.resize(count);
    for (size_t index = 0; index < count; ++index)
        result.stack[count - 1 - index] = join(left.stack[left.stack.size() - 1 - index], right.stack[right.stack.size() - 1 - index]);
    for (const std::pair<const uint32_t, VmValue> &local : left.frame) {
        const std::map<uint32_t, VmValue>::const_iterator other = right.frame.find(local.first);
        if (other != right.frame.end())
            result.frame[local.first] = join(local.second, other->second);
    }
    result.accumulator = join(left.accumulator, right.accumulator);
    result.operand = join(left.operand, right.operand);
    result.parameterCount = join(left.parameterCount, right.parameterCount);
    return result;
}

VmValue integer(uint32_t bits) {
    return VmValue{.kind = VmValueKind::Integer, .bits = bits};
}

VmValue immediate(const Instruction &instruction, VmValue previous) {
    if (!instruction.imm)
        return previous;
    const uint32_t bits = *instruction.imm;
    return integer(instruction.immWidth == 1 ? static_cast<uint32_t>(static_cast<int8_t>(bits)) :
            instruction.immWidth == 2        ? static_cast<uint32_t>(static_cast<int16_t>(bits)) :
                                               bits);
}

VmValue parameter(const Program &program, const FunctionInfo &function, uint32_t offset) {
    if (offset < 8 || offset % 4)
        return unknown;
    const std::map<uint32_t, std::vector<VmValue>>::const_iterator inputs = program.callArguments.find(function.start);
    if (inputs != program.callArguments.end()) {
        const size_t index = (offset - 8U) / 4U;
        return index < inputs->second.size() ? inputs->second[index] : unknown;
    }
    if (function.start == program.codeStart || function.start == program.parsedHeader.startupFunctionOffset)
        return unknown;
    return {};
}

VmValue load(const State &state, const Program &program, const FunctionInfo &function, VmValue address, uint32_t size) {
    if (address.kind != VmValueKind::FrameAddress)
        return unknown;
    const std::map<uint32_t, VmValue>::const_iterator local = state.frame.find(address.bits);
    VmValue value = local != state.frame.end() ? local->second : parameter(program, function, address.bits);
    if (size == 4 || !known(value))
        return value;
    if (value.kind != VmValueKind::Integer)
        return unknown;
    return integer(size == 1 ? static_cast<uint32_t>(static_cast<int8_t>(value.bits)) : static_cast<uint32_t>(static_cast<int16_t>(value.bits)));
}

VmValue binary(Alu operation, VmValue left, VmValue right) {
    if (left.kind == VmValueKind::Undefined || right.kind == VmValueKind::Undefined)
        return {};
    if (!known(left) || !known(right))
        return unknown;
    if ((operation == Alu::Add || operation == Alu::Subtract) && right.kind == VmValueKind::Integer && left.kind != VmValueKind::Integer) {
        left.bits = operation == Alu::Add ? left.bits + right.bits : left.bits - right.bits;
        return left;
    }
    if (operation == Alu::Add && left.kind == VmValueKind::Integer && right.kind != VmValueKind::Integer) {
        right.bits += left.bits;
        return right;
    }
    if (left.kind != VmValueKind::Integer || right.kind != VmValueKind::Integer)
        return unknown;
    const uint32_t a = left.bits, b = right.bits;
    switch (operation) {
    case Alu::Equal:
        return integer(a == b ? UINT32_MAX : 0);
    case Alu::NotEqual:
        return integer(a != b ? UINT32_MAX : 0);
    case Alu::Less:
        return integer(static_cast<int32_t>(a) < static_cast<int32_t>(b) ? UINT32_MAX : 0);
    case Alu::LessEqual:
        return integer(static_cast<int32_t>(a) <= static_cast<int32_t>(b) ? UINT32_MAX : 0);
    case Alu::Greater:
        return integer(static_cast<int32_t>(a) > static_cast<int32_t>(b) ? UINT32_MAX : 0);
    case Alu::GreaterEqual:
        return integer(static_cast<int32_t>(a) >= static_cast<int32_t>(b) ? UINT32_MAX : 0);
    case Alu::And:
        return integer(a & b);
    case Alu::Or:
        return integer(a | b);
    case Alu::Xor:
        return integer(a ^ b);
    case Alu::Add:
        return integer(a + b);
    case Alu::Subtract:
        return integer(a - b);
    case Alu::Multiply:
        return integer(a * b);
    case Alu::Divide:
    case Alu::Modulo:
        if (!b || (a == 0x80000000U && b == UINT32_MAX))
            return unknown;
        return integer(static_cast<uint32_t>(
            operation == Alu::Divide ? static_cast<int32_t>(a) / static_cast<int32_t>(b) : static_cast<int32_t>(a) % static_cast<int32_t>(b)));
    case Alu::ShiftLeft:
        return b < 32 ? integer(a << b) : unknown;
    case Alu::ShiftRight:
        return b < 32 ? integer(static_cast<uint32_t>(static_cast<int32_t>(a) >> b)) : unknown;
    default:
        return unknown;
    }
}

VmValue addressValue(const Program &program, const FunctionInfo &function, const Instruction &instruction, VmValue operand) {
    if (!known(operand))
        return operand;
    if (instruction.addrBase == AddrBase::CurrBase) {
        if (instruction.addrKind == AddrKind::Literal)
            return operand;
        const uint32_t target = function.baseOffset + operand.bits;
        return VmValue{.kind = target >= program.codeEnd ? VmValueKind::DataAddress : VmValueKind::CodeAddress, .bits = target};
    }
    return VmValue{.kind = instruction.addrBase == AddrBase::PrevSp ? VmValueKind::FrameAddress : VmValueKind::TaskAddress, .bits = operand.bits};
}

std::vector<VmValue> callArguments(const FunctionInfo &function, size_t index, const State &state) {
    size_t count = 0;
    const Instruction &instruction = function.instructions[index];
    if (controlOperation(instruction.opcode) == Control::CallScnf && known(state.parameterCount))
        count = state.parameterCount.bits;
    else if (index + 1 < function.instructions.size()) {
        const Instruction &cleanup = function.instructions[index + 1];
        const VmValue bytes = immediate(cleanup, state.operand);
        if (cleanup.cls == InstrClass::Ctrl && controlOperation(cleanup.opcode) == Control::AdjustStack && known(bytes) && static_cast<int32_t>(bytes.bits) > 0)
            count = bytes.bits / 4U;
    }
    if (count > state.stack.size())
        return {};
    std::vector<VmValue> arguments;
    arguments.reserve(count);
    for (size_t argument = 0; argument < count; ++argument)
        arguments.push_back(state.stack[state.stack.size() - 1 - argument]);
    return arguments;
}

void mergeArguments(std::map<uint32_t, std::vector<VmValue>> &inputs, uint32_t target, const std::vector<VmValue> &arguments) {
    const std::map<uint32_t, std::vector<VmValue>>::iterator previous = inputs.find(target);
    if (previous == inputs.end()) {
        inputs.emplace(target, arguments);
        return;
    }
    std::vector<VmValue> &merged = previous->second;
    const size_t common = std::min(merged.size(), arguments.size());
    merged.resize(std::max(merged.size(), arguments.size()), unknown);
    for (size_t index = 0; index < merged.size(); ++index)
        merged[index] = index < common ? join(merged[index], arguments[index]) : unknown;
}

} // namespace

namespace {

struct Transfer {
    State state;
    std::vector<uint32_t> successors;
    std::optional<VmCallSite> call;
};

Transfer transfer(const Program &program, const FunctionInfo &function, size_t index, State input) {
    Transfer result{.state = std::move(input), .successors = {}, .call = {}};
    State &state = result.state;
    std::vector<uint32_t> &successors = result.successors;
    const Instruction &instruction = function.instructions[index];
    const uint32_t next = instruction.offset + instruction.size;
    if (instruction.cls == InstrClass::Addr || instruction.cls == InstrClass::Ctrl)
        state.operand = immediate(instruction, state.operand);
    if (instruction.cls == InstrClass::Addr) {
        const VmValue address = addressValue(program, function, instruction, state.operand);
        state.stack.push_back(instruction.addrBase == AddrBase::CurrBase || instruction.addrKind == AddrKind::NewPointer ?
                address :
                load(state, program, function, address,
                    instruction.addrKind == AddrKind::Idx8     ? 1U :
                        instruction.addrKind == AddrKind::Ld16 ? 2U :
                                                                 4U));
    } else if (instruction.cls == InstrClass::Alu) {
        const Alu operation = aluOperation(instruction.opcode);
        if (operation == Alu::PopEax)
            state.accumulator = pop(state);
        else if (operation == Alu::PushEax)
            state.stack.push_back(state.accumulator);
        else if (operation >= Alu::Load8 && operation <= Alu::Load32) {
            const VmValue address = pop(state);
            state.stack.push_back(load(state, program, function, address, 1U << (static_cast<uint32_t>(operation) - static_cast<uint32_t>(Alu::Load8))));
        } else if (operation >= Alu::Store8 && operation <= Alu::Store32) {
            const VmValue value = pop(state), address = pop(state);
            state.frame.clear();
            if (operation == Alu::Store32 && address.kind == VmValueKind::FrameAddress)
                state.frame[address.bits] = value;
            state.stack.push_back(value);
        } else if (operation == Alu::Not || operation == Alu::IntToFloat || operation == Alu::FloatToInt) {
            const VmValue value = pop(state);
            state.stack.push_back(operation == Alu::Not && value.kind == VmValueKind::Integer ? integer(~value.bits) :
                    value.kind == VmValueKind::Undefined                                      ? value :
                                                                                                unknown);
        } else {
            const Operation *spec = operationOf(instruction.opcode);
            if (spec && spec->inputs == 2) {
                const VmValue right = pop(state), left = pop(state);
                state.stack.push_back(binary(operation, left, right));
            } else
                state.stack.clear();
        }
    } else if (instruction.cls == InstrClass::Ctrl) {
        const Control control = controlOperation(instruction.opcode);
        if (control == Control::LatchOperand)
            state.parameterCount = state.operand;
        else if (control == Control::AdjustStack && known(state.operand)) {
            const int32_t words = static_cast<int32_t>(state.operand.bits) / 4;
            if (words > 0)
                state.stack.resize(state.stack.size() > static_cast<size_t>(words) ? state.stack.size() - words : 0);
            else if (words < 0 && index != 0)
                state.stack.clear();
        } else if (control == Control::LocalCall || control == Control::IndirectCall || nativeCall(control)) {
            VmValue target = state.operand;
            if (control == Control::IndirectCall)
                target = pop(state);
            if (known(target)) {
                const uint32_t identity = control == Control::LocalCall ? function.baseOffset + target.bits : target.bits;
                VmCallSite call{.offset = instruction.offset, .target = identity, .control = control, .arguments = callArguments(function, index, state)};
                result.call = std::move(call);
            }
            state.accumulator = unknown;
            state.frame.clear();
        }
        if (control == Control::Return || control == Control::ScnfOffset)
            return result;
        if (control == Control::LocalJump || control == Control::Jump) {
            if (known(state.operand))
                successors.push_back((control == Control::LocalJump ? function.baseOffset : next) + state.operand.bits);
        } else if (control == Control::BranchFalse && known(state.operand)) {
            if (!known(state.accumulator) || !state.accumulator.bits)
                successors.push_back(next + state.operand.bits);
            if (!known(state.accumulator) || state.accumulator.bits)
                successors.push_back(next);
        } else
            successors.push_back(next);
    } else
        return result;
    if (instruction.cls != InstrClass::Ctrl)
        successors.push_back(next);
    return result;
}

} // namespace

std::vector<VmCallSite> analyzeVmCalls(const Program &program, const FunctionInfo &function) {
    std::vector<VmCallSite> calls;
    if (function.instructions.empty())
        return calls;
    std::map<uint32_t, size_t> positions;
    for (size_t index = 0; index < function.instructions.size(); ++index)
        positions[function.instructions[index].offset] = index;
    std::vector<std::optional<State>> inputs(function.instructions.size());
    inputs[0] = State{};
    std::vector<size_t> pending{0};
    for (size_t cursor = 0; cursor < pending.size(); ++cursor) {
        const size_t index = pending[cursor];
        const Transfer output = transfer(program, function, index, *inputs[index]);
        for (uint32_t successor : output.successors) {
            const std::map<uint32_t, size_t>::const_iterator position = positions.find(successor);
            if (position == positions.end())
                continue;
            const size_t destination = position->second;
            if (!inputs[destination]) {
                inputs[destination] = output.state;
                pending.push_back(destination);
            } else {
                State merged = join(*inputs[destination], output.state);
                if (merged != *inputs[destination]) {
                    inputs[destination] = std::move(merged);
                    pending.push_back(destination);
                }
            }
        }
    }
    for (size_t index = 0; index < inputs.size(); ++index) {
        if (!inputs[index])
            continue;
        Transfer output = transfer(program, function, index, *inputs[index]);
        if (output.call)
            calls.push_back(std::move(*output.call));
    }
    return calls;
}

VmProgramAnalysis analyzeVmProgram(const Program &program) {
    VmProgramAnalysis result;
    result.arguments = program.callArguments;
    for (const std::pair<const uint32_t, FunctionInfo> &entry : program.functions)
        for (const VmCallSite &call : analyzeVmCalls(program, entry.second)) {
            if (call.control == Control::LocalCall || call.control == Control::IndirectCall) {
                if (call.target >= program.codeStart && call.target < program.codeEnd)
                    mergeArguments(result.arguments, call.target, call.arguments);
                continue;
            }
            if (call.target > UINT16_MAX)
                continue;
            const abi::CommandSpec *command = abi::findCommandByDispatch(static_cast<uint8_t>(call.control), static_cast<uint16_t>(call.target), 0);
            if (!command || !command->handlerName)
                continue;
            std::vector<std::optional<uint32_t>> constants;
            for (VmValue argument : call.arguments)
                constants.push_back(known(argument) ? std::optional<uint32_t>{argument.bits} : std::nullopt);
            const NativeArgumentLayout *layout = nativeArgumentLayout(command->handlerName, constants);
            if (!layout || layout->callbackOffset < 0 || layout->countOffset < 0 || layout->tailOffset < 0)
                continue;
            const size_t callback = static_cast<size_t>(layout->callbackOffset) / 4U, count = static_cast<size_t>(layout->countOffset) / 4U;
            const size_t tail = static_cast<size_t>(layout->tailOffset) / 4U;
            if (callback >= call.arguments.size())
                continue;
            const VmValue target = call.arguments[callback];
            if (target.kind != VmValueKind::CodeAddress || target.bits < program.codeStart || target.bits >= program.codeEnd)
                continue;
            result.callbackTargets.insert(target.bits);
            std::vector<VmValue> arguments;
            if (count < call.arguments.size() && known(call.arguments[count]) && tail <= call.arguments.size() &&
                call.arguments[count].bits <= call.arguments.size() - tail)
                arguments.assign(call.arguments.begin() + static_cast<std::ptrdiff_t>(tail),
                    call.arguments.begin() + static_cast<std::ptrdiff_t>(tail + call.arguments[count].bits));
            mergeArguments(result.arguments, target.bits, arguments);
        }
    return result;
}

} // namespace shenmue::scenario
