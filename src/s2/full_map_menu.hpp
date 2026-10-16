#pragma once

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {
inline constexpr shen_name FULL_MAP_SPRITE_FILE{SHEN_NAME(NTST)};

struct FullMapFloor {
    std::int16_t item;
    std::int16_t width;
    float x;
    float z;
    std::uint32_t reserved0c;
    const char *label;
    std::uint64_t reserved18;
    std::uint16_t flags;
    std::byte reserved22[6];
};
struct FullMapConfig {
    shen_name map;
    float scale;
    float tileScale;
    std::uint32_t reserved0c;
    const FullMapFloor *floors;
    float originX;
    float originZ;
    const char *sprite;
    const char *areaLabel;
    std::uint64_t reserved30;
};
static_assert(sizeof(FullMapFloor) == 0x28);
static_assert(offsetof(FullMapFloor, flags) == 0x20);
static_assert(sizeof(FullMapConfig) == 0x38);
static_assert(offsetof(FullMapConfig, floors) == 0x10);
static_assert(offsetof(FullMapConfig, sprite) == 0x20);

void InitializeFullMapMenu();
bool CanOpenFullMapMenu();
std::int64_t OpenFullMapMenu();
std::int64_t UpdateFullMapMenu();
void CloseFullMapMenu();
std::span<const FullMapConfig, 3> GetFullMapConfigs() noexcept;
std::span<const FullMapFloor> GetFullMapFloors(std::int32_t index) noexcept;
std::uint32_t GetFullMapFloorMask(std::int32_t index, std::uint16_t excludedFlags);

} // namespace shenmue::s2
