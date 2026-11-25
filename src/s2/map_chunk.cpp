#include "s2/map_info.hpp"

#include <cstring>

namespace shenmue {
namespace s2 {

// @verified - pd
MapInfoChunk *FindNativeChunk(const char name[4], MapInfoChunk *first) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(name);
    std::uint32_t token = bytes[0];
    token |= bytes[1] << 8;
    token |= bytes[2] << 16;
    token |= static_cast<std::uint32_t>(bytes[3]) << 24;
    MapInfoChunk *chunk = first;
    while (static_cast<std::uint32_t>(chunk->name) != token) {
        if (!chunk->nextByteOffset)
#else
    std::uint32_t token;
    std::memcpy(&token, name, sizeof(token));
    for (;;) {
        if (first->name.name == token)
            return first;
        if (!first->nextByteOffset)
#endif
            return 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->nextByteOffset);
#else
        first = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(first) + first->nextByteOffset);
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return chunk;
#endif
}

// @verified - p
std::uint32_t *GetNativeChunkSizeField(const char name[4], MapInfoChunk *first) {
    return reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uintptr_t>(FindNativeChunk(name, first)) + sizeof(std::uint32_t));
}

} // namespace s2
} // namespace shenmue
