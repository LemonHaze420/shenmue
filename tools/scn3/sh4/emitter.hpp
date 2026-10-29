#pragma once

#include "scn3/common/std.hpp"
#include "scn3/bytecode/isa.hpp"

namespace shenmue::scenario::sh4 {

enum class PoolKind : uint8_t {
    Literal,
    Jump,
    Call,
    Code,
};

enum class BranchKind : uint8_t {
    False,
    True,
    Always,
};

struct Item {
    enum class Kind : uint8_t {
        Word,
        PoolLoad,
        Branch,
        Label,
        Pool,
        Boundary,
    };
    Kind kind = Kind::Word;
    uint16_t word = 0;
    uint8_t reg = 0;
    PoolKind pool = PoolKind::Literal;
    BranchKind branch = BranchKind::False;
    uint32_t value = 0;
    uint8_t depth = 0;
    uint8_t opcode = 0;
};

struct VmInsn {
    uint32_t offset = 0;
    uint8_t opcode = 0;
    int32_t operand = 0;
    uint32_t target = 0;
    uint8_t size = 1;
};

inline constexpr uint32_t kStubOffset = 0x30;
inline constexpr uint32_t kCodeOffset = 0x74;
inline constexpr uint32_t kNativeFlags = 0x20000;
inline constexpr uint32_t kVmFlags = 0x100;
inline constexpr uint32_t kCodeRelocation = 0x2402;
inline constexpr uint32_t kDataRelocation = 0x1402;

inline constexpr uint32_t kForcedPoolDistance = 904;

inline constexpr uint32_t kLongBranchPoolLoads = 59;

inline constexpr Alu kAluTranslated[] = {Alu::PopEax, Alu::Load8, Alu::Load16, Alu::Load32, Alu::Store8, Alu::Store16, Alu::Store32, Alu::Equal, Alu::NotEqual,
    Alu::Less, Alu::LessEqual, Alu::Greater, Alu::GreaterEqual, Alu::Not, Alu::And, Alu::Or, Alu::Xor, Alu::Add, Alu::Subtract, Alu::Multiply, Alu::Divide,
    Alu::Modulo, Alu::ShiftLeft, Alu::ShiftRight, Alu::IntToFloat, Alu::FloatToInt, Alu::FloatLess, Alu::FloatLessEqual, Alu::FloatGreater,
    Alu::FloatGreaterEqual, Alu::FloatAdd, Alu::FloatSubtract, Alu::FloatMultiply, Alu::FloatDivide};

class FunctionTranslator {
  public:
    FunctionTranslator(int32_t frame, uint32_t dataStart) : frame_(frame), dataStart_(dataStart) {
    }

    void prologue();
    void translate(const VmInsn &insn);

    std::vector<Item> items;
    std::vector<uint8_t> cache;

  private:
    void word(uint16_t value);
    void poolLoad(PoolKind kind, uint32_t value, uint8_t reg);
    void branch(BranchKind kind, uint32_t label);
    void label(uint32_t id);
    uint8_t alloc();
    uint8_t pop(int avoid = -1);
    void push(uint8_t reg);
    void flush();
    void constant(int32_t value, uint8_t reg);
    void addImmediate(int32_t value, uint8_t reg);
    void load(uint8_t base, int32_t disp, uint8_t reg, uint8_t size, bool shortForm);
    void binary(std::initializer_list<uint16_t> words);
    void floatOperands(uint8_t b, uint8_t a);
    [[noreturn]] void unsupported(const VmInsn &insn) const;

    int32_t frame_;
    uint32_t dataStart_;
    std::optional<int32_t> latch_;
    uint32_t nextLabel_ = 0;
    uint8_t opcode_ = 0;
};

struct FunctionLayout {
    uint32_t start = 0;
    uint32_t end = 0;
    std::vector<uint8_t> bytes;
};

using AddressOf = std::function<uint32_t(uint32_t)>;

FunctionLayout layoutFunction(const std::vector<Item> &items, uint32_t start, const AddressOf &nativeFunction);
uint32_t functionSize(const std::vector<Item> &items, uint32_t start);
std::vector<uint8_t> stubBytes();

} // namespace shenmue::scenario::sh4
