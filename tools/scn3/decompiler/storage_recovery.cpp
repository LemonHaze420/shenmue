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

struct FrameTypeEvidence {
    std::map<std::string, const ScenarioDataSchema *> records;
    std::set<std::string> conflicts;
};

void collectFrameTypes(const SourceExpr &invoked, FrameTypeEvidence &evidence) {
    for (const SourceExpr &child : invoked.args)
        collectFrameTypes(child, evidence);
    if (invoked.kind != SourceExprKind::Call || invoked.text.starts_with("vm_"))
        return;
    const abi::CommandSpec *command = abiCommandForCallName(invoked.text);
    if (!command || !command->handlerName)
        return;
    std::vector<std::optional<uint32_t>> observed;
    for (const SourceExpr &argument : invoked.args)
        observed.push_back(sourceDataConstantValue(argument));
    for (size_t index = 0; index < invoked.args.size(); ++index) {
        const NativeArgumentType *argument = nativeArgumentForHandler(command->handlerName, index, observed);
        if (!argument || !argument->pointer)
            continue;
        const SourceExpr &address = invoked.args[index];
        if (address.kind != SourceExprKind::Unary || address.text != "&" || address.args.size() != 1)
            continue;
        const SourceExpr &base = address.args[0];
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(base.text);
        const ScenarioDataSchema *schema = findScenarioDataSchema(argument->type);
        if (base.kind != SourceExprKind::Identifier || !slot || slot->aggregateField || !schema)
            continue;
        const std::map<std::string, const ScenarioDataSchema *>::const_iterator previous = evidence.records.find(base.text);
        if (previous != evidence.records.end() && previous->second != schema)
            evidence.conflicts.insert(base.text);
        else
            evidence.records.emplace(base.text, schema);
    }
}

void collectFrameTypes(const std::vector<SourceStmt> &stmts, FrameTypeEvidence &evidence) {
    for (const SourceStmt &stmt : stmts) {
        collectFrameTypes(stmt.expr, evidence);
        collectFrameTypes(stmt.value, evidence);
        collectFrameTypes(stmt.body, evidence);
        collectFrameTypes(stmt.elseBody, evidence);
        for (const SourceSwitchCase &clause : stmt.cases) {
            collectFrameTypes(clause.value, evidence);
            collectFrameTypes(clause.body, evidence);
        }
    }
}

void rewriteFrameStores(std::vector<SourceStmt> &stmts, SourceFunctionAst &function, const FrameTypeEvidence &evidence) {
    for (size_t i = 0; i < stmts.size(); ++i) {
        SourceStmt &stmt = stmts[i];
        rewriteFrameStores(stmt.body, function, evidence);
        rewriteFrameStores(stmt.elseBody, function, evidence);
        for (SourceSwitchCase &clause : stmt.cases)
            rewriteFrameStores(clause.body, function, evidence);

        const SourceExpr *invoked = stmt.kind == SourceStmtKind::Assign ? &stmt.value : &stmt.expr;
        if (invoked->kind == SourceExprKind::Binary && invoked->text == "=" && invoked->args.size() == 2)
            invoked = &invoked->args[1];
        if (invoked->kind != SourceExprKind::Call || invoked->text.starts_with("vm_"))
            continue;
        const abi::CommandSpec *command = abiCommandForCallName(invoked->text);
        if (!command || !command->handlerName)
            continue;
        std::vector<std::optional<uint32_t>> observed;
        for (const SourceExpr &argument : invoked->args)
            observed.push_back(sourceDataConstantValue(argument));
        for (size_t argIndex = 0; argIndex < invoked->args.size(); ++argIndex) {
            const NativeArgumentType *arg = nativeArgumentForHandler(command->handlerName, argIndex, observed);
            if (!arg || !arg->pointer)
                continue;
            const SourceExpr &address = invoked->args[argIndex];
            if (address.kind != SourceExprKind::Unary || address.text != "&" || address.args.size() != 1)
                continue;
            const SourceExpr &base = address.args[0];
            const std::optional<SourceSlotRef> slot = localSlotRefFromName(base.text);
            if (base.kind != SourceExprKind::Identifier || !slot || slot->aggregateField || evidence.conflicts.count(base.text))
                continue;
            if (!evidence.records.count(base.text))
                continue;
            const ScenarioDataSchema *schema = findScenarioDataSchema(arg->type);
            if (!schema)
                continue;
            function.localTypes[base.text] = schema->name;
            for (size_t previous = i; previous > 0; --previous) {
                SourceStmt &store = stmts[previous - 1];
                if (store.kind != SourceStmtKind::Assign || store.value.kind != SourceExprKind::Binary || store.value.text != "=")
                    break;
                SourceExpr &target = store.value.args[0];
                SourceExpr &value = store.value.args[1];
                uint32_t offset = 0;
                if (target.kind == SourceExprKind::Identifier && target.text == base.text) {
                    offset = 0;
                } else if (target.kind == SourceExprKind::Unary && target.text == "*" && target.args.size() == 1) {
                    const SourceExpr &sum = target.args[0];
                    if (sum.kind != SourceExprKind::Binary || sum.text != "+" || sum.args.size() != 2)
                        break;
                    const SourceExpr &pointer = sum.args[0];
                    const std::optional<uint32_t> displacement = sourceDataConstantValue(sum.args[1]);
                    if (pointer.kind != SourceExprKind::Unary || pointer.text != "&" || pointer.args.size() != 1 || pointer.args[0].text != base.text ||
                        !displacement)
                        break;
                    offset = *displacement;
                } else
                    break;
                const ScenarioDataField *field = nullptr;
                for (const ScenarioDataField &candidate : schema->fields)
                    if (candidate.offset == offset && scenarioDataTypeSize(candidate.type) == 4 && candidate.elementCount == 1)
                        field = &candidate;
                if (!field)
                    break;
                function.localTypes[base.text] = schema->name;
                target = identifier(base.text + "." + field->name);
                if (field->type == ScenarioDataType::F32) {
                    value = typedFloatLiteral(value);
                    if (!explicitExpressionIsFloat(value, nullptr, &function))
                        value = call("as_float", {std::move(value)});
                } else if (explicitExpressionIsFloat(value, nullptr, &function))
                    value = call("as_word", {std::move(value)});
            }
        }
    }
}

void recoverCSiblingNames(SourceFunctionAst &function) {
    struct Scope {
        size_t parent;
        std::vector<SourceStmt> *statements;
        SourceStmt *owner = nullptr;
    };
    std::vector<Scope> scopes{{0, &function.body.body}};
    std::vector<std::pair<size_t, SourceStmt *>> declarations;
    const std::function<void(std::vector<SourceStmt> &, size_t)> collect = [&](std::vector<SourceStmt> &list, size_t scope) {
        for (SourceStmt &statement : list) {
            if (statement.kind == SourceStmtKind::Declaration)
                declarations.emplace_back(scope, &statement);

            if (statement.kind == SourceStmtKind::Switch) {
                scopes.push_back(Scope{scope, &statement.body, &statement});
                const size_t body = scopes.size() - 1;
                collect(statement.body, body);
                for (SourceSwitchCase &clause : statement.cases)
                    collect(clause.body, body);
                continue;
            }
            for (std::vector<SourceStmt> *child : {&statement.initializer, &statement.body, &statement.elseBody}) {
                if (child->empty())
                    continue;
                scopes.push_back(Scope{scope, child});
                collect(*child, scopes.size() - 1);
            }
        }
    };
    collect(function.body.body, 0);
    const auto encloses = [&](size_t outer, size_t inner) {
        for (;;) {
            if (inner == outer)
                return true;
            if (!inner)
                return false;
            inner = scopes[inner].parent;
        }
    };
    const auto base = [](const std::string &name) {
        size_t suffix = name.size();
        while (suffix && std::isdigit(static_cast<unsigned char>(name[suffix - 1])))
            --suffix;
        if (suffix == name.size() || suffix < 2 || name[suffix - 1] != 'v')
            return std::string();
        const std::string stem = name.substr(0, suffix - 1);
        return localSlotRefFromName(stem) || stem.starts_with("unused_") ? stem : std::string();
    };
    const std::function<void(SourceExpr &, const std::string &, const std::string &)> rename = [&](SourceExpr &expression, const std::string &from,
                                                                                                   const std::string &to) {
        if (expression.kind == SourceExprKind::Identifier && (expression.text == from || expression.text.starts_with(from + ".")))
            expression.text = to + expression.text.substr(from.size());
        for (SourceExpr &child : expression.args)
            rename(child, from, to);
    };
    std::map<std::string, std::vector<size_t>> claimed;
    for (const std::pair<size_t, SourceStmt *> &declaration : declarations) {
        const std::string &name = declaration.second->declaration.name;
        claimed[base(name).empty() ? name : base(name)];
        if (base(name).empty())
            claimed[name].push_back(declaration.first);
    }
    for (const std::pair<size_t, SourceStmt *> &declaration : declarations) {
        const std::string name = declaration.second->declaration.name, stem = base(name);
        if (stem.empty())
            continue;
        bool free = true;
        for (size_t scope : claimed[stem])
            free &= !encloses(scope, declaration.first) && !encloses(declaration.first, scope);
        if (!free)
            continue;
        claimed[stem].push_back(declaration.first);
        declaration.second->declaration.name = stem;
        const Scope &scope = scopes[declaration.first];
        visitExpressions(*scope.statements, [&](SourceExpr &expression) { rename(expression, name, stem); });
        if (scope.owner)
            for (SourceSwitchCase &clause : scope.owner->cases)
                visitExpressions(clause.body, [&](SourceExpr &expression) { rename(expression, name, stem); });
    }
}

void typedFrameStores(std::vector<SourceStmt> &stmts, SourceFunctionAst &function, bool shared) {
    FrameTypeEvidence evidence{};
    collectFrameTypes(stmts, evidence);

    const auto extent = [&](const std::pair<const std::string, const ScenarioDataSchema *> &record) {
        const uint32_t raw = localSlotRefFromName(record.first)->baseOffset;
        const int32_t start = raw <= 0xFFU ? static_cast<int8_t>(raw) : static_cast<int16_t>(raw);
        return std::pair<int64_t, int64_t>{start, static_cast<int64_t>(start) + record.second->size};
    };
    for (const std::pair<const std::string, const ScenarioDataSchema *> &first : evidence.records) {
        if (extent(first).first >= 0 || extent(first).second > 0)
            evidence.conflicts.insert(first.first);
        for (const std::pair<const std::string, const ScenarioDataSchema *> &second : evidence.records)
            if (!shared && first.first < second.first && extent(first).first < extent(second).second && extent(second).first < extent(first).second) {
                evidence.conflicts.insert(first.first);
                evidence.conflicts.insert(second.first);
            }
    }
    for (const std::pair<const std::string, const ScenarioDataSchema *> &record : evidence.records)
        if (!evidence.conflicts.count(record.first))
            function.localTypes[record.first] = record.second->name;
    rewriteFrameStores(stmts, function, evidence);
}

