#include "scn3/backend/types.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario::decompile {

std::string printExpression(const SourceExpr &expr) {
    if (expr.kind == SourceExprKind::StringLiteral) {
        const std::vector<uint8_t> bytes(expr.text.begin(), expr.text.end());
        return cCharacterLiteral(bytes, 0, static_cast<uint32_t>(bytes.size()));
    }
    if (expr.kind == SourceExprKind::VariadicArgument && expr.args.size() == 1)
        return "va_arg(" + expr.args[0].text + ", " + expr.cType + ")";
    if (expr.kind == SourceExprKind::Identifier || expr.kind == SourceExprKind::IntLiteral || expr.kind == SourceExprKind::FloatLiteral)
        return expr.text;
    if (expr.kind == SourceExprKind::Cast && expr.text.starts_with("."))
        return "(" + printExpression(expr.args[0]) + ")" + expr.text;
    if (expr.kind == SourceExprKind::Cast)
        return "(" + expr.text + ")" + printExpression(expr.args[0]);
    if (expr.kind == SourceExprKind::Binary)
        return "(" + printExpression(expr.args[0]) + " " + expr.text + " " + printExpression(expr.args[1]) + ")";
    if (expr.kind == SourceExprKind::Unary) {
        const std::string child = printExpression(expr.args[0]);
        const bool simple = expr.args[0].kind == SourceExprKind::Identifier || expr.args[0].kind == SourceExprKind::IntLiteral ||
            expr.args[0].kind == SourceExprKind::FloatLiteral || expr.args[0].kind == SourceExprKind::Call || expr.args[0].kind == SourceExprKind::Binary;
        return expr.text + (simple ? child : "(" + child + ")");
    }
    if (expr.kind == SourceExprKind::Index && expr.args.size() == 2)
        return printExpression(expr.args[0]) + "[" + printExpression(expr.args[1]) + "]";
    if (expr.kind == SourceExprKind::Call) {
        std::string result = expr.text + "(";
        for (size_t i = 0; i < expr.args.size(); ++i) {
            if (i)
                result += ", ";
            result += printExpression(expr.args[i]);
        }
        return result + ")";
    }
    throw std::runtime_error("Explicit printer received an unsupported expression");
}

std::string printCondition(const SourceExpr &expr) {
    const std::string text = printExpression(expr);
    return expr.kind == SourceExprKind::Binary ? text.substr(1, text.size() - 2) : text;
}

namespace {

int cPrecedence(const SourceExpr &expr) {
    if (expr.kind == SourceExprKind::Binary) {
        const std::string &op = expr.text;
        if (op == "*" || op == "/" || op == "%")
            return 13;
        if (op == "+" || op == "-")
            return 12;
        if (op == "<<" || op == ">>")
            return 11;
        if (op == "<" || op == "<=" || op == ">" || op == ">=")
            return 10;
        if (op == "==" || op == "!=")
            return 9;
        if (op == "&")
            return 8;
        if (op == "^")
            return 7;
        if (op == "|")
            return 6;
        if (op == "&&")
            return 5;
        if (op == "||")
            return 4;
        return 2;
    }
    if (expr.kind == SourceExprKind::Unary || (expr.kind == SourceExprKind::Cast && !expr.text.starts_with(".")))
        return 14;
    if (expr.kind == SourceExprKind::Call || expr.kind == SourceExprKind::Index || expr.kind == SourceExprKind::Cast)
        return 15;
    return 16;
}

bool bitwise(const SourceExpr &expr) {
    return expr.kind == SourceExprKind::Binary &&
        (expr.text == "&" || expr.text == "|" || expr.text == "^" || expr.text == "<<" || expr.text == ">>" || expr.text == "&&");
}

} // namespace

