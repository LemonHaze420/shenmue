#include "shenmue.h"

#if SHENMUE_IS_S2

#include "s2/blur_figure_pose.hpp"
#include "s2/pause.hpp"
#include <bit>
#include <cfloat>
#include <cmath>

namespace shenmue { namespace s2 {

// @verified - pd
void figpUpdateBlurModelState(FigurePose *figure) {
    if (figure->startTime != 0 && figure->motionScale != 0 && (g_loadingFlag & 2U) == 0 && figure->mode != 0) {
        figure->counter = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(figure->counter) - 1U));
        if (figure->counter <= 0) {
            BlurWorkEntry &entry = figure->blurWork[static_cast<std::uint8_t>(figure->entryIndex)];
            entry.flags |= 1U;
            entry.mode = figure->mode;
            const std::uint8_t index = static_cast<std::uint8_t>(static_cast<std::uint8_t>(figure->entryIndex) + 1U);
            figure->entryIndex = index >= blurWorkEntryCount ? 0 : static_cast<std::int8_t>(index);
            figure->counter = static_cast<std::uint8_t>(figure->startTime);
            figure->maximumTime = figure->time / static_cast<float>(static_cast<std::uint8_t>(figure->motionScale));
        }
    } else {
        figure->counter = 0;
    }
    if ((figure->flags & 4U) != 0) {
        figure->mode = 0;
        for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
            BlurWorkEntry &entry = figure->blurWork[index];
            entry.flags &= ~1U;
            entry.time = 0.0f;
            entry.mode = 0;
        }
        figure->flags &= ~4U;
    }
}

// @verified - pd
bool LerpBlurMatrix(Matrix4x4 *destination, float alpha, const Matrix4x4 *first, const Matrix4x4 *second) {
    const float inverse = 1.0f - alpha;
    const Vector3 x = {SHEN_FIELD(x) inverse * first->rows[0].x + alpha * second->rows[0].x,
        SHEN_FIELD(y) inverse * first->rows[1].x + alpha * second->rows[1].x,
        SHEN_FIELD(z) inverse * first->rows[2].x + alpha * second->rows[2].x};
    destination->rows[0].x = x.x;
    destination->rows[1].x = x.y;
    destination->rows[2].x = x.z;
    const float xLength = std::sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
    if (std::fabs(xLength) > FLT_EPSILON) {
        destination->rows[0].x /= xLength;
        destination->rows[1].x /= xLength;
        destination->rows[2].x /= xLength;
    }
    const Vector3 blendedY = {SHEN_FIELD(x) inverse * first->rows[0].y + alpha * second->rows[0].y,
        SHEN_FIELD(y) inverse * first->rows[1].y + alpha * second->rows[1].y,
        SHEN_FIELD(z) inverse * first->rows[2].y + alpha * second->rows[2].y};
    const Vector3 z = {SHEN_FIELD(x) blendedY.z * x.y - blendedY.y * x.z, SHEN_FIELD(y) blendedY.x * x.z - blendedY.z * x.x, SHEN_FIELD(z) blendedY.y * x.x - blendedY.x * x.y};
    const Vector3 y = {SHEN_FIELD(x) z.y * x.z - z.z * x.y, SHEN_FIELD(y) z.z * x.x - z.x * x.z, SHEN_FIELD(z) z.x * x.y - z.y * x.x};
    const float yLength = std::sqrt(y.x * y.x + y.y * y.y + y.z * y.z);
    if (!(std::fabs(yLength) > FLT_EPSILON))
        return false;
    destination->rows[0].y = y.x / yLength;
    destination->rows[1].y = y.y / yLength;
    destination->rows[2].y = y.z / yLength;
    const float zLength = std::sqrt(z.x * z.x + z.y * z.y + z.z * z.z);
    if (!(std::fabs(zLength) > FLT_EPSILON))
        return false;
    destination->rows[0].z = z.x / zLength;
    destination->rows[1].z = z.y / zLength;
    destination->rows[2].z = z.z / zLength;
    destination->rows[3].x = (second->rows[3].x - first->rows[3].x) * alpha + first->rows[3].x;
    destination->rows[3].y = (second->rows[3].y - first->rows[3].y) * alpha + first->rows[3].y;
    destination->rows[3].z = (second->rows[3].z - first->rows[3].z) * alpha + first->rows[3].z;
    destination->rows[0].w = destination->rows[1].w = destination->rows[2].w = 0.0f;
    destination->rows[3].w = 1.0f;
    return true;
}

} } // namespace shenmue::s2

#endif
