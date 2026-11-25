#include "shenmue.h"

#if SHENMUE_IS_S2

#if SHENMUE_IS_DC
#include "s2/camera_dc.hpp"
#else
#include "s2/camera.hpp"
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#define SHENMUE_CAMERA_PARAMETER_NOEXCEPT
#else
#define SHENMUE_CAMERA_PARAMETER_NOEXCEPT noexcept
#endif

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
