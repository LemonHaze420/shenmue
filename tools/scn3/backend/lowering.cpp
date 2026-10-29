#include "scn3/backend/compiler.hpp"
#include "scn3/backend/lowering.hpp"
#include "scn3/backend/object.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

VmFunctionObject VmLowering::lower() {
    functionBody();
    for (std::pair<const std::string, uint32_t> &label : labels_)
        label.second -= function_.offset;
    return VmFunctionObject{.bytes = std::move(bytes_), .relocations = std::move(patches_), .labels = std::move(labels_)};
}

std::vector<uint8_t> VmLowering::link(VmFunctionObject object) {
    bytes_ = std::move(object.bytes);
    patches_ = std::move(object.relocations);
    labels_ = std::move(object.labels);
    for (std::pair<const std::string, uint32_t> &label : labels_)
        label.second += function_.offset;
    for (const VmRelocation &patch : patches_) {
        const bool relative = patch.opcode == encode(Control::Jump) || patch.opcode == encode(Control::BranchFalse);
        const bool based = patch.opcode == encode(Control::LocalCall) || patch.opcode == encode(Control::LocalJump) ||
            patch.opcode == encode(Control::ScnfOffset) || (patch.opcode >= encode(Address::Relative1) && patch.opcode <= encode(Address::Relative3));
        bool symbolic = false;
        uint32_t value = constant(patch.value, patch.opcode, symbolic);
        if (symbolic) {
            if (relative) {
                const int64_t displacement = static_cast<int64_t>(value) - (function_.offset + patch.position + immWidthBytes(patch.width));
                const int64_t limit = patch.width == 1 ? 128LL : patch.width == 2 ? 32768LL : 2147483648LL;
                if (displacement < -limit || displacement >= limit)
                    fail("branch target does not fit the explicit immediate width");
                value = static_cast<uint32_t>(displacement);
            } else if (based) {
                const int64_t displacement = static_cast<int64_t>(value) - function_.explicitBase.value_or(0);
                const int64_t limit = patch.width == 1 ? 128LL : patch.width == 2 ? 32768LL : 2147483648LL;
                if (displacement < -limit || displacement >= limit)
                    fail("based target does not fit the signed immediate width");
                value = static_cast<uint32_t>(displacement);
            }
        }
        const uint32_t mask = patch.width == 1 ? 0xFFU : patch.width == 2 ? 0xFFFFU : 0xFFFFFFFFU;
        if ((value & ~mask) != 0 && (value | mask) != 0xFFFFFFFFU)
            fail("operand does not fit its explicit immediate width");
        for (uint32_t i = 0; i < immWidthBytes(patch.width); ++i)
            bytes_[patch.position + i] = static_cast<uint8_t>(value >> (i * 8U));
    }
    return bytes_;
}

[[noreturn]] void VmLowering::fail(const std::string &message) const {
    throw std::runtime_error("explicit source " + function_.name + ": " + message);
}

uint32_t VmLowering::constant(const SourceExpr &expr, uint8_t opcode, bool &symbolic) const {
    if (expr.kind == SourceExprKind::IntLiteral)
        return parseSourceIntegerLiteral(expr.text);
    if (expr.kind == SourceExprKind::Unary && expr.text == "-" && expr.args.size() == 1) {
        const uint32_t value = constant(expr.args[0], opcode, symbolic);
        if (symbolic)
            fail("negating an address symbol is not supported");
        return 0U - value;
    }
    if (expr.kind == SourceExprKind::Unary && expr.text == "&" && expr.args.size() == 1)
        return constant(expr.args[0], opcode, symbolic);
    if (expr.kind == SourceExprKind::Identifier) {
        const std::map<std::string, uint32_t>::const_iterator label = labels_.find(expr.text);
        if (label != labels_.end()) {
            symbolic = true;
            return label->second;
        }
        const std::map<std::string, SourceFunctionAst>::const_iterator function = unit_.functions.find(expr.text);
        if (function != unit_.functions.end()) {
            symbolic = true;
            return function->second.offset;
        }
        const std::map<std::string, uint32_t>::const_iterator code = unit_.codeLabels.find(expr.text);
        if (code != unit_.codeLabels.end()) {
            symbolic = true;
            return code->second;
        }
        const std::map<std::string, uint32_t>::const_iterator data = unit_.dataSymbols.find(expr.text);
        if (data != unit_.dataSymbols.end()) {
            symbolic = true;
            return data->second;
        }
        if (opcode < kAddressClass && nativeCall(controlOperation(opcode)) && unit_.abiProfile == "s1_pc") {
            for (const abi::FamilySpec &family : abi::kFamilies) {
                if (family.opcodeFamily != opcode)
                    continue;
                const std::optional<uint32_t> dispatch = lookupAbiBytecodeIndexByName(family.family, expr.text);
                if (dispatch)
                    return *dispatch;
            }
        }
        if (const std::optional<uint32_t> value = sourceDataConstantValue(expr, cUnit()))
            return *value;
        if (const std::optional<uint32_t> value = lookupAbiValueByName(expr.text))
            return *value;
    }
    if (expr.kind == SourceExprKind::Call && expr.args.size() == 1 && expr.args[0].kind == SourceExprKind::Identifier) {
        if (const ScenarioDataSchema *schema = findScenarioDataSchema(expr.args[0].text)) {
            if (expr.text == "sizeof")
                return schema->size;
        }
    }
    if (expr.kind == SourceExprKind::Call && expr.text == "offsetof" && expr.args.size() == 2) {
        const ScenarioDataSchema *schema = findScenarioDataSchema(expr.args[0].text);
        if (schema)
            for (const ScenarioDataField &field : schema->fields)
                if (expr.args[1].text == field.name)
                    return field.offset;
    }
    if (expr.kind != SourceExprKind::Binary) {
        const std::optional<SourceDataAddress> data = resolveSourceDataAddress(unit_, expr);
        if (data) {
            symbolic = true;
            return data->offset;
        }
    }
    if (expr.kind == SourceExprKind::Binary && expr.args.size() == 2 && (expr.text == "+" || expr.text == "-")) {
        bool leftSymbol = false;
        bool rightSymbol = false;
        const uint32_t left = constant(expr.args[0], opcode, leftSymbol);
        const uint32_t right = constant(expr.args[1], opcode, rightSymbol);
        if (leftSymbol && rightSymbol && expr.text == "+")
            fail("cannot add two address symbols");
        if (!leftSymbol && rightSymbol && expr.text == "-")
            fail("subtracting an address from a numeric value is not supported");
        symbolic = leftSymbol != rightSymbol;
        return expr.text == "+" ? left + right : left - right;
    }
    fail("unresolved operand '" + expr.text + "' (native names require abi s1_pc)");
}

