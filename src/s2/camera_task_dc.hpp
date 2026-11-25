#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"

namespace shenmue {
namespace s2 {

#define TASK_CAMT SHEN_NAME4('C', 'A', 'M', 'T')
void camCtrlCallback(void *data);
void camCtrlComputeNextPosition(CameraRuntime &camera);
void UpdateCameraTransitions(CameraRuntime &camera);
void camCtrlCheckCollision(CameraRuntime &camera);
void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera);
void camCtrlApply(CameraRuntime &camera);
void camCtrlApplyJitterAdjustment(CameraRuntime &camera);
void camCtrlSmoothMotion(CameraRuntime &camera);
float GenerateOscillationForCamera(std::int32_t phase);
void ApplyRotationMatrixFromTransform(CameraRuntime &camera);
void camCtrlRecalcFov(CameraRuntime &camera);
void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);
void camCtrlSetup(CameraRuntime &camera);
void camCtrlHandleStates(CameraRuntime &camera);
void camCtrlSetShakeAmplitude(float x, float y, float z);
void InitializeConversationCameraState();
void camCtrlInitSplines();
HLib::Task *Queue_CameraCtrl(std::uint8_t nextFunctionIndex);

} // namespace s2
} // namespace shenmue

#endif
