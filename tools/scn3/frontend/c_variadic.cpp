#include "scn3/frontend/c_frontend.hpp"

namespace shenmue::scenario::c_frontend {

uint32_t CVariadicAnalysis::declaration(const CNode &node) {
    if (node.kind == CKind::Reference)
        return node.target;
    if ((node.kind == CKind::Paren || node.kind == CKind::ImplicitCast) && node.children.size() == 1)
        return declaration(node.children[0]);
    return 0;
}

size_t CVariadicAnalysis::append(Operation operation) {
    graph.push_back(std::move(operation));
    return graph.size() - 1;
}

size_t CVariadicAnalysis::expression(const CNode &node, size_t next) {
    const std::vector<CNode> &inner = node.children;
    if (node.kind == CKind::Reference && cursors.count(declaration(node)))
        throw std::runtime_error("C va_list is used outside va_start, va_arg or va_end");
    if (node.kind == CKind::VariadicArgument) {
        const uint32_t cursor = declaration(inner.at(0));
        if (!cursors.count(cursor))
            throw std::runtime_error("C va_arg requires a local standard va_list");
        const ObjectType type = objectType(node.type);
        if (type.array || type.size != 4 || (!type.pointer && type.element != "int32_t" && type.element != "uint32_t"))
            throw std::runtime_error("C va_arg requires a promoted integer or pointer VM word");
        return append({Operation::Read, cursor, 0, node.id, {next}});
    }
    if (node.kind == CKind::Call && !inner.empty() && builtins.count(declaration(inner[0]))) {
        const CNode *callee = &inner[0];
        while (callee->kind != CKind::Reference)
            callee = &callee->children.at(0);
        const std::string &name = callee->name;
        Operation operation;
        if (name == "__builtin_va_start") {
            if (inner.size() != 3 || declaration(inner[2]) != lastParameter || !firstOffset)
                throw std::runtime_error("C va_start requires the last fixed parameter of a variadic function");
            operation.kind = Operation::Start;
        } else if (name == "__builtin_va_end")
            operation.kind = Operation::End;
        else if (name == "__builtin_va_copy") {
            operation.kind = Operation::Copy;
            operation.source = declaration(inner.at(2));
            if (!cursors.count(operation.source))
                throw std::runtime_error("C va_copy source is not a local va_list");
        } else
            throw std::runtime_error("Unsupported C variadic builtin");
        operation.cursor = declaration(inner.at(1));
        operation.edges = {next};
        if (!cursors.count(operation.cursor))
            throw std::runtime_error("C variadic builtin requires a local va_list");
        erasedCalls.insert(node.id);
        return append(std::move(operation));
    }
    if (node.kind == CKind::Binary && (node.text == "&&" || node.text == "||")) {
        const size_t right = expression(inner.at(1), next);
        const size_t branch = append({Operation::Pass, 0, 0, 0, {right, next}});
        return expression(inner.at(0), branch);
    }

    if (node.kind == CKind::Call) {
        for (size_t index = 1; index < inner.size(); ++index)
            next = expression(inner[index], next);
        return expression(inner.at(0), next);
    }
    for (size_t index = inner.size(); index > 0; --index)
        next = expression(inner[index - 1], next);
    return next;
}

size_t CVariadicAnalysis::statement(const CNode &node, size_t next, size_t breakTarget, size_t continueTarget) {
    const std::vector<CNode> &inner = node.children;
    if (node.kind == CKind::Compound) {
        for (size_t index = inner.size(); index > 0; --index)
            next = statement(inner[index - 1], next, breakTarget, continueTarget);
        return next;
    }
    if (node.kind == CKind::If) {
        const size_t yes = statement(inner.at(1), next, breakTarget, continueTarget);
        const size_t no = inner.size() > 2 ? statement(inner.at(2), next, breakTarget, continueTarget) : next;
        return expression(inner.at(0), append({Operation::Pass, 0, 0, 0, {yes, no}}));
    }
    if (node.kind == CKind::While || node.kind == CKind::For) {
        const size_t head = append({});
        const size_t body = statement(inner.back(), head, next, head);
        if (node.kind == CKind::For) {
            if (inner.size() != 5 || inner[0].kind != CKind::None || inner[1].kind != CKind::None || inner[2].kind != CKind::None ||
                inner[3].kind != CKind::None)
                throw std::runtime_error("C variadic analysis requires a lowered for-loop");
            graph[head].edges = {body};
        } else {
            const size_t condition = expression(inner[0], append({Operation::Pass, 0, 0, 0, {body, next}}));
            graph[head].edges = {condition};
        }
        return head;
    }
    if (node.kind == CKind::Switch) {
        std::vector<size_t> targets;
        bool hasDefault = false;
        const std::function<void(const CNode &)> collect = [&](const CNode &child) {
            if (child.kind == CKind::Switch)
                return;
            if (child.kind == CKind::Case || child.kind == CKind::Default) {
                const size_t entry = append({});
                cases[child.id] = entry;
                targets.push_back(entry);
                if (child.kind == CKind::Default)
                    hasDefault = true;
            }
            for (const CNode &nested : child.children)
                collect(nested);
        };
        collect(inner.at(1));
        statement(inner.at(1), next, next, continueTarget);
        if (!hasDefault)
            targets.push_back(next);
        return expression(inner.at(0), append({Operation::Pass, 0, 0, 0, std::move(targets)}));
    }
    if (node.kind == CKind::Case || node.kind == CKind::Default) {
        const size_t entry = cases.at(node.id);
        graph[entry].edges = {statement(inner.back(), next, breakTarget, continueTarget)};
        return entry;
    }
    if (node.kind == CKind::Break)
        return breakTarget;
    if (node.kind == CKind::Continue)
        return continueTarget;
    if (node.kind == CKind::Return)
        return inner.empty() ? 0 : expression(inner[0], 0);
    if (node.kind == CKind::Goto)
        return labels.at(node.target);
    if (node.kind == CKind::Label) {
        const size_t label = labels.at(node.target);
        graph[label].edges = {statement(inner[0], next, breakTarget, continueTarget)};
        return label;
    }
    return expression(node, next);
}

void CVariadicAnalysis::analyze(const CNode &body, const std::set<uint32_t> &vaTypes, size_t fixed, uint32_t last) {
    const std::function<void(const CNode &)> collect = [&](const CNode &node) {
        if (node.kind == CKind::Variable && vaTypes.count(node.type.alias)) {
            if (node.initialized)
                throw std::runtime_error("C va_list initialization requires va_start or va_copy");
            cursors.insert(node.id);
        }
        for (const CNode &child : node.children)
            collect(child);
    };
    collect(body);
    if (cursors.empty())
        return;
    firstOffset = static_cast<uint32_t>(8 + fixed * 4);
    lastParameter = last;
    append({Operation::Exit});
    const std::function<void(const CNode &)> collectLabels = [&](const CNode &node) {
        if (node.kind == CKind::Label)
            labels[node.target] = append({});
        for (const CNode &child : node.children)
            collectLabels(child);
    };
    collectLabels(body);
    const size_t entry = statement(body, 0, 0, 0);
    using State = std::map<uint32_t, Cursor>;
    std::vector<std::optional<State>> input(graph.size());
    State initial;
    for (const uint32_t cursor : cursors)
        initial[cursor] = {};
    input[entry] = initial;
    std::vector<size_t> pending{entry};
    for (size_t at = 0; at < pending.size(); ++at) {
        const size_t index = pending[at];
        const Operation &operation = graph[index];
        State state = *input[index];
        if (operation.kind == Operation::Start)
            state[operation.cursor].offset = static_cast<int32_t>(firstOffset);
        else if (operation.kind == Operation::End)
            state[operation.cursor].offset = -1;
        else if (operation.kind == Operation::Copy)
            state[operation.cursor] = state[operation.source];
        else if (operation.kind == Operation::Read && state[operation.cursor].offset >= 0)
            state[operation.cursor].offset += 4;
        for (size_t edge : operation.edges) {
            if (!input[edge]) {
                input[edge] = state;
                pending.push_back(edge);
                continue;
            }
            State joined = *input[edge];
            for (const uint32_t cursor : cursors) {
                const int32_t left = joined[cursor].offset, right = state[cursor].offset;
                if (left != right)
                    joined[cursor].offset = left == -3 || right == -3 || left == -1 || right == -1 ? -3 : -2;
            }
            if (joined != *input[edge]) {
                input[edge] = std::move(joined);
                pending.push_back(edge);
            }
        }
    }
    for (size_t index = 0; index < graph.size(); ++index) {
        if (!input[index])
            continue;
        const Operation &operation = graph[index];
        const State &state = *input[index];
        const int32_t offset = operation.cursor == 0 ? -1 : state.at(operation.cursor).offset;
        if ((operation.kind == Operation::Start || operation.kind == Operation::Copy) && offset != -1)
            throw std::runtime_error("C va_list is restarted or copied before va_end");
        if ((operation.kind == Operation::Read || operation.kind == Operation::End) && (offset == -1 || offset == -3))
            throw std::runtime_error("C va_list operation is not dominated by va_start");
        if (operation.kind == Operation::Read) {
            if (offset < 0)
                throw std::runtime_error("C va_arg position depends on runtime control flow");
            if (offset > 32764)
                throw std::runtime_error("C va_arg exceeds the VM parameter displacement");
            reads[operation.expression] = static_cast<uint32_t>(offset);
        }
        if (operation.kind == Operation::Copy && (state.at(operation.source).offset == -1 || state.at(operation.source).offset == -3))
            throw std::runtime_error("C va_copy source is not active");
        if (operation.kind == Operation::Exit)
            for (const std::pair<const uint32_t, Cursor> &cursor : state)
                if (cursor.second.offset != -1)
                    throw std::runtime_error("C function exits without va_end");
    }
}

} // namespace shenmue::scenario::c_frontend
