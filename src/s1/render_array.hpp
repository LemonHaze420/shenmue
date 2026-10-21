#pragma once

#include "s1/d3t/namespace.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct RenderArrayEntry;



struct RenderArrayOwner {
    std::uintptr_t handle;
    std::uint32_t instanceHandle;
    std::uint32_t loadJob;
    std::uint32_t renderHandle;
    std::uint32_t flags;
    std::uint32_t state;
    std::uint32_t remapType;
    char path[0x118];
    std::uint32_t pathHash;
    std::uint32_t referenceCount;
    std::uint8_t retire;
    std::byte reserved_141[7];
    RenderArrayEntry **children;
    std::uint32_t childCount;
    std::uint32_t childCapacity;
    std::uint64_t resolveArgument;
    void (*resolveCallback)(std::uint64_t argument, RenderArrayOwner *object);
};



struct RenderArrayEntry {
    std::uintptr_t modelData;
    RenderArrayOwner *owner;
    std::uintptr_t handle;
    std::uint32_t state;
    std::uint8_t flags;
    std::byte reserved_1d[0xa7];
    std::uint32_t cewpInitialized;
    std::byte rigState[0x84f8];
    std::uint64_t *headRigNodes;
    std::uint32_t reflectionSlot;
    Vector2 reflectionSize;
    Vector2 reflectionOffset;
    Vector2 reflectionMinimum;
    Vector2 reflectionMaximum;
    std::uint32_t id;
    std::uint32_t retire;
    std::byte reserved_85f4[4];
};




#pragma pack(push, 4)
struct EffectProbeSlot {
    RenderArrayEntry *owner;
    std::byte faceData[0x540];
    std::uint64_t parameter08;
    std::uint32_t parameter10;
    std::uint64_t parameter14;
    std::uint32_t parameter1c;
    std::uint64_t cacheHandle;
    std::uint64_t node;
    std::uint32_t parameter2c;
    float parameter20[2];
    float parameter28;
    float parameter00;
    float parameter04;
    std::byte reserved_588[4];
    std::uint8_t parameter30;
    std::byte reserved_58d[3];
};
#pragma pack(pop)


struct HmdlEntryStorage {
    RenderArrayEntry entries[1024];
    std::uint32_t generations[1024];
    std::uint32_t cursor;
    std::byte reserved_217f004[4];
};


struct HmdlEntryLookup {
    std::uint32_t values[1024];
    std::uint32_t hashes[1024];
    std::uint16_t probes[1024];
    std::uint32_t count;
};


struct HmdlObjectStorage {
    RenderArrayOwner objects[1024];
    std::uint32_t hashes[1024];
    std::uint16_t probes[1024];
    std::uint32_t count;
    std::byte reserved_5b804[4];
};


struct HmdlHandleRemap {
    std::uintptr_t values[0x4000];
    std::uint32_t hashes[0x4000];
    std::uint16_t probes[0x4000];
    std::uint32_t count;
    std::byte reserved_38004[4];
};

