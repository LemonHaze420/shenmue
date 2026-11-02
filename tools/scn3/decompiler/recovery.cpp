#include "scn3/backend/compiler.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/export.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/constants.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/lookup.hpp"
#include <json.hpp>

namespace shenmue::scenario {
namespace {
using namespace decompile;
using Json = nlohmann::json;

struct CRecovery {
    SourceUnit unit;
    SourceUnit original;
    Json moduleIssues = Json::array();
    std::map<std::string, Json> issues;
    std::set<std::string> layouts;
    std::map<std::string, bool> readOnlyData;
    std::set<std::string> plain;
};

Json issue(unsigned category, const std::string &code, const std::string &detail) {
    return {{"category", category}, {"code", code}, {"detail", detail}};
}

CRecovery recoverC(const Program &program, const std::string &profile, const std::set<std::string> &plain) {
    if (profile != "s1-pc")
        throw std::runtime_error("C export requires the S1 command profile");
    CRecovery recovery;
    recovery.plain = plain;
    SourceUnit &unit = recovery.unit;
    Scn3Module module = buildScn3Module(program);
    attachUnreferencedReturns(module);
    renderExplicitSource(program, profile, plain, &unit);
    for (const LowLevelFunction &native : module.functions) {
        const std::map<std::string, SourceFunctionAst>::iterator view = unit.functions.find(scriptFunctionName(native.start));
        if (view == unit.functions.end())
            throw std::runtime_error("Missing source view for native function " + hexUnsigned(native.start));
        view->second.offset = native.start;
        for (const LowLevelInsn &instruction : native.instructions)
            unit.codeLabels["L" + hexName(instruction.offset, 6)] = instruction.offset;
    }
    for (const std::string &name : unit.functionOrder) {
        const SourceFunctionAst &function = unit.functions.at(name);
        const std::vector<uint8_t> encoded = compileExplicitFunction(unit, function);
        if (function.offset + encoded.size() > module.originalBlob.size() ||
            !std::equal(encoded.begin(), encoded.end(), module.originalBlob.begin() + function.offset))
            throw std::runtime_error("Function bytecode does not match its original offset: " + name);
    }
    recovery.original = unit;
    unit.ordinaryC = true;

    std::set<std::string> functionPointers;
    for (uint32_t offset = module.parsedHeader.relocationTableOffset; offset + 8U <= module.originalBlob.size(); offset += 8U) {
        const uint32_t kind = readU32(module.originalBlob, offset), site = readU32(module.originalBlob, offset + 4U);
        if (!kind && !site)
            break;
        if (kind != 0x2402U)
            continue;
        for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols)
            if (symbol.second == site)
                functionPointers.insert(symbol.first);
    }

