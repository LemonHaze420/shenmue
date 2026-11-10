#pragma once

#include <d3t/scratch_memory.hpp>

namespace shenmue::s1 {

using d3t::AllocateMemoryFromGlobalPool;
using d3t::GetGlobalPoolCheckpoint;
using d3t::ResetGlobalPoolSlices;
using d3t::RestoreGlobalPoolCheckpoint;

} // namespace shenmue::s1
