#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include "scn3/native/constants.hpp"
#include "s1/task/scen.hpp"

namespace shenmue::scenario::abi {

enum class Family : std::uint8_t {
    Legacy,
    GeneralPurpose,
    Shenmue,
    DSWK,
    SCNF,
};

enum class DispatchKind : std::uint8_t {
    LegacyCallback,
    GeneralPurposeTwoInt,
    VmStackNative,
    ScnfPaired,
};

enum class ResultKind : std::uint8_t {
    Unknown,
    Void,
    Integer,
    BoolEax,
    FloatEax,
    FixedInt,
};

enum class ArityKind : std::uint8_t {
    Unknown,
    Fixed,
    CleanupDriven,
    OverlayDriven,
};

enum class ArgKind : std::uint8_t {
    Opaque,
    Integer,
    EnumValue,
    FlagsValue,
    InlineString,
    CharaId,
    MotnAnimId,
    LocalFunctionTarget,
    ScriptDataSymbol,
    Vector2Ptr,
    Vector3Ptr,
    Vector4Ptr,
};

enum class AliasKind : std::uint8_t {
    ExternalName,
};

enum class ValueTableFlags : std::uint8_t {
    None = 0,
    Bitmask = 1,
};

enum class GuardKind : std::uint8_t {
    ArgConstEquals,
};

enum class RuleDomain : std::uint8_t {
    Both,
    CompileOnly,
    DecompileOnly,
};

enum class ArgSelector : std::uint8_t {
    WholeCall,
    Exact,
    Range,
    FromIndex,
    Any,
};

enum class RuleKind : std::uint8_t {
    ArgIsFloat,
    ArgInlineString,
    ArgConsumesString,
    ArgIsCharaId,
    ArgIsMotnAnimId,
    ArgInlineDataSymbol,
    ArgPreferDecimal,
    CallPrintfFormatString,
    CallReturnsFloat,
    CallReturnsFixedInt,
    DirectEaxBranch,
    LogicalNotNeedsZeroCompare,
    CompileLoadPointerByte,
    CompileLoadPointerWord,
    CompileTaskVectorComponent,
    CompileTaskVectorArithmetic,
    CompileIntegerIndexExpression,
    CompileRootFloatArithmetic,
    CompileFixed16Mask,
};

enum class CommandFlags : std::uint32_t {
    None = 0,
    NullTarget = 1u << 0,
    Placeholder = 1u << 2,
    DuplicateTarget = 1u << 3,
    PairPrimary = 1u << 4,
    PairSecondary = 1u << 5,
};

enum class ArgFlags : std::uint32_t {
    None = 0,
    SignExtendNarrowLiteral = 1u << 0,
};

enum class VariantFlags : std::uint32_t {
    None = 0,
};

constexpr std::uint32_t operator|(CommandFlags lhs, CommandFlags rhs) {
    return static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs);
}

constexpr std::uint32_t operator|(std::uint32_t lhs, CommandFlags rhs) {
    return lhs | static_cast<std::uint32_t>(rhs);
}

