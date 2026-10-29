#include "scn3/backend/types.hpp"
#include "scn3/decompiler/passes.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario::decompile {

void recoverCUnaryOperators(SourceFunctionAst &function, const SourceUnit &unit) {
    if (function.semantics != SourceSemantics::C11)
        return;
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Binary || expression.text != "-" || expression.args.size() != 2 ||
            expression.args[0].kind != SourceExprKind::IntLiteral || sourceDataConstantValue(expression.args[0]) != 0U ||
            explicitExpressionIsFloat(expression.args[1], &unit, &function))
            return;
        expression = SourceExpr{.kind = SourceExprKind::Unary, .text = "-", .args = {std::move(expression.args[1])}, .cType = expression.cType};
    });
}

std::string cVariadicCursor(const SourceFunctionAst &function) {
    std::string name = "arguments";
    while (function.localTypes.count(name))
        name += "_";
    return name;
}

void recoverCBooleanValues(SourceFunctionAst &function) {
    const std::function<bool(const SourceExpr &)> predicate = [&](const SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Binary)
            return expression.text == "==" || expression.text == "!=" || expression.text == "<" || expression.text == "<=" || expression.text == ">" ||
                expression.text == ">=";
        return expression.kind == SourceExprKind::Unary && expression.text == "!" && predicate(expression.args.at(0));
    };
    const std::function<void(SourceExpr &, bool)> rewrite = [&](SourceExpr &expression, bool truth) {
        if (predicate(expression)) {
            if (expression.kind == SourceExprKind::Unary)
                rewrite(expression.args[0], true);
            else
                for (SourceExpr &child : expression.args)
                    rewrite(child, false);
            if (!truth)
                expression = SourceExpr{.kind = SourceExprKind::Unary, .text = "-", .args = {std::move(expression)}, .cType = "int32_t"};
            return;
        }
        const bool truthChildren = truth && expression.kind == SourceExprKind::Binary && expression.args.size() == 2 &&
            (expression.text == "|" || ((expression.text == "&" || expression.text == "^") && predicate(expression.args[0]) && predicate(expression.args[1])));
        for (SourceExpr &child : expression.args)
            rewrite(child, truthChildren);
    };
    const std::function<void(std::vector<SourceStmt> &)> statements = [&](std::vector<SourceStmt> &body) {
        for (SourceStmt &statement : body) {
            const bool truth = statement.kind == SourceStmtKind::If || statement.kind == SourceStmtKind::While || statement.kind == SourceStmtKind::For ||
                statement.kind == SourceStmtKind::DoWhile;
            if (statement.kind == SourceStmtKind::ExprStmt) {
                for (SourceExpr &argument : statement.expr.args)
                    rewrite(argument, false);
            } else if (statement.kind != SourceStmtKind::Declaration)
                rewrite(statement.expr, truth);
            rewrite(statement.value, false);
            statements(statement.initializer);
            statements(statement.body);
            statements(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                statements(clause.body);
        }
    };
    statements(function.body.body);
}