    for (const std::string &name : unit.functionOrder)
        recoverCNarrowParameters(unit.functions.at(name));
    try {
        recoverCCallSignatures(unit);
    } catch (const std::exception &error) {
        recovery.moduleIssues.push_back(issue(1, "call_signature", error.what()));
    }
    for (const std::string &name : unit.functionOrder) {
        recoverCScopedRecords(unit.functions.at(name));
    }

    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call || expression.text != "vm_scnf_offset" || expression.args.size() != 1)
                return;
            const SourceExpr &encoded = expression.args[0];
            if (encoded.kind != SourceExprKind::Cast || encoded.args.size() != 1)
                return;
            const SourceExpr &text = encoded.args[0];
            const SourceExpr object = text.kind == SourceExprKind::Unary && text.text == "&" && text.args.size() == 1 ? text.args[0] : text;
            if (object.kind != SourceExprKind::Identifier || !unit.dataSymbols.count(object.text))
                return;
            expression = SourceExpr{.kind = SourceExprKind::Call, .text = "SCNT_ShowSubtitle", .args = {object}};
        });
    try {
        recovery.readOnlyData = recoverCNativeData(unit, moduleDataEnd(module));
    } catch (const std::exception &error) {
        recovery.moduleIssues.push_back(issue(1, "data_ownership", error.what()));
    }
    try {
        recoverCWorkspaceRecords(unit);
    } catch (const std::exception &error) {
        recovery.moduleIssues.push_back(issue(1, "workspace_types", error.what()));
    }
    recoverCWorkspaceValues(unit);
    for (bool typed = true; typed;) {
        recoverCForwardedPointerParameters(unit);
        typed = recoverCPassedWorkspaceRecords(unit);
        typed |= recoverCWorkspacePointers(unit);
        for (const std::string &name : unit.functionOrder)
            typed |= recoverCCopiedRecords(unit.functions.at(name), unit);
    }
    for (const std::pair<const std::string, SourceDataBinding> &binding : unit.workspaceBindings)
        if (const std::optional<SourceSlotRef> slot = taskSlotRefFromName(binding.first))
            unit.workspaceOffsets[binding.first] = slot->baseOffset;
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        recoverCRecordAccesses(function, unit);
        recoverCRecordMembers(function, false);
        recoverCValueReturns(function);
        recoverCRegisterExpressions(function.body.body);
    }

    for (bool pointers = true; pointers;) {
        recoverCFloatStorage(unit);
        std::map<std::string, std::map<std::string, std::string>> before;
        for (const std::string &name : unit.functionOrder)
            before[name] = unit.functions.at(name).localTypes;
        recoverCForwardedPointerParameters(unit);
        pointers = false;
        for (const std::string &name : unit.functionOrder)
            if (before.at(name) != unit.functions.at(name).localTypes) {
                recoverCRecordAccesses(unit.functions.at(name), unit);
                pointers = true;
            }
    }
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        Json &issues = recovery.issues[name] = Json::array();
        recoverCRecordMembers(function, true);
        recoverCLocalArrays(function);
        try {
            recoverCFloatExpressions(function, unit);
        } catch (const std::exception &error) {
            issues.push_back(issue(1, "float_type", error.what()));
        }
        recoverCByteStorage(function);
        recoverCBooleanValues(function);
        recoverCWordArithmetic(function, unit);
        recoverCMemberAddresses(function);
        const uint32_t encoded = function.explicitFrameAdjust.value_or(0);
        const int32_t reserved = -(encoded <= 255U ? static_cast<int32_t>(static_cast<int8_t>(encoded)) : static_cast<int32_t>(static_cast<int16_t>(encoded)));
        try {
            establishCLocals(function);
        } catch (const std::exception &error) {
            if (!std::string(error.what()).starts_with("C local storage is unresolved")) {
                issues.push_back(issue(1, "local_ownership", error.what()));
                continue;
            }
            recovery.layouts.insert(name);
            function.requestedFrameSize = static_cast<uint32_t>(reserved);
            for (const SourceLocalDeclaration &declaration : function.localDeclarations) {
                const std::optional<SourceSlotRef> slot = localSlotRefFromName(declaration.name);
                const std::map<std::string, uint32_t>::const_iterator offset = function.localOffsets.find(declaration.name);
                if (offset == function.localOffsets.end() && !slot)
                    throw std::runtime_error("Missing local binding: " + declaration.name);
                const uint32_t value = offset != function.localOffsets.end() ? offset->second : slot->baseOffset;
                function.requestedLocalOffsets[declaration.name] = value <= 255U ? static_cast<int8_t>(value) : static_cast<int16_t>(value);
            }
            function.explicitFrameAdjust.reset();
            function.explicitBase.reset();
            function.declaredOffset.reset();
            function.semantics = SourceSemantics::C11;
        }
        for (const std::string &parameter : function.params)
            function.localTypes.emplace(parameter, "int32_t");
        recoverCUnaryOperators(function, unit);
        recoverCVariadicArguments(function);
        recoverCStatementSyntax(function.body.body);
        discardUnusedCLabels(function);
    }

    if (plain.empty() && !recovery.layouts.empty())
        return recovery;
    const std::set<std::string> read = discardUnreadCReturns(unit);
    for (const std::string &name : unit.functionOrder)
        unit.functions.at(name).returnType.clear();
    for (bool changed = true; changed;) {
        changed = false;
        for (const std::string &name : unit.functionOrder) {
            SourceFunctionAst &function = unit.functions.at(name);
            std::string type = "void";
            bool unknown = false;
            try {
                cReturnType(function.body.body, type, unit, function, unknown);
            } catch (const std::exception &error) {
                recovery.issues[name].push_back(issue(1, "return_type", error.what()));
            }
            if (unknown && type == "void")
                type.clear();

            if (type == "void" && read.count(name))
                type = "int32_t";
            if (type != function.returnType) {
                function.returnType = type;
                changed = true;
            }
        }
    }

    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        if (function.returnType != "int32_t")
            continue;
        std::string narrow;
        bool valid = true, falls = function.body.body.empty() || function.body.body.back().kind != SourceStmtKind::Return;
        const std::function<void(const std::vector<SourceStmt> &)> returns = [&](const std::vector<SourceStmt> &list) {
            for (const SourceStmt &statement : list) {
                if (statement.kind == SourceStmtKind::Return) {
                    const SourceExpr &value = statement.expr;
                    const std::map<std::string, std::string>::const_iterator local =
                        value.kind == SourceExprKind::Identifier ? function.localTypes.find(value.text) : function.localTypes.end();
                    const std::optional<uint32_t> constant = sourceDataConstantValue(value, true);
                    if (local != function.localTypes.end() && (local->second == "int8_t" || local->second == "int16_t")) {
                        valid &= narrow.empty() || narrow == local->second;
                        narrow = local->second;
                    } else if (!constant || static_cast<int32_t>(*constant) < -128 || static_cast<int32_t>(*constant) > 127)
                        valid = false;
                }
                returns(statement.body);
                returns(statement.elseBody);
                for (const SourceSwitchCase &clause : statement.cases)
                    returns(clause.body);
            }
        };
        returns(function.body.body);
        if (valid && !falls && !narrow.empty())
            function.returnType = narrow;
    }
    for (const std::string &name : read) {
        SourceFunctionAst &function = unit.functions.at(name);
        std::string type = "void";
        bool unknown = false;
        try {
            cReturnType(function.body.body, type, unit, function, unknown);
        } catch (const std::exception &) {
            continue;
        }
        if (type != "void" || unknown)
            continue;

        std::vector<SourceStmt> &body = function.body.body;
        const auto bare = [](const SourceStmt &statement) {
            return statement.kind == SourceStmtKind::Return && statement.expr.text.empty() && statement.expr.args.empty();
        };
        const bool trailing = !body.empty() && bare(body.back());
        if (trailing)
            body.pop_back();
        bool jumps = false;
        const std::function<void(std::vector<SourceStmt> &)> returns = [&](std::vector<SourceStmt> &list) {
            for (SourceStmt &statement : list) {
                if (bare(statement)) {
                    statement = SourceStmt{.kind = SourceStmtKind::Goto, .label = "end"};
                    jumps = true;
                }
                returns(statement.body);
                returns(statement.elseBody);
                for (SourceSwitchCase &clause : statement.cases)
                    returns(clause.body);
            }
        };
        returns(body);
        if (jumps || (trailing && (body.empty() || body.back().kind != SourceStmtKind::Label)))
            body.push_back(SourceStmt{.kind = SourceStmtKind::Label, .label = "end"});
    }
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Call && expression.text == "TASK_FN" && expression.args.size() == 1)
                expression = expression.args[0];
            const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(expression.text);
            if (expression.kind == SourceExprKind::Identifier && data != unit.dataBindings.end() && !data->second.isArray)
                expression = SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {expression}, .cType = data->second.type + " *"};
        });
    for (const std::string &name : unit.functionOrder)
        recoverCAddressWords(unit.functions.at(name), unit);

    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        const std::function<uint32_t(const SourceExpr &)> elementSize = [&](const SourceExpr &address) -> uint32_t {
            std::string type;
            if (address.kind == SourceExprKind::Unary && address.text == "&" && address.args.size() == 1 &&
                address.args[0].kind == SourceExprKind::Identifier) {
                const std::string &object = address.args[0].text;
                if (function.localTypes.count(object))
                    type = function.localTypes.at(object);
                else if (unit.workspaceBindings.count(object))
                    type = unit.workspaceBindings.at(object).type;
                else if (unit.dataBindings.count(object))
                    type = unit.dataBindings.at(object).type;
                else if (localSlotRefFromName(object) || taskSlotRefFromName(object))
                    type = "int32_t";
            } else if (address.kind == SourceExprKind::Identifier && unit.dataBindings.count(address.text) && unit.dataBindings.at(address.text).isArray)
                type = unit.dataBindings.at(address.text).type;
            if (type == "cstr" || type == "bytes" || type == "pad")
                type = "char";
            if (type == "u32")
                type = "uint32_t";
            const ScenarioDataSchema *schema = findScenarioDataSchema(type);
            const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(type);
            return schema ? schema->size : type == "char" ? 1U : dataType ? scenarioDataTypeSize(*dataType) : 0U;
        };

        std::set<const SourceExpr *> narrow;
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Call && expression.args.size() == 1 &&
                (expression.text == "vm_load8" || expression.text == "vm_mem8" || expression.text == "vm_load16" || expression.text == "vm_mem16"))
                narrow.insert(&expression.args[0]);
        });
        std::set<const SourceExpr *> unscaled;
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Binary || (expression.text != "+" && expression.text != "-") || expression.args.size() != 2)
                return;
            if (narrow.count(&expression))
                return;
            const uint32_t size = elementSize(expression.args[0]);
            if (size < 2)
                return;
            unscaled.insert(&expression);
            const std::function<const SourceExpr &(const SourceExpr &)> integral = [&](const SourceExpr &value) -> const SourceExpr & {
                if (value.kind == SourceExprKind::Cast && value.args.size() == 1 && (value.text == "int32_t" || value.text == "uint32_t"))
                    return integral(value.args[0]);
                return value;
            };
            SourceExpr &offset = expression.args[1];
            const SourceExpr &bytes = integral(offset);
            if (const std::optional<uint32_t> constant = sourceDataConstantValue(bytes, true)) {
                if (*constant % size == 0) {
                    offset = number(*constant / size);
                    unscaled.erase(&expression);
                }
                return;
            }
            if (bytes.kind != SourceExprKind::Binary || bytes.args.size() != 2)
                return;
            const std::optional<uint32_t> factor = sourceDataConstantValue(bytes.args[1], true);
            if (!factor)
                return;
            const bool shifted = bytes.text == "<<" && ((size == 4 && *factor == 2) || (size == 2 && *factor == 1));
            const bool multiplied = bytes.text == "*" && size != 4 && size != 2 && *factor == size;
            if (shifted || multiplied) {
                const SourceExpr index = integral(bytes.args[0]);
                offset = index;
                unscaled.erase(&expression);
            }
        });

        const auto byteOffset = [&](const SourceExpr &expression) -> std::string {
            if (!unscaled.count(&expression))
                return {};
            const SourceExpr &base = expression.args[0];
            const std::string object = base.kind == SourceExprKind::Unary && base.text == "&" && base.args.size() == 1 ? base.args[0].text :
                base.kind == SourceExprKind::Identifier                                                                ? base.text :
                                                                                                                         std::string();
            const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(object);
            if (data == unit.dataBindings.end() || data->second.pointer || function.localTypes.count(object) ||
                (base.kind == SourceExprKind::Identifier) != data->second.isArray)
                return {};
            const bool readOnly = !(recovery.readOnlyData.count(object) && !recovery.readOnlyData.at(object));
            const std::string type = data->second.type == "u32" ? "uint32_t" : data->second.type;
            return std::string(readOnly ? "const " : "") + (type == "uint32_t" || type == "int32_t" || type == "float" ? type : "int32_t");
        };
        const std::function<void(SourceExpr &)> characterArithmetic = [&](SourceExpr &expression) {
            SourceExpr *arithmetic =
                expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 ? &expression.args[0] : &expression;
            const std::string word = byteOffset(*arithmetic);
            if (!word.empty()) {
                const std::string character = word.starts_with("const ") ? "const char *" : "char *";
                arithmetic->args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = character, .args = {arithmetic->args[0]}};
                if (arithmetic != &expression)
                    expression.args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = word + " *", .args = {std::move(expression.args[0])}};
            }
            for (SourceExpr &child : expression.args)
                characterArithmetic(child);
        };
        const std::function<void(std::vector<SourceStmt> &)> characterStatements = [&](std::vector<SourceStmt> &list) {
            for (SourceStmt &statement : list) {
                characterArithmetic(statement.expr);
                characterArithmetic(statement.value);
                characterArithmetic(statement.increment);
                characterStatements(statement.initializer);
                characterStatements(statement.body);
                characterStatements(statement.elseBody);
                for (SourceSwitchCase &clause : statement.cases) {
                    characterArithmetic(clause.value);
                    characterStatements(clause.body);
                }
            }
        };
        characterStatements(function.body.body);
    }

    {
        const auto wordSlot = [&](const SourceExpr &expression) -> std::optional<SourceSlotRef> {
            if (expression.kind != SourceExprKind::Identifier || expression.text.find('.') != std::string::npos)
                return std::nullopt;
            const std::optional<SourceSlotRef> slot = taskSlotRefFromName(expression.text);
            if (!slot || slot->aggregateField)
                return std::nullopt;
            const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.workspaceBindings.find(expression.text);
            if (binding != unit.workspaceBindings.end() && (binding->second.isArray || !scenarioDataTypeByName(binding->second.type)))
                return std::nullopt;
            return slot;
        };
        std::set<uint32_t> boundaries{static_cast<uint32_t>(unit.header.workspaceSize)};
        std::map<std::string, uint32_t> bases;
        std::map<std::string, std::set<uint32_t>> rowSizes;
        std::set<std::string> elementIndexed;
        const auto integral = [](const SourceExpr &value) -> const SourceExpr & {
            const SourceExpr *inner = &value;
            while (inner->kind == SourceExprKind::Cast && inner->args.size() == 1 && (inner->text == "int32_t" || inner->text == "uint32_t"))
                inner = &inner->args[0];
            return *inner;
        };

        struct RowOffset {
            uint32_t size;
            const SourceExpr *index;
            uint32_t bytes;
        };
        const auto rowOffset = [&](const SourceExpr &offset) -> std::optional<RowOffset> {
            const SourceExpr *bytes = &integral(offset);
            uint32_t column = 0;
            if (bytes->kind == SourceExprKind::Binary && bytes->text == "+" && bytes->args.size() == 2)
                if (const std::optional<uint32_t> constant = sourceDataConstantValue(bytes->args[1], true)) {
                    column = *constant;
                    bytes = &integral(bytes->args[0]);
                }
            if (bytes->kind != SourceExprKind::Binary || bytes->text != "*" || bytes->args.size() != 2)
                return std::nullopt;
            const std::optional<uint32_t> size = sourceDataConstantValue(bytes->args[1], true);
            if (!size)
                return std::nullopt;
            return RowOffset{.size = *size, .index = &integral(bytes->args[0]), .bytes = column};
        };
        for (const std::pair<const std::string, uint32_t> &offset : unit.workspaceOffsets)
            boundaries.insert(offset.second);
        for (const std::string &name : unit.functionOrder) {
            SourceFunctionAst &function = unit.functions.at(name);
            visitExpressions(function.body.body, [&](SourceExpr &expression) {
                if (expression.kind == SourceExprKind::Identifier)
                    if (const std::optional<SourceSlotRef> slot = taskSlotRefFromName(expression.text))
                        boundaries.insert(slot->baseOffset);
                if (expression.kind != SourceExprKind::Binary || (expression.text != "+" && expression.text != "-") || expression.args.size() != 2)
                    return;
                const SourceExpr &address = expression.args[0];
                if (address.kind != SourceExprKind::Unary || address.text != "&" || address.args.size() != 1)
                    return;
                if (sourceDataConstantValue(expression.args[1], true))
                    return;
                const std::optional<SourceSlotRef> slot = wordSlot(address.args[0]);
                if (!slot)
                    return;
                bases[address.args[0].text] = slot->byteOffset;
                const std::optional<RowOffset> row = expression.text == "+" ? rowOffset(expression.args[1]) : std::nullopt;
                if (row)
                    rowSizes[address.args[0].text].insert(row->size);
                else
                    elementIndexed.insert(address.args[0].text);
            });
        }
        std::set<std::string> arrays;
        std::map<std::string, std::pair<uint32_t, uint32_t>> rows;
        for (const std::pair<const std::string, uint32_t> &base : bases) {
            const std::set<uint32_t>::const_iterator next = boundaries.upper_bound(base.second);
            const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.workspaceBindings.find(base.first);
            const std::string type = binding == unit.workspaceBindings.end() ? "int32_t" : binding->second.type;
            const uint32_t width = scenarioDataTypeSize(*scenarioDataTypeByName(type));
            if (next == boundaries.end() || *next - base.second < 2 * width || (*next - base.second) % width)
                continue;
            const uint32_t extent = *next - base.second;
            const std::map<std::string, std::set<uint32_t>>::const_iterator row = rowSizes.find(base.first);
            if (row != rowSizes.end()) {
                const uint32_t size = *row->second.begin();
                if (elementIndexed.count(base.first) || row->second.size() != 1 || size % width || size / width < 2 || extent % size)
                    continue;
                unit.workspaceBindings[base.first] =
                    SourceDataBinding{.type = type + "[" + std::to_string(size / width) + "]", .elementCount = extent / size, .isArray = true};
                unit.workspaceOffsets[base.first] = base.second;
                rows[base.first] = {size, width};
                continue;
            }
            unit.workspaceBindings[base.first] = SourceDataBinding{.type = type, .elementCount = extent / width, .isArray = true};
            unit.workspaceOffsets[base.first] = base.second;
            arrays.insert(base.first);
        }
        const std::function<bool(const SourceExpr &)> element = [&](const SourceExpr &address) {
            return address.kind == SourceExprKind::Unary && address.text == "&" && address.args.size() == 1 &&
                address.args[0].kind == SourceExprKind::Identifier && arrays.count(address.args[0].text);
        };
        const auto index = [](SourceExpr base, SourceExpr position, const std::string &type) {
            return SourceExpr{.kind = SourceExprKind::Index, .args = {std::move(base), std::move(position)}, .cType = type};
        };
        const auto rowArray = [&](const SourceExpr &value) -> const std::pair<const std::string, std::pair<uint32_t, uint32_t>> * {
            if (value.kind != SourceExprKind::Unary || value.text != "&" || value.args.size() != 1 || value.args[0].kind != SourceExprKind::Identifier)
                return nullptr;
            const std::map<std::string, std::pair<uint32_t, uint32_t>>::const_iterator found = rows.find(value.args[0].text);
            return found == rows.end() ? nullptr : &*found;
        };

        const auto rowElement = [&](const SourceExpr &address, SourceExpr &row, uint32_t &column) -> bool {
            if (const std::pair<const std::string, std::pair<uint32_t, uint32_t>> *array = rowArray(address)) {
                row = index(identifier(array->first), number(0), "int32_t *");
                column = 0;
                return true;
            }
            if (address.kind != SourceExprKind::Binary || address.text != "+" || address.args.size() != 2)
                return false;
            const std::pair<const std::string, std::pair<uint32_t, uint32_t>> *array = rowArray(address.args[0]);
            if (!array)
                return false;
            const uint32_t size = array->second.first, width = array->second.second, columns = size / width;
            if (const std::optional<uint32_t> constant = sourceDataConstantValue(address.args[1], true)) {
                if (*constant >= unit.workspaceBindings.at(array->first).elementCount * columns)
                    return false;
                row = index(identifier(array->first), number(*constant / columns), "int32_t *");
                column = *constant % columns;
                return true;
            }
            const std::optional<RowOffset> offset = rowOffset(address.args[1]);
            if (!offset || offset->size != size || offset->bytes % width || offset->bytes >= size)
                return false;
            row = index(identifier(array->first), *offset->index, "int32_t *");
            column = offset->bytes / width;
            return true;
        };

        const std::function<void(SourceExpr &)> rewrite = [&](SourceExpr &expression) {
            SourceExpr row;
            uint32_t column = 0;
            if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 &&
                rowElement(expression.args[0], row, column)) {
                rewrite(row.args[1]);

                const uint32_t width = rows.at(row.args[0].text).second;
                SourceExpr element = index(std::move(row), number(column), expression.cType);
                expression = width == 4 ? std::move(element) :
                                          SourceExpr{.kind = SourceExprKind::Unary,
                                              .text = "*",
                                              .args = {SourceExpr{.kind = SourceExprKind::Cast,
                                                  .text = "int32_t *",
                                                  .args = {SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {std::move(element)}}}}}};
                return;
            }
            if (rowElement(expression, row, column)) {
                rewrite(row.args[1]);
                expression = column ? SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {index(std::move(row), number(column), "int32_t")}} :
                                      std::move(row);
                return;
            }
            if (expression.kind == SourceExprKind::Identifier && rows.count(expression.text)) {
                expression = index(index(expression, number(0), "int32_t *"), number(0), "int32_t");
                return;
            }
            if (element(expression)) {
                SourceExpr base = expression.args[0];
                expression = std::move(base);
                return;
            }
            if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 &&
                expression.args[0].kind == SourceExprKind::Binary && expression.args[0].text == "+" && element(expression.args[0].args[0])) {
                SourceExpr index{.kind = SourceExprKind::Index, .args = {expression.args[0].args[0].args[0], expression.args[0].args[1]}};
                rewrite(index.args[1]);
                expression = std::move(index);
                return;
            }
            if (expression.kind == SourceExprKind::Binary && (expression.text == "+" || expression.text == "-") && expression.args.size() == 2 &&
                element(expression.args[0])) {
                SourceExpr base = expression.args[0].args[0];
                expression.args[0] = std::move(base);
                rewrite(expression.args[1]);
                return;
            }
            if (expression.kind == SourceExprKind::Identifier && arrays.count(expression.text)) {
                SourceExpr base = expression;
                expression = SourceExpr{.kind = SourceExprKind::Index, .args = {std::move(base), number(0)}};
                return;
            }
            for (SourceExpr &argument : expression.args)
                rewrite(argument);
        };
        if (!arrays.empty() || !rows.empty())

            for (const std::string &name : unit.functionOrder) {
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
                statements(unit.functions.at(name).body.body);
            }
    }

    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        const auto wordObject = [&](const SourceExpr &base) -> std::string {
            if (base.kind != SourceExprKind::Unary || base.text != "&" || base.args.size() != 1 || base.args[0].kind != SourceExprKind::Identifier)
                return {};
            const std::string &object = base.args[0].text;
            std::string type;
            if (const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(object); local != function.localTypes.end()) {
                for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                    if (declaration.name == object && declaration.isArray)
                        return {};
                type = local->second;
            } else if (const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(object);
                       workspace != unit.workspaceBindings.end() && !workspace->second.isArray && !workspace->second.pointer) {
                type = workspace->second.type;
            }
            return type == "int32_t" || type == "uint32_t" || type == "float" ? type : std::string();
        };
        const auto bytes = [&](const SourceExpr &offset) {
            const SourceExpr *row = &offset;
            if (row->kind == SourceExprKind::Binary && row->text == "+" && row->args.size() == 2 && sourceDataConstantValue(row->args[1], true))
                row = &row->args[0];
            if (row->kind != SourceExprKind::Binary || row->text != "*" || row->args.size() != 2)
                return false;
            const std::optional<uint32_t> size = sourceDataConstantValue(row->args[1], true);
            return size && *size != 4U;
        };
        const std::function<void(SourceExpr &)> characters = [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Call && expression.args.size() == 1 &&
                (expression.text == "vm_load8" || expression.text == "vm_mem8" || expression.text == "vm_load16" || expression.text == "vm_mem16"))
                return;
            SourceExpr *arithmetic =
                expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 ? &expression.args[0] : &expression;
            const std::string type = arithmetic->kind == SourceExprKind::Binary && (arithmetic->text == "+" || arithmetic->text == "-") &&
                    arithmetic->args.size() == 2 && bytes(arithmetic->args[1]) ?
                wordObject(arithmetic->args[0]) :
                std::string();
            if (!type.empty()) {
                arithmetic->args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = "char *", .args = {arithmetic->args[0]}, .cType = "char *"};
                if (arithmetic != &expression)
                    expression.args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = type + " *", .args = {std::move(expression.args[0])}};
            }
            for (SourceExpr &child : expression.args)
                characters(child);
        };
        const std::function<void(std::vector<SourceStmt> &)> statements = [&](std::vector<SourceStmt> &list) {
            for (SourceStmt &statement : list) {
                characters(statement.expr);
                characters(statement.value);
                characters(statement.increment);
                statements(statement.initializer);
                statements(statement.body);
                statements(statement.elseBody);
                for (SourceSwitchCase &clause : statement.cases) {
                    characters(clause.value);
                    statements(clause.body);
                }
            }
        };
        statements(function.body.body);
    }

    for (const std::string &name : unit.functionOrder) {
        const SourceFunctionAst &function = unit.functions.at(name);

        const auto pointer = [&](const SourceExpr &base) {
            if (base.kind == SourceExprKind::Unary && base.text == "&")
                return true;
            if (base.kind != SourceExprKind::Identifier)
                return false;
            if (const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(base.text); local != function.localTypes.end()) {
                bool array = false;
                for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                    array |= declaration.name == base.text && declaration.isArray;
                return array || local->second.find('*') != std::string::npos;
            }
            const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(base.text);
            const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(base.text);
            return (workspace != unit.workspaceBindings.end() && (workspace->second.isArray || workspace->second.pointer)) ||
                (data != unit.dataBindings.end() && (data->second.isArray || data->second.pointer));
        };
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call || expression.args.size() != 1)
                return;
            const bool byte = expression.text == "vm_load8" || expression.text == "vm_mem8";
            const bool half = expression.text == "vm_load16" || expression.text == "vm_mem16";
            if (!byte && !half)
                return;
            const std::string view = byte ? "int8_t *" : "int16_t *";
            SourceExpr address = expression.args[0];
            if (address.kind == SourceExprKind::Binary && (address.text == "+" || address.text == "-") && address.args.size() == 2 &&
                pointer(address.args[0])) {
                SourceExpr base = address.args[0];
                address.args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = "int8_t *", .args = {std::move(base)}, .cType = "int8_t *"};
                address.cType = "int8_t *";
                if (half)
                    address = SourceExpr{.kind = SourceExprKind::Cast, .text = view, .args = {std::move(address)}, .cType = view};
            } else
                address = SourceExpr{.kind = SourceExprKind::Cast, .text = view, .args = {std::move(address)}, .cType = view};
            expression = SourceExpr{.kind = SourceExprKind::Unary, .text = "*", .args = {std::move(address)}, .cType = byte ? "int8_t" : "int16_t"};
        });
    }

    std::set<uint32_t> characters;
    for (uint32_t character : shenmue::s1::kValidScenCharacterNames)
        characters.insert(character);
    const auto spelling = [&](uint32_t value) -> std::string {
        std::string letters;
        bool alphabetic = false, valid = true;
        for (uint32_t byte = 0; byte < 4; ++byte) {
            const char character = static_cast<char>(value >> (byte * 8U));
            alphabetic |= character >= 'A' && character <= 'Z';
            valid &= (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_';
            letters += character;
        }
        if (!valid || !alphabetic)
            return {};
        return characters.count(value) ? "CHARA_" + letters : "SHEN_NAME('" + letters + "')";
    };
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            if (!command || !command->handlerName)
                return;
            std::vector<std::optional<uint32_t>> observed;
            for (const SourceExpr &argument : expression.args)
                observed.push_back(sourceDataConstantValue(argument, true));
            for (size_t index = 0; index < expression.args.size(); ++index) {
                if (!observed[index] || nativeParameter(command->handlerName, index, observed).type != "shen_name")
                    continue;
                const std::string spelled = spelling(*observed[index]);
                if (!spelled.empty())
                    expression.args[index] = identifier(spelled);
            }
        });

    std::map<std::string, std::set<size_t>> names;
    const auto parameter = [](const SourceFunctionAst &function, const SourceExpr &argument) -> std::optional<size_t> {
        if (argument.kind != SourceExprKind::Identifier)
            return std::nullopt;
        for (size_t index = 0; index < function.params.size(); ++index)
            if (function.params[index] == argument.text)
                return index;
        return std::nullopt;
    };

    const auto callee = [&](const SourceExpr &call) -> std::pair<std::string, size_t> {
        if (unit.functions.count(call.text))
            return {call.text, 0};
        if (call.text == "Script_scenCreateTask" && call.args.size() >= 2 && call.args[0].kind == SourceExprKind::Identifier &&
            unit.functions.count(call.args[0].text))
            return {call.args[0].text, 2};
        return {};
    };
    for (bool grown = true; grown;) {
        grown = false;
        for (const std::string &name : unit.functionOrder) {
            const SourceFunctionAst &function = unit.functions.at(name);
            visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
                if (expression.kind != SourceExprKind::Call)
                    return;
                const abi::CommandSpec *command = abiCommandForCallName(expression.text);
                std::vector<std::optional<uint32_t>> observed;
                for (const SourceExpr &argument : expression.args)
                    observed.push_back(sourceDataConstantValue(argument, true));
                const std::pair<std::string, size_t> target = callee(expression);
                for (size_t index = 0; index < expression.args.size(); ++index) {
                    const std::optional<size_t> passed = parameter(function, expression.args[index]);
                    if (!passed)
                        continue;
                    const bool native = command && command->handlerName && !unit.functions.count(expression.text) &&
                        nativeParameter(command->handlerName, index, observed).type == "shen_name";
                    const bool script = !target.first.empty() && index >= target.second && names[target.first].count(index - target.second);
                    if ((native || script) && names[name].insert(*passed).second)
                        grown = true;
                }
            });
        }
    }
    for (const std::string &name : unit.functionOrder)
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            const std::pair<std::string, size_t> target = callee(expression);
            if (target.first.empty() || !names.count(target.first))
                return;
            for (size_t index = target.second; index < expression.args.size(); ++index) {
                if (!names.at(target.first).count(index - target.second))
                    continue;
                const std::optional<uint32_t> value = sourceDataConstantValue(expression.args[index], true);
                const std::string spelled = value ? spelling(*value) : std::string();
                if (!spelled.empty())
                    expression.args[index] = identifier(spelled);
            }
        });

    const auto normalized = [](std::string type) {
        if (type.starts_with("const "))
            type.erase(0, 6);
        if (type == "shen_name" || type == "u32")
            type = "uint32_t";
        if (type == "cstr" || type == "int8_t")
            type = "char";
        if (type == "bytes" || type == "pad")
            type = "uint8_t";
        return type;
    };
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        const auto pointee = [&](const SourceExpr &argument) -> std::string {
            if (argument.kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(argument.text);
                if (local != function.localTypes.end())
                    return local->second.find('*') != std::string::npos ? trimCopy(local->second.substr(0, local->second.rfind('*'))) : std::string();
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(argument.text);
                if (workspace != unit.workspaceBindings.end()) {
                    if (workspace->second.pointer)
                        return trimCopy(workspace->second.pointerType.substr(0, workspace->second.pointerType.rfind('*')));
                    return workspace->second.isArray && workspace->second.type.find('[') == std::string::npos ? workspace->second.type : std::string();
                }
                const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(argument.text);
                if (data != unit.dataBindings.end() && data->second.isArray && !data->second.pointer)
                    return data->second.type;

                if (data != unit.dataBindings.end() && !data->second.isArray && data->second.pointer)
                    return data->second.pointerType;
                if (functionPointers.count(argument.text))
                    return "void (*)(void)";
            }
            if (argument.kind == SourceExprKind::Unary && argument.text == "&" && argument.args.size() == 1 &&
                argument.args[0].kind == SourceExprKind::Identifier) {
                const std::string &object = argument.args[0].text;
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(object);
                if (local != function.localTypes.end())
                    return local->second;
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(object);
                if (workspace != unit.workspaceBindings.end() && !workspace->second.isArray && !workspace->second.pointer)
                    return workspace->second.type;
                const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(object);
                if (functionPointers.count(object))
                    return "void (*)(void)";
                if (data != unit.dataBindings.end() && !data->second.isArray && !data->second.pointer)
                    return data->second.type;

                if (data != unit.dataBindings.end() && !data->second.isArray && data->second.pointer)
                    return data->second.pointerType;
                if (workspace != unit.workspaceBindings.end() && !workspace->second.isArray && workspace->second.pointer)
                    return workspace->second.pointerType;
            }
            return {};
        };
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Call)
                return;
            std::vector<std::pair<size_t, std::string>> parameters;
            if (const std::map<std::string, SourceFunctionAst>::const_iterator callee = unit.functions.find(expression.text); callee != unit.functions.end()) {
                for (size_t index = 0; index < callee->second.params.size() && index < expression.args.size(); ++index) {
                    const std::map<std::string, std::string>::const_iterator type = callee->second.localTypes.find(callee->second.params[index]);
                    if (type != callee->second.localTypes.end() && type->second.find('*') != std::string::npos)
                        parameters.emplace_back(index, type->second);
                }
            } else {
                const abi::CommandSpec *command = abiCommandForCallName(expression.text);
                if (!command || !command->handlerName)
                    return;
                std::vector<std::optional<uint32_t>> constants;
                for (const SourceExpr &argument : expression.args)
                    constants.push_back(sourceDataConstantValue(argument, true));
                const NativeArgumentLayout *layout = nativeArgumentLayout(command->handlerName, constants);
                if (!layout)
                    return;
                for (const NativePointerParameter &pointer : layout->pointers)
                    if (pointer.byteOffset / 4U < expression.args.size())
                        parameters.emplace_back(pointer.byteOffset / 4U, std::string(pointer.readOnly ? "const " : "") + pointer.type + " *");
            }
            for (const std::pair<size_t, std::string> &parameter : parameters) {
                SourceExpr &argument = expression.args[parameter.first];
                const std::optional<uint32_t> constant = sourceDataConstantValue(argument, true);
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(argument.text);
                const bool address = (argument.kind == SourceExprKind::Unary && argument.text == "&") || argument.cType.find('*') != std::string::npos ||
                    (argument.kind == SourceExprKind::Cast && argument.text.find('*') != std::string::npos) || (constant && *constant == 0) ||
                    (argument.kind == SourceExprKind::Identifier &&
                        (unit.dataBindings.count(argument.text) || unit.functions.count(argument.text) ||
                            (unit.workspaceBindings.count(argument.text) &&
                                (unit.workspaceBindings.at(argument.text).isArray || unit.workspaceBindings.at(argument.text).pointer)) ||
                            (local != function.localTypes.end() && local->second.find('*') != std::string::npos)));
                const std::string target = normalized(trimCopy(parameter.second.substr(0, parameter.second.rfind('*'))));
                const std::string object = pointee(argument);
                const bool different = !object.empty() && target != "void" && normalized(object) != target;

                const std::string data = argument.kind == SourceExprKind::Identifier                            ? argument.text :
                    argument.kind == SourceExprKind::Unary && argument.text == "&" && argument.args.size() == 1 ? argument.args[0].text :
                                                                                                                  std::string();
                const bool readOnly = unit.dataBindings.count(data) && !(recovery.readOnlyData.count(data) && !recovery.readOnlyData.at(data));
                const bool discards = readOnly && !parameter.second.starts_with("const ");
                if (!address || different || discards)
                    argument = SourceExpr{.kind = SourceExprKind::Cast, .text = parameter.second, .args = {argument}};
            }
        });
    }

    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst &function = unit.functions.at(name);
        const auto localArray = [&](const std::string &object) {
            bool array = false;
            for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                array |= declaration.name == object && declaration.isArray;
            return array;
        };
        const auto address = [&](const SourceExpr &value) {
            if (value.kind == SourceExprKind::Unary && value.text == "&")
                return true;
            if (value.kind == SourceExprKind::Cast)
                return value.text.find('*') != std::string::npos;
            if (value.kind != SourceExprKind::Identifier)
                return false;
            if (const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.text); local != function.localTypes.end())
                return local->second.find('*') != std::string::npos || local->second.find('[') != std::string::npos || localArray(value.text);
            const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(value.text);
            const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(value.text);
            return unit.functions.count(value.text) || (data != unit.dataBindings.end() && (data->second.isArray || data->second.pointer)) ||
                (workspace != unit.workspaceBindings.end() && (workspace->second.isArray || workspace->second.pointer));
        };
        const std::function<bool(const SourceExpr &)> integer = [&](const SourceExpr &value) {
            if (value.kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.text);
                if (local != function.localTypes.end())
                    return local->second == "int32_t" || local->second == "uint32_t";
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(value.text);
                return workspace != unit.workspaceBindings.end() && !workspace->second.isArray && !workspace->second.pointer &&
                    (workspace->second.type == "int32_t" || workspace->second.type == "uint32_t");
            }
            if (value.kind == SourceExprKind::Index && value.args.size() == 2 && value.args[0].kind == SourceExprKind::Identifier) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.args[0].text);
                if (local != function.localTypes.end())
                    return localArray(value.args[0].text) && (local->second == "int32_t" || local->second == "uint32_t");
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(value.args[0].text);
                return workspace != unit.workspaceBindings.end() && (workspace->second.type == "int32_t" || workspace->second.type == "uint32_t");
            }
            return value.kind == SourceExprKind::Binary && value.args.size() == 2 && (value.text == "+" || value.text == "-") && integer(value.args[0]);
        };

        const auto narrowArray = [&](const SourceExpr &value) {
            const SourceExpr &offset =
                value.kind == SourceExprKind::Binary && value.args.size() == 2 && (value.text == "+" || value.text == "-") ? value.args[0] : value;

            const bool addressOf = offset.kind == SourceExprKind::Unary && offset.text == "&" && offset.args.size() == 1;
            const SourceExpr &base = addressOf ? offset.args[0] : offset;
            if (base.kind != SourceExprKind::Identifier)
                return false;
            std::string type;
            if (addressOf) {
                const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(base.text);
                const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(base.text);
                if (local != function.localTypes.end()) {
                    if (!localArray(base.text))
                        type = local->second;
                } else if (workspace != unit.workspaceBindings.end() && !workspace->second.isArray && !workspace->second.pointer) {
                    type = workspace->second.type;
                }
            } else if (function.localTypes.count(base.text)) {
                for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                    if (declaration.name == base.text && declaration.isArray)
                        type = declaration.type;
            } else if (const std::map<std::string, SourceDataBinding>::const_iterator data = unit.dataBindings.find(base.text);
                       data != unit.dataBindings.end() && data->second.isArray && !data->second.pointer) {
                type = data->second.type;
            } else if (const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(base.text);
                       workspace != unit.workspaceBindings.end() && workspace->second.isArray && !workspace->second.pointer) {
                type = workspace->second.type;
            }
            return type == "bytes" || type == "pad" || type == "cstr" || type == "uint8_t" || type == "int8_t" || type == "char" || type == "uint16_t" ||
                type == "int16_t";
        };
        const std::function<void(SourceExpr &)> conversions = [&](SourceExpr &expression) {
            for (SourceExpr &child : expression.args)
                conversions(child);
            if (expression.kind == SourceExprKind::Unary && expression.text == "*" && expression.args.size() == 1 &&
                (integer(expression.args[0]) || narrowArray(expression.args[0])))
                expression.args[0] = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t *", .args = {expression.args[0]}};
            const std::map<std::string, SourceFunctionAst>::const_iterator callee =
                expression.kind == SourceExprKind::Call ? unit.functions.find(expression.text) : unit.functions.end();
            if (callee != unit.functions.end())
                for (size_t index = 0; index < expression.args.size() && index < callee->second.params.size(); ++index) {
                    const std::map<std::string, std::string>::const_iterator type = callee->second.localTypes.find(callee->second.params[index]);
                    if (type != callee->second.localTypes.end() && (type->second == "int32_t" || type->second == "uint32_t") && address(expression.args[index]))
                        expression.args[index] = SourceExpr{.kind = SourceExprKind::Cast, .text = type->second, .args = {expression.args[index]}};
                }

            const abi::CommandSpec *command =
                expression.kind == SourceExprKind::Call && callee == unit.functions.end() ? abiCommandForCallName(expression.text) : nullptr;
            if (command && command->handlerName) {
                std::vector<std::optional<uint32_t>> constants;
                for (const SourceExpr &argument : expression.args)
                    constants.push_back(sourceDataConstantValue(argument, true));
                if (const NativeArgumentLayout *layout = nativeArgumentLayout(command->handlerName, constants))
                    for (size_t index = 0; index < expression.args.size(); ++index) {
                        const uint32_t offset = static_cast<uint32_t>(index * 4U);
                        bool word = offset < layout->parameterBytes && (layout->variadicOffset < 0 || offset < static_cast<uint32_t>(layout->variadicOffset));
                        for (const NativePointerParameter &pointer : layout->pointers)
                            word &= pointer.byteOffset != offset;
                        for (const uint32_t floating : layout->floatArguments)
                            word &= floating != offset;
                        if (word && address(expression.args[index]))
                            expression.args[index] = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {expression.args[index]}};
                    }
            }

            if (expression.kind == SourceExprKind::Cast && expression.text == "float" && expression.args.size() == 1) {
                const SourceExpr &value = expression.args[0];
                const std::map<std::string, SourceFunctionAst>::const_iterator target =
                    value.kind == SourceExprKind::Call ? unit.functions.find(value.text) : unit.functions.end();
                const abi::CommandSpec *native =
                    value.kind == SourceExprKind::Call && target == unit.functions.end() ? abiCommandForCallName(value.text) : nullptr;
                const NativePrototype *prototype = native && native->handlerName ? nativePrototype(native->handlerName) : nullptr;
                const bool floating = value.kind == SourceExprKind::Call ?
                    (target != unit.functions.end() && target->second.returnType == "float") || (prototype && prototype->result == "float") :
                    explicitExpressionIsFloat(value, &unit, &function);
                if (floating)
                    expression.args[0] =
                        SourceExpr{.kind = SourceExprKind::Cast, .text = "scn3_bits.i", .args = {std::move(expression.args[0])}, .cType = "int32_t"};
            }

            if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2 &&
                (expression.text == "<" || expression.text == "<=" || expression.text == ">" || expression.text == ">="))
                for (SourceExpr &operand : expression.args) {
                    const SourceExpr *object = &operand;
                    if (object->kind == SourceExprKind::Unary && object->text == "*" && object->args.size() == 1) {
                        object = &object->args[0];
                        if (object->kind == SourceExprKind::Binary && (object->text == "+" || object->text == "-") && object->args.size() == 2)
                            object = &object->args[0];
                        if (object->kind == SourceExprKind::Unary && object->text == "&" && object->args.size() == 1)
                            object = &object->args[0];
                    } else if (object->kind == SourceExprKind::Index && !object->args.empty()) {
                        object = &object->args[0];
                    }
                    if (object->kind != SourceExprKind::Identifier)
                        continue;
                    std::string type;
                    if (const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(object->text);
                        local != function.localTypes.end())
                        type = local->second;
                    else if (unit.workspaceBindings.count(object->text) && !unit.workspaceBindings.at(object->text).pointer)
                        type = unit.workspaceBindings.at(object->text).type;
                    else if (unit.dataBindings.count(object->text) && !unit.dataBindings.at(object->text).pointer)
                        type = unit.dataBindings.at(object->text).type;
                    if (type == "u32" || type == "uint32_t" || type == "shen_name")
                        operand = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {std::move(operand)}};
                }

            if (expression.kind == SourceExprKind::Binary && (expression.text == "==" || expression.text == "!=") && expression.args.size() == 2)
                for (size_t side = 0; side < 2; ++side)
                    if (expression.args[side].kind == SourceExprKind::Identifier && unit.functions.count(expression.args[side].text) &&
                        sourceDataConstantValue(expression.args[1 - side], true))
                        expression.args[side] = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {expression.args[side]}};
        };

        const auto record = [&](const SourceExpr &value) {
            if (value.kind != SourceExprKind::Identifier)
                return false;
            const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(value.text);
            if (local == function.localTypes.end() || !findScenarioDataSchema(local->second))
                return false;
            for (const SourceLocalDeclaration &declaration : function.localDeclarations)
                if (declaration.name == value.text && declaration.isArray)
                    return false;
            return true;
        };
        const auto word = [&](SourceExpr &value, bool floating) {
            SourceExpr object = std::move(value);
            value = SourceExpr{.kind = SourceExprKind::Unary,
                .text = "*",
                .args = {SourceExpr{.kind = SourceExprKind::Cast,
                    .text = floating ? "float *" : "int32_t *",
                    .args = {SourceExpr{.kind = SourceExprKind::Unary, .text = "&", .args = {std::move(object)}}}}}};
        };
        const std::function<void(SourceExpr &)> words = [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Unary && expression.text == "&")
                return;
            if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2)
                for (size_t side = 0; side < 2; ++side)
                    if (record(expression.args[side]))
                        word(expression.args[side], explicitExpressionIsFloat(expression.args[1 - side], &unit, &function));
            for (SourceExpr &child : expression.args)
                words(child);
        };
        const std::function<void(std::vector<SourceStmt> &)> statements = [&](std::vector<SourceStmt> &list) {
            for (SourceStmt &statement : list) {
                words(statement.expr);
                words(statement.value);
                if (statement.kind == SourceStmtKind::Assign && record(statement.expr))
                    word(statement.expr, explicitExpressionIsFloat(statement.value, &unit, &function));
                if (statement.kind == SourceStmtKind::Assign && record(statement.value))
                    word(statement.value, explicitExpressionIsFloat(statement.expr, &unit, &function));
                conversions(statement.expr);
                conversions(statement.value);

                if (statement.kind == SourceStmtKind::Assign && statement.expr.kind == SourceExprKind::Unary && statement.expr.text == "*" &&
                    statement.expr.args.size() == 1 && statement.expr.args[0].kind == SourceExprKind::Cast && statement.expr.args[0].text == "int32_t *" &&
                    explicitExpressionIsFloat(statement.value, &unit, &function))
                    statement.expr.args[0].text = "float *";

                const SourceExpr *element = statement.expr.kind == SourceExprKind::Unary && statement.expr.text == "*" && statement.expr.args.size() == 1 ?
                    &statement.expr.args[0] :
                    nullptr;
                if (element && element->kind == SourceExprKind::Binary && (element->text == "+" || element->text == "-") && element->args.size() == 2)
                    element = &element->args[0];
                if (element && element->kind == SourceExprKind::Unary && element->text == "&" && element->args.size() == 1)
                    element = &element->args[0];
                const bool floatElement = element && element->kind == SourceExprKind::Identifier &&
                    ((function.localTypes.count(element->text) && function.localTypes.at(element->text) == "float") ||
                        (!function.localTypes.count(element->text) && unit.workspaceBindings.count(element->text) &&
                            unit.workspaceBindings.at(element->text).type == "float"));
                if (statement.kind == SourceStmtKind::Assign && statement.value.kind == SourceExprKind::Unary && statement.value.text == "*" &&
                    statement.value.args.size() == 1 && statement.value.args[0].kind == SourceExprKind::Cast &&
                    (floatElement || explicitExpressionIsFloat(statement.expr, &unit, &function))) {
                    std::string &view = statement.value.args[0].text;
                    const bool readOnly = view.starts_with("const ");
                    const std::string pointee = readOnly ? view.substr(6) : view;
                    if (pointee == "int32_t *" || pointee == "uint32_t *")
                        view = readOnly ? "const float *" : "float *";
                }
                if (statement.kind == SourceStmtKind::Assign && address(statement.value) && integer(statement.expr))
                    statement.value = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {statement.value}};

                if (statement.kind == SourceStmtKind::Assign && statement.expr.kind == SourceExprKind::Identifier &&
                    statement.value.kind == SourceExprKind::Identifier && unit.dataBindings.count(statement.value.text) &&
                    !(recovery.readOnlyData.count(statement.value.text) && !recovery.readOnlyData.at(statement.value.text)) &&
                    !function.localTypes.count(statement.expr.text) && unit.workspaceBindings.count(statement.expr.text)) {
                    const SourceDataBinding &target = unit.workspaceBindings.at(statement.expr.text);
                    if (target.pointer && !target.pointerType.starts_with("const "))
                        statement.value = SourceExpr{.kind = SourceExprKind::Cast, .text = target.pointerType, .args = {statement.value}};
                }
                if (statement.kind == SourceStmtKind::Return && (function.returnType == "int32_t" || function.returnType == "uint32_t") &&
                    address(statement.expr))
                    statement.expr = SourceExpr{.kind = SourceExprKind::Cast, .text = function.returnType, .args = {statement.expr}};
                statements(statement.body);
                statements(statement.elseBody);
                for (SourceSwitchCase &clause : statement.cases)
                    statements(clause.body);
            }
        };
        statements(function.body.body);

        std::vector<SourceStmt> &body = function.body.body;
        if (!body.empty() && body.back().kind == SourceStmtKind::Return && body.back().expr.kind == SourceExprKind::Binary && body.back().expr.text == "=" &&
            body.back().expr.args.size() == 2 && body.back().expr.args[0].kind == SourceExprKind::Identifier) {
            const std::string &target = body.back().expr.args[0].text;
            const std::map<std::string, std::string>::const_iterator local = function.localTypes.find(target);
            const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit.workspaceBindings.find(target);
            const std::string type = local != function.localTypes.end() ? local->second :
                workspace != unit.workspaceBindings.end()               ? workspace->second.type :
                                                                          "";
            if (type == "int8_t" || type == "int16_t" || type == "uint8_t" || type == "uint16_t") {
                SourceExpr assignment = std::move(body.back().expr);
                body.back() = SourceStmt{.kind = SourceStmtKind::Assign, .expr = std::move(assignment.args[0]), .value = std::move(assignment.args[1])};
            }
        }
    }
    recoverCDataTables(unit, module.originalBlob, moduleDataEnd(module), recovery.readOnlyData);

    std::map<std::string, uint32_t> references;
    for (const std::string &name : unit.functionOrder) {
        const std::function<void(const std::vector<SourceStmt> &)> count = [&](const std::vector<SourceStmt> &statements) {
            for (const SourceStmt &statement : statements) {
                if (statement.kind == SourceStmtKind::Goto)
                    ++references[statement.label];
                count(statement.initializer);
                count(statement.body);
                count(statement.elseBody);
                for (const SourceSwitchCase &clause : statement.cases)
                    count(clause.body);
            }
        };
        count(unit.functions.at(name).body.body);
        visitExpressions(unit.functions.at(name).body.body, [&](SourceExpr &expression) {
            if (expression.kind == SourceExprKind::Identifier && unit.codeLabels.count(expression.text))
                references[expression.text] += 2;
        });
    }
    for (const std::string &name : unit.functionOrder) {
        if (!plain.count(name))
            recoverCForwardBranches(unit.functions.at(name).body.body, references);
        recoverCLiteralRadix(unit.functions.at(name).body.body);
        if (!plain.count(name))
            recoverCSiblingNames(unit.functions.at(name));
    }
    return recovery;
}

