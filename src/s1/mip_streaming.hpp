#pragma once

#include <d3t/mip_streaming.hpp>
#include "s1/d3t/namespace.hpp"

namespace shenmue::s1 {
std::int32_t LoadMipMap_Wrapper(void *unused);
std::uint32_t QueuePendingMipMapLoads();
void UpdateMipMaps();
} // namespace shenmue::s1
