#include "s1/motion_registry.hpp"

#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/task/motf.hpp"
#include "s1/task/pack.hpp"

#include <array>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {
extern void *g_handNodePoolSecondary;
extern void *g_handCollisionPool;

HandMotionAllocation *g_handMotionHead = nullptr;

namespace {

constexpr std::size_t kMotnFileSlotCount = 32;

std::array<MotnFileNode, kMotnFileSlotCount> g_motnFiles{};
MotnFileNode *g_motnFileHead = g_motnFiles.data();
std::int32_t g_registeredFileSize = 0;
void releaseHandMotionData(HandMotionAllocation *owner) {
    if (owner && owner->data && !owner->packOwner) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(owner->data), 0);
    }
}

std::size_t allocatedPayloadSize(const std::byte *data) {
    if (!data)
        return 0;
    const auto *const allocation = reinterpret_cast<const HLib::TaskData *>(data) - 1;
    if (allocation->status != static_cast<std::int32_t>(make_shen_name('U', 'S', 'E', ' ')) ||
        allocation->content_size <= static_cast<std::int64_t>(sizeof(HLib::TaskData))) {
        return 0;
    }
    return static_cast<std::size_t>(allocation->content_size - sizeof(HLib::TaskData));
}

std::byte *loadHandMotionData(std::int32_t sceneId, std::uint32_t areaId, const char *filename, PackSlot *&packOwner, std::size_t &dataSize) {
    packOwner = nullptr;
    dataSize = 0;
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    if (pack && pack->usePackCache && pack->readyPack && pack->readyPack->pakData && pack->readyPack->pakSize) {
        PackSlot *const slot = pack->readyPack;
        auto *const data = static_cast<std::byte *>(LoadFileIntoHeapMem(slot, filename, make_shen_name('B', 'I', 'N', ' ')));
        const auto packBegin = reinterpret_cast<std::uintptr_t>(slot->pakData);
        const auto packEnd = packBegin + slot->pakSize;
        const auto dataAddress = reinterpret_cast<std::uintptr_t>(data);
        if (dataAddress >= packBegin && dataAddress < packEnd) {
            packOwner = slot;
            dataSize = static_cast<std::size_t>(packEnd - dataAddress);
            return data;
        }
    }

    char directory[64]{};
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", sceneId, static_cast<char>(areaId), static_cast<char>(areaId >> 8),
        static_cast<char>(areaId >> 16), static_cast<char>(areaId >> 24));
    auto *const data = static_cast<std::byte *>(ReadFile(directory, filename, SHEN_NAME(HNDM)));
    dataSize = allocatedPayloadSize(data);
    if (data && !dataSize) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
        return nullptr;
    }
    return data;
}

void resetHandMotionAllocations() {
    while (g_handMotionHead) {
        HandMotionAllocation *const owner = g_handMotionHead;
        g_handMotionHead = owner->next;
        releaseHandMotionData(owner);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(owner), 0);
    }
}

std::int64_t removeRegisteredFile(std::int32_t handle, bool freeMotionData) {
    if (handle == 0) {
        return 0;
    }

    MotnFileNode *node = g_motnFileHead;
    while (node->handle != handle) {
        node = node->next;
        if (!node) {
            return 0;
        }
    }

    std::memset(node->motionData, 0, static_cast<std::size_t>(node->motionDataSize));
    if (freeMotionData) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(node->motionData), 0);
    }

    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node->previous) {
        node->previous->next = node->next;
    }
    g_registeredFileSize -= node->motionDataSize;

    MOTF_RegisterInvalidatedRange(node->firstAnimId, node->endAnimId);

    for (std::size_t index = 1; index < g_motnFiles.size(); ++index) {
        if (node == &g_motnFiles[index]) {
            g_motnFiles[index] = {};
            g_motnFiles[index].handle = -1;
            break;
        }
    }
    return 1;
}

struct MotnRegistryInitializer {
    MotnRegistryInitializer() {
        MOTN_ResetFileRegistry();
    }
} g_motnRegistryInitializer;

} // namespace

void MOTN_ResetFileRegistry() {
    resetHandMotionAllocations();
    g_motnFiles = {};
    for (std::size_t index = 1; index < g_motnFiles.size(); ++index) {
        g_motnFiles[index].handle = -1;
    }
    g_motnFileHead = g_motnFiles.data();
    g_registeredFileSize = 0;
}

void MOTN_ResetMapFileRegistrations() {


    for (std::size_t index = 1; index < g_motnFiles.size(); ++index) {
        g_motnFiles[index] = {};
        g_motnFiles[index].handle = -1;
    }
    g_motnFileHead->next = nullptr;
}
void MOTN_SetBaseMotionData(void *motionData) {
    MotnFileNode &base = g_motnFiles[0];
    g_registeredFileSize -= base.motionDataSize;
    base.motionData = motionData;
    base.firstAnimId = 0;
    base.endAnimId = 0;
    base.motionDataSize = 0;
    if (motionData) {
        const auto *const bytes = static_cast<const std::byte *>(motionData);
        base.firstAnimId = 0;
        std::memcpy(&base.endAnimId, bytes + 0x0c, sizeof(base.endAnimId));
        std::memcpy(&base.motionDataSize, bytes + 0x10, sizeof(base.motionDataSize));
    }
    g_registeredFileSize += base.motionDataSize;
}

