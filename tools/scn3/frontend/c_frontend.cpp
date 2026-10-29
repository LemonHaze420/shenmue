#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/c_frontend.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario::c_frontend {

class CFrontend {
  public:
    explicit CFrontend(const CNode &tree) : tree_(tree) {
        unit_.ordinaryC = true;
        unit_.abiProfile = "s1_pc";
        unit_.header.token = SHEN_NAME(SCN3);
        unit_.header.flags = SCF_Unknown0100;
        unit_.header.entrypoint = sizeof(ScenarioHeader);
    }

    SourceUnit parse() {
        for (const CNode &node : tree_.children) {
            if (node.kind == CKind::Function)
                for (const CNode &attribute : node.attributes)
                    if (attribute.kind == CKind::Builtin)
                        if (node.name.starts_with("__builtin_va_"))
                            variadic_.builtins.insert(node.id);
            if (node.kind == CKind::Typedef && node.name == "__builtin_va_list" && node.implicit)
                vaTypes_.insert(node.id);
        }
        for (const CNode &node : tree_.children)
            if (node.kind == CKind::Typedef && vaTypes_.count(node.type.alias))
                vaTypes_.insert(node.id);
        for (const CNode &node : tree_.children) {
            if (node.source && !node.name.empty())
                globalNames_.insert(node.name);
        }
        for (const CNode &node : tree_.children) {
            if (!node.source)
                continue;
            if (node.kind == CKind::Function || node.kind == CKind::Variable)
                declare(node, 0);
        }
        for (const CNode &node : tree_.children) {
            if (!node.source)
                continue;
            if (node.kind == CKind::Function)
                function(node);
            else if (node.kind == CKind::Variable)
                global(node);
            else if (node.kind != CKind::Empty && node.kind != CKind::Typedef && node.kind != CKind::Record && node.kind != CKind::Enum &&
                node.kind != CKind::StaticAssert)
                throw std::runtime_error("Unsupported C declaration: " + node.className);
        }
        if (!unit_.startupFunctionSymbol)
            throw std::runtime_error("A scenario requires one externally visible entry function definition");
        for (const std::string &text : stringOrder_) {
            const std::string &name = stringObjects_.at(text);
            std::vector<uint8_t> &bytes = unit_.dataSegment;
            const uint32_t begin = static_cast<uint32_t>(bytes.size());
            bytes.insert(bytes.end(), text.begin(), text.end());
            bytes.push_back(0);
            unit_.dataSymbols[name] = sizeof(ScenarioHeader) + begin;
            unit_.dataBindings[name] = SourceDataBinding{.type = "char", .elementCount = static_cast<uint32_t>(text.size() + 1), .isArray = true};
            unit_.cstrOffsetsByText[text].push_back(sizeof(ScenarioHeader) + begin);
        }
        unit_.dataSegmentDeclaredStart = sizeof(ScenarioHeader);
        unit_.header.dataSegmentOffset = sizeof(ScenarioHeader);
        unit_.header.dataEndOffset = static_cast<int32_t>(sizeof(ScenarioHeader) + unit_.dataSegment.size());
        unit_.header.relocationTableOffset = unit_.header.dataEndOffset;
        unit_.header.workspaceSize = workspaceSize_;
        unit_.trailingDataDeclaredStart = unit_.header.dataEndOffset;
        unit_.trailingData.assign(8, 0);
        unit_.header.wholeSize = unit_.header.dataEndOffset + 8;
        unit_.parsedHeader = parseHeaderSemantics(unit_.header);
        applySourceDataRelocations(unit_);
        return unit_;
    }

  private:
    static std::optional<int32_t> layoutAttribute(const CNode &node, uint32_t tag) {
        std::optional<int32_t> result;
        for (const CNode &attribute : node.attributes) {
            if (attribute.kind != CKind::Annotate)
                continue;
            const std::vector<CNode> &arguments = attribute.children;
            if (arguments.size() != 2 || !arguments[0].value || !arguments[1].value)
                continue;
            if (static_cast<uint64_t>(*arguments[0].value) != tag)
                continue;
            const int64_t value = *arguments[1].value;
            if (result || value < INT32_MIN || value > INT32_MAX)
                throw std::runtime_error("Invalid or duplicate C layout attribute");
            result = static_cast<int32_t>(value);
        }
        return result;
    }

    static std::map<uint32_t, std::string> stringBindings(const CNode &node) {
        std::map<uint32_t, std::string> result;
        bool found = false;
        for (const CNode &attribute : node.attributes) {
            if (attribute.kind != CKind::Annotate)
                continue;
            const std::vector<CNode> &arguments = attribute.children;
            if (arguments.empty() || !arguments[0].value || static_cast<uint64_t>(*arguments[0].value) != 0x53434e05U)
                continue;
            if (found || arguments.size() < 3 || arguments.size() % 2 != 1 || node.kind != CKind::Function)
                throw std::runtime_error("scn_string_bindings requires literal-index and storage pairs on a function");
            found = true;
            for (size_t index = 1; index < arguments.size(); index += 2) {
                if (!arguments[index].value)
                    throw std::runtime_error("String binding index must be a nonnegative integer");
                const int64_t ordinal = *arguments[index].value;
                const CNode *object = &arguments[index + 1];
                while ((object->kind == CKind::Constant || object->kind == CKind::ImplicitCast || object->kind == CKind::Paren) && object->children.size() == 1)
                    object = &object->children.at(0);
                if (ordinal < 0 || ordinal > UINT32_MAX || object->kind != CKind::Reference || object->targetKind != CKind::Variable)
                    throw std::runtime_error("String binding requires a nonnegative literal index and a named backing array");
                if (!result.emplace(static_cast<uint32_t>(ordinal), object->name).second)
                    throw std::runtime_error("Duplicate string binding index");
            }
        }
        return result;
    }

