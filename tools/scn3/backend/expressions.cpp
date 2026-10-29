#include "scn3/backend/lowering.hpp"
#include <utility>
#include "scn3/backend/types.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

void VmLowering::assignedValue(const SourceExpr &expr, bool targetFloat, bool targetUnsigned) {
    const bool sourceFloat = explicitExpressionIsFloat(expr, &unit_, &function_);
    if (targetFloat != sourceFloat && !(function_.semantics == SourceSemantics::C11))
        fail("assignment requires an explicit float/word conversion");
    if ((function_.semantics == SourceSemantics::C11) && ((targetFloat && cExpressionIsUnsigned(expr, unit_, function_)) || (targetUnsigned && sourceFloat)))
        fail("unsigned/float conversion requires an established unsigned backend lowering");
    expression(expr);
    if (targetFloat != sourceFloat)
        bytes_.push_back(targetFloat ? encode(Alu::IntToFloat) : encode(Alu::FloatToInt));
}

std::optional<std::pair<int64_t, int64_t>> cValueRange(const SourceExpr &value) {
    if (const std::optional<uint32_t> constant = sourceDataConstantValue(value, true)) {
        const int64_t signedValue = static_cast<int32_t>(*constant);
        return std::pair{signedValue, signedValue};
    }

    if (value.kind == SourceExprKind::Call) {
        if (value.cType == "int8_t" || value.cType == "signed char")
            return std::pair<int64_t, int64_t>{-128, 127};
        if (value.cType == "int16_t" || value.cType == "short")
            return std::pair<int64_t, int64_t>{-32768, 32767};
    }
    if (value.kind == SourceExprKind::Cast && value.args.size() == 1 && value.text.find('*') == std::string::npos) {
        if (value.cType == "int8_t" || value.cType == "signed char")
            return std::pair<int64_t, int64_t>{-128, 127};
        if (value.cType == "uint8_t" || value.cType == "unsigned char")
            return std::pair<int64_t, int64_t>{0, 255};
        if (value.cType == "int16_t" || value.cType == "short")
            return std::pair<int64_t, int64_t>{-32768, 32767};
        if (value.cType == "uint16_t" || value.cType == "unsigned short")
            return std::pair<int64_t, int64_t>{0, 65535};
        return cValueRange(value.args[0]);
    }
    if (value.kind == SourceExprKind::Identifier) {
        if (value.cType == "int8_t")
            return std::pair<int64_t, int64_t>{-128, 127};
        if (value.cType == "uint8_t")
            return std::pair<int64_t, int64_t>{0, 255};
        if (value.cType == "int16_t")
            return std::pair<int64_t, int64_t>{-32768, 32767};
        if (value.cType == "uint16_t")
            return std::pair<int64_t, int64_t>{0, 65535};
        return std::nullopt;
    }
    if (value.kind == SourceExprKind::Unary && value.args.size() == 1) {
        if (value.text == "!")
            return std::pair<int64_t, int64_t>{0, 1};
        if (value.text == "-")
            if (const std::optional<std::pair<int64_t, int64_t>> operand = cValueRange(value.args[0]))
                return std::pair{-operand->second, -operand->first};
        return std::nullopt;
    }
    if (value.kind == SourceExprKind::Binary && value.args.size() == 2) {
        const std::string &op = value.text;
        if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=" || op == "&&" || op == "||")
            return std::pair<int64_t, int64_t>{0, 1};
        if (op == "&")
            for (const SourceExpr &side : value.args)
                if (const std::optional<uint32_t> mask = sourceDataConstantValue(side, true); mask && static_cast<int32_t>(*mask) >= 0)
                    return std::pair<int64_t, int64_t>{0, static_cast<int64_t>(*mask)};
    }
    return std::nullopt;
}

