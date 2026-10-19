#include "s1/motion_internal.hpp"
#include <shendk/files/animation/motn_reader.h>

namespace shenmue::s1::motion_detail {
float HermiteEase(const MotnFCVKey &first, const MotnFCVKey &second, float currentTime);

float HermiteTangent(const MotnFCVKey &first, const MotnFCVKey &second, float currentTime);

std::uint8_t KeyTypeOf(const MotnKeyframeChannel &channel, std::uint16_t index);

std::uint32_t KeyFrameTableAt(const MotnKeyframeChannel &channel, std::uint32_t index);

MotnFCVKey ConstructKey(const MotnKeyRef &key);

float HermiteSlope(const MotnFCVKey &first, const MotnFCVKey &second, float time);

float F16ToF32(std::int16_t value) {
#if 0
    if (value == 0)
        return 0.0f;
    std::uint32_t expanded = static_cast<std::uint16_t>(value);
    if (value < 0)
        expanded = (expanded & 0x7fffU) | 0xfffc0000U;
    return std::bit_cast<float>((expanded << 13U) + 0x38000000U);
#else
    return shendk::DecodeMotnFloat(static_cast<std::uint16_t>(value));
#endif
}

float HermiteEase(const MotnFCVKey &first, const MotnFCVKey &second, float currentTime) {
    const float duration = second.time - first.time;
    const float amount = (currentTime - first.time) / duration;
    const float amount2 = amount * amount;
    const float amount3 = amount2 * amount;
    const float firstValue = F16ToF32(first.value);
    const float secondValue = F16ToF32(second.value);
    const float firstSlope = F16ToF32(first.rightSlope);
    const float secondSlope = F16ToF32(second.leftSlope);
    return (((amount3 - (amount2 + amount2)) + amount) * firstSlope + ((amount3 - amount2) * secondSlope)) * duration +
        ((((amount3 + amount3) - amount2 * 3.0f) + 1.0f) * firstValue + ((amount2 * 3.0f - (amount3 + amount3)) * secondValue));
}

float HermiteTangent(const MotnFCVKey &first, const MotnFCVKey &second, float currentTime) {
    const float inverseDuration = 1.0f / (second.time - first.time);
    const float elapsed = currentTime - first.time;
    const float elapsedSquared = elapsed * elapsed;
    const float inverseSquared = inverseDuration * inverseDuration;
    const float firstValue = F16ToF32(first.value);
    const float secondValue = F16ToF32(second.value);
    const float firstSlope = F16ToF32(first.rightSlope);
    const float secondSlope = F16ToF32(second.leftSlope);
    const float cubicDerivative = elapsedSquared * 6.0f * inverseSquared * inverseDuration;
    const float quadraticDerivative = elapsed * 6.0f * inverseSquared;
    const float doubledNormalizedTime = (elapsed + elapsed) * inverseDuration;
    const float squaredNormalizedTime = elapsedSquared * 3.0f * inverseSquared;
    return ((cubicDerivative - quadraticDerivative) * firstValue + (quadraticDerivative - cubicDerivative) * secondValue) +
        ((squaredNormalizedTime - doubledNormalizedTime - doubledNormalizedTime + 1.0f) * firstSlope) +
        ((squaredNormalizedTime - doubledNormalizedTime) * secondSlope);
}

std::int16_t Fixed16(float value) {
    return static_cast<std::int16_t>(static_cast<std::int32_t>(value));
}

std::uint8_t KeyTypeOf(const MotnKeyframeChannel &channel, std::uint16_t index) {
    return static_cast<std::uint8_t>((channel.block4Data[index >> 2U] >> (6U - 2U * (index & 3U))) & 3U);
}

std::uint32_t KeyFrameTableAt(const MotnKeyframeChannel &channel, std::uint32_t index) {
    return channel.sizeData ? channel.sizeData[index] : static_cast<const std::uint8_t *>(channel.block3Data)[index];
}

// MOTN_ConstructFCVKey.
MotnFCVKey ConstructKey(const MotnKeyRef &key) {
    MotnFCVKey result{};
    result.time = key.time;
    if (!key.data)
        return result;
    switch (key.type) {
    case 1:
        result.value = static_cast<std::int16_t>(key.data[0]);
        break;
    case 2:
        result.leftSlope = static_cast<std::int16_t>(key.data[0]);
        result.rightSlope = result.leftSlope;
        result.value = static_cast<std::int16_t>(key.data[1]);
        break;
    case 3:
        result.leftSlope = static_cast<std::int16_t>(key.data[0]);
        result.rightSlope = static_cast<std::int16_t>(key.data[1]);
        result.value = static_cast<std::int16_t>(key.data[2]);
        break;
    default:
        break;
    }
    return result;
}

// MOTN_readAndConvertData: the reverse-playback FCV lookup. It caches the key after the sample time and walks backwards.
float MOTN_readAndConvertData(MotnKeyframeChannel &channel, float currentTime) {
    const std::uint16_t cached = static_cast<std::uint16_t>(channel.keyframeCount);
    const std::uint16_t cachedTable = cached ? static_cast<std::uint16_t>(cached - 1U) : 0;
    const float cachedTime = cached ? static_cast<float>(KeyFrameTableAt(channel, cachedTable)) * kMotnFrameTimeF : 0.0f;
    if (currentTime >= cachedTime)
        channel.block5Cursor = nullptr;
    const std::uint16_t *data = channel.block5Cursor;
    std::uint16_t index = cached;
    if (!data) {
        if (channel.present == 2) {
            index = 1;
            channel.keyframeCount = 1;
            data = channel.block5Begin + (channel.block4Data[0] >> 6U);
        } else {
            index = static_cast<std::uint16_t>(channel.present - 1U);
            channel.keyframeCount = static_cast<std::int16_t>(index);
            data = channel.block5End - KeyTypeOf(channel, index);
        }
        channel.block5Cursor = data;
    }
    MotnKeyRef first{};
    MotnKeyRef second{data, 0.0f, KeyTypeOf(channel, index)};
    if (static_cast<std::uint16_t>(channel.present - 1U) == index) {
        second.time = static_cast<float>(g_motnFrameCount - 1) * kMotnFrameTimeF;
    } else {
        if (!index)
            return F16ToF32(ConstructKey(first).value);
        second.time = static_cast<float>(KeyFrameTableAt(channel, index - 1U)) * kMotnFrameTimeF;
    }
    if (!index)
        return F16ToF32(ConstructKey(first).value);
    --index;
    first.type = KeyTypeOf(channel, index);
    first.data = data - first.type;
    first.time = index ? static_cast<float>(KeyFrameTableAt(channel, index - 1U)) * kMotnFrameTimeF : 0.0f;
    if (currentTime >= static_cast<float>(g_motnFrameCount - 1) * kMotnFrameTimeF)
        return F16ToF32(ConstructKey(second).value);
    while (index && currentTime < first.time) {
        second = first;
        channel.keyframeCount = static_cast<std::int16_t>(index);
        channel.block5Cursor = first.data;
        --index;
        first.type = KeyTypeOf(channel, index);
        first.data -= first.type;
        if (!index) {
            first.time = 0.0f;
            break;
        }
        first.time = static_cast<float>(KeyFrameTableAt(channel, index - 1U)) * kMotnFrameTimeF;
    }
    if (std::fabs(first.time - currentTime) < 0x1p-23f)
        return F16ToF32(ConstructKey(first).value);
    if (currentTime <= first.time || second.time <= currentTime)
        return F16ToF32(ConstructKey(first).value);
    return HermiteEase(ConstructKey(first), ConstructKey(second), currentTime);
}
}

