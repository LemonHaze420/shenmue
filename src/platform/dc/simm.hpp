#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue { namespace s1 {

struct SimmEntry {
    std::uint8_t type;
    char name[15];
    std::uint32_t offset;
    std::uint32_t size;
};

#if SHENMUE_S2_GCC295
typedef char SimmEntrySizeCheck[sizeof(SimmEntry) == 0x18 ? 1 : -1];
typedef char SimmEntryOffsetCheck[offsetof(SimmEntry, offset) == 0x10 ? 1 : -1];
typedef char SimmEntrySizeOffsetCheck[offsetof(SimmEntry, size) == 0x14 ? 1 : -1];
#else
static_assert(sizeof(SimmEntry) == 0x18);
static_assert(offsetof(SimmEntry, offset) == 0x10);
static_assert(offsetof(SimmEntry, size) == 0x14);
#endif

} } // namespace shenmue::s1

#if SHENMUE_IS_S2
namespace shenmue { namespace s2 {
#if SHENMUE_S2_GCC295
typedef s1::SimmEntry SimmEntry;
#else
using SimmEntry = s1::SimmEntry;
#endif
#else
namespace shenmue { namespace s1 {
#endif

extern std::int32_t g_simmError;
std::int32_t SIMM_UppercaseAscii(std::int32_t value);
std::int32_t SIMM_CompareNames(const char *left, const char *right);
const std::byte *SIMM_GetRoot(const void *buffer);
const SimmEntry *SIMM_FindEntry(const std::byte *table, const char *name);
const std::byte *SIMM_FindDirectory(const std::byte *root, const char *path);
const SimmEntry *SIMM_ResolveFile(const std::byte *root, const char *directory, const char *filename);
std::int32_t SIMM_GetEntrySize(const void *buffer, const char *directory, const char *filename, std::uint32_t *size);
const std::byte *SIMM_GetEntryData(const void *buffer, const char *directory, const char *filename);

} } // namespace shenmue

#endif
