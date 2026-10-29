#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario {

std::vector<uint8_t> SourceParser::parseInlineBytes() {
    std::vector<uint8_t> bytes;
    expectPunct("{");
    while (!matchPunct("}")) {
        const uint32_t value = expectNumber();
        if (value > 0xFFu) {
            fail("Byte literal out of range");
        }
        bytes.push_back(static_cast<uint8_t>(value));
        matchPunct(",");
    }
    return bytes;
}

std::optional<float> SourceParser::dataFloatValue(const SourceExpr &expression) {
    if (ordinaryC_) {
        const std::optional<uint32_t> integer = sourceDataConstantValue(expression, true);
        if (integer)
            return sourceConstantIsUnsigned(expression) ? static_cast<float>(*integer) : static_cast<float>(static_cast<int32_t>(*integer));
    }
    if (expression.kind == SourceExprKind::FloatLiteral) {
        const std::uint32_t bits = parseSourceFloatBits(expression.text);
        float value = 0;
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }
    if (expression.kind == SourceExprKind::Call && expression.text == "f32_bits" && expression.args.size() == 1) {
        const std::optional<std::uint32_t> bits = sourceDataConstantValue(expression.args[0]);
        if (!bits.has_value())
            return std::nullopt;
        float value = 0;
        std::memcpy(&value, &*bits, sizeof(value));
        return value;
    }
    if (expression.kind == SourceExprKind::Unary && expression.args.size() == 1) {
        const std::optional<float> value = dataFloatValue(expression.args[0]);
        if (value.has_value() && expression.text == "-")
            return -*value;
        if (value.has_value() && expression.text == "+")
            return value;
    }
    if (expression.kind == SourceExprKind::Binary && expression.args.size() == 2) {
        const std::optional<float> left = dataFloatValue(expression.args[0]);
        const std::optional<float> right = dataFloatValue(expression.args[1]);
        if (!left.has_value() || !right.has_value())
            return std::nullopt;
        if (expression.text == "+")
            return *left + *right;
        if (expression.text == "-")
            return *left - *right;
        if (expression.text == "*")
            return *left * *right;
        if (expression.text == "/")
            return *left / *right;
    }
    const std::optional<std::uint32_t> integer = sourceDataConstantValue(expression, ordinaryC_);
    if (integer.has_value())
        return (ordinaryC_ ? !sourceConstantIsUnsigned(expression) : expression.kind == SourceExprKind::Cast) ?
            static_cast<float>(static_cast<std::int32_t>(*integer)) :
            static_cast<float>(*integer);
    return std::nullopt;
}

void SourceParser::parseDataValue(std::vector<std::uint8_t> &bytes, std::uint32_t offset, ScenarioDataType type) {
    SourceExpr expression = parseExpression();
    const std::uint32_t width = scenarioDataTypeSize(type);
    std::uint32_t bits = 0;
    if (type == ScenarioDataType::F32) {
        const std::optional<float> value = dataFloatValue(expression);
        if (!value.has_value())
            fail("Expected a constant floating-point data initializer");
        std::memcpy(&bits, &*value, sizeof(bits));
    } else if (const std::optional<std::uint32_t> value = sourceDataConstantValue(expression, ordinaryC_)) {
        bits = *value;
        if ((width == 1 && bits > 0xFFU && bits < 0xFFFFFF80U) || (width == 2 && bits > 0xFFFFU && bits < 0xFFFF8000U))
            fail("Integer data initializer does not fit its declared width");
    } else {
        if (width != 4)
            fail("Symbolic SCN3 offsets require a 32-bit data field");
        dataRelocations_.push_back(
            SourceDataRelocation{.blockKind = activeBlockKind_, .blockIndex = activeBlockIndex_, .byteOffset = offset, .target = std::move(expression)});
    }
    for (std::uint32_t byte = 0; byte < width; ++byte)
        bytes[offset + byte] = static_cast<std::uint8_t>(bits >> (byte * 8U));
}

void SourceParser::parseDataField(std::vector<std::uint8_t> &bytes, std::uint32_t offset, const ScenarioDataField &field) {
    if (field.elementCount == 1) {
        parseDataValue(bytes, offset, field.type);
        return;
    }
    expectPunct("{");
    std::uint32_t index = 0;
    while (!matchPunct("}")) {
        if (index == field.elementCount)
            fail("Too many values for native data field " + std::string(field.name));
        if (field.type == ScenarioDataType::U8 && current_.kind == SourceTokenKind::String) {
            const std::string value = expectString();
            if (value.size() > field.elementCount - index)
                fail("Packed string exceeds native data field " + std::string(field.name));
            for (char byte : value)
                bytes[offset + index++] = static_cast<std::uint8_t>(byte);
        } else {
            parseDataValue(bytes, offset + index * scenarioDataTypeSize(field.type), field.type);
            ++index;
        }
        if (!matchPunct(","))
            expectPunct("}");
        else
            continue;
        break;
    }
}

void SourceParser::parseDataRecord(std::vector<std::uint8_t> &bytes, std::uint32_t offset, const ScenarioDataSchema &schema) {
    expectPunct("{");
    std::uint64_t assignedFields = 0;
    while (!matchPunct("}")) {
        expectPunct(".");
        const std::string name = expectIdentifier();
        std::size_t fieldIndex = 0;
        while (fieldIndex < schema.fields.size() && name != schema.fields[fieldIndex].name)
            ++fieldIndex;
        if (fieldIndex == schema.fields.size())
            fail("Unknown field " + name + " in " + schema.name);
        if ((assignedFields & (std::uint64_t{1} << fieldIndex)) != 0)
            fail("Duplicate field " + name + " in " + schema.name);
        assignedFields |= std::uint64_t{1} << fieldIndex;
        expectPunct("=");
        const ScenarioDataField &field = schema.fields[fieldIndex];
        parseDataField(bytes, offset + field.offset, field);
        if (!matchPunct(","))
            expectPunct("}");
        else
            continue;
        break;
    }
}

