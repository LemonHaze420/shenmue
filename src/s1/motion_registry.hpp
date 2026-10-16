#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct MotnFileNode {
    std::int32_t handle;
    std::int32_t reserved_04;
    void *motionData;
    std::uint16_t firstAnimId;
    std::uint16_t endAnimId;
    std::uint32_t reserved_14;
    MotnFileNode *previous;
    MotnFileNode *next;
    std::int32_t motionDataSize;
    std::int32_t reserved_2c;
};

static_assert(sizeof(MotnFileNode) == 0x30);
static_assert(offsetof(MotnFileNode, motionData) == 0x08);
static_assert(offsetof(MotnFileNode, firstAnimId) == 0x10);
static_assert(offsetof(MotnFileNode, previous) == 0x18);
static_assert(offsetof(MotnFileNode, next) == 0x20);

struct PackSlot;


struct HandMotionAllocation {
    std::byte *data;
    std::int16_t firstSequence;
    std::int16_t lastSequence;
    std::uint32_t reserved0c;
    HandMotionAllocation *next;
    HandMotionAllocation **previousLink;
    PackSlot *packOwner;
    std::size_t dataSize;
    std::byte reserved30[0x10];
};
static_assert(sizeof(HandMotionAllocation) == 0x40);

extern HandMotionAllocation *g_handMotionHead;

struct HandMotionSequence {
    const std::byte *record;
    const void *curves;
    std::uint32_t frameCount;
    std::uint16_t jointCount;
};
static_assert(offsetof(MotnFileNode, motionDataSize) == 0x28);

void MOTN_ResetFileRegistry();
void MOTN_ResetMapFileRegistrations();
void HND_ResetMotionRegistry();
std::int32_t MOTN_ReadMOTNHeader(void *motionData);
void MOTN_SetBaseMotionData(void *motionData);
std::int32_t MOTN_AllocateMOTN(std::int32_t sceneId, std::uint32_t areaId, const char *filename);
std::uint32_t LoadHandMot(std::int32_t sceneId, std::uint32_t areaId, const char *filename);
bool HND_FindMotionSequence(std::int16_t sequenceId, HandMotionSequence &sequence);
const void *HND_ResolveMotionOffset(std::uint32_t offset, std::size_t size);
std::int64_t ReallocateUNFO(std::int32_t handle);
std::int64_t AllocateUNFO_2(std::int32_t handle);
bool HND_UnregisterMotion(std::uint32_t handle);

MotnFileNode *MOTN_GetFileRegistryHead();
std::int32_t MOTN_GetRegisteredFileSize();

} // namespace shenmue::s1
