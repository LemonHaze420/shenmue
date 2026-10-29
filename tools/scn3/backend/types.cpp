#include "scn3/backend/types.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

std::optional<ExplicitLocalMember> explicitLocalMember(const SourceExpr &expr, const SourceFunctionAst &function, const SourceUnit *unit) {
    SourceExpr base;
    std::string member;
    if (expr.kind == SourceExprKind::Identifier) {
        const size_t dot = expr.text.find('.');
        if (dot == std::string::npos)
            return std::nullopt;
        base = SourceExpr{.kind = SourceExprKind::Identifier, .text = expr.text.substr(0, dot)};
        member = expr.text.substr(dot + 1);
    } else if (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".") && expr.args.size() == 1) {
        base = expr.args[0];
        member = expr.text.substr(1);
    } else
        return std::nullopt;
    const std::map<std::string, std::string>::const_iterator binding = function.localTypes.find(base.text);
    if (base.kind != SourceExprKind::Identifier)
        return std::nullopt;
    const std::map<std::string, SourceDataBinding>::const_iterator workspace =
        unit ? unit->workspaceBindings.find(base.text) : std::map<std::string, SourceDataBinding>::const_iterator{};
    if (binding == function.localTypes.end() && (!unit || workspace == unit->workspaceBindings.end()))
        return std::nullopt;
    for (const SourceLocalDeclaration &declaration : function.localDeclarations)
        if (declaration.name == base.text && declaration.isArray)
            return std::nullopt;
    const ScenarioDataSchema *schema = findScenarioDataSchema(binding != function.localTypes.end() ? binding->second : workspace->second.type);
    if (schema)
        for (const ScenarioDataField &field : schema->fields)
            if (member == field.name && field.elementCount == 1)
                return ExplicitLocalMember{.base = std::move(base), .field = &field};
    return std::nullopt;
}

bool explicitExpressionIsFloat(const SourceExpr &expr, const SourceUnit *unit, const SourceFunctionAst *function) {
    if (!expr.cType.empty())
        return expr.cType == "float";
    if (function) {
        if (expr.kind == SourceExprKind::Index && expr.args.size() == 2 && expr.args[0].kind == SourceExprKind::Identifier) {
            const std::map<std::string, std::string>::const_iterator array = function->localTypes.find(expr.args[0].text);
            if (array != function->localTypes.end())
                return array->second == "float";
        }
        const std::map<std::string, std::string>::const_iterator local = function->localTypes.find(expr.text);
        if (expr.kind == SourceExprKind::Identifier && local != function->localTypes.end() && local->second == "float") {
            for (const SourceLocalDeclaration &declaration : function->localDeclarations)
                if (declaration.name == expr.text && declaration.isArray)
                    return false;
            return true;
        }
        const std::optional<ExplicitLocalMember> member = explicitLocalMember(expr, *function, unit);
        if (member)
            return member->field->type == ScenarioDataType::F32;
    }
    if (function && expr.kind == SourceExprKind::Identifier && function->localTypes.count(expr.text))
        return false;
    if (unit && expr.kind == SourceExprKind::Identifier) {
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit->workspaceBindings.find(expr.text);
        if (workspace != unit->workspaceBindings.end())
            return workspace->second.type == "float";
    }
    const bool dataMember = (unit && unit->ordinaryC && expr.kind == SourceExprKind::Identifier && unit->dataSymbols.count(expr.text)) ||
        expr.kind == SourceExprKind::Index || (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".")) ||
        (expr.kind == SourceExprKind::Identifier && expr.text.find('.') != std::string::npos);
    if (unit && dataMember) {
        const std::optional<SourceDataAddress> address = resolveSourceDataAddress(*unit, expr, true);
        if (address && !address->isArray && address->dataType == ScenarioDataType::F32)
            return true;
    }
    if (unit && function && (function->semantics == SourceSemantics::C11) && expr.kind == SourceExprKind::Call) {
        const std::map<std::string, SourceFunctionAst>::const_iterator callee = unit->functions.find(expr.text);
        if (callee != unit->functions.end() && callee->second.returnType == "float")
            return true;
        const abi::CommandSpec *native = unit->abiProfile == "s1_pc" ? abiCommandForCallName(expr.text) : nullptr;
        if (native && native->resultKind == abi::ResultKind::FloatEax)
            return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.text == "float")
        return true;
    if (expr.kind == SourceExprKind::FloatLiteral)
        return true;
    if (expr.kind == SourceExprKind::Call &&
        (expr.text == "as_float" || expr.text == "vm_int_to_float" || expr.text == "vm_fadd" || expr.text == "vm_fsub" || expr.text == "vm_fmul" ||
            expr.text == "vm_fdiv"))
        return true;
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2 && (expr.text == "+" || expr.text == "-" || expr.text == "*" || expr.text == "/"))
        return explicitExpressionIsFloat(expr.args[0], unit, function) || explicitExpressionIsFloat(expr.args[1], unit, function);
    if (expr.kind == SourceExprKind::Binary && expr.text == "=" && expr.args.size() == 2)
        return explicitExpressionIsFloat(expr.args[function && (function->semantics == SourceSemantics::C11) ? 0 : 1], unit, function);
    return expr.kind == SourceExprKind::Unary && expr.text == "-" && expr.args.size() == 1 && explicitExpressionIsFloat(expr.args[0], unit, function);
}

