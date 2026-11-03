#pragma once

#include "s1/file_state.hpp"

#include <cstdint>
#include <cstddef>
#include <array>
#include "shenmue.h"

namespace shenmue::s1 {
extern bool g_skipArchiveReadWait;

#if SHENMUE_IS_DEMO
struct SimmEntry {
    std::uint8_t type;
    char name[15];
    std::uint32_t offset;
    std::uint32_t size;
};

static_assert(sizeof(SimmEntry) == 0x18);
static_assert(offsetof(SimmEntry, offset) == 0x10);
static_assert(offsetof(SimmEntry, size) == 0x14);

extern std::int32_t g_simmError;
inline constexpr std::size_t kSimmStartupBytes = 0x190000;
extern std::array<std::byte, kSimmStartupBytes> g_simmStartupData;
void SIMM_LoadStartupData();
extern std::array<const void *, 2> g_simmRoots;
extern std::int32_t g_simmCompressedFileActive;
std::int32_t SIMM_UppercaseAscii(std::int32_t value);
std::int32_t SIMM_CompareNames(const char *left, const char *right);
const std::byte *SIMM_GetRoot(const void *buffer);
const SimmEntry *SIMM_FindEntry(const std::byte *table, const char *name);
const std::byte *SIMM_FindDirectory(const std::byte *root, const char *path);
const SimmEntry *SIMM_ResolveFile(const std::byte *root, const char *directory, const char *filename);
std::int32_t SIMM_GetEntrySize(const void *buffer, const char *directory, const char *filename, std::uint32_t *size);
const std::byte *SIMM_GetEntryData(const void *buffer, const char *directory, const char *filename);

void *ReadLooseFile(const char *directory, const char *filename, std::int32_t token);
#endif

std::uint64_t OpenFileInternal(const char *path);
void OpenFile(FileState *file, void *destination, std::int32_t size);
void FileStatAndProcessIfExists(FileState *file);
std::uint64_t shenOpenFile(const char *directory, const char *filename);
std::int32_t PrepareDirectoryPathForFile(FileState *file, const char *directory, const char *filename);
std::int64_t shenLoadFile(const char *directory, const char *filename, void *destination, std::int32_t size);
void *ReadFile(const char *directory, const char *filename, std::int32_t token);

} // namespace shenmue::s1
