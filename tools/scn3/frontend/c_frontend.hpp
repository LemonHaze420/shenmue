#pragma once

#include "scn3/frontend/c_ast.hpp"

namespace shenmue::scenario::c_frontend {

struct ObjectType {
    std::string element;
    uint32_t count = 1;
    bool array = false;
    bool pointer = false;
    bool functionPointer = false;
    uint32_t size = 0;
    uint32_t alignment = 0;
};

std::string cTypeName(std::string name);

ObjectType objectType(const CType &type);

struct CVariadicAnalysis {
    struct Operation {
        enum Kind { Pass, Start, Read, End, Copy, Exit } kind = Pass;
        uint32_t cursor = 0;
        uint32_t source = 0;
        uint32_t expression = 0;
        std::vector<size_t> edges{};
    };
    struct Cursor {

        int32_t offset = -1;
        bool operator==(const Cursor &) const = default;
    };
    std::set<uint32_t> cursors;
    std::map<uint32_t, uint32_t> reads;
    std::set<uint32_t> builtins;
    std::set<uint32_t> erasedCalls;
    std::vector<Operation> graph;
    std::map<uint32_t, size_t> labels;
    std::map<uint32_t, size_t> cases;
    uint32_t firstOffset = 0;
    uint32_t lastParameter = 0;

    static uint32_t declaration(const CNode &node);

    size_t append(Operation operation);

    size_t expression(const CNode &node, size_t next);

    size_t statement(const CNode &node, size_t next, size_t breakTarget, size_t continueTarget);

    void analyze(const CNode &body, const std::set<uint32_t> &vaTypes, size_t fixed, uint32_t last);
};

} // namespace shenmue::scenario::c_frontend
