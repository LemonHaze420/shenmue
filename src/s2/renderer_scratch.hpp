#pragma once

#include "d3t/scratch_memory.hpp"

namespace shenmue::s2 {
using d3t::g_rendererScratch;
using d3t::GetRendererScratchBlock;
using d3t::RendererScratchState;
using d3t::RestoreRendererScratchBlock;
void InitializeRendererScratch();
void FreeRendererScratch();
} // namespace shenmue::s2
