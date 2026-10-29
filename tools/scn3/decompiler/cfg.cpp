#include "scn3/decompiler/cfg.hpp"
#include "scn3/decompiler/passes.hpp"

namespace shenmue::scenario::decompile {

namespace {

bool relativeJump(const SourceStmt &stmt) {
    return stmt.kind == SourceStmtKind::ExprStmt && stmt.expr.kind == SourceExprKind::Call && stmt.expr.text == "vm_goto_rel" && stmt.expr.args.size() == 1;
}

void jumpOperand(const SourceStmt &jump, BasicBlock &block) {
    const SourceExpr &encoded = jump.expr.args[0];
    if (encoded.kind != SourceExprKind::Cast || encoded.args.size() != 1 || encoded.args[0].kind != SourceExprKind::Identifier)
        return;
    block.targetLabel = encoded.args[0].text;
    block.near = encoded.text == "i16";
}

bool foldValue(BasicBlock &block) {
    if (block.body.empty())
        return false;
    const SourceStmt &previous = block.body.back();
    const bool assigned = previous.kind == SourceStmtKind::Assign && previous.expr.kind == SourceExprKind::Identifier && previous.expr.text == "eax" &&
        previous.value.kind != SourceExprKind::Call && !(previous.value.kind == SourceExprKind::Identifier && previous.value.text == "eax");
    if (!assigned && !(previous.kind == SourceStmtKind::ExprStmt && previous.expr.kind == SourceExprKind::Call && !previous.expr.text.starts_with("vm_")))
        return false;
    block.exit.push_back(std::move(block.body.back()));
    block.body.pop_back();
    block.folded = true;
    return true;
}

void countReferences(const SourceExpr &expr, std::map<std::string, uint32_t> &counts) {
    if (expr.kind == SourceExprKind::Identifier)
        ++counts[expr.text];
    for (const SourceExpr &arg : expr.args)
        countReferences(arg, counts);
}

} // namespace

ControlFlowGraph::ControlFlowGraph(std::vector<SourceStmt> statements) {
    blocks.emplace_back();
    bool open = true;
    for (SourceStmt &stmt : statements) {
        countReferences(stmt.expr, references);
        countReferences(stmt.value, references);
        for (const SourceStmt &nested : stmt.body)
            countReferences(nested.expr, references);
        if (stmt.kind == SourceStmtKind::Label) {
            if (!open || !blocks.back().label.empty() || !blocks.back().body.empty())
                blocks.emplace_back();
            blocks.back().label = stmt.label;
            open = true;
            continue;
        }
        if (!open)
            blocks.emplace_back();
        open = true;
        BasicBlock &block = blocks.back();
        if (relativeJump(stmt)) {
            jumpOperand(stmt, block);
            block.kind = BlockExit::Goto;
        } else if (stmt.kind == SourceStmtKind::If && stmt.elseBody.empty() && stmt.body.size() == 1 && relativeJump(stmt.body[0]) &&
            stmt.expr.kind == SourceExprKind::Unary && stmt.expr.text == "!" && stmt.expr.args.size() == 1 && stmt.expr.args[0].text == "eax") {
            jumpOperand(stmt.body[0], block);
            block.kind = BlockExit::Branch;
            foldValue(block);
        } else if (stmt.kind == SourceStmtKind::Return && stmt.expr.text.empty() && stmt.expr.args.empty()) {
            block.kind = BlockExit::Return;
            if (!block.body.empty() && block.body.back().kind == SourceStmtKind::Assign)
                foldValue(block);
        } else if (stmt.kind == SourceStmtKind::ExprStmt && stmt.expr.kind == SourceExprKind::Call &&
            (stmt.expr.text == "vm_goto_local" || stmt.expr.text == "vm_return")) {
            block.kind = BlockExit::Stop;
        } else {
            block.body.push_back(std::move(stmt));
            continue;
        }
        block.exit.push_back(std::move(stmt));
        open = false;
    }
    refold();
    std::map<std::string, size_t> labels;
    for (size_t index = 0; index < blocks.size(); ++index)
        if (!blocks[index].label.empty())
            labels.emplace(blocks[index].label, index);
    for (BasicBlock &block : blocks) {
        const std::map<std::string, size_t>::const_iterator found = block.targetLabel.empty() ? labels.end() : labels.find(block.targetLabel);
        if (found != labels.end())
            block.target = found->second;
        if (block.target == kNoBlock && block.kind == BlockExit::Goto)
            block.kind = BlockExit::Stop;
    }
    for (size_t index = 0; index < blocks.size(); ++index)
        for (size_t successor : successors(index))
            blocks[successor].predecessors.push_back(index);

    std::vector<size_t> order, rank(blocks.size(), kNoBlock);
    std::vector<std::pair<size_t, size_t>> stack{{0, 0}};
    std::vector<bool> seen(blocks.size());
    seen[0] = true;
    while (!stack.empty()) {
        const std::vector<size_t> next = successors(stack.back().first);
        if (stack.back().second < next.size()) {
            const size_t successor = next[stack.back().second++];
            if (!seen[successor]) {
                seen[successor] = true;
                stack.emplace_back(successor, 0);
            }
            continue;
        }
        rank[stack.back().first] = order.size();
        order.push_back(stack.back().first);
        stack.pop_back();
    }
    blocks[0].dominator = 0;
    for (bool changed = true; changed;) {
        changed = false;
        for (size_t position = order.size() - 1; position-- > 0;) {
            BasicBlock &block = blocks[order[position]];
            size_t dominator = kNoBlock;
            for (size_t predecessor : block.predecessors) {
                if (blocks[predecessor].dominator == kNoBlock)
                    continue;
                if (dominator == kNoBlock) {
                    dominator = predecessor;
                    continue;
                }
                size_t left = predecessor, right = dominator;
                while (left != right) {
                    while (rank[left] < rank[right])
                        left = blocks[left].dominator;
                    while (rank[right] < rank[left])
                        right = blocks[right].dominator;
                }
                dominator = left;
            }
            if (dominator != block.dominator) {
                block.dominator = dominator;
                changed = true;
            }
        }
    }
}

std::vector<size_t> ControlFlowGraph::successors(size_t index) const {
    const BasicBlock &block = blocks[index];
    std::vector<size_t> result;
    if ((block.kind == BlockExit::Fall || block.kind == BlockExit::Branch) && index + 1 < blocks.size())
        result.push_back(index + 1);
    if (block.kind != BlockExit::Fall && block.kind != BlockExit::Return && block.target != kNoBlock)
        result.push_back(block.target);
    return result;
}

bool ControlFlowGraph::dominates(size_t dominator, size_t index) const {
    if (blocks[index].dominator == kNoBlock)
        return false;
    for (;; index = blocks[index].dominator) {
        if (index == dominator)
            return true;
        if (index == blocks[index].dominator)
            return false;
    }
}

void ControlFlowGraph::refold() {
    for (BasicBlock &block : blocks) {
        if (block.folded)
            block.value = block.exit[0].kind == SourceStmtKind::Assign ? block.exit[0].value : block.exit[0].expr;
        else if (block.kind == BlockExit::Branch)
            block.value = identifier("eax");
    }
}

} // namespace shenmue::scenario::decompile
