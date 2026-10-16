#include "s1/modern_probes.hpp"

#include "s1/d3t/renderer.hpp"
#include "d3t/render_state.hpp"

#include <array>
#include <cstddef>
#include <cstring>

namespace shenmue::s1 {
namespace {


std::uint32_t g_probeStructuredBuffer = 0;
std::array<std::uint32_t, 2> g_probeTextureArrays{};
std::uint32_t g_probeCubeTexture = 0;
std::uint8_t g_disableLocalProbes = 0;
std::int32_t g_globalProbeIndex = 0;

}

void UpdateSceneProbes(D3TDeviceContext *context) {
#if defined(_WIN32)
    if (void *const mapped = MapTempConstantBufferForWriteDiscard(context, 2, 11, sizeof(ModernProbeConstants))) {
        const ModernProbeConstants constants{.disableLocalProbes = g_disableLocalProbes, .probeMapPass = 0, .globalProbeIndex = g_globalProbeIndex};
        std::memcpy(mapped, &constants, sizeof(constants));
    }
#endif
    std::byte *const bytes = context->renderState.data();
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t buffer;
    std::memcpy(&buffer, bytes + 0x764, sizeof(buffer));
    if (buffer != g_probeStructuredBuffer) {
        std::memcpy(bytes + 0x764, &g_probeStructuredBuffer, sizeof(g_probeStructuredBuffer));
        state.shaderResourceDirtyMasks[1] |= 1U << 4;
    }
    for (std::uint32_t index = 0; index < 3; ++index) {
        const std::uint32_t slot = 24 + index;
        const std::uint32_t texture = index < 2 ? g_probeTextureArrays[index] : g_probeCubeTexture;

        const std::uint32_t flags = index < 2 ? 1U : 0U;
        std::uint32_t previousTexture;
        std::uint32_t previousFlags;
        std::memcpy(&previousTexture, bytes + 0x1b4 + slot * 4, sizeof(previousTexture));
        std::memcpy(&previousFlags, bytes + 0x334 + slot * 4, sizeof(previousFlags));
        if (previousTexture != texture || previousFlags != flags) {
            std::memcpy(bytes + 0x1b4 + slot * 4, &texture, sizeof(texture));
            std::memcpy(bytes + 0x334 + slot * 4, &flags, sizeof(flags));
            state.shaderResourceDirtyMasks[1] |= 1U << slot;
        }
    }
    constexpr std::array<std::uint32_t, 2> slots{2, 10};
    constexpr std::array<std::uint64_t, 2> descriptors{661, 640};
    for (std::uint32_t index = 0; index < 2; ++index) {
        std::byte *const descriptor = bytes + 0x534 + slots[index] * 16;
        std::uint64_t previous;
        std::memcpy(&previous, descriptor, sizeof(previous));
        if (previous != descriptors[index]) {
            const std::array<std::uint64_t, 2> sampler{descriptors[index], 0};
            std::memcpy(descriptor, sampler.data(), sizeof(sampler));
            state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << slots[index]);
        }
    }
}

} // namespace shenmue::s1
