#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/cli/diagnostics.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

std::string baseText(AddrBase base) {
    switch (base) {
    case AddrBase::CurrBase:
        return "curr_sp->R0";
    case AddrBase::ScriptData:
        return "task->script_data_ptr";
    case AddrBase::ScenarioWork:
        return "task->scenarioWork";
    case AddrBase::PrevSp:
        return "prev_sp";
    }
    return "base?";
}

std::string addrKindText(AddrKind kind) {
    switch (kind) {
    case AddrKind::Literal:
        return "IMM";
    case AddrKind::Idx8:
        return "IDX8";
    case AddrKind::Ld16:
        return "LD16";
    case AddrKind::Ld32:
        return "LD32";
    case AddrKind::NewPointer:
        return "NEWP";
    case AddrKind::RelativeOffset:
        return "RELOFF";
    }
    return "?";
}

std::string rawAddrLine(const Instruction &insn) {
    const std::string immHex = hexUnsigned(insn.imm.value_or(0));
    if (insn.addrKind == AddrKind::Literal) {
        return "PUSH_IMM  push imm ; imm=" + immHex;
    }
    if (insn.addrKind == AddrKind::RelativeOffset) {
        return "PUSH_RELOFF  push (curr_sp->R0 + imm) ; imm=" + immHex;
    }
    const std::string base = baseText(insn.addrBase);
    std::ostringstream oss;
    oss << "PUSH_PTR  BASE=" << base << " ; " << addrKindText(insn.addrKind) << "  ";
    switch (insn.addrKind) {
    case AddrKind::Idx8:
        oss << "push [BASE+imm]";
        break;
    case AddrKind::Ld16:
        oss << "push *(u16*)(BASE+imm)";
        break;
    case AddrKind::Ld32:
        oss << "push *(u32*)(BASE+imm)";
        break;
    case AddrKind::NewPointer:
        oss << "push Script_GetNewPointer(BASE+imm)";
        break;
    case AddrKind::RelativeOffset:
    case AddrKind::Literal:
        break;
    }
    oss << " ; imm=" << immHex;
    return oss.str();
}

std::string callFamilyName(const Instruction &insn) {
    const int index = static_cast<int>(insn.imm.value_or(0));
    switch (controlOperation(insn.opcode)) {
    case Control::ScnfOffset:
        return "ExecuteSCNFScriptFn_0";
    case Control::CallLegacy:
        return lookupLegacy(index);
    case Control::CallGp:
        return lookupGp(index);
    case Control::CallShenmue:
        return lookupShenmue(index);
    case Control::CallScnf:
        return lookupScnf(index);
    case Control::CallDswk:
        return lookupDSWK(index);
    default:
        return "CallFamily[?]";
    }
}

std::string instructionBytes(const Program &program, const Instruction &insn) {
    std::ostringstream oss;
    for (uint32_t i = 0; i < insn.size && insn.offset + i < program.blob.size(); ++i) {
        if (i != 0)
            oss << ' ';
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(program.blob[insn.offset + i]);
    }
    return oss.str();
}