void SourceParser::appendTypedData(std::vector<std::uint8_t> &bytes, const std::string &type, const std::string &name) {
    const ScenarioDataSchema *schema = findScenarioDataSchema(type);
    const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(type);
    if (!schema && !dataType.has_value())
        fail("Unsupported data declaration type: " + type);
    const std::uint32_t elementSize = schema ? schema->size : scenarioDataTypeSize(*dataType);
    const bool array = matchPunct("[");
    std::optional<std::uint32_t> declaredCount;
    if (array) {
        if (current_.kind == SourceTokenKind::Number)
            declaredCount = expectNumber();
        expectPunct("]");
    }
    expectPunct("=");
    if (array)
        expectPunct("{");
    const std::uint32_t begin = static_cast<std::uint32_t>(bytes.size());
    std::uint32_t count = 0;
    while (!array || !matchPunct("}")) {
        if (declaredCount.has_value() && count == *declaredCount)
            fail("Too many values for data array " + name);
        if (bytes.size() > std::numeric_limits<std::uint32_t>::max() - elementSize)
            fail("Data declaration exceeds the SCN3 offset range");
        const std::uint32_t offset = static_cast<std::uint32_t>(bytes.size());
        bytes.resize(bytes.size() + elementSize, 0);
        if (schema)
            parseDataRecord(bytes, offset, *schema);
        else
            parseDataValue(bytes, offset, *dataType);
        ++count;
        if (!array)
            break;
        if (!matchPunct(","))
            expectPunct("}");
        else
            continue;
        break;
    }
    if (declaredCount.has_value() && count < *declaredCount) {
        const std::uint64_t finalSize = static_cast<std::uint64_t>(begin) + static_cast<std::uint64_t>(*declaredCount) * elementSize;
        if (finalSize > std::numeric_limits<std::uint32_t>::max())
            fail("Data declaration exceeds the SCN3 offset range");
        bytes.resize(static_cast<std::size_t>(finalSize), 0);
        count = *declaredCount;
    }
    dataBindings_[name] = SourceDataBinding{.type = type, .elementCount = count, .isArray = array};
}

void SourceParser::appendDataDeclarationBytes(std::vector<uint8_t> &bytes) {
    const std::string type = expectIdentifier();
    const std::string name = expectIdentifier();
    if (symbolOffsets_.count(name) != 0) {
        fail("Duplicate data symbol: " + name);
    }
    symbolOffsets_[name] = activeBlockStart_ + static_cast<uint32_t>(bytes.size());
    const std::uint32_t begin = static_cast<std::uint32_t>(bytes.size());
    if (type == "bytes") {
        expectPunct("[");
        expectPunct("]");
        expectPunct("=");
        const std::vector<uint8_t> literal = parseInlineBytes();
        bytes.insert(bytes.end(), literal.begin(), literal.end());
    } else if (type == "pad") {
        expectPunct("[");
        const uint32_t count = expectNumber();
        expectPunct("]");
        expectPunct("=");
        const uint32_t value = expectNumber();
        if (value > 0xFFu) {
            fail("pad byte out of range");
        }
        bytes.insert(bytes.end(), count, static_cast<uint8_t>(value));
    } else if (type == "cstr") {
        expectPunct("=");
        const std::string text = expectString();
        cstrOffsetsByText_[text].push_back(symbolOffsets_[name]);
        bytes.insert(bytes.end(), text.begin(), text.end());
        bytes.push_back(0);
    } else if (scenarioDataTypeByName(type).has_value() || findScenarioDataSchema(type)) {
        appendTypedData(bytes, type, name);
    } else if (type == "f32_bits") {
        expectPunct("=");
        const uint32_t value = expectNumber();
        bytes.push_back(static_cast<uint8_t>(value & 0xFFu));
        bytes.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
        bytes.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
        bytes.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
    } else {
        fail("Unsupported data declaration type: " + type);
    }
    if (dataBindings_.count(name) == 0)
        dataBindings_[name] = SourceDataBinding{.type = type, .elementCount = static_cast<std::uint32_t>(bytes.size()) - begin, .isArray = true};
    expectPunct(";");
}

void SourceParser::parseRawRangeBlock() {
    SourceRawBlock block{};
    block.kind = parseRangeKindName(expectIdentifier());
    block.declaredStart = expectNumber();
    block.declaredEnd = expectNumber();
    activeBlockStart_ = block.declaredStart;
    activeBlockKind_ = block.kind;
    activeBlockIndex_ = rawBlocks_.size();
    expectPunct("{");
    while (!matchPunct("}")) {
        appendDataDeclarationBytes(block.bytes);
    }
    rawBlocks_.push_back(std::move(block));
}

void SourceParser::parseDataLikeBlock(RangeKind kind) {
    SourceRawBlock block{};
    block.kind = kind;
    block.declaredStart = expectNumber();
    block.declaredEnd = expectNumber();
    activeBlockStart_ = block.declaredStart;
    activeBlockKind_ = kind;
    activeBlockIndex_ = 0;
    expectPunct("{");
    while (!matchPunct("}")) {
        appendDataDeclarationBytes(block.bytes);
    }
    if (kind == RangeKind::DataSegment) {
        dataBlock_ = std::move(block);
    } else {
        trailingBlock_ = std::move(block);
    }
}

} // namespace shenmue::scenario