std::map<std::string, SourceExpr> cStringLiterals(const CRecovery &recovery, const Scn3Module &module) {
    const SourceUnit &unit = recovery.unit;
    const std::vector<uint8_t> &blob = module.originalBlob;
    const uint32_t dataEnd = moduleDataEnd(module);
    std::map<std::string, SourceExpr> strings;
    std::map<std::string, uint32_t> counts;
    std::set<uint32_t> boundaries, writableTargets;
    for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols)
        if (symbol.second >= module.parsedHeader.dataSegmentOffset && symbol.second < dataEnd)
            boundaries.insert(symbol.second);
    boundaries.insert(dataEnd);
    for (uint32_t offset = module.parsedHeader.relocationTableOffset; offset + 8U <= blob.size(); offset += 8U) {
        const uint32_t kind = readU32(blob, offset), site = readU32(blob, offset + 4U);
        if (!kind && !site)
            break;
        if (kind == 0x1402U && site >= module.parsedHeader.dataSegmentOffset && site + 4U <= dataEnd)
            writableTargets.insert(readU32(blob, site));
    }
    for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols) {
        const SourceDataBinding &binding = unit.dataBindings.at(symbol.first);
        if (binding.pointer || !binding.isArray || (binding.type != "char" && binding.type != "cstr") ||
            symbol.second < module.parsedHeader.dataSegmentOffset || symbol.second >= dataEnd)
            continue;
        const uint32_t start = symbol.second, end = *boundaries.upper_bound(start);
        uint32_t length = 0;
        while (start + length < end && blob[start + length])
            ++length;
        if (start + length == end)
            continue;
        bool padding = true;
        for (uint32_t offset = start + length; offset < end; ++offset)
            padding &= blob[offset] == 0;
        if (!padding)
            continue;
        const std::string text(blob.begin() + start, blob.begin() + start + length);
        const std::map<std::string, bool>::const_iterator readonly = recovery.readOnlyData.find(symbol.first);
        const std::set<uint32_t>::const_iterator writable = writableTargets.lower_bound(start);
        if ((writable == writableTargets.end() || *writable >= end) && (readonly == recovery.readOnlyData.end() || readonly->second)) {
            ++counts[text];
            strings.emplace(symbol.first, SourceExpr{.kind = SourceExprKind::StringLiteral, .text = text});
        }
    }
    for (std::pair<const std::string, SourceExpr> &string : strings)
        if (counts.at(string.second.text) != 1)
            string.second.args.push_back(identifier(string.first));
    return strings;
}

