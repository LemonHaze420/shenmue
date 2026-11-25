#include "s2/door_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/door_control_dc.hpp"

namespace shenmue {
namespace s2 {

DoorRuntimeStatePrefix g_doorRuntimeState = {};
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
DoorDestination g_doorDestination = {};
shen_name g_doorQueuedAuthor = 0;
std::int32_t g_doorCachedSceneId = 0;
#endif

// @verified - d
shen_name drGetCurrentAuthorId() {
    if (!g_doorTaskEntries)
        return shen_name();
    if (g_doorQueuedAuthor)
        return g_doorQueuedAuthor;
    return g_doorRuntimeState.temporaryAuthor;
}

// @verified - d
void SetDoorQueuedAuthor(shen_name author) {
    shen_name *queued = &g_doorQueuedAuthor;
    *queued = author;
}

// @verified - d
shen_name GetCurrentDoorAuthor() {
    return drGetCurrentAuthorId();
}

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" {
void __attribute__((renesas)) shenmue_s2_event_SetAreaCleanupCallback(shenmue::s2::DoorAreaCleanupCallback callback) {
    shenmue::s2::SetDoorAreaCleanupCallback(callback);
}
void __attribute__((renesas)) shenmue_s2_event_ExecuteCleanupFuncForAreaWithCachedScene() {
    shenmue::s2::ExecuteCleanupFuncForAreaWithCachedScene();
}
}
#endif

#endif
