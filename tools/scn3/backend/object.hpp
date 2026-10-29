#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario {

struct VmRelocation {
    uint32_t position;
    uint8_t width;
    uint8_t opcode;
    SourceExpr value;
};

struct VmFunctionObject {
    std::vector<uint8_t> bytes;
    std::vector<VmRelocation> relocations;
    std::map<std::string, uint32_t> labels;
};

VmFunctionObject lowerVmFunction(const SourceUnit &unit, const SourceFunctionAst &function);
std::vector<uint8_t> linkVmFunction(const SourceUnit &unit, const SourceFunctionAst &function, VmFunctionObject object);

void validateCStackDepth(const SourceUnit &unit, const std::map<std::string, std::vector<uint8_t>> &code);

} // namespace shenmue::scenario
