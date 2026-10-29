#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario {

bool isNativeScenario(const std::vector<uint8_t> &blob);

std::vector<uint8_t> translateScenarioToSh4(const std::vector<uint8_t> &vm);

std::vector<uint8_t> liftScenarioFromSh4(const std::vector<uint8_t> &native);

} // namespace shenmue::scenario
