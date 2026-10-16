#include "s1/map_entries.hpp"

#include <algorithm>
#include <array>

namespace shenmue::s1 {

std::span<const MapEntry> GetMapEntries() noexcept {
    return kMapEntries;
}

bool IsMapEntry(std::int32_t sceneId, shen_name areaId, std::uint32_t entryPoint) noexcept {
    return std::ranges::any_of(kMapEntries,
        [=](const MapEntry &entry) { return entry.sceneId == sceneId && entry.areaId.name == areaId.name && entry.entryPoint == entryPoint; });
}

} // namespace shenmue::s1