std::string printCExpression(const SourceExpr &expr, int context) {
    std::string text;
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2) {
        const int precedence = cPrecedence(expr);
        const bool assignment = precedence == 2;
        const std::function<std::string(const SourceExpr &, int)> operand = [&](const SourceExpr &child, int required) {
            if (bitwise(child) && child.text != expr.text)
                return "(" + printCExpression(child, 0) + ")";
            if (bitwise(expr) && child.kind == SourceExprKind::Binary && child.text != expr.text && cPrecedence(child) < 14)
                return "(" + printCExpression(child, 0) + ")";
            return printCExpression(child, required);
        };
        text = operand(expr.args[0], assignment ? 14 : precedence) + " " + expr.text + " " + operand(expr.args[1], assignment ? precedence : precedence + 1);
        return precedence < context ? "(" + text + ")" : text;
    }
    if (expr.kind == SourceExprKind::Cast && (expr.text == "scn3_bits.i" || expr.text == "scn3_bits.u" || expr.text == "scn3_bits.f") && expr.args.size() == 1)
        text =
            std::string("((scn3_bits){.") + (expr.text == "scn3_bits.f" ? "i" : "f") + " = " + printCExpression(expr.args[0], 0) + "})." + expr.text.substr(10);
    else if (expr.kind == SourceExprKind::Unary && expr.args.size() == 1) {
        const std::string operand = printCExpression(expr.args[0], 14);
        text = expr.text + ((expr.text == "-" || expr.text == "+") && operand.starts_with(expr.text) ? " " : "") + operand;
    } else if (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".") && expr.args.size() == 1 && expr.args[0].kind == SourceExprKind::Unary &&
        expr.args[0].text == "*" && expr.args[0].args.size() == 1)
        text = printCExpression(expr.args[0].args[0], 15) + "->" + expr.text.substr(1);
    else if (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".") && expr.args.size() == 1)
        text = printCExpression(expr.args[0], 15) + expr.text;
    else if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1)
        text = "(" + expr.text + ")" + printCExpression(expr.args[0], 14);
    else if (expr.kind == SourceExprKind::Index && expr.args.size() == 2)
        text = printCExpression(expr.args[0], 15) + "[" + printCExpression(expr.args[1], 0) + "]";
    else if (expr.kind == SourceExprKind::Call) {
        text = expr.text + "(";
        for (size_t i = 0; i < expr.args.size(); ++i)
            text += (i ? ", " : "") + printCExpression(expr.args[i], 2);
        text += ")";
    } else
        return printExpression(expr);
    return cPrecedence(expr) < context ? "(" + text + ")" : text;
}

