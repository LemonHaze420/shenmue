#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario {

std::vector<uint8_t> compileExplicitFunction(const SourceUnit &unit, const SourceFunctionAst &function);
std::vector<uint8_t> compileSourceFunctionBytes(const SourceUnit &unit, const std::string &functionName);
Scn3Module buildScn3ModuleFromSourceUnit(const SourceUnit &sourceUnit);

} // namespace shenmue::scenario
