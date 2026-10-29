#include "scn3/backend/types.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/lookup.hpp"
#include "scn3/native/motion.hpp"

namespace shenmue::scenario::decompile {

uint8_t naturalWidth(uint32_t value) {
    const int32_t signedValue = static_cast<int32_t>(value);
    return signedValue >= -128 && signedValue <= 127 ? 1 : signedValue >= -32768 && signedValue <= 32767 ? 2 : 3;
}

SourceExpr friendlyExpression(const SourceExpr &original) {
    if (original.kind == SourceExprKind::Cast && original.args.size() == 1 && original.args[0].kind == SourceExprKind::IntLiteral) {
        const std::optional<uint8_t> width = sourceImmediateWidthCallName(original.text);
        uint32_t value = parseSourceIntegerLiteral(original.args[0].text);
        if (width == 1)
            value = static_cast<uint32_t>(static_cast<int8_t>(value));
        if (width == 2)
            value = static_cast<uint32_t>(static_cast<int16_t>(value));
        if (width && naturalWidth(value) == *width)
            return signedNumber(value);
    }
    if (original.kind == SourceExprKind::Call) {
        const Operation *op = operationNamed(original.text);
        if (op && op->opcode >= encode(Address::Task8) && op->opcode <= encode(Address::LocalAddress) && original.args.size() == 1) {
            const SourceExpr &encoded = original.args[0];
            if (encoded.kind == SourceExprKind::Cast && encoded.args.size() == 1 && encoded.args[0].kind == SourceExprKind::IntLiteral) {
                const uint32_t offset = parseSourceIntegerLiteral(encoded.args[0].text);
                const bool local = op->opcode >= encode(Address::Local8);
                const uint8_t width = local && offset <= 0xFFU ? 1 : offset <= 0xFFFFU ? 2 : 3;
                const std::optional<uint8_t> encodedWidth = sourceImmediateWidthCallName(encoded.text);
                if (encodedWidth == width) {
                    const uint8_t kind = addressAccess(addressOperation(op->opcode));
                    std::string name = (local ? "local_" : "task_") + hexName(offset);
                    if (local && kind == 2 && offset >= 8 && offset < 0x80 && offset % 4 == 0)
                        name = "arg" + std::to_string((offset - 8U) / 4U);
                    if (kind == 0)
                        name += ".u8";
                    if (kind == 1)
                        name += ".u16";
                    SourceExpr slot = identifier(std::move(name));
                    return kind == 3 ? SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {std::move(slot)}} : slot;
                }
            }
        }
        if (original.text == "vm_base_rel3" && original.args.size() == 1) {
            const SourceExpr &encoded = original.args[0];
            if (encoded.kind == SourceExprKind::Cast && encoded.text == "i32" && encoded.args.size() == 1 &&
                encoded.args[0].kind == SourceExprKind::Identifier) {
                if (encoded.args[0].text.starts_with("script_"))
                    return call("TASK_FN", {encoded.args[0]});
                if (encoded.args[0].text.starts_with("data_") || encoded.args[0].text.starts_with("str_") || encoded.args[0].text.starts_with("pad_"))
                    return encoded.args[0];
            }
        }
        SourceExpr result = original;

        const size_t firstValue = op && op->opcode < kAluClass ? 1 : 0;
        for (size_t i = firstValue; i < result.args.size(); ++i)
            result.args[i] = friendlyExpression(result.args[i]);
        const abi::CommandSpec *command = original.text.starts_with("vm_") ? nullptr : abiCommandForCallName(original.text);
        if (command) {
            std::vector<AbiValue> observedArgs(result.args.size());
            for (size_t i = 0; i < result.args.size(); ++i) {
                if (const std::optional<uint32_t> value = sourceDataConstantValue(result.args[i])) {
                    observedArgs[i].hasConst = true;
                    observedArgs[i].constValue = *value;
                    observedArgs[i].literalWidth = naturalWidth(*value);
                }
            }
            std::vector<std::optional<uint32_t>> nativeValues;
            for (const AbiValue &observed : observedArgs)
                nativeValues.push_back(observed.hasConst ? std::optional<uint32_t>(observed.constValue) : std::nullopt);
            for (size_t i = 0; i < result.args.size(); ++i) {
                if (!observedArgs[i].hasConst)
                    continue;
                const uint32_t value = observedArgs[i].constValue;
                const NativeArgumentType *native = command->handlerName ? nativeArgumentForHandler(command->handlerName, i, nativeValues) : nullptr;
                if (native && !native->pointer && std::string_view(native->type) == "float" && naturalWidth(value) == 3) {
                    result.args[i] = typedFloatLiteral(result.args[i]);
                    continue;
                }
                const abi::ArgSpec *arg = abiCallArgSpecForSourceArg(original.text, i);
                if (((arg && arg->kind == abi::ArgKind::CharaId) ||
                        abiCallArgHasRule(original.text, observedArgs, i, abi::RuleKind::ArgIsCharaId, abi::RuleDomain::Both)) &&
                    naturalWidth(value) == 3) {
                    std::string name = "CHARA_";
                    bool valid = true;
                    for (uint32_t byte = 0; byte < 4; ++byte) {
                        const char ch = static_cast<char>(value >> (byte * 8U));
                        valid = valid && ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
                        name += ch;
                    }
                    if (valid)
                        result.args[i] = identifier(name);
                } else if (arg) {
                    if (const std::optional<std::string_view> name = lookupAbiArgValueName(*arg, observedArgs[i]))
                        result.args[i] = identifier(std::string(*name));
                } else if (native && !native->pointer) {

                    const std::optional<std::string_view> name =
                        std::string_view(native->type) == "MotionId" ? motionName(value) : lookupAbiValueName(abiValueTableNamed(native->type), value);
                    if (name)
                        result.args[i] = identifier(std::string(*name));
                }
            }
        }
        if (original.text == "vm_load32" && result.args.size() == 1)
            return SourceExpr{.kind = SourceExprKind::Unary, .text = "*", .args = std::move(result.args)};
        if (original.text == "vm_mem32" && result.args.size() == 1)
            return SourceExpr{.kind = SourceExprKind::Unary, .text = "*", .args = std::move(result.args)};
        if (original.text == "vm_not" && result.args.size() == 1) {
            if (explicitExpressionIsFloat(result.args[0]))
                result.args[0] = call("as_word", {std::move(result.args[0])});
            return SourceExpr{.kind = SourceExprKind::Unary, .text = "~", .args = std::move(result.args)};
        }
        return result;
    }
    SourceExpr result = original;
    for (SourceExpr &arg : result.args)
        arg = friendlyExpression(arg);
    return result;
}