enum class ValueTableId : std::uint16_t {
    None = 0,
    AicaCommand,
    AtckUtilMode,
    AuthorState,
    AuthorUtilsMode,
    BatteryLifeCmd,
    BatteryLifeMode,
    BattleUtilParam,
    CDTTMode,
    CamCtrlCmds,
    CameraCollisionStairCommand,
    CameraSettingsMode,
    CameraUtilMode,
    CharaSelectState,
    ConversationDetailsMode,
    CycleManagerCommand,
    DoorCommand,
    EEngineInfoMode,
    ESfxOverlayTimeoutTuningMode,
    EVAT_SetPosFlags,
    EntityTransformFlags,
    EvPakAfsAfsReadSubmode,
    EvPakAfsMode,
    EvPakAfsPackFileSubmode,
    EvPakAfsPackTaskSubmode,
    EvPakAfsProcessColsSubmode,
    ForkliftUtilCommand,
    ForkliftUtilTargetMode,
    HatoUtilMode,
    InstructionFlags,
    LODUtilsMode,
    LodNpcDisplayRadiusCmds,
    MOTMBlendFrameCount,
    MatrixTransformFlags,
    MemoryLoadingIconCommand,
    PlayerControlMode,
    ReadWriteTaskDataArrayMode,
    STEPUtilsModes,
    ScenarioRuntimeCommand,
    ScriptCameraSettingsMode,
    ScriptCycleCommand,
    ScriptMathMode,
    ScriptMathOp,
    ScriptPlayerControlMode,
    ScriptSetShadowFlagsMode,
    ScriptWeatherTaskMode,
    SetShadowFlagsMode,
    SetStatsCmds,
    ShenmueArcadeHighScoreSlot,
    ShenmueCpctFlag80Command,
    ShenmueCpctScriptMode,
    ShenmueFlag,
    ShenmueGameUtilsCommand,
    ShenmueOwnedItemId,
    ShenmuePlayerStatOp,
    ShenmueUtilsState,
    SpriteUtilCommand,
    SrchScriptCommand,
    StepUtilsMode,
    SysModType,
    TMLTState,
    TelmCmds,
    VFActorFlags,
    VFFightStyle,
    VFFormationMode,
    VehicleCommand,
    VirtualFighterCommand,
    W3D_Opcode,
    WeatherDataUtilMode,
    WeatherScriptCommand,
    WeatherSettingsMode,
    WeatherTaskMode,
    WeatherWindCmds,
    eActionSelectorMode,
    eLNWKScriptMode,
    eMoveSkillAndUiCommand,
    eScenarioRuntimeCommand,
    vfStageSetupScriptCommand,
};

struct FamilySpec {
    Family family;
    std::uint8_t opcodeFamily;
    DispatchKind dispatch;
    ResultKind defaultResult;
    ArityKind defaultArity;
    std::uint8_t fixedArgCount;
    std::uint16_t commandCount;
};

struct ValueTableSpec {
    ValueTableId id;
    const char *displayName;
    ValueTableFlags flags;
};

struct CommandSpec {
    Family family;
    std::uint16_t publicIndex;
    std::uint16_t dispatchIndex;
    std::uint8_t dispatchSubslot;
    const char *handlerName;
    const char *nativeTargetSymbol;
    ResultKind resultKind;
    ArityKind arityKind;
    std::uint8_t fixedArgCount;
    std::uint32_t flags;
};

struct AliasSpec {
    Family family;
    std::uint16_t publicIndex;
    AliasKind kind;
    const char *alias;
};

struct VariantSpec {
    Family family;
    std::uint16_t publicIndex;
    std::uint16_t variantIndex;
    const char *name;
    ResultKind resultKind;
    ArityKind arityKind;
    std::uint8_t fixedArgCount;
    std::uint32_t flags;
};

struct GuardSpec {
    Family family;
    std::uint16_t publicIndex;
    std::uint16_t variantIndex;
    GuardKind kind;
    std::uint8_t argIndex;
    std::uint32_t value;
};

struct ArgSpec {
    Family family;
    std::uint16_t publicIndex;
    std::uint16_t variantIndex;
    std::uint8_t argIndex;
    const char *name;
    ArgKind kind;
    std::uint8_t preferredLiteralWidth;
    ValueTableId valueTable;
    std::uint32_t flags;
};

struct RuleSpec {
    Family family;
    std::uint16_t publicIndex;
    std::uint16_t variantIndex;
    RuleKind kind;
    RuleDomain domain;
    ArgSelector selector;
    std::uint8_t argIndex;
    std::uint8_t argIndexEnd;
};

struct ValueNameSpec {
    ValueTableId table;
    std::uint32_t value;
    const char *symbol;
};
template <typename T> struct Slice {
    const T *data = nullptr;
    std::size_t size = 0;

    constexpr const T *begin() const {
        return data;
    }
    constexpr const T *end() const {
        return data + size;
    }
    constexpr bool empty() const {
        return size == 0;
    }
    constexpr const T &operator[](std::size_t index) const {
        return data[index];
    }
};

template <typename T, std::size_t N> constexpr std::size_t countof(const T (&)[N]) {
    return N;
}

template <typename T, std::size_t N> constexpr std::size_t countof(const std::array<T, N> &) {
    return N;
}

