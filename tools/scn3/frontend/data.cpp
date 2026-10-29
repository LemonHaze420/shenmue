#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"
#include "scn3/native/motion.hpp"
#include "scn3/native/symbols.hpp"

namespace shenmue::scenario {

std::optional<ScenarioDataType> scenarioDataTypeByName(std::string_view name) {
    if (name == "_Bool" || name == "u8" || name == "uint8_t" || name == "unsigned char")
        return ScenarioDataType::U8;
    if (name == "u16" || name == "uint16_t")
        return ScenarioDataType::U16;
    if (name == "u32" || name == "uint32_t")
        return ScenarioDataType::U32;
    if (name == "i8" || name == "int8_t" || name == "char")
        return ScenarioDataType::I8;
    if (name == "i16" || name == "int16_t")
        return ScenarioDataType::I16;
    if (name == "i32" || name == "int32_t")
        return ScenarioDataType::I32;
    if (name == "f32" || name == "float")
        return ScenarioDataType::F32;
    return std::nullopt;
}

bool sourceConstantIsUnsigned(const SourceExpr &expression) {
    if (expression.kind == SourceExprKind::IntLiteral)
        return !expression.text.starts_with("-") &&
            (expression.text.find_first_of("uU") != std::string::npos || parseSourceIntegerLiteral(expression.text) > 0x7FFFFFFFU);
    if (expression.kind == SourceExprKind::Identifier) {
        const std::optional<uint32_t> value = sourceDataConstantValue(expression);
        return value && *value > 0x7FFFFFFFU;
    }
    if (expression.kind == SourceExprKind::Cast)
        return expression.text == "u8" || expression.text == "u16" || expression.text == "u32" || expression.text == "uint8_t" ||
            expression.text == "uint16_t" || expression.text == "uint32_t";
    if (expression.kind == SourceExprKind::Unary && expression.args.size() == 1)
        return expression.text != "!" && sourceConstantIsUnsigned(expression.args[0]);
    if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2) {
        if (expression.text == "<<" || expression.text == ">>")
            return sourceConstantIsUnsigned(expression.args[0]);
        return sourceConstantIsUnsigned(expression.args[0]) || sourceConstantIsUnsigned(expression.args[1]);
    }
    return false;
}

