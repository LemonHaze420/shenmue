#pragma once

#include "common/scenario_isa.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

namespace shenmue::scenario {

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
#include "common/scenario_isa.def"
};

inline constexpr Operation kAddressOperations[] = {
#define SCN3_ADDRESS(identifier, operation, mnemonic, name) {mnemonic, name, encode(Address::identifier), 0, 1},
#include "common/scenario_isa.def"
};

inline constexpr Operation kAluOperations[] = {
#define SCN3_ALU(identifier, operation, mnemonic, name, operands, results, binary) {mnemonic, name, encode(Alu::identifier), operands, results, binary},
#include "common/scenario_isa.def"
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