    static void collectStringLiterals(const CNode &node, std::map<uint32_t, std::set<uint32_t>> &positions) {
        if (node.kind == CKind::String)
            positions[node.offset].insert(node.id);
        for (const CNode &child : node.children)
            collectStringLiterals(child, positions);
        for (const CNode &attribute : node.attributes)
            collectStringLiterals(attribute, positions);
    }

    static const CNode &initializer(const CNode &node) {
        if (node.children.empty())
            throw std::runtime_error("C declaration has no initializer expression");
        return node.children.front();
    }

    uint32_t declare(const CNode &node, uint32_t scope) {
        (void)stringBindings(node);
        if (layoutAttribute(node, 0x53434e01U) && node.kind != CKind::Function)
            throw std::runtime_error("scn_stack_size belongs to a C function");
        if (layoutAttribute(node, 0x53434e02U) && (node.kind != CKind::Variable || scope == 0))
            throw std::runtime_error("scn_stack_offset belongs to an automatic C object");
        if (layoutAttribute(node, 0x53434e03U) && (node.kind != CKind::Variable || scope != 0 || node.initialized))
            throw std::runtime_error("scn_task_offset belongs to an uninitialized module C object");
        if (layoutAttribute(node, 0x53434e04U)) {
            const ObjectType type = objectType(node.type);
            if (node.kind != CKind::Variable || scope != 0 || !node.initialized || !type.array || type.element != "int8_t" || !node.type.constant ||
                initializer(node).kind != CKind::String)
                throw std::runtime_error("scn_string_literal requires an initialized constant character array at module scope");
            const SourceExpr literal = expression(initializer(node));
            if (literal.text.size() >= type.count || literal.text.find('\0') != std::string::npos)
                throw std::runtime_error("scn_string_literal requires one terminated string");
            literalTextByObject_[node.name] = literal.text;
            const std::pair<std::map<std::string, std::string>::iterator, bool> inserted = literalObjects_.emplace(literal.text, node.name);
            if (!inserted.second && inserted.first->second != node.name)
                inserted.first->second.clear();
        }
        if (identities_.count(node.id))
            return identities_.at(node.id);
        if (node.previous && identities_.count(node.previous))
            return identities_[node.id] = identities_.at(node.previous);
        std::string name = node.name;
        if (name.empty())
            throw std::runtime_error("Unnamed C declaration cannot be allocated");
        if (scope) {
            name = "__c_decl" + std::to_string(unit_.cDeclarations.size());
            while (globalNames_.count(name) || localNames_.count(name))
                name += "_";
            localNames_.insert(name);
        }
        unit_.cDeclarations.push_back({name, node.type.name, scope});
        const uint32_t identity = static_cast<uint32_t>(unit_.cDeclarations.size());
        identities_[node.id] = identity;
        return identity;
    }

    SourceExpr reference(const CNode &node, uint32_t declaration) const {
        SourceExpr result{.kind = SourceExprKind::Identifier, .text = node.name};
        const std::map<uint32_t, uint32_t>::const_iterator identity = identities_.find(declaration);
        if (identity != identities_.end()) {
            result.declarationId = identity->second;
            result.text = unit_.cDeclarations[identity->second - 1].name;
        }
        result.cType = node.type.name;
        result.volatileAccess = node.type.isVolatile;
        return result;
    }

