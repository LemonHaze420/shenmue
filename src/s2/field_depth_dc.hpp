#pragma once

#include "s2/camera_dc.hpp"
#include "s2/map_models_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct CameraFieldDepthSample {
    std::uint32_t id;
    float depth;
};
typedef char Layout_field_depth_dc_0[(sizeof(CameraFieldDepthSample) == 8) ? 1 : -1];
typedef char Layout_field_depth_dc_1[(offsetof(CameraFieldDepthSample, depth) == 4) ? 1 : -1];
static const std::int32_t kFieldDepthHalfWidth = 128;
static const std::size_t kFieldDepthSampleCount = 257;
static const std::uint32_t kInitialFieldDepthId = 0x10000;
static const float kInitialFieldDepth = 20000.0f, kFieldDepthNearClip = 0.1f, kFieldDepthPixelScale = 0.0078125f;
extern CameraFieldDepthSample worldFieldDepth[kFieldDepthSampleCount], cameraFieldDepth[kFieldDepthSampleCount];
extern std::uint32_t fieldDepthPointCount;
extern Matrix4x4 fieldDepthTransform, fieldDepthModelTransform;
void camCtrlBuildYawDepthTransform();
void camCtrlSubmitFieldDepth();
void PublishFieldDepthSphereSpan(float x, float depth, float radius, std::uint32_t id);
void PublishFieldDepthSphere(const Vector4 &sphere, std::uint32_t id);
void PublishFieldDepthWall(std::int32_t cameraOnly, const Vector3 *first, const Vector3 *second);
bool camCtrlIsModelDepthVisible(const Vector3 &centre, const MT7Node &node, float radius);
bool camCtrlCheckDepthBufferPosition(const CameraFieldDepthSample *samples, const Vector3 &position, float radius);
bool camCtrlCheckWorldDepthPosition(const Vector3 &position, float radius);
bool camCtrlCheckPosition(const Vector3 &position, float radius);
void SetMapFieldDepthSpheres(Vector4 *value, std::int32_t index);
Vector4 *GetMapFieldDepthSpheres(std::int32_t index);
void SetMapFieldDepthSphereCount(std::int32_t value, std::int32_t index);
std::int32_t GetMapFieldDepthSphereCount(std::int32_t index);
void SetMapFieldDepthWalls(MapFieldDepthWall *value, std::int32_t index);
MapFieldDepthWall *GetMapFieldDepthWalls(std::int32_t index);
void SetMapFieldDepthCameraWalls(MapFieldDepthWall *value, std::int32_t index);
MapFieldDepthWall *GetMapFieldDepthCameraWalls(std::int32_t index);
void SetMapFieldDepthWallCount(std::int32_t value, std::int32_t index);
std::int32_t GetMapFieldDepthWallCount(std::int32_t index);
void SetMapFieldDepthCameraWallCount(std::int32_t value, std::int32_t index);
std::int32_t GetMapFieldDepthCameraWallCount(std::int32_t index);
void SetMapFieldDepthCells(MapModelCommandCell *value, std::int32_t index);
MapModelCommandCell *GetMapFieldDepthCells(std::int32_t index);
void SetMapFieldDepthCellCount(std::int32_t value, std::int32_t index);
std::int32_t GetMapFieldDepthCellCount(std::int32_t index);
std::uint32_t GetFieldDepthPointCount();
void SetFieldDepthPointCount(std::uint32_t count);
const Matrix4x4 &GetFieldDepthTransform();
Matrix4x4 &GetFieldDepthModelTransform();
const MapCellOffset *GetMapCellDirectionOffsets(std::int32_t direction);
std::int32_t GetMapVisibleCellCount(std::int32_t direction, std::int32_t tier);
void GetFieldDepthCell(const Vector3 &position, MapCellOffset &cell);

} // namespace s2
} // namespace shenmue

#endif
