#include "platform/dc/sdk.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <bit>
#endif
#include <cmath>

namespace shenmue { namespace dc { namespace sdk {

// @verified - d
float ReduceRadiansTangent(float angle, std::int32_t *quadrant) {
    const float scaled = angle / 1.5707963705062866f;
    const float rounded = (angle < 0.0f ? -0.5f : 0.5f) + scaled;
    std::int32_t fixed;
    asm("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(fixed) : "f"(rounded) : "fpul");
    *quadrant = fixed;
    const float turns = static_cast<float>(fixed);
    float reduced = angle - 1.57080078125f * turns;
    asm("fmac fr0,%1,%0" : "+f"(reduced) : "f"(4.454454938240815e-06f), "w"(turns));
    const float square = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor > 2; divisor -= 2)
        fraction = square / (static_cast<float>(divisor) - fraction);
    return reduced / (1.0f - fraction);
}

// @verified - d
float SinRadians(float angle) {
    std::int32_t quadrant;
    const float tangent = ReduceRadiansTangent(angle * 0.5f, &quadrant);
    const float numerator = tangent + tangent;
    float denominator = 1.0f;
    asm("fmac fr0,%1,%0" : "+f"(denominator) : "f"(tangent), "w"(tangent));
    const float result = numerator / denominator;
    return quadrant & 1 ? -result : result;
}

// @verified - d
float CosRadians(float angle) {
    return SinRadians(1.5707963705062866f - std::fabs(angle));
}

} } } // namespace shenmue::dc::sdk

#endif
