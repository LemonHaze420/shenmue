#include "s2/field_sphere_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t GatherFieldSphereGridCells(const Vector3 *position, float radius, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
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
#else
    if (!grid) {
        cells[0] = 0;
        cells[1] = -1;
        return 1;
    }
    const float firstX = ((position->x - radius) - grid->minimum.x) * grid->reciprocalCellSize;
    const float firstZ = ((position->z - radius) - grid->minimum.z) * grid->reciprocalCellSize;
    const float lastX = ((radius + position->x) - grid->minimum.x) * grid->reciprocalCellSize;
    const float lastZ = ((radius + position->z) - grid->minimum.z) * grid->reciprocalCellSize;
    const float level = (position->y - grid->minimum.y) * grid->reciprocalCellSize;
    std::int32_t width, height, depth, minimumX, minimumZ, maximumX, maximumZ, y;
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(width) : "f"(grid->width) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(height) : "f"(grid->height) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(depth) : "f"(grid->depth) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(minimumX) : "f"(firstX) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(y) : "f"(level) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(minimumZ) : "f"(firstZ) : "fpul");
#endif
    if (y < 0 || y >= height) {
        cells[0] = -1;
        return 0;
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    maximumX = static_cast<std::int32_t>((highX - originX) * reciprocal);
    maximumZ = static_cast<std::int32_t>((highZ - originZ) * reciprocal);
#else
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(maximumX) : "f"(lastX) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(maximumZ) : "f"(lastZ) : "fpul");
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const std::int32_t plane = depth * width;
    std::int32_t row = minimumZ * width;
    const std::int32_t lastRow = maximumZ * width;
    const std::int32_t layer = y * plane;
#else
    const std::uint32_t plane = static_cast<std::uint32_t>(depth) * static_cast<std::uint32_t>(width);
    const std::uint32_t layer = static_cast<std::uint32_t>(y) * plane;
    const std::int32_t lastRow = static_cast<std::int32_t>(static_cast<std::uint32_t>(maximumZ) * static_cast<std::uint32_t>(width));
    std::int32_t row = static_cast<std::int32_t>(static_cast<std::uint32_t>(minimumZ) * static_cast<std::uint32_t>(width));
#endif
    std::uint32_t count = 0;
    for (; row <= lastRow; row = static_cast<std::int32_t>(static_cast<std::uint32_t>(row) + static_cast<std::uint32_t>(width))) {
        if (row < 0 || row >= static_cast<std::int32_t>(plane))
            continue;
        for (std::int32_t x = minimumX; x <= maximumX; x = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + 1U)) {
            if (x >= 0 && x < width) {
                *cells++ = static_cast<std::int16_t>(static_cast<std::uint32_t>(x) + static_cast<std::uint32_t>(row) + layer);
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
