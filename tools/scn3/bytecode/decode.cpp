#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

ScenarioHeader parseHeader(const std::vector<uint8_t> &blob) {
    if (blob.size() < sizeof(ScenarioHeader)) {
        throw std::runtime_error("File too small for SCN3 header");
    }
    ScenarioHeader header{};
    std::memcpy(&header, blob.data(), sizeof(header));
    if (header.token != 0x334E4353u && header.token != 0x464E4353u) {
        throw std::runtime_error("Invalid SCN token: expected 0x334E4353 (SCN3) or 0x464E4353 (SCNF), got " + hexUnsigned(header.token, 8));
    }
    if (static_cast<std::uint32_t>(header.wholeSize) > blob.size()) {
        throw std::runtime_error("Header wholeSize exceeds file size");
    }
    if (static_cast<std::uint32_t>(header.dataSegmentOffset) > blob.size() || static_cast<std::uint32_t>(header.entrypoint) > blob.size() ||
        static_cast<std::uint32_t>(header.functionOffset) > blob.size()) {
        throw std::runtime_error("Header offsets exceed file size");
    }
    if (header.entrypoint > header.dataSegmentOffset) {
        throw std::runtime_error("Function pool start is past data segment offset");
    }
    if (header.functionOffset > header.dataSegmentOffset) {
        throw std::runtime_error("Startup function offset is past data segment offset");
    }
    return header;
}

ParsedHeader parseHeaderSemantics(const ScenarioHeader &header) {
    ParsedHeader parsed{};
    parsed.token = header.token;
    parsed.wholeSize = header.wholeSize;
    parsed.flags = header.flags;
    parsed.startupFunctionOffset = header.functionOffset;
    parsed.dataSegmentOffset = header.dataSegmentOffset;
    parsed.dataEndOffset = header.dataEndOffset;
    parsed.workspaceSize = header.workspaceSize;
    parsed.field1C = header.unknownOffset;
    parsed.functionPoolStart = header.entrypoint;
    parsed.relocationTableOffset = header.relocationTableOffset;
    parsed.field28 = header.unknown1;
    parsed.field2C = header.unknown2;
    return parsed;
}

std::string scenarioFlagsText(uint32_t flags) {
    std::string text;
    if ((flags & SCF_Unknown0100) != 0) {
        text = "SCF_Unknown0100";
        flags &= ~SCF_Unknown0100;
    }
    if ((flags & SCF_NativeCodeMask) == SCF_NativeCodeMask) {
        text += (text.empty() ? "" : " | ") + std::string("SCF_NativeCodeMask");
        flags &= ~SCF_NativeCodeMask;
    }
    if (flags || text.empty())
        text += (text.empty() ? "" : " | ") + hexUnsigned(flags);
    return text;
}

std::string tokenString(uint32_t token) {
    std::string out(4, '\0');
    out[0] = static_cast<char>(token & 0xFFu);
    out[1] = static_cast<char>((token >> 8) & 0xFFu);
    out[2] = static_cast<char>((token >> 16) & 0xFFu);
    out[3] = static_cast<char>((token >> 24) & 0xFFu);
    return out;
}

std::string publicAbiCallName(const abi::CommandSpec &command) {
    std::string name = command.handlerName != nullptr ? std::string(command.handlerName) : std::string();
    if (command.family != abi::Family::SCNF) {
        return name;
    }
    static const std::string scriptPrefix = "Script_";
    static const std::string scnfPrefix = "SCNF_";
    if (name.rfind(scnfPrefix, 0) == 0) {
        return name;
    }
    if (name.rfind(scriptPrefix, 0) == 0) {
        return scnfPrefix + name.substr(scriptPrefix.size());
    }
    return scnfPrefix + name;
}

std::string lookupAbiName(abi::Family family, int index, const char *fallbackPrefix) {
    if (index < 0 || index > static_cast<int>(std::numeric_limits<uint16_t>::max())) {
        return std::string(fallbackPrefix) + "_" + hexName(static_cast<uint32_t>(index), 4);
    }
    const abi::CommandSpec *command = abi::findCommand(family, static_cast<uint16_t>(index));
    if (command == nullptr || command->handlerName == nullptr || command->handlerName[0] == '\0') {
        return std::string(fallbackPrefix) + "_" + hexName(static_cast<uint32_t>(index), 4);
    }

    size_t nameCount = 0;
    for (const abi::CommandSpec &other : abi::kCommands) {
        if (other.family == family && other.handlerName != nullptr && std::string_view(other.handlerName) == command->handlerName) {
            ++nameCount;
        }
    }
    const std::string publicName = publicAbiCallName(*command);
    if (nameCount > 1 || publicName == "SCNF_ShowSubtitleText") {
        return publicName + "_" + hexName(command->publicIndex, 4);
    }
    return publicName;
}

