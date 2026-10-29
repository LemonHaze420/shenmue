#include "scn3/backend/lowering.hpp"
#include "scn3/backend/types.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario {

const SourceLocalDeclaration *VmLowering::localDeclaration(const std::string &name) const {
    for (const SourceLocalDeclaration &declaration : function_.localDeclarations)
        if (declaration.name == name)
            return &declaration;
    return nullptr;
}

bool VmLowering::slot(const SourceExpr &expr, bool address) {

    if (cUnit() && expr.kind == SourceExprKind::Index && expr.args.size() == 2) {
        const SourceExpr *base = &expr;
        while (base->kind == SourceExprKind::Index && base->args.size() == 2 && sourceDataConstantValue(base->args[1], true) == 0U)
            base = &base->args[0];
        const std::map<std::string, SourceDataBinding>::const_iterator array =
            base->kind == SourceExprKind::Identifier ? unit_.workspaceBindings.find(base->text) : unit_.workspaceBindings.end();
        if (base != &expr && array != unit_.workspaceBindings.end() && array->second.isArray) {
            SourceExpr element = *base;
            element.cConversion.clear();
            return slot(element, address);
        }
    }
    if (expr.kind != SourceExprKind::Identifier)
        return false;
    AddrBase base = AddrBase::CurrBase;
    uint32_t offset = 0;
    uint8_t width = 0;
    const std::map<std::string, uint32_t>::const_iterator allocated = function_.localOffsets.find(expr.text);
    if (allocated != function_.localOffsets.end()) {
        const SourceLocalDeclaration *declaration = localDeclaration(expr.text);
        if (!address && declaration) {
            if (declaration->isArray)
                address = true;
            else if (findScenarioDataSchema(declaration->type))
                fail("record values require an explicit member or address");
        }
        base = AddrBase::PrevSp;
        offset = allocated->second;
        width = literalWidth(offset);
        offset &= width == 1 ? 0xFFU : 0xFFFFU;
    } else if ((function_.semantics == SourceSemantics::C11)) {
        size_t index = 0;
        for (; index < function_.params.size(); ++index)
            if (function_.params[index] == expr.text)
                break;
        if (index != function_.params.size()) {
            base = AddrBase::PrevSp;
            offset = static_cast<uint32_t>(index) * 4U + 8U;
            width = literalWidth(offset);
        } else {
            const std::map<std::string, uint32_t>::const_iterator workspace = unit_.workspaceOffsets.find(expr.text);
            if (workspace != unit_.workspaceOffsets.end()) {
                base = AddrBase::ScenarioWork;
                offset = workspace->second;
                width = offset <= 0xFFFFU ? 2 : 3;
            } else {
                const std::optional<SourceSlotRef> task = taskSlotRefFromName(expr.text);
                if (!task || task->aggregateField || cUnit())
                    return false;
                base = AddrBase::ScenarioWork;
                offset = task->byteOffset;
                width = offset <= 0xFFFFU ? 2 : 3;
            }
        }
    } else if (unit_.workspaceOffsets.count(expr.text)) {
        base = AddrBase::ScenarioWork;
        offset = unit_.workspaceOffsets.at(expr.text);
        width = offset <= 0xFFFFU ? 2 : 3;
    } else if (const std::optional<uint32_t> arg = argSlotOffsetFromName(expr.text)) {
        base = AddrBase::PrevSp;
        offset = *arg;
        width = literalWidth(offset);
    } else if (const std::optional<SourceSlotRef> local = localSlotRefFromName(expr.text); local && !local->aggregateField) {
        base = AddrBase::PrevSp;
        offset = local->byteOffset;
        width = offset <= 0xFFU ? 1 : 2;
    } else if (const std::optional<SourceSlotRef> task = taskSlotRefFromName(expr.text); task && !task->aggregateField) {
        base = AddrBase::ScenarioWork;
        offset = task->byteOffset;
        width = offset <= 0xFFFFU ? 2 : 3;
    } else
        return false;
    uint8_t access = sourceIdentifierAccessWidth(expr.text);
    if ((function_.semantics == SourceSemantics::C11) && function_.localTypes.count(expr.text)) {
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(function_.localTypes.at(expr.text));
        if (dataType)
            access = static_cast<uint8_t>(scenarioDataTypeSize(*dataType));
        else if (!address && findScenarioDataSchema(function_.localTypes.at(expr.text)))
            fail("C aggregate values require a member selection or address");
    }
    const std::map<std::string, SourceDataBinding>::const_iterator workspace = unit_.workspaceBindings.find(expr.text);
    if (allocated == function_.localOffsets.end() && !function_.localTypes.count(expr.text) && workspace != unit_.workspaceBindings.end()) {
        if (const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(workspace->second.type.substr(0, workspace->second.type.find('['))))
            access = static_cast<uint8_t>(scenarioDataTypeSize(*dataType));
        else if (!address)
            fail("C aggregate values require a member selection or address");
    }
    const uint8_t kind = address ? 3 : access == 1 ? 0 : access == 2 ? 1 : 2;
    fixedOperand(encode(addressOperation(base, kind)), offset, width);
    std::optional<ScenarioDataType> dataType;
    if (!address && (function_.semantics == SourceSemantics::C11) && function_.localTypes.count(expr.text))
        dataType = scenarioDataTypeByName(function_.localTypes.at(expr.text));
    else if (!address && workspace != unit_.workspaceBindings.end() && !function_.localTypes.count(expr.text))
        dataType = scenarioDataTypeByName(workspace->second.type.substr(0, workspace->second.type.find('[')));
    if (dataType && (*dataType == ScenarioDataType::U8 || *dataType == ScenarioDataType::U16)) {
        const uint32_t mask = *dataType == ScenarioDataType::U8 ? 0xFFU : 0xFFFFU;
        fixedOperand(encode(Address::Literal), mask, literalWidth(mask));
        bytes_.push_back(encode(Alu::And));
    }
    return true;
}

