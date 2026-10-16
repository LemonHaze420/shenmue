#pragma once

#include "d3t/compression.hpp"

namespace shenmue::s1 {

using d3t::CompressedContentHeader;
using d3t::D3TCompressionFunction;
using d3t::D3TCompressionMode;
using d3t::D3TCompressionRequest;
using d3t::D3TCompressionSizeFunction;
using d3t::d3t_DecompressContent;
using d3t::d3t_InitializeOodleZLib;
using d3t::g_d3tCompressionModes;
using d3t::g_oodleMode;
using d3t::g_shouldShutdownOodle;

} // namespace shenmue::s1