    SourceExpr expression(const CNode &node) {
        const std::vector<CNode> &inner = node.children;
        SourceExpr result;
        result.cType = node.type.name;
        result.volatileAccess = node.type.isVolatile;
        if (node.kind == CKind::TypeTrait) {
            const std::string &operation = node.name;
            if (operation != "sizeof" && operation != "alignof")
                throw std::runtime_error("Unsupported C type trait: " + operation);
            const ObjectType object = objectType(inner.empty() ? node.operand : inner.at(0).type);
            result.kind = SourceExprKind::IntLiteral;
            result.text = std::to_string(operation == "sizeof" ? object.size * object.count : object.alignment) + "U";
            return result;
        }
        if (node.kind == CKind::Reference) {
            if (variadic_.cursors.count(node.target))
                throw std::runtime_error("C va_list is used outside va_start, va_arg or va_end");
            return reference(node, node.target);
        }
        if (node.kind == CKind::VariadicArgument) {
            const std::map<uint32_t, uint32_t>::const_iterator read = variadic_.reads.find(node.id);
            if (read == variadic_.reads.end())
                throw std::runtime_error("C va_arg is unreachable or its parameter position is unknown");
            return SourceExpr{.kind = SourceExprKind::VariadicArgument, .text = std::to_string(read->second), .cType = node.type.name};
        }
        if (result.cType == "long long" || result.cType == "unsigned long long")
            throw std::runtime_error("The VM has no 64-bit integer expression lowering");

        if (node.kind == CKind::Binary || node.kind == CKind::Unary || node.kind == CKind::ExplicitCast || node.kind == CKind::Paren) {
            bool character = false;
            const std::optional<uint32_t> folded = characterConstant(node, character);
            if (folded && character && (result.cType == "int32_t" || result.cType == "uint32_t")) {
                result.kind = SourceExprKind::IntLiteral;
                result.text = hexUnsigned(*folded) + (result.cType == "uint32_t" ? "U" : "");
                return result;
            }
        }
        if (node.kind == CKind::Paren || node.kind == CKind::Constant)
            return expression(inner.at(0));
        if (node.kind == CKind::Generic) {
            if (node.selected < inner.size())
                return expression(inner[node.selected]);
            throw std::runtime_error("C generic selection has no selected association");
        }
        if (node.kind == CKind::ImplicitCast || node.kind == CKind::ExplicitCast) {
            result = expression(inner.at(0));
            const std::string &cast = node.text;
            const std::string &type = node.type.name;
            if (cast == "LValueToRValue" || cast == "ArrayToPointerDecay" || cast == "FunctionToPointerDecay" || cast == "NoOp" || cast == "NullToPointer") {

                const bool reinterpretation = result.kind == SourceExprKind::Cast && result.cConversion == "BitCast";
                if (!reinterpretation && (cast != "NoOp" || (result.cConversion != "ArrayToPointerDecay" && result.cConversion != "FunctionToPointerDecay")))
                    result.cConversion = cast;
                result.cType = type;
                return result;
            }
            if (cast == "PointerToIntegral" || cast == "BitCast" || cast == "IntegralToPointer")
                return SourceExpr{.kind = SourceExprKind::Cast, .text = type, .args = {std::move(result)}, .cType = type, .cConversion = cast};
            if (cast == "ToVoid")
                return SourceExpr{.kind = SourceExprKind::Cast, .text = "void", .args = {std::move(result)}, .cType = "void", .cConversion = cast};
            if (cast == "IntegralToFloating" || cast == "FloatingToIntegral" || cast == "IntegralCast" || cast == "FloatingCast" ||
                cast == "IntegralToBoolean" || cast == "FloatingToBoolean" || cast == "PointerToBoolean")
                return SourceExpr{.kind = SourceExprKind::Cast, .text = type, .args = {std::move(result)}, .cType = type, .cConversion = cast};
            throw std::runtime_error("C conversion has no VM lowering: " + cast + " to " + type);
        }
        if (node.kind == CKind::Integer || node.kind == CKind::Character) {
            result.kind = SourceExprKind::IntLiteral;
            if (!node.value)
                throw std::runtime_error("C integer exceeds the VM word type");
            const uint64_t number = static_cast<uint64_t>(*node.value);
            if (number > UINT32_MAX || (result.cType != "int32_t" && result.cType != "uint32_t"))
                throw std::runtime_error("C integer exceeds the VM word type");
            result.text = hexUnsigned(static_cast<uint32_t>(number)) + (result.cType == "uint32_t" ? "U" : "");
        } else if (node.kind == CKind::Floating) {
            if (result.cType != "float")
                throw std::runtime_error("The VM has no double precision type");
            result.kind = SourceExprKind::FloatLiteral;
            result.text = node.text;
            if (result.text.find_first_of(".eE") == std::string::npos)
                result.text += ".0";
            result.text += "f";
        } else if (node.kind == CKind::String) {
            const std::optional<SourceExpr> literal = parseExpressionText(node.text);
            if (!literal || literal->kind != SourceExprKind::StringLiteral)
                throw std::runtime_error("Unsupported C string literal");
            if (!inFunctionBody_)
                return *literal;
            const uint32_t ordinal = literalOrdinals_.at(node.id);
            const std::map<uint32_t, std::string>::const_iterator selected = literalBindings_.find(ordinal);
            if (selected != literalBindings_.end()) {
                const std::map<std::string, std::string>::const_iterator backing = literalTextByObject_.find(selected->second);
                if (backing == literalTextByObject_.end() || backing->second != literal->text)
                    throw std::runtime_error("String binding " + std::to_string(ordinal) + " for " + node.text +
                        " must name matching scn_string_literal storage: " + selected->second);
                usedLiteralBindings_.insert(ordinal);
                return SourceExpr{.kind = SourceExprKind::Identifier, .text = selected->second, .cType = node.type.name};
            }
            const std::map<std::string, std::string>::const_iterator storage = literalObjects_.find(literal->text);
            if (storage != literalObjects_.end()) {
                if (storage->second.empty())
                    throw std::runtime_error("Ambiguous string literal storage; provide scn_string_bindings on the function");
                return SourceExpr{.kind = SourceExprKind::Identifier, .text = storage->second, .cType = node.type.name};
            }

            std::string &name = stringObjects_[literal->text];
            if (name.empty()) {
                name = "__scn3_string_" + std::to_string(stringOrder_.size());
                while (globalNames_.count(name))
                    name += "_";
                stringOrder_.push_back(literal->text);
            }
            return SourceExpr{.kind = SourceExprKind::Identifier, .text = name, .cType = node.type.name};
        } else if (node.kind == CKind::Unary) {
            result.kind = node.postfix ? SourceExprKind::Postfix : SourceExprKind::Unary;
            result.text = node.text;
            result.args.push_back(expression(inner.at(0)));
        } else if (node.kind == CKind::Conditional) {
            result.kind = SourceExprKind::Conditional;
            result.text = "?:";
            for (const CNode &child : inner)
                result.args.push_back(expression(child));
        } else if (node.kind == CKind::Binary || node.kind == CKind::CompoundAssign) {
            result.kind = node.kind == CKind::CompoundAssign ? SourceExprKind::CompoundAssign : SourceExprKind::Binary;
            if (node.kind == CKind::CompoundAssign)
                result.cConversion = node.operand.written;
            result.text = node.text;
            for (const CNode &child : inner)
                result.args.push_back(expression(child));
        } else if (node.kind == CKind::Subscript) {
            result.kind = SourceExprKind::Index;
            result.text = "[]";
            for (const CNode &child : inner)
                result.args.push_back(expression(child));
        } else if (node.kind == CKind::Member) {

            const CNode *literal = &inner.at(0);
            while (literal->kind == CKind::Paren && !literal->children.empty())
                literal = &literal->children.at(0);
            if (literal->kind == CKind::CompoundLiteral && literal->type.name.starts_with("union") && !node.arrow) {
                const CNode *list = literal->children.empty() ? nullptr : &literal->children.at(0);
                const auto word = [](const std::string &type) { return type == "float" || type == "int32_t" || type == "uint32_t"; };
                if (!list || list->kind != CKind::InitList || list->children.size() != 1 || !word(result.cType))
                    throw std::runtime_error("C union reinterpretation requires one 4-byte member initializer");
                SourceExpr value = expression(list->children.at(0));
                if (!word(value.cType))
                    throw std::runtime_error("C union reinterpretation requires one 4-byte member initializer");
                return SourceExpr{
                    .kind = SourceExprKind::Cast, .text = result.cType, .args = {std::move(value)}, .cType = result.cType, .cConversion = "BitCast"};
            }
            result.kind = SourceExprKind::Cast;
            result.text = "." + node.name;
            SourceExpr base = expression(inner.at(0));
            if (node.arrow) {
                std::string record = base.cType;
                const size_t star = record.rfind('*');
                if (star == std::string::npos)
                    throw std::runtime_error("C arrow expression has no pointer type");
                record = trimCopy(record.substr(0, star));
                if (record.starts_with("const "))
                    record.erase(0, 6);
                base = SourceExpr{.kind = SourceExprKind::Unary, .text = "*", .args = {std::move(base)}, .cType = record};
            }
            result.args.push_back(std::move(base));
        } else if (node.kind == CKind::Call) {
            SourceExpr callee = expression(inner.at(0));
            const bool pointerObject = callee.declarationId && unit_.cDeclarations[callee.declarationId - 1].type.find("(*") != std::string::npos;
            const bool indirect = callee.kind != SourceExprKind::Identifier || pointerObject;
            result.kind = indirect ? SourceExprKind::IndirectCall : SourceExprKind::Call;
            result.text = indirect ? "indirect" : callee.text;
            if (indirect)
                result.args.push_back(callee);
            result.declarationId = callee.declarationId;
            if (result.text.starts_with("__scn_")) {
                std::string command = result.text.substr(6);
                if (command.starts_with("v")) {
                    const size_t separator = command.find('_');
                    if (separator != std::string::npos && separator > 1 &&
                        command.substr(1, separator - 1).find_first_not_of("0123456789") == std::string::npos)
                        command.erase(0, separator + 1);
                }
                if (!nativeArgumentLayout(command) && !nativeCallHasModes(command))
                    throw std::runtime_error("Unknown generated native layout");
                result.text = command;
            }
            for (size_t i = 1; i < inner.size(); ++i)
                result.args.push_back(expression(inner[i]));
        } else if (node.kind == CKind::ImplicitValue) {
            result.kind = SourceExprKind::IntLiteral;
            result.text = "0";
        } else
            throw std::runtime_error("C expression has no VM lowering: " + node.className);
        return result;
    }

