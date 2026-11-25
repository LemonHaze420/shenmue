#include "s2/time_hud_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/camera_state_query_dc.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/map_event_character_dc.hpp"

namespace shenmue { namespace s2 {

// @verified - d
std::int32_t UpdateCompassAdmission(TimeCompassContext &context) {
    Vector3 position;
    camCtrlGetNextCamPos(&position);
    std::int32_t counter = context.counter;
    const std::uint32_t frame = static_cast<std::uint32_t>(spliGetCurrentFrmCnt());
    const bool controlled = IsPlayerControlEnabled();
    const std::int32_t state = camCtrlGetState();
    if (controlled && state != 14) {
        if (static_cast<float>(counter) < 45.0f)
            ++counter;
        else
            context.enabled = 1;
    } else if (counter <= 0) {
        context.enabled = 0;
    } else {
        if (!context.previousSearch) {
            const std::int32_t delta = static_cast<std::int32_t>(frame - context.previousFrame);
            const std::int32_t magnitude = delta < 0 ? static_cast<std::int32_t>(context.previousFrame - frame) : delta;
            if (magnitude > 2 && static_cast<float>(counter) > 22.5f)
                counter = 22;
        }
        const Vector3 previous = context.previousCameraPosition;
        const float epsilon = 0.00000011920929f;
        if (position.x < epsilon)
            position.x = 0.0f;
        if (position.y < epsilon)
            position.y = 0.0f;
        if (position.z < epsilon)
            position.z = 0.0f;
        bool changed = state == 9 || state == 14 || authGetLoaded() != 0;
        if (!changed) {
            const float x = position.x - previous.x;
            changed = (x < 0.0f ? -x : x) > epsilon;
            if (!changed) {
                const float y = position.y - previous.y;
                changed = (y < 0.0f ? -y : y) > epsilon;
                if (!changed) {
                    const float z = position.z - previous.z;
                    changed = (z < 0.0f ? -z : z) > epsilon;
                }
            }
        }
        if (changed)
            --counter;
    }
    context.previousCameraPosition.x = position.x;
    context.previousCameraPosition.y = position.y;
    context.previousCameraPosition.z = position.z;
    context.counter = counter;
    context.previousFrame = frame;
    context.previousSearch = controlled ? 1 : 0;
    return context.enabled;
}

} } // namespace shenmue::s2

#endif
