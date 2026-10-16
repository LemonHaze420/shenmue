#pragma once

#include "d3t/archive_handles.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name MEMORY_ADFS{SHEN_NAME(ADFS)};

struct AfsArchiveSlot {
    std::int32_t index;
    std::uint32_t reserved04;
    void *directoryMemory;
    std::uint64_t reserved10;
};
static_assert(sizeof(AfsArchiveSlot) == 0x18 && offsetof(AfsArchiveSlot, directoryMemory) == 0x08);
void InitializeAfsArchiveSlots();
AfsArchiveSlot *LoadAfsArchive(const char *directory, const char *filename, std::int32_t capacity, void *memory = nullptr);
void *AllocateAfsDirectoryMemory(std::int32_t capacity);

void FreeAfsArchive(AfsArchiveSlot *slot);
std::uint64_t GetAfsFileLoadState(const d3t::LargeFileHandle *file);
void AfsReadNullsub(const void *file);
} // namespace shenmue::s2
