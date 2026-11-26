#include "s2/field_sphere_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t GatherFieldSphereGridCells(const Vector3 *position, float radius, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        *cells++ = 0;
        *cells = -1;
        return 1;
    }
    const float lowX = position->x - radius, positionY = position->y, lowZ = position->z - radius;
    const float highX = radius + position->x, highZ = radius + position->z;
    const float reciprocal = grid->reciprocalCellSize;
    const float originX = grid->minimum.x, originY = grid->minimum.y, originZ = grid->minimum.z;
    std::int32_t width, height, depth, minimumX, minimumZ, maximumX, maximumZ, y;
    width = static_cast<std::int32_t>(grid->width);
    height = static_cast<std::int32_t>(grid->height);
    depth = static_cast<std::int32_t>(grid->depth);
    minimumX = static_cast<std::int32_t>((lowX - originX) * reciprocal);
    y = static_cast<std::int32_t>((positionY - originY) * reciprocal);
    minimumZ = static_cast<std::int32_t>((lowZ - originZ) * reciprocal);
    if (y < 0 || y >= height) {
        cells[0] = -1;
        return 0;
    }
    maximumX = static_cast<std::int32_t>((highX - originX) * reciprocal);
    maximumZ = static_cast<std::int32_t>((highZ - originZ) * reciprocal);
    depth *= width;
    minimumZ *= width;
    maximumZ *= width;
    const std::int32_t layer = y * depth;
    std::uint32_t count = 0;
    for (std::int32_t row = minimumZ; row <= maximumZ; row += width) {
        if (row < 0 || row >= depth)
            continue;
        for (std::int32_t x = minimumX; x <= maximumX; ++x) {
            if (x >= 0 && x < width) {
                *cells++ = static_cast<std::int16_t>(x + row + layer);
                ++count;
            }
        }
    }
    *cells = -1;
    return static_cast<std::int32_t>(count);
}

} // namespace s2
} // namespace shenmue

#endif
