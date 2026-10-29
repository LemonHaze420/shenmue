#include "scn3/backend/types.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario::decompile {

SourceExpr typedFloatLiteral(const SourceExpr &value) {
    const std::optional<uint32_t> bits = sourceDataConstantValue(value);
    if (bits && naturalWidth(*bits) == 3 && value.kind != SourceExprKind::Cast) {
        const std::string literal = formatFloatLiteral(*bits);
        const std::optional<SourceExpr> parsed = parseExpressionText(literal);
        if (parsed && explicitExpressionIsFloat(*parsed) && floatLiteralTextRoundTrips(literal, *bits))
            return *parsed;
    }
    return call("as_float", {value});
}

void recoverCCallSignatures(SourceUnit &unit) {
    std::map<std::string, std::set<size_t>> counts;
    const std::function<void(const SourceExpr &, size_t)> record = [&](const SourceExpr &target, size_t count) {
        const SourceExpr *symbol = &target;
        if (target.kind == SourceExprKind::Call && target.text == "TASK_FN" && target.args.size() == 1)
            symbol = &target.args[0];
        if (symbol->kind == SourceExprKind::Identifier && unit.functions.count(symbol->text))
            counts[symbol->text].insert(count);
    };
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expr) {
            if (expr.kind != SourceExprKind::Call)
                return;
            if (unit.functions.count(expr.text))
                counts[expr.text].insert(expr.args.size());
            const abi::CommandSpec *command = abiCommandForCallName(expr.text);
            if (!command || !command->handlerName)
                return;
            std::vector<std::optional<uint32_t>> constants;
            for (const SourceExpr &argument : expr.args)
                constants.push_back(sourceDataConstantValue(argument));
            const NativeArgumentLayout *layout = nativeArgumentLayout(command->handlerName, constants);
            if (!layout || layout->callbackOffset < 0 || layout->countOffset < 0)
                return;
            const size_t callback = static_cast<size_t>(layout->callbackOffset) / 4U;
            const size_t count = static_cast<size_t>(layout->countOffset) / 4U;
            if (callback < expr.args.size() && count < constants.size() && constants[count])
                record(expr.args[callback], *constants[count]);
        });
    for (const std::pair<const std::string, std::set<size_t>> &observed : counts) {
        SourceFunctionAst &function = unit.functions.at(observed.first);
        const size_t count = *observed.second.begin();
        if (count < function.params.size())
            function.fixedParameterCount = count;

        if ((observed.second.size() > 1 || function.fixedParameterCount) && !count) {
            function.fixedParameterCount.reset();
            continue;
        }
        while (function.params.size() < count) {
            const std::string parameter = "arg" + std::to_string(function.params.size());
            function.params.push_back(parameter);
            function.localTypes.emplace(parameter, "int32_t");
        }
        function.variadic = observed.second.size() > 1 || function.fixedParameterCount.has_value();
    }
}

std::optional<std::pair<std::string, uint32_t>> dereferencedParameter(const SourceExpr &expression, const SourceFunctionAst &function) {
    if (expression.kind != SourceExprKind::Unary || expression.text != "*" || expression.args.size() != 1)
        return std::nullopt;
    const SourceExpr *address = &expression.args[0];
    uint32_t offset = 0;
    if (address->kind == SourceExprKind::Binary && address->text == "+" && address->args.size() == 2) {
        const std::optional<uint32_t> constant = sourceDataConstantValue(address->args[1]);
        if (!constant)
            return std::nullopt;
        offset = *constant;
        address = &address->args[0];
    }
    if (address->kind != SourceExprKind::Identifier)
        return std::nullopt;
    for (const std::string &parameter : function.params)
        if (parameter == address->text)
            return std::pair{parameter, offset};
    return std::nullopt;
}

const ScenarioDataField *recordWord(const ScenarioDataSchema &schema, uint32_t offset) {
    for (const ScenarioDataField &field : schema.fields)
        if (field.offset == offset && field.elementCount == 1 && scenarioDataTypeSize(field.type) == 4)
            return &field;
    return nullptr;
}

namespace {

struct PointerUses {
    std::map<std::string, uint32_t> uses, forwarded;
    std::map<std::string, std::set<std::string>> pointees;
    std::set<std::string> writable;
    std::map<std::string, std::vector<uint32_t>> fields;
    struct PassedOn {
        std::string name;
        std::string callee;
        size_t index;
    };
    std::vector<PassedOn> passedOn;
};

PointerUses collectPointerUses(SourceFunctionAst &function, const SourceUnit &unit, std::set<std::string> &callees) {
    PointerUses uses;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier)
            ++uses.uses[expression.text];

        if (expression.kind == SourceExprKind::Binary && (expression.text == "==" || expression.text == "!=") && expression.args.size() == 2)
            for (size_t side = 0; side < 2; ++side) {
                const std::optional<uint32_t> zero = sourceDataConstantValue(expression.args[1 - side]);
                if (expression.args[side].kind == SourceExprKind::Identifier && zero && *zero == 0)
                    ++uses.forwarded[expression.args[side].text];
            }
        if (const std::optional<std::pair<std::string, uint32_t>> field = dereferencedParameter(expression, function)) {
            ++uses.forwarded[field->first];
            uses.fields[field->first].push_back(field->second);
        }
        if (expression.kind != SourceExprKind::Call)
            return;
        const abi::CommandSpec *command = abiCommandForCallName(expression.text);
        std::vector<std::optional<uint32_t>> constants;
        for (const SourceExpr &argument : expression.args)
            constants.push_back(sourceDataConstantValue(argument));
        const NativeArgumentLayout *layout = command && command->handlerName ? nativeArgumentLayout(command->handlerName, constants) : nullptr;
        const bool script = unit.functions.count(expression.text) != 0;
        if (script)
            callees.insert(expression.text);
        for (size_t index = 0; index < expression.args.size(); ++index) {
            const SourceExpr &argument = expression.args[index];
            if (argument.kind != SourceExprKind::Identifier)
                continue;
            if (!layout && script) {
                uses.passedOn.push_back({.name = argument.text, .callee = expression.text, .index = index});
                continue;
            }
            std::string type;
            if (layout)
                for (const NativePointerParameter &pointer : layout->pointers)
                    if (pointer.byteOffset == index * 4U)
                        type = std::string(pointer.readOnly ? "const " : "") + pointer.type + " *";
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            if (type.empty() && prototype && prototype->parameters.find("...") != std::string_view::npos &&
                index >= nativeParameterTypes(command->handlerName).size()) {
                ++uses.forwarded[argument.text];
                continue;
            }
            if (type.empty() || type.find('*') != type.rfind('*'))
                continue;
            ++uses.forwarded[argument.text];
            const bool readOnly = type.starts_with("const ");
            uses.pointees[argument.text].insert(readOnly ? type.substr(6) : type);
            if (!readOnly)
                uses.writable.insert(argument.text);
        }
    });
    const std::function<void(const std::vector<SourceStmt> &)> stores = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Assign)
                if (const std::optional<std::pair<std::string, uint32_t>> field = dereferencedParameter(statement.expr, function))
                    uses.writable.insert(field->first);
            if (statement.kind == SourceStmtKind::Assign && statement.value.kind == SourceExprKind::Identifier &&
                statement.expr.kind == SourceExprKind::Identifier && taskSlotRefFromName(statement.expr.text)) {
                const std::map<std::string, SourceDataBinding>::const_iterator target = unit.workspaceBindings.find(statement.expr.text);
                const bool untyped = target == unit.workspaceBindings.end() || (!target->second.isArray && target->second.type == "int32_t");
                if (untyped || (target->second.pointer && !target->second.pointerType.empty())) {
                    ++uses.forwarded[statement.value.text];
                    if (!untyped)
                        uses.pointees[statement.value.text].insert(
                            target->second.pointerType.starts_with("const ") ? target->second.pointerType.substr(6) : target->second.pointerType);
                }
            }
            stores(statement.body);
            stores(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases)
                stores(clause.body);
        }
    };
    stores(function.body.body);
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Binary && expression.text == "=" && expression.args.size() == 2)
            if (const std::optional<std::pair<std::string, uint32_t>> field = dereferencedParameter(expression.args[0], function))
                uses.writable.insert(field->first);
    });
    return uses;
}

