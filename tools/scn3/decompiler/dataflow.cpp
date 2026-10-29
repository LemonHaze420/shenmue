#include "scn3/decompiler/cfg.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/lookup.hpp"
#include "scn3/native/prototypes.hpp"

namespace shenmue::scenario::decompile {

namespace {

template <typename Visit> void walkExpression(SourceExpr &expression, const Visit &visit) {
    if (expression.kind == SourceExprKind::Binary && expression.text == "=" && expression.args.size() == 2 &&
        expression.args[0].kind == SourceExprKind::Identifier) {
        walkExpression(expression.args[1], visit);
        visit(expression.args[0], true);
        return;
    }
    if (expression.kind == SourceExprKind::Identifier) {
        visit(expression, false);
        return;
    }
    for (SourceExpr &child : expression.args)
        walkExpression(child, visit);
}

template <typename Visit> void walkStatement(SourceStmt &statement, const Visit &visit) {
    walkExpression(statement.value, visit);
    if (statement.kind == SourceStmtKind::Assign && statement.expr.kind == SourceExprKind::Identifier && statement.expr.text != "eax")
        visit(statement.expr, true);
    else
        walkExpression(statement.expr, visit);
    for (SourceStmt &nested : statement.body)
        walkStatement(nested, visit);
}

template <typename Visit> void walkBlock(BasicBlock &block, const Visit &visit) {
    for (SourceStmt &statement : block.body)
        walkStatement(statement, visit);
    for (SourceStmt &statement : block.exit)
        walkStatement(statement, visit);
}

int32_t frameOffset(const SourceSlotRef &slot) {
    const int32_t base = slot.baseOffset <= 0xFFU ? static_cast<int8_t>(slot.baseOffset) : static_cast<int16_t>(slot.baseOffset);
    return base + static_cast<int32_t>(slot.byteOffset - slot.baseOffset);
}

std::optional<std::pair<int32_t, int32_t>> frameExtent(const std::string &name) {
    const std::string base = name.substr(0, name.find('.'));
    std::optional<SourceSlotRef> slot = localSlotRefFromName(name);
    if (!slot)
        slot = localSlotRefFromName(base);
    if (!slot)
        return std::nullopt;
    const int32_t start = frameOffset(*slot);
    const int32_t width = name.ends_with(".u8") ? 1 : name.ends_with(".u16") ? 2 : name.find('.') == std::string::npos && !slot->aggregateField ? 4 : 16;
    return std::pair{start, start + width};
}

struct UnionFind {
    std::vector<size_t> parent;
    size_t find(size_t node) {
        while (parent[node] != node)
            node = parent[node] = parent[parent[node]];
        return node;
    }
    void unite(size_t left, size_t right) {
        parent[find(left)] = find(right);
    }
};

struct Access {
    size_t object;
    std::vector<size_t> keys;
    bool store;
    SourceExpr *name;
    const ScenarioDataSchema *schema = nullptr;
    bool reads = false;
    const SourceExpr *through = nullptr;
};

struct Webs {
    std::vector<size_t> ofAccess;
    std::vector<size_t> entry;
    std::vector<std::vector<size_t>> others;
};

Webs accessWebs(const ControlFlowGraph &graph, const std::vector<std::vector<Access>> &accesses, const std::vector<size_t> &keyObject, size_t objects) {
    const std::vector<BasicBlock> &blocks = graph.blocks;
    const size_t keys = keyObject.size();
    struct Definition {
        size_t key;
        size_t store;
    };
    std::vector<Definition> definitions;
    for (size_t key = 0; key < keys; ++key)
        definitions.push_back(Definition{.key = key, .store = SIZE_MAX});
    std::vector<std::vector<std::vector<size_t>>> stored(blocks.size());
    size_t stores = 0;
    for (size_t index = 0; index < blocks.size(); ++index)
        for (const Access &access : accesses[index]) {
            stored[index].emplace_back();
            if (!access.store)
                continue;
            for (size_t key : access.keys) {
                stored[index].back().push_back(definitions.size());
                definitions.push_back(Definition{.key = key, .store = stores});
            }
            ++stores;
        }
    const size_t words64 = (definitions.size() + 63U) / 64U;
    using Bits = std::vector<uint64_t>;
    const auto set = [](Bits &bits, size_t bit) { bits[bit / 64U] |= uint64_t{1} << (bit % 64U); };
    const auto test = [](const Bits &bits, size_t bit) { return (bits[bit / 64U] >> (bit % 64U)) & 1U; };
    std::vector<Bits> killed(keys, Bits(words64));
    std::vector<std::vector<size_t>> defining(keys);
    for (size_t definition = 0; definition < definitions.size(); ++definition) {
        set(killed[definitions[definition].key], definition);
        defining[definitions[definition].key].push_back(definition);
    }
    const auto define = [&](Bits &live, const std::vector<size_t> &created) {
        for (size_t definition : created) {
            const Bits &kill = killed[definitions[definition].key];
            for (size_t word = 0; word < words64; ++word)
                live[word] &= ~kill[word];
        }
        for (size_t definition : created)
            set(live, definition);
    };
    std::vector<Bits> in(blocks.size(), Bits(words64)), out(blocks.size(), Bits(words64));
    for (size_t key = 0; key < keys; ++key)
        set(in[0], key);
    for (bool changed = true; changed;) {
        changed = false;
        for (size_t index = 0; index < blocks.size(); ++index) {
            Bits entering = index ? Bits(words64) : in[0];
            for (size_t predecessor : blocks[index].predecessors)
                for (size_t word = 0; word < words64; ++word)
                    entering[word] |= out[predecessor][word];
            in[index] = std::move(entering);
            Bits leaving = in[index];
            for (const std::vector<size_t> &created : stored[index])
                define(leaving, created);
            if (leaving != out[index]) {
                out[index] = std::move(leaving);
                changed = true;
            }
        }
    }
    UnionFind webs{.parent = std::vector<size_t>(definitions.size())};
    for (size_t definition = 0; definition < definitions.size(); ++definition)
        webs.parent[definition] = definition;
    std::vector<size_t> firstKey(objects, SIZE_MAX);
    for (size_t key = 0; key < keys; ++key) {
        if (firstKey[keyObject[key]] == SIZE_MAX)
            firstKey[keyObject[key]] = key;
        webs.unite(key, firstKey[keyObject[key]]);
    }
    std::vector<size_t> owners;
    std::vector<bool> read(definitions.size());
    for (size_t index = 0; index < blocks.size(); ++index) {
        Bits live = in[index];
        for (size_t position = 0; position < accesses[index].size(); ++position) {
            const Access &access = accesses[index][position];
            const std::vector<size_t> &created = stored[index][position];
            if (access.store && !access.reads) {
                for (size_t definition : created)
                    webs.unite(definition, created[0]);
                define(live, created);
                owners.push_back(created[0]);
                continue;
            }

            bool written = false;
            for (size_t key : access.keys)
                for (size_t definition : defining[key])
                    written |= definition >= keys && test(live, definition);
            size_t owner = access.store ? created[0] : firstKey[access.object];
            bool first = true;
            for (size_t key : access.keys)
                for (size_t definition : defining[key])
                    if (test(live, definition) && (definition >= keys || !written)) {
                        read[definition] = true;
                        if (first && !access.store)
                            owner = definition;
                        else
                            webs.unite(definition, owner);
                        first = false;
                    }
            if (access.store) {
                for (size_t definition : created)
                    webs.unite(definition, owner);
                define(live, created);
            }
            owners.push_back(owner);
        }
    }
    std::vector<bool> live(definitions.size());
    for (size_t definition = 0; definition < definitions.size(); ++definition)
        if (read[definition])
            live[webs.find(definition)] = true;
    for (size_t definition = keys; definition < definitions.size(); ++definition) {
        if (live[webs.find(definition)])
            continue;
        const size_t object = keyObject[definitions[definition].key];
        size_t later = SIZE_MAX, earlier = SIZE_MAX;
        for (size_t other = keys; other < definitions.size(); ++other)
            if (keyObject[definitions[other].key] == object && live[webs.find(other)] && definitions[other].store != definitions[definition].store) {
                if (other > definition && later == SIZE_MAX)
                    later = other;
                if (other < definition)
                    earlier = other;
            }
        webs.unite(definition, later != SIZE_MAX ? later : earlier != SIZE_MAX ? earlier : firstKey[object]);
    }
    Webs result{.ofAccess = {}, .entry = std::vector<size_t>(objects), .others = std::vector<std::vector<size_t>>(objects)};
    for (size_t object = 0; object < objects; ++object)
        result.entry[object] = firstKey[object] == SIZE_MAX ? SIZE_MAX : webs.find(firstKey[object]);
    for (size_t definition = keys; definition < definitions.size(); ++definition) {
        const size_t object = keyObject[definitions[definition].key], web = webs.find(definition);
        bool known = web == result.entry[object];
        for (size_t other : result.others[object])
            known |= other == web;
        if (!known)
            result.others[object].push_back(web);
    }
    for (size_t owner : owners)
        result.ofAccess.push_back(webs.find(owner));
    return result;
}

bool renameWebs(ControlFlowGraph &graph, std::vector<std::vector<Access>> &accesses, const Webs &webs, const std::vector<std::string> &names) {
    std::set<size_t> named;
    size_t position = 0;
    for (const std::vector<Access> &block : accesses)
        for (const Access &access : block)
            if (const size_t web = webs.ofAccess[position++]; access.name)
                named.insert(web);
    std::map<size_t, std::string> renamed;
    for (size_t object = 0; object < names.size(); ++object) {
        const std::string base = stripSourceWidthSuffix(names[object]), suffix = names[object].substr(base.size());
        uint32_t version = 1;
        for (size_t web : webs.others[object]) {
            if (!named.count(web))
                continue;
            std::string name;
            do
                name = base + "v" + std::to_string(++version) + suffix;
            while (graph.references.count(name));
            renamed.emplace(web, name);
        }
    }
    size_t index = 0;
    for (std::vector<Access> &block : accesses)
        for (Access &access : block) {
            const std::map<size_t, std::string>::const_iterator version = renamed.find(webs.ofAccess[index++]);
            if (version != renamed.end() && access.name)
                access.name->text = version->second;
        }
    return !renamed.empty();
}

} // namespace

void splitFrameVariables(ControlFlowGraph &graph) {
    std::vector<BasicBlock> &blocks = graph.blocks;

    std::map<std::string, std::pair<int32_t, int32_t>> words;
    std::map<std::string, std::set<std::string>> spellings;
    std::set<std::string> excluded;
    const std::function<void(const SourceExpr &)> scan = [&](const SourceExpr &expression) {
        if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1 &&
            expression.args[0].kind == SourceExprKind::Identifier)
            excluded.insert(expression.args[0].text.substr(0, expression.args[0].text.find('.')));
        if (expression.kind == SourceExprKind::Identifier)
            if (const std::optional<std::pair<int32_t, int32_t>> extent = frameExtent(expression.text)) {
                const std::optional<SourceSlotRef> slot = localSlotRefFromName(expression.text);
                const std::string base = stripSourceWidthSuffix(expression.text);
                spellings[base.substr(0, base.find('.'))].insert(expression.text);
                if (slot && !slot->aggregateField && expression.text.find('v') == std::string::npos && extent->first < 0 && base.find('.') == std::string::npos)
                    words.emplace(expression.text, *extent);
            }
        for (const SourceExpr &child : expression.args)
            scan(child);
    };
    const std::function<void(const SourceStmt &)> scanStatement = [&](const SourceStmt &statement) {
        scan(statement.expr);
        scan(statement.value);
        for (const SourceStmt &nested : statement.body)
            scanStatement(nested);
    };
    for (const BasicBlock &block : blocks) {
        for (const SourceStmt &statement : block.body)
            scanStatement(statement);
        for (const SourceStmt &statement : block.exit)
            scanStatement(statement);
    }
    std::map<std::string, size_t> candidates;
    std::vector<std::string> names;
    for (const std::pair<const std::string, std::pair<int32_t, int32_t>> &word : words) {
        const std::string base = stripSourceWidthSuffix(word.first);
        if (!excluded.count(base) && spellings[base].size() == 1) {
            candidates.emplace(word.first, names.size());
            names.push_back(word.first);
        }
    }
    if (candidates.empty())
        return;