const SourceExpr &cSwitchSelector(const SourceExpr &selector) {

    if (selector.kind == SourceExprKind::Cast && selector.cConversion == "BitCast" && selector.args.size() == 1 &&
        selector.args[0].kind == SourceExprKind::Identifier)
        return selector.args[0];
    if (selector.kind == SourceExprKind::Cast && selector.cConversion == "IntegralCast" && selector.args.size() == 1 &&
        selector.args[0].kind == SourceExprKind::Identifier &&
        (selector.args[0].cType == "int8_t" || selector.args[0].cType == "int16_t" || selector.args[0].cType == "uint8_t" ||
            selector.args[0].cType == "uint16_t"))
        return selector.args[0];
    return selector;
}

SourceFunctionAst allocateCLocals(const SourceFunctionAst &original) {
    SourceFunctionAst function = original;
    if (!(function.semantics == SourceSemantics::C11))
        return function;
    if (function.explicitFrameAdjust || function.explicitBase || function.declaredOffset)
        throw std::runtime_error("C functions derive their own frames and placement");
    uint32_t size = 0;
    std::set<std::string> names(function.params.begin(), function.params.end());
    for (const std::pair<const std::string, std::string> &binding : function.localTypes)
        if (binding.first == "eax" || binding.first == "true" || binding.first == "false" || binding.first.starts_with("vm_") || binding.first == "as_float" ||
            binding.first == "as_word" || binding.first == "TASK_FN" || binding.first == "byte_address")
            throw std::runtime_error("Reserved backend identifier in C declaration: " + binding.first);
    if (names.size() != function.params.size())
        throw std::runtime_error("Duplicate parameter name");
    uint32_t highWater = 0;
    std::vector<std::pair<std::pair<uint32_t, uint32_t>, std::string>> liveStorage;
    const std::function<void(const SourceLocalDeclaration &)> allocate = [&](const SourceLocalDeclaration &declaration) {
        if (!names.insert(declaration.name).second)
            throw std::runtime_error("Duplicate parameter/local name: " + declaration.name);

        const size_t bracket = declaration.type.find('[');
        const std::string element = bracket == std::string::npos ? declaration.type : trimCopy(declaration.type.substr(0, bracket));
        uint32_t rows = 1;
        for (size_t open = bracket; open != std::string::npos; open = declaration.type.find('[', open + 1))
            rows *= static_cast<uint32_t>(std::stoul(declaration.type.substr(open + 1)));
        const ScenarioDataSchema *schema = findScenarioDataSchema(element);
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(element);
        const uint32_t elementWidth = element.find('*') != std::string::npos ? 4U : schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 0;
        const uint32_t elementSize = rows * elementWidth;
        const uint32_t alignment = schema ? schema->alignment : std::min(elementWidth, 4U);
        if (!elementSize || !alignment || !declaration.count || declaration.count > (32768U - size) / elementSize)
            throw std::runtime_error("Local declaration exceeds the supported VM frame");
        const uint32_t bytes = elementSize * declaration.count;
        uint32_t depth = (size + bytes + alignment - 1U) / alignment * alignment;
        const std::map<std::string, int32_t>::const_iterator pinned = function.requestedLocalOffsets.find(declaration.name);
        if (pinned != function.requestedLocalOffsets.end()) {
            if (pinned->second >= 0 || pinned->second < -32768)
                throw std::runtime_error("C automatic objects require negative frame offsets");
            depth = static_cast<uint32_t>(-pinned->second);
            if (depth < bytes || depth % alignment)
                throw std::runtime_error("C local placement violates its object size or alignment");
        }
        for (const std::pair<std::pair<uint32_t, uint32_t>, std::string> &live : liveStorage)
            if (depth - bytes < live.first.second && live.first.first < depth)
                throw std::runtime_error("C local placement overlaps a live object: " + declaration.name + " and " + live.second);
        liveStorage.push_back({{depth - bytes, depth}, declaration.name});
        size = std::max(size, depth);
        function.localOffsets[declaration.name] = 0U - depth;
        highWater = std::max(size, highWater);
    };
    const std::function<void(const std::vector<SourceStmt> &)> declarations = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Declaration) {
                allocate(statement.declaration);
                continue;
            }
            const uint32_t outer = size;
            const size_t liveOuter = liveStorage.size();
            declarations(statement.initializer);
            if (statement.kind == SourceStmtKind::Switch) {
                declarations(statement.body);
                for (const SourceSwitchCase &clause : statement.cases)
                    declarations(clause.body);
            } else {
                declarations(statement.body);
                size = outer;
                liveStorage.resize(liveOuter);
                declarations(statement.elseBody);
            }
            size = outer;
            liveStorage.resize(liveOuter);
        }
    };
    bool scoped = false;
    const std::function<void(const std::vector<SourceStmt> &)> findDeclarations = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Declaration)
                scoped = true;
            findDeclarations(statement.initializer);
            findDeclarations(statement.body);
            findDeclarations(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases)
                findDeclarations(clause.body);
        }
    };
    findDeclarations(function.body.body);
    if (scoped)
        declarations(function.body.body);
    else
        for (const SourceLocalDeclaration &declaration : function.localDeclarations)
            allocate(declaration);
    if (names.size() != function.params.size() + function.localDeclarations.size())
        throw std::runtime_error("C local declaration is missing from its lexical scope");
    const std::function<uint32_t(const SourceExpr &)> expressionScratch = [&](const SourceExpr &expr) {
        uint32_t children = 0;
        for (const SourceExpr &arg : expr.args)
            children = std::max(children, expressionScratch(arg));
        uint32_t own = expr.kind == SourceExprKind::CompoundAssign || expr.kind == SourceExprKind::Postfix ||
                (expr.kind == SourceExprKind::Unary && (expr.text == "++" || expr.text == "--")) ?
            3U :
            0U;
        if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2 &&
            (((expr.text == "==" || expr.text == "!=") && (expr.args[0].cType == "float" || expr.args[1].cType == "float")) ||
                (expr.text == ">>" && expr.args[0].cType == "uint32_t")))
            own = 2;
        if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1 && expr.cConversion != "BitCast" &&
            ((expr.text == "float" && expr.args[0].cType == "uint32_t") || (expr.text == "uint32_t" && expr.args[0].cType == "float")))
            own = 1;
        if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2 && (expr.text == "/" || expr.text == "%") &&
            (expr.args[0].cType == "uint32_t" || expr.args[1].cType == "uint32_t"))
            own = 6;
        if (expr.kind == SourceExprKind::CompoundAssign) {
            const bool unsignedComputation = expr.cConversion == "unsigned int" || expr.cConversion == "uint32_t";
            if (unsignedComputation && (expr.text == "/=" || expr.text == "%="))
                own += 6;
            else if (unsignedComputation && expr.text == ">>=")
                own += 1;
            else if (expr.args[0].cType == "uint32_t" && expr.cConversion == "float")
                own += 2;
        }
        return own + children;
    };
    const std::function<void(const std::vector<SourceStmt> &, uint32_t)> scratch = [&](const std::vector<SourceStmt> &statements, uint32_t held) {
        for (const SourceStmt &stmt : statements) {
            const bool capturedSelector = stmt.kind == SourceStmtKind::Switch &&
                (cSwitchSelector(stmt.expr).kind != SourceExprKind::Identifier || cSwitchSelector(stmt.expr).volatileAccess);
            const uint32_t depth = held + (capturedSelector ? 1U : 0U);
            function.temporaryWords = std::max(function.temporaryWords, depth + expressionScratch(stmt.expr));
            function.temporaryWords = std::max(function.temporaryWords, depth + expressionScratch(stmt.value));
            function.temporaryWords = std::max(function.temporaryWords, depth + expressionScratch(stmt.increment));
            scratch(stmt.initializer, depth);
            scratch(stmt.body, depth);
            scratch(stmt.elseBody, depth);
            for (const SourceSwitchCase &clause : stmt.cases)
                scratch(clause.body, depth);
        }
    };
    scratch(function.body.body, 0);
    size = (highWater + 3U) / 4U * 4U;
    function.temporaryBase = 0U - size;
    if (function.temporaryWords > (32768U - size) / 4U)
        throw std::runtime_error("C expression temporaries exceed the VM frame");
    size += function.temporaryWords * 4U;
    if (function.requestedFrameSize) {
        if (*function.requestedFrameSize < size)
            throw std::runtime_error("C frame reservation is smaller than its objects and temporaries");
        size = *function.requestedFrameSize;
    }
    function.explicitFrameAdjust = size <= 128U ? (0U - size) & 0xFFU : (0U - size) & 0xFFFFU;
    return function;
}