bool recoverCCopiedRecords(SourceFunctionAst &function, const SourceUnit &unit) {
    struct Access {
        std::string base;
        uint32_t offset;
        uint32_t width;
        bool pointer = false;
    };
    const auto frameOffset = [](const SourceSlotRef &slot) {
        return slot.baseOffset <= 0xffU ? static_cast<int32_t>(static_cast<int8_t>(slot.baseOffset)) :
                                          static_cast<int32_t>(static_cast<int16_t>(slot.baseOffset));
    };

    const auto access = [&](const SourceExpr &expression) -> std::optional<Access> {
        if (expression.kind == SourceExprKind::Identifier && expression.text.find('.') == std::string::npos &&
            sourceIdentifierAccessWidth(expression.text) == 4)
            if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text); slot && !slot->aggregateField)
                return Access{.base = expression.text, .offset = 0, .width = 4};
        if (const std::optional<std::pair<std::string, uint32_t>> field = dereferencedParameter(expression, function))
            return Access{.base = field->first, .offset = field->second, .width = 4, .pointer = true};
        const bool byte = expression.text == "vm_load8" || expression.text == "vm_mem8";
        const bool half = expression.text == "vm_load16" || expression.text == "vm_mem16";
        const bool word = expression.kind == SourceExprKind::Unary && expression.text == "*";
        if ((expression.kind != SourceExprKind::Call && !word) || expression.args.size() != 1 || (!byte && !half && !word))
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
        if (address->kind != SourceExprKind::Unary || address->text != "&" || address->args.size() != 1 || address->args[0].kind != SourceExprKind::Identifier)
            return std::nullopt;
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(address->args[0].text);
        if (!slot || slot->aggregateField || address->args[0].text.find('.') != std::string::npos)
            return std::nullopt;
        return Access{.base = address->args[0].text, .offset = offset, .width = byte ? 1U : half ? 2U : 4U};
    };
    std::map<std::string, std::vector<Access>> accesses;
    std::map<std::string, uint32_t> uses, addressed;
    std::set<int32_t> objects;
    std::map<std::string, std::set<std::string>> candidates;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier) {
            if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text)) {
                ++uses[stripSourceWidthSuffix(expression.text)];
                objects.insert(frameOffset(*slot));
            }
            return;
        }
        if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1 &&
            expression.args[0].kind == SourceExprKind::Identifier)
            ++addressed[expression.args[0].text];
        if (const std::optional<Access> found = access(expression); found && !found->pointer)
            accesses[found->base].push_back(*found);
    });
    const auto recordOf = [&](const Access &item) -> const ScenarioDataSchema * {
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(item.base);
        if (type == function.localTypes.end())
            return nullptr;
        if (!item.pointer)
            return findScenarioDataSchema(type->second);
        if (type->second.find('*') == std::string::npos)
            return nullptr;
        std::string pointee = trimCopy(type->second.substr(0, type->second.rfind('*')));
        if (pointee.starts_with("const "))
            pointee.erase(0, 6);
        return findScenarioDataSchema(pointee);
    };
    const auto fieldOf = [](const ScenarioDataSchema &schema, const Access &item) {
        for (const ScenarioDataField &field : schema.fields)
            if (field.offset == item.offset && field.elementCount == 1 && scenarioDataTypeSize(field.type) == item.width)
                return true;
        return false;
    };
    const std::function<void(const std::vector<SourceStmt> &)> copies = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            const std::optional<Access> target = statement.kind == SourceStmtKind::Assign ? access(statement.expr) : std::nullopt;
            const std::optional<Access> source = target ? access(statement.value) : std::nullopt;
            if (target && source && target->offset == source->offset && target->width == source->width && target->base != source->base)
                for (const std::pair<const Access *, const Access *> pair : {std::pair{&*target, &*source}, std::pair{&*source, &*target}}) {
                    const ScenarioDataSchema *schema = recordOf(*pair.first);
                    const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(pair.second->base);
                    if (schema && fieldOf(*schema, *pair.first) && !pair.second->pointer && (type == function.localTypes.end() || type->second == "int32_t"))
                        candidates[pair.second->base].insert(schema->name);
                }
            copies(statement.body);
            copies(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases)
                copies(clause.body);
        }
    };
    copies(function.body.body);
    std::map<std::string, uint32_t> passed;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        const std::map<std::string, SourceFunctionAst>::const_iterator callee =
            expression.kind == SourceExprKind::Call ? unit.functions.find(expression.text) : unit.functions.end();
        if (callee == unit.functions.end())
            return;
        for (size_t index = 0; index < expression.args.size() && index < callee->second.params.size(); ++index) {
            const SourceExpr &argument = expression.args[index];
            if (argument.kind != SourceExprKind::Unary || argument.text != "&" || argument.args.size() != 1 ||
                argument.args[0].kind != SourceExprKind::Identifier || !localSlotRefFromName(argument.args[0].text))
                continue;
            const std::map<std::string, std::string>::const_iterator parameter = callee->second.localTypes.find(callee->second.params[index]);
            if (parameter == callee->second.localTypes.end() || parameter->second == "int32_t") {
                ++passed[argument.args[0].text];
                continue;
            }
            if (parameter->second.find('*') == std::string::npos)
                continue;
            std::string pointee = trimCopy(parameter->second.substr(0, parameter->second.rfind('*')));
            if (pointee.starts_with("const "))
                pointee.erase(0, 6);
            const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(argument.args[0].text);
            if (!findScenarioDataSchema(pointee) || (type != function.localTypes.end() && type->second != "int32_t"))
                continue;
            candidates[argument.args[0].text].insert(pointee);
            ++passed[argument.args[0].text];
        }
    });
    bool changed = false;
    for (const std::pair<const std::string, std::set<std::string>> &candidate : candidates) {
        if (candidate.second.size() != 1)
            continue;
        const ScenarioDataSchema &schema = *findScenarioDataSchema(*candidate.second.begin());
        const std::vector<Access> &items = accesses[candidate.first];

        bool fits = addressed[candidate.first] == items.size() + passed[candidate.first] && uses[candidate.first] >= addressed[candidate.first];
        if (uses[candidate.first] > addressed[candidate.first])
            fits &= fieldOf(schema, Access{.base = candidate.first, .offset = 0, .width = 4});
        for (const Access &item : items)
            fits &= fieldOf(schema, item);
        const int32_t start = frameOffset(*localSlotRefFromName(candidate.first));
        const int32_t end = start + static_cast<int32_t>(schema.size);
        fits &= end <= 0 && start % static_cast<int32_t>(schema.alignment) == 0;
        for (const int32_t object : objects)
            fits &= object <= start || object >= end;
        if (fits) {
            function.localTypes[candidate.first] = schema.name;
            changed = true;
        }
    }
    return changed;
}

void recoverCNarrowParameters(SourceFunctionAst &function) {
    std::map<uint32_t, std::set<uint8_t>> widths;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Identifier)
            return;
        if (const std::optional<uint32_t> argument = argSlotOffsetFromName(expression.text)) {
            widths[(*argument - 8U) / 4U].insert(4);
            return;
        }
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text);
        if (!slot || slot->aggregateField || slot->baseOffset > 0x7FU || slot->baseOffset < 8U || (slot->baseOffset - 8U) % 4U)
            return;
        widths[(slot->baseOffset - 8U) / 4U].insert(sourceIdentifierAccessWidth(expression.text));
    });
    std::map<uint32_t, uint8_t> narrow;
    for (const std::pair<const uint32_t, std::set<uint8_t>> &parameter : widths)
        if (parameter.second.size() == 1 && *parameter.second.begin() < 4)
            narrow[parameter.first] = *parameter.second.begin();
    if (narrow.empty())
        return;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Identifier)
            return;
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text);
        if (!slot || slot->aggregateField || slot->baseOffset > 0x7FU || slot->baseOffset < 8U || (slot->baseOffset - 8U) % 4U)
            return;
        const std::map<uint32_t, uint8_t>::const_iterator parameter = narrow.find((slot->baseOffset - 8U) / 4U);
        if (parameter != narrow.end())
            expression.text = "arg" + std::to_string(parameter->first);
    });
    while (function.params.size() <= narrow.rbegin()->first)
        function.params.push_back("arg" + std::to_string(function.params.size()));
    for (const std::pair<const uint32_t, uint8_t> &parameter : narrow)
        function.localTypes["arg" + std::to_string(parameter.first)] = parameter.second == 1 ? "int8_t" : "int16_t";
}

bool namesObject(const SourceExpr &expression, const std::string &name) {
    if (expression.kind == SourceExprKind::Identifier && expression.text.substr(0, expression.text.find('.')) == name)
        return true;
    for (const SourceExpr &child : expression.args)
        if (namesObject(child, name))
            return true;
    return false;
}

bool namesObjectItself(const SourceStmt &statement, const std::string &name) {
    bool found = namesObject(statement.expr, name) || namesObject(statement.value, name) || namesObject(statement.increment, name);
    for (const SourceSwitchCase &clause : statement.cases)
        found |= namesObject(clause.value, name);
    return found;
}

bool usesObject(const SourceStmt &statement, const std::string &name) {
    if (namesObjectItself(statement, name))
        return true;
    for (const std::vector<SourceStmt> *list : {&statement.initializer, &statement.body, &statement.elseBody})
        for (const SourceStmt &child : *list)
            if (usesObject(child, name))
                return true;
    for (const SourceSwitchCase &clause : statement.cases)
        for (const SourceStmt &child : clause.body)
            if (usesObject(child, name))
                return true;
    return false;
}

