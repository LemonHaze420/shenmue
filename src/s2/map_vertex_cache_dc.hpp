#pragma once

#include "s2/map_models_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct MapVertexCacheWork {
    const MapModelClipRecord *records;
    std::int32_t recordIndex;
    Vector3 *vertices;
    std::int32_t vertexCount;
};
typedef char Layout_map_vertex_cache_dc_0[(sizeof(MapVertexCacheWork) == 0x10) ? 1 : -1];
typedef char Layout_map_vertex_cache_dc_1[(offsetof(MapVertexCacheWork, vertices) == 8) ? 1 : -1];
#define MAP_VERTEX_CACHE_STORAGE SHEN_NAME4('V', 'X', 'P', 'C')
static const std::int32_t MAP_VERTEX_CACHE_CAPACITY = 100000;
static const float MAP_VERTEX_CACHE_DISTANCE_SQUARED = 2.500000277905201e-07f;
extern MapVertexCacheWork g_mapVertexCache;
extern std::int32_t g_mapVertexCacheEnabled;

void MarkSharedMapMesh(MT7Node *owner, MT7Node *node);
void MarkSharedMapMeshForced(MT7Node *owner, MT7Node *node);
void MarkSharedMapMeshes(MT7Node *node);
void BakeMapNodeVertices(MT7Node *node);
void InitializeMapVertexCache();

} // namespace s2
} // namespace shenmue

#endif
