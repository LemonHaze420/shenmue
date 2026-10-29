#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario {

struct Program;

void loadScriptMotionNames(const Program &program, const std::filesystem::path &scenario);
void defineMotionName(const std::string &name, uint32_t id);
std::optional<std::string_view> motionName(uint32_t id);
std::optional<uint32_t> motionIdByName(std::string_view name);
const std::vector<std::filesystem::path> &loadedMotionHeaders();
void loadMotionSourceHeaders(const std::string &text, const std::filesystem::path &directory);
bool loadMotionInclude(const std::string &name);
std::string usedMotionIncludes(const std::string &text);
std::string insertMotionIncludes(std::string text);

} // namespace shenmue::scenario