bool cExpressionIsUnsigned(const SourceExpr &expr, const SourceUnit &unit, const SourceFunctionAst &function) {
    if (!expr.cType.empty())
        return expr.cType == "uint32_t" || expr.cType.find('*') != std::string::npos;
    if (const std::optional<ExplicitLocalMember> member = explicitLocalMember(expr, function, &unit))
        return member->field->type == ScenarioDataType::U32;
    if (expr.kind == SourceExprKind::Identifier && expr.text.find('.') == std::string::npos) {
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(expr.text);
        if (type != function.localTypes.end())
            return type->second == "uint32_t";
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(expr.text);
        if (workspace != unit.workspaceBindings.end())
            return workspace->second.type == "uint32_t";
        const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(expr.text);
        if (data != unit.dataBindings.end())
            return data->second.type == "uint32_t" || data->second.type == "u32";
        return sourceConstantIsUnsigned(expr);
    }
    if (expr.kind == SourceExprKind::Index || (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".")) ||
        (expr.kind == SourceExprKind::Identifier && expr.text.find('.') != std::string::npos)) {
        const SourceExpr *root = &expr;
        while (!root->args.empty())
            root = &root->args[0];
        const std::string name = root->text.substr(0, root->text.find('.'));
        if (!function.localTypes.count(name)) {
            const std::optional<SourceDataAddress> data = resolveSourceDataAddress(unit, expr, true);
            if (data && !data->isArray)
                return data->dataType == ScenarioDataType::U32;
        }
    }
    if (expr.kind == SourceExprKind::Cast && (expr.text == "uint32_t" || expr.text == "int32_t" || expr.text == "float"))
        return expr.text == "uint32_t";
    if (expr.kind == SourceExprKind::IntLiteral)
        return !expr.text.starts_with("-") && (expr.text.ends_with("u") || expr.text.ends_with("U") || parseSourceIntegerLiteral(expr.text) > 0x7FFFFFFFU);
    if (expr.kind == SourceExprKind::Index && expr.args.size() == 2)
        return cExpressionIsUnsigned(expr.args[0], unit, function);
    if (expr.kind == SourceExprKind::Call) {
        const std::map<std::string, SourceFunctionAst>::const_iterator callee = unit.functions.find(expr.text);
        return callee != unit.functions.end() && callee->second.returnType == "uint32_t";
    }
    if (expr.kind == SourceExprKind::Unary && expr.args.size() == 1 && (expr.text == "-" || expr.text == "+" || expr.text == "~"))
        return cExpressionIsUnsigned(expr.args[0], unit, function);
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2) {
        if (expr.text == "=")
            return cExpressionIsUnsigned(expr.args[0], unit, function);
        if (expr.text == "==" || expr.text == "!=" || expr.text == "<" || expr.text == "<=" || expr.text == ">" || expr.text == ">=")
            return false;
        return cExpressionIsUnsigned(expr.args[0], unit, function) || cExpressionIsUnsigned(expr.args[1], unit, function);
    }
    return false;
}

} // namespace shenmue::scenario
