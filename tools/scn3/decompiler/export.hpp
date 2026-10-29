#pragma once

#include "scn3/bytecode/program.hpp"

namespace shenmue::scenario {

std::string renderScenarioSource(const Program &program, const std::string &profile);

std::string renderExplicitSource(const Program &program, const std::string &profile, const std::set<std::string> &plain = {}, SourceUnit *parsed = nullptr);

void exportExplicitSource(const Program &program, const std::string &path, const std::string &profile, const std::optional<std::string> &names = {});
std::map<std::string, std::string> readSymbolNames(const std::string &path);
std::string renameSourceSymbols(const std::string &text, const std::map<std::string, std::string> &names);

std::string renderCRecoveryReport(const Program &program, const std::string &profile, const std::optional<std::string> &onlyFunction = {});
std::string renderRecoveredCFunction(const Program &program, const std::string &profile, const std::string &name, bool strict = false);

void exportScenarioAssembly(const Program &program, const std::string &path, const std::string &profile);

} // namespace shenmue::scenario
