#include "s2/event_package_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_info.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void *LoadCurrentPakBinary(const char *filename) {
    void *data = 0;
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    if (package)
        data = FileLoader(package, filename, PACKAGE_BINARY_EXTENSION);
    return data;
}

// @verified - d
std::uint32_t GetCurrentPakBinarySize(const char *filename) {
    std::uint32_t size = 0;
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    if (package)
        size = GetPakFileSize(package, filename, PACKAGE_BINARY_EXTENSION);
    return size;
}

} // namespace s2
} // namespace shenmue

extern "C" {

std::uint32_t *__attribute__((renesas)) shenmue_s2_event_FindMapInfoChunkSize(const char name[4], shenmue::s2::MapInfoChunk *first) {
    return shenmue::s2::GetNativeChunkSizeField(name, first);
}

void *__attribute__((renesas)) shenmue_s2_event_LoadCurrentPakBinary(const char *filename) {
    return shenmue::s2::LoadCurrentPakBinary(filename);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetCurrentPakBinarySize(const char *filename) {
    return shenmue::s2::GetCurrentPakBinarySize(filename);
}
}

#endif
