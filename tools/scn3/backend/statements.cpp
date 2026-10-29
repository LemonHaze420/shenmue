#include "scn3/backend/lowering.hpp"
#include "scn3/backend/object.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

void VmLowering::functionBody() {
    if (function_.explicitFrameAdjust)
        fixedOperand(encode(Control::AdjustStack), *function_.explicitFrameAdjust, *function_.explicitFrameAdjust <= 0xFFU ? 1 : 2);
    block(function_.body.body);
    if ((function_.semantics == SourceSemantics::C11) && (function_.body.body.empty() || function_.body.body.back().kind != SourceStmtKind::Return)) {
        bool epilogue = function_.body.body.empty() || function_.body.body.back().kind != SourceStmtKind::For ||
            !(function_.body.body.back().expr.text.empty() && function_.body.body.back().expr.args.empty());
        const std::function<bool(const std::vector<SourceStmt> &)> exitsLoop = [&](const std::vector<SourceStmt> &statements) {
            for (const SourceStmt &statement : statements) {
                if (statement.kind == SourceStmtKind::Break)
                    return true;
                if (statement.kind == SourceStmtKind::While || statement.kind == SourceStmtKind::For || statement.kind == SourceStmtKind::Switch)
                    continue;
                if (exitsLoop(statement.body) || exitsLoop(statement.elseBody))
                    return true;
            }
            return false;
        };
        if (!epilogue)
            epilogue = exitsLoop(function_.body.body.back().body);
        for (const VmRelocation &patch : patches_)
            if (patch.value.kind == SourceExprKind::Identifier && patch.value.text == "__c_epilogue")
                epilogue = true;
        if (epilogue) {
            mark("__c_epilogue");
            bytes_.push_back(0);
        }
    }
}

void VmLowering::mark(const std::string &name) {
    if (!labels_.emplace(name, function_.offset + static_cast<uint32_t>(bytes_.size())).second)
        fail("duplicate label " + name);
}

void VmLowering::jump(Control operation, const std::string &name) {
    SourceExpr target{.kind = SourceExprKind::Identifier, .text = name, .args = {}};
    SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i16", .args = {std::move(target)}};
    operand(encode(operation), &encoded);
}

void VmLowering::block(const std::vector<SourceStmt> &stmts) {
    for (const SourceStmt &stmt : stmts)
        statement(stmt);
}

