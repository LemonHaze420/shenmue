#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct MapInfoChunk {
    shen_name name;
    std::uint32_t nextByteOffset;
};

static_assert(sizeof(MapInfoChunk) == 8);

void *SceneMapinfoReader(std::int32_t actNumber, shen_name areaName);
MapInfoChunk *FindMapInfoChunk(const char name[4], void *mapInfo);
const MapInfoChunk *FindMapInfoChunk(const char name[4], const void *mapInfo);
MapInfoChunk *FindNativeChunk(const char name[4], MapInfoChunk *first);
std::uint32_t *GetNativeChunkSizeField(const char name[4], MapInfoChunk *first);
void MAPINFORead(void *mapInfo);
bool GetCurrentMapOperandName(std::uint16_t index, shen_name &name);
// Resolves SCNT mode 15's current-map arm. DOOR and SCEN dynamic-name owners
// compare against GetCurrentMapOperandName at the expression boundary.
bool ResolveCurrentMapOperand(std::uint16_t index, std::int32_t &value);

extern std::uint64_t g_mapInfoSize;

} // namespace shenmue::s2