void HND_ResetMotionRegistry() {
    resetHandMotionAllocations();
}

std::int32_t MOTN_ReadMOTNHeader(void *motionData) {
    if (!motionData)
        return 0;
    MotnFileNode *tail = g_motnFileHead;
    while (tail->next) {
        tail = tail->next;
    }

    std::size_t index = 1;
    while (index < g_motnFiles.size() && g_motnFiles[index].handle != -1) {
        ++index;
    }
    if (index == g_motnFiles.size())
        return 0;
    MotnFileNode *const node = &g_motnFiles[index];

    tail->next = node;
    node->previous = tail;
    node->handle = tail->handle + 1;
    node->next = nullptr;
    node->motionData = motionData;

    const auto *const bytes = static_cast<const std::byte *>(motionData);
    std::memcpy(&node->firstAnimId, bytes + 0x14, sizeof(node->firstAnimId));
    std::memcpy(&node->endAnimId, bytes + 0x18, sizeof(node->endAnimId));
    std::memcpy(&node->motionDataSize, bytes + 0x10, sizeof(node->motionDataSize));
    g_registeredFileSize += node->motionDataSize;
    return node->handle;
}

std::int32_t MOTN_AllocateMOTN(std::int32_t sceneId, std::uint32_t areaId, const char *filename) {
    char directory[64]{};
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", sceneId, static_cast<char>(areaId), static_cast<char>(areaId >> 8),
        static_cast<char>(areaId >> 16), static_cast<char>(areaId >> 24));
    void *const data = ReadFile(directory, filename, SHEN_NAME(MOTI));
    const std::int32_t handle = MOTN_ReadMOTNHeader(data);
    if (data && !handle) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(data), 0);
    }
    return handle;
}

std::uint32_t LoadHandMot(std::int32_t sceneId, std::uint32_t areaId, const char *filename) {
    if (!g_handNodePoolSecondary || !g_handCollisionPool)
        return 0;

    PackSlot *packOwner = nullptr;
    std::size_t size = 0;
    std::byte *const data = loadHandMotionData(sceneId, areaId, filename, packOwner, size);
    if (!data || size < 8) {
        if (data && !packOwner) {
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
        }
        return 0;
    }
    std::int32_t packedRange{};
    std::uint32_t firstOffset{};
    std::memcpy(&packedRange, data, sizeof(packedRange));
    std::memcpy(&firstOffset, data + 4, sizeof(firstOffset));
    const std::uint32_t correction = packedRange < 0 ? 0xffffu : 0u;
    const std::uint32_t adjusted = static_cast<std::uint32_t>(packedRange) + correction;
    const std::int16_t first = static_cast<std::int16_t>(adjusted >> 16u);
    const std::int16_t last = static_cast<std::int16_t>(static_cast<std::uint16_t>(adjusted) - static_cast<std::uint16_t>(correction));
    const std::int32_t count = static_cast<std::int32_t>(last) - static_cast<std::int32_t>(first) + 1;

    auto *const owner = static_cast<HandMotionAllocation *>(HLib_AllocMemBlock(sizeof(HandMotionAllocation), SHEN_NAME(HNDM)));
    if (!owner) {
        if (!packOwner) {
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
        }
        return 0;
    }
    std::memset(owner, 0, sizeof(*owner));
    owner->data = data;
    owner->firstSequence = first;
    owner->lastSequence = last;
    owner->packOwner = packOwner;
    owner->dataSize = size;
    HandMotionAllocation **link = &g_handMotionHead;
    while (*link)
        link = &(*link)->next;
    *link = owner;
    owner->previousLink = link;

    const std::size_t tableBytes = size >= sizeof(std::uint32_t) ? size - sizeof(std::uint32_t) : 0;
    if ((firstOffset & 0xffff0000U) == 0 && count > 0 && static_cast<std::uint64_t>(count) <= tableBytes / 4) {
        const std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(g_pCurrentTaskData);
        for (std::int32_t index = 0; index < count; ++index) {
            std::uint32_t relative{};
            std::memcpy(&relative, data + 4 + index * 4, sizeof(relative));
            if (relative >= size)
                continue;

            std::byte *const record = data + relative;
            const std::uintptr_t recordAddress = reinterpret_cast<std::uintptr_t>(record);
            if (recordAddress < baseAddress || recordAddress - baseAddress > UINT32_MAX) {
                continue;
            }
            const auto storedOffset = static_cast<std::uint32_t>(recordAddress - baseAddress);
            std::memcpy(data + 4 + index * 4, &storedOffset, sizeof(storedOffset));

            constexpr std::size_t kSequenceHeaderSize = 0x28;
            const std::size_t recordOffset = static_cast<std::size_t>(relative);
            if (kSequenceHeaderSize > size - recordOffset) {
                continue;
            }
            std::uint16_t jointCount{};
            std::memcpy(&jointCount, record + 0x24, sizeof(jointCount));
            const std::size_t descriptorCount = static_cast<std::size_t>(jointCount) * 3;
            if (descriptorCount > (size - recordOffset - kSequenceHeaderSize) / 8) {
                continue;
            }
            std::byte *const descriptorBase = record + kSequenceHeaderSize;
            for (std::size_t descriptor = 0; descriptor < descriptorCount; ++descriptor) {
                std::int32_t present{};
                std::uint32_t nested{};
                std::byte *const entry = descriptorBase + descriptor * 8;
                std::memcpy(&present, entry, sizeof(present));
                std::memcpy(&nested, entry + 4, sizeof(nested));
                if (present <= 0 || nested > size - recordOffset - kSequenceHeaderSize) {
                    continue;
                }
                std::byte *const nestedAddress = descriptorBase + nested;
                const std::uintptr_t nestedPointer = reinterpret_cast<std::uintptr_t>(nestedAddress);
                if (nestedPointer < baseAddress || nestedPointer - baseAddress > UINT32_MAX) {
                    continue;
                }
                const auto storedNested = static_cast<std::uint32_t>(nestedPointer - baseAddress);
                std::memcpy(entry + 4, &storedNested, sizeof(storedNested));
            }
        }
    }
    const std::uintptr_t dataAddress = reinterpret_cast<std::uintptr_t>(data);
    const std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(g_pCurrentTaskData);
    if (dataAddress < baseAddress || dataAddress - baseAddress > UINT32_MAX) {
        *owner->previousLink = owner->next;
        if (owner->next)
            owner->next->previousLink = owner->previousLink;
        releaseHandMotionData(owner);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(owner), 0);
        return 0;
    }
    return static_cast<std::uint32_t>(dataAddress - baseAddress);
}