bool cStoredValueFits(const SourceExpr &value, ScenarioDataType type) {
    const std::optional<std::pair<int64_t, int64_t>> range = cValueRange(value);
    if (!range)
        return false;
    const std::pair<int64_t, int64_t> limits = type == ScenarioDataType::I8 ? std::pair<int64_t, int64_t>{-128, 127} :
        type == ScenarioDataType::U8                                        ? std::pair<int64_t, int64_t>{0, 255} :
        type == ScenarioDataType::I16                                       ? std::pair<int64_t, int64_t>{-32768, 32767} :
        type == ScenarioDataType::U16                                       ? std::pair<int64_t, int64_t>{0, 65535} :
                                                                              std::pair<int64_t, int64_t>{INT32_MIN, UINT32_MAX};
    return limits.first <= range->first && range->second <= limits.second;
}

void VmLowering::expression(const SourceExpr &expr, bool valueRequired, bool truthContext) {
    if (function_.semantics == SourceSemantics::C11 && cExpression(expr, valueRequired, truthContext))
        return;
    if (expr.kind == SourceExprKind::VariadicArgument) {
        if (!(function_.semantics == SourceSemantics::C11))
            fail("variadic argument reads require a typed C function");
        const uint32_t offset = static_cast<uint32_t>(std::stoul(expr.text));
        if (offset < 8 || offset > 32764 || offset % 4)
            fail("invalid variadic argument displacement");
        fixedOperand(encode(Address::Local32), offset, literalWidth(offset));
        return;
    }
    if (cIndirectAddress(expr)) {
        const std::optional<ScenarioDataType> dataType = cDataType(expr.cType);
        if (!dataType)
            fail("C aggregate values require a member selection or address");
        loadDataValue(*dataType);
        return;
    }
    if (localArrayAddress(expr)) {
        loadDataValue(*cDataType(localDeclaration(expr.args[0].text)->type));
        return;
    }
    if (const std::optional<ExplicitLocalMember> member = explicitLocalMember(expr, function_, &unit_)) {
        if ((function_.semantics == SourceSemantics::C11) && function_.localOffsets.count(member->base.text)) {
            const uint32_t offset = function_.localOffsets.at(member->base.text) + member->field->offset;
            const uint8_t width = literalWidth(offset);
            const uint32_t size = scenarioDataTypeSize(member->field->type);
            fixedOperand(encode(size == 1 ? Address::Local8 : size == 2 ? Address::Local16 : Address::Local32), offset & (width == 1 ? 0xFFU : 0xFFFFU), width);
            if (member->field->type == ScenarioDataType::U8 || member->field->type == ScenarioDataType::U16) {
                fixedOperand(encode(Address::Literal), size == 1 ? 0xFFU : 0xFFFFU, size == 1 ? 2 : 3);
                bytes_.push_back(encode(Alu::And));
            }
        } else {
            localMemberAddress(expr);
            loadDataValue(member->field->type);
        }
        return;
    }
    if (slot(expr, false))
        return;
    const bool dataMember = (cUnit() && expr.kind == SourceExprKind::Identifier && unit_.dataSymbols.count(expr.text)) || expr.kind == SourceExprKind::Index ||
        (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".")) ||
        (expr.kind == SourceExprKind::Identifier && expr.text.find('.') != std::string::npos);
    if (dataMember) {
        const std::optional<SourceDataAddress> type = resolveSourceDataAddress(unit_, expr, true);
        if (type && dataAddress(expr)) {
            if (!type->isArray && type->dataType)
                loadDataValue(*type->dataType);
            return;
        }
    }
    if (expr.kind == SourceExprKind::Call && (expr.text == "as_float" || expr.text == "as_word" || expr.text == "byte_address") && expr.args.size() == 1) {
        expression(expr.args[0]);
        return;
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "*" && expr.args.size() == 1) {
        if ((function_.semantics == SourceSemantics::C11))
            fail("Unsupported operation: typed C pointer dereference");
        expression(expr.args[0]);
        bytes_.push_back(encode(Alu::Load32));
        return;
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "-" && expr.args.size() == 1 && expr.args[0].kind == SourceExprKind::FloatLiteral) {
        const uint32_t bits = parseSourceFloatBits(expr.args[0].text) ^ 0x80000000U;
        fixedOperand(encode(Address::Literal), bits, (function_.semantics == SourceSemantics::C11) ? literalWidth(bits) : 3);
        return;
    }
    if (expr.kind == SourceExprKind::FloatLiteral) {
        const uint32_t bits = parseSourceFloatBits(expr.text);
        fixedOperand(encode(Address::Literal), bits, (function_.semantics == SourceSemantics::C11) ? literalWidth(bits) : 3);
        return;
    }
    if (expr.kind == SourceExprKind::IntLiteral || (expr.kind == SourceExprKind::Unary && expr.text == "-")) {
        const std::optional<uint32_t> value = sourceDataConstantValue(expr, cUnit());
        if (value) {
            fixedOperand(encode(Address::Literal), *value, literalWidth(*value));
            return;
        }
    }
    if (expr.kind == SourceExprKind::Identifier && expr.text != "eax") {
        std::optional<uint32_t> value = sourceDataConstantValue(expr, cUnit());
        if (!value)
            value = lookupAbiValueByName(expr.text);
        if (value) {
            fixedOperand(encode(Address::Literal), *value, literalWidth(*value));
            return;
        }
        if (unit_.dataSymbols.count(expr.text) || (cUnit() && unit_.functions.count(expr.text))) {
            SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {expr}};
            operand(encode(Address::Relative3), &encoded);
            return;
        }
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "&" && expr.args.size() == 1) {
        if (cIndirectAddress(expr.args[0]) || localArrayAddress(expr.args[0]) || localMemberAddress(expr.args[0]) || slot(expr.args[0], true) ||
            dataAddress(expr.args[0]))
            return;
        if (expr.args[0].kind == SourceExprKind::Identifier && (unit_.dataSymbols.count(expr.args[0].text) || unit_.functions.count(expr.args[0].text))) {
            SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {expr.args[0]}};
            operand(encode(Address::Relative3), &encoded);
            return;
        }
    }
    if (expr.kind == SourceExprKind::Cast && expr.text == "void" && expr.args.size() == 1) {
        expression(expr.args[0]);
        return;
    }
    if (normalCall(expr, valueRequired, valueRequired))
        return;
    if (expr.kind == SourceExprKind::Call && expr.text == "TASK_FN" && expr.args.size() == 1) {
        SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {expr.args[0]}};
        operand(encode(Address::Relative3), &encoded);
        return;
    }
    if (expr.kind == SourceExprKind::Cast && sourceImmediateWidthCallName(expr.text)) {
        operand(encode(Address::Literal), &expr);
        return;
    }
    if (function_.semantics == SourceSemantics::Explicit && expr.kind == SourceExprKind::Identifier && expr.text == "eax") {
        bytes_.push_back(encode(Alu::PushEax));
        return;
    }
    if ((function_.semantics == SourceSemantics::C11) && expr.kind == SourceExprKind::Unary && expr.text == "!" && expr.args.size() == 1) {
        const SourceExpr &value = expr.args[0];
        const bool relation = value.kind == SourceExprKind::Binary && value.args.size() == 2 &&
            (value.text == "<" || value.text == "<=" || value.text == ">" || value.text == ">=");
        if (relation && !explicitExpressionIsFloat(value.args[0], &unit_, &function_) && !explicitExpressionIsFloat(value.args[1], &unit_, &function_)) {
            if (cExpressionIsUnsigned(value.args[0], unit_, function_) || cExpressionIsUnsigned(value.args[1], unit_, function_))
                fail("Unsupported operation: unsigned ordering");
            expression(value.args[0]);
            expression(value.args[1]);
            bytes_.push_back(encode(value.text == "<" ? Alu::GreaterEqual :
                    value.text == "<="                ? Alu::Greater :
                    value.text == ">"                 ? Alu::LessEqual :
                                                        Alu::Less));
        } else if (relation && (explicitExpressionIsFloat(value.args[0], &unit_, &function_) || explicitExpressionIsFloat(value.args[1], &unit_, &function_))) {
            for (const SourceExpr &argument : value.args) {
                if (cExpressionIsUnsigned(argument, unit_, function_))
                    fail("Unsupported operation: unsigned/float promotion");
                expression(argument);
                if (!explicitExpressionIsFloat(argument, &unit_, &function_))
                    bytes_.push_back(encode(Alu::IntToFloat));
            }
            bytes_.push_back(encode(value.text == "<" ? Alu::FloatGreaterEqual :
                    value.text == "<="                ? Alu::FloatGreater :
                    value.text == ">"                 ? Alu::FloatLessEqual :
                                                        Alu::FloatLess));
        } else {
            expression(value, true, true);
            if (explicitExpressionIsFloat(value, &unit_, &function_)) {
                fixedOperand(encode(Address::Literal), 0x7FFFFFFFU, 3);
                bytes_.push_back(encode(Alu::And));
            }
            fixedOperand(encode(Address::Literal), 0, 1);
            bytes_.push_back(encode(Alu::Equal));
        }
        if (!truthContext)
            normalizeBoolean();
        return;
    }
    if (cUnit() && expr.kind == SourceExprKind::Cast && expr.args.size() == 1 && (expr.text == "int8_t" || expr.text == "int16_t")) {
        const std::optional<ExplicitLocalMember> member = explicitLocalMember(expr.args[0], function_, &unit_);
        if (member && scenarioDataTypeSize(member->field->type) == (expr.text == "int8_t" ? 1U : 2U)) {
            localMemberAddress(expr.args[0]);
            loadDataValue(expr.text == "int8_t" ? ScenarioDataType::I8 : ScenarioDataType::I16);
            return;
        }
    }
    if ((function_.semantics == SourceSemantics::C11) && expr.kind == SourceExprKind::Cast && expr.args.size() == 1 && cDataType(expr.text)) {
        const ScenarioDataType type = *cDataType(expr.text);
        assignedValue(expr.args[0], type == ScenarioDataType::F32, type == ScenarioDataType::U32);
        convertCValue(type, explicitExpressionIsFloat(expr.args[0], &unit_, &function_) ? std::nullopt : sourceDataConstantValue(expr.args[0], true));
        return;
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "~" && expr.args.size() == 1) {
        if (explicitExpressionIsFloat(expr.args[0], &unit_, &function_))
            fail("bitwise not on float requires as_word");
        expression(expr.args[0]);
        bytes_.push_back(encode(Alu::Not));
        return;
    }
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2) {
        if (expr.text == "=") {
            const bool extended = valueRequired && !std::exchange(maskedAssignment_, false);
            const SourceExpr &lvalue = expr.args[0];
            const SourceExpr &converted = expr.args[1];
            const std::optional<ScenarioDataType> storage = cDataType(lvalue.cType);
            const bool narrowStore = storage && scenarioDataTypeSize(*storage) < 4 && converted.kind == SourceExprKind::Cast &&
                converted.cConversion == "IntegralCast" && converted.args.size() == 1 && converted.cType == lvalue.cType;
            const SourceExpr &rhs = narrowStore ? converted.args[0] : converted;
            const std::optional<uint32_t> constant = explicitExpressionIsFloat(rhs, &unit_, &function_) ? std::nullopt : sourceDataConstantValue(rhs, true);
            if (cIndirectAddress(lvalue)) {
                const std::optional<ScenarioDataType> type = cDataType(lvalue.cType);
                if (!type)
                    fail("C aggregate assignment requires a record copy");
                assignedValue(rhs, *type == ScenarioDataType::F32, *type == ScenarioDataType::U32);
                const uint32_t width = scenarioDataTypeSize(*type);
                bytes_.push_back(width == 1 ? encode(Alu::Store8) : width == 2 ? encode(Alu::Store16) : encode(Alu::Store32));
                if (extended && !cStoredValueFits(rhs, *type))
                    convertCValue(*type, constant);
                return;
            }
            if (localArrayAddress(lvalue)) {
                const SourceLocalDeclaration *declaration = localDeclaration(lvalue.args[0].text);
                assignedValue(rhs, declaration->type == "float", declaration->type == "uint32_t");
                const ScenarioDataType type = *cDataType(declaration->type);
                const uint32_t width = scenarioDataTypeSize(type);
                bytes_.push_back(width == 1 ? encode(Alu::Store8) : width == 2 ? encode(Alu::Store16) : encode(Alu::Store32));
                if (extended && !cStoredValueFits(rhs, type))
                    convertCValue(type, constant);
                return;
            }
            if (const std::optional<ExplicitLocalMember> member = explicitLocalMember(lvalue, function_, &unit_)) {
                localMemberAddress(lvalue);
                assignedValue(rhs, member->field->type == ScenarioDataType::F32, member->field->type == ScenarioDataType::U32);
                const uint32_t width = scenarioDataTypeSize(member->field->type);
                bytes_.push_back(width == 1 ? encode(Alu::Store8) : width == 2 ? encode(Alu::Store16) : encode(Alu::Store32));
                if ((function_.semantics == SourceSemantics::C11) && extended && !cStoredValueFits(rhs, member->field->type))
                    convertCValue(member->field->type, constant);
                return;
            }
            if (lvalue.kind == SourceExprKind::Unary && lvalue.text == "*" && lvalue.args.size() == 1) {
                if ((function_.semantics == SourceSemantics::C11))
                    fail("Unsupported operation: typed C pointer dereference");
                expression(lvalue.args[0]);
                expression(rhs);
                bytes_.push_back(encode(Alu::Store32));
                return;
            }
            const bool localLvalue =
                (function_.semantics == SourceSemantics::C11) && lvalue.kind == SourceExprKind::Identifier && function_.localTypes.count(lvalue.text);
            const std::optional<SourceDataAddress> data = localLvalue ? std::nullopt : resolveSourceDataAddress(unit_, lvalue, true);
            if (data && data->dataType && !data->isArray && dataAddress(lvalue)) {
                if ((function_.semantics == SourceSemantics::C11))
                    assignedValue(rhs, *data->dataType == ScenarioDataType::F32, *data->dataType == ScenarioDataType::U32);
                else {
                    if ((*data->dataType == ScenarioDataType::F32) != explicitExpressionIsFloat(expr.args[1], &unit_, &function_))
                        fail("record field assignment requires an explicit float/word conversion");
                    expression(rhs);
                }
                const uint32_t width = scenarioDataTypeSize(*data->dataType);
                bytes_.push_back(width == 1 ? encode(Alu::Store8) : width == 2 ? encode(Alu::Store16) : encode(Alu::Store32));
                if ((function_.semantics == SourceSemantics::C11) && extended && !cStoredValueFits(rhs, *data->dataType))
                    convertCValue(*data->dataType, constant);
                return;
            }
            if (slot(lvalue, true)) {
                const SourceLocalDeclaration *declaration = (function_.semantics == SourceSemantics::C11) ? localDeclaration(lvalue.text) : nullptr;
                if (declaration && (declaration->isArray || findScenarioDataSchema(declaration->type)))
                    fail("aggregate assignment requires a record copy");
                if (!function_.localTypes.count(lvalue.text) && unit_.workspaceBindings.count(lvalue.text) &&
                    findScenarioDataSchema(unit_.workspaceBindings.at(lvalue.text).type))
                    fail("aggregate assignment requires a record copy");
                if (declaration)
                    assignedValue(rhs, declaration->type == "float", declaration->type == "uint32_t");
                else if ((function_.semantics == SourceSemantics::C11) && function_.localTypes.count(lvalue.text))
                    assignedValue(rhs, function_.localTypes.at(lvalue.text) == "float", function_.localTypes.at(lvalue.text) == "uint32_t");
                else if ((function_.semantics == SourceSemantics::C11) && unit_.workspaceBindings.count(lvalue.text))
                    assignedValue(rhs, unit_.workspaceBindings.at(lvalue.text).type == "float", unit_.workspaceBindings.at(lvalue.text).type == "uint32_t");
                else
                    expression(rhs);
                const std::optional<ScenarioDataType> localType =
                    function_.localTypes.count(lvalue.text) ? scenarioDataTypeByName(function_.localTypes.at(lvalue.text)) : std::nullopt;
                const uint8_t access = localType ?
                    static_cast<uint8_t>(scenarioDataTypeSize(*localType)) :
                    unit_.workspaceBindings.count(lvalue.text) && !function_.localTypes.count(lvalue.text) ?
                    static_cast<uint8_t>(scenarioDataTypeSize(*scenarioDataTypeByName(unit_.workspaceBindings.at(lvalue.text).type))) :
                    sourceIdentifierAccessWidth(lvalue.text);
                bytes_.push_back(access == 1 ? encode(Alu::Store8) : access == 2 ? encode(Alu::Store16) : encode(Alu::Store32));
                if (cUnit() && extended && access != 4) {
                    const std::optional<ScenarioDataType> stored = localType ? localType :
                        unit_.workspaceBindings.count(lvalue.text)           ? scenarioDataTypeByName(unit_.workspaceBindings.at(lvalue.text).type) :
                                                                               std::nullopt;
                    if (stored && !cStoredValueFits(rhs, *stored))
                        convertCValue(*stored, constant);
                }
                return;
            }
            if (lvalue.kind != SourceExprKind::Call || lvalue.args.size() != 1)
                fail("assignment requires vm_mem8/16/32(address)");
            const Operation *direct = operationNamed(lvalue.text);
            const Address access = direct ? addressOperation(direct->opcode) : Address::Literal;
            if (direct && direct->opcode >= kAddressClass && direct->opcode < kAluClass && addressBase(access) != AddrBase::CurrBase &&
                addressAccess(access) != kPointerAccess) {

                operand(encode(addressOperation(addressBase(access), kPointerAccess)), &lvalue.args[0]);
                expression(rhs);
                bytes_.push_back(static_cast<uint8_t>(encode(Alu::Store8) + addressAccess(access)));
                return;
            }
            const uint8_t opcode = lvalue.text == "vm_mem8" ? encode(Alu::Store8) :
                lvalue.text == "vm_mem16"                   ? encode(Alu::Store16) :
                lvalue.text == "vm_mem32"                   ? encode(Alu::Store32) :
                                                              0;
            if (!opcode)
                fail("assignment requires vm_mem8/16/32(address)");
            expression(lvalue.args[0]);
            expression(rhs);
            bytes_.push_back(opcode);
            return;
        }
        if (!unit_.cDeclarations.empty() && expr.text == "-" && expr.cType.find('*') == std::string::npos &&
            expr.args[0].cType.find('*') != std::string::npos && expr.args[1].cType.find('*') != std::string::npos) {
            const std::string &pointerType = expr.args[0].cType;
            const uint32_t stride = cObjectSize(trimCopy(pointerType.substr(0, pointerType.rfind('*'))));
            expression(expr.args[0]);
            expression(expr.args[1]);
            bytes_.push_back(encode(Alu::Subtract));
            if (stride != 1) {
                fixedOperand(encode(Address::Literal), stride, literalWidth(stride));
                bytes_.push_back(encode(Alu::Divide));
            }
            return;
        }
        if (!unit_.cDeclarations.empty() && (expr.text == "+" || expr.text == "-") && expr.cType.find('*') != std::string::npos) {
            const size_t star = expr.cType.rfind('*');
            const uint32_t stride = cObjectSize(trimCopy(expr.cType.substr(0, star)));
            const bool pointerLeft = expr.args[0].cType.find('*') != std::string::npos;
            if (!pointerLeft && expr.text == "-")
                fail("integer minus pointer is not a C address expression");
            expression(expr.args[pointerLeft ? 0 : 1]);
            if (scaledIndex(expr.args[pointerLeft ? 1 : 0], stride))
                bytes_.push_back(expr.text == "+" ? encode(Alu::Add) : encode(Alu::Subtract));
            return;
        }
        const bool leftFloat = explicitExpressionIsFloat(expr.args[0], &unit_, &function_);
        const bool rightFloat = explicitExpressionIsFloat(expr.args[1], &unit_, &function_);
        if (leftFloat != rightFloat && !(function_.semantics == SourceSemantics::C11))
            fail("mixed float/word operands require explicit vm_int_to_float or as_float/as_word");
        if (leftFloat || rightFloat) {
            if ((function_.semantics == SourceSemantics::C11) &&
                (cExpressionIsUnsigned(expr.args[0], unit_, function_) || cExpressionIsUnsigned(expr.args[1], unit_, function_)))
                fail("Unsupported operation: unsigned/float promotion");
            const uint8_t opcode = expr.text == "+" ? encode(Alu::FloatAdd) :
                expr.text == "-"                    ? encode(Alu::FloatSubtract) :
                expr.text == "*"                    ? encode(Alu::FloatMultiply) :
                expr.text == "/"                    ? encode(Alu::FloatDivide) :
                expr.text == "<"                    ? encode(Alu::FloatLess) :
                expr.text == "<="                   ? encode(Alu::FloatLessEqual) :
                expr.text == ">"                    ? encode(Alu::FloatGreater) :
                expr.text == ">="                   ? encode(Alu::FloatGreaterEqual) :
                                                      0;
            if (!opcode)
                fail("float operation requires an explicit supported operator");
            expression(expr.args[0]);
            if (!leftFloat)
                bytes_.push_back(encode(Alu::IntToFloat));
            expression(expr.args[1]);
            if (!rightFloat)
                bytes_.push_back(encode(Alu::IntToFloat));
            if ((function_.semantics == SourceSemantics::C11) && opcode >= encode(Alu::FloatLess) && opcode <= encode(Alu::FloatGreaterEqual)) {

                bytes_.push_back(static_cast<uint8_t>(encode(Alu::FloatLess) + encode(Alu::FloatGreaterEqual) - opcode));
                bytes_.push_back(encode(Alu::Not));
                if (!truthContext)
                    normalizeBoolean();
            } else
                bytes_.push_back(opcode);
            return;
        }
        if ((function_.semantics == SourceSemantics::C11) &&
            (expr.text == "<" || expr.text == "<=" || expr.text == ">" || expr.text == ">=" || expr.text == "/" || expr.text == "%") &&
            (cExpressionIsUnsigned(expr.args[0], unit_, function_) || cExpressionIsUnsigned(expr.args[1], unit_, function_)))
            fail("Unsupported operation: unsigned ordering/division");
        if ((function_.semantics == SourceSemantics::C11) && expr.text == ">>" && cExpressionIsUnsigned(expr.args[0], unit_, function_))
            fail("unsigned right shift requires a logical-shift backend lowering");
        for (const Operation &op : kAluOperations) {
            if (op.binary && expr.text == op.binary) {
                expression(expr.args[0]);
                expression(expr.args[1]);
                bytes_.push_back(op.opcode);
                if ((function_.semantics == SourceSemantics::C11) && !truthContext && op.opcode >= encode(Alu::Equal) && op.opcode <= encode(Alu::GreaterEqual))
                    normalizeBoolean();
                return;
            }
        }
    }
    if (expr.kind != SourceExprKind::Call)
        fail("value requires an explicit operation or encoding: " + expr.text);
    if (expr.text == "vm_mem8" || expr.text == "vm_mem16" || expr.text == "vm_mem32") {
        if (expr.args.size() != 1)
            fail("memory access requires one address");
        expression(expr.args[0]);
        bytes_.push_back(expr.text == "vm_mem8" ? encode(Alu::Load8) : expr.text == "vm_mem16" ? encode(Alu::Load16) : encode(Alu::Load32));
        return;
    }
    if (expr.text == "vm_alu") {
        if (valueRequired)
            fail("raw ALU instructions are statements, not value expressions");
        if (expr.args.size() != 1 || expr.args[0].kind != SourceExprKind::IntLiteral)
            fail("vm_alu requires one literal subopcode");
        const uint32_t sub = parseSourceIntegerLiteral(expr.args[0].text);
        if (sub >= 64)
            fail("vm_alu subopcode exceeds 63");
        bytes_.push_back(static_cast<uint8_t>(kAluClass | sub));
        return;
    }
    if (expr.text == "vm_reserved") {
        if (valueRequired)
            fail("reserved instructions are statements, not value expressions");
        if (expr.args.size() != 1 || expr.args[0].kind != SourceExprKind::IntLiteral)
            fail("vm_reserved requires one reserved opcode");
        const uint32_t opcode = parseSourceIntegerLiteral(expr.args[0].text);
        if (opcode < kReservedClass || opcode > 0xFF)
            fail("vm_reserved only accepts opcodes 0xC0..0xFF");
        bytes_.push_back(static_cast<uint8_t>(opcode));
        return;
    }
    const Operation *op = operationNamed(expr.text);
    if (!op)
        fail("unknown operation " + expr.text);
    if (op->opcode < kAddressClass && valueRequired)
        fail("control instructions are statements; read eax explicitly for call results");
    if (op->opcode < kAluClass) {
        const Control control = controlOperation(op->opcode);
        if (expr.args.size() > 1 && !(op->opcode < kAddressClass && (control == Control::LocalCall || control == Control::IndirectCall || nativeCall(control))))
            fail("operation expects a single encoded operand: " + expr.text);

        for (size_t i = expr.args.size(); i > 1; --i)
            expression(expr.args[i - 1]);
        operand(op->opcode, expr.args.empty() ? nullptr : &expr.args[0]);
    } else {
        if (valueRequired && (expr.args.empty() || !op->results))
            fail("value expression must supply its stack inputs: " + expr.text);
        if (!expr.args.empty() && expr.args.size() != op->inputs)
            fail("wrong operand count for " + expr.text);
        for (const SourceExpr &arg : expr.args)
            expression(arg);
        bytes_.push_back(op->opcode);
    }
}

void VmLowering::normalizeBoolean() {
    fixedOperand(encode(Address::Literal), 0xFFFFFFFFU, 1);
    bytes_.push_back(encode(Alu::Multiply));
}

void VmLowering::condition(const SourceExpr &expr, bool truthContext) {
    if (function_.semantics == SourceSemantics::Explicit && expr.kind == SourceExprKind::Identifier && expr.text == "eax")
        return;
    if (normalCall(expr, false, (function_.semantics == SourceSemantics::C11))) {
        if (!(function_.semantics == SourceSemantics::C11) || !truthContext || !explicitExpressionIsFloat(expr, &unit_, &function_))
            return;
        bytes_.push_back(encode(Alu::PushEax));
    } else
        expression(expr, true, truthContext);
    if ((function_.semantics == SourceSemantics::C11) && truthContext && explicitExpressionIsFloat(expr, &unit_, &function_)) {
        fixedOperand(encode(Address::Literal), 0x7FFFFFFFU, 3);
        bytes_.push_back(encode(Alu::And));
    }
    bytes_.push_back(encode(Alu::PopEax));
}

} // namespace shenmue::scenario
