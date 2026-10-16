#include "s2/afs.hpp"

#include "s2/hlib.hpp"

#include <bit>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {
namespace {
AfsArchiveSlot afsArchiveSlots[12]{};
std::int32_t firstAfsArchiveSlot{};
std::uint64_t AfsDirectorySize(std::int32_t capacity) {
    const std::int32_t entries = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(capacity) + 1U);
    return static_cast<std::uint64_t>(2LL * entries + 278) & ~3ULL;
}
}

// @verified - p
void InitializeAfsArchiveSlots() {
    for (std::int32_t index = firstAfsArchiveSlot; index < 12; ++index)
        afsArchiveSlots[index].index = -1;
}

// @verified - p
void *AllocateAfsDirectoryMemory(std::int32_t capacity) {
    return HLib::AllocateTaskData(AfsDirectorySize(capacity), MEMORY_ADFS);
}

// @verified - p
void AfsReadNullsub(const void *) {
}

void FreeAfsArchive(AfsArchiveSlot *slot) {
    HLib::FreeTaskData(slot->directoryMemory);
    slot->index = -1;
}

// @verified - p
AfsArchiveSlot *LoadAfsArchive(const char *directory, const char *filename, std::int32_t capacity, void *memory) {
    std::int32_t index = 0;
    while (index < 12 && afsArchiveSlots[index].index != -1)
        ++index;
    if (index >= 12)
        return nullptr;
    AfsArchiveSlot &slot = afsArchiveSlots[index];
    slot.index = index;
    slot.directoryMemory = memory ? memory : AllocateAfsDirectoryMemory(capacity);
    std::memset(slot.directoryMemory, 0, static_cast<std::size_t>(AfsDirectorySize(capacity)));
    char normalized[512];
    std::strcpy(normalized, directory);
    for (char *character = normalized; *character; ++character)
        if (*character == '/')
            *character = '\\';
    char path[260];
    std::sprintf(path, "%s\\%s", normalized, filename);
    if (d3t::LargeFile_LoadADFSArchiveToc(index, path) != 0)
        return nullptr;
    AfsReadNullsub(nullptr);
    return &slot;
}

// @verified - p
std::uint64_t GetAfsFileLoadState(const d3t::LargeFileHandle *file) {
    if (!file || static_cast<std::uint32_t>(file->entryIndex) >= 2048U)
        return 3;
    const d3t::LargeFileDataEntry &entry = d3t::g_largeFileData[static_cast<std::size_t>(file->entryIndex)];
    if (entry.inUse == 0)
        return 3;
    return d3t::Archive_HasPendingRead(entry.archiveHandle) ? 2 : 3;
}
} // namespace shenmue::s2
