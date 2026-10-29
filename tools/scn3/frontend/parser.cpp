#include "scn3/backend/compiler.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/motion.hpp"

namespace shenmue::scenario {

SourceParser::SourceParser(std::string input, SourceParserMode mode) : mode_(mode), lexer_(std::move(input)) {
    current_ = lexer_.next();
}

Scn3Module SourceParser::parseAssembly() {
    if (mode_ != SourceParserMode::Assembly)
        fail("Expected assembly parser mode");
    parseTopLevel();
    return buildModule();
}

SourceUnit SourceParser::buildSourceUnit() {
    parseTopLevel();
    SourceUnit unit{};
    unit.ordinaryC = ordinaryC_;
    unit.workspaceOffsets = workspaceOffsets_;
    unit.workspaceBindings = workspaceBindings_;
    unit.abiProfile = abiProfile_;
    unit.header = header_;
    unit.parsedHeader = parseHeaderSemantics(header_);
    unit.startupFunctionSymbol = startupFunctionSymbol_;
    unit.functions = sourceFunctions_;
    unit.functionOrder = sourceFunctionOrder_;
    if (dataBlock_.has_value()) {
        unit.dataSegment = dataBlock_->bytes;
        unit.dataSegmentDeclaredStart = dataBlock_->declaredStart;
    }
    if (trailingBlock_.has_value()) {
        unit.trailingData = trailingBlock_->bytes;
        unit.trailingDataDeclaredStart = trailingBlock_->declaredStart;
    }
    unit.dataSymbols = symbolOffsets_;
    unit.cstrOffsetsByText = cstrOffsetsByText_;
    unit.rawBlocks = rawBlocks_;
    unit.dataBindings = dataBindings_;
    unit.dataRelocations = dataRelocations_;
    unit.codeLabels = codeLabels_;
    applySourceDataRelocations(unit);
    return unit;
}

SourceUnit SourceParser::parseSourceUnit() {
    if (mode_ != SourceParserMode::Source)
        fail("Expected source parser mode");
    return buildSourceUnit();
}

SourceExpr SourceParser::parseExpressionOnly() {
    SourceExpr expr = parseExpression();
    if (current_.kind != SourceTokenKind::End) {
        fail("Unexpected token after expression: " + current_.text);
    }
    return expr;
}

SourceStmt SourceParser::parseStatementOnly() {
    SourceStmt stmt = parseStatement();
    if (current_.kind != SourceTokenKind::End) {
        fail("Unexpected token after statement: " + current_.text);
    }
    return stmt;
}

void SourceParser::parseTopLevel() {
    if (parsedTopLevel_) {
        return;
    }
    while (current_.kind != SourceTokenKind::End) {
        if (current_.kind == SourceTokenKind::Punct && current_.text == "#") {
            parseIncludeDirective();
            continue;
        }
        const std::string top = expectIdentifier();
        if (top == "scn3_source" || top == "scn3_assembly") {
            parseFormatHeader(top);
        } else if (top == "abi") {
            abiProfile_ = expectIdentifier();
            if (abiProfile_ != "raw" && abiProfile_ != "s1_pc")
                fail("Unsupported ABI profile: " + abiProfile_);
            expectPunct(";");
        } else if (top == "scn3_header") {
            parseHeaderBlock();
        } else if (top == "function") {
            if (mode_ != SourceParserMode::Source)
                fail("Assembly input cannot contain source functions");
            parseHighLevelFunctionBlock();
        } else if ((!formatDeclared_ || ordinaryC_) &&
            (top == "static" || top == "extern" || top == "const" || top == "void" || scenarioDataTypeByName(top) || findScenarioDataSchema(top))) {
            if (mode_ != SourceParserMode::Source)
                fail("Assembly input cannot contain C declarations");
            beginOrdinaryC();
            parseCDeclaration(top);
        } else if (mode_ == SourceParserMode::Source && (top == "void" || top == "uint32_t" || top == "int32_t" || top == "float")) {
            fail("Use function declarations in explicit source or ordinary C without scn3_source");
        } else if (top == "lowering") {
            if (mode_ != SourceParserMode::Assembly)
                fail("Source input cannot contain assembly blocks");
            parseLoweringBlock();
        } else if (top == "code_label") {
            const std::string name = expectIdentifier();
            if (!matchIdentifier("at"))
                fail("Code label requires at(offset)");
            expectPunct("(");
            const std::uint32_t offset = expectNumber();
            expectPunct(")");
            expectPunct(";");
            if (!codeLabels_.emplace(name, offset).second)
                fail("Duplicate code label: " + name);
        } else if (top == "raw_range") {
            parseRawRangeBlock();
        } else if (top == "data_segment") {
            parseDataLikeBlock(RangeKind::DataSegment);
        } else if (top == "trailing_data_or_unknown" || top == "module_data") {
            parseDataLikeBlock(RangeKind::TrailingDataOrUnknown);
        } else {
            fail("Unexpected top-level item: " + top);
        }
    }
    if (ordinaryC_) {
        if (!startupFunctionSymbol_)
            fail("A scenario requires one externally visible entry function definition");
        header_.dataSegmentOffset = sizeof(ScenarioHeader);
        header_.dataEndOffset = header_.dataSegmentOffset + static_cast<uint32_t>(dataBlock_->bytes.size());
        header_.relocationTableOffset = header_.dataEndOffset;
        header_.workspaceSize = workspaceSize_;
        header_.wholeSize = header_.dataEndOffset + 8;
        trailingBlock_->declaredStart = header_.dataEndOffset;
        trailingBlock_->bytes.assign(8, 0);
    }
    parsedTopLevel_ = true;
}

void SourceParser::parseIncludeDirective() {
    expectPunct("#");
    const std::string directive = expectIdentifier();
    if (directive == "define") {
        const std::string name = expectIdentifier();
        if (!name.starts_with("MN_"))
            fail("Unsupported #define: " + name);
        defineMotionName(name, expectNumber());
        return;
    }
    if (directive != "include") {
        fail("Unsupported preprocessor directive #" + directive);
    }
    const std::string header = expectString();
    if (mode_ == SourceParserMode::Source && header != "scenario.h" && header != "s1/task/scen.hpp" && header != "s1/task/dswk.hpp" &&
        !loadMotionInclude(header))
        fail("Header has no compile-time scenario binding: " + header);
    matchPunct(";");
}

[[noreturn]] void SourceParser::fail(const std::string &message) const {
    std::ostringstream oss;
    oss << message << " at line " << current_.line << ", column " << current_.column;
    throw std::runtime_error(oss.str());
}

void SourceParser::advance() {
    current_ = lexer_.next();
}

bool SourceParser::matchPunct(const std::string &text) {
    if (current_.kind == SourceTokenKind::Punct && current_.text == text) {
        advance();
        return true;
    }
    return false;
}

void SourceParser::expectPunct(const std::string &text) {
    if (!matchPunct(text)) {
        fail("Expected '" + text + "'");
    }
}

bool SourceParser::matchIdentifier(const std::string &text) {
    if (current_.kind == SourceTokenKind::Identifier && current_.text == text) {
        advance();
        return true;
    }
    return false;
}

std::string SourceParser::expectIdentifier() {
    if (current_.kind != SourceTokenKind::Identifier) {
        fail("Expected identifier");
    }
    const std::string out = current_.text;
    advance();
    return out;
}

std::string SourceParser::expectString() {
    if (current_.kind != SourceTokenKind::String) {
        fail("Expected string literal");
    }
    const std::string out = current_.text;
    advance();
    return out;
}

uint32_t SourceParser::expectNumber() {
    if (current_.kind != SourceTokenKind::Number) {
        fail("Expected number");
    }
    const uint32_t out = (parsingCFunction_ || ordinaryC_) ? parseSourceIntegerLiteral(normalizedCInteger(current_.text)) : parseIntegerText(current_.text);
    advance();
    return out;
}

void SourceParser::parseFormatHeader(std::string_view format) {
    if (formatDeclared_)
        fail("Duplicate format declaration");
    if (ordinaryC_)
        fail("Format declarations cannot follow C declarations");
    const bool assembly = mode_ == SourceParserMode::Assembly;
    if (format != (assembly ? "scn3_assembly" : "scn3_source"))
        fail(assembly ? "Expected scn3_assembly input" : "Expected scn3_source input");
    const std::uint32_t version = expectNumber();
    if (version != (assembly ? 1U : 2U))
        fail("Unsupported " + std::string(format) + " version");
    expectPunct(";");
    formatDeclared_ = true;
}

void SourceParser::parseHeaderBlock() {
    expectPunct("{");
    std::optional<uint32_t> dataSegmentOffset;
    std::optional<uint32_t> codeEnd;
    while (!matchPunct("}")) {
        const std::string key = expectIdentifier();
        expectPunct("=");
        if (key == "token") {
            const std::string token = expectString();
            if (token.size() != 4) {
                fail("Header token must be 4 bytes");
            }
            header_.token = static_cast<uint8_t>(token[0]) | (static_cast<uint32_t>(static_cast<uint8_t>(token[1])) << 8) |
                (static_cast<uint32_t>(static_cast<uint8_t>(token[2])) << 16) | (static_cast<uint32_t>(static_cast<uint8_t>(token[3])) << 24);
        } else if (key == "flags" || key == "field08") {
            const std::optional<uint32_t> value = sourceDataConstantValue(parseExpression());
            if (!value)
                fail("Scenario flags require a constant expression");
            header_.flags = *value;
        } else if (key == "startupFunctionOffset" && current_.kind == SourceTokenKind::Identifier) {
            startupFunctionSymbol_ = expectIdentifier();
            header_.functionOffset = 0;
        } else {
            const uint32_t value = expectNumber();
            if (key == "wholeSize")
                header_.wholeSize = value;
            else if (key == "startupFunctionOffset") {
                header_.functionOffset = value;
                startupFunctionSymbol_.reset();
            } else if (key == "dataSegmentOffset") {
                header_.dataSegmentOffset = value;
                dataSegmentOffset = value;
                if (codeEnd.has_value() && *codeEnd != value) {
                    fail("codeEnd must match dataSegmentOffset");
                }
            } else if (key == "codeStart" || key == "functionPoolStart")
                header_.entrypoint = value;
            else if (key == "codeEnd") {
                codeEnd = value;
                if (dataSegmentOffset.has_value() && value != *dataSegmentOffset) {
                    fail("codeEnd must match dataSegmentOffset");
                }
            } else if (key == "dataEndOffset" || key == "field14")
                header_.dataEndOffset = value;
            else if (key == "workspaceSize" || key == "field18")
                header_.workspaceSize = value;
            else if (key == "field1C")
                header_.unknownOffset = value;
            else if (key == "relocationTableOffset" || key == "field24")
                header_.relocationTableOffset = value;
            else if (key == "field28")
                header_.unknown1 = value;
            else if (key == "field2C")
                header_.unknown2 = value;
            else
                fail("Unknown header field: " + key);
        }
        expectPunct(";");
    }
    parsedHeader_ = parseHeaderSemantics(header_);
    sawHeader_ = true;
}

DiscoveryReason SourceParser::parseDiscoveryReasonName(const std::string &name) {
    if (name == "function_pool_start")
        return DiscoveryReason::FunctionPoolStart;
    if (name == "startup_entry")
        return DiscoveryReason::StartupEntry;
    if (name == "local_call")
        return DiscoveryReason::LocalCall;
    if (name == "task_created")
        return DiscoveryReason::TaskCreated;
    if (name == "packed_next")
        return DiscoveryReason::PackedNext;
    if (name == "decode_uncovered")
        return DiscoveryReason::DecodeUncovered;
    fail("Unknown discovery reason: " + name);
}

RangeKind SourceParser::parseRangeKindName(const std::string &name) {
    if (name == "raw_code_gap")
        return RangeKind::RawCodeGap;
    if (name == "tail_merged_stub_candidate")
        return RangeKind::TailMergedStubCandidate;
    if (name == "packed_padding")
        return RangeKind::PackedPadding;
    if (name == "data_segment")
        return RangeKind::DataSegment;
    if (name == "trailing_data_or_unknown")
        return RangeKind::TrailingDataOrUnknown;
    fail("Unsupported source range kind: " + name);
}

SourceUnit parseScn3SourceUnit(const std::string &path) {
    setScenarioCIncludeDirectory(std::filesystem::absolute(path).parent_path());
    const std::vector<uint8_t> bytes = readFile(path);
    const std::string text(bytes.begin(), bytes.end());
    loadMotionSourceHeaders(text, std::filesystem::absolute(path).parent_path());
    return parseScenarioSourceText(text);
}

std::optional<SourceExpr> parseExpressionText(const std::string &text) {
    try {
        SourceParser parser(text);
        return parser.parseExpressionOnly();
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

std::optional<SourceStmt> parseStatementText(const std::string &text) {
    try {
        SourceParser parser(text);
        return parser.parseStatementOnly();
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

Scn3Module compileScn3Source(const std::string &path) {
    const SourceUnit unit = parseScn3SourceUnit(path);
    return buildScn3ModuleFromSourceUnit(unit);
}

SourceUnit parseScenarioSourceText(const std::string &text) {
    SourceLexer lexer(text);
    SourceToken token = lexer.next();
    while (token.text == "#") {
        const size_t line = token.line;
        do {
            token = lexer.next();
        } while (token.kind != SourceTokenKind::End && token.line == line);
    }
    if (token.text != "scn3_source" && token.text != "scn3_assembly")
        return parseScenarioC(text);
    SourceParser parser(text);
    return parser.parseSourceUnit();
}

Scn3Module assembleScenarioText(const std::string &path) {
    const std::vector<std::uint8_t> bytes = readFile(path);
    SourceParser parser(std::string(bytes.begin(), bytes.end()), SourceParserMode::Assembly);
    return parser.parseAssembly();
}

} // namespace shenmue::scenario