void passedRecord(const SourceFunctionAst &caller, const SourceExpr &argument, const SourceUnit &unit, std::set<std::string> &records) {
    std::string record;
    if (argument.kind == SourceExprKind::Unary && argument.text == "&" && argument.args.size() == 1 && argument.args[0].kind == SourceExprKind::Identifier) {
        const std::map<std::string, std::string>::const_iterator local = caller.localTypes.find(argument.args[0].text);
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(argument.args[0].text);
        if (local != caller.localTypes.end() && (findScenarioDataSchema(local->second) || local->second == "float"))
            record = local->second + " *";
        else if (local == caller.localTypes.end() && workspace != unit.workspaceBindings.end() && !workspace->second.isArray &&
            findScenarioDataSchema(workspace->second.type))
            record = workspace->second.type + " *";
        const bool untyped = local != caller.localTypes.end() ?
            local->second == "int32_t" :
            workspace == unit.workspaceBindings.end() || (!workspace->second.isArray && workspace->second.type == "int32_t");
        if (record.empty() && untyped)
            return;
    } else if (argument.kind == SourceExprKind::Identifier) {
        const std::map<std::string, std::string>::const_iterator local = caller.localTypes.find(argument.text);
        const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(argument.text);
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(argument.text);
        bool parameter = false;
        for (const std::string &own : caller.params)
            parameter |= own == argument.text;
        if (parameter && (local == caller.localTypes.end() || local->second == "int32_t"))
            return;
        if (local != caller.localTypes.end() && local->second.find('*') != std::string::npos)
            record = local->second.starts_with("const ") ? local->second.substr(6) : local->second;
        else if (local == caller.localTypes.end() && data != unit.dataBindings.end() && data->second.isArray && !data->second.pointer)
            record = (data->second.type == "u32" ? std::string("uint32_t") : data->second.type) + " *";
        else if (local == caller.localTypes.end() && workspace != unit.workspaceBindings.end() && workspace->second.isArray &&
            workspace->second.type.find('[') == std::string::npos)
            record = workspace->second.type + " *";
    }
    records.insert(record);
}

} // namespace

void recoverCForwardedPointerParameters(SourceUnit &unit) {
    std::map<std::string, PointerUses> summaries;
    std::map<std::string, std::set<std::string>> callees, callers;
    std::map<std::string, std::vector<std::pair<const SourceFunctionAst *, const SourceExpr *>>> calls;
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        summaries.emplace(name, collectPointerUses(function, unit, callees[name]));
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Call && unit.functions.count(expression.text))
                calls[expression.text].emplace_back(&function, &expression);
        });
        for (const std::string &callee : callees[name])
            callers[callee].insert(name);
    }
    const auto resolve = [&](const std::string &name) {
        SourceFunctionAst &function = unit.functions.at(name);
        const PointerUses &summary = summaries.at(name);
        std::map<std::string, uint32_t> forwarded = summary.forwarded;
        std::map<std::string, std::set<std::string>> pointees = summary.pointees;
        std::set<std::string> writable = summary.writable;
        for (const PointerUses::PassedOn &passed : summary.passedOn) {
            const SourceFunctionAst &callee = unit.functions.at(passed.callee);
            if (passed.index >= callee.params.size())
                continue;
            const std::map<std::string, std::string>::const_iterator parameter = callee.localTypes.find(callee.params[passed.index]);
            if (parameter == callee.localTypes.end() || parameter->second.find('*') == std::string::npos ||
                parameter->second.find('*') != parameter->second.rfind('*'))
                continue;
            ++forwarded[passed.name];
            const bool readOnly = parameter->second.starts_with("const ");
            pointees[passed.name].insert(readOnly ? parameter->second.substr(6) : parameter->second);
            if (!readOnly)
                writable.insert(passed.name);
        }
        bool changed = false;
        for (size_t position = 0; position < function.params.size(); ++position) {
            const std::string &parameter = function.params[position];
            const uint32_t used = summary.uses.count(parameter) ? summary.uses.at(parameter) : 0;
            const std::vector<uint32_t> &fields = summary.fields.count(parameter) ? summary.fields.at(parameter) : std::vector<uint32_t>{};

            std::optional<std::set<std::string>> passed;
            for (const std::pair<const SourceFunctionAst *, const SourceExpr *> &call : calls[name])
                if (position < call.second->args.size())
                    passedRecord(*call.first, call.second->args[position], unit, passed ? *passed : passed.emplace());
            std::set<std::string> records;
            bool other = false;
            if (passed)
                for (const std::string &record : *passed) {
                    const std::string element = record.empty() ? std::string() : trimCopy(record.substr(0, record.rfind('*')));
                    if (findScenarioDataSchema(element))
                        records.insert(record);
                    else
                        other |= element != "int32_t" && element != "uint32_t" && element != "float";
                }
            std::set<std::string> &candidates = pointees[parameter];
            if (candidates.empty() && !fields.empty() && passed && passed->size() == 1 && !passed->begin()->empty())
                candidates.insert(*passed->begin());
            else if (candidates.empty() && !fields.empty() && records.size() == 1 && !other)
                candidates.insert(*records.begin());
            if (!used || used != forwarded[parameter] || candidates.size() != 1)
                continue;
            const std::string &pointee = *candidates.begin();

            const std::string element = trimCopy(pointee.substr(0, pointee.rfind('*')));
            const ScenarioDataSchema *record = findScenarioDataSchema(element);
            const bool word = element == "int32_t" || element == "uint32_t" || element == "float";
            bool fieldsFit = true;
            for (const uint32_t offset : fields)
                fieldsFit &= record ? recordWord(*record, offset) != nullptr : word && offset % 4U == 0;
            if (!fieldsFit)
                continue;
            std::string type = (writable.count(parameter) ? "" : "const ") + pointee;
            const std::map<std::string, std::string>::const_iterator existing = function.localTypes.find(parameter);
            if (existing != function.localTypes.end() && existing->second != "int32_t") {
                const std::string previous = existing->second.starts_with("const ") ? existing->second.substr(6) : existing->second;
                if (previous != pointee)
                    continue;
                if (!existing->second.starts_with("const "))
                    type = pointee;
            }
            if (existing == function.localTypes.end() || existing->second != type) {
                function.localTypes[parameter] = type;
                changed = true;
            }
        }
        return changed;
    };
    std::vector<std::string> pending(unit.functionOrder.rbegin(), unit.functionOrder.rend());
    std::set<std::string> queued(unit.functionOrder.begin(), unit.functionOrder.end());
    while (!pending.empty()) {
        const std::string name = pending.back();
        pending.pop_back();
        queued.erase(name);
        if (!resolve(name))
            continue;
        for (const std::set<std::string> *related : {&callers[name], &callees[name]})
            for (const std::string &other : *related)
                if (queued.insert(other).second)
                    pending.insert(pending.begin(), other);
    }
}