void inlineCStringArguments(SourceFunctionAst &function, const std::map<std::string, SourceExpr> &strings) {
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Call)
            return;
        for (SourceExpr &argument : expression.args) {
            SourceExpr *value = &argument;
            while (value->kind == SourceExprKind::Cast && value->args.size() == 1)
                value = &value->args[0];
            if (value->kind == SourceExprKind::Unary && value->text == "&" && value->args.size() == 1 && value->args[0].kind == SourceExprKind::Index &&
                value->args[0].args.size() == 2)
                value = &value->args[0].args[0];
            else if (value->kind == SourceExprKind::Binary && value->text == "+" && value->args.size() == 2)
                value = &value->args[0];
            if (value->kind != SourceExprKind::Identifier)
                continue;
            const std::map<std::string, SourceExpr>::const_iterator text = strings.find(value->text);
            if (text == strings.end())
                continue;
            SourceExpr literal = text->second;
            literal.cType = value->cType;
            literal.cConversion = value->cConversion;
            *value = std::move(literal);
        }
    });
}

void functionDefinition(std::ostream &out, SourceFunctionAst function);

std::string functionSource(const CRecovery &recovery, const std::string &name) {
    const SourceUnit &unit = recovery.unit;
    SourceFunctionAst function = unit.functions.at(name);
    if (function.returnType.empty())
        throw std::runtime_error("Unresolved C return type");
    std::set<std::string> calls, globals, characters;
    std::map<std::string, SourceExpr> literalStorage;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::StringLiteral && expression.args.size() == 1)
            literalStorage.emplace(expression.args[0].text, expression);
        if (unit.functions.count(expression.text) && expression.text != name)
            calls.insert(expression.text);
        if (expression.kind != SourceExprKind::Identifier)
            return;
        const std::string base = expression.text.substr(0, expression.text.find('.'));
        if (unit.dataBindings.count(base) || unit.workspaceBindings.count(base))
            globals.insert(base);
        if (expression.text.starts_with("CHARA_") && expression.text.size() == 10)
            characters.insert(expression.text);
    });
    std::ostringstream out;
    out << "#include \"scenario.h\"\n\n";
    std::set<uint32_t> knownCharacters;
    for (uint32_t character : shenmue::s1::kValidScenCharacterNames)
        knownCharacters.insert(character);
    for (const std::string &character : characters) {
        const std::optional<uint32_t> value = sourceDataConstantValue(SourceExpr{.kind = SourceExprKind::Identifier, .text = character});
        if (value && !knownCharacters.count(*value))
            out << "enum { " << character << " = " << hexUnsigned(*value) << " };\n";
    }
    for (const std::string &call : calls) {

        SourceFunctionAst callee = unit.functions.at(call);
        bool typed = !callee.returnType.empty();
        for (const std::string &parameter : callee.params)
            typed &= callee.localTypes.count(parameter) != 0;
        if (!typed) {
            callee.returnType = "int32_t";
            callee.variadic = false;
            callee.fixedParameterCount.reset();
            for (const std::string &parameter : callee.params)
                callee.localTypes[parameter] = "int32_t";
        }
        callee.externalDefinition = true;
        cPrototype(out, callee);
        out << ";\n";
    }
    for (const std::string &global : globals) {
        const SourceDataBinding &binding = unit.workspaceBindings.count(global) ? unit.workspaceBindings.at(global) : unit.dataBindings.at(global);
        if (literalStorage.count(global)) {
            SourceExpr literal = literalStorage.at(global);
            literal.args.clear();
            out << "static const char " << global << "[" << binding.elementCount << "] __attribute__((scn_string_literal)) = " << printExpression(literal)
                << ";\n";
            continue;
        }
        const std::string type = binding.type == "u32"       ? "uint32_t" :
            binding.type == "bytes" || binding.type == "pad" ? "uint8_t" :
            binding.type == "cstr"                           ? "char" :
                                                               binding.type;
        const size_t columns = type.find('[');
        if (binding.pointer)
            out << "extern " << binding.pointerType << (binding.pointerType.ends_with("*") ? "" : " ") << global;
        else
            out << "extern " << type.substr(0, columns) << " " << global;
        if (binding.isArray)
            out << "[" << binding.elementCount << "]";
        if (columns != std::string::npos)
            out << type.substr(columns);
        out << ";\n";
    }
    out << "\n";
    function.externalDefinition = true;
    functionDefinition(out, function);
    return out.str();
}