void recoverCWordArithmetic(SourceFunctionAst &function, const SourceUnit &unit) {
    using Bounds = std::pair<int64_t, int64_t>;
    const Bounds word{INT32_MIN, INT32_MAX};
    const std::function<Bounds(const SourceExpr &)> bounds = [&](const SourceExpr &expression) -> Bounds {
        if (const std::optional<uint32_t> value = sourceDataConstantValue(expression, true)) {
            const int64_t number = sourceConstantIsUnsigned(expression) ? static_cast<int64_t>(*value) : static_cast<int32_t>(*value);
            return {number, number};
        }
        if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2) {
            if (expression.text == "==" || expression.text == "!=" || expression.text == "<" || expression.text == "<=" || expression.text == ">" ||
                expression.text == ">=")
                return {0, 1};
            if (expression.text == "&")
                for (const SourceExpr &child : expression.args)
                    if (const std::optional<uint32_t> mask = sourceDataConstantValue(child, true); mask && *mask <= INT32_MAX)
                        return {0, *mask};
        }
        if (expression.kind == SourceExprKind::Unary && expression.args.size() == 1) {
            if (expression.text == "!")
                return {0, 1};
            if (expression.text == "-") {
                const Bounds operand = bounds(expression.args[0]);
                return {-operand.second, -operand.first};
            }
        }
        return word;
    };
    const std::function<SourceExpr(SourceExpr)> unsignedWord = [](SourceExpr expression) {
        if (expression.kind == SourceExprKind::Cast && expression.text == "int32_t" && expression.args.size() == 1 && expression.args[0].cType == "uint32_t")
            return expression.args[0];
        if (expression.cType == "uint32_t")
            return expression;
        return SourceExpr{.kind = SourceExprKind::Cast, .text = "uint32_t", .args = {std::move(expression)}, .cType = "uint32_t"};
    };
    const std::function<void(SourceExpr &)> rewrite = [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Binary || expression.args.size() != 2 || explicitExpressionIsFloat(expression, &unit, &function) ||
            expression.cType == "uint32_t")
            return;
        const bool signedOperands = expression.text == "<" || expression.text == "<=" || expression.text == ">" || expression.text == ">=" ||
            expression.text == "/" || expression.text == "%" || expression.text == ">>";
        if (signedOperands)
            for (size_t index = 0; index < expression.args.size(); ++index)
                if ((expression.text != ">>" || index == 0) && cExpressionIsUnsigned(expression.args[index], unit, function))
                    expression.args[index] =
                        SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {std::move(expression.args[index])}, .cType = "int32_t"};
        const bool arithmetic = expression.text == "+" || expression.text == "-" || expression.text == "*";
        const bool shift = expression.text == "<<" || expression.text == ">>";

        if (arithmetic || !shift || expression.text == "<<")
            return;
        const std::function<bool(const SourceExpr &)> address = [&](const SourceExpr &value) {
            if (value.kind == SourceExprKind::Unary && value.text == "&")
                return true;
            if (value.kind == SourceExprKind::Identifier && (unit.dataSymbols.count(value.text) || unit.functions.count(value.text)))
                return true;
            return value.cType.find('*') != std::string::npos;
        };
        if (address(expression.args[0]) || address(expression.args[1]))
            return;
        if (shift) {
            const Bounds count = bounds(expression.args[1]);
            if (count.first < 0 || count.second > 31)
                expression.args[1] = SourceExpr{.kind = SourceExprKind::Binary,
                    .text = "&",
                    .args = {unsignedWord(std::move(expression.args[1])), SourceExpr{.kind = SourceExprKind::IntLiteral, .text = "31U"}},
                    .cType = "uint32_t"};
            if (expression.text == ">>")
                return;
        } else {
            const Bounds left = bounds(expression.args[0]), right = bounds(expression.args[1]);
            Bounds result = word;
            if (expression.text == "+")
                result = {left.first + right.first, left.second + right.second};
            else if (expression.text == "-")
                result = {left.first - right.second, left.second - right.first};
            else {
                const int64_t products[] = {left.first * right.first, left.first * right.second, left.second * right.first, left.second * right.second};
                result = {products[0], products[0]};
                for (int64_t product : products) {
                    result.first = std::min(result.first, product);
                    result.second = std::max(result.second, product);
                }
            }
            if (result.first >= INT32_MIN && result.second <= INT32_MAX)
                return;
        }
        if (expression.text == "-" && sourceDataConstantValue(expression.args[0], true) == 0U) {
            expression = SourceExpr{.kind = SourceExprKind::Unary, .text = "-", .args = {unsignedWord(std::move(expression.args[1]))}, .cType = "uint32_t"};
        } else {
            expression.args[0] = unsignedWord(std::move(expression.args[0]));
            if (!shift)
                expression.args[1] = unsignedWord(std::move(expression.args[1]));
            expression.cType = "uint32_t";
        }
        expression = SourceExpr{.kind = SourceExprKind::Cast, .text = "int32_t", .args = {std::move(expression)}, .cType = "int32_t"};
    };
    visitExpressions(function.body.body, [&](SourceExpr &expression) { rewrite(expression); });
}

void recoverCVariadicArguments(SourceFunctionAst &function) {
    if (!function.fixedParameterCount)
        return;
    const size_t fixed = *function.fixedParameterCount;
    const std::string cursor = cVariadicCursor(function);
    visitExpressions(function.body.body, [&](SourceExpr &expression) {
        if (expression.kind != SourceExprKind::Identifier)
            return;
        for (size_t index = fixed; index < function.params.size(); ++index)
            if (expression.text == function.params[index]) {
                const std::string type = function.localTypes.at(function.params[index]);
                if (type == "float")
                    throw std::runtime_error("C variadic float parameter has no promoted VM representation");
                expression = SourceExpr{.kind = SourceExprKind::VariadicArgument,
                    .text = std::to_string(8 + index * 4),
                    .args = {SourceExpr{.kind = SourceExprKind::Identifier, .text = cursor}},
                    .cType = type};
                return;
            }
    });
}

