#include "scn3/common/util.hpp"
#include "scn3/decompiler/data.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::scenario::decompile {

bool recoverCWorkspacePointers(SourceUnit &unit) {
    std::map<std::string, std::set<std::string>> stored;
    std::set<std::string> integers;
    const auto word = [&](const SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Identifier || expression.text.find('.') != std::string::npos)
            return false;
        const std::optional<SourceSlotRef> slot = taskSlotRefFromName(expression.text);
        const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.workspaceBindings.find(expression.text);
        return slot && !slot->aggregateField && (binding == unit.workspaceBindings.end() || (!binding->second.isArray && binding->second.type == "int32_t"));
    };
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        const auto pointerType = [&](const SourceExpr &value) -> std::string {
            if (value.kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.text);
                if (local != function.localTypes.end())
                    return local->second.find('*') != std::string::npos ? local->second : std::string();
                const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(value.text);
                if (data != unit.dataBindings.end() && data->second.isArray && !data->second.pointer) {
                    const std::string element = data->second.type == "cstr" || data->second.type == "char" ? "char" : data->second.type;
                    return (scenarioDataTypeByName(element) || findScenarioDataSchema(element) || element == "char") ? element + " *" : std::string();
                }
            }
            if (value.kind == SourceExprKind::Unary && value.text == "&" && value.args.size() == 1 && value.args[0].kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.args[0].text);
                if (local != function.localTypes.end() && findScenarioDataSchema(local->second))
                    return local->second + " *";
                const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(value.args[0].text);
                if (data != unit.dataBindings.end() && !data->second.pointer && findScenarioDataSchema(data->second.type))
                    return data->second.type + " *";
            }
            return {};
        };
        const std::function<void(const std::vector<SourceStmt> &)> stores = [&](const std::vector<SourceStmt> &statements) {
            for (const SourceStmt &statement : statements) {
                if (statement.kind == SourceStmtKind::Assign && word(statement.expr)) {
                    const std::string type = pointerType(statement.value);
                    const std::optional<uint32_t> constant = sourceDataConstantValue(statement.value, true);
                    if (!type.empty())
                        stored[statement.expr.text].insert(type.starts_with("const ") ? type.substr(6) : type);
                    else if (!constant || *constant)
                        integers.insert(statement.expr.text);
                }
                stores(statement.body);
                stores(statement.elseBody);
                for (const SourceSwitchCase &clause : statement.cases)
                    stores(clause.body);
            }
        };
        stores(function.body.body);
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if ((expression.kind == SourceExprKind::Binary && expression.text != "==" && expression.text != "!=") || expression.kind == SourceExprKind::Unary)
                for (const SourceExpr &operand : expression.args)
                    if (word(operand))
                        integers.insert(operand.text);
        });
    }
    bool changed = false;
    for (const std::pair<const std::string, std::set<std::string>> &object : stored) {
        if (object.second.size() != 1 || integers.count(object.first))
            continue;
        unit.workspaceBindings[object.first] = SourceDataBinding{.type = "uint32_t", .pointer = true, .pointerType = *object.second.begin()};
        unit.workspaceOffsets[object.first] = taskSlotRefFromName(object.first)->baseOffset;
        changed = true;
    }
    return changed;
}

