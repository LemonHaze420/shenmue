#include "scn3/native/prototypes.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/fields.hpp"
#include "s1/task/scen/internal.hpp"
#include "s1/lighting.hpp"

#include <array>
#include <cctype>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#define SCENARIO_LEGACY_SLOT(index, handler) static_assert(std::is_convertible_v<decltype(&shenmue::s1::handler), shenmue::s1::LegacyScenarioFunction>);
#define SCENARIO_GENERAL_SLOT(index, handler) static_assert(std::is_convertible_v<decltype(&shenmue::s1::handler), shenmue::s1::GeneralScenarioFunction>);
#define SCENARIO_SHENMUE_SLOT(index, handler) static_assert(std::is_convertible_v<decltype(&shenmue::s1::handler), shenmue::s1::ShenmueScenarioFunction>);
#define SCENARIO_SCNF_SLOT(index, handler) static_assert(std::is_convertible_v<decltype(&shenmue::s1::handler), shenmue::s1::SCNFScenarioFunction>);
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_LEGACY_SLOT
#undef SCENARIO_GENERAL_SLOT
#undef SCENARIO_SHENMUE_SLOT
#undef SCENARIO_SCNF_SLOT
#undef SCENARIO_DOOR_SLOT

namespace shenmue::scenario {
namespace {

constexpr NativeParameterRead taskEntryReads[] = {{.byteOffset = offsetof(s1::scen_detail::CreateScenarioTaskParameters, argumentCount), .guards = {}},
    {.byteOffset = offsetof(s1::scen_detail::CreateScenarioTaskParameters, functionOffset), .guards = {}}};
constexpr NativeArgumentLayout taskEntryLayout{.command = "Script_scenCreateTask",
    .reads = taskEntryReads,
    .callbackOffset = offsetof(s1::scen_detail::CreateScenarioTaskParameters, functionOffset),
    .countOffset = offsetof(s1::scen_detail::CreateScenarioTaskParameters, argumentCount),
    .tailOffset = offsetof(s1::scen_detail::CreateScenarioTaskParameters, arguments)};
constexpr std::string_view noScriptArguments[] = {
    "DSWK_GetSettingsFlags",
    "DoorPreserveAccumulator",
    "SCNF_CompleteImmediately",
    "SCNF_GetCashOverlayResult",
    "SCNF_GetSelectedBinary",
    "SCNF_GetSelectedHelpToken",
    "SCNF_GetQteOverlayResultIfIdle",
    "SCNF_PrepareCassetteUI",
    "SCNF_ReturnMinusOne",
    "SCNF_ReturnZero",
    "SCNF_WaitCashOverlayIdle",
    "SCNF_WaitCassetteUI",
    "SCNF_WaitIconNotification",
    "SCNF_WaitIconSystemMenu",
    "SCNF_WaitSleepSaveTraining",
    "SCNT_Enqueue",
    "SCNT_WaitSCNFSubtitleText",
    "SCN_GetIsFighting",
    "Script_ClearAccumulator",
    "Script_ClearInputFlags",
    "Script_ExitCharaSelector",
    "Script_GetCurrentControlPointCode",
    "Script_GetCurrentEntryPoint",
    "Script_GetCurrentFrameCount",
    "Script_GetCycleManagerDeltaTime",
    "Script_GetFreeTalkStatus",
    "Script_GetGlobalFlag",
    "Script_GetRemainingFrames",
    "Script_GetSceneID",
    "Script_GetScriptedCameraState",
    "Script_GetTaskData_0",
    "Script_InitSakuraEffectTasks_0",
    "Script_IsInEventCam",
    "Script_IsPlayerAllowed",
    "Script_ParseDigitsForInt",
    "Script_PreserveAccumulator",
    "Script_SetICval",
    "Script_cassetteUtils",
    "Script_cpctGetCurrControlMode",
    "Script_ctrlProcessReqScene",
    "Script_cuffUtils",
    "Script_effeInitTexture2D",
    "Script_faceGetFilename",
    "Script_fightingStyle",
    "Script_freeTalkGetAndResetState",
    "Script_freeTalkGetCurrentChara",
    "Script_seqGetCamInterpActive",
    "Script_shenGetTaskBase",
};

constexpr NativePrototype prototypeDeclarations[] = {
#define SCN3_PROTOTYPE(command, result, ...) {#command, #result, #__VA_ARGS__},
#define SCN3_MODE(command, guard, ...)
#include "scn3/native/prototypes.def"
#undef SCN3_MODE
#undef SCN3_PROTOTYPE
};

struct ModeDeclaration {
    std::string_view command;
    std::string_view guard;
    std::string_view parameters;
};

constexpr ModeDeclaration modeDeclarations[] = {
#define SCN3_PROTOTYPE(command, result, ...)
#define SCN3_MODE(command, guard, ...) {#command, #guard, #__VA_ARGS__},
#include "scn3/native/prototypes.def"
#undef SCN3_MODE
#undef SCN3_PROTOTYPE
};

struct PrototypeMode {
    std::vector<std::string> types;
    std::vector<std::string> names;
    std::vector<std::string> pointees;
    std::vector<NativeParameterRead> reads;
    std::vector<NativePointerParameter> pointers;
    std::vector<std::uint32_t> floats;
    std::vector<std::uint32_t> words;
    std::vector<NativeModeCondition> guards;
    std::vector<NativeArgumentType> arguments;
    std::int32_t variadic = -1;
    NativeArgumentLayout layout{};
};

struct Prototype {
    const NativePrototype *declaration = nullptr;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> parameterNames;
    std::vector<PrototypeMode> modes;
};

std::string_view trimmed(std::string_view text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
        text.remove_prefix(1);
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
        text.remove_suffix(1);
    return text;
}

bool parameterTypes(std::string_view parameters, std::vector<std::string> &types, std::vector<std::string> &names) {
    bool variadic = false;
    for (std::size_t begin = 0; begin <= parameters.size();) {
        std::size_t end = parameters.find(',', begin);
        if (end == std::string_view::npos)
            end = parameters.size();
        const std::string_view parameter = trimmed(parameters.substr(begin, end - begin));
        begin = end + 1;
        if (parameter.empty() || parameter == "void")
            continue;
        if (parameter == "...") {
            variadic = true;
            continue;
        }
        std::size_t name = parameter.size();
        while (name && (std::isalnum(static_cast<unsigned char>(parameter[name - 1])) || parameter[name - 1] == '_'))
            --name;
        const std::string type(trimmed(parameter.substr(0, name)));
        if (type.find('(') != std::string::npos)
            throw std::runtime_error("Function-pointer parameters are not supported");
        types.push_back(type);
        names.emplace_back(parameter.substr(name));
    }
    return variadic;
}

std::vector<NativeModeCondition> modeGuards(std::string_view guard) {
    std::vector<NativeModeCondition> guards;
    for (std::size_t begin = 0; begin < guard.size();) {
        std::size_t end = guard.find("&&", begin);
        if (end == std::string_view::npos)
            end = guard.size();
        const std::string_view term = trimmed(guard.substr(begin, end - begin));
        begin = end + 2;
        const std::size_t equal = term.find("==");
        const std::size_t differ = term.find("!=");
        const std::size_t split = equal != std::string_view::npos ? equal : differ;
        if (split == std::string_view::npos)
            throw std::runtime_error("Invalid prototype mode comparison");
        const std::string word(trimmed(term.substr(0, split)));
        const std::string value(trimmed(term.substr(split + 2)));
        guards.push_back({.byteOffset = static_cast<std::uint32_t>(std::stoul(word)) * 4U,
            .value = static_cast<std::uint32_t>(std::stoll(value, nullptr, 0)),
            .negate = split == differ});
    }
    return guards;
}

const std::vector<Prototype> &prototypes() {
    static const std::vector<Prototype> definitions = [] {
        std::vector<Prototype> result(std::size(prototypeDeclarations));
        for (std::size_t index = 0; index < result.size(); ++index) {
            Prototype &prototype = result[index];
            prototype.declaration = &prototypeDeclarations[index];
            const bool variadic = parameterTypes(prototype.declaration->parameters, prototype.parameterTypes, prototype.parameterNames);
            for (const ModeDeclaration &mode : modeDeclarations) {
                if (mode.command != prototype.declaration->command)
                    continue;
                if (!variadic)
                    throw std::runtime_error("Mode-specific arguments require a variadic prototype: " + std::string(mode.command));
                PrototypeMode variant{.types = prototype.parameterTypes, .names = prototype.parameterNames, .guards = modeGuards(mode.guard)};
                const bool trailing = parameterTypes(mode.parameters, variant.types, variant.names);
                (void)trailing;
                variant.variadic = static_cast<std::int32_t>(variant.types.size() * 4U);
                prototype.modes.push_back(std::move(variant));
            }
            PrototypeMode base{.types = prototype.parameterTypes, .names = prototype.parameterNames};
            base.variadic = variadic ? static_cast<std::int32_t>(base.types.size() * 4U) : -1;
            prototype.modes.push_back(std::move(base));
        }
        for (ModeDeclaration mode : modeDeclarations) {
            bool known = false;
            for (const Prototype &prototype : result)
                known |= prototype.declaration->command == mode.command;
            if (!known)
                throw std::runtime_error("Unknown prototype for mode: " + std::string(mode.command));
        }
        for (Prototype &prototype : result)
            for (PrototypeMode &variant : prototype.modes) {

                for (std::uint32_t position = 0; position < variant.types.size(); ++position) {
                    const std::string &type = variant.types[position];
                    variant.reads.push_back({.byteOffset = position * 4U, .guards = {}});
                    if (type == "float")
                        variant.floats.push_back(position * 4U);
                    if (type == "scn3_word")
                        variant.words.push_back(position * 4U);
                    if (const std::size_t star = type.find('*'); star != std::string::npos) {
                        std::string pointee(trimmed(std::string_view(type).substr(0, star)));
                        if (pointee.starts_with("const "))
                            pointee = std::string(trimmed(std::string_view(pointee).substr(6)));
                        variant.pointees.push_back(pointee);
                    }
                }
                std::size_t pointee = 0;
                for (std::uint32_t position = 0; position < variant.types.size(); ++position) {
                    const std::string &type = variant.types[position];
                    if (type.find('*') == std::string::npos)
                        continue;
                    const std::string &name = variant.pointees[pointee++];
                    variant.pointers.push_back({.byteOffset = position * 4U, .type = name.c_str(), .readOnly = type.starts_with("const ")});
                    if (name != "void")
                        variant.arguments.push_back({.handler = prototype.declaration->command.data(),
                            .byteOffset = position * 4U,
                            .type = name.c_str(),
                            .pointer = true,
                            .guards = variant.guards});
                }
                for (std::uint32_t offset : variant.floats)
                    variant.arguments.push_back(
                        {.handler = prototype.declaration->command.data(), .byteOffset = offset, .type = "float", .pointer = false, .guards = variant.guards});
                for (std::uint32_t offset : variant.words)
                    variant.arguments.push_back({.handler = prototype.declaration->command.data(),
                        .byteOffset = offset,
                        .type = "scn3_word",
                        .pointer = false,
                        .guards = variant.guards});

                for (std::uint32_t position = 0; position < variant.types.size(); ++position)
                    if (variant.types[position] == "MotionId")
                        variant.arguments.push_back({.handler = prototype.declaration->command.data(),
                            .byteOffset = position * 4U,
                            .type = "MotionId",
                            .pointer = false,
                            .guards = variant.guards});
                for (std::uint32_t position = 0; position < variant.types.size(); ++position)
                    for (const abi::ValueTableSpec &table : abi::kValueTables)
                        if (table.displayName && variant.types[position] == table.displayName)
                            variant.arguments.push_back({.handler = prototype.declaration->command.data(),
                                .byteOffset = position * 4U,
                                .type = variant.types[position].c_str(),
                                .pointer = false,
                                .guards = variant.guards});
                variant.layout = NativeArgumentLayout{.command = prototype.declaration->command.data(),
                    .reads = variant.reads,
                    .callbackOffset = -1,
                    .countOffset = -1,
                    .tailOffset = -1,
                    .pointers = variant.pointers,
                    .parameterBytes = static_cast<std::uint32_t>(variant.types.size() * 4U),
                    .floatArguments = variant.floats,
                    .guards = variant.guards,
                    .variadicOffset = variant.variadic};
            }
        return result;
    }();
    return definitions;
}

const Prototype *findPrototype(std::string_view handler) {
    for (const Prototype &prototype : prototypes())
        if (prototype.declaration->command == handler)
            return &prototype;
    return nullptr;
}

const std::vector<NativeArgumentLayout> &argumentLayouts() {
    static const std::vector<NativeArgumentLayout> layouts = [] {
        std::vector<NativeArgumentLayout> result{taskEntryLayout};
        for (const Prototype &prototype : prototypes())
            for (const PrototypeMode &variant : prototype.modes)
                result.push_back(variant.layout);
        return result;
    }();
    return layouts;
}

const std::vector<NativeArgumentType> &argumentTypes() {
    static const std::vector<NativeArgumentType> arguments = [] {
        std::vector<NativeArgumentType> result;
        for (const Prototype &prototype : prototypes())
            for (const PrototypeMode &variant : prototype.modes)
                result.insert(result.end(), variant.arguments.begin(), variant.arguments.end());
        return result;
    }();
    return arguments;
}

} // namespace

const NativePrototype *nativePrototype(std::string_view handler) {
    const Prototype *prototype = findPrototype(handler);
    return prototype ? prototype->declaration : nullptr;
}

std::span<const std::string> nativeParameterTypes(std::string_view handler) {
    const Prototype *prototype = findPrototype(handler);
    return prototype ? std::span<const std::string>(prototype->parameterTypes) : std::span<const std::string>{};
}

std::span<const NativeArgumentLayout> nativeArgumentLayouts() {
    return argumentLayouts();
}

const NativeArgumentLayout *nativeArgumentLayout(std::string_view handler, std::span<const std::optional<std::uint32_t>> arguments) {
    for (const NativeArgumentLayout &layout : argumentLayouts()) {
        if (handler != layout.command)
            continue;
        bool selected = true;
        for (const NativeModeCondition &guard : layout.guards) {
            const std::size_t index = guard.byteOffset / sizeof(std::uint32_t);
            if (guard.byteOffset % sizeof(std::uint32_t) || index >= arguments.size() || !arguments[index] ||
                ((*arguments[index] != guard.value) != guard.negate)) {
                selected = false;
                break;
            }
        }
        if (selected)
            return &layout;
    }
    return nullptr;
}

bool nativeCallHasModes(std::string_view handler) {
    for (const NativeArgumentLayout &layout : argumentLayouts())
        if (handler == layout.command && !layout.guards.empty())
            return true;
    return false;
}

bool nativeCallIgnoresParameters(std::string_view handler) {
    if (findPrototype(handler))
        return false;
    for (std::string_view name : noScriptArguments)
        if (name == handler)
            return true;
    return false;
}

NativeParameter nativeParameter(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments) {
    const Prototype *prototype = findPrototype(handler);
    if (!prototype)
        return {};
    for (const PrototypeMode &variant : prototype->modes) {
        bool selected = true;
        for (const NativeModeCondition &guard : variant.guards) {
            const std::size_t guardIndex = guard.byteOffset / sizeof(std::uint32_t);
            selected &= guard.byteOffset % sizeof(std::uint32_t) == 0 && guardIndex < arguments.size() && arguments[guardIndex] &&
                ((*arguments[guardIndex] != guard.value) != guard.negate);
        }
        if (selected)
            return index < variant.types.size() ? NativeParameter{.type = variant.types[index], .name = variant.names[index]} : NativeParameter{};
    }
    return {};
}

namespace {

struct NativePointerAccess {
    std::string_view handler;
    std::size_t index;
    NativeAccess access;
    std::size_t selector = SIZE_MAX;
    std::array<std::uint32_t, 3> values{};
    std::size_t count = 0;
};

constexpr NativePointerAccess pointerAccesses[] = {
    {.handler = "Script_charaPosRotUtils", .index = 2, .access = NativeAccess::Read, .selector = 4, .values = {0, 1, 3}, .count = 3},
    {.handler = "Script_charaPosRotUtils", .index = 3, .access = NativeAccess::Read, .selector = 4, .values = {0, 1, 3}, .count = 3},
    {.handler = "Script_charaPosRotUtils", .index = 2, .access = NativeAccess::Write, .selector = 4, .values = {2}, .count = 1},
    {.handler = "Script_charaPosRotUtils", .index = 3, .access = NativeAccess::Write, .selector = 4, .values = {2}, .count = 1},
    {.handler = "Script_EVT_GetPosition", .index = 2, .access = NativeAccess::Write, .selector = 1, .values = {0xFFFFFFFFU}, .count = 1},
    {.handler = "Script_ReturnTimeDate", .index = 0, .access = NativeAccess::Write},
    {.handler = "Script_ReadMOTNPositionAtFrame", .index = 3, .access = NativeAccess::Write},
    {.handler = "Script_GetAuthorState_And_SetCamPos", .index = 0, .access = NativeAccess::Write},
    {.handler = "Script_GetAuthorState_And_SetCamPos", .index = 1, .access = NativeAccess::Write},
    {.handler = "Script_charaSetupXForm", .index = 2, .access = NativeAccess::Write},
    {.handler = "Script_mtwkGetSetRotation", .index = 1, .access = NativeAccess::Write},
};

} // namespace

NativeAccess nativePointerAccess(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments) {
    for (const NativePointerAccess &entry : pointerAccesses) {
        if (entry.handler != handler || entry.index != index)
            continue;
        if (entry.selector == SIZE_MAX)
            return entry.access;
        if (entry.selector >= arguments.size() || !arguments[entry.selector])
            continue;
        for (std::size_t value = 0; value < entry.count; ++value)
            if (*arguments[entry.selector] == entry.values[value])
                return entry.access;
    }
    return NativeAccess::ReadWrite;
}

std::uint32_t nativePointerReads(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments) {
    const bool rotation = handler == "Script_EVT_SetRotation";
    if ((!rotation && handler != "Script_EVT_SetPosition") || index != 2 || arguments.size() < 2 || !arguments[1])
        return ~0U;
    const std::uint32_t flags = *arguments[1];
    return ((flags & 0x20000000U) ? 1U : 0U) | ((flags & 0x10000000U) || (rotation && (flags & 0x4200000U)) ? 2U : 0U) | ((flags & 0x8000000U) ? 4U : 0U);
}

const NativeArgumentType *nativeArgumentForHandler(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments) {
    const NativeArgumentType *selected = nullptr;
    const NativeArgumentType *possible = nullptr;
    for (const NativeArgumentType &candidate : argumentTypes()) {
        if (handler != candidate.handler || candidate.byteOffset != index * sizeof(std::uint32_t))
            continue;
        bool matches = true;
        bool resolved = true;
        for (const NativeModeCondition &guard : candidate.guards) {
            if (guard.byteOffset % sizeof(std::uint32_t) != 0) {
                matches = false;
                break;
            }
            const std::size_t guardIndex = guard.byteOffset / sizeof(std::uint32_t);
            if (guardIndex >= arguments.size() || !arguments[guardIndex]) {
                resolved = false;
                continue;
            }
            if ((*arguments[guardIndex] != guard.value) != guard.negate) {
                matches = false;
                break;
            }
        }
        if (!matches)
            continue;
        if (possible && (possible->pointer != candidate.pointer || std::string_view(possible->type) != candidate.type))
            return nullptr;
        possible = &candidate;
        if (resolved)
            selected = &candidate;
    }
    return selected;
}

} // namespace shenmue::scenario