    std::vector<SourceStmt> body(const CNode &node, SourceFunctionAst &function) {
        std::vector<SourceStmt> result;
        if (node.kind == CKind::Compound) {
            const uint32_t previous = currentScope_;
            currentScope_ = ++nextScope_;
            for (const CNode &child : node.children)
                appendStatement(result, child, function);
            currentScope_ = previous;
        } else
            appendStatement(result, node, function);
        return result;
    }

    void appendStatement(std::vector<SourceStmt> &output, const CNode &node, SourceFunctionAst &function) {
        const std::vector<CNode> &inner = node.children;
        SourceStmt statement;
        if (node.kind == CKind::Null)
            return;
        if (node.kind == CKind::Call && !inner.empty() && variadic_.erasedCalls.count(node.id))
            return;
        if (node.kind == CKind::Declarations) {
            for (const CNode &declaration : inner) {
                if (declaration.kind != CKind::Variable || declaration.storage != CStorage::None)
                    throw std::runtime_error("Only automatic C local declarations have a frame allocation");
                if (variadic_.cursors.count(declaration.id))
                    continue;
                const uint32_t id = declare(declaration, currentScope_);
                const SourceCDeclaration &object = unit_.cDeclarations[id - 1];
                const ObjectType type = objectType(declaration.type);
                if (const std::optional<int32_t> offset = layoutAttribute(declaration, 0x53434e02U))
                    function.requestedLocalOffsets[object.name] = *offset;
                function.localTypes[object.name] = type.element;
                function.localDeclarations.push_back({type.element, object.name, type.count, type.array});
                SourceStmt allocation;
                allocation.kind = SourceStmtKind::Declaration;
                allocation.declaration = function.localDeclarations.back();
                output.push_back(std::move(allocation));
                if (declaration.initialized) {
                    if (type.array || findScenarioDataSchema(type.element))
                        throw std::runtime_error("C aggregate local initialization is not supported");
                    statement.kind = SourceStmtKind::Assign;
                    statement.expr = reference(declaration, declaration.id);
                    statement.value = expression(initializer(declaration));
                    output.push_back(statement);
                }
            }
            return;
        }
        if (node.kind == CKind::Compound) {
            statement.kind = SourceStmtKind::Block;
            statement.body = body(node, function);
        } else if (node.kind == CKind::Return) {
            statement.kind = SourceStmtKind::Return;
            if (!inner.empty())
                statement.expr = expression(inner.at(0));
        } else if (node.kind == CKind::If) {
            statement.kind = SourceStmtKind::If;
            statement.expr = expression(inner.at(0));
            statement.body = body(inner.at(1), function);
            if (inner.size() > 2) {
                statement.elseBody = body(inner.at(2), function);
                statement.elseIf = inner.at(2).kind == CKind::If;
            }
        } else if (node.kind == CKind::Switch) {
            statement.kind = SourceStmtKind::Switch;
            statement.expr = expression(inner.at(0));
            switchBody(inner.at(1), statement, function);
        } else if (node.kind == CKind::For) {
            if (inner.size() != 5 || inner[1].kind != CKind::None)
                throw std::runtime_error("Unsupported C for declaration condition");
            statement.kind = SourceStmtKind::For;
            if (inner[0].kind != CKind::None)
                appendStatement(statement.initializer, inner[0], function);
            if (inner[2].kind != CKind::None)
                statement.expr = expression(inner[2]);
            if (inner[3].kind != CKind::None)
                statement.increment = expression(inner[3]);
            statement.body = body(inner[4], function);
        } else if (node.kind == CKind::Do) {
            statement.kind = SourceStmtKind::DoWhile;
            statement.body = body(inner.at(0), function);
            statement.expr = expression(inner.at(1));
        } else if (node.kind == CKind::While) {
            statement.kind = SourceStmtKind::While;
            statement.expr = expression(inner.at(0));
            statement.body = body(inner.at(1), function);
        } else if (node.kind == CKind::Break)
            statement.kind = SourceStmtKind::Break;
        else if (node.kind == CKind::Continue)
            statement.kind = SourceStmtKind::Continue;
        else if (node.kind == CKind::Goto) {
            statement.kind = SourceStmtKind::Goto;
            statement.label = labels_.at(node.target);
        } else if (node.kind == CKind::Label) {
            statement.kind = SourceStmtKind::Label;
            statement.label = node.name;
            output.push_back(statement);
            appendStatement(output, inner.at(0), function);
            return;
        } else {
            statement.kind = SourceStmtKind::ExprStmt;
            statement.expr = expression(node);
            if (statement.expr.kind == SourceExprKind::Cast && statement.expr.text == "void" && statement.expr.args.size() == 1 &&
                statement.expr.args[0].kind == SourceExprKind::VariadicArgument)
                return;
            if (statement.expr.kind == SourceExprKind::Binary && statement.expr.text == "=" && statement.expr.args.size() == 2) {
                SourceExpr assignment = std::move(statement.expr);
                statement.kind = SourceStmtKind::Assign;
                statement.expr = std::move(assignment.args[0]);
                statement.value = std::move(assignment.args[1]);
            }
        }
        output.push_back(std::move(statement));
    }