void recoverCRecordAccesses(SourceFunctionAst &function, const SourceUnit &unit) {
    const std::function<const ScenarioDataSchema *(const std::string &)> schemaFor = [&](const std::string &name) {
        const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(name);
        if (local != function.localTypes.end())
            return findScenarioDataSchema(local->second);
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(name);
        return workspace == unit.workspaceBindings.end() ? nullptr : findScenarioDataSchema(workspace->second.type);
    };
    const std::function<void(SourceExpr &, bool)> rewrite = [&](SourceExpr &expression, bool addressContext) {
        if (const std::optional<std::pair<std::string, uint32_t>> field = dereferencedParameter(expression, function)) {
            const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(field->first);
            std::string pointee = type == function.localTypes.end() || type->second.find('*') == std::string::npos ?
                "" :
                trimCopy(type->second.substr(0, type->second.rfind('*')));
            if (pointee.starts_with("const "))
                pointee.erase(0, 6);
            const ScenarioDataSchema *record = findScenarioDataSchema(pointee);
            if (!record && (pointee == "int32_t" || pointee == "uint32_t" || pointee == "float") && field->second % 4U == 0 && field->second) {
                expression = SourceExpr{.kind = SourceExprKind::Index, .args = {identifier(field->first), number(field->second / 4U)}, .cType = pointee};
                return;
            }
            if (const ScenarioDataField *member = record ? recordWord(*record, field->second) : nullptr) {
                SourceExpr object{.kind = SourceExprKind::Unary, .text = "*", .args = {identifier(field->first)}, .cType = record->name};
                expression = SourceExpr{.kind = SourceExprKind::Cast,
                    .text = "." + std::string(member->name),
                    .args = {std::move(object)},
                    .cType = member->type == ScenarioDataType::F32 ? "float" :
                        member->type == ScenarioDataType::U32      ? "uint32_t" :
                                                                     "int32_t"};
                return;
            }
        }
        if (expression.kind == SourceExprKind::Identifier && !addressContext) {
            const ScenarioDataSchema *schema = schemaFor(expression.text);
            if (schema)
                for (const ScenarioDataField &field : schema->fields)
                    if (field.offset == 0 && scenarioDataTypeSize(field.type) == 4 && field.elementCount == 1) {
                        expression.text += "." + std::string(field.name);
                        return;
                    }
        }
        const uint32_t accessWidth = expression.kind == SourceExprKind::Unary && expression.text == "*"                  ? 4U :
            expression.kind == SourceExprKind::Call && (expression.text == "vm_load8" || expression.text == "vm_mem8")   ? 1U :
            expression.kind == SourceExprKind::Call && (expression.text == "vm_load16" || expression.text == "vm_mem16") ? 2U :
                                                                                                                           0U;
        if (accessWidth && expression.args.size() == 1) {
            const SourceExpr *address = &expression.args[0];
            uint32_t offset = 0;
            if (address->kind == SourceExprKind::Binary && address->text == "+" && address->args.size() == 2) {
                const std::optional<uint32_t> displacement = sourceDataConstantValue(address->args[1]);
                if (!displacement)
                    return;
                offset = *displacement;
                address = &address->args[0];
            }

            const std::map<std::string, SourceDataBinding>::const_iterator data =
                address->kind == SourceExprKind::Identifier ? unit.dataBindings.find(address->text) : unit.dataBindings.end();
            if (accessWidth == 4 && data != unit.dataBindings.end() && !data->second.isArray && !data->second.pointer)
                if (const ScenarioDataSchema *record = findScenarioDataSchema(data->second.type))
                    if (const ScenarioDataField *member = recordWord(*record, offset)) {
                        SourceExpr object{.kind = SourceExprKind::Unary, .text = "*", .args = {*address}, .cType = record->name};
                        expression = SourceExpr{.kind = SourceExprKind::Cast,
                            .text = "." + std::string(member->name),
                            .args = {std::move(object)},
                            .cType = member->type == ScenarioDataType::F32 ? "float" :
                                member->type == ScenarioDataType::U32      ? "uint32_t" :
                                                                             "int32_t"};
                        return;
                    }
            if (address->kind == SourceExprKind::Unary && address->text == "&" && address->args.size() == 1) {
                const SourceExpr &base = address->args[0];
                const ScenarioDataSchema *schema = schemaFor(base.text);
                if (base.kind == SourceExprKind::Identifier && schema)
                    for (const ScenarioDataField &field : schema->fields)
                        if (field.offset == offset && scenarioDataTypeSize(field.type) == accessWidth && field.elementCount == 1) {
                            if (accessWidth < 4 && offset && !function.localTypes.count(base.text))
                                expression = SourceExpr{.kind = SourceExprKind::Cast, .text = "." + std::string(field.name), .args = {base}};
                            else if (accessWidth < 4) {
                                SourceExpr object{.kind = SourceExprKind::Unary, .text = "*", .args = {*address}, .cType = schema->name};
                                expression = SourceExpr{.kind = SourceExprKind::Cast,
                                    .text = "." + std::string(field.name),
                                    .args = {std::move(object)},
                                    .cType = accessWidth == 1 ? "int8_t" : "int16_t"};
                            } else if (offset)
                                expression = identifier(base.text + "." + field.name);
                            else {
                                SourceExpr object{.kind = SourceExprKind::Unary, .text = "*", .args = {*address}, .cType = schema->name};
                                expression = SourceExpr{.kind = SourceExprKind::Cast,
                                    .text = "." + std::string(field.name),
                                    .args = {std::move(object)},
                                    .cType = field.type == ScenarioDataType::F32 ? "float" :
                                        field.type == ScenarioDataType::U32      ? "uint32_t" :
                                                                                   "int32_t"};
                            }
                            return;
                        }
            }
        }
        const bool preserveBase = expression.kind == SourceExprKind::Unary && expression.text == "&";
        for (SourceExpr &child : expression.args)
            rewrite(child, preserveBase);
    };
    const std::function<void(std::vector<SourceStmt> &)> statements = [&](std::vector<SourceStmt> &body) {
        for (SourceStmt &statement : body) {
            rewrite(statement.expr, false);
            rewrite(statement.value, false);
            statements(statement.body);
            statements(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases) {
                rewrite(clause.value, false);
                statements(clause.body);
            }
        }
    };
    statements(function.body.body);
}

void recoverCMemberAddresses(SourceFunctionAst &function) {
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Identifier || expression.text.find('.') == std::string::npos)
            return;
        const std::string base = expression.text.substr(0, expression.text.find('.')), name = expression.text.substr(base.size() + 1);
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(base);
        const bool frame = localSlotRefFromName(base) || function.localOffsets.count(base);
        const ScenarioDataSchema *record = type != function.localTypes.end() && frame ? findScenarioDataSchema(type->second) : nullptr;
        for (const ScenarioDataField &field : record ? record->fields : std::span<const ScenarioDataField>{})
            if (name == field.name && field.offset && field.elementCount == 1) {
                SourceExpr address{.kind = SourceExprKind::Unary, .text = "&", .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = base}}};
                SourceExpr object{.kind = SourceExprKind::Unary, .text = "*", .args = {std::move(address)}, .cType = record->name};
                const std::string word = field.type == ScenarioDataType::F32 ? "float" : field.type == ScenarioDataType::U32 ? "uint32_t" : "int32_t";
                const std::string type = !expression.cType.empty() || scenarioDataTypeSize(field.type) != 4 ? expression.cType : word;
                expression = SourceExpr{.kind = SourceExprKind::Cast, .text = "." + name, .args = {std::move(object)}, .cType = type};
                return;
            }
    });
}

bool nativeCWordArgument(const SourceExpr &expression, size_t index) {
    const abi::CommandSpec *command = abiCommandForCallName(expression.text);
    if (!command || !command->handlerName)
        return false;
    std::vector<std::optional<uint32_t>> observed;
    for (const SourceExpr &argument : expression.args)
        observed.push_back(sourceDataConstantValue(argument));
    const NativeArgumentType *argument = nativeArgumentForHandler(command->handlerName, index, observed);
    return argument && !argument->pointer && std::string_view(argument->type) == "scn3_word";
}

