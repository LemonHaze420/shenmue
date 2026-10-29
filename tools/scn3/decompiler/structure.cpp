#include "scn3/decompiler/cfg.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario::decompile {

namespace {

struct Transfers {
    size_t breakTarget = kNoBlock;
    size_t continueTarget = kNoBlock;
};

SourceExpr negated(SourceExpr value) {
    return SourceExpr{.kind = SourceExprKind::Unary, .text = "!", .args = {std::move(value)}};
}

class Structurer {
  public:
    explicit Structurer(ControlFlowGraph graph) : graph_(std::move(graph)), blocks_(graph_.blocks) {
    }

    std::vector<SourceStmt> structure() {
        return range(0, blocks_.size(), false, {});
    }

  private:
    bool singleEntry(size_t begin, size_t end, size_t from) const {
        for (size_t index = begin; index < end; ++index)
            for (size_t predecessor : blocks_[index].predecessors)
                if ((predecessor < begin || predecessor >= end) && !(index == begin && (from == kNoBlock || predecessor == from)))
                    return false;
        return true;
    }

    static bool reaches(size_t target, size_t end, bool dropLast) {
        return target < end || (target == end && !dropLast);
    }

    size_t loopLatch(size_t header, size_t end, bool dropLast) const {
        size_t latch = kNoBlock;
        for (size_t index = header; index < end - (dropLast ? 1U : 0U); ++index) {
            const BasicBlock &block = blocks_[index];
            if (block.kind == BlockExit::Goto && block.near && block.target == header && graph_.dominates(header, index))
                latch = index;
        }
        return latch != kNoBlock && singleEntry(header, latch + 1, kNoBlock) ? latch : kNoBlock;
    }

    std::vector<SourceStmt> range(size_t begin, size_t end, bool dropLast, const Transfers &transfers, size_t header = kNoBlock) {
        std::vector<SourceStmt> out;
        for (size_t index = begin; index < end;) {
            const BasicBlock &block = blocks_[index];
            if (index != header && !block.label.empty()) {
                out.push_back(SourceStmt{.kind = SourceStmtKind::Label, .label = block.label});
                if (const size_t latch = loopLatch(index, end, dropLast); latch != kNoBlock) {
                    const Transfers inner{.breakTarget = latch + 1, .continueTarget = index};
                    SourceStmt loop{.kind = SourceStmtKind::While};
                    if (latch > index && block.body.empty() && block.kind == BlockExit::Branch && block.near && block.target == latch + 1) {
                        loop.expr = block.value;
                        loop.body = range(index + 1, latch + 1, true, inner);
                    } else {
                        loop.expr = identifier("true");
                        loop.body = range(index, latch + 1, true, inner, index);
                    }
                    out.push_back(std::move(loop));
                    index = latch + 1;
                    continue;
                }
            }
            out.insert(out.end(), block.body.begin(), block.body.end());
            if (index + 1 == end && dropLast)
                break;
            index = exit(index, end, dropLast, transfers, out);
        }
        return out;
    }