    void switchBody(const CNode &node, SourceStmt &statement, SourceFunctionAst &function, bool top = true) {
        const std::vector<CNode> &inner = node.children;
        if (node.kind == CKind::Compound && top) {
            for (const CNode &child : inner)
                switchBody(child, statement, function, false);
        } else if (node.kind == CKind::Case || node.kind == CKind::Default) {
            SourceSwitchCase clause;
            clause.isDefault = node.kind == CKind::Default;
            if (!clause.isDefault)
                clause.value = expression(inner.at(0));
            statement.cases.push_back(std::move(clause));
            switchBody(inner.back(), statement, function, false);
        } else {
            if (statement.cases.empty()) {
                if (node.kind != CKind::Declarations)
                    throw std::runtime_error("C switch has executable statements before its first label");
                appendStatement(statement.body, node, function);
            } else
                appendStatement(statement.cases.back().body, node, function);
        }
    }

    std::optional<uint32_t> characterConstant(const CNode &node, bool &character) {
        const std::vector<CNode> &inner = node.children;
        if (node.kind == CKind::Character || node.kind == CKind::Integer) {
            if (!node.value)
                return std::nullopt;
            character |= node.kind == CKind::Character;
            return static_cast<uint32_t>(*node.value);
        }
        if ((node.kind == CKind::Paren || node.kind == CKind::Constant) && inner.size() == 1)
            return characterConstant(inner.at(0), character);
        if ((node.kind == CKind::ImplicitCast || node.kind == CKind::ExplicitCast) && inner.size() == 1) {
            const std::string &cast = node.text, &type = node.type.name;
            if ((cast != "IntegralCast" && cast != "NoOp") || (type != "int32_t" && type != "uint32_t"))
                return std::nullopt;
            return characterConstant(inner.at(0), character);
        }
        if (node.kind == CKind::Unary && inner.size() == 1) {
            const std::optional<uint32_t> value = characterConstant(inner.at(0), character);
            const std::string &operation = node.text;
            if (!value)
                return std::nullopt;
            if (operation == "-")
                return 0U - *value;
            if (operation == "~")
                return ~*value;
            if (operation == "+")
                return value;
            return std::nullopt;
        }
        if (node.kind == CKind::Binary && inner.size() == 2) {
            const std::optional<uint32_t> left = characterConstant(inner.at(0), character), right = characterConstant(inner.at(1), character);
            const std::string &operation = node.text;
            if (!left || !right)
                return std::nullopt;
            if (operation == "|")
                return *left | *right;
            if (operation == "&")
                return *left & *right;
            if (operation == "^")
                return *left ^ *right;
            if (operation == "+")
                return *left + *right;
            if (operation == "-")
                return *left - *right;
            if (operation == "<<" && *right < 32)
                return *left << *right;
            if (operation == ">>" && *right < 32)
                return inner.at(0).type.name == "uint32_t" ? *left >> *right : static_cast<uint32_t>(static_cast<int32_t>(*left) >> *right);
        }
        return std::nullopt;
    }