void functionDefinition(std::ostream &out, SourceFunctionAst function) {
    bool scoped = false;
    const std::function<void(std::vector<SourceStmt> &)> declarations = [&](std::vector<SourceStmt> &body) {
        for (SourceStmt &statement : body) {
            if (statement.kind == SourceStmtKind::Declaration) {
                scoped = true;
                const std::map<std::string, int32_t>::const_iterator offset = function.requestedLocalOffsets.find(statement.declaration.name);
                if (offset != function.requestedLocalOffsets.end())
                    statement.declaration.stackOffset = offset->second;
            }
            declarations(statement.body);
            declarations(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                declarations(clause.body);
        }
    };
    declarations(function.body.body);
    std::vector<SourceStmt> body = function.fixedParameterCount ? cVariadicBody(function) : function.body.body;
    std::ostringstream bindings;
    uint32_t ordinal = 0;
    visitExpressions(body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::StringLiteral)
            return;
        if (expression.args.size() == 1)
            bindings << (bindings.tellp() > 0 ? ", " : "") << ordinal << ", " << expression.args[0].text;
        ++ordinal;
    });
    if (function.requestedFrameSize)
        out << "__attribute__((scn_stack_size(" << *function.requestedFrameSize << ")))\n";
    if (bindings.tellp() > 0)
        out << "__attribute__((scn_string_bindings(" << bindings.str() << ")))\n";
    cPrototype(out, function);
    out << " {\n";
    if (!scoped)
        for (const SourceLocalDeclaration &declaration : function.localDeclarations) {
            const size_t rows = declaration.type.find('[');
            out << "  " << declaration.type.substr(0, rows) << (declaration.type.ends_with("*") ? "" : " ") << declaration.name;
            if (declaration.isArray)
                out << "[" << declaration.count << "]";
            if (rows != std::string::npos)
                out << declaration.type.substr(rows);
            const std::map<std::string, int32_t>::const_iterator offset = function.requestedLocalOffsets.find(declaration.name);
            if (offset != function.requestedLocalOffsets.end())
                out << " __attribute__((scn_stack_offset(" << offset->second << ")))";
            out << ";\n";
        }
    for (const SourceStmt &statement : body)
        printStatement(out, statement, "  ", true);
    out << "}\n";
}