    std::vector<std::vector<Access>> accesses(blocks.size());
    for (size_t index = 0; index < blocks.size(); ++index)
        walkBlock(blocks[index], [&](SourceExpr &identifier, bool store) {
            const std::map<std::string, size_t>::const_iterator candidate = candidates.find(identifier.text);
            if (candidate != candidates.end())
                accesses[index].push_back(Access{.object = candidate->second, .keys = {candidate->second}, .store = store, .name = &identifier});
        });
    std::vector<size_t> keyObject(names.size());
    for (size_t object = 0; object < names.size(); ++object)
        keyObject[object] = object;
    if (renameWebs(graph, accesses, accessWebs(graph, accesses, keyObject, names.size()), names))
        graph.refold();
}

namespace {

enum class RecordUse : uint8_t { Load, Store, Merge, Native, Escape };

std::optional<std::pair<SourceExpr *, uint32_t>> addressedWord(SourceExpr &expression) {
    if (expression.kind != SourceExprKind::Unary || expression.text != "*" || expression.args.size() != 1)
        return std::nullopt;
    SourceExpr &address = expression.args[0];
    const bool offset = address.kind == SourceExprKind::Binary && address.text == "+" && address.args.size() == 2;
    SourceExpr &base = offset ? address.args[0] : address;
    if (base.kind != SourceExprKind::Unary || base.text != "&" || base.args.size() != 1 || base.args[0].kind != SourceExprKind::Identifier)
        return std::nullopt;
    const std::optional<uint32_t> displacement = offset ? sourceDataConstantValue(address.args[1]) : std::optional<uint32_t>(0);
    if (!displacement)
        return std::nullopt;
    return std::pair{&base.args[0], *displacement};
}

std::optional<std::pair<SourceExpr *, uint32_t>> addressedNarrow(SourceExpr &expression) {
    const uint32_t width = expression.kind != SourceExprKind::Call || expression.args.size() != 1 ? 0U :
        expression.text == "vm_load8" || expression.text == "vm_mem8"                             ? 1U :
        expression.text == "vm_load16" || expression.text == "vm_mem16"                           ? 2U :
                                                                                                    0U;
    if (!width)
        return std::nullopt;
    SourceExpr word{.kind = SourceExprKind::Unary, .text = "*", .args = {expression.args[0]}};
    const std::optional<std::pair<SourceExpr *, uint32_t>> reached = addressedWord(word);
    if (!reached || reached->second % 4U + width > 4U)
        return std::nullopt;
    SourceExpr &address = expression.args[0];
    SourceExpr &base = address.kind == SourceExprKind::Binary ? address.args[0] : address;
    return std::pair{&base.args[0], reached->second - reached->second % 4U};
}

template <typename Visit> void walkRecordExpression(SourceExpr &expression, const Visit &visit) {
    if (expression.kind == SourceExprKind::Binary && expression.text == "=" && expression.args.size() == 2) {
        walkRecordExpression(expression.args[1], visit);
        SourceExpr &target = expression.args[0];
        if (target.kind == SourceExprKind::Identifier)
            visit(target, 0U, RecordUse::Store, nullptr, NativeAccess::ReadWrite, ~0U);
        else if (const std::optional<std::pair<SourceExpr *, uint32_t>> word = addressedWord(target))
            visit(*word->first, word->second, RecordUse::Store, nullptr, NativeAccess::ReadWrite, ~0U);
        else if (const std::optional<std::pair<SourceExpr *, uint32_t>> narrow = addressedNarrow(target))
            visit(*narrow->first, narrow->second, RecordUse::Merge, nullptr, NativeAccess::ReadWrite, ~0U);
        else
            walkRecordExpression(target, visit);
        return;
    }
    if (const std::optional<std::pair<SourceExpr *, uint32_t>> narrow = addressedNarrow(expression)) {
        visit(*narrow->first, narrow->second, RecordUse::Load, nullptr, NativeAccess::ReadWrite, ~0U);
        return;
    }
    if (const std::optional<std::pair<SourceExpr *, uint32_t>> word = addressedWord(expression)) {
        visit(*word->first, word->second, RecordUse::Load, nullptr, NativeAccess::ReadWrite, ~0U);
        return;
    }
    if (expression.kind == SourceExprKind::Identifier) {
        visit(expression, 0U, RecordUse::Load, nullptr, NativeAccess::ReadWrite, ~0U);
        return;
    }
    if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1 &&
        expression.args[0].kind == SourceExprKind::Identifier) {
        visit(expression.args[0], 0U, RecordUse::Escape, nullptr, NativeAccess::ReadWrite, ~0U);
        return;
    }
    const abi::CommandSpec *command =
        expression.kind == SourceExprKind::Call && !expression.text.starts_with("vm_") ? abiCommandForCallName(expression.text) : nullptr;
    if (command && command->handlerName) {
        std::vector<std::optional<uint32_t>> observed;
        for (const SourceExpr &argument : expression.args)
            observed.push_back(sourceDataConstantValue(argument));
        const NativeArgumentLayout *layout = nativeArgumentLayout(command->handlerName, observed);
        for (size_t index = 0; index < expression.args.size(); ++index) {
            SourceExpr &argument = expression.args[index];
            const NativeArgumentType *parameter = nativeArgumentForHandler(command->handlerName, index, observed);
            if (!parameter || !parameter->pointer || argument.kind != SourceExprKind::Unary || argument.text != "&" || argument.args.size() != 1 ||
                argument.args[0].kind != SourceExprKind::Identifier) {
                walkRecordExpression(argument, visit);
                continue;
            }
            bool readOnly = false;
            if (layout)
                for (const NativePointerParameter &pointer : layout->pointers)
                    readOnly |= pointer.byteOffset == index * 4U && pointer.readOnly;
            visit(argument.args[0], 0U, RecordUse::Native, findScenarioDataSchema(parameter->type),
                readOnly ? NativeAccess::Read : nativePointerAccess(command->handlerName, index, observed),
                readOnly ? nativePointerReads(command->handlerName, index, observed) : ~0U);
        }
        return;
    }
    for (SourceExpr &child : expression.args)
        walkRecordExpression(child, visit);
}

