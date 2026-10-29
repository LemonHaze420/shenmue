#include "scn3/frontend/c_frontend.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/motion.hpp"

#include <chrono>
#include <random>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Attr.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecordLayout.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/TypeTraits.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/VirtualFileSystem.h>

namespace shenmue::scenario {

static std::filesystem::path g_scriptIncludeDirectory;
static std::vector<std::filesystem::path> g_includeDirectories;

void setScenarioCIncludeDirectory(const std::filesystem::path &directory) {
    g_scriptIncludeDirectory = directory;
}

void addScenarioCIncludeDirectory(const std::filesystem::path &directory) {
    const std::filesystem::path path = std::filesystem::absolute(directory).lexically_normal();
    for (const std::filesystem::path &known : g_includeDirectories)
        if (known == path)
            return;
    g_includeDirectories.push_back(path);
}

std::optional<std::filesystem::path> findScenarioCHeader(const std::string &name, const std::filesystem::path &directory) {
    for (const std::filesystem::path &base : {directory, g_scriptIncludeDirectory, g_scriptIncludeDirectory.parent_path()})
        if (!base.empty() && std::filesystem::is_regular_file(base / name))
            return std::filesystem::absolute(base / name).lexically_normal();
    for (const std::filesystem::path &base : g_includeDirectories)
        if (std::filesystem::is_regular_file(base / name))
            return base / name;
    return std::nullopt;
}

namespace c_frontend {
namespace {

class Importer {
  public:
    explicit Importer(clang::ASTContext &context) : context_(context), sources_(context.getSourceManager()), policy_(context.getPrintingPolicy()) {
    }

    CUnit import() {
        CUnit unit;
        unit.root = CNode{.kind = CKind::Unit, .className = "TranslationUnitDecl"};
        for (const clang::Decl *decl : context_.getTranslationUnitDecl()->decls()) {
            unit.root.children.push_back(declaration(decl));
            if (const auto *record = llvm::dyn_cast<clang::RecordDecl>(decl);
                record && record->isCompleteDefinition() && !record->isInvalidDecl() && !record->getName().empty())
                unit.records.push_back(layout(record));
        }
        return unit;
    }

  private:
    uint32_t identity(const clang::Decl *decl) {
        uint32_t &id = declarations_[decl];
        if (!id)
            id = ++next_;
        return id;
    }

    static CKind declarationKind(const clang::Decl *decl) {
        switch (decl->getKind()) {
        case clang::Decl::Function:
            return CKind::Function;
        case clang::Decl::Var:
            return CKind::Variable;
        case clang::Decl::ParmVar:
            return CKind::Parameter;
        case clang::Decl::Typedef:
            return CKind::Typedef;
        case clang::Decl::Record:
            return CKind::Record;
        case clang::Decl::Enum:
            return CKind::Enum;
        case clang::Decl::StaticAssert:
            return CKind::StaticAssert;
        case clang::Decl::Empty:
            return CKind::Empty;
        default:
            return CKind::Other;
        }
    }