    size_t exit(size_t index, size_t end, bool dropLast, const Transfers &transfers, std::vector<SourceStmt> &out) {
        const BasicBlock &block = blocks_[index];
        if (block.kind == BlockExit::Return && block.folded) {
            out.push_back(SourceStmt{.kind = SourceStmtKind::Return, .expr = block.value});
            return index + 1;
        }
        if (!block.near || block.target == kNoBlock || (block.kind != BlockExit::Goto && block.kind != BlockExit::Branch)) {
            out.insert(out.end(), block.exit.begin(), block.exit.end());
            return index + 1;
        }
        const bool transfer = block.target == transfers.continueTarget || block.target == transfers.breakTarget;
        const SourceStmt jump = block.target == transfers.continueTarget ? SourceStmt{.kind = SourceStmtKind::Continue} :
            block.target == transfers.breakTarget                        ? SourceStmt{.kind = SourceStmtKind::Break} :
                                                                           SourceStmt{.kind = SourceStmtKind::Goto, .label = blocks_[block.target].label};
        if (block.kind == BlockExit::Goto || transfer) {
            out.push_back(block.kind == BlockExit::Goto ? jump : SourceStmt{.kind = SourceStmtKind::If, .expr = negated(block.value), .body = {jump}});
            return index + 1;
        }
        if (const size_t next = selection(index, end, dropLast, transfers, out); next != kNoBlock)
            return next;
        const size_t join = block.target;
        if (join > index + 1 && reaches(join, end, dropLast) && singleEntry(index + 1, join, index)) {
            SourceStmt structured{.kind = SourceStmtKind::If, .expr = block.value};
            const BasicBlock &tail = blocks_[join - 1];
            if (tail.kind == BlockExit::Goto && tail.near && tail.target != kNoBlock && tail.target > join && reaches(tail.target, end, dropLast) &&
                singleEntry(join, tail.target, index)) {
                structured.body = range(index + 1, join, true, transfers);
                structured.elseBody = range(join, tail.target, false, transfers);
                out.push_back(std::move(structured));
                return tail.target;
            }
            structured.body = range(index + 1, join, false, transfers);

            const SourceStmtKind only = structured.body.size() == 1 ? structured.body[0].kind : SourceStmtKind::Block;
            if (!(block.value.kind == SourceExprKind::Unary && block.value.text == "!" &&
                    (only == SourceStmtKind::Goto || only == SourceStmtKind::Break || only == SourceStmtKind::Continue))) {
                out.push_back(std::move(structured));
                return join;
            }
        }
        out.push_back(SourceStmt{.kind = SourceStmtKind::If, .expr = negated(block.value), .body = {jump}});
        return index + 1;
    }

    static std::optional<uint32_t> caseValue(const SourceExpr &test, const SourceExpr &selector) {
        if (test.kind != SourceExprKind::Binary || test.text != "==" || test.args.size() != 2 || test.args[0].kind != SourceExprKind::Identifier ||
            test.args[0].text == "eax" || test.args[0].text != selector.text)
            return std::nullopt;
        if (const std::optional<uint32_t> value = sourceDataConstantValue(test.args[1]))
            return value;
        return test.args[1].kind == SourceExprKind::Identifier ? lookupAbiValueByName(test.args[1].text) : std::nullopt;
    }

    size_t selection(size_t index, size_t end, bool dropLast, const Transfers &transfers, std::vector<SourceStmt> &out) {
        const BasicBlock &first = blocks_[index];
        if (first.value.kind != SourceExprKind::Binary || first.value.args.size() != 2)
            return kNoBlock;
        const SourceExpr &selector = first.value.args[0];
        std::set<uint32_t> values;
        std::vector<std::pair<size_t, size_t>> clauses;
        size_t exit = kNoBlock;
        for (size_t test = index;;) {
            const BasicBlock &compare = blocks_[test];
            const std::optional<uint32_t> value = caseValue(compare.value, selector);
            if (compare.kind != BlockExit::Branch || !compare.near || compare.target == kNoBlock || !value || !values.insert(*value).second)
                return kNoBlock;
            if (test != index && (!compare.body.empty() || compare.predecessors.size() != 1 || graph_.references[compare.label] != 1))
                return kNoBlock;
            const size_t next = compare.target;
            if (next <= test + 1 || !reaches(next, end, dropLast) || !singleEntry(test + 1, next, test))
                return kNoBlock;
            const BasicBlock &tail = blocks_[next - 1];
            if (tail.kind != BlockExit::Goto || !tail.near || tail.target == kNoBlock || (exit != kNoBlock && tail.target != exit))
                return kNoBlock;
            exit = tail.target;
            clauses.emplace_back(test, next);
            if (next == exit)
                break;
            test = next;
        }
        if (clauses.size() < 2)
            return kNoBlock;
        SourceStmt selected{.kind = SourceStmtKind::Switch, .expr = selector};
        for (const std::pair<size_t, size_t> &clause : clauses) {
            SourceSwitchCase item{.value = blocks_[clause.first].value.args[1]};
            item.body = range(clause.first + 1, clause.second, true, Transfers{.breakTarget = exit, .continueTarget = transfers.continueTarget});
            item.body.push_back(SourceStmt{.kind = SourceStmtKind::Break});
            selected.cases.push_back(std::move(item));
        }
        out.push_back(std::move(selected));
        return exit;
    }

    ControlFlowGraph graph_;
    const std::vector<BasicBlock> &blocks_;
};

} // namespace

std::vector<SourceStmt> structureFunction(ControlFlowGraph graph) {
    return Structurer(std::move(graph)).structure();
}

} // namespace shenmue::scenario::decompile
