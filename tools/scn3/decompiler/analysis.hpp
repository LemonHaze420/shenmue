#pragma once

#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/value.hpp"

#include <cstdint>
#include <map>
#include <set>
#include <vector>

namespace shenmue::scenario {

struct Program;
struct FunctionInfo;

struct VmCallSite {
    std::uint32_t offset = 0;
    std::uint32_t target = 0;
    Control control = Control::Return;
    std::vector<VmValue> arguments;
};

struct VmProgramAnalysis {
    std::set<std::uint32_t> callbackTargets;
    std::map<std::uint32_t, std::vector<VmValue>> arguments;
};

std::vector<VmCallSite> analyzeVmCalls(const Program &program, const FunctionInfo &function);
VmProgramAnalysis analyzeVmProgram(const Program &program);

} // namespace shenmue::scenario
