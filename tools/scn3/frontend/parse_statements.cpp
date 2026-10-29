#include "scn3/bytecode/program.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

SourceStmt SourceParser::parseBlockStatement() {
    SourceStmt block{};
    block.kind = SourceStmtKind::Block;
    block.line = current_.line;
    block.column = current_.column;
    expectPunct("{");
    while (!matchPunct("}")) {
        block.body.push_back(parseStatement());
    }
    return block;
}

SourceStmt SourceParser::parseStatement() {
    SourceStmt stmt{};
    stmt.line = current_.line;
    stmt.column = current_.column;
    if (current_.kind == SourceTokenKind::Punct && current_.text == "{") {
        return parseBlockStatement();
    }
    if (matchIdentifier("if")) {
        stmt.kind = SourceStmtKind::If;
        expectPunct("(");
        stmt.expr = parseExpression();
        expectPunct(")");
        SourceStmt thenBlock = parseStatement();
        stmt.body = (thenBlock.kind == SourceStmtKind::Block) ? std::move(thenBlock.body) : std::vector<SourceStmt>{std::move(thenBlock)};
        if (matchIdentifier("else")) {
            SourceStmt elseBlock = parseStatement();
            stmt.elseBody = (elseBlock.kind == SourceStmtKind::Block) ? std::move(elseBlock.body) : std::vector<SourceStmt>{std::move(elseBlock)};
        }
        return stmt;
    }
    if (matchIdentifier("for")) {
        stmt.kind = SourceStmtKind::For;
        expectPunct("(");
        expectPunct(";");
        expectPunct(";");
        expectPunct(")");
        SourceStmt body = parseStatement();
        stmt.body = body.kind == SourceStmtKind::Block ? std::move(body.body) : std::vector<SourceStmt>{std::move(body)};
        return stmt;
    }
    if (matchIdentifier("while")) {
        stmt.kind = SourceStmtKind::While;
        expectPunct("(");
        stmt.expr = parseExpression();
        expectPunct(")");
        SourceStmt body = parseStatement();
        stmt.body = (body.kind == SourceStmtKind::Block) ? std::move(body.body) : std::vector<SourceStmt>{std::move(body)};
        return stmt;
    }
    if (matchIdentifier("switch")) {
        stmt.kind = SourceStmtKind::Switch;
        expectPunct("(");
        stmt.expr = parseExpression();
        expectPunct(")");
        expectPunct("{");
        while (!matchPunct("}")) {
            SourceSwitchCase caseNode{};
            caseNode.line = current_.line;
            caseNode.column = current_.column;
            if (matchIdentifier("case")) {
                caseNode.value = parseExpression();
                expectPunct(":");
            } else if (matchIdentifier("default")) {
                caseNode.isDefault = true;
                expectPunct(":");
            } else {
                fail("Expected case/default in switch");
            }
            while (!(current_.kind == SourceTokenKind::Identifier && (current_.text == "case" || current_.text == "default")) &&
                !(current_.kind == SourceTokenKind::Punct && current_.text == "}")) {
                caseNode.body.push_back(parseStatement());
            }
            stmt.cases.push_back(std::move(caseNode));
        }
        return stmt;
    }
    if (matchIdentifier("return")) {
        stmt.kind = SourceStmtKind::Return;
        if (!matchPunct(";")) {
            stmt.expr = parseExpression();
            expectPunct(";");
        }
        return stmt;
    }
    if (matchIdentifier("break")) {
        stmt.kind = SourceStmtKind::Break;
        expectPunct(";");
        return stmt;
    }
    if (matchIdentifier("continue")) {
        stmt.kind = SourceStmtKind::Continue;
        expectPunct(";");
        return stmt;
    }
    if (matchIdentifier("goto")) {
        stmt.kind = SourceStmtKind::Goto;
        stmt.label = expectIdentifier();
        expectPunct(";");
        return stmt;
    }
    if (matchIdentifier("tailgoto")) {
        stmt.kind = SourceStmtKind::TailGoto;
        stmt.expr = parseExpression();
        expectPunct(";");
        return stmt;
    }
    SourceExpr lhs = parseExpression();
    if (matchPunct(":")) {
        stmt.kind = SourceStmtKind::Label;
        stmt.label = lhs.text;
        return stmt;
    }
    if (matchPunct("=")) {
        stmt.kind = SourceStmtKind::Assign;
        stmt.expr = std::move(lhs);
        stmt.value = parseExpression();
        expectPunct(";");
        return stmt;
    }
    stmt.kind = SourceStmtKind::ExprStmt;
    stmt.expr = std::move(lhs);
    expectPunct(";");
    return stmt;
}