std::string lookupAbiDispatchName(abi::Family family, int dispatchIndex, uint8_t dispatchSubslot, const char *fallbackPrefix) {
    if (dispatchIndex < 0 || dispatchIndex > static_cast<int>(std::numeric_limits<uint16_t>::max())) {
        return std::string(fallbackPrefix) + "_" + hexName(static_cast<uint32_t>(dispatchIndex), 4);
    }
    const abi::FamilySpec *familySpec = abi::findFamily(family);
    if (familySpec == nullptr) {
        return std::string(fallbackPrefix) + "_" + hexName(static_cast<uint32_t>(dispatchIndex), 4);
    }
    const abi::CommandSpec *command = abi::findCommandByDispatch(familySpec->opcodeFamily, static_cast<uint16_t>(dispatchIndex), dispatchSubslot);
    if (command == nullptr) {
        return std::string(fallbackPrefix) + "_" + hexName(static_cast<uint32_t>(dispatchIndex), 4);
    }
    return lookupAbiName(family, command->publicIndex, fallbackPrefix);
}

std::string lookupLegacy(int index) {
    return lookupAbiName(abi::Family::Legacy, index, "LegacyFn");
}

std::string lookupGp(int index) {
    return lookupAbiName(abi::Family::GeneralPurpose, index, "GeneralPurposeFn");
}

std::string lookupShenmue(int index) {
    return lookupAbiName(abi::Family::Shenmue, index, "ShenmueFn");
}

std::string lookupDSWK(int index) {
    return lookupAbiName(abi::Family::DSWK, index, "DSWKFn");
}

std::string lookupScnf(int index) {
    return lookupAbiDispatchName(abi::Family::SCNF, index, 0, "SCNF");
}

std::string formatScriptFunctionName(const Program &program, uint32_t offset) {
    if (offset < program.codeStart || offset >= program.codeEnd) {
        return std::string("script_offset_") + hexOffset(offset);
    }
    const char *prefix = program.scriptCreatedTargets.count(offset) != 0 ? "script_task_func_" : "script_local_func_";
    return std::string(prefix) + hexOffset(offset);
}

Instruction decodeInstruction(const std::vector<uint8_t> &blob, uint32_t off, uint32_t functionBase, uint32_t codeEnd) {
    if (off >= codeEnd || off >= blob.size()) {
        throw std::runtime_error("Decode offset out of range");
    }
    Instruction insn{};
    insn.offset = off;
    insn.opcode = blob[off];
    insn.immWidth = immediateWidth(insn.opcode);
    insn.cls = insn.opcode < kAddressClass ? InstrClass::Ctrl :
        insn.opcode < kAluClass            ? InstrClass::Addr :
        insn.opcode < kReservedClass       ? InstrClass::Alu :
                                             InstrClass::Unknown;

    uint32_t cursor = off + 1;
    if (insn.cls == InstrClass::Ctrl || insn.cls == InstrClass::Addr) {
        switch (insn.immWidth) {
        case 0:
            break;
        case 1:
            ensureCodeReadable(blob, cursor, 1, codeEnd, "u8 immediate");
            insn.imm = blob[cursor];
            cursor += 1;
            break;
        case 2:
            insn.imm = readU16Code(blob, cursor, codeEnd);
            cursor += 2;
            break;
        case 3:
            insn.imm = readU32Code(blob, cursor, codeEnd);
            cursor += 4;
            break;
        default:
            break;
        }
    }
    insn.size = static_cast<uint8_t>(cursor - off);

    if (insn.cls == InstrClass::Addr) {
        insn.addrBase = addressBase(addressOperation(insn.opcode));
        const uint8_t addrKind = addressAccess(addressOperation(insn.opcode));
        if (insn.addrBase == AddrBase::CurrBase) {
            insn.addrKind = (addrKind == 0) ? AddrKind::Literal : AddrKind::RelativeOffset;
        } else if (addrKind == 0) {
            insn.addrKind = AddrKind::Idx8;
        } else if (addrKind == 1) {
            insn.addrKind = AddrKind::Ld16;
        } else if (addrKind == 2) {
            insn.addrKind = AddrKind::Ld32;
        } else {
            insn.addrKind = AddrKind::NewPointer;
        }
    }

    const Control control = controlOperation(insn.opcode);
    if (insn.cls == InstrClass::Ctrl) {
        if (control == Control::Jump || control == Control::BranchFalse) {
            if (insn.imm.has_value()) {
                insn.hasRelativeTarget = true;
                switch (insn.immWidth) {
                case 1:
                    insn.relativeImmediate = static_cast<int8_t>(insn.imm.value());
                    break;
                case 2:
                    insn.relativeImmediate = static_cast<int16_t>(insn.imm.value());
                    break;
                case 3:
                    insn.relativeImmediate = static_cast<int32_t>(insn.imm.value());
                    break;
                default:
                    insn.relativeImmediate = 0;
                    break;
                }
                insn.relativeTarget = static_cast<uint32_t>(off + insn.size + insn.relativeImmediate);
            }
        } else if (control == Control::LocalCall || control == Control::LocalJump) {
            insn.hasLocalTarget = true;
            insn.localTarget = functionBase + insn.imm.value_or(0);
        }
    }
    return insn;
}

