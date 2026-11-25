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

// @verified - pd
float camCtrlSetFarDistance(float distance) {
    const float previous = g_cameraControl->farDistance;
    g_cameraControl->farDistance = distance;
    return previous;
}

#if SHENMUE_IS_DC
// @verified - d
float camCtrlSetNearDistance(float distance) {
    const float previous = g_cameraControl->nearDistance;
    g_cameraControl->nearDistance = distance;
    return previous;
}
#else
// @verified - p
void camCtrlSetNearDistance(float distance) {
    g_cameraControl->nearDistance = distance;
}
#endif

#if SHENMUE_IS_DC
// @verified - d
float camCtrlSetNpcRadius(float radius) {
    const float previous = g_cameraControl->npcRadius;
    g_cameraControl->npcRadius = radius;
    return previous;
}
#else
// @verified - p
void camCtrlSetNpcRadius(float radius) {
    g_cameraControl->npcRadius = radius;
}
#endif

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

} } // namespace shenmue::s2

#undef SHENMUE_CAMERA_PARAMETER_NOEXCEPT

#endif
