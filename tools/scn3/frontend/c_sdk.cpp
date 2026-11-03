#include "scn3/common/util.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/prototypes.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"
#include "scn3/native/motion.hpp"

#include <chrono>
#include <random>
#include <thread>

namespace shenmue::scenario {
namespace {

const char *dataCType(ScenarioDataType type) {
    switch (type) {
    case ScenarioDataType::U8:
        return "uint8_t";
    case ScenarioDataType::I8:
        return "int8_t";
    case ScenarioDataType::U16:
        return "uint16_t";
    case ScenarioDataType::I16:
        return "int16_t";
    case ScenarioDataType::I32:
        return "int32_t";
    case ScenarioDataType::F32:
        return "float";
    default:
        return "uint32_t";
    }
}

void writeCRecord(std::ostream &out, const ScenarioDataSchema &schema) {
    out << "#pragma pack(push, " << schema.alignment << ")\n";
    out << "typedef struct " << schema.name << " {\n";
    for (const ScenarioDataField &field : schema.fields) {
        if (field.type == ScenarioDataType::ScriptOffset32)
            out << "  " << scenarioPointeeCType(field.pointeeType) << " *" << field.name;
        else
            out << "  " << dataCType(field.type) << " " << field.name;
        if (field.elementCount > 1)
            out << "[" << field.elementCount << "]";
        out << ";\n";
    }
    out << "} " << schema.name << ";\n#pragma pack(pop)\n";
    out << "_Static_assert(sizeof(" << schema.name << ") == " << schema.size << ", \"project record size\");\n";
    for (const ScenarioDataField &field : schema.fields)
        out << "_Static_assert(offsetof(" << schema.name << ", " << field.name << ") == " << field.offset << ", \"project member offset\");\n";
    out << "\n";
}

bool writeCNativeVariants(std::ostream &out, std::string_view handler, const std::string &name, const std::string &result) {
    std::map<std::vector<uint32_t>, std::vector<std::string>> signatures;
    std::set<std::vector<uint32_t>> conflicts;
    std::set<uint32_t> positions;
    for (const NativeArgumentLayout &candidate : nativeArgumentLayouts()) {
        if (handler != candidate.command || candidate.guards.empty() || candidate.floatArguments.empty())
            continue;
        uint32_t count = 0;
        for (const NativeParameterRead &read : candidate.reads)
            count = std::max(count, (read.byteOffset + read.byteSize + 3U) / 4U);
        std::vector<std::string> types(count, "uint32_t");
        for (const NativePointerParameter &pointer : candidate.pointers)
            types.at(pointer.byteOffset / 4U) = std::string(pointer.readOnly ? "const " : "") + pointer.type + " *";
        std::vector<uint32_t> profile;
        for (uint32_t offset : candidate.floatArguments) {
            types.at(offset / 4U) = "float";
            profile.push_back(offset / 4U);
            positions.insert(offset / 4U);
        }
        if (signatures.count(profile) && signatures.at(profile) != types)
            conflicts.insert(profile);
        else
            signatures[profile] = std::move(types);
    }
    for (const std::vector<uint32_t> &conflict : conflicts)
        signatures.erase(conflict);
    if (signatures.empty())
        return false;
    const std::string fallback = "__scn_v0_" + name;
    out << result << fallback << "();\n";
    std::map<std::vector<uint32_t>, std::string> names;
    for (const std::pair<const std::vector<uint32_t>, std::vector<std::string>> &signature : signatures) {
        const std::string target = "__scn_v" + std::to_string(names.size() + 1) + "_" + name;
        names[signature.first] = target;
        out << result << target << "(";
        for (size_t index = 0; index < signature.second.size(); ++index) {
            if (index)
                out << ", ";
            out << signature.second[index] << " arg" << index;
        }
        out << ");\n";
    }
    const std::vector<uint32_t> indices(positions.begin(), positions.end());
    const std::function<std::string(size_t, std::vector<uint32_t>)> select = [&](size_t index, std::vector<uint32_t> profile) -> std::string {
        if (index == indices.size())
            return names.count(profile) ? names.at(profile) : fallback;
        const std::string integer = select(index + 1, profile);
        profile.push_back(indices[index]);
        const std::string floating = select(index + 1, profile);
        if (integer == floating)
            return integer;
        std::string argument = "__scn_arg" + std::to_string(indices[index]) + "(__VA_ARGS__";
        for (uint32_t padding = 0; padding <= indices[index]; ++padding)
            argument += ", 0";
        return "_Generic((" + argument + ")), float: " + floating + ", default: " + integer + ")";
    };
    out << "#define " << name << "(...) " << select(0, {}) << "(__VA_ARGS__)\n";
    return true;
}

void writeCWordMacro(std::ostream &out, const std::string &name, std::string_view parameters) {
    std::vector<std::string> types;
    std::string current;
    for (const char character : parameters) {
        if (character == ',') {
            types.push_back(trimCopy(current));
            current.clear();
        } else
            current += character;
    }
    if (!trimCopy(current).empty())
        types.push_back(trimCopy(current));
    bool words = false;
    for (const std::string &type : types)
        words |= type.starts_with("scn3_word ");
    if (!words)
        return;
    const bool variadic = !types.empty() && types.back() == "...";
    if (variadic)
        types.pop_back();
    out << "#define " << name << "(";
    for (size_t index = 0; index < types.size(); ++index)
        out << (index ? ", " : "") << "a" << index;
    out << (variadic ? ", ...) " : ") ") << name << "(";
    for (size_t index = 0; index < types.size(); ++index)
        out << (index ? ", " : "") << (types[index].starts_with("scn3_word ") ? "SCN3_WORD(a" + std::to_string(index) + ")" : "a" + std::to_string(index));
    out << (variadic ? ", __VA_ARGS__)\n" : ")\n");
}

} // namespace

void writeScenarioCHeader(const std::string &path) {
    const std::filesystem::path parent = std::filesystem::path(path).parent_path();
    if (!parent.empty())
        std::filesystem::create_directories(parent);
    std::ofstream out(path);
    if (!out)
        throw std::runtime_error("Cannot write the compiled scenario C header");
    out << "#ifdef __clang__\n#if __has_warning(\"-Wdeprecated-non-prototype\")\n"
           "#pragma clang diagnostic ignored \"-Wdeprecated-non-prototype\"\n#endif\n"
           "#if __has_warning(\"-Wstrict-prototypes\")\n#pragma clang diagnostic ignored \"-Wstrict-prototypes\"\n#endif\n#endif\n";
    out << "#ifndef SHENMUE_SCENARIO_H\n#define SHENMUE_SCENARIO_H\n#include <stdint.h>\n#include <stddef.h>\n#include <stdbool.h>\n#include <stdarg.h>\n\n";
    out << "#define scn_stack_size(n) annotate(\"scn_stack_size\", 0x53434e01, (n))\n"
           "#define scn_stack_offset(n) annotate(\"scn_stack_offset\", 0x53434e02, (n))\n"
           "#define scn_task_offset(n) annotate(\"scn_task_offset\", 0x53434e03, (n))\n"
           "#define scn_string_literal annotate(\"scn_string_literal\", 0x53434e04, 0)\n"
           "#define scn_string_bindings(...) annotate(\"scn_string_bindings\", 0x53434e05, __VA_ARGS__)\n\n";
    out << "enum ScenarioFlag { SCF_Unknown0100 = " << hexUnsigned(SCF_Unknown0100) << ", SCF_NativeCodeMask = " << hexUnsigned(SCF_NativeCodeMask) << " };\n";
    out << "typedef uint32_t shen_name;\n"
           "#ifdef __clang__\n#pragma clang diagnostic ignored \"-Wmultichar\"\n#pragma clang diagnostic ignored \"-Wexcess-initializers\"\n#endif\n"
           "#define SHEN_NAME(c) ((shen_name)(c) >> 24 | ((shen_name)(c) >> 8 & 0xFF00U) | ((shen_name)(c) << 8 & 0xFF0000U) | (shen_name)(c) << 24)\n";
    out << "typedef union scn3_bits {\n  float f;\n  int32_t i;\n  uint32_t u;\n} scn3_bits;\n"
           "typedef uint32_t scn3_word;\n"
           "typedef int32_t ShenmueFlag;\n"
           "typedef int32_t MotionId;\n"
           "#define SCN3_WORD(x) _Generic((x), float: ((scn3_bits){.f = (x)}).u, default: (uint32_t)(x))\n";
    out << "void SCNT_ShowSubtitle(const char *text);\n";
#define SCENARIO_ENUM_BEGIN(name) out << "typedef enum " #name " {\n";
#define SCENARIO_ENUM_VALUE(table, name, value) out << "  " #name " = " << value << ",\n";
#define SCENARIO_ENUM_END(name) out << "} " #name ";\n";
#include "s1/task/scen/commands.def"
#undef SCENARIO_ENUM_BEGIN
#undef SCENARIO_ENUM_VALUE
#undef SCENARIO_ENUM_END

    for (const ScenarioDataSchema &schema : scenarioDataSchemas())
        out << "typedef struct " << schema.name << " " << schema.name << ";\n";
    out << "\n";
    for (const ScenarioDataSchema &schema : scenarioDataSchemas())
        writeCRecord(out, schema);
    std::set<std::string> constants;
#define SCENARIO_ENUM_BEGIN(name)
#define SCENARIO_ENUM_VALUE(table, name, value) constants.insert(#name);
#define SCENARIO_ENUM_END(name)
#include "s1/task/scen/commands.def"
#undef SCENARIO_ENUM_BEGIN
#undef SCENARIO_ENUM_VALUE
#undef SCENARIO_ENUM_END
    for (const abi::ValueNameSpec &value : abi::kValueNames)
        if (value.symbol && lookupAbiValueByName(value.symbol) == value.value && constants.insert(value.symbol).second)
            out << "#define " << value.symbol << " " << hexUnsigned(value.value) << "\n";
    for (uint32_t value : shenmue::s1::kValidScenCharacterNames) {
        std::string name = "CHARA_";
        bool valid = true;
        for (uint32_t byte = 0; byte < 4; ++byte) {
            const char character = static_cast<char>(value >> (byte * 8U));
            valid = valid && ((character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_');
            name += character;
        }
        if (valid && constants.insert(name).second)
            out << "#define " << name << " SHEN_NAME('" << name.substr(6) << "')\n";
    }
    out << "\n#define __scn_first(first, ...) first\n#define __scn_rest(first, ...) __VA_ARGS__\n\n";
    uint32_t lastFloatingArgument = 0;
    for (const NativeArgumentLayout &layout : nativeArgumentLayouts())
        for (uint32_t offset : layout.floatArguments)
            lastFloatingArgument = std::max(lastFloatingArgument, offset / 4U);
    for (uint32_t index = 0; index <= lastFloatingArgument; ++index) {
        out << "#define __scn_arg" << index << "(";
        for (uint32_t parameter = 0; parameter <= index; ++parameter)
            out << (parameter ? ", " : "") << "a" << parameter;
        out << ", ...) a" << index << "\n";
    }
    for (const abi::CommandSpec &command : abi::kCommands) {
        if (!command.handlerName)
            continue;
        if (const NativePrototype *declared = nativePrototype(command.handlerName)) {
            const std::string name = lookupAbiName(command.family, command.publicIndex, "command");

            if (writeCNativeVariants(out, command.handlerName, name, std::string(declared->result) + " "))
                continue;
            out << declared->result << " " << name << "(" << (declared->parameters.empty() ? "void" : declared->parameters) << ");\n";
            writeCWordMacro(out, name, declared->parameters);
            continue;
        }
        const bool ignored = nativeCallIgnoresParameters(command.handlerName);
        const NativeArgumentLayout *layout = ignored ? nullptr : nativeArgumentLayout(command.handlerName);
        const bool variants = nativeCallHasModes(command.handlerName);
        if (!layout && !ignored && !variants)
            continue;
        const std::string name = lookupAbiName(command.family, command.publicIndex, "command");
        const std::string result = command.resultKind == abi::ResultKind::FloatEax ? "float " :
            command.resultKind == abi::ResultKind::Void                            ? "void " :
                                                                                     "int32_t ";
        if (variants && writeCNativeVariants(out, command.handlerName, name, result))
            continue;
        if (layout && layout->callbackOffset >= 0) {
            if (layout->callbackOffset != 0 || layout->countOffset != 4 || layout->tailOffset != 8)
                continue;
            out << result << "__scn_" << name << "(uint32_t callback, int32_t count, ...);\n";
            out << "#define " << name << "(...) __scn_" << name << "((uint32_t)(uintptr_t)(__scn_first(__VA_ARGS__)), __scn_rest(__VA_ARGS__))\n";
        } else if (layout && (!layout->pointers.empty() || !layout->floatArguments.empty())) {
            uint32_t count = 0;
            bool uniform = true;
            std::set<uint32_t> offsets;
            for (const NativeParameterRead &read : layout->reads) {
                if (!read.guards.empty())
                    uniform = false;
                const uint32_t end = (read.byteOffset + read.byteSize + 3U) / 4U;
                for (uint32_t index = read.byteOffset / 4U; index < end; ++index)
                    offsets.insert(index);
                count = std::max(count, end);
            }
            if (layout->variadicOffset >= 0) {
                if (layout->variadicOffset % 4)
                    continue;
                count = std::max(count, static_cast<uint32_t>(layout->variadicOffset) / 4U);
            }
            if (!uniform || (offsets.size() != count && layout->parameterBytes < count * 4U))
                continue;
            out << result << name << "(";
            for (uint32_t index = 0; index < count; ++index) {
                if (index)
                    out << ", ";
                const NativePointerParameter *pointer = nullptr;
                for (const NativePointerParameter &candidate : layout->pointers)
                    if (candidate.byteOffset == index * 4U)
                        pointer = &candidate;
                bool floating = false;
                for (uint32_t offset : layout->floatArguments)
                    if (offset == index * 4U)
                        floating = true;
                if (pointer)
                    out << (pointer->readOnly ? "const " : "") << pointer->type << " *";
                else
                    out << (floating ? "float " : "uint32_t ");
                out << "arg" << index;
            }
            out << ", ...";
            out << ");\n";
        } else
            out << result << name << (ignored || variants ? "();\n" : "(uint32_t first, ...);\n");
    }
    out << "\n#endif\n";
    if (!out)
        throw std::runtime_error("Failed writing the compiled scenario C header");
}

} // namespace shenmue::scenario
