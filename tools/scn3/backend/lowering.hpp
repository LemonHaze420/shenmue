#pragma once

#include "scn3/bytecode/isa.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/backend/object.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/native/prototypes.hpp"

namespace shenmue::scenario {

class VmLowering {
  public:
    VmLowering(const SourceUnit &unit, const SourceFunctionAst &function) : unit_(unit), function_(function) {
    }

    VmFunctionObject lower();

    std::vector<uint8_t> link(VmFunctionObject object);

  private:
    void functionBody();

    [[noreturn]] void fail(const std::string &message) const;

    bool cUnit() const {
        return unit_.ordinaryC;
    }

    uint32_t constant(const SourceExpr &expr, uint8_t opcode, bool &symbolic) const;

    void operand(uint8_t opcode, const SourceExpr *expr);

    static uint8_t literalWidth(uint32_t value);

    void fixedOperand(uint8_t opcode, uint32_t value, uint8_t width);

    void scaleIndex(uint32_t stride);

    bool scaledIndex(const SourceExpr &index, uint32_t stride);

    const SourceLocalDeclaration *localDeclaration(const std::string &name) const;

    bool slot(const SourceExpr &expr, bool address);

    void checkNativeArguments(const SourceExpr &expr, const NativeArgumentLayout &layout);

    bool normalCall(const SourceExpr &expr, bool pushResult, bool needsValue = false);

    void rejectPointerObjectUse(const SourceExpr &expr) const;

    bool dataAddress(const SourceExpr &expr);

    void loadDataValue(ScenarioDataType type);

    void convertCValue(ScenarioDataType type, std::optional<uint32_t> value = {});

    bool localMemberAddress(const SourceExpr &expr);

    bool localArrayAddress(const SourceExpr &expr);

    void assignedValue(const SourceExpr &expr, bool targetFloat, bool targetUnsigned = false);

    std::optional<ScenarioDataType> cDataType(const std::string &type) const;

    uint32_t cObjectSize(const std::string &qualified) const;

    bool cIndirectAddress(const SourceExpr &expr);

    void expression(const SourceExpr &expr, bool valueRequired = true, bool truthContext = false);

    void mark(const std::string &name);

    void jump(Control operation, const std::string &name);

    void normalizeBoolean();
    bool cExpression(const SourceExpr &expr, bool valueRequired, bool truthContext);
    void cBranch(const SourceExpr &expr, const std::string &label, bool whenTrue);
    void cAddress(const SourceExpr &expr);
    void effect(const SourceExpr &expr);
    uint32_t temporary();
    void temporaryLoad(uint32_t offset);
    void temporaryAddress(uint32_t offset);
    void temporaryStore(uint32_t offset, const SourceExpr &expr);
    size_t temporaryDepth_ = 0;
    bool maskedAssignment_ = false;

    void condition(const SourceExpr &expr, bool truthContext = true);

    void block(const std::vector<SourceStmt> &stmts);

    void statement(const SourceStmt &stmt);

    const SourceUnit &unit_;
    const SourceFunctionAst &function_;
    std::vector<std::string> breakLabels_;
    std::vector<std::string> continueLabels_;
    size_t nextLabel_ = 0;
    bool chainArm_ = false;
    std::vector<uint8_t> bytes_;
    std::vector<VmRelocation> patches_;
    std::map<std::string, uint32_t> labels_;
};

} // namespace shenmue::scenario
