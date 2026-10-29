#include "scn3/backend/types.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario::decompile {

SourceExpr encodedOperand(const LowLevelInsn &insn, const LowLevelFunction &function, const std::set<uint32_t> &labels,
    const std::map<uint32_t, std::string> &data, const std::string &profile) {
    if (!insn.immWidth)
        return identifier("vm_operand");
    SourceExpr value = number(insn.imm.value_or(0));
    if (insn.relativeTarget && labels.count(*insn.relativeTarget))
        value = identifier("L" + hexName(*insn.relativeTarget, 6));
    else if (insn.localTarget && labels.count(*insn.localTarget))
        value = identifier("L" + hexName(*insn.localTarget, 6));
    else if (insn.cls == InstrClass::Addr && addressOperation(insn.opcode) >= Address::Relative1 && addressOperation(insn.opcode) <= Address::Relative3) {
        const uint32_t target = function.baseOffset + insn.imm.value_or(0);
        const std::map<uint32_t, std::string>::const_iterator symbol = data.find(target);
        if (symbol != data.end())
            value = identifier(symbol->second);
        else if (labels.count(target))
            value = identifier("L" + hexName(target, 6));
    } else if (insn.cls == InstrClass::Ctrl && controlOperation(insn.opcode) == Control::ScnfOffset) {
        const std::map<uint32_t, std::string>::const_iterator symbol = data.find(function.baseOffset + insn.imm.value_or(0));
        if (symbol != data.end())
            value = identifier(symbol->second);
    } else if (insn.cls == InstrClass::Ctrl && profile == "s1-pc" && insn.imm.value_or(0) <= 0xFFFFU) {
        const abi::CommandSpec *command = abi::findCommandByDispatch(static_cast<uint8_t>(controlOperation(insn.opcode)), static_cast<uint16_t>(*insn.imm), 0);
        if (command && command->handlerName)
            value = identifier(lookupAbiName(command->family, command->publicIndex, "command"));
    }
    return SourceExpr{.kind = SourceExprKind::Cast, .text = "i" + std::to_string(immWidthBytes(insn.immWidth) * 8U), .args = {std::move(value)}};
}