std::optional<std::uint32_t> sourceDataConstantValue(const SourceExpr &expression, bool cSemantics) {
    if (expression.kind == SourceExprKind::IntLiteral)
        return parseSourceIntegerLiteral(expression.text);
    if (expression.kind == SourceExprKind::Identifier) {
        if (expression.text == "true")
            return 1;
        if (expression.text == "false")
            return 0;
        if (expression.text == "SCF_Unknown0100")
            return SCF_Unknown0100;
        if (expression.text == "SCF_NativeCodeMask")
            return SCF_NativeCodeMask;
        if (const std::optional<uint32_t> value = lookupAbiValueByName(expression.text))
            return value;
        if (expression.text.size() == 17 && expression.text.starts_with("SHEN_NAME('") && expression.text.ends_with("')")) {
            uint32_t value = 0;
            for (uint32_t byte = 0; byte < 4; ++byte)
                value |= static_cast<uint32_t>(static_cast<uint8_t>(expression.text[11 + byte])) << (byte * 8U);
            return value;
        }
        if (expression.text.size() == 10 && expression.text.compare(0, 6, "CHARA_") == 0) {
            const char *name = expression.text.data() + 6;
            return static_cast<std::uint8_t>(name[0]) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[1])) << 8U) |
                (static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[2])) << 16U) |
                (static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[3])) << 24U);
        }
        if (const std::optional<std::uint32_t> animation = motionIdByName(expression.text))
            return animation;
        if (const std::optional<std::uint32_t> native = scenarioDataConstantValue(expression.text))
            return native;
        return shenTypeConstantValueByName(expression.text);
    }
    if (expression.kind == SourceExprKind::Cast && expression.args.size() == 1) {
        const std::optional<std::uint32_t> value = sourceDataConstantValue(expression.args[0], cSemantics);
        const std::optional<std::uint8_t> width = sourceImmediateWidthCallName(expression.text);
        if (value && expression.text == "_Bool")
            return *value != 0 ? 1U : 0U;
        if (value.has_value() && (expression.text == "u8" || expression.text == "uint8_t"))
            return *value & 0xFFU;
        if (value.has_value() && expression.text == "int8_t")
            return static_cast<uint32_t>(static_cast<int8_t>(*value));
        if (value.has_value() && (expression.text == "u16" || expression.text == "uint16_t"))
            return *value & 0xFFFFU;
        if (value.has_value() && expression.text == "int16_t")
            return static_cast<uint32_t>(static_cast<int16_t>(*value));
        if (value.has_value() && (expression.text == "u32" || expression.text == "uint32_t" || expression.text == "int32_t"))
            return value;
        if (value.has_value() && width.has_value()) {
            if (*width == 1)
                return static_cast<std::uint32_t>(static_cast<std::int8_t>(*value));
            if (*width == 2)
                return static_cast<std::uint32_t>(static_cast<std::int16_t>(*value));
            return value;
        }
    }
    if (expression.kind == SourceExprKind::Unary && expression.args.size() == 1) {
        const std::optional<std::uint32_t> value = sourceDataConstantValue(expression.args[0], cSemantics);
        if (!value.has_value())
            return std::nullopt;
        if (expression.text == "-")
            return 0U - *value;
        if (expression.text == "+")
            return value;
        if (expression.text == "~")
            return ~*value;
        if (expression.text == "!")
            return *value == 0 ? 1U : 0U;
    }
    if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2) {
        const std::optional<std::uint32_t> left = sourceDataConstantValue(expression.args[0], cSemantics);
        const std::optional<std::uint32_t> right = sourceDataConstantValue(expression.args[1], cSemantics);
        if (!left.has_value() || !right.has_value())
            return std::nullopt;
        if (expression.text == "+")
            return *left + *right;
        if (expression.text == "-")
            return *left - *right;
        if (expression.text == "*")
            return *left * *right;
        if ((expression.text == "/" || expression.text == "%") && *right != 0) {
            if (cSemantics && !sourceConstantIsUnsigned(expression)) {
                const int64_t lhs = static_cast<int32_t>(*left), rhs = static_cast<int32_t>(*right);
                if (lhs == INT32_MIN && rhs == -1)
                    throw std::runtime_error("Signed constant division overflows");
                return static_cast<uint32_t>(expression.text == "/" ? lhs / rhs : lhs % rhs);
            }
            return expression.text == "/" ? *left / *right : *left % *right;
        }
        if (expression.text == "|")
            return *left | *right;
        if (expression.text == "&")
            return *left & *right;
        if (expression.text == "^")
            return *left ^ *right;
        if (expression.text == "<<" && *right < 32)
            return *left << *right;
        if (expression.text == ">>" && *right < 32) {
            if (cSemantics && !sourceConstantIsUnsigned(expression.args[0]))
                return static_cast<uint32_t>(static_cast<int32_t>(*left) >> *right);
            return *left >> *right;
        }
    }
    return std::nullopt;
}

static std::optional<SourceDataAddress> dataMemberAddress(const SourceDataAddress &base, std::string_view name) {
    if (!base.schema)
        return std::nullopt;
    for (const ScenarioDataField &field : base.schema->fields) {
        if (name == field.name)
            return SourceDataAddress{.offset = base.offset + field.offset,
                .schema = nullptr,
                .dataType = field.type,
                .elementCount = field.elementCount,
                .stride = scenarioDataTypeSize(field.type),
                .isArray = field.elementCount > 1};
    }
    return std::nullopt;
}

