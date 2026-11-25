#pragma once

#include "common/types.hpp"

#include <stddef.h>
#if !SHENMUE_IS_DC
#include <span>
#endif

namespace shenmue { namespace s2 {

struct ColdArchiveSection {
    char name[8];
    char type[4];
    std::uint32_t offset;
    std::uint32_t size;
};

typedef char ColdArchiveSectionSizeCheck[sizeof(ColdArchiveSection) == 0x14 ? 1 : -1];
typedef char ColdArchiveSectionOffsetCheck[offsetof(ColdArchiveSection, offset) == 0x0c ? 1 : -1];

bool LoadColdArchive();
void FreeColdArchive();
#if !SHENMUE_IS_DC
std::span<const std::byte> GetColdArchiveSection(std::uint32_t index);
#endif
void *CopyColdArchiveSection(std::uint32_t index, bool persistent, shen_name allocationName);
const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index);
std::uint64_t GetColdArchiveSize();

} } // namespace shenmue::s2
