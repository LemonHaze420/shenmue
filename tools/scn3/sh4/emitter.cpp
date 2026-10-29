#include "scn3/sh4/emitter.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/common/util.hpp"

namespace shenmue::scenario::sh4 {

namespace {

constexpr uint16_t rr(uint16_t base, uint8_t m, uint8_t n) {
    return static_cast<uint16_t>(base | n << 8U | m << 4U);
}
constexpr uint16_t movImm(int32_t imm, uint8_t n) {
    return static_cast<uint16_t>(0xE000U | n << 8U | (static_cast<uint32_t>(imm) & 0xFFU));
}
constexpr uint16_t addImm(int32_t imm, uint8_t n) {
    return static_cast<uint16_t>(0x7000U | n << 8U | (static_cast<uint32_t>(imm) & 0xFFU));
}
constexpr uint16_t movReg(uint8_t m, uint8_t n) {
    return rr(0x6003, m, n);
}
constexpr uint16_t pushReg(uint8_t m) {
    return rr(0x2006, m, 13);
}
constexpr uint16_t popReg(uint8_t n) {
    return rr(0x6006, 13, n);
}
constexpr uint16_t loadDisp(uint8_t disp, uint8_t m, uint8_t n) {
    return static_cast<uint16_t>(0x5000U | n << 8U | m << 4U | disp / 4U);
}
constexpr uint16_t sizeBits(uint8_t size) {
    return size == 1 ? 0 : size == 2 ? 1 : 2;
}

constexpr uint16_t kNop = 0x0009;
constexpr uint16_t kRts = 0x000B;
constexpr uint16_t kBrafR1 = 0x0123;
constexpr uint16_t kBsrfR1 = 0x0103;
constexpr uint16_t kJsrR0 = 0x400B;
constexpr uint16_t kJsrR1 = 0x410B;
constexpr uint16_t kJsrR3 = 0x430B;
constexpr uint16_t kCmpZero = 0x8800;
constexpr uint16_t kStubWords[] = {
    0x2F86,
    0x2F96,
    0x2FA6,
    0x2FD6,
    0x2FE6,
    0x4F22,
    0x6853,
    0xC709,
    0x184D,
    0x186E,
    0x180F,
    0x5983,
    0x5A84,
    0x5160,
    0x5D61,
    0x5E62,
    0x410B,
    0x5053,
    0xE000,
    0x4F26,
    0x6EF6,
    0x6DF6,
    0x6AF6,
    0x69F6,
    0x000B,
    0x68F6,
    0x528E,
    0x012A,
    0x1203,
    0x1210,
    0x12D1,
    0x12E2,
    0xAFF1,
    0x6023,
};

} // namespace

std::vector<uint8_t> stubBytes() {
    std::vector<uint8_t> out;
    for (const uint16_t value : kStubWords) {
        out.push_back(static_cast<uint8_t>(value));
        out.push_back(static_cast<uint8_t>(value >> 8U));
    }
    return out;
}

void FunctionTranslator::word(uint16_t value) {
    items.push_back(Item{.kind = Item::Kind::Word, .word = value});
}

void FunctionTranslator::poolLoad(PoolKind kind, uint32_t value, uint8_t reg) {
    items.push_back(Item{.kind = Item::Kind::PoolLoad, .reg = reg, .pool = kind, .value = value, .opcode = opcode_});
}

void FunctionTranslator::branch(BranchKind kind, uint32_t id) {
    items.push_back(Item{.kind = Item::Kind::Branch, .branch = kind, .value = id});
}

void FunctionTranslator::label(uint32_t id) {
    items.push_back(Item{.kind = Item::Kind::Label, .value = id});
}

uint8_t FunctionTranslator::alloc() {
    for (uint8_t reg = 4; reg <= 7; ++reg)
        if (std::find(cache.begin(), cache.end(), reg) == cache.end())
            return reg;
    const uint8_t oldest = cache.front();
    cache.erase(cache.begin());
    word(pushReg(oldest));
    return oldest;
}

uint8_t FunctionTranslator::pop(int avoid) {
    if (!cache.empty()) {
        const uint8_t reg = cache.back();
        cache.pop_back();
        return reg;
    }
    for (uint8_t reg = 4; reg <= 7; ++reg)
        if (reg != avoid) {
            word(popReg(reg));
            return reg;
        }
    throw std::logic_error("no register to pop into");
}

void FunctionTranslator::push(uint8_t reg) {
    cache.push_back(reg);
}

void FunctionTranslator::flush() {
    for (const uint8_t reg : cache)
        word(pushReg(reg));
    cache.clear();
}

void FunctionTranslator::constant(int32_t value, uint8_t reg) {
    if (value >= -128 && value <= 127)
        word(movImm(value, reg));
    else
        poolLoad(PoolKind::Literal, static_cast<uint32_t>(value), reg);
}

void FunctionTranslator::addImmediate(int32_t value, uint8_t reg) {
    if (value < -256 || value > 254) {
        poolLoad(PoolKind::Literal, static_cast<uint32_t>(value), 0);
        word(rr(value < 0 ? 0x3008 : 0x300C, 0, reg));
        return;
    }
    if (value < -128 || value > 127) {
        const int32_t step = value < 0 ? -128 : 127;
        word(addImm(step, reg));
        value -= step;
    }
    word(addImm(value, reg));
}

void FunctionTranslator::load(uint8_t base, int32_t disp, uint8_t reg, uint8_t size, bool shortForm) {
    const int32_t limit = size == 1 ? 15 : size == 2 ? 30 : 60;
    if (shortForm && disp >= 0 && disp <= limit && disp % size == 0) {
        if (size == 4) {
            word(loadDisp(static_cast<uint8_t>(disp), base, reg));
        } else {
            word(static_cast<uint16_t>((size == 1 ? 0x8400U : 0x8500U) | base << 4U | static_cast<uint32_t>(disp) / size));
            word(movReg(0, reg));
        }
        return;
    }
    constant(disp, 0);
    word(rr(static_cast<uint16_t>(0x000C | sizeBits(size)), base, reg));
}

void FunctionTranslator::binary(std::initializer_list<uint16_t> words) {
    const uint8_t b = pop();
    const uint8_t a = pop(b);

    const auto map = [&](uint32_t r) { return r == 1 ? a : r == 2 ? b : static_cast<uint8_t>(r); };
    for (const uint16_t value : words)
        word(static_cast<uint16_t>((value & 0xF00FU) | map((value >> 8U) & 0xFU) << 8U | map((value >> 4U) & 0xFU) << 4U));
    push(a);
}

void FunctionTranslator::floatOperands(uint8_t b, uint8_t a) {
    word(static_cast<uint16_t>(0x405AU | b << 8U));
    word(0xF30D);
    word(static_cast<uint16_t>(0x405AU | a << 8U));
    word(0xF20D);
}

void FunctionTranslator::unsupported(const VmInsn &insn) const {
    throw std::runtime_error("No native translation for VM opcode " + hexUnsigned(insn.opcode, 2) + " at " + hexUnsigned(insn.offset));
}

void FunctionTranslator::prologue() {
    word(pushReg(14));
    word(0x4D22);
    addImmediate(-frame_, 13);
    word(movReg(13, 14));
}

void FunctionTranslator::translate(const VmInsn &insn) {
    const uint8_t opcode = insn.opcode;
    opcode_ = opcode;
    const int32_t v = insn.operand;
    if ((opcode & 0xC0U) == kAddressClass) {
        const Address mode = addressOperation(opcode);
        const bool shortForm = immediateWidth(opcode) == 1;
        const uint8_t r = alloc();
        switch (mode) {
        case Address::Literal:
            constant(v, r);
            break;
        case Address::Relative3:
            if (insn.target < dataStart_) {
                poolLoad(PoolKind::Code, insn.target, 1);
                word(loadDisp(0, 8, r));
            } else {
                word(loadDisp(4, 8, r));
                constant(static_cast<int32_t>(insn.target - dataStart_), 1);
            }
            word(rr(0x300C, 1, r));
            break;
        case Address::Task8:
        case Address::Task16:
        case Address::Task32:
            load(9, v, r, mode == Address::Task8 ? 1 : mode == Address::Task16 ? 2 : 4, shortForm);
            break;
        case Address::TaskAddress:
            constant(v, r);
            word(rr(0x300C, 9, r));
            break;
        case Address::Local8:
        case Address::Local16:
        case Address::Local32:
            load(14, v + frame_, r, mode == Address::Local8 ? 1 : mode == Address::Local16 ? 2 : 4, true);
            break;
        case Address::LocalAddress:
            constant(v + frame_, r);
            word(rr(0x300C, 14, r));
            break;
        default:
            unsupported(insn);
        }
        push(r);
        return;
    }
    if ((opcode & 0xC0U) == kAluClass) {
        const Alu alu = aluOperation(opcode);
        switch (alu) {
        case Alu::PopEax: {
            const uint8_t r = pop();
            word(movReg(r, 0));
            return;
        }
        case Alu::PushEax: {
            const uint8_t r = alloc();
            word(movReg(0, r));
            push(r);
            return;
        }
        case Alu::Load8:
        case Alu::Load16:
        case Alu::Load32: {
            const uint8_t a = pop();
            word(rr(static_cast<uint16_t>(0x6000 | sizeBits(alu == Alu::Load8 ? 1 : alu == Alu::Load16 ? 2 : 4)), a, a));
            push(a);
            return;
        }
        case Alu::Store8:
        case Alu::Store16:
        case Alu::Store32: {
            const uint8_t value = pop();
            const uint8_t address = pop(value);
            word(rr(static_cast<uint16_t>(0x2000 | sizeBits(alu == Alu::Store8 ? 1 : alu == Alu::Store16 ? 2 : 4)), value, address));
            push(value);
            return;
        }

        case Alu::Equal:
            return binary({0x3120, 0x311A});
        case Alu::NotEqual:
            return binary({0x3120, 0x311A, 0x6117});
        case Alu::Less:
            return binary({0x3217, 0x311A});
        case Alu::LessEqual:
            return binary({0x3213, 0x311A});
        case Alu::Greater:
            return binary({0x3127, 0x311A});
        case Alu::GreaterEqual:
            return binary({0x3123, 0x311A});
        case Alu::Not: {
            const uint8_t a = pop();
            word(rr(0x6007, a, a));
            push(a);
            return;
        }
        case Alu::And:
            return binary({0x2129});
        case Alu::Or:
            return binary({0x212B});
        case Alu::Xor:
            return binary({0x212A});
        case Alu::Add:
            return binary({0x312C});
        case Alu::Subtract:
            return binary({0x3128});
        case Alu::Multiply: {
            const uint8_t b = pop();
            const uint8_t a = pop(b);
            word(rr(0x0007, b, a));
            word(static_cast<uint16_t>(0x001AU | a << 8U));
            push(a);
            return;
        }
        case Alu::Divide:
        case Alu::Modulo: {
            const uint8_t b = pop();
            const uint8_t a = pop(b);
            word(loadDisp(alu == Alu::Divide ? 20 : 24, 8, 3));
            word(movReg(b, 0));
            word(kJsrR3);
            word(movReg(a, 1));
            word(movReg(0, a));
            push(a);
            return;
        }
        case Alu::ShiftLeft:
            return binary({0x412C});
        case Alu::ShiftRight:
            return binary({0x622B, 0x412C});
        case Alu::IntToFloat:
        case Alu::FloatToInt: {
            const uint8_t a = pop();
            word(static_cast<uint16_t>(0x405AU | a << 8U));
            if (alu == Alu::IntToFloat) {
                word(0xF22D);
            } else {
                word(0xF20D);
                word(0xF23D);
            }
            if (alu == Alu::IntToFloat)
                word(0xF21D);
            word(static_cast<uint16_t>(0x005AU | a << 8U));
            push(a);
            return;
        }
        case Alu::FloatLess:
        case Alu::FloatLessEqual:
        case Alu::FloatGreater:
        case Alu::FloatGreaterEqual: {
            const uint8_t b = pop();
            const uint8_t a = pop(b);
            floatOperands(b, a);
            word(alu == Alu::FloatLess || alu == Alu::FloatGreaterEqual ? 0xF325 : 0xF235);
            word(rr(0x300A, a, a));
            if (alu == Alu::FloatLessEqual || alu == Alu::FloatGreaterEqual)
                word(rr(0x6007, a, a));
            push(a);
            return;
        }
        case Alu::FloatAdd:
        case Alu::FloatSubtract:
        case Alu::FloatMultiply:
        case Alu::FloatDivide: {
            const uint8_t b = pop();
            const uint8_t a = pop(b);
            floatOperands(b, a);
            word(static_cast<uint16_t>(0xF230U | (static_cast<uint8_t>(alu) - static_cast<uint8_t>(Alu::FloatAdd))));
            word(0xF21D);
            word(static_cast<uint16_t>(0x005AU | a << 8U));
            push(a);
            return;
        }
        default:
            unsupported(insn);
        }
    }
    if (opcode == encode(Control::Return)) {
        addImmediate(frame_, 13);
        word(0x4D26);
        word(kRts);
        word(popReg(14));
        items.push_back(Item{.kind = Item::Kind::Pool});
        return;
    }
    switch (controlOperation(opcode)) {
    case Control::AdjustStack:
        addImmediate(v, 13);
        return;
    case Control::BranchFalse: {
        const uint32_t skip = nextLabel_++;
        word(kCmpZero);
        branch(BranchKind::False, skip);
        poolLoad(PoolKind::Jump, insn.target, 1);
        word(kBrafR1);
        word(kNop);
        items.push_back(Item{.kind = Item::Kind::Pool});
        label(skip);
        return;
    }
    case Control::Jump:
        poolLoad(PoolKind::Jump, insn.target, 1);
        word(kBrafR1);
        word(kNop);
        items.push_back(Item{.kind = Item::Kind::Pool});
        return;
    case Control::LocalCall:
        flush();
        poolLoad(PoolKind::Call, insn.target, 1);
        word(kBsrfR1);
        word(kNop);
        return;
    case Control::CallShenmue:
    case Control::CallDswk:
        flush();
        constant(v, 5);
        word(loadDisp(controlOperation(opcode) == Control::CallShenmue ? 40 : 48, 8, 0));
        word(loadDisp(52, 8, 4));
        word(kJsrR0);
        word(movReg(13, 6));
        return;
    case Control::ScnfOffset:
        flush();
        word(loadDisp(4, 8, 5));
        word(loadDisp(28, 8, 0));
        constant(static_cast<int32_t>(insn.target - dataStart_), 1);
        word(loadDisp(52, 8, 4));
        word(kJsrR0);
        word(rr(0x300C, 1, 5));
        word(loadDisp(60, 8, 1));
        word(kJsrR1);
        word(kNop);
        return;
    case Control::LatchOperand:
        latch_ = v;
        return;
    case Control::CallScnf: {
        if (!latch_)
            throw std::runtime_error("SCNF call without a latched operand at " + hexUnsigned(insn.offset));
        flush();
        constant(v, 5);
        word(loadDisp(44, 8, 0));
        constant(*latch_, 7);
        word(loadDisp(52, 8, 4));
        word(kJsrR0);
        word(movReg(13, 6));
        const uint32_t resume = nextLabel_++;
        word(0x6009);
        word(0xC801);
        branch(BranchKind::False, resume);
        word(loadDisp(60, 8, 1));
        word(kJsrR1);
        word(kNop);
        label(resume);
        word(loadDisp(56, 8, 0));
        word(loadDisp(12, 0, 0));
        latch_.reset();
        return;
    }
    default:
        unsupported(insn);
    }
}

namespace {

struct PoolKey {
    PoolKind kind;
    uint32_t value;
    uint32_t site;
    bool operator==(const PoolKey &) const = default;
};

struct Placed {
    enum class Kind : uint8_t {
        Word,
        Load,
        Branch,
        Pad,
        Entry,
    };
    Kind kind;
    uint32_t pc;
    uint16_t word = 0;
    uint8_t reg = 0;
    BranchKind branch = BranchKind::False;
    uint64_t label = 0;
    PoolKey key{};
    uint32_t entry = 0;
};

constexpr uint64_t vmLabel(uint32_t offset) {
    return uint64_t{1} << 32U | offset;
}

constexpr bool flushesAfter(uint8_t opcode) {
    return opcode == 0x1B || opcode == 0x2B || opcode == 0x31 || opcode == 0x1C || opcode == 0x1D || opcode == 0x38 || opcode == 0x80;
}

constexpr int32_t referenceOrigin(const Item &item) {
    if (item.pool == PoolKind::Call)
        return 4;
    const bool addressPush = (item.opcode & 0xC0U) == kAddressClass &&
        (addressOperation(item.opcode) == Address::TaskAddress || addressOperation(item.opcode) == Address::LocalAddress);
    const bool callOperand = item.opcode == encode(Control::CallShenmue, 1) || item.opcode == encode(Control::CallShenmue, 2) ||
        item.opcode == encode(Control::CallScnf, 1) || item.opcode == encode(Control::CallDswk, 1) || item.opcode == encode(Control::ScnfOffset, 3);
    return item.pool != PoolKind::Code && (addressPush || callOperand) ? -2 : 0;
}

struct Layout {
    std::vector<Placed> placed;
    std::map<uint64_t, uint32_t> labels;
    uint32_t end = 0;
};

Layout place(const std::vector<Item> &items, uint32_t start) {
    Layout out;
    std::vector<std::pair<PoolKey, uint32_t>> pending;
    std::vector<size_t> waiting;
    uint32_t pc = start;
    uint64_t nextLabel = uint64_t{1} << 40U;
    uint8_t previous = 0xFF;
    uint32_t loads = 0;
    const auto flush = [&](bool forced) {
        if (pending.empty())
            return;
        uint64_t over = 0;
        if (forced) {
            over = nextLabel++;
            out.placed.push_back(Placed{.kind = Placed::Kind::Branch, .pc = pc, .branch = BranchKind::Always, .label = over});
            out.placed.push_back(Placed{.kind = Placed::Kind::Word, .pc = pc + 2, .word = kNop});
            pc += 4;
        }
        if (pc % 4) {
            out.placed.push_back(Placed{.kind = Placed::Kind::Pad, .pc = pc});
            pc += 2;
        }
        std::vector<std::pair<PoolKey, uint32_t>> entries;
        for (const auto &[key, use] : pending) {
            entries.emplace_back(key, pc);
            out.placed.push_back(Placed{.kind = Placed::Kind::Entry, .pc = pc, .key = key});
            pc += 4;
        }
        for (const size_t index : waiting)
            for (const auto &[key, address] : entries)
                if (out.placed[index].key == key)
                    out.placed[index].entry = address;
        waiting.clear();
        pending.clear();
        loads = 0;
        if (forced)
            out.labels[over] = pc;
    };
    for (size_t i = 0; i < items.size(); ++i) {
        const Item &item = items[i];
        switch (item.kind) {
        case Item::Kind::Label:
            out.labels[item.value] = pc;
            break;
        case Item::Kind::Boundary: {
            const bool point = item.depth == 0 && flushesAfter(previous);
            if (!pending.empty() && point && pc - pending.front().second >= kForcedPoolDistance)
                flush(true);
            out.labels[vmLabel(item.value)] = pc;
            previous = item.opcode;
            break;
        }
        case Item::Kind::Pool:
            flush(false);
            break;
        case Item::Kind::Branch:
            if (item.branch == BranchKind::False && i + 5 < items.size() && items[i + 1].kind == Item::Kind::PoolLoad && items[i + 1].pool == PoolKind::Jump &&
                items[i + 4].kind == Item::Kind::Pool && items[i + 5].kind == Item::Kind::Label && items[i + 5].value == item.value) {
                const uint32_t count = static_cast<uint32_t>(pending.size()) + 1;
                const uint32_t pad = (pc + 8) % 4 ? 2 : 0;
                if ((8 + pad + 4 * count - 4) / 2 > 127 || loads >= kLongBranchPoolLoads) {
                    const uint64_t thunk = nextLabel++;
                    out.placed.push_back(Placed{.kind = Placed::Kind::Branch, .pc = pc, .branch = BranchKind::True, .label = thunk});
                    out.placed.push_back(Placed{.kind = Placed::Kind::Branch, .pc = pc + 2, .branch = BranchKind::Always, .label = item.value});
                    out.placed.push_back(Placed{.kind = Placed::Kind::Word, .pc = pc + 4, .word = kNop});
                    out.labels[thunk] = pc + 6;
                    pc += 6;
                    break;
                }
            }
            out.placed.push_back(Placed{.kind = Placed::Kind::Branch, .pc = pc, .branch = item.branch, .label = item.value});
            pc += 2;
            break;
        case Item::Kind::PoolLoad: {
            PoolKey key{.kind = item.pool, .value = item.value, .site = 0};
            if (item.pool == PoolKind::Jump || item.pool == PoolKind::Call)
                key.site = pc + 2;
            if (std::find_if(pending.begin(), pending.end(), [&](const auto &entry) { return entry.first == key; }) == pending.end())
                pending.emplace_back(key, pc + static_cast<uint32_t>(referenceOrigin(item)));

            loads += item.pool == PoolKind::Literal && item.reg == 1 ? 2 : 1;
            waiting.push_back(out.placed.size());
            out.placed.push_back(Placed{.kind = Placed::Kind::Load, .pc = pc, .reg = item.reg, .key = key});
            pc += 2;
            break;
        }
        case Item::Kind::Word:
            out.placed.push_back(Placed{.kind = Placed::Kind::Word, .pc = pc, .word = item.word});
            pc += 2;
            break;
        }
    }
    flush(false);
    out.end = pc;
    return out;
}

} // namespace

uint32_t functionSize(const std::vector<Item> &items, uint32_t start) {
    return place(items, start).end - start;
}

FunctionLayout layoutFunction(const std::vector<Item> &items, uint32_t start, const AddressOf &nativeFunction) {
    const Layout layout = place(items, start);
    FunctionLayout out{.start = start, .end = layout.end};
    const auto labelAt = [&](uint64_t id) {
        const auto found = layout.labels.find(id);
        if (found == layout.labels.end())
            throw std::runtime_error("Branch to a VM offset outside its function: " + hexUnsigned(static_cast<uint32_t>(id)));
        return found->second;
    };
    const auto put16 = [&](uint16_t value) {
        out.bytes.push_back(static_cast<uint8_t>(value));
        out.bytes.push_back(static_cast<uint8_t>(value >> 8U));
    };
    for (const Placed &p : layout.placed) {
        switch (p.kind) {
        case Placed::Kind::Word:
            put16(p.word);
            break;
        case Placed::Kind::Pad:
            put16(0);
            break;
        case Placed::Kind::Load: {
            const int64_t disp = (static_cast<int64_t>(p.entry) - static_cast<int64_t>((p.pc & ~3U) + 4)) / 4;
            if (disp < 0 || disp > 255)
                throw std::runtime_error("Pool entry out of reach at " + hexUnsigned(p.pc));
            put16(static_cast<uint16_t>(0xD000U | p.reg << 8U | static_cast<uint32_t>(disp)));
            break;
        }
        case Placed::Kind::Branch: {
            const uint32_t target = labelAt(p.label);
            const int64_t disp = (static_cast<int64_t>(target) - static_cast<int64_t>(p.pc + 4)) / 2;
            if (p.branch == BranchKind::Always) {
                if (disp < -2048 || disp > 2047)
                    throw std::runtime_error("bra out of reach at " + hexUnsigned(p.pc));
                put16(static_cast<uint16_t>(0xA000U | (static_cast<uint32_t>(disp) & 0xFFFU)));
            } else {
                if (disp < -128 || disp > 127)
                    throw std::runtime_error("Conditional branch out of reach at " + hexUnsigned(p.pc));
                put16(static_cast<uint16_t>((p.branch == BranchKind::True ? 0x8900U : 0x8B00U) | (static_cast<uint32_t>(disp) & 0xFFU)));
            }
            break;
        }
        case Placed::Kind::Entry: {
            uint32_t value = p.key.value;
            if (p.key.kind == PoolKind::Code)
                value = nativeFunction(p.key.value);
            else if (p.key.kind == PoolKind::Call)
                value = nativeFunction(p.key.value) - (p.key.site + 4);
            else if (p.key.kind == PoolKind::Jump)
                value = labelAt(vmLabel(p.key.value)) - (p.key.site + 4);
            for (int shift = 0; shift < 32; shift += 8)
                out.bytes.push_back(static_cast<uint8_t>(value >> shift));
            break;
        }
        }
    }
    return out;
}

} // namespace shenmue::scenario::sh4
