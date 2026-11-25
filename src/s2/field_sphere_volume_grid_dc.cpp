#include "s2/field_event_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GatherFieldSphereVolumeCells(const Vector3 *position, float radius, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        *cells = 0;
        *++cells = -1;
        return 1;
    }
    const float lowX = position->x - radius, lowY = position->y - radius, lowZ = position->z - radius;
    const float highX = position->x + radius, highY = radius + position->y, highZ = radius + position->z;
    const float scale = grid->reciprocalCellSize;
    const float originX = grid->minimum.x, originY = grid->minimum.y, originZ = grid->minimum.z;
    const std::int32_t width = static_cast<std::int32_t>(grid->width);
    std::int32_t height = static_cast<std::int32_t>(grid->height);
    std::int32_t plane = static_cast<std::int32_t>(grid->depth);
    const std::int32_t firstX = static_cast<std::int32_t>((lowX - originX) * scale);
    std::int32_t firstLayer = static_cast<std::int32_t>((lowY - originY) * scale);
    std::int32_t firstRow = static_cast<std::int32_t>((lowZ - originZ) * scale);
    const std::int32_t lastX = static_cast<std::int32_t>((highX - originX) * scale);
    std::int32_t lastLayer = static_cast<std::int32_t>((highY - originY) * scale);
    std::int32_t lastRow = static_cast<std::int32_t>((highZ - originZ) * scale);
    plane *= width;
    firstRow *= width;
    lastRow *= width;
    height *= plane;
    firstLayer *= plane;
    lastLayer *= plane;
    std::int32_t count = 0;
    for (std::int32_t layer = firstLayer; layer <= lastLayer; layer += plane) {
        if (layer >= 0 && layer < height) {
            for (std::int32_t row = firstRow; row <= lastRow; row += width) {
                if (row >= 0 && row < plane) {
                    for (std::int32_t x = firstX; x <= lastX; ++x) {
                        if (x >= 0 && x < width) {
                            *cells++ = static_cast<std::int16_t>(x + row + layer);
                            ++count;
                        }
                    }
                }
            }
        }
    }
    *cells = -1;
    return count;
}
#else
std::int32_t GatherFieldSphereVolumeCells(const Vector3 *position, float radius, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        cells[0] = 0;
        cells[1] = -1;
        return 1;
    }
    const float minimumX = ((position->x - radius) - grid->minimum.x) * grid->reciprocalCellSize;
    const float minimumY = ((position->y - radius) - grid->minimum.y) * grid->reciprocalCellSize;
    const float minimumZ = ((position->z - radius) - grid->minimum.z) * grid->reciprocalCellSize;
    const float maximumX = ((position->x + radius) - grid->minimum.x) * grid->reciprocalCellSize;
    const float maximumY = ((radius + position->y) - grid->minimum.y) * grid->reciprocalCellSize;
    const float maximumZ = ((radius + position->z) - grid->minimum.z) * grid->reciprocalCellSize;
    std::int32_t width, height, depth, firstX, firstY, firstZ, lastX, lastY, lastZ;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    width = static_cast<std::int32_t>(grid->width);
    height = static_cast<std::int32_t>(grid->height);
    depth = static_cast<std::int32_t>(grid->depth);
    firstX = static_cast<std::int32_t>(minimumX);
    firstY = static_cast<std::int32_t>(minimumY);
    firstZ = static_cast<std::int32_t>(minimumZ);
    lastX = static_cast<std::int32_t>(maximumX);
    lastY = static_cast<std::int32_t>(maximumY);
    lastZ = static_cast<std::int32_t>(maximumZ);
#else
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(width) : "f"(grid->width) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(height) : "f"(grid->height) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(depth) : "f"(grid->depth) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(firstX) : "f"(minimumX) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(firstY) : "f"(minimumY) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(firstZ) : "f"(minimumZ) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(lastX) : "f"(maximumX) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(lastY) : "f"(maximumY) : "fpul");
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(lastZ) : "f"(maximumZ) : "fpul");
#endif
    const std::uint32_t plane = static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(depth);
    const std::int32_t maximumLayer = static_cast<std::int32_t>(static_cast<std::uint32_t>(height) * plane);
    const std::int32_t lastLayer = static_cast<std::int32_t>(static_cast<std::uint32_t>(lastY) * plane);
    const std::int32_t lastRow = static_cast<std::int32_t>(static_cast<std::uint32_t>(lastZ) * static_cast<std::uint32_t>(width));
    std::uint32_t count = 0;
    for (std::int32_t layer = static_cast<std::int32_t>(static_cast<std::uint32_t>(firstY) * plane); layer <= lastLayer;
        layer = static_cast<std::int32_t>(static_cast<std::uint32_t>(layer) + plane)) {
        if (layer < 0 || layer >= maximumLayer)
            continue;
        for (std::int32_t row = static_cast<std::int32_t>(static_cast<std::uint32_t>(firstZ) * static_cast<std::uint32_t>(width)); row <= lastRow;
            row = static_cast<std::int32_t>(static_cast<std::uint32_t>(row) + static_cast<std::uint32_t>(width))) {
            if (row < 0 || row >= static_cast<std::int32_t>(plane))
                continue;
            for (std::int32_t x = firstX; x <= lastX; x = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + 1U)) {
                if (x >= 0 && x < width) {
                    *cells++ = static_cast<std::int16_t>(static_cast<std::uint32_t>(x) + static_cast<std::uint32_t>(row) + static_cast<std::uint32_t>(layer));
                    ++count;
                }
            }
        }
    }
    *cells = -1;
    return static_cast<std::int32_t>(count);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