    static CKind statementKind(const clang::Stmt *statement) {
        switch (statement->getStmtClass()) {
        case clang::Stmt::CompoundStmtClass:
            return CKind::Compound;
        case clang::Stmt::DeclStmtClass:
            return CKind::Declarations;
        case clang::Stmt::ReturnStmtClass:
            return CKind::Return;
        case clang::Stmt::IfStmtClass:
            return CKind::If;
        case clang::Stmt::SwitchStmtClass:
            return CKind::Switch;
        case clang::Stmt::ForStmtClass:
            return CKind::For;
        case clang::Stmt::DoStmtClass:
            return CKind::Do;
        case clang::Stmt::WhileStmtClass:
            return CKind::While;
        case clang::Stmt::BreakStmtClass:
            return CKind::Break;
        case clang::Stmt::ContinueStmtClass:
            return CKind::Continue;
        case clang::Stmt::GotoStmtClass:
            return CKind::Goto;
        case clang::Stmt::LabelStmtClass:
            return CKind::Label;
        case clang::Stmt::NullStmtClass:
            return CKind::Null;
        case clang::Stmt::CaseStmtClass:
            return CKind::Case;
        case clang::Stmt::DefaultStmtClass:
            return CKind::Default;
        case clang::Stmt::UnaryExprOrTypeTraitExprClass:
            return CKind::TypeTrait;
        case clang::Stmt::DeclRefExprClass:
            return CKind::Reference;
        case clang::Stmt::VAArgExprClass:
            return CKind::VariadicArgument;
        case clang::Stmt::BinaryOperatorClass:
            return CKind::Binary;
        case clang::Stmt::CompoundAssignOperatorClass:
            return CKind::CompoundAssign;
        case clang::Stmt::UnaryOperatorClass:
            return CKind::Unary;
        case clang::Stmt::ConditionalOperatorClass:
            return CKind::Conditional;
        case clang::Stmt::ParenExprClass:
            return CKind::Paren;
        case clang::Stmt::ConstantExprClass:
            return CKind::Constant;
        case clang::Stmt::GenericSelectionExprClass:
            return CKind::Generic;
        case clang::Stmt::ImplicitCastExprClass:
            return CKind::ImplicitCast;
        case clang::Stmt::CStyleCastExprClass:
            return CKind::ExplicitCast;
        case clang::Stmt::IntegerLiteralClass:
            return CKind::Integer;
        case clang::Stmt::CharacterLiteralClass:
            return CKind::Character;
        case clang::Stmt::FloatingLiteralClass:
            return CKind::Floating;
        case clang::Stmt::StringLiteralClass:
            return CKind::String;
        case clang::Stmt::ArraySubscriptExprClass:
            return CKind::Subscript;
        case clang::Stmt::MemberExprClass:
            return CKind::Member;
        case clang::Stmt::CallExprClass:
            return CKind::Call;
        case clang::Stmt::ImplicitValueInitExprClass:
            return CKind::ImplicitValue;
        case clang::Stmt::InitListExprClass:
            return CKind::InitList;
        case clang::Stmt::CompoundLiteralExprClass:
            return CKind::CompoundLiteral;
        default:
            return CKind::Other;
        }
    }

    static CStorage storage(clang::StorageClass specifier) {
        if (specifier == clang::SC_None)
            return CStorage::None;
        return specifier == clang::SC_Static ? CStorage::Static : specifier == clang::SC_Extern ? CStorage::Extern : CStorage::Other;
    }

    CType type(clang::QualType qualified) {
        CType result;
        if (qualified.isNull())
            return result;
        result.written = clang::QualType::getAsString(qualified.split(), policy_);
        result.name = cTypeName(clang::QualType::getAsString(qualified.getSplitDesugaredType(), policy_));
        if (const auto *alias = qualified->getAs<clang::TypedefType>())
            result.alias = identity(alias->getDecl());
        for (clang::QualType part = qualified.getCanonicalType(); !part.isNull();) {
            result.constant |= part.isConstQualified();
            result.isVolatile |= part.isVolatileQualified();
            if (const clang::ArrayType *array = context_.getAsArrayType(part))
                part = array->getElementType().getCanonicalType();
            else if (const auto *pointer = part->getAs<clang::PointerType>())
                part = pointer->getPointeeType().getCanonicalType();
            else
                break;
        }
        if (const clang::ConstantArrayType *array = context_.getAsConstantArrayType(qualified)) {
            result.kind = CType::Kind::Array;
            result.count = static_cast<uint32_t>(array->getSize().getZExtValue());
            result.inner.push_back(type(array->getElementType()));
        } else if (qualified->isFunctionPointerType())
            result.kind = CType::Kind::FunctionPointer;
        else if (const auto *pointer = qualified->getAs<clang::PointerType>()) {
            result.kind = CType::Kind::Pointer;
            result.inner.push_back(type(pointer->getPointeeType()));
        } else if (const auto *function = qualified->getAs<clang::FunctionType>()) {
            result.kind = CType::Kind::Function;
            result.inner.push_back(type(function->getReturnType()));
        }
        return result;
    }