bool cAlwaysReturns(const std::vector<SourceStmt> &statements) {
    for (size_t index = statements.size(); index > 0; --index) {
        const SourceStmt &statement = statements[index - 1];
        if (statement.kind == SourceStmtKind::Label)
            continue;
        if (statement.kind == SourceStmtKind::Return)
            return true;
        if (statement.kind == SourceStmtKind::Block)
            return cAlwaysReturns(statement.body);
        if (statement.kind == SourceStmtKind::If)
            return !statement.elseBody.empty() && cAlwaysReturns(statement.body) && cAlwaysReturns(statement.elseBody);
        return false;
    }
    return false;
}

std::set<std::string> discardUnreadCReturns(SourceUnit &unit) {
    std::set<std::string> read;
    const std::function<void(const SourceExpr &, bool)> uses = [&](const SourceExpr &expression, bool statement) {
        if (unit.functions.count(expression.text) && !(statement && expression.kind == SourceExprKind::Call))
            read.insert(expression.text);
        const bool discarded = statement && expression.kind == SourceExprKind::Cast && expression.text == "void";
        for (const SourceExpr &argument : expression.args)
            uses(argument, discarded);
    };
    const std::function<void(const std::vector<SourceStmt> &)> statements = [&](const std::vector<SourceStmt> &list) {
        for (const SourceStmt &statement : list) {
            uses(statement.expr, statement.kind == SourceStmtKind::ExprStmt);
            uses(statement.value, false);
            uses(statement.increment, false);
            statements(statement.initializer);
            statements(statement.body);
            statements(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases) {
                uses(clause.value, false);
                statements(clause.body);
            }
        }
    };
    for (const std::string &name : unit.functionOrder)
        statements(unit.functions.at(name).body.body);
    const std::function<void(std::vector<SourceStmt> &)> discard = [&](std::vector<SourceStmt> &list) {
        for (size_t index = 0; index < list.size(); ++index) {
            discard(list[index].body);
            discard(list[index].elseBody);
            for (SourceSwitchCase &clause : list[index].cases)
                discard(clause.body);
            if (list[index].kind != SourceStmtKind::Return || (list[index].expr.text.empty() && list[index].expr.args.empty()))
                continue;
            SourceExpr value = std::move(list[index].expr);
            list[index].expr = {};
            const bool effect = value.kind == SourceExprKind::Call || (value.kind == SourceExprKind::Binary && value.text == "=");
            SourceStmt statement{.kind = SourceStmtKind::ExprStmt,
                .expr = effect ? std::move(value) : SourceExpr{.kind = SourceExprKind::Cast, .text = "void", .args = {std::move(value)}}};
            list.insert(list.begin() + static_cast<std::ptrdiff_t>(index), std::move(statement));
            ++index;
        }
    };
    for (const std::string &name : unit.functionOrder)
        if (!read.count(name))
            discard(unit.functions.at(name).body.body);

    const std::function<void(std::vector<SourceStmt> &)> keep = [&](std::vector<SourceStmt> &list) {
        for (size_t index = 0; index < list.size(); ++index) {
            keep(list[index].body);
            keep(list[index].elseBody);
            for (SourceSwitchCase &clause : list[index].cases)
                keep(clause.body);
            if (index + 1 >= list.size() || list[index].kind != SourceStmtKind::ExprStmt || list[index + 1].kind != SourceStmtKind::Return ||
                !list[index + 1].expr.text.empty() || !list[index + 1].expr.args.empty())
                continue;
            SourceExpr value = list[index].expr;
            if (value.kind == SourceExprKind::Cast && value.text == "void" && value.args.size() == 1)
                value = value.args[0];
            const abi::CommandSpec *command = value.kind == SourceExprKind::Call ? abiCommandForCallName(value.text) : nullptr;
            const NativePrototype *prototype = command && command->handlerName ? nativePrototype(command->handlerName) : nullptr;
            if (value.kind == SourceExprKind::Call && (!prototype || prototype->result == "void"))
                continue;
            list[index + 1].expr = std::move(value);
            list.erase(list.begin() + static_cast<std::ptrdiff_t>(index));
        }
    };
    for (const std::string &name : unit.functionOrder)
        if (read.count(name))
            keep(unit.functions.at(name).body.body);
    return read;
}