bool usesApart(const std::vector<SourceStmt> &list, const std::string &first, const std::string &second, bool top) {
    size_t lowFirst = SIZE_MAX, highFirst = 0, lowSecond = SIZE_MAX, highSecond = 0;
    for (size_t index = 0; index < list.size(); ++index) {
        if (usesObject(list[index], first)) {
            lowFirst = index < lowFirst ? index : lowFirst;
            highFirst = index;
        }
        if (usesObject(list[index], second)) {
            lowSecond = index < lowSecond ? index : lowSecond;
            highSecond = index;
        }
    }
    const auto block = [&](size_t low, size_t high, const std::string &name) {
        if (top && high + 1 == list.size() && list[high].kind == SourceStmtKind::Return)
            return false;
        return low != high || list[low].kind == SourceStmtKind::ExprStmt || list[low].kind == SourceStmtKind::Assign || !namesObjectItself(list[low], name);
    };
    if (lowFirst == SIZE_MAX || lowSecond == SIZE_MAX)
        return true;
    if (highFirst < lowSecond || highSecond < lowFirst)
        return block(lowFirst, highFirst, first) && block(lowSecond, highSecond, second);
    for (const SourceStmt &statement : list) {
        const bool usesFirst = usesObject(statement, first), usesSecond = usesObject(statement, second);
        if (usesFirst != usesSecond || (usesFirst && (namesObjectItself(statement, first) || namesObjectItself(statement, second))))
            return false;
        if (!usesFirst)
            continue;
        for (const std::vector<SourceStmt> *nested : {&statement.initializer, &statement.body, &statement.elseBody})
            if (!usesApart(*nested, first, second, false))
                return false;
        for (const SourceSwitchCase &clause : statement.cases)
            if (!usesApart(clause.body, first, second, false))
                return false;
    }
    return true;
}

void recoverCRecordMembers(SourceFunctionAst &function, bool typed) {
    const auto frameOffset = [&](const std::string &name) -> std::optional<int32_t> {
        if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(name); slot && !slot->aggregateField)
            return slot->baseOffset <= 0xFFU ? static_cast<int8_t>(slot->baseOffset) : static_cast<int16_t>(slot->baseOffset);
        const std::map<std::string, uint32_t>::const_iterator allocated = function.localOffsets.find(name);
        return allocated == function.localOffsets.end() ? std::nullopt : std::optional<int32_t>(static_cast<int32_t>(allocated->second));
    };

    std::set<std::string> viewed;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier && expression.text.find('.') != std::string::npos)
            viewed.insert(expression.text.substr(0, expression.text.find('.')));
    });
    const std::function<void(const std::vector<SourceStmt> &)> selectors = [&](const std::vector<SourceStmt> &list) {
        for (const SourceStmt &statement : list) {
            if (statement.kind == SourceStmtKind::Switch && statement.expr.kind == SourceExprKind::Identifier)
                viewed.insert(statement.expr.text);
            for (const std::vector<SourceStmt> *nested : {&statement.initializer, &statement.body, &statement.elseBody})
                selectors(*nested);
            for (const SourceSwitchCase &clause : statement.cases)
                selectors(clause.body);
        }
    };
    selectors(function.body.body);
    std::vector<std::pair<std::string, std::string>> pairs(function.frameMembers.begin(), function.frameMembers.end());
    if (typed) {
        std::set<std::string> words;
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier && expression.text.starts_with("local_") && expression.text.find('.') == std::string::npos)
                words.insert(expression.text);
        });
        pairs.clear();
        for (const std::string &word : words)
            for (const std::pair<const std::string, std::string> &record : function.localTypes)
                if (findScenarioDataSchema(record.second))
                    pairs.emplace_back(word, record.first);
    }
    for (const std::pair<std::string, std::string> &pair : pairs) {
        const std::string &word = pair.first;
        const std::map<std::string, std::string>::const_iterator record = function.localTypes.find(pair.second);
        const ScenarioDataSchema *schema = record != function.localTypes.end() ? findScenarioDataSchema(record->second) : nullptr;
        const std::optional<int32_t> start = schema ? frameOffset(record->first) : std::nullopt, offset = frameOffset(word);
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(word);
        if (!start || !offset || viewed.count(word) || (type != function.localTypes.end() && !scenarioDataTypeByName(type->second)))
            continue;
        const int32_t delta = offset.value() - start.value();
        const ScenarioDataField *member = nullptr;
        for (const ScenarioDataField &field : schema->fields)
            if (delta > 0 && static_cast<int32_t>(field.offset) == delta && field.elementCount == 1 && scenarioDataTypeSize(field.type) == 4)
                member = &field;
        const std::string cType = !member         ? "" :
            member->type == ScenarioDataType::F32 ? "float" :
            member->type == ScenarioDataType::U32 ? "uint32_t" :
                                                    "int32_t";
        if (!member || (typed && (type != function.localTypes.end() ? type->second : "int32_t") != cType) ||
            usesApart(function.body.body, record->first, word, true))
            continue;
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier && expression.text == word)
                expression = SourceExpr{.kind = SourceExprKind::Cast,
                    .text = "." + std::string(member->name),
                    .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = record->first}},
                    .cType = cType};
        });
        function.localTypes.erase(word);
    }
}

void recoverCScopedRecords(SourceFunctionAst &function) {
    struct Scope {
        uint32_t parent;
        std::vector<std::vector<SourceStmt> *> bodies;
        std::map<std::string, std::set<std::string>> types{};
        std::set<std::string> uses{};
    };
    std::vector<Scope> scopes{{0, {&function.body.body}}};
    const std::function<void(SourceExpr &, uint32_t)> inspect = [&](SourceExpr &expression, uint32_t scope) {
        if (expression.kind == SourceExprKind::Identifier) {
            const std::string base = expression.text.substr(0, expression.text.find('.'));
            if (localSlotRefFromName(base))
                scopes[scope].uses.insert(base);
        }
        if (expression.kind != SourceExprKind::Call)
            return;
        const abi::CommandSpec *command = abiCommandForCallName(expression.text);
        if (!command || !command->handlerName)
            return;
        std::vector<std::optional<uint32_t>> constants;
        for (const SourceExpr &argument : expression.args)
            constants.push_back(sourceDataConstantValue(argument));
        for (size_t index = 0; index < expression.args.size(); ++index) {
            const NativeArgumentType *argument = nativeArgumentForHandler(command->handlerName, index, constants);
            if (!argument || !argument->pointer || !findScenarioDataSchema(argument->type))
                continue;
            const SourceExpr &address = expression.args[index];
            if (address.kind != SourceExprKind::Unary || address.text != "&" || address.args.size() != 1 || address.args[0].kind != SourceExprKind::Identifier)
                continue;
            const std::optional<SourceSlotRef> slot = localSlotRefFromName(address.args[0].text);
            if (slot && !slot->aggregateField)
                scopes[scope].types[address.args[0].text].insert(argument->type);
        }
    };
    const std::function<void(std::vector<SourceStmt> &, uint32_t)> collect = [&](std::vector<SourceStmt> &body, uint32_t scope) {
        for (SourceStmt &statement : body) {
            visitExpressions(statement.expr, [&](SourceExpr &expression) { inspect(expression, scope); });
            visitExpressions(statement.value, [&](SourceExpr &expression) { inspect(expression, scope); });
            if (statement.kind == SourceStmtKind::Switch) {
                const uint32_t nested = static_cast<uint32_t>(scopes.size());
                scopes.push_back(Scope{scope, {&statement.body}});
                collect(statement.body, nested);
                for (SourceSwitchCase &clause : statement.cases) {
                    scopes[nested].bodies.push_back(&clause.body);
                    collect(clause.body, nested);
                }
            } else {
                for (std::vector<SourceStmt> *child : {&statement.body, &statement.elseBody})
                    if (!child->empty()) {
                        const uint32_t nested = static_cast<uint32_t>(scopes.size());
                        scopes.push_back(Scope{scope, {child}});
                        collect(*child, nested);
                    }
            }
        }
    };
    collect(function.body.body, 0);
    for (size_t index = scopes.size(); index > 1; --index) {
        const Scope &scope = scopes[index - 1];
        for (const std::pair<const std::string, std::set<std::string>> &types : scope.types)
            scopes[scope.parent].types[types.first].insert(types.second.begin(), types.second.end());
    }
    size_t objectIndex = 0;
    for (const std::pair<const std::string, std::set<std::string>> &slotTypes : scopes[0].types) {
        if (slotTypes.second.size() < 2)
            continue;
        std::vector<uint32_t> owners;
        for (uint32_t scope = 1; scope < scopes.size(); ++scope) {
            const std::map<std::string, std::set<std::string>>::const_iterator types = scopes[scope].types.find(slotTypes.first);
            if (types == scopes[scope].types.end() || types->second.size() != 1)
                continue;
            const std::map<std::string, std::set<std::string>>::const_iterator parent = scopes[scopes[scope].parent].types.find(slotTypes.first);
            if (parent == scopes[scopes[scope].parent].types.end() || parent->second.size() > 1)
                owners.push_back(scope);
        }
        bool complete = !owners.empty();
        for (uint32_t scope = 0; scope < scopes.size(); ++scope)
            if (scopes[scope].uses.count(slotTypes.first)) {
                bool owned = false;
                for (uint32_t owner : owners) {
                    uint32_t ancestor = scope;
                    while (ancestor && ancestor != owner)
                        ancestor = scopes[ancestor].parent;
                    if (ancestor == owner)
                        owned = true;
                }
                if (!owned)
                    complete = false;
            }
        if (!complete)
            continue;
        const uint32_t raw = localSlotRefFromName(slotTypes.first)->baseOffset;
        const int32_t offset = raw <= 0xffU ? static_cast<int8_t>(raw) : static_cast<int16_t>(raw);
        for (uint32_t owner : owners) {
            const std::string type = *scopes[owner].types.at(slotTypes.first).begin();
            std::string name;
            do {
                name = "object_" + std::to_string(objectIndex++);
            } while (function.localTypes.count(name));
            function.localTypes[name] = type;
            function.localOffsets[name] = static_cast<uint32_t>(offset);
            function.localDeclarations.push_back(SourceLocalDeclaration{.type = type, .name = name});
            for (std::vector<SourceStmt> *body : scopes[owner].bodies)
                visitExpressions(*body, [&](SourceExpr &expression) {
                    if (expression.kind != SourceExprKind::Identifier)
                        return;
                    const size_t dot = expression.text.find('.');
                    if (expression.text.substr(0, dot) == slotTypes.first)
                        expression.text = name + (dot == std::string::npos ? "" : expression.text.substr(dot));
                });
        }
        function.localTypes.erase(slotTypes.first);
    }
}