uint32_t immWidthBytes(uint8_t immWidth) {
    switch (immWidth) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 4;
    default:
        throw std::runtime_error("Invalid immediate width code");
    }
}

bool isReservedOpcode(uint8_t opcode) {
    return opcode >= kReservedClass;
}

std::string discoveryReasonName(DiscoveryReason reason) {
    switch (reason) {
    case DiscoveryReason::FunctionPoolStart:
        return "function_pool_start";
    case DiscoveryReason::StartupEntry:
        return "startup_entry";
    case DiscoveryReason::LocalCall:
        return "local_call";
    case DiscoveryReason::TaskCreated:
        return "task_created";
    case DiscoveryReason::PackedNext:
        return "packed_next";
    case DiscoveryReason::DecodeUncovered:
        return "decode_uncovered";
    }
    return "unknown";
}

std::string rangeKindName(RangeKind kind) {
    switch (kind) {
    case RangeKind::Header:
        return "header";
    case RangeKind::DecodedInstruction:
        return "decoded_instruction";
    case RangeKind::RawCodeGap:
        return "raw_code_gap";
    case RangeKind::TailMergedStubCandidate:
        return "tail_merged_stub_candidate";
    case RangeKind::PackedPadding:
        return "packed_padding";
    case RangeKind::DataSegment:
        return "data_segment";
    case RangeKind::TrailingDataOrUnknown:
        return "trailing_data_or_unknown";
    case RangeKind::Unowned:
        return "unowned";
    }
    return "unowned";
}

int discoveryReasonPriority(DiscoveryReason reason) {
    switch (reason) {
    case DiscoveryReason::StartupEntry:
        return 6;
    case DiscoveryReason::FunctionPoolStart:
        return 5;
    case DiscoveryReason::TaskCreated:
        return 4;
    case DiscoveryReason::LocalCall:
        return 3;
    case DiscoveryReason::PackedNext:
        return 2;
    case DiscoveryReason::DecodeUncovered:
        return 1;
    }
    return 0;
}

std::optional<std::string> comparisonOperatorForAlu(Alu operation) {
    if (operation >= Alu::FloatLess && operation <= Alu::FloatGreaterEqual)
        operation = static_cast<Alu>(static_cast<uint8_t>(operation) - static_cast<uint8_t>(Alu::FloatLess) + static_cast<uint8_t>(Alu::Less));
    if (operation >= Alu::Equal && operation <= Alu::GreaterEqual)
        return kAluOperations[static_cast<uint8_t>(operation)].binary;
    return std::nullopt;
}

std::string comparisonRawText(Alu sub) {
    const auto op = comparisonOperatorForAlu(sub);
    if (!op.has_value()) {
        return "R1=(R1 ? R0)?-1:0";
    }
    return "R1=(R1" + *op + "R0)?-1:0";
}

} // namespace shenmue::scenario
