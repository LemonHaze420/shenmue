#pragma once

#include "scn3/bytecode/program.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario {

enum class SourceSemantics { Explicit, C11 };

enum class SourceExprKind {
    Identifier,
    IntLiteral,
    FloatLiteral,
    StringLiteral,
    Unary,
    Binary,
    Call,
    NamedArg,
    Cast,
    Index,
    VariadicArgument,
    Temporary,
    Conditional,
    CompoundAssign,
    IndirectCall,
    Postfix,
};

struct SourceExpr {
    SourceExprKind kind = SourceExprKind::Identifier;
    std::string text{};
    std::vector<SourceExpr> args{};
    size_t line = 1;
    size_t column = 1;
    uint32_t declarationId = 0;
    std::string cType{};
    std::string cConversion{};
    bool volatileAccess = false;
};

enum class SourceStmtKind {
    Block,
    Declaration,
    ExprStmt,
    Assign,
    Return,
    If,
    While,
    DoWhile,
    For,
    Switch,
    Break,
    Continue,
    Label,
    Goto,
    TailGoto,
};

struct SourceLocalDeclaration {
    std::string type;
    std::string name;
    uint32_t count = 1;
    bool isArray = false;
    std::optional<int32_t> stackOffset{};
};

struct SourceSwitchCase;

struct SourceStmt {
    SourceStmtKind kind = SourceStmtKind::ExprStmt;
    SourceExpr expr{};
    SourceExpr value{};
    std::vector<SourceStmt> body{};
    std::vector<SourceStmt> elseBody{};
    std::vector<SourceSwitchCase> cases{};
    std::string label{};
    size_t line = 1;
    size_t column = 1;
    SourceLocalDeclaration declaration{};
    std::vector<SourceStmt> initializer{};
    SourceExpr increment{};

    bool elseIf = false;
};

struct SourceSwitchCase {
    bool isDefault = false;
    SourceExpr value{};
    std::vector<SourceStmt> body{};
    size_t line = 1;
    size_t column = 1;
};

struct SourceFunctionAst {
    std::string name;
    uint32_t offset = 0;
    std::optional<uint32_t> declaredOffset;
    std::vector<std::string> params;
    std::optional<uint32_t> explicitFrameAdjust;
    SourceStmt body;
    std::optional<uint32_t> explicitBase;
    std::map<std::string, std::string> localTypes;
    SourceSemantics semantics = SourceSemantics::Explicit;
    bool variadic = false;
    std::optional<size_t> fixedParameterCount;
    bool externalDefinition = false;
    std::string returnType;
    std::vector<SourceLocalDeclaration> localDeclarations;
    std::map<std::string, uint32_t> localOffsets;
    std::optional<uint32_t> requestedFrameSize;
    std::map<std::string, int32_t> requestedLocalOffsets;
    std::map<std::string, std::string> frameMembers;
    uint32_t temporaryBase = 0;
    uint32_t temporaryWords = 0;
    size_t line = 1;
    size_t column = 1;
};

struct SourceRawBlock {
    RangeKind kind = RangeKind::RawCodeGap;
    uint32_t declaredStart = 0;
    uint32_t declaredEnd = 0;
    std::vector<uint8_t> bytes;
};

struct SourceDataBinding {
    std::string type;
    std::uint32_t elementCount = 1;
    bool isArray = false;
    bool pointer = false;
    bool functionPointer = false;

    std::string pointerType{};
};

struct SourceDataAddress {
    std::uint32_t offset;
    const ScenarioDataSchema *schema;
    std::optional<ScenarioDataType> dataType;
    std::uint32_t elementCount;
    std::uint32_t stride;
    bool isArray;
};

struct SourceDataRelocation {
    RangeKind blockKind;
    std::size_t blockIndex;
    std::uint32_t byteOffset;
    SourceExpr target;
};

struct SourceCDeclaration {
    std::string name;
    std::string type;
    uint32_t scope = 0;
};

struct SourceUnit {
    std::vector<SourceCDeclaration> cDeclarations;
    bool ordinaryC = false;
    std::map<std::string, uint32_t> workspaceOffsets;
    std::map<std::string, SourceDataBinding> workspaceBindings;
    std::string abiProfile = "raw";
    ScenarioHeader header;
    ParsedHeader parsedHeader;
    std::optional<std::string> startupFunctionSymbol;
    std::map<std::string, SourceFunctionAst> functions;
    std::vector<std::string> functionOrder;
    std::vector<uint8_t> dataSegment;
    std::vector<uint8_t> trailingData;
    uint32_t dataSegmentDeclaredStart = 0;
    uint32_t trailingDataDeclaredStart = 0;
    std::map<std::string, uint32_t> dataSymbols;
    std::map<std::string, std::vector<uint32_t>> cstrOffsetsByText;
    std::vector<SourceRawBlock> rawBlocks;
    std::map<std::string, SourceDataBinding> dataBindings;
    std::vector<SourceDataRelocation> dataRelocations;
    std::map<std::string, std::uint32_t> codeLabels;
};

} // namespace shenmue::scenario