std::vector<SourceStmt> liftFunction(const LowLevelFunction &function, const std::set<uint32_t> &labels, const std::map<uint32_t, std::string> &data,
    const std::string &profile, const SourceUnit &verification) {
    std::vector<SourceStmt> result;
    std::vector<SourceExpr> pending;
    const auto flush = [&]() {
        for (SourceExpr &expr : pending)
            result.push_back(expressionStatement(call("vm_push", {std::move(expr)})));
        pending.clear();
    };
    uint32_t position = function.start;
    for (size_t index = 0; index < function.instructions.size(); ++index) {
        const LowLevelInsn &insn = function.instructions[index];
        if (insn.offset != position)
            throw std::runtime_error("Explicit export requires a contiguous function view at " + hexUnsigned(insn.offset));
        position += static_cast<uint32_t>(insn.originalBytes.size());
        if (labels.count(insn.offset)) {
            flush();
            result.push_back(SourceStmt{.kind = SourceStmtKind::Label, .label = "L" + hexName(insn.offset, 6)});
        }
        if (insn.cls == InstrClass::Addr) {
            SourceExpr operand = encodedOperand(insn, function, labels, data, profile);
            if (addressOperation(insn.opcode) == Address::Literal && insn.immWidth)
                pending.push_back(std::move(operand));
            else
                pending.push_back(call(operation(addressOperation(insn.opcode)).name, {std::move(operand)}));
            continue;
        }
        if (insn.cls == InstrClass::Alu) {
            const Alu alu = aluOperation(insn.opcode);
            if (alu == Alu::PushEax) {
                pending.push_back(identifier("eax"));
                continue;
            }
            if (alu == Alu::PopEax && !pending.empty()) {
                SourceExpr value = std::move(pending.back());
                pending.pop_back();
                flush();
                result.push_back(SourceStmt{.kind = SourceStmtKind::Assign, .expr = identifier("eax"), .value = std::move(value)});
                continue;
            }
            if (static_cast<size_t>(alu) < std::size(kAluOperations)) {
                const Operation &op = kAluOperations[static_cast<size_t>(alu)];
                if (op.inputs && pending.size() >= op.inputs) {
                    std::vector<SourceExpr> args(pending.end() - op.inputs, pending.end());
                    pending.resize(pending.size() - op.inputs);
                    if (op.binary) {
                        for (SourceExpr &arg : args)
                            if (explicitExpressionIsFloat(arg))
                                arg = call("as_word", {std::move(arg)});
                        pending.push_back(SourceExpr{.kind = SourceExprKind::Binary, .text = op.binary, .args = std::move(args)});
                    } else if (alu >= Alu::Store8 && alu <= Alu::Store32) {
                        const Operation *address = args[0].kind == SourceExprKind::Call ? operationNamed(args[0].text) : nullptr;
                        const uint8_t width = static_cast<uint8_t>(alu) - static_cast<uint8_t>(Alu::Store8);
                        const Address pointer = address ? addressOperation(address->opcode) : Address::Literal;
                        SourceExpr lvalue = address && address->opcode >= kAddressClass && addressBase(pointer) != AddrBase::CurrBase &&
                                addressAccess(pointer) == kPointerAccess ?
                            call(operation(addressOperation(addressBase(pointer), width)).name, std::move(args[0].args)) :
                            call("vm_mem" + std::to_string(8U << width), {std::move(args[0])});
                        pending.push_back(SourceExpr{.kind = SourceExprKind::Binary, .text = "=", .args = {std::move(lvalue), std::move(args[1])}});
                    } else
                        pending.push_back(call(op.name, std::move(args)));
                } else {
                    flush();
                    result.push_back(expressionStatement(call(op.name)));
                }
            } else {
                flush();
                result.push_back(expressionStatement(call("vm_alu", {number(static_cast<uint32_t>(alu))})));
            }
            continue;
        }
        if (insn.cls == InstrClass::Ctrl) {
            const Control control = controlOperation(insn.opcode);
            if (control == Control::LatchOperand && insn.immWidth == 1 && profile == "s1-pc" && index + 1 < function.instructions.size()) {
                const LowLevelInsn &invocation = function.instructions[index + 1];
                const uint32_t count = insn.imm.value_or(0xFFFFFFFFU);
                const uint32_t dispatch = invocation.imm.value_or(0xFFFFFFFFU);
                const abi::CommandSpec *command =
                    dispatch <= 0xFFFFU ? abi::findCommandByDispatch(static_cast<uint8_t>(Control::CallScnf), static_cast<uint16_t>(dispatch), 0) : nullptr;
                size_t consumed = 1;
                bool cleanupMatches = count == 0;
                if (count && index + 2 < function.instructions.size()) {
                    const LowLevelInsn &cleanup = function.instructions[index + 2];
                    cleanupMatches = cleanup.cls == InstrClass::Ctrl && controlOperation(cleanup.opcode) == Control::AdjustStack && cleanup.imm == count * 4U &&
                        cleanup.immWidth == naturalWidth(count * 4U) && !labels.count(cleanup.offset);
                    if (cleanupMatches)
                        ++consumed;
                }
                if (command && command->handlerName && invocation.cls == InstrClass::Ctrl && controlOperation(invocation.opcode) == Control::CallScnf &&
                    invocation.immWidth == (dispatch < 128U ? 1 : 2) && cleanupMatches && pending.size() >= count && !labels.count(invocation.offset)) {
                    std::vector<SourceExpr> args;
                    for (size_t i = 0; i < count; ++i)
                        args.push_back(std::move(pending[pending.size() - 1U - i]));
                    pending.resize(pending.size() - count);
                    SourceExpr named = call(lookupAbiName(command->family, command->publicIndex, "command"), std::move(args));
                    for (size_t i = 0; i < consumed; ++i)
                        position += static_cast<uint32_t>(function.instructions[++index].originalBytes.size());
                    if (index + 1 < function.instructions.size() && function.instructions[index + 1].opcode == encode(Alu::PushEax) &&
                        !labels.count(function.instructions[index + 1].offset)) {
                        pending.push_back(std::move(named));
                        ++position;
                        ++index;
                    } else {
                        flush();
                        result.push_back(expressionStatement(std::move(named)));
                    }
                    continue;
                }
            }
            if (control == Control::LocalCall || (nativeCall(control) && control != Control::CallScnf)) {
                std::string name;
                bool zeroArguments = false;
                if (control == Control::LocalCall && insn.localTarget) {
                    for (const std::pair<const std::string, SourceFunctionAst> &candidate : verification.functions) {
                        if (candidate.second.offset != *insn.localTarget || insn.immWidth != 3)
                            continue;
                        name = candidate.first;
                        zeroArguments = candidate.second.params.empty();
                        break;
                    }
                } else if (profile == "s1-pc" && insn.imm && *insn.imm <= 0xFFFFU && insn.immWidth == (*insn.imm < 128U ? 1 : 2)) {
                    const abi::CommandSpec *command = abi::findCommandByDispatch(static_cast<uint8_t>(control), static_cast<uint16_t>(*insn.imm), 0);
                    if (command && command->handlerName) {
                        name = lookupAbiName(command->family, command->publicIndex, "command");
                        zeroArguments = nativeCallIgnoresParameters(command->handlerName);
                    }
                }
                size_t count = 0;
                bool cleanupMatches = false;
                if (index + 1 < function.instructions.size()) {
                    const LowLevelInsn &cleanup = function.instructions[index + 1];
                    const uint32_t bytes = cleanup.imm.value_or(0xFFFFFFFFU);
                    cleanupMatches = cleanup.cls == InstrClass::Ctrl && controlOperation(cleanup.opcode) == Control::AdjustStack && bytes > 0 &&
                        bytes % 4U == 0 && cleanup.immWidth == naturalWidth(bytes) && bytes / 4U <= pending.size() && !labels.count(cleanup.offset);
                    if (cleanupMatches)
                        count = bytes / 4U;
                }
                if (!name.empty() && (cleanupMatches || zeroArguments)) {
                    std::vector<SourceExpr> args;
                    for (size_t i = 0; i < count; ++i)
                        args.push_back(std::move(pending[pending.size() - 1U - i]));
                    pending.resize(pending.size() - count);
                    SourceExpr invocation = call(name, std::move(args));
                    if (cleanupMatches)
                        position += static_cast<uint32_t>(function.instructions[++index].originalBytes.size());
                    if (index + 1 < function.instructions.size() && function.instructions[index + 1].opcode == encode(Alu::PushEax) &&
                        !labels.count(function.instructions[index + 1].offset)) {
                        pending.push_back(std::move(invocation));
                        ++position;
                        ++index;
                    } else {
                        flush();
                        result.push_back(expressionStatement(std::move(invocation)));
                    }
                    continue;
                }
            }
            SourceExpr operand = encodedOperand(insn, function, labels, data, profile);
            if (control == Control::LocalCall || control == Control::IndirectCall || nativeCall(control)) {
                std::vector<SourceExpr> args{std::move(operand)};
                for (size_t i = pending.size(); i > 0; --i)
                    args.push_back(std::move(pending[i - 1]));
                pending.clear();
                result.push_back(expressionStatement(call(operation(control).name, std::move(args))));
            } else {
                flush();
                if (control == Control::Return && !insn.immWidth)
                    result.push_back(SourceStmt{.kind = SourceStmtKind::Return});
                else if (control == Control::BranchFalse) {
                    SourceExpr condition{.kind = SourceExprKind::Unary, .text = "!", .args = {identifier("eax")}};
                    result.push_back(SourceStmt{
                        .kind = SourceStmtKind::If, .expr = std::move(condition), .body = {expressionStatement(call("vm_goto_rel", {std::move(operand)}))}});
                } else
                    result.push_back(expressionStatement(call(operation(control).name, {std::move(operand)})));
            }
            continue;
        }
        flush();
        result.push_back(expressionStatement(call("vm_reserved", {number(insn.opcode)})));
    }
    flush();
    return result;
}

