#pragma once

#if defined(_WIN32)
#include <xaudio2fx.h>
#include <xapofx.h>

namespace shenmue::s2 {

inline constexpr FXMASTERINGLIMITER_PARAMETERS CONVERSATION_MASTERING_LIMITER{.Release = 6, .Loudness = 1000};
inline constexpr float CONVERSATION_WET_OUTPUT_GAIN = 5.0f;
inline constexpr XAUDIO2FX_REVERB_I3DL2_PARAMETERS CONVERSATION_REVERB_PRESET{.WetDryMix = 100.0f,
    .Room = -1000,
    .RoomHF = -600,
    .RoomRolloffFactor = 0.0f,
    .DecayTime = 1.8f,
    .DecayHFRatio = 0.7f,
    .Reflections = -1300,
    .ReflectionsDelay = 0.015f,
    .Reverb = -800,
    .ReverbDelay = 0.03f,
    .Diffusion = 100.0f,
    .Density = 100.0f,
    .HFReference = 5000.0f};
static_assert(sizeof(FXMASTERINGLIMITER_PARAMETERS) == 8);
static_assert(sizeof(XAUDIO2FX_REVERB_I3DL2_PARAMETERS) == 52);

} // namespace shenmue::s2
#endif