void functionReferences(SourceExpr &expr, const SourceUnit &unit) {
    bool label = expr.kind == SourceExprKind::Identifier && expr.text.size() > 1 && expr.text[0] == 'L';
    for (size_t index = 1; label && index < expr.text.size(); ++index)
        label = (expr.text[index] >= '0' && expr.text[index] <= '9') || (expr.text[index] >= 'A' && expr.text[index] <= 'F');
    if (label && expr.text.size() <= 9) {
        const uint32_t offset = static_cast<uint32_t>(std::stoul(expr.text.substr(1), nullptr, 16));
        for (const std::pair<const std::string, SourceFunctionAst> &entry : unit.functions)
            if (entry.second.offset == offset && expr.text == "L" + hexName(offset, 6)) {
                expr.text = entry.first;
                break;
            }
    }
    for (SourceExpr &arg : expr.args)
        functionReferences(arg, unit);
}

void functionReferences(std::vector<SourceStmt> &stmts, const SourceUnit &unit) {
    for (SourceStmt &stmt : stmts) {
        functionReferences(stmt.expr, unit);
        functionReferences(stmt.value, unit);
        functionReferences(stmt.body, unit);
        functionReferences(stmt.elseBody, unit);
        for (SourceSwitchCase &clause : stmt.cases) {
            functionReferences(clause.value, unit);
            functionReferences(clause.body, unit);
        }
    }
}

void collectParameters(const SourceExpr &expr, uint32_t &count) {
    if (const std::optional<uint32_t> slot = argSlotOffsetFromName(expr.text)) {
        const uint32_t index = (*slot - 8U) / 4U;
        if (index < 128 && index >= count)
            count = index + 1;
    }
    for (const SourceExpr &arg : expr.args)
        collectParameters(arg, count);
}

void collectParameters(const std::vector<SourceStmt> &stmts, uint32_t &count) {
    for (const SourceStmt &stmt : stmts) {
        collectParameters(stmt.expr, count);
        collectParameters(stmt.value, count);
        collectParameters(stmt.body, count);
        collectParameters(stmt.elseBody, count);
        for (const SourceSwitchCase &clause : stmt.cases) {
            collectParameters(clause.value, count);
            collectParameters(clause.body, count);
        }
    }
}

void friendlyStatements(std::vector<SourceStmt> &stmts) {
    for (SourceStmt &stmt : stmts) {
        if (stmt.kind != SourceStmtKind::If || stmt.expr.kind != SourceExprKind::Unary)
            stmt.expr = friendlyExpression(stmt.expr);
        stmt.value = friendlyExpression(stmt.value);
        friendlyStatements(stmt.body);
        friendlyStatements(stmt.elseBody);
        for (SourceSwitchCase &clause : stmt.cases) {
            clause.value = friendlyExpression(clause.value);
            friendlyStatements(clause.body);
        }
    }
}

} // namespace shenmue::scenario::decompile