    bool isSource(const clang::Decl *decl) const {
        if (decl->isImplicit())
            return false;
        const clang::PresumedLoc location = sources_.getPresumedLoc(sources_.getExpansionLoc(decl->getLocation()));
        return location.isValid() && location.getIncludeLoc().isInvalid();
    }

    CNode attribute(const clang::Attr *attribute) {
        CNode result{.kind = CKind::Other, .className = std::string(attribute->getSpelling()), .id = ++next_};
        if (const auto *annotation = llvm::dyn_cast<clang::AnnotateAttr>(attribute)) {
            result.kind = CKind::Annotate;
            result.className = "AnnotateAttr";
            for (const clang::Expr *argument : annotation->args())
                result.children.push_back(statement(argument));
        } else if (llvm::isa<clang::BuiltinAttr>(attribute)) {
            result.kind = CKind::Builtin;
            result.className = "BuiltinAttr";
        }
        return result;
    }

    CNode declaration(const clang::Decl *decl) {
        CNode result{.kind = declarationKind(decl), .className = std::string(decl->getDeclKindName()) + "Decl", .id = identity(decl)};
        result.implicit = decl->isImplicit();
        result.source = isSource(decl);
        if (const clang::Decl *previous = decl->getPreviousDecl())
            result.previous = identity(previous);
        if (const auto *named = llvm::dyn_cast<clang::NamedDecl>(decl); named && named->getDeclName())
            result.name = named->getNameAsString();
        for (const clang::Attr *attr : decl->attrs())
            result.attributes.push_back(attribute(attr));
        if (const auto *value = llvm::dyn_cast<clang::ValueDecl>(decl))
            result.type = type(value->getType());
        else if (const auto *alias = llvm::dyn_cast<clang::TypedefNameDecl>(decl))
            result.type = type(alias->getUnderlyingType());
        if (const auto *function = llvm::dyn_cast<clang::FunctionDecl>(decl)) {
            result.storage = storage(function->getStorageClass());
            result.variadic = function->isVariadic();
            for (const clang::ParmVarDecl *parameter : function->parameters())
                result.children.push_back(declaration(parameter));
            if (function->doesThisDeclarationHaveABody())
                result.children.push_back(statement(function->getBody()));
        } else if (const auto *variable = llvm::dyn_cast<clang::VarDecl>(decl)) {
            result.storage = storage(variable->getStorageClass());
            result.initialized = variable->hasInit();
            if (const clang::Expr *initializer = variable->getInit())
                result.children.push_back(statement(initializer));
        } else if (const auto *record = llvm::dyn_cast<clang::RecordDecl>(decl))
            result.complete = record->isCompleteDefinition();
        return result;
    }

