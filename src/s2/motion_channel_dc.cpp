#include "s2/motion_stream_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <float.h>
#include <cstring>

namespace shenmue {
namespace s2 {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {
float MotionChannelFloat(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
} // namespace
#endif
#define SHENMUE_MOTION_CHANNEL_FLOAT MotionChannelFloat
#define SHENMUE_MOTION_CHANNEL_ABS __builtin_fabsf
#define SHENMUE_MOTION_CHANNEL_FMA(x, y, z) ((x) * (y) + (z))

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float DecodeMotnFloat(std::uint16_t value) {
    volatile float result = 0.0f;
    if (!value)
        return 0.0f;
    *reinterpret_cast<volatile std::uint32_t *>(&result) =
        (static_cast<std::uint32_t>(value & 0x8000U) << 16U) | ((static_cast<std::uint32_t>(value & 0x7fffU) << 13U) + 0x38000000U);
    return result;
}

// @verified - d
float SampleMotionRigChannelAdvance(MotionRigChannel &channel) {
    if (!(g_motionCurrentAnimTime > channel.current.timeSeconds))
        return channel.current.value;
    for (;;) {
        if (g_motionCurrentAnimTime < channel.next.timeSeconds) {
            if (g_motionInterpolationType & MI_Linear) {
                const float inverse = channel.inverseDuration;
                const float time = g_motionCurrentAnimTime - channel.current.timeSeconds;
                const float currentValue = channel.current.value;
                float value = currentValue;
                value = (channel.next.value - currentValue) * inverse * time + value;
                return value;
            }
            const float inverse = channel.inverseDuration;
            const float time = g_motionCurrentAnimTime - channel.current.timeSeconds;
            const float currentValue = channel.current.value;
            const float inverseSquared = inverse * inverse;
            const float currentTangent = channel.current.tangentOut;
            const float timeSquared = time * time;
            const float scaledSquared = timeSquared * inverse;
            const float nextValue = channel.next.value;
            const float cubic = (timeSquared * time) * inverseSquared;
            const float nextTangent = channel.next.tangentIn;
            const float twiceCubic = (cubic + cubic) * inverse, tripleSquared = (timeSquared * 3.0f) * inverseSquared;
            const float firstWeight = (twiceCubic - tripleSquared) + 1.0f, secondWeight = -twiceCubic + tripleSquared;
            float value = currentValue * firstWeight + nextValue * secondWeight;
            const float lastTangentWeight = cubic - scaledSquared, firstTangentWeight = (lastTangentWeight - scaledSquared) + time;
            value = currentTangent * firstTangentWeight + value;
            value = nextTangent * lastTangentWeight + value;
            return value;
        }
        if (__builtin_fabsf(channel.next.timeSeconds - g_motionCurrentAnimTime) < FLT_MIN)
            return channel.next.value;
        if (channel.keysRemaining) {
            channel.current = channel.next;
            const std::int16_t flags = *channel.frameCursor;
            channel.next.value = DecodeMotnFloat(*channel.frameCursor++) * channel.scale + channel.bias;
            channel.next.tangentIn = DecodeMotnFloat(*channel.frameCursor++) * channel.scale;
            if (flags & 1)
                channel.next.tangentOut = DecodeMotnFloat(*channel.frameCursor++) * channel.scale;
            else
                channel.next.tangentOut = channel.next.tangentIn;
            if (--channel.keysRemaining)
                channel.next.timeSeconds = static_cast<float>(static_cast<std::int16_t>(*channel.frameCursor++)) * kMotionStreamFrameSeconds;
            else
                channel.next.timeSeconds = g_motionMaximumAnimTime;
            channel.inverseDuration = 1.0f / (channel.next.timeSeconds - channel.current.timeSeconds);
        } else
            return channel.next.value;
    }
}
#else
// @verified - d
float DecodeMotnFloat(std::uint16_t value) {
    if (!value)
        return 0.0f;
    return SHENMUE_MOTION_CHANNEL_FLOAT(
        (static_cast<std::uint32_t>(value & 0x8000U) << 16U) | ((static_cast<std::uint32_t>(value & 0x7fffU) << 13U) + 0x38000000U));
}

// @verified - d
float SampleMotionRigChannelAdvance(MotionRigChannel &channel) {
    if (!(g_motionCurrentAnimTime > channel.current.timeSeconds))
        return channel.current.value;
    for (;;) {
        if (channel.next.timeSeconds > g_motionCurrentAnimTime) {
            const float time = g_motionCurrentAnimTime - channel.current.timeSeconds;
            const float inverse = channel.inverseDuration;
            if (g_motionInterpolationType & MI_Linear) {
                const float rate = (channel.next.value - channel.current.value) * inverse;
                float value = channel.current.value;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                value = SHENMUE_MOTION_CHANNEL_FMA(rate, time, value);
#else
                asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+&f"(value) : "f"(rate), "f"(time) : "fr0");
#endif
                return value;
            }
            const float inverseSquared = inverse * inverse, timeSquared = time * time;
            const float scaledSquared = timeSquared * inverse, cubic = (timeSquared * time) * inverseSquared;
            const float twiceCubic = (cubic + cubic) * inverse, tripleSquared = (timeSquared * 3.0f) * inverseSquared;
            const float firstWeight = (twiceCubic - tripleSquared) + 1.0f, secondWeight = -twiceCubic + tripleSquared;
            const float lastTangentWeight = cubic - scaledSquared, firstTangentWeight = (lastTangentWeight - scaledSquared) + time;
            volatile float initial = secondWeight * channel.next.value;
            float value = initial;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            value = SHENMUE_MOTION_CHANNEL_FMA(channel.current.value, firstWeight, value);
            value = SHENMUE_MOTION_CHANNEL_FMA(channel.current.tangentOut, firstTangentWeight, value);
            value = SHENMUE_MOTION_CHANNEL_FMA(channel.next.tangentIn, lastTangentWeight, value);
#else
            asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0\n\tfmov %3,fr0\n\tfmac fr0,%4,%0\n\tfmov %5,fr0\n\tfmac fr0,%6,%0"
                : "+&f"(value)
                : "f"(channel.current.value), "f"(firstWeight), "f"(channel.current.tangentOut), "f"(firstTangentWeight), "f"(channel.next.tangentIn),
                "f"(lastTangentWeight)
                : "fr0");
#endif
            return value;
        }
        if (SHENMUE_MOTION_CHANNEL_ABS(channel.next.timeSeconds - g_motionCurrentAnimTime) < FLT_MIN || !channel.keysRemaining)
            return channel.next.value;
        channel.current = channel.next;
        const std::uint16_t rawValue = *channel.frameCursor++;
        const float decoded = DecodeMotnFloat(rawValue);
        float value = channel.bias;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        value = SHENMUE_MOTION_CHANNEL_FMA(decoded, channel.scale, value);
#else
        asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+&f"(value) : "f"(decoded), "f"(channel.scale) : "fr0");
#endif
        channel.next.value = value;
        channel.next.tangentIn = channel.scale * DecodeMotnFloat(*channel.frameCursor++);
        channel.next.tangentOut = rawValue & 1U ? channel.scale * DecodeMotnFloat(*channel.frameCursor++) : channel.next.tangentIn;
        --channel.keysRemaining;
        channel.next.timeSeconds =
            channel.keysRemaining ? static_cast<float>(static_cast<std::int16_t>(*channel.frameCursor++)) * kMotionStreamFrameSeconds : g_motionMaximumAnimTime;
        channel.inverseDuration = 1.0f / (channel.next.timeSeconds - channel.current.timeSeconds);
    }
}
#endif

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MOTION_CHANNEL_FLOAT
#undef SHENMUE_MOTION_CHANNEL_ABS
#undef SHENMUE_MOTION_CHANNEL_FMA

#endif
