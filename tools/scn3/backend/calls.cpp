#include "scn3/backend/lowering.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/lookup.hpp"

#include <iostream>
#include <mutex>
#include <set>

namespace shenmue::scenario {

void VmLowering::checkNativeArguments(const SourceExpr &expr, const NativeArgumentLayout &layout) {
    std::vector<std::optional<uint32_t>> constants;
    for (const SourceExpr &argument : expr.args)
        constants.push_back(sourceDataConstantValue(argument, true));

    static std::set<std::string> reported;
    static std::mutex reporting;
    const auto underSupplied = [&](const std::string &problem) {
        const std::lock_guard<std::mutex> lock(reporting);
        if (reported.insert(expr.text + problem).second)
            std::cerr << "warning: " << expr.text << problem << "\n";
    };
    if (layout.variadicOffset >= 0 && static_cast<uint32_t>(layout.variadicOffset) > expr.args.size() * 4U)
        underSupplied(" is missing fixed arguments before its variadic tail");
    const bool variadicWords = layout.guards.empty() && layout.pointers.empty() && layout.floatArguments.empty() && layout.callbackOffset < 0;
    for (const NativeParameterRead &read : layout.reads) {
        bool possible = true;
        bool resolved = true;
        for (const NativeModeCondition &guard : read.guards) {
            const size_t index = guard.byteOffset / 4U;
            if (index >= constants.size() || !constants[index])
                resolved = false;
            else if ((*constants[index] != guard.value) != guard.negate)
                possible = false;
        }
        if (possible && (resolved || !variadicWords) && (static_cast<uint64_t>(read.byteOffset) + read.byteSize + 3U) / 4U > expr.args.size())
            underSupplied(" reads beyond its supplied C arguments");
    }
    for (const NativePointerParameter &pointer : layout.pointers) {
        const size_t index = pointer.byteOffset / 4U;
        if (index >= expr.args.size())
            fail("native pointer argument is missing");
        const SourceExpr &argument = expr.args[index];
        const std::optional<uint32_t> zero = sourceDataConstantValue(argument, true);
        if (zero && *zero == 0)
            continue;
        std::string type = argument.cType;
        if (type.empty() && argument.kind == SourceExprKind::Unary && argument.text == "&" && argument.args.size() == 1) {
            const std::map<std::string, std::string>::const_iterator local = function_.localTypes.find(argument.args[0].text);
            if (local != function_.localTypes.end())
                type = local->second + " *";
            else {
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit_.workspaceBindings.find(argument.args[0].text);
                if (workspace != unit_.workspaceBindings.end())
                    type = workspace->second.type + " *";
            }
        }
        const size_t star = type.rfind('*');
        if (star == std::string::npos)
            fail("native argument requires a typed pointer to " + std::string(pointer.type));
        type = trimCopy(type.substr(0, star));
        const bool readOnly = type.starts_with("const ");
        if (readOnly)
            type.erase(0, 6);
        if ((type != pointer.type && std::string_view(pointer.type) != "void") || (readOnly && !pointer.readOnly))
            fail("native pointer argument has an incompatible pointee type");
    }
    if (expr.args.empty())
        fail("integer native layout requires its first word");
    for (size_t index = 0; index < expr.args.size(); ++index) {
        bool floating = false;
        for (uint32_t offset : layout.floatArguments)
            if (offset == index * 4U)
                floating = true;
        const std::string &type = expr.args[index].cType;
        if (!type.empty() && type != "int32_t" && type != "uint32_t" && type != "float" && type.find('*') == std::string::npos)
            fail("Native C arguments require a 32-bit value or pointer");

        const bool tail = (layout.variadicOffset >= 0 && index * 4U >= static_cast<uint32_t>(layout.variadicOffset)) ||
            (layout.tailOffset >= 0 && index * 4U >= static_cast<uint32_t>(layout.tailOffset));
        if (!tail && explicitExpressionIsFloat(expr.args[index], &unit_, &function_) != floating)
            fail("native float arguments require fixed parameter types");
    }
}

bool VmLowering::normalCall(const SourceExpr &expr, bool pushResult, bool needsValue) {
    if (expr.kind != SourceExprKind::Call)
        return false;

    if (function_.semantics == SourceSemantics::C11 && expr.text == "SCNT_ShowSubtitle" && !unit_.functions.count(expr.text)) {
        if (needsValue)
            fail("SCNT_ShowSubtitle has no value");
        const SourceExpr *text = expr.args.size() == 1 ? &expr.args[0] : nullptr;
        if (text && text->kind == SourceExprKind::Unary && text->text == "&" && text->args.size() == 1)
            text = &text->args[0];
        if (!text || text->kind != SourceExprKind::Identifier || !unit_.dataSymbols.count(text->text))
            fail("SCNT_ShowSubtitle requires a constant text object");
        SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = text->text}}};
        operand(encode(Control::ScnfOffset), &encoded);
        return true;
    }
    if ((function_.semantics == SourceSemantics::Explicit || !unit_.functions.count(expr.text)) &&
        (expr.text.starts_with("vm_") || expr.text == "as_float" || expr.text == "as_word"))
        return false;
    const std::map<std::string, SourceFunctionAst>::const_iterator local = unit_.functions.find(expr.text);
    const abi::CommandSpec *native = unit_.abiProfile == "s1_pc" ? abiCommandForCallName(expr.text) : nullptr;
    if (local == unit_.functions.end() && !native)
        return false;
    if ((function_.semantics == SourceSemantics::C11) && function_.localTypes.count(expr.text))
        fail("local object is not callable");

    if (cUnit() && native && local == unit_.functions.end()) {
        SourceExpr words = expr;
        bool promoted = false;
        for (SourceExpr &argument : words.args)
            if (argument.kind == SourceExprKind::Cast && argument.cConversion == "FloatingCast" && argument.cType == "double" && argument.args.size() == 1 &&
                argument.args[0].cType == "float") {
                SourceExpr value = std::move(argument.args[0]);
                argument = std::move(value);
                promoted = true;
            }
        if (promoted)
            return normalCall(words, pushResult, needsValue);
    }
    if ((function_.semantics == SourceSemantics::C11) && needsValue &&
        ((local != unit_.functions.end() && local->second.returnType == "void") || (native && native->resultKind == abi::ResultKind::Void)))
        fail("void call cannot supply a value");
    if (cUnit() && native) {
        std::vector<std::optional<uint32_t>> constants;
        for (const SourceExpr &argument : expr.args)
            constants.push_back(sourceDataConstantValue(argument, true));
        const NativeArgumentLayout *layout =
            native->handlerName && !nativeCallIgnoresParameters(native->handlerName) ? nativeArgumentLayout(native->handlerName, constants) : nullptr;
        if (layout)
            checkNativeArguments(expr, *layout);
        else if (!native->handlerName || !nativeCallIgnoresParameters(native->handlerName))
            fail("Complete typed native C parameter lowering is not established for " + expr.text);
        else
            for (const SourceExpr &argument : expr.args) {
                if (explicitExpressionIsFloat(argument, &unit_, &function_) ||
                    (!argument.cType.empty() && argument.cType != "int32_t" && argument.cType != "uint32_t" && argument.cType.find('*') == std::string::npos))
                    fail("Ignored native arguments must retain one 32-bit integer or pointer after C argument promotion");
            }
    }
    if (native && native->family == abi::Family::SCNF && native->dispatchSubslot != 0)
        fail("SCNF secondary handlers are not callable entrypoints");
    const size_t fixed = local == unit_.functions.end() ? 0 : local->second.fixedParameterCount.value_or(local->second.params.size());
    const bool typedCallee = local != unit_.functions.end() && local->second.semantics == SourceSemantics::C11;
    if (typedCallee && (expr.args.size() < fixed || (!local->second.variadic && expr.args.size() != fixed)))
        fail("C call argument count does not match its declaration");
    for (size_t i = expr.args.size(); i > 0; --i) {

        const SourceExpr &argument = typedCallee && i <= fixed && expr.args[i - 1].kind == SourceExprKind::Cast &&
                expr.args[i - 1].cConversion == "IntegralCast" && expr.args[i - 1].args.size() == 1 &&
                (expr.args[i - 1].cType == "int8_t" || expr.args[i - 1].cType == "int16_t") ?
            expr.args[i - 1].args[0] :
            expr.args[i - 1];
        if (typedCallee && i <= fixed)
            assignedValue(argument, local->second.localTypes.at(local->second.params[i - 1]) == "float",
                local->second.localTypes.at(local->second.params[i - 1]) == "uint32_t");
        else {
            if (typedCallee && local->second.variadic && explicitExpressionIsFloat(expr.args[i - 1], &unit_, &function_))
                fail("C variadic float arguments promote to double, which has no VM word representation");
            expression(expr.args[i - 1]);
        }
    }
    if (local != unit_.functions.end()) {
        SourceExpr target{.kind = SourceExprKind::Identifier, .text = expr.text};
        SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {std::move(target)}};
        operand(encode(Control::LocalCall), &encoded);
    } else {
        const abi::FamilySpec *family = abi::findFamily(native->family);
        if (native->family == abi::Family::SCNF)
            fixedOperand(encode(Control::LatchOperand), static_cast<uint32_t>(expr.args.size()), 1);
        fixedOperand(family->opcodeFamily, native->dispatchIndex, native->dispatchIndex < 128U ? 1 : 2);
    }
    const uint32_t cleanup = static_cast<uint32_t>(expr.args.size()) * 4U;
    if (cleanup)
        fixedOperand(encode(Control::AdjustStack), cleanup, literalWidth(cleanup));
    if (pushResult)
        bytes_.push_back(encode(Alu::PushEax));
    return true;
}

} // namespace shenmue::scenario