namespace shenmue::s1 {



float MOTN_CalculateKeyframeForTime(MotnKeyframeChannel &channel, float currentTime, std::int32_t frameCount) {
    const std::uint16_t cachedKey = channel.keyframeCount ? static_cast<std::uint16_t>(channel.keyframeCount - 1) : 0;
    MotnKeyRef first{.time = static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, cachedKey))) * kMotnFrameTimeF};
    if (first.time > currentTime) {
        channel.block5Cursor = nullptr;
        channel.keyframeCount = 0;
    }
    const std::uint16_t *data = channel.block5Begin;
    std::uint16_t index = 0;
    if (channel.block5Cursor) {
        index = static_cast<std::uint16_t>(channel.keyframeCount);
        data = channel.block5Cursor;
    }
    const std::uint16_t last = static_cast<std::uint16_t>(channel.present - 1);
    MotnKeyRef second{.data = data, .time = 0.0f, .type = KeyTypeOf(channel, index)};
    if (last == index)
        second.time = static_cast<float>(frameCount - 1) * kMotnFrameTimeF;
    else if (index)
        second.time = static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, index - 1U))) * kMotnFrameTimeF;
    const std::uint16_t *cursor = data + second.type;
    std::uint16_t next = static_cast<std::uint16_t>(index + 1);
    for (; next < last; cursor += second.type) {
        first = second;
        second.data = cursor;
        second.type = KeyTypeOf(channel, next);
        second.time = static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, next - 1U))) * kMotnFrameTimeF;
        if (second.time > currentTime)
            break;
        channel.keyframeCount = static_cast<std::int16_t>(next++);
        channel.block5Cursor = cursor;
    }
    if (next == last) {
        first = second;
        second.data = cursor;
        second.time = static_cast<float>(frameCount - 1) * kMotnFrameTimeF;
        second.type = KeyTypeOf(channel, next);
    }
    if (std::fabs(second.time - currentTime) < 0x1p-23f || second.time <= currentTime)
        return F16ToF32(ConstructKey(second).value);
    return HermiteEase(ConstructKey(first), ConstructKey(second), currentTime);
}
}

