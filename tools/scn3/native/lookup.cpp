#include "scn3/native/abi.hpp"
#include "scn3/native/lookup.hpp"

#include <mutex>

namespace shenmue::scenario {

std::optional<ExplicitAbiName> splitExplicitAbiName(std::string_view name) {
    if (name.size() < 6) {
        return std::nullopt;
    }
    const size_t suffixPos = name.size() - 5;
    if (name[suffixPos] != '_') {
        return std::nullopt;
    }
    uint32_t value = 0;
    for (size_t i = suffixPos + 1; i < name.size(); ++i) {
        const unsigned char ch = static_cast<unsigned char>(name[i]);
        uint32_t digit = 0;
        if (ch >= '0' && ch <= '9') {
            digit = ch - '0';
        } else if (ch >= 'A' && ch <= 'F') {
            digit = 10u + ch - 'A';
        } else if (ch >= 'a' && ch <= 'f') {
            digit = 10u + ch - 'a';
        } else {
            return std::nullopt;
        }
        value = (value << 4) | digit;
    }
    return ExplicitAbiName{name.substr(0, suffixPos), static_cast<uint16_t>(value)};
}

bool abiCommandNameMatches(const abi::CommandSpec &command, std::string_view name) {
    if (command.handlerName != nullptr && name == command.handlerName) {
        return true;
    }
    if (name == publicAbiCallName(command)) {
        return true;
    }
    for (const abi::AliasSpec &alias : abi::aliasesFor(command.family, command.publicIndex)) {
        if (alias.alias != nullptr && name == alias.alias) {
            return true;
        }
    }
    return false;
}

const abi::CommandSpec *abiExplicitCommandForName(std::string_view name, std::optional<abi::Family> family) {
    const std::optional<ExplicitAbiName> explicitName = splitExplicitAbiName(name);
    if (!explicitName.has_value()) {
        return nullptr;
    }
    const abi::CommandSpec *found = nullptr;
    for (const abi::CommandSpec &command : abi::kCommands) {
        if (family.has_value() && command.family != *family) {
            continue;
        }
        if (command.publicIndex != explicitName->publicIndex || !abiCommandNameMatches(command, explicitName->baseName)) {
            continue;
        }
        if (found != nullptr) {
            return nullptr;
        }
        found = &command;
    }
    return found;
}

const abi::CommandSpec *abiCommandForHandler(std::string_view name, std::optional<abi::Family> family) {
    const abi::CommandSpec *found = nullptr;
    for (const abi::CommandSpec &command : abi::kCommands) {
        if (family.has_value() && command.family != *family) {
            continue;
        }
        if (command.handlerName == nullptr || name != command.handlerName) {
            continue;
        }
        if (found != nullptr) {
            return nullptr;
        }
        found = &command;
    }
    return found;
}

static const abi::CommandSpec *resolveAbiCommandForCallName(const std::string &name) {
    if (const abi::CommandSpec *command = abiExplicitCommandForName(name)) {
        return command;
    }
    if (const abi::CommandSpec *command = abiCommandForHandler(name)) {
        return command;
    }
    const abi::NameLookupResult lookup = abi::findCommandByName(name);
    if (lookup.ambiguous) {
        return nullptr;
    }
    if (lookup.command != nullptr) {
        return lookup.command;
    }
    const abi::CommandSpec *found = nullptr;
    for (const abi::CommandSpec &command : abi::kCommands) {
        if (!abiCommandNameMatches(command, name)) {
            continue;
        }
        if (found != nullptr) {
            return nullptr;
        }
        found = &command;
    }
    if (found != nullptr) {
        return found;
    }
    return lookup.command;
}

const abi::CommandSpec *abiCommandForCallName(const std::string &name) {
    static std::mutex mutex;
    static std::unordered_map<std::string, const abi::CommandSpec *> resolved;
    {
        const std::lock_guard<std::mutex> lock(mutex);
        const std::unordered_map<std::string, const abi::CommandSpec *>::const_iterator found = resolved.find(name);
        if (found != resolved.end())
            return found->second;
    }
    const abi::CommandSpec *command = resolveAbiCommandForCallName(name);
    const std::lock_guard<std::mutex> lock(mutex);
    resolved.emplace(name, command);
    return command;
}

std::vector<const abi::CommandSpec *> abiCommandCandidatesForCallName(const std::string &name) {
    std::vector<const abi::CommandSpec *> out;
    auto append = [&](const abi::CommandSpec *command) {
        if (command != nullptr && std::find(out.begin(), out.end(), command) == out.end()) {
            out.push_back(command);
        }
    };

    if (const abi::CommandSpec *command = abiExplicitCommandForName(name)) {
        append(command);
        return out;
    }
    if (const abi::CommandSpec *command = abiCommandForHandler(name)) {
        append(command);
        return out;
    }

    const abi::NameLookupResult lookup = abi::findCommandByName(name);
    if (!lookup.ambiguous && lookup.command != nullptr) {
        append(lookup.command);
        return out;
    }

    for (const abi::CommandSpec &command : abi::kCommands) {
        if (abiCommandNameMatches(command, name)) {
            append(&command);
        }
    }
    for (const abi::AliasSpec &alias : abi::kAliases) {
        if (alias.alias != nullptr && name == alias.alias) {
            append(abi::findCommand(alias.family, alias.publicIndex));
        }
    }
    return out;
}

bool abiCallIsPublicIndex(const std::string &name, abi::Family family, uint16_t publicIndex) {
    if (const abi::CommandSpec *explicitCommand = abiExplicitCommandForName(name, family)) {
        return explicitCommand->publicIndex == publicIndex;
    }
    const abi::CommandSpec *command = abi::findCommand(family, publicIndex);
    if (command != nullptr && abiCommandNameMatches(*command, name)) {
        return true;
    }
    for (const abi::AliasSpec &alias : abi::aliasesFor(family, publicIndex)) {
        if (alias.alias != nullptr && name == alias.alias) {
            return true;
        }
    }
    return false;
}

std::vector<abi::ObservedArg> abiObservedArgs(const std::vector<AbiValue> &args) {
    std::vector<abi::ObservedArg> observed(args.size());
    for (size_t i = 0; i < args.size(); ++i) {
        observed[i].hasConst = args[i].hasConst;
        observed[i].value = args[i].constValue;
    }
    return observed;
}

std::vector<uint16_t> abiVariantSearchOrder(const abi::CommandSpec &command, const std::vector<AbiValue> &args) {
    std::vector<uint16_t> variants{0};
    const std::vector<abi::ObservedArg> observed = abiObservedArgs(args);
    auto appendVariant = [&](uint16_t variantIndex) {
        if (std::find(variants.begin(), variants.end(), variantIndex) == variants.end()) {
            variants.push_back(variantIndex);
        }
    };
    const abi::VariantSpec *variant =
        abi::resolveVariant(command.family, command.publicIndex, args.size(), observed.empty() ? nullptr : observed.data(), observed.size());
    if (variant != nullptr && variant->variantIndex != 0) {
        appendVariant(variant->variantIndex);
    }
    const abi::VariantSpec *manualVariant =
        abi::resolveManualVariant(command.family, command.publicIndex, args.size(), observed.empty() ? nullptr : observed.data(), observed.size());
    if (manualVariant != nullptr && manualVariant->variantIndex != 0) {
        appendVariant(manualVariant->variantIndex);
    }
    return variants;
}

bool abiRuleDomainApplies(abi::RuleDomain domain, abi::RuleDomain use) {
    if (use == abi::RuleDomain::Both) {
        return domain == abi::RuleDomain::Both;
    }
    if (domain == abi::RuleDomain::Both) {
        return true;
    }
    return domain == use;
}

bool abiArgSelectorMatches(const abi::RuleSpec &rule, size_t index) {
    switch (rule.selector) {
    case abi::ArgSelector::Exact:
        return index == rule.argIndex;
    case abi::ArgSelector::Range:
        return index >= rule.argIndex && index <= rule.argIndexEnd;
    case abi::ArgSelector::FromIndex:
        return index >= rule.argIndex;
    case abi::ArgSelector::Any:
        return true;
    case abi::ArgSelector::WholeCall:
        return false;
    }
    return false;
}

bool abiCallArgHasRule(const std::string &name, const std::vector<AbiValue> &args, size_t index, abi::RuleKind kind, abi::RuleDomain use) {
    for (const abi::CommandSpec *command : abiCommandCandidatesForCallName(name)) {
        for (uint16_t variantIndex : abiVariantSearchOrder(*command, args)) {
            const abi::Slice<abi::RuleSpec> rules = abi::rulesFor(command->family, command->publicIndex, variantIndex);
            for (const abi::RuleSpec &rule : rules) {
                if (rule.kind == kind && abiRuleDomainApplies(rule.domain, use) && abiArgSelectorMatches(rule, index)) {
                    return true;
                }
            }
            const abi::Slice<abi::RuleSpec> manualRules = abi::manualRulesFor(command->family, command->publicIndex, variantIndex);
            for (const abi::RuleSpec &rule : manualRules) {
                if (rule.kind == kind && abiRuleDomainApplies(rule.domain, use) && abiArgSelectorMatches(rule, index)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool abiArgHasFlag(const abi::ArgSpec &arg, abi::ArgFlags flag) {
    return (arg.flags & static_cast<uint32_t>(flag)) != 0;
}

const abi::ArgSpec *abiCallArgSpecForSourceArg(const std::string &name, size_t index) {
    const abi::ArgSpec *selected = nullptr;
    for (const abi::CommandSpec *command : abiCommandCandidatesForCallName(name)) {
        const abi::Slice<abi::ArgSpec> argSpecs = abi::argsFor(command->family, command->publicIndex, 0);
        for (const abi::ArgSpec &arg : argSpecs) {
            if (arg.argIndex != index) {
                continue;
            }
            if (selected != nullptr && (selected->kind != arg.kind || selected->valueTable != arg.valueTable || selected->flags != arg.flags)) {
                return nullptr;
            }
            selected = &arg;
        }
    }
    return selected;
}

std::optional<uint32_t> signExtendNarrowLiteralValue(uint32_t value, uint8_t width) {
    if (width == 1) {
        return static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(value & 0xFFu)));
    }
    if (width == 2) {
        return static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(value & 0xFFFFu)));
    }
    return std::nullopt;
}

std::optional<uint32_t> lookupAbiValueByName(std::string_view name) {
    static const std::unordered_map<std::string_view, std::optional<uint32_t>> values = [] {
        std::unordered_map<std::string_view, std::optional<uint32_t>> index;
        for (const abi::ValueNameSpec &row : abi::kValueNames) {
            if (row.symbol == nullptr)
                continue;
            const std::pair<std::unordered_map<std::string_view, std::optional<uint32_t>>::iterator, bool> inserted = index.emplace(row.symbol, row.value);
            if (!inserted.second && inserted.first->second != row.value)
                inserted.first->second.reset();
        }
        return index;
    }();
    const std::unordered_map<std::string_view, std::optional<uint32_t>>::const_iterator found = values.find(name);
    return found == values.end() ? std::nullopt : found->second;
}

abi::ValueTableId abiValueTableNamed(std::string_view name) {
    for (const abi::ValueTableSpec &table : abi::kValueTables)
        if (table.displayName && name == table.displayName)
            return table.id;
    return abi::ValueTableId::None;
}

std::optional<std::string_view> lookupAbiValueName(abi::ValueTableId table, uint32_t value) {
    if (table == abi::ValueTableId::None) {
        return std::nullopt;
    }
    for (const abi::ValueNameSpec &row : abi::valueNamesFor(table)) {
        if (row.symbol != nullptr && row.value == value) {
            const std::string_view symbol(row.symbol);
            const std::optional<uint32_t> roundTripValue = lookupAbiValueByName(symbol);
            if (roundTripValue.has_value() && *roundTripValue == value) {
                return symbol;
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string_view> lookupAbiArgValueName(const abi::ArgSpec &arg, const AbiValue &value) {
    if (const std::optional<std::string_view> symbol = lookupAbiValueName(arg.valueTable, value.constValue)) {
        return symbol;
    }
    if (!abiArgHasFlag(arg, abi::ArgFlags::SignExtendNarrowLiteral)) {
        return std::nullopt;
    }
    const std::optional<uint32_t> runtimeValue = signExtendNarrowLiteralValue(value.constValue, value.literalWidth);
    if (!runtimeValue.has_value() || *runtimeValue == value.constValue) {
        return std::nullopt;
    }
    return lookupAbiValueName(arg.valueTable, *runtimeValue);
}

std::optional<uint32_t> abiBytecodeIndexIfCallable(const abi::CommandSpec &command) {
    if (command.family == abi::Family::SCNF && command.dispatchSubslot != 0) {
        return std::nullopt;
    }
    return command.dispatchIndex;
}

std::optional<uint32_t> lookupAbiBytecodeIndexByName(abi::Family family, const std::string &name) {
    if (const abi::CommandSpec *command = abiExplicitCommandForName(name, family)) {
        return abiBytecodeIndexIfCallable(*command);
    }
    if (const abi::CommandSpec *command = abiCommandForHandler(name, family)) {
        return abiBytecodeIndexIfCallable(*command);
    }
    const abi::NameLookupResult lookup = abi::findCommandByName(name);
    if (!lookup.ambiguous && lookup.command != nullptr && lookup.command->family == family) {
        return abiBytecodeIndexIfCallable(*lookup.command);
    }
    const abi::CommandSpec *found = nullptr;
    for (const abi::CommandSpec &command : abi::kCommands) {
        if (command.family != family || !abiCommandNameMatches(command, name)) {
            continue;
        }
        if (!abiBytecodeIndexIfCallable(command).has_value()) {
            continue;
        }
        if (found != nullptr) {
            return std::nullopt;
        }
        found = &command;
    }
    if (found != nullptr) {
        return abiBytecodeIndexIfCallable(*found);
    }
    return std::nullopt;
}

} // namespace shenmue::scenario