std::pair<uint32_t, uint32_t> cElementLayout(const std::string &type) {
    const size_t bracket = type.find('[');
    const std::string element = bracket == std::string::npos ? type : trimCopy(type.substr(0, bracket));
    uint32_t rows = 1;
    for (size_t open = bracket; open != std::string::npos; open = type.find('[', open + 1))
        rows *= static_cast<uint32_t>(std::stoul(type.substr(open + 1)));
    const ScenarioDataSchema *schema = findScenarioDataSchema(element);
    const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(element);
    const uint32_t size = schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 4U;
    return {rows * size, schema ? schema->alignment : size};
}

void recoverCLocalArrays(SourceFunctionAst &function) {
    struct Extent {
        uint32_t bytes = 0;
        bool floating = false;
        bool rejected = false;
        std::set<uint32_t> rows{};
    };

    struct Row {
        uint32_t size;
        const SourceExpr *index;
        uint32_t column;
    };
    const auto rowOffset = [](const SourceExpr &offset) -> std::optional<Row> {
        const SourceExpr *bytes = &offset;
        uint32_t column = 0;
        if (bytes->kind == SourceExprKind::Binary && bytes->text == "+" && bytes->args.size() == 2)
            if (const std::optional<uint32_t> constant = sourceDataConstantValue(bytes->args[1])) {
                column = *constant;
                bytes = &bytes->args[0];
            }
        if (bytes->kind != SourceExprKind::Binary || bytes->text != "*" || bytes->args.size() != 2)
            return std::nullopt;
        const std::optional<uint32_t> size = sourceDataConstantValue(bytes->args[1]);
        if (!size || !*size || sourceDataConstantValue(bytes->args[0]))
            return std::nullopt;
        return Row{.size = *size, .index = &bytes->args[0], .column = column};
    };
    std::map<std::string, Extent> extents;
    std::set<int32_t> objects;
    const auto frameOffset = [](const SourceSlotRef &slot) {
        return slot.baseOffset <= 0xffU ? static_cast<int32_t>(static_cast<int8_t>(slot.baseOffset)) :
                                          static_cast<int32_t>(static_cast<int16_t>(slot.baseOffset));
    };
    const auto addressed = [](const SourceExpr &address) -> const SourceExpr * {
        if (address.kind != SourceExprKind::Unary || address.text != "&" || address.args.size() != 1 || address.args[0].kind != SourceExprKind::Identifier ||
            address.args[0].text.find('.') != std::string::npos)
            return nullptr;
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(address.args[0].text);
        return slot && !slot->aggregateField ? &address.args[0] : nullptr;
    };
    const auto floatValue = [&](const SourceExpr &value) {
        if (value.kind == SourceExprKind::FloatLiteral)
            return true;
        if (value.kind == SourceExprKind::Unary && value.text == "-" && value.args.size() == 1 && value.args[0].kind == SourceExprKind::FloatLiteral)
            return true;
        if (value.kind == SourceExprKind::Call)
            return value.text == "as_float" || value.text == "vm_fadd" || value.text == "vm_fsub" || value.text == "vm_fmul" || value.text == "vm_fdiv" ||
                value.text == "vm_int_to_float";
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(value.text);
        return value.kind == SourceExprKind::Identifier && type != function.localTypes.end() && type->second == "float";
    };

    std::set<const SourceExpr *> byteAddresses;
    std::set<std::string> words;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Call && expression.args.size() == 1 && (expression.text == "vm_mem8" || expression.text == "vm_load8"))
            byteAddresses.insert(&expression.args[0]);
    });
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier)
            if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text))
                objects.insert(frameOffset(*slot));
        if (expression.kind != SourceExprKind::Binary || (expression.text != "+" && expression.text != "-") || expression.args.size() != 2)
            return;
        const SourceExpr *base = addressed(expression.args[0]);
        if (!base)
            return;
        if (!byteAddresses.count(&expression))
            words.insert(base->text);
        const std::optional<uint32_t> bytes = expression.text == "+" ? sourceDataConstantValue(expression.args[1]) : std::nullopt;
        const std::optional<Row> row = expression.text == "+" && !bytes ? rowOffset(expression.args[1]) : std::nullopt;
        Extent &extent = extents[base->text];
        if (row && row->column < row->size && row->column % 4U == 0)
            extent.rows.insert(row->size);
        else if (!bytes || *bytes > 0x7fffU)
            extent.rejected = true;
        else if (*bytes > extent.bytes)
            extent.bytes = *bytes;
    });
    const std::function<void(const std::vector<SourceStmt> &)> stores = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Assign && floatValue(statement.value)) {
                const SourceExpr &target = statement.expr;
                const SourceExpr *base = target.kind == SourceExprKind::Unary && target.text == "*" && target.args.size() == 1 &&
                        target.args[0].kind == SourceExprKind::Binary && target.args[0].args.size() == 2 ?
                    addressed(target.args[0].args[0]) :
                    nullptr;
                if (base && extents.count(base->text))
                    extents[base->text].floating = true;
                if (target.kind == SourceExprKind::Identifier && extents.count(target.text))
                    extents[target.text].floating = true;
            }
            stores(statement.body);
            stores(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases)
                stores(clause.body);
        }
    };
    stores(function.body.body);
    std::map<std::string, uint32_t> widths, rows;
    for (const std::pair<const std::string, Extent> &extent : extents) {

        if (!extent.second.rejected && extent.second.rows.size() == 1 && extent.second.bytes % 4U == 0) {
            const uint32_t size = *extent.second.rows.begin();
            const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(extent.first);
            bool declared = false;
            for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                declared |= declaration.name == extent.first;
            const int32_t start = frameOffset(*localSlotRefFromName(extent.first));
            int32_t end = 0;
            for (const int32_t object : objects)
                if (object > start && object < end)
                    end = object;
            const uint32_t extentBytes = static_cast<uint32_t>(end - start);
            if (declared || size % 4U || size / 4U < 2 || extentBytes % size || extentBytes / size < 1 || extent.second.bytes >= extentBytes ||
                (type != function.localTypes.end() && type->second != "int32_t" && type->second != "float"))
                continue;
            const std::string element = extent.second.floating || (type != function.localTypes.end() && type->second == "float") ? "float" : "int32_t";
            function.localDeclarations.push_back(SourceLocalDeclaration{
                .type = element + "[" + std::to_string(size / 4U) + "]", .name = extent.first, .count = extentBytes / size, .isArray = true});
            function.localTypes[extent.first] = element;
            rows[extent.first] = size;
            continue;
        }
        if (extent.second.rejected || !extent.second.bytes || extent.second.bytes % 4U || !words.count(extent.first))
            continue;
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(extent.first);
        if (type != function.localTypes.end() && type->second != "int32_t" && type->second != "float")
            continue;
        bool declared = false;
        for (const SourceLocalDeclaration &declaration : function.localDeclarations)
            declared |= declaration.name == extent.first;
        if (declared)
            continue;
        const int32_t start = frameOffset(*localSlotRefFromName(extent.first));
        const uint32_t count = extent.second.bytes / 4U + 1U;
        const int32_t end = start + static_cast<int32_t>(count * 4U);
        bool overlaps = end > 0;
        for (const int32_t object : objects)
            overlaps |= object > start && object < end;
        if (overlaps)
            continue;
        const std::string element = extent.second.floating || (type != function.localTypes.end() && type->second == "float") ? "float" : "int32_t";
        function.localDeclarations.push_back(SourceLocalDeclaration{.type = element, .name = extent.first, .count = count, .isArray = true});
        function.localTypes[extent.first] = element;
        widths[extent.first] = 4U;
    }
    if (widths.empty() && rows.empty())
        return;
    const auto element = [](const std::string &name, uint32_t position) {
        return SourceExpr{.kind = SourceExprKind::Index,
            .text = "[]",
            .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = name},
                SourceExpr{.kind = SourceExprKind::IntLiteral, .text = std::to_string(position)}}};
    };
    const auto index = [](SourceExpr base, SourceExpr position) {
        return SourceExpr{.kind = SourceExprKind::Index, .text = "[]", .args = {std::move(base), std::move(position)}};
    };
    const auto number = [](uint32_t value) { return SourceExpr{.kind = SourceExprKind::IntLiteral, .text = std::to_string(value)}; };

    const auto rowElement = [&](const SourceExpr &address, SourceExpr &row, uint32_t &column) -> bool {
        const bool offset = address.kind == SourceExprKind::Binary && address.text == "+" && address.args.size() == 2;
        const SourceExpr *base = offset ? addressed(address.args[0]) : addressed(address);
        if (!base || !rows.count(base->text))
            return false;
        const uint32_t size = rows.at(base->text);
        if (!offset) {
            row = element(base->text, 0);
            column = 0;
            return true;
        }
        if (const std::optional<uint32_t> constant = sourceDataConstantValue(address.args[1])) {
            row = element(base->text, *constant / size);
            column = *constant % size / 4U;
            return true;
        }
        const std::optional<Row> selected = rowOffset(address.args[1]);
        if (!selected)
            return false;
        row = index(SourceExpr{.kind = SourceExprKind::Identifier, .text = base->text}, *selected->index);
        column = selected->column / 4U;
        return true;
    };

    const std::function<void(SourceExpr &)> rewrite = [&](SourceExpr &expression) {
        const SourceExpr *base = nullptr;
        SourceExpr row;
        uint32_t column = 0;
        if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 && rowElement(expression.args[0], row, column)) {
            rewrite(row.args[1]);
            expression = index(std::move(row), number(column));
            return;
        }
        if (rowElement(expression, row, column)) {
            rewrite(row.args[1]);
            expression = column ? SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {index(std::move(row), number(column))}} : std::move(row);
            return;
        }
        if (expression.kind == SourceExprKind::Identifier && rows.count(expression.text)) {
            expression = index(element(expression.text, 0), number(0));
            return;
        }
        if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 &&
            expression.args[0].kind == SourceExprKind::Binary && expression.args[0].text == "+" && expression.args[0].args.size() == 2 &&
            (base = addressed(expression.args[0].args[0])) && widths.count(base->text)) {
            expression = element(base->text, *sourceDataConstantValue(expression.args[0].args[1]) / widths.at(base->text));
            return;
        }
        if (expression.kind == SourceExprKind::Binary && expression.text == "+" && expression.args.size() == 2 && (base = addressed(expression.args[0])) &&
            widths.count(base->text)) {
            expression = SourceExpr{.kind = SourceExprKind::Unary,
                .text = "&",
                .args = {element(base->text, *sourceDataConstantValue(expression.args[1]) / widths.at(base->text))}};
            return;
        }
        if ((base = addressed(expression)) && widths.count(base->text)) {
            SourceExpr decayed = *base;
            expression = std::move(decayed);
            return;
        }
        if (expression.kind == SourceExprKind::Identifier && widths.count(expression.text)) {
            expression = element(expression.text, 0);
            return;
        }
        for (SourceExpr &argument : expression.args)
            rewrite(argument);
    };
    const std::function<void(std::vector<SourceStmt> &)> statements = [&](std::vector<SourceStmt> &list) {
        for (SourceStmt &statement : list) {
            if (!statement.expr.text.empty() || !statement.expr.args.empty())
                rewrite(statement.expr);
            if (!statement.value.text.empty() || !statement.value.args.empty())
                rewrite(statement.value);
            statements(statement.body);
            statements(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                statements(clause.body);
        }
    };
    statements(function.body.body);
}

