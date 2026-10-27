#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

// The navigation start task's token; its '!' keeps it out of SHEN_NAME, whose argument clang-format would space.
inline constexpr shen_name TASK_NAVI_START{make_shen_name("nav!")};

enum class NaviMarkerType : std::uint32_t {
    ButtonX = 0,
    ButtonA = 1,
    ButtonB = 2,
    FixedX = 4,
    FixedA = 5,
    FixedB = 6,
};

struct NaviMapConfig {
    std::int16_t itemId;
    std::int16_t floorDefinition;
    float scale;
    float originX;
    float originZ;
    const char *spriteStem;
};

struct NaviAreaConfig {
    shen_name map;
    std::int32_t configIndex;
};

struct NaviFloorImage {
    std::int16_t x;
    std::int16_t z;
    std::int16_t width;
    std::int16_t textureIndex;
    std::uint64_t floorMask;
};

struct NaviFloorDefinition {
    const char *filename;
    std::int32_t imageCount;
    std::int32_t reserved0c;
    const NaviFloorImage *images;
    std::uint64_t floorMask;
};

struct NaviTaskData {
    const NaviMapConfig *config;
    std::uint64_t reserved08;
    const NaviFloorDefinition *definition;
    std::int32_t stream;
    Vector3 position;
    std::int16_t clipColumns;
    std::int16_t clipRows;
    std::uint8_t trigger;
    std::uint8_t animation;
    std::uint16_t reserved2e;
};

struct NaviTrailPoint {
    std::int16_t x;
    std::int16_t y;
};

struct NaviTrailStorage {
    std::array<NaviTrailPoint, 1022> points;
    std::int16_t next;
    std::int16_t count;
};

static_assert(sizeof(NaviMapConfig) == 0x18);
static_assert(sizeof(NaviAreaConfig) == 8);
static_assert(sizeof(NaviFloorImage) == 0x10);
static_assert(sizeof(NaviFloorDefinition) == 0x20);
static_assert(sizeof(NaviTaskData) == 0x30);
static_assert(offsetof(NaviTaskData, stream) == 0x18);
static_assert(offsetof(NaviTaskData, position) == 0x1c);
static_assert(offsetof(NaviTaskData, trigger) == 0x2c);
static_assert(sizeof(NaviTrailStorage) == 0xffc);

struct LegacySpritePacket;
bool DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t textureHandle, float depth);
bool SetDrawWithAlpha(const LegacySpritePacket &packet, std::int32_t textureIndex, float depth);
bool naviDraw(const LegacySpritePacket &packet, std::int32_t textureIndex, float depth);



bool IsNaviActive() noexcept;
bool IsNaviTaskEnabled() noexcept;
void SetNaviTaskEnabled(bool enabled);
bool IsNaviDisplayOverridden() noexcept;
bool SetNaviDisplayOverride(bool enabled) noexcept;
void ResetNaviInputPhase() noexcept;
void ReleaseNaviResources();

struct LegacyInputSlot;


bool InitializeNaviResources(HLib::Task *task, std::int32_t configIndex);
void InitializeNaviInputState(NaviTaskData &task);
// CtrlNavi runs interaction only after full input admission, otherwise it
// resets the phase. Animation and frame preparation run on every callback.
void UpdateNaviInteraction(const LegacyInputSlot &input);
void UpdateNaviInputAnimation(NaviTaskData &task, const LegacyInputSlot &input);
void PrepareNaviFrame(void *taskData);
float ComputeNaviAlphaLoss(float order);


void DrawNaviFrame(bool showFloorLabel);
void QueueNaviFrameDraw(bool showFloorLabel);

std::span<const NaviAreaConfig> GetNaviAreaConfigs() noexcept;
std::span<const NaviMapConfig> GetNaviMapConfigs() noexcept;
std::span<const NaviFloorDefinition> GetNaviFloorDefinitions() noexcept;
std::int32_t navGetNumEntriesFor(shen_name map) noexcept;
void ResetNaviSavedData() noexcept;
std::uint32_t LoadNaviMarkers(const std::uint32_t *markers) noexcept;
std::uint32_t SaveNaviMarkers(std::uint32_t *markers) noexcept;
std::int32_t LoadNaviTrail(const NaviTrailStorage *trail) noexcept;
std::int32_t SaveNaviTrail(NaviTrailStorage *trail) noexcept;
void SelectNaviMapMarkers(shen_name map) noexcept;
void RemoveLatestNaviMarker() noexcept;
void RemoveNaviMapMarkers() noexcept;
std::int32_t navCreate(const Vector3 &position, std::int32_t type);
std::int32_t navPlaceMarker(const Vector3 &position, std::int32_t type);
void AddNaviTrailLocations(std::span<const Vector3> locations);

} // namespace shenmue::s2