void recoverCValueReturns(SourceFunctionAst &function) {
    const std::function<void(std::vector<SourceStmt> &)> directReturns = [&](std::vector<SourceStmt> &statements) {
        for (size_t index = 0; index < statements.size(); ++index) {
            SourceStmt &statement = statements[index];
            directReturns(statement.body);
            directReturns(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                directReturns(clause.body);
            if (index + 1 == statements.size() || statement.kind != SourceStmtKind::Assign || statement.expr.kind != SourceExprKind::Identifier ||
                statement.expr.text != "eax" || statements[index + 1].kind != SourceStmtKind::Return ||
                !(statements[index + 1].expr.text.empty() && statements[index + 1].expr.args.empty()))
                continue;
            statement.kind = SourceStmtKind::Return;
            statement.expr = std::move(statement.value);
            statement.value = {};
            statements.erase(statements.begin() + static_cast<std::ptrdiff_t>(index + 1));
        }
    };
    directReturns(function.body.body);
    std::vector<SourceStmt> &body = function.body.body;
    if (body.empty() || body.back().kind != SourceStmtKind::Return || !(body.back().expr.text.empty() && body.back().expr.args.empty()))
        return;
    size_t epilogue = body.size() - 1;
    std::set<std::string> labels;
    while (epilogue && body[epilogue - 1].kind == SourceStmtKind::Label)
        labels.insert(body[--epilogue].label);
    if (labels.empty())
        return;
    SourceFunctionAst candidate = function;
    const std::function<void(std::vector<SourceStmt> &)> rewrite = [&](std::vector<SourceStmt> &statements) {
        for (size_t index = 0; index < statements.size(); ++index) {
            SourceStmt &statement = statements[index];
            rewrite(statement.body);
            rewrite(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                rewrite(clause.body);
            if (index + 1 < statements.size() && statement.kind == SourceStmtKind::Assign && statement.expr.kind == SourceExprKind::Identifier &&
                statement.expr.text == "eax" && statements[index + 1].kind == SourceStmtKind::Goto && labels.count(statements[index + 1].label)) {
                statement.kind = SourceStmtKind::Return;
                statement.expr = statement.value;
                statement.value = {};
                statements.erase(statements.begin() + static_cast<std::ptrdiff_t>(index + 1));
            }
        }
    };
    candidate.body.body.resize(epilogue);
    rewrite(candidate.body.body);
    bool referenced = false;
    const std::function<void(const std::vector<SourceStmt> &)> inspect = [&](const std::vector<SourceStmt> &statements) {
        for (const SourceStmt &statement : statements) {
            if (statement.kind == SourceStmtKind::Goto && labels.count(statement.label))
                referenced = true;
            inspect(statement.body);
            inspect(statement.elseBody);
            for (const SourceSwitchCase &clause : statement.cases)
                inspect(clause.body);
        }
    };
    inspect(candidate.body.body);
    if (!referenced && cAlwaysReturns(candidate.body.body))
        function = std::move(candidate);
}

void recoverCRegisterExpressions(std::vector<SourceStmt> &statements) {
    for (size_t index = 0; index < statements.size(); ++index) {
        SourceStmt &statement = statements[index];
        recoverCRegisterExpressions(statement.body);
        recoverCRegisterExpressions(statement.elseBody);
        for (SourceSwitchCase &clause : statement.cases)
            recoverCRegisterExpressions(clause.body);
        if (statement.kind != SourceStmtKind::Assign || statement.expr.kind != SourceExprKind::Identifier || statement.expr.text != "eax")
            continue;
        if (index + 1 < statements.size() && statements[index + 1].kind == SourceStmtKind::If) {
            SourceExpr &condition = statements[index + 1].expr;
            SourceExpr *use = &condition;
            if (condition.kind == SourceExprKind::Unary && condition.text == "!" && condition.args.size() == 1)
                use = &condition.args[0];
            if (use->kind == SourceExprKind::Identifier && use->text == "eax") {
                *use = statement.value;
                statements.erase(statements.begin() + static_cast<std::ptrdiff_t>(index));
                --index;
                continue;
            }
        }

        const bool address = statement.value.kind == SourceExprKind::Call && statement.value.text == "TASK_FN";
        if (statement.value.kind != SourceExprKind::Call || address) {
            statement.kind = SourceStmtKind::ExprStmt;
            statement.expr =
                address ? SourceExpr{.kind = SourceExprKind::Cast, .text = "void", .args = {std::move(statement.value)}} : std::move(statement.value);
            statement.value = {};
        }
    }
}

void recoverCForwardBranches(std::vector<SourceStmt> &statements, std::map<std::string, uint32_t> &references) {
    for (SourceStmt &statement : statements) {
        recoverCForwardBranches(statement.initializer, references);
        recoverCForwardBranches(statement.body, references);
        recoverCForwardBranches(statement.elseBody, references);
        for (SourceSwitchCase &clause : statement.cases)
            recoverCForwardBranches(clause.body, references);
    }
    const auto negated = [](SourceExpr condition) {
        if (condition.kind == SourceExprKind::Unary && condition.text == "!" && condition.args.size() == 1) {
            SourceExpr inner = std::move(condition.args[0]);
            return inner;
        }
        return SourceExpr{.kind = SourceExprKind::Unary, .text = "!", .args = {std::move(condition)}};
    };
    const auto branch = [](const SourceStmt &statement) {
        return statement.kind == SourceStmtKind::If && statement.elseBody.empty() && statement.body.size() == 1 &&
            statement.body.front().kind == SourceStmtKind::Goto;
    };
    for (size_t index = 0; index < statements.size(); ++index) {
        SourceStmt &statement = statements[index];

        if (statement.kind == SourceStmtKind::If && !statement.elseBody.empty() && index + 1 < statements.size() &&
            statements[index + 1].kind == SourceStmtKind::Label && statement.elseBody.size() >= 2 && branch(statement.elseBody.front()) &&
            statement.elseBody.back().kind == SourceStmtKind::Goto) {
            const std::string &label = statements[index + 1].label;
            if (statement.elseBody.front().body.front().label == label && statement.elseBody.back().label == label && references[label] == 2) {
                SourceStmt arm{.kind = SourceStmtKind::If, .expr = negated(std::move(statement.elseBody.front().expr))};
                arm.body.assign(std::make_move_iterator(statement.elseBody.begin() + 1), std::make_move_iterator(statement.elseBody.end() - 1));
                bool declares = false;
                for (const SourceStmt &moved : arm.body)
                    declares |= moved.kind == SourceStmtKind::Declaration;
                if (!declares) {
                    statement.elseBody.clear();
                    statement.elseBody.push_back(std::move(arm));
                    statement.elseIf = true;
                    references.erase(label);
                    statements.erase(statements.begin() + static_cast<std::ptrdiff_t>(index + 1));
                    continue;
                }
            }
        }
        if (!branch(statement))
            continue;

        const std::string label = statement.body.front().label;
        size_t first = index + 1;
        while (first < statements.size() && branch(statements[first]) && statements[first].body.front().label == label)
            ++first;
        if (references[label] != first - index)
            continue;
        size_t target = first;
        bool declares = false;
        while (target < statements.size() && !(statements[target].kind == SourceStmtKind::Label && statements[target].label == label)) {
            declares |= statements[target].kind == SourceStmtKind::Declaration;
            ++target;
        }
        if (target == statements.size() || declares || target == first)
            continue;
        SourceExpr condition = negated(std::move(statement.expr));
        for (size_t operand = index + 1; operand < first; ++operand)
            condition = SourceExpr{.kind = SourceExprKind::Binary, .text = "&&", .args = {std::move(condition), negated(std::move(statements[operand].expr))}};
        SourceStmt guarded{.kind = SourceStmtKind::If, .expr = std::move(condition)};
        guarded.body.assign(std::make_move_iterator(statements.begin() + static_cast<std::ptrdiff_t>(first)),
            std::make_move_iterator(statements.begin() + static_cast<std::ptrdiff_t>(target)));
        statements.erase(statements.begin() + static_cast<std::ptrdiff_t>(index + 1), statements.begin() + static_cast<std::ptrdiff_t>(target + 1));
        statement = std::move(guarded);
        references.erase(label);
        recoverCForwardBranches(statement.body, references);
    }
}

void recoverCLiteralRadix(std::vector<SourceStmt> &statements) {
    const auto bitwise = [](const SourceExpr &expression) {
        return (expression.kind == SourceExprKind::Binary && (expression.text == "&" || expression.text == "|" || expression.text == "^")) ||
            (expression.kind == SourceExprKind::Unary && expression.text == "~");
    };
    const auto pattern = [](const std::string &name) {
        std::string lower;
        for (char character : name)
            lower += static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
        return lower.find("flag") != std::string::npos || lower.find("mask") != std::string::npos || lower.find("bits") != std::string::npos;
    };
    const std::function<void(SourceExpr &, bool)> literal = [&](SourceExpr &expression, bool bits) {
        if (expression.kind == SourceExprKind::IntLiteral) {
            const std::string &text = expression.text;
            const size_t start = text.starts_with("-") ? 1 : 0;
            if (text.compare(start, 2, "0x") != 0 && text.compare(start, 2, "0X") != 0)
                return;
            size_t end = start + 2;
            while (end < text.size() && std::isxdigit(static_cast<unsigned char>(text[end])))
                ++end;
            if (end == start + 2)
                return;
            const uint64_t value = std::stoull(text.substr(start + 2, end - start - 2), nullptr, 16);
            uint64_t significant = value;
            uint32_t zeros = 0;
            while (significant && !(significant & 0xFU)) {
                significant >>= 4;
                ++zeros;
            }
            const bool round = value >= 0x100U && zeros >= 2 && significant <= 0xFFU;
            const bool mask = value >= 0xFFFU && !((value + 1) & value);
            bool name = value >= 0x1000000U, letter = false;
            for (uint32_t byte = 0; name && byte < 4; ++byte) {
                const char character = static_cast<char>(value >> (byte * 8U));
                letter |= character >= 'A' && character <= 'Z';
                name &= (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_';
            }
            if (bits || round || mask || (name && letter) || value >= 0x100000U)
                return;
            expression.text = text.substr(0, start) + std::to_string(value) + text.substr(end);
            return;
        }
        if (bitwise(expression) || (expression.kind == SourceExprKind::Binary && expression.text == "<<" && expression.args.size() == 2)) {
            literal(expression.args[0], true);
            if (expression.args.size() > 1)
                literal(expression.args[1], expression.text != "<<");
            return;
        }
        if (expression.kind == SourceExprKind::Binary && (expression.text == "==" || expression.text == "!=") && expression.args.size() == 2) {
            const bool masked = bitwise(expression.args[0]) || bitwise(expression.args[1]);
            literal(expression.args[0], masked);
            literal(expression.args[1], masked);
            return;
        }
        if (expression.kind == SourceExprKind::Call) {
            const abi::CommandSpec *command = abiCommandForCallName(expression.text);
            std::vector<std::optional<uint32_t>> observed;
            for (const SourceExpr &argument : expression.args)
                observed.push_back(sourceDataConstantValue(argument, true));
            for (size_t index = 0; index < expression.args.size(); ++index) {
                const NativeParameter argument = command && command->handlerName ? nativeParameter(command->handlerName, index, observed) : NativeParameter{};
                literal(expression.args[index], pattern(std::string(argument.name)));
            }
            return;
        }

        const bool through = expression.kind == SourceExprKind::Cast;
        for (SourceExpr &child : expression.args)
            literal(child, through && bits);
    };
    for (SourceStmt &statement : statements) {
        literal(statement.expr, false);
        literal(statement.value, false);
        literal(statement.increment, false);
        const bool masked = statement.kind == SourceStmtKind::Switch && bitwise(statement.expr);
        for (SourceSwitchCase &clause : statement.cases) {
            literal(clause.value, masked);
            recoverCLiteralRadix(clause.body);
        }
        recoverCLiteralRadix(statement.initializer);
        recoverCLiteralRadix(statement.body);
        recoverCLiteralRadix(statement.elseBody);
    }
}

void recoverCStatementSyntax(std::vector<SourceStmt> &statements) {
    for (SourceStmt &statement : statements) {
        if (statement.kind == SourceStmtKind::While && statement.expr.kind == SourceExprKind::Identifier && statement.expr.text == "true") {
            statement.kind = SourceStmtKind::For;
            statement.expr = {};
        }
        if (statement.kind == SourceStmtKind::ExprStmt && statement.expr.kind != SourceExprKind::Call)
            statement.expr = SourceExpr{.kind = SourceExprKind::Cast, .text = "void", .args = {std::move(statement.expr)}};
        recoverCStatementSyntax(statement.body);
        recoverCStatementSyntax(statement.elseBody);
        for (SourceSwitchCase &clause : statement.cases)
            recoverCStatementSyntax(clause.body);
    }
}

void discardUnusedCLabels(SourceFunctionAst &function) {
    std::set<std::string> referenced;
    const std::function<void(std::vector<SourceStmt> &, bool)> walk = [&](std::vector<SourceStmt> &body, bool erase) {
        for (size_t index = 0; index < body.size();) {
            SourceStmt &statement = body[index];
            if (statement.kind == SourceStmtKind::Goto)
                referenced.insert(statement.label);
            if (erase && statement.kind == SourceStmtKind::Label && !referenced.count(statement.label)) {
                body.erase(body.begin() + static_cast<std::ptrdiff_t>(index));
                continue;
            }
            walk(statement.body, erase);
            walk(statement.elseBody, erase);
            for (SourceSwitchCase &clause : statement.cases)
                walk(clause.body, erase);
            ++index;
        }
    };
    walk(function.body.body, false);
    walk(function.body.body, true);
}

std::vector<SourceStmt> cVariadicBody(const SourceFunctionAst &function) {
    const size_t fixed = *function.fixedParameterCount;
    std::vector<SourceStmt> body = function.body.body;
    uint32_t cursorIndex = 0;
    const std::function<void(std::vector<SourceStmt> &)> rewrite = [&](std::vector<SourceStmt> &statements) {
        std::vector<SourceStmt> result;
        for (SourceStmt &statement : statements) {
            rewrite(statement.body);
            rewrite(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                rewrite(clause.body);
            std::vector<SourceStmt> prepare, finish;
            const std::function<void(SourceExpr &)> argument = [&](SourceExpr &expression) {
                if (expression.kind == SourceExprKind::VariadicArgument) {

                    if (statement.kind != SourceStmtKind::Assign && statement.kind != SourceStmtKind::ExprStmt && statement.kind != SourceStmtKind::If &&
                        statement.kind != SourceStmtKind::Return && statement.kind != SourceStmtKind::Switch)
                        throw std::runtime_error("Optional argument has no bounded C lifetime in " + function.name);
                    std::string cursor;
                    do {
                        cursor = "arguments" + std::to_string(cursorIndex++);
                    } while (function.localTypes.count(cursor));
                    prepare.push_back(
                        SourceStmt{.kind = SourceStmtKind::Declaration, .declaration = SourceLocalDeclaration{.type = "va_list", .name = cursor}});
                    prepare.push_back(expressionStatement(call("va_start", {identifier(cursor), identifier(function.params[fixed - 1])})));
                    const size_t index = (static_cast<size_t>(std::stoul(expression.text)) - 8) / 4;
                    for (size_t skip = fixed; skip < index; ++skip) {
                        const SourceExpr value{.kind = SourceExprKind::VariadicArgument,
                            .text = std::to_string(8 + skip * 4),
                            .args = {identifier(cursor)},
                            .cType = function.localTypes.at(function.params[skip])};
                        prepare.push_back(expressionStatement(SourceExpr{.kind = SourceExprKind::Cast, .text = "void", .args = {value}}));
                    }
                    expression.args = {identifier(cursor)};
                    finish.push_back(expressionStatement(call("va_end", {identifier(cursor)})));
                    return;
                }
                for (SourceExpr &child : expression.args)
                    argument(child);
            };
            argument(statement.expr);
            argument(statement.value);
            if (!prepare.empty()) {
                SourceStmt scoped{.kind = SourceStmtKind::Block, .body = std::move(prepare)};
                scoped.body.push_back(std::move(statement));
                scoped.body.insert(scoped.body.end(), finish.begin(), finish.end());
                result.push_back(std::move(scoped));
            } else
                result.push_back(std::move(statement));
        }
        statements = std::move(result);
    };
    rewrite(body);
    return body;
}

} // namespace shenmue::scenario::decompile
