#include "scn3/backend/lowering.hpp"
#include "scn3/frontend/source.hpp"

namespace shenmue::scenario {

uint32_t VmLowering::temporary() {
    if (++temporaryDepth_ > function_.temporaryWords)
        fail("typed expression exceeds its reserved temporaries");
    return function_.temporaryBase - static_cast<uint32_t>(temporaryDepth_) * 4U;
}

void VmLowering::temporaryAddress(uint32_t offset) {
    fixedOperand(encode(Address::LocalAddress), offset, literalWidth(offset));
}
void VmLowering::temporaryLoad(uint32_t offset) {
    fixedOperand(encode(Address::Local32), offset, literalWidth(offset));
}
void VmLowering::temporaryStore(uint32_t offset, const SourceExpr &expr) {
    temporaryAddress(offset);
    expression(expr);
    bytes_.push_back(encode(Alu::Store32));
    bytes_.push_back(encode(Alu::PopEax));
}

void VmLowering::effect(const SourceExpr &expr) {
    if (normalCall(expr, false))
        return;

    const bool comparison = expr.kind == SourceExprKind::Binary && expr.args.size() == 2 &&
        (expr.text == "==" || expr.text == "!=" || expr.text == "<" || expr.text == "<=" || expr.text == ">" || expr.text == ">=");

    const bool assignment = expr.kind == SourceExprKind::Binary && expr.text == "=";
    expression(expr, expr.cType != "void" && !assignment, comparison);
    if (expr.cType != "void")
        bytes_.push_back(encode(Alu::PopEax));
}

void VmLowering::cAddress(const SourceExpr &expr) {
    if (cIndirectAddress(expr) || localArrayAddress(expr) || localMemberAddress(expr) || slot(expr, true) || dataAddress(expr))
        return;
    fail("C lvalue has no address: " + expr.text);
}

void VmLowering::cBranch(const SourceExpr &expr, const std::string &target, bool whenTrue) {

    const bool floatPredicate = expr.kind == SourceExprKind::Unary && expr.text == "!" && expr.args.size() == 1 &&
        expr.args[0].kind == SourceExprKind::Binary && expr.args[0].args.size() == 2 &&
        (expr.args[0].text == "<" || expr.args[0].text == "<=" || expr.args[0].text == ">" || expr.args[0].text == ">=") &&
        (explicitExpressionIsFloat(expr.args[0].args[0], &unit_, &function_) || explicitExpressionIsFloat(expr.args[0].args[1], &unit_, &function_));
    if (expr.kind == SourceExprKind::Unary && expr.text == "!" && !floatPredicate) {
        cBranch(expr.args[0], target, !whenTrue);
        return;
    }
    if (expr.kind == SourceExprKind::Binary && (expr.text == "&&" || expr.text == "||")) {
        const bool either = expr.text == "||";
        if (whenTrue == either) {
            cBranch(expr.args[0], target, whenTrue);
            cBranch(expr.args[1], target, whenTrue);
        } else {
            const std::string skip = "__c_logic_" + std::to_string(nextLabel_++);
            cBranch(expr.args[0], skip, !whenTrue);
            cBranch(expr.args[1], target, whenTrue);
            mark(skip);
        }
        return;
    }

    if (whenTrue && expr.kind == SourceExprKind::Binary && expr.args.size() == 2 &&
        (expr.text == "<" || expr.text == "<=" || expr.text == ">" || expr.text == ">=") &&
        (explicitExpressionIsFloat(expr.args[0], &unit_, &function_) || explicitExpressionIsFloat(expr.args[1], &unit_, &function_))) {
        condition(SourceExpr{.kind = SourceExprKind::Unary, .text = "!", .args = {expr}, .cType = "int32_t"});
        jump(Control::BranchFalse, target);
        return;
    }
    if (whenTrue) {
        const std::string skip = "__c_branch_" + std::to_string(nextLabel_++);
        condition(expr);
        jump(Control::BranchFalse, skip);
        jump(Control::Jump, target);
        mark(skip);
    } else {
        condition(expr);
        jump(Control::BranchFalse, target);
    }
}

bool VmLowering::cExpression(const SourceExpr &expr, bool valueRequired, bool truthContext) {
    if (expr.kind == SourceExprKind::Call && unit_.functions.count(expr.text) &&
        (expr.text.starts_with("vm_") || expr.text == "as_float" || expr.text == "as_word" || expr.text == "byte_address" || expr.text == "TASK_FN"))
        return normalCall(expr, valueRequired, valueRequired);
    if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1 &&
        (expr.cConversion == "PointerToIntegral" || expr.cConversion == "BitCast" || expr.cConversion == "IntegralToPointer")) {
        expression(expr.args[0]);
        return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.text == "void" && expr.args.size() == 1) {
        effect(expr.args[0]);
        return true;
    }
    if (expr.kind == SourceExprKind::Temporary) {
        temporaryLoad(static_cast<uint32_t>(std::stoul(expr.text)));
        return true;
    }
    if (expr.kind == SourceExprKind::IndirectCall) {
        for (size_t i = expr.args.size(); i > 1; --i)
            expression(expr.args[i - 1]);
        expression(expr.args[0]);
        bytes_.push_back(2);
        const uint32_t cleanup = static_cast<uint32_t>(expr.args.size() - 1) * 4U;
        if (cleanup)
            fixedOperand(encode(Control::AdjustStack), cleanup, literalWidth(cleanup));
        if (expr.cType != "void" && valueRequired)
            bytes_.push_back(encode(Alu::PushEax));
        return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1 && expr.text == "_Bool") {
        expression(expr.args[0], true, true);
        if (explicitExpressionIsFloat(expr.args[0], &unit_, &function_)) {
            fixedOperand(encode(Address::Literal), 0x7FFFFFFFU, 3);
            bytes_.push_back(encode(Alu::And));
        }
        fixedOperand(encode(Address::Literal), 0, 1);
        bytes_.push_back(encode(Alu::NotEqual));
        if (!truthContext)
            normalizeBoolean();
        return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1 &&
        ((expr.text == "float" && cExpressionIsUnsigned(expr.args[0], unit_, function_)) ||
            (expr.text == "uint32_t" && explicitExpressionIsFloat(expr.args[0], &unit_, &function_)))) {
        const size_t outer = temporaryDepth_;
        const uint32_t value = temporary();
        temporaryStore(value, expr.args[0]);
        const std::string low = "__c_conversion_low_" + std::to_string(nextLabel_++);
        const std::string end = "__c_conversion_end_" + std::to_string(nextLabel_++);
        if (expr.text == "float") {
            temporaryLoad(value);
            fixedOperand(encode(Address::Literal), 0, 1);
            bytes_.push_back(encode(Alu::Less));
            bytes_.push_back(encode(Alu::PopEax));
            jump(Control::BranchFalse, low);
            temporaryLoad(value);
            fixedOperand(encode(Address::Literal), 1, 1);
            bytes_.push_back(encode(Alu::ShiftRight));
            fixedOperand(encode(Address::Literal), 0x7FFFFFFFU, 3);
            bytes_.push_back(encode(Alu::And));
            temporaryLoad(value);
            fixedOperand(encode(Address::Literal), 1, 1);
            bytes_.push_back(encode(Alu::And));
            bytes_.push_back(encode(Alu::Or));
            bytes_.push_back(encode(Alu::IntToFloat));
            fixedOperand(encode(Address::Literal), 0x40000000U, 3);
            bytes_.push_back(encode(Alu::FloatMultiply));
        } else {
            temporaryLoad(value);
            fixedOperand(encode(Address::Literal), 0x4F000000U, 3);
            bytes_.push_back(encode(Alu::FloatLess));
            bytes_.push_back(encode(Alu::Not));
            bytes_.push_back(encode(Alu::PopEax));
            jump(Control::BranchFalse, low);
            temporaryLoad(value);
            fixedOperand(encode(Address::Literal), 0x4F000000U, 3);
            bytes_.push_back(encode(Alu::FloatSubtract));
            bytes_.push_back(encode(Alu::FloatToInt));
            fixedOperand(encode(Address::Literal), 0x80000000U, 3);
            bytes_.push_back(encode(Alu::Xor));
        }
        jump(Control::Jump, end);
        mark(low);
        temporaryLoad(value);
        bytes_.push_back(expr.text == "float" ? encode(Alu::IntToFloat) : encode(Alu::FloatToInt));
        mark(end);
        temporaryDepth_ = outer;
        return true;
    }
    if (expr.cConversion == "ArrayToPointerDecay" && (expr.kind == SourceExprKind::Identifier || expr.kind == SourceExprKind::Index)) {
        cAddress(expr);
        return true;
    }
    if (expr.kind == SourceExprKind::Conditional) {
        const std::string alternate = "__c_cond_else_" + std::to_string(nextLabel_++);
        const std::string end = "__c_cond_end_" + std::to_string(nextLabel_++);
        cBranch(expr.args[0], alternate, false);
        expression(expr.args[1], valueRequired, truthContext);
        jump(Control::Jump, end);
        mark(alternate);
        expression(expr.args[2], valueRequired, truthContext);
        mark(end);
        return true;
    }
    if (expr.kind == SourceExprKind::CompoundAssign || expr.kind == SourceExprKind::Postfix ||
        (expr.kind == SourceExprKind::Unary && (expr.text == "++" || expr.text == "--"))) {
        const size_t outer = temporaryDepth_;
        const uint32_t address = temporary(), old = temporary(), result = temporary();
        const SourceExpr &lvalue = expr.args[0];
        temporaryAddress(address);
        cAddress(lvalue);
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        const SourceExpr addressValue{.kind = SourceExprKind::Temporary, .text = std::to_string(address), .cType = "uint32_t"};
        const SourceExpr loaded{.kind = SourceExprKind::Unary, .text = "*", .args = {addressValue}, .cType = lvalue.cType};
        temporaryStore(old, loaded);
        SourceExpr left{.kind = SourceExprKind::Temporary, .text = std::to_string(old), .cType = lvalue.cType};
        const bool compound = expr.kind == SourceExprKind::CompoundAssign;
        const SourceExpr right = compound ? expr.args[1] : SourceExpr{.kind = SourceExprKind::IntLiteral, .text = "1", .cType = "int32_t"};
        const std::string operation = compound ? expr.text.substr(0, expr.text.size() - 1) : expr.text == "++" ? "+" : "-";
        const std::string computationType = compound ? expr.cConversion : lvalue.cType;
        if (computationType == "float" && left.cType != "float")
            left = SourceExpr{.kind = SourceExprKind::Cast, .text = "float", .args = {left}, .cType = "float", .cConversion = "IntegralToFloating"};
        SourceExpr binary{.kind = SourceExprKind::Binary, .text = operation, .args = {left, right}, .cType = computationType};
        SourceExpr destination{.kind = SourceExprKind::Unary, .text = "*", .args = {addressValue}, .cType = lvalue.cType};
        SourceExpr converted = binary;
        if ((binary.cType == "float" && lvalue.cType != "float") || lvalue.cType == "_Bool")
            converted =
                SourceExpr{.kind = SourceExprKind::Cast, .text = lvalue.cType, .args = {binary}, .cType = lvalue.cType, .cConversion = "FloatingToIntegral"};
        SourceExpr assignment{.kind = SourceExprKind::Binary, .text = "=", .args = {destination, converted}, .cType = lvalue.cType};
        temporaryStore(result, assignment);
        temporaryLoad(expr.kind == SourceExprKind::Postfix ? old : result);
        temporaryDepth_ = outer;
        return true;
    }
    if (expr.kind == SourceExprKind::Unary && expr.args.size() == 1 && (expr.text == "+" || expr.text == "-")) {
        const SourceExpr &value = expr.args[0];
        if (sourceDataConstantValue(expr, true) || (expr.text == "-" && value.kind == SourceExprKind::FloatLiteral))
            return false;
        if (expr.text == "+") {
            expression(value);
        } else if (explicitExpressionIsFloat(value, &unit_, &function_)) {
            expression(value);
            fixedOperand(encode(Address::Literal), 0x80000000U, 3);
            bytes_.push_back(encode(Alu::Xor));
        } else if ((value.kind == SourceExprKind::Binary &&
                       (value.text == "==" || value.text == "!=" || value.text == "<" || value.text == "<=" || value.text == ">" || value.text == ">=")) ||
            (value.kind == SourceExprKind::Unary && value.text == "!")) {
            expression(value, true, true);
        } else {
            fixedOperand(encode(Address::Literal), 0, 1);
            expression(value);
            bytes_.push_back(encode(Alu::Subtract));
        }
        return true;
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "!" && expr.args.size() == 1 && expr.args[0].kind == SourceExprKind::Binary &&
        expr.args[0].args.size() == 2 &&
        (cExpressionIsUnsigned(expr.args[0].args[0], unit_, function_) || cExpressionIsUnsigned(expr.args[0].args[1], unit_, function_))) {
        expression(expr.args[0], true, true);
        fixedOperand(encode(Address::Literal), 0, 1);
        bytes_.push_back(encode(Alu::Equal));
        if (!truthContext)
            normalizeBoolean();
        return true;
    }
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2 && (expr.text == "<<" || expr.text == ">>")) {
        const SourceExpr &count = expr.args[1];
        if (count.kind == SourceExprKind::Binary && count.text == "&" && count.args.size() == 2)
            for (size_t mask = 0; mask < 2; ++mask)
                if (sourceDataConstantValue(count.args[mask], true) == 31U) {
                    SourceExpr shift = expr;
                    shift.args[1] = count.args[1 - mask];
                    expression(shift, valueRequired, truthContext);
                    return true;
                }
    }
    if (expr.kind != SourceExprKind::Binary || expr.args.size() != 2)
        return false;
    if (expr.text == ",") {
        effect(expr.args[0]);
        expression(expr.args[1], valueRequired, truthContext);
        return true;
    }
    if (expr.text == "&&" || expr.text == "||") {
        const std::string zero = "__c_bool_zero_" + std::to_string(nextLabel_++);
        const std::string end = "__c_bool_end_" + std::to_string(nextLabel_++);
        cBranch(expr, zero, false);
        fixedOperand(encode(Address::Literal), truthContext ? 0xFFFFFFFFU : 1U, 1);
        jump(Control::Jump, end);
        mark(zero);
        fixedOperand(encode(Address::Literal), 0, 1);
        mark(end);
        return true;
    }
    const bool floating = explicitExpressionIsFloat(expr.args[0], &unit_, &function_) || explicitExpressionIsFloat(expr.args[1], &unit_, &function_);
    if (floating && (expr.text == "==" || expr.text == "!=")) {
        const size_t outer = temporaryDepth_;
        const uint32_t left = temporary(), right = temporary();
        temporaryStore(left, expr.args[0]);
        temporaryStore(right, expr.args[1]);
        temporaryLoad(left);
        temporaryLoad(right);
        bytes_.push_back(encode(Alu::FloatLess));
        temporaryLoad(left);
        temporaryLoad(right);
        bytes_.push_back(encode(Alu::FloatGreater));
        bytes_.push_back(encode(Alu::Or));
        if (expr.text == "==")
            bytes_.push_back(encode(Alu::Not));
        if (!truthContext)
            normalizeBoolean();
        temporaryDepth_ = outer;
        return true;
    }
    if ((expr.text == "/" || expr.text == "%") && !floating &&
        (cExpressionIsUnsigned(expr.args[0], unit_, function_) || cExpressionIsUnsigned(expr.args[1], unit_, function_))) {
        const size_t outer = temporaryDepth_;
        const uint32_t dividend = temporary(), divisor = temporary(), quotient = temporary(), remainder = temporary(), bit = temporary(), carry = temporary();
        temporaryStore(dividend, expr.args[0]);
        temporaryStore(divisor, expr.args[1]);
        for (const uint32_t slot : {quotient, remainder, bit}) {
            temporaryAddress(slot);
            fixedOperand(encode(Address::Literal), slot == bit ? 31 : 0, 1);
            bytes_.push_back(encode(Alu::Store32));
            bytes_.push_back(encode(Alu::PopEax));
        }
        const std::string loop = "__c_udiv_loop_" + std::to_string(nextLabel_++);
        const std::string compare = "__c_udiv_compare_" + std::to_string(nextLabel_++);
        const std::string subtract = "__c_udiv_subtract_" + std::to_string(nextLabel_++);
        const std::string next = "__c_udiv_next_" + std::to_string(nextLabel_++);
        const std::string end = "__c_udiv_end_" + std::to_string(nextLabel_++);
        mark(loop);
        temporaryAddress(carry);
        temporaryLoad(remainder);
        fixedOperand(encode(Address::Literal), 31, 1);
        bytes_.push_back(encode(Alu::ShiftRight));
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        temporaryAddress(remainder);
        temporaryLoad(remainder);
        fixedOperand(encode(Address::Literal), 1, 1);
        bytes_.push_back(encode(Alu::ShiftLeft));
        temporaryLoad(dividend);
        temporaryLoad(bit);
        bytes_.push_back(encode(Alu::ShiftRight));
        fixedOperand(encode(Address::Literal), 1, 1);
        bytes_.push_back(encode(Alu::And));
        bytes_.push_back(encode(Alu::Or));
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        temporaryLoad(carry);
        bytes_.push_back(encode(Alu::PopEax));
        jump(Control::BranchFalse, compare);
        jump(Control::Jump, subtract);
        mark(compare);
        temporaryLoad(remainder);
        fixedOperand(encode(Address::Literal), 0x80000000U, 3);
        bytes_.push_back(encode(Alu::Xor));
        temporaryLoad(divisor);
        fixedOperand(encode(Address::Literal), 0x80000000U, 3);
        bytes_.push_back(encode(Alu::Xor));
        bytes_.push_back(encode(Alu::GreaterEqual));
        bytes_.push_back(encode(Alu::PopEax));
        jump(Control::BranchFalse, next);
        mark(subtract);
        temporaryAddress(remainder);
        temporaryLoad(remainder);
        temporaryLoad(divisor);
        bytes_.push_back(encode(Alu::Subtract));
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        temporaryAddress(quotient);
        temporaryLoad(quotient);
        fixedOperand(encode(Address::Literal), 1, 1);
        temporaryLoad(bit);
        bytes_.push_back(encode(Alu::ShiftLeft));
        bytes_.push_back(encode(Alu::Or));
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        mark(next);
        temporaryAddress(bit);
        temporaryLoad(bit);
        fixedOperand(encode(Address::Literal), 1, 1);
        bytes_.push_back(encode(Alu::Subtract));
        bytes_.push_back(encode(Alu::Store32));
        bytes_.push_back(encode(Alu::PopEax));
        temporaryLoad(bit);
        fixedOperand(encode(Address::Literal), 0, 1);
        bytes_.push_back(encode(Alu::GreaterEqual));
        bytes_.push_back(encode(Alu::PopEax));
        jump(Control::BranchFalse, end);
        jump(Control::Jump, loop);
        mark(end);
        temporaryLoad(expr.text == "/" ? quotient : remainder);
        temporaryDepth_ = outer;
        return true;
    }
    if (!floating && (expr.text == "<" || expr.text == "<=" || expr.text == ">" || expr.text == ">=") &&
        (cExpressionIsUnsigned(expr.args[0], unit_, function_) || cExpressionIsUnsigned(expr.args[1], unit_, function_) ||
            expr.args[0].cType.find('*') != std::string::npos)) {
        for (const SourceExpr &arg : expr.args) {
            expression(arg);
            fixedOperand(encode(Address::Literal), 0x80000000U, 3);
            bytes_.push_back(encode(Alu::Xor));
        }
        bytes_.push_back(encode(expr.text == "<" ? Alu::Less : expr.text == "<=" ? Alu::LessEqual : expr.text == ">" ? Alu::Greater : Alu::GreaterEqual));
        if (!truthContext)
            normalizeBoolean();
        return true;
    }
    if (expr.text == ">>" && cExpressionIsUnsigned(expr.args[0], unit_, function_)) {
        const size_t outer = temporaryDepth_;
        const uint32_t count = temporary();
        temporaryStore(count, expr.args[1]);
        expression(expr.args[0]);
        temporaryLoad(count);
        bytes_.push_back(encode(Alu::ShiftRight));
        fixedOperand(encode(Address::Literal), 0x80000000U, 3);
        temporaryLoad(count);
        bytes_.push_back(encode(Alu::ShiftRight));
        fixedOperand(encode(Address::Literal), 1, 1);
        bytes_.push_back(encode(Alu::ShiftLeft));
        bytes_.push_back(encode(Alu::Not));
        bytes_.push_back(encode(Alu::And));
        temporaryDepth_ = outer;
        return true;
    }

    const SourceExpr *assigned = expr.kind == SourceExprKind::Binary && expr.text == "&" && expr.args.size() == 2 ? &expr.args[0] : nullptr;
    if (assigned && assigned->kind == SourceExprKind::Cast && assigned->cConversion == "IntegralCast" && assigned->args.size() == 1)
        assigned = &assigned->args[0];
    if (assigned && assigned->kind == SourceExprKind::Binary && assigned->text == "=" && assigned->args.size() == 2) {
        const std::optional<ScenarioDataType> stored = cDataType(assigned->args[0].cType);
        const std::optional<uint32_t> mask = sourceDataConstantValue(expr.args[1], true);
        if (stored && scenarioDataTypeSize(*stored) < 4 && mask && *mask <= (scenarioDataTypeSize(*stored) == 1 ? 0x7FU : 0x7FFFU)) {
            maskedAssignment_ = true;
            expression(*assigned);
            expression(expr.args[1]);
            bytes_.push_back(encode(Alu::And));
            return true;
        }
    }
    if (truthContext && (expr.text == "|" || expr.text == "&" || expr.text == "^")) {
        const std::function<bool(const SourceExpr &)> relational = [&](const SourceExpr &e) {
            if (e.kind == SourceExprKind::Unary && e.text == "!" && e.args.size() == 1)
                return relational(e.args[0]);
            return e.kind == SourceExprKind::Binary && (e.text == "==" || e.text == "!=" || e.text == "<" || e.text == "<=" || e.text == ">" || e.text == ">=");
        };
        if (expr.text == "|" || (relational(expr.args[0]) && relational(expr.args[1]))) {
            expression(expr.args[0], true, true);
            expression(expr.args[1], true, true);
            bytes_.push_back(expr.text == "|" ? encode(Alu::Or) : expr.text == "&" ? encode(Alu::And) : encode(Alu::Xor));
            return true;
        }
    }
    return false;
}

} // namespace shenmue::scenario
