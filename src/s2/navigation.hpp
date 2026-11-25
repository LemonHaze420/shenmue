#pragma once
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/navi.hpp"
#include "common/types.hpp"
#include "s2/legacy_sprite_types.hpp"

namespace shenmue {
namespace s2 {
struct NaviTaskData {
    const NativeNaviMapConfig *config;
    std::uint32_t retained04;
    const void *definition;
    std::int32_t stream;
    Vector3 position;
    std::int16_t clipColumns, clipRows;
    std::uint8_t trigger, animation;
    std::uint16_t retained22;
};
struct NaviAreaConfig {
    shen_name map;
    std::int32_t configIndex;
};
struct NativeNaviFloorImage {
    std::int16_t x, z, width, textureIndex;
    std::uint64_t floorMask;
};
struct NativeNaviFloorDefinition {
    const char *filename;
    std::int32_t imageCount;
    const NativeNaviFloorImage *images;
    std::uint64_t floorMask;
};
typedef char NativeNaviFloorImageCheck[sizeof(NativeNaviFloorImage) == 0x10 ? 1 : -1];
typedef char NativeNaviFloorDefinitionCheck[sizeof(NativeNaviFloorDefinition) == 0x14 ? 1 : -1];
typedef char NativeNaviTaskDataSizeCheck[sizeof(NaviTaskData) == 0x24 ? 1 : -1];
typedef char NativeNaviTaskDataPositionCheck[offsetof(NaviTaskData, position) == 0x10 ? 1 : -1];
struct NativeNaviFolderEntry {
    std::int16_t itemId, textureSize;
    Vector2 offset;
    const char *label, *secondaryLabel;
    std::uint16_t flags, retained16;
};
struct NativeNaviFolderPage {
    shen_name area;
    float scaleX, scaleY;
    const NativeNaviFolderEntry *entries;
    float originX, originY;
    const char *sprite, *label, *secondaryLabel;
};
typedef char NativeNaviFolderEntrySizeCheck[sizeof(NativeNaviFolderEntry) == 0x18 ? 1 : -1];
typedef char NativeNaviFolderPageSizeCheck[sizeof(NativeNaviFolderPage) == 0x24 ? 1 : -1];
const NativeNaviFolderPage *navGetFolderPage(std::uint32_t page);
const NativeNaviFolderEntry *navGetFolderEntries(std::uint32_t page);
std::int32_t NaviFolderEntryCount(std::uint32_t page, std::uint16_t excluded);
std::uint32_t NaviFolderEntryMask(std::uint32_t page, std::uint16_t excluded);
void *LoadNaviFolderSprite(std::uint32_t page);
extern std::int32_t naviTextures[5];
extern std::int32_t naviBackgroundTexture, naviFloorTextures[6];
extern LegacySpritePacket naviPackets[5], naviOverlayPacket, naviTrailPacket;
extern std::int32_t naviPhase;
extern float naviTriggerBlend;
extern std::uint32_t naviTiltAngle;
extern std::uint8_t naviTrailStamp;
void NAVI_DummyWait_Enqueue(std::int8_t queue);
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t queue);
bool NaviIsDisplayable(bool skipMapItemRequirement);
std::int32_t navGetNumEntriesFor(shen_name map);
void InitializeNaviIcons();
void InitializeNaviInputState(NaviTaskData &task);
void SelectNaviMapMarkers();
void NAVI_DummyWait_Callback(void *data);
void NAVI_CtrlNavi_Callback(void *data);
void PrepareNaviFrame(void *data);
void navPlaceMarker(const Vector3 *position, std::int32_t type);
std::int32_t IsNaviActive();
std::int32_t navCreate(const Vector3 &position, std::int32_t type);
bool ConstructNaviMap(std::uint32_t configIndex);
std::int32_t OpenNaviWindow();
std::int32_t UpdateNaviWindow();
std::int32_t CloseNaviWindow(std::int32_t force);
} // namespace s2
} // namespace shenmue

#else

#include "s2/hlib.hpp"

#include <cstdint>

namespace shenmue::s2 {

// NAVI_DummyWait_Enqueue owns the native 0x30-byte NaviTaskData payload and
// switches from its one-frame initializer to the main loop.
HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex);

// Native GUI_Tasks_Enqueue supplies queue 12. The owner has no HLib payload;
// its persistent 0x60-byte work block is module storage.
#if SHENMUE_S2_GCC295
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t nextFunctionIndex = 12);
#else
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex = 12);
#endif
std::int64_t OpenNaviWindow();
std::int64_t CloseNaviWindow(std::int32_t force);

bool NaviIsDisplayable(bool skipMapItemRequirement);
std::uint32_t NaviFolderEntryMask(std::int32_t page, std::uint16_t excludedFlags = 2);
std::int32_t NaviFolderEntryCount(std::int32_t page, std::uint16_t excludedFlags = 2);
bool IsCurrentMapNaviFolderAvailable();
const char *GetNaviFolderLabel(std::int32_t page);
const char *GetNaviFolderEntryLabel(std::int32_t page, std::int32_t entry);

void ReleaseNavigationTaskOwners();

} // namespace shenmue::s2

#endif