void attachUnreferencedReturns(Scn3Module &module) {
    std::set<uint32_t> referenced{module.parsedHeader.startupFunctionOffset};
    for (const LowLevelFunction &function : module.functions)
        for (const LowLevelInsn &insn : function.instructions) {
            if (insn.relativeTarget)
                referenced.insert(*insn.relativeTarget);
            if (insn.localTarget)
                referenced.insert(*insn.localTarget);
            if (insn.cls == InstrClass::Addr && addressOperation(insn.opcode) >= Address::Relative1 && addressOperation(insn.opcode) <= Address::Relative3 &&
                insn.imm)
                referenced.insert(function.baseOffset + *insn.imm);
        }
    const uint32_t dataEnd = module.parsedHeader.dataEndOffset;
    for (uint32_t offset = module.parsedHeader.relocationTableOffset; offset + 8U <= module.originalBlob.size(); offset += 8U) {
        const uint32_t kind = readU32(module.originalBlob, offset);
        const uint32_t site = readU32(module.originalBlob, offset + 4U);
        if (!kind && !site)
            break;
        if (kind == 0x2402U && site >= module.parsedHeader.dataSegmentOffset && site + 4U <= dataEnd)
            referenced.insert(readU32(module.originalBlob, site));
    }
    for (size_t i = 1; i < module.functions.size();) {
        LowLevelFunction &owner = module.functions[i - 1];
        const LowLevelFunction &fragment = module.functions[i];
        const bool contiguous =
            !owner.instructions.empty() && owner.instructions.back().offset + owner.instructions.back().originalBytes.size() == fragment.start;

        const uint8_t first = fragment.instructions.empty() ? 0 : fragment.instructions[0].opcode;
        if (fragment.reason == DiscoveryReason::PackedNext && !fragment.instructions.empty() &&
            !(first < kAddressClass && controlOperation(first) == Control::AdjustStack) && contiguous && fragment.baseOffset == owner.baseOffset &&
            !referenced.count(fragment.start)) {
            owner.instructions.insert(owner.instructions.end(), fragment.instructions.begin(), fragment.instructions.end());
            module.functions.erase(module.functions.begin() + static_cast<std::ptrdiff_t>(i));
        } else
            ++i;
    }
}

} // namespace shenmue::scenario::decompile