void VmLowering::rejectPointerObjectUse(const SourceExpr &expr) const {
    if (expr.kind == SourceExprKind::Identifier && !function_.localTypes.count(expr.text)) {
        const std::map<std::string, SourceDataBinding>::const_iterator binding = unit_.dataBindings.find(expr.text);
        if (binding != unit_.dataBindings.end() && binding->second.pointer)
            fail("Unsupported operation: C pointer object indirection");
    }
    for (const SourceExpr &argument : expr.args)
        rejectPointerObjectUse(argument);
}

bool VmLowering::dataAddress(const SourceExpr &expr) {
    if (cUnit() && unit_.cDeclarations.empty())
        rejectPointerObjectUse(expr);
    if (expr.kind == SourceExprKind::Binary)
        return false;
    const std::optional<SourceDataAddress> type = resolveSourceDataAddress(unit_, expr, true);
    if (!type)
        return false;
    const std::optional<SourceDataAddress> fixed = resolveSourceDataAddress(unit_, expr);
    if (fixed) {
        SourceExpr encoded{.kind = SourceExprKind::Cast, .text = "i32", .args = {expr}};
        operand(encode(Address::Relative3), &encoded);
        return true;
    }
    if (expr.kind == SourceExprKind::Index && expr.args.size() == 2) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit_, expr.args[0], true);
        if (!base || !dataAddress(expr.args[0]))
            fail("unknown indexed data base");
        if (explicitExpressionIsFloat(expr.args[1], &unit_, &function_))
            fail("data index must be an integer");
        if (scaledIndex(expr.args[1], base->stride))
            bytes_.push_back(encode(Alu::Add));
        return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".") && expr.args.size() == 1) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit_, expr.args[0], true);
        if (!base || !dataAddress(expr.args[0]))
            fail("unknown record data base");
        const uint32_t delta = type->offset - base->offset;
        if (delta) {
            fixedOperand(encode(Address::Literal), delta, literalWidth(delta));
            bytes_.push_back(encode(Alu::Add));
        }
        return true;
    }
    fail("unsupported typed data address");
}

void VmLowering::loadDataValue(ScenarioDataType type) {
    const uint32_t width = scenarioDataTypeSize(type);
    bytes_.push_back(width == 1 ? encode(Alu::Load8) : width == 2 ? encode(Alu::Load16) : encode(Alu::Load32));
    if (type == ScenarioDataType::U8 || type == ScenarioDataType::U16) {
        const uint32_t mask = type == ScenarioDataType::U8 ? 0xFFU : 0xFFFFU;
        fixedOperand(encode(Address::Literal), mask, literalWidth(mask));
        bytes_.push_back(encode(Alu::And));
    }
}