template <typename Visit> void walkRecordBlock(BasicBlock &block, const Visit &visit) {
    const std::function<void(SourceStmt &)> statement = [&](SourceStmt &item) {
        walkRecordExpression(item.value, visit);
        if (item.kind == SourceStmtKind::Assign && item.expr.kind == SourceExprKind::Identifier && item.expr.text != "eax")
            visit(item.expr, 0U, RecordUse::Store, nullptr, NativeAccess::ReadWrite, ~0U);
        else
            walkRecordExpression(item.expr, visit);
        for (SourceStmt &nested : item.body)
            statement(nested);
    };
    for (SourceStmt &item : block.body)
        statement(item);
    for (SourceStmt &item : block.exit)
        statement(item);
}

} // namespace

std::map<std::string, std::string> splitFrameRecords(ControlFlowGraph &graph) {
    std::vector<BasicBlock> &blocks = graph.blocks;

    struct Slot {
        bool typed = false, excluded = false;
        std::set<uint32_t> offsets;
    };
    std::map<std::string, Slot> slots;
    std::set<std::string> viewed;
    for (BasicBlock &block : blocks)
        walkRecordBlock(block, [&](SourceExpr &name, uint32_t offset, RecordUse use, const ScenarioDataSchema *schema, NativeAccess, uint32_t) {
            const std::optional<SourceSlotRef> slot = localSlotRefFromName(name.text);
            if (!slot)
                return;
            if (slot->aggregateField || name.text.find_first_of(".v") != std::string::npos) {
                viewed.insert(stripSourceWidthSuffix(name.text.substr(0, name.text.find('.'))).substr(0, name.text.find('v')));
                return;
            }
            Slot &entry = slots[name.text];
            entry.excluded |= use == RecordUse::Escape || offset % 4U;
            entry.typed |= use == RecordUse::Native && schema;
            if (use == RecordUse::Native && schema)
                for (const ScenarioDataField &field : schema->fields)
                    if (scenarioDataTypeSize(field.type) == 4 && field.elementCount == 1)
                        entry.offsets.insert(field.offset);
            if (use != RecordUse::Native)
                entry.offsets.insert(offset);
        });
    std::map<std::string, size_t> candidates;
    std::vector<std::string> names;
    std::vector<size_t> keyObject;
    std::vector<std::map<uint32_t, size_t>> keys;
    for (std::pair<const std::string, Slot> &slot : slots) {
        if (!slot.second.typed || slot.second.excluded || viewed.count(slot.first))
            continue;
        candidates.emplace(slot.first, names.size());
        names.push_back(slot.first);
        slot.second.offsets.insert(0);
        keys.emplace_back();
        for (uint32_t offset : slot.second.offsets) {
            keys.back().emplace(offset, keyObject.size());
            keyObject.push_back(names.size() - 1);
        }
    }
    if (candidates.empty())
        return {};

    std::vector<int32_t> starts;
    for (const std::string &name : names)
        starts.push_back(frameExtent(name)->first);

    std::vector<std::vector<Access>> accesses(blocks.size());
    for (size_t index = 0; index < blocks.size(); ++index)
        walkRecordBlock(
            blocks[index], [&](SourceExpr &name, uint32_t offset, RecordUse use, const ScenarioDataSchema *schema, NativeAccess access, uint32_t words) {
                const std::map<std::string, size_t>::const_iterator candidate = candidates.find(name.text);
                const bool native = use == RecordUse::Native;
                const std::optional<std::pair<int32_t, int32_t>> named = native && access == NativeAccess::Read ? std::nullopt : frameExtent(name.text);
                if (named) {
                    const int32_t first = named->first + static_cast<int32_t>(offset);
                    const int32_t last = native && schema ? named->first + static_cast<int32_t>(schema->size) : first + named->second - named->first;
                    for (size_t object = 0; object < names.size(); ++object) {
                        if (candidate != candidates.end() && candidate->second == object)
                            continue;
                        std::vector<size_t> reached;
                        for (const std::pair<const uint32_t, size_t> &key : keys[object])
                            if (starts[object] + static_cast<int32_t>(key.first) < last && first < starts[object] + static_cast<int32_t>(key.first) + 4)
                                reached.push_back(key.second);
                        if (!reached.empty())
                            accesses[index].push_back(Access{.object = object,
                                .keys = reached,
                                .store = use != RecordUse::Load,
                                .name = nullptr,
                                .reads = (native && access == NativeAccess::ReadWrite) || use == RecordUse::Merge,
                                .through = &name});
                    }
                }
                if (candidate == candidates.end())
                    return;
                const size_t object = candidate->second;
                if (!native) {
                    accesses[index].push_back(Access{
                        .object = object, .keys = {keys[object].at(offset)}, .store = use != RecordUse::Load, .name = &name, .reads = use == RecordUse::Merge});
                    return;
                }
                std::vector<size_t> all;
                for (const std::pair<const uint32_t, size_t> &key : keys[object])
                    if (access != NativeAccess::Read || key.first / 4U >= 32U || ((words >> (key.first / 4U)) & 1U))
                        all.push_back(key.second);
                accesses[index].push_back(Access{.object = object,
                    .keys = all,
                    .store = access != NativeAccess::Read,
                    .name = &name,
                    .schema = schema,
                    .reads = access != NativeAccess::Write});
            });
    const Webs webs = accessWebs(graph, accesses, keyObject, names.size());
    if (renameWebs(graph, accesses, webs, names))
        graph.refold();

    std::map<size_t, std::string> variables;
    size_t position = 0;
    for (const std::vector<Access> &block : accesses)
        for (const Access &access : block)
            if (const size_t web = webs.ofAccess[position++]; access.name)
                variables[web] = access.name->text;
    std::map<std::string, std::string> members;
    std::set<std::string> ambiguous;
    position = 0;
    for (const std::vector<Access> &block : accesses)
        for (const Access &access : block) {
            const std::map<size_t, std::string>::const_iterator variable = variables.find(webs.ofAccess[position++]);
            if (!access.through || variable == variables.end() || access.through->text.find('.') != std::string::npos)
                continue;
            const std::map<std::string, std::string>::const_iterator known = members.find(access.through->text);
            if (known != members.end() && known->second != variable->second)
                ambiguous.insert(access.through->text);
            members.emplace(access.through->text, variable->second);
        }
    for (const std::string &word : ambiguous)
        members.erase(word);

    return members;
}