void recoverCByteStorage(SourceFunctionAst &function) {
    std::map<std::string, SourceLocalDeclaration> arrays;
    const std::function<void(const std::vector<SourceStmt> &)> discover = [&](const std::vector<SourceStmt> &statements) {
        for (size_t i = 0; i < statements.size(); ++i) {
            const SourceStmt &loop = statements[i];
            discover(loop.body);
            discover(loop.elseBody);
            for (const SourceSwitchCase &clause : loop.cases)
                discover(clause.body);
            if (loop.kind != SourceStmtKind::While || loop.expr.kind != SourceExprKind::Binary || loop.expr.text != "<" || loop.expr.args.size() != 2)
                continue;
            const SourceExpr &index = loop.expr.args[0];
            const std::optional<uint32_t> count = sourceDataConstantValue(loop.expr.args[1]);
            if (index.kind != SourceExprKind::Identifier || !count || !*count || *count > 32768U)
                continue;
            size_t prior = i;
            while (prior && statements[prior - 1].kind == SourceStmtKind::Label)
                --prior;
            if (!prior || statements[prior - 1].kind != SourceStmtKind::Assign || !sourceExprStructurallyEquals(statements[prior - 1].expr, index) ||
                sourceDataConstantValue(statements[prior - 1].value) != std::optional<uint32_t>(0))
                continue;
            uint32_t writes = 0;
            bool increment = false;
            for (const SourceStmt &statement : loop.body)
                if (statement.kind == SourceStmtKind::Assign && sourceExprStructurallyEquals(statement.expr, index)) {
                    ++writes;
                    increment = statement.value.kind == SourceExprKind::Binary && statement.value.text == "+" && statement.value.args.size() == 2 &&
                        sourceExprStructurallyEquals(statement.value.args[0], index) &&
                        sourceDataConstantValue(statement.value.args[1]) == std::optional<uint32_t>(1);
                }
            const uint8_t indexWidth = sourceIdentifierAccessWidth(index.text);
            if (*count > (indexWidth == 1 ? 127U : indexWidth == 2 ? 32767U : 0x7fffffffU))
                continue;
            bool unsafe = false;
            uint32_t allWrites = 0;
            const std::function<void(const std::vector<SourceStmt> &)> inspect = [&](const std::vector<SourceStmt> &body) {
                for (const SourceStmt &statement : body) {
                    if (statement.kind == SourceStmtKind::Return || statement.kind == SourceStmtKind::Goto || statement.kind == SourceStmtKind::Break ||
                        statement.kind == SourceStmtKind::Continue)
                        unsafe = true;
                    if (statement.kind == SourceStmtKind::Assign && sourceExprStructurallyEquals(statement.expr, index))
                        ++allWrites;
                    inspect(statement.body);
                    inspect(statement.elseBody);
                    for (const SourceSwitchCase &clause : statement.cases)
                        inspect(clause.body);
                }
            };
            inspect(loop.body);
            SourceStmt copy = loop;
            visitExpressions(copy.body, [&](SourceExpr &expression) {
                if (expression.kind == SourceExprKind::Binary && expression.text == "=" && !expression.args.empty() &&
                    sourceExprStructurallyEquals(expression.args[0], index))
                    unsafe = true;
                if (expression.kind == SourceExprKind::Unary && expression.text == "&" && !expression.args.empty() &&
                    sourceExprStructurallyEquals(expression.args[0], index))
                    unsafe = true;
            });
            if (writes != 1 || allWrites != 1 || !increment || unsafe)
                continue;
            visitExpressions(copy.body, [&](SourceExpr &expression) {
                if (expression.kind != SourceExprKind::Call || (expression.text != "vm_mem8" && expression.text != "vm_load8") || expression.args.size() != 1)
                    return;
                const SourceExpr &address = expression.args[0];
                if (address.kind != SourceExprKind::Binary || address.text != "+" || address.args.size() != 2 ||
                    address.args[0].kind != SourceExprKind::Unary || address.args[0].text != "&" || address.args[0].args.size() != 1 ||
                    !sourceExprStructurallyEquals(address.args[1], index))
                    return;
                const SourceExpr &base = address.args[0].args[0];
                const std::optional<SourceSlotRef> slot = localSlotRefFromName(base.text);
                if (!slot || slot->aggregateField || base.kind != SourceExprKind::Identifier)
                    return;
                const int32_t offset = slot->baseOffset <= 0xffU ? static_cast<int8_t>(slot->baseOffset) : static_cast<int16_t>(slot->baseOffset);
                if (offset >= 0 || *count > static_cast<uint32_t>(-offset))
                    return;
                arrays[base.text] = SourceLocalDeclaration{.type = "int8_t", .name = base.text, .count = *count, .isArray = true};
            });
        }
    };
    discover(function.body.body);

    std::map<std::string, uint32_t> uses, indexed, addresses;
    std::map<std::string, std::set<std::string>> offsets;
    std::set<std::string> variable, words;
    std::set<int32_t> objects;
    const auto frameOffset = [](const SourceSlotRef &slot) {
        return slot.baseOffset <= 0xffU ? static_cast<int32_t>(static_cast<int8_t>(slot.baseOffset)) :
                                          static_cast<int32_t>(static_cast<int16_t>(slot.baseOffset));
    };
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier)
            if (const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text)) {
                ++uses[stripSourceWidthSuffix(expression.text)];
                objects.insert(frameOffset(*slot));
                if (sourceIdentifierAccessWidth(expression.text) == 1 && !slot->aggregateField) {
                    ++indexed[stripSourceWidthSuffix(expression.text)];
                    offsets[stripSourceWidthSuffix(expression.text)].insert("0");
                }
            }
        if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1 &&
            expression.args[0].kind == SourceExprKind::Identifier)
            ++addresses[expression.args[0].text];

        if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1) {
            const SourceExpr &address = expression.args[0];
            const SourceExpr &base = address.kind == SourceExprKind::Binary && address.text == "+" && address.args.size() == 2 ? address.args[0] : address;
            if (base.kind == SourceExprKind::Unary && base.text == "&" && base.args.size() == 1 && base.args[0].kind == SourceExprKind::Identifier)
                words.insert(base.args[0].text);
        }
        if (expression.kind != SourceExprKind::Call || (expression.text != "vm_mem8" && expression.text != "vm_load8") || expression.args.size() != 1)
            return;
        const SourceExpr &address = expression.args[0];
        const bool offset = address.kind == SourceExprKind::Binary && address.text == "+" && address.args.size() == 2;
        const SourceExpr &base = offset ? address.args[0] : address;
        if (base.kind != SourceExprKind::Unary || base.text != "&" || base.args.size() != 1 || base.args[0].kind != SourceExprKind::Identifier)
            return;
        ++indexed[base.args[0].text];
        --addresses[base.args[0].text];
        const std::optional<uint32_t> constant = offset ? sourceDataConstantValue(address.args[1]) : std::optional<uint32_t>(0);
        offsets[base.args[0].text].insert(constant ? std::to_string(*constant) : "*");
        if (offset && !constant)
            variable.insert(base.args[0].text);
    });
    for (const std::pair<const std::string, uint32_t> &base : indexed) {
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(base.first);
        const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(base.first);
        bool declared = arrays.count(base.first) || (type != function.localTypes.end() && type->second != "int32_t");
        for (const SourceLocalDeclaration &declaration : function.localDeclarations)
            declared |= declaration.name == base.first;
        if (!slot || slot->aggregateField || declared || words.count(base.first) || uses[base.first] != base.second + addresses[base.first] ||
            (!variable.count(base.first) && offsets[base.first].size() < 2))
            continue;
        const int32_t start = frameOffset(*slot);
        int32_t end = 0;
        for (const int32_t object : objects)
            if (object > start && object < end)
                end = object;
        if (start >= end)
            continue;
        arrays[base.first] = SourceLocalDeclaration{.type = "int8_t", .name = base.first, .count = static_cast<uint32_t>(end - start), .isArray = true};
    }
    for (const std::pair<const std::string, SourceLocalDeclaration> &array : arrays) {
        function.localDeclarations.push_back(array.second);
        function.localTypes[array.first] = array.second.type;
    }
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier && arrays.count(stripSourceWidthSuffix(expression.text)) &&
            sourceIdentifierAccessWidth(expression.text) == 1) {
            const SourceExpr base{.kind = SourceExprKind::Identifier, .text = stripSourceWidthSuffix(expression.text)};
            expression = SourceExpr{.kind = SourceExprKind::Index, .text = "[]", .args = {base, SourceExpr{.kind = SourceExprKind::IntLiteral, .text = "0"}}};
            return;
        }
        if (expression.kind == SourceExprKind::Identifier && localSlotRefFromName(expression.text) && sourceIdentifierAccessWidth(expression.text) < 4) {
            const uint8_t width = sourceIdentifierAccessWidth(expression.text);
            expression.text = stripSourceWidthSuffix(expression.text);

            const std::map<std::string, std::string>::const_iterator type = function.localTypes.find(expression.text);
            if (const ScenarioDataSchema *record = type != function.localTypes.end() ? findScenarioDataSchema(type->second) : nullptr) {
                for (const ScenarioDataField &field : record->fields)
                    if (field.offset == 0 && field.elementCount == 1 && scenarioDataTypeSize(field.type) == width) {
                        expression.text += "." + std::string(field.name);
                        break;
                    }
                return;
            }
            function.localTypes[expression.text] = width == 1 ? "int8_t" : "int16_t";
        }
        if (expression.kind != SourceExprKind::Call || (expression.text != "vm_mem8" && expression.text != "vm_load8") || expression.args.size() != 1)
            return;
        const SourceExpr &address = expression.args[0];
        if (address.kind == SourceExprKind::Unary && address.text == "&" && address.args.size() == 1 && arrays.count(address.args[0].text)) {
            const SourceExpr base = address.args[0];
            expression = SourceExpr{.kind = SourceExprKind::Index, .text = "[]", .args = {base, SourceExpr{.kind = SourceExprKind::IntLiteral, .text = "0"}}};
            return;
        }
        if (address.kind != SourceExprKind::Binary || address.text != "+" || address.args.size() != 2 || address.args[0].kind != SourceExprKind::Unary ||
            address.args[0].text != "&" || address.args[0].args.size() != 1)
            return;
        const SourceExpr &base = address.args[0].args[0];
        if (!arrays.count(base.text))
            return;
        expression = SourceExpr{.kind = SourceExprKind::Index, .text = "[]", .args = {base, address.args[1]}};
    });
}