void VmLowering::convertCValue(ScenarioDataType type, std::optional<uint32_t> value) {
    const uint32_t width = scenarioDataTypeSize(type);
    if (width == 4)
        return;
    if (value) {
        const uint32_t converted = type == ScenarioDataType::U8 ? *value & 0xFFU :
            type == ScenarioDataType::U16                       ? *value & 0xFFFFU :
            type == ScenarioDataType::I8                        ? static_cast<uint32_t>(static_cast<int8_t>(*value)) :
                                                                  static_cast<uint32_t>(static_cast<int16_t>(*value));
        if (converted == *value)
            return;
    }
    if (type == ScenarioDataType::U8 || type == ScenarioDataType::U16) {
        const uint32_t mask = width == 1 ? 0xFFU : 0xFFFFU;
        fixedOperand(encode(Address::Literal), mask, literalWidth(mask));
        bytes_.push_back(encode(Alu::And));
    } else {
        const uint32_t shift = 32U - width * 8U;
        fixedOperand(encode(Address::Literal), shift, 1);
        bytes_.push_back(encode(Alu::ShiftLeft));
        fixedOperand(encode(Address::Literal), shift, 1);
        bytes_.push_back(encode(Alu::ShiftRight));
    }
}

bool VmLowering::localMemberAddress(const SourceExpr &expr) {
    const std::optional<ExplicitLocalMember> member = explicitLocalMember(expr, function_, &unit_);
    if (!member)
        return false;
    if ((function_.semantics == SourceSemantics::C11) && function_.localOffsets.count(member->base.text)) {
        const uint32_t offset = function_.localOffsets.at(member->base.text) + member->field->offset;
        const uint8_t width = literalWidth(offset);
        fixedOperand(encode(Address::LocalAddress), offset & (width == 1 ? 0xFFU : 0xFFFFU), width);
        return true;
    }
    if (!slot(member->base, true))
        fail("typed frame binding requires a local_HEX slot");
    if (member->field->offset) {
        fixedOperand(encode(Address::Literal), member->field->offset, literalWidth(member->field->offset));
        bytes_.push_back(encode(Alu::Add));
    }
    return true;
}

bool VmLowering::localArrayAddress(const SourceExpr &expr) {
    if (!(function_.semantics == SourceSemantics::C11) || expr.kind != SourceExprKind::Index || expr.args.size() != 2 ||
        expr.args[0].kind != SourceExprKind::Identifier)
        return false;
    const SourceLocalDeclaration *declaration = localDeclaration(expr.args[0].text);
    if (!declaration || !declaration->isArray)
        return false;
    const std::optional<ScenarioDataType> dataType = cDataType(declaration->type);
    if (!dataType)
        fail("local array member access requires a supported element type");
    slot(expr.args[0], true);
    if (const std::optional<uint32_t> index = sourceDataConstantValue(expr.args[1], cUnit())) {
        if (*index >= declaration->count)
            fail("local array index exceeds its declaration");
        const uint32_t delta = *index * scenarioDataTypeSize(*dataType);
        if (delta) {
            fixedOperand(encode(Address::Literal), delta, literalWidth(delta));
            bytes_.push_back(encode(Alu::Add));
        }
    } else {
        if (explicitExpressionIsFloat(expr.args[1], &unit_, &function_))
            fail("array index must be an integer");
        if (scaledIndex(expr.args[1], scenarioDataTypeSize(*dataType)))
            bytes_.push_back(encode(Alu::Add));
    }
    return true;
}

std::optional<ScenarioDataType> VmLowering::cDataType(const std::string &type) const {
    if (type.find('*') != std::string::npos)
        return ScenarioDataType::U32;
    if (type == "int")
        return ScenarioDataType::I32;
    if (type == "unsigned int")
        return ScenarioDataType::U32;
    return scenarioDataTypeByName(type);
}

