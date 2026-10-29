#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario {

struct ExplicitLocalMember {
    SourceExpr base;
    const ScenarioDataField *field;
};

bool cExpressionIsUnsigned(const SourceExpr &expr, const SourceUnit &unit, const SourceFunctionAst &function);
SourceFunctionAst allocateCLocals(const SourceFunctionAst &function);
const SourceExpr &cSwitchSelector(const SourceExpr &selector);
std::optional<ExplicitLocalMember> explicitLocalMember(const SourceExpr &expr, const SourceFunctionAst &function, const SourceUnit *unit = nullptr);
bool explicitExpressionIsFloat(const SourceExpr &expr, const SourceUnit *unit = nullptr, const SourceFunctionAst *function = nullptr);

} // namespace shenmue::scenario