bool recoverCLexicalStorage(SourceFunctionAst &function, const std::multimap<uint32_t, SourceLocalDeclaration> &original, uint32_t reserved) {
    struct Scope {
        uint32_t parent;
        std::vector<SourceStmt> *statements;
        std::vector<SourceLocalDeclaration> declarations{};
    };
    std::multimap<uint32_t, SourceLocalDeclaration> locals = original;
    bool overlap = false;
    std::set<std::string> scopedObjects;
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &first : locals) {
        const uint32_t firstSize = cElementLayout(first.second.type).first * first.second.count;
        for (const std::pair<const uint32_t, SourceLocalDeclaration> &second : locals) {
            if (first.second.name >= second.second.name)
                continue;
            const uint32_t secondSize = cElementLayout(second.second.type).first * second.second.count;
            if (static_cast<int64_t>(first.first) > static_cast<int64_t>(second.first) - secondSize &&
                static_cast<int64_t>(second.first) > static_cast<int64_t>(first.first) - firstSize) {
                overlap = true;
                scopedObjects.insert(first.second.name);
                scopedObjects.insert(second.second.name);
            }
        }
    }
    if (!overlap)
        return false;

    std::map<std::string, std::set<std::string>> conflicts;
    const std::function<void()> findConflicts = [&]() {
        conflicts.clear();
        for (const std::pair<const uint32_t, SourceLocalDeclaration> &first : locals)
            for (const std::pair<const uint32_t, SourceLocalDeclaration> &second : locals) {
                if (first.second.name == second.second.name || !scopedObjects.count(first.second.name) || !scopedObjects.count(second.second.name))
                    continue;
                const uint32_t firstSize = cElementLayout(first.second.type).first * first.second.count;
                const uint32_t secondSize = cElementLayout(second.second.type).first * second.second.count;
                if (static_cast<int64_t>(first.first) > static_cast<int64_t>(second.first) - secondSize &&
                    static_cast<int64_t>(second.first) > static_cast<int64_t>(first.first) - firstSize)
                    conflicts[first.second.name].insert(second.second.name);
            }
    };
    findConflicts();
    const std::function<bool(SourceExpr &, const std::string &)> mentions = [&](SourceExpr &expression, const std::string &name) {
        if (expression.kind == SourceExprKind::Identifier && expression.text.substr(0, expression.text.find('.')) == name)
            return true;
        for (SourceExpr &child : expression.args)
            if (mentions(child, name))
                return true;
        return false;
    };
    const std::function<bool(SourceStmt &, const std::string &)> uses = [&](SourceStmt &statement, const std::string &name) {
        if (mentions(statement.expr, name) || mentions(statement.value, name) || mentions(statement.increment, name))
            return true;
        for (std::vector<SourceStmt> *list : {&statement.body, &statement.elseBody, &statement.initializer})
            for (SourceStmt &child : *list)
                if (uses(child, name))
                    return true;
        for (SourceSwitchCase &clause : statement.cases) {
            if (mentions(clause.value, name))
                return true;
            for (SourceStmt &child : clause.body)
                if (uses(child, name))
                    return true;
        }
        return false;
    };
    const std::function<void(std::vector<SourceStmt> &)> separate = [&](std::vector<SourceStmt> &list) {
        std::map<std::string, std::pair<size_t, size_t>> runs;
        for (const std::string &name : scopedObjects)
            for (size_t index = 0; index < list.size(); ++index)
                if (uses(list[index], name)) {
                    std::pair<size_t, size_t> &run = runs.try_emplace(name, index, index).first->second;
                    run.second = index;
                }
        std::set<std::string> separated;
        for (const std::pair<const std::string, std::pair<size_t, size_t>> &run : runs)
            for (const std::string &other : conflicts[run.first]) {
                const std::map<std::string, std::pair<size_t, size_t>>::const_iterator second = runs.find(other);
                if (second != runs.end() && (run.second.second < second->second.first || second->second.second < run.second.first)) {
                    separated.insert(run.first);
                    separated.insert(other);
                }
            }

        const auto depth = [&](const std::string &name) -> uint32_t {
            for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
                if (local.second.name == name)
                    return local.first;
            return 0;
        };

        for (const std::pair<const uint32_t, SourceLocalDeclaration> &below : locals) {
            std::pair<size_t, size_t> run{SIZE_MAX, 0};
            for (size_t index = 0; index < list.size(); ++index)
                if (uses(list[index], below.second.name))
                    run = {run.first == SIZE_MAX ? index : run.first, index};
            if (run.first == SIZE_MAX || separated.count(below.second.name))
                continue;
            const uint32_t bottom = below.first - cElementLayout(below.second.type).first * below.second.count;
            for (const std::pair<const uint32_t, SourceLocalDeclaration> &above : locals) {
                const std::map<std::string, std::pair<size_t, size_t>>::const_iterator other = runs.find(above.second.name);
                if (above.first > bottom || other == runs.end() || !(other->second.second < run.first || run.second < other->second.first))
                    continue;
                separated.insert(below.second.name);
                runs[below.second.name] = run;
                scopedObjects.insert(below.second.name);
                break;
            }
        }
        std::map<std::string, std::pair<size_t, size_t>> candidates = runs;
        if (!separated.empty())
            for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
                for (size_t index = 0; !scopedObjects.count(local.second.name) && index < list.size(); ++index)
                    if (uses(list[index], local.second.name)) {
                        std::pair<size_t, size_t> &run = candidates.try_emplace(local.second.name, index, index).first->second;
                        run.second = index;
                    }
        for (bool grown = !separated.empty(); grown;) {
            grown = false;
            for (const std::pair<const std::string, std::pair<size_t, size_t>> &run : candidates) {
                if (separated.count(run.first))
                    continue;
                for (const std::string &block : std::set<std::string>(separated)) {
                    const std::pair<size_t, size_t> &range = runs.at(block);
                    if (depth(run.first) > depth(block) && run.second.first <= range.second && range.first <= run.second.second &&
                        !conflicts[run.first].count(block)) {
                        separated.insert(run.first);
                        runs[run.first] = run.second;
                        scopedObjects.insert(run.first);
                        grown = true;
                        break;
                    }
                }
            }
        }
        std::set<std::pair<size_t, size_t>> blocks;
        for (const std::string &name : separated)
            blocks.insert(runs.at(name));
        std::vector<std::pair<size_t, size_t>> merged;
        for (const std::pair<size_t, size_t> &block : blocks)
            if (!merged.empty() && block.first <= merged.back().second)
                merged.back().second = std::max(merged.back().second, block.second);
            else
                merged.push_back(block);
        for (size_t index = merged.size(); index > 0; --index) {
            const std::pair<size_t, size_t> &block = merged[index - 1];

            if (block.first == 0 && block.second + 1 == list.size())
                continue;
            bool spans = false;
            if (block.first == block.second)
                for (const std::string &name : separated) {
                    if (runs.at(name) != block)
                        continue;
                    SourceStmt &statement = list[block.first];
                    uint32_t lists = 0;
                    for (std::vector<SourceStmt> *child : {&statement.initializer, &statement.body, &statement.elseBody}) {
                        bool used = false;
                        for (SourceStmt &nested : *child)
                            used |= uses(nested, name);
                        lists += used ? 1U : 0U;
                    }
                    for (SourceSwitchCase &clause : statement.cases) {
                        bool used = false;
                        for (SourceStmt &nested : clause.body)
                            used |= uses(nested, name);
                        lists += used ? 1U : 0U;
                    }
                    spans |= lists > 1;
                }
            if (block.first == block.second && !spans && list[block.first].kind != SourceStmtKind::ExprStmt && list[block.first].kind != SourceStmtKind::Assign)
                continue;
            SourceStmt scope{.kind = SourceStmtKind::Block};
            scope.body.assign(std::make_move_iterator(list.begin() + static_cast<std::ptrdiff_t>(block.first)),
                std::make_move_iterator(list.begin() + static_cast<std::ptrdiff_t>(block.second + 1)));
            list.erase(list.begin() + static_cast<std::ptrdiff_t>(block.first), list.begin() + static_cast<std::ptrdiff_t>(block.second + 1));
            list.insert(list.begin() + static_cast<std::ptrdiff_t>(block.first), std::move(scope));
        }

        for (SourceStmt &statement : list) {
            if (statement.kind != SourceStmtKind::Switch)
                continue;
            std::vector<std::set<std::string>> clauses;
            for (SourceSwitchCase &clause : statement.cases) {
                std::set<std::string> &used = clauses.emplace_back();
                for (const std::string &name : scopedObjects)
                    for (SourceStmt &child : clause.body)
                        if (uses(child, name))
                            used.insert(name);
            }
            for (size_t index = 0; index < statement.cases.size(); ++index) {
                bool shared = false;
                for (size_t other = 0; other < clauses.size(); ++other)
                    for (const std::string &name : clauses[index])
                        for (const std::string &conflict : conflicts[name])
                            shared |= other != index && clauses[other].count(conflict);
                std::vector<SourceStmt> &body = statement.cases[index].body;
                if (!shared || (body.size() == 1 && body.front().kind == SourceStmtKind::Block))
                    continue;
                SourceStmt scope{.kind = SourceStmtKind::Block, .body = std::move(body)};
                body.clear();
                body.push_back(std::move(scope));
            }
        }
        for (SourceStmt &statement : list) {
            separate(statement.body);
            separate(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                separate(clause.body);
        }
    };

    separate(function.body.body);
    std::vector<Scope> scopes{{0, &function.body.body}};
    std::map<std::string, uint32_t> useScopes;
    const std::function<uint32_t(uint32_t, uint32_t)> commonScope = [&](uint32_t first, uint32_t second) {
        std::set<uint32_t> ancestors;
        for (;;) {
            ancestors.insert(first);
            if (!first)
                break;
            first = scopes[first].parent;
        }
        while (!ancestors.count(second))
            second = scopes[second].parent;
        return second;
    };
    const std::function<void(SourceExpr &, uint32_t)> expressionUses = [&](SourceExpr &expression, uint32_t scope) {
        if (expression.kind == SourceExprKind::Identifier &&
            (localSlotRefFromName(expression.text) || function.localOffsets.count(expression.text.substr(0, expression.text.find('.'))))) {
            const std::string name = expression.text.substr(0, expression.text.find('.'));
            const std::map<std::string, uint32_t>::iterator previous = useScopes.find(name);
            if (previous == useScopes.end())
                useScopes.emplace(name, scope);
            else
                previous->second = commonScope(previous->second, scope);
        }
        for (SourceExpr &child : expression.args)
            expressionUses(child, scope);
    };
    const std::function<void(std::vector<SourceStmt> &, uint32_t)> collect = [&](std::vector<SourceStmt> &statements, uint32_t scope) {
        for (SourceStmt &statement : statements) {
            expressionUses(statement.expr, scope);
            expressionUses(statement.value, scope);
            if (statement.kind == SourceStmtKind::Switch) {
                const uint32_t nested = static_cast<uint32_t>(scopes.size());
                scopes.push_back(Scope{scope, &statement.body});
                collect(statement.body, nested);
                for (SourceSwitchCase &clause : statement.cases) {
                    expressionUses(clause.value, scope);
                    collect(clause.body, nested);
                }
            } else {
                if (!statement.body.empty()) {
                    const uint32_t nested = static_cast<uint32_t>(scopes.size());
                    scopes.push_back(Scope{scope, &statement.body});
                    collect(statement.body, nested);
                }
                if (!statement.elseBody.empty()) {
                    const uint32_t nested = static_cast<uint32_t>(scopes.size());
                    scopes.push_back(Scope{scope, &statement.elseBody});
                    collect(statement.elseBody, nested);
                }
            }
        }
    };
    collect(function.body.body, 0);

    uint32_t used = 0, deepest = 0;
    std::set<std::string> fillers;
    const auto unused = [&](size_t scope, uint32_t bytes) {
        const uint32_t start = used + bytes;
        const uint32_t encoded = start <= 0x80U ? (0x100U - start) & 0xFFU : (0x10000U - start) & 0xFFFFU;
        const bool words = used % 4U == 0 && bytes % 4U == 0;
        SourceLocalDeclaration declaration{.type = words ? "int32_t" : "uint8_t", .name = "unused_" + hexName(encoded), .count = words ? bytes / 4U : bytes};
        declaration.isArray = declaration.count > 1;

        for (uint32_t copy = 2; function.localTypes.count(declaration.name); ++copy)
            declaration.name = "unused_" + hexName(encoded) + "v" + std::to_string(copy);
        scopes[scope].declarations.push_back(declaration);
        function.localDeclarations.push_back(declaration);
        function.localTypes[declaration.name] = declaration.type;
        fillers.insert(declaration.name);
        used = start;
    };

    uint32_t nested = UINT32_MAX;
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
        if (scopedObjects.count(local.second.name) && useScopes.count(local.second.name) && useScopes.at(local.second.name))
            nested = std::min(nested, local.first - cElementLayout(local.second.type).first * local.second.count);
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
        if (!scopedObjects.count(local.second.name) && useScopes.count(local.second.name) &&
            local.first - cElementLayout(local.second.type).first * local.second.count >= nested)
            scopedObjects.insert(local.second.name);

    std::map<std::string, uint32_t> declaredIn;
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
        declaredIn[local.second.name] = scopedObjects.count(local.second.name) && useScopes.count(local.second.name) ? useScopes.at(local.second.name) : 0;
    const auto encloses = [&](uint32_t outer, uint32_t inner) {
        while (inner && inner != outer)
            inner = scopes[inner].parent;
        return inner == outer;
    };
    for (bool hoisted = true; hoisted;) {
        hoisted = false;
        for (const std::pair<const uint32_t, SourceLocalDeclaration> &above : locals)
            for (const std::pair<const uint32_t, SourceLocalDeclaration> &below : locals) {
                const uint32_t outer = declaredIn.at(below.second.name), inner = declaredIn.at(above.second.name);
                if (below.first - cElementLayout(below.second.type).first * below.second.count >= above.first && outer != inner && encloses(outer, inner)) {
                    declaredIn[above.second.name] = outer;
                    hoisted = true;
                }
            }
    }
    std::vector<std::vector<std::pair<uint32_t, SourceLocalDeclaration>>> scoped(scopes.size());
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals) {
        scoped[declaredIn.at(local.second.name)].push_back(local);
        deepest = std::max(deepest, local.first);
    }
    std::vector<uint32_t> ends(scopes.size(), 0);
    for (size_t scope = 0; scope < scopes.size(); ++scope) {
        used = scope ? ends[scopes[scope].parent] : 0;
        for (const std::pair<uint32_t, SourceLocalDeclaration> &local : scoped[scope]) {
            const uint32_t bytes = cElementLayout(local.second.type).first * local.second.count;
            if (local.first >= bytes && local.first - bytes > used)
                unused(scope, local.first - bytes - used);
            used = std::max(used, local.first);
            scopes[scope].declarations.push_back(local.second);
        }
        ends[scope] = used;
    }
    used = ends[0];
    if (used == deepest && reserved > used)
        unused(0, reserved - used);
    for (size_t index = scopes.size(); index > 0; --index) {
        Scope &scope = scopes[index - 1];
        std::vector<SourceStmt> declarations;
        for (const SourceLocalDeclaration &declaration : scope.declarations)
            declarations.push_back(SourceStmt{.kind = SourceStmtKind::Declaration, .declaration = declaration});
        scope.statements->insert(scope.statements->begin(), declarations.begin(), declarations.end());
    }
    SourceFunctionAst candidate = function;
    candidate.semantics = SourceSemantics::C11;
    candidate.explicitFrameAdjust.reset();
    candidate.explicitBase.reset();
    candidate.declaredOffset.reset();

    const std::function<void(std::vector<SourceStmt> &)> unfill = [&](std::vector<SourceStmt> &list) {
        for (size_t index = list.size(); index > 0; --index)
            if (list[index - 1].kind == SourceStmtKind::Declaration && fillers.count(list[index - 1].declaration.name))
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(index - 1));
        for (SourceStmt &statement : list) {
            unfill(statement.body);
            unfill(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                unfill(clause.body);
        }
    };
    const auto discard = [&]() {
        unfill(function.body.body);
        for (size_t index = function.localDeclarations.size(); index > 0; --index)
            if (fillers.count(function.localDeclarations[index - 1].name))
                function.localDeclarations.erase(function.localDeclarations.begin() + static_cast<std::ptrdiff_t>(index - 1));
        for (const std::string &name : fillers)
            function.localTypes.erase(name);
        return false;
    };
    SourceFunctionAst allocated;
    try {
        allocated = allocateCLocals(candidate);
    } catch (...) {
        discard();
        throw;
    }
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals)
        if (allocated.localOffsets.at(local.second.name) != 0U - local.first)
            return discard();
    const uint32_t frame = *allocated.explicitFrameAdjust;
    const uint32_t actual =
        static_cast<uint32_t>(-(frame <= 0xffU ? static_cast<int32_t>(static_cast<int8_t>(frame)) : static_cast<int32_t>(static_cast<int16_t>(frame))));
    return actual == reserved || discard();
}