std::string disassemblyText(const Program &program, const Instruction &insn) {
    if (insn.cls == InstrClass::Addr) {
        return rawAddrLine(insn);
    }
    if (insn.cls == InstrClass::Alu) {
        const Alu sub = aluOperation(insn.opcode);
        switch (sub) {
        case Alu::PopEax:
            return "POP.EAX";
        case Alu::PushEax:
            return "PUSH.EAX";
        case Alu::Store8:
            return "ST1.R0->R1 (SP+=16)";
        case Alu::Store16:
            return "ST2.R0->R1 (SP+=16)";
        case Alu::Store32:
            return "ST4.R0->R1 (SP+=16)";
        case Alu::Equal:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::NotEqual:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::Less:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::LessEqual:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::Greater:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::GreaterEqual:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::Not:
            return "R0=~R0";
        case Alu::And:
            return "R1=R1&R0 (SP+=16)";
        case Alu::Or:
            return "R1=R1|R0 (SP+=16)";
        case Alu::Xor:
            return "R1=R1^R0 (SP+=16)";
        case Alu::Add:
            return "R1=R1+R0 (SP+=16)";
        case Alu::Subtract:
            return "R1=R1-R0 (SP+=16)";
        case Alu::Multiply:
            return "R1=R1*R0 (SP+=16)";
        case Alu::Divide:
            return "R1=R1/R0 (SP+=16)";
        case Alu::Modulo:
            return "R1=R1%R0 (SP+=16)";
        case Alu::ShiftLeft:
            return "R1=R1<<R0 (SP+=16)";
        case Alu::ShiftRight:
            return "R1=R1>>R0 (SP+=16)";
        case Alu::IntToFloat:
            return "R0=(float)R0";
        case Alu::FloatToInt:
            return "R0=(int)R0";
        case Alu::FloatLess:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::FloatLessEqual:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::FloatGreater:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::FloatGreaterEqual:
            return comparisonRawText(sub) + " (SP+=16)";
        case Alu::FloatAdd:
            return "R1=R1+R0 (SP+=16)";
        case Alu::FloatSubtract:
            return "R1=R1-R0 (SP+=16)";
        case Alu::FloatMultiply:
            return "R1=R1*R0 (SP+=16)";
        case Alu::FloatDivide:
            return "R1=R1/R0 (SP+=16)";
        default:
            return "ALU_" + hexUnsigned(static_cast<uint32_t>(sub), 2);
        }
    }
    if (insn.cls == InstrClass::Ctrl) {
        const uint32_t imm = insn.imm.value_or(0);
        switch (controlOperation(insn.opcode)) {
        case Control::Return:
            return "RET/UNWIND";
        case Control::LocalCall:
            return "CALL abs  ip=(curr_sp->R0 + imm) ; target=" + formatScriptFunctionName(program, insn.localTarget);
        case Control::IndirectCall:
            return "CALL abs  ip=<stack-derived> ; imm=" + hexUnsigned(imm);
        case Control::AdjustStack:
            return "ADD.SP    sp+=4*(imm/4) ; imm=" + hexUnsigned(imm);
        case Control::LocalJump:
            return "JMP abs   ip=(curr_sp->R0 + imm) ; target=" + formatScriptFunctionName(program, insn.localTarget);
        case Control::Jump:
            return "JMP rel   ip+=imm ; imm=" + hexUnsigned(imm) + " ; target=" + hexUnsigned(insn.relativeTarget);
        case Control::BranchFalse:
            return "BRANCH.IF_FALSE   if(EAX==0) ip+=imm ; imm=" + hexUnsigned(imm) + " ; target=" + hexUnsigned(insn.relativeTarget);
        case Control::LatchOperand:
            return "LATCH.Operand  operand=" + hexUnsigned(imm);
        case Control::ScnfOffset:
            return "CALL_ASYNC_SCNF_OFFSET unresolved/yielding ; imm=" + hexUnsigned(imm);
        case Control::CallLegacy:
            return "CALL " + callFamilyName(insn) + ",(... ) ; imm=" + hexUnsigned(imm);
        case Control::CallGp:
            return "CALL " + callFamilyName(insn) + ",(... ) ; imm=" + hexUnsigned(imm);
        case Control::CallShenmue:
            return "CALL " + callFamilyName(insn) + ",(... ) ; imm=" + hexUnsigned(imm);
        case Control::CallScnf:
            return "CALL " + callFamilyName(insn) + ",(latched_param_count args) ; imm=" + hexUnsigned(imm);
        case Control::CallDswk:
            return "CALL " + callFamilyName(insn) + ",(... ) ; imm=" + hexUnsigned(imm);
        default:
            return "CTRL_" + hexUnsigned(static_cast<uint32_t>(controlOperation(insn.opcode)), 2) + " ; imm=" + hexUnsigned(imm);
        }
    }
    return "DB " + hexUnsigned(insn.opcode, 2);
}

bool shouldRenderFunction(const FunctionInfo &function, const std::set<uint32_t> &referencedStarts) {
    if (referencedStarts.count(function.start) != 0) {
        return true;
    }
    for (const Instruction &insn : function.instructions) {
        if (insn.cls != InstrClass::Unknown) {
            return true;
        }
    }
    return false;
}

std::set<uint32_t> collectReferencedFunctionStarts(const Program &program) {
    std::set<uint32_t> starts{program.codeStart};
    starts.insert(program.scriptCreatedTargets.begin(), program.scriptCreatedTargets.end());
    std::vector<uint32_t> worklist(starts.begin(), starts.end());
    for (size_t i = 0; i < worklist.size(); ++i) {
        const std::map<uint32_t, FunctionInfo>::const_iterator fit = program.functions.find(worklist[i]);
        if (fit == program.functions.end())
            continue;
        for (uint32_t target : fit->second.localTargets) {
            if (starts.insert(target).second) {
                worklist.push_back(target);
            }
        }
    }
    return starts;
}

void renderDisassembly(const Program &program, bool showBytes) {
    const std::set<uint32_t> referencedStarts = collectReferencedFunctionStarts(program);
    bool first = true;
    for (const auto &[_, function] : program.functions) {
        if (!shouldRenderFunction(function, referencedStarts)) {
            continue;
        }
        if (!first) {
            std::cout << "\n";
        }
        first = false;
        std::cout << formatScriptFunctionName(program, function.start) << ":\n";
        for (const Instruction &insn : function.instructions) {
            if (function.labelTargets.count(insn.offset) != 0) {
                std::cout << "L" << hexOffset(insn.offset) << ":\n";
            }
            std::cout << hexOffset(insn.offset) << ": ";
            if (showBytes) {
                std::cout << std::left << std::setw(16) << instructionBytes(program, insn) << std::right;
            } else {
                std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(insn.opcode) << "  ";
            }
            std::cout << disassemblyText(program, insn) << "\n";
        }
    }
}

} // namespace shenmue::scenario
