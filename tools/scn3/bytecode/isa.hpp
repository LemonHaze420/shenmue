#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

namespace shenmue::scenario {

enum class Control : std::uint8_t {
#define SCN3_CONTROL(identifier, operation, mnemonic, name, operands) identifier = operation,
#include "scn3/bytecode/isa.def"
};

enum class Address : std::uint8_t {
#define SCN3_ADDRESS(identifier, operation, mnemonic, name) identifier = operation,
#include "scn3/bytecode/isa.def"
};

enum class Alu : std::uint8_t {
#define SCN3_ALU(identifier, operation, mnemonic, name, operands, results, binary) identifier = operation,
#include "scn3/bytecode/isa.def"
};

inline constexpr std::uint8_t kAddressClass = 0x40;
inline constexpr std::uint8_t kAluClass = 0x80;
inline constexpr std::uint8_t kReservedClass = 0xC0;

constexpr std::uint8_t encode(Control operation, std::uint8_t width = 0) {
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>(operation) | width << 4U);
}

constexpr std::uint8_t encode(Address operation, std::uint8_t width = 0) {
    return static_cast<std::uint8_t>(kAddressClass | static_cast<std::uint8_t>(operation) | width << 4U);
}

constexpr std::uint8_t encode(Alu operation) {
    return static_cast<std::uint8_t>(kAluClass | static_cast<std::uint8_t>(operation));
}

constexpr Control controlOperation(std::uint8_t opcode) {
    return static_cast<Control>(opcode & 0x0FU);
}

constexpr Address addressOperation(std::uint8_t opcode) {
    return static_cast<Address>(opcode & 0x0FU);
}

constexpr Alu aluOperation(std::uint8_t opcode) {
    return static_cast<Alu>(opcode & 0x3FU);
}

constexpr std::uint8_t immediateWidth(std::uint8_t opcode) {
    return (opcode >> 4U) & 3U;
}

enum class AddrBase : std::uint8_t {
    CurrBase,
    ScriptData,
    ScenarioWork,
    PrevSp,
};

inline constexpr std::uint8_t kPointerAccess = 3;

constexpr AddrBase addressBase(Address operation) {
    return static_cast<AddrBase>(static_cast<std::uint8_t>(operation) >> 2U);
}

constexpr std::uint8_t addressAccess(Address operation) {
    return static_cast<std::uint8_t>(operation) & 3U;
}

constexpr Address addressOperation(AddrBase base, std::uint8_t access) {
    return static_cast<Address>(static_cast<std::uint8_t>(base) << 2U | access);
}

constexpr bool nativeCall(Control operation) {
    return operation >= Control::CallLegacy && operation <= Control::CallDswk;
}

struct Operation {
    const char *mnemonic;
    const char *name;
    std::uint8_t opcode;
    std::uint8_t inputs;
    std::uint8_t results = 0;
    const char *binary = nullptr;
};

inline constexpr Operation kControlOperations[] = {
#define SCN3_CONTROL(identifier, operation, mnemonic, name, operands) {mnemonic, name, encode(Control::identifier), operands, 0},
#include "scn3/bytecode/isa.def"
};

inline constexpr Operation kAddressOperations[] = {
#define SCN3_ADDRESS(identifier, operation, mnemonic, name) {mnemonic, name, encode(Address::identifier), 0, 1},
#include "scn3/bytecode/isa.def"
};

inline constexpr Operation kAluOperations[] = {
#define SCN3_ALU(identifier, operation, mnemonic, name, operands, results, binary) {mnemonic, name, encode(Alu::identifier), operands, results, binary},
#include "scn3/bytecode/isa.def"
};

static_assert(std::size(kControlOperations) == 16 && std::size(kAddressOperations) == 16);

constexpr const Operation &operation(Control operation) {
    return kControlOperations[static_cast<std::uint8_t>(operation)];
}

constexpr const Operation &operation(Address operation) {
    return kAddressOperations[static_cast<std::uint8_t>(operation)];
}

constexpr const Operation *operationOf(std::uint8_t opcode) {
    if (opcode < kAddressClass)
        return &kControlOperations[opcode & 0x0FU];
    if (opcode < kAluClass)
        return &kAddressOperations[opcode & 0x0FU];
    const std::uint8_t alu = opcode & 0x3FU;
    return opcode < kReservedClass && alu < std::size(kAluOperations) ? &kAluOperations[alu] : nullptr;
}

constexpr std::optional<std::uint8_t> opcodeForMnemonic(std::string_view mnemonic) {
    for (const Operation *table : {kControlOperations, kAddressOperations})
        for (size_t index = 0; index < 16; ++index)
            if (mnemonic == table[index].mnemonic)
                return table[index].opcode;
    for (const Operation &op : kAluOperations)
        if (op.mnemonic && mnemonic == op.mnemonic)
            return op.opcode;
    return std::nullopt;
}

constexpr const Operation *operationNamed(std::string_view name) {
    for (const Operation &op : kControlOperations)
        if (name == op.name)
            return &op;
    for (const Operation &op : kAddressOperations)
        if (name == op.name)
            return &op;
    for (const Operation &op : kAluOperations)
        if (name == op.name)
            return &op;
    return nullptr;
}

} // namespace shenmue::scenario
