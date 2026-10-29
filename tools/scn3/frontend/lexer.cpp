#include "scn3/common/util.hpp"
#include "scn3/frontend/parser.hpp"

namespace shenmue::scenario {

SourceLexer::SourceLexer(std::string input) : input_(std::move(input)) {
}

SourceToken SourceLexer::next() {
    skipWhitespaceAndComments();
    SourceToken tok;
    tok.line = line_;
    tok.column = column_;
    if (pos_ >= input_.size()) {
        tok.kind = SourceTokenKind::End;
        return tok;
    }
    const char ch = input_[pos_];
    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
        tok.kind = SourceTokenKind::Identifier;
        const size_t start = pos_;
        advance();
        while (pos_ < input_.size()) {
            const char c = input_[pos_];
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.') {
                advance();
            } else {
                break;
            }
        }
        tok.text = input_.substr(start, pos_ - start);
        return tok;
    }
    if (std::isdigit(static_cast<unsigned char>(ch))) {
        tok.kind = SourceTokenKind::Number;
        const size_t start = pos_;
        advance();
        while (pos_ < input_.size()) {
            const char c = input_[pos_];
            const bool exponentSign = (c == '+' || c == '-') && pos_ > start && (input_[pos_ - 1] == 'e' || input_[pos_ - 1] == 'E') &&
                !(pos_ >= start + 2 && input_[start] == '0' && (input_[start + 1] == 'x' || input_[start + 1] == 'X'));
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '.' || exponentSign) {
                advance();
            } else {
                break;
            }
        }
        tok.text = input_.substr(start, pos_ - start);
        return tok;
    }
    if (ch == '"') {
        tok.kind = SourceTokenKind::String;
        tok.text = readString();
        return tok;
    }
    tok.kind = SourceTokenKind::Punct;
    if (pos_ + 1 < input_.size()) {
        const std::string two = input_.substr(pos_, 2);
        if (two == "==" || two == "!=" || two == "<=" || two == ">=" || two == "&&" || two == "||" || two == "<<" || two == ">>" || two == "->") {
            tok.text = two;
            advance();
            advance();
            return tok;
        }
    }
    tok.text = std::string(1, ch);
    advance();
    return tok;
}

void SourceLexer::advance() {
    if (pos_ < input_.size() && input_[pos_] == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    ++pos_;
}

void SourceLexer::skipWhitespaceAndComments() {
    while (pos_ < input_.size()) {
        const char ch = input_[pos_];
        if (std::isspace(static_cast<unsigned char>(ch))) {
            advance();
            continue;
        }
        if (ch == '/' && pos_ + 1 < input_.size() && input_[pos_ + 1] == '/') {
            while (pos_ < input_.size() && input_[pos_] != '\n') {
                advance();
            }
            continue;
        }
        if (ch == '/' && pos_ + 1 < input_.size() && input_[pos_ + 1] == '*') {
            advance();
            advance();
            while (pos_ + 1 < input_.size() && !(input_[pos_] == '*' && input_[pos_ + 1] == '/')) {
                advance();
            }
            if (pos_ + 1 >= input_.size()) {
                throw std::runtime_error("Unterminated block comment");
            }
            advance();
            advance();
            continue;
        }
        break;
    }
}

std::string SourceLexer::readString() {
    std::string out;
    advance();
    while (pos_ < input_.size()) {
        const char ch = input_[pos_];
        if (ch == '"') {
            advance();
            return utf8ToShiftJis(out).value_or(out);
        }
        if (ch != '\\') {
            out.push_back(ch);
            advance();
            continue;
        }
        advance();
        if (pos_ >= input_.size()) {
            throw std::runtime_error("Unterminated escape in string literal");
        }
        const char esc = input_[pos_];
        advance();
        switch (esc) {
        case 'n':
            out.push_back('\n');
            break;
        case 'r':
            out.push_back('\r');
            break;
        case 't':
            out.push_back('\t');
            break;
        case '\\':
            out.push_back('\\');
            break;
        case '"':
            out.push_back('"');
            break;
        case 'x': {
            if (pos_ + 1 > input_.size()) {
                throw std::runtime_error("Short hex escape in string literal");
            }
            const std::string hex = input_.substr(pos_, 2);
            if (hex.size() != 2 || !std::isxdigit(static_cast<unsigned char>(hex[0])) || !std::isxdigit(static_cast<unsigned char>(hex[1]))) {
                throw std::runtime_error("Invalid hex escape in string literal");
            }
            out.push_back(static_cast<char>(std::stoul(hex, nullptr, 16)));
            advance();
            advance();
            break;
        }
        case 'a':
            out.push_back('\a');
            break;
        case 'b':
            out.push_back('\b');
            break;
        case 'f':
            out.push_back('\f');
            break;
        case 'v':
            out.push_back('\v');
            break;
        case '?':
            out.push_back('?');
            break;
        default:
            if (esc >= '0' && esc <= '7') {
                unsigned value = static_cast<unsigned>(esc - '0');
                for (unsigned count = 1; count < 3 && pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '7'; ++count) {
                    value = value * 8U + static_cast<unsigned>(input_[pos_] - '0');
                    advance();
                }
                if (value > 255U)
                    throw std::runtime_error("Octal string escape exceeds one byte");
                out.push_back(static_cast<char>(value));
            } else
                throw std::runtime_error(std::string("Unsupported string escape: ") + esc);
            break;
        }
    }
    throw std::runtime_error("Unterminated string literal");
}

} // namespace shenmue::scenario
