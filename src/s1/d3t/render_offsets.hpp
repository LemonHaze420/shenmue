#pragma once

#include "s1/d3t/namespace.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

namespace d3t {
void *GetPointerFromOffset(std::uint32_t handle);
std::byte *GetRenderOffsetEntry(std::uint32_t handle);
} // namespace d3t

} // namespace shenmue::s1
