#pragma once

#include <cstdint>

namespace shenmue::scenario {

enum class Control : std::uint8_t {
#define SCN3_CONTROL(identifier, operation, mnemonic, name, operands) identifier = operation,
#include "common/scenario_isa.def"
};

enum class Address : std::uint8_t {
#define SCN3_ADDRESS(identifier, operation, mnemonic, name) identifier = operation,
#include "common/scenario_isa.def"
};

enum class Alu : std::uint8_t {
#define SCN3_ALU(identifier, operation, mnemonic, name, operands, results, binary) identifier = operation,
#include "common/scenario_isa.def"
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

constexpr std::uint8_t opcodeClass(std::uint8_t opcode) {
    return static_cast<std::uint8_t>(opcode & 0xc0U);
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

} // namespace shenmue::scenario
