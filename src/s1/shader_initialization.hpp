#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/buffer.hpp"

#include <cstdint>

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;

extern std::uint32_t g_activeRenderHandle;
extern std::int32_t g_bHasSetupShaders;

std::int64_t LoadShaders();
namespace d3t {
std::int64_t SetupForDepthBufferPass(D3TDeviceContext *renderer);
} // namespace d3t
std::int64_t ClearEffectABuffer();
std::int64_t ComposeEffectABuffer();

} // namespace shenmue::s1