void VmLowering::statement(const SourceStmt &stmt) {
    switch (stmt.kind) {
    case SourceStmtKind::Block:
        block(stmt.body);
        return;
    case SourceStmtKind::Declaration:
        return;
    case SourceStmtKind::Label:
        mark(stmt.label);
        return;
    case SourceStmtKind::Return:
        if ((function_.semantics == SourceSemantics::C11) && function_.returnType == "void" && !(stmt.expr.text.empty() && stmt.expr.args.empty()))
            fail("void function cannot return a value");
        if ((function_.semantics == SourceSemantics::C11) && function_.returnType != "void" && (stmt.expr.text.empty() && stmt.expr.args.empty()))
            fail("non-void return requires a value");
        if (!(stmt.expr.text.empty() && stmt.expr.args.empty())) {
            condition(stmt.expr, false);
            if ((function_.semantics == SourceSemantics::C11) &&
                ((function_.returnType == "float" && cExpressionIsUnsigned(stmt.expr, unit_, function_)) ||
                    (function_.returnType == "uint32_t" && explicitExpressionIsFloat(stmt.expr, &unit_, &function_))))
                fail("Unsupported operation: unsigned/float return conversion");
            if ((function_.semantics == SourceSemantics::C11) &&
                (function_.returnType == "float") != explicitExpressionIsFloat(stmt.expr, &unit_, &function_)) {
                bytes_.push_back(encode(Alu::PushEax));
                bytes_.push_back(function_.returnType == "float" ? encode(Alu::IntToFloat) : encode(Alu::FloatToInt));
                bytes_.push_back(encode(Alu::PopEax));
            }
        }
        if ((function_.semantics == SourceSemantics::C11) && &stmt != &function_.body.body.back())
            jump(Control::Jump, "__c_epilogue");
        else {
            if ((function_.semantics == SourceSemantics::C11))
                mark("__c_epilogue");
            bytes_.push_back(0);
        }
        return;
    case SourceStmtKind::Assign:
        if (function_.semantics == SourceSemantics::Explicit && stmt.expr.kind == SourceExprKind::Identifier && stmt.expr.text == "eax")
            expression(stmt.value);
        else
            expression(SourceExpr{.kind = SourceExprKind::Binary, .text = "=", .args = {stmt.expr, stmt.value}}, false);
        bytes_.push_back(encode(Alu::PopEax));
        return;
    case SourceStmtKind::ExprStmt:
        if (function_.semantics == SourceSemantics::Explicit && stmt.expr.kind == SourceExprKind::Call && stmt.expr.text == "vm_push") {
            if (stmt.expr.args.size() != 1)
                fail("vm_push requires one expression");
            expression(stmt.expr.args[0]);
            return;
        }
        if ((function_.semantics == SourceSemantics::C11) && !(stmt.expr.kind == SourceExprKind::Call && stmt.expr.text.starts_with("vm_"))) {
            effect(stmt.expr);
            return;
        }
        expression(stmt.expr, false);
        return;
    case SourceStmtKind::If:
        if (function_.semantics == SourceSemantics::C11) {

            const bool arm = std::exchange(chainArm_, false);
            if (!arm && stmt.elseBody.empty() && stmt.body.size() == 1) {
                const SourceStmt &transfer = stmt.body[0];
                if (transfer.kind == SourceStmtKind::Goto) {
                    cBranch(stmt.expr, transfer.label, true);
                    return;
                }
                if (transfer.kind == SourceStmtKind::Break && !breakLabels_.empty()) {
                    cBranch(stmt.expr, breakLabels_.back(), true);
                    return;
                }
                if (transfer.kind == SourceStmtKind::Continue && !continueLabels_.empty()) {
                    cBranch(stmt.expr, continueLabels_.back(), true);
                    return;
                }
            }
            const std::string alternate = "__c_if_else_" + std::to_string(nextLabel_++);
            const std::string end = "__c_if_end_" + std::to_string(nextLabel_++);
            cBranch(stmt.expr, stmt.elseBody.empty() ? end : alternate, false);
            block(stmt.body);
            if (!stmt.elseBody.empty() || arm)
                jump(Control::Jump, end);
            if (!stmt.elseBody.empty()) {
                mark(alternate);
                chainArm_ = stmt.elseIf;
                block(stmt.elseBody);
                chainArm_ = false;
            }
            mark(end);
            return;
        }
        if (stmt.elseBody.empty() && stmt.expr.kind == SourceExprKind::Unary && stmt.expr.text == "!" && stmt.expr.args.size() == 1 && stmt.body.size() == 1) {
            const SourceStmt &transfer = stmt.body[0];
            if (transfer.kind == SourceStmtKind::Goto) {
                condition(stmt.expr.args[0]);
                jump(Control::BranchFalse, transfer.label);
                return;
            }
            if (transfer.kind == SourceStmtKind::Break && !breakLabels_.empty()) {
                condition(stmt.expr.args[0]);
                jump(Control::BranchFalse, breakLabels_.back());
                return;
            }
            if (transfer.kind == SourceStmtKind::Continue && !continueLabels_.empty()) {
                condition(stmt.expr.args[0]);
                jump(Control::BranchFalse, continueLabels_.back());
                return;
            }
        }
        if (stmt.elseBody.empty() && stmt.expr.kind == SourceExprKind::Unary && stmt.expr.text == "!" && stmt.expr.args.size() == 1 &&
            stmt.expr.args[0].kind == SourceExprKind::Identifier && stmt.expr.args[0].text == "eax" && stmt.body.size() == 1 &&
            stmt.body[0].kind == SourceStmtKind::ExprStmt && stmt.body[0].expr.text == "vm_goto_rel" && stmt.body[0].expr.args.size() == 1) {
            operand(encode(Control::BranchFalse), &stmt.body[0].expr.args[0]);
            return;
        }
        {
            const std::string end = "__vm_if_end_" + std::to_string(nextLabel_++);
            const std::string alternate = "__vm_if_else_" + std::to_string(nextLabel_++);
            condition(stmt.expr);
            jump(Control::BranchFalse, stmt.elseBody.empty() ? end : alternate);
            block(stmt.body);
            if (!stmt.elseBody.empty()) {
                jump(Control::Jump, end);
                mark(alternate);
                block(stmt.elseBody);
            }
            mark(end);
            return;
        }
    case SourceStmtKind::Switch: {
        const size_t outer = temporaryDepth_;
        SourceExpr selector = function_.semantics == SourceSemantics::C11 ? cSwitchSelector(stmt.expr) : stmt.expr;
        if (function_.semantics == SourceSemantics::C11 && (selector.kind != SourceExprKind::Identifier || selector.volatileAccess)) {
            const uint32_t offset = temporary();
            temporaryStore(offset, selector);
            selector = SourceExpr{.kind = SourceExprKind::Temporary, .text = std::to_string(offset), .cType = "int32_t"};
        } else if (selector.kind != SourceExprKind::Identifier || selector.text == "eax")
            fail("switch selector must be an explicit local, task or argument slot");
        const std::string end = "__vm_switch_end_" + std::to_string(nextLabel_++);
        bool terminated = true;
        std::set<uint32_t> caseValues;
        uint32_t defaults = 0;
        for (const SourceSwitchCase &clause : stmt.cases) {
            if (clause.isDefault) {
                if (++defaults > 1)
                    fail("duplicate default case");
                continue;
            }
            std::optional<uint32_t> value = sourceDataConstantValue(clause.value, cUnit());
            if (!value && clause.value.kind == SourceExprKind::Identifier)
                value = lookupAbiValueByName(clause.value.text);
            if (!value || !caseValues.insert(*value).second)
                fail("switch cases require unique integer constants");
        }
        for (size_t i = 0; i < stmt.cases.size(); ++i) {
            const SourceSwitchCase &clause = stmt.cases[i];
            if (clause.isDefault && i + 1 != stmt.cases.size())
                terminated = false;
            if (clause.body.empty()) {
                terminated = false;
                continue;
            }

            const SourceStmt *final = &clause.body.back();
            while (final->kind == SourceStmtKind::Block && !final->body.empty())
                final = &final->body.back();
            const SourceStmtKind last = final->kind;
            if (last != SourceStmtKind::Break && last != SourceStmtKind::Return && last != SourceStmtKind::Goto)
                terminated = false;
        }
        breakLabels_.push_back(end);
        if (terminated) {
            for (const SourceSwitchCase &clause : stmt.cases) {
                if (clause.isDefault) {
                    block(clause.body);
                    continue;
                }
                const std::string next = "__vm_case_next_" + std::to_string(nextLabel_++);
                expression(selector);
                expression(clause.value);
                bytes_.push_back(encode(Alu::Equal));
                bytes_.push_back(encode(Alu::PopEax));
                jump(Control::BranchFalse, next);
                block(clause.body);
                mark(next);
            }
        } else {
            std::vector<std::string> bodies;
            std::string fallback = end;
            bool haveDefault = false;
            for (size_t i = 0; i < stmt.cases.size(); ++i) {
                bodies.push_back("__vm_case_body_" + std::to_string(nextLabel_++));
                const SourceSwitchCase &clause = stmt.cases[i];
                if (clause.isDefault) {
                    if (haveDefault)
                        fail("duplicate default case");
                    haveDefault = true;
                    fallback = bodies.back();
                    continue;
                }
                const std::string next = "__vm_case_next_" + std::to_string(nextLabel_++);
                expression(selector);
                expression(clause.value);
                bytes_.push_back(encode(Alu::Equal));
                bytes_.push_back(encode(Alu::PopEax));
                jump(Control::BranchFalse, next);
                jump(Control::Jump, bodies.back());
                mark(next);
            }
            jump(Control::Jump, fallback);
            for (size_t i = 0; i < stmt.cases.size(); ++i) {
                mark(bodies[i]);
                block(stmt.cases[i].body);
            }
        }
        breakLabels_.pop_back();
        mark(end);
        temporaryDepth_ = outer;
        return;
    }
    case SourceStmtKind::Break:
        if (breakLabels_.empty())
            fail("break outside loop or switch");
        jump(Control::Jump, breakLabels_.back());
        return;
    case SourceStmtKind::Continue:
        if (continueLabels_.empty())
            fail("continue outside loop");
        jump(Control::Jump, continueLabels_.back());
        return;
    case SourceStmtKind::For:
    case SourceStmtKind::DoWhile:
    case SourceStmtKind::While: {
        if (function_.semantics == SourceSemantics::C11) {
            const std::string start = "__c_loop_" + std::to_string(nextLabel_++);
            const std::string next = "__c_continue_" + std::to_string(nextLabel_++);
            const std::string end = "__c_loop_end_" + std::to_string(nextLabel_++);
            block(stmt.initializer);
            mark(start);
            if (stmt.kind != SourceStmtKind::DoWhile && !(stmt.expr.text.empty() && stmt.expr.args.empty()))
                cBranch(stmt.expr, end, false);
            breakLabels_.push_back(end);

            continueLabels_.push_back(stmt.kind != SourceStmtKind::DoWhile && stmt.increment.text.empty() && stmt.increment.args.empty() ? start : next);
            block(stmt.body);
            continueLabels_.pop_back();
            breakLabels_.pop_back();
            mark(next);
            if (!stmt.increment.text.empty())
                effect(stmt.increment);
            if (stmt.kind == SourceStmtKind::DoWhile)
                cBranch(stmt.expr, end, false);
            jump(Control::Jump, start);
            mark(end);
            return;
        }
        const std::string start = "__vm_while_start_" + std::to_string(nextLabel_++);
        const std::string end = "__vm_while_end_" + std::to_string(nextLabel_++);
        const bool forever = stmt.kind == SourceStmtKind::For || (stmt.expr.kind == SourceExprKind::Identifier && stmt.expr.text == "true");
        mark(start);
        if (!forever) {
            condition(stmt.expr);
            jump(Control::BranchFalse, end);
        }
        breakLabels_.push_back(end);
        continueLabels_.push_back(start);
        block(stmt.body);
        continueLabels_.pop_back();
        breakLabels_.pop_back();
        jump(Control::Jump, start);
        mark(end);
        return;
    }
    case SourceStmtKind::Goto: {
        SourceExpr target{.kind = SourceExprKind::Identifier, .text = stmt.label, .args = {}};
        SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i16", .args = {target}};
        operand(encode(Control::Jump), &encoded);
        return;
    }
    default:
        fail("unsupported statement; use explicit VM control flow");
    }
}

} // namespace shenmue::scenario