void printStatement(std::ostream &out, const SourceStmt &stmt, const std::string &indent, bool cSyntax) {
    const std::function<std::string(const SourceExpr &)> printExpression = [&](const SourceExpr &expr) {
        return cSyntax ? printCExpression(expr, 0) : decompile::printExpression(expr);
    };
    const std::function<std::string(const SourceExpr &)> printCondition = [&](const SourceExpr &expr) {
        return cSyntax ? printCExpression(expr, 0) : decompile::printCondition(expr);
    };
    switch (stmt.kind) {
    case SourceStmtKind::Block:
        out << indent << "{\n";
        for (const SourceStmt &child : stmt.body)
            printStatement(out, child, indent + "  ", cSyntax);
        out << indent << "}\n";
        break;
    case SourceStmtKind::Declaration: {

        const std::string &type = stmt.declaration.type;
        const size_t rows = type.find('[');
        out << indent << type.substr(0, rows) << (type.ends_with("*") ? "" : " ") << stmt.declaration.name;
        if (stmt.declaration.isArray)
            out << "[" << stmt.declaration.count << "]";
        if (rows != std::string::npos)
            out << type.substr(rows);
    }
        if (stmt.declaration.stackOffset)
            out << " __attribute__((scn_stack_offset(" << *stmt.declaration.stackOffset << ")))";
        out << ";\n";
        break;
    case SourceStmtKind::Label:
        out << indent << stmt.label << (cSyntax ? ":;\n" : ":\n");
        break;
    case SourceStmtKind::Return:
        out << indent << "return" << ((stmt.expr.text.empty() && stmt.expr.args.empty()) ? "" : " " + printExpression(stmt.expr)) << ";\n";
        break;
    case SourceStmtKind::Assign:
        if (((stmt.expr.text.empty() && stmt.expr.args.empty()) || stmt.expr.text == "eax") && stmt.value.kind == SourceExprKind::Binary &&
            stmt.value.text == "=")
            out << indent << printExpression(stmt.value.args[0]) << " = " << printExpression(stmt.value.args[1]) << ";\n";
        else
            out << indent << ((stmt.expr.text.empty() && stmt.expr.args.empty()) ? "eax" : printExpression(stmt.expr)) << " = " << printExpression(stmt.value)
                << ";\n";
        break;
    case SourceStmtKind::ExprStmt:
        out << indent << printExpression(stmt.expr) << ";\n";
        break;
    case SourceStmtKind::If:
        if (stmt.elseBody.empty() && stmt.body.size() == 1 && stmt.body[0].kind == SourceStmtKind::Goto) {
            out << indent << "if (" << printCondition(stmt.expr) << ") goto " << stmt.body[0].label << ";\n";
            break;
        }
        if (stmt.elseBody.empty() && stmt.body.size() == 1 && stmt.body[0].kind == SourceStmtKind::ExprStmt && stmt.expr.kind == SourceExprKind::Unary &&
            stmt.expr.text == "!" && stmt.expr.args[0].text == "eax" && stmt.body[0].expr.text == "vm_goto_rel") {
            out << indent << "if (!eax) " << printExpression(stmt.body[0].expr) << ";\n";
            break;
        }
        out << indent << "if (" << printCondition(stmt.expr) << ") {\n";
        for (const SourceStmt &child : stmt.body)
            printStatement(out, child, indent + "  ", cSyntax);

        for (const SourceStmt *arm = &stmt; !arm->elseBody.empty();) {
            const SourceStmt &next = arm->elseBody.front();
            if (cSyntax && arm->elseBody.size() == 1 && next.kind == SourceStmtKind::If && (arm->elseIf || !next.elseBody.empty())) {
                out << indent << "} else if (" << printCondition(next.expr) << ") {\n";
                for (const SourceStmt &child : next.body)
                    printStatement(out, child, indent + "  ", cSyntax);
                arm = &next;
                continue;
            }
            out << indent << "} else {\n";
            for (const SourceStmt &child : arm->elseBody)
                printStatement(out, child, indent + "  ", cSyntax);
            break;
        }
        out << indent << "}\n";
        break;
    case SourceStmtKind::Switch:
        out << indent << "switch (" << printExpression(stmt.expr) << ") {\n";
        for (const SourceStmt &child : stmt.body)
            printStatement(out, child, indent + "  ", cSyntax);
        for (const SourceSwitchCase &clause : stmt.cases) {
            out << indent << "  " << (clause.isDefault ? "default" : "case " + printExpression(clause.value)) << ":\n";
            for (const SourceStmt &child : clause.body)
                printStatement(out, child, indent + "    ", cSyntax);
        }
        out << indent << "}\n";
        break;
    case SourceStmtKind::Goto:
        out << indent << "goto " << stmt.label << ";\n";
        break;
    case SourceStmtKind::Break:
        out << indent << "break;\n";
        break;
    case SourceStmtKind::Continue:
        out << indent << "continue;\n";
        break;
    case SourceStmtKind::For:
    case SourceStmtKind::While:
        if (stmt.kind == SourceStmtKind::For)
            out << indent << "for (;;) {\n";
        else
            out << indent << "while (" << printCondition(stmt.expr) << ") {\n";
        for (const SourceStmt &child : stmt.body)
            printStatement(out, child, indent + "  ", cSyntax);
        out << indent << "}\n";
        break;
    default:
        throw std::runtime_error("Explicit printer received an unsupported statement");
    }
}

std::string cCharacterLiteral(const std::vector<uint8_t> &bytes, uint32_t start, uint32_t end) {
    while (end > start && bytes[end - 1] == 0)
        --end;
    const std::string_view range(reinterpret_cast<const char *>(bytes.data()) + start, end - start);
    const std::optional<std::string> utf8 = shiftJisToUtf8(range);
    if (!utf8 && utf8ToShiftJis(range))
        throw std::runtime_error("Character data would read back as UTF-8 text");
    std::ostringstream text;
    text << '\"';
    for (const char character : utf8 ? std::string_view(*utf8) : range) {
        const uint8_t value = static_cast<uint8_t>(character);
        if (value == '\"' || value == '\\')
            text << '\\' << static_cast<char>(value);
        else if ((value >= 0x20U && value <= 0x7EU) || (utf8 && value >= 0x80U))
            text << static_cast<char>(value);
        else
            text << '\\' << static_cast<char>('0' + (value >> 6U)) << static_cast<char>('0' + ((value >> 3U) & 7U)) << static_cast<char>('0' + (value & 7U));
    }
    text << '\"';
    return text.str();
}