static SourceDataAddress sourcePointerView(SourceDataAddress address, const SourceExpr &expression) {
    if (expression.cConversion != "BitCast" || expression.cType.find('*') == std::string::npos)
        return address;
    std::string pointee = trimCopy(expression.cType.substr(0, expression.cType.rfind('*')));
    while (pointee.starts_with("const ") || pointee.starts_with("volatile "))
        pointee.erase(0, pointee.find(' ') + 1);
    if (pointee == "int")
        pointee = "int32_t";
    if (pointee == "unsigned int")
        pointee = "uint32_t";
    const ScenarioDataSchema *schema = findScenarioDataSchema(pointee);
    const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(pointee);
    const uint32_t stride = schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 0;
    if (!stride)
        throw std::runtime_error("C pointer cast has no established data layout: " + pointee);
    const uint64_t extent = static_cast<uint64_t>(address.elementCount) * address.stride;
    address.schema = schema;
    address.dataType = dataType;
    address.stride = stride;
    address.elementCount = static_cast<uint32_t>(extent / stride);
    address.isArray = true;
    return address;
}

std::optional<SourceDataAddress> resolveSourceDataAddress(const SourceUnit &unit, const SourceExpr &expression, bool typeOnly) {
    if (expression.kind == SourceExprKind::Cast && expression.args.size() == 1 &&
        (expression.cConversion == "BitCast" || expression.cConversion == "PointerToIntegral" || expression.cConversion == "IntegralToPointer")) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        return base ? std::optional<SourceDataAddress>(sourcePointerView(*base, expression)) : std::nullopt;
    }
    if (expression.kind == SourceExprKind::Unary && expression.text == "&" && expression.args.size() == 1) {
        const std::optional<SourceDataAddress> address = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        return address ? std::optional<SourceDataAddress>(sourcePointerView(*address, expression)) : std::nullopt;
    }
    if (expression.kind == SourceExprKind::Call && expression.text == "byte_address" && expression.args.size() == 1) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        if (base.has_value())
            return SourceDataAddress{.offset = base->offset,
                .schema = nullptr,
                .dataType = std::nullopt,
                .elementCount = std::numeric_limits<std::uint32_t>::max(),
                .stride = 1,
                .isArray = true};
        return std::nullopt;
    }
    if (expression.kind == SourceExprKind::Identifier) {
        const std::map<std::string, std::uint32_t>::const_iterator symbol = unit.dataSymbols.find(expression.text);
        if (symbol != unit.dataSymbols.end()) {
            SourceDataAddress result{.offset = symbol->second, .schema = nullptr, .dataType = std::nullopt, .elementCount = 1, .stride = 1, .isArray = true};
            const std::map<std::string, SourceDataBinding>::const_iterator binding = unit.dataBindings.find(expression.text);
            if (binding != unit.dataBindings.end()) {
                result.schema = findScenarioDataSchema(binding->second.type);
                result.dataType = scenarioDataTypeByName(binding->second.type);
                result.stride = result.schema ? result.schema->size : result.dataType ? scenarioDataTypeSize(*result.dataType) : 1;
                result.elementCount = binding->second.elementCount;
                result.isArray = binding->second.isArray;
            }
            return sourcePointerView(result, expression);
        }
        const std::size_t dot = expression.text.find('.');
        if (dot != std::string::npos) {
            SourceExpr base{.kind = SourceExprKind::Identifier, .text = expression.text.substr(0, dot), .args = {}};
            const std::optional<SourceDataAddress> address = resolveSourceDataAddress(unit, base, typeOnly);
            if (address.has_value())
                return dataMemberAddress(*address, std::string_view(expression.text).substr(dot + 1));
        }
        const std::map<std::string, SourceFunctionAst>::const_iterator function = unit.functions.find(expression.text);
        if (function != unit.functions.end())
            return SourceDataAddress{
                .offset = function->second.offset, .schema = nullptr, .dataType = std::nullopt, .elementCount = 1, .stride = 1, .isArray = true};
    }
    if (expression.kind == SourceExprKind::Cast && expression.args.size() == 1 && !expression.text.empty() && expression.text[0] == '.') {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        if (base.has_value())
            return dataMemberAddress(*base, std::string_view(expression.text).substr(1));
    }
    if (expression.kind == SourceExprKind::Index && expression.args.size() == 2) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        const std::optional<std::uint32_t> index = sourceDataConstantValue(expression.args[1], unit.ordinaryC);
        if (base.has_value() && (index.has_value() || typeOnly)) {
            if (index.has_value() && *index > base->elementCount)
                throw std::runtime_error("Scenario data index exceeds its declared array");
            SourceDataAddress address = *base;
            if (index.has_value()) {
                const std::uint64_t offset = static_cast<std::uint64_t>(address.offset) + static_cast<std::uint64_t>(*index) * address.stride;
                if (offset > std::numeric_limits<std::uint32_t>::max())
                    throw std::runtime_error("Scenario data address exceeds the SCN3 offset range");
                address.offset = static_cast<std::uint32_t>(offset);
            }
            address.elementCount = 1;
            address.isArray = false;
            return address;
        }
    }
    if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2 && (expression.text == "+" || expression.text == "-")) {
        const std::optional<SourceDataAddress> base = resolveSourceDataAddress(unit, expression.args[0], typeOnly);
        const std::optional<std::uint32_t> index = sourceDataConstantValue(expression.args[1], unit.ordinaryC);
        const bool explicitAddress = expression.args[0].kind == SourceExprKind::Unary && expression.args[0].text == "&";
        if (base.has_value() && (base->isArray || base->schema || explicitAddress) && (index.has_value() || typeOnly)) {
            SourceDataAddress address = *base;
            address.isArray = true;
            if (index.has_value()) {
                const bool integerAddress = unit.ordinaryC && !expression.args[0].cType.empty() && expression.args[0].cType.find('*') == std::string::npos;
                const uint32_t stride = integerAddress ? 1U : base->stride;
                const std::int64_t displacement = static_cast<std::int32_t>(*index) * static_cast<std::int64_t>(stride);
                const std::int64_t offset = base->offset + (expression.text == "+" ? displacement : -displacement);
                if (offset < 0 || offset > std::numeric_limits<std::uint32_t>::max())
                    throw std::runtime_error("Scenario data address exceeds the SCN3 offset range");
                address.offset = static_cast<std::uint32_t>(offset);
            }
            return sourcePointerView(address, expression);
        }
    }
    return std::nullopt;
}

