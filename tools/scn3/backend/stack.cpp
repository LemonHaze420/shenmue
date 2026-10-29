#include "scn3/backend/object.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"

#include <iostream>
#include <set>

namespace shenmue::scenario {

void validateCStackDepth(const SourceUnit &unit, const std::map<std::string, std::vector<uint8_t>> &code) {
    struct Call {
        std::string target;
        int32_t depth;
    };
    struct Summary {
        int32_t yield = -1;
        bool indirect = false;
        std::vector<Call> calls;
    };
    std::map<uint32_t, std::string> targets;
    std::map<std::string, Summary> summaries;
    for (const std::pair<const std::string, SourceFunctionAst> &item : unit.functions)
        if (item.second.semantics == SourceSemantics::C11)
            targets[item.second.offset] = item.first;
    for (const std::pair<const std::string, std::vector<uint8_t>> &item : code) {
        if (unit.functions.at(item.first).semantics != SourceSemantics::C11)
            continue;
        const std::vector<uint8_t> &bytes = item.second;
        Summary &summary = summaries[item.first];
        std::map<uint32_t, Instruction> instructions;
        for (uint32_t offset = 0; offset < bytes.size();) {
            const Instruction instruction = decodeInstruction(bytes, offset, 0, static_cast<uint32_t>(bytes.size()));
            instructions[offset] = instruction;
            offset += instruction.size;
        }
        std::map<uint32_t, int32_t> depths{{0, 0}};
        std::vector<uint32_t> pending{0};
        const std::function<void(uint32_t, int32_t)> enqueue = [&](uint32_t offset, int32_t depth) {
            if (!instructions.count(offset))
                throw std::runtime_error("C control flow leaves function " + item.first);
            const std::pair<std::map<uint32_t, int32_t>::iterator, bool> inserted = depths.emplace(offset, depth);
            if (inserted.second)
                pending.push_back(offset);
            else if (inserted.first->second != depth)
                throw std::runtime_error("C control-flow stack mismatch in " + item.first);
        };
        while (!pending.empty()) {
            const uint32_t offset = pending.back();
            pending.pop_back();
            const Instruction &instruction = instructions.at(offset);
            int32_t depth = depths.at(offset);
            const uint8_t op = instruction.opcode;
            const Control control = controlOperation(op);
            const uint32_t next = offset + instruction.size;
            if (op >= kAddressClass) {
                if (const Operation *operation = operationOf(op))
                    depth += operation->results - operation->inputs;
            } else {
                if (control == Control::Return)
                    continue;
                if (control == Control::AdjustStack) {
                    const uint32_t immediate = instruction.imm.value_or(0);
                    const int32_t signedValue = instruction.immWidth == 1 ? static_cast<int8_t>(immediate) :
                        instruction.immWidth == 2                         ? static_cast<int16_t>(immediate) :
                                                                            static_cast<int32_t>(immediate);
                    depth -= signedValue / 4;
                } else if (control == Control::LocalCall) {
                    const std::map<uint32_t, std::string>::const_iterator target = targets.find(instruction.localTarget);
                    if (target == targets.end())
                        throw std::runtime_error("C call has no known script target in " + item.first);
                    summary.calls.push_back({target->second, depth + 2});
                } else if (control == Control::IndirectCall) {
                    summary.indirect = true;
                    --depth;
                } else if (control == Control::ScnfOffset || control == Control::CallScnf) {
                    int32_t captured = depth;
                    if (control == Control::CallScnf && next + 1 < bytes.size() && bytes[next] == encode(Control::AdjustStack, 1))
                        captured -= bytes[next + 1] >> 2;
                    summary.yield = std::max(summary.yield, captured);
                }
                if (control == Control::LocalJump || control == Control::Jump) {
                    if (control == Control::LocalJump && targets.count(instruction.localTarget) && targets.at(instruction.localTarget) != item.first)
                        summary.calls.push_back({targets.at(instruction.localTarget), depth});
                    else
                        enqueue(control == Control::Jump ? instruction.relativeTarget : instruction.localTarget - unit.functions.at(item.first).offset, depth);
                    continue;
                }
                if (control == Control::BranchFalse)
                    enqueue(instruction.relativeTarget, depth);
            }
            if (depth < 0)
                throw std::runtime_error("C evaluation stack underflow in " + item.first);
            if (next < bytes.size())
                enqueue(next, depth);
        }
    }
    std::map<std::string, bool> mayYield;
    for (const std::pair<const std::string, Summary> &item : summaries)
        mayYield[item.first] = item.second.yield >= 0;
    bool changed = true;
    while (changed) {
        changed = false;
        for (const std::pair<const std::string, Summary> &item : summaries)
            for (const Call &call : item.second.calls)
                if (mayYield[call.target] && !mayYield[item.first]) {
                    mayYield[item.first] = true;
                    changed = true;
                }
    }
    bool anyYield = false;
    for (const std::pair<const std::string, bool> &item : mayYield)
        anyYield |= item.second;
    std::map<std::string, int32_t> maximum;
    std::set<std::string> active;

    const std::function<int32_t(const std::string &)> calculate = [&](const std::string &name) -> int32_t {
        if (maximum.count(name))
            return maximum.at(name);
        if (!active.insert(name).second)
            return summaries.at(name).yield;
        const Summary &summary = summaries.at(name);
        if (summary.indirect && anyYield)
            throw std::runtime_error("Indirect C calls require non-yielding targets in " + name);
        int32_t result = summary.yield;
        for (const Call &call : summary.calls)
            if (mayYield[call.target])
                result = std::max(result, call.depth + calculate(call.target));
        active.erase(name);
        maximum[name] = result;
        return result;
    };

    static std::set<std::string> reported;
    for (const std::pair<const std::string, Summary> &item : summaries) {
        if (!mayYield[item.first] && !item.second.indirect)
            continue;
        const int32_t depth = calculate(item.first);
        const uint32_t incoming = static_cast<uint32_t>(unit.functions.at(item.first).params.size()) + 2U;
        if (item.second.yield >= 0 && static_cast<uint32_t>(item.second.yield) + incoming > 128U)
            throw std::runtime_error("C thread exceeds the native 128-word saved stack at a yield in " + item.first);
        if (depth >= 0 && static_cast<uint32_t>(depth) + incoming > 128U && reported.insert(item.first).second)
            std::cerr << "warning: calls from " << item.first << " may exceed the native 128-word saved stack at a yield\n";
    }
}

} // namespace shenmue::scenario