    CNode statement(const clang::Stmt *stmt) {
        if (!stmt)
            return CNode{};
        CNode result{.kind = statementKind(stmt), .className = stmt->getStmtClassName(), .id = ++next_};
        if (const auto *expression = llvm::dyn_cast<clang::Expr>(stmt))
            result.type = type(expression->getType());
        if (const auto *declarations = llvm::dyn_cast<clang::DeclStmt>(stmt)) {
            for (const clang::Decl *decl : declarations->decls())
                result.children.push_back(declaration(decl));
            return result;
        }
        if (const auto *reference = llvm::dyn_cast<clang::DeclRefExpr>(stmt)) {
            const clang::ValueDecl *decl = reference->getDecl();
            result.target = identity(decl);
            result.targetKind = declarationKind(decl);
            if (decl->getDeclName())
                result.name = decl->getNameAsString();
            result.type = type(decl->getType());
        } else if (const auto *trait = llvm::dyn_cast<clang::UnaryExprOrTypeTraitExpr>(stmt)) {
            result.name = clang::getTraitSpelling(trait->getKind());
            if (trait->isArgumentType())
                result.operand = type(trait->getArgumentType());
        } else if (const auto *cast = llvm::dyn_cast<clang::CastExpr>(stmt))
            result.text = cast->getCastKindName();
        else if (const auto *unary = llvm::dyn_cast<clang::UnaryOperator>(stmt)) {
            result.text = clang::UnaryOperator::getOpcodeStr(unary->getOpcode()).str();
            result.postfix = unary->isPostfix();
        } else if (const auto *binary = llvm::dyn_cast<clang::BinaryOperator>(stmt)) {
            result.text = clang::BinaryOperator::getOpcodeStr(binary->getOpcode()).str();
            if (const auto *compound = llvm::dyn_cast<clang::CompoundAssignOperator>(binary))
                result.operand = type(compound->getComputationResultType());
        } else if (const auto *integer = llvm::dyn_cast<clang::IntegerLiteral>(stmt)) {
            if (integer->getValue().getActiveBits() <= 64)
                result.value = static_cast<int64_t>(integer->getValue().getZExtValue());
        } else if (const auto *character = llvm::dyn_cast<clang::CharacterLiteral>(stmt))
            result.value = character->getValue();
        else if (const auto *floating = llvm::dyn_cast<clang::FloatingLiteral>(stmt)) {
            llvm::SmallString<16> spelling;
            floating->getValue().toString(spelling);
            result.text = spelling.str().str();
        } else if (const auto *literal = llvm::dyn_cast<clang::StringLiteral>(stmt)) {
            llvm::raw_string_ostream spelling(result.text);
            literal->outputString(spelling);
            spelling.flush();
            result.offset = sources_.getDecomposedLoc(sources_.getSpellingLoc(literal->getBeginLoc())).second;
        } else if (const auto *constant = llvm::dyn_cast<clang::ConstantExpr>(stmt)) {
            if (constant->getResultAPValueKind() == clang::APValue::Int) {
                const llvm::APSInt value = constant->getResultAsAPSInt();
                if (value.getActiveBits() <= 64)
                    result.value = value.isSigned() ? value.getSExtValue() : static_cast<int64_t>(value.getZExtValue());
            }
        } else if (const auto *member = llvm::dyn_cast<clang::MemberExpr>(stmt)) {
            const clang::ValueDecl *decl = member->getMemberDecl();
            if (decl && decl->getDeclName())
                result.name = decl->getNameAsString();
            result.arrow = member->isArrow();
        } else if (const auto *selection = llvm::dyn_cast<clang::GenericSelectionExpr>(stmt))
            result.selected = selection->getResultIndex() + (selection->getControllingExpr() ? 1U : 0U);
        else if (const auto *label = llvm::dyn_cast<clang::LabelStmt>(stmt)) {
            result.name = label->getName();
            result.target = identity(label->getDecl());
        } else if (const auto *jump = llvm::dyn_cast<clang::GotoStmt>(stmt))
            result.target = identity(jump->getLabel());
        for (const clang::Stmt *child : stmt->children())
            result.children.push_back(statement(child));
        return result;
    }

    CRecordLayout layout(const clang::RecordDecl *record) {
        const clang::ASTRecordLayout &recordLayout = context_.getASTRecordLayout(record);
        CRecordLayout result{.name = record->getNameAsString(),
            .size = static_cast<uint32_t>(recordLayout.getSize().getQuantity()),
            .alignment = static_cast<uint32_t>(recordLayout.getAlignment().getQuantity())};
        for (const clang::FieldDecl *field : record->fields()) {
            const int64_t bits = static_cast<int64_t>(recordLayout.getFieldOffset(field->getFieldIndex()));
            const uint32_t offset = static_cast<uint32_t>(context_.toCharUnitsFromBits(bits).getQuantity());
            result.fields.push_back({.name = field->getNameAsString(), .offset = offset, .type = type(field->getType())});
        }
        return result;
    }