void SourceParser::beginOrdinaryC() {
    if (ordinaryC_)
        return;
    if (formatDeclared_ || sawHeader_)
        fail("Ordinary C cannot contain diagnostic container directives");
    ordinaryC_ = true;
    abiProfile_ = "s1_pc";
    header_.token = SHEN_NAME(SCN3);
    header_.flags = 0x100;
    header_.entrypoint = sizeof(ScenarioHeader);
    sawHeader_ = true;
    dataBlock_ = SourceRawBlock{.kind = RangeKind::DataSegment, .declaredStart = sizeof(ScenarioHeader), .bytes = {}};
    trailingBlock_ = SourceRawBlock{.kind = RangeKind::TrailingDataOrUnknown, .bytes = {}};
}

void SourceParser::parseCDeclaration(std::string type) {
    bool internal = false;
    bool external = false;
    while (type == "static" || type == "const" || type == "extern") {
        if (type == "static")
            internal = true;
        if (type == "extern")
            external = true;
        type = expectIdentifier();
    }
    bool pointer = matchPunct("*");
    bool functionPointer = false;
    std::string name;
    if (matchPunct("(")) {
        expectPunct("*");
        pointer = true;
        functionPointer = true;
        name = expectIdentifier();
    } else
        name = expectIdentifier();
    if (!pointer && current_.text == "(" && abiCommandForCallName(name)) {
        uint32_t depth = 0;
        do {
            if (current_.text == "(")
                ++depth;
            if (current_.text == ")")
                --depth;
            if (current_.kind == SourceTokenKind::End)
                fail("Unterminated native function declaration");
            advance();
        } while (depth != 0);
        expectPunct(";");
        return;
    }
    if (!pointer && current_.text == "(") {
        parseHighLevelFunctionBlock(SourceSemantics::C11, type, name, !internal);
        return;
    }
    const bool array = matchPunct("[");
    std::optional<uint32_t> extent;
    if (array) {
        if (current_.kind == SourceTokenKind::Number)
            extent = expectNumber();
        expectPunct("]");
    }
    if (functionPointer) {
        expectPunct(")");
        expectPunct("(");
        while (!matchPunct(")")) {
            expectIdentifier();
            if (current_.kind == SourceTokenKind::Identifier)
                advance();
            if (!matchPunct(",")) {
                expectPunct(")");
                break;
            }
        }
    }
    const ScenarioDataSchema *schema = findScenarioDataSchema(type);
    const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(type);
    const uint32_t elementSize = pointer ? 4U : schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 0U;
    const uint32_t alignment = pointer ? 4U : schema ? schema->alignment : elementSize;
    if (!elementSize || !alignment)
        fail("Unsupported C object type: " + type);
    if (external && matchPunct(";"))
        return;
    const bool initialized = matchPunct("=");
    SourceDataBinding binding{
        .type = pointer ? "uint32_t" : type, .elementCount = extent.value_or(1), .isArray = array, .pointer = pointer, .functionPointer = functionPointer};
    if (workspaceOffsets_.count(name) || dataBindings_.count(name))
        fail("Duplicate C object definition: " + name);
    if (!initialized) {
        if (pointer || schema || array)
            fail("Unsupported operation: Workspace aggregate and pointer lowering");
        if (array && !extent)
            fail("Workspace arrays require an extent");
        workspaceSize_ = (workspaceSize_ + alignment - 1U) / alignment * alignment;
        if (!workspaceOffsets_.emplace(name, workspaceSize_).second)
            fail("Duplicate workspace definition: " + name);
        workspaceBindings_[name] = binding;
        if (binding.elementCount > (0x7fffffffU - workspaceSize_) / elementSize)
            fail("Workspace object is too large");
        workspaceSize_ += binding.elementCount * elementSize;
    } else {
        std::vector<uint8_t> &bytes = dataBlock_->bytes;
        bytes.resize((bytes.size() + alignment - 1U) / alignment * alignment, 0);
        const uint32_t begin = static_cast<uint32_t>(bytes.size());
        if (!symbolOffsets_.emplace(name, sizeof(ScenarioHeader) + begin).second)
            fail("Duplicate initialized definition: " + name);
        activeBlockKind_ = RangeKind::DataSegment;
        activeBlockStart_ = sizeof(ScenarioHeader);
        if (array && type == "char" && current_.kind == SourceTokenKind::String) {
            const std::string text = expectString();
            bytes.insert(bytes.end(), text.begin(), text.end());
            bytes.push_back(0);
            binding.elementCount = static_cast<uint32_t>(text.size()) + 1U;
            cstrOffsetsByText_[text].push_back(symbolOffsets_[name]);
        } else {
            if (array)
                expectPunct("{");
            uint32_t count = 0;
            while (!array || !matchPunct("}")) {
                const uint32_t offset = static_cast<uint32_t>(bytes.size());
                bytes.resize(bytes.size() + elementSize, 0);
                if (schema && !pointer)
                    parseDataRecord(bytes, offset, *schema);
                else
                    parseDataValue(bytes, offset, pointer ? ScenarioDataType::ScriptOffset32 : *dataType);
                ++count;
                if (!array)
                    break;
                if (!matchPunct(",")) {
                    expectPunct("}");
                    break;
                }
            }
            if (extent && count > *extent)
                fail("Too many C array initializer elements");
            binding.elementCount = extent.value_or(count);
        }
        if (extent) {
            if (bytes.size() > begin + *extent * elementSize)
                fail("C string initializer exceeds array extent");
            bytes.resize(begin + *extent * elementSize, 0);
        }
        dataBindings_[name] = binding;
    }
    expectPunct(";");
}