bool nativeCFloatArgument(const SourceExpr &expression, size_t index) {
    const abi::CommandSpec *command = abiCommandForCallName(expression.text);
    if (!command || !command->handlerName)
        return false;
    std::vector<std::optional<uint32_t>> observed;
    for (const SourceExpr &argument : expression.args)
        observed.push_back(sourceDataConstantValue(argument));
    const NativeArgumentType *argument = nativeArgumentForHandler(command->handlerName, index, observed);
    return argument && !argument->pointer && std::string_view(argument->type) == "float";
}

namespace {

class FloatWords {
  public:
    explicit FloatWords(SourceUnit &unit) : unit_(unit) {
    }

    struct Condition {
        bool always = false;
        size_t word = SIZE_MAX;
    };

    void solve() {
        integers();
        for (const std::string &name : unit_.functionOrder)
            collect(unit_.functions.at(name));
        std::vector<bool> floating(words_.size());
        std::vector<size_t> pending = seeds_;
        while (!pending.empty()) {
            const size_t word = pending.back();
            pending.pop_back();
            if (floating[word])
                continue;
            floating[word] = true;
            pending.insert(pending.end(), implied_[word].begin(), implied_[word].end());
        }
        for (size_t word = 0; word < words_.size(); ++word) {
            if (!floating[word])
                continue;
            if (words_[word].first) {
                words_[word].first->localTypes[words_[word].second] = "float";
                continue;
            }
            std::string &type = unit_.workspaceBindings.at(words_[word].second).type;
            type = "float" + type.substr(7);
        }
    }

  private:
    size_t word(SourceFunctionAst *function, const std::string &name) {
        const std::pair<SourceFunctionAst *, std::string> key{function, name};
        const std::map<std::pair<SourceFunctionAst *, std::string>, size_t>::const_iterator found = index_.find(key);
        if (found != index_.end())
            return found->second;
        index_.emplace(key, words_.size());
        words_.push_back(key);
        implied_.emplace_back();
        return words_.size() - 1;
    }

    void imply(const Condition &condition, size_t word) {
        if (condition.always)
            seeds_.push_back(word);
        else if (condition.word != SIZE_MAX)
            implied_[condition.word].push_back(word);
    }

    std::optional<size_t> local(SourceFunctionAst &function, const std::string &name) {
        bool parameter = false;
        for (const std::string &own : function.params)
            parameter |= own == name;
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(name);
        if (!parameter && !(slot && !slot->aggregateField && name.find('.') == std::string::npos))
            return std::nullopt;
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(name);
        if (type != function.localTypes.end() && type->second != "int32_t")
            return std::nullopt;
        return word(&function, name);
    }

    std::string object(const SourceExpr &expression) const {
        if (expression.kind == SourceExprKind::Identifier) {
            const std::map<std::string, SourceDataBinding>::const_iterator binding = unit_.workspaceBindings.find(expression.text);
            return binding != unit_.workspaceBindings.end() && !binding->second.isArray && !binding->second.pointer &&
                    (binding->second.type == "int32_t" || binding->second.type == "float") ?
                expression.text :
                std::string();
        }
        const SourceExpr *base = nullptr;
        bool addressed = false;
        if (expression.kind == SourceExprKind::Index && !expression.args.empty()) {
            base = &expression.args[0];
            while (base->kind == SourceExprKind::Index && !base->args.empty())
                base = &base->args[0];
        } else if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1) {
            base = &expression.args[0];
            while (base->kind == SourceExprKind::Binary && (base->text == "+" || base->text == "-") && base->args.size() == 2)
                base = &base->args[0];
            addressed = base->kind == SourceExprKind::Unary && base->text == "&" && base->args.size() == 1;
            if (addressed)
                base = &base->args[0];
        }
        if (!base || base->kind != SourceExprKind::Identifier)
            return {};
        const std::map<std::string, SourceDataBinding>::const_iterator binding = unit_.workspaceBindings.find(base->text);
        if (binding == unit_.workspaceBindings.end() || binding->second.pointer)
            return {};
        if (!binding->second.isArray && addressed)
            return binding->second.type == "int32_t" || binding->second.type == "float" ? base->text : std::string();
        if (!binding->second.isArray)
            return {};
        const std::string &type = binding->second.type;
        return type == "int32_t" || type == "float" || type.starts_with("int32_t[") || type.starts_with("float[") ? base->text : std::string();
    }

    std::optional<size_t> workspace(const std::string &name) {
        if (name.empty() || integers_.count(name) || !unit_.workspaceBindings.at(name).type.starts_with("int32_t"))
            return std::nullopt;
        return word(nullptr, name);
    }

    static bool floatOperation(const SourceExpr &expression) {
        const std::string &text = expression.text;
        return expression.kind == SourceExprKind::Call &&
            (text == "as_float" || text == "vm_flt" || text == "vm_fle" || text == "vm_fgt" || text == "vm_fge" || text == "vm_fadd" || text == "vm_fsub" ||
                text == "vm_fmul" || text == "vm_fdiv" || text == "vm_float_to_int");
    }

    void integers() {
        for (const std::string &name : unit_.functionOrder) {
            SourceFunctionAst &function = unit_.functions.at(name);
            visitExpressions(function.body.body, [&](SourceExpr &expression) {
                const std::string &text = expression.text;
                if ((expression.kind == SourceExprKind::Call && text == "vm_int_to_float") || (expression.kind == SourceExprKind::Binary && text != "=") ||
                    (expression.kind == SourceExprKind::Unary && text != "&"))
                    for (const SourceExpr &argument : expression.args)
                        if (const std::string word = object(argument); !word.empty())
                            integers_.insert(word);
            });
            const std::function<void(const std::vector<SourceStmt> &)> assignments = [&](const std::vector<SourceStmt> &statements) {
                for (const SourceStmt &statement : statements) {
                    if (statement.kind == SourceStmtKind::Assign && statement.value.kind == SourceExprKind::Call && statement.value.text == "vm_float_to_int")
                        if (const std::string word = object(statement.expr); !word.empty())
                            integers_.insert(word);
                    assignments(statement.body);
                    assignments(statement.elseBody);
                    for (const SourceSwitchCase &clause : statement.cases)
                        assignments(clause.body);
                }
            };
            assignments(function.body.body);
        }
    }

    Condition floatValue(SourceFunctionAst &function, const SourceExpr &value) {
        if (value.kind == SourceExprKind::Call) {
            if (value.text == "as_float" || value.text == "vm_fadd" || value.text == "vm_fsub" || value.text == "vm_fmul" || value.text == "vm_fdiv" ||
                value.text == "vm_int_to_float")
                return {.always = true};
            const abi::CommandSpec *command = abiCommandForCallName(value.text);
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            return {.always = prototype && prototype->result == "float"};
        }
        if (const std::optional<ExplicitLocalMember> member = explicitLocalMember(value, function, &unit_))
            return {.always = member->field->type == ScenarioDataType::F32};
        if (value.kind != SourceExprKind::Identifier)
            return {};
        if (const std::optional<size_t> open = local(function, value.text))
            return {.word = *open};
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(value.text);
        if (type != function.localTypes.end())
            return {.always = type->second == "float"};
        const std::map<std::string, SourceDataBinding>::const_iterator binding = unit_.workspaceBindings.find(value.text);
        if (binding == unit_.workspaceBindings.end())
            return {};
        if (binding->second.type == "int32_t")
            if (const std::optional<size_t> open = workspace(object(value)))
                return {.word = *open};
        return {.always = binding->second.type == "float"};
    }

