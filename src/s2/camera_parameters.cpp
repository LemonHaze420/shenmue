#include "shenmue.h"

#if SHENMUE_IS_S2

#include "s2/camera.hpp"

#define SHENMUE_CAMERA_PARAMETER_NOEXCEPT noexcept

namespace shenmue { namespace s2 {

// @verified - pd
float camCtrlGetFarDistance() SHENMUE_CAMERA_PARAMETER_NOEXCEPT {
    return g_cameraControl->farDistance;
}

// @verified - pd
float camCtrlGetNearDistance() SHENMUE_CAMERA_PARAMETER_NOEXCEPT {
    return g_cameraControl->nearDistance;
}

// @verified - pd
float camCtrlGetNpcRadius() SHENMUE_CAMERA_PARAMETER_NOEXCEPT {
    return g_cameraControl->npcRadius;
}

// @verified - pd
float camCtrlSetFarDistance(float distance) {
    const float previous = g_cameraControl->farDistance;
    g_cameraControl->farDistance = distance;
    return previous;
}

// @verified - p
void camCtrlSetNearDistance(float distance) {
    g_cameraControl->nearDistance = distance;
}

// @verified - p
void camCtrlSetNpcRadius(float radius) {
    g_cameraControl->npcRadius = radius;
}

} } // namespace shenmue::s2

#undef SHENMUE_CAMERA_PARAMETER_NOEXCEPT

#endif