void recoverCWorkspaceValues(SourceUnit &unit) {
    std::map<std::string, std::set<uint8_t>> widths;
    std::map<std::string, std::pair<uint32_t, uint32_t>> ranges;
    for (const std::pair<const std::string, SourceDataBinding> &binding : unit.workspaceBindings) {
        const std::optional<SourceSlotRef> slot = taskSlotRefFromName(binding.first);
        const ScenarioDataSchema *record = findScenarioDataSchema(binding.second.type);
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(binding.second.type);
        if (slot && (record || dataType))
            ranges[binding.first] = {
                slot->byteOffset, (record ? record->size : scenarioDataTypeSize(*dataType)) * (binding.second.isArray ? binding.second.elementCount : 1U)};
    }

    std::map<std::string, uint32_t> uses, addressUses;
    std::map<std::string, uint8_t> narrowest;
    const auto addressedSlot = [](const SourceExpr &address) -> const SourceExpr * {
        const SourceExpr *base = &address;
        if (base->kind == SourceExprKind::Binary && (base->text == "+" || base->text == "-") && base->args.size() == 2)
            base = &base->args[0];
        if (base->kind != SourceExprKind::Unary || base->text != "&" || base->args.size() != 1 || base->args[0].kind != SourceExprKind::Identifier)
            return nullptr;
        const std::optional<SourceSlotRef> slot = taskSlotRefFromName(base->args[0].text);
        return slot && !slot->aggregateField && base->args[0].text.find('.') == std::string::npos ? &base->args[0] : nullptr;
    };
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier && taskSlotRefFromName(expression.text))
                ++uses[stripSourceWidthSuffix(expression.text)];
            if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1)
                if (const SourceExpr *base = addressedSlot(expression)) {
                    ++addressUses[base->text];
                    narrowest.try_emplace(base->text, 4);
                }
            const bool byte = expression.text == "vm_load8" || expression.text == "vm_mem8";
            if (expression.kind == SourceExprKind::Call && expression.args.size() == 1 &&
                (byte || expression.text == "vm_load16" || expression.text == "vm_mem16"))
                if (const SourceExpr *base = addressedSlot(expression.args[0])) {
                    uint8_t &width = narrowest.try_emplace(base->text, 4).first->second;
                    width = std::min<uint8_t>(width, byte ? 1 : 2);
                }
        });
    const auto addressOnly = [&](const std::string &name) { return addressUses.count(name) && uses.at(name) == addressUses.at(name); };
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Identifier)
                return;
            const std::optional<SourceSlotRef> slot = taskSlotRefFromName(expression.text);
            const std::string base = stripSourceWidthSuffix(expression.text);
            if (!slot || addressOnly(base))
                return;
            const uint8_t width = sourceIdentifierAccessWidth(expression.text);
            if (!unit.workspaceBindings.count(base)) {
                std::pair<uint32_t, uint32_t> &range = ranges[base];
                range.first = slot->byteOffset;
                range.second = std::max(range.second, static_cast<uint32_t>(width));
            }
            if (slot->aggregateField || unit.workspaceBindings.count(base))
                return;
            widths[base].insert(width);
        });
    for (const std::pair<const std::string, uint8_t> &address : narrowest) {
        if (!addressOnly(address.first) || unit.workspaceBindings.count(address.first))
            continue;
        const uint32_t start = taskSlotRefFromName(address.first)->byteOffset;
        uint8_t width = address.second;
        while (start % width)
            width /= 2;
        ranges[address.first] = {start, width};
        widths[address.first].insert(width);
    }
    for (const std::pair<const std::string, std::set<uint8_t>> &item : widths) {
        if (item.second.size() != 1)
            continue;
        const uint8_t width = *item.second.begin();
        const uint32_t start = ranges.at(item.first).first;
        if (start % width)
            continue;
        bool overlap = false;
        for (const std::pair<const std::string, std::pair<uint32_t, uint32_t>> &other : ranges)
            if (other.first != item.first && start < other.second.first + other.second.second && other.second.first < start + width)
                overlap = true;
        if (overlap)
            continue;
        const std::string type = width == 1 ? "int8_t" : width == 2 ? "int16_t" : "int32_t";
        unit.workspaceBindings[item.first] = SourceDataBinding{.type = type};
        unit.workspaceOffsets[item.first] = taskSlotRefFromName(item.first)->byteOffset;
    }

    std::set<uint32_t> starts;
    for (const std::pair<const std::string, std::pair<uint32_t, uint32_t>> &range : ranges)
        starts.insert(range.second.first);
    std::map<std::string, uint8_t> narrowed;
    for (const std::pair<const std::string, std::set<uint8_t>> &item : widths) {
        if (unit.workspaceBindings.count(item.first))
            continue;
        const uint32_t start = ranges.at(item.first).first;
        const std::set<uint32_t>::const_iterator next = starts.upper_bound(start);
        const uint32_t limit = next == starts.end() ? 4U : *next - start;
        uint8_t width = *item.second.rbegin();
        while (width > 1 && (width > limit || start % width))
            width = static_cast<uint8_t>(width / 2);
        narrowed[item.first] = width;
    }
    for (std::map<std::string, uint8_t>::iterator item = narrowed.begin(); item != narrowed.end();) {
        const uint32_t start = ranges.at(item->first).first;
        bool clash = false;
        for (const std::pair<const std::string, std::pair<uint32_t, uint32_t>> &other : ranges) {
            if (other.first == item->first)
                continue;
            const std::map<std::string, uint8_t>::const_iterator shrunk = narrowed.find(other.first);
            const bool bound = unit.workspaceBindings.count(other.first) != 0;
            if (!bound && shrunk == narrowed.end())
                continue;
            const uint32_t size = shrunk != narrowed.end() ? shrunk->second : other.second.second;
            if (start < other.second.first + size && other.second.first < start + item->second)
                clash = true;
        }
        if (clash)
            item = narrowed.erase(item);
        else
            ++item;
    }
    for (const std::pair<const std::string, uint8_t> &item : narrowed) {
        unit.workspaceBindings[item.first] = SourceDataBinding{.type = item.second == 1 ? "int8_t" : item.second == 2 ? "int16_t" : "int32_t"};
        unit.workspaceOffsets[item.first] = taskSlotRefFromName(item.first)->byteOffset;
    }
    if (!narrowed.empty())
        for (const std::string &name : unit.functionOrder)
            visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
                if (expression.kind != SourceExprKind::Identifier)
                    return;
                const std::string base = stripSourceWidthSuffix(expression.text);
                const std::map<std::string, uint8_t>::const_iterator item = narrowed.find(base);

                const uint8_t access = sourceIdentifierAccessWidth(expression.text);
                if (item == narrowed.end() || access == item->second)
                    return;
                const std::string view = access == 1 ? "int8_t *" : access == 2 ? "int16_t *" : "int32_t *";
                expression = SourceExpr{.kind = SourceExprKind::Unary,
                    .text = "*",
                    .args = {SourceExpr{.kind = SourceExprKind::Cast,
                        .text = view,
                        .args = {
                            SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = base}}}}}}};
            });
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Identifier)
                return;
            const std::string base = stripSourceWidthSuffix(expression.text);

            const uint8_t access = sourceIdentifierAccessWidth(expression.text);
            const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.workspaceBindings.find(base);
            if (access < 4 && binding != unit.workspaceBindings.end() && !binding->second.isArray)
                if (const ScenarioDataSchema *record = findScenarioDataSchema(binding->second.type)) {
                    for (const ScenarioDataField &field : record->fields)
                        if (field.offset == 0 && field.elementCount == 1 && scenarioDataTypeSize(field.type) == access) {
                            expression.text = base + "." + field.name;
                            return;
                        }
                }
            if (widths.count(base) && unit.workspaceBindings.count(base))
                expression.text = base;
        });
}