bool recoverCPassedWorkspaceRecords(SourceUnit &unit) {
    std::map<std::string, std::set<std::string>> records;
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            const std::map<std::string, SourceFunctionAst>::const_iterator callee =
                expression.kind == SourceExprKind::Call ? unit.functions.find(expression.text) : unit.functions.end();
            if (callee == unit.functions.end())
                return;
            for (size_t index = 0; index < expression.args.size() && index < callee->second.params.size(); ++index) {
                const SourceExpr &argument = expression.args[index];
                if (argument.kind != SourceExprKind::Unary || argument.text != "&" || argument.args.size() != 1 ||
                    argument.args[0].kind != SourceExprKind::Identifier)
                    continue;
                const std::optional<SourceSlotRef> slot = taskSlotRefFromName(argument.args[0].text);
                const std::map<std::string, std::string>::const_iterator parameter = callee->second.localTypes.find(callee->second.params[index]);
                if (!slot || slot->aggregateField || parameter == callee->second.localTypes.end() || parameter->second.find('*') == std::string::npos)
                    continue;
                std::string pointee = trimCopy(parameter->second.substr(0, parameter->second.rfind('*')));
                if (pointee.starts_with("const "))
                    pointee.erase(0, 6);
                if (findScenarioDataSchema(pointee))
                    records[argument.args[0].text].insert(pointee);
            }
        });
    bool changed = false;
    for (const std::pair<const std::string, std::set<std::string>> &record : records) {
        const std::map<std::string, SourceDataBinding>::const_iterator previous = unit.workspaceBindings.find(record.first);
        const bool word =
            previous == unit.workspaceBindings.end() || (!previous->second.isArray && (previous->second.type == "int32_t" || previous->second.type == "float"));
        if (record.second.size() != 1 || !word)
            continue;
        unit.workspaceBindings[record.first] = SourceDataBinding{.type = *record.second.begin()};
        unit.workspaceOffsets[record.first] = taskSlotRefFromName(record.first)->baseOffset;
        changed = true;
    }
    return changed;
}

void recoverCWorkspaceRecords(SourceUnit &unit) {
    std::set<std::string> conflicts;
    for (const std::string &name : unit.functionOrder) {
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            if (!command || !command->handlerName)
                return;
            std::vector<std::optional<uint32_t>> constants;
            for (const SourceExpr &argument : expression.args)
                constants.push_back(sourceDataConstantValue(argument, true));
            for (size_t index = 0; index < expression.args.size(); ++index) {
                const NativeArgumentType *argumentType = nativeArgumentForHandler(command->handlerName, index, constants);
                if (!argumentType || !argumentType->pointer)
                    continue;
                const ScenarioDataSchema *schema = findScenarioDataSchema(argumentType->type);
                const SourceExpr &argument = expression.args[index];
                if (!schema || argument.kind != SourceExprKind::Unary || argument.text != "&" || argument.args.size() != 1)
                    continue;
                const SourceExpr &base = argument.args[0];
                const std::optional<SourceSlotRef> slot = taskSlotRefFromName(base.text);
                if (base.kind != SourceExprKind::Identifier || !slot || slot->aggregateField)
                    continue;
                const std::map<std::string, SourceDataBinding>::const_iterator previous = unit.workspaceBindings.find(base.text);
                if (previous != unit.workspaceBindings.end() && previous->second.type != schema->name)
                    conflicts.insert(base.text);
                unit.workspaceBindings[base.text] = SourceDataBinding{.type = schema->name};
                unit.workspaceOffsets[base.text] = slot->byteOffset;
            }
        });
    }

    for (const std::string &name : conflicts) {
        unit.workspaceBindings.erase(name);
        unit.workspaceOffsets.erase(name);
    }
}

static constexpr NativePointerParameter subtitleText[] = {{.byteOffset = 0, .type = "char", .readOnly = true}};

