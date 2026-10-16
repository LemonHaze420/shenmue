#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

extern void *g_pSTCH;
extern std::uint32_t g_sceneDataMemoryFlags;
extern std::uint32_t g_originalStripShaderCapacity;
extern std::uint32_t g_originalStripBufferCapacity;
extern std::uint32_t g_originalStripIndexCapacity;
extern void *g_original_strip00_shader;
extern void *g_original_strip00_buffer;
extern std::int32_t g_original_strip00_Buffer;



struct OriginalStripPassRecord {
    std::int32_t texture;
    std::int32_t vertexCount;
    std::int32_t startVertex;
    float depth;
};
static_assert(sizeof(OriginalStripPassRecord) == 0x10);



extern std::uint32_t g_originalStrip00VertexCursor;
extern std::uint32_t g_originalStrip03VertexCursor;
extern std::int32_t g_originalStripPassCount;

extern std::uint32_t *g_originalStripBlendBlock;
inline constexpr std::array<std::uint32_t, 4> kOriginalStripBlendPairs{8, 6, 8, 10};

void *AllocateSceneTransient(std::size_t size);

std::int32_t InitBuffer(std::uint32_t recordSize);
std::int64_t InitSceneDataMemory();
void FreeSceneDataMemory();

} // namespace shenmue::s1
