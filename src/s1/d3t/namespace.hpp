#pragma once

// S1's own D3T engine functions live in shenmue::s1::d3t, which also sees the shared engine (shenmue::d3t), so `d3t::` reaches both.
namespace shenmue::d3t {}

namespace shenmue::s1::d3t {
using namespace ::shenmue::d3t;
} // namespace shenmue::s1::d3t
