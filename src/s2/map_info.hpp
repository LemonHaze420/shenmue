#pragma once

#include "common/types.hpp"
#include "shenmue.h"

#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace shenmue { namespace s1 {
struct FileState;
} }
#endif

namespace shenmue { namespace s2 {

struct MapInfoChunk {
    shen_name name;
    std::uint32_t nextByteOffset;
};

#if SHENMUE_S2_GCC295
typedef char sizeofMapInfoChunkCheck[sizeof(MapInfoChunk) == 8 ? 1 : -1];
#else
static_assert(sizeof(MapInfoChunk) == 8);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define MAP_INFO_STORAGE shen_name(SHEN_NAME4('I', 'N', 'F', 'O'))
#else
inline constexpr shen_name MAP_INFO_STORAGE{SHEN_NAME(INFO)};
#endif
extern s1::FileState *g_mapInfoFile;
MapInfoChunk *GetCurrentMapInfo();
#endif

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

} } // namespace shenmue::s2
