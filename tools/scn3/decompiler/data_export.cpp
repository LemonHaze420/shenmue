#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/bytecode/value.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/decompiler/data.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario {

static bool dataFits(const Scn3Module &module, std::uint32_t offset, std::uint32_t size) {
    const std::uint32_t end = moduleDataEnd(module);
    return offset >= module.parsedHeader.dataSegmentOffset && offset <= end && size <= end - offset;
}

static void addDataView(std::vector<ScenarioDataView> &views, std::uint32_t start, std::uint32_t end, const ScenarioDataSchema *schema) {
    for (const ScenarioDataView &view : views) {
        if (view.start == start && view.end == end && view.schema == schema)
            return;
        if (start < view.end && end > view.start)
            return;
    }
    views.push_back(ScenarioDataView{.start = start, .end = end, .schema = schema});
}

std::vector<ScenarioDataView> discoverScenarioDataViews(const Program &program, const Scn3Module &module, std::set<std::uint32_t> &references) {
    std::vector<ScenarioDataView> views;
    const ScenarioDataSchema *init = findScenarioDataSchema("DoorSystemInitParams");
    const ScenarioDataSchema *entry = findScenarioDataSchema("DSWKDoorEntry");
    for (const std::pair<const std::uint32_t, FunctionInfo> &functionEntry : program.functions) {
        const FunctionInfo &function = functionEntry.second;
        for (const VmCallSite &call : analyzeVmCalls(program, function)) {
            if (call.control != Control::CallDswk || (call.target != 0 && call.target != 2))
                continue;
            const uint32_t count = call.target == 0 ? static_cast<uint32_t>(init->fields.size()) : 5U;
            for (uint32_t argument = 0; argument < count && argument < call.arguments.size(); ++argument) {
                const ScenarioDataField &field = init->fields[argument];
                const VmValue value = call.arguments[argument];
                if (!field.pointeeType || value.kind != VmValueKind::DataAddress)
                    continue;
                const uint32_t target = value.bits;
                const ScenarioDataSchema *pointee = findScenarioDataSchema(field.pointeeType);
                if (!pointee || !dataFits(module, target, pointee->size))
                    continue;
                references.insert(target);
                uint32_t end = target + pointee->size;

                if (pointee != entry) {
                    const std::set<std::uint32_t>::const_iterator next = references.upper_bound(target);
                    const uint32_t limit = next == references.end() ? moduleDataEnd(module) : std::min(*next, moduleDataEnd(module));
                    std::set<uint32_t> relocated;
                    for (uint32_t offset = module.parsedHeader.relocationTableOffset; offset + 8U <= module.originalBlob.size(); offset += 8U) {
                        if (!readU32(module.originalBlob, offset) && !readU32(module.originalBlob, offset + 4U))
                            break;
                        relocated.insert(readU32(module.originalBlob, offset + 4U));
                    }
                    const auto record = [&](uint32_t start) {
                        for (const ScenarioDataField &member : pointee->fields)
                            for (uint32_t element = 0; element < member.elementCount; ++element) {
                                const uint32_t offset = start + member.offset + element * scenarioDataTypeSize(member.type);
                                const uint32_t word = member.type == ScenarioDataType::F32 || member.type == ScenarioDataType::ScriptOffset32 ?
                                    readU32(module.originalBlob, offset) :
                                    0U;
                                float number = 0;
                                std::memcpy(&number, &word, sizeof(number));
                                if (member.type == ScenarioDataType::F32 &&
                                    (!std::isfinite(number) || !floatLiteralTextRoundTrips(formatFloatLiteral(word), word)))
                                    return false;
                                if (member.type == ScenarioDataType::ScriptOffset32 && word && !relocated.count(offset))
                                    return false;
                            }
                        return true;
                    };
                    while (end + pointee->size <= limit && record(end))
                        end += pointee->size;
                }
                if (pointee == entry) {
                    uint32_t cursor = target;
                    while (dataFits(module, cursor, 4) && readU32(module.originalBlob, cursor) != UINT32_MAX && dataFits(module, cursor, entry->size))
                        cursor += entry->size;
                    if (dataFits(module, cursor, 4) && readU32(module.originalBlob, cursor) == UINT32_MAX) {
                        end = cursor;
                        references.insert(cursor);
                    }
                }
                if (end > target)
                    addDataView(views, target, end, pointee);
            }
        }
    }

    for (const ScenarioDataView &view : views) {
        for (std::uint32_t record = view.start; record < view.end; record += view.schema->size) {
            for (const ScenarioDataField &field : view.schema->fields) {
                if (field.type == ScenarioDataType::ScriptOffset32) {
                    const std::uint32_t target = readU32(module.originalBlob, record + field.offset);
                    if (target && dataFits(module, target, 1))
                        references.insert(target);
                }
            }
        }
    }
    return views;
}

static void renderDataValue(std::ostream &out, const std::vector<std::uint8_t> &blob, std::uint32_t offset, ScenarioDataType type,
    const std::map<std::uint32_t, std::string> &symbols) {
    const std::uint32_t size = scenarioDataTypeSize(type);
    std::uint32_t bits = 0;
    for (std::uint32_t byte = 0; byte < size; ++byte)
        bits |= static_cast<std::uint32_t>(blob[offset + byte]) << (byte * 8U);
    if (type == ScenarioDataType::ScriptOffset32) {
        const std::map<std::uint32_t, std::string>::const_iterator target = symbols.find(bits);
        if (bits && target != symbols.end()) {
            out << "&" << target->second;
            return;
        }
    }
    if (type == ScenarioDataType::F32) {
        float value = 0;
        std::memcpy(&value, &bits, sizeof(value));
        if (!std::isfinite(value)) {
            out << "f32_bits(" << hexUnsigned(bits, 8) << ")";
            return;
        }
        std::ostringstream text;
        text << std::setprecision(std::numeric_limits<float>::max_digits10) << value;
        std::string literal = text.str();
        if (literal.find_first_of(".eE") == std::string::npos)
            literal += ".0";
        out << literal << "f";
    } else if (type == ScenarioDataType::I32) {
        out << static_cast<std::int32_t>(bits);
    } else {
        out << hexUnsigned(bits, static_cast<int>(size * 2));
    }
}

void renderScenarioDataRecord(std::ostream &out, const std::vector<std::uint8_t> &blob, std::uint32_t offset, const ScenarioDataSchema &schema,
    const std::map<std::uint32_t, std::string> &symbols, const std::string &indent) {
    out << "{\n";
    for (const ScenarioDataField &field : schema.fields) {
        out << indent << "  ." << field.name << " = ";
        if (field.elementCount > 1)
            out << "{";
        for (std::uint32_t index = 0; index < field.elementCount; ++index) {
            if (index)
                out << ", ";
            renderDataValue(out, blob, offset + field.offset + index * scenarioDataTypeSize(field.type), field.type, symbols);
        }
        if (field.elementCount > 1)
            out << "}";
        out << ",\n";
    }
    out << indent << "}";
}

} // namespace shenmue::scenario