namespace {

using Path = std::vector<std::pair<size_t, size_t>>;

std::map<std::string, std::vector<Path>> frameOccurrences(std::vector<SourceStmt> &body) {
    std::map<std::string, std::vector<Path>> occurrences;
    Path path;
    const std::function<void(SourceExpr &)> expression = [&](SourceExpr &node) {
        if (node.kind == SourceExprKind::Identifier && node.text.starts_with("local_"))
            occurrences[node.text].push_back(path);
        for (SourceExpr &child : node.args)
            expression(child);
    };
    const std::function<void(std::vector<SourceStmt> &, size_t)> statements = [&](std::vector<SourceStmt> &list, size_t which) {
        for (size_t index = 0; index < list.size(); ++index) {
            SourceStmt &statement = list[index];
            path.emplace_back(index, which);
            expression(statement.expr);
            expression(statement.value);
            statements(statement.body, 0);
            statements(statement.elseBody, 1);
            for (size_t clause = 0; clause < statement.cases.size(); ++clause)
                statements(statement.cases[clause].body, 2 + clause);
            path.pop_back();
        }
    };
    statements(body, 0);
    return occurrences;
}

struct Scope {
    Path prefix;
    size_t list, first, last;
};

Scope scopeOf(const std::vector<Path> &paths) {
    Path prefix = paths[0];
    for (const Path &other : paths) {
        size_t common = 0;
        while (common < prefix.size() && common < other.size() && prefix[common] == other[common])
            ++common;
        prefix.resize(common);
    }
    size_t depth = prefix.size();
    while (depth) {
        bool listed = true;
        for (const Path &other : paths)
            listed &= other.size() > depth && other[depth].second == paths[0][depth].second;
        if (listed)
            break;
        prefix.pop_back();
        --depth;
    }
    Scope scope{.prefix = prefix, .list = paths[0][depth].second, .first = SIZE_MAX, .last = 0};
    for (const Path &other : paths) {
        scope.first = other[depth].first < scope.first ? other[depth].first : scope.first;
        scope.last = other[depth].first > scope.last ? other[depth].first : scope.last;
    }
    return scope;
}

bool overlap(const Scope &left, const Scope &right) {
    const size_t depth = left.prefix.size() < right.prefix.size() ? left.prefix.size() : right.prefix.size();
    for (size_t level = 0; level < depth; ++level)
        if (left.prefix[level] != right.prefix[level])
            return false;
    if (left.prefix.size() == right.prefix.size())
        return left.list == right.list && left.first <= right.last && right.first <= left.last;
    const Scope &outer = left.prefix.size() < right.prefix.size() ? left : right, &inner = &outer == &left ? right : left;
    const std::pair<size_t, size_t> &step = inner.prefix[outer.prefix.size()];
    return step.second == outer.list && step.first >= outer.first && step.first <= outer.last;
}

bool encloses(const Scope &declared, const Scope &inner) {
    if (inner.prefix.size() < declared.prefix.size())
        return false;
    for (size_t level = 0; level < declared.prefix.size(); ++level)
        if (declared.prefix[level] != inner.prefix[level])
            return false;
    const size_t list = inner.prefix.size() == declared.prefix.size() ? inner.list : inner.prefix[declared.prefix.size()].second;
    return list == declared.list || (list >= 2 && declared.list >= 2);
}

bool contains(const Scope &outer, const Scope &inner) {
    if (inner.prefix.size() < outer.prefix.size())
        return false;
    for (size_t level = 0; level < outer.prefix.size(); ++level)
        if (outer.prefix[level] != inner.prefix[level])
            return false;
    if (inner.prefix.size() == outer.prefix.size())
        return inner.list == outer.list && inner.first >= outer.first && inner.last <= outer.last;
    const std::pair<size_t, size_t> &step = inner.prefix[outer.prefix.size()];
    return step.second == outer.list && step.first >= outer.first && step.first <= outer.last;
}

} // namespace

