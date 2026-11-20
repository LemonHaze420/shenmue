#include "shenmue.h"

#if SHENMUE_IS_S2

#include "s2/camera.hpp"

#define SHENMUE_CAMERA_PARAMETER_NOEXCEPT noexcept

namespace shenmue { namespace s2 {

// @verified - d
float camCtrlGetHalfVerticalFovTangent() SHENMUE_CAMERA_PARAMETER_NOEXCEPT {
    return g_cameraControl->halfVerticalFovTangent;
}

// @verified - d
float camCtrlGetPitchAngleTangent() SHENMUE_CAMERA_PARAMETER_NOEXCEPT {
    return g_cameraControl->pitchAngleTangent;
}

} } // namespace shenmue::s2

#undef SHENMUE_CAMERA_PARAMETER_NOEXCEPT

#endif
