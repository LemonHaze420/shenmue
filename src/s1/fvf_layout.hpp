#pragma once

#include "d3t/fvf.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

using D3TFVFElement = d3t::FVFElement;
using D3TFVFDescription = d3t::FVFDescription;
using d3t::CreateFVF;

extern std::array<std::uint32_t, 40> g_fvfLayouts;
extern std::uint32_t g_originalStrip00InputLayout;
extern std::uint32_t g_originalStrip03InputLayout;

inline const D3TFVFDescription *d3t_GetFVFDescription(std::uint32_t handle, std::uint32_t *index) {
    return d3t::GetFVFDescription(handle, index);
}
std::int64_t CreateFVFLayouts();

} // namespace shenmue::s1