static_assert(offsetof(EffectProbeSlot, parameter08) == 0x548);
static_assert(offsetof(EffectProbeSlot, cacheHandle) == 0x560);
static_assert(offsetof(EffectProbeSlot, node) == 0x568);
static_assert(offsetof(EffectProbeSlot, parameter2c) == 0x570);
static_assert(offsetof(EffectProbeSlot, parameter00) == 0x580);
static_assert(offsetof(EffectProbeSlot, parameter30) == 0x58c);
static_assert(sizeof(EffectProbeSlot) == 0x590);
static_assert(offsetof(RenderArrayOwner, flags) == 0x14);
static_assert(offsetof(RenderArrayOwner, state) == 0x18);
static_assert(offsetof(RenderArrayOwner, pathHash) == 0x138);
static_assert(offsetof(RenderArrayOwner, referenceCount) == 0x13c);
static_assert(offsetof(RenderArrayOwner, retire) == 0x140);
static_assert(offsetof(RenderArrayOwner, children) == 0x148);
static_assert(offsetof(RenderArrayOwner, childCount) == 0x150);
static_assert(offsetof(RenderArrayOwner, resolveArgument) == 0x158);
static_assert(sizeof(RenderArrayOwner) == 0x168);
static_assert(offsetof(RenderArrayEntry, owner) == 0x08);
static_assert(offsetof(RenderArrayEntry, handle) == 0x10);
static_assert(offsetof(RenderArrayEntry, state) == 0x18);
static_assert(offsetof(RenderArrayEntry, flags) == 0x1c);
static_assert(offsetof(RenderArrayEntry, cewpInitialized) == 0xc4);
static_assert(offsetof(RenderArrayEntry, rigState) == 0xc8);
static_assert(offsetof(RenderArrayEntry, headRigNodes) == 0x85c0);
static_assert(offsetof(RenderArrayEntry, reflectionSlot) == 0x85c8);
static_assert(offsetof(RenderArrayEntry, reflectionSize) == 0x85cc);
static_assert(offsetof(RenderArrayEntry, reflectionOffset) == 0x85d4);
static_assert(offsetof(RenderArrayEntry, reflectionMinimum) == 0x85dc);
static_assert(offsetof(RenderArrayEntry, reflectionMaximum) == 0x85e4);
static_assert(offsetof(RenderArrayEntry, id) == 0x85ec);
static_assert(offsetof(RenderArrayEntry, retire) == 0x85f0);
static_assert(sizeof(RenderArrayEntry) == 0x85f8);
static_assert(offsetof(HmdlEntryStorage, generations) == 0x217e000);
static_assert(offsetof(HmdlEntryStorage, cursor) == 0x217f000);
static_assert(sizeof(HmdlEntryStorage) == 35123208);
static_assert(offsetof(HmdlEntryLookup, hashes) == 0x1000);
static_assert(offsetof(HmdlEntryLookup, probes) == 0x2000);
static_assert(offsetof(HmdlEntryLookup, count) == 0x2800);
static_assert(sizeof(HmdlEntryLookup) == 10244);
static_assert(offsetof(HmdlObjectStorage, hashes) == 0x5a000);
static_assert(offsetof(HmdlObjectStorage, probes) == 0x5b000);
static_assert(offsetof(HmdlObjectStorage, count) == 0x5b800);
static_assert(sizeof(HmdlObjectStorage) == 374792);
static_assert(offsetof(HmdlHandleRemap, hashes) == 0x20000);
static_assert(offsetof(HmdlHandleRemap, probes) == 0x30000);
static_assert(offsetof(HmdlHandleRemap, count) == 0x38000);
static_assert(sizeof(HmdlHandleRemap) == 0x38008);

extern std::uint8_t g_hmdlEntryDirty;
extern void *g_renderObjectStorage;
extern void *g_renderLookupStorage;
extern void *g_renderPacketStorage;
extern void *g_renderIndexStorageA;
extern void *g_renderIndexStorageB;
extern EffectProbeSlot g_effectProbeSlots[32];

RenderArrayEntry *CheckRenderArrayEntryGeneration(std::uint32_t value);
RenderArrayEntry *GetRenderArrayEntry(std::uint32_t index);
std::byte *ResolveHmdlEntry(std::uintptr_t handle);
std::byte *ResolveHmdlRuntimeObject(std::uintptr_t handle);
bool ReleaseHmdlRuntimeObject(std::uintptr_t handle);
RenderArrayOwner BuildHmdlRuntimeObjectRecord(std::uintptr_t handle, const char *path, std::uint32_t instanceHandle, std::uint16_t flags);
namespace d3t {
bool FileRead_ALL(std::uintptr_t handle, const char *path, std::uint32_t instanceHandle, std::uint16_t flags,
    void (*resolveCallback)(std::uint64_t argument, RenderArrayOwner *object) = nullptr);
} // namespace d3t
bool RemoveHmdlRuntimeObject(std::uint32_t hash);
bool InsertLargeHmdlMapping(void *mappingStorage, std::uintptr_t handle, std::uintptr_t target);
bool SetupEngineObject(std::uintptr_t modelFileData, std::uintptr_t modelData, std::uint8_t mode);
void *InitChara(void *modelFileData, void *sourceModel);
std::uint8_t ProcessHmdlResourceTransitions(std::int32_t setupShaders);
void Shenmue_Free();

} // namespace shenmue::s1
