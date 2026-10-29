#include "scn3/common/util.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario::decompile {

SourceExpr identifier(std::string text) {
    return SourceExpr{.kind = SourceExprKind::Identifier, .text = std::move(text)};
}

SourceExpr number(uint32_t value) {
    return SourceExpr{.kind = SourceExprKind::IntLiteral, .text = value < 10U ? std::to_string(value) : hexUnsigned(value)};
}

SourceExpr call(std::string name, std::vector<SourceExpr> args) {
    return SourceExpr{.kind = SourceExprKind::Call, .text = std::move(name), .args = std::move(args)};
}

SourceExpr signedNumber(uint32_t value) {
    if (static_cast<int32_t>(value) >= 0)
        return number(value);
    return SourceExpr{.kind = SourceExprKind::Unary, .text = "-", .args = {number(0U - value)}};
}

SourceStmt expressionStatement(SourceExpr expr) {
    return SourceStmt{.kind = SourceStmtKind::ExprStmt, .expr = std::move(expr)};
}

void visitExpressions(SourceExpr &expr, const std::function<void(SourceExpr &)> &visitor) {
    for (SourceExpr &child : expr.args)
        visitExpressions(child, visitor);
    visitor(expr);
}

void visitExpressions(std::vector<SourceStmt> &statements, const std::function<void(SourceExpr &)> &visitor) {
    for (SourceStmt &statement : statements) {
        visitExpressions(statement.expr, visitor);
        visitExpressions(statement.value, visitor);
        visitExpressions(statement.body, visitor);
        visitExpressions(statement.elseBody, visitor);
        for (SourceSwitchCase &clause : statement.cases) {
            visitExpressions(clause.value, visitor);
            visitExpressions(clause.body, visitor);
        }
    }
}

} // namespace shenmue::scenario::decompile
