#include "platform/dc/particles.hpp"


#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || (SHENMUE_IS_DC && SHENMUE_IS_S2))
namespace shenmue { namespace dc { namespace sdk {

// @verified - d
void DrawParticleChannel(std::int32_t index) {
    if (index == -1) {
        DrawParticleChannels();
        return;
    }
    for (std::int32_t channel = 0; channel < 16; ++channel) {
        if (channel == index)
            continue;
        std::uint32_t flags = g_particleChannels[channel].flags & 0xfe7fffffU;
        flags |= (flags >> 2U) & 0x01800000U;
        g_particleChannels[channel].flags = flags | 0x06000000U;
    }
    DrawParticleChannels();
    for (std::int32_t channel = 0; channel < 16; ++channel) {
        if (channel == index)
            continue;
        std::uint32_t flags = g_particleChannels[channel].flags & 0xf9ffffffU;
        flags |= (flags << 2U) & 0x06000000U;
        g_particleChannels[channel].flags = flags;
    }
}

} } } // namespace shenmue::dc::sdk
#endif
