#pragma once

#include "scn3/frontend/source.hpp"

namespace shenmue::scenario {

enum class SourceTokenKind {
    End,
    Identifier,
    Number,
    String,
    Punct,
};

struct SourceToken {
    SourceTokenKind kind = SourceTokenKind::End;
    std::string text;
    size_t line = 1;
    size_t column = 1;
};

struct PendingSourceInsn {
    LowLevelInsn insn;
    std::optional<std::string> operandSymbol;
};

struct PendingSourceFunction {
    LowLevelFunction function;
    std::vector<PendingSourceInsn> instructions;
};

class SourceLexer {
  public:
    explicit SourceLexer(std::string input);

    SourceToken next();

  private:
    void advance();

    void skipWhitespaceAndComments();

    std::string readString();

    std::string input_;
    size_t pos_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;
};

enum class SourceParserMode { Source, Assembly };

class SourceParser {
  public:
    explicit SourceParser(std::string input, SourceParserMode mode = SourceParserMode::Source);

    Scn3Module parseAssembly();

    SourceUnit parseSourceUnit();

    SourceExpr parseExpressionOnly();

    SourceStmt parseStatementOnly();

  private:
    void parseTopLevel();

    SourceUnit buildSourceUnit();

    void parseIncludeDirective();

    [[noreturn]] void fail(const std::string &message) const;

    void advance();

    bool matchPunct(const std::string &text);

    void expectPunct(const std::string &text);

    bool matchIdentifier(const std::string &text);

    std::string expectIdentifier();

    std::string expectString();

    std::string normalizedCInteger(const std::string &text);

    uint32_t expectNumber();

    void parseFormatHeader(std::string_view format);

    void parseHeaderBlock();

    DiscoveryReason parseDiscoveryReasonName(const std::string &name);

    RangeKind parseRangeKindName(const std::string &name);

    SourceExpr makeExpr(SourceExprKind kind, const SourceToken &token, std::string text = {});

    int binaryPrecedence(const std::string &op) const;

    SourceExpr parseExpression(int minPrec = 1);

    SourceExpr parseUnaryExpression();

    SourceExpr parsePostfixExpression();

    SourceExpr parsePrimaryExpression();

    SourceStmt parseBlockStatement();

    SourceStmt parseStatement();

    void beginOrdinaryC();

    void parseCDeclaration(std::string type);

    void parseHighLevelFunctionBlock(SourceSemantics semantics = SourceSemantics::Explicit, const std::string &returnType = "",
        const std::string &declaredName = "", bool externalDefinition = false);

    void parseLoweringBlock();

    PendingSourceInsn parseInstruction();

    void parseOptionalImmediate(PendingSourceInsn &pending);

    std::vector<uint8_t> parseInlineBytes();

    std::optional<float> dataFloatValue(const SourceExpr &expression);

    void parseDataValue(std::vector<std::uint8_t> &bytes, std::uint32_t offset, ScenarioDataType type);

    void parseDataField(std::vector<std::uint8_t> &bytes, std::uint32_t offset, const ScenarioDataField &field);

    void parseDataRecord(std::vector<std::uint8_t> &bytes, std::uint32_t offset, const ScenarioDataSchema &schema);

    void appendTypedData(std::vector<std::uint8_t> &bytes, const std::string &type, const std::string &name);

    void appendDataDeclarationBytes(std::vector<uint8_t> &bytes);

    void parseRawRangeBlock();

    void parseDataLikeBlock(RangeKind kind);

    Scn3Module buildModule();

    const SourceParserMode mode_;
    SourceLexer lexer_;
    SourceToken current_;
    ScenarioHeader header_{};
    ParsedHeader parsedHeader_{};
    std::optional<std::string> startupFunctionSymbol_;
    bool parsingCFunction_ = false;
    bool ordinaryC_ = false;
    bool formatDeclared_ = false;
    uint32_t workspaceSize_ = 0;
    std::map<std::string, uint32_t> workspaceOffsets_;
    std::map<std::string, SourceDataBinding> workspaceBindings_;
    std::string abiProfile_ = "raw";
    bool sawHeader_ = false;
    bool parsedTopLevel_ = false;
    uint32_t activeBlockStart_ = 0;
    RangeKind activeBlockKind_ = RangeKind::DataSegment;
    std::size_t activeBlockIndex_ = 0;
    std::map<std::string, SourceFunctionAst> sourceFunctions_;
    std::vector<std::string> sourceFunctionOrder_;
    std::vector<PendingSourceFunction> pendingFunctions_;
    std::vector<SourceRawBlock> rawBlocks_;
    std::optional<SourceRawBlock> dataBlock_;
    std::optional<SourceRawBlock> trailingBlock_;
    std::map<std::string, uint32_t> symbolOffsets_;
    std::map<std::string, std::vector<uint32_t>> cstrOffsetsByText_;
    std::map<std::string, SourceDataBinding> dataBindings_;
    std::vector<SourceDataRelocation> dataRelocations_;
    std::map<std::string, std::uint32_t> codeLabels_;
};

} // namespace shenmue::scenario