bool HND_UnregisterMotion(std::uint32_t handle) {
    if (!handle)
        return false;

    std::byte *const data = g_pCurrentTaskData + handle;
    HandMotionAllocation *owner = g_handMotionHead;
    while (owner && owner->data != data)
        owner = owner->next;
    if (!owner)
        return false;

    *owner->previousLink = owner->next;
    if (owner->next)
        owner->next->previousLink = owner->previousLink;

    releaseHandMotionData(owner);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(owner), 0);
    return true;
}

bool HND_FindMotionSequence(std::int16_t sequenceId, HandMotionSequence &sequence) {
    sequence = {};
    for (HandMotionAllocation *owner = g_handMotionHead; owner; owner = owner->next) {
        if (sequenceId < owner->firstSequence || sequenceId > owner->lastSequence) {
            continue;
        }

        const std::size_t tableIndex = static_cast<std::size_t>(sequenceId - owner->firstSequence + 1);
        if ((tableIndex + 1) * sizeof(std::uint32_t) > owner->dataSize)
            return false;

        std::uint32_t offset{};
        std::memcpy(&offset, owner->data + tableIndex * sizeof(offset), sizeof(offset));
        if (!offset)
            return false;

        const std::byte *const record = g_pCurrentTaskData + offset;
        const std::uintptr_t recordAddress = reinterpret_cast<std::uintptr_t>(record);
        const std::uintptr_t dataAddress = reinterpret_cast<std::uintptr_t>(owner->data);
        if (recordAddress < dataAddress || recordAddress - dataAddress > owner->dataSize || 0x28 > owner->dataSize - (recordAddress - dataAddress)) {
            return false;
        }

        std::memcpy(&sequence.frameCount, record + 0x20, sizeof(sequence.frameCount));
        std::memcpy(&sequence.jointCount, record + 0x24, sizeof(sequence.jointCount));
        sequence.record = record;
        sequence.curves = record + 0x28;

        return true;
    }
    return false;
}

const void *HND_ResolveMotionOffset(std::uint32_t offset, std::size_t size) {
    if (!offset)
        return nullptr;

    const std::byte *const pointer = g_pCurrentTaskData + offset;
    const std::uintptr_t pointerAddress = reinterpret_cast<std::uintptr_t>(pointer);

    for (HandMotionAllocation *owner = g_handMotionHead; owner; owner = owner->next) {
        const std::uintptr_t dataAddress = reinterpret_cast<std::uintptr_t>(owner->data);
        if (pointerAddress >= dataAddress && pointerAddress - dataAddress <= owner->dataSize && size <= owner->dataSize - (pointerAddress - dataAddress)) {
            return pointer;
        }
    }
    return nullptr;
}

std::int64_t ReallocateUNFO(std::int32_t handle) {
    return removeRegisteredFile(handle, true);
}

std::int64_t AllocateUNFO_2(std::int32_t handle) {
    return removeRegisteredFile(handle, false);
}

MotnFileNode *MOTN_GetFileRegistryHead() {
    return g_motnFileHead;
}

std::int32_t MOTN_GetRegisteredFileSize() {
    return g_registeredFileSize;
}

} // namespace shenmue::s1
