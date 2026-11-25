#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && (SHENMUE_IS_S1 || SHENMUE_IS_S2)
#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstdint>
#endif
namespace shenmue {
namespace s1 {
#if !SHENMUE_S2_GCC295
#pragma pack(push, 1)
#endif
struct PakDirectory {
    std::uint32_t tag;
    std::uint32_t size;
    std::int32_t entryCount;
    std::uint32_t entriesOffset;
};

struct PakDirectoryEntry {
    char name[8];
    std::uint32_t tag;
    std::uint32_t dataOffset;
    std::uint32_t auxiliary;
};

#if !SHENMUE_S2_GCC295
#pragma pack(pop)
#endif
#if SHENMUE_S2_GCC295
typedef char PakDirectorySizeCheck[sizeof(PakDirectory) == 0x10 ? 1 : -1];
typedef char PakDirectoryAlignmentCheck[__alignof__(PakDirectory) == 4 ? 1 : -1];
typedef char PakDirectoryEntrySizeCheck[sizeof(PakDirectoryEntry) == 0x14 ? 1 : -1];
#else
static_assert(sizeof(PakDirectory) == 0x10);
static_assert(alignof(PakDirectory) == 1);
static_assert(sizeof(PakDirectoryEntry) == 0x14);
#endif
} // namespace s1
} // namespace shenmue
#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
using s1::PakDirectory;
using s1::PakDirectoryEntry;
#else
namespace shenmue {
namespace s1 {
#endif
std::int32_t PACK_FindNamedEntry(PakDirectory *directory, const char *filename);
}
} // namespace shenmue
#endif
