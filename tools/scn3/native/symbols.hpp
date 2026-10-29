#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario {

std::optional<uint32_t> shenTypeConstantValueByName(std::string_view name);

} // namespace shenmue::scenario
