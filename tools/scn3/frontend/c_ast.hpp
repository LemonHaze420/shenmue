#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario::c_frontend {

struct CType {
    enum class Kind : uint8_t { Other, Pointer, FunctionPointer, Array, Function };
    Kind kind = Kind::Other;
    std::string name{};
    std::string written{};
    uint32_t count = 0;

    std::vector<CType> inner{};

    bool constant = false;
    bool isVolatile = false;

    uint32_t alias = 0;
};

enum class CKind : uint8_t {
    None,
    Unit,
    Function,
    Variable,
    Parameter,
    Typedef,
    Record,
    Enum,
    StaticAssert,
    Empty,
    Compound,
    Declarations,
    Return,
    If,
    Switch,
    For,
    Do,
    While,
    Break,
    Continue,
    Goto,
    Label,
    Null,
    Case,
    Default,
    TypeTrait,
    Reference,
    VariadicArgument,
    Binary,
    CompoundAssign,
    Unary,
    Conditional,
    Paren,
    Constant,
    Generic,
    ImplicitCast,
    ExplicitCast,
    Integer,
    Character,
    Floating,
    String,
    Subscript,
    Member,
    Call,
    ImplicitValue,
    InitList,
    CompoundLiteral,
    Annotate,
    Builtin,
    Other,
};

enum class CStorage : uint8_t { None, Static, Extern, Other };

struct CNode {
    CKind kind = CKind::None;

    std::string className{};

    uint32_t id = 0;
    uint32_t previous = 0;

    uint32_t target = 0;
    CKind targetKind = CKind::None;

    std::string name{};

    std::string text{};

    CType type{};

    CType operand{};

    std::optional<int64_t> value{};

    uint32_t offset = 0;

    uint32_t selected = 0;

    CStorage storage = CStorage::None;
    bool implicit = false;

    bool source = false;
    bool variadic = false;
    bool postfix = false;
    bool arrow = false;
    bool initialized = false;
    bool complete = false;
    std::vector<CNode> children{};
    std::vector<CNode> attributes{};
};

struct CRecordLayout {
    struct Field {
        std::string name{};
        uint32_t offset = 0;
        CType type{};
    };
    std::string name{};
    uint32_t size = 0;
    uint32_t alignment = 0;
    std::vector<Field> fields{};
};

struct CUnit {
    CNode root{};

    std::vector<CRecordLayout> records{};
};

CUnit parseCUnit(const std::string &text);

} // namespace shenmue::scenario::c_frontend
