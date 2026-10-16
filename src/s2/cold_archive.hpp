#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct ColdArchiveSection {
    char name[8];
    char type[4];
    std::uint32_t offset;
    std::uint32_t size;
};

static_assert(sizeof(ColdArchiveSection) == 0x14);
static_assert(offsetof(ColdArchiveSection, offset) == 0x0c);

bool LoadColdArchive();
void FreeColdArchive();
std::span<const std::byte> GetColdArchiveSection(std::uint32_t index);
void *CopyColdArchiveSection(std::uint32_t index, bool persistent, shen_name allocationName);
const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index);
std::uint64_t GetColdArchiveSize();

} // namespace shenmue::s2