std::map<std::string, bool> recoverCNativeData(SourceUnit &unit, uint32_t dataEnd) {
    std::map<std::string, bool> readOnly;
    std::map<std::string, std::string> observedTypes;
    std::set<std::string> referenced;
    const std::function<void(SourceExpr &)> reference = [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Identifier && unit.dataSymbols.count(expression.text))
            referenced.insert(expression.text);
    };
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, reference);
    for (SourceDataRelocation &relocation : unit.dataRelocations)
        visitExpressions(relocation.target, reference);

    std::set<uint32_t> pointerBoundaries;
    for (size_t offset = 0; offset + 8U <= unit.trailingData.size(); offset += 8U) {
        const uint32_t kind = readU32(unit.trailingData, static_cast<uint32_t>(offset)), site = readU32(unit.trailingData, static_cast<uint32_t>(offset + 4U));
        if (!kind && !site)
            break;
        pointerBoundaries.insert(site);
        pointerBoundaries.insert(site + 4U);
    }
    for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols)
        if (pointerBoundaries.count(symbol.second))
            referenced.insert(symbol.first);

    std::map<std::string, std::set<std::string>> pointees;
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            std::vector<std::optional<uint32_t>> constants;
            for (const SourceExpr &argument : expression.args)
                constants.push_back(sourceDataConstantValue(argument, true));
            const NativeArgumentLayout *layout = command && command->handlerName ? nativeArgumentLayout(command->handlerName, constants) : nullptr;
            if (!layout && expression.text != "SCNT_ShowSubtitle")
                return;
            for (const NativePointerParameter &pointer : layout ? layout->pointers : std::span<const NativePointerParameter>(subtitleText))
                if (pointer.byteOffset / 4U < expression.args.size() && std::string_view(pointer.type) != "void" &&
                    expression.args[pointer.byteOffset / 4U].kind == SourceExprKind::Identifier)
                    pointees[expression.args[pointer.byteOffset / 4U].text].insert(pointer.type);
        });
    std::set<std::string> absorbed;
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            std::vector<std::optional<uint32_t>> constants;
            for (const SourceExpr &argument : expression.args)
                constants.push_back(sourceDataConstantValue(argument, true));
            const NativeArgumentLayout *layout = command && command->handlerName ? nativeArgumentLayout(command->handlerName, constants) : nullptr;
            if (!layout && expression.text != "SCNT_ShowSubtitle")
                return;
            for (const NativePointerParameter &pointer : layout ? layout->pointers : std::span<const NativePointerParameter>(subtitleText)) {
                if (pointer.byteOffset / 4U >= expression.args.size())
                    continue;
                SourceExpr &argument = expression.args[pointer.byteOffset / 4U];
                if (argument.kind != SourceExprKind::Identifier)
                    continue;
                const std::map<std::string, uint32_t>::const_iterator symbol = unit.dataSymbols.find(argument.text);
                if (symbol == unit.dataSymbols.end() || symbol->second < static_cast<uint32_t>(unit.header.dataSegmentOffset) || symbol->second >= dataEnd)
                    continue;
                if (std::string_view(pointer.type) == "void" || pointees[argument.text].size() > 1) {
                    readOnly[argument.text] = (!readOnly.count(argument.text) || readOnly[argument.text]) && pointer.readOnly;
                    continue;
                }
                SourceDataBinding &binding = unit.dataBindings.at(argument.text);
                const std::string type = pointer.type;
                const ScenarioDataSchema *schema = findScenarioDataSchema(type);
                const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(type);
                const uint32_t stride = schema ? schema->size : type == "char" ? 1U : dataType ? scenarioDataTypeSize(*dataType) : 0U;
                const uint32_t alignment = schema ? schema->alignment : stride;
                if (!stride || binding.pointer)
                    throw std::runtime_error("Unknown C data type for " + argument.text);
                const std::map<std::string, std::string>::const_iterator previous = observedTypes.find(argument.text);
                if (previous != observedTypes.end() && previous->second != type)
                    throw std::runtime_error("Native C data has incompatible observed pointee types: " + argument.text);
                observedTypes[argument.text] = type;
                if (!readOnly.count(argument.text))
                    readOnly[argument.text] = pointer.readOnly;
                else
                    readOnly[argument.text] = readOnly[argument.text] && pointer.readOnly;
                uint32_t end = dataEnd;
                for (const std::pair<const std::string, uint32_t> &next : unit.dataSymbols)
                    if (referenced.count(next.first) && next.second > symbol->second && next.second < end)
                        end = next.second;
                if (schema) {
                    const uint32_t count = binding.type == type && binding.isArray ? binding.elementCount : 1U;
                    const uint64_t recordEnd = static_cast<uint64_t>(symbol->second) + static_cast<uint64_t>(count) * stride;
                    if (recordEnd > end)
                        throw std::runtime_error("Native C record overlaps another referenced object: " + argument.text);
                    end = static_cast<uint32_t>(recordEnd);
                    if (end < dataEnd) {
                        bool boundary = false;
                        for (const std::pair<const std::string, uint32_t> &next : unit.dataSymbols)
                            if (next.second == end)
                                boundary = true;
                        if (!boundary) {
                            uint32_t nextBoundary = dataEnd;
                            for (const std::pair<const std::string, uint32_t> &next : unit.dataSymbols)
                                if (next.second > end && next.second < nextBoundary)
                                    nextBoundary = next.second;
                            const std::string remainder = "data_" + hexName(end, 6);
                            if (unit.dataSymbols.count(remainder))
                                throw std::runtime_error("Conflicting C data fragment name: " + remainder);
                            unit.dataSymbols.emplace(remainder, end);
                            unit.dataBindings.emplace(remainder, SourceDataBinding{.type = "bytes", .elementCount = nextBoundary - end, .isArray = true});
                        }
                    }
                }
                if (symbol->second % alignment || (end - symbol->second) % stride)
                    throw std::runtime_error("Native C data extent does not contain complete typed objects: " + argument.text);
                for (const std::pair<const std::string, uint32_t> &fragment : unit.dataSymbols)
                    if (fragment.second > symbol->second && fragment.second < end)
                        absorbed.insert(fragment.first);
                binding.type = type;
                binding.elementCount = (end - symbol->second) / stride;
                binding.isArray = binding.elementCount > 1 || type == "char";
                argument.cType = (pointer.readOnly ? "const " : "") + type + " *";
            }
        });
    }
    for (const std::string &fragment : absorbed) {
        unit.dataSymbols.erase(fragment);
        unit.dataBindings.erase(fragment);
    }
    return readOnly;
}

