#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario::decompile {

inline constexpr size_t kNoBlock = SIZE_MAX;

enum class BlockExit : uint8_t { Fall, Goto, Branch, Return, Stop };

struct BasicBlock {
    std::string label;
    std::vector<SourceStmt> body;
    std::vector<SourceStmt> exit;
    BlockExit kind = BlockExit::Fall;
    SourceExpr value;
    bool folded = false;
    std::string targetLabel;
    size_t target = kNoBlock;

    bool near = false;
    std::vector<size_t> predecessors;
    size_t dominator = kNoBlock;
};

struct ControlFlowGraph {
    std::vector<BasicBlock> blocks;

    std::map<std::string, uint32_t> references;

    explicit ControlFlowGraph(std::vector<SourceStmt> statements);
    std::vector<size_t> successors(size_t index) const;
    bool dominates(size_t dominator, size_t index) const;

    void refold();
};

void splitFrameVariables(ControlFlowGraph &graph);

std::map<std::string, std::string> splitFrameRecords(ControlFlowGraph &graph);

void placeFrameVariables(SourceFunctionAst &function);

std::vector<SourceStmt> structureFunction(ControlFlowGraph graph);

} // namespace shenmue::scenario::decompile
