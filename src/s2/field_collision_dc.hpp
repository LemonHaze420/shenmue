#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"

#define SHENMUE_FIELD_DATA_LAYOUT_JOIN_(a, b) a##b
#define SHENMUE_FIELD_DATA_LAYOUT_JOIN(a, b) SHENMUE_FIELD_DATA_LAYOUT_JOIN_(a, b)
#define SHENMUE_FIELD_DATA_LAYOUT(condition) typedef char SHENMUE_FIELD_DATA_LAYOUT_JOIN(FieldDataLayout_, __LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

void FieldDebugNullsub();

struct FieldDataPolygon {
    std::uint16_t flags;
    std::uint16_t dataGroup;
    std::int16_t normalIndex;
    std::int16_t planeIndex;
    std::int16_t vertexIndices[4];
};
struct FieldDataGridConfig {
    Vector3 minimum;
    float reciprocalCellSize;
    float width;
    float height;
    float depth;
};
struct FieldDataMetadata {
    float gridMode;
    std::int32_t polygonCount;
    float retained;
};
struct FieldDataRoot {
    Vector3 *vertices;
    Vector3 *normals;
    float *planeDistances;
    FieldDataPolygon *polygons;
    FieldDataGridConfig *grid;
    std::int16_t **polygonLists;
    FieldDataMetadata *metadata;
    void **auxiliaryLists;
};
struct FieldDataRuntimePart {
    shen_name name;
    std::int32_t partClass;
    FieldDataRoot *data;
    std::int32_t enabled;
    std::int32_t transformed;
    std::byte retained14[4];
    std::int32_t yaw;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    Vector3 position;
#else
    std::byte retained1c[0x0c];
#endif
    Matrix4x4 transform, inverseTransform;
};
struct FieldPlane {
    Vector3 normal;
    float distance;
    const std::int16_t *vertices;
};
struct FieldIntersectionState {
    std::uint16_t found;
    std::int16_t polygon, flags;
    std::uint16_t retained;
    FieldDataRuntimePart *part;
    FieldPlane plane;
    Vector3 position;
};
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldPlane) == 0x14);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldIntersectionState) == 0x2c);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldIntersectionState, plane) == 0x0c);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldIntersectionState, position) == 0x20);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldDataMetadata) == 0x0c);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldDataPolygon) == 0x10);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldDataGridConfig) == 0x1c);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldDataRoot) == 0x20);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRoot, grid) == 0x10);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRoot, metadata) == 0x18);
SHENMUE_FIELD_DATA_LAYOUT(sizeof(FieldDataRuntimePart) == 0xa8);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, data) == 8);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, enabled) == 0x0c);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, transformed) == 0x10);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, yaw) == 0x18);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, position) == 0x1c);
#endif
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, transform) == 0x28);
SHENMUE_FIELD_DATA_LAYOUT(offsetof(FieldDataRuntimePart, inverseTransform) == 0x68);

using ::shenmue::shen_name;
#define FIELD_DATA_STORAGE shen_name(SHEN_NAME4('F', 'L', 'D', 'D'))
#define GENERATED_FIELD_NAME shen_name(SHEN_NAME4('@', '@', '@', '@'))
static const float kFieldPolygonTolerance = -0.0087259998545050621f;
extern FieldDataRoot *g_generatedFieldData;
extern std::int32_t g_generatedFieldVolumeCount;
extern FieldDataRuntimePart g_fieldParts[8];
extern std::int32_t g_fieldPartCount;
extern FieldDataRuntimePart *g_currentFieldPart;
extern FieldDataRoot *g_currentFieldData;
extern std::uint16_t g_fieldDataFlags;
extern std::uint16_t g_fieldDataClearFlags;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::uint16_t g_fieldDataResetFlags;
#endif
extern FieldPlane g_currentFieldPlane;
extern FieldIntersectionState g_fieldIntersection;
extern std::uint16_t &g_fieldIntersectionFound;
extern std::int16_t g_fieldGridCells[128];
FieldDataRoot *RelocateFieldData(void *data);
FieldDataRuntimePart *FindFieldDataPart(shen_name name);
std::int32_t ParseFieldData(void *data, shen_name name, std::int32_t relocate);
std::int32_t ParseFLDD(void *data, shen_name name);
std::int32_t RemoveFieldPart(shen_name name);
std::int32_t FLDD_ReadFieldData(void *chunk);
bool InitializeGeneratedFieldData();
std::int32_t flddSetPointFlagOnGrid(shen_name name, std::int32_t enabled);
void ResetFieldDataFlags();
std::uint32_t flddTestMapEventIntersection(const Vector3 *position);
void ClearFieldIntersection();
float FindFieldIntersectionHeight(const Vector3 *position);
std::int32_t GatherFieldGridCells(const Vector3 *position, std::int16_t *cells);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::int32_t yaw);
#else
bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::uint16_t yaw);
#endif
bool flddClearAllGeneratedVolumes();
float TransformClippedPoint(const Vector3 *position);
float FindFieldPartMinimumHeight(const Vector3 *position);
std::int32_t GatherFieldGridCellsUpward(const Vector3 *position, std::int16_t *cells);
#endif

} // namespace s2
} // namespace shenmue
#undef SHENMUE_FIELD_DATA_LAYOUT_JOIN_
#undef SHENMUE_FIELD_DATA_LAYOUT_JOIN
#undef SHENMUE_FIELD_DATA_LAYOUT

#endif
