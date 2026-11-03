#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace shenmue {
void DecodeYamahaAdpcm(const std::uint8_t *input, std::size_t bytes, std::vector<std::uint8_t> &output);
}