std::map<std::string, SourceUnit> g_parsedSources;
std::map<std::string, std::string> g_sourceFailures;

Json functionIssues(CRecovery &recovery, const std::string &name) {
    SourceFunctionAst &function = recovery.unit.functions.at(name);
    Json result = {{"name", name}, {"offset", function.offset}, {"verified_c", false}, {"layout_metadata", recovery.layouts.count(name) != 0},
        {"issues", recovery.issues.at(name)}, {"intrinsics", Json::object()}};
    std::set<std::string> missingPrototypes;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Call) {
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            if (command && command->handlerName && !nativeCallIgnoresParameters(command->handlerName) && !nativeArgumentLayout(command->handlerName) &&
                !nativeCallHasModes(command->handlerName) && missingPrototypes.insert(command->handlerName).second)
                result["issues"].push_back(issue(1, "native_prototype", command->handlerName));
        }
        if (expression.kind != SourceExprKind::Call ||
            (!expression.text.starts_with("vm_") && expression.text != "as_float" && expression.text != "as_word" && expression.text != "byte_address"))
            return;
        Json &intrinsics = result["intrinsics"];
        if (intrinsics.count(expression.text)) {
            intrinsics[expression.text] = intrinsics[expression.text].get<unsigned>() + 1;
            return;
        }
        intrinsics[expression.text] = 1;
        const bool memory = expression.text == "vm_load8" || expression.text == "vm_mem8" || expression.text == "vm_load16" || expression.text == "vm_mem16";
        const bool bitRepresentation = expression.text == "as_word" || expression.text == "as_float";
        const unsigned category = memory ? 2 : bitRepresentation ? 3 : (expression.text == "vm_scnf_offset" || expression.text == "vm_float_to_int") ? 4 : 1;
        result["issues"].push_back(issue(category, memory ? "typed_memory_not_emitted" : "vm_operation", expression.text));
    });
    return result;
}

