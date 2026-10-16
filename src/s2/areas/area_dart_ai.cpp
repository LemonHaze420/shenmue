#include "s2/debug.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/darts.hpp"
#include "s2/motion_work.hpp"
#include <bit>
#include <cmath>
namespace shenmue::s2::areas {
// @verified - p
std::uint32_t DartsSelectAiTargetBand(DartsEventWork *work) {
    std::int32_t band = static_cast<std::int32_t>(GetRandomNumberInRange(1) * 5) % 5;
    std::int32_t attempts = 0;
    while (attempts < 5 && work->usedBands[band]) {
        ++attempts;
        band = (band + 1) % 5;
    }
    work->usedBands[band] = 1;
    work->targetBand = DART_AI_BANDS[band];
    return std::bit_cast<std::uint32_t>(work->targetBand);
}
// @verified - p
std::int32_t DartsComputeAiImpactPoint(DartsEventWork *work) {
    const float maximum = static_cast<float>(work->entry->maximumRadius);
    const float minimum = static_cast<float>(static_cast<std::int8_t>(work->entry->minimumRadius));
    float outerRadius = work->targetBand * maximum;
    float innerRadius = work->targetBand * minimum;
    if (outerRadius < 10)
        outerRadius = maximum < 10 ? maximum : 10;
    if (innerRadius < 10 && minimum > 10)
        innerRadius = 10;
    else if (outerRadius < 10)
        outerRadius = maximum;
    work->wobblePhaseX = static_cast<std::uint16_t>(static_cast<std::int32_t>(GetRandomNumberInRange(360) * 8192 / 45));
    work->wobblePhaseY = static_cast<std::uint16_t>(static_cast<std::int32_t>(GetRandomNumberInRange(360) * 8192 / 45));
    bool accepted = false;
    bool finished = false;
    std::uint32_t iterations = 0;
    float impactX, impactY, distance;
    do {
        ++iterations;
        const std::uint32_t phase = work->wobblePhaseX;
        const float first = sin_fixed16(static_cast<std::int16_t>(22U * phase)) * 0.3f;
        const float second = sin_fixed16(static_cast<std::int16_t>(31U * phase)) * 0.37f + first;
        const float third = sin_fixed16(static_cast<std::int16_t>(103U * phase)) * 0.15f + second;
        impactY = (sin_fixed16(static_cast<std::int16_t>(147U * phase)) * 0.18f + third) * 0.171f;
        work->wobblePhaseX += 18U;
        impactX = sin_fixed16(static_cast<std::int16_t>(work->wobblePhaseY)) * 0.171f;
        work->wobblePhaseY += 218U;
        const float scaledY = impactY * DART_BOARD_Y_SCALE;
        distance = static_cast<float>(std::sqrt(static_cast<double>(impactX * impactX + scaledY * scaledY)));
        if (accepted)
            finished = true;
        else {
            accepted = static_cast<double>(outerRadius * 0.171f) / 100.0 > static_cast<double>(distance);
            if (accepted)
                accepted = static_cast<double>(distance) > static_cast<double>(innerRadius * 0.171f) / 100.0;
        }
    } while (!finished);

    EV_debug_log("DART\\eDART_et_darts.c", 1744, "area = %f, len = %f, x = %f, y = %f\n", outerRadius, distance, impactX, impactY);
    work->impactPosition = {impactX, impactY + 1.4174f, -0.068200007f};
    const std::int32_t count = static_cast<std::int32_t>(iterations);
    return count > 600 ? 600 : count;
}
} // namespace shenmue::s2::areas
