#include "scn3/backend/compiler.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/decompiler/cfg.hpp"
#include "scn3/decompiler/data.hpp"
#include "scn3/decompiler/export.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/constants.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/motion.hpp"

namespace shenmue::scenario {

using namespace decompile;

std::string renderExplicitSource(const Program &program, const std::string &profile, const std::set<std::string> &plain, SourceUnit *parsed) {
    Scn3Module module = buildScn3Module(program);
    attachUnreferencedReturns(module);
    const uint32_t dataStart = module.parsedHeader.dataSegmentOffset;
    const uint32_t dataEnd = moduleDataEnd(module);
    const uint32_t fileEnd = static_cast<uint32_t>(module.originalBlob.size());
    std::set<uint32_t> references = collectSourceDataReferenceOffsets(module);

    for (uint32_t offset = module.parsedHeader.relocationTableOffset; offset + 8U <= module.originalBlob.size(); offset += 8U) {
        const uint32_t kind = readU32(module.originalBlob, offset), site = readU32(module.originalBlob, offset + 4U);
        if (!kind && !site)
            break;
        if (site >= dataStart && site + 4U <= dataEnd) {
            references.insert(site);
            if (site + 4U < dataEnd)
                references.insert(site + 4U);

            const uint32_t target = readU32(module.originalBlob, site);
            if (kind == 0x1402U && target >= dataStart && target < dataEnd)
                references.insert(target);
        }
    }
    const std::vector<ScenarioDataView> views = profile == "s1-pc" ? discoverScenarioDataViews(program, module, references) : std::vector<ScenarioDataView>{};
    const std::vector<SourceDataDecl> declarations = planSourceDataDecls(module.originalBlob, dataStart, dataEnd, references, views);
    const std::vector<SourceDataDecl> trailing = planSourceDataDecls(module.originalBlob, dataEnd, fileEnd, {});
    const std::map<uint32_t, std::string> symbols = sourceDataSymbolMap(declarations);
    std::set<uint32_t> instructionOffsets;
    for (const LowLevelFunction &function : module.functions)
        for (const LowLevelInsn &insn : function.instructions)
            instructionOffsets.insert(insn.offset);
    std::set<uint32_t> labels;
    for (const LowLevelFunction &function : module.functions) {
        labels.insert(function.start);
        for (const LowLevelInsn &insn : function.instructions) {
            if (insn.relativeTarget && instructionOffsets.count(*insn.relativeTarget))
                labels.insert(*insn.relativeTarget);
            if (insn.localTarget && instructionOffsets.count(*insn.localTarget))
                labels.insert(*insn.localTarget);
            if (insn.cls == InstrClass::Addr && addressOperation(insn.opcode) >= Address::Relative1 && addressOperation(insn.opcode) <= Address::Relative3 &&
                insn.imm) {
                const uint32_t target = function.baseOffset + *insn.imm;
                if (instructionOffsets.count(target))
                    labels.insert(target);
            }
        }
    }
    SourceUnit verification{};
    verification.abiProfile = profile == "s1-pc" ? "s1_pc" : "raw";
    verification.codeLabels.clear();
    for (const LowLevelFunction &function : module.functions) {
        SourceFunctionAst entry{};
        entry.name = scriptFunctionName(function.start);
        entry.offset = function.start;
        for (const LowLevelInsn &insn : function.instructions) {
            if (insn.cls != InstrClass::Addr || addressBase(addressOperation(insn.opcode)) != AddrBase::PrevSp || !insn.imm || !insn.immWidth)
                continue;
            const int32_t offset = insn.immWidth == 1 ? static_cast<int8_t>(*insn.imm) :
                insn.immWidth == 2                    ? static_cast<int16_t>(*insn.imm) :
                                                        static_cast<int32_t>(*insn.imm);
            if (offset >= 8) {
                entry.params.push_back("arg0");
                break;
            }
        }
        verification.functions.emplace(entry.name, std::move(entry));
    }
    for (uint32_t offset : labels)
        verification.codeLabels.emplace("L" + hexName(offset, 6), offset);
    for (const std::pair<const uint32_t, std::string> &symbol : symbols)
        verification.dataSymbols.emplace(symbol.second, symbol.first);
    std::ostringstream out;
    out << "scn3_source 2;\n\n#include \"scenario.h\"\n\nabi " << verification.abiProfile << ";\n\n";
    std::ostringstream header;
    writeScenarioHeader(header, module);
    std::string headerText = header.str();
    const std::string numericStartup = "startupFunctionOffset = " + hexUnsigned(module.parsedHeader.startupFunctionOffset);
    const size_t startupAt = headerText.find(numericStartup);
    if (startupAt != std::string::npos && verification.functions.count(scriptFunctionName(module.parsedHeader.startupFunctionOffset)))
        headerText.replace(startupAt, numericStartup.size(), "startupFunctionOffset = " + scriptFunctionName(module.parsedHeader.startupFunctionOffset));
    out << headerText;

    std::vector<std::string> functionTexts(module.functions.size());
    std::vector<std::map<std::string, std::string>> frameMembers(module.functions.size());
    std::vector<std::exception_ptr> failures(module.functions.size());
    parallelFor(module.functions.size(), [&](size_t index) {
        try {
            const LowLevelFunction &function = module.functions[index];
            std::ostringstream functionText;
            SourceFunctionAst lifted{};
            lifted.name = scriptFunctionName(function.start);
            lifted.offset = function.start;
            lifted.explicitBase = function.baseOffset;
            std::vector<SourceStmt> &body = lifted.body.body;
            body = liftFunction(function, labels, symbols, profile, verification);
            functionReferences(body, verification);
            friendlyStatements(body);
            const std::string entry = "L" + hexName(function.start, 6);
            bool entered = false;
            visitExpressions(body, [&](SourceExpr &expression) { entered |= expression.kind == SourceExprKind::Identifier && expression.text == entry; });
            if (!entered && !body.empty() && body.front().kind == SourceStmtKind::Label && body.front().label == entry)
                body.erase(body.begin());
            if (!body.empty() && !function.instructions.empty()) {
                const LowLevelInsn &prologue = function.instructions.front();
                if (body.front().kind == SourceStmtKind::ExprStmt && body.front().expr.text == "vm_adjust_sp" && prologue.imm &&
                    (prologue.opcode == encode(Control::AdjustStack, 1) || (prologue.opcode == encode(Control::AdjustStack, 2) && *prologue.imm > 0xFFU))) {
                    lifted.explicitFrameAdjust = *prologue.imm;
                    body.erase(body.begin());
                }
            }
            ControlFlowGraph graph(std::move(body));
            splitFrameVariables(graph);
            if (verification.abiProfile == "s1_pc" && !plain.count(lifted.name))
                lifted.frameMembers = splitFrameRecords(graph);
            body = structureFunction(std::move(graph));
            placeFrameVariables(lifted);
            if (verification.abiProfile == "s1_pc")
                typedFrameStores(body, lifted, !plain.count(lifted.name));
            std::vector<uint8_t> expected;
            for (const LowLevelInsn &insn : function.instructions)
                expected.insert(expected.end(), insn.originalBytes.begin(), insn.originalBytes.end());
            if (compileExplicitFunction(verification, lifted) != expected)
                throw std::runtime_error("Decompiled source changes the instructions of " + lifted.name);
            uint32_t parameterCount = 0;
            collectParameters(lifted.body.body, parameterCount);
            for (uint32_t i = 0; i < parameterCount; ++i)
                lifted.params.push_back("arg" + std::to_string(i));
            functionText << "\nfunction " << lifted.name << "(";
            for (uint32_t i = 0; i < parameterCount; ++i)
                functionText << (i ? ", " : "") << "arg" << i;
            if (function.baseOffset != 0)
                throw std::runtime_error("Source export requires an established module-relative code base");
            functionText << ")";
            if (lifted.explicitFrameAdjust)
                functionText << " frame(" << hexUnsigned(*lifted.explicitFrameAdjust) << ")";
            functionText << " {\n";
            for (const std::pair<const std::string, std::string> &binding : lifted.localTypes)
                if (!argSlotOffsetFromName(binding.first))
                    functionText << "  " << binding.second << " " << binding.first << ";\n";
            for (const SourceStmt &stmt : lifted.body.body)
                printStatement(functionText, stmt);
            functionText << "}\n";
            functionTexts[index] = functionText.str();
            frameMembers[index] = lifted.frameMembers;
        } catch (...) {
            failures[index] = std::current_exception();
        }
    });
    for (size_t index = 0; index < functionTexts.size(); ++index) {
        if (failures[index])
            std::rethrow_exception(failures[index]);
        out << functionTexts[index];
    }
    for (const ByteRange &range : module.rawRanges) {
        if (range.kind != RangeKind::Header && range.kind != RangeKind::DataSegment && range.kind != RangeKind::TrailingDataOrUnknown)
            renderSourceRawRange(out, module, range);
    }
    out << "\ndata_segment " << hexUnsigned(dataStart, 6) << " " << hexUnsigned(dataEnd, 6) << " {\n";
    renderSourceDataDecls(out, module.originalBlob, declarations, "  ");
    out << "}\n\n"
        << "module_data " << hexUnsigned(dataEnd, 6) << " " << hexUnsigned(fileEnd, 6) << " {\n";
    renderSourceDataDecls(out, module.originalBlob, trailing, "  ");
    out << "}\n";
    const std::string text = out.str();

    Scn3Module rebuilt;
    SourceUnit unit;
    try {
        unit = parseScenarioSourceText(text);
        rebuilt = buildScn3ModuleFromSourceUnit(unit);
    } catch (const std::runtime_error &error) {
        const std::string message = error.what();
        const size_t at = message.find(" at line ");
        if (at != std::string::npos) {
            const size_t wanted = std::stoul(message.substr(at + 9));
            std::istringstream lines(text);
            std::string line;
            for (size_t index = 1; std::getline(lines, line); ++index)
                if (index == wanted)
                    throw std::runtime_error(message + ": " + line);
        }
        throw;
    }
    if (emitScn3Module(rebuilt) != module.originalBlob)
        throw std::runtime_error("Explicit source export failed its independent recompile check");
    if (parsed) {

        for (size_t index = 0; index < module.functions.size(); ++index)
            unit.functions.at(scriptFunctionName(module.functions[index].start)).frameMembers = frameMembers[index];
        *parsed = std::move(unit);
    }
    return text;
}

void decompile::printCModuleData(std::ostream &out, const SourceUnit &unit, const Scn3Module &original, uint32_t dataEnd,
    const std::map<std::string, bool> &readOnlyData, const std::map<uint32_t, SourceFunctionAst> &functions,
    const std::map<std::string, SourceExpr> &stringLiterals) {
    const std::vector<uint8_t> &blob = original.originalBlob;
    std::vector<std::pair<uint32_t, std::string>> data;
    for (const std::pair<const std::string, uint32_t> &symbol : unit.dataSymbols)
        if (symbol.second >= original.parsedHeader.dataSegmentOffset && symbol.second < dataEnd)
            data.emplace_back(symbol.second, symbol.first);
    for (size_t index = 1; index < data.size(); ++index) {
        size_t position = index;
        while (position && data[position] < data[position - 1]) {
            std::swap(data[position], data[position - 1]);
            --position;
        }
    }
    std::map<uint32_t, uint32_t> relocations;
    for (uint32_t offset = original.parsedHeader.relocationTableOffset; offset + 8U <= blob.size(); offset += 8U) {
        const uint32_t kind = readU32(blob, offset), site = readU32(blob, offset + 4U);
        if (!kind && !site)
            break;
        if ((kind != 0x2402U && kind != 0x1402U) || site < original.parsedHeader.dataSegmentOffset || site + 4U > dataEnd)
            throw std::runtime_error("Relocation record has no C pointer representation at " + hexUnsigned(site));
        if (!relocations.emplace(site, kind).second)
            throw std::runtime_error("Duplicate relocation record at " + hexUnsigned(site));
    }
    const std::function<std::string(const SourceDataBinding &)> elementType = [](const SourceDataBinding &binding) -> std::string {
        if (binding.type == "char" || binding.type == "cstr")
            return "char";
        if (binding.type == "u32")
            return "uint32_t";
        if (binding.type == "bytes" || binding.type == "pad")
            return "uint8_t";
        return binding.type;
    };
    const std::function<bool(const SourceDataBinding &)> decays = [](const SourceDataBinding &binding) {
        return binding.isArray || binding.type == "char" || binding.type == "cstr" || binding.type == "bytes";
    };

    const std::function<std::pair<size_t, std::string>(uint32_t)> dataTarget = [&](uint32_t target) -> std::pair<size_t, std::string> {
        for (size_t index = 0; index < data.size(); ++index) {
            const uint32_t start = data[index].first;
            const uint32_t end = index + 1 < data.size() ? data[index + 1].first : dataEnd;
            if (target < start || target >= end)
                continue;
            const SourceDataBinding &binding = unit.dataBindings.at(data[index].second);
            if (target == start)
                return {index, (decays(binding) ? "" : "&") + data[index].second};
            const ScenarioDataSchema *schema = findScenarioDataSchema(binding.type);
            const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(binding.type);
            const uint32_t stride = schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 1;
            if (decays(binding) && (target - start) % stride == 0)
                return {index, "&" + data[index].second + "[" + std::to_string((target - start) / stride) + "]"};
            break;
        }
        throw std::runtime_error("Data relocation target is not the start of a C object or element: " + hexUnsigned(target));
    };
    std::set<std::string> writableTargets;
    for (const std::pair<const uint32_t, uint32_t> &relocation : relocations)
        if (relocation.second == 0x1402U)
            writableTargets.insert(data[dataTarget(readU32(blob, relocation.first)).first].second);
    std::set<uint32_t> printedPointers;
    const std::function<std::string(uint32_t)> pointerText = [&](uint32_t site) -> std::string {
        const uint32_t value = readU32(blob, site);
        const std::map<uint32_t, uint32_t>::const_iterator relocation = relocations.find(site);
        if (relocation == relocations.end()) {
            if (value)
                throw std::runtime_error("Nonzero C pointer field has no relocation record at " + hexUnsigned(site));
            return "0";
        }
        printedPointers.insert(site);
        if (relocation->second == 0x1402U)
            return dataTarget(value).second;
        const std::map<uint32_t, SourceFunctionAst>::const_iterator function = functions.find(value);
        if (function == functions.end())
            throw std::runtime_error("Code relocation target is not a function start: " + hexUnsigned(value));
        return function->second.name;
    };

    const auto list = [&](const std::vector<std::string> &items, bool lines) {
        size_t length = 0;
        for (const std::string &item : items)
            length += item.size() + 2;
        if (!lines && length < 120) {
            out << "{";
            for (size_t item = 0; item < items.size(); ++item)
                out << (item ? ", " : "") << items[item];
            out << "}";
            return;
        }
        out << "{\n ";
        size_t column = 1;
        for (size_t item = 0; item < items.size(); ++item) {
            const std::string text = items[item] + (item + 1 < items.size() ? "," : "");
            if (lines || column + text.size() + 1 > 150) {
                if (item)
                    out << "\n ";
                column = 1;
            }
            out << " " << text;
            column += text.size() + 1;
        }
        out << "\n}";
    };
    for (size_t index = 0; index < data.size(); ++index) {
        const uint32_t start = data[index].first;
        const uint32_t end = index + 1 < data.size() ? data[index + 1].first : dataEnd;
        const SourceDataBinding &binding = unit.dataBindings.at(data[index].second);
        const ScenarioDataSchema *schema = findScenarioDataSchema(binding.type);
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(binding.type);
        const std::map<std::string, bool>::const_iterator readonly = readOnlyData.find(data[index].second);
        const bool constant = (readonly == readOnlyData.end() || readonly->second) && !writableTargets.count(data[index].second);
        const std::map<uint32_t, uint32_t>::const_iterator pointer = relocations.lower_bound(start);
        if (!schema && pointer != relocations.end() && pointer->first < end) {
            if (pointer->first != start || end - start != 4 || binding.isArray)
                throw std::runtime_error("C data object mixes pointer and non-pointer words: " + data[index].second);
            const std::string declarator = std::string(constant ? "const " : "") + data[index].second;
            const std::string target = pointerText(start);
            if (pointer->second == 0x2402U)
                out << "static " << cFunctionPointerType(functions.at(readU32(blob, start)), declarator) << " = " << target << ";\n";
            else
                out << "static " << elementType(unit.dataBindings.at(data[dataTarget(readU32(blob, start)).first].second)) << " *" << declarator << " = "
                    << target << ";\n";
            continue;
        }

        if (binding.type.starts_with("row:")) {
            const std::string columns = binding.type.substr(4);
            const std::string record = "Row_" + hexName(start, start < 0x10000U ? 4 : 0);
            out << "typedef struct " << record << " {";
            for (size_t column = 0; column < columns.size(); ++column)
                out << " "
                    << (columns[column] == '2'        ? "float" :
                               columns[column] == '3' ? "shen_name" :
                               columns[column] == '4' ? "uint32_t" :
                                                        "int32_t")
                    << " w" << column << ";";
            out << " } " << record << ";\n";
            out << "static " << (constant ? "const " : "") << record << " " << data[index].second << "[" << binding.elementCount << "] = ";

            std::set<uint32_t> characters;
            for (uint32_t character : shenmue::s1::kValidScenCharacterNames)
                characters.insert(character);
            std::vector<std::string> rows;
            for (uint32_t row = 0; row < binding.elementCount; ++row) {
                std::ostringstream text;
                text << "{";
                for (size_t column = 0; column < columns.size(); ++column) {
                    const uint32_t offset = start + (row * static_cast<uint32_t>(columns.size()) + static_cast<uint32_t>(column)) * 4U;
                    const uint32_t value = readU32(original.originalBlob, offset);
                    if (column)
                        text << ", ";
                    if (columns[column] == '2')
                        cDataValue(text, original.originalBlob, offset, ScenarioDataType::F32);
                    else if (columns[column] == '3' && value) {
                        std::string letters;
                        for (uint32_t byte = 0; byte < 4; ++byte)
                            letters += static_cast<char>(value >> (byte * 8U));
                        text << (characters.count(value) ? "CHARA_" + letters : "SHEN_NAME('" + letters + "')");
                    } else if (columns[column] == '4')
                        text << hexUnsigned(value) << "U";
                    else
                        text << static_cast<int32_t>(value);
                }
                text << "}";
                rows.push_back(text.str());
            }
            list(rows, true);
            out << ";\n";
            continue;
        }
        out << "static " << (constant ? "const " : "");
        if (binding.type == "shen_name") {

            std::set<uint32_t> characters;
            for (uint32_t character : shenmue::s1::kValidScenCharacterNames)
                characters.insert(character);
            out << "shen_name " << data[index].second;
            if (binding.isArray)
                out << "[" << binding.elementCount << "]";
            out << " = ";
            std::vector<std::string> names;
            for (uint32_t element = 0; element < binding.elementCount; ++element) {
                const uint32_t value = readU32(original.originalBlob, start + element * 4U);
                std::string letters;
                for (uint32_t byte = 0; byte < 4; ++byte)
                    letters += static_cast<char>(value >> (byte * 8U));
                names.push_back(!value ? std::string("0") : characters.count(value) ? "CHARA_" + letters : "SHEN_NAME('" + letters + "')");
            }
            if (binding.isArray)
                list(names, false);
            else
                out << names.front();
        } else if (binding.type == "char") {
            out << "char " << data[index].second << "[" << end - start << "]";
            if (stringLiterals.count(data[index].second)) {
                if (!constant)
                    throw std::runtime_error("A writable object cannot back a string literal");
                out << " __attribute__((scn_string_literal))";
            }
            out << " = " << cCharacterLiteral(original.originalBlob, start, end);
        } else if (schema || (dataType && binding.type != "u32" && binding.type != "bytes")) {
            const uint32_t stride = schema ? schema->size : scenarioDataTypeSize(*dataType);
            if (end - start != binding.elementCount * stride)
                throw std::runtime_error("C initialized object extent does not match its declaration");
            out << binding.type << " " << data[index].second;
            if (binding.isArray)
                out << "[" << binding.elementCount << "]";
            out << " = ";
            std::vector<std::string> elements;
            for (uint32_t element = 0; element < binding.elementCount; ++element) {
                std::ostringstream text;
                if (schema)
                    cDataRecord(text, original.originalBlob, start + element * stride, *schema, pointerText);
                else
                    cDataValue(text, original.originalBlob, start + element * stride, *dataType);
                elements.push_back(text.str());
            }
            if (binding.isArray)
                list(elements, schema != nullptr);
            else
                out << elements.front();
        } else if (binding.type == "cstr") {
            out << "char " << data[index].second << "[]";
            if (stringLiterals.count(data[index].second)) {
                if (!constant)
                    throw std::runtime_error("A writable object cannot back a string literal");
                out << " __attribute__((scn_string_literal))";
            }
            out << " = " << sourceEscapeString(original.originalBlob, start, end - start - 1U);
        } else if (binding.type == "u32") {

            const uint32_t value = readU32(original.originalBlob, start);
            std::string letters;
            bool name = true, letter = false;
            for (uint32_t byte = 0; byte < 4; ++byte) {
                const char character = static_cast<char>(value >> (byte * 8U));
                letter |= character >= 'A' && character <= 'Z';
                name &= (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_';
                letters += character;
            }
            out << "uint32_t " << data[index].second << " = "
                << (name && letter           ? "SHEN_NAME('" + letters + "')" :
                           value < 0x100000U ? std::to_string(value) :
                                               hexUnsigned(value));
        } else {
            out << "uint8_t " << data[index].second << "[" << end - start << "] = ";
            bool zero = true;
            for (uint32_t byte = start; byte < end; ++byte)
                zero &= !original.originalBlob[byte];
            if (zero)
                out << "{0}";
            else
                writeBytesArray(out, original.originalBlob, start, end, "");
        }
        out << ";\n";
    }
    for (const std::pair<const uint32_t, uint32_t> &relocation : relocations)
        if (!printedPointers.count(relocation.first))
            throw std::runtime_error("Relocation record has no C pointer declaration at " + hexUnsigned(relocation.first));
}

void exportExplicitSource(const Program &program, const std::string &path, const std::string &profile, const std::optional<std::string> &names) {
    const std::filesystem::path output = std::filesystem::absolute(path), header = output.parent_path() / (output.stem().string() + ".h");
    if (header.filename() == "scenario.h")
        throw std::runtime_error("A script's header cannot be named scenario.h: " + path);
    if (std::filesystem::exists(output) && std::filesystem::exists(header))
        return;
    if (std::filesystem::exists(output))
        throw std::runtime_error("Script source exists without its header: " + path);
    ScenarioSource source = renderScenarioSource(program, profile);
    if (names) {
        const std::map<std::string, std::string> symbols = readSymbolNames(*names);
        source.declarations = renameSourceSymbols(source.declarations, symbols);
        source.definitions = renameSourceSymbols(source.definitions, symbols);
    }
    const std::string include = "#include \"scenario.h\"\n", includes = usedMotionIncludes(source.declarations + source.definitions);
    const size_t at = source.declarations.find(include);
    if (at == std::string::npos)
        throw std::runtime_error("Exported source does not include scenario.h");
    source.declarations.replace(at, include.size(), include + includes);
    const std::string text = "#include \"" + header.filename().string() + "\"\n" + source.definitions;
    if (!std::filesystem::exists(header)) {
        std::ofstream headerFile(header, std::ios::binary);
        if (!headerFile || !(headerFile << "#pragma once\n\n" << source.declarations))
            throw std::runtime_error("Failed to write script header: " + header.string());
    }
    setScenarioCIncludeDirectory(output.parent_path());
    if (emitScn3Module(buildScn3ModuleFromSourceUnit(parseScenarioSourceText(text))) != buildScn3Module(program).originalBlob)
        throw std::runtime_error("Exported source and header do not reproduce the module");
    if (!std::filesystem::exists(output)) {
        std::ofstream file(path);
        if (!file || !(file << text))
            throw std::runtime_error("Failed to write explicit source: " + path);
    }
}

std::map<std::string, std::string> readSymbolNames(const std::string &path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Cannot read names file: " + path);
    const std::function<bool(const std::string &)> identifier = [](const std::string &name) {
        if (name.empty() || std::isdigit(static_cast<unsigned char>(name[0])))
            return false;
        for (char character : name)
            if (!std::isalnum(static_cast<unsigned char>(character)) && character != '_')
                return false;
        return true;
    };
    std::map<std::string, std::string> names;
    std::set<std::string> targets;
    std::string line;
    while (std::getline(file, line)) {
        for (char &character : line)
            if (character == ',' || character == '=' || character == '\r')
                character = ' ';
        const size_t arrow = line.find("->");
        if (arrow != std::string::npos)
            line.replace(arrow, 2, "  ");
        std::istringstream fields(line);
        std::string from, to;
        if (!(fields >> from) || from.starts_with("#") || from.starts_with("//") || !(fields >> to))
            continue;
        if (!identifier(from) || !identifier(to))
            throw std::runtime_error("Names file entry is not a pair of C identifiers: " + line);
        if (from == to)
            continue;
        if (names.count(from))
            throw std::runtime_error("Names file repeats a symbol: " + line);

        if (!targets.insert(to).second) {
            const std::string unique = to + from.substr(from.rfind('_'));
            std::cerr << "note: " << to << " names several symbols; " << from << " becomes " << unique << "\n";
            to = unique;
            if (!targets.insert(to).second)
                throw std::runtime_error("Names file repeats a symbol: " + line);
        }
        names.emplace(from, to);
    }
    return names;
}

std::string renameSourceSymbols(const std::string &text, const std::map<std::string, std::string> &names) {
    const std::function<bool(char)> identifierChar = [](char character) { return std::isalnum(static_cast<unsigned char>(character)) || character == '_'; };
    std::set<std::string> present;
    std::vector<std::pair<size_t, size_t>> identifiers;
    for (size_t position = 0; position < text.size();) {
        const char character = text[position];
        if (character == '/' && position + 1 < text.size() && text[position + 1] == '/') {
            while (position < text.size() && text[position] != '\n')
                ++position;
        } else if (character == '/' && position + 1 < text.size() && text[position + 1] == '*') {
            const size_t close = text.find("*/", position + 2);
            position = close == std::string::npos ? text.size() : close + 2;
        } else if (character == '"' || character == '\'') {
            for (++position; position < text.size() && text[position] != character; position += text[position] == '\\' ? 2 : 1) {
            }
            ++position;
        } else if (identifierChar(character) && !std::isdigit(static_cast<unsigned char>(character))) {
            const size_t begin = position;
            while (position < text.size() && identifierChar(text[position]))
                ++position;
            identifiers.emplace_back(begin, position - begin);
            present.insert(text.substr(begin, position - begin));
        } else if (std::isdigit(static_cast<unsigned char>(character))) {
            while (position < text.size() && (identifierChar(text[position]) || text[position] == '.'))
                ++position;
        } else
            ++position;
    }
    for (const std::pair<const std::string, std::string> &name : names)
        if (present.count(name.first) && present.count(name.second))
            throw std::runtime_error("Name already in use: " + name.second);
    std::string result;
    size_t copied = 0;
    for (const std::pair<size_t, size_t> &identifier : identifiers) {
        const std::map<std::string, std::string>::const_iterator name = names.find(text.substr(identifier.first, identifier.second));
        if (name == names.end())
            continue;
        result += text.substr(copied, identifier.first - copied);
        result += name->second;
        copied = identifier.first + identifier.second;
    }
    return result + text.substr(copied);
}

} // namespace shenmue::scenario