void cDataValue(std::ostream &out, const std::vector<uint8_t> &bytes, uint32_t offset, ScenarioDataType type) {
    const uint32_t size = scenarioDataTypeSize(type);
    const uint32_t word = size == 1 ? bytes.at(offset) : size == 2 ? readU16(bytes, offset) : readU32(bytes, offset);
    switch (type) {
    case ScenarioDataType::I8:
        out << static_cast<int32_t>(static_cast<int8_t>(word));
        break;
    case ScenarioDataType::I16:
        out << static_cast<int32_t>(static_cast<int16_t>(word));
        break;
    case ScenarioDataType::I32:
        out << static_cast<int32_t>(word);
        break;
    case ScenarioDataType::F32: {
        const std::string literal = formatFloatLiteral(word);
        const std::optional<SourceExpr> expression = parseExpressionText(literal);
        if (!expression || !explicitExpressionIsFloat(*expression) || !floatLiteralTextRoundTrips(literal, word))
            throw std::runtime_error("Initialized C data requires a representable floating constant");
        out << literal;
        break;
    }
    case ScenarioDataType::ScriptOffset32:
        throw std::runtime_error("C script pointer initializer requires a resolved symbol");
    default:

        if (word < 0x100000U)
            out << word;
        else
            out << hexUnsigned(word) << "U";
        break;
    }
}

void cDataRecord(std::ostream &out, const std::vector<uint8_t> &bytes, uint32_t start, const ScenarioDataSchema &schema,
    const std::function<std::string(uint32_t)> &pointer) {
    std::vector<bool> covered(schema.size, false);
    out << "{";
    for (size_t index = 0; index < schema.fields.size(); ++index) {
        const ScenarioDataField &field = schema.fields[index];
        if (index)
            out << ", ";
        out << "." << field.name << " = ";
        if (field.elementCount > 1)
            out << "{";
        const uint32_t size = scenarioDataTypeSize(field.type);
        for (uint32_t element = 0; element < field.elementCount; ++element) {
            if (element)
                out << ", ";
            const uint32_t offset = field.offset + element * size;
            for (uint32_t byte = 0; byte < size; ++byte)
                covered.at(offset + byte) = true;
            if (field.type == ScenarioDataType::ScriptOffset32 && pointer)
                out << pointer(start + offset);
            else
                cDataValue(out, bytes, start + offset, field.type);
        }
        if (field.elementCount > 1)
            out << "}";
    }
    for (uint32_t byte = 0; byte < schema.size; ++byte)
        if (!covered[byte] && bytes.at(start + byte))
            throw std::runtime_error("C record contains nonzero padding with no field initializer");
    out << "}";
}

void cPrototype(std::ostream &out, const SourceFunctionAst &function) {
    if (!function.externalDefinition)
        out << "static ";
    out << function.returnType << (function.returnType.ends_with("*") ? "" : " ") << function.name << "(";
    const size_t fixed = function.fixedParameterCount.value_or(function.params.size());
    if (!fixed)
        out << "void";
    for (size_t index = 0; index < fixed; ++index) {
        if (index)
            out << ", ";
        const std::string &type = function.localTypes.at(function.params[index]);
        out << type << (type.ends_with("*") ? "" : " ") << function.params[index];
    }
    if (function.variadic)
        out << ", ...";
    out << ")";
}

std::string cFunctionPointerType(const SourceFunctionAst &function, const std::string &declarator) {
    std::ostringstream out;
    out << function.returnType << " (*" << declarator << ")(";
    const size_t fixed = function.fixedParameterCount.value_or(function.params.size());
    if (!fixed)
        out << "void";
    for (size_t index = 0; index < fixed; ++index)
        out << (index ? ", " : "") << function.localTypes.at(function.params[index]);
    if (function.variadic)
        out << ", ...";
    out << ")";
    return out.str();
}

} // namespace shenmue::scenario::decompile