void SourceParser::parseHighLevelFunctionBlock(
    SourceSemantics semantics, const std::string &returnType, const std::string &declaredName, bool externalDefinition) {
    const bool cFunction = semantics == SourceSemantics::C11;
    SourceFunctionAst function{};
    function.semantics = semantics;
    if (cFunction && returnType != "void" && returnType != "int32_t" && returnType != "uint32_t" && returnType != "float")
        fail("Unsupported C return type: " + returnType);
    function.returnType = returnType;
    function.externalDefinition = externalDefinition;
    function.line = current_.line;
    function.column = current_.column;
    function.name = declaredName.empty() ? expectIdentifier() : declaredName;
    expectPunct("(");
    if (!matchPunct(")")) {
        do {
            std::string param = expectIdentifier();
            const std::string type = param;
            if ((cFunction || !returnType.empty()) && param == "void" && current_.text == ")")
                break;
            if (cFunction && type != "uint32_t" && type != "int32_t" && type != "float")
                fail("Unsupported C parameter type");
            if (current_.kind == SourceTokenKind::Identifier && (cFunction || scenarioDataTypeByName(type) || current_.text.rfind("arg", 0) == 0))
                param = expectIdentifier();
            else if (cFunction)
                fail("C parameters require a declared name");
            if ((cFunction || param != type) && !function.localTypes.emplace(param, type).second)
                fail("Duplicate parameter name");
            if (!cFunction && param != type)
                function.localOffsets[param] = 8U + static_cast<uint32_t>(function.params.size()) * 4U;
            function.params.push_back(param);
        } while (matchPunct(","));
        expectPunct(")");
    }
    if (ordinaryC_ && matchPunct(";"))
        return;
    if (ordinaryC_ && externalDefinition) {
        if (startupFunctionSymbol_ && *startupFunctionSymbol_ != function.name)
            fail("A scenario requires a unique externally visible entry function definition");
        startupFunctionSymbol_ = function.name;
    }
    if (!cFunction && matchIdentifier("at")) {
        expectPunct("(");
        function.offset = expectNumber();
        function.declaredOffset = function.offset;
        expectPunct(")");
    }
    if (matchPunct("->")) {
        expectIdentifier();
    }
    if (!cFunction && matchIdentifier("frame")) {
        expectPunct("(");
        function.explicitFrameAdjust = expectNumber();
        expectPunct(")");
    }
    if (!cFunction && matchIdentifier("base")) {
        expectPunct("(");
        function.explicitBase = expectNumber();
        expectPunct(")");
    }
    parsingCFunction_ = cFunction;
    function.body.kind = SourceStmtKind::Block;
    expectPunct("{");
    while (current_.kind == SourceTokenKind::Identifier && (findScenarioDataSchema(current_.text) || scenarioDataTypeByName(current_.text))) {
        const std::string type = expectIdentifier();
        const std::string name = expectIdentifier();
        if (cFunction && !findScenarioDataSchema(type) && type != "uint32_t" && type != "int32_t" && type != "float")
            fail("C declarations require a supported standard type");
        const std::optional<SourceSlotRef> slot = localSlotRefFromName(name);
        if ((!cFunction && (!slot || slot->aggregateField || name.find('.') != std::string::npos)) || !function.localTypes.emplace(name, type).second)
            fail("Duplicate or invalid local declaration");
        if (cFunction) {
            SourceLocalDeclaration declaration{.type = type, .name = name};
            if (matchPunct("[")) {
                declaration.count = expectNumber();
                declaration.isArray = true;
                if (!declaration.count)
                    fail("Local array extent must be positive");
                expectPunct("]");
            }
            function.localDeclarations.push_back(std::move(declaration));
        }
        expectPunct(";");
    }
    while (!matchPunct("}"))
        function.body.body.push_back(parseStatement());
    parsingCFunction_ = false;
    if (sourceFunctions_.count(function.name) != 0) {
        throw std::runtime_error("duplicate function name in source: " + function.name);
    }
    sourceFunctionOrder_.push_back(function.name);
    sourceFunctions_[function.name] = std::move(function);
}

} // namespace shenmue::scenario