void recoverCDataTables(SourceUnit &unit, const std::vector<uint8_t> &blob, uint32_t dataEnd, std::map<std::string, bool> &readOnly) {
    const uint32_t dataStart = static_cast<uint32_t>(unit.header.dataSegmentOffset);
    std::vector<std::pair<uint32_t, std::string>> symbols;
    for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols)
        if (symbol.second >= dataStart && symbol.second < dataEnd)
            symbols.emplace_back(symbol.second, symbol.first);
    for (size_t index = 1; index < symbols.size(); ++index)
        for (size_t position = index; position && symbols[position] < symbols[position - 1]; --position)
            std::swap(symbols[position], symbols[position - 1]);
    std::set<uint32_t> pointerWords;
    for (size_t offset = 0; offset + 8U <= unit.trailingData.size(); offset += 8U) {
        const uint32_t kind = readU32(unit.trailingData, static_cast<uint32_t>(offset)), site = readU32(unit.trailingData, static_cast<uint32_t>(offset + 4U));
        if (!kind && !site)
            break;
        pointerWords.insert(site);
    }
    std::map<std::string, uint32_t> uses, views;
    std::set<std::string> written, named;
    for (const SourceDataRelocation &relocation : unit.dataRelocations) {
        std::vector<SourceStmt> holder{SourceStmt{.expr = relocation.target}};
        visitExpressions(holder, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier)
                named.insert(expression.text);
        });
    }

    const auto byteView = [&](const SourceExpr &expression) -> const SourceExpr * {
        if (expression.kind != SourceExprKind::Unary || expression.text != "*" || expression.args.size() != 1)
            return nullptr;
        const SourceExpr &address = expression.args[0];
        const bool offset = address.kind == SourceExprKind::Binary && address.text == "+" && address.args.size() == 2;
        const SourceExpr &view = offset ? address.args[0] : address;
        if (view.kind != SourceExprKind::Cast || view.text != "int8_t *" || view.args.size() != 1 || view.args[0].kind != SourceExprKind::Unary ||
            view.args[0].text != "&" || view.args[0].args.size() != 1 || view.args[0].args[0].kind != SourceExprKind::Identifier)
            return nullptr;
        return &view.args[0].args[0];
    };
    const std::function<void(std::vector<SourceStmt> &)> count = [&](std::vector<SourceStmt> &statements) {
        for (SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Assign)
                if (const SourceExpr *object = byteView(statement.expr))
                    written.insert(object->text);
            count(statement.initializer);
            count(statement.body);
            count(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                count(clause.body);
        }
    };
    for (const std::string &name : unit.functionOrder) {
        std::vector<SourceStmt> &body = unit.functions.at(name).body.body;
        visitExpressions(body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier) {
                ++uses[expression.text];
                named.insert(expression.text);
            }
            if (const SourceExpr *object = byteView(expression))
                ++views[object->text];
        });
        count(body);
    }

    const auto plain = [&](size_t index) {
        const uint32_t start = symbols[index].first;
        const uint32_t end = index + 1 < symbols.size() ? symbols[index + 1].first : dataEnd;
        if (!unit.dataBindings.count(symbols[index].second))
            return false;
        const SourceDataBinding &binding = unit.dataBindings.at(symbols[index].second);
        if (binding.pointer || (binding.type != "u32" && binding.type != "bytes" && binding.type != "pad"))
            return false;
        for (uint32_t word = start & ~3U; word < end; word += 4U)
            if (pointerWords.count(word))
                return false;
        return true;
    };
    const auto absorb = [&](size_t first, size_t last, const std::string &type, uint32_t elements) {
        for (size_t index = first + 1; index < last; ++index) {
            unit.dataBindings.erase(symbols[index].second);
            unit.dataSymbols.erase(symbols[index].second);
        }
        unit.dataBindings[symbols[first].second] = SourceDataBinding{.type = type, .elementCount = elements, .isArray = elements > 1 || type == "int8_t"};
    };
    std::set<std::string> tables;
    for (size_t index = 0; index < symbols.size(); ++index) {
        const std::string &name = symbols[index].second;
        if (!views.count(name) || views.at(name) != uses[name] || !plain(index))
            continue;
        size_t last = index + 1;
        while (last < symbols.size() && !named.count(symbols[last].second) && plain(last))
            ++last;
        const uint32_t end = last < symbols.size() ? symbols[last].first : dataEnd;
        absorb(index, last, "int8_t", end - symbols[index].first);
        tables.insert(name);
        if (written.count(name))
            readOnly[name] = false;
    }
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            const SourceExpr *object = byteView(expression);
            if (!object || !tables.count(object->text))
                return;
            SourceExpr base = *object;
            const SourceExpr &address = expression.args[0];
            SourceExpr position = address.kind == SourceExprKind::Binary ? address.args[1] : SourceExpr{.kind = SourceExprKind::IntLiteral, .text = "0"};
            expression = SourceExpr{.kind = SourceExprKind::Index, .text = "[]", .args = {std::move(base), std::move(position)}, .cType = "int8_t"};
        });

    const auto kind = [&](uint32_t value) {
        if (!value)
            return 0;
        bool name = true, letter = false;
        for (uint32_t byte = 0; byte < 4; ++byte) {
            const char character = static_cast<char>(value >> (byte * 8U));
            letter |= character >= 'A' && character <= 'Z';
            name &= (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_';
        }
        if (name && letter)
            return 3;
        if (value <= 0xFFFFFFU || value >= 0xFF000000U)
            return 1;
        float number = 0;
        std::memcpy(&number, &value, sizeof(number));
        const std::string literal = formatFloatLiteral(value);
        size_t digits = 0;
        bool leading = true;
        for (char character : literal) {
            if (character == 'e' || character == 'E')
                break;
            if (!std::isdigit(static_cast<unsigned char>(character)))
                continue;
            if (leading && character == '0')
                continue;
            leading = false;
            ++digits;
        }
        const float magnitude = number < 0 ? -number : number;
        return std::isfinite(number) && magnitude >= 1e-3f && magnitude <= 1e7f && digits <= 7 && floatLiteralTextRoundTrips(literal, value) ? 2 : 4;
    };
    static constexpr const char *types[] = {"int32_t", "int32_t", "float", "shen_name", "uint32_t"};
    for (size_t index = 0; index < symbols.size();) {
        if (named.count(symbols[index].second) || !plain(index) || symbols[index].first % 4U || tables.count(symbols[index].second)) {
            ++index;
            continue;
        }
        size_t last = index;
        while (last < symbols.size() && !named.count(symbols[last].second) && plain(last) && !tables.count(symbols[last].second))
            ++last;
        const uint32_t start = symbols[index].first;
        uint32_t end = last < symbols.size() ? symbols[last].first : dataEnd;
        end = start + (end - start) / 4U * 4U;
        if (end - start < 4U || (last < symbols.size() && end != symbols[last].first)) {
            index = last + (last == index);
            continue;
        }

        const uint32_t words = (end - start) / 4U;
        std::string columns;
        for (uint32_t period = 2; columns.empty() && period <= 8; ++period) {
            if (words / period < 2)
                continue;
            std::vector<int> column(period, 0);
            bool uniform = true;
            for (uint32_t word = 0; uniform && word < words / period * period; ++word) {
                const int current = kind(readU32(blob, start + word * 4U));
                if (current && column[word % period] && column[word % period] != current)
                    uniform = false;
                if (current)
                    column[word % period] = current;
            }
            std::set<int> kinds;
            for (int current : column)
                kinds.insert(current ? current : 1);
            if (!uniform || kinds.size() < 2)
                continue;
            for (int current : column)
                columns += static_cast<char>('0' + (current ? current : 1));
        }
        const uint32_t rows = columns.empty() ? 0U : words / static_cast<uint32_t>(columns.size());
        const uint32_t tail = start + rows * static_cast<uint32_t>(columns.size()) * 4U;
        std::vector<std::pair<uint32_t, int>> segments;
        for (uint32_t word = tail; word < end; word += 4U) {
            const int current = kind(readU32(blob, word));
            if (segments.empty())
                segments.emplace_back(word, current);
            else if (current && segments.back().second && current != segments.back().second)
                segments.emplace_back(word, current);
            else if (current && !segments.back().second)
                segments.back().second = current;
        }
        if (!rows && segments.size() == 1 && !segments.front().second) {
            index = last;
            continue;
        }
        for (size_t first = index; first < last; ++first) {
            unit.dataBindings.erase(symbols[first].second);
            unit.dataSymbols.erase(symbols[first].second);
        }
        if (rows) {
            unit.dataSymbols[sourceDataName(start)] = start;
            unit.dataBindings[sourceDataName(start)] = SourceDataBinding{.type = "row:" + columns, .elementCount = rows, .isArray = true};
        }
        for (size_t segment = 0; segment < segments.size(); ++segment) {
            const uint32_t from = segments[segment].first, to = segment + 1 < segments.size() ? segments[segment + 1].first : end;
            const uint32_t elements = (to - from) / 4U;
            unit.dataSymbols[sourceDataName(from)] = from;
            unit.dataBindings[sourceDataName(from)] =
                SourceDataBinding{.type = types[segments[segment].second], .elementCount = elements, .isArray = elements > 1};
        }
        if (last == symbols.size() && end < dataEnd) {
            unit.dataSymbols[sourceDataName(end)] = end;
            unit.dataBindings[sourceDataName(end)] = SourceDataBinding{.type = "bytes", .elementCount = dataEnd - end, .isArray = true};
        }
        index = last;
    }
}

} // namespace shenmue::scenario::decompile
