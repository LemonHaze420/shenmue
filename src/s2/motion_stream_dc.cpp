#include "s2/motion_stream_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/motion_rig_data_dc.hpp"

#define SHENMUE_MOTION_STREAM_NULL 0
namespace shenmue {
namespace s2 {
namespace {
struct MotionStreamKey {
    std::uint16_t value, tangentIn, tangentOut;
    float timeSeconds;
};
} // namespace

float g_motionCurrentAnimTime = 0.0f, g_motionMaximumAnimTime = 0.0f, g_motionPreviousAnimTime = 0.0f;
std::uint32_t g_motionInterpolationType = 0, g_motionInitFlags = 0;
std::uint16_t g_motionAnimFlags = 0;
const std::uint8_t *g_motionKeyCounts = SHENMUE_MOTION_STREAM_NULL;
const std::uint16_t *g_motionWideKeyCounts = SHENMUE_MOTION_STREAM_NULL, *g_motionKeyData = SHENMUE_MOTION_STREAM_NULL;

// @verified - d
void UpdateAnimRate(MotionRigChannel &channel, float scale, float bias) {
    channel.scale = scale;
    channel.bias = bias;
}

// @verified - d
void ReadMotionKeyBlock(MotionRigChannel &channel) {
    std::uint32_t count;
    if (g_motionInitFlags & motionInitStream) {
        if (g_motionAnimFlags & motionWideKeyCounts)
            channel.keyCount = count = *g_motionWideKeyCounts++ + 2;
        else
            channel.keyCount = count = *g_motionKeyCounts++ + 2;
        channel.keyBlock = g_motionKeyData;
    } else {
        count = channel.keyCount;
        g_motionKeyData = channel.keyBlock;
    }
    MotionStreamKey first, second;
    first.value = *g_motionKeyData++;
    first.tangentIn = *g_motionKeyData++;
    if (first.value & 1U)
        first.tangentOut = *g_motionKeyData++;
    else
        first.tangentOut = first.tangentIn;
    first.timeSeconds = 0.0f;
    --count;
    second.value = *g_motionKeyData++;
    second.tangentIn = *g_motionKeyData++;
    if (second.value & 1U)
        second.tangentOut = *g_motionKeyData++;
    else
        second.tangentOut = second.tangentIn;
    if (--count)
        second.timeSeconds = static_cast<float>(static_cast<std::int16_t>(*g_motionKeyData++)) * kMotionStreamFrameSeconds;
    else
        second.timeSeconds = g_motionMaximumAnimTime;
    while (count && second.timeSeconds < g_motionPreviousAnimTime) {
        first = second;
        second.value = *g_motionKeyData++;
        second.tangentIn = *g_motionKeyData++;
        if (second.value & 1U)
            second.tangentOut = *g_motionKeyData++;
        else
            second.tangentOut = second.tangentIn;
        if (--count)
            second.timeSeconds = static_cast<float>(static_cast<std::int16_t>(*g_motionKeyData++)) * kMotionStreamFrameSeconds;
        else
            second.timeSeconds = g_motionMaximumAnimTime;
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    channel.current.value = DecodeMotnFloat(first.value) * channel.scale + channel.bias;
#else
    float value = DecodeMotnFloat(first.value);
    float scaled = channel.bias;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+&f"(scaled) : "f"(value), "f"(channel.scale) : "fr0");
    channel.current.value = scaled;
#endif
    channel.current.tangentIn = DecodeMotnFloat(first.tangentIn) * channel.scale;
    if (first.value & 1U)
        channel.current.tangentOut = DecodeMotnFloat(first.tangentOut) * channel.scale;
    else
        channel.current.tangentOut = channel.current.tangentIn;
    channel.current.timeSeconds = first.timeSeconds;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    channel.next.value = DecodeMotnFloat(second.value) * channel.scale + channel.bias;
#else
    value = DecodeMotnFloat(second.value);
    scaled = channel.bias;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+&f"(scaled) : "f"(value), "f"(channel.scale) : "fr0");
    channel.next.value = scaled;
#endif
    channel.next.tangentIn = DecodeMotnFloat(second.tangentIn) * channel.scale;
    if (second.value & 1U)
        channel.next.tangentOut = DecodeMotnFloat(second.tangentOut) * channel.scale;
    else
        channel.next.tangentOut = channel.next.tangentIn;
    channel.next.timeSeconds = second.timeSeconds;
    channel.inverseDuration = 1.0f / (channel.next.timeSeconds - channel.current.timeSeconds);
    channel.keysRemaining = count;
    channel.frameCursor = g_motionKeyData;
    if (g_motionInitFlags & motionInitStream) {
        while (count-- > 0U) {
            const std::uint16_t flags = *g_motionKeyData++;
            ++g_motionKeyData;
            if (flags & 1U)
                ++g_motionKeyData;
            if (count)
                ++g_motionKeyData;
        }
    }
}

// @verified - d
void SkipMotionKeyBlock() {
    if (!(g_motionInitFlags & motionInitStream))
        return;
    std::uint32_t count = g_motionAnimFlags & motionWideKeyCounts ? *g_motionWideKeyCounts++ : *g_motionKeyCounts++;
    count += 2;
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::uint16_t flags = *g_motionKeyData;
        g_motionKeyData += 2;
        if (flags & 1U)
            ++g_motionKeyData;
        if (index && index + 1U < count)
            ++g_motionKeyData;
    }
}

// @verified - d
void SetMotionStreamFlags(std::int32_t channels, std::uint32_t initFlags, std::int32_t animFlags) {
    g_motionInitFlags = initFlags;
    g_motionAnimFlags = animFlags;
    while (static_cast<std::uint32_t>(channels--) > 0U)
        SkipMotionKeyBlock();
}

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MOTION_STREAM_NULL

#endif
