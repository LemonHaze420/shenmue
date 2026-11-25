#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

struct DoorTaskEntry;
typedef void(__attribute__((renesas)) * DoorAreaCleanupCallback)(std::int32_t sceneId);
struct DoorRuntimeStatePrefix {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t transitionState;
    shen_name temporaryAuthor, destinationDoor;
    char savedSoundMap[16], savedSoundSequence[16];
    std::int32_t savedSoundScene;
#else
    std::uint32_t retainedHeader;
    shen_name temporaryAuthor;
    std::byte retainedState[0x38];
    shen_name queuedAuthor;
    std::int32_t cachedSceneId;
#endif
};
typedef char Layout_door_state_dc_1[(offsetof(DoorRuntimeStatePrefix, temporaryAuthor) == 4) ? 1 : -1];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char Layout_door_state_dc_0[(sizeof(DoorRuntimeStatePrefix) == 0x30) ? 1 : -1];
typedef char Layout_door_state_dc_4[(offsetof(DoorRuntimeStatePrefix, savedSoundMap) == 0x0c) ? 1 : -1];
typedef char Layout_door_state_dc_5[(offsetof(DoorRuntimeStatePrefix, savedSoundSequence) == 0x1c) ? 1 : -1];
typedef char Layout_door_state_dc_6[(offsetof(DoorRuntimeStatePrefix, savedSoundScene) == 0x2c) ? 1 : -1];
// GameJam keeps the pending door destination, the queued author and the cached scene id as their own objects
// after the door state.
struct DoorDestination {
    std::int32_t phase;
    shen_name area, region;
    std::int32_t entry;
};
typedef char Layout_door_state_dc_7[(sizeof(DoorDestination) == 0x10) ? 1 : -1];
extern DoorDestination g_doorDestination;
extern shen_name g_doorQueuedAuthor;
extern std::int32_t g_doorCachedSceneId;
#else
typedef char Layout_door_state_dc_0[(sizeof(DoorRuntimeStatePrefix) == 0x48) ? 1 : -1];
typedef char Layout_door_state_dc_2[(offsetof(DoorRuntimeStatePrefix, queuedAuthor) == 0x40) ? 1 : -1];
typedef char Layout_door_state_dc_3[(offsetof(DoorRuntimeStatePrefix, cachedSceneId) == 0x44) ? 1 : -1];
#endif
extern DoorTaskEntry *g_doorTaskEntries;
extern DoorRuntimeStatePrefix g_doorRuntimeState;
shen_name drGetCurrentAuthorId();
shen_name GetCurrentDoorAuthor();
void SetDoorQueuedAuthor(shen_name author);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern DoorAreaCleanupCallback g_doorAreaCleanupCallback;
void SetDoorAreaCleanupCallback(DoorAreaCleanupCallback callback);
void ExecuteCleanupFuncForAreaWithCachedScene();
void ExecuteDoorAreaCleanup(std::int32_t sceneId, std::int32_t apply);
#endif

} // namespace s2
} // namespace shenmue

#endif
