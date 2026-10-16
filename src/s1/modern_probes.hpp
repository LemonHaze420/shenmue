#pragma once

#include "s1/d3t/d3t_buffer.hpp"

#include <cstdint>

namespace shenmue::s1 {

struct ModernProbeConstants {
    std::uint32_t disableLocalProbes;
    std::uint32_t probeMapPass;
    std::int32_t globalProbeIndex;
};

static_assert(sizeof(ModernProbeConstants) == 12);

void UpdateSceneProbes(D3TDeviceContext *context);

} // namespace shenmue::s1