    void collectLabels(const CNode &node) {
        if (node.kind == CKind::Label)
            labels_[node.target] = node.name;
        for (const CNode &child : node.children)
            collectLabels(child);
    }

    void function(const CNode &node) {
        const CNode *compound = nullptr;
        for (const CNode &child : node.children)
            if (child.kind == CKind::Compound)
                compound = &child;
        if (!compound)
            return;
        SourceFunctionAst result;
        result.name = node.name;
        result.semantics = SourceSemantics::C11;
        if (const std::optional<int32_t> size = layoutAttribute(node, 0x53434e01U)) {
            if (*size < 0 || *size > 32768 || *size % 4)
                throw std::runtime_error("C stack reservation must be a nonnegative aligned displacement");
            result.requestedFrameSize = static_cast<uint32_t>(*size);
        }
        result.variadic = node.variadic;
        result.externalDefinition = node.storage != CStorage::Static;
        const CType returnType = node.type.inner.empty() ? CType{} : node.type.inner.front();
        result.returnType = returnType.name;
        if (result.returnType != "void" && !scenarioDataTypeByName(result.returnType) && !objectType(returnType).pointer)
            throw std::runtime_error("C return type has no VM lowering: " + result.returnType);
        if (result.externalDefinition) {
            if (unit_.startupFunctionSymbol)
                throw std::runtime_error("A scenario requires a unique externally visible entry function definition");
            unit_.startupFunctionSymbol = result.name;
        }
        localNames_.clear();
        labels_.clear();
        currentScope_ = ++nextScope_;
        for (const CNode &child : node.children)
            if (child.kind == CKind::Parameter) {
                const uint32_t id = declare(child, currentScope_);
                const SourceCDeclaration &parameter = unit_.cDeclarations[id - 1];
                if (parameter.type != "float" && parameter.type != "int32_t" && parameter.type != "uint32_t" && parameter.type != "int8_t" &&
                    parameter.type != "int16_t" && !objectType(child.type).pointer)
                    throw std::runtime_error("C parameter type has no VM lowering: " + parameter.type);
                result.params.push_back(parameter.name);
                result.localTypes[parameter.name] = parameter.type;
            }
        const std::set<uint32_t> builtins = variadic_.builtins;
        variadic_ = {};
        variadic_.builtins = builtins;
        uint32_t lastParameter = 0;
        for (const CNode &child : node.children)
            if (child.kind == CKind::Parameter)
                lastParameter = child.id;
        variadic_.analyze(*compound, vaTypes_, result.variadic ? result.params.size() : 0, result.variadic ? lastParameter : 0);
        collectLabels(*compound);
        result.body.kind = SourceStmtKind::Block;
        literalBindings_ = stringBindings(node);
        literalOrdinals_.clear();
        usedLiteralBindings_.clear();
        std::map<uint32_t, std::set<uint32_t>> positions;
        collectStringLiterals(*compound, positions);
        uint32_t ordinal = 0;
        for (const std::pair<const uint32_t, std::set<uint32_t>> &position : positions) {
            for (const uint32_t id : position.second)
                literalOrdinals_.emplace(id, ordinal);
            ++ordinal;
        }
        inFunctionBody_ = true;
        result.body.body = body(*compound, result);
        inFunctionBody_ = false;
        if (usedLiteralBindings_.size() != literalBindings_.size())
            throw std::runtime_error("String binding has no corresponding code literal");
        unit_.functionOrder.push_back(result.name);
        unit_.functions.emplace(result.name, std::move(result));
    }