    void expect(SourceFunctionAst &function, const SourceExpr &expression, const Condition &floating) {
        if (expression.kind == SourceExprKind::Identifier) {
            if (const std::optional<size_t> open = local(function, expression.text))
                imply(floating, *open);
            return;
        }
        if (expression.kind == SourceExprKind::Call) {
            const std::map<std::string, SourceFunctionAst>::iterator callee = unit_.functions.find(expression.text);
            if (callee != unit_.functions.end()) {
                for (size_t index = 0; index < expression.args.size(); ++index)
                    expect(function, expression.args[index],
                        index < callee->second.params.size() ? floatValue(callee->second, identifier(callee->second.params[index])) : Condition{});
                return;
            }
            if (abiCommandForCallName(expression.text)) {
                for (size_t index = 0; index < expression.args.size(); ++index)
                    expect(function, expression.args[index], {.always = nativeCFloatArgument(expression, index)});
                return;
            }
            if (expression.text == "vm_int_to_float") {
                for (const SourceExpr &child : expression.args)
                    expect(function, child, {});
                return;
            }
        }
        const bool dereference = (expression.kind == SourceExprKind::Unary && expression.text == "*") || expression.kind == SourceExprKind::Index;
        const auto objectAddress = [](const SourceExpr &address) {
            const SourceExpr &base =
                address.kind == SourceExprKind::Binary && (address.text == "+" || address.text == "-") && address.args.size() == 2 ? address.args[0] : address;
            return base.kind == SourceExprKind::Unary && base.text == "&";
        };
        const Condition propagated = floatOperation(expression) ? Condition{.always = true} : expression.kind != SourceExprKind::Call ? floating : Condition{};
        for (size_t index = 0; index < expression.args.size(); ++index) {
            const SourceExpr &child = expression.args[index];
            if (dereference && index == 0 && objectAddress(child))
                expect(function, child.kind == SourceExprKind::Binary ? child.args[0] : child, floating);
            expect(function, child, dereference ? Condition{} : propagated);
        }
    }

    void collect(SourceFunctionAst &function) {
        visitExpressions(function.body.body, [&](SourceExpr &expression) { expect(function, expression, {}); });

        std::set<std::string> converted;
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Call && expression.text == "vm_int_to_float" && expression.args.size() == 1 &&
                expression.args[0].kind == SourceExprKind::Identifier)
                converted.insert(expression.args[0].text);
        });
        const auto assignment = [&](const SourceExpr &target, const SourceExpr &value) {
            const std::optional<ExplicitLocalMember> member = explicitLocalMember(target, function, &unit_);
            if (member && member->field->type == ScenarioDataType::F32)
                expect(function, value, {.always = true});
            if (target.kind == SourceExprKind::Cast && target.text.starts_with(".") && target.args.size() == 1 &&
                target.args[0].kind == SourceExprKind::Unary && target.args[0].text == "*" && target.args[0].args.size() == 1 &&
                target.args[0].args[0].kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator pointer = function.localTypes.find(target.args[0].args[0].text);
                std::string pointee = pointer == function.localTypes.end() || pointer->second.find('*') == std::string::npos ?
                    std::string() :
                    trimCopy(pointer->second.substr(0, pointer->second.rfind('*')));
                if (pointee.starts_with("const "))
                    pointee.erase(0, 6);
                if (const ScenarioDataSchema *record = findScenarioDataSchema(pointee))
                    for (const ScenarioDataField &field : record->fields)
                        if (target.text.substr(1) == field.name && field.type == ScenarioDataType::F32)
                            expect(function, value, {.always = true});
            }

            if (target.kind == SourceExprKind::Identifier && !converted.count(target.text))
                expect(function, target, floatValue(function, value));
            const SourceExpr *object = &target;
            if (target.kind == SourceExprKind::Unary && target.text == "*" && target.args.size() == 1) {
                object = &target.args[0];
                if (object->kind == SourceExprKind::Binary && (object->text == "+" || object->text == "-") && object->args.size() == 2)
                    object = &object->args[0];
                object = object->kind == SourceExprKind::Unary && object->text == "&" && object->args.size() == 1 ? &object->args[0] : nullptr;
            }
            bool parameter = false;
            for (const std::string &name : function.params)
                parameter |= value.kind == SourceExprKind::Identifier && name == value.text;
            if (object && object->kind == SourceExprKind::Identifier && parameter)
                expect(function, value, floatValue(function, *object));
        };
        const std::function<void(const std::vector<SourceStmt> &)> assignments = [&](const std::vector<SourceStmt> &statements) {
            for (const SourceStmt &statement : statements) {
                if (statement.kind == SourceStmtKind::Assign) {
                    assignment(statement.expr, statement.value);

                    if (const std::optional<size_t> open = workspace(object(statement.expr)))
                        imply(floatValue(function, statement.value), *open);
                }
                assignments(statement.body);
                assignments(statement.elseBody);
                for (const SourceSwitchCase &clause : statement.cases)
                    assignments(clause.body);
            }
        };
        assignments(function.body.body);

        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Binary && expression.text == "=" && expression.args.size() == 2)
                assignment(expression.args[0], expression.args[1]);
        });

        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            const bool native = expression.kind == SourceExprKind::Call && abiCommandForCallName(expression.text);
            if (!floatOperation(expression) && !native)
                return;
            for (size_t index = 0; index < expression.args.size(); ++index)
                if (const std::optional<size_t> open = workspace(object(expression.args[index])))
                    imply({.always = !native || nativeCFloatArgument(expression, index)}, *open);
        });
    }

    SourceUnit &unit_;
    std::set<std::string> integers_;
    std::vector<std::pair<SourceFunctionAst *, std::string>> words_;
    std::map<std::pair<SourceFunctionAst *, std::string>, size_t> index_;
    std::vector<std::vector<size_t>> implied_;
    std::vector<size_t> seeds_;
};

} // namespace

void recoverCFloatStorage(SourceUnit &unit) {
    FloatWords(unit).solve();
}

SourceExpr cFloatLiteral(const SourceExpr &expression) {
    const std::optional<uint32_t> bits = sourceDataConstantValue(expression);
    if (!bits)
        throw std::runtime_error("Missing floating-point type");
    const std::string text = formatFloatLiteral(*bits);
    const std::optional<SourceExpr> literal = parseExpressionText(text);
    if (!literal || !explicitExpressionIsFloat(*literal) || !floatLiteralTextRoundTrips(text, *bits))
        throw std::runtime_error("Floating-point bit pattern " + hexUnsigned(*bits) + " cannot be represented as a C constant");
    return *literal;
}

bool cFloatStorage(const SourceExpr &target, const SourceFunctionAst &function, const SourceUnit &unit) {
    const auto floatObject = [&](const SourceExpr &object, bool element) {
        if (object.kind != SourceExprKind::Identifier)
            return false;
        const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(object.text);
        if (local != function.localTypes.end())
            return local->second == "float" || (element && (local->second == "float *" || local->second == "const float *"));
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(object.text);
        return workspace != unit.workspaceBindings.end() && !workspace->second.pointer && workspace->second.type.starts_with("float") &&
            (element || !workspace->second.isArray);
    };
    if (const std::optional<ExplicitLocalMember> member = explicitLocalMember(target, function, &unit))
        return member->field->type == ScenarioDataType::F32;
    if (target.kind == SourceExprKind::Identifier)
        return floatObject(target, false);
    if (target.kind == SourceExprKind::Cast && target.text.starts_with(".") && target.args.size() == 1 && target.args[0].kind == SourceExprKind::Unary &&
        target.args[0].text == "*" && target.args[0].args.size() == 1 && target.args[0].args[0].kind == SourceExprKind::Identifier) {
        const std::map<std::string, std::string>::const_iterator pointer = function.localTypes.find(target.args[0].args[0].text);
        std::string pointee = pointer == function.localTypes.end() || pointer->second.find('*') == std::string::npos ?
            std::string() :
            trimCopy(pointer->second.substr(0, pointer->second.rfind('*')));
        if (pointee.starts_with("const "))
            pointee.erase(0, 6);
        if (const ScenarioDataSchema *record = findScenarioDataSchema(pointee))
            for (const ScenarioDataField &field : record->fields)
                if (target.text.substr(1) == field.name)
                    return field.type == ScenarioDataType::F32;
        return false;
    }
    const SourceExpr *base = nullptr;
    if (target.kind == SourceExprKind::Index && !target.args.empty()) {
        base = &target.args[0];
        while (base->kind == SourceExprKind::Index && !base->args.empty())
            base = &base->args[0];
    } else if (target.kind == SourceExprKind::Unary && target.text == "*" && target.args.size() == 1) {
        base = &target.args[0];
        if (base->kind == SourceExprKind::Binary && (base->text == "+" || base->text == "-") && base->args.size() == 2)
            base = &base->args[0];
        if (base->kind == SourceExprKind::Unary && base->text == "&" && base->args.size() == 1)
            base = &base->args[0];
    }
    return base && floatObject(*base, true);
}