Json analyzeFunction(CRecovery &recovery, const std::string &name, bool includeSource) {
    SourceUnit &unit = recovery.unit;
    SourceFunctionAst &function = unit.functions.at(name);
    Json result = functionIssues(recovery, name);
    if (!result["issues"].empty())
        return result;
    std::string source;
    try {
        source = functionSource(recovery, name);
        if (g_sourceFailures.count(source))
            throw std::runtime_error(g_sourceFailures.at(source));
        if (!g_parsedSources.count(source)) {
            try {
                g_parsedSources.emplace(source, parseScenarioSourceText(source));
            } catch (const std::exception &error) {
                g_sourceFailures.emplace(source, error.what());
                throw;
            }
        }
        const SourceUnit &parsed = g_parsedSources.at(source);
        SourceFunctionAst typed = parsed.functions.at(name);
        typed.offset = function.offset;
        std::vector<SourceCDeclaration> declarations = parsed.cDeclarations;
        std::swap(unit.cDeclarations, declarations);
        std::swap(function, typed);
        std::vector<uint8_t> bytes;
        try {
            bytes = compileExplicitFunction(unit, function);
        } catch (...) {
            std::swap(function, typed);
            std::swap(unit.cDeclarations, declarations);
            throw;
        }
        std::swap(function, typed);
        std::swap(unit.cDeclarations, declarations);
        const std::vector<uint8_t> expected = compileExplicitFunction(recovery.original, recovery.original.functions.at(name));
        if (bytes != expected) {
            size_t first = 0;
            while (first < bytes.size() && first < expected.size() && bytes[first] == expected[first])
                ++first;
            std::ostringstream bytesText;
            const size_t from = first > 6 ? first - 6 : 0;
            bytesText << "First differing function byte " << hexUnsigned(static_cast<uint32_t>(first)) << "; C";
            for (size_t index = from; index < bytes.size() && index < first + 10; ++index)
                bytesText << (index == first ? " [" : " ") << hexName(bytes[index], 2);
            bytesText << " / native";
            for (size_t index = from; index < expected.size() && index < first + 10; ++index)
                bytesText << (index == first ? " [" : " ") << hexName(expected[index], 2);

            const auto decode = [](const std::vector<uint8_t> &code) {
                std::vector<Instruction> instructions;
                for (uint32_t offset = 0; offset < code.size();) {
                    instructions.push_back(decodeInstruction(code, offset, 0, static_cast<uint32_t>(code.size())));
                    offset += std::max<uint32_t>(instructions.back().size, 1U);
                }
                return instructions;
            };
            const std::vector<Instruction> cCode = decode(bytes), nativeCode = decode(expected);
            const auto same = [](const Instruction &left, const Instruction &right) {
                return left.opcode == right.opcode && left.size == right.size && (left.hasRelativeTarget || left.hasLocalTarget || left.imm == right.imm);
            };
            size_t instruction = 0;
            while (instruction < cCode.size() && instruction < nativeCode.size() && same(cCode[instruction], nativeCode[instruction]))
                ++instruction;
            const auto window = [&](const std::vector<Instruction> &code, const std::vector<uint8_t> &blob) {
                for (size_t index = instruction > 3 ? instruction - 3 : 0; index < code.size() && index < instruction + 4; ++index) {
                    bytesText << (index == instruction ? " [" : " ");
                    for (uint32_t byte = 0; byte < code[index].size; ++byte)
                        bytesText << hexName(blob[code[index].offset + byte], 2);
                }
            };
            bytesText << "; first differing instruction; C";
            window(cCode, bytes);
            bytesText << " / native";
            window(nativeCode, expected);
            result["issues"].push_back(issue(1, "instruction_selection_unresolved", bytesText.str()));
            result["c_size"] = bytes.size();
            result["original_size"] = expected.size();
            if (includeSource)
                result["source"] = source;
            return result;
        }
        result["verified_c"] = true;
        if (recovery.layouts.count(name))
            result["issues"].push_back(issue(4, "frame_layout", "C body is exact; accessed objects do not determine native allocation"));
        if (includeSource)
            result["source"] = source;
    } catch (const std::exception &error) {
        const std::string message = error.what();
        const bool prototypeIssue = message.find("typed native C parameter") != std::string::npos || message.find("undeclared function") != std::string::npos;
        const bool lowering = message.find("no VM lowering") != std::string::npos || message.find("Unsupported operation: ") != std::string::npos ||
            message.find("aggregate local initialization") != std::string::npos;
        result["issues"].push_back(issue(lowering && !prototypeIssue ? 3 : 1,
            prototypeIssue ? "native_prototype" :
                lowering   ? "compiler_lowering" :
                             "c_reconstruction",
            message.substr(0, 2048)));
        if (includeSource && !source.empty())
            result["source"] = source;
    }
    return result;
}

