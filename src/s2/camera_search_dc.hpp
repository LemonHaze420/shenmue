#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct SearchCameraState {
    std::int32_t fovStyle;
    std::int32_t phase;
    Vector3 lookBase;
    std::int16_t rotationY;
    std::uint16_t retained16;
    float lockedOnAngle;
    Vector3 cameraPosition;
    Vector3 offsetPosition;
    Vector3s initialRotation;
    std::int16_t retained3a[3];
    float distance;
    float retained44[7];
    Vector3 lookTarget;
    std::uint32_t flags;
};
typedef char Layout_camera_search_dc_0[(sizeof(SearchCameraState) == 0x70) ? 1 : -1];
typedef char Layout_camera_search_dc_1[(offsetof(SearchCameraState, lookBase) == 8) ? 1 : -1];
typedef char Layout_camera_search_dc_2[(offsetof(SearchCameraState, rotationY) == 0x14) ? 1 : -1];
typedef char Layout_camera_search_dc_3[(offsetof(SearchCameraState, cameraPosition) == 0x1c) ? 1 : -1];
typedef char Layout_camera_search_dc_4[(offsetof(SearchCameraState, initialRotation) == 0x34) ? 1 : -1];
typedef char Layout_camera_search_dc_5[(offsetof(SearchCameraState, distance) == 0x40) ? 1 : -1];
typedef char Layout_camera_search_dc_6[(offsetof(SearchCameraState, lookTarget) == 0x60) ? 1 : -1];
typedef char Layout_camera_search_dc_7[(offsetof(SearchCameraState, flags) == 0x6c) ? 1 : -1];

extern SearchCameraState g_searchCameraState;
extern std::uint32_t g_searchCameraFov;
extern std::uint32_t g_searchCameraZoomFov;
extern float g_searchCameraZoomScale;
extern std::int32_t g_searchCameraYaw;
extern std::int32_t g_searchCameraPitch;
void srchSetupCameraFov();
bool shenCheckCurrentFoVModeIs2();
bool camCtrlSwitchToLockedOn(std::int32_t fovStyle);
std::uint32_t shenApplyLockonFlag(std::uint32_t flags);
bool camCtrlSetLookAroundMode();
void srchSetLookBase(const Vector3 *base);
void srchSetRotY(std::int16_t yaw);
void camCtrlSetLockedOnAngle(float angle);
void srchSetLookTarget(const Vector3 *target);
void srchGetLookTarget(Vector3 *target);

} // namespace s2
} // namespace shenmue

#endif