    void initialize(const CNode &node, const ObjectType &type, uint32_t offset) {
        const std::vector<CNode> &inner = node.children;
        if (node.kind == CKind::ImplicitValue)
            return;
        if (type.array) {
            if (node.kind == CKind::String) {
                const SourceExpr literal = expression(node);
                if (literal.text.size() > type.count)
                    throw std::runtime_error("C string exceeds its array");
                std::copy(literal.text.begin(), literal.text.end(), unit_.dataSegment.begin() + offset);
                unit_.cstrOffsetsByText[literal.text].push_back(sizeof(ScenarioHeader) + offset);
                return;
            }
            if (node.kind != CKind::InitList)
                throw std::runtime_error("C array initializer has no VM layout");
            ObjectType element = type;
            element.array = false;
            element.count = 1;
            size_t index = 0;
            for (const CNode &child : inner) {
                if (index >= type.count)
                    throw std::runtime_error("C initializer exceeds its array");
                initialize(child, element, offset + static_cast<uint32_t>(index++) * type.size);
            }
            return;
        }
        if (const ScenarioDataSchema *schema = findScenarioDataSchema(type.element); schema && !type.pointer) {
            if (node.kind != CKind::InitList)
                throw std::runtime_error("C record initializer has no VM layout");
            for (size_t i = 0; i < inner.size(); ++i) {
                if (i >= schema->fields.size())
                    throw std::runtime_error("C record field mismatch");
                const ScenarioDataField &field = schema->fields[i];
                const ObjectType member{.element = "",
                    .count = field.elementCount,
                    .array = field.elementCount > 1,
                    .size = scenarioDataTypeSize(field.type),
                    .alignment = scenarioDataTypeSize(field.type)};
                ObjectType named = member;
                static constexpr const char *names[] = {"uint8_t", "int8_t", "uint16_t", "int16_t", "uint32_t", "int32_t", "float"};

                switch (field.type) {
                case ScenarioDataType::U8:
                    named.element = names[0];
                    break;
                case ScenarioDataType::I8:
                    named.element = names[1];
                    break;
                case ScenarioDataType::U16:
                    named.element = names[2];
                    break;
                case ScenarioDataType::I16:
                    named.element = names[3];
                    break;
                case ScenarioDataType::I32:
                    named.element = names[5];
                    break;
                case ScenarioDataType::F32:
                    named.element = names[6];
                    break;
                case ScenarioDataType::ScriptOffset32:
                    named.element = scenarioPointeeCType(field.pointeeType);
                    named.pointer = true;
                    break;
                default:
                    named.element = names[4];
                    break;
                }
                initialize(inner[i], named, offset + field.offset);
            }
            return;
        }
        if (node.kind == CKind::ImplicitValue)
            return;
        if (node.kind == CKind::InitList) {
            if (!inner.empty())
                initialize(inner.at(0), type, offset);
            return;
        }
        const SourceExpr value = expression(node);
        if (type.pointer) {
            if (value.kind == SourceExprKind::IntLiteral && parseSourceIntegerLiteral(value.text) == 0)
                return;
            unit_.dataRelocations.push_back({RangeKind::DataSegment, 0, offset, value});
            return;
        }
        uint32_t bits;
        const std::optional<uint32_t> integer = sourceDataConstantValue(value, true);
        if (type.element == "float") {
            std::function<float(const SourceExpr &)> number = [&](const SourceExpr &expr) -> float {
                if (expr.kind == SourceExprKind::FloatLiteral) {
                    const uint32_t raw = parseSourceFloatBits(expr.text);
                    float f;
                    std::memcpy(&f, &raw, sizeof(f));
                    return f;
                }
                if (const std::optional<uint32_t> word = sourceDataConstantValue(expr, true))
                    return sourceConstantIsUnsigned(expr) ? static_cast<float>(*word) : static_cast<float>(static_cast<int32_t>(*word));
                if (expr.kind == SourceExprKind::Cast && expr.args.size() == 1) {
                    const float input = number(expr.args[0]);
                    if (expr.text == "float")
                        return input;
                    if (expr.text == "int32_t" && std::isfinite(input) && static_cast<double>(input) >= INT32_MIN && static_cast<double>(input) <= INT32_MAX)
                        return static_cast<float>(static_cast<int32_t>(input));
                    if (expr.text == "uint32_t" && std::isfinite(input) && input >= 0 && static_cast<double>(input) <= UINT32_MAX)
                        return static_cast<float>(static_cast<uint32_t>(input));
                    throw std::runtime_error("C constant conversion is outside its integer type");
                }
                if (expr.kind == SourceExprKind::Unary && expr.text == "-")
                    return -number(expr.args[0]);
                const std::optional<uint32_t> word = sourceDataConstantValue(expr, true);
                if (!word)
                    throw std::runtime_error("C floating initializer is not a supported constant");
                return sourceConstantIsUnsigned(expr) ? static_cast<float>(*word) : static_cast<float>(static_cast<int32_t>(*word));
            };
            const float f = number(value);
            std::memcpy(&bits, &f, sizeof(bits));
        } else {
            if (!integer)
                throw std::runtime_error("C initializer is not a supported integer constant");
            bits = *integer;
        }
        for (uint32_t i = 0; i < type.size; ++i)
            unit_.dataSegment[offset + i] = static_cast<uint8_t>(bits >> (i * 8U));
    }