namespace shenmue::s1::motion_detail {

// InterpolateHermiteKeyframes_FP16_0: the derivative of the hermite segment at time.
float HermiteSlope(const MotnFCVKey &first, const MotnFCVKey &second, float time) {
    const float firstValue = F16ToF32(first.value);
    const float secondValue = F16ToF32(second.value);
    const float firstSlope = F16ToF32(first.rightSlope);
    const float secondSlope = F16ToF32(second.leftSlope);
    const float delta = time - first.time;
    const auto inverse = static_cast<float>(1.0 / static_cast<double>(second.time - first.time));
    const float inverseSquared = inverse * inverse;
    const double deltaSquared = static_cast<double>(delta * delta);
    const auto cubicTerm = static_cast<float>(deltaSquared * 6.0 * static_cast<double>(inverseSquared) * static_cast<double>(inverse));
    const auto linearTerm = static_cast<float>(static_cast<double>(delta) * 6.0 * static_cast<double>(inverseSquared));
    const auto doubled = static_cast<float>((static_cast<double>(delta) + static_cast<double>(delta)) * static_cast<double>(inverse));
    const auto squaredTerm = static_cast<float>(deltaSquared * 3.0 * static_cast<double>(inverseSquared));
    const float secondShape = squaredTerm - doubled;
    return (((cubicTerm - linearTerm) * firstValue + (linearTerm - cubicTerm) * secondValue) + ((secondShape - doubled) + 1.0f) * firstSlope) +
        secondShape * secondSlope;
}

// InterpolateHermiteKeyframes_FP16_1 / InterpolateHermiteKeyframes_FP16: the slope of an FCV channel at time, using the forward
// cached key lookup of MOTN_CalculateKeyframeForTime.
float MOTN_EvaluateKeyframeSlope(MotnKeyframeChannel &channel, float time) {
    const std::uint16_t cachedKey = channel.keyframeCount ? static_cast<std::uint16_t>(channel.keyframeCount - 1) : 0;
    MotnKeySpan span{};
    span.first.time = static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, cachedKey))) * kMotnFrameTimeF;
    if (span.first.time > time) {
        channel.block5Cursor = nullptr;
        channel.keyframeCount = 0;
    }
    const std::uint16_t *data = channel.block5Begin;
    std::uint16_t index = 0;
    if (channel.block5Cursor) {
        index = static_cast<std::uint16_t>(channel.keyframeCount);
        data = channel.block5Cursor;
    }
    const std::int32_t frameCount = g_motnFrameCount;
    const auto last = static_cast<std::uint16_t>(channel.present - 1);
    if (last == index)
        span.second.time = static_cast<float>(frameCount - 1) * kMotnFrameTimeF;
    else
        span.second.time = index ? static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, index - 1U))) * kMotnFrameTimeF : 0.0f;
    span.second.data = data;
    span.second.type = KeyTypeOf(channel, index);
    const std::uint16_t *cursor = data + span.second.type;
    std::uint16_t next = static_cast<std::uint16_t>(index + 1);
    for (; next < last; cursor += span.second.type) {
        span.first = span.second;
        span.second.data = cursor;
        span.second.type = KeyTypeOf(channel, next);
        span.second.time = static_cast<float>(static_cast<std::int32_t>(KeyFrameTableAt(channel, next - 1U))) * kMotnFrameTimeF;
        if (span.second.time > time)
            break;
        channel.keyframeCount = static_cast<std::int16_t>(next++);
        channel.block5Cursor = cursor;
    }
    if (next == last) {
        span.first = span.second;
        span.second.data = cursor;
        span.second.time = static_cast<float>(frameCount - 1) * kMotnFrameTimeF;
        span.second.type = KeyTypeOf(channel, next);
    }
    std::int32_t mode = 0;
    if (std::fabs(span.second.time - time) >= 0x1p-23f) {
        if (span.second.time <= time) {
            mode = 3;
            span.first = span.second;
        } else {
            mode = 2;
        }
    }
    switch (mode) {
    case 0: {
        const std::uint16_t *key = span.second.data;
        std::uint16_t left = 0;
        std::uint16_t right = 0;
        if (span.second.type == 2) {
            left = key[0];
            right = key[0];
        } else if (span.second.type == 3) {
            left = key[0];
            right = key[1];
        }
        return static_cast<float>(static_cast<double>(F16ToF32(static_cast<std::int16_t>(right)) + F16ToF32(static_cast<std::int16_t>(left))) * 0.5);
    }
    case 2:
        return HermiteSlope(ConstructKey(span.first), ConstructKey(span.second), time);
    default:
        if (span.first.type == 2 || span.first.type == 3)
            return F16ToF32(static_cast<std::int16_t>(span.first.data[0]));
        return 0.0f;
    }
}
}
