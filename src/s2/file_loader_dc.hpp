#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstdint>
#include "common/types.hpp"
#include "platform/dc/disc.hpp"
#include "platform/dc/simm.hpp"

namespace shenmue {
namespace s2 {

using ::shenmue::shen_name;
#define COMPRESSED_FILE_WORK shen_name(SHEN_NAME4('G', 'Z', 'I', 'P'))

struct DiscFileLocationData {
    std::uint32_t size;
    char filename[32];
};
struct MemoryFileLocationData {
    const std::byte *data;
    std::uint32_t size;
    char filename[28];
};
struct DiscFileLocation {
    std::uint32_t kind;
    std::uint32_t sectorOrCompression;
    union {
        DiscFileLocationData disc;
        MemoryFileLocationData memory;
    };
};
typedef char DiscFileLocationSizeCheck[sizeof(DiscFileLocation) == 0x2c ? 1 : -1];
typedef char DiscFileLocationDiscSizeOffsetCheck[offsetof(DiscFileLocation, disc.size) == 8 ? 1 : -1];
typedef char DiscFileLocationDiscNameOffsetCheck[offsetof(DiscFileLocation, disc.filename) == 0x0c ? 1 : -1];
typedef char DiscFileLocationMemorySizeOffsetCheck[offsetof(DiscFileLocation, memory.size) == 0x0c ? 1 : -1];
typedef char DiscFileLocationMemoryNameOffsetCheck[offsetof(DiscFileLocation, memory.filename) == 0x10 ? 1 : -1];

extern const void *g_simmRoots[2];
extern std::int32_t g_ryoModelVariant;
extern char g_lastFileDirectory[32];
extern char g_lastFileName[16];

void ResetDiscAndExit();
void CheckDiscTray();
void DecompressMemoryFile(const void *source, void *destination, std::uint32_t outputBytes);
void WaitForBackupMemoryIdle();
void InitializeDiscFileSystem();
void CloseAllFiles();
std::int32_t IsAnyDiscFileReading();
bool IsDiscWorkHandleBusy();
void ResetSceneArchiveFileHandles();
std::int32_t SelectDiscDirectoryChange(const char *directory);
std::int32_t SelectDiscDirectoryDefault(const char *directory);
std::int32_t SelectDiscDirectory(const char *directory, std::int32_t mode = 0);
std::uint32_t GetDiscFileLocationSize(const DiscFileLocation *location);
void ReleaseDiscFileLocation(DiscFileLocation *location);
void NormalizeDiscFilename(const char *filename, char *destination);
DiscFileLocation *FindDiscFileLocation(const char *directory, const char *filename);
std::int32_t OpenFileFromLocation(FileState *file, const DiscFileLocation *location, std::int32_t sectorOffset, std::uint32_t maximumBytes);
std::int32_t OpenFile(FileState *file, const char *directory, const char *filename);
std::uint32_t CopyFileToHLibMemory(FileState *file, void *destination, std::uint32_t bytes);
void FileStatAndProcessIfExists(FileState *file);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int IsFileReadIdle(const FileState *file);
#else
bool IsFileReadIdle(const FileState *file);
#endif
std::int32_t SeekFile(FileState *file, std::int32_t offset, std::int32_t origin);
std::uint32_t GetFilePosition(const FileState *file);
std::uint32_t GetFileSize(const FileState *file);
std::uint32_t shenOpenFile(const char *directory, const char *filename);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int shenFileExists(const char *directory, const char *filename);
#else
bool shenFileExists(const char *directory, const char *filename);
#endif
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);
void *SafeTryLoadFile(const char *directory, const char *filename, shen_name allocationName, std::uint32_t requiredHeapBytes);
void *shenLoadCompressedFile(const char *directory, const char *filename, shen_name allocationName);
void *ReadFile(const char *directory, const char *filename, shen_name allocationName);

} // namespace s2
} // namespace shenmue

#endif