SourceExpr cTypedExpression(const SourceExpr &expression, const SourceFunctionAst &function, bool floating, const SourceUnit *unit) {
    if (expression.kind == SourceExprKind::Call) {
        if (expression.text == "as_float" && expression.args.size() == 1)
            return cTypedExpression(expression.args[0], function, true, unit);
        if (expression.text == "as_word" && expression.args.size() == 1)
            return SourceExpr{
                .kind = SourceExprKind::Cast, .text = "scn3_bits.i", .args = {cTypedExpression(expression.args[0], function, true, unit)}, .cType = "int32_t"};

        if ((expression.text == "vm_int_to_float" || expression.text == "vm_float_to_int") && expression.args.size() == 1)
            return SourceExpr{.kind = SourceExprKind::Cast,
                .text = expression.text == "vm_int_to_float" ? "float" : "int32_t",
                .args = {cTypedExpression(expression.args[0], function, expression.text == "vm_float_to_int", unit)}};
        const std::string name = expression.text;
        const std::string arithmetic = name == "vm_fadd" ? "+" : name == "vm_fsub" ? "-" : name == "vm_fmul" ? "*" : name == "vm_fdiv" ? "/" : "";
        const std::string complement = name == "vm_flt" ? ">=" : name == "vm_fle" ? ">" : name == "vm_fgt" ? "<=" : name == "vm_fge" ? "<" : "";
        if (expression.args.size() == 2 && (!arithmetic.empty() || !complement.empty())) {
            SourceExpr binary{.kind = SourceExprKind::Binary,
                .text = arithmetic.empty() ? complement : arithmetic,
                .args = {cTypedExpression(expression.args[0], function, true, unit), cTypedExpression(expression.args[1], function, true, unit)}};
            if (arithmetic.empty())
                return SourceExpr{.kind = SourceExprKind::Unary, .text = "!", .args = {std::move(binary)}};
            return binary;
        }
    }

    if (floating && unit) {
        const auto integerWord = [&](const std::string &name) {
            const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(name);
            if (local != function.localTypes.end())
                return local->second == "int32_t" || local->second == "uint32_t";
            const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit->workspaceBindings.find(name);
            if (workspace != unit->workspaceBindings.end())
                return workspace->second.type == "int32_t" || workspace->second.type == "uint32_t";
            const std::map<std::string, SourceDataBinding>::const_iterator data = unit->dataBindings.find(name);
            return data != unit->dataBindings.end() && !data->second.pointer &&
                (data->second.type == "u32" || data->second.type == "uint32_t" || data->second.type == "int32_t");
        };
        const SourceExpr *address =
            expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 ? &expression.args[0] : nullptr;
        const SourceExpr *base =
            address && address->kind == SourceExprKind::Binary && address->text == "+" && address->args.size() == 2 ? &address->args[0] : address;

        const std::map<std::string, SourceDataBinding>::const_iterator table =
            base && base->kind == SourceExprKind::Identifier ? unit->dataBindings.find(base->text) : unit->dataBindings.end();
        const bool otherElements = table != unit->dataBindings.end() && table->second.isArray && !table->second.pointer && table->second.type != "float";
        const bool dataBase =
            base && base->kind == SourceExprKind::Identifier && unit->dataSymbols.count(base->text) && (integerWord(base->text) || otherElements);

        bool parameter = false;
        bool array = false;
        for (const std::string &name : function.params)
            parameter |= base && name == base->text;
        for (const SourceLocalDeclaration &declaration : function.localDeclarations)
            array |= base && declaration.isArray && declaration.name == base->text;
        const bool wordAddress = base && base->kind == SourceExprKind::Identifier && !array &&
            ((function.localTypes.count(base->text) && integerWord(base->text)) || (parameter && !function.localTypes.count(base->text)));
        if (dataBase || wordAddress ||
            (base && base->kind == SourceExprKind::Unary && base->text == "&" && base->args.size() == 1 && base->args[0].kind == SourceExprKind::Identifier &&
                integerWord(base->args[0].text)))
            return SourceExpr{.kind = SourceExprKind::Unary,
                .text = "*",
                .args = {SourceExpr{.kind = SourceExprKind::Cast, .text = "float *", .args = {*address}}},
                .cType = "float"};
        if (expression.kind == SourceExprKind::Identifier && !function.localTypes.count(expression.text) && integerWord(expression.text))
            return SourceExpr{.kind = SourceExprKind::Unary,
                .text = "*",
                .args = {SourceExpr{
                    .kind = SourceExprKind::Cast, .text = "float *", .args = {SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {expression}}}}},
                .cType = "float"};
    }
    if (floating &&
        (expression.kind == SourceExprKind::IntLiteral || (expression.kind == SourceExprKind::Cast && !expression.text.starts_with(".")) ||
            (expression.kind == SourceExprKind::Unary && expression.text == "-" && sourceDataConstantValue(expression))))
        return cFloatLiteral(expression);
    SourceExpr result = expression;

    if (result.kind == SourceExprKind::Binary && result.text == "=" && result.args.size() == 2) {
        result.args[1] = cTypedExpression(result.args[1], function, unit && cFloatStorage(result.args[0], function, *unit), unit);
        return result;
    }
    for (size_t index = 0; index < result.args.size(); ++index) {
        bool argumentFloat = false;
        if (unit && expression.kind == SourceExprKind::Call) {
            const std::map<std::string, SourceFunctionAst>::const_iterator callee = unit->functions.find(expression.text);
            if (callee != unit->functions.end() && index < callee->second.params.size()) {
                const std::map<std::string, std::string>::const_iterator type = callee->second.localTypes.find(callee->second.params[index]);
                argumentFloat = type != callee->second.localTypes.end() && type->second == "float";
            }
        }
        if (expression.kind == SourceExprKind::Call && nativeCFloatArgument(expression, index))
            argumentFloat = true;
        if (expression.kind == SourceExprKind::Call && nativeCWordArgument(expression, index) && sourceDataConstantValue(result.args[index]) &&
            !explicitExpressionIsFloat(result.args[index])) {
            try {
                result.args[index] = cFloatLiteral(result.args[index]);
            } catch (const std::exception &) {
            }
            continue;
        }
        result.args[index] = cTypedExpression(result.args[index], function, argumentFloat, unit);

        const SourceExpr &argument = result.args[index];
        const abi::CommandSpec *command = argumentFloat && argument.kind == SourceExprKind::Call ? abiCommandForCallName(argument.text) : nullptr;
        const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
        if (prototype && prototype->result != "float" && prototype->result != "void")
            result.args[index] = SourceExpr{.kind = SourceExprKind::Cast, .text = "scn3_bits.f", .args = {std::move(result.args[index])}, .cType = "float"};

        const abi::CommandSpec *native = !argumentFloat && expression.kind == SourceExprKind::Call && (!unit || !unit->functions.count(expression.text)) ?
            abiCommandForCallName(expression.text) :
            nullptr;
        if (native && native->handlerName && explicitExpressionIsFloat(result.args[index], unit, &function)) {
            std::vector<std::optional<uint32_t>> constants;
            for (const SourceExpr &value : expression.args)
                constants.push_back(sourceDataConstantValue(value));
            if (const NativeArgumentLayout *layout = nativeArgumentLayout(native->handlerName, constants)) {
                const uint32_t offset = static_cast<uint32_t>(index * 4U);
                bool word = offset < layout->parameterBytes && (layout->variadicOffset < 0 || offset < static_cast<uint32_t>(layout->variadicOffset));
                for (const NativePointerParameter &pointer : layout->pointers)
                    word &= pointer.byteOffset != offset;
                for (const uint32_t floating : layout->floatArguments)
                    word &= floating != offset;
                if (word)
                    result.args[index] =
                        SourceExpr{.kind = SourceExprKind::Cast, .text = "scn3_bits.i", .args = {std::move(result.args[index])}, .cType = "int32_t"};
            }
        }
    }

    if (result.kind == SourceExprKind::Binary && result.args.size() == 2 && result.text != "=" && result.text != "&&" && result.text != "||")
        for (SourceExpr &operand : result.args) {
            const abi::CommandSpec *command = operand.kind == SourceExprKind::Call ? abiCommandForCallName(operand.text) : nullptr;
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            if (explicitExpressionIsFloat(operand, unit, &function) || (prototype && prototype->result == "float"))
                operand = SourceExpr{.kind = SourceExprKind::Cast, .text = "scn3_bits.i", .args = {std::move(operand)}, .cType = "int32_t"};
        }
    if (result.kind == SourceExprKind::Unary && result.text == "!" && result.args.size() == 1 && result.args[0].kind == SourceExprKind::Unary &&
        result.args[0].text == "!" && result.args[0].args.size() == 1 && result.args[0].args[0].kind == SourceExprKind::Binary &&
        (result.args[0].args[0].text == "<" || result.args[0].args[0].text == "<=" || result.args[0].args[0].text == ">" ||
            result.args[0].args[0].text == ">=" || result.args[0].args[0].text == "==" || result.args[0].args[0].text == "!="))
        return result.args[0].args[0];
    return result;
}

