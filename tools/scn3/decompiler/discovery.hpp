#pragma once

#include "scn3/bytecode/program.hpp"

namespace shenmue::scenario {

struct DecodeOptions {
    std::optional<uint32_t> startOverride;
    bool decodeUncovered = false;
};

Program buildProgram(const std::vector<uint8_t> &blob, const DecodeOptions &options);

} // namespace shenmue::scenario
