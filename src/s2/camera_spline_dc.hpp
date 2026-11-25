#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct CameraSplineTrack {
    std::int32_t count;
    std::int32_t firstKey;
    std::uint32_t segmentCount;
    const float *times;
    const float *values;
    const float *tangents;
    float *coefficients;
};
typedef char Layout_camera_spline_dc_0[(sizeof(CameraSplineTrack) == 0x1c) ? 1 : -1];
typedef char Layout_camera_spline_dc_1[(offsetof(CameraSplineTrack, firstKey) == 4) ? 1 : -1];
typedef char Layout_camera_spline_dc_2[(offsetof(CameraSplineTrack, segmentCount) == 8) ? 1 : -1];
typedef char Layout_camera_spline_dc_3[(offsetof(CameraSplineTrack, times) == 0x0c) ? 1 : -1];
typedef char Layout_camera_spline_dc_4[(offsetof(CameraSplineTrack, values) == 0x10) ? 1 : -1];
typedef char Layout_camera_spline_dc_5[(offsetof(CameraSplineTrack, tangents) == 0x14) ? 1 : -1];
typedef char Layout_camera_spline_dc_6[(offsetof(CameraSplineTrack, coefficients) == 0x18) ? 1 : -1];

struct CameraSplineState {
    std::uint32_t state;
    std::uint32_t retained04;
    std::uint16_t eyeMode, targetMode;
    shen_name name;
    std::uint32_t *record;
    std::uint32_t authorFlags;
    CameraSplineTrack eyeTracks[3];
    CameraSplineTrack targetTracks[3];
    CameraSplineTrack rollTrack;
    CameraSplineTrack fovTrack;
    float coefficientStorage[8][400];
    void *eyeNode, *targetNode;
    Vector3 eyeBasePosition, targetBasePosition;
    Vector3i eyeBaseRotation, targetBaseRotation;
    Vector3 fallbackPosition;
    Vector3i fallbackRotation;
    std::uint32_t roll;
    std::int32_t fov;
    Vector3 cachedEye, cachedTarget;
    std::int32_t currentFrame;
    float currentTime;
    Vector3 eye, target;
    float retained3388;
    Vector3 localEye, localTarget;
    std::uint16_t publishedFov, publishedRoll;
};
typedef char Layout_camera_spline_dc_7[(sizeof(CameraSplineState) == 0x33a8) ? 1 : -1];
typedef char Layout_camera_spline_dc_8[(offsetof(CameraSplineState, name) == 0x0c) ? 1 : -1];
typedef char Layout_camera_spline_dc_9[(offsetof(CameraSplineState, authorFlags) == 0x14) ? 1 : -1];
typedef char Layout_camera_spline_dc_10[(offsetof(CameraSplineState, eyeTracks) == 0x18) ? 1 : -1];
typedef char Layout_camera_spline_dc_11[(offsetof(CameraSplineState, targetTracks) == 0x6c) ? 1 : -1];
typedef char Layout_camera_spline_dc_12[(offsetof(CameraSplineState, rollTrack) == 0xc0) ? 1 : -1];
typedef char Layout_camera_spline_dc_13[(offsetof(CameraSplineState, fovTrack) == 0xdc) ? 1 : -1];
typedef char Layout_camera_spline_dc_14[(offsetof(CameraSplineState, currentFrame) == 0x3368) ? 1 : -1];
typedef char Layout_camera_spline_dc_15[(offsetof(CameraSplineState, coefficientStorage) == 0xf8) ? 1 : -1];
typedef char Layout_camera_spline_dc_16[(offsetof(CameraSplineState, eyeNode) == 0x32f8) ? 1 : -1];
typedef char Layout_camera_spline_dc_17[(offsetof(CameraSplineState, eye) == 0x3370) ? 1 : -1];
typedef char Layout_camera_spline_dc_18[(offsetof(CameraSplineState, localEye) == 0x338c) ? 1 : -1];
extern CameraSplineState g_cameraSplineStorage;
extern CameraSplineState *g_currentCameraSplines;
void camCtrlInitSplines();
std::int32_t camCtrlGetState();
std::int32_t camCtrlCheckIsEventCam();
std::int32_t spliGetCurrentFrmCnt();
void camCtrlSetSplineShouldRebuildRotTo(std::int32_t enabled);

} // namespace s2
} // namespace shenmue

#endif
