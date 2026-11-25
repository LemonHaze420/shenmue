#include "platform/dc/sdk.hpp"

#if SHENMUE_IS_DC

#include "platform/dc/ninja.hpp"
#include <cmath>

namespace shenmue {
namespace dc {
namespace sdk {

// @verified - d
void ScaleFogIntensity(float *table, float intensity) {
    for (std::size_t index = 0; index < 128; ++index) {
        float &value = table[index];
        value *= intensity;
        if (value > 1.0f)
            value = 1.0f;
        if (value < 0.0f)
            value = 0.0f;
    }
}

// @verified - d
void ComputeFogAttenuation(float *table, float nearDistance, float farDistance) {
    std::uint32_t density = 0;
    if (nearDistance > 65535.0f) {
        density = 0x8010;
    } else {
        int exponent;
        float mantissa = static_cast<float>(std::frexp(static_cast<double>(-farDistance), &exponent));
        --exponent;
        mantissa += mantissa;
        if (!(mantissa - 1.0f < 0.0f)) {
            mantissa -= 1.0f;
            density |= 0x8000;
        }
        if (!(mantissa - 0.5f < 0.0f)) {
            mantissa -= 0.5f;
            density |= 0x4000;
        }
        if (!(mantissa - 0.25f < 0.0f)) {
            mantissa -= 0.25f;
            density |= 0x2000;
        }
        if (!(mantissa - 0.125f < 0.0f)) {
            mantissa -= 0.125f;
            density |= 0x1000;
        }
        if (!(mantissa - 0.0625f < 0.0f)) {
            mantissa -= 0.0625f;
            density |= 0x800;
        }
        if (!(mantissa - 0.03125f < 0.0f)) {
            mantissa -= 0.03125f;
            density |= 0x400;
        }
        if (!(mantissa - 0.015625f < 0.0f)) {
            mantissa -= 0.015625f;
            density |= 0x200;
        }
        if (!(mantissa - 0.0078125f < 0.0f))
            density |= 0x100;
        density |= static_cast<std::uint32_t>(exponent);
    }
    njSetFogDensity(density);
    std::int32_t index;
    if (!(-nearDistance > -farDistance / 256.0f)) {
        for (index = 0; index < 128; ++index)
            table[index] = (128.0f - static_cast<float>(index)) / 128.0f;
    } else {
        int exponent;
        const float mantissa = static_cast<float>(std::frexp(static_cast<double>(farDistance / nearDistance), &exponent));
        const std::int32_t fraction = static_cast<std::int32_t>((mantissa + mantissa) * 16.0f) - 16;
        std::int32_t count = (16 * (exponent - 1)) | fraction;
#if SHENMUE_IS_S2
        if (count < 1)
            count = 1;
        if (count > 128)
            count = 128;
#endif
        for (index = 0; index < count; ++index)
            table[index] = (static_cast<float>(count) - static_cast<float>(index)) / static_cast<float>(count);
        for (index = count; index < 128; ++index)
            table[index] = 0.0f;
    }
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