#define SCN3_FAMILY(FAMILY, OPCODE, DISPATCH, DEFAULT_RESULT, DEFAULT_ARITY, FIXED_ARG_COUNT, COMMAND_COUNT)                                                   \
    FamilySpec{Family::FAMILY, OPCODE, DISPATCH, DEFAULT_RESULT, DEFAULT_ARITY, FIXED_ARG_COUNT, COMMAND_COUNT},
inline constexpr FamilySpec kFamilies[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_FAMILY

#define SCN3_VALUE_TABLE(TABLE_ID, FLAGS, DISPLAY_NAME) ValueTableSpec{ValueTableId::TABLE_ID, DISPLAY_NAME, FLAGS},
inline constexpr ValueTableSpec kValueTables[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_VALUE_TABLE

constexpr std::size_t commandOffset(Family family, std::uint16_t slot) {
    std::size_t offset = 0;
    for (const FamilySpec &spec : kFamilies) {
        if (spec.family == family)
            return offset + slot;
        offset += spec.commandCount;
    }
    throw "Unknown scenario function family";
}

inline constexpr std::size_t kCommandCount = s1::kLegacyScenarioFunctionCount + s1::kGeneralScenarioFunctionCount + s1::kShenmueScenarioFunctionCount +
    s1::kDoorScenarioFunctionCount + s1::kSCNFScenarioFunctionCount;

consteval std::array<CommandSpec, kCommandCount> buildCommandCatalog() {
    std::array<CommandSpec, kCommandCount> commands{};
    std::size_t offset = 0;
    for (const FamilySpec &family : kFamilies) {
        for (std::uint16_t slot = 0; slot < family.commandCount; ++slot) {
            const bool paired = family.family == Family::SCNF;
            commands[offset++] = CommandSpec{.family = family.family,
                .publicIndex = slot,
                .dispatchIndex = static_cast<std::uint16_t>(paired ? slot / 2U : slot),
                .dispatchSubslot = static_cast<std::uint8_t>(paired ? slot % 2U : 0U),
                .handlerName = nullptr,
                .nativeTargetSymbol = nullptr,
                .resultKind = family.defaultResult,
                .arityKind = family.defaultArity,
                .fixedArgCount = family.fixedArgCount,
                .flags = static_cast<std::uint32_t>(CommandFlags::NullTarget)};
        }
    }
    if (offset != commands.size())
        throw "Scenario family counts do not match the dispatch inventory";

    const auto bind = [&commands](Family family, std::uint16_t slot, const char *handler) constexpr {
        CommandSpec &command = commands[commandOffset(family, slot)];
        command.handlerName = handler;
        command.nativeTargetSymbol = handler;
        command.flags = family == Family::SCNF ? static_cast<std::uint32_t>(slot % 2U ? CommandFlags::PairSecondary : CommandFlags::PairPrimary) : 0U;
    };

#define SCENARIO_PRESERVE_SLOT(index) bind(Family::Shenmue, index, "Script_PreserveAccumulator");
#define SCENARIO_ZERO_SLOT(index) bind(Family::Shenmue, index, "Script_ClearAccumulator");
#define SCENARIO_LEGACY_SLOT(index, handler) bind(Family::Legacy, index, #handler);
#define SCENARIO_GENERAL_SLOT(index, handler) bind(Family::GeneralPurpose, index, #handler);
#define SCENARIO_SHENMUE_SLOT(index, handler) bind(Family::Shenmue, index, #handler);
#define SCENARIO_SCNF_SLOT(index, handler) bind(Family::SCNF, index, #handler);
#define SCENARIO_DOOR_SLOT(index, handler) bind(Family::DSWK, index, #handler);
#define SCENARIO_SCNF_FILL(first, handler)                                                                                                                     \
    for (std::uint16_t slot = first; slot < s1::kSCNFScenarioFunctionCount; ++slot)                                                                            \
        bind(Family::SCNF, slot, #handler);
#include "s1/task/scenario_dispatch.def"
#undef SCENARIO_PRESERVE_SLOT
#undef SCENARIO_ZERO_SLOT
#undef SCENARIO_LEGACY_SLOT
#undef SCENARIO_GENERAL_SLOT
#undef SCENARIO_SHENMUE_SLOT
#undef SCENARIO_SCNF_SLOT
#undef SCENARIO_DOOR_SLOT
#undef SCENARIO_SCNF_FILL

    return commands;
}

inline constexpr std::array<CommandSpec, kCommandCount> kCommands = buildCommandCatalog();

#define SCN3_ALIAS(FAMILY, PUBLIC_INDEX, ALIAS_KIND, ALIAS_NAME) AliasSpec{Family::FAMILY, PUBLIC_INDEX, ALIAS_KIND, ALIAS_NAME},
inline constexpr std::array<AliasSpec, 0> kAliases = {{
#include "scn3/native/commands.def"
}};
#undef SCN3_ALIAS

#define SCN3_VARIANT(FAMILY, PUBLIC_INDEX, VARIANT_INDEX, NAME, RESULT_KIND, ARITY_KIND, FIXED_ARG_COUNT, FLAGS)                                               \
    VariantSpec{Family::FAMILY, PUBLIC_INDEX, VARIANT_INDEX, NAME, RESULT_KIND, ARITY_KIND, FIXED_ARG_COUNT, static_cast<std::uint32_t>(FLAGS)},
inline constexpr VariantSpec kVariants[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_VARIANT

#define SCN3_GUARD(FAMILY, PUBLIC_INDEX, VARIANT_INDEX, GUARD_KIND, ARG_INDEX, VALUE_EXPR)                                                                     \
    GuardSpec{Family::FAMILY, PUBLIC_INDEX, VARIANT_INDEX, GUARD_KIND, ARG_INDEX, static_cast<std::uint32_t>(VALUE_EXPR)},
inline constexpr GuardSpec kGuards[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_GUARD

#define SCN3_ARG(FAMILY, PUBLIC_INDEX, VARIANT_INDEX, ARG_INDEX, ARG_NAME, ARG_KIND, PREFERRED_WIDTH, VALUE_TABLE_ID, FLAGS)                                   \
    ArgSpec{Family::FAMILY, PUBLIC_INDEX, VARIANT_INDEX, ARG_INDEX, ARG_NAME, ARG_KIND, PREFERRED_WIDTH, VALUE_TABLE_ID, static_cast<std::uint32_t>(FLAGS)},
inline constexpr ArgSpec kArgs[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_ARG

#define SCN3_RULE(FAMILY, PUBLIC_INDEX, VARIANT_INDEX, RULE_KIND, DOMAIN, SELECTOR, ARG_INDEX, ARG_INDEX_END)                                                  \
    RuleSpec{Family::FAMILY, PUBLIC_INDEX, VARIANT_INDEX, RULE_KIND, DOMAIN, SELECTOR, ARG_INDEX, ARG_INDEX_END},
inline constexpr RuleSpec kRules[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_RULE

#define SCN3_VALUE_NAME(TABLE_ID, VALUE_EXPR, SYMBOL_NAME) ValueNameSpec{ValueTableId::TABLE_ID, static_cast<std::uint32_t>(VALUE_EXPR), SYMBOL_NAME},
inline constexpr ValueNameSpec kValueNames[] = {
#include "scn3/native/commands.def"
};
#undef SCN3_VALUE_NAME

inline constexpr VariantSpec kManualVariants[] = {
    VariantSpec{Family::Shenmue, 180, 1, "Script_SpriteUtils_Scroll_Read1", ResultKind::Integer, ArityKind::CleanupDriven, 0, 0},
};

inline constexpr GuardSpec kManualGuards[] = {
    GuardSpec{Family::Shenmue, 180, 1, GuardKind::ArgConstEquals, 0, static_cast<std::uint32_t>(Scroll_Read1)},
};

inline constexpr RuleSpec kManualRules[] = {
    RuleSpec{Family::Shenmue, 0, 0, RuleKind::CallPrintfFormatString, RuleDomain::Both, ArgSelector::Exact, 0, 0},
    RuleSpec{Family::Shenmue, 180, 1, RuleKind::ArgConsumesString, RuleDomain::Both, ArgSelector::Exact, 1, 1},
    RuleSpec{Family::Shenmue, 180, 1, RuleKind::ArgConsumesString, RuleDomain::Both, ArgSelector::Exact, 2, 2},
    RuleSpec{Family::SCNF, 0, 0, RuleKind::ArgPreferDecimal, RuleDomain::Both, ArgSelector::Exact, 0, 0},
    RuleSpec{Family::SCNF, 1, 0, RuleKind::ArgPreferDecimal, RuleDomain::Both, ArgSelector::Exact, 0, 0},
};

constexpr const FamilySpec *findFamily(Family family) {
    for (const auto &row : kFamilies)
        if (row.family == family)
            return &row;
    return nullptr;
}

constexpr const ValueTableSpec *findValueTable(ValueTableId id) {
    for (const auto &row : kValueTables)
        if (row.id == id)
            return &row;
    return nullptr;
}

constexpr const CommandSpec *findCommand(Family family, std::uint16_t publicIndex) {
    for (const auto &row : kCommands)
        if (row.family == family && row.publicIndex == publicIndex)
            return &row;
    return nullptr;
}

constexpr const CommandSpec *findCommandByDispatch(std::uint8_t opcodeFamily, std::uint16_t dispatchIndex, std::uint8_t dispatchSubslot = 0xFF) {
    for (const auto &row : kCommands) {
        const auto *family = findFamily(row.family);
        if (family != nullptr && family->opcodeFamily == opcodeFamily && row.dispatchIndex == dispatchIndex &&
            (dispatchSubslot == 0xFF || row.dispatchSubslot == dispatchSubslot))
            return &row;
    }
    return nullptr;
}

struct NameLookupResult {
    const CommandSpec *command = nullptr;
    bool viaAlias = false;
    bool ambiguous = false;
};

constexpr NameLookupResult findCommandByName(std::string_view name) {
    NameLookupResult result{};
    std::size_t matches = 0;
    for (const auto &row : kCommands) {
        if (row.handlerName != nullptr && name == row.handlerName) {
            if (result.command == nullptr)
                result.command = &row;
            result.viaAlias = false;
            ++matches;
        }
    }
    for (const auto &row : kAliases) {
        if (row.alias != nullptr && name == row.alias) {
            if (result.command == nullptr)
                result.command = findCommand(row.family, row.publicIndex);
            result.viaAlias = true;
            ++matches;
        }
    }
    result.ambiguous = matches > 1;
    return result;
}

constexpr Slice<AliasSpec> aliasesFor(Family family, std::uint16_t publicIndex) {
    std::size_t start = 0;
    while (start < countof(kAliases) && !(kAliases[start].family == family && kAliases[start].publicIndex == publicIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kAliases) && kAliases[end].family == family && kAliases[end].publicIndex == publicIndex)
        ++end;
    return {start < countof(kAliases) ? kAliases.data() + start : nullptr, end - start};
}

constexpr Slice<VariantSpec> variantsFor(Family family, std::uint16_t publicIndex) {
    std::size_t start = 0;
    while (start < countof(kVariants) && !(kVariants[start].family == family && kVariants[start].publicIndex == publicIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kVariants) && kVariants[end].family == family && kVariants[end].publicIndex == publicIndex)
        ++end;
    return {start < countof(kVariants) ? kVariants + start : nullptr, end - start};
}

constexpr Slice<VariantSpec> manualVariantsFor(Family family, std::uint16_t publicIndex) {
    std::size_t start = 0;
    while (start < countof(kManualVariants) && !(kManualVariants[start].family == family && kManualVariants[start].publicIndex == publicIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kManualVariants) && kManualVariants[end].family == family && kManualVariants[end].publicIndex == publicIndex)
        ++end;
    return {start < countof(kManualVariants) ? kManualVariants + start : nullptr, end - start};
}

constexpr Slice<GuardSpec> guardsFor(Family family, std::uint16_t publicIndex, std::uint16_t variantIndex) {
    std::size_t start = 0;
    while (start < countof(kGuards) &&
        !(kGuards[start].family == family && kGuards[start].publicIndex == publicIndex && kGuards[start].variantIndex == variantIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kGuards) && kGuards[end].family == family && kGuards[end].publicIndex == publicIndex && kGuards[end].variantIndex == variantIndex)
        ++end;
    return {start < countof(kGuards) ? kGuards + start : nullptr, end - start};
}

constexpr Slice<GuardSpec> manualGuardsFor(Family family, std::uint16_t publicIndex, std::uint16_t variantIndex) {
    std::size_t start = 0;
    while (start < countof(kManualGuards) &&
        !(kManualGuards[start].family == family && kManualGuards[start].publicIndex == publicIndex && kManualGuards[start].variantIndex == variantIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kManualGuards) && kManualGuards[end].family == family && kManualGuards[end].publicIndex == publicIndex &&
        kManualGuards[end].variantIndex == variantIndex)
        ++end;
    return {start < countof(kManualGuards) ? kManualGuards + start : nullptr, end - start};
}

constexpr Slice<ArgSpec> argsFor(Family family, std::uint16_t publicIndex, std::uint16_t variantIndex) {
    std::size_t start = 0;
    while (start < countof(kArgs) && !(kArgs[start].family == family && kArgs[start].publicIndex == publicIndex && kArgs[start].variantIndex == variantIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kArgs) && kArgs[end].family == family && kArgs[end].publicIndex == publicIndex && kArgs[end].variantIndex == variantIndex)
        ++end;
    return {start < countof(kArgs) ? kArgs + start : nullptr, end - start};
}

constexpr Slice<RuleSpec> rulesFor(Family family, std::uint16_t publicIndex, std::uint16_t variantIndex) {
    std::size_t start = 0;
    while (
        start < countof(kRules) && !(kRules[start].family == family && kRules[start].publicIndex == publicIndex && kRules[start].variantIndex == variantIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kRules) && kRules[end].family == family && kRules[end].publicIndex == publicIndex && kRules[end].variantIndex == variantIndex)
        ++end;
    return {start < countof(kRules) ? kRules + start : nullptr, end - start};
}

constexpr Slice<RuleSpec> manualRulesFor(Family family, std::uint16_t publicIndex, std::uint16_t variantIndex) {
    std::size_t start = 0;
    while (start < countof(kManualRules) &&
        !(kManualRules[start].family == family && kManualRules[start].publicIndex == publicIndex && kManualRules[start].variantIndex == variantIndex))
        ++start;
    std::size_t end = start;
    while (end < countof(kManualRules) && kManualRules[end].family == family && kManualRules[end].publicIndex == publicIndex &&
        kManualRules[end].variantIndex == variantIndex)
        ++end;
    return {start < countof(kManualRules) ? kManualRules + start : nullptr, end - start};
}

struct ObservedArg {
    bool hasConst = false;
    std::uint32_t value = 0;
};

constexpr bool guardMatches(const GuardSpec &guard, const ObservedArg *args, std::size_t argsCount) {
    switch (guard.kind) {
    case GuardKind::ArgConstEquals:
        return guard.argIndex < argsCount && args[guard.argIndex].hasConst && args[guard.argIndex].value == guard.value;
    }
    return false;
}

constexpr const VariantSpec *resolveVariant(
    Family family, std::uint16_t publicIndex, std::size_t argCount, const ObservedArg *args = nullptr, std::size_t argsCount = 0) {
    (void)argCount;
    const auto rows = variantsFor(family, publicIndex);
    for (const auto &variant : rows) {
        bool match = true;
        const auto guards = guardsFor(family, publicIndex, variant.variantIndex);
        for (const auto &guard : guards) {
            if (!guardMatches(guard, args, argsCount)) {
                match = false;
                break;
            }
        }
        if (match)
            return &variant;
    }
    return nullptr;
}

constexpr const VariantSpec *resolveManualVariant(
    Family family, std::uint16_t publicIndex, std::size_t argCount, const ObservedArg *args = nullptr, std::size_t argsCount = 0) {
    (void)argCount;
    const auto rows = manualVariantsFor(family, publicIndex);
    for (const auto &variant : rows) {
        bool match = true;
        const auto guards = manualGuardsFor(family, publicIndex, variant.variantIndex);
        for (const auto &guard : guards) {
            if (!guardMatches(guard, args, argsCount)) {
                match = false;
                break;
            }
        }
        if (match)
            return &variant;
    }
    return nullptr;
}

constexpr Slice<ValueNameSpec> valueNamesFor(ValueTableId table) {
    std::size_t start = 0;
    while (start < countof(kValueNames) && kValueNames[start].table != table)
        ++start;
    std::size_t end = start;
    while (end < countof(kValueNames) && kValueNames[end].table == table)
        ++end;
    return {start < countof(kValueNames) ? kValueNames + start : nullptr, end - start};
}

} // namespace shenmue::scenario::abi
