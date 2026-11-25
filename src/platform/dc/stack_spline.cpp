#include "platform/dc/stack_spline.hpp"

#if SHENMUE_IS_DC

namespace shenmue { namespace dc {

namespace {
// @verified - d
std::int32_t TruncateSplineAtRepeatedTime(std::int32_t count, const float *times) {
    for (std::int32_t index = 0; index < count - 1; ++index) {
        if (times[index] == times[index + 1])
            return index + 1;
    }
    return count;
}

// @verified - d
void PrepareSplineSpans(std::int32_t count, const float *times, const float *values, float *spans, float *doubledSpans, float *deltas) {
    if (count == 2) {
        spans[0] = times[1] - times[0];
        return;
    }
    for (std::int32_t index = 0; index < count - 2; ++index) {
        spans[index] = times[index + 1] - times[index];
        const float span = times[index + 2] - times[index];
        doubledSpans[index] = span + span;
        deltas[index] = values[index + 1] - values[index];
    }
    spans[count - 2] = times[count - 1] - times[count - 2];
    deltas[count - 2] = values[count - 1] - values[count - 2];
}

// @verified - d
void SolveSplineFirstDerivatives(std::int32_t count, const float *spans, const float *doubledSpans, float *diagonal, float *factors, const float *deltas,
    float *rightHandSide, float *derivatives) {
    const std::int32_t interior = count - 2;
    diagonal[0] = doubledSpans[0];
    for (std::int32_t row = 1; row < interior; ++row) {
        factors[row] = spans[row + 1] / diagonal[row - 1];
        diagonal[row] = doubledSpans[row] - factors[row] * spans[row - 1];
    }
    float sum = (spans[0] * deltas[1]) / spans[1] + (spans[1] * deltas[0]) / spans[0];
    rightHandSide[0] = (sum + sum) + sum;
    for (std::int32_t row = 1; row < interior; ++row) {
        sum = (spans[row + 1] * deltas[row]) / spans[row] + (spans[row] * deltas[row + 1]) / spans[row + 1];
        rightHandSide[row] = ((sum + sum) + sum) - factors[row] * rightHandSide[row - 1];
    }
    derivatives[interior] = rightHandSide[interior - 1] / diagonal[interior - 1];
    for (std::int32_t row = interior - 2; row >= 0; --row)
        derivatives[row + 1] = (rightHandSide[row] - derivatives[row + 2] * spans[row]) / diagonal[row];
}

// @verified - d
void StoreSplineBezierCoefficients(float *coefficients, std::int32_t count, const float *values, const float *spans, const float *derivatives) {
    coefficients[0] = 0.0f;
    for (std::int32_t key = 1; key < count; ++key) {
        const float first = (values[key - 1] + values[key - 1]) + values[key - 1];
        const float second = (values[key] + values[key]) + values[key];
        coefficients[2 * key] = derivatives[key - 1] * spans[key - 1] + first;
        coefficients[2 * key + 1] = second - derivatives[key] * spans[key - 1];
    }
}
} // namespace

// @verified - d
std::int32_t AllocSPLI(float *coefficients, std::int32_t count, const float *times, const float *values, const float *slopes) {
    count = TruncateSplineAtRepeatedTime(count, times);
    if (count >= kStackSplineKeyLimit)
        asm volatile("trapa #0x31" : : : "memory");
    if (count < 2)
        return count;
    float *const scratch = static_cast<float *>(__builtin_alloca(static_cast<std::uint32_t>(count) * 28U));
    float *const spans = scratch;
    float *const doubledSpans = spans + count;
    float *const diagonal = doubledSpans + count;
    float *const factors = diagonal + count;
    float *const deltas = factors + count;
    float *const rightHandSide = deltas + count;
    float *const derivatives = rightHandSide + count;
    PrepareSplineSpans(count, times, values, spans, doubledSpans, deltas);
    derivatives[0] = slopes ? slopes[0] : 0.0f;
    derivatives[count - 1] = slopes ? slopes[count - 1] : 0.0f;
    if (count >= 3)
        SolveSplineFirstDerivatives(count, spans, doubledSpans, diagonal, factors, deltas, rightHandSide, derivatives);
    StoreSplineBezierCoefficients(coefficients, count, values, spans, derivatives);
    return count;
}

} } // namespace shenmue::dc

#endif
