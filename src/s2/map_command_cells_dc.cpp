#include "s2/map_draw_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace s2 {

__attribute__((aligned(32))) Matrix4x4 mapCommandMatrix = {}, mapAlphaCommandMatrix = {};

// @verified - d
void CacheMapCommandMatrix() {
    matrix_stack::GetMatrix(&mapCommandMatrix);
}

// @verified - d
void CacheAlphaMapCommandMatrix() {
    matrix_stack::GetMatrix(&mapAlphaCommandMatrix);
}

// @verified - d
void DrawMapCommandCells(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells, MapModelClipRecord *const *routing,
    std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ) {
    CacheMapCommandMatrix();
    std::int32_t start = 0;
    if (cells->x == kMapScreenSpaceCell) {
        DrawMapScreenCommandRecords(routing, cells->count);
        ++cells;
        start = 1;
    }
    for (std::int32_t index = start; index < cellCount; ++index, ++cells) {
        const MapCellOffset *offset = visible;
        const std::int16_t x = static_cast<std::int16_t>(cells->x - cellX), z = static_cast<std::int16_t>(cells->z - cellZ);
        for (std::int32_t count = 0; count < visibleCount; ++offset, ++count) {
            if (x == offset->x && z == offset->z) {
                DrawMapCommandRecords(routing + cells->firstRecord, cells->count);
                break;
            }
        }
    }
}

// @verified - d
void DrawMapCommandCellsDepth(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells, MapModelClipRecord *const *routing,
    std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ) {
    CacheMapCommandMatrix();
    std::int32_t start = 0;
    if (cells->x == kMapScreenSpaceCell) {
        DrawMapScreenCommandRecords(routing, cells->count);
        ++cells;
        start = 1;
    }
    for (std::int32_t index = start; index < cellCount; ++index, ++cells) {
        const MapCellOffset *offset = visible;
        const std::int16_t x = static_cast<std::int16_t>(cells->x - cellX), z = static_cast<std::int16_t>(cells->z - cellZ);
        for (std::int32_t count = 0; count < visibleCount; ++offset, ++count) {
            if (x == offset->x && z == offset->z) {
                DrawMapDepthCommandRecords(routing + cells->firstRecord, cells->count);
                break;
            }
        }
    }
}

// @verified - d
void DrawMapCommandCellsAlpha(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells, MapModelClipRecord *const *routing,
    std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ, float alpha) {
    CacheAlphaMapCommandMatrix();
    std::int32_t start = 0;
    if (cells->x == kMapScreenSpaceCell) {
        DrawMapScreenAlphaCommandRecords(routing, cells->count, alpha);
        ++cells;
        start = 1;
    }
    for (std::int32_t index = start; index < cellCount; ++index, ++cells) {
        const MapCellOffset *offset = visible;
        const std::int16_t x = static_cast<std::int16_t>(cells->x - cellX), z = static_cast<std::int16_t>(cells->z - cellZ);
        for (std::int32_t count = 0; count < visibleCount; ++offset, ++count) {
            if (x == offset->x && z == offset->z) {
                DrawMapAlphaCommandRecords(routing + cells->firstRecord, cells->count, alpha);
                break;
            }
        }
    }
}

} // namespace s2
} // namespace shenmue

#endif