    void global(const CNode &node) {
        if (node.storage == CStorage::Extern && !node.initialized)
            return;
        const std::string &name = node.name;
        if (unit_.dataBindings.count(name) || unit_.workspaceBindings.count(name))
            throw std::runtime_error("Duplicate C object definition: " + name);
        const ObjectType type = objectType(node.type);
        SourceDataBinding binding{.type = type.pointer ? "uint32_t" : type.element,
            .elementCount = type.count,
            .isArray = type.array,
            .pointer = type.pointer,
            .functionPointer = type.functionPointer,
            .pointerType = type.pointer ? type.element : std::string()};
        if (!node.initialized) {
            workspaceSize_ = (workspaceSize_ + type.alignment - 1) / type.alignment * type.alignment;
            if (const std::optional<int32_t> offset = layoutAttribute(node, 0x53434e03U)) {
                if (*offset < 0 || static_cast<uint32_t>(*offset) < workspaceSize_ || *offset % type.alignment)
                    throw std::runtime_error("C workspace placement overlaps prior storage or violates alignment");
                workspaceSize_ = static_cast<uint32_t>(*offset);
            }
            unit_.workspaceOffsets[name] = workspaceSize_;
            unit_.workspaceBindings[name] = binding;
            workspaceSize_ += type.size * type.count;
        } else {
            std::vector<uint8_t> &bytes = unit_.dataSegment;
            bytes.resize((bytes.size() + type.alignment - 1) / type.alignment * type.alignment, 0);
            const uint32_t begin = static_cast<uint32_t>(bytes.size());
            unit_.dataSymbols[name] = sizeof(ScenarioHeader) + begin;
            unit_.dataBindings[name] = binding;
            bytes.resize(bytes.size() + type.size * type.count, 0);
            initialize(initializer(node), type, begin);
        }
    }

    bool inFunctionBody_ = false;
    std::map<std::string, std::string> stringObjects_;
    std::map<std::string, std::string> literalObjects_;
    std::map<std::string, std::string> literalTextByObject_;
    std::map<uint32_t, uint32_t> literalOrdinals_;
    std::map<uint32_t, std::string> literalBindings_;
    std::set<uint32_t> usedLiteralBindings_;
    std::vector<std::string> stringOrder_;
    CVariadicAnalysis variadic_;
    std::set<uint32_t> vaTypes_;
    const CNode &tree_;
    SourceUnit unit_{};
    std::map<uint32_t, uint32_t> identities_;
    std::set<std::string> localNames_;
    std::set<std::string> globalNames_;
    std::map<uint32_t, std::string> labels_;
    uint32_t currentScope_ = 0;
    uint32_t nextScope_ = 0;
    uint32_t workspaceSize_ = 0;
};
} // namespace shenmue::scenario::c_frontend

namespace shenmue::scenario {

namespace {

void registerScriptRecords(const c_frontend::CUnit &unit) {
    std::set<std::string> records;
    for (const c_frontend::CNode &node : unit.root.children)
        if (node.kind == c_frontend::CKind::Record && node.complete && !node.name.empty() && !findScenarioDataSchema(node.name))
            records.insert(node.name);
    for (const c_frontend::CRecordLayout &layout : unit.records) {
        if (!records.count(layout.name))
            continue;
        std::vector<ScenarioScriptField> fields;
        for (const c_frontend::CRecordLayout::Field &member : layout.fields) {

            const c_frontend::CType *type = &member.type;
            uint32_t count = 1;
            if (type->kind == c_frontend::CType::Kind::Array && !type->inner.empty()) {
                count = type->count;
                type = &type->inner.front();
            }
            ScenarioScriptField field{.name = member.name, .offset = member.offset, .elementCount = count, .type = ScenarioDataType::U32};
            if (type->kind == c_frontend::CType::Kind::Pointer && !type->inner.empty()) {
                field.type = ScenarioDataType::ScriptOffset32;
                field.pointeeType = type->inner.front().written;
                if (field.pointeeType.starts_with("const "))
                    field.pointeeType.erase(0, 6);
            } else {
                const std::optional<ScenarioDataType> known = scenarioDataTypeByName(c_frontend::cTypeName(type->written));
                if (!known)
                    throw std::runtime_error("Script record member has no VM data type: " + layout.name + "." + member.name);
                field.type = *known;
            }
            fields.push_back(field);
        }
        registerScenarioScriptSchema(layout.name, layout.size, layout.alignment, fields);
    }
}

} // namespace

SourceUnit parseScenarioC(const std::string &text) {
    const c_frontend::CUnit unit = c_frontend::parseCUnit(text);
    registerScriptRecords(unit);
    return c_frontend::CFrontend(unit.root).parse();
}
} // namespace shenmue::scenario