void VmLowering::operand(uint8_t opcode, const SourceExpr *expr) {
    if (!expr || (expr->kind == SourceExprKind::Identifier && expr->text == "vm_operand")) {
        bytes_.push_back(opcode);
        return;
    }
    if (expr->kind != SourceExprKind::Cast || expr->args.size() != 1)
        fail("operand requires an explicit (i8), (i16), (i32), or vm_operand");
    const std::optional<uint8_t> width = sourceImmediateWidthCallName(expr->text);
    if (!width)
        fail("unknown operand encoding " + expr->text);
    bytes_.push_back(static_cast<uint8_t>(opcode | (*width << 4U)));
    patches_.push_back(VmRelocation{.position = static_cast<uint32_t>(bytes_.size()), .width = *width, .opcode = opcode, .value = expr->args[0]});
    bytes_.resize(bytes_.size() + immWidthBytes(*width), 0);
}

uint8_t VmLowering::literalWidth(uint32_t value) {
    const int32_t signedValue = static_cast<int32_t>(value);
    return signedValue >= -128 && signedValue <= 127 ? 1 : signedValue >= -32768 && signedValue <= 32767 ? 2 : 3;
}

void VmLowering::fixedOperand(uint8_t opcode, uint32_t value, uint8_t width) {
    SourceExpr literal{.kind = SourceExprKind::IntLiteral, .text = hexUnsigned(value)};
    SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i" + std::to_string(immWidthBytes(width) * 8U), .args = {std::move(literal)}};
    operand(opcode, &encoded);
}

bool VmLowering::scaledIndex(const SourceExpr &index, uint32_t stride) {
    if (cUnit())
        if (const std::optional<uint32_t> value = sourceDataConstantValue(index, true)) {
            const uint32_t bytes = *value * stride;
            if (!bytes)
                return false;
            fixedOperand(encode(Address::Literal), bytes, literalWidth(bytes));
            return true;
        }
    expression(index);
    scaleIndex(stride);
    return true;
}

void VmLowering::scaleIndex(uint32_t stride) {
    if (stride == 1)
        return;
    if (stride == 4 || stride == 2) {
        fixedOperand(encode(Address::Literal), stride == 4 ? 2U : 1U, 1);
        bytes_.push_back(encode(Alu::ShiftLeft));
        return;
    }
    fixedOperand(encode(Address::Literal), stride, literalWidth(stride));
    bytes_.push_back(encode(Alu::Multiply));
}

VmFunctionObject lowerVmFunction(const SourceUnit &unit, const SourceFunctionAst &function) {
    const SourceFunctionAst allocated = allocateCLocals(function);
    VmLowering compiler(unit, allocated);
    return compiler.lower();
}

std::vector<uint8_t> linkVmFunction(const SourceUnit &unit, const SourceFunctionAst &function, VmFunctionObject object) {
    VmLowering compiler(unit, function);
    return compiler.link(std::move(object));
}

std::vector<uint8_t> compileExplicitFunction(const SourceUnit &unit, const SourceFunctionAst &function) {
    return linkVmFunction(unit, function, lowerVmFunction(unit, function));
}

} // namespace shenmue::scenario
