#include "s2/map_info.hpp"

#include <cstring>

namespace shenmue { namespace s2 {

// @verified - pd
MapInfoChunk *FindNativeChunk(const char name[4], MapInfoChunk *first) {
    std::uint32_t token;
    std::memcpy(&token, name, sizeof(token));
    for (;;) {
        if (first->name.name == token)
            return first;
        if (!first->nextByteOffset)
            return 0;
        first = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(first) + first->nextByteOffset);
    }
}

// @verified - p
std::uint32_t *GetNativeChunkSizeField(const char name[4], MapInfoChunk *first) {
    return reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uintptr_t>(FindNativeChunk(name, first)) + sizeof(std::uint32_t));
}

} } // namespace shenmue::s2