void recoverCAddressWords(SourceFunctionAst &function, const SourceUnit &unit) {
    const auto storageType = [&](const SourceExpr &target, const std::map<std::string, std::string> &scope) -> std::string {
        if (target.kind == SourceExprKind::Cast && target.text.starts_with("."))
            return target.cType;
        if (target.kind != SourceExprKind::Identifier)
            return {};
        const size_t dot = target.text.find('.');
        const std::string base = target.text.substr(0, dot);
        const std::map<std::string, std::string>::const_iterator local = scope.find(base);
        const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(base);
        const std::string type = local != scope.end() ? local->second :
            workspace == unit.workspaceBindings.end() ? "" :
            workspace->second.pointer                 ? workspace->second.pointerType :
                                                        workspace->second.type;
        if (dot == std::string::npos)
            return type;
        if (const ScenarioDataSchema *schema = findScenarioDataSchema(type))
            for (const ScenarioDataField &field : schema->fields)
                if (field.name == target.text.substr(dot + 1) && field.elementCount == 1)
                    return field.type == ScenarioDataType::I32 ? "int32_t" : field.type == ScenarioDataType::U32 ? "uint32_t" : "";
        return {};
    };
    const std::function<void(std::vector<SourceStmt> &, std::map<std::string, std::string>)> statements = [&](std::vector<SourceStmt> &body,
                                                                                                              std::map<std::string, std::string> scope) {
        const auto store = [&](const SourceExpr &target, SourceExpr &value) {
            const std::map<std::string, SourceDataBinding>::const_iterator data =
                value.kind == SourceExprKind::Identifier ? unit.dataBindings.find(value.text) : unit.dataBindings.end();
            if ((value.kind != SourceExprKind::Unary || value.text != "&") && (data == unit.dataBindings.end() || !data->second.isArray))
                return;
            const std::string type = storageType(target, scope);
            if (type == "int32_t" || type == "uint32_t")
                value = SourceExpr{.kind = SourceExprKind::Cast, .text = type, .args = {std::move(value)}, .cType = type, .cConversion = "PointerToIntegral"};
        };
        const std::function<void(SourceExpr &)> convert = [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Binary && expression.text == "=" && expression.args.size() == 2)
                store(expression.args[0], expression.args[1]);
        };
        for (SourceStmt &statement : body) {
            if (statement.kind == SourceStmtKind::Declaration)
                scope[statement.declaration.name] = statement.declaration.type;
            if (statement.kind == SourceStmtKind::Assign)
                store(statement.expr, statement.value);
            visitExpressions(statement.expr, convert);
            visitExpressions(statement.value, convert);
            statements(statement.body, scope);
            statements(statement.elseBody, scope);
            for (SourceSwitchCase &clause : statement.cases)
                statements(clause.body, scope);
        }
    };
    statements(function.body.body, function.localTypes);
}