void establishCLocals(SourceFunctionAst &function) {
    std::multimap<uint32_t, SourceLocalDeclaration> locals;
    std::set<std::string> names;
    visitExpressions(function.body.body, [&](SourceExpr &expr) {
        if (expr.kind != SourceExprKind::Identifier)
            return;
        if (expr.text == "eax" || expr.text.starts_with("vm_"))
            throw std::runtime_error("VM register has no C binding in " + function.name);
        const std::string base = expr.text.substr(0, expr.text.find('.'));
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(expr.text);
        const std::map<std::string, uint32_t>::const_iterator recovered = function.localOffsets.find(base);
        if (!slot && recovered == function.localOffsets.end())
            return;
        const uint32_t encoded = recovered != function.localOffsets.end() ? recovered->second : slot->baseOffset;
        const int32_t offset = encoded <= 0xffU ? static_cast<int8_t>(encoded) : static_cast<int16_t>(encoded);
        if (offset >= 0)
            throw std::runtime_error("C frame access does not identify a local object in " + function.name);
        std::string name = expr.text.substr(0, expr.text.find('.'));
        const std::map<std::string, std::string>::const_iterator binding = function.localTypes.find(name);
        const std::string type = binding == function.localTypes.end() ? "int32_t" : binding->second;
        if (slot && slot->aggregateField && binding == function.localTypes.end())
            throw std::runtime_error("C frame member has no established type in " + function.name);
        SourceLocalDeclaration declaration{.type = type, .name = name};
        for (const SourceLocalDeclaration &existing : function.localDeclarations)
            if (existing.name == name)
                declaration = existing;
        if (names.insert(name).second)
            locals.emplace(static_cast<uint32_t>(-offset), declaration);
    });
    uint32_t size = 0;
    function.localDeclarations.clear();
    bool dense = true;
    std::string firstDifference;
    for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals) {
        const std::pair<uint32_t, uint32_t> layout = cElementLayout(local.second.type);
        const uint32_t width = layout.first, alignment = layout.second;
        size = (size + width * local.second.count + alignment - 1U) / alignment * alignment;
        if (size != local.first) {
            dense = false;
            if (firstDifference.empty())
                firstDifference = local.second.type + " " + local.second.name + " requires FP-" + std::to_string(local.first) +
                    "; declarations place it at FP-" + std::to_string(size);
        }
        function.localDeclarations.push_back(local.second);
        function.localTypes[local.second.name] = local.second.type;
    }
    size = (size + 3U) / 4U * 4U;
    const uint32_t reserved = function.explicitFrameAdjust ?
        static_cast<uint32_t>(-(*function.explicitFrameAdjust <= 0xffU ? static_cast<int32_t>(static_cast<int8_t>(*function.explicitFrameAdjust)) :
                                                                         static_cast<int32_t>(static_cast<int16_t>(*function.explicitFrameAdjust)))) :
        0U;

    const std::function<bool()> declareUnusedStorage = [&]() {
        std::vector<SourceLocalDeclaration> declarations;
        uint32_t used = 0;
        const std::function<void(uint32_t)> unused = [&](uint32_t bytes) {
            const uint32_t start = used + bytes;
            const uint32_t encoded = start <= 0x80U ? (0x100U - start) & 0xFFU : (0x10000U - start) & 0xFFFFU;
            const bool words = used % 4U == 0 && bytes % 4U == 0;
            SourceLocalDeclaration declaration{
                .type = words ? "int32_t" : "uint8_t", .name = "unused_" + hexName(encoded), .count = words ? bytes / 4U : bytes};
            declaration.isArray = declaration.count > 1;
            declarations.push_back(declaration);
            used = start;
        };
        for (const std::pair<const uint32_t, SourceLocalDeclaration> &local : locals) {
            const uint32_t width = cElementLayout(local.second.type).first * local.second.count;
            const uint32_t alignment = cElementLayout(local.second.type).second;
            if (local.first < used + width || local.first % alignment)
                return false;
            if (local.first - width > used)
                unused(local.first - width - used);
            used = local.first;
            declarations.push_back(local.second);
        }
        if (used > reserved)
            return false;
        if (reserved > used)
            unused(reserved - used);
        function.localDeclarations = declarations;
        for (const SourceLocalDeclaration &declaration : declarations)
            function.localTypes[declaration.name] = declaration.type;
        return true;
    };
    if ((!dense || size != reserved) && !recoverCLexicalStorage(function, locals, reserved) && !declareUnusedStorage())
        throw std::runtime_error("C local storage is unresolved in " + function.name + ": native reserves " + std::to_string(reserved) +
            " bytes; established declarations occupy " + std::to_string(size) + (firstDifference.empty() ? "" : "; " + firstDifference));
    function.explicitFrameAdjust.reset();
    function.explicitBase.reset();
    function.declaredOffset.reset();
    function.semantics = SourceSemantics::C11;
    function.returnType = "void";
    for (const std::string &parameter : function.params)
        function.localTypes.emplace(parameter, "int32_t");
}

} // namespace shenmue::scenario::decompile
