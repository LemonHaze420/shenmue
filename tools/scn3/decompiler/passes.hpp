#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario::decompile {

SourceExpr identifier(std::string text);
SourceExpr number(uint32_t value);
SourceExpr call(std::string name, std::vector<SourceExpr> args = {});
SourceExpr encodedOperand(const LowLevelInsn &insn, const LowLevelFunction &function, const std::set<uint32_t> &labels,
    const std::map<uint32_t, std::string> &data, const std::string &profile);
uint8_t naturalWidth(uint32_t value);
SourceExpr signedNumber(uint32_t value);
SourceExpr friendlyExpression(const SourceExpr &original);
void functionReferences(SourceExpr &expr, const SourceUnit &unit);
void functionReferences(std::vector<SourceStmt> &stmts, const SourceUnit &unit);
void collectParameters(const SourceExpr &expr, uint32_t &count);
void collectParameters(const std::vector<SourceStmt> &stmts, uint32_t &count);
void friendlyStatements(std::vector<SourceStmt> &stmts);
std::string printExpression(const SourceExpr &expr);
SourceStmt expressionStatement(SourceExpr expr);
std::vector<SourceStmt> liftFunction(const LowLevelFunction &function, const std::set<uint32_t> &labels, const std::map<uint32_t, std::string> &data,
    const std::string &profile, const SourceUnit &verification);
SourceExpr typedFloatLiteral(const SourceExpr &value);
void typedFrameStores(std::vector<SourceStmt> &stmts, SourceFunctionAst &function, bool shared);
void recoverCSiblingNames(SourceFunctionAst &function);
std::string printCondition(const SourceExpr &expr);
std::string printCExpression(const SourceExpr &expr, int context = 0);
void printStatement(std::ostream &out, const SourceStmt &stmt, const std::string &indent = "  ", bool cSyntax = false);
void attachUnreferencedReturns(Scn3Module &module);
void visitExpressions(SourceExpr &expr, const std::function<void(SourceExpr &)> &visitor);
void visitExpressions(std::vector<SourceStmt> &statements, const std::function<void(SourceExpr &)> &visitor);
void recoverCScopedRecords(SourceFunctionAst &function);
void recoverCNarrowParameters(SourceFunctionAst &function);
bool recoverCCopiedRecords(SourceFunctionAst &function, const SourceUnit &unit);
bool recoverCPassedWorkspaceRecords(SourceUnit &unit);
bool recoverCWorkspacePointers(SourceUnit &unit);
std::optional<std::pair<std::string, uint32_t>> dereferencedParameter(const SourceExpr &expression, const SourceFunctionAst &function);
void recoverCCallSignatures(SourceUnit &unit);
void recoverCForwardedPointerParameters(SourceUnit &unit);
std::string cVariadicCursor(const SourceFunctionAst &function);
void recoverCVariadicArguments(SourceFunctionAst &function);
void recoverCRecordAccesses(SourceFunctionAst &function, const SourceUnit &unit);
void recoverCMemberAddresses(SourceFunctionAst &function);
void recoverCRecordMembers(SourceFunctionAst &function, bool typed);
void recoverCWorkspaceRecords(SourceUnit &unit);
std::map<std::string, bool> recoverCNativeData(SourceUnit &unit, uint32_t dataEnd);
void recoverCDataTables(SourceUnit &unit, const std::vector<uint8_t> &blob, uint32_t dataEnd, std::map<std::string, bool> &readOnly);
std::string cCharacterLiteral(const std::vector<uint8_t> &bytes, uint32_t start, uint32_t end);
bool cAlwaysReturns(const std::vector<SourceStmt> &statements);
void recoverCValueReturns(SourceFunctionAst &function);
std::set<std::string> discardUnreadCReturns(SourceUnit &unit);
void recoverCBooleanValues(SourceFunctionAst &function);
void recoverCWordArithmetic(SourceFunctionAst &function, const SourceUnit &unit);
void recoverCWorkspaceValues(SourceUnit &unit);
void recoverCUnaryOperators(SourceFunctionAst &function, const SourceUnit &unit);
void recoverCRegisterExpressions(std::vector<SourceStmt> &statements);
bool nativeCFloatArgument(const SourceExpr &expression, size_t index);

void recoverCFloatStorage(SourceUnit &unit);
SourceExpr cFloatLiteral(const SourceExpr &expression);
SourceExpr cTypedExpression(const SourceExpr &expression, const SourceFunctionAst &function, bool floating = false, const SourceUnit *unit = nullptr);
void recoverCFloatExpressions(SourceFunctionAst &function, const SourceUnit &unit);
void recoverCAddressWords(SourceFunctionAst &function, const SourceUnit &unit);
bool cUnknownReturnCall(const SourceExpr &expression, const SourceUnit &unit);
void cReturnType(const std::vector<SourceStmt> &statements, std::string &type, const SourceUnit &unit, const SourceFunctionAst &function, bool &unknown);
void recoverCStatementSyntax(std::vector<SourceStmt> &statements);
void recoverCLiteralRadix(std::vector<SourceStmt> &statements);
void recoverCForwardBranches(std::vector<SourceStmt> &statements, std::map<std::string, uint32_t> &references);
void discardUnusedCLabels(SourceFunctionAst &function);
void recoverCByteStorage(SourceFunctionAst &function);
void recoverCLocalArrays(SourceFunctionAst &function);
bool recoverCLexicalStorage(SourceFunctionAst &function, const std::multimap<uint32_t, SourceLocalDeclaration> &locals, uint32_t reserved);
void establishCLocals(SourceFunctionAst &function);
void cDataValue(std::ostream &out, const std::vector<uint8_t> &bytes, uint32_t offset, ScenarioDataType type);
void cDataRecord(std::ostream &out, const std::vector<uint8_t> &bytes, uint32_t start, const ScenarioDataSchema &schema,
    const std::function<std::string(uint32_t)> &pointer = {});
std::vector<SourceStmt> cVariadicBody(const SourceFunctionAst &function);
void cPrototype(std::ostream &out, const SourceFunctionAst &function);
std::string cFunctionPointerType(const SourceFunctionAst &function, const std::string &declarator);

void printCModuleData(std::ostream &out, const SourceUnit &unit, const Scn3Module &original, uint32_t dataEnd, const std::map<std::string, bool> &readOnlyData,
    const std::map<uint32_t, SourceFunctionAst> &functions, const std::map<std::string, SourceExpr> &stringLiterals);

} // namespace shenmue::scenario::decompile
