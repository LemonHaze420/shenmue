#pragma once

#include "s2/camera_spline_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct Character;
static const float CAMERA_SPLINE_FRAME_TIME = 0.016666668f;
static const std::uint32_t CAMERA_SPLINE_DRIVE_ROLL = 0x20U, CAMERA_SPLINE_DRIVE_FOV = 0x40U;
std::int32_t DoKeyframeCubicHermiteInterpolation(CameraSplineTrack &track, float time, float *out);
void SeekCameraSplineTrack(std::int32_t frame, CameraSplineTrack *track);
std::uint32_t AllocSpline(CameraSplineTrack &track);
void SeekAnimatedCameraSequence(std::uint32_t frame);
void camCtrlBuildScriptCamSpline(std::int32_t name, const void *eye, const void *target);
void camCtrlCreateSpline(const std::uint32_t *record, const void *eye, const void *target);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t *camCtrlFindEventCamera(std::int32_t index);
std::int32_t GetSplineMaxDistinctKeyRun(const CameraSplineTrack &track);
std::int32_t GetSplineSegmentBufferSize(const CameraSplineTrack &track);
std::int32_t InitializeAuthorSplineTrack(
    CameraSplineTrack &track, float *coefficients, std::int32_t count, const float *times, const float *values, const float *tangents, std::int32_t reserved);
std::int32_t camCtrlIsLookAroundActive();
void camCtrlSetDefaultOrbitDistance(float distance);
std::uint32_t camCtrlIsFlag21Set();
#endif
void camCtrlHandleQueuedSpline();
std::int32_t TestScriptCameraSplineCollision(std::int32_t name, const void *eye, const void *target, Character *obstacle);
void DoCameraSplineInterpolation(CameraSplineState &spline);

} // namespace s2
} // namespace shenmue

#endif