std::string moduleSource(const CRecovery &recovery, const Scn3Module &original) {
    const SourceUnit &unit = recovery.unit;
    const std::string entry = unit.startupFunctionSymbol.value_or("");
    std::ostringstream out;
    out << "#include \"scenario.h\"\n\n";
    std::set<uint32_t> knownCharacters;
    for (uint32_t character : shenmue::s1::kValidScenCharacterNames)
        knownCharacters.insert(character);
    std::map<std::string, uint32_t> characters;
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst function = unit.functions.at(name);
        visitExpressions(function.body.body, [&](SourceExpr &expression) {
            if (expression.kind != SourceExprKind::Identifier || !expression.text.starts_with("CHARA_") || expression.text.size() != 10)
                return;
            const std::optional<uint32_t> value = sourceDataConstantValue(expression);
            if (value && !knownCharacters.count(*value))
                characters[expression.text] = *value;
        });
    }
    for (const std::pair<const std::string, uint32_t> &character : characters)
        out << "enum { " << character.first << " = " << hexUnsigned(character.second) << " };\n";
    std::map<uint32_t, SourceFunctionAst> prototypes;
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst prototype = unit.functions.at(name);
        bool typed = !prototype.returnType.empty();
        for (const std::string &parameter : prototype.params)
            typed &= prototype.localTypes.count(parameter) != 0;
        if (!typed) {
            prototype.returnType = "int32_t";
            prototype.variadic = false;
            prototype.fixedParameterCount.reset();
            for (const std::string &parameter : prototype.params)
                prototype.localTypes[parameter] = "int32_t";
        }
        prototype.externalDefinition = name == entry;
        cPrototype(out, prototype);
        out << ";\n";
        prototypes.emplace(recovery.original.functions.at(name).offset, std::move(prototype));
    }
    std::map<uint32_t, std::pair<std::string, uint32_t>> workspace;
    for (const std::pair<const std::string, SourceDataBinding> &binding : unit.workspaceBindings) {
        const size_t columns = binding.second.type.find('[');
        const std::string element = binding.second.type.substr(0, columns);
        const ScenarioDataSchema *schema = findScenarioDataSchema(element);
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(element);
        if (!schema && !dataType)
            throw std::runtime_error("Workspace object has no C type: " + binding.first);
        const std::map<std::string, uint32_t>::const_iterator offset = unit.workspaceOffsets.find(binding.first);
        if (offset == unit.workspaceOffsets.end())
            throw std::runtime_error("Workspace object has no established offset: " + binding.first);
        const uint32_t row = columns == std::string::npos ? 1U : static_cast<uint32_t>(std::stoul(binding.second.type.substr(columns + 1)));
        workspace[offset->second] = {
            binding.first, (schema ? schema->size : scenarioDataTypeSize(*dataType)) * row * (binding.second.isArray ? binding.second.elementCount : 1U)};
    }
    const uint32_t workspaceSize = static_cast<uint32_t>(recovery.original.header.workspaceSize);
    out << "\n";
    for (uint32_t offset = 0; offset < workspaceSize;) {
        const std::map<uint32_t, std::pair<std::string, uint32_t>>::const_iterator object = workspace.find(offset);
        if (object != workspace.end()) {
            const SourceDataBinding &binding = unit.workspaceBindings.at(object->second.first);
            const size_t columns = binding.type.find('[');
            if (binding.pointer)
                out << "static " << binding.pointerType << (binding.pointerType.ends_with("*") ? "" : " ") << object->second.first;
            else
                out << "static " << binding.type.substr(0, columns) << " " << object->second.first;
            if (binding.isArray)
                out << "[" << binding.elementCount << "]";
            if (columns != std::string::npos)
                out << binding.type.substr(columns);
            out << ";\n";
            offset += object->second.second;
            continue;
        }
        const std::map<uint32_t, std::pair<std::string, uint32_t>>::const_iterator next = workspace.upper_bound(offset);
        const uint32_t gap = (next == workspace.end() ? workspaceSize : next->first) - offset;
        const uint32_t size = offset % 4 == 0 && gap >= 4 ? 4 : offset % 2 == 0 && gap >= 2 ? 2 : 1;
        out << "static " << (size == 4 ? "int32_t" : size == 2 ? "uint16_t" : "uint8_t") << " task_" << hexName(offset) << ";\n";
        offset += size;
    }
    for (const std::pair<const uint32_t, std::pair<std::string, uint32_t>> &object : workspace)
        if (object.first >= workspaceSize)
            throw std::runtime_error("Workspace object lies beyond the module workspace: " + object.second.first);
    const std::map<std::string, SourceExpr> strings = cStringLiterals(recovery, original);
    printCModuleData(out, unit, original, moduleDataEnd(original), recovery.readOnlyData, prototypes, strings);
    for (const std::string &name : unit.functionOrder) {
        SourceFunctionAst function = unit.functions.at(name);
        function.externalDefinition = name == entry;
        inlineCStringArguments(function, strings);
        out << "\n";
        functionDefinition(out, function);
    }
    return out.str();
}

CRecovery recoverPlacedC(const Program &program, const std::string &profile) {
    CRecovery recovery = recoverC(program, profile, {});
    return recovery.layouts.empty() ? recovery : recoverC(program, profile, recovery.layouts);
}
} // namespace

std::string renderScenarioSource(const Program &program, const std::string &profile) {
    CRecovery recovery = recoverPlacedC(program, profile);
    if (!recovery.moduleIssues.empty())
        throw std::runtime_error("Cannot export C module: " + recovery.moduleIssues.dump());
    Scn3Module original = buildScn3Module(program);
    attachUnreferencedReturns(original);
    std::string unresolved;
    for (const std::string &name : recovery.unit.functionOrder)
        if (!functionIssues(recovery, name)["issues"].empty() || recovery.layouts.count(name))
            unresolved += (unresolved.empty() ? "" : ", ") + name;
    if (!unresolved.empty())
        throw std::runtime_error("Cannot export functions as C: " + unresolved);
    if (!recovery.plain.empty())
        std::cerr << "note: " << recovery.plain.size() << " functions use raw words for overlapping stack objects\n";
    const std::string text = moduleSource(recovery, original);
    const SourceUnit parsed = parseScenarioSourceText(text);
    SourceUnit placed = parsed;
    for (const std::string &name : placed.functionOrder)
        placed.functions.at(name).offset = recovery.original.functions.at(name).offset;
    for (std::pair<const std::string, uint32_t> &symbol : placed.dataSymbols) {
        const std::map<std::string, uint32_t>::const_iterator native = recovery.original.dataSymbols.find(symbol.first);
        if (native != recovery.original.dataSymbols.end())
            symbol.second = native->second;
    }
    placed.cstrOffsetsByText = recovery.original.cstrOffsetsByText;
    for (const std::string &name : placed.functionOrder)
        if (compileExplicitFunction(placed, placed.functions.at(name)) != compileExplicitFunction(recovery.original, recovery.original.functions.at(name)))
            throw std::runtime_error("C function differs in module context: " + name);
    const std::vector<uint8_t> rebuilt = emitScn3Module(buildScn3ModuleFromSourceUnit(parsed));
    if (rebuilt != original.originalBlob) {
        size_t offset = 0;
        while (offset < rebuilt.size() && offset < original.originalBlob.size() && rebuilt[offset] == original.originalBlob[offset])
            ++offset;
        throw std::runtime_error("C source does not reproduce the module layout: first difference at " + hexUnsigned(offset) + "; original size " +
            std::to_string(original.originalBlob.size()) + ", rebuilt size " + std::to_string(rebuilt.size()));
    }
    return text;
}

std::string renderCRecoveryReport(const Program &program, const std::string &profile, const std::optional<std::string> &onlyFunction) {
    CRecovery recovery = recoverPlacedC(program, profile);
    Json report = {{"module_issues", recovery.moduleIssues}, {"functions", Json::array()}, {"verified_c", 0}, {"layout_only", 0}};
    for (const std::string &name : recovery.unit.functionOrder) {
        if (onlyFunction && name != *onlyFunction)
            continue;
        Json function = analyzeFunction(recovery, name, onlyFunction.has_value());
        function["shared_storage_words"] = recovery.plain.count(name) != 0;
        if (function["verified_c"].get<bool>()) {
            report["verified_c"] = report["verified_c"].get<unsigned>() + 1;
            if (function["layout_metadata"].get<bool>())
                report["layout_only"] = report["layout_only"].get<unsigned>() + 1;
        }
        report["functions"].push_back(std::move(function));
    }
    if (onlyFunction && report["functions"].empty())
        throw std::runtime_error("Unknown function: " + *onlyFunction);
    return report.dump(2);
}

std::string renderRecoveredCFunction(const Program &program, const std::string &profile, const std::string &name, bool strict) {
    CRecovery recovery = recoverPlacedC(program, profile);
    if (!recovery.unit.functions.count(name))
        throw std::runtime_error("Unknown function: " + name);
    const Json function = analyzeFunction(recovery, name, true);
    if (!function.at("verified_c").get<bool>())
        throw std::runtime_error("Cannot export C function: " + function.at("issues").dump());
    if (strict && function.at("layout_metadata").get<bool>())
        throw std::runtime_error("C export requires frame layout annotations");
    inlineCStringArguments(recovery.unit.functions.at(name), cStringLiterals(recovery, buildScn3Module(program)));
    return functionSource(recovery, name);
}
} // namespace shenmue::scenario
