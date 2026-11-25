#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/cold_archive_dc.hpp"

#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/event_overlay.hpp"
#else
#include "s2/cold_archive.hpp"

#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

constexpr std::uint64_t coldAllocationSize = 0x500000;

constexpr std::uint64_t coldHeapOffset = 0xa0000;

constexpr std::uint32_t coldSectionSelectorLimit = 12;

std::byte *coldData = nullptr;

std::uint64_t coldSize = 0;

const ColdArchiveSection *coldSections = nullptr;

std::uint32_t coldSectionCount = 0;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void FreeColdArchive();
const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index);
void *CopyColdArchiveSection(std::uint32_t index, bool persistent, shen_name allocationName);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <typename Value> bool ReadColdValue(std::size_t offset, Value &value) {
    if (!coldData || offset > coldSize || sizeof(Value) > coldSize - offset) {
        return false;
    }
    std::memcpy(&value, coldData + offset, sizeof(Value));
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void LoadColdStorageFromMisc() {
    void *const memory = reinterpret_cast<void *>(kColdArchiveAddress);
    HLib::AllocateHeapSpace(memory, kColdArchiveAllocationBytes, COLD_ARCHIVE);
    shenLoadFile("misc", "COLD.BIN", memory, UINT32_MAX);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FreeColdArchive() {
    HLib::FreeTaskData(reinterpret_cast<void *>(kColdArchiveAddress));
}

#else

void FreeColdArchive() {
    if (coldData) {
        HLib::FreeTaskData(coldData);
    }
    coldData = nullptr;
    coldSize = 0;
    coldSections = nullptr;
    coldSectionCount = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index) {
    if (index >= kColdArchiveSectionCount)
        return 0;
    const std::byte *const base = reinterpret_cast<const std::byte *>(kColdArchiveAddress + 0x10);
    const std::uint32_t directoryOffset = *reinterpret_cast<const std::uint32_t *>(base + 0x0c);
    return reinterpret_cast<const ColdArchiveSection *>(base + directoryOffset) + index;
}

#else

const ColdArchiveSection *GetColdArchiveSectionDescriptor(std::uint32_t index) {
    return coldSections && index < coldSectionCount ? &coldSections[index] : nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const std::byte> GetColdArchiveSection(std::uint32_t index) {
    const ColdArchiveSection *const section = GetColdArchiveSectionDescriptor(index);
    return section ? std::span<const std::byte>(coldData + 0x10 + section->offset, section->size) : std::span<const std::byte>{};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void *CopyColdArchiveSection(std::uint32_t index, bool persistent, shen_name allocationName) {
    if (index >= kColdArchiveSectionCount)
        return 0;
    const std::byte *base = reinterpret_cast<const std::byte *>(kColdArchiveAddress + 0x10);
    const ColdArchiveSection *const section =
        reinterpret_cast<const ColdArchiveSection *>(base + *reinterpret_cast<const std::uint32_t *>(base + 0x0c)) + index;
    const std::byte *const source = base + section->offset;
    const std::uint32_t size = section->size;
    void *memory;
    if (persistent)
        memory = HLib::AllocateStaticOrDynamicBlock(size, allocationName);
    else
        memory = HLib::AllocateTaskData(size, allocationName);
    CopyEventOverlayAligned(memory, source, size);
    return memory;
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t GetColdArchiveSize() {
    return coldSize;
}

#endif

} // namespace s2
} // namespace shenmue
