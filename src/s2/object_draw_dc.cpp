#include "s2/object_draw_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/camera_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cmath>
#include <cstring>
#include "s2/map_visibility_dc.hpp"

namespace shenmue {
namespace s2 {

std::uint32_t GetFieldDepthPointCount();

std::uint32_t GetFieldDepthPointCount();
std::uint32_t GetFieldDepthPointCount();
std::uint32_t GetFieldDepthPointCount();
std::uint32_t GetFieldDepthPointCount();
std::uint32_t GetFieldDepthPointCount();
void UpdateWindForFlag();
std::int32_t fieldInverseYaw, g_currentDrawTier;
float fieldCellScale, fieldFarDistance;
std::uint32_t g_currentFieldPointCount;

// @verified - d
std::int32_t camCtrlGetDrawTier(float distance) {
    std::int32_t tier = static_cast<std::int32_t>(distance * fieldCellScale);
    if (tier < 0)
        tier = 0;
    if (tier > kMaximumMapDrawTier)
        tier = kMaximumMapDrawTier;
    return tier;
}

// @verified - d
void DrawWorldObjects() {
    g_currentFieldPointCount = GetFieldDepthPointCount();
    g_currentDrawTier = camCtrlGetDrawTier(fieldFarDistance);
    HLib::ExecuteCallbackNow(DrawOrdinaryMapCallback, 0);
    HLib::ExecuteCallbackNow(DrawMapPlacementCallback, 0);
    UpdateWindForFlag();
    HLib::ExecuteCallbackNow(DrawPropObjectCallback, 0);
}

// @verified - d
void InitializeObjectDrawNullsub() {
}

ObjectDrawTaskParameters *GetObjectDrawTaskParameters() {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<ObjectDrawTaskParameters *>(HLib::GetTaskParameter(control->retainedTaskA));
}

// @verified - d
void OBJ_Object_Callback(void *data) {
    static_cast<ObjectDrawTaskParameters *>(data)->submissionState = 0;
    if (g_worldDrawingEnabled)
        DrawWorldObjects();
}

// @verified - d
void GetFieldDepthCell(const Vector3 &position, MapCellOffset &cell) {
    float x = position.x * fieldCellScale;
    float z = position.z * fieldCellScale;
    if (x < 0.0f)
        x -= 1.0f;
    if (z < 0.0f)
        z -= 1.0f;
    cell.x = static_cast<std::int16_t>(static_cast<std::int32_t>(x));
    cell.z = static_cast<std::int16_t>(static_cast<std::int32_t>(z));
}

// @verified - d
void camCtrlSetupDepthScaling() {
    fieldCellScale = GetMapCellScale();
    fieldInverseYaw = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(camCtrlGetNextRotY()));
    const float farDistance = camCtrlGetFarDistance();
    fieldFarDistance = farDistance;
    const float nearDistance = camCtrlGetNearDistance();
    const float npcRadius = camCtrlGetNpcRadius();
    const float halfVerticalFovTangent = camCtrlGetHalfVerticalFovTangent();
    const float pitchAngleTangent = camCtrlGetPitchAngleTangent();
    shenSetDepthScale(farDistance, nearDistance, npcRadius, halfVerticalFovTangent, pitchAngleTangent);
}

} // namespace s2
} // namespace shenmue

#endif