uint32_t VmLowering::cObjectSize(const std::string &qualified) const {
    std::string type = qualified;
    while (type.starts_with("const ") || type.starts_with("volatile "))
        type.erase(0, type.find(' ') + 1);
    if (const std::optional<ScenarioDataType> dataType = cDataType(type))
        return scenarioDataTypeSize(*dataType);
    if (const ScenarioDataSchema *record = findScenarioDataSchema(type))
        return record->size;
    if (const size_t bracket = type.rfind('['); bracket != std::string::npos && type.ends_with("]"))
        return cObjectSize(trimCopy(type.substr(0, bracket))) * static_cast<uint32_t>(std::stoul(type.substr(bracket + 1)));
    fail("C object has no established layout: " + type);
}

bool VmLowering::cIndirectAddress(const SourceExpr &expr) {
    if (!cUnit() || expr.cType.empty())
        return false;
    if (expr.kind == SourceExprKind::Unary && expr.text == "*" && expr.args.size() == 1) {
        expression(expr.args[0]);
        return true;
    }
    if (expr.kind == SourceExprKind::Index && expr.args.size() == 2 && expr.args[0].cType.find('*') != std::string::npos) {
        const SourceExpr &base = expr.args[0];
        const SourceLocalDeclaration *local = base.kind == SourceExprKind::Identifier ? localDeclaration(base.text) : nullptr;
        const size_t row = base.cType.find("(*)");
        const std::string pointee =
            row != std::string::npos ? trimCopy(base.cType.substr(0, row) + base.cType.substr(row + 3)) : trimCopy(base.cType.substr(0, base.cType.rfind('*')));
        const std::optional<ScenarioDataType> effective = cDataType(pointee);
        if (local && local->isArray && effective && effective == cDataType(local->type))
            return false;
        const std::map<std::string, SourceDataBinding>::const_iterator data = unit_.dataBindings.find(base.text);
        if (base.kind == SourceExprKind::Identifier && data != unit_.dataBindings.end() && data->second.isArray && effective &&
            effective == cDataType(data->second.type))
            return false;

        const std::optional<uint32_t> column = sourceDataConstantValue(expr.args[1], true);
        const size_t rows = base.kind == SourceExprKind::Index && base.args.size() == 2 ? base.args[0].cType.find("(*)") : std::string::npos;
        if (column && *column && rows != std::string::npos && base.cConversion == "ArrayToPointerDecay" && !sourceDataConstantValue(base.args[1], true)) {
            const std::string &arrayType = base.args[0].cType;
            expression(base.args[0]);
            scaledIndex(base.args[1], cObjectSize(trimCopy(arrayType.substr(0, rows) + arrayType.substr(rows + 3))));
            const uint32_t bytes = *column * cObjectSize(pointee);
            fixedOperand(encode(Address::Literal), bytes, literalWidth(bytes));
            bytes_.push_back(encode(Alu::Add));
            bytes_.push_back(encode(Alu::Add));
            return true;
        }

        const std::optional<uint32_t> constantRow =
            rows != std::string::npos && base.cConversion == "ArrayToPointerDecay" ? sourceDataConstantValue(base.args[1], true) : std::nullopt;
        if (column && constantRow) {
            const std::string &arrayType = base.args[0].cType;
            expression(base.args[0]);
            const uint32_t bytes =
                *constantRow * cObjectSize(trimCopy(arrayType.substr(0, rows) + arrayType.substr(rows + 3))) + *column * cObjectSize(pointee);
            if (bytes) {
                fixedOperand(encode(Address::Literal), bytes, literalWidth(bytes));
                bytes_.push_back(encode(Alu::Add));
            }
            return true;
        }
        expression(base);
        if (scaledIndex(expr.args[1], cObjectSize(pointee)))
            bytes_.push_back(encode(Alu::Add));
        return true;
    }
    if (expr.kind == SourceExprKind::Cast && expr.text.starts_with(".") && expr.args.size() == 1) {
        const SourceExpr &base = expr.args[0];
        const ScenarioDataSchema *record = findScenarioDataSchema(base.cType);
        if (!record || base.kind == SourceExprKind::Identifier)
            return false;
        const ScenarioDataField *member = nullptr;
        for (const ScenarioDataField &field : record->fields)
            if (expr.text.substr(1) == field.name)
                member = &field;
        if (!member || !cIndirectAddress(base))
            return false;
        if (member->offset) {
            fixedOperand(encode(Address::Literal), member->offset, literalWidth(member->offset));
            bytes_.push_back(encode(Alu::Add));
        }
        return true;
    }
    return false;
}

} // namespace shenmue::scenario
