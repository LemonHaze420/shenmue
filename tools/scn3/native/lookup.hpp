#pragma once

#include "scn3/common/std.hpp"
#include "scn3/native/abi.hpp"

namespace shenmue::scenario {

struct AbiValue {
    bool hasConst = false;
    uint32_t constValue = 0;
    uint8_t literalWidth = 0;
};

struct ExplicitAbiName {
    std::string_view baseName;
    uint16_t publicIndex = 0;
};

std::optional<ExplicitAbiName> splitExplicitAbiName(std::string_view name);
bool abiCommandNameMatches(const abi::CommandSpec &command, std::string_view name);
std::vector<const abi::CommandSpec *> abiCommandCandidatesForCallName(const std::string &name);
bool abiCallIsPublicIndex(const std::string &name, abi::Family family, uint16_t publicIndex);
std::vector<abi::ObservedArg> abiObservedArgs(const std::vector<AbiValue> &args);
std::vector<uint16_t> abiVariantSearchOrder(const abi::CommandSpec &command, const std::vector<AbiValue> &args);
bool abiRuleDomainApplies(abi::RuleDomain domain, abi::RuleDomain use);
bool abiArgSelectorMatches(const abi::RuleSpec &rule, size_t index);
bool abiCallArgHasRule(const std::string &name, const std::vector<AbiValue> &args, size_t index, abi::RuleKind kind, abi::RuleDomain use);
bool abiArgHasFlag(const abi::ArgSpec &arg, abi::ArgFlags flag);
std::optional<uint32_t> signExtendNarrowLiteralValue(uint32_t value, uint8_t width);
std::optional<uint32_t> lookupAbiValueByName(std::string_view name);
std::optional<std::string_view> lookupAbiValueName(abi::ValueTableId table, uint32_t value);
abi::ValueTableId abiValueTableNamed(std::string_view name);
std::optional<std::string_view> lookupAbiArgValueName(const abi::ArgSpec &arg, const AbiValue &value);
std::optional<uint32_t> abiBytecodeIndexIfCallable(const abi::CommandSpec &command);
std::optional<uint32_t> lookupAbiBytecodeIndexByName(abi::Family family, const std::string &name);
std::string publicAbiCallName(const abi::CommandSpec &command);
std::string lookupAbiName(abi::Family family, int index, const char *fallbackPrefix);
std::string lookupAbiDispatchName(abi::Family family, int dispatchIndex, uint8_t dispatchSubslot, const char *fallbackPrefix);
std::string lookupLegacy(int index);
std::string lookupGp(int index);
std::string lookupShenmue(int index);
std::string lookupDSWK(int index);
std::string lookupScnf(int index);

const abi::CommandSpec *abiExplicitCommandForName(std::string_view name, std::optional<abi::Family> family = std::nullopt);

const abi::CommandSpec *abiCommandForHandler(std::string_view name, std::optional<abi::Family> family = std::nullopt);

const abi::CommandSpec *abiCommandForCallName(const std::string &name);

const abi::ArgSpec *abiCallArgSpecForSourceArg(const std::string &name, size_t index);

} // namespace shenmue::scenario
