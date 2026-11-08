#include "s2/cold_archive.hpp"

#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <cstring>

namespace shenmue::s2 {
namespace {

constexpr std::uint64_t coldAllocationSize = 0x500000;
constexpr std::uint64_t coldHeapOffset = 0xa0000;
constexpr std::uint32_t coldSectionSelectorLimit = 12;

std::byte *coldData = nullptr;
std::uint64_t coldSize = 0;
const ColdArchiveSection *coldSections = nullptr;
std::uint32_t coldSectionCount = 0;

template <typename Value> bool ReadColdValue(std::size_t offset, Value &value) {
    if (!coldData || offset > coldSize || sizeof(Value) > coldSize - offset) {
        return false;
    }
    std::memcpy(&value, coldData + offset, sizeof(Value));
    return true;
}

} // namespace

bool LoadColdArchive() {
    if (coldData) {
        return true;
    }
    const std::uint64_t fileSize = ArchiveData_GetFileSize("/misc/COLD.BIN");
    if (!fileSize || fileSize > coldAllocationSize) {
        return false;
    }

    coldData = HLib::DynamicHeapBase + coldHeapOffset;
    if (!HLib::AllocateHeapSpace(coldData, coldAllocationSize, SHEN_NAME(COLD))) {
        coldData = nullptr;
        return false;
    }
    if (!ArchiveData_Read("/misc/COLD.BIN", 0, coldData, fileSize)) {
        if (coldData) {
            HLib::FreeTaskData(coldData);
            coldData = nullptr;
        }
        return false;
    }
    coldSize = fileSize;




    std::uint32_t directoryOffset = 0;
    if (!ReadColdValue(0x1c, directoryOffset)) {
        FreeColdArchive();
        return false;
    }

    const std::uint64_t tableOffset = static_cast<std::uint64_t>(directoryOffset) + 0x10;
    if (tableOffset > coldSize) {
        FreeColdArchive();
        return false;
    }
    coldSectionCount = static_cast<std::uint32_t>((coldSize - tableOffset) / sizeof(ColdArchiveSection));
    if (coldSectionCount > coldSectionSelectorLimit) {
        coldSectionCount = coldSectionSelectorLimit;
    }
    if (coldSectionCount == 0) {
        FreeColdArchive();
        return false;
    }
    coldSections = reinterpret_cast<const ColdArchiveSection *>(coldData + tableOffset);
    for (std::uint32_t index = 0; index < coldSectionCount; ++index) {
        const ColdArchiveSection &section = coldSections[index];
        const std::uint64_t sourceOffset = static_cast<std::uint64_t>(section.offset) + 0x10;
        if (sourceOffset > coldSize || section.size > coldSize - sourceOffset) {
            FreeColdArchive();
            return false;
        }
    }
    return true;
}

void FreeColdArchive() {
    if (coldData) {
        HLib::FreeTaskData(coldData);
    }
    coldData = nullptr;
    coldSize = 0;
    coldSections = nullptr;
    coldSectionCount = 0;
}

const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index) {
    return coldSections && index < coldSectionCount ? &coldSections[index] : nullptr;
}

std::span<const std::byte> GetColdArchiveSection(std::uint32_t index) {
    const ColdArchiveSection *const section = GetColdArchiveSectionDescriptor(index);
    return section ? std::span<const std::byte>(coldData + 0x10 + section->offset, section->size) : std::span<const std::byte>{};
}

void *CopyColdArchiveSection(std::uint32_t index, bool persistent, shen_name allocationName) {
    const std::span<const std::byte> section = GetColdArchiveSection(index);
    if (section.empty()) {
        return nullptr;
    }
    void *const destination =
        persistent ? HLib::AllocateStaticOrDynamicBlock(section.size(), allocationName) : HLib::AllocateTaskData(section.size(), allocationName);
    if (!destination) {
        return nullptr;
    }
    std::memcpy(destination, section.data(), section.size());
    return destination;
}

std::uint64_t GetColdArchiveSize() {
    return coldSize;
}

} // namespace shenmue::s2
