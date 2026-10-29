#include "scn3/sh4/emitter.hpp"
#include "scn3/sh4/native.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/common/util.hpp"

namespace shenmue::scenario {

using namespace sh4;

namespace {

struct Token {
    enum class Kind : uint8_t {
        Word,
        Load,
        Branch,
    };
    Kind kind = Kind::Word;
    uint32_t pc = 0;
    uint16_t word = 0;
    uint8_t reg = 0;
    PoolKind pool = PoolKind::Literal;
    uint32_t value = 0;
    BranchKind branch = BranchKind::False;
};

std::vector<Token> tokenize(const std::vector<uint8_t> &native, uint32_t start, uint32_t end) {
    std::vector<Token> out;
    std::set<uint32_t> pool;
    uint32_t skipTo = 0;
    for (uint32_t pc = start; pc + 2 <= end; pc += 2) {
        if (pc < skipTo || pool.count(pc) || (pc % 4 == 2 && pool.count(pc - 2)))
            continue;
        const uint16_t w = readU16(native, pc);
        if (pc % 4 == 2 && w == 0 && pool.count(pc + 2))
            continue;
        if ((w & 0xF000U) == 0xD000U) {
            const uint32_t entry = (pc & ~3U) + 4 + (w & 0xFFU) * 4;
            if (entry + 4 > native.size())
                throw std::runtime_error("Pool load outside the module at " + hexUnsigned(pc));
            pool.insert(entry);
            const uint32_t value = readU32(native, entry);
            const uint16_t next = pc + 4 <= end ? readU16(native, pc + 2) : 0;
            Token token{.kind = Token::Kind::Load, .pc = pc, .reg = static_cast<uint8_t>((w >> 8U) & 0xFU), .value = value};
            if (next == 0x0123 || next == 0x0103) {
                token.pool = next == 0x0123 ? PoolKind::Jump : PoolKind::Call;
                token.value = pc + 2 + 4 + value;
            }
            out.push_back(token);
        } else if ((w & 0xFD00U) == 0x8900U) {
            out.push_back(Token{.kind = Token::Kind::Branch,
                .pc = pc,
                .value = pc + 4 + static_cast<uint32_t>(static_cast<int8_t>(w & 0xFFU) * 2),
                .branch = (w & 0x0200U) ? BranchKind::False : BranchKind::True});
        } else if ((w & 0xF000U) == 0xA000U) {
            const int32_t disp = static_cast<int32_t>((w & 0xFFFU) ^ 0x800U) - 0x800;
            const uint32_t target = pc + 4 + static_cast<uint32_t>(disp * 2);
            if (!out.empty() && out.back().kind == Token::Kind::Branch && out.back().branch == BranchKind::True && out.back().value == pc + 4) {
                out.back().branch = BranchKind::False;
                skipTo = pc + 4;
            } else {
                skipTo = target;
            }
        } else {
            out.push_back(Token{.kind = Token::Kind::Word, .pc = pc, .word = w});
        }
    }
    return out;
}

bool same(const Item &item, const Token &token) {
    switch (item.kind) {
    case Item::Kind::Word:
        return token.kind == Token::Kind::Word && token.word == item.word;
    case Item::Kind::Branch:
        return token.kind == Token::Kind::Branch && token.branch == item.branch;
    case Item::Kind::PoolLoad:
        if (token.kind != Token::Kind::Load || token.reg != item.reg)
            return false;
        if (item.pool == PoolKind::Code || item.pool == PoolKind::Literal)
            return token.pool == PoolKind::Literal && token.value == item.value;
        return token.pool == item.pool && token.value == item.value;
    default:
        return true;
    }
}

int32_t immediate8(uint16_t w) {
    return static_cast<int8_t>(w & 0xFFU);
}

std::optional<int32_t> constantInto(const Token &token, uint8_t reg) {
    if (token.kind == Token::Kind::Word && (token.word & 0xFF00U) == (0xE000U | reg << 8U))
        return immediate8(token.word);
    if (token.kind == Token::Kind::Load && token.pool == PoolKind::Literal && token.reg == reg)
        return static_cast<int32_t>(token.value);
    return std::nullopt;
}

uint8_t minimalWidth(int32_t value) {
    return value >= -128 && value <= 127 ? 1 : value >= -32768 && value <= 32767 ? 2 : 3;
}

struct Candidate {
    uint8_t opcode;
    int32_t operand = 0;
    uint32_t target = 0;
    std::optional<int32_t> latch;
};

std::vector<Candidate> candidates(const FunctionTranslator &state, const std::vector<Token> &tokens, size_t at, int32_t frame, uint32_t dataStart) {
    std::vector<Candidate> out;
    const auto token = [&](size_t i) -> const Token * { return at + i < tokens.size() ? &tokens[at + i] : nullptr; };
    const auto wordAt = [&](size_t i) -> std::optional<uint16_t> {
        const Token *t = token(i);
        return t && t->kind == Token::Kind::Word ? std::optional<uint16_t>(t->word) : std::nullopt;
    };
    size_t p = 0;
    while (const auto w = wordAt(p)) {
        const uint8_t reg = (*w >> 4U) & 0xFU;
        if ((*w & 0xFF0FU) != 0x2D06U || std::find(state.cache.begin(), state.cache.end(), reg) == state.cache.end())
            break;
        ++p;
    }
    const auto addPush = [&](Address base, int32_t operand, bool fixedWide) {
        const uint8_t width = fixedWide ? 2 : minimalWidth(operand);
        out.push_back(Candidate{.opcode = encode(base, width), .operand = operand});
        if (width == 1)
            out.push_back(Candidate{.opcode = encode(base, 2), .operand = operand});
    };
    if (const Token *t = token(p)) {
        std::optional<int32_t> literal;
        if (t->kind == Token::Kind::Word && (t->word & 0xF000U) == 0xE000U)
            literal = immediate8(t->word);
        else if (t->kind == Token::Kind::Load && t->pool == PoolKind::Literal)
            literal = static_cast<int32_t>(t->value);
        const auto next = wordAt(p + 1);
        if (literal) {
            if (next && (*next & 0xF0FFU) == 0x30ECU)
                addPush(Address::LocalAddress, *literal - frame, false);
            if (next && (*next & 0xF0FFU) == 0x309CU)
                out.push_back(Candidate{.opcode = encode(Address::TaskAddress, 2), .operand = *literal});
            if (next && (*next & 0xF00CU) == 0x000CU && ((*next >> 4U) & 0xFU) == 14)
                addPush(addressOperation(AddrBase::PrevSp, *next & 3U), *literal - frame, false);
            if (next && (*next & 0xF00CU) == 0x000CU && ((*next >> 4U) & 0xFU) == 9) {
                out.push_back(Candidate{.opcode = encode(addressOperation(AddrBase::ScenarioWork, *next & 3U), 2), .operand = *literal});
                out.push_back(Candidate{.opcode = encode(addressOperation(AddrBase::ScenarioWork, *next & 3U), 1), .operand = *literal});
            }
            out.push_back(Candidate{.opcode = encode(Address::Literal, minimalWidth(*literal)), .operand = *literal});
        }
        if (t->kind == Token::Kind::Word && (t->word & 0xF000U) == 0x5000U) {
            const uint8_t base = (t->word >> 4U) & 0xFU;
            const int32_t disp = (t->word & 0xFU) * 4;
            if (base == 14)
                addPush(Address::Local32, disp - frame, false);
            if (base == 9)
                out.push_back(Candidate{.opcode = encode(Address::Task32, 1), .operand = disp});
            if (base == 8 && disp == 4)
                if (const Token *c = token(p + 1))
                    if (const auto offset = constantInto(*c, 1))
                        out.push_back(Candidate{.opcode = encode(Address::Relative3, 3), .target = dataStart + static_cast<uint32_t>(*offset)});
        }
        if (t->kind == Token::Kind::Word && (t->word & 0xFE00U) == 0x8400U) {
            const uint8_t base = (t->word >> 4U) & 0xFU;
            const uint8_t size = (t->word & 0x0100U) ? 2 : 1;
            const int32_t disp = (t->word & 0xFU) * size;
            const uint8_t access = size == 1 ? 0 : 1;
            if (base == 14)
                addPush(addressOperation(AddrBase::PrevSp, access), disp - frame, false);
            if (base == 9)
                out.push_back(Candidate{.opcode = encode(addressOperation(AddrBase::ScenarioWork, access), 1), .operand = disp});
        }
        if (t->kind == Token::Kind::Load && t->pool == PoolKind::Literal && t->reg == 1 && next && (*next & 0xF0FFU) == 0x5080U)
            out.push_back(Candidate{.opcode = encode(Address::Relative3, 3), .target = t->value});
        if (t->kind == Token::Kind::Word && t->word == 0x5581U && next && *next == 0x5087U)
            if (const Token *c = token(p + 2))
                if (const auto offset = constantInto(*c, 1))
                    out.push_back(Candidate{.opcode = encode(Control::ScnfOffset, 3), .target = dataStart + static_cast<uint32_t>(*offset)});
    }
    out.push_back(Candidate{.opcode = encode(Alu::PushEax)});
    for (const Alu alu : kAluTranslated)
        out.push_back(Candidate{.opcode = encode(alu)});
    for (size_t i = 0; i < 4; ++i)
        if (wordAt(i) == std::optional<uint16_t>(0x4D26))
            out.push_back(Candidate{.opcode = encode(Control::Return)});
    std::vector<int32_t> adds;
    for (size_t i = 0; i < 2; ++i) {
        const auto w = wordAt(i);
        if (!w || (*w & 0xFF00U) != 0x7D00U)
            break;
        adds.push_back(immediate8(*w));
    }
    if (!adds.empty())
        out.push_back(Candidate{.opcode = encode(Control::AdjustStack, 1), .operand = adds[0]});
    if (adds.size() == 2 && (adds[0] == -128 || adds[0] == 127))
        out.push_back(Candidate{.opcode = encode(Control::AdjustStack, 2), .operand = adds[0] + adds[1]});
    if (const Token *t = token(0); t && t->kind == Token::Kind::Load && t->reg == 0 && t->pool == PoolKind::Literal)
        if (const auto w = wordAt(1); w && (*w == 0x3D08U || *w == 0x3D0CU))
            out.push_back(Candidate{.opcode = encode(Control::AdjustStack, 2), .operand = static_cast<int32_t>(t->value)});
    for (size_t i = 0; i < 6; ++i) {
        const Token *t = token(i);
        if (!t || t->kind != Token::Kind::Load)
            continue;
        if (t->pool == PoolKind::Jump) {
            out.push_back(Candidate{.opcode = encode(Control::BranchFalse, 2), .target = t->value});
            out.push_back(Candidate{.opcode = encode(Control::Jump, 2), .target = t->value});
            break;
        }
        if (t->pool == PoolKind::Call) {
            out.push_back(Candidate{.opcode = encode(Control::LocalCall, 3), .operand = static_cast<int32_t>(t->value), .target = t->value});
            break;
        }
    }
    for (size_t i = 0; i + 1 < 8; ++i) {
        const Token *t = token(i);
        const auto next = wordAt(i + 1);
        if (!t || !next)
            continue;
        const auto slot = constantInto(*t, 5);
        if (!slot)
            continue;
        if (*next == 0x508AU)
            out.push_back(
                Candidate{.opcode = static_cast<uint8_t>(*slot >= -128 && *slot <= 127 ? encode(Control::CallShenmue, 1) : encode(Control::CallShenmue, 2)),
                    .operand = *slot});
        if (*next == 0x508CU)
            out.push_back(Candidate{.opcode = encode(Control::CallDswk, 1), .operand = *slot});
        if (*next == 0x508BU)
            if (const Token *l = token(i + 2))
                if (const auto latch = constantInto(*l, 7))
                    out.push_back(Candidate{.opcode = encode(Control::CallScnf, 1), .operand = *slot, .latch = *latch});
        break;
    }
    return out;
}

struct LiftedFunction {
    uint32_t start = 0;
    int32_t frame = 0;
    std::vector<VmInsn> code;
};

LiftedFunction liftFunction(const std::vector<Token> &tokens, size_t begin, size_t end, uint32_t dataStart) {
    const auto word = [&](size_t i) { return i < end && tokens[i].kind == Token::Kind::Word ? tokens[i].word : uint16_t{0}; };
    if (word(begin) != 0x2DE6 || word(begin + 1) != 0x4D22)
        throw std::runtime_error("Native function without a prologue at " + hexUnsigned(tokens[begin].pc));
    size_t i = begin + 2;
    int32_t frame = 0;
    if (i < end && tokens[i].kind == Token::Kind::Load && tokens[i].reg == 0 && word(i + 1) == 0x3D08U) {
        frame = -static_cast<int32_t>(tokens[i].value);
        i += 2;
    } else {
        while ((word(i) & 0xFF00U) == 0x7D00U) {
            frame -= immediate8(word(i));
            ++i;
        }
    }
    if (word(i) != 0x6ED3U)
        throw std::runtime_error("Native function prologue not understood at " + hexUnsigned(tokens[begin].pc));
    ++i;
    LiftedFunction out{.start = tokens[begin].pc, .frame = frame};
    out.code.push_back(VmInsn{.offset = tokens[begin].pc, .opcode = static_cast<uint8_t>(-frame >= -128 && -frame <= 127 ? 0x13 : 0x23), .operand = -frame});
    FunctionTranslator state(frame, dataStart);

    while (i < end) {
        std::optional<FunctionTranslator> best;
        std::vector<VmInsn> bestCode;
        size_t bestEnd = i;
        for (const Candidate &candidate : candidates(state, tokens, i, frame, dataStart)) {
            FunctionTranslator trial = state;
            trial.items.clear();
            std::vector<VmInsn> emitted;
            try {
                if (candidate.latch) {
                    const VmInsn latch{.offset = tokens[i].pc, .opcode = encode(Control::LatchOperand, 1), .operand = *candidate.latch};
                    trial.translate(latch);
                    emitted.push_back(latch);
                }
                const VmInsn insn{.offset = tokens[i].pc, .opcode = candidate.opcode, .operand = candidate.operand, .target = candidate.target};
                trial.translate(insn);
                emitted.push_back(insn);
            } catch (const std::exception &) {
                continue;
            }
            size_t at = i;
            bool ok = true;
            for (const Item &item : trial.items) {
                if (item.kind != Item::Kind::Word && item.kind != Item::Kind::PoolLoad && item.kind != Item::Kind::Branch)
                    continue;
                if (at >= end || !same(item, tokens[at])) {
                    ok = false;
                    break;
                }
                ++at;
            }
            if (!ok || at <= bestEnd)
                continue;
            trial.items.clear();
            best = std::move(trial);
            bestCode = std::move(emitted);
            bestEnd = at;
        }
        if (!best)
            throw std::runtime_error("No VM instruction translates to the native code at " + hexUnsigned(tokens[i].pc));
        out.code.insert(out.code.end(), bestCode.begin(), bestCode.end());
        state = std::move(*best);
        i = bestEnd;
    }
    return out;
}

uint8_t vmSize(uint8_t opcode) {
    if ((opcode & 0xC0U) == kAluClass)
        return 1;
    constexpr uint8_t bytes[] = {0, 1, 2, 4};
    return static_cast<uint8_t>(1 + bytes[immediateWidth(opcode)]);
}

void append(std::vector<uint8_t> &out, uint32_t value, uint8_t bytes) {
    for (uint8_t i = 0; i < bytes; ++i)
        out.push_back(static_cast<uint8_t>(value >> (8 * i)));
}

} // namespace

std::vector<uint8_t> liftScenarioFromSh4(const std::vector<uint8_t> &native) {
    if (!isNativeScenario(native))
        throw std::runtime_error("The module is not native SH-4 code");
    const ScenarioHeader input = parseHeader(native);
    const uint32_t startup = static_cast<uint32_t>(input.functionOffset);
    const uint32_t dataStart = static_cast<uint32_t>(input.dataSegmentOffset), dataEnd = static_cast<uint32_t>(input.dataEndOffset);
    const uint32_t workspace = static_cast<uint32_t>(input.workspaceSize);
    if (dataStart > dataEnd || dataEnd > native.size() || dataStart < kCodeOffset)
        throw std::runtime_error("Native module header offsets are invalid");
    const std::vector<uint8_t> stub = stubBytes();
    if (!std::equal(stub.begin(), stub.end(), native.begin() + kStubOffset))
        throw std::runtime_error("Native module does not carry the expected entry stub");

    const std::vector<Token> tokens = tokenize(native, kCodeOffset, dataStart);
    std::vector<size_t> starts;
    for (size_t i = 0; i + 1 < tokens.size(); ++i)
        if (tokens[i].kind == Token::Kind::Word && tokens[i].word == 0x2DE6 && tokens[i + 1].kind == Token::Kind::Word && tokens[i + 1].word == 0x4D22)
            starts.push_back(i);
    if (starts.empty() || tokens[starts.front()].pc != kCodeOffset)
        throw std::runtime_error("Native module does not open with a function");
    std::vector<LiftedFunction> functions;
    for (size_t f = 0; f < starts.size(); ++f)
        functions.push_back(liftFunction(tokens, starts[f], f + 1 < starts.size() ? starts[f + 1] : tokens.size(), dataStart));

    std::map<uint32_t, uint32_t> vmAt;
    uint32_t vm = kStubOffset;
    for (const LiftedFunction &function : functions)
        for (const VmInsn &insn : function.code) {
            if (!vmAt.count(insn.offset))
                vmAt[insn.offset] = vm;
            vm += vmSize(insn.opcode);
        }
    const uint32_t vmCodeEnd = vm;
    const uint32_t vmDataStart = (vmCodeEnd + 3) & ~3U;
    const auto vmOf = [&](uint32_t nativeAddress) {
        const auto found = vmAt.find(nativeAddress);
        if (found == vmAt.end())
            throw std::runtime_error("Native address without a VM instruction: " + hexUnsigned(nativeAddress));
        return found->second;
    };
    std::set<uint32_t> nativeFunctions;
    for (const LiftedFunction &function : functions)
        nativeFunctions.insert(function.start);

    std::vector<uint8_t> out(sizeof(ScenarioHeader), 0);
    std::vector<uint32_t> entries;
    uint32_t vmOffset = kStubOffset;
    for (const LiftedFunction &function : functions)
        for (const VmInsn &insn : function.code) {
            out.push_back(insn.opcode);
            const uint8_t size = vmSize(insn.opcode);
            uint32_t operand = static_cast<uint32_t>(insn.operand);
            if ((insn.opcode & 0xC0U) == 0 && insn.opcode != 0) {
                const Control control = controlOperation(insn.opcode);
                if (control == Control::Jump || control == Control::BranchFalse)
                    operand = vmOf(insn.target) - (vmOffset + size);
                else if (control == Control::LocalCall)
                    operand = vmOf(insn.target);
                else if (control == Control::ScnfOffset)
                    operand = vmDataStart + (insn.target - dataStart);
            } else if (insn.opcode == 0x73) {
                operand = insn.target < dataStart ? vmOf(insn.target) : vmDataStart + (insn.target - dataStart);
            }
            if ((insn.opcode == encode(Control::LocalCall, 3) || (insn.opcode == encode(Address::Relative3, 3) && insn.target < dataStart)) &&
                std::find(entries.begin(), entries.end(), insn.target) == entries.end())
                entries.push_back(insn.target);
            append(out, operand, static_cast<uint8_t>(size - 1));
            vmOffset += size;
        }

    out.resize(vmDataStart, 0xFF);
    const uint32_t tableStart = dataEnd - 4 * static_cast<uint32_t>(entries.size());
    if (tableStart < dataStart)
        throw std::runtime_error("Native data segment is smaller than its entry table");
    for (size_t i = 0; i < entries.size(); ++i)
        if (readU32(native, tableStart + 4 * static_cast<uint32_t>(i)) != entries[i])
            throw std::runtime_error("Native entry table does not list the referenced functions");
    std::vector<std::pair<uint32_t, uint32_t>> relocs;
    out.insert(out.end(), native.begin() + dataStart, native.begin() + tableStart);
    for (uint32_t site = (dataStart + 3) & ~3U; site + 4 <= tableStart; site += 4) {
        const uint32_t value = readU32(native, site);
        const uint32_t vmSite = vmDataStart + (site - dataStart);
        if (nativeFunctions.count(value)) {
            relocs.emplace_back(kCodeRelocation, vmSite);
            for (int b = 0; b < 4; ++b)
                out[vmSite + b] = static_cast<uint8_t>(vmOf(value) >> (8 * b));
        } else if (value >= dataStart && value < tableStart) {
            relocs.emplace_back(kDataRelocation, vmSite);
            const uint32_t moved = value - dataStart + vmDataStart;
            for (int b = 0; b < 4; ++b)
                out[vmSite + b] = static_cast<uint8_t>(moved >> (8 * b));
        }
    }
    const uint32_t vmDataEnd = static_cast<uint32_t>(out.size());
    for (const auto &[kind, site] : relocs) {
        append(out, kind, 4);
        append(out, site, 4);
    }
    append(out, 0, 4);
    append(out, 0, 4);
    const auto header = [&](uint32_t offset, uint32_t value) {
        for (int b = 0; b < 4; ++b)
            out[offset + b] = static_cast<uint8_t>(value >> (8 * b));
    };
    header(offsetof(ScenarioHeader, token), SHEN_NAME(SCN3));
    header(offsetof(ScenarioHeader, wholeSize), static_cast<uint32_t>(out.size()));
    header(offsetof(ScenarioHeader, flags), kVmFlags);
    header(offsetof(ScenarioHeader, functionOffset), vmOf(startup));
    header(offsetof(ScenarioHeader, dataSegmentOffset), vmDataStart);
    header(offsetof(ScenarioHeader, dataEndOffset), vmDataEnd);
    header(offsetof(ScenarioHeader, workspaceSize), workspace);
    header(offsetof(ScenarioHeader, entrypoint), kStubOffset);
    header(offsetof(ScenarioHeader, relocationTableOffset), vmDataEnd);
    return out;
}

} // namespace shenmue::scenario