void applySourceDataRelocations(SourceUnit &unit) {
    for (const SourceDataRelocation &relocation : unit.dataRelocations) {
        const std::optional<SourceDataAddress> target = resolveSourceDataAddress(unit, relocation.target);
        if (!target.has_value())
            throw std::runtime_error("Unknown scenario data reference at source line " + std::to_string(relocation.target.line));
        std::vector<std::uint8_t> *bytes = nullptr;
        if (relocation.blockKind == RangeKind::DataSegment)
            bytes = &unit.dataSegment;
        else if (relocation.blockKind == RangeKind::TrailingDataOrUnknown)
            bytes = &unit.trailingData;
        else if (relocation.blockIndex < unit.rawBlocks.size())
            bytes = &unit.rawBlocks[relocation.blockIndex].bytes;
        if (!bytes || relocation.byteOffset > bytes->size() || bytes->size() - relocation.byteOffset < 4)
            throw std::runtime_error("Scenario data relocation lies outside its owning declaration block");
        for (std::uint32_t byte = 0; byte < 4; ++byte)
            (*bytes)[relocation.byteOffset + byte] = static_cast<std::uint8_t>(target->offset >> (byte * 8U));
    }
}

bool sourceExprStructurallyEquals(const SourceExpr &lhs, const SourceExpr &rhs) {
    if (lhs.kind != rhs.kind || lhs.text != rhs.text || lhs.args.size() != rhs.args.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs.args.size(); ++i) {
        if (!sourceExprStructurallyEquals(lhs.args[i], rhs.args[i])) {
            return false;
        }
    }
    return true;
}

} // namespace shenmue::scenario