void recoverCFloatExpressions(SourceFunctionAst &function, const SourceUnit &unit) {

    const std::function<bool(const SourceExpr &)> integerWord = [&](const SourceExpr &value) {
        if (value.kind == SourceExprKind::Call) {
            if (value.text == "vm_float_to_int")
                return true;
            if (value.text == "as_float" && value.args.size() == 1)
                return integerWord(value.args[0]);
            const abi::CommandSpec *command = abiCommandForCallName(value.text);
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            return prototype && prototype->result != "float" && prototype->result != "void";
        }
        if (value.kind == SourceExprKind::Binary && value.args.size() == 2 && value.text != "=")
            return true;

        if (value.kind == SourceExprKind::Cast && value.text.starts_with("."))
            return value.cType == "int32_t" || value.cType == "uint32_t";
        if (value.kind != SourceExprKind::Identifier)
            return false;
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(value.text);
        if (type != function.localTypes.end())
            return type->second != "float";
        for (const std::string &parameter : function.params)
            if (parameter == value.text)
                return true;
        if (const size_t dot = value.text.find('.'); dot != std::string::npos) {
            const std::map<std::string, std::string>::const_iterator record = function.localTypes.find(value.text.substr(0, dot));
            const std::map<std::string, SourceDataBinding>::const_iterator shared = unit.workspaceBindings.find(value.text.substr(0, dot));
            const ScenarioDataSchema *schema = record != function.localTypes.end() ? findScenarioDataSchema(record->second) :
                shared != unit.workspaceBindings.end()                             ? findScenarioDataSchema(shared->second.type) :
                                                                                     nullptr;
            for (const ScenarioDataField &field : schema ? schema->fields : std::span<const ScenarioDataField>{})
                if (value.text.substr(dot + 1) == field.name)
                    return field.elementCount == 1 && field.type != ScenarioDataType::F32 && scenarioDataTypeSize(field.type) == 4;
        }

        if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(value.text);
            slot && !slot->aggregateField && value.text.find('.') == std::string::npos)
            return true;
        const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.workspaceBindings.find(value.text);
        return binding != unit.workspaceBindings.end() && scenarioDataTypeByName(binding->second.type) && binding->second.type != "float";
    };
    const std::function<void(std::vector<SourceStmt> &)> rewrite = [&](std::vector<SourceStmt> &statements) {
        for (SourceStmt &statement : statements) {

            const SourceExpr *element = statement.expr.kind == SourceExprKind::Index && !statement.expr.args.empty() ? &statement.expr.args[0] : nullptr;
            while (element && element->kind == SourceExprKind::Index && !element->args.empty())
                element = &element->args[0];
            if (statement.expr.kind == SourceExprKind::Unary && statement.expr.text == "*" && statement.expr.args.size() == 1) {
                element = &statement.expr.args[0];
                while (element->kind == SourceExprKind::Binary && (element->text == "+" || element->text == "-") && element->args.size() == 2)
                    element = &element->args[0];
                element = element->kind == SourceExprKind::Unary && element->text == "&" && element->args.size() == 1 ? &element->args[0] : nullptr;
            }
            const std::map<std::string, SourceDataBinding>::const_iterator elementBinding =
                element && element->kind == SourceExprKind::Identifier && !function.localTypes.count(element->text) ?
                unit.workspaceBindings.find(element->text) :
                unit.workspaceBindings.end();
            const bool floatElement =
                (elementBinding != unit.workspaceBindings.end() && !elementBinding->second.pointer && elementBinding->second.type.starts_with("float")) ||
                (element && element->kind == SourceExprKind::Identifier && statement.expr.kind == SourceExprKind::Unary &&
                    function.localTypes.count(element->text) && function.localTypes.at(element->text) == "float") ||
                (statement.kind == SourceStmtKind::Assign && cFloatStorage(statement.expr, function, unit));

            if ((statement.kind == SourceStmtKind::ExprStmt || statement.kind == SourceStmtKind::Return) && statement.expr.kind == SourceExprKind::Binary &&
                statement.expr.text == "=" && statement.expr.args.size() == 2 && !cFloatStorage(statement.expr.args[0], function, unit) &&
                (statement.expr.args[0].kind == SourceExprKind::Identifier || statement.expr.args[0].kind == SourceExprKind::Index) &&
                explicitExpressionIsFloat(statement.expr.args[1], &unit, &function)) {
                SourceExpr target = std::move(statement.expr.args[0]);
                statement.expr.args[0] = SourceExpr{.kind = SourceExprKind::Unary,
                    .text = "*",
                    .args = {SourceExpr{.kind = SourceExprKind::Cast,
                        .text = "float *",
                        .args = {SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {std::move(target)}}}}}};
            }
            statement.expr = cTypedExpression(statement.expr, function, false, &unit);

            if (statement.kind == SourceStmtKind::Switch && explicitExpressionIsFloat(statement.expr, &unit, &function))
                statement.expr = SourceExpr{.kind = SourceExprKind::Cast, .text = "scn3_bits.i", .args = {std::move(statement.expr)}, .cType = "int32_t"};
            const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(statement.expr.text);
            const std::optional<ExplicitLocalMember> member = explicitLocalMember(statement.expr, function, &unit);
            const SourceExpr *arrayBase = statement.expr.kind == SourceExprKind::Index && !statement.expr.args.empty() ? &statement.expr.args[0] : nullptr;
            while (arrayBase && arrayBase->kind == SourceExprKind::Index && !arrayBase->args.empty())
                arrayBase = &arrayBase->args[0];
            const std::map<std::string, std::string>::const_iterator array =
                arrayBase && arrayBase->kind == SourceExprKind::Identifier ? function.localTypes.find(arrayBase->text) : function.localTypes.end();
            const std::map<std::string, SourceDataBinding>::const_iterator workspace =
                type == function.localTypes.end() ? unit.workspaceBindings.find(statement.expr.text) : unit.workspaceBindings.end();
            const bool floating = statement.kind == SourceStmtKind::Assign &&
                ((type != function.localTypes.end() && type->second == "float") || (member && member->field->type == ScenarioDataType::F32) ||
                    (array != function.localTypes.end() && array->second == "float") ||
                    (workspace != unit.workspaceBindings.end() && statement.expr.kind == SourceExprKind::Identifier && workspace->second.type == "float") ||
                    floatElement);

            bool sentinel = false;
            if (floating && sourceDataConstantValue(statement.value) && !explicitExpressionIsFloat(statement.value)) {
                try {
                    (void)cFloatLiteral(statement.value);
                } catch (const std::exception &) {
                    sentinel = true;
                }
            }
            const bool word = floating && (integerWord(statement.value) || sentinel);

            const SourceExpr *loaded = statement.value.kind == SourceExprKind::Unary && statement.value.text == "*" && statement.value.args.size() == 1 ?
                &statement.value.args[0] :
                nullptr;
            if (loaded && loaded->kind == SourceExprKind::Unary && loaded->text == "&" && loaded->args.size() == 1)
                loaded = &loaded->args[0];
            const bool floatLoad = loaded && loaded->kind == SourceExprKind::Identifier && function.localTypes.count(loaded->text) &&
                function.localTypes.at(loaded->text) == "float";
            const abi::CommandSpec *command = statement.value.kind == SourceExprKind::Call ? abiCommandForCallName(statement.value.text) : nullptr;
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            const bool floatResult = prototype && prototype->result == "float";
            const bool dereference = statement.expr.kind == SourceExprKind::Unary && statement.expr.text == "*" && statement.expr.args.size() == 1;
            const bool bits = statement.kind == SourceStmtKind::Assign && !floating && statement.expr.cType.find('*') == std::string::npos &&
                (statement.expr.kind == SourceExprKind::Identifier || statement.expr.kind == SourceExprKind::Index || dereference ||
                    (statement.expr.kind == SourceExprKind::Cast && statement.expr.text.starts_with("."))) &&
                (explicitExpressionIsFloat(statement.value, &unit, &function) || floatLoad || floatResult);

            if ((word || bits) && dereference) {
                SourceExpr address = std::move(statement.expr.args[0]);
                if (address.kind == SourceExprKind::Cast && address.text.ends_with("*") && address.args.size() == 1) {
                    SourceExpr inner = std::move(address.args[0]);
                    address = std::move(inner);
                }
                statement.expr = SourceExpr{.kind = SourceExprKind::Unary,
                    .text = "*",
                    .args = {SourceExpr{.kind = SourceExprKind::Cast, .text = word ? "int32_t *" : "float *", .args = {std::move(address)}}}};
            } else if (word || bits)
                statement.expr = SourceExpr{.kind = SourceExprKind::Unary,
                    .text = "*",
                    .args = {SourceExpr{.kind = SourceExprKind::Cast,
                        .text = word ? "int32_t *" : "float *",
                        .args = {SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {std::move(statement.expr)}}}}}};
            statement.value = cTypedExpression(statement.value, function, bits || (floating && !word), &unit);
            rewrite(statement.body);
            rewrite(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                rewrite(clause.body);
        }
    };
    rewrite(function.body.body);
}

bool cUnknownReturnCall(const SourceExpr &expression, const SourceUnit &unit) {
    if (expression.kind == SourceExprKind::Call) {
        const std::map<std::string, SourceFunctionAst>::const_iterator callee = unit.functions.find(expression.text);
        if (callee != unit.functions.end() && callee->second.returnType.empty())
            return true;
    }
    for (const SourceExpr &child : expression.args)
        if (cUnknownReturnCall(child, unit))
            return true;
    return false;
}

void cReturnType(const std::vector<SourceStmt> &statements, std::string &type, const SourceUnit &unit, const SourceFunctionAst &function, bool &unknown) {
    for (const SourceStmt &statement : statements) {
        if (statement.kind == SourceStmtKind::Return && !(statement.expr.text.empty() && statement.expr.args.empty())) {
            if (cUnknownReturnCall(statement.expr, unit)) {
                unknown = true;
                continue;
            }
            const abi::CommandSpec *command = statement.expr.kind == SourceExprKind::Call ? abiCommandForCallName(statement.expr.text) : nullptr;
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            const bool floating = explicitExpressionIsFloat(statement.expr, &unit, &function) || (prototype && prototype->result == "float");
            const std::string valueType = floating ? "float" : "int32_t";
            if (type != "void" && type != valueType)
                throw std::runtime_error("Conflicting C return types in " + function.name);
            type = valueType;
        }
        cReturnType(statement.body, type, unit, function, unknown);
        cReturnType(statement.elseBody, type, unit, function, unknown);
        for (const SourceSwitchCase &clause : statement.cases)
            cReturnType(clause.body, type, unit, function, unknown);
    }
}

} // namespace shenmue::scenario::decompile