void placeFrameVariables(SourceFunctionAst &function) {
    std::vector<SourceStmt> &body = function.body.body;
    const std::map<std::string, std::vector<Path>> occurrences = frameOccurrences(body);

    std::map<std::string, const ScenarioDataSchema *> records;
    const std::function<void(std::vector<SourceStmt> &)> evidence = [&](std::vector<SourceStmt> &list) {
        for (SourceStmt &statement : list) {
            for (SourceExpr *expression : {&statement.expr, &statement.value})
                walkRecordExpression(*expression, [&](SourceExpr &name, uint32_t, RecordUse use, const ScenarioDataSchema *schema, NativeAccess, uint32_t) {
                    if (use == RecordUse::Native && schema)
                        records.emplace(name.text, schema);
                });
            evidence(statement.body);
            evidence(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                evidence(clause.body);
        }
    };
    evidence(body);

    std::map<std::string, std::vector<Path>> objects;
    std::map<std::string, std::pair<int32_t, int32_t>> bytes;
    std::vector<std::pair<int32_t, int32_t>> extents;
    for (const std::pair<const std::string, std::vector<Path>> &name : occurrences) {
        const std::string object = name.first.substr(0, name.first.find('.'));
        std::vector<Path> &paths = objects[object];
        paths.insert(paths.end(), name.second.begin(), name.second.end());
        if (std::optional<std::pair<int32_t, int32_t>> extent = frameExtent(name.first)) {
            const std::map<std::string, const ScenarioDataSchema *>::const_iterator record = records.find(object);
            if (record != records.end())
                extent = std::pair{frameExtent(object)->first, frameExtent(object)->first + static_cast<int32_t>(record->second->size)};
            extents.push_back(*extent);
            const std::map<std::string, std::pair<int32_t, int32_t>>::iterator known = bytes.find(object);
            if (known == bytes.end())
                bytes.emplace(object, *extent);
            else
                known->second = {extent->first < known->second.first ? extent->first : known->second.first,
                    extent->second > known->second.second ? extent->second : known->second.second};
        }
    }
    const uint32_t encoded = function.explicitFrameAdjust.value_or(0);
    const int32_t reserved = -(encoded <= 0xFFU ? static_cast<int32_t>(static_cast<int8_t>(encoded)) : static_cast<int32_t>(static_cast<int16_t>(encoded)));

    int32_t deepest = 0;
    for (const std::pair<int32_t, int32_t> &extent : extents)
        deepest = extent.first < deepest ? extent.first : deepest;
    const int32_t floor = (-deepest + 3) / 4 * 4 == reserved ? deepest : -reserved;
    const auto family = [](const std::string &object) { return object.substr(0, object.find('v')); };

    std::map<std::string, std::vector<std::vector<std::string>>> families;
    for (const std::pair<const std::string, std::vector<Path>> &object : objects)
        if (bytes.count(object.first))
            families[family(object.first)].push_back({object.first});
    const auto scopeOfVariable = [&](const std::vector<std::string> &variable) {
        std::vector<Path> paths;
        for (const std::string &object : variable)
            paths.insert(paths.end(), objects.at(object).begin(), objects.at(object).end());
        return scopeOf(paths);
    };
    const auto bytesOfVariable = [&](const std::vector<std::string> &variable) {
        std::pair<int32_t, int32_t> range = bytes.at(variable[0]);
        for (const std::string &object : variable) {
            range.first = bytes.at(object).first < range.first ? bytes.at(object).first : range.first;
            range.second = bytes.at(object).second > range.second ? bytes.at(object).second : range.second;
        }
        return range;
    };
    const auto merge = [](std::vector<std::vector<std::string>> &variables) {
        for (size_t variable = 1; variable < variables.size(); ++variable)
            variables[0].insert(variables[0].end(), variables[variable].begin(), variables[variable].end());
        variables.resize(1);
    };

    for (std::pair<const std::string, std::vector<std::vector<std::string>>> &slot : families)
        for (bool merged = true; merged;) {
            merged = false;
            std::vector<std::vector<std::string>> &variables = slot.second;
            for (size_t left = 0; left < variables.size() && !merged; ++left)
                for (size_t right = left + 1; right < variables.size() && !merged; ++right)
                    if (overlap(scopeOfVariable(variables[left]), scopeOfVariable(variables[right]))) {
                        variables[left].insert(variables[left].end(), variables[right].begin(), variables[right].end());
                        variables.erase(variables.begin() + static_cast<std::ptrdiff_t>(right));
                        merged = true;
                    }
        }

    for (bool changed = true; changed;) {
        changed = false;
        std::set<const std::vector<std::string> *> scoped;
        for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &first : families)
            for (const std::vector<std::string> &variable : first.second) {
                bool own = first.second.size() > 1;
                const std::pair<int32_t, int32_t> range = bytesOfVariable(variable);
                for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &second : families)
                    for (const std::vector<std::string> &other : second.second)
                        own |= first.first != second.first && bytesOfVariable(other).first < range.second && range.first < bytesOfVariable(other).second;
                if (own)
                    scoped.insert(&variable);
            }
        for (bool nested = true; nested;) {
            nested = false;
            for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &first : families)
                for (const std::vector<std::string> &variable : first.second)
                    for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &second : families)
                        for (const std::vector<std::string> &block : second.second)
                            if (!scoped.count(&variable) && scoped.count(&block) && first.first != second.first &&
                                bytesOfVariable(variable).second <= bytesOfVariable(block).first &&
                                contains(scopeOfVariable(block), scopeOfVariable(variable))) {
                                scoped.insert(&variable);
                                nested = true;
                            }
        }
        for (std::pair<const std::string, std::vector<std::vector<std::string>>> &slot : families) {
            std::vector<std::vector<std::string>> &variables = slot.second;
            if (variables.size() < 2)
                continue;
            int32_t lowest = 0;
            for (const std::vector<std::string> &variable : variables)
                lowest = bytesOfVariable(variable).first < lowest ? bytesOfVariable(variable).first : lowest;
            bool placed = true;
            for (int32_t byte = floor; byte < lowest && placed; ++byte) {
                bool covered = false;
                for (const std::pair<int32_t, int32_t> &extent : extents)
                    covered |= byte >= extent.first && byte < extent.second;
                placed = covered;
            }

            std::vector<Scope> blocks;
            std::vector<std::pair<int32_t, int32_t>> spans;
            for (const std::vector<std::string> &variable : variables) {
                const std::pair<int32_t, int32_t> own = bytesOfVariable(variable);
                std::vector<Path> paths;
                for (const std::string &object : variable)
                    paths.insert(paths.end(), objects.at(object).begin(), objects.at(object).end());
                for (bool grown = true; grown;) {
                    grown = false;
                    const Scope block = scopeOf(paths);
                    for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &other : families)
                        for (const std::vector<std::string> &deeper : other.second) {
                            const std::pair<int32_t, int32_t> range = bytesOfVariable(deeper);
                            if (other.first == slot.first || range.first >= own.first || (range.first < own.second && own.first < range.second))
                                continue;
                            const Scope scope = scopeOfVariable(deeper);
                            if (!overlap(block, scope) || contains(block, scope))
                                continue;
                            for (const std::string &object : deeper)
                                paths.insert(paths.end(), objects.at(object).begin(), objects.at(object).end());
                            grown = true;
                        }
                }
                blocks.push_back(scopeOf(paths));
                spans.push_back(own);
                placed &= !(blocks.back().prefix.empty() && blocks.back().last + 1 == body.size() && body.back().kind == SourceStmtKind::Return);
            }
            for (size_t left = 0; left < blocks.size(); ++left)
                for (size_t right = left + 1; right < blocks.size(); ++right)
                    placed &= !overlap(blocks[left], blocks[right]);

            for (const std::pair<const std::string, std::vector<std::vector<std::string>>> &other : families) {
                if (other.first == slot.first)
                    continue;
                for (const std::vector<std::string> &variable : other.second) {
                    const std::pair<int32_t, int32_t> range = bytesOfVariable(variable);
                    const Scope scope = scopeOfVariable(variable);
                    bool inside = false;
                    for (const Scope &block : blocks)
                        inside |= contains(block, scope);
                    for (size_t index = 0; index < blocks.size(); ++index) {
                        const std::pair<int32_t, int32_t> &own = spans[index];
                        if (range.first < own.second && own.first < range.second)
                            placed &= !overlap(blocks[index], scope);
                        else if (range.first < own.first)
                            placed &= contains(blocks[index], scope) ||
                                (!overlap(blocks[index], scope) && (inside || scoped.count(&variable) || !encloses(scope, blocks[index])));
                    }
                }
            }
            if (!placed) {
                merge(variables);
                changed = true;
            }
        }
    }

    std::map<std::string, std::string> renamed;
    for (std::pair<const std::string, std::vector<std::vector<std::string>>> &slot : families) {
        std::vector<std::vector<std::string>> &variables = slot.second;
        if (variables.size() == 1 && variables[0].size() == 1 && variables[0][0] == slot.first)
            continue;
        std::vector<std::pair<Path, size_t>> order;
        for (size_t variable = 0; variable < variables.size(); ++variable) {
            Path first = objects.at(variables[variable][0])[0];
            for (const std::string &object : variables[variable])
                for (const Path &path : objects.at(object))
                    first = path < first ? path : first;
            order.emplace_back(first, variable);
        }
        for (size_t index = 1; index < order.size(); ++index)
            for (size_t position = index; position && order[position] < order[position - 1]; --position)
                std::swap(order[position], order[position - 1]);
        for (size_t position = 0; position < order.size(); ++position)
            for (const std::string &object : variables[order[position].second])
                renamed[object] = position ? slot.first + "v" + std::to_string(position + 1) : slot.first;
    }
    const std::function<void(SourceExpr &)> rename = [&](SourceExpr &node) {
        if (node.kind == SourceExprKind::Identifier) {
            const size_t member = node.text.find('.');
            const std::map<std::string, std::string>::const_iterator name = renamed.find(node.text.substr(0, member));
            if (name != renamed.end())
                node.text = name->second + (member == std::string::npos ? std::string() : node.text.substr(member));
        }
        for (SourceExpr &child : node.args)
            rename(child);
    };
    const std::function<void(std::vector<SourceStmt> &)> apply = [&](std::vector<SourceStmt> &list) {
        for (SourceStmt &statement : list) {
            rename(statement.expr);
            rename(statement.value);
            apply(statement.body);
            apply(statement.elseBody);
            for (SourceSwitchCase &clause : statement.cases)
                apply(clause.body);
        }
    };
    apply(body);
    const auto current = [&](const std::string &name) {
        const std::map<std::string, std::string>::const_iterator to = renamed.find(name);
        return to == renamed.end() ? name : to->second;
    };
    std::map<std::string, std::string> members;
    for (const std::pair<const std::string, std::string> &member : function.frameMembers)
        members.emplace(current(member.first), current(member.second));
    function.frameMembers = members;
}

} // namespace shenmue::scenario::decompile