    clang::ASTContext &context_;
    const clang::SourceManager &sources_;
    const clang::PrintingPolicy policy_;
    std::unordered_map<const clang::Decl *, uint32_t> declarations_;
    uint32_t next_ = 0;
};

class ImportConsumer : public clang::ASTConsumer {
  public:
    ImportConsumer(CUnit &unit, std::exception_ptr &failure) : unit_(unit), failure_(failure) {
    }

    void HandleTranslationUnit(clang::ASTContext &context) override {
        if (context.getDiagnostics().hasErrorOccurred())
            return;
        try {
            unit_ = Importer(context).import();
        } catch (...) {
            failure_ = std::current_exception();
        }
    }

  private:
    CUnit &unit_;
    std::exception_ptr &failure_;
};

class ImportAction : public clang::ASTFrontendAction {
  public:
    ImportAction(CUnit &unit, std::exception_ptr &failure) : unit_(unit), failure_(failure) {
    }

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override {
        return std::make_unique<ImportConsumer>(unit_, failure_);
    }

  private:
    CUnit &unit_;
    std::exception_ptr &failure_;
};

void runClang(const std::string &text, std::unique_ptr<clang::FrontendAction> action) {
    const std::string unique = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "_" + std::to_string(std::random_device{}());
    const std::filesystem::path base = std::filesystem::temp_directory_path() / ("scn3_c_" + unique);
    const std::string source = base.string() + ".c";
    llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem> files(new llvm::vfs::OverlayFileSystem(llvm::vfs::getRealFileSystem()));
    llvm::IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem> memory(new llvm::vfs::InMemoryFileSystem);
    files->pushOverlay(memory);
    std::vector<std::string> arguments{"scn3cc", "-x", "c", "-std=c11", "-ffreestanding", "-target", "i386-unknown-none",
        "-Werror=implicit-function-declaration", "-Werror=incompatible-pointer-types", "-Werror=int-conversion", "-pedantic-errors", "-fsyntax-only",
        "-fno-caret-diagnostics", "-resource-dir", SCN3_CLANG_RESOURCE_DIR, "-I" SCN3_C_SDK_PATH, "-include", "scenario.h"};

    for (const std::filesystem::path &header : loadedMotionHeaders())
        arguments.insert(arguments.end(), {"-include", header.string()});
    if (!g_scriptIncludeDirectory.empty())
        arguments.insert(arguments.end(), {"-iquote", g_scriptIncludeDirectory.string(), "-I", g_scriptIncludeDirectory.parent_path().string()});
    for (const std::filesystem::path &directory : g_includeDirectories)
        arguments.insert(arguments.end(), {"-I", directory.string()});
    memory->addFile(source, 0, llvm::MemoryBuffer::getMemBufferCopy(text.empty() || text.back() != '\n' ? text + "\n" : text));
    arguments.push_back(source);
    const llvm::IntrusiveRefCntPtr<clang::FileManager> manager(new clang::FileManager(clang::FileSystemOptions(), files));
    std::string diagnostics;
    llvm::raw_string_ostream output(diagnostics);
    const llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> options(new clang::DiagnosticOptions());
    options->ShowOptionNames = true;
    clang::TextDiagnosticPrinter printer(output, options.get());
    clang::tooling::ToolInvocation invocation(arguments, std::move(action), manager.get());
    invocation.setDiagnosticConsumer(&printer);
    const bool compiled = invocation.run();
    output.flush();
    if (!compiled)
        throw std::runtime_error("C compilation failed:\n" + diagnostics);
}

} // namespace

CUnit parseCUnit(const std::string &text) {
    CUnit unit;
    std::exception_ptr failure;
    runClang(text, std::make_unique<ImportAction>(unit, failure));
    if (failure)
        std::rethrow_exception(failure);
    return unit;
}

} // namespace c_frontend

void validateScenarioCSource(const std::string &text) {
    c_frontend::runClang(text, std::make_unique<clang::SyntaxOnlyAction>());
}

} // namespace shenmue::scenario
