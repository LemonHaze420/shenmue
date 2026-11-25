#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <array>
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue {
namespace s1 {

#if SHENMUE_S2_GCC295
enum DiscFileFlag {
#else
enum DiscFileFlag : std::uint32_t {
#endif
    DiscFileUnknown01 = 0x01,
    DiscFileInUse = 0x02,
    DiscFileMemory = 0x04,
    DiscFileReading = 0x08,
    DiscFileCompressed = 0x10,
    DiscFileBuffered = 0x20,
    DiscFileUnalignedStart = 0x40,
    DiscFileUnalignedEnd = 0x80,
};

struct FileState {
#if SHENMUE_S2_GCC295
    volatile std::uint32_t flags;
#else
    std::uint32_t flags;
#endif
    std::uint32_t size;
    std::uint32_t remaining;
    void *handle;
    std::uint32_t pendingBytes;
    std::byte *destination;
    std::byte *sectorBuffer;
};

#if SHENMUE_S2_GCC295
typedef char DiscFileStateSizeCheck[sizeof(FileState) == 0x1c ? 1 : -1];
typedef char DiscFileHandleOffsetCheck[offsetof(FileState, handle) == 0x0c ? 1 : -1];
typedef char DiscFileSectorBufferOffsetCheck[offsetof(FileState, sectorBuffer) == 0x18 ? 1 : -1];
#else
static_assert(sizeof(FileState) == 0x1c);
static_assert(offsetof(FileState, handle) == 0x0c);
static_assert(offsetof(FileState, sectorBuffer) == 0x18);
#endif
}
} // namespace shenmue::s1

namespace shenmue {
#if SHENMUE_IS_S2
namespace s2 {
#if SHENMUE_S2_GCC295
typedef s1::FileState FileState;
using s1::DiscFileUnknown01;
using s1::DiscFileInUse;
using s1::DiscFileMemory;
using s1::DiscFileReading;
using s1::DiscFileCompressed;
using s1::DiscFileBuffered;
using s1::DiscFileUnalignedStart;
using s1::DiscFileUnalignedEnd;
#else
using FileState = s1::FileState;
using enum s1::DiscFileFlag;
#endif
#else
namespace s1 {
#endif

#if SHENMUE_S2_GCC295
extern FileState g_files[4];
#else
extern std::array<FileState, 4> g_files;
#endif
extern std::int32_t g_discDirectoryError;
extern std::int32_t g_discFileResult;
extern std::int32_t g_simmCompressedFileActive;
FileState *Archive_GetNextUnusedFileState();
void CloseFile(FileState *file);
void RequestDiscRead(FileState *file, std::byte *destination, std::uint32_t bytes);
void ProcessDiscFileReads();
void WaitForDiscCommand();
void WaitForDiscFileReads();
}
} // namespace shenmue

#endif
