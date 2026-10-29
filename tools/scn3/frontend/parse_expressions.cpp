#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"

namespace shenmue::scenario {

std::string SourceParser::normalizedCInteger(const std::string &text) {
    std::string digits = text;
    bool unsignedType = !digits.empty() && (digits.back() == 'u' || digits.back() == 'U');
    if (unsignedType)
        digits.pop_back();
    size_t consumed = 0;
    uint64_t value = 0;
    try {
        value = std::stoull(digits, &consumed, 0);
    } catch (const std::exception &) {
        fail("Unsupported C integer literal: " + text);
    }
    if (consumed != digits.size() || value > 0xFFFFFFFFULL)
        fail("Unsupported C integer literal: " + text);
    const bool decimal = !digits.empty() && digits[0] != '0';
    if (decimal && value > 0x7FFFFFFFULL && !unsignedType)
        fail("Unsupported operation: 64-bit C integer literals");
    if (value > 0x7FFFFFFFULL)
        unsignedType = true;
    return hexUnsigned(static_cast<uint32_t>(value)) + (unsignedType ? "U" : "");
}

SourceExpr SourceParser::makeExpr(SourceExprKind kind, const SourceToken &token, std::string text) {
    SourceExpr expr{};
    expr.kind = kind;
    expr.text = std::move(text);
    expr.line = token.line;
    expr.column = token.column;
    return expr;
}

int SourceParser::binaryPrecedence(const std::string &op) const {
    if (op == "||")
        return 1;
    if (op == "&&")
        return 2;
    if (op == "|")
        return 3;
    if (op == "^")
        return 4;
    if (op == "&")
        return 5;
    if (op == "==" || op == "!=")
        return 6;
    if (op == "<" || op == "<=" || op == ">" || op == ">=")
        return 7;
    if (op == "<<" || op == ">>")
        return 8;
    if (op == "+" || op == "-")
        return 9;
    if (op == "*" || op == "/" || op == "%")
        return 10;
    return 0;
}

SourceExpr SourceParser::parseExpression(int minPrec) {
    SourceExpr lhs = parseUnaryExpression();
    while (current_.kind == SourceTokenKind::Punct) {
        const std::string op = current_.text;
        const int prec = binaryPrecedence(op);
        if (prec < minPrec) {
            break;
        }
        const SourceToken opTok = current_;
        advance();
        SourceExpr rhs = parseExpression(prec + 1);
        SourceExpr binary = makeExpr(SourceExprKind::Binary, opTok, op);
        binary.args.push_back(std::move(lhs));
        binary.args.push_back(std::move(rhs));
        lhs = std::move(binary);
    }
    return lhs;
}

SourceExpr SourceParser::parseUnaryExpression() {
    if (current_.kind == SourceTokenKind::Punct &&
        (current_.text == "!" || current_.text == "~" || current_.text == "-" || current_.text == "+" || current_.text == "&" || current_.text == "*")) {
        const SourceToken tok = current_;
        advance();
        if ((parsingCFunction_ || ordinaryC_) && tok.text == "-" && current_.kind == SourceTokenKind::Number && current_.text == "2147483648") {
            advance();
            return makeExpr(SourceExprKind::IntLiteral, tok, "-2147483648");
        }
        SourceExpr expr = makeExpr(SourceExprKind::Unary, tok, tok.text);
        expr.args.push_back(parseUnaryExpression());
        return expr;
    }
    return parsePostfixExpression();
}

SourceExpr SourceParser::parsePostfixExpression() {
    SourceExpr expr = parsePrimaryExpression();
    while (true) {
        if (matchPunct("(")) {
            SourceExpr call = expr;
            call.kind = sourceImmediateWidthCallName(call.text).has_value() ? SourceExprKind::Cast : SourceExprKind::Call;
            call.args.clear();
            if (!matchPunct(")")) {
                do {
                    SourceExpr arg = parseExpression();
                    if (matchPunct("=")) {
                        if (arg.kind != SourceExprKind::Identifier || !arg.args.empty()) {
                            fail("Named call argument expects an identifier");
                        }
                        SourceExpr named = arg;
                        named.kind = SourceExprKind::NamedArg;
                        named.args.clear();
                        named.args.push_back(parseExpression());
                        call.args.push_back(std::move(named));
                    } else {
                        call.args.push_back(std::move(arg));
                    }
                } while (matchPunct(","));
                expectPunct(")");
            }
            expr = std::move(call);
            continue;
        }
        if (matchPunct("[")) {
            SourceToken tok{};
            tok.line = expr.line;
            tok.column = expr.column;
            SourceExpr index = makeExpr(SourceExprKind::Index, tok, "[]");
            index.args.push_back(std::move(expr));
            index.args.push_back(parseExpression());
            expectPunct("]");
            expr = std::move(index);
            continue;
        }
        if (matchPunct(".")) {
            SourceToken tok{};
            tok.line = expr.line;
            tok.column = expr.column;
            SourceExpr field = makeExpr(SourceExprKind::Cast, tok, "." + expectIdentifier());
            field.args.push_back(std::move(expr));
            expr = std::move(field);
            continue;
        }
        break;
    }
    return expr;
}

SourceExpr SourceParser::parsePrimaryExpression() {
    const SourceToken tok = current_;
    if (current_.kind == SourceTokenKind::Identifier) {
        const std::string text = current_.text;
        advance();
        return makeExpr(SourceExprKind::Identifier, tok, text);
    }
    if (current_.kind == SourceTokenKind::Number) {
        const std::string text = current_.text;
        advance();
        const bool isHex = text.rfind("0x", 0) == 0 || text.rfind("0X", 0) == 0;
        const bool isFloat = !isHex &&
            (text.find('.') != std::string::npos || text.find('e') != std::string::npos || text.find('E') != std::string::npos ||
                (!text.empty() && (text.back() == 'f' || text.back() == 'F')));
        if (parsingCFunction_ || ordinaryC_) {
            if (isFloat &&
                (text.empty() || (text.back() != 'f' && text.back() != 'F') ||
                    (text.find('.') == std::string::npos && text.find('e') == std::string::npos && text.find('E') == std::string::npos)))
                fail("C floating literals require a supported float spelling and f suffix");
            if (!isFloat)
                return makeExpr(SourceExprKind::IntLiteral, tok, normalizedCInteger(text));
        }
        return makeExpr(isFloat ? SourceExprKind::FloatLiteral : SourceExprKind::IntLiteral, tok, text);
    }
    if (current_.kind == SourceTokenKind::String) {
        const std::string text = current_.text;
        advance();
        return makeExpr(SourceExprKind::StringLiteral, tok, text);
    }
    if (matchPunct("(")) {
        if (current_.kind == SourceTokenKind::Identifier &&
            (sourceImmediateWidthCallName(current_.text).has_value() ||
                ((ordinaryC_ || parsingCFunction_ || mode_ == SourceParserMode::Source) &&
                    (current_.text == "void" || current_.text == "float" || current_.text == "int32_t" || current_.text == "uint32_t")))) {
            SourceExpr cast = makeExpr(SourceExprKind::Cast, current_, current_.text);
            advance();
            expectPunct(")");
            cast.args.push_back(parseUnaryExpression());
            return cast;
        }
        SourceExpr expr = parseExpression();
        if (matchPunct("=")) {
            SourceToken assignTok{};
            assignTok.line = expr.line;
            assignTok.column = expr.column;
            SourceExpr assign = makeExpr(SourceExprKind::Binary, assignTok, "=");
            assign.args.push_back(std::move(expr));
            assign.args.push_back(parseExpression());
            expectPunct(")");
            return assign;
        }
        expectPunct(")");
        return expr;
    }
    fail("Expected expression");
}

} // namespace shenmue::scenario
